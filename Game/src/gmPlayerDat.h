// ==========================================================================
/*!
  @file gmPlayerDat.h
  @brief プレイヤーデータ定義

  @author Ishizaki
                Copyright(c) 2009 Dimps

  $Id: gmPlayerDat.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * memo
 *
 *
 */

#ifndef GM_PLAYER_DAT_H_
#define GM_PLAYER_DAT_H_


//----- Include Files -------------------------------------------------------
#include "gsMainSys.h"

#if _WII
#include "gmPlyLod.h"
#endif

// データヘッダ

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
#define GMD_PLY_DAT_IVA_ENABLE	(0)			//!< IVAモーションデータ有効設定

#define GMD_PLY_DAT_MTN_NO_NULL	(0xFF)		//!< モーション無し定義


/// プレイヤーアクションステータス
typedef enum tag_GME_PLY_ACT_STATE {
	GME_PLY_ACT_STATE_INVALID	= -1,	//!< 無効アクションステータス

	GME_PLY_ACT_STATE_FW	= 0,		//!< FW
	GME_PLY_ACT_STATE_FW_EX,			//!< FW おまけ
	GME_PLY_ACT_STATE_WAIT_0_1,			//!< 待機 0-1
	GME_PLY_ACT_STATE_WAIT_0_2,			//!< 待機 0-2
	GME_PLY_ACT_STATE_WAIT_1_1,			//!< 待機 1-1
	GME_PLY_ACT_STATE_WAIT_1_2,			//!< 待機 1-2
	GME_PLY_ACT_STATE_WAIT_2_1,			//!< 待機 2-1
	GME_PLY_ACT_STATE_WAIT_2_2,			//!< 待機 2-2
	GME_PLY_ACT_STATE_TURN,				//!< 振り向き FW・歩き
	GME_PLY_ACT_STATE_TURN_RUN,			//!< 振り向き 走り
	GME_PLY_ACT_STATE_TURN_BRAKE,		//!< 振り向き ブレーキ中
	GME_PLY_ACT_STATE_LOOKUP1,			//!< 見上げ 開始
	GME_PLY_ACT_STATE_LOOKUP2,			//!< 見上げ 中
	GME_PLY_ACT_STATE_LOOKUP3,			//!< 見上げ 終了
	GME_PLY_ACT_STATE_SQUATDOWN1,		//!< しゃがみ 開始
	GME_PLY_ACT_STATE_SQUATDOWN2,		//!< しゃがみ 中
	GME_PLY_ACT_STATE_SQUATDOWN3,		//!< しゃがみ 終了
	GME_PLY_ACT_STATE_PUSH1,			//!< 押す
	GME_PLY_ACT_STATE_PUSH2,			//!< 押し中

	// 歩き系は順番固定
	GME_PLY_ACT_STATE_WALK,				//!< 歩き
	GME_PLY_ACT_STATE_RUN,				//!< 走り
	GME_PLY_ACT_STATE_DASH_1,			//!< ダッシュ1
	GME_PLY_ACT_STATE_DASH_2,			//!< ダッシュ2

	GME_PLY_ACT_STATE_BRAKE1,			//!< ブレーキ 開始		BRAKEタイプで範囲チェックあり
	GME_PLY_ACT_STATE_BRAKE2,			//!< ブレーキ 踏ん張り
	GME_PLY_ACT_STATE_BRAKE3,			//!< ブレーキ 戻り

	// GME_PLY_ACT_STATE_SPIN ～ GME_PLY_ACT_STATE_SPIN_DASH 範囲指定あり
	GME_PLY_ACT_STATE_SPIN,				//!< スピン状態
	GME_PLY_ACT_STATE_SPIN_SMALL,		//!< スピン状態 小さいタイプ
	GME_PLY_ACT_STATE_SPIN_SHIFT,		//!< スピン モデル移行
	GME_PLY_ACT_STATE_SPIN_ACC,			//!< スピン 加速
	GME_PLY_ACT_STATE_SPIN_DASH,		//!< スピンダッシュ

	GME_PLY_ACT_STATE_HOMING,			//!< ホーミング
	GME_PLY_ACT_STATE_HOMING_REF1,		//!< ホーミング跳ね返り1
//	GME_PLY_ACT_STATE_HOMING_REF2,		//!< ホーミング跳ね返り2
	GME_PLY_ACT_STATE_STAGGER_F,		//!< よろける前
	GME_PLY_ACT_STATE_STAGGER_B,		//!< よろける後
	GME_PLY_ACT_STATE_STAGGER_D,		//!< よろける危険

	GME_PLY_ACT_STATE_DAMAGE,			//!< ダメージ
	GME_PLY_ACT_STATE_DIE1,				//!< 死亡始まり
	GME_PLY_ACT_STATE_DIE2,				//!< 死亡ループ

	GME_PLY_ACT_STATE_JUMP_SPIN,		//!< スピンジャンプ
	GME_PLY_ACT_STATE_JUMP_FALL,		//!< ジャンプ通常落下
	GME_PLY_ACT_STATE_JUMP_FALL_TURN,	//!< ジャンプ通常落下中ターン
	GME_PLY_ACT_STATE_JUMP_FALL_R,		//!< ジャンプ一定角度以上時落下
	GME_PLY_ACT_STATE_JUMP_FALL_R_TURN,	//!< ジャンプ一定角度以上時落下中ターン

	GME_PLY_ACT_STATE_JUMP_G01,			//!< ギミックジャンプ 上昇ループ
	GME_PLY_ACT_STATE_JUMP_G02,			//!< ギミックジャンプ 下降開始
	GME_PLY_ACT_STATE_JUMP_G03,			//!< ギミックジャンプ 下降ループ

	GME_PLY_ACT_STATE_JUMP_GF01,		//!< ギミック前転ジャンプ 上昇>下降開始
	GME_PLY_ACT_STATE_JUMP_GF02,		//!< ギミック前転ジャンプ 下降ループ

	GME_PLY_ACT_STATE_CHANGE_S,			//!< スーパーソニック変身 開始1
	GME_PLY_ACT_STATE_CHANGE_01,		//!< スーパーソニック変身 開始2～溜め
	GME_PLY_ACT_STATE_CHANGE_01_1,		//!< スーパーソニック変身 溜めループ
	GME_PLY_ACT_STATE_CHANGE_01_2,		//!< スーパーソニック変身 溜め～変身
	GME_PLY_ACT_STATE_CHANGE_02,		//!< スーパーソニック変身 キメループ
	GME_PLY_ACT_STATE_GOAL_01,			//!< ゴール 開始
	GME_PLY_ACT_STATE_GOAL_02,			//!< ゴール ループ

	/* ギミックアクション */
	GME_PLY_ACT_STATE_GIMMICK_START,	//!< ギミックアクション開始

	GME_PLY_ACT_STATE_GMK_HANG = GME_PLY_ACT_STATE_GIMMICK_START,	//!< 滑車 ぶら下がり
	GME_PLY_ACT_STATE_GMK_HANG_F,		//!< 滑車 ぶら下がり 前移動
	GME_PLY_ACT_STATE_GMK_HANG_B,		//!< 滑車 ぶら下がり 後ろ移動
	GME_PLY_ACT_STATE_GMK_HANG_ACT,		//!< 滑車 ぶら下がり 反動
	GME_PLY_ACT_STATE_GMK_BALL_01,		//!< 大岩 移動
	GME_PLY_ACT_STATE_GMK_BALL_02,		//!< 大岩 おっとっと
	GME_PLY_ACT_STATE_GMK_BREATH,		//!< 息継ぎ
	GME_PLY_ACT_STATE_GMK_ROPE,			//!< ターザンロープ
	GME_PLY_ACT_STATE_GMK_ROPE_ST,		//!< ターザンロープ 停止
	GME_PLY_ACT_STATE_GMK_SLIDE,		//!< ウォータースライダー
	GME_PLY_ACT_STATE_GMK_HANG_ST,		//!< 滑車 ぶら下がり スタート
	GME_PLY_ACT_STATE_GMK_CANNON_SHOOT,	//!< 大砲 発射
	GME_PLY_ACT_STATE_GMK_BREATH_J,		//!< 息継ぎ ジャンプ中
	GME_PLY_ACT_STATE_GMK_TRUCK_FW,		//!< トロッコFW
	GME_PLY_ACT_STATE_GMK_TRUCK_FW_L,	//!< トロッコFW 左
	GME_PLY_ACT_STATE_GMK_TRUCK_RUN,	//!< トロッコ走り
	GME_PLY_ACT_STATE_GMK_TRUCK_RUN_L,	//!< トロッコ走り 左
	GME_PLY_ACT_STATE_GMK_TRUCK_DOWN,	//!< トロッコ着地
	GME_PLY_ACT_STATE_GMK_TRUCK_DANGER,	//!< トロッコ危険発生
	GME_PLY_ACT_STATE_GMK_TRUCK_DANGER_ST,	//!< トロッコ危険発生中
	GME_PLY_ACT_STATE_GMK_TRUCK_DANGER_RE,	//!< トロッコ危険回避

	GME_PLY_ACT_STATE_GMK_ENDING_FR1,	//!< エンディング手前向き１
	GME_PLY_ACT_STATE_GMK_ENDING_FR2,	//!< エンディング手前向き２
	GME_PLY_ACT_STATE_GMK_ENDING_FN11,	//!< エンディングフィニッシュ１
	GME_PLY_ACT_STATE_GMK_ENDING_FN12,	//!< エンディングフィニッシュ２
	GME_PLY_ACT_STATE_GMK_ENDING_FN21,	//!< エンディングフィニッシュ１type2
	GME_PLY_ACT_STATE_GMK_ENDING_FN22,	//!< エンディングフィニッシュ２type2
	GME_PLY_ACT_STATE_GMK_ENDING_FNS1,	//!< エンディングフィニッシュ@スーパーソニック１
	GME_PLY_ACT_STATE_GMK_ENDING_FNS2,	//!< エンディングフィニッシュ@スーパーソニック２


	GME_PLY_ACT_STATE_MAX		// 126を超える場合は要相談(ゴースト保持ステータス関連)

} GME_PLY_ACT_STATE;

//#define GMD_PLY_UNIQUE_SIZE ( GME_PLY_ACT_STATE_MAX - GME_PLY_ACT_STATE_UNIQUE_START )




/// プレイヤーパラメータ構造体
typedef struct _GMS_PLY_PARAMETER {
	fx32		spd_add;					//!< 通常 加速値
	fx32		spd_max;					//!< 通常 最大速度値
	fx32		spd_dec;					//!< 通常放置 減速値

	fx32		spd_spin;					//!< スピン 初速値
	fx32		spd_add_spin;				//!< スピン 加速値
	fx32		spd_max_spin;				//!< スピン 最大速度値
	fx32		spd_dec_spin;				//!< スピン放置 減速値

//	fx32		spd_max_boost;				//!< ブースト最大速度
//	fx32		spd_add_nitro;				//!< ニトロ 加速値
//	fx32		spd_max_nitro;				//!< ニトロ 最大速度値
//	fx32		spd_dec_nitro;				//!< ニトロ放置 減速値
//	fx32		spd_chk_nitro;				//!< ニトロダウン（解除

	fx32		spd_max_add_slope;			//!< 坂道時の最大速度アップ値

	s16			time_air;					//!< 空気の持ち時間(水中耐久時間)
	s16			time_damage;				//!< ダメージ後の無敵時間

	s16			pool_max;					//!< ブースト溜め時間
	u16			fall_wait_time;				//!< 落下待ち時間(u8)

	fx32		spd_slope;					//!< (obj)傾斜加速度
	fx32		spd_slope_max;				//!< (obj)傾斜最大速度
	fx32		spd_slope_spin;				//!< (obj)傾斜スピン時加速度
	fx32		spd_slope_spin_spipe;		//!< (obj)Ｓ字パイプ傾斜スピン時加速度
	fx32		spd_slope_spin_pinball;		//!< (obj)ピンボール傾斜スピン時加速度

	fx32		spd_jump;					//!< ジャンプ速度
	fx32		spd_fall;					//!< (obj)落下加速度
	fx32		spd_fall_max;				//!< (obj)落下最大速度

	fx32		push_max;					//!< (obj)押し最大速度

	fx32		spd_jump_add;				//!< ジャンプ横方向 加速値
	fx32		spd_jump_max;				//!< ジャンプ横方向 最大速度値
	fx32		spd_jump_dec;				//!< ジャンプ横方向 減速値

	fx32		spd_add_spin_pinball;		//!< ピンボールスピン 加速値
	fx32		spd_max_spin_pinball;		//!< ピンボールスピン 最大速度値
	fx32		spd_dec_spin_pinball;		//!< ピンボールスピン放置 減速値
	fx32		spd_max_add_slope_spin_pinball;	//!< ピンボールスピン坂道時の最大速度アップ値
} GMS_PLY_PARAMETER;


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- External Variables --------------------------------------------------
/* プレイヤーアクション設定データ */
//extern const u8 *g_gm_player_action_tbl[GSD_CHAR_ID_MAX];			//!< プレイヤー矩形用アクションIDリストテーブル
//extern const u8 *g_gm_player_action3d_ca_tbl[GSD_CHAR_ID_MAX];		//!< プレイヤーCAモーションIDリストテーブル

/// プレイヤーモーションテーブル
extern const u8 *g_gm_player_motion_right_tbl[GSD_CHAR_ID_MAX];
extern const u8 *g_gm_player_motion_left_tbl[GSD_CHAR_ID_MAX];
/// プレイヤーモデルテーブル
extern const u8 *g_gm_player_model_tbl[GSD_CHAR_ID_MAX];

/// モーションブレンド設定
//extern const u8 g_gm_player_mtn_blend_setting[];
extern const u8 *g_gm_player_mtn_blend_setting_tbl[GSD_CHAR_ID_MAX];

/// プレイヤーパラメータ
extern const GMS_PLY_PARAMETER g_gm_player_parameter[GSD_CHAR_ID_MAX];


#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_PLAYER_DAT_H_

//----- Include Files -------------------------------------------------------
