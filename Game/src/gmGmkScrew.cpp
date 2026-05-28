// ==========================================================================
/*!
  @file gmGmkCamScrLim.cpp
  @brief ギミック コークスクリュー

  @author K.Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkScrew.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ==========================================================================
/*
 * $Log: $
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gmEnemy.h"
#include "gmMain.h"
#include "gmTask.h"
#include "gmPlySeq.h"
#include "gmPlySeqGmk.h"
#include "gmGmkScrew.h"

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
#define GMD_GMK_SCREW_RECT_LF	(4)
#define GMD_GMK_SCREW_RECT_RT	(16)
#define GMD_GMK_SCREW_RECT_UP	(-8)
#define GMD_GMK_SCREW_RECT_DW	(0)

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkScrewMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkScrewDefFunc(OBS_RECT_WORK* mine_rect, OBS_RECT_WORK* match_rect);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------

// ==========================================================================
// GmGmkScrewInit
/*!
 *	ギミック コークスクリュー 初期化関数
 *
 *	@param	eve_rec	[inout]	レコードポインタ
 *	@param	pos_x	[in]	出現座標
 *	@param	pos_y	[in]
 *	@param	type	[in]	処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkScrewInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_COM_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_COM_WORK), "GMK_SCREW");
	gmk_work = (GMS_ENEMY_COM_WORK*)obj_work;

	// 個別設定
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_DISP_NODISP;	// 移動無し 当たり無し 描画無し

	// 矩形設定
	gmk_work->rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gmk_work->rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	rect_work = &gmk_work->rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppHit = NULL;
	rect_work->ppDef = gmGmkScrewDefFunc;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
//	rect_work->flag |= OBD_RECT_OUT;	// 矩形ヒット後にスピンダッシュ溜め等で加速すると乗れないので常時ヒットで動作するよう処理変更

	if (eve_rec->flag & GMD_GMK_SCREW_EVE_FLAG_LEFT) {
		// 左向き矩形セット
		ObjRectWorkSet(rect_work,
							-GMD_GMK_SCREW_RECT_LF, GMD_GMK_SCREW_RECT_UP,
							-GMD_GMK_SCREW_RECT_RT, GMD_GMK_SCREW_RECT_DW);
	} else {
		// 右向き矩形セット
		ObjRectWorkSet(rect_work,
							GMD_GMK_SCREW_RECT_LF, GMD_GMK_SCREW_RECT_UP,
							GMD_GMK_SCREW_RECT_RT, GMD_GMK_SCREW_RECT_DW);
	}

	/*** メイン処理設定 ***/
	obj_work->ppFunc = gmGmkScrewMain;
	return (obj_work);
}



//----- Local Functions -----------------------------------------------------
// ================================================================
// gmGmkScrewMain
/*!
 *	ギミック コークスクリュー メイン関数
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 */
// ==========================================================================
void gmGmkScrewMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK		*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	if (  (ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_SCREW)
		&&(ply_work->gmk_obj != obj_work) ) {
		// 自身との干渉なくなれば画面外での生存OFF
		obj_work->flag &= ~OBD_OBJECT_NOCLIP;						// 画面外でも生存 をOFF
	}
}
// ================================================================
// gmGmkScrewDefFunc
/*!
  ヒット時処理

	@param	mine_rect		[in]	自分矩形ワークポインタ
	@param	match_rect		[in]	相手矩形ワークポインタ

  @note
	ppDefへ登録
 */
// ================================================================
void gmGmkScrewDefFunc(OBS_RECT_WORK* mine_rect, OBS_RECT_WORK* match_rect)
{
	GMS_ENEMY_COM_WORK	*gmk_work = (GMS_ENEMY_COM_WORK *)mine_rect->parent_obj;
	GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK *)match_rect->parent_obj;

    if ( gmk_work == NULL ) return;
    if ( ply_work == NULL ) return;
	if (ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER) return;
	if (ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_SCREW) return;
	
	fx32	check_spd = ply_work->spd3;
	u16		flag = (u16)(gmk_work->eve_rec->flag);

	// 向きと速度をチェック
	if	(   (  (ply_work->obj_work.spd_m >= check_spd)
		     &&(!(gmk_work->eve_rec->flag & GMD_GMK_SCREW_EVE_FLAG_LEFT) ) )
		 || (  (ply_work->obj_work.spd_m <= -check_spd)
		 	 &&(gmk_work->eve_rec->flag & GMD_GMK_SCREW_EVE_FLAG_LEFT) ) ) {

		if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {
			// ソニック接地時のみ発動
			GmPlySeqInitScrew(ply_work, gmk_work, gmk_work->obj_work.pos.x, gmk_work->obj_work.pos.y, flag);
			gmk_work->obj_work.flag |= OBD_OBJECT_NOCLIP;					// 画面外でも生存
		}
	}
}
// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
