// =======================================================================
/*!
	@file	gmGmkForceSpin.c
	@brief	ギミック 強制スピン＠主にゾーン２

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkForceSpin.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmPlySeqGmk.h"
#include "gmPlayer.h"
#include "gmObjDef.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmPlySeq.h"

#include "gmSound.h"

#include "gmGmkForceSpin.h"




// ----- Struct Definitions --------------------------------------（型の宣言）



// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// -------------

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -------------

// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ----- Macros ------------------------------------------------（マクロ定義）
// -------------

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// -------------

// ----- Definitions -------------------------------------------（定数の宣言）
// eve_rec.flag	// set
#define GMD_GMK_EVE_FLAG_FORCE_SPIN_SPD_DEC			(0x0001)	//!< 減速タイプ		// 20090914 Dimps Ishizaki

// eve_rec.flag	// reset
#define GMD_GMK_EVE_FLAG_FORCE_SPIN_TO_SPIN			(0x0001)	//!< 解除時にスピンにする





// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------



// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分
static void gmGmkForceSpinSetMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkForceSpinResetMain(OBS_OBJECT_WORK *obj_work);
static BOOL gmGmkForceSpinRectChk(OBS_OBJECT_WORK *obj_work, GMS_PLAYER_WORK *ply_work);

// ----- Struct Definitions --------------------------------------（型の宣言）
// ---------------------------------------------------------------------------


// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// GmGmkForceSpinSetInit
/*!
 *	ギミック 強制スピンセット＠主にゾーン２ 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkForceSpinSetInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_COM_WORK	*gmk_work;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);
	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_COM_WORK), "GMK_FORCE_SPIN_SET");
	gmk_work = (GMS_ENEMY_COM_WORK*)obj_work;

    // 矩形設定
	rect_work = &gmk_work->rect_work[GMD_ENEMY_RECT_BODY];
	ObjRectSet(&rect_work->rect,
				eve_rec->left,
				eve_rec->top,
				(s16)(eve_rec->width + eve_rec->left),
				(s16)(eve_rec->height + eve_rec->top) );

	// 個別設定
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_DISP_NODISP;

	// メイン処理
	obj_work->ppFunc = gmGmkForceSpinSetMain;
	return (obj_work);
}

// ==========================================================================
// GmGmkForceSpinResetInit
/*!
 *	ギミック 強制スピン解除＠主にゾーン２ 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkForceSpinResetInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_COM_WORK	*gmk_work;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);
	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_COM_WORK), "GMK_FORCE_SPIN_RESET");
	gmk_work = (GMS_ENEMY_COM_WORK*)obj_work;

    // 矩形設定
	rect_work = &gmk_work->rect_work[GMD_ENEMY_RECT_BODY];
	ObjRectSet(&rect_work->rect,
				eve_rec->left,
				eve_rec->top,
				(s16)(eve_rec->width + eve_rec->left),
				(s16)(eve_rec->height + eve_rec->top) );

	// 個別設定
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_DISP_NODISP;

	// メイン処理
	obj_work->ppFunc = gmGmkForceSpinResetMain;
	return (obj_work);
}

//----- Local Functions ------------------------------------------------
// ==========================================================================
// gmGmkForceSpinSetMain
/*!
 *	ギミック 強制スピンセットメイン関数
 *
 */
// ==========================================================================
void gmGmkForceSpinSetMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_COM_WORK	*gmk_work = (GMS_ENEMY_COM_WORK*)obj_work;
	GMS_PLAYER_WORK		*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	if (  (ply_work->player_flag & GMD_PLF_DIE)								// 死亡
		||(ply_work->obj_work.flag & OBD_OBJECT_NOHIT)						// ヒット判定無し
		||(g_gm_main_system.game_flag & (GMD_GAME_FLAG_TIMEOVER				// タイムオーバー死亡
										| GMD_GAME_FLAG_SPL_TIMEOVER) ) ) {	// スペステ：タイムオーバー
		return;
	}

	if (gmGmkForceSpinRectChk(obj_work, ply_work)) {
		// 矩形内にプレイヤー中心あり
		if (  (ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_FORCESPIN)
			&&(ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_FORCESPIN_DEC)
			&&(ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_FORCESPIN_FALL) ) {
			// 既に強制スピン状態じゃなければソニックを強制スピン状態に変更
			if (gmk_work->eve_rec->flag & GMD_GMK_EVE_FLAG_FORCE_SPIN_SPD_DEC) {
				// 減速タイプ
				GmPlySeqGmkInitForceSpinDec(ply_work);
			}
			else {
				// 通常タイプ
				GmPlySeqGmkInitForceSpin(ply_work);
			}
		}
	}
}

// ==========================================================================
// gmGmkForceSpinSetMain
/*!
 *	ギミック 強制スピン解除メイン関数
 *
 */
// ==========================================================================
void gmGmkForceSpinResetMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_COM_WORK	*gmk_work = (GMS_ENEMY_COM_WORK*)obj_work;
	GMS_PLAYER_WORK		*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	if (  (ply_work->player_flag & GMD_PLF_DIE)								// 死亡
		||(ply_work->obj_work.flag & OBD_OBJECT_NOHIT)						// ヒット判定無し
		||(g_gm_main_system.game_flag & (GMD_GAME_FLAG_TIMEOVER				// タイムオーバー死亡
										| GMD_GAME_FLAG_SPL_TIMEOVER) ) ) {	// スペステ：タイムオーバー
		return;
	}

	if (gmGmkForceSpinRectChk(obj_work, ply_work)) {
		// 矩形内にプレイヤー中心あり
		if (  (ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_FORCESPIN)
			||(ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_FORCESPIN_DEC)
			||(ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_FORCESPIN_FALL) ) {
			// 強制スピン状態ならソニックを強制解除
			if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {
				// 接地状態
				ply_work->no_spddown_timer = 0;
				if (gmk_work->eve_rec->flag & GMD_GMK_EVE_FLAG_FORCE_SPIN_TO_SPIN) {
					// スピンへ移行
					GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SPIN);
				} else {
					// ダッシュへ移行
					GmPlySeqInitFw(ply_work);
				}
			} else {
				// 空中状態
				GmPlySeqGmkInitSpinFall(ply_work, ply_work->obj_work.spd.x, ply_work->obj_work.spd.y);
			}
		}
	}
}

// ==========================================================================
// gmGmkForceSpinRectChk
/*!
 *	ギミック 強制スピン 矩形範囲内にソニック中心が入っているかチェック
 *
 *	@param obj_work	[in] ギミックOBJワーク
 *	@param ply_work	[in] プレイヤーワーク
 *
 *	@return	矩形内にソニックがいればTRUE
 *
 *	@note
 */
// ==========================================================================
BOOL gmGmkForceSpinRectChk(OBS_OBJECT_WORK *obj_work, GMS_PLAYER_WORK *ply_work)
{
	GMS_ENEMY_COM_WORK	*gmk_work = (GMS_ENEMY_COM_WORK*)obj_work;
	OBS_RECT_WORK	*rect_work = &gmk_work->rect_work[GMD_ENEMY_RECT_BODY];

	if (  (ply_work->obj_work.pos.x >= obj_work->pos.x + (fx32)(rect_work->rect.left << FX32_SHIFT))
		&&(ply_work->obj_work.pos.x <= obj_work->pos.x + (fx32)(rect_work->rect.right << FX32_SHIFT))
		&&(ply_work->obj_work.pos.y >= obj_work->pos.y + (fx32)(rect_work->rect.top << FX32_SHIFT))
		&&(ply_work->obj_work.pos.y <= obj_work->pos.y + (fx32)(rect_work->rect.bottom << FX32_SHIFT)) ) {
		return TRUE;
	}
	return FALSE;
}
