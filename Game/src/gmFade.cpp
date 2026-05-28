// ==========================================================================
/*!
  @file gmFade.cpp
  @brief フェードオブジェクト

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmFade.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmTask.h"
#include "gmObj.h"

#include "gmFade.h"

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmFadeDispFunc(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// IzFadeSetWork
/*!
 *	フェードワーク初期化
 *
 *	@param	prio			[in]	フェード処理タスク優先
 *	@param	group			[in]	フェード処理タスクグループ
 *	@param	pause_level		[in]	フェード処理ポーズレベル
 *	@param	work_size		[in]	ワークサイズ sizeof(GMS_FADE_OBJ_WORK) 以上
 *	@param	dt_prio			[in]	描画処理プライオリティ
 *	@param	draw_state		[in]	描画ステート
 *
 *	@note
 *		
 */
// ==========================================================================
GMS_FADE_OBJ_WORK* GmFadeCreateFadeObj(u16 prio, u8 group, u8 pause_level, u32 work_size, u16 dt_prio, u32 draw_state)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_FADE_OBJ_WORK	*fade_obj;

	MTM_ASSERT(work_size >= sizeof(GMS_FADE_OBJ_WORK));

	obj_work = OBM_OBJECT_TASK_DETAIL_INIT(prio, group,
					pause_level, GMD_OBJ_OBJPAUSELEVEL_DEF,
					work_size, "FADE_OBJ");

	fade_obj = (GMS_FADE_OBJ_WORK*)obj_work;

	// システム設定
	obj_work->obj_type = GMD_OBJTYPE_FADE;
	obj_work->flag |= OBD_OBJECT_NOHIT | OBD_OBJECT_NOCLIP;
	obj_work->move_flag |= OBD_MOVE_NOCOL | OBD_MOVE_NOMOVE;

	// 描画プライオリティと描画ステートのみ先に設定
	fade_obj->fade_work.dt_prio		= dt_prio;
	fade_obj->fade_work.draw_state	= draw_state;

	// 初期設定
	fade_obj->fade_work.time = 1.0f;

	// メイン処理
	// なし

	// 描画処理
	fade_obj->obj_work.ppOut = gmFadeDispFunc;

	return (fade_obj);
}

// ==========================================================================
// GmFadeIsEnd
/*!
 *	フェード終了チェック
 *
 *	@reuturn	TRUE : 終了 or フェード実行中でない
 */
// ==========================================================================
BOOL GmFadeIsEnd(GMS_FADE_OBJ_WORK *fade_obj)
{
	if (fade_obj->fade_work.count >= fade_obj->fade_work.time) {
		return (TRUE);
	}
	return (FALSE);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmFadeDispFunc
/*!
 *	フェード描画処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmFadeDispFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_FADE_OBJ_WORK	*fade_obj;

	fade_obj = (GMS_FADE_OBJ_WORK*)obj_work;

	if (!(obj_work->disp_flag & (OBD_DISP_NOUPDATE | OBD_DISP_STOP))) {
		// フェード更新
		IzFadeUpdate(&fade_obj->fade_work);
	}

	if (!(obj_work->disp_flag & (OBD_DISP_NODISP))) {
		// 描画
		IzFadeDraw(&fade_obj->fade_work);
	}
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
