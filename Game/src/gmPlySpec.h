// ==========================================================================
/*!
  @file gmPlySepc.h
  @brief プレイヤー スペック定義

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmPlySpec.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_PLY_SPEC_H_
#define GM_PLY_SPEC_H_


//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
// ==========================================================================
// 各種プレイヤースペック
// ==========================================================================
#define GMD_PL_DEF_MAX_SPD				(0x0F000)			//!< プレイヤー最高速度

#if _PS3
	#define GMD_PL_DEF_ROLL_MAX			(0x2000)			//!< プレイヤー角度取得最大値
#elif _WII
	#define GMD_PL_DEF_ROLL_MAX			(0x2000)			//!< プレイヤー角度取得最大値
#elif _IPHONE
	#define GMD_PL_DEF_ROLL_MAX			(0x6000)			//!< プレイヤー角度取得最大値
#else// _PC | _XBOX
	#define GMD_PL_DEF_ROLL_MAX			(0x2000)			//!< プレイヤー角度取得最大値
#endif

#if _IPHONE
#define GMD_PL_SJUMP_SPD				(-0x4000)			//!< 小ジャンプチェック速度
#else
#define GMD_PL_SJUMP_SPD				(-0x0400)			//!< 小ジャンプチェック速度
#endif // _IPHONE

// プレイヤー地形判定用矩形
#define GMD_PL_REC_L					(-6)				//!< プレイヤー地形判定矩形 LEFT
#define GMD_PL_REC_T					(-12)				//!< プレイヤー地形判定矩形 TOP
#define GMD_PL_REC_R					( 6)				//!< プレイヤー地形判定矩形 RIGHT
#define GMD_PL_REC_B					( 13)				//!< プレイヤー地形判定矩形 BOTTOM


// 描画オフセット
#define GMD_PL_DISP_OFST_Y				(-(GMD_PL_REC_B+2))	//!< 描画時ずらし量
#define GMD_PL_DISP_OFST_Y_PINBALL		(-(GMD_PL_REC_B+8))	//!< 描画時ずらし量 ピンボール用

#define GMD_PL_DISP_OFST_X_TRUCK		(0)//(10)				//!< 描画時ずらし量 トロッコ用
#define GMD_PL_DISP_OFST_Y_TRUCK		(0)//(5)					//!< 描画時ずらし量 トロッコ用

// ==================================
// よろけ範囲
// ==================================
#define GMD_PL_STAGGER_NORMAL_OFST		(2)					//!< おっとっと通常チェックオフセット
#define GMD_PL_STAGGER_DANGER_OFST		(-4)				//!< おっとっと危険チェックオフセット

// ==================================
// 攻撃反射
// ==================================
#define GMD_PL_ATK_REF_X				(0x1800)
#define GMD_PL_ATK_REF_Y				(-0x4000)
#define GMD_PL_ATK_NOSPD_TIME			(24)

// ==================================
// ホーミング
// ==================================
#define GMD_PL_HOMING_DIR_S				(0)				//!< ホーミング角度 START (逆回転設定)
#define GMD_PL_HOMING_DIR_E				(0x38E3)		//!< ホーミング角度 END (逆回転設定) (0x5000, 0x2E38(65度))
#define GMD_PL_HOMING_DIST				(192*FX32_ONE)	//!< ホーミング距離
#define GMD_PL_HOMING_DIST_UNDER		(128*FX32_ONE)	//!< ホーミング距離 下
#define GMD_PL_HOMING_DIST_PER			((float)((float)GMD_PL_HOMING_DIST/(float)GMD_PL_HOMING_DIST_UNDER))
#define GMD_PL_HOMING_SPD				(0xF000)		//!< ホーミング速度
#define GMD_PL_HOMING_TIME				(32)			//!< ホーミング実行時間
#define GMD_PL_HOMING_WAIT_TIME			(24)			//!< ホーミング待機時間
#define GMD_PL_HOMING_BOOST_TIME		(64)			//!< ホーミング範囲ブースト時間
#define GMD_PL_HOMING_HIT_NOMOVE_TIME	(10)	//!< ホーミングヒット後の移動不可時間

#define GMD_PL_HOMING_REF_SPD_Y			(-5*FX32_ONE)	//!< ホーミング跳ね返り速度

// ==================================
// ジャンプダッシュ
// ==================================
#define GMD_PL_JUMP_DASH_NOSPD_TIME		(8)			//!< ジャンプダッシュ時の減速無し時間
//#define GMD_PL_JUMP_DASH_DIR			(0x1000)	//!< ジャンプダッシュ角度
#define GMD_PL_JUMP_DASH_DIR			(0xF800)	//!< ジャンプダッシュ角度
#define GMD_PL_JUMP_DASH_SPD			(0x4000)	//!< ジャンプダッシュ速度
#define GMD_PL_JUMP_DASH_TIME			(20)		//!< ジャンプダッシュ時間

// オートラン用
#define GMD_PL_JUMP_DASH_AUTO_RUN_SPD			(0x1000)	//!< ジャンプダッシュ速度 オートラン用
#define GMD_PL_JUMP_DASH_AUTO_RUN_NOSPD_TIME	(8)			//!< ジャンプダッシュ時の減速無し時間 オートラン用
#define GMD_PL_JUMP_DASH_AUTO_RUN_TIME			(20)		//!< ジャンプダッシュ時間 オートラン用

// ==================================
// プログラムターン
// ==================================
#define GMD_PL_PGM_TURN_SPD_DEF				(0x1000)	//!< ターン速度

// ==================================
// スーパーソニック
// ==================================
#define GMD_PL_SUPER_SONIC_RING_DEC_INT		(60)		//!< リングが減る間隔(フレーム)
	
// ==========================================================================
// アイテム設定
// ==========================================================================
#define GMD_PL_ITEM_GENOCIDE_TIME		(999)		//!< 無敵アイテム有効時間
#define GMD_PL_ITEM_HISPEED_SCALE		(0x2000)	//!< ハイスピード スピード倍率
#define GMD_PL_ITEM_HISPEED_TIME		(15*60)		//!< ハイスピード時間	サウンドデータの長さにあわせて変更
//#define GMD_PL_ITEM_HISPEED_TIME		(20*60)		//!< ハイスピード時間

// ==========================================================================
// リング設定
// ==========================================================================
#define GMD_PLAYER_RING_NUM_MAX			( 999 )								//!< リング数最大値
#define GMD_PLAYER_RING_STAGE_NUM_MAX	( 9999 )							//!< ステージ累計リング数最大値

// ==========================================================================
// プレイヤーストック設定
// ==========================================================================
//#define GMD_PLAYER_STOCK_DEFAULT_NUM	( 2 )								//!< プレイヤー初期残機
//#define GMD_PLAYER_STOCK_NUM_MAX		( 99 )								//!< ストック人数最大


// ==========================================================================
// スピンダッシュ
// ==========================================================================
#define GMD_PL_SPIN_SPDDA_SHIFT		( 5 )		//!< Spin加速、減速シフト値
#if _IPHONE
#define GMD_PL_SPINDASH_SPD			(0x0bc00)	//!< Spin加速ダッシュ速度値(初速速め)
#define GMD_PL_SPINDASH_MUL			(0x00200)	//!< Spin加速ダッシュ掛け値(倍率低め)
#else
#define GMD_PL_SPINDASH_SPD			(0x08000)	//!< Spin加速ダッシュ速度値
#define GMD_PL_SPINDASH_MUL			(0x00800)	//!< Spin加速ダッシュ掛け値
#endif // _IPHONE
#define GMD_PL_SPINDASH_NOSPD_TIME	(72)		//!< Spinダッシュ時の減速無し時間
#define GMD_PL_SPINDASH_JUMP_NOSPD_TIME	(20)	//!< Spinダッシュジャンプ時の減速無し時間

#define GMD_PL_STOP_SPD				(0x00800)	//!< Spin時などの停止判定を行う値

// ==========================================================================
// MAXダッシュ
// ==========================================================================
#define GMD_PL_MAXDASH_TIME			(30)		//!< MAXダッシュが発動不可状態になってから表示しておく時間
#define GMD_PL_MAXDASH_DIR			(0x1000)	//!< MAXダッシュが発動可能になる角度 (0x1800)

// ==========================================================================
// プレイヤー速度⇔アクション 対応速度定義
// ==========================================================================
#define GMD_PL_1ST_SPD				(0x1400)	//!< Normal Slow (walk)  0x001 ～ 0x140
#define GMD_PL_2ND_SPD				(0x2800)	//!< Normal Low  (run)   0x141 ～ 0x280
#define GMD_PL_3RD_SPD				(0x4000)	//!< Normal Mid  (dash1) 0x281 ～ 0x400
#define GMD_PL_4TH_SPD				(0x7000)	//!< Normal Max  (dash2) 0x401 ～ 0x700
#define GMD_PL_5TH_SPD				(0x9000)	//!< Boost1              0x701 ～ 0x900
#define GMD_PL_MAX_SPD				(0xa000)	//!< Boost2              0x901 ～ 0xa00

#if 0
// ==========================================================================
// ホバー速度
// ==========================================================================
#define GMD_PL_HOVER_SPDAD			(0x00300)
#define GMD_PL_HOVER_SPDDO			(0x00600)
#define GMD_PL_HOVER_N_SPDAD		(0x00380)
#define GMD_PL_HOVER_N_SPDDO		(0x00600)
#define GMD_PL_FALL_H_SPDAD			(0x00080)	//!< ホバー落下
#define GMD_PL_FALL_H_SPDMA			(0x02200) 
#endif


// ==========================================================================
// 落ちるかどうかの分岐点
// ==========================================================================
#define GMD_PL_FALL_SPD				(0x2000)

// ==========================================================================
// 傾斜判定角度
// ==========================================================================
//#define GMD_PL_KEI_DIR				(0x2000)//(0x20 - 4) // 坂道判定角度 45°
//#define GMD_PL_KEI_DIR_SPIN			(0x1000)//(0x10 - 2) // 坂道判定角度 22.5°
#define GMD_PL_KEI_DIR				(0x00100*3/4)//(0x20 - 4) // 坂道判定角度 45°
#define GMD_PL_KEI_DIR_SPIN			(0x00500*4/8)//(0x10 - 2) // 坂道判定角度 22.5°
#define GMD_PL_KEI_DIR_PINBALL_SPIN	(0x00100)
#define GMD_PL_KEI_DIR_TRUCK		(0x200)//(0x00100*3/8)//(0x10 - 2) // 坂道判定角度 トロッコ用

// ==========================================================================
// ダメージ受けたときの移動量(水中 X,Y ともに 1/2)
// ==========================================================================
#define GMD_PL_DAMAGE_JUMP_X		((0x02000*3/4))
#define GMD_PL_DAMAGE_JUMP_Y		(-(0x04000*3/4))
#define GMD_PL_DAMAGE_CHECK_SPD		(0x01800) // この値以下の速度の場合、ダメージによる移動方向は向き依存

// オートラン用
#define GMD_PL_DAMAGE_AUTO_RUN_JUMP_X_LEFT	(-0x6000)//((0x01000*3/4))
#define GMD_PL_DAMAGE_AUTO_RUN_JUMP_X_RIGHT	(0xA000)//((0x01000*3/4))

// ==========================================================================
// ブレーキ
// ==========================================================================
//これ以上の速度から逆レバー入力でブレーキアクション
//#define GMD_PL_BRAKE_PERMIT_SPD		(0x02000)
#define GMD_PL_BRAKE_PERMIT_SPD		(0x04000)

// ==========================================================================
// グラインド
// ==========================================================================
#define GMD_PL_GRAIND_SPDMI			(0x02000)		// 最低速度
#define GMD_PL_GRAIND_KEI_SPD		(0x00280)
#define GMD_PL_GRAIND_KEY_SPD		(0x00100)

#if 0
// ==========================================================================
// カメラずらし
// ==========================================================================
#define GMD_PL_CAMERA_OFST_MAX		(0x58000)		// 最大ずらし値
#define GMD_PL_CAMERA_OFST_MINI_X	(16 << (8 + 4)/*FX32_SHIFT?*/)		// この値以下のずらしを無視する
#define GMD_PL_CAMERA_OFST_MINI_Y	(40 << (8 + 4))
#define GMD_PL_CAMERA_OFST_Y1_MAX	(0x16000)		// 最大ずらし値
#define GMD_PL_CAMERA_OFST_Y2_MAX	(-0x48000)		// 最大ずらし値
#endif

#if 0
// ブレイズホバー時間
#define GMD_PL_BLAZE_HOVER_TIME (120 )

// ブーストに達するための時間(超過速度蓄積量
#define GMD_BOOST_POOL_TIME ( 96 )
#endif

// ==========================================================================
// 放置時の加減速
// ==========================================================================
//#define GMD_PL_ADD_SPD				(0x00080)	// ◆未使用
//#define GMD_PL_RET_SPD				(0x00600)	// ◆未使用
#define GMD_PL_STOP_SPD				(0x00800) // Spin時などの停止判定を行う値


#if 0
// 自力で得れる最高速度
#define GMD_PL_USUAL_TOP_SPD		(0x07800) // 通常時	// ◆未使用
#define GMD_PL_BOOST_TOP_SPD		(0x0a000) // ブースト時	// ◆未使用

// 坂を利用した最高速度
#define GMD_PL_USUAL_MAX_SPD		(0x09800) // 通常時	// ◆未使用
#define GMD_PL_BOOST_MAX_SPD		(0x0c000) // ブースト時	// ◆未使用
#endif

// 水しぶき
#define GMD_PL_SPRASH_TIMER (10)

// Super Sonic
#define GMD_PL_JUMP_SPSONIC			(0x08000*3/4)	// ◆未使用


// ==========================================================================
// 水中設定
// ==========================================================================
#define GMD_PL_WATER_ATTENSION_SE_INT	(5*60)		//!< 水中警告音 呼び出し間隔


// ==========================================================================
// オートラン
// ==========================================================================
#define GMD_PL_AUTO_RUN_MAX_SPD_OFST			(0x2000)	//!< 画面スクロール速度に対する最高速度オフセット
#define GMD_PL_AUTO_RUN_FREE_DEC_MIN_SPD_OFST	(-0x1000)	//!< 画面スクロール速度に対する減速最低速度オフセット

// ==========================================================================
// トロッコ
// ==========================================================================
#define GMD_PL_TRUCK_STICK_TIME					(60)		//!< トロッコ踏ん張り時間
#define GMD_PL_TRUCK_WALK_SPD_MIN				(0x0040)	//!< 歩きをキャンセルする最小速度
#define GMD_PL_TRUCK_LEFT_FLIP_TIMER			(60)		//!< 左振り向き待機時間

#define GMD_PL_TRUCK_TRANS_SUPER_POS_Z			(-8*FX32_ONE)	//!< スーパーソニック変身時Z位置補正値

#define GMD_PL_TRUCK_DANGER_DIR					(0x6B00)//(0x5555)//(0x4000)	//!< トロッコ危険角度
#define GMD_PL_TRUCK_DANGER_DIR_REV				(0x9500)//(0xAAAB)//(0xC000)	//!< トロッコ危険角度 反対側

// ==========================================================================
// スペステ
// ==========================================================================
#define GMD_PL_SS_NUDGING_DI_TIME	(30)			//!< ナッジングDissableTime
#define GMD_PL_SS_NUDGING_TIME		(30)			//!< ナッジング継続時間
#define GMD_PL_SS_NUDGING_WIDTH		(8)				//!< ナッジング振幅幅(dot)
//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _PT_H_

//----- Include Files -------------------------------------------------------
