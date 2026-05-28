// ===========================================================================
/*!
	@file	gmGmkSteamPipe.cpp
	@brief	ギミック スチームパイプ＠ゾーン４工場

	@author	ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkSteamPipe.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
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
#include "gmPlySpec.h"
#include "gmPlySeq.h"
#include "gmObjDef.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmEffectCmn.h"
#include "gmPadVib.h"

#include "gmSound.h"

#include "gmGmkSteamPipe.h"

// データヘッダ
#include "common/model/GMK_STEAMPIPE_MDL.HMB"
#include "common/model/GMK_STEAMPIPE_F_MDL.HMB"




// ----- Struct Definitions --------------------------------------（型の宣言）
// -----

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// -----

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static void gmGmkSteamPipe_GateOutColSet(OBS_OBJECT_WORK *obj_work);
static void gmGmkSteamPipe_GateOutColClear(OBS_OBJECT_WORK *obj_work);
// -----

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -----

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_steampipe_obj_3d_list = NULL;

// ----- Macros ------------------------------------------------（マクロ定義）
#define	OBJWORK		COMWORK.obj_work
// -----

// ----- Macro Functions -----------------------------------（処理マクロ定義）
#define	chgf(f)		ppFunc = (f)
// -----

// ----- Definitions -------------------------------------------（定数の宣言）

//GMS_EVE_RECORD_EVENT::flag
#define GMD_GMK_STMP_EVE_FLAG_OUT_L	(0x01)				// パイプ出口は左向き
#define GMD_GMK_STMP_EVE_FLAG_NOCOL	(0x02)				// パイプ出口に地形コリジョンを付けない


#define GMD_GMK_STEAMPIPE_USE_DIR_MODEL	(1)	//向き別にモデルを変更する



#define	GMD_GMK_STEAMPIPE_SPEED			(fx32)(2.0)		//	引きずられ速度
#define	GMD_GMK_STEAMPIPE_COL_HEIGHT		(8)			//	適当に0.5ブロック
#define	GMD_GMK_STEAMPIPE_COL_OFST_Y		(-16)		//	中心から上面まで

#define	GMD_GMK_STEAMPIPE_MOVE_SPEED		(GMD_PL_DEF_MAX_SPD)

#define GMD_GMK_STEAMPIPE_VIB_TIME		(60.f)			// 発動時振動時間

typedef enum tag_GME_GMK_TYPE{

	GME_GMK_PIPE_TYPE_C_LU = 0,	// クランク┌
	GME_GMK_PIPE_TYPE_C_RU,		// クランク┐
	GME_GMK_PIPE_TYPE_C_RD,		// クランク┘
	GME_GMK_PIPE_TYPE_C_LD,		// クランク└

	GME_GMK_PIPE_TYPE_GR,		// 入り口右
	GME_GMK_PIPE_TYPE_GL,		// 入り口左
	GME_GMK_PIPE_TYPE_E,		// 出口
		GME_GMK_PIPE_TYPE_ER = GME_GMK_PIPE_TYPE_E,
								// 右
		GME_GMK_PIPE_TYPE_EL,
								// 左

	GME_GMK_PIPE_TYPE_H,		// 横
	GME_GMK_PIPE_TYPE_V,		// 縦

	GME_GMK_PIPE_TYPE_MAX

}GME_GMK_TYPE;
#define	GME_GMK_PIPE_TYPE_C 	(GME_GMK_PIPE_TYPE_C_LD+1)

#define	GMD_GMK_PIPE_VECT_LEFT		0x0001	// クランク ┌/└
#define	GMD_GMK_PIPE_VECT_RIGHT		0x0002	// クランク ┐/┘
#define	GMD_GMK_PIPE_VECT_DOWN		0x0004	// クランク ┘/└
#define	GMD_GMK_PIPE_VECT_UP		0x0008	// クランク ┐/┌



// ===========================================================================
// gmGmkSteamPipe*
/*!
	ギミック スチームパイプ＠ゾーン４工場

	@note
 */
// ---------------------------------------------------------------------------

// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分

//●企画要望により設定に手間の掛かるZdepth制御は機能OFFとし
//  eve_rec->left の値を単純な優先値として制御するよう変更しました。@kuramoto 09.12.02
//static fx32	gmk_steampipe_Zdepth;

// ----- Struct Definitions --------------------------------------（型の宣言）
//! スチームパイプ＠ゾーン４工場
typedef struct tag_GMS_GMK_STEAMP_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	GME_GMK_TYPE		obj_type;
	fx32				zdepth;

	GMS_PLAYER_WORK		*ply_work;
	s16					timer;
	u8					status;

}GMS_GMK_STEAMP_WORK;
#define	OBJ_3D		gmk_work.obj_3d
#define	COMWORK		gmk_work.ene_com
// ---------------------------------------------------------------------------












// --------------------------------------------------------------------------
// gmGmkSteamPipe_ppOut
/*!
	ギミック スチームパイプ＠ゾーン４工場
		重なり部分に対応して全体の優先を前後させる
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//static void gmGmkSteamPipe_ppOut(OBS_OBJECT_WORK *obj_work)
//{
//	fx32 z = obj_work->pos.z;
//
//	obj_work->pos.z -= gmk_steampipe_Zdepth;
//	ObjDrawActionSummary(obj_work);
//	obj_work->pos.z = z;
//}
// --------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// gmGmkSteamPipeHit
/*!
	ギミック スチームパイプ パイプ通過

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/* 使わなかった
static void gmGmkSteamPipeHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	OBS_OBJECT_WORK *obj_work = mine_rect->parent_obj;
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	//	一応プレイヤーが当たったかどうかを確認
	if( ply_work == g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P] )
	{
		GMS_GMK_STEAMP_WORK *pwork = (GMS_GMK_STEAMP_WORK*)obj_work;
		//	プレイヤーの速度が自分の向きにあっているか確認
		if( (ply_work->obj_work.spd.x != 0 && pwork->obj_type == GME_GMK_PIPE_TYPE_H)||
		      (ply_work->obj_work.spd.y != 0 && pwork->obj_type == GME_GMK_PIPE_TYPE_V) )
		{
			obj_work->flag |= OBD_OBJECT_NOHIT;		// 矩形あたり無効
		}
		// ヒットしなかった事にする
		ObjRectFuncNoHit(mine_rect, match_rect);
	}
}
*/
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// gmGmkSteamGateHit
/*!
	ギミック スチームパイプ ゲート通過

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSteamGateHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	OBS_OBJECT_WORK *obj_work = mine_rect->parent_obj;
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	//	一応プレイヤーが当たったかどうかを確認
	if( ply_work == g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P] )
	{
		if (!(ply_work->player_flag & GMD_PLF_DIE)) {					// 死亡中
			GMS_GMK_STEAMP_WORK *pwork = (GMS_GMK_STEAMP_WORK*)obj_work;

			ply_work->obj_work.pos.y = obj_work->pos.y;
			GmPlySeqInitSteamPipeIn(ply_work);
			pwork->status = 1;		//	発動
			pwork->ply_work = ply_work;
			obj_work->flag |= OBD_OBJECT_NOHIT;		// 矩形あたり無効
			// 振動
			GMM_PAD_VIB_SMALL_TIME(GMD_GMK_STEAMPIPE_VIB_TIME);
		}
	}
	// ヒットしなかった事にする
	mine_rect->flag &= ~(OBD_RECT_DAMAGE | OBD_RECT_FRAMEHIT | OBD_RECT_FRAMEOUT);
}
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// gmGmkSteamExitHit
/*!
	ギミック スチームパイプ 出口

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSteamExitHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	OBS_OBJECT_WORK *obj_work = mine_rect->parent_obj;
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	//	一応プレイヤーが当たったかどうかを確認
	if( ply_work == g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P] &&
	       ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_STEAMPIPE )
	{
		if (!(ply_work->player_flag & GMD_PLF_DIE)) {					// 死亡中
			GMS_GMK_STEAMP_WORK *pwork = (GMS_GMK_STEAMP_WORK*)obj_work;

			ply_work->obj_work.spd.x = ply_work->obj_work.spd.y = 0;

			pwork->status = 1;
			pwork->ply_work = ply_work;
			obj_work->flag |= OBD_OBJECT_NOHIT;		// 矩形あたり無効
		}
	}
}
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// gmGmkSteamCrankHit
/*!
	ギミック スチームパイプ クランク通過

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static u8 tbl_gmk_pipe_vect[GME_GMK_PIPE_TYPE_C]=
{
	GMD_GMK_PIPE_VECT_LEFT |GMD_GMK_PIPE_VECT_UP,	// クランク┌
	GMD_GMK_PIPE_VECT_RIGHT|GMD_GMK_PIPE_VECT_UP,	// クランク┐
	GMD_GMK_PIPE_VECT_RIGHT|GMD_GMK_PIPE_VECT_DOWN,	// クランク┘
	GMD_GMK_PIPE_VECT_LEFT |GMD_GMK_PIPE_VECT_DOWN,	// クランク└
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if 0	// 処理優先の問題で、ヒットコールバックでプレイヤー座標を処理するとエフェクトがパイプからはみ出す。
		// gmGmkSteamCrankCheck にて、タスク処理から制御する方式に変更
static void gmGmkSteamCrankHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	OBS_OBJECT_WORK *obj_work = mine_rect->parent_obj;
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	//	一応プレイヤーが当たったかどうかを確認
	if( ply_work == g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P] &&
	       ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_STEAMPIPE )
	{
		GMS_GMK_STEAMP_WORK *pwork = (GMS_GMK_STEAMP_WORK*)obj_work;
		//	プレイヤーの速度を確認して曲がり角を越えたかチェックする
		fx32 spd;
		fx32 oldpos;
		if( ply_work->obj_work.spd.x != 0 )
		{
			// 横に移動中
			spd = ply_work->obj_work.spd.x;
			oldpos = ply_work->obj_work.pos.x-spd;
			if(
				(spd > 0 && tbl_gmk_pipe_vect[pwork->obj_type]&GMD_GMK_PIPE_VECT_RIGHT &&
				   oldpos <= obj_work->pos.x && obj_work->pos.x <= ply_work->obj_work.pos.x )||	//	右へ移動中
				(spd < 0 && tbl_gmk_pipe_vect[pwork->obj_type]&GMD_GMK_PIPE_VECT_LEFT &&
				   ply_work->obj_work.pos.x <= obj_work->pos.x && obj_work->pos.x <= oldpos ))	//	左へ移動中
			{
				//	更新
				oldpos = ply_work->obj_work.pos.x - obj_work->pos.x;
				ply_work->obj_work.pos.x = obj_work->pos.x;
				ply_work->obj_work.spd.x = 0;
				ply_work->obj_work.pos.y = obj_work->pos.y;
				if( !(tbl_gmk_pipe_vect[pwork->obj_type]&GMD_GMK_PIPE_VECT_UP) )	//上向き
				{
					ply_work->obj_work.spd.y = -GMD_GMK_STEAMPIPE_MOVE_SPEED;
					ply_work->obj_work.pos.y -= MTM_MATH_ABS(oldpos);
				}
				else	//	下向き
				{
					ply_work->obj_work.spd.y = +GMD_GMK_STEAMPIPE_MOVE_SPEED;
					ply_work->obj_work.pos.y += MTM_MATH_ABS(oldpos);
				}
				//	Ｚ深度情報を書き換え
//				gmk_steampipe_Zdepth = (fx32)(obj_work->user_timer);
				obj_work->flag |= OBD_OBJECT_NOHIT;		// 矩形あたり無効

				// SE
				GmSoundPlaySE("PipeMoving");
			}
		}
		else
		{
			// 縦に移動中
			spd = ply_work->obj_work.spd.y;
			oldpos = ply_work->obj_work.pos.y-spd;
			if(
				(spd > 0 && tbl_gmk_pipe_vect[pwork->obj_type]&GMD_GMK_PIPE_VECT_DOWN &&
				   oldpos <= obj_work->pos.y && obj_work->pos.y <= ply_work->obj_work.pos.y )||	//	下へ移動中
				(spd < 0 && tbl_gmk_pipe_vect[pwork->obj_type]&GMD_GMK_PIPE_VECT_UP &&
				   ply_work->obj_work.pos.y <= obj_work->pos.y && obj_work->pos.y <= oldpos ))	//	左へ移動中
			{
				//	更新
				oldpos = ply_work->obj_work.pos.y - obj_work->pos.y;
				ply_work->obj_work.pos.y = obj_work->pos.y;
				ply_work->obj_work.spd.y = 0;
				ply_work->obj_work.pos.x = obj_work->pos.x;
				if( !(tbl_gmk_pipe_vect[pwork->obj_type]&GMD_GMK_PIPE_VECT_RIGHT) )	//右向き
				{
					ply_work->obj_work.spd.x = +GMD_GMK_STEAMPIPE_MOVE_SPEED;
					ply_work->obj_work.pos.x += MTM_MATH_ABS(oldpos);
				}
				else	//	左向き
				{
					ply_work->obj_work.spd.x = -GMD_GMK_STEAMPIPE_MOVE_SPEED;
					ply_work->obj_work.pos.x -= MTM_MATH_ABS(oldpos);
				}
				//	Ｚ深度情報を書き換え
//				gmk_steampipe_Zdepth = (fx32)(obj_work->user_timer);
				obj_work->flag |= OBD_OBJECT_NOHIT;		// 矩形あたり無効

				// SE
				GmSoundPlaySE("PipeMoving");
			}
		}
	}
	// ヒットはしなかった事にする
	ObjRectFuncNoHit(mine_rect, match_rect);
}
#endif
// ---------------------------------------------------------------------------
static void gmGmkSteamCrankCheck(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	if (  (ply_work->obj_work.pos.x < (obj_work->pos.x + FX32_ONE * 16))
		&&(ply_work->obj_work.pos.x > (obj_work->pos.x - FX32_ONE * 16))
		&&(ply_work->obj_work.pos.y < (obj_work->pos.y + FX32_ONE * 16))
		&&(ply_work->obj_work.pos.y > (obj_work->pos.y - FX32_ONE * 16)) ) {
		// 一定距離内にソニックがいたらチェック対象

		if (  (ply_work->player_flag & GMD_PLF_DIE)		// 死亡中
			||(ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_STEAMPIPE) ) {
			// 死亡＆スチームパイプ以外は無視
			return;
		}

		if (obj_work->user_flag) {
			// 既にクランク通過判定処理済み
			return;
		}

		GMS_GMK_STEAMP_WORK *pwork = (GMS_GMK_STEAMP_WORK*)obj_work;
		//	プレイヤーの速度を確認して曲がり角を越えたかチェックする
		fx32 spd;
		fx32 oldpos;
		if( ply_work->obj_work.spd.x != 0 )
		{
			// 横に移動中
			spd = ply_work->obj_work.spd.x;
			oldpos = ply_work->obj_work.pos.x-spd;
			if(
				(spd > 0 && tbl_gmk_pipe_vect[pwork->obj_type]&GMD_GMK_PIPE_VECT_RIGHT &&
				   oldpos <= obj_work->pos.x && obj_work->pos.x <= ply_work->obj_work.pos.x )||	//	右へ移動中
				(spd < 0 && tbl_gmk_pipe_vect[pwork->obj_type]&GMD_GMK_PIPE_VECT_LEFT &&
				   ply_work->obj_work.pos.x <= obj_work->pos.x && obj_work->pos.x <= oldpos ))	//	左へ移動中
			{
				//	更新
				oldpos = ply_work->obj_work.pos.x - obj_work->pos.x;
				ply_work->obj_work.pos.x = obj_work->pos.x;
				ply_work->obj_work.spd.x = 0;
				ply_work->obj_work.pos.y = obj_work->pos.y;
				if( !(tbl_gmk_pipe_vect[pwork->obj_type]&GMD_GMK_PIPE_VECT_UP) )	//上向き
				{
					ply_work->obj_work.spd.y = -GMD_GMK_STEAMPIPE_MOVE_SPEED;
					ply_work->obj_work.pos.y -= MTM_MATH_ABS(oldpos);
				}
				else	//	下向き
				{
					ply_work->obj_work.spd.y = +GMD_GMK_STEAMPIPE_MOVE_SPEED;
					ply_work->obj_work.pos.y += MTM_MATH_ABS(oldpos);
				}
				//	Ｚ深度情報を書き換え
//				gmk_steampipe_Zdepth = (fx32)(obj_work->user_timer);
				obj_work->flag |= OBD_OBJECT_NOHIT;		// 矩形あたり無効

				// SE
				GmSoundPlaySE("PipeMoving");
				obj_work->user_flag = TRUE;				// 判定済みフラグ
			}
		}
		else
		{
			// 縦に移動中
			spd = ply_work->obj_work.spd.y;
			oldpos = ply_work->obj_work.pos.y-spd;
			if(
				(spd > 0 && tbl_gmk_pipe_vect[pwork->obj_type]&GMD_GMK_PIPE_VECT_DOWN &&
				   oldpos <= obj_work->pos.y && obj_work->pos.y <= ply_work->obj_work.pos.y )||	//	下へ移動中
				(spd < 0 && tbl_gmk_pipe_vect[pwork->obj_type]&GMD_GMK_PIPE_VECT_UP &&
				   ply_work->obj_work.pos.y <= obj_work->pos.y && obj_work->pos.y <= oldpos ))	//	左へ移動中
			{
				//	更新
				oldpos = ply_work->obj_work.pos.y - obj_work->pos.y;
				ply_work->obj_work.pos.y = obj_work->pos.y;
				ply_work->obj_work.spd.y = 0;
				ply_work->obj_work.pos.x = obj_work->pos.x;
				if( !(tbl_gmk_pipe_vect[pwork->obj_type]&GMD_GMK_PIPE_VECT_RIGHT) )	//右向き
				{
					ply_work->obj_work.spd.x = +GMD_GMK_STEAMPIPE_MOVE_SPEED;
					ply_work->obj_work.pos.x += MTM_MATH_ABS(oldpos);
				}
				else	//	左向き
				{
					ply_work->obj_work.spd.x = -GMD_GMK_STEAMPIPE_MOVE_SPEED;
					ply_work->obj_work.pos.x -= MTM_MATH_ABS(oldpos);
				}
				//	Ｚ深度情報を書き換え
//				gmk_steampipe_Zdepth = (fx32)(obj_work->user_timer);
				obj_work->flag |= OBD_OBJECT_NOHIT;		// 矩形あたり無効

				// SE
				GmSoundPlaySE("PipeMoving");
				obj_work->user_flag = TRUE;				// 判定済みフラグ
			}
		}
	} else {
		// 一定範囲外に出たら再度チェック許可
		obj_work->user_flag = FALSE;
	}
}
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// gmGmkSteamPipe*
/*!
	ギミック スチームパイプ＠ゾーン４工場

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSteamPipeStay(OBS_OBJECT_WORK *obj_work);
// ---------------------------------------------------------------------------




// ---------------------------------------------------------------------------
// gmGmkSteamPipeStay
/*!
	ギミック スチームパイプ＠ゾーン４工場 待機

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSteamPipeStay_100(OBS_OBJECT_WORK *obj_work);
static void gmGmkSteamPipeStay_Exit(OBS_OBJECT_WORK *obj_work);
static void gmGmkSteamPipe_GateIn(OBS_OBJECT_WORK *obj_work);
static void gmGmkSteamPipe_GateOut(OBS_OBJECT_WORK *obj_work);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSteamPipeStay(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STEAMP_WORK *pwork = (GMS_GMK_STEAMP_WORK*)obj_work;

	switch( pwork->obj_type )
	{
		case GME_GMK_PIPE_TYPE_GR:		// 入口
		case GME_GMK_PIPE_TYPE_GL:
			obj_work->chgf(gmGmkSteamPipeStay_100);
			break;

		case GME_GMK_PIPE_TYPE_ER:		// 出口
		case GME_GMK_PIPE_TYPE_EL:
			obj_work->chgf(gmGmkSteamPipeStay_Exit);
			break;

		case GME_GMK_PIPE_TYPE_C_LU:	// クランク
		case GME_GMK_PIPE_TYPE_C_RU:
		case GME_GMK_PIPE_TYPE_C_RD:
		case GME_GMK_PIPE_TYPE_C_LD:
			gmGmkSteamCrankCheck(obj_work);
			break;

		case GME_GMK_PIPE_TYPE_H:
		case GME_GMK_PIPE_TYPE_V:
		case GME_GMK_PIPE_TYPE_MAX:
		default:
			obj_work->chgf(NULL);	//	処理ナシ
			break;
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSteamPipeStay_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STEAMP_WORK *pwork = (GMS_GMK_STEAMP_WORK*)obj_work;

	if( pwork->status )
	{
		switch( pwork->obj_type )
		{
			case GME_GMK_PIPE_TYPE_GR:
			case GME_GMK_PIPE_TYPE_GL:
				pwork->timer = 60;
				obj_work->chgf(gmGmkSteamPipe_GateIn);
				// SE
				GmSoundPlaySE("PipeIn");
				break;

			case GME_GMK_PIPE_TYPE_ER:
			case GME_GMK_PIPE_TYPE_EL:
				pwork->timer = 0;
				gmGmkSteamPipe_GateOutColClear(obj_work);					// 排出の間だけコリジョン無し
				obj_work->chgf(gmGmkSteamPipe_GateOut);
				break;

			case GME_GMK_PIPE_TYPE_C_LU:
			case GME_GMK_PIPE_TYPE_C_RU:
			case GME_GMK_PIPE_TYPE_C_RD:
			case GME_GMK_PIPE_TYPE_C_LD:
			case GME_GMK_PIPE_TYPE_H:
			case GME_GMK_PIPE_TYPE_V:
			case GME_GMK_PIPE_TYPE_MAX:
				break;
		}
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSteamPipeStay_Exit(OBS_OBJECT_WORK *obj_work)
{
//	GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
//
//	if( ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_STEAMPIPE )
//		obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z+16*FX32_ONE;
//	else
//		obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z-16*FX32_ONE;

	gmGmkSteamPipeStay_100(obj_work);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSteamPipe_GateIn(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STEAMP_WORK *pwork = (GMS_GMK_STEAMP_WORK*)obj_work;

	pwork->timer -= 1;
	if( pwork->timer <= 0 )
	{
		fx32 spd_x;
		if( pwork->obj_type == GME_GMK_PIPE_TYPE_GR )
			spd_x = +GMD_GMK_STEAMPIPE_MOVE_SPEED;
		else
			spd_x = -GMD_GMK_STEAMPIPE_MOVE_SPEED;

		pwork->ply_work->obj_work.move_flag |= OBD_MOVE_JUMP;	// ジャンプ状態に
		GmPlySeqGmkSpdSet(pwork->ply_work, spd_x, 0);
		// バリア, 無敵エフェクト表示OFF	// 20091126 Dimps Ishizaki
		pwork->ply_work->gmk_flag2 |= GMD_PLGF2_BARRIER_DISP_OFF | GMD_PLGF2_INVINCIBLE_DISP_OFF;
		// SE
		GmSoundPlaySE("PipeMoving");

		obj_work->chgf(NULL);	//	処理なしへ
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSteamPipe_GateOut_100(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSteamPipe_GateOut(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STEAMP_WORK *pwork = (GMS_GMK_STEAMP_WORK*)obj_work;

	pwork->timer -= 1;
	if( pwork->timer <= 0 )
	{
		OBS_OBJECT_WORK *eff_obj =
				(OBS_OBJECT_WORK*)GmEfctCmnEsCreate( obj_work, GME_EFCT_CMN_IDX_STEAM_SHOT );
		eff_obj->pos.x = obj_work->pos.x;
		eff_obj->pos.y = obj_work->pos.y;

		fx32 spd_x;
		if( !(obj_work->user_flag & 0x01) )
		{
			spd_x = +GMD_GMK_STEAMPIPE_MOVE_SPEED;
			eff_obj->dir.z = 0x4000;
			eff_obj->pos.x += 56*FX32_ONE;
		}
		else
		{
			spd_x = -GMD_GMK_STEAMPIPE_MOVE_SPEED;
			eff_obj->dir.z = 0xc000;
			eff_obj->pos.x -= 56*FX32_ONE;
		}
		GmPlySeqInitSteamPipeOut(pwork->ply_work, spd_x);
		// SE
		GmSoundPlaySE("PipeOut");

		pwork->timer = 8;	//	プレイヤーが離れる程度待つ
		obj_work->chgf(gmGmkSteamPipe_GateOut_100);
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSteamPipe_GateOut_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STEAMP_WORK *pwork = (GMS_GMK_STEAMP_WORK*)obj_work;

	pwork->timer -= 1;
	if( pwork->timer <= 0 )
	{
		//	射出後は地形になる
		gmGmkSteamPipe_GateOutColSet(obj_work);

		pwork->status = 0;
		obj_work->chgf(gmGmkSteamPipeStay_Exit);
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
//	ギミック スチームパイプ 地形コリジョンのセット(出口用)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSteamPipe_GateOutColSet(OBS_OBJECT_WORK *obj_work)
{
	if (!(((GMS_ENEMY_COM_WORK*)obj_work)->eve_rec->flag & GMD_GMK_STMP_EVE_FLAG_NOCOL)) {
		// コリジョン抑制フラグがONでなければ地形コリジョン生成
		GMS_GMK_STEAMP_WORK *pwork = (GMS_GMK_STEAMP_WORK*)obj_work;

		pwork->COMWORK.col_work.obj_col.obj    = obj_work;
		// 地形矩形設定
		pwork->COMWORK.col_work.obj_col.width  = 64;
		pwork->COMWORK.col_work.obj_col.height = 64;
		pwork->COMWORK.col_work.obj_col.ofst_x = 0;
		pwork->COMWORK.col_work.obj_col.ofst_y = -32;
		pwork->COMWORK.col_work.obj_col.dir = (u16)(0x0000);
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
//	ギミック スチームパイプ 地形コリジョンのクリア
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSteamPipe_GateOutColClear(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STEAMP_WORK *pwork = (GMS_GMK_STEAMP_WORK*)obj_work;
	pwork->COMWORK.col_work.obj_col.obj    = NULL;
}
// ---------------------------------------------------------------------------




// ---------------------------------------------------------------------------
// gmGmkSteamPipeStart
/*!
	ギミック スチームパイプ　サブ初期化

	@note
		
		
		

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const static s16 tbl_gm_gmk_steampipe_rect[GME_GMK_PIPE_TYPE_MAX-2][4] = {

	{  -20,  -20,  +32,  +32 }, //	クランクタイプ┌
	{  -32,  -20,  +20,  +32 }, //	クランクタイプ┐
	{  -32,  -32,  +20,  +20 }, //	クランクタイプ┘
	{  -20,  -32,  +32,  +20 }, //	クランクタイプ└

	{   +8,  -16,  +40,  +16 }, //	入り口右
	{  -40,  -16,   -8,  +16 }, //	入り口左
	{  +48,  -16,  +80,  +16 }, //	出口右
	{  -80,  -16,  -48,  +16 }, //	出口左
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// --------------------------------------------------------------------------
static void gmGmkSteamPipeStart(OBS_OBJECT_WORK *obj_work,GME_GMK_TYPE type)
{
	GMS_GMK_STEAMP_WORK *pwork = (GMS_GMK_STEAMP_WORK*)obj_work;

	if (  (type < GME_GMK_PIPE_TYPE_C)	//	クランク
		||(type >= GME_GMK_PIPE_TYPE_H) )	//	パイプ
	{
//		// 描画変更
//		obj_work->ppOut = gmGmkSteamPipe_ppOut;
//		//	描画優先設定
//		obj_work->pos.z += obj_work->user_timer;
	}
	else
	{
		OBS_RECT_WORK *rect_work;
		// 矩形設定
		// 対プレイヤー
		pwork->COMWORK.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
		pwork->COMWORK.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

		rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];
		rect_work->ppHit = NULL;
//		if( type < GME_GMK_PIPE_TYPE_C )	//	クランク
//		{
//			rect_work->ppDef = gmGmkSteamCrankHit;
//		}
//		else
		if( type < GME_GMK_PIPE_TYPE_E )	//	入り口
		{
			rect_work->ppDef = gmGmkSteamGateHit;

			pwork->COMWORK.col_work.obj_col.obj       = obj_work;
			// 地形矩形設定
			pwork->COMWORK.col_work.obj_col.width  = 32;
			pwork->COMWORK.col_work.obj_col.height = 16;
			pwork->COMWORK.col_work.obj_col.ofst_x = -14;
			pwork->COMWORK.col_work.obj_col.ofst_y = -34;
			pwork->COMWORK.col_work.obj_col.dir = (u16)(0x0000);
		}
		else
		{
			rect_work->ppDef = gmGmkSteamExitHit;
		}
		ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
		ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
		// 被破壊矩形設定
		ObjRectWorkSet(rect_work,
		               tbl_gm_gmk_steampipe_rect[type][0],
		               tbl_gm_gmk_steampipe_rect[type][1],
		               tbl_gm_gmk_steampipe_rect[type][2],
		               tbl_gm_gmk_steampipe_rect[type][3]);
		rect_work->flag |= OBD_RECT_ENABLE;
//		rect_work->flag |= OBD_RECT_OUT;
		obj_work->flag &= ~OBD_OBJECT_NOHIT;							// 矩形あたり



	}
	pwork->obj_type = type;
	pwork->status = 0;
	obj_work->chgf(gmGmkSteamPipeStay);

#if 0	// alpha test
	{ 
		obj_work->disp_flag |= OBD_DISP_DRAWSTATE;			// ユーザー描画ステート反映
		obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_ALPHA;	// アルファ制御あり
		obj_work->obj_3d->draw_state.alpha.alpha = 0.8f;	// 半透明
	}
#endif
}
// --------------------------------------------------------------------------
#undef	OBJ_3D
#undef	COMWORK
// ===========================================================================



// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ===========================================================================
// gmGmkSteamPipeInit
/*!
	ギミック スチームパイプ＠ゾーン４工場 初期化関数
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note

 */
// ===========================================================================
static OBS_OBJECT_WORK* gmGmkSteamPipeInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type, u16 model)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_STEAMP_WORK), "Gmk_SteamPipe");

	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work /*&pwork->gmk_work*/;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_steampipe_obj_3d_list[model],
	                             &gmk_work->obj_3d);

	//	優先設定
//	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z;
//	obj_work->user_timer = (s32)(eve_rec->left*64*FX32_ONE);
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z
						+ (s32)(eve_rec->left*8*FX32_ONE);

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOMOVE|OBD_MOVE_NOCOL;			// 移動無し 地形あたり無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->flag |= OBD_OBJECT_NOHIT;							// 矩形あたり無し◆

	return obj_work;
}
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
OBS_OBJECT_WORK* GmGmkSteamPipeGateRInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, IDB_GMK_STEAMPIPE_MDL_STM_PIPE_GATE_R_ZNO);
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->pos.z += 16*FX32_ONE;								// 入り口はパイプより優先高く

//	gmk_steampipe_Zdepth = 0;
	gmGmkSteamPipeStart(obj_work,GME_GMK_PIPE_TYPE_GR);
	return obj_work;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
OBS_OBJECT_WORK* GmGmkSteamPipeGateLInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;

	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, IDB_GMK_STEAMPIPE_MDL_STM_PIPE_GATE_L_ZNO);
	obj_work->pos.z += 16*FX32_ONE;								// 入り口はパイプより優先高く

//	gmk_steampipe_Zdepth = 0;
	gmGmkSteamPipeStart(obj_work,GME_GMK_PIPE_TYPE_GL);
	return obj_work;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
OBS_OBJECT_WORK* GmGmkSteamPipeGateEInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	u16 model;
	GME_GMK_TYPE lrtype;

	if( !(eve_rec->flag & GMD_GMK_STMP_EVE_FLAG_OUT_L) )	//	→射出
	{
		// 左から右へ排出
		model = IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_O_L_ZNO;
		lrtype = GME_GMK_PIPE_TYPE_ER;
	}
	else
	{
		// 右から左へ排出
		model = IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_O_R_ZNO;
		lrtype = GME_GMK_PIPE_TYPE_EL;
	}
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, model);
//	obj_work->pos.z += FX32_ONE;
	obj_work->pos.z += 32*FX32_ONE;								// 出口はパイプより優先高く
	obj_work->user_flag = eve_rec->flag;
	if( !(obj_work->user_flag&0x01) )
	{
		obj_work->pos.x -= 32*FX32_ONE;
	}
	else
	{
		obj_work->pos.x += 32*FX32_ONE;
	}

	gmGmkSteamPipe_GateOutColSet(obj_work);
	gmGmkSteamPipeStart(obj_work,lrtype);
	return obj_work;
}
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
OBS_OBJECT_WORK* GmGmkSteamPipeA1Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	上から下への画像
	OBS_OBJECT_WORK		*obj_work;
#if	GMD_GMK_STEAMPIPE_USE_DIR_MODEL
	u16 id = IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_A_ZNO;
	if ( GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL ){
		id = IDB_GMK_STEAMPIPE_F_MDL_GMK_STM_PIPE_F_A_ZNO;
	}
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, id);
#else
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_A_ZNO);
#endif	//GMD_GMK_STEAMPIPE_USE_DIR_MODEL

	gmGmkSteamPipeStart(obj_work,GME_GMK_PIPE_TYPE_V);
	return obj_work;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
OBS_OBJECT_WORK* GmGmkSteamPipeA2Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	右から左への画像
	OBS_OBJECT_WORK		*obj_work;
#if	GMD_GMK_STEAMPIPE_USE_DIR_MODEL
	u16 id = IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_A_02_ZNO;
	if ( GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL ){
		id = IDB_GMK_STEAMPIPE_F_MDL_GMK_STM_PIPE_F_2A_ZNO;
	}
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, id);
#else
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_A_ZNO);
	obj_work->dir.z = 0x4000;
#endif	//GMD_GMK_STEAMPIPE_USE_DIR_MODEL

	gmGmkSteamPipeStart(obj_work,GME_GMK_PIPE_TYPE_H);
	return obj_work;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
OBS_OBJECT_WORK* GmGmkSteamPipeA3Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	下から上への画像
	OBS_OBJECT_WORK		*obj_work;
#if	GMD_GMK_STEAMPIPE_USE_DIR_MODEL
	u16 id = IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_A_ZNO;
	if ( GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL ){
		id = IDB_GMK_STEAMPIPE_F_MDL_GMK_STM_PIPE_F_A_ZNO;
	}
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, id);
#else
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_A_ZNO);
	obj_work->dir.z = 0x8000;
#endif	//GMD_GMK_STEAMPIPE_USE_DIR_MODEL

	gmGmkSteamPipeStart(obj_work,GME_GMK_PIPE_TYPE_V);
	return obj_work;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
OBS_OBJECT_WORK* GmGmkSteamPipeA4Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	左から右への画像
	OBS_OBJECT_WORK		*obj_work;
#if	GMD_GMK_STEAMPIPE_USE_DIR_MODEL
	u16 id = IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_A_03_ZNO;
	if ( GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL ){
		id = IDB_GMK_STEAMPIPE_F_MDL_GMK_STM_PIPE_F_2A_ZNO;
	}
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, id);
#else
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_A_ZNO);
	obj_work->dir.z = 0xC000;
#endif	//GMD_GMK_STEAMPIPE_USE_DIR_MODEL

	gmGmkSteamPipeStart(obj_work,GME_GMK_PIPE_TYPE_H);
	return obj_work;
}
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
OBS_OBJECT_WORK* GmGmkSteamPipeB1Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	上から下への画像
	OBS_OBJECT_WORK		*obj_work;
#if	GMD_GMK_STEAMPIPE_USE_DIR_MODEL
	u16 id = IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_B_ZNO;
	if ( GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL ){
		id = IDB_GMK_STEAMPIPE_F_MDL_GMK_STM_PIPE_F_B_ZNO;
	}
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, id);
#else
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_B_ZNO);
#endif	//GMD_GMK_STEAMPIPE_USE_DIR_MODEL

	gmGmkSteamPipeStart(obj_work,GME_GMK_PIPE_TYPE_V);
	return obj_work;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
OBS_OBJECT_WORK* GmGmkSteamPipeB2Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	右から左への画像
	OBS_OBJECT_WORK		*obj_work;
#if	GMD_GMK_STEAMPIPE_USE_DIR_MODEL
	u16 id = IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_B_02_ZNO;
	if ( GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL ){
		id = IDB_GMK_STEAMPIPE_F_MDL_GMK_STM_PIPE_F_2B_ZNO;
	}
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, id);
#else
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_B_ZNO);
	obj_work->dir.z = 0x4000;
#endif	//GMD_GMK_STEAMPIPE_USE_DIR_MODEL

	gmGmkSteamPipeStart(obj_work,GME_GMK_PIPE_TYPE_H);
	return obj_work;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
OBS_OBJECT_WORK* GmGmkSteamPipeB3Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	下から上への画像
	OBS_OBJECT_WORK		*obj_work;
#if	GMD_GMK_STEAMPIPE_USE_DIR_MODEL
	u16 id = IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_B_ZNO;
	if ( GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL ){
		id = IDB_GMK_STEAMPIPE_F_MDL_GMK_STM_PIPE_F_B_ZNO;
	}
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, id);
#else
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_B_ZNO);
	obj_work->dir.z = 0x8000;
#endif	//GMD_GMK_STEAMPIPE_USE_DIR_MODEL

	gmGmkSteamPipeStart(obj_work,GME_GMK_PIPE_TYPE_V);
	return obj_work;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
OBS_OBJECT_WORK* GmGmkSteamPipeB4Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	左から右への画像
	OBS_OBJECT_WORK		*obj_work;
#if	GMD_GMK_STEAMPIPE_USE_DIR_MODEL
	u16 id = IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_B_03_ZNO;
	if ( GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL ){
		id = IDB_GMK_STEAMPIPE_F_MDL_GMK_STM_PIPE_F_2B_ZNO;
	}
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, id);
#else
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_B_ZNO);
	obj_work->dir.z = 0xC000;
#endif	//GMD_GMK_STEAMPIPE_USE_DIR_MODEL

	gmGmkSteamPipeStart(obj_work,GME_GMK_PIPE_TYPE_H);
	return obj_work;
}
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
OBS_OBJECT_WORK* GmGmkSteamPipeJ1Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	右と下 ┌
	OBS_OBJECT_WORK		*obj_work;
#if	GMD_GMK_STEAMPIPE_USE_DIR_MODEL
	u16 id = IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_CRN_ZNO;
	u16 dir = 0;
	if ( GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL ){
		id = IDB_GMK_STEAMPIPE_F_MDL_GMK_STM_PIPE_F_2CRN_ZNO;
		dir = 0x4000;
	}
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, id);
	obj_work->dir.z = dir;
#else
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_CRN_ZNO);
#endif	//GMD_GMK_STEAMPIPE_USE_DIR_MODEL

	gmGmkSteamPipeStart(obj_work,GME_GMK_PIPE_TYPE_C_LU);
	return obj_work;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
OBS_OBJECT_WORK* GmGmkSteamPipeJ2Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	下と左 ┐
	OBS_OBJECT_WORK		*obj_work;
#if	GMD_GMK_STEAMPIPE_USE_DIR_MODEL
	u16 id = IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_CRN_02_ZNO;
	if ( GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL ){
		id = IDB_GMK_STEAMPIPE_F_MDL_GMK_STM_PIPE_F_CRN_ZNO;
	}
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, id);
#else
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_CRN_ZNO);
	obj_work->dir.z = 0x4000;
#endif	//GMD_GMK_STEAMPIPE_USE_DIR_MODEL

	gmGmkSteamPipeStart(obj_work,GME_GMK_PIPE_TYPE_C_RU);
	return obj_work;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
OBS_OBJECT_WORK* GmGmkSteamPipeJ3Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	左と上 ┘
	OBS_OBJECT_WORK		*obj_work;
#if	GMD_GMK_STEAMPIPE_USE_DIR_MODEL
	u16 id = IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_CRN_03_ZNO;
	u16 dir = 0;
	if ( GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL ){
		id = IDB_GMK_STEAMPIPE_F_MDL_GMK_STM_PIPE_F_CRN_ZNO;
		dir = 0x4000;
	}
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, id);
	obj_work->dir.z = dir;
#else
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_CRN_ZNO);
	obj_work->dir.z = 0x8000;
#endif	//GMD_GMK_STEAMPIPE_USE_DIR_MODEL

	gmGmkSteamPipeStart(obj_work,GME_GMK_PIPE_TYPE_C_RD);
	return obj_work;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
OBS_OBJECT_WORK* GmGmkSteamPipeJ4Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	上と右 └
	OBS_OBJECT_WORK		*obj_work;
#if	GMD_GMK_STEAMPIPE_USE_DIR_MODEL
	u16 id = IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_CRN_04_ZNO;
	if ( GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL ){
		id = IDB_GMK_STEAMPIPE_F_MDL_GMK_STM_PIPE_F_2CRN_ZNO;
	}
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, id);
#else
	obj_work = gmGmkSteamPipeInit( eve_rec, pos_x, pos_y, type, IDB_GMK_STEAMPIPE_MDL_GMK_STM_PIPE_CRN_ZNO);
	obj_work->dir.z = 0xC000;
#endif	//GMD_GMK_STEAMPIPE_USE_DIR_MODEL

	gmGmkSteamPipeStart(obj_work,GME_GMK_PIPE_TYPE_C_LD);
	return obj_work;
}
// ===========================================================================



// ===========================================================================
// GmGmkSteamPipeBuild
/*!
	ギミック スチームパイプ＠ゾーン４工場 データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkSteamPipeBuild(void)
{
	gm_gmk_steampipe_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_STEAMPIPE_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_STEAMPIPE_TEX),
								0/*draw_flag*/);
}
// ===========================================================================


// ===========================================================================
// GmGmkSteamPipeFlush
/*!
	ギミック スチームパイプ＠ゾーン４工場 データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkSteamPipeFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_STEAMPIPE_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_steampipe_obj_3d_list, amb->file_num);
}
// ===========================================================================
