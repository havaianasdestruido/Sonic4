/************************************************************************/
/*      amDraw.cpp                                                      */
/*            Copyright(c) 2000-2009 Dimps CORP. All Rights Reserved.   */
/*----------------------------------------------------------------------*/
/* 描画関連ライブラリ                                                   */
/*----------------------------------------------------------------------*/
/* Date      Ver.   Comment                                             */
/* 20090324  0.10   簡易バージョン                                      */
/************************************************************************/

/*----- Update Logs ----------------------------------------------------*/

/*----- Macro switches -------------------------------------------------*/

/*----- Include files --------------------------------------------------*/

#if _PS3
#include <sys/paths.h>
#include <Cg/cgc.h>
#endif

#include "alice.h"


/*----- Macros ---------------------------------------------------------*/

/*----- definitions ----------------------------------------------------*/

void _amDrawTaskMake(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawPrintf(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawPrintColor(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawHeapMap(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawThreadMap(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawObject(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawObjectSetMaterial(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawMotion(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawMotionTRS(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawPrimitive2D(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawPrimitive3D(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSetDiffuse(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSetAmbient(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSetAlpha(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSetSpecular(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSetEnvMap(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSetBlend(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSetTexOffset(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSetFog(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSetFogColor(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSetFogRange(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSetZMode(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSetPrimitive3DParam(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSetPrimitive2DParam(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);

void _amDrawSortObject(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSortPrimitive2D(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
void _amDrawSortPrimitive3D(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);

void _amDrawRegistNop(AMS_REGISTLIST *regist);
void _amDrawLoadTexture(AMS_REGISTLIST *regist);
void _amDrawReleaseTexture(AMS_REGISTLIST *regist);
void _amDrawVertexBufferObject(AMS_REGISTLIST *regist);
void _amDrawDeleteVertexObject(AMS_REGISTLIST *regist);
void _amDrawLoadShaderObject(AMS_REGISTLIST *regist);
void _amDrawReleaseStdShader(AMS_REGISTLIST *regist);
void _amDrawLoadShader(AMS_REGISTLIST *regist);
void _amDrawBuildShader(AMS_REGISTLIST *regist);
void _amDrawCreateShader(AMS_REGISTLIST *regist);
void _amDrawReleaseShader(AMS_REGISTLIST *regist);
void _amDrawLoadTextureImage(AMS_REGISTLIST *regist);
void _amDrawReleaseTextureImage(AMS_REGISTLIST *regist);


/*----- External variables ---------------------------------------------*/

#if _WII
extern MEMHeapHandle		_am_wii_texture_heap;
#endif


/*----- Global valiables -----------------------------------------------*/

AMS_DRAW_VIDEO		_am_draw_video;
AMS_RENDER_TARGET	_am_draw_target;

AMS_ALARM			*_am_draw_vsync_alarm = NULL;

void (*_am_draw_command_func)(AMS_COMMAND_HEADER *, NNF_DRAWOBJ) = NULL;
void (*_am_draw_command_sort)(AMS_COMMAND_HEADER *, NNF_DRAWOBJ) = NULL;

AMS_DISPLAYLIST_MANAGER		_am_displaylist_manager;

Sint32	_am_draw_command_buf_max;
Sint32	_am_draw_data_buf_max;
Sint32	_am_draw_work_buf_max;
Sint32	_am_draw_work_buf_size;

char	*_am_draw_command_buf[AMD_DISPLAYLIST_NUM];
char	*_am_draw_data_buf[AMD_DISPLAYLIST_NUM];
char	*_am_draw_work_buf_ptr;

char	*_am_draw_work_buf;

Sint32	_am_draw_in_scene = 0;

NNS_MATRIX	_am_draw_world_view_matrix;

NNS_MATRIX44		_am_draw_proj_mtx;
NNE_PROJECTION_TYPE _am_draw_proj_type;


#if _WII
Uint32	_am_draw_clear_depth = GX_MAX_Z24;
#endif
#if AMD_DEBUG
NNS_RGBA_U8 _am_draw_bg_color = { 0x00, 0x40, 0x00, 0xff};
#else
NNS_RGBA_U8 _am_draw_bg_color = { 0x00, 0x00, 0x00, 0xff};
#endif

#if _PC | _XBOX | _PS3
Uint32	_am_draw_vs_code_size;
Uint32	_am_draw_ps_code_size;
void	*_am_draw_vs_code_buf;
void	*_am_draw_ps_code_buf;
Sint32	_am_draw_shader_compile;
Sint32	_am_draw_regist_flag;
#elif _WII
NNS_VECTOR		_am_draw_toonDir = {1.0f, 0.0f, 0.0f};
NNS_MATRIX44	_am_draw_fall_projmtx;
AMS_RENDER_TARGET	_am_draw_render_work;

//スクリーンキャプチャ
#if AMD_DEBUG
static Uint8 g_screen_capture_work[SCRNSHOT_SHARE_MEMORY_SIZE]; 
#endif
#endif

Sint32	_am_draw_offset_x = 0;

AMS_DRAWSTATE	_am_draw_state;

Sint32			_am_draw_state_stack_num = 0;
AMS_DRAWSTATE	_am_draw_state_stack[AMD_DRAWSTATE_STACK_NUM];


/*----- Local valiables ------------------------------------------------*/

#if _PC | _XBOX | _WII
AMS_ALARM	_am_draw_alarm_vsync;	// 擬似V-Sync
#endif

static AMS_TASK		*_am_draw_task;

#if _WII
static NNS_MATRIX	_am_draw_unit_matrix = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
};
#endif

// システムコマンド処理関数テーブル
static void (*_am_draw_system_exec[])(AMS_COMMAND_HEADER *, NNF_DRAWOBJ) = {
	NULL,						//   0
	_amDrawTaskMake,			//  -1 : タスク生成
	_amDrawPrintf,				//  -2 : デバッグ文字表示
	_amDrawPrintColor,			//  -3 : デバッグ文字色設定
	_amDrawHeapMap,				//  -4 : メモリメーター表示
	_amDrawThreadMap,			//  -5 : スレッドメーター表示
	_amDrawObject,				//  -6 : オブジェクト描画
	_amDrawObject,				//  -7 : オブジェクト描画
	_amDrawObjectSetMaterial,	//  -8 : マテリアル設定＋オブジェクト描画
	_amDrawMotion,				//  -9 : モーション描画
	_amDrawMotion,				// -10 : モーション描画
	_amDrawMotionTRS,			// -11 : モーション描画(TRS付き)
	_amDrawMotionTRS,			// -12 : モーション描画(TRS付き)
	_amDrawPrimitive2D,			// -13 : プリミティブ描画(2D)
	_amDrawPrimitive3D,			// -14 : プリミティブ描画(3D)
	_amDrawSetDiffuse,			// -15 : ディフューズ設定
	_amDrawSetAmbient,			// -16 : アンビエント設定
	_amDrawSetAlpha,			// -17 : α設定
	_amDrawSetSpecular,			// -18 : スペキュラー設定
	_amDrawSetEnvMap,			// -19 : 環境マップ設定
	_amDrawSetBlend,			// -20 : ブレンドモード設定
	_amDrawSetTexOffset,		// -21 : テクスチャオフセット設定
	_amDrawSetFog,				// -22 : フォグ設定
	_amDrawSetFogColor,			// -23 : フォグカラー設定
	_amDrawSetFogRange,			// -24 : フォグレンジ設定
	_amDrawSetZMode,			// -25 : Z比較更新設定
};

// システムコマンド処理関数テーブル(ソート)
static void (*_am_draw_sort_system_exec[])(AMS_COMMAND_HEADER *, NNF_DRAWOBJ) = {
	NULL,						//  0
	_amDrawSortObject,			// -1 : オブジェクト描画
	_amDrawSortObject,			// -2 : オブジェクト描画
	_amDrawSortPrimitive2D,		// -3 : プリミティブ描画（2D）
	_amDrawSortPrimitive3D,		// -4 : プリミティブ描画（3D）
};

// 登録コマンド処理関数テーブル
static void (*_am_draw_regist_func[])(AMS_REGISTLIST *) = {
	_amDrawRegistNop,			//  0 : NOP
	_amDrawLoadTexture,			//  1 : テクスチャの登録
	_amDrawReleaseTexture,		//  2 : テクスチャの解放
	_amDrawVertexBufferObject,	//  3 : バッファの取得
	_amDrawDeleteVertexObject,	//  4 : バッファの解放
	_amDrawLoadShaderObject,	//  5 : シェーダーのロード
	_amDrawReleaseStdShader,	//  6 : シェーダーの解放
	_amDrawLoadShader,			//  7 : シェーダーのロード
	_amDrawBuildShader,			//  8 : シェーダーのビルド
	_amDrawCreateShader,		//  9 : シェーダーの作成
	_amDrawReleaseShader,		// 10 : シェーダーの解放
	_amDrawLoadTextureImage,	// 11 : テクスチャの登録
	_amDrawReleaseTextureImage,	// 12 : テクスチャの解放
};


#if _PC | _XBOX
IDirect3DVertexShader9*		 _am_draw_sprite_VS = NULL;
IDirect3DPixelShader9*		 _am_draw_sprite_PS = NULL;

#if !AMD_SHADER_PRECOMPILED
static const CHAR* _am_draw_sprite_code =
	"struct VS_INPUT_PT                                        \n"
	"{                                                         \n"
	"   float4   Pos          : POSITION;                      \n"
	"   float2   UV           : TEXCOORD0;                     \n"
	"};                                                        \n"
	"                                                          \n"
	"struct VS_OUTPUT_PT                                       \n"
	"{                                                         \n"
	"   float4 Pos	          : POSITION;                      \n"
	"   float2 UV             : TEXCOORD0;                     \n"
	"};                                                        \n"
	"                                                          \n"
	"VS_OUTPUT_PT main_VS( VS_INPUT_PT Input )                 \n"
	"{                                                         \n"
	"   VS_OUTPUT_PT Output;                                   \n"
	"   Output.Pos.x  = Input.Pos.x - 0.5;                     \n"
	"   Output.Pos.y  = Input.Pos.y - 0.5;                     \n"
	"   Output.Pos.z  = Input.Pos.z;                           \n"
	"   Output.Pos.w  = Input.Pos.w;                           \n"
	"   Output.UV	  = Input.UV;                              \n"
	"   return( Output );                                      \n"
	"}                                                         \n"
	"                                                          \n"
	"                                                          \n"
	"struct PS_INPUT_T                                         \n"
	"{                                                         \n"
	"   float2 UV            : TEXCOORD0;                      \n"
	"};                                                        \n"
	"                                                          \n"
	"sampler2D        s_Tex : register( s0 );				   \n"
	"                                                          \n"
	"float4 main_PS( PS_INPUT_T Input ) : COLOR0               \n"
	"{                                                         \n"
// 	"   return( tex2D( s_Tex, Input.UV ) );                    \n"
 	"   return( float4( tex2D( s_Tex, Input.UV ).rgb, 1.0f) ); \n"
	"}                                                         \n";
#else
#include "draw_sprite_VS.fxh"
#include "draw_sprite_PS.fxh"
#endif
#endif


/*----- Local declarations ---------------------------------------------*/

/*----- Global functions -----------------------------------------------*/

/************************************************************************/
/* void amDrawCreateBuffer(Sint32 command_size, Sint32 data_size,       */
/*                                                    Sint32 work_size) */
/*----------------------------------------------------------------------*/
/* [INPUT] command_size : コマンドバッファのサイズ                      */
/*         data_size    : データバッファのサイズ                        */
/*         work_size    : ワークバッファのサイズ                        */
/* [FUNCTION] 描画用バッファの確保                                      */
/************************************************************************/
void amDrawCreateBuffer(Sint32 command_size, Sint32 data_size, Sint32 work_size)
{
	Sint32		i;

	_am_draw_command_buf_max	= command_size;
	_am_draw_data_buf_max		= data_size;
	_am_draw_work_buf_max		= work_size;
#if _PC | _XBOX | _PS3
	_am_draw_shader_compile		= 1;
#endif

	for (i = 0; i < AMD_DISPLAYLIST_NUM; i++) {
		_am_draw_command_buf[i]		= (char *)amMemAllocSystem(command_size);
		_am_draw_data_buf[i]		= (char *)amMemAllocSystem(data_size);
	}
	_am_draw_work_buf	= (char *)amMemAllocSystem(work_size);

#if _PC | _XBOX | _WII
	amAlarmCreateTimer(&_am_draw_alarm_vsync);
#if _PC
	amAlarmSetTimer(&_am_draw_alarm_vsync,
			(Uint32)(amSystemGetFrameRateDraw() * 1000000.0f / 60.0f + 0.5f));
#elif _XBOX
	amAlarmSetTimer(&_am_draw_alarm_vsync,
			(Uint32)(1000000.0f / 60.0f + 0.5f));
#elif _WII
	amAlarmSetTimer(&_am_draw_alarm_vsync,
			(Uint32)(1000000.0f / 60.0f + 0.5f));
#endif
#endif
}


/************************************************************************/
/* void amDrawDeleteBuffer(void)                                        */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 描画用バッファの解放                                      */
/************************************************************************/
void amDrawDeleteBuffer(void)
{
	Sint32		i;

#if _PC | _XBOX | _WII
	amAlarmDelete(&_am_draw_alarm_vsync);
#endif

	amMemFreeSystem(_am_draw_work_buf);
	for (i = 0; i < AMD_DISPLAYLIST_NUM; i++) {
		amMemFreeSystem(_am_draw_data_buf[i]);
		amMemFreeSystem(_am_draw_command_buf[i]);
	}
}


/************************************************************************/
/* void amDrawSetShaderCompile(Sint32 flag)                             */
/*----------------------------------------------------------------------*/
/* [INPUT]  flag : シェーダーをランタイムでコンパイルするか             */
/* [FUNCTION] シェーダーコンパイルの設定                                */
/************************************************************************/
void amDrawSetShaderCompile(Sint32 flag)
{
#if _PS3
	if (amThreadCheckDraw())
		_am_draw_shader_compile		= flag;
	else {
		AMS_COMMAND_BUFFER_HEADER	*header;

		amMutexLock(&_am_displaylist_manager.mutex);

		if (flag) {
			_am_displaylist_manager.reg_flag	&=
					~AMD_REGFLG_NO_SHADER_COMPILE;
		} else {
			_am_displaylist_manager.reg_flag	|=
					AMD_REGFLG_NO_SHADER_COMPILE;
		}

		header		= amDrawGetWriteHeader();
		header->regist_flag		= _am_displaylist_manager.reg_flag;

		amMutexUnlock(&_am_displaylist_manager.mutex);
	}
#else
	UNREFERENCED_PARAMETER(flag);
#endif
}


/************************************************************************/
/* void amDrawSetVSyncAlarm(AMS_ALARM *alarm)                           */
/*----------------------------------------------------------------------*/
/* [INPUT]  alarm : V-Syncを通知するアラーム                            */
/* [FUNCTION] V-Sync待ち                                                */
/************************************************************************/
void amDrawSetVSyncAlarm(AMS_ALARM *alarm)
{
	_am_draw_vsync_alarm	= alarm;
}


/************************************************************************/
/* void amDrawWaitVSync(void)                                           */
/*----------------------------------------------------------------------*/
/* [FUNCTION] V-Sync待ち                                                */
/************************************************************************/
void amDrawWaitVSync(void)
{
#if _PC
	LPDIRECT3DDEVICE9	d3dDev;
	HRESULT		result;
	AMS_ALARM	*alarm;

	if (!_am_draw_video.vsync_flag) {
		amAlarmWaitTimer(&_am_draw_alarm_vsync);
//		amAlarmSetTimer(&_am_draw_alarm_vsync,
//			(Sint32)(amSystemGetFrameRateDraw() * 1000000.0f / 60.0f + 0.5f));
	}

	d3dDev	= amWinDxGetDirect3DDevice();
	result	= d3dDev->Present(NULL, NULL, NULL, NULL);
	amDebugSetPerformanceGPU();
	alarm	= _am_draw_vsync_alarm;
	if (alarm != NULL) {
		amAlarmSet(alarm);
		amAlarmSetTimer(alarm,
				(Sint32)(amSystemGetFrameRateMain() * 1000000.0f / 60.0f + 0.5f));
	}
//	if (FAILED(result))
//		NNM_ASSERT(0, "Error : IDirect3DDevice9::Present()\n");
#elif _XBOX
	LPDIRECT3DDEVICE9	d3dDev;
	HRESULT		result;

	if (!_am_draw_video.vsync_flag) {
		amAlarmWaitTimer(&_am_draw_alarm_vsync);
//		amAlarmSetTimer(&_am_draw_alarm_vsync,
//			(Sint32)(1000000.0f / 60.0f + 0.5f));
	}

	d3dDev	= amXboxDxGetDirect3DDevice();
	if (_am_render_manager.target_now.tiling == NULL)
		result	= d3dDev->Present(NULL, NULL, NULL, NULL);
	else {
		d3dDev->SynchronizeToPresentationInterval();
		d3dDev->Swap(amXboxDxGetFrontBuf(), NULL);
		amXboxDxSetNextFrontBuf();
	}
	amDebugSetPerformanceGPU();
	if (_am_draw_vsync_alarm != NULL) {
		amAlarmSet(_am_draw_vsync_alarm);
		amAlarmSetTimer(_am_draw_vsync_alarm,
				(Sint32)(amSystemGetFrameRateMain() * 1000000.0f / 60.0f + 0.5f));
	}
//	if (FAILED(result))
//		NNM_ASSERT(0, "Error : IDirect3DDevice9::Present()\n");
#elif _PS3
#if AMD_PS3_USE_LIBRESC
	nnDevicePs3RescSwap(_am_ps3_device);
#else
	nnDevicePs3Swap(_am_ps3_device);
#endif
	amDebugSetPerformanceGPU();
	if (_am_draw_vsync_alarm != NULL) {
		amAlarmSet(_am_draw_vsync_alarm);
		amAlarmSetTimer(_am_draw_vsync_alarm,
				(Sint32)(amSystemGetFrameRateMain() * 1000000.0f / 60.0f + 0.5f));
	}
#elif _WII

#if AMD_DEBUG
	// 説明書の画像作成で使うのでコメントアウトしないこと
	SCREENSHOTSetRenderModeObj(&_am_wii_render_mode);
	SCREENSHOTService(_am_wii_frame_buffer[_am_wii_next_buffer], g_screen_capture_work, SCRNSHOT_SHARE_MEMORY_SIZE); //スクリーンキャプチャ
#endif

	if (!_am_draw_video.vsync_flag) {
		amAlarmWaitTimer(&_am_draw_alarm_vsync);
//		amAlarmSetTimer(&_am_draw_alarm_vsync,
//			(Sint32)(1000000.0f / 60.0f + 0.5f));
		VIFlush();
	} else {
		VIFlush();
		VIWaitForRetrace();
	}
	amWiiSwapFrameBuffer();
	amDebugSetPerformanceGPU();
	if (_am_draw_vsync_alarm != NULL) {
		amAlarmSet(_am_draw_vsync_alarm);
		amAlarmSetTimer(_am_draw_vsync_alarm,
				(Sint32)(amSystemGetFrameRateMain() * 1000000.0f / 60.0f + 0.5f));
	}
#endif
}


/************************************************************************/
/* Sint32 amDrawBegin(AMS_RENDER_TARGET *target, NNS_RGBA_U8 *color,    */
/*                            float depth, Sint32 stencil, Uint32 flag) */
/*----------------------------------------------------------------------*/
/* [INPUT]  target  : 描画対象レンダーターゲット                        */
/*          flag    : クリアフラグ(AMD_RENDER_CLEAR_～)                 */
/*          color   : 背景色                                            */
/*          depth   : デプス値                                          */
/*          stencil : ステンシル値                                      */
/* [FUNCTION] 描画開始                                                  */
/************************************************************************/
Sint32 amDrawBegin(AMS_RENDER_TARGET *target, Uint32 flag, NNS_RGBA_U8 *color, float depth, Sint32 stencil)
{
	_am_draw_offset_x	= 0;
	nnSetPrintSize(
			AMD_SCREEN_WIDTH / (float)AMD_TEXT_WIDTH,
			AMD_SCREEN_HEIGHT / (float)AMD_TEXT_HEIGHT);

#if _PC | _XBOX | _PS3
	if (color == NULL)
		color		= amDrawGetBGColor();
#endif

	amRenderSetTarget(target, flag, color, depth, stencil);

	if (_am_draw_in_scene)
		return	TRUE;

#if _PC
	LPDIRECT3DDEVICE9	d3dDev;
	d3dDev	= amWinDxGetDirect3DDevice();
	if (flag) {
		d3dDev->Clear(0, NULL, flag,
				D3DCOLOR_RGBA(color->r, color->g, color->b, color->a),
				depth, stencil);
	}

	if (SUCCEEDED(d3dDev->BeginScene())) {
		_am_draw_in_scene	= 1;
		return	TRUE;
	}

	return	FALSE;
#elif _XBOX
	LPDIRECT3DDEVICE9	d3dDev;
	AMS_XBOXDX_TILE_INFO	*tiling;

	d3dDev	= amXboxDxGetDirect3DDevice();
	tiling	= _am_render_manager.target_now.tiling;

	if (tiling != NULL) {
		D3DVECTOR4	clear_color;
		clear_color.x	= (float)color->r / 255.0f;
		clear_color.y	= (float)color->g / 255.0f;
		clear_color.z	= (float)color->b / 255.0f;
		clear_color.w	= (float)color->a / 255.0f;
		d3dDev->BeginTiling(0, tiling->count, tiling->rect,
				&clear_color, depth, stencil);
	} else	if (flag) {
		d3dDev->Clear(0, NULL, flag,
				D3DCOLOR_RGBA(color->r, color->g, color->b, color->a),
				depth, stencil);
	}

	if (SUCCEEDED(d3dDev->BeginScene())) {
		_am_draw_in_scene	= 1;
		return	TRUE;
	}

	return	FALSE;
#elif _PS3
	if (flag) {
		if (flag & AMD_RENDER_CLEAR_COLOR)
			nnDevicePs3SetClearColor(_am_ps3_device, color);
		if (flag & AMD_RENDER_CLEAR_DEPTH)
			nnDevicePs3SetClearDepth(_am_ps3_device, depth);
		if (flag & AMD_RENDER_CLEAR_STENCIL)
			nnDevicePs3SetClearStencil(_am_ps3_device, stencil);
		nnDevicePs3Clear(_am_ps3_device, flag);
	}

	nnDevicePs3BeginScene(_am_ps3_device);

	_am_draw_in_scene	= 1;

	return	TRUE;
#elif _WII
//	if (_am_render_manager.targetp == &_am_render_default) {
	if (!_am_draw_in_scene) {
		if (_am_wii_render_mode.field_rendering) {
			GXSetViewportJitter(0.0f, 0.0f,
					(float)_am_wii_render_mode.fbWidth,
					(float)_am_wii_render_mode.efbHeight,
					0.0f, 1.0f, VIGetNextField());
		} else {
			GXSetViewport(0.0f, 0.0f,
					(float)_am_wii_render_mode.fbWidth,
					(float)_am_wii_render_mode.efbHeight,
					0.0f, 1.0f);
		}

		static BOOL		bFirst = TRUE;
		if (bFirst) 
		{
			VISetBlack(GX_FALSE);
			bFirst	= FALSE;
		}
	}

	if (flag) {
		if (flag & (AMD_RENDER_CLEAR_COLOR | AMD_RENDER_CLEAR_DEPTH)) {
			if (color != NULL) {
				_am_draw_bg_color.r		= color->r;
				_am_draw_bg_color.g		= color->g;
				_am_draw_bg_color.b		= color->b;
				_am_draw_bg_color.a		= color->a;
			}
			if (flag & AMD_RENDER_CLEAR_DEPTH) {
				_am_draw_clear_depth	= (Uint32)
						(depth * (float)_am_draw_video.depth_max);
			} else
				_am_draw_clear_depth	= _am_draw_video.depth_max;

			GXColor		clear_color;
			clear_color.r	= _am_draw_bg_color.r;
			clear_color.g	= _am_draw_bg_color.g;
			clear_color.b	= _am_draw_bg_color.b;
			clear_color.a	= _am_draw_bg_color.a;

			GXSetCopyClear(clear_color, _am_draw_clear_depth);
		}
	}

	_am_draw_in_scene	= 1;

	return	TRUE;
#elif _IPHONE
	if (flag) {
		GLbitfield bit = 0;
		if (flag & AMD_RENDER_CLEAR_COLOR) {
			if (color != NULL) {
				_am_draw_bg_color.r		= color->r;
				_am_draw_bg_color.g		= color->g;
				_am_draw_bg_color.b		= color->b;
				_am_draw_bg_color.a		= color->a;
			}
			GLclampf clear_color[4];
			clear_color[0]	= (GLclampf)_am_draw_bg_color.r / 255.0f;
			clear_color[1]	= (GLclampf)_am_draw_bg_color.g / 255.0f;
			clear_color[2]	= (GLclampf)_am_draw_bg_color.b / 255.0f;
			clear_color[3]	= (GLclampf)_am_draw_bg_color.a / 255.0f;

			glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
			bit |= GL_COLOR_BUFFER_BIT;
		}
		if (flag & AMD_RENDER_CLEAR_DEPTH) {
			glClearDepthf(depth);
			bit |= GL_DEPTH_BUFFER_BIT;
		}
		glClear(bit);
	}
	return	TRUE;
#endif
}


/************************************************************************/
/* void amDrawEnd(void)                                                 */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 描画終了                                                  */
/************************************************************************/
void amDrawEnd(NNS_RGBA_U8 *color, float z, Sint32 stencil)
{
//	amDrawEndScene();

	if (!_am_draw_in_scene)
		return;

#if _PC | _XBOX | _PS3
	if (color == NULL)
		color		= &_am_draw_bg_color;
#endif

#if _PC
	LPDIRECT3DDEVICE9	d3dDev;

	UNREFERENCED_PARAMETER(stencil);
	UNREFERENCED_PARAMETER(z);

	d3dDev	= amWinDxGetDirect3DDevice();
	d3dDev->EndScene();
#elif _XBOX
	LPDIRECT3DDEVICE9	d3dDev;
	AMS_RENDER_TARGET	*target;
	AMS_XBOXDX_TILE_INFO	*tiling;
	Sint32		i;

	d3dDev	= amXboxDxGetDirect3DDevice();
	d3dDev->EndScene();

	target	= &_am_render_manager.target_now;

	D3DRECT		rect = { 0, 0, target->width, target->height,};
	D3DPOINT	point = { 0, 0,};

	tiling	= target->tiling;
	if (tiling != NULL) {
		// タイリングあり
#if 0
		if (target->flag & AMD_RENDER_FLAG_RESOLVE_DEPTH) {
			if (target->surface_num != 0) {
				d3dDev->Resolve(D3DRESOLVE_DEPTHSTENCIL,
						&rect, target->texture_depth, &point, 0, 0, NULL, 1.0f, 0, NULL);
			} else {
				d3dDev->EndTiling(D3DRESOLVE_DEPTHSTENCIL,
						NULL, target->texture_depth, NULL, 1.0f, 0, NULL);
			}
		}
		if (target->surface_num > 0) {
			for (i = target->surface_num - 1; i >= 0; i--) {
				if (!(target->flag & (AMD_RENDER_FLAG_RESOLVE_COLOR0 << i)))
					continue;
				d3dDev->Resolve(i,
						&rect, target->texture_color[i], &point, 0, 0, NULL, 1.0f, 0, NULL);
			}
			d3dDev->EndTiling(D3DRESOLVE_RENDERTARGET0,
					NULL, target->texture_color[0], NULL, 1.0f, 0, NULL);
		}
#else
	Uint32		t;
	D3DVECTOR4	clearColor;
	if (color != NULL) {
		clearColor.x	= (float)color->r / 255.0f;
		clearColor.y	= (float)color->g / 255.0f;
		clearColor.z	= (float)color->b / 255.0f;
		clearColor.w	= (float)color->a / 255.0f;
	} else {
		clearColor.x	= 0.0f;
		clearColor.y	= 0.0f;
		clearColor.z	= 0.0f;
		clearColor.w	= 1.0f;
	}
	if ((target->surface_num == 1) && (target->texture_depth == NULL)) {
		d3dDev->EndTiling(D3DRESOLVE_CLEARRENDERTARGET | D3DRESOLVE_CLEARDEPTHSTENCIL,
				NULL, target->texture_color[0], &clearColor, z, stencil, NULL);
	} else {
		for (t = 0; t < tiling->count; t++) {
			d3dDev->SetPredication(D3DPRED_TILE(t));

			if (target->texture_depth != NULL) {
				d3dDev->Resolve(D3DRESOLVE_DEPTHSTENCIL | D3DRESOLVE_FRAGMENT0,
						&tiling->rect[t], target->texture_depth,
						(D3DPOINT *)&tiling->rect[t], 0, 0, NULL, z, stencil, NULL);
			}

			for (i = target->surface_num - 1; i >= 0; i--) {
				if (!(target->flag & (AMD_RENDER_FLAG_RESOLVE_COLOR0 << i)))
					continue;
				d3dDev->Resolve(i | D3DRESOLVE_CLEARRENDERTARGET | D3DRESOLVE_CLEARDEPTHSTENCIL,
						&tiling->rect[t], target->texture_color[i],
						(D3DPOINT *)&tiling->rect[t], 0, 0, &clearColor, z, stencil, NULL);
			}
		}
		d3dDev->SetPredication(0);
		d3dDev->EndTiling(0, NULL, NULL, &clearColor, z, stencil, NULL);
	}
#endif
	} else {
		// タイリングなし
		if (target->flag & AMD_RENDER_FLAG_RESOLVE_DEPTH) {
			d3dDev->Resolve(D3DRESOLVE_DEPTHSTENCIL,
					&rect, target->texture_depth, &point, 0, 0, NULL, 1.0f, 0, NULL);
		}
		for (i = 0; i < target->surface_num; i++) {
			if (!(target->flag & (AMD_RENDER_FLAG_RESOLVE_COLOR0 << i)))
				continue;
			d3dDev->Resolve(i,
					&rect, target->texture_color[i], &point, 0, 0, NULL, 1.0f, 0, NULL);
		}
	}
#elif _PS3
	nnDevicePs3EndScene(_am_ps3_device);
#elif _WII
	UNREFERENCED_PARAMETER(color);
	UNREFERENCED_PARAMETER(z);
	UNREFERENCED_PARAMETER(stencil);

	GXColor		clear_color;
	clear_color.r	= _am_draw_bg_color.r;
	clear_color.g	= _am_draw_bg_color.g;
	clear_color.b	= _am_draw_bg_color.b;
	clear_color.a	= _am_draw_bg_color.a;
	GXSetCopyFilter(_am_wii_render_mode.aa, _am_wii_render_mode.sample_pattern,
			GX_TRUE, _am_wii_render_mode.vfilter);
	GXSetCopyClear(clear_color, _am_draw_clear_depth);
	GXSetDispCopySrc(_am_draw_video.copy_left, _am_draw_video.copy_top,
			_am_draw_video.copy_width, _am_draw_video.copy_height);
	GXCopyDisp(_am_wii_frame_buffer[_am_wii_next_buffer], GX_TRUE);
	GXDrawDone();

	GXInvalidateVtxCache();
	nnResetVertexBufferGC();

	VISetNextFrameBuffer(_am_wii_frame_buffer[_am_wii_next_buffer]);
#endif

	_am_draw_in_scene	= 0;
}


/************************************************************************/
/* void amDrawBeginScene(void)                                          */
/*----------------------------------------------------------------------*/
/* [FUNCTION] シーン描画開始                                            */
/************************************************************************/
void amDrawBeginScene(void)
{
	_am_displaylist_manager.sort_num	= 0;

	_am_draw_work_buf_ptr		= _am_draw_work_buf;
	_am_draw_work_buf_size		= 0;
}


/************************************************************************/
/* void amDrawEndScene(void)                                            */
/*----------------------------------------------------------------------*/
/* [FUNCTION] シーン描画終了(ソート描画の開始)                          */
/************************************************************************/
void amDrawEndScene(void)
{
	AMS_DISPLAYLIST_MANAGER		*manager;
	Sint32		n;

	manager		= &_am_displaylist_manager;
	n		= manager->sort_num;
	manager->sort_num	= 0;

	if (n == 0)
		return;

	// ソート
	AMS_DRAW_SORT		*sort, *sort0, *sort1, tmpsort;
	Sint32		key0, key1;
	Sint32		i, j;

	sort	= &manager->sortlist[0];
	for (i = n - 1; i > 0; i--, sort++) {
		sort0	= sort;
		sort1	= sort + 1;
		key0	= sort0->key;
		for (j = i; j > 0; j--, sort1++) {
			key1	= sort1->key;
			if (key1 < key0)
				continue;
			key0	= key1;
			sort0	= sort1;
		}
		tmpsort	= *sort;
		*sort	= *sort0;
		*sort0	= tmpsort;
	}

	// 描画
	AMS_COMMAND_HEADER	*command;

	amDrawPushState();

	sort	= &manager->sortlist[0];
	for (i = n; i > 0; i--, sort++) {
		command		= sort->command;
		if (command->command_id >= 0) {
			// 描画コマンド実行
			amAssert(_am_draw_command_sort);
			_am_draw_command_sort(command, 0);
		} else {
			// システムコマンド実行
			_am_draw_sort_system_exec[-command->command_id](command, 0);
		}
	}

	amDrawPopState();
}


/************************************************************************/
/* void amDrawBuildShader(void)                                         */
/*----------------------------------------------------------------------*/
/* [FUNCTION] スプライト描画シェーダーの作成                            */
/************************************************************************/
void amDrawBuildShader(void)
{
#if _PC | _XBOX
	LPDIRECT3DDEVICE9	d3ddev;

#if _PC
	d3ddev	= amWinDxGetDirect3DDevice();
#elif _XBOX
	d3ddev	= amXboxDxGetDirect3DDevice();
#endif

	if (_am_draw_sprite_VS == NULL) {
		HRESULT		hr;
#if !AMD_SHADER_PRECOMPILED
		size_t		code_size;
		ID3DXBuffer	*shader_code;

		// 頂点シェーダーの作成
		code_size	= strlen(_am_draw_sprite_code);
		hr	= D3DXCompileShader(_am_draw_sprite_code, code_size,
				NULL, NULL, "main_VS", "vs_3_0", 0,
				// D3DXGetVertexShaderProfile(nng_pd3dDevice), D3DXSHADER_MICROCODE_BACKEND_OLD,
				&shader_code, NULL, NULL);
		hr	= d3ddev->CreateVertexShader(
				(DWORD*)shader_code->GetBufferPointer(), &_am_draw_sprite_VS);
		shader_code->Release();

		// ピクセルシェーダーの作成
		hr	= D3DXCompileShader(_am_draw_sprite_code, code_size,
				NULL, NULL, "main_PS", "ps_3_0", 0,
				// D3DXGetPixelShaderProfile(nng_pd3dDevice), D3DXSHADER_MICROCODE_BACKEND_OLD,
				&shader_code, NULL, NULL);
		hr	= d3ddev->CreatePixelShader(
				(DWORD*)shader_code->GetBufferPointer(), &_am_draw_sprite_PS);
		shader_code->Release();
#else
#if _PC
		// 頂点シェーダーの作成
		hr	= d3ddev->CreateVertexShader(
				(DWORD*)g_vs30_draw_sprite_VS, &_am_draw_sprite_VS);

		// ピクセルシェーダーの作成
		hr	= d3ddev->CreatePixelShader(
				(DWORD*)g_ps30_draw_sprite_PS, &_am_draw_sprite_PS);
#elif _XBOX
		// 頂点シェーダーの作成
		hr	= d3ddev->CreateVertexShader(
				(DWORD*)g_xvs_draw_sprite_VS, &_am_draw_sprite_VS);

		// ピクセルシェーダーの作成
		hr	= d3ddev->CreatePixelShader(
				(DWORD*)g_xps_draw_sprite_PS, &_am_draw_sprite_PS);
#endif
#endif
	}
#endif
}


/************************************************************************/
/* void amDrawDisplay(AMS_RENDER_TARGET *target, Sint32 index)          */
/*----------------------------------------------------------------------*/
/* [INPUT]  target : 表示するレンダーターゲット(NULLならばカレント)     */
/*          index  : target内のテクスチャインデックス                   */
/* [FUNCTION] ディスプレイ描画                                          */
/************************************************************************/
void amDrawDisplay(AMS_RENDER_TARGET *target, Sint32 index)
{
	AMS_RENDER_TARGET	*target0;

#if _PS3
	// どうやってもコピー時にα抜きらしき現象がおきるのでクリアする
	target0		= amRenderSetTarget(NULL, AMD_RENDER_CLEAR_COLOR);
#else
	target0		= amRenderSetTarget(NULL);
#endif
	if (target == NULL)
		target		= target0;

	if (target != NULL) {
		amRenderSetTexture(NNE_TEXSLOT_0, target, index);

#if _PC | _XBOX
		struct TVERTEX {
			float		p[4];
			float		tu, tv;
		} vtx[] = {
			{ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,},
			{ _am_draw_video.disp_width, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,},
			{ 0.0f, _am_draw_video.disp_height, 0.0f, 1.0f, 0.0f, 1.0f,},
			{ _am_draw_video.disp_width, _am_draw_video.disp_height, 0.0f, 1.0f, 1.0f, 1.0f,},
		};

#if _PC
		if (!(_am_draw_video.style & AMD_WIN_STYLE_WII) && !_am_draw_video.wide_screen) {
#elif _XBOX
		if (!_am_draw_video.wide_screen) {
#endif
			float	scale = target->height / _am_draw_video.disp_height;
			vtx[0].tu	= (target->width - _am_draw_video.disp_width * scale)
						/ (target->width * 2.0f);
			vtx[2].tu	= vtx[0].tu;
			vtx[1].tu	= 1.0f - vtx[0].tu;
			vtx[3].tu	= vtx[1].tu;
		}

		amDrawBuildShader();

		LPDIRECT3DDEVICE9	d3ddev;

#if _PC
		d3ddev	= amWinDxGetDirect3DDevice();
#elif _XBOX
		d3ddev	= amXboxDxGetDirect3DDevice();
#endif

#if _PC
		d3ddev->SetTextureStageState(0, D3DTSS_COLOROP,	  D3DTOP_SELECTARG1);
		d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		d3ddev->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
		d3ddev->SetRenderState(D3DRS_FOGENABLE, FALSE);
		d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
#elif _XBOX
		D3DBLENDSTATE	blendstate;
		blendstate.SrcBlend			= D3DBLEND_ONE;
		blendstate.BlendOp			= D3DBLENDOP_ADD;
		blendstate.DestBlend		= D3DBLEND_ZERO;
		blendstate.SrcBlendAlpha	= D3DBLEND_ONE;
		blendstate.BlendOpAlpha		= D3DBLENDOP_ADD;
		blendstate.DestBlendAlpha	= D3DBLEND_ZERO;
		d3ddev->SetBlendState(0, blendstate);
#endif
		d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		D3DVIEWPORT9 vp;
		d3ddev->GetViewport(&vp);
		d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		d3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		d3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);
		d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
#if _PC
		d3ddev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0));
#elif _XBOX
		d3ddev->SetRenderState(D3DRS_VIEWPORTENABLE, FALSE);
		d3ddev->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0));
#endif
		d3ddev->SetVertexShader(_am_draw_sprite_VS);
		d3ddev->SetPixelShader(_am_draw_sprite_PS);
		d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx, sizeof( TVERTEX ));
		d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
#if _XBOX
		d3ddev->SetRenderState(D3DRS_VIEWPORTENABLE, TRUE);
#endif
		//
		nnResetVertexDeclarationDXG20();
#elif _PS3
		NNS_PRIM2D_PCT	vtx[] = {
			{ { 0.0f, 0.0f,}, 0xffffffff, { 0.0f, 0.0f,}, },
			{ { _am_draw_video.disp_width, 0.0f,}, 0xffffffff, { 1.0f, 0.0f,}, },
			{ { 0.0f, _am_draw_video.disp_height,}, 0xffffffff, { 0.0f, 1.0f,}, },
			{ { _am_draw_video.disp_width, _am_draw_video.disp_height,}, 0xffffffff, { 1.0f, 1.0f,}, },
		};

#if !AMD_PS3_USE_LIBRESC
		if (!_am_draw_video.wide_screen) {
			float	scale = _am_draw_video.draw_height / _am_draw_video.disp_height;
			vtx[0].Tex.u	= (_am_draw_video.draw_width - _am_draw_video.disp_width_2d * scale)
						/ (_am_draw_video.draw_width * 2.0f);
			vtx[2].Tex.u	= vtx[0].Tex.u;
			vtx[1].Tex.u	= 1.0f - vtx[0].Tex.u;
			vtx[3].Tex.u	= vtx[1].Tex.u;
		}
#endif

		nnSetPrimitiveTexState(
				NNE_PRIM_TEXBLEND_MODULATE, NNE_PRIM_TEXCOORD_UV,
				NNE_PRIM_TEXWRAP_CLAMP, NNE_PRIM_TEXWRAP_CLAMP);
		nnSetPrimitive2DDepthFuncPS3(NND_CMPFUNC_PS3_ALWAYS);
		nnSetPrimitive2DDepthMaskPS3(NNE_OFF);

		nnBeginDrawPrimitive2D(NNE_PRIM2D_FMT_PCT, NNE_PRIM_ALPHABLEND_OFF);
		nnDrawPrimitive2D(NNE_PRIM_TRIANGLE_STRIP, vtx, 4, -1.0f);
		nnEndDrawPrimitive2D();

		nnSetPrimitive2DAlphaTestPS3(NNE_TRUE);
		nnSetPrimitive2DDepthMaskPS3(NNE_ON);
		nnSetPrimitive2DDepthFuncPS3(NND_CMPFUNC_PS3_LEQUAL);
#elif _WII
#endif
	}

#if _WII
	// HOMEボタン禁止アイコン
	amWiiDrawIconHBM();

	// HOMEボタンメニュー
	amWiiDrawHBM();
#endif

#if _PS3 && AMD_PS3_USE_LIBRESC
	if (!_am_draw_video.wide_screen) {
		float	dx, tx;
		dx		= AMD_DISPLAY_HEIGHT * 4.0f / 3.0f;		// 表示X範囲
		tx		= dx / (float)AMD_TEXT_WIDTH;			// 文字Xサイズ
		_am_draw_offset_x	= (Sint32)((AMD_DISPLAY_WIDTH - dx) * 0.5f / tx);
		nnSetPrintSize(
				tx, AMD_DISPLAY_HEIGHT / (float)AMD_TEXT_HEIGHT);
	} else {
		_am_draw_offset_x	= 0;
		nnSetPrintSize(
				AMD_DISPLAY_WIDTH / (float)AMD_TEXT_WIDTH,
				AMD_DISPLAY_HEIGHT / (float)AMD_TEXT_HEIGHT);
	}
#else
	_am_draw_offset_x	= 0;
	nnSetPrintSize(
			AMD_DISPLAY_WIDTH / (float)AMD_TEXT_WIDTH,
			AMD_DISPLAY_HEIGHT / (float)AMD_TEXT_HEIGHT);
#endif
}


/************************************************************************/
/* void amDrawInitDisplayList(Sint32 user_header_size)                  */
/*----------------------------------------------------------------------*/
/* [INPUT] user_header_size : ユーザーヘッダのサイズ                    */
/* [FUNCTION] ディスプレイリストの初期化                                */
/************************************************************************/
void amDrawInitDisplayList(Sint32 user_header_size)
{
	AMS_DISPLAYLIST_MANAGER		*manager;
	AMS_DISPLAYLIST				*displaylist;
	Sint32		i;

	_am_draw_task	= amTaskInitSystem(
			AMD_TASK_DEFAULT_BUFFER_SIZE, AMD_TASK_DEFAULT_WORK_SIZE, 1);

	manager		= &_am_displaylist_manager;

	amMutexCreate(&manager->mutex);
#if AMD_TASK_THREAD_NUM > 1
	amMutexCreate(&manager->command_buf_mutex);
	amMutexCreate(&manager->data_buf_mutex);
	amMutexCreate(&manager->regist_buf_mutex);
#endif

	amMutexLock(&manager->mutex);
#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&manager->command_buf_mutex);
	amMutexLock(&manager->data_buf_mutex);
	amMutexLock(&manager->regist_buf_mutex);
#endif

	manager->write_index	= 0;
	manager->last_index		= -1;
	manager->read_index		= -1;
	manager->user_header_size	= user_header_size;

	displaylist	= &manager->displaylist[0];
	for (i = 0; i < AMD_DISPLAYLIST_NUM; i++, displaylist++) {
		displaylist->counter		= -1;		// no data
		displaylist->command_buf	= _am_draw_command_buf[i];
		displaylist->data_buf		= _am_draw_data_buf[i];
		displaylist->command_buf_size	= 0;
		displaylist->data_buf_size		= 0;
	}

	manager->regist_num		= 0;
	manager->reg_read_index	= 0;
	manager->reg_end_index	= 0;
	manager->reg_write_index	= 0;
	manager->reg_write_num	= 0;

#if _PC | _XBOX | _PS3
	manager->reg_flag		= 0;
#endif

	amDrawOpenDisplayList();

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&manager->data_buf_mutex);
	amMutexUnlock(&manager->command_buf_mutex);
	amMutexUnlock(&manager->regist_buf_mutex);
#endif
	amMutexUnlock(&manager->mutex);
}


/************************************************************************/
/* void amDrawExitDisplayList(void)                                     */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ディスプレイリストの終了                                  */
/************************************************************************/
void amDrawExitDisplayList(void)
{
	AMS_DISPLAYLIST_MANAGER		*manager;

	manager		= &_am_displaylist_manager;

	amTaskExitSystem(_am_draw_task);

#if AMD_TASK_THREAD_NUM > 1
	amMutexDelete(&manager->data_buf_mutex);
	amMutexDelete(&manager->command_buf_mutex);
	amMutexDelete(&manager->regist_buf_mutex);
#endif
	amMutexDelete(&manager->mutex);
}


/************************************************************************/
/* void amDrawOpenDisplayList(void)                                     */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ディスプレイリストのオープン                              */
/************************************************************************/
void amDrawOpenDisplayList(void)
{
	AMS_DISPLAYLIST_MANAGER		*manager;
	AMS_DISPLAYLIST				*displaylist;
	AMS_COMMAND_BUFFER_HEADER	*header;

	manager		= &_am_displaylist_manager;

	amMutexLock(&manager->mutex);
#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&manager->command_buf_mutex);
	amMutexLock(&manager->data_buf_mutex);
#endif

	displaylist	= &manager->displaylist[manager->write_index];
	displaylist->command_buf_size	=
			sizeof(AMS_COMMAND_BUFFER_HEADER) + manager->user_header_size;
	displaylist->data_buf_size		= 0;
	displaylist->counter		= 0;		// 作成中

	header		= (AMS_COMMAND_BUFFER_HEADER *)displaylist->command_buf;
	manager->write_header		= header;
	memcpy(header->system_flag, _am_system_flag, sizeof(Uint32) * 4);
	memcpy(header->debug_flag, _am_debug_flag, sizeof(Uint32) * 4);
	header->user_header_size	= manager->user_header_size;
	header->display_flag		= 0;
#if _PC | _XBOX | _PS3
	header->regist_flag			= manager->reg_flag;
#endif
	header->icon_alpha			= 0.0f;
	manager->write_user_header	= (char *)(header + 1);
	header		= (AMS_COMMAND_BUFFER_HEADER *)(
			(Uint32)(header + 1) + manager->user_header_size);

	manager->command_buf_ptr	= (char *)header;
	manager->data_buf_ptr		= displaylist->data_buf;

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&manager->data_buf_mutex);
	amMutexUnlock(&manager->command_buf_mutex);
#endif
	amMutexUnlock(&manager->mutex);
}


/************************************************************************/
/* void amDrawCloseDisplayList(void)                                    */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ディスプレイリストのクローズ                              */
/*            次のディスプレイリストのオープンも一緒にする              */
/************************************************************************/
void amDrawCloseDisplayList(void)
{
	AMS_DISPLAYLIST_MANAGER		*manager;

	manager		= &_am_displaylist_manager;

	amMutexLock(&manager->mutex);

	for (Sint32 i = 0; i < AMD_DISPLAYLIST_NUM; i++) {
		if (manager->displaylist[i].counter >= 0)
			manager->displaylist[i].counter++;
	}

	manager->last_index		= manager->write_index;
	manager->write_index	= (manager->write_index + 1) % AMD_DISPLAYLIST_NUM;
	if (manager->write_index == manager->read_index)
		manager->write_index	= (manager->write_index + 1) % AMD_DISPLAYLIST_NUM;

//	amSystemLog("write = %d\n", manager->write_index);

	manager->regist_num		+= manager->reg_write_num;
	manager->reg_end_index	= manager->reg_write_index;
	manager->reg_write_num	= 0;

	amDrawOpenDisplayList();

	amMutexUnlock(&manager->mutex);
}


/************************************************************************/
/* Sint32 amDrawGetDisplayList(void)                                    */
/*----------------------------------------------------------------------*/
/* [RETURN] 取得したディスプレイリストインデックス                      */
/* [FUNCTION] 読み出し用ディスプレイリストの取得                        */
/************************************************************************/
Sint32 amDrawGetDisplayList(void)
{
	AMS_DISPLAYLIST_MANAGER		*manager;
	AMS_COMMAND_BUFFER_HEADER	*header;
	Sint32		ret;

	manager		= &_am_displaylist_manager;

	amMutexLock(&manager->mutex);

#if AMD_DISPLAYLIST_NUM == 3
	manager->read_index		= manager->last_index;
	ret		= manager->last_index;
#else
	Sint32	i, index, cnt0, cnt_old, count;
	ret		= manager->last_index;
	if (manager->read_index != -1) {
		index	= manager->read_index;
		cnt_old	= manager->displaylist[index].counter;
		cnt0	= 0;
		for (i = 0; i < AMD_DISPLAYLIST_NUM - 1; i++) {
			index	= (index + 1) % AMD_DISPLAYLIST_NUM;
			count	= manager->displaylist[index].counter;
			if ((count <= cnt0) || (count >= cnt_old))
				continue;
			ret		= index;
			cnt0	= count;
		}
	}
	manager->read_index		= ret;
#endif

//	amSystemLog("read  = %d\n", manager->read_index);

	header		= (AMS_COMMAND_BUFFER_HEADER *)
			manager->displaylist[ret].command_buf;
	manager->read_header	= header;
	manager->read_user_header	= (char *)(header + 1);

	amMutexUnlock(&manager->mutex);

	return	ret;
}


/************************************************************************/
/* void amDrawSetDisplayFlag(Sint32 flag, float alpha)                  */
/*----------------------------------------------------------------------*/
/* [INPUT]  flag  : 表示フラグ                                          */
/*          alpha : HOMEボタン禁止アイコンα値(Wiiのみ有効)             */
/* [FUNCTION] 表示フラグとHOMEボタン禁止アイコンのα値を設定する        */
/************************************************************************/
void amDrawSetDisplayFlag(Sint32 flag, float alpha)
{
	AMS_COMMAND_BUFFER_HEADER	*header;

	header		= amDrawGetWriteHeader();

	header->display_flag	= (Uint16)flag;
	header->icon_alpha		= alpha;
}


/************************************************************************/
/* void amDrawSetSystemFlagBool(Sint32 id, BOOL val)                    */
/*----------------------------------------------------------------------*/
/* [INPUT]  id  : フラグID(0-127)                                       */
/*          val : 設定する値(1bit)                                      */
/* [FUNCTION] システムフラグにBOOL値を設定する                          */
/************************************************************************/
void amDrawSetSystemFlagBool(Sint32 id, BOOL val)
{
	Uint32	flag, bits;

	bits	= id & 31;
	id		>>= 5;
	val		&= 1;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_displaylist_manager.command_buf_mutex);
#endif

	flag	= _am_displaylist_manager.write_header->system_flag[id];
	flag	&= ~(1 << bits);
	flag	|= (Uint32)val << bits;
	_am_displaylist_manager.write_header->system_flag[id]	= flag;

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_displaylist_manager.command_buf_mutex);
#endif
}


/************************************************************************/
/* BOOL amDrawGetSystemFlagBool(Sint32 id)                              */
/*----------------------------------------------------------------------*/
/* [INPUT]  id  : フラグID(0-127)                                       */
/* [FUNCTION] システムフラグからBOOL値を取得する                        */
/************************************************************************/
BOOL amDrawGetSystemFlagBool(Sint32 id)
{
	Uint32	flag, bits;

	bits	= 1 << (id & 31);
	id		>>= 5;

	flag	= _am_displaylist_manager.read_header->system_flag[id];

	return	(flag & bits)? TRUE: FALSE;
}


/************************************************************************/
/* void amDrawRegistCommand(Uint32 state, Sint32 command_id, void *param)*/
/*----------------------------------------------------------------------*/
/* [INPUT] state      : 描画ステート                                    */
/*         command_id : 描画コマンドID                                  */
/*         param      : パラメータへのポインタ                          */
/* [FUNCTION] ディスプレイリストにコマンドを登録する                    */
/************************************************************************/
void amDrawRegistCommand(Uint32 state, Sint32 command_id, void *param)
{
	AMS_DISPLAYLIST_MANAGER		*manager;
	AMS_DISPLAYLIST				*displaylist;
	AMS_COMMAND_HEADER			*header;

	manager		= &_am_displaylist_manager;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&manager->command_buf_mutex);
#endif

	displaylist	= &manager->displaylist[manager->write_index];
	displaylist->command_buf_size	+= sizeof(AMS_COMMAND_HEADER);
#if AMD_DEBUG
	amAssert(displaylist->command_buf_size <= _am_draw_command_buf_max);
#endif

	header		= (AMS_COMMAND_HEADER *)manager->command_buf_ptr;
	header->state	= state;
	header->command_id	= command_id;
	header->param	= param;
	header++;

	manager->command_buf_ptr	= (char *)header;

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&manager->command_buf_mutex);
#endif
}


/************************************************************************/
/* Sint32 amDrawRegistCommand(Sint32 command_id, void *param)           */
/*----------------------------------------------------------------------*/
/* [INPUT] command_id : 登録コマンドID                                  */
/*         param      : パラメータへのポインタ                          */
/* [RETURN]  登録インデックス                                           */
/* [FUNCTION] 登録リストにコマンドを登録する                            */
/************************************************************************/
Sint32 amDrawRegistCommand(Sint32 command_id, void *param)
{
	AMS_DISPLAYLIST_MANAGER		*manager;
	AMS_REGISTLIST		*regist;
	Sint32		windex;

	manager		= &_am_displaylist_manager;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&manager->regist_buf_mutex);
#endif

	amAssert(manager->regist_num + manager->reg_write_num < AMD_REGISTLIST_NUM);

	windex		= manager->reg_write_index;
	regist		= &manager->registlist[windex];
	regist->command_id		= command_id;
	if (param != NULL)
		memcpy(regist->param, param, sizeof(Sint32) * 11);
	else
		memset(regist->param, 0, sizeof(Sint32) * 11);
	manager->reg_write_index	= (windex + 1) % AMD_REGISTLIST_NUM;
	manager->reg_write_num++;

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&manager->regist_buf_mutex);
#endif

	return	windex;
}


/************************************************************************/
/* Sint32 amDrawIsRegistComplete(Sint32 index)                          */
/*----------------------------------------------------------------------*/
/* [INPUT] index : 登録インデックス                                     */
/* [RETURN]  完了していたら1 していなかったら0                          */
/* [FUNCTION]  登録したコマンドの実行完了チェック                       */
/************************************************************************/
Sint32 amDrawIsRegistComplete(Sint32 index)
{
	return	(_am_displaylist_manager.registlist[index].command_id == 0)? 1: 0;
}


/************************************************************************/
/* char *amDrawMallocDataBuffer(Sint32 size)                            */
/*----------------------------------------------------------------------*/
/* [INPUT] size : 確保するデータ領域のサイズ                            */
/* [RETURN]  確保したデータバッファ                                     */
/* [FUNCTION] ディスプレイリストのデータバッファを確保する              */
/************************************************************************/
char *amDrawMallocDataBuffer(Sint32 size)
{
	AMS_DISPLAYLIST_MANAGER		*manager;
	AMS_DISPLAYLIST				*displaylist;
	char		*ret;

	manager		= &_am_displaylist_manager;

	ret		= amDrawGetDataBuffer();

	displaylist	= &manager->displaylist[manager->write_index];
	displaylist->data_buf_size	+= size;

	manager->data_buf_ptr	= ret + size;
#if AMD_DEBUG
	amAssert(displaylist->data_buf_size <= _am_draw_data_buf_max);
#endif

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&manager->data_buf_mutex);
#endif

	return	ret;
}


/************************************************************************/
/* void amDrawIncDataBuffer(Sint32 size)                                */
/*----------------------------------------------------------------------*/
/* [INPUT] size : 確保するデータ領域のサイズ                            */
/* [FUNCTION] ディスプレイリストのデータバッファのポインタを進める      */
/************************************************************************/
void amDrawIncDataBuffer(Sint32 size)
{
	AMS_DISPLAYLIST_MANAGER		*manager;
	AMS_DISPLAYLIST				*displaylist;
	char		*ptr;

	ptr		= amDrawGetDataBuffer();
	ptr		= ptr + size;

	manager		= &_am_displaylist_manager;
	manager->data_buf_ptr	= ptr;

	displaylist	= &manager->displaylist[manager->write_index];
	displaylist->data_buf_size	= (Sint32)ptr - (Sint32)displaylist->data_buf;

#if AMD_DEBUG
	amAssert(displaylist->data_buf_size <= _am_draw_data_buf_max);
#endif

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&manager->data_buf_mutex);
#endif
}


/************************************************************************/
/* char *amDrawGetDataBuffer(void)                                      */
/*----------------------------------------------------------------------*/
/* [RETURN]  データバッファ                                             */
/* [FUNCTION] ディスプレイリストのデータバッファを取得する              */
/*            データが可変長な場合などに使用する                        */
/************************************************************************/
char *amDrawGetDataBuffer(void)
{
	AMS_DISPLAYLIST_MANAGER		*manager;
	char		*ret;

	manager		= &_am_displaylist_manager;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&manager->data_buf_mutex);
#endif
	ret		= (char *)(((Sint32)manager->data_buf_ptr + 15) & ~15);

	return	ret;
}


/************************************************************************/
/* void amDrawSetDataBuffer(char *ptr)                                  */
/*----------------------------------------------------------------------*/
/* [INPUT] ptr : 設定するのデータバッファ                               */
/* [FUNCTION] ディスプレイリストのデータバッファを設定する              */
/*            データが可変長な場合などに使用する                        */
/************************************************************************/
void amDrawSetDataBuffer(char *ptr)
{
	AMS_DISPLAYLIST_MANAGER		*manager;
	AMS_DISPLAYLIST				*displaylist;

	manager		= &_am_displaylist_manager;
	displaylist	= &manager->displaylist[manager->write_index];

	manager->data_buf_ptr	= ptr;
	displaylist->data_buf_size	= (Sint32)ptr - (Sint32)displaylist->data_buf;

#if AMD_DEBUG
	amAssert(displaylist->data_buf_size >= 0);
	amAssert(displaylist->data_buf_size <= _am_draw_data_buf_max);
#endif

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&manager->data_buf_mutex);
#endif
}


/************************************************************************/
/* char *amDrawMallocWorkBuffer(Sint32 size)                            */
/*----------------------------------------------------------------------*/
/* [INPUT] size : 確保するワーク領域のサイズ                            */
/* [RETURN]  確保したワーク領域                                         */
/* [FUNCTION] 描画用ワーク領域を確保する                                */
/************************************************************************/
char *amDrawMallocWorkBuffer(Sint32 size)
{
	char		*ret;

	ret		= amDrawGetWorkBuffer();

	_am_draw_work_buf_size	+= size;

	_am_draw_work_buf_ptr	= ret + size;
#if AMD_DEBUG
	amAssert(_am_draw_work_buf_size <= _am_draw_work_buf_max);
#endif

	return	ret;
}


/************************************************************************/
/* void amDrawIncWorkBuffer(Sint32 size)                                */
/*----------------------------------------------------------------------*/
/* [INPUT] size : 確保するワーク領域のサイズ                            */
/* [FUNCTION] 描画用ワーク領域のポインタを進める                        */
/************************************************************************/
void amDrawIncWorkBuffer(Sint32 size)
{
	char		*ptr;

	ptr		= amDrawGetWorkBuffer();
	ptr		= ptr + size;

	_am_draw_work_buf_ptr	= ptr;
	_am_draw_work_buf_size	= (Sint32)ptr - (Sint32)_am_draw_work_buf;

#if AMD_DEBUG
	amAssert(_am_draw_work_buf_size <= _am_draw_work_buf_max);
#endif
}


/************************************************************************/
/* char *amDrawGetWorkBuffer(void)                                      */
/*----------------------------------------------------------------------*/
/* [RETURN]  ワークバッファ                                             */
/* [FUNCTION] ワークバッファを取得する                                  */
/*            ワークが可変長な場合などに使用する                        */
/************************************************************************/
char *amDrawGetWorkBuffer(void)
{
	char		*ret;

	ret		= (char *)(((Sint32)_am_draw_work_buf_ptr + 15) & ~15);

	return	ret;
}


/************************************************************************/
/* void amDrawSetWorkBuffer(char *ptr)                                  */
/*----------------------------------------------------------------------*/
/* [INPUT] ptr : 設定するのワークバッファ                               */
/* [FUNCTION] ワークバッファを設定する                                  */
/*            ワークが可変長な場合などに使用する                        */
/************************************************************************/
void amDrawSetWorkBuffer(char *ptr)
{
	_am_draw_work_buf_ptr	= ptr;
	_am_draw_work_buf_size	= (Sint32)ptr - (Sint32)_am_draw_work_buf;

#if AMD_DEBUG
	amAssert(_am_draw_work_buf_size >= 0);
	amAssert(_am_draw_work_buf_size <= _am_draw_work_buf_max);
#endif
}


/************************************************************************/
/* void amDrawInitState(void)                                           */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 描画環境の初期化                                          */
/************************************************************************/
void amDrawInitState(void)
{
	Sint32			slot;
	NNS_MATRIX		*mtx;

	_am_draw_state.drawflag			= 0;
	_am_draw_state.diffuse.mode		= NNE_MATCTRLMODE_MODULATE;
	_am_draw_state.diffuse.r		= 1.0f;
	_am_draw_state.diffuse.g		= 1.0f;
	_am_draw_state.diffuse.b		= 1.0f;
	_am_draw_state.ambient.mode		= NNE_MATCTRLMODE_MODULATE;
	_am_draw_state.ambient.r		= 1.0f;
	_am_draw_state.ambient.g		= 1.0f;
	_am_draw_state.ambient.b		= 1.0f;
	_am_draw_state.alpha.mode		= NNE_MATCTRLMODE_MODULATE;
	_am_draw_state.alpha.alpha		= 1.0f;
	_am_draw_state.specular.mode	= NNE_MATCTRLMODE_MODULATE;
	_am_draw_state.specular.r		= 1.0f;
	_am_draw_state.specular.g		= 1.0f;
	_am_draw_state.specular.b		= 1.0f;
	_am_draw_state.blend.mode		= NNE_MATCTRL_BLEND_ALPHA;
	_am_draw_state.envmap.texsrc	= NNE_MATCTRL_TEXCOORDSRC_NORMAL;
	_am_draw_state.zmode.compare	= NNE_TRUE;
	_am_draw_state.zmode.func		= AMD_ZFUNC_DEFAULT;
	_am_draw_state.zmode.update		= NNE_TRUE;
	mtx		= &_am_draw_state.envmap.texmtx;
	nnMakeUnitMatrix(mtx);
	nnTranslateMatrix(mtx, mtx, 0.5f, 0.5f, 0.0f);
	nnScaleMatrix(mtx, mtx, 0.5f, 0.5f, 0.0f);
	for (slot = 0; slot < 4; slot++) {
		_am_draw_state.texoffset[slot].mode	= NNE_MATCTRLMODE_ADD;
		_am_draw_state.texoffset[slot].u	= 0.0f;
		_am_draw_state.texoffset[slot].v	= 0.0f;
	}

	amDrawSetState(&_am_draw_state);
}


/************************************************************************/
/* void amDrawPushState(void)                                           */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 描画環境のプッシュ                                        */
/************************************************************************/
void amDrawPushState(void)
{
	amAssert(_am_draw_state_stack_num < AMD_DRAWSTATE_STACK_NUM);

	_am_draw_state_stack[_am_draw_state_stack_num++]	= _am_draw_state;
}


/************************************************************************/
/* void amDrawPopState(void)                                            */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 描画環境のポップ                                          */
/************************************************************************/
void amDrawPopState(void)
{
	amAssert(_am_draw_state_stack_num > 0);

	_am_draw_state_stack_num--;

	amDrawSetState(&_am_draw_state_stack[_am_draw_state_stack_num]);
}


/************************************************************************/
/* void amDrawSetState(AMS_DRAWSTATE *state)                            */
/*----------------------------------------------------------------------*/
/* [INPUT]  state : 描画環境                                            */
/* [FUNCTION] 描画環境の設定                                            */
/************************************************************************/
void amDrawSetState(AMS_DRAWSTATE *state)
{
	Sint32		slot;

	_am_draw_state		= *state;

	nnSetMaterialControlDiffuse(state->diffuse.mode,
			state->diffuse.r, state->diffuse.g, state->diffuse.b);
	nnSetMaterialControlAmbient(state->ambient.mode,
			state->ambient.r, state->ambient.g, state->ambient.b);
	nnSetMaterialControlAlpha(state->alpha.mode, state->alpha.alpha);
#if _PC | _XBOX
	nnSetMaterialControlSpecularDXG20(state->specular.mode,
			state->specular.r, state->specular.g, state->specular.b);
#elif _PS3
	nnSetMaterialControlSpecularPS3(state->specular.mode,
			state->specular.r, state->specular.g, state->specular.b);
#elif _WII
	nnSetMaterialControlSpecularGC(state->specular.mode,
			state->specular.r, state->specular.g, state->specular.b);
#endif
	nnSetMaterialControlBlendMode(state->blend.mode);
	nnSetMaterialControlEnvTexMatrix(state->envmap.texsrc,
			&_am_draw_state.envmap.texmtx);
	for (slot = 0; slot < 4; slot++) {
		nnSetMaterialControlTextureOffset((NNE_TEXSLOT)slot,
				state->texoffset[slot].mode,
				state->texoffset[slot].u, state->texoffset[slot].v);
	}

	nnSetFogSwitch((NNE_BOOL)state->fog.flag);
	nnSetFogColor(
			state->fog_color.r, state->fog_color.g, state->fog_color.b);
#if !_WII
	nnSetFogRange(state->fog_range.fnear, state->fog_range.ffar);
#else
	nnSetFogRangeGC((amDrawGetProjectionType() == NNE_PROJECTION_TYPE_ORTHO)?
			GX_FOG_ORTHO_LIN : GX_FOG_PERSP_LIN, state->fog_range.fnear, state->fog_range.ffar);
#endif

#if _PC | _XBOX
	nnSetPrimitive2DDepthTestDXG20((NNE_BOOL)state->zmode.compare);
	nnSetPrimitive2DDepthFuncDXG20((NNE_CMPFUNC)state->zmode.func);
	nnSetPrimitive2DDepthMaskDXG20((NNE_BOOL)state->zmode.update);

	nnSetPrimitive3DDepthTestDXG20((NNE_BOOL)state->zmode.compare);
	nnSetPrimitive3DDepthFuncDXG20((NNE_CMPFUNC)state->zmode.func);
	nnSetPrimitive3DDepthMaskDXG20((NNE_BOOL)state->zmode.update);

	nnSetZModeDXG20((NNE_BOOL)state->zmode.compare,
			(NNE_CMPFUNC)state->zmode.func, (NNE_BOOL)state->zmode.update);
#elif _PS3
	nnSetPrimitive2DDepthTestPS3((NNE_BOOL)state->zmode.compare);
	nnSetPrimitive2DDepthFuncPS3(state->zmode.func);
	nnSetPrimitive2DDepthMaskPS3((NNE_BOOL)state->zmode.update);

	nnSetPrimitive3DDepthTestPS3((NNE_BOOL)state->zmode.compare);
	nnSetPrimitive3DDepthFuncPS3(state->zmode.func);
	nnSetPrimitive3DDepthMaskPS3((NNE_BOOL)state->zmode.update);
#elif _WII
//	nnSetZModeGC((GXBool)state->zmode.compare,
//			(GXCompare)state->zmode.func, (GXBool)state->zmode.update);
#endif
}


/************************************************************************/
/* AMS_DRAWSTATE *amDrawGetState(AMS_DRAWSTATE *state)                  */
/*----------------------------------------------------------------------*/
/* [OUTPUT]  state : 描画環境                                           */
/* [RETURN]  描画環境（参照のみならこちらで可）                         */
/* [FUNCTION] 描画環境の取得                                            */
/************************************************************************/
AMS_DRAWSTATE *amDrawGetState(AMS_DRAWSTATE *state)
{
	if (state != NULL)
		*state		= _am_draw_state;

	return	&_am_draw_state;
}


/************************************************************************/
/* void amDrawSetProjection(NNS_MATRIX44 *proj_mtx,                     */
/*                                       NNE_PROJECTION_TYPE proj_type) */
/*----------------------------------------------------------------------*/
/* [INPUT] proj_mtx  : プロジェクションマトリクス                       */
/*         proj_type : プロジェクションタイプ                           */
/* [FUNCTION] プロジェクションマトリクスの設定                          */
/************************************************************************/
void amDrawSetProjection(NNS_MATRIX44 *proj_mtx, NNE_PROJECTION_TYPE proj_type)
{
	memcpy(&_am_draw_proj_mtx, proj_mtx, sizeof(NNS_MATRIX44));
	_am_draw_proj_type		= proj_type;

	nnSetProjection(proj_mtx, proj_type);
}


/************************************************************************/
/* NNS_MATRIX44 *amDrawGetProjectionMatrix(void)                        */
/*----------------------------------------------------------------------*/
/* [RETURN] プロジェクションマトリクス(read only)                       */
/* [FUNCTION] プロジェクションマトリクスの取得                          */
/************************************************************************/
NNS_MATRIX44 *amDrawGetProjectionMatrix(void)
{
	return	&_am_draw_proj_mtx;
}


/************************************************************************/
/* NNE_PROJECTION_TYPE amDrawGetProjectionType(void)                    */
/*----------------------------------------------------------------------*/
/* [RETURN] プロジェクションタイプ(read only)                           */
/* [FUNCTION] プロジェクションタイプの取得                              */
/************************************************************************/
NNE_PROJECTION_TYPE amDrawGetProjectionType(void)
{
	return	_am_draw_proj_type;
}


/************************************************************************/
/* void amDrawExecute(void)                                             */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ディスプレイリストタスクの実行                            */
/************************************************************************/
void amDrawExecute(void)
{
	amTaskExecute(_am_draw_task);
	amTaskReset(_am_draw_task);
}


/************************************************************************/
/* void amDrawExecCommand(Uint32 state, NNF_DRAWOBJ drawflag)           */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 実行するステート                                  */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] ディスプレイリストの実行                                  */
/************************************************************************/
void amDrawExecCommand(Uint32 state, NNF_DRAWOBJ drawflag)
{
	AMS_DISPLAYLIST_MANAGER		*manager;
	AMS_DISPLAYLIST				*displaylist;
	AMS_COMMAND_BUFFER_HEADER	*header;
	AMS_COMMAND_HEADER			*command;
	Sint32		endp;

	manager		= &_am_displaylist_manager;
	displaylist	= &manager->displaylist[manager->read_index];

	header		= (AMS_COMMAND_BUFFER_HEADER *)displaylist->command_buf;
	command		= (AMS_COMMAND_HEADER *)(header + 1);
	endp		= (Sint32)displaylist->command_buf + displaylist->command_buf_size;

	for (; (Sint32)command < endp; command++) {
		// ステートチェック
		if (command->state == state) {
			if (command->command_id >= 0) {
				// 描画コマンド実行
				amAssert(_am_draw_command_func);
				_am_draw_command_func(command, drawflag);
			} else {
				// システムコマンド実行
				_am_draw_system_exec[-command->command_id](command, drawflag);
			}
		}
	}
}


/************************************************************************/
/* void amDrawExecRegist(void)                                          */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 登録リストの実行                                          */
/************************************************************************/
void amDrawExecRegist(void)
{
	AMS_DISPLAYLIST_MANAGER		*manager;
	AMS_REGISTLIST		*regist;
	Sint32		rindex, eindex, num;

	manager	= &_am_displaylist_manager;

	amMutexLock(&manager->mutex);

	num		= manager->regist_num;
	rindex	= manager->reg_read_index;
	eindex	= manager->reg_end_index;

	amMutexUnlock(&manager->mutex);

#if _PC | _XBOX
	IDirect3DDevice9		*d3ddev;
#if _PC
	d3ddev	= amWinDxGetIDirect3DDevice();
#elif _XBOX
	d3ddev	= amXboxDxGetIDirect3DDevice();
#endif
	d3ddev->SetTexture(0, NULL);
	d3ddev->SetTexture(1, NULL);
	d3ddev->SetTexture(2, NULL);
	d3ddev->SetTexture(3, NULL);
	d3ddev->SetVertexShader(NULL);
	d3ddev->SetPixelShader(NULL);
#elif _PS3
#elif _WII
#endif

	if (num == 0)
		return;

#if _PC | _XBOX | _PS3
	_am_draw_shader_compile		=
			!(amDrawGetReadHeader()->regist_flag & AMD_REGFLG_NO_SHADER_COMPILE);
	_am_draw_regist_flag	= 0;
#endif

	num		= 0;
	while (rindex != eindex) {
		regist	= &manager->registlist[rindex];
		rindex	= (rindex + 1) % AMD_REGISTLIST_NUM;
		num++;

		_am_draw_regist_func[regist->command_id](regist);
	}

#if _PC | _XBOX | _PS3
	if (_am_draw_regist_flag & AMD_REG_FLAG_BUILD_SHADER) {
#if _PS3
		if (_am_draw_video.style & AMD_PS3_STYLE_BUILD_MAIN) {
			amAlarmClear(&_am_ps3_build_end);
			amAlarmSet(&_am_ps3_build_req);
			amAlarmWait(&_am_ps3_build_end);
		} else
#endif
			amShaderBuildStd();
	}
#endif

	amMutexLock(&manager->mutex);

	manager->regist_num		-= num;
	manager->reg_read_index	= rindex;

	amMutexUnlock(&manager->mutex);
}


/************************************************************************/
/* void amDrawAddSort(AMS_COMMAND_HEADER *command, Sint32 key)          */
/*----------------------------------------------------------------------*/
/* [INPUT] command : 描画コマンドヘッダ                                 */
/*         key     : ソートキー                                         */
/* [FUNCTION] ソートリストへの描画コマンド登録                          */
/************************************************************************/
void amDrawAddSort(AMS_COMMAND_HEADER *command, Sint32 key)
{
	AMS_DISPLAYLIST_MANAGER		*manager;
	AMS_DRAW_SORT		*sort;

	manager		= &_am_displaylist_manager;

//	amAssert(manager->sort_num < AMD_SORTLIST_NUM);
	if (manager->sort_num >= AMD_SORTLIST_NUM) {
		amSystemLog("[WARN] sort_num over.\n");
		return;
	}

	sort	= &manager->sortlist[manager->sort_num++];
	sort->key		= key;
	sort->command	= command;
}


#if AMD_USE_VIRTUAL_RESOLUTION_2D
void *_amDrawConvVertex2D(NNS_PRIM2D_P *vs, Sint32 count)
{
	void			*work;
	NNS_PRIM2D_P	*vd;

	work	= amDrawMallocWorkBuffer(sizeof(NNS_PRIM2D_P) * count);

	for (vd = (NNS_PRIM2D_P *)work; count > 0; count--, vs++, vd++) {
		amDrawConv2D(&vd->Pos, &vs->Pos);
	}

	return	work;
}

void *_amDrawConvVertex2D(NNS_PRIM2D_PC *vs, Sint32 count)
{
	void			*work;
	NNS_PRIM2D_PC	*vd;

	work	= amDrawMallocWorkBuffer(sizeof(NNS_PRIM2D_PC) * count);

	for (vd = (NNS_PRIM2D_PC *)work; count > 0; count--, vs++, vd++) {
		amDrawConv2D(&vd->Pos, &vs->Pos);
		vd->Col		= vs->Col;
	}

	return	work;
}

void *_amDrawConvVertex2D(NNS_PRIM2D_PCT *vs, Sint32 count)
{
	void			*work;
	NNS_PRIM2D_PCT	*vd;

	work	= amDrawMallocWorkBuffer(sizeof(NNS_PRIM2D_PC) * count);

	for (vd = (NNS_PRIM2D_PCT *)work; count > 0; count--, vs++, vd++) {
		amDrawConv2D(&vd->Pos, &vs->Pos);
		vd->Col		= vs->Col;
		vd->Tex		= vs->Tex;
	}

	return	work;
}

/************************************************************************/
/* void amDrawPrimitive2D(NNE_PRIM2D_FMT format,                        */
/*                            const void *vtx, Sint32 count, float pri) */
/*----------------------------------------------------------------------*/
/* [INPUT] format : 頂点フォーマット                                    */
/*         type   : トライアングルプリミティブタイプ                    */
/*         vtx    : プリミティブ頂点データへのポインタ                  */
/*         count  : 頂点数                                              */
/*         pri    : プライオリティ                                      */
/* [FUNCTION] 2Dプリミティブ描画                                        */
/************************************************************************/
void amDrawPrimitive2D(NNE_PRIM2D_FMT format, NNE_PRIM_TRIANGLE type, const void *vtx, Sint32 count, float pri)
{
	switch (format) {
		case	NNE_PRIM2D_FMT_PC:
			vtx		= _amDrawConvVertex2D((NNS_PRIM2D_PC *)vtx, count);
			break;
		case	NNE_PRIM2D_FMT_PCT:
			vtx		= _amDrawConvVertex2D((NNS_PRIM2D_PCT *)vtx, count);
			break;
	}

	nnDrawPrimitive2D(type, vtx, count, pri);
}


/************************************************************************/
/* void amDrawPrimitiveLine2D(NNE_PRIM_LINE type,                       */
/*                            const void *vtx, Sint32 count, float pri) */
/*----------------------------------------------------------------------*/
/* [INPUT] type   : ラインプリミティブタイプ                            */
/*         vtx    : プリミティブ頂点データへのポインタ                  */
/*         count  : 頂点数                                              */
/*         pri    : プライオリティ                                      */
/* [FUNCTION] 2Dラインプリミティブ描画                                  */
/************************************************************************/
void amDrawPrimitiveLine2D(NNE_PRIM_LINE type, const void *vtx, Sint32 count, float pri)
{
	vtx		= _amDrawConvVertex2D((NNS_PRIM2D_P *)vtx, count);

	nnDrawPrimitiveLine2D(type, vtx, count, pri);
}


/************************************************************************/
/* void amDrawPrimitivePoint2D(NNE_PRIM2D_POINT_FMT format,             */
/*                            const void *vtx, Sint32 count, float pri) */
/*----------------------------------------------------------------------*/
/* [INPUT] format : 頂点フォーマット                                    */
/*         vtx    : プリミティブ頂点データへのポインタ                  */
/*         count  : 頂点数                                              */
/*         pri    : プライオリティ                                      */
/* [FUNCTION] 2Dポイントプリミティブ描画                                */
/************************************************************************/
void amDrawPrimitivePoint2D(NNE_PRIM2D_POINT_FMT format, const void *vtx, Sint32 count, float pri)
{
	switch (format) {
		case	NNE_PRIM2D_FMT_P:
			vtx		= _amDrawConvVertex2D((NNS_PRIM2D_P *)vtx, count);
			break;
		case	NNE_PRIM2D_FMT_PC:
			vtx		= _amDrawConvVertex2D((NNS_PRIM2D_PC *)vtx, count);
			break;
	}

	nnDrawPrimitivePoint2D(vtx, count, pri);
}
#endif


/************************************************************************/
/* void amDrawMakeTask(TaskProc proc, Uint16 prio, void *data)          */
/*----------------------------------------------------------------------*/
/* [INPUT] proc : 処理関数                                              */
/*         prio : 優先順位                                              */
/*         data : TCBワークの先頭に格納されるデータへのポインタ(8byte)  */
/* [FUNCTION] 描画タスク生成                                            */
/************************************************************************/
void amDrawMakeTask(TaskProc proc, Uint16 prio, void *data)
{
	amThreadCheckSafe(0, "amDrawTaskMake");

	// パラメータの生成
	AMS_PARAM_MAKE_TASK	*param;
	param	= (AMS_PARAM_MAKE_TASK *)
			amDrawMallocDataBuffer(sizeof(AMS_PARAM_MAKE_TASK));
	param->prio		= prio;
	param->proc		= proc;
	if (data != NULL)
		memcpy(param->work_data, data, 8);
	else
		memset(param->work_data, 0, 8);

	// コマンド登録
	amDrawRegistCommand(
			AMD_COMMAND_STATE_MAKE_TASK,
			AMD_COMMAND_MAKE_TASK,
			param);
}


/************************************************************************/
/* void amDrawMakeTask(TaskProc proc, Uint16 prio, Uint32 data)         */
/*----------------------------------------------------------------------*/
/* [INPUT] proc : 処理関数                                              */
/*         prio : 優先順位                                              */
/*         data : TCBワークの先頭に格納されるデータ(4byte)              */
/* [FUNCTION] 描画タスク生成                                            */
/************************************************************************/
void amDrawMakeTask(TaskProc proc, Uint16 prio, Uint32 data)
{
	amThreadCheckSafe(0, "amDrawTaskMake");

	// パラメータの生成
	AMS_PARAM_MAKE_TASK	*param;
	param	= (AMS_PARAM_MAKE_TASK *)
			amDrawMallocDataBuffer(sizeof(AMS_PARAM_MAKE_TASK));
	param->prio		= prio;
	param->proc		= proc;
	param->work_data[0]		= data;

	// コマンド登録
	amDrawRegistCommand(
			AMD_COMMAND_STATE_MAKE_TASK,
			AMD_COMMAND_MAKE_TASK,
			param);
}


/************************************************************************/
/* void amDrawPrintf(Sint32 pos_x, Sint32 pos_y, char *format, ...)     */
/*----------------------------------------------------------------------*/
/* [INPUT] pos_x, pos_y : 表示位置                                      */
/*         format : printf互換の書式                                    */
/* [FUNCTION] デバッグ文字の表示                                        */
/************************************************************************/
void amDrawPrintf(Sint32 pos_x, Sint32 pos_y, char *format, ...)
{
#if AMD_DEBUG
	char buf[256];

	va_list		args;
	va_start(args, format);
#if _PC
	vsprintf(buf, (const char *)format, args);
#else
	vsprintf(buf, (const char *)format, args);
#endif
	va_end(args);

	amDrawPrint(pos_x, pos_y, buf);
#endif
}


/************************************************************************/
/* void amDrawPrint(Sint32 pos_x, Sint32 pos_y, char *text)             */
/*----------------------------------------------------------------------*/
/* [INPUT] pos_x, pos_y : 表示位置                                      */
/*         text : 表示文字列                                            */
/* [FUNCTION] デバッグ文字の表示                                        */
/************************************************************************/
void amDrawPrint(Sint32 pos_x, Sint32 pos_y, char *text)
{
#if AMD_DEBUG
	if (amThreadCheckDraw())
		nnPrint(_am_draw_offset_x + pos_x, pos_y, text);
	else {
		// パラメータの生成
		AMS_PARAM_DEBUG_PRINT	*param;
		Sint32	size;
		size	= strlen(text) + 1;
		param	= (AMS_PARAM_DEBUG_PRINT *)amDrawMallocDataBuffer(
				sizeof(AMS_PARAM_DEBUG_PRINT) + size - 1);
		param->pos_x	= (Sint16)pos_x;
		param->pos_y	= (Sint16)pos_y;
		memcpy(param->text, text, size);

		// コマンド登録
		amDrawRegistCommand(
				AMD_COMMAND_STATE_DEBUG_PRINT,
				AMD_COMMAND_DEBUG_PRINT,
				param);
	}
#endif
}


/************************************************************************/
/* void amDrawPrintColor(Uint32 rgba)                                   */
/*----------------------------------------------------------------------*/
/* [INPUT] rgba : 表示文字色                                            */
/* [FUNCTION] デバッグ文字色の設定                                      */
/************************************************************************/
void amDrawPrintColor(Uint32 rgba)
{
	NNS_RGBA8888	color;

	color	= ((rgba & 0x000000ff) <<  (24 -  0))
			| ((rgba & 0x0000ff00) <<  (16 -  8))
			| ((rgba & 0x00ff0000) >> -( 8 - 16))
			| ((rgba & 0xff000000) >> -( 0 - 24));

	if (amThreadCheckDraw())
		nnSetPrintColor(color);
	else {
		// パラメータの生成
		AMS_PARAM_DEBUG_COLOR	*param;
		param	= (AMS_PARAM_DEBUG_COLOR *)amDrawMallocDataBuffer(
				sizeof(AMS_PARAM_DEBUG_COLOR));
		param->color	= color;

		// コマンド登録
		amDrawRegistCommand(
				AMD_COMMAND_STATE_DEBUG_PRINT,
				AMD_COMMAND_DEBUG_COLOR,
				param);
	}
}


/************************************************************************/
/* void amDrawObject(Uint32 state, NNS_OBJECT *object,                  */
/*                         NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag,  */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート                                      */
/*         object   : オブジェクト                                      */
/*         texlist  : テクスチャリスト                                  */
/*         drawflag : 描画フラグ                                        */
/*         func     : マテリアルコールバック関数                        */
/* [FUNCTION] オブジェクト描画                                          */
/************************************************************************/
void amDrawObject(Uint32 state, NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func)
{
	AMS_PARAM_DRAW_OBJECT	*param;
	NNS_MATRIX		*mtx;

	amThreadCheckSafe(0, "amDrawObject");

	param	= (AMS_PARAM_DRAW_OBJECT *)amDrawMallocDataBuffer(
					sizeof(AMS_PARAM_DRAW_OBJECT) + sizeof(NNS_MATRIX));

	mtx		= (NNS_MATRIX *)(param + 1);
	nnCopyMatrix(mtx, amMatrixGetCurrent());

	param->object	= object;
	param->mtx		= mtx;
	param->sub_obj_type	= 0;
	param->flag		= drawflag;
	param->texlist	= texlist;
	param->material_func	= func;
	param->scaleZ	= 1.0f;

	amDrawRegistCommand(state, AMD_COMMAND_DRAW_OBJECT, param);
}


/************************************************************************/
/* void amDrawObjectMaterialMotion(Uint32 state, NNS_MATMOTOBJ *object, */
/*                          NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート                                      */
/*         object   : マテリアルモーションオブジェクト                  */
/*         texlist  : テクスチャリスト                                  */
/*         drawflag : 描画フラグ                                        */
/*         func     : マテリアルコールバック関数                        */
/* [FUNCTION] オブジェクト描画                                          */
/************************************************************************/
void amDrawObjectMaterialMotion(Uint32 state, NNS_MATMOTOBJ *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func)
{
	AMS_PARAM_DRAW_OBJECT	*param;
	NNS_MATRIX		*mtx;

	amThreadCheckSafe(0, "amDrawObjectMaterialMotion");

	param	= (AMS_PARAM_DRAW_OBJECT *)amDrawMallocDataBuffer(
					sizeof(AMS_PARAM_DRAW_OBJECT) + sizeof(NNS_MATRIX));

	mtx		= (NNS_MATRIX *)(param + 1);
	nnCopyMatrix(mtx, amMatrixGetCurrent());

	param->object	= (NNS_OBJECT *)object;
	param->mtx		= mtx;
	param->sub_obj_type	= 0;
	param->flag		= drawflag;
	param->texlist	= texlist;
	param->material_func	= func;

	amDrawRegistCommand(state, AMD_COMMAND_DRAW_OBJECT_MATMTN, param);
}


/************************************************************************/
/* void amDrawObjectSetMaterial()                                       */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート                                      */
/*         object   : オブジェクト                                      */
/*         texlist  : テクスチャリスト                                  */
/*         scale    : スケール                                          */
/*         color    : マテリアルカラー                                  */
/*         u        : テクスチャＵ値                                    */
/*         v        : テクスチャＶ値                                    */
/*         blend    : αブレンド                                        */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] マテリアル情報のセット＋オブジェクト描画                  */
/************************************************************************/
void amDrawObjectSetMaterial(Uint32 state, NNS_OBJECT *object, NNS_TEXLIST *texlist,
		NNS_VECTOR* scale, NNS_RGBA color, float u, float v, Sint32 blend, NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func)
{
	AMS_PARAM_DRAW_OBJECT_MATERIAL	*param;
	NNS_MATRIX		*mtx;

	amThreadCheckSafe(0, "amDrawObjectMaterial");

	param	= (AMS_PARAM_DRAW_OBJECT_MATERIAL *)amDrawMallocDataBuffer(
					sizeof(AMS_PARAM_DRAW_OBJECT_MATERIAL) + sizeof(NNS_MATRIX));

	mtx		= (NNS_MATRIX *)(param + 1);
	nnCopyMatrix(mtx, amMatrixGetCurrent());

	param->object	= object;
	param->mtx		= mtx;
	param->sub_obj_type	= 0;
	param->flag		= drawflag;
	param->texlist	= texlist;
	param->scaleZ	= -scale->z;
	nnCopyVector(&param->scale, scale);
	param->color	= color;
	param->scroll_u	= u;
	param->scroll_v = v;
	param->blend	= blend;
	param->material_func = func;

	amDrawRegistCommand(state, AMD_COMMAND_DRAW_OBJECT_MATERIAL, param);
}


/************************************************************************/
/* void amDrawMotion(Uint32 state, NNS_MOTION *motion, float frame,     */
/*     NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag,  */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート                                      */
/*         motion   : モーション                                        */
/*         frame    : モーションフレーム                                */
/*         object   : オブジェクト                                      */
/*         texlist  : テクスチャリスト                                  */
/*         drawflag : 描画フラグ                                        */
/*         func     : マテリアルコールバック関数                        */
/* [FUNCTION] モーション描画                                            */
/************************************************************************/
void amDrawMotion(Uint32 state, NNS_MOTION *motion, float frame, NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func)
{
	AMS_PARAM_DRAW_MOTION	*param;
	NNS_MATRIX		*mtx;

	amThreadCheckSafe(0, "amDrawMotion");

	param	= (AMS_PARAM_DRAW_MOTION *)amDrawMallocDataBuffer(
					sizeof(AMS_PARAM_DRAW_MOTION) + sizeof(NNS_MATRIX));

	mtx		= (NNS_MATRIX *)(param + 1);
	nnCopyMatrix(mtx, amMatrixGetCurrent());

	param->object	= object;
	param->mtx		= mtx;
	param->sub_obj_type	= 0;
	param->flag		= drawflag;
	param->texlist	= texlist;
	param->motion	= motion;
	param->frame	= frame;
	param->material_func	= func;

	amDrawRegistCommand(state, AMD_COMMAND_DRAW_MOTION, param);
}


/************************************************************************/
/* void amDrawMotionMaterialMotion(Uint32 state, NNS_MOTION *motion,    */
/*                         float frame, NNS_MATMOTOBJ *object,          */
/*                         NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag,  */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート                                      */
/*         motion   : モーション                                        */
/*         frame    : モーションフレーム                                */
/*         object   : マテリアルモーションオブジェクト                  */
/*         texlist  : テクスチャリスト                                  */
/*         drawflag : 描画フラグ                                        */
/*         func     : マテリアルコールバック関数                        */
/* [FUNCTION] モーション描画                                            */
/************************************************************************/
void amDrawMotionMaterialMotion(Uint32 state, NNS_MOTION *motion, float frame, NNS_MATMOTOBJ *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func)
{
	AMS_PARAM_DRAW_MOTION	*param;
	NNS_MATRIX		*mtx;

	amThreadCheckSafe(0, "amDrawMotionMaterialMotion");

	param	= (AMS_PARAM_DRAW_MOTION *)amDrawMallocDataBuffer(
					sizeof(AMS_PARAM_DRAW_MOTION) + sizeof(NNS_MATRIX));

	mtx		= (NNS_MATRIX *)(param + 1);
	nnCopyMatrix(mtx, amMatrixGetCurrent());

	param->object	= (NNS_OBJECT *)object;
	param->mtx		= mtx;
	param->sub_obj_type	= 0;
	param->flag		= drawflag;
	param->texlist	= texlist;
	param->motion	= motion;
	param->frame	= frame;
	param->material_func	= func;

	amDrawRegistCommand(state, AMD_COMMAND_DRAW_MOTION_MATMTN, param);
}


/************************************************************************/
/* void amDrawPrimitive3D()                                             */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート                                      */
/*         param    : プリミティブ描画設定                              */
/* [FUNCTION] プリミティブ描画（3D）                                    */
/************************************************************************/
void amDrawPrimitive3D(Uint32 state, AMS_PARAM_DRAW_PRIMITIVE* setParam)
{
	AMS_PARAM_DRAW_PRIMITIVE	*param;
	NNS_MATRIX		*mtx;
	char            *buf;

	amThreadCheckSafe(0, "amDrawPrimitive3D");

	param	= (AMS_PARAM_DRAW_PRIMITIVE *)amDrawMallocDataBuffer(
					sizeof(AMS_PARAM_DRAW_PRIMITIVE) + sizeof(NNS_MATRIX));
	buf     = (char*)param;

	mtx		= (NNS_MATRIX *)(buf + sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	nnCopyMatrix(mtx, amMatrixGetCurrent());

	memcpy(param, setParam, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	param->mtx = mtx;

	amDrawRegistCommand(state, AMD_COMMAND_DRAW_PRIMITIVE_3D, param);
}


/************************************************************************/
/* void amDrawPrim2D()                                                  */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート                                      */
/*         param    : プリミティブ描画設定                              */
/* [FUNCTION] プリミティブ描画（2D）                                    */
/************************************************************************/
void amDrawPrim2D(Uint32 state, AMS_PARAM_DRAW_PRIMITIVE* setParam)
{
	AMS_PARAM_DRAW_PRIMITIVE	*param;
	NNS_MATRIX		*mtx;
	char            *buf;

	amThreadCheckSafe(0, "amDrawPrim2D");

	param	= (AMS_PARAM_DRAW_PRIMITIVE *)amDrawMallocDataBuffer(
					sizeof(AMS_PARAM_DRAW_PRIMITIVE) + sizeof(NNS_MATRIX));
	buf     = (char*)param;

	mtx		= (NNS_MATRIX *)(buf + sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	nnCopyMatrix(mtx, amMatrixGetCurrent());

	memcpy(param, setParam, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	param->mtx = mtx;

	amDrawRegistCommand(state, AMD_COMMAND_DRAW_PRIMITIVE_2D, param);
}

/************************************************************************/
/* void amDrawGetPrimBlendParam()                                       */
/*----------------------------------------------------------------------*/
/* [INPUT] type     : αブレンドタイプ                                  */
/*         param    : プリミティブ描画設定                              */
/* [FUNCTION] プリミティブ描画用αブレンド設定値を取得                  */
/************************************************************************/
void amDrawGetPrimBlendParam(AMDRAWE_BLENDTYPE type, AMS_PARAM_DRAW_PRIMITIVE* setParam)
{
	switch ( type )
	{
	case AMDRAWE_BLENDTYPE_NORMAL: // 乗算
#if _PC | _XBOX
		setParam->bldSrc = NNE_BLENDMODE_SRCALPHA;
		setParam->bldDst = NNE_BLENDMODE_INVSRCALPHA;
		setParam->bldMode = NNE_BLENDOP_ADD;
#elif _PS3
		setParam->bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
		setParam->bldDst = NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA;
		setParam->bldMode = NND_BLENDOP_PS3_FUNC_ADD;
#elif _WII
		setParam->bldSrc = GX_BL_SRCALPHA;
		setParam->bldDst = GX_BL_INVSRCALPHA;
		setParam->bldMode = GX_BM_BLEND;
#elif _IPHONE
		setParam->bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
		setParam->bldDst = NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA;
		setParam->bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif
		break;

	case AMDRAWE_BLENDTYPE_ADD: // 加算
#if _PC | _XBOX
		setParam->bldSrc = NNE_BLENDMODE_SRCALPHA;
		setParam->bldDst = NNE_BLENDMODE_ONE;
		setParam->bldMode = NNE_BLENDOP_ADD;
#elif _PS3
		setParam->bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
		setParam->bldDst = NND_BLENDFUNC_PS3_ONE;
		setParam->bldMode = NND_BLENDOP_PS3_FUNC_ADD;
#elif _WII
		setParam->bldSrc = GX_BL_SRCALPHA;
		setParam->bldDst = GX_BL_ONE;
		setParam->bldMode = GX_BM_BLEND;
#elif _IPHONE
		setParam->bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
		setParam->bldDst = NND_BLENDFUNC_GL_ONE;
		setParam->bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif
		break;

	case AMDRAWE_BLENDTYPE_SUB: // 減算
#if _PC | _XBOX
		setParam->bldSrc = NNE_BLENDMODE_SRCALPHA;
		setParam->bldDst = NNE_BLENDMODE_ONE;
		setParam->bldMode = NNE_BLENDOP_REVSUB;
#elif _PS3
		setParam->bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
		setParam->bldDst = NND_BLENDFUNC_PS3_ONE;
		setParam->bldMode = NND_BLENDOP_PS3_FUNC_REVERSE_SUBTRACT;
#elif _WII
		setParam->bldSrc = GX_BL_SRCALPHA;
		setParam->bldDst = GX_BL_ONE;
		setParam->bldMode = GX_BM_SUBTRACT;
#elif _IPHONE
		setParam->bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
		setParam->bldDst = NND_BLENDFUNC_GL_ONE;
		setParam->bldMode = NND_BLENDOP_GL_FUNC_REVERSE_SUBTRACT;
#endif
		break;
	default:
		break;
	}
}


/************************************************************************/
/* void amDrawSetMaterialDiffuse(Uint32 state,                          */
/*                     NNE_MATCTRLMODE mode, float r, float g, float b) */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート                                       */
/*         mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         r, g, b : ディフューズカラー                                 */
/* [FUNCTION] ディフューズカラーの設定                                  */
/************************************************************************/
void amDrawSetMaterialDiffuse(Uint32 state, NNE_MATCTRLMODE mode, float r, float g, float b)
{
	AMS_DRAWSTATE_DIFFUSE	*param;

	amThreadCheckSafe(0, "amDrawSetMaterialDiffuse");

	param	= (AMS_DRAWSTATE_DIFFUSE *)amDrawMallocDataBuffer(
					sizeof(AMS_DRAWSTATE_DIFFUSE));

	param->mode		= mode;
	param->r		= r;
	param->g		= g;
	param->b		= b;

	amDrawRegistCommand(state, AMD_COMMAND_SET_DIFFUSE, param);
}


/************************************************************************/
/* void amDrawSetMaterialAmbient(Uint32 state,                          */
/*                     NNE_MATCTRLMODE mode, float r, float g, float b) */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート                                       */
/*         mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         r, g, b : アンビエントカラー                                 */
/* [FUNCTION] アンビエントカラーの設定                                  */
/************************************************************************/
void amDrawSetMaterialAmbient(Uint32 state, NNE_MATCTRLMODE mode, float r, float g, float b)
{
	AMS_DRAWSTATE_AMBIENT	*param;

	amThreadCheckSafe(0, "amDrawSetMaterialAmbient");

	param	= (AMS_DRAWSTATE_AMBIENT *)amDrawMallocDataBuffer(
					sizeof(AMS_DRAWSTATE_AMBIENT));

	param->mode		= mode;
	param->r		= r;
	param->g		= g;
	param->b		= b;

	amDrawRegistCommand(state, AMD_COMMAND_SET_AMBIENT, param);
}


/************************************************************************/
/* void amDrawSetMaterialAlpha(Uint32 state,                            */
/*                                   NNE_MATCTRLMODE mode, float alpha) */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート                                       */
/*         mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         alpha   : α値                                               */
/* [FUNCTION] α値の設定                                                */
/************************************************************************/
void amDrawSetMaterialAlpha(Uint32 state, NNE_MATCTRLMODE mode, float alpha)
{
	AMS_DRAWSTATE_ALPHA		*param;

	amThreadCheckSafe(0, "amDrawSetMaterialAlpha");

	param	= (AMS_DRAWSTATE_ALPHA *)amDrawMallocDataBuffer(
					sizeof(AMS_DRAWSTATE_ALPHA));

	param->mode		= mode;
	param->alpha	= alpha;

	amDrawRegistCommand(state, AMD_COMMAND_SET_ALPHA, param);
}


/************************************************************************/
/* void amDrawSetMaterialSpecular(Uint32 state,                         */
/*                     NNE_MATCTRLMODE mode, float r, float g, float b) */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート                                       */
/*         mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         r, g, b : スペキュラーカラー                                 */
/* [FUNCTION] アンビエントカラーの設定                                  */
/************************************************************************/
void amDrawSetMaterialSpecular(Uint32 state, NNE_MATCTRLMODE mode, float r, float g, float b)
{
	AMS_DRAWSTATE_SPECULAR	*param;

	amThreadCheckSafe(0, "amDrawSetMaterialSpecular");

	param	= (AMS_DRAWSTATE_SPECULAR *)amDrawMallocDataBuffer(
					sizeof(AMS_DRAWSTATE_SPECULAR));

	param->mode		= mode;
	param->r		= r;
	param->g		= g;
	param->b		= b;

	amDrawRegistCommand(state, AMD_COMMAND_SET_SPECULAR, param);
}


/************************************************************************/
/* void amDrawSetMaterialEnvMap(Uint32 state,                           */
/*                  NNE_MATCTRL_TEXCOORDSRC texsrc, NNS_MATRIX *texmtx) */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート                                       */
/*         texsrc  : テクスチャ座標ソース(NNE_MATCTRLMODE_OFFでOFF)     */
/*         texmtx  : テクスチャ座標変換マトリクス                       */
/* [FUNCTION] テクスチャ座標変換マトリクスの設定                        */
/************************************************************************/
void amDrawSetMaterialEnvMap(Uint32 state, NNE_MATCTRL_TEXCOORDSRC texsrc, NNS_MATRIX *texmtx)
{
	AMS_DRAWSTATE_ENVMAP	*param;

	amThreadCheckSafe(0, "amDrawSetMaterialEnvMap");

	param	= (AMS_DRAWSTATE_ENVMAP *)amDrawMallocDataBuffer(
					sizeof(AMS_DRAWSTATE_ENVMAP));

	param->texsrc	= texsrc;
	nnCopyMatrix(&param->texmtx, texmtx);

	amDrawRegistCommand(state, AMD_COMMAND_SET_ENVMAP, param);
}


/************************************************************************/
/* void amDrawSetMaterialBlendMode(Uint32 state, NNE_MATCTRL_BLEND mode)*/
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート                                       */
/*         mode    : フレームバッファブレンドモード                     */
/*                     (NNE_MATCTRLMODE_OFFでOFF)                       */
/* [FUNCTION] フレームバッファブレンドモードの設定                      */
/************************************************************************/
void amDrawSetMaterialBlendMode(Uint32 state, NNE_MATCTRL_BLEND mode)
{
	AMS_DRAWSTATE_BLEND		*param;

	amThreadCheckSafe(0, "amDrawSetMaterialBlendMode");

	param	= (AMS_DRAWSTATE_BLEND *)amDrawMallocDataBuffer(
					sizeof(AMS_DRAWSTATE_BLEND));

	param->mode		= mode;

	amDrawRegistCommand(state, AMD_COMMAND_SET_BLEND, param);
}


/************************************************************************/
/* void amDrawSetMaterialTexOffset(Uint32 state,                        */
/*            NNE_TEXSLOT slot, NNE_MATCTRLMODE mode, float u, float v) */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート                                       */
/*         slot    : テクスチャスロット番号(-1ならばすべて)             */
/*         mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         u, v    : テクスチャ座標オフセット                           */
/* [FUNCTION] テクスチャ座標オフセットの設定                            */
/************************************************************************/
void amDrawSetMaterialTexOffset(Uint32 state, NNE_TEXSLOT slot, NNE_MATCTRLMODE mode, float u, float v)
{
	AMS_PARAM_SET_TEXOFFSET	*param;

	amThreadCheckSafe(0, "amDrawSetMaterialTexOffset");

	param	= (AMS_PARAM_SET_TEXOFFSET *)amDrawMallocDataBuffer(
					sizeof(AMS_PARAM_SET_TEXOFFSET));

	param->slot		= slot;
	param->texoffset.mode	= mode;
	param->texoffset.u		= u;
	param->texoffset.v		= v;

	amDrawRegistCommand(state, AMD_COMMAND_SET_TEXOFFSET, param);
}


/************************************************************************/
/* void amDrawSetFog(Uint32 state, Sint32 flag)                         */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート                                       */
/*         flag    : フォグのON/OFF                                     */
/* [FUNCTION] フォグの設定                                              */
/************************************************************************/
void amDrawSetFog(Uint32 state, Sint32 flag)
{
	AMS_DRAWSTATE_FOG		*param;

	amThreadCheckSafe(0, "amDrawSetFog");

	param	= (AMS_DRAWSTATE_FOG *)amDrawMallocDataBuffer(
					sizeof(AMS_DRAWSTATE_FOG));

	param->flag		= flag & 1;

	amDrawRegistCommand(state, AMD_COMMAND_SET_FOG, param);
}


/************************************************************************/
/* void amDrawSetFogColor(Uint32 state, float r, float g, float b)      */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート                                       */
/*         r, g, b : フォグのカラー                                     */
/* [FUNCTION] フォグカラーの設定                                        */
/************************************************************************/
void amDrawSetFogColor(Uint32 state, float r, float g, float b)
{
	AMS_DRAWSTATE_FOG_COLOR		*param;

	amThreadCheckSafe(0, "amDrawSetFogColor");

	param	= (AMS_DRAWSTATE_FOG_COLOR *)amDrawMallocDataBuffer(
					sizeof(AMS_DRAWSTATE_FOG_COLOR));

	param->r		= r;
	param->g		= g;
	param->b		= b;

	amDrawRegistCommand(state, AMD_COMMAND_SET_FOG_COLOR, param);
}


/************************************************************************/
/* void amDrawSetFogRange(Uint32 state, float fnear, float ffar)        */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート                                       */
/*         fnear   : フォグの開始範囲                                   */
/*         ffar    : フォグの終了範囲                                   */
/* [FUNCTION] フォグレンジの設定                                        */
/************************************************************************/
void amDrawSetFogRange(Uint32 state, float fnear, float ffar)
{
	AMS_DRAWSTATE_FOG_RANGE		*param;

	amThreadCheckSafe(0, "amDrawSetFogRange");

	param	= (AMS_DRAWSTATE_FOG_RANGE *)amDrawMallocDataBuffer(
					sizeof(AMS_DRAWSTATE_FOG_RANGE));

	param->fnear	= fnear;
	param->ffar		= ffar;

	amDrawRegistCommand(state, AMD_COMMAND_SET_FOG_RANGE, param);
}


/************************************************************************/
/* void amDrawSetZMode(Uint32 state,                                    */
/*                      NNE_BOOL compare, Sint32 func, NNE_BOOL update) */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート                                       */
/*         compare : Z比較をするかどうか                                */
/*         func    : Z比較関数                                          */
/*         update  : Zバッファを更新するかどうか                        */
/* [FUNCTION] Zバッファ比較・更新モードの設定                           */
/************************************************************************/
void amDrawSetZMode(Uint32 state, NNE_BOOL compare, Sint32 func, NNE_BOOL update)
{
	AMS_DRAWSTATE_Z_MODE		*param;

	amThreadCheckSafe(0, "amDrawSetZMode");

	return;

	param	= (AMS_DRAWSTATE_Z_MODE *)amDrawMallocDataBuffer(
					sizeof(AMS_DRAWSTATE_Z_MODE));

	param->compare	= (Uint16)compare;
	param->update	= (Uint16)update;
	param->func		= func;

	amDrawRegistCommand(state, AMD_COMMAND_SET_Z_MODE, param);
}


/************************************************************************/
/* void amDrawObject(NNS_OBJECT *object, NNS_TEXLIST *texlist,          */
/*               NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func)  */
/*----------------------------------------------------------------------*/
/* [INPUT] object   : オブジェクト                                      */
/*         texlist  : テクスチャリスト                                  */
/*         drawflag : 描画フラグ                                        */
/*         func     : マテリアルコールバック関数                        */
/* [FUNCTION] オブジェクト描画                                          */
/************************************************************************/
void amDrawObject(NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func)
{
	AMS_COMMAND_HEADER		*command;
	AMS_PARAM_DRAW_OBJECT	*param;

	amThreadCheckSafe(1, "amDrawObject");

	command		= (AMS_COMMAND_HEADER *)amDrawMallocWorkBuffer(
			sizeof(AMS_COMMAND_HEADER) + sizeof(AMS_PARAM_DRAW_OBJECT));
	param		= (AMS_PARAM_DRAW_OBJECT *)(command + 1);

	command->command_id	= AMD_COMMAND_DRAW_OBJECT;
	command->param		= param;

	param->object		= object;
	param->texlist		= texlist;
	param->mtx			= NULL;
	param->sub_obj_type	= 0;
	param->flag			= drawflag;
	param->material_func	= func;
	param->scaleZ		= 1.0f;

	_amDrawObject(command, drawflag);
}


/************************************************************************/
/* void amDrawObjectMaterialMotion(NNS_MATMOTOBJ *object,               */
/*                         NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag,  */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/*----------------------------------------------------------------------*/
/* [INPUT] object   : マテリアルモーションオブジェクト                  */
/*         texlist  : テクスチャリスト                                  */
/*         drawflag : 描画フラグ                                        */
/*         func     : マテリアルコールバック関数                        */
/* [FUNCTION] オブジェクト描画                                          */
/************************************************************************/
void amDrawObjectMaterialMotion(NNS_MATMOTOBJ *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func)
{
	AMS_COMMAND_HEADER		*command;
	AMS_PARAM_DRAW_OBJECT	*param;

	amThreadCheckSafe(1, "amDrawObjectMaterialMotion");

	command		= (AMS_COMMAND_HEADER *)amDrawMallocWorkBuffer(
			sizeof(AMS_COMMAND_HEADER) + sizeof(AMS_PARAM_DRAW_OBJECT));
	param		= (AMS_PARAM_DRAW_OBJECT *)(command + 1);

	command->command_id	= AMD_COMMAND_DRAW_OBJECT_MATMTN;
	command->param		= param;

	param->object		= (NNS_OBJECT *)object;
	param->texlist		= texlist;
	param->mtx			= NULL;
	param->sub_obj_type	= 0;
	param->flag			= drawflag;
	param->material_func	= func;
	param->scaleZ		= 1.0f;

	_amDrawObject(command, drawflag);
}


/************************************************************************/
/* void amDrawMotion(NNS_MOTION *motion, float frame,                   */
/*     NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag,  */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/*----------------------------------------------------------------------*/
/* [INPUT] motion   : モーション                                        */
/*         frame    : モーションフレーム                                */
/*         object   : オブジェクト                                      */
/*         texlist  : テクスチャリスト                                  */
/*         drawflag : 描画フラグ                                        */
/*         func     : マテリアルコールバック関数                        */
/* [FUNCTION] モーション描画                                            */
/************************************************************************/
void amDrawMotion(NNS_MOTION *motion, float frame, NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func)
{
	AMS_COMMAND_HEADER		*command;
	AMS_PARAM_DRAW_MOTION	*param;

	amThreadCheckSafe(1, "amDrawMotion");

	command		= (AMS_COMMAND_HEADER *)amDrawMallocWorkBuffer(
			sizeof(AMS_COMMAND_HEADER) + sizeof(AMS_PARAM_DRAW_MOTION));
	param		= (AMS_PARAM_DRAW_MOTION *)(command + 1);

	command->command_id	= AMD_COMMAND_DRAW_MOTION;
	command->param		= param;

	param->object		= object;
	param->mtx			= NULL;
	param->sub_obj_type	= 0;
	param->flag			= drawflag;
	param->texlist		= texlist;
	param->motion		= motion;
	param->frame		= frame;
	param->material_func	= func;

	_amDrawMotion(command, drawflag);
}


/************************************************************************/
/* void amDrawMotionMaterialMotion(NNS_MOTION *motion,                  */
/*                         float frame, NNS_MATMOTOBJ *object,          */
/*                         NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag,  */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/*----------------------------------------------------------------------*/
/* [INPUT] motion   : モーション                                        */
/*         frame    : モーションフレーム                                */
/*         object   : マテリアルモーションオブジェクト                  */
/*         texlist  : テクスチャリスト                                  */
/*         drawflag : 描画フラグ                                        */
/*         func     : マテリアルコールバック関数                        */
/* [FUNCTION] モーション描画                                            */
/************************************************************************/
void amDrawMotionMaterialMotion(NNS_MOTION *motion, float frame, NNS_MATMOTOBJ *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func)
{
	AMS_COMMAND_HEADER		*command;
	AMS_PARAM_DRAW_MOTION	*param;

	amThreadCheckSafe(1, "amDrawMotionMaterialMotion");

	command		= (AMS_COMMAND_HEADER *)amDrawMallocWorkBuffer(
			sizeof(AMS_COMMAND_HEADER) + sizeof(AMS_PARAM_DRAW_MOTION));
	param		= (AMS_PARAM_DRAW_MOTION *)(command + 1);

	command->command_id	= AMD_COMMAND_DRAW_MOTION_MATMTN;
	command->param		= param;

	param->object		= (NNS_OBJECT *)object;
	param->mtx			= NULL;
	param->sub_obj_type	= 0;
	param->flag			= drawflag;
	param->texlist		= texlist;
	param->motion		= motion;
	param->frame		= frame;
	param->material_func	= func;

	_amDrawMotion(command, drawflag);
}


/************************************************************************/
/* void amDrawSetMaterialDiffuse(NNE_MATCTRLMODE mode,                  */
/*                                           float r, float g, float b) */
/*----------------------------------------------------------------------*/
/* [INPUT] mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         r, g, b : ディフューズカラー                                 */
/* [FUNCTION] ディフューズカラーの設定                                  */
/************************************************************************/
void amDrawSetMaterialDiffuse(NNE_MATCTRLMODE mode, float r, float g, float b)
{
	amThreadCheckSafe(1, "amDrawSetMaterialDiffuse");

	if (mode != NNE_MATCTRLMODE_OFF) {
		nnSetMaterialControlDiffuse(mode, r, g, b);
		_am_draw_state.drawflag		|= NND_DRAWOBJ_MATCTRL_DIFFUSE;
	} else
		_am_draw_state.drawflag		&= ~NND_DRAWOBJ_MATCTRL_DIFFUSE;

	_am_draw_state.diffuse.mode		= mode;
	_am_draw_state.diffuse.r		= r;
	_am_draw_state.diffuse.g		= g;
	_am_draw_state.diffuse.b		= b;
}


/************************************************************************/
/* void amDrawSetMaterialAmbient(NNE_MATCTRLMODE mode,                  */
/*                                           float r, float g, float b) */
/*----------------------------------------------------------------------*/
/* [INPUT] mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         r, g, b : アンビエントカラー                                 */
/* [FUNCTION] アンビエントカラーの設定                                  */
/************************************************************************/
void amDrawSetMaterialAmbient(NNE_MATCTRLMODE mode, float r, float g, float b)
{
	amThreadCheckSafe(1, "amDrawSetMaterialAmbient");

	if (mode != NNE_MATCTRLMODE_OFF) {
		nnSetMaterialControlAmbient(mode, r, g, b);
		_am_draw_state.drawflag		|= NND_DRAWOBJ_MATCTRL_AMBIENT;
	} else
		_am_draw_state.drawflag		&= ~NND_DRAWOBJ_MATCTRL_AMBIENT;

	_am_draw_state.ambient.mode		= mode;
	_am_draw_state.ambient.r		= r;
	_am_draw_state.ambient.g		= g;
	_am_draw_state.ambient.b		= b;
}


/************************************************************************/
/* void amDrawSetMaterialAlpha(NNE_MATCTRLMODE mode, float alpha)       */
/*----------------------------------------------------------------------*/
/* [INPUT] mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         alpha   : α値                                               */
/* [FUNCTION] α値の設定                                                */
/************************************************************************/
void amDrawSetMaterialAlpha(NNE_MATCTRLMODE mode, float alpha)
{
	amThreadCheckSafe(1, "amDrawSetMaterialAlpha");

	if (mode != NNE_MATCTRLMODE_OFF) {
		nnSetMaterialControlAlpha(mode, alpha);
		_am_draw_state.drawflag		|= NND_DRAWOBJ_MATCTRL_ALPHA;
	} else
		_am_draw_state.drawflag		&= ~NND_DRAWOBJ_MATCTRL_ALPHA;
		
	_am_draw_state.alpha.mode		= mode;
	_am_draw_state.alpha.alpha		= alpha;
}


/************************************************************************/
/* void amDrawSetMaterialSpecular(NNE_MATCTRLMODE mode,                 */
/*                                           float r, float g, float b) */
/*----------------------------------------------------------------------*/
/* [INPUT] mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         r, g, b : スペキュラーカラー                                 */
/* [FUNCTION] アンビエントカラーの設定                                  */
/************************************************************************/
void amDrawSetMaterialSpecular(NNE_MATCTRLMODE mode, float r, float g, float b)
{
	amThreadCheckSafe(1, "amDrawSetMaterialSpecular");

	if (mode != NNE_MATCTRLMODE_OFF) {
#if _PC | _XBOX
		nnSetMaterialControlSpecularDXG20(mode, r, g, b);
#elif _PS3
		nnSetMaterialControlSpecularPS3(mode, r, g, b);
#elif _WII
		nnSetMaterialControlSpecularGC(mode, r, g, b);
#endif
		_am_draw_state.drawflag		|= NND_DRAWOBJ_MATCTRL_SPECULAR;
	} else
		_am_draw_state.drawflag		&= ~NND_DRAWOBJ_MATCTRL_SPECULAR;

	_am_draw_state.specular.mode	= mode;
	_am_draw_state.specular.r		= r;
	_am_draw_state.specular.g		= g;
	_am_draw_state.specular.b		= b;
}


/************************************************************************/
/* void amDrawSetMaterialEnvMap(NNE_MATCTRL_TEXCOORDSRC texsrc,         */
/*                                                  NNS_MATRIX *texmtx) */
/*----------------------------------------------------------------------*/
/* [INPUT] texsrc  : テクスチャ座標ソース                               */
/*                   (NNE_MATCTRL_TEXCOORDSRC_OFFでOFF)                 */
/*         texmtx  : テクスチャ座標変換マトリクス                       */
/* [FUNCTION] テクスチャ座標変換マトリクスの設定                        */
/************************************************************************/
void amDrawSetMaterialEnvMap(NNE_MATCTRL_TEXCOORDSRC texsrc, NNS_MATRIX *texmtx)
{
	amThreadCheckSafe(1, "amDrawSetMaterialEnvMap");

	if (texsrc != NNE_MATCTRL_TEXCOORDSRC_OFF) {
		nnSetMaterialControlEnvTexMatrix(texsrc, texmtx);
		_am_draw_state.drawflag		|= NND_DRAWOBJ_MATCTRL_ENVTEXMTX;
	} else
		_am_draw_state.drawflag		&= ~NND_DRAWOBJ_MATCTRL_ENVTEXMTX;

	_am_draw_state.envmap.texsrc	= texsrc;
	nnCopyMatrix(&_am_draw_state.envmap.texmtx, texmtx);
}


/************************************************************************/
/* void amDrawSetMaterialBlendMode(NNE_MATCTRL_BLEND mode)              */
/*----------------------------------------------------------------------*/
/* [INPUT] mode    : フレームバッファブレンドモード                     */
/*                     (NNE_MATCTRL_BLEND_OFFでOFF)                     */
/* [FUNCTION] フレームバッファブレンドモードの設定                      */
/************************************************************************/
void amDrawSetMaterialBlendMode(NNE_MATCTRL_BLEND mode)
{
	amThreadCheckSafe(1, "amDrawSetMaterialBlendMode");

	if (mode != NNE_MATCTRL_BLEND_OFF) {
		nnSetMaterialControlBlendMode(mode);
		_am_draw_state.drawflag		|= NND_DRAWOBJ_MATCTRL_BLEND;
	} else
		_am_draw_state.drawflag		&= ~NND_DRAWOBJ_MATCTRL_BLEND;

	_am_draw_state.blend.mode	= mode;
}


/************************************************************************/
/* void amDrawSetMaterialTexOffset(NNE_TEXSLOT slot,                    */
/*                              NNE_MATCTRLMODE mode, float u, float v) */
/*----------------------------------------------------------------------*/
/* [INPUT] slot    : テクスチャスロット番号(-1ならばすべて)             */
/*         mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         u, v    : テクスチャ座標オフセット                           */
/* [FUNCTION] テクスチャ座標オフセットの設定                            */
/************************************************************************/
void amDrawSetMaterialTexOffset(NNE_TEXSLOT slot, NNE_MATCTRLMODE mode, float u, float v)
{
	AMS_DRAWSTATE_TEXOFFSET	*texoffset;
	Sint32		i;

	amThreadCheckSafe(1, "amDrawSetMaterialTexOffset");

	if (slot != -1) {
		if (mode != NNE_MATCTRLMODE_OFF) {
			nnSetMaterialControlTextureOffset((NNE_TEXSLOT)slot, mode, u, v);
			_am_draw_state.drawflag		|= NND_DRAWOBJ_MATCTRL_TEXOFFSET;
		} else
			_am_draw_state.drawflag		&= ~NND_DRAWOBJ_MATCTRL_TEXOFFSET;
		texoffset	= &_am_draw_state.texoffset[slot];
		texoffset->mode		= mode;
		texoffset->u		= u;
		texoffset->v		= v;
	} else {
		texoffset	= &_am_draw_state.texoffset[0];
		if (mode != NNE_MATCTRLMODE_OFF) {
			for (i = 0; i < 4; i++, texoffset++) {
				nnSetMaterialControlTextureOffset(
						(NNE_TEXSLOT)i, mode, u, v);
				_am_draw_state.drawflag		|= NND_DRAWOBJ_MATCTRL_TEXOFFSET;
				texoffset->mode		= mode;
				texoffset->u		= u;
				texoffset->v		= v;
			}
		} else {
			for (i = 0; i < 4; i++, texoffset++) {
				_am_draw_state.drawflag		&= ~NND_DRAWOBJ_MATCTRL_TEXOFFSET;
				texoffset->mode		= mode;
				texoffset->u		= u;
				texoffset->v		= v;
			}
		}
	}
}


/************************************************************************/
/* void amDrawSetFog(Sint32 flag)                                       */
/*----------------------------------------------------------------------*/
/* [INPUT] flag    : フォグのON/OFF                                     */
/* [FUNCTION] フォグの設定                                              */
/************************************************************************/
void amDrawSetFog(Sint32 flag)
{
	amThreadCheckSafe(1, "amDrawSetFog");

	flag	&= 1;

	nnSetFogSwitch((NNE_BOOL)flag);

	_am_draw_state.fog.flag		= flag;
}


/************************************************************************/
/* void amDrawSetFogColor(float r, float g, float b)                    */
/*----------------------------------------------------------------------*/
/* [INPUT] r, g, b : フォグのカラー                                     */
/* [FUNCTION] フォグカラーの設定                                        */
/************************************************************************/
void amDrawSetFogColor(float r, float g, float b)
{
	amThreadCheckSafe(1, "amDrawSetFogColor");

	nnSetFogColor(r, g, b);

	_am_draw_state.fog_color.r	= r;
	_am_draw_state.fog_color.g	= g;
	_am_draw_state.fog_color.b	= b;
}


/************************************************************************/
/* void amDrawSetFogRange(float fnear, float ffar)                      */
/*----------------------------------------------------------------------*/
/* [INPUT] fnear   : フォグの開始範囲                                   */
/*         ffar    : フォグの終了範囲                                   */
/* [FUNCTION] フォグレンジの設定                                        */
/************************************************************************/
void amDrawSetFogRange(float fnear, float ffar)
{
	amThreadCheckSafe(1, "amDrawSetFogRange");

#if !_WII
	nnSetFogRange(fnear, ffar);
#else
	nnSetFogRangeGC((amDrawGetProjectionType() == NNE_PROJECTION_TYPE_ORTHO)?
			GX_FOG_ORTHO_LIN : GX_FOG_PERSP_LIN, fnear, ffar);
#endif

	_am_draw_state.fog_range.fnear	= fnear;
	_am_draw_state.fog_range.ffar	= ffar;
}


/************************************************************************/
/* void amDrawSetZMode(NNE_BOOL compare, Sint32 func, NNE_BOOL update)  */
/*----------------------------------------------------------------------*/
/* [INPUT] compare : Z比較をするかどうか                                */
/*         func    : Z比較関数                                          */
/*         update  : Zバッファを更新するかどうか                        */
/* [FUNCTION] Zバッファ比較・更新モードの設定                           */
/************************************************************************/
void amDrawSetZMode(NNE_BOOL compare, Sint32 func, NNE_BOOL update)
{
	amThreadCheckSafe(1, "amDrawSetZMode");

#if _PC | _XBOX
	nnSetPrimitive2DDepthTestDXG20((NNE_BOOL)compare);
	nnSetPrimitive2DDepthFuncDXG20((NNE_CMPFUNC)func);
	nnSetPrimitive2DDepthMaskDXG20((NNE_BOOL)update);

	nnSetPrimitive3DDepthTestDXG20((NNE_BOOL)compare);
	nnSetPrimitive3DDepthFuncDXG20((NNE_CMPFUNC)func);
	nnSetPrimitive3DDepthMaskDXG20((NNE_BOOL)update);

	nnSetZModeDXG20(compare, (NNE_CMPFUNC)func, update);
#elif _PS3
	nnSetPrimitive2DDepthTestPS3((NNE_BOOL)compare);
	nnSetPrimitive2DDepthFuncPS3(func);
	nnSetPrimitive2DDepthMaskPS3((NNE_BOOL)update);

	nnSetPrimitive3DDepthTestPS3((NNE_BOOL)compare);
	nnSetPrimitive3DDepthFuncPS3(func);
	nnSetPrimitive3DDepthMaskPS3((NNE_BOOL)update);
#elif _WII
//	nnSetZModeGC((GXBool)compare, (GXCompare)func, (GXBool)update);
#endif

	_am_draw_state.zmode.compare	= (Uint16)compare;
	_am_draw_state.zmode.func		= func;
	_am_draw_state.zmode.update		= (Uint16)update;
}


/*----- Local functions ------------------------------------------------*/

/************************************************************************/
/* void _amDrawTaskMake(AMS_COMMAND_HEADER *command,                    */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] タスクの作成                                              */
/************************************************************************/
void _amDrawTaskMake(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_PARAM_MAKE_TASK	*param;
	AMS_TCB		*tcbp;
	Sint32		*work;

	param		= (AMS_PARAM_MAKE_TASK *)command->param;
	tcbp		= amTaskMake(_am_draw_task, param->proc,
			NULL, param->prio, 0, 0, "DRAW");
	work		= (Sint32 *)amTaskGetWork(tcbp);
	work[0]		= param->work_data[0];
	work[1]		= param->work_data[1];
}


/************************************************************************/
/* void _amDrawPrintf(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)*/
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] デバッグ文字の表示                                        */
/************************************************************************/
void _amDrawPrintf(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_PARAM_DEBUG_PRINT	*param;

	param		= (AMS_PARAM_DEBUG_PRINT *)command->param;
	nnPrint(_am_draw_offset_x + param->pos_x, param->pos_y, param->text);
}


/************************************************************************/
/* void _amDrawPrintColor(AMS_COMMAND_HEADER *command,                  */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] デバッグ文字色の設定                                      */
/************************************************************************/
void _amDrawPrintColor(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_PARAM_DEBUG_COLOR	*param;

	param		= (AMS_PARAM_DEBUG_COLOR *)command->param;
	nnSetPrintColor(param->color);
}


/************************************************************************/
/* void _amDrawHeapMap(AMS_COMMAND_HEADER *command,                     */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] メモリメーターの表示                                      */
/************************************************************************/
void _amDrawHeapMap(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

#if AMD_DEBUG
	AMS_HEAP_MAP_HEADER			*header;
	AMS_HEAP_MAP_BLOCK			*block;
	Sint32		i;
	float		x0, x1;

	header	= (AMS_HEAP_MAP_HEADER *)command->param;
	block	= (AMS_HEAP_MAP_BLOCK *)(header + 1);
	i		= header->block_num;
	x0		= header->x0;
	x1		= header->x1;

	nnBeginDrawPrimitive2D(NNE_PRIM2D_FMT_PC, NNE_PRIM_ALPHABLEND_OFF);
	for (; i > 0; i--, block++) {
		float	y0 = block->y0;
		float	y1 = block->y1;
		Uint32	color = block->color;
		NNS_PRIM2D_PC	vtx[4] = {
			{ { x0, y0,}, color, },
			{ { x1, y0,}, color, },
			{ { x0, y1,}, color, },
			{ { x1, y1,}, color, },
		};
		nnDrawPrimitive2D(NNE_PRIM_TRIANGLE_STRIP, vtx, 4, -2.0f);
	}
	nnEndDrawPrimitive2D();
#else
	UNREFERENCED_PARAMETER(command);
#endif
}


/************************************************************************/
/* void _amDrawThreadMap(AMS_COMMAND_HEADER *command,                   */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] スレッドメーターの表示                                    */
/************************************************************************/
void _amDrawThreadMap(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

#if AMD_DEBUG
	AMS_TASK_THREAD_MAP_HEADER	*header;
	AMS_TASK_THREAD_MAP_THREAD	*thread;
	AMS_TASK_THREAD_MAP_TCB		*tcb;
	Sint32		i, j;
	float		xx0, xx1, dx, yy0, dy, ny;
	Uint32		color = 0x00ffffff;

	header	= (AMS_TASK_THREAD_MAP_HEADER *)command->param;
	thread	= (AMS_TASK_THREAD_MAP_THREAD *)(header + 1);
	i		= header->thread_num;
	xx0		= header->x0;
	xx1		= header->x1;
	dx		= xx1 - xx0;
	yy0		= header->y0;
	dy		= header->dy;
	ny		= header->ny;

	nnBeginDrawPrimitive2D(NNE_PRIM2D_FMT_PC, NNE_PRIM_ALPHABLEND_OFF);
	for (; i > 0; i--) {
		float	y0 = yy0;
		float	y1 = yy0 + dy;
		j		= thread->thread_tcb_num;
		tcb		= (AMS_TASK_THREAD_MAP_TCB *)(thread + 1);
		for (; j > 0; j--, tcb++) {
			float	x0 = xx0 + dx * tcb->frame_start;
			float	x1 = xx0 + dx * tcb->frame_end;
			NNS_PRIM2D_PC	vtx[6] = {
				{ { x0, y0,}, color, },
				{ { x1, y0,}, color, },
				{ { x0, y1,}, color, },
				{ { x1, y0,}, color, },
				{ { x0, y1,}, color, },
				{ { x1, y1,}, color, },
			};
			nnDrawPrimitive2D(NNE_PRIM_TRIANGLE_LIST, vtx, 6, -1.0f);
		}
		yy0		+= ny;
		thread	= (AMS_TASK_THREAD_MAP_THREAD *)tcb;
	}
	nnEndDrawPrimitive2D();
#else
	UNREFERENCED_PARAMETER(command);
#endif
}


/************************************************************************/
/* void _amDrawObject(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)*/
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] オブジェクト描画                                          */
/************************************************************************/
void _amDrawObject(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	AMS_PARAM_DRAW_OBJECT	*param;
	NNS_MATRIX		base_mtx, *plt_mtx;
	NNF_NODESTATUS	*nstat;
	Sint32			num;

	amMatrixPush();

	// 不透明描画
	param	= (AMS_PARAM_DRAW_OBJECT *)command->param;
	num		= param->object->nNode;

	plt_mtx	= (NNS_MATRIX *)amDrawMallocWorkBuffer(
			sizeof(NNS_MATRIX) * num +
			sizeof(NNF_NODESTATUS) * ((num + 3) & ~3) +
			sizeof(AMS_COMMAND_HEADER) + sizeof(AMS_PARAM_SORT_DRAW_OBJECT) +
			sizeof(AMS_DRAWSTATE));
	nstat	= (NNF_NODESTATUS *)(plt_mtx + num);

	if (param->mtx != NULL) {
		nnMultiplyMatrix(&base_mtx, amMatrixGetCurrent(), param->mtx);
		nnMultiplyMatrix(&base_mtx, &_am_draw_world_view_matrix, &base_mtx);
	} else {
		nnMultiplyMatrix(&base_mtx,
				&_am_draw_world_view_matrix, amMatrixGetCurrent());
	}
	nnSetUpNodeStatusList(nstat, num, NND_NODESTATUS_NONE);
#if AMD_USE_DRAW_THREAD && !_IPHONE
	nnCalcMatrixPalette(plt_mtx, nstat, param->object, &base_mtx,
			&_am_draw_stack, NND_SETNODESTATUS_CLIP_HIDE);
#else
	nnCalcMatrixPalette(plt_mtx, nstat, param->object, &base_mtx,
			&_am_default_stack, NND_SETNODESTATUS_CLIP_HIDE);
#endif

#if _WII
	NNS_VTXLISTPTR	*vtx_list;
	nnCalcPliableVerticesGC(&vtx_list, param->object, plt_mtx);
	nnSetPliableVertexBufferGC(vtx_list, &_am_draw_unit_matrix);
#endif

	if (param->texlist != NULL)
		nnSetTextureList(param->texlist);

	nnSetMaterialCallback(param->material_func);

	if (command->command_id == AMD_COMMAND_DRAW_OBJECT) {
		nnDrawObject(
				param->object, plt_mtx, nstat,
				param->sub_obj_type
					| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
#if _IPHONE
					| NND_SUBOBJTYPE_TRANSPARENCY_ALL,
#else
					| NND_SUBOBJTYPE_OPAQUE | NND_SUBOBJTYPE_PUNCHTHROUGH,
#endif
				param->flag | drawflag | _am_draw_state.drawflag);
	} else {
		nnDrawMaterialMotionObject(
				(NNS_MATMOTOBJ *)param->object, plt_mtx, nstat,
				param->sub_obj_type
					| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
#if _IPHONE
					| NND_SUBOBJTYPE_TRANSPARENCY_ALL,
#else
					| NND_SUBOBJTYPE_OPAQUE | NND_SUBOBJTYPE_PUNCHTHROUGH,
#endif
				param->flag | drawflag | _am_draw_state.drawflag);
	}

	if (param->material_func != NULL) {
		nnSetMaterialCallback(NULL);
#if _WII
		GXSetTevDirect(GX_TEVSTAGE0);
#endif
	}

	// 半透明描画登録
#if !_IPHONE
#if NND_NEW_OBJECT_FORMAT
	if (param->object->fType & NND_OBJTYPE_TRANSPARENT) {
#else
	if (1) {
#endif
		AMS_COMMAND_HEADER			*command_sort;
		AMS_PARAM_SORT_DRAW_OBJECT	*param_sort;
		AMS_DRAWSTATE				*draw_state;

		command_sort	= (AMS_COMMAND_HEADER *)(nstat + ((num + 3) & ~3));
		param_sort		= (AMS_PARAM_SORT_DRAW_OBJECT *)(command_sort + 1);
		draw_state		= (AMS_DRAWSTATE *)(param_sort + 1);

		amDrawGetState(draw_state);

		param_sort->drawflag	= drawflag;
		param_sort->draw_object	= param;
		param_sort->mtx			= plt_mtx;
		param_sort->nstat_list	= nstat;
		param_sort->draw_state	= draw_state;
#if _WII
		param_sort->vtx_list	= vtx_list;
#endif
		if (command->command_id == AMD_COMMAND_DRAW_OBJECT)
			command_sort->command_id	= AMD_COMMAND_SORT_DRAW_OBJECT;
		else
			command_sort->command_id	= AMD_COMMAND_SORT_DRAW_OBJECT_MATMTN;
		command_sort->param			= param_sort;

		amDrawAddSort(command_sort,
			(Sint32)((param->object->Radius * param->scaleZ - NNM_MTX(base_mtx, 2, 3))
				* 100.0f));
	}
#endif

	amMatrixPop();
}

/************************************************************************/
/* void _amDrawObjectSetMaterial()                                      */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] マテリアル設定＋オブジェクト描画                          */
/************************************************************************/
void _amDrawObjectSetMaterial(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	AMS_PARAM_DRAW_OBJECT_MATERIAL	*param;
	param	= (AMS_PARAM_DRAW_OBJECT_MATERIAL *)command->param;

	amDrawPushState();

	// マテリアル設定
	if ( param->flag & (NND_DRAWOBJ_MATCTRL_DIFFUSE | NND_DRAWOBJ_MATCTRL_AMBIENT))
	{
		amDrawSetMaterialDiffuse(NNE_MATCTRLMODE_MODULATE, param->color.r, param->color.g, param->color.b);
		amDrawSetMaterialAmbient(NNE_MATCTRLMODE_MODULATE, param->color.r, param->color.g, param->color.b);
	}

	if ( param->flag & NND_DRAWOBJ_MATCTRL_ALPHA) 
		amDrawSetMaterialAlpha(NNE_MATCTRLMODE_MODULATE, param->color.a);

	if ( param->flag & NND_DRAWOBJ_MATCTRL_TEXOFFSET) 
		amDrawSetMaterialTexOffset(NNE_TEXSLOT_0, NNE_MATCTRLMODE_REPLACE, param->scroll_u, param->scroll_v);

	if ( param->blend != -1 )
	{
//		amDrawSetMaterialBlendMode((NNE_MATCTRL_BLEND)param->blend);
	}

	command->command_id = AMD_COMMAND_DRAW_OBJECT;

	_amDrawObject(command, drawflag);

	amDrawPopState();
}


/************************************************************************/
/* void _amDrawMotion(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)*/
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] モーション描画                                            */
/************************************************************************/
void _amDrawMotion(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	AMS_PARAM_DRAW_MOTION	*param;
	NNS_MATRIX		base_mtx, *plt_mtx;
	NNF_NODESTATUS	*nstat;
	Sint32			num;

	amMatrixPush();

	// 不透明描画
	param	= (AMS_PARAM_DRAW_MOTION *)command->param;
	num		= param->object->nNode;

	plt_mtx	= (NNS_MATRIX *)amDrawMallocWorkBuffer(
			(sizeof(NNF_NODESTATUS) + sizeof(NNS_MATRIX)) * num +
			sizeof(AMS_COMMAND_HEADER) + sizeof(AMS_PARAM_SORT_DRAW_OBJECT) +
			sizeof(AMS_DRAWSTATE));
	nstat	= (NNF_NODESTATUS *)(plt_mtx + num);

	if (param->mtx != NULL) {
		nnMultiplyMatrix(&base_mtx, amMatrixGetCurrent(), param->mtx);
		nnMultiplyMatrix(&base_mtx, &_am_draw_world_view_matrix, &base_mtx);
	} else {
		nnMultiplyMatrix(&base_mtx,
				&_am_draw_world_view_matrix, amMatrixGetCurrent());
	}
	nnSetUpNodeStatusList(nstat, num, NND_NODESTATUS_NONE);
#if AMD_USE_DRAW_THREAD && !_IPHONE
	nnCalcMatrixPaletteMotion(plt_mtx, nstat, param->object,
			param->motion, param->frame, &base_mtx, &_am_draw_stack,
			NND_SETNODESTATUS_CLIP_HIDE);
#else
	nnCalcMatrixPaletteMotion(plt_mtx, nstat, param->object,
			param->motion, param->frame, &base_mtx, &_am_default_stack,
			NND_SETNODESTATUS_CLIP_HIDE);
#endif

	nnCalcNodeHideMotion(nstat, param->motion, param->frame);

#if _WII
	NNS_VTXLISTPTR	*vtx_list;
	nnCalcPliableVerticesGC(&vtx_list, param->object, plt_mtx);
	nnSetPliableVertexBufferGC(vtx_list, &_am_draw_unit_matrix);
#endif

	if (param->texlist != NULL)
		nnSetTextureList(param->texlist);

	nnSetMaterialCallback(param->material_func);

	if (command->command_id == AMD_COMMAND_DRAW_MOTION) {
		nnDrawObject(
				param->object, plt_mtx, nstat,
				param->sub_obj_type
					| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
#if _IPHONE
					| NND_SUBOBJTYPE_TRANSPARENCY_ALL,
#else
					| NND_SUBOBJTYPE_OPAQUE | NND_SUBOBJTYPE_PUNCHTHROUGH,
#endif
				param->flag | drawflag | _am_draw_state.drawflag);
	} else {
		nnDrawMaterialMotionObject(
				(NNS_MATMOTOBJ *)param->object, plt_mtx, nstat,
				param->sub_obj_type
					| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
#if _IPHONE
					| NND_SUBOBJTYPE_TRANSPARENCY_ALL,
#else
					| NND_SUBOBJTYPE_OPAQUE | NND_SUBOBJTYPE_PUNCHTHROUGH,
#endif
				param->flag | drawflag | _am_draw_state.drawflag);
	}

	if (param->material_func != NULL) {
		nnSetMaterialCallback(NULL);
#if _WII
		GXSetTevDirect(GX_TEVSTAGE0);
#endif
	}

	// 半透明描画登録
#if !_IPHONE
#if NND_NEW_OBJECT_FORMAT
	if (param->object->fType & NND_OBJTYPE_TRANSPARENT) {
#else
	if (1) {
#endif
		AMS_COMMAND_HEADER			*command_sort;
		AMS_PARAM_SORT_DRAW_OBJECT	*param_sort;
		AMS_DRAWSTATE				*draw_state;

		command_sort	= (AMS_COMMAND_HEADER *)(nstat + num);
		param_sort		= (AMS_PARAM_SORT_DRAW_OBJECT *)(command_sort + 1);
		draw_state		= (AMS_DRAWSTATE *)(param_sort + 1);

		amDrawGetState(draw_state);

		param_sort->drawflag	= drawflag;
		param_sort->draw_object	= (AMS_PARAM_DRAW_OBJECT *)param;
		param_sort->mtx			= plt_mtx;
		param_sort->nstat_list	= nstat;
		param_sort->draw_state	= draw_state;
#if _WII
		param_sort->vtx_list	= vtx_list;
#endif
		if (command->command_id == AMD_COMMAND_DRAW_OBJECT)
			command_sort->command_id	= AMD_COMMAND_SORT_DRAW_OBJECT;
		else
			command_sort->command_id	= AMD_COMMAND_SORT_DRAW_OBJECT_MATMTN;
		command_sort->param			= param_sort;

		amDrawAddSort(command_sort,
				(Sint32)((param->object->Radius - NNM_MTX(base_mtx, 2, 3))
				* 100.0f));
	}
#endif

	amMatrixPop();
}


/************************************************************************/
/* void _amDrawMotionTRS(AMS_COMMAND_HEADER *command,                   */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] モーション描画(TRS付き)                                   */
/************************************************************************/
void _amDrawMotionTRS(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	AMS_PARAM_DRAW_MOTION_TRS	*param;
	NNS_MATRIX		base_mtx, *plt_mtx;
	NNF_NODESTATUS	*nstat;
	Sint32			node_num, plt_num;

	amMatrixPush();

	// 不透明描画
	param	= (AMS_PARAM_DRAW_MOTION_TRS *)command->param;
	node_num	= param->object->nNode;
	plt_num		= param->object->nMtxPal;

	if ((command->command_id == AMD_COMMAND_DRAW_MOTION_TRS_MATMTN)
			&& (param->mmotion != NULL)) {
		NNS_MATMOTOBJ	*mmobject;
		mmobject	= (NNS_MATMOTOBJ *)(param->trslist + node_num);
		nnInitMaterialMotionObject(mmobject,
				param->object, param->mmotion);
		nnCalcMaterialMotion(mmobject,
				param->object, param->mmotion, param->mframe);
		param->object	= (NNS_OBJECT *)mmobject;
	}

	plt_mtx	= (NNS_MATRIX *)amDrawMallocWorkBuffer(
			sizeof(NNF_NODESTATUS) * node_num +
			sizeof(NNS_MATRIX) * plt_num +
			sizeof(AMS_COMMAND_HEADER) + sizeof(AMS_PARAM_SORT_DRAW_OBJECT) +
			sizeof(AMS_DRAWSTATE));
	nstat	= (NNF_NODESTATUS *)(plt_mtx + plt_num);

	if (param->mtx != NULL) {
		nnMultiplyMatrix(&base_mtx, amMatrixGetCurrent(), param->mtx);
		nnMultiplyMatrix(&base_mtx, &_am_draw_world_view_matrix, &base_mtx);
	} else {
		nnMultiplyMatrix(&base_mtx,
				&_am_draw_world_view_matrix, amMatrixGetCurrent());
	}
	nnSetUpNodeStatusList(nstat, node_num, NND_NODESTATUS_NONE);
#if AMD_USE_DRAW_THREAD && !_IPHONE
	nnCalcMatrixPaletteTRSList(plt_mtx, nstat, param->object,
			param->trslist, &base_mtx, &_am_draw_stack,
			NND_SETNODESTATUS_CLIP_HIDE);
#else
	nnCalcMatrixPaletteTRSList(plt_mtx, nstat, param->object,
			param->trslist, &base_mtx, &_am_default_stack,
			NND_SETNODESTATUS_CLIP_HIDE);
#endif

	if (param->motion != NULL)
		nnCalcNodeHideMotion(nstat, param->motion, param->frame);

#if _WII
	NNS_VTXLISTPTR	*vtx_list;
	nnCalcPliableVerticesGC(&vtx_list, param->object, plt_mtx);
	nnSetPliableVertexBufferGC(vtx_list, &_am_draw_unit_matrix);
#endif

	if (param->texlist != NULL)
		nnSetTextureList(param->texlist);

	nnSetMaterialCallback(param->material_func);

	if (command->command_id == AMD_COMMAND_DRAW_MOTION_TRS) {
		nnDrawObject(
				param->object, plt_mtx, nstat,
				param->sub_obj_type
					| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
#if _IPHONE
					| NND_SUBOBJTYPE_TRANSPARENCY_ALL,
#else
					| NND_SUBOBJTYPE_OPAQUE | NND_SUBOBJTYPE_PUNCHTHROUGH,
#endif
				param->flag | drawflag | _am_draw_state.drawflag);
	} else {
		nnDrawMaterialMotionObject(
				(NNS_MATMOTOBJ *)param->object, plt_mtx, nstat,
				param->sub_obj_type
					| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
#if _IPHONE
					| NND_SUBOBJTYPE_TRANSPARENCY_ALL,
#else
					| NND_SUBOBJTYPE_OPAQUE | NND_SUBOBJTYPE_PUNCHTHROUGH,
#endif
				param->flag | drawflag | _am_draw_state.drawflag);
	}

	if (param->material_func != NULL) {
		nnSetMaterialCallback(NULL);
#if _WII
		GXSetTevDirect(GX_TEVSTAGE0);
#endif
	}

	// 半透明描画登録
#if !_IPHONE
#if NND_NEW_OBJECT_FORMAT
	if (param->object->fType & NND_OBJTYPE_TRANSPARENT) {
#else
	if (1) {
#endif
		AMS_COMMAND_HEADER			*command_sort;
		AMS_PARAM_SORT_DRAW_OBJECT	*param_sort;
		AMS_DRAWSTATE				*draw_state;

		command_sort	= (AMS_COMMAND_HEADER *)(nstat + node_num);
		param_sort		= (AMS_PARAM_SORT_DRAW_OBJECT *)(command_sort + 1);
		draw_state		= (AMS_DRAWSTATE *)(param_sort + 1);

		amDrawGetState(draw_state);

		param_sort->drawflag	= drawflag;
		param_sort->draw_object	= (AMS_PARAM_DRAW_OBJECT *)param;
		param_sort->mtx			= plt_mtx;
		param_sort->nstat_list	= nstat;
		param_sort->draw_state	= draw_state;
#if _WII
		param_sort->vtx_list	= vtx_list;
#endif
		if (command->command_id == AMD_COMMAND_DRAW_MOTION_TRS)
			command_sort->command_id	= AMD_COMMAND_SORT_DRAW_OBJECT;
		else
			command_sort->command_id	= AMD_COMMAND_SORT_DRAW_OBJECT_MATMTN;
		command_sort->param			= param_sort;

		amDrawAddSort(command_sort,
				(Sint32)((param->object->Radius - NNM_MTX(base_mtx, 2, 3))
				* 100.0f));
	}
#endif

	amMatrixPop();
}

/************************************************************************/
/* void _amDrawPrimitive2D(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)*/
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] プリミティブ描画（2D）                                    */
/************************************************************************/
void _amDrawPrimitive2D(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_PARAM_DRAW_PRIMITIVE	*param;
	AMS_COMMAND_HEADER			*command_sort;
	NNS_MATRIX					*mtx;
	char                        *buf;
	param	= (AMS_PARAM_DRAW_PRIMITIVE *)command->param;
	
	if (param->texlist != NULL)
	{
        nnSetPrimitiveTexNum( param->texlist, param->texId );
	    nnSetPrimitiveTexState( NNE_PRIM_TEXBLEND_MODULATE, NNE_PRIM_TEXCOORD_UV, param->uwrap, param->vwrap );
	}

	// 2Dポリゴン描画
	if ( param->noSort || (param->ablend == NNE_PRIM_ALPHABLEND_OFF) )
	{
		// αブレンドやＺテストなどを設定
		_amDrawSetPrimitive2DParam(command, drawflag);

		nnBeginDrawPrimitive2D( param->format2D, param->ablend );

		switch ( param->format2D )
		{
		case NNE_PRIM2D_FMT_PCT:
			amDrawPrimitive2D( param->format2D, param->type, param->vtxPCT2D, param->count, param->zOffset );
			break;

		case NNE_PRIM2D_FMT_PC:
			amDrawPrimitive2D( param->format2D, param->type, param->vtxPC2D, param->count, param->zOffset );
			break;

		default:
			amAssert(0);
			break;
		}
		
		nnEndDrawPrimitive2D();
	}
	// 半透明
	else
	{
		// AMS_COMMAND_HEADER のコピー
		command_sort	= (AMS_COMMAND_HEADER *)amDrawMallocWorkBuffer(sizeof(AMS_COMMAND_HEADER));
		memcpy(command_sort, command, sizeof(AMS_COMMAND_HEADER));
		command_sort->command_id	= AMD_COMMAND_SORT_DRAW_PRIMITIVE2D;

		// AMS_PARAM_DRAW_PRIMITIVE とマトリクスのコピー
		param	= (AMS_PARAM_DRAW_PRIMITIVE *)amDrawMallocWorkBuffer(
					sizeof(AMS_PARAM_DRAW_PRIMITIVE) + sizeof(NNS_MATRIX));
		buf     = (char*)param;
		mtx		= (NNS_MATRIX *)(buf + sizeof(AMS_PARAM_DRAW_PRIMITIVE));
		nnCopyMatrix(mtx, amMatrixGetCurrent());
		memcpy(param, (AMS_PARAM_DRAW_PRIMITIVE*)command->param, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
		param->mtx = mtx;

		command_sort->param = param;
		amDrawAddSort(command_sort, (Sint32)(param->zOffset * 100.0f));
	}
}

/************************************************************************/
/* void _amDrawPrimitive3D(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)*/
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] プリミティブ描画（3D）                                    */
/************************************************************************/
void _amDrawPrimitive3D(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_PARAM_DRAW_PRIMITIVE	*param;
	AMS_COMMAND_HEADER			*command_sort;
	NNS_MATRIX					*mtx;
	char                        *buf;
	param	= (AMS_PARAM_DRAW_PRIMITIVE *)command->param;

	if ((param->texlist != NULL) && ((Sint32)param->texId != -1))
	{
        nnSetPrimitiveTexNum( param->texlist, param->texId );
		nnSetPrimitiveTexState( NNE_PRIM_TEXBLEND_MODULATE, NNE_PRIM_TEXCOORD_UV, param->uwrap, param->vwrap );
	}
	
	// 3Dポリゴン描画
	if ( param->noSort || (param->ablend == NNE_PRIM_ALPHABLEND_OFF) )
	{
		// αブレンドやＺテストなどを設定
		_amDrawSetPrimitive3DParam(command, drawflag);

		nnBeginDrawPrimitive3D( param->format3D, param->ablend, NNE_PRIM_LIGHT_DISABLE, NNE_PRIM_CULL_NONE );

		switch ( param->format3D )
		{
		case NNE_PRIM3D_FMT_PCT:
			nnDrawPrimitive3D( param->type, param->vtxPCT3D, param->count );
			break;

		case NNE_PRIM3D_FMT_PC:
			nnDrawPrimitive3D( param->type, param->vtxPC3D, param->count );
			break;

		default:
			amAssert(0);
			break;
		}

	    nnEndDrawPrimitive3D();
	}
	// 半透明
	else
	{
		// AMS_COMMAND_HEADER のコピー
		command_sort	= (AMS_COMMAND_HEADER *)amDrawMallocWorkBuffer(sizeof(AMS_COMMAND_HEADER));
		memcpy(command_sort, command, sizeof(AMS_COMMAND_HEADER));
		command_sort->command_id	= AMD_COMMAND_SORT_DRAW_PRIMITIVE3D;

		// AMS_PARAM_DRAW_PRIMITIVE とマトリクスのコピー
		param	= (AMS_PARAM_DRAW_PRIMITIVE *)amDrawMallocWorkBuffer(
					sizeof(AMS_PARAM_DRAW_PRIMITIVE) + sizeof(NNS_MATRIX));
		buf     = (char*)param;
		mtx		= (NNS_MATRIX *)(buf + sizeof(AMS_PARAM_DRAW_PRIMITIVE));
		nnCopyMatrix(mtx, amMatrixGetCurrent());
		memcpy(param, (AMS_PARAM_DRAW_PRIMITIVE*)command->param, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
		param->mtx = mtx;

		// 確認用
		command_sort->param = param;
		amDrawAddSort(command_sort, (Sint32)(param->sortZ * 100.0f));
	}	
}


/************************************************************************/
/* void _amDrawSortObject(AMS_COMMAND_HEADER *command,                  */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] オブジェクト描画(ソート)                                  */
/************************************************************************/
void _amDrawSortObject(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_PARAM_SORT_DRAW_OBJECT	*param_sort;
	AMS_PARAM_DRAW_OBJECT		*param;

	// 半透明描画
	param_sort	= (AMS_PARAM_SORT_DRAW_OBJECT *)command->param;
	param		= param_sort->draw_object;

#if _WII
	nnSetPliableVertexBufferGC(param_sort->vtx_list, &_am_draw_unit_matrix);
#endif

	if (param->texlist != NULL)
		nnSetTextureList(param->texlist);

	if (param_sort->draw_state != NULL)
		amDrawSetState(param_sort->draw_state);

	nnSetMaterialCallback(param->material_func);

	if (command->command_id == AMD_COMMAND_SORT_DRAW_OBJECT) {
		nnDrawObject(
				param->object, param_sort->mtx, param_sort->nstat_list,
				param->sub_obj_type
						| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
						| NND_SUBOBJTYPE_TRANSPARENT,
				param->flag | param_sort->drawflag | _am_draw_state.drawflag);
	} else {
		nnDrawMaterialMotionObject(
				(NNS_MATMOTOBJ *)param->object, param_sort->mtx, param_sort->nstat_list,
				param->sub_obj_type
						| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
						| NND_SUBOBJTYPE_TRANSPARENT,
				param->flag | param_sort->drawflag | _am_draw_state.drawflag);
	}

	if (param->material_func != NULL) {
		nnSetMaterialCallback(NULL);
#if _WII
		GXSetTevDirect(GX_TEVSTAGE0);
#endif
	}

#if _WII
	nnSetPliableVertexBufferGC(NULL, &_am_draw_unit_matrix);
#endif
}

/************************************************************************/
/* void _amDrawSortPrimitive3D(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)*/
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] プリミティブ描画（3D）                                    */
/************************************************************************/
void _amDrawSortPrimitive3D(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_PARAM_DRAW_PRIMITIVE	*param;
	NNS_MATRIX  base_mtx;
	param	= (AMS_PARAM_DRAW_PRIMITIVE *)command->param;

	if ((param->texlist != NULL) && ((Sint32)param->texId != -1))
	{
        nnSetPrimitiveTexNum( param->texlist, param->texId );
	    nnSetPrimitiveTexState( NNE_PRIM_TEXBLEND_MODULATE, NNE_PRIM_TEXCOORD_UV, NNE_PRIM_TEXWRAP_REPEAT, NNE_PRIM_TEXWRAP_REPEAT );
	}

	nnCopyMatrix(&base_mtx, param->mtx);
    nnMultiplyMatrix(&base_mtx, amDrawGetWorldViewMatrix(), &base_mtx);
    nnSetPrimitive3DMatrix(&base_mtx);

	// αブレンドやＺテストなどを設定
	_amDrawSetPrimitive3DParam(command, drawflag);
	
	// 3Dポリゴン描画
	nnBeginDrawPrimitive3D( param->format3D, param->ablend, NNE_PRIM_LIGHT_DISABLE, NNE_PRIM_CULL_NONE );

	switch ( param->format3D )
	{
		case NNE_PRIM3D_FMT_PCT:
			nnDrawPrimitive3D( param->type, param->vtxPCT3D, param->count );
			break;

		case NNE_PRIM3D_FMT_PC:
			nnDrawPrimitive3D( param->type, param->vtxPC3D, param->count );
			break;

		default:
			amAssert(0);
			break;
	}

	nnEndDrawPrimitive3D();
}

/************************************************************************/
/* void _amDrawSortPrimitive2D(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)*/
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] プリミティブ描画（2D）                                    */
/************************************************************************/
void _amDrawSortPrimitive2D(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_PARAM_DRAW_PRIMITIVE	*param;
	param	= (AMS_PARAM_DRAW_PRIMITIVE *)command->param;

	if ((param->texlist != NULL) && ((Sint32)param->texId != -1))
	{
        nnSetPrimitiveTexNum( param->texlist, param->texId );
	    nnSetPrimitiveTexState( NNE_PRIM_TEXBLEND_MODULATE, NNE_PRIM_TEXCOORD_UV, NNE_PRIM_TEXWRAP_REPEAT, NNE_PRIM_TEXWRAP_REPEAT );
	}

	// αブレンドやＺテストなどを設定
	_amDrawSetPrimitive2DParam(command, drawflag);
	
	// 3Dポリゴン描画
	nnBeginDrawPrimitive2D( param->format2D, param->ablend);

	switch ( param->format2D )
	{
		case NNE_PRIM2D_FMT_PCT:
			nnDrawPrimitive2D( param->type, param->vtxPCT2D, param->count, param->zOffset );
			break;

		case NNE_PRIM2D_FMT_PC:
			nnDrawPrimitive2D( param->type, param->vtxPC2D, param->count, param->zOffset );
			break;

		default:
			amAssert(0);
			break;
	}

	nnEndDrawPrimitive2D();
}


/************************************************************************/
/* void _amDrawSetDiffuse(AMS_COMMAND_HEADER *command,                  */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] ディフューズカラーの設定                                  */
/************************************************************************/
void _amDrawSetDiffuse(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_DRAWSTATE_DIFFUSE	*param;

	param		= (AMS_DRAWSTATE_DIFFUSE *)command->param;

	amDrawSetMaterialDiffuse(param->mode, param->r, param->g, param->b);
}


/************************************************************************/
/* void _amDrawSetAmbient(AMS_COMMAND_HEADER *command,                  */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] アンビエントカラーの設定                                  */
/************************************************************************/
void _amDrawSetAmbient(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_DRAWSTATE_AMBIENT	*param;

	param		= (AMS_DRAWSTATE_AMBIENT *)command->param;

	amDrawSetMaterialAmbient(param->mode, param->r, param->g, param->b);
}


/************************************************************************/
/* void _amDrawSetAlpha(AMS_COMMAND_HEADER *command,                    */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] α値の設定                                                */
/************************************************************************/
void _amDrawSetAlpha(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_DRAWSTATE_ALPHA		*param;

	param		= (AMS_DRAWSTATE_ALPHA *)command->param;

	amDrawSetMaterialAlpha(param->mode, param->alpha);
}


/************************************************************************/
/* void _amDrawSetSpecular(AMS_COMMAND_HEADER *command,                 */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] スペキュラーカラーの設定                                  */
/************************************************************************/
void _amDrawSetSpecular(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_DRAWSTATE_SPECULAR	*param;

	param		= (AMS_DRAWSTATE_SPECULAR *)command->param;

	amDrawSetMaterialSpecular(param->mode, param->r, param->g, param->b);
}


/************************************************************************/
/* void _amDrawSetEnvMap(AMS_COMMAND_HEADER *command,                   */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] 環境マップの設定                                          */
/************************************************************************/
void _amDrawSetEnvMap(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_DRAWSTATE_ENVMAP	*param;

	param		= (AMS_DRAWSTATE_ENVMAP *)command->param;

	amDrawSetMaterialEnvMap(param->texsrc, &param->texmtx);
}


/************************************************************************/
/* void _amDrawSetBlend(AMS_COMMAND_HEADER *command,                    */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] ブレンドモードの設定                                      */
/************************************************************************/
void _amDrawSetBlend(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_DRAWSTATE_BLEND		*param;

	param		= (AMS_DRAWSTATE_BLEND *)command->param;

	amDrawSetMaterialBlendMode(param->mode);
}


/************************************************************************/
/* void _amDrawSetTexOffset(AMS_COMMAND_HEADER *command,                */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] テクスチャオフセットの設定                                */
/************************************************************************/
void _amDrawSetTexOffset(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_PARAM_SET_TEXOFFSET		*param;

	param		= (AMS_PARAM_SET_TEXOFFSET *)command->param;

	amDrawSetMaterialTexOffset((NNE_TEXSLOT)param->slot,
			param->texoffset.mode, param->texoffset.u, param->texoffset.v);
}


/************************************************************************/
/* void _amDrawSetFog(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)*/
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] フォグの設定                                              */
/************************************************************************/
void _amDrawSetFog(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_DRAWSTATE_FOG		*param;

	param		= (AMS_DRAWSTATE_FOG *)command->param;

	amDrawSetFog(param->flag);
}


/************************************************************************/
/* void _amDrawSetFogColor(AMS_COMMAND_HEADER *command,                 */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] フォグカラーの設定                                        */
/************************************************************************/
void _amDrawSetFogColor(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_DRAWSTATE_FOG_COLOR		*param;

	param		= (AMS_DRAWSTATE_FOG_COLOR *)command->param;

	amDrawSetFogColor(param->r, param->g, param->b);
}


/************************************************************************/
/* void _amDrawSetFogRange(AMS_COMMAND_HEADER *command,                 */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] フォグレンジの設定                                        */
/************************************************************************/
void _amDrawSetFogRange(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_DRAWSTATE_FOG_RANGE		*param;

	param		= (AMS_DRAWSTATE_FOG_RANGE *)command->param;

	amDrawSetFogRange(param->fnear, param->ffar);
}


/************************************************************************/
/* void _amDrawSetZMode(AMS_COMMAND_HEADER *command,                    */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] フォグカラーの設定                                        */
/************************************************************************/
void _amDrawSetZMode(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_DRAWSTATE_Z_MODE		*param;

	param		= (AMS_DRAWSTATE_Z_MODE *)command->param;

	amDrawSetZMode((NNE_BOOL)param->compare,
			param->func, (NNE_BOOL)param->update);
}


/************************************************************************/
/* void _amDrawSetPrimitive3DParam(AMS_COMMAND_HEADER *command,         */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] 3Dプリミティブパラメータの設定                            */
/************************************************************************/
void _amDrawSetPrimitive3DParam(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_PARAM_DRAW_PRIMITIVE	*param;
	param	= (AMS_PARAM_DRAW_PRIMITIVE *)command->param;

#if _WII
	GXBool ztest = GX_TRUE;
	GXBool zmask = GX_FALSE;
	GXCompare zfunc = GX_LEQUAL;
#endif

	// αテスト
	if ( param->aTest )
	{
#if _PC | _XBOX 
		// 暫定的に設定（アルファ比較式やアルファ参照値は要検討）
		nnSetPrimitive3DAlphaTestDXG20(NNE_TRUE);
		nnSetPrimitive3DAlphaFuncDXG20(NNE_CMPFUNC_GREATER, 0x80);
#elif _PS3
        nnSetPrimitive3DAlphaFuncPS3(NND_CMPFUNC_PS3_GREATER, 0.5f);
#elif _WII
        nnSetPrimitive3DAlphaCompareGC( GX_GREATER, 0x80, GX_AOP_AND, GX_GREATER, 0x80 );
#elif _IPHONE
		nnSetPrimitive3DAlphaFuncGL(NND_CMPFUNC_GL_GREATER, 0.5f);
#endif
	}
	// αテストしない
	else
	{
#if _PC | _XBOX
        nnSetPrimitive3DAlphaTestDXG20(NNE_FALSE);
#elif _PS3
        nnSetPrimitive3DAlphaFuncPS3(NND_CMPFUNC_PS3_ALWAYS, 0.5f);
#elif _WII
		nnSetPrimitive3DAlphaCompareGC( GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0 );
#elif _IPHONE
		nnSetPrimitive3DAlphaFuncGL(NND_CMPFUNC_GL_ALWAYS, 0.5f);
#endif
	}

	// Zマスク（Ｚバッファを更新しない）
	if( param->zMask )
	{
#if _PC | _XBOX
	    nnSetPrimitive3DDepthMaskDXG20(NNE_FALSE);
#elif _PS3
        nnSetPrimitive3DDepthMaskPS3(NNE_FALSE);
#elif _WII
		zmask = GX_FALSE;
#elif _IPHONE
		nnSetPrimitive3DDepthMaskGL(NNE_FALSE);
#endif
	}
	else
	{
#if _PC | _XBOX
	    nnSetPrimitive3DDepthMaskDXG20(NNE_TRUE);
#elif _PS3
        nnSetPrimitive3DDepthMaskPS3(NNE_TRUE);
#elif _WII
		zmask = GX_TRUE;
#elif _IPHONE
		nnSetPrimitive3DDepthMaskGL(NNE_TRUE);
#endif
	}

	// Zテストする
	if( param->zTest )
	{
#if _PC | _XBOX
        nnSetPrimitive3DDepthTestDXG20(NNE_TRUE);
		nnSetPrimitive3DDepthFuncDXG20(NNE_CMPFUNC_LESSEQUAL);
#elif _PS3
		nnSetPrimitive3DDepthFuncPS3(NND_CMPFUNC_PS3_LEQUAL);
#elif _WII
		ztest = GX_TRUE;
		zfunc = GX_LEQUAL;
#elif _IPHONE
		nnSetPrimitive3DDepthFuncGL(NND_CMPFUNC_GL_LEQUAL);
#endif
	}
	// Zテストしない
	else
	{
#if _PC | _XBOX
        nnSetPrimitive3DDepthTestDXG20(NNE_FALSE);
#elif _PS3
        nnSetPrimitive3DDepthFuncPS3(NND_CMPFUNC_PS3_ALWAYS);
#elif _WII
		ztest = GX_FALSE;
		zfunc = GX_NEVER;
#elif _IPHONE
		nnSetPrimitive3DDepthFuncGL(NND_CMPFUNC_GL_ALWAYS);
#endif
	}
	
#if _WII
	nnSetPrimitive3DZModeGC(ztest, zfunc, zmask);
#endif

	// αブレンド
	if ( param->ablend )
	{
#if _PC | _XBOX
		nnSetPrimitive3DBlendDXG20( param->bldSrc, param->bldDst, param->bldMode );
#elif _PS3
        nnSetPrimitive3DBlendPS3( param->bldSrc, param->bldDst, param->bldMode);
#elif _WII
		nnSetPrimitive3DBlendModeGC( param->bldMode, param->bldSrc, param->bldDst, GX_LO_NOOP );
#elif _IPHONE
		if ( param->bldMode == NND_BLENDOP_GL_FUNC_ADD )
		{
			switch ( param->bldDst )
			{
			case NND_BLENDFUNC_GL_ONE: // 加算
				nnSetPrimitiveBlend( NNE_PRIM_BLEND_ADD );
				break;

			case NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA: // 乗算
				nnSetPrimitiveBlend( NNE_PRIM_BLEND_BLEND );
				break;

			default:
				nnSetPrimitiveBlend( NNE_PRIM_BLEND_BLEND );
				break;
			}
		}
#endif
	}
	else
	{
#if _WII
		nnSetPrimitiveBlend( NNE_PRIM_BLEND_BLEND ); // これを呼んでいないと加算半透明が設定される
#endif	
	}
}

/************************************************************************/
/* void _amDrawSetPrimitive2DParam(AMS_COMMAND_HEADER *command,         */
/*                                                NNF_DRAWOBJ drawflag) */
/*----------------------------------------------------------------------*/
/* [INPUT] command  : 実行する描画コマンドのヘッダ                      */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] 2Dプリミティブパラメータの設定                            */
/************************************************************************/
void _amDrawSetPrimitive2DParam(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	UNREFERENCED_PARAMETER(drawflag);

	AMS_PARAM_DRAW_PRIMITIVE	*param;
	param	= (AMS_PARAM_DRAW_PRIMITIVE *)command->param;

#if _WII
	GXBool ztest = GX_TRUE;
	GXBool zmask = GX_FALSE;
	GXCompare zfunc = GX_LEQUAL;
#endif

	// αテスト
	if ( param->aTest )
	{
#if _PC | _XBOX 
		// 暫定的に設定（アルファ比較式やアルファ参照値は要検討）
		nnSetPrimitive2DAlphaTestDXG20(NNE_TRUE);
		nnSetPrimitive2DAlphaFuncDXG20(NNE_CMPFUNC_GREATER, 0x80);
#elif _PS3
        nnSetPrimitive2DAlphaFuncPS3(NND_CMPFUNC_PS3_GREATER, 0.5f);
#elif _WII
        nnSetPrimitive2DAlphaCompareGC( GX_GREATER, 0x80, GX_AOP_AND, GX_GREATER, 0x80 );
#elif _IPHONE
		nnSetPrimitive2DAlphaFuncGL(NND_CMPFUNC_GL_GREATER, 0.5f);
#endif
	}
	// αテストしない
	else
	{
#if _PC | _XBOX
        nnSetPrimitive2DAlphaTestDXG20(NNE_FALSE);
#elif _PS3
        nnSetPrimitive2DAlphaFuncPS3(NND_CMPFUNC_PS3_ALWAYS, 0.5f);
#elif _WII
		nnSetPrimitive2DAlphaCompareGC( GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0 );
#elif _IPHONE
		nnSetPrimitive2DAlphaFuncGL(NND_CMPFUNC_GL_ALWAYS, 0.5f);
#endif
	}

	// Zマスク（Ｚバッファを更新しない）
	if( param->zMask )
	{
#if _PC | _XBOX
		// Z書き込みをしない
	    nnSetPrimitive2DDepthMaskDXG20(NNE_FALSE);
#elif _PS3
        nnSetPrimitive2DDepthMaskPS3(NNE_FALSE);
#elif _WII
		zmask = GX_FALSE;
#elif _IPHONE
		nnSetPrimitive3DDepthMaskGL(NNE_FALSE);
#endif
	}
	else
	{
#if _PC | _XBOX
        // Z書き込みをする
	    nnSetPrimitive2DDepthMaskDXG20(NNE_TRUE);
#elif _PS3
        nnSetPrimitive2DDepthMaskPS3(NNE_TRUE);
#elif _WII
		zmask = GX_TRUE;
#elif _IPHONE
		nnSetPrimitive3DDepthMaskGL(NNE_TRUE);
#endif
	}

	// Zテストする
	if( param->zTest )
	{
#if _PC | _XBOX
        nnSetPrimitive2DDepthTestDXG20(NNE_TRUE);
		nnSetPrimitive2DDepthFuncDXG20(NNE_CMPFUNC_LESSEQUAL);
#elif _PS3
		nnSetPrimitive2DDepthFuncPS3(NND_CMPFUNC_PS3_LEQUAL);
#elif _WII
		ztest = GX_TRUE;
		zfunc = GX_LEQUAL;
#elif _IPHONE
		nnSetPrimitive3DDepthFuncGL(NND_CMPFUNC_GL_LEQUAL);
#endif
	}
	// Zテストしない
	else
	{
#if _PC | _XBOX
        nnSetPrimitive2DDepthTestDXG20(NNE_FALSE);
#elif _PS3
        nnSetPrimitive2DDepthFuncPS3(NND_CMPFUNC_PS3_ALWAYS);
#elif _WII
		ztest = GX_FALSE;
		zfunc = GX_NEVER;
#elif _IPHONE
		nnSetPrimitive3DDepthFuncGL(NND_CMPFUNC_GL_ALWAYS);
#endif
	}
	
#if _WII
	nnSetPrimitive2DZModeGC(ztest, zfunc, zmask);
#endif

	// αブレンド
	if ( param->ablend )
	{
#if _PC | _XBOX
		nnSetPrimitive2DBlendDXG20( param->bldSrc, param->bldDst, param->bldMode );
#elif _PS3
        nnSetPrimitive2DBlendPS3( param->bldSrc, param->bldDst, param->bldMode);
#elif _WII
		nnSetPrimitive2DBlendModeGC( param->bldMode, param->bldSrc, param->bldDst, GX_LO_NOOP );
#elif _IPHONE
		if ( param->bldMode == NND_BLENDOP_GL_FUNC_ADD )
		{
			switch ( param->bldDst )
			{
			case NND_BLENDFUNC_GL_ONE: // 加算
				nnSetPrimitiveBlend( NNE_PRIM_BLEND_ADD );
				break;

			case NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA: // 乗算
				nnSetPrimitiveBlend( NNE_PRIM_BLEND_BLEND );
				break;

			default:
				nnSetPrimitiveBlend( NNE_PRIM_BLEND_BLEND );
				break;
			}
		}
#endif
	}
	else
	{
#if _WII
		nnSetPrimitiveBlend( NNE_PRIM_BLEND_BLEND ); // これを呼んでいないと加算半透明が設定される
#endif	
	}
}


/************************************************************************/
/* void _amDrawRegistNop(AMS_REGISTLIST *regist)                        */
/*----------------------------------------------------------------------*/
/* [INPUT] regist : 実行する登録コマンド                                */
/* [FUNCTION] NOP                                                       */
/************************************************************************/
void _amDrawRegistNop(AMS_REGISTLIST *regist)
{
	regist->command_id		= 0;
}


/************************************************************************/
/* void _amDrawLoadTexture(AMS_REGISTLIST *regist)                      */
/*----------------------------------------------------------------------*/
/* [INPUT] regist : 実行する登録コマンド                                */
/* [FUNCTION] テクスチャの登録                                          */
/************************************************************************/
void _amDrawLoadTexture(AMS_REGISTLIST *regist)
{
	AMS_PARAM_LOAD_TEXTURE	*param = (AMS_PARAM_LOAD_TEXTURE *)regist->param;

#if _PC | _XBOX
	if (nnLoadTextureMemoryOneDXG20(
			param->pTexInfo,
			param->tex,
			param->minfilter,
			param->magfilter,
			param->globalIndex,
			param->bank,
			param->flag,
			param->size) < 0)
		amSystemLog("[WARN] LoadTexture failed.\n");
#elif _PS3
	NNS_TEXTURE_PS3		*tex = amPs3LoadDDSTexture2D((void *)param->tex);
	param->pTexInfo->pTexture	= tex;
	if (tex != NULL)
		amPs3SetTextureAttribute(tex, param);
	else
		amSystemLog("[ERR] Texture load failed.\n");
#elif _WII
	if (param->flag & NND_TEXFLAG_ALLOCATE) {
		void	*tbuf = (void *)param->tex;
		param->tex	= MEMAllocFromExpHeapEx(
				_am_wii_texture_heap, param->size, 32);
		memcpy((void *)param->tex, tbuf, param->size);
		DCFlushRange((void*)param->tex, param->size);
	}

	if (nnLoadTextureMemoryOne(
			param->pTexInfo,
			param->tex,
			param->minfilter,
			param->magfilter,
			param->globalIndex,
			param->bank,
			param->flag) < 0)
		amSystemLog("[WARN] LoadTexture failed.\n");
#elif _IPHONE
	Uint32 tex = amIPhonePVRTLoadTextureFromPointer( (void *)param->tex, &param->pTexInfo->TexName, NULL );
	if (tex != NULL)
		amIPhoneSetTextureAttribute(param);
	else
		amSystemLog("[ERR] Texture load failed.\n");
#endif

	if (param->buf_delete != NULL)
		amMemFree(param->buf_delete);

	regist->command_id		= 0;
}


/************************************************************************/
/* void _amDrawReleaseTexture(AMS_REGISTLIST *regist)                   */
/*----------------------------------------------------------------------*/
/* [INPUT] regist : 実行する登録コマンド                                */
/* [FUNCTION] テクスチャの解放                                          */
/************************************************************************/
void _amDrawReleaseTexture(AMS_REGISTLIST *regist)
{
	AMS_PARAM_RELEASE_TEXTURE	*param =
			(AMS_PARAM_RELEASE_TEXTURE *)regist->param;
	NNS_TEXINFO		*texinfo;
	Sint32			i, n;

	n		= param->texlist->nTex;
	texinfo	= param->texlist->pTexInfoList;

#if _PC | _XBOX
	for (i = n - 1, texinfo += n - 1; i >= 0; i--, texinfo--)
		nnReleaseTextureOne(texinfo);
#elif _PS3
	for (i = n - 1, texinfo += n - 1; i >= 0; i--, texinfo--) {
		if (texinfo->pTexture) {
			nnTexturePs3Destroy((NNS_TEXTURE_PS3 *)texinfo->pTexture);
			texinfo->pTexture	= NULL;
		}
	}
#elif _WII
	for (i = n - 1, texinfo += n - 1; i >= 0; i--, texinfo--) {
		if (texinfo->Flag & NND_TEXFLAG_ALLOCATE) {
//			nnTexFreeGC(texinfo->pMainMemory);
			MEMFreeToExpHeap(_am_wii_texture_heap, texinfo->pMainMemory);
			texinfo->pMainMemory	= NULL;
		}
		nnReleaseTextureOne(texinfo);
	}
#elif _IPHONE
	for (i = n - 1, texinfo += n - 1; i >= 0; i--, texinfo--) {
		if (texinfo->TexName) {
			glDeleteTextures(1, (const GLuint*)&texinfo->TexName);
		}
	}
#endif

	regist->command_id		= 0;
}


/************************************************************************/
/* void _amDrawVertexBufferObject(AMS_REGISTLIST *regist)               */
/*----------------------------------------------------------------------*/
/* [INPUT] regist : 実行する登録コマンド                                */
/* [FUNCTION] バッファの取得                                            */
/************************************************************************/
void _amDrawVertexBufferObject(AMS_REGISTLIST *regist)
{
	AMS_PARAM_VERTEX_BUFFER_OBJECT	*param =
			(AMS_PARAM_VERTEX_BUFFER_OBJECT *)regist->param;

#if _PC | _XBOX
	nnVertexBufferObjectDXG20(param->obj, param->srcobj, param->vtxflag);
#elif _PS3
	nnBindBufferObject(param->obj, param->srcobj, param->bindflag);
#elif _WII
#elif _IPHONE
	nnBindBufferObjectGL(param->obj, param->srcobj, param->bindflag);
#endif

#if _PC | _XBOX | _PS3
	if (_am_draw_shader_compile) {
		nnRegistObjectStdShaderProfiles(param->obj, param->drawflag);

		_am_draw_regist_flag	|= AMD_REG_FLAG_BUILD_SHADER;
	}
#endif

	regist->command_id		= 0;
}


/************************************************************************/
/* void _amDrawDeleteVertexObject(AMS_REGISTLIST *regist)               */
/*----------------------------------------------------------------------*/
/* [INPUT] regist : 実行する登録コマンド                                */
/* [FUNCTION] バッファの解放                                            */
/************************************************************************/
void _amDrawDeleteVertexObject(AMS_REGISTLIST *regist)
{
#if _PC | _XBOX
	AMS_PARAM_DELETE_VERTEX_OBJECT	*param =
			(AMS_PARAM_DELETE_VERTEX_OBJECT *)regist->param;

	nnDeleteVertexObjectDXG20(param->obj);
#elif _PS3
	AMS_PARAM_DELETE_VERTEX_OBJECT	*param =
			(AMS_PARAM_DELETE_VERTEX_OBJECT *)regist->param;

	nnDeleteBufferObject(param->obj);
#elif _WII
#elif _IPHONE
	AMS_PARAM_DELETE_VERTEX_OBJECT	*param =
			(AMS_PARAM_DELETE_VERTEX_OBJECT *)regist->param;

	nnDeleteBufferObjectGL(param->obj);
#endif

	regist->command_id		= 0;
}


/************************************************************************/
/* void _amDrawLoadShaderObject(AMS_REGISTLIST *regist)                 */
/*----------------------------------------------------------------------*/
/* [INPUT] regist : 実行する登録コマンド                                */
/* [FUNCTION] オブジェクトのシェーダーのロード                          */
/************************************************************************/
void _amDrawLoadShaderObject(AMS_REGISTLIST *regist)
{
#if _PC | _XBOX | _PS3
	if (_am_draw_shader_compile) {
		AMS_PARAM_LOAD_SHADER_OBJECT	*param =
				(AMS_PARAM_LOAD_SHADER_OBJECT *)regist->param;

		Sint32			n;
		NNS_OBJECT		*obj;
		NNF_DRAWOBJ		*drawflag;

		n			= param->flag_num;
		obj			= *param->obj;
		drawflag	= param->drawflag;
		for (; n > 0; n--, drawflag++)
			nnRegistObjectStdShaderProfiles(obj, *drawflag);

		_am_draw_regist_flag	|= AMD_REG_FLAG_BUILD_SHADER;
	}
#endif

	regist->command_id		= 0;
}


/************************************************************************/
/* void _amDrawReleaseStdShader(AMS_REGISTLIST *regist)                 */
/*----------------------------------------------------------------------*/
/* [INPUT] regist : 実行する登録コマンド                                */
/* [FUNCTION] シェーダーの解放                                          */
/************************************************************************/
void _amDrawReleaseStdShader(AMS_REGISTLIST *regist)
{
#if _PC | _XBOX | _PS3
	nnReleaseStdShader();
	nnClearStdShaderProfiles();

	_am_draw_regist_flag	&= ~AMD_REG_FLAG_BUILD_SHADER;
#endif

	regist->command_id		= 0;
}


/************************************************************************/
/* void _amDrawLoadShader(AMS_REGISTLIST *regist)                       */
/*----------------------------------------------------------------------*/
/* [INPUT] regist : 実行する登録コマンド                                */
/* [FUNCTION] シェーダーのロード                                        */
/************************************************************************/
void _amDrawLoadShader(AMS_REGISTLIST *regist)
{
#if _PC | _XBOX
	AMS_PARAM_LOAD_SHADER		*param =
			(AMS_PARAM_LOAD_SHADER *)regist->param;
	AMS_AMB_HEADER		*amb_header =
			(AMS_AMB_HEADER *)param->image;

	// 必要なときに使えるように設定しておく
	amShaderSetFileStd(amb_header);
	amShaderBuildStd(amb_header);
#elif _PS3
	AMS_PARAM_LOAD_SHADER		*param =
			(AMS_PARAM_LOAD_SHADER *)regist->param;

	// その場でロードする
	if (!nnLoadStdShader(param->image))
		NNM_ASSERT(0, "[ERR] shader load failed.\n");
#endif

	regist->command_id		= 0;
}


/************************************************************************/
/* void _amDrawBuildShader(AMS_REGISTLIST *regist)                      */
/*----------------------------------------------------------------------*/
/* [INPUT] regist : 実行する登録コマンド                                */
/* [FUNCTION] シェーダーのビルド                                        */
/************************************************************************/
void _amDrawBuildShader(AMS_REGISTLIST *regist)
{
#if _PC | _XBOX
	LPDIRECT3DDEVICE9	d3ddev;
	HRESULT		hr;
	ID3DXBuffer	*shader_code;

	AMS_PARAM_BUILD_SHADER		*param =
			(AMS_PARAM_BUILD_SHADER *)regist->param;

#if _PC
	d3ddev	= amWinDxGetDirect3DDevice();
#elif _XBOX
	d3ddev	= amXboxDxGetDirect3DDevice();
#endif

	// 頂点シェーダーの作成
	hr	= D3DXCompileShader(param->vs_code, param->vs_size,
			NULL, NULL, "main_VS", "vs_3_0", 0,
			&shader_code, NULL, (LPD3DXCONSTANTTABLE *)param->vs_param);
	hr	= d3ddev->CreateVertexShader(
			(DWORD*)shader_code->GetBufferPointer(),
			(IDirect3DVertexShader9 **)param->vs_shader);
	shader_code->Release();

	// ピクセルシェーダーの作成
	hr	= D3DXCompileShader(param->ps_code, param->ps_size,
			NULL, NULL, "main_PS", "ps_3_0", 0,
			// D3DXGetPixelShaderProfile(nng_pd3dDevice), D3DXSHADER_MICROCODE_BACKEND_OLD,
			&shader_code, NULL, (LPD3DXCONSTANTTABLE *)param->ps_param);
	hr	= d3ddev->CreatePixelShader(
			(DWORD*)shader_code->GetBufferPointer(),
			(IDirect3DPixelShader9 **)param->ps_shader);
	shader_code->Release();
#elif _PS3
#if 0		// なぜか描画スレッドだとコンパイルできない
	char			*binary;

	const char		*profile_VS = "sce_vp_rsx";
	const char		*profile_PS = "sce_fp_rsx";

	// 頂点シェーダーの作成
	binary		= NULL;
	compile_program_from_string(param->vs_code,
			profile_VS, "main_VS", 0, &binary);
	*param->vs_shader	= nnVertexShaderPs3Create(binary,
			(NNS_SHADER_PARAM_PS3 *)param->vs_param, param->vs_constants);
	free_compiled_program(binary);

	// ピクセルシェーダーの作成
	binary		= NULL;
	compile_program_from_string(param->ps_code,
			profile_PS, "main_PS", 0, &binary);
	*param->ps_shader	= nnPixelShaderPs3Create(binary,
			(NNS_SHADER_PARAM_PS3 *)param->ps_param, param->ps_constants);
	free_compiled_program(binary);
#endif
#elif _WII
#endif

	regist->command_id		= 0;
}


/************************************************************************/
/* void _amDrawCreateShader(AMS_REGISTLIST *regist)                     */
/*----------------------------------------------------------------------*/
/* [INPUT] regist : 実行する登録コマンド                                */
/* [FUNCTION] シェーダーの作成                                          */
/************************************************************************/
void _amDrawCreateShader(AMS_REGISTLIST *regist)
{
#if _PC | _XBOX
	LPDIRECT3DDEVICE9	d3ddev;

	AMS_PARAM_CREATE_SHADER		*param =
			(AMS_PARAM_CREATE_SHADER *)regist->param;

#if _PC
	d3ddev	= amWinDxGetDirect3DDevice();
#elif _XBOX
	d3ddev	= amXboxDxGetDirect3DDevice();
#endif

	// 頂点シェーダーの作成
	d3ddev->CreateVertexShader(
			(DWORD*)param->vs_image,
			(IDirect3DVertexShader9 **)param->vs_shader);

	// ピクセルシェーダーの作成
	d3ddev->CreatePixelShader(
			(DWORD*)param->ps_image,
			(IDirect3DPixelShader9 **)param->ps_shader);
#elif _PS3
	AMS_PARAM_CREATE_SHADER		*param =
			(AMS_PARAM_CREATE_SHADER *)regist->param;

	// 頂点シェーダーの作成
	*param->vs_shader	= nnVertexShaderPs3Create(
			(char *)param->vs_image,
			(NNS_SHADER_PARAM_PS3 *)param->vs_param, param->vs_constants);

	// ピクセルシェーダーの作成
	*param->ps_shader	= nnPixelShaderPs3Create(
			(char *)param->ps_image,
			(NNS_SHADER_PARAM_PS3 *)param->ps_param, param->ps_constants);
#elif _WII
#endif

	regist->command_id		= 0;
}


/************************************************************************/
/* void _amDrawReleaseShader(AMS_REGISTLIST *regist)                    */
/*----------------------------------------------------------------------*/
/* [INPUT] regist : 実行する登録コマンド                                */
/* [FUNCTION] シェーダーの解放                                          */
/************************************************************************/
void _amDrawReleaseShader(AMS_REGISTLIST *regist)
{
	AMS_PARAM_RELEASE_SHADER		*param =
			(AMS_PARAM_RELEASE_SHADER *)regist->param;

#if _PC | _XBOX
	if (param->vs_shader != NULL)
		((IDirect3DVertexShader9 *)param->vs_shader)->Release();
	if (param->ps_shader != NULL)
		((IDirect3DPixelShader9  *)param->ps_shader)->Release();
#elif _PS3
	if (param->vs_shader != NULL)
		nnVertexShaderPs3Destroy((NNS_VERTEXSHADER_PS3 *)param->vs_shader);
	if (param->ps_shader != NULL)
		nnPixelShaderPs3Destroy((NNS_PIXELSHADER_PS3 *)param->ps_shader);
#elif _WII
#endif

	regist->command_id		= 0;

}


/************************************************************************/
/* void _amDrawLoadTextureImage(AMS_REGISTLIST *regist)                 */
/*----------------------------------------------------------------------*/
/* [INPUT] regist : 実行する登録コマンド                                */
/* [FUNCTION] テクスチャの登録                                          */
/************************************************************************/
void _amDrawLoadTextureImage(AMS_REGISTLIST *regist)
{
	AMS_PARAM_LOAD_TEXTURE_IMAGE	*param =
			(AMS_PARAM_LOAD_TEXTURE_IMAGE *)regist->param;

#if _PC | _XBOX
	LPDIRECT3DDEVICE9	d3ddev;
	HRESULT		hr;

#if _PC
	d3ddev	= amWinDxGetDirect3DDevice();
#elif _XBOX
	d3ddev	= amXboxDxGetDirect3DDevice();
#endif

	// テクスチャの生成
	hr	= D3DXCreateTextureFromFileInMemoryEx(d3ddev,
			param->image, param->size,
			D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
#if _PC
			D3DUSAGE_DYNAMIC,
#elif _XBOX
			0,
#endif
			D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
			D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0, NULL, NULL,
			(LPDIRECT3DTEXTURE9 *)param->texture);
#elif _PS3
	// テクスチャの生成
	*param->texture	= amPs3LoadDDSTexture2D(param->image);
	nnTexturePs3SetFilter((NNS_TEXTURE_PS3 *)*param->texture,
			NNE_FILTER_LINEAR, NNE_FILTER_LINEAR, NNE_FILTER_NONE);
	nnTexturePs3SetMaxAnisotropy((NNS_TEXTURE_PS3 *)*param->texture, 1);
#elif _WII
	NVS_GVROBJ		*gvrobj;
	void			*image;

	gvrobj		= param->gvrobj;
	nvGetGVRHeader(gvrobj, param->image);

	image		= (void *)((Sint8 *)param->image + 0x20);
	DCFlushRange(param->image, param->size);

	GXInitTexObj((GXTexObj *)param->texture, image,
			gvrobj->gvrh.width, gvrobj->gvrh.height,
			(GXTexFmt)gvrobj->gvrh.type,
			(GXTexWrapMode)param->u_wrap, (GXTexWrapMode)param->v_wrap, GX_FALSE);
	GXInitTexObjFilter((GXTexObj *)param->texture,
			(GXTexFilter)param->minfilter, (GXTexFilter)param->magfilter);
#endif

	regist->command_id		= 0;
}


/************************************************************************/
/* void _amDrawReleaseTextureImage(AMS_REGISTLIST *regist)              */
/*----------------------------------------------------------------------*/
/* [INPUT] regist : 実行する登録コマンド                                */
/* [FUNCTION] テクスチャの解放                                          */
/************************************************************************/
void _amDrawReleaseTextureImage(AMS_REGISTLIST *regist)
{
	AMS_PARAM_RELEASE_TEXTURE_IMAGE	*param =
			(AMS_PARAM_RELEASE_TEXTURE_IMAGE *)regist->param;

#if _PC | _XBOX
	if (param->texture != NULL)
		((LPDIRECT3DTEXTURE9)param->texture)->Release();
#elif _PS3
	if (param->texture != NULL)
		nnTexturePs3Destroy((NNS_TEXTURE_PS3 *)param->texture);
#elif _WII
#endif

	regist->command_id		= 0;
}


#if _WII
/************************************************************************/
/* void amDrawToonMaterial(NNS_DRAWCALLBACK_VAL *val)                   */
/*----------------------------------------------------------------------*/
/* [INPUT] val   : ドローコールバック変数へのポインタ                   */
/* [FUNCTION] Wiiトゥーン用マテリアルコールバック                       */
/*      メインスレッドの描画コール(amMotionDrawなど)に設定すると        */
/*      トゥーン描画になります                                          */
/*      _am_draw_toonDir にライト方向を設定しておいてください           */
/************************************************************************/
NNE_BOOL amDrawToonMaterial(NNS_DRAWCALLBACK_VAL *val)
{
	NNE_BOOL	exit = NNE_FALSE;
	NNE_BOOL	ret;
	void		*mat;

	NNF_MATFLAG	*mat_flag,	backup_mat_flag;
	NNS_RGBA	*diffuse,	backup_diffuse;
	Uint32		*user_data, backup_user_data;

	GXTevStageID	tevstage_id;
	GXTexCoordID	texcoord_id;
	GXTexMapID		texmap_id;
//	Mtx			tex_mtx;

	GXLightObj	toon_light;
	Vec			toon_light_dir;
	GXColor		toon_light_color = { 255, 255, 255, 255};
	GXColor		ambient_color    = {   0,   0,   0, 255};
	GXColor		material_color   = { 255, 255, 255, 255}; 
	GXColor		const_color;

	mat			= val->pMaterial->pMaterial;
	mat_flag	= &((NNS_MATERIAL_NOTEXTURE *)mat)->fMatFlag;
	diffuse		= &((NNS_MATERIAL_NOTEXTURE *)mat)->Diffuse;
	user_data	= &((NNS_MATERIAL_NOTEXTURE *)mat)->User;

	// ライティングOFFならばトゥーンにしないで普通に描く
	if ((*mat_flag) & NND_MATFLAG_DISABLE_LIGHTING)
		return	nnPutMaterialCore(val);

	// マテリアル設定の保存
	backup_mat_flag		= *mat_flag;
	backup_diffuse		= *diffuse;
	backup_user_data	= *user_data;

	// 頂点カラー、ライティングOFF
	*mat_flag		&= ~NND_MATFLAG_VERTEX_COLOR;
	*mat_flag		|= NND_MATFLAG_DISABLE_LIGHTING;

	// マテリアル変更フラグON
	val->bModified	= NNE_TRUE;

	// 通常のマテリアル処理
	ret		= nnPutMaterialCore(val);

	// NNで使用しているステージ数などを取得する
	tevstage_id	= (GXTevStageID)nnGetCurrentMaterialTevStageNumGC();
	texcoord_id	= (GXTexCoordID)nnGetCurrentMaterialTexCoordNumGC();
	texmap_id	= (GXTexMapID)nnGetCurrentMaterialTexMapNumGC();

	// トゥーンのライト設定
	MTXMultVecSR(*(Mtx *)nnGetCurrentViewMatrixGC(),
			(Vec *)&_am_draw_toonDir, &toon_light_dir);
	VECScale(&toon_light_dir, &toon_light_dir, -10000000.0f);

	GXInitLightPos(&toon_light,
			toon_light_dir.x, toon_light_dir.y, toon_light_dir.z);
	GXInitLightColor(&toon_light, toon_light_color);
	GXLoadLightObjImm(&toon_light, GX_LIGHT7);
	GXSetNumChans(2);
	GXSetChanAmbColor(GX_COLOR1, ambient_color);
	GXSetChanMatColor(GX_COLOR1, material_color);
	GXSetChanCtrl(GX_COLOR1, GX_ENABLE, GX_SRC_REG, GX_SRC_REG,
			GX_LIGHT7, GX_DF_CLAMP, GX_AF_NONE);
	GXSetChanCtrl(GX_ALPHA1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG,
			0, GX_DF_NONE, GX_AF_NONE);

#if 0
	// 外部階調テクスチャ設定
	GXLoadTexObj(&_test_toon_texobj, (GXTexMapID)texmap_id);
#endif

	// テクスチャ座標
	GXSetNumTexGens((Uint8)(texcoord_id + 2));
	GXSetTexCoordGen((GXTexCoordID)(texcoord_id + 0),
			GX_TG_SRTG, GX_TG_COLOR0, GX_IDENTITY);
	GXSetTexCoordGen((GXTexCoordID)(texcoord_id + 1),
			GX_TG_SRTG, GX_TG_COLOR1, GX_IDENTITY);

	// カラー制御
	const_color.r	= 0xff;
	const_color.g	= 0xff;
	const_color.b	= 0xff;
	const_color.a	= (Uint8)(_am_draw_state.alpha.alpha * 255.0f);
	GXSetTevKColor(GX_KCOLOR0, const_color);

	// Tev設定
#if 1
	// 暗部階調あり

	// 暗部階調用ライト設定
	GXInitLightPos(&toon_light,
			-toon_light_dir.x, -toon_light_dir.y, -toon_light_dir.z);
	GXLoadLightObjImm(&toon_light, GX_LIGHT6);
	GXSetChanAmbColor(GX_COLOR0, ambient_color);
	GXSetChanMatColor(GX_COLOR0, material_color);
	GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG,
			GX_LIGHT6, GX_DF_CLAMP, GX_AF_NONE);
	GXSetChanCtrl(GX_ALPHA0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG,
			0, GX_DF_NONE, GX_AF_NONE);

	GXSetNumTevStages(3);

	// 正方向ライト
	GXSetTevOrder(GX_TEVSTAGE0,
			(GXTexCoordID)(texcoord_id + 1), GX_TEXMAP1, GX_COLOR_NULL);
	GXSetTevColorOp(GX_TEVSTAGE0,
			GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVREG0);
	GXSetTevColorIn(GX_TEVSTAGE0,
			GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
	GXSetTevAlphaOp(GX_TEVSTAGE0,
			GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0,
			GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);

	// 逆方向ライト
	GXSetTevOrder(GX_TEVSTAGE1,
			(GXTexCoordID)(texcoord_id + 0), GX_TEXMAP2, GX_COLOR_NULL);
	GXSetTevColorOp(GX_TEVSTAGE1,
			GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVREG0);
	GXSetTevColorIn(GX_TEVSTAGE1,
			GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
	GXSetTevAlphaOp(GX_TEVSTAGE1,
			GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE1,
			GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);

#if 0
	// 乗算トゥーン
	GXSetTevOrder(GX_TEVSTAGE2,
			GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorOp(GX_TEVSTAGE2,
			GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	GXSetTevColorIn(GX_TEVSTAGE2,
			GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
#else
	// 減算トゥーン
	GXSetTevOrder(GX_TEVSTAGE2,
			GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorOp(GX_TEVSTAGE2,
			GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	GXSetTevColorIn(GX_TEVSTAGE2,
			GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
#endif
	switch (_am_draw_state.alpha.mode) {
		case	NNE_MATCTRLMODE_NONE:
			GXSetTevAlphaOp(GX_TEVSTAGE2,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
			GXSetTevAlphaIn(GX_TEVSTAGE2,
					GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
			break;
		case	NNE_MATCTRLMODE_REPLACE:
			GXSetTevKAlphaSel(GX_TEVSTAGE2, GX_TEV_KASEL_K0_A);
			GXSetTevAlphaOp(GX_TEVSTAGE2,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
			GXSetTevAlphaIn(GX_TEVSTAGE2,
					GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
			break;
		case	NNE_MATCTRLMODE_ADD:
			GXSetTevKAlphaSel(GX_TEVSTAGE2, GX_TEV_KASEL_K0_A);
			GXSetTevAlphaOp(GX_TEVSTAGE2,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
			GXSetTevAlphaIn(GX_TEVSTAGE2,
					GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
			break;
		case	NNE_MATCTRLMODE_MODULATE:
			GXSetTevKAlphaSel(GX_TEVSTAGE2, GX_TEV_KASEL_K0_A);
			GXSetTevAlphaOp(GX_TEVSTAGE2,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
			GXSetTevAlphaIn(GX_TEVSTAGE2,
					GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, GX_CA_ZERO);
			break;
	}
#elif 1
	// 暗部階調なし
	GXSetNumTevStages((Uint8)(tevstage_id + 1));

#if 0
	// 乗算トゥーン
	GXSetTevOrder((GXTevStageID)tevstage_id,
			(GXTexCoordID)(texcoord_id + 1), (GXTexMapID)(texmap_id - 1), GX_COLOR_NULL);
	GXSetTevColorOp((GXTevStageID)tevstage_id,
			GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	GXSetTevColorIn((GXTevStageID)tevstage_id,
			GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
#else
	// 減算トゥーン
	GXSetTevOrder((GXTevStageID)tevstage_id,
			(GXTexCoordID)(texcoord_id + 1), (GXTexMapID)(texmap_id - 1), GX_COLOR_NULL);
	GXSetTevColorOp((GXTevStageID)tevstage_id,
			GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	GXSetTevColorIn((GXTevStageID)tevstage_id,
			GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
#endif
	switch (_am_draw_state.alpha.mode) {
		case	NNE_MATCTRLMODE_NONE:
			GXSetTevAlphaOp((GXTevStageID)tevstage_id,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
			GXSetTevAlphaIn((GXTevStageID)tevstage_id,
					GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_PREV);
			break;
		case	NNE_MATCTRLMODE_REPLACE:
			GXSetTevKAlphaSel((GXTevStageID)tevstage_id, GX_TEV_KASEL_K0_A);
			GXSetTevAlphaOp((GXTevStageID)tevstage_id,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
			GXSetTevAlphaIn((GXTevStageID)tevstage_id,
					GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
			break;
		case	NNE_MATCTRLMODE_ADD:
			GXSetTevKAlphaSel((GXTevStageID)tevstage_id, GX_TEV_KASEL_K0_A);
			GXSetTevAlphaOp((GXTevStageID)tevstage_id,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
			GXSetTevAlphaIn((GXTevStageID)tevstage_id,
					GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO, GX_CA_PREV);
			break;
		case	NNE_MATCTRLMODE_MODULATE:
			GXSetTevKAlphaSel((GXTevStageID)tevstage_id, GX_TEV_KASEL_K0_A);
			GXSetTevAlphaOp((GXTevStageID)tevstage_id,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
			GXSetTevAlphaIn((GXTevStageID)tevstage_id,
					GX_CA_ZERO, GX_CA_PREV, GX_CA_KONST, GX_CA_ZERO);
			break;
	}
#endif

	// マテリアルの復帰
	*diffuse	= backup_diffuse;
	*mat_flag	= backup_mat_flag;

	return	ret;
}

/************************************************************************/
/* void amDrawWaterFallMaterial(NNS_DRAWCALLBACK_VAL *val)              */
/*----------------------------------------------------------------------*/
/* [INPUT] val   : ドローコールバック変数へのポインタ                   */
/* [FUNCTION] Wii滝用マテリアルコールバック                             */
/*      メインスレッドの描画コール(amMotionMaterialDraw)に設定すると    */
/*      滝描画になります                                                */
/************************************************************************/
NNE_BOOL amDrawWaterFallMaterial(NNS_DRAWCALLBACK_VAL *val)
{
	NNE_BOOL	exit = NNE_FALSE;
	NNE_BOOL	ret;
	NNS_MATERIAL_NOTEXTURE	*mat;

//	NNF_MATFLAG	*mat_flag,	backup_mat_flag;
//	NNS_RGBA	*diffuse,	backup_diffuse;
//	Uint32		*user_data, backup_user_data;

	GXTevStageID	tevstage_id;
	GXTexCoordID	texcoord_id;
	GXTexMapID		texmap_id;
//	NNS_MATRIX		mtx;
	NNS_MATRIX44	tex_mtx;

	mat		= (NNS_MATERIAL_NOTEXTURE *)(val->pMaterial->pMaterial);

	// 通常のマテリアル処理
	ret		= nnPutMaterialCore(val);

#if 1
	switch (mat->User) {
		case	0:
			break;

		case	1: // 滝
		case	3: // ガラス
#endif
{
			// NNで使用しているステージ数などを取得する
			tevstage_id	= (GXTevStageID)nnGetCurrentMaterialTevStageNumGC();
			texcoord_id	= (GXTexCoordID)nnGetCurrentMaterialTexCoordNumGC();
			texmap_id	= (GXTexMapID)nnGetCurrentMaterialTexMapNumGC();

			// テクスチャの設定
			amRenderSetTexture((NNE_TEXSLOT)texmap_id, &_am_draw_render_work, 0);

			// テクスチャ座標
			GXSetNumTexGens((Uint8)texcoord_id + 1);
			GXSetTexCoordGen(texcoord_id, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX9);
			nnMultiplyProjectionMatrix(&tex_mtx,
					&_am_draw_fall_projmtx, nnGetCurrentNodeMatrixGC());
			nnMultiplyScalingMatrix44(&tex_mtx, 0.5f, -0.5f, 1.0f, &tex_mtx);
			nnMultiplyTranslationMatrix44(&tex_mtx, 0.5f, 0.5f, 1.0f, &tex_mtx);
			NNM_MTX(tex_mtx, 2, 0)	= NNM_MTX(tex_mtx, 3, 0);
			NNM_MTX(tex_mtx, 2, 1)	= NNM_MTX(tex_mtx, 3, 1);
			NNM_MTX(tex_mtx, 2, 2)	= NNM_MTX(tex_mtx, 3, 2);
			NNM_MTX(tex_mtx, 2, 3)	= NNM_MTX(tex_mtx, 3, 3);
			GXLoadTexMtxImm((MtxPtr)tex_mtx, GX_TEXMTX9, GX_MTX3x4);

			// インダイレクトテクスチャ
			GXSetNumIndStages(1);
			GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD0, GX_TEXMAP0);
			GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
			float	indMtx[2][3] = {
				{ 0.5f / 15.0f, 0.0f, 0.0f},
				{ 0.0f, 0.5f, 0.0f},
			};
			if (mat->User == 3)
				indMtx[0][0]	= 0.5f;
			GXSetIndTexMtx(GX_ITM_0, indMtx, -5+2+1);

			GXSetNumTevStages(5);

			// TevStage0 : レンダリングテクスチャ(インダイレクト)
			GXSetTevKAlphaSel(GX_TEVSTAGE2, GX_TEV_KASEL_1);
			GXSetTevIndWarp(GX_TEVSTAGE0,
					GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
			GXSetTevOrder(GX_TEVSTAGE0,
					texcoord_id, texmap_id, GX_COLOR_NULL);
			GXSetTevColorOp(GX_TEVSTAGE0,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevColorIn(GX_TEVSTAGE0,
					GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
			GXSetTevAlphaOp(GX_TEVSTAGE0,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevAlphaIn(GX_TEVSTAGE0,
					GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);

			// TevStage1 : αテクスチャ
			GXSetTevOrder(GX_TEVSTAGE1,
					GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
			GXSetTevColorOp(GX_TEVSTAGE1,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevColorIn(GX_TEVSTAGE1,
					GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
			GXSetTevAlphaOp(GX_TEVSTAGE1,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevAlphaIn(GX_TEVSTAGE1,
					GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);

			// TevStage2 : 水カラーテクスチャ
			GXSetTevKAlphaSel(GX_TEVSTAGE2, GX_TEV_KASEL_1);
			GXSetTevOrder(GX_TEVSTAGE2,
					GX_TEXCOORD1, GX_TEXMAP3, GX_COLOR_NULL);
			GXSetTevColorOp(GX_TEVSTAGE2,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevColorIn(GX_TEVSTAGE2,
//					GX_CC_CPREV, GX_CC_TEXC, GX_CC_APREV, GX_CC_ZERO);
					GX_CC_ZERO, GX_CC_TEXC, GX_CC_APREV, GX_CC_CPREV);
			GXSetTevAlphaOp(GX_TEVSTAGE2,
					GX_TEV_COMP_A8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
			GXSetTevAlphaIn(GX_TEVSTAGE2,
					GX_CA_APREV, GX_CA_ZERO, GX_CA_KONST, GX_CA_ZERO);

			// TevStage3 : αテクスチャ
			GXSetTevOrder(GX_TEVSTAGE3,
					GX_TEXCOORD0, GX_TEXMAP2, GX_COLOR_NULL);
			GXSetTevColorOp(GX_TEVSTAGE3,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevColorIn(GX_TEVSTAGE3,
					GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
			GXSetTevAlphaOp(GX_TEVSTAGE3,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevAlphaIn(GX_TEVSTAGE3,
					GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);

			// TevStage4 : 水カラーテクスチャ
			GXSetTevOrder(GX_TEVSTAGE4,
					GX_TEXCOORD0, GX_TEXMAP3, GX_COLOR_NULL);
			GXSetTevColorOp(GX_TEVSTAGE4,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevColorIn(GX_TEVSTAGE4,
//					GX_CC_CPREV, GX_CC_TEXC, GX_CC_APREV, GX_CC_ZERO);
					GX_CC_ZERO, GX_CC_TEXC, GX_CC_APREV, GX_CC_CPREV);
			GXSetTevAlphaOp(GX_TEVSTAGE4,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevAlphaIn(GX_TEVSTAGE4,
					GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
}
			break;
	}

	return	ret;
	
}

/************************************************************************/
/* void amDrawSeaMaterial(NNS_DRAWCALLBACK_VAL *val)                    */
/*----------------------------------------------------------------------*/
/* [INPUT] val   : ドローコールバック変数へのポインタ                   */
/* [FUNCTION] Wii海用マテリアルコールバック                             */
/*      メインスレッドの描画コール(amMotionMaterialDraw)に設定すると    */
/*      海描画になります                                                */
/************************************************************************/
NNE_BOOL amDrawSeaMaterial(NNS_DRAWCALLBACK_VAL *val)
{
	NNE_BOOL	exit = NNE_FALSE;
	NNE_BOOL	ret;
	NNS_MATERIAL_NOTEXTURE	*mat;

//	NNF_MATFLAG	*mat_flag,	backup_mat_flag;
//	NNS_RGBA	*diffuse,	backup_diffuse;
//	Uint32		*user_data, backup_user_data;

	GXTevStageID	tevstage_id;
	GXTexCoordID	texcoord_id;
	GXTexMapID		texmap_id;
//	NNS_MATRIX		mtx;
	NNS_MATRIX44	tex_mtx;

	mat		= (NNS_MATERIAL_NOTEXTURE *)(val->pMaterial->pMaterial);

	// 通常のマテリアル処理
	ret		= nnPutMaterialCore(val);

#if 0
	switch (mat->User) {
		case	0:
			break;

		case	1:
#endif
{
			// NNで使用しているステージ数などを取得する
			tevstage_id	= (GXTevStageID)nnGetCurrentMaterialTevStageNumGC();
			texcoord_id	= (GXTexCoordID)nnGetCurrentMaterialTexCoordNumGC();
			texmap_id	= (GXTexMapID)nnGetCurrentMaterialTexMapNumGC();

			// テクスチャの設定
			amRenderSetTexture((NNE_TEXSLOT)texmap_id, &_am_draw_render_work, 0);

			// テクスチャ座標
			GXSetNumTexGens((Uint8)texcoord_id + 1);
			GXSetTexCoordGen(texcoord_id, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX9);
			nnMultiplyProjectionMatrix(&tex_mtx,
					&_am_draw_fall_projmtx, nnGetCurrentNodeMatrixGC());
			nnMultiplyScalingMatrix44(&tex_mtx, 0.5f, -0.5f, 1.0f, &tex_mtx);
			nnMultiplyTranslationMatrix44(&tex_mtx, 0.5f, 0.5f, 1.0f, &tex_mtx);
			NNM_MTX(tex_mtx, 2, 0)	= NNM_MTX(tex_mtx, 3, 0);
			NNM_MTX(tex_mtx, 2, 1)	= NNM_MTX(tex_mtx, 3, 1);
			NNM_MTX(tex_mtx, 2, 2)	= NNM_MTX(tex_mtx, 3, 2);
			NNM_MTX(tex_mtx, 2, 3)	= NNM_MTX(tex_mtx, 3, 3);
			GXLoadTexMtxImm((MtxPtr)tex_mtx, GX_TEXMTX9, GX_MTX3x4);

			// インダイレクトテクスチャ
			GXSetNumIndStages(1);
			GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD0, GX_TEXMAP0);
			GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
			float	indMtx[2][3] = {
				{ 0.5f, 0.0f, 0.0f},
				{ 0.0f, 0.5f, 0.0f},
			};
			GXSetIndTexMtx(GX_ITM_0, indMtx, -4+1); // -になればなるほど触れ幅が小さくなる

			GXSetNumTevStages(3);

			// TevStage0 : レンダリングテクスチャ(インダイレクト)
			GXSetTevKAlphaSel(GX_TEVSTAGE2, GX_TEV_KASEL_1);
			GXSetTevIndWarp(GX_TEVSTAGE0,
					GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
			GXSetTevOrder(GX_TEVSTAGE0,
					texcoord_id, texmap_id, GX_COLOR_NULL);
			GXSetTevColorOp(GX_TEVSTAGE0,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevColorIn(GX_TEVSTAGE0,
					GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
			GXSetTevAlphaOp(GX_TEVSTAGE0,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevAlphaIn(GX_TEVSTAGE0,
					GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);

			// TevStage1 : αテクスチャ
			GXSetTevOrder(GX_TEVSTAGE1,
					GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
			GXSetTevColorOp(GX_TEVSTAGE1,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevColorIn(GX_TEVSTAGE1,
					GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
			GXSetTevAlphaOp(GX_TEVSTAGE1,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevAlphaIn(GX_TEVSTAGE1,
					GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);

			// TevStage2 : 水カラーテクスチャ
			GXSetTevKAlphaSel(GX_TEVSTAGE2, GX_TEV_KASEL_1);
			GXSetTevOrder(GX_TEVSTAGE2,
					GX_TEXCOORD1, GX_TEXMAP2, GX_COLOR_NULL);
			GXSetTevColorOp(GX_TEVSTAGE2,
					GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevColorIn(GX_TEVSTAGE2,
					GX_CC_CPREV, GX_CC_TEXC, GX_CC_APREV, GX_CC_ZERO);
//					GX_CC_ZERO, GX_CC_TEXC, GX_CC_APREV, GX_CC_CPREV);
			GXSetTevAlphaOp(GX_TEVSTAGE2,
					GX_TEV_COMP_A8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GXSetTevAlphaIn(GX_TEVSTAGE2,
					GX_CA_APREV, GX_CA_ZERO, GX_CA_KONST, GX_CA_ZERO);
}
#if 0
			break;
	}
#endif

	return	ret;
	
}

#else
#define	amDrawToonMaterial			((NNS_MATERIALCALLBACK_FUNC)NULL)
#define	amDrawWaterFallMaterial		((NNS_MATERIALCALLBACK_FUNC)NULL)
#define	amDrawSeaMaterial			((NNS_MATERIALCALLBACK_FUNC)NULL)
#endif


