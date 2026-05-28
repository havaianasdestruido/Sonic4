/*****************************************************************************/
/*      amTexture.cpp               Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* テクスチャライブラリプログラム                                            */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090401-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files ---------------------------------------------------------*/

#include "alice.h"


/*--- Macros ----------------------------------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

void *_amTextureConvertHeader(NNS_TEXINFO *texinfo, void *texbuf);
void _amTextureSetupLoadParam(AMS_PARAM_LOAD_TEXTURE *param,
		NNS_TEXFILE *texfile, void *texload);

/*--- External Valiables ----------------------------------------------------*/

/*--- Global Variables ------------------------------------------------------*/

/*--- Local Variables -------------------------------------------------------*/

/*--- Global Functions ------------------------------------------------------*/

/*****************************************************************************/
/* Sint32 amTextureLoad(NNS_TEXLIST *texlist, NNS_TEXFILELIST *texfilelist,  */
/*                                      char *filepath, AMS_AMB_HEADER *amb) */
/*---------------------------------------------------------------------------*/
/* [INPUT] texlist     : テクスチャリストへのポインタ                        */
/*         texfilelist : テクスチャファイル定義リストへのポインタ            */
/*         filepath    : テクスチャファイルパス                              */
/*         amb         : テクスチャファイルが入ったAMBファイルへのポインタ   */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  テクスチャの登録                                              */
/*****************************************************************************/
Sint32 amTextureLoad(NNS_TEXLIST *texlist, NNS_TEXFILELIST *texfilelist, char *filepath, AMS_AMB_HEADER *amb)
{
	NNS_TEXINFO		*texinfo;
	NNS_TEXFILE		*texfile;
	Sint32			i, index;

	texinfo		= texlist->pTexInfoList;
	texfile		= texfilelist->pTexFileList;
	i			= texfilelist->nTex;
	for (; i > 0; i--, texinfo++, texfile++) {
		if (amb == NULL)
			index	= amTextureLoad(texinfo, texfile, filepath, NULL, 0);
		else {
			AMS_AMB_FILE	*amb_file;
			amBindSearch(amb, texfile->Filename, &amb_file);
			amAssert(amb_file != NULL);
			index	= amTextureLoad(texinfo, texfile, filepath,
					amb_file->data, amb_file->size);
		}
	}

	return	index;
}


/*****************************************************************************/
/* Sint32 amTextureLoad(NNS_TEXINFO *texinfo, NNS_TEXFILE *texfile,          */
/*                               char *filepath, void *texbuf, Sint32 size)  */
/*---------------------------------------------------------------------------*/
/* [INPUT] texinfo : テクスチャ情報へのポインタ                              */
/*         texfile : テクスチャファイル定義へのポインタ                      */
/*         filepath : テクスチャファイルパス                                 */
/*         texbuf  : テクスチャファイルへのポインタ                          */
/*         size    : ファイルサイズ                                          */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  テクスチャの登録                                              */
/*****************************************************************************/
Sint32 amTextureLoad(NNS_TEXINFO *texinfo, NNS_TEXFILE *texfile, char *filepath, void *texbuf, Sint32 size)
{
	AMS_PARAM_LOAD_TEXTURE	param;

	memset(&param, 0, sizeof(AMS_PARAM_LOAD_TEXTURE));

	// ファイルの読み込み（ブロック）
	if (texbuf == NULL) {
		char	fname[MAX_PATH];
		strcpy(fname, filepath);
		strcat(fname, texfile->Filename);
		size		= amFsRead(fname, &texbuf);
		param.buf_delete	= texbuf;
		param.flag			= NND_TEXFLAG_ALLOCATE;
	}

	// テクスチャの登録
	param.pTexInfo		= texinfo;
	param.tex			= _amTextureConvertHeader(texinfo, texbuf);
#if !_WII
	param.size			= size;
#endif
	_amTextureSetupLoadParam(&param, texfile, texbuf);

	return	amDrawRegistCommand(AMD_REGIST_LOAD_TEXTURE, &param);
}


/*****************************************************************************/
/* Sint32 amTextureLoad(void **texture, void *image, Sint32 size,            */
/*                         Sint32 minfilter, Sint32 magfilter,               */
/*                               Sint32 u_wrap, Sint32 v_wrap, void *gvrobj) */
/*---------------------------------------------------------------------------*/
/* [INPUT] image       : テクスチャイメージ                                  */
/*         size        : テクスチャイメージデータサイズ                      */
/*         minfilter   : 縮小フィルタ                                        */
/*         magfilter   : 拡大フィルタ                                        */
/*         u_wrap      : U方向テクスチャラップモード                         */
/*         v_wrap      : V方向テクスチャラップモード                         */
/*         gvrobj      : GVRオブジェクト(Wiiのみ)                            */
/* [OUTPUT] texture    : テクスチャへのポインタ                              */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  テクスチャの登録                                              */
/*****************************************************************************/
Sint32 amTextureLoad(void **texture, void *image, Sint32 size, Sint32 minfilter, Sint32 magfilter, Sint32 u_wrap, Sint32 v_wrap, void *gvrobj)
{
	AMS_PARAM_LOAD_TEXTURE_IMAGE	param;

	param.texture	= texture;
	param.image		= image;
	param.size		= size;
	param.minfilter	= (Sint16)minfilter;
	param.magfilter	= (Sint16)magfilter;
	param.u_wrap	= (Sint16)u_wrap;
	param.v_wrap	= (Sint16)v_wrap;
#if !_WII
	UNREFERENCED_PARAMETER(gvrobj);
#else
	param.gvrobj	= (NVS_GVROBJ *)gvrobj;
#endif

	return	amDrawRegistCommand(AMD_REGIST_LOAD_TEXTURE_IMAGE, &param);
}


/*****************************************************************************/
/* Sint32 amTextureRelease(NNS_TEXLIST *texlist)                             */
/*---------------------------------------------------------------------------*/
/* [INPUT] texlist     : テクスチャリストへのポインタ                        */
/* [RETURN]  解放完了をチェックするための登録ID                              */
/* [FUNCTION]  テクスチャの解放                                              */
/*****************************************************************************/
Sint32 amTextureRelease(NNS_TEXLIST *texlist)
{
	AMS_PARAM_RELEASE_TEXTURE	*param;

	param	= (AMS_PARAM_RELEASE_TEXTURE *)amDrawMallocDataBuffer(
			sizeof(AMS_PARAM_RELEASE_TEXTURE));
	param->texlist	= texlist;

	return	amDrawRegistCommand(AMD_REGIST_RELEASE_TEXTURE, param);
}


/*****************************************************************************/
/* Sint32 amTextureRelease(void *texture)                                    */
/*---------------------------------------------------------------------------*/
/* [INPUT] texture     : テクスチャへのポインタ                              */
/* [RETURN]  解放完了をチェックするための登録ID                              */
/* [FUNCTION]  テクスチャの解放                                              */
/*****************************************************************************/
Sint32 amTextureRelease(void *texture)
{
	AMS_PARAM_RELEASE_TEXTURE_IMAGE	param;

	param.texture	= texture;

	return	amDrawRegistCommand(AMD_REGIST_RELEASE_TEXTURE_IMAGE, &param);
}


/*--- Local Functions -------------------------------------------------------*/


/*****************************************************************************/
/* void *_amTextureConvertHeader(NNS_TEXINFO *texinfo, void *texbuf)         */
/*---------------------------------------------------------------------------*/
/* [INPUT] texinfo : テクスチャ情報へのポインタ                              */
/*         texbuf  : テクスチャファイルへのポインタ                          */
/* [FUNCTION]  テクスチャの登録パラメータの設定                              */
/*****************************************************************************/
void *_amTextureConvertHeader(NNS_TEXINFO *texinfo, void *texbuf)
{
#if _PC | _XBOX
	NVS_XVRHEADER	*xvr_header;

	xvr_header	= (NVS_XVRHEADER *)&texinfo->xvrobj;
	memcpy(xvr_header, texbuf, sizeof(NVS_XVRHEADER));

	amConvertLittleEndian(&xvr_header->gbix_cnk_long);
	amConvertLittleEndian(&xvr_header->gbix_next);
	amConvertLittleEndian(&xvr_header->gbix);
	amConvertLittleEndian(&xvr_header->gbix_pad);
	amConvertLittleEndian(&xvr_header->pvrt_cnk_long);
	amConvertLittleEndian(&xvr_header->pvrt_next);
	amConvertLittleEndian(&xvr_header->attr);
	amConvertLittleEndian(&xvr_header->width);
	amConvertLittleEndian(&xvr_header->height);

	if (xvr_header->gbix_cnk_long == 'XIBG') {
		if (xvr_header->xbTexHeader.Dummy[0] == 0)
			texbuf	= (void *)(((Uint8 *)texbuf) + 0x800);
		else
			texbuf	= (void *)(((Uint8 *)texbuf) + 0x040);
	} else
		memset(xvr_header, 0, sizeof(NVS_XVRHEADER));
#elif _WII
	texbuf	= (void *)((Uint8 *)texbuf + 0x20);
#endif

	return	texbuf;
}


/*****************************************************************************/
/* void _amTextureSetupLoadParam(AMS_PARAM_LOAD_TEXTURE *param,              */
/*                                     NNS_TEXFILE *texfile, void *texload)  */
/*---------------------------------------------------------------------------*/
/* [INPUT] param   : テクスチャ登録パラメータへのポインタ                    */
/*         texfile : テクスチャファイル定義へのポインタ                      */
/*         texload : 読み込んだテクスチャファイルへのポインタ                */
/* [FUNCTION]  テクスチャの登録パラメータの設定                              */
/*****************************************************************************/
void _amTextureSetupLoadParam(AMS_PARAM_LOAD_TEXTURE *param, NNS_TEXFILE *texfile, void *texload)
{
#if _WII
	NVS_GVROBJ		*gvrobj;

	gvrobj		= &param->pTexInfo->GvrObj;
	nvGetGVRHeader(gvrobj, texload);

	param->size	= nvCalcGVRTexSize(gvrobj);
#else
	UNREFERENCED_PARAMETER(texload);
#endif

	// テクスチャフィルタ
	if (texfile->fType & NND_TEXFTYPE_NO_FILTER) {
		param->minfilter	= NND_MIN_NEAREST;
		param->magfilter	= NND_MAG_NEAREST;
	} else {
		param->minfilter	= texfile->MinFilter;
		param->magfilter	= texfile->MagFilter;
	}

	// グローバルインデックス
	if (texfile->fType & NND_TEXFTYPE_LISTGLBIDX)
		param->globalIndex	= texfile->GlobalIndex;
	else {
#if _WII
		param->globalIndex	= gvrobj->gbixh.globalIndex;
#else
		param->globalIndex	= 0;
#endif
	}

	// バンク
	if (texfile->fType & NND_TEXFTYPE_LISTBANK)
		param->bank			= texfile->Bank;
	else {
#if _WII
		param->bank			= gvrobj->gbixh.bank;
#else
		param->bank			= 0;
#endif
	}
}

