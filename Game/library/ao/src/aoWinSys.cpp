// ===========================================================================
/*!
	@file	aoWinSys.cpp
	@brief	AoLibrary ウインドウシステム定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct AOS_WIN_DRAW_WORK
// ---------------------------------------------------------------------------
//!	描画タスクワーク
// ===========================================================================
typedef struct tag_AOS_WIN_DRAW_WORK {
	AOE_WIN_TYPE		type;			//!< ウインドウタイプ
	NNS_TEXLIST*		texlist;		//!< テクスチャリスト
	u32					tex_id;			//!< テクスチャID
	f32					x;				//!< ウインドウ中心X位置
	f32					y;				//!< ウインドウ中心Y位置
	f32					w;				//!< ウインドウ横サイズ
	f32					h;				//!< ウインドウ縦サイズ
} AOS_WIN_DRAW_WORK; // 28 byte

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// タイプ共通
static void aoWinSysTaskDraw(AMS_TCB* tcb);

// タイプA
static void aoWinSysDrawPrimitveA(
	NNS_TEXLIST* texlist, u32 tex_id, f32 x, f32 y, f32 w, f32 h);
static void aoWinSysMakeCommandA(
	u32 state, NNS_TEXLIST* texlist, u32 tex_id,
	f32 x, f32 y, f32 w, f32 h, f32 z);
static void aoWinSysMakeVertex00A(
	NNS_PRIM3D_PCT* v, f32 x, f32 y, f32 w, f32 h);
static void aoWinSysMakeVertex01A(
	NNS_PRIM3D_PCT* v, f32 x, f32 y, f32 w, f32 h);
static void aoWinSysMakeVertex02A(
	NNS_PRIM3D_PCT* v, f32 x, f32 y, f32 w, f32 h);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ***************************************************************************
// 描画
// ***************************************************************************
// ===========================================================================
//	AoWinSysDraw
/*!
	ウインドウ描画(タイプ指定)

	@param type		[in] ウインドウタイプ
	@param texlist	[io] テクスチャリスト
	@param tex_id	[in] テクスチャID
	@param x		[in] ウインドウ中心X位置
	@param y		[in] ウインドウ中心Y位置
	@param w		[in] ウインドウ横サイズ
	@param h		[in] ウインドウ縦サイズ
	@note
	Zテスト無効、Zマスク有効として描画します。\n
*/
// ===========================================================================
void AoWinSysDraw(
	AOE_WIN_TYPE type, NNS_TEXLIST* texlist, u32 tex_id,
	f32 x, f32 y, f32 w, f32 h)
{
	amAssert((u32)type < AOD_WIN_TYPE_NUM);
	amAssert(texlist);
	amAssert(tex_id < (u32)texlist->nTex);

	UNREFERENCED_PARAMETER(type);

	// カメラ設定
	AoActDrawPre();

	// タイプA
	aoWinSysDrawPrimitveA(texlist, tex_id, x, y, w, h);
}

// ===========================================================================
//	AoWinSysDraw
/*!
	ウインドウ描画(タイプ指定 ステート描画)

	@param type		[in] ウインドウタイプ
	@param texlist	[io] テクスチャリスト
	@param tex_id	[in] テクスチャID
	@param x		[in] ウインドウ中心X位置
	@param y		[in] ウインドウ中心Y位置
	@param w		[in] ウインドウ横サイズ
	@param h		[in] ウインドウ縦サイズ
	@param state	[in] ステート
	@param z		[in] Z値
	@note
	カメラ設定等が必要になりますので、
	ステート実行する際には事前にAoActDrawPreを呼び出しておいて下さい。\n
*/
// ===========================================================================
void AoWinSysDrawState(
	AOE_WIN_TYPE type, NNS_TEXLIST* texlist, u32 tex_id,
	f32 x, f32 y, f32 w, f32 h, u32 state, f32 z)
{
	amAssert((u32)type < AOD_WIN_TYPE_NUM);
	amAssert(texlist);
	amAssert(tex_id < (u32)texlist->nTex);

	UNREFERENCED_PARAMETER(type);

	// タイプA
	aoWinSysMakeCommandA(state, texlist, tex_id, x, y, w, h, z);
}

// ===========================================================================
//	AoWinSysDraw
/*!
	ウインドウ描画(タイプ指定 タスク描画)

	@param type		[in] ウインドウタイプ
	@param texlist	[io] テクスチャリスト
	@param tex_id	[in] テクスチャID
	@param x		[in] ウインドウ中心X位置
	@param y		[in] ウインドウ中心Y位置
	@param w		[in] ウインドウ横サイズ
	@param h		[in] ウインドウ縦サイズ
	@param prio		[in] 描画タスク優先度
*/
// ===========================================================================
void AoWinSysDrawTask(
	AOE_WIN_TYPE type, NNS_TEXLIST* texlist, u32 tex_id,
	f32 x, f32 y, f32 w, f32 h, u16 prio)
{
	amAssert((u32)type < AOD_WIN_TYPE_NUM);
	amAssert(texlist);
	amAssert(tex_id < (u32)texlist->nTex);

	// ワーク作成
	AOS_WIN_DRAW_WORK* work =
		(AOS_WIN_DRAW_WORK*)amDrawMallocDataBuffer(sizeof(AOS_WIN_DRAW_WORK));
	work->type = type;
	work->texlist = texlist;
	work->tex_id = tex_id;
	work->x = x;
	work->y = y;
	work->w = w;
	work->h = h;

	// 描画タスク作成
	amDrawMakeTask(aoWinSysTaskDraw, prio, (u32)work);
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ***************************************************************************
// タイプ共通
// ***************************************************************************
// ===========================================================================
//! 描画タスク
// ===========================================================================
void aoWinSysTaskDraw(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_WIN_DRAW_WORK* work = *((AOS_WIN_DRAW_WORK**)amTaskGetWork(tcb));

	// カメラ設定
	AoActDrawPre();

	// タイプA
	aoWinSysDrawPrimitveA(
		work->texlist, work->tex_id, work->x, work->y, work->w, work->h);
}


// ***************************************************************************
// タイプA
// ***************************************************************************
// ===========================================================================
//! プリミティブ描画
// ===========================================================================
void aoWinSysDrawPrimitveA(
	NNS_TEXLIST* texlist, u32 tex_id, f32 x, f32 y, f32 w, f32 h)
{
	NNS_PRIM3D_PCT v[8];

	// 描画ステート初期化
	amDrawPushState();
	amDrawInitState();

	// Z更新無効 & Zテスト無効
#if defined(AOD_PLATFORM_WIN32) | defined(AOD_PLATFORM_XBOX360)

	nnSetPrimitive3DAlphaTestDXG20(NNE_FALSE);
	nnSetPrimitive3DDepthMaskDXG20(NNE_FALSE);
	nnSetPrimitive3DDepthTestDXG20(NNE_FALSE);

#elif defined(AOD_PLATFORM_PS3)

	nnSetPrimitive3DAlphaTestPS3(NNE_FALSE);
	nnSetPrimitive3DDepthMaskPS3(NNE_FALSE);
	nnSetPrimitive3DDepthFuncPS3(NND_CMPFUNC_PS3_ALWAYS);

#elif defined(AOD_PLATFORM_WII)

	nnSetPrimitive3DAlphaCompareGC(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	nnSetPrimitive3DZModeGC(GX_FALSE, GX_ALWAYS, GX_FALSE);

#elif defined(AOD_PLATFORM_IPHONE)

	nnSetPrimitive3DAlphaFuncGL(NND_CMPFUNC_GL_ALWAYS, 0.5f);
	nnSetPrimitive3DDepthMaskGL(FALSE);
	nnSetPrimitive3DDepthFuncGL(NND_CMPFUNC_GL_ALWAYS);

#endif

	// ブレンド設定
#if defined(AOD_PLATFORM_WIN32) | defined(AOD_PLATFORM_XBOX360)
	nnSetPrimitive3DBlendDXG20(
		NNE_BLENDMODE_SRCALPHA, NNE_BLENDMODE_INVSRCALPHA,
		NNE_BLENDOP_ADD);
#elif defined(AOD_PLATFORM_PS3)
	nnSetPrimitive3DBlendPS3(
		NND_BLENDFUNC_PS3_SRC_ALPHA,
		NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA,
		NND_BLENDOP_PS3_FUNC_ADD);
#elif defined(AOD_PLATFORM_WII)
	nnSetPrimitive3DBlendModeGC(
		GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
#elif defined(AOD_PLATFORM_IPHONE)
	nnSetPrimitiveBlend(NNE_PRIM_BLEND_BLEND);
#endif

	// フォグ設定
	amDrawSetFog(0);

	// テクスチャ設定
	nnSetPrimitiveTexNum(texlist, (s32)tex_id);
	nnSetPrimitiveTexState(
		NNE_PRIM_TEXBLEND_MODULATE, NNE_PRIM_TEXCOORD_UV,
		NNE_PRIM_TEXWRAP_CLAMP, NNE_PRIM_TEXWRAP_CLAMP);

	// プリミティブ描画開始
	nnBeginDrawPrimitive3D(
		NNE_PRIM3D_FMT_PCT,
		NNE_PRIM_ALPHABLEND_ON,
		NNE_PRIM_LIGHT_DISABLE,
		NNE_PRIM_CULL_NONE);

	// ウインドウ上側描画
	aoWinSysMakeVertex00A(v, x, y, w, h);
	nnDrawPrimitive3D(NNE_PRIM_TRIANGLE_STRIP, v, 8);

	// ウインドウ中央描画
	aoWinSysMakeVertex01A(v, x, y, w, h);
	nnDrawPrimitive3D(NNE_PRIM_TRIANGLE_STRIP, v, 8);

	// ウインドウ下側描画
	aoWinSysMakeVertex02A(v, x, y, w, h);
	nnDrawPrimitive3D(NNE_PRIM_TRIANGLE_STRIP, v, 8);

	// プリミティブ描画終了
	nnEndDrawPrimitive3D();

	// 描画ステート復帰
	amDrawPopState();
}

// ===========================================================================
//! コマンド発行
// ===========================================================================
void aoWinSysMakeCommandA(
	u32 state, NNS_TEXLIST* texlist, u32 tex_id,
	f32 x, f32 y, f32 w, f32 h, f32 z)
{
	// 頂点作成
	NNS_PRIM3D_PCT* v;

	// データ作成
	AMS_PARAM_DRAW_PRIMITIVE prim;
	amZeroMemory(&prim, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	prim.mtx = NULL;
	prim.vtxPCT3D = NULL;
	prim.format3D = NNE_PRIM3D_FMT_PCT;
	prim.type = NNE_PRIM_TRIANGLE_STRIP;
	prim.count = 8;
	prim.texlist = texlist;
	prim.texId = (s32)tex_id;
	prim.ablend = NNE_PRIM_ALPHABLEND_ON;
	prim.sortZ = z;

#if defined(AOD_PLATFORM_WIN32) | defined(AOD_PLATFORM_XBOX360)

	prim.bldSrc = NNE_BLENDMODE_SRCALPHA;
	prim.bldDst = NNE_BLENDMODE_INVSRCALPHA;
	prim.bldMode = NNE_BLENDOP_ADD;

#elif defined(AOD_PLATFORM_PS3)

	prim.bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
	prim.bldDst = NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA;
	prim.bldMode = NND_BLENDOP_PS3_FUNC_ADD;

#elif defined(AOD_PLATFORM_WII)

	prim.bldSrc = GX_BL_SRCALPHA;
	prim.bldDst = GX_BL_INVSRCALPHA;
	prim.bldMode = GX_BM_BLEND;

#elif defined(AOD_PLATFORM_IPHONE)
	// 未作成
#endif

	prim.aTest = 0;
	prim.zMask = 1;
	prim.zTest = 0;
	prim.noSort = 1;
	prim.uwrap = NNE_PRIM_TEXWRAP_CLAMP;
	prim.vwrap = NNE_PRIM_TEXWRAP_CLAMP;

	// ウインドウ上側頂点コマンド発行
	v = (NNS_PRIM3D_PCT*)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 8);
	aoWinSysMakeVertex00A(v, x, y, w, h);
	prim.vtxPCT3D = v;
	amDrawPrimitive3D(state, &prim);

	// ウインドウ中央頂点コマンド発行
	v = (NNS_PRIM3D_PCT*)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 8);
	aoWinSysMakeVertex01A(v, x, y, w, h);
	prim.vtxPCT3D = v;
	amDrawPrimitive3D(state, &prim);

	// ウインドウ下側頂点コマンド発行
	v = (NNS_PRIM3D_PCT*)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 8);
	aoWinSysMakeVertex02A(v, x, y, w, h);
	prim.vtxPCT3D = v;
	amDrawPrimitive3D(state, &prim);
}

// ===========================================================================
//! 頂点作成00
// ===========================================================================
void aoWinSysMakeVertex00A(
	NNS_PRIM3D_PCT* v, f32 x, f32 y, f32 w, f32 h)
{
	// UV座標設定
	v[0].Tex.u = v[1].Tex.u = v[6].Tex.u = v[7].Tex.u = 0.0f;
	v[2].Tex.u = v[3].Tex.u = v[4].Tex.u = v[5].Tex.u = 1.0f;
	v[0].Tex.v = v[2].Tex.v = v[4].Tex.v = v[6].Tex.v = 0.0f;
	v[1].Tex.v = v[3].Tex.v = v[5].Tex.v = v[7].Tex.v = 1.0f;

	// カラー設定
	v[0].Col = v[1].Col = v[2].Col = v[3].Col =
	v[4].Col = v[5].Col = v[6].Col = v[7].Col = 0xffffffff;

	// 頂点設定
	f32 l = x - (w * 0.5f);
	f32 t = y - (h * 0.5f);
	f32 r = l + w;
	v[0].Pos.x = v[1].Pos.x = l - 32.0f;
	v[2].Pos.x = v[3].Pos.x = l;
	v[4].Pos.x = v[5].Pos.x = r;
	v[6].Pos.x = v[7].Pos.x = r + 32.0f;
	v[0].Pos.y = v[2].Pos.y = v[4].Pos.y = v[6].Pos.y = t - 32.0f;
	v[1].Pos.y = v[3].Pos.y = v[5].Pos.y = v[7].Pos.y = t;
	v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = 
	v[4].Pos.z = v[5].Pos.z = v[6].Pos.z = v[7].Pos.z = -2.0f;

	// 頂点補正
	AoActDrawCorWide(v, 8, AOD_ACT_CORW_CENTER);
}

// ===========================================================================
//! 頂点作成01
// ===========================================================================
void aoWinSysMakeVertex01A(
	NNS_PRIM3D_PCT* v, f32 x, f32 y, f32 w, f32 h)
{
	// UV座標設定
	v[0].Tex.u = v[1].Tex.u = v[6].Tex.u = v[7].Tex.u = 0.0f;
	v[2].Tex.u = v[3].Tex.u = v[4].Tex.u = v[5].Tex.u = 1.0f;
	v[0].Tex.v = v[2].Tex.v = v[4].Tex.v = v[6].Tex.v = 1.0f;
	v[1].Tex.v = v[3].Tex.v = v[5].Tex.v = v[7].Tex.v = 1.0f;

	// カラー設定
	v[0].Col = v[1].Col = v[2].Col = v[3].Col =
	v[4].Col = v[5].Col = v[6].Col = v[7].Col = 0xffffffff;

	// 頂点設定
	f32 l = x - (w * 0.5f);
	f32 t = y - (h * 0.5f);
	f32 r = l + w;
	f32 b = t + h;
	v[0].Pos.x = v[1].Pos.x = l - 32.0f;
	v[2].Pos.x = v[3].Pos.x = l;
	v[4].Pos.x = v[5].Pos.x = r;
	v[6].Pos.x = v[7].Pos.x = r + 32.0f;
	v[0].Pos.y = v[2].Pos.y = v[4].Pos.y = v[6].Pos.y = t;
	v[1].Pos.y = v[3].Pos.y = v[5].Pos.y = v[7].Pos.y = b;
	v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = 
	v[4].Pos.z = v[5].Pos.z = v[6].Pos.z = v[7].Pos.z = -2.0f;

	// 頂点補正
	AoActDrawCorWide(v, 8, AOD_ACT_CORW_CENTER);
}

// ===========================================================================
//! 頂点作成02
// ===========================================================================
void aoWinSysMakeVertex02A(
	NNS_PRIM3D_PCT* v, f32 x, f32 y, f32 w, f32 h)
{
	// UV座標設定
	v[0].Tex.u = v[1].Tex.u = v[6].Tex.u = v[7].Tex.u = 0.0f;
	v[2].Tex.u = v[3].Tex.u = v[4].Tex.u = v[5].Tex.u = 1.0f;
	v[0].Tex.v = v[2].Tex.v = v[4].Tex.v = v[6].Tex.v = 1.0f;
	v[1].Tex.v = v[3].Tex.v = v[5].Tex.v = v[7].Tex.v = 0.0f;

	// カラー設定
	v[0].Col = v[1].Col = v[2].Col = v[3].Col =
	v[4].Col = v[5].Col = v[6].Col = v[7].Col = 0xffffffff;

	// 頂点設定
	f32 l = x - (w * 0.5f);
	f32 t = y - (h * 0.5f);
	f32 r = l + w;
	f32 b = t + h;
	v[0].Pos.x = v[1].Pos.x = l - 32.0f;
	v[2].Pos.x = v[3].Pos.x = l;
	v[4].Pos.x = v[5].Pos.x = r;
	v[6].Pos.x = v[7].Pos.x = r + 32.0f;
	v[0].Pos.y = v[2].Pos.y = v[4].Pos.y = v[6].Pos.y = b;
	v[1].Pos.y = v[3].Pos.y = v[5].Pos.y = v[7].Pos.y = b + 32.0f;
	v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = 
	v[4].Pos.z = v[5].Pos.z = v[6].Pos.z = v[7].Pos.z = -2.0f;

	// 頂点補正
	AoActDrawCorWide(v, 8, AOD_ACT_CORW_CENTER);
}

// ===========================================================================
//	function
/*!
	説明

	@param param0	[in] 入力引数0説明
	@param param1	[out] 出力ポインタ引数1説明
	@param param2	[io] 入出力ポインタ引数2説明
	@return 返値説明
	@note 補足説明
*/
// ===========================================================================

// ===========================================================================
//	int variable
// ---------------------------------------------------------------------------
//!	変数説明
// ===========================================================================

	// =======================================================================
	//	function
	/*!
		説明

		@param param0	[in] 入力引数0説明
		@param param1	[out] 出力ポインタ引数1説明
		@param param2	[io] 入出力ポインタ引数2説明
		@return 返値説明
		@note 補足説明
	*/
	// =======================================================================

// ***************************************************************************
// ラベル
// ***************************************************************************
