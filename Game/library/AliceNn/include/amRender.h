/*****************************************************************************/
/*      amRender.h                  Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* レンダリングターゲット管理ライブラリヘッダ                                */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090417-       0.01   first version                                        */
/*****************************************************************************/

#ifndef _AM_RENDER_H
#define _AM_RENDER_H

/*--- Include Files (Pre Definitions) ---------------------------------------*/

#if _XBOX
#include "Xbox360/amXbox.h"
#include "Xbox360/amXboxDx.h"
#elif _PS3
#include <cell/gcm.h>
#include <nnldeviceps3.h>
#endif


/*--- Definitions -----------------------------------------------------------*/

#define AMD_RENDER_SURFACE_MAX		(4)			// 最大サーフェイス数

#define AMD_RENDER_FLAG_RESOLVE_COLOR0	(0x0001)
#define AMD_RENDER_FLAG_RESOLVE_COLOR1	(0x0002)
#define AMD_RENDER_FLAG_RESOLVE_COLOR2	(0x0004)
#define AMD_RENDER_FLAG_RESOLVE_COLOR3	(0x0008)
#define AMD_RENDER_FLAG_RESOLVE_DEPTH	(0x0010)
#define AMD_RENDER_FLAG_USE_DEPTH		(0x0020)
#define AMD_RENDER_FLAG_TILED			(0x4000)
#define AMD_RENDER_FLAG_DELETE			(0x8000)

#if _PC | _XBOX
#define AMD_RENDER_CLEAR_COLOR			D3DCLEAR_TARGET
#define AMD_RENDER_CLEAR_DEPTH			D3DCLEAR_ZBUFFER
#define AMD_RENDER_CLEAR_STENCIL		D3DCLEAR_STENCIL
#define AMD_RENDER_CLEAR_ALL			\
	(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL)
#elif _PS3
#define AMD_RENDER_CLEAR_COLOR			\
	(CELL_GCM_CLEAR_R | CELL_GCM_CLEAR_G | CELL_GCM_CLEAR_B | CELL_GCM_CLEAR_A)
#define AMD_RENDER_CLEAR_DEPTH			CELL_GCM_CLEAR_Z
#define AMD_RENDER_CLEAR_STENCIL		CELL_GCM_CLEAR_S
#define AMD_RENDER_CLEAR_ALL			\
	(AMD_RENDER_CLEAR_COLOR | AMD_RENDER_CLEAR_DEPTH | AMD_RENDER_CLEAR_STENCIL)
#else
#define AMD_RENDER_CLEAR_COLOR			(0x0001)
#define AMD_RENDER_CLEAR_DEPTH			(0x0001)
#define AMD_RENDER_CLEAR_STENCIL		(0x0001)
#define AMD_RENDER_CLEAR_ALL			(0x0001)
#endif

// レンダリングターゲット
typedef struct {
	Uint32		flag;					// フラグ
	Sint32		surface_num;			// サーフェイス数
	Sint32		width;					// サイズ
	Sint32		height;
	float		aspect;					// イメージアスペクト比
#if _PC | _XBOX
	LPDIRECT3DSURFACE9		surface_color[AMD_RENDER_SURFACE_MAX];
	LPDIRECT3DSURFACE9		surface_depth;
	LPDIRECT3DTEXTURE9		texture_color[AMD_RENDER_SURFACE_MAX];
	LPDIRECT3DTEXTURE9		texture_depth;
#if _XBOX
	AMS_XBOXDX_TILE_INFO	*tiling;
#endif
#elif _PS3
	NNS_RENDERTARGET_PS3	*surface;
	NNS_TEXTURE_PS3			*texture_color[AMD_RENDER_SURFACE_MAX];
	NNS_DEPTHBUFFER_PS3		*texture_depth;
#elif _WII
	void					*texture_color;
	GXTexFmt				format_color;
	GXZFmt16				format_depth;
	GXTexObj				texobj;
#endif
} AMS_RENDER_TARGET;

// レンダリングターゲット管理
typedef struct {
	AMS_RENDER_TARGET	*targetp;
	AMS_RENDER_TARGET	target_now;		// 現在のターゲット
} AMS_RENDER_MANAGER;


/*--- Macros ----------------------------------------------------------------*/

/*--- Include Files (Post Definitions) --------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/

extern AMS_RENDER_MANAGER		_am_render_manager;
extern AMS_RENDER_TARGET		_am_render_default;


/*--- External Functions ----------------------------------------------------*/

/*****************************************************************************/
/* void amRenderInit(void)                                                   */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  レンダリングターゲット管理の初期化                            */
/*****************************************************************************/
void amRenderInit(void);

/*****************************************************************************/
/* AMS_RENDER_TARGET *amRenderCreate(AMS_RENDER_TARGET *target,              */
/*                 Sint32 width, Sint32 height,                              */
/*                 Sint32 surface_num, Sint32 format, Sint32 depth,          */
/*                 Uint32 flag)                                              */
/*---------------------------------------------------------------------------*/
/* [INPUT]  target      : レンダリングターゲット構造体(NULLなら内部で確保)   */
/*          width       : 横サイズ                                           */
/*          height      : 縦サイズ                                           */
/*          surface_num : サーフェイス数                                     */
/*          format      : サーフェイスフォーマット(配列)                     */
/*          depth       : デプス・ステンシルフォーマット                     */
/*          flag        : フラグ                                             */
/* [FUNCTION]  レンダリングターゲットの作成                                  */
/*****************************************************************************/
AMS_RENDER_TARGET *amRenderCreate(AMS_RENDER_TARGET *target,
		Sint32 width, Sint32 height, Sint32 surface_num, Sint32 *format,
		Sint32 depth, Uint32 flag);

#define AMD_RENDER_DEPTH_DEFAULT		(-1)	// デフォルトのデプスバッファ


/*****************************************************************************/
/* void amRenderDelete(AMS_RENDER_TARGET *target)                            */
/*---------------------------------------------------------------------------*/
/* [INPUT]  target      : レンダリングターゲット構造体                       */
/* [FUNCTION]  レンダリングターゲットの削除                                  */
/*****************************************************************************/
void amRenderDelete(AMS_RENDER_TARGET *target);

/*****************************************************************************/
/* AMS_RENDER_TARGET *amRenderSetTarget(AMS_RENDER_TARGET *target,           */
/*                 Uint32 flag, NNS_RGBA_U8 *color, float z, Sint32 stencil) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  target      : レンダリングターゲット構造体                       */
/*          flag        : クリアフラグ(AMD_RENDER_CLEAR～)                   */
/*          color       : クリアカラー                                       */
/*          z           : クリアＺ                                           */
/*          stencil     : クリアステンシル                                   */
/* [RETURN]  設定されていたレンダリングターゲットの構造体                    */
/* [FUNCTION]  レンダリングターゲットの設定                                  */
/*****************************************************************************/
AMS_RENDER_TARGET *amRenderSetTarget(AMS_RENDER_TARGET *target, Uint32 flag = 0,
		NNS_RGBA_U8 *color = NULL, float z = 1.0f, Sint32 stencil = 0);

/*****************************************************************************/
/* AMS_RENDER_TARGET *amRenderGetTarget(void)                                */
/*---------------------------------------------------------------------------*/
/* [RETURN]  設定されているレンダリングターゲットの構造体                    */
/* [FUNCTION]  レンダリングターゲットの取得                                  */
/*****************************************************************************/
AMS_RENDER_TARGET *amRenderGetTarget(void);

/*****************************************************************************/
/* void amRenderSetTexture(NNE_TEXSOLT slot,                                 */
/*                                  AMS_RENDER_TARGET *target, Sint32 index) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  slot    : テクスチャスロット                                     */
/*          target  : レンダリングターゲット構造体                           */
/*          index   : レンダリングテクスチャインデックス                     */
/* [FUNCTION]  レンダリングテクスチャの設定                                  */
/*****************************************************************************/
void amRenderSetTexture(NNE_TEXSLOT slot, AMS_RENDER_TARGET *target, Sint32 index);

/*****************************************************************************/
/* void amRenderCopyTarget(AMS_RENDER_TARGET *target)                        */
/*---------------------------------------------------------------------------*/
/* [INPUT]  target      : レンダリングターゲット構造体                       */
/* [FUNCTION]  レンダリングターゲットのコピー                                */
/*             カレントターゲットの内容を指定のターゲットにコピーする        */
/*****************************************************************************/
void amRenderCopyTarget(AMS_RENDER_TARGET *target, NNS_RGBA_U8* color=0);


#endif
