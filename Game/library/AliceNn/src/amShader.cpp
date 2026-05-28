/*****************************************************************************/
/*      amShader.cpp                Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* シェーダーライブラリプログラム                                            */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090410-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files ---------------------------------------------------------*/

#if _PS3
#include <sys/paths.h>
#include <Cg/cgc.h>
#endif

#include "alice.h"


/*--- Macros ----------------------------------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

#if _PC | _XBOX
#include <io.h>
#include <direct.h>
#if _PC
#include "Win32/amWin.h"
#include "Win32/amWinDx.h"
#elif _XBOX
#include "Xbox360/amXboxDx.h"
#endif
#define MKDIR(_dir)		_mkdir(_dir)
#elif _PS3
#include <sys/stat.h>
#define MKDIR(_dir)		mkdir(_dir, 0755)
#endif

#if _PC | _XBOX
Sint32 _amShaderMakeFileName(NNS_STDSHADER_NAME *name, char *fname);

static Sint32 _amShaderBuildStdToFile(const char *code, Sint32 size,
		const NNE_STDSHADER_PROFILE ptype, NNS_STDSHADER_PROFILE *profile,
		const char *fname);
#endif


/*--- External Valiables ----------------------------------------------------*/

/*--- Global Variables ------------------------------------------------------*/

const char *_am_shader_path = AMD_SHADER_NOT_PRECOMPILED;

AMS_AMB_HEADER	*_am_shader_amb = NULL;


/*--- Local Variables -------------------------------------------------------*/

const char *_am_shader_32x =
	"0123456789ABCDEFGHIJKLMNOPQRSTUV";


/*--- Global Functions ------------------------------------------------------*/

/*****************************************************************************/
/* void amShaderBuildStd(const char *vs_code, Sint32 vs_size,                */
/*                       const char *ps_code, Sint32 ps_size,                */
/*                           const char *code_path, const char *shader_path) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  vs_code     : バーテックスシェーダーコード                       */
/*          vs_size     : バーテックスシェーダーコードサイズ                 */
/*          ps_code     : ピクセルシェーダーコード                           */
/*          ps_size     : ピクセルシェーダーコードサイズ                     */
/*          code_path   : シェーダーコードのパス                             */
/*          shader_path : コンパイル済みシェーダーのパス                     */
/* [FUNCTION]  標準シェーダーのビルド                                        */
/*****************************************************************************/
void amShaderBuildStd(const char *vs_code, Sint32 vs_size, const char *ps_code, Sint32 ps_size, const char *code_path, const char *shader_path)
{
#if _PC | _XBOX | _PS3
#ifdef _DEBUG
	if (vs_code != NULL)
		_am_draw_vs_code_buf	= (void *)vs_code;
	else
		vs_code		= (const char *)_am_draw_vs_code_buf;
	if (ps_code != NULL)
		_am_draw_ps_code_buf	= (void *)ps_code;
	else
		ps_code		= (const char *)_am_draw_ps_code_buf;
	if (vs_size != 0)
		_am_draw_vs_code_size	= vs_size;
	else
		vs_size		= _am_draw_vs_code_size;
	if (ps_size != 0)
		_am_draw_ps_code_size	= ps_size;
	else
		ps_size		= _am_draw_ps_code_size;
#if _PC
	if (code_path != NULL)
		_am_windx_include_path	= code_path;
	else
		code_path	= _am_windx_include_path;
#elif _XBOX
	if (code_path != NULL)
		_am_xboxdx_include_path	= code_path;
	else
		code_path	= _am_xboxdx_include_path;
#elif _PS3
	if (code_path != NULL)
		_am_ps3_include_path	= code_path;
	else
		code_path	= _am_ps3_include_path;
#endif
	if (shader_path == NULL)
		shader_path	= _am_shader_path;
	else
		_am_shader_path			= shader_path;
#endif
		
#if _PS3
#ifdef _DEBUG
	// プリコンパイルシェーダーを使用しない
//	if (shader_path == AMD_SHADER_NOT_PRECOMPILED) {
	if (1) {
		Uint8	*workbuf;
		if (_am_heap_manager[0].buf != NULL) {
//		if (0) {
			workbuf		= (Uint8 *)amMemAlloc(
					nnCalcBuildStdShaderWorkBufferSize(vs_size, ps_size));
			nnBuildStdShader(workbuf, (const Uint8 *)vs_code, vs_size,
					(const Uint8 *)ps_code, ps_size);
			amMemFree(workbuf);
		} else {
			workbuf		= (Uint8 *)amMemAllocSystem(
					nnCalcBuildStdShaderWorkBufferSize(vs_size, ps_size));
			nnBuildStdShader(workbuf, (const Uint8 *)vs_code, vs_size,
					(const Uint8 *)ps_code, ps_size);
			amMemFreeSystem(workbuf);
		}
		return;
	}
#endif
#else
	// プリコンパイルシェーダーファイル
	if (_am_shader_amb != NULL) {
		amShaderBuildStd(_am_shader_amb);
		return;
	}

#ifdef _DEBUG
	// プリコンパイルシェーダーを使用しない
	if (shader_path == AMD_SHADER_NOT_PRECOMPILED) {
		nnBuildStdShader(vs_code, vs_size, ps_code, ps_size);
		return;
	}

	NNS_STDSHADER_PROFILE	profile;
	IDirect3DDevice9		*d3ddev;
	FILE	*file;
	char	fname[MAX_PATH], *ex_name;
	char	*buf;
	Sint32	fsize;
	Sint32	system_malloc = (_am_heap_manager[0].buf == NULL)? AMD_FS_MALLOC_SYSTEM: 0;

#if _PC
	d3ddev	= amWinDxGetIDirect3DDevice();
#elif _XBOX
	d3ddev	= amXboxDxGetIDirect3DDevice();
#endif

	MKDIR(shader_path);

	while (nnGetUnbuildStdShaderProfileOne(&profile) > 0) {
		NNS_COMPILED_STDSHADER_PROFILE		shader;
		IDirect3DVertexShader9	*vs;
		IDirect3DPixelShader9	*ps;
		LPD3DXCONSTANTTABLE		vconst;
		LPD3DXCONSTANTTABLE		pconst;
		HRESULT					result;
		NNS_STDSHADER_NAME		name;

		// シェーダープロファイル
		name	= nnGetStdShaderName(&profile);
#if 0
		sprintf(fname, "%snss_%04x%04x%04x%04x.spf",
				shader_path,
				(Uint32)(name.high >> 32), (Uint32)name.high,
				(Uint32)(name.low >> 32), (Uint32)name.low);

		// ファイル書き込み
#if _PC
		if (fopen_s(&file, fname, "wb") == 0) {
#elif _XBOX
		if ((file = fopen(fname, "wb")) != NULL) {
#endif
			fsize	= sizeof(NNS_STDSHADER_PROFILE);
			buf		= (char *)&profile;
			fwrite(buf, fsize, 1, file);
			fclose(file);
		} else {
			// ファイルオープンエラー
			amSystemLog("[WARN] Write profile '%s' error.\n", fname);
		}
#endif

		// バーテックスシェーダー
//		sprintf(fname, "%snss_%04x%04x%04x%04x",
//				shader_path,
//				(Uint32)(name.high >> 32), (Uint32)name.high,
//				(Uint32)(name.low >> 32), (Uint32)name.low);
//		ex_name	= &fname[strlen(fname)];
		strcpy(fname, shader_path);
		ex_name	= &fname[strlen(fname)];
		ex_name	= &ex_name[_amShaderMakeFileName(&name, ex_name)];

		strcpy(ex_name, ".VSH");

		// ファイルがなければビルドする
#if _PC
		if (fopen_s(&file, fname, "rb") != 0) {
#elif _XBOX
		if ((file = fopen(fname, "rb")) == 0) {
#endif
			amAssert(_amShaderBuildStdToFile(vs_code, vs_size,
					NNE_STDSHADER_PROFILE_VERTEX, &profile, fname) >= 0);
		} else {
			fclose(file);
		}

		// ファイル読み込み
		buf		= NULL;
		fsize	= amFsRead(fname, (void **)&buf, system_malloc);
		amAssert(fsize > 0);

		result	= IDirect3DDevice9_CreateVertexShader(
				d3ddev, (DWORD *)buf, &vs);
		if (FAILED(result))
			NNM_ASSERT(0, "Vertex Shader Creation Failed.\n");
		D3DXGetShaderConstantTable((DWORD *)buf, &vconst);
		if (system_malloc)
			amMemFreeSystem(buf);
		else
			amMemFree(buf);

		// ピクセルシェーダー
		strcpy(ex_name, ".PSH");

		// ファイルがなければビルドする
#if _PC
		if (fopen_s(&file, fname, "rb") != 0) {
#elif _XBOX
		if ((file = fopen(fname, "rb")) == 0) {
#endif
			amAssert(_amShaderBuildStdToFile(ps_code, ps_size,
					NNE_STDSHADER_PROFILE_PIXEL, &profile, fname) >= 0);
		} else {
			fclose(file);
		}

		// ファイル読み込み
		buf		= NULL;
		fsize	= amFsRead(fname, (void **)&buf, system_malloc);
		amAssert(fsize > 0);

		result	= IDirect3DDevice9_CreatePixelShader(
				d3ddev, (DWORD *)buf, &ps);
		if (FAILED(result))
			NNM_ASSERT(0, "Pixel Shader Creation Failed.\n");
		D3DXGetShaderConstantTable((DWORD *)buf, &pconst);
		if (system_malloc)
			amMemFreeSystem(buf);
		else
			amMemFree(buf);

		// NNに登録
		shader.ShaderType		= NNE_STDSHADERTYPE_HLSL;
		shader.pVertexShader	= vs;
		shader.pPixelShader		= ps;
		shader.pVtxConstTbl		= vconst;
		shader.pPixelConstTbl	= pconst;
		nnRegistCompiledStdShaderProfile(&shader, &profile);
	}
#endif
#endif

#endif
}


/*****************************************************************************/
/* void amShaderBuildStd(AMS_AMB_HEADER *shader_amb)                         */
/*---------------------------------------------------------------------------*/
/* [INPUT]  shader_amb : コンパイル済みシェーダーファイル                    */
/* [FUNCTION]  標準シェーダーのビルド                                        */
/*****************************************************************************/
void amShaderBuildStd(AMS_AMB_HEADER *shader_amb)
{
#if _PC | _XBOX
	amAssert(shader_amb);

	NNS_STDSHADER_PROFILE	profile;
	IDirect3DDevice9		*d3ddev;
	char	fname[AMD_BIND_DEBUG_NAME_LEN], *ex_name;

#if _PC
	d3ddev	= amWinDxGetIDirect3DDevice();
#elif _XBOX
	d3ddev	= amXboxDxGetIDirect3DDevice();
#endif

	while (nnGetUnbuildStdShaderProfileOne(&profile) > 0) {
		NNS_COMPILED_STDSHADER_PROFILE		shader;
		IDirect3DVertexShader9	*vs;
		IDirect3DPixelShader9	*ps;
		LPD3DXCONSTANTTABLE		vconst;
		LPD3DXCONSTANTTABLE		pconst;
		HRESULT					result;
		NNS_STDSHADER_NAME		name;
		DWORD					*vs_bin;
		DWORD					*ps_bin;

		// シェーダープロファイル
		name	= nnGetStdShaderName(&profile);
//		sprintf(fname, "nss_%04x%04x%04x%04x",
//				(Uint32)(name.high >> 32), (Uint32)name.high,
//				(Uint32)(name.low >> 32), (Uint32)name.low);
		ex_name	= &fname[_amShaderMakeFileName(&name, fname)];
		amSystemLog("shader '%s' load.\n", fname);
//		ex_name	= &fname[strlen(fname)];

		// ファイルの検索
		strcpy(ex_name, ".VSH");
		vs_bin	= (DWORD *)amBindSearch(shader_amb, fname);
		if (vs_bin == NULL) {
			amSystemLog("[ERR] '%s' not found.\n", fname);
			continue;
		}

		strcpy(ex_name, ".PSH");
		ps_bin	= (DWORD *)amBindSearch(shader_amb, fname);
		if (ps_bin == NULL) {
			amSystemLog("[ERR] '%s' not found.\n", fname);
			continue;
		}

		// バーテックスシェーダー
		result	= IDirect3DDevice9_CreateVertexShader(
				d3ddev, vs_bin, &vs);
		if (FAILED(result))
			NNM_ASSERT(0, "Vertex Shader Creation Failed.\n");
		D3DXGetShaderConstantTable(vs_bin, &vconst);

		// ピクセルシェーダー
		result	= IDirect3DDevice9_CreatePixelShader(
				d3ddev, ps_bin, &ps);
		if (FAILED(result))
			NNM_ASSERT(0, "Pixel Shader Creation Failed.\n");
		D3DXGetShaderConstantTable(ps_bin, &pconst);

		// NNに登録
		shader.ShaderType		= NNE_STDSHADERTYPE_HLSL;
		shader.pVertexShader	= vs;
		shader.pPixelShader		= ps;
		shader.pVtxConstTbl		= vconst;
		shader.pPixelConstTbl	= pconst;
		nnRegistCompiledStdShaderProfile(&shader, &profile);
	}
#endif
}


/*****************************************************************************/
/* Sint32 amShaderLoadStd(void *image)                                       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  image : シェーダーバイナリ                                       */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  標準シェーダーのロード                                        */
/*****************************************************************************/
Sint32 amShaderLoadStd(void *image)
{
	AMS_PARAM_LOAD_SHADER	param;

#if _PC | _XBOX
	amAssert(!strncmp(((AMS_AMB_HEADER *)image)->file_id, "!AMB", 4));
#endif

	param.image		= image;

	return	amDrawRegistCommand(AMD_REGIST_LOAD_SHADER, &param);
}


/*****************************************************************************/
/* void amShaderSetFileStd(AMS_AMB_HEADER *shader_amb)                       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  shader_amb : プリコンパイルシェーダーファイル                    */
/* [FUNCTION]  コンパイル済みシェーダーファイルの設定                        */
/*****************************************************************************/
void amShaderSetFileStd(AMS_AMB_HEADER *shader_amb)
{
	_am_shader_amb		= shader_amb;
}


/*****************************************************************************/
/* Sint32 amShaderBuild(const char *vs_code, Sint32 vs_size,                 */
/*                      const char *ps_code, Sint32 ps_size,                 */
/*                      void **vs_shader, void **ps_shader,                  */
/*                      void *vs_param, void *ps_param,                      */
/*                      Sint32 vs_constants, Sint32 ps_constants)            */
/*---------------------------------------------------------------------------*/
/* [INPUT]  vs_code      : バーテックスシェーダーコード                      */
/*          vs_size      : バーテックスシェーダーコードサイズ                */
/*          ps_code      : ピクセルシェーダーコード                          */
/*          ps_size      : ピクセルシェーダーコードサイズ                    */
/*          vs_param     : バーテックスシェーダーパラメータ                  */
/*          ps_param     : ピクセルシェーダーパラメータ                      */
/*          vs_constants : バーテックスシェーダーパラメータ数                */
/*          ps_constants : ピクセルシェーダーパラメータ数                    */
/* [OUTPUT] vs_shader    : 頂点シェーダー                                    */
/*          ps_shader    : ピクセルシェーダー                                */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  シェーダーのビルド（ランタイムコンパイル）                    */
/*****************************************************************************/
Sint32 amShaderBuild(const char *vs_code, Sint32 vs_size, const char *ps_code, Sint32 ps_size, void **vs_shader, void **ps_shader, void *vs_param, void *ps_param, Sint32 vs_constants, Sint32 ps_constants)
{
#ifdef _DEBUG
	AMS_PARAM_BUILD_SHADER	param;

	param.vs_code		= vs_code;
	param.vs_size		= vs_size;
	param.ps_code		= ps_code;
	param.ps_size		= ps_size;
	param.vs_shader		= vs_shader;
	param.ps_shader		= ps_shader;
	param.vs_param		= vs_param;
	param.ps_param		= ps_param;
	param.vs_constants	= (Sint16)vs_constants;
	param.ps_constants	= (Sint16)ps_constants;

#if _PS3
#if 1		// なぜか描画スレッドだとコンパイルできない
	char			*binary;

	const char		*profile_VS = "sce_vp_rsx";
	const char		*profile_PS = "sce_fp_rsx";

	// 頂点シェーダーの作成
	binary		= NULL;
	compile_program_from_string(vs_code,
			profile_VS, "main_VS", 0, &binary);
	*vs_shader	= nnVertexShaderPs3Create(binary,
			(NNS_SHADER_PARAM_PS3 *)vs_param, vs_constants);
	free_compiled_program(binary);

	// ピクセルシェーダーの作成
	binary		= NULL;
	compile_program_from_string(ps_code,
			profile_PS, "main_PS", 0, &binary);
	*ps_shader	= nnPixelShaderPs3Create(binary,
			(NNS_SHADER_PARAM_PS3 *)ps_param, ps_constants);
	free_compiled_program(binary);
#endif
#endif

	return	amDrawRegistCommand(AMD_REGIST_BUILD_SHADER, &param);
#else
	return	-1;
#endif
}


/*****************************************************************************/
/* Sint32 amShaderBuild(void *vs_image, void *ps_image,                      */
/*                      void **vs_shader, void **ps_shader,                  */
/*                      void *vs_param, void *ps_param,                      */
/*                      Sint32 vs_constants, Sint32 ps_constants)            */
/*---------------------------------------------------------------------------*/
/* [INPUT]  vs_image     : バーテックスシェーダーバイナリイメージ            */
/*          ps_image     : ピクセルシェーダーバイナリイメージ                */
/*          vs_param     : バーテックスシェーダーパラメータ                  */
/*          ps_param     : ピクセルシェーダーパラメータ                      */
/*          vs_constants : バーテックスシェーダーパラメータ数                */
/*          ps_constants : ピクセルシェーダーパラメータ数                    */
/* [OUTPUT] vs_shader    : 頂点シェーダー                                    */
/*          ps_shader    : ピクセルシェーダー                                */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  シェーダーのビルド（オフラインコンパイル）                    */
/*****************************************************************************/
Sint32 amShaderBuild(void *vs_image, void *ps_image, void **vs_shader, void **ps_shader, void *vs_param, void *ps_param, Sint32 vs_constants, Sint32 ps_constants)
{
	AMS_PARAM_CREATE_SHADER	param;

	param.vs_image		= vs_image;
	param.ps_image		= ps_image;
	param.vs_shader		= vs_shader;
	param.ps_shader		= ps_shader;
	param.vs_param		= vs_param;
	param.ps_param		= ps_param;
	param.vs_constants	= (Sint16)vs_constants;
	param.ps_constants	= (Sint16)ps_constants;

	return	amDrawRegistCommand(AMD_REGIST_CREATE_SHADER, &param);
}


/*****************************************************************************/
/* Sint32 amShaderRelease(void *vs_shader, void *ps_shader)                  */
/*---------------------------------------------------------------------------*/
/* [INPUT]  vs_shader    : 頂点シェーダー                                    */
/*          ps_shader    : ピクセルシェーダー                                */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  シェーダーの解放                                              */
/*****************************************************************************/
Sint32 amShaderRelease(void *vs_shader, void *ps_shader)
{
	AMS_PARAM_RELEASE_SHADER	param;

	param.vs_shader		= vs_shader;
	param.ps_shader		= ps_shader;

	return	amDrawRegistCommand(AMD_REGIST_RELEASE_SHADER, &param);
}


/*--- Local Functions -------------------------------------------------------*/


#if _PC | _XBOX
/*****************************************************************************/
/* Sint32 _amShaderMakeFileName(NNS_STDSHADER_NAME *name, char *fname)       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  name  : シェーダー名                                             */
/*          fname : シェーダーファイル名                                     */
/* [RETURN]  シェーダーファイル名の文字数                                    */
/* [FUNCTION]  標準シェーダーファイル名の生成                                */
/*****************************************************************************/
Sint32 _amShaderMakeFileName(NNS_STDSHADER_NAME *name, char *fname)
{
	Sint32	i, bit;
	Uint32	id;
	Uint64	id_low, id_high;

	id_low		= name->high;
	id_high		= 0;
	bit			= 64 - 3;

	for (i = 0; i < (128 + 4) / 5; i++) {
		id		= (Uint32)(id_low >> bit);
		if (bit > 64 - 5)
			id		|= (Uint32)(id_high << (64 - bit));
		id		&= 0x1f;

		*fname	= _am_shader_32x[id];
		fname++;

		bit		-= 5;
		if (bit < 0) {
			bit			+= 64;
			id_high		= id_low;
			id_low		= name->low;
		}
	}
	*fname	= 0;

	return	i;
}


/*****************************************************************************/
/* Sint32 amShaderBuildStdToFile(const char *code, Sint32 size,              */
/*        const NNE_STDSHADER_PROFILE ptype, NNS_STDSHADER_PROFILE &profile, */
/*                                                        const char *fname) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  code     : シェーダーコード                                      */
/*          size     : シェーダーコードサイズ                                */
/*          ptype    : シェーダープロファイルタイプ                          */
/*          profile  : シェーダープロファイル                                */
/*          fname    : シェーダーファイル名                                  */
/* [RETURN]  0ならば成功                                                     */
/* [FUNCTION]  標準シェーダーのビルド                                        */
/*****************************************************************************/
Sint32 _amShaderBuildStdToFile(const char *code, Sint32 size, const NNE_STDSHADER_PROFILE ptype, NNS_STDSHADER_PROFILE *profile, const char *fname)
{
#if _PC | _XBOX
	LPD3DXBUFFER		shader;
	LPD3DXCONSTANTTABLE	ctable;
#elif _PS3
#endif
	Sint32				ret = 0;

#if _PC | _XBOX
	if (nnCompileStdShaderDXG20(code, size, ptype, profile, &shader, &ctable) < 0) {
#elif _PS3
	if (nnBuildStdShader(workbuf, 
#endif
		// ビルドエラー
		return	-1;
	}

	// ファイル書き込み
	FILE		*file;
#if _PC
	if (fopen_s(&file, fname, "wb") == 0) {
#elif _XBOX
	if ((file = fopen(fname, "wb")) != NULL) {
#elif _PS3
	if (cellFsOpen(fname, CELL_FS_O_WRONLY | CELL_FS_O_CREAT | CELL_FS_O_TRUNC,
			&file, NULL, 0) == CELL_FS_SUCCEEDED) {
#endif
		DWORD	fsize;
		void	*ptr;
		fsize	= shader->GetBufferSize();
		ptr		= shader->GetBufferPointer();
#if _PC | _XBOX
		fwrite(ptr, fsize, 1, file);
		fclose(file);
#elif _PS3
		cellFsWrite(file, ptr, fsize, NULL);
		cellFsClose(file);
#endif
	} else {
		// ファイルオープンエラー
		ret		= -2;
	}

#if _PC | _XBOX
	if (shader != NULL)
		shader->Release();
	if (ctable != NULL)
		ctable->Release();
#elif _PS3
#endif

	return	ret;
}
#endif

