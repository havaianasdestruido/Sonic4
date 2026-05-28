// ==========================================================================
/*!
  @file izFade.cpp
  @brief フェード

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: izFade.cpp 20 2011-04-22 12:46:46Z thamada $
  $Date:: 2011-04-22 21:46:46 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gs.h"

#include "izFade.h"

//----- Definitions ---------------------------------------------------------

// IZS_FADE_WORK.flag
#define IZD_FADE_FLAG_DRAW_START			(0x00000001)	//!< 描画コマンド実行開始を行う
#define IZD_FADE_FLAG_STOP_UPDATE_1FRAME	(0x00000002)	//!< 1フレームだけ更新を停止します

typedef struct tag_IZS_FADE_DT_WORK {
	u32				draw_state;
	NNF_DRAWOBJ		drawflag;
} IZS_FADE_DT_WORK;

// NNS_RGBA8888
#define IZD_RGBA8888_SHIFT_RED		(24)
#define IZD_RGBA8888_SHIFT_GREEN	(16)
#define IZD_RGBA8888_SHIFT_BLUE		(8)
#define IZD_RGBA8888_SHIFT_ALPHA	(0)

//----- Macros --------------------------------------------------------------
// ==========================================================================
// IZM_FADE_COL_PAC
/*!
 *	カラーパック化
 *
 *	@param	r	[in]	カラー値 赤
 *	@param	g	[in]	カラー値 緑
 *	@param	b	[in]	カラー値 青
 *	@param	a	[in]	カラー値 α
 */
// ==========================================================================
#define IZM_FADE_COL_PAC(r, g, b, a)	((NNS_RGBA8888)((((r)&0xFF)<<IZD_RGBA8888_SHIFT_RED) | \
										(((g)&0xFF)<<IZD_RGBA8888_SHIFT_GREEN) | \
										(((b)&0xFF)<<IZD_RGBA8888_SHIFT_BLUE) | \
										(((a)&0xFF)<<IZD_RGBA8888_SHIFT_ALPHA)))

#define IZD_FADE_TASK_PRIO				(0x1000)	//!< タスク優先
										
//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void izFadeDest(MTS_TASK_TCB *tcb);
static void izFadeMain(MTS_TASK_TCB *tcb);
static void izFadeEndWaitMain(MTS_TASK_TCB *tcb);
static void izFadeDrawStart_DT(AMS_TCB *am_tcb);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
MTS_TASK_TCB	*iz_fade_tcb = NULL;

/// 標準フェードカラー
static const u8 iz_fade_color[2][3] = {
	// r, g, b
	{0x00, 0x00, 0x00},		// 黒
	{0xFF, 0xFF, 0xFF},		// 白
};
/// 標準フェードα
static const u8 iz_fade_alpha[2] = {
	0xFF,
	0x00,
};

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// IzFadeInitEasy
/*!
 *	フェード初期化
 *
 *	@param	fade_set_type	[in]	フェードセットタイプ IZE_FADE_SET_TYPE
 *	@param	fade_type		[in]	フェードタイプ IZE_FADE_TYPE		
 *	@param	time			[in]	フェード時間 (フレーム)
 *	@param	draw_start		[in]	描画開始処理の有無 (default TRUE)
 *
 *	@note
 *		設定されたカラーのプリミティブで画面を覆い、全画面フェードを行います。
 */
// ==========================================================================
void IzFadeInitEasy(IZE_FADE_SET_TYPE fade_set_type, IZE_FADE_TYPE fade_type, float time, BOOL draw_start/*=TRUE*/)
{
	s32	col_type, fade_a_type;

	MTM_ASSERT((u32)fade_set_type < IZE_FADE_SET_TYPE_MAX);
	MTM_ASSERT((u32)fade_type < IZE_FADE_TYPE_MAX);

	col_type	= (fade_type >> 1);
	fade_a_type	= fade_type & 0x01;

	IzFadeInit(IZD_FADE_TASK_GROUP_DEF, IZD_FADE_TASK_PAUSE_LEVEL_DEF, IZD_FADE_DT_PRIO_DEF, IZD_FADE_DRAW_STATE_DEF,
				fade_set_type,
				iz_fade_color[col_type][0], iz_fade_color[col_type][1], iz_fade_color[col_type][2],
				iz_fade_alpha[fade_a_type],
				iz_fade_color[col_type][0], iz_fade_color[col_type][1], iz_fade_color[col_type][2],
				iz_fade_alpha[fade_a_type ^ 0x01],
				time,
				draw_start);
}

// ==========================================================================
// IzFadeInitEasyTask
/*!
 *	フェード初期化
 *
 *	@param	fade_set_type	[in]	フェードセットタイプ IZE_FADE_SET_TYPE
 *	@param	start_col_r		[in]	開始カラー値 RED
 *	@param	start_col_g		[in]	開始カラー値 GREEN	
 *	@param	start_col_b		[in]	開始カラー値 BLUE	
 *	@param	start_col_a		[in]	開始カラー値 ALPHA	
 *	@param	end_col_r		[in]	終了カラー値 RED	
 *	@param	end_col_g		[in]	終了カラー値 GREEN			
 *	@param	end_col_b		[in]	終了カラー値 BLUE	
 *	@param	end_col_a		[in]	終了カラー値 RED			
 *	@param	time			[in]	フェード時間 (フレーム)
 *	@param	draw_start		[in]	描画開始処理の有無 (default TRUE)
 *
 *	@note
 *		設定されたカラーのプリミティブで画面を覆い、全画面フェードを行います。
 */
// ==========================================================================
void IzFadeInitEasyTask(IZE_FADE_SET_TYPE fade_set_type,
				u8 start_col_r, u8 start_col_g, u8 start_col_b, u8 start_col_a,
				u8 end_col_r, u8 end_col_g, u8 end_col_b, u8 end_col_a,
				float time, BOOL draw_start/*=TRUE*/)
{
	MTM_ASSERT((u32)fade_set_type < IZE_FADE_SET_TYPE_MAX);

	IzFadeInit(IZD_FADE_TASK_GROUP_DEF, IZD_FADE_TASK_PAUSE_LEVEL_DEF, IZD_FADE_DT_PRIO_DEF, IZD_FADE_DRAW_STATE_DEF,
				fade_set_type,
				start_col_r, start_col_g, start_col_b,
				start_col_a,
				end_col_r, end_col_g, end_col_b,
				end_col_a,
				time,
				draw_start);
}

// ==========================================================================
// IzFadeInitEasyColor
/*!
 *	フェード初期化
 *
 *	@param	group			[in]	フェード処理タスクグループ
 *	@param	pause_level		[in]	フェード処理ポーズレベル
 *	@param	dt_prio			[in]	描画処理プライオリティ
 *	@param	draw_state		[in]	描画ステート
 *	@param	fade_set_type	[in]	フェードセットタイプ IZE_FADE_SET_TYPE
 *	@param	fade_type		[in]	フェードタイプ IZE_FADE_TYPE
 *	@param	time			[in]	フェード時間 (フレーム)
 *	@param	draw_start		[in]	描画開始処理の有無 (default TRUE)
 *
 *	@note
 *		設定されたカラーのプリミティブで画面を覆い、全画面フェードを行います。
 */
// ==========================================================================
void IzFadeInitEasyColor(s32 group, u16 pause_level, u16 dt_prio, u32 draw_state, IZE_FADE_SET_TYPE fade_set_type,
				IZE_FADE_TYPE fade_type, float time, BOOL draw_start/*=TRUE*/)
{
	s32	col_type, fade_a_type;

	MTM_ASSERT((u32)fade_set_type < IZE_FADE_SET_TYPE_MAX);
	MTM_ASSERT((u32)fade_type < IZE_FADE_TYPE_MAX);

	col_type	= (fade_type >> 1);
	fade_a_type	= fade_type & 0x01;

	IzFadeInit(group, pause_level, dt_prio, draw_state,
				fade_set_type,
				iz_fade_color[col_type][0], iz_fade_color[col_type][1], iz_fade_color[col_type][2],
				iz_fade_alpha[fade_a_type],
				iz_fade_color[col_type][0], iz_fade_color[col_type][1], iz_fade_color[col_type][2],
				iz_fade_alpha[fade_a_type ^ 0x01],
				time,
				draw_start);
}

// ==========================================================================
// IzFadeInit
/*!
 *	フェード初期化
 *
 *	@param	group			[in]	フェード処理タスクグループ
 *	@param	pause_level		[in]	フェード処理ポーズレベル
 *	@param	dt_prio			[in]	描画処理プライオリティ
 *	@param	draw_state		[in]	描画ステート
 *	@param	fade_set_type	[in]	フェードセットタイプ IZE_FADE_SET_TYPE
 *	@param	start_col_r		[in]	開始カラー値 RED
 *	@param	start_col_g		[in]	開始カラー値 GREEN	
 *	@param	start_col_b		[in]	開始カラー値 BLUE	
 *	@param	start_col_a		[in]	開始カラー値 ALPHA	
 *	@param	end_col_r		[in]	終了カラー値 RED	
 *	@param	end_col_g		[in]	終了カラー値 GREEN			
 *	@param	end_col_b		[in]	終了カラー値 BLUE	
 *	@param	end_col_a		[in]	終了カラー値 RED			
 *	@param	time			[in]	フェード時間 (フレーム)
 *	@param	draw_start		[in]	描画開始処理の有無 (default TRUE)
 *
 *	@note
 *		設定されたカラーのプリミティブで画面を覆い、全画面フェードを行います。
 *		draw_startがFALSEの場合は、描画の登録を行うのみで、描画の開始を行いません。
 *		FALSEを設定する場合は、必ずユーザーが描画の開始(amDrawExecCommand)を
 *		行ってください。
 *		また、この設定のときは、ブレンディング設定などを復旧しないので、
 *		必要な場合はユーザーが復旧して下さい。
 */
// ==========================================================================
void IzFadeInit(s32 group, u16 pause_level, u16 dt_prio, u32 draw_state, IZE_FADE_SET_TYPE fade_set_type,
				u8 start_col_r, u8 start_col_g, u8 start_col_b, u8 start_col_a,
				u8 end_col_r, u8 end_col_g, u8 end_col_b, u8 end_col_a,
				float time, BOOL draw_start/*=TRUE*/)
{
#if 1
	IZS_FADE_WORK				*fade_work;
	BOOL						conti_state = FALSE;

	MTM_ASSERT((u32)fade_set_type < IZE_FADE_SET_TYPE_MAX);
	MTM_ASSERT(time > 0);

	if (!iz_fade_tcb) {
		iz_fade_tcb = MTM_TASK_MAKE_TCB(izFadeMain, izFadeDest,
						MTD_TASK_TCB_FLAG_IMMORTAL, pause_level,
						IZD_FADE_TASK_PRIO, group,
						sizeof(IZS_FADE_WORK), "IZ_FADE_SYS");

		fade_work = (IZS_FADE_WORK*)mtTaskGetTcbWork(iz_fade_tcb);
	}
	else {
		fade_work = (IZS_FADE_WORK*)mtTaskGetTcbWork(iz_fade_tcb);

		// 設定引継ぎ
		conti_state = TRUE;
	}

	// ワーク初期化
	IzFadeSetWork(fade_work, dt_prio, draw_state, fade_set_type,
				start_col_r, start_col_g, start_col_b, start_col_a,
				end_col_r, end_col_g, end_col_b, end_col_a,
				time, draw_start, conti_state);

#else
	s32							i;
	IZS_FADE_WORK				*fade_work;
	AMS_PARAM_DRAW_PRIMITIVE	*prim_param;
	NNS_RGBA					start_col;
	u16							save_vtx_no = 1;

	MTM_ASSERT((u32)fade_set_type < IZE_FADE_SET_TYPE_MAX);
	MTM_ASSERT(time > 0);

	if (!iz_fade_tcb) {
		iz_fade_tcb = MTM_TASK_MAKE_TCB(izFadeMain, izFadeDest,
						MTD_TASK_TCB_FLAG_IMMORTAL, pause_level,
						IZD_FADE_TASK_PRIO, group,
						sizeof(IZS_FADE_WORK), "IZ_FADE_SYS");

		fade_work = (IZS_FADE_WORK*)mtTaskGetTcbWork(iz_fade_tcb);
		MI_CpuClear8(fade_work, sizeof(IZS_FADE_WORK));

		// スタートカラー設定
		start_col.r = start_col_r;
		start_col.g = start_col_g;
		start_col.b = start_col_b;
		start_col.a = start_col_a;
	}
	else {
		fade_work = (IZS_FADE_WORK*)mtTaskGetTcbWork(iz_fade_tcb);

		// スタートカラー設定
		if (fade_set_type == IZE_FADE_SET_TYPE_TAKEOEVER) {
			// 以前のフェードカラーを継続
			start_col = fade_work->now_col;		// 退避
			// 現在の設定VTX no
			save_vtx_no = fade_work->vtx_no;	// 退避
		}
		else {
			// 通常
			start_col.r = start_col_r;
			start_col.g = start_col_g;
			start_col.b = start_col_b;
			start_col.a = start_col_a;
		}
	}
	// カウンタクリア
	fade_work->count	= 0.f;

	// スタートカラー
	fade_work->start_col = start_col;
	// エンドカラー
	fade_work->end_col.r = end_col_r;
	fade_work->end_col.g = end_col_g;
	fade_work->end_col.b = end_col_b;
	fade_work->end_col.a = end_col_a;
	// 現在の色をスタートカラーに
	fade_work->now_col = fade_work->start_col;
	// フェード時間設定
	fade_work->time	= time;
	// フェード速度
	fade_work->speed = 1.f;	// ◆fps影響あり
	// 描画タスク設定
	fade_work->dt_prio		= dt_prio;
	fade_work->draw_state	= draw_state;
	// 現在使用中のvtx no
	fade_work->vtx_no		= save_vtx_no;

	// MATRIX設定
	nnMakeUnitMatrix(&fade_work->mtx);

	// 描画開始設定
	fade_work->flag &= ~IZD_FADE_FLAG_DRAW_START;
	if (draw_start) {
		fade_work->flag |= IZD_FADE_FLAG_DRAW_START;
	}
	
	// プリミティブパラメータ設定
	prim_param = &fade_work->prim_param;
	prim_param->vtxPC2D		= &fade_work->vtx[fade_work->vtx_no][0];
	prim_param->mtx			= &fade_work->mtx;
	prim_param->format2D	= NNE_PRIM2D_FMT_PC;
	prim_param->type		= NNE_PRIM_TRIANGLE_STRIP;
	prim_param->count		= IZD_FADE_VTX_NUM;
	prim_param->texlist		= NULL;
	prim_param->texId		= -1;
	prim_param->ablend		= NNE_PRIM_ALPHABLEND_ON;
	prim_param->zOffset		= -1.f;	//100.f;  PS3 Wii では-1.fである必要があるようだ
									// PC XBOX では問題なかった

#if 1
	amDrawGetPrimBlendParam(AMDRAWE_BLENDTYPE_NORMAL, prim_param);
#else
#if _PC | _XBOX
	prim_param->bldSrc	= NNE_BLENDMODE_SRCALPHA;
	prim_param->bldDst	= NNE_BLENDMODE_INVSRCALPHA;
	prim_param->bldMode	= NNE_BLENDOP_ADD;
#elif _PS3 | _IPHONE
	prim_param->bldSrc	= NND_BLENDFUNC_PS3_SRC_ALPHA;
	prim_param->bldDst	= NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA;
	prim_param->bldMode	= NND_BLENDOP_PS3_FUNC_ADD;
#elif _WII
	prim_param->bldSrc	= GX_BL_SRCALPHA;
	prim_param->bldDst	= GX_BL_INVSRCALPHA;
	prim_param->bldMode	= GX_BM_BLEND;
#endif
#endif
	prim_param->aTest = 0;
	prim_param->zMask = 1;
	prim_param->zTest = 0;


	// ポリ設定
	for (i = 0; i < IZD_FADE_SURFACE_SET_NUM; i++) {
#if !_IPHONE
		fade_work->vtx[i][0].Pos.x = 0;
		fade_work->vtx[i][0].Pos.y = 0;
		fade_work->vtx[i][1].Pos.x = 0;
		fade_work->vtx[i][1].Pos.y = AMD_SCREEN_HEIGHT;
		fade_work->vtx[i][2].Pos.x = AMD_SCREEN_WIDTH;
		fade_work->vtx[i][2].Pos.y = 0;
		fade_work->vtx[i][3].Pos.x = AMD_SCREEN_WIDTH;
		fade_work->vtx[i][3].Pos.y = AMD_SCREEN_HEIGHT;
#else
		fade_work->vtx[i][0].Pos.x = 0;
		fade_work->vtx[i][0].Pos.y = 0;
		fade_work->vtx[i][1].Pos.x = AMD_SCREEN_HEIGHT;
		fade_work->vtx[i][1].Pos.y = 0;
		fade_work->vtx[i][2].Pos.x = 0;
		fade_work->vtx[i][2].Pos.y = AMD_SCREEN_WIDTH;
		fade_work->vtx[i][3].Pos.x = AMD_SCREEN_HEIGHT;
		fade_work->vtx[i][3].Pos.y = AMD_SCREEN_WIDTH;
#endif
	}
#endif
}

// ==========================================================================
// IzFadeExit
/*!
 *	フェード終了
 *
 *	@note
 *		IzFadeInitで生成したフェード処理は、自分では終了しませんので、\n
 *		不要になった時点で終了して下さい。
 */
// ==========================================================================
void IzFadeExit(void)
{
	if (iz_fade_tcb) {
		IZS_FADE_WORK		*fade_work;

		fade_work = (IZS_FADE_WORK*)mtTaskGetTcbWork(iz_fade_tcb);

		// 終了待機に切り替え
		mtTaskChangeTcbProcedure(iz_fade_tcb, izFadeEndWaitMain);

		// カウンタクリア
		fade_work->count = 0.f;

		// フェード終了
		iz_fade_tcb = NULL;
	}

}

// ==========================================================================
// IzFadeIsExe
/*!
 *	フェード実行中チェック
 *
 *	@reuturn	TRUE : 実行中
 *
 *	@note
 *		フェード処理が生成されているかどうかで判定します。\n
 *		フェード処理が終了しているかどうかは IzFadeIsEnd で確認して下さい。
 */
// ==========================================================================
BOOL IzFadeIsExe(void)
{
	if (iz_fade_tcb) {
		return (TRUE);
	}
	return (FALSE);
}

// ==========================================================================
// IzFadeIsEnd
/*!
 *	フェード終了チェック
 *
 *	@reuturn	TRUE : 終了 or フェード実行中でない
 */
// ==========================================================================
BOOL IzFadeIsEnd(void)
{
	IZS_FADE_WORK	*fade_work;

	if (iz_fade_tcb) {
		fade_work = (IZS_FADE_WORK*)mtTaskGetTcbWork(iz_fade_tcb);

		if (fade_work->count >= fade_work->time) {
			return (TRUE);
		}
	}
	else {
		return (TRUE);
	}
	return (FALSE);
}

// ==========================================================================
// IzFadeRestoreDrawSetting
/*!
 *	フェードで使用した描画設定を通常設定に復旧する
 *
 *	@note
 *		フェード初期化時にdraw_startをFALSEにした場合、フェードで使用した
 *		描画設定が復旧されません。
 *		ユーザーは必要に応じて、自分で復旧して下さい。
 *		ユーザー描画コマンド内で、IzFadeRestoreDrawSetting を呼び出すと
 *		標準状態に復旧します。
 */
// ==========================================================================
void IzFadeRestoreDrawSetting(void)
{
#if _WII
	GXBool ztest = GX_TRUE;
	GXBool zmask = GX_FALSE;
	GXCompare zfunc = GX_LEQUAL;
#endif

		// αテストしない
	{
#if _PC | _XBOX
		nnSetPrimitive2DAlphaTestDXG20(NNE_FALSE);
#elif _PS3
		nnSetPrimitive2DAlphaFuncPS3(NND_CMPFUNC_PS3_ALWAYS, 0x10);
#elif _WII
		nnSetPrimitive2DAlphaCompareGC(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
#eles _IPHONE
		nnSetPrimitive2DAlphaFuncGL(NND_CMPFUNC_GL_ALWAYS, 0x10);
#endif
	}

	// Zマスク
	{
#if _PC | _XBOX
		// Z書き込みをする
		nnSetPrimitive2DDepthMaskDXG20(NNE_TRUE);
#elif _PS3
		nnSetPrimitive2DDepthMaskPS3(NNE_TRUE);
#elif _WII
		zmask = GX_TRUE;
#elif _IPHONE
		nnSetPrimitive2DDepthMaskGL(NNE_TRUE);
#endif
	}

	// Zテストする
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
	
#if _WII
	nnSetPrimitive2DZModeGC(ztest, zfunc, zmask);
#endif

	// αブレンド
	{
#if _PC | _XBOX
		nnSetPrimitive2DBlendDXG20(NNE_BLENDMODE_SRCALPHA, NNE_BLENDMODE_INVSRCALPHA, NNE_BLENDOP_ADD);
#elif _PS3
		nnSetPrimitive2DBlendPS3(NND_BLENDFUNC_PS3_SRC_ALPHA, NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA, NND_BLENDOP_PS3_FUNC_ADD);
#elif _WII
		nnSetPrimitive2DBlendModeGC(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
#elif _IPHONE
		nnSetPrimitiveBlend(NNE_PRIM_BLEND_ADD);
#endif
	}
}

// ==========================================================================
// IzFadeSetStopUpdate1Frame
/*!
 *	1フレーム更新停止設定
 *
 *	@param	fade_work	[in]	フェードワーク NULLの場合は標準フェードに設定
 */
// ==========================================================================
void IzFadeSetStopUpdate1Frame(IZS_FADE_WORK *fade_work)
{
	if (fade_work == NULL && iz_fade_tcb) {
		fade_work = (IZS_FADE_WORK*)mtTaskGetTcbWork(iz_fade_tcb);
	}

	if (fade_work) {
		fade_work->flag |= IZD_FADE_FLAG_STOP_UPDATE_1FRAME;
	}
}


// ==========================================================================
// IzFadeSetWork
/*!
 *	フェードワーク設定
 *
 *	@param	fade_work		[in]	フェードワーク
 *	@param	dt_prio			[in]	描画処理プライオリティ
 *	@param	draw_state		[in]	描画ステート
 *	@param	fade_set_type	[in]	フェードセットタイプ IZE_FADE_SET_TYPE
 *	@param	start_col_r		[in]	開始カラー値 RED
 *	@param	start_col_g		[in]	開始カラー値 GREEN	
 *	@param	start_col_b		[in]	開始カラー値 BLUE	
 *	@param	start_col_a		[in]	開始カラー値 ALPHA	
 *	@param	end_col_r		[in]	終了カラー値 RED	
 *	@param	end_col_g		[in]	終了カラー値 GREEN			
 *	@param	end_col_b		[in]	終了カラー値 BLUE	
 *	@param	end_col_a		[in]	終了カラー値 RED			
 *	@param	time			[in]	フェード時間 (フレーム)
 *	@param	draw_start		[in]	描画開始処理の有無 (default TRUE)
 *	@param	conti_state		[in]	fade_workの設定の引継ぎあり(default FALSE)
 *
 *	@note
 *		IZS_FADE_WORKを設定します。\n
 *		conti_state==FALSEの場合は、完全初期化、\n
 *		conti_state==TRUEの場合は、以前のステータスを引き継ぎます。\n
 *		初めての設定の場合は、必ずFALSEを設定してください。
 */
// ==========================================================================
void IzFadeSetWork(IZS_FADE_WORK *fade_work, u16 dt_prio, u32 draw_state, IZE_FADE_SET_TYPE fade_set_type,
				u8 start_col_r, u8 start_col_g, u8 start_col_b, u8 start_col_a,
				u8 end_col_r, u8 end_col_g, u8 end_col_b, u8 end_col_a,
				float time, BOOL draw_start/*=TRUE*/, BOOL conti_state/*=FALSE*/)
{
	s32							i;
	AMS_PARAM_DRAW_PRIMITIVE	*prim_param;
	NNS_RGBA					start_col;
	u16							save_vtx_no = 1;

	MTM_ASSERT(fade_work);
	MTM_ASSERT((u32)fade_set_type < IZE_FADE_SET_TYPE_MAX);
	MTM_ASSERT(time > 0);

	if (!conti_state) {
		MI_CpuClear8(fade_work, sizeof(IZS_FADE_WORK));

		// スタートカラー設定
		start_col.r = start_col_r;
		start_col.g = start_col_g;
		start_col.b = start_col_b;
		start_col.a = start_col_a;
	}
	else {
		// スタートカラー設定
		if (fade_set_type == IZE_FADE_SET_TYPE_TAKEOEVER) {
			// 以前のフェードカラーを継続
			start_col = fade_work->now_col;		// 退避
			// 現在の設定VTX no
			save_vtx_no = fade_work->vtx_no;	// 退避
		}
		else {
			// 通常
			start_col.r = start_col_r;
			start_col.g = start_col_g;
			start_col.b = start_col_b;
			start_col.a = start_col_a;
		}
	}
	// カウンタクリア
	fade_work->count	= 0.f;

	// スタートカラー
	fade_work->start_col = start_col;
	// エンドカラー
	fade_work->end_col.r = end_col_r;
	fade_work->end_col.g = end_col_g;
	fade_work->end_col.b = end_col_b;
	fade_work->end_col.a = end_col_a;
	// 現在の色をスタートカラーに
	fade_work->now_col = fade_work->start_col;
	// フェード時間設定
	fade_work->time	= time;
	// フェード速度
	fade_work->speed = 1.f;	// ◆fps影響あり
	// 描画タスク設定
	fade_work->dt_prio		= dt_prio;
	fade_work->draw_state	= draw_state;
	// 現在使用中のvtx no
	fade_work->vtx_no		= save_vtx_no;

	// MATRIX設定
	nnMakeUnitMatrix(&fade_work->mtx);

	// 描画開始設定
	fade_work->flag &= ~IZD_FADE_FLAG_DRAW_START;
	if (draw_start) {
		fade_work->flag |= IZD_FADE_FLAG_DRAW_START;
	}
	
	// プリミティブパラメータ設定
	prim_param = &fade_work->prim_param;
	prim_param->vtxPC2D		= &fade_work->vtx[fade_work->vtx_no][0];
	prim_param->mtx			= &fade_work->mtx;
	prim_param->format2D	= NNE_PRIM2D_FMT_PC;
	prim_param->type		= NNE_PRIM_TRIANGLE_STRIP;
	prim_param->count		= IZD_FADE_VTX_NUM;
	prim_param->texlist		= NULL;
	prim_param->texId		= -1;
	prim_param->ablend		= NNE_PRIM_ALPHABLEND_ON;
	prim_param->zOffset		= -1.f;	//100.f;  PS3 Wii では-1.fである必要があるようだ
									// PC XBOX では問題なかった

	amDrawGetPrimBlendParam(AMDRAWE_BLENDTYPE_NORMAL, prim_param);
	prim_param->aTest = 0;
	prim_param->zMask = 1;
	prim_param->zTest = 0;


	// ポリ設定
	for (i = 0; i < IZD_FADE_SURFACE_SET_NUM; i++) {
#if !_IPHONE
		fade_work->vtx[i][0].Pos.x = 0;
		fade_work->vtx[i][0].Pos.y = 0;
		fade_work->vtx[i][1].Pos.x = 0;
		fade_work->vtx[i][1].Pos.y = AMD_SCREEN_HEIGHT;
		fade_work->vtx[i][2].Pos.x = AMD_SCREEN_WIDTH;
		fade_work->vtx[i][2].Pos.y = 0;
		fade_work->vtx[i][3].Pos.x = AMD_SCREEN_WIDTH;
		fade_work->vtx[i][3].Pos.y = AMD_SCREEN_HEIGHT;
#else
		fade_work->vtx[i][0].Pos.x = 0;
		fade_work->vtx[i][0].Pos.y = 0;
		fade_work->vtx[i][1].Pos.x = AMD_SCREEN_HEIGHT;
		fade_work->vtx[i][1].Pos.y = 0;
		fade_work->vtx[i][2].Pos.x = 0;
		fade_work->vtx[i][2].Pos.y = AMD_SCREEN_WIDTH;
		fade_work->vtx[i][3].Pos.x = AMD_SCREEN_HEIGHT;
		fade_work->vtx[i][3].Pos.y = AMD_SCREEN_WIDTH;
#endif
	}
}

// ==========================================================================
// IzFadeUpdate
/*!
 *	フェード更新
 *
 *	@param	fade_work	[in]	フェードワーク
 */
// ==========================================================================
void IzFadeUpdate(IZS_FADE_WORK *fade_work)
{
	s32					i;
	float				per;
	u8					r, g, b, a;
	NNS_PRIM2D_PC		*vtx;
	
	MTM_ASSERT(fade_work);

	if (fade_work->flag & IZD_FADE_FLAG_STOP_UPDATE_1FRAME) {
		fade_work->flag &= ~IZD_FADE_FLAG_STOP_UPDATE_1FRAME;
	}
	else {
		fade_work->count += fade_work->speed;
		if (fade_work->count > fade_work->time) {
			fade_work->count = fade_work->time;
		}
	}

	// カラー計算
	per = fade_work->count / fade_work->time;
	fade_work->now_col.a = (fade_work->start_col.a * (1.f - per) + fade_work->end_col.a * per);
	fade_work->now_col.r = (fade_work->start_col.r * (1.f - per) + fade_work->end_col.r * per);
	fade_work->now_col.g = (fade_work->start_col.g * (1.f - per) + fade_work->end_col.g * per);
	fade_work->now_col.b = (fade_work->start_col.b * (1.f - per) + fade_work->end_col.b * per);

	r = (u8)nnRoundOff(fade_work->now_col.r + 0.5f);
	g = (u8)nnRoundOff(fade_work->now_col.g + 0.5f);
	b = (u8)nnRoundOff(fade_work->now_col.b + 0.5f);
	a = (u8)nnRoundOff(fade_work->now_col.a + 0.5f);

	// 使用するvtxを取得
	fade_work->vtx_no++;
	if (fade_work->vtx_no >= IZD_FADE_SURFACE_SET_NUM) {
		fade_work->vtx_no = 0;
	}
	vtx = &fade_work->vtx[fade_work->vtx_no][0];

	// カラー設定
	for (i = 0; i < IZD_FADE_VTX_NUM; i++, vtx++) {
		vtx->Col = IZM_FADE_COL_PAC(r, g, b, a);
	}
}

// ==========================================================================
// IzFadeDraw
/*!
 *	フェード描画
 *
 *	@param	fade_work	[in]	フェードワーク
 */
// ==========================================================================
void IzFadeDraw(IZS_FADE_WORK *fade_work)
{
	IZS_FADE_DT_WORK	*dt_work;

	MTM_ASSERT(fade_work);

	// 描画
	fade_work->prim_param.vtxPC2D = &fade_work->vtx[fade_work->vtx_no][0];
	amDrawPrim2D(fade_work->draw_state, &fade_work->prim_param);

	if (fade_work->flag & IZD_FADE_FLAG_DRAW_START) {
		// 描画タスク生成
		dt_work = (IZS_FADE_DT_WORK *)amDrawMallocDataBuffer(sizeof(IZS_FADE_DT_WORK));
		dt_work->draw_state	= fade_work->draw_state;
		dt_work->drawflag	= 0;//fade_work->drawflag;

		amDrawMakeTask(izFadeDrawStart_DT, fade_work->dt_prio, (void*)&dt_work/*8byteまで*/);
	}
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// izFadeDest
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void izFadeDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	//iz_fade_tcb = NULL;
	// Exit呼び出し時にクリアする
}

// ==========================================================================
// izFadeMain
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void izFadeMain(MTS_TASK_TCB *tcb)
{
#if 1
	IZS_FADE_WORK		*fade_work;

	fade_work = (IZS_FADE_WORK*)mtTaskGetTcbWork(tcb);

	// フェード更新
	IzFadeUpdate(fade_work);

	// 描画
	IzFadeDraw(fade_work);
#else
	s32					i;
	IZS_FADE_WORK		*fade_work;
	float				per;
	u8					r, g, b, a;
	NNS_PRIM2D_PC		*vtx;
	IZS_FADE_DT_WORK	*dt_work;
	
	fade_work = (IZS_FADE_WORK*)mtTaskGetTcbWork(tcb);

	fade_work->count += fade_work->speed;
	if (fade_work->count > fade_work->time) {
		fade_work->count = fade_work->time;
	}

	// カラー計算
	per = fade_work->count / fade_work->time;
	fade_work->now_col.a = (fade_work->start_col.a * (1.f - per) + fade_work->end_col.a * per);
	fade_work->now_col.r = (fade_work->start_col.r * (1.f - per) + fade_work->end_col.r * per);
	fade_work->now_col.g = (fade_work->start_col.g * (1.f - per) + fade_work->end_col.g * per);
	fade_work->now_col.b = (fade_work->start_col.b * (1.f - per) + fade_work->end_col.b * per);

	r = (u8)nnRoundOff(fade_work->now_col.r + 0.5f);
	g = (u8)nnRoundOff(fade_work->now_col.g + 0.5f);
	b = (u8)nnRoundOff(fade_work->now_col.b + 0.5f);
	a = (u8)nnRoundOff(fade_work->now_col.a + 0.5f);

	// 使用するvtxを取得
	fade_work->vtx_no++;
	if (fade_work->vtx_no >= IZD_FADE_SURFACE_SET_NUM) {
		fade_work->vtx_no = 0;
	}
	vtx = &fade_work->vtx[fade_work->vtx_no][0];

	// カラー設定
	for (i = 0; i < IZD_FADE_VTX_NUM; i++, vtx++) {
		vtx->Col = IZM_FADE_COL_PAC(r, g, b, a);
	}

	// 描画
	fade_work->prim_param.vtxPC2D = &fade_work->vtx[fade_work->vtx_no][0];
	amDrawPrim2D(fade_work->draw_state, &fade_work->prim_param);

	if (fade_work->flag & IZD_FADE_FLAG_DRAW_START) {
		// 描画タスク生成
		dt_work = (IZS_FADE_DT_WORK *)amDrawMallocDataBuffer(sizeof(IZS_FADE_DT_WORK));
		dt_work->draw_state	= fade_work->draw_state;
		dt_work->drawflag	= 0;//fade_work->drawflag;

		amDrawMakeTask(izFadeDrawStart_DT, fade_work->dt_prio, (void*)&dt_work/*8byteまで*/);
	}
#endif
}

// ==========================================================================
// izFadeEndWaitMain
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void izFadeEndWaitMain(MTS_TASK_TCB *tcb)
{
	IZS_FADE_WORK		*fade_work;

	fade_work = (IZS_FADE_WORK*)mtTaskGetTcbWork(tcb);

	fade_work->count++;

	if (fade_work->count > 1.f) {
		// フェード終了
		mtTaskClearTcb(tcb);
	}

}

// ==========================================================================
// izFadeDrawStart_DT
/*!
 *	@param	tcb	[in]	AMS_TCB
 */
// ==========================================================================
void izFadeDrawStart_DT(AMS_TCB *am_tcb)
{
	IZS_FADE_DT_WORK	*dt_work;

	dt_work = *(IZS_FADE_DT_WORK**)amTaskGetWork(am_tcb);

	// カメラ設定
	AoActDrawPre();

	// 描画
	amDrawExecCommand(dt_work->draw_state, dt_work->drawflag);

	// シーン描画終了(半透明描画開始)
	amDrawEndScene();

	// 描画ステートを戻す
	IzFadeRestoreDrawSetting();
}

// ==========================================================================
// izFadeStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void izFadeStaticVarInit(void)
{
	iz_fade_tcb = NULL;
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
