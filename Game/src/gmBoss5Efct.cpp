// =======================================================================
/*!
  @file	gmBoss5Efct.cpp
  @brief ボスファイナル エフェクト

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss5Efct.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akMath.h"
#include "gmMain.h"
#include "gmGameDat.h"
#include "gmSound.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmEneCom.h"
#include "gmBoss5.h"
#include "gmBoss5Turret.h"
#include "gmBoss5Rocket.h"
#include "gmBoss5Egg.h"
#include "gmBoss5Efct.h"

#include "gmEffectCmn.h"
#include "gmEffectBoss.h"

// データヘッダ
#include "../file/common/arc/BOSS05.hmb"

/*------ Macros --------------------------------------------------------*/
/* ファイル関連 */
#define GMD_BOSS5_EFCT_ARC	(g_gm_gamedat_enemy_arc)

//############ 共通 ###########################################################
/* 定義値 */

//############ 汎用エフェクトワーク ###########################################
/* フラグ */
#define GMD_BOSS5_EFCT_GENERAL_FLAG_FADING				(1 << 0)		//!< 消失中フラグ

//############ 漏電エフェクト #################################################
/* ユーザフラグ */
#define GMD_BOSS5_EFCT_LEAKAGE_USRFLAG_IS_BODYPART		(1 << 0)		//!< 胴体用のパーツ識別フラグ
#define GMD_BOSS5_EFCT_LEAKAGE_USRFLAG_SE_RESPONSIBLE	(1 << 1)		//!< SE管理担当フラグ（複数のエフェクトに立てないこと）
/* 定義値 */
#define GMD_BOSS5_EFCT_LEAKAGE_PART_NUM					(8)				//!< 漏電エフェクト構成パーツ数
#define GMD_BOSS5_EFCT_LEAKAGE_OFST_Z					((fx32)(FX32_ONE * 64))		//!< ボスより手前に表示させるための接着ノードからのオフセットZ
#define GMD_BOSS5_EFCT_LEAKAGE_BODYPART_OFST_Z			((fx32)(FX32_ONE * 80))		//!< （本体用パーツ用）ボスより手前に表示させるための接着ノードからのオフセットZ

//############ 漏電消失エフェクト #############################################
/* 定義値 */
#define GMD_BOSS5_EFCT_LEAKAGE_VANISH_PART_NUM			(2)
#define GMD_BOSS5_EFCT_LEAKAGE_VANISH_OFST_Z			((fx32)(FX32_ONE * 80))		//!< ボスより手前に表示させるための接着ノードからのオフセットZ

//############ 予備漏電エフェクト #############################################
#define GMD_BOSS5_EFCT_PRELIM_LEAKAGE_OFST_Z			((fx32)(FX32_ONE * 80))

//############ 歩き足接地時 煙エフェクト ######################################
/* 定義値 */
#define GMD_BOSS5_EFCT_WALK_STEP_SMOKE_OFST_Z			((fx32)(FX32_ONE * 16))		//!< 脚より手前に表示させるための接着ノードからのオフセットZ

//############ 走り足接地時 煙エフェクト ######################################
/* 定義値 */
#define GMD_BOSS5_EFCT_RUN_STEP_SMOKE_OFST_Z			((fx32)(FX32_ONE * 16))		//!< 脚より手前に表示させるための接着ノードからのオフセットZ

//############ 凶暴化演出時 踏み込み煙エフェクト ##############################
/* 定義値 */
#define GMD_BOSS5_EFCT_BERSERK_STAMP_SMOKE_OFST_Z		(GMD_BOSS5_EFCT_RUN_STEP_SMOKE_OFST_Z)	//!< 足より手前に表示させるための接着ノードからのオフセットZ

//############ 地球割り着地時 煙エフェクト ####################################
/* 定義値 */
#define GMD_BOSS5_EFCT_CRASH_LANDING_SMOKE_OFST_Z		((fx32)(FX32_ONE * 64))		//!< ボスより手前に表示させるための親からのオフセットZ

//############ 割れガラスエフェクト ###########################################
/* 定義値 */
#define GMD_BOSS5_EFCT_BREAKING_GLASS_OFST_Y			((fx32)(FX32_ONE * -32))	//!< 足場中心から少し上にずらす
#define GMD_BOSS5_EFCT_BREAKING_GLASS_OFST_Z			((fx32)(FX32_ONE * 64))		//!< 足場より手前に表示させるための親からのオフセットZ

//############ 噴射エフェクト #################################################
/* 定義値 */
#define GMD_BOSS5_EFCT_JET_VIEWOUT_OFST					((Sint16)16)	//!< 噴射エフェクトのクリッピングオフセット
#define GMD_BOSS5_EFCT_JET_ATK_ACTIVATE_TIMING_FRAME	(65)			//!< 攻撃矩形を有効にするタイミング
// 攻撃矩形サイズ
#define GMD_BOSS5_EFCT_JET_ATK_RECT_SIZE_LEFT			(-8)
#define GMD_BOSS5_EFCT_JET_ATK_RECT_SIZE_TOP			(0)
#define GMD_BOSS5_EFCT_JET_ATK_RECT_SIZE_RIGHT			(8)
#define GMD_BOSS5_EFCT_JET_ATK_RECT_SIZE_BOTTOM			(88)

//############ ロケット漏電エフェクト #########################################
/* 定義値 */
#define GMD_BOSS5_EFCT_ROCKET_LEAKAGE_OFST_Z			((fx32)(FX32_ONE * 64))		//!< ロケットより手前に表示させるための接着ノードからのオフセットZ
#define GMD_BOSS5_EFCT_ROCKET_LEAKAGE_STUCK_ND_OFST_X	(-7.f)			//!< ノード追随オフセットX

//############ ロケット発射エフェクト #########################################
/* 定義値 */
#define GMD_BOSS5_EFCT_ROCKET_LAUNCH_OFST_Z				((fx32)(FX32_ONE * 16))	//!< ロケットより手前に表示させるための接着ノードからのオフセットZ
#define GMD_BOSS5_EFCT_ROCKET_LAUNCH_STUCK_ND_OFST_X	(-7.f)			//!< ノード追随オフセットX

//############ ロケットドッキングエフェクト ###################################
/* 定義値 */
#define GMD_BOSS5_EFCT_ROCKET_DOCK_OFST_Z				((fx32)(FX32_ONE * 16))	//!< ロケットより手前に表示させるための接着ノードからのオフセットZ
#define GMD_BOSS5_EFCT_ROCKET_DOCK_STUCK_ND_OFST_X		(2.f)			//!< ノード追随オフセットX（左腕基準）

//############ ロケット噴射・逆噴射エフェクト #################################
/* ユーザフラグ*/
#define GMD_BOSS5_EFCT_ROCKET_JET_USRFLAG_IS_REVERSE	(1 << 0)		//!< 逆噴射タイプフラグ
/* 定義値 */
#define GMD_BOSS5_EFCT_ROCKET_JET_OFST_Z				((fx32)(FX32_ONE * 8))	//!< ロケットより手前に表示させるための接着ノードからのオフセットZ
#define GMD_BOSS5_EFCT_ROCKET_JET_STUCK_ND_OFST_X		(-6.f)			//!< ノード追随オフセットX
#define GMD_BOSS5_EFCT_ROCKET_JET_REV_STUCK_ND_OFST_X	(4.f)			//!< 逆噴射用ノード追随オフセットX

//############ ロケット着地衝撃波 #############################################
/* 定義値 */
#define GMD_BOSS5_EFCT_ROCKET_LANDING_SW_OFST_Z			((fx32)(FX32_ONE * 16))		//!< ロケットより手前に表示させるための親からのオフセットZ

//############ 着地衝撃波 #####################################################
/* 定義値 */
#define GMD_BOSS5_EFCT_LANDING_SW_OFST_Z				((fx32)(FX32_ONE * 64))		//!< ボスより手前に表示させるための親からのオフセットZ
#define GMD_BOSS5_EFCT_LANDING_SW_VIEWOUT_OFST			((Sint16)128)				//!< 着地衝撃波エフェクトのクリッピングオフセット
#define GMD_BOSS5_EFCT_LANDING_SW_ATK_RECT_ACTIVE_TIME	(8)							//!< 攻撃矩形有効時間
// 攻撃矩形サイズ
#define GMD_BOSS5_EFCT_LANDING_SW_ATK_RECT_SIZE_LEFT	(-88)
#define GMD_BOSS5_EFCT_LANDING_SW_ATK_RECT_SIZE_TOP		(-32)
#define GMD_BOSS5_EFCT_LANDING_SW_ATK_RECT_SIZE_RIGHT	(88)
#define GMD_BOSS5_EFCT_LANDING_SW_ATK_RECT_SIZE_BOTTOM	(32)

//############ 地面突き衝撃波 #################################################
/* 定義値 */
#define GMD_BOSS5_EFCT_STRIKE_SW_OFST_Z					((fx32)(FX32_ONE * 64))		//!< ボスより手前に表示させるための親からのオフセットZ
#define GMD_BOSS5_EFCT_STRIKE_SW_VIEWOUT_OFST			((Sint16)64)				//!< 地面突き衝撃波エフェクトのクリッピングオフセット
#define GMD_BOSS5_EFCT_STRIKE_SW_ATK_RECT_ACTIVE_TIME	(15)						//!< 攻撃矩形有効時間
// 攻撃矩形サイズ
#define GMD_BOSS5_EFCT_STRIKE_SW_ATK_RECT_SIZE_LEFT		(-64)
#define GMD_BOSS5_EFCT_STRIKE_SW_ATK_RECT_SIZE_TOP		(-64)
#define GMD_BOSS5_EFCT_STRIKE_SW_ATK_RECT_SIZE_RIGHT	(64)
#define GMD_BOSS5_EFCT_STRIKE_SW_ATK_RECT_SIZE_BOTTOM	(32)

//############ ターゲットカーソルエフェクト ###################################
/* 定義値 */
#define GMD_BOSS5_EFCT_TARGETCURSOR_DISP_OFST_Z			((Float)32.f)	//!< 表示オフセットZ（手前に表示）
#define GMD_BOSS5_EFCT_CURSOR_START_DURATION_TIME		(120)			//!< 開始エフェクト継続時間
#define GMD_BOSS5_EFCT_CURSOR_START_INIT_UPDATE_SPD		((Float)0.5f)	//!< 開始エフェクト 初期再生速度
#define GMD_BOSS5_EFCT_CURSOR_START_DEST_UPDATE_SPD		((Float)1.0f)	//!< 開始エフェクト 目標再生速度
#define GMD_BOSS5_EFCT_CURSOR_START_FLICKER_OFF_TIME	(10)			//!< 開始エフェクト 点滅 暗時間
#define GMD_BOSS5_EFCT_CURSOR_START_FLICKER_ON_TIME		(10)				//!< 開始エフェクト 点滅 明時間
#define GMD_BOSS5_EFCT_CURSOR_LOOP_INIT_UPDATE_SPD		((Float)0.8f)	//!< ループエフェクト 初期再生速度
#define GMD_BOSS5_EFCT_CURSOR_LOOP_UPDATE_SPD_ADD		((Float)0.005f)	//!< ループエフェクト 再生速度加速度
#define GMD_BOSS5_EFCT_CURSOR_LOOP_DEST_UPDATE_SPD		((Float)1.5f)	//!< ループエフェクト 目標再生速度
#define GMD_BOSS5_EFCT_CURSOR_LOOP_FLICKER_OFF_TIME		(10)			//!< ループエフェクト 点滅 暗時間
#define GMD_BOSS5_EFCT_CURSOR_LOOP_FLICKER_ON_TIME		(10)			//!< ループエフェクト 点滅 明時間

//############ 地球割り用ターゲットカーソルエフェクト #########################
/* 定義値 */
#define GMD_BOSS5_EFCT_CRASH_CURSOR_OFST_Y_FROM_GROUND	((fx32)(FX32_ONE * -32))	//!< 地球割りカーソルエフェクト 生成位置 地面からのオフセットY
#define GMD_BOSS5_EFCT_CRASH_CURSOR_FLICKER_OFF_TIME	(10)	//!< 地球割りカーソルエフェクト 点滅 暗時間
#define GMD_BOSS5_EFCT_CRASH_CURSOR_FLICKER_ON_TIME		(10)	//!< 地球割りカーソルエフェクト 点滅 明時間

#define GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_RATIO_CURVE_RANGE_FACTOR	((Float)0.9f)

#define GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_LEFT_EDGE_OFST		((fx32)(FX32_ONE * -256))	//!< カーソル移動範囲左端の、エリア中央からのオフセットX
#define GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_RIGHT_EDGE_OFST	((fx32)(FX32_ONE * 256))	//!< カーソル移動範囲右端の、エリア中央からのオフセットX
#define GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_WIDTH				((fx32)(GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_RIGHT_EDGE_OFST \
																	- GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_LEFT_EDGE_OFST))	//!< カーソル移動範囲の幅
#define GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_DISTANCE			((fx32)(GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_WIDTH * 6.5f/*2.5f*/))		//!< カーソルの移動距離

//############ バルカン弾エフェクト ###########################################
/* 定義値 */
#define GMD_BOSS5_EFCT_VULCAN_BULLET_VIEWOUT_OFST		(16)		//!< バルカン弾 クリッピングオフセット
#define GMD_BOSS5_EFCT_VULCAN_BULLET_OFST_Z				((fx32)(FX32_ONE * 64))		//!< ボスより手前に表示させるためのオフセットZ
//! バルカン弾 攻撃矩形サイズ
#define GMD_BOSS5_EFCT_VULCAN_BULLET_ATK_RECT_SIZE_LEFT		(-8)
#define GMD_BOSS5_EFCT_VULCAN_BULLET_ATK_RECT_SIZE_TOP		(-8)
#define GMD_BOSS5_EFCT_VULCAN_BULLET_ATK_RECT_SIZE_RIGHT	(8)
#define GMD_BOSS5_EFCT_VULCAN_BULLET_ATK_RECT_SIZE_BOTTOM	(8)

//############ ダメージエフェクト #############################################
/* 定義値 */
#define GMD_BOSS5_EFCT_DAMAGE_OFST_Z		((fx32)(FX32_ONE * 64))	//!< ボスより手前に表示させるための親からのオフセットZ

//############ 機能停止黒煙エフェクト #########################################
/* 定義値 */
#define GMD_BOSS5_EFCT_BREAKDOWN_SMOKES_NUM				(2)		//!< 機能停止黒煙構成パーツ数

//############ 本体 小さい黒煙エフェクト ######################################
/* 定義値 */
#define GMD_BOSS5_EFCT_BODY_SMALL_SMOKES_NUM			(3)		//!< 本体小さい黒煙構成パーツ数
#define GMD_BOSS5_EFCT_BODY_SMALL_SMOKE_LOOP_TIME_MAX	(240)	//!< 本体小さい黒煙ループ最長時間
#define GMD_BOSS5_EFCT_BODY_SMALL_SMOKE_LOOP_TIME_MIN	(120)	//!< 本体小さい黒煙ループ最短時間
#define GMD_BOSS5_EFCT_BODY_SMALL_SMOKE_IDLE_TIME_MAX	(120)	//!< 本体小さい黒煙停滞最長時間
#define GMD_BOSS5_EFCT_BODY_SMALL_SMOKE_IDLE_TIME_MIN	(60)	//!< 本体小さい黒煙停滞最短時間

//############ 凶暴化スチームエフェクト #######################################
/* 定義値 */
#define GMD_BOSS5_EFCT_BERSERK_STEAM_NUM				(2)		//!< 凶暴化スチーム構成パーツ数
#define GMD_BOSS5_EFCT_BERSERK_STEAM_BLAST_TIME			(6)	//!< 凶暴化スチーム噴出ループ時間
#define GMD_BOSS5_EFCT_BERSERK_STEAM_IDLE_TIME			(0)	//!< 凶暴化スチーム停滞時間

//############ エッグマン汗エフェクト #########################################
#define GMD_BOSS5_EFCT_EGG_SWEAT_DISP_OFST_Y			((Float)56.f)	//!< 表示オフセットY
#define GMD_BOSS5_EFCT_EGG_SWEAT_DISP_OFST_Z			((Float)-8.f)	//!< 表示オフセットZ（後ろ気味にする）

//############ ロケット回転火花エフェクト #####################################
#define GMD_BOSS5_EFCT_ROCKET_ROLLING_SPARK_DIR_Z		((Uint16)AKM_DEGtoA16(180))
#define GMD_BOSS5_EFCT_ROCKET_ROLLING_SPARK_OFST_Z		((fx32)(FX32_ONE * 32))

//############ SE関連 #########################################################
/* 噴射SE */
#define GMD_BOSS5_EFCT_SE_JET_START_WAIT_TIME			(50)	//!< 噴射SE再生開始遅延時間

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! ボス5エフェクトインデックス
typedef enum
{
	GME_BOSS5_EFCT_IDX_BLITZ_FB_00	= 0,
	GME_BOSS5_EFCT_IDX_BLITZ_FB_01,
	GME_BOSS5_EFCT_IDX_BLITZ_FB_02,
	GME_BOSS5_EFCT_IDX_BLITZ_FB_03,
	GME_BOSS5_EFCT_IDX_BLITZ_FB_C_00,
	GME_BOSS5_EFCT_IDX_BLITZ_FB_C_01,
	GME_BOSS5_EFCT_IDX_BLITZ_FB_C_02,
	GME_BOSS5_EFCT_IDX_BLITZ_FB_START,
	GME_BOSS5_EFCT_IDX_FB_SMORK00,
	GME_BOSS5_EFCT_IDX_FB_SMORK01,
	GME_BOSS5_EFCT_IDX_FB_SMORK02,
	GME_BOSS5_EFCT_IDX_GLASS,
	GME_BOSS5_EFCT_IDX_JET_FB,
	GME_BOSS5_EFCT_IDX_JET_FB_SMORK,
	GME_BOSS5_EFCT_IDX_ROCKET_BLITZ,
	GME_BOSS5_EFCT_IDX_ROCKET_E,
	GME_BOSS5_EFCT_IDX_ROCKET_JET,
	GME_BOSS5_EFCT_IDX_ROCKET_S,
	GME_BOSS5_EFCT_IDX_ROCKET_SMORK,
	GME_BOSS5_EFCT_IDX_SHOCK,
	GME_BOSS5_EFCT_IDX_SHOCK_ATK,
	GME_BOSS5_EFCT_IDX_TARGET_FB,
	GME_BOSS5_EFCT_IDX_TARGET_FB_E,
	GME_BOSS5_EFCT_IDX_TARGET_FB_S,
	GME_BOSS5_EFCT_IDX_TARGET_FB_W,
	GME_BOSS5_EFCT_IDX_TARGET_FB_W_E,
	GME_BOSS5_EFCT_IDX_TARGET_FB_W_S,
	
	GME_BOSS5_EFCT_IDX_MAX
} GME_BOSS5_EFCT_IDX;

//! 汎用ワーク
typedef struct tag_GMS_BOSS5_EFCT_GENERAL_WORK
{
	GMS_EFFECT_3DES_WORK	efct_3des;
	NNS_MATRIX				ofst_mtx;			//!< ノード追随用オフセットマトリクス
	Uint32					flag;
	Uint32					user_flag;			//!< ユーザフラグ（各エフェクトで自由に使える）
	Uint32					user_work;			//!< ユーザワーク（各エフェクトで自由に使える）
	Sint32					ref_node_snm_id;	//!< 参照ノードのSNM登録ID（どのSNMワークかはコンテキスト依存）
	Uint32					timer;				//!< 汎用タイマ
	Float					ratio_timer;		//!< 汎用進捗タイマ
	GMS_BOSS5_1SHOT_TIMER	se_timer;			//!< SE用1ショットタイマ
	Uint32					se_cnt;				//!< SE再生カウント（何回も再生するときなどに利用）
	GSS_SND_SE_HANDLE		*se_handle;			//!< SEハンドル
} GMS_BOSS5_EFCT_GENERAL_WORK;

//! ボス5エフェクトデータ情報構造体
typedef struct tag_GMS_BOSS5_EFCT_DATA_INFO
{
	BOOL	use_model;			//!< モデル使用フラグ
	Sint32	ame_arc_idx;		//!< AMEのAMBアーカイブ内のインデックス
	Sint32	ame_dwork_no;		//!< AMEデータワーク番号
	Sint32	tex_amb_arc_idx;	//!< テクスチャAMBのAMBアーカイブ内のインデックス
	Sint32	tex_amb_dwork_no;	//!< テクスチャAMBのデータワーク番号
	Sint32	tex_list_dwork_no;	//!< テクスチャリストのデータワーク番号
	Sint32	model_arc_idx;		//!< モデルデータのAMBアーカイブ内のインデックス
	Sint32	model_dwork_no;		//!< モデルデータのデータワーク番号
	Sint32	object_dwork_no;	//!< NNオブジェクトのデータワーク番号
} GMS_BOSS5_EFCT_DATA_INFO;

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
static GMS_EFFECT_3DES_WORK* gmBoss5EfctEsCreate(OBS_OBJECT_WORK *parent_obj, Sint32 efct_idx,
												 Uint32 work_size=sizeof(GMS_EFFECT_3DES_WORK));
static void gmBoss5EfctCreateLeakage(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5EfctLeakagePartProcMain(OBS_OBJECT_WORK *obj_work);
#if _IPHONE
static void gmBoss5EfctLeakageProcFade(OBS_OBJECT_WORK *obj_work);
#else
static void gmBoss5EfctCreateLeakageVanish(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5EfctLeakageVanishProcMain(OBS_OBJECT_WORK *obj_work);
#endif // _IPHONE
static void gmBoss5EfctCreatePrelimLeakage(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5EfctPrelimLeakageProcLoop(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctPrelimLeakageProcFade(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctBerserkStampSmokeProcWaitStart(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctCreateJet(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5EfctJetProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctCreateJetSmoke(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5EfctJetSmokeProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctCreateRocketLeakage(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5EfctRocketLeakageExit(MTS_TASK_TCB *tcb);
static void gmBoss5EfctRocketLeakageProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctRocketDockProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctCreateRocketJet(GMS_BOSS5_ROCKET_WORK *rkt_work, BOOL is_rev_jet);
static void gmBoss5EfctRocketJetProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctLandingShockwaveProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctStrikeShockwaveProcWaitStart(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctStrikeShockwaveProcLoop(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctCreateTargetCursorStart(GMS_BOSS5_BODY_WORK *body_work);
static void gmBoss5EfctTargetCursorStartProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctCreateTargetCursorLoop(GMS_BOSS5_BODY_WORK *body_work,
											  const GMS_EFFECT_COM_WORK *former_efct);
static void gmBoss5EfctTargetCursorLoopProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctCreateTargetCursorEnd(GMS_BOSS5_BODY_WORK *body_work,
											 const GMS_EFFECT_COM_WORK *former_efct);
static void gmBoss5EfctTargetCursorEndProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctCreateTargetCursorFlash(GMS_EFFECT_COM_WORK *parent_efct, GME_BOSS5_EFCT_IDX efct_idx);
static void gmBoss5EfctTargetCursorFlashProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctTargetCursorInitFlickerNoDisp(GMS_BOSS5_EFCT_GENERAL_WORK *targ_cursor);
static void gmBoss5EfctTargetCursorUpdateFlickerNoDisp(GMS_BOSS5_EFCT_GENERAL_WORK *targ_cursor,
													   Float nodisp_time, Float cycle_time);
static void gmBoss5EfctCreateCrashCursorStart(GMS_BOSS5_BODY_WORK *body_work,
											  fx32 pos_x, Uint32 duration_time);
static void gmBoss5EfctCrashCursorStartProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctCrashCursorStartSetCurPos(GMS_BOSS5_EFCT_GENERAL_WORK *ctarg_start, Float ratio, BOOL app_dir_left);
static void gmBoss5EfctCreateCrashCursorLoop(GMS_BOSS5_BODY_WORK *body_work, fx32 pos_x);
static void gmBoss5EfctCrashCursorLoopProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctCreateCrashCursorEnd(GMS_BOSS5_BODY_WORK *body_work,
											const GMS_EFFECT_COM_WORK *former_efct);
static void gmBoss5EfctCrashCursorEndProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctVulcanBulletProcMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctCreateRocketSmoke(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5EfctRocketSmokeProcLoop(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctRocketSmokeProcFade(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctBreakdownSmokeProcLoop(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctBreakdownSmokeProcFade(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctCreateBodySmallSmoke(GMS_BOSS5_BODY_WORK *body_work, Uint32 part_idx);
static void gmBoss5EfctBodySmallSmokeProcLoop(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctBodySmallSmokeProcFade(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctCreateBerserkSteam(GMS_BOSS5_BODY_WORK *body_work, Uint32 count, Uint32 part_idx);
static void gmBoss5EfctBerserkSteamProcLoop(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctBerserkSteamProcFade(OBS_OBJECT_WORK *obj_work);
static void gmBoss5EfctCreateEggSweat(GMS_BOSS5_EGG_WORK *egg_work);
static void gmBoss5EfctEggSweatProcLoop(OBS_OBJECT_WORK *obj_work);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/

// エフェクトテーブル
#include "gmBoss5EfctTbl.inc"

/*------ Global Functions ----------------------------------------------*/

// =======================================================================
// GmBoss5EfctBuild
/*!
  ボス5 エフェクト構築
  
  @note
  GmBoss5Build()から呼び出してください。
 */
// =======================================================================
void GmBoss5EfctBuild(void)
{
	for (Sint32 i = 0; i < GME_BOSS5_EFCT_IDX_MAX; ++i) {
		const GMS_BOSS5_EFCT_DATA_INFO *data_info	= &gm_boss5_efct_data_info_tbl[i];
		OBS_DATA_WORK	*model_dwork	= NULL;
		OBS_DATA_WORK	*object_dwork	= NULL;
		
		// モデルを使用する場合は参照格納先のデータワークを取得
		if (data_info->use_model) {
			model_dwork	= ObjDataGet(data_info->model_dwork_no);
			object_dwork	= ObjDataGet(data_info->object_dwork_no);
		}
		
		// エフェクトのテクスチャ・モデルをVRAMにロード
		GmEfctBossBuildSingleDataReg(data_info->tex_amb_arc_idx,
									 ObjDataGet(data_info->tex_amb_dwork_no),
									 ObjDataGet(data_info->tex_list_dwork_no),
									 data_info->model_arc_idx,
									 model_dwork,
									 object_dwork,
									 GMD_BOSS5_EFCT_ARC);
		
	}
}

// =======================================================================
// GmBoss5EfctFlush
/*!
  ボス5 エフェクト片付け
  
  @note
  GmBoss5Flush()から呼び出してください。
 */
// =======================================================================
void GmBoss5EfctFlush(void)
{
	// エフェクト解放
	GmEfctBossFlushSingleDataInit();
}

// =======================================================================
// GmBoss5EfctTryStartLeakage
/*!
  漏電エフェクト開始
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void GmBoss5EfctTryStartLeakage(GMS_BOSS5_BODY_WORK *body_work)
{
	MTM_ASSERT(body_work);

	if (!(body_work->flag & GMD_BOSS5_BODY_FLAG_LEAKAGE_ACTIVE)) {
//#if !_IPHONE
		// 漏電SE再生開始
		GmSoundPlaySE("FinalBoss11", body_work->se_hnd_leakage);
//#endif // !_IPHONE
		
		gmBoss5EfctCreateLeakage(body_work);
		body_work->flag	|= GMD_BOSS5_BODY_FLAG_LEAKAGE_ACTIVE;
	}
}

// =======================================================================
// GmBoss5EfctEndLeakage
/*!
  漏電エフェクト終了
  
  @param body_work	[io]	本体ワーク
  @param no_vanish	[in]	消失エフェクト無効フラグ
  							(TRUE: 消失EF生成しない, FALSE: 消失EF生成する)
  
  @note
  消去を開始します。直ちには消えません。
 */
// =======================================================================
void GmBoss5EfctEndLeakage(GMS_BOSS5_BODY_WORK *body_work, BOOL no_vanish/*=FALSE*/)
{
	MTM_ASSERT(body_work);
	
	if (body_work->flag & GMD_BOSS5_BODY_FLAG_LEAKAGE_ACTIVE) {
		// フラグがオフになる瞬間だけ
		// 漏電SE停止
		GsSoundStopSeHandle(body_work->se_hnd_leakage, 30);
		
#if !_IPHONE
		if (!no_vanish) {
			// 消失エフェクト生成
			gmBoss5EfctCreateLeakageVanish(body_work);
		}
#endif // !_IPHONE
	}
	
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_LEAKAGE_ACTIVE;
}

// =======================================================================
// GmBoss5EfctStartPrelimLeakage
/*!
  予備漏電エフェクト開始
  
  @param body_work	[io]	本体ワーク
  
  @note
  漏電エフェクト前の予備動作エフェクト
 */
// =======================================================================
void GmBoss5EfctStartPrelimLeakage(GMS_BOSS5_BODY_WORK *body_work)
{
	MTM_ASSERT(body_work);
	
	// 呼び漏電エフェクト有効フラグセット
	body_work->flag	|= GMD_BOSS5_BODY_FLAG_PRELIM_LEAKAGE_ACTIVE;
	
	// エフェクト生成
	gmBoss5EfctCreatePrelimLeakage(body_work);
}

// =======================================================================
// GmBoss5EfctEndPrelimLeakage
/*!
  予備漏電エフェクト終了
  
  @param body_work	[io]	本体ワーク
  
  @note
  消去を開始します。直ちには消えません。
 */
// =======================================================================
void GmBoss5EfctEndPrelimLeakage(GMS_BOSS5_BODY_WORK *body_work)
{
	MTM_ASSERT(body_work);
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_PRELIM_LEAKAGE_ACTIVE;
}

// =======================================================================
// GmBoss5EfctCreateWalkStepSmoke
/*!
  歩き足接地時 煙エフェクト 生成
  
  @param body_work	[io]	本体ワーク
  @param leg_type	[in]	脚タイプ
 */
// =======================================================================
void GmBoss5EfctCreateWalkStepSmoke(GMS_BOSS5_BODY_WORK *body_work,
									GME_BOSS5_LEG_TYPE leg_type)
{
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*walk_smoke;
	
	walk_smoke	=
		(GMS_BOSS5_EFCT_GENERAL_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
														  GME_BOSS5_EFCT_IDX_FB_SMORK00,
														  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	efct_com	= (GMS_EFFECT_COM_WORK*)walk_smoke;
	obj_work	= GMM_BS_OBJ(walk_smoke);
	
	
	// 生成位置設定
	{
		Sint32	targ_node_snm_reg_id;
		const NNS_MATRIX	*w_mtx;
		
		// 指定された脚ノードのSNM登録IDを得る
		targ_node_snm_reg_id	= body_work->leg_snm_reg_ids[leg_type];
		
		w_mtx	= GmBsCmnGetSNMMtx(&body_work->snm_work, targ_node_snm_reg_id);
		
		// 座標設定
		obj_work->pos.x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3));	// 脚のX成分
		obj_work->pos.y	= body_work->ground_v_pos;	// 地面の高さに設定
		obj_work->pos.z	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 2, 3));	// Zもあわせる
		
		// 手前に表示させる
		obj_work->pos.z	+= GMD_BOSS5_EFCT_WALK_STEP_SMOKE_OFST_Z;
	}
	
	// メイン処理はデフォルト
}

// =======================================================================
// GmBoss5EfctCreateRunStepSmoke
/*!
  走り足接地時 煙エフェクト 生成
  
  @param body_work	[io]	本体ワーク
  @param leg_type	[in]	脚タイプ
 */
// =======================================================================
void GmBoss5EfctCreateRunStepSmoke(GMS_BOSS5_BODY_WORK *body_work,
								   GME_BOSS5_LEG_TYPE leg_type)
{
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*run_smoke;
	
	run_smoke	=
		(GMS_BOSS5_EFCT_GENERAL_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
														  GME_BOSS5_EFCT_IDX_FB_SMORK01,
														  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	efct_com	= (GMS_EFFECT_COM_WORK*)run_smoke;
	obj_work	= GMM_BS_OBJ(run_smoke);
	
	
	// 生成位置設定
	{
		Sint32	targ_node_snm_reg_id;
		const NNS_MATRIX	*w_mtx;
		
		// 指定された脚ノードのSNM登録IDを得る
		targ_node_snm_reg_id	= body_work->leg_snm_reg_ids[leg_type];
		
		w_mtx	= GmBsCmnGetSNMMtx(&body_work->snm_work, targ_node_snm_reg_id);
		
		// 座標設定
		obj_work->pos.x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3));	// 脚のX成分
		obj_work->pos.y	= body_work->ground_v_pos;	// 地面の高さに設定
		obj_work->pos.z	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 2, 3));	// Zもあわせる
		
		// 手前に表示させる
		obj_work->pos.z	+= GMD_BOSS5_EFCT_RUN_STEP_SMOKE_OFST_Z;
	}
	
	// メイン処理はデフォルト
}

// =======================================================================
// GmBoss5EfctCreateBerserkStampSmoke
/*!
  凶暴化演出時 踏み込み煙エフェクト 生成
  
  @param body_work		[io]	本体ワーク
  @param leg_type		[in]	脚タイプ
  @param spawn_delay	[in]	エフェクト発生までの遅延時間
 */
// =======================================================================
void GmBoss5EfctCreateBerserkStampSmoke(GMS_BOSS5_BODY_WORK *body_work,
										GME_BOSS5_LEG_TYPE leg_type,
										Uint32 spawn_delay)
{
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*stamp_smoke;
	
	efct_com	= (GMS_EFFECT_COM_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
															GME_BOSS5_EFCT_IDX_FB_SMORK01,
															sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	obj_work	= (OBS_OBJECT_WORK*)efct_com;
	stamp_smoke	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 参照SNMノード設定
	stamp_smoke->ref_node_snm_id	= body_work->leg_snm_reg_ids[leg_type];
	
	// 初期状態では描画更新・表示は無効
	obj_work->disp_flag	|= (OBD_DISP_NOUPDATE | OBD_DISP_NODISP);
	
	// 発生までの待ち時間設定
	stamp_smoke->timer	= spawn_delay;
	
	obj_work->ppFunc	= gmBoss5EfctBerserkStampSmokeProcWaitStart;
}

// =======================================================================
// GmBoss5EfctCreateCrashLandingSmoke
/*!
  地球割り着地時 煙エフェクト 生成
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void GmBoss5EfctCreateCrashLandingSmoke(GMS_BOSS5_BODY_WORK *body_work)
{
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*landing_smoke;
	
	landing_smoke	=
		(GMS_BOSS5_EFCT_GENERAL_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
														  GME_BOSS5_EFCT_IDX_FB_SMORK02,
														  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	efct_com	= (GMS_EFFECT_COM_WORK*)landing_smoke;
	obj_work	= GMM_BS_OBJ(landing_smoke);
	
	// 生成位置設定
	obj_work->pos.x	= GMM_BS_OBJ(body_work)->pos.x;
	obj_work->pos.y	= body_work->ground_v_pos;	// 地面の高さに設定
	obj_work->pos.z	= GMM_BS_OBJ(body_work)->pos.z + GMD_BOSS5_EFCT_CRASH_LANDING_SMOKE_OFST_Z;	// 手前に表示させる
	
	// メイン処理はデフォルト
}

// =======================================================================
// GmBoss5EfctCreateBreakingGlass
/*!
  割れガラスエフェクト 生成
  
  @param parent_obj	[io]	親オブジェクトワーク
 */
// =======================================================================
void GmBoss5EfctCreateBreakingGlass(OBS_OBJECT_WORK *parent_obj)
{
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*brk_glass;
	BOOL	is_out;
	
	// 水平方向の画面外チェック
	is_out	= ObjViewOutCheck(parent_obj->pos.x,
							  parent_obj->pos.y,
							  0,
							  0,
							  (Sint16)(-(OBD_OBJ_CLIP_LCD_Y / 2)),	// 垂直方向は画面外と判定させない
							  0,
							  (Sint16)(OBD_OBJ_CLIP_LCD_Y / 2)	// 垂直方向は画面外と判定させない
							  );
	
	// 画面外なら生成しない
	if (is_out) {
		return;
	}
	
	brk_glass	=
		(GMS_BOSS5_EFCT_GENERAL_WORK*)gmBoss5EfctEsCreate(parent_obj,
														  GME_BOSS5_EFCT_IDX_GLASS,
														  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	efct_com	= (GMS_EFFECT_COM_WORK*)brk_glass;
	obj_work	= GMM_BS_OBJ(brk_glass);
	
	// 座標調整
	obj_work->pos.y	+= GMD_BOSS5_EFCT_BREAKING_GLASS_OFST_Y;	// 真中より上にずらす
	obj_work->pos.z	+= GMD_BOSS5_EFCT_BREAKING_GLASS_OFST_Z;	// 手前に表示させる
	
	// メイン処理はデフォルト
}

// =======================================================================
// GmBoss5EfctStartJet
/*!
  本体噴射エフェクト開始
  
  @param body_work	[io]	本体ワーク

  @note
  GmBoss5EfctEndJet()で停止してください。
 */
// =======================================================================
void GmBoss5EfctStartJet(GMS_BOSS5_BODY_WORK *body_work)
{
	MTM_ASSERT(body_work);
	
	// 噴射エフェクト有効フラグセット
	body_work->flag	|= GMD_BOSS5_BODY_FLAG_JET_ACTIVE;
	
	// エフェクト生成
	gmBoss5EfctCreateJet(body_work);
}

// =======================================================================
// GmBoss5EfctEndJet
/*!
  本体噴射エフェクト終了
  
  @param body_work	[io]	本体ワーク
  
  @note
  消去を開始します。直ちには消えません。
 */
// =======================================================================
void GmBoss5EfctEndJet(GMS_BOSS5_BODY_WORK *body_work)
{
	MTM_ASSERT(body_work);
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_JET_ACTIVE;
}

// =======================================================================
// GmBoss5EfctStartJetSmoke
/*!
  本体噴射スモークエフェクト開始
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void GmBoss5EfctStartJetSmoke(GMS_BOSS5_BODY_WORK *body_work)
{
	MTM_ASSERT(body_work);
	
	// 噴射スモークエフェクト有効フラグセット
	body_work->flag	|= GMD_BOSS5_BODY_FLAG_JETSMOKE_ACTIVE;
	
	// エフェクト生成
	gmBoss5EfctCreateJetSmoke(body_work);
}

// =======================================================================
// GmBoss5EfctEndJetSmoke
/*!
  本体噴射スモークエフェクト開始
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void GmBoss5EfctEndJetSmoke(GMS_BOSS5_BODY_WORK *body_work)
{
	MTM_ASSERT(body_work);
	body_work->flag	&= ~GMD_BOSS5_BODY_FLAG_JETSMOKE_ACTIVE;
}

// =======================================================================
// GmBoss5EfctTryStartRocketLeakage
/*!
  ロケット漏電エフェクト開始
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void GmBoss5EfctTryStartRocketLeakage(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	MTM_ASSERT(rkt_work);
	
	if (!(rkt_work->flag & GMD_BOSS5_RKT_FLAG_LEAKAGE_ACTIVE)) {
		gmBoss5EfctCreateRocketLeakage(rkt_work);
		rkt_work->flag	|= GMD_BOSS5_RKT_FLAG_LEAKAGE_ACTIVE;
	}
}

// =======================================================================
// GmBoss5EfctEndRocketLeakage
/*!
  ロケット漏電エフェクト終了
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  消去を開始します。直ちには消えません。
 */
// =======================================================================
void GmBoss5EfctEndRocketLeakage(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	MTM_ASSERT(rkt_work);
	rkt_work->flag	&= ~GMD_BOSS5_RKT_FLAG_LEAKAGE_ACTIVE;
}

// =======================================================================
// GmBoss5EfctCreateRocketLaunch
/*!
  ロケット発射エフェクト
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void GmBoss5EfctCreateRocketLaunch(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*rkt_launch;
	
	efct_com	= (GMS_EFFECT_COM_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(rkt_work),
															GME_BOSS5_EFCT_IDX_ROCKET_S,
															sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	obj_work	= (OBS_OBJECT_WORK*)efct_com;
	rkt_launch	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// オフセットマトリクス
	nnMakeTranslateMatrix(&rkt_launch->ofst_mtx,
						  GMD_BOSS5_EFCT_ROCKET_LAUNCH_STUCK_ND_OFST_X, 0, 0);
	
	
	// 座標更新（最初だけ）
	GmBsCmnUpdateObject3DESStuckWithNodeRelative(obj_work,
												 &rkt_work->snm_work,
												 rkt_work->drill_snm_reg_id,
												 TRUE,
												 &obj_work->parent_obj->pos,
												 &rkt_work->pivot_prev_pos,
												 &rkt_launch->ofst_mtx);
	
	// 手前に表示
	obj_work->pos.z	+= GMD_BOSS5_EFCT_ROCKET_LAUNCH_OFST_Z;
	
	// メイン処理はデフォルト
}

// =======================================================================
// GmBoss5EfctCreateRocketDock
/*!
  ロケットドッキングエフェクト
  
  @param body_work	[io]	本体ワーク
  @param rkt_type	[in]	ロケットタイプ
  
  @note
  本体オブジェクトが親として設定されます。
 */
// =======================================================================
void GmBoss5EfctCreateRocketDock(GMS_BOSS5_BODY_WORK *body_work, GME_BOSS5_RKT_TYPE rkt_type)
{
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*rkt_dock;
	Float	stuck_node_ofst_x;
	
	efct_com	= (GMS_EFFECT_COM_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
															GME_BOSS5_EFCT_IDX_ROCKET_E,
															sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	obj_work	= (OBS_OBJECT_WORK*)efct_com;
	rkt_dock	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 左右に応じて適切な値を設定する
	if (rkt_type == GME_BOSS5_RKT_TYPE_LEFT) {
		// 接着ノード設定（本体側のノード）
		rkt_dock->ref_node_snm_id	= body_work->armpt_snm_reg_ids[GME_BOSS5_ARM_TYPE_LEFT][GME_BOSS5_ARMPART_IDX_FOREARM];
		// ノード追随配置オフセット
		stuck_node_ofst_x	= GMD_BOSS5_EFCT_ROCKET_DOCK_STUCK_ND_OFST_X;
	}
	else {
		MTM_ASSERT(rkt_type == GME_BOSS5_RKT_TYPE_RIGHT);
		// 接着ノード設定（本体側のノード）
		rkt_dock->ref_node_snm_id	= body_work->armpt_snm_reg_ids[GME_BOSS5_ARM_TYPE_RIGHT][GME_BOSS5_ARMPART_IDX_FOREARM];
		// ノード追随配置オフセット
		stuck_node_ofst_x	= -GMD_BOSS5_EFCT_ROCKET_DOCK_STUCK_ND_OFST_X;
	}
	
	// オフセットマトリクス
	nnMakeTranslateMatrix(&rkt_dock->ofst_mtx,
						  stuck_node_ofst_x, 0, 0);
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss5EfctRocketDockProcMain;
}

// =======================================================================
// GmBoss5EfctStartRocketJet
/*!
  ロケット噴射エフェクト開始
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void GmBoss5EfctStartRocketJet(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	MTM_ASSERT(rkt_work);
	
	if (!(rkt_work->flag & GMD_BOSS5_RKT_FLAG_JET_NML_ACTIVE)) {
		gmBoss5EfctCreateRocketJet(rkt_work, FALSE);
		rkt_work->flag	|= GMD_BOSS5_RKT_FLAG_JET_NML_ACTIVE;
	}
}

// =======================================================================
// GmBoss5EfctEndRocketJet
/*!
  ロケット噴射エフェクト終了
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  消去を開始します。直ちには消えません。
 */
// =======================================================================
void GmBoss5EfctEndRocketJet(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	MTM_ASSERT(rkt_work);
	rkt_work->flag	&= ~GMD_BOSS5_RKT_FLAG_JET_NML_ACTIVE;
}

// =======================================================================
// GmBoss5EfctStartRocketJetReverse
/*!
  ロケット逆噴射エフェクト開始
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void GmBoss5EfctStartRocketJetReverse(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	MTM_ASSERT(rkt_work);
	
	if (!(rkt_work->flag & GMD_BOSS5_RKT_FLAG_JET_REV_ACTIVE)) {
		gmBoss5EfctCreateRocketJet(rkt_work, TRUE);
		rkt_work->flag	|= GMD_BOSS5_RKT_FLAG_JET_REV_ACTIVE;
	}
}

// =======================================================================
// GmBoss5EfctEndRocketJetReverse
/*!
  ロケット逆噴射エフェクト終了
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  消去を開始します。直ちには消えません。
 */
// =======================================================================
void GmBoss5EfctEndRocketJetReverse(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	MTM_ASSERT(rkt_work);
	rkt_work->flag	&= ~GMD_BOSS5_RKT_FLAG_JET_REV_ACTIVE;
}

// =======================================================================
// GmBoss5EfctCreateRocketLandingShockwave
/*!
  ロケット着地衝撃波エフェクト生成
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void GmBoss5EfctCreateRocketLandingShockwave(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*rkt_landing_sw;
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)GMM_BS_OBJ(rkt_work)->parent_obj;
	
	efct_com	= (GMS_EFFECT_COM_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(rkt_work),
															GME_BOSS5_EFCT_IDX_ROCKET_SMORK,
															sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	obj_work	= (OBS_OBJECT_WORK*)efct_com;
	rkt_landing_sw	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 生成位置設定
	obj_work->pos.y	= parent_body->ground_v_pos;
	
	// 手前に表示する
	obj_work->pos.z	+= GMD_BOSS5_EFCT_LANDING_SW_OFST_Z;
	
	// 処理関数はデフォルト
}

// =======================================================================
// GmBoss5EfctCreateLandingShockwave
/*!
  着地衝撃波生成
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void GmBoss5EfctCreateLandingShockwave(GMS_BOSS5_BODY_WORK *body_work)
{
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*landing_sw;
	
	efct_com	= (GMS_EFFECT_COM_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work), 
															GME_BOSS5_EFCT_IDX_SHOCK,
															sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	obj_work	= (OBS_OBJECT_WORK*)efct_com;
	landing_sw	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 手前に表示させる
	obj_work->pos.z	+= GMD_BOSS5_EFCT_LANDING_SW_OFST_Z;
	
	// 当たり設定
	GmBsCmnSetEfctAtkVsPly((GMS_EFFECT_COM_WORK*)obj_work,
						   GMD_BOSS5_EFCT_LANDING_SW_VIEWOUT_OFST);
	obj_work->flag	|= OBD_OBJECT_NOCLIP;	// クリッピングしない
	ObjRectWorkSet(&efct_com->rect_work[GME_EFFECT_RECT_ATK],
				   GMD_BOSS5_EFCT_LANDING_SW_ATK_RECT_SIZE_LEFT,
				   GMD_BOSS5_EFCT_LANDING_SW_ATK_RECT_SIZE_TOP,
				   GMD_BOSS5_EFCT_LANDING_SW_ATK_RECT_SIZE_RIGHT,
				   GMD_BOSS5_EFCT_LANDING_SW_ATK_RECT_SIZE_BOTTOM);
	efct_com->rect_work[GME_EFFECT_RECT_ATK].flag	&= ~OBD_RECT_NOHIT;
	
	// 攻撃矩形有効時間
	landing_sw->timer	= GMD_BOSS5_EFCT_LANDING_SW_ATK_RECT_ACTIVE_TIME;
	
	obj_work->ppFunc	= gmBoss5EfctLandingShockwaveProcMain;
}

// =======================================================================
// GmBoss5EfctCreateStrikeShockwave
/*!
  地面突き衝撃波
  
  @param body_work		[io]	本体ワーク
  @param spawn_delay	[in]	エフェクト発生までの遅延時間
 */
// =======================================================================
void GmBoss5EfctCreateStrikeShockwave(GMS_BOSS5_BODY_WORK *body_work, Uint32 spawn_delay)
{
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*strike_sw;
	
	efct_com	= (GMS_EFFECT_COM_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
															GME_BOSS5_EFCT_IDX_SHOCK_ATK,
															sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	obj_work	= (OBS_OBJECT_WORK*)efct_com;
	strike_sw	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 当たり設定
	GmBsCmnSetEfctAtkVsPly((GMS_EFFECT_COM_WORK*)obj_work,
						   GMD_BOSS5_EFCT_STRIKE_SW_VIEWOUT_OFST);
	obj_work->flag	|= OBD_OBJECT_NOCLIP;	// クリッピングしない
	ObjRectWorkSet(&efct_com->rect_work[GME_EFFECT_RECT_ATK],
				   GMD_BOSS5_EFCT_STRIKE_SW_ATK_RECT_SIZE_LEFT,
				   GMD_BOSS5_EFCT_STRIKE_SW_ATK_RECT_SIZE_TOP,
				   GMD_BOSS5_EFCT_STRIKE_SW_ATK_RECT_SIZE_RIGHT,
				   GMD_BOSS5_EFCT_STRIKE_SW_ATK_RECT_SIZE_BOTTOM);
	efct_com->rect_work[GME_EFFECT_RECT_ATK].flag	&= ~OBD_RECT_NOHIT;
	
	// 初期状態では当たり・描画更新・表示は無効
	obj_work->flag	|= OBD_OBJECT_NOHIT;
	obj_work->disp_flag	|= (OBD_DISP_NOUPDATE | OBD_DISP_NODISP);
	
	// 発生までの待ち時間設定
	strike_sw->timer	= spawn_delay;
	
	obj_work->ppFunc	= gmBoss5EfctStrikeShockwaveProcWaitStart;
}

// =======================================================================
// GmBoss5EfctTargetCursorInit
/*!
  ターゲットカーソルエフェクト初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void GmBoss5EfctTargetCursorInit(GMS_BOSS5_BODY_WORK *body_work)
{
	gmBoss5EfctCreateTargetCursorStart(body_work);
}

// =======================================================================
// GmBoss5EfctCrashCursorInit
/*!
  地球割り用ターゲットカーソル初期化
  
  @param body_work		[io]	本体ワーク
  @param pos_x			[in]	停止目標X位置
  @param duration_time	[in]	継続時間
 */
// =======================================================================
void GmBoss5EfctCrashCursorInit(GMS_BOSS5_BODY_WORK *body_work,
								fx32 pos_x, Uint32 duration_time)
{
	gmBoss5EfctCreateCrashCursorStart(body_work, pos_x, duration_time);
}

// =======================================================================
// GmBoss5EfctCreateVulcanFire
/*!
  バルカン発射エフェクト初期化
  
  @param trt_work	[io]	砲塔ワーク
  @param pos		[in]	発射位置
  @param angle		[in]	発射方向
 */
// =======================================================================
void GmBoss5EfctCreateVulcanFire(GMS_BOSS5_TURRET_WORK *trt_work,
								 const VecFx32 *pos, Angle32 angle)
{
	UNREFERENCED_PARAMETER(trt_work);
	
	GMS_EFFECT_3DES_WORK	*efct_fire;
	OBS_OBJECT_WORK	*fire_obj;
	
	// エフェクト生成
	efct_fire	= GmEfctCmnEsCreate(GMM_BS_OBJ(trt_work),
									GME_EFCT_CMN_IDX_BULLET);
	
	fire_obj	= (OBS_OBJECT_WORK*)efct_fire;
	
	// 角度設定
	fire_obj->dir.z	= (Uint16)(Angle16)(MTD_MATH_ANGLE_MASK & angle);
	
	// 表示角度調整
	GmEffect3DESAddDispRotation(efct_fire, AKM_DEGtoA32(90), 0, 0);
	
	// 座標設定
	fire_obj->pos	= *pos;
	
	// 発射SE再生
	GmSoundPlaySE("FinalBoss14");
}

// =======================================================================
// GmBoss5EfctCreateVulcanBullet
/*!
  バルカン弾エフェクト初期化
  
  @param trt_work	[io]	砲塔ワーク
  @param pos		[in]	発射位置
  @param angle		[in]	発射方向
  @param spd		[in]	移動速度
 */
// =======================================================================
void GmBoss5EfctCreateVulcanBullet(GMS_BOSS5_TURRET_WORK *trt_work,
								   const VecFx32 *pos, Angle32 angle, fx32 spd)
{
	UNREFERENCED_PARAMETER(trt_work);
	
	GMS_EFFECT_3DES_WORK	*efct_bullet;
	OBS_OBJECT_WORK	*bullet_obj;
	OBS_RECT_WORK	*rect_work;
	
	// エフェクト生成
	efct_bullet	= GmEfctCmnEsCreate(GMM_BS_OBJ(trt_work),
									GME_EFCT_CMN_IDX_BULLET_CORE);
	
	bullet_obj	= (OBS_OBJECT_WORK*)efct_bullet;
	
	// 攻撃オブジェクトに設定
	GmBsCmnSetEfctAtkVsPly(&efct_bullet->efct_com,
						   GMD_BOSS5_EFCT_VULCAN_BULLET_VIEWOUT_OFST);
	bullet_obj->flag	|= OBD_OBJECT_NOCLIP;	// 最初はクリッピングしない
	
	// 矩形設定
	rect_work	= &efct_bullet->efct_com.rect_work[GME_EFFECT_RECT_ATK];
	ObjRectWorkSet(rect_work,
				   GMD_BOSS5_EFCT_VULCAN_BULLET_ATK_RECT_SIZE_LEFT,
				   GMD_BOSS5_EFCT_VULCAN_BULLET_ATK_RECT_SIZE_TOP,
				   GMD_BOSS5_EFCT_VULCAN_BULLET_ATK_RECT_SIZE_RIGHT,
				   GMD_BOSS5_EFCT_VULCAN_BULLET_ATK_RECT_SIZE_BOTTOM);
	rect_work->flag	|= OBD_RECT_ENABLE;
	
	// 角度設定
	bullet_obj->dir.z	= (Uint16)(Angle16)(MTD_MATH_ANGLE_MASK & angle);
	
	// 表示角度調整
	GmEffect3DESAddDispRotation(efct_bullet, AKM_DEGtoA32(-90), 0, 0);
	
	// 座標設定
	bullet_obj->pos	= *pos;
	
	// 手前に表示させる
	bullet_obj->pos.z	+= GMD_BOSS5_EFCT_VULCAN_BULLET_OFST_Z;
	
	// 速度設定
	bullet_obj->spd.x	= FX_Mul(spd, FX_F32_TO_FX32(nnCos(angle)));
	bullet_obj->spd.y	= FX_Mul(spd, FX_F32_TO_FX32(nnSin(angle)));
	bullet_obj->spd.z	= 0;
	
	// 処理関数設定
	bullet_obj->ppFunc	= gmBoss5EfctVulcanBulletProcMain;
}

// =======================================================================
// GmBoss5EfctCreateSmallExplosion
/*!
  小爆発生成
  
  @param pos_x	[in]	生成座標X
  @param pos_y	[in]	生成座標Y
  @param pos_z	[in]	生成座標Z
 */
// =======================================================================
void GmBoss5EfctCreateSmallExplosion(fx32 pos_x, fx32 pos_y, fx32 pos_z)
{
	GMS_EFFECT_3DES_WORK	*efct_expl;
	OBS_OBJECT_WORK	*expl_obj;
	
	// エフェクト生成
	efct_expl	= GmEfctCmnEsCreate(NULL, GME_EFCT_CMN_IDX_BOMB);
	
	expl_obj	= (OBS_OBJECT_WORK*)efct_expl;
	
	// 座標設定
	expl_obj->pos.x	= pos_x;
	expl_obj->pos.y	= pos_y;
	expl_obj->pos.z	= pos_z;
}

// =======================================================================
// GmBoss5EfctCreateBigExplosion
/*!
  大爆発生成
  
  @param pos_x	[in]	生成座標X
  @param pos_y	[in]	生成座標Y
  @param pos_z	[in]	生成座標Z
 */
// =======================================================================
void GmBoss5EfctCreateBigExplosion(fx32 pos_x, fx32 pos_y, fx32 pos_z)
{
	GMS_EFFECT_3DES_WORK	*efct_expl;
	OBS_OBJECT_WORK	*expl_obj;
	
	// エフェクト生成
	efct_expl	= GmEfctCmnEsCreate(NULL, GME_EFCT_CMN_IDX_BOMB_BIG);
	
	expl_obj	= (OBS_OBJECT_WORK*)efct_expl;
	
	// 座標設定
	expl_obj->pos.x	= pos_x;
	expl_obj->pos.y	= pos_y;
	expl_obj->pos.z	= pos_z;
}

// =======================================================================
// GmBoss5EfctCreateFragments
/*!
  破片エフェクト生成
  
  @param pos_x	[in]	生成座標X
  @param pos_y	[in]	生成座標Y
  @param pos_z	[in]	生成座標Z
 */
// =======================================================================
void GmBoss5EfctCreateFragments(fx32 pos_x, fx32 pos_y, fx32 pos_z)
{
	GMS_EFFECT_3DES_WORK	*efct_frags;
	OBS_OBJECT_WORK	*frags_obj;
	
	// エフェクト生成
	efct_frags	= GmEfctBossCmnEsCreate(NULL, GME_EFCT_BOSS_CMN_IDX_BOSS_DM);
	
	frags_obj	= (OBS_OBJECT_WORK*)efct_frags;
	
	// 座標設定
	frags_obj->pos.x	= pos_x;
	frags_obj->pos.y	= pos_y;
	frags_obj->pos.z	= pos_z;
}

// =======================================================================
// GmBoss5EfctCreateDamage
/*!
  ダメージエフェクト 生成
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void GmBoss5EfctCreateDamage(GMS_BOSS5_BODY_WORK *body_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	OBS_OBJECT_WORK	*obj_work;
	
	efct_work	= GmEfctBossCmnEsCreate(NULL, GME_EFCT_BOSS_CMN_IDX_BOSS_DM);
	
	obj_work	= (OBS_OBJECT_WORK*)efct_work;
	
	// 本体の中心オブジェクトの位置に表示
	obj_work->pos.x	= body_work->part_obj_core->pos.x;
	obj_work->pos.y	= body_work->part_obj_core->pos.y;
	obj_work->pos.z	= body_work->part_obj_core->pos.z + GMD_BOSS5_EFCT_DAMAGE_OFST_Z;
}

// =======================================================================
// GmBoss5EfctStartRocketSmoke
/*!
  ロケット黒煙エフェクト 開始
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void GmBoss5EfctStartRocketSmoke(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	MTM_ASSERT(rkt_work);
	
	// ロケット黒煙エフェクト有効フラグセット
	rkt_work->flag	|= GMD_BOSS5_RKT_FLAG_SMOKE_ACTIVE;
	
	// エフェクト生成
	gmBoss5EfctCreateRocketSmoke(rkt_work);
}

// =======================================================================
// GmBoss5EfctEndRocketSmoke
/*!
  ロケット黒煙エフェクト 終了
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  消去を開始します。直ちには消えません。
 */
// =======================================================================
void GmBoss5EfctEndRocketSmoke(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	MTM_ASSERT(rkt_work);
	rkt_work->flag	&= ~GMD_BOSS5_RKT_FLAG_SMOKE_ACTIVE;
}

// =======================================================================
// GmBoss5EfctBreakdownSmokesInit
/*!
  機能停止黒煙エフェクト
  
  @param body_work		[io]	本体ワーク
  @param duration_time	[in]	継続時間
  
  @note
  duration_time経過後に自動的に消えます。
 */
// =======================================================================
void GmBoss5EfctBreakdownSmokesInit(GMS_BOSS5_BODY_WORK *body_work,
								   Uint32 duration_time)
{
	
	for (Uint32 i = 0; i < GMD_BOSS5_EFCT_BREAKDOWN_SMOKES_NUM; ++i) {
		GMS_EFFECT_3DES_WORK	*efct_3des;
		OBS_OBJECT_WORK	*obj_work;
		
		efct_3des	=
			GmEfctBossCmnEsCreate(GMM_BS_OBJ(body_work),
								  GME_EFCT_BOSS_CMN_IDX_BOSS_SMORK);
		
		obj_work	= (OBS_OBJECT_WORK*)efct_3des;
		
		// オブジェクトの追随は自前で制御(StuckWithNode)
		// ＆ パーティクルをエミッタに追随させない
		GmEffect3DESChangeBase(efct_3des,
							   GME_EFFECT_3DES_POS_TYPE_EMT,
							   efct_3des->saved_init_flag & ~(GMD_EFFECT_3DES_FLAG_STICKPARENT));
		
		// 表示オフセット設定
		GmEffect3DESSetDispOffset(efct_3des,
								  gm_boss5_efct_breakdown_smoke_disp_ofst_tbl[i][MTD_X],
								  gm_boss5_efct_breakdown_smoke_disp_ofst_tbl[i][MTD_Y],
								  gm_boss5_efct_breakdown_smoke_disp_ofst_tbl[i][MTD_Z]);
		
		// 表示回転設定
		GmEffect3DESSetDispRotation(efct_3des,
									gm_boss5_efct_breakdown_smoke_disp_rot_tbl[i][MTD_X],
									gm_boss5_efct_breakdown_smoke_disp_rot_tbl[i][MTD_Y],
									gm_boss5_efct_breakdown_smoke_disp_rot_tbl[i][MTD_Z]);
		
		// 継続時間設定
		obj_work->user_timer	= duration_time;
		
		// 処理関数設定
		obj_work->ppFunc	= gmBoss5EfctBreakdownSmokeProcLoop;
	}
}

// =======================================================================
// GmBoss5EfctBodySmallSmokesInit
/*!
  本体 小さい黒煙エフェクト 初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void GmBoss5EfctBodySmallSmokesInit(GMS_BOSS5_BODY_WORK *body_work)
{
	for (Uint32 i = 0; i < GMD_BOSS5_EFCT_BODY_SMALL_SMOKES_NUM; ++i) {
		gmBoss5EfctCreateBodySmallSmoke(body_work, i);
	}
}

// =======================================================================
// GmBoss5EfctBerserkSteamInit
/*!
  凶暴スチームエフェクト
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void GmBoss5EfctBerserkSteamInit(GMS_BOSS5_BODY_WORK *body_work, Uint32 count)
{
	for (Uint32 i = 0; i < GMD_BOSS5_EFCT_BERSERK_STEAM_NUM; ++i) {
		gmBoss5EfctCreateBerserkSteam(body_work, count, i);
	}
}


// =======================================================================
// GmBoss5EfctStartEggSweat
/*!
  エッグマン 汗エフェクト開始
  
  @param egg_work	[io]	エッグマンワーク
 */
// =======================================================================
void GmBoss5EfctStartEggSweat(GMS_BOSS5_EGG_WORK *egg_work)
{
	MTM_ASSERT(egg_work);
	
	// 汗エフェクト有効フラグセット
	egg_work->flag	|= GMD_BOSS5_EGG_FLAG_SWEAT_ACTIVE;
	
	// エフェクト生成
	gmBoss5EfctCreateEggSweat(egg_work);
}

// =======================================================================
// GmBoss5EfctEndEggSweat
/*!
  エッグマン 汗エフェクト終了
  
  @param egg_work	[io]	エッグマンワーク
 */
// =======================================================================
void GmBoss5EfctEndEggSweat(GMS_BOSS5_EGG_WORK *egg_work)
{
	MTM_ASSERT(egg_work);
	egg_work->flag	&= ~GMD_BOSS5_EGG_FLAG_SWEAT_ACTIVE;
}

// =======================================================================
// GmBoss5EfctCreateRocketRollSpark
/*!
  ロケット 回転火花エフェクト生成
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void GmBoss5EfctCreateRocketRollSpark(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work;
	
	obj_work	= (OBS_OBJECT_WORK*)GmEfctCmnEsCreate(GMM_BS_OBJ(rkt_work),
													  GME_EFCT_CMN_IDX_PISTON);
	
	// 生成位置設定
	obj_work->pos.y = ((GMS_BOSS5_BODY_WORK*)GMM_BS_OBJ(rkt_work)->parent_obj)->ground_v_pos;
	
	// 向き設定
	obj_work->dir.z	= GMD_BOSS5_EFCT_ROCKET_ROLLING_SPARK_DIR_Z;
	
	// 手前に表示させる
	obj_work->pos.z	+= GMD_BOSS5_EFCT_ROCKET_ROLLING_SPARK_OFST_Z;
}


/*------ Static Functions ----------------------------------------------*/

// =======================================================================
// gmBoss5EfctEsCreate
/*!
  ボス5エフェクト生成
 
  @param parent_obj	[io]	親オブジェクト（NULL可）
  @param efct_idx	[in]	エフェクトインデックス
  
  @return エフェクト3DESワーク
 */
// =======================================================================
GMS_EFFECT_3DES_WORK* gmBoss5EfctEsCreate(OBS_OBJECT_WORK *parent_obj, Sint32 efct_idx,
										  Uint32 work_size/*=sizeof(GMS_EFFECT_3DES_WORK)*/)
{
	GMS_EFFECT_3DES_WORK	*eff_3des;
	const GMS_EFFECT_CREATE_PARAM	*cr_param;
	const GMS_BOSS5_EFCT_DATA_INFO	*data_info;
	OBS_DATA_WORK	*model_data_work;
	OBS_DATA_WORK	*object_data_work;
	
	// 指定インデックスの生成情報取得
	cr_param	= &gm_boss5_efct_create_param_tbl[efct_idx];
	
	// 指定インデックスのデータ情報取得
	data_info	= &gm_boss5_efct_data_info_tbl[efct_idx];
	
	// テーブル間のAMEインデックス整合性チェック
	MTM_ASSERT(cr_param->ame_idx == data_info->ame_arc_idx);
	
	// モデル取得
	if (cr_param->model_idx != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
		MTM_ASSERT(data_info->use_model);
		MTM_ASSERT(cr_param->model_idx == data_info->model_arc_idx);
		
		// モデルデータを格納するデータワーク取得
		model_data_work	= ObjDataGet(data_info->model_dwork_no);
		
		// オブジェクトを格納するデータワークを取得
		object_data_work	= ObjDataGet(data_info->object_dwork_no);
	}
	else {
		model_data_work	= NULL;
		object_data_work	= NULL;
	}
	
	eff_3des	= GmEffect3dESCreateByParam(cr_param,
											parent_obj,
											GMD_BOSS5_EFCT_ARC,
											ObjDataGet(data_info->ame_dwork_no),
											ObjDataGet(data_info->tex_amb_dwork_no),
											ObjDataGet(data_info->tex_list_dwork_no),
											model_data_work,
											object_data_work,
											work_size);
	
	return eff_3des;
}

#if _IPHONE
// =======================================================================
// gmBoss5EfctCreateLeakage
/*!
  漏電エフェクト初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5EfctCreateLeakage(GMS_BOSS5_BODY_WORK *body_work)
{
	// 予備漏電開始処理を流用
	GMS_BOSS5_EFCT_GENERAL_WORK	*prelim_lkg;
	
	prelim_lkg	=
		(GMS_BOSS5_EFCT_GENERAL_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
														  GME_BOSS5_EFCT_IDX_BLITZ_FB_START,
														  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	// 使用するメイン処理は独自の設定
	GMM_BS_OBJ(prelim_lkg)->ppFunc	= gmBoss5EfctLeakagePartProcMain;
}

// =======================================================================
// gmBoss5EfctLeakagePartProcMain
/*!
  漏電エフェクト メイン更新処理関数
  
  @param obj_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5EfctLeakagePartProcMain(OBS_OBJECT_WORK *obj_work)
{
	// 予備漏電ループ処理を流用
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	// 座標更新
	GmBsCmnUpdateObject3DESStuckWithNodeRelative(obj_work,
												 &parent_body->snm_work,
												 parent_body->body_snm_reg_id,
												 TRUE,
												 &obj_work->parent_obj->pos,
												 &parent_body->pivot_prev_pos);
	
	// 手前に表示する
	obj_work->pos.z	+= GMD_BOSS5_EFCT_PRELIM_LEAKAGE_OFST_Z;
	
	// 有効フラグがオフになったら消去開始
	if (!(parent_body->flag & GMD_BOSS5_BODY_FLAG_LEAKAGE_ACTIVE)) {
		ObjDrawKillAction3DES(obj_work);
		
		obj_work->ppFunc	= gmBoss5EfctLeakageProcFade;
	}
}

// =======================================================================
// gmBoss5EfctLeakageProcFade
/*!
  漏電エフェクト 更新処理 フェード
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctLeakageProcFade(OBS_OBJECT_WORK *obj_work)
{
	// 予備漏電フェード処理を流用
	// 再生終了時に消去
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

#else
// =======================================================================
// gmBoss5EfctCreateLeakage
/*!
  漏電エフェクト初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5EfctCreateLeakage(GMS_BOSS5_BODY_WORK *body_work)
{
	const Sint32	efct_idx_tbl[GMD_BOSS5_EFCT_LEAKAGE_PART_NUM]	= {
		GME_BOSS5_EFCT_IDX_BLITZ_FB_00,		// 胴体
		GME_BOSS5_EFCT_IDX_BLITZ_FB_C_00,	// 胴体
		GME_BOSS5_EFCT_IDX_BLITZ_FB_01,		// 左脚 足
		GME_BOSS5_EFCT_IDX_BLITZ_FB_02,		// 左脚 つま先
		GME_BOSS5_EFCT_IDX_BLITZ_FB_C_01,	// 左脚 足
		GME_BOSS5_EFCT_IDX_BLITZ_FB_01,		// 右脚 足
		GME_BOSS5_EFCT_IDX_BLITZ_FB_02,		// 右脚 つま先
		GME_BOSS5_EFCT_IDX_BLITZ_FB_C_01,	// 右脚 足
	};
	
	const Sint32	snm_id_tbl[GMD_BOSS5_EFCT_LEAKAGE_PART_NUM]	= {
		body_work->body_snm_reg_id,
		body_work->body_snm_reg_id,
		body_work->leg_snm_reg_ids[GME_BOSS5_LEG_TYPE_LEFT],
		body_work->lfoot_snm_reg_id,
		body_work->leg_snm_reg_ids[GME_BOSS5_LEG_TYPE_LEFT],
		body_work->leg_snm_reg_ids[GME_BOSS5_LEG_TYPE_RIGHT],
		body_work->rfoot_snm_reg_id,
		body_work->leg_snm_reg_ids[GME_BOSS5_LEG_TYPE_RIGHT],
	};
	
	// 各パーツの生成・初期化
	for (Sint32 i = 0; i < GMD_BOSS5_EFCT_LEAKAGE_PART_NUM; ++i) {
		GMS_BOSS5_EFCT_GENERAL_WORK	*lkg_part;
		
		lkg_part	= 
			(GMS_BOSS5_EFCT_GENERAL_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
															  efct_idx_tbl[i],
															  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
		
		// SE管理担当を設定
		if (i == 0) {
			lkg_part->user_flag	|= GMD_BOSS5_EFCT_LEAKAGE_USRFLAG_SE_RESPONSIBLE;
		}
		
		// 接着ノード設定
		lkg_part->ref_node_snm_id	= snm_id_tbl[i];
		
		// 本体用パーツだけ、より手前に表示させる
		if (lkg_part->ref_node_snm_id == body_work->body_snm_reg_id) {
			lkg_part->user_flag	|= GMD_BOSS5_EFCT_LEAKAGE_USRFLAG_IS_BODYPART;
		}
		
		// メイン処理設定
		GMM_BS_OBJ(lkg_part)->ppFunc	= gmBoss5EfctLeakagePartProcMain;
	}
}

// =======================================================================
// gmBoss5EfctLeakagePartProcMain
/*!
  漏電エフェクト メイン更新処理関数
  
  @param obj_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5EfctLeakagePartProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS5_EFCT_GENERAL_WORK	*lkg_part	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// SE管理担当処理
	if (lkg_part->user_flag & GMD_BOSS5_EFCT_LEAKAGE_USRFLAG_SE_RESPONSIBLE) {
		// 漏電SEが停止していたら再生しなおす
		if (GsSoundIsSeStop(parent_body->se_hnd_leakage)) {
			// 再生復帰
			GsSoundStopSeHandle(parent_body->se_hnd_leakage);
			GmSoundPlaySE("FinalBoss11", parent_body->se_hnd_leakage);
		}
	}
	
	// 座標更新
	GmBsCmnUpdateObject3DESStuckWithNodeRelative(obj_work,
												 &parent_body->snm_work,
												 lkg_part->ref_node_snm_id,
												 TRUE,
												 &obj_work->parent_obj->pos,
												 &parent_body->pivot_prev_pos);
	
	// 手前に表示する
	if (lkg_part->user_flag & GMD_BOSS5_EFCT_LEAKAGE_USRFLAG_IS_BODYPART) {
		// 本体用のパーツのオフセットは別の値を設定
		obj_work->pos.z	+= GMD_BOSS5_EFCT_LEAKAGE_BODYPART_OFST_Z;
	}
	else {
		obj_work->pos.z	+= GMD_BOSS5_EFCT_LEAKAGE_OFST_Z;
	}
	
	// 有効フラグがオフになったら消去開始
	if (!(parent_body->flag & GMD_BOSS5_BODY_FLAG_LEAKAGE_ACTIVE)) {
		if (!(lkg_part->flag & GMD_BOSS5_EFCT_GENERAL_FLAG_FADING)) {
			lkg_part->flag	|= GMD_BOSS5_EFCT_GENERAL_FLAG_FADING;
			ObjDrawKillAction3DES(obj_work);
		}
	}
	
	// 消去開始後、パーティクルが全て消えた時点でタスククリア
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctCreateLeakageVanish
/*!
  漏電消失エフェクト生成
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5EfctCreateLeakageVanish(GMS_BOSS5_BODY_WORK *body_work)
{
	const Sint32	efct_idx_tbl[GMD_BOSS5_EFCT_LEAKAGE_VANISH_PART_NUM]	= {
		GME_BOSS5_EFCT_IDX_BLITZ_FB_03,		//!< 胴体
		GME_BOSS5_EFCT_IDX_BLITZ_FB_C_02,	//!< 胴体
	};
	
	const Sint32	snm_id_tbl[GMD_BOSS5_EFCT_LEAKAGE_VANISH_PART_NUM]	= {
		body_work->body_snm_reg_id,
		body_work->body_snm_reg_id,
	};
	
	// 各パーツの生成・初期化
	for (Sint32 i = 0; i < GMD_BOSS5_EFCT_LEAKAGE_VANISH_PART_NUM; ++i) {
		GMS_BOSS5_EFCT_GENERAL_WORK	*lkg_vanish;
		
		lkg_vanish	= (GMS_BOSS5_EFCT_GENERAL_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
																		efct_idx_tbl[i],
																		sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
		lkg_vanish->ref_node_snm_id	= snm_id_tbl[i];
		
		GMM_BS_OBJ(lkg_vanish)->ppFunc	= gmBoss5EfctLeakageVanishProcMain;
	}
}

// =======================================================================
// gmBoss5EfctLeakageVanishProcMain
/*!
  漏電消失エフェクト メイン更新処理関数
  
  @param obj_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5EfctLeakageVanishProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS5_EFCT_GENERAL_WORK	*lkg_vanish	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 座標更新
	GmBsCmnUpdateObject3DESStuckWithNodeRelative(obj_work,
												 &parent_body->snm_work,
												 lkg_vanish->ref_node_snm_id,
												 TRUE,
												 &obj_work->parent_obj->pos,
												 &parent_body->pivot_prev_pos);
	
	// 手前に表示する
	obj_work->pos.z	+= GMD_BOSS5_EFCT_LEAKAGE_VANISH_OFST_Z;
	
	// 再生終了したらタスククリア
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}
#endif // _IPHONE

// =======================================================================
// gmBoss5EfctCreatePrelimLeakage
/*!
  予備漏電エフェクト 生成
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5EfctCreatePrelimLeakage(GMS_BOSS5_BODY_WORK *body_work)
{
	GMS_BOSS5_EFCT_GENERAL_WORK	*prelim_lkg;
	
	prelim_lkg	=
		(GMS_BOSS5_EFCT_GENERAL_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
														  GME_BOSS5_EFCT_IDX_BLITZ_FB_START,
														  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	GMM_BS_OBJ(prelim_lkg)->ppFunc	= gmBoss5EfctPrelimLeakageProcLoop;
}

// =======================================================================
// gmBoss5EfctPrelimLeakageProcLoop
/*!
  予備漏電エフェクト 更新処理 ループ
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctPrelimLeakageProcLoop(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	// 座標更新
	GmBsCmnUpdateObject3DESStuckWithNodeRelative(obj_work,
												 &parent_body->snm_work,
												 parent_body->body_snm_reg_id,
												 TRUE,
												 &obj_work->parent_obj->pos,
												 &parent_body->pivot_prev_pos);
	
	// 手前に表示する
	obj_work->pos.z	+= GMD_BOSS5_EFCT_PRELIM_LEAKAGE_OFST_Z;
	
	// 有効フラグがオフになったら消去開始
	if (!(parent_body->flag & GMD_BOSS5_BODY_FLAG_PRELIM_LEAKAGE_ACTIVE)) {
		ObjDrawKillAction3DES(obj_work);
		
		obj_work->ppFunc	= gmBoss5EfctPrelimLeakageProcFade;
	}
}

// =======================================================================
// gmBoss5EfctPrelimLeakageProcFade
/*!
  予備漏電エフェクト 更新処理 フェード
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctPrelimLeakageProcFade(OBS_OBJECT_WORK *obj_work)
{
	// 再生終了時に消去
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctBerserkStampSmokeProcWaitStart
/*!
  凶暴化演出時 踏み込み煙エフェクト 更新処理 開始待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctBerserkStampSmokeProcWaitStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_EFCT_GENERAL_WORK	*stamp_smoke	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 既定時間後に発生
	if (stamp_smoke->timer) {
		stamp_smoke->timer--;
	}
	else {
		// 生成位置設定
		{
			GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
			const NNS_MATRIX	*w_mtx;
			
			// 参照SNMノードのマトリクスを得る
			w_mtx	=
				GmBsCmnGetSNMMtx(&parent_body->snm_work,
								 stamp_smoke->ref_node_snm_id);
			
			// 座標設定
			obj_work->pos.x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3));
			obj_work->pos.y	= parent_body->ground_v_pos;	// 地面の高さに設定
			obj_work->pos.z	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 2, 3));
			
			// 手前に表示させる
			obj_work->pos.z	= GMD_BOSS5_EFCT_BERSERK_STAMP_SMOKE_OFST_Z;
		}
		
		// 更新・表示開始
		obj_work->disp_flag	&= ~(OBD_DISP_NOUPDATE | OBD_DISP_NODISP);
		
		obj_work->ppFunc	= GmEffectDefaultMainFuncDeleteAtEnd;
	}
}

// =======================================================================
// gmBoss5EfctCreateJet
/*!
  噴射エフェクト初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  カメラ側の噴射口にエフェクトを生成します。
 */
// =======================================================================
void gmBoss5EfctCreateJet(GMS_BOSS5_BODY_WORK *body_work)
{
	Sint32	targ_node_snm_reg_id;
	
	// カメラ側の噴射口だけエフェクトを付ける
	if (GMM_BS_OBJ(body_work)->disp_flag & OBD_DISP_HFLIP) {
		targ_node_snm_reg_id	= body_work->nozzle_snm_reg_ids[GME_BOSS5_NOZZLE_TYPE_LEFT];
	}
	else {
		targ_node_snm_reg_id	= body_work->nozzle_snm_reg_ids[GME_BOSS5_NOZZLE_TYPE_RIGHT];
	}
	
	// パーツの生成・初期化
	{
		GMS_BOSS5_EFCT_GENERAL_WORK	*jet_part;
		GMS_EFFECT_COM_WORK	*efct_com;
		OBS_OBJECT_WORK	*obj_work;
		
		jet_part	=
			(GMS_BOSS5_EFCT_GENERAL_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
															   GME_BOSS5_EFCT_IDX_JET_FB,
															   sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
		
		efct_com	= (GMS_EFFECT_COM_WORK*)jet_part;
		obj_work	= GMM_BS_OBJ(jet_part);
#if _IPHONE
		obj_work->obj_3des->ecb->drawObjState = OBD_DRAW_CMD_STATE_3DNN; // 描画コマンドを通常へ
#endif // _IPHONE
		
		// 接着ノード設定
		jet_part->ref_node_snm_id	= targ_node_snm_reg_id;
		
		// 当たり設定
		GmBsCmnSetEfctAtkVsPly((GMS_EFFECT_COM_WORK*)obj_work,
							   GMD_BOSS5_EFCT_JET_VIEWOUT_OFST);
		obj_work->flag	|= OBD_OBJECT_NOCLIP;	// クリッピングしない
		ObjRectWorkSet(&efct_com->rect_work[GME_EFFECT_RECT_ATK],
					   GMD_BOSS5_EFCT_JET_ATK_RECT_SIZE_LEFT,
					   GMD_BOSS5_EFCT_JET_ATK_RECT_SIZE_TOP,
					   GMD_BOSS5_EFCT_JET_ATK_RECT_SIZE_RIGHT,
					   GMD_BOSS5_EFCT_JET_ATK_RECT_SIZE_BOTTOM);
		efct_com->rect_work[GME_EFFECT_RECT_ATK].flag	|= OBD_RECT_NOHIT;	// 最初は無効
		
		// 攻撃矩形発生待ち時間設定
		jet_part->timer	= GMD_BOSS5_EFCT_JET_ATK_ACTIVATE_TIMING_FRAME;
		
		// 噴射SEタイマ設定
		GmBoss5Init1ShotTimer(&jet_part->se_timer,
							  GMD_BOSS5_EFCT_SE_JET_START_WAIT_TIME);
		
		// メイン処理設定
		GMM_BS_OBJ(jet_part)->ppFunc	= gmBoss5EfctJetProcMain;
	}
}

// =======================================================================
// gmBoss5EfctJetProcMain
/*!
  噴射エフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctJetProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS5_EFCT_GENERAL_WORK	*jet_part	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 噴射SE再生
	if (GmBoss5Update1ShotTimer(&jet_part->se_timer)) {
		GmSoundPlaySE("FinalBoss04");
	}
	
	// 既定時間経過したら当たりを有効化
	if (jet_part->timer) {
		jet_part->timer--;
	}
	else {
		GMS_EFFECT_COM_WORK	*efct_com	= (GMS_EFFECT_COM_WORK*)obj_work;
		efct_com->rect_work[GME_EFFECT_RECT_ATK].flag	&= ~OBD_RECT_NOHIT;
	}
	
	// 座標更新
	GmBsCmnUpdateObject3DESStuckWithNodeRelative(obj_work,
												 &parent_body->snm_work,
												 jet_part->ref_node_snm_id,
												 TRUE,
												 &obj_work->parent_obj->pos,
												 &parent_body->pivot_prev_pos);
	
	// 有効フラグがオフになったら消去開始
	if (!(parent_body->flag & GMD_BOSS5_BODY_FLAG_JET_ACTIVE)) {
		if (!(jet_part->flag & GMD_BOSS5_EFCT_GENERAL_FLAG_FADING)) {
			jet_part->flag	|= GMD_BOSS5_EFCT_GENERAL_FLAG_FADING;
			ObjDrawKillAction3DES(obj_work);
		}
	}
	
	// 消去開始後、パーティクルが全て消えた時点でタスククリア
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctCreateJetSmoke
/*!
  噴射スモークエフェクト初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
 */
// =======================================================================
void gmBoss5EfctCreateJetSmoke(GMS_BOSS5_BODY_WORK *body_work)
{
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*jet_smoke;
	
	jet_smoke	=
		(GMS_BOSS5_EFCT_GENERAL_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
														  GME_BOSS5_EFCT_IDX_JET_FB_SMORK,
														  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	efct_com	= (GMS_EFFECT_COM_WORK*)jet_smoke;
	obj_work	= GMM_BS_OBJ(jet_smoke);
	
	
	// 生成位置設定
	{
		Sint32	targ_node_snm_reg_id;
		const NNS_MATRIX	*w_mtx;
		
		// カメラ側の噴射口のSNM登録IDを得る
		if (GMM_BS_OBJ(body_work)->disp_flag & OBD_DISP_HFLIP) {
			targ_node_snm_reg_id	= body_work->nozzle_snm_reg_ids[GME_BOSS5_NOZZLE_TYPE_LEFT];
		}
		else {
			targ_node_snm_reg_id	= body_work->nozzle_snm_reg_ids[GME_BOSS5_NOZZLE_TYPE_RIGHT];
		}
		
		w_mtx	= GmBsCmnGetSNMMtx(&body_work->snm_work, targ_node_snm_reg_id);
		
		// 座標設定
		obj_work->pos.x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3));	// 噴射口のX成分
		obj_work->pos.y	= body_work->ground_v_pos;	// 地面の高さに設定
		obj_work->pos.z	= GMM_BS_OBJ(body_work)->pos.z;
	}
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss5EfctJetSmokeProcMain;
}

// =======================================================================
// gmBoss5EfctJetSmokeProcMain
/*!
  噴射スモークエフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctJetSmokeProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS5_EFCT_GENERAL_WORK	*jet_smoke	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 有効フラグがオフになったら消去開始
	if (!(parent_body->flag & GMD_BOSS5_BODY_FLAG_JETSMOKE_ACTIVE)) {
		if (!(jet_smoke->flag & GMD_BOSS5_EFCT_GENERAL_FLAG_FADING)) {
			jet_smoke->flag	|= GMD_BOSS5_EFCT_GENERAL_FLAG_FADING;
			ObjDrawKillAction3DES(obj_work);
		}
	}
	
	// 消去開始後、パーティクルが全て消えた時点でタスククリア
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctCreateRocketLeakage
/*!
  ロケット漏電エフェクト初期化
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void gmBoss5EfctCreateRocketLeakage(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	GMS_BOSS5_EFCT_GENERAL_WORK	*rkt_lkg;
	
	rkt_lkg	= 
		(GMS_BOSS5_EFCT_GENERAL_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(rkt_work),
														  GME_BOSS5_EFCT_IDX_ROCKET_BLITZ,
														  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	// オフセットマトリクス
	nnMakeTranslateMatrix(&rkt_lkg->ofst_mtx,
						  GMD_BOSS5_EFCT_ROCKET_LEAKAGE_STUCK_ND_OFST_X, 0, 0);
	
	// SEハンドル確保
	rkt_lkg->se_handle	= GsSoundAllocSeHandle();
	
	// 終了処理差し替え
	mtTaskChangeTcbDestructor(((OBS_OBJECT_WORK*)rkt_lkg)->tcb,
							  gmBoss5EfctRocketLeakageExit);
	
	// 電撃SE再生
	GmSoundPlaySE("FinalBoss11", rkt_lkg->se_handle);
	
	// メイン処理設定
	GMM_BS_OBJ(rkt_lkg)->ppFunc	= gmBoss5EfctRocketLeakageProcMain;
}

// =======================================================================
// gmBoss5EfctRocketLeakageExit
/*!
  ロケット漏電エフェクト 終了処理関数
 */
// =======================================================================
void gmBoss5EfctRocketLeakageExit(MTS_TASK_TCB *tcb)
{
	GMS_BOSS5_EFCT_GENERAL_WORK	*rkt_lkg	= (GMS_BOSS5_EFCT_GENERAL_WORK*)mtTaskGetTcbWork(tcb);
	
	// SEハンドル解放
	GsSoundFreeSeHandle(rkt_lkg->se_handle);
	
	// デフォルト終了処理
	GmEffectDefaultExit(tcb);
}

// =======================================================================
// gmBoss5EfctRocketLeakageProcMain
/*!
  ロケット漏電エフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctRocketLeakageProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_ROCKET_WORK	*parent_rkt	= (GMS_BOSS5_ROCKET_WORK*)obj_work->parent_obj;
	GMS_BOSS5_EFCT_GENERAL_WORK	*rkt_lkg	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 座標更新
	GmBsCmnUpdateObject3DESStuckWithNodeRelative(obj_work,
												 &parent_rkt->snm_work,
												 parent_rkt->drill_snm_reg_id,
												 TRUE,
												 &obj_work->parent_obj->pos,
												 &parent_rkt->pivot_prev_pos,
												 &rkt_lkg->ofst_mtx);
	
	// 手前に表示する
	obj_work->pos.z	+= GMD_BOSS5_EFCT_ROCKET_LEAKAGE_OFST_Z;
	
	// 有効フラグがオフになったら消去開始
	if (!(parent_rkt->flag & GMD_BOSS5_RKT_FLAG_LEAKAGE_ACTIVE)) {
		if (!(rkt_lkg->flag & GMD_BOSS5_EFCT_GENERAL_FLAG_FADING)) {
			rkt_lkg->flag	|= GMD_BOSS5_EFCT_GENERAL_FLAG_FADING;
			ObjDrawKillAction3DES(obj_work);
		}
	}
	
	// 消去開始後、パーティクルが全て消えた時点でタスククリア
	if (obj_work->disp_flag & OBD_DISP_END) {
		// 電撃SE停止（リリースタイムが設定されているので即時停止）
		GsSoundStopSeHandle(rkt_lkg->se_handle);
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctRocketDockProcMain
/*!
  ロケットドッキングエフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctRocketDockProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS5_EFCT_GENERAL_WORK	*rkt_dock	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 座標更新
	GmBsCmnUpdateObject3DESStuckWithNodeRelative(obj_work,
												 &parent_body->snm_work,
												 rkt_dock->ref_node_snm_id,
												 TRUE,
												 &obj_work->parent_obj->pos,
												 &parent_body->pivot_prev_pos,
												 &rkt_dock->ofst_mtx);
	
	// 手前に表示する
	obj_work->pos.z	+= GMD_BOSS5_EFCT_ROCKET_DOCK_OFST_Z;
	
	// 終了チェック
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctCreateRocketJet
/*!
  ロケット噴射エフェクト初期化
  
  @param rkt_work	[io]	ロケットワーク
  @param is_rev_jet	[in]	逆噴射バージョン
 */
// =======================================================================
void gmBoss5EfctCreateRocketJet(GMS_BOSS5_ROCKET_WORK *rkt_work, BOOL is_rev_jet)
{
	GMS_BOSS5_EFCT_GENERAL_WORK	*rkt_jet;
	
	rkt_jet	=
		(GMS_BOSS5_EFCT_GENERAL_WORK*)gmBoss5EfctEsCreate(GMM_BS_OBJ(rkt_work),
														  GME_BOSS5_EFCT_IDX_ROCKET_JET,
														  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	// タイプ別設定
	if (is_rev_jet) {
		
		// オフセットマトリクスを逆噴射用の姿勢に設定
		// 先端寄り
		nnMakeTranslateMatrix(&rkt_jet->ofst_mtx, GMD_BOSS5_EFCT_ROCKET_JET_REV_STUCK_ND_OFST_X, 0, 0);
		// 反対向き
		nnRotateYMatrix(&rkt_jet->ofst_mtx, &rkt_jet->ofst_mtx, AKM_DEGtoA32(180));
		
		// 逆噴射タイプ設定
		rkt_jet->user_flag	|= GMD_BOSS5_EFCT_ROCKET_JET_USRFLAG_IS_REVERSE;
	}
	else {
		// オフセットマトリクス設定
		nnMakeTranslateMatrix(&rkt_jet->ofst_mtx, GMD_BOSS5_EFCT_ROCKET_JET_STUCK_ND_OFST_X, 0, 0);
	}
	
	// メイン処理設定
	GMM_BS_OBJ(rkt_jet)->ppFunc	= gmBoss5EfctRocketJetProcMain;
}

// =======================================================================
// gmBoss5EfctRocketJetProcMain
/*!
  ロケット噴射エフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctRocketJetProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_ROCKET_WORK	*parent_rkt	= (GMS_BOSS5_ROCKET_WORK*)obj_work->parent_obj;
	GMS_BOSS5_EFCT_GENERAL_WORK	*rkt_jet	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	BOOL	is_inactive	= FALSE;
	
	// 座標更新
	GmBsCmnUpdateObject3DESStuckWithNodeRelative(obj_work,
												 &parent_rkt->snm_work,
												 parent_rkt->drill_snm_reg_id,
												 TRUE,
												 &obj_work->parent_obj->pos,
												 &parent_rkt->pivot_prev_pos,
												 &rkt_jet->ofst_mtx);
	
	// 手前に表示する
	obj_work->pos.z	+= GMD_BOSS5_EFCT_ROCKET_JET_OFST_Z;
	
	// 有効フラグがオフになったら消去開始
	if (rkt_jet->user_flag & GMD_BOSS5_EFCT_ROCKET_JET_USRFLAG_IS_REVERSE) {
		if (!(parent_rkt->flag & GMD_BOSS5_RKT_FLAG_JET_REV_ACTIVE)) {
			is_inactive	= TRUE;
		}
	}
	else {
		if (!(parent_rkt->flag & GMD_BOSS5_RKT_FLAG_JET_NML_ACTIVE)) {
			is_inactive	= TRUE;
		}
	}
	
	if (is_inactive && !(rkt_jet->flag & GMD_BOSS5_EFCT_GENERAL_FLAG_FADING)) {
		rkt_jet->flag	|= GMD_BOSS5_EFCT_GENERAL_FLAG_FADING;
		ObjDrawKillAction3DES(obj_work);
	}
	
	// 消去開始後、パーティクルが全て消えた時点でタスククリア
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctLandingShockwaveProcMain
/*!
  着地衝撃波エフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctLandingShockwaveProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_EFCT_GENERAL_WORK	*landing_sw	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 既定時間後に攻撃矩形を無効化
	if (landing_sw->timer) {
		landing_sw->timer--;
	}
	else {
		GMS_EFFECT_COM_WORK	*efct_com	= (GMS_EFFECT_COM_WORK*)obj_work;
		
		efct_com->rect_work[GME_EFFECT_RECT_ATK].flag	|= OBD_RECT_NOHIT;
		efct_com->rect_work[GME_EFFECT_RECT_ATK].flag	&= ~OBD_RECT_ENABLE;
	}
	
	// アニメーション終了時消去
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctStrikeShockwaveProcWaitStart
/*!
  地面突き衝撃波エフェクト 更新処理 開始待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctStrikeShockwaveProcWaitStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_EFCT_GENERAL_WORK	*strike_sw	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 既定時間後に発生
	if (strike_sw->timer) {
		strike_sw->timer--;
	}
	else {
		
		// 生成位置設定
		{
			GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
			const NNS_MATRIX	*w_mtx;
			
			// 右腕前腕のマトリクスを得る
			w_mtx	=
				GmBsCmnGetSNMMtx(&parent_body->snm_work,
								 parent_body->armpt_snm_reg_ids[GME_BOSS5_ARM_TYPE_RIGHT][GME_BOSS5_ARMPART_IDX_FOREARM]);
			
			// 座標設定
			obj_work->pos.x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3));	// 前腕のX成分
			obj_work->pos.y	= parent_body->ground_v_pos;	// 地面の高さに設定
			obj_work->pos.z	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 2, 3));	// 前腕のZ成分
			
			// 手前に表示させる
			obj_work->pos.z	+= GMD_BOSS5_EFCT_STRIKE_SW_OFST_Z;
		}
		
		// 当たり判定開始
		obj_work->flag	&= ~OBD_OBJECT_NOHIT;
		
		// 更新・表示開始
		obj_work->disp_flag	&= ~(OBD_DISP_NOUPDATE | OBD_DISP_NODISP);
		
		// 攻撃矩形有効時間
		strike_sw->timer	= GMD_BOSS5_EFCT_STRIKE_SW_ATK_RECT_ACTIVE_TIME;
		
		obj_work->ppFunc	= gmBoss5EfctStrikeShockwaveProcLoop;
	}
}

// =======================================================================
// gmBoss5EfctStrikeShockwaveProcLoop
/*!
  地面突き衝撃波エフェクト 更新処理 ループ
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctStrikeShockwaveProcLoop(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_EFCT_GENERAL_WORK *strike_sw	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 既定時間後に攻撃矩形を無効化
	if (strike_sw->timer) {
		strike_sw->timer--;
	}
	else {
		GMS_EFFECT_COM_WORK	*efct_com	= (GMS_EFFECT_COM_WORK*)obj_work;
		
		efct_com->rect_work[GME_EFFECT_RECT_ATK].flag	|= OBD_RECT_NOHIT;
		efct_com->rect_work[GME_EFFECT_RECT_ATK].flag	&= ~OBD_RECT_ENABLE;
	}
	
	// アニメーション終了時消去
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctCreateTargetCursorStart
/*!
  ターゲットカーソル開始エフェクト 生成
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5EfctCreateTargetCursorStart(GMS_BOSS5_BODY_WORK *body_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*targ_start;
	
	efct_work	= gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
									  GME_BOSS5_EFCT_IDX_TARGET_FB_S,
									  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	efct_com	= (GMS_EFFECT_COM_WORK*)efct_work;
	obj_work	= (OBS_OBJECT_WORK*)efct_com;
	targ_start	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 手前に表示させる
	// （表示対象が親とは別のオブジェクトなのでdisp_ofstで手前にずらしておく）
	GmEffect3DESAddDispOffset(efct_work, 0, 0, GMD_BOSS5_EFCT_TARGETCURSOR_DISP_OFST_Z);
	
	// 初期位置設定
	obj_work->pos	= GmBsCmnGetPlayerObj()->pos;
	
	// 開始エフェクト継続時間設定
	targ_start->timer	= GMD_BOSS5_EFCT_CURSOR_START_DURATION_TIME;
	
	// 初期再生速度設定
	obj_work->obj_3des->speed	= GMD_BOSS5_EFCT_CURSOR_START_INIT_UPDATE_SPD;
	
	// 非表示点滅処理初期化
	gmBoss5EfctTargetCursorInitFlickerNoDisp(targ_start);
	
	// 点滅用エフェクト生成
	gmBoss5EfctCreateTargetCursorFlash(efct_com, GME_BOSS5_EFCT_IDX_TARGET_FB_W_S);
	
	obj_work->ppFunc	= gmBoss5EfctTargetCursorStartProcMain;
}

// =======================================================================
// gmBoss5EfctTargetCursorStartProcMain
/*!
  ターゲットカーソル開始エフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctTargetCursorStartProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS5_EFCT_GENERAL_WORK	*targ_start	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	Float	ratio;
	
	MTM_ASSERT(parent_body);
	
	// プレイヤーを追いかける
	GmBoss5BodyGetPlySearchPos(parent_body, &obj_work->pos);
	
	// 再生速度更新
	ratio	= 1.f - ((Float)targ_start->timer / (Float)GMD_BOSS5_EFCT_CURSOR_START_DURATION_TIME);
	ratio	= MTM_MATH_CLIP(ratio, 0.f, 1.f);
	
	obj_work->obj_3des->speed	=
		GMD_BOSS5_EFCT_CURSOR_START_INIT_UPDATE_SPD +
			ratio * (GMD_BOSS5_EFCT_CURSOR_START_DEST_UPDATE_SPD - GMD_BOSS5_EFCT_CURSOR_START_INIT_UPDATE_SPD);
	
	// 点滅更新
	{
		Float	nodisp_time	= GMD_BOSS5_EFCT_CURSOR_START_FLICKER_ON_TIME / obj_work->obj_3des->speed;
		Float	cycle_time	=
			(GMD_BOSS5_EFCT_CURSOR_START_FLICKER_OFF_TIME + GMD_BOSS5_EFCT_CURSOR_START_FLICKER_ON_TIME)
				/ obj_work->obj_3des->speed;
		gmBoss5EfctTargetCursorUpdateFlickerNoDisp(targ_start, nodisp_time, cycle_time);
	}
	
	// 既定時間表示したら自分削除＆ループエフェクト生成
	if (targ_start->timer) {
		targ_start->timer--;
	}
	else {
		// ループエフェクト生成
		gmBoss5EfctCreateTargetCursorLoop(parent_body, &targ_start->efct_3des.efct_com);
		
		// 自分消去
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctCreateTargetCursorLoop
/*!
  ターゲットカーソルループエフェクト 生成
  
  @param body_work		[io]	本体ワーク
  @param former_efct	[in]	座標継承元エフェクト
 */
// =======================================================================
void gmBoss5EfctCreateTargetCursorLoop(GMS_BOSS5_BODY_WORK *body_work,
									   const GMS_EFFECT_COM_WORK *former_efct)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*targ_loop;
	
	efct_work	= gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
									  GME_BOSS5_EFCT_IDX_TARGET_FB,
									  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	efct_com	= (GMS_EFFECT_COM_WORK*)efct_work;
	obj_work	= (OBS_OBJECT_WORK*)efct_com;
	targ_loop	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 手前に表示させる
	// （表示対象が親とは別のオブジェクトなのでdisp_ofstで手前にずらしておく）
	GmEffect3DESAddDispOffset(efct_work, 0, 0, GMD_BOSS5_EFCT_TARGETCURSOR_DISP_OFST_Z);
	
	// 座標設定（座標を引き継ぐ）
	obj_work->pos	= ((OBS_OBJECT_WORK*)former_efct)->pos;
	
	// 初期再生速度設定
	obj_work->obj_3des->speed	= GMD_BOSS5_EFCT_CURSOR_LOOP_INIT_UPDATE_SPD;
	
	// 点滅用エフェクト生成
	gmBoss5EfctCreateTargetCursorFlash(efct_com, GME_BOSS5_EFCT_IDX_TARGET_FB_W);
	
	obj_work->ppFunc	= gmBoss5EfctTargetCursorLoopProcMain;
}

// =======================================================================
// gmBoss5EfctTargetCursorLoopProcMain
/*!
  ターゲットカーソルループエフェクト メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctTargetCursorLoopProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS5_EFCT_GENERAL_WORK	*targ_loop	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	MTM_ASSERT(parent_body);
	
	// プレイヤーを追いかける
	GmBoss5BodyGetPlySearchPos(parent_body, &obj_work->pos);
	
	// 再生速度更新
	obj_work->obj_3des->speed	+= GMD_BOSS5_EFCT_CURSOR_LOOP_UPDATE_SPD_ADD;
	obj_work->obj_3des->speed	= MTM_MATH_CLIP(obj_work->obj_3des->speed, 0.f, GMD_BOSS5_EFCT_CURSOR_LOOP_DEST_UPDATE_SPD);
	
	// 点滅更新
	{
		Float	nodisp_time	= GMD_BOSS5_EFCT_CURSOR_LOOP_FLICKER_ON_TIME / obj_work->obj_3des->speed;
		Float	cycle_time	=
			(GMD_BOSS5_EFCT_CURSOR_LOOP_FLICKER_OFF_TIME + GMD_BOSS5_EFCT_CURSOR_LOOP_FLICKER_ON_TIME)
				/ obj_work->obj_3des->speed;
		gmBoss5EfctTargetCursorUpdateFlickerNoDisp(targ_loop, nodisp_time, cycle_time);
	}
	
	// 表示中フラグがオフになったら自分削除＆消失エフェクト生成
	if (!(parent_body->flag & GMD_BOSS5_BODY_FLAG_TARGET_ON)) {
		
		// 消失エフェクト生成
		gmBoss5EfctCreateTargetCursorEnd(parent_body, (GMS_EFFECT_COM_WORK*)obj_work);
		
		// 自分消去
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctCreateTargetCursorEnd
/*!
  ターゲットカーソル消失エフェクト 生成
  
  @param body_work		[io]	本体ワーク
  @param former_efct	[in]	座標継承元エフェクト
 */
// =======================================================================
void gmBoss5EfctCreateTargetCursorEnd(GMS_BOSS5_BODY_WORK *body_work,
									  const GMS_EFFECT_COM_WORK *former_efct)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*targ_end;
	
	efct_work	= gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
									  GME_BOSS5_EFCT_IDX_TARGET_FB_E,
									  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	efct_com	= (GMS_EFFECT_COM_WORK*)efct_work;
	obj_work	= (OBS_OBJECT_WORK*)efct_com;
	targ_end	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 手前に表示させる
	// （表示対象が親とは別のオブジェクトなのでdisp_ofstで手前にずらしておく）
	GmEffect3DESAddDispOffset(efct_work, 0, 0, GMD_BOSS5_EFCT_TARGETCURSOR_DISP_OFST_Z);
	
	// 座標設定（座標を引き継ぐ）
	obj_work->pos	= ((OBS_OBJECT_WORK*)former_efct)->pos;
	
	// 点滅用エフェクト生成
	gmBoss5EfctCreateTargetCursorFlash(efct_com, GME_BOSS5_EFCT_IDX_TARGET_FB_W_E);
	
	obj_work->ppFunc	= gmBoss5EfctTargetCursorEndProcMain;
}

// =======================================================================
// gmBoss5EfctTargetCursorEndProcMain
/*!
  ターゲットカーソル消失エフェクト メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctTargetCursorEndProcMain(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctCreateTargetCursorFlash
/*!
  ターゲットカーソル点滅用エフェクト 生成
  
  @param parent_efct	[io]	親エフェクト
  @param efct_idx		[in]	エフェクトインデックス(GME_BOSS5_EFCT_IDX_TARGET_FB_W_XXXX)
  
  @note
  点滅用の白バージョンターゲットカーソルを生成します。
  親のNODISPフラグを見て自分の表示・非表示を切り替えます。
 */
// =======================================================================
void gmBoss5EfctCreateTargetCursorFlash(GMS_EFFECT_COM_WORK *parent_efct, GME_BOSS5_EFCT_IDX efct_idx)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	OBS_OBJECT_WORK	*obj_work;
	
#if defined(MTD_DEBUG)
	switch (efct_idx) {
	case GME_BOSS5_EFCT_IDX_TARGET_FB_W_E: // no break
	case GME_BOSS5_EFCT_IDX_TARGET_FB_W_S: // no break
	case GME_BOSS5_EFCT_IDX_TARGET_FB_W:
		break;
	default:
		MTM_ASSERT(FALSE);
	}
#endif /* defined(MTD_DEBUG) */
	
	// エフェクト生成
	efct_work	= gmBoss5EfctEsCreate((OBS_OBJECT_WORK*)parent_efct, efct_idx);
	
	obj_work	= (OBS_OBJECT_WORK*)efct_work;
	
	// 最初は表示しない
	obj_work->disp_flag	|= OBD_DISP_NODISP;
	
	// 手前に表示させる
	GmEffect3DESAddDispOffset(efct_work, 0, 0, GMD_BOSS5_EFCT_TARGETCURSOR_DISP_OFST_Z);
	
	// 親再生速度コピー
	obj_work->obj_3des->speed	= ((OBS_OBJECT_WORK*)parent_efct)->obj_3des->speed;
	
	obj_work->ppFunc	= gmBoss5EfctTargetCursorFlashProcMain;
}

// =======================================================================
// gmBoss5EfctTargetCursorFlashProcMain
/*!
  ターゲットカーソル点滅用エフェクト メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctTargetCursorFlashProcMain(OBS_OBJECT_WORK *obj_work)
{
	// 親再生速度コピー
	obj_work->obj_3des->speed	= obj_work->parent_obj->obj_3des->speed;
	
	// 親が表示されていたら自分を表示v.v.
	if (obj_work->parent_obj->disp_flag & OBD_DISP_NODISP) {
		obj_work->disp_flag	&= ~OBD_DISP_NODISP;
	}
	else {
		obj_work->disp_flag	|= OBD_DISP_NODISP;
	}
	
	/* 親が消去されたら消去されます */
}

// =======================================================================
// gmBoss5EfctTargetCursorInitFlickerNoDisp
/*!
  非表示点滅処理 初期化
  
  @param targ_cursor	[io]	ターゲットカーソル用汎用ワーク
 */
// =======================================================================
void gmBoss5EfctTargetCursorInitFlickerNoDisp(GMS_BOSS5_EFCT_GENERAL_WORK *targ_cursor)
{
	targ_cursor->ratio_timer	= 0.f;
}

// =======================================================================
// gmBoss5EfctTargetCursorUpdateFlickerNoDisp
/*!
  非表示点滅処理 更新
  
  @param targ_cursor	[io]	ターゲットカーソル用汎用ワーク
  @param nodisp_time	[in]	非表示時間
  @param cycle_time		[in]	表示・非表示時間を合わせた1周期の時間
 */
// =======================================================================
void gmBoss5EfctTargetCursorUpdateFlickerNoDisp(GMS_BOSS5_EFCT_GENERAL_WORK *targ_cursor,
												Float nodisp_time, Float cycle_time)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)targ_cursor;
	
	targ_cursor->ratio_timer	+= 1.f;
	
	if (targ_cursor->ratio_timer >= cycle_time) {
		targ_cursor->ratio_timer	= 0.f;
	}
	
	if (targ_cursor->ratio_timer >= nodisp_time) {
		obj_work->disp_flag	&= ~OBD_DISP_NODISP;
	}
	else {
		obj_work->disp_flag	|= OBD_DISP_NODISP;
	}
}

// =======================================================================
// gmBoss5EfctCreateCrashCursorStart
/*!
  地球割り用ターゲットカーソル開始エフェクト 生成
  
  @param body_work		[io]	本体ワーク
  @param pos_x			[in]	停止目標位置
  @param duration_time	[in]	継続時間
 */
// =======================================================================
void gmBoss5EfctCreateCrashCursorStart(GMS_BOSS5_BODY_WORK *body_work,
									   fx32 pos_x, Uint32 duration_time)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*ctarg_start;
	
	efct_work	= gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
									  GME_BOSS5_EFCT_IDX_TARGET_FB_S,
									  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	efct_com	= (GMS_EFFECT_COM_WORK*)efct_work;
	obj_work	= (OBS_OBJECT_WORK*)efct_com;
	ctarg_start	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 手前に表示させる
	// （表示対象が親とは別のオブジェクトなのでdisp_ofstで手前にずらしておく）
	GmEffect3DESAddDispOffset(efct_work, 0, 0, GMD_BOSS5_EFCT_TARGETCURSOR_DISP_OFST_Z);
	
	// 目標座標保存
	ctarg_start->user_work	= (Uint32)pos_x;
	
	// 継続タイマ設定
	ctarg_start->timer	= duration_time;
	
	// 継続時間保存
	obj_work->user_work	= duration_time;
	
	// カーソルの移動方向決定
	if (mtMathRand() & 0x01) {
		ctarg_start->user_flag	= TRUE;
	}
	else {
		ctarg_start->user_flag	= FALSE;
	}
	
	// 初期座標設定
	gmBoss5EfctCrashCursorStartSetCurPos(ctarg_start, 0.f, (BOOL)ctarg_start->user_flag);	// 初期水平位置設定
	obj_work->pos.y	= body_work->ground_v_pos + GMD_BOSS5_EFCT_CRASH_CURSOR_OFST_Y_FROM_GROUND;
	obj_work->pos.z	= GmBsCmnGetPlayerObj()->pos.z;
	
	// 非表示点滅処理初期化
	gmBoss5EfctTargetCursorInitFlickerNoDisp(ctarg_start);
	
	// 点滅用エフェクト生成
	gmBoss5EfctCreateTargetCursorFlash(efct_com, GME_BOSS5_EFCT_IDX_TARGET_FB_W_S);
	
	obj_work->ppFunc	= gmBoss5EfctCrashCursorStartProcMain;
}

// =======================================================================
// gmBoss5EfctCrashCursorStartProcMain
/*!
  地球割り用ターゲットカーソル開始エフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctCrashCursorStartProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS5_EFCT_GENERAL_WORK	*ctarg_start	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 点滅更新
	gmBoss5EfctTargetCursorUpdateFlickerNoDisp(ctarg_start,
											   GMD_BOSS5_EFCT_CRASH_CURSOR_FLICKER_ON_TIME,
											   (GMD_BOSS5_EFCT_CRASH_CURSOR_FLICKER_ON_TIME +
												GMD_BOSS5_EFCT_CRASH_CURSOR_FLICKER_OFF_TIME));
	
	// 既定時間表示したら自分削除＆ループエフェクト生成
	if (ctarg_start->timer) {
		ctarg_start->timer--;
		
		// 線形補間ratioを円の部分曲線にマッピング
		// （単位円270deg～360degのY値[-1,0]を[0,1]にずらして、その内の[0,?]を[0,1]にマップ）
		Float	ratio	= 1.f - ((Float)ctarg_start->timer / (Float)obj_work->user_work);
		ratio	=
			(1.f - nnSin(nnArcCos(ratio * GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_RATIO_CURVE_RANGE_FACTOR)))
				/ (1.f - nnSin(nnArcCos(GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_RATIO_CURVE_RANGE_FACTOR)));
		ratio	= MTM_MATH_CLIP(ratio, 0.f, 1.f);
		
		// 水平位置更新
		gmBoss5EfctCrashCursorStartSetCurPos(ctarg_start, ratio,
											 (BOOL)ctarg_start->user_flag);
	}
	else {
		// 水平位置を目標位置に設定
		gmBoss5EfctCrashCursorStartSetCurPos(ctarg_start, 1.f, (BOOL)ctarg_start->user_flag);
		
		// ループエフェクト生成
		gmBoss5EfctCreateCrashCursorLoop(parent_body, obj_work->pos.x);
		
		// 自分消去
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctCrashCursorStartSetCurPos
/*!
  地球割り用ターゲットカーソル開始エフェクト 現在の座標を設定
  
  @param ctarg_start	[io]	汎用ワーク
  @param ratio			[in]	進捗度(0.f→初期位置, 1.f→目標位置)
  @param app_dir_left	[in]	停止直前の移動方向（TRUE:左方向, FALSE:右方向）
  
  @note
  進捗度から現在の水平座標を計算して、設定します。
 */
// =======================================================================
void gmBoss5EfctCrashCursorStartSetCurPos(GMS_BOSS5_EFCT_GENERAL_WORK *ctarg_start, Float ratio, BOOL app_dir_left)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)ctarg_start;
	
	MTM_ASSERT(GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_LEFT_EDGE_OFST <= GMD_BOSS5_BODY_CRASH_POINT_A_POS_OFST_X);
	MTM_ASSERT(GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_RIGHT_EDGE_OFST >= GMD_BOSS5_BODY_CRASH_POINT_C_POS_OFST_X);
	
	fx32	left_edge	= GMM_BOSS5_AREA_CENTER_X() + GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_LEFT_EDGE_OFST;
	fx32	right_edge	= GMM_BOSS5_AREA_CENTER_X() + GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_RIGHT_EDGE_OFST;
	
	fx32	dest_phase_ofst	= GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_WIDTH -
		(fx32)(FX32_ONE * fmod(FX_FX32_TO_F32(GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_DISTANCE),
							   FX_FX32_TO_F32(GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_WIDTH)));
	
	fx32	dest_pos_x	= (fx32)ctarg_start->user_work;
	// カーソルの停止目標地点がカーソルの移動範囲外の場合は範囲内に無理やり収める
	dest_pos_x	= MTM_MATH_CLIP(dest_pos_x, left_edge, right_edge);
	
	// 三角波の初期位相を決定
	if (app_dir_left) {
		// 最後は左移動
		dest_phase_ofst	+= right_edge - dest_pos_x;
	}
	else {
		// 最後は右移動
		dest_phase_ofst	+= GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_WIDTH;
		dest_phase_ofst	+= dest_pos_x - left_edge;
	}
	
	// 移動範囲の左端を変位0,移動範囲幅を振幅として、
	// 三角波によって範囲内の往復移動の現在地を算出
	{
		fx32	phase;
		fx32	edge_ofst_x;	//! 現在地の左右端からのオフセット（絶対値）
		phase	= (fx32)(GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_DISTANCE * ratio) + dest_phase_ofst;
		edge_ofst_x	= (fx32)(FX32_ONE * fmod(FX_FX32_TO_F32(phase), FX_FX32_TO_F32(GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_WIDTH)));
		
		if ((FX_Div(phase, GMD_BOSS5_EFCT_CRASH_CURSOR_MOVE_WIDTH) >> FX32_SHIFT) & 0x01) {
			// 往路
			obj_work->pos.x	= right_edge - edge_ofst_x;
		}
		else {
			// 復路
			obj_work->pos.x	= left_edge + edge_ofst_x;
		}
 	}
}

// =======================================================================
// gmBoss5EfctCreateCrashCursorLoop
/*!
  地球割り用ターゲットカーソルループエフェクト 生成
  
  @param body_work	[io]	本体ワーク
  @param pos_x		[in]	生成X位置
 */
// =======================================================================
void gmBoss5EfctCreateCrashCursorLoop(GMS_BOSS5_BODY_WORK *body_work, fx32 pos_x)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*ctarg_loop;
	
	efct_work	= gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
									  GME_BOSS5_EFCT_IDX_TARGET_FB,
									  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	efct_com	= (GMS_EFFECT_COM_WORK*)efct_work;
	obj_work	= (OBS_OBJECT_WORK*)efct_com;
	ctarg_loop	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 手前に表示させる
	// （表示対象が親とは別のオブジェクトなのでdisp_ofstで手前にずらしておく）
	GmEffect3DESAddDispOffset(efct_work, 0, 0, GMD_BOSS5_EFCT_TARGETCURSOR_DISP_OFST_Z);
	
	// 座標設定
	obj_work->pos.x	= pos_x;
	obj_work->pos.y	= body_work->ground_v_pos + GMD_BOSS5_EFCT_CRASH_CURSOR_OFST_Y_FROM_GROUND;
	obj_work->pos.z	= GmBsCmnGetPlayerObj()->pos.z;
	
	// 非表示点滅処理初期化
	gmBoss5EfctTargetCursorInitFlickerNoDisp(ctarg_loop);
	
	// 点滅用エフェクト生成
	gmBoss5EfctCreateTargetCursorFlash(efct_com, GME_BOSS5_EFCT_IDX_TARGET_FB_W);
	
	obj_work->ppFunc	= gmBoss5EfctCrashCursorLoopProcMain;
}

// =======================================================================
// gmBoss5EfctCrashCursorLoopProcMain
/*!
  地球割り用ターゲットカーソルループエフェクト メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctCrashCursorLoopProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS5_EFCT_GENERAL_WORK	*ctarg_loop	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	MTM_ASSERT(parent_body);
	
	// 点滅更新
	gmBoss5EfctTargetCursorUpdateFlickerNoDisp(ctarg_loop,
											   GMD_BOSS5_EFCT_CRASH_CURSOR_FLICKER_ON_TIME,
											   (GMD_BOSS5_EFCT_CRASH_CURSOR_FLICKER_ON_TIME +
												GMD_BOSS5_EFCT_CRASH_CURSOR_FLICKER_OFF_TIME));
	
	// 通常のターゲットカーソル用のフラグを共有する
	if (!(parent_body->flag & GMD_BOSS5_BODY_FLAG_TARGET_ON)) {
		
		// 消失エフェクト生成
		gmBoss5EfctCreateCrashCursorEnd(parent_body, (GMS_EFFECT_COM_WORK*)obj_work);
		
		// 自分消去
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctCreateCrashCursorEnd
/*!
  地球割り用ターゲットカーソル消失エフェクト 生成
  
  @param body_work		[io]	本体ワーク
  @param former_efct	[in]	座標継承元エフェクト
 */
// =======================================================================
void gmBoss5EfctCreateCrashCursorEnd(GMS_BOSS5_BODY_WORK *body_work,
									 const GMS_EFFECT_COM_WORK *former_efct)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	GMS_EFFECT_COM_WORK	*efct_com;
	OBS_OBJECT_WORK	*obj_work;
	GMS_BOSS5_EFCT_GENERAL_WORK	*targ_end;
	
	efct_work	= gmBoss5EfctEsCreate(GMM_BS_OBJ(body_work),
									  GME_BOSS5_EFCT_IDX_TARGET_FB_E,
									  sizeof(GMS_BOSS5_EFCT_GENERAL_WORK));
	
	efct_com	= (GMS_EFFECT_COM_WORK*)efct_work;
	obj_work	= (OBS_OBJECT_WORK*)efct_com;
	targ_end	= (GMS_BOSS5_EFCT_GENERAL_WORK*)obj_work;
	
	// 手前に表示させる
	// （表示対象が親とは別のオブジェクトなのでdisp_ofstで手前にずらしておく）
	GmEffect3DESAddDispOffset(efct_work, 0, 0, GMD_BOSS5_EFCT_TARGETCURSOR_DISP_OFST_Z);
	
	// 座標設定（座標を引き継ぐ）
	obj_work->pos	= ((OBS_OBJECT_WORK*)former_efct)->pos;
	
	obj_work->ppFunc	= gmBoss5EfctCrashCursorEndProcMain;
}

// =======================================================================
// gmBoss5EfctCrashCursorEndProcMain
/*!
  ターゲットカーソル消失エフェクト メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctCrashCursorEndProcMain(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctVulcanBulletProcMain
/*!
  バルカン弾エフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctVulcanBulletProcMain(OBS_OBJECT_WORK *obj_work)
{
	// エリア端に到達したらクリッピングを有効にする
	// （発射元が画面外だと直ぐに消えてしまう問題への対処）
	if (obj_work->pos.x <= GMM_BOSS5_AREA_LEFT() ||
		obj_work->pos.y <= GMM_BOSS5_AREA_TOP() ||
		obj_work->pos.x >= GMM_BOSS5_AREA_RIGHT() ||
		obj_work->pos.y >= GMM_BOSS5_AREA_BOTTOM()) {
		
		obj_work->flag	&= ~OBD_OBJECT_NOCLIP;
	}
	
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctCreateRocketSmoke
/*!
  ロケット黒煙エフェクト 生成
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void gmBoss5EfctCreateRocketSmoke(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	GMS_EFFECT_3DES_WORK	*efct_3des;
	efct_3des	=
		GmEfctBossCmnEsCreate(GMM_BS_OBJ(rkt_work),
							  GME_EFCT_BOSS_CMN_IDX_BOSS_SMORK);
	
	// 表示オフセット設定
	GmEffect3DESSetDispOffset(efct_3des, 0, 0, 0);	// とりあえず中心に配置
	
	// 上に向けて発生させる
	GmEffect3DESSetDispRotation(efct_3des, AKM_DEGtoA32(0), 0, 0);
	
	// メイン処理設定
	GMM_BS_OBJ(efct_3des)->ppFunc	= gmBoss5EfctRocketSmokeProcLoop;
}

// =======================================================================
// gmBoss5EfctRocketSmokeProcLoop
/*!
  ロケット黒煙エフェクト 更新処理 ループ
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctRocketSmokeProcLoop(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_ROCKET_WORK	*parent_rkt	= (GMS_BOSS5_ROCKET_WORK*)obj_work->parent_obj;
	
	// 有効フラグがオフになったら消去開始
	if (!(parent_rkt->flag & GMD_BOSS5_RKT_FLAG_SMOKE_ACTIVE)) {
		ObjDrawKillAction3DES(obj_work);
		
		obj_work->ppFunc	= gmBoss5EfctRocketSmokeProcFade;
	}
}

// =======================================================================
// gmBoss5EfctRocketSmokeProcFade
/*!
  ロケット黒煙エフェクト 更新処理 フェード
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctRocketSmokeProcFade(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctBreakdownSmokeProcLoop
/*!
  機能停止黒煙エフェクト 更新処理 ループ
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctBreakdownSmokeProcLoop(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	// 座標更新
	GmBsCmnUpdateObject3DESStuckWithNodeRelative(obj_work,
												 &parent_body->snm_work,
												 parent_body->body_snm_reg_id,
												 TRUE,
												 &obj_work->parent_obj->pos,
												 &parent_body->pivot_prev_pos);
	
	// 既定時間ループ
	if (obj_work->user_timer) {
		obj_work->user_timer--;
	}
	else {
		// パーティクル生成停止
		ObjDrawKillAction3DES(obj_work);
		
		obj_work->ppFunc	= gmBoss5EfctBreakdownSmokeProcFade;
	}
}

// =======================================================================
// gmBoss5EfctBreakdownSmokeProcFade
/*!
  機能停止黒煙エフェクト 更新処理 フェード
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctBreakdownSmokeProcFade(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5EfctCreateBodySmallSmoke
/*!
  本体 小さい黒煙エフェクト 生成
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5EfctCreateBodySmallSmoke(GMS_BOSS5_BODY_WORK *body_work, Uint32 part_idx)
{
	GMS_EFFECT_3DES_WORK	*efct_3des;
	OBS_OBJECT_WORK	*obj_work;
	
	efct_3des	=
		GmEfctBossCmnEsCreate(GMM_BS_OBJ(body_work),
							  GME_EFCT_BOSS_CMN_IDX_BOSS_SMOKE02);
	
	obj_work	= (OBS_OBJECT_WORK*)efct_3des;
	
	// オブジェクトの追随は自前で制御(StuckWithNode)
	// ＆ パーティクルをエミッタに追随させない
	GmEffect3DESChangeBase(efct_3des,
						   GME_EFFECT_3DES_POS_TYPE_EMT,
						   efct_3des->saved_init_flag & ~(GMD_EFFECT_3DES_FLAG_STICKPARENT));
	
	// パーツ番号を保存
	obj_work->user_work	= part_idx;
	
	// 表示オフセット設定
	GmEffect3DESSetDispOffset(efct_3des,
							  gm_boss5_efct_body_small_smoke_disp_ofst_tbl[part_idx][MTD_X],
							  gm_boss5_efct_body_small_smoke_disp_ofst_tbl[part_idx][MTD_Y],
							  gm_boss5_efct_body_small_smoke_disp_ofst_tbl[part_idx][MTD_Z]);
	
	// 表示回転設定
	GmEffect3DESSetDispRotation(efct_3des,
								gm_boss5_efct_body_small_smoke_disp_rot_tbl[part_idx][MTD_X],
								gm_boss5_efct_body_small_smoke_disp_rot_tbl[part_idx][MTD_Y],
								gm_boss5_efct_body_small_smoke_disp_rot_tbl[part_idx][MTD_Z]);
	
	// 既定範囲内でランダムな値をループ時間として設定
	obj_work->user_timer	= (AkMathRandFx() *
							   (GMD_BOSS5_EFCT_BODY_SMALL_SMOKE_LOOP_TIME_MAX -
								GMD_BOSS5_EFCT_BODY_SMALL_SMOKE_LOOP_TIME_MIN) +
							   GMD_BOSS5_EFCT_BODY_SMALL_SMOKE_LOOP_TIME_MIN) >> FX32_SHIFT;
	
	
	// 処理関数設定
	obj_work->ppFunc	= gmBoss5EfctBodySmallSmokeProcLoop;
}

// =======================================================================
// gmBoss5EfctBodySmallSmokeProcLoop
/*!
  本体 小さい黒煙エフェクト 更新処理 ループ
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctBodySmallSmokeProcLoop(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	// 座標更新
	GmBsCmnUpdateObject3DESStuckWithNodeRelative(obj_work,
												 &parent_body->snm_work,
												 parent_body->body_snm_reg_id,
												 TRUE,
												 &obj_work->parent_obj->pos,
												 &parent_body->pivot_prev_pos);
	
	// 既定時間ループ
	if (obj_work->user_timer) {
		obj_work->user_timer--;
	}
	else {
		// パーティクル生成停止
		ObjDrawKillAction3DES(obj_work);
		
		// 既定範囲内でランダムな値を停滞時間として設定
		obj_work->user_timer	= (AkMathRandFx() *
								   (GMD_BOSS5_EFCT_BODY_SMALL_SMOKE_IDLE_TIME_MAX -
									GMD_BOSS5_EFCT_BODY_SMALL_SMOKE_IDLE_TIME_MIN) +
								   GMD_BOSS5_EFCT_BODY_SMALL_SMOKE_IDLE_TIME_MIN) >> FX32_SHIFT;
		
		obj_work->ppFunc	= gmBoss5EfctBodySmallSmokeProcFade;
	}
}

// =======================================================================
// gmBoss5EfctBodySmallSmokeProcFade
/*!
  本体 小さい黒煙エフェクト 更新処理 フェード
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctBodySmallSmokeProcFade(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	// エフェクトの更新終了待ち
	if (obj_work->disp_flag & OBD_DISP_END) {
		
		// エフェクトが表示されなくなった後既定フレーム待機
		if (obj_work->user_timer) {
			obj_work->user_timer--;
		}
		else {
			// 次のエフェクト生成
			gmBoss5EfctCreateBodySmallSmoke(parent_body,
											obj_work->user_work);
			
			// 自分消去
			obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
		}
	}
}

// =======================================================================
// gmBoss5EfctCreateBerserkSteam
/*!
  凶暴化スチームエフェクト 生成
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss5EfctCreateBerserkSteam(GMS_BOSS5_BODY_WORK *body_work, Uint32 count, Uint32 part_idx)
{
	GMS_EFFECT_3DES_WORK	*efct_3des;
	OBS_OBJECT_WORK	*obj_work;
	
	// 回数0なら何も生成しない
	if (count == 0) {
		return;
	}
	
	efct_3des	=
		GmEfctCmnEsCreate(GMM_BS_OBJ(body_work),
						  GME_EFCT_CMN_IDX_STEAM_S);
	
	obj_work	= (OBS_OBJECT_WORK*)efct_3des;
	
	// パーツ番号を保存
	obj_work->user_flag	= part_idx;
	
	// 残り発生回数を設定
	obj_work->user_work	= count - 1;
	
	// 表示オフセット設定
	GmEffect3DESSetDispOffset(efct_3des,
							  gm_boss5_efct_berserk_steam_disp_ofst_tbl[part_idx][MTD_X],
							  gm_boss5_efct_berserk_steam_disp_ofst_tbl[part_idx][MTD_Y],
							  gm_boss5_efct_berserk_steam_disp_ofst_tbl[part_idx][MTD_Z]);
	
	// 表示回転設定
	GmEffect3DESSetDispRotation(efct_3des,
								gm_boss5_efct_berserk_steam_disp_rot_tbl[part_idx][MTD_X],
								gm_boss5_efct_berserk_steam_disp_rot_tbl[part_idx][MTD_Y],
								gm_boss5_efct_berserk_steam_disp_rot_tbl[part_idx][MTD_Z]);
	
	// 噴出時間設定
	obj_work->user_timer	= GMD_BOSS5_EFCT_BERSERK_STEAM_BLAST_TIME;
	
	// 処理関数設定
	obj_work->ppFunc	= gmBoss5EfctBerserkSteamProcLoop;
}

// =======================================================================
// gmBoss5EfctBerserkSteamProcLoop
/*!
  凶暴化スチームエフェクト 更新処理 ループ
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctBerserkSteamProcLoop(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	// 座標更新
	GmBsCmnUpdateObject3DESStuckWithNodeRelative(obj_work,
												 &parent_body->snm_work,
												 parent_body->body_snm_reg_id,
												 TRUE,
												 &obj_work->parent_obj->pos,
												 &parent_body->pivot_prev_pos);
	
	// 既定時間ループ
	if (obj_work->user_timer) {
		obj_work->user_timer--;
	}
	else {
		// パーティクル生成停止
		ObjDrawKillAction3DES(obj_work);
		
		// 停滞時間設定
		obj_work->user_timer	= GMD_BOSS5_EFCT_BERSERK_STEAM_IDLE_TIME;
		
		obj_work->ppFunc	= gmBoss5EfctBerserkSteamProcFade;
	}
}

// =======================================================================
// gmBoss5EfctBerserkSteamProcFade
/*!
  凶暴化スチームエフェクト 更新処理 フェード
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctBerserkSteamProcFade(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	// 座標更新
	GmBsCmnUpdateObject3DESStuckWithNodeRelative(obj_work,
												 &parent_body->snm_work,
												 parent_body->body_snm_reg_id,
												 TRUE,
												 &obj_work->parent_obj->pos,
												 &parent_body->pivot_prev_pos);
	
	// エフェクトの更新終了待ち
	if (obj_work->disp_flag & OBD_DISP_END) {
		
		// エフェクトが表示されなくなった後既定フレーム待機
		if (obj_work->user_timer) {
			obj_work->user_timer--;
		}
		else {
			
			if (obj_work->user_work != 0) {
				// 残り回数がある場合、次のエフェクト生成
				gmBoss5EfctCreateBerserkSteam(parent_body,
											  obj_work->user_work,
											  obj_work->user_flag);
			}
			
			// 自分消去
			obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
		}
	}
}

// =======================================================================
// gmBoss5EfctCreateEggSweat
/*!
  エッグマン 汗エフェクト初期化
  
  @param egg_work	[io]	エッグマンワーク
 */
// =======================================================================
void gmBoss5EfctCreateEggSweat(GMS_BOSS5_EGG_WORK *egg_work)
{
	GMS_EFFECT_3DES_WORK	*efct_3des;
	
	efct_3des	=
		GmEfctCmnEsCreate(GMM_BS_OBJ(egg_work), GME_EFCT_CMN_IDX_SWEAT);
	
	// 位置調整
	GmEffect3DESAddDispOffset(efct_3des,
							  0,
							  GMD_BOSS5_EFCT_EGG_SWEAT_DISP_OFST_Y,
							  GMD_BOSS5_EFCT_EGG_SWEAT_DISP_OFST_Z);
	
	GMM_BS_OBJ(efct_3des)->ppFunc	= gmBoss5EfctEggSweatProcLoop;
}

// =======================================================================
// gmBoss5EfctEggSweatProcLoop
/*!
  エッグマン汗エフェクト 更新処理 ループ
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EfctEggSweatProcLoop(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_EGG_WORK	*parent_egg	= (GMS_BOSS5_EGG_WORK*)obj_work->parent_obj;
	
	// 有効フラグがオフになったら消去開始
	if (!(parent_egg->flag & GMD_BOSS5_EGG_FLAG_SWEAT_ACTIVE)) {
		
		ObjDrawKillAction3DES(obj_work);
		
		// パーティクルが全て消えた時点でタスククリア
		obj_work->ppFunc	= GmEffectDefaultMainFuncDeleteAtEnd;
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
