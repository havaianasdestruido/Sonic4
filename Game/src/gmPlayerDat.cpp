// ==========================================================================
/*!
  @file gmPlayerDat.cpp
  @brief プレイヤーデータ定義

  @author Ishizaki
                Copyright(c) 2009 Dimps

  $Id: gmPlayerDat.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * memo
 *
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gsMainSys.h"
#include "gmPlayer.h"
#include "gmPlayerDat.h"
#include "gmPlySeq.h"


// データヘッダ
//#include "common/model/SON_MTN.hmb"

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
// ==========================================================================
// 各種プレイヤースペック
// ==========================================================================
// 各ギア時の加速度 fx32
// ソニック
#define GMD_PL_SONIC_SPDAD			(0x00091) //(0x00068)	//(0x00180)							// 09/06/11変更
#define GMD_PL_SONIC_SPDDO			(0x00400)
#define GMD_PL_SONIC_SPDMA			(0x09000)
//#define GMD_PL_SONIC_BOOST_SPDMA	(0x0a000)
//#define GMD_PL_SONIC_NITRO_SPDAD	(0x00300)
//#define GMD_PL_SONIC_NITRO_SPDDO	(0x00500)
//#define GMD_PL_SONIC_NITRO_SPDMA	(0x0c000)
//#define GMD_PL_SONIC_NITRO_CHECK	(GMD_PL_SONIC_NITRO_SPDMA >> 1)
#define GMD_PL_SONIC_SLOPE_SPD		(0x02000)		//!< 坂道時の最大速度アップ値
#if _IPHONE
#define GMD_PL_SONIC_SLOPE_SPD_TRUCK	(0x06000)		//!< 坂道時の最大速度アップ値 トロッコ
#else
#define GMD_PL_SONIC_SLOPE_SPD_TRUCK	(0x08000)		//!< 坂道時の最大速度アップ値 トロッコ
#endif // _IPHONE
// スーパーソニック
#define GMD_PL_SSONIC_SPDAD			((fx32)(0x00091*3.0)) //(0x00068)	//(0x00180)							// 09/06/11変更
#define GMD_PL_SSONIC_SPDDO			((fx32)(0x00400*2.0))
#define GMD_PL_SSONIC_SPDMA			(0xF000)
#define GMD_PL_SSONIC_SLOPE_SPD		((fx32)(0x02000*1.5))		//!< 坂道時の最大速度アップ値
#define GMD_PL_SSONIC_SLOPE_SPD_PINBALL		((fx32)(0x02000))	//!< 坂道時の最大速度アップ値 (ピンボール時)
#if _IPHONE
#define GMD_PL_SSONIC_SLOPE_SPD_TRUCK		((fx32)(0x07800))		//!< 坂道時の最大速度アップ値 トロッコ
#else
#define GMD_PL_SSONIC_SLOPE_SPD_TRUCK		((fx32)(0x0A000))		//!< 坂道時の最大速度アップ値 トロッコ
#endif // _IPHONE
// トロッコソニック
#define GMD_PL_SONIC_SPDAD_TRUCK	(0x00091)
#define GMD_PL_SONIC_SPDDO_TRUCK	(0x00200)		//!< トロッコ用減速度
#define GMD_PL_SONIC_SPDMA_TRUCK	(0x06000)
// トロッコスーパーソニック
#define GMD_PL_SSONIC_SPDAD_TRUCK	((fx32)(0x00091*3.0))
#define GMD_PL_SSONIC_SPDDO_TRUCK	(0x00200)		//!< トロッコ用減速度
#define GMD_PL_SSONIC_SPDMA_TRUCK	(0xA000)

/* スピンダッシュ */
// ソニック
#define GMD_PL_SONIC_SPIN_SPD		(0x03000)	//!< Spin加速、初速値
#define GMD_PL_SONIC_SPIN_SPDAD		(0x02000)	//!< Spin加速、加速値
#define GMD_PL_SONIC_SPIN_SPDMA		(0x0A000)//(0x08800)	//!< Spin加速、最大値
#define GMD_PL_SONIC_SPIN_SPDDO		(0x00080)	//!< Spin加速、減速値
// スーパーソニック
#define GMD_PL_SSONIC_SPIN_SPD		((fx32)(0x03000*3.0))	//!< Spin加速、初速値
#define GMD_PL_SSONIC_SPIN_SPDAD	((fx32)(0x02000*3.0))	//!< Spin加速、加速値
#define GMD_PL_SSONIC_SPIN_SPDMA	(0xF000)				//!< Spin加速、最大値
#define GMD_PL_SSONIC_SPIN_SPDDO	((fx32)(0x00080*1.0))	//!< Spin加速、減速値


/* ピンボールスピンダッシュ */
// ソニック
#define GMD_PL_SONIC_SPIN_PINBALL_SPDAD			(0x00091)				//!< ピンボールSpin加速、加速値
#define GMD_PL_SONIC_SPIN_PINBALL_SPDMA			(0x09000)				//!< ピンボールSpin加速、最大値
#define GMD_PL_SONIC_SPIN_PINBALL_SPDDO			(0x00080)				//!< ピンボールSpin加速、減速値
#define GMD_PL_SONIC_SPIN_PINBALL_SLOPE_SPD		((fx32)(0x03000))		//!< ピンボールSpin 坂道時の最大速度アップ値
// スーパーソニック
#define GMD_PL_SSONIC_SPIN_PINBALL_SPDAD		((fx32)(0x00091*3.0))	//!< ピンボールSpin加速、加速値
#define GMD_PL_SSONIC_SPIN_PINBALL_SPDMA		(0xF000)				//!< ピンボールSpin加速、最大値
#define GMD_PL_SSONIC_SPIN_PINBALL_SPDDO		((fx32)(0x00080))	//!< ピンボールSpin加速、減速値
#define GMD_PL_SSONIC_SPIN_PINBALL_SLOPE_SPD	((fx32)(0x03000*1.5))	//!< ピンボールSpin坂道時の最大速度アップ値



/* ジャンプ初速度 */
// ソニック
#define GMD_PL_JUMP_SPD				(0x5A5A)	// (0x6800*3/4)
//define GMD_PL_JUMP_WATER_SPD		(0x3800*3/4)
// スーパーソニック
#define GMD_PL_SSONIC_JUMP_SPD		(0x7FBF)
// スペステソニック
#define GMD_PL_SPL_JUMP_SPD			(0x4000)//(0x4800)
// トロッコソニック
#define GMD_PL_TRUCK_JUMP_SPD			(0x5A5A*5/6)
// トロッコスーパーソニック
#define GMD_PL_TRUCK_SSONIC_JUMP_SPD	(0x7FBF*5/6)

/* ジャンプ横方向速度 */
// ソニック
#define GMD_PL_SONIC_JUMP_SPDAD		(0x00100)
#define GMD_PL_SONIC_JUMP_SPDDO		(0x00800)	// (0x00200) (0x00400)
#define GMD_PL_SONIC_JUMP_SPDMA		(0x09000)
// スーパーソニック
#define GMD_PL_SSONIC_JUMP_SPDAD	((fx32)(0x00100*3.0))
#define GMD_PL_SSONIC_JUMP_SPDDO	((fx32)(0x00800*2.0))
#define GMD_PL_SSONIC_JUMP_SPDMA	(0xF000)
// トロッコソニック
#define GMD_PL_SONIC_JUMP_SPDAD_TRUCK	(0x00100)
#define GMD_PL_SONIC_JUMP_SPDDO_TRUCK	(0x00200)
#define GMD_PL_SONIC_JUMP_SPDMA_TRUCK	(0x09000)
// トロッコスーパーソニック
#define GMD_PL_SSONIC_JUMP_SPDAD_TRUCK	((fx32)(0x00100*3.0))
#define GMD_PL_SSONIC_JUMP_SPDDO_TRUCK	((fx32)(0x00200*2.0))
#define GMD_PL_SSONIC_JUMP_SPDMA_TRUCK	(0xF000)


// 落下加速度
#define GMD_PL_FALL_SPDAD			(0x002A8)	// (0x002a0)
#define GMD_PL_FALL_WATER_SPDAD		(0x000c0)
#define GMD_PL_FALL_SPDMA			(0x0f000)
// スペステソニック
#define GMD_PL_SPL_FALL_SPDAD		(0x00138)//(0x00170)
#define GMD_PL_SPL_FALL_SPDMA		(0x06000)


// 落下待ち時間
#define GMD_PL_SONIC_FALL_TIME			( 24 )
#if _IPHONE
#define GMD_PL_SONIC_FALL_TIME_TRUCK	( 240 )		//!< トロッコ用
#else
#define GMD_PL_SONIC_FALL_TIME_TRUCK	( 120 )		//!< トロッコ用
#endif // _IPHONE

/* 傾斜速度 */
// ソニック
#if 0
#define GMD_PL_SONIC_KEI_SPD		(0x00100)//(0x00200*3/4)// (0x0020*3/4)		// 09/06/11変更
#define GMD_PL_SONIC_KEI_SPDMA		(0x0f000)
#define GMD_PL_SONIC_KEI_SPD_SPIN	(0x00500*3/4)
#else
#define GMD_PL_SONIC_KEI_SPD		(0x00100*3/4)//(0x00200*3/4)// (0x0020*3/4)		// 09/06/11変更
#define GMD_PL_SONIC_KEI_SPDMA		(0x0D000)	// 0x0F000
#define GMD_PL_SONIC_KEI_SPD_SPIN	(0x00500*4/8)//(0x00500*3/4)
#define GMD_PL_SONIC_KEI_SPD_SPIN_S	(0x00500)//(0x00500*3/4)
#define GMD_PL_SONIC_KEI_SPD_P_SPIN	(0x00500)//(0x00500*3/4)		// ピンボール時
#define GMD_PL_SONIC_KEI_SPD_TRUCK	(0x00200)//(0x00200)		// トロッコ時
#define GMD_PL_SONIC_KEI_SPDMA_TRUCK	(0x0A000)//(0x0D000)	// 0x0F000
#endif
// スーパーソニック
#define GMD_PL_SSONIC_KEI_SPD			((fx32)(0x00100*3/4*1.5))
#define GMD_PL_SSONIC_KEI_SPDMA			(0xF000)
#define GMD_PL_SSONIC_KEI_SPD_SPIN		((fx32)(0x00500*4/8*1.5))
#define GMD_PL_SSONIC_KEI_SPD_SPIN_S	((fx32)(0x00500))
#define GMD_PL_SSONIC_KEI_SPD_P_SPIN	((fx32)(0x00500))		// ピンボール時
#define GMD_PL_SSONIC_KEI_SPD_SPIN_FOR_PINBALL	((fx32)(0x00500*4/8))	// ピンボールの時の通常スピン
#define GMD_PL_SSONIC_KEI_SPD_TRUCK		(0x00200)		// トロッコ時
#define GMD_PL_SSONIC_KEI_SPDMA_TRUCK	(0x0A000)//(0xF000)	// 0x0F000
// スペステソニック
#define GMD_PL_SPL_KEI_SPDMA		(0x05000)	// 0x0F000
#define GMD_PL_SPL_KEI_SPD			(0x00200)		// 傾斜速度

// 空気が持つ時間(frame) 水中耐久時間
#define GMD_PL_SONIC_AIR_COUNT (30*60) // 実質30秒

// ダメージ中の無敵時間
#define GMD_PL_SONIC_DAMAGE_TIME (180)

// ニトロ時 ブーストに達するための時間(超過速度蓄積量)
#define GMD_PL_SONIC_POOL_TIME ( 96 )

/* 押し速度 */
// ソニック
#define GMD_PL_SONIC_PUSH_SPD		(0x01c00)
// スーパーソニック
#define GMD_PL_SSONIC_PUSH_SPD		((fx32)(0x01c00*3.0))

/// プレイヤーモーションIDリスト
typedef enum tag_GME_PLY_MTN_ID {
	GME_PLY_MTN_ID_BRAKE01		= 0,
	GME_PLY_MTN_ID_BRAKE02,
	GME_PLY_MTN_ID_BRAKE03,
	GME_PLY_MTN_ID_CHANGE_01,
	GME_PLY_MTN_ID_CHANGE_01_1,
	GME_PLY_MTN_ID_CHANGE_01_2,
	GME_PLY_MTN_ID_CHANGE_02,
	GME_PLY_MTN_ID_DAMAGE,
	GME_PLY_MTN_ID_DASH1,
	GME_PLY_MTN_ID_DASH2,
	GME_PLY_MTN_ID_DIE_01,
	GME_PLY_MTN_ID_DIE_02,
	GME_PLY_MTN_ID_FW,
	GME_PLY_MTN_ID_FWEX,
	GME_PLY_MTN_ID_FWWAIT0_01,
	GME_PLY_MTN_ID_FWWAIT0_02,
	GME_PLY_MTN_ID_FWWAIT1_01,
	GME_PLY_MTN_ID_FWWAIT1_02,
	GME_PLY_MTN_ID_FWWAIT2_01,
	GME_PLY_MTN_ID_FWWAIT2_02,
	GME_PLY_MTN_ID_GOAL_01,
	GME_PLY_MTN_ID_GOAL_02,
	GME_PLY_MTN_ID_JUMP_F,
	GME_PLY_MTN_ID_JUMP_S_01,
	GME_PLY_MTN_ID_JUMP_S_02,
	GME_PLY_MTN_ID_JUMP_S_03,
	GME_PLY_MTN_ID_LOOKUP_01,
	GME_PLY_MTN_ID_LOOKUP_02,
	GME_PLY_MTN_ID_LOOKUP_03,
	GME_PLY_MTN_ID_RUN,
	GME_PLY_MTN_ID_SPIN01,
	GME_PLY_MTN_ID_SPIN02,
	GME_PLY_MTN_ID_SPIN,
	GME_PLY_MTN_ID_SQUAT_01,
	GME_PLY_MTN_ID_SQUAT_02,
	GME_PLY_MTN_ID_SQUAT_03,
	GME_PLY_MTN_ID_ST_B,
	GME_PLY_MTN_ID_ST_D,
	GME_PLY_MTN_ID_ST_F,
	GME_PLY_MTN_ID_TURN,
	GME_PLY_MTN_ID_TURN_BRAKE,
	GME_PLY_MTN_ID_TURN_RUN,
	GME_PLY_MTN_ID_WALK,
	GME_PLY_MTN_ID_WALL_01,
	GME_PLY_MTN_ID_WALL_02,
	GME_PLY_MTN_ID_HANG,
	GME_PLY_MTN_ID_HANG_B,
	GME_PLY_MTN_ID_HANG_F,
	GME_PLY_MTN_ID_BALL_01,
	GME_PLY_MTN_ID_BALL_02,
	GME_PLY_MTN_ID_BREATH,
	GME_PLY_MTN_ID_ROPE,
	GME_PLY_MTN_ID_ROPE_ST,
	GME_PLY_MTN_ID_SLIDE,
	GME_PLY_MTN_ID_SPIN_N,		// 通常モデル用スピン
	GME_PLY_MTN_ID_SPIN_G,		// 通常モデル地面用スピン
	GME_PLY_MTN_ID_SPIN_B,		// 通常モデル地面用スピン 丸球ブラーエフェクトつき
	GME_PLY_MTN_ID_FALL,		// 通常落下
	GME_PLY_MTN_ID_FALL_TURN_L,	// 通常落下ターン 内部データは右向き用だが、システム上左向き用として使用
	GME_PLY_MTN_ID_FALL_R,		// 一定角度以上の落下
	GME_PLY_MTN_ID_FALL_R_TURN_L,	// 一定角度以上の落下ターン 内部データは右向き用だが、システム上左向き用として使用
	GME_PLY_MTN_ID_HANG_ST,
	GME_PLY_MTN_ID_HANG_ACT,
	GME_PLY_MTN_ID_BREATH_J,

	GME_PLY_MTN_ID_TRUCK_RUN,
	GME_PLY_MTN_ID_TRUCK_JP,
	GME_PLY_MTN_ID_TRUCK_DOWN,
	GME_PLY_MTN_ID_TRUCK_ST,	// トロッコFW
	GME_PLY_MTN_ID_TRUCK_DANGER,
	GME_PLY_MTN_ID_TRUCK_DANGER_ST,
	GME_PLY_MTN_ID_TRUCK_DANGER_RE,

	GME_PLY_MTN_ID_ENDING_FR1,	// エンディング手前を見るまで
	GME_PLY_MTN_ID_ENDING_FR2,	// エンディング手前を見る
	GME_PLY_MTN_ID_ENDING_FN11,	// エンディングフィニッシュ１
	GME_PLY_MTN_ID_ENDING_FN12,	// エンディングフィニッシュ２(キメ)
	GME_PLY_MTN_ID_ENDING_FN21,	// エンディングフィニッシュ１ type2
	GME_PLY_MTN_ID_ENDING_FN22,	// エンディングフィニッシュ２ type2(キメ)

	// 左向き
	GME_PLY_MTN_ID_DASH2_L,
	GME_PLY_MTN_ID_DAMAGE_L,
	GME_PLY_MTN_ID_FW_L,
	GME_PLY_MTN_ID_TURN_L,
	GME_PLY_MTN_ID_SQUAT_02_L,
	GME_PLY_MTN_ID_SQUAT_03_L,
	GME_PLY_MTN_ID_BRAKE01_L,
	GME_PLY_MTN_ID_BRAKE02_L,
	GME_PLY_MTN_ID_BRAKE03_L,
	GME_PLY_MTN_ID_TURN_BRAKE_L,
	GME_PLY_MTN_ID_LOOKUP_01_L,
	GME_PLY_MTN_ID_LOOKUP_02_L,
	GME_PLY_MTN_ID_LOOKUP_03_L,
	GME_PLY_MTN_ID_BREATH_L,
	GME_PLY_MTN_ID_CHANGE_01_L,
	GME_PLY_MTN_ID_CHANGE_01_1_L,
	GME_PLY_MTN_ID_CHANGE_01_2_L,
	GME_PLY_MTN_ID_CHANGE_02_L,
	GME_PLY_MTN_ID_TRUCK_RUN_L,
	GME_PLY_MTN_ID_TRUCK_ST_L,	// トロッコFW
	GME_PLY_MTN_ID_FALL_L,
	GME_PLY_MTN_ID_FALL_TURN,	// 内部データは左向き用だが、システム上右向き用として使用
	GME_PLY_MTN_ID_FALL_R_L,
	GME_PLY_MTN_ID_FALL_R_TURN,	// 内部データは左向き用だが、システム上右向き用として使用
	GME_PLY_MTN_ID_BREATH_J_L,

	// スーパーソニック
	GME_PLY_MTN_ID_SSON_DASH1,
	GME_PLY_MTN_ID_SSON_FW,
	GME_PLY_MTN_ID_SSON_FWWAIT1_01,
	GME_PLY_MTN_ID_SSON_FWWAIT1_02,
	GME_PLY_MTN_ID_SSON_JUMP_F,
	GME_PLY_MTN_ID_SSON_JUMP_S_01,
	GME_PLY_MTN_ID_SSON_JUMP_S_02,
	GME_PLY_MTN_ID_SSON_JUMP_S_03,
	GME_PLY_MTN_ID_SSON_LOOKUP_01,
	GME_PLY_MTN_ID_SSON_LOOKUP_02,
	GME_PLY_MTN_ID_SSON_LOOKUP_03,
	GME_PLY_MTN_ID_SSPIN_N,			// 通常モデル用スピン
	GME_PLY_MTN_ID_SSPIN_G,			// 通常モデル地面用スピン
	GME_PLY_MTN_ID_SSPIN_B,			// 通常モデル地面用スピン 丸球ブラーエフェクトつき
	GME_PLY_MTN_ID_SSON_SQUAT_01,
	GME_PLY_MTN_ID_SSON_SQUAT_02,
	GME_PLY_MTN_ID_SSON_SQUAT_03,
	GME_PLY_MTN_ID_SSON_TURN,
	GME_PLY_MTN_ID_ENDING_FNS1,	// エンディングフィニッシュ@スーパーソニック１
	GME_PLY_MTN_ID_ENDING_FNS2,	// エンディングフィニッシュ@スーパーソニック２(キメ)

	// スーパーソニック 左向き
	GME_PLY_MTN_ID_SSON_DASH1_L,
	GME_PLY_MTN_ID_SSON_FW_L,
	GME_PLY_MTN_ID_SSON_SQUAT_02_L,
	GME_PLY_MTN_ID_SSON_SQUAT_03_L,
	GME_PLY_MTN_ID_SSON_TURN_L,
	GME_PLY_MTN_ID_SSON_LOOKUP_01_L,
	GME_PLY_MTN_ID_SSON_LOOKUP_02_L,
	GME_PLY_MTN_ID_SSON_LOOKUP_03_L,

	GME_PLY_MTN_ID_MAX

} GME_PLY_MTN_ID;

/// プレイヤーモデルID
typedef enum tag_GME_PLY_MODEL_ID {
	GME_PLY_MODEL_ID_NORMAL	= 0,
	GME_PLY_MODEL_ID_SPIN,
#if _IPHONE
	GME_PLY_MODEL_ID_IPHONE,
	GME_PLY_MODEL_ID_SPINJUMP,
#endif // _IPHONE

	GME_PLY_MODEL_ID_MAX
} GME_PLY_MODEL_ID;

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------
//===========================================================================
// プレイヤーモーションIDリスト
//===========================================================================
/* 右 */
/// ソニックモーションリスト 右
const u8 gm_player_motion_list_son_right[GME_PLY_ACT_STATE_MAX] = {

	GME_PLY_MTN_ID_FW,			// FW
	GME_PLY_MTN_ID_FWEX,		// FW おまけ
	GME_PLY_MTN_ID_FWWAIT0_01,	// 待機 0-1
	GME_PLY_MTN_ID_FWWAIT0_02,	// 待機 0-2
	GME_PLY_MTN_ID_FWWAIT1_01,	// 待機 1-1
	GME_PLY_MTN_ID_FWWAIT1_02,	// 待機 1-2
	GME_PLY_MTN_ID_FWWAIT2_01,	// 待機 2-1
	GME_PLY_MTN_ID_FWWAIT2_02,	// 待機 2-2
	GME_PLY_MTN_ID_TURN,		// 振り向き FW・歩き
	GME_PLY_MTN_ID_TURN_RUN,	// 振り向き 走り
	GME_PLY_MTN_ID_TURN_BRAKE,	// 振り向き ブレーキ中
	GME_PLY_MTN_ID_LOOKUP_01,	// 見上げ 開始
	GME_PLY_MTN_ID_LOOKUP_02,	// 見上げ 中
	GME_PLY_MTN_ID_LOOKUP_03,	// 見上げ 終了
	GME_PLY_MTN_ID_SQUAT_01,	// しゃがみ 開始
	GME_PLY_MTN_ID_SQUAT_02,	// しゃがみ 中
	GME_PLY_MTN_ID_SQUAT_03,	// しゃがみ 終了
	GME_PLY_MTN_ID_WALL_01,		// 押す
	GME_PLY_MTN_ID_WALL_02,		// 押す中

	GME_PLY_MTN_ID_WALK,		// 歩き
	GME_PLY_MTN_ID_RUN,			// 走り
	GME_PLY_MTN_ID_DASH1,		// ダッシュ1
	GME_PLY_MTN_ID_DASH2,		// ダッシュ2

	GME_PLY_MTN_ID_BRAKE01,		// ブレーキ 開始
	GME_PLY_MTN_ID_BRAKE02,		// ブレーキ 踏ん張り
	GME_PLY_MTN_ID_BRAKE03,		// ブレーキ 戻り
	GME_PLY_MTN_ID_SPIN_G,		// スピン状態
	GME_PLY_MTN_ID_SPIN_B,		// スピン状態 小さいタイプ
	GME_PLY_MTN_ID_SPIN01,		// スピン モデル移行
	GME_PLY_MTN_ID_SPIN01,//GME_PLY_MTN_ID_SPIN02,		// スピン加速
	GME_PLY_MTN_ID_SPIN01,		// スピンダッシュ
	GME_PLY_MTN_ID_SPIN,		// ホーミング
	//GME_PLY_MTN_ID_SPIN_N,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_JUMP_F,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_ST_F,		// よろける前
	GME_PLY_MTN_ID_ST_B,		// よろける後
	GME_PLY_MTN_ID_ST_D,		// よろける危険

	GME_PLY_MTN_ID_DAMAGE,		// ダメージ
	GME_PLY_MTN_ID_DIE_01,		// 死亡始まり
	GME_PLY_MTN_ID_DIE_02,		// 死亡ループ

	GME_PLY_MTN_ID_SPIN_N,		// スピンジャンプ
	GME_PLY_MTN_ID_FALL,		// ジャンプ通常落下
	GME_PLY_MTN_ID_FALL_TURN,	// ジャンプ通常落下中ターン
	GME_PLY_MTN_ID_FALL_R,		// ジャンプ一定角度以上時落下
	GME_PLY_MTN_ID_FALL_R_TURN,	// ジャンプ一定角度以上時落下中ターン

	GME_PLY_MTN_ID_JUMP_S_01,	// ギミックジャンプ 上昇ループ
	GME_PLY_MTN_ID_JUMP_S_02,	// ギミックジャンプ 下降開始
	GME_PLY_MTN_ID_JUMP_S_03,	// ギミックジャンプ 下降ループ

	GME_PLY_MTN_ID_JUMP_F,		// ギミック前転ジャンプ 上昇>下降開始
	GME_PLY_MTN_ID_FALL,		// ギミック前転ジャンプ 下降ループ

	GME_PLY_MTN_ID_SPIN_N,		// スーパーソニック変身 開始1
	GME_PLY_MTN_ID_CHANGE_01,	// スーパーソニック変身 開始2～溜め
	GME_PLY_MTN_ID_CHANGE_01_1,	// スーパーソニック変身 溜めループ
	GME_PLY_MTN_ID_CHANGE_01_2,	// スーパーソニック変身 溜め～変身
	GME_PLY_MTN_ID_CHANGE_02,	// スーパーソニック変身 キメループ
	GME_PLY_MTN_ID_GOAL_01,		// ゴール 開始
	GME_PLY_MTN_ID_GOAL_02,		// ゴール ループ

	GME_PLY_MTN_ID_HANG,		// 滑車 ぶら下がり
	GME_PLY_MTN_ID_HANG_F,		// 滑車 ぶら下がり 前移動
	GME_PLY_MTN_ID_HANG_B,		// 滑車 ぶら下がり 後ろ移動
	GME_PLY_MTN_ID_HANG_ACT,	// 滑車 ぶら下がり 反動
	GME_PLY_MTN_ID_BALL_01,		// 大岩 移動
	GME_PLY_MTN_ID_BALL_02,		// 大岩 おっとっと
	GME_PLY_MTN_ID_BREATH,		// 息継ぎ
	GME_PLY_MTN_ID_ROPE,		// ターザンロープ
	GME_PLY_MTN_ID_ROPE_ST,		// ターザンロープ 停止
	GME_PLY_MTN_ID_SLIDE,		// ウォータースライダー
	GME_PLY_MTN_ID_HANG_ST,		// 滑車 ぶら下がり スタート
	GME_PLY_MTN_ID_SPIN_N,		// 大砲 発射
	GME_PLY_MTN_ID_BREATH_J,	// 息継ぎ ジャンプ中
	GME_PLY_MTN_ID_TRUCK_ST,	// トロッコFW
	GME_PLY_MTN_ID_TRUCK_ST_L,	// トロッコFW 左
	GME_PLY_MTN_ID_TRUCK_RUN,	// トロッコ走り
	GME_PLY_MTN_ID_TRUCK_RUN_L,	// トロッコ走り 左
	GME_PLY_MTN_ID_TRUCK_DOWN,	// トロッコ着地
	GME_PLY_MTN_ID_TRUCK_DANGER,	// トロッコ危険発生
	GME_PLY_MTN_ID_TRUCK_DANGER_ST,	// トロッコ危険発生中
	GME_PLY_MTN_ID_TRUCK_DANGER_RE,	// トロッコ危険回避
	GME_PLY_MTN_ID_ENDING_FR1,	// エンディング手前を見るまで
	GME_PLY_MTN_ID_ENDING_FR2,	// エンディング手前を見る
	GME_PLY_MTN_ID_ENDING_FN11,	// エンディングフィニッシュ１
	GME_PLY_MTN_ID_ENDING_FN12,	// エンディングフィニッシュ２(キメ)
	GME_PLY_MTN_ID_ENDING_FN21,	// エンディングフィニッシュ１ type2
	GME_PLY_MTN_ID_ENDING_FN22,	// エンディングフィニッシュ２ type2(キメ)
	GME_PLY_MTN_ID_ENDING_FNS1,	// エンディングフィニッシュ@スーパーソニック１
	GME_PLY_MTN_ID_ENDING_FNS2,	// エンディングフィニッシュ@スーパーソニック２(キメ)
};

/// スーパーソニックモーションリスト 右
const u8 gm_player_motion_list_sson_right[GME_PLY_ACT_STATE_MAX] = {

	GME_PLY_MTN_ID_SSON_FW,			// FW
	GME_PLY_MTN_ID_FWEX,			// FW おまけ
	GME_PLY_MTN_ID_FWWAIT0_01,		// 待機 0-1
	GME_PLY_MTN_ID_FWWAIT0_02,		// 待機 0-2
	GME_PLY_MTN_ID_SSON_FWWAIT1_01,	// 待機 1-1
	GME_PLY_MTN_ID_SSON_FWWAIT1_02,	// 待機 1-2
	GME_PLY_MTN_ID_FWWAIT2_01,		// 待機 2-1
	GME_PLY_MTN_ID_FWWAIT2_02,		// 待機 2-2
	GME_PLY_MTN_ID_SSON_TURN,		// 振り向き FW・歩き
	GME_PLY_MTN_ID_TURN_RUN,		// 振り向き 走り
	GME_PLY_MTN_ID_TURN_BRAKE,		// 振り向き ブレーキ中
	GME_PLY_MTN_ID_SSON_LOOKUP_01,	// 見上げ 開始
	GME_PLY_MTN_ID_SSON_LOOKUP_02,	// 見上げ 中
	GME_PLY_MTN_ID_SSON_LOOKUP_03,	// 見上げ 終了
	GME_PLY_MTN_ID_SSON_SQUAT_01,	// しゃがみ 開始
	GME_PLY_MTN_ID_SSON_SQUAT_02,	// しゃがみ 中
	GME_PLY_MTN_ID_SSON_SQUAT_03,	// しゃがみ 終了
	GME_PLY_MTN_ID_WALL_01,			// 押す
	GME_PLY_MTN_ID_WALL_02,			// 押す中

	GME_PLY_MTN_ID_WALK,			// 歩き
	GME_PLY_MTN_ID_RUN,				// 走り
	GME_PLY_MTN_ID_SSON_DASH1,		// ダッシュ1
	GME_PLY_MTN_ID_SSON_DASH1,		// ダッシュ2

	GME_PLY_MTN_ID_BRAKE01,			// ブレーキ 開始
	GME_PLY_MTN_ID_BRAKE02,			// ブレーキ 踏ん張り
	GME_PLY_MTN_ID_BRAKE03,			// ブレーキ 戻り
	GME_PLY_MTN_ID_SSPIN_G,			// スピン状態
	GME_PLY_MTN_ID_SSPIN_B,			// スピン状態 小さいタイプ
	GME_PLY_MTN_ID_SPIN01,			// スピン モデル移行
	GME_PLY_MTN_ID_SPIN01,//GME_PLY_MTN_ID_SPIN02,			// スピン加速
	GME_PLY_MTN_ID_SPIN01,			// スピンダッシュ
	GME_PLY_MTN_ID_SPIN,			// ホーミング
	//GME_PLY_MTN_ID_SSPIN_N,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_SSON_JUMP_F,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_ST_F,			// よろける前
	GME_PLY_MTN_ID_ST_B,			// よろける後
	GME_PLY_MTN_ID_ST_D,			// よろける危険

	GME_PLY_MTN_ID_DAMAGE,			// ダメージ
	GME_PLY_MTN_ID_DIE_01,			// 死亡始まり
	GME_PLY_MTN_ID_DIE_02,			// 死亡ループ

	GME_PLY_MTN_ID_SSPIN_N,			// スピンジャンプ
	GME_PLY_MTN_ID_FALL,			// ジャンプ通常落下
	GME_PLY_MTN_ID_FALL_TURN,		// ジャンプ通常落下中ターン
	GME_PLY_MTN_ID_FALL_R,			// ジャンプ一定角度以上時落下
	GME_PLY_MTN_ID_FALL_R_TURN,		// ジャンプ一定角度以上時落下中ターン

	GME_PLY_MTN_ID_SSON_JUMP_S_01,	// ギミックジャンプ 上昇ループ
	GME_PLY_MTN_ID_SSON_JUMP_S_02,	// ギミックジャンプ 下降開始
	GME_PLY_MTN_ID_SSON_JUMP_S_03,	// ギミックジャンプ 下降ループ

	GME_PLY_MTN_ID_SSON_JUMP_F,		// ギミック前転ジャンプ 上昇>下降開始
	GME_PLY_MTN_ID_FALL,			// ギミック前転ジャンプ 下降ループ

	GME_PLY_MTN_ID_SPIN_N,			// スーパーソニック変身 開始1
	GME_PLY_MTN_ID_CHANGE_01,		// スーパーソニック変身 開始2～溜め
	GME_PLY_MTN_ID_CHANGE_01_1,		// スーパーソニック変身 溜めループ
	GME_PLY_MTN_ID_CHANGE_01_2,		// スーパーソニック変身 溜め～変身
	GME_PLY_MTN_ID_CHANGE_02,		// スーパーソニック変身 キメループ
	GME_PLY_MTN_ID_GOAL_01,			// ゴール 開始
	GME_PLY_MTN_ID_GOAL_02,			// ゴール ループ

	GME_PLY_MTN_ID_HANG,			// 滑車 ぶら下がり
	GME_PLY_MTN_ID_HANG_F,			// 滑車 ぶら下がり 前移動
	GME_PLY_MTN_ID_HANG_B,			// 滑車 ぶら下がり 後ろ移動
	GME_PLY_MTN_ID_HANG_ACT,		// 滑車 ぶら下がり 反動
	GME_PLY_MTN_ID_BALL_01,			// 大岩 移動
	GME_PLY_MTN_ID_BALL_02,			// 大岩 おっとっと
	GME_PLY_MTN_ID_BREATH,			// 息継ぎ
	GME_PLY_MTN_ID_ROPE,			// ターザンロープ
	GME_PLY_MTN_ID_ROPE_ST,			// ターザンロープ 停止
	GME_PLY_MTN_ID_SLIDE,			// ウォータースライダー
	GME_PLY_MTN_ID_HANG_ST,			// 滑車 ぶら下がり スタート
	GME_PLY_MTN_ID_SSPIN_N,			// 大砲 発射
	GME_PLY_MTN_ID_BREATH_J,		// 息継ぎ ジャンプ中
	GME_PLY_MTN_ID_TRUCK_ST,		// トロッコFW
	GME_PLY_MTN_ID_TRUCK_ST_L,		// トロッコFW 左
	GME_PLY_MTN_ID_TRUCK_RUN,		// トロッコ走り
	GME_PLY_MTN_ID_TRUCK_RUN_L,		// トロッコ走り 左
	GME_PLY_MTN_ID_TRUCK_DOWN,		// トロッコ着地
	GME_PLY_MTN_ID_TRUCK_DANGER,	// トロッコ危険発生
	GME_PLY_MTN_ID_TRUCK_DANGER_ST,	// トロッコ危険発生中
	GME_PLY_MTN_ID_TRUCK_DANGER_RE,	// トロッコ危険回避
	GME_PLY_MTN_ID_ENDING_FR1,		// エンディング手前を見るまで
	GME_PLY_MTN_ID_ENDING_FR2,		// エンディング手前を見る
	GME_PLY_MTN_ID_ENDING_FN11,		// エンディングフィニッシュ１
	GME_PLY_MTN_ID_ENDING_FN12,		// エンディングフィニッシュ２(キメ)
	GME_PLY_MTN_ID_ENDING_FN21,		// エンディングフィニッシュ１ type2
	GME_PLY_MTN_ID_ENDING_FN22,		// エンディングフィニッシュ２ type2(キメ)
	GME_PLY_MTN_ID_ENDING_FNS1,		// エンディングフィニッシュ@スーパーソニック１
	GME_PLY_MTN_ID_ENDING_FNS2,		// エンディングフィニッシュ@スーパーソニック２(キメ)
};

/// ピンボールソニックモーションリスト 右
const u8 gm_player_motion_list_pn_son_right[GME_PLY_ACT_STATE_MAX] = {

	GME_PLY_MTN_ID_SPIN_N,		// FW
	GME_PLY_MTN_ID_SPIN_N,		// FW おまけ
	GME_PLY_MTN_ID_SPIN_N,		// 待機 0-1
	GME_PLY_MTN_ID_SPIN_N,		// 待機 0-2
	GME_PLY_MTN_ID_SPIN_N,		// 待機 1-1
	GME_PLY_MTN_ID_SPIN_N,		// 待機 1-2
	GME_PLY_MTN_ID_SPIN_N,		// 待機 2-1
	GME_PLY_MTN_ID_SPIN_N,		// 待機 2-2
	GME_PLY_MTN_ID_SPIN_N,		// 振り向き FW・歩き
	GME_PLY_MTN_ID_SPIN_N,		// 振り向き 走り
	GME_PLY_MTN_ID_SPIN_N,		// 振り向き ブレーキ中
	GME_PLY_MTN_ID_SPIN_N,		// 見上げ 開始
	GME_PLY_MTN_ID_SPIN_N,		// 見上げ 中
	GME_PLY_MTN_ID_SPIN_N,		// 見上げ 終了
	GME_PLY_MTN_ID_SPIN_N,		// しゃがみ 開始
	GME_PLY_MTN_ID_SPIN_N,		// しゃがみ 中
	GME_PLY_MTN_ID_SPIN_N,		// しゃがみ 終了
	GME_PLY_MTN_ID_SPIN_N,		// 押す
	GME_PLY_MTN_ID_SPIN_N,		// 押す中

	GME_PLY_MTN_ID_SPIN_N,		// 歩き
	GME_PLY_MTN_ID_SPIN_N,		// 走り
	GME_PLY_MTN_ID_SPIN_N,		// ダッシュ1
	GME_PLY_MTN_ID_SPIN_N,		// ダッシュ2

	GME_PLY_MTN_ID_SPIN_N,		// ブレーキ 開始
	GME_PLY_MTN_ID_SPIN_N,		// ブレーキ 踏ん張り
	GME_PLY_MTN_ID_SPIN_N,		// ブレーキ 戻り
	GME_PLY_MTN_ID_SPIN_G,		// スピン状態
	GME_PLY_MTN_ID_SPIN_B,		// スピン状態 小さいタイプ
	GME_PLY_MTN_ID_SPIN01,		// スピン モデル移行
	GME_PLY_MTN_ID_SPIN01,//GME_PLY_MTN_ID_SPIN02,		// スピン加速
	GME_PLY_MTN_ID_SPIN01,		// スピンダッシュ
	GME_PLY_MTN_ID_SPIN_N,		// ホーミング
	//GME_PLY_MTN_ID_SPIN_N,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_SPIN_N,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_SPIN_N,		// よろける前
	GME_PLY_MTN_ID_SPIN_N,		// よろける後
	GME_PLY_MTN_ID_SPIN_N,		// よろける危険

	GME_PLY_MTN_ID_SPIN_N,		// ダメージ
	GME_PLY_MTN_ID_DIE_01,		// 死亡始まり
	GME_PLY_MTN_ID_DIE_02,		// 死亡ループ

	GME_PLY_MTN_ID_SPIN_N,		// スピンジャンプ
	GME_PLY_MTN_ID_SPIN_N,		// ジャンプ通常落下
	GME_PLY_MTN_ID_SPIN_N,		// ジャンプ通常落下中ターン
	GME_PLY_MTN_ID_SPIN_N,		// ジャンプ一定角度以上時落下
	GME_PLY_MTN_ID_SPIN_N,		// ジャンプ一定角度以上時落下中ターン

	GME_PLY_MTN_ID_SPIN_N,		// ギミックジャンプ 上昇ループ
	GME_PLY_MTN_ID_SPIN_N,		// ギミックジャンプ 下降開始
	GME_PLY_MTN_ID_SPIN_N,		// ギミックジャンプ 下降ループ

	GME_PLY_MTN_ID_SPIN_N,		// ギミック前転ジャンプ 上昇>下降開始
	GME_PLY_MTN_ID_SPIN_N,		// ギミック前転ジャンプ 下降ループ

	GME_PLY_MTN_ID_SPIN_N,		// スーパーソニック変身 開始1
	GME_PLY_MTN_ID_SPIN_N,		// スーパーソニック変身 開始2～溜め
	GME_PLY_MTN_ID_SPIN_N,		// スーパーソニック変身 溜めループ
	GME_PLY_MTN_ID_SPIN_N,		// スーパーソニック変身 溜め～変身
	GME_PLY_MTN_ID_SPIN_N,		// スーパーソニック変身 キメループ
	GME_PLY_MTN_ID_SPIN_N,		// ゴール 開始
	GME_PLY_MTN_ID_SPIN_N,		// ゴール ループ

	GME_PLY_MTN_ID_SPIN_N,		// 滑車 ぶら下がり
	GME_PLY_MTN_ID_SPIN_N,		// 滑車 ぶら下がり 前移動
	GME_PLY_MTN_ID_SPIN_N,		// 滑車 ぶら下がり 後ろ移動
	GME_PLY_MTN_ID_SPIN_N,		// 滑車 ぶら下がり 反動
	GME_PLY_MTN_ID_SPIN_N,		// 大岩 移動
	GME_PLY_MTN_ID_SPIN_N,		// 大岩 おっとっと
	GME_PLY_MTN_ID_SPIN_N,		// 息継ぎ
	GME_PLY_MTN_ID_SPIN_N,		// ターザンロープ
	GME_PLY_MTN_ID_SPIN_N,		// ターザンロープ 停止
	GME_PLY_MTN_ID_SPIN_N,		// ウォータースライダー
	GME_PLY_MTN_ID_SPIN_N,		// 滑車 ぶら下がり スタート
	GME_PLY_MTN_ID_SPIN_N,		// 大砲 発射
	GME_PLY_MTN_ID_SPIN_N,		// 息継ぎ ジャンプ中
	GME_PLY_MTN_ID_SPIN_N,		// トロッコFW
	GME_PLY_MTN_ID_SPIN_N,		// トロッコFW 左
	GME_PLY_MTN_ID_SPIN_N,		// トロッコ走り
	GME_PLY_MTN_ID_SPIN_N,		// トロッコ走り 左
	GME_PLY_MTN_ID_SPIN_N,		// トロッコ着地
	GME_PLY_MTN_ID_SPIN_N,		// トロッコ危険発生
	GME_PLY_MTN_ID_SPIN_N,		// トロッコ危険発生中
	GME_PLY_MTN_ID_SPIN_N,		// トロッコ危険回避
	GME_PLY_MTN_ID_SPIN_N,		// エンディング手前を見るまで
	GME_PLY_MTN_ID_SPIN_N,		// エンディング手前を見る
	GME_PLY_MTN_ID_SPIN_N,		// エンディングフィニッシュ１
	GME_PLY_MTN_ID_SPIN_N,		// エンディングフィニッシュ２(キメ)
	GME_PLY_MTN_ID_SPIN_N,		// エンディングフィニッシュ１ type2
	GME_PLY_MTN_ID_SPIN_N,		// エンディングフィニッシュ２ type2(キメ)
	GME_PLY_MTN_ID_SPIN_N,		// エンディングフィニッシュ@スーパーソニック１
	GME_PLY_MTN_ID_SPIN_N,		// エンディングフィニッシュ@スーパーソニック２(キメ)
};

/// ピンボールスーパーソニックモーションリスト 右
const u8 gm_player_motion_list_pn_sson_right[GME_PLY_ACT_STATE_MAX] = {

	GME_PLY_MTN_ID_SSPIN_N,		// FW
	GME_PLY_MTN_ID_SSPIN_N,		// FW おまけ
	GME_PLY_MTN_ID_SSPIN_N,		// 待機 0-1
	GME_PLY_MTN_ID_SSPIN_N,		// 待機 0-2
	GME_PLY_MTN_ID_SSPIN_N,		// 待機 1-1
	GME_PLY_MTN_ID_SSPIN_N,		// 待機 1-2
	GME_PLY_MTN_ID_SSPIN_N,		// 待機 2-1
	GME_PLY_MTN_ID_SSPIN_N,		// 待機 2-2
	GME_PLY_MTN_ID_SSPIN_N,		// 振り向き FW・歩き
	GME_PLY_MTN_ID_SSPIN_N,		// 振り向き 走り
	GME_PLY_MTN_ID_SSPIN_N,		// 振り向き ブレーキ中
	GME_PLY_MTN_ID_SSPIN_N,		// 見上げ 開始
	GME_PLY_MTN_ID_SSPIN_N,		// 見上げ 中
	GME_PLY_MTN_ID_SSPIN_N,		// 見上げ 終了
	GME_PLY_MTN_ID_SSPIN_N,		// しゃがみ 開始
	GME_PLY_MTN_ID_SSPIN_N,		// しゃがみ 中
	GME_PLY_MTN_ID_SSPIN_N,		// しゃがみ 終了
	GME_PLY_MTN_ID_SSPIN_N,		// 押す
	GME_PLY_MTN_ID_SSPIN_N,		// 押す中

	GME_PLY_MTN_ID_SSPIN_N,		// 歩き
	GME_PLY_MTN_ID_SSPIN_N,		// 走り
	GME_PLY_MTN_ID_SSPIN_N,		// ダッシュ1
	GME_PLY_MTN_ID_SSPIN_N,		// ダッシュ2

	GME_PLY_MTN_ID_SSPIN_N,		// ブレーキ 開始
	GME_PLY_MTN_ID_SSPIN_N,		// ブレーキ 踏ん張り
	GME_PLY_MTN_ID_SSPIN_N,		// ブレーキ 戻り
	GME_PLY_MTN_ID_SSPIN_G,		// スピン状態
	GME_PLY_MTN_ID_SSPIN_B,		// スピン状態 小さいタイプ
	GME_PLY_MTN_ID_SPIN01,		// スピン モデル移行
	GME_PLY_MTN_ID_SPIN01,//GME_PLY_MTN_ID_SPIN02,		// スピン加速
	GME_PLY_MTN_ID_SPIN01,		// スピンダッシュ
	GME_PLY_MTN_ID_SSPIN_N,		// ホーミング
	//GME_PLY_MTN_ID_SSPIN_N,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_SSPIN_N,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_SSPIN_N,		// よろける前
	GME_PLY_MTN_ID_SSPIN_N,		// よろける後
	GME_PLY_MTN_ID_SSPIN_N,		// よろける危険

	GME_PLY_MTN_ID_SSPIN_N,		// ダメージ
	GME_PLY_MTN_ID_DIE_01,		// 死亡始まり
	GME_PLY_MTN_ID_DIE_02,		// 死亡ループ

	GME_PLY_MTN_ID_SSPIN_N,		// スピンジャンプ
	GME_PLY_MTN_ID_SSPIN_N,		// ジャンプ通常落下
	GME_PLY_MTN_ID_SSPIN_N,		// ジャンプ通常落下中ターン
	GME_PLY_MTN_ID_SSPIN_N,		// ジャンプ一定角度以上時落下
	GME_PLY_MTN_ID_SSPIN_N,		// ジャンプ一定角度以上時落下中ターン

	GME_PLY_MTN_ID_SSPIN_N,		// ギミックジャンプ 上昇ループ
	GME_PLY_MTN_ID_SSPIN_N,		// ギミックジャンプ 下降開始
	GME_PLY_MTN_ID_SSPIN_N,		// ギミックジャンプ 下降ループ

	GME_PLY_MTN_ID_SSPIN_N,		// ギミック前転ジャンプ 上昇>下降開始
	GME_PLY_MTN_ID_SSPIN_N,		// ギミック前転ジャンプ 下降ループ

	GME_PLY_MTN_ID_SPIN_N,		// スーパーソニック変身 開始1
	GME_PLY_MTN_ID_SSPIN_N,		// スーパーソニック変身 開始2～溜め
	GME_PLY_MTN_ID_SSPIN_N,		// スーパーソニック変身 溜めループ
	GME_PLY_MTN_ID_SSPIN_N,		// スーパーソニック変身 溜め～変身
	GME_PLY_MTN_ID_SSPIN_N,		// スーパーソニック変身 キメループ
	GME_PLY_MTN_ID_SSPIN_N,		// ゴール 開始
	GME_PLY_MTN_ID_SSPIN_N,		// ゴール ループ

	GME_PLY_MTN_ID_SSPIN_N,		// 滑車 ぶら下がり
	GME_PLY_MTN_ID_SSPIN_N,		// 滑車 ぶら下がり 前移動
	GME_PLY_MTN_ID_SSPIN_N,		// 滑車 ぶら下がり 後ろ移動
	GME_PLY_MTN_ID_SSPIN_N,		// 滑車 ぶら下がり 反動
	GME_PLY_MTN_ID_SSPIN_N,		// 大岩 移動
	GME_PLY_MTN_ID_SSPIN_N,		// 大岩 おっとっと
	GME_PLY_MTN_ID_SSPIN_N,		// 息継ぎ
	GME_PLY_MTN_ID_SSPIN_N,		// ターザンロープ
	GME_PLY_MTN_ID_SSPIN_N,		// ターザンロープ 停止
	GME_PLY_MTN_ID_SSPIN_N,		// ウォータースライダー
	GME_PLY_MTN_ID_SSPIN_N,		// 滑車 ぶら下がり スタート
	GME_PLY_MTN_ID_SSPIN_N,		// 大砲 発射
	GME_PLY_MTN_ID_SSPIN_N,		// 息継ぎ ジャンプ中
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコFW
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコFW 左
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコ走り
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコ走り 左
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコ着地
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコ危険発生
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコ危険発生中
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコ危険回避
	GME_PLY_MTN_ID_SSPIN_N,		// エンディング手前を見るまで
	GME_PLY_MTN_ID_SSPIN_N,		// エンディング手前を見る
	GME_PLY_MTN_ID_SSPIN_N,		// エンディングフィニッシュ１
	GME_PLY_MTN_ID_SSPIN_N,		// エンディングフィニッシュ２(キメ)
	GME_PLY_MTN_ID_SSPIN_N,		// エンディングフィニッシュ１ type2
	GME_PLY_MTN_ID_SSPIN_N,		// エンディングフィニッシュ２ type2(キメ)
	GME_PLY_MTN_ID_SSPIN_N,		// エンディングフィニッシュ@スーパーソニック１
	GME_PLY_MTN_ID_SSPIN_N,		// エンディングフィニッシュ@スーパーソニック２(キメ)
};

/// トロッコソニックモーションリスト 右
const u8 gm_player_motion_list_tr_son_right[GME_PLY_ACT_STATE_MAX] = {

	GME_PLY_MTN_ID_FW,			// FW
	GME_PLY_MTN_ID_FWEX,		// FW おまけ
	GME_PLY_MTN_ID_FWWAIT0_01,	// 待機 0-1
	GME_PLY_MTN_ID_FWWAIT0_02,	// 待機 0-2
	GME_PLY_MTN_ID_FWWAIT1_01,	// 待機 1-1
	GME_PLY_MTN_ID_FWWAIT1_02,	// 待機 1-2
	GME_PLY_MTN_ID_FWWAIT2_01,	// 待機 2-1
	GME_PLY_MTN_ID_FWWAIT2_02,	// 待機 2-2
	GME_PLY_MTN_ID_TURN,		// 振り向き FW・歩き
	GME_PLY_MTN_ID_TURN_RUN,	// 振り向き 走り
	GME_PLY_MTN_ID_TURN_BRAKE,	// 振り向き ブレーキ中
	GME_PLY_MTN_ID_LOOKUP_01,	// 見上げ 開始
	GME_PLY_MTN_ID_LOOKUP_02,	// 見上げ 中
	GME_PLY_MTN_ID_LOOKUP_03,	// 見上げ 終了
	GME_PLY_MTN_ID_SQUAT_01,	// しゃがみ 開始
	GME_PLY_MTN_ID_SQUAT_02,	// しゃがみ 中
	GME_PLY_MTN_ID_SQUAT_03,	// しゃがみ 終了
	GME_PLY_MTN_ID_WALL_01,		// 押す
	GME_PLY_MTN_ID_WALL_02,		// 押す中

	GME_PLY_MTN_ID_TRUCK_RUN,	// 歩き
	GME_PLY_MTN_ID_TRUCK_RUN,	// 走り
	GME_PLY_MTN_ID_TRUCK_RUN,	// ダッシュ1
	GME_PLY_MTN_ID_TRUCK_RUN,	// ダッシュ2

	GME_PLY_MTN_ID_BRAKE01,		// ブレーキ 開始
	GME_PLY_MTN_ID_BRAKE02,		// ブレーキ 踏ん張り
	GME_PLY_MTN_ID_BRAKE03,		// ブレーキ 戻り
	GME_PLY_MTN_ID_SPIN_G,		// スピン状態
	GME_PLY_MTN_ID_SPIN_B,		// スピン状態 小さいタイプ
	GME_PLY_MTN_ID_SPIN01,		// スピン モデル移行
	GME_PLY_MTN_ID_SPIN01,//GME_PLY_MTN_ID_SPIN02,		// スピン加速
	GME_PLY_MTN_ID_SPIN01,		// スピンダッシュ
	GME_PLY_MTN_ID_SPIN,		// ホーミング
	//GME_PLY_MTN_ID_SPIN_N,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_JUMP_F,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_ST_F,		// よろける前
	GME_PLY_MTN_ID_ST_B,		// よろける後
	GME_PLY_MTN_ID_ST_D,		// よろける危険

	GME_PLY_MTN_ID_DAMAGE,		// ダメージ
	GME_PLY_MTN_ID_DIE_01,		// 死亡始まり
	GME_PLY_MTN_ID_DIE_02,		// 死亡ループ

	GME_PLY_MTN_ID_SPIN_N,		// スピンジャンプ
	GME_PLY_MTN_ID_TRUCK_JP,	// ジャンプ通常落下
	GME_PLY_MTN_ID_TRUCK_JP,	// ジャンプ通常落下中ターン
	GME_PLY_MTN_ID_TRUCK_JP,	// ジャンプ一定角度以上時落下
	GME_PLY_MTN_ID_TRUCK_JP,	// ジャンプ一定角度以上時落下中ターン

	GME_PLY_MTN_ID_JUMP_S_01,	// ギミックジャンプ 上昇ループ
	GME_PLY_MTN_ID_JUMP_S_02,	// ギミックジャンプ 下降開始
	GME_PLY_MTN_ID_JUMP_S_03,	// ギミックジャンプ 下降ループ

	GME_PLY_MTN_ID_JUMP_F,		// ギミック前転ジャンプ 上昇>下降開始
	GME_PLY_MTN_ID_FALL,		// ギミック前転ジャンプ 下降ループ

	GME_PLY_MTN_ID_SPIN_N,		// スーパーソニック変身 開始1
	GME_PLY_MTN_ID_CHANGE_01,	// スーパーソニック変身 開始2～溜め
	GME_PLY_MTN_ID_CHANGE_01_1,	// スーパーソニック変身 溜めループ
	GME_PLY_MTN_ID_CHANGE_01_2,	// スーパーソニック変身 溜め～変身
	GME_PLY_MTN_ID_CHANGE_02,	// スーパーソニック変身 キメループ
	GME_PLY_MTN_ID_GOAL_01,		// ゴール 開始
	GME_PLY_MTN_ID_GOAL_02,		// ゴール ループ

	GME_PLY_MTN_ID_HANG,		// 滑車 ぶら下がり
	GME_PLY_MTN_ID_HANG_F,		// 滑車 ぶら下がり 前移動
	GME_PLY_MTN_ID_HANG_B,		// 滑車 ぶら下がり 後ろ移動
	GME_PLY_MTN_ID_HANG_ACT,	// 滑車 ぶら下がり 反動
	GME_PLY_MTN_ID_BALL_01,		// 大岩 移動
	GME_PLY_MTN_ID_BALL_02,		// 大岩 おっとっと
	GME_PLY_MTN_ID_BREATH,		// 息継ぎ
	GME_PLY_MTN_ID_ROPE,		// ターザンロープ
	GME_PLY_MTN_ID_ROPE_ST,		// ターザンロープ 停止
	GME_PLY_MTN_ID_SLIDE,		// ウォータースライダー
	GME_PLY_MTN_ID_HANG_ST,		// 滑車 ぶら下がり スタート
	GME_PLY_MTN_ID_SPIN_N,		// 大砲 発射
	GME_PLY_MTN_ID_BREATH_J,	// 息継ぎ ジャンプ中
	GME_PLY_MTN_ID_TRUCK_ST,	// トロッコFW
	GME_PLY_MTN_ID_TRUCK_ST_L,	// トロッコFW 左
	GME_PLY_MTN_ID_TRUCK_RUN,	// トロッコ走り
	GME_PLY_MTN_ID_TRUCK_RUN_L,	// トロッコ走り 左
	GME_PLY_MTN_ID_TRUCK_DOWN,	// トロッコ着地
	GME_PLY_MTN_ID_TRUCK_DANGER,	// トロッコ危険発生
	GME_PLY_MTN_ID_TRUCK_DANGER_ST,	// トロッコ危険発生中
	GME_PLY_MTN_ID_TRUCK_DANGER_RE,	// トロッコ危険回避
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディング手前を見るまで
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディング手前を見る
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディングフィニッシュ１
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディングフィニッシュ２(キメ)
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディングフィニッシュ１ type2
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディングフィニッシュ２ type2(キメ)
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディングフィニッシュ@スーパーソニック１
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディングフィニッシュ@スーパーソニック２(キメ)
};

/// プレイヤーモーションIDリスト右テーブル
const u8 *g_gm_player_motion_right_tbl[GSD_CHAR_ID_MAX] = {
	gm_player_motion_list_son_right,		// ソニック
	gm_player_motion_list_sson_right,		// スーパーソニック
	gm_player_motion_list_son_right,		// スペステソニック
	gm_player_motion_list_pn_son_right,		// ピンボールソニック
	gm_player_motion_list_pn_sson_right,	// ピンボールスーパーソニック
	gm_player_motion_list_tr_son_right,		// トロッコソニック
	gm_player_motion_list_tr_son_right,		// トロッコスーパーソニック
};

/* 左 */
/// ソニックモーションリスト 左
const u8 gm_player_motion_list_son_left[GME_PLY_ACT_STATE_MAX] = {

	GME_PLY_MTN_ID_FW_L,		// FW
	GME_PLY_MTN_ID_FWEX,		// FW おまけ
	GME_PLY_MTN_ID_FWWAIT0_01,	// 待機 0-1
	GME_PLY_MTN_ID_FWWAIT0_02,	// 待機 0-2
	GME_PLY_MTN_ID_FWWAIT1_01,	// 待機 1-1
	GME_PLY_MTN_ID_FWWAIT1_02,	// 待機 1-2
	GME_PLY_MTN_ID_FWWAIT2_01,	// 待機 2-1
	GME_PLY_MTN_ID_FWWAIT2_02,	// 待機 2-2
	GME_PLY_MTN_ID_TURN_L,		// 振り向き FW・歩き
	GME_PLY_MTN_ID_TURN_RUN,	// 振り向き 走り
	GME_PLY_MTN_ID_TURN_BRAKE_L,// 振り向き ブレーキ中
	GME_PLY_MTN_ID_LOOKUP_01_L,	// 見上げ 開始
	GME_PLY_MTN_ID_LOOKUP_02_L,	// 見上げ 中
	GME_PLY_MTN_ID_LOOKUP_03_L,	// 見上げ 終了
	GME_PLY_MTN_ID_SQUAT_01,	// しゃがみ 開始
	GME_PLY_MTN_ID_SQUAT_02_L,	// しゃがみ 中
	GME_PLY_MTN_ID_SQUAT_03_L,	// しゃがみ 終了
	GME_PLY_MTN_ID_WALL_01,		// 押す
	GME_PLY_MTN_ID_WALL_02,		// 押す中

	GME_PLY_MTN_ID_WALK,		// 歩き
	GME_PLY_MTN_ID_RUN,			// 走り
	GME_PLY_MTN_ID_DASH1,		// ダッシュ1
	GME_PLY_MTN_ID_DASH2_L,		// ダッシュ2

	GME_PLY_MTN_ID_BRAKE01_L,	// ブレーキ 開始
	GME_PLY_MTN_ID_BRAKE02_L,	// ブレーキ 踏ん張り
	GME_PLY_MTN_ID_BRAKE03_L,	// ブレーキ 戻り
	GME_PLY_MTN_ID_SPIN_G,		// スピン状態
	GME_PLY_MTN_ID_SPIN_B,		// スピン状態 小さいタイプ
	GME_PLY_MTN_ID_SPIN01,		// スピン モデル移行
	GME_PLY_MTN_ID_SPIN01,//GME_PLY_MTN_ID_SPIN02,		// スピン加速
	GME_PLY_MTN_ID_SPIN01,		// スピンダッシュ
	GME_PLY_MTN_ID_SPIN,		// ホーミング
	//GME_PLY_MTN_ID_SPIN_N,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_JUMP_F,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_ST_F,		// よろける前
	GME_PLY_MTN_ID_ST_B,		// よろける後
	GME_PLY_MTN_ID_ST_D,		// よろける危険

	GME_PLY_MTN_ID_DAMAGE_L,	// ダメージ
	GME_PLY_MTN_ID_DIE_01,		// 死亡始まり
	GME_PLY_MTN_ID_DIE_02,		// 死亡ループ

	GME_PLY_MTN_ID_SPIN_N,		// スピンジャンプ
	GME_PLY_MTN_ID_FALL_L,		// ジャンプ通常落下
	GME_PLY_MTN_ID_FALL_TURN_L,	// ジャンプ通常落下中ターン
	GME_PLY_MTN_ID_FALL_R_L,		// ジャンプ一定角度以上時落下
	GME_PLY_MTN_ID_FALL_R_TURN_L,	// ジャンプ一定角度以上時落下中ターン

	GME_PLY_MTN_ID_JUMP_S_01,	// ギミックジャンプ 上昇ループ
	GME_PLY_MTN_ID_JUMP_S_02,	// ギミックジャンプ 下降開始
	GME_PLY_MTN_ID_JUMP_S_03,	// ギミックジャンプ 下降ループ

	GME_PLY_MTN_ID_JUMP_F,		// ギミック前転ジャンプ 上昇>下降開始
	GME_PLY_MTN_ID_FALL_L,		// ギミック前転ジャンプ 下降ループ

	GME_PLY_MTN_ID_SPIN_N,			// スーパーソニック変身 開始1
	GME_PLY_MTN_ID_CHANGE_01_L,		// スーパーソニック変身 開始2～溜め
	GME_PLY_MTN_ID_CHANGE_01_1_L,	// スーパーソニック変身 溜めループ
	GME_PLY_MTN_ID_CHANGE_01_2_L,	// スーパーソニック変身 溜め～変身
	GME_PLY_MTN_ID_CHANGE_02_L,		// スーパーソニック変身 キメループ
	GME_PLY_MTN_ID_GOAL_01,		// ゴール 開始
	GME_PLY_MTN_ID_GOAL_02,		// ゴール ループ

	GME_PLY_MTN_ID_HANG,		// 滑車 ぶら下がり
	GME_PLY_MTN_ID_HANG_F,		// 滑車 ぶら下がり 前移動
	GME_PLY_MTN_ID_HANG_B,		// 滑車 ぶら下がり 後ろ移動
	GME_PLY_MTN_ID_HANG_ACT,	// 滑車 ぶら下がり 反動
	GME_PLY_MTN_ID_BALL_01,		// 大岩 移動
	GME_PLY_MTN_ID_BALL_02,		// 大岩 おっとっと
	GME_PLY_MTN_ID_BREATH_L,	// 息継ぎ
	GME_PLY_MTN_ID_ROPE,		// ターザンロープ
	GME_PLY_MTN_ID_ROPE_ST,		// ターザンロープ 停止
	GME_PLY_MTN_ID_SLIDE,		// ウォータースライダー
	GME_PLY_MTN_ID_HANG_ST,		// 滑車 ぶら下がり スタート
	GME_PLY_MTN_ID_SPIN_N,		// 大砲 発射
	GME_PLY_MTN_ID_BREATH_J_L,	// 息継ぎ ジャンプ中
	GME_PLY_MTN_ID_TRUCK_ST,	// トロッコFW
	GME_PLY_MTN_ID_TRUCK_ST_L,	// トロッコFW 左
	GME_PLY_MTN_ID_TRUCK_RUN,	// トロッコ走り
	GME_PLY_MTN_ID_TRUCK_RUN_L,	// トロッコ走り 左
	GME_PLY_MTN_ID_TRUCK_DOWN,	// トロッコ着地
	GME_PLY_MTN_ID_TRUCK_DANGER,	// トロッコ危険発生
	GME_PLY_MTN_ID_TRUCK_DANGER_ST,	// トロッコ危険発生中
	GME_PLY_MTN_ID_TRUCK_DANGER_RE,	// トロッコ危険回避
	GME_PLY_MTN_ID_ENDING_FR1,	// エンディング手前を見るまで
	GME_PLY_MTN_ID_ENDING_FR2,	// エンディング手前を見る
	GME_PLY_MTN_ID_ENDING_FN11,	// エンディングフィニッシュ１
	GME_PLY_MTN_ID_ENDING_FN12,	// エンディングフィニッシュ２(キメ)
	GME_PLY_MTN_ID_ENDING_FN21,	// エンディングフィニッシュ１ type2
	GME_PLY_MTN_ID_ENDING_FN22,	// エンディングフィニッシュ２ type2(キメ)
	GME_PLY_MTN_ID_ENDING_FNS1,	// エンディングフィニッシュ@スーパーソニック１
	GME_PLY_MTN_ID_ENDING_FNS2,	// エンディングフィニッシュ@スーパーソニック２(キメ)
};

/// スーパーソニックモーションリスト 左
const u8 gm_player_motion_list_sson_left[GME_PLY_ACT_STATE_MAX] = {

	GME_PLY_MTN_ID_SSON_FW_L,			// FW
	GME_PLY_MTN_ID_FWEX,				// FW おまけ
	GME_PLY_MTN_ID_FWWAIT0_01,			// 待機 0-1
	GME_PLY_MTN_ID_FWWAIT0_02,			// 待機 0-2
	GME_PLY_MTN_ID_SSON_FWWAIT1_01,		// 待機 1-1
	GME_PLY_MTN_ID_SSON_FWWAIT1_02,		// 待機 1-2
	GME_PLY_MTN_ID_FWWAIT2_01,			// 待機 2-1
	GME_PLY_MTN_ID_FWWAIT2_02,			// 待機 2-2
	GME_PLY_MTN_ID_SSON_TURN_L,			// 振り向き FW・歩き
	GME_PLY_MTN_ID_TURN_RUN,			// 振り向き 走り
	GME_PLY_MTN_ID_TURN_BRAKE_L,		// 振り向き ブレーキ中
	GME_PLY_MTN_ID_SSON_LOOKUP_01_L,	// 見上げ 開始
	GME_PLY_MTN_ID_SSON_LOOKUP_02_L,	// 見上げ 中
	GME_PLY_MTN_ID_SSON_LOOKUP_03_L,	// 見上げ 終了
	GME_PLY_MTN_ID_SSON_SQUAT_01,		// しゃがみ 開始
	GME_PLY_MTN_ID_SSON_SQUAT_02_L,		// しゃがみ 中
	GME_PLY_MTN_ID_SSON_SQUAT_03_L,		// しゃがみ 終了
	GME_PLY_MTN_ID_WALL_01,				// 押す
	GME_PLY_MTN_ID_WALL_02,				// 押す中

	GME_PLY_MTN_ID_WALK,				// 歩き
	GME_PLY_MTN_ID_RUN,					// 走り
	GME_PLY_MTN_ID_SSON_DASH1_L,		// ダッシュ1
	GME_PLY_MTN_ID_SSON_DASH1_L,		// ダッシュ2

	GME_PLY_MTN_ID_BRAKE01_L,			// ブレーキ 開始
	GME_PLY_MTN_ID_BRAKE02_L,			// ブレーキ 踏ん張り
	GME_PLY_MTN_ID_BRAKE03_L,			// ブレーキ 戻り
	GME_PLY_MTN_ID_SSPIN_G,				// スピン状態
	GME_PLY_MTN_ID_SSPIN_B,				// スピン状態 小さいタイプ
	GME_PLY_MTN_ID_SPIN01,				// スピン モデル移行
	GME_PLY_MTN_ID_SPIN01,//GME_PLY_MTN_ID_SPIN02,				// スピン加速
	GME_PLY_MTN_ID_SPIN01,				// スピンダッシュ
	GME_PLY_MTN_ID_SPIN,				// ホーミング
	//GME_PLY_MTN_ID_SSPIN_N,			// ホーミング跳ね返り1
	GME_PLY_MTN_ID_SSON_JUMP_F,			// ホーミング跳ね返り1
	GME_PLY_MTN_ID_ST_F,				// よろける前
	GME_PLY_MTN_ID_ST_B,				// よろける後
	GME_PLY_MTN_ID_ST_D,				// よろける危険

	GME_PLY_MTN_ID_DAMAGE_L,			// ダメージ
	GME_PLY_MTN_ID_DIE_01,				// 死亡始まり
	GME_PLY_MTN_ID_DIE_02,				// 死亡ループ

	GME_PLY_MTN_ID_SSPIN_N,				// スピンジャンプ
	GME_PLY_MTN_ID_FALL_L,				// ジャンプ通常落下
	GME_PLY_MTN_ID_FALL_TURN_L,			// ジャンプ通常落下中ターン
	GME_PLY_MTN_ID_FALL_R_L,			// ジャンプ一定角度以上時落下
	GME_PLY_MTN_ID_FALL_R_TURN_L,		// ジャンプ一定角度以上時落下中ターン

	GME_PLY_MTN_ID_SSON_JUMP_S_01,		// ギミックジャンプ 上昇ループ
	GME_PLY_MTN_ID_SSON_JUMP_S_02,		// ギミックジャンプ 下降開始
	GME_PLY_MTN_ID_SSON_JUMP_S_03,		// ギミックジャンプ 下降ループ

	GME_PLY_MTN_ID_SSON_JUMP_F,			// ギミック前転ジャンプ 上昇>下降開始
	GME_PLY_MTN_ID_FALL_L,				// ギミック前転ジャンプ 下降ループ

	GME_PLY_MTN_ID_SPIN_N,				// スーパーソニック変身 開始1
	GME_PLY_MTN_ID_CHANGE_01_L,			// スーパーソニック変身 開始2～溜め
	GME_PLY_MTN_ID_CHANGE_01_1_L,		// スーパーソニック変身 溜めループ
	GME_PLY_MTN_ID_CHANGE_01_2_L,		// スーパーソニック変身 溜め～変身
	GME_PLY_MTN_ID_CHANGE_02_L,			// スーパーソニック変身 キメループ
	GME_PLY_MTN_ID_GOAL_01,				// ゴール 開始
	GME_PLY_MTN_ID_GOAL_02,				// ゴール ループ

	GME_PLY_MTN_ID_HANG,				// 滑車 ぶら下がり
	GME_PLY_MTN_ID_HANG_F,				// 滑車 ぶら下がり 前移動
	GME_PLY_MTN_ID_HANG_B,				// 滑車 ぶら下がり 後ろ移動
	GME_PLY_MTN_ID_HANG_ACT,			// 滑車 ぶら下がり 反動
	GME_PLY_MTN_ID_BALL_01,				// 大岩 移動
	GME_PLY_MTN_ID_BALL_02,				// 大岩 おっとっと
	GME_PLY_MTN_ID_BREATH_L,			// 息継ぎ
	GME_PLY_MTN_ID_ROPE,				// ターザンロープ
	GME_PLY_MTN_ID_ROPE_ST,				// ターザンロープ 停止
	GME_PLY_MTN_ID_SLIDE,				// ウォータースライダー
	GME_PLY_MTN_ID_HANG_ST,				// 滑車 ぶら下がり スタート
	GME_PLY_MTN_ID_SSPIN_N,				// 大砲 発射
	GME_PLY_MTN_ID_BREATH_J_L,			// 息継ぎ ジャンプ中
	GME_PLY_MTN_ID_TRUCK_ST,			// トロッコFW
	GME_PLY_MTN_ID_TRUCK_ST_L,			// トロッコFW 左
	GME_PLY_MTN_ID_TRUCK_RUN,			// トロッコ走り
	GME_PLY_MTN_ID_TRUCK_RUN_L,			// トロッコ走り 左
	GME_PLY_MTN_ID_TRUCK_DOWN,			// トロッコ着地
	GME_PLY_MTN_ID_TRUCK_DANGER,		// トロッコ危険発生
	GME_PLY_MTN_ID_TRUCK_DANGER_ST,		// トロッコ危険発生中
	GME_PLY_MTN_ID_TRUCK_DANGER_RE,		// トロッコ危険回避
	GME_PLY_MTN_ID_ENDING_FR1,			// エンディング手前を見るまで
	GME_PLY_MTN_ID_ENDING_FR2,			// エンディング手前を見る
	GME_PLY_MTN_ID_ENDING_FN11,			// エンディングフィニッシュ１
	GME_PLY_MTN_ID_ENDING_FN12,			// エンディングフィニッシュ２(キメ)
	GME_PLY_MTN_ID_ENDING_FN21,			// エンディングフィニッシュ１ type2
	GME_PLY_MTN_ID_ENDING_FN22,			// エンディングフィニッシュ２ type2(キメ)
	GME_PLY_MTN_ID_ENDING_FNS1,			// エンディングフィニッシュ@スーパーソニック１
	GME_PLY_MTN_ID_ENDING_FNS2,			// エンディングフィニッシュ@スーパーソニック２(キメ)
};

/// ピンボールソニックモーションリスト 左
const u8 gm_player_motion_list_pn_son_left[GME_PLY_ACT_STATE_MAX] = {

	GME_PLY_MTN_ID_SPIN_N,		// FW
	GME_PLY_MTN_ID_SPIN_N,		// FW おまけ
	GME_PLY_MTN_ID_SPIN_N,		// 待機 0-1
	GME_PLY_MTN_ID_SPIN_N,		// 待機 0-2
	GME_PLY_MTN_ID_SPIN_N,		// 待機 1-1
	GME_PLY_MTN_ID_SPIN_N,		// 待機 1-2
	GME_PLY_MTN_ID_SPIN_N,		// 待機 2-1
	GME_PLY_MTN_ID_SPIN_N,		// 待機 2-2
	GME_PLY_MTN_ID_SPIN_N,		// 振り向き FW・歩き
	GME_PLY_MTN_ID_SPIN_N,		// 振り向き 走り
	GME_PLY_MTN_ID_SPIN_N,		// 振り向き ブレーキ中
	GME_PLY_MTN_ID_SPIN_N,		// 見上げ 開始
	GME_PLY_MTN_ID_SPIN_N,		// 見上げ 中
	GME_PLY_MTN_ID_SPIN_N,		// 見上げ 終了
	GME_PLY_MTN_ID_SPIN_N,		// しゃがみ 開始
	GME_PLY_MTN_ID_SPIN_N,		// しゃがみ 中
	GME_PLY_MTN_ID_SPIN_N,		// しゃがみ 終了
	GME_PLY_MTN_ID_SPIN_N,		// 押す
	GME_PLY_MTN_ID_SPIN_N,		// 押す中

	GME_PLY_MTN_ID_SPIN_N,		// 歩き
	GME_PLY_MTN_ID_SPIN_N,		// 走り
	GME_PLY_MTN_ID_SPIN_N,		// ダッシュ1
	GME_PLY_MTN_ID_SPIN_N,		// ダッシュ2

	GME_PLY_MTN_ID_SPIN_N,		// ブレーキ 開始
	GME_PLY_MTN_ID_SPIN_N,		// ブレーキ 踏ん張り
	GME_PLY_MTN_ID_SPIN_N,		// ブレーキ 戻り
	GME_PLY_MTN_ID_SPIN_G,		// スピン状態
	GME_PLY_MTN_ID_SPIN_B,		// スピン状態 小さいタイプ
	GME_PLY_MTN_ID_SPIN01,		// スピン モデル移行
	GME_PLY_MTN_ID_SPIN01,//GME_PLY_MTN_ID_SPIN02,		// スピン加速
	GME_PLY_MTN_ID_SPIN01,		// スピンダッシュ
	GME_PLY_MTN_ID_SPIN_N,		// ホーミング
	//GME_PLY_MTN_ID_SPIN_N,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_SPIN_N,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_SPIN_N,		// よろける前
	GME_PLY_MTN_ID_SPIN_N,		// よろける後
	GME_PLY_MTN_ID_SPIN_N,		// よろける危険

	GME_PLY_MTN_ID_SPIN_N,		// ダメージ
	GME_PLY_MTN_ID_DIE_01,		// 死亡始まり
	GME_PLY_MTN_ID_DIE_02,		// 死亡ループ

	GME_PLY_MTN_ID_SPIN_N,		// スピンジャンプ
	GME_PLY_MTN_ID_SPIN_N,		// ジャンプ通常落下
	GME_PLY_MTN_ID_SPIN_N,		// ジャンプ通常落下中ターン
	GME_PLY_MTN_ID_SPIN_N,		// ジャンプ一定角度以上時落下
	GME_PLY_MTN_ID_SPIN_N,		// ジャンプ一定角度以上時落下中ターン

	GME_PLY_MTN_ID_SPIN_N,		// ギミックジャンプ 上昇ループ
	GME_PLY_MTN_ID_SPIN_N,		// ギミックジャンプ 下降開始
	GME_PLY_MTN_ID_SPIN_N,		// ギミックジャンプ 下降ループ

	GME_PLY_MTN_ID_SPIN_N,		// ギミック前転ジャンプ 上昇>下降開始
	GME_PLY_MTN_ID_SPIN_N,		// ギミック前転ジャンプ 下降ループ

	GME_PLY_MTN_ID_SPIN_N,		// スーパーソニック変身 開始1
	GME_PLY_MTN_ID_SPIN_N,		// スーパーソニック変身 開始2～溜め
	GME_PLY_MTN_ID_SPIN_N,		// スーパーソニック変身 溜めループ
	GME_PLY_MTN_ID_SPIN_N,		// スーパーソニック変身 溜め～変身
	GME_PLY_MTN_ID_SPIN_N,		// スーパーソニック変身 キメループ
	GME_PLY_MTN_ID_SPIN_N,		// ゴール 開始
	GME_PLY_MTN_ID_SPIN_N,		// ゴール ループ

	GME_PLY_MTN_ID_SPIN_N,		// 滑車 ぶら下がり
	GME_PLY_MTN_ID_SPIN_N,		// 滑車 ぶら下がり 前移動
	GME_PLY_MTN_ID_SPIN_N,		// 滑車 ぶら下がり 後ろ移動
	GME_PLY_MTN_ID_SPIN_N,		// 滑車 ぶら下がり 反動
	GME_PLY_MTN_ID_SPIN_N,		// 大岩 移動
	GME_PLY_MTN_ID_SPIN_N,		// 大岩 おっとっと
	GME_PLY_MTN_ID_SPIN_N,		// 息継ぎ
	GME_PLY_MTN_ID_SPIN_N,		// ターザンロープ
	GME_PLY_MTN_ID_SPIN_N,		// ターザンロープ 停止
	GME_PLY_MTN_ID_SPIN_N,		// ウォータースライダー
	GME_PLY_MTN_ID_SPIN_N,		// 滑車 ぶら下がり スタート
	GME_PLY_MTN_ID_SPIN_N,		// 大砲 発射
	GME_PLY_MTN_ID_SPIN_N,		// 息継ぎ ジャンプ中
	GME_PLY_MTN_ID_SPIN_N,		// トロッコFW
	GME_PLY_MTN_ID_SPIN_N,		// トロッコFW 左
	GME_PLY_MTN_ID_SPIN_N,		// トロッコ走り
	GME_PLY_MTN_ID_SPIN_N,		// トロッコ走り 左
	GME_PLY_MTN_ID_SPIN_N,		// トロッコ着地
	GME_PLY_MTN_ID_SPIN_N,		// トロッコ危険発生
	GME_PLY_MTN_ID_SPIN_N,		// トロッコ危険発生中
	GME_PLY_MTN_ID_SPIN_N,		// トロッコ危険回避
	GME_PLY_MTN_ID_SPIN_N,		// エンディング手前を見るまで
	GME_PLY_MTN_ID_SPIN_N,		// エンディング手前を見る
	GME_PLY_MTN_ID_SPIN_N,		// エンディングフィニッシュ１
	GME_PLY_MTN_ID_SPIN_N,		// エンディングフィニッシュ２(キメ)
	GME_PLY_MTN_ID_SPIN_N,		// エンディングフィニッシュ１ type2
	GME_PLY_MTN_ID_SPIN_N,		// エンディングフィニッシュ２ type2(キメ)
	GME_PLY_MTN_ID_SPIN_N,		// エンディングフィニッシュ@スーパーソニック１
	GME_PLY_MTN_ID_SPIN_N,		// エンディングフィニッシュ@スーパーソニック２(キメ)
};

/// ピンボールスーパーソニックモーションリスト 左
const u8 gm_player_motion_list_pn_sson_left[GME_PLY_ACT_STATE_MAX] = {

	GME_PLY_MTN_ID_SSPIN_N,		// FW
	GME_PLY_MTN_ID_SSPIN_N,		// FW おまけ
	GME_PLY_MTN_ID_SSPIN_N,		// 待機 0-1
	GME_PLY_MTN_ID_SSPIN_N,		// 待機 0-2
	GME_PLY_MTN_ID_SSPIN_N,		// 待機 1-1
	GME_PLY_MTN_ID_SSPIN_N,		// 待機 1-2
	GME_PLY_MTN_ID_SSPIN_N,		// 待機 2-1
	GME_PLY_MTN_ID_SSPIN_N,		// 待機 2-2
	GME_PLY_MTN_ID_SSPIN_N,		// 振り向き FW・歩き
	GME_PLY_MTN_ID_SSPIN_N,		// 振り向き 走り
	GME_PLY_MTN_ID_SSPIN_N,		// 振り向き ブレーキ中
	GME_PLY_MTN_ID_SSPIN_N,		// 見上げ 開始
	GME_PLY_MTN_ID_SSPIN_N,		// 見上げ 中
	GME_PLY_MTN_ID_SSPIN_N,		// 見上げ 終了
	GME_PLY_MTN_ID_SSPIN_N,		// しゃがみ 開始
	GME_PLY_MTN_ID_SSPIN_N,		// しゃがみ 中
	GME_PLY_MTN_ID_SSPIN_N,		// しゃがみ 終了
	GME_PLY_MTN_ID_SSPIN_N,		// 押す
	GME_PLY_MTN_ID_SSPIN_N,		// 押す中

	GME_PLY_MTN_ID_SSPIN_N,		// 歩き
	GME_PLY_MTN_ID_SSPIN_N,		// 走り
	GME_PLY_MTN_ID_SSPIN_N,		// ダッシュ1
	GME_PLY_MTN_ID_SSPIN_N,		// ダッシュ2

	GME_PLY_MTN_ID_SSPIN_N,		// ブレーキ 開始
	GME_PLY_MTN_ID_SSPIN_N,		// ブレーキ 踏ん張り
	GME_PLY_MTN_ID_SSPIN_N,		// ブレーキ 戻り
	GME_PLY_MTN_ID_SSPIN_G,		// スピン状態
	GME_PLY_MTN_ID_SSPIN_B,		// スピン状態 小さいタイプ
	GME_PLY_MTN_ID_SPIN01,		// スピン モデル移行
	GME_PLY_MTN_ID_SPIN01,//GME_PLY_MTN_ID_SPIN02,		// スピン加速
	GME_PLY_MTN_ID_SPIN01,		// スピンダッシュ
	GME_PLY_MTN_ID_SSPIN_N,		// ホーミング
	//GME_PLY_MTN_ID_SSPIN_N,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_SSPIN_N,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_SSPIN_N,		// よろける前
	GME_PLY_MTN_ID_SSPIN_N,		// よろける後
	GME_PLY_MTN_ID_SSPIN_N,		// よろける危険

	GME_PLY_MTN_ID_SSPIN_N,		// ダメージ
	GME_PLY_MTN_ID_DIE_01,		// 死亡始まり
	GME_PLY_MTN_ID_DIE_02,		// 死亡ループ

	GME_PLY_MTN_ID_SSPIN_N,		// スピンジャンプ
	GME_PLY_MTN_ID_SSPIN_N,		// ジャンプ通常落下
	GME_PLY_MTN_ID_SSPIN_N,		// ジャンプ通常落下中ターン
	GME_PLY_MTN_ID_SSPIN_N,		// ジャンプ一定角度以上時落下
	GME_PLY_MTN_ID_SSPIN_N,		// ジャンプ一定角度以上時落下中ターン

	GME_PLY_MTN_ID_SSPIN_N,		// ギミックジャンプ 上昇ループ
	GME_PLY_MTN_ID_SSPIN_N,		// ギミックジャンプ 下降開始
	GME_PLY_MTN_ID_SSPIN_N,		// ギミックジャンプ 下降ループ

	GME_PLY_MTN_ID_SSPIN_N,		// ギミック前転ジャンプ 上昇>下降開始
	GME_PLY_MTN_ID_SSPIN_N,		// ギミック前転ジャンプ 下降ループ

	GME_PLY_MTN_ID_SSPIN_N,		// スーパーソニック変身 開始1
	GME_PLY_MTN_ID_SSPIN_N,		// スーパーソニック変身 開始2～溜め
	GME_PLY_MTN_ID_SSPIN_N,		// スーパーソニック変身 溜めループ
	GME_PLY_MTN_ID_SSPIN_N,		// スーパーソニック変身 溜め～変身
	GME_PLY_MTN_ID_SSPIN_N,		// スーパーソニック変身 キメループ
	GME_PLY_MTN_ID_SSPIN_N,		// ゴール 開始
	GME_PLY_MTN_ID_SSPIN_N,		// ゴール ループ

	GME_PLY_MTN_ID_SSPIN_N,		// 滑車 ぶら下がり
	GME_PLY_MTN_ID_SSPIN_N,		// 滑車 ぶら下がり 前移動
	GME_PLY_MTN_ID_SSPIN_N,		// 滑車 ぶら下がり 後ろ移動
	GME_PLY_MTN_ID_SSPIN_N,		// 滑車 ぶら下がり 反動
	GME_PLY_MTN_ID_SSPIN_N,		// 大岩 移動
	GME_PLY_MTN_ID_SSPIN_N,		// 大岩 おっとっと
	GME_PLY_MTN_ID_SSPIN_N,		// 息継ぎ
	GME_PLY_MTN_ID_SSPIN_N,		// ターザンロープ
	GME_PLY_MTN_ID_SSPIN_N,		// ターザンロープ 停止
	GME_PLY_MTN_ID_SSPIN_N,		// ウォータースライダー
	GME_PLY_MTN_ID_SSPIN_N,		// 滑車 ぶら下がり スタート
	GME_PLY_MTN_ID_SSPIN_N,		// 大砲 発射
	GME_PLY_MTN_ID_SSPIN_N,		// 息継ぎ ジャンプ中
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコFW
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコFW 左
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコ走り
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコ走り 左
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコ着地
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコ危険発生
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコ危険発生中
	GME_PLY_MTN_ID_SSPIN_N,		// トロッコ危険回避
	GME_PLY_MTN_ID_SSPIN_N,		// エンディング手前を見るまで
	GME_PLY_MTN_ID_SSPIN_N,		// エンディング手前を見る
	GME_PLY_MTN_ID_SSPIN_N,		// エンディングフィニッシュ１
	GME_PLY_MTN_ID_SSPIN_N,		// エンディングフィニッシュ２(キメ)
	GME_PLY_MTN_ID_SSPIN_N,		// エンディングフィニッシュ１ type2
	GME_PLY_MTN_ID_SSPIN_N,		// エンディングフィニッシュ２ type2(キメ)
	GME_PLY_MTN_ID_SSPIN_N,		// エンディングフィニッシュ@スーパーソニック１
	GME_PLY_MTN_ID_SSPIN_N,		// エンディングフィニッシュ@スーパーソニック２(キメ)
};

/// トロッコソニックモーションリスト 左
const u8 gm_player_motion_list_tr_son_left[GME_PLY_ACT_STATE_MAX] = {

	GME_PLY_MTN_ID_FW_L,		// FW
	GME_PLY_MTN_ID_FWEX,		// FW おまけ
	GME_PLY_MTN_ID_FWWAIT0_01,	// 待機 0-1
	GME_PLY_MTN_ID_FWWAIT0_02,	// 待機 0-2
	GME_PLY_MTN_ID_FWWAIT1_01,	// 待機 1-1
	GME_PLY_MTN_ID_FWWAIT1_02,	// 待機 1-2
	GME_PLY_MTN_ID_FWWAIT2_01,	// 待機 2-1
	GME_PLY_MTN_ID_FWWAIT2_02,	// 待機 2-2
	GME_PLY_MTN_ID_TURN,		// 振り向き FW・歩き
	GME_PLY_MTN_ID_TURN_RUN,	// 振り向き 走り
	GME_PLY_MTN_ID_TURN_BRAKE,	// 振り向き ブレーキ中
	GME_PLY_MTN_ID_LOOKUP_01,	// 見上げ 開始
	GME_PLY_MTN_ID_LOOKUP_02,	// 見上げ 中
	GME_PLY_MTN_ID_LOOKUP_03,	// 見上げ 終了
	GME_PLY_MTN_ID_SQUAT_01,	// しゃがみ 開始
	GME_PLY_MTN_ID_SQUAT_02,	// しゃがみ 中
	GME_PLY_MTN_ID_SQUAT_03,	// しゃがみ 終了
	GME_PLY_MTN_ID_WALL_01,		// 押す
	GME_PLY_MTN_ID_WALL_02,		// 押す中

	GME_PLY_MTN_ID_TRUCK_RUN_L,	// 歩き
	GME_PLY_MTN_ID_TRUCK_RUN_L,	// 走り
	GME_PLY_MTN_ID_TRUCK_RUN_L,	// ダッシュ1
	GME_PLY_MTN_ID_TRUCK_RUN_L,	// ダッシュ2

	GME_PLY_MTN_ID_BRAKE01,		// ブレーキ 開始
	GME_PLY_MTN_ID_BRAKE02,		// ブレーキ 踏ん張り
	GME_PLY_MTN_ID_BRAKE03,		// ブレーキ 戻り
	GME_PLY_MTN_ID_SPIN_G,		// スピン状態
	GME_PLY_MTN_ID_SPIN_B,		// スピン状態 小さいタイプ
	GME_PLY_MTN_ID_SPIN01,		// スピン モデル移行
	GME_PLY_MTN_ID_SPIN01,//GME_PLY_MTN_ID_SPIN02,		// スピン加速
	GME_PLY_MTN_ID_SPIN01,		// スピンダッシュ
	GME_PLY_MTN_ID_SPIN,		// ホーミング
	//GME_PLY_MTN_ID_SPIN_N,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_JUMP_F,		// ホーミング跳ね返り1
	GME_PLY_MTN_ID_ST_F,		// よろける前
	GME_PLY_MTN_ID_ST_B,		// よろける後
	GME_PLY_MTN_ID_ST_D,		// よろける危険

	GME_PLY_MTN_ID_DAMAGE,		// ダメージ
	GME_PLY_MTN_ID_DIE_01,		// 死亡始まり
	GME_PLY_MTN_ID_DIE_02,		// 死亡ループ

	GME_PLY_MTN_ID_SPIN_N,		// スピンジャンプ
	GME_PLY_MTN_ID_TRUCK_JP,	// ジャンプ通常落下
	GME_PLY_MTN_ID_TRUCK_JP,	// ジャンプ通常落下中ターン
	GME_PLY_MTN_ID_TRUCK_JP,	// ジャンプ一定角度以上時落下
	GME_PLY_MTN_ID_TRUCK_JP,	// ジャンプ一定角度以上時落下中ターン

	GME_PLY_MTN_ID_JUMP_S_01,	// ギミックジャンプ 上昇ループ
	GME_PLY_MTN_ID_JUMP_S_02,	// ギミックジャンプ 下降開始
	GME_PLY_MTN_ID_JUMP_S_03,	// ギミックジャンプ 下降ループ

	GME_PLY_MTN_ID_JUMP_F,		// ギミック前転ジャンプ 上昇>下降開始
	GME_PLY_MTN_ID_FALL_L,		// ギミック前転ジャンプ 下降ループ

	GME_PLY_MTN_ID_SPIN_N,		// スーパーソニック変身 開始1
	GME_PLY_MTN_ID_CHANGE_01,	// スーパーソニック変身 開始2～溜め
	GME_PLY_MTN_ID_CHANGE_01_1,	// スーパーソニック変身 溜めループ
	GME_PLY_MTN_ID_CHANGE_01_2,	// スーパーソニック変身 溜め～変身
	GME_PLY_MTN_ID_CHANGE_02,	// スーパーソニック変身 キメループ
	GME_PLY_MTN_ID_GOAL_01,		// ゴール 開始
	GME_PLY_MTN_ID_GOAL_02,		// ゴール ループ

	GME_PLY_MTN_ID_HANG,		// 滑車 ぶら下がり
	GME_PLY_MTN_ID_HANG_F,		// 滑車 ぶら下がり 前移動
	GME_PLY_MTN_ID_HANG_B,		// 滑車 ぶら下がり 後ろ移動
	GME_PLY_MTN_ID_HANG_ACT,	// 滑車 ぶら下がり 反動
	GME_PLY_MTN_ID_BALL_01,		// 大岩 移動
	GME_PLY_MTN_ID_BALL_02,		// 大岩 おっとっと
	GME_PLY_MTN_ID_BREATH,		// 息継ぎ
	GME_PLY_MTN_ID_ROPE,		// ターザンロープ
	GME_PLY_MTN_ID_ROPE_ST,		// ターザンロープ 停止
	GME_PLY_MTN_ID_SLIDE,		// ウォータースライダー
	GME_PLY_MTN_ID_HANG_ST,		// 滑車 ぶら下がり スタート
	GME_PLY_MTN_ID_SPIN_N,		// 大砲 発射
	GME_PLY_MTN_ID_BREATH_J_L,	// 息継ぎ ジャンプ中
	GME_PLY_MTN_ID_TRUCK_ST,	// トロッコFW
	GME_PLY_MTN_ID_TRUCK_ST_L,	// トロッコFW 左
	GME_PLY_MTN_ID_TRUCK_RUN,	// トロッコ走り
	GME_PLY_MTN_ID_TRUCK_RUN_L,	// トロッコ走り 左
	GME_PLY_MTN_ID_TRUCK_DOWN,	// トロッコ着地
	GME_PLY_MTN_ID_TRUCK_DANGER,	// トロッコ危険発生
	GME_PLY_MTN_ID_TRUCK_DANGER_ST,	// トロッコ危険発生中
	GME_PLY_MTN_ID_TRUCK_DANGER_RE,	// トロッコ危険回避
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディング手前を見るまで
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディング手前を見る
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディングフィニッシュ１
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディングフィニッシュ２(キメ)
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディングフィニッシュ１ type2
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディングフィニッシュ２ type2(キメ)
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディングフィニッシュ@スーパーソニック１
	GME_PLY_MTN_ID_TRUCK_ST,	// エンディングフィニッシュ@スーパーソニック２(キメ)
};

/// プレイヤーモーションIDリスト左テーブル
const u8 *g_gm_player_motion_left_tbl[GSD_CHAR_ID_MAX] = {
	gm_player_motion_list_son_left,		// ソニック
	gm_player_motion_list_sson_left,	// スーパーソニック
	gm_player_motion_list_son_left,		// スペステソニック
	gm_player_motion_list_pn_son_left,	// ピンボールソニック
	gm_player_motion_list_pn_sson_left,	// ピンボールスーパーソニック
	gm_player_motion_list_tr_son_left,	// トロッコソニック
	gm_player_motion_list_tr_son_left,	// トロッコスーパーソニック
};


//===========================================================================
// プレイヤーモデルIDリスト
//===========================================================================
/// ソニック, スーパーソニック, スペステソニック モデルIDリスト
const u8 gm_player_model_list_son[GME_PLY_ACT_STATE_MAX] = {

	GME_PLY_MODEL_ID_NORMAL,	// FW
	GME_PLY_MODEL_ID_NORMAL,	// FW おまけ
#if _IPHONE
	GME_PLY_MODEL_ID_IPHONE,	// 待機 0-1
	GME_PLY_MODEL_ID_IPHONE,	// 待機 0-2
	GME_PLY_MODEL_ID_IPHONE,	// 待機 1-1
	GME_PLY_MODEL_ID_IPHONE,	// 待機 1-2
	GME_PLY_MODEL_ID_IPHONE,	// 待機 2-1
	GME_PLY_MODEL_ID_IPHONE,	// 待機 2-2
#else
	GME_PLY_MODEL_ID_NORMAL,	// 待機 0-1
	GME_PLY_MODEL_ID_NORMAL,	// 待機 0-2
	GME_PLY_MODEL_ID_NORMAL,	// 待機 1-1
	GME_PLY_MODEL_ID_NORMAL,	// 待機 1-2
	GME_PLY_MODEL_ID_NORMAL,	// 待機 2-1
	GME_PLY_MODEL_ID_NORMAL,	// 待機 2-2
#endif // _IPHONE
	GME_PLY_MODEL_ID_NORMAL,	// 振り向き
	GME_PLY_MODEL_ID_NORMAL,	// 振り向き
	GME_PLY_MODEL_ID_NORMAL,	// 振り向き
	GME_PLY_MODEL_ID_NORMAL,	// 見上げ
	GME_PLY_MODEL_ID_NORMAL,	// 見上げ
	GME_PLY_MODEL_ID_NORMAL,	// 見上げ
	GME_PLY_MODEL_ID_NORMAL,	// しゃがみ
	GME_PLY_MODEL_ID_NORMAL,	// しゃがみ
	GME_PLY_MODEL_ID_NORMAL,	// しゃがみ
	GME_PLY_MODEL_ID_NORMAL,	// 押す
	GME_PLY_MODEL_ID_NORMAL,	// 押し中

	GME_PLY_MODEL_ID_NORMAL,	// 歩き
	GME_PLY_MODEL_ID_NORMAL,	// 走り
	GME_PLY_MODEL_ID_NORMAL,	// ダッシュ1
	GME_PLY_MODEL_ID_NORMAL,	// ダッシュ2

	GME_PLY_MODEL_ID_NORMAL,	// ブレーキ
	GME_PLY_MODEL_ID_NORMAL,	// ブレーキ
	GME_PLY_MODEL_ID_NORMAL,	// ブレーキ
#if _IPHONE
	GME_PLY_MODEL_ID_SPINJUMP,	// スピン状態
	GME_PLY_MODEL_ID_SPINJUMP,	// スピン状態 小さいタイプ
#else
	GME_PLY_MODEL_ID_NORMAL,	// スピン状態
	GME_PLY_MODEL_ID_NORMAL,	// スピン状態 小さいタイプ
#endif // _IPHONE
	GME_PLY_MODEL_ID_SPIN,		// スピン モデル移行
	GME_PLY_MODEL_ID_SPIN,		// スピン 加速
	GME_PLY_MODEL_ID_SPIN,		// スピンダッシュ
	GME_PLY_MODEL_ID_SPIN,		// ホーミング
	GME_PLY_MODEL_ID_NORMAL,	// ホーミング跳ね返り1
	GME_PLY_MODEL_ID_NORMAL,	// よろける前
	GME_PLY_MODEL_ID_NORMAL,	// よろける後
	GME_PLY_MODEL_ID_NORMAL,	// よろける危険

	GME_PLY_MODEL_ID_NORMAL,	// ダメージ
	GME_PLY_MODEL_ID_NORMAL,	// 死亡始まり
	GME_PLY_MODEL_ID_NORMAL,	// 死亡ループ

#if _IPHONE
	GME_PLY_MODEL_ID_SPINJUMP,	// スピンジャンプ
#else
	GME_PLY_MODEL_ID_NORMAL,	// スピンジャンプ
#endif // _IPHONE
	GME_PLY_MODEL_ID_NORMAL,	// ジャンプ通常落下
	GME_PLY_MODEL_ID_NORMAL,	// ジャンプ通常落下中ターン
	GME_PLY_MODEL_ID_NORMAL,	// ジャンプ一定角度以上時落下
	GME_PLY_MODEL_ID_NORMAL,	// ジャンプ一定角度以上時落下中ターン

	GME_PLY_MODEL_ID_NORMAL,	// ギミックジャンプ 上昇ループ
	GME_PLY_MODEL_ID_NORMAL,	// ギミックジャンプ 下降開始
	GME_PLY_MODEL_ID_NORMAL,	// ギミックジャンプ 下降ループ

	GME_PLY_MODEL_ID_NORMAL,	// ギミック前転ジャンプ 上昇>下降開始
	GME_PLY_MODEL_ID_NORMAL,	// ギミック前転ジャンプ 下降ループ

	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 開始1
	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 開始2～溜め
	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 溜めループ
	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 溜め～変身
	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 キメループ
	GME_PLY_MODEL_ID_NORMAL,	// ゴール 開始
	GME_PLY_MODEL_ID_NORMAL,	// ゴール ループ

	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり
	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり 前移動
	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり 後ろ移動
	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり 反動
	GME_PLY_MODEL_ID_NORMAL,	// 大岩 移動
	GME_PLY_MODEL_ID_NORMAL,	// 大岩 おっとっと
	GME_PLY_MODEL_ID_NORMAL,	// 息継ぎ
	GME_PLY_MODEL_ID_NORMAL,	// ターザンロープ
	GME_PLY_MODEL_ID_NORMAL,	// ターザンロープ 停止
	GME_PLY_MODEL_ID_NORMAL,	// ウォータースライダー
	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり スタート
#if _IPHONE
	GME_PLY_MODEL_ID_SPINJUMP,	// 大砲 発射
#else
	GME_PLY_MODEL_ID_NORMAL,	// 大砲 発射
#endif // _IPHONE
	GME_PLY_MODEL_ID_NORMAL,	// 息継ぎ ジャンプ中
	GME_PLY_MODEL_ID_NORMAL,	// トロッコFW
	GME_PLY_MODEL_ID_NORMAL,	// トロッコFW 左
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ走り
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ走り 左
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ着地
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ危険発生
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ危険発生中
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ危険回避
	GME_PLY_MODEL_ID_NORMAL,	// エンディング手前を見るまで
	GME_PLY_MODEL_ID_NORMAL,	// エンディング手前を見る
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ１
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ２(キメ)
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ１ type2
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ２ type2(キメ)
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ@スーパーソニック１
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ@スーパーソニック２(キメ)

};

/// ピンボールソニック モデルIDリスト
const u8 gm_player_model_list_pn_son[GME_PLY_ACT_STATE_MAX] = {

	GME_PLY_MODEL_ID_NORMAL,	// FW
	GME_PLY_MODEL_ID_NORMAL,	// FW おまけ
#if _IPHONE
	GME_PLY_MODEL_ID_IPHONE,	// 待機 0-1
	GME_PLY_MODEL_ID_IPHONE,	// 待機 0-2
	GME_PLY_MODEL_ID_IPHONE,	// 待機 1-1
	GME_PLY_MODEL_ID_IPHONE,	// 待機 1-2
	GME_PLY_MODEL_ID_IPHONE,	// 待機 2-1
	GME_PLY_MODEL_ID_IPHONE,	// 待機 2-2
#else
	GME_PLY_MODEL_ID_NORMAL,	// 待機 0-1
	GME_PLY_MODEL_ID_NORMAL,	// 待機 0-2
	GME_PLY_MODEL_ID_NORMAL,	// 待機 1-1
	GME_PLY_MODEL_ID_NORMAL,	// 待機 1-2
	GME_PLY_MODEL_ID_NORMAL,	// 待機 2-1
	GME_PLY_MODEL_ID_NORMAL,	// 待機 2-2
#endif // _IPHONE
	GME_PLY_MODEL_ID_NORMAL,	// 振り向き
	GME_PLY_MODEL_ID_NORMAL,	// 振り向き
	GME_PLY_MODEL_ID_NORMAL,	// 振り向き
	GME_PLY_MODEL_ID_NORMAL,	// 見上げ
	GME_PLY_MODEL_ID_NORMAL,	// 見上げ
	GME_PLY_MODEL_ID_NORMAL,	// 見上げ
	GME_PLY_MODEL_ID_NORMAL,	// しゃがみ
	GME_PLY_MODEL_ID_NORMAL,	// しゃがみ
	GME_PLY_MODEL_ID_NORMAL,	// しゃがみ
	GME_PLY_MODEL_ID_NORMAL,	// 押す
	GME_PLY_MODEL_ID_NORMAL,	// 押し中

	GME_PLY_MODEL_ID_NORMAL,	// 歩き
	GME_PLY_MODEL_ID_NORMAL,	// 走り
	GME_PLY_MODEL_ID_NORMAL,	// ダッシュ1
	GME_PLY_MODEL_ID_NORMAL,	// ダッシュ2

	GME_PLY_MODEL_ID_NORMAL,	// ブレーキ
	GME_PLY_MODEL_ID_NORMAL,	// ブレーキ
	GME_PLY_MODEL_ID_NORMAL,	// ブレーキ
#if _IPHONE
	GME_PLY_MODEL_ID_SPINJUMP,	// スピン状態
	GME_PLY_MODEL_ID_SPINJUMP,	// スピン状態 小さいタイプ
#else
	GME_PLY_MODEL_ID_NORMAL,	// スピン状態
	GME_PLY_MODEL_ID_NORMAL,	// スピン状態 小さいタイプ
#endif // _IPHONE
	GME_PLY_MODEL_ID_SPIN,		// スピン モデル移行
	GME_PLY_MODEL_ID_SPIN,		// スピン 加速
	GME_PLY_MODEL_ID_SPIN,		// スピンダッシュ
	GME_PLY_MODEL_ID_NORMAL,	// ホーミング
	GME_PLY_MODEL_ID_NORMAL,	// ホーミング跳ね返り1
	GME_PLY_MODEL_ID_NORMAL,	// よろける前
	GME_PLY_MODEL_ID_NORMAL,	// よろける後
	GME_PLY_MODEL_ID_NORMAL,	// よろける危険

	GME_PLY_MODEL_ID_NORMAL,	// ダメージ
	GME_PLY_MODEL_ID_NORMAL,	// 死亡始まり
	GME_PLY_MODEL_ID_NORMAL,	// 死亡ループ

#if _IPHONE
	GME_PLY_MODEL_ID_SPINJUMP,	// スピンジャンプ
#else
	GME_PLY_MODEL_ID_NORMAL,	// スピンジャンプ
#endif // _IPHONE
	GME_PLY_MODEL_ID_NORMAL,	// ジャンプ通常落下
	GME_PLY_MODEL_ID_NORMAL,	// ジャンプ通常落下中ターン
	GME_PLY_MODEL_ID_NORMAL,	// ジャンプ一定角度以上時落下
	GME_PLY_MODEL_ID_NORMAL,	// ジャンプ一定角度以上時落下中ターン

	GME_PLY_MODEL_ID_NORMAL,	// ギミックジャンプ 上昇ループ
	GME_PLY_MODEL_ID_NORMAL,	// ギミックジャンプ 下降開始
	GME_PLY_MODEL_ID_NORMAL,	// ギミックジャンプ 下降ループ

	GME_PLY_MODEL_ID_NORMAL,	// ギミック前転ジャンプ 上昇>下降開始
	GME_PLY_MODEL_ID_NORMAL,	// ギミック前転ジャンプ 下降ループ

	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 開始1
	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 開始2～溜め
	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 溜めループ
	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 溜め～変身
	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 キメループ
	GME_PLY_MODEL_ID_NORMAL,	// ゴール 開始
	GME_PLY_MODEL_ID_NORMAL,	// ゴール ループ

	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり
	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり 前移動
	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり 後ろ移動
	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり 反動
	GME_PLY_MODEL_ID_NORMAL,	// 大岩 移動
	GME_PLY_MODEL_ID_NORMAL,	// 大岩 おっとっと
	GME_PLY_MODEL_ID_NORMAL,	// 息継ぎ
	GME_PLY_MODEL_ID_NORMAL,	// ターザンロープ
	GME_PLY_MODEL_ID_NORMAL,	// ターザンロープ 停止
	GME_PLY_MODEL_ID_NORMAL,	// ウォータースライダー
	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり スタート
#if _IPHONE
	GME_PLY_MODEL_ID_SPINJUMP,	// 大砲 発射
#else
	GME_PLY_MODEL_ID_NORMAL,	// 大砲 発射
#endif // _IPHONE
	GME_PLY_MODEL_ID_NORMAL,	// 息継ぎ ジャンプ中
	GME_PLY_MODEL_ID_NORMAL,	// トロッコFW
	GME_PLY_MODEL_ID_NORMAL,	// トロッコFW 左
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ走り
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ走り 左
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ着地
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ危険発生
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ危険発生中
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ危険回避
	GME_PLY_MODEL_ID_NORMAL,	// エンディング手前を見るまで
	GME_PLY_MODEL_ID_NORMAL,	// エンディング手前を見る
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ１
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ２(キメ)
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ１ type2
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ２ type2(キメ)
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ@スーパーソニック１
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ@スーパーソニック２(キメ)

};

/// トロッコソニック モデルIDリスト
const u8 gm_player_model_list_tr_son[GME_PLY_ACT_STATE_MAX] = {

	GME_PLY_MODEL_ID_NORMAL,	// FW
	GME_PLY_MODEL_ID_NORMAL,	// FW おまけ
#if _IPHONE
	GME_PLY_MODEL_ID_IPHONE,	// 待機 0-1
	GME_PLY_MODEL_ID_IPHONE,	// 待機 0-2
	GME_PLY_MODEL_ID_IPHONE,	// 待機 1-1
	GME_PLY_MODEL_ID_IPHONE,	// 待機 1-2
	GME_PLY_MODEL_ID_IPHONE,	// 待機 2-1
	GME_PLY_MODEL_ID_IPHONE,	// 待機 2-2
#else
	GME_PLY_MODEL_ID_NORMAL,	// 待機 0-1
	GME_PLY_MODEL_ID_NORMAL,	// 待機 0-2
	GME_PLY_MODEL_ID_NORMAL,	// 待機 1-1
	GME_PLY_MODEL_ID_NORMAL,	// 待機 1-2
	GME_PLY_MODEL_ID_NORMAL,	// 待機 2-1
	GME_PLY_MODEL_ID_NORMAL,	// 待機 2-2
#endif // _IPHONE
	GME_PLY_MODEL_ID_NORMAL,	// 振り向き
	GME_PLY_MODEL_ID_NORMAL,	// 振り向き
	GME_PLY_MODEL_ID_NORMAL,	// 振り向き
	GME_PLY_MODEL_ID_NORMAL,	// 見上げ
	GME_PLY_MODEL_ID_NORMAL,	// 見上げ
	GME_PLY_MODEL_ID_NORMAL,	// 見上げ
	GME_PLY_MODEL_ID_NORMAL,	// しゃがみ
	GME_PLY_MODEL_ID_NORMAL,	// しゃがみ
	GME_PLY_MODEL_ID_NORMAL,	// しゃがみ
	GME_PLY_MODEL_ID_NORMAL,	// 押す
	GME_PLY_MODEL_ID_NORMAL,	// 押し中

	GME_PLY_MODEL_ID_NORMAL,	// 歩き
	GME_PLY_MODEL_ID_NORMAL,	// 走り
	GME_PLY_MODEL_ID_NORMAL,	// ダッシュ1
	GME_PLY_MODEL_ID_NORMAL,	// ダッシュ2

	GME_PLY_MODEL_ID_NORMAL,	// ブレーキ
	GME_PLY_MODEL_ID_NORMAL,	// ブレーキ
	GME_PLY_MODEL_ID_NORMAL,	// ブレーキ
#if _IPHONE
	GME_PLY_MODEL_ID_SPINJUMP,	// スピン状態
	GME_PLY_MODEL_ID_SPINJUMP,	// スピン状態 小さいタイプ
#else
	GME_PLY_MODEL_ID_NORMAL,	// スピン状態
	GME_PLY_MODEL_ID_NORMAL,	// スピン状態 小さいタイプ
#endif // _IPHONE
	GME_PLY_MODEL_ID_SPIN,		// スピン モデル移行
	GME_PLY_MODEL_ID_SPIN,		// スピン 加速
	GME_PLY_MODEL_ID_SPIN,		// スピンダッシュ
	GME_PLY_MODEL_ID_SPIN,		// ホーミング
	GME_PLY_MODEL_ID_NORMAL,	// ホーミング跳ね返り1
	GME_PLY_MODEL_ID_NORMAL,	// よろける前
	GME_PLY_MODEL_ID_NORMAL,	// よろける後
	GME_PLY_MODEL_ID_NORMAL,	// よろける危険

	GME_PLY_MODEL_ID_NORMAL,	// ダメージ
	GME_PLY_MODEL_ID_NORMAL,	// 死亡始まり
	GME_PLY_MODEL_ID_NORMAL,	// 死亡ループ

#if _IPHONE
	GME_PLY_MODEL_ID_SPINJUMP,	// スピンジャンプ
#else
	GME_PLY_MODEL_ID_NORMAL,	// スピンジャンプ
#endif // _IPHONE
	GME_PLY_MODEL_ID_NORMAL,	// ジャンプ通常落下
	GME_PLY_MODEL_ID_NORMAL,	// ジャンプ通常落下中ターン
	GME_PLY_MODEL_ID_NORMAL,	// ジャンプ一定角度以上時落下
	GME_PLY_MODEL_ID_NORMAL,	// ジャンプ一定角度以上時落下中ターン

	GME_PLY_MODEL_ID_NORMAL,	// ギミックジャンプ 上昇ループ
	GME_PLY_MODEL_ID_NORMAL,	// ギミックジャンプ 下降開始
	GME_PLY_MODEL_ID_NORMAL,	// ギミックジャンプ 下降ループ

	GME_PLY_MODEL_ID_NORMAL,	// ギミック前転ジャンプ 上昇>下降開始
	GME_PLY_MODEL_ID_NORMAL,	// ギミック前転ジャンプ 下降ループ

	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 開始1
	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 開始2～溜め
	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 溜めループ
	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 溜め～変身
	GME_PLY_MODEL_ID_NORMAL,	// スーパーソニック変身 キメループ
	GME_PLY_MODEL_ID_NORMAL,	// ゴール 開始
	GME_PLY_MODEL_ID_NORMAL,	// ゴール ループ

	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり
	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり 前移動
	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり 後ろ移動
	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり 反動
	GME_PLY_MODEL_ID_NORMAL,	// 大岩 移動
	GME_PLY_MODEL_ID_NORMAL,	// 大岩 おっとっと
	GME_PLY_MODEL_ID_NORMAL,	// 息継ぎ
	GME_PLY_MODEL_ID_NORMAL,	// ターザンロープ
	GME_PLY_MODEL_ID_NORMAL,	// ターザンロープ 停止
	GME_PLY_MODEL_ID_NORMAL,	// ウォータースライダー
	GME_PLY_MODEL_ID_NORMAL,	// 滑車 ぶら下がり スタート
#if _IPHONE
	GME_PLY_MODEL_ID_SPINJUMP,	// 大砲 発射
#else
	GME_PLY_MODEL_ID_NORMAL,	// 大砲 発射
#endif // _IPHONE
	GME_PLY_MODEL_ID_NORMAL,	// 息継ぎ ジャンプ中
	GME_PLY_MODEL_ID_NORMAL,	// トロッコFW
	GME_PLY_MODEL_ID_NORMAL,	// トロッコFW 左
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ走り
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ走り 左
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ着地
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ危険発生
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ危険発生中
	GME_PLY_MODEL_ID_NORMAL,	// トロッコ危険回避
	GME_PLY_MODEL_ID_NORMAL,	// エンディング手前を見るまで
	GME_PLY_MODEL_ID_NORMAL,	// エンディング手前を見る
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ１
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ２(キメ)
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ１ type2
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ２ type2(キメ)
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ@スーパーソニック１
	GME_PLY_MODEL_ID_NORMAL,	// エンディングフィニッシュ@スーパーソニック２(キメ)

};

/// プレイヤーモデルIDリストテーブル
const u8 *g_gm_player_model_tbl[GSD_CHAR_ID_MAX] = {
	gm_player_model_list_son,		// ソニック
	gm_player_model_list_son,		// スーパーソニック
	gm_player_model_list_son,		// スペステソニック
	gm_player_model_list_pn_son,	// ピンボールソニック
	gm_player_model_list_pn_son,	// ピンボールスーパーソニック
	gm_player_model_list_tr_son,	// トロッコソニック
	gm_player_model_list_tr_son,	// トロッコスーパーソニック
};


//===========================================================================
// モーションブレンド設定
//===========================================================================
/// ソニック, スーパーソニック, スペステソニック モーションブレンド設定
const u8 gm_player_mtn_blend_setting_son[/*GME_PLY_MTN_ID_MAX*/] = {
	// 変更前後がFALSEの時、補完しない

	TRUE,		// FW
	TRUE,		// FW おまけ
	TRUE,		// 待機 0-1
	TRUE,		// 待機 0-2
	TRUE,		// 待機 1-1
	TRUE,		// 待機 1-2
	TRUE,		// 待機 2-1
	TRUE,		// 待機 2-2
	TRUE,		// 振り向き FW・歩き
	FALSE,		// 振り向き 走り
	FALSE,		// 振り向き ブレーキ中
	TRUE,		// 見上げ 開始
	TRUE,		// 見上げ 中
	TRUE,		// 見上げ 終了
	TRUE,		// しゃがみ 開始
	TRUE,		// しゃがみ 中
	TRUE,		// しゃがみ 終了
	TRUE,		// 押す
	TRUE,		// 押す中

	TRUE,		// 歩き
	TRUE,		// 走り
	TRUE,		// ダッシュ1
	TRUE,		// ダッシュ2

	TRUE,		// ブレーキ 開始
	TRUE,		// ブレーキ 踏ん張り
	TRUE,		// ブレーキ 戻り
	TRUE,		// スピン状態
	TRUE,		// スピン状態 小さいタイプ
	FALSE,		// スピン モデル移行
	FALSE,		// スピン加速
	FALSE,		// スピンダッシュ
	FALSE,		// ホーミング
	TRUE,		// ホーミング跳ね返り1
	TRUE,		// よろける前
	TRUE,		// よろける後
	TRUE,		// よろける危険

	TRUE,		// ダメージ
	TRUE,		// 死亡始まり
	FALSE,		// 死亡ループ

	TRUE,		// スピンジャンプ
	TRUE,		// ジャンプ通常落下
	FALSE,		// ジャンプ通常落下中ターン
	TRUE,		// ジャンプ一定角度以上時落下
	FALSE,		// ジャンプ一定角度以上時落下中ターン

	TRUE,		// ギミックジャンプ 上昇ループ
	TRUE,		// ギミックジャンプ 下降開始
	TRUE,		// ギミックジャンプ 下降ループ

	TRUE,		// ギミック前転ジャンプ 上昇>下降開始
	TRUE,		// ギミック前転ジャンプ 下降ループ

	TRUE,		// スーパーソニック変身 開始1
	TRUE,		// スーパーソニック変身 開始2～溜め
	TRUE,		// スーパーソニック変身 溜めループ
	TRUE,		// スーパーソニック変身 溜め～変身
	TRUE,		// スーパーソニック変身 キメループ
	TRUE,		// ゴール 開始
	FALSE,		// ゴール ループ

	TRUE,		// 滑車 ぶら下がり
	TRUE,		// 滑車 ぶら下がり 前移動
	TRUE,		// 滑車 ぶら下がり 後ろ移動
	TRUE,		// 滑車 ぶら下がり 反動
	TRUE,		// 大岩 移動
	TRUE,		// 大岩 おっとっと
	FALSE,		// 息継ぎ
	TRUE,		// ターザンロープ
	TRUE,		// ターザンロープ 停止
	FALSE,		// ウォータースライダー
	TRUE,		// 滑車 ぶら下がり スタート
	TRUE,		// 大砲 発射
	FALSE,		// 息継ぎ ジャンプ中
	TRUE,		// トロッコFW
	TRUE,		// トロッコFW 左
	TRUE,		// トロッコ走り
	TRUE,		// トロッコ走り 左
	TRUE,		// トロッコ着地
	TRUE,		// トロッコ危険発生
	TRUE,		// トロッコ危険発生中
	TRUE,		// トロッコ危険回避
	TRUE,		// エンディング手前を見るまで
	TRUE,		// エンディング手前を見る
	TRUE,		// エンディングフィニッシュ１
	TRUE,		// エンディングフィニッシュ２(キメ)
	TRUE,		// エンディングフィニッシュ１ type2
	TRUE,		// エンディングフィニッシュ２ type2(キメ)
	TRUE,		// エンディングフィニッシュ@スーパーソニック１
	FALSE,		// エンディングフィニッシュ@スーパーソニック２(キメ)
};

/// ピンボールソニック モーションブレンド設定
const u8 gm_player_mtn_blend_setting_pn_son[/*GME_PLY_MTN_ID_MAX*/] = {
	// 変更前後がFALSEの時、補完しない

	TRUE,		// FW
	TRUE,		// FW おまけ
	TRUE,		// 待機 0-1
	TRUE,		// 待機 0-2
	TRUE,		// 待機 1-1
	TRUE,		// 待機 1-2
	TRUE,		// 待機 2-1
	TRUE,		// 待機 2-2
	TRUE,		// 振り向き FW・歩き
	TRUE,		// 振り向き 走り
	TRUE,		// 振り向き ブレーキ中
	TRUE,		// 見上げ 開始
	TRUE,		// 見上げ 中
	TRUE,		// 見上げ 終了
	TRUE,		// しゃがみ 開始
	TRUE,		// しゃがみ 中
	TRUE,		// しゃがみ 終了
	TRUE,		// 押す
	TRUE,		// 押す中

	TRUE,		// 歩き
	TRUE,		// 走り
	TRUE,		// ダッシュ1
	TRUE,		// ダッシュ2

	TRUE,		// ブレーキ 開始
	TRUE,		// ブレーキ 踏ん張り
	TRUE,		// ブレーキ 戻り
	TRUE,		// スピン状態
	TRUE,		// スピン状態 小さいタイプ
	FALSE,		// スピン モデル移行
	FALSE,		// スピン加速
	FALSE,		// スピンダッシュ
	TRUE,		// ホーミング
	TRUE,		// ホーミング跳ね返り1
	TRUE,		// よろける前
	TRUE,		// よろける後
	TRUE,		// よろける危険

	TRUE,		// ダメージ
	TRUE,		// 死亡始まり
	FALSE,		// 死亡ループ

	TRUE,		// スピンジャンプ
	TRUE,		// ジャンプ通常落下
	FALSE,		// ジャンプ通常落下中ターン
	TRUE,		// ジャンプ一定角度以上時落下
	FALSE,		// ジャンプ一定角度以上時落下中ターン

	TRUE,		// ギミックジャンプ 上昇ループ
	TRUE,		// ギミックジャンプ 下降開始
	TRUE,		// ギミックジャンプ 下降ループ

	TRUE,		// ギミック前転ジャンプ 上昇>下降開始
	TRUE,		// ギミック前転ジャンプ 下降ループ

	TRUE,		// スーパーソニック変身 開始1
	TRUE,		// スーパーソニック変身 開始2～溜め
	TRUE,		// スーパーソニック変身 溜めループ
	TRUE,		// スーパーソニック変身 溜め～変身
	TRUE,		// スーパーソニック変身 キメループ
	TRUE,		// ゴール 開始
	FALSE,		// ゴール ループ

	TRUE,		// 滑車 ぶら下がり
	TRUE,		// 滑車 ぶら下がり 前移動
	TRUE,		// 滑車 ぶら下がり 後ろ移動
	TRUE,		// 滑車 ぶら下がり 反動
	TRUE,		// 大岩 移動
	TRUE,		// 大岩 おっとっと
	TRUE,		// 息継ぎ
	TRUE,		// ターザンロープ
	TRUE,		// ターザンロープ 停止
	TRUE,		// ウォータースライダー
	TRUE,		// 滑車 ぶら下がり スタート
	TRUE,		// 大砲 発射
	FALSE,		// 息継ぎ ジャンプ中
	TRUE,		// トロッコFW
	TRUE,		// トロッコFW 左
	TRUE,		// トロッコ走り
	TRUE,		// トロッコ走り 左
	TRUE,		// トロッコ着地
	TRUE,		// トロッコ危険発生
	TRUE,		// トロッコ危険発生中
	TRUE,		// トロッコ危険回避
	TRUE,		// エンディング手前を見るまで
	TRUE,		// エンディング手前を見る
	TRUE,		// エンディングフィニッシュ１
	TRUE,		// エンディングフィニッシュ２(キメ)
	TRUE,		// エンディングフィニッシュ１ type2
	TRUE,		// エンディングフィニッシュ２ type2(キメ)
	TRUE,		// エンディングフィニッシュ@スーパーソニック１
	FALSE,		// エンディングフィニッシュ@スーパーソニック２(キメ)
};

/// トロッコソニック モーションブレンド設定
const u8 gm_player_mtn_blend_setting_tr_son[/*GME_PLY_MTN_ID_MAX*/] = {
	// 変更前後がFALSEの時、補完しない

	TRUE,		// FW
	TRUE,		// FW おまけ
	TRUE,		// 待機 0-1
	TRUE,		// 待機 0-2
	TRUE,		// 待機 1-1
	TRUE,		// 待機 1-2
	TRUE,		// 待機 2-1
	TRUE,		// 待機 2-2
	TRUE,		// 振り向き FW・歩き
	FALSE,		// 振り向き 走り
	FALSE,		// 振り向き ブレーキ中
	TRUE,		// 見上げ 開始
	TRUE,		// 見上げ 中
	TRUE,		// 見上げ 終了
	TRUE,		// しゃがみ 開始
	TRUE,		// しゃがみ 中
	TRUE,		// しゃがみ 終了
	TRUE,		// 押す
	TRUE,		// 押す中

	TRUE,		// 歩き
	TRUE,		// 走り
	TRUE,		// ダッシュ1
	TRUE,		// ダッシュ2

	TRUE,		// ブレーキ 開始
	TRUE,		// ブレーキ 踏ん張り
	TRUE,		// ブレーキ 戻り
	TRUE,		// スピン状態
	TRUE,		// スピン状態 小さいタイプ
	FALSE,		// スピン モデル移行
	FALSE,		// スピン加速
	FALSE,		// スピンダッシュ
	FALSE,		// ホーミング
	TRUE,		// ホーミング跳ね返り1
	TRUE,		// よろける前
	TRUE,		// よろける後
	TRUE,		// よろける危険

	TRUE,		// ダメージ
	FALSE,		// 死亡始まり
	FALSE,		// 死亡ループ

	TRUE,		//!< スピンジャンプ
	TRUE,		//!< ジャンプ通常落下
	FALSE,		// ジャンプ通常落下中ターン
	TRUE,		// ジャンプ一定角度以上時落下
	FALSE,		// ジャンプ一定角度以上時落下中ターン

	TRUE,		//!< ギミックジャンプ 上昇ループ
	TRUE,		//!< ギミックジャンプ 下降開始
	TRUE,		//!< ギミックジャンプ 下降ループ

	TRUE,		//!< ギミック前転ジャンプ 上昇>下降開始
	TRUE,		//!< ギミック前転ジャンプ 下降ループ

	TRUE,		// スーパーソニック変身 開始1
	TRUE,		// スーパーソニック変身 開始2～溜め
	TRUE,		// スーパーソニック変身 溜めループ
	TRUE,		// スーパーソニック変身 溜め～変身
	TRUE,		// スーパーソニック変身 キメループ
	TRUE,		// ゴール 開始
	FALSE,		// ゴール ループ

	TRUE,		// 滑車 ぶら下がり
	TRUE,		// 滑車 ぶら下がり 前移動
	TRUE,		// 滑車 ぶら下がり 後ろ移動
	TRUE,		// 滑車 ぶら下がり 反動
	TRUE,		// 大岩 移動
	TRUE,		// 大岩 おっとっと
	FALSE,		// 息継ぎ
	TRUE,		// ターザンロープ
	TRUE,		// ターザンロープ 停止
	FALSE,		// ウォータースライダー
	TRUE,		// 滑車 ぶら下がり スタート
	TRUE,		// 大砲 発射
	FALSE,		// 息継ぎ ジャンプ中
	TRUE,		// トロッコFW
	TRUE,		// トロッコFW 左
	TRUE,		// トロッコ走り
	TRUE,		// トロッコ走り 左
	TRUE,		// トロッコ着地
	TRUE,		// トロッコ危険発生
	TRUE,		// トロッコ危険発生中
	TRUE,		// トロッコ危険回避
	TRUE,		// エンディング手前を見るまで
	TRUE,		// エンディング手前を見る
	TRUE,		// エンディングフィニッシュ１
	TRUE,		// エンディングフィニッシュ２(キメ)
	TRUE,		// エンディングフィニッシュ１ type2
	TRUE,		// エンディングフィニッシュ２ type2(キメ)
	TRUE,		// エンディングフィニッシュ@スーパーソニック１
	FALSE,		// エンディングフィニッシュ@スーパーソニック２(キメ)
};

/// プレイヤーモーションブレンディング設定リストテーブル
const u8 *g_gm_player_mtn_blend_setting_tbl[GSD_CHAR_ID_MAX] = {
	gm_player_mtn_blend_setting_son,		// ソニック
	gm_player_mtn_blend_setting_son,		// スーパーソニック
	gm_player_mtn_blend_setting_son,		// スペステソニック
	gm_player_mtn_blend_setting_pn_son,		// ピンボールソニック
	gm_player_mtn_blend_setting_pn_son,		// ピンボールスーパーソニック
	gm_player_mtn_blend_setting_tr_son,		// トロッコソニック
	gm_player_mtn_blend_setting_tr_son,		// トロッコスーパーソニック
};


//===========================================================================
// パラメーターテーブル
//===========================================================================
const GMS_PLY_PARAMETER g_gm_player_parameter[GSD_CHAR_ID_MAX] = {
	// ソニック
	{
		GMD_PL_SONIC_SPDAD,			// 通常 加速値
		GMD_PL_SONIC_SPDMA,			// 通常 最大速度値
		GMD_PL_SONIC_SPDDO,			// 通常放置 減速値
		GMD_PL_SONIC_SPIN_SPD,		// スピン 初速値
		GMD_PL_SONIC_SPIN_SPDAD,	// スピン 加速値
		GMD_PL_SONIC_SPIN_SPDMA,	// スピン 最大速度値
		GMD_PL_SONIC_SPIN_SPDDO,	// スピン放置 減速値
//		GMD_PL_SONIC_BOOST_SPDMA,	// ブースト最大速度
//		GMD_PL_SONIC_NITRO_SPDAD,	// ニトロ 加速値
//		GMD_PL_SONIC_NITRO_SPDMA,	// ニトロ 最大速度値
//		GMD_PL_SONIC_NITRO_SPDDO,	// ニトロ放置 減速値
//		GMD_PL_SONIC_NITRO_CHECK,	// ニトロダウン（解除
		GMD_PL_SONIC_SLOPE_SPD,		// 坂道時の最大速度アップ値
		GMD_PL_SONIC_AIR_COUNT,		// 空気の持ち時間
		GMD_PL_SONIC_DAMAGE_TIME,	// ダメージ後の無敵時間
		GMD_PL_SONIC_POOL_TIME,		// ブーストまでの時間
		GMD_PL_SONIC_FALL_TIME,		// 落下待ち時間
		GMD_PL_SONIC_KEI_SPD,		// 傾斜加速度
		GMD_PL_SONIC_KEI_SPDMA,		// 傾斜最大速度
		GMD_PL_SONIC_KEI_SPD_SPIN,	// 傾斜回転時加速度
		GMD_PL_SONIC_KEI_SPD_SPIN_S,// Ｓ字パイプ傾斜回転時加速度
		GMD_PL_SONIC_KEI_SPD_P_SPIN,// ピンボール傾斜回転時加速度
		GMD_PL_JUMP_SPD,			// ジャンプ速度
		GMD_PL_FALL_SPDAD,			// 落下加速度
		GMD_PL_FALL_SPDMA,			// 落下最大速度
		GMD_PL_SONIC_PUSH_SPD,		// 押し最大速度
		GMD_PL_SONIC_JUMP_SPDAD,	// ジャンプ 横方向 加速値
		GMD_PL_SONIC_JUMP_SPDMA,	// ジャンプ 横方向 最大速度値
		GMD_PL_SONIC_JUMP_SPDDO,	// ジャンプ 横方向 減速値
		GMD_PL_SONIC_SPIN_PINBALL_SPDAD,		// ピンボールスピン 加速値
		GMD_PL_SONIC_SPIN_PINBALL_SPDMA,		// ピンボールスピン 最大速度値
		GMD_PL_SONIC_SPIN_PINBALL_SPDDO,		// ピンボールスピン放置 減速値
		GMD_PL_SONIC_SPIN_PINBALL_SLOPE_SPD,	// 便ボールスピン坂道時の最大速度アップ値
	},
	// スーパーソニック
	{
		GMD_PL_SSONIC_SPDAD,			// 通常 加速値
		GMD_PL_SSONIC_SPDMA,			// 通常 最大速度値
		GMD_PL_SSONIC_SPDDO,			// 通常放置 減速値
		GMD_PL_SSONIC_SPIN_SPD,		// スピン 初速値
		GMD_PL_SSONIC_SPIN_SPDAD,	// スピン 加速値
		GMD_PL_SSONIC_SPIN_SPDMA,	// スピン 最大速度値
		GMD_PL_SSONIC_SPIN_SPDDO,	// スピン放置 減速値
//		GMD_PL_SONIC_BOOST_SPDMA,	// ブースト最大速度
//		GMD_PL_SONIC_NITRO_SPDAD,	// ニトロ 加速値
//		GMD_PL_SONIC_NITRO_SPDMA,	// ニトロ 最大速度値
//		GMD_PL_SONIC_NITRO_SPDDO,	// ニトロ放置 減速値
//		GMD_PL_SONIC_NITRO_CHECK,	// ニトロダウン（解除
		GMD_PL_SSONIC_SLOPE_SPD,		// 坂道時の最大速度アップ値
		GMD_PL_SONIC_AIR_COUNT,		// 空気の持ち時間
		GMD_PL_SONIC_DAMAGE_TIME,	// ダメージ後の無敵時間
		GMD_PL_SONIC_POOL_TIME,		// ブーストまでの時間
		GMD_PL_SONIC_FALL_TIME,		// 落下待ち時間
		GMD_PL_SSONIC_KEI_SPD,		// 傾斜加速度
		GMD_PL_SSONIC_KEI_SPDMA,		// 傾斜最大速度
		GMD_PL_SSONIC_KEI_SPD_SPIN,	// 傾斜回転時加速度
		GMD_PL_SSONIC_KEI_SPD_SPIN_S,// Ｓ字パイプ傾斜回転時加速度
		GMD_PL_SSONIC_KEI_SPD_P_SPIN,// ピンボール傾斜回転時加速度
		GMD_PL_SSONIC_JUMP_SPD,		// ジャンプ速度
		GMD_PL_FALL_SPDAD,			// 落下加速度
		GMD_PL_FALL_SPDMA,			// 落下最大速度
		GMD_PL_SONIC_PUSH_SPD,		// 押し最大速度
		GMD_PL_SSONIC_JUMP_SPDAD,	// ジャンプ 横方向 加速値
		GMD_PL_SSONIC_JUMP_SPDMA,	// ジャンプ 横方向 最大速度値
		GMD_PL_SSONIC_JUMP_SPDDO,	// ジャンプ 横方向 減速値
		GMD_PL_SSONIC_SPIN_PINBALL_SPDAD,		// ピンボールスピン 加速値
		GMD_PL_SSONIC_SPIN_PINBALL_SPDMA,		// ピンボールスピン 最大速度値
		GMD_PL_SSONIC_SPIN_PINBALL_SPDDO,		// ピンボールスピン放置 減速値
		GMD_PL_SSONIC_SPIN_PINBALL_SLOPE_SPD,	// 便ボールスピン坂道時の最大速度アップ値
	},
	// スペステソニック
	{
		GMD_PL_SONIC_SPDAD,			// 通常 加速値
		GMD_PL_SONIC_SPDMA,			// 通常 最大速度値
		GMD_PL_SONIC_SPDDO,			// 通常放置 減速値
		GMD_PL_SONIC_SPIN_SPD,		// スピン 初速値
		GMD_PL_SONIC_SPIN_SPDAD,	// スピン 加速値
		GMD_PL_SONIC_SPIN_SPDMA,	// スピン 最大速度値
		GMD_PL_SONIC_SPIN_SPDDO,	// スピン放置 減速値
//		GMD_PL_SONIC_BOOST_SPDMA,	// ブースト最大速度
//		GMD_PL_SONIC_NITRO_SPDAD,	// ニトロ 加速値
//		GMD_PL_SONIC_NITRO_SPDMA,	// ニトロ 最大速度値
//		GMD_PL_SONIC_NITRO_SPDDO,	// ニトロ放置 減速値
//		GMD_PL_SONIC_NITRO_CHECK,	// ニトロダウン（解除
		GMD_PL_SONIC_SLOPE_SPD,		// 坂道時の最大速度アップ値
		GMD_PL_SONIC_AIR_COUNT,		// 空気の持ち時間
		GMD_PL_SONIC_DAMAGE_TIME,	// ダメージ後の無敵時間
		GMD_PL_SONIC_POOL_TIME,		// ブーストまでの時間
		GMD_PL_SONIC_FALL_TIME,		// 落下待ち時間
		GMD_PL_SPL_KEI_SPD,			// 傾斜加速度
		GMD_PL_SPL_KEI_SPDMA,		// 傾斜最大速度
		GMD_PL_SONIC_KEI_SPD_SPIN,	// 傾斜回転時加速度
		GMD_PL_SONIC_KEI_SPD_SPIN_S,// Ｓ字パイプ傾斜回転時加速度
		GMD_PL_SONIC_KEI_SPD_P_SPIN,// ピンボール傾斜回転時加速度
		GMD_PL_SPL_JUMP_SPD,		// ジャンプ速度
		GMD_PL_SPL_FALL_SPDAD,		// 落下加速度
		GMD_PL_SPL_FALL_SPDMA,		// 落下最大速度
		GMD_PL_SONIC_PUSH_SPD,		// 押し最大速度
		GMD_PL_SONIC_JUMP_SPDAD,	// ジャンプ 横方向 加速値
		GMD_PL_SONIC_JUMP_SPDMA,	// ジャンプ 横方向 最大速度値
		GMD_PL_SONIC_JUMP_SPDDO,	// ジャンプ 横方向 減速値
		GMD_PL_SONIC_SPIN_PINBALL_SPDAD,		// ピンボールスピン 加速値
		GMD_PL_SONIC_SPIN_PINBALL_SPDMA,		// ピンボールスピン 最大速度値
		GMD_PL_SONIC_SPIN_PINBALL_SPDDO,		// ピンボールスピン放置 減速値
		GMD_PL_SONIC_SPIN_PINBALL_SLOPE_SPD,	// 便ボールスピン坂道時の最大速度アップ値
	},
	// ピンボールソニック
	{
		GMD_PL_SONIC_SPDAD,			// 通常 加速値
		GMD_PL_SONIC_SPDMA,			// 通常 最大速度値
		GMD_PL_SONIC_SPDDO,			// 通常放置 減速値
		GMD_PL_SONIC_SPIN_SPD,		// スピン 初速値
		GMD_PL_SONIC_SPIN_SPDAD,	// スピン 加速値
		GMD_PL_SONIC_SPIN_SPDMA,	// スピン 最大速度値
		GMD_PL_SONIC_SPIN_SPDDO,	// スピン放置 減速値
//		GMD_PL_SONIC_BOOST_SPDMA,	// ブースト最大速度
//		GMD_PL_SONIC_NITRO_SPDAD,	// ニトロ 加速値
//		GMD_PL_SONIC_NITRO_SPDMA,	// ニトロ 最大速度値
//		GMD_PL_SONIC_NITRO_SPDDO,	// ニトロ放置 減速値
//		GMD_PL_SONIC_NITRO_CHECK,	// ニトロダウン（解除
		GMD_PL_SONIC_SLOPE_SPD,		// 坂道時の最大速度アップ値
		GMD_PL_SONIC_AIR_COUNT,		// 空気の持ち時間
		GMD_PL_SONIC_DAMAGE_TIME,	// ダメージ後の無敵時間
		GMD_PL_SONIC_POOL_TIME,		// ブーストまでの時間
		GMD_PL_SONIC_FALL_TIME,		// 落下待ち時間
		GMD_PL_SONIC_KEI_SPD,		// 傾斜加速度
		GMD_PL_SONIC_KEI_SPDMA,		// 傾斜最大速度
		GMD_PL_SONIC_KEI_SPD_SPIN,	// 傾斜回転時加速度
		GMD_PL_SONIC_KEI_SPD_SPIN_S,// Ｓ字パイプ傾斜回転時加速度
		GMD_PL_SONIC_KEI_SPD_P_SPIN,// ピンボール傾斜回転時加速度
		GMD_PL_JUMP_SPD,			// ジャンプ速度
		GMD_PL_FALL_SPDAD,			// 落下加速度
		GMD_PL_FALL_SPDMA,			// 落下最大速度
		GMD_PL_SONIC_PUSH_SPD,		// 押し最大速度
		GMD_PL_SONIC_JUMP_SPDAD,	// ジャンプ 横方向 加速値
		GMD_PL_SONIC_JUMP_SPDMA,	// ジャンプ 横方向 最大速度値
		GMD_PL_SONIC_JUMP_SPDDO,	// ジャンプ 横方向 減速値
		GMD_PL_SONIC_SPIN_PINBALL_SPDAD,	// ピンボールスピン 加速値
		GMD_PL_SONIC_SPIN_PINBALL_SPDMA,	// ピンボールスピン 最大速度値
		GMD_PL_SONIC_SPIN_PINBALL_SPDDO,	// ピンボールスピン放置 減速値
		GMD_PL_SONIC_SPIN_PINBALL_SLOPE_SPD,	// 便ボールスピン坂道時の最大速度アップ値
	},
	// ピンボールスーパーソニック
	{
		GMD_PL_SSONIC_SPDAD,			// 通常 加速値
		GMD_PL_SSONIC_SPDMA,			// 通常 最大速度値
		GMD_PL_SSONIC_SPDDO,			// 通常放置 減速値
		GMD_PL_SSONIC_SPIN_SPD,		// スピン 初速値
		GMD_PL_SSONIC_SPIN_SPDAD,	// スピン 加速値
		GMD_PL_SSONIC_SPIN_SPDMA,	// スピン 最大速度値
		GMD_PL_SSONIC_SPIN_SPDDO,	// スピン放置 減速値
//		GMD_PL_SONIC_BOOST_SPDMA,	// ブースト最大速度
//		GMD_PL_SONIC_NITRO_SPDAD,	// ニトロ 加速値
//		GMD_PL_SONIC_NITRO_SPDMA,	// ニトロ 最大速度値
//		GMD_PL_SONIC_NITRO_SPDDO,	// ニトロ放置 減速値
//		GMD_PL_SONIC_NITRO_CHECK,	// ニトロダウン（解除
		GMD_PL_SSONIC_SLOPE_SPD_PINBALL,		// 坂道時の最大速度アップ値
		GMD_PL_SONIC_AIR_COUNT,		// 空気の持ち時間
		GMD_PL_SONIC_DAMAGE_TIME,	// ダメージ後の無敵時間
		GMD_PL_SONIC_POOL_TIME,		// ブーストまでの時間
		GMD_PL_SONIC_FALL_TIME,		// 落下待ち時間
		GMD_PL_SSONIC_KEI_SPD,		// 傾斜加速度
		GMD_PL_SSONIC_KEI_SPDMA,		// 傾斜最大速度
		GMD_PL_SSONIC_KEI_SPD_SPIN_FOR_PINBALL,	// 回転時傾斜加速度
		GMD_PL_SSONIC_KEI_SPD_SPIN_S,// Ｓ字パイプ回転時傾斜加速度
		GMD_PL_SSONIC_KEI_SPD_P_SPIN,// ピンボール回転時傾斜加速度
		GMD_PL_SSONIC_JUMP_SPD,		// ジャンプ速度
		GMD_PL_FALL_SPDAD,			// 落下加速度
		GMD_PL_FALL_SPDMA,			// 落下最大速度
		GMD_PL_SONIC_PUSH_SPD,		// 押し最大速度
		GMD_PL_SSONIC_JUMP_SPDAD,	// ジャンプ 横方向 加速値
		GMD_PL_SSONIC_JUMP_SPDMA,	// ジャンプ 横方向 最大速度値
		GMD_PL_SSONIC_JUMP_SPDDO,	// ジャンプ 横方向 減速値
		GMD_PL_SSONIC_SPIN_PINBALL_SPDAD,		// ピンボールスピン 加速値
		GMD_PL_SSONIC_SPIN_PINBALL_SPDMA,		// ピンボールスピン 最大速度値
		GMD_PL_SSONIC_SPIN_PINBALL_SPDDO,		// ピンボールスピン放置 減速値
		GMD_PL_SSONIC_SPIN_PINBALL_SLOPE_SPD,	// 便ボールスピン坂道時の最大速度アップ値 
	},
	// トロッコソニック
	{
		GMD_PL_SONIC_SPDAD_TRUCK,	// 通常 加速値
		GMD_PL_SONIC_SPDMA_TRUCK,	// 通常 最大速度値
		GMD_PL_SONIC_SPDDO_TRUCK,	// 通常放置 減速値
		GMD_PL_SONIC_SPIN_SPD,		// スピン 初速値
		GMD_PL_SONIC_SPIN_SPDAD,	// スピン 加速値
		GMD_PL_SONIC_SPIN_SPDMA,	// スピン 最大速度値
		GMD_PL_SONIC_SPIN_SPDDO,	// スピン放置 減速値
//		GMD_PL_SONIC_BOOST_SPDMA,	// ブースト最大速度
//		GMD_PL_SONIC_NITRO_SPDAD,	// ニトロ 加速値
//		GMD_PL_SONIC_NITRO_SPDMA,	// ニトロ 最大速度値
//		GMD_PL_SONIC_NITRO_SPDDO,	// ニトロ放置 減速値
//		GMD_PL_SONIC_NITRO_CHECK,	// ニトロダウン（解除
		GMD_PL_SONIC_SLOPE_SPD_TRUCK,	// 坂道時の最大速度アップ値
		GMD_PL_SONIC_AIR_COUNT,		// 空気の持ち時間
		GMD_PL_SONIC_DAMAGE_TIME,	// ダメージ後の無敵時間
		GMD_PL_SONIC_POOL_TIME,		// ブーストまでの時間
		GMD_PL_SONIC_FALL_TIME_TRUCK,		// 落下待ち時間
		GMD_PL_SONIC_KEI_SPD_TRUCK,	// 傾斜加速度
		GMD_PL_SONIC_KEI_SPDMA_TRUCK,	// 傾斜最大速度
		GMD_PL_SONIC_KEI_SPD_TRUCK,	// 傾斜回転時加速度
		GMD_PL_SONIC_KEI_SPD_TRUCK,	// Ｓ字パイプ傾斜回転時加速度
		GMD_PL_SONIC_KEI_SPD_TRUCK,	// ピンボール傾斜回転時加速度
		GMD_PL_TRUCK_JUMP_SPD,		// ジャンプ速度
		GMD_PL_FALL_SPDAD,			// 落下加速度
		GMD_PL_FALL_SPDMA,			// 落下最大速度
		GMD_PL_SONIC_PUSH_SPD,		// 押し最大速度
		GMD_PL_SONIC_JUMP_SPDAD_TRUCK,	// ジャンプ 横方向 加速値
		GMD_PL_SONIC_JUMP_SPDMA_TRUCK,	// ジャンプ 横方向 最大速度値
		GMD_PL_SONIC_JUMP_SPDDO_TRUCK,	// ジャンプ 横方向 減速値
		GMD_PL_SONIC_SPIN_PINBALL_SPDAD,		// ピンボールスピン 加速値
		GMD_PL_SONIC_SPIN_PINBALL_SPDMA,		// ピンボールスピン 最大速度値
		GMD_PL_SONIC_SPIN_PINBALL_SPDDO,		// ピンボールスピン放置 減速値
		GMD_PL_SONIC_SPIN_PINBALL_SLOPE_SPD,	// 便ボールスピン坂道時の最大速度アップ値
	},
	// トロッコスーパーソニック
	{
		GMD_PL_SSONIC_SPDAD_TRUCK,	// 通常 加速値
		GMD_PL_SSONIC_SPDMA_TRUCK,	// 通常 最大速度値
		GMD_PL_SSONIC_SPDDO_TRUCK,	// 通常放置 減速値
		GMD_PL_SSONIC_SPIN_SPD,		// スピン 初速値
		GMD_PL_SSONIC_SPIN_SPDAD,	// スピン 加速値
		GMD_PL_SSONIC_SPIN_SPDMA,	// スピン 最大速度値
		GMD_PL_SSONIC_SPIN_SPDDO,	// スピン放置 減速値
//		GMD_PL_SONIC_BOOST_SPDMA,	// ブースト最大速度
//		GMD_PL_SONIC_NITRO_SPDAD,	// ニトロ 加速値
//		GMD_PL_SONIC_NITRO_SPDMA,	// ニトロ 最大速度値
//		GMD_PL_SONIC_NITRO_SPDDO,	// ニトロ放置 減速値
//		GMD_PL_SONIC_NITRO_CHECK,	// ニトロダウン（解除
		GMD_PL_SSONIC_SLOPE_SPD_TRUCK,	// 坂道時の最大速度アップ値
		GMD_PL_SONIC_AIR_COUNT,		// 空気の持ち時間
		GMD_PL_SONIC_DAMAGE_TIME,	// ダメージ後の無敵時間
		GMD_PL_SONIC_POOL_TIME,		// ブーストまでの時間
		GMD_PL_SONIC_FALL_TIME_TRUCK,		// 落下待ち時間
		GMD_PL_SSONIC_KEI_SPD_TRUCK,// 傾斜加速度
		GMD_PL_SSONIC_KEI_SPDMA_TRUCK,	// 傾斜最大速度
		GMD_PL_SSONIC_KEI_SPD_TRUCK,// 傾斜回転時加速度
		GMD_PL_SSONIC_KEI_SPD_TRUCK,// Ｓ字パイプ傾斜回転時加速度
		GMD_PL_SSONIC_KEI_SPD_TRUCK,// ピンボール傾斜回転時加速度
		GMD_PL_TRUCK_SSONIC_JUMP_SPD,	// ジャンプ速度
		GMD_PL_FALL_SPDAD,			// 落下加速度
		GMD_PL_FALL_SPDMA,			// 落下最大速度
		GMD_PL_SONIC_PUSH_SPD,		// 押し最大速度
		GMD_PL_SSONIC_JUMP_SPDAD_TRUCK,	// ジャンプ 横方向 加速値
		GMD_PL_SSONIC_JUMP_SPDMA_TRUCK,	// ジャンプ 横方向 最大速度値
		GMD_PL_SSONIC_JUMP_SPDDO_TRUCK,	// ジャンプ 横方向 減速値
		GMD_PL_SSONIC_SPIN_PINBALL_SPDAD,		// ピンボールスピン 加速値
		GMD_PL_SSONIC_SPIN_PINBALL_SPDMA,		// ピンボールスピン 最大速度値
		GMD_PL_SSONIC_SPIN_PINBALL_SPDDO,		// ピンボールスピン放置 減速値
		GMD_PL_SSONIC_SPIN_PINBALL_SLOPE_SPD,	// 便ボールスピン坂道時の最大速度アップ値
	},
};

//----- Local Variables -----------------------------------------------------

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
