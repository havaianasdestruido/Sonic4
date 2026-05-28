// ==========================================================================
/*!
  @file gmPlySeq.cpp
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmPlySeq.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gmMain.h"
#include "gmObj.h"
#include "gmPlayer.h"
#include "gmPlySeqDat.h"
#include "gmSound.h"
#include "gmPlyEfct.h"
#include "gmEnemy.h"
#include "gmCamera.h"
#include "objCamera.h"
#include "gmPadVib.h"
#include "gmSplStage.h"

#include "gmPlySeq.h"
#include "gmPlySpec.h"

#if _IPHONE
#include "dbgPadEmu.hpp"
#endif //_IPHONE

//mpp -------------------------
#include "mppCheckPointStorage.h"
#include "mppAchievementSupport.h"

//----- Definitions ---------------------------------------------------------
#if defined MTD_DEBUG
//#define GMD_PLY_SEQ_DEBUG_TRUCK_NO_DIR_DIE	// トロッコ角度死亡無し
#endif

#define GMD_PLY_SEQ_T_SLOPEFLY_DEC_PER			((fx32)(FX32_ONE * 0.75))	//!< トロッコ坂道飛び出し時 減速度係数
#define GMD_PLY_SEQ_T_SLOPEFLY_RANDING_ACC_PER	((fx32)(FX32_ONE * 0.5))	//!< トロッコ坂道飛び出し時 接地加速度係数

#if _IPHONE
#define GMD_PLY_SEQ_PLAY_SE_TIMER		(25)	//!< スピンSE(高音)再生待ちタイマー時間
#define GMD_PLY_SEQ_PLAY_BACK_SE_TIMER	(50)	//!< スピンSE(低音)再生待ちタイマー時間
#endif // _IPHONE

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmPlySeqFwMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqWalkMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqTurnMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqLookupMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqLookupEndMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqSquatMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqSquatEndMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqBrakeMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqSpinMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqSpinDashMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqStaggerMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqJumpMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqWallPushMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqHomingMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqHomingRefMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqJumpDashMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqDamageMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqDeathMain(GMS_PLAYER_WORK *ply_work);
/*static*/ void gmPlySeqTransformSuperMain(GMS_PLAYER_WORK *ply_work);

static void gmPlySeqActGoal(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqBossGoalPre(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqBossGoalMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqBoss5DemoPre(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqBoss5DemoMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqTRetryFw(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqTRetryAcc(GMS_PLAYER_WORK *ply_work);
//static void gmPlySeqFallMain(GMS_PLAYER_WORK *ply_work);
// トロッコ
static void gmPlySeqTruckFwMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqTruckWalkMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqTruckJumpMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqTruckSquatMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqTruckStaggerMain(GMS_PLAYER_WORK *ply_work);


static void gmPlySeqTruckMove(OBS_OBJECT_WORK *obj_work);
static void gmPlySeqSplMove(OBS_OBJECT_WORK *obj_work);
static void gmPlySeqSplJumpDirec(GMS_PLAYER_WORK *ply_work);

// シーケンス遷移判定処理
static void gmPlySeqCheckChangeSequence(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckChangeSequenceUserInput(GMS_PLAYER_WORK *ply_work);

//static BOOL gmPlySeqCheckEndSpinDash(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckEndWalk(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckTurn(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckDirectTurn(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckFall(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckStagger(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckEndLookup(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckEndSquat(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckEndSpin(GMS_PLAYER_WORK *ply_work);
//static BOOL gmPlySeqCheckEndBrake(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckEndWallPush(GMS_PLAYER_WORK *ply_work);

static BOOL gmPlySeqCheckHoming(GMS_PLAYER_WORK *ply_work);
//static BOOL gmPlySeqCheckJumpDash(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckSquatSpin(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckSpin(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckSpinDashAcc(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckPinballSpinDashAcc(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckJump(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckBrake(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckWalk(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckLookup(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckSquat(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckWallPush(GMS_PLAYER_WORK *ply_work);

// トロッコ
static BOOL gmPlySeqCheckTruckWalk(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlySeqCheckEndTruckWalk(GMS_PLAYER_WORK *ply_work);

static void gmPlySeqSplStgRollCtrl(GMS_PLAYER_WORK *ply_work);

#if defined (MTD_DEBUG)
static void gmPlySeqDebugMove(GMS_PLAYER_WORK *ply_work);
#endif

//----- Global Variables ----------------------------------------------------
//===========================================================================
// シーケンステーブル
//===========================================================================
// シーケンス初期化処理テーブル
/// ソニック, スーパーソニック, ピンボールソニック
void (*const g_gm_ply_seq_init_tbl_son[GME_PLY_SEQ_STATE_MAX])(GMS_PLAYER_WORK*) = {
	// 通常シーケンス
	GmPlySeqInitFw,				// FW
	GmPlySeqInitWalk,			// 歩き
	GmPlySeqInitTurn,			// 振り向き
	GmPlySeqInitLookupStart,	// 見上げ開始
	GmPlySeqInitLookupMiddle,	// 見上げ中
	GmPlySeqInitLookupEnd,		// 見上げ終了
	GmPlySeqInitSquatStart,		// しゃがみ開始
	GmPlySeqInitSquatMiddle,	// しゃがみ中
	GmPlySeqInitSquatEnd,		// しゃがみ終了
	GmPlySeqInitBrake,			// ブレーキ
	GmPlySeqInitSpin,			// スピン
	GmPlySeqInitSpinDashAcc,	// スピン加速
	GmPlySeqInitSpinDash,		// スピンダッシュ
	GmPlySeqInitStaggerFront,	// よろける 前
	GmPlySeqInitStaggerBack,	// よろける 後ろ
	GmPlySeqInitStaggerDanger,	// よろける 危険
	GmPlySeqInitFall,			// 落下
	GmPlySeqInitJump,			// ジャンプ
	GmPlySeqInitWallPush,		// 壁押し
	GmPlySeqInitHoming,			// ホーミング
	GmPlySeqInitHomingRef,		// ホーミング跳ね返り
	GmPlySeqInitJumpDash,		// ジャンプダッシュ
	GmPlySeqInitDamage,			// ダメージ
	GmPlySeqInitDeath,			// 死亡
	GmPlySeqInitTransformSuper,	// スーパーソニック変身
	GmPlySeqInitBossGaol,		// ボスクリア
	GmPlySeqInitBoss5Demo,		// ボスFINAL演出
	GmPlySeqInitTRetryFw,		// タイムアタックリトライ FW
	GmPlySeqInitTRetryAcc,		// タイムアタックリトライ その場加速

	// ギミックシーケンス
	NULL,						// スプリング
	NULL,						// 岩乗り開始
	NULL,						// 岩乗り
	NULL,						// 滑車掴まり
	NULL,						// 息継ぎ
	NULL,						// ダッシュパネル
	NULL,						// ターザンロープ
	NULL,						// ウォータースライダー
	NULL,						// S字パイプ
	NULL,						// コークスクリュー
	NULL,						// デモ用フットワーク
	NULL,						// ストッパー＠ピンボール
	NULL,						// 大砲
	NULL,						// 大砲発射
	NULL,						// アップバンパー
	NULL,						// シーソー
	NULL,						// ピンボール
	NULL,						// ピンボール（空中）
	NULL,						// フリッパー
	NULL,						// スプリングカタパルト確保
	NULL,						// スプリングカタパルト↑
	NULL,						// スプリングカタパルト↑以外
	NULL,						// 強制スピン
	NULL,						// 強制スピン 減速タイプ
	NULL,						// 強制スピン 落下
	NULL,						// 移動歯車
	NULL,						// 排液装置
	NULL,						// 排液装置（落下）
	NULL,						// スチームパイプモード
	NULL,						// ポップスチームジャンプ
	NULL,						// スペステリングＩＮ
	NULL,						// ボス2掴み
	NULL,						// ボスFINAL地球割り着地振動
	NULL,						// エンディング 演出１
	NULL,						// エンディング 演出２
	NULL,						// トロッコ危険状態
	NULL,						// トロッコ危険復帰
	NULL,						// スピンで落下
};


/// スペステソニック
void (*const g_gm_ply_seq_init_tbl_sp_son[GME_PLY_SEQ_STATE_MAX])(GMS_PLAYER_WORK*) = {
	// 通常シーケンス
	GmPlySeqInitFw,				// FW
	GmPlySeqInitWalk,			// 歩き
	GmPlySeqInitTurn,			// 振り向き
	GmPlySeqInitLookupStart,	// 見上げ開始
	GmPlySeqInitLookupMiddle,	// 見上げ中
	GmPlySeqInitLookupEnd,		// 見上げ終了
	GmPlySeqInitSquatStart,		// しゃがみ開始
	GmPlySeqInitSquatMiddle,	// しゃがみ中
	GmPlySeqInitSquatEnd,		// しゃがみ終了
	GmPlySeqInitBrake,			// ブレーキ
	GmPlySeqInitSpin,			// スピン
	GmPlySeqInitSpinDashAcc,	// スピン加速
	GmPlySeqInitSpinDash,		// スピンダッシュ
	GmPlySeqInitStaggerFront,	// よろける 前
	GmPlySeqInitStaggerBack,	// よろける 後ろ
	GmPlySeqInitStaggerDanger,	// よろける 危険
	GmPlySeqInitFall,			// 落下
	GmPlySeqInitJump,			// ジャンプ
	GmPlySeqInitWallPush,		// 壁押し
	GmPlySeqInitHoming,			// ホーミング
	GmPlySeqInitHomingRef,		// ホーミング跳ね返り
	GmPlySeqInitJumpDash,		// ジャンプダッシュ
	GmPlySeqInitDamage,			// ダメージ
	GmPlySeqInitDeath,			// 死亡
	GmPlySeqInitTransformSuper,	// スーパーソニック変身
	GmPlySeqInitBossGaol,		// ボスクリア
	GmPlySeqInitBoss5Demo,		// ボスFINAL演出
	GmPlySeqInitTRetryFw,		// タイムアタックリトライ FW
	GmPlySeqInitTRetryAcc,		// タイムアタックリトライ その場加速

	// ギミックシーケンス
	NULL,						// スプリング
	NULL,						// 岩乗り開始
	NULL,						// 岩乗り
	NULL,						// 滑車掴まり
	NULL,						// 息継ぎ
	NULL,						// ダッシュパネル
	NULL,						// ターザンロープ
	NULL,						// ウォータースライダー
	NULL,						// S字パイプ
	NULL,						// コークスクリュー
	NULL,						// デモ用フットワーク
	NULL,						// ストッパー＠ピンボール
	NULL,						// 大砲
	NULL,						// 大砲発射
	NULL,						// アップバンパー
	NULL,						// シーソー
	NULL,						// ピンボール
	NULL,						// ピンボール（空中）
	NULL,						// フリッパー
	NULL,						// スプリングカタパルト確保
	NULL,						// スプリングカタパルト↑
	NULL,						// スプリングカタパルト↑以外
	NULL,						// 強制スピン
	NULL,						// 強制スピン 減速タイプ
	NULL,						// 強制スピン 落下
	NULL,						// 移動歯車
	NULL,						// 排液装置
	NULL,						// 排液装置（落下）
	NULL,						// スチームパイプモード
	NULL,						// ポップスチームジャンプ
	NULL,						// スペステリングＩＮ
	NULL,						// ボス2掴み
	NULL,						// ボスFINAL地球割り着地振動
	NULL,						// エンディング 演出１
	NULL,						// エンディング 演出２
	NULL,						// トロッコ危険状態
	NULL,						// トロッコ危険復帰
	NULL,						// スピンで落下
};

/// トロッコ用
void (*const g_gm_ply_seq_init_tbl_tr_son[GME_PLY_SEQ_STATE_MAX])(GMS_PLAYER_WORK*) = {
	// 通常シーケンス
	GmPlySeqInitTruckFw,		// FW
	GmPlySeqInitTruckWalk,		// 歩き
		GmPlySeqInitTurn,			// 振り向き
	GmPlySeqInitLookupStart,	// 見上げ開始
	GmPlySeqInitLookupMiddle,	// 見上げ中
	GmPlySeqInitLookupEnd,		// 見上げ終了
	GmPlySeqInitTruckSquatStart,		// しゃがみ開始
	GmPlySeqInitTruckSquatMiddle,	// しゃがみ中
	GmPlySeqInitTruckSquatEnd,		// しゃがみ終了
		GmPlySeqInitBrake,			// ブレーキ
		GmPlySeqInitSpin,			// スピン
		GmPlySeqInitSpinDashAcc,	// スピン加速
		GmPlySeqInitSpinDash,		// スピンダッシュ
	GmPlySeqInitTruckStaggerFront,	// よろける 前
	GmPlySeqInitTruckStaggerBack,	// よろける 後ろ
		GmPlySeqInitStaggerDanger,	// よろける 危険
	GmPlySeqInitTruckFall,		// 落下
	GmPlySeqInitTruckJump,		// ジャンプ
		GmPlySeqInitWallPush,		// 壁押し
		GmPlySeqInitHoming,			// ホーミング
		GmPlySeqInitHomingRef,		// ホーミング跳ね返り
		GmPlySeqInitJumpDash,		// ジャンプダッシュ
	GmPlySeqInitDamage,			// ダメージ
	GmPlySeqInitDeath,			// 死亡
	GmPlySeqInitTransformSuper,	// スーパーソニック変身
		GmPlySeqInitBossGaol,		// ボスクリア
		GmPlySeqInitBoss5Demo,		// ボスFINAL演出
	GmPlySeqInitTRetryFw,		// タイムアタックリトライ FW
	GmPlySeqInitTRetryAcc,		// タイムアタックリトライ その場加速

	// ギミックシーケンス
	NULL,						// スプリング
	NULL,						// 岩乗り開始
	NULL,						// 岩乗り
	NULL,						// 滑車掴まり
	NULL,						// 息継ぎ
	NULL,						// ダッシュパネル
	NULL,						// ターザンロープ
	NULL,						// ウォータースライダー
	NULL,						// S字パイプ
	NULL,						// コークスクリュー
	NULL,						// デモ用フットワーク
	NULL,						// ストッパー＠ピンボール
	NULL,						// 大砲
	NULL,						// 大砲発射
	NULL,						// アップバンパー
	NULL,						// シーソー
	NULL,						// ピンボール
	NULL,						// ピンボール（空中）
	NULL,						// フリッパー
	NULL,						// スプリングカタパルト確保
	NULL,						// スプリングカタパルト↑
	NULL,						// スプリングカタパルト↑以外
	NULL,						// 強制スピン
	NULL,						// 強制スピン 減速タイプ
	NULL,						// 強制スピン 落下
	NULL,						// 移動歯車
	NULL,						// 排液装置
	NULL,						// 排液装置（落下）
	NULL,						// スチームパイプモード
	NULL,						// ポップスチームジャンプ
	NULL,						// スペステリングＩＮ
	NULL,						// ボス2掴み
	NULL,						// ボスFINAL地球割り着地振動
	NULL,						// エンディング 演出１
	NULL,						// エンディング 演出２
	NULL,						// トロッコ危険状態
	NULL,						// トロッコ危険復帰
	NULL,						// スピンで落下
};

void (*const *g_gm_ply_seq_init_tbl_list[GSD_CHAR_ID_MAX])(GMS_PLAYER_WORK*) = {
	g_gm_ply_seq_init_tbl_son,		// ソニック
	g_gm_ply_seq_init_tbl_son,		// スーパーソニック
	g_gm_ply_seq_init_tbl_sp_son,	// スペステソニック
	g_gm_ply_seq_init_tbl_son,		// ピンボールソニック
	g_gm_ply_seq_init_tbl_son,		// ピンボールスーパーソニック
	g_gm_ply_seq_init_tbl_tr_son,	// トロッコソニック
	g_gm_ply_seq_init_tbl_tr_son,	// トロッコスーパーソニック
};


//----- Local Variables -----------------------------------------------------

//===========================================================================
// FWターン専用データ
//===========================================================================
#define GMD_PLY_SEQ_TURN_FRAME	(10)	//!< ターンフレーム

u16	gm_ply_seq_turn_dir_tbl[GMD_PLY_SEQ_TURN_FRAME] = {
	(u16)-0x10A9,
	(u16)-0x2152,
	(u16)-0x31FB,
	(u16)-0x42A4,
	(u16)-0x534D,
	(u16)-0x63F6,
	(u16)-0x74A0,
	(u16)-0x786A,
	(u16)-0x7C34,
	(u16)-0x8000,
};
u16	gm_ply_seq_turn_l_dir_tbl[GMD_PLY_SEQ_TURN_FRAME] = {
	0x10A9,
	0x2152,
	0x31FB,
	0x42A4,
	0x534D,
	0x63F6,
	0x74A0,
	0x786A,
	0x7C34,
	0x8000,
};

//===========================================================================
// 通常落下ターン専用データ
//===========================================================================
#define GMD_PLY_SEQ_FALL_TURN_FRAME	(10)	//!< ターンフレーム
u16	gm_ply_seq_fall_turn_dir_tbl[GMD_PLY_SEQ_FALL_TURN_FRAME] = {
	(u16)(-0x10A9 + 0x8000),// - 0x438E/2),
	(u16)(-0x2152 + 0x8000),// - 0x438E/2),
	(u16)(-0x31FB + 0x8000),// - 0x438E/2),
	(u16)(-0x42A4 + 0x8000),// - 0x438E/2),
	(u16)(-0x534D + 0x8000),// - 0x438E/2),
	(u16)(-0x63F6 + 0x8000),// - 0x438E/2),
	(u16)(-0x74A0 + 0x8000),// - 0x438E/2),
	(u16)(-0x786A + 0x8000),// - 0x438E/2),
	(u16)(-0x7C34 + 0x8000),// - 0x438E/2),
	(u16)(-0x8000 + 0x8000),// - 0x438E/2),
};
u16	gm_ply_seq_fall_turn_l_dir_tbl[GMD_PLY_SEQ_FALL_TURN_FRAME] = {
	(u16)(0x10A9 + 0x8000),// + 0x438E/2),
	(u16)(0x2152 + 0x8000),// + 0x438E/2),	// 10A9
	(u16)(0x31FB + 0x8000),// + 0x438E/2),	// 10A9
	(u16)(0x42A4 + 0x8000),// + 0x438E/2),	// 10A9
	(u16)(0x534D + 0x8000),// + 0x438E/2),	// 10A9
	(u16)(0x63F6 + 0x8000),// + 0x438E/2),	// 10A9
	(u16)(0x74A0 + 0x8000),// + 0x438E/2),	// 10AA
	(u16)(0x786A + 0x8000),// + 0x438E/2),	// 3CA
	(u16)(0x7C34 + 0x8000),// + 0x438E/2),	// 3CA
	(u16)(0),//Warning回避(0x8000 + 0x8000),// + 0x438E/2),	// 3CC
};


//===========================================================================
// ジャンプ(GmPlySeqInitJump)時 SE呼び出しチェック
//===========================================================================
static BOOL gm_ply_seq_jump_call_se_jump = TRUE;	//!< TRUEの時 GmPlySeqInitJump 内で ジャンプSEをコールする
													// note 攻撃跳ね返りの時にのみFALSEで使用

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmPlySeqSetSeqState
/*!
 *	シーケンス 必要ステータス設定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqSetSeqState(GMS_PLAYER_WORK *ply_work)
{
	/* プレイヤーシーケンス初期化処理テーブル取得 */
	//ply_work->seq_init_tbl = &g_gm_ply_seq_init_tbl[ply_work->char_id][0];
	ply_work->seq_init_tbl = &(g_gm_ply_seq_init_tbl_list[ply_work->char_id])[0];
	/* シーケンスステートデータテーブル取得 */
	ply_work->seq_state_data_tbl = &g_gm_ply_seq_state_data_tbl[ply_work->char_id][0];
}

// ==========================================================================
// GmPlySeqMain
/*!
 *	シーケンスメイン処理
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		現在の状況・入力によって、シーケンス変更チェックを行ったりします。
 */
// ==========================================================================
void GmPlySeqMain(GMS_PLAYER_WORK *ply_work)
{
	
	OBS_OBJECT_WORK					*obj_work;
	const GMS_PLY_SEQ_STATE_DATA	*seq_state_data;
//	s32	i;

	obj_work = (OBS_OBJECT_WORK*)ply_work;
	seq_state_data	= ply_work->seq_state_data_tbl;

#if defined (MTD_DEBUG)
#if !_IPHONE || !TARGET_IPHONE_SIMULATOR
	if (!GSM_MAIN_STAGE_IS_SPSTAGE()) {
	if (AoPadDirect() & KEY_R_UP) {
		ply_work->obj_work.move_flag &= ~OBD_MOVE_NOMOVE;

		gmPlySeqDebugMove(ply_work);
		return;
	}
	else if (AoPadRelease() & KEY_R_UP) {
		ply_work->obj_work.move_flag &= ~OBD_MOVE_NOCOL;
		ply_work->obj_work.flag &= ~OBD_OBJECT_NOHIT;
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
	}
	}
#else //!_IPHONE || !TARGET_IPHONE_SIMULATOR
	{
		static bool s_toggle_dbg = false;
		if (AoPadStand() & KEY_R_UP) {
			s_toggle_dbg = !s_toggle_dbg;
			if (!s_toggle_dbg) {
				ply_work->obj_work.move_flag &= ~OBD_MOVE_NOCOL;
				ply_work->obj_work.flag &= ~OBD_OBJECT_NOHIT;
				GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
			}
		}
		if (s_toggle_dbg) {
			ply_work->obj_work.move_flag &= ~OBD_MOVE_NOMOVE;

			gmPlySeqDebugMove(ply_work);
			return;
		}
	}
#endif //!_IPHONE || !TARGET_IPHONE_SIMULATOR
#endif

	/* カウンタ関連 */
	if (ply_work->no_spddown_timer) {	// gmPlayerDefaultInFuncへ移動？
		ply_work->no_spddown_timer = ObjTimeCountDown(ply_work->no_spddown_timer);
	}

	/* MAXダッシュ終了チェックタイマー */
	if (ply_work->maxdash_timer) {
		// 終了判定は別の場所で行うのでここではカウントダウンのみ
		ply_work->maxdash_timer = ObjTimeCountDown(ply_work->maxdash_timer);
	}

//	// フレームカウンタ
//	ply_work->frame = ObjTimeCountUp(ply_work->frame);
//	//ply_work->atk_frame = ObjTimeCountUp(ply_work->atk_frame);
//	if (ply_work->end_frame != -1) {
//		ply_work->end_frame = ObjTimeCountDown(ply_work->end_frame);
//	}

//	// 継続無敵カウンタ
//	if (ply_work->conti_invincible_frame) {
//		ply_work->conti_invincible_frame = ObjTimeCountDown(ply_work->conti_invincible_frame);
//	}

	/* 通常ACTゴール処理 */
	if (ply_work->player_flag & GMD_PLF_ACT_GOAL) {
		gmPlySeqActGoal(ply_work);
	}
	
	/* ボスステージゴール処理 */
	if (ply_work->player_flag & GMD_PLF_BOSS_GOAL_PRE) {
		gmPlySeqBossGoalPre(ply_work);
	}
	
	/* ボスFINAL演出走り処理 */
	if (ply_work->player_flag & GMD_PLF_BOSS5_DEMO) {
		gmPlySeqBoss5DemoPre(ply_work);
	}

	/* スペステ回転操作処理 */
	if (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {
		gmPlySeqSplStgRollCtrl(ply_work);
	}

	/* シーケンス変更判定 */
	gmPlySeqCheckChangeSequence(ply_work);

	/* シーケンス処理 */
	if (ply_work->seq_func) {
		ply_work->seq_func(ply_work);
	}

	// アニメーション速度設定
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK) {
		// 歩きタイプ速度設定
		GmPlayerAnimeSpeedSetWalk(ply_work, ply_work->obj_work.spd_m);
	}
#if 0
	else if (ply_work->act_state == GME_PLY_ACT_STATE_BRAKE1) {//(ply_work->seq_state == GME_PLY_SEQ_STATE_BRAKE) {
		ply_work->obj_work.obj_3d->speed[0] = 4.f;
		ply_work->obj_work.obj_3d->speed[1] = 4.f;
	}
#endif
	else if (!(seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_NOSET)) {
		// 速度初期化
		ply_work->obj_work.obj_3d->speed[0] = 1.f;
		ply_work->obj_work.obj_3d->speed[1] = 1.f;
	}

	// プログラムターン
	if (ply_work->player_flag & GMD_PLF_PGM_TURN) {
		s32	dir = ply_work->pgm_turn_dir;

		if (ply_work->pgm_turn_dir_tbl) {
			dir = *(ply_work->pgm_turn_dir_tbl + ply_work->pgm_turn_tbl_cnt);

			ply_work->pgm_turn_tbl_cnt++;
			if (ply_work->pgm_turn_tbl_cnt >= ply_work->pgm_turn_tbl_num) {
				ply_work->pgm_turn_tbl_cnt = ply_work->pgm_turn_tbl_num - 1;

				// ターン終了
				ply_work->player_flag &= ~GMD_PLF_PGM_TURN;
				if (!(ply_work->player_flag & GMD_PLF_PGM_TURN_RDM)) {
					dir = 0;
				}
			}
		}
		else {
			if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
				//dir -= GMD_PL_PGM_TURN_SPD;
				dir -= ply_work->pgm_turn_spd;
				if (dir <= 0) {
					// ターン終了
					dir = 0;
					ply_work->player_flag &= ~GMD_PLF_PGM_TURN;
				}
			}
			else {
				dir += ply_work->pgm_turn_spd;
				if (dir >= 0x10000) {
					// ターン終了
					dir = 0;
					ply_work->player_flag &= ~GMD_PLF_PGM_TURN;
				}
			}
		}

		ply_work->pgm_turn_dir = (u16)dir;

		if ((ply_work->player_flag & GMD_PLF_PGM_FALL_TURN) &&
				!(ply_work->player_flag & GMD_PLF_PGM_TURN)) {
			// モーション復帰
			GmPlayerActionChange(ply_work, ply_work->fall_act_state);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
			ply_work->player_flag &= ~GMD_PLF_PGM_FALL_TURN;
		}
	}
}


// ==========================================================================
// GmPlySeqChangeSequence
/*!
 *	シーケンス変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	seq_state	[in]	シーケンスタイプ
 *
 *	@return	TRUE : シーケンス変更
 */
// ==========================================================================
BOOL GmPlySeqChangeSequence(GMS_PLAYER_WORK *ply_work, GME_PLY_SEQ_STATE seq_state)
{
	MTM_ASSERT((u32)seq_state < GME_PLY_SEQ_STATE_MAX);

	// シーケンスステート置き換え
	GmPlySeqChangeSequenceState(ply_work, seq_state);

#if 0
	// モーション依存ターン状態解除
	if (ply_work->player_flag & GMD_PLF_PGM_TURN_RDM) {
		GmPlayerSetReverseOnlyState(ply_work);
	}
	if (ply_work->player_flag & GMD_PLF_PGM_FALL_TURN) {
		// プログラムターンOFF
		ply_work->player_flag &= ~GMD_PLF_PGM_TURN_MASK;
		ply_work->pgm_turn_dir_tbl = NULL;
		ply_work->pgm_turn_dir = 0;
		ply_work->pgm_turn_spd = 0;
	}
#endif

	if (ply_work->seq_init_tbl[seq_state]) {
		ply_work->seq_init_tbl[seq_state](ply_work);
		return (TRUE);
	}
	return (FALSE);
}

// ==========================================================================
// GmPlySeqChangeSequenceState
/*!
 *	シーケンスステート変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	seq_state	[in]	シーケンスタイプ
 *
 *	@note
 *		ギミック用にシーケンスステートのみ置き換えます
 */
// ==========================================================================
void GmPlySeqChangeSequenceState(GMS_PLAYER_WORK *ply_work, GME_PLY_SEQ_STATE seq_state)
{
	MTM_ASSERT((u32)seq_state < GME_PLY_SEQ_STATE_MAX);

	if (ply_work->gmk_obj) {
		// ギミックの影響を受けていた時はギミックステート初期化
		GmPlayerStateGimmickInit(ply_work);
	}

	// シーケンスステート置き換え
	ply_work->prev_seq_state = ply_work->seq_state;
	ply_work->seq_state = seq_state;

	// 各種ステート初期化
	// ◆暫定
	ply_work->rect_work[GMD_PLAYER_RECT_ATK].flag &= ~OBD_RECT_ENABLE;	// 無効化

	// モーション依存ターン状態解除
	if (ply_work->player_flag & GMD_PLF_PGM_TURN_RDM) {
		GmPlayerSetReverseOnlyState(ply_work);
	}
	if (ply_work->player_flag & GMD_PLF_PGM_FALL_TURN) {
		// プログラムターンOFF
		ply_work->player_flag &= ~GMD_PLF_PGM_TURN_MASK;
		ply_work->pgm_turn_dir_tbl = NULL;
		ply_work->pgm_turn_dir = 0;
		ply_work->pgm_turn_spd = 0;
	}
}

// ==========================================================================
// プログラムターン
// ==========================================================================
// ==========================================================================
// GmPlySeqSetProgramTurn
/*!
 *	プログラム 振り向き設定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@parma	turn_spd	[in]	ターン速度
 */
// ==========================================================================
void GmPlySeqSetProgramTurn(GMS_PLAYER_WORK *ply_work, u16 turn_spd)
{
	if (!(ply_work->player_flag & GMD_PLF_PGM_TURN)) {
		// 現在ターン中でない
		ply_work->pgm_turn_dir = 0;		// 念の為クリア
	}

	// 反転は先に行う
	GmPlayerSetReverse(ply_work);

	ply_work->player_flag |= GMD_PLF_PGM_TURN;

	ply_work->pgm_turn_spd = turn_spd;
	ply_work->pgm_turn_dir += 0x8000;

	ply_work->pgm_turn_dir_tbl = NULL;
}

// ==========================================================================
// GmPlySeqSetProgramTurnTbl
/*!
 *	プログラム 振り向き設定 テーブルタイプ
 *
 *	@param	ply_work		[in]	プレイヤーワーク
 *	@parma	turn_tbl		[in]	ターン角度テーブル
 *	@parma	tbl_num			[in]	ターンテーブルデータ数
 *	@param	rev_depend_mtn	[in]	モーション依存タイプターン
 */
// ==========================================================================
void GmPlySeqSetProgramTurnTbl(GMS_PLAYER_WORK *ply_work, u16 *turn_tbl, s32 tbl_num, BOOL rev_depend_mtn)
{
	if (!(ply_work->player_flag & GMD_PLF_PGM_TURN)) {
		// 現在ターン中でない
		ply_work->pgm_turn_dir = 0;		// 念の為クリア
	}

	if (!rev_depend_mtn) {
		// 反転は先に行う
		GmPlayerSetReverse(ply_work);
	}
	else {
		ply_work->player_flag |= GMD_PLF_PGM_TURN_RDM;
	}

	ply_work->player_flag |= GMD_PLF_PGM_TURN;

	ply_work->pgm_turn_dir_tbl = turn_tbl;
	ply_work->pgm_turn_tbl_num = tbl_num;
	ply_work->pgm_turn_tbl_cnt = 0;
}

// ==========================================================================
// GmPlySeqSetProgramTurnFwTurn
/*!
 *	プログラム 振り向き設定 FW時ターン
 *
 *	@param	ply_work		[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqSetProgramTurnFwTurn(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
		GmPlySeqSetProgramTurnTbl(ply_work, gm_ply_seq_turn_l_dir_tbl, GMD_PLY_SEQ_TURN_FRAME, TRUE);
	}
	else {
		GmPlySeqSetProgramTurnTbl(ply_work, gm_ply_seq_turn_dir_tbl, GMD_PLY_SEQ_TURN_FRAME, TRUE);
	}
}

// ==========================================================================
// 落下ターン
// ==========================================================================
// ==========================================================================
// GmPlySeqSetFallTurn
/*!
 *	プログラム 落下振り向き設定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@parma	turn_spd	[in]	ターン速度
 */
// ==========================================================================
void GmPlySeqSetFallTurn(GMS_PLAYER_WORK *ply_work)
{
	s32	pgm_trun_tbl_cnt_temp = 0;

	if (ply_work->player_flag & GMD_PLF_PGM_FALL_TURN) {
		// 既にターン中
		// 途中から
		pgm_trun_tbl_cnt_temp = ply_work->pgm_turn_tbl_cnt;
	}
	else {
		// アクトステート保存
		ply_work->fall_act_state = ply_work->act_state;
	}


	if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
		GmPlySeqSetProgramTurnTbl(ply_work, gm_ply_seq_fall_turn_l_dir_tbl, GMD_PLY_SEQ_FALL_TURN_FRAME, FALSE);
	}
	else {
		GmPlySeqSetProgramTurnTbl(ply_work, gm_ply_seq_fall_turn_dir_tbl, GMD_PLY_SEQ_FALL_TURN_FRAME, FALSE);
	}

	ply_work->player_flag |= GMD_PLF_PGM_FALL_TURN;

	// アクション設定
	if (ply_work->act_state == GME_PLY_ACT_STATE_JUMP_FALL_R ||
				ply_work->act_state == GME_PLY_ACT_STATE_JUMP_FALL_R_TURN) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_FALL_R_TURN);
	}
	else {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_FALL_TURN);
	}

	// ターン途中から設定
	if (pgm_trun_tbl_cnt_temp) {
		ply_work->pgm_turn_tbl_cnt = GMD_PLY_SEQ_FALL_TURN_FRAME - pgm_trun_tbl_cnt_temp;
		ply_work->obj_work.obj_3d->frame[0] = (float)ply_work->pgm_turn_tbl_cnt;
	}
}

// ==========================================================================
// FW
// ==========================================================================
// ==========================================================================
// GmPlySeqChangeFw
/*!
 *	FWへ変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqChangeFw(GMS_PLAYER_WORK *ply_work)
{
	GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
}

// ==========================================================================
// GmPlySeqInitFw
/*!
 *	FW 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_work	: FW回数カウンタ
 *		user_timer	: FW時間カウンタ
 */
// ==========================================================================
void GmPlySeqInitFw(GMS_PLAYER_WORK *ply_work)
{
	if (!GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// 通常シーケンス
		if (ply_work->obj_work.spd_m) {
			if (ply_work->prev_seq_state == GME_PLY_SEQ_STATE_TURN) {
				// 一旦FWアクションにしてから(モーションを繋ぐ為)
				// アクション変更
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_FW);
			}

			// 歩きへ移行
			GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_WALK);
			return;
		}
		// アクション変更
		if (!(ply_work->player_flag & GMD_PLF_PINBALL_SONIC)) {	// ピンボールの時は変更しない
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_FW);
		}
		else {
			// ピンボールの時のみ
			// エフェクト ブラー
			GmPlyEfctCreateSpinJumpBlur(ply_work);
		}
	} else {
		// スペステ用
		// エフェクト ブラー
		GmPlyEfctCreateSpinJumpBlur(ply_work);
	}

	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// ワーククリア
	ply_work->obj_work.user_timer	= 0;
	ply_work->obj_work.user_work	= 0;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqFwMain;
#if 0

//	    if ( ply_work->obj_work.spd_m) {
  //      // 歩く
//		ply_work->ppWalk(ply_work);
//		return;
  //  }

//	if ( ply_work->act_state < GME_PLY_ACT_STATE_UNIQUE_START){	// ◆後で範囲を制限するかも

        // ２軸以上回転するモーションはアニメの終わりでしかブレンドを行わない
        if ( !(ply_work->act_state == GME_PLY_ACT_STATE_TURN) &&
				((ply_work->obj_work.disp_flag & OBD_DISP_END) ||
					!( ply_work->act_state == GME_PLY_ACT_STATE_JUMPG11 ||
					ply_work->act_state == GME_PLY_ACT_STATE_TR10 ||
					ply_work->act_state == GME_PLY_ACT_STATE_TR20 ||
					ply_work->act_state == GME_PLY_ACT_STATE_TR30)) ) {
            ply_work->obj_work.disp_flag |= OBD_DISP_3D_BLEND;
		}
//	}

	/* アクション設定 */
	if (!(ply_work->gmk_flag & GMD_PLGF_DH_BOARD)) {
	// 通常
		GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_FW );
	}
	else {
	// 滑降ボード
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_DHB_FW);
		GmGmkDHBoardSetAction(ply_work, GMD_GMK_DH_BOARD_ACT_TYPE_FW);		// ボードアクション設定
	}
#else
#endif
}

// ==========================================================================
// gmPlySeqFwMain
/*!
 *	FW
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqFwMain(GMS_PLAYER_WORK *ply_work)
{
	//ply_work->obj_work.user_timer++;

	if (ply_work->act_state == GME_PLY_ACT_STATE_FW) {
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			ply_work->obj_work.user_work++;
			if (ply_work->obj_work.user_work >= 8) {
			//if (ply_work->obj_work.user_timer > 60*30) {
				// 待機0へ
				// アクション変更
				if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
					GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_WAIT_1_1);
				}
				else {
					GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_WAIT_0_1);
				}
				if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
					// 強制反転
					GmPlySeqSetProgramTurn(ply_work, GMD_PL_PGM_TURN_SPD_DEF);
				}
				//ply_work->obj_work.user_timer = 0;
				ply_work->obj_work.user_work = 0;
			}
#if 0
			else {
				ply_work->obj_work.user_work++;
				if (ply_work->obj_work.user_work >= 10) {
					// おまけFWへ
					// アクション変更
					//GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_FW_EX);
					ply_work->obj_work.user_work = 0;
				}
			}
#endif
		}
	}
#if 0
	else if (ply_work->act_state == GME_PLY_ACT_STATE_FW_EX) {
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// 通常FWへ
			// アクション変更
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_FW);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
	}
#endif
	else if (ply_work->act_state == GME_PLY_ACT_STATE_WAIT_0_1 ||
			ply_work->act_state == GME_PLY_ACT_STATE_WAIT_1_1 ||
			ply_work->act_state == GME_PLY_ACT_STATE_WAIT_2_1) {
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// アクション変更
			GmPlayerActionChange(ply_work, (GME_PLY_ACT_STATE)(ply_work->act_state + 1));
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
			ply_work->obj_work.user_work = 0;
		}
	}
	else if (ply_work->act_state == GME_PLY_ACT_STATE_WAIT_0_2) {
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			ply_work->obj_work.user_work++;
			if (ply_work->obj_work.user_work >= 10) {
				// 待機1へ
				// アクション変更
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_WAIT_1_1);
				ply_work->obj_work.user_work = 0;
			}
		}
	}
	else if (ply_work->act_state == GME_PLY_ACT_STATE_WAIT_1_2) {
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {

			ply_work->obj_work.user_work++;
			if (ply_work->obj_work.user_work >= 3 &&
					!(ply_work->player_flag & GMD_PLF_SUPER_SONIC)) {
				// 待機2へ
				// アクション変更
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_WAIT_2_1);
				ply_work->obj_work.user_work = 0;
			}
		}
	}

	// おまけFW切り替え

	// 待機アクション切り替え1

	// 待機アクション切り替え2
}

// ==========================================================================
// 歩き
// ==========================================================================
// ==========================================================================
// GmPlySeqInitWalk
/*!
 *	歩き 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_timer	: エフェクト間隔用
 */
// ==========================================================================
void GmPlySeqInitWalk(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	if (!(ply_work->player_flag & GMD_PLF_PINBALL_SONIC)) {	// ピンボールの時は切り替えない
		GmPlayerWalkActionSet(ply_work);
	}
	else {
		// ピンボールの時のみ
		// エフェクト ブラー
		GmPlyEfctCreateSpinJumpBlur(ply_work);
	}

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqWalkMain;
	ply_work->obj_work.user_timer = 0;
}

// ==========================================================================
// gmPlySeqWalkMain
/*!
 *	歩き
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqWalkMain(GMS_PLAYER_WORK *ply_work)
{
    // アニメ速度設定
	//GmPlayerAnimeSpeedSetWalk(ply_work, ply_work->obj_work.spd_m);

	// 向き変更チェック
	if ((ply_work->obj_work.spd_m > 0 && GmPlayerKeyCheckWalkRight(ply_work) && (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP)) ||
			(ply_work->obj_work.spd_m < 0 && GmPlayerKeyCheckWalkLeft(ply_work) && !(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP))) {
		// 方向変換
		GmPlySeqSetProgramTurn(ply_work, GMD_PL_PGM_TURN_SPD_DEF);
	}

	// 歩きアクション変更チェック
	GmPlayerWalkActionCheck(ply_work);

	// エフェクト
	if (((ply_work->obj_work.user_timer & 0x3F) == 1) &&
			!ply_work->obj_work.ride_obj) {
		GmPlyEfctCreateFootSmoke(ply_work);
	}
	ply_work->obj_work.user_timer++;
}

// ==========================================================================
// ターン
// ==========================================================================
// ==========================================================================
// GmPlySeqInitTurn
/*!
 *	振り向き 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitTurn(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->prev_seq_state == GME_PLY_SEQ_STATE_TURN) {
		// ターン中の場合は、即時反転して終了する
		// ターン中フラグOFF
		ply_work->player_flag &= ~GMD_PLF_PGM_TURN_MASK;
		// ◆まじめにまわす？
		GmPlayerSetReverse(ply_work);
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
		return;
	}

	if (GME_PLY_ACT_STATE_BRAKE1 <= ply_work->act_state &&
			ply_work->act_state <= GME_PLY_ACT_STATE_BRAKE3) {
		// アクション変更
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_TURN_BRAKE);
	}
	else if (GME_PLY_ACT_STATE_RUN <= ply_work->act_state &&
			ply_work->act_state <= GME_PLY_ACT_STATE_DASH_2) {
		// アクション変更
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_TURN_RUN);
	}
	else {
		// アクション変更
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_TURN);

		// プログラムターン有り
		GmPlySeqSetProgramTurnFwTurn(ply_work);
	//	if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
	//		GmPlySeqSetProgramTurnTbl(ply_work, gm_ply_seq_turn_l_dir_tbl, GMD_PLY_SEQ_TURN_FRAME, TRUE);
	//	}
	//	else {
	//		GmPlySeqSetProgramTurnTbl(ply_work, gm_ply_seq_turn_dir_tbl, GMD_PLY_SEQ_TURN_FRAME, TRUE);
	//	}
	}

	// 反転
	//GmPlayerSetReverse(ply_work);

	// ターン中フラグ設定
	//ply_work->player_flag |= GMD_PLF_TURN;

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqTurnMain;
    //ply_work->obj_work.user_timer = 0;
}

// ==========================================================================
// gmPlySeqTurnMain
/*!
 *	振り向き
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqTurnMain(GMS_PLAYER_WORK *ply_work)
{
	// 終了チェック
	if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
		// ターン中フラグOFF
		//ply_work->player_flag &= ~GMD_PLF_PGM_TURN_MASK;

		// 反転
		GmPlayerSetReverseOnlyState(ply_work);

		// シーケンス変更
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
	}
}

// ==========================================================================
// 見上げ
// ==========================================================================
// ==========================================================================
// GmPlySeqInitLookupStart
/*!
 *	見上げ開始 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitLookupStart(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_LOOKUP1);

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqLookupMain;
    //ply_work->obj_work.user_timer = 0;
}

// ==========================================================================
// GmPlySeqInitLookupMiddle
/*!
 *	見上げ中 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitLookupMiddle(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_LOOKUP2);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqLookupMain;
    //ply_work->obj_work.user_timer = 0;
}

// ==========================================================================
// GmPlySeqInitLookupEnd
/*!
 *	見上げ終了 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitLookupEnd(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_LOOKUP3);

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqLookupEndMain;
    //ply_work->obj_work.user_timer = 0;
}

// ==========================================================================
// gmPlySeqLookupMain
/*!
 *	見上げ
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqLookupMain(GMS_PLAYER_WORK *ply_work)
{
	// 通常シーケンス
	if (ply_work->obj_work.spd_m) {
		// 歩きへ移行
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_WALK);
		return;
	}

	switch (ply_work->act_state) {
	case GME_PLY_ACT_STATE_LOOKUP1:
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// シーケンス変更
			GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_LOOKUP_M);
		}
		break;
	//case GME_PLY_ACT_STATE_LOOKUP2:
	//	break;
	//case GME_PLY_ACT_STATE_LOOKUP3:
	//	break;
	default:
		break;
	}
}

// ==========================================================================
// gmPlySeqLookupEndMain
/*!
 *	見上げ終了
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqLookupEndMain(GMS_PLAYER_WORK *ply_work)
{
	// 終了チェック
	if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
	}
}

// ==========================================================================
// しゃがみ
// ==========================================================================
// ==========================================================================
// GmPlySeqInitSquatStart
/*!
 *	しゃがみ開始 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitSquatStart(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SQUATDOWN1);

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqSquatMain;
    //ply_work->obj_work.user_timer = 0;
}

// ==========================================================================
// GmPlySeqInitSquatMiddle
/*!
 *	しゃがみ中 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitSquatMiddle(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SQUATDOWN2);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < 0x1000) {
		ply_work->obj_work.spd_m = 0;
	}

	// メイン処理設定
    ply_work->seq_func = gmPlySeqSquatMain;
    //ply_work->obj_work.user_timer = 0;
}

// ==========================================================================
// GmPlySeqInitSquatEnd
/*!
 *	しゃがみ終了 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitSquatEnd(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SQUATDOWN3);

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqSquatEndMain;
    //ply_work->obj_work.user_timer = 0;
}

// ==========================================================================
// gmPlySeqSquatMain
/*!
 *	しゃがみ 開始・中
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqSquatMain(GMS_PLAYER_WORK *ply_work)
{
	switch (ply_work->act_state) {
	case GME_PLY_ACT_STATE_SQUATDOWN1:
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// シーケンス変更変更
			GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SQUAT_M);
		}
		break;
	//case GME_PLY_ACT_STATE_SQUATDOWN2:
	//	break;
	//case GME_PLY_ACT_STATE_SQUATDOWN3:
	//	break;
	default:
		break;
	}

	if (ply_work->seq_state == GME_PLY_SEQ_STATE_SQUAT_M) {
		if (ply_work->obj_work.spd_m) {
			// ダッシュ開始
			// 1フレームだけ壁HITしても速度クリアを行わない
			ply_work->obj_work.move_flag |= OBD_MOVE_NOSPD;

			// スピンへ
			GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SPIN);
		}
	}
}

// ==========================================================================
// gmPlySeqSquatEndMain
/*!
 *	しゃがみ 終了
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqSquatEndMain(GMS_PLAYER_WORK *ply_work)
{
	// 終了チェック
	if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
	}
}

// ==========================================================================
// ブレーキ
// ==========================================================================
// ==========================================================================
// GmPlySeqInitBrake
/*!
 *	ブレーキ 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitBrake(GMS_PLAYER_WORK *ply_work)
{
	// 速度でアクションを変更する必要あり？◆

	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_BRAKE1);

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqBrakeMain;
    //ply_work->obj_work.user_timer = 0;

    // SE
	GmSoundPlaySE("Brake");

	// エフェクト ブレーキ 衝撃波
	GmPlyEfctCreateBrakeImpact(ply_work);
	// エフェクト ブレーキ 砂煙
	GmPlyEfctCreateBrakeDust(ply_work);
}

// ==========================================================================
// gmPlySeqBrakeMain
/*!
 *	ブレーキ
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqBrakeMain(GMS_PLAYER_WORK *ply_work)
{
#if 1
	if (ply_work->act_state != GME_PLY_ACT_STATE_BRAKE3) {
		if (((ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && !GmPlayerKeyCheckWalkRight(ply_work)) ||
				(!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && !GmPlayerKeyCheckWalkLeft(ply_work))) {
			// アクション変更
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_BRAKE3);
		}
	}
	switch (ply_work->act_state) {
	case GME_PLY_ACT_STATE_BRAKE1:
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// アクション変更
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_BRAKE2);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
		break;
	case GME_PLY_ACT_STATE_BRAKE2:
		if (!((ply_work->obj_work.spd_m > 0 && GmPlayerKeyCheckWalkLeft(ply_work)) ||
				(ply_work->obj_work.spd_m < 0 && GmPlayerKeyCheckWalkRight(ply_work))) ) {
			// ブレーキSE停止
			//NNS_SndPlayerStopSeq( &ply_work->h_snd_se, 0 );

			//ブレーキ終了へ
		//	if (GmPlayerKeyCheckWalkLeft(ply_work) || GmPlayerKeyCheckWalkRight(ply_work)) {
		//		// 反転
		//		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_TURN);
		//		return;
		//	}
			// ブレーキ終了
			// ターンへ変更
			GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_TURN);
			// アクション変更
			//GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_BRAKE3);
		}
		break;
	case GME_PLY_ACT_STATE_BRAKE3:
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// ブレーキ終了
			if (ply_work->obj_work.spd_m) {
				GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_WALK);
			}
			else {
				GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
			}
		}
		break;
	default:
		break;
	}
#else
	if (ply_work->act_state != GME_PLY_ACT_STATE_BRAKE3) {
		if (!(GmPlayerKeyCheckWalkLeft(ply_work) | GmPlayerKeyCheckWalkRight(ply_work))) {
			// アクション変更
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_BRAKE3);
		}
	}
	switch (ply_work->act_state) {
	case GME_PLY_ACT_STATE_BRAKE1:
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// アクション変更
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_BRAKE2);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
		break;
	case GME_PLY_ACT_STATE_BRAKE2:
		if (!((ply_work->obj_work.spd_m > 0 && GmPlayerKeyCheckWalkLeft(ply_work)) ||
				(ply_work->obj_work.spd_m < 0 && GmPlayerKeyCheckWalkRight(ply_work))) ) {
			// ブレーキSE停止
			//NNS_SndPlayerStopSeq( &ply_work->h_snd_se, 0 );

			//ブレーキ終了へ
		//	if (GmPlayerKeyCheckWalkLeft(ply_work) || GmPlayerKeyCheckWalkRight(ply_work)) {
		//		// 反転
		//		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_TURN);
		//		return;
		//	}
			// アクション変更
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_BRAKE3);
		}
		break;
	case GME_PLY_ACT_STATE_BRAKE3:
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// ブレーキ終了
			if (ply_work->obj_work.spd_m) {
				GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_WALK);
			}
			else {
				GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
			}
		}
		break;
	default:
		break;
	}
#endif
}


// ==========================================================================
// スピン
// ==========================================================================
// ==========================================================================
// GmPlySeqInitSpin
/*!
 *	スピン 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitSpin(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN_SMALL);
	
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqSpinMain;

	// 攻撃設定
	GmPlayerSetAtk(ply_work);

	if (ply_work->prev_seq_state != GME_PLY_SEQ_STATE_GMK_SPIPE &&
			!(ply_work->player_flag & GMD_PLF_PINBALL_SONIC)) {
		// SE
		GmSoundPlaySE("Spin");
	}

	// エフェクト スピンダッシュ 砂煙
	GmPlyEfctCreateSpinDashDust(ply_work);

	// エフェクト スーパーソニックオーラー
	GmPlyEfctCreateSuperAuraSpin(ply_work);

	// エフェクト ブラー
	GmPlyEfctCreateSpinDashBlur(ply_work, 1/*小*/);
	GmPlyEfctCreateSpinDashCircleBlur(ply_work);

	// 軌跡エフェクト作成
	GmPlyEfctCreateTrail(ply_work, GME_PLY_EFCT_TRAIL_TYPE_SPINDASH);
}

// ==========================================================================
// gmPlySeqSpinMain
/*!
 *	スピン
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqSpinMain(GMS_PLAYER_WORK *ply_work)
{
	UNREFERENCED_PARAMETER(ply_work);

	// 終了チェックは gmPlySeqCheckEndSpin で
//	// 一定速度以下で終了
//	if ((ply_work->obj_work.spd_m < GMD_PL_STOP_SPD) && (ply_work->obj_work.spd_m > -GMD_PL_STOP_SPD)) {
//		ply_work->obj_work.spd_m = 0;
//		GmPlayerSpdParameterSet(ply_work);
//		// FWへ
//		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
//		return;
//	}
}


// ==========================================================================
// スピンダッシュ
// ==========================================================================
// ==========================================================================
// GmPlySeqInitSpinDashAcc
/*!
 *	スピンダッシュ加速 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitSpinDashAcc(GMS_PLAYER_WORK *ply_work)
{
	if (!(ply_work->act_state == GME_PLY_ACT_STATE_SPIN_ACC ||
			ply_work->act_state == GME_PLY_ACT_STATE_SPIN_DASH ||
			ply_work->act_state == GME_PLY_ACT_STATE_SPIN_SHIFT)) {
		// スピン移行エフェクト呼び出し
		GmPlyEfctCreateSpinStartBlur(ply_work);
	}

	// アクション変更
	if (ply_work->efct_spin_start_blur) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN_SHIFT);
	}
	else {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN_ACC);
	}
	
	//ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	if (ply_work->dash_power) {
		// 加速
		ply_work->dash_power = ObjSpdUpSet(ply_work->dash_power, ply_work->spd_add_spin, ply_work->spd_max_spin);
	}
	else {
		ply_work->dash_power = ply_work->spd_spin;
	}

	// メイン処理設定
    ply_work->seq_func = gmPlySeqSpinDashMain;

	// 攻撃設定
	GmPlayerSetAtk(ply_work);

    // SE
#if _IPHONE
	if (ply_work->spin_se_timer <= 0) {
		GmSoundPlaySE("Dash1");
		ply_work->spin_se_timer = GMD_PLY_SEQ_PLAY_SE_TIMER;
	}
	if (ply_work->spin_back_se_timer <= 0) {
		GmSoundPlaySE("Dash2");
		ply_work->spin_se_timer = GMD_PLY_SEQ_PLAY_BACK_SE_TIMER;
	}
#else
	GmSoundPlaySE("Dash1");
	GmSoundPlaySE("Dash2");
#endif // _IPHONE

	// エフェクト スピン加速 砂煙
	if (ply_work->prev_seq_state != GME_PLY_SEQ_STATE_SPIN_DASHACC) {
		GmPlyEfctCreateSpinAddDust(ply_work);
	}
}

// ==========================================================================
// GmPlySeqInitSpinDash
/*!
 *	スピンダッシュ 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitSpinDash(GMS_PLAYER_WORK *ply_work)
{
	if (!(ply_work->act_state == GME_PLY_ACT_STATE_SPIN_ACC ||
			ply_work->act_state == GME_PLY_ACT_STATE_SPIN_DASH ||
			ply_work->act_state == GME_PLY_ACT_STATE_SPIN_SHIFT)) {
		// スピン移行エフェクト呼び出し
		GmPlyEfctCreateSpinStartBlur(ply_work);
	}

	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN_DASH);
	
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqSpinDashMain;

	// 攻撃設定
	GmPlayerSetAtk(ply_work);

    // SE
	//GmSoundPlaySE("Dash1");
	//GmSoundPlaySE("Dash2");

	// エフェクト その場スピン 砂煙
	GmPlyEfctCreateSpinDust(ply_work);
}

// ==========================================================================
// gmPlySeqSpinDashMain
/*!
 *	スピンダッシュ
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqSpinDashMain(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->act_state == GME_PLY_ACT_STATE_SPIN_SHIFT &&
			!ply_work->efct_spin_start_blur) {
		// アクション変更
		if (ply_work->seq_state == GME_PLY_SEQ_STATE_SPIN_DASHACC) {
			// スピンダッシュへ移行
			GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SPIN_DASH);
			return;
		}
		else {
			// スピンダッシュアクションに変更
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN_DASH);
		}
	}

	if (ply_work->act_state == GME_PLY_ACT_STATE_SPIN_ACC) {
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// スピンダッシュへ移行
			GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SPIN_DASH);
			return;
		}
	}
	
    // ダッシュ開始
	if (!(ply_work->key_on & PAD_KEY_DOWN) || ply_work->obj_work.spd_m) {
		fx32	spd_m;

		ply_work->no_spddown_timer = GMD_PL_SPINDASH_NOSPD_TIME;
		ply_work->camera_stop_timer = 8*FX32_ONE;

        // 速度設定
		spd_m = GMD_PL_SPINDASH_SPD + FX_Mul(ply_work->dash_power, GMD_PL_SPINDASH_MUL);
		if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
			spd_m = -spd_m;
		}
		if (MTM_MATH_ABS(spd_m) > MTM_MATH_ABS(ply_work->obj_work.spd_m)) {
			// ダッシュ速度の方が大きければ設定
			ply_work->obj_work.spd_m = spd_m;
		}
		
#if _IPHONE
		// スピンダッシュ用パワー初期化
		ply_work->dash_power = 0;
#endif // _IPHONE
		
		// 1フレームだけ壁HITしても速度クリアを行わない
		ply_work->obj_work.move_flag |= OBD_MOVE_NOSPD;

        // スピンへ
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SPIN);

		// スピン発動
		// エフェクト スピンダッシュ 衝撃波
		GmPlyEfctCreateSpinDashImpact(ply_work);

		// コントローラー振動
		GMM_PAD_VIB_SMALL();
		return;
	}

	// ◆後で減速
    //ply_work->dash_power = ObjSpdDownSet( ply_work->dash_power, (ply_work->dash_power >> GMD_PL_SPIN_SPDDA_SHIFT));

    if (ply_work->key_on & PAD_KEY_UP) {
        // 立ち上がる
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
        return;
    }
}

// ==========================================================================
// よろけ
// ==========================================================================
// ==========================================================================
// GmPlySeqInitStaggerFront
/*!
 *	よろけ前 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitStaggerFront(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_STAGGER_F);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqStaggerMain;
    //ply_work->obj_work.user_timer = 0;

	// エフェクト 汗
	GmPlyEfctCreateSweat(ply_work);
}

// ==========================================================================
// GmPlySeqInitStaggerBack
/*!
 *	よろけ後ろ 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitStaggerBack(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_STAGGER_B);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqStaggerMain;
    //ply_work->obj_work.user_timer = 0;

	// エフェクト 汗
	GmPlyEfctCreateSweat(ply_work);
}

// ==========================================================================
// GmPlySeqInitStaggerDanger
/*!
 *	よろけ危険 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitStaggerDanger(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_STAGGER_D);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqStaggerMain;
    //ply_work->obj_work.user_timer = 0;

	// エフェクト 汗
	GmPlyEfctCreateSweat(ply_work);
}

// ==========================================================================
// gmPlySeqStaggerMain
/*!
 *	よろけ
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqStaggerMain(GMS_PLAYER_WORK *ply_work)
{
	UNREFERENCED_PARAMETER(ply_work);
}

// ==========================================================================
// 落下
// ==========================================================================
// ==========================================================================
// GmPlySeqInitFall
/*!
 *	落下 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitFall(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	if (  (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY())							// スペステでは切り替えない
		||(  (ply_work->player_flag & GMD_PLF_SUPER_SONIC)					// スーパーソニックで
		   &&(  (ply_work->act_state == GME_PLY_ACT_STATE_DASH_1)			//   最高速アクションの時は
			  ||(ply_work->act_state == GME_PLY_ACT_STATE_DASH_2) ) )		//   切り替えない
		||(ply_work->player_flag & GMD_PLF_PINBALL_SONIC) 					// ピンボールの時は切り替えない
		||(ply_work->prev_seq_state == GME_PLY_SEQ_STATE_GMK_STOPPER) ) {	// 直前がストッパーの時は切り替えない
		// アクション切り替え無し
	} else {
		// アクション切り替える
//	if (!GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {
//		// 通常シーケンス(スペステじゃない)
//		if ((ply_work->player_flag & GMD_PLF_SUPER_SONIC) &&
//				(ply_work->act_state == GME_PLY_ACT_STATE_DASH_1 ||
//					ply_work->act_state == GME_PLY_ACT_STATE_DASH_2)) {
//			// スーパーソニック最高速アクションの場合は切り替えない
//		}
//		else {
//			if (!(ply_work->player_flag & GMD_PLF_PINBALL_SONIC)) {	// ピンボールの時は切り替えない
				if ((u16)(ply_work->obj_work.dir.z - 0x2000) <= 0xC000) {
					// 90度こえた
					GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_FALL_R);
				}
				else {
					GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_FALL);
				}
//			}
//		}
//	} else {
//		// スペステも切り替えない
	}

	GmPlySeqInitFallState(ply_work);
}

// ==========================================================================
// GmPlySeqInitFall
/*!
 *	落下 初期化(アクション変更したくない場合に対応するためステート変更部のみ切り出し)
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitFallState(GMS_PLAYER_WORK *ply_work)
{
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM | OBD_MOVE_FALL;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_UNDER;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqJumpMain;
    //ply_work->seq_func = gmPlySeqFallMain;

	// 速度設定
#if 0	// 既存
	ply_work->obj_work.spd.x = FX_Mul(	ply_work->obj_work.spd_m, mtMathCos(ply_work->obj_work.dir.z));
	ply_work->obj_work.spd.y = FX_Mul(	ply_work->obj_work.spd_m, mtMathSin(ply_work->obj_work.dir.z));
#elif 0	// スペステ画面回転対応
	ply_work->obj_work.spd.x = FX_Mul(	ply_work->obj_work.spd_m, mtMathCos(ply_work->obj_work.dir.z - (g_gm_main_system.pseudofall_dir - ply_work->obj_work.dir_fall)));
	ply_work->obj_work.spd.y = FX_Mul(	ply_work->obj_work.spd_m, mtMathSin(ply_work->obj_work.dir.z - (g_gm_main_system.pseudofall_dir - ply_work->obj_work.dir_fall)));
#else	// スペステ画面回転対応
	ply_work->obj_work.spd.x = FX_Mul(	ply_work->obj_work.spd_m, mtMathCos(ply_work->obj_work.dir.z - (g_gm_main_system.pseudofall_dir - ply_work->prev_dir_fall2)));
	ply_work->obj_work.spd.y = FX_Mul(	ply_work->obj_work.spd_m, mtMathSin(ply_work->obj_work.dir.z - (g_gm_main_system.pseudofall_dir - ply_work->prev_dir_fall2)));
	ply_work->obj_work.spd_m = 0;
#endif	// スペステ画面回転対応

	// 攻撃矩形設定OFF◆
	//ObjRectAtkSet(&ply_work->rect_work[1], 0, 0);
	//ObjRectDefSet(&ply_work->rect_work[1], 0, 0);
	//ObjRectDefSet(&ply_work->rect_work[1], GMD_OBJ_RECT_DEF_FLAG_INVINCIBLE, GMD_OBJ_RECT_DEF_POWER_INVINCIBLE);
    //objRectAttrSet(&ply_work->rect_work[1], 0, 0);

	ply_work->player_flag &= ~GMD_PLF_USER_MASK;
	ply_work->player_flag |= GMD_PLF_USER1;		// ジャンプボタン無視

#if 0	// ◆不要コードの為切っておく
    if ( (ply_work->obj_work.col_flag | ply_work->obj_work.col_flag_prev) & OBD_COLAT_CLIFF && ply_work->obj_work.spd.y < -0x00800) {
        ply_work->player_flag |= GMD_PLF_USER1;
	}
#endif

	ply_work->obj_work.user_timer	= 0;
	ply_work->obj_work.user_work	= 0;
	ply_work->timer					= 0;
}


// ==========================================================================
// ジャンプ
// ==========================================================================
// ==========================================================================
// GmPlySeqInitJump
/*!
 *	ジャンプ 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitJump(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	if (!(ply_work->player_flag & GMD_PLF_PINBALL_SONIC)) {		// ピンボール状態の時は切り替えない
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_SPIN);
	}

	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM;
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_UNDER | OBD_MOVE_UNDERPREV);

	// メイン処理設定
    //ply_work->seq_func = gmPlySeqJumpMain;	GmPlySeqSetJumpStateで切り替え

	// 速度設定
#if 1
	// 20090827
	{
		u16	dir;
		dir = ply_work->obj_work.dir.z;

		if (((dir + 0x100/*45度角度取得時の補正値*/) & 0x2000) &&
				(((dir + 0x100) & 0x0FFF) <= 0x400)) {
			// 45度角付近の時
			// 下りの時のみ
			if (ply_work->obj_work.spd_m > 0 &&
					dir < 0x8000) {
				dir -= 0x480;

			}
			else if (ply_work->obj_work.spd_m < 0 &&
					dir > 0x8000) {
				dir += 0x480;
			}
		}


		ply_work->obj_work.spd.x = FX_Mul(ply_work->obj_work.spd_m, mtMathCos(dir));
		ply_work->obj_work.spd.y = FX_Mul(ply_work->obj_work.spd_m, mtMathSin(dir));
		ply_work->obj_work.spd.x += FX_Mul(ply_work->spd_jump, mtMathSin(ply_work->obj_work.dir.z));
		ply_work->obj_work.spd.y += FX_Mul(-ply_work->spd_jump, mtMathCos(ply_work->obj_work.dir.z));
	}

#else
	ply_work->obj_work.spd.x = FX_Mul(ply_work->obj_work.spd_m, mtMathCos(ply_work->obj_work.dir.z));
	ply_work->obj_work.spd.y = FX_Mul(ply_work->obj_work.spd_m, mtMathSin(ply_work->obj_work.dir.z));
    ply_work->obj_work.spd.x += FX_Mul(ply_work->spd_jump, mtMathSin(ply_work->obj_work.dir.z));
    ply_work->obj_work.spd.y += FX_Mul(-ply_work->spd_jump, mtMathCos(ply_work->obj_work.dir.z));
#endif

    // 攻撃設定
	//ObjRectAtkSet(&ply_work->rect_work[1], GMD_OBJ_RECT_HIT_FLAG_NORMAL, GMD_OBJ_RECT_HIT_POWER_DEFAULT);

    // 壁張り付き対応
	if (ply_work->gmk_flag & GMD_PLGF_GMK_WALL) {
		ply_work->obj_work.spd.z = ply_work->obj_work.spd.y;
		ply_work->obj_work.spd.y = 0;
		if ( ply_work->obj_work.pos.z < 0 ) {
			ply_work->obj_work.spd.z = -ply_work->obj_work.spd.z;
		}
    }

	ply_work->player_flag &= ~GMD_PLF_USER_MASK;
//	ply_work->player_flag &= ~(GMD_PLF_TRICK_SP | GMD_PLF_TRICK);
	

#if 0	// ◆不要コードの為切っておく
    if ( (ply_work->obj_work.col_flag | ply_work->obj_work.col_flag_prev) & OBD_COLAT_CLIFF && ply_work->obj_work.spd.y < -0x00800) {
        ply_work->player_flag |= GMD_PLF_USER1;
	}
#endif

	ply_work->obj_work.user_timer	= 0;
	ply_work->obj_work.user_work	= 0;
	ply_work->timer					= 0;

	// ジャンプステータス設定
	GmPlySeqSetJumpState(ply_work, 0, 0);

	// 前のシーケンスがスピンの時は スピンダッシュジャンプ減速OFF時間を設定
	if (ply_work->prev_seq_state == GME_PLY_SEQ_STATE_SPIN) {
		if (ply_work->no_spddown_timer >= GMD_PL_SPINDASH_JUMP_NOSPD_TIME) {
			ply_work->no_spddown_timer = GMD_PL_SPINDASH_JUMP_NOSPD_TIME;
		}
	}

    // 水中泡表示

	// 攻撃設定
	GmPlayerSetAtk(ply_work);

    // SE
	if (gm_ply_seq_jump_call_se_jump) {
		GmSoundPlaySE("Jump");
	}

	// エフェクト ジャンプ煙
	GmPlyEfctCreateJumpDust(ply_work);
	// エフェクト ブラー
	GmPlyEfctCreateSpinJumpBlur(ply_work);
}

// ==========================================================================
// GmPlySeqSetJumpState
/*!
 *	ジャンプ ステータス初期化
 *
 *	@param	ply_work		[in]	プレイヤーワーク
 *	@param	nofall_timer	[in]	FALL計算を行わない時間
 *	@param	flag			[in]	各種設定フラグ GMD_PLY_SEQ_SETJUMPSTATE_***
 *
 *	@note
 *		ジャンプシーケンス用のステータス設定を行い、\n
 *		処理をジャンプ(gmPlySeqJumpMain)に切り替えます
 */
// ==========================================================================
void GmPlySeqSetJumpState(GMS_PLAYER_WORK *ply_work, s32 nofall_timer, u32 flag)
{
	ply_work->obj_work.user_timer = nofall_timer;

	if (!ply_work->no_jump_move_timer) {
		ply_work->player_flag &= ~(GMD_PLF_NOJUMPMOVE);
	}

	ply_work->player_flag &= ~(GMD_PLF_USER_MASK | GMD_PLF_NOHOMING);
	//ply_work->no_jump_move_timer = 0;

	// ジャンプボタン無視
	if (flag & GMD_PLY_SEQ_SETJUMPSTATE_IGNORE_JUMPBTN) {
		ply_work->player_flag |= GMD_PLF_USER1;
	}
	// ギミックからのジャンプフラグ
	if (flag & GMD_PLY_SEQ_SETJUMPSTATE_GMK_JUMP) {
		ply_work->player_flag |= GMD_PLF_USER2;
	}
	// ホーミング不可に
	if (flag & GMD_PLY_SEQ_SETJUMPSTATE_NOHOMING) {
		ply_work->player_flag |= GMD_PLF_NOHOMING;
	}
	// ジャンプ中移動不可に
	if (flag & GMD_PLY_SEQ_SETJUMPSTATE_NOJUMPMOVE) {
		ply_work->player_flag |= GMD_PLF_NOJUMPMOVE;
	}
	// メイン処理設定
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		ply_work->seq_func = gmPlySeqTruckJumpMain;
	}
	else {
		ply_work->seq_func = gmPlySeqJumpMain;
	}
}

// ==========================================================================
// GmPlySeqInitJumpEX
/*!
 *	ジャンプ 特殊初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_x		[in]	ジャンプ速度X
 *	@param	spd_y		[in]	ジャンプ速度Y
 */
// ==========================================================================
void GmPlySeqInitJumpEX(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y)
{
	// ジャンプシーケンスへ移行
	GmPlySeqInitJump(ply_work);

	// 速度再設定
	ply_work->obj_work.spd.x = spd_x;
	ply_work->obj_work.spd.y = spd_y;

	ply_work->obj_work.spd_m = 0;


	// 向き設定
	if (ply_work->obj_work.spd.x < 0) {
		if (ply_work->obj_work.spd_m > 0) {
			ply_work->obj_work.spd_m = 0;
		}
		if (!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP)) {
			GmPlayerSetReverse(ply_work);
		}
	}
	else {
		if (ply_work->obj_work.spd_m < 0) {
			ply_work->obj_work.spd_m = 0;
		}
		if ((ply_work->obj_work.disp_flag & OBD_DISP_HFLIP)) {
			GmPlayerSetReverse(ply_work);
		}
	}
}

// ==========================================================================
// GmPlySeqAtkReactionInit
/*!
 *	攻撃跳ね返り 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		spd.x, spd_m は、初期化前の速度を維持する
 */
// ==========================================================================
void GmPlySeqAtkReactionInit(GMS_PLAYER_WORK *ply_work)
{
	fx32	spd_x, spd_m;

	if (ply_work->seq_state == GME_PLY_SEQ_STATE_HOMING) {
	// ホーミング中
		// シーケンスをホーミング跳ね返りへ
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_HOMING_REF);
	}
	else if (ply_work->obj_work.move_flag & OBD_MOVE_JUMP) {
	// 通常攻撃
		// 速度退避
		spd_x = ply_work->obj_work.spd.x;
		spd_m = ply_work->obj_work.spd_m;
		// ステート初期化
		GmPlayerStateInit(ply_work);

		// シーケンスをジャンプへ
		gm_ply_seq_jump_call_se_jump = FALSE;	// ジャンプSE OFF
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_JUMP);
		gm_ply_seq_jump_call_se_jump = TRUE;	// ジャンプSE ON

		// ジャンプステート設定
		GmPlySeqSetJumpState(ply_work, 0, GMD_PLY_SEQ_SETJUMPSTATE_IGNORE_JUMPBTN);

		// 跳ね速度設定
		ply_work->obj_work.spd.y = GMD_PL_ATK_REF_Y;
		ply_work->obj_work.spd.x = spd_x;
		ply_work->obj_work.spd_m = spd_m;
	}

#if 0
	if (ply_work->obj_work.move_flag & OBD_MOVE_JUMP) {
		// ジャンプ中のみ
		// 速度退避
		spd_x = ply_work->obj_work.spd.x;
		spd_m = ply_work->obj_work.spd_m;

		// ステート初期化
		GmPlayerStateInit(ply_work);

		// シーケンスをジャンプへ
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_JUMP);

		// ジャンプステート設定
		GmPlySeqSetJumpState(ply_work, 0, jump_state);

		// 移動不可時間設定
		if (no_jump_move_time) {
			GmPlySeqSetNoJumpMoveTime(ply_work, no_jump_move_time);
		}

		// 跳ね速度設定
		ply_work->obj_work.spd.y = GMD_PL_ATK_REF_Y;
		ply_work->obj_work.spd.x = spd_x;
		ply_work->obj_work.spd_m = spd_m;
	}
#endif
}

// ==========================================================================
// GmPlySeqAtkReactionSpdInit
/*!
 *	攻撃跳ね返り 速度設定つき 初期化
 *
 *	@param	ply_work			[in]	プレイヤーワーク
 *	@param	spd_x				[in]	速度X
 *	@param	no_spddown_timer	[in]	スピードダウンなし時間
 */
// ==========================================================================
void GmPlySeqAtkReactionSpdInit(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 no_spddown_timer)
{
	ply_work->obj_work.spd.x	= spd_x;
	ply_work->no_spddown_timer	= no_spddown_timer;

	GmPlySeqAtkReactionInit(ply_work);
}

// ==========================================================================
// gmPlySeqJumpMain
/*!
 *	ジャンプ～落下
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_tiemr	FALL計算を行わない時間
 *	//user_work	先押しチェック用
 *	//timer		トリック可能チェックタイマ
    GMD_PLF_USER1をジャンプボタン無視フラグとして使用\n
    GMD_PLF_USER2をGimmickからのジャンプフラグとして使用\n
    GMD_PLF_USER3を小ジャンプ有効化フラグとして使用
 */
// ==========================================================================
void gmPlySeqJumpMain(GMS_PLAYER_WORK *ply_work)
{
   fx32	check_spd = ply_work->obj_work.spd.y;

    // 壁張り付きギミック対応
    if ( ply_work->gmk_flag & GMD_PLGF_GMK_WALL ) {
        check_spd = ply_work->obj_work.spd.z;
        if ( ply_work->obj_work.dir.x > 0x8000 ){
            check_spd = -check_spd;
        }
    }    
    // 角度を徐々に０に戻す
    //GmPlySeqJumpDirec(ply_work);

    // 移動速度設定
    //GmPlySeqMoveJump(ply_work);
    
    // FALLフラグ復活タイマ
    if ( ply_work->obj_work.user_timer ){
        --ply_work->obj_work.user_timer;
        if ( !ply_work->obj_work.user_timer )
            ply_work->obj_work.move_flag |= OBD_MOVE_FALL;
    }
    
#if 1
	if (!(ply_work->player_flag & (GMD_PLF_USER1 | GMD_PLF_USER3))) {
		if (!(GmPlayerKeyCheckJumpKeyOn(ply_work)) ){
			if (check_spd < GMD_PL_SJUMP_SPD) {
				// 小ジャンプ有効化
				ply_work->player_flag |= GMD_PLF_USER3;
			}
		}
	}

	if (ply_work->player_flag & GMD_PLF_USER3) {
		// 一定速度になるまで落下加速度を+
		if (ply_work->obj_work.spd.y < 0) {
			ply_work->obj_work.spd.y += ply_work->obj_work.spd_fall;
		}
	}

#else
    // ボタンによる落下切り替えチェック
    if (!( ply_work->player_flag & GMD_PLF_USER1 )) {
        if ( !(GmPlayerKeyCheckJumpKeyOn(ply_work)) ){
            if (check_spd < GMD_PL_SJUMP_SPD) {
                if ( ply_work->gmk_flag & GMD_PLGF_GMK_WALL )
                    ply_work->obj_work.spd.z = 0x0;
                else
                    ply_work->obj_work.spd.y = -0x0040;
            }
        }
    }

    // 落下時
    if ( check_spd > 0x00000000 ){
        // 以降はジャンプボタン離しチェックを行わない
        ply_work->player_flag |= GMD_PLF_USER1;
    }
#endif

    /* アクション移行チェック */
	{
	/* 通常 */
	    switch ( ply_work->act_state ) {
	    case GME_PLY_ACT_STATE_JUMP_SPIN:				// ジャンプ(スピンタイプ)
	        // エフェクト
	        break;

		/* バネジャンプ */
	    case GME_PLY_ACT_STATE_JUMP_G01:				// ギミックジャンプ上昇ループ
	        if ( check_spd > 0x00000400){
	        	// ギミックジャンプ下降開始へ
	            GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_JUMP_G02 );
	        }
	        break;

	    case GME_PLY_ACT_STATE_JUMP_G02:				// ギミックジャンプ下降開始
	        // バネジャンプアクション経過
	        if ( ply_work->obj_work.disp_flag & OBD_DISP_END ){
	        	// ギミックジャンプ下降ループへ
	            GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_JUMP_G03 );
	            ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	        }
	        break;

	    case GME_PLY_ACT_STATE_JUMP_GF01:				// 前転ジャンプ
	        if ( ply_work->obj_work.disp_flag & OBD_DISP_END ){
	        	// 前転ジャンプ下降ループへ
	            ply_work->obj_work.disp_flag |= OBD_DISP_3D_BLEND;
	            GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_JUMP_GF02 );
	            ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	        }
	        break;

//	    case GME_PLY_ACT_STATE_JUMP_F01:				// 前方ジャンプループ
//	        if ( check_spd > 0x00001c00){
//				// 前方ジャンプ下降ループへ
//	            ply_work->obj_work.disp_flag |= OBD_DISP_3D_BLEND;
//	            GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_JUMP_F02 );
//	            ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
//	        }
//	        break;

		/* ギミック対応 */

#if 0	// トリック対応
		/* 上昇トリック */
		case GMD_PLY_STATE_TR_A00:	// 上昇トリック 溜め
	        if (ply_work->obj_work.disp_flag & OBD_DISP_END ) {
	            GmPlayerActionChange(ply_work, GMD_PLY_STATE_TR_A01);	// 上昇トリック 上昇
				ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

				ply_work->obj_work.move_flag &= ~OBD_MOVE_NOMOVE;		// 溜め停止終了
	        }
	        else if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {
				// 接地した時は解除しておく
				ply_work->obj_work.move_flag &= ~OBD_MOVE_NOMOVE;		// 溜め停止終了
	        }
			break;

		case GMD_PLY_STATE_TR_A01:	// 上昇トリック 上昇
			if (check_spd > -0x00000200) {
				// 下降つなぎへ
				ply_work->obj_work.disp_flag |= OBD_DISP_3D_BLEND;
				GmPlayerActionChange( ply_work, GMD_PLY_STATE_TR_A02);
			}
			if (ply_work->char_no == GSD_PLAYER_BLAZE) {
			// ブレイズ
				// エフェクトあり
	            if (!(ply_work->trick_fire_timer & 0x3)) {
	                GmEffectInitPlayerTrickFire(ply_work);
	            }
	            ++ply_work->trick_fire_timer;
			}
			break;

		case GMD_PLY_STATE_TR_A02:	// 上昇トリック 下降つなぎ
			if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
				// 専用下降ループへ
				GmPlayerActionChange( ply_work, GMD_PLY_STATE_TR_A03);
				ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
			}

			break;

		/* 横移動トリック */
	    case GMD_PLY_STATE_TR_A10:	// 横移動トリック 溜め
	        if (ply_work->obj_work.disp_flag & OBD_DISP_END ) {
	            GmPlayerActionChange(ply_work, GMD_PLY_STATE_TR_A11);	// 横移動トリック 横移動

				ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
#if GMD_PLAYER_TRICK_ACT1_NOSTOP
#else
				ply_work->obj_work.move_flag &= ~OBD_MOVE_NOMOVE;		// 溜め停止終了
#endif
	        }
#if GMD_PLAYER_TRICK_ACT1_NOSTOP
			if ( ply_work->char_no == GSD_PLAYER_BLAZE ) {
			// ブレイズ瞬間移動演出
				if (ply_work->blaze_timer) {
					// 一定時間消える
					ply_work->obj_work.disp_flag ^= OBD_DISP_NODISP;

					// 一定時間落下せず
					ply_work->obj_work.spd.y = 0;

					--ply_work->blaze_timer;
					if (!ply_work->blaze_timer) {
						// 横トリック終了
						ply_work->obj_work.disp_flag &= ~OBD_DISP_NODISP;
						ply_work->obj_work.spd.x = ply_work->blaze_prev_spd;
						// ヒットストップ
						ply_work->obj_work.hitstop_timer	= GMD_PLAYER_HIT_STOP_TIME * FX32_ONE;
						ply_work->obj_work.vib_timer		= GMD_PLAYER_HIT_STOP_TIME * FX32_ONE;

						ply_work->rect_work[0].flag &= ~(OBD_RECT_HIT | OBD_RECT_DAMAGE);	// HIT復帰
					}
				}
			}
#endif	// GMD_PLAYER_TRICK_ACT1_NOSTOP
			break;

	    case GMD_PLY_STATE_TR_A11:	// 横移動トリック 横移動
			if ( ply_work->char_no == GSD_PLAYER_BLAZE ) {
			// ブレイズ瞬間移動演出
				if (ply_work->blaze_timer) {
					// 一定時間消える
					ply_work->obj_work.disp_flag ^= OBD_DISP_NODISP;

					// 一定時間落下せず
					ply_work->obj_work.spd.y = 0;

					--ply_work->blaze_timer;
					if (!ply_work->blaze_timer) {
						// 横トリック終了
						ply_work->obj_work.disp_flag &= ~OBD_DISP_NODISP;
						ply_work->obj_work.spd.x = ply_work->blaze_prev_spd;
						// ヒットストップ
						ply_work->obj_work.hitstop_timer	= GMD_PLAYER_HIT_STOP_TIME * FX32_ONE;
						ply_work->obj_work.vib_timer		= GMD_PLAYER_HIT_STOP_TIME * FX32_ONE;

						ply_work->rect_work[0].flag &= ~(OBD_RECT_HIT | OBD_RECT_DAMAGE);	// HIT復帰
					}
				}
			}
			break;

		/* ジャストトリック */
	    case GMD_PLY_STATE_TR00:
	        if ( !(g_gm_game_info.time & 0x07)){
	            // ジャストエフェクト
				GmEffectInitPlayerJustTrick(ply_work);
	        }
	        if ( ply_work->obj_work.disp_flag & OBD_DISP_END ){
	            GmPlayerActionChange( ply_work, GMD_PLY_STATE_JUMP3 );
	            ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	        }
	        break;
#endif
		default:
			break;

	    }	// switch ( ply_work->act_state )
	}

	/* 着地チェック */
	if ( ply_work->obj_work.move_flag & OBD_MOVE_UNDER ) {
		// 着地設定
		//ply_work->rect_work[0].flag &= ~(OBD_RECT_HIT | OBD_RECT_DAMAGE);	// HIT復帰
		GmPlySeqLandingSet(ply_work, 0);
		//ply_work->trick_combo = 0;			// トリックコンボ終了

		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);

		return;
	}


#if 0
    if ( ply_work->player_flag & GMD_PLF_USER2 ) {
	// Gimmickからのジャンプ中行動チェック
        if ( !(ply_work->player_flag & GMD_PLF_USER4) ) {
		// 移動トリックをまだ使用していない
			if (!((GMD_PLY_STATE_MOVE_TRICK_START <= ply_work->act_state && ply_work->act_state <= GMD_PLY_STATE_MOVE_TRICK_END) ||	// 通常
					(GMD_PLY_STATE_GMK_DHB_MOVE_TRICK_START <= ply_work->act_state && ply_work->act_state <= GMD_PLY_STATE_GMK_DHB_MOVE_TRICK_END))) {	// 滑降
			// 移動トリック中以外
                if ( ply_work->key_push & PAD_BUTTON_R && ply_work->key_on & PAD_KEY_UP ) {
				// 上昇トリック
                    gmPlayerTrickAction0Init(ply_work);
                    return;
                }
                if ( ply_work->key_push & PAD_BUTTON_R ){
                // 横移動トリック
                    gmPlayerTrickAction1Init(ply_work);
                    return;
                }
            }
        }

        if ( (ply_work->key_push & PAD_BUTTON_JUMP ) ) {
            // 先押しチェック
            ply_work->obj_work.user_work = ply_work->key_push;
        }

		/* トリック可能タイマ設定 */
        if ( (ply_work->act_state >= GMD_PLY_STATE_TR_A10 && ply_work->act_state <= GMD_PLY_STATE_TR30) ||						// 通常
        		 (ply_work->act_state >= GMD_PLY_STATE_GMK_DHB_TR_A00 && ply_work->act_state <= GMD_PLY_STATE_GMK_DHB_TR_30)) {	// 滑降ボード

            // ブレイズの横トリックは落下速度が一定以上になるまでトリック不可
#if 1
			if (ply_work->char_no == GSD_PLAYER_BLAZE) {
				if (check_spd < 0x00000000 &&	// 上昇中
						(ply_work->act_state == GMD_PLY_STATE_TR_A10 || ply_work->act_state == GMD_PLY_STATE_TR_A11 ||
							ply_work->act_state == GMD_PLY_STATE_GMK_DHB_TR_A10 || ply_work->act_state == GMD_PLY_STATE_GMK_DHB_TR_A11)) {
					// トリック可能チェックタイマクリア
					ply_work->timer = 0;
				}
			}
#else
            if ( ply_work->act_state == GMD_PLY_STATE_TR_A10 &&
					ply_work->char_no == GSD_PLAYER_BLAZE &&
					check_spd < 0x00000000 ) {
				ply_work->timer = 0;
			}
#endif
            // 経過タイマー
            ++ply_work->timer;
			if (ply_work->timer > 0xff) {
				ply_work->timer = 0xff;
			}
        }

		/*トリックコンボチェック */
#if 1
		if ( !(ply_work->player_flag & GMD_PLF_USER3) ) {	// トリック派生が終了していない時
			s32	trick_permit_frame;
			if (ply_work->hs_trick_timer) {
				// 高速トリック時
				trick_permit_frame = 12;
			}
			else {
				trick_permit_frame = 24;
			}

			if ( ply_work->timer > trick_permit_frame ||
					(!(GMD_PLY_STATE_TR_A10 <= ply_work->act_state && ply_work->act_state <= GMD_PLY_STATE_TR30) &&
					!(GMD_PLY_STATE_GMK_DHB_TR_A10 <= ply_work->act_state && ply_work->act_state <= GMD_PLY_STATE_GMK_DHB_TR_30)) ) {
			// タイマが規定値以上か横移動トリック中・テンショントリック中でない場合
				if ( ply_work->obj_work.user_work ) {	// ボタンが押されている
    				ply_work->obj_work.move_flag &= ~OBD_MOVE_NOMOVE;		// 移動無しになっている場合は解除
					ply_work->obj_work.disp_flag &= ~OBD_DISP_NODISP;		// 表示無しになっている場合は解除

					ply_work->rect_work[0].flag &= ~(OBD_RECT_HIT | OBD_RECT_DAMAGE);	// HIT復帰
					GmPlayerTrickInit(ply_work);
                    return;
                }
            }
        }
#else
		if ( !(ply_work->player_flag & GMD_PLF_USER3) ) {	// トリック派生が終了していない時
			if ( ply_work->act_state < GMD_PLY_STATE_TR_A10 || ply_work->timer > 24 ) {
				if ( ply_work->obj_work.user_work ) {
    				ply_work->obj_work.move_flag &= ~OBD_MOVE_NOMOVE;		// 移動無しになっている場合は解除
					GmPlayerTrickInit(ply_work);
                    return;
                }
            }
        }
#endif
    }
#endif
}


// ==========================================================================
// 壁押し
// ==========================================================================
// ==========================================================================
// GmPlySeqInitWallPush
/*!
 *	壁押し 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitWallPush(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_PUSH1);

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
	ply_work->seq_func = gmPlySeqWallPushMain;
}

// ==========================================================================
// gmPlySeqWallPushMain
/*!
 *	壁押し
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqWallPushMain(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->act_state == GME_PLY_ACT_STATE_PUSH1) {
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// アクション変更
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_PUSH2);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
	}
}

// ==========================================================================
// ホーミング
// ==========================================================================
// ==========================================================================
// GmPlySeqInitHoming
/*!
 *	ホーミング 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitHoming(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->enemy_obj == NULL) {
		// ターゲットがない場合はジャンプダッシュへ
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_JUMPDASH);
		return;
	}

	// アクション変更
	if (!(ply_work->player_flag & GMD_PLF_PINBALL_SONIC)) {	// ピンボールの時はきりかえない
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_HOMING);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}

	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM;
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_UNDER | OBD_MOVE_FALL);
	ply_work->player_flag |= GMD_PLF_NOHOMING;		// ホーミング不可
	ply_work->obj_work.dir.z = 0;

	// 自由接地OFF
	ply_work->gmk_flag &= ~(GMD_PLGF_TOUCH | GMD_PLGF_TOUCH_FORCE_DIR | GMD_PLGF_TOUCH_FLIP | GMD_PLGF_TOUCH_DISP_FLIP);

	// メイン処理設定
	ply_work->seq_func = gmPlySeqHomingMain;

	// ホーミング時間
	ply_work->obj_work.user_timer = GMD_PL_HOMING_TIME * FX32_ONE;

	// 再ホーミング待機時間
	ply_work->homing_timer = GMD_PL_HOMING_WAIT_TIME * FX32_ONE;

	// ホーミング範囲ブースト時間
	ply_work->homing_boost_timer = GMD_PL_HOMING_BOOST_TIME * FX32_ONE;

	// 攻撃設定
	GmPlayerSetAtk(ply_work);

	// エフェクト ホーミング衝撃波
	GmPlyEfctCreateHomingImpact(ply_work);

	// 軌跡エフェクト作成
	GmPlyEfctCreateTrail(ply_work, GME_PLY_EFCT_TRAIL_TYPE_HOMING);


	// SE
	GmSoundPlaySE("Homing");

}

// ==========================================================================
// gmPlySeqHomingMain
/*!
 *	ホーミング
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqHomingMain(GMS_PLAYER_WORK *ply_work)
{
	// 落下開始チェック
	if (!ply_work->obj_work.user_timer) {
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL);
		return;
	}
	ply_work->obj_work.user_timer = ObjTimeCountDown(ply_work->obj_work.user_timer);

	// 着地チェック
	if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {
		// 着地設定
		GmPlySeqLandingSet(ply_work, 0);
		// FWへ移行
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
		return;
	}

	// ホーミング移動設定
	if (ply_work->enemy_obj) {
		float	dist_x, dist_y;
		fx32	ene_pos_x, ene_pos_y;
		Angle32	dir;
		OBS_RECT_WORK	*rect_work;

		rect_work = &((GMS_ENEMY_COM_WORK*)ply_work->enemy_obj)->rect_work[GMD_ENEMY_RECT_BODY];

		ene_pos_x = ply_work->enemy_obj->pos.x;
		if (ply_work->enemy_obj->disp_flag & OBD_DISP_VFLIP) {
			ene_pos_y = ply_work->enemy_obj->pos.y - ((rect_work->rect.top + rect_work->rect.bottom)<<(FX32_SHIFT-1));
		}
		else {
			ene_pos_y = ply_work->enemy_obj->pos.y + ((rect_work->rect.top + rect_work->rect.bottom)<<(FX32_SHIFT-1));
		}

		dist_x = FXM_FX32_TO_FLOAT(ene_pos_x - ply_work->obj_work.pos.x);
		dist_y = FXM_FX32_TO_FLOAT(ene_pos_y - ply_work->obj_work.pos.y);

		dir = nnArcTan2(dist_y, dist_x);
        dir += ply_work->obj_work.dir_fall;
        ply_work->obj_work.spd.x = (fx32)(nnCos(dir) * GMD_PL_HOMING_SPD);
        ply_work->obj_work.spd.y = (fx32)(nnSin(dir) * GMD_PL_HOMING_SPD);

		// 向き設定
		if (ply_work->obj_work.spd.x < 0) {
			ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
		}
		else if (ply_work->obj_work.spd.x > 0) {
			ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
		}

		if ((MTM_MATH_ABS(ply_work->obj_work.spd.x) > 0x100) &&
					(ply_work->obj_work.move_flag & OBD_MOVE_FRONT)) {
			// 着地設定
			GmPlySeqLandingSet(ply_work, 0);
			// FWへ移行
			GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
			return;
		}
	}
}

// ==========================================================================
// ホーミング跳ね返り
// ==========================================================================
// ==========================================================================
// GmPlySeqInitHomingRef
/*!
 *	ホーミング跳ね返り 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitHomingRef(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	if (!(ply_work->player_flag & GMD_PLF_PINBALL_SONIC)) {	// ピンボールの時は切り替えない
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_HOMING_REF1);
		//GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_HOMING_REF2);
		// 2回切り替えで体をのばす
	}

	ply_work->player_flag &= ~GMD_PLF_NOHOMING;		// ホーミングを可能に

	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM | OBD_MOVE_FALL;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_UNDER;

	// メイン処理設定
	ply_work->seq_func = gmPlySeqHomingRefMain;

	// 速度設定
	ply_work->obj_work.spd.x = 0;
	if (ply_work->player_flag & GMD_PLF_WATER) {
		ply_work->obj_work.spd.y = GMD_PLAYER_WATERJUMP_GET(GMD_PL_HOMING_REF_SPD_Y);
	}
	else {
		ply_work->obj_work.spd.y = GMD_PL_HOMING_REF_SPD_Y;
	}
	ply_work->obj_work.spd_add.x = ply_work->obj_work.spd_add.y = 0;
	ply_work->obj_work.spd_m = 0;
	// ◆ 要重力方向調整

	ply_work->player_flag &= ~GMD_PLF_USER_MASK;
//	ply_work->player_flag &= ~(GMD_PLF_TRICK_SP | GMD_PLF_TRICK);

	ply_work->obj_work.user_timer	= 0;
	ply_work->obj_work.user_work	= 0;
	ply_work->timer					= 0;

	// ジャンプステータス設定
	//GmPlySeqSetJumpState(ply_work, 0, 0);

	// 水中泡表示

	// SE
	// 攻撃後の跳ね返りではJump音は無しに
	// GmSoundPlaySE("Jump");

	// エフェクト ジャンプ煙
	GmPlyEfctCreateJumpDust(ply_work);

#if 0
		// アクション変更
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_SPIN);

		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM;
		ply_work->obj_work.move_flag &= ~OBD_MOVE_UNDER;

		// メイン処理設定
		//ply_work->seq_func = gmPlySeqJumpMain;	GmPlySeqSetJumpStateで切り替え

		// 速度設定
		ply_work->obj_work.spd.x = FX_Mul(ply_work->obj_work.spd_m, mtMathCos(ply_work->obj_work.dir.z));
		ply_work->obj_work.spd.y = FX_Mul(ply_work->obj_work.spd_m, mtMathSin(ply_work->obj_work.dir.z));
		ply_work->obj_work.spd.x += FX_Mul(ply_work->spd_jump, mtMathSin(ply_work->obj_work.dir.z));
		ply_work->obj_work.spd.y += FX_Mul(-ply_work->spd_jump, mtMathCos(ply_work->obj_work.dir.z));

		// 攻撃設定
		//ObjRectAtkSet(&ply_work->rect_work[1], GMD_OBJ_RECT_HIT_FLAG_NORMAL, GMD_OBJ_RECT_HIT_POWER_DEFAULT);

		// 壁張り付き対応
		if (ply_work->gmk_flag & GMD_PLGF_GMK_WALL) {
			ply_work->obj_work.spd.z = ply_work->obj_work.spd.y;
			ply_work->obj_work.spd.y = 0;
			if ( ply_work->obj_work.pos.z < 0 ) {
				ply_work->obj_work.spd.z = -ply_work->obj_work.spd.z;
			}
		}

		ply_work->player_flag &= ~GMD_PLF_USER_MASK;
	//	ply_work->player_flag &= ~(GMD_PLF_TRICK_SP | GMD_PLF_TRICK);
		

	#if 0	// ◆不要コードの為切っておく
		if ( (ply_work->obj_work.col_flag | ply_work->obj_work.col_flag_prev) & OBD_COLAT_CLIFF && ply_work->obj_work.spd.y < -0x00800) {
			ply_work->player_flag |= GMD_PLF_USER1;
		}
	#endif

		ply_work->obj_work.user_timer	= 0;
		ply_work->obj_work.user_work	= 0;
		ply_work->timer					= 0;

		// ジャンプステータス設定
		GmPlySeqSetJumpState(ply_work, 0, 0);

		// 前のシーケンスがスピンの時は スピンダッシュジャンプ減速OFF時間を設定
		if (ply_work->prev_seq_state == GME_PLY_SEQ_STATE_SPIN) {
			if (ply_work->no_spddown_timer >= GMD_PL_SPINDASH_JUMP_NOSPD_TIME) {
				ply_work->no_spddown_timer = GMD_PL_SPINDASH_JUMP_NOSPD_TIME;
			}
		}

		// 水中泡表示

		// 攻撃設定
		GmPlayerSetAtk(ply_work);

		// SE
		GmSoundPlaySE("Jump");

		// エフェクト ジャンプ煙
		GmPlyEfctCreateJumpDust(ply_work);
#endif

}

// ==========================================================================
// gmPlySeqHomingRefMain
/*!
 *	ホーミング跳ね返り
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqHomingRefMain(GMS_PLAYER_WORK *ply_work)
{
	// 落下開始チェック
	if (ply_work->obj_work.spd.y >= 0) {
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL);
		return;
	}

	// 着地チェック
	if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {
		// 着地設定
		GmPlySeqLandingSet(ply_work, 0);
		// FWへ移行
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
		return;
	}
}

// ==========================================================================
// ジャンプダッシュ(ホーミングできなかった時)
// ==========================================================================
// ==========================================================================
// GmPlySeqInitJumpDash
/*!
 *	ジャンプダッシュ 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitJumpDash(GMS_PLAYER_WORK *ply_work)
{
	Angle32	dir;

	// アクション変更
	if (!(ply_work->player_flag & GMD_PLF_PINBALL_SONIC)) {	// ピンボールの時は切り替えない
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_SPIN);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}

	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM;
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_UNDER);
	ply_work->player_flag |= GMD_PLF_NOHOMING | GMD_PLF_NOJUMPMOVE;		// ホーミング不可 ジャンプMOVE不可
	ply_work->obj_work.dir.z = 0;

	// 自由接地OFF
	ply_work->gmk_flag &= ~(GMD_PLGF_TOUCH | GMD_PLGF_TOUCH_FORCE_DIR | GMD_PLGF_TOUCH_FLIP | GMD_PLGF_TOUCH_DISP_FLIP);

	//		jump_state |= GMD_PLY_SEQ_SETJUMPSTATE_NOJUMPMOVE;
	//	GmPlySeqSetJumpState(ply_work, 0, jump_state);
	// 移動角度
	if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
		dir = 0x8000 - GMD_PL_JUMP_DASH_DIR;
	}
	else {
		dir = GMD_PL_JUMP_DASH_DIR;
	}

	if (ply_work->player_flag & GMD_PLF_AUTO_RUN) {
	// オートラン時
		// 速度設定
		ply_work->obj_work.spd.y = 0;
		ply_work->obj_work.spd.x += (fx32)(GMD_PL_JUMP_DASH_AUTO_RUN_SPD * nnCos(dir));
		ply_work->obj_work.spd.y += (fx32)-(GMD_PL_JUMP_DASH_AUTO_RUN_SPD * nnSin(dir));

		// 減速なし時間
		ply_work->no_spddown_timer = GMD_PL_JUMP_DASH_AUTO_RUN_NOSPD_TIME;

		// ジャンプダッシュ時間
		ply_work->obj_work.user_timer = GMD_PL_JUMP_DASH_AUTO_RUN_TIME;
	}
	else {
		// 速度設定
		ply_work->obj_work.spd.y = 0;
		ply_work->obj_work.spd.x += (fx32)(GMD_PL_JUMP_DASH_SPD * nnCos(dir));
		ply_work->obj_work.spd.y += (fx32)-(GMD_PL_JUMP_DASH_SPD * nnSin(dir));

		// 減速なし時間
		ply_work->no_spddown_timer = GMD_PL_JUMP_DASH_NOSPD_TIME;

		// ジャンプダッシュ時間
		ply_work->obj_work.user_timer = GMD_PL_JUMP_DASH_TIME;
	}


	// 攻撃設定
	GmPlayerSetAtk(ply_work);

	// エフェクト
	GmPlyEfctCreateJumpDash(ply_work);

    // SE

	// メイン処理設定
	ply_work->seq_func = gmPlySeqJumpDashMain;
}

// ==========================================================================
// gmPlySeqJumpDashMain
/*!
 *	ジャンプダッシュ
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqJumpDashMain(GMS_PLAYER_WORK *ply_work)
{
	/* 着地チェック */
	if ( ply_work->obj_work.move_flag & OBD_MOVE_UNDER ) {
		ply_work->player_flag &= ~GMD_PLF_NOJUMPMOVE;
		// 着地設定
		GmPlySeqLandingSet(ply_work, 0);
		// FWへ
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
		return;
	}

	if (!ply_work->obj_work.user_timer) {
		// ジャンプダッシュ終了
		fx32	spd_x, spd_y;
		// 速度退避
		spd_x = ply_work->obj_work.spd.x;
		spd_y = ply_work->obj_work.spd.y;
		// 落下へ移行
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL);
		// 速度復旧
		ply_work->obj_work.spd.x = spd_x;
		ply_work->obj_work.spd.y = spd_y;
		ply_work->player_flag &= ~GMD_PLF_NOJUMPMOVE;
		return;
	}
	ply_work->obj_work.user_timer--;
}

// ==========================================================================
// ダメージ
// ==========================================================================
// ==========================================================================
// GmPlySeqChangeDamage
/*!
 *	ダメージへ変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqChangeDamage(GMS_PLAYER_WORK *ply_work)
{
	GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_DAMAGE);
}

// ==========================================================================
// GmPlySeqChangeDamageSetSpd
/*!
 *	ダメージへ変更 速度変更つき
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_x		[in]	設定速度X
 *	@param	spd_y		[in]	設定速度Y
 *
 */
// ==========================================================================
void GmPlySeqChangeDamageSetSpd(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y)
{
	GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_DAMAGE);

	// 速度設定
	ply_work->obj_work.spd.x = spd_x;
	ply_work->obj_work.spd.y = spd_y;

	// 方向設定
	if (spd_x < 0) {
		ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
	}
	else {
		ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
	}
}

// ==========================================================================
// GmPlySeqInitDamage
/*!
 *	ダメージ 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitDamage(GMS_PLAYER_WORK *ply_work)
{
	// ステート初期化
	GmPlayerStateInit(ply_work);

    // 速度基本設定
	if (ply_work->player_flag & GMD_PLF_AUTO_RUN) {
	// オートラン時
		ply_work->obj_work.spd.x = -GMD_PL_DAMAGE_AUTO_RUN_JUMP_X_LEFT;
		ply_work->obj_work.spd.y = GMD_PL_DAMAGE_JUMP_Y;
		ply_work->obj_work.spd_m = 0;

		// 吹き飛び方向設定(必ず左に飛ばされるように変更090908 Yurita)
		ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
//		if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
//			ply_work->obj_work.spd.x = GMD_PL_DAMAGE_AUTO_RUN_JUMP_X_RIGHT;
//		}
	}
	else {
		ply_work->obj_work.spd.x = -GMD_PL_DAMAGE_JUMP_X;
		ply_work->obj_work.spd.y = GMD_PL_DAMAGE_JUMP_Y;
		ply_work->obj_work.spd_m = 0;

		// 吹き飛び方向設定
		if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
			ply_work->obj_work.spd.x = -ply_work->obj_work.spd.x;
		}
	}

	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_DAMAGE);

	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_UNDER;

    // 無敵設定
	ply_work->invincible_timer = ply_work->time_damage;
	GmPlayerSetDefInvincible(ply_work);

	// メイン処理設定
    ply_work->seq_func = gmPlySeqDamageMain;
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

	// コントローラー振動
	GMM_PAD_VIB_LARGE_TIME(60.f);
}

// ==========================================================================
// gmPlySeqDamageMain
/*!
 *	ダメージ
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqDamageMain(GMS_PLAYER_WORK *ply_work)
{
//	if (ply_work->obj_work.flag & OBD_OBJECT_NOHIT) {
//		return;
//	}
	
	// 着地
	if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {
		// 連続HIT回避フラ寝かす
		ply_work->rect_work[GMD_PLAYER_RECT_DEF].flag &= ~(OBD_RECT_DAMAGE);

		// 着地設定
		GmPlySeqLandingSet(ply_work, 0);

		// FWへ
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
	}
}


// ==========================================================================
// 死亡
// ==========================================================================
// ==========================================================================
// GmPlySeqChangeDeath
/*!
 *	死亡へ変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqChangeDeath(GMS_PLAYER_WORK *ply_work)
{
	{{//qqq
		OS_TPrintf("try to clear saved state after PLAYER_DEATH...\n");			
		mppCheckPointStorage::removeState();
	}}	
	GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_DEATH);
}

// ==========================================================================
// GmPlySeqInitDeath
/*!
 *	死亡 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitDeath(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->player_flag & GMD_PLF_DIE) {
		return;
	}
	if (ply_work->player_flag & GMD_PLF_GOAL) {
		return;
	}

	// スーパーソニックの時は元に戻す
	if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
		GmPlayerSetEndSuperSonic(ply_work);
	}

	// ステート初期化
	GmPlayerStateInit(ply_work);

	// 各種設定クリア
//	ply_work->nitro_timer = 0;
//	ply_work->genocide_timer = 0;
//	ply_work->hs_trick_timer = 0;

	// フラグ設定
//	ply_work->player_flag &= ~GMD_PLF_MAX_TENSION_USE; // 減らしフラグを消す
	ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
	ply_work->obj_work.move_flag |= OBD_MOVE_NOCOL | OBD_MOVE_NOCOLOBJ;	// あたりOFF

	// 速度設定
	ply_work->obj_work.spd.x = 0;
	ply_work->obj_work.spd_m = 0;
	ply_work->obj_work.spd.y = -ply_work->spd_jump;
	ply_work->obj_work.spd_add.x = ply_work->obj_work.spd_add.y = 0;
	ply_work->obj_work.dir.z = 0;

	ply_work->obj_work.pos.z = GMD_OBJ_DEFAULT_POS_Z_N_BACK + 16*FX32_ONE;	// 超近景の後ろに。

	// トロッコ時速度設定
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
#if 1
		// ジャンプ擬似重力方向を通常重力方向に
		ply_work->jump_pseudofall_dir = g_gm_main_system.pseudofall_dir;

		// 死亡中擬似ジャンプ重力固定
		ply_work->gmk_flag |= GMD_PLGF_GMK_JUMP_PSEUDOFALL_DIR_FIX;

		// 不要角度クリア
		ply_work->obj_work.dir.x =
			ply_work->obj_work.dir.y = 0;
			ply_work->obj_work.dir.z = 0;

		// 角度移動OFF
		ply_work->obj_work.move_flag &= ~OBD_MOVE_DIR;

#else
	//	u16	dir;
		// 死亡中擬似ジャンプ重力固定
		ply_work->gmk_flag |= GMD_PLGF_GMK_JUMP_PSEUDOFALL_DIR_FIX;
		ply_work->jump_pseudofall_dir = g_gm_main_system.pseudofall_dir;

		ObjObjectSpdDirFall(&ply_work->obj_work.spd.x, &ply_work->obj_work.spd.y,
												g_gm_main_system.pseudofall_dir);

		// 角度を落下方向に変更
		ply_work->obj_work.dir.z = (u16)(g_gm_main_system.pseudofall_dir + 0x8000);

	//	dir = (u16)(ply_work->obj_work.dir_fall - ply_work->jump_pseudofall_dir);
	//	ply_work->obj_work.spd.x = FX_Mul(ply_work->spd_jump, mtMathSin(dir));
	//	ply_work->obj_work.spd.y = FX_Mul(-ply_work->spd_jump, mtMathCos(dir));
#endif
	}


	// バリア解除
//	if (ply_work->player_flag & (GMD_PLF_BARRIER | GMD_PLF_MAGNET)) {
//		// バリア解除サウンド
//		GmSoundPlaySE("Ex_Barrier");
//	}
	ply_work->player_flag &= ~(GMD_PLF_BARRIER | GMD_PLF_MAGNET);

	// 死亡フラグ設定
	ply_work->player_flag |= GMD_PLF_DIE;

	// 矩形 OFF
	ply_work->obj_work.flag |= OBD_OBJECT_NOHIT;


	// 画面振動
	//EfQuake( EFD_QUAKE_S_BIG );

	// SE
	if (ply_work->player_flag & GMD_PLF_WATER) {
		// 水中死亡
		GmSoundPlaySE("Damage3");
	}
	else {
		// 通常
		GmSoundPlaySE("Damage1");
	}

	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_DIE1);

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqDeathMain;
	ply_work->obj_work.user_timer = 0;
	ply_work->water_timer = 0;

	// コントローラー振動
	GMM_PAD_VIB_LARGE_TIME(90.f);
}

// ==========================================================================
// gmPlySeqDeathMain
/*!
 *	死亡
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqDeathMain(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->act_state == GME_PLY_ACT_STATE_DIE1) {
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// アクション変更
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_DIE2);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
	}

	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// 回転する
		ply_work->obj_work.dir.z += 0x400;
	}


	// 死亡後のシステム移行処理はMain側で
}

// ==========================================================================
// スーパーソニック変身
// ==========================================================================
// ==========================================================================
// GmPlySeqChangeTransformSuper
/*!
 *	スーパーソニック変身へ変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqChangeTransformSuper(GMS_PLAYER_WORK *ply_work)
{
	GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_TRANS_SUPER);
}

// ==========================================================================
// GmPlySeqInitTransformSuper
/*!
 *	スーパーソニック変身 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitTransformSuper(GMS_PLAYER_WORK *ply_work)
{
	fx32	float_x = 0, float_y = 0;
	u16		dir_temp;

	if (ply_work->player_flag & GMD_PLF_DIE) {
		return;
	}
	if (ply_work->player_flag & GMD_PLF_GOAL) {
		return;
	}

	// 角度保存
	dir_temp = ply_work->obj_work.dir.z;

	// ジャンプ状態でない時は少しだけ浮かす
	if (!(ply_work->obj_work.move_flag & OBD_MOVE_JUMP)) {
		// 先に計算しておく
		//float_x = FXM_FLOAT_TO_FX32(nnCos(0x10000 - ply_work->obj_work.dir.z + 0x4000) * 2.f);
		//float_y = -FXM_FLOAT_TO_FX32(nnSin(0x10000 - ply_work->obj_work.dir.z + 0x4000) * 2.f);
		float_x = FXM_FLOAT_TO_FX32(nnCos(0x14000 - ply_work->obj_work.dir.z) * 3.f);
		float_y = -FXM_FLOAT_TO_FX32(nnSin(0x14000 - ply_work->obj_work.dir.z) * 3.f);
	}

	// ステート初期化
	GmPlayerStateInit(ply_work);

#if 0
	// スーパーソニックに変身
	GmPlayerSetSuperSonic(ply_work);

	// リング減少タイマーを演出中だけMAXにしておく
	ply_work->super_sonic_ring_timer = 0x7FFFFFFF;
#endif

	// フラグ設定
	ply_work->obj_work.move_flag &= ~OBD_MOVE_FALL;	// HIT OFF
	ply_work->obj_work.flag |= OBD_OBJECT_NOHIT;	// あたらない
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		ply_work->obj_work.move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動なし地形チェックなし
	}
	//ply_work->obj_work.move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動なし地形チェックなし

	// ジャンプ状態でない時は少しだけ浮かす
	if (!(ply_work->obj_work.move_flag & OBD_MOVE_JUMP)) {
		ply_work->obj_work.move_flag |= OBD_MOVE_JUMP;
		ply_work->obj_work.move_flag &= ~OBD_MOVE_COL_MASK;

		ply_work->obj_work.pos.x += float_x;
		ply_work->obj_work.pos.y += float_y;
	}

	// 速度設定
	ply_work->obj_work.spd.x = ply_work->obj_work.spd.y = 0;
	ply_work->obj_work.spd_add.x = ply_work->obj_work.spd_add.y = 0;
	ply_work->obj_work.spd_m = 0;
	ply_work->obj_work.dir.z = 0;
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// 角度復帰
		ply_work->obj_work.dir.z = dir_temp;
	}

	// トロッコ時 Z座標補正
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		ply_work->obj_work.pos.z = GMD_PL_TRUCK_TRANS_SUPER_POS_Z;
		ply_work->gmk_flag |= GMD_PLGF_GMK_HOLD_POS_Z;
	}

	// アクション変更
//	if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
//		// 強制反転
//		GmPlySeqSetProgramTurn(ply_work, GMD_PL_PGM_TURN_SPD_DEF);
//	}
	//GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_CHANGE_01);
	//ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_CHANGE_01);		// 30F

	// メイン処理設定
    ply_work->seq_func = gmPlySeqTransformSuperMain;
	ply_work->obj_work.user_timer = (25 + 60 + 60)*FX32_ONE;
	ply_work->obj_work.user_work = 0;		// スピンカウンタ

	// スーパーソニック開始エフェクト
	GmPlyEfctCreateSuperStart(ply_work);
}

// ==========================================================================
// gmPlySeqTransformSuperMain
/*!
 *	スーパーソニック変身
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_timer	: 演出タイマー
 *		user_work	: スピン回数カウンタ
 */
// ==========================================================================
void gmPlySeqTransformSuperMain(GMS_PLAYER_WORK *ply_work)
{
	ply_work->obj_work.user_timer = ObjTimeCountDown(ply_work->obj_work.user_timer);

	if (ply_work->act_state == GME_PLY_ACT_STATE_CHANGE_01) {
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// 変身前溜めへ
			// アクション変更
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_CHANGE_01_1);	// 45F分
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
	}
	else if (ply_work->act_state != GME_PLY_ACT_STATE_CHANGE_01_2) {
		if ((ply_work->obj_work.user_timer & 0xFFFFF000) == 70*FX32_ONE) {
			// 変身へ
			// アクション変更
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_CHANGE_01_2);	// 10F
		}
	}

	if ((ply_work->obj_work.user_timer & 0xFFFFF000) == 60*FX32_ONE &&
			!(ply_work->player_flag & GMD_PLF_SUPER_SONIC)) {
		u16							dir_temp;
		GMS_PLAYER_RESET_ACT_WORK	reset_act_work;

		dir_temp = ply_work->obj_work.dir.z;

		// アクション変更
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_CHANGE_02);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

		// アクション再設定情報取得
		GmPlayerSaveResetAction(ply_work, &reset_act_work);

		// スーパーソニック状態に変更
		GmPlayerSetSuperSonic(ply_work);

		// 現在のアクションを設定しなおす
		GmPlayerResetAction(ply_work, &reset_act_work);

		// フラグ再設定
		ply_work->obj_work.move_flag &= ~OBD_MOVE_FALL;	// HIT OFF
		ply_work->obj_work.flag |= OBD_OBJECT_NOHIT;	// あたらない
		if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
			ply_work->obj_work.move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動なし地形チェックなし
		}
		//ply_work->obj_work.move_flag |= OBD_MOVE_NOMOVE; | OBD_MOVE_NOCOL;	// 移動なし地形チェックなし

		// トロッコ時角度復帰
		if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
			ply_work->obj_work.dir.z = dir_temp;
		}
	}
	
	if (!ply_work->obj_work.user_timer) {
		// 通常シーケンスへ戻る
		ply_work->obj_work.move_flag |= OBD_MOVE_FALL;	// 落下ON
		ply_work->obj_work.flag &= ~OBD_OBJECT_NOHIT;	// HIT ON
		ply_work->obj_work.move_flag &= ~(OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);	// 移動 地形チェック復帰

		// リング減少タイマー初期設定
		ply_work->super_sonic_ring_timer = GMD_PL_SUPER_SONIC_RING_DEC_INT*FX32_ONE;

		// トロッコ時 Z座標補正クリア
		if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
			ply_work->obj_work.pos.z = 0;
			ply_work->gmk_flag &= ~GMD_PLGF_GMK_HOLD_POS_Z;
		}
		// シーケンス変更
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
	}
}


// ==========================================================================
// 通常ACTゴール
// ==========================================================================
// ==========================================================================
// GmPlySeqChangeNormalActGoal
/*!
 *	通常ACTゴールへ変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqChangeActGoal(GMS_PLAYER_WORK *ply_work)
{
	u32		move_flag;
	
	{{//qqq
		OS_TPrintf("try to clear saved state -- in GmPlySeqChangeActGoal...\n");			
		mppCheckPointStorage::removeState();
	}}
	

	if (ply_work->player_flag & GMD_PLF_DIE ||
			g_gm_main_system.game_flag & GMD_GAME_FLAG_SPECIAL_STAGE) {
		// スペステリング突入時
		// 死亡時は無効
		return;
	}

	// move_flag保存
	move_flag = ply_work->obj_work.move_flag;

	// ステート初期化
	GmPlayerStateInit(ply_work);

	if (ply_work->seq_state == GME_PLY_SEQ_STATE_SPIN_DASHACC ||
			ply_work->seq_state == GME_PLY_SEQ_STATE_SPIN_DASH) {
		// FWシーケンスへ変更
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
	}

	// 地形接触系フラグ復帰
	ply_work->obj_work.move_flag |= move_flag & OBD_MOVE_UNDER;

	// フラグ設定
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_LIMIT_OUT | OBD_MOVE_NOCOL_W);	// 横壁あたり無し

	// プレイヤーキー入力無視
	// ゴール処理開始
	ply_work->player_flag |= GMD_PLF_NOKEY | GMD_PLF_ACT_GOAL | GMD_PLF_GOAL;

	// くらいOFF
//	ply_work->rect_work[GMD_PLAYER_RECT_DEF].def_power = GMD_OBJ_RECT_DEF_POWER_INVINCIBLE;
	GmPlayerSetDefInvincible(ply_work);					// 無敵セット
	ply_work->invincible_timer = 0;						// ダメージ後の無敵解消対策

	// FWシーケンスへ変更
//	GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
}

// ==========================================================================
// gmPlySeqActGoal
/*!
 *	通常ACTゴール処理
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqActGoal(GMS_PLAYER_WORK *ply_work)
{
	OBS_CAMERA	*camera;

	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPECIAL_STAGE) {
		// スペステが有効になった時点で設定を行わない
		return;
	}

	// プレイヤーキー入力無視フラグ設定
	ply_work->player_flag |= GMD_PLF_NOKEY;
	// くらいOFF
	GmPlayerSetDefInvincible(ply_work);					// 無敵セット
	ply_work->invincible_timer = 0;						// ダメージ後の無敵解消対策

	// 水中タイマクリア
	ply_work->water_timer = 0;

	camera = ObjCameraGet(g_obj.glb_camera_id);
	if ((FXM_FLOAT_TO_FX32(camera->disp_pos.x) + (OBD_LCD_X >> 1) + 128) >
					(ply_work->obj_work.pos.x >> FX32_SHIFT)) {
		// プレイヤーが画面内にいる
		// 右方向強制入力
		ply_work->key_on |= PAD_KEY_RIGHT;
		ply_work->key_walk_rot_z = 0x7FFF;
	}
}


// ==========================================================================
// ボスステージゴール
// ==========================================================================
// ==========================================================================
// GmPlySeqChangeBossGoal
/*!
 *	ボスステージゴールへ変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqChangeBossGoal(GMS_PLAYER_WORK *ply_work, fx32 capsule_pos_x, fx32 capsule_pos_y)
{
	
	{{//qqq
		OS_TPrintf("try to clear saved state -- in GmPlySeqChangeBossGoal...\n");			
		mppCheckPointStorage::removeState();
		mppAchievementSupport::get()->event_DefeatBoss();
	}}
	
	if (ply_work->player_flag & GMD_PLF_DIE) {
		// 死亡時は無効
		return;
	}

	// ステート初期化
	GmPlayerStateInit(ply_work);

	// プレイヤーキー入力無視
	// ゴール処理開始
	ply_work->player_flag |= GMD_PLF_NOKEY | GMD_PLF_BOSS_GOAL_PRE | GMD_PLF_GOAL;

	// くらいOFF
	ply_work->rect_work[GMD_PLAYER_RECT_DEF].def_power = GMD_OBJ_RECT_DEF_POWER_INVINCIBLE;

	// カプセル位置保存
	ply_work->gmk_work0 = capsule_pos_x;
	ply_work->gmk_work1 = capsule_pos_y;
	if (ply_work->obj_work.pos.x >= capsule_pos_x) {
		// 右へ
		ply_work->gmk_work2 = 0;
	}
	else {
		// 左へ
		ply_work->gmk_work2 = 1;
	}

	// FWシーケンスへ変更
	GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
}

// ==========================================================================
// gmPlySeqBossGoalPre
/*!
 *	ボスステージゴール前処理
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		gmk_work0	: カプセル位置X
 *		gmk_work1	: カプセル位置Y
 *		gmk_work2	: 移動方向 0 : 右   1 : 左
 */
// ==========================================================================
void gmPlySeqBossGoalPre(GMS_PLAYER_WORK *ply_work)
{
	// プレイヤーキー入力無視フラグ設定
	ply_work->player_flag |= GMD_PLF_NOKEY;
	// くらいOFF
	ply_work->rect_work[GMD_PLAYER_RECT_DEF].def_power = GMD_OBJ_RECT_DEF_POWER_INVINCIBLE;
	// 水中タイマクリア
	ply_work->water_timer = 0;

	if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER) ||
			((ply_work->obj_work.move_flag & OBD_MOVE_UNDER) &&
				(ply_work->obj_work.pos.y < ply_work->gmk_work1 - 24*FX32_ONE))) {
		// プレイヤーがまだカプセルある地面に接地していない
		if (!(!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER) &&
				MTM_MATH_ABS(ply_work->obj_work.pos.x - ply_work->gmk_work0) > 64*FX32_ONE)) {
			// 地上状態 もしくは空中状態で一定距離以内
			if (ply_work->gmk_work2) {
				// 左方向強制入力
				ply_work->key_on |= PAD_KEY_LEFT;
				ply_work->key_walk_rot_z = -0x7FFF;
			}
			else {
				// 右方向強制入力
				ply_work->key_on |= PAD_KEY_RIGHT;
				ply_work->key_walk_rot_z = 0x7FFF;
			}
		}
	}
	else {
		// 下の地面についた
		ply_work->player_flag &= ~GMD_PLF_BOSS_GOAL_PRE;

		// ボスゴールシーケンスへ変更
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_BOSS_GOAL);
	}
}

// ==========================================================================
// GmPlySeqInitBossGaol
/*!
 *	ボスステージゴール 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitBossGaol(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->player_flag & GMD_PLF_DIE) {
		return;
	}

	// ステート初期化
	GmPlayerStateInit(ply_work);

	// プレイヤーキー入力無視フラグ設定
	ply_work->player_flag |= GMD_PLF_NOKEY;
	// くらいOFF
	ply_work->rect_work[GMD_PLAYER_RECT_DEF].def_power = GMD_OBJ_RECT_DEF_POWER_INVINCIBLE;
	// 水中タイマクリア
	ply_work->water_timer = 0;

	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_FW);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

	// 演出時間設定
	ply_work->obj_work.user_timer = 60*FX32_ONE;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqBossGoalMain;
}

// ==========================================================================
// gmPlySeqBossGoalMain
/*!
 *	ボスステージゴール
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_timer	: 演出タイマー
 */
// ==========================================================================
void gmPlySeqBossGoalMain(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->act_state == GME_PLY_ACT_STATE_FW) {
		ply_work->obj_work.user_timer = ObjTimeCountDown(ply_work->obj_work.user_timer);

		if (!ply_work->obj_work.user_timer) {
			// アクション変更
			if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
				// 強制反転
				GmPlySeqSetProgramTurn(ply_work, GMD_PL_PGM_TURN_SPD_DEF);
			}
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GOAL_01);
		}
	}
	else if (ply_work->obj_work.disp_flag & OBD_DISP_END &&
			ply_work->act_state == GME_PLY_ACT_STATE_GOAL_01) {
		// アクション変更
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GOAL_02);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}
}


// ==========================================================================
// ボスFINAL演出
// ==========================================================================
// ==========================================================================
// GmPlySeqChangeBoss5Demo
/*!
 *	ボスFINAL 演出へ変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	dest_pos_x	[in]	移動目標位置X
 *	@param	is_goal		[in]	クリア演出フラグ
 *
 *	@note
 *	外部から直接呼び出してください。
 */
// ==========================================================================
void GmPlySeqChangeBoss5Demo(GMS_PLAYER_WORK *ply_work, fx32 dest_pos_x, BOOL is_goal)
{
		
	{{//qqq
		OS_TPrintf("try to clear saved state -- in GmPlySeqChangeBoss5Demo...\n");			
		mppCheckPointStorage::removeState();
	}}
	
	if (ply_work->player_flag & GMD_PLF_DIE) {
		// 死亡時は無効
		return;
	}
	
	// シームレスに繋げるためステート初期化しない
	// (GmPlayerStateInit)
	
	// 移動停止
	ply_work->obj_work.spd_m	= 0;
	ply_work->obj_work.spd.x	= 0;
	
	// 一旦着地した扱いにする
	GmPlySeqLandingSet(ply_work, 0);
	
	// プレイヤーキー入力無視フラグ設定
	// ボスFINAL演出処理開始
	ply_work->player_flag	|= GMD_PLF_NOKEY | GMD_PLF_BOSS5_DEMO;
	
	if (is_goal) {
		ply_work->player_flag	|= GMD_PLF_GOAL;
		// くらいOFF
		ply_work->rect_work[GMD_PLAYER_RECT_DEF].def_power = GMD_OBJ_RECT_DEF_POWER_INVINCIBLE;
	}
	
	// 目標位置保存
	ply_work->gmk_work0	= dest_pos_x;
	
	// FWシーケンスへ変更
	GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
}

// ==========================================================================
// gmPlySeqBoss5DemoPre
/*!
 *	ボスFINAL 演出走り
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *	GmPlySeqMain()で呼ばれます。
 */
// ==========================================================================
void gmPlySeqBoss5DemoPre(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->obj_work.pos.x >= ply_work->gmk_work0) {
		if (ply_work->obj_work.spd.x == 0) {
			// ボス5演出シーケンスへ変更
			ply_work->player_flag	&= ~GMD_PLF_BOSS5_DEMO;
			GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_BOSS5_DEMO);
		}
	}
	else {
		// 右方向強制入力
		ply_work->key_on |= PAD_KEY_RIGHT;
		ply_work->key_walk_rot_z = 0x7FFF;
	}
}

// ==========================================================================
// GmPlySeqInitBoss5Demo
/*!
 *	ボスFINAL 演出 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *	GmPlySeqChangeSequence()によって呼び出されます。
 */
// ==========================================================================
void GmPlySeqInitBoss5Demo(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->player_flag & GMD_PLF_DIE) {
		return;
	}
	
	// ステート初期化
	GmPlayerStateInit(ply_work);
	
	// プレイヤーキー入力無視フラグ設定
	ply_work->player_flag	|= GMD_PLF_NOKEY;
	
	// アクション変更
	if (ply_work->act_state != GME_PLY_ACT_STATE_FW) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_FW);
		ply_work->obj_work.disp_flag	|= OBD_DISP_REPEAT;
	}
	
	// メイン処理設定
	ply_work->seq_func	= gmPlySeqBoss5DemoMain;
}

// ==========================================================================
// gmPlySeqBoss5DemoMain
/*!
 *	ボスFINAL 演出走り
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_timer	: 
 */
// ==========================================================================
void gmPlySeqBoss5DemoMain(GMS_PLAYER_WORK *ply_work)
{
	UNREFERENCED_PARAMETER(ply_work);
	;
}

// ==========================================================================
// GmPlySeqChangeBoss5DemoEnd
/*!
 *	ボスFINAL 演出終了へ変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *	演出シーケンスを終了させます。外部から直接呼び出してください。
 */
// ==========================================================================
void GmPlySeqChangeBoss5DemoEnd(GMS_PLAYER_WORK *ply_work)
{
	MTM_ASSERT(ply_work->seq_state == GME_PLY_SEQ_STATE_BOSS5_DEMO);
	
	if (ply_work->player_flag & GMD_PLF_DIE) {
		// 死亡時は無効
		return;
	}
	
	// シームレスに繋げるためステート初期化しない
	// (GmPlayerStateInit)
	
	// プレイヤーキー入力無視解除フラグ設定
	ply_work->player_flag	&= ~(GMD_PLF_NOKEY | GMD_PLF_BOSS5_DEMO);
	
	// FWシーケンスへ変更
	GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
}

// ==========================================================================
// タイムアタックリトライ FW
// ==========================================================================
// ==========================================================================
// GmPlySeqChangeTRetryFw
/*!
 *	タイムアタックリトライFWへ変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqChangeTRetryFw(GMS_PLAYER_WORK *ply_work)
{
	GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_T_RETRY_FW);
}

// ==========================================================================
// GmPlySeqInitTRetryFw
/*!
 *	タイムアタックリトライFWへ変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitTRetryFw(GMS_PLAYER_WORK *ply_work)
{
//	OBS_CAMERA	*camera;
	
	MTM_ASSERT(!(g_gm_main_system.game_flag & GMD_GAME_FLAG_SPECIAL_STAGE));
	
	// カメラ情報を取得
//	camera = ObjCameraGet(g_obj.glb_camera_id);
	
	// ピンボール状態終了
	if (ply_work->player_flag & GMD_PLF_PINBALL_SONIC) {
		GmPlayerSetEndPinballSonic(ply_work);
	}
	
	// トロッコ状態終了
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		GmPlayerSetEndTruckRide(ply_work);
	}
	
    // プレイヤーパラメータ初期化
	GmPlayerSpdParameterSet(ply_work);

    // オブジェクト用数値初期化
    ply_work->obj_work.dir.x = 0;
    ply_work->obj_work.dir.y = 0;
    ply_work->obj_work.dir.z = 0;
    ply_work->obj_work.spd_m = 0;
    ply_work->obj_work.spd.x = 0;
    ply_work->obj_work.spd.y = 0;
    ply_work->obj_work.spd.z = 0;
	
	// disp_flag 初期化
	ply_work->obj_work.disp_flag &= ~( OBD_DISP_HFLIP
									  |OBD_DISP_VFLIP );
	// FWに切り替え
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_WAIT_1_1);//GME_PLY_ACT_STATE_FW);
	
	// フラグ設定
	ply_work->player_flag &= ~GMD_PLF_ACT_GOAL;
	
	// ソニック移動速度クリア
	ply_work->obj_work.spd_m = 0;
	
	// 水中タイマーをクリア
	ply_work->water_timer = 0;
	
	// くらいOFF(無敵設定)
	ply_work->rect_work[GMD_PLAYER_RECT_DEF].def_power = GMD_OBJ_RECT_DEF_POWER_INVINCIBLE;
	ply_work->invincible_timer = 0;						// ダメージ後の無敵解消対策
	
	// move_flag 初期化
	ply_work->obj_work.move_flag = (  OBD_MOVE_DIR				// 坂移動あり
									| OBD_MOVE_NOCOL_MASK		// コリジョン判定無し
									| OBD_MOVE_NOMOVE );		// 移動無し
	
	ply_work->obj_work.move_flag &= ~OBD_MOVE_FALL;
	
	ply_work->obj_work.flag |= OBD_OBJECT_NOHIT;
	
	// 擬似重力初期化
	ply_work->obj_work.dir_fall = 0;
	ply_work->ply_pseudofall_dir = 0;
	ply_work->jump_pseudofall_dir = 0;
	g_gm_main_system.pseudofall_dir = 0;
	
	// プレイヤーキー入力無視フラグ設定
	ply_work->player_flag |= GMD_PLF_NOKEY;
	
    // フラグ設定
    ply_work->player_flag &= ~(GMD_PLF_ACT_GOAL | GMD_PLF_BOSS5_DEMO);
	
	ply_work->obj_work.dir_slope = GMD_PL_KEI_DIR;
	
	// メイン処理設定
    ply_work->seq_func = gmPlySeqTRetryFw;
}

// ==========================================================================
// gmPlySeqTRetryFw
/*!
 *	タイムアタックリトライFW処理
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqTRetryFw(GMS_PLAYER_WORK *ply_work)
{
	// FW中は処理なし(常にFWのまま)
	if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_WAIT_1_2);//GME_PLY_ACT_STATE_FW);
	}
	
	// 水中タイマーをクリア
	ply_work->water_timer = 0;
	
	// くらいOFF(無敵設定)
	ply_work->rect_work[GMD_PLAYER_RECT_DEF].def_power = GMD_OBJ_RECT_DEF_POWER_INVINCIBLE;
	
}

// ==========================================================================
// タイムアタックリトライ その場加速
// ==========================================================================
// ==========================================================================
// GmPlySeqChangeTRetryAcc
/*!
 *	タイムアタックリトライその場加速へ変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqChangeTRetryAcc(GMS_PLAYER_WORK *ply_work)
{
	GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_T_RETRY_ACC);
}

// ==========================================================================
// GmPlySeqInitTRetryAcc
/*!
 *	タイムアタックリトライその場加速へ変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitTRetryAcc(GMS_PLAYER_WORK *ply_work)
{
	// プレイヤーの煙エフェクトOFFフラグON
	ply_work->player_flag |= GMD_PLF_WALK_SMK_EFCT_OFF;
	
	GmPlySeqMoveWalk(ply_work);
	// ここで速度に対応した走り(歩き)アクション設定
	GmPlayerWalkActionSet(ply_work);
	
	// 使用前にユーザータイマーを初期化
	ply_work->obj_work.user_timer = 0;
	
	// メイン処理設定
    ply_work->seq_func = gmPlySeqTRetryAcc;
}

// ==========================================================================
// gmPlySeqTRetryAcc
/*!
 *	タイムアタックリトライその場加速処理
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqTRetryAcc(GMS_PLAYER_WORK *ply_work)
{
	// タイマー更新
	ply_work->obj_work.user_timer++;
	
	// 移動速度加算(その場で回転足の速度以上)
	ply_work->obj_work.spd_m += 0x200;
	
	// 移動開始の時間になったとき
	if (ply_work->obj_work.user_timer > 100) {		// 時間値は仮設定
		// シーケンス処理
		ply_work->seq_func = NULL;
		
		// タイマー初期化
		ply_work->obj_work.user_timer = 0;
		
		// 移動開始させるために移動禁止フラグをOFF
		ply_work->obj_work.move_flag &= ~OBD_MOVE_NOMOVE;
		
		// 画面外へ移動できるようにフラグ設定
		ply_work->obj_work.move_flag &= ~OBD_MOVE_NOCOL_W;
		
		// NOCLIPにしないとソニックがCLIPでプレイヤーワークが解放されるため
		ply_work->obj_work.flag |= OBD_OBJECT_NOCLIP;
	}
	
	// GOALフラグOFFかつ、速度が回転足状態になれる速度の場合
	if (ply_work->obj_work.spd_m > ply_work->spd4 - 0x200
		&& !(ply_work->player_flag & GMD_PLF_ACT_GOAL)) {
		// 回転足状態にするため、一時的に角度を設定
		ply_work->obj_work.dir.z = GMD_PL_MAXDASH_DIR + 1;
		
		GmPlayerWalkActionSet(ply_work);
		
		GmPlayerWalkActionCheck(ply_work);
		
		// 回転足状態にしたあとは角度を元に戻す
		ply_work->obj_work.dir.z = 0;
		
		GmPlySeqChangeTRetryRun(ply_work);
	}
	
	// 水中タイマーをクリア
	ply_work->water_timer = 0;
	
	// くらいOFF(無敵設定)
	ply_work->rect_work[GMD_PLAYER_RECT_DEF].def_power = GMD_OBJ_RECT_DEF_POWER_INVINCIBLE;
	
}

// ==========================================================================
// リトライ走りだし
// ==========================================================================
// ==========================================================================
// GmPlySeqChangeTRetryRun
/*!
 *	タイムアタックリトライRUNへ変更
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqChangeTRetryRun(GMS_PLAYER_WORK *ply_work)
{
	// フラグ設定
	ply_work->player_flag |= GMD_PLF_ACT_GOAL;
}

// ==========================================================================
// ==========================================================================
// トロッコ専用
// ==========================================================================
// ==========================================================================
// ==========================================================================
// トロッコFW
// ==========================================================================
// ==========================================================================
// GmPlySeqInitTruckFw
/*!
 *	トロッコFW 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 */
// ==========================================================================
void GmPlySeqInitTruckFw(GMS_PLAYER_WORK *ply_work)
{
	// 通常シーケンス
	if (ply_work->obj_work.spd_m) {
		// 歩きへ移行
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_WALK);
		return;
	}
	// アクション変更
	if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDERPREV)) {
		// 着地した時
		// 左向き設定クリア
		ply_work->gmk_flag &= ~GMD_PLGF_GMK_TRUCK_L;
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_DOWN);
	}
	else if ((ply_work->obj_3d[*(g_gm_player_model_tbl[ply_work->char_id] + GME_PLY_ACT_STATE_FW)]->act_id[0]
				!= *(g_gm_player_motion_right_tbl[ply_work->char_id] + GME_PLY_ACT_STATE_FW)) &&
			(ply_work->obj_3d[*(g_gm_player_model_tbl[ply_work->char_id] + GME_PLY_ACT_STATE_GMK_TRUCK_DOWN)]->act_id[0]
				!= *(g_gm_player_motion_right_tbl[ply_work->char_id] + GME_PLY_ACT_STATE_GMK_TRUCK_DOWN))) {
		// アクション変更
		if (ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_L) {
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_FW_L);
		}
		else {
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_FW);
		}
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// ワーククリア
	ply_work->obj_work.user_timer	= 0;
	ply_work->obj_work.user_work	= 0;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqTruckFwMain;
}


// ==========================================================================
// gmPlySeqTruckFwMain
/*!
 *	トロッコFW
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqTruckFwMain(GMS_PLAYER_WORK *ply_work)
{
	//ply_work->obj_work.user_timer++;

	// アクション切り替えチェック
	if (ply_work->obj_3d[*(g_gm_player_model_tbl[ply_work->char_id] + GME_PLY_ACT_STATE_GMK_TRUCK_DOWN)]->act_id[0]
				== *(g_gm_player_motion_right_tbl[ply_work->char_id] + GME_PLY_ACT_STATE_GMK_TRUCK_DOWN)) {

		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// アクション変更
			if (ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_L) {
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_FW_L);
			}
			else {
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_FW);
			}
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
	}

	if (ply_work->act_state == GME_PLY_ACT_STATE_GMK_TRUCK_FW ||
				ply_work->act_state == GME_PLY_ACT_STATE_GMK_TRUCK_FW_L) {
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			ply_work->obj_work.user_work++;
			if (ply_work->obj_work.user_work >= 8) {
			//if (ply_work->obj_work.user_timer > 60*30) {
				// 待機0へ
				// アクション変更
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_WAIT_0_1);
			// 常に右向きのはず
			//	if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
			//		// 強制反転
			//		GmPlySeqSetProgramTurn(ply_work, GMD_PL_PGM_TURN_SPD_DEF);
			//	}
				//ply_work->obj_work.user_timer = 0;
				ply_work->obj_work.user_work = 0;
			}
		}
	}
	else if (ply_work->act_state == GME_PLY_ACT_STATE_WAIT_0_1 ||
			ply_work->act_state == GME_PLY_ACT_STATE_WAIT_1_1 ||
			ply_work->act_state == GME_PLY_ACT_STATE_WAIT_2_1) {
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// アクション変更
			GmPlayerActionChange(ply_work, (GME_PLY_ACT_STATE)(ply_work->act_state + 1));
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
			ply_work->obj_work.user_work = 0;
		}
	}
	else if (ply_work->act_state == GME_PLY_ACT_STATE_WAIT_0_2) {
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			ply_work->obj_work.user_work++;
			if (ply_work->obj_work.user_work >= 10) {
				// 待機1へ
				// アクション変更
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_WAIT_1_1);
				ply_work->obj_work.user_work = 0;
			}
		}
	}
#if 0
	else if (ply_work->act_state == GME_PLY_ACT_STATE_WAIT_1_2) {
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {

			ply_work->obj_work.user_work++;
			if (ply_work->obj_work.user_work >= 3) {
				// 待機2へ
				// アクション変更
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_WAIT_2_1);
				ply_work->obj_work.user_work = 0;
			}
		}
	}
#endif
}

// ==========================================================================
// トロッコ歩き
// ==========================================================================
// ==========================================================================
// GmPlySeqInitTruckWalk
/*!
 *	トロッコ歩き 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_timer	: エフェクト間隔用
 */
// ==========================================================================
void GmPlySeqInitTruckWalk(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDERPREV)) {
		// 着地した時
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_DOWN);
	}
	else if (!(ply_work->act_state == GME_PLY_ACT_STATE_GMK_TRUCK_RUN || 
				ply_work->act_state == GME_PLY_ACT_STATE_GMK_TRUCK_RUN_L)) {
		// アクション変更
		if (ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_L) {
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_RUN_L);
		}
		else {
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_RUN);
		}
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqTruckWalkMain;
	ply_work->obj_work.user_timer = 0;

	// 左振り向きタイマ
	ply_work->truck_left_flip_timer = GMD_PL_TRUCK_LEFT_FLIP_TIMER*FX32_ONE;
}

// ==========================================================================
// gmPlySeqTruckWalkMain
/*!
 *	歩き
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqTruckWalkMain(GMS_PLAYER_WORK *ply_work)
{
	float	frame;
	BOOL	b_flip = FALSE;

	// 向き変更チェック
	if (ply_work->obj_work.spd_m < 0 && ply_work->act_state == GME_PLY_ACT_STATE_GMK_TRUCK_RUN) {
		ply_work->truck_left_flip_timer = ObjTimeCountDown(ply_work->truck_left_flip_timer);

		if (!ply_work->truck_left_flip_timer) {
			// 方向フラグ設定
			ply_work->gmk_flag |= GMD_PLGF_GMK_TRUCK_L;
			b_flip = TRUE;
		}
	}
	else {
		// 左振り向きタイマクリア
		ply_work->truck_left_flip_timer = GMD_PL_TRUCK_LEFT_FLIP_TIMER*FX32_ONE;

		if (ply_work->obj_work.spd_m >= 0 && ply_work->act_state == GME_PLY_ACT_STATE_GMK_TRUCK_RUN_L) {
			// 方向フラグ設定
			ply_work->gmk_flag &= ~GMD_PLGF_GMK_TRUCK_L;
			b_flip = TRUE;
		}
	}
	if (b_flip) {
		// フレーム保存
		frame = ply_work->obj_work.obj_3d->frame[0];
		// アクション設定
		if (ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_L) {
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_RUN_L);
		}
		else {
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_RUN);
		}
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		// フレーム復帰
		ply_work->obj_work.obj_3d->frame[0] =
			ply_work->obj_work.obj_3d->frame[1] = frame;
	}

	// 着地アクション切り替えチェック
	if (ply_work->obj_3d[*(g_gm_player_model_tbl[ply_work->char_id] + GME_PLY_ACT_STATE_GMK_TRUCK_DOWN)]->act_id[0]
				== *(g_gm_player_motion_right_tbl[ply_work->char_id] + GME_PLY_ACT_STATE_GMK_TRUCK_DOWN)) {

		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// アクション変更
			if (ply_work->obj_work.spd_m >= 0) {
				ply_work->gmk_flag &= ~GMD_PLGF_GMK_TRUCK_L;
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_RUN);
			}
			else if (ply_work->obj_work.spd_m < 0) {
				ply_work->gmk_flag |= GMD_PLGF_GMK_TRUCK_L;
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_RUN_L);
			}
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
	}
}

// ==========================================================================
// トロッコ落下
// ==========================================================================
// ==========================================================================
// GmPlySeqInitTruckFall
/*!
 *	トロッコ落下 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitTruckFall(GMS_PLAYER_WORK *ply_work)
{
	u16		dir;

	// アクション変更
	if (ply_work->obj_3d[*(g_gm_player_model_tbl[ply_work->char_id] + GME_PLY_ACT_STATE_JUMP_FALL)]->act_id[0]
				!= *(g_gm_player_motion_right_tbl[ply_work->char_id] + GME_PLY_ACT_STATE_JUMP_FALL)) {
		// アクション変更
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_FALL);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}

	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM | OBD_MOVE_FALL | OBD_MOVE_NOSPD;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_UNDER;

	// トロッコ踏ん張り関連フラグクリア
	ply_work->gmk_flag &= ~(GMD_PLGF_GMK_TRUCK_DANGER | GMD_PLGF_GMK_TRUCK_DANGER_RET | GMD_PLGF_GMK_TRUCK_STICK);

	// メイン処理設定
    ply_work->seq_func = gmPlySeqTruckJumpMain;
    //ply_work->seq_func = gmPlySeqFallMain;

#if 1
	// 速度分解
	dir = (u16)(ply_work->obj_work.dir.z + ply_work->obj_work.dir_fall - ply_work->jump_pseudofall_dir);
	ply_work->obj_work.spd.x = FX_Mul(ply_work->obj_work.spd_m, mtMathCos(dir));
	ply_work->obj_work.spd.y = FX_Mul(ply_work->obj_work.spd_m, mtMathSin(dir));
#else
	// 速度設定
	ply_work->obj_work.spd.x = FX_Mul(ply_work->obj_work.spd_m, mtMathCos(ply_work->obj_work.dir.z));
	ply_work->obj_work.spd.y = FX_Mul(ply_work->obj_work.spd_m, mtMathSin(ply_work->obj_work.dir.z));
#endif

	// 攻撃矩形設定OFF◆
	//ObjRectAtkSet(&ply_work->rect_work[1], 0, 0);
	//ObjRectDefSet(&ply_work->rect_work[1], 0, 0);
	//ObjRectDefSet(&ply_work->rect_work[1], GMD_OBJ_RECT_DEF_FLAG_INVINCIBLE, GMD_OBJ_RECT_DEF_POWER_INVINCIBLE);
    //objRectAttrSet(&ply_work->rect_work[1], 0, 0);

	ply_work->player_flag &= ~GMD_PLF_USER_MASK;
	ply_work->player_flag |= GMD_PLF_USER1;		// ジャンプボタン無視

	ply_work->obj_work.user_timer	= 0;
	ply_work->obj_work.user_work	= 0;
	ply_work->timer					= 0;
}


// ==========================================================================
// トロッコジャンプ
// ==========================================================================
// ==========================================================================
// GmPlySeqInitTruckJump
/*!
 *	トロッコジャンプ 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitTruckJump(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	if (ply_work->obj_3d[*(g_gm_player_model_tbl[ply_work->char_id] + GME_PLY_ACT_STATE_JUMP_FALL)]->act_id[0]
				!= *(g_gm_player_motion_right_tbl[ply_work->char_id] + GME_PLY_ACT_STATE_JUMP_FALL)) {
		// アクション変更
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_FALL);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}

	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM | OBD_MOVE_NOSPD;
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_UNDER | OBD_MOVE_UNDERPREV);

	// メイン処理設定
    //ply_work->seq_func = gmPlySeqJumpMain;	GmPlySeqSetJumpStateで切り替え

	// 速度設定
	// 20090827
	{
		u16	dir;
		dir = ply_work->obj_work.dir.z;

		if (((dir + 0x100/*45度角度取得時の補正値*/) & 0x2000) &&
				(((dir + 0x100) & 0x0FFF) <= 0x400)) {
			// 45度角付近の時
			// 下りの時のみ
			if (ply_work->obj_work.spd_m > 0 &&
					dir < 0x8000) {
				dir -= 0x480;

			}
			else if (ply_work->obj_work.spd_m < 0 &&
					dir > 0x8000) {
				dir += 0x480;
			}
		}

		// 速度分解
		dir = (u16)(dir + ply_work->obj_work.dir_fall - ply_work->jump_pseudofall_dir);
		ply_work->obj_work.spd.x = FX_Mul(ply_work->obj_work.spd_m, mtMathCos(dir));
		ply_work->obj_work.spd.y = FX_Mul(ply_work->obj_work.spd_m, mtMathSin(dir));

		// ジャンプ力加算
		dir = (u16)(ply_work->obj_work.dir.z + ply_work->obj_work.dir_fall - ply_work->jump_pseudofall_dir);
		ply_work->obj_work.spd.x += FX_Mul(ply_work->spd_jump, mtMathSin(dir));
		ply_work->obj_work.spd.y += FX_Mul(-ply_work->spd_jump, mtMathCos(dir));
	}

	ply_work->player_flag &= ~GMD_PLF_USER_MASK;
//	ply_work->player_flag &= ~(GMD_PLF_TRICK_SP | GMD_PLF_TRICK);

	ply_work->obj_work.user_timer	= 0;
	ply_work->obj_work.user_work	= 0;
	ply_work->timer					= 0;

	// ジャンプステータス設定
	GmPlySeqSetJumpState(ply_work, 0, 0);
	// メイン処理差し替え
    ply_work->seq_func = gmPlySeqTruckJumpMain;

//	// 前のシーケンスがスピンの時は スピンダッシュジャンプ減速OFF時間を設定
//	if (ply_work->prev_seq_state == GME_PLY_SEQ_STATE_SPIN) {
//		if (ply_work->no_spddown_timer >= GMD_PL_SPINDASH_JUMP_NOSPD_TIME) {
//			ply_work->no_spddown_timer = GMD_PL_SPINDASH_JUMP_NOSPD_TIME;
//		}
//	}

    // 水中泡表示

	// 攻撃設定
	GmPlayerSetAtk(ply_work);

    // SE
	GmSoundPlaySE("Lorry3");

	// エフェクト ジャンプ煙
	//GmPlyEfctCreateJumpDust(ply_work);
	// エフェクト ブラー
	//GmPlyEfctCreateSpinJumpBlur(ply_work);
}

// ==========================================================================
// gmPlySeqTruckJumpMain
/*!
 *	トロッコジャンプ～落下
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_tiemr	FALL計算を行わない時間
 *	//user_work	先押しチェック用
 *	//timer		トリック可能チェックタイマ
    GMD_PLF_USER1をジャンプボタン無視フラグとして使用\n
    GMD_PLF_USER2をGimmickからのジャンプフラグとして使用\n
    GMD_PLF_USER3を小ジャンプ有効化フラグとして使用
 */
// ==========================================================================
void gmPlySeqTruckJumpMain(GMS_PLAYER_WORK *ply_work)
{
   fx32	check_spd = ply_work->obj_work.spd.y;

    // FALLフラグ復活タイマ
    if ( ply_work->obj_work.user_timer ){
        --ply_work->obj_work.user_timer;
        if ( !ply_work->obj_work.user_timer )
            ply_work->obj_work.move_flag |= OBD_MOVE_FALL;
    }

	if (!(ply_work->player_flag & (GMD_PLF_USER1 | GMD_PLF_USER3))) {
		if (!(GmPlayerKeyCheckJumpKeyOn(ply_work)) ){
			if (check_spd < GMD_PL_SJUMP_SPD) {
				// 小ジャンプ有効化
				ply_work->player_flag |= GMD_PLF_USER3;
			}
		}
	}

	if (ply_work->player_flag & GMD_PLF_USER3) {
		// 一定速度になるまで落下加速度を+
		if (ply_work->obj_work.spd.y < 0) {
			ply_work->obj_work.spd.y += ply_work->obj_work.spd_fall;
		}
	}
	
#if _IPHONE
	// 特定条件での傾き方向への加速
	// iPhoneフリック操作対策
	if (!(ply_work->obj_work.move_flag & (OBD_MOVE_OVER | OBD_MOVE_FRONT))) {
		BOOL flag = FALSE;

		if (MTM_MATH_ABS(ply_work->obj_work.spd.x) < FX32_ONE) {
			flag = TRUE;
		}
	
		if (flag) {
			u16 dir = (ply_work->obj_work.dir_fall - g_gm_main_system.pseudofall_dir);
			ply_work->obj_work.spd.x += mtMathSin(dir);
		}
	}
	// 上接触時の下方修正
	if (ply_work->obj_work.move_flag & OBD_MOVE_OVER) {
		ply_work->obj_work.spd.y += ply_work->obj_work.spd_fall * 5;
	}
#endif // _IPHONE

	/* 着地チェック */
	if ( ply_work->obj_work.move_flag & OBD_MOVE_UNDER ) {
		// 着地設定
		//ply_work->rect_work[0].flag &= ~(OBD_RECT_HIT | OBD_RECT_DAMAGE);	// HIT復帰
		GmPlySeqLandingSet(ply_work, 0);
		//ply_work->trick_combo = 0;			// トリックコンボ終了

		// SE
		GmSoundPlaySE("Lorry4");	// 着地SE

		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);

		// 振動
		GMM_PAD_VIB_MID();

		return;
	}
}

// ==========================================================================
// トロッコしゃがみ
// ==========================================================================
// ==========================================================================
// GmPlySeqInitTruckSquatStart
/*!
 *	トロッコしゃがみ開始 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitTruckSquatStart(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SQUATDOWN1);

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqTruckSquatMain;
    //ply_work->obj_work.user_timer = 0;
}

// ==========================================================================
// GmPlySeqInitTruckSquatMiddle
/*!
 *	トロッコしゃがみ中 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitTruckSquatMiddle(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SQUATDOWN2);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < 0x1000) {
		ply_work->obj_work.spd_m = 0;
	}

	// メイン処理設定
    ply_work->seq_func = gmPlySeqTruckSquatMain;
    //ply_work->obj_work.user_timer = 0;
}

// ==========================================================================
// GmPlySeqInitTruckSquatEnd
/*!
 *	トロッコしゃがみ終了 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitTruckSquatEnd(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SQUATDOWN3);

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqSquatEndMain;	// 通常しゃがみ終了で
    //ply_work->obj_work.user_timer = 0;
}

// ==========================================================================
// gmPlySeqTruckSquatMain
/*!
 *	しゃがみ 開始・中
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqTruckSquatMain(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->obj_work.spd_m) {
		// 歩きへ移行
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_WALK);
		return;
	}

	switch (ply_work->act_state) {
	case GME_PLY_ACT_STATE_SQUATDOWN1:
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// シーケンス変更変更
			GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SQUAT_M);
		}
		break;
	//case GME_PLY_ACT_STATE_SQUATDOWN2:
	//	break;
	//case GME_PLY_ACT_STATE_SQUATDOWN3:
	//	break;
	default:
		break;
	}
}

// ==========================================================================
// トロッコ用よろけ
// ==========================================================================
// ==========================================================================
// GmPlySeqInitTruckStaggerFront
/*!
 *	トロッコ用よろけ前 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitTruckStaggerFront(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_STAGGER_F);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqTruckStaggerMain;
    //ply_work->obj_work.user_timer = 0;

	// 回転量
	ply_work->obj_work.user_timer = 0;

	// エフェクト 汗
	GmPlyEfctCreateSweat(ply_work);
}

// ==========================================================================
// GmPlySeqInitTruckStaggerBack
/*!
 *	トロッコ用よろけ後ろ 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitTruckStaggerBack(GMS_PLAYER_WORK *ply_work)
{
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_STAGGER_B);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqTruckStaggerMain;
    //ply_work->obj_work.user_timer = 0;

	// 回転量
	ply_work->obj_work.user_timer = 0;

	// エフェクト 汗
	GmPlyEfctCreateSweat(ply_work);
}

// ==========================================================================
// gmPlySeqTruckStaggerMain
/*!
 *	よろけ
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqTruckStaggerMain(GMS_PLAYER_WORK *ply_work)
{
	if (!(ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_STICK)) {
		// 踏ん張り終了
		// FWへ
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
	}
	else {
		// おっとっと角度暫定
		if (ply_work->seq_state == GME_PLY_SEQ_STATE_STAGGER_F) {
			ply_work->obj_work.user_timer += 0x400;
			if (ply_work->obj_work.user_timer > 0x2000) {
				ply_work->obj_work.user_timer = 0x2000;
			}
		}
		else {
			ply_work->obj_work.user_timer -= 0x400;
			if (ply_work->obj_work.user_timer < -0x2000) {
				ply_work->obj_work.user_timer = -0x2000;
			}
		}
	}
}


#if 0
// ==========================================================================
// gmPlySeqFallMain
/*!
 *	落下
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqFallMain(GMS_PLAYER_WORK *ply_work)
{
	/* 着地チェック */
	if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {
		// 着地設定
		//ply_work->rect_work[0].flag &= ~(OBD_RECT_HIT | OBD_RECT_DAMAGE);	// HIT復帰
		GmPlySeqLandingSet(ply_work, 0);
		//ply_work->trick_combo = 0;			// トリックコンボ終了

		// FWへ移行
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
	}
}
#endif

#if 0
// ==========================================================================
// 移動時ターン
// ==========================================================================
// ==========================================================================
// GmPlySeqSetWalkTurn
/*!
 *	ターン
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		移動時の即時ターン シーケンスの変更は行わない
 */
// ==========================================================================
void GmPlySeqSetWalkTurn(GMS_PLAYER_WORK *ply_work)
{
	// ターン中はこちらで変更しない
	if (ply_work->act_state == GME_PLY_ACT_STATE_TURN) {
		return;
	}

//	if ( (ply_work->act_state >= GME_PLY_ACT_STATE_BRAKE && ply_work->act_state <= GME_PLY_ACT_STATE_BRAKED3) ){
//		// ブレーキSE停止
//        //NNS_SndPlayerStopSeq( &ply_work->h_snd_se, 0 );
//    }

	// 反転
	GmPlayerSetReverse(ply_work);
}
#endif


// ==========================================================================
// 各種設定
// ==========================================================================
// ==========================================================================
// GmPlySeqLandingSet
/*!
 *	着地設定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	dir_z		[in]	接地角度
 */
// ==========================================================================
void GmPlySeqLandingSet(GMS_PLAYER_WORK *ply_work, u16 dir_z)
{
    // 落下待ち時間再設定
    GmPlayerSpdParameterSet(ply_work);		// fall_timer設定のみならば必要は無い◆

//    ply_work->trick_gmk_eve_rec = NULL;
//    ply_work->player_flag &= ~(GMD_PLF_TRICK_SP | GMD_PLF_TRICK);
    ply_work->obj_work.move_flag &= ~(OBD_MOVE_NOSPDM | OBD_MOVE_JUMP);
    ply_work->obj_work.move_flag |= OBD_MOVE_FALL;
    ply_work->obj_work.disp_flag &= ~OBD_DISP_NODISP;
	ply_work->gmk_flag &= ~GMD_PLGF_GMK_JUMP_PSEUDOFALL_DIR_FIX;
	ply_work->gmk_flag2 &= ~(GMD_PLGF2_TRUCK_CAM_ROT_SLOW
#if GMD_PLY_USE_CAM_ROT_TRUCK_JUMP
							| GMD_PLGF2_TRUCK_JUMP_NO_CAM_ROT);
#else
							);
#endif
#if _IPHONE
	ply_work->gmk_flag2 &= ~GMD_PLGF2_TRUCK_JUMP_MOVE_ROT;
#endif // _IPHONE
    //ply_work->hover_timer = GMD_PL_BLAZE_HOVER_TIME;
    //ply_work->blaze_timer = 0;

	// ホーミング復帰 ジャンプ中移動不可解除
	ply_work->player_flag &= ~(GMD_PLF_NOHOMING | GMD_PLF_NOJUMPMOVE);
	ply_work->no_jump_move_timer = 0;

	// コンボカウンタクリア
	ply_work->score_combo_cnt = 0;

	if (!(ply_work->gmk_flag & GMD_PLGF_TOUCH)) {	// 吸着でない時
		if (ply_work->obj_work.col_flag & OBD_COLAT_CLIFF) {
			// 崖に着地した場合は角度を0に
			ply_work->obj_work.dir.z = 0;
		}
	}

#if 0	// ◆後で
    /* グラインド初期化 */
    if ( !((ply_work->act_state >= GMD_PLY_STATE_GRAIND && ply_work->act_state <= GMD_PLY_STATE_GRAIND_END) ||
    			 (ply_work->act_state >= GMD_PLY_STATE_GMK_DHB_GRAIND && ply_work->act_state <= GMD_PLY_STATE_GMK_DHB_GRAIND_END)) ) {
		// グラインド中でない時
        if ( ply_work->graind_id ) {
            if ( ply_work->graind_prev_ride & GMD_PLG_GRAIND_OUTCHECK) {
                // このFRAMEはグラインド矩形にHITしていないため設定面を元に戻す
                ply_work->obj_work.flag &= ~OBD_OBJECT_B;
                ply_work->obj_work.flag |= ply_work->graind_prev_ride & OBD_OBJECT_B;
                ply_work->graind_prev_ride = 0;
            }
        }
        ply_work->graind_id = 0;
    }
#endif

	/* 速度設定 */
	if ( dir_z ) {
#if 1
		if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
#if 1
			if (ply_work->gmk_flag2 & GMD_PLGF2_TRUCK_SLOPEFLY_DEC) {
				ply_work->obj_work.spd_m += FX_Mul(FX_Mul(ply_work->obj_work.move.x, GMD_PLY_SEQ_T_SLOPEFLY_RANDING_ACC_PER), mtMathCos((u16)(dir_z - g_gm_main_system.pseudofall_dir)));
				ply_work->obj_work.spd_m += FX_Mul(FX_Mul(ply_work->obj_work.move.y, GMD_PLY_SEQ_T_SLOPEFLY_RANDING_ACC_PER), mtMathSin((u16)(dir_z - g_gm_main_system.pseudofall_dir)));
			}
			else {
				ply_work->obj_work.spd_m += FX_Mul(ply_work->obj_work.move.x, mtMathCos((u16)(dir_z - g_gm_main_system.pseudofall_dir)));
				ply_work->obj_work.spd_m += FX_Mul(ply_work->obj_work.move.y, mtMathSin((u16)(dir_z - g_gm_main_system.pseudofall_dir)));
			}
#else
			ply_work->obj_work.spd_m += FX_Mul(ply_work->obj_work.move.x, mtMathCos((u16)(dir_z - g_gm_main_system.pseudofall_dir)));
			ply_work->obj_work.spd_m += FX_Mul(ply_work->obj_work.move.y, mtMathSin((u16)(dir_z - g_gm_main_system.pseudofall_dir)));
#endif
		}
		else {
			ply_work->obj_work.spd_m += FX_Mul(ply_work->obj_work.move.x, mtMathCos(dir_z));
			ply_work->obj_work.spd_m += FX_Mul(ply_work->obj_work.move.y, mtMathSin(dir_z));

			// 重力反転時は逆向き
			if ( ObjObjectDirFallReverseCheck( ply_work->obj_work.dir_fall) ){
				ply_work->obj_work.spd_m = -ply_work->obj_work.spd_m;
			}
		}
#else
		ply_work->obj_work.spd_m += FX_Mul(ply_work->obj_work.move.x, mtMathCos(dir_z));
		ply_work->obj_work.spd_m += FX_Mul(ply_work->obj_work.move.y, mtMathSin(dir_z));
	        
		// 重力反転時は逆向き
		if ( ObjObjectDirFallReverseCheck( ply_work->obj_work.dir_fall) ){
			ply_work->obj_work.spd_m = -ply_work->obj_work.spd_m;
		}
#endif
	}
	else {
#if 1
		if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {

#if 0
                // 下方向が進行方向
                if ( pWork->spd.y > 0){
                    // 坂道加減速対応
                    if ( pWork->move_flag & OBD_MOVE_SLOPE ){
                        //if ( pWork->ucDir >= 0)
                        // pWork->sSpdM -= pWork->sSpdY;
                        pWork->spd.x += ( (( pWork->spd.y ) * mtMathSin( (u16)(pWork->dir.z ) )) >> FX32_SHIFT );
                        //else
                        //    pWork->sSpdM += pWork->sSpdY;
                    }
                    pWork->spd.y = 0;
                }


			///
#endif

			if ( MTM_MATH_ABS(ply_work->obj_work.spd_m) < MTM_MATH_ABS(ply_work->obj_work.spd.x) ) {
				ply_work->obj_work.spd_m = ply_work->obj_work.spd.x;
			}
			//ply_work->obj_work.spd_m += FX_Mul(MTM_MATH_ABS(ply_work->obj_work.spd.x),
			//								mtMathSin(ply_work->obj_work.dir.z - g_gm_main_system.pseudofall_dir));
			ply_work->obj_work.spd_m += FX_Mul(MTM_MATH_ABS(ply_work->obj_work.spd.x),
											mtMathSin((u16)(ply_work->obj_work.dir.z + (ply_work->obj_work.dir_fall - g_gm_main_system.pseudofall_dir))));
		}
		else {
			if ( MTM_MATH_ABS(ply_work->obj_work.spd_m) < MTM_MATH_ABS(ply_work->obj_work.spd.x) ) {
				ply_work->obj_work.spd_m = ply_work->obj_work.spd.x;
			}
			ply_work->obj_work.spd_m += FX_Mul(MTM_MATH_ABS(ply_work->obj_work.spd.x), mtMathSin(ply_work->obj_work.dir.z));
		}
#else
		if ( MTM_MATH_ABS(ply_work->obj_work.spd_m) < MTM_MATH_ABS(ply_work->obj_work.spd.x) ) {
			ply_work->obj_work.spd_m = ply_work->obj_work.spd.x;
		}
		ply_work->obj_work.spd_m += FX_Mul(MTM_MATH_ABS(ply_work->obj_work.spd.x), mtMathSin(ply_work->obj_work.dir.z));
#endif
	}

	// トロッコ坂道飛び出しフラグOFF 速度分解が終わってからOFFにする
	ply_work->gmk_flag2 &= ~GMD_PLGF2_TRUCK_SLOPEFLY_DEC;

    // 瞬間速度記録
    ply_work->spd_work_max = MTM_MATH_ABS(ply_work->obj_work.spd_m);
    if ( ply_work->spd_work_max > ply_work->obj_work.spd_slope_max ) {
         ply_work->spd_work_max = ply_work->obj_work.spd_slope_max;
	}

    ply_work->obj_work.spd.x = 0;
    ply_work->obj_work.spd.y = 0;
    if ( dir_z != 0) {
        ply_work->obj_work.dir.z  = dir_z;
	}

    if ( !(ply_work->gmk_flag & GMD_PLGF_GMK_WALL) ) {
        ply_work->obj_work.dir.x = 0;
	}

    // ジャンプ開始時のY座標をクリア ◆前の時は特に使用していなかった
    //ply_work->camera_jump_pos_y = 0;

    // 歓声SE停止
    //NNS_SndPlayerStopSeq( &ply_work->h_snd_gallery, 32 );
    //NNS_SndHandleReleaseSeq( &ply_work->h_snd_gallery );

	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// 壁あたりスピードクリアフラグOFF
		ply_work->obj_work.move_flag &= ~OBD_MOVE_NOSPD;
	}
}



// ==========================================================================
// 移動処理
// ==========================================================================
// ==========================================================================
// GmPlySeqMoveFunc
/*!
 *	移動処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		ppMoveに設定
 */
// ==========================================================================
void GmPlySeqMoveFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK					*ply_work;
	const GMS_PLY_SEQ_STATE_DATA	*seq_state_data;

	ply_work		= (GMS_PLAYER_WORK*)obj_work;
	seq_state_data	= ply_work->seq_state_data_tbl;

	//fx32			spd_fall_max = obj_work->spd_fall_max;

	/* 移動処理特殊 */


	/* 通常 */

	// 歩き中移動チェック
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK) {
		if (ply_work->player_flag & GMD_PLF_AUTO_RUN) {
			GmPlySeqMoveWalkAutoRun(ply_work);
		}
		else if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
			GmPlySeqMoveWalkTruck(ply_work);
		}
		else {
			GmPlySeqMoveWalk(ply_work);
		}
	}
	// ジャンプ中移動チェック
	if (seq_state_data[ply_work->seq_state].check_attr &
				GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP &&
			!(ply_work->player_flag & GMD_PLF_NOJUMPMOVE)) {
		if (ply_work->player_flag & GMD_PLF_AUTO_RUN) {
			GmPlySeqMoveJumpAutoRun(ply_work);
		}
		else if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
			GmPlySeqMoveJumpTruck(ply_work);
		}
		else {
			GmPlySeqMoveJump(ply_work);
		}
	}
	if (ply_work->no_jump_move_timer) {
		ply_work->no_jump_move_timer = ObjTimeCountDown(ply_work->no_jump_move_timer);
		if (!ply_work->no_jump_move_timer) {
			ply_work->player_flag &= ~GMD_PLF_NOJUMPMOVE;
		}
	}

	// スピン中移動チェック
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN) {
		GmPlySeqMoveSpin(ply_work);
	}

	// スピン減速無し移動チェック
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC) {
		GmPlySeqMoveSpinNoDec(ply_work);
	}

	// ピンボール中移動チェック
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_PINBALL) {
		GmPlySeqMoveSpinPinball(ply_work);
	}

	// ジャンプ中角度変化(角度を徐々に0に戻す)
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR) {
		if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
			// トロッコ時
			//GmPlySeqJumpDirec(ply_work);
			GmPlySeqTruckJumpDirec(ply_work);
		} else if (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {
			// スペステ時
			gmPlySeqSplJumpDirec(ply_work);
		} else {
			// 通常
			GmPlySeqJumpDirec(ply_work);
		}
	}

	// 最大移動速度クリッピング◆GMD_MAIN_SPD_MAX

	/* 移動処理特殊 */
	if (  (ply_work->player_flag & GMD_PLF_AUTO_RUN)			// オートRUN中
		&&(!(ply_work->player_flag & GMD_PLF_DIE)) ) {			//  死亡中は除外
		// オートランチェック用コード
		//g_gm_main_system.map_fcol.left += ply_work->scroll_spd_x >> FX32_SHIFT;
		//g_gm_main_system.map_fcol.right += ply_work->scroll_spd_x >> FX32_SHIFT;
		//if (g_gm_main_system.map_fcol.left > 12000) {
		//	GmPlayerSetAutoRun(ply_work, 0, FALSE);
		//}

		// マップ限界(左)に引っかかった場合は、移動量を与える
		if (obj_work->pos.x <= (g_obj.camera[0][MTD_X] + 16*FX32_ONE) &&
				!(obj_work->pos.x <= (g_obj.camera[0][MTD_X] + 16*FX32_ONE) - 0x400*FX32_ONE)/* ループした瞬間だけ除く */) {

			if (obj_work->move_flag & OBD_MOVE_JUMP) {
				if (obj_work->spd.x < ply_work->scroll_spd_x) {
					obj_work->spd.x = ply_work->scroll_spd_x;

					if (obj_work->disp_flag & OBD_DISP_HFLIP) {
						// 反転
						GmPlySeqSetProgramTurn(ply_work, GMD_PL_PGM_TURN_SPD_DEF);
					}
				}
			}
			else {
				if (obj_work->spd_m < ply_work->scroll_spd_x) {
					obj_work->spd_m = ply_work->scroll_spd_x;

					if (GME_PLY_SEQ_STATE_LOOKUP_ST <= ply_work->seq_state &&
							ply_work->seq_state <= GME_PLY_SEQ_STATE_LOOKUP_END) {
						// 見上げの時はFWに戻す
						GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
					}

					if (obj_work->disp_flag & OBD_DISP_HFLIP) {
						// 反転
						if (ply_work->seq_state == GME_PLY_SEQ_STATE_SPIN) {
							// スピン中は即反転
							GmPlayerSetReverse(ply_work);
							GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
						} else
						if (GME_PLY_SEQ_STATE_SQUAT_ST <= ply_work->seq_state &&
								ply_work->seq_state <= GME_PLY_SEQ_STATE_SQUAT_END) {
							// しゃがみ動作中
							GmPlySeqSetProgramTurn(ply_work, GMD_PL_PGM_TURN_SPD_DEF);
						}
						else {
							// その他動作中
							GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_TURN);
						}
					}
				}
			}
		}
	}

	// 実移動処理
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// トロッコ時
		if (!(obj_work->move_flag & OBD_MOVE_UNDER) ||
				!(ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_STICK)) {
			// 踏ん張り中でない
			gmPlySeqTruckMove(obj_work);
		}
	} else if (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {
		// スペステ時
		gmPlySeqSplMove(obj_work);
	} else {
		// 通常時
		ObjObjectMove(obj_work);
	}
	
	/* 移動処理特殊 */

	// 落下速度復帰◆後で他の方法を取るかも
	//obj_work->spd_fall_max = spd_fall_max;
}

// =====================================================================
// 移動設定
// =====================================================================
// ================================================================
// GmPlySeqMoveWalk
/*!
  プレイヤー移動設定関数 歩き移動

  @param ply_work [in] プレイヤーワークポインタ
    
 */
// ================================================================
//static s32 gm_ply_seq_per_test = 0x00f80;
void GmPlySeqMoveWalk(GMS_PLAYER_WORK *ply_work)
{
    fx32 fSpdAdd;// = 0;
    fx32 fSpdMax;// = 0;
    fx32 fSpdDec;// = 0;

    fSpdAdd = ply_work->spd_add;
    fSpdDec = ply_work->spd_dec;
    fSpdMax = ply_work->spd_max;

#if 0
    if ( ply_work->player_flag & GMD_PLF_BOOST ) {
        fSpdMax = ply_work->spd_max_boost;
    }
    if ( ply_work->player_flag & GMD_PLF_NITRO ) {
        fSpdAdd = ply_work->spd_add_nitro;
        fSpdDec = ply_work->spd_dec_nitro;
        fSpdMax = ply_work->spd_max_nitro;
    }

    // ブーストチェック
    gmPlayerMoveBoostPoolCheck(ply_work);
#endif

	// 傾斜角に合わせて最高速度設定
	if (GmPlayerKeyCheckWalkRight(ply_work) ||
			GmPlayerKeyCheckWalkLeft(ply_work)) {		// 0になった時点で通常のMAX速度でチェックする
		s32	roll = MTM_MATH_ABS(ply_work->key_walk_rot_z);

		if (roll > GMD_PL_DEF_ROLL_MAX) {
			roll = GMD_PL_DEF_ROLL_MAX;
		}


		fSpdMax = fSpdMax * roll / GMD_PL_DEF_ROLL_MAX;
	}
	if (fSpdMax < ply_work->prev_walk_roll_spd_max) {
		// 減速は緩やかに
		fSpdMax = ply_work->prev_walk_roll_spd_max - fSpdDec;
		if (fSpdMax < 0) {
			fSpdMax = 0;
		}
	}
	ply_work->prev_walk_roll_spd_max = fSpdMax;

    // 角度に合わせて最大速度調整
    if ( ply_work->obj_work.dir.z ) {
        fx32 fSpd = FX_Mul(ply_work->spd_max_add_slope, mtMathSin(ply_work->obj_work.dir.z));
        if ( fSpd > 0 ) {
            fSpdMax += fSpd;
		}
    }
    
    if ( ply_work->no_spddown_timer ) {
        fSpdDec = 0;
    }
    // 速度に合わせて加速度修正
    else {
#if 1
        fx32 fPercent = 0;
        if ( MTM_MATH_ABS(ply_work->obj_work.spd_m ) > ply_work->spd3 ) {
			if (fSpdMax - ply_work->spd3) {
				fPercent = FX_Div(MTM_MATH_ABS(ply_work->obj_work.spd_m) - ply_work->spd3, fSpdMax - ply_work->spd3);
				if ( fPercent > FX32_ONE/*0x0100*/ ) {
					fPercent = FX32_ONE;
				}
				//fPercent = (s16)((fPercent * 0x00f8 ) >> 8);
			}
			else {
				fPercent = FX32_ONE;
			}
			// 倍率調整
			fPercent = (fPercent * 0x00f80) >> FX32_SHIFT;//◆どちらが早いかFX_Mul(fPercent, 0x00f80);
			//fPercent = (fPercent * gm_ply_seq_per_test) >> FX32_SHIFT;//◆どちらが早いかFX_Mul(fPercent, 0x00f80);
        }
        fSpdAdd = fSpdAdd - FX_Mul(fSpdAdd, fPercent);
        //fSpdAdd = (s16)(fSpdAdd - ((fSpdAdd * fPercent) >> 8));
#else
        fx32 fPercent = 0;
        if ( MTM_MATH_ABS(ply_work->obj_work.spd_m ) > ply_work->spd2 ) {
            fPercent = FX_Div(MTM_MATH_ABS(ply_work->obj_work.spd_m) - ply_work->spd2, fSpdMax - ply_work->spd2);
            if ( fPercent > FX32_ONE/*0x0100*/ ) {
                fPercent = FX32_ONE;
			}
            // 倍率調整
            fPercent = (fPercent * 0x00f80) >> FX32_SHIFT;//◆どちらが早いかFX_Mul(fPercent, 0x00f80);
            //fPercent = (s16)((fPercent * 0x00f8 ) >> 8);
        }
        fSpdAdd = fSpdAdd - FX_Mul(fSpdAdd, fPercent);
        //fSpdAdd = (s16)(fSpdAdd - ((fSpdAdd * fPercent) >> 8));
#endif
    }

	// 水中対応
    if ( ply_work->player_flag & GMD_PLF_WATER ) {
        GMD_PLAYER_WATER_SET(fSpdAdd);
        GMD_PLAYER_WATER_SET(fSpdDec);
    }

    // 坂道瞬間最大速度アップ
    if ( ply_work->spd_work_max >= fSpdMax &&
			MTM_MATH_ABS(ply_work->obj_work.spd_m) >= fSpdMax ) {
        if ( ply_work->spd_work_max > ply_work->obj_work.spd_m ) {
            ply_work->spd_work_max = MTM_MATH_ABS(ply_work->obj_work.spd_m);
		}
        fSpdMax = ply_work->spd_work_max;
    }

	// オートラン最高速度補正
	if (ply_work->player_flag & GMD_PLF_AUTO_RUN) {
		if (GmPlayerKeyCheckWalkRight(ply_work) &&
				(fSpdMax > ply_work->scroll_spd_x + GMD_PL_AUTO_RUN_MAX_SPD_OFST)) {
			// 右移動中は最高速度減少
			fSpdMax = ply_work->scroll_spd_x + GMD_PL_AUTO_RUN_MAX_SPD_OFST;
		}
	}

    // 加速
	{
	// 通常
	    if ( GmPlayerKeyCheckWalkLeft(ply_work) | GmPlayerKeyCheckWalkRight(ply_work) ) {
	        if ( GmPlayerKeyCheckWalkRight(ply_work) ) {
			// 反転はシーケンス変更部で
	        //    if ( ply_work->act_state != GMD_PLY_STATE_TURN && !(ply_work->act_state >= GMD_PLY_STATE_BRAKE1 && ply_work->act_state <= GMD_PLY_STATE_BRAKED3) ) {
	        //        ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;	// 即時反転
			//	}

	            if ( ply_work->obj_work.spd_m < 0 ) {
	                ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
				}
	            ply_work->obj_work.spd_m = ObjSpdUpSet( ply_work->obj_work.spd_m, fSpdAdd, fSpdMax);
	        }
			else {
			// 反転はシーケンス変更部で
	        //    if ( ply_work->act_state != GMD_PLY_STATE_TURN && !(ply_work->act_state >= GMD_PLY_STATE_BRAKE1 && ply_work->act_state <= GMD_PLY_STATE_BRAKED3) ) {
	        //        ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;		// 即時反転
			//	}
	            if ( ply_work->obj_work.spd_m > 0 ) {
	                ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
				}
	            ply_work->obj_work.spd_m = ObjSpdUpSet( ply_work->obj_work.spd_m,-fSpdAdd, fSpdMax);
	        }
	    }
		else {
	        ply_work->spd_pool = 0;
	        ply_work->obj_work.spd.x = MTM_MATH_CLIP( ply_work->obj_work.spd.x, -fSpdMax, fSpdMax );
	        ply_work->obj_work.spd_m = MTM_MATH_CLIP( ply_work->obj_work.spd_m, -fSpdMax, fSpdMax );
	        if ( (((ply_work->obj_work.dir.z + 0x2000) & 0xff00) <= ( 0x2000 << 1)) ) {
	            // 減速
	            if ( ply_work->player_flag & GMD_PLF_NOBRAKE ){
	                // このフレームは減速できません
	                ply_work->player_flag &= ~GMD_PLF_NOBRAKE;
	                return;
	            }
				if (ply_work->player_flag & GMD_PLF_AUTO_RUN) {
					// オートラン時減速
					fx32	min_spd;

					if (!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) &&
							ply_work->seq_state == GME_PLY_SEQ_STATE_WALK) {
						min_spd = ply_work->scroll_spd_x + GMD_PL_AUTO_RUN_FREE_DEC_MIN_SPD_OFST;
						if (min_spd < 0) {
							min_spd = 0;
						}

						ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
						if (ply_work->obj_work.spd_m < min_spd) {
							// 右向き走りでキーを離した減速では一定速度以下にはならない
							ply_work->obj_work.spd_m = min_spd;
						}
					}
					else {
						// 通常減速
						ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
					}
				}
				else {
					// 通常減速
					ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
				}
	        }
	    }
    }
    // 特定ギミックZ移動チェック
    //GmPlayerMoveZ(ply_work);
}

// ================================================================
// GmPlySeqMoveWalkTruck
/*!
  プレイヤー移動設定関数 歩き移動 トロッコ用

  @param ply_work [in] プレイヤーワークポインタ
    
 */
// ================================================================
void GmPlySeqMoveWalkTruck(GMS_PLAYER_WORK *ply_work)
{
    fx32 fSpdAdd;// = 0;
    fx32 fSpdMax;// = 0;
    fx32 fSpdDec;// = 0;
	u16	now_dir;

	// 踏ん張り時は移動しない
	if (ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_STICK) {
		return;
	}

    fSpdAdd = ply_work->spd_add;
    fSpdDec = ply_work->spd_dec;
    fSpdMax = ply_work->spd_max;

	// 重力角度と画面角度を加味した現在の角度
	now_dir = (u16)(ply_work->obj_work.dir.z + (ply_work->obj_work.dir_fall - g_gm_main_system.pseudofall_dir));

#if 0
	// 傾斜角に合わせて最高速度設定
	if (GmPlayerKeyCheckWalkRight(ply_work) ||
			GmPlayerKeyCheckWalkLeft(ply_work)) {		// 0になった時点で通常のMAX速度でチェックする
		s32	roll = MTM_MATH_ABS(ply_work->key_walk_rot_z);

		if (roll > GMD_PL_DEF_ROLL_MAX) {
			roll = GMD_PL_DEF_ROLL_MAX;
		}


		fSpdMax = fSpdMax * roll / GMD_PL_DEF_ROLL_MAX;
	}
	if (fSpdMax < ply_work->prev_walk_roll_spd_max) {
		// 減速は緩やかに
		fSpdMax = ply_work->prev_walk_roll_spd_max - fSpdDec;
		if (fSpdMax < 0) {
			fSpdMax = 0;
		}
	}
	ply_work->prev_walk_roll_spd_max = fSpdMax;
#endif

    // 角度に合わせて最大速度調整
	if (now_dir) {
        fx32 fSpd = FX_Mul(ply_work->spd_max_add_slope, mtMathSin(now_dir));
        if ( fSpd > 0 ) {
            fSpdMax += fSpd;
		}
    }
//	if ( ply_work->obj_work.dir.z ) {
//        fx32 fSpd = FX_Mul(ply_work->spd_max_add_slope, mtMathSin(ply_work->obj_work.dir.z));
//        if ( fSpd > 0 ) {
//            fSpdMax += fSpd;
//		}
//    }
    
    if ( ply_work->no_spddown_timer ) {
        fSpdDec = 0;
    }
    // 速度に合わせて加速度修正
    else {
        fx32 fPercent = 0;
        if ( MTM_MATH_ABS(ply_work->obj_work.spd_m ) > ply_work->spd3 ) {
			if (fSpdMax - ply_work->spd3) {
				fPercent = FX_Div(MTM_MATH_ABS(ply_work->obj_work.spd_m) - ply_work->spd3, fSpdMax - ply_work->spd3);
				if ( fPercent > FX32_ONE/*0x0100*/ ) {
					fPercent = FX32_ONE;
				}
				//fPercent = (s16)((fPercent * 0x00f8 ) >> 8);
			}
			else {
				fPercent = FX32_ONE;
			}
			// 倍率調整
			fPercent = (fPercent * 0x00f80) >> FX32_SHIFT;//◆どちらが早いかFX_Mul(fPercent, 0x00f80);
			//fPercent = (fPercent * gm_ply_seq_per_test) >> FX32_SHIFT;//◆どちらが早いかFX_Mul(fPercent, 0x00f80);
        }
        fSpdAdd = fSpdAdd - FX_Mul(fSpdAdd, fPercent);
        //fSpdAdd = (s16)(fSpdAdd - ((fSpdAdd * fPercent) >> 8));
    }

	// 水中対応
    if ( ply_work->player_flag & GMD_PLF_WATER ) {
        GMD_PLAYER_WATER_SET(fSpdAdd);
        GMD_PLAYER_WATER_SET(fSpdDec);
    }

    // 坂道瞬間最大速度アップ
    if ( ply_work->spd_work_max >= fSpdMax &&
			MTM_MATH_ABS(ply_work->obj_work.spd_m) >= fSpdMax ) {
        if ( ply_work->spd_work_max > ply_work->obj_work.spd_m ) {
            ply_work->spd_work_max = MTM_MATH_ABS(ply_work->obj_work.spd_m);
		}
        fSpdMax = ply_work->spd_work_max;
    }

	if ((g_gm_main_system.game_flag & GMD_GAME_FLAG_GOAL_IN ||
				ply_work->player_flag & GMD_PLF_GOAL) &&
				(GmPlayerKeyCheckWalkLeft(ply_work) | GmPlayerKeyCheckWalkRight(ply_work))) {
		// ゴール演出中のみ
		// 加速
		if ( GmPlayerKeyCheckWalkRight(ply_work) ) {
		// 反転はシーケンス変更部で
		//	if ( ply_work->act_state != GMD_PLY_STATE_TURN && !(ply_work->act_state >= GMD_PLY_STATE_BRAKE1 && ply_work->act_state <= GMD_PLY_STATE_BRAKED3) ) {
		//        ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;	// 即時反転
		//	}

			if ( ply_work->obj_work.spd_m < 0 ) {
				ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
			}
			ply_work->obj_work.spd_m = ObjSpdUpSet( ply_work->obj_work.spd_m, fSpdAdd, fSpdMax);
		}
		else {
		// 反転はシーケンス変更部で
		//	if ( ply_work->act_state != GMD_PLY_STATE_TURN && !(ply_work->act_state >= GMD_PLY_STATE_BRAKE1 && ply_work->act_state <= GMD_PLY_STATE_BRAKED3) ) {
		//        ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;		// 即時反転
		//	}
			if ( ply_work->obj_work.spd_m > 0 ) {
				ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
			}
			ply_work->obj_work.spd_m = ObjSpdUpSet( ply_work->obj_work.spd_m,-fSpdAdd, fSpdMax);
		}
	}
#if 1
	else if (((now_dir + ply_work->obj_work.dir_slope) & 0xffff) < (ply_work->obj_work.dir_slope << 1)) {
#else
	else if (!(ply_work->key_rot_z || ply_work->key_walk_rot_z)) {
#endif
	// 減速
		ply_work->spd_pool = 0;
		ply_work->obj_work.spd.x = MTM_MATH_CLIP( ply_work->obj_work.spd.x, -fSpdMax, fSpdMax );
		ply_work->obj_work.spd_m = MTM_MATH_CLIP( ply_work->obj_work.spd_m, -fSpdMax, fSpdMax );
	        //if ( (((ply_work->obj_work.dir.z + 0x2000) & 0xff00) <= ( 0x2000 << 1)) ) {
		if ( (((now_dir + 0x0800) & 0xff00) <= ( 0x0800 << 1)) ) {
			// 減速
			if ( ply_work->player_flag & GMD_PLF_NOBRAKE ){
				// このフレームは減速できません
				ply_work->player_flag &= ~GMD_PLF_NOBRAKE;
				return;
			}
			ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
		}
	}
}

// ================================================================
// GmPlySeqMoveWalkAutoRun
/*!
  プレイヤー移動設定関数 歩き移動 オートラン用

  @param ply_work [in] プレイヤーワークポインタ
    
 */
// ================================================================
void GmPlySeqMoveWalkAutoRun(GMS_PLAYER_WORK *ply_work)
{
    fx32 fSpdAdd;// = 0;
    fx32 fSpdMax;// = 0;
    fx32 fSpdDec;// = 0;

    fSpdAdd = ply_work->spd_add;
    fSpdDec = ply_work->spd_dec;
    fSpdMax = ply_work->spd_max;

	// オートランでは常にキー操作は同じにしておく
	//ply_work->spd_work_max = ply_work->spd_work_max + 0x1000;	//GMD_PL_AUTO_RUN_MAX_SPD_OFST;
    fSpdMax = FX_F32_TO_FX32(9.5f);//ply_work->spd_work_max + 0x1000;	//GMD_PL_AUTO_RUN_MAX_SPD_OFST;

	// オートラン加速度補正
	if (GmPlayerKeyCheckWalkRight(ply_work)) {
		// 右移動中は加速度減少
		if (ply_work->obj_work.spd_m <= ply_work->spd3) {
			fSpdAdd >>= 2;
		}

		if (ply_work->obj_work.spd_m < ply_work->scroll_spd_x + FX_F32_TO_FX32( 0.25f )) {
			// 右にキーが入った時は最低速度を保つ
			ply_work->obj_work.spd_m = ply_work->scroll_spd_x + FX_F32_TO_FX32( 0.25f );
		}

		// 操作がしにくいので最低レベルの速度は保つ
		if ( ply_work->obj_work.spd_m < FX_F32_TO_FX32( 8.4 )){
			ply_work->obj_work.spd_m = FX_F32_TO_FX32( 8.4 );
		}
		// 操作がしにくいので最低レベルの速度は保つ
		if ( ply_work->obj_work.spd_m > FX_F32_TO_FX32( 8.7 )){
			ply_work->obj_work.spd_m = FX_F32_TO_FX32( 8.7 );
		}
	}

	// 傾斜角に合わせて最高速度設定
	if (GmPlayerKeyCheckWalkRight(ply_work) ||
			GmPlayerKeyCheckWalkLeft(ply_work)) {		// 0になった時点で通常のMAX速度でチェックする
		s32	roll = MTM_MATH_ABS(ply_work->key_walk_rot_z);

		if (roll > GMD_PL_DEF_ROLL_MAX) {
			roll = GMD_PL_DEF_ROLL_MAX;
		}


		fSpdMax = fSpdMax * roll / GMD_PL_DEF_ROLL_MAX;
	}
	if (fSpdMax < ply_work->prev_walk_roll_spd_max) {
		// 減速は緩やかに
		fSpdMax = ply_work->prev_walk_roll_spd_max - fSpdDec;
		if (fSpdMax < 0) {
			fSpdMax = 0;
		}
	}
	ply_work->prev_walk_roll_spd_max = fSpdMax;

    // 角度に合わせて最大速度調整
    if ( ply_work->obj_work.dir.z ) {
        fx32 fSpd = FX_Mul(ply_work->spd_max_add_slope, mtMathSin(ply_work->obj_work.dir.z));
        if ( fSpd > 0 ) {
            fSpdMax += fSpd;
		}
    }
    
    if ( ply_work->no_spddown_timer ) {
        fSpdDec = 0;
    }
    // 速度に合わせて加速度修正
    else {
        fx32 fPercent = 0;
        if ( MTM_MATH_ABS(ply_work->obj_work.spd_m ) > ply_work->spd3 ) {
			if (fSpdMax - ply_work->spd3) {
				fPercent = FX_Div(MTM_MATH_ABS(ply_work->obj_work.spd_m) - ply_work->spd3, fSpdMax - ply_work->spd3);
				if ( fPercent > FX32_ONE/*0x0100*/ ) {
					fPercent = FX32_ONE;
				}
				//fPercent = (s16)((fPercent * 0x00f8 ) >> 8);
			}
			else {
				fPercent = FX32_ONE;
			}
			// 倍率調整
			fPercent = (fPercent * 0x00f80) >> FX32_SHIFT;//FX_Mul(fPercent, 0x00f80);
        }
        fSpdAdd = fSpdAdd - FX_Mul(fSpdAdd, fPercent);
        //fSpdAdd = (s16)(fSpdAdd - ((fSpdAdd * fPercent) >> 8));
    }

	// 水中対応
    if ( ply_work->player_flag & GMD_PLF_WATER ) {
        GMD_PLAYER_WATER_SET(fSpdAdd);
        GMD_PLAYER_WATER_SET(fSpdDec);
    }

    // 坂道瞬間最大速度アップ
    if ( ply_work->spd_work_max >= fSpdMax &&
			MTM_MATH_ABS(ply_work->obj_work.spd_m) >= fSpdMax ) {
        if ( ply_work->spd_work_max > ply_work->obj_work.spd_m ) {
            ply_work->spd_work_max = MTM_MATH_ABS(ply_work->obj_work.spd_m);
		}
        fSpdMax = ply_work->spd_work_max;
    }

	// オートラン最高速度補正
	if (ply_work->player_flag & GMD_PLF_AUTO_RUN) {

		// オートランでは常にキー操作は同じにしておく
		ply_work->spd_work_max = ply_work->spd_work_max + GMD_PL_AUTO_RUN_MAX_SPD_OFST;
        fSpdMax = ply_work->spd_work_max + GMD_PL_AUTO_RUN_MAX_SPD_OFST;
/*
		if (GmPlayerKeyCheckWalkRight(ply_work) &&
				(fSpdMax > ply_work->scroll_spd_x + GMD_PL_AUTO_RUN_MAX_SPD_OFST)) {
			// 右移動中は最高速度減少
			fSpdMax = ply_work->scroll_spd_x + GMD_PL_AUTO_RUN_MAX_SPD_OFST;
		}
*/
	}

    // 加速
	{
	// 通常
	    if ( GmPlayerKeyCheckWalkLeft(ply_work) | GmPlayerKeyCheckWalkRight(ply_work) ) {
	        if ( GmPlayerKeyCheckWalkRight(ply_work) ) {
			// 反転はシーケンス変更部で
	            if ( ply_work->obj_work.spd_m < 0 ) {
	                ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
				}
	            ply_work->obj_work.spd_m = ObjSpdUpSet( ply_work->obj_work.spd_m, fSpdAdd, fSpdMax);
	        }
			else {
			// 反転はシーケンス変更部で
	            if ( ply_work->obj_work.spd_m > 0 ) {
	                ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
				}
	            ply_work->obj_work.spd_m = ObjSpdUpSet( ply_work->obj_work.spd_m,-fSpdAdd, fSpdMax);
	        }
	    }
		else {
	        ply_work->spd_pool = 0;
	        ply_work->obj_work.spd.x = MTM_MATH_CLIP( ply_work->obj_work.spd.x, -fSpdMax, fSpdMax );
	        ply_work->obj_work.spd_m = MTM_MATH_CLIP( ply_work->obj_work.spd_m, -fSpdMax, fSpdMax );
	        if ( (((ply_work->obj_work.dir.z + 0x2000) & 0xff00) <= ( 0x2000 << 1)) ) {
	            // 減速
	            if ( ply_work->player_flag & GMD_PLF_NOBRAKE ){
	                // このフレームは減速できません
	                ply_work->player_flag &= ~GMD_PLF_NOBRAKE;
	                return;
	            }
				if (ply_work->player_flag & GMD_PLF_AUTO_RUN) {
					// オートラン時減速
					fx32	min_spd;

					if (!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) &&
							ply_work->seq_state == GME_PLY_SEQ_STATE_WALK) {
						min_spd = ply_work->scroll_spd_x + GMD_PL_AUTO_RUN_FREE_DEC_MIN_SPD_OFST;
						if (min_spd < 0) {
							min_spd = 0;
						}

						ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
						if (ply_work->obj_work.spd_m < min_spd) {
							// 右向き走りでキーを離した減速では一定速度以下にはならない
							ply_work->obj_work.spd_m = min_spd;
						}
					}
					else {
						// 通常減速
						ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
					}
				}
				else {
					// 通常減速
					ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
				}
	        }
	    }
    }
}

// ================================================================
// GmPlySeqMoveJump
/*!
  プレイヤー移動設定関数 ジャンプ時移動

  @param ply_work [in] プレイヤーワークポインタ
    
 */
// ================================================================
void GmPlySeqMoveJump(GMS_PLAYER_WORK *ply_work)
{
    fx32 fSpdAdd;// = 0;
    fx32 fSpdMax;// = 0;
    fx32 fSpdDec;// = 0;
    fx32 fSpdDecM = 0;
    fx32 fSpdDecBrake;// = 0;
    fx32 fSpdDecMBrake = 0;

    fSpdAdd = ply_work->spd_jump_add;
    fSpdDec  = ply_work->spd_jump_dec;
    fSpdDecM = ply_work->spd_jump_dec;
    fSpdMax = ply_work->spd_jump_max;
    ply_work->spd_work_max = 0;

#if 0
    if ( ply_work->player_flag & GMD_PLF_BOOST ){
        fSpdMax = ply_work->spd_max_boost;
    }

    if ( ply_work->player_flag & GMD_PLF_NITRO ){
        fSpdAdd = ply_work->spd_add_nitro;
        fSpdDec = ply_work->spd_dec_nitro;
        fSpdMax = ply_work->spd_max_nitro;
    }
    // ブーストチェック
    gmPlayerMoveBoostPoolCheck(ply_work);

    if ( ply_work->blaze_timer ){
		// ブレイズ瞬間移動中
        return;
    }
#endif

    // 加速減チェック
    if (((ply_work->obj_work.dir.z + 0x2000) & 0xc000 || ply_work->obj_work.dir.z == 0xe000) ){
        fSpdDec >>= 2;
    }
    if ( ply_work->no_spddown_timer ){
        fSpdDec = 0;
        fSpdAdd >>= 2;
    }
    // 速度に合わせて加速度修正
    else {
        fx32 fPercent = 0;
        if ( MTM_MATH_ABS(ply_work->obj_work.spd.x ) > ply_work->spd2 ) {
			if (fSpdMax - ply_work->spd2) {
				fPercent = FX_Div(MTM_MATH_ABS(ply_work->obj_work.spd.x) - ply_work->spd2, fSpdMax - ply_work->spd2);
				if ( fPercent > FX32_ONE/*0x0100*/ ) {
					fPercent = FX32_ONE;
				}
			}
			else {
				fPercent = FX32_ONE;
			}
            // 倍率調整
            fPercent = (fPercent * 0x00f80) >> FX32_SHIFT;//◆どちらが早いかFX_Mul(fPercent, 0x00f80);
            //fPercent = (s16)((fPercent * 0x00f8 ) >> 8);
        }
        fSpdAdd = fSpdAdd - FX_Mul(fSpdAdd, fPercent);
        //fSpdAdd = (s16)(fSpdAdd - ((fSpdAdd * fPercent) >> 8));
    }

	// 水中対応
    if ( ply_work->player_flag & GMD_PLF_WATER ){
        GMD_PLAYER_WATER_SET(fSpdAdd);
        GMD_PLAYER_WATER_SET(fSpdDec);
    }

	// ブレーキ減速度
	//fSpdDecBrake = FX_Mul(fSpdDec, (fx32)(FX32_ONE*1.5));
	//fSpdDecMBrake = FX_Mul(fSpdDecM, (fx32)(FX32_ONE*1.5));
	fSpdDecBrake = FX_Mul(fSpdDec, (fx32)(FX32_ONE*1.0));
	fSpdDecMBrake = FX_Mul(fSpdDecM, (fx32)(FX32_ONE*1.0));

//    if ( ply_work->blaze_timer ){	// 上に移動
//		// ブレイズ瞬間移動中
//        return;
//    }

    // 加減速
	{
	// 通常
	    if ( GmPlayerKeyCheckWalkLeft(ply_work) | GmPlayerKeyCheckWalkRight(ply_work) ){
	        if ( GmPlayerKeyCheckWalkRight(ply_work) ){
	            //ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;

	            if ( ply_work->obj_work.spd.x < 0 ){
	                ply_work->obj_work.spd.x = ObjSpdDownSet( ply_work->obj_work.spd.x, fSpdDecBrake);
	            }
	           // if ( ply_work->obj_work.spd_m < 0 ){		// M速度は空中ではかならず減速するように変更
	                ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDecM);
	           // }
	            ply_work->obj_work.spd.x = ObjSpdUpSet( ply_work->obj_work.spd.x, fSpdAdd, fSpdMax);
	        }
			else {
	            //ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
	            if ( ply_work->obj_work.spd.x > 0 ){
	                ply_work->obj_work.spd.x = ObjSpdDownSet( ply_work->obj_work.spd.x, fSpdDecBrake);
	            }
	           // if ( ply_work->obj_work.spd_m > 0 ){		// M速度は空中ではかならず減速するように変更
	                ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDecM);
	           // }
	            ply_work->obj_work.spd.x = ObjSpdUpSet( ply_work->obj_work.spd.x,(-fSpdAdd), fSpdMax);
	        }
	    }
		else {
	        ply_work->obj_work.spd.x = MTM_MATH_CLIP( ply_work->obj_work.spd.x, -fSpdMax, fSpdMax );
	        ply_work->obj_work.spd_m = MTM_MATH_CLIP( ply_work->obj_work.spd_m, -fSpdMax, fSpdMax );
	        ply_work->spd_pool = 0;
	        ply_work->obj_work.spd.x = ObjSpdDownSet( ply_work->obj_work.spd.x, fSpdDec);
	        ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDecM);
	    }
	}

    // 特定ギミックZ移動チェック
    //GmPlayerMoveZ(ply_work);
}

// ================================================================
// GmPlySeqMoveJumpTruck
/*!
  プレイヤー移動設定関数 ジャンプ時移動 トロッコ

  @param ply_work [in] プレイヤーワークポインタ
    
 */
// ================================================================
void GmPlySeqMoveJumpTruck(GMS_PLAYER_WORK *ply_work)
{
    fx32 fSpdAdd;// = 0;
    fx32 fSpdMax;// = 0;
    fx32 fSpdDec;// = 0;
    fx32 fSpdDecM = 0;
    fx32 fSpdDecBrake;// = 0;
    fx32 fSpdDecMBrake = 0;

	//u16	now_dir;

    fSpdAdd = ply_work->spd_jump_add;
    fSpdDec  = ply_work->spd_jump_dec;
    fSpdDecM = ply_work->spd_jump_dec;
    fSpdMax = ply_work->spd_jump_max;
    ply_work->spd_work_max = 0;

#if 0 // ◎トロッコ新仕様対応
	// 一定角度で横移動停止
	now_dir = (u16)(ply_work->obj_work.dir.z + (ply_work->obj_work.dir_fall - g_gm_main_system.pseudofall_dir));
	if (0x4000 < now_dir &&
			now_dir < 0xC000) {
		ply_work->obj_work.spd_m = 0;
		ply_work->obj_work.spd.x = 0;

		return;
	}
#endif
    // 加速減チェック
    if (((ply_work->obj_work.dir.z + 0x2000) & 0xc000 || ply_work->obj_work.dir.z == 0xe000) ){
        fSpdDec >>= 2;
    }
    if ( ply_work->no_spddown_timer ){
        fSpdDec = 0;
        fSpdAdd >>= 2;
    }
    // 速度に合わせて加速度修正
    else {
        fx32 fPercent = 0;
        if ( MTM_MATH_ABS(ply_work->obj_work.spd.x ) > ply_work->spd2 ) {
			if (fSpdMax - ply_work->spd2) {
				fPercent = FX_Div(MTM_MATH_ABS(ply_work->obj_work.spd.x) - ply_work->spd2, fSpdMax - ply_work->spd2);
				if ( fPercent > FX32_ONE/*0x0100*/ ) {
					fPercent = FX32_ONE;
				}
			}
			else {
				fPercent = FX32_ONE;
			}
            // 倍率調整
            fPercent = (fPercent * 0x00f80) >> FX32_SHIFT;//◆どちらが早いかFX_Mul(fPercent, 0x00f80);
            //fPercent = (s16)((fPercent * 0x00f8 ) >> 8);
        }
        fSpdAdd = fSpdAdd - FX_Mul(fSpdAdd, fPercent);
        //fSpdAdd = (s16)(fSpdAdd - ((fSpdAdd * fPercent) >> 8));
    }

	// 水中対応
    if ( ply_work->player_flag & GMD_PLF_WATER ){
        GMD_PLAYER_WATER_SET(fSpdAdd);
        GMD_PLAYER_WATER_SET(fSpdDec);
    }

	// ブレーキ減速度
	fSpdDecBrake = FX_Mul(fSpdDec, (fx32)(FX32_ONE*1.0));
	fSpdDecMBrake = FX_Mul(fSpdDecM, (fx32)(FX32_ONE*1.0));

    // 加減速
#if _IPHONE
	// 特定条件での傾き方向への加速
	// iPhoneフリック操作対策
#define GMD_PLY_SEQ_TRUCK_MAX_SPEED_JUMP (3* FX32_ONE)
	// 特殊スプリングジャンプ時
	if (ply_work->gmk_flag2 & GMD_PLGF2_TRUCK_JUMP_MOVE_ROT) {
		u16 dir = (ply_work->obj_work.dir_fall - g_gm_main_system.pseudofall_dir);
		ply_work->obj_work.spd.x += mtMathSin(dir) / 3;
		if (ply_work->obj_work.spd.x > GMD_PLY_SEQ_TRUCK_MAX_SPEED_JUMP) {
			ply_work->obj_work.spd.x = GMD_PLY_SEQ_TRUCK_MAX_SPEED_JUMP;
		}
		else if (ply_work->obj_work.spd.x < -GMD_PLY_SEQ_TRUCK_MAX_SPEED_JUMP) {
			ply_work->obj_work.spd.x = -GMD_PLY_SEQ_TRUCK_MAX_SPEED_JUMP;
		}
	} else
#endif // _IPHONE
	if (ply_work->gmk_flag2 & GMD_PLGF2_TRUCK_SLOPEFLY_DEC) {
	// 坂道飛び出し時
		fSpdDecBrake	= FX_Mul(fSpdDecBrake, GMD_PLY_SEQ_T_SLOPEFLY_DEC_PER);
		fSpdDecMBrake	= FX_Mul(fSpdDecMBrake, GMD_PLY_SEQ_T_SLOPEFLY_DEC_PER);

		ply_work->obj_work.spd.x = MTM_MATH_CLIP( ply_work->obj_work.spd.x, -fSpdMax, fSpdMax );
		ply_work->obj_work.spd_m = MTM_MATH_CLIP( ply_work->obj_work.spd_m, -fSpdMax, fSpdMax );
		ply_work->spd_pool = 0;
		ply_work->obj_work.spd.x = ObjSpdDownSet( ply_work->obj_work.spd.x, fSpdDecBrake);
		ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDecMBrake);
	}
	else {
	// 通常
	    if ( GmPlayerKeyCheckWalkLeft(ply_work) | GmPlayerKeyCheckWalkRight(ply_work) ){
	        if ( GmPlayerKeyCheckWalkRight(ply_work) ){

	            if ( ply_work->obj_work.spd.x < 0 ){
	                ply_work->obj_work.spd.x = ObjSpdDownSet( ply_work->obj_work.spd.x, fSpdDecBrake);
	            }
	            if ( ply_work->obj_work.spd_m < 0 ){
	                ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDecM);
	            }
	            ply_work->obj_work.spd.x = ObjSpdUpSet( ply_work->obj_work.spd.x, fSpdAdd, fSpdMax);
	        }
			else {
	            if ( ply_work->obj_work.spd.x > 0 ){
	                ply_work->obj_work.spd.x = ObjSpdDownSet( ply_work->obj_work.spd.x, fSpdDecBrake);
	            }
	            if ( ply_work->obj_work.spd_m > 0 ){
	                ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDecM);
	            }
	            ply_work->obj_work.spd.x = ObjSpdUpSet( ply_work->obj_work.spd.x,(-fSpdAdd), fSpdMax);
	        }
	    }
		else {
	        ply_work->obj_work.spd.x = MTM_MATH_CLIP( ply_work->obj_work.spd.x, -fSpdMax, fSpdMax );
	        ply_work->obj_work.spd_m = MTM_MATH_CLIP( ply_work->obj_work.spd_m, -fSpdMax, fSpdMax );
	        ply_work->spd_pool = 0;
	        ply_work->obj_work.spd.x = ObjSpdDownSet( ply_work->obj_work.spd.x, fSpdDec);
	        ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDecM);
	    }
	}
}

// ================================================================
// GmPlySeqMoveJumpAutoRun
/*!
  プレイヤー移動設定関数 ジャンプ時移動 オートラン用

  @param ply_work [in] プレイヤーワークポインタ
    
 */
// ================================================================
void GmPlySeqMoveJumpAutoRun(GMS_PLAYER_WORK *ply_work)
{
    fx32 fSpdAdd;// = 0;
    fx32 fSpdMax;// = 0;
    fx32 fSpdDec;// = 0;
    fx32 fSpdDecM = 0;
    fx32 fSpdDecBrake;// = 0;
    fx32 fSpdDecMBrake = 0;

    fSpdAdd = ply_work->spd_jump_add;
    fSpdDec  = ply_work->spd_jump_dec;
    fSpdDecM = ply_work->spd_jump_dec;
    fSpdMax = ply_work->spd_jump_max;
    ply_work->spd_work_max = 0;

	// オートラン加速度補正
	if (GmPlayerKeyCheckWalkRight(ply_work)) {
		// 右移動中は加速度減少
		//fSpdAdd >>= 2;
		//fSpdAdd = FX_Mul( fSpdAdd, FX_F32_TO_FX32( 0.05f ) );
		fSpdAdd = 0;
	}

    // 加速減チェック
    if (((ply_work->obj_work.dir.z + 0x2000) & 0xc000 || ply_work->obj_work.dir.z == 0xe000) ){
        fSpdDec >>= 2;
    }
    if ( ply_work->no_spddown_timer ){
        fSpdDec = 0;
        fSpdAdd >>= 2;
    }
    // 速度に合わせて加速度修正
    else {
        fx32 fPercent = 0;
        if ( MTM_MATH_ABS(ply_work->obj_work.spd.x ) > ply_work->spd2 ) {
			if (fSpdMax - ply_work->spd2) {
				fPercent = FX_Div(MTM_MATH_ABS(ply_work->obj_work.spd.x) - ply_work->spd2, fSpdMax - ply_work->spd2);
				if ( fPercent > FX32_ONE/*0x0100*/ ) {
					fPercent = FX32_ONE;
				}
			}
			else {
				fPercent = FX32_ONE;
			}
            // 倍率調整
            fPercent = (fPercent * 0x00f80) >> FX32_SHIFT;//◆FX_Mul(fPercent, 0x00f80);
            //fPercent = (s16)((fPercent * 0x00f8 ) >> 8);
        }
        fSpdAdd = fSpdAdd - FX_Mul(fSpdAdd, fPercent);
        //fSpdAdd = (s16)(fSpdAdd - ((fSpdAdd * fPercent) >> 8));
    }

	// 水中対応
    if ( ply_work->player_flag & GMD_PLF_WATER ){
        GMD_PLAYER_WATER_SET(fSpdAdd);
        GMD_PLAYER_WATER_SET(fSpdDec);
    }

	// ブレーキ減速度
	fSpdDecBrake = FX_Mul(fSpdDec, (fx32)(FX32_ONE*1.0));
	fSpdDecMBrake = FX_Mul(fSpdDecM, (fx32)(FX32_ONE*1.0));


    // 加減速
	{
	// 通常
	    if ( GmPlayerKeyCheckWalkLeft(ply_work) | GmPlayerKeyCheckWalkRight(ply_work) ){
	        if ( GmPlayerKeyCheckWalkRight(ply_work) ){
				// 右にカーソルを入れている場合
	            //ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;

	            if ( ply_work->obj_work.spd.x < 0 ){
	                ply_work->obj_work.spd.x = ObjSpdDownSet( ply_work->obj_work.spd.x, fSpdDecBrake);
	            }
	            if ( ply_work->obj_work.spd_m < 0 ){
	                ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDecM);
	            }
	            ply_work->obj_work.spd.x = ObjSpdUpSet( ply_work->obj_work.spd.x, fSpdAdd, fSpdMax);
	        }
			else {
				// 左にカーソルを入れている場合
	            //ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
	            if ( ply_work->obj_work.spd.x > 0 ){
	                ply_work->obj_work.spd.x = ObjSpdDownSet( ply_work->obj_work.spd.x, fSpdDecBrake);
	            }
	            if ( ply_work->obj_work.spd_m > 0 ){
	                ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDecM);
	            }
	            ply_work->obj_work.spd.x = ObjSpdUpSet( ply_work->obj_work.spd.x,(-fSpdAdd), fSpdMax);
	        }
	    }
		else {
			// カーソルを入れてない場合
	        ply_work->obj_work.spd.x = MTM_MATH_CLIP( ply_work->obj_work.spd.x, -fSpdMax, fSpdMax );
	        ply_work->obj_work.spd_m = MTM_MATH_CLIP( ply_work->obj_work.spd_m, -fSpdMax, fSpdMax );
	        ply_work->spd_pool = 0;
	        ply_work->obj_work.spd.x = ObjSpdDownSet( ply_work->obj_work.spd.x, fSpdDec);
	        ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDecM);
	    }
	}
}

// ================================================================
// GmPlySeqMoveSpin
/*!
  プレイヤー移動設定関数 スピン時移動

  @param ply_work [in] プレイヤーワークポインタ
    
 */
// ================================================================
void GmPlySeqMoveSpin(GMS_PLAYER_WORK *ply_work)
{
#if 1
	fx32	dec_spin;

	dec_spin = ply_work->spd_dec_spin;

    if ( ply_work->no_spddown_timer ) {
        //--ply_work->no_spddown_timer;
        ply_work->obj_work.spd_slope  = 0;
		dec_spin = 0;
    }
	else{
		if (ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_SPIPE) {
			ply_work->obj_work.spd_slope  = g_gm_player_parameter[ply_work->char_id].spd_slope_spin;
		} else {
			ply_work->obj_work.spd_slope  = g_gm_player_parameter[ply_work->char_id].spd_slope_spin_spipe;
		}
        ply_work->obj_work.dir_slope = GMD_PL_KEI_DIR_SPIN;
    }

	/* 減速 */
    if ( ((ply_work->obj_work.spd_m > 0 && ply_work->key_on & PAD_KEY_RIGHT) ||
			(ply_work->obj_work.spd_m < 0 && ply_work->key_on & PAD_KEY_LEFT)) ) {
		// 進行方向キー押し時は減速値修正
		ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, (dec_spin >> 1));
	}
    else if ( !((ply_work->obj_work.spd_m > 0 && ply_work->key_on & PAD_KEY_RIGHT) ||
			(ply_work->obj_work.spd_m < 0 && ply_work->key_on & PAD_KEY_LEFT)) ) {
		// 逆進行方向押し時は微ブレーキ
		ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, (dec_spin << 1));
	}
    else {
        ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, dec_spin);
	}

#else
    if ( ply_work->no_spddown_timer ) {
        //--ply_work->no_spddown_timer;
        ply_work->obj_work.spd_slope  = 0;
    }
	else{
        ply_work->obj_work.spd_slope  = g_gm_player_parameter[ply_work->char_id].spd_slope_spin;
        ply_work->obj_work.dir_slope = GMD_PL_KEI_DIR_SPIN;
    }

	/* 減速 */
    if ( ((ply_work->obj_work.spd_m > 0 && ply_work->key_on & PAD_KEY_RIGHT) ||
			(ply_work->obj_work.spd_m < 0 && ply_work->key_on & PAD_KEY_LEFT)) ) {
		// 進行方向キー押し時は減速値修正
		ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, (ply_work->spd_dec_spin >> 1));
	}
    else if ( !((ply_work->obj_work.spd_m > 0 && ply_work->key_on & PAD_KEY_RIGHT) ||
			(ply_work->obj_work.spd_m < 0 && ply_work->key_on & PAD_KEY_LEFT)) ) {
		// 逆進行方向押し時は微ブレーキ
		ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, (ply_work->spd_dec_spin << 1));
	}
    else {
        ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, ply_work->spd_dec_spin);
	}
#endif
}

// ================================================================
// GmPlySeqMoveSpinNoDec
/*!
  プレイヤー移動設定関数 スピン減速無し移動

  @param ply_work [in] プレイヤーワークポインタ
    
 */
// ================================================================
void GmPlySeqMoveSpinNoDec(GMS_PLAYER_WORK *ply_work)
{
	fx32	dec_spin;

	dec_spin = ply_work->spd_dec_spin;

    if ( ply_work->no_spddown_timer ) {
        //--ply_work->no_spddown_timer;
        ply_work->obj_work.spd_slope  = 0;
		dec_spin = 0;
    }
	else{
		if (ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_SPIPE) {
			ply_work->obj_work.spd_slope  = g_gm_player_parameter[ply_work->char_id].spd_slope_spin;
		} else {
			ply_work->obj_work.spd_slope  = g_gm_player_parameter[ply_work->char_id].spd_slope_spin_spipe;
		}
        ply_work->obj_work.dir_slope = GMD_PL_KEI_DIR_SPIN;
    }

#if 0
	/* 減速 */
    if ( ((ply_work->obj_work.spd_m > 0 && ply_work->key_on & PAD_KEY_RIGHT) ||
			(ply_work->obj_work.spd_m < 0 && ply_work->key_on & PAD_KEY_LEFT)) ) {
		// 進行方向キー押し時は減速値修正
		ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, (dec_spin >> 1));
	}
    else if ( !((ply_work->obj_work.spd_m > 0 && ply_work->key_on & PAD_KEY_RIGHT) ||
			(ply_work->obj_work.spd_m < 0 && ply_work->key_on & PAD_KEY_LEFT)) ) {
		// 逆進行方向押し時は微ブレーキ
		ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, (dec_spin << 1));
	}
    else {
        ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, dec_spin);
	}
#endif
}

// ================================================================
// GmPlySeqMoveSpinPinball
/*!
  プレイヤー移動設定関数 ピンボールスピン時移動

  @param ply_work [in] プレイヤーワークポインタ
    
 */
// ================================================================
void GmPlySeqMoveSpinPinball(GMS_PLAYER_WORK *ply_work)
{
	fx32	fSpdAdd;// = 0;
	fx32	fSpdMax;// = 0;
	fx32	fSpdDec;// = 0;

	// 坂道速度, 角度設定
	ply_work->obj_work.spd_slope  = g_gm_player_parameter[ply_work->char_id].spd_slope_spin_pinball;
	ply_work->obj_work.dir_slope = GMD_PL_KEI_DIR_PINBALL_SPIN;

    fSpdAdd = ply_work->spd_add_spin_pinball;
    fSpdDec = ply_work->spd_dec_spin_pinball;
    fSpdMax = ply_work->spd_max_spin_pinball;

	// 傾斜角に合わせて最高速度設定
	{
		s32	roll = MTM_MATH_ABS(ply_work->key_walk_rot_z);

		if (roll > GMD_PL_DEF_ROLL_MAX) {
			roll = GMD_PL_DEF_ROLL_MAX;
		}


		fSpdMax = fSpdMax * roll / GMD_PL_DEF_ROLL_MAX;
	}
	if (fSpdMax < ply_work->prev_walk_roll_spd_max) {
		// 減速は緩やかに
		fSpdMax = ply_work->prev_walk_roll_spd_max - fSpdDec;
		if (fSpdMax < 0) {
			fSpdMax = 0;
		}
	}
	ply_work->prev_walk_roll_spd_max = fSpdMax;

    // 角度に合わせて最大速度調整
    if ( ply_work->obj_work.dir.z ) {
        fx32 fSpd = FX_Mul(ply_work->spd_max_add_slope_spin_pinball, mtMathSin(ply_work->obj_work.dir.z));
        if ( fSpd > 0 ) {
            fSpdMax += fSpd;
		}
    }
    
    if ( ply_work->no_spddown_timer ) {
        fSpdDec = 0;
    }
    // 速度に合わせて加速度修正
    else {
#if 0
        fx32 fPercent = 0;
        if ( MTM_MATH_ABS(ply_work->obj_work.spd_m ) > ply_work->spd3 ) {
			if (fSpdMax - ply_work->spd3) {
				fPercent = FX_Div(MTM_MATH_ABS(ply_work->obj_work.spd_m) - ply_work->spd3, fSpdMax - ply_work->spd3);
				if ( fPercent > FX32_ONE/*0x0100*/ ) {
					fPercent = FX32_ONE;
				}
				//fPercent = (s16)((fPercent * 0x00f8 ) >> 8);
			}
			else {
				fPercent = FX32_ONE;
			}
			// 倍率調整
			fPercent = (fPercent * 0x00f80) >> FX32_SHIFT;//◆どちらが早いかFX_Mul(fPercent, 0x00f80);
			//fPercent = (fPercent * gm_ply_seq_per_test) >> FX32_SHIFT;//◆どちらが早いかFX_Mul(fPercent, 0x00f80);
        }
        fSpdAdd = fSpdAdd - FX_Mul(fSpdAdd, fPercent);
        //fSpdAdd = (s16)(fSpdAdd - ((fSpdAdd * fPercent) >> 8));
#endif
    }

	// 水中対応
//    if ( ply_work->player_flag & GMD_PLF_WATER ) {
//        GMD_PLAYER_WATER_SET(fSpdAdd);
//        GMD_PLAYER_WATER_SET(fSpdDec);
//    }

    // 坂道瞬間最大速度アップ
    if ( ply_work->spd_work_max >= fSpdMax &&
			MTM_MATH_ABS(ply_work->obj_work.spd_m) >= fSpdMax ) {
        if ( ply_work->spd_work_max > ply_work->obj_work.spd_m ) {
            ply_work->spd_work_max = MTM_MATH_ABS(ply_work->obj_work.spd_m);
		}
        fSpdMax = ply_work->spd_work_max;
    }

    // 加速
	{
	// 通常
	    if ( GmPlayerKeyCheckWalkLeft(ply_work) | GmPlayerKeyCheckWalkRight(ply_work) ) {
	        if ( GmPlayerKeyCheckWalkRight(ply_work) ) {
			// 反転はシーケンス変更部で
	        //    if ( ply_work->act_state != GMD_PLY_STATE_TURN && !(ply_work->act_state >= GMD_PLY_STATE_BRAKE1 && ply_work->act_state <= GMD_PLY_STATE_BRAKED3) ) {
	        //        ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;	// 即時反転
			//	}

	            if ( ply_work->obj_work.spd_m < 0 ) {
	                ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
				}
	            ply_work->obj_work.spd_m = ObjSpdUpSet( ply_work->obj_work.spd_m, fSpdAdd, fSpdMax);
	        }
			else {
			// 反転はシーケンス変更部で
	        //    if ( ply_work->act_state != GMD_PLY_STATE_TURN && !(ply_work->act_state >= GMD_PLY_STATE_BRAKE1 && ply_work->act_state <= GMD_PLY_STATE_BRAKED3) ) {
	        //        ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;		// 即時反転
			//	}
	            if ( ply_work->obj_work.spd_m > 0 ) {
	                ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
				}
	            ply_work->obj_work.spd_m = ObjSpdUpSet( ply_work->obj_work.spd_m,-fSpdAdd, fSpdMax);
	        }
	    }
		else {
	        ply_work->spd_pool = 0;
	        ply_work->obj_work.spd.x = MTM_MATH_CLIP( ply_work->obj_work.spd.x, -fSpdMax, fSpdMax );
	        ply_work->obj_work.spd_m = MTM_MATH_CLIP( ply_work->obj_work.spd_m, -fSpdMax, fSpdMax );
	        if ( (((ply_work->obj_work.dir.z + 0x2000) & 0xff00) <= ( 0x2000 << 1)) ) {
	            // 減速
	            if ( ply_work->player_flag & GMD_PLF_NOBRAKE ){
	                // このフレームは減速できません
	                ply_work->player_flag &= ~GMD_PLF_NOBRAKE;
	                return;
	            }
				// 減速
				ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
	        }
	    }
    }
}



// ================================================================
// gmPlySeqTruckMove
/*!
 *	トロッコ実移動処理
 *
 *	@param obj_work [in] オブジェクトワーク
 *
 *	@note
 *		ObjObjectMove の代わりに呼び出す
 */
// ================================================================
void gmPlySeqTruckMove(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;
    fx32 spd_x = 0, spd_y = 0, spd_z = 0;
    fx32 flow_x;
    fx32 flow_y;
	fx32 spd_slope;
//	fx32 spd_fall, spd_fall_y;

	u16	now_dir;//, fall_dir;

	ply_work = (GMS_PLAYER_WORK*)obj_work;

	// 重力角度と画面角度を加味した現在の角度
#if 1
	now_dir = (u16)(obj_work->dir.z + (obj_work->dir_fall - g_gm_main_system.pseudofall_dir));
#else
	if (obj_work->move_flag & OBD_MOVE_UNDER) {
		now_dir = (u16)(obj_work->dir.z + (obj_work->dir_fall - g_gm_main_system.pseudofall_dir));
	}
	else {
		now_dir = obj_work->dir.z;
	}
#endif

    // 前座標保持
    obj_work->prev_pos.x = obj_work->pos.x;
    obj_work->prev_pos.y = obj_work->pos.y;
    obj_work->prev_pos.z = obj_work->pos.z;

    // sFlowの影響を受けないフラグをチェック
    if ( obj_work->move_flag & OBD_MOVE_NOFLOW ){
        obj_work->flow.x = 0;
        obj_work->flow.y = 0;
        obj_work->flow.z = 0;
    }
    flow_x = obj_work->flow.x;
    flow_y = obj_work->flow.y;
    
    if ( flow_x || flow_y ){
        if ( obj_work->dir_fall )
            ObjObjectSpdDirFall( &flow_x, &flow_y, obj_work->dir_fall );
    }
    
    if ( obj_work->hitstop_timer ){
        // HIT STOP時移動処理
        obj_work->move.x = FX_Mul( flow_x        , g_obj.speed);
        obj_work->move.y = FX_Mul( flow_y        , g_obj.speed);
        obj_work->move.z = FX_Mul( obj_work->flow.z, g_obj.speed);
        
    }else{
        //　通常移動処理
        if (!( obj_work->move_flag & OBD_MOVE_UNDER )){
            // 落下加速
            if ( obj_work->move_flag & OBD_MOVE_FALL && !(obj_work->move_flag & OBD_MOVE_UNDER)){
                obj_work->spd.y += FX_Mul(obj_work->spd_fall, g_obj.speed);
            }
            if ( obj_work->move_flag & OBD_MOVE_FALL ){
                // 落下速度オーバーチェック
                if ( obj_work->spd.y >  obj_work->spd_fall_max )
                    obj_work->spd.y = obj_work->spd_fall_max;
            }
        }

        // マスタースピードを角度に合わせて分解
        if( obj_work->move_flag & OBD_MOVE_DIR ){
            if ( obj_work->move_flag & OBD_MOVE_SLOPE && (obj_work->spd_m || !(obj_work->move_flag & OBD_MOVE_SLOPE_STICK )) ){
                // 角度チェック
				// 20090817 重力反映
			//	if ( (((obj_work->dir.z + obj_work->dir_slope) & 0xffff) >= ( obj_work->dir_slope << 1)) ) {
			//			obj_work->spd_m = ObjSpdUpSet( obj_work->spd_m,
			//										FX_Mul(obj_work->spd_slope, mtMathSin((u16)(obj_work->dir.z))),
			//										obj_work->spd_slope_max);
            //  }
				if ( (((now_dir + obj_work->dir_slope) & 0xffff) >= ( obj_work->dir_slope << 1)) ) {
					fx32	slope_acc = 0;
#if 1
					if (MTM_MATH_ABS(obj_work->spd_m) < 0x4000) {
#else
					if (MTM_MATH_ABS(obj_work->spd_m) < 0x2000) {
						// 速度が低い間はspd_slope反映量を減らす
						spd_slope = obj_work->spd_slope >> 1;
					}
					else if (MTM_MATH_ABS(obj_work->spd_m) < 0x4000) {
#endif
						// 通常
						spd_slope = obj_work->spd_slope;
					}
					else {
						// 一定以上
						spd_slope = obj_work->spd_slope << 1;
					}

					if ((obj_work->spd_m > 0 && now_dir > 0x8000) ||
								(obj_work->spd_m < 0 && now_dir < 0x8000)) {
						// 移動方向に対して上り坂
						// 減速率アップ
						spd_slope <<= 1;
					}

#if 1
					slope_acc = FX_Mul(spd_slope, mtMathSin((u16)(now_dir)));

					if (slope_acc > 0 || (now_dir < 0x8000)) {
						if (slope_acc < 0x100) {
							slope_acc = 0x100;
						}
					}
					else {
						if (slope_acc > -0x100) {
							slope_acc = -0x100;
						}
					}

					obj_work->spd_m = ObjSpdUpSet(obj_work->spd_m,
												slope_acc,
												obj_work->spd_slope_max);
#else
					obj_work->spd_m = ObjSpdUpSet(obj_work->spd_m,
												FX_Mul(spd_slope, mtMathSin((u16)(now_dir))),
												obj_work->spd_slope_max);
#endif
                }


            }
            if( !(obj_work->move_flag & OBD_MOVE_NOSPDM) ){
                spd_x = FX_Mul((obj_work->spd_m ), mtMathCos( obj_work->dir.z ));
                spd_y = FX_Mul((obj_work->spd_m ), mtMathSin( obj_work->dir.z ));
            }
        }

        if ( obj_work->move_flag & OBD_MOVE_NO_AUTO_SCROLL ){
            obj_work->move.x = FX_Mul((obj_work->spd.x + spd_x + flow_x ), g_obj.speed);
            obj_work->move.y = FX_Mul((obj_work->spd.y + spd_y + flow_y ), g_obj.speed);
        }else{
            obj_work->move.x = FX_Mul((obj_work->spd.x + spd_x + flow_x + g_obj.scroll[0]), g_obj.speed);
            obj_work->move.y = FX_Mul((obj_work->spd.y + spd_y + flow_y + g_obj.scroll[1]), g_obj.speed);
        }
        obj_work->move.z = FX_Mul((obj_work->spd.z + spd_z + obj_work->flow.z), g_obj.speed);

		// 重力チェックあり
		if (obj_work->move_flag & OBD_MOVE_UNDER) {
			// 接地時は地面の角度に対応
			ObjObjectSpdDirFall(&obj_work->move.x, &obj_work->move.y, obj_work->dir_fall);
		}
		else {
			// 空中の時は指定角度(jump_pseudofall_dir)に対応
			ObjObjectSpdDirFall(&obj_work->move.x, &obj_work->move.y, ply_work->jump_pseudofall_dir);
		}
    }
    // 実移動
    obj_work->pos.x += obj_work->move.x;
    obj_work->pos.y += obj_work->move.y;
    obj_work->pos.z += obj_work->move.z;
    
    // 加速
    obj_work->spd.x += obj_work->spd_add.x;
    obj_work->spd.y += obj_work->spd_add.y;
    obj_work->spd.z += obj_work->spd_add.z;

	// flowクリア
	obj_work->flow.x = 0;
	obj_work->flow.y = 0;
	obj_work->flow.z = 0;
}

// ================================================================
// gmPlySeqSplMove
/*!
 *	スペステ実移動処理
 *
 *	@param obj_work [in] オブジェクトワーク
 *
 *	@note
 *		ObjObjectMove の代わりに呼び出す
 */
// ================================================================
void gmPlySeqSplMove(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;
    fx32 spd_x = 0, spd_y = 0, spd_z = 0;
    fx32 flow_x;
    fx32 flow_y;

	u16	now_dir;

	ply_work = (GMS_PLAYER_WORK*)obj_work;

	// 重力角度と画面角度を加味した現在の角度
	now_dir = (u16)(obj_work->dir.z + (obj_work->dir_fall - g_gm_main_system.pseudofall_dir));

    // 前座標保持
    obj_work->prev_pos.x = obj_work->pos.x;
    obj_work->prev_pos.y = obj_work->pos.y;
    obj_work->prev_pos.z = obj_work->pos.z;

    // sFlowの影響を受けないフラグをチェック
    if ( obj_work->move_flag & OBD_MOVE_NOFLOW ){
        obj_work->flow.x = 0;
        obj_work->flow.y = 0;
        obj_work->flow.z = 0;
    }
    flow_x = obj_work->flow.x;
    flow_y = obj_work->flow.y;
    
    if ( flow_x || flow_y ){
        if ( obj_work->dir_fall )
            ObjObjectSpdDirFall( &flow_x, &flow_y, obj_work->dir_fall );
    }
    
    if ( obj_work->hitstop_timer ){
        // HIT STOP時移動処理
        obj_work->move.x = FX_Mul( flow_x        , g_obj.speed);
        obj_work->move.y = FX_Mul( flow_y        , g_obj.speed);
        obj_work->move.z = FX_Mul( obj_work->flow.z, g_obj.speed);
        
    }else{
        //　通常移動処理
        if (!( obj_work->move_flag & OBD_MOVE_UNDER )){
            // 落下加速
            if ( obj_work->move_flag & OBD_MOVE_FALL && !(obj_work->move_flag & OBD_MOVE_UNDER)){
                obj_work->spd.y += FX_Mul(obj_work->spd_fall, g_obj.speed);
            }
            if ( obj_work->move_flag & OBD_MOVE_FALL ){
                // 落下速度オーバーチェック
                if ( obj_work->spd.y >  obj_work->spd_fall_max )
                    obj_work->spd.y = obj_work->spd_fall_max;
            }
        }

        // マスタースピードを角度に合わせて分解
        if( obj_work->move_flag & OBD_MOVE_DIR ){
            if ( obj_work->move_flag & OBD_MOVE_SLOPE && (obj_work->spd_m || !(obj_work->move_flag & OBD_MOVE_SLOPE_STICK )) ){
                // 角度チェック
				// 20090817 重力反映
			//	if ( (((obj_work->dir.z + obj_work->dir_slope) & 0xffff) >= ( obj_work->dir_slope << 1)) ) {
			//			obj_work->spd_m = ObjSpdUpSet( obj_work->spd_m,
			//										FX_Mul(obj_work->spd_slope, mtMathSin((u16)(obj_work->dir.z))),
			//										obj_work->spd_slope_max);
            //  }

// 空中でもslopeの影響を受けてしまうので変更
//				if ( (((now_dir + obj_work->dir_slope) & 0xffff) >= ( obj_work->dir_slope << 1)) ) {
// ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
				if ( (((now_dir + obj_work->dir_slope) & 0xffff) >= ( obj_work->dir_slope << 1)) 
					&&(obj_work->move_flag & OBD_MOVE_UNDER)) {
// ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
					if (MTM_MATH_ABS(obj_work->spd_m) < 0x2000) {
						// 速度が低い間はspd_slope反映量を減らす
						obj_work->spd_m = ObjSpdUpSet( obj_work->spd_m,
													FX_Mul(obj_work->spd_slope >> 1, mtMathSin((u16)(now_dir))),
													obj_work->spd_slope_max);
					}
					else if (MTM_MATH_ABS(obj_work->spd_m) < 0x4000) {
						// 通常
						obj_work->spd_m = ObjSpdUpSet( obj_work->spd_m,
													FX_Mul(obj_work->spd_slope, mtMathSin((u16)(now_dir))),
													obj_work->spd_slope_max);
					}
					else {
						// 一定以上
						obj_work->spd_m = ObjSpdUpSet( obj_work->spd_m,
													FX_Mul(obj_work->spd_slope << 1, mtMathSin((u16)(now_dir))),
													obj_work->spd_slope_max);
					}
                }
// ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
                else {
					obj_work->spd_m = 0;
				}
// ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑


            }
            if( !(obj_work->move_flag & OBD_MOVE_NOSPDM) ){
                spd_x = FX_Mul((obj_work->spd_m ), mtMathCos( obj_work->dir.z ));
                spd_y = FX_Mul((obj_work->spd_m ), mtMathSin( obj_work->dir.z ));
                //sSpdX = FX_Mul((pWork->spd_m ), mtMathCos( pWork->dir.z ));
               // sSpdY = FX_Mul((pWork->spd_m ), mtMathSin( pWork->dir.z ));
            }
        }

        if ( obj_work->move_flag & OBD_MOVE_NO_AUTO_SCROLL ){
            obj_work->move.x = FX_Mul((obj_work->spd.x + spd_x + flow_x ), g_obj.speed);
            obj_work->move.y = FX_Mul((obj_work->spd.y + spd_y + flow_y ), g_obj.speed);
        }else{
            obj_work->move.x = FX_Mul((obj_work->spd.x + spd_x + flow_x + g_obj.scroll[0]), g_obj.speed);
            obj_work->move.y = FX_Mul((obj_work->spd.y + spd_y + flow_y + g_obj.scroll[1]), g_obj.speed);
        }
        obj_work->move.z = FX_Mul((obj_work->spd.z + spd_z + obj_work->flow.z), g_obj.speed);

		// 重力チェックあり
		if (obj_work->move_flag & OBD_MOVE_UNDER) {
			// 接地時は地面の角度に対応
			ObjObjectSpdDirFall(&obj_work->move.x, &obj_work->move.y, obj_work->dir_fall);
		}
		else {
			// 空中の時は画面の角度に対応
			static s32 test = 0;
			if (test) {
			ObjObjectSpdDirFall(&obj_work->move.x, &obj_work->move.y, g_gm_main_system.pseudofall_dir);
			}
			else {
			// 空中の時は指定角度(jump_pseudofall_dir)に対応
			ObjObjectSpdDirFall(&obj_work->move.x, &obj_work->move.y, ply_work->jump_pseudofall_dir);
			}
		}

        

    }
    // 実移動
    obj_work->pos.x += obj_work->move.x;
    obj_work->pos.y += obj_work->move.y;
    obj_work->pos.z += obj_work->move.z;
    
    // 加速
    obj_work->spd.x += obj_work->spd_add.x;
    obj_work->spd.y += obj_work->spd_add.y;
    obj_work->spd.z += obj_work->spd_add.z;

	// flowクリア
	obj_work->flow.x = 0;
	obj_work->flow.y = 0;
	obj_work->flow.z = 0;
}

// =====================================================================
// GmPlySeqSplJumpDirec
/*!
  スペステジャンプ中の角度変化

  @param ply_work [io] プレイヤーワークポインタ
    
 */
// =====================================================================
void gmPlySeqSplJumpDirec(GMS_PLAYER_WORK *ply_work)
{
	// Z角度復帰
	ply_work->obj_work.dir.z =
			//ObjRoopMove16(ply_work->obj_work.dir.z, g_gm_main_system.pseudofall_dir, 0x200);
			ObjRoopMove16(ply_work->obj_work.dir.z, (u16)(ply_work->jump_pseudofall_dir - ply_work->obj_work.dir_fall), 0x200);


	if (!(ply_work->gmk_flag & (GMD_PLGF_BELT | GMD_PLGF_GMK_WALL | GMD_PLGF_GMK_HOLD_POS_Z))) {
		// Z座標復帰
		ply_work->obj_work.pos.z = (fx32)(ObjSpdDownSet(ply_work->obj_work.pos.z, 0x00004000) & 0xFFFFF000);	// 小数点部切り落とし◆
		ply_work->obj_work.spd.z = ObjSpdDownSet( ply_work->obj_work.spd.z, 0x00200 );

		// X角度復帰
		ply_work->obj_work.dir.x = ObjRoopMove16(ply_work->obj_work.dir.x, 0, 0x400);
	}
}

#if 0
// ================================================================
// GmPlySeqMoveFly
/*!
  プレイヤー移動設定関数

  @param ply_work [in] プレイヤーワークポインタ
    
 */
// ================================================================
void GmPlySeqMoveFly(GMS_PLAYER_WORK *ply_work)
{
    if ( ply_work->key_on & (PAD_KEY_LEFT | PAD_KEY_RIGHT) ) {
        if ( ply_work->key_on & PAD_KEY_RIGHT ) {
            ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
            
            if ( ply_work->obj_work.spd.x < 0 ) {
                ply_work->obj_work.spd.x = ObjSpdDownSet( ply_work->obj_work.spd.x, 0x02000 );
            }
            ply_work->obj_work.spd.x = ObjSpdUpSet( ply_work->obj_work.spd.x, 0x00800, 0x1f000);
        }
		else {
            if ( ply_work->obj_work.spd.x > 0 ){
                ply_work->obj_work.spd.x = ObjSpdDownSet( ply_work->obj_work.spd.x, 0x02000 );
            }
            ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
            ply_work->obj_work.spd.x = ObjSpdUpSet( ply_work->obj_work.spd.x, -0x00800, 0x1f000);
        }
    }
	else {
        ply_work->obj_work.spd.x = ObjSpdDownSet( ply_work->obj_work.spd.x, 0x02000 );
    }

    if ( ply_work->key_on & (PAD_KEY_UP | PAD_KEY_DOWN) /*&&
         ((  (ply_work->obj_work.mAct.flag & MTD_ACT_FLAG_FLIP_H && ply_work->obj_work.spd.x < 0) ||
         ( !(ply_work->obj_work.mAct.flag & MTD_ACT_FLAG_FLIP_H) && ply_work->obj_work.spd.x > 0) )*/){
        if ( ply_work->key_on & PAD_KEY_DOWN ) {
            if ( ply_work->obj_work.spd.y < 0 ){
                ply_work->obj_work.spd.y = ObjSpdDownSet( ply_work->obj_work.spd.y, 0x02000 );
            }
            ply_work->obj_work.spd.y = ObjSpdUpSet( ply_work->obj_work.spd.y, 0x00800, 0x0f000);
        }
		else{
            if ( ply_work->obj_work.spd.y > 0 ) {
                ply_work->obj_work.spd.y = ObjSpdDownSet( ply_work->obj_work.spd.y, 0x02000 );
            }
            ply_work->obj_work.spd.y = ObjSpdUpSet( ply_work->obj_work.spd.y, -0x00800, 0x0f000);
        }
    }
	else{
        ply_work->obj_work.spd.y = ObjSpdDownSet( ply_work->obj_work.spd.y, 0x02000 );
    }    
}
#endif

#if 0
// ================================================================
// GmPlySeqMoveAuto
/*!
  プレイヤー移動設定関数

  @param ply_work [in] プレイヤーワークポインタ
    
 */
// ================================================================
void GmPlySeqMoveAuto(GMS_PLAYER_WORK *ply_work)
{
    fx32 fSpdAdd;// = 0;
    fx32 fSpdMax;// = 0;
    fx32 fSpdDec;// = 0;
    
    ply_work->spd_work_max = 0;

    fSpdAdd       = g_gm_player_parameter[ply_work->char_no].spd_add;  // 通常 加速値
    fSpdDec       = g_gm_player_parameter[ply_work->char_no].spd_dec;  // 通常放置 減速値
    fSpdMax       = g_gm_player_parameter[ply_work->char_no].spd_max;  // 通常 最大速度値
    
    if ( ply_work->player_flag & GMD_PLF_BOOST ){
        fSpdMax = g_gm_player_parameter[ply_work->char_no].spd_max_boost; // ニトロ 加速値
    }
    if ( ply_work->player_flag & GMD_PLF_NITRO ){
        fSpdAdd = g_gm_player_parameter[ply_work->char_no].spd_add_nitro; // ニトロ 加速値
        fSpdMax = g_gm_player_parameter[ply_work->char_no].spd_max_nitro; // ニトロ 最大速度値
        fSpdDec = g_gm_player_parameter[ply_work->char_no].spd_dec_nitro; // ニトロ放置 減速値
    }

    // ブーストチェック
    gmPlayerMoveBoostPoolCheck(ply_work);
    
    // 角度に合わせて最大速度調整
    if ( ply_work->obj_work.dir.z ){
        fx32 fSpd = FX_Mul(ply_work->spd_max_add_slope, mtMathSin(ply_work->obj_work.dir.z));
        if ( fSpd > 0 ) {
            fSpdMax += fSpd;
		}
    }

    if ( ply_work->no_spddown_timer ) {
        fSpdDec = 0;
    }
    // 速度に合わせて加速度修正
    else {
        fx32 fPercent = 0;
        if ( MTM_MATH_ABS(ply_work->obj_work.spd_m ) > ply_work->spd2 ) {
            fPercent = FX_Div(MTM_MATH_ABS(ply_work->obj_work.spd_m) - ply_work->spd2, fSpdMax - ply_work->spd2);
            if ( fPercent > FX32_ONE/*0x0100*/ ) {
                fPercent = FX32_ONE;
			}
            // 倍率調整
            fPercent = (fPercent * 0x00f80) >> FX32_SHIFT;//◆どちらが早いかFX_Mul(fPercent, 0x00f80);
            //fPercent = (s16)((fPercent * 0x00f8 ) >> 8);
        }
        fSpdAdd = fSpdAdd - FX_Mul(fSpdAdd, fPercent);
        //fSpdAdd = (s16)(fSpdAdd - ((fSpdAdd * fPercent) >> 8));
    }
    
    if ( ply_work->player_flag & GMD_PLF_WATER ) {
        GMD_PLAYER_WATER_SET(fSpdAdd);
        GMD_PLAYER_WATER_SET(fSpdDec);
    }
    // 加速
    if ( !(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) ) {
        if ( ply_work->obj_work.spd_m < 0 ) {
            ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
		}
        ply_work->obj_work.spd_m = ObjSpdUpSet( ply_work->obj_work.spd_m, fSpdAdd, fSpdMax);
    }
	else{
        if ( ply_work->obj_work.spd_m > 0 ) {
            ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, fSpdDec);
		}
        ply_work->obj_work.spd_m = ObjSpdUpSet( ply_work->obj_work.spd_m,-fSpdAdd, fSpdMax);
    }

}
#endif


// =====================================================================
// 移動ユーティリティ
// =====================================================================
// =====================================================================
// GmPlySeqJumpDirec
/*!
  プレイヤージャンプ中の角度変化

  @param ply_work [io] プレイヤーワークポインタ
    
 */
// =====================================================================
void GmPlySeqJumpDirec(GMS_PLAYER_WORK *ply_work)
{
    //s32 lDir = (s32)((s8)ply_work->obj_work.dir.z);
    /*
    if (lDir < 0) {
        lDir += 2;
        if (lDir > 0) lDir = 0;
    } else if (lDir > 0) {
        lDir -= 2;
        if (lDir < 0) lDir = 0;
    }
     */
	// Z角度復帰
    ply_work->obj_work.dir.z = ObjRoopMove16( ply_work->obj_work.dir.z, 0, 0x200 );

    // ply_work->obj_work.dir.z = (u8)(lDir & 0xff);

    if ( !(ply_work->gmk_flag & (GMD_PLGF_BELT | GMD_PLGF_GMK_WALL | GMD_PLGF_GMK_HOLD_POS_Z)) ){
 	   // Z座標復帰
		ply_work->obj_work.pos.z = (fx32)(ObjSpdDownSet(ply_work->obj_work.pos.z, 0x00004000) & 0xFFFFF000);	// 小数点部切り落とし◆
        ply_work->obj_work.spd.z = ObjSpdDownSet( ply_work->obj_work.spd.z, 0x00200 );

		// X角度復帰
        ply_work->obj_work.dir.x = ObjRoopMove16( ply_work->obj_work.dir.x, 0, 0x400 );
    }

#if 0
	// スケール復帰
	if (!(ply_work->gmk_flag & GMD_PLGF_GMK_USE_SCALE)) {	// スケール演出使用中で無い場合
		fx32	scale_diff;

		if (ply_work->obj_work.scale.x != FX32_ONE) {
			scale_diff = ply_work->obj_work.scale.x - FX32_ONE;
			ply_work->obj_work.scale.x -= scale_diff - ObjSpdDownSet(scale_diff, 0x0080);
		}
		if (ply_work->obj_work.scale.y != FX32_ONE) {
			scale_diff = ply_work->obj_work.scale.y - FX32_ONE;
			ply_work->obj_work.scale.y -= scale_diff - ObjSpdDownSet(scale_diff, 0x0080);
		}
		if (ply_work->obj_work.scale.z != FX32_ONE) {
			scale_diff = ply_work->obj_work.scale.z - FX32_ONE;
			ply_work->obj_work.scale.z -= scale_diff - ObjSpdDownSet(scale_diff, 0x0080);
		}
	}
#endif
}

// =====================================================================
// GmPlySeqTruckJumpDirec
/*!
  プレイヤートロッコジャンプ中の角度変化

  @param ply_work [io] プレイヤーワークポインタ
    
 */
// =====================================================================
void GmPlySeqTruckJumpDirec(GMS_PLAYER_WORK *ply_work)
{
#if 1 // ◎トロッコ新仕様対応
	// Z角度復帰
	ply_work->obj_work.dir.z =
			//ObjRoopMove16(ply_work->obj_work.dir.z, g_gm_main_system.pseudofall_dir, 0x200);
			ObjRoopMove16(ply_work->obj_work.dir.z, (u16)(ply_work->jump_pseudofall_dir - ply_work->obj_work.dir_fall), 0x200);

#else
	// Z角度復帰
	ply_work->obj_work.dir.z =
			//ObjRoopMove16(ply_work->obj_work.dir.z, g_gm_main_system.pseudofall_dir, 0x200);
			ObjRoopMove16(ply_work->obj_work.dir.z, g_gm_main_system.pseudofall_dir - ply_work->obj_work.dir_fall, 0x200);
#endif

	if (!(ply_work->gmk_flag & (GMD_PLGF_BELT | GMD_PLGF_GMK_WALL | GMD_PLGF_GMK_HOLD_POS_Z))) {
		// Z座標復帰
		ply_work->obj_work.pos.z = (fx32)(ObjSpdDownSet(ply_work->obj_work.pos.z, 0x00004000) & 0xFFFFF000);	// 小数点部切り落とし◆
		ply_work->obj_work.spd.z = ObjSpdDownSet( ply_work->obj_work.spd.z, 0x00200 );

		// X角度復帰
		ply_work->obj_work.dir.x = ObjRoopMove16(ply_work->obj_work.dir.x, 0, 0x400);
	}
}

// ==========================================================================
//
// ==========================================================================
// ==========================================================================
// GmPlySeqCheckAcceptHoming
/*!
  プレイヤー ホーミング可能チェック

  @param ply_work [io] プレイヤーワークポインタ
    
 */
// ==========================================================================
BOOL GmPlySeqCheckAcceptHoming(GMS_PLAYER_WORK *ply_work)
{
//	const GMS_PLY_SEQ_STATE_DATA	*seq_state_data;
//	seq_state_data	= g_gm_ply_seq_state_data_tbl;
	
	// ホーミング可能チェック
	if ((ply_work->seq_state_data_tbl[ply_work->seq_state].accept_attr &
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING) &&
			!(ply_work->player_flag & GMD_PLF_NOHOMING)) {
		return (TRUE);
	}
	return (FALSE);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmPlySeqCheckChangeSequence
/*!
 *	シーケンス変更判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqCheckChangeSequence(GMS_PLAYER_WORK *ply_work)
{
	OBS_OBJECT_WORK	*obj_work;

	obj_work = (OBS_OBJECT_WORK*)ply_work;


	/* ゲームシステムからの分岐命令 */

	/* スーパーソニック変身チェック */
	if (GmPlayerIsTransformSuperSonic(ply_work)) {
		if (GmPlayerKeyCheckTransformKeyPush(ply_work) &&
				GME_PLY_SEQ_STATE_FW <= ply_work->seq_state &&
				ply_work->seq_state <= GME_PLY_SEQ_STATE_JUMPDASH) {
			 // スーパーソニック変身
			 GmPlySeqChangeTransformSuper(ply_work);
		}
	}


	// 分岐判定
	/* 以下、分岐確定時には それ以下の分岐・終了判定は行わない */

	/* 特殊分岐 */

	/* ユーザー入力分岐 */
	if (gmPlySeqCheckChangeSequenceUserInput(ply_work)) {
		return;
	}


	/* 終了判定 */


	/* 以下分岐の可能性がある終了判定 */


}

// ==========================================================================
// gmPlySeqCheckChangeSequenceUserInput
/*!
 * シーケンスチェンジ判定部
 * ユーザーの入力によるシーケンスチェンジのみを抽出
 *
 *	@param	ply_work	[in]	リンクアクション管理ワーク
 */
// ==========================================================================
BOOL gmPlySeqCheckChangeSequenceUserInput(GMS_PLAYER_WORK *ply_work)
{
	OBS_OBJECT_WORK					*obj_work;
	const GMS_PLY_SEQ_STATE_DATA	*seq_state_data;

	obj_work		= (OBS_OBJECT_WORK*)ply_work;
	seq_state_data	= ply_work->seq_state_data_tbl;

	/* プレイヤー状態変化判定 */
	// 終了判定
	// スピンダッシュ終了チェック
//	if (seq_state_data[ply_work->seq_state].check_attr &
//			GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPINDASH) {
//		/* スピンダッシュ終了判定 */
//		gmPlySeqCheckEndSpinDash(ply_work);
//	}
	// 歩き終了チェック
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_WALK_END) {
		/* 歩き終了判定 */
		if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
			gmPlySeqCheckEndTruckWalk(ply_work);
		}
		else {
			gmPlySeqCheckEndWalk(ply_work);
		}
	}
	// 振り向き可能判定
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_TURN) {
		/* 振り向き判定 */
		gmPlySeqCheckTurn(ply_work);
	}
	// 歩き中振り向き可能判定
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN) {
		/* 振り向き判定 */
		gmPlySeqCheckDirectTurn(ply_work);
	}
	// 落下可能判定
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL) {
		/* 落下判定 */
		gmPlySeqCheckFall(ply_work);
	}
	// よろけ可能判定
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_STAGGER) {
		/* よろけ判定 */
		gmPlySeqCheckStagger(ply_work);
	}
	// 見上げ終了チェック
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_END_LOOKUP) {
		/* 見上げ終了判定 */
		gmPlySeqCheckEndLookup(ply_work);
	}
	// しゃがみ終了チェック
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SQUAT) {
		/* しゃがみ終了判定 */
		gmPlySeqCheckEndSquat(ply_work);
	}
#if 0
	// ブレーキ終了チェック
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_END_BRAKE) {
		/* ブレーキ終了判定 */
		gmPlySeqCheckEndBrake(ply_work);
	}
#endif
	// スピン終了チェック
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPIN) {
		/* スピン終了判定 */
		gmPlySeqCheckEndSpin(ply_work);
	}
	// 壁押し終了チェック
	if (seq_state_data[ply_work->seq_state].check_attr &
			GMD_PLY_SEQ_STATE_CHECK_ATTR_END_PUSH) {
		/* 壁押し終了判定 */
		gmPlySeqCheckEndWallPush(ply_work);
	}

	/* ここまでは状態変化が起こってもチェックを終了しない */

#if 0
	/* 勝敗決着判定時終了分岐を行わない判定 */
	if (ply_work->lact_data->flag0 & GMD_LACT_FLAG0_GAME_END_NO_BRANCH) {
		if (ply_work->obj_work->obj_type == GMD_OBJTYPE_PLAYER) {
			GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)ply_work->obj_work;
			if (ply_work->life <= 0) {
				// 死亡中
				return (FALSE);
			}
		}
	}
#endif

#if 0
	// リカバリー可能判定
	if (seq_state_data[ply_work->seq_state].accept_attr & 
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_RECOVERY) {
		/* リカバリー判定 */
		if (gmPlySeqRecovery(ply_work)) {
			/* リカバリー確定 */
			return (TRUE);
		}
	}
#endif

	// ホーミング可能判定
#if 1
	if (GmPlySeqCheckAcceptHoming(ply_work)) {
#else
	if (seq_state_data[ply_work->seq_state].accept_attr &
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING) {
#endif
		// ターゲットカーソル呼び出し
		if (ply_work->cursol_enemy_obj == NULL) {
			GmPlyEfctCreateHomingCursol(ply_work);
			ply_work->cursol_enemy_obj = ply_work->enemy_obj;
		}

		if (gmPlySeqCheckHoming(ply_work)) {
			// ホーミング確定
			return (TRUE);
		}
	}
//	// ジャンプダッシュ可能判定
//	if (seq_state_data[ply_work->seq_state].accept_attr &
//			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMPDASH) {
//		if (gmPlySeqCheckJumpDash(ply_work)) {
//			// ジャンプダッシュ確定
//			return (TRUE);
//		}
//	}
	// しゃがみ入力時スピン可能判定
	if (seq_state_data[ply_work->seq_state].accept_attr &
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT_SPIN) {
		if (gmPlySeqCheckSquatSpin(ply_work)) {
			// スピン確定
			return (TRUE);
		}
	}
	// スピン可能判定
	if (seq_state_data[ply_work->seq_state].accept_attr &
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPIN) {
		if (gmPlySeqCheckSpin(ply_work)) {
			// スピン確定
			return (TRUE);
		}
	}
	// スピンダッシュ加速可能判定
	if (seq_state_data[ply_work->seq_state].accept_attr &
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPINACC) {
		if (gmPlySeqCheckSpinDashAcc(ply_work)) {
			// スピンダッシュ確定
			return (TRUE);
		}
	}
	// ピンボール時スピンダッシュ加速可能判定
	if (seq_state_data[ply_work->seq_state].accept_attr &
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_PINBALL_SPINACC) {
		if (gmPlySeqCheckPinballSpinDashAcc(ply_work)) {
			// スピンダッシュ確定
			return (TRUE);
		}
	}


	/* 以下 反応優先順で並べる事 */

#if 0
	// すり抜け落下判定
	if (seq_state_data[ply_work->seq_state].accept_attr &
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_THROUGH) {
		if (gmPlySeqThroughFall(ply_work)) {
			// すり抜け落下確定
			return (TRUE);
		}
	}
#endif

	// ジャンプ可能判定
	if (seq_state_data[ply_work->seq_state].accept_attr &
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP) {
		// ジャンプ判定
		if (gmPlySeqCheckJump(ply_work)) {
			// ジャンプ確定
			return (TRUE);
		}
	}
	// ブレーキ可能判定
	if (seq_state_data[ply_work->seq_state].accept_attr &
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_BRAKE) {
		// ブレーキ判定
		if (gmPlySeqCheckBrake(ply_work)) {
			// ブレーキ確定
			return (TRUE);
		}
	}
	// 壁押し可能判定
	if (seq_state_data[ply_work->seq_state].accept_attr &
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALLPUSH) {
		// 壁押し判定
		if (gmPlySeqCheckWallPush(ply_work)) {
			// 壁押し確定
			return (TRUE);
		}
	}
	// 歩き可能判定
	if (seq_state_data[ply_work->seq_state].accept_attr &
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK) {
		// 歩き判定
		if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
			if (gmPlySeqCheckTruckWalk(ply_work)) {
				// 歩き確定
				return (TRUE);
			}
		}
		else {
			if (gmPlySeqCheckWalk(ply_work)) {
				// 歩き確定
				return (TRUE);
			}
		}
	}
	// 見上げ可能判定
	if (seq_state_data[ply_work->seq_state].accept_attr &
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_LOOKUP) {
		// 見上げ判定
		if (gmPlySeqCheckLookup(ply_work)) {
			// 見上げ確定
			return (TRUE);
		}
	}
	// しゃがみ可能判定
	if (seq_state_data[ply_work->seq_state].accept_attr &
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT) {
		// しゃがみ判定
		if (gmPlySeqCheckSquat(ply_work)) {
			// 歩き確定
			return (TRUE);
		}
	}

	return (FALSE);
}


// ==========================================================================
// シーケンス変更チェック
// ==========================================================================
// ==========================================================================
// gmPlySeqCheckEndSpinDash
/*!
 *	スピンダッシュ判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
//BOOL gmPlySeqCheckEndSpinDash(GMS_PLAYER_WORK *ply_work)
//{
//	return (FALSE);
//}

// ==========================================================================
// gmPlySeqCheckEndWalk
/*!
 *	歩き終了判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckEndWalk(GMS_PLAYER_WORK *ply_work)
{
	// 終了チェック
	if (!ply_work->obj_work.spd_m && !ply_work->obj_work.spd.z) {
		/* 歩き停止 */
		// フリップ反映
        //gmPlayerWalkTurnCheck(ply_work);
		// FWへ
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
		return (TRUE);
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckTurn
/*!
 *	振り向き判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckTurn(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->seq_state == GME_PLY_SEQ_STATE_TURN) {
		// ターン中
		if ((!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && GmPlayerKeyCheckWalkRight(ply_work)) ||
				((ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && GmPlayerKeyCheckWalkLeft(ply_work))) {
			// ターンを行っている方向と逆にキーが入った
			return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_TURN));
		}
	}
	else {
		if ((((ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && GmPlayerKeyCheckWalkRight(ply_work)) ||
				(!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && GmPlayerKeyCheckWalkLeft(ply_work))) &&
				MTM_MATH_ABS(ply_work->obj_work.spd_m) < GMD_PL_BRAKE_PERMIT_SPD) {	// ブレーキ発生状況でない時
			// ターンへ
			return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_TURN));
		}
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckDirectTurn
/*!
 *	即時振り向き判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckDirectTurn(GMS_PLAYER_WORK *ply_work)
{
	if (((ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && GmPlayerKeyCheckWalkRight(ply_work)) ||
			(!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && GmPlayerKeyCheckWalkLeft(ply_work))) {
		if ((ply_work->obj_work.move_flag & OBD_MOVE_JUMP) ||
				(!(ply_work->obj_work.move_flag & OBD_MOVE_JUMP) &&
					MTM_MATH_ABS(ply_work->obj_work.spd_m) < GMD_PL_BRAKE_PERMIT_SPD)) {

			if (ply_work->act_state == GME_PLY_ACT_STATE_JUMP_FALL ||
					ply_work->act_state == GME_PLY_ACT_STATE_JUMP_GF02 ||
					ply_work->act_state == GME_PLY_ACT_STATE_JUMP_FALL_TURN ||
					ply_work->act_state == GME_PLY_ACT_STATE_JUMP_FALL_R ||
					ply_work->act_state == GME_PLY_ACT_STATE_JUMP_FALL_R_TURN) {
				// 落下中即時ターン
				GmPlySeqSetFallTurn(ply_work);
			}
			else {

				// 即時ターン
				GmPlySeqSetProgramTurn(ply_work, GMD_PL_PGM_TURN_SPD_DEF);
				//GmPlayerSetReverse(ply_work);
			}
			return (TRUE);
		}
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckFall
/*!
 *	落下判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckFall(GMS_PLAYER_WORK *ply_work)
{
    // 足元が浮いていたら落下する
	if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER)) {
		// 落下へ移行
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL));
	}

#if 1
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
	// トロッコ用判定
		if (ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_STICK) {
			if (ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_DANGER) {	// 危険状態に移行してからチェック
				// 踏ん張り中
				// 一定フレーム数は落下しない
				if (ply_work->fall_timer) {
					ply_work->fall_timer = ObjTimeCountDown(ply_work->fall_timer);
					return (FALSE);
				}
#if defined GMD_PLY_SEQ_DEBUG_TRUCK_NO_DIR_DIE
				return (FALSE);
#endif
				// 踏ん張り終了
				ply_work->gmk_flag &= ~(GMD_PLGF_GMK_TRUCK_STICK | GMD_PLGF_GMK_TRUCK_DANGER);
				// 落下待ち時間再設定
				GmPlayerSpdParameterSet(ply_work);

				// 落下角度修正
				ply_work->jump_pseudofall_dir = g_gm_main_system.pseudofall_dir;

				// トロッコからこぼれているので座標を調整する。
				// 表示位置設定
				ply_work->obj_work.pos.x = FXM_FLOAT_TO_FX32(NNM_MTX(ply_work->truck_mtx_ply_mtn_pos, 0, 3));
				ply_work->obj_work.pos.y = FXM_FLOAT_TO_FX32(-NNM_MTX(ply_work->truck_mtx_ply_mtn_pos, 1, 3));
				ply_work->obj_work.pos.z = FXM_FLOAT_TO_FX32(NNM_MTX(ply_work->truck_mtx_ply_mtn_pos, 2, 3));

				// 即死
				GmPlySeqChangeDeath(ply_work);

				// トロッコ落下死亡
				ply_work->gmk_flag2 |= GMD_PLGF2_TRUCK_FALL_DEATH;
				// トロッコから開放する
				//GmPlayerSetEndTruckRide(ply_work);
				return (TRUE);
			}
		}
		else {
			u16	now_dir;
			now_dir = (u16)(ply_work->obj_work.dir.z + (ply_work->obj_work.dir_fall - g_gm_main_system.pseudofall_dir));

			// 一定角度以上で一定速度以下
			if (((now_dir + 0x4000) & 0x8000 || now_dir == 0xc000) &&
					(MTM_MATH_ABS(ply_work->obj_work.spd_m) < GMD_PL_FALL_SPD)) {
				// 一定フレーム数は落下しない
				if (ply_work->fall_timer) {
					ply_work->fall_timer = ObjTimeCountDown(ply_work->fall_timer);
					return (FALSE);
				}

				// 落下待ち時間再設定
				GmPlayerSpdParameterSet(ply_work);
				// 落下へ移行
				return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL));
			}
			else {
				// 落下待ち時間再設定
				GmPlayerSpdParameterSet(ply_work);
			}
		}
	}
	else {
	// 通常判定
		// 一定フレーム数は落下しない
		if (ply_work->fall_timer) {
			ply_work->fall_timer = ObjTimeCountDown(ply_work->fall_timer);
		}
		else {
			// 一定角度以上で一定速度以下
			if (((ply_work->obj_work.dir.z + 0x4000) & 0x8000 || ply_work->obj_work.dir.z == 0xc000) &&
			//if (((ply_work->obj_work.dir.z - g_gm_main_system.pseudofall_dir + 0x4000) & 0x8000
			//			|| ply_work->obj_work.dir.z - g_gm_main_system.pseudofall_dir == 0xc000) &&
					(MTM_MATH_ABS(ply_work->obj_work.spd_m) < GMD_PL_FALL_SPD)) {
				// 落下待ち時間再設定
				GmPlayerSpdParameterSet(ply_work);
				// 落下へ移行
				return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL));
			}
		}
	}

#else
	// ◆一定角度以内なら落下しない判定をしてからフレームを減らすべきではないか？
    // 一定フレーム数は落下しない
	if (ply_work->fall_timer) {
		ply_work->fall_timer = ObjTimeCountDown(ply_work->fall_timer);
    }
	else {
		// 一定角度以内なら落下しない
		if (!((ply_work->obj_work.dir.z + 0x4000) & 0x8000 || ply_work->obj_work.dir.z == 0xc000) ) {
			return (FALSE);
		}

		// 一定速度以上なら落下しない
		if (MTM_MATH_ABS(ply_work->obj_work.spd_m) >= GMD_PL_FALL_SPD) {
			return (FALSE);
		}

		// 落下待ち時間再設定
		GmPlayerSpdParameterSet(ply_work);
		// 落下へ移行
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL));
	}
#endif

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckStagger
/*!
 *	よろけ判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckStagger(GMS_PLAYER_WORK *ply_work)
{
	OBS_COL_CHK_DATA	col_data;
	s32					diff_1, diff_2, diff_3;
					
	if ((ply_work->obj_work.dir.z & 0x7FFF) || ply_work->obj_work.ride_obj) {
		// 地面が水平でない場合・オブジェクト地形に乗っている時はよろけチェックを行わない
		// ◆暫定
		return (FALSE);
	}

	// プレイヤーの足元中心点が浮いているか確認
	col_data.pos_x	= ply_work->obj_work.pos.x >> FX32_SHIFT;
	col_data.pos_y	= (ply_work->obj_work.pos.y >> FX32_SHIFT) + ply_work->obj_work.field_rect[OBD_BOTTOM];
	col_data.flag	= (u16)(ply_work->obj_work.flag & OBD_OBJECT_B);
	col_data.vec	= OBD_COL_DOWN;
	col_data.dir	= NULL;
	col_data.attr	= NULL;
	diff_3 = ObjDiffCollision(&col_data);

	if (diff_3 > 0) {
		// 左端確認
		col_data.pos_x	= (ply_work->obj_work.pos.x >> FX32_SHIFT) +
						ply_work->obj_work.field_rect[OBD_LEFT] - GMD_PL_STAGGER_NORMAL_OFST;
		diff_1 = ObjDiffCollision(&col_data);

		// 右端確認
		col_data.pos_x	= (ply_work->obj_work.pos.x >> FX32_SHIFT) +
						ply_work->obj_work.field_rect[OBD_RIGHT] + GMD_PL_STAGGER_NORMAL_OFST;
		diff_2 = ObjDiffCollision(&col_data);

		//return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL));
		// 片足が浮いている時はよろけ移行
		if (diff_1 <= 0 && diff_2 >= 16) {
			// 左側接地
			col_data.pos_x	= (ply_work->obj_work.pos.x >> FX32_SHIFT) +
						ply_work->obj_work.field_rect[OBD_LEFT] - GMD_PL_STAGGER_DANGER_OFST;
			diff_1 = ObjDiffCollision(&col_data);
#if 1
			if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
				// 後ろ
				return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_STAGGER_B));
			}
			else {
				if (diff_1 > 0) {
					// 危険
					return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_STAGGER_D));
				}
				else {
					// 前
					return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_STAGGER_F));
				}
			}
#else
			if (diff_1 > 0) {
				// 危険
				return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_STAGGER_D));
			}
			else {
				if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
					// 後ろ
					return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_STAGGER_B));
				}
				else {
					// 前
					return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_STAGGER_F));
				}
			}
#endif
		}
		if (diff_1 >= 16 && diff_2 <= 0) {
			// 右側接地
			col_data.pos_x	= (ply_work->obj_work.pos.x >> FX32_SHIFT) +
							ply_work->obj_work.field_rect[OBD_RIGHT] + GMD_PL_STAGGER_DANGER_OFST;
			diff_2 = ObjDiffCollision(&col_data);
#if 1
			if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
				if (diff_2 > 0) {
					// 危険
					return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_STAGGER_D));
				}
				else {
					// 前
					return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_STAGGER_F));
				}
			}
			else {
				// 後ろ
				return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_STAGGER_B));
			}
#else
			if (diff_2 > 0) {
				// 危険
				return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_STAGGER_D));
			}
			else {
				if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
					// 前
					return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_STAGGER_F));
				}
				else {
					// 後ろ
					return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_STAGGER_B));
				}
			}
#endif
		}
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckEndLookup
/*!
 *	見上げ終了判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckEndLookup(GMS_PLAYER_WORK *ply_work)
{
    // 足元が浮いていたら落下する
	if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER)) {
		// 落下へ移行
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL));
	}
	if (!(ply_work->key_on & PAD_KEY_UP)) {
		// 見上げ終了
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_LOOKUP_END));
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckEndSquat
/*!
 *	しゃがみ終了判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckEndSquat(GMS_PLAYER_WORK *ply_work)
{
    // 足元が浮いていたら落下する
	if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER)) {
		// 落下へ移行
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL));
	}
	if (!(ply_work->key_on & PAD_KEY_DOWN)) {
		// しゃがみ終了立ち上がり
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SQUAT_END));
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckEndSpin
/*!
 *	スピン終了判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckEndSpin(GMS_PLAYER_WORK *ply_work)
{
	// 一定速度以下で終了
	if ((ply_work->obj_work.spd_m < GMD_PL_STOP_SPD) && (ply_work->obj_work.spd_m > -GMD_PL_STOP_SPD)) {
		ply_work->obj_work.spd_m = 0;
		GmPlayerSpdParameterSet(ply_work);

		if (ply_work->player_flag & GMD_PLF_PINBALL_SONIC) {
			// ピンボールの時はアクションを戻しておく
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_SPIN);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
		// FWへ
		return(GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW));
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckEndWallPush
/*!
 *	壁押し終了判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckEndWallPush(GMS_PLAYER_WORK *ply_work)
{
	if (!(ply_work->obj_work.move_flag & OBD_MOVE_FRONT) ||
			((ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && !GmPlayerKeyCheckWalkLeft(ply_work)) ||
				(!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && !GmPlayerKeyCheckWalkRight(ply_work))) {
		// FWへ
		return(GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW));
	}

	return (FALSE);
}

#if 0
// ==========================================================================
// gmPlySeqCheckEndBrake
/*!
 *	ブレーキ終了判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckEndBrake(GMS_PLAYER_WORK *ply_work)
{
#if 0
	if (!(GmPlayerKeyCheckWalkLeft(ply_work) | GmPlayerKeyCheckWalkRight(ply_work))) {
		// ブレーキ終了
		if (ply_work->obj_work.spd_m) {
			return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_WALK));
		}
		else {
			return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW));
		}
	}
#endif
	return (FALSE);
}
#endif

// ==========================================================================
// gmPlySeqCheckHoming
/*!
 *	ホーミング判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckHoming(GMS_PLAYER_WORK *ply_work)
{
	if (GmPlayerKeyCheckJumpKeyPush(ply_work)
		&& !ply_work->homing_timer
		&& !(ply_work->player_flag & GMD_PLF_NOHOMING)
		&& !GMM_MAIN_STAGE_IS_ENDING() ) {
		if (ply_work->enemy_obj) {
			// ホーミングアタック
			return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_HOMING));
		}
		else {
			// ジャンプダッシュ
			return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_JUMPDASH));

		}
	}
	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckJumpDash
/*!
 *	ジャンプダッシュ判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
//BOOL gmPlySeqCheckJumpDash(GMS_PLAYER_WORK *ply_work)
//{
//	return (FALSE);
//}

// ==========================================================================
// gmPlySeqCheckSquatSpin
/*!
 *	しゃがみ入力時スピン判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckSquatSpin(GMS_PLAYER_WORK *ply_work)
{
    // しゃがみ開始+速度が一定以上の場合
	if ((ply_work->key_on & PAD_KEY_DOWN) &&
			(ply_work->obj_work.spd_m > GMD_PL_STOP_SPD || ply_work->obj_work.spd_m < -GMD_PL_STOP_SPD)) {
		// スピンへ移行
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SPIN));
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckSpin
/*!
 *	スピン判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckSpin(GMS_PLAYER_WORK *ply_work)
{
    // 速度が一定以上の場合
	if (ply_work->obj_work.spd_m > GMD_PL_STOP_SPD || ply_work->obj_work.spd_m < -GMD_PL_STOP_SPD) {
		// スピンへ移行
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SPIN));
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckSpinDashAcc
/*!
 *	スピンダッシュ判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckSpinDashAcc(GMS_PLAYER_WORK *ply_work)
{
    // ジャンプキー入力があった場合
	if (GmPlayerKeyCheckJumpKeyPush(ply_work)) {
		// スピンダッシュ加速へ移行
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SPIN_DASHACC));
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckPinballSpinDashAcc
/*!
 *	ピンボール用 スピンダッシュ判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckPinballSpinDashAcc(GMS_PLAYER_WORK *ply_work)
{
    // 下入力とジャンプキー入力があった場合
	if (ply_work->key_on & PAD_KEY_DOWN &&
			GmPlayerKeyCheckJumpKeyPush(ply_work)) {
		// スピンダッシュ加速へ移行
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SPIN_DASHACC));
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckJump
/*!
 *	ジャンプ判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckJump(GMS_PLAYER_WORK *ply_work)
{
	if (GmPlayerKeyCheckJumpKeyPush(ply_work) &&
				((ply_work->obj_work.move_flag & OBD_MOVE_UNDER) ||
			(ply_work->gmk_obj && (ply_work->gmk_flag & GMD_PLGF_GMK_AIR_JUMP)))) {
		// ジャンプへ移行
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_JUMP));
	}
	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckBrake
/*!
 *	ブレーキ判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckBrake(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->seq_state != GME_PLY_SEQ_STATE_BRAKE &&
			((GmPlayerKeyCheckWalkLeft(ply_work) && ply_work->obj_work.spd_m >= GMD_PL_BRAKE_PERMIT_SPD) ||
			(GmPlayerKeyCheckWalkRight(ply_work) && ply_work->obj_work.spd_m <= -GMD_PL_BRAKE_PERMIT_SPD))) {
		// ブレーキへ移行
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_BRAKE));
	}
	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckWalk
/*!
 *	歩き判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckWalk(GMS_PLAYER_WORK *ply_work)
{
	if ((GmObjCheckMapLeftLimit(&ply_work->obj_work, GMD_PLAYER_MAP_END_OFST) && GmPlayerKeyCheckWalkLeft(ply_work)) ||
		(GmObjCheckMapRightLimit(&ply_work->obj_work, GMD_PLAYER_MAP_END_OFST) && GmPlayerKeyCheckWalkRight(ply_work))) {
		// 画面端に向けて移動しようとしている
		return (FALSE);
	}

	if ((GME_PLY_SEQ_STATE_STAGGER_F <= ply_work->seq_state &&
				ply_work->seq_state <= GME_PLY_SEQ_STATE_STAGGER_D) &&
			(ply_work->obj_work.move_flag & OBD_MOVE_FRONT)) {

		if (((ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && GmPlayerKeyCheckWalkLeft(ply_work)) ||
				(!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && GmPlayerKeyCheckWalkRight(ply_work))) {
			// おっとっと中に前に障害物がある時は前に歩けない
			return (FALSE);
		}
	}

	if (ply_work->obj_work.spd_m ||		// すでに速度がのっている
			GmPlayerKeyCheckWalkLeft(ply_work) || GmPlayerKeyCheckWalkRight(ply_work)) {
		// 歩きへ移行
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_WALK));
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckLookup
/*!
 *	見上げ判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckLookup(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->obj_work.spd_m != 0 || !(ply_work->obj_work.move_flag & OBD_MOVE_UNDER)) {
		return (FALSE);
	}
	if (ply_work->key_on & PAD_KEY_UP) {
		// 見上げ開始
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_LOOKUP_ST));
	}
	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckSquat
/*!
 *	しゃがみ判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckSquat(GMS_PLAYER_WORK *ply_work)
{
//	// 足元が浮いていたら落下する
//	if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER)) {
//		// 落下へ移行
//		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL));
//	}
	if (ply_work->key_on & PAD_KEY_DOWN) {
		// しゃがみ開始
		//return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SQUAT_ST));
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SQUAT_M));
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckWallPush
/*!
 *	壁押し判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckWallPush(GMS_PLAYER_WORK *ply_work)
{
//	if (ply_work->key_on & PAD_KEY_DOWN) {
//		// しゃがみ開始
//		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SQUAT_ST));
//	}

	if ((ply_work->obj_work.move_flag & OBD_MOVE_FRONT) &&
		(!(ply_work->player_flag & GMD_PLF_AUTO_RUN)) &&						// オートラン中は壁押し無し
			(( (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && GmPlayerKeyCheckWalkLeft(ply_work)) ||
				(!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && GmPlayerKeyCheckWalkRight(ply_work))) &&
			((ply_work->obj_work.pos.x >> FX32_SHIFT) > g_gm_main_system.map_fcol.left + GMD_PLAYER_MAP_END_OFST) && /* マップ端チェック */
			((ply_work->obj_work.pos.x >> FX32_SHIFT) < g_gm_main_system.map_fcol.right - GMD_PLAYER_MAP_END_OFST) /* マップ端チェック */) {

		// 壁押し開始
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_WALLPUSH));
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckTruckWalk
/*!
 *	トロッコ歩き判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckTruckWalk(GMS_PLAYER_WORK *ply_work)
{
	if ((GmObjCheckMapLeftLimit(&ply_work->obj_work, GMD_PLAYER_MAP_END_OFST) && GmPlayerKeyCheckWalkLeft(ply_work)) ||
		(GmObjCheckMapRightLimit(&ply_work->obj_work, GMD_PLAYER_MAP_END_OFST) && GmPlayerKeyCheckWalkRight(ply_work))) {
		// 画面端に向けて移動しようとしている
		return (FALSE);
	}

	if ((MTM_MATH_ABS(ply_work->obj_work.spd_m) >= GMD_PL_TRUCK_WALK_SPD_MIN)  ||		// すでに速度がのっている
			GmPlayerKeyCheckWalkLeft(ply_work) || GmPlayerKeyCheckWalkRight(ply_work)) {
		// 歩きへ移行
		return (GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_WALK));
	}
	else {
		ply_work->obj_work.spd_m = 0;
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqCheckEndTruckWalk
/*!
 *	トロッコ歩き終了判定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 * @retval	TRUE	シーケンスチェンジあり
 * @retval	FALSE	シーケンスチェンジなし
 */
// ==========================================================================
BOOL gmPlySeqCheckEndTruckWalk(GMS_PLAYER_WORK *ply_work)
{
	// 終了チェック
	if ((MTM_MATH_ABS(ply_work->obj_work.spd_m) < GMD_PL_TRUCK_WALK_SPD_MIN) &&	// 一定速度以下で停止
			!ply_work->obj_work.spd.z) {
		/* 歩き停止 */
		// 速度クリア
		ply_work->obj_work.spd_m = 0;
		// FWへ
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
		return (TRUE);
	}

	return (FALSE);
}

// ==========================================================================
// gmPlySeqSplStgRollCtrl
/*!
 *	スペステ回転操作入力制御
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqSplStgRollCtrl(GMS_PLAYER_WORK *ply_work)
{
	OBS_CAMERA	*obj_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);

	if (g_gm_main_system.game_flag & (GMD_GAME_FLAG_RESULT_START		// リザルト演出開始
									| GMD_GAME_FLAG_RESULT_END			// リザルト演出終了
									| GMD_GAME_FLAG_TIMEOVER			// タイムオーバー死亡
									| GMD_GAME_FLAG_SPL_TIMEOVER		// スペステ：タイムオーバー
									| GMD_GAME_FLAG_SPL_CHAOSGET		// スペステ：カオス取得
									| GMD_GAME_FLAG_SPL_FAILED			// スペステ：ゴール
									| GMD_GAME_FLAG_START_DEMO			// スタートデモ中
									| GMD_GAME_FLAG_START_MSG ) ) {		// ゲーム開始時メッセージ表示
#if _PS3
		// PS3で開始直後に揺らしが発動しないようaxis_yは更新行う
		if (g_gm_main_system.game_flag & (GMD_GAME_FLAG_START_DEMO			// スタートデモ中
										| GMD_GAME_FLAG_START_MSG ) ) {		// ゲーム開始時メッセージ表示
			ply_work->axis_y = AoPadAxisY();
		}
#endif // _PS3
		return;
	}
	// ■■■■ 画面回転入力 ■■■■
#if _PS3 | _WII
// 傾斜角を現在角度に加算
	s32	dir_vec;

// PS3 & Wiiはスティック入力なし
//	dir_vec = (s32)ply_work->key_walk_rot_z;	// スティック等入力
//	dir_vec /= 112;	// ±0x8000 想定で計算
//
//	if (!dir_vec)
	{
		// スティック入力無ければ傾斜を入力
		dir_vec = (s32)ply_work->key_rot_z;
		dir_vec = MTM_MATH_CLIP(dir_vec, -0x2800, 0x2800);
		dir_vec /= 34;	// ±0x2800 想定で計算
	}

	obj_camera->roll += (u32)dir_vec;

#elif _PC | _XBOX
// 傾斜角を現在角度に加算
	s32	dir_vec;

	dir_vec = (s32)ply_work->key_walk_rot_z;	// スティック等入力
	dir_vec /= 112;	// ±0x8000 想定で計算

// PC & XBOXはトリガー入力なし
//	if (!dir_vec)
//	{
//		// スティック入力無ければ傾斜(トリガー)を入力
//		dir_vec = (s32)ply_work->key_rot_z;
//		dir_vec /= 112;	// ±0x8000 想定で計算
//	}

	obj_camera->roll += (u32)dir_vec;
#elif _IPHONE
#ifndef GMD_MAIN_USE_BODY_ROTATE
	s32 dir_vec = (s32)ply_work->key_rot_z;
	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK)
	{
		//obj_camera->roll = (u32)dir_vec; // 角度をそのまま使う
		dir_vec = dir_vec * 26 / 10; // base:15/10
		obj_camera->roll += (u32)dir_vec; // 差分となる角度を加算
	}
	else
	{
#define GMD_PLY_DIR_VEC_DIV (66)  // base:112 , x1.7(iPhone)
#if 0
		// 直値
		dir_vec /= GMD_PLY_DIR_VEC_DIV;
#else
		// 加速度補正
		ply_work->accel_counter;
		ply_work->dir_vec_add;
		
		ply_work->accel_counter--;
		if (ply_work->accel_counter <= 0) {
			ply_work->accel_counter = 0;
			s32 dir_vec_scale = dir_vec - ply_work->prev_key_rot_z;
			dir_vec_scale = dir_vec_scale * 3 / 4;
			if (MTM_MATH_ABS(dir_vec_scale) < 0x4000) {
				dir_vec_scale = 0;
			}
			else if ((dir_vec_scale < 0 && dir_vec > 0) || (dir_vec_scale > 0 && dir_vec < 0)) {
				dir_vec_scale = 0;
			}
			else {
				dir_vec_scale /= 64;
				ply_work->dir_vec_add = dir_vec_scale;
				ply_work->accel_counter = 8;
			}
		}
		dir_vec /= GMD_PLY_DIR_VEC_DIV;
		dir_vec += ply_work->dir_vec_add * ply_work->accel_counter;
#endif // 01
		obj_camera->roll += (u32)dir_vec;
	}
#endif // GMD_MAIN_USE_BODY_ROTATE
#endif
	
	// ■■■■ ナッジング(揺らし)入力 ■■■■
#if _PS3
	AOT_AXIS axis_y_before;
	axis_y_before = ply_work->axis_y;
	ply_work->axis_y = AoPadAxisY();
#endif // _PS3
	if (ply_work->nudge_di_timer) {
		// ナッジング禁止中
		ply_work->nudge_di_timer--;
	} else {
		// ナッジング受付
#if _IPHONE
#if 01
		BOOL is_nudge = FALSE;
#if 0 // 常にボタン入力のエミュレーションに変更した
		if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK)
		{
			is_nudge = ply_work->is_nudge;
		}
		else
#endif // 0
		{
			if (ply_work->key_push & (PAD_BUTTON_A | PAD_BUTTON_B)) { // キー入力でナッジング
				is_nudge = TRUE;
			}
		}
		if (is_nudge)
#else
		if (ply_work->is_nudge)
#endif // 01
		{
			GMS_SPL_STG_WORK *spl_work = GmSplStageGetWork();

			ply_work->nudge_di_timer = GMD_PL_SS_NUDGING_DI_TIME;
			ply_work->nudge_timer = GMD_PL_SS_NUDGING_TIME;
			spl_work->flag &= ~GMD_SPL_STAGE_NUDGE_HIT;
		}
#else
#if _PS3
//		if (AoPadAxisY() > 555) {							// PS3：axis Y が閾値を超えたら入力とみなす
		if (  (ply_work->axis_y > (axis_y_before + 160))
			||(ply_work->axis_y < (axis_y_before - 160)) ) {
#elif _WII
		if (  (AoPadAxisX() > 2.0f) 						// Wii：axis X or Yが
			||(AoPadAxisY() < -2.0) ) {						//		閾値を超えたら入力とみなす
#else
//		if (ply_work->key_push & PAD_BUTTON_X) {			// PC/360：Ｘボタン
		if (ply_work->key_push & (PAD_BUTTON_A | PAD_BUTTON_B)) { // PC/360：Ａ｜Ｂボタン
#endif
			GMS_SPL_STG_WORK *spl_work = GmSplStageGetWork();

			ply_work->nudge_di_timer = GMD_PL_SS_NUDGING_DI_TIME;
			ply_work->nudge_timer = GMD_PL_SS_NUDGING_TIME;
			spl_work->flag &= ~GMD_SPL_STAGE_NUDGE_HIT;
		}
#endif // _IPHONE
	}
}

#if defined (MTD_DEBUG)
// ==========================================================================
// gmPlySeqDebugMove
/*!
 *	デバッグ移動
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqDebugMove(GMS_PLAYER_WORK *ply_work)
{
	ply_work->obj_work.spd.x = ply_work->obj_work.spd.y = ply_work->obj_work.spd.z = 0;
	ply_work->obj_work.spd_add.x = ply_work->obj_work.spd_add.y = ply_work->obj_work.spd_add.z = 0;
	if (PAD_MDIRECT(0) & KEY_L_LEFT) {
		ply_work->obj_work.spd.x -= 0xF000;
	}
	if (PAD_MDIRECT(0) & KEY_L_RIGHT) {
		ply_work->obj_work.spd.x += 0xF000;
	}
	if (PAD_MDIRECT(0) & KEY_L_UP) {
		ply_work->obj_work.spd.y -= 0xF000;
	}
	if (PAD_MDIRECT(0) & KEY_L_DOWN) {
		ply_work->obj_work.spd.y += 0xF000;
	}

	ply_work->obj_work.move_flag |= OBD_MOVE_NOCOL;
	ply_work->obj_work.flag |= OBD_OBJECT_NOHIT;
}


#endif


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
