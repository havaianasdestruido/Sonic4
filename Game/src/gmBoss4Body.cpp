// =======================================================================
/*!
  @file	gmBoss4Body.cpp
  @brief ボス4 ボス機

  @author K.Yurita(SafariGames)
 				Copyright(c) 2009 Dimps
  $Id: gmBoss4Body.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akMath.h"
#include "gs.h"
#include "gmMain.h"
#include "gmGameDBuild.h"
#include "gmGamedat.h"
#include "gmEventTbl.h"
#include "gmCamera.h"
#include "gmSound.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectBoss.h"
#include "gmBossCommon.h"
#include "gmDeco.h"

#include "gmBoss4.h"
#include "gmBoss4Body.h"
#include "gmBoss4Capsule.h"
#include "gmBoss4Eggman.h"
#include "gmBoss4Chibi.h"
#include "gmBoss4Util.h"

#include "gmPlySeq.h"

#include "gmGmkCamScrLim.h"

#include "gmPadVib.h"
#include "gmPlyScoreDef.h"

#include "hgTrophy.h"

#if _IPHONE
#include "gmMap.h"
#endif // _IPHONE

#if	defined(_WII) && !defined(_PS3)
#pragma warn_impl_s2u_conv off		// signed unsignedの暗黙的な変換に対する警告ははずしておく(マクロ内に書き込んであるため)
#endif	//_WII

/*------ Macros --------------------------------------------------------*/

//------------------------------------------------------------------------------------------
//	マップにより修正が必要な部分
//------------------------------------------------------------------------------------------
/* 定義値 */
// 共通
#define		GMD_BOSS4_BODY_HIDE_RADIUS							((fx32)(FX32_ONE * 0))			//!< この距離分、画面端から離れたら完全に画面外に消えたと判定する
#define		GMD_BOSS4_BODY_DMG_FLICKER_RADIUS					((Float)32.f)					//!< ダメージ点滅処理用モデル半径値



//------------------------------------------------------------------------------------------------------
//! 第一段階関係
//------------------------------------------------------------------------------------------------------
// 初期開始動作関連
#define		GMD_BOSS4_BODY_START_POS_Y							GMM_BOSS4_STAGE(-120.0f, 1906-(306+120))						//!< 出現初期位置
#define		GMD_BOSS4_BODY_END_POS_Y							GMM_BOSS4_STAGE( 280.0f, 1906-(306-280))						//!< 到達位置

#define		GMD_BOSS4_SPEED_TIMES_IN_DAMAGE						GMM_BOSS4_PAL_TIME(5)			//!< ダメージ中のスピード(倍数)
// 攻撃
#define		GMD_BOSS4_BODY_PRE_ATKNML_SPD_ADD					GMM_BOSS4_PAL_SPEED(0.02f)		//!< 初期の進むスピード(加速力)
#define		GMD_BOSS4_BODY_PRE_ATKNML_SPD_MAX_ABS				GMM_BOSS4_PAL_TIME(1.0f)		//!< 初期の最大のスピード

// 通常移動関連
#define		GMD_BOSS4_BODY_ATKNML_LEFT_LIMIT					(144-100+30)						//!< 通常攻撃移動範囲左端
#define		GMD_BOSS4_BODY_ATKNML_RIGHT_LIMIT					(240+100-30)						//!< 通常攻撃移動範囲右端

#define		GMD_BOSS4_BODY_ATKNML_MOVE_FRAME					GMM_BOSS4_PAL_TIME(60*12.0f)	//!< 左右移動所要フレーム

// ターン関係
#define		GMD_BOSS4_BODY_ATKNML_DRIFT_FRAME					GMM_BOSS4_PAL_TIME(30)			//!< ドリフト移動所要フレーム
#define		GMD_BOSS4_BODY_ATKNML_DRIFT_AMP						(16)							//!< ドリフト移動幅
#define		GMD_BOSS4_BODY_ATKNML_TURN_FRAME					GMM_BOSS4_PAL_TIME(28)			//!< 通常攻撃時振り向き所要時間
																								//!< GMD_BOSS4_BODY_ATKNML_DRIFT_FRAMEよりも小さい
// ゆっくりとターンするバージョン用
#if defined(GMD_BOSS4_AKTNML_MOVE_USE_PARTIAL_CURVE)
	#define	GMD_BOSS4_BODY_ATKNML_MOVE_CURVE_ANGLE_WIDTH		(AKM_DEGtoA32(120.f))
	#define GMD_BOSS4_BODY_ATKNML_MOVE_CURVE_START_ANGLE		(AKM_DEGtoA32(30.f))
#endif /* defined(GMD_BOSS4_AKTNML_MOVE_USE_PARTIAL_CURVE) */


#define	GMD_BOSS4_BODY_UP_POS_Y		(190.0f)
#define	GMD_BOSS4_BODY_UP_POS_Y_1	(240.0f)

//------------------------------------------------------------------------------------------------------
//! ボスライト関係
//------------------------------------------------------------------------------------------------------
//#define		GMD_BOSS4_BODY_BOSSLIGHT_R_OFFSET_X					(-60.0f)			//!< ボスライト右のXオフセット
//#define		GMD_BOSS4_BODY_BOSSLIGHT_R_OFFSET_Y					(  0.0f)			//!< ボスライト右のYオフセット
//#define		GMD_BOSS4_BODY_BOSSLIGHT_R_OFFSET_Z					( 10.0f)			//!< ボスライト右のZオフセット
#define		GMD_BOSS4_BODY_BOSSLIGHT_R_OFFSET_X					(  0.0f)			//!< ボスライト右のXオフセット
#define		GMD_BOSS4_BODY_BOSSLIGHT_R_OFFSET_Y					(  0.0f)			//!< ボスライト右のYオフセット
#define		GMD_BOSS4_BODY_BOSSLIGHT_R_OFFSET_Z					(  0.0f)			//!< ボスライト右のZオフセット
#define		GMD_BOSS4_BODY_BOSSLIGHT_R_ROTATE_X					(  0.0f)			//!< ボスライト右のX回転
#define		GMD_BOSS4_BODY_BOSSLIGHT_R_ROTATE_Y					(  0.0f)			//!< ボスライト右のY回転
#define		GMD_BOSS4_BODY_BOSSLIGHT_R_ROTATE_Z					(  0.0f)			//!< ボスライト右のZ回転

//#define		GMD_BOSS4_BODY_BOSSLIGHT_L_OFFSET_X					( 60.0f)			//!< ボスライト左のXオフセット
//#define		GMD_BOSS4_BODY_BOSSLIGHT_L_OFFSET_Y					(  0.0f)			//!< ボスライト左のYオフセット
//#define		GMD_BOSS4_BODY_BOSSLIGHT_L_OFFSET_Z					( 10.0f)			//!< ボスライト左のZオフセット
#define		GMD_BOSS4_BODY_BOSSLIGHT_L_OFFSET_X					(  0.0f)			//!< ボスライト左のXオフセット
#define		GMD_BOSS4_BODY_BOSSLIGHT_L_OFFSET_Y					(  0.0f)			//!< ボスライト左のYオフセット
#define		GMD_BOSS4_BODY_BOSSLIGHT_L_OFFSET_Z					(  0.0f)			//!< ボスライト左のZオフセット
#define		GMD_BOSS4_BODY_BOSSLIGHT_L_ROTATE_X					(  0.0f)			//!< ボスライト左のX回転
#define		GMD_BOSS4_BODY_BOSSLIGHT_L_ROTATE_Y					(  0.0f)			//!< ボスライト左のY回転
#define		GMD_BOSS4_BODY_BOSSLIGHT_L_ROTATE_Z					(  0.0f)			//!< ボスライト左のZ回転

//------------------------------------------------------------------------------------------------------
//! 第一段階終了関係
//------------------------------------------------------------------------------------------------------
#define		GMD_BOSS4_BODY_1STEND_POS_Y_FROM_CENTER				(-50.0f)			//!< 第一段階終了 中心からの位置
#if _IPHONE
#define		GMD_BOSS4_BODY_1STEND_POS_Y							(220.0f)			//!< 第一段階終了 中心位置Y(カメラがずれるので強制設定)
#endif // _IPHONE
#define		GMD_BOSS4_BODY_1STEND_ARRIVED_TIME					(60*3)				//!< 第一段階終了位置に到着までの時間
#define		GMD_BOSS4_BODY_1STEND_TRUN_TIME						(40)				//!< 第一弾終了時に、振り向き(左に向く)のスピード

#define		GMD_BOSS4_BODY_1STEND_EXPLOSION_TIME				(60*3)				//!< 第一段階終了、カプセル爆発中の時間
#if _IPHONE
#define		GMD_BOSS4_BODY_1STEND_EXPLOSION_POS_Y_FROM_CENTER	(20.0f)				//!< 第一段階終了、爆発後の位置。(中心から) iPhoneではソニックに接触することがあったので少し下げ幅を少なくする
#else
#define		GMD_BOSS4_BODY_1STEND_EXPLOSION_POS_Y_FROM_CENTER	(50.0f)				//!< 第一段階終了、爆発後の位置。(中心から)
#endif // _IPHONE
#define		GMD_BOSS4_BODY_1STEND_EXPLOSION_ARRIVED_TIME		(60)				//!< 第一段階終了、爆発後の位置に到着までの時間

#define		GMD_BOSS4_BODY_1STEND_ESCAPE_POS					(200.0f)			//!< 第一段階終了、逃げきる時の位置(画面中心から)
#define		GMD_BOSS4_BODY_1STEND_ESCAPE_ARRIVED_TIME			(60*2.5)				//!< 第一段階終了、逃げきるまでの時間

//------------------------------------------------------------------------------------------------------
//! 第二段階関係
//------------------------------------------------------------------------------------------------------
//! 出現位置
#define		GMD_BOSS4_BODY_2ND_POS_X							GMM_BOSS4_STAGE( 3500, 12100 )								//((fx32)(FX32_ONE * 250))	//!< 第2形態初期の横の位置
#define		GMD_BOSS4_BODY_2ND_POS_Y							GMM_BOSS4_STAGE( 250, 1906-(306-250))								//((fx32)(FX32_ONE * 250))	//!< 第2形態初期の高さ

//! 手からカプセル
#define		GMD_BOSS4_BODY_SONIC_CTRL_TIME						GMM_BOSS4_PAL_TIME(60*4)			//!< 高速スクロールからソニックが操作可能になるまでの時間
#define		GMD_BOSS4_BODY_CREATE_CAP_FIRST_TIME				GMM_BOSS4_PAL_TIME(60*2)			//!< ボスが現れてから初期に攻撃するまでの時間

#define		GMD_BOSS4_BODY_CREATE_CAP_OFFSET_X					(0.0f)								//!< ちびエッグマンの手からのカプセル発生オフセット
#define		GMD_BOSS4_BODY_CREATE_CAP_OFFSET_Y					(-22.0f)							//!< ちびエッグマンの手からのカプセル発生オフセット
#define		GMD_BOSS4_BODY_CREATE_CAP_THROW_SPD_X_1				GMM_BOSS4_PAL_SPEED(-1.0f)			//!< カプセル投げる方向(その1)
#define		GMD_BOSS4_BODY_CREATE_CAP_THROW_SPD_Y_1				GMM_BOSS4_PAL_SPEED(-2.0f)			//!< カプセル投げる方向(その1)
#define		GMD_BOSS4_BODY_CREATE_CAP_THROW_SPD_X_2				GMM_BOSS4_PAL_SPEED(-2.0f)			//!< カプセル投げる方向(その2)
#define		GMD_BOSS4_BODY_CREATE_CAP_THROW_SPD_Y_2				GMM_BOSS4_PAL_SPEED(-2.0f)			//!< カプセル投げる方向(その2)
#define		GMD_BOSS4_BODY_CREATE_CAP_TIMING_LIFE_3				GMM_BOSS4_PAL_TIME(60*5)			//!< カプセル発生間隔
#define		GMD_BOSS4_BODY_CREATE_CAP_TIMING_LIFE_2				GMM_BOSS4_PAL_TIME(60*3)			//!< カプセル発生間隔
#define		GMD_BOSS4_BODY_CREATE_CAP_TIMING_LIFE_1				GMM_BOSS4_PAL_TIME(60*2)			//!< カプセル発生間隔

#define		GMD_BOSS4_BODY_CREATE_CAP_TIMING_LIFE_2_2			GMM_BOSS4_PAL_TIME(90)				//!< カプセル発生間隔(２発目)

//------------------------------------------------------------------------------------------------------
//	爆破関係
//------------------------------------------------------------------------------------------------------
#define		GMD_BOSS4_BODY_DEFEAT_BOMB_SMALL_TIME				GMM_BOSS4_PAL_TIME(120)				//!< 初期の小爆発の時間
#define		GMD_BOSS4_BODY_DEFEAT_BOMB_SMALL_LEN_X				(80.0f)								//!< 小爆発のXの範囲
#define		GMD_BOSS4_BODY_DEFEAT_BOMB_SMALL_LEN_Y				(80.0f)								//!< 小爆発のYの範囲
#define		GMD_BOSS4_BODY_DEFEAT_BOMB_SMALL_INTERVAL_MIN_TIME	GMM_BOSS4_PAL_TIME(10)				//!< 小爆発の最小間隔
#define		GMD_BOSS4_BODY_DEFEAT_BOMB_SMALL_INTERVAL_MAX_TIME	GMM_BOSS4_PAL_TIME(30)				//!< 小爆発の最大間隔

#define		GMD_BOSS4_BODY_DEFEAT_BOMB_PARTS_LEN_X				(80.0f)								//!< 小爆発時のパーツ飛び散り範囲X
#define		GMD_BOSS4_BODY_DEFEAT_BOMB_PARTS_LEN_Y				(80.0f)								//!< 小爆発時のパーツ飛び散り範囲Y
#define		GMD_BOSS4_BODY_DEFEAT_BOMB_PARTS_INTERVAL_MIN_TIME	GMM_BOSS4_PAL_TIME(10)				//!< パーツ飛び散り最小間隔
#define		GMD_BOSS4_BODY_DEFEAT_BOMB_PARTS_INTERVAL_MAX_TIME	GMM_BOSS4_PAL_TIME(30)				//!< パーツ飛び散り最大間隔

#define		GMD_BOSS4_BODY_DEFEAT_FLASH_INTO_TIME				(4)									//!< 撃破のフラッシュ完全に白になるまでのフレーム
#define		GMD_BOSS4_BODY_DEFEAT_FLASH_KEEP_TIME				(5)									//!< 撃破のフラッシュ完全に白の間のフレーム
#define		GMD_BOSS4_BODY_DEFEAT_FLASH_RETURN_TIME				(30)								//!< 撃破のフラッシュ完全に白から戻るまでのフレーム

//------------------------------------------------------------------------------------------------------
// 逃亡関連
//------------------------------------------------------------------------------------------------------
#define		GMD_BOSS4_BODY_ESCAPE_SPD_X_ADD						((fx32)(FX32_ONE * 0.1f))
#define		GMD_BOSS4_BODY_ESCAPE_SPD_Y_ADD						((fx32)(FX32_ONE * -0.05f))
#define		GMD_BOSS4_BODY_ESCAPE_SPD_X_MAX						((fx32)(FX32_ONE * 2.75f))
#define		GMD_BOSS4_BODY_ESCAPE_SPD_Y_MAX						((fx32)(FX32_ONE * -0.375f))


//############ アフターバーナーエフェクト #####################################
/* 定義値 */
#define GMD_BOSS4_EFF_ABURNER1_DISP_OFST_X			((Float)0.f)			//!< 表示オフセットX
#define GMD_BOSS4_EFF_ABURNER1_DISP_OFST_Y			((Float)-15.f)			//!< 表示オフセットY
#define GMD_BOSS4_EFF_ABURNER1_DISP_OFST_Z			((Float)-45.f)			//!< 表示オフセットZ

#define GMD_BOSS4_EFF_ABURNER2_DISP_OFST_X			((Float)0.f)			//!< 表示オフセットX
#define GMD_BOSS4_EFF_ABURNER2_DISP_OFST_Y			((Float)+5.f)			//!< 表示オフセットY
#define GMD_BOSS4_EFF_ABURNER2_DISP_OFST_Z			((Float)-45.f)			//!< 表示オフセットZ
/*
#define GMD_BOSS4_EFF_ABURNER3_DISP_OFST_X			((Float)-30.f)			//!< 表示オフセットX
#define GMD_BOSS4_EFF_ABURNER3_DISP_OFST_Y			((Float)-40.f)			//!< 表示オフセットY
#define GMD_BOSS4_EFF_ABURNER3_DISP_OFST_Z			((Float)+40.f)			//!< 表示オフセットZ

#define GMD_BOSS4_EFF_ABURNER4_DISP_OFST_X			((Float)+30.f)			//!< 表示オフセットX
#define GMD_BOSS4_EFF_ABURNER4_DISP_OFST_Y			((Float)-40.f)			//!< 表示オフセットY
#define GMD_BOSS4_EFF_ABURNER4_DISP_OFST_Z			((Float)+40.f)			//!< 表示オフセットZ
*/
#define GMD_BOSS4_EFF_ABURNER3_DISP_OFST_X			((Float)-0.f)			//!< 表示オフセットX
#define GMD_BOSS4_EFF_ABURNER3_DISP_OFST_Y			((Float)-0.f)//((Float)-15.f)			//!< 表示オフセットY
#define GMD_BOSS4_EFF_ABURNER3_DISP_OFST_Z			((Float)+0.f)			//!< 表示オフセットZ

#define GMD_BOSS4_EFF_ABURNER4_DISP_OFST_X			((Float)+0.f)			//!< 表示オフセットX
#define GMD_BOSS4_EFF_ABURNER4_DISP_OFST_Y			((Float)-0.f)//((Float)-15.f)			//!< 表示オフセットY
#define GMD_BOSS4_EFF_ABURNER4_DISP_OFST_Z			((Float)+0.f)			//!< 表示オフセットZ

#define GMD_BOSS4_EFF_ABURNER5_DISP_OFST_X			((Float)  0.f)			//!< 表示オフセットX(下につくバーニア)
#define GMD_BOSS4_EFF_ABURNER5_DISP_OFST_Y			((Float)-30.f)			//!< 表示オフセットY(下につくバーニア)
#define GMD_BOSS4_EFF_ABURNER5_DISP_OFST_Z			((Float)  0.f)			//!< 表示オフセットZ(下につくバーニア)

#define	GMD_BOSS4_EFF_ABURNER3_DISP_ROT_X			(AKM_DEGtoA32(90.f))	//!後ろ向けバーニア

#define	GMD_BOSS4_EFF_ABURNER5_DISP_ROT_X			(180.0f)					//!下向けバーニア
#define	GMD_BOSS4_EFF_ABURNER5_DISP_ROT_Y			(  0.0f)					//!下向けバーニア
#define	GMD_BOSS4_EFF_ABURNER5_DISP_ROT_Z			(  0.0f)					//!下向けバーニア

//############ アフターバーナー煙エフェクト ###################################
/* 定義値 */
#define GMD_BOSS4_EFF_ABSMOKE_DISP_OFST_Z			((Float)-32.f)			//!< 表示オフセットZ
//############ 本体煙エフェクト ###############################################
/* 定義値 */
#define GMD_BOSS4_EFF_BODYSMOKE_DISP_OFST_Z			((Float)-32.f)			//!< 表示オフセットZ


/*------ Macro Functions -----------------------------------------------*/
// =======================================================================
// GMM_BOSS4_MGR
/*!
  ボス1本体ワークから管理ワークを取り出す
  
  @param work	[in]	ボス４本体ワーク
  
  @return 管理ワーク(GMS_BOSS4_MGR_WORK)
 */
// =======================================================================
#define GMM_BOSS4_MGR(work)	((work)->mgr_work)

/*------ Definitions ---------------------------------------------------*/

//! 本体ステート開始関数
typedef void (*GMF_BOSS4_BODY_STATE_ENTER_FUNC)(GMS_BOSS4_BODY_WORK*);
//! 本体ステート終了関数
typedef void (*GMF_BOSS4_BODY_STATE_LEAVE_FUNC)(GMS_BOSS4_BODY_WORK*);


//! 本体ステート開始情報構造体
typedef struct tag_GMS_BOSS4_BODY_STATE_ENTER_INFO
{
	GMF_BOSS4_BODY_STATE_ENTER_FUNC	enter_func;	//!< 本体ステート開始関数
	BOOL	is_wrapped;		//!< 間接呼び出しフラグ
} GMS_BOSS4_BODY_STATE_ENTER_INFO;

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
//############ ボス4本体 #####################################################
static void gmBoss4BodyExit(MTS_TASK_TCB *tcb);
/* 補助関数 */
static void gmBoss4BodySetActionWhole(GMS_BOSS4_BODY_WORK *body_work,
									  GME_BOSS4_ACT_ID act_id, BOOL force_change=FALSE);

//static void gmBoss4BodySetSuspendAction(GMS_BOSS4_BODY_WORK *body_work,
//										GME_BOSS4_PART_IDX part_idx,
//										Uint32 suspend_time);

static void gmBoss4BodyUpdateSuspendAction(GMS_BOSS4_BODY_WORK *body_work);
//static BOOL gmBoss4BodyCheckChainMotionMergeEnd(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyExecDamageRoutine(GMS_BOSS4_BODY_WORK *body_work);
static BOOL gmBoss4BodyIsExtraAttack(GMS_BOSS4_BODY_WORK *body_work);
//static BOOL gmBoss4BodyIsEscapeScrUnlock(GMS_BOSS4_BODY_WORK *body_work);
/*
static BOOL gmBoss4BodyIsDirectionPositiveFromCurrent(GMS_BOSS4_BODY_WORK *body_work, Angle16 target_angle);
static void gmBoss4BodyUpdateDirection(GMS_BOSS4_BODY_WORK *body);
static void gmBoss4BodySetDirectionNormal(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodySetDirection(GMS_BOSS4_BODY_WORK *body_work, Angle16 deg);
static void gmBoss4BodyInitTurn(GMS_BOSS4_BODY_WORK *body_work,
								Angle32 turn_amount, Angle32 turn_spd);
static void gmBoss4BodyInitTurn(GMS_BOSS4_BODY_WORK *body_work,
								Angle16 dest_angle, Sint32 frame, BOOL is_positive);
static BOOL gmBoss4BodyUpdateTurn(GMS_BOSS4_BODY_WORK *body_work, Float spd_rate=1.f);
static void gmBoss4BodyInitTurnGently(GMS_BOSS4_BODY_WORK *body_work, Angle16 dest_angle,
									  Sint32 frame, BOOL is_positive);
static BOOL gmBoss4BodyUpdateTurnGently(GMS_BOSS4_BODY_WORK *body_work);
*/
static void gmBoss4BodyInitPreANChainMotion(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyInitPreANMove(GMS_BOSS4_BODY_WORK *body_work);
static BOOL gmBoss4BodyUpdatePreANMoveLeft(GMS_BOSS4_BODY_WORK *body_work);
static BOOL gmBoss4BodyUpdatePreANMoveRight(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodySetANChainInitialBlendSpd(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyInitAtkNmlMove(GMS_BOSS4_BODY_WORK *body_work, Sint32 frame);
static BOOL gmBoss4BodyUpdateAtkNmlMove(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodySetFlipForAtkNmlMove(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyInitAtkNmlFlipAndTurn(GMS_BOSS4_BODY_WORK *body_work);
static BOOL gmBoss4BodyUpdateAtkNmlFlipAndTurn(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyInitAtkNmlDrift(GMS_BOSS4_BODY_WORK *body_work, Sint32 frame);
static BOOL gmBoss4BodyUpdateAtkNmlDrift(GMS_BOSS4_BODY_WORK *body_work);

//static void gmBoss4BodyInitRush(GMS_BOSS4_BODY_WORK *body_work, BOOL is_left);
//static BOOL gmBoss4BodyUpdateRush(GMS_BOSS4_BODY_WORK *body_work);
//static void gmBoss4BodyInitBashReturn(GMS_BOSS4_BODY_WORK *body_work, BOOL is_left);
//static BOOL gmBoss4BodyUpdateBashReturn(GMS_BOSS4_BODY_WORK *body_work);

static void gmBoss4BodyInitEscapeMove(GMS_BOSS4_BODY_WORK *body_work);
static BOOL gmBoss4BodyUpdateEscapeMove(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyInitDefeatState(GMS_BOSS4_BODY_WORK *body_work);
/* ノード操作関連 */
static void gmBoss4BodyUpdateChainTopDirection(GMS_BOSS4_BODY_WORK *body_work);
/* 処理関数 */
static void gmBoss4BodyAtkHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);
static void gmBoss4BodyDamageDefFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);
static void gmBoss4BodyOutFunc(OBS_OBJECT_WORK *obj_work);
static void gmBoss4BodyDefHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);

/* 制御処理 */
static void gmBoss4BodyChangeState(GMS_BOSS4_BODY_WORK *body_work,
								   GME_BOSS4_BODY_STATE state, BOOL is_wrapped=FALSE);
static void gmBoss4BodyWaitLoad(OBS_OBJECT_WORK *obj_work);
static void gmBoss4BodyMain(OBS_OBJECT_WORK *obj_work);
/* シーケンス */
// 開始ステート
static void gmBoss4BodyStateEnterStart(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateLeaveStart(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateStartWithWait(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateStartWithWaitEnd(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateStartWithFall(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateStartWithFallWait(GMS_BOSS4_BODY_WORK *body_work);

//static void gmBoss4BodyStateUpdateStartWithMove(GMS_BOSS4_BODY_WORK *body_work);
//static void gmBoss4BodyStateUpdateStartWithWaitEnd(GMS_BOSS4_BODY_WORK *body_work);
// 鉄球準備ステート
//static void gmBoss4BodyStateEnterPrep(GMS_BOSS4_BODY_WORK *body_work);
//static void gmBoss4BodyStateLeavePrep(GMS_BOSS4_BODY_WORK *body_work);
//static void gmBoss4BodyStateUpdatePrepWithWait(GMS_BOSS4_BODY_WORK *body_work);
// 通常攻撃開始ステート
static void gmBoss4BodyStateEnterPreAtkNml(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateLeavePreAtkNml(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdatePreAtkNmlWithMoveRight(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdatePreAtkNmlWithMoveLeft(GMS_BOSS4_BODY_WORK *body_work);
// 通常攻撃ステート
static void gmBoss4BodyStateEnterAtkNml(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateLeaveAtkNml(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateAtkNmlWithTurn(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateAtkNmlWithMove(GMS_BOSS4_BODY_WORK *body_work);

/*
// 叩きつけステート
static void gmBoss4BodyStateEnterAtkBash(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateLeaveAtkBash(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateAtkBashWithLock(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateAtkBashWithPrep(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateAtkBashWithSwing(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateAtkBashWithFinish(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateAtkBashWithHoming(GMS_BOSS4_BODY_WORK *body_work);
*/


// 第一段階終了ステート

static void gmBoss4BodyStateEnter1stEnd(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateLeave1stEnd(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdate1stEnd(GMS_BOSS4_BODY_WORK *body_work);

static void gmBoss4BodyStateInit1stEndExplosion(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdate1stEndExplosion(GMS_BOSS4_BODY_WORK *body_work);

static void gmBoss4BodyStateInit1stEndAngry(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdate1stEndAngry(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdate1stEndAngryL2(GMS_BOSS4_BODY_WORK *body_work);

static void gmBoss4BodyStateInit1stEndEscape(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdate1stEndEscape(GMS_BOSS4_BODY_WORK *body_work);

// 第2段階開始
static void gmBoss4BodyStateEnter2nd(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateLeave2nd(GMS_BOSS4_BODY_WORK *body_work);

static void gmBoss4BodyStateInit2nd(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdate2nd(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdate2ndWaitBoss(GMS_BOSS4_BODY_WORK *body_work);

static void gmBoss4BodyStateInit2ndAttack(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdate2ndAttackWait(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateInit2ndAttack2(GMS_BOSS4_BODY_WORK *body_work);		// ２発目攻撃
static void gmBoss4BodyStateUpdate2ndAttack(GMS_BOSS4_BODY_WORK *body_work);



// 通常ダメージステート
static void gmBoss4BodyStateEnterDmgNml(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateLeaveDmgNml(GMS_BOSS4_BODY_WORK *body_work);
// 撃破ステート
static void gmBoss4BodyStateEnterDefeat(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateLeaveDefeat(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateDefeatWithWaitStart(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateDefeatWithExplode(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateDefeatWithScatter(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateDefeatWithWaitEnd(GMS_BOSS4_BODY_WORK *body_work);
// 逃亡ステート
static void gmBoss4BodyStateEnterEscape(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateLeaveEscape(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateEscapeWithTurn(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateEscapeWithMoveLocked(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateEscapeWithMoveUnlocked(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4BodyStateUpdateEscapeWithMoveFinish(GMS_BOSS4_BODY_WORK *body_work);


//############ ダメージエフェクト #############################################
//static void gmBoss4EffDamageInit(GMS_BOSS4_BODY_WORK *body_work);

//############ アフターバーナーエフェクト #####################################
static void gmBoss4EffAfterburnerSetEnable(GMS_BOSS4_BODY_WORK *body_work, GME_BOSS4_BODY_ABURNER_TYPE type );
static void gmBoss4EffAfterburnerUpdateCreate(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4EffAfterburnerInit(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4EffAfterburnerProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss4EffAfterburnerExInit(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4EffAfterburnerExProcMainL(OBS_OBJECT_WORK *obj_work);
static void gmBoss4EffAfterburnerExProcMainR(OBS_OBJECT_WORK *obj_work);

//############ アフターバーナー煙エフェクト ###################################
static void gmBoss4EffABSmokeInit(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4EffABSmokeProcMain(OBS_OBJECT_WORK *obj_work);

//############ 本体黒煙エフェクト #############################################
static void gmBoss4EffBodySmokeInit(GMS_BOSS4_BODY_WORK *body_work);
static void gmBoss4EffBodySmokeProcMain(OBS_OBJECT_WORK *obj_work);

//############ ライトエフェクト #############################################
static void gmBoss4EffBossLightSetEnable(GMS_BOSS4_BODY_WORK *body_work, BOOL on );
static void gmBoss4EffBossLightUpdateCreate(GMS_BOSS4_BODY_WORK *body_work);

#if defined(MTD_DEBUG)
//############ デバッグ ###############################################
#endif /* defined(MTD_DEBUG) */

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
//! 本体 ステート開始関数テーブル
const static GMS_BOSS4_BODY_STATE_ENTER_INFO gm_boss4_body_state_enter_info_tbl[GME_BOSS4_BODY_STATE_MAX]	= {
	{	NULL,							FALSE	},
	{	gmBoss4BodyStateEnterStart,		FALSE	},
//	{	gmBoss4BodyStateEnterPrep,		FALSE	},
	{	gmBoss4BodyStateEnterPreAtkNml,	FALSE	},
	{	gmBoss4BodyStateEnterAtkNml,	FALSE	},
//	{	gmBoss4BodyStateEnterAtkBash,	FALSE	},
	{	gmBoss4BodyStateEnter1stEnd,	FALSE	},
	{	gmBoss4BodyStateEnter2nd,		FALSE	},
	{	gmBoss4BodyStateEnterDmgNml,	FALSE	},
	{	gmBoss4BodyStateEnterDefeat,	TRUE	},
	{	gmBoss4BodyStateEnterEscape,	FALSE	},
};

//! 本体 ステート終了関数テーブル
const static GMF_BOSS4_BODY_STATE_LEAVE_FUNC gm_boss4_body_state_leave_func_tbl[GME_BOSS4_BODY_STATE_MAX]	= {
	NULL,
	gmBoss4BodyStateLeaveStart,
//	gmBoss4BodyStateLeavePrep,
	gmBoss4BodyStateLeavePreAtkNml,
	gmBoss4BodyStateLeaveAtkNml,
//	gmBoss4BodyStateLeaveAtkBash,
	gmBoss4BodyStateLeave1stEnd,
	gmBoss4BodyStateLeave2nd,
	gmBoss4BodyStateLeaveDmgNml,
	gmBoss4BodyStateLeaveDefeat,
	gmBoss4BodyStateLeaveEscape,
};

Sint32	gm_boss4_locking = 0;
/*------ Global Functions ----------------------------------------------*/

// =======================================================================
// GmBoss4BodyBuild
/*!
  ボス4 データ構築
 */
// =======================================================================
void GmBoss4BodyBuild(void)
{
	// モーション
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_BODY_MTN),
						IDB_BOSS04_BOSS04_BODY_MTN_AMB, GMD_BOSS4_ARC);
}


// =======================================================================
// GmBoss4BodyFlush
/*!
  ボス4 データ片付け
 */
// =======================================================================
void GmBoss4BodyFlush(void)
{
	// モーション
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_04_BODY_MTN));
}



// =======================================================================
// GmBoss4BodyInit
/*!
  ボス4本体初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4BodyInit(GMS_EVE_RECORD_EVENT *eve_rec,
								 fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	
	OBS_OBJECT_WORK	*obj_work;
	GMS_ENEMY_3D_WORK	*ene_3d;
	GMS_BOSS4_BODY_WORK	*body_work;
	
	// オブジェクト作成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x, pos_y,
										sizeof(GMS_BOSS4_BODY_WORK),
										"BOSS4_BODY");
	
	ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	body_work	= (GMS_BOSS4_BODY_WORK*)obj_work;
	
	// 初期位置設定
//	OBS_CAMERA*	obj_cam = ObjCameraGet(GME_CAMERA_NO_MAIN);
//	obj_work->pos.x = obj_cam->target_pos.x;
//	obj_work->pos.y	= -obj_cam->target_pos.y - 420;//GMD_BOSS4_BODY_START_POS_Y;

	obj_work->pos.y	= FX_F32_TO_FX32( GMD_BOSS4_BODY_START_POS_Y );
	
	// Z位置設定
	obj_work->pos.z	= GMD_BOSS4_DEFAULT_POS_Z;
	
	// 通常攻撃高度設定
	body_work->atk_nml_alt	= FX_F32_TO_FX32( GMD_BOSS4_BODY_END_POS_Y );

	// ワーク設定
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag	|= OBD_DISP_HFLIP | OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP;
	obj_work->move_flag	|= OBD_MOVE_NOCOLFIELD;
	obj_work->move_flag	&= ~OBD_MOVE_FALL;
	
	// ライフ設定
	//TODO : 未実装
	ene_3d->ene_com.vit	= 1;// TODO : 仮
	
	// 地形当たり
	//TODO : 未実装
	
	// TODO : 食らい当たり設定(地面のごろごろアタックは無効にする)
	ObjRectWorkSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF], -40, -16, 40, 2);
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].ppDef	= gmBoss4BodyDamageDefFunc;

	// TODO : 仮
	ObjRectWorkSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK], -32, -8, 32, 40);
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].ppHit	= gmBoss4BodyAtkHitFunc;
//	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;

	// 体当たりを作ってみる
	ObjRectWorkSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY], -30, -18, 30, 40);
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].ppHit = gmBoss4BodyDefHitFunc;
	ObjRectGroupSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY], GMD_OBJ_RECT_GROUP_ENEMY, GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER);
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_NOHIT;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag |= OBD_RECT_ENABLE;


	// 本体モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
									GmBoss4GetObj3D(IDB_BOSS04_MDL_B04_BODY_ZNO),
									//&gm_boss4_obj_3d_list[IDB_BOSS01_MDL_B01_BODY_ZNO],
									&ene_3d->obj_3d);
	
	// 本体モーションロード
	ObjObjectAction3dNNMotionLoad(obj_work,
								  0,// TODO : 仮
								  TRUE,	// TODO : 仮
								  ObjDataGet(GMD_DWORK_NO_BOSS_04_BODY_MTN),
								  NULL,
								  0,
								  NULL);
	
	// Wii向けトゥーン設定
	ObjDrawObjectSetToon(obj_work);
	
	// ブレンド速度設定
	//obj_work->obj_3d->blend_spd	= 0.125f;// TODO : 仮
	
	// ユーザ描画ステートを使用
	obj_work->disp_flag	|= OBD_DISP_DRAWSTATE;
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss4BodyWaitLoad;
	
	// 初期アクション設定
	gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_NOP);
	
	// 専用描画処理設定
	obj_work->ppOut	= gmBoss4BodyOutFunc;
	
	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, gmBoss4BodyExit);
	
#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}


/*------ Static Functions ----------------------------------------------*/

// ############################################################################
// ボス4 本体
// ############################################################################

// =======================================================================
// gmBoss4BodyExit
/*!
  本体終了処理
  
  @note
  ノードマトリクス取得関連バッファを解放しています。
 */
// =======================================================================
void gmBoss4BodyExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_BOSS4_BODY_WORK	*body_work	= (GMS_BOSS4_BODY_WORK*)obj_work;
	
	// オブジェクト生成数デクリメント
	GmBoss4DecObjCreateCount();

	/*
	// ボスモーションコールバックシステム解除・クリア
	GmBsCmnClearBossMotionCBSystem(obj_work);
	
	// ノードマトリクス取得関連解放
	GmBsCmnDeleteSNMWork(&body_work->snm_work);
	*/

	GmBoss4UtilExitNodeMatrix( &body_work->node_work );	

	// ノードマトリクス操作解除・クリア
	GmBsCmnClearCNMCb(obj_work);
	
	// ノードマトリクス操作処理管理ワーク削除
	GmBsCmnDeleteCNMMgrWork(&body_work->cnm_mgr_work);
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}


// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss4BodySetActionWhole
/*!
  ボス４全体アクション設定
  
  @param body_work	[io]	本体ワーク
  @param act_id		[in]	全体アクションインデックス(GME_BOSS4_ACT_ID_XXX)
  
  @note
  本体と、本体を構成している各パーツのアクションをまとめて設定します。
 */
// =======================================================================
void gmBoss4BodySetActionWhole(GMS_BOSS4_BODY_WORK *body_work,
							   GME_BOSS4_ACT_ID act_id, BOOL force_change/*=FALSE*/)
{
	const GMS_BOSS4_PART_ACT_INFO	*pt_act_info	= GmBoss4GetActInfo(act_id, 0 );//gm_boss4_act_id_tbl[act_id];
	
	// 設定済みなら何もしない
	if (!force_change &&
		body_work->whole_act_id == act_id) {
		return;
	}
	
	// アクションID設定
	body_work->whole_act_id	= act_id;
	
	// 構成パーツのアクション設定を反映
	for (Sint32 i = 0; i < GME_BOSS4_PART_IDX_MAX; ++i) {
		
		if (body_work->parts_objs[i] == NULL) {
			continue;
		}

		// エッグマンについては、独立アクション中ならばアクション変更しない
		if (i == GME_BOSS4_PART_IDX_EGG ) {

			GMS_BOSS4_EGG_WORK	*egg_work	= (GMS_BOSS4_EGG_WORK*)body_work->parts_objs[i];
			
			// 戻るべきモーション番号を記録しておく
			body_work->egg_revert_mtn_id	= act_id/*pt_act_info[i].act_id*/;

			/*
			// ただし、ダメージの場合は通常モーションにしておく
			if ( body_work->egg_revert_mtn_id==IDB_BOSS04_EGG_MTN_B04_DMG01_01E_ZNM){
				// 通常モーション
				if (GmBoss4Is2ndStage()!=0){
					// 第２形態
					body_work->egg_revert_mtn_id=IDB_BOSS04_EGG_MTN_B04_2_ATT01_01E_ZNM;
				}else{
					body_work->egg_revert_mtn_id=IDB_BOSS04_EGG_MTN_B04_1_ATT01_01E_ZNM;
				}
			}
			*/
			Uint16	act_id = GmBoss4GetActInfo( body_work->egg_revert_mtn_id, GME_BOSS4_PART_IDX_EGG)->act_id;
			if ( act_id==IDB_BOSS04_EGG_MTN_B04_DMG01_01E_ZNM){
				// 通常モーション
				if (GmBoss4Is2ndStage()!=0){
					// 第２形態
					body_work->egg_revert_mtn_id=GME_BOSS4_ACT_ID_ATK_NML_MOVE2;
				}else{
					body_work->egg_revert_mtn_id=GME_BOSS4_ACT_ID_ATK_NML_MOVE;
				}
			}
		
			if (egg_work->flag & GMD_BOSS4_EGG_FLAG_INDP_ACT_SET) {
				continue;
			}
		}
		
		// 継続フラグがオフの時のみ、新たなアクションを設定
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
		// 手動マージチェック
		if (pt_act_info[i].is_blend) {
			if (pt_act_info[i].is_merge_manual) {
/*
				// 鎖のみサポート
				if (i == GME_BOSS4_PART_IDX_CAP) {
					GMS_BOSS4_CHAIN_WORK	*chain_work	= (GMS_BOSS4_CHAIN_WORK*)body_work->parts_objs[GME_BOSS4_PART_IDX_CAP];
					chain_work->flag |= GMD_BOSS4_CHAIN_FLAG_MANUAL_MOTION_MERGE;
					GMM_BS_OBJ(chain_work)->disp_flag	|= OBD_DISP_REPEAT;	// リピートを強制オン
				}
				else {
					MTM_ASSERT(!"gmBoss4.cpp::gmBoss4BodySetActionWhole() manual merge not supported\n");
				}
*/
			}
		}
		
		// モーション速度設定
		body_work->parts_objs[i]->obj_3d->speed[0]	= pt_act_info[i].mtn_spd;
		
		// ブレンド速度設定
		body_work->parts_objs[i]->obj_3d->blend_spd	= pt_act_info[i].blend_spd;
	}
}

#if 0
// =======================================================================
// gmBoss4BodySetSuspendAction
/*!
  再生停滞設定
  
  @param body_work		[io]	本体ワーク
  @param part_idx		[in]	パーツ番号
  @param suspend_time	[in]	停滞時間
  
  @note
  既定時間再生が停止されます。
  ブレンドは行われます（i.e. 遷移元モーションの再生は継続する）。
 */
// =======================================================================
void gmBoss4BodySetSuspendAction(GMS_BOSS4_BODY_WORK *body_work,
								 GME_BOSS4_PART_IDX part_idx,
								 Uint32 suspend_time)
{
	UNREFERENCED_PARAMETER(body_work);
	UNREFERENCED_PARAMETER(suspend_time);
	UNREFERENCED_PARAMETER(part_idx);
/*
	OBS_ACTION3D_NN_WORK	*obj_3d	= body_work->parts_objs[part_idx]->obj_3d;
	
	// 現状、鎖のみ対応
	// （エッグマンは独立アクションがあり、復帰が煩雑なので
	//   必要な場合は対応方法について要検討）
	MTM_ASSERT(part_idx == GME_BOSS4_PART_IDX_CAP);
	
	// 再生停止
	obj_3d->speed[0]	= 0;	// 復帰後の再生速度はアクションIDテーブルを参照して設定されます
	
	// 停滞有効
	body_work->mtn_suspend[part_idx].is_suspended	= TRUE;
	
	// 停滞時間設定
	body_work->mtn_suspend[part_idx].suspend_timer	= suspend_time;
*/
}
#endif

// =======================================================================
// gmBoss4BodyUpdateSuspendAction
/*!
  再生停滞更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  停滞タイマを更新し、タイマが満了したパーツについては停滞から復帰させます。
 */
// =======================================================================
void gmBoss4BodyUpdateSuspendAction(GMS_BOSS4_BODY_WORK *body_work)
{
	for (Sint32 i = 0; i < GME_BOSS4_PART_IDX_MAX; ++i) {
		GMS_BOSS4_MTN_SUSPEND_WORK	*suspend_work	= &body_work->mtn_suspend[i];
		
		if (suspend_work->is_suspended) {
/*			
			// 現状、鎖のみ対応
			// （エッグマンは独立アクションがあり、復帰が煩雑なので
			//   必要な場合は対応方法について要検討）
			MTM_ASSERT(i == GME_BOSS4_PART_IDX_CAP);
			
			// 既定時間停滞
			if (suspend_work->suspend_timer) {
				suspend_work->suspend_timer--;
			}
			else {
				
				// 再生速度復帰
				body_work->parts_objs[i]->obj_3d->speed[0]	=
					gm_boss4_act_id_tbl[body_work->whole_act_id][i].mtn_spd;
				
				// 停滞無効
				suspend_work->is_suspended	= FALSE;
			}
*/
		}
	}
}

#if 0
// =======================================================================
// gmBoss4BodyCheckChainMotionMergeEnd
/*!
  鎖パーツの手動モーションマージ終了チェック
  
  @param body_work	[in]	本体ワーク
  
  @retval TRUE	手動モーションマージ終了
  @retval FALSE	手動モーションマージ中
  
  @note
  鎖パーツが手動モーションマージ中がそうでないかを判定しています。
 */
// =======================================================================
BOOL gmBoss4BodyCheckChainMotionMergeEnd(GMS_BOSS4_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
	/*
	GMS_BOSS4_CHAIN_WORK	*chain_work	= (GMS_BOSS4_CHAIN_WORK*)body_work->parts_objs[GME_BOSS4_PART_IDX_CAP];
	
	if (chain_work->flag & GMD_BOSS4_CHAIN_FLAG_MANUAL_MOTION_MERGE) {
		return FALSE;
	}
	else {
		return TRUE;
	}
	*/
	return TRUE;
}
#endif
// =======================================================================
// gmBoss4BodyExecDamageRoutine
/*!
  ダメージ時処理
  
  @param body_work	[io]	本体ワーク
  
  @note
  ダメージ時の諸々の共通処理を行います。（ライフ減少、ゲージ更新、SE再生など）
 */
// =======================================================================
void gmBoss4BodyExecDamageRoutine(GMS_BOSS4_BODY_WORK *body_work)
{
	GMS_BOSS4_MGR_WORK	*mgr_work	= (GMS_BOSS4_MGR_WORK*)body_work->mgr_work;
	
	MTM_ASSERT(mgr_work);

	// TODO ダメージ中はダメージを受けない
	if (body_work->damage_timer>0)
		return;

	// ライフ減少
	if (mgr_work->life) {
		mgr_work->life	-= 1;
	}
	
	if (0 < mgr_work->life) {
		// ライフがある場合
		
		// ダメージ演出予約
		body_work->flag	|= GMD_BOSS4_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE;

		//ライフ1以下になると装飾動作開始
		if ( 1 >= mgr_work->life ){
			GmDecoStartLoop();
		}
	}
	else {
		// ライフが無い場合
		
		// ボス撃破タイミングのトロフィー獲得チェック
		HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING_DEFEAT_BOSS);
		
		// 死亡演出予約
		body_work->flag	|= GMD_BOSS4_BODY_FLAG_SIGNAL_B2B_DEFEAT;
	}

	// TODO 仮ダメージ時間
	body_work->damage_timer = 60;

	// カプセルにも当たらないようにする
	GmBoss4CapsuleSetInvincible( 30 );

	// TODO 第２形態のときのみ
	// ダメージアクションにする
	gmBoss4BodySetActionWhole(body_work, GME_BOSS4_ACT_ID_DAMAGE_NML, TRUE);


	
	// TODO : 未実装
}


// =======================================================================
// gmBoss4BodyIsExtraAttack
/*!
  追加攻撃の条件がそろったかチェック
 
  @param body_work	[io]	本体ワーク
  
  @retval	TRUE	条件満たした
  @retval	FALSE	条件満たしていない
 */
// =======================================================================
BOOL gmBoss4BodyIsExtraAttack(GMS_BOSS4_BODY_WORK *body_work)
{
	if (GMM_BOSS4_MGR(body_work)->life <= GMD_BOSS4_EXTRA_ATK_THRESHOLD_LIFE) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

#if 0
// =======================================================================
// gmBoss4BodyIsEscapeScrUnlock
/*!
  逃亡時のスクロール解除判定
  
  @retval TRUE	スクロール解除条件満たした
  @retval FALSE	スクロール解除条件満たしてない
 */
// =======================================================================
BOOL gmBoss4BodyIsEscapeScrUnlock(GMS_BOSS4_BODY_WORK *body_work)
{
	if (GMM_BS_OBJ(body_work)->pos.x >= GMM_BOSS4_AREA_RIGHT() + GMD_BOSS4_BODY_ESCAPE_SCR_UNLOCK_X_FROM_RIGHT) {
		return TRUE;
	}
	
	return FALSE;
}
#endif

#if 0
// =======================================================================
// gmBoss4BodyIsDirectionPositiveFromCurrent
/*!
  最短回転が正回転方向か判定
  
  @param body_work		[io]	本体ワーク
  @param target_angle	[in]	目標方向
  
  @retval TRUE	現在の角度→指定角度が正回転方向
  @retval FALSE 現在の角度→指定角度が負回転方向
  
  @note
  現在の角度から指定角度への最短回転が正回転方向か判定します。
  (e.g. 現在の角度が130degで指定角が90degの場合は最短回転は負方向。
        現在の角度が270degだった場合は最短回転は正方向。)
 */
// =======================================================================
BOOL gmBoss4BodyIsDirectionPositiveFromCurrent(GMS_BOSS4_BODY_WORK *body_work, Angle16 target_angle)
{
	Angle32	diff_angle;
	
	// ANGLE_MASKすることでAngle16の範囲に収まる。
	// また、32bit型なので必ず正数になる
	diff_angle	= MTD_MATH_ANGLE_MASK & ((Angle32)body_work->cur_angle - (Angle32)target_angle);
	
	if (diff_angle >= AKM_DEGtoA32(180)) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss4BodyUpdateDirection
/*!
  ボス４ 向き更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  ボスの向き情報をオブジェクトの角度に反映します。
  毎フレーム呼んでください。
 */
// =======================================================================
void gmBoss4BodyUpdateDirection(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	obj_work->dir.y	= (Uint16)body_work->cur_angle;
}


// =======================================================================
// gmBoss4BodySetDirectionNormal
/*!
  標準の角度（真横より正面寄り）に設定
  
  @param body_work	[io]	本体ワーク
  
  @note
  真横よりも少し正面寄りに向くように角度設定を行います。
  OBD_DISP_HFLIPを参照して対応する角度に設定しているため、
  適切にOBD_DISP_HFLIPを設定しておいてください。
 */
// =======================================================================
void gmBoss4BodySetDirectionNormal(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (obj_work->disp_flag	& OBD_DISP_HFLIP) {
		gmBoss4BodySetDirection(body_work, GMD_BOSS4_LEFTWARD_ANGLE);
	}
	else {
		gmBoss4BodySetDirection(body_work, GMD_BOSS4_RIGHTWARD_ANGLE);
	}
	
	// パラメータクリア
	body_work->orig_angle	= 0;
	body_work->turn_angle	= 0;
}

// =======================================================================
// gmBoss4BodySetDirection
/*!
  任意角度設定
  
  @param body_work	[io]	本体ワーク
  @param deg		[in]	角度
  
  @note
  指定の値に角度を設定します。オブジェクトへの反映は行われません。
 */
// =======================================================================
void gmBoss4BodySetDirection(GMS_BOSS4_BODY_WORK *body_work, Angle16 deg)
{
	body_work->cur_angle	= deg;
}

// =======================================================================
// gmBoss4BodyInitTurn
/*!
  振り向き回転処理を初期化
 
  @param body_work		[io]	本体ワーク
  @param turn_amount	[in]	振り向き回転量
  @param turn_spd		[in]	回転角速度
  
  @note
  turn_amountには現在の角度から差分でどれだけ回転させるか指定します。
  時計回りはマイナス値、反時計回りはプラス値を指定します。
  Angle32で扱える角度の範囲に注意してください。
 */
// =======================================================================
void gmBoss4BodyInitTurn(GMS_BOSS4_BODY_WORK *body_work,
						 Angle32 turn_amount, Angle32 turn_spd)
{
	MTM_ASSERT(0 == ((1 << 31) & (turn_amount ^ turn_spd))); // 符号比較
	
	body_work->orig_angle	= body_work->cur_angle;
	body_work->turn_angle	= 0;
	body_work->turn_amount	= turn_amount;
	body_work->turn_spd		= turn_spd;
	
	gmBoss4BodySetDirection(body_work,
							(Angle16)(body_work->orig_angle + body_work->turn_angle));
}

// =======================================================================
// gmBoss4BodyInitTurn
/*!
  振り向き回転処理を初期化（目標向き指定）
 
  @param body_work		[io]	本体ワーク
  @param dest_angle		[in]	目標となる角度（オフセットではなく、絶対的な角度）
  @param frame			[in]	回転にかけるフレーム数
  @param is_positive	[in]	正方向回転フラグ(TRUE : 正回転, FALSE : 負回転)
  
  @note
  目標の角度と所要フレーム数から、回転量と回転速度を算出して振り向き回転を初期化します。
  gmBoss4BodyUpdateTurn()時にspd_rateを指定するとフレーム数どおりに回転が完了しなくなります。
 */
// =======================================================================
void gmBoss4BodyInitTurn(GMS_BOSS4_BODY_WORK *body_work,
						 Angle16 dest_angle, Sint32 frame, BOOL is_positive)
{
	Uint16	turn_amount_u16;
	Angle32	turn_amount;
	Angle32	turn_spd;
	
	MTM_ASSERT(frame > 0);
	
	if (is_positive) {
		// 正方向（絶対値360度以内かつ適切な符号になるようにする）
		turn_amount_u16	= (Uint16)((Angle32)dest_angle - (Angle32)body_work->cur_angle);
		turn_amount	= (Angle32)turn_amount_u16;
	}
	else {
		// 負方向（絶対値360度以内かつ適切な符号になるようにする）
		turn_amount_u16	= (Uint16)(((Angle32)dest_angle - AKM_DEGtoA32(360)) -
								   ((Angle32)body_work->cur_angle - AKM_DEGtoA32(360)));
		turn_amount	= ((Angle32)((Uint32)turn_amount_u16) - AKM_DEGtoA32(360));	// キャストで符号情報が消えてるので戻す
	}
	
	// 回転速度取得
	turn_spd	= turn_amount / frame;
	
	// 回転初期化
	gmBoss4BodyInitTurn(body_work, turn_amount, turn_spd);
}


// =======================================================================
// gmBoss4BodyUpdateTurn
/*!
  振り向き回転処理更新
  
  @param body_work	[io]	本体ワーク
  @param spd_rate	[in]	速度係数（デフォルト1.f）
  
  @retval	TRUE	振り向き回転完了
  @retval	FALSE	振り向き回転中
  
  @note
  実際に回転を実施します。deg_addはgmBoss4BodyInitTurn()で指定した回転角と
  同じ符号になるようにしてください。
 */
// =======================================================================
BOOL gmBoss4BodyUpdateTurn(GMS_BOSS4_BODY_WORK *body_work, Float spd_rate/*=1.f*/)
{
	BOOL	result	= FALSE;
	Float	deg_spd;
	
	MTM_ASSERT(spd_rate >= 0.f);
	
	// 回転角度更新
	deg_spd	= spd_rate * body_work->turn_spd;
	MTM_ASSERT(MTM_MATH_ABS(deg_spd) <= (Sint32)0x7fffffff);
	body_work->turn_angle	+= (Angle32)deg_spd;
	
	// 目標到達判定
	if (body_work->turn_spd > 0) {
		if (body_work->turn_angle >= body_work->turn_amount) {
			result	= TRUE;
		}
	}
	else if (body_work->turn_spd < 0) {
		
		if (body_work->turn_angle <= body_work->turn_amount) {
			result	= TRUE;
		}
	}
	
	if (result) {
		// 目標角度にきっちりそろえる
		body_work->turn_angle	= body_work->turn_amount;
	}
	
	// 現在の向き設定
	gmBoss4BodySetDirection(body_work,
							(Angle16)((Angle32)body_work->orig_angle + body_work->turn_angle));
	
	return result;
}

// =======================================================================
// gmBoss4BodyInitTurnGently
/*!
  緩やか振り向き回転 初期化
 
  @param body_work		[io]	本体ワーク
  @param dest_angle		[in]	目標角度
  @param frame			[in]	回転にかけるフレーム数
  @param is_positive	[in]	正方向回転フラグ(TRUE : 正回転, FALSE : 負回転)
 */
// =======================================================================
void gmBoss4BodyInitTurnGently(GMS_BOSS4_BODY_WORK *body_work, Angle16 dest_angle,
							   Sint32 frame, BOOL is_positive)
{
	Uint16	turn_amount_u16;
	Float	frame_deg;
	MTM_ASSERT(frame > 0);
	
	body_work->orig_angle	= body_work->cur_angle;
	body_work->turn_angle	= 0;
	body_work->turn_spd		= 0;
	
	if (is_positive) {
		// 正方向（絶対値360度以内かつ適切な符号になるようにする）
		turn_amount_u16	= (Uint16)((Angle32)dest_angle - (Angle32)body_work->cur_angle);
		body_work->turn_amount	= (Angle32)turn_amount_u16;
	}
	else {
		// 負方向（絶対値360度以内かつ適切な符号になるようにする）
		turn_amount_u16	= (Uint16)(((Angle32)dest_angle - AKM_DEGtoA32(360)) -
								   ((Angle32)body_work->cur_angle - AKM_DEGtoA32(360)));
		body_work->turn_amount	= ((Angle32)((Uint32)turn_amount_u16) - AKM_DEGtoA32(360));	// キャストで符号情報が消えてるので戻す
	}
	
	// 速度カーブ角度初期化
	body_work->turn_gen_var		= 0;
	// 速度カーブ決定値初期化（コサイン半回転を0～1.0に対応させるので180degをフレーム数で割る）
	frame_deg	= 180.f / frame;
	MTM_ASSERT(MTM_MATH_ABS(frame_deg) <= (Sint32)0x7fffffff);
	body_work->turn_gen_factor	= AKM_DEGtoA32(frame_deg);
	
	gmBoss4BodySetDirection(body_work,
							(Angle16)((Angle32)body_work->orig_angle + body_work->turn_angle));
}

// =======================================================================
// gmBoss4BodyUpdateTurnGently
/*!
  緩やか振り向き回転 更新
 
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
BOOL gmBoss4BodyUpdateTurnGently(GMS_BOSS4_BODY_WORK *body_work)
{
	BOOL	result	= FALSE;
	Float	turn_angle_f;
	
	MTM_ASSERT(body_work->turn_gen_factor > 0);
	
	body_work->turn_gen_var	+= body_work->turn_gen_factor;
	if (body_work->turn_gen_var >= AKM_DEGtoA32(180)) {
		body_work->turn_gen_var	= AKM_DEGtoA32(180);
		result	= TRUE;
	}
	
	// コサインカーブで向きを決定
	turn_angle_f	= (body_work->turn_amount) * 0.5f * (1.f - nnCos(body_work->turn_gen_var));
	MTM_ASSERT(MTM_MATH_ABS(turn_angle_f) <= (Sint32)0x7fffffff);
	body_work->turn_angle	= (Angle32)(turn_angle_f);
	
	if (result) {
		// 目標角度にきっちりそろえる
		body_work->turn_angle	= body_work->turn_amount;
	}
	
	// 現在の向き設定
	gmBoss4BodySetDirection(body_work,
							(Angle16)((Angle32)body_work->orig_angle + body_work->turn_angle));
	
	return result;
}

#endif
// =======================================================================
// gmBoss4BodyInitPreANChainMotion
/*!
  通常攻撃「開始」 鎖モーション制御 初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  通常攻撃開始時の鎖の動きを手動で制御します。
 */
// =======================================================================
void gmBoss4BodyInitPreANChainMotion(GMS_BOSS4_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
/*
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	OBS_OBJECT_WORK	*obj_chain;
	
	MTM_ASSERT(obj_work);
	MTM_ASSERT(body_work->parts_objs[GME_BOSS4_PART_IDX_CAP]);
	
	obj_chain	= body_work->parts_objs[GME_BOSS4_PART_IDX_CAP];
	
	// 初期フレーム設定
	obj_chain->obj_3d->frame[0]	= GMD_BOSS4_BODY_PRE_ATKNML_CHAIN_INI_MTN_FRAME;
*/
}


// =======================================================================
// gmBoss4BodyInitPreANMove
/*!
  通常攻撃「開始」移動処理 初期化
  
  @note
  左方向の移動のみ。
 */
// =======================================================================
void gmBoss4BodyInitPreANMove(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	obj_work->spd.x	= 0;
	
	obj_work->spd_add.x	= -FX_F32_TO_FX32( GMD_BOSS4_BODY_PRE_ATKNML_SPD_ADD );
}

// =======================================================================
// gmBoss4BodyUpdatePreANMoveLeft
/*!
  通常攻撃「開始」移動処理 更新
  
  @retval TRUE	通常攻撃開始移動到達
  @retval FALSE	通常攻撃開始移動中
  
  @note
  左方向の移動のみ。
 */
// =======================================================================
BOOL gmBoss4BodyUpdatePreANMoveLeft(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	BOOL	result	= FALSE;
	
	// 速度制限
	if (MTM_MATH_ABS(obj_work->spd.x) >= FX_F32_TO_FX32( GMD_BOSS4_BODY_PRE_ATKNML_SPD_MAX_ABS ) ) {
		obj_work->spd.x	= -FX_F32_TO_FX32( GMD_BOSS4_BODY_PRE_ATKNML_SPD_MAX_ABS );
		obj_work->spd_add.x	= 0;
	}
	
	// 到達チェック
	if (obj_work->pos.x <= GMM_BOSS4_AREA_LEFT() + FX_F32_TO_FX32( GMD_BOSS4_BODY_ATKNML_LEFT_LIMIT ) ) {
		obj_work->pos.x	= GMM_BOSS4_AREA_LEFT()  + FX_F32_TO_FX32( GMD_BOSS4_BODY_ATKNML_LEFT_LIMIT );
		result	= TRUE;
	}
	
	// 停止
	if (result) {
		GmBsCmnSetObjSpdZero(obj_work);
	}
	
	return result;
}

// =======================================================================
// gmBoss4BodyUpdatePreANMoveRight
/*!
  通常攻撃「開始」移動処理 更新
  
  @retval TRUE	通常攻撃開始移動到達
  @retval FALSE	通常攻撃開始移動中
  
  @note
  右方向の移動のみ。
 */
// =======================================================================
BOOL gmBoss4BodyUpdatePreANMoveRight(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	BOOL	result	= FALSE;
	
	// 速度制限
	if (MTM_MATH_ABS(obj_work->spd.x) >= FX_F32_TO_FX32( GMD_BOSS4_BODY_PRE_ATKNML_SPD_MAX_ABS ) ) {
		obj_work->spd.x	= FX_F32_TO_FX32( GMD_BOSS4_BODY_PRE_ATKNML_SPD_MAX_ABS );
		obj_work->spd_add.x	= 0;
	}
	
	// 到達チェック
	if (obj_work->pos.x >= GMM_BOSS4_AREA_LEFT() + FX_F32_TO_FX32( GMD_BOSS4_BODY_ATKNML_RIGHT_LIMIT )) {
		obj_work->pos.x	= GMM_BOSS4_AREA_LEFT() + FX_F32_TO_FX32( GMD_BOSS4_BODY_ATKNML_RIGHT_LIMIT );
		result	= TRUE;
	}
	
	// 停止
	if (result) {
		GmBsCmnSetObjSpdZero(obj_work);
	}
	
	return result;
}

// =======================================================================
// gmBoss4BodySetANChainInitialBlendSpd
/*!
  通常攻撃 鎖 初回ブレンド速度設定
  
  @param body_work	[io]	本体ワーク
  
  @note
  通常攻撃初回時の鎖のモーションブレンド速度を設定します。
  アクション設定後に呼び出してください。
 */
// =======================================================================
void gmBoss4BodySetANChainInitialBlendSpd(GMS_BOSS4_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
}

// =======================================================================
// gmBoss4BodyInitAtkNmlMove
/*!
  通常攻撃移動処理 初期化
  
  @param body_work	[io]	本体ワーク
  @param frame		[in]	所要フレーム数
  
  @note
  OBD_DISP_HFLIP 設定に応じた(加)速度設定を行います。
 */
// =======================================================================
void gmBoss4BodyInitAtkNmlMove(GMS_BOSS4_BODY_WORK *body_work, Sint32 frame)
{
	// 所要フレーム設定
	body_work->move_time	= frame;
	
	// フレームカウント初期化
	body_work->move_cnt		= 0;
	
	// OBS_OBJECT_WORK::spd を使わないと、このフレームは速度反映されないので、
	// 代わりに今フレーム分の座標更新を一回手動実行しておく。
	gmBoss4BodyUpdateAtkNmlMove(body_work);
}

// =======================================================================
// gmBoss4BodyUpdateAtkNmlMove
/*!
  通常攻撃移動処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval	TRUE	通常攻撃移動到達
  @retval	FALSE	通常攻撃移動中
 */
// =======================================================================
BOOL gmBoss4BodyUpdateAtkNmlMove(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	fx32	start_x;
	fx32	end_x;
	BOOL	result;
	
	start_x	= GMM_BOSS4_AREA_LEFT() + FX_F32_TO_FX32( GMD_BOSS4_BODY_ATKNML_LEFT_LIMIT );
	end_x	= GMM_BOSS4_AREA_LEFT() + FX_F32_TO_FX32( GMD_BOSS4_BODY_ATKNML_RIGHT_LIMIT );
	
	// 左右反映
//	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
	if (body_work->dir.direction == GME_BOSS4_DIR_LEFT){
		fx32	temp;
		temp	= start_x;
		start_x	= end_x;
		end_x	= temp;
	}
	
	// 所要フレーム数厳守で終了判定を行う
	if (body_work->move_cnt < body_work->move_time) {
		
#if defined(GMD_BOSS4_AKTNML_MOVE_USE_PARTIAL_CURVE)
		/* 円の部分曲線バージョン*/
		Angle32	diff_angle;
		Float	factor;
		Float	amp;
		
		Angle32	angle_width	= GMD_BOSS4_BODY_ATKNML_MOVE_CURVE_ANGLE_WIDTH;
		Angle32	start_angle	= GMD_BOSS4_BODY_ATKNML_MOVE_CURVE_START_ANGLE;
		
		body_work->move_cnt++;
		
		// 1フレーム分の角度
		diff_angle	= (Angle32)(angle_width / (Float)body_work->move_time);
		
		// コサイン値の振り幅
		amp	= nnCos(start_angle) - nnCos(start_angle + angle_width);
		
		// 既定範囲の角度のコサイン値を0.0f ～ 1.0f にマップする
		factor	= ((nnCos(start_angle) - nnCos(start_angle + diff_angle * body_work->move_cnt)) / amp);
		
		// 座標設定
		obj_work->pos.x	= start_x + (fx32)((end_x - start_x) * factor);
#else
		/* 線形補間バージョン */
		fx32	diff_x;
		
		body_work->move_cnt++;
		
		// 1フレーム分の移動量
		diff_x	= (fx32)((end_x - start_x) / (Float)body_work->move_time);
		
		// 経過フレーム数に対応した座標を反映
		obj_work->pos.x	= start_x + (fx32)((Float)body_work->move_cnt * diff_x);
#endif	/* defined(GMD_BOSS4_AKTNML_MOVE_USE_PARTIAL_CURVE) */
		
		result	= FALSE;
	}
	else {
		// 所要フレーム経過後はきっちりそろえる
		obj_work->pos.x	= end_x;
		
		result	= TRUE;
	}
	
	return result;
}

// =======================================================================
// gmBoss4BodySetFlipForAtkNmlMove
/*!
  通常攻撃移動 フリップ設定
  
  @param body_work	[io]	本体ワーク
  
  @note
  現在地に応じたフリップ設定を行います。
  片道の移動が終わったタイミングで呼び出してください。
 */
// =======================================================================
void gmBoss4BodySetFlipForAtkNmlMove(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	fx32	center	= GMM_BOSS4_AREA_CENTER_X();
	if (obj_work->pos.x < center) {
		//obj_work->disp_flag	&= ~OBD_DISP_HFLIP;
		body_work->dir.direction = GME_BOSS4_DIR_RIGHT;
	}
	else {
		//obj_work->disp_flag	|= OBD_DISP_HFLIP;
		body_work->dir.direction = GME_BOSS4_DIR_LEFT;
	}
}

// =======================================================================
// gmBoss4BodyInitAtkNmlFlipAndTurn
/*!
  通常攻撃 フリップ+振り向き処理初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  フリップ設定を行った後に振り向き処理を初期化します。
  gmBoss4BodyUpdateAtkNmlFlipAndTurn()で更新してください。
  通常攻撃の移動が目標地点に到達したタイミングで呼びます。
 */
// =======================================================================
void gmBoss4BodyInitAtkNmlFlipAndTurn(GMS_BOSS4_BODY_WORK *body_work)
{
//	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	Sint32	frame	= GMD_BOSS4_BODY_ATKNML_TURN_FRAME;
	
	// ドリフトと同時にターンするときは、ドリフトより長くならないようにする
	// （通常攻撃の折り返しではドリフトの所要フレーム数を厳守する必要があるため）
	MTM_ASSERT(frame < GMD_BOSS4_BODY_ATKNML_DRIFT_FRAME);
	
	gmBoss4BodySetFlipForAtkNmlMove(body_work);
	
//	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
	if (body_work->dir.direction == GME_BOSS4_DIR_LEFT) {
		// これから左に向く
		GmBoss4UtilInitTurnGently(&body_work->dir, GMD_BOSS4_LEFTWARD_ANGLE,
								  frame, TRUE);
	}
	else {
		// これから右に向く
		GmBoss4UtilInitTurnGently(&body_work->dir, GMD_BOSS4_RIGHTWARD_ANGLE,
								  frame, FALSE);
	}
}

// =======================================================================
// gmBoss4BodyUpdateAtkNmlFlipAndTurn
/*!
  通常攻撃 フリップ+振り向き処理更新
  
  @param body_work	[io]	本体ワーク
  
  @retval	TRUE	振り向き完了
  @retval	FALSE	振り向き中
 */
// =======================================================================
BOOL gmBoss4BodyUpdateAtkNmlFlipAndTurn(GMS_BOSS4_BODY_WORK *body_work)
{
	return GmBoss4UtilUpdateTurnGently(&body_work->dir);
}

// =======================================================================
// gmBoss4BodyInitAtkNmlDrift
/*!
  通常攻撃 ドリフト移動処理 初期化
  
  @param body_work	[io]	本体ワーク
  @param frame		[in]	所要フレーム数
  
  @note
  OBD_DISP_HFLIP 設定に応じたドリフト移動を行います。
  ドリフト後の向きを設定した後に呼び出してください。
 */
// =======================================================================
void gmBoss4BodyInitAtkNmlDrift(GMS_BOSS4_BODY_WORK *body_work, Sint32 frame)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	MTM_ASSERT(frame > 0);
	
	// 速度クリア
	GmBsCmnSetObjSpdZero(obj_work);
	
	// 角度初期化
	body_work->drift_angle	= 0;
	
	// 角速度初期化
	body_work->drift_ang_spd	= (Angle32)nnRoundOff((AKM_DEGtoA32(180.f) / (Float)frame) + 0.5f);
	
	// タイマ初期化
	body_work->drift_timer	= frame;
	
	// 軸座標設定
//	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
	if (body_work->dir.direction == GME_BOSS4_DIR_LEFT){
		// これから左に向く（右にドリフト）
		body_work->drift_pivot_x	= GMM_BOSS4_AREA_LEFT() + FX_F32_TO_FX32( GMD_BOSS4_BODY_ATKNML_RIGHT_LIMIT );
	}
	else {
		// これから右に向く（左にドリフト）
		body_work->drift_pivot_x	= GMM_BOSS4_AREA_LEFT() + FX_F32_TO_FX32( GMD_BOSS4_BODY_ATKNML_LEFT_LIMIT );
	}
	
	// OBS_OBJECT_WORK::spd を使わないと、このフレームは速度反映されないので、
	// 代わりに今フレーム分の座標更新を一回手動実行しておく。
	gmBoss4BodyUpdateAtkNmlDrift(body_work);
}

// =======================================================================
// gmBoss4BodyUpdateAtkNmlDrift
/*!
  通常攻撃 ドリフト移動処理 更新
  
  @param param0 [in] 入力引数0説明
  
  @retval TRUE	ドリフト移動中
  @retval FALSE	ドリフト移動終了
 */
// =======================================================================
BOOL gmBoss4BodyUpdateAtkNmlDrift(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	fx32	offset_x;
	BOOL	result;
	
	if (body_work->drift_timer) {
		body_work->drift_timer--;
		
		// 角度更新
		body_work->drift_angle	= MTD_MATH_ANGLE_MASK & (body_work->drift_angle +
														 body_work->drift_ang_spd);
		
		
		offset_x	= FX_Mul(FX_Sin(body_work->drift_angle), FX_F32_TO_FX32( GMD_BOSS4_BODY_ATKNML_DRIFT_AMP ) );
		
		result	= FALSE;
	}
	else {
		offset_x	= 0;
		
		result	= TRUE;
	}
	
	// 左右ドリフト方向反映
//	if (!(obj_work->disp_flag & OBD_DISP_HFLIP)) {
	if (body_work->dir.direction == GME_BOSS4_DIR_RIGHT){
		offset_x	= -offset_x;
	}
	
	// 座標反映
	obj_work->pos.x	= body_work->drift_pivot_x + offset_x;
	
	return result;
}

#if 0
// =======================================================================
// gmBoss4BodyInitRush
/*!
  叩きつけ突進処理 初期化
  
  @param body_work	[io]	本体ワーク
  @param is_left	[in]	左向きフラグ
 */
// =======================================================================
void gmBoss4BodyInitRush(GMS_BOSS4_BODY_WORK *body_work, BOOL is_left)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	fx32	spd_x;
	fx32	spd_y;
	
	if (is_left) {
		body_work->bash_targ_pos.x	= GMM_BOSS4_AREA_LEFT() + GMD_BOSS4_BODY_ATKBASH_TARG_X_LEFT;
		body_work->bash_targ_pos.y	= GMD_BOSS4_BODY_ATKBASH_TARG_Y;
		body_work->bash_targ_pos.z	= GMD_BOSS4_DEFAULT_POS_Z;
	}
	else {
		body_work->bash_targ_pos.x	= GMM_BOSS4_AREA_LEFT() + GMD_BOSS4_BODY_ATKBASH_TARG_X_RIGHT;
		body_work->bash_targ_pos.y	= GMD_BOSS4_BODY_ATKBASH_TARG_Y;
		body_work->bash_targ_pos.z	= GMD_BOSS4_DEFAULT_POS_Z;
	}
	
	spd_x	= (body_work->bash_targ_pos.x - obj_work->pos.x) / GMD_BOSS4_BODY_ATKBASH_MTN_FRAME;
	spd_y	= (body_work->bash_targ_pos.y - obj_work->pos.y) / GMD_BOSS4_BODY_ATKBASH_MTN_FRAME;
	
	obj_work->spd_add.x	= (fx32)(spd_x * GMD_BOSS4_BODY_ATKBASH_SPD_ADD_FACTOR);
	obj_work->spd_add.y	= (fx32)(spd_y * GMD_BOSS4_BODY_ATKBASH_SPD_ADD_FACTOR);
	obj_work->spd.x	= 0;
	obj_work->spd.y	= 0;
}

// =======================================================================
// gmBoss4BodyUpdateRush
/*!
  叩きつけ突進処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval	TRUE	叩きつけ突進完了
  @retval	FALSE	叩きつけ突進中
 */
// =======================================================================
BOOL gmBoss4BodyUpdateRush(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	NNS_VECTOR	vec_diff;
	NNS_VECTOR	vec_spd;
	
	amVectorSet(&vec_diff,
				FX_FX32_TO_F32(body_work->bash_targ_pos.x) - FX_FX32_TO_F32(obj_work->pos.x),
				FX_FX32_TO_F32(body_work->bash_targ_pos.y) - FX_FX32_TO_F32(obj_work->pos.y),
				0);
	
	amVectorSet(&vec_spd,
				FX_FX32_TO_F32(obj_work->spd.x),
				FX_FX32_TO_F32(obj_work->spd.y),
				0);
	
	// 通り過ぎたかチェック
	if (0 >= nnDotProductVector(&vec_spd, &vec_diff)) {
		// 目標位置に固定する
		GmBsCmnSetObjSpdZero(obj_work);
		VEC_Set(&obj_work->pos,
				body_work->bash_targ_pos.x,
				body_work->bash_targ_pos.y,
				body_work->bash_targ_pos.z);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss4BodyInitBashReturn
/*!
  叩きつけ 元の高度に戻る 初期化
  
  @param body_work	[io]	本体ワーク
  @param is_left	[in]	左側処理フラグ
  							(TRUE:左側の既定位置に戻る
 	 						 FALSE:右側の既定位置に戻る)
  
  @note
  現在の向きに応じて適切な位置に戻る処理の初期化を行います。
 */
// =======================================================================
void gmBoss4BodyInitBashReturn(GMS_BOSS4_BODY_WORK *body_work, BOOL is_left)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (is_left) {
		body_work->bash_ret_pos.x	= GMM_BOSS4_AREA_LEFT() + GMD_BOSS4_BODY_ATKBASH_HOMEPOS_X_LEFT;
		body_work->bash_ret_pos.y	= body_work->atk_nml_alt;
		body_work->bash_ret_pos.z	= GMD_BOSS4_DEFAULT_POS_Z;
	}
	else {
		body_work->bash_ret_pos.x	= GMM_BOSS4_AREA_LEFT() + GMD_BOSS4_BODY_ATKBASH_HOMEPOS_X_RIGHT;
		body_work->bash_ret_pos.y	= body_work->atk_nml_alt;
		body_work->bash_ret_pos.z	= GMD_BOSS4_DEFAULT_POS_Z;
	}
	
	VEC_Set(&body_work->bash_orig_pos,
			obj_work->pos.x,
			obj_work->pos.y,
			obj_work->pos.z);
	
	// 角度初期化
	body_work->bash_homing_deg	= 0;
}

// =======================================================================
// gmBoss4BodyUpdateBashReturn
/*!
  叩きつけ 元の高度に戻る 更新
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
BOOL gmBoss4BodyUpdateBashReturn(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	body_work->bash_homing_deg	+= AKM_DEGtoA32(180 / 60);	// TODO : 仮
	if (body_work->bash_homing_deg >= AKM_DEGtoA32(180)) {// TODO : 仮
		body_work->bash_homing_deg	= AKM_DEGtoA32(180);// TODO : 仮
		
		// 目標位置に固定
		obj_work->pos.x	= body_work->bash_ret_pos.x;
		obj_work->pos.y	= body_work->bash_ret_pos.y;
		
		return TRUE;
	}
	else {
		obj_work->pos.x	=
			body_work->bash_orig_pos.x +
				FX_Mul(body_work->bash_ret_pos.x - body_work->bash_orig_pos.x,
					   (FX32_ONE - mtMathCos(body_work->bash_homing_deg)) >> 1);
		obj_work->pos.y	=
			body_work->bash_orig_pos.y +
				FX_Mul(body_work->bash_ret_pos.y - body_work->bash_orig_pos.y,
					   (FX32_ONE - mtMathCos(body_work->bash_homing_deg)) >> 1);
		return FALSE;
	}
}
#endif

// =======================================================================
// gmBoss4BodyInitEscapeMove
/*!
  逃亡移動処理 初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss4BodyInitEscapeMove(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	obj_work->spd.x	= 0;
	
//	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
	if (body_work->dir.direction == GME_BOSS4_DIR_LEFT){
		
		obj_work->spd_add.x	= -GMD_BOSS4_BODY_ESCAPE_SPD_X_ADD;
	}
	else {
		obj_work->spd_add.x	= GMD_BOSS4_BODY_ESCAPE_SPD_X_ADD;
	}
	
	obj_work->spd_add.y	= -GMD_BOSS4_BODY_ESCAPE_SPD_Y_ADD;
}

// =======================================================================
// gmBoss4BodyUpdateEscapeMove
/*!
  逃亡移動処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	スクロールロック位置
  @retval FALSE	移動中
 */
// =======================================================================
BOOL gmBoss4BodyUpdateEscapeMove(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	BOOL	result	= FALSE;
	
	// 速度制限
	if (MTM_MATH_ABS(obj_work->spd.x) >= GMD_BOSS4_BODY_ESCAPE_SPD_X_MAX) {
		obj_work->spd.x	= GMD_BOSS4_BODY_ESCAPE_SPD_X_MAX;
		obj_work->spd.y	= GMD_BOSS4_BODY_ESCAPE_SPD_Y_MAX;
		obj_work->spd_add.x	= 0;
		obj_work->spd_add.y	= 0;
	}
	
//	// マップ上端から上の方に完全に消えたかチェック
//	if (obj_work->pos.y < 0 - GMD_BOSS4_BODY_HIDE_RADIUS) {
//		result	= TRUE;
//	}

	// マップ右から完全に消えたかチェック
	Float right = (Float)g_gm_main_system.map_fcol.right;

	if (obj_work->pos.x > FX_F32_TO_FX32(right+100.0f) ) {
		result	= TRUE;
	}

	// 停止
	if (result) {
		GmBsCmnSetObjSpdZero(obj_work);
	}
	
	return result;
}

// =======================================================================
// gmBoss4BodyInitDefeatState
/*!
  撃破ステート初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  GMD_BOSS4_BODY_FLAG_CHAIN_DEPENDフラグ設定を持ち越しつつ
  DEFEATステートへの遷移を行います。
  DEFEATステートに遷移する場合は、
  gmBoss4BodyChangeState()を直接呼ばずにこの関数でステート遷移してください。
 */
// =======================================================================
void gmBoss4BodyInitDefeatState(GMS_BOSS4_BODY_WORK *body_work)
{
	BOOL	is_depend	= FALSE;
	
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_CHAIN_DEPEND) {
		is_depend	= TRUE;
	}
	
	// ステート遷移
	gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_DEFEAT, TRUE);
	
	if (is_depend) {
		body_work->flag	|= GMD_BOSS4_BODY_FLAG_CHAIN_DEPEND;
	}
	else {
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_CHAIN_DEPEND;
	}

	// ボス戦勝利BGMへ変更
	GmSoundChangeWinBossBGM();
}


// ============================================================================
// ノード操作関連
// ============================================================================
// =======================================================================
// gmBoss4BodyUpdateChainTopDirection
/*!
  鎖の付け根パーツを正面に向ける
  
  @param body_work	[io]	本体ワーク
  
  @note
  鎖の付け根パーツ（本体側のパーツ）を正面に向くようにCNMの設定を行います。
 */
// =======================================================================
void gmBoss4BodyUpdateChainTopDirection(GMS_BOSS4_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
/*
	NNS_MATRIX	*w_mtx;
	NNS_MATRIX	rotated_mtx;
	
	if (!(body_work->flag & GMD_BOSS4_BODY_FLAG_CHAIN_DEPEND)) {
		// ノードのワールドマトリクス取得
		w_mtx	= GmBsCmnGetSNMMtx(&body_work->node_work.snm_work,
								   body_work->node_work.work[ chaintop_snm_reg_id);
		
		nnRotateYMatrix(&rotated_mtx, w_mtx,
						-GMM_BS_OBJ(body_work)->dir.y + AKM_DEGtoA16(90));
		
		GmBsCmnSetCNMMtx(&body_work->cnm_mgr_work,
						 &rotated_mtx,
						 body_work->chaintop_cnm_reg_id);
		GmBsCmnEnableCNMMtxNode(&body_work->cnm_mgr_work,
								body_work->chaintop_cnm_reg_id,
								TRUE);
	}
	else {
		GmBsCmnEnableCNMMtxNode(&body_work->cnm_mgr_work,
								body_work->chaintop_cnm_reg_id,
								FALSE);
	}
*/
}


// ============================================================================
// 処理関数
// ============================================================================

// =======================================================================
// gmBoss1ChainAtkHitFunc
/*!
  鎖 攻撃ヒット時処理関数
  
  @param my_rect	[io]	自分の矩形
  @parma your_rect	[io]	相手の矩形
 */
// =======================================================================
void gmBoss4BodyAtkHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{
	GMS_BOSS4_BODY_WORK	*body_work	= (GMS_BOSS4_BODY_WORK*)my_rect->parent_obj;
	
	// ヒットしたことを本体に知らせる
	body_work->flag	|= GMD_BOSS4_BODY_FLAG_SIGNAL_C2E_ATK_HIT;
	
	// エネミー標準攻撃ヒット処理
	GmEnemyDefaultAtkFunc(my_rect, your_rect);
}

// =======================================================================
// gmBoss4BodyDamageDefFunc
/*!
  本体 プレイヤー攻撃ヒット時くらい処理関数
  
  @param my_rect	[io]	自分の矩形
  @parma your_rect	[io]	相手の矩形
 */
// =======================================================================
void gmBoss4BodyDamageDefFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{
	OBS_OBJECT_WORK	*my_obj		= my_rect->parent_obj;
	OBS_OBJECT_WORK	*your_obj	= your_rect->parent_obj;
	GMS_BOSS4_BODY_WORK	*body_work	= (GMS_BOSS4_BODY_WORK*)my_obj;
	//GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)my_obj;
	
	if (your_obj && GMD_OBJTYPE_PLAYER == your_obj->obj_type) {

		GmBoss4UtilSetPlayerAttackReaction( your_obj, my_obj );
		/*
#if defined(GMD_BOSS4_TEMPORARY)
		GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)my_obj;
		body_work->no_hit_timer	= 10;//仮
		ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag	|= OBD_RECT_NOHIT;//仮
#endif // defined(GMD_BOSS4_TEMPORARY)//
*/		
		if (body_work->nohit_work.timer == 0){
		//if (!(body_work->flag & GMD_BOSS4_BODY_FLAG_INVINCIBLE)) {
			// 無敵ではないときだけダメージ処理
			
			// ダメージSE再生
			GmSoundPlaySE("Boss0_01");
			
			// ダメージエフェクト生成
			gmBoss4EffDamageInit(body_work);

			// ダメージ処理
			gmBoss4BodyExecDamageRoutine(body_work);
			// 第２形態
			if (GmBoss4Is2ndStage()){
				// 今いるちびエッグマンを消す
				GmBoss4ChibiExplosion();
				// すぐにカプセルを投げないようにする
				body_work->wait_timer = 60;
				body_work->proc_update	= gmBoss4BodyStateUpdate2nd;

			}
			// コントローラー振動
			GMM_PAD_VIB_SMALL_TIME(30);

		}

		GmBoss4UtilInitNoHitTimer( &body_work->nohit_work, (GMS_ENEMY_COM_WORK*)my_obj, 10 );
	}
}


// =======================================================================
// gmBoss4BodyDefHitFunc
/*!
  本体 プレイヤーとの体当たり  
  
  @param my_rect	[io]	自分の矩形
  @parma your_rect	[io]	相手の矩形
  
  @note
// ボディに当たりをつけてソニックを追いやる
 */
// =======================================================================
void gmBoss4BodyDefHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{
	OBS_OBJECT_WORK	*my_obj		= my_rect->parent_obj;
	OBS_OBJECT_WORK	*your_obj	= your_rect->parent_obj;
/*
	// 自分が右のときは
	if (my_obj->pos.x > your_obj->pos.x){
		my_obj->pos.x += FX_F32_TO_FX32( GMD_BOSS4_CHIBI_HIT_CHIBI_MOVE);
		my_obj->spd.x += FX_F32_TO_FX32( GMD_BOSS4_CHIBI_HIT_CHIBI_ADDSPD);
	}
	if (my_obj->pos.x < your_obj->pos.x){
		my_obj->pos.x -= FX_F32_TO_FX32( GMD_BOSS4_CHIBI_HIT_CHIBI_MOVE);
		my_obj->spd.x -= FX_F32_TO_FX32( GMD_BOSS4_CHIBI_HIT_CHIBI_ADDSPD);
	}
*/
	your_obj->pos.x = your_obj->pos.x - (fx32)((your_obj->move.x));

	// 自分が右のときは
	if (my_obj->pos.x > your_obj->pos.x){
		your_obj->pos.x -= FX_F32_TO_FX32( 2.0f );
		your_obj->spd.x = -MTM_MATH_ABS(your_obj->spd.x);
		your_obj->spd_m = -MTM_MATH_ABS(your_obj->spd_m);
	}
	if (my_obj->pos.x < your_obj->pos.x){
		your_obj->pos.x += FX_F32_TO_FX32( 2.0f );
		your_obj->spd.x = MTM_MATH_ABS(your_obj->spd.x);
		your_obj->spd_m = MTM_MATH_ABS(your_obj->spd_m);
	}

}

// =======================================================================
// gmBoss4BodyOutFunc
/*!
  本体 専用描画関数
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  通常の描画に加えてノード操作処理も行っています。
 */
// =======================================================================
void gmBoss4BodyOutFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_BODY_WORK	*body_work	= (GMS_BOSS4_BODY_WORK*)obj_work;
	
	// ノード操作更新
	GmBsCmnUpdateCNMParam(obj_work, &body_work->cnm_mgr_work);
	
	// 標準描画関数
	ObjDrawActionSummary(obj_work);
}

// ============================================================================
// 制御処理
// ============================================================================

// =======================================================================
// gmBoss4BodyChangeState
/*!
  本体ステート変更
 
  @param body_work	[io]	本体ワーク
  @param state		[in]	遷移先ステート
  
  @note
  前ステートの終了関数呼び出しと、遷移先ステートの開始関数呼び出しを行います。
 */
// =======================================================================
void gmBoss4BodyChangeState(GMS_BOSS4_BODY_WORK *body_work,
							GME_BOSS4_BODY_STATE state, BOOL is_wrapped/*=FALSE*/)
{
//#ifndef	MTD_DEBUG
	UNREFERENCED_PARAMETER(is_wrapped);
//#endif	//MTD_DEBUG

	const GMS_BOSS4_BODY_STATE_ENTER_INFO	*enter_info;
	GMF_BOSS4_BODY_STATE_LEAVE_FUNC	leave_func;
	
	// 前ステート終了処理
	leave_func	= gm_boss4_body_state_leave_func_tbl[body_work->state];
	if (leave_func) {
		leave_func(body_work);
	}
	
	// ステート設定
	body_work->prev_state	= body_work->state;
	body_work->state		= state;
	
	// 次ステート開始処理
	enter_info	= &gm_boss4_body_state_enter_info_tbl[body_work->state];
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
// gmBoss4BodyWaitLoad
/*!
  本体 ロード完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss4BodyWaitLoad(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_BODY_WORK	*body_work	= (GMS_BOSS4_BODY_WORK*)obj_work;
	
	
	// 全ての構成パーツのロードを待つ
//	if (body_work->mgr_work->flag & GMD_BOSS4_MGR_FLAG_LOAD_END) {
	if (GmBoss4IsBuilded()){
		
		// ノードマトリクス取得スタート
		GmBoss4UtilInitNodeMatrix( &body_work->node_work, obj_work, GMD_BOSS4_BODY_NODE_SNM_NUM );

		/*
		// BMCBシステム初期化
		GmBsCmnInitBossMotionCBSystem(obj_work,
									  &body_work->bmcb_mgr);
		
		// ノードマトリクス取得初期化
		GmBsCmnCreateSNMWork(&body_work->snm_work,
							 obj_work->obj_3d->object,
							 GMD_BOSS4_BODY_NODE_SNM_NUM);
		// モーションコールバックを実行リストに追加
		GmBsCmnAppendBossMotionCallback(&body_work->bmcb_mgr,
										&body_work->snm_work.bmcb_link);
		
		*/
		/*
		// ノードマトリクス取得ノード追加
		body_work->chain_snm_reg_id	=
			GmBsCmnRegisterSNMNode(&body_work->snm_work, GMD_BOSS4_BODY_NODE_IDX_CHAIN_CONNECT);
		body_work->egg_snm_reg_id	=
			GmBsCmnRegisterSNMNode(&body_work->snm_work, GMD_BOSS4_BODY_NODE_IDX_EGG_CONNECT);
		body_work->body_snm_reg_id	=
			GmBsCmnRegisterSNMNode(&body_work->snm_work, GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE);
		body_work->chaintop_snm_reg_id	=
			GmBsCmnRegisterSNMNode(&body_work->snm_work, GMD_BOSS4_BODY_NODE_IDX_CHAIN_ROOT_PART);
		*/
		// 初期取得(初期はマトリクスが取れないためここで一度取得しておく)
		GmBoss4UtilGetNodeMatrix( &body_work->node_work, GMD_BOSS4_BODY_NODE_IDX_EGG_CONNECT );
		GmBoss4UtilGetNodeMatrix( &body_work->node_work, GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE );
		GmBoss4UtilGetNodeMatrix( &body_work->node_work, GMD_BOSS4_BODY_NODE_IDX_BODY_LIGHT_L );
		GmBoss4UtilGetNodeMatrix( &body_work->node_work, GMD_BOSS4_BODY_NODE_IDX_BODY_LIGHT_R );
		GmBoss4UtilGetNodeMatrix( &body_work->node_work, GMD_BOSS4_BODY_NODE_IDX_BODY_BOOSTER_L );
		GmBoss4UtilGetNodeMatrix( &body_work->node_work, GMD_BOSS4_BODY_NODE_IDX_BODY_BOOSTER_R );
		

		// ノードマトリクス操作処理管理ワーク初期化
		GmBsCmnCreateCNMMgrWork(&body_work->cnm_mgr_work,
								obj_work->obj_3d->object,
								GMD_BOSS4_BODY_NODE_CNM_NUM);
		
		// ノードマトリクス操作コールバック初期化
		GmBsCmnInitCNMCb(obj_work, &body_work->cnm_mgr_work);
		
		// 操作ノード追加
		body_work->chaintop_cnm_reg_id	=
			GmBsCmnRegisterCNMNode(&body_work->cnm_mgr_work, 0);
		
		
		obj_work->ppFunc	= gmBoss4BodyMain;

		// ダメージ中ではなし
		body_work->damage_timer = 0;

		GmBoss4UtilInitNoHitTimer( &body_work->nohit_work, (GMS_ENEMY_COM_WORK*)body_work, 0 );

		// 開始ステート設定

		if (GmBoss4CheckBossRush()){
			// ボスラッシュ
			gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_2ND );
		}else{
			// 通常ZONE4
			gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_START);
		}
	}
}

// =======================================================================
// gmBoss4BodyMain
/*!
  本体 メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss4BodyMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_BODY_WORK	*body_work	= (GMS_BOSS4_BODY_WORK*)obj_work;

	GmBoss4UtilUpdateNoHitTimer( &body_work->nohit_work );
/*
#if defined(GMD_BOSS4_TEMPORARY)
	//仮
	if (body_work->no_hit_timer) {
		body_work->no_hit_timer--;

		GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)obj_work;
		ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag	|= OBD_RECT_NOHIT;
		ene_com->rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_RECT_NOHIT;
	}
	else {
		GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)obj_work;
		ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag	&= ~OBD_RECT_NOHIT;
		ene_com->rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_NOHIT;
	}
#endif // defined(GMD_BOSS4_TEMPORARY)
*/	
	// 更新処理
	if (body_work->proc_update) {
		body_work->proc_update(body_work);
	}
	
	// モーション再生停滞更新
	gmBoss4BodyUpdateSuspendAction(body_work);
	
	// アフターバーナー生成ループ更新
	gmBoss4EffAfterburnerUpdateCreate(body_work);
#if !_IPHONE
	// ライト生成ループ更新
	gmBoss4EffBossLightUpdateCreate(body_work);
#endif // !_IPHONE
	// 死亡演出開始チェック
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_SIGNAL_B2B_DEFEAT) {
		
		
		body_work->flag	&= ~(GMD_BOSS4_BODY_FLAG_SIGNAL_B2B_DEFEAT |
							 GMD_BOSS4_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE);
							 
		// ↑ダメージシグナルは無視させる
		
		// 撃破ステートへ
		gmBoss4BodyInitDefeatState(body_work);
		return;
	}
	
	// 仮実装
	if (body_work->damage_timer > 0){
		body_work->damage_timer--;
	}
	
	// ダメージ演出開始チェック
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE) {
		body_work->flag	&= ~(GMD_BOSS4_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE);
		
		// エッグマンにダメージを通知
		body_work->flag	|= GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_DAMAGE;
		
		// TODO : 未実装 もしくはgmBoss4BodyExecDamageRoutine() で直接シーケンス変更させる？
		
		// ダメージ点滅初期化
		GmBsCmnInitObject3DNNDamageFlicker(obj_work, &body_work->flk_work,
										   GMD_BOSS4_BODY_DMG_FLICKER_RADIUS);
	}
	
	// ダメージ点滅更新
	GmBsCmnUpdateObject3DNNDamageFlicker(obj_work, &body_work->flk_work);
	
	// 角度反映
	GmBoss4UtilUpdateDirection(&body_work->dir, obj_work);
	
	// 鎖付け根パーツの向きを更新
	gmBoss4BodyUpdateChainTopDirection(body_work);
	

#if 0 // TODO : 仮  削除する
	if (AoPadAnalogRX() > 0x1000) {
		obj_work->pos.x	+= AoPadAnalogRX();
	}
	else if (AoPadAnalogRX() < -0x1000) {
		obj_work->pos.x	+= AoPadAnalogRX();
	}
	
	if (AoPadAnalogRY() < -0x1000) {
		obj_work->pos.y	-= AoPadAnalogRY();
	}
	else if (AoPadAnalogRY() > 0x1000) {
		obj_work->pos.y	-= AoPadAnalogRY();
	}
#endif

	// ソニックがジャンプしているときはボディ当たりをなくす
	GMS_ENEMY_3D_WORK*	ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	GMS_PLAYER_WORK* ply_work = (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj();
	if (ply_work->seq_state == GME_PLY_SEQ_STATE_JUMP ||
		ply_work->seq_state == GME_PLY_SEQ_STATE_HOMING ) 
	{
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;
	}else{
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag |= OBD_RECT_ENABLE;
	}
}


// ============================================================================
// 各状態のシーケンス処理
// ============================================================================
// =======================================================================
// gmBoss4BodyState{Enter|Leave|Update}StartXXX
/*!
  本体 開始ステート遷移時処理関数
 */
// =======================================================================
// 開始ステート開始関数
void gmBoss4BodyStateEnterStart(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 矩形当たりオフ
	obj_work->flag	|= OBD_OBJECT_NOHIT;
	
	// 鎖当たりオフ
	body_work->flag	|= GMD_BOSS4_BODY_FLAG_CHAIN_NOHIT;
	
	// アクション設定（強制設定）
	gmBoss4BodySetActionWhole(body_work, GME_BOSS4_ACT_ID_APP_FALL, TRUE);
	
	// 鎖非表示
	body_work->flag	|= GMD_BOSS4_BODY_FLAG_CHAIN_NODISP;
	
	// 速度設定
	GmBsCmnSetObjSpdZero(obj_work);
	
	// 通常角度に設定
	GmBoss4UtilSetDirectionNormal(&body_work->dir);
	
	// 待機時間設定
	body_work->wait_timer	= 120;//仮
	
	// 処理関数設定
	body_work->proc_update	= gmBoss4BodyStateUpdateStartWithWait;

	// ボスライト
	gmBoss4EffBossLightSetEnable( body_work, TRUE );
}

// 開始ステート終了関数
void gmBoss4BodyStateLeaveStart(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// アフターバーナー無効化
	gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE);
	
	// 鎖当たりオン
	body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_CHAIN_NOHIT;
	
	// 矩形当たりオン
	obj_work->flag	&= ~OBD_OBJECT_NOHIT;
	
	// フラグ元にもどす
	body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_CHAIN_NODISP;
}

// 開始ステート更新 待機処理
void gmBoss4BodyStateUpdateStartWithWait(GMS_BOSS4_BODY_WORK *body_work)
{
//	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);

	if (gmBoss4IsScrollLockBusy()) {
		body_work->proc_update	= gmBoss4BodyStateUpdateStartWithWaitEnd;
	}
}

void gmBoss4BodyStateUpdateStartWithWaitEnd(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);

	if (!gmBoss4IsScrollLockBusy()) {
		// 速度設定
		GmBsCmnSetObjSpd(obj_work,
						 0,
						 FX32_ONE * 1,// TODO : 仮
						 0);
		
		// 処理関数設定
		body_work->proc_update	= gmBoss4BodyStateUpdateStartWithFall;

		// カプセルを無敵
		GmBoss4CapsuleSetInvincible(60*10, FALSE);
		// ちびエッグマン無敵解除
		GmBoss4ChibiSetInvincible(FALSE);

		body_work->wait_timer2 = 90; //TODO PAL
		// エッグマンを笑わせる
		body_work->flag |= GMD_BOSS4_BODY_FLAG_SIGNAL_C2E_ATK_HIT;
		body_work->wait_timer = 60*2; //TODO PAL

		GmBoss4UtilLookAtPlayer(&body_work->dir, obj_work, 1);
		GmBoss4UtilLookAt( &body_work->dir );

		/* システムから呼び出しを行うため必要なし
		// ボスサウンド　１段階目(テスト)
		GmSoundPlayBGM("snd_sng_boss", 60);
		 */
#if _IPHONE
		GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_ZONE4_BOSS);
#endif // _IPHONE
	}
}

// 開始ステート更新 降下処理
void gmBoss4BodyStateUpdateStartWithFall(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);


	GMS_ENEMY_3D_WORK*	ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	ene_3d->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;

	if (body_work->wait_timer2>0){
		body_work->wait_timer2--;
		GmBoss4UtilLookAtPlayer(&body_work->dir, obj_work, 1);
	}

	// 笑わせ続ける(仮)
//	OBS_OBJECT_WORK* egg_obj = (OBS_OBJECT_WORK*)(body_work->parts_objs[ GME_BOSS4_PART_IDX_EGG ]);
//	if (GmBsCmnIsActionEnd(egg_obj)) {
	if (--body_work->wait_timer<=0 && obj_work->pos.y <= FX_F32_TO_FX32(235.0f) ){
		// エッグマンを笑わせる ある程度の高さまでしか笑わないようにしておく
		body_work->flag |= GMD_BOSS4_BODY_FLAG_SIGNAL_C2E_ATK_HIT;
		body_work->wait_timer = 60*2; //TODO PAL
	}

	// プレイヤーの方を向く
	// プレイヤーの方を向かないように変更
//	GmBoss4UtilLookAtPlayer(&body_work->dir, obj_work, 5);

	if (obj_work->pos.y >= body_work->atk_nml_alt) {

		// カプセルを無敵解除
//		gmBoss4CapsuleSetInvincible( 0 );

		// 停止
		GmBsCmnSetObjSpdZero(obj_work);

		// 目標位置に固定する
		obj_work->pos.y	= body_work->atk_nml_alt;
		
		// 速度設定
		GmBsCmnSetObjSpd(obj_work,
						 0,// TODO : 仮
						 0,
						 0);

		// アフターバーナー有効化
//		gmBoss4EffAfterburnerSetEnable(body_work, TRUE);

		// プレイヤーの方を向く(念のため)
//		GmBoss4UtilLookAtPlayer(&body_work->dir, obj_work, 1);

//		gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_PRE_ATK_NML);

		body_work->wait_timer = 30;

		// 処理関数設定
		body_work->proc_update	= gmBoss4BodyStateUpdateStartWithFallWait;

		// プレイヤーの方を向く
		GmBoss4UtilLookAtPlayer(&body_work->dir, obj_work, 28);
	}
}



void gmBoss4BodyStateUpdateStartWithFallWait(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	// プレイヤーの方を向く
	GmBoss4UtilLookAt(&body_work->dir);

	// 少しの間待つ
	if (--body_work->wait_timer == 0)
	{

		// カプセルを無敵解除
		GmBoss4CapsuleSetInvincible( 0 );

		// 停止
		GmBsCmnSetObjSpdZero(obj_work);

		// 目標位置に固定する
		obj_work->pos.y	= body_work->atk_nml_alt;
		
		// 速度設定
		GmBsCmnSetObjSpd(obj_work,
						 FX32_ONE * -1,// TODO : 仮
						 0,
						 0);

		// アフターバーナー有効化
		gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NORMAL);

		// プレイヤーの方を向く(念のため)
		//GmBoss4UtilLookAtPlayer(&body_work->dir, obj_work, 1);

		gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_PRE_ATK_NML);

		// エッグマンのホーミングをONにする
		GMS_BOSS4_EGG_WORK* egg_work = (GMS_BOSS4_EGG_WORK*)(body_work->parts_objs[ GME_BOSS4_PART_IDX_EGG ]);
		GMS_ENEMY_3D_WORK*	ene_3d	= (GMS_ENEMY_3D_WORK*)egg_work;
		ene_3d->ene_com.enemy_flag &= ~GMD_ENEMY_FLAG_NOHOMING;
	}
}



// =======================================================================
// gmBoss4BodyState{Enter|Leave|Update}PreAtkNmlXXX
/*!
  本体 通常攻撃開始ステート遷移時処理関数
 */
// =======================================================================
// 通常攻撃開始ステート開始関数
void gmBoss4BodyStateEnterPreAtkNml(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 矩形当たりオン
	obj_work->flag	&= ~OBD_OBJECT_NOHIT;
	
	// アクション設定
	gmBoss4BodySetActionWhole(body_work, GME_BOSS4_ACT_ID_PRE_ATK_NML_MOVE);
	
	// 通常攻撃開始時 鎖モーション設定初期化
	gmBoss4BodyInitPreANChainMotion(body_work);
	
	// フリップ設定
//	gmBoss4BodySetFlipForAtkNmlMove(body_work);
	
	// 移動開始
	gmBoss4BodyInitPreANMove(body_work);
	
	// アフターバーナー有効化
	gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NORMAL );
	
	// 処理関数設定
	if (body_work->dir.direction == GME_BOSS4_DIR_RIGHT){
		body_work->proc_update	= gmBoss4BodyStateUpdatePreAtkNmlWithMoveRight;

		obj_work->spd_add.x = -obj_work->spd_add.x;
	}else{
		body_work->proc_update	= gmBoss4BodyStateUpdatePreAtkNmlWithMoveLeft;
	}
}

// 通常攻撃開始ステート終了関数
void gmBoss4BodyStateLeavePreAtkNml(GMS_BOSS4_BODY_WORK *body_work)
{
	// アフターバーナー無効化
	gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE );
}


// 通常攻撃開始ステート更新 移動処理(左移動)
void gmBoss4BodyStateUpdatePreAtkNmlWithMoveLeft(GMS_BOSS4_BODY_WORK *body_work)
{
	VecFx32	ofs = {
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_X ),
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_Y ),
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_Z ),
	};
	VecFx32	rot = {
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_X ),
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_Y ),
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_Z ),
	};

	// 上に避ける
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_UP_AVOID){

		// アフターバーナー無効化
		gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE );
		if (!(body_work->flag & GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN)){
			GmBoss4EffCommonInit(	//GME_EFCT_BOSS_CMN_IDX_JET_B,			// コモンエフェクトの場合はENUMを設定する
									GMD_DWORK_NO_BOSS_04_EF_BOOSTX2_ES,		// エフェクト種類(現在仮で指定)
									&ofs,									// オフセット位置
									(OBS_OBJECT_WORK*)body_work,			// 親
									GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,	// 親依存
									GMD_EFFECT_3DES_FLAG_STICKPARENT,		// 親に引っ付く
									&body_work->node_work,					// ノード情報
									GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE,	// ノード番号
									&rot,									// 回転
									&body_work->flag,						// 監視用フラグ
									GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN	// 監視パターン
									);
		}

		OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
		obj_work->spd.x	= 0;
		obj_work->spd_add.x = 0;

		if (body_work->ene_3d.ene_com.obj_work.pos.y > FX_F32_TO_FX32( GMD_BOSS4_BODY_UP_POS_Y_1 )){
			body_work->avoid_yspd += FX_F32_TO_FX32( 0.03f );
		}else{
			body_work->avoid_yspd -= FX_F32_TO_FX32( 0.05f );
			if (body_work->avoid_yspd < FX_F32_TO_FX32( 1.0f )){
				body_work->avoid_yspd = FX_F32_TO_FX32( 1.0f );
			}
		}

		body_work->ene_3d.ene_com.obj_work.pos.y -= body_work->avoid_yspd;
		if (body_work->ene_3d.ene_com.obj_work.pos.y < FX_F32_TO_FX32( GMD_BOSS4_BODY_UP_POS_Y )){
			body_work->ene_3d.ene_com.obj_work.pos.y = FX_F32_TO_FX32( GMD_BOSS4_BODY_UP_POS_Y );
			body_work->avoid_timer--;
		}
		if (body_work->avoid_timer<0){
			body_work->flag &= ~GMD_BOSS4_BODY_FLAG_UP_AVOID;
			body_work->avoid_yspd = 0;
		}
		// 上に避けるときは攻撃あたりを変更する(バーニア部分のみにする)
		ObjRectWorkSet( &body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK], -8, 20, 8, 40);
		return;

	}else{
		// 
		if (body_work->atk_nml_alt > body_work->ene_3d.ene_com.obj_work.pos.y ){
			// 落下する
			body_work->flag |= GMD_BOSS4_BODY_FLAG_DOWN_AVOID;

			// 下に下りるときは攻撃あたりなし
			body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

			// アフターバーナー無効化
			gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE );
			body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN;

			if (body_work->ene_3d.ene_com.obj_work.pos.y > FX_F32_TO_FX32( GMD_BOSS4_BODY_UP_POS_Y_1 )){
				body_work->avoid_yspd -= FX_F32_TO_FX32( 0.05f );
				if (body_work->avoid_yspd < FX_F32_TO_FX32( 1.0f )){
					body_work->avoid_yspd = FX_F32_TO_FX32( 1.0f );
				}
			}else{
				body_work->avoid_yspd += FX_F32_TO_FX32( 0.03f );
			}
			body_work->ene_3d.ene_com.obj_work.pos.y += body_work->avoid_yspd;

			if (body_work->atk_nml_alt <= body_work->ene_3d.ene_com.obj_work.pos.y ){
				gmBoss4BodyInitPreANMove(body_work);

				// アフターバーナー
				gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE );
				gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NORMAL );
				body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN;

				// 当たりを標準にする
				ObjRectWorkSet( &body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK], -32, -8, 32, 40);
				body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_RECT_ENABLE;
			}
			return;
		}else{
			// 固定位置
			body_work->ene_3d.ene_com.obj_work.pos.y = body_work->atk_nml_alt;
			body_work->flag &= ~GMD_BOSS4_BODY_FLAG_DOWN_AVOID;
		}
	}


	// 標準の角度に設定
	GmBoss4UtilSetDirectionNormal(&body_work->dir);
	
	if (gmBoss4BodyUpdatePreANMoveLeft(body_work)) {
		
		// 通常攻撃へ
		gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_ATK_NML);
		
		// 通常攻撃ステート 初回のブレンド設定
		gmBoss4BodySetANChainInitialBlendSpd(body_work);
	}

	// ダメージ中はスピード加速(5倍)
	if (body_work->damage_timer){

		for (int i=1; i< GMD_BOSS4_SPEED_TIMES_IN_DAMAGE ; i++ ){

			if (gmBoss4BodyUpdatePreANMoveLeft(body_work)) {
				
				// 通常攻撃へ
				gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_ATK_NML);
				
				// 通常攻撃ステート 初回のブレンド設定
				gmBoss4BodySetANChainInitialBlendSpd(body_work);
				return;
			}

		}
	}
}

// 通常攻撃開始ステート更新 移動処理(左移動)
void gmBoss4BodyStateUpdatePreAtkNmlWithMoveRight(GMS_BOSS4_BODY_WORK *body_work)
{
	VecFx32	ofs = {
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_X ),
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_Y ),
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_Z ),
	};
	VecFx32	rot = {
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_X ),
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_Y ),
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_Z ),
	};

	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);

	// 上に避ける
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_UP_AVOID){

		// アフターバーナー無効化
		gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE );
		if (!(body_work->flag & GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN)){
			GmBoss4EffCommonInit(	//GME_EFCT_BOSS_CMN_IDX_JET_B,			// コモンエフェクトの場合はENUMを設定する
									GMD_DWORK_NO_BOSS_04_EF_BOOSTX2_ES,		// エフェクト種類(現在仮で指定)
									&ofs,									// オフセット位置
									(OBS_OBJECT_WORK*)body_work,			// 親
									GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,	// 親依存
									GMD_EFFECT_3DES_FLAG_STICKPARENT,		// 親に引っ付く
									&body_work->node_work,					// ノード情報
									GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE,	// ノード番号
									&rot,									// 回転
									&body_work->flag,						// 監視用フラグ
									GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN	// 監視パターン
									);
		}

		obj_work->spd.x	= 0;
		obj_work->spd_add.x = 0;

		if (body_work->ene_3d.ene_com.obj_work.pos.y > FX_F32_TO_FX32( GMD_BOSS4_BODY_UP_POS_Y_1 )){
			body_work->avoid_yspd += FX_F32_TO_FX32( 0.03f );
		}else{
			body_work->avoid_yspd -= FX_F32_TO_FX32( 0.05f );
			if (body_work->avoid_yspd < FX_F32_TO_FX32( 1.0f )){
				body_work->avoid_yspd = FX_F32_TO_FX32( 1.0f );
			}
		}

		body_work->ene_3d.ene_com.obj_work.pos.y -= body_work->avoid_yspd;
		if (body_work->ene_3d.ene_com.obj_work.pos.y < FX_F32_TO_FX32( GMD_BOSS4_BODY_UP_POS_Y )){
			body_work->ene_3d.ene_com.obj_work.pos.y = FX_F32_TO_FX32( GMD_BOSS4_BODY_UP_POS_Y );
			body_work->avoid_timer--;
		}
		if (body_work->avoid_timer<0){
			body_work->flag &= ~GMD_BOSS4_BODY_FLAG_UP_AVOID;
			body_work->avoid_yspd = 0;
		}
		// 上に避けるときは攻撃あたりを変更する(バーニア部分のみにする)
		ObjRectWorkSet( &body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK], -8, 20, 8, 40);

		return;

	}else{
		// 
		if (body_work->atk_nml_alt > body_work->ene_3d.ene_com.obj_work.pos.y ){
			// 落下する

			// 下に下りるときは攻撃あたりなし
			body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

			body_work->flag |= GMD_BOSS4_BODY_FLAG_DOWN_AVOID;

			// アフターバーナー無効化
			gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE );
			body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN;

			if (body_work->ene_3d.ene_com.obj_work.pos.y > FX_F32_TO_FX32( GMD_BOSS4_BODY_UP_POS_Y_1 )){
				body_work->avoid_yspd -= FX_F32_TO_FX32( 0.05f );
				if (body_work->avoid_yspd < FX_F32_TO_FX32( 1.0f )){
					body_work->avoid_yspd = FX_F32_TO_FX32( 1.0f );
				}
			}else{
				body_work->avoid_yspd += FX_F32_TO_FX32( 0.03f );
			}
			body_work->ene_3d.ene_com.obj_work.pos.y += body_work->avoid_yspd;

			if (body_work->atk_nml_alt <= body_work->ene_3d.ene_com.obj_work.pos.y ){
				gmBoss4BodyInitPreANMove(body_work);
				obj_work->spd_add.x = -obj_work->spd_add.x;

				// アフターバーナー
				gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE );
				gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NORMAL );
				body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN;

				// 当たりを標準にする
				ObjRectWorkSet( &body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK], -32, -8, 32, 40);
				body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_RECT_ENABLE;
			}

			return;
		}else{
			// 固定位置
			body_work->ene_3d.ene_com.obj_work.pos.y = body_work->atk_nml_alt;
			body_work->flag &= ~GMD_BOSS4_BODY_FLAG_DOWN_AVOID;
		}
	}

	// 標準の角度に設定
	GmBoss4UtilSetDirectionNormal(&body_work->dir);
	
	if (gmBoss4BodyUpdatePreANMoveRight(body_work)) {
		
		// 通常攻撃へ
		gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_ATK_NML);
		
		// 通常攻撃ステート 初回のブレンド設定
		gmBoss4BodySetANChainInitialBlendSpd(body_work);
	}

	// ダメージ中はスピード加速(5倍)
	if (body_work->damage_timer){

		for (int i=1; i< GMD_BOSS4_SPEED_TIMES_IN_DAMAGE ; i++ ){

			if (gmBoss4BodyUpdatePreANMoveRight(body_work)) {
				
				// 通常攻撃へ
				gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_ATK_NML);
				
				// 通常攻撃ステート 初回のブレンド設定
				gmBoss4BodySetANChainInitialBlendSpd(body_work);
				return;
			}
		}
	}
}

// =======================================================================
// gmBoss4BodyState{Enter|Leave|Update}AtkNmlXXX
/*!
  本体 通常攻撃ステート遷移時処理関数
 */
// =======================================================================
// 通常攻撃ステート開始関数
void gmBoss4BodyStateEnterAtkNml(GMS_BOSS4_BODY_WORK *body_work)
{
	/* 振り向きから開始する */
	
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	BOOL	b_force_change	= FALSE;
	
	// 矩形当たりオン
	obj_work->flag	&= ~OBD_OBJECT_NOHIT;
	
	// アクション設定（左方向時のみ強制設定）
//	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
	if (body_work->dir.direction == GME_BOSS4_DIR_LEFT){
		b_force_change	= TRUE;
	}
	gmBoss4BodySetActionWhole(body_work,
							  GME_BOSS4_ACT_ID_ATK_NML_MOVE,
							  b_force_change);
	
	// 通常攻撃振り向き初期化
	gmBoss4BodyInitAtkNmlFlipAndTurn(body_work);
	
	// 通常攻撃ドリフト移動初期化
	gmBoss4BodyInitAtkNmlDrift(body_work, GMD_BOSS4_BODY_ATKNML_DRIFT_FRAME);
	
	// アフターバーナー無効化
	gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE );
	
	// 処理関数設定
	body_work->proc_update	= gmBoss4BodyStateUpdateAtkNmlWithTurn;
}

// 通常攻撃ステート終了関数
void gmBoss4BodyStateLeaveAtkNml(GMS_BOSS4_BODY_WORK *body_work)
{
	// アフターバーナー無効化
	gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE );
}

// 通常攻撃ステート更新 振り向き処理
void gmBoss4BodyStateUpdateAtkNmlWithTurn(GMS_BOSS4_BODY_WORK *body_work)
{
	BOOL	drift_result;
	drift_result	= gmBoss4BodyUpdateAtkNmlDrift(body_work);
	
	if (gmBoss4BodyUpdateAtkNmlFlipAndTurn(body_work)) {
		if (drift_result) {
			// フリップ設定
			gmBoss4BodySetFlipForAtkNmlMove(body_work);
			
			// 移動開始
			gmBoss4BodyInitAtkNmlMove(body_work, (Sint32)GMD_BOSS4_BODY_ATKNML_MOVE_FRAME );
			
			// アフターバーナー有効化
			gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NORMAL );
			
			// 鉄球振り子SE再生
			//GmSoundPlaySE("Boss4_02");
			
			// 処理関数設定
			body_work->proc_update	= gmBoss4BodyStateUpdateAtkNmlWithMove;
		}
	}
}

// 通常攻撃ステート更新 移動処理
void gmBoss4BodyStateUpdateAtkNmlWithMove(GMS_BOSS4_BODY_WORK *body_work)
{
	VecFx32	ofs = {
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_X ),
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_Y ),
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_Z ),
	};
	VecFx32	rot = {
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_X ),
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_Y ),
		FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_Z ),
	};

	// 上に避ける
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_UP_AVOID){

		// アフターバーナー
		gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE );
		if (!(body_work->flag & GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN)){
			GmBoss4EffCommonInit(	//GME_EFCT_BOSS_CMN_IDX_JET_B,			// コモンエフェクトの場合はENUMを設定する
									GMD_DWORK_NO_BOSS_04_EF_BOOSTX2_ES,		// エフェクト種類(現在仮で指定)
									&ofs,									// オフセット位置
									(OBS_OBJECT_WORK*)body_work,			// 親
									GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,	// 親依存
									GMD_EFFECT_3DES_FLAG_STICKPARENT,		// 親に引っ付く
									&body_work->node_work,					// ノード情報
									GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE,	// ノード番号
									&rot,									// 回転
									&body_work->flag,						// 監視用フラグ
									GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN	// 監視パターン
									);
		}

		if (body_work->ene_3d.ene_com.obj_work.pos.y > FX_F32_TO_FX32( GMD_BOSS4_BODY_UP_POS_Y_1 )){
			body_work->avoid_yspd += FX_F32_TO_FX32( 0.03f );
		}else{
			body_work->avoid_yspd -= FX_F32_TO_FX32( 0.05f );
			if (body_work->avoid_yspd < FX_F32_TO_FX32( 1.0f )){
				body_work->avoid_yspd = FX_F32_TO_FX32( 1.0f );
			}
		}

		body_work->ene_3d.ene_com.obj_work.pos.y -= body_work->avoid_yspd;
		if (body_work->ene_3d.ene_com.obj_work.pos.y < FX_F32_TO_FX32( GMD_BOSS4_BODY_UP_POS_Y )){
			body_work->ene_3d.ene_com.obj_work.pos.y = FX_F32_TO_FX32( GMD_BOSS4_BODY_UP_POS_Y );
			body_work->avoid_timer--;
		}
		if (body_work->avoid_timer<0){
			body_work->flag &= ~GMD_BOSS4_BODY_FLAG_UP_AVOID;
			body_work->avoid_yspd = 0;
		}
		// 上に避けるときは攻撃あたりを変更する(バーニア部分のみにする)
		ObjRectWorkSet( &body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK], -8, 20, 8, 40);

		return;

	}else{
		// 
		if (body_work->atk_nml_alt > body_work->ene_3d.ene_com.obj_work.pos.y ){
			// 落下する

			// 下に下りるときは攻撃あたりなし
			body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

			body_work->flag |= GMD_BOSS4_BODY_FLAG_DOWN_AVOID;

			// アフターバーナー
			gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE );
			body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN;

			if (body_work->ene_3d.ene_com.obj_work.pos.y > FX_F32_TO_FX32( GMD_BOSS4_BODY_UP_POS_Y_1 )){
				body_work->avoid_yspd -= FX_F32_TO_FX32( 0.05f );
				if (body_work->avoid_yspd < FX_F32_TO_FX32( 1.0f )){
					body_work->avoid_yspd = FX_F32_TO_FX32( 1.0f );
				}
			}else{
				body_work->avoid_yspd += FX_F32_TO_FX32( 0.03f );
			}
			body_work->ene_3d.ene_com.obj_work.pos.y += body_work->avoid_yspd;
			if (body_work->atk_nml_alt <= body_work->ene_3d.ene_com.obj_work.pos.y ){
				gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE );
				gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NORMAL );
				body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN;

				// 当たりを標準にする
				ObjRectWorkSet( &body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK], -32, -8, 32, 40);
				body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_RECT_ENABLE;
			}
			return;
		}else{
			// アフターバーナー

			// 固定位置
			body_work->ene_3d.ene_com.obj_work.pos.y = body_work->atk_nml_alt;
			body_work->flag &= ~GMD_BOSS4_BODY_FLAG_DOWN_AVOID;

		}
	}


	// 標準の角度に設定
	GmBoss4UtilSetDirectionNormal(&body_work->dir);
	
	// 第2形態への移行
	if (gmBoss4BodyIsExtraAttack(body_work)) {
		gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_1ST_END );
		return;
	}
	/*
	// 追加攻撃開始チェック
	if (gmBoss4BodyIsExtraAttack(body_work)) {
		gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_ATK_BASH);
		return;
	}
	*/
	
	if (gmBoss4BodyUpdateAtkNmlMove(body_work)) {
		// 通常攻撃へ
		gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_ATK_NML);
		return;
	}

	/*
	// プレイヤーダメージ中はさらに2回移動させる
	GMS_PLAYER_WORK* ply = (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj();

	Sint32 time = (ObjTimeCountGet( ply->invincible_timer ) / FX32_ONE);
	if (time > 60*2){
		if (gmBoss4BodyUpdateAtkNmlMove(body_work)) {
			// 通常攻撃へ
			gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_ATK_NML);
			return;
		}
		if (gmBoss4BodyUpdateAtkNmlMove(body_work)) {
			// 通常攻撃へ
			gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_ATK_NML);
			return;
		}		
	}
	*/
	// エッグマンがダメージを受けると移動を速める
	if (body_work->damage_timer){

		for (int i=1; i< GMD_BOSS4_SPEED_TIMES_IN_DAMAGE ; i++ ){

			if (gmBoss4BodyUpdateAtkNmlMove(body_work)) {
				// 通常攻撃へ
				gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_ATK_NML);
				return;
			}

		}
	}


}

// =======================================================================
// gmBoss4BodyState{Enter|Leave|Update}1stEnd
/*!
  本体 第一形態終了
 */
// =======================================================================
// 第一形態終了ステート開始関数
void gmBoss4BodyStateEnter1stEnd(GMS_BOSS4_BODY_WORK *body_work)
{

	GmBoss4UtilInitNoHitTimer( &body_work->nohit_work, (GMS_ENEMY_COM_WORK*)body_work, 60*20 );
//	body_work->no_hit_timer = 60*20;
	// 完全にあたらなくする
	GMS_ENEMY_3D_WORK	*ene_3d = (GMS_ENEMY_3D_WORK*)body_work;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag	|= OBD_RECT_NOHIT;


	GmBoss4CapsuleSetInvincible(60*10, FALSE);
	// ちびエッグマン無敵
	GmBoss4ChibiSetInvincible(TRUE);
	GmBoss4ChibiExplosion();

	// ソニックをとめる
	GmBoss4UtilPlayerStop( true );

	// カウンターストップ
	GmBoss4UtilTimerStop( true );

	// 無敵にして画面中央上に移動

	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)body_work;

	// 位置設定
	OBS_CAMERA*	obj_cam = ObjCameraGet(GME_CAMERA_NO_MAIN);
	
	// 画面の真ん中よりも少し上
	VecFx32	end = { 
		FX_F32_TO_FX32( obj_cam->target_pos.x ),	//* FX32_ONE,
#if _IPHONE
		FX_F32_TO_FX32( GMD_BOSS4_BODY_1STEND_POS_Y),
#else
		FX_F32_TO_FX32( -obj_cam->target_pos.y + GMD_BOSS4_BODY_1STEND_POS_Y_FROM_CENTER ),
#endif // _IPHONE
		0 
	};

	// 移動初期化する
	GmBoss4UtilInitMove( &body_work->move_work, &obj_work->pos, &end, GMD_BOSS4_BODY_1STEND_ARRIVED_TIME, 1 );

	// 左を向く
	BOOL	dir = GmBoss4UtilIsDirectionPositiveFromCurrent( &body_work->dir, GMD_BOSS4_LEFTWARD_ANGLE);
	body_work->dir.direction = GME_BOSS4_DIR_LEFT;
	GmBoss4UtilInitTurnGently( &body_work->dir, GMD_BOSS4_LEFTWARD_ANGLE, GMD_BOSS4_BODY_1STEND_TRUN_TIME, dir);

	// 処理移行
	body_work->proc_update	= gmBoss4BodyStateUpdate1stEnd;
}

void gmBoss4BodyStateUpdate1stEnd(GMS_BOSS4_BODY_WORK *body_work)
{
	// ソニックをとめる
	GmBoss4UtilPlayerStop( true );
	// 念のためここで「ちびエッグマン」を破壊
	GmBoss4ChibiExplosion();

	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)body_work;

	GmBoss4UtilUpdateTurnGently( &body_work->dir );

	// TODO PAL
	if (body_work->move_work.now_count == 30){
//-------------------------------------
		// 下のバーニア発生
		// 張り付くオフセットと回転
		VecFx32	ofs = {
			FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_X ),
			FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_Y ),
			FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_Z ),
		};
		VecFx32	rot = {
			FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_X ),
			FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_Y ),
			FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_Z ),
		};

		GmBoss4EffCommonInit(	//GME_EFCT_BOSS_CMN_IDX_JET_B,			// コモンエフェクトの場合はENUMを設定する
								GMD_DWORK_NO_BOSS_04_EF_BOOSTX2_ES,		// エフェクト種類(現在仮で指定)
								&ofs,									// オフセット位置
								(OBS_OBJECT_WORK*)body_work,			// 親
								GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,	// 親依存
								GMD_EFFECT_3DES_FLAG_STICKPARENT,		// 親に引っ付く
								&body_work->node_work,					// ノード情報
								GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE,	// ノード番号
								&rot,									// 回転
								&body_work->flag,						// 監視用フラグ
								GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN	// 監視パターン
								);
//-------------------------------------
	}

	// 無敵にして画面中央上に移動中
	if ( GmBoss4UtilUpdateMove( &body_work->move_work ) ){
		// 移動終了したら爆発演出か怒り演出へ
		// 残りカプセル数がある?
		if (GmBoss4CapsuleGetCount()>0){
			// カプセルがあれば爆発演出へ
			body_work->proc_update	= gmBoss4BodyStateInit1stEndExplosion;
		}else{
			// なければその場で怒り演出へ
			body_work->proc_update	= gmBoss4BodyStateInit1stEndAngry;
		}
	}

	GmBoss4UtilUpdateMovePosition( &body_work->move_work, obj_work );
}

// 爆発中
void gmBoss4BodyStateInit1stEndExplosion(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)body_work;
	// ソニックをとめる
	GmBoss4UtilPlayerStop( true );

	// カプセル爆発させる(ちびエッグマンは生まないように)
	GmBoss4CapsuleExplosion();

	body_work->wait_timer = GMD_BOSS4_BODY_1STEND_EXPLOSION_TIME;	
	// エッグマンに黒こげ通知
	body_work->flag	|= GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_BURNT;

	VecFx32	end = { obj_work->pos.x, obj_work->pos.y + FX_F32_TO_FX32( GMD_BOSS4_BODY_1STEND_EXPLOSION_POS_Y_FROM_CENTER ), 0 };
	GmBoss4UtilInitMove( &body_work->move_work, &obj_work->pos, &end, GMD_BOSS4_BODY_1STEND_EXPLOSION_ARRIVED_TIME, 1 );

	// 処理移行
	body_work->proc_update	= gmBoss4BodyStateUpdate1stEndExplosion;
}

void gmBoss4BodyStateUpdate1stEndExplosion(GMS_BOSS4_BODY_WORK *body_work)
{
	// ソニックをとめる
	GmBoss4UtilPlayerStop( true );

	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)body_work;
	GmBoss4UtilUpdateMove( &body_work->move_work );
	GmBoss4UtilUpdateMovePosition( &body_work->move_work, obj_work );

	// カプセル爆発させ中
	if (body_work->wait_timer > 0)
	{
		body_work->wait_timer--;
	}else{
		// 怒りへ
		body_work->proc_update	= gmBoss4BodyStateInit1stEndAngry;	
	}
}


void gmBoss4BodyStateInit1stEndAngry(GMS_BOSS4_BODY_WORK *body_work)
{
	// ソニックをとめる
	GmBoss4UtilPlayerStop( true );

	// 怒りアクションに変更
	gmBoss4BodySetActionWhole(body_work, GME_BOSS4_ACT_ID_ANGRY_L1);

	// 怒り中へ
	body_work->proc_update	= gmBoss4BodyStateUpdate1stEndAngry;	
	
}

void gmBoss4BodyStateUpdate1stEndAngry(GMS_BOSS4_BODY_WORK *body_work)
{
	// ソニックをとめる
	GmBoss4UtilPlayerStop( true );

	// いかりのアクション中
	if (GmBsCmnIsActionEnd( (OBS_OBJECT_WORK*)body_work )) {
		// 第一段階怒り終了
		// 第二段階怒りへ

		// 怒りアクションL2に変更
		gmBoss4BodySetActionWhole(body_work, GME_BOSS4_ACT_ID_ANGRY_L2);

		// マシンを変形アクションに

#if 0
//-------------------------------------
		// 下のバーニア発生
		// 張り付くオフセットと回転
		VecFx32	ofs = {
			FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_X ),
			FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_Y ),
			FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_OFST_Z ),
		};
		VecFx32	rot = {
			FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_X ),
			FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_Y ),
			FX_F32_TO_FX32( GMD_BOSS4_EFF_ABURNER5_DISP_ROT_Z ),
		};

		GmBoss4EffCommonInit(	//GME_EFCT_BOSS_CMN_IDX_JET_B,			// コモンエフェクトの場合はENUMを設定する
								GMD_DWORK_NO_BOSS_04_EF_BOOSTX2_ES,		// エフェクト種類(現在仮で指定)
								&ofs,									// オフセット位置
								(OBS_OBJECT_WORK*)body_work,			// 親
								GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,	// 親依存
								GMD_EFFECT_3DES_FLAG_STICKPARENT,		// 親に引っ付く
								&body_work->node_work,					// ノード情報
								GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE,	// ノード番号
								&rot,									// 回転
								&body_work->flag,						// 監視用フラグ
								GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN	// 監視パターン
								);
//-------------------------------------
#endif
	

		body_work->proc_update	= gmBoss4BodyStateUpdate1stEndAngryL2;
	}
}

void gmBoss4BodyStateUpdate1stEndAngryL2(GMS_BOSS4_BODY_WORK *body_work)
{
	// ソニックをとめる
	GmBoss4UtilPlayerStop( true );

	// いかりのアクション中
	if (GmBsCmnIsActionEnd( (OBS_OBJECT_WORK*)body_work )) {
		// 第二段階怒り終了
		// 第三段階怒りへ

		// 怒りアクションL3に変更
		gmBoss4BodySetActionWhole(body_work, GME_BOSS4_ACT_ID_ANGRY_L3);

		// 逃げる動きに変更
		body_work->proc_update	= gmBoss4BodyStateInit1stEndEscape;
	}
}

void gmBoss4BodyStateInit1stEndEscape(GMS_BOSS4_BODY_WORK *body_work)
{
	// ソニックをとめる
	GmBoss4UtilPlayerStop( true );

	// アフターバーナー有効化(2段階目!)
	gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_EX );

	// みぎへ移動し、スクロール開始
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)body_work;

	// 移動初期化
	OBS_CAMERA*	obj_cam = ObjCameraGet(GME_CAMERA_NO_MAIN);
	VecFx32	end = { FX_F32_TO_FX32(obj_cam->target_pos.x + GMD_BOSS4_BODY_1STEND_ESCAPE_POS/*200.0f*/) + ((GMM_BOSS4_AREA_RIGHT() - GMM_BOSS4_AREA_LEFT()) / 2), obj_work->pos.y, 0 };
	GmBoss4UtilInitMove( &body_work->move_work, &obj_work->pos, &end, (Sint32)GMD_BOSS4_BODY_1STEND_ESCAPE_ARRIVED_TIME, 1 );
	
#if _IPHONE
	GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_HORI);
#endif // _IPHONE

	// 処理移行
	body_work->proc_update	= gmBoss4BodyStateUpdate1stEndEscape;	
}

void gmBoss4BodyStateUpdate1stEndEscape(GMS_BOSS4_BODY_WORK *body_work)
{
	// ソニックをとめる
	GmBoss4UtilPlayerStop( true );

	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)body_work;

	// みぎへ移動し、スクロール開始
	if ( GmBoss4UtilUpdateMove( &body_work->move_work ) ){
		// 画面外になったら
		gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_2ND );
		return;
	}

	GmBoss4UtilUpdateMovePosition( &body_work->move_work, obj_work );

	// ループマップに突入

}

// =======================================================================
// gmBoss4BodyState{Enter|Leave|Update}2nd
/*!
  本体 第2形態終了
 */
// =======================================================================
void gmBoss4BodyStateEnter2nd(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)body_work;
	// 第2形態ボス戦スタート

	GmBoss4UtilInitNoHitTimer( &body_work->nohit_work, (GMS_ENEMY_COM_WORK*)body_work, 0 );
//	body_work->no_hit_timer = 60*20;
//	body_work->no_hit_timer = 0;

	// カプセルを無敵解除
	GmBoss4CapsuleSetInvincible(0);
	// ちびエッグマン無敵解除
	GmBoss4ChibiSetInvincible(FALSE);

	// ソニック動作させる!
	GmBoss4UtilPlayerStop( false );
	// カウンター動作させる!
	GmBoss4UtilTimerStop( false );

	// スクロールロック解除
	if (!GmBoss4CheckBossRush()){
		OBS_OBJECT_WORK* scrrel_obj = GmGmkCamScrLimitRelease(GMD_GMK_SCR_LMT_RELEASE_RIGHT);
		scrrel_obj->user_work = 0x10;	// Scr制限リリース速度値変更(widh/height共用)
	}

	obj_work->pos.x	=	FX_F32_TO_FX32( GMD_BOSS4_BODY_2ND_POS_X );
	obj_work->pos.y =	FX_F32_TO_FX32( GMD_BOSS4_BODY_2ND_POS_Y );
	obj_work->pos.z =	GMD_OBJ_DEFAULT_POS_Z_C_BACK;

	// 押し当たりはなくす
	GMS_ENEMY_3D_WORK	*ene_3d = (GMS_ENEMY_3D_WORK*)body_work;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;	
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag |= OBD_RECT_NOHIT;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;	
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_RECT_NOHIT;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag	&= ~OBD_RECT_NOHIT;

	ObjRectWorkSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF], -38, -24, 38, 32);

/*
	// エッグマンのみ左に向ける(仮) // TOD 独立アクションがあるため 最終的に方向関数をEggmanに持たせる必要があるか?
	GMS_BOSS4_EGG_WORK* egg_work = (GMS_BOSS4_EGG_WORK*)(body_work->parts_objs[ GME_BOSS4_PART_IDX_EGG ]);
	GmBoss4UtilInitTurnGently( &egg_work->dir_work, AKM_DEGtoA16(180.f), 1, FALSE);

	GmBoss4UtilInitTurnGently( &body_work->dir, GMD_BOSS4_RIGHTWARD_ANGLE_2ND, 1, TRUE);
	GmBoss4UtilUpdateTurnGently( &body_work->dir );
*/
	// 通常テクスチャに戻す
	GMS_BOSS4_EGG_WORK* egg_work = (GMS_BOSS4_EGG_WORK*)(body_work->parts_objs[ GME_BOSS4_PART_IDX_EGG ]);
	gmBoss4SetPartTextureBurnt( (OBS_OBJECT_WORK*)egg_work, false );


	// TODO 第２段階のアクションに変化
	GmBoss4UtilInitTurnGently( &body_work->dir, GMD_BOSS4_LEFTWARD_ANGLE, 1, FALSE);
	GmBoss4UtilUpdateTurnGently( &body_work->dir );


	// 通常アクションに戻す TODO 第二形態
	gmBoss4BodySetActionWhole(body_work, GME_BOSS4_ACT_ID_ATK_NML_MOVE2 );

	// 処理移行
	body_work->proc_update	= gmBoss4BodyStateInit2nd;

	// BOSSラッシュ用
	// カプセル爆発させる(ちびエッグマンは生まないように)
	GmBoss4CapsuleExplosion();
	// 左を向く
	BOOL	dir = GmBoss4UtilIsDirectionPositiveFromCurrent( &body_work->dir, GMD_BOSS4_LEFTWARD_ANGLE);
	body_work->dir.direction = GME_BOSS4_DIR_LEFT;
	GmBoss4UtilInitTurnGently( &body_work->dir, GMD_BOSS4_LEFTWARD_ANGLE, 1, dir);
	GmBoss4UtilUpdateTurnGently( &body_work->dir );
	
	/*
	if (GmBoss4CheckBossRush()){
		gmBoss4BodySetActionWhole(body_work, GME_BOSS4_ACT_ID_START_2 );
	}
	*/
	gm_boss4_locking = 0;
}

// 現在は一番はしにおいてる
void gmBoss4BodyStateInit2nd(GMS_BOSS4_BODY_WORK *body_work)
{
#if 0
	// 現在はこれが必要だが、正式なボスラッシュにはいらない可能性あり
	if (GmBoss4CheckBossRush()){
		if (gmBoss4IsScrollLocked()) {
			gm_boss4_locking = 1;
		}
		if (gm_boss4_locking==1 && !gmBoss4IsScrollLocked()){
			// ロックをはずして進む
			GmGmkCamScrLimitRelease(GMD_GMK_SCR_LMT_RELEASE_RIGHT);
//			gmBoss4BodySetActionWhole(body_work, GME_BOSS4_ACT_ID_START_2 );
//			gmBoss4BodySetActionWhole(body_work, GME_BOSS4_ACT_ID_ATK_NML_MOVE2 );

//			gmBoss4BodySetActionWhole(body_work, GME_BOSS4_ACT_ID_ANGRY_L3);

		}else{
			// ボスラッシュの場合はロックするまで待つ
			return;
		}
	}
#endif
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)body_work;

	// 位置に着いたらここに移動
	// TODO プレイヤーポジションが2500になったら(仮) ボスラッシュ時には変更しなければならない
	OBS_OBJECT_WORK* ply = (OBS_OBJECT_WORK*)GmBsCmnGetPlayerObj();

	fx32	pos = FX_F32_TO_FX32( GMD_BOSS4_SCROLL_INIT_X );
	if (ply->pos.x >= pos ){
		body_work->proc_update	= gmBoss4BodyStateUpdate2ndWaitBoss;

		// 強制スクロール
		GmBoss4ScrollInit( NULL, 0, 0, 0 );

		body_work->wait_timer = GMD_BOSS4_BODY_SONIC_CTRL_TIME;	//60*8;	// 初期は8秒にまつ

		if (GmBoss4CheckBossRush()){
			// 通常アクションに戻す TODO 第二形態
			gmBoss4BodySetActionWhole(body_work, GME_BOSS4_ACT_ID_ATK_NML_MOVE2 );

			gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE );
			// アフターバーナー有効化(2段階目!)
			gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_EX );

		}else{
			//ボスラッシュで無い場合、BGMを変更する
			GmSoundChangeAngryBossBGM();

		}
		
#if _IPHONE
		GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_ZONE4_BOSS_2);
#endif // _IPHONE
		
	}

	obj_work->pos.y = FX_F32_TO_FX32( GMD_BOSS4_BODY_2ND_POS_Y );

//	GMS_PLAYER_WORK* ply = (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj();
//	GmPlayerCameraOffsetSet( ply, 180.0f, 0.0f);

	// とりあえず無敵にしておく
	//GmBoss4ChibiSetInvincible(TRUE);
}

// ボスが出てくるまで待つ
void gmBoss4BodyStateUpdate2ndWaitBoss(GMS_BOSS4_BODY_WORK *body_work)
{
	
	GMS_PLAYER_WORK* ply_work = (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj();
//	if (ply_work->seq_state == GME_PLY_SEQ_STATE_WALK) {	// ソニックが通常走りモーションになっているかチェック
	if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {	// ソニックが接地状態かチェック
		// ソニック動作させない
		GmBoss4UtilPlayerStop( true );
	}
	// ボスが画面内に出てくるまで攻撃は待つ
	Float right = (Float)g_gm_main_system.map_fcol.right;
	if (((OBS_OBJECT_WORK*)body_work)->pos.x < FX_F32_TO_FX32(right-10.0f) ) {
		return;
	}

	// 攻撃のタイミングが来るまで待つ
	if (body_work->wait_timer>0){
		body_work->wait_timer--;
		return;
	}

	body_work->proc_update	= gmBoss4BodyStateUpdate2nd;

	// 強制スクロール
	GmBoss4ScrollInit( NULL, 0, 0, 0 );

	body_work->wait_timer = GMD_BOSS4_BODY_CREATE_CAP_FIRST_TIME;

	// ソニック動作させる!!
	GmBoss4UtilPlayerStop( false );
}



// 攻撃のタイミングをうかがう
void gmBoss4BodyStateUpdate2nd(GMS_BOSS4_BODY_WORK *body_work)
{
	// 攻撃のタイミングが来るまで待つ
	if (body_work->wait_timer>0){
		body_work->wait_timer--;
		return;
	}

	// 攻撃のタイミングがきたら
	body_work->proc_update	= gmBoss4BodyStateInit2ndAttack;

	body_work->wait_timer = 34; // TODO PAL対応

	// エッグマンを投げるモーションにする
	body_work->flag |= GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_THROW;


	// とりあえず初期１フレームはきちんとデータが取れないため
	// 先に１回取得しておく
	GMS_BOSS4_EGG_WORK* egg_obj = (GMS_BOSS4_EGG_WORK*)(body_work->parts_objs[ GME_BOSS4_PART_IDX_EGG ]);	
	GmBoss4UtilGetNodeMatrix( &egg_obj->node_work, GMD_BOSS4_EGG_NODE_IDX_HAND_RIGHT );
	GmBoss4UtilGetNodeMatrix( &egg_obj->node_work, GMD_BOSS4_EGG_NODE_IDX_HAND_LEFT );
}


// ちびエッグマンの攻撃開始(1発目)
void gmBoss4BodyStateInit2ndAttack(GMS_BOSS4_BODY_WORK *body_work)
{

	// 投げるモーションが終了したら
	if (body_work->wait_timer>0){
		body_work->wait_timer--;
		return;
	}

	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)body_work;


	// ちびエッグマンを吐き出す( TODO カプセルの地面衝突時に入れ込む)
	OBS_OBJECT_WORK	*obj_chibi;
//	static GMS_EVE_RECORD_EVENT	rec[16];
//	static Sint32	recIndex = 0;

//	recIndex++;
//	recIndex %= 16;

	// １フレーム遅くなるので、エッグマンと同じ仕組みでいちが飛ぶ可能性をなくす
	// エッグマン設置ノードにくっつける(ワープ対策つき)
	GMS_BOSS4_EGG_WORK* egg_obj = (GMS_BOSS4_EGG_WORK*)(body_work->parts_objs[ GME_BOSS4_PART_IDX_EGG ]);	
	const NNS_MATRIX* mtx1 = GmBoss4UtilGetNodeMatrix( &egg_obj->node_work, GMD_BOSS4_EGG_NODE_IDX_HAND_RIGHT );	// エッグマン設置場所
	const NNS_MATRIX* mtx2 = GmBoss4UtilGetNodeMatrix( &body_work->node_work, GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE );	// 中心

	NNS_VECTOR	v;
	
	v.x = NNM_MTX( *mtx1, 0, 3 ) - NNM_MTX( *mtx2, 0, 3 ) + (Float)obj_work->pos.x / FX32_ONE;
	v.y = NNM_MTX( *mtx1, 1, 3 );	// yとzはそのまま使用する
	v.z = NNM_MTX( *mtx1, 2, 3 );
/*
	// 手の位置にあわせる
	// TODO １フレーム遅くなるので、エッグマンと同じ仕組みでいちが飛ぶ可能性をなくす
	GMS_BOSS4_EGG_WORK* egg_obj = (GMS_BOSS4_EGG_WORK*)(body_work->parts_objs[ GME_BOSS4_PART_IDX_EGG ]);	
	NNS_MATRIX*	mtx = GmBoss4UtilGetNodeMatrix( &egg_obj->node_work, GMD_BOSS4_EGG_NODE_IDX_HAND_RIGHT );

	NNS_VECTOR	v;
	nnCopyMatrixTranslationVector( &v, mtx );
*/

	// GmBoss4CapsuleInit2nd()の呼び出し
	obj_chibi = GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS4_CAP_2,
											 FX_F32_TO_FX32( v.x + GMD_BOSS4_BODY_CREATE_CAP_OFFSET_X ), 
											-FX_F32_TO_FX32( v.y + GMD_BOSS4_BODY_CREATE_CAP_OFFSET_Y ),
											0,//flag
											0,0,0,0,
											0);

	obj_chibi->parent_obj = obj_work;//->parent_obj;
	GmBoss4IncObjCreateCount();

	// 方向を設定
	if ( gmBoss4ChibiGetThrowType() ){
		// 基本の方向
		obj_chibi->spd.x = FX_F32_TO_FX32( GMD_BOSS4_BODY_CREATE_CAP_THROW_SPD_X_1 );
		obj_chibi->spd.y = FX_F32_TO_FX32( GMD_BOSS4_BODY_CREATE_CAP_THROW_SPD_Y_1 );
	}else{
		obj_chibi->spd.x = FX_F32_TO_FX32( GMD_BOSS4_BODY_CREATE_CAP_THROW_SPD_X_2 );
		obj_chibi->spd.y = FX_F32_TO_FX32( GMD_BOSS4_BODY_CREATE_CAP_THROW_SPD_Y_2 );
	}
	obj_chibi->pos.z = GMD_OBJ_DEFAULT_POS_Z_C;

	// ライフチェック
	if ((GmBoss4GetLife() < GME_BOSS4_LIFE_H ) && (GmBoss4GetLife() > GME_BOSS4_LIFE_L )) {
		// ミドルライフのとき(あくまで定数ではもたない[1つとは限らないため])
		// ２発目を撃つ
		body_work->proc_update	= gmBoss4BodyStateUpdate2ndAttackWait;

		// 仕様では60フレーム後に
		body_work->wait_timer = GMD_BOSS4_BODY_CREATE_CAP_TIMING_LIFE_2_2; // TODO PAL対応

	}else{
		// 1発で終了
		body_work->proc_update	= gmBoss4BodyStateUpdate2ndAttack;	
	}
}

// 攻撃のタイミングをうかがう
void gmBoss4BodyStateUpdate2ndAttackWait(GMS_BOSS4_BODY_WORK *body_work)
{
	// 攻撃のタイミングが来るまで待つ
	if (body_work->wait_timer>0){
		body_work->wait_timer--;
		return;
	}

	// 攻撃のタイミングがきたら
	body_work->proc_update	= gmBoss4BodyStateInit2ndAttack2;

	body_work->wait_timer = 34; // TODO PAL対応

	// エッグマンを投げるモーションにする
	body_work->flag |= GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_THROW_L;
}


// ちびエッグマンの攻撃開始(二発目)
void gmBoss4BodyStateInit2ndAttack2(GMS_BOSS4_BODY_WORK *body_work)
{
	// 投げるモーションが終了したら
	if (body_work->wait_timer>0){
		body_work->wait_timer--;
		return;
	}

	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)body_work;

	// ちびエッグマンを吐き出す
	OBS_OBJECT_WORK	*obj_chibi = NULL;

	// 手の位置にあわせる
	// １フレーム遅くなるので、エッグマンと同じ仕組みでいちが飛ぶ可能性をなくす
	// エッグマン設置ノードにくっつける(ワープ対策つき)
	GMS_BOSS4_EGG_WORK* egg_obj = (GMS_BOSS4_EGG_WORK*)(body_work->parts_objs[ GME_BOSS4_PART_IDX_EGG ]);	
	const NNS_MATRIX* mtx1 = GmBoss4UtilGetNodeMatrix( &egg_obj->node_work, GMD_BOSS4_EGG_NODE_IDX_HAND_LEFT );	// エッグマン設置場所
	const NNS_MATRIX* mtx2 = GmBoss4UtilGetNodeMatrix( &body_work->node_work, GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE );	// 中心

	NNS_VECTOR	v;
	
	v.x = NNM_MTX( *mtx1, 0, 3 ) - NNM_MTX( *mtx2, 0, 3 ) + (Float)obj_work->pos.x / FX32_ONE;
	v.y = NNM_MTX( *mtx1, 1, 3 );	// yとzはそのまま使用する
	v.z = NNM_MTX( *mtx1, 2, 3 );
/*
	GMS_BOSS4_EGG_WORK* egg_obj = (GMS_BOSS4_EGG_WORK*)(body_work->parts_objs[ GME_BOSS4_PART_IDX_EGG ]);	
	NNS_MATRIX*	mtx = GmBoss4UtilGetNodeMatrix( &egg_obj->node_work, GMD_BOSS4_EGG_NODE_IDX_HAND_LEFT );

	nnCopyMatrixTranslationVector( &v, mtx );
*/
	// GmBoss4CapsuleInit2nd()の呼び出し
	obj_chibi = GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS4_CAP_2,
											 FX_F32_TO_FX32( v.x + GMD_BOSS4_BODY_CREATE_CAP_OFFSET_X ), 
											-FX_F32_TO_FX32( v.y + GMD_BOSS4_BODY_CREATE_CAP_OFFSET_Y ),
											0,//flag
											0,0,0,0,
											0);

	obj_chibi->parent_obj = obj_work;//->parent_obj;
	GmBoss4IncObjCreateCount();

	// 方向を設定
	if ( gmBoss4ChibiGetThrowType() ){
		// 基本の方向
		obj_chibi->spd.x = FX_F32_TO_FX32( GMD_BOSS4_BODY_CREATE_CAP_THROW_SPD_X_1 );
		obj_chibi->spd.y = FX_F32_TO_FX32( GMD_BOSS4_BODY_CREATE_CAP_THROW_SPD_Y_1 );
	}else{
		obj_chibi->spd.x = FX_F32_TO_FX32( GMD_BOSS4_BODY_CREATE_CAP_THROW_SPD_X_2 );
		obj_chibi->spd.y = FX_F32_TO_FX32( GMD_BOSS4_BODY_CREATE_CAP_THROW_SPD_Y_2 );
	}

	// エッグマンの種類を発射し、つぎへ
	body_work->proc_update	= gmBoss4BodyStateUpdate2ndAttack;	
}

// 攻撃中
void gmBoss4BodyStateUpdate2ndAttack(GMS_BOSS4_BODY_WORK *body_work)
{
	// 発射間隔を設定
	body_work->wait_timer = GMD_BOSS4_BODY_CREATE_CAP_TIMING_LIFE_3;
	if (GmBoss4GetLife()==1){
		body_work->wait_timer = GMD_BOSS4_BODY_CREATE_CAP_TIMING_LIFE_1;
	}
	if (GmBoss4GetLife()==2){
		body_work->wait_timer = GMD_BOSS4_BODY_CREATE_CAP_TIMING_LIFE_2;
	}

	// タイミングをうかがう
	body_work->proc_update	= gmBoss4BodyStateUpdate2nd;
}



void gmBoss4BodyStateLeave1stEnd(GMS_BOSS4_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
}

void gmBoss4BodyStateLeave2nd(GMS_BOSS4_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
}


// =======================================================================
// gmBoss4BodyState{Enter|Leave|Update}DmgNmlXXX
/*!
  本体 通常ダメージステート遷移時処理関数
 */
// =======================================================================
// 通常ダメージステート開始関数
void gmBoss4BodyStateEnterDmgNml(GMS_BOSS4_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
}

// 通常ダメージステート終了関数
void gmBoss4BodyStateLeaveDmgNml(GMS_BOSS4_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
}





//TODO 現在BOSS1と同じのため、修正が必要
// =======================================================================
// gmBoss4BodyStateEnterDefeat
/*!
  本体 撃破ステート遷移時処理関数
 */
// =======================================================================
// 撃破ステート開始関数
void gmBoss4BodyStateEnterDefeat(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 矩形当たりオフ
	obj_work->flag	|= OBD_OBJECT_NOHIT;
	
#if 1	// TODO : 仮
	// アニメーションストップ
//	obj_work->disp_flag	|= OBD_DISP_STOP;
#endif
	
	// 鎖を死亡状態にする
	body_work->flag	|= GMD_BOSS4_BODY_FLAG_CHAIN_DEAD;
	
	// 速度設定
	GmBsCmnSetObjSpdZero(obj_work);
	
	// 待機時間設定
	body_work->wait_timer	= 40;//仮
	
	// 処理関数設定
	body_work->proc_update	= gmBoss4BodyStateUpdateDefeatWithWaitStart;

	// アフターバーナー無効化
	gmBoss4EffAfterburnerSetEnable(body_work, GME_BOSS4_BODY_ABURNER_TYPE_NONE );

	// 強制スクロールを次の段階に移す
	GmBoss4ScrollNext();


	// ちびエッグマン無敵
	GmBoss4UtilInitNoHitTimer( &body_work->nohit_work, (GMS_ENEMY_COM_WORK*)body_work, 60*20 );
	GmBoss4CapsuleSetInvincible(60*10, FALSE);
	GmBoss4ChibiSetInvincible(TRUE);
}

// 撃破ステート終了関数
void gmBoss4BodyStateLeaveDefeat(GMS_BOSS4_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
}

// 撃破ステート更新 開始待機処理
void gmBoss4BodyStateUpdateDefeatWithWaitStart(GMS_BOSS4_BODY_WORK *body_work)
{
	GmBoss4ChibiExplosion();

	// 既定時間待機
	if (body_work->wait_timer > 0) {
		body_work->wait_timer--;
	}
	else {
		
		// 小爆発初期化
		GmBoss4EffBombInitCreate(&body_work->bomb_work,
								 GME_BOSS4_EFF_BOMB_TYPE_SMALL,
								 GMM_BS_OBJ(body_work),
								 GMM_BS_OBJ(body_work)->pos.x,//仮
								 GMM_BS_OBJ(body_work)->pos.y,//仮
								 FX_F32_TO_FX32( GMD_BOSS4_BODY_DEFEAT_BOMB_SMALL_LEN_X ),	//FX32_ONE * 80,//仮
								 FX_F32_TO_FX32( GMD_BOSS4_BODY_DEFEAT_BOMB_SMALL_LEN_Y ),	//FX32_ONE * 80,//仮
								 GMD_BOSS4_BODY_DEFEAT_BOMB_SMALL_INTERVAL_MIN_TIME,
								 GMD_BOSS4_BODY_DEFEAT_BOMB_SMALL_INTERVAL_MAX_TIME
								 );
		// 破片初期化
		GmBoss4EffBombInitCreate(&body_work->bomb_work2,
								 GME_BOSS4_EFF_BOMB_TYPE_PARTS,
								 GMM_BS_OBJ(body_work),
								 GMM_BS_OBJ(body_work)->pos.x,//仮
								 GMM_BS_OBJ(body_work)->pos.y,//仮
								 FX_F32_TO_FX32( GMD_BOSS4_BODY_DEFEAT_BOMB_PARTS_LEN_X ),	//FX32_ONE * 80,//仮
								 FX_F32_TO_FX32( GMD_BOSS4_BODY_DEFEAT_BOMB_PARTS_LEN_Y ),	//FX32_ONE * 80,//仮
								 GMD_BOSS4_BODY_DEFEAT_BOMB_PARTS_INTERVAL_MIN_TIME,
								 GMD_BOSS4_BODY_DEFEAT_BOMB_PARTS_INTERVAL_MAX_TIME
								 );

		// 爆発時間
		body_work->wait_timer	= GMD_BOSS4_BODY_DEFEAT_BOMB_SMALL_TIME;
		
		// 処理関数設定
		body_work->proc_update	= gmBoss4BodyStateUpdateDefeatWithExplode;

		//GmGmkCamScrLimitRelease(GMD_GMK_SCR_LMT_RELEASE_RIGHT|GMD_GMK_SCR_LMT_RELEASE_LEFT|GMD_GMK_SCR_LMT_RELEASE_BOTTOM|GMD_GMK_SCR_LMT_RELEASE_TOP);

	}
}

// 撃破ステート更新 爆発処理
void gmBoss4BodyStateUpdateDefeatWithExplode(GMS_BOSS4_BODY_WORK *body_work)
{
	if (body_work->wait_timer > 0) {
		body_work->wait_timer--;
		
		// 小爆発更新
		GmBoss4EffBombUpdateCreate(&body_work->bomb_work);
		GmBoss4EffBombUpdateCreate(&body_work->bomb_work2);

		// 徐々に左に移動(gmBoss4.cppにて)
		body_work->bomb_work.pos[0]		-= FX_F32_TO_FX32(GMD_BOSS4_SCROLL_SPD_MAX - GMD_BOSS4_SCROLL_SPD_BOSS_BROKEN);	// (0.5f*FX32_ONE);
		body_work->bomb_work2.pos[0]	-= FX_F32_TO_FX32(GMD_BOSS4_SCROLL_SPD_MAX - GMD_BOSS4_SCROLL_SPD_BOSS_BROKEN);	//(0.5f*FX32_ONE);
	}
	else {
		// パーツバラバラを鎖パーツに通知
		//body_work->flag	|= GMD_BOSS4_BODY_FLAG_SIGNAL_B2C_SCATTER;
		// TODO パーツの一部を飛ばす?
		
		// 大爆発SE再生
		GmSoundPlaySE("Boss0_03");

		// コントローラー振動
		GMM_PAD_VIB_MID_TIME(120);

		// 画面フラッシュ設定
		GmBsCmnInitFlashScreen( &body_work->flash_work, 
											GMD_BOSS4_BODY_DEFEAT_FLASH_INTO_TIME,
											GMD_BOSS4_BODY_DEFEAT_FLASH_KEEP_TIME,
											GMD_BOSS4_BODY_DEFEAT_FLASH_RETURN_TIME );

		// 得点加算
		// スコア加算 (ボス共通)
		GmPlayerAddScoreNoDisp((GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj(), GMD_PLY_SCORE_BOSS);
 
		// 大爆発生成
		{
			OBS_OBJECT_WORK	*bomb_obj;
			bomb_obj	= (OBS_OBJECT_WORK*)GmEfctCmnEsCreate(GMM_BS_OBJ(body_work), GME_EFCT_CMN_IDX_BOMB_BIG);
			bomb_obj->pos.z	= bomb_obj->parent_obj->pos.z + GMD_BOSS4_EFF_BOMB_OFST_Z;

			// 依存タイプに変更し、処理をBOSS4用に変更
			GmBoss4EffChangeType( (GMS_EFFECT_3DES_WORK*)bomb_obj, 
									GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND, GMD_EFFECT_3DES_FLAG_NOFLIP );
			bomb_obj->spd.x -= FX_F32_TO_FX32( 1.0f );
		}
		// 逃亡アクションに変化
		gmBoss4BodySetActionWhole(body_work, GME_BOSS4_ACT_ID_ESCAPE);

		// 指定場所にワープ
		GmBoss4ScrollOut();
		
		// 待機時間設定
		body_work->wait_timer	= 40;//仮
		
		// 処理関数設定
		body_work->proc_update	= gmBoss4BodyStateUpdateDefeatWithScatter;
	}
}

// 撃破ステート更新 パーツ飛散処理
void gmBoss4BodyStateUpdateDefeatWithScatter(GMS_BOSS4_BODY_WORK *body_work)
{
	// 画面フラッシュ更新
	GmBsCmnUpdateFlashScreen( &body_work->flash_work );


	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		
		// 黒こげテクスチャに変更
		gmBoss4SetPartTextureBurnt(GMM_BS_OBJ(body_work));
		// エッグマンに黒こげ通知
		body_work->flag	|= GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_BURNT;
		
		// アフターバーナー煙生成
		gmBoss4EffABSmokeInit(body_work);
		// 本体煙生成
		gmBoss4EffBodySmokeInit(body_work);
		
		body_work->proc_update	= gmBoss4BodyStateUpdateDefeatWithWaitEnd;
	}
}

// 撃破ステート更新 爆発後待機処理
void gmBoss4BodyStateUpdateDefeatWithWaitEnd(GMS_BOSS4_BODY_WORK *body_work)
{
	// 既定時間待機
	if (body_work->wait_timer > 0) {
		body_work->wait_timer--;
	}
	else {
		gmBoss4BodyChangeState(body_work, GME_BOSS4_BODY_STATE_ESCAPE);
	}
}


// =======================================================================
// gmBoss4BodyState{Enter|Leave|Update}EscapeXXX
/*!
  本体 逃亡ステート遷移時処理関数
 */
// =======================================================================
// 逃亡ステート開始関数
void gmBoss4BodyStateEnterEscape(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 矩形当たりオフ
	obj_work->flag	|= OBD_OBJECT_NOHIT;
	
#if 1	// TODO : 仮
	// アニメーション再開
	obj_work->disp_flag	&= ~OBD_DISP_STOP;
#endif
	
	// アクション設定
	gmBoss4BodySetActionWhole(body_work, GME_BOSS4_ACT_ID_ESCAPE);
	
	// エッグマンに逃亡を通知
	body_work->flag	|= GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_ESCAPE;
	
	// 振り返り初期化
	{
		BOOL	is_positive;
		if (GmBoss4UtilIsDirectionPositiveFromCurrent(&body_work->dir, GMD_BOSS4_RIGHTWARD_ANGLE)) {
			is_positive	= TRUE;
		}
		else {
			is_positive	= FALSE;
		}
		
		GmBoss4UtilInitTurnGently(&body_work->dir, GMD_BOSS4_RIGHTWARD_ANGLE,
								  90,//仮
								  is_positive);//仮
	}

	
	// 処理関数設定
	body_work->proc_update	= gmBoss4BodyStateUpdateEscapeWithTurn;
}

// 逃亡ステート終了関数
void gmBoss4BodyStateLeaveEscape(GMS_BOSS4_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
}

// 逃亡ステート更新 振り返り
void gmBoss4BodyStateUpdateEscapeWithTurn(GMS_BOSS4_BODY_WORK *body_work)
{
	if (GmBoss4UtilUpdateTurnGently(&body_work->dir)) {
		// 加速移動初期化
		gmBoss4BodyInitEscapeMove(body_work);
		
		// 処理関数設定
		body_work->proc_update	= gmBoss4BodyStateUpdateEscapeWithMoveLocked;
	}
}

// 逃亡ステート（スクロールロック中）更新 
void gmBoss4BodyStateUpdateEscapeWithMoveLocked(GMS_BOSS4_BODY_WORK *body_work)
{
	// 移動更新
	gmBoss4BodyUpdateEscapeMove(body_work);


	// スクロールロック解除
//	GmGmkCamScrLimitRelease(GMD_GMK_SCR_LMT_RELEASE_RIGHT|GMD_GMK_SCR_LMT_RELEASE_LEFT);
//	GmGmkCamScrLimitRelease(GMD_GMK_SCR_LMT_RELEASE_RIGHT|GMD_GMK_SCR_LMT_RELEASE_LEFT|GMD_GMK_SCR_LMT_RELEASE_BOTTOM|GMD_GMK_SCR_LMT_RELEASE_TOP);

	// スクロール解除判定
//	if (gmBoss4BodyIsEscapeScrUnlock(body_work)) {
		// スクロールロック解除
//		GmGmkCamScrLimitRelease(GMD_GMK_SCR_LMT_RELEASE_RIGHT);
		
		// 処理関数設定
		body_work->proc_update	= gmBoss4BodyStateUpdateEscapeWithMoveUnlocked;
//	}
}

// 逃亡ステート（スクロールロック解除後）更新
void gmBoss4BodyStateUpdateEscapeWithMoveUnlocked(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK*	obj_work = (OBS_OBJECT_WORK*)body_work;

	// ねじを落とす
	// マップ右から完全に消えたかチェック
	Float right = (Float)g_gm_main_system.map_fcol.right;

	gmBoss4BodyUpdateEscapeMove(body_work);

	if (obj_work->pos.x > FX_F32_TO_FX32(right-100.0f) ) 
	{
		GMS_EFFECT_3DES_WORK*	efct_work	= GmEfctBossCmnEsCreate(obj_work, GME_EFCT_BOSS_CMN_IDX_JET_B);

		GmBoss4EffChangeType( (GMS_EFFECT_3DES_WORK*)efct_work, 
								GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND, GMD_EFFECT_3DES_FLAG_NOFLIP|GMD_EFFECT_3DES_FLAG_STICKPARENT );

		body_work->proc_update	= gmBoss4BodyStateUpdateEscapeWithMoveFinish;
		
#if _IPHONE
		GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_HORI);
#endif // _IPHONE
		
	}
}

// 逃亡ステート（スクロールロック解除後）更新
void gmBoss4BodyStateUpdateEscapeWithMoveFinish(GMS_BOSS4_BODY_WORK *body_work)
{
	// 画面外移動待ち
	if (gmBoss4BodyUpdateEscapeMove(body_work)) {
		// 消去
		GMM_BOSS4_MGR(body_work)->flag	|= GMD_BOSS4_MGR_FLAG_CLEAR_BOSS;
		
		body_work->proc_update	= NULL;

		// 強制スクロールを次の段階に移す
		GmBoss4ScrollNext();
	}
}







// ############################################################################
// アフターバーナーエフェクト
// ############################################################################
// =======================================================================
// gmBoss4EffAfterburnerSetEnable
/*!
  アフターバーナーエフェクト 初期化
  
  @param body_work	[io]	本体ワーク
  @param is_enable	[in]	有効フラグ（TRUE:有効化, FALSE:無効化）
  
  @note
  TRUEを指定するとアフターバーナーエフェクトの生成をトリガーします。
  FALSEを指定するとすでに生成されているアフターバーナーエフェクトが消去されるように
  フラグ設定が行われます。
 */
// =======================================================================
void gmBoss4EffAfterburnerSetEnable(GMS_BOSS4_BODY_WORK *body_work, GME_BOSS4_BODY_ABURNER_TYPE type)
{
	switch( type ){
	default:
	case GME_BOSS4_BODY_ABURNER_TYPE_NONE:
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE;
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER;
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_EX;
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER_EX;
		break;

	case GME_BOSS4_BODY_ABURNER_TYPE_NORMAL:
		MTM_ASSERT(!(body_work->flag & GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE));
		MTM_ASSERT(!(body_work->flag & GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER));

		// 念のため第２段階をOFFにする
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_EX;
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER_EX;

		body_work->flag	|= GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER;
		break;
	case GME_BOSS4_BODY_ABURNER_TYPE_EX:
		MTM_ASSERT(!(body_work->flag & GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_EX));
		MTM_ASSERT(!(body_work->flag & GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER_EX));

		// 第一段階はOFFにする
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE;
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER;
		
		body_work->flag	|= GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER_EX;

		// 下のバーニアをOFF
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN;

		break;
	}
}

// =======================================================================
// gmBoss4EffAfterburnerUpdateCreate
/*!
  アフターバーナーエフェクト生成処理更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  フラグ監視して、必要ならばエフェクトの生成を行います。
  毎フレーム呼び出してください。
 */
// =======================================================================
void gmBoss4EffAfterburnerUpdateCreate(GMS_BOSS4_BODY_WORK *body_work)
{
	// 第２段階目(第一段階目よりも優先が高い)
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER_EX) {
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER;
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER_EX;
		body_work->flag	|= GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_EX;

		gmBoss4EffAfterburnerExInit(body_work);
	}

	// 通常
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER) {
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER;
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER_EX;
		body_work->flag	|= GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE;

		gmBoss4EffAfterburnerInit(body_work);
	}

}

// =======================================================================
// gmBoss4EffAfterburnerInit
/*!
  アフターバーナーエフェクト初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  アフターバーナーエフェクトを生成します。
  直接呼ばないでください。
 */
// =======================================================================
void gmBoss4EffAfterburnerInit(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*parent_obj	= GMM_BS_OBJ(body_work);
	GMS_EFFECT_3DES_WORK	*efct_work;

	// 一つ目のバーナー
	efct_work	= GmEfctBossCmnEsCreate(parent_obj, GME_EFCT_BOSS_CMN_IDX_JET_B);
	// 表示オフセット調整
	GmEffect3DESAddDispOffset(efct_work,	GMD_BOSS4_EFF_ABURNER1_DISP_OFST_X,
											GMD_BOSS4_EFF_ABURNER1_DISP_OFST_Y,
											GMD_BOSS4_EFF_ABURNER1_DISP_OFST_Z);
	
	GMM_BS_OBJ(efct_work)->ppFunc	= gmBoss4EffAfterburnerProcMain;

	// エフェクトサイズを変えても内部のスケールの掛け合わせがうまくいっていない場所があるため、
	// 完全にスケールを使用するのは難しい。
	//efct_work->efct_com.obj_work.scale.x = FX32_ONE * 0.5f;
	//efct_work->efct_com.obj_work.scale.y = FX32_ONE * 0.5f;
	//efct_work->efct_com.obj_work.scale.z = FX32_ONE * 0.5f;

	// 二つ目のバーナー
	efct_work	= GmEfctBossCmnEsCreate(parent_obj, GME_EFCT_BOSS_CMN_IDX_JET_B);	
	// 表示オフセット調整
	GmEffect3DESAddDispOffset(efct_work,	GMD_BOSS4_EFF_ABURNER2_DISP_OFST_X,
											GMD_BOSS4_EFF_ABURNER2_DISP_OFST_Y,
											GMD_BOSS4_EFF_ABURNER2_DISP_OFST_Z);
	
	GMM_BS_OBJ(efct_work)->ppFunc	= gmBoss4EffAfterburnerProcMain;

}

// =======================================================================
// gmBoss4EffAfterburnerProcMain
/*!
  アフターバーナーエフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  アフターバーナー有効フラグがオフになったら自身の消去処理を行います。
 */
// =======================================================================
void gmBoss4EffAfterburnerProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_BODY_WORK	*parent_body	= (GMS_BOSS4_BODY_WORK*)obj_work->parent_obj;
	
	MTM_ASSERT(parent_body->node_work.snm_work.reg_node_max);
	
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
	
	// アフターバーナー有効フラグが消えたらkill
	if (!(parent_body->flag & GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE)) {
		// これを行うと内部的に obj_work->disp_flag | OBD_DISP_END される
		ObjDrawKillAction3DES(obj_work);
	}
	
	// 本体SNMマトリクスでくっつける
	GmBsCmnUpdateObject3DESStuckWithNode(obj_work,
									 &parent_body->node_work.snm_work,
									 parent_body->node_work.work[ GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE ],
									 TRUE);
}




// =======================================================================
// gmBoss4EffAfterburnerExInit
/*!
  アフターバーナー(第２段階用)エフェクト初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  アフターバーナーエフェクトを生成します。
  直接呼ばないでください。
 */
// =======================================================================
void gmBoss4EffAfterburnerExInit(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*parent_obj	= GMM_BS_OBJ(body_work);
	GMS_EFFECT_3DES_WORK	*efct_work;

	// 一つ目
	//efct_work	= GmEfctBossCmnEsCreate(parent_obj, GME_EFCT_BOSS_CMN_IDX_JET_B);
	efct_work	= GmBoss4EffCommonInit( GMD_DWORK_NO_BOSS_04_EF_BOOSTX_ES, NULL, parent_obj );
	GMM_BS_OBJ(efct_work)->ppFunc	= NULL;

	GmEffect3DESSetupBase( efct_work, GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND/*GME_EFFECT_3DES_POS_TYPE_EMT*/,
						  /*GMD_EFFECT_3DES_FLAG_NOFLIP |*/ GMD_EFFECT_3DES_FLAG_STICKPARENT);

	// 表示角度設定
	GmEffect3DESSetDispRotation( efct_work, GMD_BOSS4_EFF_ABURNER3_DISP_ROT_X, 0, 0);


	// 表示オフセット調整
	GmEffect3DESAddDispOffset(efct_work,	GMD_BOSS4_EFF_ABURNER3_DISP_OFST_X,
											GMD_BOSS4_EFF_ABURNER3_DISP_OFST_Y,
											GMD_BOSS4_EFF_ABURNER3_DISP_OFST_Z);

	GMM_BS_OBJ(efct_work)->ppFunc	= gmBoss4EffAfterburnerExProcMainL;


	// 二つ目
	efct_work	= GmBoss4EffCommonInit( GMD_DWORK_NO_BOSS_04_EF_BOOSTX_ES, NULL, parent_obj );
	GMM_BS_OBJ(efct_work)->ppFunc	= NULL;


	GmEffect3DESSetupBase( efct_work, GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND/*GME_EFFECT_3DES_POS_TYPE_EMT*/,
						  /*GMD_EFFECT_3DES_FLAG_NOFLIP |*/ GMD_EFFECT_3DES_FLAG_STICKPARENT);

	// 表示角度設定
	GmEffect3DESSetDispRotation( efct_work, GMD_BOSS4_EFF_ABURNER3_DISP_ROT_X, 0, 0);


	// 表示オフセット調整
	GmEffect3DESAddDispOffset(efct_work,	GMD_BOSS4_EFF_ABURNER4_DISP_OFST_X,
											GMD_BOSS4_EFF_ABURNER4_DISP_OFST_Y,
											GMD_BOSS4_EFF_ABURNER4_DISP_OFST_Z);
	
	GMM_BS_OBJ(efct_work)->ppFunc	= gmBoss4EffAfterburnerExProcMainR;

}

// =======================================================================
// gmBoss4EffAfterburnerExProcMain
/*!
  アフターバーナーエフェクト(第２段階用) メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  アフターバーナー有効フラグがオフになったら自身の消去処理を行います。
 */
// =======================================================================
void gmBoss4EffAfterburnerExProcMainL(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_BODY_WORK	*parent_body	= (GMS_BOSS4_BODY_WORK*)obj_work->parent_obj;
	
	MTM_ASSERT(parent_body->node_work.snm_work.reg_node_max);
	
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
	
	// アフターバーナー有効フラグが消えたらkill
	if (!(parent_body->flag & GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_EX)) {
		// これを行うと内部的に obj_work->disp_flag | OBD_DISP_END される
		ObjDrawKillAction3DES(obj_work);
	}

#if 1
	// エッグマン設置ノードにくっつける(ワープ対策つき)
	const NNS_MATRIX* mtx1 = GmBoss4UtilGetNodeMatrix( &parent_body->node_work, GMD_BOSS4_BODY_NODE_IDX_BODY_BOOSTER_L );	// エッグマン設置場所
	const NNS_MATRIX* mtx2 = GmBoss4UtilGetNodeMatrix( &parent_body->node_work, GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE );	// 中心

	// 姿勢はmtx1を使ったまま、位置の差分をもらい、現在のポジションを足しこむようにする。

	NNS_MATRIX	mtx;
	nnCopyMatrix( &mtx, mtx1 );

	NNM_MTX( mtx, 0, 3 ) = NNM_MTX( *mtx1, 0, 3 ) - NNM_MTX( *mtx2, 0, 3 ) + (Float)parent_body->ene_3d.ene_com.obj_work.pos.x / FX32_ONE;
//	NNM_MTX( *mtx1, 1, 3 ) = NNM_MTX( *mtx1, 1, 3 ) - NNM_MTX( *mtx2, 1, 3 ) + (Float)body_obj->pos.y / FX32_ONE;
//	NNM_MTX( *mtx1, 2, 3 ) = NNM_MTX( *mtx1, 2, 3 ) - NNM_MTX( *mtx2, 2, 3 ) + (Float)body_obj->pos.z / FX32_ONE;


	GmBoss4UtilSetMatrixES( obj_work, &mtx );

#else
	// 本体SNMマトリクスでくっつける
	GmBsCmnUpdateObject3DESStuckWithNode(obj_work,
									 &parent_body->node_work.snm_work,
									 parent_body->node_work.work[GMD_BOSS4_BODY_NODE_IDX_BODY_BOOSTER_L],
									 TRUE);
#endif

	// ポーズ中はUPDATEしないようにする
	obj_work->disp_flag &= ~OBD_DISP_NOUPDATE;
	if (g_obj.flag & OBD_OBJ_PAUSE){
		obj_work->disp_flag |= OBD_DISP_NOUPDATE;
	}
}

void gmBoss4EffAfterburnerExProcMainR(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_BODY_WORK	*parent_body	= (GMS_BOSS4_BODY_WORK*)obj_work->parent_obj;
	
	MTM_ASSERT(parent_body->node_work.snm_work.reg_node_max);
	
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
	
	// アフターバーナー有効フラグが消えたらkill
	if (!(parent_body->flag & GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_EX)) {
		// これを行うと内部的に obj_work->disp_flag | OBD_DISP_END される
		ObjDrawKillAction3DES(obj_work);
	}

#if 1
	// エッグマン設置ノードにくっつける(ワープ対策つき)
	const NNS_MATRIX* mtx1 = GmBoss4UtilGetNodeMatrix( &parent_body->node_work, GMD_BOSS4_BODY_NODE_IDX_BODY_BOOSTER_R );	// エッグマン設置場所
	const NNS_MATRIX* mtx2 = GmBoss4UtilGetNodeMatrix( &parent_body->node_work, GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE );	// 中心

	// 姿勢はmtx1を使ったまま、位置の差分をもらい、現在のポジションを足しこむようにする。
	NNS_MATRIX	mtx;
	nnCopyMatrix( &mtx, mtx1 );

	NNM_MTX( mtx, 0, 3 ) = NNM_MTX( *mtx1, 0, 3 ) - NNM_MTX( *mtx2, 0, 3 ) + (Float)parent_body->ene_3d.ene_com.obj_work.pos.x / FX32_ONE;
//	NNM_MTX( *mtx1, 1, 3 ) = NNM_MTX( *mtx1, 1, 3 ) - NNM_MTX( *mtx2, 1, 3 ) + (Float)body_obj->pos.y / FX32_ONE;
//	NNM_MTX( *mtx1, 2, 3 ) = NNM_MTX( *mtx1, 2, 3 ) - NNM_MTX( *mtx2, 2, 3 ) + (Float)body_obj->pos.z / FX32_ONE;

	GmBoss4UtilSetMatrixES( obj_work, &mtx );
#else

	// 本体SNMマトリクスでくっつける
	GmBsCmnUpdateObject3DESStuckWithNode(obj_work,
									 &parent_body->node_work.snm_work,
									 parent_body->node_work.work[GMD_BOSS4_BODY_NODE_IDX_BODY_BOOSTER_R],
									 TRUE);
#endif

	// ポーズ中はUPDATEしないようにする
	obj_work->disp_flag &= ~OBD_DISP_NOUPDATE;
	if (g_obj.flag & OBD_OBJ_PAUSE){
		obj_work->disp_flag |= OBD_DISP_NOUPDATE;
	}

}


// =======================================================================
// gmBoss4EffABSmokeInit
/*!
  アフターバーナー煙エフェクト 初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss4EffABSmokeInit(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*parent_obj	= GMM_BS_OBJ(body_work);
	GMS_EFFECT_3DES_WORK	*efct_work;
	efct_work	= GmEfctBossCmnEsCreate(parent_obj, GME_EFCT_BOSS_CMN_IDX_JET_B_SMORK);

	// エフェクトタイプをBOSS4用に変更
	GmBoss4EffChangeType( efct_work, 
							GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND, 
							(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR) );

	// 表示オフセット調整
	GmEffect3DESAddDispOffset(efct_work, 0, 0, GMD_BOSS4_EFF_ABSMOKE_DISP_OFST_Z);
	
	GMM_BS_OBJ(efct_work)->ppFunc	= gmBoss4EffABSmokeProcMain;
}

// =======================================================================
// gmBoss4EffABSmokeProcMain
/*!
  アフターバーナー煙エフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss4EffABSmokeProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_BODY_WORK	*parent_body	= (GMS_BOSS4_BODY_WORK*)obj_work->parent_obj;
	
	MTM_ASSERT(parent_body->node_work.snm_work.reg_node_max);

	// ポーズ中はUPDATEしないようにする
	obj_work->disp_flag &= ~OBD_DISP_NOUPDATE;
	if (g_obj.flag & OBD_OBJ_PAUSE){
		obj_work->disp_flag |= OBD_DISP_NOUPDATE;
	}else{
		obj_work->pos.x += GmBoss4GetScrollOffset();
	}

	// SNMから取得したやつを設定（アフターバーナーと同じ座標）
	GmBsCmnUpdateObject3DESStuckWithNode(obj_work,
									 &parent_body->node_work.snm_work,
									 parent_body->node_work.work[ GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE ],
									 TRUE);
}

// =======================================================================
// gmBoss4EffBodySmokeInit
/*!
  本体煙エフェクト初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss4EffBodySmokeInit(GMS_BOSS4_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*parent_obj	= GMM_BS_OBJ(body_work);
	GMS_EFFECT_3DES_WORK	*efct_work;

	// 本体の煙
	efct_work	= GmEfctBossCmnEsCreate(parent_obj, GME_EFCT_BOSS_CMN_IDX_BOSS_SMORK);

	// エフェクトタイプをBOSS4用に変更
	GmBoss4EffChangeType( efct_work, 
							GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND, 
							(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR) );

	// 表示オフセット調整
	GmEffect3DESAddDispOffset(efct_work, 0, 0, GMD_BOSS4_EFF_BODYSMOKE_DISP_OFST_Z);
	GMM_BS_OBJ(efct_work)->ppFunc	= gmBoss4EffBodySmokeProcMain;


	const Float _pt[4][3]={
		{-18*2,0,-3*2},
		{-10*2,3*2,8*2},
		{0,4*2,-12*2},
		{18*2,0,0},
	};

	for(int i=0;i<4;i++){
		// 本体の煙02
		efct_work	= GmEfctBossCmnEsCreate(parent_obj, GME_EFCT_BOSS_CMN_IDX_BOSS_SMOKE02);

		// エフェクトタイプをBOSS4用に変更
		GmBoss4EffChangeType( efct_work, 
								GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND, 
								(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR) );

		// 表示オフセット調整
//		Sint32 z = (rand() % 60) -30;
//		Sint32 y = (rand() % 30)-20;
//		GmEffect3DESAddDispOffset(efct_work, -32.0f, (Float)y, (Float)z/*GMD_BOSS4_EFF_BODYSMOKE_DISP_OFST_Z*/);
		GmEffect3DESAddDispOffset(efct_work, _pt[i][0], _pt[i][1], _pt[i][2]);
		GMM_BS_OBJ(efct_work)->ppFunc	= gmBoss4EffBodySmokeProcMain;
	}
}

// =======================================================================
// gmBoss4EffBodySmokeProcMain
/*!
  本体煙エフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss4EffBodySmokeProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_BODY_WORK	*parent_body	= (GMS_BOSS4_BODY_WORK*)obj_work->parent_obj;
	
	MTM_ASSERT(parent_body->node_work.snm_work.reg_node_max);
	
	// ポーズ中はUPDATEしないようにする
	obj_work->disp_flag &= ~OBD_DISP_NOUPDATE;
	if (g_obj.flag & OBD_OBJ_PAUSE){
		obj_work->disp_flag |= OBD_DISP_NOUPDATE;
	}else{
		obj_work->pos.x += GmBoss4GetScrollOffset();
	}
	// SNMから取得したやつを設定（アフターバーナーと同じ座標）
	GmBsCmnUpdateObject3DESStuckWithNode(obj_work,
									 &parent_body->node_work.snm_work,
									 parent_body->node_work.work[ GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE ],
									 TRUE);
}


// =======================================================================
// gmBoss4EffBossLightUpdateCreate
/*!
  ボスライトエフェクト生成処理更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  フラグ監視して、必要ならばエフェクトの生成を行います。
  毎フレーム呼び出してください。
 */
// =======================================================================
void gmBoss4EffBossLightUpdateCreate(GMS_BOSS4_BODY_WORK *body_work)
{
	// ボスライト
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_BOSSLIGHT) {
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_BOSSLIGHT;

		// 張り付くオフセットと回転
		VecFx32	ofs = {
			FX_F32_TO_FX32( GMD_BOSS4_BODY_BOSSLIGHT_L_OFFSET_X ),
			FX_F32_TO_FX32( GMD_BOSS4_BODY_BOSSLIGHT_L_OFFSET_Y ),
			FX_F32_TO_FX32( GMD_BOSS4_BODY_BOSSLIGHT_L_OFFSET_Z ),
		};
		VecFx32	rot = {
			FX_F32_TO_FX32( GMD_BOSS4_BODY_BOSSLIGHT_L_ROTATE_X ),
			FX_F32_TO_FX32( GMD_BOSS4_BODY_BOSSLIGHT_L_ROTATE_Y ),
			FX_F32_TO_FX32( GMD_BOSS4_BODY_BOSSLIGHT_L_ROTATE_Z ),
		};

		GmBoss4EffCommonInit(	GMD_DWORK_NO_BOSS_04_EF_BOSS_LIGHT_ES,	// エフェクト種類
								//GMD_DWORK_NO_BOSS_04_EF_BOOSTX_ES,
								&ofs,									// オフセット位置
								(OBS_OBJECT_WORK*)body_work,			// 親
								GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,	// 親依存
								GMD_EFFECT_3DES_FLAG_STICKPARENT,		// 親に引っ付く
								&body_work->node_work,					// ノード情報
								GMD_BOSS4_BODY_NODE_IDX_BODY_LIGHT_L,	// ノード番号
								&rot,									// 回転
								&body_work->flag,						// 監視用フラグ
								GMD_BOSS4_BODY_FLAG_BOSSLIGHT_ACTIVE	// 監視パターン
								);

		// 張り付くオフセットと回転
		VecFx32	ofs2 = {
			FX_F32_TO_FX32( GMD_BOSS4_BODY_BOSSLIGHT_R_OFFSET_X ),
			FX_F32_TO_FX32( GMD_BOSS4_BODY_BOSSLIGHT_R_OFFSET_Y ),
			FX_F32_TO_FX32( GMD_BOSS4_BODY_BOSSLIGHT_R_OFFSET_Z ),
		};
		VecFx32	rot2 = {
			FX_F32_TO_FX32( GMD_BOSS4_BODY_BOSSLIGHT_R_ROTATE_X ),
			FX_F32_TO_FX32( GMD_BOSS4_BODY_BOSSLIGHT_R_ROTATE_Y ),
			FX_F32_TO_FX32( GMD_BOSS4_BODY_BOSSLIGHT_R_ROTATE_Z ),
		};

		GmBoss4EffCommonInit(	GMD_DWORK_NO_BOSS_04_EF_BOSS_LIGHT_ES,	// エフェクト種類
								//GMD_DWORK_NO_BOSS_04_EF_BOOSTX_ES,
								&ofs2,									// オフセット位置
								(OBS_OBJECT_WORK*)body_work,			// 親
								GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,	// 親依存
								GMD_EFFECT_3DES_FLAG_STICKPARENT,		// 親に引っ付く
								&body_work->node_work,					// ノード情報
								GMD_BOSS4_BODY_NODE_IDX_BODY_LIGHT_R,	// ノード番号
								&rot2,									// 回転
								&body_work->flag,						// 監視用フラグ
								GMD_BOSS4_BODY_FLAG_BOSSLIGHT_ACTIVE	// 監視パターン
								);

	}

}

void gmBoss4EffBossLightSetEnable(GMS_BOSS4_BODY_WORK *body_work, BOOL on )
{
	if (on){
		// すでに存在している
		if (body_work->flag & GMD_BOSS4_BODY_FLAG_BOSSLIGHT_ACTIVE)
			return;

		body_work->flag	|= GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_BOSSLIGHT;
	}else{
		body_work->flag &= ~GMD_BOSS4_BODY_FLAG_BOSSLIGHT_ACTIVE;
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_BOSSLIGHT;
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
