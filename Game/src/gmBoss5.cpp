// =======================================================================
/*!
  @file	gmBoss5.cpp
  @brief ボスファイナル

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss5.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akMath.h"
#include "akUtil.h"
#include "izFade.h"
#include "gs.h"
#include "gmMain.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmEventTbl.h"
#include "gmCamera.h"
#include "gmSound.h"
#include "gmPadVib.h"
#include "gmEnemy.h"
#include "gmFix.h"
#include "gmMap.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectBoss.h"
#include "gmFade.h"
#include "gmBossCommon.h"
#include "gmBoss5.h"
#include "gmBoss5Rocket.h"
#include "gmBoss5Turret.h"
#include "gmBoss5Egg.h"
#include "gmBoss5Efct.h"
#include "gmBoss5Land.h"
#include "gmBoss5Ctplt.h"
#include "gmDeco.h"

#include "gmPlayer.h"
#include "gmPlySeq.h"
#include "gmPlySeqGmk.h"
#include "gmPlyScoreDef.h"

#include "gmGmkCamScrLim.h"

// データヘッダ
#include "../file/common/arc/BOSS05.hmb"
#include "../file/common/model/BOSS05_MDL.hmb"
#include "../file/common/model/BOSS05_BODY_MTN.hmb"


/*------ Macros --------------------------------------------------------*/
/* デバッグ関連 */
#if defined(MTD_DEBUG)
	//#define GMD_BOSS5_DEBUG_MODELTEST	//!< 仮モデル表示テスト
	//#define GMD_BOSS5_DEBUG_SEQ_TEST	//!< シーケンステスト
	#define GMD_BOSS5_DEBUG_PROGRESS_CHECK	//!< 進捗チェック
#endif /* defined(MTD_DEBUG) */

/* ファイル関連 */
#define GMD_BOSS5_ARC	(g_gm_gamedat_enemy_arc)

//############ ノード番号 #####################################################
// 本体
#define GMD_BOSS5_BODY_NODE_IDX_BODY		(2)		//!< 胴体
#define GMD_BOSS5_BODY_NODE_IDX_FOOT_L		(12)	//!< 左足（つま先）
#define GMD_BOSS5_BODY_NODE_IDX_FOOT_R		(20)	//!< 右足（つま先）
#define GMD_BOSS5_BODY_NODE_IDX_LEG_L		(11)	//!< 左脚（「足」の付け根あたり）
#define GMD_BOSS5_BODY_NODE_IDX_LEG_R		(18)	//!< 右脚（「足」の付け根あたり）
#define GMD_BOSS5_BODY_NODE_IDX_POLE		(29)	//!< バルカン砲の支柱
#define GMD_BOSS5_BODY_NODE_IDX_GROIN_L		(7)		//!< 左脚の付け根
#define GMD_BOSS5_BODY_NODE_IDX_GROIN_R		(15)	//!< 右足の付け根
#define GMD_BOSS5_BODY_NODE_IDX_NOZZLE_L	(30)	//!< 噴射口 左
#define GMD_BOSS5_BODY_NODE_IDX_NOZZLE_R	(31)	//!< 噴射口 右
#define GMD_BOSS5_BODY_NODE_IDX_SHOULDER_L	(26)	//!< 左肩
#define GMD_BOSS5_BODY_NODE_IDX_ELBOW_L		(27)	//!< 左肘
#define GMD_BOSS5_BODY_NODE_IDX_FOREARM_L	(28)	//!< 左前腕
#define GMD_BOSS5_BODY_NODE_IDX_SHOULDER_R	(3)		//!< 右肩
#define GMD_BOSS5_BODY_NODE_IDX_ELBOW_R		(4)		//!< 右ひじ
#define GMD_BOSS5_BODY_NODE_IDX_FOREARM_R	(5)		//!< 右前腕
#define GMD_BOSS5_BODY_NODE_IDX_HEAD		(23)	//!< 頭部（後頭部の辺り）
#define GMD_BOSS5_BODY_NODE_IDX_NECK		(24)	//!< 首
#define GMD_BOSS5_BODY_NODE_IDX_COVER		(25)	//!< バルカン砲のカバー

#define GMD_BOSS5_BODY_NODE_SNM_NUM			(16)		//!< SNM登録ノード数
#define GMD_BOSS5_BODY_NODE_CNM_NUM			(10 + GME_BOSS5_SCT_PART_IDX_MAX)	//!< CNM登録ノード数

//############ 共通 ###########################################################
/* フラグ */
// エネミーの当たり矩形の有効・無効フラグ
#define GMD_BOSS5_RECT_ENABLE_FLAG_DEF		(1 << GMD_ENEMY_RECT_DEF)
#define GMD_BOSS5_RECT_ENABLE_FLAG_ATK		(1 << GMD_ENEMY_RECT_ATK)
#define GMD_BOSS5_RECT_ENABLE_FLAG_BODY		(1 << GMD_ENEMY_RECT_BODY)

/* 定義値 */
// 開始デモでのプレイヤーの移動目標座標Xの本体座標からのオフセット
#define GMD_BOSS5_PLY_OP_DEMO_RUN_DEST_X_OFST_FROM_BODY		((fx32)(FX32_ONE * -128))

// 爆発エフェクト関連
#define GMD_BOSS5_EXPL_OFST_Z					((fx32)(FX32_ONE * 64))		//!< 爆発を親より手前に表示させるためのオフセットZ
	// 本体爆発
#define GMD_BOSS5_EXPL_BODY_OFST_X				((fx32)(FX32_ONE * 0))		//!< 本体用の爆発範囲中心X
#define GMD_BOSS5_EXPL_BODY_OFST_Y				((fx32)(FX32_ONE * 0))		//!< 本体用の爆発範囲中心Y
#define GMD_BOSS5_EXPL_BODY_WIDTH				((fx32)(FX32_ONE * 160))	//!< 本体用の爆発範囲幅
#define GMD_BOSS5_EXPL_BODY_HEIGHT				((fx32)(FX32_ONE * 160))	//!< 本体用の爆発範囲高さ
#define GMD_BOSS5_EXPL_BODY_INTERVAL_MIN		(10)						//!< 本体用の爆発生成 最短間隔
#define GMD_BOSS5_EXPL_BODY_INTERVAL_MAX		(10)						//!< 本体用の爆発生成 最長間隔
#define GMD_BOSS5_EXPL_BODY_SE_FREQUENCY		(0.5f)						//!< 本体用の爆発SE再生周期
	// プレイヤー追いかけ爆発
#define GMD_BOSS5_EXPL_CHASE_SMALL_WIDTH		((fx32)((OBD_OBJ_CLIP_LCD_X << FX32_SHIFT) / 2))
#define GMD_BOSS5_EXPL_CHASE_SMALL_HEIGHT		((fx32)(FX32_ONE * 96))
#define GMD_BOSS5_EXPL_CHASE_SMALL_OFST_X		((fx32)-((OBD_OBJ_CLIP_LCD_X << FX32_SHIFT) + GMD_BOSS5_EXPL_CHASE_SMALL_WIDTH / 2))
#define GMD_BOSS5_EXPL_CHASE_SMALL_OFST_Y		((fx32)(FX32_ONE * 0))
#define GMD_BOSS5_EXPL_CHASE_SMALL_INTERVAL_MIN	(5)
#define GMD_BOSS5_EXPL_CHASE_SMALL_INTERVAL_MAX	(5)
#define GMD_BOSS5_EXPL_CHASE_SMALL_CHASE_SPD_X	((fx32)(FX32_ONE * 8))		//!< プレイヤーとのオフセットの移動速度X(i.e.プレイヤーとの相対速度)
#define GMD_BOSS5_EXPL_CHASE_SMALL_CHASE_RIGHTEDGE_OFST_X_MAX	((fx32)(FX32_ONE * -8))	//! 爆発生成範囲矩形の右端が、プレイヤーからこの値オフセットした位置まで近づく
#define GMD_BOSS5_EXPL_CHASE_SMALL_CHASE_OFST_X_MAX	((fx32)(GMD_BOSS5_EXPL_CHASE_SMALL_CHASE_RIGHTEDGE_OFST_X_MAX \
															- GMD_BOSS5_EXPL_CHASE_SMALL_WIDTH / 2))	//!< 爆発生成範囲矩形のプレイヤーからのオフセット最大値
#if _IPHONE
#define GMD_BOSS5_EXPL_CHASE_SMALL_SE_FREQUENCY		(0.125f)		//!< SE再生周期(コンソールの1/2の速度)
#else
#define GMD_BOSS5_EXPL_CHASE_SMALL_SE_FREQUENCY		(0.25f)			//!< SE再生周期
#endif // _IPHONE

#define GMD_BOSS5_EXPL_CHASE_BIG_WIDTH			((fx32)((OBD_OBJ_CLIP_LCD_X << FX32_SHIFT) / 2))
#define GMD_BOSS5_EXPL_CHASE_BIG_HEIGHT			((fx32)(FX32_ONE * 128))
#define GMD_BOSS5_EXPL_CHASE_BIG_OFST_X			((fx32)-((OBD_OBJ_CLIP_LCD_X << FX32_SHIFT) + GMD_BOSS5_EXPL_CHASE_SMALL_WIDTH / 2))
#define GMD_BOSS5_EXPL_CHASE_BIG_OFST_Y			((fx32)(FX32_ONE * 0))
#define GMD_BOSS5_EXPL_CHASE_BIG_INTERVAL_MIN	(15)
#define GMD_BOSS5_EXPL_CHASE_BIG_INTERVAL_MAX	(15)
#define GMD_BOSS5_EXPL_CHASE_BIG_CHASE_SPD_X	((fx32)(FX32_ONE * 8))		//!< プレイヤーとのオフセットの移動速度X(i.e.プレイヤーとの相対速度)
#define GMD_BOSS5_EXPL_CHASE_BIG_CHASE_RIGHTEDGE_OFST_X_MAX	((fx32)(FX32_ONE * -8))	//! 爆発生成範囲矩形の右端が、プレイヤーからこの値オフセットした位置まで近づく
#define GMD_BOSS5_EXPL_CHASE_BIG_CHASE_OFST_X_MAX	((fx32)(GMD_BOSS5_EXPL_CHASE_BIG_CHASE_RIGHTEDGE_OFST_X_MAX \
															- GMD_BOSS5_EXPL_CHASE_BIG_WIDTH / 2))	//!< 爆発生成範囲矩形のプレイヤーからのオフセット最大値
#define GMD_BOSS5_EXPL_CHASE_BIG_DELAY_OFST_X	((fx32)(FX32_ONE * -32))	//!< 小爆発の生成範囲よりも後ろにずらすためのオフセット
#define GMD_BOSS5_EXPL_CHASE_BIG_SE_FREQUENCY	(0.25f)			//!< SE再生周期

// カメラ上昇関連
#define GMD_BOSS5_CAMERA_LIFT_OFFSET_POS_Y		(92)	//!< カメラ上昇設定時のカメラオフセットY
// 4:3アスペクト比用カメラ横スライド関連
#define GMD_BOSS5_CAMERA_SLIDE_FOR_NARROW_OFFSET_POS_X	(56)	//!< カメラ横スライド設定時のカメラオフセットX

// スクロール制限解除関連
#define GMD_BOSS5_CAM_SCR_LIMIT_RELEASE_GNTL_SPD_X_INIT	((fx32)(FX32_ONE * 1.f))	//!< 右スクロール制限移動初速度
#define GMD_BOSS5_CAM_SCR_LIMIT_RELEASE_GNTL_ACC_X		((fx32)(FX32_ONE * 0.05f))	//!< 右スクロール制限移動加速度

//############ 管理 ###########################################################
/* 定義値 */
#define GMD_BOSS5_MGR_WAIT_EXPLODE_TIME					(120)	//!< 爆発中待ち時間
#define GMD_BOSS5_MGR_CLOSING_DEMO_WAIT_BEGIN_TIME_MAX	(120)	//!< 終了デモ開始待ち最大待ち時間
#define GMD_BOSS5_MGR_CLOSING_DEMO_DURATION_TIME		(240)	//!< 終了デモ継続時間
#define GMD_BOSS5_MGR_CLOSING_DEMO_WHITEOUT_TIME		(60)	//!< フェードアウト完了後の白画面待機時間

//############ 本体 ###########################################################
/* 定義値 */
// 共通
#define GMD_BOSS5_BODY_DMG_FLICKER_RADIUS	((Float)32.f)		//!< ダメージ点滅処理用モデル半径値

#define GMD_BOSS5_BODY_DMG_NO_HIT_TIME		(10)				//!< ダメージ時ヒット無効時間

#define GMD_BOSS5_BODY_HIDE_RADIUS			((fx32)(FX32_ONE * 8))	//!< この距離分、画面端から離れたら完全に画面外に消えたと判定する（モデル下端に中心があるので小さい値を採用）

#define GMD_BOSS5_ARM_POKE_ANIM_PHASE_NUM	(9)					//!< 腕突き出しアニメーションフェーズ数

#define GMD_BOSS5_BODY_JETSMOKE_CLEAR_HEIGHT	((fx32)(FX32_ONE * 128))	//!< 本体が、地面からこの高さ離れた位置よりも上に行った場合、噴射スモークエフェクトを消去する

// 地形当たり矩形 通常時
#define GMD_BOSS5_BODY_DEFAULT_FIELD_RECT_SIZE_LEFT		(-32)
#define GMD_BOSS5_BODY_DEFAULT_FIELD_RECT_SIZE_TOP		(-128)
#define GMD_BOSS5_BODY_DEFAULT_FIELD_RECT_SIZE_RIGHT	(32)
#define GMD_BOSS5_BODY_DEFAULT_FIELD_RECT_SIZE_BOTTOM	(-2)
// 地形当たり矩形 撃破・パーツ飛散時
#define GMD_BOSS5_BODY_SCT_FIEELD_RECT_SIZE_LEFT		(-32)
#define GMD_BOSS5_BODY_SCT_FIEELD_RECT_SIZE_TOP			(-128)
#define GMD_BOSS5_BODY_SCT_FIEELD_RECT_SIZE_RIGHT		(32)
#define GMD_BOSS5_BODY_SCT_FIEELD_RECT_SIZE_BOTTOM		(-32)

// 跳ね返りパラメータ
#define GMD_BOSS5_BODY_PLY_NML_REBOUND_X		((fx32)(FX32_ONE * 3))	//!< 通常跳ね返り時X速度
#define GMD_BOSS5_BODY_PLY_NML_REBOUND_Y		((fx32)(FX32_ONE * -4))	//!< 通常跳ね返り時Y速度
#define GMD_BOSS5_BODY_PLY_NML_REBOUND_NOJUMPMOVE_TIME	((fx32)(FX32_ONE * 16))	//!< 通常跳ね返り時ジャンプ中移動禁止時間
#define GMD_BOSS5_BODY_PLY_HOMING_REBOUND_X		((fx32)(FX32_ONE * 5))	//!< ホーミング跳ね返り時X速度
#define GMD_BOSS5_BODY_PLY_HOMING_REBOUND_Y		((fx32)(FX32_ONE * -4))	//!< ホーミング跳ね返り時Y速度
#define GMD_BOSS5_BODY_PLY_HOMING_REBOUND_NOJUMPMOVE_TIME	((fx32)(FX32_ONE * 25))	//!< ホーミング跳ね返り時ジャンプ中移動禁止時間

// 開始関連
#define GMD_BOSS5_BODY_START_WAIT_RISE_TIME		(30)			//!< せり上がり開始待ち時間
#define GMD_BOSS5_BODY_START_BURY_HEIGHT		((fx32)(FX32_ONE * 104))	//!< 埋める高さ（接地している時の座標からこの値を加えた位置に座標をずらして地面に埋める）
#define GMD_BOSS5_BODY_START_RISE_SPD_Y			((fx32)(FX32_ONE * -2))		//!< せり上がり速度
#define GMD_BOSS5_BODY_START_WAIT_END_TIME		(30)			//!< 終了待ち時間
#define GMD_BOSS5_BODY_START_RISE_VIB_AMP_MAX	((fx32)(FX32_ONE * 2))	//!< せり上がり時振動最大振幅
#define GMD_BOSS5_BODY_START_RISE_VIB_INTERVAL	(3)						//!< せり上がり時振動発生間隔

// キャノピー関連
#define GMD_BOSS5_BODY_CANOPY_CLOSE_RATIO_SPD_ACC	(0.001f)	//!< キャノピークローズ進捗加速度
#define GMD_BOSS5_BODY_CANOPY_CLOSE_RATIO_SPD_MAX	(0.5f)		//!< キャノピークローズ進捗最高速度
#define GMD_BOSS5_BODY_CANOPY_CLOSE_START_ANGLE_X	(AKM_DEGtoA32(-80.f))	//!< キャノピークローズ キャノピー初期角度

// 腕突き出し関連
#define GMD_BOSS5_BODY_POKE_TRIGGER_LIMIT_TIME	(120)			//!< ボスが一度攻撃を食らった後、この時間以内にもう一度食らったら腕突き出し攻撃を発動

// 通常移動関連
#define GMD_BOSS5_BODY_WALK_MOVE_PHASE_NUM		(4)				//!< 歩行移動フェーズ数（接地ノードを設定する回数に等しい）
#define GMD_BOSS5_BODY_WALK_GROUND_TIMING_PAHSE_NUM	(5)			//!< 歩行足接地タイミングフェーズ数（足が地面に接地する回数）
#define GMD_BOSS5_BODY_WALK_WALK_END_WALL_DISTANCE	((fx32)(FX32_ONE * 96))	//!< 足接地タイミングの時に壁からこの距離以内に入っていたら歩行終了

// 高速移動関連
#define GMD_BOSS5_BODY_RUN_DURATION_TIME			(360)					//!< 高速移動継続時間
#define GMD_BOSS5_BODY_RUN_FWD_JUMP_INIT_SPD_X		((fx32)(FX32_ONE * 10))	//!< 前方移動 ジャンプ動作時の水平方向速度（右向き基準）
#define GMD_BOSS5_BODY_RUN_FWD_FLY_INIT_SPD_X		((fx32)(FX32_ONE * 5))	//!< 前方移動 空中移動時の水平方向速度（右向き基準）
#define GMD_BOSS5_BODY_RUN_FWD_FLY_INIT_SPD_Y		((fx32)(FX32_ONE * -2))	//!< 前方移動 空中移動時の垂直方向速度
#define GMD_BOSS5_BODY_RUN_BWD_SPD_X_FACTOR			((fx32)(FX32_ONE * (3.f/4.f)))	//!< 前方移動の速度にこの係数を掛けて後方移動時の速度とする
#define GMD_BOSS5_BODY_RUN_GROUNDING_EFCT_DELAY		(6)						//!< 着地アクションに入ってからこのフレーム経過したら足接地時エフェクト発生処理を行う
#define GMD_BOSS5_BODY_RUN_ACT_FRAME_OVERRUN_ALLOW_RATIO	(0.4f)			//!< ジャンプから滞空に移行する際のモーション終端フレーム超過許容率
#define GMD_BOSS5_BODY_RUN_PRE_RECOVER_TIMEOUT_FRAME	(10)				//!< 高速移動を中断して復帰動作に移行する前処理のタイムアウト時間
#define GMD_BOSS5_BODY_RUN_RECOVER_TIMEOUT_FRAME		(20)				//!< 高速移動を中断した後の復帰前処理後、復帰動作処理のタイムアウト時間

// ストンプ関連
#define GMD_BOSS5_BODY_STOMP_IGNITE_TIME		(30)					//!< エンジン点火動作時間
#define GMD_BOSS5_BODY_SFLYUP_INIT_SPD			((fx32)(FX32_ONE * -1))
#define GMD_BOSS5_BODY_SFLYUP_ACC				((fx32)(FX32_ONE * -0.3f))	//!< 画面外への上昇加速度
#define GMD_BOSS5_BODY_STOMP_WAIT_TIME			(240)		//!< 画面外待機時間
#define GMD_BOSS5_BODY_STOMP_FALL_INIT_SPD		((fx32)(FX32_ONE * 3))		//!< ストンプ時降下初速度
#define GMD_BOSS5_BODY_STOMP_FALL_ACC			((fx32)(FX32_ONE * 0.1f))	//!< ストンプ時降下加速度
#define GMD_BOSS5_BODY_STOMP_SEARCH_DELAY		(10)		//!< プレイヤーサーチ遅延フレーム数
#define GMD_BOSS5_BODY_STOMP_NO_SEARCH_TIME		(10)		//!< 落下タイミングからこのフレーム分前のタイミングまでサーチを行う
#define GMD_BOSS5_BODY_STOMP_FALL_POS_MARGIN	((fx32)(FX32_ONE * 32))		//!< ストンプの落下地点を、最短でもこの値の距離だけ壁から離す
#define GMD_BOSS5_BODY_STOMP_WALL_BEHIND_WALL_DISTANCE	((fx32)(FX32_ONE * 96))	//!< 壁からこの距離以内に入っていたら壁に背を向けて落下する

// 凶暴化演出関連
#define GMD_BOSS5_BODY_BERSERK_ROAR_PREP_TIME	(60)						//!< 凶暴化演出咆哮準備継続時間
#define GMD_BOSS5_BODY_BERSERK_ROAR_LOOP_TIME	(120)						//!< 凶暴化演出咆哮継続時間
#define GMD_BOSS5_BODY_BERSERK_TURN_FRONT_DEG_F	((Float)75.f)				//!< 凶暴化演出方向転換正面方向角度（右向き基準）
#define GMD_BOSS5_BODY_BERSERK_TURN_RETURN_DEG_F	((Float)90.f)			//!< 凶暴化演出方向転換元方向角度（右向き基準）
#define GMD_BOSS5_BODY_BERSERK_TURN_RATIO_SPD	(0.1f)						//!< 凶暴化演出方向転換進捗速度
#define GMD_BOSS5_BODY_BERSERK_BREAKDOWN_TIME	(90)						//!< 凶暴化演出開始時機能停止時間
#define GMD_BOSS5_BODY_BERSERK_SHAKE_MOTION_SPD_DEST	(2.f)				//!< 凶暴化演出時振動動作モーション加速目標速度
#define GMD_BOSS5_BODY_BERSERK_SHAKE_MOTION_SPD_RATIO_ACC	(0.00005f)		//!< 凶暴化演出時振動動作モーション加速進捗加速度
#define GMD_BOSS5_BODY_BERSERK_SHAKE_STAY_TIME	(60)						//!< 凶暴化演出時振動動作モーション速度最高速度で停滞する時間
#define GMD_BOSS5_BODY_BERSERK_STAMP_SMOKE_CREATE_DELAY	(45)					//!< 凶暴化演出時足踏み込みエフェクト発生遅延時間
#define GMD_BOSS5_BODY_BERSERK_STAMP_VIB_START_DELAY	(GMD_BOSS5_BODY_BERSERK_STAMP_SMOKE_CREATE_DELAY)	//!< 凶暴化演出時足踏み込み振動開始遅延時間
#define GMD_BOSS5_BODY_BERSERK_STAMP_SE_START_DELAY		(GMD_BOSS5_BODY_BERSERK_STAMP_SMOKE_CREATE_DELAY)	//!< 凶暴化演出時足踏み込みSE再生開始遅延時間

// 地球割り関連
#define GMD_BOSS5_BODY_CFLYUP_INIT_SPD			((fx32)(FX32_ONE * 0))		//!< 地球割り・上昇時初速度
#define GMD_BOSS5_BODY_CFLYUP_ACC				((fx32)(FX32_ONE * -0.1f))	//!< 地球割り・上昇時加速度
#define GMD_BOSS5_BODY_CRASH_WAIT_TIME			(300)						//!< 地球割り・画面外待機時間
#define GMD_BOSS5_BODY_CRASH_CURSOR_SPAWN_TIME_TRHESHOLD	(60)			//!< 地球割り・画面外待機時間の残り時間がこの値を切ったらカーソルを表示
#define GMD_BOSS5_BODY_CRASH_FALL_INIT_SPD		((fx32)(FX32_ONE * 3))		//!< 地球割り・降下時初速度
#define GMD_BOSS5_BODY_CRASH_FALL_ACC			((fx32)(FX32_ONE * 0.1f))	//!< 地球割り・降下時加速度
#define GMD_BOSS5_BODY_CRASH_LANDED_IDLE_TIME	(120)						//!< 地球割り・着地後停滞時間
#define GMD_BOSS5_BODY_CRASH_PLY_IMMOBILE_TIME	(90)						//!< 着地振動でプレイヤーを足止めする時間
#define GMD_BOSS5_BODY_CRASH_STRIKE_SW_CREATE_DELAY	(26)					//!< 着地＆突きモーション開始後、このフレーム数経過したら地面突き衝撃波発生
#define GMD_BOSS5_BODY_CRASH_STRIKE_VIB_START_DELAY	(GMD_BOSS5_BODY_CRASH_STRIKE_SW_CREATE_DELAY)	//! 着地＆突きモーション開始後、このフレーム数経過したら地面突き振動発生
#define GMD_BOSS5_BODY_CRASH_STRIKE_BODY_VIB_START_DELAY	(GMD_BOSS5_BODY_CRASH_STRIKE_SW_CREATE_DELAY)	//! 着地＆突きモーション開始後、このフレーム数経過したら本体振動発生
#define GMD_BOSS5_BODY_CRASH_STRIKE_BODY_VIB_RATIO_ADD		((Float)0.0125f)		//!< 地球割り地面突き本体振動の進捗値加算速度
#define GMD_BOSS5_BODY_CRASH_STRIKE_BODY_VIB_SCALE			((fx32)(FX32_ONE * 8))	//!< 地球割り地面突き本体振動の振幅スケール
#define GMD_BOSS5_BODY_CRASH_STRIKE_SE_START_DELAY	(GMD_BOSS5_BODY_CRASH_STRIKE_SW_CREATE_DELAY - 12)	//!< 着地＆突きモーション開始後、このフレーム数経過したら地面突きSE再生開始

// 撃破関連
#define GMD_BOSS5_BODY_DEFEAT_WAIT_START_TIME	(40)						//!< 撃破開始待ち時間

#define GMD_BOSS5_BODY_DEFEAT_SCT_FALL_LAND_VIB_TIME	(8)						//!< パーツ飛散後 本体落下 着地時振動時間
#define GMD_BOSS5_BODY_DEFEAT_SCT_FALL_LAND_VIB_AMP		((fx32)(FX32_ONE * 2))	//!< パーツ飛散後 本体落下 着地時振動最大振幅
#define GMD_BOSS5_BODY_DEFEAT_SCT_FALL_LAND_VIB_DEG_SPD	(AKM_DEGtoA32(60))		//!< パーツ飛散後 本体落下 着地時振動サイン波位相角速度

// ロケットパンチ関連
#define GMD_BOSS5_BODY_RPUNCH_LAUNCH_TIMING_DELAY	(10.f)		//!< 発射をモーションに合わせるために、モーションフレームがこの値だけ経過したら発射

// のけぞり（強ロケットパンチ時ダメージ）関連
#define GMD_BOSS5_BODY_RPC_STR_DMG_SWING_TIME	(240)		//!< よろけ時間

//############ ボスFINAL警告フェード ##########################################
/* 定義値 */
#define GMD_BOSS5_ALARM_FADE_DEST_ALPHA		(0x3F)		//!< フェードアウト後・フェードイン開始時のα値
#define GMD_BOSS5_ALARM_FADE_DEST_RED		(0xFF)		//!< フェードアウト後・フェードイン開始時のR値
#define GMD_BOSS5_ALARM_FADE_DEST_GREEN		(0x00)		//!< フェードアウト後・フェードイン開始時のG値
#define GMD_BOSS5_ALARM_FADE_DEST_BLUE		(0x00)		//!< フェードアウト後・フェードイン開始時のB値

//############ ボスFINAL画面フラッシュ ########################################
/* 定義値 */
#define GMD_BOSS5_FLASH_SCREEN_FADEOUT_TIME			(4)		//!< 撃破のフラッシュ完全に白になるまでのフレーム
#define GMD_BOSS5_FLASH_SCREEN_DURATION_TIME		(5)		//!< 撃破のフラッシュ完全に白の間のフレーム
#define GMD_BOSS5_FLASH_SCREEN_FADEIN_TIME			(30)	//!< 撃破のフラッシュ完全に白から戻るまでのフレーム

//############ ボスFINAL終了フェードアウト ####################################
/* 定義値 */
#define GMD_BOSS5_LAST_FADE_OUT_FRAME		(300)		//!< 最後のフェードアウトのフェードフレーム数

//############ パーツ飛散 #####################################################
/* フラグ */
/* 定義値 */
#define GMD_BOSS5_SCT_NDC_NUM					(6)			//!< NDC対象ノード数
#define GMD_BOSS5_SCT_NDC_FLY_SPD_FLOAT			((Float)3.f)	//!< パーツ飛散速度

//############ 動作シーケンス #################################################
#define GMD_BOSS5_BODY_STRAT_BRANCH_NUM			(3)				//!< 戦略フローでの最大分岐数
// 通常ロケットパンチ
#define GMD_BOSS5_BODY_SEQ_RPUNCH_NML_FASTSHOT_PROB			((fx32)(FX32_ONE * 0.2f))	//!< 通常ロケットパンチ時のサーチ時間が短くなる確率
#define GMD_BOSS5_BODY_SEQ_RPUNCH_NML_SEARCH_TIME_SHORT		(10)						//!< 通常ロケットパンチの短縮サーチ時間
#define GMD_BOSS5_BODY_SEQ_RPUNCH_NML_SEARCH_TIME_NORMAL	(60)						//!< 通常ロケットパンチの通常サーチ時間
// 強化ロケットパンチ
#define GMD_BOSS5_BODY_SEQ_RPUNCH_STR_FASTSHOT_PROB			(GMD_BOSS5_BODY_SEQ_RPUNCH_NML_FASTSHOT_PROB)		//!< 強化ロケットパンチ時のサーチ時間が短くなる確率
#define GMD_BOSS5_BODY_SEQ_RPUNCH_STR_SEARCH_TIME_SHORT		(GMD_BOSS5_BODY_SEQ_RPUNCH_NML_SEARCH_TIME_SHORT)	//!< 強化ロケットパンチの短縮サーチ時間
#define GMD_BOSS5_BODY_SEQ_RPUNCH_STR_SEARCH_TIME_NORMAL	(GMD_BOSS5_BODY_SEQ_RPUNCH_NML_SEARCH_TIME_NORMAL)	//!< 強化ロケットパンチの通常サーチ時間

//############ SE関連 #########################################################
// ターゲットSE
#define GMD_BOSS5_BODY_SE_TARGET_INIT_INTERVAL				(GMD_BOSS5_BODY_STOMP_WAIT_TIME >> 2)	//!< 初期再生間隔
#define GMD_BOSS5_BODY_SE_TARGET_INTERVAL_DEC_SPD			(0.25f)		//!< カウントダウンタイマにこの値をかけた結果を現在のインターバルにする
#define GMD_BOSS5_BODY_SE_TARGET_INTERVAL_MIN				(10)		//!< 最短インターバル

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! ボス５確率分岐ポイント列挙型
typedef enum
{
	GME_BOSS5_BODY_STRAT_BRANCH_NML_A	= 0,	// 通常 確率A
	GME_BOSS5_BODY_STRAT_BRANCH_NML_B,			// 通常 確率B
	GME_BOSS5_BODY_STRAT_BRANCH_STR_A,			// 凶暴 確率A
	GME_BOSS5_BODY_STRAT_BRANCH_STR_B,			// 凶暴 確率B
	//GME_BOSS5_BODY_STRAT_BRANCH_STR_C,		// 凶暴 確率C（テーブルを使用しないため無効化）
	
	GME_BOSS5_BODY_STRAT_BRANCH_MAX
} GME_BOSS5_BODY_STRAT_BRANCH;

//! ボス５ 方向タイプ列挙型
typedef enum
{
	GME_BOSS5_BODY_DIRECTION_TYPE_LEFT	= 0,	// 左向き
	GME_BOSS5_BODY_DIRECTION_TYPE_RIGHT,		// 右向き
	GME_BOSS5_BODY_DIRECTION_TYPE_NEAR,			// カメラ側向き
	
	GME_BOSS5_BODY_DIRECTION_TYPE_MAX
} GME_BOSS5_BODY_DIRECTION_TYPE;

//! ボス５ 矩形設定タイプ列挙型
typedef enum
{
	GME_BOSS5_BODY_RECT_SETTING_DEFAULT_NML	= 0,	//!< デフォルト（通常）
	GME_BOSS5_BODY_RECT_SETTING_DEFAULT_STR,		//!< デフォルト（凶暴）
	GME_BOSS5_BODY_RECT_SETTING_START,				//!< 開始演出
	GME_BOSS5_BODY_RECT_SETTING_STOMP_NML_FALL,		//!< 通常時 ストンプ降下
	GME_BOSS5_BODY_RECT_SETTING_STOMP_STR_FALL,		//!< 凶暴時 ストンプ降下
	GME_BOSS5_BODY_RECT_SETTING_CRASH_FALL,			//!< 地球割り降下
	GME_BOSS5_BODY_RECT_SETTING_CRASH_GROUND,		//!< 地球割り接地中（着地・停滞）
	GME_BOSS5_BODY_RECT_SETTING_CRASH_SINK,			//!< 地球割り沈下中
	GME_BOSS5_BODY_RECT_SETTING_RPC_STR_DMG,		//!< 強ロケットパンチ時のけぞり
	GME_BOSS5_BODY_RECT_SETTING_BERSERK_START,		//!< 凶暴化演出 開始（漏電開始前）
	GME_BOSS5_BODY_RECT_SETTING_DEFEAT,				//!< 撃破演出
	
	GME_BOSS5_BODY_RECT_SETTING_MAX
} GME_BOSS5_BODY_RECT_SETTING;


//! ボス５ 凶暴演出用方向転換タイプ
typedef enum
{
	GME_BOSS5_BODY_BSK_TURN_TYPE_FRONT	= 0,
	GME_BOSS5_BODY_BSK_TURN_TYPE_RETURN,
	
	GME_BOSS5_BODY_BSK_TURN_TYPE_MAX
} GME_BOSS5_BODY_BSK_TURN_TYPE;


//! ボス５ 警告フェード フェーズ列挙型
typedef enum
{
	GME_BOSS5_ALARM_FADE_PHASE_NONE	= 0,
	GME_BOSS5_ALARM_FADE_PHASE_FADE_OUT,
	GME_BOSS5_ALARM_FADE_PHASE_STAY_ON,
	GME_BOSS5_ALARM_FADE_PHASE_FADE_IN,
	GME_BOSS5_ALARM_FADE_PHASE_STAY_OFF,
	
	GME_BOSS5_ALARM_FADE_PHASE_MAX
} GME_BOSS5_ALARM_FADE_PHASE;

//! 振動インデックス列挙型
typedef enum
{
	GME_BOSS5_VIB_IDX_WALK_STEP	= 0,
	GME_BOSS5_VIB_IDX_RUN_STEP,
	GME_BOSS5_VIB_IDX_BERSERK_STAMP,
	GME_BOSS5_VIB_IDX_STOMP_LANDING,
	GME_BOSS5_VIB_IDX_CRASH_LANDING,
	GME_BOSS5_VIB_IDX_CRASH_STRIKE,
	
	GME_BOSS5_VIB_IDX_MAX
} GME_BOSS5_VIB_IDX;

//! ボス５ 警告フェードワーク
typedef struct tag_GMS_BOSS5_ALARM_FADE_WORK
{
	GMS_FADE_OBJ_WORK	fade_obj;
	
	GMS_BOSS5_MGR_WORK	*mgr_work;
	
	void (*proc_update)(struct tag_GMS_BOSS5_ALARM_FADE_WORK*);
	
	GME_BOSS5_ALARM_FADE_PHASE	cur_phase;	//!< 現在のフェード処理フェーズ
	
	GME_BOSS5_ALARM_LEVEL	cur_level;
	
	Uint32					wait_timer;
	
	GMS_BOSS5_1SHOT_TIMER	alert_se_timer;	//!< アラートSE用1ショットタイマ
	GME_BOSS5_ALARM_LEVEL	alert_se_ref_level;	//!< アラートSE用の現在の警告レベル
} GMS_BOSS5_ALARM_FADE_WORK;

//! ボス５ 撃破時画面フラッシュワーク
typedef struct tag_GMS_BOSS5_FLASH_SCREEN_WORK
{
	GMS_EFFECT_COM_WORK	efct_com;
	GMS_CMN_FLASH_SCR_WORK	flash_work;	//!< 画面白フラッシュワーク
} GMS_BOSS5_FLASH_SCREEN_WORK;

//! パーツ飛散 パーツ操作NDCワーク
typedef struct tag_GMS_BOSS5_SCT_PART_NDC_WORK
{
	GMS_BS_CMN_NODE_CTRL_OBJECT	ndc_obj;
		
	AMS_QUAT	spin_quat;	//!< 回転差分クォータニオン
} GMS_BOSS5_SCT_PART_NDC_WORK;

//! ボス5警告フェード情報構造体
typedef struct tag_GMS_BOSS5_ALARM_FADE_INFO
{
	Uint32	fo_frame;	//!< フェードアウトにかけるフレーム数
	Uint32	on_frame;	//!< フェードアウト後の状態で停滞するフレーム数
	Uint32	fi_frame;	//!< フェードインにかけるフレーム数
	Uint32	off_frame;	//!< フェードイン後の状態で停滞するフレーム数
} GMS_BOSS5_ALARM_FADE_INFO;

//! ボス5パーツ飛散情報構造体
typedef struct tag_GMS_BOSS5_SCT_PART_INFO
{
	GME_BS_CMN_CNM_MODE	cnm_mode;
	BOOL	is_local_coord;
	BOOL	is_inherit_scale;
} GMS_BOSS5_SCT_PART_INFO;

//! ボス5パーツ飛散 NDC情報構造体
typedef struct tag_GMS_BOSS5_SCT_NDC_INFO
{
	GME_BOSS5_SCT_PART_IDX	part_idx;
	Uint32	delay_time;
} GMS_BOSS5_SCT_NDC_INFO;

//! ボス５パーツアクション情報構造体
typedef struct tag_GMS_BOSS5_PART_ACT_INFO
{
	Uint16	act_id;	//!< モーション番号（AMBインデックス）
	Uint8	is_maintain;	//!< 前のアクション継続
	Uint8	is_repeat;		//!< リピート
	Float	mtn_spd;		//!< モーション再生速度
	BOOL	is_blend;		//!< ブレンド有無
	Float	blend_spd;		//!< ブレンド速度
	BOOL	is_merge_manual;	//!< マニュアルマージ有無
} GMS_BOSS5_PART_ACT_INFO;

//! ボス５本体 移動処理情報
typedef struct tag_GMS_BOSS5_MOVE_INFO
{
	Float	switching_frame;	//!< 切り替えタイミングのフレーム
	GME_BOSS5_BODY_MOVE_PHASE_TYPE	move_phase_type;	//!< 移動フェーズタイプ
} GMS_BOSS5_MOVE_INFO;

//! ボス５本体 移動処理 足接地タイミング情報
typedef struct tag_GMD_BOSS5_WALK_GROUND_TIMING_INFO
{
	Float	grounding_frame;
	GME_BOSS5_LEG_TYPE	leg_type;
} GMD_BOSS5_WALK_GROUND_TIMING_INFO;

//! ボス５本体 腕構成パーツアニメーション情報
typedef struct tag_GMS_BOSS5_ARM_PART_ANIM_INFO
{
	BOOL	is_anim;
	NNS_ROTATE_A32	start_rot;
	NNS_ROTATE_A32	end_rot;
} GMS_BOSS5_ARM_PART_ANIM_INFO;

//! ボス５本体 腕アニメーション情報
typedef struct tag_GMS_BOSS5_ARM_ANIM_INFO
{
	Uint32	wait_time;
	Float	slerp_inc_rate;
	GMS_BOSS5_ARM_PART_ANIM_INFO	part_anim_info[GME_BOSS5_ARMPART_IDX_MAX];
} GMS_BOSS5_ARM_ANIM_INFO;

//! ボス５ 当たり矩形 配置箇所別設定情報
typedef struct tag_GMS_BOSS5_RECTPOINT_SETTING_INFO
{
	Uint32	enable_bit_flag;	// 有効にする矩形のビットフラグ（GMD_BOSS5_RECT_ENABLE_FLAG_XXX）
	Sint16	rect_size[GMD_ENEMY_RECT_NUM][MTD_RECT];	// 矩形サイズ
} GMS_BOSS5_RECTPOINT_SETTING_INFO;

//! ボス５本体当たり矩形設定情報
typedef struct tag_GMS_BOSS5_BODY_RECT_SETTING_INFO
{
	BOOL	is_invincible;	//!< 無敵設定フラグ
	BOOL	is_leakage;		//!< 漏電エフェクト生成フラグ
	GMS_BOSS5_RECTPOINT_SETTING_INFO	point_setting_info[GME_BOSS5_BODY_RECTPOINT_MAX];
} GMS_BOSS5_BODY_RECT_SETTING_INFO;

//! 本体ステート開始関数
typedef void (*GMF_BOSS5_BODY_STATE_ENTER_FUNC)(GMS_BOSS5_BODY_WORK*);
//! 本体ステート終了関数
typedef void (*GMF_BOSS5_BODY_STATE_LEAVE_FUNC)(GMS_BOSS5_BODY_WORK*);
//! 本体サブシーケンス開始関数
typedef void (*GMF_BOSS5_BODY_SUBSEQ_ENTER_FUNC)(GMS_BOSS5_BODY_WORK*);

//! 本体ステート開始情報構造体
typedef struct tag_GMS_BOSS5_BODY_STATE_ENTER_INFO
{
	GMF_BOSS5_BODY_STATE_ENTER_FUNC	enter_func;	//!< 本体ステート開始関数
	BOOL	is_wrapped;		//!< 間接呼び出しフラグ
} GMS_BOSS5_BODY_STATE_ENTER_INFO;

//! 本体サブシーケンス開始情報構造体
typedef struct tag_GMS_BOSS5_BODY_SUBSEQ_ENTER_INFO
{
	GMF_BOSS5_BODY_SUBSEQ_ENTER_FUNC	enter_func;	//!< 本体サブシーケンス開始関数
	GME_BOSS5_BODY_STATE	super_state;	//!< 上位ステート
} GMS_BOSS5_BODY_SUBSEQ_ENTER_INFO;

//! 戦略ステート遷移確率情報構造体
typedef struct tag_GMS_BOSS5_STRAT_PROB_INFO
{
	GME_BOSS5_STRAT_STATE	strat_state;	//!< 戦略ステート
	fx32					probability;	//!< 遷移確率
	BOOL					is_rkt;			//!< ロケットパンチフラグ（ロケットパンチ選択肢除外判定に使用）
} GMS_BOSS5_STRAT_PROB_INFO;

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
//############ 共通 ###########################################################
/* 補助関数 */
#if (GMD_BOSS5_BOOL_USE_OBSOLETE_FUNCTION)
static void gmBoss5SetPartTextureBurnt(OBS_OBJECT_WORK *obj_work);
#endif /* (GMD_BOSS5_BOOL_USE_OBSOLETE_FUNCTION) */
static void gmBoss5InitExplCreate(GMS_BOSS5_EXPL_WORK *expl_work,
								  GME_BOSS5_EXPL_TYPE expl_type,
								  OBS_OBJECT_WORK *parent_obj,
								  fx32 ofst_pos_x, fx32 ofst_pos_y, fx32 width, fx32 height,
								  Uint32 interval_min, Uint32 interval_max, Float se_freq);
static void gmBoss5UpdateExplCreate(GMS_BOSS5_EXPL_WORK *expl_work);
static void gmBoss5SetCameraLift(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5RestoreCameraLift(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5SetCameraSlideForNarrowScreen(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5RestoreCameraSlideForNarrowScreen(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5CamScrLimitReleaseGently(void);
static void gmBoss5CamScrLimitReleaseGentlyProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5HideMapBSide(void);
static void gmBoss5TransferPlayerToASide(void);
static void gmBoss5Vibration(GME_BOSS5_VIB_IDX vib_idx);
static void gmBoss5DelayedVibration(GME_BOSS5_VIB_IDX vib_idx, Uint32 delay);
static void gmBoss5DelayedVibrationProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5DelayedSePlayback(const char *cue_name, Uint32 delay);
static void gmBoss5DelayedSePlaybackProcMain(OBS_OBJECT_WORK *obj_work);
//############ ボスFINAL管理 ##################################################
/* インターフェース関数 */
static void gmBoss5MgrSetAlarmLevel(GMS_BOSS5_MGR_WORK *mgr_work, GME_BOSS5_ALARM_LEVEL alarm_level);
/* 補助関数 */
static inline void gmBoss5MgrSetDemoRunDestPos(GMS_BOSS5_MGR_WORK *mgr_work, fx32 dest_pos_x);
static void gmBoss5InitChasingExpl(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5UpdateChasingExpl(GMS_BOSS5_MGR_WORK *mgr_work);
/* 制御処理 */
static void gmBoss5MgrWaitLoad(OBS_OBJECT_WORK *obj_work);
static void gmBoss5MgrWaitSetup(OBS_OBJECT_WORK *obj_work);
static void gmBoss5MgrMain(OBS_OBJECT_WORK *obj_work);
/* シーケンス */
static void gmBoss5MgrProcInit(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5MgrProcUpdateWaitOpeningDemoBegin(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5MgrProcUpdateOpeningDemo(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5MgrProcUpdateIdle(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5MgrProcUpdateWaitDefeat(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5MgrProcUpdateWaitExplode(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5MgrProcUpdateWaitClosingDemoBegin(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5MgrProcUpdateClosingDemoLeaveBody(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5MgrProcUpdateClosingDemoEscape(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5MgrProcUpdateClosingDemoWaitFadeEnd(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5MgrProcUpdateClosingDemoWaitFinish(GMS_BOSS5_MGR_WORK *mgr_work);

//############ ボスFINAL本体 ##################################################
static void gmBoss5BodyExit(MTS_TASK_TCB *tcb);
/* 補助関数 */
// 汎用
static void gmBoss5BodySetActionWhole(GMS_BOSS5_BODY_WORK *body_work,
									  GME_BOSS5_ACT_ID act_id, BOOL force_change=FALSE);
static void gmBoss5BodySetDirection(GMS_BOSS5_BODY_WORK *body_work, GME_BOSS5_BODY_DIRECTION_TYPE dir_type);
static BOOL gmBoss5BodyIsPlayerBehind(const GMS_BOSS5_BODY_WORK *body_work);
// 当たり関連
static void gmBoss5BodySetNoHitTime(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyUpdateNoHitTime(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodySetPokeTriggerLimitTime(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyUpdatePokeTriggerLimitTime(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyIsWithinPokeTriggerLimitTime(const GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyClearPokeTriggerLimitTime(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyExecDamageRoutine(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyUpdateMainRectPosition(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyUpdateSubRectPosition(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodySetupRect(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyChangeRectSetting(GMS_BOSS5_BODY_WORK *body_work, GME_BOSS5_BODY_RECT_SETTING rect_setting);
static void gmBoss5BodyChangeRectSettingDefault(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodySwitchEnableLegRectOneSide(GMS_BOSS5_BODY_WORK *body_work, GME_BOSS5_LEG_TYPE leg_type);
// プレイヤー操作関連
static void gmBoss5BodyTryImmobilizePlayer(GMS_BOSS5_BODY_WORK *body_work);
// SE関連
static void gmBoss5BodyAllocSeHandles(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyFreeSeHandles(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitPlayTargetSe(GMS_BOSS5_BODY_WORK *body_work, Float init_interval);
static void gmBoss5BodyUpdatePlayTargetSe(GMS_BOSS5_BODY_WORK *body_work);
// シーケンス関連
static void gmBoss5BodyForceEndLeakage(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitPlySearch(GMS_BOSS5_BODY_WORK *body_work, Sint32 delay);
static void gmBoss5BodyUpdatePlySearch(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodySetPlyRebound(GMS_PLAYER_WORK *ply_work, const GMS_BOSS5_BODY_WORK *body_work);
static inline void gmBoss5BodySetMoveFastTime(GMS_BOSS5_BODY_WORK *body_work, Uint32 fast_move_time);
static inline void gmBoss5BodyUpdateMoveFastTime(GMS_BOSS5_BODY_WORK *body_work);
static inline BOOL gmBoss5BodyIsMoveFastEnd(const GMS_BOSS5_BODY_WORK *body_work);
static fx32 gmBoss5BodyGetStompFallPosX(const GMS_BOSS5_BODY_WORK *body_work, fx32 search_pos_x);
static void gmBoss5BodyDecideCrashFallPosX(GMS_BOSS5_BODY_WORK *body_work);
static fx32 gmBoss5BodyGetCrashFallPosX(const GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyCheckJetSmokeClearTiming(const GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyIsMoveFastDirFwd(const GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyIsBodyExplosionStopAllowed(const GMS_BOSS5_BODY_WORK *body_work);
static GME_BOSS5_BODY_DIRECTION_TYPE gmBoss5BodyGetStompFallDirectionType(const GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyTryStartTurret(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyRecordGapAdjustmentDest(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyRecordGapAdjustmentSrc(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitAdjustMtnBlendHGap(GMS_BOSS5_BODY_WORK *body_work,
											  GME_BOSS5_ACT_ID dest_act_id,
											  GME_BOSS5_LEG_TYPE leg_type);
static void gmBoss5BodyUpdateAdjustMtnBlendHGap(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyClearAdjustMtnBlendHGap(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitGroundingMove(GMS_BOSS5_BODY_WORK *body_work, Sint32 ref_snm_reg_id);
static void gmBoss5BodyUpdateGroundingMove(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyChangeMovePhase(GMS_BOSS5_BODY_WORK *body_work,
									   GME_BOSS5_BODY_MOVE_PHASE_TYPE move_phase_type);
static void gmBoss5BodyInitWalk(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyUpdateWalk(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitWalkAbortRecovery(GMS_BOSS5_BODY_WORK *body_work,
											 GME_BOSS5_BODY_MOVE_PHASE_TYPE cur_move_phase_type);
static void gmBoss5BodyInitWalkAbortRecoveryByLegType(GMS_BOSS5_BODY_WORK *body_work,
													  GME_BOSS5_LEG_TYPE leg_type);
static BOOL gmBoss5BodyUpdateWalkAbortRecovery(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitMonitoringWalkEnd(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyUpdateMonitoringWalkEnd(GMS_BOSS5_BODY_WORK *body_work,
											   GME_BOSS5_LEG_TYPE *leg_type);
static void gmBoss5BodyInitWalkGroundingEffects(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyUpdateWalkGroundingEffects(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitRunGroundingEffects(GMS_BOSS5_BODY_WORK *body_work,
											   GME_BOSS5_BODY_RUN_TYPE run_type,
											   Uint32 delay);
static BOOL gmBoss5BodyUpdateRunGroundingEffects(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitStompFlyUp(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyUpdateStompFlyUp(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitStompFall(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyUpdateStompFall(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitCrashFlyUp(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyUpdateCrashFlyUp(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitCrashFall(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyUpdateCrashFall(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitCrashSink(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyUpdateCrashSink(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitBerserkTurn(GMS_BOSS5_BODY_WORK *body_work, GME_BOSS5_BODY_BSK_TURN_TYPE turn_type);
static BOOL gmBoss5BodyUpdateBerserkTurn(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyClearBerserkTurn(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitPoke(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyUpdatePoke(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyEndPoke(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyIsPoking(const GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitArmAnim(GMS_BOSS5_BODY_WORK *body_work,
								   const GMS_BOSS5_ARM_ANIM_INFO *anim_info);
static BOOL gmBoss5BodyUpdateArmAnim(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitCloseCanopy(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyUpdateCloseCanopy(GMS_BOSS5_BODY_WORK *body_work, BOOL is_update);
static void gmBoss5BodyInitScatterFall(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyUpdateScatterFall(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitShakeAccelerate(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodyUpdateShakeAccelerate(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitCrashStrikeVib(GMS_BOSS5_BODY_WORK *body_work, Uint32 delay);
static BOOL gmBoss5BodyUpdateCrashStrikeVib(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitStartRiseVib(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyUpdateStartRiseVib(GMS_BOSS5_BODY_WORK *body_work);
// ノード操作関連
static void gmBoss5BodyInitArmPose(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyEndArmPose(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodySetArmPoseParam(GMS_BOSS5_BODY_WORK *body_work,
									   GME_BOSS5_ARM_TYPE arm_type,
									   GME_BOSS5_ARMPART_IDX arm_part_idx,
									   const NNS_QUATERNION *quat);
static void gmBoss5BodyApplyArmPose(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyInitCanopyPartsPose(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyEndCanopyPartsPose(GMS_BOSS5_BODY_WORK *body_work);
// サブシーケンス関連
static BOOL gmBoss5BodyTryTransitCrashWall(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyResumeMoveFast(GMS_BOSS5_BODY_WORK *body_work);
// シグナル関連
static inline BOOL gmBoss5BodyReceiveSignalRocketReturned(GMS_BOSS5_BODY_WORK *body_work);
/* ノード処理関連 */
static void gmBoss5BodyInitCallbacks(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyReleaseCallbacks(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyRegisterScatterPartsCNM(GMS_BOSS5_BODY_WORK *body_work);
/* 処理関数 */
static void gmBoss5BodyDamageDefFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);
static void gmBoss5BodyOutFunc(OBS_OBJECT_WORK *obj_work);
static void gmBoss5BodyRecFunc(OBS_OBJECT_WORK *obj_work);
/* 制御処理 */
static void gmBoss5BodyChangeState(GMS_BOSS5_BODY_WORK *body_work,
								   GME_BOSS5_BODY_STATE state,
								   GME_BOSS5_STRAT_STATE strat_state,
								   BOOL is_wrapped=FALSE);
static void gmBoss5BodyStartSubsequence(GMS_BOSS5_BODY_WORK *body_work,
										GME_BOSS5_BODY_SUB_SEQ sub_seq);
static void gmBoss5BodyWaitSetup(OBS_OBJECT_WORK *obj_work);
static void gmBoss5BodyMain(OBS_OBJECT_WORK *obj_work);

/* シーケンス */
// 開始ステート
static void gmBoss5BodyStateEnterStart(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateLeaveStart(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateStartWithPlacement(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateStartWithWaitEggRide(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateStartWithCockpitClose(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateStartWithWaitRise(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateStartWithRise(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateStartWithWaitCtplt(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateStartWithWaitEnd(GMS_BOSS5_BODY_WORK *body_work);
// 通常移動ステート
static void gmBoss5BodyStateEnterMoveNml(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateLeaveMoveNml(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateMoveNmlWithLoop(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateMoveNmlWithAbort(GMS_BOSS5_BODY_WORK *body_work);
// 高速移動ステート
static void gmBoss5BodyStateEnterMoveFast(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateLeaveMoveFast(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateMoveFastWithPrep(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateMoveFastWithJump(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateMoveFastWithAir(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateMoveFastWithLand(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateMoveFastWithPreRecover(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateMoveFastWithRecover(GMS_BOSS5_BODY_WORK *body_work);
// ストンプステート
static void gmBoss5BodyStateEnterStomp(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateLeaveStomp(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateStompWithPrep(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateStompWithHover(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateStompWithFlyUp(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateStompWithWait(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateStompWithFall(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateStompWithLand(GMS_BOSS5_BODY_WORK *body_work);
// 地球割りステート
static void gmBoss5BodyStateEnterCrash(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateLeaveCrash(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateCrashWithPrep(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateCrashWithStartFly(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateCrashWithFlyUp(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateCrashWithWait(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateCrashWithFall(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateCrashWithLand(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateCrashWithIdle(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateCrashWithSink(GMS_BOSS5_BODY_WORK *body_work);
// ロケットパンチステート
static void gmBoss5BodyStateEnterRpc(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateLeaveRpc(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateRpcWithPrep(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateRpcWithSearch(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateRpcWithLaunchFirst(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateRpcWithWaitReturnFirst(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateRpcWithLaunchSecond(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateRpcWithWaitReturnSecond(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateRpcWithRecover(GMS_BOSS5_BODY_WORK *body_work);
// 凶暴化演出ステート
static void gmBoss5BodyStateEnterBerserk(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateLeaveBerserk(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateBerserkWithBreakdown(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateBerserkWithShake(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateBerserkWithTurnFront(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateBerserkWithRoarPrep(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateBerserkWithRoarStart(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateBerserkWithRoarLoop(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateBerserkWithTurnSide(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateBerserkWithStamp(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateBerserkWithKickUp(GMS_BOSS5_BODY_WORK *body_work);
// 撃破ステート
static void gmBoss5BodyStateEnterDefeat(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateLeaveDefeat(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateDefeatWithWaitStart(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyStateUpdateDefeatWithExplode(GMS_BOSS5_BODY_WORK *body_work);
/* サブシーケンス */
// 壁衝突サブシーケンス（前・後共有）
static void gmBoss5BodySubSeqEnterMoveFastCrash(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodySubSeqUpdateMoveFastCrashWithStagger(GMS_BOSS5_BODY_WORK *body_work);
// のけぞりサブシーケンス（強パンチ時のよろけ）
static void gmBoss5BodySubSeqEnterRpcStrDmg(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodySubSeqUpdateRpcStrDmgWithBend(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodySubSeqUpdateRpcStrDmgWithSwing(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodySubSeqUpdateRpcStrDmgWithWaitReturn(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodySubSeqUpdateRpcStrDmgWithRecover(GMS_BOSS5_BODY_WORK *body_work);

//############ ボスFINAL中心オブジェクト ######################################
/* 補助関数 */
/* 処理関数 */
/* 制御処理 */
static void gmBoss5CoreWaitSetup(OBS_OBJECT_WORK *obj_work);
static void gmBoss5CoreMain(OBS_OBJECT_WORK *obj_work);
/* シーケンス */
static void gmBoss5CoreProcInit(GMS_BOSS5_CORE_WORK *core_work);
static void gmBoss5CoreProcUpdateLoop(GMS_BOSS5_CORE_WORK *core_work);

//############ ボスFINAL警告フェード ##########################################
static void gmBoss5InitAlarmFade(GMS_BOSS5_MGR_WORK *mgr_work);
static void gmBoss5RequestClearAlarmFade(GMS_BOSS5_MGR_WORK *mgr_work);
/* 補助関数 */
static void gmBoss5AlarmFadeInitFade(GMS_BOSS5_ALARM_FADE_WORK *alarm_fade,
									 GME_BOSS5_ALARM_LEVEL alarm_level);
static BOOL gmBoss5AlarmFadeUpdateFade(GMS_BOSS5_ALARM_FADE_WORK *alarm_fade);
static void gmBoss5AlarmFadeInitAlertSe(GMS_BOSS5_ALARM_FADE_WORK *alarm_fade);
static void gmBoss5AlarmFadeUpdateAlertSe(GMS_BOSS5_ALARM_FADE_WORK *alarm_fade);
/* 処理関数 */
/* 制御処理 */
static void gmBoss5AlarmFadeMain(OBS_OBJECT_WORK *obj_work);
/* シーケンス */
static void gmBoss5AlarmFadeProcInit(GMS_BOSS5_ALARM_FADE_WORK *alarm_fade);
static void gmBoss5AlarmFadeProcUpdateLoop(GMS_BOSS5_ALARM_FADE_WORK *alarm_fade);

//############ ボスFINAL画面フラッシュ ##########################################
static void gmBoss5InitFlashScreen(void);
/* 補助関数 */
/* 処理関数*/
/* シーケンス */
static void gmBoss5FlashScreenMain(OBS_OBJECT_WORK *obj_work);

//############ ボスFINAL終了フェード ##########################################
static void gmBoss5InitLastFadeOut(GMS_BOSS5_MGR_WORK *mgr_work);
/* 補助関数 */
/* 処理関数 */
/* 制御関数 */
static void gmBoss5LastFadeOutMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5LastFadeOutEnd(OBS_OBJECT_WORK *obj_work);

//############ ボスFINALパーツ飛散 ############################################
static void gmBoss5InitScatter(GMS_BOSS5_BODY_WORK *body_work);
/* 補助関数 */
static void gmBoss5ScatterSetPartParam(GMS_BOSS5_SCT_PART_NDC_WORK *sct_part_ndc);
/* 処理関数 */
/* 制御処理 */
/* シーケンス */
static void gmBoss5ScatterProcWait(OBS_OBJECT_WORK *obj_work);
static void gmBoss5ScatterProcFly(OBS_OBJECT_WORK *obj_work);

//############ 動作シーケンス #################################################
static void gmBoss5BodySeqTryRequestEnableStr(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodySeqTryEnableStr(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodySeqIsStr(GMS_BOSS5_BODY_WORK *body_work);
static BOOL gmBoss5BodySeqIsNearDeath(const GMS_BOSS5_BODY_WORK *body_work);
static GME_BOSS5_STRAT_STATE gmBoss5BodySeqLotStrat(GME_BOSS5_BODY_STRAT_BRANCH strat_branch,
													BOOL b_no_rkt=FALSE);
static void gmBoss5BodyProceedToNextSeqNml(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyProceedToNextSeqStr(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5BodyProceedToDefeatState(GMS_BOSS5_BODY_WORK *body_work);
static Uint32 gmBoss5BodySeqGetRpcNmlSearchTime(const GMS_BOSS5_BODY_WORK *body_work);
static Uint32 gmBoss5BodySeqGetRpcStrSearchTime(const GMS_BOSS5_BODY_WORK *body_work);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
// 構築済みモデル(obj_3d)格納リスト
static OBS_ACTION3D_NN_WORK *gm_boss5_obj_3d_list	= NULL;

//! 本体 ステート開始関数テーブル
const static GMS_BOSS5_BODY_STATE_ENTER_INFO gm_boss5_body_state_enter_info_tbl[GME_BOSS5_BODY_STATE_MAX]	= {
	{	NULL,								FALSE	},
	{	gmBoss5BodyStateEnterStart,			FALSE	},
	{	gmBoss5BodyStateEnterMoveNml,		TRUE	},	// STATE_MOVE_NML
	{	gmBoss5BodyStateEnterMoveFast,		TRUE	},	// STATE_MOVE_FAST_FWD
	{	gmBoss5BodyStateEnterMoveFast,		TRUE	},	// STATE_MOVE_FAST_BWD
	{	gmBoss5BodyStateEnterStomp,			TRUE	},	// STATE_STOMP_NML
	{	gmBoss5BodyStateEnterStomp,			TRUE	},	// STATE_STOMP_STR
	{	gmBoss5BodyStateEnterCrash,			TRUE	},	// STATE_CRASH
	{	gmBoss5BodyStateEnterRpc,			TRUE	},	// STATE_RPUNCH_NML
	{	gmBoss5BodyStateEnterRpc,			TRUE	},	// STATE_RPUNCH_STR
	{	gmBoss5BodyStateEnterBerserk,		TRUE	},	// STATE_BERSERK
	{	gmBoss5BodyStateEnterDefeat,		TRUE	},	// STATE_DEFEAT
};

//! 本体 ステート終了関数テーブル
const static GMF_BOSS5_BODY_STATE_LEAVE_FUNC gm_boss5_body_state_leave_func_tbl[GME_BOSS5_BODY_STATE_MAX]	= {
	NULL,
	gmBoss5BodyStateLeaveStart,
	gmBoss5BodyStateLeaveMoveNml,
	gmBoss5BodyStateLeaveMoveFast,
	gmBoss5BodyStateLeaveMoveFast,
	gmBoss5BodyStateLeaveStomp,
	gmBoss5BodyStateLeaveStomp,
	gmBoss5BodyStateLeaveCrash,
	gmBoss5BodyStateLeaveRpc,
	gmBoss5BodyStateLeaveRpc,
	gmBoss5BodyStateLeaveBerserk,
	gmBoss5BodyStateLeaveDefeat,
};

//! 本体 サブシーケンス開始関数テーブル
const static GMS_BOSS5_BODY_SUBSEQ_ENTER_INFO gm_boss5_body_sub_seq_enter_func_tbl[GME_BOSS5_BODY_SUB_SEQ_MAX]	= {
	{	gmBoss5BodySubSeqEnterMoveFastCrash,	GME_BOSS5_BODY_STATE_MOVE_FAST_FWD	},
	{	gmBoss5BodySubSeqEnterMoveFastCrash,	GME_BOSS5_BODY_STATE_MOVE_FAST_BWD	},
	{	gmBoss5BodySubSeqEnterRpcStrDmg,		GME_BOSS5_BODY_STATE_RPUNCH_STR	},
};

//! 本体 戦略ステート・本体ステート対応テーブル
const static GME_BOSS5_BODY_STATE gm_boss5_body_state_strat_tbl[GME_BOSS5_STRAT_STATE_MAX]	= {
	GME_BOSS5_BODY_STATE_NOP,			// GME_BOSS5_STRAT_STATE_NONE
	GME_BOSS5_BODY_STATE_START,			// GME_BOSS5_STRAT_STATE_START
	GME_BOSS5_BODY_STATE_MOVE_NML,		// GME_BOSS5_STRAT_STATE_NML_MOVE_A
	GME_BOSS5_BODY_STATE_MOVE_NML,		// GME_BOSS5_STRAT_STATE_NML_MOVE_B
	GME_BOSS5_BODY_STATE_MOVE_NML,		// GME_BOSS5_STRAT_STATE_NML_MOVE_C
	GME_BOSS5_BODY_STATE_STOMP_NML,		// GME_BOSS5_STRAT_STATE_NML_STOMP_A
	GME_BOSS5_BODY_STATE_STOMP_NML,		// GME_BOSS5_STRAT_STATE_NML_STOMP_B
	GME_BOSS5_BODY_STATE_RPUNCH_NML,	// GME_BOSS5_STRAT_STATE_NML_RPUNCH
	GME_BOSS5_BODY_STATE_BERSERK,		// GME_BOSS5_STRAT_STATE_BERSERK
	GME_BOSS5_BODY_STATE_MOVE_FAST_FWD,	// GME_BOSS5_STRAT_STATE_STR_MOVE_A
	GME_BOSS5_BODY_STATE_MOVE_FAST_FWD,	// GME_BOSS5_STRAT_STATE_STR_MOVE_B
	GME_BOSS5_BODY_STATE_MOVE_FAST_FWD,	// GME_BOSS5_STRAT_STATE_STR_MOVE_C
	GME_BOSS5_BODY_STATE_STOMP_STR,		// GME_BOSS5_STRAT_STATE_STR_STOMP_A
	GME_BOSS5_BODY_STATE_STOMP_STR,		// GME_BOSS5_STRAT_STATE_STR_STOMP_B
	GME_BOSS5_BODY_STATE_RPUNCH_STR,	// GME_BOSS5_STRAT_STATE_STR_RPUNCH_A
	GME_BOSS5_BODY_STATE_RPUNCH_STR,	// GME_BOSS5_STRAT_STATE_STR_RPUNCH_B
	GME_BOSS5_BODY_STATE_CRASH,			// GME_BOSS5_STRAT_STATE_CRASH
};

//! 本体 歩行移動情報テーブル
const static GMS_BOSS5_MOVE_INFO gm_boss5_body_walk_move_info_tbl[GMD_BOSS5_BODY_WALK_MOVE_PHASE_NUM]	= {
	// 	switching_frame		phase_type
	{	0.0f,				GME_BOSS5_BODY_MOVE_PHASE_TYPE_RIGHT,	},
	{	60.f,				GME_BOSS5_BODY_MOVE_PHASE_TYPE_LEFT,	},
	{	142.0f,				GME_BOSS5_BODY_MOVE_PHASE_TYPE_RIGHT,	},
	{	222.0f,				GME_BOSS5_BODY_MOVE_PHASE_TYPE_LEFT,	},
};

//! 本体 歩行移動時 足接地タイミングテーブル（エフェクトの生成や画面振動などのタイミングあわせに使用）
const static GMD_BOSS5_WALK_GROUND_TIMING_INFO gm_boss5_body_walk_ground_timing_info_tbl[GMD_BOSS5_BODY_WALK_GROUND_TIMING_PAHSE_NUM]	= {
	//	grounding_frame		leg_type
	{	0.0f,				GME_BOSS5_LEG_TYPE_RIGHT,	},	// ダミーフェーズ
	{	59.0f,				GME_BOSS5_LEG_TYPE_LEFT,	},
	{	140.0f,				GME_BOSS5_LEG_TYPE_RIGHT,	},
	{	220.0f,				GME_BOSS5_LEG_TYPE_LEFT,	},
	{	298.0f,				GME_BOSS5_LEG_TYPE_RIGHT,	},
};


//! 本体 飛散用CNM対象ノードIDテーブル
const static Sint32 gm_boss5_body_scatter_parts_cnm_node_id_tbl[GME_BOSS5_SCT_PART_IDX_MAX]	= {
	26,	// shoulder
	3,
	7,	// groin
	15,
	27,	// elbow
	4,
	28,	// forearm
	5,
	8,	// thigh
	16,
	9,	// knee
	17,
	10,	// knee pad
	22,
	14,	// foot cover
	21,
	11,	// foot
	18,
	12,	// foot f
	20,
	13,	// foot b
	19,
};

//! パーツ飛散情報テーブル
const static GMS_BOSS5_SCT_PART_INFO gm_boss5_scatter_parts_info_tbl[GME_BOSS5_SCT_PART_IDX_MAX]	= {
	//	cnm_mode						is_local_coord	is_inherit_scale
	{	GME_BS_CMN_CNM_MODE_REPLACE,	FALSE,			TRUE,	},	// GME_BOSS5_SCT_PART_IDX_SHOULDER_L
	{	GME_BS_CMN_CNM_MODE_REPLACE,	FALSE,			TRUE,	},	// GME_BOSS5_SCT_PART_IDX_SHOULDER_R
	{	GME_BS_CMN_CNM_MODE_REPLACE,	FALSE,			TRUE,	},	// GME_BOSS5_SCT_PART_IDX_GROIN_L
	{	GME_BS_CMN_CNM_MODE_REPLACE,	FALSE,			TRUE,	},	// GME_BOSS5_SCT_PART_IDX_GROIN_R
	
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE,	},	// GME_BOSS5_SCT_PART_IDX_ELBOW_L
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE,	},	// GME_BOSS5_SCT_PART_IDX_ELBOW_R
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE,	},	// GME_BOSS5_SCT_PART_IDX_FOREARM_L
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE,	},	// GME_BOSS5_SCT_PART_IDX_FOREARM_R
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE,	},	// GME_BOSS5_SCT_PART_IDX_THIGH_L
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE,	},	// GME_BOSS5_SCT_PART_IDX_THIGH_R
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE,	},	// GME_BOSS5_SCT_PART_IDX_KNEE_L
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE,	},	// GME_BOSS5_SCT_PART_IDX_KNEE_R
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE,	},	// GME_BOSS5_SCT_PART_IDX_KNEEPAD_L
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE,	},	// GME_BOSS5_SCT_PART_IDX_KNEEPAD_R
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE,	},	// GME_BOSS5_SCT_PART_IDX_FOOTCOVER_L
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE,	},	// GME_BOSS5_SCT_PART_IDX_FOOTCOVER_R
	
	{	GME_BS_CMN_CNM_MODE_REPLACE,	FALSE,			TRUE	},	// GME_BOSS5_SCT_PART_IDX_FOOT_L
	{	GME_BS_CMN_CNM_MODE_REPLACE,	FALSE,			TRUE	},	// GME_BOSS5_SCT_PART_IDX_FOOT_R
	
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE	},	// GME_BOSS5_SCT_PART_IDX_TOE_L
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE	},	// GME_BOSS5_SCT_PART_IDX_TOE_R
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE	},	// GME_BOSS5_SCT_PART_IDX_HEEL_L
	{	GME_BS_CMN_CNM_MODE_MULT_LEFT,	TRUE,			FALSE	},	// GME_BOSS5_SCT_PART_IDX_HEEL_R
};

//! パーツ飛散 NDCインデックス列挙型
const static GMS_BOSS5_SCT_NDC_INFO gm_boss5_scatter_ndc_info_tbl[GMD_BOSS5_SCT_NDC_NUM]	= {
	//	part_idx							delay_time
	{	GME_BOSS5_SCT_PART_IDX_SHOULDER_L,	GMD_BOSS5_SCT_ARM_FLY_DELAY_LEFT,	},
	{	GME_BOSS5_SCT_PART_IDX_SHOULDER_R,	GMD_BOSS5_SCT_ARM_FLY_DELAY_RIGHT,	},
	{	GME_BOSS5_SCT_PART_IDX_GROIN_L,		GMD_BOSS5_SCT_LEG_FLY_DELAY_LEFT,	},
	{	GME_BOSS5_SCT_PART_IDX_GROIN_R,		GMD_BOSS5_SCT_LEG_FLY_DELAY_RIGHT,	},
	{	GME_BOSS5_SCT_PART_IDX_FOOT_L,		GMD_BOSS5_SCT_LEG_FLY_DELAY_LEFT,	},
	{	GME_BOSS5_SCT_PART_IDX_FOOT_R,		GMD_BOSS5_SCT_LEG_FLY_DELAY_RIGHT,	},
};

//! 警告フェード情報テーブル
const static GMS_BOSS5_ALARM_FADE_INFO gm_boss5_alarm_fade_info[GME_BOSS5_ALARM_LEVEL_MAX]	= {
	{	40,	90,	40,	180,	},
	{	30,	50,	30,	50,	},
};

//! 警告フェード時SE再生間隔時間テーブル
const static Uint32 gm_boss5_alarm_se_interval_time_tbl[GME_BOSS5_ALARM_LEVEL_MAX]	= {
	120,
	60,
};

// テーブル
#include "gmBoss5DefTbl.inc"

/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GmBoss5Build
/*!
  ボスFINAL データ構築
 */
// =======================================================================
void GmBoss5Build(void)
{
	void	*mdl_amb;
	void	*tex_amb;
	mdl_amb	= ObjDataLoadAmbIndex(NULL, IDB_BOSS05_BOSS05_MDL_AMB, GMD_BOSS5_ARC);
	tex_amb	= ObjDataLoadAmbIndex(NULL, IDB_BOSS05_BOSS05_TEX_AMB, GMD_BOSS5_ARC);
	
	// モデル構築
	gm_boss5_obj_3d_list	=
		GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)mdl_amb, (AMS_AMB_HEADER*)tex_amb,
								  NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
	
	// モーション
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_05_BODY_MTN),
						IDB_BOSS05_BOSS05_BODY_MTN_AMB, GMD_BOSS5_ARC);
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_05_EGG_MTN),
						IDB_BOSS05_BOSS05_EGG_MTN_AMB, GMD_BOSS5_ARC);
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_05_ROCKET_MTN),
						IDB_BOSS05_BOSS05_ROCKET_MTN_AMB, GMD_BOSS5_ARC);
	
	// マテリアルモーション
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_05_CTPLT_MAT),
						IDB_BOSS05_BOSS05_CTPLT_MTN_AMB, GMD_BOSS5_ARC);
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_05_LAND01_MAT),
						IDB_BOSS05_BOSS05_LAND01_MTN_AMB, GMD_BOSS5_ARC);
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_05_LAND02_MAT),
						IDB_BOSS05_BOSS05_LAND02_MTN_AMB, GMD_BOSS5_ARC);
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_05_LAND03_MAT),
						IDB_BOSS05_BOSS05_LAND03_MTN_AMB, GMD_BOSS5_ARC);
	
	// エフェクト構築
	GmBoss5EfctBuild();
}

// =======================================================================
// GmBoss5Flush
/*!
  ボスFINAL データ片付け
 */
// =======================================================================
void GmBoss5Flush(void)
{
	AMS_AMB_HEADER	*mdl_amb;
	
	// エフェクト解放
	GmBoss5EfctFlush();
	
	// マテリアルモーション
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_05_LAND03_MAT));
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_05_LAND02_MAT));
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_05_LAND01_MAT));
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_05_CTPLT_MAT));
	
	// モーション
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_05_ROCKET_MTN));
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_05_EGG_MTN));
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_05_BODY_MTN));
	
	// モデル解放
	mdl_amb	= (AMS_AMB_HEADER*)ObjDataLoadAmbIndex(NULL, IDB_BOSS05_BOSS05_MDL_AMB, GMD_BOSS5_ARC);
	
	GmGameDBuildRegFlushModel(gm_boss5_obj_3d_list, mdl_amb->file_num);
	
	gm_boss5_obj_3d_list	= NULL;
}

// =======================================================================
// GmBoss5Init
/*!
  ボスFINAL（管理）初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss5Init(GMS_EVE_RECORD_EVENT *eve_rec,
							 fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	
	OBS_OBJECT_WORK	*obj_mgr;
	GMS_BOSS5_MGR_WORK	*mgr_work;
	
	// オブジェクト作成
	obj_mgr	= GMM_ENEMY_CREATE_WORK(eve_rec,
									pos_x, pos_y,
									sizeof(GMS_BOSS5_MGR_WORK),
									"BOSS5_MGR");
	
	mgr_work	= (GMS_BOSS5_MGR_WORK*)obj_mgr;
	
	// ワーク設定
	obj_mgr->flag	|= OBD_OBJECT_NOCLIP;
	obj_mgr->disp_flag	|= OBD_DISP_NODISP;
	obj_mgr->move_flag	|= (OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);
	
	// ホーミングアタックの対象からはずす
	mgr_work->ene_3d.ene_com.enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
	
	// ライフ設定
	mgr_work->life	= GMD_BOSS5_LIFE;
	
	// 処理関数設定
	obj_mgr->ppFunc	= gmBoss5MgrWaitLoad;
	
	return obj_mgr;
}

// =======================================================================
// GmBoss5BodyInit
/*!
  ボスFINAL本体初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss5BodyInit(GMS_EVE_RECORD_EVENT *eve_rec,
								 fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	
	OBS_OBJECT_WORK	*obj_work;
	GMS_ENEMY_3D_WORK	*ene_3d;
	GMS_BOSS5_BODY_WORK	*body_work;
	
	// オブジェクト作成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x, pos_y,
										sizeof(GMS_BOSS5_BODY_WORK),
										"BOSS5_BODY");
	
	ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	body_work	= (GMS_BOSS5_BODY_WORK*)obj_work;
	
	// Z位置設定
	obj_work->pos.z	= GMD_BOSS5_DEFAULT_POS_Z;
	
	// 地面位置初期化（保険）
	body_work->ground_v_pos	= pos_y;
	
	// ライフ設定
	ene_3d->ene_com.vit	= 1;	// 使用しないが念のため値を設定
	
	// 地形当たり
	ObjObjectFieldRectSet(obj_work,
						  GMD_BOSS5_BODY_DEFAULT_FIELD_RECT_SIZE_LEFT,
						  GMD_BOSS5_BODY_DEFAULT_FIELD_RECT_SIZE_TOP,
						  GMD_BOSS5_BODY_DEFAULT_FIELD_RECT_SIZE_RIGHT,
						  GMD_BOSS5_BODY_DEFAULT_FIELD_RECT_SIZE_BOTTOM);
	
	// 当たり矩形設定
	gmBoss5BodySetupRect(body_work);
	
	// 本体モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
								 &gm_boss5_obj_3d_list[IDB_BOSS05_MDL_B05_BODY_ZNO],
								 &ene_3d->obj_3d);
	
	// 本体モーションロード
	ObjObjectAction3dNNMotionLoad(obj_work,
								  0,
								  TRUE,
								  ObjDataGet(GMD_DWORK_NO_BOSS_05_BODY_MTN),
								  NULL,
								  0,
								  NULL);
	
	// Wii向けトゥーン設定
	ObjDrawObjectSetToon(obj_work);
	
	// ブレンド速度設定
	obj_work->obj_3d->blend_spd	= GMD_BOSS5_DEFAULT_BLEND_SPD;	// 念のため設定しておく
	
	// ユーザ描画ステートを使用
	obj_work->disp_flag	|= OBD_DISP_DRAWSTATE;
	
	// ワーク設定
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag	|= OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP;
	obj_work->move_flag	&= ~OBD_MOVE_FALL;	// 最初は落下しない
	obj_work->move_flag	&= ~OBD_MOVE_LIMIT_OUT;	// マップ外を壁扱いしない
	obj_work->move_flag	|= OBD_MOVE_NOCOL;	// 最初は地形に当たらない
	obj_work->move_flag	|= OBD_MOVE_NOCOL_W;	// 基本は左右の壁は無視
	
	// ホーミングアタックの対象からはずす
	body_work->ene_3d.ene_com.enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss5BodyWaitSetup;
	
	// 初期アクション設定
	gmBoss5BodyChangeState(body_work, GME_BOSS5_BODY_STATE_NOP, GME_BOSS5_STRAT_STATE_NONE);
	
	// 専用描画処理設定
	obj_work->ppOut	= gmBoss5BodyOutFunc;
	
	// 専用矩形登録処理設定
	obj_work->ppRec	= gmBoss5BodyRecFunc;
	
	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, gmBoss5BodyExit);
	
	// 本体用SEハンドル確保
	gmBoss5BodyAllocSeHandles(body_work);
	
#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}

// =======================================================================
// GmBoss5CoreInit
/*!
  ボスFINAL 中心オブジェクト初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
  
  @note
  常に本体の胴体部中心を追随するオブジェクトです。
  本体の当たり矩形を胴体部位置に設定する処理を行ったり、
  胴体部中心の座標を取得するためなどに使用されます。
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss5CoreInit(GMS_EVE_RECORD_EVENT *eve_rec,
								 fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	
	OBS_OBJECT_WORK	*obj_work;
	GMS_ENEMY_COM_WORK	*ene_com;
	GMS_BOSS5_CORE_WORK	*core_work;
	
	// オブジェクト作成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x, pos_y,
										sizeof(GMS_BOSS5_CORE_WORK),
										"BOSS5_CORE");
	
	ene_com	= (GMS_ENEMY_COM_WORK*)obj_work;
	core_work	= (GMS_BOSS5_CORE_WORK*)obj_work;
	
	// ワーク設定
	obj_work->move_flag	|= OBD_MOVE_NOCOL;
	obj_work->flag	|= OBD_OBJECT_NOCLIP | OBD_OBJECT_NOHIT;
	obj_work->disp_flag	&= ~OBD_DISP_NODISP;	// NODISPを立てるとターゲットカーソルが表示されないのでオフ
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss5CoreWaitSetup;

	return obj_work;
}

// =======================================================================
// GmBoss5GetObject3dList
/*!
  構築済みモデル(obj_3d)格納リスト取得
  
  @return 構築済みモデル格納リスト
 */
// =======================================================================
OBS_ACTION3D_NN_WORK* GmBoss5GetObject3dList(void)
{
	return gm_boss5_obj_3d_list;
}


// =======================================================================
// GmBoss5BodyGetPlySearchPos
/*!
  プレイヤーサーチ 座標取得
  
  @param body_work	[io]	本体ワーク
  @param pos		[out]	座標格納先
 */
// =======================================================================
void GmBoss5BodyGetPlySearchPos(const GMS_BOSS5_BODY_WORK *body_work, VecFx32 *pos)
{
	GmBsCmnGetDelaySearchPos(&body_work->dsearch_work,
							 body_work->ply_search_delay,
							 pos);
}


// =======================================================================
// GmBoss5ScatterSetFlyParam
/*!
  パーツ飛散エフェクト パーツ飛散パラメータ設定
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  主に移動関連のパラメータを設定します。
 */
// =======================================================================
void GmBoss5ScatterSetFlyParam(OBS_OBJECT_WORK *obj_work)
{
	// 左右90度の範囲でランダムに飛散
	// 現在の座標が親より左なら左方向、右なら右方向に飛散
	/*
	  ＼     ／
	  ←＼ ／→
	   ← C →
      ←／ ＼→
	  ／     ＼
	*/
	Sint32	rand_deg	= ((Sint32)mtMathRand() % 90);
	Angle32	rand_angle;
	Float	spd;
	
	if (obj_work->pos.x <= obj_work->parent_obj->pos.x) {
		rand_angle	= AKM_DEGtoA32(rand_deg + 90 + 45);
	}
	else {
		rand_angle	= AKM_DEGtoA32(rand_deg - 45);
	}
	
	spd	= GMD_BOSS5_SCT_NDC_FLY_SPD_FLOAT;
	
	obj_work->spd.y	= (fx32)(FX32_ONE * spd * nnSin(rand_angle));
	obj_work->spd.x	= (fx32)(FX32_ONE * spd * nnCos(rand_angle));
	obj_work->move_flag	|= OBD_MOVE_FALL;
}


// =======================================================================
// GmBoss5Init1ShotTimer
/*!
  1ショットタイマ 初期化
  
  @param one_shot_timer	[io]	1ショットタイマワーク
 */
// =======================================================================
void GmBoss5Init1ShotTimer(GMS_BOSS5_1SHOT_TIMER *one_shot_timer, Uint32 frame)
{
	MTM_ASSERT(one_shot_timer);
	
	one_shot_timer->timer	= frame;
	one_shot_timer->is_active	= TRUE;
}

// =======================================================================
// GmBoss5Update1ShotTimer
/*!
  1ショットタイマ 更新
  
  @param one_shot_timer	[io]	1ショットタイマワーク
  
  @retval TRUE	指定フレーム到達
  @retval FALSE	指定フレームに満たないor超過
  
  @note
  既定フレーム経過したら一度だけTRUEを返すタイマの初期化
 */
// =======================================================================
BOOL GmBoss5Update1ShotTimer(GMS_BOSS5_1SHOT_TIMER *one_shot_timer)
{
	MTM_ASSERT(one_shot_timer);
	
	if (one_shot_timer->is_active == FALSE) {
		return FALSE;
	}
	
	if (one_shot_timer->timer) {
		one_shot_timer->timer--;
	}
	else {
		one_shot_timer->is_active	= FALSE;
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// GmBoss5UpdateVib
/*!
  汎用振動処理 更新
  
  @param phase_cnt	[in]	現在のフェーズ
  @param scale		[in]	スケール
  @param pos_x		[out]	X座標格納先
  @param pos_y		[out]	Y座標格納先
  
  @return 次のフェーズ
 
  @note
  phase_cntがフェーズの最大値を超えていた場合は回り込んだ値が使用されます。
  戻り値も回り込んだ値が返されます。
 */
// =======================================================================
Sint32 GmBoss5UpdateVib(Sint32 phase_cnt, fx32 scale, fx32 *pos_x, fx32 *pos_y)
{
	Sint32	cur_phase;
	Sint32	next_phase;
	
	MTM_ASSERT(phase_cnt >= 0);
	MTM_ASSERT(pos_x);
	MTM_ASSERT(pos_y);
	
	cur_phase	= phase_cnt;
	if (cur_phase >= GMD_BOSS5_VIB_PHASE_NUM) {
		cur_phase	= cur_phase % GMD_BOSS5_VIB_PHASE_NUM;
	}
	
	*pos_x	= FX_Mul(scale, gm_boss5_vib_tbl[phase_cnt][MTD_X]);
	*pos_y	= FX_Mul(scale, gm_boss5_vib_tbl[phase_cnt][MTD_Y]);
	
	next_phase	= cur_phase + 1;
	if (next_phase >= GMD_BOSS5_VIB_PHASE_NUM) {
		next_phase	= 0;
	}
	
	return next_phase;
}

/*------ Static Functions ----------------------------------------------*/

// ############################################################################
// 共通
// ############################################################################
// ============================================================================
// 補助関数
// ============================================================================
#if (GMD_BOSS5_BOOL_USE_OBSOLETE_FUNCTION)
// =======================================================================
// gmBoss5SetPartTextureBurnt
/*!
  パーツのテクスチャを黒こげタイプにする
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  スロット0のテクスチャオフセットをu+0.5しています。
 */
// =======================================================================
void gmBoss5SetPartTextureBurnt(OBS_OBJECT_WORK *obj_work)
{
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	MTM_ASSERT(obj_work->disp_flag & OBD_DISP_DRAWSTATE);
	
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_TEXOFFSET;
	obj_work->obj_3d->draw_state.texoffset[0].mode	= NNE_MATCTRLMODE_ADD;
	obj_work->obj_3d->draw_state.texoffset[0].u	= 0.5f;
}
#endif /* (GMD_BOSS5_BOOL_USE_OBSOLETE_FUNCTION) */

// =======================================================================
// gmBoss5InitExplCreate
/*!
  爆発エフェクト 生成処理初期化
 
  @param expl_work		[io]	爆発ワーク
  @param expl_type		[in]	爆発タイプ
  @param parent_obj		[io]	親オブジェクト
  @param ofst_pos_x		[in]	生成範囲中心座標X
  @param ofst_pos_y		[in]	生成範囲中心座標Y
  @param width			[in]	生成範囲幅
  @param height			[in]	生成範囲高さ
  @param interval_min	[in]	生成間隔最短時間
  @param interval_max	[in]	生成間隔最長時間
  @param se_freq		[in]	SE再生頻度（0.f～1.f）
 */
// =======================================================================
void gmBoss5InitExplCreate(GMS_BOSS5_EXPL_WORK *expl_work,
						   GME_BOSS5_EXPL_TYPE expl_type,
						   OBS_OBJECT_WORK *parent_obj,
						   fx32 ofst_pos_x, fx32 ofst_pos_y, fx32 width, fx32 height,
						   Uint32 interval_min, Uint32 interval_max, Float se_freq)
{
	MTM_ASSERT(expl_work);
	MTM_ASSERT(parent_obj);
	
	expl_work->parent_obj	= parent_obj;
	expl_work->expl_type	= expl_type;
	expl_work->interval_timer	= 0;
	expl_work->interval_min	= interval_min;
	expl_work->interval_max	= interval_max;
	expl_work->se_frequency	= se_freq;
	expl_work->se_freq_cnt	= 0.f;
	expl_work->ofst_pos[MTD_X]	= ofst_pos_x;
	expl_work->ofst_pos[MTD_Y]	= ofst_pos_y;
	expl_work->area[MTD_WIDTH]	= width;
	expl_work->area[MTD_HEIGHT]	= height;
}

// =======================================================================
// gmBoss5UpdateExplCreate
/*!
  爆発エフェクト 生成処理更新
  
  @param expl_work	[io]	爆発ワーク
  
  @note
  初期化時に指定されたパラメータで爆発エフェクトを生成します。
  生成期間中は毎フレーム呼び出してください。
 */
// =======================================================================
void gmBoss5UpdateExplCreate(GMS_BOSS5_EXPL_WORK *expl_work)
{
	MTM_ASSERT(expl_work->parent_obj);
	
	if (expl_work->interval_timer) {
		expl_work->interval_timer--;
	}
	else {
		fx32	width	= expl_work->area[MTD_WIDTH];
		fx32	height	= expl_work->area[MTD_HEIGHT];
		fx32	rand_x;
		fx32	rand_y;
		const VecFx32	*parent_pos	= &expl_work->parent_obj->pos;
		BOOL	is_se_play	= FALSE;
		
		// (pos_x, pos_y) が中心となるwidth x heightの長方形内のランダムな座標を取得
		rand_x	= FX_Mul(AkMathRandFx(), width);
		rand_y	= FX_Mul(AkMathRandFx(), height);
		
		// SE再生周期更新
		if (expl_work->se_freq_cnt < 1.f) {
			expl_work->se_freq_cnt	+= expl_work->se_frequency;
		}
		// 初回更新でも再生判定
		if (expl_work->se_freq_cnt >= 1.f) {
			expl_work->se_freq_cnt	= expl_work->se_freq_cnt - 1.f;
			is_se_play	= TRUE;
		}
		
		// エフェクト生成
		switch (expl_work->expl_type) {
		case GME_BOSS5_EXPL_TYPE_SMALL:	// 小爆発
			GmBoss5EfctCreateSmallExplosion(
				parent_pos->x + expl_work->ofst_pos[MTD_X] - (width >> 1) + rand_x,
				parent_pos->y + expl_work->ofst_pos[MTD_Y] - (height >> 1) + rand_y,
				parent_pos->z + GMD_BOSS5_EXPL_OFST_Z);	// Z方向に一律にずらす
			
			// 小爆発SE再生
			if (is_se_play) {
				GmSoundPlaySE("Boss0_02");
			}
			break;
			
		case GME_BOSS5_EXPL_TYPE_SMALL_WITH_FRAGS:	// 破片付き小爆発
			GmBoss5EfctCreateSmallExplosion(
				parent_pos->x + expl_work->ofst_pos[MTD_X] - (width >> 1) + rand_x,
				parent_pos->y + expl_work->ofst_pos[MTD_Y] - (height >> 1) + rand_y,
				parent_pos->z + GMD_BOSS5_EXPL_OFST_Z);	// Z方向に一律にずらす
			GmBoss5EfctCreateFragments(
				parent_pos->x + expl_work->ofst_pos[MTD_X] - (width >> 1) + rand_x,
				parent_pos->y + expl_work->ofst_pos[MTD_Y] - (height >> 1) + rand_y,
				parent_pos->z + GMD_BOSS5_EXPL_OFST_Z);	// Z方向に一律にずらす
			
			// 小爆発SE再生
			if (is_se_play) {
				GmSoundPlaySE("Boss0_02");
			}
			break;
			
		case GME_BOSS5_EXPL_TYPE_BIG:	// 大爆発
			GmBoss5EfctCreateBigExplosion(
				parent_pos->x + expl_work->ofst_pos[MTD_X] - (width >> 1) + rand_x,
				parent_pos->y + expl_work->ofst_pos[MTD_Y] - (height >> 1) + rand_y,
				parent_pos->z + GMD_BOSS5_EXPL_OFST_Z);	// Z方向に一律にずらす
			
			// 大爆発SE再生
			if (is_se_play) {
				GmSoundPlaySE("Boss0_03");
			}
			
			break;
			
		default:
			MTM_ASSERT(!"gmBoss5Efct::GmBoss5EfctExplUpdateCreate() invalid expl type\n");
			return;
		}
		
		
		// 次の生成までのインターバルを設定
		{
			Uint32	rand_ofst;
			rand_ofst	= (Uint32)((AkMathRandFx() * (expl_work->interval_max - expl_work->interval_min)) >> FX32_SHIFT);
			expl_work->interval_timer = expl_work->interval_min + rand_ofst;
		}
	}
}

// =======================================================================
// gmBoss5SetCameraLift
/*!
  カメラ上昇有効化
  
  @param mgr_work	[io]	管理ワーク
  
  @note
  カメラの中心点を既定の高さまでずらします。
  元の高さに戻す場合はgmBoss5RestoreCameraLift()を呼び出してください。
 */
// =======================================================================
void gmBoss5SetCameraLift(GMS_BOSS5_MGR_WORK *mgr_work)
{
	GMS_PLAYER_WORK	*ply_work	= (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj();
	
	if (!(mgr_work->flag & GMD_BOSS5_MGR_FLAG_CAMERA_OFST_SAVED)) {
		// カメラオフセット保存済み設定
		mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_CAMERA_OFST_SAVED;
	
		mgr_work->save_camera_offset[MTD_X]	= ply_work->gmk_camera_center_ofst_x;
		mgr_work->save_camera_offset[MTD_Y]	= ply_work->gmk_camera_center_ofst_y;
	}
	
	GmPlayerCameraOffsetSet((GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj(),
							0, GMD_BOSS5_CAMERA_LIFT_OFFSET_POS_Y);
}

// =======================================================================
// gmBoss5RestoreCameraLift
/*!
  カメラ上昇設定を戻す
  
  @param mgr_work	[io]	管理ワーク
  
  @note
  カメラオフセットをgmBoss5SetCameraLift()を呼び出す直前の設定に戻します。
 */
// =======================================================================
void gmBoss5RestoreCameraLift(GMS_BOSS5_MGR_WORK *mgr_work)
{
	// 元のオフセットを保存していなかったら何もしない
	if (!(mgr_work->flag & GMD_BOSS5_MGR_FLAG_CAMERA_OFST_SAVED)) {
		return;
	}
	
	// カメラオフセット設定
	GmPlayerCameraOffsetSet((GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj(),
							mgr_work->save_camera_offset[MTD_X],
							mgr_work->save_camera_offset[MTD_Y]);
	
	// 保存値クリア
	mgr_work->save_camera_offset[MTD_X]	=
		mgr_work->save_camera_offset[MTD_Y]	= 0;
	mgr_work->flag	&= ~GMD_BOSS5_MGR_FLAG_CAMERA_OFST_SAVED;
}

// =======================================================================
// gmBoss5SetCameraSlideForNarrowScreen
/*!
  4:3アスペクト比用カメラ横スライド有効化
  
  @param mgr_work	[io]	管理ワーク
  
  @note
  カメラの中心点を既定の長さ横にずらします。
  元の位置に戻す場合はgmBoss5RestoreCameraSlideForNarrowScreen()を呼び出してください。
  画面設定がワイドスクリーンの時は何もしません。
  gmBoss5SetCameraLift()と同時に使用しないでください。
 */
// =======================================================================
void gmBoss5SetCameraSlideForNarrowScreen(GMS_BOSS5_MGR_WORK *mgr_work)
{
	GMS_PLAYER_WORK	*ply_work	= (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj();
	
	// ワイドスクリーンでは何もしない
	if (_am_draw_video.wide_screen) {
		return;
	}
		
	if (!(mgr_work->flag & GMD_BOSS5_MGR_FLAG_CAMERA_OFST_SAVED)) {
		// カメラオフセット保存済み設定
		mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_CAMERA_OFST_SAVED;
		
		mgr_work->save_camera_offset[MTD_X]	= ply_work->gmk_camera_center_ofst_x;
		mgr_work->save_camera_offset[MTD_Y]	= ply_work->gmk_camera_center_ofst_y;
	}
	
	GmPlayerCameraOffsetSet((GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj(),
							GMD_BOSS5_CAMERA_SLIDE_FOR_NARROW_OFFSET_POS_X, 0);
}

// =======================================================================
// gmBoss5RestoreCameraLift
/*!
  4:3アスペクト比用カメラ横スライド設定を戻す
  
  @param mgr_work	[io]	管理ワーク
  
  @note
  カメラオフセットをgmBoss5SetCameraSlideForNarrowScreen()を呼び出す直前の設定に戻します。
  画面設定がワイドスクリーンの時は何もしません。
 */
// =======================================================================
void gmBoss5RestoreCameraSlideForNarrowScreen(GMS_BOSS5_MGR_WORK *mgr_work)
{
	// ワイドスクリーンでは何もしない
	if (_am_draw_video.wide_screen) {
		return;
	}
	
	// 元のオフセットを保存していなかったら何もしない
	if (!(mgr_work->flag & GMD_BOSS5_MGR_FLAG_CAMERA_OFST_SAVED)) {
		return;
	}
	
	// カメラオフセット設定
	GmPlayerCameraOffsetSet((GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj(),
							mgr_work->save_camera_offset[MTD_X],
							mgr_work->save_camera_offset[MTD_Y]);
	
	// 保存値クリア
	mgr_work->save_camera_offset[MTD_X]	=
		mgr_work->save_camera_offset[MTD_Y]	= 0;
	mgr_work->flag	&= ~GMD_BOSS5_MGR_FLAG_CAMERA_OFST_SAVED;
}

// =======================================================================
// gmBoss5CamScrLimitReleaseGently
/*!
  スクロール制限解除
  
  @note
  GmGmkCamScrLimitRelease()を使用せずに、
  緩やかなスクロール解除を独自処理で行います。
  非表示エフェクトタスクを生成して処理を行っています。
 */
// =======================================================================
void gmBoss5CamScrLimitReleaseGently(void)
{
	OBS_OBJECT_WORK	*obj_work;
	
	obj_work	= GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_COM_WORK),
										 NULL,
										 0,
										 "scr_lim_rel_gently");
	
	// 表示はしない
	obj_work->disp_flag	|= (OBD_DISP_NODISP | OBD_DISP_NOUPDATE);
	
	// クリッピングしない
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	
	// 右限界の移動初速度設定
	obj_work->user_timer	= GMD_BOSS5_CAM_SCR_LIMIT_RELEASE_GNTL_SPD_X_INIT;
	
	obj_work->ppFunc	= gmBoss5CamScrLimitReleaseGentlyProcMain;
}

// =======================================================================
// gmBoss5CamScrLimitReleaseGentlyProcMain
/*!
  スクロール制限解除処理 処理関数
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  右スクロール制限をマップの右端まで移動させることで徐々に解除していきます。
 */
// =======================================================================
void gmBoss5CamScrLimitReleaseGentlyProcMain(OBS_OBJECT_WORK *obj_work)
{
	fx32	right_limit_max	= (fx32)(g_gm_main_system.map_fcol.map_block_num_x*64 << FX32_SHIFT);
	fx32	cur_right_limit	= (fx32)(g_gm_main_system.map_fcol.right << FX32_SHIFT);
	fx32	next_right_limit;
	BOOL	is_end	= FALSE;
	
	// 右スクロール制限移動加速
	// （プレイヤーが右スクロール制限の移動に追いつかないようにする）
	obj_work->user_timer	= obj_work->user_timer + GMD_BOSS5_CAM_SCR_LIMIT_RELEASE_GNTL_ACC_X;
	obj_work->user_timer	= MTM_MATH_CLIP(obj_work->user_timer, 0, FX32_MAX);
	
	
	next_right_limit	= cur_right_limit	+ obj_work->user_timer;
	if (next_right_limit >= right_limit_max) {
		next_right_limit	= right_limit_max;
		is_end	= TRUE;
	}
	
	// 右スクロール制限セット
	{
		GMS_EVE_RECORD_EVENT eve_rec;
		eve_rec.flag	= GMD_GMK_SCR_LMT_EVE_FLAG_RIGHT;
		eve_rec.left	= 0;
		eve_rec.top		= 0;
		eve_rec.width	= 0;
		eve_rec.height	= 0;
		GmCamScrLimitSetDirect(&eve_rec, next_right_limit, 0);
	}
	
	if (is_end) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5HideMapBSide
/*!
  近景マップB面非表示
  
  @note
  近景マップのB面表示を無効化します。
 */
// =======================================================================
void gmBoss5HideMapBSide(void)
{
	GmMapSetDispB(FALSE);
}

// =======================================================================
// gmBoss5TransferPlayerToASide
/*!
  プレイヤーをA面に移動
  
  @note
  プレイヤーをA面側に移動します。
 */
// =======================================================================
void gmBoss5TransferPlayerToASide(void)
{
	OBS_OBJECT_WORK	*ply_obj	= GmBsCmnGetPlayerObj();
	GMS_PLAYER_WORK	*ply_work	= (GMS_PLAYER_WORK*)ply_obj;
	
	// A面へ移行
	ply_obj->flag &= ~OBD_OBJECT_B;
	ply_work->graind_prev_ride = 0;
}

// =======================================================================
// gmBoss5Vibration
/*!
  振動処理
  
  @param vib_idx	[in]	振動番号
 */
// =======================================================================
void gmBoss5Vibration(GME_BOSS5_VIB_IDX vib_idx)
{
	MTM_ASSERT(vib_idx < GME_BOSS5_VIB_IDX_MAX);
	
	GmCameraVibrationSet(gm_boss5_vib_param_tbl[vib_idx][MTD_X],
						 gm_boss5_vib_param_tbl[vib_idx][MTD_Y],
						 gm_boss5_vib_param_tbl[vib_idx][MTD_Z]);
}

// =======================================================================
// gmBoss5DelayedVibration
/*!
  遅延振動処理
 
  @param vib_idx	[in]	振動番号
  @param delay		[in]	遅延フレーム数
  
  @note
  非表示エフェクトオブジェクトを生成しています。
 */
// =======================================================================
void gmBoss5DelayedVibration(GME_BOSS5_VIB_IDX vib_idx, Uint32 delay)
{
	OBS_OBJECT_WORK	*obj_work;
	
	MTM_ASSERT(vib_idx < GME_BOSS5_VIB_IDX_MAX);
	
	obj_work	= GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_COM_WORK),
										 NULL,
										 0,
										 "boss5_delay_vib");
	
	// 表示はしない
	obj_work->disp_flag	|= (OBD_DISP_NODISP | OBD_DISP_NOUPDATE);
	
	// クリッピングしない
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	
	// 振動番号保存
	obj_work->user_work	= vib_idx;
	// 遅延タイマ設定
	obj_work->user_timer	= delay;
	
	obj_work->ppFunc	= gmBoss5DelayedVibrationProcMain;
}

// =======================================================================
// gmBoss5DelayedVibrationProcMain
/*!
  遅延振動処理 処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5DelayedVibrationProcMain(OBS_OBJECT_WORK *obj_work)
{
	// 既定時間後に振動を発生
	if (obj_work->user_timer) {
		obj_work->user_timer--;
	}
	else {
		gmBoss5Vibration((GME_BOSS5_VIB_IDX)obj_work->user_work);
		
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5DelayedSePlayback
/*!
  遅延SE再生処理
 
  @param cue_name	[in]	CUE名
  @param delay		[in]	遅延フレーム数
  
  @note
  非表示エフェクトオブジェクトを生成しています。
 */
// =======================================================================
void gmBoss5DelayedSePlayback(const char *cue_name, Uint32 delay)
{
	OBS_OBJECT_WORK	*obj_work;
	
	obj_work	= GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_COM_WORK),
										 NULL,
										 0,
										 "boss5_delay_se");
	
	// 表示はしない
	obj_work->disp_flag	|= (OBD_DISP_NODISP | OBD_DISP_NOUPDATE);
	
	// クリッピングしない
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	
	// CUE ID保存
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if	= amCriAudioGetGlobal();
	obj_work->user_work	=
		cri_audio_if->CueSheet[AME_CRIAUDIO_CSB_GAME]->GetCueId(cue_name,
																cri_audio_if->err);
	// 遅延タイマ設定
	obj_work->user_timer	= delay;
	
	obj_work->ppFunc	= gmBoss5DelayedSePlaybackProcMain;
}

// =======================================================================
// gmBoss5DelayedSePlaybackProcMain
/*!
  遅延SE再生処理 処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5DelayedSePlaybackProcMain(OBS_OBJECT_WORK *obj_work)
{
	// 既定時間後にSEを再生
	if (obj_work->user_timer) {
		obj_work->user_timer--;
	}
	else {
		GsSoundPlaySeById(obj_work->user_work);
		
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}


// ############################################################################
// ボスFINAL 管理
// ############################################################################

// ============================================================================
// インターフェース関数
// ============================================================================
// =======================================================================
// gmBoss5MgrSetAlarmLevel
/*!
  警告レベル設定
  
  @param mgr_work		[io]	管理ワーク
  @param alarm_level	[in]	警告レベル
 */
// =======================================================================
void gmBoss5MgrSetAlarmLevel(GMS_BOSS5_MGR_WORK *mgr_work, GME_BOSS5_ALARM_LEVEL alarm_level)
{
	MTM_ASSERT(mgr_work);
	MTM_ASSERT(alarm_level < GME_BOSS5_ALARM_LEVEL_MAX);
	
	mgr_work->alarm_level	= alarm_level;
}

// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss5MgrSetDemoRunDestPos
/*!
  プレイヤー演出走りの移動目標座標設定
  
  @param mgr_work	[io]	管理ワーク
  @param dest_pos_x	[in]	目標地点座標X
  
  @note
  設定した座標に到達するとプレイヤーが減速開始します。
 */
// =======================================================================
inline void gmBoss5MgrSetDemoRunDestPos(GMS_BOSS5_MGR_WORK *mgr_work, fx32 dest_pos_x)
{
	mgr_work->ply_demo_run_dest_x	= dest_pos_x;
}

// =======================================================================
// gmBoss5InitChasingExpl
/*!
  プレイヤー追いかけ爆発 初期化
  
  @param mgr_work	[io]	管理ワーク
  
  @note
  爆発生成範囲の座標をプレイヤー後方で追随させます。
 */
// =======================================================================
void gmBoss5InitChasingExpl(GMS_BOSS5_MGR_WORK *mgr_work)
{
	gmBoss5InitExplCreate(&mgr_work->small_expl_work,
						  GME_BOSS5_EXPL_TYPE_SMALL,
						  GmBsCmnGetPlayerObj(),
						  GMD_BOSS5_EXPL_CHASE_SMALL_OFST_X,
						  GMD_BOSS5_EXPL_CHASE_SMALL_OFST_Y,
						  GMD_BOSS5_EXPL_CHASE_SMALL_WIDTH,
						  GMD_BOSS5_EXPL_CHASE_SMALL_HEIGHT,
						  GMD_BOSS5_EXPL_CHASE_SMALL_INTERVAL_MIN,
						  GMD_BOSS5_EXPL_CHASE_SMALL_INTERVAL_MAX,
						  GMD_BOSS5_EXPL_CHASE_SMALL_SE_FREQUENCY);
	
	gmBoss5InitExplCreate(&mgr_work->big_expl_work,
						  GME_BOSS5_EXPL_TYPE_BIG,
						  GmBsCmnGetPlayerObj(),
						  GMD_BOSS5_EXPL_CHASE_BIG_OFST_X + GMD_BOSS5_EXPL_CHASE_BIG_DELAY_OFST_X,
						  GMD_BOSS5_EXPL_CHASE_BIG_OFST_Y,
						  GMD_BOSS5_EXPL_CHASE_BIG_WIDTH,
						  GMD_BOSS5_EXPL_CHASE_BIG_HEIGHT,
						  GMD_BOSS5_EXPL_CHASE_BIG_INTERVAL_MIN,
						  GMD_BOSS5_EXPL_CHASE_BIG_INTERVAL_MAX,
						  GMD_BOSS5_EXPL_CHASE_BIG_SE_FREQUENCY);
}

// =======================================================================
// gmBoss5UpdateChasingExpl
/*!
  プレイヤー追いかけ爆発 更新
  
  @param mgr_work	[io]	管理ワーク
 */
// =======================================================================
void gmBoss5UpdateChasingExpl(GMS_BOSS5_MGR_WORK *mgr_work)
{
	GMS_BOSS5_EXPL_WORK	*small_expl_work	= &mgr_work->small_expl_work;
	GMS_BOSS5_EXPL_WORK	*big_expl_work	= &mgr_work->big_expl_work;
	
	// 徐々にプレイヤーとの距離を詰める
	small_expl_work->ofst_pos[MTD_X]	+= GMD_BOSS5_EXPL_CHASE_SMALL_CHASE_SPD_X;
	if (small_expl_work->ofst_pos[MTD_X] >= GMD_BOSS5_EXPL_CHASE_SMALL_CHASE_OFST_X_MAX) {
		small_expl_work->ofst_pos[MTD_X]	= GMD_BOSS5_EXPL_CHASE_SMALL_CHASE_OFST_X_MAX;
	}
	
	// 徐々にプレイヤーとの距離を詰める
	big_expl_work->ofst_pos[MTD_X]	+= GMD_BOSS5_EXPL_CHASE_BIG_CHASE_SPD_X;
	if (big_expl_work->ofst_pos[MTD_X] >= GMD_BOSS5_EXPL_CHASE_BIG_CHASE_OFST_X_MAX + GMD_BOSS5_EXPL_CHASE_BIG_DELAY_OFST_X) {
		big_expl_work->ofst_pos[MTD_X]	= GMD_BOSS5_EXPL_CHASE_BIG_CHASE_OFST_X_MAX + GMD_BOSS5_EXPL_CHASE_BIG_DELAY_OFST_X;
	}
	
	// エフェクト生成更新
	gmBoss5UpdateExplCreate(&mgr_work->small_expl_work);
	gmBoss5UpdateExplCreate(&mgr_work->big_expl_work);
}


// ============================================================================
// 制御処理
// ============================================================================
// =======================================================================
// gmBoss5MgrWaitLoad
/*!
  本体 ロード・ビルド完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5MgrWaitLoad(OBS_OBJECT_WORK *obj_work)
{
	BOOL	is_load_end	= FALSE;
	
#if defined(GMD_BOSS5_DEBUG_PROGRESS_CHECK)
	if ((g_gs_main_sys_info.debug_flag & GSD_DEBUG_ZONE_FINAL_5_CHECK) &&
		!(g_gs_main_sys_info.debug_flag & GSD_DEBUG_ZONE_FINAL_5_CHECK_DONE)) {
		
		if (GmBsCmnGetPlayerObj() == NULL) {
			;
		}
		else {
			GmBsCmnGetPlayerObj()->pos.x	= 14430 * FX32_ONE;
			GmBsCmnGetPlayerObj()->pos.y	= 620 * FX32_ONE;
			// カメラ位置セット
			GmCameraPosSet(GmBsCmnGetPlayerObj()->pos.x, GmBsCmnGetPlayerObj()->pos.y, 0);
			
			OBS_CAMERA	*obj_camera;
			obj_camera = ObjCameraGet(g_obj.glb_camera_id);
			// オブジェクトカメラ設定
			ObjObjectCameraSet(FXM_FLOAT_TO_FX32(obj_camera->disp_pos.x - (float)(OBD_LCD_X/2)),
							   FXM_FLOAT_TO_FX32(-obj_camera->disp_pos.y - (float)(OBD_LCD_Y/2)),
							   FXM_FLOAT_TO_FX32(obj_camera->disp_pos.x - (float)(OBD_LCD_X/2)),
							   FXM_FLOAT_TO_FX32(-obj_camera->disp_pos.y - (float)(OBD_LCD_Y/2)));

			
			// クリッピングカメラ設定
			GmCameraSetClipCamera(obj_camera);
			
			GmEveMgrCreateEventLcd( GMD_EVE_SEARCH_PROC_FLAG_NONE );
			
			g_gs_main_sys_info.debug_flag	|= GSD_DEBUG_ZONE_FINAL_5_CHECK_DONE;
		}
	}
#endif /* defined(GMD_BOSS5_DEBUG_PROGRESS_CHECK) */
	
	// データロード待ち
	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_FINAL_1 ||
		GmMainDatLoadBossBattleLoadCheck(GMD_GAMEDAT_LOAD_BOSS_TYPE_F)) {
		is_load_end	= TRUE;
	}
	
	// データロード・ビルドが完了した時点で各パーツを生成
	if (is_load_end) {
		GMS_BOSS5_MGR_WORK	*mgr_work	= (GMS_BOSS5_MGR_WORK*)obj_work;
		OBS_OBJECT_WORK	*obj_body;
		OBS_OBJECT_WORK	*obj_core;
		GMS_BOSS5_BODY_WORK	*body_work;
		
		obj_body	= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS5_BODY,
												obj_work->pos.x, obj_work->pos.y,
												0,//flag
												0,0,0,0,
												0);
		
		obj_core	= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS5_CORE,
												obj_work->pos.x, obj_work->pos.y,
												0,//flag
												0,0,0,0,
												0);
		
		body_work	= (GMS_BOSS5_BODY_WORK*)obj_body;
		
		// 本体を管理の子に設定
		mgr_work->body_work	= body_work;
		
		// 本体に管理ワークの参照設定
		body_work->mgr_work	= mgr_work;
		
		// 各パーツの親設定
		obj_body->parent_obj	= obj_work;
		obj_core->parent_obj	= obj_body;
		
		// 各パーツへの参照を設定
		body_work->parts_objs[GME_BOSS5_PART_IDX_BODY]	= obj_body;
		body_work->part_obj_core	= obj_core;	// アクション設定しないのでparts_objsとは別のメンバに格納
		
		// 処理関数設定
		obj_work->ppFunc	= gmBoss5MgrWaitSetup;
	}
}

// =======================================================================
// gmBoss5MgrWaitSetup
/*!
  管理 セットアップ完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5MgrWaitSetup(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_MGR_WORK	*mgr_work	= (GMS_BOSS5_MGR_WORK*)obj_work;
	GMS_BOSS5_BODY_WORK	*body_work	= mgr_work->body_work;
	BOOL	result	= TRUE;
	
	// 本体パーツの処理で、全てのパーツのアクションをまとめて設定する必要があるので、
	// パーツへの参照が一通り揃うのを待つ
	for (Sint32 i = 0; i < GME_BOSS5_PART_IDX_MAX; ++i) {
		if (body_work->parts_objs[i] == NULL) {
			result	= FALSE;
		}
	}
	
	if (body_work->part_obj_core == NULL) {
		result	= FALSE;
	}
	
	if (result) {
		mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_SETUP_END;
		obj_work->ppFunc	= gmBoss5MgrMain;
		
		// シーケンス初期化
		gmBoss5MgrProcInit(mgr_work);
	}
}

// =======================================================================
// gmBoss5MgrMain
/*!
  管理 メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5MgrMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_MGR_WORK	*mgr_work	= (GMS_BOSS5_MGR_WORK*)obj_work;
	
#if defined(GMD_BOSS5_DEBUG_PROGRESS_CHECK)
//	if (PAD_STAND(0)& KEY_R_RIGHT) {
//		OBS_OBJECT_WORK	*land_obj	= (OBS_OBJECT_WORK*)GmBoss5LandCreate(mgr_work);
//		land_obj->pos.x	= GmBsCmnGetPlayerObj()->pos.x;
//	}
//	else if (PAD_STAND(0) & KEY_R1) {
//		mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_LAND_SHAKE_NEEDED;
//	}
//	else if (PAD_STAND(0) & KEY_R2) {
//		mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_LAND_BREAK_NEEDED;
//	}
#endif /* defined(GMD_BOSS5_DEBUG_PROGRESS_CHECK) */
	
	if (mgr_work->flag & GMD_BOSS5_MGR_FLAG_CLEAR_BOSS) {
		if (mgr_work->body_work) {
			GMM_BS_OBJ(mgr_work->body_work)->flag	|= OBD_OBJECT_TASKCLEAR_REQUEST;
			mgr_work->body_work	= NULL;
		}
	}
	
	// 更新処理
	if (mgr_work->proc_update) {
		mgr_work->proc_update(mgr_work);
	}
}


// ============================================================================
// シーケンス
// ============================================================================
// =======================================================================
// gmBoss5MgrProc****
/*!
  管理シーケンス
  
  @param mgr_work	[io]	管理ワーク
  
  @note
  全体の同期処理や、演出などを行います。
 */
// =======================================================================
// 管理シーケンス初期化
void gmBoss5MgrProcInit(GMS_BOSS5_MGR_WORK *mgr_work)
{
	mgr_work->proc_update	= gmBoss5MgrProcUpdateWaitOpeningDemoBegin;
}

// 管理シーケンス更新 開始デモ開始待ち
void gmBoss5MgrProcUpdateWaitOpeningDemoBegin(GMS_BOSS5_MGR_WORK *mgr_work)
{
	if (mgr_work->flag & GMD_BOSS5_MGR_FLAG_BODY_STOOD_BY) {
		if (mgr_work->flag & GMD_BOSS5_MGR_FLAG_PLY_PASSED_TRG) {
			
			GMS_PLAYER_WORK	*ply_work	= (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj();
			
			// ゲームタイマ停止
			g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_COUNT_GAME_TIME;
			
			// FINALボス用BGM開始
			GmSoundChangeFinalBossBGM();
			
			// プレイヤーをFINALボス演出シーケンスに変更
			GmPlySeqChangeBoss5Demo(ply_work,
									mgr_work->ply_demo_run_dest_x,
									FALSE);
			
			mgr_work->proc_update	= gmBoss5MgrProcUpdateOpeningDemo;
			
#if _IPHONE
			GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_ZONEF_BOSS);
#endif // _IPHONE
		}
	}
}

// 管理シーケンス更新 開始デモ終了待ち
void gmBoss5MgrProcUpdateOpeningDemo(GMS_BOSS5_MGR_WORK *mgr_work)
{
	// REMINDER :
	//    何らかの理由でプレイヤーシーケンスが「FINALボス演出シーケンス」にならなかった場合を想定し、
	//    プレイヤーが「FINALボス演出シーケンス」になるのを待って同期する、といったことは行いません。
	//    （ただし、デバッグ版では当該シーケンスでないとアサートに失敗します）
	
	// 本体側のデモ処理完了待ち
	if (mgr_work->flag & GMD_BOSS5_MGR_FLAG_BODY_DEMO_IS_FINISHED) {
		GMS_PLAYER_WORK	*ply_work	= (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj();
		
		// ゲームタイマ再開
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_COUNT_GAME_TIME;
		
		// プレイヤーのボスFINAL演出シーケンスを一方的に解除
		MTM_ASSERT(ply_work->seq_state == GME_PLY_SEQ_STATE_BOSS5_DEMO);
		GmPlySeqChangeBoss5DemoEnd(ply_work);
		
		mgr_work->proc_update	= gmBoss5MgrProcUpdateIdle;
	}
}

// 管理シーケンス更新 停滞処理（i.e.本体が戦っている時）
void gmBoss5MgrProcUpdateIdle(GMS_BOSS5_MGR_WORK *mgr_work)
{
	// 足場生成・処理面変更開始待ち
	if (mgr_work->flag & GMD_BOSS5_MGR_FLAG_LAND_NEEDED) {
		
		// 足場生成
		GmBoss5LandCreate(mgr_work);
		
		// プレイヤーをA面に移動
		gmBoss5TransferPlayerToASide();
		
		// B面消去
		gmBoss5HideMapBSide();
		
		mgr_work->proc_update	= gmBoss5MgrProcUpdateWaitDefeat;
	}
}

// 管理シーケンス更新 本体撃破待ち
void gmBoss5MgrProcUpdateWaitDefeat(GMS_BOSS5_MGR_WORK *mgr_work)
{
	// 本体撃破待ち
	if (mgr_work->flag & GMD_BOSS5_MGR_FLAG_BODY_DEFEAT) {
		
		// ゲームタイマ停止
		g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_COUNT_GAME_TIME;
		// ゴール通知（これ以降ポーズが禁止になるので↑のゲームタイマ停止と必ず同じタイミングでセット）
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_GOAL_IN;
		
		// 爆発中待ち時間設定
		mgr_work->wait_timer	= GMD_BOSS5_MGR_WAIT_EXPLODE_TIME;
		
		// 振動中（後で止める）
		GmPadVibSet(GMD_PAD_VIB_MID_TYPE, -1.f,
					GMD_PAD_VIB_MID_LEFT_VIB, GMD_PAD_VIB_MID_RIGHT_VIB,
					-1.f,
					GMD_PAD_VIB_MID_INT_VIB_TIME, GMD_PAD_VIB_MID_INT_STOP_TIME,
					GMD_PAD_VIB_LARGE_PRIO);	// プライオリティは高めに設定
		
		mgr_work->proc_update	= gmBoss5MgrProcUpdateWaitExplode;
	}
}

// 管理シーケンス更新 爆発中処理
void gmBoss5MgrProcUpdateWaitExplode(GMS_BOSS5_MGR_WORK *mgr_work)
{
	// 既定時間待機
	if (mgr_work->wait_timer) {
		mgr_work->wait_timer--;
	}
	else {
		// 終了デモ開始最大待ち時間設定
		mgr_work->wait_timer	= GMD_BOSS5_MGR_CLOSING_DEMO_WAIT_BEGIN_TIME_MAX;
		
		// スクロールロック解除
		gmBoss5CamScrLimitReleaseGently();
		
		mgr_work->proc_update	= gmBoss5MgrProcUpdateWaitClosingDemoBegin;
	}
}

// 管理シーケンス更新 終了デモ開始待ち処理
void gmBoss5MgrProcUpdateWaitClosingDemoBegin(GMS_BOSS5_MGR_WORK *mgr_work)
{
	// 既定時間経過したら強制的に進行
	if (mgr_work->wait_timer) {
		mgr_work->wait_timer--;
	}
	
	if (GmBsCmnGetPlayerObj()->move_flag & OBD_MOVE_UNDER ||
		mgr_work->wait_timer == 0) {
		
		GMS_PLAYER_WORK	*ply_work	= (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj();
		
		// プレイヤーをマップの右端まで延々と走らせる
		GmPlySeqChangeBoss5Demo(ply_work,
								g_gm_main_system.map_size[MTD_X] << FX32_SHIFT,
								TRUE);
		
		mgr_work->proc_update	= gmBoss5MgrProcUpdateClosingDemoLeaveBody;
	}
}

//管理シーケンス更新 終了デモ本体から離れる処理
void gmBoss5MgrProcUpdateClosingDemoLeaveBody(GMS_BOSS5_MGR_WORK *mgr_work)
{
	if (mgr_work->flag & GMD_BOSS5_MGR_FLAG_CHASING_EXPL_NEEDED) {
		// プレイヤー追いかけ爆発初期化
		gmBoss5InitChasingExpl(mgr_work);
		
		// 終了デモ継続時間設定
		mgr_work->wait_timer	= GMD_BOSS5_MGR_CLOSING_DEMO_DURATION_TIME;
		
		mgr_work->proc_update	= gmBoss5MgrProcUpdateClosingDemoEscape;
	}
}

// 管理シーケンス更新 終了デモ脱出走り処理
void gmBoss5MgrProcUpdateClosingDemoEscape(GMS_BOSS5_MGR_WORK *mgr_work)
{
	// 追いかけ爆発更新
	gmBoss5UpdateChasingExpl(mgr_work);
	
	if (mgr_work->wait_timer) {
		mgr_work->wait_timer--;
	}
	else {
		// 終了フェードアウトを開始
		gmBoss5InitLastFadeOut(mgr_work);
		
		mgr_work->proc_update	= gmBoss5MgrProcUpdateClosingDemoWaitFadeEnd;
	}
}

// 管理シーケンス更新 終了デモ フェード完了待ち
void gmBoss5MgrProcUpdateClosingDemoWaitFadeEnd(GMS_BOSS5_MGR_WORK *mgr_work)
{
	// 追いかけ爆発更新
	gmBoss5UpdateChasingExpl(mgr_work);
	
	// 終了フェード完了待ち
	if (mgr_work->flag & GMD_BOSS5_MGR_FLAG_LAST_FADE_END) {
		
		// 振動中停止
		GMM_PAD_VIB_STOP();
		
		// 白画面待機時間設定
		mgr_work->wait_timer	= GMD_BOSS5_MGR_CLOSING_DEMO_WHITEOUT_TIME;
		
		mgr_work->proc_update	= gmBoss5MgrProcUpdateClosingDemoWaitFinish;
	}
}

// 管理シーケンス更新 終了デモ 完了待ち
void gmBoss5MgrProcUpdateClosingDemoWaitFinish(GMS_BOSS5_MGR_WORK *mgr_work)
{
	// 白画面で既定時間待機
	if (mgr_work->wait_timer) {
		mgr_work->wait_timer--;
	}
	else {
		// ステージクリア通知
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_CLEAR;		// ステージクリア
		
		mgr_work->proc_update	= NULL;
	}
}


// ############################################################################
// ボスFINAL 本体
// ############################################################################
// =======================================================================
// gmBoss5BodyExit
/*!
  本体終了処理
  
  @note
  標準の終了処理に加えて、ノードマトリクス取得関連バッファの解放などを行っています。
 */
// =======================================================================
void gmBoss5BodyExit(MTS_TASK_TCB *tcb)
{
	GMS_BOSS5_BODY_WORK	*body_work	= (GMS_BOSS5_BODY_WORK*)mtTaskGetTcbWork(tcb);
	
	// コールバック関係解放
	gmBoss5BodyReleaseCallbacks(body_work);
	
	// 本体用SEハンドル解放
	gmBoss5BodyFreeSeHandles(body_work);
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}


// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss5BodySetActionWhole
/*!
  ボスFINAL全体アクション設定
  
  @param body_work	[io]	本体ワーク
  @parma act_id		[in]	全体アクションインデックス(GME_BOSS5_ACT_ID_XXX)
 */
// =======================================================================
void gmBoss5BodySetActionWhole(GMS_BOSS5_BODY_WORK *body_work,
							   GME_BOSS5_ACT_ID act_id, BOOL force_change/*=FALSE*/)
{
	const GMS_BOSS5_PART_ACT_INFO	*pt_act_info	= gm_boss5_act_id_tbl[act_id];
	
	// 設定済みなら何もしない
	if (!force_change &&
		body_work->whole_act_id == act_id) {
		return;
	}
	
	// アクションID設定
	body_work->whole_act_id	= act_id;
	
	// 構成パーツのアクション設定を反映
	for (Sint32 i = 0; i < GME_BOSS5_PART_IDX_MAX; ++i) {
		
		if (body_work->parts_objs[i] == NULL) {
			continue;
		}
		
		// 継続フラグがオフのときのみ、新たなアクションを設定
		if (FALSE == pt_act_info[i].is_maintain) {
			GmBsCmnSetAction(body_work->parts_objs[i],
							 pt_act_info[i].act_id,
							 pt_act_info[i].is_repeat,
							 pt_act_info[i].is_blend);
		}
		else if (pt_act_info[i].is_repeat) {
			// リピートフラグは継続フラグの有無に関わらず反映
			GMM_BS_OBJ(body_work)->disp_flag	|= OBD_DISP_REPEAT;
		}
		
		
		// モーション速度設定
		body_work->parts_objs[i]->obj_3d->speed[0]	= pt_act_info[i].mtn_spd;
		
		// ブレンド速度設定
		body_work->parts_objs[i]->obj_3d->blend_spd	= pt_act_info[i].blend_spd;
	}
}


// =======================================================================
// gmBoss5BodySetDirection
/*!
  本体 方向設定
  
  @param body_work	[io]	本体ワーク
  @param dir_type	[in]	方向タイプ(GME_BOSS5_BODY_DIRECTION_TYPE_XXX)
  
  @note
  左右のフリップなどはOBD_DISP_HFLIPを直接設定せずに、この関数を使用すること。
 */
// =======================================================================
void gmBoss5BodySetDirection(GMS_BOSS5_BODY_WORK *body_work, GME_BOSS5_BODY_DIRECTION_TYPE dir_type)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	switch (dir_type) {
	case GME_BOSS5_BODY_DIRECTION_TYPE_LEFT:
		obj_work->dir.y	= (Uint16)AKM_DEGtoA16(-90);
		obj_work->disp_flag	|= OBD_DISP_HFLIP;
		break;
		
	case GME_BOSS5_BODY_DIRECTION_TYPE_RIGHT:
		obj_work->dir.y	= (Uint16)AKM_DEGtoA16(90);
		obj_work->disp_flag	&= ~OBD_DISP_HFLIP;
		break;
		
	case GME_BOSS5_BODY_DIRECTION_TYPE_NEAR:
		obj_work->dir.y	= 0;
		obj_work->disp_flag	&= ~OBD_DISP_HFLIP;
		break;
		
	default:
		MTM_ASSERT(0);
		obj_work->dir.y	= (Uint16)AKM_DEGtoA16(-90);
		obj_work->disp_flag	|= OBD_DISP_HFLIP;
	}
}

// =======================================================================
// gmBoss5BodyIsPlayerBehind
/*!
  プレイヤーが本体の後ろ側にいるか判定
  
  @param body_work	[in]	本体ワーク
  
  @retval TRUE	プレイヤーが本体の後ろにいる
  @retval FALSE	プレイヤーが本体の前にいる
  
  @note
  中心オブジェクトの座標を元に判定します。
 */
// =======================================================================
BOOL gmBoss5BodyIsPlayerBehind(const GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*ply_obj	= GmBsCmnGetPlayerObj();
	OBS_OBJECT_WORK	*body_obj	= GMM_BS_OBJ(body_work);
	OBS_OBJECT_WORK	*core_obj;
	
	MTM_ASSERT(body_work);
	MTM_ASSERT(body_work->part_obj_core);
	
	core_obj	= body_work->part_obj_core;
	
	if (body_obj->disp_flag & OBD_DISP_HFLIP) {
		if (core_obj->pos.x < ply_obj->pos.x) {
			return TRUE;
		}
	}
	else {
		if (ply_obj->pos.x < core_obj->pos.x) {
			return TRUE;
		}
	}
	
	return FALSE;
}

// =======================================================================
// gmBoss5BodySetNoHitTime
/*!
  ヒット無効時間設定
  
  @param body_work	[io]	本体ワーク
  
  @note
  連続ヒットを防ぐためのヒット無効時間を設定します。
  無効時間経過後にヒット有効化されるようにするには、
  gmBoss5BodyUpdateNoHitTime()を毎フレーム呼んでください。
 */
// =======================================================================
void gmBoss5BodySetNoHitTime(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_RECT_WORK* const rectpoint_array[GME_BOSS5_BODY_RECTPOINT_MAX]	= {
		&body_work->sub_rect_work[GME_BOSS5_BODY_RECTPOINT_LEFT_FOOT][0],
		&body_work->sub_rect_work[GME_BOSS5_BODY_RECTPOINT_RIGHT_FOOT][0],
		&((GMS_ENEMY_COM_WORK*)body_work)->rect_work[0],
	};
	
	body_work->no_hit_timer	= GMD_BOSS5_BODY_DMG_NO_HIT_TIME;
	
	// カウント中
	body_work->flag	|= GMD_BOSS5_BODY_FLAG_NO_HIT_TIME_COUNTING;
	
	for (Sint32 i = 0; i < GME_BOSS5_BODY_RECTPOINT_MAX; ++i) {
		rectpoint_array[i][GMD_ENEMY_RECT_DEF].flag	|= OBD_RECT_NOHIT;
		rectpoint_array[i][GMD_ENEMY_RECT_DEF].flag	&= ~OBD_RECT_ENABLE;
	}
}

// =======================================================================
// gmBoss5BodyUpdateNoHitTime
/*!
  ヒット無効時間更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  ヒット無効時間を更新して、タイマカウント完了時に喰らい矩形を復活させます。
  復活させたくない場合はこの関数を呼ばないでください。
 */
// =======================================================================
void gmBoss5BodyUpdateNoHitTime(GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->no_hit_timer) {
		body_work->no_hit_timer--;
	}
	else {
		OBS_RECT_WORK* const rectpoint_array[GME_BOSS5_BODY_RECTPOINT_MAX]	= {
			&body_work->sub_rect_work[GME_BOSS5_BODY_RECTPOINT_LEFT_FOOT][0],
			&body_work->sub_rect_work[GME_BOSS5_BODY_RECTPOINT_RIGHT_FOOT][0],
			&((GMS_ENEMY_COM_WORK*)body_work)->rect_work[0],
		};
		
		// カウント終了
		body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_NO_HIT_TIME_COUNTING;
		
		for (Sint32 rp_idx = 0; rp_idx < GME_BOSS5_BODY_RECTPOINT_MAX; ++rp_idx) {
			// カウンタ終了後の矩形有効・無効状態を設定
			if (body_work->def_rect_req_flag & (1 << rp_idx)) {
				rectpoint_array[rp_idx][GMD_ENEMY_RECT_DEF].flag	&= ~OBD_RECT_NOHIT;
				rectpoint_array[rp_idx][GMD_ENEMY_RECT_DEF].flag	|= OBD_RECT_ENABLE;
			}
			else {
				rectpoint_array[rp_idx][GMD_ENEMY_RECT_DEF].flag	|= OBD_RECT_NOHIT;
				rectpoint_array[rp_idx][GMD_ENEMY_RECT_DEF].flag	&= ~OBD_RECT_ENABLE;
			}
		}
	}
}

// =======================================================================
// gmBoss5BodySetPokeTriggerLimitTime
/*!
  腕突き出し発動期限タイマ設定
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodySetPokeTriggerLimitTime(GMS_BOSS5_BODY_WORK *body_work)
{
	body_work->poke_trg_limit_timer	= GMD_BOSS5_BODY_POKE_TRIGGER_LIMIT_TIME;
	
	// カウント中に設定
	MTM_ASSERT(!(body_work->flag & GMD_BOSS5_BODY_FLAG_POKE_TRG_TIME_LMT_COUNTING));
	body_work->flag	|= GMD_BOSS5_BODY_FLAG_POKE_TRG_TIME_LMT_COUNTING;
}

// =======================================================================
// gmBoss5BodyUpdatePokeTriggerLimitTime
/*!
  腕突き出し発動期限タイマ更新
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyUpdatePokeTriggerLimitTime(GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->poke_trg_limit_timer) {
		body_work->poke_trg_limit_timer--;
	}
	else {
		body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_POKE_TRG_TIME_LMT_COUNTING;
	}
}

// =======================================================================
// gmBossBodyIsWithinPokeTriggerLimitTime
/*!
  腕突き出し発動期限タイマ 期限内判定
  
  @param body_work	[in]	本体ワーク
  
  @retval TRUE	期限内
  @retval FALSE	期限切れ
 
  @note
  タイマカウント中かどうか判定します。
 */
// =======================================================================
BOOL gmBoss5BodyIsWithinPokeTriggerLimitTime(const GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->flag & GMD_BOSS5_BODY_FLAG_POKE_TRG_TIME_LMT_COUNTING) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss5BodyClearPokeTriggerLimitTime
/*!
  腕突き出し発動期限タイマ クリア
  
  @param body_work	[io]	本体ワーク
  
  @note
  タイマカウント中などにカウントを強制終了する場合に使用します。
 */
// =======================================================================
void gmBoss5BodyClearPokeTriggerLimitTime(GMS_BOSS5_BODY_WORK *body_work)
{
	body_work->poke_trg_limit_timer	= 0;
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_POKE_TRG_TIME_LMT_COUNTING;
}

// =======================================================================
// gmBoss5BodyExecDamageRoutine
/*!
  ダメージ時処理
  
  @param body_work	[io]	本体ワーク
  
  @note
  ダメージ時の諸々の共通処理を行います。（ライフ減少、ゲージ更新、SE再生など）
 */
// =======================================================================
void gmBoss5BodyExecDamageRoutine(GMS_BOSS5_BODY_WORK *body_work)
{
	GMS_BOSS5_MGR_WORK	*mgr_work	= (GMS_BOSS5_MGR_WORK*)body_work->mgr_work;
	
	MTM_ASSERT(mgr_work);
	
	// ライフ減少
	if (mgr_work->life) {
		mgr_work->life	-= 1;
#if defined(MTD_DEBUG)
		if (g_gm_main_system.debug_check_mode == GMD_MAIN_DEBUG_CHECK_MODE_BOSS_F_FINISH) {
			// 最後の一撃チェックモードのときは一発で最後の一撃に遷移
			mgr_work->life	-= GMD_BOSS5_LIFE;
		}
#endif /* defined(MTD_DEBUG) */
	}
	
	if (0 < mgr_work->life) {
		// ライフがある場合
		
		// ダメージ演出予約
		body_work->flag	|= GMD_BOSS5_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE;
		
		// 腕突き出し
		if (body_work->state == GME_BOSS5_BODY_STATE_MOVE_NML) {
			// 通常移動時のみ
			
			if (gmBoss5BodyIsWithinPokeTriggerLimitTime(body_work)) {
				if (!gmBoss5BodyIsPoking(body_work)) {
					// 発動期限タイマカウント中で既に腕突き出し動作中でなければ、
					// 発動リクエスト発行
					body_work->flag	|= GMD_BOSS5_BODY_FLAG_SIGNAL_B2B_POKE;
				}
			}
			else {
				// 発動期限タイマスタート
				gmBoss5BodySetPokeTriggerLimitTime(body_work);
			}
		}
		
		// バルカン開始試行
		gmBoss5BodyTryStartTurret(body_work);
	}
	else {
		// ライフが無い場合
		
		if (!(body_work->flag & GMD_BOSS5_BODY_FLAG_FINAL_MODE)) {
			// 最後の一撃状態でなければ死なない
			mgr_work->life	= 1;
		}
		else {
			// 死亡演出予約
			body_work->flag	|= GMD_BOSS5_BODY_FLAG_SIGNAL_B2B_DEFEAT;
			// これ以降ヒット処理しない
			GMM_BS_OBJ(body_work)->flag	|= OBD_OBJECT_NOHIT;
			
			// 0でクリップ
			mgr_work->life	= 0;
		}
	}
	
	// 凶暴化要求試行（実際に凶暴化設定されるのはシーケンス移行処理時）
	// （※ライフがある場合のみの試行だと、万が一、ライフが一気に0になった場合に
	//   凶暴化要求試行が行われなくなってしまうため、このタイミングで行う。）
	gmBoss5BodySeqTryRequestEnableStr(body_work);
}

// =======================================================================
// gmBoss5BodyUpdateMainRectPosition
/*!
  本体矩形位置更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  矩形の位置を中心オブジェクトの位置に合わせます。
 */
// =======================================================================
void gmBoss5BodyUpdateMainRectPosition(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_body	= GMM_BS_OBJ(body_work);
	OBS_OBJECT_WORK	*obj_core	= body_work->part_obj_core;
	fx32	pos_offset[MTD_XY];
	
	pos_offset[MTD_X]	= obj_core->pos.x - obj_body->pos.x;
	pos_offset[MTD_Y]	= obj_core->pos.y - obj_body->pos.y;
	
	for (Sint32 i = 0; i < GMD_ENEMY_RECT_NUM; ++i) {
		// 矩形の、本体オブジェクト中心からのオフセットを設定
		VEC_Set(&body_work->ene_3d.ene_com.rect_work[i].rect.pos,
				pos_offset[MTD_X],
				pos_offset[MTD_Y],
				0);	// Zはオフセットしない
	}
}

// =======================================================================
// gmBoss5BodyUpdateSubRectPosition
/*!
  追加矩形位置更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  追加矩形の位置を足などの位置に合わせます。
 */
// =======================================================================
void gmBoss5BodyUpdateSubRectPosition(GMS_BOSS5_BODY_WORK *body_work)
{
	const Sint32 snm_reg_id_tbl[GME_BOSS5_BODY_RECTPOINT_SUB_NUM]	= {
		body_work->leg_snm_reg_ids[GME_BOSS5_LEG_TYPE_LEFT],
		body_work->leg_snm_reg_ids[GME_BOSS5_LEG_TYPE_RIGHT],
	};
	fx32	pos_offset[MTD_XY];
	
	for (Sint32 sub_rp_idx = 0; sub_rp_idx < GME_BOSS5_BODY_RECTPOINT_SUB_NUM; ++sub_rp_idx) {
		const NNS_MATRIX	*w_mtx;
		
		w_mtx	= GmBsCmnGetSNMMtx(&body_work->snm_work, snm_reg_id_tbl[sub_rp_idx]);
		
		pos_offset[MTD_X]	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3)) - body_work->pivot_prev_pos.x;
		pos_offset[MTD_Y]	= -FX_F32_TO_FX32(NNM_MTX(*w_mtx, 1, 3)) - body_work->pivot_prev_pos.y;
		
		for (Sint32 i = 0; i < GMD_ENEMY_RECT_NUM; ++i) {
			// 指定ノード座標に配置されるように、矩形の親オブジェクト中心から指定ノードへのオフセット座標を設定
			VEC_Set(&body_work->sub_rect_work[sub_rp_idx][i].rect.pos,
					pos_offset[MTD_X],
					pos_offset[MTD_Y],
					0);	// Zはオフセットしない
		}
	}
}


// =======================================================================
// gmBoss5BodySetupRect
/*!
  本体矩形セットアップ
  
  @param body_work	[io]	本体ワーク
  
  @note
  本体の各矩形の登録・属性設定などの初期設定を行います。
 */
// =======================================================================
void gmBoss5BodySetupRect(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	OBS_RECT_WORK* const rectpoint_array[GME_BOSS5_BODY_RECTPOINT_MAX]	= {
		&body_work->sub_rect_work[GME_BOSS5_BODY_RECTPOINT_LEFT_FOOT][0],
		&body_work->sub_rect_work[GME_BOSS5_BODY_RECTPOINT_RIGHT_FOOT][0],
		&((GMS_ENEMY_COM_WORK*)body_work)->rect_work[0],
	};
	
	/* エネミーの食らい矩形と同じ設定にする */
	
	// 攻撃フラグ
	const u16		atk_flag[GMD_ENEMY_RECT_NUM] = {0,									// くらい
											GMD_OBJ_RECT_ATK_FLAG_NORMALATK,			// 通常攻撃
											GMD_OBJ_RECT_ATK_FLAG_BODYATK};				// 体あたり(存在判定)
	// 防御フラグ
	const u16		def_flag[GMD_ENEMY_RECT_NUM] = {GMD_OBJ_RECT_DEF_FLAG_WEAK_NORMALATK,	// くらい
											GMD_OBJ_RECT_DEF_FLAG_NORMALATK,				// 通常攻撃
											GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK};		// 体あたり(存在判定)
	
	// 独自管理分だけ設定する（GMS_ENEMY_COM_WORK::rect_workはエネミー初期化時の設定に任せる）
	for (Sint32 rectpoint = 0; rectpoint < GME_BOSS5_BODY_RECTPOINT_SUB_NUM; ++rectpoint) {
		for (Sint32 i = 0; i < GMD_ENEMY_RECT_NUM; ++i) {
			ObjRectGroupSet(&body_work->sub_rect_work[rectpoint][i], GMD_OBJ_RECT_GROUP_ENEMY, GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER);
			ObjRectAtkSet(&body_work->sub_rect_work[rectpoint][i], atk_flag[i], GMD_OBJ_RECT_ATK_POWER_DEFAULT);
			ObjRectDefSet(&body_work->sub_rect_work[rectpoint][i], def_flag[i], GMD_OBJ_RECT_DEF_POWER_DEFAULT);
			body_work->sub_rect_work[rectpoint][i].parent_obj = obj_work;
			body_work->sub_rect_work[rectpoint][i].flag &= ~OBD_RECT_ENABLE;	// はじめは無効
		}
		body_work->sub_rect_work[rectpoint][GMD_ENEMY_RECT_DEF].ppDef	= GmEnemyDefaultDefFunc;
		body_work->sub_rect_work[rectpoint][GMD_ENEMY_RECT_ATK].ppHit	= GmEnemyDefaultAtkFunc;
		body_work->sub_rect_work[rectpoint][GMD_ENEMY_RECT_BODY].flag |= OBD_RECT_GROUP | OBD_RECT_NODAMAGE | OBD_RECT_NOHIT_UP | OBD_RECT_NOAUTO_ENABLEOFF;
	}
	
	/* ボス５用設定 */
	
	// 食らい処理関数を差し替え（GMS_ENEMY_COM_WORK::rect_workの分も）
	for (Sint32 i = 0; i < GME_BOSS5_BODY_RECTPOINT_MAX; ++i) {
		rectpoint_array[i][GMD_ENEMY_RECT_DEF].ppDef	= gmBoss5BodyDamageDefFunc;
	}
	
	// 矩形設定初期化
	gmBoss5BodyChangeRectSetting(body_work, GME_BOSS5_BODY_RECT_SETTING_DEFAULT_NML);
}

// =======================================================================
// gmBoss5BodyChangeRectSetting
/*!
  当たり矩形設定変更
  
  @param body_work		[io]	本体ワーク
  @param rect_setting	[in]	矩形設定タイプ
  
  @note
  設定タイプに応じて、本体の各矩形をまとめて設定します。
  基本的には矩形設定はこの関数で行いますが、
  シーケンスの途中で一部の設定を書き換える必要があるため（走りの時など）、
  この関数呼び出しによって設定された内容が次の呼び出しまで同じであると
  想定した処理は行わないでください。
  食らい矩形のOBD_RECT_NOHITフラグ設定を行う場合は、
  GMS_BOSS5_BODY_WORK::def_rect_req_flagの設定も必ず行ってください。
 */
// =======================================================================
void gmBoss5BodyChangeRectSetting(GMS_BOSS5_BODY_WORK *body_work, GME_BOSS5_BODY_RECT_SETTING rect_setting)
{
	OBS_RECT_WORK* const rectpoint_array[GME_BOSS5_BODY_RECTPOINT_MAX]	= {
		&body_work->sub_rect_work[GME_BOSS5_BODY_RECTPOINT_LEFT_FOOT][0],
		&body_work->sub_rect_work[GME_BOSS5_BODY_RECTPOINT_RIGHT_FOOT][0],
		&((GMS_ENEMY_COM_WORK*)body_work)->rect_work[0],
	};
	
	const GMS_BOSS5_BODY_RECT_SETTING_INFO	*setting_info	= &gm_boss5_body_rect_setting_info_tbl[rect_setting];
	
	// 無敵設定
	if (setting_info->is_invincible) {
		body_work->flag	|= GMD_BOSS5_BODY_FLAG_INVINCIBLE;
	}
	else {
		body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_INVINCIBLE;
	}
	
	// 漏電エフェクト設定
	if (setting_info->is_leakage) {
		body_work->flag	|= GMD_BOSS5_BODY_FLAG_LEAKAGE_NEEDED;
	}
	else {
		body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_LEAKAGE_NEEDED;
	}
	
	// 矩形パラメータ設定
	for (Sint32 rp_idx = 0; rp_idx < GME_BOSS5_BODY_RECTPOINT_MAX; ++rp_idx) {
		const GMS_BOSS5_RECTPOINT_SETTING_INFO	*rp_setting	= &setting_info->point_setting_info[rp_idx];
		
		for (Sint32 rect_type = 0; rect_type < GMD_ENEMY_RECT_NUM; ++rect_type) {
			
			// サイズ設定
			ObjRectWorkSet(&rectpoint_array[rp_idx][rect_type],
						   rp_setting->rect_size[rect_type][MTD_LEFT],
						   rp_setting->rect_size[rect_type][MTD_TOP],
						   rp_setting->rect_size[rect_type][MTD_RIGHT],
						   rp_setting->rect_size[rect_type][MTD_BOTTOM]);
			
			// 有効・無効設定
			if ((1 << rect_type) & rp_setting->enable_bit_flag) {
				// 有効化
				
				if (rect_type == GMD_ENEMY_RECT_DEF) {
					// 食らい矩形の場合
					
					// ヒット無効カウント終了後は食らい矩形オン
					body_work->def_rect_req_flag	|= (1 << rp_idx);
					
					// ヒット無効カウント中は食らい矩形を有効にしない
					if (!(body_work->flag & GMD_BOSS5_BODY_FLAG_NO_HIT_TIME_COUNTING)) {
						rectpoint_array[rp_idx][rect_type].flag	&= ~OBD_RECT_NOHIT;
						rectpoint_array[rp_idx][rect_type].flag	|= OBD_RECT_ENABLE;
					}
				}
				else {
					// 食らい矩形でない場合は無条件で矩形を有効化
					rectpoint_array[rp_idx][rect_type].flag	&= ~OBD_RECT_NOHIT;
					rectpoint_array[rp_idx][rect_type].flag	|= OBD_RECT_ENABLE;
				}
			}
			else {
				// 無効化
				
				rectpoint_array[rp_idx][rect_type].flag	|= OBD_RECT_NOHIT;
				rectpoint_array[rp_idx][rect_type].flag	&= ~OBD_RECT_ENABLE;
				
				if (rect_type == GMD_ENEMY_RECT_DEF) {
					// ヒット無効カウント終了後は食らい矩形オフ
					// （勝手にオンになってしまわないように抑制）
					body_work->def_rect_req_flag	&= ~(1 << rp_idx);
				}
			}
		}
	}
}

// =======================================================================
// gmBoss5BodyChangeRectSettingDefault
/*!
  当たり矩形設定 デフォルトに設定
  
  @param body_work	[io]	本体ワーク
  
  @note
  現在の状態に応じて適切なデフォルト矩形設定に変更します。
 */
// =======================================================================
void gmBoss5BodyChangeRectSettingDefault(GMS_BOSS5_BODY_WORK *body_work)
{
	if (gmBoss5BodySeqIsStr(body_work)) {
		gmBoss5BodyChangeRectSetting(body_work, GME_BOSS5_BODY_RECT_SETTING_DEFAULT_STR);
	}
	else {
		gmBoss5BodyChangeRectSetting(body_work, GME_BOSS5_BODY_RECT_SETTING_DEFAULT_NML);
	}
}

// =======================================================================
// gmBoss5BodySwitchEnableLegRectOneSide
/*!
  当たり矩形 脚矩形 片脚有効切り替え
  
  @param body_work	[io]	本体ワーク
  @param leg_type	[in]	脚タイプ
  
  @note
  指定した脚の攻撃矩形を有効にし、もう片方の脚の攻撃矩形を無効に設定します。
  元の設定に戻したい場合はgmBoss5BodyChangeRectSetting***()を使用してください。
 */
// =======================================================================
void gmBoss5BodySwitchEnableLegRectOneSide(GMS_BOSS5_BODY_WORK *body_work, GME_BOSS5_LEG_TYPE leg_type)
{
	GME_BOSS5_BODY_RECTPOINT	targ_leg;
	GME_BOSS5_BODY_RECTPOINT	other_leg;
	
	if (leg_type == GME_BOSS5_LEG_TYPE_LEFT) {
		targ_leg	= GME_BOSS5_BODY_RECTPOINT_LEFT_FOOT;
		other_leg	= GME_BOSS5_BODY_RECTPOINT_RIGHT_FOOT;
	}
	else {
		MTM_ASSERT(leg_type == GME_BOSS5_LEG_TYPE_RIGHT);
		targ_leg	= GME_BOSS5_BODY_RECTPOINT_RIGHT_FOOT;
		other_leg	= GME_BOSS5_BODY_RECTPOINT_LEFT_FOOT;
	}
	
	// 指定の脚矩形を有効化
	body_work->sub_rect_work[targ_leg][GMD_ENEMY_RECT_ATK].flag	&= ~OBD_RECT_NOHIT;
	body_work->sub_rect_work[targ_leg][GMD_ENEMY_RECT_ATK].flag	|= OBD_RECT_ENABLE;
	
	// 他方の脚矩形を無効化
	body_work->sub_rect_work[other_leg][GMD_ENEMY_RECT_ATK].flag	|= OBD_RECT_NOHIT;
	body_work->sub_rect_work[other_leg][GMD_ENEMY_RECT_ATK].flag	&= ~OBD_RECT_ENABLE;
}


// =======================================================================
// gmBoss5BodyTryImmobilizePlayer
/*!
  プレイヤー不動化試行
  
  @param body_work	[io]	本体ワーク
  
  @note
  プレイヤーの移動・操作を既定時間停止します。
 */
// =======================================================================
void gmBoss5BodyTryImmobilizePlayer(GMS_BOSS5_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
	
	// プレイヤーが接地していたら地球割り着地振動ギミックシーケンスに移行させる
	if (GmBsCmnGetPlayerObj()->move_flag & OBD_MOVE_UNDER) {
		GmPlySeqGmkInitBoss5Quake((GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj(),
								  GMD_BOSS5_BODY_CRASH_PLY_IMMOBILE_TIME);
	}
}

// =======================================================================
// gmBoss5BodyAllocSeHandles
/*!
  本体用SEハンドル確保
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyAllocSeHandles(GMS_BOSS5_BODY_WORK *body_work)
{
	body_work->se_hnd_leakage	= GsSoundAllocSeHandle();
}

// =======================================================================
// gmBoss5BodyFreeSeHandles
/*!
  本体用SEハンドル解放
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyFreeSeHandles(GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->se_hnd_leakage) {
		GsSoundFreeSeHandle(body_work->se_hnd_leakage);
		body_work->se_hnd_leakage	= NULL;
	}
}

// =======================================================================
// gmBoss5BodyInitPlayTargetSe
/*!
  ターゲットSE再生処理 初期化
  
  @param body_work		[io]	本体ワーク
  @param 	[in]	再生間隔を早めていく時間
 */
// =======================================================================
void gmBoss5BodyInitPlayTargetSe(GMS_BOSS5_BODY_WORK *body_work, Float init_interval)
{
	// 再生間隔初期化
	body_work->targ_se_cur_interval	= init_interval;
	
	// 1ショットタイマ設定
	GmBoss5Init1ShotTimer(&body_work->targ_se_timer,
						  (Uint32)body_work->targ_se_cur_interval);
	// 初回再生
	GmSoundPlaySE("FinalBoss05");
}

// =======================================================================
// gmBoss5BodyUpdatePlayTargetSe
/*!
  ターゲットSE再生処理 更新
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyUpdatePlayTargetSe(GMS_BOSS5_BODY_WORK *body_work)
{
	// 再生間隔更新
	if (body_work->targ_se_cur_interval >= 0.f) {
		body_work->targ_se_cur_interval	-= GMD_BOSS5_BODY_SE_TARGET_INTERVAL_DEC_SPD;
	}
	else {
		body_work->targ_se_cur_interval	= 0.f;
	}
	
	if (GmBoss5Update1ShotTimer(&body_work->targ_se_timer)) {
		Uint32	interval	= (Uint32)body_work->targ_se_cur_interval;
		
		// 既定値よりも短い間隔で再生しない
		if (interval <= GMD_BOSS5_BODY_SE_TARGET_INTERVAL_MIN) {
			interval	= GMD_BOSS5_BODY_SE_TARGET_INTERVAL_MIN;
		}
		
		GmSoundPlaySE("FinalBoss05");
		
		// タイマ再設定
		GmBoss5Init1ShotTimer(&body_work->targ_se_timer, interval);
	}
}

// =======================================================================
// gmBoss5BodyForceEndLeakage
/*!
  漏電エフェクト強制終了
  
  @param body_work	[io]	本体ワーク
  
  @note
  gmBoss5BodyChangeRectSetting()に依らず、強制的に漏電エフェクトを終了します。
  消失エフェクトは生成しませんが、パーティクルは自然に消えます(ENDフラグを待ちます)。
 */
// =======================================================================
void gmBoss5BodyForceEndLeakage(GMS_BOSS5_BODY_WORK *body_work)
{
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_LEAKAGE_NEEDED;
	
	GmBoss5EfctEndLeakage(body_work, TRUE);
}

// =======================================================================
// gmBoss5BodyInitPlySearch
/*!
  プレイヤーサーチ処理 初期化
 
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyInitPlySearch(GMS_BOSS5_BODY_WORK *body_work, Sint32 delay)
{
	MTM_ASSERT(delay < GMD_BOSS5_BODY_PLY_SEARCH_HIST_NUM);
	
	// サーチ遅延フレーム設定
	body_work->ply_search_delay	= delay;
	
	// 遅延サーチ初期化
	GmBsCmnInitDelaySearch(&body_work->dsearch_work,
						   GmBsCmnGetPlayerObj(),
						   body_work->search_hist_buf,
						   GMD_BOSS5_BODY_PLY_SEARCH_HIST_NUM);
}

// =======================================================================
// gmBoss5BodyUpdatePlySearch
/*!
  プレイヤーサーチ処理 更新
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyUpdatePlySearch(GMS_BOSS5_BODY_WORK *body_work)
{
	// 遅延サーチ更新
	GmBsCmnUpdateDelaySearch(&body_work->dsearch_work);
}

// =======================================================================
// gmBoss5BodySetPlyRebound
/*!
  プレイヤー跳ね返り設定
  
  @param ply_work	[io]	プレイヤーワーク
  @param body_work	[in]	本体ワーク
  
  @note
  プレイヤーの攻撃が本体にヒットした際のプレイヤー跳ね返り移動の各種パラメータを設定します。
 */
// =======================================================================
void gmBoss5BodySetPlyRebound(GMS_PLAYER_WORK *ply_work, const GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_ply	= (OBS_OBJECT_WORK*)ply_work;
	
	// プレイヤー跳ね返り
	GmPlySeqAtkReactionInit(ply_work);
	
	if (ply_work->seq_state == GME_PLY_SEQ_STATE_HOMING_REF) {
		// ジャンプステート設定（ホーミング跳ね返り時のみホーミングアタック禁止）
		GmPlySeqSetJumpState(ply_work, 0,
							 (GMD_PLY_SEQ_SETJUMPSTATE_IGNORE_JUMPBTN |
							  GMD_PLY_SEQ_SETJUMPSTATE_NOHOMING));
		
		// 水平方向速度設定
		ply_work->obj_work.spd_m = 0;
		if (ply_work->obj_work.move.x >= 0){
			ply_work->obj_work.spd.x = -GMD_BOSS5_BODY_PLY_HOMING_REBOUND_X;
			
		}
		else {
			ply_work->obj_work.spd.x = GMD_BOSS5_BODY_PLY_HOMING_REBOUND_X;
		}
		
		
		// 垂直方向速度設定
		if (obj_ply->pos.y <= body_work->part_obj_core->pos.y) {
			// 上へ弾く
			ply_work->obj_work.spd.y	= GMD_BOSS5_BODY_PLY_HOMING_REBOUND_Y;
		}
		else {
			// 下へ弾く
			ply_work->obj_work.spd.y	= -GMD_BOSS5_BODY_PLY_HOMING_REBOUND_Y;
		}
		
		// ジャンプ中移動禁止時間設定
		GmPlySeqSetNoJumpMoveTime(ply_work, GMD_BOSS5_BODY_PLY_HOMING_REBOUND_NOJUMPMOVE_TIME);
	}
	else {
		// 水平方向速度設定
		ply_work->obj_work.spd_m = 0;
		if ( ply_work->obj_work.move.x >= 0 ){
			ply_work->obj_work.spd.x = -GMD_BOSS5_BODY_PLY_NML_REBOUND_X;
			
		}
		else {
			ply_work->obj_work.spd.x = GMD_BOSS5_BODY_PLY_NML_REBOUND_X;
		}
		
		
		// 垂直方向速度設定
		if (obj_ply->pos.y <= body_work->part_obj_core->pos.y) {
			// 上へ弾く
			ply_work->obj_work.spd.y	= GMD_BOSS5_BODY_PLY_NML_REBOUND_Y;
		}
		else {
			// 下へ弾く
			ply_work->obj_work.spd.y	= -GMD_BOSS5_BODY_PLY_NML_REBOUND_Y;
		}
		
		// ジャンプ中移動禁止時間設定
		GmPlySeqSetNoJumpMoveTime(ply_work, GMD_BOSS5_BODY_PLY_NML_REBOUND_NOJUMPMOVE_TIME);
		
		// ヒット無効時間分だけホーミングを禁止する
		// （食らい矩形が無効の時にホーミングで突き抜けないようにするため）
		ply_work->homing_timer	= (fx32)(GMD_BOSS5_BODY_DMG_NO_HIT_TIME * FX32_ONE);
	}
}

// =======================================================================
// gmBoss5BodySetMoveFastTime
/*!
  高速移動継続時間 設定
  
  @param body_work		[io]	本体ワーク
  @param fast_move_time	[in]	高速移動継続時間
 */
// =======================================================================
inline void gmBoss5BodySetMoveFastTime(GMS_BOSS5_BODY_WORK *body_work, Uint32 fast_move_time)
{
	body_work->fast_move_timer	= fast_move_time;
}

// =======================================================================
// gmBoss5BodyUpdateMoveFastTime
/*!
  高速移動継続時間 更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  タイマカウント完了判定はgmBoss5BodyIsMoveFastEnd()で行ってください。
 */
// =======================================================================
inline void gmBoss5BodyUpdateMoveFastTime(GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->fast_move_timer) {
		body_work->fast_move_timer--;
	}
}

// =======================================================================
// gmBoss5BodyIsMoveFastEnd
/*!
  高速移動継続時間 タイマ完了チェック
  
  @param body_work	[in]	本体ワーク
  
  @retval TRUE	タイマカウント完了
  @retval FALSE	タイマカウント中
 */
// =======================================================================
inline BOOL gmBoss5BodyIsMoveFastEnd(const GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->fast_move_timer) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}

// =======================================================================
// gmBoss5BodyGetStompFallPosX
/*!
  ストンプ落下目標地点取得
  
  @param body_work		[in]	本体ワーク
  @param search_pos_x	[in]	サーチ結果位置X
  
  @note
  サーチ結果位置から、左右の壁にめり込まないように調整が行われます。
 */
// =======================================================================
fx32 gmBoss5BodyGetStompFallPosX(const GMS_BOSS5_BODY_WORK *body_work, fx32 search_pos_x)
{
	UNREFERENCED_PARAMETER(body_work);
	fx32	left_edge_pos;
	fx32	right_edge_pos;
	fx32	overwrap	= 0;
	
	left_edge_pos	= search_pos_x - GMD_BOSS5_BODY_STOMP_FALL_POS_MARGIN;
	right_edge_pos	= search_pos_x + GMD_BOSS5_BODY_STOMP_FALL_POS_MARGIN;
	
	// 既定距離だけ壁から離れているかチェック
	if (left_edge_pos <= GMM_BOSS5_AREA_LEFT()) {
		// 左にめり込んでいたら重なっている分の幅を取得
		overwrap	= GMM_BOSS5_AREA_LEFT() - left_edge_pos;
	}
	else if (right_edge_pos >= GMM_BOSS5_AREA_RIGHT()) {
		// 右にめり込んでいたら重なっている分の幅を取得
		overwrap	= GMM_BOSS5_AREA_RIGHT() - right_edge_pos;
	}
	
	// 既定幅壁から離れるようにめり込み幅だけずらした座標を返す
	return (search_pos_x + overwrap);
}

// =======================================================================
// gmBoss5BodyDecideCrashFallPosX
/*!
  地球割り落下目標地点決定
  
  @param body_work	[io]	本体ワーク
  
  @note
  タイムカウント秒の1の位の値によって落下地点を決定。
  この関数呼び出しによって決定した落下地点は、
  gmBoss5BodyGetCrashFallPosX() で取得してください。
 */
// =======================================================================
void gmBoss5BodyDecideCrashFallPosX(GMS_BOSS5_BODY_WORK *body_work)
{
	Uint16	sec;
	Uint32	game_time;
	
	if (g_gm_main_system.game_time >= GMD_MAIN_TIME_MAX) {
		game_time	= GMD_MAIN_TIME_MAX;
	}
	else {
		game_time	= g_gm_main_system.game_time;
	}
	
	// ゲームタイムフレームの秒のみ取得
	AkUtilFrame60ToTime(game_time, NULL, &sec, NULL);
	
	// 1の位だけ取り出す
	sec	=	(Uint16)((Sint32)game_time % 10);
	
	switch (sec) {
	case 4:	// no break
	case 6:	// no break
	case 8:
		// 偶数
		body_work->crash_pos_ofst_x	= GMD_BOSS5_BODY_CRASH_POINT_A_POS_OFST_X;
		break;
		
	case 3:	// no break
	case 5:	// no break
	case 7:	// no break
	case 9:
		// 奇数
		body_work->crash_pos_ofst_x	= GMD_BOSS5_BODY_CRASH_POINT_C_POS_OFST_X;
		break;
		
	default:
		MTM_ASSERT(FALSE);
		// no break
	case 0:	// no break
	case 1:	// no break
	case 2:
		// 0,1,2
		body_work->crash_pos_ofst_x	= GMD_BOSS5_BODY_CRASH_POINT_B_POS_OFST_X;
		break;
	}
}

// =======================================================================
// gmBoss5BodyGetCrashFallPosX
/*!
  地球割り落下目標地点取得
  
  @param body_work	[in]	本体ワーク
  
  @return 落下目標地点X座標
 */
// =======================================================================
fx32 gmBoss5BodyGetCrashFallPosX(const GMS_BOSS5_BODY_WORK *body_work)
{
#if defined(MTD_DEBUG)
	if (g_gm_main_system.debug_check_mode == GMD_MAIN_DEBUG_CHECK_MODE_BOSS_F_FINISH) {
		// 最後の一撃確認モードの時はプレイヤーの位置に落下
		return GmBsCmnGetPlayerObj()->pos.x;
	}
#endif /* defined(MTD_DEBUG) */
	
	return GMM_BOSS5_AREA_CENTER_X() + body_work->crash_pos_ofst_x;
}

// =======================================================================
// gmBoss5BodyCheckJetSmokeClearTiming
/*!
  噴射スモーク消去タイミングチェック
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
BOOL gmBoss5BodyCheckJetSmokeClearTiming(const GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_body	= GMM_BS_OBJ(body_work);
	
	if (obj_body->pos.y <= (body_work->ground_v_pos - GMD_BOSS5_BODY_JETSMOKE_CLEAR_HEIGHT)) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss5BodyIsMoveFastDirFwd
/*!
  高速移動の方向が前方かチェック
  
  @param body_work	[in]	本体ワーク
  
  @retval TRUE	高速前進移動中
  @retval FALSE	高速後退移動中
 
  @note
  高速移動ステート時以外の時に使用するとアサートします。
 */
// =======================================================================
BOOL gmBoss5BodyIsMoveFastDirFwd(const GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->state == GME_BOSS5_BODY_STATE_MOVE_FAST_FWD) {
		return TRUE;
	}
	else if (body_work->state == GME_BOSS5_BODY_STATE_MOVE_FAST_BWD) {
		return FALSE;
	}
	
	// 高速移動ステート時以外の使用は想定外
	MTM_ASSERT(FALSE);
	
	return TRUE;
}

// =======================================================================
// gmBoss5BodyIsBodyExplosionStopAllowed
/*!
  本体爆発処理を終了してよいかチェック
  
  @param body_work	[in]	本体ワーク
 */
// =======================================================================
BOOL gmBoss5BodyIsBodyExplosionStopAllowed(const GMS_BOSS5_BODY_WORK *body_work)
{
	BOOL	is_out;
	
	// 小爆発生成範囲矩形が画面外に消えたら、本体爆発処理の終了を許可する
	is_out	= ObjViewOutCheck(body_work->part_obj_core->pos.x,
							  body_work->ground_v_pos,	// ←本体は下に落ちていくかもしれないので、Yは地面の高さを使用
							  0,
							  -((GMD_BOSS5_EXPL_BODY_OFST_X + (GMD_BOSS5_EXPL_BODY_WIDTH / 2)) >> FX32_SHIFT), //left
							  -((GMD_BOSS5_EXPL_BODY_OFST_Y + (GMD_BOSS5_EXPL_BODY_HEIGHT / 2)) >> FX32_SHIFT), //top
							  -((GMD_BOSS5_EXPL_BODY_OFST_X - (GMD_BOSS5_EXPL_BODY_WIDTH / 2)) >> FX32_SHIFT), //right
							  -((GMD_BOSS5_EXPL_BODY_OFST_Y - (GMD_BOSS5_EXPL_BODY_HEIGHT / 2)) >> FX32_SHIFT)); //bottom
	
	if (GmBsCmnGetPlayerObj()->pos.x < body_work->part_obj_core->pos.x) {
		is_out	= FALSE;
	}
	
	// 小爆発範囲が画面外、かつ、プレイヤーがボスより右側なら
	// 小爆発範囲が画面左の外側に消えたとみなし、TRUE
	if (is_out) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss5BodyGetStompFallDirectionType
/*!
  ストンプ落下時の向きを取得
  
  @param body_work	[in]	本体ワーク
  
  @return 方向タイプ(GME_BOSS5_BODY_DIRECTION_TYPE_XXX)
 */
// =======================================================================
GME_BOSS5_BODY_DIRECTION_TYPE gmBoss5BodyGetStompFallDirectionType(const GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 壁に背を向ける判定距離は歩行を中止する判定距離よりも長くなければならない
	MTM_ASSERT(GMD_BOSS5_BODY_STOMP_WALL_BEHIND_WALL_DISTANCE >= GMD_BOSS5_BODY_WALK_WALK_END_WALL_DISTANCE);
	
	// 壁から既定距離以内なら壁に背を向ける
	if (obj_work->pos.x <= GMM_BOSS5_AREA_LEFT() + GMD_BOSS5_BODY_STOMP_WALL_BEHIND_WALL_DISTANCE) {
		return GME_BOSS5_BODY_DIRECTION_TYPE_RIGHT;
	}
	else if (obj_work->pos.x >= GMM_BOSS5_AREA_RIGHT() - GMD_BOSS5_BODY_STOMP_WALL_BEHIND_WALL_DISTANCE) {
		return GME_BOSS5_BODY_DIRECTION_TYPE_LEFT;
	}
	
	// 壁から既定距離より離れている場合
	
	// プレイヤーの方を向く
	if (obj_work->pos.x < GmBsCmnGetPlayerObj()->pos.x) {
		return GME_BOSS5_BODY_DIRECTION_TYPE_RIGHT;
	}
	else if (obj_work->pos.x > GmBsCmnGetPlayerObj()->pos.x) {
		return GME_BOSS5_BODY_DIRECTION_TYPE_LEFT;
	}
	else {
		if (GmBsCmnGetPlayerObj()->disp_flag & OBD_DISP_HFLIP) {
			return GME_BOSS5_BODY_DIRECTION_TYPE_LEFT;
		}
		else {
			return GME_BOSS5_BODY_DIRECTION_TYPE_RIGHT;
		}
	}
}

// =======================================================================
// gmBoss5BodyTryStartTurret
/*!
  バルカン砲塔開始試行
  
  @param body_work	[io]	本体ワーク
  
  @note
  バルカン砲塔を生成・シーケンスを開始します。
  既に生成されている場合は何もしません。
 */
// =======================================================================
void gmBoss5BodyTryStartTurret(GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->mgr_work->life <= GMD_BOSS5_TURRET_START_LIFE_THRESHOLD) {
		if (!(body_work->flag & GMD_BOSS5_BODY_FLAG_TURRET_ACTIVE)) {
			body_work->flag	|= GMD_BOSS5_BODY_FLAG_TURRET_ACTIVE;
			
			GmBoss5TurretStartUp(body_work);
		}
	}
}

// =======================================================================
// gmBoss5BodyRecordGapAdjustmentDest
/*!
  モーションブレンド開始・終了フレームの座標ズレ調整用に
  ブレンド「後」モーション姿勢情報を記録
  
  @param body_work	[io]	本体ワーク
  
  @note
  FWなど、前フレームとモーションが変わらない時、
  かつオブジェクトの座標が変わらない時に呼び出してください。
  （FWモーションでの左右足ノードの位置を記録しておき、
    FWモーションに変更する際の座標補正に使用）
 */
// =======================================================================
void gmBoss5BodyRecordGapAdjustmentDest(GMS_BOSS5_BODY_WORK *body_work)
{
	const NNS_MATRIX	*w_mtx_ref;
	fx32	pivot_x;
	fx32	ref_x;
	
	Sint32	foot_snm_reg_ids[GME_BOSS5_LEG_TYPE_MAX]	= {
		body_work->lfoot_snm_reg_id,
		body_work->rfoot_snm_reg_id,
	};
	
	for (Sint32 i = 0; i < GME_BOSS5_LEG_TYPE_MAX; ++i) {
		// ワールドマトリクス取得
		w_mtx_ref	= GmBsCmnGetSNMMtx(&body_work->snm_work,
									   foot_snm_reg_ids[i]);
		
		// 各ノードの水平座標を取得
		pivot_x	= body_work->grdmv_pivot_pos.x;	// pivot_prev_pos でも良いかも
		ref_x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx_ref, 0, 3));
		
		body_work->foot_ofst_record_dest[i]	= ref_x - pivot_x;
		
		// 右向き基準に直す
		if (GMM_BS_OBJ(body_work)->disp_flag & OBD_DISP_HFLIP) {
			body_work->foot_ofst_record_dest[i]	= -body_work->foot_ofst_record_dest[i];
		}
	}
}

// =======================================================================
// gmBoss5BodyRecordGapAdjustmentSrc
/*!
  モーションブレンド開始・終了フレームの座標ズレ調整用に
  ブレンド「元」モーション姿勢情報を記録
  
  @param body_work	[io]	本体ワーク
  
  @note
  FWなど、前フレームとモーションが変わらない時、
  かつオブジェクトの座標が変わらない時に呼び出してください。
  （FWモーションでの左右足ノードの位置を記録しておき、
    FWモーションに変更する際の座標補正に使用）
 */
// =======================================================================
void gmBoss5BodyRecordGapAdjustmentSrc(GMS_BOSS5_BODY_WORK *body_work)
{
	const NNS_MATRIX	*w_mtx_ref;
	fx32	pivot_x;
	fx32	ref_x;
	
	Sint32	foot_snm_reg_ids[GME_BOSS5_LEG_TYPE_MAX]	= {
		body_work->lfoot_snm_reg_id,
		body_work->rfoot_snm_reg_id,
	};
	
	for (Sint32 i = 0; i < GME_BOSS5_LEG_TYPE_MAX; ++i) {
		// ワールドマトリクス取得
		w_mtx_ref	= GmBsCmnGetSNMMtx(&body_work->snm_work,
									   foot_snm_reg_ids[i]);
		
		// 各ノードの水平座標を取得
		pivot_x	= body_work->grdmv_pivot_pos.x;	// pivot_prev_pos でも良いかも
		ref_x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx_ref, 0, 3));
		
		body_work->foot_ofst_record_src[i]	= ref_x - pivot_x;
		
		// 右向き基準に直す
		if (GMM_BS_OBJ(body_work)->disp_flag & OBD_DISP_HFLIP) {
			body_work->foot_ofst_record_src[i]	= -body_work->foot_ofst_record_src[i];
		}
	}
}

// =======================================================================
// gmBoss5BodyInitAdjustMtnBlendHGap
/*!
  モーションブレンドによる水平座標ズレを補正 初期化
  
  @param body_work		[io]	本体ワーク
  @param dest_act_id	[in]	目標モーションのアクションID
  @param leg_type		[in]	地面に固定させる足の脚タイプ
  
  @note
  目標モーションが設定された状態でgmBoss5BodyRecordGapAdjustmentDest()を呼び出し、
  モーション変更直後（同じフレーム）にgmBoss5BodyRecordGapAdjustmentSrc()
  を呼び出しておく必要があります。
  補正処理が不要になったら必ずgmBoss5BodyClearAdjustMtnBlendHGap()を呼び出してください。
  （ロケットパンチの腕接続処理における座標設定に影響します。）
 */
// =======================================================================
void gmBoss5BodyInitAdjustMtnBlendHGap(GMS_BOSS5_BODY_WORK *body_work,
									   GME_BOSS5_ACT_ID dest_act_id,
									   GME_BOSS5_LEG_TYPE leg_type)
{
	body_work->adj_hgap_is_active	= TRUE;
	body_work->adj_hgap_act_id	= dest_act_id;
	body_work->adj_hgap_leg_type	= leg_type;
}

// =======================================================================
// gmBoss5BodyUpdateAdjustMtnBlendHGap
/*!
  モーションブレンドによる水平座標ズレを補正 更新
  
  @param body_work		[io]	本体ワーク
  
  @note
  アクション変更したフレームでも呼び出してください。
 */
// =======================================================================
void gmBoss5BodyUpdateAdjustMtnBlendHGap(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	fx32	diff;
	
	if (!body_work->adj_hgap_is_active) {
		return;
	}
	
	// 元モーションと目標モーションのノード座標差分を取得
	diff	=
		body_work->foot_ofst_record_dest[body_work->adj_hgap_leg_type] -
			body_work->foot_ofst_record_src[body_work->adj_hgap_leg_type];
	
	// 1フレーム分のズレを取得
	diff	= (fx32)(diff * gm_boss5_act_id_tbl[body_work->adj_hgap_act_id][GME_BOSS5_PART_IDX_BODY].blend_spd);
	
	// 座標に反映
	if (GMM_BS_OBJ(body_work)->disp_flag & OBD_DISP_HFLIP) {
		obj_work->pos.x += diff;
	}
	else {
		obj_work->pos.x -= diff;
	}
}

// =======================================================================
// gmBoss5BodyClearAdjustMtnBlendHGap
/*!
  モーションブレンドによる水平座標ズレ補正 クリア
  
  @param body_work	[io]	本体ワーク
  
  @note
  この関数を呼んだ後はgmBoss5BodyUpdateAdjustMtnBlendHGap()を実行しても
  何も行われません。
 */
// =======================================================================
void gmBoss5BodyClearAdjustMtnBlendHGap(GMS_BOSS5_BODY_WORK *body_work)
{
	body_work->adj_hgap_is_active	= FALSE;
}


// =======================================================================
// gmBoss5BodyInitGroundingMove
/*!
  足接地移動処理 初期化
  
  @param body_work			[io]	本体ワーク
  @param ref_snm_reg_id		[in]	参照点ノードのSNM登録ID
  
  @note
  指定した参照点ノードの水平座標が移動しないように
  本体の座標を更新する処理を初期化します。
 */
// =======================================================================
void gmBoss5BodyInitGroundingMove(GMS_BOSS5_BODY_WORK *body_work, Sint32 ref_snm_reg_id)
{
	GMS_BOSS5_GRD_MOVE_WORK	*grdmv_work	= (GMS_BOSS5_GRD_MOVE_WORK*)&body_work->grdmv_work;
	
	grdmv_work->cur_diff_x	= 0;
	grdmv_work->prev_diff_x	= 0;
	
	grdmv_work->ref_snm_reg_id	= ref_snm_reg_id;
	
	grdmv_work->is_first_updated	= FALSE;
}

// =======================================================================
// gmBoss5BodyUpdateGroundingMove
/*!
  足接地移動処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  移動値を算出するのに1フレーム前のノード座標を使用しているため、
  モーションの特定のフレーム分までの移動値を漏れなく反映させたい場合は、
  そのフレームに到達した次のフレームもこの関数を呼ぶ必要があります。
 */
// =======================================================================
void gmBoss5BodyUpdateGroundingMove(GMS_BOSS5_BODY_WORK *body_work)
{
	GMS_BOSS5_GRD_MOVE_WORK	*grdmv_work	= (GMS_BOSS5_GRD_MOVE_WORK*)&body_work->grdmv_work;
	const NNS_MATRIX	*w_mtx_ref;
	fx32	pivot_x;
	fx32	ref_x;
	
	if (grdmv_work->ref_snm_reg_id == -1) {
		return;
	}
	
	// ワールドマトリクス取得
	w_mtx_ref	= GmBsCmnGetSNMMtx(&body_work->snm_work,
								   grdmv_work->ref_snm_reg_id);
	
	// 各ノードの水平座標を取得
	pivot_x	= body_work->grdmv_pivot_pos.x;
	ref_x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx_ref, 0, 3));
	
	// 基準点と参照点の水平距離情報を更新
	grdmv_work->prev_diff_x	= grdmv_work->cur_diff_x;
	grdmv_work->cur_diff_x	= ref_x - pivot_x;
	
	if (grdmv_work->is_first_updated == FALSE) {
		grdmv_work->is_first_updated	= TRUE;
		// 初回更新では座標反映をスキップする
		return;
	}
	
	// 前回からずれた分、移動させる
	GMM_BS_OBJ(body_work)->pos.x	-= grdmv_work->cur_diff_x - grdmv_work->prev_diff_x;
}

// =======================================================================
// gmBoss5BodyChangeMovePhase
/*!
  移動フェーズ切り替え
  
  @param body_work			[io]	本体ワーク
  @param move_phase_type	[in]	歩行フェーズタイプ
 */
// =======================================================================
void gmBoss5BodyChangeMovePhase(GMS_BOSS5_BODY_WORK *body_work,
								GME_BOSS5_BODY_MOVE_PHASE_TYPE move_phase_type)
{
	Sint32	ref_snm_reg_id;
	
	body_work->cur_move_phase_type	= move_phase_type;
	
	if (body_work->cur_move_phase_type == GME_BOSS5_BODY_MOVE_PHASE_TYPE_LEFT) {
		ref_snm_reg_id	= body_work->lfoot_snm_reg_id;
	}
	else if (body_work->cur_move_phase_type == GME_BOSS5_BODY_MOVE_PHASE_TYPE_RIGHT) {
		ref_snm_reg_id	= body_work->rfoot_snm_reg_id;
	}
	else {
		MTM_ASSERT(body_work->cur_move_phase_type == GME_BOSS5_BODY_MOVE_PHASE_TYPE_NONE);
		// 更新しない
		ref_snm_reg_id	= -1;
	}
	
	// 接地移動初期化
	gmBoss5BodyInitGroundingMove(body_work, ref_snm_reg_id);
}

// =======================================================================
// gmBoss5BodyInitWalk
/*!
  歩行処理初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyInitWalk(GMS_BOSS5_BODY_WORK *body_work)
{
#if defined(MTD_DEBUG)
	// 適切なアクションが設定されているかチェック
	{
		OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
		
		MTM_ASSERT(obj_work->obj_3d->act_id[0] == IDB_BOSS05_BODY_MTN_B05_1_ATT02_01B_ZNM);
		UNREFERENCED_PARAMETER(obj_work);
	}
#endif /* defined(MTD_DEBUG) */
	
	// 移動フェーズ設定
	gmBoss5BodyChangeMovePhase(body_work, gm_boss5_body_walk_move_info_tbl[0].move_phase_type);
}

// =======================================================================
// gmBoss5BodyUpdateWalk
/*!
  歩行処理更新
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	最終フェーズ到達
  @retval FALSE	最終フェーズ未到達
  
  @note
  本体モーションの経過フレームを監視して、適切なノードを接地の対象に設定します。
  最終フェーズに到達するとTRUEを返しますが、歩行処理自体は終わっていません。
  歩行処理の終了をチェックするには、最終フェーズ到達後にOBD_DISP_ENDをチェックしてください。
 */
// =======================================================================
BOOL gmBoss5BodyUpdateWalk(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	Float	cur_frame	= obj_work->obj_3d->frame[0];
	Sint32	phase;
	
	// 接地移動更新
	gmBoss5BodyUpdateGroundingMove(body_work);
	
	for (phase = GMD_BOSS5_BODY_WALK_MOVE_PHASE_NUM - 1; phase >= 0; --phase) {
		if (gm_boss5_body_walk_move_info_tbl[phase].switching_frame <= cur_frame) {
			if (gm_boss5_body_walk_move_info_tbl[phase].move_phase_type != body_work->cur_move_phase_type) {
				// 移動フェーズタイプが変わったら、接地移動を再初期化
				gmBoss5BodyChangeMovePhase(body_work,
										   gm_boss5_body_walk_move_info_tbl[phase].move_phase_type);
			}
			break;
		}
	}
	
	// 最終フェーズ到達チェック
	if (phase >= GMD_BOSS5_BODY_WALK_MOVE_PHASE_NUM - 1) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss5BodyInitWalkAbortRecovery
/*!
  歩行中断からの復帰処理 初期化
  
  @param body_work				[io]	本体ワーク
  @param cur_move_phase_type	[in]	現在の移動フェーズタイプ
  
  @note
  現在の歩行処理の移動フェーズタイプに応じて、
  適切な足を地面に固定した状態でFWモーションに復帰させます。
  gmBoss5BodyUpdateWalkAbortRecovery()で更新してください。
 */
// =======================================================================
void gmBoss5BodyInitWalkAbortRecovery(GMS_BOSS5_BODY_WORK *body_work,
									  GME_BOSS5_BODY_MOVE_PHASE_TYPE cur_move_phase_type)
{
	GME_BOSS5_LEG_TYPE leg_type;
	
	// 対象脚タイプ取得（gmBoss5BodyInitWalk()済みである前提）
	if (cur_move_phase_type == GME_BOSS5_BODY_MOVE_PHASE_TYPE_LEFT) {
		leg_type	= GME_BOSS5_LEG_TYPE_LEFT;
	}
	else {
		leg_type	= GME_BOSS5_LEG_TYPE_RIGHT;
	}
	
	gmBoss5BodyInitWalkAbortRecoveryByLegType(body_work, leg_type);
}

// =======================================================================
// gmBoss5BodyInitWalkAbortRecoveryByLegType
/*!
  歩行中断からの復帰処理 初期化（対象脚直接指定）
  
  @param body_work	[io]	本体ワーク
  @param leg_type	[in]	固定対象脚タイプ
  
  @note
  指定した足を地面に固定した状態でFWモーションに復帰させます。
  gmBoss5BodyUpdateWalkAbortRecovery()で更新してください。
 */
// =======================================================================
void gmBoss5BodyInitWalkAbortRecoveryByLegType(GMS_BOSS5_BODY_WORK *body_work,
											   GME_BOSS5_LEG_TYPE leg_type)
{
	MTM_ASSERT(body_work->state == GME_BOSS5_BODY_STATE_MOVE_NML);
	
	// FWへのブレンド時のズレ調整用に元姿勢を記録
	gmBoss5BodyRecordGapAdjustmentSrc(body_work);
	
	// モーションブレンドによるズレを補正
	gmBoss5BodyInitAdjustMtnBlendHGap(body_work,
									  body_work->whole_act_id,
									  leg_type);
	gmBoss5BodyUpdateAdjustMtnBlendHGap(body_work);
}

// =======================================================================
// gmBoss5BodyUpdateWalkAbortRecovery
/*!
  歩行中断からの復帰処理 更新
  
  @param body_work				[io]	本体ワーク
 */
// =======================================================================
BOOL gmBoss5BodyUpdateWalkAbortRecovery(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (obj_work->obj_3d->flag & OBD_ACTFLAG_3D_NN_BLEND) {
		// モーションブレンドによるズレを補正
		gmBoss5BodyUpdateAdjustMtnBlendHGap(body_work);
		return FALSE;
	}
	else {
		return TRUE;
	}
}

// =======================================================================
// gmBoss5BodyInitMonitoringWalkEnd
/*!
  歩行終了タイミング監視 初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  足が接地したタイミングかつ次の一歩で壁に衝突するか、
  また、プレイヤーが後方にいたかを監視します。
  gmBoss5BodyUpdateMonitoringWalkEnd()で更新・結果判定を行ってください。
  gmBoss5BodyUpdateMonitoringWalkEnd()による更新中に一度でもプレイヤーが
  本体よりも後方に位置していた場合は足が接地したタイミングでTRUEと判定されます。
 */
// =======================================================================
void gmBoss5BodyInitMonitoringWalkEnd(GMS_BOSS5_BODY_WORK *body_work)
{
	body_work->walk_end_monitor_phase_cnt	= 0;
	body_work->is_player_behind	= FALSE;
}

// =======================================================================
// gmBoss5BodyInitMonitoringWalkEnd
/*!
  歩行終了タイミング監視 初期化
  
  @param body_work	[io]	本体ワーク
  @param leg_type	[out]	接地した足の脚タイプを格納（NULL可）
  
  @note
  最後の一歩だった場合はそれ以上歩を進めないため、FALSEと判定されます。
  戻り値がFALSEの場合はleg_typeには最後にTRUE判定されたときの脚タイプが格納されます。
 */
// =======================================================================
BOOL gmBoss5BodyUpdateMonitoringWalkEnd(GMS_BOSS5_BODY_WORK *body_work,
										GME_BOSS5_LEG_TYPE *leg_type)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	Float	cur_frame	= obj_work->obj_3d->frame[0];
	Sint32	phase;
	BOOL	is_land_timing	= FALSE;
	
	if (gmBoss5BodyIsPlayerBehind(body_work)) {
		body_work->is_player_behind	= TRUE;
	}
	
	// フェーズ更新
	for (phase = GMD_BOSS5_BODY_WALK_GROUND_TIMING_PAHSE_NUM - 1; phase >= 0; --phase) {
		if (gm_boss5_body_walk_ground_timing_info_tbl[phase].grounding_frame <= cur_frame) {
			if (phase != body_work->walk_end_monitor_phase_cnt) {
				// 移動フェーズタイプが変わったら接地したタイミングと判定
				is_land_timing	= TRUE;
				
				// 現在のフェーズを保存
				body_work->walk_end_monitor_phase_cnt	= phase;
			}
			break;
		}
	}
	
	// 脚タイプ取得
	if (leg_type) {
		if (phase >= 0) {
			*leg_type	= gm_boss5_body_walk_ground_timing_info_tbl[phase].leg_type;
		}
		else {
			MTM_ASSERT(FALSE);
			*leg_type	= GME_BOSS5_LEG_TYPE_LEFT;	// 保険
		}
	}
	
	// 最終フェーズ到達チェック
	// （最終フェーズの場合は接地タイミングかどうかに関わらずFALSE）
	if (phase >= GMD_BOSS5_BODY_WALK_GROUND_TIMING_PAHSE_NUM - 1) {
		return FALSE;
	}
	
	// 接地タイミングだった場合、歩行終了条件を満たしているかチェック
	if (is_land_timing) {
		// プレイヤーがこれまでに後方にいたかチェック
		if (body_work->is_player_behind) {
			return TRUE;
		}
		
		// エリア左右端から既定距離以内にいるかチェック
		if (obj_work->disp_flag & OBD_DISP_HFLIP) {
			if (obj_work->pos.x <= GMM_BOSS5_AREA_LEFT() + GMD_BOSS5_BODY_WALK_WALK_END_WALL_DISTANCE) {
				return TRUE;
			}
		}
		else {
			if (obj_work->pos.x >= GMM_BOSS5_AREA_RIGHT() - GMD_BOSS5_BODY_WALK_WALK_END_WALL_DISTANCE) {
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

// =======================================================================
// gmBoss5BodyInitWalkGroundingEffects
/*!
  歩行接地時処理 初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyInitWalkGroundingEffects(GMS_BOSS5_BODY_WORK *body_work)
{
	body_work->cur_walk_grnd_phase_cnt	= 0;
}

// =======================================================================
// gmBoss5BodyUpdateWalkGroundingEffects
/*!
  歩行接地時処理 更新
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
BOOL gmBoss5BodyUpdateWalkGroundingEffects(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	Float	cur_frame	= obj_work->obj_3d->frame[0];
	Sint32	phase;
	
	for (phase = GMD_BOSS5_BODY_WALK_GROUND_TIMING_PAHSE_NUM - 1; phase >= 0; --phase) {
		if (gm_boss5_body_walk_ground_timing_info_tbl[phase].grounding_frame <= cur_frame) {
			if (phase != body_work->cur_walk_grnd_phase_cnt) {
				// 移動フェーズタイプが変わったら各種エフェクト等の処理
				
				// 接地時の煙エフェクト生成
				GmBoss5EfctCreateWalkStepSmoke(body_work,
											   gm_boss5_body_walk_ground_timing_info_tbl[phase].leg_type);
				
				// 歩行振動
				gmBoss5Vibration(GME_BOSS5_VIB_IDX_WALK_STEP);
				
				// 歩きSE再生
				GmSoundPlaySE("FinalBoss03");
				
				body_work->cur_walk_grnd_phase_cnt	= phase;
			}
			break;
		}
	}
	
	// 最終フェーズ到達チェック
	if (phase >= GMD_BOSS5_BODY_WALK_GROUND_TIMING_PAHSE_NUM - 1) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss5BodyInitRunGroundingEffects
/*!
  走行接地時処理 初期化
  
  @param body_work	[io]	本体ワーク
  @param run_type	[in]	走行タイプ
  @param delay		[in]	発生遅延フレーム
  
  @note
  タイミングの判定は行なわず、呼ばれたら直ちにor指定遅延後にエフェクト生成などの各種処理を行います。
  初期が後はgmBoss5BodyUpdateRunGroundingEffects()をTRUEが返るまで毎フレーム呼び出してください。
  RUN_TYPEが切り替わるフレームで呼び出す場合は、RUN_TYPEを設定する前に呼び出してください。
 */
// =======================================================================
void gmBoss5BodyInitRunGroundingEffects(GMS_BOSS5_BODY_WORK *body_work,
										GME_BOSS5_BODY_RUN_TYPE run_type,
										Uint32 delay)
{
	body_work->run_grnd_runtype	= run_type;
	body_work->run_grnd_delay_timer	= delay;
	body_work->run_grnd_spawn_remain	= 1;	// 1回だけ生成
}

// =======================================================================
// gmBoss5BodyUpdateRunGroundingEffects
/*!
  走行接地時処理 更新
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
BOOL gmBoss5BodyUpdateRunGroundingEffects(GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->run_grnd_delay_timer) {
		body_work->run_grnd_delay_timer--;
	}
	else {
		// 既定回数生成
		if (body_work->run_grnd_spawn_remain) {
			body_work->run_grnd_spawn_remain--;
		}
		else {
			return TRUE;
		}
		
		// 煙エフェクト生成
		switch (body_work->run_grnd_runtype) {
		case GME_BOSS5_BODY_RUN_TYPE_LEFT:
			GmBoss5EfctCreateRunStepSmoke(body_work,
										  GME_BOSS5_LEG_TYPE_LEFT);
			break;
			
		case GME_BOSS5_BODY_RUN_TYPE_RIGHT:
			GmBoss5EfctCreateRunStepSmoke(body_work,
										  GME_BOSS5_LEG_TYPE_RIGHT);
			break;
			
		default:
			MTM_ASSERT(FALSE);
		}
		
		// 走行振動
		gmBoss5Vibration(GME_BOSS5_VIB_IDX_RUN_STEP);
		
		// 歩きSE再生
		GmSoundPlaySE("FinalBoss03");
		
		return TRUE;
	}
	
	return FALSE;
}


// =======================================================================
// gmBoss5BodyInitStompFlyUp
/*!
  ストンプ用上昇移動処理 初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  上昇して画面外に移動する処理を行います。
 */
// =======================================================================
void gmBoss5BodyInitStompFlyUp(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 速度クリア
	GmBsCmnSetObjSpdZero(obj_work);
	
	// フラグ設定
	obj_work->move_flag	&= ~(OBD_MOVE_FALL | OBD_MOVE_UNDER);
	obj_work->move_flag	|= OBD_MOVE_JUMP | OBD_MOVE_NOCOL;
	
	// 初速度設定
	obj_work->spd.y	= GMD_BOSS5_BODY_SFLYUP_INIT_SPD;
	
	// 上昇加速度設定
	obj_work->spd_add.y		= GMD_BOSS5_BODY_SFLYUP_ACC;
}

// =======================================================================
// gmBoss5BodyUpdateStompFlyUp
/*!
  ストンプ用上昇移動処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	画面外到達
  @retval FALSE	上昇中
 */
// =======================================================================
BOOL gmBoss5BodyUpdateStompFlyUp(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (obj_work->pos.y <= GMM_BOSS5_AREA_TOP() - GMD_BOSS5_BODY_HIDE_RADIUS) {
		// 停止
		GmBsCmnSetObjSpdZero(obj_work);
		
		return TRUE;
	}
	else {
		return FALSE;
	}
}


// =======================================================================
// gmBoss5BodyInitStompFall
/*!
  ストンプ用降下処理 初期化
 
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyInitStompFall(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 停止
	GmBsCmnSetObjSpdZero(obj_work);
	
	// 初速設定
	obj_work->spd.y	= GMD_BOSS5_BODY_STOMP_FALL_INIT_SPD;
	
	// 加速設定
	obj_work->spd_add.y	= GMD_BOSS5_BODY_STOMP_FALL_ACC;
}

// =======================================================================
// gmBoss5BodyUpdateStompFall
/*!
  ストンプ用降下処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	降下処理完了
  @retval FALSE	降下処理中
 */
// =======================================================================
BOOL gmBoss5BodyUpdateStompFall(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (obj_work->pos.y >= GMM_BOSS5_AREA_TOP()) {
		// エリアの上端を過ぎたら地形当たりオン
		obj_work->move_flag	&= ~OBD_MOVE_NOCOL;
	}
	
	// 着地待ち
	if (obj_work->move_flag & OBD_MOVE_UNDER) {
		
		// 停止
		GmBsCmnSetObjSpdZero(obj_work);
		
		// 落下設定復帰
		obj_work->move_flag	|= OBD_MOVE_FALL;
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// gmBoss5BodyInitCrashFlyUp
/*!
  地球割り用上昇移動処理 初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  上昇して画面外に移動する処理を行います。
 */
// =======================================================================
void gmBoss5BodyInitCrashFlyUp(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 速度クリア
	GmBsCmnSetObjSpdZero(obj_work);
	
	// フラグ設定
	obj_work->move_flag	&= ~(OBD_MOVE_FALL | OBD_MOVE_UNDER);
	obj_work->move_flag	|= OBD_MOVE_JUMP | OBD_MOVE_NOCOL;
	
	// 初速度設定
	obj_work->spd.y	= GMD_BOSS5_BODY_CFLYUP_INIT_SPD;
	
	// 上昇加速度設定
	obj_work->spd_add.y	= GMD_BOSS5_BODY_CFLYUP_ACC;
}

// =======================================================================
// gmBoss5BodyUpdateCrashFlyUp
/*!
  地球割り用上昇移動処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	画面外到達
  @retval FALSE	上昇中
 */
// =======================================================================
BOOL gmBoss5BodyUpdateCrashFlyUp(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (obj_work->pos.y <= GMM_BOSS5_AREA_TOP() - GMD_BOSS5_BODY_HIDE_RADIUS) {
		// 停止
		GmBsCmnSetObjSpdZero(obj_work);
		
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss5BodyInitCrashFall
/*!
  地球割り用降下処理 初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyInitCrashFall(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 停止
	GmBsCmnSetObjSpdZero(obj_work);
	
	// 初速設定
	obj_work->spd.y	= GMD_BOSS5_BODY_CRASH_FALL_INIT_SPD;
	
	// 加速設定
	obj_work->spd_add.y	= GMD_BOSS5_BODY_CRASH_FALL_ACC;
}

// =======================================================================
// gmBoss5BodyUpdateCrashFall
/*!
  地球割り用降下処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	降下処理完了
  @retval FALSE	降下処理中
 */
// =======================================================================
BOOL gmBoss5BodyUpdateCrashFall(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (obj_work->pos.y >= GMM_BOSS5_AREA_TOP()) {
		// エリアの上端を過ぎたら地形当たりオン
		obj_work->move_flag	&= ~OBD_MOVE_NOCOL;
	}
	
	// 着地待ち
	if (obj_work->move_flag & OBD_MOVE_UNDER) {
		
		// 停止
		GmBsCmnSetObjSpdZero(obj_work);
		
		// 落下設定復帰
		obj_work->move_flag	|= OBD_MOVE_FALL;
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// gmBoss5BodyInitCrashSink
/*!
  地球割り用沈下処理 初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  地面下に沈んでいく移動処理です。
 */
// =======================================================================
void gmBoss5BodyInitCrashSink(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	/*
	  MEMO: 現状はオブジェクトの落下処理にまかせる
	 */
	
	// 速度クリア
	GmBsCmnSetObjSpdZero(obj_work);
	
	// フラグ設定
	obj_work->move_flag	|= OBD_MOVE_NOCOL | OBD_MOVE_FALL;
	obj_work->move_flag	&= ~OBD_MOVE_UNDER;
}

// =======================================================================
// gmBoss5BodyUpdateCrashSink
/*!
  地球割り用沈下処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	沈下移動完了
  @retval FALSE	沈下移動中
 */
// =======================================================================
BOOL gmBoss5BodyUpdateCrashSink(GMS_BOSS5_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
	
	/*
	  MEMO: 現状はオブジェクトの落下処理にまかせておく
	 */
	// 完了しない
	return FALSE;
}

// =======================================================================
// gmBoss5BodyInitBerserkTurn
/*!
  凶暴化演出用方向転換処理 初期化
  
  @param body_work	[io]	本体ワーク
  @param turn_type	[in]	方向転換タイプ
 */
// =======================================================================
void gmBoss5BodyInitBerserkTurn(GMS_BOSS5_BODY_WORK *body_work, GME_BOSS5_BODY_BSK_TURN_TYPE turn_type)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	Float	tgt_deg;
	Angle32	tgt_dir;
	
	switch (turn_type) {
	case GME_BOSS5_BODY_BSK_TURN_TYPE_FRONT:
		tgt_deg	= GMD_BOSS5_BODY_BERSERK_TURN_FRONT_DEG_F;
		break;
		
	case GME_BOSS5_BODY_BSK_TURN_TYPE_RETURN:
		tgt_deg	= GMD_BOSS5_BODY_BERSERK_TURN_RETURN_DEG_F;
		break;
		
	default:
		MTM_ASSERT(0);
		tgt_deg	= 0.f;
	}
	
	// 左向きの時は反対回り
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		tgt_deg	= -tgt_deg;
	}
	
	// 元の角度
	body_work->turn_src_dir	= MTD_MATH_ANGLE_MASK & (Angle32)obj_work->dir.y;
	
	// 目標の角度
	tgt_dir	= MTD_MATH_ANGLE_MASK & AKM_DEGtoA32(tgt_deg);
	
	// 目標までの差分
	body_work->turn_tgt_ofst_dir = MTD_MATH_ANGLE_MASK & (tgt_dir - body_work->turn_src_dir);
	MTM_ASSERT(body_work->turn_tgt_ofst_dir >= 0);
	
	// 最短角度となるオフセット角を取得
	if (body_work->turn_tgt_ofst_dir > AKM_DEGtoA32(180)) {
		body_work->turn_tgt_ofst_dir	= body_work->turn_tgt_ofst_dir - MTD_MATH_MAX_ANGLE;
		MTM_ASSERT(body_work->turn_tgt_ofst_dir > AKM_DEGtoA32(-180));
	}
	
	// 進捗クリア
	body_work->turn_ratio	= 0.f;
}

// =======================================================================
// gmBoss5BodyUpdateBerserkTurn
/*!
  凶暴化演出用方向転換処理 更新
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
BOOL gmBoss5BodyUpdateBerserkTurn(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	BOOL	result	= FALSE;
	Angle32	ofst_dir;
	
	// 進捗更新
	body_work->turn_ratio	+= GMD_BOSS5_BODY_BERSERK_TURN_RATIO_SPD;
	if (body_work->turn_ratio >= 1.f) {
		body_work->turn_ratio	= 1.f;
		
		result	= TRUE;
	}
	
	// 元角度からの現在の進捗角度を取得
	ofst_dir	= (Angle32)(body_work->turn_tgt_ofst_dir * body_work->turn_ratio);
	
	// 角度反映
	obj_work->dir.y	= (Uint16)(MTD_MATH_ANGLE_MASK & (body_work->turn_src_dir + ofst_dir));
	
	return result;
}

// =======================================================================
// gmBoss5BodyClearBerserkTurn
/*!
  凶暴化演出用方向転換処理 クリア
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyClearBerserkTurn(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 標準の方向にFIX
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		gmBoss5BodySetDirection(body_work, GME_BOSS5_BODY_DIRECTION_TYPE_LEFT);
	}
	else {
		gmBoss5BodySetDirection(body_work, GME_BOSS5_BODY_DIRECTION_TYPE_RIGHT);
	}
	
	// パラメータクリア
	body_work->turn_src_dir	= 0;
	body_work->turn_tgt_ofst_dir	= 0;
	body_work->turn_ratio	= 0.f;
}

// =======================================================================
// gmBoss5BodyInitPoke
/*!
  腕突き出し 初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  CNMを使用して両腕を前に突き出します。
 */
// =======================================================================
void gmBoss5BodyInitPoke(GMS_BOSS5_BODY_WORK *body_work)
{
	// 腕姿勢操作初期化
	gmBoss5BodyInitArmPose(body_work);
	
	// 先頭フェーズを初期化
	body_work->arm_poke_anim_phase	= 0;
	gmBoss5BodyInitArmAnim(body_work,
						   &gm_boss5_arm_anim_info_tbl[body_work->arm_poke_anim_phase]);
	
	// 腕突き出し動作中
	MTM_ASSERT(!(body_work->flag & GMD_BOSS5_BODY_FLAG_IS_POKING));
	body_work->flag	|= GMD_BOSS5_BODY_FLAG_IS_POKING;
	
	// 攻撃当たり有効
	body_work->flag	|= GMD_BOSS5_BODY_FLAG_RKT_CNCT_ATK_ACTIVE;
}

// =======================================================================
// gmBoss5BodyUpdatePoke
/*!
  腕突き出し 更新
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
BOOL gmBoss5BodyUpdatePoke(GMS_BOSS5_BODY_WORK *body_work)
{
	BOOL	result	= FALSE;
	
	MTM_ASSERT(body_work->arm_poke_anim_phase < GMD_BOSS5_ARM_POKE_ANIM_PHASE_NUM);
	
	if (gmBoss5BodyUpdateArmAnim(body_work)) {
		// フェーズ更新
		body_work->arm_poke_anim_phase++;
		
		if (body_work->arm_poke_anim_phase >= GMD_BOSS5_ARM_POKE_ANIM_PHASE_NUM) {
			// 最終フェーズが終了
			body_work->arm_poke_anim_phase	= GMD_BOSS5_ARM_POKE_ANIM_PHASE_NUM - 1;
			result	= TRUE;
		}
		else {
			// 新しいフェーズのアニメーションを初期化
			gmBoss5BodyInitArmAnim(body_work,
								   &gm_boss5_arm_anim_info_tbl[body_work->arm_poke_anim_phase]);
		}
	}
	
	return result;
}

// =======================================================================
// gmBoss5BodyEndPoke
/*!
  腕突き出し 終了
  
  @param body_work	[io]	本体ワーク
  
  @note
  この関数を呼ぶまでは腕突き出し処理中とみなされます。
 */
// =======================================================================
void gmBoss5BodyEndPoke(GMS_BOSS5_BODY_WORK *body_work)
{
	// 攻撃当たり無効
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_RKT_CNCT_ATK_ACTIVE;
	
	// 腕突き出し動作終了
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_IS_POKING;
	
	// 腕姿勢操作終了
	gmBoss5BodyEndArmPose(body_work);
	
	body_work->arm_poke_anim_phase	= 0;
}

// =======================================================================
// gmBoss5BodyIsPoking
/*!
  腕突き出し処理中判定
  
  @param body_work	[in]	本体ワーク
  
  @retval TRUE	腕突き出し処理中
  @retval FALSE	腕突き出し処理中でない
 */
// =======================================================================
BOOL gmBoss5BodyIsPoking(const GMS_BOSS5_BODY_WORK *body_work)
{
	MTM_ASSERT(body_work);
	
	if (body_work->flag & GMD_BOSS5_BODY_FLAG_IS_POKING) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss5BodyInitArmAnim
/*!
  腕アニメーション処理 初期化
  
  @param body_work	[io]	本体ワーク
  @param anim_info	[in]	腕アニメーション情報
 */
// =======================================================================
void gmBoss5BodyInitArmAnim(GMS_BOSS5_BODY_WORK *body_work,
							const GMS_BOSS5_ARM_ANIM_INFO *anim_info)
{
	if (anim_info->wait_time > 0) {
		// 待機時間が設定されている場合は開始姿勢のままアニメーションしない
		
		// 各種パラメータ初期化
		body_work->arm_anim_work.is_anim	= FALSE;
		body_work->arm_anim_work.anim_wait_timer	= anim_info->wait_time;
		body_work->arm_anim_work.cur_rate	= 0.f;
		body_work->arm_anim_work.rate_add	= 0.f;
		
		// 開始姿勢のみ取得
		for (Sint32 part_idx = 0; part_idx < GME_BOSS5_ARMPART_IDX_MAX; ++part_idx) {
			nnMakeRotateXYZQuaternion(&body_work->arm_anim_work.start_quat[part_idx],
									  anim_info->part_anim_info[part_idx].start_rot.x,
									  anim_info->part_anim_info[part_idx].start_rot.y,
									  anim_info->part_anim_info[part_idx].start_rot.z);
			nnMakeUnitQuaternion(&body_work->arm_anim_work.end_quat[part_idx]);
		}
	}
	else {
		// 待機時間が設定されていない場合はアニメーションする
		
		// 各種パラメータ初期化
		body_work->arm_anim_work.is_anim	= TRUE;
		body_work->arm_anim_work.anim_wait_timer	= 0;
		body_work->arm_anim_work.cur_rate	= 0.f;
		body_work->arm_anim_work.rate_add	= anim_info->slerp_inc_rate;
		
		// 開始・終了姿勢を取得
		for (Sint32 part_idx = 0; part_idx < GME_BOSS5_ARMPART_IDX_MAX; ++part_idx) {
			nnMakeRotateXYZQuaternion(&body_work->arm_anim_work.start_quat[part_idx],
									  anim_info->part_anim_info[part_idx].start_rot.x,
									  anim_info->part_anim_info[part_idx].start_rot.y,
									  anim_info->part_anim_info[part_idx].start_rot.z);
			nnMakeRotateXYZQuaternion(&body_work->arm_anim_work.end_quat[part_idx],
									  anim_info->part_anim_info[part_idx].end_rot.x,
									  anim_info->part_anim_info[part_idx].end_rot.y,
									  anim_info->part_anim_info[part_idx].end_rot.z);
		}
	}
	
	// 各腕・各パーツの姿勢パラメータ設定
	for (Sint32 part_idx = 0; part_idx < GME_BOSS5_ARMPART_IDX_MAX; ++part_idx) {
		NNS_QUATERNION	right_quat;
		gmBoss5BodySetArmPoseParam(body_work, GME_BOSS5_ARM_TYPE_LEFT, (GME_BOSS5_ARMPART_IDX)part_idx,
								   &body_work->arm_anim_work.start_quat[part_idx]);
		AkMathInvertYZQuaternion(&right_quat, &body_work->arm_anim_work.start_quat[part_idx]);	// YZ平面で対称となる姿勢を取得
		gmBoss5BodySetArmPoseParam(body_work, GME_BOSS5_ARM_TYPE_RIGHT,
								   (GME_BOSS5_ARMPART_IDX)part_idx, &right_quat);
	}
	
	// 姿勢を反映
	gmBoss5BodyApplyArmPose(body_work);
}

// =======================================================================
// gmBoss5BodyUpdateArmAnim
/*!
  腕アニメーション処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	終了
  @retval FALSE	更新中
 */
// =======================================================================
BOOL gmBoss5BodyUpdateArmAnim(GMS_BOSS5_BODY_WORK *body_work)
{
	BOOL	result	= FALSE;
	
	if (body_work->arm_anim_work.is_anim) {
		// アニメーションする場合は更新
		
		body_work->arm_anim_work.cur_rate	+= body_work->arm_anim_work.rate_add;
		if (body_work->arm_anim_work.cur_rate >= 1.f) {
			body_work->arm_anim_work.cur_rate	= 1.f;
			result	= TRUE;
		}
		
		// 各腕・各パーツの姿勢パラメータ設定
		for (Sint32 part_idx = 0; part_idx < GME_BOSS5_ARMPART_IDX_MAX; ++part_idx) {
			NNS_QUATERNION	left_quat;
			NNS_QUATERNION	right_quat;
			
			// 開始姿勢から終了姿勢の間で球面線形補間
			nnSlerpQuaternion(&left_quat,
							  &body_work->arm_anim_work.start_quat[part_idx],
							  &body_work->arm_anim_work.end_quat[part_idx],
							  body_work->arm_anim_work.cur_rate);
			
			// 左右腕の各パーツの姿勢パラメータを設定
			gmBoss5BodySetArmPoseParam(body_work, GME_BOSS5_ARM_TYPE_LEFT,
									   (GME_BOSS5_ARMPART_IDX)part_idx, &left_quat);
			AkMathInvertYZQuaternion(&right_quat, &left_quat);	// YZ平面で対称となる姿勢を取得
			gmBoss5BodySetArmPoseParam(body_work, GME_BOSS5_ARM_TYPE_RIGHT,
									   (GME_BOSS5_ARMPART_IDX)part_idx, &right_quat);
		}
	}
	else {
		// アニメーションせずに待機する場合
		
		// 既定時間待機
		if (body_work->arm_anim_work.anim_wait_timer) {
			body_work->arm_anim_work.anim_wait_timer--;
		}
		else {
			result	= TRUE;
		}
	}
	
	// 姿勢を反映
	gmBoss5BodyApplyArmPose(body_work);
	
	return result;
}

// =======================================================================
// gmBoss5BodyInitCloseCanopy
/*!
  キャノピークローズ処理 初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyInitCloseCanopy(GMS_BOSS5_BODY_WORK *body_work)
{
	
	// 開いた状態を初期姿勢に設定
	nnMakeRotateXYZQuaternion(&body_work->cnpy_close_init_quat,
							  GMD_BOSS5_BODY_CANOPY_CLOSE_START_ANGLE_X,
							  0,
							  0);
	
	// 通常姿勢を目標姿勢とする
	nnMakeRotateXYZQuaternion(&body_work->cnpy_close_dest_quat,
							  0, 0, 0);
	
	// 進捗初期化
	body_work->cnpy_close_ratio	= 0.f;
	
	// 進捗速度初期化
	body_work->cnpy_close_ratio_spd	= 0.f;
}

// =======================================================================
// gmBoss5BodyUpdateCloseCanopy
/*!
  キャノピークローズ処理 更新
  
  @param body_work	[io]	本体ワーク
  @param is_update	[in]	更新フラグ
  
  @retval TRUE	クローズ完了
  @retval FALSE	クローズ中
  
  @note
  is_updateをTRUE指定すると、姿勢を更新します。
  FALSEを指定すると、現在の進捗の姿勢を維持します。
 */
// =======================================================================
BOOL gmBoss5BodyUpdateCloseCanopy(GMS_BOSS5_BODY_WORK *body_work, BOOL is_update)
{
	NNS_QUATERNION	quat;
	BOOL	result	= FALSE;
	
	// 加速
	if (is_update) {
		body_work->cnpy_close_ratio_spd	+= GMD_BOSS5_BODY_CANOPY_CLOSE_RATIO_SPD_ACC;
	}
	
	if (body_work->cnpy_close_ratio_spd >= GMD_BOSS5_BODY_CANOPY_CLOSE_RATIO_SPD_MAX) {
		body_work->cnpy_close_ratio_spd	= GMD_BOSS5_BODY_CANOPY_CLOSE_RATIO_SPD_MAX;
	}
	
	
	// 進捗更新
	if (is_update) {
		body_work->cnpy_close_ratio	+= body_work->cnpy_close_ratio_spd;
	}
	
	if (body_work->cnpy_close_ratio >= 1.f) {
		body_work->cnpy_close_ratio	= 1.f;
		result	= TRUE;
	}
	
	// 進捗に応じた姿勢を球面線形補間して取得
	nnSlerpQuaternion(&quat,
					  &body_work->cnpy_close_init_quat,
					  &body_work->cnpy_close_dest_quat,
					  body_work->cnpy_close_ratio);
	
	// 回転をキャノピーに反映
	{
		NNS_MATRIX	mtx;
		nnMakeQuaternionMatrix(&mtx, &quat);
		GmBsCmnSetCNMMtx(&body_work->cnm_mgr_work,
						 &mtx,
						 body_work->head_cnm_reg_id);
	}
	
	// ポールをスケール0で非表示にする
	{
		NNS_MATRIX	mtx;
		nnMakeScaleMatrix(&mtx, 0, 0, 0);
		GmBsCmnSetCNMMtx(&body_work->cnm_mgr_work,
						 &mtx,
						 body_work->pole_cnm_reg_id);
	}
	
	return result;
}


// =======================================================================
// gmBoss5BodyInitScatterFall
/*!
  パーツ飛散処理時 本体落下処理 初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyInitScatterFall(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// フラグ設定
	obj_work->move_flag	|= OBD_MOVE_FALL | OBD_MOVE_JUMP;
	obj_work->move_flag	&= ~OBD_MOVE_UNDER;
	
	// 地形当たり矩形を再設定
	// お腹が接地するように
	ObjObjectFieldRectSet(obj_work,
						  GMD_BOSS5_BODY_SCT_FIEELD_RECT_SIZE_LEFT,
						  GMD_BOSS5_BODY_SCT_FIEELD_RECT_SIZE_TOP,
						  GMD_BOSS5_BODY_SCT_FIEELD_RECT_SIZE_RIGHT,
						  GMD_BOSS5_BODY_SCT_FIEELD_RECT_SIZE_BOTTOM);
	
	// 着地時の振動タイマを設定
	body_work->sct_land_vib_timer	= GMD_BOSS5_BODY_DEFEAT_SCT_FALL_LAND_VIB_TIME;
}

// =======================================================================
// gmBoss5BodyUpdateScatterFall
/*!
  パーツ飛散処理時 本体落下処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	落下処理完了
  @retval FALSE	落下処理中
  
  @note
  着地時に振動処理を行います。
 */
// =======================================================================
BOOL gmBoss5BodyUpdateScatterFall(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (body_work->sct_land_vib_timer) {
		if ((obj_work->move_flag & OBD_MOVE_UNDER)) {
			//body_work->sct_land_vib_timer) {
			
			fx32	amp;
			
			// タイマが0に近づくにつれて振幅を小さくする
			amp	= (fx32)(GMD_BOSS5_BODY_DEFEAT_SCT_FALL_LAND_VIB_AMP *
						 ((Float)body_work->sct_land_vib_timer / (Float)GMD_BOSS5_BODY_DEFEAT_SCT_FALL_LAND_VIB_TIME));
			
			// Y方向に振動させる
			obj_work->ofst.y	= (fx32)(amp *
										 nnSin((GMD_BOSS5_BODY_DEFEAT_SCT_FALL_LAND_VIB_TIME - body_work->sct_land_vib_timer) *
											   GMD_BOSS5_BODY_DEFEAT_SCT_FALL_LAND_VIB_DEG_SPD));
			
			// タイマ更新
			body_work->sct_land_vib_timer--;
		}
	}
	else {
		// タイマ終了時点で終了
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// gmBoss5BodyInitShakeAccelerate
/*!
  凶暴化演出時 振動動作加速処理 初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  振動モーションの再生速度を既定の速度まで加速していきます。
 */
// =======================================================================
void gmBoss5BodyInitShakeAccelerate(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 進捗クリア
	body_work->bsk_shake_acc_ratio	= 0.f;
	
	// 進捗速度初期化
	body_work->bsk_shake_acc_ratio_spd	= 0.f;
	
	// 初期モーション速度を記録
	body_work->bsk_shake_init_spd	= obj_work->obj_3d->speed[0];
}

// =======================================================================
// gmBoss5BodyUpdateShakeAccelerate
/*!
  凶暴化演出時 振動動作加速処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	既定の再生速度に到達
  @retval FALSE	加速中
 */
// =======================================================================
BOOL gmBoss5BodyUpdateShakeAccelerate(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	BOOL	result	= FALSE;
	
	// 進捗更新
	body_work->bsk_shake_acc_ratio_spd	+= GMD_BOSS5_BODY_BERSERK_SHAKE_MOTION_SPD_RATIO_ACC;
	body_work->bsk_shake_acc_ratio	+= body_work->bsk_shake_acc_ratio_spd;
	if (body_work->bsk_shake_acc_ratio >= 1.f) {
		body_work->bsk_shake_acc_ratio	= 1.f;
		body_work->bsk_shake_acc_ratio_spd	= 0.f;
		
		result	= TRUE;
	}
	
	// 進捗度合いに応じて初期モーション速度から目標モーション速度へ線形補間
	obj_work->obj_3d->speed[0]	=
		(body_work->bsk_shake_acc_ratio * GMD_BOSS5_BODY_BERSERK_SHAKE_MOTION_SPD_DEST) +
			((1.f - body_work->bsk_shake_acc_ratio) * body_work->bsk_shake_init_spd);
	
	return result;
}

// =======================================================================
// gmBoss5BodyInitCrashStrikeVib
/*!
  地球割り地面突き振動本体処理 初期化
  
  @param body_work	[io]	本体ワーク
  @param delay		[in]	遅延フレーム数
 */
// =======================================================================
void gmBoss5BodyInitCrashStrikeVib(GMS_BOSS5_BODY_WORK *body_work, Uint32 delay)
{
	body_work->crash_strike_vib_delay_timer	= delay;
	body_work->crash_strike_vib_phase	= 0;
	body_work->crash_strike_vib_ratio	= 0.f;
}

// =======================================================================
// gmBoss5BodyUpdateCrashStrikeVib
/*!
  地球割り地面突き本体振動処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	地面突き振動終了
  @retval FALSE	地面突き振動中
 */
// =======================================================================
BOOL gmBoss5BodyUpdateCrashStrikeVib(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	fx32	scale;
	BOOL	result	= FALSE;
	
	// 最初の既定時間は何もしない
	if (body_work->crash_strike_vib_delay_timer) {
		body_work->crash_strike_vib_delay_timer--;
		return FALSE;
	}
	
	body_work->crash_strike_vib_ratio	+= GMD_BOSS5_BODY_CRASH_STRIKE_BODY_VIB_RATIO_ADD;
	if (body_work->crash_strike_vib_ratio >= 1.f) {
		body_work->crash_strike_vib_ratio	= 1.f;
		result	= TRUE;
	}
	
	scale	= (fx32)(GMD_BOSS5_BODY_CRASH_STRIKE_BODY_VIB_SCALE * (1.f - body_work->crash_strike_vib_ratio));
	
	body_work->crash_strike_vib_phase	=
		GmBoss5UpdateVib(body_work->crash_strike_vib_phase,
						 scale,
						 &obj_work->ofst.x, &obj_work->ofst.y);
	
	return result;
}

// =======================================================================
// gmBoss5BodyInitStartRiseVib
/*!
  開始せり上がり時振動処理 初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyInitStartRiseVib(GMS_BOSS5_BODY_WORK *body_work)
{
	body_work->start_rise_vib_int_timer	= 0;
}

// =======================================================================
// gmBoss5BodyUpdateStartRiseVib
/*!
  開始せり上がり時振動処理 更新
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyUpdateStartRiseVib(GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->start_rise_vib_int_timer) {
		body_work->start_rise_vib_int_timer--;
	}
	else {
		// 初期めり込み位置から地面までの移動度合い
		Float	move_ratio	= FX_FX32_TO_F32(FX_Div(GMM_BS_OBJ(body_work)->pos.y - body_work->ground_v_pos,
													GMD_BOSS5_BODY_START_BURY_HEIGHT));
		
		// せり上がりとともにカメラ振動大きさが小→大→小となるようにサイン波0deg～180degで変化させる
		GmCameraVibrationSet((fx32)(GMD_BOSS5_BODY_START_RISE_VIB_AMP_MAX *
									nnSin(AKM_DEGtoA32(180.f * move_ratio))),
							 0, 0);
		
		body_work->start_rise_vib_int_timer	= GMD_BOSS5_BODY_START_RISE_VIB_INTERVAL;
	}
}

// =======================================================================
// gmBoss5BodyInitArmPose
/*!
  腕姿勢操作初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  腕姿勢操作が可能となるように各パーツのCNM有効化などの初期化処理を行います。
 */
// =======================================================================
void gmBoss5BodyInitArmPose(GMS_BOSS5_BODY_WORK *body_work)
{
	NNS_MATRIX	unit_mtx;
	
	nnMakeUnitMatrix(&unit_mtx);
	
	// 初期化
	for (Sint32 i = 0; i < GME_BOSS5_ARM_TYPE_MAX; ++i) {
		for (Sint32 j = 0; j < GME_BOSS5_ARMPART_IDX_MAX; ++j) {
			// CNM 左乗算モードに変更
			GmBsCmnChangeCNMModeNode(&body_work->cnm_mgr_work,
									 body_work->arm_cnm_reg_id[i][j],
									 GME_BS_CMN_CNM_MODE_MULT_LEFT);
			// CNM ノードローカル座標系でマトリクス設定
			GmBsCmnEnableCNMLocalCoordinate(&body_work->cnm_mgr_work,
											body_work->arm_cnm_reg_id[i][j],
											TRUE);
			// CNM適用有効化
			GmBsCmnEnableCNMMtxNode(&body_work->cnm_mgr_work,
									body_work->arm_cnm_reg_id[i][j],
									TRUE);
			// 設定マトリクス初期化
			GmBsCmnSetCNMMtx(&body_work->cnm_mgr_work,
							 &unit_mtx,
							 body_work->arm_cnm_reg_id[i][j]);
			
			// 各パーツの姿勢クォータニオン初期化
			nnMakeUnitQuaternion(&body_work->arm_part_rot_quat[i][j]);
		}
		
		// ロケット配置オフセットマトリクスを初期化
		nnMakeUnitMatrix(&body_work->rkt_ofst_mtx[i]);
	}
	
	// ロケット配置オフセットマトリクスを使用する
	body_work->flag	|= GMD_BOSS5_BODY_FLAG_USE_RKT_OFST;
}

// =======================================================================
// gmBoss5BodyEndArmPose
/*!
  腕姿勢操作終了
  
  @param body_work	[io]	本体ワーク
  
  @note
  腕姿勢操作対象の各パーツのCNMの無効化などを行い、腕姿勢操作を終了します。
  腕姿勢操作は負荷の高い処理ですので、使用しない場合は必ずこの関数を呼んで終了してください。
 */
// =======================================================================
void gmBoss5BodyEndArmPose(GMS_BOSS5_BODY_WORK *body_work)
{
	// ロケット配置オフセットマトリクスの使用解除
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_USE_RKT_OFST;
	
	// 終了設定
	for (Sint32 i = 0; i < GME_BOSS5_ARM_TYPE_MAX; ++i) {
		for (Sint32 j = 0; j < GME_BOSS5_ARMPART_IDX_MAX; ++j) {
			// CNM適用無効化
			GmBsCmnEnableCNMMtxNode(&body_work->cnm_mgr_work,
									body_work->arm_cnm_reg_id[i][j],
									FALSE);
		}
	}
}

// =======================================================================
// gmBoss5BodySetArmPoseParam
/*!
  腕姿勢パラメータを設定
  
  @param body_work		[io]	本体ワーク
  @param arm_type		[in]	腕タイプ
  @param arm_part_idx	[in]	腕パーツインデックス（GME_BOSS5_ARMPART_IDX_XXX）
  @param quat			[in]	パーツの姿勢クォータニオン
  
  @note
  指定パーツを指定のクォータニオンの姿勢に設定します。
 */
// =======================================================================
void gmBoss5BodySetArmPoseParam(GMS_BOSS5_BODY_WORK *body_work,
								GME_BOSS5_ARM_TYPE arm_type,
								GME_BOSS5_ARMPART_IDX arm_part_idx,
								const NNS_QUATERNION *quat)
{
	body_work->arm_part_rot_quat[arm_type][arm_part_idx]	= *quat;
}

// =======================================================================
// gmBoss5BodyApplyArmPose
/*!
  腕姿勢適用
  
  @param body_work	[io]	本体ワーク
  
  @note
  設定されたパラメータをオブジェクトの各パラメータに反映します。
 */
// =======================================================================
void gmBoss5BodyApplyArmPose(GMS_BOSS5_BODY_WORK *body_work)
{
	NNS_MATRIX	rot_mtx[GME_BOSS5_ARM_TYPE_MAX][GME_BOSS5_ARMPART_IDX_MAX];
	
	// CNM操作
	for (Sint32 arm_type = 0; arm_type < GME_BOSS5_ARM_TYPE_MAX; ++arm_type) {
		for (Sint32 part_type = 0; part_type < GME_BOSS5_ARMPART_IDX_MAX; ++part_type) {
			
			// 各パーツの回転行列取得
			nnMakeQuaternionMatrix(&rot_mtx[arm_type][part_type],
								   &body_work->arm_part_rot_quat[arm_type][part_type]);
			
			// CNM書き込みマトリクス設定
			GmBsCmnSetCNMMtx(&body_work->cnm_mgr_work,
							 &rot_mtx[arm_type][part_type],
							 body_work->arm_cnm_reg_id[arm_type][part_type]);
		}
	}
	
	// ロケット配置オフセットマトリクスを計算
	for (Sint32 arm_type = 0; arm_type < GME_BOSS5_ARM_TYPE_MAX; ++arm_type) {
		const NNS_MATRIX	*shld_mtx;	// 肩
		const NNS_MATRIX	*elb_mtx;	// 肘
		const NNS_MATRIX	*fa_mtx;	// 前腕
		NNS_MATRIX	inv_shld_mtx;
		NNS_MATRIX	inv_elb_mtx;
		NNS_MATRIX	inv_fa_mtx;
		NNS_MATRIX	rkt_ofst_mtx;
		
		// 各パーツのノードマトリクスを得る
		shld_mtx	= GmBsCmnGetSNMMtx(&body_work->snm_work, body_work->armpt_snm_reg_ids[arm_type][GME_BOSS5_ARMPART_IDX_SHOULDER]);
		elb_mtx		= GmBsCmnGetSNMMtx(&body_work->snm_work, body_work->armpt_snm_reg_ids[arm_type][GME_BOSS5_ARMPART_IDX_ELBOW]);
		fa_mtx	= GmBsCmnGetSNMMtx(&body_work->snm_work, body_work->armpt_snm_reg_ids[arm_type][GME_BOSS5_ARMPART_IDX_FOREARM]);
		
		// 各種逆行列を得る
		nnInvertMatrix(&inv_shld_mtx, shld_mtx);
		nnInvertMatrix(&inv_elb_mtx, elb_mtx);
		nnInvertMatrix(&inv_fa_mtx, fa_mtx);
		
		// ロケット接続ノードの、現在の姿勢からCNM操作後の姿勢への差分マトリクスを得る
		// inv_fa_mtx * shld_mtx * rot_mtx[SHOULDER] * inv_shld_mtx * elb_mtx * rot_mtx[ELBOW] * inv_elb_mtx * fa_mtx, rot_mtx[FOREARM]
		nnMultiplyMatrix(&rkt_ofst_mtx, &inv_fa_mtx, shld_mtx);
		nnMultiplyMatrix(&rkt_ofst_mtx, &rkt_ofst_mtx, &rot_mtx[arm_type][GME_BOSS5_ARMPART_IDX_SHOULDER]);
		nnMultiplyMatrix(&rkt_ofst_mtx, &rkt_ofst_mtx, &inv_shld_mtx);
		nnMultiplyMatrix(&rkt_ofst_mtx, &rkt_ofst_mtx, elb_mtx);
		nnMultiplyMatrix(&rkt_ofst_mtx, &rkt_ofst_mtx, &rot_mtx[arm_type][GME_BOSS5_ARMPART_IDX_ELBOW]);
		nnMultiplyMatrix(&rkt_ofst_mtx, &rkt_ofst_mtx, &inv_elb_mtx);
		nnMultiplyMatrix(&rkt_ofst_mtx, &rkt_ofst_mtx, fa_mtx);
		nnMultiplyMatrix(&rkt_ofst_mtx, &rkt_ofst_mtx, &rot_mtx[arm_type][GME_BOSS5_ARMPART_IDX_FOREARM]);
		
		nnCopyMatrix(&body_work->rkt_ofst_mtx[arm_type], &rkt_ofst_mtx);
	}
}

// =======================================================================
// gmBoss5BodyInitCanopyPartsPose
/*!
  キャノピー操作パーツ姿勢操作初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  キャノピーの姿勢操作が可能となるように対象ノードのCNM有効化
 */
// =======================================================================
static void gmBoss5BodyInitCanopyPartsPose(GMS_BOSS5_BODY_WORK *body_work)
{
	NNS_MATRIX	unit_mtx;
	const Sint32	cnm_reg_id_tbl[]	= {
		body_work->head_cnm_reg_id,
		body_work->neck_cnm_reg_id,
		body_work->cover_cnm_reg_id,
		body_work->pole_cnm_reg_id,
	};
	const Sint32	elem_num	= sizeof(cnm_reg_id_tbl) / sizeof(cnm_reg_id_tbl[0]);
	
	nnMakeUnitMatrix(&unit_mtx);
	
	for (Sint32 i = 0; i < elem_num; ++i) {
		// CNM 左乗算モードに変更
		GmBsCmnChangeCNMModeNode(&body_work->cnm_mgr_work,
								 cnm_reg_id_tbl[i],
								 GME_BS_CMN_CNM_MODE_MULT_LEFT);
		// CNM ノードローカル座標系でマトリクス設定
		GmBsCmnEnableCNMLocalCoordinate(&body_work->cnm_mgr_work,
										cnm_reg_id_tbl[i],
										TRUE);
		// CNM 適用有効化
		GmBsCmnEnableCNMMtxNode(&body_work->cnm_mgr_work,
								cnm_reg_id_tbl[i],
								TRUE);
		// 設定マトリクス初期化
		GmBsCmnSetCNMMtx(&body_work->cnm_mgr_work,
						 &unit_mtx,
						 cnm_reg_id_tbl[i]);
	}
}

// =======================================================================
// gmBoss5BodyEndCanopyPartsPose
/*!
  キャノピー操作パーツ姿勢操作終了
  
  @param body_work	[io]	本体ワーク
  
  @note
  姿勢操作対象パーツのCNMの初期化などを行い、キャノピーの姿勢操作を終了します。
  姿勢操作を使用しない場合は必ずこの関数を呼んで終了してください。
 */
// =======================================================================
static void gmBoss5BodyEndCanopyPartsPose(GMS_BOSS5_BODY_WORK *body_work)
{
	const Sint32	cnm_reg_id_tbl[]	= {
		body_work->head_cnm_reg_id,
		body_work->neck_cnm_reg_id,
		body_work->cover_cnm_reg_id,
		body_work->pole_cnm_reg_id,
	};
	const Sint32	elem_num	= sizeof(cnm_reg_id_tbl) / sizeof(cnm_reg_id_tbl[0]);
	
	for (Sint32 i = 0; i < elem_num; ++i) {
		// CNM適用無効化
		GmBsCmnEnableCNMMtxNode(&body_work->cnm_mgr_work,
								cnm_reg_id_tbl[i],
								FALSE);
	}
}

// =======================================================================
// gmBoss5BodyTryTransitCrashWall
/*!
  壁衝突試行
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	サブシーケンス遷移成功
  @retval FALSE	サブシーケンス遷移しなかった
  
  @note
  壁衝突チェックを行い、衝突したら壁衝突サーブシーケンスを開始します。
 */
// =======================================================================
BOOL gmBoss5BodyTryTransitCrashWall(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// サブシーケンスが設定されていないときのみ処理
	if (body_work->sub_seq == GME_BOSS5_BODY_SUB_SEQ_NONE) {
		
		if (body_work->state == GME_BOSS5_BODY_STATE_MOVE_FAST_FWD) {
			if (obj_work->move_flag & OBD_MOVE_FRONT) {
				// 前進移動中に前から壁に当たった場合
				gmBoss5BodyStartSubsequence(body_work, GME_BOSS5_BODY_SUB_SEQ_MOVE_FAST_FWD_CRASH);
				return TRUE;
			}
		}
		else if (body_work->state == GME_BOSS5_BODY_STATE_MOVE_FAST_BWD) {
			if (obj_work->move_flag & OBD_MOVE_BACK) {
				// 後退移動中に後ろから壁に当たった場合
				gmBoss5BodyStartSubsequence(body_work, GME_BOSS5_BODY_SUB_SEQ_MOVE_FAST_BWD_CRASH);
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

// =======================================================================
// gmBoss5BodyResumeMoveFast
/*!
  高速移動復帰
  
  @param body_work	[io]	本体ワーク
  
  @note
  壁衝突状態から高速移動への復帰を行います。
 */
// =======================================================================
void gmBoss5BodyResumeMoveFast(GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->sub_seq == GME_BOSS5_BODY_SUB_SEQ_MOVE_FAST_FWD_CRASH) {
		// 前から壁に当たってよろめいていた場合
		gmBoss5BodyChangeState(body_work,
							   GME_BOSS5_BODY_STATE_MOVE_FAST_BWD,
							   body_work->strat_state,	// 戦略ステートは変えない
							   TRUE);
	}
	else {
		MTM_ASSERT(body_work->sub_seq == GME_BOSS5_BODY_SUB_SEQ_MOVE_FAST_BWD_CRASH);
		// 背中から壁に当たってよろめいていた場合
		gmBoss5BodyChangeState(body_work,
							   GME_BOSS5_BODY_STATE_MOVE_FAST_FWD,
							   body_work->strat_state,	// 戦略ステートは変えない
							   TRUE);
	}
}

// =======================================================================
// gmBoss5BodyReceiveSignalRocketReturned
/*!
  ロケット帰還完了通知 受信処理
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	受信した
  @retval FALSE	受信していない
 */
// =======================================================================
inline BOOL gmBoss5BodyReceiveSignalRocketReturned(GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_L) {
		MTM_ASSERT(!(body_work->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_R));
		body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_L;
		
		return TRUE;
	}
	else if (body_work->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_R) {
		MTM_ASSERT(!(body_work->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_L));
		body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_R;
		
		return TRUE;
	}
	
	return FALSE;
}



// ============================================================================
// ノード処理関連
// ============================================================================
// =======================================================================
// gmBoss5BodyInitCallbacks
/*!
  ノードコールバック関連初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  初期ステートを設定するより前に呼び出してください。
 */
// =======================================================================
void gmBoss5BodyInitCallbacks(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	MTM_ASSERT(body_work);
	MTM_ASSERT(obj_work->obj_3d);
	
	const static Sint32 arm_part_node_ids[GME_BOSS5_ARM_TYPE_MAX][GME_BOSS5_ARMPART_IDX_MAX]	= {
		// 左腕
		{	GMD_BOSS5_BODY_NODE_IDX_SHOULDER_L, /* 左肩 */
			GMD_BOSS5_BODY_NODE_IDX_ELBOW_L,	/* 左肘 */
			GMD_BOSS5_BODY_NODE_IDX_FOREARM_L,	/* 左前腕 */
		},
		// 右腕
		{
			GMD_BOSS5_BODY_NODE_IDX_SHOULDER_R,	/* 右肩 */
			GMD_BOSS5_BODY_NODE_IDX_ELBOW_R,	/* 右肘 */
			GMD_BOSS5_BODY_NODE_IDX_FOREARM_R,	/* 右前腕 */
		},
	};
	
	// BMCBシステム初期化
	GmBsCmnInitBossMotionCBSystem(obj_work,
								  &body_work->bmcb_mgr);
	
	// ノードマトリクス取得初期化
	GmBsCmnCreateSNMWork(&body_work->snm_work,
						 obj_work->obj_3d->object,
						 GMD_BOSS5_BODY_NODE_SNM_NUM);
	// モーションコールバックを実行リストに追加
	GmBsCmnAppendBossMotionCallback(&body_work->bmcb_mgr,
									&body_work->snm_work.bmcb_link);
	
	// ノードマトリクス取得ノード追加
	body_work->body_snm_reg_id	=
		GmBsCmnRegisterSNMNode(&body_work->snm_work,
							   GMD_BOSS5_BODY_NODE_IDX_BODY);
	body_work->lfoot_snm_reg_id	=
		GmBsCmnRegisterSNMNode(&body_work->snm_work,
							   GMD_BOSS5_BODY_NODE_IDX_FOOT_L);
	body_work->rfoot_snm_reg_id	=
		GmBsCmnRegisterSNMNode(&body_work->snm_work,
							   GMD_BOSS5_BODY_NODE_IDX_FOOT_R);
	
	body_work->leg_snm_reg_ids[GME_BOSS5_LEG_TYPE_LEFT]	=
		GmBsCmnRegisterSNMNode(&body_work->snm_work,
							   GMD_BOSS5_BODY_NODE_IDX_LEG_L);
	body_work->leg_snm_reg_ids[GME_BOSS5_LEG_TYPE_RIGHT]	=
		GmBsCmnRegisterSNMNode(&body_work->snm_work,
							   GMD_BOSS5_BODY_NODE_IDX_LEG_R);
	
	body_work->pole_snm_reg_id	=
		GmBsCmnRegisterSNMNode(&body_work->snm_work,
							   GMD_BOSS5_BODY_NODE_IDX_POLE);
	
	body_work->groin_snm_reg_ids[GME_BOSS5_LEG_TYPE_LEFT]	=
		GmBsCmnRegisterSNMNode(&body_work->snm_work,
							   GMD_BOSS5_BODY_NODE_IDX_GROIN_L);
	body_work->groin_snm_reg_ids[GME_BOSS5_LEG_TYPE_RIGHT]	=
		GmBsCmnRegisterSNMNode(&body_work->snm_work,
							   GMD_BOSS5_BODY_NODE_IDX_GROIN_R);
	
	body_work->nozzle_snm_reg_ids[GME_BOSS5_NOZZLE_TYPE_LEFT]	=
		GmBsCmnRegisterSNMNode(&body_work->snm_work,
							   GMD_BOSS5_BODY_NODE_IDX_NOZZLE_L);
	body_work->nozzle_snm_reg_ids[GME_BOSS5_NOZZLE_TYPE_RIGHT]	=
		GmBsCmnRegisterSNMNode(&body_work->snm_work,
							   GMD_BOSS5_BODY_NODE_IDX_NOZZLE_R);
	
	for (Sint32 i = 0; i < GME_BOSS5_ARM_TYPE_MAX; ++i) {
		for (Sint32 j = 0; j < GME_BOSS5_ARMPART_IDX_MAX; ++j) {
			body_work->armpt_snm_reg_ids[i][j]	=
				GmBsCmnRegisterSNMNode(&body_work->snm_work, arm_part_node_ids[i][j]);
		}
	}
	
	// ノードマトリクス操作処理管理ワーク初期化
	GmBsCmnCreateCNMMgrWork(&body_work->cnm_mgr_work,
							obj_work->obj_3d->object,
							GMD_BOSS5_BODY_NODE_CNM_NUM);
	
	// ノードマトリクス操作コールバック初期化
	GmBsCmnInitCNMCb(obj_work, &body_work->cnm_mgr_work);
	
	// ノードマトリクス操作ノード追加
	
	for (Sint32 i = 0; i < GME_BOSS5_ARM_TYPE_MAX; ++i) {
		for (Sint32 j = 0; j < GME_BOSS5_ARMPART_IDX_MAX; ++j) {
			body_work->arm_cnm_reg_id[i][j]	=
				GmBsCmnRegisterCNMNode(&body_work->cnm_mgr_work, arm_part_node_ids[i][j]);
		}
	}
	
	body_work->head_cnm_reg_id	=
		GmBsCmnRegisterCNMNode(&body_work->cnm_mgr_work,
							   GMD_BOSS5_BODY_NODE_IDX_HEAD);
	
	body_work->neck_cnm_reg_id	=
		GmBsCmnRegisterCNMNode(&body_work->cnm_mgr_work,
							   GMD_BOSS5_BODY_NODE_IDX_NECK);
	
	body_work->cover_cnm_reg_id	=
		GmBsCmnRegisterCNMNode(&body_work->cnm_mgr_work,
							   GMD_BOSS5_BODY_NODE_IDX_COVER);
	body_work->pole_cnm_reg_id	=
		GmBsCmnRegisterCNMNode(&body_work->cnm_mgr_work,
							   GMD_BOSS5_BODY_NODE_IDX_POLE);
}

// =======================================================================
// gmBoss5BodyReleaseCallbacks
/*!
  ノードコールバック関連解放
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyReleaseCallbacks(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// ボスモーションコールバックシステム解除・クリア
	GmBsCmnClearBossMotionCBSystem(obj_work);
	
	// ノードマトリクス取得関連解放
	GmBsCmnDeleteSNMWork(&body_work->snm_work);
	
	// ノードマトリクス操作解除・クリア
	GmBsCmnClearCNMCb(obj_work);
	
	// ノードマトリクス操作処理管理ワーク削除
	GmBsCmnDeleteCNMMgrWork(&body_work->cnm_mgr_work);
}

// =======================================================================
// gmBoss5BodyRegisterScatterPartsCNM
/*!
  本体パーツ飛散用CNMノード登録
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyRegisterScatterPartsCNM(GMS_BOSS5_BODY_WORK *body_work)
{
	for (Sint32 i = 0; i < GME_BOSS5_SCT_PART_IDX_MAX; ++i) {
		body_work->scatter_cnm_reg_ids[i]	=
			GmBsCmnRegisterCNMNode(&body_work->cnm_mgr_work,
								   gm_boss5_body_scatter_parts_cnm_node_id_tbl[i]);
	}
}


// ============================================================================
// 処理関数
// ============================================================================
// =======================================================================
// gmBoss5BodyDamageDefFunc
/*!
  本体 プレイヤー攻撃ヒット時くらい処理関数
  
  @param my_rect	[io]	自分の矩形
  @parma your_rect	[io]	相手の矩形
 */
// =======================================================================
void gmBoss5BodyDamageDefFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{
	OBS_OBJECT_WORK	*my_obj		= my_rect->parent_obj;
	OBS_OBJECT_WORK	*your_obj	= your_rect->parent_obj;
	GMS_BOSS5_BODY_WORK	*body_work	= (GMS_BOSS5_BODY_WORK*)my_obj;
	//GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)my_obj;
	BOOL	is_targ_player	= FALSE;
	BOOL	is_targ_rkt	= FALSE;
	
	// 相手の種類を判定
	if (your_obj) {
		if (GMD_OBJTYPE_PLAYER == your_obj->obj_type) {
			is_targ_player	= TRUE;
		}
		else if (GMD_OBJTYPE_ENEMY == your_obj->obj_type) {
			GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)your_obj;
			
			if (ene_com->eve_rec->id == GMD_EVENT_ID_BOSS5_ROCKET) {
				is_targ_rkt	= TRUE;
			}
		}
	}
	
	if (is_targ_player || is_targ_rkt) {
		
		if (is_targ_player) {
			GMS_PLAYER_WORK	*ply_work	= (GMS_PLAYER_WORK*)your_obj;
			
			// プレイヤー跳ね返り
			gmBoss5BodySetPlyRebound(ply_work, body_work);
			
			// 振動小
			GMM_PAD_VIB_SMALL_TIME(30);
		}
		else {
			MTM_ASSERT(is_targ_rkt);
			
			// のけぞりサブシーケンスに移行
			gmBoss5BodyStartSubsequence(body_work, GME_BOSS5_BODY_SUB_SEQ_RPUNCH_STR_DMG);
			
			// 振動小
			GMM_PAD_VIB_SMALL();
			
			// ロケットパンチヒットSE再生
			GmSoundPlaySE("FinalBoss15");
		}
		
		
		// ヒット無効時間設定
		gmBoss5BodySetNoHitTime(body_work);
		
		// ダメージSE再生
		GmSoundPlaySE("Boss0_01");
		
		// ダメージエフェクト生成
		GmBoss5EfctCreateDamage(body_work);
		
		if (!(body_work->flag & GMD_BOSS5_BODY_FLAG_INVINCIBLE) ||
			is_targ_rkt) {
			// 無敵ではないとき、もしくは無敵状態に関わらず
			// 相手がロケットだった場合ダメージ処理
			
			// ダメージ処理
			gmBoss5BodyExecDamageRoutine(body_work);
		}
	}
}


// =======================================================================
// gmBoss5BodyOutFunc
/*!
  本体 専用描画関数
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  通常の描画に加えてノード操作処理も行っています。
 */
// =======================================================================
void gmBoss5BodyOutFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*body_work	= (GMS_BOSS5_BODY_WORK*)obj_work;
	
	// ノード操作更新
	GmBsCmnUpdateCNMParam(obj_work, &body_work->cnm_mgr_work);
	
	// 標準描画関数
	ObjDrawActionSummary(obj_work);
	
	// 接地移動基準座標格納（SNM結果格納時と同じタイミングの値となるようにここで格納する）
	body_work->grdmv_pivot_pos	= obj_work->pos;
	
	// ノード・矩形追随用基準座標格納（SNM結果格納時と同じタイミングの値となるようにここで格納する）
	body_work->pivot_prev_pos	= obj_work->pos;
}

// =======================================================================
// gmBoss5BodyRecFunc
/*!
  本体 専用矩形登録処理
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  本体の追加の矩形登録処理を行っています。
 */
// =======================================================================
void gmBoss5BodyRecFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*body_work	= (GMS_BOSS5_BODY_WORK*)obj_work;
	
	// 追加矩形登録
	for (Sint32 sub_rp_idx = 0; sub_rp_idx < GME_BOSS5_BODY_RECTPOINT_SUB_NUM; ++sub_rp_idx) {
		for (Sint32 i = 0; i < GMD_ENEMY_RECT_NUM; ++i) {
			ObjObjectRectRegist(obj_work,
								&body_work->sub_rect_work[sub_rp_idx][i]);
		}
	}
}

// ============================================================================
// 制御処理
// ============================================================================
// =======================================================================
// gmBoss5BodyChangeState
/*!
  本体ステート変更
 
  @param body_work		[io]	本体ワーク
  @param state			[in]	遷移先ステート
  @param strat_state	[in]	戦略ステート
  
  @note
  前ステートの終了関数呼び出しと、遷移先ステートの開始関数呼び出しを行います。
 */
// =======================================================================
void gmBoss5BodyChangeState(GMS_BOSS5_BODY_WORK *body_work,
							GME_BOSS5_BODY_STATE state,
							GME_BOSS5_STRAT_STATE strat_state,
							BOOL is_wrapped/*=FALSE*/)
{
	UNREFERENCED_PARAMETER(is_wrapped);
	const GMS_BOSS5_BODY_STATE_ENTER_INFO	*enter_info;
	GMF_BOSS5_BODY_STATE_LEAVE_FUNC	leave_func;
	
	// 前ステート終了処理
	leave_func	= gm_boss5_body_state_leave_func_tbl[body_work->state];
	if (leave_func) {
		leave_func(body_work);
	}
	
	// ステート設定
	body_work->prev_state	= body_work->state;
	body_work->state		= state;
	
	// サブシーケンスクリア
	body_work->sub_seq	= GME_BOSS5_BODY_SUB_SEQ_NONE;
	
	// 戦略ステート設定
	body_work->strat_state	= strat_state;
	
	// 次ステート開始処理
	enter_info	= &gm_boss5_body_state_enter_info_tbl[body_work->state];
#if defined(MTD_DEBUG)	// 間接呼び出しチェック
	if (enter_info->is_wrapped) {
		MTM_ASSERT(is_wrapped != FALSE);
	}
	else {
		MTM_ASSERT(is_wrapped == FALSE);
	}
#endif /* defined(MTD_DEBUG) */
	if (enter_info->enter_func) {
		enter_info->enter_func(body_work);
	}
}

// =======================================================================
// gmBoss5BodyStartSubsequence
/*!
  本体 サブシーケンス初期化
  
  @param body_work	[io]	本体ワーク
  @param sub_seq	[in]	サブシーケンスインデックス
  
  @note
  ステートを変更せずにシーケンスの分岐を行います。
  各サブシーケンスは対応付けされているステート中にのみ呼び出し可能です。
  サブシーケンスからステート変更した場合も、
  ステートのLeave関数が呼ばれることに注意してください。
 */
// =======================================================================
void gmBoss5BodyStartSubsequence(GMS_BOSS5_BODY_WORK *body_work,
								 GME_BOSS5_BODY_SUB_SEQ sub_seq)
{
	const GMS_BOSS5_BODY_SUBSEQ_ENTER_INFO	*enter_info;
	
	// 開始情報取得
	enter_info	= &gm_boss5_body_sub_seq_enter_func_tbl[sub_seq];
	
	// 上位ステートチェック
	MTM_ASSERT(body_work->state == enter_info->super_state);
	
	// サブシーケンス種別を格納
	body_work->sub_seq	= sub_seq;
	
	// サブシーケンス開始処理
	if (enter_info->enter_func) {
		enter_info->enter_func(body_work);
	}
}

// =======================================================================
// gmBoss5BodyWaitSetup
/*!
  本体 セットアップ完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5BodyWaitSetup(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*body_work	= (GMS_BOSS5_BODY_WORK*)obj_work;
	
	// 全ての構成パーツのセットアップを待つ
	if (body_work->mgr_work->flag & GMD_BOSS5_MGR_FLAG_SETUP_END) {
		// コールバック関係初期化
		gmBoss5BodyInitCallbacks(body_work);
		
		// 腕生成
		GmBoss5RocketSpawnConnected(body_work, GME_BOSS5_RKT_TYPE_LEFT);
		GmBoss5RocketSpawnConnected(body_work, GME_BOSS5_RKT_TYPE_RIGHT);
		
		obj_work->ppFunc	= gmBoss5BodyMain;
		
		// 開始ステート設定
		gmBoss5BodyChangeState(body_work, GME_BOSS5_BODY_STATE_START, GME_BOSS5_STRAT_STATE_START);
	}
}

// =======================================================================
// gmBoss5BodyMain
/*!
  本体 メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5BodyMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*body_work	= (GMS_BOSS5_BODY_WORK*)obj_work;
	BOOL	is_transition	= FALSE;
	
	// 腕突き出し発動期限タイマ更新
	gmBoss5BodyUpdatePokeTriggerLimitTime(body_work);
	
	// ヒット無効時間更新
	gmBoss5BodyUpdateNoHitTime(body_work);
	
	
	// 高速移動継続時間更新（壁衝突動作中はカウントしない）
	if (!(body_work->sub_seq == GME_BOSS5_BODY_SUB_SEQ_MOVE_FAST_FWD_CRASH ||
		  body_work->sub_seq == GME_BOSS5_BODY_SUB_SEQ_MOVE_FAST_BWD_CRASH)) {
		gmBoss5BodyUpdateMoveFastTime(body_work);
	}
	// 地球割りが成功していない場合に限り、シーケンス・サブシーケンス遷移処理を行う
	if (!(body_work->flag & GMD_BOSS5_BODY_FLAG_CRASH_SUCCESS)) {
		
		// 撃破演出開始チェック
		if (body_work->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_B2B_DEFEAT) {
			
			body_work->flag	&= ~(GMD_BOSS5_BODY_FLAG_SIGNAL_B2B_DEFEAT |
								 GMD_BOSS5_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE);
			// ↑ダメージシグナルは無視させる
			
			// ダメージ点滅表示
			GmBsCmnInitObject3DNNDamageFlicker(obj_work, &body_work->flk_work,
											   GMD_BOSS5_BODY_DMG_FLICKER_RADIUS);
			
			// 撃破ステートへ
			gmBoss5BodyProceedToDefeatState(body_work);
			
			// ステート遷移発生
			is_transition	= TRUE;
			
			// 漏電エフェクト生成チェックも念のため行うので、ここでreturnしない
		}
		
		if (!is_transition) {	// ←撃破演出を優先させる
			// 壁衝突チェック＆遷移（条件によりサブシーケンス遷移が発生）
			if (gmBoss5BodyTryTransitCrashWall(body_work)) {
				// サブシーケンス遷移発生
				is_transition	= TRUE;
				
				// 壁衝突SE再生
				GmSoundPlaySE("FinalBoss12");
			}
		}
	}
	
	/* 上記でサブシーケンス変更している場合があるので、これ以降の処理に注意 */
	
	// ステート・サブシーケンス遷移が発生していないときだけ更新処理を行う
	// （遷移処理によって行われる処理が1フレ分の更新処理に相当するので、
	//   さらに1フレ分の処理を行わないようにするため）
	if (!is_transition) {
		// 更新処理
		if (body_work->proc_update) {
			body_work->proc_update(body_work);
		}
	}
	
	// ダメージ演出開始チェック
	if (body_work->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE) {
		body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE;
		
		// ダメージ点滅表示
		GmBsCmnInitObject3DNNDamageFlicker(obj_work, &body_work->flk_work,
										   GMD_BOSS5_BODY_DMG_FLICKER_RADIUS);
	}
	
	// ダメージ点滅更新
	GmBsCmnUpdateObject3DNNDamageFlicker(obj_work, &body_work->flk_work);
	
	// 漏電エフェクト生成チェック
	if (body_work->flag & GMD_BOSS5_BODY_FLAG_LEAKAGE_NEEDED) {
		GmBoss5EfctTryStartLeakage(body_work);
	}
	else {
		GmBoss5EfctEndLeakage(body_work);	// 漏電エフェクト終了
	}
}

// ============================================================================
// シーケンス
// ============================================================================

// =======================================================================
// gmBoss5BodyState{Enter|Leave|Update}StartXXX
/*!
  本体 開始ステート遷移時処理関数
 */
// =======================================================================
// 開始ステート開始関数
void gmBoss5BodyStateEnterStart(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 開始時モーション設定
	gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_START, TRUE);
	
	// 左を向く
	gmBoss5BodySetDirection(body_work, GME_BOSS5_BODY_DIRECTION_TYPE_LEFT);
	
	// フラグ設定
	obj_work->move_flag	&= ~OBD_MOVE_NOCOL;	// 配置用に地形判定する
	obj_work->move_flag	|= OBD_MOVE_FALL;	// 配置用に落下
	
	// ホーミングアタックの対象外にする
	body_work->flag	|= GMD_BOSS5_BODY_FLAG_DENY_HOMING_ATK;
	
	// 設置用の落下速度設定
	obj_work->spd.y	= obj_work->spd_fall_max;
	
	// 座標設定
	obj_work->pos.z	= GMD_BOSS5_BG_FARSIDE_POS_Z;
	
	// スタート用矩形設定
	gmBoss5BodyChangeRectSetting(body_work, GME_BOSS5_BODY_RECT_SETTING_START);
	
	// エッグマン生成
	GmBoss5EggCreate(body_work,
					 obj_work->pos.x,
					 obj_work->pos.y);
	
	// 処理関数設定
	body_work->proc_update	= gmBoss5BodyStateUpdateStartWithPlacement;
}

// 開始ステート終了関数
void gmBoss5BodyStateLeaveStart(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 画面アスペクト比4:3用のカメラスライドを元に戻す
	gmBoss5RestoreCameraSlideForNarrowScreen(body_work->mgr_work);
	
	// キャノピーパーツ姿勢操作終了
	gmBoss5BodyEndCanopyPartsPose(body_work);
	
	// 矩形設定をデフォルトに戻す
	gmBoss5BodyChangeRectSettingDefault(body_work);
	
	// フラグクリア
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_DENY_HOMING_ATK;
	
	// フラグもどす
	obj_work->move_flag	&= ~OBD_MOVE_NOCOL;
	obj_work->move_flag	|= OBD_MOVE_FALL;
	
	// 座標戻す
	obj_work->pos.z	= GMD_BOSS5_DEFAULT_POS_Z;
}

// 開始ステート更新 配置処理
void gmBoss5BodyStateUpdateStartWithPlacement(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (obj_work->move_flag & OBD_MOVE_UNDER) {
		
		// 設置した時点の座標を地面の高さとして記憶
		body_work->ground_v_pos	= obj_work->pos.y;
		
		// 地面埋める用にフラグ設定
		obj_work->move_flag	|= OBD_MOVE_NOCOL;
		obj_work->move_flag	&= ~OBD_MOVE_FALL;
		
		// 地面に埋める
		obj_work->pos.y	+= GMD_BOSS5_BODY_START_BURY_HEIGHT;
		
		// カタパルト生成
		GmBoss5CtpltCreate(body_work);
		
		// 配置完了を管理に通知
		body_work->mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_BODY_STOOD_BY;
		// プレイヤーの演出走りの目標地点を設定しておく
		gmBoss5MgrSetDemoRunDestPos(body_work->mgr_work,
									obj_work->pos.x + GMD_BOSS5_PLY_OP_DEMO_RUN_DEST_X_OFST_FROM_BODY);
		
		// キャノピーパーツ姿勢操作開始
		gmBoss5BodyInitCanopyPartsPose(body_work);
		
		// キャノピークローズ処理初期化
		gmBoss5BodyInitCloseCanopy(body_work);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateStartWithWaitEggRide;
	}
}

// 開始ステート更新 エッグマン搭乗待ち
void gmBoss5BodyStateUpdateStartWithWaitEggRide(GMS_BOSS5_BODY_WORK *body_work)
{
	// 4:3画面用カメラスライド開始チェック
	if ((body_work->mgr_work->flag & GMD_BOSS5_MGR_FLAG_CAMERA_SLIDE_NEEDED) &&
		!(body_work->mgr_work->flag & GMD_BOSS5_MGR_FLAG_CAMERA_SLIDE_STARTED)) {
		body_work->mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_CAMERA_SLIDE_STARTED;
		gmBoss5SetCameraSlideForNarrowScreen(body_work->mgr_work);
	}
	
	// キャノピークローズ処理更新
	gmBoss5BodyUpdateCloseCanopy(body_work, FALSE);	// 開けっ放しにしておく
	
	if (body_work->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_E2B_EGG_GOT_IN) {
		body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_SIGNAL_E2B_EGG_GOT_IN;
		
		// FWへのモーションブレンド時の座標ズレ修正用に姿勢情報を記録
		MTM_ASSERT(GMM_BS_OBJ(body_work)->obj_3d->act_id[0] == IDB_BOSS05_BODY_MTN_B05_1_ATT01_01B_ZNM);
		gmBoss5BodyRecordGapAdjustmentDest(body_work);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateStartWithCockpitClose;
	}
}

// 開始ステート更新 コックピットクローズ
void gmBoss5BodyStateUpdateStartWithCockpitClose(GMS_BOSS5_BODY_WORK *body_work)
{
	// コックピットクローズ完了待ち
	if (gmBoss5BodyUpdateCloseCanopy(body_work, TRUE)) {
		
		// ハッチ閉めSE再生
		GmSoundPlaySE("FinalBoss01");
		
		// 本体側の開始デモ処理終了
		body_work->mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_BODY_DEMO_IS_FINISHED;
		
		// せり上がり開始待ち時間設定
		body_work->wait_timer	= GMD_BOSS5_BODY_START_WAIT_RISE_TIME;
		
		body_work->proc_update	= gmBoss5BodyStateUpdateStartWithWaitRise;
	}
}

// 開始ステート更新 せり上がり開始待ち
void gmBoss5BodyStateUpdateStartWithWaitRise(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 既定時間待機
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		
		// せり上がり速度設定
		obj_work->spd.y	= GMD_BOSS5_BODY_START_RISE_SPD_Y;
		
		// せり上がり振動初期化
		gmBoss5BodyInitStartRiseVib(body_work);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateStartWithRise;
	}
}

// 開始ステート更新 せり上がり処理
void gmBoss5BodyStateUpdateStartWithRise(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	GMS_BOSS5_MGR_WORK	*mgr_work	= body_work->mgr_work;
	
	// せり上がり振動更新
	gmBoss5BodyUpdateStartRiseVib(body_work);
	
	// せり上がり完了待ち
	if (obj_work->pos.y <= body_work->ground_v_pos) {
		
		GmBsCmnSetObjSpdZero(obj_work);
		
		obj_work->move_flag	&= ~OBD_MOVE_NOCOL;
		obj_work->move_flag	|= OBD_MOVE_FALL;
		
		// カタパルト収納要求
		mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_CTPLT_STORE_NEEDED;
		
		body_work->proc_update	= gmBoss5BodyStateUpdateStartWithWaitCtplt;
	}
}

// 開始ステート更新 カタパルト収納待ち
void gmBoss5BodyStateUpdateStartWithWaitCtplt(GMS_BOSS5_BODY_WORK *body_work)
{
	GMS_BOSS5_MGR_WORK	*mgr_work	= body_work->mgr_work;
	
	if (mgr_work->flag & GMD_BOSS5_MGR_FLAG_CTPLT_IS_STORED) {
		
		// 画面アスペクト比4:3用のカメラスライドを元に戻す
		gmBoss5RestoreCameraSlideForNarrowScreen(body_work->mgr_work);
		
		// 終了待ち時間設定
		body_work->wait_timer	= GMD_BOSS5_BODY_START_WAIT_END_TIME;
		
		body_work->proc_update	= gmBoss5BodyStateUpdateStartWithWaitEnd;
	}
}

// 開始ステート更新 終了待ち
void gmBoss5BodyStateUpdateStartWithWaitEnd(GMS_BOSS5_BODY_WORK *body_work)
{
	// 既定時間待機（動き出すまでの待ち時間）
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		gmBoss5BodyProceedToNextSeqNml(body_work);
	}
}

// =======================================================================
// gmBoss5BodyState{Enter|Leave|Update}MoveNmlXXX
/*!
  本体 通常移動ステート遷移時処理関数
 */
// =======================================================================
// 通常移動ステート 開始関数
void gmBoss5BodyStateEnterMoveNml(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 歩きアクション設定
	gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_NML, TRUE);
	
	// 歩行処理初期化
	gmBoss5BodyInitWalk(body_work);
	
	// 歩行終了監視初期化
	gmBoss5BodyInitMonitoringWalkEnd(body_work);
	
	// 歩行足接地時処理 初期化
	gmBoss5BodyInitWalkGroundingEffects(body_work);
	
	// デフォルト矩形設定
	gmBoss5BodyChangeRectSettingDefault(body_work);
	
	// フラグ設定
	obj_work->move_flag	&= ~OBD_MOVE_NOCOL_W;	// 左右壁に当たるようにする
	obj_work->move_flag	|= OBD_MOVE_LIMIT_OUT;	// 左右壁に当たるようにする
	
	// 処理関数設定
	body_work->proc_update	= gmBoss5BodyStateUpdateMoveNmlWithLoop;
	
	// 中断チェック
	if (gmBoss5BodyIsPlayerBehind(body_work)) {
		// 復帰先アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_NML_ABORT);
		// 中断からの復帰処理初期化
		gmBoss5BodyInitWalkAbortRecovery(body_work,
										 body_work->cur_move_phase_type);
		body_work->proc_update	= gmBoss5BodyStateUpdateMoveNmlWithAbort;
	}
}

// 通常移動ステート 終了関数
void gmBoss5BodyStateLeaveMoveNml(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// ブレンド時ズレ補正処理クリア
	gmBoss5BodyClearAdjustMtnBlendHGap(body_work);
	
	// 要求シグナルクリア
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_SIGNAL_B2B_POKE;
	
	// 腕突き出し発動期限タイマクリア
	gmBoss5BodyClearPokeTriggerLimitTime(body_work);
	
	// 腕突き出し処理終了
	gmBoss5BodyEndPoke(body_work);
	
	// フラグ戻す
	obj_work->move_flag	&= ~OBD_MOVE_LIMIT_OUT;
	obj_work->move_flag	|= OBD_MOVE_NOCOL_W;
	
	// デフォルト矩形設定に戻す
	gmBoss5BodyChangeRectSettingDefault(body_work);
}

// 通常移動ステート更新 ループ処理
void gmBoss5BodyStateUpdateMoveNmlWithLoop(GMS_BOSS5_BODY_WORK *body_work)
{
	if (gmBoss5BodyIsPoking(body_work)) {
		if (gmBoss5BodyUpdatePoke(body_work)) {
			gmBoss5BodyEndPoke(body_work);
		}
	}
	
	// 歩行足接地時処理更新
	gmBoss5BodyUpdateWalkGroundingEffects(body_work);
	
	if (gmBoss5BodyUpdateWalk(body_work)) {
		if (GmBsCmnIsActionEnd(GMM_BS_OBJ(body_work)) &&
			!gmBoss5BodyIsPoking(body_work)) {
			gmBoss5BodyProceedToNextSeqNml(body_work);
			return;
		}
		// 最終フェーズに到達しているだけならば以降の処理も行う
	}
	
	// 歩行中断チェック
	// （プレイヤー後方判定は毎フレーム行われますが、
	//   中断してシーケンス移行を開始するのは足が接地したタイミングになります。）
	GME_BOSS5_LEG_TYPE	leg_type;
	if (gmBoss5BodyUpdateMonitoringWalkEnd(body_work, &leg_type)) {
		// 復帰先アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_NML_ABORT);
		// 今接地した足を地面に固定して復帰
		gmBoss5BodyInitWalkAbortRecoveryByLegType(body_work,
												  leg_type);
		body_work->proc_update	= gmBoss5BodyStateUpdateMoveNmlWithAbort;
		return;
	}
	
	if (body_work->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_B2B_POKE) {
		body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_SIGNAL_B2B_POKE;
		gmBoss5BodyInitPoke(body_work);
	}
}

// 通常移動ステート更新 中断処理
void gmBoss5BodyStateUpdateMoveNmlWithAbort(GMS_BOSS5_BODY_WORK *body_work)
{
	if (gmBoss5BodyIsPoking(body_work)) {
		if (gmBoss5BodyUpdatePoke(body_work)) {
			gmBoss5BodyEndPoke(body_work);
		}
	}
	
	// 中断からの復帰処理
	if (gmBoss5BodyUpdateWalkAbortRecovery(body_work) &&
		!gmBoss5BodyIsPoking(body_work)) {
		
		gmBoss5BodyProceedToNextSeqNml(body_work);
		return;
	}
}

// =======================================================================
// gmBoss5BodyState{Enter|Leave|Update}MoveFastXXX
/*!
  本体 高速移動ステート遷移時処理関数
  
  @note
  ステート開始時、および着地時にのみ終了処理へ移行します。
 */
// =======================================================================
// 高速移動ステート 開始関数
void gmBoss5BodyStateEnterMoveFast(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// デフォルト矩形設定
	gmBoss5BodyChangeRectSettingDefault(body_work);
	
	if (gmBoss5BodyIsMoveFastEnd(body_work)) {
		// 高速移動終了（壁激突前にタイマが終了していた場合に遷移）
		
		// 復帰アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_RECOVER);
		
		// FWへのブレンド時のズレ調整用に元姿勢を記録
		gmBoss5BodyRecordGapAdjustmentSrc(body_work);
		
		// モーションブレンドによるズレを補正
		gmBoss5BodyInitAdjustMtnBlendHGap(body_work,
										  body_work->whole_act_id,
										  GME_BOSS5_LEG_TYPE_LEFT);
		gmBoss5BodyUpdateAdjustMtnBlendHGap(body_work);
		
		// 復帰処理タイムアウト時間設定
		body_work->wait_timer	= GMD_BOSS5_BODY_RUN_RECOVER_TIMEOUT_FRAME;
		
		body_work->proc_update	= gmBoss5BodyStateUpdateMoveFastWithRecover;
		return;
	}
	else if (body_work->prev_state == GME_BOSS5_BODY_STATE_BERSERK) {
		// 凶暴化演出シーケンスから遷移してきた場合
		
		// 蹴り上げモーションをそのまま引き継ぐ
		// （次のプロシージャでは直ちにモーション終了と判定される）
		
		// 保険
		if (body_work->whole_act_id != GME_BOSS5_ACT_ID_BERSERK_KICKUP) {
			MTM_ASSERT(FALSE);
			gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_BERSERK_KICKUP);
		}
		
		// ブレンド時ズレ補正処理を行わない
		gmBoss5BodyClearAdjustMtnBlendHGap(body_work);
	}
	else {
		// 通常時or壁激突後の再開時
		
		// 準備アクション設定
		if (gmBoss5BodyIsMoveFastDirFwd(body_work)) {
			gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_FWD_PREP);
		}
		else {
			gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_BWD_PREP);
		}
		
		// 走り準備へのブレンド時のズレ調整用に元姿勢を記録
		gmBoss5BodyRecordGapAdjustmentSrc(body_work);
		
		// モーションブレンドによるズレを補正
		// 走り準備はいずれも右足が動くので左足を基準にする
		gmBoss5BodyInitAdjustMtnBlendHGap(body_work,
										  body_work->whole_act_id,
										  GME_BOSS5_LEG_TYPE_LEFT);
		gmBoss5BodyUpdateAdjustMtnBlendHGap(body_work);
	}
	
	// フラグ設定
	obj_work->move_flag	&= ~OBD_MOVE_NOCOL_W;	// 左右壁に当たるようにする
	obj_work->move_flag	|= OBD_MOVE_LIMIT_OUT;	// 左右壁に当たるようにする
	
	// 右足先行タイプから
	body_work->cur_run_type	= GME_BOSS5_BODY_RUN_TYPE_RIGHT;
	
	// 処理関数設定
	body_work->proc_update	= gmBoss5BodyStateUpdateMoveFastWithPrep;
}

// 高速移動ステート 終了関数
void gmBoss5BodyStateLeaveMoveFast(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// フラグ戻す
	obj_work->move_flag	&= ~OBD_MOVE_JUMP;
	obj_work->move_flag	&= ~OBD_MOVE_LIMIT_OUT;
	obj_work->move_flag	|= OBD_MOVE_NOCOL_W;
	
	// ブレンド時ズレ補正処理クリア
	gmBoss5BodyClearAdjustMtnBlendHGap(body_work);
	
	// デフォルト矩形設定に戻す
	gmBoss5BodyChangeRectSettingDefault(body_work);
}

// 高速移動ステート更新 準備処理
void gmBoss5BodyStateUpdateMoveFastWithPrep(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (obj_work->obj_3d->flag & OBD_ACTFLAG_3D_NN_BLEND) {
		// モーションブレンドによるズレを補正
		gmBoss5BodyUpdateAdjustMtnBlendHGap(body_work);
	}
	else {
		// ズレ補正処理クリア
		gmBoss5BodyClearAdjustMtnBlendHGap(body_work);
	}
	
	// 準備アクション終了待ち
	if (GmBsCmnIsActionEnd(obj_work)) {
		
		// ズレ補正処理クリア
		gmBoss5BodyClearAdjustMtnBlendHGap(body_work);
		
		// ジャンプ動作中の移動速度設定
		obj_work->spd.x	= GMD_BOSS5_BODY_RUN_FWD_JUMP_INIT_SPD_X;
		if (obj_work->disp_flag & OBD_DISP_HFLIP) {
			obj_work->spd.x	= -obj_work->spd.x;
		}
		
		// 右足先行ジャンプアクション設定
		if (gmBoss5BodyIsMoveFastDirFwd(body_work)) {
			gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_FWD_R_JUMP);
		}
		else {
			// 後ろに移動する（速度方向逆転＆係数で速度を抑える）
			obj_work->spd.x	= FX_Mul(-obj_work->spd.x, GMD_BOSS5_BODY_RUN_BWD_SPD_X_FACTOR);
			gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_BWD_R_JUMP);
		}
		
		// 右足だけ攻撃を有効に
		gmBoss5BodySwitchEnableLegRectOneSide(body_work,
											  GME_BOSS5_LEG_TYPE_RIGHT);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateMoveFastWithJump;
	}
}

// 高速移動ステート更新 ジャンプ処理
void gmBoss5BodyStateUpdateMoveFastWithJump(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// ジャンプアクション終了待ち
	// （再生速度をかなり速くしているので、
	// 最終フレームにFIXする処理によって切り落とされるフレーム数が多い場合は
	// 厳格チェックが使われるように、GmBsCmnIsActionEndFlexibly()を使用）
	if (GmBsCmnIsActionEndFlexibly(obj_work, GMD_BOSS5_BODY_RUN_ACT_FRAME_OVERRUN_ALLOW_RATIO)) {
		BOOL	is_dir_fwd	= gmBoss5BodyIsMoveFastDirFwd(body_work);
		
		if (body_work->cur_run_type == GME_BOSS5_BODY_RUN_TYPE_RIGHT) {
			if (is_dir_fwd) {
				gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_FWD_R_AIR);
			}
			else {
				gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_BWD_R_AIR);
			}
		}
		else {
			if (is_dir_fwd) {
				gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_FWD_L_AIR);
			}
			else {
				gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_BWD_L_AIR);
			}
		}
		
		// 空中移動中の移動速度設定
		obj_work->spd.x	= GMD_BOSS5_BODY_RUN_FWD_FLY_INIT_SPD_X;
		if (obj_work->disp_flag & OBD_DISP_HFLIP) {
			obj_work->spd.x	= -obj_work->spd.x;
		}
		obj_work->spd.y	= GMD_BOSS5_BODY_RUN_FWD_FLY_INIT_SPD_Y;
		
		// 方向チェック
		if (is_dir_fwd == FALSE) {
			// 後方に移動（速度方向逆転＆係数で速度を抑える）
			obj_work->spd.x	= FX_Mul(-obj_work->spd.x, GMD_BOSS5_BODY_RUN_BWD_SPD_X_FACTOR);
		}
		
		// 空中移動フラグ設定
		obj_work->move_flag	|= (OBD_MOVE_JUMP | OBD_MOVE_FALL);
		obj_work->move_flag	&= ~OBD_MOVE_UNDER;
		
		body_work->proc_update	= gmBoss5BodyStateUpdateMoveFastWithAir;
	}
}

// 高速移動ステート更新 滞空処理
void gmBoss5BodyStateUpdateMoveFastWithAir(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	// 着地待ち
	if (obj_work->move_flag & OBD_MOVE_UNDER) {
		BOOL	is_dir_fwd	= gmBoss5BodyIsMoveFastDirFwd(body_work);
		
		// 停止
		GmBsCmnSetObjSpdZero(obj_work);
		
		if (GmBsCmnIsActionEnd(obj_work)) {
			
			// 接地タイミングの各種処理初期化
			gmBoss5BodyInitRunGroundingEffects(body_work,
											   body_work->cur_run_type,
											   GMD_BOSS5_BODY_RUN_GROUNDING_EFCT_DELAY);
			
			// MEMO:↓高速移動終了時でも、補間元のモーションに更新させるために一旦アクションを設定する。
			
			if (body_work->cur_run_type == GME_BOSS5_BODY_RUN_TYPE_RIGHT) {
				if (is_dir_fwd) {
					gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_FWD_R_LAND);
				}
				else {
					gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_BWD_R_LAND);
				}
			}
			else {
				if (is_dir_fwd) {
					gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_FWD_L_LAND);
				}
				else {
					gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_BWD_L_LAND);
				}
			}
			
			if (gmBoss5BodyIsMoveFastEnd(body_work)) {
				// 高速移動終了
				body_work->proc_update	= gmBoss5BodyStateUpdateMoveFastWithPreRecover;
				// 復帰前処理タイムアウト時間設定
				body_work->wait_timer	= GMD_BOSS5_BODY_RUN_PRE_RECOVER_TIMEOUT_FRAME;
				return;
			}
			else {
				// 高速移動中
				body_work->proc_update	= gmBoss5BodyStateUpdateMoveFastWithLand;
			}
		}
	}
}

// 高速移動ステート更新 着地処理
void gmBoss5BodyStateUpdateMoveFastWithLand(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 足接地時処理更新
	gmBoss5BodyUpdateRunGroundingEffects(body_work);
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		BOOL	is_dir_fwd	= gmBoss5BodyIsMoveFastDirFwd(body_work);
		
		if (body_work->cur_run_type == GME_BOSS5_BODY_RUN_TYPE_RIGHT) {
			body_work->cur_run_type	= GME_BOSS5_BODY_RUN_TYPE_LEFT;
			if (is_dir_fwd) {
				gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_FWD_L_JUMP);
			}
			else {
				gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_BWD_L_JUMP);
			}
			
			// 左足だけ攻撃を有効に
			gmBoss5BodySwitchEnableLegRectOneSide(body_work,
												  GME_BOSS5_LEG_TYPE_LEFT);
		}
		else {
			body_work->cur_run_type	= GME_BOSS5_BODY_RUN_TYPE_RIGHT;
			if (is_dir_fwd) {
				gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_FWD_R_JUMP);
			}
			else {
				gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_BWD_R_JUMP);
			}
			
			// 右足だけ攻撃を有効に
			gmBoss5BodySwitchEnableLegRectOneSide(body_work,
												  GME_BOSS5_LEG_TYPE_RIGHT);
		}
		
		// ジャンプ動作中の移動速度設定
		obj_work->spd.x	= GMD_BOSS5_BODY_RUN_FWD_JUMP_INIT_SPD_X;
		if (obj_work->disp_flag & OBD_DISP_HFLIP) {
			obj_work->spd.x	= -obj_work->spd.x;
		}
		
		// 方向チェック
		if (is_dir_fwd == FALSE) {
			// 後方に移動（速度方向逆転＆係数で速度を抑える）
			obj_work->spd.x	= FX_Mul(-obj_work->spd.x, GMD_BOSS5_BODY_RUN_BWD_SPD_X_FACTOR);
		}
		
		body_work->proc_update	= gmBoss5BodyStateUpdateMoveFastWithJump;
	}
}

// 高速移動ステート更新 復帰前処理
void gmBoss5BodyStateUpdateMoveFastWithPreRecover(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	BOOL	is_timeout	= FALSE;
	
	/*
	  復帰元モーション反映用に、当該モーションをブレンドが終了するまで更新します。
	  保険として、処理フレーム数が既定数に達したらブレンドが終了していなくても
	  その時点で次のプロシージャに移行します。
	  ステート開始時に直ちに復帰する場合はこのプロシージャを通さず、
	  直接、復帰処理（gmBoss5BodyStateUpdateMoveFastWithRecover）から行います。
	 */
	
	// 足接地時処理更新
	gmBoss5BodyUpdateRunGroundingEffects(body_work);
	 
	// タイムアウト計測
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		is_timeout	= TRUE;
	}
	
	if (!(obj_work->obj_3d->flag & OBD_ACTFLAG_3D_NN_BLEND) || is_timeout) {
		// 復帰アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_RECOVER);
		
		// FWへのブレンド時のズレ調整用に元姿勢を記録
		gmBoss5BodyRecordGapAdjustmentSrc(body_work);
		
		// モーションブレンドによるズレを補正
		if (body_work->cur_run_type == GME_BOSS5_BODY_RUN_TYPE_RIGHT) {
			gmBoss5BodyInitAdjustMtnBlendHGap(body_work,
											  body_work->whole_act_id,
											  GME_BOSS5_LEG_TYPE_RIGHT);
		}
		else {
			gmBoss5BodyInitAdjustMtnBlendHGap(body_work,
											  body_work->whole_act_id,
											  GME_BOSS5_LEG_TYPE_LEFT);
		}
		gmBoss5BodyUpdateAdjustMtnBlendHGap(body_work);
		
		// 復帰処理タイムアウト時間設定
		body_work->wait_timer	= GMD_BOSS5_BODY_RUN_RECOVER_TIMEOUT_FRAME;
		
		body_work->proc_update	= gmBoss5BodyStateUpdateMoveFastWithRecover;
	}
}

// 高速移動ステート更新 復帰処理
void gmBoss5BodyStateUpdateMoveFastWithRecover(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	BOOL	is_timeout	= FALSE;
	
	MTM_ASSERT(body_work->whole_act_id == GME_BOSS5_ACT_ID_MOVE_FAST_RECOVER);
	
	/*
	  ブレンドが終わるまでモーションブレンドズレの補正を行います。
	  既定時間以上かかる場合はブレンドが終了していなくても
	  その時点で終了します。
	 */
	
	// 足接地時処理更新
	gmBoss5BodyUpdateRunGroundingEffects(body_work);
	
	// タイムアウト計測
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		is_timeout	= TRUE;
	}
	
	if (!(obj_work->obj_3d->flag & OBD_ACTFLAG_3D_NN_BLEND) || is_timeout) {
		gmBoss5BodyProceedToNextSeqStr(body_work);
	}
	else {
		// モーションブレンドによるズレを補正
		gmBoss5BodyUpdateAdjustMtnBlendHGap(body_work);
	}
}

// =======================================================================
// gmBoss5BodyState{Enter|Leave|Update}StompXXX
/*!
  本体 ストンプステート遷移時処理関数
 */
// =======================================================================
// ストンプステート 開始関数
void gmBoss5BodyStateEnterStomp(GMS_BOSS5_BODY_WORK *body_work)
{
	// エンジン点火アクション設定
	gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_STOMP_IGNITE);
	
	// エンジン点火時間設定
	body_work->wait_timer	= GMD_BOSS5_BODY_STOMP_IGNITE_TIME;
	
	// 最初はデフォルト矩形設定
	gmBoss5BodyChangeRectSettingDefault(body_work);
	
	// 噴射エフェクト生成
	GmBoss5EfctStartJet(body_work);
	
	// 噴射スモークエフェクト生成
	GmBoss5EfctStartJetSmoke(body_work);
	
	body_work->proc_update	= gmBoss5BodyStateUpdateStompWithPrep;
}

// ストンプステート 終了関数
void gmBoss5BodyStateLeaveStomp(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// フラグ戻す
	obj_work->move_flag	&= ~(OBD_MOVE_JUMP | OBD_MOVE_NOCOL);
	obj_work->move_flag	|= OBD_MOVE_FALL;	// 浮遊処理中や上昇処理中にオフになるため
	
	// 噴射スモークエフェクト消去
	GmBoss5EfctEndJetSmoke(body_work);
	
	// 噴射エフェクト消去
	GmBoss5EfctEndJet(body_work);
	
	// デフォルト矩形設定に戻す
	gmBoss5BodyChangeRectSettingDefault(body_work);
}

// ストンプステート更新 準備処理
void gmBoss5BodyStateUpdateStompWithPrep(GMS_BOSS5_BODY_WORK *body_work)
{
	// 既定時間エンジン点火動作を行う
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		
		// 浮遊アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_STOMP_HOVER);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateStompWithHover;
	}
}

// ストンプステート更新 浮遊処理
void gmBoss5BodyStateUpdateStompWithHover(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		
		// 上昇アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_STOMP_FLYUP);
		
		// 上昇移動初期化
		gmBoss5BodyInitStompFlyUp(body_work);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateStompWithFlyUp;
	}
}

// ストンプステート更新 上昇処理
void gmBoss5BodyStateUpdateStompWithFlyUp(GMS_BOSS5_BODY_WORK *body_work)
{
	if (gmBoss5BodyCheckJetSmokeClearTiming(body_work)) {
		// 本体が既定の高さまで上昇したら噴射スモークを消去
		GmBoss5EfctEndJetSmoke(body_work);
	}
	
	if (gmBoss5BodyUpdateStompFlyUp(body_work)) {
		
		// 噴射スモークエフェクト消去
		GmBoss5EfctEndJetSmoke(body_work);
		
		// 噴射エフェクト消去
		GmBoss5EfctEndJet(body_work);
		
		// ターゲットカーソル生成
		GmBoss5EfctTargetCursorInit(body_work);
		body_work->flag	|= GMD_BOSS5_BODY_FLAG_TARGET_ON;
		
		// プレイヤーサーチ初期化
		gmBoss5BodyInitPlySearch(body_work, GMD_BOSS5_BODY_STOMP_SEARCH_DELAY);
		
		// 画面外待機時間設定
		body_work->wait_timer	= GMD_BOSS5_BODY_STOMP_WAIT_TIME;
		
		// ターゲットSE再生処理初期化
		gmBoss5BodyInitPlayTargetSe(body_work, GMD_BOSS5_BODY_SE_TARGET_INIT_INTERVAL);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateStompWithWait;
	}
}

// ストンプステート更新 画面外待機
void gmBoss5BodyStateUpdateStompWithWait(GMS_BOSS5_BODY_WORK *body_work)
{
	// タイマ終了の既定時間前までプレイヤーサーチする
	if (body_work->wait_timer >= GMD_BOSS5_BODY_STOMP_NO_SEARCH_TIME) {
		// プレイヤーサーチ更新
		gmBoss5BodyUpdatePlySearch(body_work);
	}
	
	// ターゲットSE再生処理 更新
	gmBoss5BodyUpdatePlayTargetSe(body_work);
	
	// 既定時間、画面外で待機
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
		VecFx32	last_search_pos;
		
		// ターゲット表示オフ
		body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_TARGET_ON;
		
		// 最後のサーチ位置に水平位置を合わせる
		GmBoss5BodyGetPlySearchPos(body_work, &last_search_pos);
		obj_work->pos.x	= gmBoss5BodyGetStompFallPosX(body_work, last_search_pos.x);
		
		// 降下アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_STOMP_FALL);
		
		// 向き決定＆設定（落下位置決定後に行うこと）
		gmBoss5BodySetDirection(body_work,
								gmBoss5BodyGetStompFallDirectionType(body_work));
		
		// 降下開始
		gmBoss5BodyInitStompFall(body_work);
		
		// ストンプ降下用矩形設定
		if (gmBoss5BodySeqIsStr(body_work)) {
			gmBoss5BodyChangeRectSetting(body_work, GME_BOSS5_BODY_RECT_SETTING_STOMP_STR_FALL);
		}
		else {
			gmBoss5BodyChangeRectSetting(body_work, GME_BOSS5_BODY_RECT_SETTING_STOMP_NML_FALL);
		}
		
		body_work->proc_update	= gmBoss5BodyStateUpdateStompWithFall;
	}
}

// ストンプステート更新 降下処理
void gmBoss5BodyStateUpdateStompWithFall(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 降下完了待ち
	if (gmBoss5BodyUpdateStompFall(body_work)) {
		
		// 着地衝撃SE再生
		GmSoundPlaySE("FinalBoss06");
		
		// 振動中
		GMM_PAD_VIB_MID_TIME(30);
		
		// 空中関連フラグを戻す
		obj_work->move_flag	&= ~OBD_MOVE_JUMP;
		
		// 着地アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_STOMP_LAND);
		
		// デフォルト矩形設定
		gmBoss5BodyChangeRectSettingDefault(body_work);
		
		// 着地エフェクト生成
		GmBoss5EfctCreateLandingShockwave(body_work);
		
		// ストンプ着地振動
		gmBoss5Vibration(GME_BOSS5_VIB_IDX_STOMP_LANDING);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateStompWithLand;
	}
}

// ストンプステート更新 着地
void gmBoss5BodyStateUpdateStompWithLand(GMS_BOSS5_BODY_WORK *body_work)
{
	// 着地＆復帰中
	if (GmBsCmnIsActionEnd(GMM_BS_OBJ(body_work))) {
		
		if (body_work->state == GME_BOSS5_BODY_STATE_STOMP_STR) {
			gmBoss5BodyProceedToNextSeqStr(body_work);
		}
		else {
			MTM_ASSERT(body_work->state == GME_BOSS5_BODY_STATE_STOMP_NML);
			gmBoss5BodyProceedToNextSeqNml(body_work);
		}
	}
}

// =======================================================================
// gmBoss5BodyState{Enter|Leave|Update}CrashXXX
/*!
  本体 地球割りステート遷移時処理関数
 */
// =======================================================================
// 地球割りステート 開始関数
void gmBoss5BodyStateEnterCrash(GMS_BOSS5_BODY_WORK *body_work)
{
	// 警告レベルを地球割り用に設定
	gmBoss5MgrSetAlarmLevel(body_work->mgr_work, GME_BOSS5_ALARM_LEVEL_CRASH);
	
	// 最後の一撃モードに設定
	body_work->flag	|= GMD_BOSS5_BODY_FLAG_FINAL_MODE;
	
	// バルカン禁止
	body_work->flag	|= GMD_BOSS5_BODY_FLAG_HOLD_VULCAN;
	
	// 準備アクション設定
	gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_CRASH_PREP);
	
	// 最初はデフォルト矩形設定
	gmBoss5BodyChangeRectSettingDefault(body_work);
	
	// 噴射エフェクト生成
	GmBoss5EfctStartJet(body_work);
	
	// 噴射スモークエフェクト生成
	GmBoss5EfctStartJetSmoke(body_work);
	
	// 処理関数設定
	body_work->proc_update	= gmBoss5BodyStateUpdateCrashWithPrep;
}

// 地球割りステート 終了関数
void gmBoss5BodyStateLeaveCrash(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_DENY_HOMING_ATK;
	
	// 振動大停止
	GMM_PAD_VIB_STOP();
	
	// フラグ戻す
	obj_work->move_flag	&= ~(OBD_MOVE_JUMP | OBD_MOVE_NOCOL);
	obj_work->move_flag	|= OBD_MOVE_FALL;	// 上昇処理中などにオフになるため
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_HOLD_VULCAN;
	
	// 噴射スモークエフェクト消去
	GmBoss5EfctEndJetSmoke(body_work);
	
	// カメラを元の高さにもどす
	gmBoss5RestoreCameraLift(body_work->mgr_work);
	
	// 噴射エフェクト消去
	GmBoss5EfctEndJet(body_work);
	
	// デフォルト矩形設定に戻す
	gmBoss5BodyChangeRectSettingDefault(body_work);
}

// 地球割りステート更新 準備処理
void gmBoss5BodyStateUpdateCrashWithPrep(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		
		// 落下地点を決定
		gmBoss5BodyDecideCrashFallPosX(body_work);
		
		// ジャンプ開始アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_CRASH_HOVER);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateCrashWithStartFly;
	}
}

// 地球割りステート更新 飛行開始処理
void gmBoss5BodyStateUpdateCrashWithStartFly(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		// 上昇アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_CRASH_FLYUP);
		
		// 上昇開始
		gmBoss5BodyInitCrashFlyUp(body_work);
		
		// カメラを上に持ち上げる
		gmBoss5SetCameraLift(body_work->mgr_work);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateCrashWithFlyUp;
	}
}

// 地球割りステート更新 上昇処理
void gmBoss5BodyStateUpdateCrashWithFlyUp(GMS_BOSS5_BODY_WORK *body_work)
{
	if (gmBoss5BodyCheckJetSmokeClearTiming(body_work)) {
		// 本体が既定の高さまで上昇したら噴射スモークを消去
		GmBoss5EfctEndJetSmoke(body_work);
	}
	
	if (gmBoss5BodyUpdateCrashFlyUp(body_work)) {
		
		// 足場生成要求
		body_work->mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_LAND_NEEDED;
		
		// 噴射スモークエフェクト消去
		GmBoss5EfctEndJetSmoke(body_work);
		
		// 噴射エフェクト消去
		GmBoss5EfctEndJet(body_work);
		
		// 画面外待機時間設定
		body_work->wait_timer	= GMD_BOSS5_BODY_CRASH_WAIT_TIME;
		
		// ターゲットカーソルを生成
		// （タイマ終了の既定時間前にループカーソル状態になるようにタイマ設定）
		GmBoss5EfctCrashCursorInit(body_work,
								   gmBoss5BodyGetCrashFallPosX(body_work),
								   GMD_BOSS5_BODY_CRASH_WAIT_TIME - GMD_BOSS5_BODY_CRASH_CURSOR_SPAWN_TIME_TRHESHOLD);
		body_work->flag	|= GMD_BOSS5_BODY_FLAG_TARGET_ON;		
		
		body_work->proc_update	= gmBoss5BodyStateUpdateCrashWithWait;
	}
}

// 地球割りステート更新 画面外待機
void gmBoss5BodyStateUpdateCrashWithWait(GMS_BOSS5_BODY_WORK *body_work)
{
	// 既定時間、画面外で待機
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
		
		// 落下ポイント設定
		obj_work->pos.x	= gmBoss5BodyGetCrashFallPosX(body_work);
		
		// 左向きに設定（降下アクションが、左向き前提のモーションのため）
		gmBoss5BodySetDirection(body_work, GME_BOSS5_BODY_DIRECTION_TYPE_LEFT);
		
		// 降下アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_CRASH_FALL);
		
		// 降下開始
		gmBoss5BodyInitCrashFall(body_work);
		
		// 地球割り降下用矩形設定
		gmBoss5BodyChangeRectSetting(body_work, GME_BOSS5_BODY_RECT_SETTING_CRASH_FALL);
		
		// 地球割り落下SE再生
		GmSoundPlaySE("FinalBoss17");
		
		// カメラを元の高さにもどす
		gmBoss5RestoreCameraLift(body_work->mgr_work);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateCrashWithFall;
	}
}

// 地球割りステート更新 降下処理
void gmBoss5BodyStateUpdateCrashWithFall(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 降下完了待ち
	if (gmBoss5BodyUpdateCrashFall(body_work)) {
		
		// ターゲットカーソル消去
		body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_TARGET_ON;
		
		// 空中関連フラグを戻す
		obj_work->move_flag	&= ~OBD_MOVE_JUMP;
		
		// 着地アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_CRASH_LAND);
		
		// 地面突き衝撃波生成（アクション設定から指定フレーム後に表示）
		GmBoss5EfctCreateStrikeShockwave(body_work, GMD_BOSS5_BODY_CRASH_STRIKE_SW_CREATE_DELAY);
		
		// 地球割り地面突き振動（アクション設定から指定フレーム後に開始）
		gmBoss5DelayedVibration(GME_BOSS5_VIB_IDX_CRASH_STRIKE,
								GMD_BOSS5_BODY_CRASH_STRIKE_VIB_START_DELAY);
		
		// 地球割り地面突き本体振動（アクション設定から指定フレーム後に開始）
		gmBoss5BodyInitCrashStrikeVib(body_work,
									  GMD_BOSS5_BODY_CRASH_STRIKE_BODY_VIB_START_DELAY);
		
		// 地球割り地上時矩形設定
		gmBoss5BodyChangeRectSetting(body_work, GME_BOSS5_BODY_RECT_SETTING_CRASH_GROUND);
		
		// 漏電エフェクトを強制終了
		gmBoss5BodyForceEndLeakage(body_work);
		
		// 着地エフェクト生成
		GmBoss5EfctCreateLandingShockwave(body_work);
		
		// 着地煙エフェクト生成
		GmBoss5EfctCreateCrashLandingSmoke(body_work);
		
		// プレイヤー不動化試行
		gmBoss5BodyTryImmobilizePlayer(body_work);
		
		// 地球割り着地振動
		gmBoss5Vibration(GME_BOSS5_VIB_IDX_CRASH_LANDING);
		
		// 振動大（後で止める）
		GmPadVibSet(GMD_PAD_VIB_LARGE_TYPE, -1.f,
					GMD_PAD_VIB_LARGE_LEFT_VIB, GMD_PAD_VIB_LARGE_RIGHT_VIB,
					-1.f,
					GMD_PAD_VIB_LARGE_INT_VIB_TIME, GMD_PAD_VIB_LARGE_INT_STOP_TIME,
					GMD_PAD_VIB_LARGE_PRIO);
		
		// 地球割りSE再生
		gmBoss5DelayedSePlayback("FinalBoss18", GMD_BOSS5_BODY_CRASH_STRIKE_SE_START_DELAY);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateCrashWithLand;
	}
}

// 地球割りステート更新 着地処理
void gmBoss5BodyStateUpdateCrashWithLand(GMS_BOSS5_BODY_WORK *body_work)
{
	// 本体振動更新
	gmBoss5BodyUpdateCrashStrikeVib(body_work);
	
	// 着地＆地面突き中
	if (GmBsCmnIsActionEnd(GMM_BS_OBJ(body_work))) {
		
		// 足場振動要求
		body_work->mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_LAND_SHAKE_NEEDED;
		
		// 停滞時間設定
		body_work->wait_timer	= GMD_BOSS5_BODY_CRASH_LANDED_IDLE_TIME;
		
		body_work->proc_update	= gmBoss5BodyStateUpdateCrashWithIdle;
	}
}

// 地球割りステート更新 停滞処理
void gmBoss5BodyStateUpdateCrashWithIdle(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 本体振動更新
	gmBoss5BodyUpdateCrashStrikeVib(body_work);
	
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		
		// 振動大停止
		GMM_PAD_VIB_STOP();
		
		// 沈下時アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_CRASH_SINK);
		
		// 沈下処理開始
		gmBoss5BodyInitCrashSink(body_work);
		
		// 地球割り沈下時矩形設定
		gmBoss5BodyChangeRectSetting(body_work, GME_BOSS5_BODY_RECT_SETTING_CRASH_SINK);
		obj_work->flag	|= OBD_OBJECT_NOHIT;	// 当たり処理しない
		
		// ホーミングアタックの対象外にする
		body_work->flag	|= GMD_BOSS5_BODY_FLAG_DENY_HOMING_ATK;
		
		// 地球割り成功設定
		body_work->flag	|= GMD_BOSS5_BODY_FLAG_CRASH_SUCCESS;
		
		// 足場崩壊要求
		body_work->mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_LAND_BREAK_NEEDED;
		
		// 下スクロールロック解除
		GmGmkCamScrLimitRelease(GMD_GMK_SCR_LMT_RELEASE_BOTTOM);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateCrashWithSink;
	}
}

// 地球割りステート更新 沈下処理
void gmBoss5BodyStateUpdateCrashWithSink(GMS_BOSS5_BODY_WORK *body_work)
{
	// 沈下処理更新
	gmBoss5BodyUpdateCrashSink(body_work);
}

// =======================================================================
// gmBoss5BodyState{Enter|Leave|Update}RpcXXX
/*!
  本体 ロケットパンチステート遷移時処理関数
 */
// =======================================================================
// ロケットパンチステート 開始関数
void gmBoss5BodyStateEnterRpc(GMS_BOSS5_BODY_WORK *body_work)
{
	// 準備アクション設定
	gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_RPUNCH_PREP);
	
	// デフォルト矩形設定
	gmBoss5BodyChangeRectSettingDefault(body_work);
	
	// 処理関数設定
	body_work->proc_update	= gmBoss5BodyStateUpdateRpcWithPrep;
}

// ロケットパンチステート 終了関数
void gmBoss5BodyStateLeaveRpc(GMS_BOSS5_BODY_WORK *body_work)
{
	// 要求シグナルクリア
	body_work->flag	&= ~(GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_RETURN_L |
						 GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_RETURN_R |
						 GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_LAUNCH_L |
						 GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_LAUNCH_R);
	
	// 腕表示
	body_work->flag	&= ~(GMD_BOSS5_BODY_FLAG_HIDE_ARM_L |
						 GMD_BOSS5_BODY_FLAG_HIDE_ARM_R);
	
	// デフォルト矩形設定に戻す
	gmBoss5BodyChangeRectSettingDefault(body_work);
}

// ロケットパンチステート更新 準備処理
void gmBoss5BodyStateUpdateRpcWithPrep(GMS_BOSS5_BODY_WORK *body_work)
{
	if (GmBsCmnIsActionEnd(GMM_BS_OBJ(body_work))) {
		
		// 左腕非表示
		body_work->flag	|= GMD_BOSS5_BODY_FLAG_HIDE_ARM_L;
		
		// 左ロケットパンチ生成
		if (body_work->state == GME_BOSS5_BODY_STATE_RPUNCH_STR) {
			GmBoss5RocketLaunchStrong(body_work, GME_BOSS5_RKT_TYPE_LEFT);
		}
		else {
			MTM_ASSERT(body_work->state == GME_BOSS5_BODY_STATE_RPUNCH_NML);
			GmBoss5RocketLaunchNormal(body_work, GME_BOSS5_RKT_TYPE_LEFT);
		}
		
		// サーチ時間設定
		if (body_work->state == GME_BOSS5_BODY_STATE_RPUNCH_STR) {
			body_work->wait_timer	= gmBoss5BodySeqGetRpcStrSearchTime(body_work);
		}
		else {
			MTM_ASSERT(body_work->state == GME_BOSS5_BODY_STATE_RPUNCH_NML);
			body_work->wait_timer	= gmBoss5BodySeqGetRpcNmlSearchTime(body_work);
		}
		
		body_work->proc_update	= gmBoss5BodyStateUpdateRpcWithSearch;
	}
}

// ロケットパンチステート サーチ処理
void gmBoss5BodyStateUpdateRpcWithSearch(GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		// 左発射モーション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_RPUNCH_LAUNCH_FIRST);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateRpcWithLaunchFirst;
	}
}

// ロケットパンチステート 1発目発射処理
void gmBoss5BodyStateUpdateRpcWithLaunchFirst(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (obj_work->obj_3d->frame[0] >= GMD_BOSS5_BODY_RPUNCH_LAUNCH_TIMING_DELAY) {
		
		// 左ロケット発射通知
		body_work->flag	|= GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_LAUNCH_L;
		
		body_work->proc_update	= gmBoss5BodyStateUpdateRpcWithWaitReturnFirst;
	}
}

// ロケットパンチステート 1発目戻り待ち処理
void gmBoss5BodyStateUpdateRpcWithWaitReturnFirst(GMS_BOSS5_BODY_WORK *body_work)
{
	BOOL	rkt_spawn_allowed	= FALSE;
	
	if (GmBsCmnIsActionEnd(GMM_BS_OBJ(body_work))) {
		// 発射モーション中はサーチを行わないようにするため、
		// 発射モーションが終了した時点で左ロケット生成許可
		rkt_spawn_allowed	= TRUE;
	}
	
	if (body_work->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_L) {
		body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_L;
		
		// 1発目が戻ってきた場合は直ちに左ロケット生成許可
		rkt_spawn_allowed	= TRUE;
		
		// 左腕表示
		body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_HIDE_ARM_L;
		
		// 右発射モーション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_RPUNCH_LAUNCH_SECOND);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateRpcWithLaunchSecond;
	}
	
	// 左ロケットパンチ発射チェック
	if (rkt_spawn_allowed &&
		!(body_work->flag & GMD_BOSS5_BODY_FLAG_HIDE_ARM_R)) {
		// ↑右腕が非表示かどうかで発射済みの有無を判定
		
		// 右腕非表示
		body_work->flag	|= GMD_BOSS5_BODY_FLAG_HIDE_ARM_R;
		
		// 右ロケットパンチ生成
		if (body_work->state == GME_BOSS5_BODY_STATE_RPUNCH_STR) {
			GmBoss5RocketLaunchStrong(body_work, GME_BOSS5_RKT_TYPE_RIGHT);
		}
		else {
			MTM_ASSERT(body_work->state == GME_BOSS5_BODY_STATE_RPUNCH_NML);
			GmBoss5RocketLaunchNormal(body_work, GME_BOSS5_RKT_TYPE_RIGHT);
		}
	}
}

// ロケットパンチステート 2発目発射処理
void gmBoss5BodyStateUpdateRpcWithLaunchSecond(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (obj_work->obj_3d->frame[0] >= GMD_BOSS5_BODY_RPUNCH_LAUNCH_TIMING_DELAY) {
		
		// 右ロケット発射通知
		body_work->flag	|= GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_LAUNCH_R;
		
		body_work->proc_update	= gmBoss5BodyStateUpdateRpcWithWaitReturnSecond;
	}
}

// ロケットパンチステート 2発目戻り待ち処理
void gmBoss5BodyStateUpdateRpcWithWaitReturnSecond(GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_R) {
		body_work->flag &= ~GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_R;
		
		// 右腕表示
		body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_HIDE_ARM_R;
		
		// 戻りモーション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_RPUNCH_RETURN);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateRpcWithRecover;
	}
}

// ロケットパンチステート 復帰処理
void gmBoss5BodyStateUpdateRpcWithRecover(GMS_BOSS5_BODY_WORK *body_work)
{
	if (GmBsCmnIsActionEnd(GMM_BS_OBJ(body_work))) {
		if (body_work->state == GME_BOSS5_BODY_STATE_RPUNCH_STR) {
			gmBoss5BodyProceedToNextSeqStr(body_work);
		}
		else {
			MTM_ASSERT(body_work->state == GME_BOSS5_BODY_STATE_RPUNCH_NML);
			gmBoss5BodyProceedToNextSeqNml(body_work);
		}
	}
}


// =======================================================================
// gmBoss5BodyState{Enter|Leave|Update}BerserkXXX
/*!
  本体 凶暴化演出ステート遷移時処理関数
 */
// =======================================================================
// 凶暴化演出ステート 開始関数
void gmBoss5BodyStateEnterBerserk(GMS_BOSS5_BODY_WORK *body_work)
{
	// 凶暴化開始アクション設定（凶暴演出へのブレンド元モーションとしてFWを設定）
	gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_BERSERK_BREAKDOWN);
	
	// 凶暴化開始時矩形設定（ダメージ受けないが、漏電エフェクトは出さない）
	gmBoss5BodyChangeRectSetting(body_work, GME_BOSS5_BODY_RECT_SETTING_BERSERK_START);
	
	// 煙エフェクト生成
	GmBoss5EfctBreakdownSmokesInit(body_work, GMD_BOSS5_BODY_BERSERK_BREAKDOWN_TIME);	// 機能停止時間と同じ時間を設定
	GmBoss5EfctBodySmallSmokesInit(body_work);
	
	// 機能停止時間設定
	body_work->wait_timer	= GMD_BOSS5_BODY_BERSERK_BREAKDOWN_TIME;
	
	// 処理関数設定
	body_work->proc_update	= gmBoss5BodyStateUpdateBerserkWithBreakdown;
}

// 凶暴化演出ステート 終了関数
void gmBoss5BodyStateLeaveBerserk(GMS_BOSS5_BODY_WORK *body_work)
{
	// 方向転換処理クリア
	gmBoss5BodyClearBerserkTurn(body_work);
	
	// デフォルト矩形設定に戻す
	gmBoss5BodyChangeRectSettingDefault(body_work);
}

// 凶暴化演出ステート更新 機能停止処理
void gmBoss5BodyStateUpdateBerserkWithBreakdown(GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		// 振動動作アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_BERSERK_SHAKE);
		
		// 振動加速初期化
		gmBoss5BodyInitShakeAccelerate(body_work);
		
		// 予備漏電エフェクト開始
		GmBoss5EfctStartPrelimLeakage(body_work);
		
		// 振動モーション最高速度で再生し続ける時間
		body_work->wait_timer	= GMD_BOSS5_BODY_BERSERK_SHAKE_STAY_TIME;
		
		body_work->proc_update	= gmBoss5BodyStateUpdateBerserkWithShake;
	}
}

// 凶暴化演出ステート更新 振動動作処理
void gmBoss5BodyStateUpdateBerserkWithShake(GMS_BOSS5_BODY_WORK *body_work)
{
	// 振動加速更新
	if (gmBoss5BodyUpdateShakeAccelerate(body_work)) {
		// 最高速度に到達後、既定時間再生し続ける
		if (body_work->wait_timer) {
			body_work->wait_timer--;
		}
		else {
			// 警告フェード生成
			gmBoss5InitAlarmFade(body_work->mgr_work);
			gmBoss5MgrSetAlarmLevel(body_work->mgr_work, GME_BOSS5_ALARM_LEVEL_STRONG);
			
			// 警告灯装飾開始
			GmDecoStartEffectFinalBossLight();
			
			// 咆哮準備アクション設定
			gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_BERSERK_ROAR_PREP);
			
			// 凶暴化SE再生
			GmSoundPlaySE("FinalBoss08");
			
			// 方向転換初期化
			gmBoss5BodyInitBerserkTurn(body_work, GME_BOSS5_BODY_BSK_TURN_TYPE_FRONT);
			
			// 予備漏電エフェクト終了
			GmBoss5EfctEndPrelimLeakage(body_work);
			
			// デフォルト矩形設定
			MTM_ASSERT(gmBoss5BodySeqIsStr(body_work));	// 既に凶暴化として設定されている前提
			gmBoss5BodyChangeRectSettingDefault(body_work);
			
			// 直ぐに方向転換に移行
			body_work->proc_update	= gmBoss5BodyStateUpdateBerserkWithTurnFront;
		}
	}
}

// 凶暴化演出ステート更新 正面方向転換
void gmBoss5BodyStateUpdateBerserkWithTurnFront(GMS_BOSS5_BODY_WORK *body_work)
{
	if (gmBoss5BodyUpdateBerserkTurn(body_work)) {
		
		// 凶暴化スチームエフェクト初期化
		GmBoss5EfctBerserkSteamInit(body_work, 1);
		
		// 咆哮準備継続時間設定
		body_work->wait_timer	= GMD_BOSS5_BODY_BERSERK_ROAR_PREP_TIME;
		
		body_work->proc_update	= gmBoss5BodyStateUpdateBerserkWithRoarPrep;
	}
}

// 凶暴化ステート更新 咆哮準備処理
void gmBoss5BodyStateUpdateBerserkWithRoarPrep(GMS_BOSS5_BODY_WORK *body_work)
{
	// 既定時間ループ
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		// 咆哮開始アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_BERSERK_ROAR_START);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateBerserkWithRoarStart;
	}
}

// 凶暴化ステート更新 咆哮開始処理
void gmBoss5BodyStateUpdateBerserkWithRoarStart(GMS_BOSS5_BODY_WORK *body_work)
{
	if (GmBsCmnIsActionEnd(GMM_BS_OBJ(body_work))) {
		
		// 凶暴化スチームエフェクト初期化
		GmBoss5EfctBerserkSteamInit(body_work, 1);
		
		// 咆哮ループアクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_BERSERK_ROAR_LOOP);
		
		// 咆哮ループ継続時間設定
		body_work->wait_timer	= GMD_BOSS5_BODY_BERSERK_ROAR_LOOP_TIME;
		
		body_work->proc_update	= gmBoss5BodyStateUpdateBerserkWithRoarLoop;
	}
}

// 凶暴化ステート更新 咆哮ループ処理
void gmBoss5BodyStateUpdateBerserkWithRoarLoop(GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		// 足踏み込みアクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_BERSERK_STAMP);
		
		// 足踏み込みエフェクト生成
		GmBoss5EfctCreateBerserkStampSmoke(body_work, GME_BOSS5_LEG_TYPE_LEFT,
										   GMD_BOSS5_BODY_BERSERK_STAMP_SMOKE_CREATE_DELAY);
		// 足踏み込み振動
		gmBoss5DelayedVibration(GME_BOSS5_VIB_IDX_BERSERK_STAMP,
								GMD_BOSS5_BODY_BERSERK_STAMP_VIB_START_DELAY);
		
		// 足踏み込みSE（歩きSE）再生
		gmBoss5DelayedSePlayback("FinalBoss03",
								 GMD_BOSS5_BODY_BERSERK_STAMP_SE_START_DELAY);
		
		// 方向転換初期化
		gmBoss5BodyInitBerserkTurn(body_work, GME_BOSS5_BODY_BSK_TURN_TYPE_RETURN);
		
		// 右足を地面に固定する
		gmBoss5BodyInitGroundingMove(body_work, body_work->rfoot_snm_reg_id);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateBerserkWithTurnSide;
	}
}

// 凶暴化演出ステート 左右方向転換（元の方向）
void gmBoss5BodyStateUpdateBerserkWithTurnSide(GMS_BOSS5_BODY_WORK *body_work)
{
	// 右足地面固定更新
	gmBoss5BodyUpdateGroundingMove(body_work);
	
	if (gmBoss5BodyUpdateBerserkTurn(body_work)) {
		
		// 方向転換処理クリア
		gmBoss5BodyClearBerserkTurn(body_work);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateBerserkWithStamp;
	}
}

// 凶暴化演出ステート 足踏み込み処理
void gmBoss5BodyStateUpdateBerserkWithStamp(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 右足地面固定更新
	gmBoss5BodyUpdateGroundingMove(body_work);
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		
		// 足蹴り上げアクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_BERSERK_KICKUP);
		
		// 左足を地面に固定する
		gmBoss5BodyInitGroundingMove(body_work, body_work->lfoot_snm_reg_id);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateBerserkWithKickUp;
	}
}

// 凶暴化演出ステート更新 足蹴り上げ処理
void gmBoss5BodyStateUpdateBerserkWithKickUp(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 左足地面固定更新
	gmBoss5BodyUpdateGroundingMove(body_work);
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		gmBoss5BodyProceedToNextSeqStr(body_work);
	}
}

// =======================================================================
// gmBoss5BodyState{Enter|Leave|Update}DefeatXXX
/*!
  本体 撃破ステート遷移時処理関数
 */
// =======================================================================
// 撃破ステート 開始関数
void gmBoss5BodyStateEnterDefeat(GMS_BOSS5_BODY_WORK *body_work)
{
	// 本体撃破を管理に通知
	// （このフラグがチェックされた瞬間にタイマストップ・ゴール通知が行われる）
	body_work->mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_BODY_DEFEAT;
	
	// スコア加算
	GmPlayerAddScoreNoDisp((GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj(), GMD_PLY_SCORE_BOSS);
	
	// バルカン禁止
	body_work->flag	|= GMD_BOSS5_BODY_FLAG_HOLD_VULCAN;
	
	// ホーミングアタックの対象外にする
	body_work->flag	|= GMD_BOSS5_BODY_FLAG_DENY_HOMING_ATK;
	
	// 撃破演出用矩形設定
	gmBoss5BodyChangeRectSetting(body_work, GME_BOSS5_BODY_RECT_SETTING_DEFEAT);
	
	// 小爆発初期化
	gmBoss5InitExplCreate(&body_work->expl_work,
						  GME_BOSS5_EXPL_TYPE_SMALL_WITH_FRAGS,
						  body_work->part_obj_core,
						  GMD_BOSS5_EXPL_BODY_OFST_X,
						  GMD_BOSS5_EXPL_BODY_OFST_Y,
						  GMD_BOSS5_EXPL_BODY_WIDTH,
						  GMD_BOSS5_EXPL_BODY_HEIGHT,
						  GMD_BOSS5_EXPL_BODY_INTERVAL_MIN,
						  GMD_BOSS5_EXPL_BODY_INTERVAL_MAX,
						  GMD_BOSS5_EXPL_BODY_SE_FREQUENCY);
	
	// 待機時間設定
	body_work->wait_timer	= GMD_BOSS5_BODY_DEFEAT_WAIT_START_TIME;
	
	// 処理関数設定
	body_work->proc_update	= gmBoss5BodyStateUpdateDefeatWithWaitStart;
}

// 撃破ステート 終了関数
void gmBoss5BodyStateLeaveDefeat(GMS_BOSS5_BODY_WORK *body_work)
{
	// フラグクリア
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_DENY_HOMING_ATK;
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_HOLD_VULCAN;
}

// 撃破ステート更新 開始待機処理
void gmBoss5BodyStateUpdateDefeatWithWaitStart(GMS_BOSS5_BODY_WORK *body_work)
{
	// 小爆発更新
	gmBoss5UpdateExplCreate(&body_work->expl_work);
	
	// 既定時間待機
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		
		// 飛散用にCNMノード登録
		gmBoss5BodyRegisterScatterPartsCNM(body_work);
		
		// 飛散処理開始
		gmBoss5InitScatter(body_work);
		
		// 本体落下タイマ設定（左脚or右脚で最後に飛散するほうのタイミングにあわせる）
		body_work->wait_timer	= MTM_MATH_MAX(GMD_BOSS5_SCT_LEG_FLY_DELAY_LEFT,
											   GMD_BOSS5_SCT_LEG_FLY_DELAY_RIGHT);
		
		body_work->proc_update	= gmBoss5BodyStateUpdateDefeatWithExplode;
	}
}

// 撃破ステート更新 爆発処理
void gmBoss5BodyStateUpdateDefeatWithExplode(GMS_BOSS5_BODY_WORK *body_work)
{
	BOOL	is_body_fall_end	= FALSE;
	
	// 小爆発更新
	gmBoss5UpdateExplCreate(&body_work->expl_work);
	
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		if (!(body_work->flag & GMD_BOSS5_BODY_FLAG_SCT_FALL_ACTIVE)) {
			// パーツ飛散時の落下処理初期化
			gmBoss5BodyInitScatterFall(body_work);
			
			body_work->flag	|= GMD_BOSS5_BODY_FLAG_SCT_FALL_ACTIVE;
		}
		else {
			OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
			// パーツ飛散時の落下処理更新
			if (gmBoss5BodyUpdateScatterFall(body_work)) {
				is_body_fall_end	= TRUE;
			}
			
			// 本体用の大爆発エフェクトを1回だけ生成
			if ((obj_work->move_flag & OBD_MOVE_UNDER) &&
				!(body_work->flag & GMD_BOSS5_BODY_FLAG_LAST_BIG_EXPL_CREATED)) {
				GmBoss5EfctCreateBigExplosion(body_work->part_obj_core->pos.x,
											  body_work->part_obj_core->pos.y,
											  body_work->part_obj_core->pos.z + GMD_BOSS5_EXPL_OFST_Z);
				body_work->flag	|= GMD_BOSS5_BODY_FLAG_LAST_BIG_EXPL_CREATED;
				
				// 大爆発SE再生
				GmSoundPlaySE("Boss0_03");
				
				// 画面フラッシュ
				gmBoss5InitFlashScreen();
				
				// 振動中
				// 恐らく爆発演出の時の振動にかき消されるが、念のため
				GMM_PAD_VIB_MID_TIME(120);
			}
		}
	}
	
	if (gmBoss5BodyIsBodyExplosionStopAllowed(body_work) && is_body_fall_end) {
		// 爆発関連処理の停止許可が下りたらシーケンス停止
		body_work->proc_update	= NULL;
		
		// プレイヤー追いかけ爆発をリクエスト
		body_work->mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_CHASING_EXPL_NEEDED;
	}
}

// ============================================================================
// サブシーケンス
// ============================================================================

// =======================================================================
// gmBoss5BodySubSeqEnterMoveFastCrash
/*!
  本体 壁衝突サブシーケンス（前・後共有）
 */
// =======================================================================
// 壁衝突サブシーケンス 開始関数
void gmBoss5BodySubSeqEnterMoveFastCrash(GMS_BOSS5_BODY_WORK *body_work)
{
	if (body_work->sub_seq == GME_BOSS5_BODY_SUB_SEQ_MOVE_FAST_FWD_CRASH) {
		// 前から衝突
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_FWD_CRASH);
	}
	else {
		MTM_ASSERT(body_work->sub_seq == GME_BOSS5_BODY_SUB_SEQ_MOVE_FAST_BWD_CRASH);
		// 後ろから衝突
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_MOVE_FAST_BWD_CRASH);
	}
	
	// 処理関数設定
	body_work->proc_update	= gmBoss5BodySubSeqUpdateMoveFastCrashWithStagger;
}

// 壁衝突サブシーケンス更新 よろめき処理
void gmBoss5BodySubSeqUpdateMoveFastCrashWithStagger(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (obj_work->move_flag & OBD_MOVE_UNDER) {
		if (GmBsCmnIsActionEnd(obj_work)) {
			gmBoss5BodyResumeMoveFast(body_work);
		}
	}
}

// =======================================================================
// gmBoss5BodySubSeq{Enter|Update}RpcStrDmgXXX
/*!
  本体 のけぞりサブシーケンス遷移時処理関数
 */
// =======================================================================
// のけぞりサブシーケンス 開始関数
void gmBoss5BodySubSeqEnterRpcStrDmg(GMS_BOSS5_BODY_WORK *body_work)
{
	// ノックバックアクションに設定
	gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_RPC_STR_DMG_START);
	
	// のけぞり用矩形設定
	gmBoss5BodyChangeRectSetting(body_work, GME_BOSS5_BODY_RECT_SETTING_RPC_STR_DMG);
	
	// 処理関数設定
	body_work->proc_update	= gmBoss5BodySubSeqUpdateRpcStrDmgWithBend;
}

// のけぞりサブシーケンス更新 のけぞり処理
void gmBoss5BodySubSeqUpdateRpcStrDmgWithBend(GMS_BOSS5_BODY_WORK *body_work)
{
	if (GmBsCmnIsActionEnd(GMM_BS_OBJ(body_work))) {
		
		// よろけループアクションに設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_RPC_STR_DMG_LOOP);
		
		// よろけ時間設定
		body_work->wait_timer	= GMD_BOSS5_BODY_RPC_STR_DMG_SWING_TIME;
		
		body_work->proc_update	= gmBoss5BodySubSeqUpdateRpcStrDmgWithSwing;
	}
}

// のけぞりサブシーケンス更新 よろけ処理
void gmBoss5BodySubSeqUpdateRpcStrDmgWithSwing(GMS_BOSS5_BODY_WORK *body_work)
{
	// 既定時間よろける
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		
		// ロケット帰還要求シグナル発行
		body_work->flag	|= (GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_RETURN_L |
							GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_RETURN_R);
		
		body_work->proc_update	= gmBoss5BodySubSeqUpdateRpcStrDmgWithWaitReturn;
	}
}

// のけぞりサブシーケンス更新 ロケット帰還待ち
void gmBoss5BodySubSeqUpdateRpcStrDmgWithWaitReturn(GMS_BOSS5_BODY_WORK *body_work)
{
	if (gmBoss5BodyReceiveSignalRocketReturned(body_work)) {
		
		// 腕表示
		body_work->flag	&= ~(GMD_BOSS5_BODY_FLAG_HIDE_ARM_L |
							 GMD_BOSS5_BODY_FLAG_HIDE_ARM_R);
		
		// 復帰アクション設定
		gmBoss5BodySetActionWhole(body_work, GME_BOSS5_ACT_ID_RPC_STR_DMG_RECOVER);
		
		body_work->proc_update	= gmBoss5BodySubSeqUpdateRpcStrDmgWithRecover;
	}
}

// のけぞりサブシーケンス更新 復帰処理
void gmBoss5BodySubSeqUpdateRpcStrDmgWithRecover(GMS_BOSS5_BODY_WORK *body_work)
{
	if (GmBsCmnIsActionEnd(GMM_BS_OBJ(body_work))) {
		
		// デフォルト矩形設定
		gmBoss5BodyChangeRectSettingDefault(body_work);
		
		gmBoss5BodyProceedToNextSeqStr(body_work);
	}
}



// ############################################################################
// ボスFINAL 中心オブジェクト
// ############################################################################

// ============================================================================
// 補助関数
// ============================================================================

// ============================================================================
// 処理関数
// ============================================================================

// ============================================================================
// 制御処理
// ============================================================================
// =======================================================================
// gmBoss5CoreWaitSetup
/*!
  中心オブジェクト セットアップ完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5CoreWaitSetup(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS5_CORE_WORK	*core_work	= (GMS_BOSS5_CORE_WORK*)obj_work;
	
	// 全ての構成パーツのセットアップを待つ
	if (parent_body->mgr_work->flag & GMD_BOSS5_MGR_FLAG_SETUP_END) {
		
		obj_work->ppFunc	= gmBoss5CoreMain;
		
		// シーケンス初期化
		gmBoss5CoreProcInit(core_work);
	}
}

// =======================================================================
// gmBoss5CoreMain
/*!
  中心オブジェクト メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5CoreMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_CORE_WORK	*core_work	= (GMS_BOSS5_CORE_WORK*)obj_work;
	
	// 更新処理
	if (core_work->proc_update) {
		core_work->proc_update(core_work);
	}
}

// ============================================================================
// シーケンス
// ============================================================================

// =======================================================================
// gmBoss5CoreProc****
/*!
  中心オブジェクトシーケンス
  
  @param core_work	[io]	中心オブジェクトワーク
 */
// =======================================================================
// 中心オブジェクトシーケンス初期化
void gmBoss5CoreProcInit(GMS_BOSS5_CORE_WORK *core_work)
{
	core_work->proc_update	= gmBoss5CoreProcUpdateLoop;
}

// 中心オブジェクトシーケンス更新 ループ処理
void gmBoss5CoreProcUpdateLoop(GMS_BOSS5_CORE_WORK *core_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(core_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	// 自分の位置を胴体中心に調整
	GmBsCmnUpdateObjectGeneralStuckWithNodeRelative(obj_work,
													&parent_body->snm_work,
													parent_body->body_snm_reg_id,
													&obj_work->parent_obj->pos,
													&parent_body->pivot_prev_pos);
	
	// 本体の矩形位置を調整
	gmBoss5BodyUpdateMainRectPosition(parent_body);
	
	// 追加の矩形位置を調整
	gmBoss5BodyUpdateSubRectPosition(parent_body);
	
	if (parent_body->flag & GMD_BOSS5_BODY_FLAG_DENY_HOMING_ATK) {
		// ホーミングアタックの対象からはずす
		((GMS_ENEMY_COM_WORK*)obj_work)->enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
	}
	else {
		// ホーミングアタックの対象にする
		((GMS_ENEMY_COM_WORK*)obj_work)->enemy_flag	&= ~GMD_ENEMY_FLAG_NOHOMING;
	}
}

// ############################################################################
// ボスFINAL 警告フェード
// ############################################################################

// =======================================================================
// gmBoss5InitAlarmFade
/*!
  警告フェード初期化
  
  @param mgr_work	[io]	管理ワーク
 */
// =======================================================================
void gmBoss5InitAlarmFade(GMS_BOSS5_MGR_WORK *mgr_work)
{
	GMS_BOSS5_ALARM_FADE_WORK	*alarm_fade;
	OBS_OBJECT_WORK	*obj_work;
	
	// 既に生成済みなら何もしない
	if (mgr_work->flag & GMD_BOSS5_MGR_FLAG_ALARM_FADE_ACTIVE) {
		return;
	}
	else {
		mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_ALARM_FADE_ACTIVE;
	}
	
	alarm_fade	=
		(GMS_BOSS5_ALARM_FADE_WORK*)GmFadeCreateFadeObj(GMD_TASK_PRIO_EFFECT,
														GMD_TASK_GROUP_EFFECT,
														GMD_TASK_PAUSELEVEL_DEF,
														sizeof(GMS_BOSS5_ALARM_FADE_WORK),
														IZD_FADE_DT_PRIO_DEF,
														OBD_DRAW_CMD_STATE_3DNN);
	
	obj_work	= (OBS_OBJECT_WORK*)alarm_fade;
	
	// 親設定
	obj_work->parent_obj	= GMM_BS_OBJ(mgr_work);
	
	// 管理ワークへの参照設定
	alarm_fade->mgr_work	= mgr_work;
	
	// フェード初期化（空設定）
	GmFadeSetFade(&alarm_fade->fade_obj,
				  IZE_FADE_SET_TYPE_NORMAL,
				  0x00, 0x00, 0x00, 0x00,
				  0x00, 0x00, 0x00, 0x00,
				  1,//0不可
				  FALSE,
				  FALSE);
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss5AlarmFadeMain;
	
	// シーケンス初期化
	gmBoss5AlarmFadeProcInit(alarm_fade);
}

// =======================================================================
// gmBoss5RequestClearAlarmFade
/*!
  警告フェード消去要求
  
  @param mgr_work	[io]	管理ワーク
 */
// =======================================================================
void gmBoss5RequestClearAlarmFade(GMS_BOSS5_MGR_WORK *mgr_work)
{
	mgr_work->flag	&= ~GMD_BOSS5_MGR_FLAG_ALARM_FADE_ACTIVE;
}

// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss5AlarmFadeInitFade
/*!
  警告フェード フェード処理初期化
  
  @param alarm_fade		[io]	警告フェードワーク
  @param alarm_level	[in]	警告レベル
 */
// =======================================================================
void gmBoss5AlarmFadeInitFade(GMS_BOSS5_ALARM_FADE_WORK *alarm_fade,
							  GME_BOSS5_ALARM_LEVEL alarm_level)
{
	alarm_fade->cur_phase	= GME_BOSS5_ALARM_FADE_PHASE_NONE;
	alarm_fade->cur_level	= alarm_level;
	alarm_fade->wait_timer	= 0;
	
	gmBoss5AlarmFadeUpdateFade(alarm_fade);
}

// =======================================================================
// gmBoss5AlarmFadeUpdateFade
/*!
  警告フェード フェード処理更新
  
  @param alarm_fade		[io]	警告フェードワーク
  @param alarm_level	[in]	警告レベル
 */
// =======================================================================
BOOL gmBoss5AlarmFadeUpdateFade(GMS_BOSS5_ALARM_FADE_WORK *alarm_fade)
{
	if (alarm_fade->wait_timer) {
		alarm_fade->wait_timer--;
	}
	else {
		
		if (!GmFadeIsEnd(&alarm_fade->fade_obj)) {
			return FALSE;
		}
		
		const GMS_BOSS5_ALARM_FADE_INFO	*fade_info	= &gm_boss5_alarm_fade_info[alarm_fade->cur_level];
		
		switch (alarm_fade->cur_phase) {
		case GME_BOSS5_ALARM_FADE_PHASE_NONE:
			GmFadeSetFade(&alarm_fade->fade_obj,
						  IZE_FADE_SET_TYPE_NORMAL,
						  0xFF, 0x00, 0x00, 0x00,
						  GMD_BOSS5_ALARM_FADE_DEST_RED,
						  GMD_BOSS5_ALARM_FADE_DEST_GREEN,
						  GMD_BOSS5_ALARM_FADE_DEST_BLUE,
						  GMD_BOSS5_ALARM_FADE_DEST_ALPHA,
						  (Float)fade_info->fo_frame,
						  FALSE,
						  FALSE);
			
			alarm_fade->cur_phase	= GME_BOSS5_ALARM_FADE_PHASE_FADE_OUT;
			
			break;
			
		case GME_BOSS5_ALARM_FADE_PHASE_FADE_OUT:
			alarm_fade->wait_timer	= fade_info->on_frame;
			
			alarm_fade->cur_phase	= GME_BOSS5_ALARM_FADE_PHASE_STAY_ON;
			
			break;
			
		case GME_BOSS5_ALARM_FADE_PHASE_STAY_ON:
			GmFadeSetFade(&alarm_fade->fade_obj,
						  IZE_FADE_SET_TYPE_NORMAL,
						  GMD_BOSS5_ALARM_FADE_DEST_RED,
						  GMD_BOSS5_ALARM_FADE_DEST_GREEN,
						  GMD_BOSS5_ALARM_FADE_DEST_BLUE,
						  GMD_BOSS5_ALARM_FADE_DEST_ALPHA,
						  0xFF, 0x00, 0x00, 0x00,
						  (Float)fade_info->fi_frame,
						  FALSE,
						  TRUE);
			
			alarm_fade->cur_phase	= GME_BOSS5_ALARM_FADE_PHASE_FADE_IN;
			
			break;
			
		case GME_BOSS5_ALARM_FADE_PHASE_FADE_IN:
			alarm_fade->wait_timer	= fade_info->off_frame;
			alarm_fade->cur_phase	= GME_BOSS5_ALARM_FADE_PHASE_STAY_OFF;
			break;
			
		case GME_BOSS5_ALARM_FADE_PHASE_STAY_OFF:
			return TRUE;
			
		default:
			MTM_ASSERT(FALSE);
			alarm_fade->cur_phase	= GME_BOSS5_ALARM_FADE_PHASE_NONE;
		}
	}
	
	return FALSE;
}

// =======================================================================
// gmBoss5AlarmFadeInitAlertSe
/*!
  警告フェード アラートSE処理 初期化
  
  @param alarm_fade	[io]	警告フェードワーク
 */
// =======================================================================
void gmBoss5AlarmFadeInitAlertSe(GMS_BOSS5_ALARM_FADE_WORK *alarm_fade)
{
	GMS_BOSS5_MGR_WORK	*mgr_work	= alarm_fade->mgr_work;
	
	// 参照警告レベル初期化
	alarm_fade->alert_se_ref_level	= mgr_work->alarm_level;
	
	// アラートSEタイマ設定
	GmBoss5Init1ShotTimer(&alarm_fade->alert_se_timer,
						  gm_boss5_alarm_se_interval_time_tbl[alarm_fade->alert_se_ref_level]);
	// アラートSE初回再生
	GmSoundPlaySE("FinalBoss10");
}

// =======================================================================
// gmBoss5AlarmFadeUpdateAlertSe
/*!
  警告フェード アラートSE処理 更新
  
  @param alarm_fade	[io]	警告フェードワーク
 */
// =======================================================================
void gmBoss5AlarmFadeUpdateAlertSe(GMS_BOSS5_ALARM_FADE_WORK *alarm_fade)
{
	GMS_BOSS5_MGR_WORK	*mgr_work	= alarm_fade->mgr_work;
	
	if (GmBoss5Update1ShotTimer(&alarm_fade->alert_se_timer) ||
		alarm_fade->alert_se_ref_level != mgr_work->alarm_level) {
		// 1ショットタイマが完了、もしくは警告レベルが変わったらSE再生
		
		GmSoundPlaySE("FinalBoss10");
		
		// 参照警告レベル更新
		alarm_fade->alert_se_ref_level	= mgr_work->alarm_level;
		
		// アラートSEタイマ再設定
		GmBoss5Init1ShotTimer(&alarm_fade->alert_se_timer,
							  gm_boss5_alarm_se_interval_time_tbl[alarm_fade->alert_se_ref_level]);
	}
}

// ============================================================================
// 処理関数
// ============================================================================

// ============================================================================
// 制御処理
// ============================================================================
// =======================================================================
// gmBoss5AlarmFadeMain
/*!
  警告フェード メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5AlarmFadeMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_ALARM_FADE_WORK	*alarm_fade	= (GMS_BOSS5_ALARM_FADE_WORK*)obj_work;
	GMS_BOSS5_MGR_WORK	*mgr_work	= alarm_fade->mgr_work;
	
	if (!(mgr_work->flag & GMD_BOSS5_MGR_FLAG_ALARM_FADE_ACTIVE)) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
		return;
	}
	
	// 更新処理
	if (alarm_fade->proc_update) {
		alarm_fade->proc_update(alarm_fade);
	}
}

// ============================================================================
// シーケンス
// ============================================================================
// =======================================================================
// gmBoss5AlarmFadeProc****
/*!
  警告フェード シーケンス
  
  @param alarm_fade	[io]	警告フェードワーク
 */
// =======================================================================
// 警告フェードシーケンス初期化
void gmBoss5AlarmFadeProcInit(GMS_BOSS5_ALARM_FADE_WORK *alarm_fade)
{
	GMS_BOSS5_MGR_WORK	*mgr_work	= alarm_fade->mgr_work;
	
	// フェード初期化
	gmBoss5AlarmFadeInitFade(alarm_fade, mgr_work->alarm_level);
	
	// アラートSE処理初期化
	gmBoss5AlarmFadeInitAlertSe(alarm_fade);
	
	alarm_fade->proc_update	= gmBoss5AlarmFadeProcUpdateLoop;
}

// 警告フェードシーケンス更新 ループ処理
void gmBoss5AlarmFadeProcUpdateLoop(GMS_BOSS5_ALARM_FADE_WORK *alarm_fade)
{
	GMS_BOSS5_MGR_WORK	*mgr_work	= alarm_fade->mgr_work;
	
	// アラートSE処理更新
	gmBoss5AlarmFadeUpdateAlertSe(alarm_fade);
	
	// フェード更新
	if (gmBoss5AlarmFadeUpdateFade(alarm_fade)) {
		gmBoss5AlarmFadeInitFade(alarm_fade, mgr_work->alarm_level);
	}
}

// ############################################################################
// ボスFINAL 画面フラッシュ
// ############################################################################
// =======================================================================
// gmBoss5InitFlashScreen
/*!
  撃破時画面白フラッシュ処理
  
  @note
  非表示エフェクトオブジェクトを生成しています。
 */
// =======================================================================
void gmBoss5InitFlashScreen(void)
{
	GMS_BOSS5_FLASH_SCREEN_WORK	*flash_scr;
	OBS_OBJECT_WORK	*obj_work;
	
	obj_work	= GMM_EFFECT_CREATE_WORK(sizeof(GMS_BOSS5_FLASH_SCREEN_WORK),
										 NULL,
										 0,
										 "boss5_flash_scr");
	
	flash_scr	= (GMS_BOSS5_FLASH_SCREEN_WORK*)obj_work;
	
	// 表示はしない
	obj_work->disp_flag	|= (OBD_DISP_NODISP | OBD_DISP_NOUPDATE);
	
	// クリッピングしない
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	
	// 画面フラッシュ初期化
	GmBsCmnInitFlashScreen(&flash_scr->flash_work, 
						   GMD_BOSS5_FLASH_SCREEN_FADEOUT_TIME,
						   GMD_BOSS5_FLASH_SCREEN_DURATION_TIME,
						   GMD_BOSS5_FLASH_SCREEN_FADEIN_TIME);
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss5FlashScreenMain;
}

// ============================================================================
// 補助関数
// ============================================================================
// ============================================================================
// 処理関数
// ============================================================================
// ============================================================================
// 制御関数
// ============================================================================
// ============================================================================
// シーケンス
// ============================================================================
// =======================================================================
// gmBoss5FlashScreenMain
/*!
  撃破時画面白フラッシュ処理 処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5FlashScreenMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_FLASH_SCREEN_WORK	*flash_scr	= (GMS_BOSS5_FLASH_SCREEN_WORK*)obj_work;
	
	if (GmBsCmnUpdateFlashScreen(&flash_scr->flash_work)) {
		GmBsCmnClearFlashScreen(&flash_scr->flash_work);
		
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}


// ############################################################################
// ボスFINAL 終了フェードアウト
// ############################################################################

// =======================================================================
// gmBoss5InitLastFadeOut
/*!
  クリア時 最後のフェードアウト 初期化
  
  @param mgr_work	[io]	管理ワーク
 */
// =======================================================================
void gmBoss5InitLastFadeOut(GMS_BOSS5_MGR_WORK *mgr_work)
{
	GMS_FADE_OBJ_WORK	*fade_obj_work;
	OBS_OBJECT_WORK	*obj_work;
	
	fade_obj_work	= GmFadeCreateFadeObj(GMD_TASK_PRIO_EFFECT,
										  GMD_TASK_GROUP_EFFECT,
										  GMD_TASK_PAUSELEVEL_DEF,
										  sizeof(GMS_FADE_OBJ_WORK),
										  IZD_FADE_DT_PRIO_DEF,
										  OBD_DRAW_CMD_STATE_2DAMA),
	
	obj_work	= (OBS_OBJECT_WORK*)fade_obj_work;
	
	// 管理オブジェクトを親に設定
	obj_work->parent_obj	= GMM_BS_OBJ(mgr_work);
	
	// フェード初期化
	GmFadeSetFade(fade_obj_work,
				  IZE_FADE_SET_TYPE_NORMAL,
				  0x00, 0x00, 0x00, 0x00,
				  0xFF, 0xFF, 0xFF, 0xFF,
				  GMD_BOSS5_LAST_FADE_OUT_FRAME,
				  FALSE,
				  FALSE);
	
	// 処理関数設定
	obj_work->ppFunc	= gmBoss5LastFadeOutMain;
}

// ============================================================================
// 補助関数
// ============================================================================
// ============================================================================
// 処理関数
// ============================================================================
// ============================================================================
// 制御関数
// ============================================================================
// =======================================================================
// gmBoss5LastFadeOutMain
/*!
  終了フェードアウト メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5LastFadeOutMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_FADE_OBJ_WORK	*fade_obj_work	= (GMS_FADE_OBJ_WORK*)obj_work;
	GMS_BOSS5_MGR_WORK	*mgr_work	= (GMS_BOSS5_MGR_WORK*)obj_work->parent_obj;
	
	MTM_ASSERT(mgr_work);
	
	if (GmFadeIsEnd(fade_obj_work)) {
		
		// 警告フェード消去を要求
		gmBoss5RequestClearAlarmFade(mgr_work);
		
		// FIXを非表示にする
		GmFixSetDisp(FALSE);
		
		// 前もって警告フェードなどを非表示にしておく必要があるため、
		// 終了フェードアウト通知や描画ステート変更は次のフレームで行う
		obj_work->ppFunc	= gmBoss5LastFadeOutEnd;
	}
}

// =======================================================================
// gmBoss5LastFadeOutEnd
/*!
  終了フェードアウト 終了処理
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5LastFadeOutEnd(OBS_OBJECT_WORK *obj_work)
{
	GMS_FADE_OBJ_WORK	*fade_obj_work	= (GMS_FADE_OBJ_WORK*)obj_work;
	GMS_BOSS5_MGR_WORK	*mgr_work	= (GMS_BOSS5_MGR_WORK*)obj_work->parent_obj;
	
	// フェードアウト終了通知
	mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_LAST_FADE_END;
	
	// 2D AMA が見えるようにする（3DNNステートにして、2D AMAよりも先に描く）
	fade_obj_work->fade_work.draw_state	= OBD_DRAW_CMD_STATE_3DNN;
	
	// 何もしない（表示は続ける）
	obj_work->ppFunc	= NULL;
}


// ############################################################################
// ボスFINAL パーツ飛散
// ############################################################################

// =======================================================================
// gmBoss5InitScatter
/*!
  パーツ飛散 初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5InitScatter(GMS_BOSS5_BODY_WORK *body_work)
{
	GMS_BS_CMN_NODE_CTRL_OBJECT	*ndc_obj;
	
	// REMINDER : gm_boss_scatter_ndc_idx_tblと同じならびにすること
	const Sint32 ndc_snm_id_tbl[GMD_BOSS5_SCT_NDC_NUM]	= {
		body_work->armpt_snm_reg_ids[GME_BOSS5_ARM_TYPE_LEFT][GME_BOSS5_ARMPART_IDX_FOREARM],
		body_work->armpt_snm_reg_ids[GME_BOSS5_ARM_TYPE_RIGHT][GME_BOSS5_ARMPART_IDX_FOREARM],
		body_work->groin_snm_reg_ids[GME_BOSS5_LEG_TYPE_LEFT],
		body_work->groin_snm_reg_ids[GME_BOSS5_LEG_TYPE_RIGHT],
		body_work->leg_snm_reg_ids[GME_BOSS5_LEG_TYPE_LEFT],
		body_work->leg_snm_reg_ids[GME_BOSS5_LEG_TYPE_RIGHT],
	};
	
	NNS_MATRIX	unit_mtx;
	
	nnMakeUnitMatrix(&unit_mtx);
	
	// CNM各ノードの設定初期化
	for (Sint32 i = 0; i < GME_BOSS5_SCT_PART_IDX_MAX; ++i) {
		const GMS_BOSS5_SCT_PART_INFO *sct_info	= &gm_boss5_scatter_parts_info_tbl[i];
		
		GmBsCmnChangeCNMModeNode(&body_work->cnm_mgr_work,
								 body_work->scatter_cnm_reg_ids[i],
								 sct_info->cnm_mode);
		
		GmBsCmnEnableCNMLocalCoordinate(&body_work->cnm_mgr_work,
										body_work->scatter_cnm_reg_ids[i],
										sct_info->is_local_coord);
		GmBsCmnEnableCNMInheritNodeScale(&body_work->cnm_mgr_work,
										 body_work->scatter_cnm_reg_ids[i],
										 sct_info->is_inherit_scale);
		GmBsCmnSetCNMMtx(&body_work->cnm_mgr_work,
						 &unit_mtx,
						 body_work->scatter_cnm_reg_ids[i],
						 TRUE);	// 一先ず全部反映（NDC対象のノードはこの後再度無効化）
	}
	
	// ノード操作オブジェクト生成
	for (Sint32 i = 0; i < GMD_BOSS5_SCT_NDC_NUM; ++i) {
		GMS_BOSS5_SCT_PART_NDC_WORK	*sct_part_ndc;
		const GMS_BOSS5_SCT_NDC_INFO	*sct_ndc_info	= &gm_boss5_scatter_ndc_info_tbl[i];
		GME_BOSS5_SCT_PART_IDX sct_part_idx	= sct_ndc_info->part_idx;
		
		// NDC対象のノードは最初は反映させない
		GmBsCmnEnableCNMMtxNode(&body_work->cnm_mgr_work,
								body_work->scatter_cnm_reg_ids[sct_part_idx],
								FALSE);
		
		ndc_obj	= GmBsCmnCreateNodeControlObjectBySize(GMM_BS_OBJ(body_work),
													   &body_work->cnm_mgr_work,
													   body_work->scatter_cnm_reg_ids[sct_part_idx],
													   &body_work->snm_work,
													   ndc_snm_id_tbl[i],
													   sizeof(GMS_BOSS5_SCT_PART_NDC_WORK));
		
		sct_part_ndc	= (GMS_BOSS5_SCT_PART_NDC_WORK*)ndc_obj;
		
		// 遅延時間を既定値に設定
		ndc_obj->user_timer	= sct_ndc_info->delay_time;
		
		// ワーク設定
		ndc_obj->is_enable	= FALSE;	// 最初は反映しない
		
		// パーツ毎の初期パラメータ設定
		gmBoss5ScatterSetPartParam(sct_part_ndc);
		
		// 地面に当たらない
		GMM_BS_OBJ(ndc_obj)->move_flag	|= (OBD_MOVE_NOCOLOBJ | OBD_MOVE_NOCOLFIELD);
		
		// ユーザー使用クォータニオン初期化
		nnMakeUnitQuaternion(&ndc_obj->user_quat);
		
		// 処理関数設定
		ndc_obj->proc_update	= gmBoss5ScatterProcWait;
	}
	
	
	// 腕に飛散動作開始を通知
	body_work->flag	|= GMD_BOSS5_BODY_FLAG_RKT_SCATTER_NEEDED;
}

// ============================================================================
// 補助関数
// ============================================================================
// ============================================================================
// 処理関数
// ============================================================================
// =======================================================================
// gmBoss5ScatterSetPartParam
/*!
  パーツ飛散エフェクト パーツ毎パラメータ設定
  
  @param sct_part_ndc	[io]	パーツ操作NDCワーク
  
  @note
  パーツ毎のパラメータ設定を行います。
 */
// =======================================================================
void gmBoss5ScatterSetPartParam(GMS_BOSS5_SCT_PART_NDC_WORK *sct_part_ndc)
{
	// 既定回数ひねりを加える差分回転クォータニオンを設定
	nnMakeUnitQuaternion(&sct_part_ndc->spin_quat);
	for (Sint32 i = 0; i < GMD_BOSS5_SCT_SPIN_AXIS_NUM; ++i) {
		Float	rand_z;
		Angle16	rand_angle;
		NNS_VECTOR	spin_axis;
		AMS_QUAT	diff_rot;
		
		// ランダムな回転軸を設定
		rand_z	= FX_FX32_TO_F32(AkMathRandFx()) * 2.f - 1.f;	// -1.f ～ 1.f
		rand_z	= MTM_MATH_CLIP(rand_z, -1.f, 1.f);
		rand_angle	= AKM_DEGtoA16(360.f * FX_FX32_TO_F32(AkMathRandFx()));	// 0～360degのランダム角度
		AkMathGetRandomUnitVector(&spin_axis, rand_z, rand_angle);
		
		nnMakeRotateAxisQuaternion(&diff_rot, spin_axis.x, spin_axis.y, spin_axis.z,
								   GMD_BOSS5_SCT_SPIN_SPD_ANGLE);
		nnMultiplyQuaternion(&sct_part_ndc->spin_quat, &diff_rot, &sct_part_ndc->spin_quat);
	}
}


// ============================================================================
// 制御処理
// ============================================================================
// ============================================================================
// シーケンス
// ============================================================================
// =======================================================================
// gmBoss5ScatterProc***
/*!
  パーツ飛散エフェクト パーツ更新関数
 */
// =======================================================================
// パーツ更新 飛散前待機処理
void gmBoss5ScatterProcWait(OBS_OBJECT_WORK *obj_work)
{
	GMS_BS_CMN_NODE_CTRL_OBJECT	*ndc_obj	= (GMS_BS_CMN_NODE_CTRL_OBJECT*)obj_work;
	
	if (ndc_obj->user_timer) {
		ndc_obj->user_timer--;
	}
	else {
		// 初期位置設定
		GmBsCmnAttachNCObjectToSNMNode(ndc_obj);
		
		// 飛散パラメータ設定
		GmBoss5ScatterSetFlyParam(obj_work);
		
		// ノードへのマトリクス反映有効化
		ndc_obj->is_enable	= TRUE;
		
		// 消去タイマ設定
		ndc_obj->user_timer	= GMD_BOSS5_SCT_PART_FLY_DELETE_TIME;
		
		// 小爆発生成
		GmBoss5EfctCreateSmallExplosion(obj_work->pos.x,
										obj_work->pos.y,
										obj_work->pos.z + GMD_BOSS5_EXPL_OFST_Z);
		
		// 処理関数設定
		ndc_obj->proc_update	= gmBoss5ScatterProcFly;
	}
	
}

// パーツ更新 飛散処理
void gmBoss5ScatterProcFly(OBS_OBJECT_WORK *obj_work)
{
	GMS_BS_CMN_NODE_CTRL_OBJECT	*ndc_obj	= (GMS_BS_CMN_NODE_CTRL_OBJECT*)obj_work;
	GMS_BOSS5_SCT_PART_NDC_WORK	*sct_part_ndc	= (GMS_BOSS5_SCT_PART_NDC_WORK*)ndc_obj;
	
	// 回転運動させる
	nnMultiplyQuaternion(&ndc_obj->user_quat, &sct_part_ndc->spin_quat, &ndc_obj->user_quat);
	
	// 姿勢設定
	GmBsCmnSetWorldMtxFromNCObjectPosture(ndc_obj);
	
	// 既定時間後に消去
	if (ndc_obj->user_timer) {
		ndc_obj->user_timer--;
	}
	else {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}


// ############################################################################
// ボスFINAL 動作シーケンス
// ############################################################################

// =======================================================================
// gmBoss5BodySeqTryRequestEnableStr
/*!
  凶暴化設定要求試行
  
  @param body_work	[io]	本体ワーク
  
  @note
  凶暴化条件を満たしているかチェックし、条件を満たしていれば凶暴化状態にへの以降をリクエストします。
  ライフ値変更時に呼び出してください。
 */
// =======================================================================
void gmBoss5BodySeqTryRequestEnableStr(GMS_BOSS5_BODY_WORK *body_work)
{
	MTM_ASSERT(body_work);
	
	// REMINDER :
	//	この関数はダメージ時に毎回必ず呼び出しているので、
	//  試行成功時の処理が何度行われても問題ないようにしておくこと
	//  （i.e. シグナルフラグのような、チェック後にオフされるフラグは使用しない、
	//         条件を満たさなかったときにフラグオフ操作を行わない）
	
	if (body_work->mgr_work->life <= GMD_BOSS5_STRONG_MODE_THRESHOLD_LIFE) {
		body_work->mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_REQUEST_STRONG_MODE;
	}
	
	/* 一旦凶暴化したら元に戻らないのでフラグオフ処理は行わない */
}

// =======================================================================
// gmBoss5BodySeqTryEnableStr
/*!
  凶暴化設定
  
  @param body_work	[io]	本体ワーク
  
  @note
  凶暴化リクエストが発行されていた場合、凶暴化状態に設定します。
 */
// =======================================================================
void gmBoss5BodySeqTryEnableStr(GMS_BOSS5_BODY_WORK *body_work)
{
	MTM_ASSERT(body_work);
	
	if (body_work->mgr_work->flag & GMD_BOSS5_MGR_FLAG_REQUEST_STRONG_MODE) {
		body_work->mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_STRONG_MODE;
	}
	
	/* 一旦凶暴化したら元に戻らないのでフラグオフ処理は行わない */
}

// =======================================================================
// gmBoss5BodySeqIsStr
/*!
  凶暴状態か判定
  
  @param body_work	[in]	本体ワーク
  
  @retval TRUE	凶暴状態
  @retval FALSE	凶暴状態でない
 */
// =======================================================================
BOOL gmBoss5BodySeqIsStr(GMS_BOSS5_BODY_WORK *body_work)
{
	MTM_ASSERT(body_work);
	
	if (body_work->mgr_work->flag & GMD_BOSS5_MGR_FLAG_STRONG_MODE) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss5BodySeqIsNearDeath
/*!
  瀕死状態チェック
  
  @param body_work	[in]	本体ワーク
  
  @retval TRUE	瀕死
  @retval FALSE	瀕死でない
 */
// =======================================================================
BOOL gmBoss5BodySeqIsNearDeath(const GMS_BOSS5_BODY_WORK *body_work)
{
	MTM_ASSERT(body_work);
	
	if (body_work->mgr_work->life <= 1) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss5BodySeqLotStrat
/*!
  戦略分岐 抽選
  
  @param strat_branch	[in]	戦略フロー分岐ポイント
  @param b_no_rkt		[in]	ロケットパンチ選択肢除外フラグ
  								（TRUE:除外, FALSE:除外しない,  デフォルト:FALSE）
  
  @return 抽選結果の戦略ステート
  
  @note
  指定分岐ポイントの分岐抽選を行います。
 */
// =======================================================================
GME_BOSS5_STRAT_STATE gmBoss5BodySeqLotStrat(GME_BOSS5_BODY_STRAT_BRANCH strat_branch,
											 BOOL b_no_rkt/*=FALSE*/)
{
	const GMS_BOSS5_STRAT_PROB_INFO *prob_info_list	= gm_boss5_body_seq_strat_branch_prob_tbl[strat_branch];
	fx32	rand_val;
	Sint32	choice_no;
	Sint32	last_valid_choice	= 0;
	fx32	threshold	= 0;
	
	rand_val	= AkMathRandFx();
	
	if (b_no_rkt) {
		fx32	rand_val_factor	= FX32_ONE;
		// ロケットパンチを除外する場合は当該選択肢の確率値を除外した
		// 確率最大値までの範囲にrand_valをマッピング
		for (Sint32 i = 0; i < GMD_BOSS5_BODY_STRAT_BRANCH_NUM; ++i) {
			if (prob_info_list[i].is_rkt) {
				rand_val_factor	-= prob_info_list[i].probability;
				MTM_ASSERT(rand_val_factor >= 0);
				rand_val_factor	= MTM_MATH_CLIP(rand_val_factor, 0, FX32_ONE);
			}
		}
		rand_val	= FX_Mul(rand_val, rand_val_factor);
		rand_val	= MTM_MATH_CLIP(rand_val, 0, rand_val_factor);
	}
	
	// どの選択肢が当たったかチェック
	for (choice_no = 0; choice_no < GMD_BOSS5_BODY_STRAT_BRANCH_NUM; ++choice_no) {
		fx32	prob	= prob_info_list[choice_no].probability;
		
		if (b_no_rkt && prob_info_list[choice_no].is_rkt) {
			// ロケットパンチ除外時に、選択肢がロケットパンチならスキップ
			continue;
		}
		
		if (prob > 0) {
			last_valid_choice	= choice_no;
			
			threshold	+= prob;
			
			if (rand_val <= threshold) {
				return prob_info_list[choice_no].strat_state;
			}
		}
	}
	
	// 閾値チェックに漏れた場合
	// 有効な確率値が設定されている選択肢のうち、
	// 最も後方に位置している選択肢を採用します。
	return prob_info_list[last_valid_choice].strat_state;
}

// =======================================================================
// gmBoss5BodyProceedToNextSeqNml
/*!
  通常時 次ステート決定・遷移
  
  @param body_work	[io]	本体ワーク
  
  @note
  現ステート・前ステート・各種条件などに基づいて次のステートを決定し、
  ステート遷移を行います。
 */
// =======================================================================
void gmBoss5BodyProceedToNextSeqNml(GMS_BOSS5_BODY_WORK *body_work)
{
	GME_BOSS5_BODY_STATE	next_state	= body_work->state;
	GME_BOSS5_STRAT_STATE	strat_state	= GME_BOSS5_STRAT_STATE_NONE;
	
	// 凶暴化リクエストチェック＆凶暴化設定
	gmBoss5BodySeqTryEnableStr(body_work);
	
	if (gmBoss5BodySeqIsStr(body_work)) {
		// 凶暴化時は凶暴化演出へ移行
		strat_state	= GME_BOSS5_STRAT_STATE_BERSERK;
	}
	else {
		BOOL	b_no_rkt	= FALSE;
		
		// プレイヤーが背後にいるかチェック
		if (gmBoss5BodyIsPlayerBehind(body_work)) {
			b_no_rkt	= TRUE;
		}
		
		switch (body_work->strat_state) {
		case GME_BOSS5_STRAT_STATE_NONE:
			MTM_ASSERT(0);
			break;
			
		case GME_BOSS5_STRAT_STATE_START:
			strat_state	= GME_BOSS5_STRAT_STATE_NML_MOVE_A;
			break;
			
		case GME_BOSS5_STRAT_STATE_NML_MOVE_A:	// no break
		case GME_BOSS5_STRAT_STATE_NML_MOVE_B:	// no break
		case GME_BOSS5_STRAT_STATE_NML_MOVE_C:
			strat_state	= GME_BOSS5_STRAT_STATE_NML_STOMP_A;
			break;
			
		case GME_BOSS5_STRAT_STATE_NML_STOMP_A:
			// 通常時確率Aで決定
			strat_state	= gmBoss5BodySeqLotStrat(GME_BOSS5_BODY_STRAT_BRANCH_NML_A,
												 b_no_rkt);
			break;
			
		case GME_BOSS5_STRAT_STATE_NML_STOMP_B:
			strat_state	= GME_BOSS5_STRAT_STATE_NML_STOMP_A;
			break;
			
		case GME_BOSS5_STRAT_STATE_NML_RPUNCH:
			// 通常時確率Bで決定
			strat_state	= gmBoss5BodySeqLotStrat(GME_BOSS5_BODY_STRAT_BRANCH_NML_B,
												 b_no_rkt);
			break;
			
		default:
			MTM_ASSERT(0);
			break;
		}
	}
	
	// 次の本体ステート取得
	next_state	= gm_boss5_body_state_strat_tbl[strat_state];
	
	// ステート変更
	gmBoss5BodyChangeState(body_work, next_state, strat_state, TRUE);
}

// =======================================================================
// gmBoss5BodyProceedToNextSeqStr
/*!
  凶暴時 次ステート決定・遷移
  
  @param body_work	[io]	本体ワーク
  
  @note
  現ステート・前ステート・各種条件などに基づいて次のステートを決定し、
  ステート遷移を行います。
 */
// =======================================================================
void gmBoss5BodyProceedToNextSeqStr(GMS_BOSS5_BODY_WORK *body_work)
{
	GME_BOSS5_BODY_STATE	next_state	= body_work->state;
	GME_BOSS5_STRAT_STATE	strat_state	= GME_BOSS5_STRAT_STATE_NONE;
	
#if defined(GMD_BOSS5_DEBUG_SEQ_TEST)
//	strat_state	= GME_BOSS5_STRAT_STATE_STR_RPUNCH_A;
//	next_state	= gm_boss5_body_state_strat_tbl[strat_state];
//	gmBoss5BodyChangeState(body_work, next_state, strat_state, TRUE);
//	return;
#endif	/* defined(GMD_BOSS5_DEBUG_SEQ_TEST) */
	
	if (gmBoss5BodySeqIsNearDeath(body_work)) {
		// 瀕死状態なら地球割りへ移行
		strat_state	= GME_BOSS5_STRAT_STATE_CRASH;
	}
	else {
		BOOL	b_no_rkt	= FALSE;
		
		// プレイヤーが背後にいるかチェック
		if (gmBoss5BodyIsPlayerBehind(body_work)) {
			b_no_rkt	= TRUE;
		}
		
		switch (body_work->strat_state) {
		case GME_BOSS5_STRAT_STATE_NONE:
			MTM_ASSERT(0);
			break;
			
		case GME_BOSS5_STRAT_STATE_BERSERK:
			strat_state	= GME_BOSS5_STRAT_STATE_STR_MOVE_A;
			break;
			
		case GME_BOSS5_STRAT_STATE_STR_MOVE_A:
			strat_state	= GME_BOSS5_STRAT_STATE_STR_STOMP_A;
			break;
			
		case GME_BOSS5_STRAT_STATE_STR_MOVE_B:	// no break
		case GME_BOSS5_STRAT_STATE_STR_STOMP_B:
			// 凶暴時確率Cは100%ロケットパンチ。
			// ただし、ジャンピングストンピングは
			// 0%の選択肢（プレイヤーが背後にいたとき用）として残しておく
			if (b_no_rkt) {
				// プレイヤーが背後にいるときはストンプ
				strat_state	= GME_BOSS5_STRAT_STATE_STR_STOMP_A;
			}
			else {
				// 通常
				strat_state	= GME_BOSS5_STRAT_STATE_STR_RPUNCH_B;
			}
			break;
			
		case GME_BOSS5_STRAT_STATE_STR_MOVE_C:
			strat_state	= GME_BOSS5_STRAT_STATE_STR_STOMP_A;
			break;
			
		case GME_BOSS5_STRAT_STATE_STR_STOMP_A:
			// 凶暴時確率Aで決定
			strat_state	= gmBoss5BodySeqLotStrat(GME_BOSS5_BODY_STRAT_BRANCH_STR_A,
												 b_no_rkt);
			break;
			
		case GME_BOSS5_STRAT_STATE_STR_RPUNCH_A:
			// 凶暴時確率Bで決定
			strat_state	= gmBoss5BodySeqLotStrat(GME_BOSS5_BODY_STRAT_BRANCH_STR_B,
												 b_no_rkt);
			break;
			
		case GME_BOSS5_STRAT_STATE_STR_RPUNCH_B:
			strat_state	= GME_BOSS5_STRAT_STATE_STR_STOMP_A;
			break;
			
		default:
			MTM_ASSERT(0);
			break;
		}
	}
	
	// 次の本体ステート取得
	next_state	= gm_boss5_body_state_strat_tbl[strat_state];
	
	// 初期パラメータ設定
	if (next_state == GME_BOSS5_BODY_STATE_MOVE_FAST_FWD) {
		gmBoss5BodySetMoveFastTime(body_work, GMD_BOSS5_BODY_RUN_DURATION_TIME);
	}
	else {
		gmBoss5BodySetMoveFastTime(body_work, 0);
	}
	
	// ステート変更
	gmBoss5BodyChangeState(body_work, next_state, strat_state, TRUE);
}

// =======================================================================
// gmBoss5BodyProceedToDefeatState
/*!
  撃破ステートへ遷移
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5BodyProceedToDefeatState(GMS_BOSS5_BODY_WORK *body_work)
{
	gmBoss5BodyChangeState(body_work, GME_BOSS5_BODY_STATE_DEFEAT,
						   GME_BOSS5_STRAT_STATE_NONE, TRUE);
}

// =======================================================================
// gmBoss5BodySeqGetRpcNmlSearchTime
/*!
  通常ロケットパンチ サーチ時間取得
  
  @param body_work	[in]	本体ワーク
  
  @return サーチ時間
 */
// =======================================================================
Uint32 gmBoss5BodySeqGetRpcNmlSearchTime(const GMS_BOSS5_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
	
	if (AkMathRandFx() < GMD_BOSS5_BODY_SEQ_RPUNCH_NML_FASTSHOT_PROB) {
		return GMD_BOSS5_BODY_SEQ_RPUNCH_NML_SEARCH_TIME_SHORT;
	}
	else {
		return GMD_BOSS5_BODY_SEQ_RPUNCH_NML_SEARCH_TIME_NORMAL;
	}
}

// =======================================================================
// gmBoss5BodySeqGetRpcStrSearchTime
/*!
  強化ロケットパンチ サーチ時間取得
  
  @param body_work	[in]	本体ワーク
  
  @return サーチ時間
 */
// =======================================================================
Uint32 gmBoss5BodySeqGetRpcStrSearchTime(const GMS_BOSS5_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
	
	if (AkMathRandFx() < GMD_BOSS5_BODY_SEQ_RPUNCH_STR_FASTSHOT_PROB) {
		return GMD_BOSS5_BODY_SEQ_RPUNCH_STR_SEARCH_TIME_SHORT;
	}
	else {
		return GMD_BOSS5_BODY_SEQ_RPUNCH_STR_SEARCH_TIME_NORMAL;
	}
}


// =======================================================================
// test_func
/*!
  関数機能
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return 返値説明
 
  @note
  補足説明
 */
// =======================================================================
