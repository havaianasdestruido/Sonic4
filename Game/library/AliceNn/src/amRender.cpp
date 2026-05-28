/*****************************************************************************/
/*      amRender.cpp                Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* レンダリングターゲット管理ライブラリプログラム                            */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090417-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files ---------------------------------------------------------*/

#include "alice.h"


/*--- Macros ----------------------------------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

void amDrawBuildShader(void);


/*--- External Valiables ----------------------------------------------------*/

#if _PC | _XBOX
extern IDirect3DVertexShader9*		 _am_draw_sprite_VS;
extern IDirect3DPixelShader9*		 _am_draw_sprite_PS;
#elif _WII
extern MEMHeapHandle		_am_wii_texture_heap;
#endif


/*--- Global Variables ------------------------------------------------------*/

AMS_RENDER_MANAGER		_am_render_manager;

AMS_RENDER_TARGET		_am_render_default;

/*--- Local Variables -------------------------------------------------------*/

/*--- Global Functions ------------------------------------------------------*/

/*****************************************************************************/
/* void amRenderInit(void)                                                   */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  レンダリングターゲット管理の初期化                            */
/*****************************************************************************/
void amRenderInit(void)
{
	AMS_RENDER_MANAGER		*manager;
	AMS_RENDER_TARGET		*target;

	// デフォルトターゲットの設定
	target		= &_am_render_default;
#if _PC | _XBOX
#if _PC
	LPDIRECT3DDEVICE9	d3ddev = amWinDxGetDirect3DDevice();
	target->flag		= AMD_RENDER_FLAG_USE_DEPTH;
#else
	LPDIRECT3DDEVICE9	d3ddev = amXboxDxGetDirect3DDevice();
	target->tiling		= _am_draw_video.tiling;
	target->flag		= AMD_RENDER_FLAG_USE_DEPTH;
	if (target->tiling != NULL)
		target->flag		|= AMD_RENDER_FLAG_RESOLVE_COLOR0;
#endif
	target->surface_num	= 1;
	target->width		= (Sint32)AMD_DISPLAY_WIDTH;
	target->height		= (Sint32)AMD_DISPLAY_HEIGHT;
	target->aspect		= AMD_DISPLAY_WIDTH / AMD_DISPLAY_HEIGHT;
	d3ddev->GetRenderTarget(0, &target->surface_color[0]);
	d3ddev->GetDepthStencilSurface(&target->surface_depth);
#elif _PS3
	target->flag		= AMD_RENDER_FLAG_USE_DEPTH;
	target->surface_num	= 1;
	target->width		= (Sint32)AMD_DISPLAY_WIDTH;
	target->height		= (Sint32)AMD_DISPLAY_HEIGHT;
	target->aspect		= AMD_DISPLAY_WIDTH / AMD_DISPLAY_HEIGHT;
	target->surface		= NULL;
#elif _WII
	target->flag		= AMD_RENDER_FLAG_USE_DEPTH;
	target->surface_num	= 1;
	target->width		= (Sint32)AMD_DISPLAY_WIDTH;
	target->height		= (Sint32)AMD_DISPLAY_HEIGHT;
	target->aspect		= AMD_DISPLAY_WIDTH / AMD_DISPLAY_HEIGHT;
	target->texture_color	= NULL;

	GXPixelFmt		format;
	GXGetPixelFmt(&format, &target->format_depth);
	switch (format) {
		case	GX_PF_RGB8_Z24:
			target->format_color	= GX_TF_RGBA8;
			break;
		case	GX_PF_RGBA6_Z24:
			target->format_color	= GX_TF_RGBA8;
			break;
		case	GX_PF_RGB565_Z16:
			target->format_color	= GX_TF_RGB565;
			break;
	}
#endif

	// ワークの初期化
	manager		= &_am_render_manager;
	manager->targetp	= &_am_render_default;
	manager->target_now	= _am_render_default;
}


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
AMS_RENDER_TARGET *amRenderCreate(AMS_RENDER_TARGET *target, Sint32 width, Sint32 height, Sint32 surface_num, Sint32 *format, Sint32 depth, Uint32 flag)
{
	amThreadCheckSafe(1 | AMD_THREAD_SAFE_NO_THREAD_DRAW, "amRenderCreate");

	if (target == NULL) {
		target		= (AMS_RENDER_TARGET *)amMemAlloc(sizeof(AMS_RENDER_TARGET));
		memset(target, 0, sizeof(AMS_RENDER_TARGET));
		target->flag	|= AMD_RENDER_FLAG_DELETE;
	} else
		memset(target, 0, sizeof(AMS_RENDER_TARGET));

	target->width		= width;
	target->height		= height;
	target->surface_num	= surface_num;
	target->aspect		= (float)width / (float)height;

#if _PC
	LPDIRECT3DDEVICE9	d3ddev = amWinDxGetDirect3DDevice();
	Sint32		i;

	if (depth != AMD_RENDER_DEPTH_DEFAULT) {
		d3ddev->CreateDepthStencilSurface(width, height, (D3DFORMAT)depth,
				D3DMULTISAMPLE_NONE, 0, TRUE,
				&target->surface_depth, NULL);
	}
	target->flag	|= AMD_RENDER_FLAG_USE_DEPTH;

	for (i = 0; i < surface_num; i++) {
		if (flag & (AMD_RENDER_FLAG_RESOLVE_COLOR0 << i)) {
			D3DXCreateTexture(d3ddev, width, height, 1, D3DUSAGE_RENDERTARGET,
					(D3DFORMAT)format[i], D3DPOOL_DEFAULT, &target->texture_color[i]);
			target->texture_color[i]->GetSurfaceLevel(0, &target->surface_color[i]);
			target->flag	|= AMD_RENDER_FLAG_RESOLVE_COLOR0 << i;
		}
	}
#elif _XBOX
	LPDIRECT3DDEVICE9	d3ddev = amXboxDxGetDirect3DDevice();
	Sint32		i;
	D3DSURFACE_PARAMETERS	surface_param = { 0};

	if (height < 1080) {
		// タイリングなし
		target->tiling		= NULL;
		for (i = 0; i < surface_num; i++) {
			d3ddev->CreateRenderTarget(width, height, (D3DFORMAT)format[i],
					D3DMULTISAMPLE_NONE, 0, FALSE,
					&target->surface_color[i], &surface_param);
			surface_param.Base	+= XGSurfaceSize(width, height, (D3DFORMAT)format[i],
					D3DMULTISAMPLE_NONE);
			if (flag & (AMD_RENDER_FLAG_RESOLVE_COLOR0 << i)) {
				D3DXCreateTexture(d3ddev, width, height, 1, D3DUSAGE_RENDERTARGET,
						(D3DFORMAT)format[i], D3DPOOL_DEFAULT, &target->texture_color[i]);
				target->flag	|= AMD_RENDER_FLAG_RESOLVE_COLOR0 << i;
			}
		}
		if (depth != AMD_RENDER_DEPTH_DEFAULT) {
			d3ddev->CreateDepthStencilSurface(width, height, (D3DFORMAT)depth,
					D3DMULTISAMPLE_NONE, 0, TRUE,
					&target->surface_depth, &surface_param);
			if (flag & AMD_RENDER_FLAG_RESOLVE_DEPTH) {
				D3DXCreateTexture(d3ddev, width, height, 1, D3DUSAGE_RENDERTARGET,
						(D3DFORMAT)depth, D3DPOOL_DEFAULT, &target->texture_depth);
				target->flag	|= AMD_RENDER_FLAG_RESOLVE_DEPTH;
			}
		}
	} else {
		// タイリングあり
		target->tiling		= &_am_xboxdx_tile_info[AMD_XBOXDX_TILE_1920x1080_1];

		DWORD	tile_width, tile_height;
		amXboxDxGetTileSize(target->tiling, &tile_width, &tile_height);

		for (i = 0; i < surface_num; i++) {
			d3ddev->CreateRenderTarget(tile_width, tile_height, (D3DFORMAT)format[i],
					target->tiling->msaa, 0, FALSE,
					&target->surface_color[i], &surface_param);
			surface_param.Base	+= XGSurfaceSize(tile_width, tile_height,
					(D3DFORMAT)format[i], target->tiling->msaa);
			if (flag & (AMD_RENDER_FLAG_RESOLVE_COLOR0 << i)) {
				D3DXCreateTexture(d3ddev, width, height, 1, D3DUSAGE_RENDERTARGET,
						(D3DFORMAT)format[i], D3DPOOL_DEFAULT, &target->texture_color[i]);
				target->flag	|= AMD_RENDER_FLAG_RESOLVE_COLOR0 << i;
			}
		}
		if (depth != AMD_RENDER_DEPTH_DEFAULT) {
			d3ddev->CreateDepthStencilSurface(tile_width, tile_height, (D3DFORMAT)depth,
					target->tiling->msaa, 0, TRUE,
					&target->surface_depth, &surface_param);
			if (flag & AMD_RENDER_FLAG_RESOLVE_DEPTH) {
				D3DXCreateTexture(d3ddev, width, height, 1, D3DUSAGE_RENDERTARGET,
						(D3DFORMAT)depth, D3DPOOL_DEFAULT, &target->texture_depth);
				target->flag	|= AMD_RENDER_FLAG_RESOLVE_DEPTH;
			}
		}
	}

	target->flag	|= AMD_RENDER_FLAG_USE_DEPTH;
#elif _PS3
	target->texture_color[0]	= nnTexturePs3Create(NULL, width, height, 1,
			(NNE_FORMAT)format[0],
			(flag & AMD_RENDER_FLAG_TILED)? (NND_TEXTURE_RENDER | NND_TEXTURE_TILED):
			NND_TEXTURE_RENDER);
	amAssert(target->texture_color[0] != NULL);

	if (depth != AMD_RENDER_DEPTH_DEFAULT) {
		target->texture_depth		= nnDepthBufferPs3Create(
				width, height, (NNE_FORMAT)depth,
				(flag & AMD_RENDER_FLAG_TILED)? NND_DEPTHBUFFER_TILED: 0);
		target->surface				= nnRenderTargetPs3Create(
				target->texture_color[0], target->texture_depth);
	} else {
		target->surface				= nnRenderTargetPs3Create(
				target->texture_color[0], NND_RENDERTARGET_DEPTHBUFFER_DEFAULT);
	}
	amAssert(target->surface != NULL);
	target->flag				|= AMD_RENDER_FLAG_USE_DEPTH
								| AMD_RENDER_FLAG_RESOLVE_DEPTH;
#elif _WII
	Uint32		size;
	target->format_color		= (GXTexFmt)format[0];
	target->format_depth		= (GXZFmt16)depth;
	size		= GXGetTexBufferSize(width, height,
			(GXTexFmt)format[0], GX_FALSE, 0);
	target->texture_color		= MEMAllocFromExpHeapEx(
			_am_wii_texture_heap, size, 32);
	DCFlushRange(target->texture_color, size);
	GXInitTexObj(&target->texobj,
			target->texture_color,
			width, height, target->format_color,
			GX_CLAMP, GX_CLAMP, GX_DISABLE);
	GXInitTexObjLOD(&target->texobj,
			GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f,
			GX_DISABLE, GX_DISABLE, GX_ANISO_1);
#else
#endif

	return	target;
}


/*****************************************************************************/
/* void amRenderDelete(AMS_RENDER_TARGET *target)                            */
/*---------------------------------------------------------------------------*/
/* [INPUT]  target      : レンダリングターゲット構造体                       */
/* [FUNCTION]  レンダリングターゲットの削除                                  */
/*****************************************************************************/
void amRenderDelete(AMS_RENDER_TARGET *target)
{
	amThreadCheckSafe(1 | AMD_THREAD_SAFE_NO_THREAD_DRAW, "amRenderDelete");

#if _PC | _XBOX
	if (target->flag & AMD_RENDER_FLAG_USE_DEPTH) {
		target->surface_depth->Release();
		if (target->texture_depth != NULL)
			target->texture_depth->Release();
	}
	Sint32		i;
	for (i = 0; i < target->surface_num; i++) {
		IDirect3DSurface9_Release(target->surface_color[i]);
		if (target->texture_color[i] != NULL)
			target->texture_color[i]->Release();
	}
#elif _PS3
	if (target->texture_color[0] != NULL)
		nnTexturePs3Destroy(target->texture_color[0]);
	if (target->texture_depth != NULL)
		nnDepthBufferPs3Destroy(target->texture_depth);
	if (target->surface != NULL)
		nnRenderTargetPs3Destroy(target->surface);
#elif _WII
	if (target->texture_color != NULL)
		MEMFreeToExpHeap(_am_wii_texture_heap, target->texture_color);
#else
#endif

	memset(target, 0, sizeof(AMS_RENDER_TARGET));

	if (target->flag & AMD_RENDER_FLAG_DELETE)
		amMemFree(target);
}


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
AMS_RENDER_TARGET *amRenderSetTarget(AMS_RENDER_TARGET *target, Uint32 flag, NNS_RGBA_U8 *color, float z, Sint32 stencil)
{
	AMS_RENDER_MANAGER		*manager;
	AMS_RENDER_TARGET		*target0;

	amThreadCheckSafe(1, "amRenderSetTarget");

	if (target == NULL)
		target		= &_am_render_default;

	manager		= &_am_render_manager;
	target0		= manager->targetp;

	if (target0 == target)
		return	(target0 != &_am_render_default)? target0: NULL;

#if _PC | _XBOX | _PS3
	amDrawEnd(color, z, stencil);
#endif

	// Resolve
#if _WII
	target0		= &manager->target_now;

	if (_am_draw_in_scene) {
		if (target0->texture_color != NULL) {
			if (flag) {
				GXColor		col;
				if (color != NULL) {
					col.r	= color->r;
					col.g	= color->g;
					col.b	= color->b;
					col.a	= color->a;
				} else {
					col.r	= _am_draw_bg_color.r;
					col.g	= _am_draw_bg_color.g;
					col.b	= _am_draw_bg_color.b;
					col.a	= _am_draw_bg_color.a;
				}
				GXSetCopyClear(col, z);
				GXSetTexCopySrc(0, 0, target0->width, target0->height);
				GXSetCopyFilter(GX_FALSE, NULL, GX_FALSE, NULL);
				GXSetTexCopyDst(target0->width, target0->height,
						target0->format_color, GX_TRUE);
				GXCopyTex(target0->texture_color, GX_TRUE);
				flag	= 0;
			} else {
				GXSetTexCopySrc(0, 0, target0->width, target0->height);
				GXSetCopyFilter(GX_FALSE, NULL, GX_FALSE, NULL);
				GXSetTexCopyDst(target0->width, target0->height,
						target0->format_color, GX_TRUE);
				GXCopyTex(target0->texture_color, GX_FALSE);
			}
		}
	}

	target0		= manager->targetp;
#endif

	// Setting
	manager->target_now		= *target;
	manager->targetp		= target;
#if _PC | _XBOX
#if _PC
	LPDIRECT3DDEVICE9	d3ddev = amWinDxGetDirect3DDevice();
#elif _XBOX
	LPDIRECT3DDEVICE9	d3ddev = amXboxDxGetDirect3DDevice();
#endif

#if _PC
	Sint32		i;
	for (i = 0; i < target->surface_num; i++)
		d3ddev->SetRenderTarget(i, target->surface_color[i]);
	if (target->flag & AMD_RENDER_FLAG_USE_DEPTH)
		d3ddev->SetDepthStencilSurface(target->surface_depth);
#else
	Sint32		i;
	D3DSURFACES	surfaces = {
		NULL, NULL, NULL, NULL, NULL,
	};
	for (i = 0; i < target->surface_num; i++)
		surfaces.pRenderTarget[i]		= target->surface_color[i];
	if (target->flag & AMD_RENDER_FLAG_USE_DEPTH)
		surfaces.pDepthStencilSurface	= target->surface_depth;
	d3ddev->SetSurfaces(&surfaces,
			(target->tiling)? D3DSETSURFACES_SET_AS_TILING_SURFACES: 0);
#endif

#elif _PS3
	NNS_CONFIG_PS3		config;
	config.pSbglDevice	= _am_ps3_device;
	config.WindowWidth	= target->width;
	config.WindowHeight	= target->height;
	nnConfigureSystemPS3(&config);

	if (target->surface != NULL)
		nnRenderTargetPs3Bind(target->surface);
	else if ((target0 != NULL) && (target0->surface != NULL))
		nnRenderTargetPs3Unbind(target0->surface);
	nnDevicePs3SetViewport(_am_ps3_device);
	nnDevicePs3SetScissor(_am_ps3_device);

	// 仮設定
	nnSetProjection(amDrawGetProjectionMatrix(), amDrawGetProjectionType());
#elif _WII
	GXSetViewport(0.0f, 0.0f,
			(float)target->width,
			(float)target->height,
			0.0f, 1.0f);
	GXSetScissor(0, 0, target->width, target->height);
#endif

#if _PC | _XBOX | _PS3
	amDrawBegin(target, flag, color, z, stencil);
#endif

	return	(target0 != &_am_render_default)? target0: NULL;
}


/*****************************************************************************/
/* AMS_RENDER_TARGET *amRenderGetTarget(void)                                */
/*---------------------------------------------------------------------------*/
/* [RETURN]  設定されているレンダリングターゲットの構造体                    */
/* [FUNCTION]  レンダリングターゲットの取得                                  */
/*****************************************************************************/
AMS_RENDER_TARGET *amRenderGetTarget(void)
{
	AMS_RENDER_TARGET	*target;

	target	= _am_render_manager.targetp;

	return	(target != &_am_render_default)? target: NULL;
}


/*****************************************************************************/
/* void amRenderSetTexture(NNE_TEXSOLT slot,                                 */
/*                                  AMS_RENDER_TARGET *target, Sint32 index) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  slot    : テクスチャスロット                                     */
/*          target  : レンダリングターゲット構造体                           */
/*          index   : レンダリングテクスチャインデックス                     */
/* [FUNCTION]  レンダリングテクスチャの設定                                  */
/*****************************************************************************/
void amRenderSetTexture(NNE_TEXSLOT slot, AMS_RENDER_TARGET *target, Sint32 index)
{
	amThreadCheckSafe(1, "amRenderSetTexture");

#if _PC | _XBOX
#if _PC
	LPDIRECT3DDEVICE9	d3ddev = amWinDxGetDirect3DDevice();
#else
	LPDIRECT3DDEVICE9	d3ddev = amXboxDxGetDirect3DDevice();
#endif
	if ((index >= 0) && (index < target->surface_num))
		d3ddev->SetTexture(slot, target->texture_color[index]);
	else
		d3ddev->SetTexture(slot, target->texture_depth);
#elif _PS3
	NNS_TEXLIST		texlist;
	NNS_TEXINFO		texinfo;

	texlist.nTex	= 1;
	texlist.pTexInfoList	= &texinfo;
	texinfo.pSbglTexture	= target->texture_color[index];
	texinfo.Flag	= 0;

	nnSetPrimitiveTexNum(&texlist, 0);
#elif _WII
	GXPixModeSync();
	GXLoadTexObj(&target->texobj, (GXTexMapID)slot);
#endif
}


/*****************************************************************************/
/* void amRenderCopyTarget(AMS_RENDER_TARGET *target)                        */
/*---------------------------------------------------------------------------*/
/* [INPUT]  target      : レンダリングターゲット構造体                       */
/* [FUNCTION]  レンダリングターゲットのコピー                                */
/*             カレントターゲットの内容を指定のターゲットにコピーする        */
/*****************************************************************************/
void amRenderCopyTarget(AMS_RENDER_TARGET *target, NNS_RGBA_U8* color)
{
	amThreadCheckSafe(1, "amRenderCopyTarget");

#if _PC
	AMS_RENDER_TARGET	*target0;

//	target0		= amRenderSetTarget(target);
	target0		= amRenderSetTarget(target, AMD_RENDER_CLEAR_ALL, color);

	amRenderSetTexture(NNE_TEXSLOT_0, target0, 0);

	struct TVERTEX {
		float		p[4];
		float		tu, tv;
	} vtx[] = {
		{ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,},
		{ (float)target->width, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,},
		{ 0.0f, (float)target->height, 0.0f, 1.0f, 0.0f, 1.0f,},
		{ (float)target->width, (float)target->height, 0.0f, 1.0f, 1.0f, 1.0f,},
	};

	amDrawBuildShader();

	LPDIRECT3DDEVICE9	d3ddev;

	d3ddev	= amWinDxGetDirect3DDevice();

	d3ddev->SetTextureStageState(0, D3DTSS_COLOROP,	  D3DTOP_SELECTARG1);
	d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	d3ddev->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	d3ddev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	D3DVIEWPORT9 vp;
	d3ddev->GetViewport(&vp);
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	d3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);
	d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3ddev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0));
	d3ddev->SetVertexShader(_am_draw_sprite_VS);
	d3ddev->SetPixelShader(_am_draw_sprite_PS);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx, sizeof( TVERTEX ));

	nnResetVertexDeclarationDXG20();

	amRenderSetTarget(target0);
#elif _XBOX
	AMS_RENDER_TARGET		*target0;

	target0		= amRenderSetTarget(target);

	if (target->tiling != NULL) {
		struct TVERTEX {
			float		p[4];
			float		tu, tv;
		} vtx[] = {
			{ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,},
			{ (float)target->width, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,},
			{ 0.0f, (float)target->height, 0.0f, 1.0f, 0.0f, 1.0f,},
			{ (float)target->width, (float)target->height, 0.0f, 1.0f, 1.0f, 1.0f,},
		};
		Sint32		i;

		amRenderSetTexture(NNE_TEXSLOT_0, target0, 0);

		amDrawBuildShader();

		LPDIRECT3DDEVICE9	d3ddev;

		d3ddev	= amXboxDxGetDirect3DDevice();

		// コピーと復元（デプスは復元しなくても残ってる？）
		for (i = 0; i < 2; i++) {
			D3DBLENDSTATE	blendstate;
			blendstate.SrcBlend			= D3DBLEND_ONE;
			blendstate.BlendOp			= D3DBLENDOP_ADD;
			blendstate.DestBlend		= D3DBLEND_ZERO;
			blendstate.SrcBlendAlpha	= D3DBLEND_ONE;
			blendstate.BlendOpAlpha		= D3DBLENDOP_ADD;
			blendstate.DestBlendAlpha	= D3DBLEND_ZERO;
			d3ddev->SetBlendState(0, blendstate);

			d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
			d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

			D3DVIEWPORT9 vp;
			d3ddev->GetViewport(&vp);
			d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			d3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);
			d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			d3ddev->SetRenderState(D3DRS_VIEWPORTENABLE, FALSE);
			d3ddev->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0));
			d3ddev->SetVertexShader(_am_draw_sprite_VS);
			d3ddev->SetPixelShader(_am_draw_sprite_PS);
			d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx, sizeof( TVERTEX ));
			d3ddev->SetRenderState(D3DRS_VIEWPORTENABLE, TRUE);

			nnResetVertexDeclarationDXG20();

			if (i == 0)
				amRenderSetTarget(target0);
		}
	} else
		amRenderSetTarget(target0);
#elif _PS3
	AMS_RENDER_TARGET		*target0;

	amDrawPushState();

	target0		= amRenderSetTarget(target);

	amRenderSetTexture(NNE_TEXSLOT_0, target0, 0);

	NNS_PRIM2D_PCT	vtx[] = {
		{ { 0.0f, 0.0f,}, 0xffffffff, { 0.0f, 0.0f,}, },
		{ { target0->width, 0.0f,}, 0xffffffff, { 1.0f, 0.0f,}, },
		{ { 0.0f, target0->height,}, 0xffffffff, { 0.0f, 1.0f,}, },
		{ { target0->width, target0->height,}, 0xffffffff, { 1.0f, 1.0f,}, },
	};

	nnSetPrimitiveTexState(
			NNE_PRIM_TEXBLEND_MODULATE, NNE_PRIM_TEXCOORD_UV,
			NNE_PRIM_TEXWRAP_CLAMP, NNE_PRIM_TEXWRAP_CLAMP);
	nnSetPrimitive2DDepthFuncPS3(NND_CMPFUNC_PS3_ALWAYS);
	nnSetPrimitive2DDepthMaskPS3(NNE_OFF);

	nnBeginDrawPrimitive2D(NNE_PRIM2D_FMT_PCT, NNE_PRIM_ALPHABLEND_OFF);
	nnDrawPrimitive2D(NNE_PRIM_TRIANGLE_STRIP, vtx, 4, -1.0f);
	nnEndDrawPrimitive2D();

	nnSetPrimitive2DDepthMaskPS3(NNE_ON);
	nnSetPrimitive2DDepthFuncPS3(NND_CMPFUNC_PS3_LEQUAL);

	amRenderSetTarget(target0);

	amDrawPopState();
#elif _WII
	AMS_RENDER_MANAGER		*manager;
	AMS_RENDER_TARGET		*target0;

	manager		= &_am_render_manager;
	target0		= manager->targetp;

	GXSetTexCopySrc(0, 0, target0->width, target0->height);
	GXSetCopyFilter(GX_FALSE, NULL, GX_FALSE, NULL);
	GXSetTexCopyDst(target->width, target->height,
			target->format_color, GX_FALSE);
	GXCopyTex(target->texture_color, GX_FALSE);
//	GXDrawDone();

	GXPixModeSync();
	GXInvalidateTexAll();
#endif
}


/*--- Local Functions -------------------------------------------------------*/

