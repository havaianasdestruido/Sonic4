// ==========================================================================
/*!
  @file gmPlySeqDat.cpp
  @brief シーケンスデータ

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmPlySeqDat.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gsMainSys.h"

#include "gmPlySeqDat.h"

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
//===========================================================================
// シーケンス設定データ
//===========================================================================
/// 通常
const GMS_PLY_SEQ_STATE_DATA g_gm_ply_seq_state_data_tbl[GSD_CHAR_ID_MAX][GME_PLY_SEQ_STATE_MAX] = {
  // ========================================================================
  // ソニック
  // ========================================================================
  {
	// FW
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_STAGGER,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_LOOKUP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT_SPIN |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALLPUSH,			// 移行可能属性
	},

	// 歩き
	{
		//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_STAND |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_WALK_END |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_BRAKE |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT_SPIN |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALLPUSH,			// 移行可能属性
	},

	// 振り向き
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// 見上げ開始
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_END_LOOKUP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 見上げ中
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_END_LOOKUP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 見上げ終了
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// しゃがみ開始
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPIN |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPINACC,			// 移行可能属性
	},

	// しゃがみ中
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SQUAT |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPIN |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPINACC,			// 移行可能属性
	},

	// しゃがみ終了
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT,			// 移行可能属性
	},

	// ブレーキ
	{
		//GMD_PLY_SEQ_STATE_CHECK_ATTR_END_BRAKE |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// スピン
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// スピンダッシュ加速
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPINACC,				// 移行可能属性
	},

	// スピンダッシュ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPINACC,				// 移行可能属性
	},

	// よろける 前
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// よろける 後ろ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// よろける 危険
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 落下
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},

	// ジャンプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},

	// 押す
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_END_PUSH |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// ホーミング
	{
		0/*GMD_PLY_SEQ_STATE_CHECK_ATTR_END_HOMING*/,		// 各種チェック属性
		0,													// 移行可能属性
	},

	// ホーミング跳ね返り
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			//GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},

	// ジャンプダッシュ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,													// 移行可能属性
	},

	// ダメージ
	{
		0,													// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,			// 移行可能属性
	},

	// 死亡
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// スーパーソニック化
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ボスゴール(カプセル開放後)
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ボスFINAL演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// タイムアタックリトライ FW 演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// タイムアタックリトライ その場加速 演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},



	/* 以下ギミック関連 */

	// スプリングジャンプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},
	// 岩乗り開始
	{
		0,	// 各種チェック属性
		
		0,		// 移行可能属性
	},
	// 岩乗り
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,		// 移行可能属性
	},
	// 滑車掴まり
	{
		0,													// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// 息継ぎ
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// ダッシュパネル
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			//GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// ターザンロープ
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// ウォータースライダー
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// Ｓ字パイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// コークスクリュー
	{
//		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK |	// 歩きタイプモーション速度設定
//		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK |		// 歩き移動
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,	// 移行可能属性
	},
	// デモ用フットワーク
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},

	// ストッパー＠ピンボール
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// 大砲
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// 大砲発射
	{
		// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |		// 即振り向き可能..
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP,			// ジャンプ時移動可
		// 移行可能属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,			// ホーミング可
	},
	// アップバンパー
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// シーソー
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// ピンボール
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN,	// 各種チェック属性
		0,	// 移行可能属性	
	},
	// ピンボール（空中）
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR | 
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,			// 移行可能属性
	},
	// フリッパー
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// スプリングカタパルト確保
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// スプリングカタパルト↑
	{
		// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |		// 即振り向き可能..
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP,			// ジャンプ時移動可
		// 移行可能属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,			// ホーミング可
	},
	// スプリングカタパルト↑以外
	{
		// 各種チェック属性
			0,
		// 移行可能属性
			0,
	},

	// 強制スピン
	{
		// 各種チェック属性
	//	GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |					//	落下ＯＮ
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC,	//	スピン減速無し
		// 移行可能属性
		0,
	},
	// 強制スピン 減速タイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC,		// 各種チェック属性
		0,													// 移行可能属性
	},
	// 強制スピン 落下
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN,			// 各種チェック属性
		0,													// 移行可能属性
	},
	// 移動歯車
	{
		//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_STAND |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_NOSET,		// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// 排液装置
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// 排液装置（落下）
	{
		0,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,	// 移行可能属性
	},
	// スチームパイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_NOSET,	// 各種チェック属性
		0,												// 移行可能属性
	},
	// ポップスチーム
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,		// 移行可能属性
	},
	// スペステリングＩＮ
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// ボス2掴み
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// ボスFINAL地球割り着地振動
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// エンディング１
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// エンディング２
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// トロッコ危険状態
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,			// 各種チェック属性
		0,											// 移行可能属性
	},
	// トロッコ危険復帰
	{
		0,			// 各種チェック属性
		0,			// 移行可能属性
	},
	// スピン状態で落下
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},
  },

  // ========================================================================
  // スーパーソニック
  // ========================================================================
  {
	// FW
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_LOOKUP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT_SPIN |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALLPUSH,			// 移行可能属性
	},

	// 歩き
	{
		//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_STAND |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_WALK_END |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_BRAKE |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT_SPIN |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALLPUSH,			// 移行可能属性
	},

	// 振り向き
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// 見上げ開始
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_END_LOOKUP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 見上げ中
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_END_LOOKUP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 見上げ終了
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// しゃがみ開始
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPIN |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPINACC,			// 移行可能属性
	},

	// しゃがみ中
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SQUAT |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPIN |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPINACC,			// 移行可能属性
	},

	// しゃがみ終了
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT,			// 移行可能属性
	},

	// ブレーキ
	{
		//GMD_PLY_SEQ_STATE_CHECK_ATTR_END_BRAKE |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// スピン
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// スピンダッシュ加速
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPINACC,				// 移行可能属性
	},

	// スピンダッシュ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPINACC,				// 移行可能属性
	},

	// よろける 前
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// よろける 後ろ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// よろける 危険
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 落下
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},

	// ジャンプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},

	// 押す
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_END_PUSH |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// ホーミング
	{
		0/*GMD_PLY_SEQ_STATE_CHECK_ATTR_END_HOMING*/,		// 各種チェック属性
		0,													// 移行可能属性
	},

	// ホーミング跳ね返り
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			//GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},

	// ジャンプダッシュ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,													// 移行可能属性
	},

	// ダメージ
	{
		0,													// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,			// 移行可能属性
	},

	// 死亡
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// スーパーソニック化
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ボスゴール(カプセル開放後)
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ボスFINAL演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// タイムアタックリトライ FW 演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// タイムアタックリトライ その場加速 演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},


	/* 以下ギミック関連 */

	// スプリングジャンプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},
	// 岩乗り開始
	{
		0,	// 各種チェック属性
		
		0,		// 移行可能属性
	},
	// 岩乗り
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,		// 移行可能属性
	},
	// 滑車掴まり
	{
		0,													// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// 息継ぎ
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// ダッシュパネル
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			//GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// ターザンロープ
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// ウォータースライダー
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// Ｓ字パイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// コークスクリュー
	{
//		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK |	// 歩きタイプモーション速度設定
//		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK |		// 歩き移動
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,	// 移行可能属性
	},
	// デモ用フットワーク
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},

	// ストッパー＠ピンボール
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// 大砲
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// 大砲発射
	{
		// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |		// 即振り向き可能.
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP,			// ジャンプ時移動可
		// 移行可能属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,			// ホーミング可
	},
	// アップバンパー
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// シーソー
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// ピンボール
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// ピンボール（空中）
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR | 
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,			// 移行可能属性
	},
	// フリッパー
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// スプリングカタパルト確保
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// スプリングカタパルト↑
	{
		// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |		// 即振り向き可能.
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP,			// ジャンプ時移動可
		// 移行可能属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,			// ホーミング可
	},
	// スプリングカタパルト↑以外
	{
		// 各種チェック属性
			0,
		// 移行可能属性
			0,
	},
	// 強制スピン
	{
		// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC,	//	スピン減速無し
		// 移行可能属性
		0,
	},
	// 強制スピン 減速タイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN,				// 各種チェック属性
		0,													// 移行可能属性
	},
	// 強制スピン 落下
	{
		0,												// 各種チェック属性
		0,												// 移行可能属性
	},
	// 移動歯車
	{
		//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_STAND |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_NOSET,		// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// 排液装置
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// 排液装置（落下）
	{
		0,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,	// 移行可能属性
	},
	// スチームパイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_NOSET,	// 各種チェック属性
		0,												// 移行可能属性
	},
	// ポップスチーム
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,		// 移行可能属性
	},
	// スペステリングＩＮ
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// ボス2掴み
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// ボスFINAL地球割り着地振動
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// エンディング１
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// エンディング２
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// トロッコ危険状態
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,			// 各種チェック属性
		0,											// 移行可能属性
	},
	// トロッコ危険復帰
	{
		0,			// 各種チェック属性
		0,			// 移行可能属性
	},
	// スピン状態で落下
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},
  },


  // ========================================================================
  // スペステソニック
  // ========================================================================
  {
	// FW
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 歩き
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 振り向き
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 見上げ開始
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 見上げ中
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 見上げ終了
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// しゃがみ開始
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// しゃがみ中
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// しゃがみ終了
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// ブレーキ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// スピン
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// スピンダッシュ加速
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// スピンダッシュ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// よろける 前
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// よろける 後ろ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// よろける 危険
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 落下
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},

	// ジャンプ
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},

	// 押す
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// ホーミング
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},

	// ホーミング跳ね返り
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},

	// ジャンプダッシュ
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},

	// ダメージ
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},

	// 死亡
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// スーパーソニック化
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ボスゴール(カプセル開放後)
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ボスFINAL演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// タイムアタックリトライ FW 演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// タイムアタックリトライ その場加速 演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},


	/* 以下ギミック関連 */

	// スプリングジャンプ
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// 岩乗り開始
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// 岩乗り
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// 滑車掴まり
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// 息継ぎ
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// ダッシュパネル
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// ターザンロープ
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// ウォータースライダー
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// Ｓ字パイプ
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// コークスクリュー
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// デモ用フットワーク
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},

	// ストッパー＠ピンボール
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// 大砲
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// 大砲発射
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// アップバンパー
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// シーソー
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// ピンボール
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// ピンボール（空中）
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// フリッパー
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// スプリングカタパルト確保
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// スプリングカタパルト↑
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// スプリングカタパルト↑以外
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// 強制スピン
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// 強制スピン 減速タイプ
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},
	// 強制スピン 落下
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},
	// 移動歯車
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// 排液装置
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},
	// 排液装置（落下）
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},
	// スチームパイプ
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},
	// ポップスチーム
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},
	// スペステリングＩＮ
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},
	// ボス2掴み
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},
	// ボスFINAL地球割り着地振動
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},
	// エンディング１
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},
	// エンディング２
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},
	// トロッコ危険状態
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},
	// トロッコ危険復帰
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},
	// スピン状態で落下
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},

  },


  // ========================================================================
  // ピンボールソニック
  // ========================================================================
  {
	// FW
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_PINBALL_SPINACC,	// 移行可能属性
	},

	// 歩き
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_WALK_END |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_PINBALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// 振り向き
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_PINBALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// 見上げ開始
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// 見上げ中
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// 見上げ終了
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// しゃがみ開始
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// しゃがみ中
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// しゃがみ終了
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ブレーキ
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// スピン
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// スピンダッシュ加速
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPINACC,				// 移行可能属性
	},

	// スピンダッシュ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPINACC,				// 移行可能属性
	},

	// よろける 前
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// よろける 後ろ
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// よろける 危険
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// 落下
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},

	// ジャンプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},

	// 押す
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ホーミング
	{
		0/*GMD_PLY_SEQ_STATE_CHECK_ATTR_END_HOMING*/,		// 各種チェック属性
		0,													// 移行可能属性
	},

	// ホーミング跳ね返り
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			//GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},

	// ジャンプダッシュ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,													// 移行可能属性
	},

	// ダメージ
	{
		0,													// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,			// 移行可能属性
	},

	// 死亡
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// スーパーソニック化
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ボスゴール(カプセル開放後)
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ボスFINAL演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// タイムアタックリトライ FW 演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// タイムアタックリトライ その場加速 演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},


	/* 以下ギミック関連 */

	// スプリングジャンプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},
	// 岩乗り開始
	{
		0,	// 各種チェック属性
		
		0,		// 移行可能属性
	},
	// 岩乗り
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,		// 移行可能属性
	},
	// 滑車掴まり
	{
		0,													// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// 息継ぎ
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// ダッシュパネル
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			//GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// ターザンロープ
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// ウォータースライダー
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// Ｓ字パイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// コークスクリュー
	{
//		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK |	// 歩きタイプモーション速度設定
//		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK |		// 歩き移動
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,	// 移行可能属性
	},
	// デモ用フットワーク
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},

	// ストッパー＠ピンボール
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// 大砲
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// 大砲発射
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// アップバンパー
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// シーソー
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// ピンボール
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN,		// 各種チェック属性
		0,	// 移行可能属性		
	},
	// ピンボール（空中）
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR | 
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,			// 移行可能属性
	},
	// フリッパー
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// スプリングカタパルト確保
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// スプリングカタパルト↑
	{
		// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |		// 即振り向き可能.
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP,			// ジャンプ時移動可
		// 移行可能属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,			// ホーミング可
	},
	// スプリングカタパルト↑以外
	{
		// 各種チェック属性
			0,
		// 移行可能属性
			0,
	},
	// 強制スピン
	{
		// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC,	//	スピン減速無し
		// 移行可能属性
		0,
	},
	// 強制スピン 減速タイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN,				// 各種チェック属性
		0,													// 移行可能属性
	},
	// 強制スピン 落下
	{
		0,												// 各種チェック属性
		0,												// 移行可能属性
	},
	// 移動歯車
	{
		//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_STAND |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_NOSET,		// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// 排液装置
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// 排液装置（落下）
	{
		0,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,	// 移行可能属性
	},
	// スチームパイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_NOSET,	// 各種チェック属性
		0,												// 移行可能属性
	},
	// ポップスチーム
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,		// 移行可能属性
	},
	// スペステリングＩＮ
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// ボス2掴み
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// ボスFINAL地球割り着地振動
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// エンディング１
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// エンディング２
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// トロッコ危険状態
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,			// 各種チェック属性
		0,											// 移行可能属性
	},
	// トロッコ危険復帰
	{
		0,			// 各種チェック属性
		0,			// 移行可能属性
	},
	// スピン状態で落下
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},

  },

  // ========================================================================
  // ピンボールスーパーソニック
  // ========================================================================
  {
	// FW
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_PINBALL_SPINACC,	// 移行可能属性
	},

	// 歩き
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_WALK_END |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_PINBALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// 振り向き
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_PINBALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// 見上げ開始
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// 見上げ中
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// 見上げ終了
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// しゃがみ開始
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// しゃがみ中
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// しゃがみ終了
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ブレーキ
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// スピン
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// スピンダッシュ加速
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPINACC,				// 移行可能属性
	},

	// スピンダッシュ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPINACC,				// 移行可能属性
	},

	// よろける 前
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// よろける 後ろ
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// よろける 危険
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// 落下
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},

	// ジャンプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},

	// 押す
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ホーミング
	{
		0/*GMD_PLY_SEQ_STATE_CHECK_ATTR_END_HOMING*/,		// 各種チェック属性
		0,													// 移行可能属性
	},

	// ホーミング跳ね返り
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			//GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},

	// ジャンプダッシュ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,													// 移行可能属性
	},

	// ダメージ
	{
		0,													// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,			// 移行可能属性
	},

	// 死亡
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// スーパーソニック化
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ボスゴール(カプセル開放後)
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ボスFINAL演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// タイムアタックリトライ FW 演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// タイムアタックリトライ その場加速 演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},


	/* 以下ギミック関連 */

	// スプリングジャンプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},
	// 岩乗り開始
	{
		0,	// 各種チェック属性
		
		0,		// 移行可能属性
	},
	// 岩乗り
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,		// 移行可能属性
	},
	// 滑車掴まり
	{
		0,													// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// 息継ぎ
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// ダッシュパネル
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			//GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// ターザンロープ
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// ウォータースライダー
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// Ｓ字パイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// コークスクリュー
	{
//		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK |	// 歩きタイプモーション速度設定
//		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK |		// 歩き移動
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,	// 移行可能属性
	},
	// デモ用フットワーク
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},

	// ストッパー＠ピンボール
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// 大砲
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// 大砲発射
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// アップバンパー
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// シーソー
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// ピンボール
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN,		// 各種チェック属性
		0,	// 移行可能属性	
	},
	// ピンボール（空中）
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR | 
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,			// 移行可能属性
	},
	// フリッパー
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// スプリングカタパルト確保
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// スプリングカタパルト↑
	{
		// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |		// 即振り向き可能.
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP,			// ジャンプ時移動可
		// 移行可能属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,			// ホーミング可
	},
	// スプリングカタパルト↑以外
	{
		// 各種チェック属性
			0,
		// 移行可能属性
			0,
	},
	// 強制スピン
	{
		// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC,	//	スピン減速無し
		// 移行可能属性
		0,
	},
	// 強制スピン 減速タイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN,				// 各種チェック属性
		0,													// 移行可能属性
	},
	// 強制スピン 落下
	{
		0,												// 各種チェック属性
		0,												// 移行可能属性
	},
	// 移動歯車
	{
		//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_STAND |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_NOSET,		// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// 排液装置
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// 排液装置（落下）
	{
		0,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,	// 移行可能属性
	},
	// スチームパイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_NOSET,	// 各種チェック属性
		0,												// 移行可能属性
	},
	// ポップスチーム
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,		// 移行可能属性
	},
	// スペステリングＩＮ
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// ボス2掴み
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// ボスFINAL地球割り着地振動
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// エンディング１
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// エンディング２
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// トロッコ危険状態
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,			// 各種チェック属性
		0,											// 移行可能属性
	},
	// トロッコ危険復帰
	{
		0,			// 各種チェック属性
		0,			// 移行可能属性
	},
	// スピン状態で落下
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},
  },

  
  // ========================================================================
  // トロッコソニック
  // ========================================================================
  {
	// FW
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_LOOKUP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT,			// 移行可能属性
	},

	// 歩き
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_WALK_END |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// 振り向き
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// 見上げ開始
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_END_LOOKUP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 見上げ中
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_END_LOOKUP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 見上げ終了
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// しゃがみ開始
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,													// 移行可能属性
	},

	// しゃがみ中
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SQUAT |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
		0,													// 移行可能属性
	},

	// しゃがみ終了
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT,			// 移行可能属性
	},

	// ブレーキ
	{
		//GMD_PLY_SEQ_STATE_CHECK_ATTR_END_BRAKE |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// スピン
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// スピンダッシュ加速
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// スピンダッシュ
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// よろける 前
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// よろける 後ろ
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// よろける 危険
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// 落下
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,													// 移行可能属性
	},

	// ジャンプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,													// 移行可能属性
	},

	// 押す
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ホーミング
	{
		0/*GMD_PLY_SEQ_STATE_CHECK_ATTR_END_HOMING*/,		// 各種チェック属性
		0,													// 移行可能属性
	},

	// ホーミング跳ね返り
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ジャンプダッシュ
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ダメージ
	{
		0,													// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,			// 移行可能属性
	},

	// 死亡
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// スーパーソニック化
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ボスゴール(カプセル開放後)
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ボスFINAL演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// タイムアタックリトライ FW 演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// タイムアタックリトライ その場加速 演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},


	/* 以下ギミック関連 */

	// スプリングジャンプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},
	// 岩乗り開始
	{
		0,	// 各種チェック属性
		
		0,		// 移行可能属性
	},
	// 岩乗り
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,		// 移行可能属性
	},
	// 滑車掴まり
	{
		0,													// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// 息継ぎ
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// ダッシュパネル
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			//GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// ターザンロープ
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// ウォータースライダー
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// Ｓ字パイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// コークスクリュー
	{
//		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK |	// 歩きタイプモーション速度設定
//		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK |		// 歩き移動
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,	// 移行可能属性
	},
	// デモ用フットワーク
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},

	// ストッパー＠ピンボール
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// 大砲
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// 大砲発射
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// アップバンパー
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// シーソー
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// ピンボール
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN,		// 各種チェック属性
		0,	// 移行可能属性	
	},
	// ピンボール（空中）
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR | 
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN,		// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,			// 移行可能属性
	},
	// フリッパー
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// スプリングカタパルト確保
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// スプリングカタパルト↑
	{
		// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |		// 即振り向き可能.
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP,			// ジャンプ時移動可
		// 移行可能属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,			// ホーミング可
	},
	// スプリングカタパルト↑以外
	{
		// 各種チェック属性
			0,
		// 移行可能属性
			0,
	},

	// 強制スピン
	{
		// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC,	//	スピン減速無し
		// 移行可能属性
		0,
	},
	// 強制スピン 減速タイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN,				// 各種チェック属性
		0,													// 移行可能属性
	},
	// 強制スピン 落下
	{
		0,												// 各種チェック属性
		0,												// 移行可能属性
	},
	// 移動歯車
	{
		//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_STAND |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_NOSET,		// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// 排液装置
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// 排液装置（落下）
	{
		0,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,	// 移行可能属性
	},
	// スチームパイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_NOSET,	// 各種チェック属性
		0,												// 移行可能属性
	},
	// ポップスチーム
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,		// 移行可能属性
	},
	// スペステリングＩＮ
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// ボス2掴み
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// ボスFINAL地球割り着地振動
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// エンディング１
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// エンディング２
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// トロッコ危険状態
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,			// 各種チェック属性
		0,											// 移行可能属性
	},
	// トロッコ危険復帰
	{
		0,			// 各種チェック属性
		0,			// 移行可能属性
	},
	// スピン状態で落下
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,													// 移行可能属性
	},
  },

  // ========================================================================
  // トロッコスーパーソニック
  // ========================================================================
  {
	// FW
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_LOOKUP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT,			// 移行可能属性
	},

	// 歩き
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_WALK_END |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// 振り向き
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// 見上げ開始
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_END_LOOKUP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 見上げ中
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_END_LOOKUP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// 見上げ終了
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,				// 移行可能属性
	},

	// しゃがみ開始
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		0,													// 移行可能属性
	},

	// しゃがみ中
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SQUAT |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,				// 各種チェック属性
		0,													// 移行可能属性
	},

	// しゃがみ終了
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,					// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP |
			GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT,			// 移行可能属性
	},

	// ブレーキ
	{
		//GMD_PLY_SEQ_STATE_CHECK_ATTR_END_BRAKE |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK,			// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},

	// スピン
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// スピンダッシュ加速
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// スピンダッシュ
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// よろける 前
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// よろける 後ろ
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// よろける 危険
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// 落下
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,													// 移行可能属性
	},

	// ジャンプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,													// 移行可能属性
	},

	// 押す
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ホーミング
	{
		0/*GMD_PLY_SEQ_STATE_CHECK_ATTR_END_HOMING*/,		// 各種チェック属性
		0,													// 移行可能属性
	},

	// ホーミング跳ね返り
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ジャンプダッシュ
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ダメージ
	{
		0,													// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,			// 移行可能属性
	},

	// 死亡
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// スーパーソニック化
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ボスゴール(カプセル開放後)
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// ボスFINAL演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// タイムアタックリトライ FW 演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},

	// タイムアタックリトライ その場加速 演出
	{
		0,													// 各種チェック属性
		0,													// 移行可能属性
	},


	/* 以下ギミック関連 */

	// スプリングジャンプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,				// 移行可能属性
	},
	// 岩乗り開始
	{
		0,	// 各種チェック属性
		
		0,		// 移行可能属性
	},
	// 岩乗り
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,		// 移行可能属性
	},
	// 滑車掴まり
	{
		0,													// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// 息継ぎ
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// ダッシュパネル
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL |
			//GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// ターザンロープ
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// ウォータースライダー
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// Ｓ字パイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// コークスクリュー
	{
//		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK |	// 歩きタイプモーション速度設定
//		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK |		// 歩き移動
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK,	// 各種チェック属性
		
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,	// 移行可能属性
	},
	// デモ用フットワーク
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},

	// ストッパー＠ピンボール
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// 大砲
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// 大砲発射
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// アップバンパー
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// シーソー
	{
		0,	// 各種チェック属性
		
		0,	// 移行可能属性
	},
	// ピンボール
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN,	// 各種チェック属性
		0,	// 移行可能属性	
	},
	// ピンボール（空中）
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR | 
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN,		// 各種チェック属性
		0,		// 移行可能属性
	},
	// フリッパー
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// スプリングカタパルト確保
	{
		NULL,												// 各種チェック属性
		NULL,												// 移行可能属性
	},
	// スプリングカタパルト↑
	{
		// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |		// 即振り向き可能.
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP,			// ジャンプ時移動可
		// 移行可能属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,			// ホーミング可
	},
	// スプリングカタパルト↑以外
	{
		// 各種チェック属性
			0,
		// 移行可能属性
			0,
	},
	// 強制スピン
	{
		// 各種チェック属性
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC,	//	スピン減速無し
		// 移行可能属性
		0,
	},
	// 強制スピン 減速タイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN,				// 各種チェック属性
		0,													// 移行可能属性
	},
	// 強制スピン 落下
	{
		0,												// 各種チェック属性
		0,												// 移行可能属性
	},
	// 移動歯車
	{
		//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_STAND |
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_NOSET,		// 各種チェック属性
		0,//GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP,					// 移行可能属性
	},
	// 排液装置
	{
		0,	// 各種チェック属性
		0,	// 移行可能属性
	},
	// 排液装置（落下）
	{
		0,	// 各種チェック属性
		GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING,	// 移行可能属性
	},
	// スチームパイプ
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_NOSET,	// 各種チェック属性
		0,												// 移行可能属性
	},
	// ポップスチーム
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,		// 移行可能属性
	},
	// スペステリングＩＮ
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// ボス2掴み
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// ボスFINAL地球割り着地振動
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// エンディング１
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// エンディング２
	{
		0,	// 各種チェック属性
		0,		// 移行可能属性
	},
	// トロッコ危険状態
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL,			// 各種チェック属性
		0,											// 移行可能属性
	},
	// トロッコ危険復帰
	{
		0,			// 各種チェック属性
		0,			// 移行可能属性
	},
	// スピン状態で落下
	{
		GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP |
			GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR,		// 各種チェック属性
		0,													// 移行可能属性
	},
  },



};

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// pt
/*!
 */
// ==========================================================================

//----- Local Functions -----------------------------------------------------



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
