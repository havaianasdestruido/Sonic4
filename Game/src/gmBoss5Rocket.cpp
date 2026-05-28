// =======================================================================
/*!
  @file	gmBoss5Rocket.cpp
  @brief ボスファイナル ロケットパンチ
  
  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss5Rocket.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akMath.h"
#include "gmMain.h"
#include "gmEventTbl.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmBossCommon.h"
#include "gmBoss5.h"
#include "gmBoss5Turret.h"
#include "gmBoss5Rocket.h"
#include "gmBoss5Egg.h"
#include "gmBoss5Efct.h"
#include "gmSound.h"

#include "gmPlySeq.h"
#include "gmPlyScoreDef.h"

// データヘッダ
#include "../file/common/arc/BOSS05.hmb"
#include "../file/common/model/BOSS05_MDL.hmb"
#include "../file/common/model/BOSS05_BODY_MTN.hmb"
#include "../file/common/model/BOSS05_ROCKET_MTN.hmb"

/*------ Macros --------------------------------------------------------*/
//############ ノード番号 #####################################################
// ロケット
#define GMD_BOSS5_RKT_NODE_IDX_DRILL		(3)		//!< 爪の部分

#define GMD_BOSS5_RKT_NODE_SNM_NUM			(1)		//!< SNM登録ノード数

//############ ロケット #######################################################
/* フラグ */

/* 定義値 */
#define GMD_BOSS5_RKT_HIDE_RADIUS			((fx32)(FX32_ONE * 48))	//!< この距離分、画面端から離れたら完全に画面外に消えたと判定する
#define GMD_BOSS5_RKT_SEARCH_INITIAL_DIR_Z_L	(AKM_DEGtoA16(180))		//!< サーチ開始時のZ周り初期角度（左向き）
#define GMD_BOSS5_RKT_SEARCH_INITIAL_DIR_Z_R	(AKM_DEGtoA16(0))		//!< サーチ開始時のZ周り初期角度（右向き）
#define GMD_BOSS5_RKT_SEARCH_INITIAL_ADJ_DIR_X_LA	(AKM_DEGtoA32(-15.f))	//!< サーチ開始時のX周り初期調整角度（左腕）
#define GMD_BOSS5_RKT_SEARCH_INITIAL_ADJ_DIR_X_RA	(AKM_DEGtoA32(40.f))	//!< サーチ開始時のX周り初期調整角度（右腕）
#define GMD_BOSS5_RKT_SEARCH_ROT_SPD_DEG	(1.f)					//!< サーチ時のロケット回転角速度

// 地形当たり矩形サイズ
#define GMD_BOSS5_RKT_FIELD_RECT_SIZE_LEFT		(-16)
#define GMD_BOSS5_RKT_FIELD_RECT_SIZE_TOP		(-16)
#define GMD_BOSS5_RKT_FIELD_RECT_SIZE_RIGHT		(16)
#define GMD_BOSS5_RKT_FIELD_RECT_SIZE_BOTTOM	(16)

// 外側矩形サイズ
#define GMD_BOSS5_RKT_OUT_RECT_SIZE_LEFT	(-20)
#define GMD_BOSS5_RKT_OUT_RECT_SIZE_TOP		(-20)
#define GMD_BOSS5_RKT_OUT_RECT_SIZE_RIGHT	(20)
#define GMD_BOSS5_RKT_OUT_RECT_SIZE_BOTTOM	(20)
// 内側矩形サイズ
#define GMD_BOSS5_RKT_IN_RECT_SIZE_LEFT		(-10)
#define GMD_BOSS5_RKT_IN_RECT_SIZE_TOP		(-10)
#define GMD_BOSS5_RKT_IN_RECT_SIZE_RIGHT	(10)
#define GMD_BOSS5_RKT_IN_RECT_SIZE_BOTTOM	(10)

// 通常ロケットパンチパラメータ
#define GMD_BOSS5_RKT_NML_SEARCH_DELAY		(20)						//!< 通常ロケット プレイヤーサーチ遅延フレーム数
#define GMD_BOSS5_RKT_NML_FLY_INIT_ACC		((fx32)(FX32_ONE * 1.f))	//!< 通常ロケット 初期加速度
#define GMD_BOSS5_RKT_NML_FLY_INIT_SPD		((fx32)(FX32_ONE * 10))		//!< 通常ロケット 初速度
#define GMD_BOSS5_RKT_NML_FLY_MAX_SPD		((fx32)(FX32_ONE * 12))		//!< 通常ロケット 飛行速度最大値
#define GMD_BOSS5_RKT_NML_FLY_DISTNACE		((fx32)(FX32_ONE * 192))	//!< 通常ロケット 飛行距離
#define GMD_BOSS5_RKT_NML_FLY_INIT_DECEL	((fx32)(FX32_ONE * 1.f))	//!< 通常ロケット 減速度
#define GMD_BOSS5_RKT_NML_RET_INIT_ACC		((fx32)(FX32_ONE * 0.5f))	//!< 通常ロケット 戻り時初期加速度
#define GMD_BOSS5_RKT_NML_RET_MAX_SPD		((fx32)(FX32_ONE * 10))		//!< 通常ロケット 戻り時飛行速度最大値

// 強化ロケットパンチパラメータ
#define GMD_BOSS5_RKT_STR_SEARCH_DELAY		(20)						//!< 強化ロケット プレイヤーサーチ遅延フレーム数
#define GMD_BOSS5_RKT_STR_FLY_INIT_ACC		((fx32)(FX32_ONE * 1.5f))	//!< 強化ロケット 初期化速度
#define GMD_BOSS5_RKT_STR_FLY_INIT_SPD		((fx32)(FX32_ONE * 10))		//!< 強化ロケット 初速度
#define GMD_BOSS5_RKT_STR_FLY_MAX_SPD		((fx32)(FX32_ONE * 15))		//!< 強化ロケット 飛行速度最大値
#define GMD_BOSS5_RKT_STR_FLY_DITANCE		((fx32)(FX32_ONE * 192))	//!< 強化ロケット 飛行距離
#define GMD_BOSS5_RKT_STR_FLY_REDIR_ACC		((fx32)(FX32_ONE * 0.5f))	//!< 強化ロケット 上方への方向転換時初期加速度
#define GMD_BOSS5_RKT_STR_FLY_REV_H_ACC_X	((fx32)(FX32_ONE * -0.5f))	//!< 強化ロケット 上方への方向転換時の水平方向逆噴射加速度
#define GMD_BOSS5_RKT_STR_FLY_REDIR_ROT_SPD_DEG	(5.f)					//!< 強化ロケット 上方への方向転換時の方向転換角速度
#define GMD_BOSS5_RKT_STR_FLY_ABOVE_INIT_ACC	((fx32)(FX32_ONE * 0.5f))	//!< 強化ロケット 上昇初期化速度
#define GMD_BOSS5_RKT_STR_FLY_ABOVE_ROT_SPD_DEG	(1.f)					//!< 強化ロケット 上昇時の方向転換最大角速度
#define GMD_BOSS5_RKT_STR_NO_SEARCH_TIME	(10)						//!< 落下タイミングからこのフレーム分前のタイミングまでサーチを行う
#define GMD_BOSS5_RKT_STR_FALL_INIT_SPD		((fx32)(FX32_ONE * 2))		//!< 強化ロケット 落下時の初速度
#define GMD_BOSS5_RKT_STR_RET_INIT_ACC		((fx32)(FX32_ONE * 0.3f))	//!< 強化ロケット 戻り時初期化速度
#define GMD_BOSS5_RKT_STR_RET_INIT_SPD		((fx32)(FX32_ONE * 0))		//!< 強化ロケット 戻り時初速度
#define GMD_BOSS5_RKT_STR_RET_MAX_SPD		((fx32)(FX32_ONE * 10))		//!< 強化ロケット 戻り時飛行速度最大値
#define GMD_BOSS5_RKT_STR_RET_ROT_SPD_DEG	(2.f)						//!< 強化ロケット 戻り時方向転換角速度
#define GMD_BOSS5_RKT_STR_DOWN_RET_ROT_SPD_DEG	(5.f)					//!< 強化ロケット 戻り時方向転換角速度（ダウン時）

#define GMD_BOSS5_RKT_STR_RECOVER_SLERP_SPD	(0.1f)						//!< 強化ロケット 未発射状態からの復帰時の姿勢補間進捗速度

#define GMD_BOSS5_RKT_FALL_SPIN_ANGLE_SPD	(AKM_DEGtoA32(3))			//!< 落下時のツメ方向軸回転 回転角速度
#define GMD_BOSS5_RKT_FALL_WOBBLE_SIN_PARAM_DEG_SPD	(AKM_DEGtoA32(5))	//!< 落下時ふらつき用サイン波のパラメータ角度角速度（角度速度が大きいほど、ふらつきの周期が短くなる）
#define GMD_BOSS5_RKT_FALL_WOBBLE_DEG_MAX	((Float)15.f)				//!< 落下時ふらつき角度の最大絶対角（ふらつきの最大傾斜）

#define GMD_BOSS5_RKT_GRD_STUCK_TIME_SHORT	(30)					//!< プレイヤーにヒット済みの時の地面突き刺さり時間
#define GMD_BOSS5_RKT_GRD_STUCK_TIME_LONG	(300)					//!< プレイヤーにヒットしていない時の地面突き刺さり時間
#define GMD_BOSS5_RKT_GRD_STUCK_LEAN_DOWN_OFST_DEG	(180.f)			//!< ダウン状態のロケット角度オフセット（突き刺さり時初期角度からのオフセット）
#define GMD_BOSS5_RKT_GRD_STUCK_LEAN_DOWN_EXTEND_F_COL_HEIGHT	((Sint16)8)
#define GMD_BOSS5_RKT_GRD_STUCK_LEAN_DOWN_RATIO_ADD	(0.075f)		//!< 突き刺さり傾き補間係数加算速度
#define GMD_BOSS5_RKT_GRD_STUCK_LEAN_DOWN_MOVE_DISTANCE		((fx32)(FX32_ONE * 32))		//!< ダウン状態になる時の回転中に移動する距離
#define GMD_BOSS5_RKT_GRD_STUCK_LEAN_OFST_DEG_ADD	(-10.f)			//!< 一回のヒット毎に傾ける角度
#define GMD_BOSS5_RKT_GRD_STUCK_LEAN_HIT_VIB_AMP_DEG_INIT	(10.f)				//!< ヒット時振動振幅角度初期値
#define GMD_BOSS5_RKT_GRD_STUCK_LEAN_HIT_VIB_AMP_DEG_SUBTRACT	(0.5f)			//!< ヒット時振動振幅減衰量
#define GMD_BOSS5_RKT_GRD_STUCK_LEAN_HIT_VIB_SIN_ANGLE_ADD	(AKM_DEGtoA32(-90))	//!< ヒット時振動波形用パラメタ角度加算値

#define GMD_BOSS5_RKT_BLOW_TRIGGER_HIT_NUM	(3)						//!< 地面突き刺さり状態の時にこの回数攻撃がヒットしたら本体に飛ばす
#define GMD_BOSS5_RKT_BLOW_FLY_SPD			((fx32)(FX32_ONE * 12.f))	//!< 吹っ飛び移動時速度
#define GMD_BOSS5_RKT_BLOW_FLY_ROT_SPD		(AKM_DEGtoA32(30.f))		//!< 吹っ飛び移動時回転速度

#define GMD_BOSS5_RKT_BOUNCE_DIR_ANGLE		(AKM_DEGtoA32(255.f))		//!< 跳ね返り方向
#define GMD_BOSS5_RKT_BOUNCE_FLY_SPD		((fx32)(FX32_ONE * 4.f))	//!< 跳ね返り移動時速度
#define GMD_BOSS5_RKT_BOUNCE_FLY_ROT_SPD	(AKM_DEGtoA32(30.f))			//!< 跳ね返り移動時回転速度
#define GMD_BOSS5_RKT_BOUNCE_RET_INIT_ACC	(GMD_BOSS5_RKT_STR_RET_INIT_ACC)	//!< 跳ね返りロケット 戻り時初期加速度
#define GMD_BOSS5_RKT_BOUNCE_RET_INIT_SPD	(GMD_BOSS5_RKT_STR_RET_INIT_SPD)	//!< 跳ね返りロケット 戻り時初速度
#define GMD_BOSS5_RKT_BOUNCE_RET_ROT_SPD_DEG	(5.f)					//!< 跳ね返りロケット 戻り時方向転換角速度

#define GMD_BOSS5_RKT_LOCKON_DIR_LIMIT_R_START	(AKM_DEGtoA32(330))		//!< 
#define GMD_BOSS5_RKT_LOCKON_DIR_LIMIT_R_END	(AKM_DEGtoA32(370))
#define GMD_BOSS5_RKT_LOCKON_DIR_LIMIT_L_START	(AKM_DEGtoA32(170))
#define GMD_BOSS5_RKT_LOCKON_DIR_LIMIT_L_END	(AKM_DEGtoA32(210))

#define GMD_BOSS5_RKT_DMG_NO_HIT_TIME		(10)						//!< ダメージ時ヒット無効時間

#define GMD_BOSS5_RKT_STR_FLY_DISP_OFST		(-10.f)					//!< 強化ロケット飛行時の表示オフセット

#define GMD_BOSS5_RKT_LEAKAGE_ON_TIME		(20)			//!< 落下時漏電エフェクト オン時間
#define GMD_BOSS5_RKT_LEAKAGE_OFF_TIME		(20)			//!< 落下時漏電エフェクト オフ時間

// 跳ね返りパラメータ
#define GMD_BOSS5_RKT_PLY_NML_REBOUND_X		((fx32)(FX32_ONE * 3))	//!< 通常跳ね返り時X速度
#define GMD_BOSS5_RKT_PLY_NML_REBOUND_Y		((fx32)(FX32_ONE * -4))	//!< 通常跳ね返り時Y速度
#define GMD_BOSS5_RKT_PLY_NML_REBOUND_NOJUMPMOVE_TIME	((fx32)(FX32_ONE * 8))	//!< 通常跳ね返り時ジャンプ中移動禁止時間
#define GMD_BOSS5_RKT_PLY_HOMING_REBOUND_X	((fx32)(FX32_ONE * 4))	//!< ホーミング跳ね返り時X速度
#define GMD_BOSS5_RKT_PLY_HOMING_REBOUND_Y	((fx32)(FX32_ONE * -4))	//!< ホーミング跳ね返り時Y速度
#define GMD_BOSS5_RKT_PLY_HOMING_REBOUND_NOJUMPMOVE_TIME	((fx32)(FX32_ONE * 12))	//!< ホーミング跳ね返り時ジャンプ中移動禁止時間

//############ 動作シーケンス #################################################
// ロケット
#define GMD_BOSS5_RKT_SEQ_LIFE_LEVEL_NUM		(3)					//!< ライフレベル数
#define GMD_BOSS5_RKT_SEQ_WAITFALL_CHOICE_NUM	(3)					//!< 画面外待機時間選択肢数

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! 攻撃・食らい矩形の内側・外側タイプ列挙型
typedef enum
{
	GME_BOSS5_RKT_SIDES_DMG_OUTSIDE	= 0,	//!< 食らい矩形が外側
	GME_BOSS5_RKT_SIDES_ATK_OUTSIDE,		//!< 攻撃矩形が外側
	
	GME_BOSS5_RKT_SIDES_MAX
} GME_BOSS5_RKT_SIDES;

//! ロケット シーケンス 画面外待機情報構造体
typedef struct tag_GMS_BOSS5_RKT_SEQ_WAITFALL_INFO
{
	Sint32	life_threshold;	//!< 残りライフ(最後の一撃分含む)がこの値「以下」なら該当
	fx32	probability[GMD_BOSS5_RKT_SEQ_WAITFALL_CHOICE_NUM];	//!< 確率テーブル
	Uint32	frame[GMD_BOSS5_RKT_SEQ_WAITFALL_CHOICE_NUM];		//!< フレーム数テーブル
} GMS_BOSS5_RKT_SEQ_WAITFALL_INFO;


/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/

//############ ボスFINALロケット ##############################################
static void gmBoss5RocketExit(MTS_TASK_TCB *tcb);
/* インターフェース関数 */
static GMS_BOSS5_ROCKET_WORK* gmBoss5RocketCreate(GMS_BOSS5_BODY_WORK *body_work, GME_BOSS5_RKT_TYPE rkt_type);
/* 補助関数 */
// 当たり関連
static void gmBoss5RocketSetAtkBodyRect(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketSetNoHitTime(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketUpdateNoHitTime(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketUpdateMainRectPosition(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketSetAtkEnable(GMS_BOSS5_ROCKET_WORK *rkt_work, BOOL enable);
static void gmBoss5RocketSetDmgEnable(GMS_BOSS5_ROCKET_WORK *rkt_work, BOOL enable);
static void gmBoss5RocketSetRectSize(GMS_BOSS5_ROCKET_WORK *rkt_work, GME_BOSS5_RKT_SIDES sides_type);
// シーケンス関連
static void gmBoss5RocketInitPlySearch(GMS_BOSS5_ROCKET_WORK *rkt_work, Sint32 delay);
static void gmBoss5RocketUpdatePlySearch(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketGetPlySearchPos(const GMS_BOSS5_ROCKET_WORK *rkt_work, VecFx32 *pos);
static BOOL gmBoss5RocketSetPlyRebound(const OBS_RECT_WORK *rkt_rect, OBS_RECT_WORK *ply_rect);
static void gmBoss5RocketUpdateRocketStuckWithArm(GMS_BOSS5_ROCKET_WORK *rkt_work, BOOL b_rotation);
static void gmBoss5RocketInitRocketStuckWithArmLerpRot(GMS_BOSS5_ROCKET_WORK *rkt_work,
													   Float ratio_spd);
static BOOL gmBoss5RocketUpdateRocketStuckWithArmLerpRot(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketGetArmNodePosFx(const GMS_BOSS5_ROCKET_WORK *rkt_work, VecFx32 *pos_out);
static void gmBoss5RocketSetDispOfst(GMS_BOSS5_ROCKET_WORK *rkt_work, Float disp_ofst,
									 BOOL b_pos_slide);
static void gmBoss5RocketGetDispOfst(const GMS_BOSS5_ROCKET_WORK *rkt_work, VecFx32 *ofst_pos_out);
static Angle32 gmBoss5RocketInitFlyDestPos(GMS_BOSS5_ROCKET_WORK *rkt_work,
										   fx32 init_acc, fx32 init_spd, fx32 max_spd,
										   const VecFx32 *launch_pos, const VecFx32 *dest_pos);
static void gmBoss5RocketInitFlyDestDistance(GMS_BOSS5_ROCKET_WORK *rkt_work,
											 fx32 init_acc, fx32 init_spd, fx32 max_spd,
											 const VecFx32 *launch_pos,
											 Angle32 angle, fx32 distance);
static void gmBoss5RocketRedirectFlyDestPos(GMS_BOSS5_ROCKET_WORK *rkt_work,
											fx32 init_acc, const VecFx32 *dest_pos);
#if (GMD_BOSS5_BOOL_USE_OBSOLETE_FUNCTION)
static void gmBoss5RocketRedirectFlyDestDistance(GMS_BOSS5_ROCKET_WORK *rkt_work,
												 fx32 init_acc, Angle32 angle, fx32 distance);
#endif /* (GMD_BOSS5_BOOL_USE_OBSOLETE_FUNCTION) */
static BOOL gmBoss5RocketUpdateFlyDest(GMS_BOSS5_ROCKET_WORK *rkt_work, BOOL b_mdl_center=FALSE);
static void gmBoss5RocketInitFlyReverse(GMS_BOSS5_ROCKET_WORK *rkt_work,
										fx32 acc_scalar, BOOL is_add=FALSE);
static void gmBoss5RocketInitFlyReverseVec(GMS_BOSS5_ROCKET_WORK *rkt_work,
										   const VecFx32 *acc_vec, BOOL is_add=FALSE);
static BOOL gmBoss5RocketUpdateFlyReverse(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketSetInitialDir(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketUpdateDirFollowingPos(GMS_BOSS5_ROCKET_WORK *rkt_work, const VecFx32 *targ_pos,
											   Float deg, BOOL is_reverse, Angle32 force_rot_spd=0);
static void gmBoss5RocketUpdateDirPlyLockOn(GMS_BOSS5_ROCKET_WORK *rkt_work, const VecFx32 *lock_pos);
#if (GMD_BOSS5_BOOL_USE_OBSOLETE_FUNCTION)
static void gmBoss5RocketUpdateDirFollowingSpd(GMS_BOSS5_ROCKET_WORK *rkt_work, Float deg, BOOL is_reverse);
#endif /* (GMD_BOSS5_BOOL_USE_OBSOLETE_FUNCTION) */
static void gmBoss5RocketUpdateDirFollowingAccSpd(GMS_BOSS5_ROCKET_WORK *rkt_work, Float deg, BOOL is_reverse);
static void gmBoss5RocketLimitDir(GMS_BOSS5_ROCKET_WORK *rkt_work, Angle32 start_angle, Angle32 end_angle);
static void gmBoss5RocketInitDirFalling(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketUpdateDirFalling(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketEndDirFalling(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketInitLeakageFlicker(GMS_BOSS5_ROCKET_WORK *rkt_work);
static BOOL gmBoss5RocketUpdateLeakageFlicker(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketClearLeakageFlicker(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketInitFlyBlow(GMS_BOSS5_ROCKET_WORK *rkt_work);
static BOOL gmBoss5RocketUpdateFlyBlow(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketInitFlyBounce(GMS_BOSS5_ROCKET_WORK *rkt_work);
static BOOL gmBoss5RocketUpdateFlyBounce(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketInitStuckLean(GMS_BOSS5_ROCKET_WORK *rkt_work);
static BOOL gmBoss5RocketUpdateStuckLean(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketSetStuckLeanHitVib(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketInitScatter(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketUpdateScatter(GMS_BOSS5_ROCKET_WORK *rkt_work);
// シグナル関連
static inline BOOL gmBoss5RocketReceiveSignalLaunch(GMS_BOSS5_ROCKET_WORK *rkt_work);
static inline BOOL gmBoss5RocketReceiveSignalReturn(GMS_BOSS5_ROCKET_WORK *rkt_work);
static inline void gmBoss5RocketDispatchSignalReturned(GMS_BOSS5_ROCKET_WORK *rkt_work);
/* ノード処理関連 */
static void gmBoss5RocketInitCallbacks(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketReleaseCallbacks(GMS_BOSS5_ROCKET_WORK *rkt_work);
/* 処理関数 */
static void gmBoss5RocketAtkPlyHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);
static void gmBoss5RocketAtkBossHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);
static void gmBoss5RocketDamageDefFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);
static void gmBoss5RocketOutFunc(OBS_OBJECT_WORK *obj_work);
/* 制御処理 */
static void gmBoss5RocketMain(OBS_OBJECT_WORK *obj_work);
/* シーケンス */
// 通常ロケットパンチシーケンス
static void gmBoss5RocketNmlProcInit(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketNmlProcUpdateFace(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketNmlProcUpdateFly(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketNmlProcUpdateWaitDecel(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketNmlProcUpdateWaitReturn(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketNmlProcUpdateFinalize(GMS_BOSS5_ROCKET_WORK *rkt_work);
// 強化ロケットパンチシーケンス
static void gmBoss5RocketStrProcInit(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketStrProcUpdateFace(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketStrProcUpdateFlyTarget(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketStrProcUpdateFlyDecel(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketStrProcUpdateFlyAbove(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketStrProcUpdateWaitFall(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketStrProcUpdateFall(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketStrProcUpdateStuck(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketStrProcUpdateReturn(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketStrProcUpdateFinalize(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketStrProcUpdateRecover(GMS_BOSS5_ROCKET_WORK *rkt_work);
// 吹っ飛ばしシーケンス
static void gmBoss5RocketBlowProcInit(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketBlowProcUpdateFly(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketBlowProcUpdateWaitHit(GMS_BOSS5_ROCKET_WORK *rkt_work);
// 跳ね返りシーケンス
static void gmBoss5RocketBounceProcInit(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketBounceProcUpdateFlyUp(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketBounceProcUpdateWait(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketBounceProcUpdateReturn(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketBounceProcUpdateFinalize(GMS_BOSS5_ROCKET_WORK *rkt_work);
// 本体接続シーケンス
static void gmBoss5RocketCnctProcInit(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketCnctProcUpdateIdle(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketCnctProcUpdateWaitScatterStart(GMS_BOSS5_ROCKET_WORK *rkt_work);
static void gmBoss5RocketCnctProcUpdateScatter(GMS_BOSS5_ROCKET_WORK *rkt_work);

//############ 動作シーケンス #################################################
static Uint32 gmBoss5RocketSeqGetWaitFallTime(GMS_BOSS5_ROCKET_WORK *rkt_work);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
//############ 動作シーケンス #################################################
//! ロケット シーケンス 画面外待機情報テーブル
const static GMS_BOSS5_RKT_SEQ_WAITFALL_INFO gm_boss5_rkt_seq_wait_fall_time_tbl[GMD_BOSS5_RKT_SEQ_LIFE_LEVEL_NUM]	= {
	{ 4,	 { (fx32)(FX32_ONE * 0.6f),	(fx32)(FX32_ONE * 0.4f),	(fx32)(FX32_ONE * 0.0f),},	{ 60,	30,	0,}},
	{ 8,	 { (fx32)(FX32_ONE * 0.5f),	(fx32)(FX32_ONE * 0.3f),	(fx32)(FX32_ONE * 0.2f),},	{ 120,	60,	30,}},
	{ 17,	 { (fx32)(FX32_ONE * 1.0f),	(fx32)(FX32_ONE * 0.f),		(fx32)(FX32_ONE * 0.f), },	{ 150,	0,	0, }},
};

/*------ Global Functions ----------------------------------------------*/

// =======================================================================
// GmBoss5RocketInit
/*!
  ボスFINAL ロケット（ロケットパンチの弾）初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ（0: 通常時, 1: 凶暴時）
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss5RocketInit(GMS_EVE_RECORD_EVENT *eve_rec,
								   fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	
	OBS_OBJECT_WORK	*obj_work;
	GMS_ENEMY_3D_WORK	*ene_3d;
	GMS_BOSS5_ROCKET_WORK	*rkt_work;
	
	// オブジェクト生成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x, pos_y,
										sizeof(GMS_BOSS5_ROCKET_WORK),
										"BOSS5_RKT");
	
	ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	rkt_work	= (GMS_BOSS5_ROCKET_WORK*)obj_work;
	
	// 地形当たり
	ObjObjectFieldRectSet(obj_work,
						  GMD_BOSS5_RKT_FIELD_RECT_SIZE_LEFT,
						  GMD_BOSS5_RKT_FIELD_RECT_SIZE_TOP,
						  GMD_BOSS5_RKT_FIELD_RECT_SIZE_RIGHT,
						  GMD_BOSS5_RKT_FIELD_RECT_SIZE_BOTTOM);
	obj_work->move_flag	|= OBD_MOVE_NOCOL;
	obj_work->move_flag	&= ~OBD_MOVE_LIMIT_OUT;	// 画面端には当たらない
	
	// ロケットモデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
								 &(GmBoss5GetObject3dList()[IDB_BOSS05_MDL_B05_ROCKET_L_ZNO]),
								 &ene_3d->obj_3d);
	
	// ロケットモーションロード
	ObjObjectAction3dNNMotionLoad(obj_work,
								  0,
								  TRUE,
								  ObjDataGet(GMD_DWORK_NO_BOSS_05_ROCKET_MTN),
								  NULL,
								  0,
								  NULL);
	
	// Wii向けトゥーン設定
	ObjDrawObjectSetToon(obj_work);
	
	// ブレンド速度設定
	obj_work->obj_3d->blend_spd	= GMD_BOSS5_DEFAULT_BLEND_SPD;	// 念のため設定しておく
	
	// ワーク設定
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag	|= OBD_DISP_NODIRFLIP;
	obj_work->move_flag	|= OBD_MOVE_JUMP;	// 地形当たりフラグが角度の影響を受けないようにする
	
	// 最初はホーミングの対象からはずす
	ene_3d->ene_com.enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
	
	// 当たり矩形設定
	gmBoss5RocketSetRectSize(rkt_work, GME_BOSS5_RKT_SIDES_ATK_OUTSIDE);	// サイズ設定
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].ppHit	= gmBoss5RocketAtkPlyHitFunc;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].ppDef	= gmBoss5RocketDamageDefFunc;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag	|= OBD_RECT_NOHIT;	// 最初は食らわない
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag |= OBD_RECT_NOHIT;	// 体当たりは使わない
	
	// 初期アクション設定（ダミーモーション）
	GmBsCmnSetAction(obj_work, IDB_BOSS05_ROCKET_MTN_B05_ROCKET_L_ZNM, TRUE, FALSE);
	
	// コールバック関係初期化
	gmBoss5RocketInitCallbacks(rkt_work);
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss5RocketMain;
	
	// 専用描画処理設定
	obj_work->ppOut	= gmBoss5RocketOutFunc;
	
	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, gmBoss5RocketExit);
	
#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}



// =======================================================================
// GmBoss5RocketLaunchNormal
/*!
  通常ロケット発射
  
  @param body_work		[io]	本体ワーク
  @param rkt_type		[in]	ロケットタイプ(GME_BOSS5_RKT_TYPE_XXX)
  
  @return ロケットワーク
  
  @note
  ボス通常時版のロケットパンチを発射します。
 */
// =======================================================================
GMS_BOSS5_ROCKET_WORK* GmBoss5RocketLaunchNormal(GMS_BOSS5_BODY_WORK *body_work,
												 GME_BOSS5_RKT_TYPE rkt_type)
{
	GMS_BOSS5_ROCKET_WORK	*rkt_work;
	
	// ロケット生成
	rkt_work	= gmBoss5RocketCreate(body_work, rkt_type);
	
	// シーケンス初期化
	gmBoss5RocketNmlProcInit(rkt_work);
	
	return rkt_work;
}

// =======================================================================
// gmBoss5RocketLaunchStrong
/*!
  強化ロケット発射
  
  @param body_work		[io]	本体ワーク
  @param rkt_type		[in]	ロケットタイプ(GME_BOSS5_RKT_TYPE_XXX)
  
  @return ロケットワーク
  
  @note
  ボス凶暴時版のロケットパンチを発射します。
 */
// =======================================================================
GMS_BOSS5_ROCKET_WORK* GmBoss5RocketLaunchStrong(GMS_BOSS5_BODY_WORK *body_work,
												 GME_BOSS5_RKT_TYPE rkt_type)
{
	GMS_BOSS5_ROCKET_WORK	*rkt_work;
	
	// ロケット生成
	rkt_work	= gmBoss5RocketCreate(body_work, rkt_type);
	
	// シーケンス初期化
	gmBoss5RocketStrProcInit(rkt_work);
	
	return rkt_work;
}

// =======================================================================
// GmBoss5RocketSpawnConnected
/*!
  本体接続ロケット 生成
  
  @param body_work	[io]	本体ワーク
  @param rkt_type	[in]	ロケットタイプ(GME_BOSS5_RKT_TYPE_XXX)
  
  @return ロケットワーク
 
  @note
  ボス本体に接続している状態のロケットパンチを生成します。
 */
// =======================================================================
GMS_BOSS5_ROCKET_WORK* GmBoss5RocketSpawnConnected(GMS_BOSS5_BODY_WORK *body_work,
												   GME_BOSS5_RKT_TYPE rkt_type)
{
	GMS_BOSS5_ROCKET_WORK	*rkt_work;
	
	// ロケット生成
	rkt_work	= gmBoss5RocketCreate(body_work, rkt_type);
	
	// シーケンス初期化
	gmBoss5RocketCnctProcInit(rkt_work);
	
	return rkt_work;
}

/*------ Static Functions ----------------------------------------------*/

// ############################################################################
// ボスFINAL ロケット
// ############################################################################
// =======================================================================
// gmBoss5RocketExit
/*!
  ロケット終了処理
  
  @note
  標準の終了処理に加えて、ノードマトリクス取得関連バッファの解放などを行っています。
 */
// =======================================================================
void gmBoss5RocketExit(MTS_TASK_TCB *tcb)
{
	GMS_BOSS5_ROCKET_WORK	*rkt_work	= (GMS_BOSS5_ROCKET_WORK*)mtTaskGetTcbWork(tcb);
	
	// コールバック関係解放
	gmBoss5RocketReleaseCallbacks(rkt_work);
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}

// ============================================================================
// インターフェース関数
// ============================================================================
// =======================================================================
// gmBoss5RocketCreate
/*!
  ロケット生成
  
  @param body_work	[io]	本体ワーク
  @param rkt_type	[in]	ロケットタイプ
  
  @return ロケットワーク
 */
// =======================================================================
GMS_BOSS5_ROCKET_WORK* gmBoss5RocketCreate(GMS_BOSS5_BODY_WORK *body_work, GME_BOSS5_RKT_TYPE rkt_type)
{
	OBS_OBJECT_WORK	*obj_body	= GMM_BS_OBJ(body_work);
	OBS_OBJECT_WORK	*obj_rkt;
	GMS_BOSS5_ROCKET_WORK	*rkt_work;
	
	// 生成
	obj_rkt	= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS5_ROCKET,
										obj_body->pos.x, obj_body->pos.y,
										0,//flag
										0,0,0,0,
										0);
	
	rkt_work	= (GMS_BOSS5_ROCKET_WORK*)obj_rkt;
	
	// 親設定
	obj_rkt->parent_obj	= obj_body;
	
	// ロケットタイプ保存
	rkt_work->rkt_type	= rkt_type;
	
	// 接続ノード設定
	if (GME_BOSS5_RKT_TYPE_RIGHT == rkt_type) {
		rkt_work->arm_snm_id	= body_work->armpt_snm_reg_ids[GME_BOSS5_ARM_TYPE_RIGHT][GME_BOSS5_ARMPART_IDX_FOREARM];
	}
	else {
		MTM_ASSERT(GME_BOSS5_RKT_TYPE_LEFT == rkt_type);
		rkt_work->arm_snm_id	= body_work->armpt_snm_reg_ids[GME_BOSS5_ARM_TYPE_LEFT][GME_BOSS5_ARMPART_IDX_FOREARM];
	}
	
	
	// 適切な角度に設定
	obj_rkt->disp_flag	|= OBD_DISP_USERMTX_RIGHT;
	if (obj_body->disp_flag & OBD_DISP_HFLIP) {
		nnMakeRotateXMatrix(&obj_rkt->obj_3d->user_obj_mtx_r,
							AKM_DEGtoA32(180));
		obj_rkt->disp_flag	|= OBD_DISP_HFLIP;
	}
	else {
		nnMakeRotateXMatrix(&obj_rkt->obj_3d->user_obj_mtx_r,
							AKM_DEGtoA32(0));
		obj_rkt->disp_flag	&= ~OBD_DISP_HFLIP;
	}
	
	
	return (GMS_BOSS5_ROCKET_WORK*)obj_rkt;
}



// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss5RocketSetAtkBodyRect
/*!
  本体攻撃用に矩形設定
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void gmBoss5RocketSetAtkBodyRect(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)rkt_work;
	OBS_RECT_WORK	*atk_rect;
	
	atk_rect	= &ene_com->rect_work[GMD_ENEMY_RECT_ATK];
	
	ObjRectGroupSet(atk_rect,
					GMD_OBJ_RECT_GROUP_PLAYER,
					GMD_OBJ_RECT_TARGET_GROUPFLAG_ENEMY);
	ObjRectAtkSet(atk_rect,
				  GMD_OBJ_RECT_ATK_FLAG_NORMALATK,
				  GMD_OBJ_RECT_ATK_POWER_DEFAULT);
	ObjRectDefSet(atk_rect,
				  GMD_OBJ_RECT_DEF_FLAG_NOHIT,
				  GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	atk_rect->flag	|= OBD_RECT_ENABLE;
	
	atk_rect->ppHit	= gmBoss5RocketAtkBossHitFunc;
	atk_rect->ppDef	= NULL;
}

// =======================================================================
// gmBoss5RocketSetNoHitTime
/*!
  ヒット無効時間設定
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  連続ヒットを防ぐためのヒット無効時間を設定します。
  無効時間経過後にヒット有効化されるようにするには、
  gmBoss5RocketUpdateNoHitTime()を毎フレーム呼んでください。
 */
// =======================================================================
void gmBoss5RocketSetNoHitTime(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)rkt_work;
	
	rkt_work->no_hit_timer	= GMD_BOSS5_RKT_DMG_NO_HIT_TIME;
	ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag	|= OBD_RECT_NOHIT;
	// 食らい矩形（外側）と攻撃矩形（内側）のマージンが狭いので、
	// 内側に当たってしまうことのないよう、攻撃矩形も無効化しておく
	ene_com->rect_work[GMD_ENEMY_RECT_ATK].flag	|= OBD_RECT_NOHIT;
}

// =======================================================================
// gmBoss5RocketUpdateNoHitTime
/*!
  ヒット無効時間更新
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  ヒット無効時間を更新して、タイマカウント完了時に喰らい（と攻撃）矩形を復活させます。
  復活させたくない場合はこの関数を呼ばないでください。
 */
// =======================================================================
void gmBoss5RocketUpdateNoHitTime(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	if (rkt_work->no_hit_timer) {
		rkt_work->no_hit_timer--;
	}
	else {
		GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)rkt_work;
		
		ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag	&= ~OBD_RECT_NOHIT;
		ene_com->rect_work[GMD_ENEMY_RECT_ATK].flag	&= ~OBD_RECT_NOHIT;
	}
}

// =======================================================================
// gmBoss5RocketUpdateMainRectPosition
/*!
  ドリル矩形位置更新
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  矩形の位置を既定ノードの位置に合わせます。
 */
// =======================================================================
void gmBoss5RocketUpdateMainRectPosition(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	fx32	pos_offset[MTD_XY];
	const NNS_MATRIX	*w_mtx;
	
	w_mtx	= GmBsCmnGetSNMMtx(&rkt_work->snm_work,
							   rkt_work->drill_snm_reg_id);
	
	pos_offset[MTD_X]	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3)) - rkt_work->pivot_prev_pos.x;
	pos_offset[MTD_Y]	= -FX_F32_TO_FX32(NNM_MTX(*w_mtx, 1, 3)) - rkt_work->pivot_prev_pos.y;
	
	for (Sint32 i = 0; i < GMD_ENEMY_RECT_NUM; ++i) {
		// 指定ノード座標に配置されるように、矩形の親オブジェクト中心から指定ノードへのオフセット座標を設定
		VEC_Set(&rkt_work->ene_3d.ene_com.rect_work[i].rect.pos,
				pos_offset[MTD_X],
				pos_offset[MTD_Y],
				0);	// Zはオフセットしない
	}
}

// =======================================================================
// gmBoss5RocketSetAtkEnable
/*!
  攻撃矩形のON/OFF設定
  
  @param rkt_work	[io]	ロケットワーク
  @param enable		[in]	有効フラグ(TRUE:有効化, FALSE:無効化)
 */
// =======================================================================
void gmBoss5RocketSetAtkEnable(GMS_BOSS5_ROCKET_WORK *rkt_work, BOOL enable)
{
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)rkt_work;
	
	if (enable) {
		ene_com->rect_work[GMD_ENEMY_RECT_ATK].flag	&= ~OBD_RECT_NOHIT;
	}
	else {
		ene_com->rect_work[GMD_ENEMY_RECT_ATK].flag	|= OBD_RECT_NOHIT;
	}
}

// =======================================================================
// gmBoss5RocketSetDmgEnable
/*!
  喰らい矩形のON/OFF設定
  
  @param rkt_work	[io]	ロケットワーク
  @param enable		[in]	有効フラグ(TRUE:有効化, FALSE:無効化)
 */
// =======================================================================
void gmBoss5RocketSetDmgEnable(GMS_BOSS5_ROCKET_WORK *rkt_work, BOOL enable)
{

	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)rkt_work;
	
	if (enable) {
		ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag	&= ~OBD_RECT_NOHIT;
	}
	else {
		ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag	|= OBD_RECT_NOHIT;
	}
}

// =======================================================================
// gmBoss5RocketSetRectSize
/*!
  攻撃矩形・くらい矩形のサイズ設定
  
  @param rkt_work	[io]	ロケットワーク
  @param sides_type	[in]	内側・外側タイプ指定(GME_BOSS5_RKT_SIDES_XXX)
  
  @note
  内側の矩形を小さく、外側の矩形を大きい矩形となるように設定を行います。
 */
// =======================================================================
void gmBoss5RocketSetRectSize(GMS_BOSS5_ROCKET_WORK *rkt_work, GME_BOSS5_RKT_SIDES sides_type)
{
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)rkt_work;
	
	switch (sides_type) {
	default:
		MTM_ASSERT(FALSE);
		// no break;
	case GME_BOSS5_RKT_SIDES_DMG_OUTSIDE:
		ObjRectWorkSet(&ene_com->rect_work[GMD_ENEMY_RECT_DEF],
					   GMD_BOSS5_RKT_OUT_RECT_SIZE_LEFT,
					   GMD_BOSS5_RKT_OUT_RECT_SIZE_TOP,
					   GMD_BOSS5_RKT_OUT_RECT_SIZE_RIGHT,
					   GMD_BOSS5_RKT_OUT_RECT_SIZE_BOTTOM);
		ObjRectWorkSet(&ene_com->rect_work[GMD_ENEMY_RECT_ATK],
					   GMD_BOSS5_RKT_IN_RECT_SIZE_LEFT,
					   GMD_BOSS5_RKT_IN_RECT_SIZE_TOP,
					   GMD_BOSS5_RKT_IN_RECT_SIZE_RIGHT,
					   GMD_BOSS5_RKT_IN_RECT_SIZE_BOTTOM);
		break;
		
	case GME_BOSS5_RKT_SIDES_ATK_OUTSIDE:
		ObjRectWorkSet(&ene_com->rect_work[GMD_ENEMY_RECT_ATK],
					   GMD_BOSS5_RKT_OUT_RECT_SIZE_LEFT,
					   GMD_BOSS5_RKT_OUT_RECT_SIZE_TOP,
					   GMD_BOSS5_RKT_OUT_RECT_SIZE_RIGHT,
					   GMD_BOSS5_RKT_OUT_RECT_SIZE_BOTTOM);
		ObjRectWorkSet(&ene_com->rect_work[GMD_ENEMY_RECT_DEF],
					   GMD_BOSS5_RKT_IN_RECT_SIZE_LEFT,
					   GMD_BOSS5_RKT_IN_RECT_SIZE_TOP,
					   GMD_BOSS5_RKT_IN_RECT_SIZE_RIGHT,
					   GMD_BOSS5_RKT_IN_RECT_SIZE_BOTTOM);
		break;
	}
}

// =======================================================================
// gmBoss5RocketInitPlySearch
/*!
  プレイヤーサーチ処理 初期化
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void gmBoss5RocketInitPlySearch(GMS_BOSS5_ROCKET_WORK *rkt_work, Sint32 delay)
{
	MTM_ASSERT(delay < GMD_BOSS5_RKT_PLY_SEARCH_HIST_NUM);
	
	// サーチ遅延フレーム設定
	rkt_work->ply_search_delay	= delay;
	
	// 遅延サーチ初期化
	GmBsCmnInitDelaySearch(&rkt_work->dsearch_work,
						   GmBsCmnGetPlayerObj(),
						   rkt_work->search_hist_buf,
						   GMD_BOSS5_RKT_PLY_SEARCH_HIST_NUM);
}

// =======================================================================
// gmBoss5RocketUpdatePlySearch
/*!
  プレイヤーサーチ処理 更新
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void gmBoss5RocketUpdatePlySearch(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	// 遅延サーチ更新
	GmBsCmnUpdateDelaySearch(&rkt_work->dsearch_work);
}

// =======================================================================
// gmBoss5RocketGetPlySearchPos
/*!
  プレイヤーサーチ 座標取得
  
  @param rkt_work	[io]	ロケットワーク
  @param pos		[out]	座標格納先
 */
// =======================================================================
void gmBoss5RocketGetPlySearchPos(const GMS_BOSS5_ROCKET_WORK *rkt_work, VecFx32 *pos)
{
	GmBsCmnGetDelaySearchPos(&rkt_work->dsearch_work,
							 rkt_work->ply_search_delay,
							 pos);
}

// =======================================================================
// gmBoss5RocketSetPlyRebound
/*!
  プレイヤー跳ね返り設定
  
  @param ply_rect	[io]	プレイヤー側矩形
  @param rkt_rect	[in]	ロケット側矩形
  
  @retval TRUE	本体と反対側に弾かれた
  @retval FALSE	本体側に弾かれた
  
  @note
  プレイヤーの攻撃がロケットにヒットした際のプレイヤー跳ね返り移動の各種パラメータを設定します。
  また、プレイヤーが弾かれた方向（本体側or本体と反対側）を返します。
 */
// =======================================================================
BOOL gmBoss5RocketSetPlyRebound(const OBS_RECT_WORK *rkt_rect, OBS_RECT_WORK *ply_rect)
{
	OBS_OBJECT_WORK	*obj_ply	= ply_rect->parent_obj;
	const OBS_OBJECT_WORK	*obj_rkt	= rkt_rect->parent_obj;
	GMS_PLAYER_WORK	*ply_work	= (GMS_PLAYER_WORK*)obj_ply;
	OBS_OBJECT_WORK	*obj_body	= obj_rkt->parent_obj;
	
	// プレイヤー跳ね返り
	GmPlySeqAtkReactionInit(ply_work);
	
	if (ply_work->seq_state == GME_PLY_SEQ_STATE_HOMING_REF) {
		// ジャンプステート設定（ホーミング跳ね返り時のみホーミングアタック禁止）
		GmPlySeqSetJumpState(ply_work, 0,
							 (GMD_PLY_SEQ_SETJUMPSTATE_IGNORE_JUMPBTN |
							  GMD_PLY_SEQ_SETJUMPSTATE_NOHOMING));
		
		/* ホーミングの跳ね返り時は遠くに飛ばす */
		
		Uint32	hit_side	= GmBsCmnCheckRectHitSideVFirst(rkt_rect, ply_rect);
		
		// 水平方向速度設定
		ply_work->obj_work.spd_m	= 0;
		if (hit_side & GMD_BS_CMN_RECT_HIT_SIDE_H_MASK) {
			// 左右側面に当たった場合はその側面方向に弾く
			if (hit_side & GMD_BS_CMN_RECT_HIT_SIDE_LEFT) {
				ply_work->obj_work.spd.x	= -GMD_BOSS5_RKT_PLY_HOMING_REBOUND_X;
			}
			else {
				MTM_ASSERT(hit_side & GMD_BS_CMN_RECT_HIT_SIDE_RIGHT);
				ply_work->obj_work.spd.x	= GMD_BOSS5_RKT_PLY_HOMING_REBOUND_X;
			}
		}
		else {
			// 上下側面に当たった場合は移動方向の逆に弾く
			if (ply_work->obj_work.move.x > 0){
				ply_work->obj_work.spd.x = -GMD_BOSS5_RKT_PLY_HOMING_REBOUND_X;
			}
			else if (ply_work->obj_work.move.x < 0) {
				ply_work->obj_work.spd.x = GMD_BOSS5_RKT_PLY_HOMING_REBOUND_X;
			}
			else {
				// 水平速度が0のときは本体側に飛ばす
				if (ply_work->obj_work.pos.x < obj_body->pos.x) {
					ply_work->obj_work.spd.x = GMD_BOSS5_RKT_PLY_HOMING_REBOUND_X;
				}
				else if (obj_body->pos.x < ply_work->obj_work.pos.x) {
					ply_work->obj_work.spd.x = -GMD_BOSS5_RKT_PLY_HOMING_REBOUND_X;
				}
				else {
					// 本体と全く同じ水平座標ならプレイヤーの向きで決める
					if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
						ply_work->obj_work.spd.x = GMD_BOSS5_RKT_PLY_HOMING_REBOUND_X;
					}
					else {
						ply_work->obj_work.spd.x = -GMD_BOSS5_RKT_PLY_HOMING_REBOUND_X;
					}
				}
			}
		}
		
		// 垂直方向速度設定
		ply_work->obj_work.spd.y	= GMD_BOSS5_RKT_PLY_HOMING_REBOUND_Y;
		
		// ジャンプ中移動禁止時間設定
		GmPlySeqSetNoJumpMoveTime(ply_work, GMD_BOSS5_RKT_PLY_HOMING_REBOUND_NOJUMPMOVE_TIME);
	}
	else {
		
		Uint32	hit_side	= GmBsCmnCheckRectHitSideVFirst(rkt_rect, ply_rect);
		
		ply_work->obj_work.spd_m	= 0;
		if (hit_side & GMD_BS_CMN_RECT_HIT_SIDE_H_MASK) {
			// 左右側面に当たった場合はその側面方向に弾く
			if (hit_side & GMD_BS_CMN_RECT_HIT_SIDE_LEFT) {
				ply_work->obj_work.spd.x = -GMD_BOSS5_RKT_PLY_NML_REBOUND_X;
			}
			else {
				MTM_ASSERT(hit_side & GMD_BS_CMN_RECT_HIT_SIDE_RIGHT);
				ply_work->obj_work.spd.x = GMD_BOSS5_RKT_PLY_NML_REBOUND_X;
			}
		}
		else {
			// 上下側面に当たった場合は移動方向の逆に弾く
			if (ply_work->obj_work.move.x > 0){
				ply_work->obj_work.spd.x = -GMD_BOSS5_RKT_PLY_NML_REBOUND_X;
			}
			else if (ply_work->obj_work.move.x < 0) {
				ply_work->obj_work.spd.x = GMD_BOSS5_RKT_PLY_NML_REBOUND_X;
			}
			else {
				// 水平速度が0のときは本体側に飛ばす
				if (ply_work->obj_work.pos.x < obj_body->pos.x) {
					ply_work->obj_work.spd.x = GMD_BOSS5_RKT_PLY_NML_REBOUND_X;
				}
				else if (obj_body->pos.x < ply_work->obj_work.pos.x) {
					ply_work->obj_work.spd.x = -GMD_BOSS5_RKT_PLY_NML_REBOUND_X;
				}
				else {
					// 本体と全く同じ水平座標ならプレイヤーの向きで決める
					if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
						ply_work->obj_work.spd.x = GMD_BOSS5_RKT_PLY_NML_REBOUND_X;
					}
					else {
						ply_work->obj_work.spd.x = -GMD_BOSS5_RKT_PLY_NML_REBOUND_X;
					}
				}
			}
		}
		
		// 垂直方向速度設定
		ply_work->obj_work.spd.y	= GMD_BOSS5_RKT_PLY_NML_REBOUND_Y;
		
		// ジャンプ中移動禁止時間設定
		GmPlySeqSetNoJumpMoveTime(ply_work, GMD_BOSS5_RKT_PLY_NML_REBOUND_NOJUMPMOVE_TIME);
		
		// ヒット無効時間分だけホーミングを禁止する
		// （食らい矩形が無効の時にホーミングで突き抜けないようにするため）
		ply_work->homing_timer	= (fx32)(GMD_BOSS5_RKT_DMG_NO_HIT_TIME * FX32_ONE);
	}
	
#if _IPHONE
	// 常に成功判定
	return TRUE;
#else
	// プレイヤーの弾かれた方向チェック
	if (obj_rkt->pos.x < obj_body->pos.x) {
		// ロケットが本体よりも左側
		if (obj_ply->spd.x > 0) {
			return FALSE;
		}
		else {
			return TRUE;
		}
	}
	else {
		// ロケットが本体よりも右側
		if (obj_ply->spd.x < 0) {
			return FALSE;
		}
		else {
			return TRUE;
		}
	}
#endif // _IPHONE
}

// =======================================================================
// gmBoss5RocketUpdateRocketStuckWithArm
/*!
  ロケットパンチの本体腕ノード追随
  
  @param rkt_work	[io]	ロケットワーク
  @param b_rotation	[in]	回転反映フラグ（TRUE:回転を反映, FALSE:回転反映しない）
 */
// =======================================================================
void gmBoss5RocketUpdateRocketStuckWithArm(GMS_BOSS5_ROCKET_WORK *rkt_work, BOOL b_rotation)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	NNS_MATRIX	*rkt_ofst_mtx;
	
	// ロケット配置オフセット取得
	if (parent_body->flag & GMD_BOSS5_BODY_FLAG_USE_RKT_OFST) {
		GME_BOSS5_ARM_TYPE	arm_type;
		
		if (rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_RIGHT) {
			arm_type	= GME_BOSS5_ARM_TYPE_RIGHT;
		}
		else {
			arm_type	= GME_BOSS5_ARM_TYPE_LEFT;
		}
		
		rkt_ofst_mtx	= &parent_body->rkt_ofst_mtx[arm_type];
	}
	else {
		rkt_ofst_mtx	= NULL;
	}
	
	// 指定ノードに位置調整
	if (parent_body->adj_hgap_is_active) {
		/*
		  モーションブレンドによるズレの補正は
		  Translateモーションによる移動をobj_workの移動で打ち消しているので、
		  Releativeを使用すると、ブレンド中にTranslateの1フレ分の移動値分だけ
		  常にズレている状態になってしまう。
		  そのため、ズレ補正中はRelativeを使用せずに通常のStuckWithNodeを使用。
		 */
		GmBsCmnUpdateObject3DNNStuckWithNode(obj_work,
											 &parent_body->snm_work,
											 rkt_work->arm_snm_id,
											 b_rotation,
											 rkt_ofst_mtx);
	}
	else {
		// 通常時は相対座標配置版を使用
		GmBsCmnUpdateObject3DNNStuckWithNodeRelative(obj_work,
													 &parent_body->snm_work,
													 rkt_work->arm_snm_id,
													 b_rotation,
													 &obj_work->parent_obj->pos,
													 &parent_body->pivot_prev_pos,
													 rkt_ofst_mtx);
	}
	
	// 右腕の場合は真逆方向を向かせる
	if (b_rotation && rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_RIGHT) {
		
		// GmBsCmnUpdateObject3DNNStuckWithNode()でuser_obj_mtr_rで使用しているため、
		// 上書きせずに、反転用のマトリクスをさらに右から掛ける
		obj_work->disp_flag	|= OBD_DISP_USERMTX_RIGHT;
		nnRotateYMatrix(&obj_work->obj_3d->user_obj_mtx_r,
						&obj_work->obj_3d->user_obj_mtx_r,
						AKM_DEGtoA32(180));
	}
}

// =======================================================================
// gmBoss5RocketInitRocketStuckWithArmLerpRot
/*!
  ロケットパンチの本体腕ノード追随 回転反映移行補間 初期化
  
  @param rkt_work	[io]	ロケットワーク
  @param ratio_spd	[in]	補間の進捗速度
  
  @note
  回転を反映した姿勢へ、補間しつつ移行します。
 */
// =======================================================================
void gmBoss5RocketInitRocketStuckWithArmLerpRot(GMS_BOSS5_ROCKET_WORK *rkt_work,
												Float ratio_spd)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	const NNS_MATRIX	*src_w_mtx;
	NNS_MATRIX	src_rot_mtx;
	
	MTM_ASSERT(ratio_spd > 0.f);
	
	/* 補間元姿勢を記録 */
	
	// 爪の回転姿勢を腕全体の回転姿勢とみなして取得
	src_w_mtx	= GmBsCmnGetSNMMtx(&rkt_work->snm_work,
								   rkt_work->drill_snm_reg_id);
	// 現在の回転姿勢のクォータニオンを取得
	AkMathNormalizeMtx(&src_rot_mtx, src_w_mtx);
	nnMakeRotateMatrixQuaternion(&rkt_work->stuck_lerp_src_quat,
								 &src_rot_mtx);
	
	/* 姿勢初期化 */
	
	// 位置だけ反映
	gmBoss5RocketUpdateRocketStuckWithArm(rkt_work, FALSE);
	
	// オブジェクトの角度クリア
	obj_work->dir.x	=
		obj_work->dir.y	=
			obj_work->dir.z	= 0;
	
	// 進捗クリア
	rkt_work->stuck_lerp_ratio	= 0.f;
	
	// 補間進捗速度設定
	rkt_work->stuck_lerp_ratio_spd	= ratio_spd;
}

// =======================================================================
// gmBoss5RocketUpdateRocketStuckWithArmLerpRot
/*!
  ロケットパンチの本体腕ノード追随 回転反映移行補間 更新
  
  @param rkt_work	[io]	ロケットワーク
  
  @retval TRUE	補間移行完了
  @retval FALSE	補間移行中
 */
// =======================================================================
BOOL gmBoss5RocketUpdateRocketStuckWithArmLerpRot(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	const NNS_MATRIX	*dest_w_mtx;
	NNS_MATRIX	dest_rot_mtx;
	NNS_QUATERNION	dest_rot_quat;
	NNS_QUATERNION	slerp_quat;
	BOOL	is_end	= FALSE;
	
	// 進捗更新
	rkt_work->stuck_lerp_ratio	+= rkt_work->stuck_lerp_ratio_spd;
	if (rkt_work->stuck_lerp_ratio >= 1.f) {
		rkt_work->stuck_lerp_ratio	= 1.f;
		is_end	= TRUE;
	}
	
	// 目標姿勢のクォータニオンを取得
	dest_w_mtx	= GmBsCmnGetSNMMtx(&parent_body->snm_work,
								   rkt_work->arm_snm_id);
	// 目標姿勢のクォータニオンを取得
	AkMathNormalizeMtx(&dest_rot_mtx, dest_w_mtx);
	nnMakeRotateMatrixQuaternion(&dest_rot_quat, &dest_rot_mtx);
	if (rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_RIGHT) {
		NNS_QUATERNION	flip_quat;
		nnMakeRotateXYZQuaternion(&flip_quat, 0, AKM_DEGtoA32(180), 0);
		nnMultiplyQuaternion(&dest_rot_quat, &dest_rot_quat, &flip_quat);
	}
	
	// 球面線形補間した回転姿勢を取得
	nnSlerpQuaternion(&slerp_quat,
					  &rkt_work->stuck_lerp_src_quat,
					  &dest_rot_quat,
					  rkt_work->stuck_lerp_ratio);
	
	// 位置だけ反映
	gmBoss5RocketUpdateRocketStuckWithArm(rkt_work, FALSE);
	
	// 回転姿勢を反映
	obj_work->disp_flag	|= OBD_DISP_USERMTX_RIGHT;
	nnQuaternionMatrix(&obj_work->obj_3d->user_obj_mtx_r,
					   &obj_work->obj_3d->user_obj_mtx_r,
					   &slerp_quat);
	
	if (is_end) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss5RocketGetArmNodePosFx
/*!
  対応する腕ノード座標取得
  
  @param rkt_work	[io]	ロケットワーク
  @param pos_out	[out]	座標格納先
  
  @note
  指定したrkt_workに対応する腕ノードの座標をマップ座標系で取得します。
 */
// =======================================================================
void gmBoss5RocketGetArmNodePosFx(const GMS_BOSS5_ROCKET_WORK *rkt_work, VecFx32 *pos_out)
{
	MTM_ASSERT(rkt_work);
	MTM_ASSERT(pos_out);
	
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	NNS_MATRIX	*w_mtx;
	
	// ノードのワールドマトリクス取得
	w_mtx	= GmBsCmnGetSNMMtx(&parent_body->snm_work, rkt_work->arm_snm_id);
	
	// ノードにくっつける
	pos_out->x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3));
	pos_out->y	= -FX_F32_TO_FX32((NNM_MTX(*w_mtx, 1, 3)));
	pos_out->z	= FX_F32_TO_FX32((NNM_MTX(*w_mtx, 2, 3)));
}

// =======================================================================
// gmBoss5RocketSetDispOfst
/*!
  描画オフセット設定
  
  @param rkt_work		[io]	ロケットワーク
  @param disp_ofst		[in]	進行方向オフセット（モデルの描画ローカル座標）
  @param b_pos_slide	[in]	表示上の座標が変わらないように
  								オブジェクト座標を調整する処理の有効フラグ
  
  @note
  ロケットの表示回転中心をずらすために使用します。
  disp_ofst > 0 を設定すると描画ローカル座標で+X方向に描画位置をずらします。
  （ロケットモデルの爪が向いている方向が+Xのため。）
  b_pos_slideがTRUEの場合、OBS_OBJECT_WORK::dir.zを参照して
  描画オフセットによってずれた分だけOBS_OBJECT_WORK::posを移動させます。
  これにより、表示上のズレが無いように表示回転中心をオフセットできます。
 */
// =======================================================================
void gmBoss5RocketSetDispOfst(GMS_BOSS5_ROCKET_WORK *rkt_work, Float disp_ofst,
							  BOOL b_pos_slide)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	MTM_ASSERT(obj_work);
	
	obj_work->disp_flag	|= OBD_DISP_USERMTX_RIGHT;
	
	nnTranslateMatrix(&obj_work->obj_3d->user_obj_mtx_r,
					  &obj_work->obj_3d->user_obj_mtx_r,
					  disp_ofst, 0, 0);
	
	if (b_pos_slide) {
		obj_work->pos.x	-= FX_Mul(FX_F32_TO_FX32(nnCos(obj_work->dir.z) * disp_ofst), g_obj.draw_scale.x);
		obj_work->pos.y	-= FX_Mul(FX_F32_TO_FX32(nnSin(obj_work->dir.z) * disp_ofst), g_obj.draw_scale.y);
	}
}

// =======================================================================
// gmBoss5RocketGetDispOfst
/*!
  描画オフセット取得
  
  @param rkt_work	[in]	ロケットワーク
  @param ofst_out	[out]	オフセット座標格納先（マップ座標系）
  
  @note
  描画するモデルの中心位置を、オブジェクト中心からのオフセット座標として取得します。
  取得する座標はマップ座標系に変換されています。
 */
// =======================================================================
void gmBoss5RocketGetDispOfst(const GMS_BOSS5_ROCKET_WORK *rkt_work, VecFx32 *ofst_pos_out)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	NNS_VECTOR	ofst_pos;
	NNS_MATRIX	mtx;
	
	MTM_ASSERT(rkt_work);
	MTM_ASSERT(ofst_pos_out);
	
	/*
	  REMINDER:
	  現状、OBS_OBJECT_WORK::scale や OBS_OBJECT_WORK::ofst等の計算は
	  端折ってある（考慮していない）ので注意してください。
	  上記のパラメータを変更すると、取得できる値は正しくなくなります。
	 */
	
	// 姿勢変換前のモデル中心のオブジェクト中心からのオフセット座標は(0,0,0)
	amVectorSet(&ofst_pos, 0, 0, 0);
	
	// dir回転姿勢（描画座標系で取得）
	nnMakeRotateXYZMatrix(&mtx,
						  (Angle32)(MTD_MATH_ANGLE_MASK & -obj_work->dir.x),
						  (Angle32)(MTD_MATH_ANGLE_MASK & obj_work->dir.y),
						  (Angle32)(MTD_MATH_ANGLE_MASK & -obj_work->dir.z));
	
	// マップ座標系のスケールに変換するマトリクス
	// （dir回転はスケーリング後の回転なので、dir回転行列よりスケール行列を先＝右に掛ける）
	nnScaleMatrix(&mtx,
				  &mtx,
				  FX_FX32_TO_F32(g_obj.draw_scale.x),
				  FX_FX32_TO_F32(g_obj.draw_scale.y),
				  FX_FX32_TO_F32(g_obj.draw_scale.z));
	
	// スケール&dir回転姿勢は描画用の姿勢変換より後に（＝左から）掛ける
	nnMultiplyMatrix(&mtx, &mtx, &obj_work->obj_3d->user_obj_mtx_r);
	
	// 姿勢変換後の中心位置
	nnTransformVector(&ofst_pos, &mtx, &ofst_pos);
	
	VEC_Set(ofst_pos_out,
			FX_F32_TO_FX32(ofst_pos.x),
			FX_F32_TO_FX32(-ofst_pos.y),
			FX_F32_TO_FX32(ofst_pos.z));
}

// =======================================================================
// gmBoss5RocketInitFlyDestPos
/*!
  目標座標への飛行処理 初期化
  
  @param rkt_work	[io]	ロケットワーク
  @param init_acc	[in]	初期加速度
  @param init_spd	[in]	初速度
  @param max_spd	[in]	最高速度
  @param launch_pos	[in]	発射開始位置
  @param dest_pos	[in]	目標位置
  
  @return 移動方向角度
 */
// =======================================================================
Angle32 gmBoss5RocketInitFlyDestPos(GMS_BOSS5_ROCKET_WORK *rkt_work,
									fx32 init_acc, fx32 init_spd, fx32 max_spd,
									const VecFx32 *launch_pos, const VecFx32 *dest_pos)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	fx32	cos_val;
	fx32	sin_val;
	Angle32	angle;
	
	// 初加速度設定
	rkt_work->acc	= init_acc;
	
	// 最高速度設定
	rkt_work->max_spd	= max_spd;
	
	// 方向を取得
	angle	= nnArcTan2(FX_FX32_TO_F32(dest_pos->y - launch_pos->y),
						FX_FX32_TO_F32(dest_pos->x - launch_pos->x));
	
	// 移動角度設定
	rkt_work->move_dir	= angle;
	
	cos_val	= FX_F32_TO_FX32(nnCos(rkt_work->move_dir));
	sin_val	= FX_F32_TO_FX32(nnSin(rkt_work->move_dir));
	
	// 加速度設定
	obj_work->spd_add.x	= FX_Mul(rkt_work->acc, cos_val);
	obj_work->spd_add.y	= FX_Mul(rkt_work->acc, sin_val);
	obj_work->spd_add.z	= 0;
	
	// 初速度設定
	obj_work->spd.x	= FX_Mul(init_spd, cos_val);
	obj_work->spd.y	= FX_Mul(init_spd, sin_val);
	
	// 発射開始位置、目標位置設定
	rkt_work->launch_pos	= *launch_pos;
	rkt_work->dest_pos		= *dest_pos;
	
	return angle;
}

// =======================================================================
// gmBoss5RocketInitFlyDestDistance
/*!
  角度＋距離指定による目標座標への飛行処理 初期化
  
  @param rkt_work	[io]	ロケットワーク
  @param init_acc	[in]	初期加速度
  @param init_spd	[in]	初速度
  @param max_spd	[in]	最高速度
  @param launch_pos	[in]	発射開始位置
  @param angle		[in]	移動方向角度
  @param distance	[in]	launch_posからの距離
 */
// =======================================================================
void gmBoss5RocketInitFlyDestDistance(GMS_BOSS5_ROCKET_WORK *rkt_work,
									  fx32 init_acc, fx32 init_spd, fx32 max_spd,
									  const VecFx32 *launch_pos,
									  Angle32 angle, fx32 distance)
{
	VecFx32	dest_pos;
	
	// 角度と距離から目標座標を計算
	dest_pos.x	= launch_pos->x + FX_Mul(distance, FX_F32_TO_FX32(nnCos(angle)));
	dest_pos.y	= launch_pos->y + FX_Mul(distance, FX_F32_TO_FX32(nnSin(angle)));
	dest_pos.z	= 0;
	
	gmBoss5RocketInitFlyDestPos(rkt_work, init_acc, init_spd, max_spd,
								launch_pos, &dest_pos);
}

// =======================================================================
// gmBoss5RocketRedirectFlyDestPos
/*!
  目標座標再設定（座標指定）
  
  @param rkt_work	[io]	ロケットワーク
  @param init_acc	[in]	初期加速度
  @parma dest_pos	[in]	目標座標
  
  @note
  速度を変更せずに目標座標の再設定を行います。
 */
// =======================================================================
void gmBoss5RocketRedirectFlyDestPos(GMS_BOSS5_ROCKET_WORK *rkt_work,
									 fx32 init_acc, const VecFx32 *dest_pos)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	VecFx32	save_spd;
	
	// 速度退避
	save_spd	= obj_work->spd;
	
	// 初期化
	gmBoss5RocketInitFlyDestPos(rkt_work,
								init_acc,
								FX32_ONE,	// 上書きされるので、適当な値を入れておく
								rkt_work->max_spd,
								&obj_work->pos,
								dest_pos);
	
	// 速度を書き戻す
	obj_work->spd	= save_spd;
}

#if (GMD_BOSS5_BOOL_USE_OBSOLETE_FUNCTION)
// =======================================================================
// gmBoss5RocketRedirectFlyDestDistance
/*!
  目標座標再設定（角度＋距離指定）
  
  @param rkt_work	[io]	ロケットワーク
  @param init_acc	[in]	初期加速度
  @param angle		[in]	移動方向角度
  @param distance	[in]	launch_posからの距離
  
  @note
  速度を変更せずに目標座標の再設定を行います。
 */
// =======================================================================
void gmBoss5RocketRedirectFlyDestDistance(GMS_BOSS5_ROCKET_WORK *rkt_work,
										  fx32 init_acc, Angle32 angle, fx32 distance)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	VecFx32	save_spd;
	
	// 速度退避
	save_spd	= obj_work->spd;
	
	// 初期化
	gmBoss5RocketInitFlyDestDistance(rkt_work,
									 init_acc,
									 FX32_ONE,	// 上書きされるので、適当な値を入れておく
									 rkt_work->max_spd,
									 &obj_work->pos,
									 angle,
									 distance);
	
	// 速度を書き戻す
	obj_work->spd	= save_spd;
}
#endif /* (GMD_BOSS5_BOOL_USE_OBSOLETE_FUNCTION) */

// =======================================================================
// gmBoss5RocketUpdateFlyDest
/*!
  目標座標への飛行処理 更新
  
  @param rkt_work		[io]	ロケットワーク
  @param b_mdl_center	[in]	到達判定にモデルの中心の座標を使用
  								（デフォルト：FALSE）
  
  @retval TRUE	目標到達（通過）
  @retval FALSE	目標未到達
  
  @note
  最高速度を超えたら加速を停止しますが、
  速度を最高速度値にきっちり合わせる処理は行いません。
 */
// =======================================================================
BOOL gmBoss5RocketUpdateFlyDest(GMS_BOSS5_ROCKET_WORK *rkt_work, BOOL b_mdl_center/*=FALSE*/)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	NNS_VECTOR	spd_vec;
	NNS_VECTOR	diff_vec;
	fx32	scalar_spd;
	VecFx32	center_pos_adj;	// 参照する中心座標の補正
	
	VEC_Set(&center_pos_adj, 0, 0, 0);
	
	scalar_spd	= FX_Sqrt(FX_Mul(obj_work->spd.x, obj_work->spd.x)
						  + FX_Mul(obj_work->spd.y, obj_work->spd.y));
	
	// 最高速度を超えたら加速停止
	if (scalar_spd >= rkt_work->max_spd) {
		obj_work->spd_add.x	=
			obj_work->spd_add.y	=
				obj_work->spd_add.z	= 0;
	}
	
	
	amVectorSet(&spd_vec,
				FX_FX32_TO_F32(obj_work->spd.x),
				FX_FX32_TO_F32(obj_work->spd.y),
				0);
	
	if (b_mdl_center) {
		// 描画オフセットからモデルの中心を取得
		gmBoss5RocketGetDispOfst(rkt_work, &center_pos_adj);
	}
	
	amVectorSet(&diff_vec,
				FX_FX32_TO_F32(rkt_work->dest_pos.x - (obj_work->pos.x + center_pos_adj.x)),
				FX_FX32_TO_F32(rkt_work->dest_pos.y - (obj_work->pos.y + center_pos_adj.y)),
				0);
	
	if (nnDotProductVector(&spd_vec, &diff_vec) <= 0) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss5RocketInitFlyReverse
/*!
  逆噴射処理 初期化
  
  @param rkt_work	[io]	ロケットワーク
  @param acc_scalar	[in]	現在の速度方向と逆方向にこの加速度を設定
  @param is_add		[in]	ベクトル加算フラグ（デフォルト:FALSE）
  
  @note
  既に設定されている速度方向に対して逆方向に加速を行います。
  ベクトル加算フラグにTRUEを指定すると既存の加速値に加算して設定します。
  省略した場合/FALSE指定した場合は既存の加速値を上書きます。
 */
// =======================================================================
void gmBoss5RocketInitFlyReverse(GMS_BOSS5_ROCKET_WORK *rkt_work,
								 fx32 acc_scalar, BOOL is_add/*=FALSE*/)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	NNS_VECTOR	vec;
	VecFx32	vec_fx;
	
	// 速度と逆方向のベクトルを得る
	amVectorSet(&vec,
				-FX_FX32_TO_F32(obj_work->spd.x),
				-FX_FX32_TO_F32(obj_work->spd.y),
				0);
	nnNormalizeVector(&vec, &vec);	// 方向だけ抽出
	
	// 指定スカラ加速度となる加速度ベクトルを得る
	VEC_Set(&vec_fx,
			FX_Mul(FX_F32_TO_FX32(vec.x), acc_scalar),
			FX_Mul(FX_F32_TO_FX32(vec.y), acc_scalar),
			0);
	
	gmBoss5RocketInitFlyReverseVec(rkt_work, &vec_fx, is_add);
}

// =======================================================================
// gmBoss5RocketInitFlyReverseVec
/*!
  逆噴射処理 初期化
  
  @param rkt_work	[io]	ロケットワーク
  @param acc_vec	[in]	加速ベクトル
  @param is_add		[in]	ベクトル加算フラグ（デフォルト:FALSE）
  
  @note
  逆噴射移動のための加速ベクトルを直接指定します。
  ベクトル加算フラグにTRUEを指定すると既存の加速値に加算して設定します。
  省略した場合/FALSE指定した場合は既存の加速値を上書きます。
 */
// =======================================================================
void gmBoss5RocketInitFlyReverseVec(GMS_BOSS5_ROCKET_WORK *rkt_work,
									const VecFx32 *acc_vec, BOOL is_add/*=FALSE*/)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// 加速方向を設定
	rkt_work->rvs_acc.x	= acc_vec->x;
	rkt_work->rvs_acc.y	= acc_vec->y;
	rkt_work->rvs_acc.z	= 0;
	
	// オブジェクトに反映
	if (is_add) {
		obj_work->spd_add.x	+= rkt_work->rvs_acc.x;
		obj_work->spd_add.y	+= rkt_work->rvs_acc.y;
		obj_work->spd_add.z	= 0;
	}
	else {
		obj_work->spd_add	= rkt_work->rvs_acc;
	}
}

// =======================================================================
// gmBoss5RocketUpdateFlyReverse
/*!
  逆噴射処理 更新
  
  @param rkt_work	[io]	ロケットワーク
  
  @retval TRUE	完了
  @retval FALSE	未完了
 
  @note
  逆噴射前に設定されていた加速方向の速度成分がなくなった時点でTRUEを返します。
 */
// =======================================================================
BOOL gmBoss5RocketUpdateFlyReverse(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	NNS_VECTOR	acc_vec;
	NNS_VECTOR	spd_vec;
	
	amVectorSet(&acc_vec,
				FX_FX32_TO_F32(rkt_work->rvs_acc.x),
				FX_FX32_TO_F32(rkt_work->rvs_acc.y),
				0);
	
	amVectorSet(&spd_vec,
				FX_FX32_TO_F32(obj_work->spd.x),
				FX_FX32_TO_F32(obj_work->spd.y),
				0);
	
	// 加速方向と速度方向の角度差が+-90deg以内になったら速度を打ち消し終えたとみなす
	if (nnDotProductVector(&acc_vec, &spd_vec) >= 0) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}


// =======================================================================
// gmBoss5RocketSetInitialDir
/*!
  初期角度設定
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  本体のフリップ設定に応じてロケットの初期角度を設定します。
 */
// =======================================================================
void gmBoss5RocketSetInitialDir(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	OBS_OBJECT_WORK	*body_obj	= obj_work->parent_obj;
	
	if (body_obj->disp_flag & OBD_DISP_HFLIP) {
		obj_work->dir.z	= (Uint16)GMD_BOSS5_RKT_SEARCH_INITIAL_DIR_Z_L;
	}
	else {
		obj_work->dir.z	= (Uint16)GMD_BOSS5_RKT_SEARCH_INITIAL_DIR_Z_R;
	}
	
	// ノード回転が反映されている時とされていない時とで姿勢が異なるのを
	// 手動で調整して近づける
	// （奥側は見えないので手前にあるとき基準で調整）
	if (rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_RIGHT) {
		obj_work->dir.x	=
			(Uint16)(MTD_MATH_ANGLE_MASK & (obj_work->dir.x +
											GMD_BOSS5_RKT_SEARCH_INITIAL_ADJ_DIR_X_RA));
	}
	else {
		MTM_ASSERT(rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_LEFT);
		obj_work->dir.x	=
			(Uint16)(MTD_MATH_ANGLE_MASK & (obj_work->dir.x +
											GMD_BOSS5_RKT_SEARCH_INITIAL_ADJ_DIR_X_LA));
	}
}

// =======================================================================
// gmBoss5RocketUpdateDirFollowingPos
/*!
  指定座標の方向にオブジェクトの角度を更新
  
  @param rkt_work		[io]	ロケットワーク
  @param targ_pos		[in]	目標座標
  @param deg			[in]	最大回転速度（degree値）
  @param is_reverse		[in]	逆向きフラグ
  @param force_rot_spd	[in]	強制回転速度（0ならば無視, default=0）
  
  @note
  force_rot_spdを指定すると、回転速度が指定された角度に強制的に上書かれます。
 */
// =======================================================================
void gmBoss5RocketUpdateDirFollowingPos(GMS_BOSS5_ROCKET_WORK *rkt_work, const VecFx32 *targ_pos,
										Float deg, BOOL is_reverse, Angle32 force_rot_spd/*=0*/)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	Angle32	angle;
	Angle32	diff_angle;
	Angle32	add_angle;
	
	MTM_ASSERT(deg >= 0.f);
	
	// 逆向きに設定する場合は現在の向きの反対方向を基準に角度計算する
	if (is_reverse) {
		obj_work->dir.z	-= AKM_DEGtoA32(180);
	}
	
	angle	= (Angle32)(MTD_MATH_ANGLE_MASK & nnArcTan2(FX_FX32_TO_F32(targ_pos->y - obj_work->pos.y),
														FX_FX32_TO_F32(targ_pos->x - obj_work->pos.x)));
	
	// 差分角度を取得
	diff_angle	= (Angle32)(MTD_MATH_ANGLE_MASK & (angle - (Angle32)obj_work->dir.z));
	
	// 回転方向を決定
	if (diff_angle >= AKM_DEGtoA32(180)) {
		diff_angle	= -(AKM_DEGtoA32(360) - diff_angle);
		add_angle	= AKM_DEGtoA32(-deg);
	}
	else {
		add_angle	= AKM_DEGtoA32(deg);
	}
	
	// 指定速度未満の角速度ならば差分角度をそのまま適用する
	if (MTM_MATH_ABS(diff_angle) <= MTM_MATH_ABS(add_angle)) {
		add_angle	= diff_angle;
	}
	
	// 逆向き反映
	if (is_reverse) {
		add_angle	+= AKM_DEGtoA32(180);
	}
	
	// 強制回転速度設定
	if (force_rot_spd != 0) {
		add_angle	= force_rot_spd;
	}
	
	obj_work->dir.z	= (Uint16)(Angle16)(MTD_MATH_ANGLE_MASK & ((Angle32)obj_work->dir.z + add_angle));
}

// =======================================================================
// gmBoss5RocketUpdateDirFollowingPos
/*!
  プレイヤーロックオン動作時の角度更新
  
  @param rkt_work		[io]	ロケットワーク
  @param lock_pos			[in]	ロックオン座標
 */
// =======================================================================
void gmBoss5RocketUpdateDirPlyLockOn(GMS_BOSS5_ROCKET_WORK *rkt_work, const VecFx32 *lock_pos)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	OBS_OBJECT_WORK	*body_obj	= obj_work->parent_obj;
	OBS_OBJECT_WORK	*ply_obj	= GmBsCmnGetPlayerObj();
	
	if (body_obj->disp_flag & OBD_DISP_HFLIP) {
		if (ply_obj->pos.x >= obj_work->pos.x) {
			// プレイヤーがロケットより後ろ側にいる場合は上側の上端に向かって回転
			gmBoss5RocketUpdateDirFollowingPos(rkt_work, lock_pos,
											   GMD_BOSS5_RKT_SEARCH_ROT_SPD_DEG,
											   FALSE,
											   AKM_DEGtoA32(GMD_BOSS5_RKT_SEARCH_ROT_SPD_DEG));
		}
		else {
			// プレイヤーがロケットより前なら通常回転
			gmBoss5RocketUpdateDirFollowingPos(rkt_work, lock_pos,
											   GMD_BOSS5_RKT_SEARCH_ROT_SPD_DEG, FALSE);
		}
		
		// 角度制限
		gmBoss5RocketLimitDir(rkt_work,
							  GMD_BOSS5_RKT_LOCKON_DIR_LIMIT_L_START,
							  GMD_BOSS5_RKT_LOCKON_DIR_LIMIT_L_END);
	}
	else {
		if (ply_obj->pos.x <= obj_work->pos.x) {
			// プレイヤーがロケットより後ろ側にいる場合は上側の上端に向かって回転
			gmBoss5RocketUpdateDirFollowingPos(rkt_work, lock_pos,
											   GMD_BOSS5_RKT_SEARCH_ROT_SPD_DEG,
											   FALSE,
											   AKM_DEGtoA32(-GMD_BOSS5_RKT_SEARCH_ROT_SPD_DEG)); // 左回転なので負数
		}
		else {
			// プレイヤーがロケットより前なら通常回転
			gmBoss5RocketUpdateDirFollowingPos(rkt_work, lock_pos,
											   GMD_BOSS5_RKT_SEARCH_ROT_SPD_DEG, FALSE);
		}
		
		// 角度制限
		gmBoss5RocketLimitDir(rkt_work,
							  GMD_BOSS5_RKT_LOCKON_DIR_LIMIT_R_START,
							  GMD_BOSS5_RKT_LOCKON_DIR_LIMIT_R_END);
	}
}

#if (GMD_BOSS5_BOOL_USE_OBSOLETE_FUNCTION)
// =======================================================================
// gmBoss5RocketUpdateDirFollowingSpd
/*!
  速度方向にオブジェクトの角度を更新
  
  @param rkt_work	[io]	ロケットワーク
  @param deg		[in]	最大回転角速度（degree値）
  @param is_reverse	[in]	逆向きフラグ
 */
// =======================================================================
void gmBoss5RocketUpdateDirFollowingSpd(GMS_BOSS5_ROCKET_WORK *rkt_work, Float deg, BOOL is_reverse)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	Angle32	angle;
	Angle32	diff_angle;
	Angle32	add_angle;
	
	MTM_ASSERT(deg >= 0.f);
	
	// 逆向きに設定する場合は現在の向きの反対方向を基準に角度計算する
	if (is_reverse) {
		obj_work->dir.z	-= AKM_DEGtoA32(180);
	}
	
	angle	= (Angle32)(MTD_MATH_ANGLE_MASK & nnArcTan2(FX_FX32_TO_F32(obj_work->spd.y),
														FX_FX32_TO_F32(obj_work->spd.x)));
	
	// 差分角度を取得
	diff_angle	= (Angle32)(MTD_MATH_ANGLE_MASK & (angle - (Angle32)obj_work->dir.z));
	
	// 回転方向を決定
	if (diff_angle >= AKM_DEGtoA32(180)) {
		diff_angle	= -(AKM_DEGtoA32(360) - diff_angle);
		add_angle	= AKM_DEGtoA32(-deg);
	}
	else {
		add_angle	= AKM_DEGtoA32(deg);
	}
	
	// 指定速度未満の角速度ならば差分角度をそのまま適用する
	if (MTM_MATH_ABS(diff_angle) <= MTM_MATH_ABS(add_angle)) {
		add_angle	= diff_angle;
	}
	
	// 逆向き反映
	if (is_reverse) {
		add_angle	+= AKM_DEGtoA32(180);
	}
	
	obj_work->dir.z	= (Uint16)(Angle16)(MTD_MATH_ANGLE_MASK & ((Angle32)obj_work->dir.z + add_angle));
}
#endif /* (GMD_BOSS5_BOOL_USE_OBSOLETE_FUNCTION) */

// =======================================================================
// gmBoss5RocketUpdateDirFollowingAccSpd
/*!
  加速度方向と速度方向の中間方向にオブジェクトの角度を更新
  
  @param rkt_work	[io]	ロケットワーク
  @param deg		[in]	最大回転角速度（degree値）
  @param is_reverse	[in]	逆向きフラグ
 */
// =======================================================================
void gmBoss5RocketUpdateDirFollowingAccSpd(GMS_BOSS5_ROCKET_WORK *rkt_work, Float deg, BOOL is_reverse)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	Angle32	acc_angle_diff;
	Angle32	spd_angle_diff;
	Angle32	angle_add;
	Angle32	diff_angle;
	
	MTM_ASSERT(deg >= 0.f);
	
	// 逆向きに設定する場合は現在の向きの反対方向を基準に角度計算する
	if (is_reverse) {
		obj_work->dir.z	-= AKM_DEGtoA32(180);
	}
	
	// 速度方向との差分角度取得
	{
		Angle32	spd_angle;
		spd_angle	= nnArcTan2(FX_FX32_TO_F32(obj_work->spd.y),
								FX_FX32_TO_F32(obj_work->spd.x));
		
		// 差分角度を取得
		spd_angle_diff	= (Angle32)(MTD_MATH_ANGLE_MASK & (spd_angle - (Angle32)obj_work->dir.z));
		
		if (spd_angle_diff >= AKM_DEGtoA32(180)) {
			spd_angle_diff	= -(AKM_DEGtoA32(360) - spd_angle_diff);
		}
	}
	
	// 加速度方向との差分角度取得
	if (obj_work->spd_add.x != 0 || obj_work->spd_add.y != 0) {
		Angle32	acc_angle;
		acc_angle	= nnArcTan2(FX_FX32_TO_F32(obj_work->spd_add.y),
								FX_FX32_TO_F32(obj_work->spd_add.x));
		
		// 差分角度を取得
		acc_angle_diff	= (Angle32)(MTD_MATH_ANGLE_MASK & (acc_angle - (Angle32)obj_work->dir.z));
		
		// 差分角度の絶対値を180deg以内に収める
		if (acc_angle_diff >= AKM_DEGtoA32(180)) {
			acc_angle_diff	= -(AKM_DEGtoA32(360) - acc_angle_diff);
		}
	}
	else {
		// 加速度が設定されていない場合は速度方向との差分角度を使用
		acc_angle_diff	= spd_angle_diff;
	}
	
	// 加速度方向と速度方向の中間の方向を目標の角度として使用する
	diff_angle	= (Angle32)(MTD_MATH_ANGLE_MASK & (Angle32)((acc_angle_diff + spd_angle_diff) / 2));
	
	// 回転方向を決定
	if (diff_angle >= AKM_DEGtoA32(180)) {
		diff_angle	= -(AKM_DEGtoA32(360) - diff_angle);
		angle_add	= AKM_DEGtoA32(-deg);
	}
	else {
		angle_add	= AKM_DEGtoA32(deg);
	}
	
	// 指定速度未満の角速度ならば差分角度をそのまま適用する
	if (MTM_MATH_ABS(diff_angle) < MTM_MATH_ABS(angle_add)) {
		angle_add	= diff_angle;
	}
	
	// 逆向き反映
	if (is_reverse) {
		angle_add	+= AKM_DEGtoA32(180);
	}
	
	obj_work->dir.z	= (Uint16)(Angle16)(MTD_MATH_ANGLE_MASK & ((Angle32)obj_work->dir.z + angle_add));
}

// =======================================================================
// gmBoss5RocketLimitDir
/*!
  角度制限
  
  @param rkt_work		[io]	ロケットワーク
  @param start_angle	[in]	範囲開始角度
  @param end_angle		[in]	範囲終了角度
  
  @note
  dir.zを、指定の範囲に収まるように修正します。
  範囲外だった場合は、開始・終了角度のうち近傍の角度に設定されます。
  角度値はカメラ側から見て時計回り（※左手系のため）です。
 */
// =======================================================================
void gmBoss5RocketLimitDir(GMS_BOSS5_ROCKET_WORK *rkt_work, Angle32 start_angle, Angle32 end_angle)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	Angle32	cur_angle;
	Angle32	diff_angle;
	Angle32	range_angle;
	
	// 現在の角度を取得
	cur_angle	= (Angle32)(MTD_MATH_ANGLE_MASK & (Angle32)obj_work->dir.z);
	
	// 現在の角度の、startからのオフセット角度を取得
	diff_angle	= (Angle32)(MTD_MATH_ANGLE_MASK & (cur_angle - start_angle));
	
	// end角度のstartからのオフセット角度を取得
	range_angle	= (Angle32)(MTD_MATH_ANGLE_MASK & (end_angle - start_angle));
	
	MTM_ASSERT(range_angle >= 0);	// Angle32をMTD_MATH_ANGLE_MASKすると必ず正整数
	
	if (diff_angle > range_angle) {
		// 範囲外だったら範囲内の角度に収める
		
		Angle32	threshold_angle;
		
		// start/endのどちら側に揃えるかを判定する基準角度を取得
		threshold_angle	= (range_angle / 2) + AKM_DEGtoA32(180);
		
		// 近傍の端に揃える
		if (diff_angle >= threshold_angle) {
			obj_work->dir.z	= (Uint16)(Angle16)(MTD_MATH_ANGLE_MASK & start_angle);
		}
		else {
			obj_work->dir.z	= (Uint16)(Angle16)(MTD_MATH_ANGLE_MASK & end_angle);
		}
	}
}

// =======================================================================
// gmBoss5RocketInitDirFalling
/*!
  落下時角度制御 初期化
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void gmBoss5RocketInitDirFalling(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// 基本は下を向く
	rkt_work->pivot_fall_angle	= AKM_DEGtoA32(90);
	
	// ふらつきオフセット角初期化
	rkt_work->wobble_sin_param_angle	= AKM_DEGtoA32(0);
	
	// 初期姿勢
	obj_work->dir.z	= (Uint16)(MTD_MATH_ANGLE_MASK & rkt_work->pivot_fall_angle);
}

// =======================================================================
// gmBoss5RocketUpdateDirFalling
/*!
  落下時角度制御 更新
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void gmBoss5RocketUpdateDirFalling(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// ツメ方向軸回転更新
	obj_work->dir.x	= (Uint16)(MTD_MATH_ANGLE_MASK & ((Angle32)obj_work->dir.x + GMD_BOSS5_RKT_FALL_SPIN_ANGLE_SPD));
	
	// サイン波パラメータ更新
	rkt_work->wobble_sin_param_angle	+= GMD_BOSS5_RKT_FALL_WOBBLE_SIN_PARAM_DEG_SPD;
	rkt_work->wobble_sin_param_angle	= (Angle32)(MTD_MATH_ANGLE_MASK & rkt_work->wobble_sin_param_angle);
	
	// オフセット角度をサイン波で振動させる
	obj_work->dir.z	=
		(Uint16)(MTD_MATH_ANGLE_MASK & (rkt_work->pivot_fall_angle +
										AKM_DEGtoA32(GMD_BOSS5_RKT_FALL_WOBBLE_DEG_MAX * nnSin(rkt_work->wobble_sin_param_angle))));
}

// =======================================================================
// gmBoss5RocketEndDirFalling
/*!
  落下時角度制御 終了
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void gmBoss5RocketEndDirFalling(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// パラメータクリア
	rkt_work->pivot_fall_angle	= 0;
	rkt_work->wobble_sin_param_angle	= 0;
	
	// 基本姿勢
	obj_work->dir.z	= AKM_DEGtoA32(90);
}

// =======================================================================
// gmBoss5RocketInitLeakageFlicker
/*!
  漏電のちらつき処理 初期化
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  漏電エフェクトのオンオフを行います。
 */
// =======================================================================
void gmBoss5RocketInitLeakageFlicker(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	// タイマ初期化
	rkt_work->wfall_atk_toggle_timer	= 0;
	
	// 漏電エフェクト生成しない
	rkt_work->flag	&= ~GMD_BOSS5_RKT_FLAG_LEAKAGE_NEEDED;
}

// =======================================================================
// gmBoss5RocketUpdateLeakageFlicker
/*!
  漏電のちらつき処理 更新
  
  @param rkt_work	[io]	ロケットワーク
  
  @retval TRUE	オフ状態
  @retval FALSE	オン状態
  
  @note
  ちらつきを更新し続けます。終了する場合はgmBoss5RocketClearLeakageFlicker()
  を呼んでください。
 */
// =======================================================================
BOOL gmBoss5RocketUpdateLeakageFlicker(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	if (rkt_work->wfall_atk_toggle_timer) {
		rkt_work->wfall_atk_toggle_timer--;
	}
	else {
		rkt_work->wfall_atk_toggle_timer	=
			GMD_BOSS5_RKT_LEAKAGE_ON_TIME +
				GMD_BOSS5_RKT_LEAKAGE_OFF_TIME - 1;
	}
	
	// 既定時間間隔でON/OFF切り替える
	if (rkt_work->wfall_atk_toggle_timer < GMD_BOSS5_RKT_LEAKAGE_OFF_TIME) {
		// 漏電エフェクト生成しない
		rkt_work->flag	&= ~GMD_BOSS5_RKT_FLAG_LEAKAGE_NEEDED;
		
		return FALSE;
	}
	else {
		// 漏電エフェクト生成する
		rkt_work->flag	|= GMD_BOSS5_RKT_FLAG_LEAKAGE_NEEDED;
		
		return TRUE;
	}
}


// =======================================================================
// gmBoss5RocketClearLeakageFlicker
/*!
  漏電のちらつき処理 クリア
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  ちらつき処理を終了する際は必ず呼び出してください。
 */
// =======================================================================
void gmBoss5RocketClearLeakageFlicker(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	// タイマクリア
	rkt_work->wfall_atk_toggle_timer	= 0;
	
	// 漏電エフェクト生成しない
	rkt_work->flag	&= ~GMD_BOSS5_RKT_FLAG_LEAKAGE_NEEDED;
}

// =======================================================================
// gmBoss5RocketInitFlyBlow
/*!
  吹っ飛び移動処理 初期化
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  回転しながら一定方向に移動させます。
 */
// =======================================================================
void gmBoss5RocketInitFlyBlow(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	Angle32	angle;
	fx32	cos_val;
	fx32	sin_val;
	
	// 空中移動フラグ設定
	obj_work->move_flag	|= OBD_MOVE_JUMP;
	obj_work->move_flag	&= ~(OBD_MOVE_UNDER | OBD_MOVE_FALL);
	
	// 発射位置設定
	rkt_work->launch_pos	= obj_work->pos;
	
	// 目標設定
	rkt_work->dest_pos	= parent_body->part_obj_core->pos;
	
	// 方向を取得
	angle	= nnArcTan2(FX_FX32_TO_F32(rkt_work->dest_pos.y - obj_work->pos.y),
						FX_FX32_TO_F32(rkt_work->dest_pos.x - obj_work->pos.x));
	
	cos_val	= FX_F32_TO_FX32(nnCos(angle));
	sin_val	= FX_F32_TO_FX32(nnSin(angle));
	
	// 速度設定
	obj_work->spd.x	= FX_Mul(GMD_BOSS5_RKT_BLOW_FLY_SPD, cos_val);
	obj_work->spd.y	= FX_Mul(GMD_BOSS5_RKT_BLOW_FLY_SPD, sin_val);
	obj_work->spd.z	= 0;
	
	// 回転角速度設定
	if (obj_work->spd.x < 0) {
		rkt_work->rot_spd	= -GMD_BOSS5_RKT_BLOW_FLY_ROT_SPD;
	}
	else {
		rkt_work->rot_spd	= GMD_BOSS5_RKT_BLOW_FLY_ROT_SPD;
	}
}

// =======================================================================
// gmBoss5RocketUpdateFlyBlow
/*!
  吹っ飛び移動処理 更新
  
  @param rkt_work	[io]	ロケットワーク
  
  @retval TRUE	目標通過
  @retval FALSE	目標未到達
 */
// =======================================================================
BOOL gmBoss5RocketUpdateFlyBlow(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	NNS_VECTOR	move_dir_vec;
	NNS_VECTOR	diff_vec;
	
	// 回転処理
	obj_work->dir.z	= (Uint16)(Angle16)(MTD_MATH_ANGLE_MASK & ((Angle32)obj_work->dir.z + rkt_work->rot_spd));
	
	amVectorSet(&move_dir_vec,
				FX_FX32_TO_F32(rkt_work->dest_pos.x - rkt_work->launch_pos.x),
				FX_FX32_TO_F32(rkt_work->dest_pos.y - rkt_work->launch_pos.y),
				0);
	
	amVectorSet(&diff_vec,
				FX_FX32_TO_F32(rkt_work->dest_pos.x - obj_work->pos.x),
				FX_FX32_TO_F32(rkt_work->dest_pos.y - obj_work->pos.y),
				0);
	
	// 通過チェック
	if (nnDotProductVector(&move_dir_vec, &diff_vec) <= 0) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss5RocketInitFlyBounce
/*!
  跳ね返り移動処理 初期化
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  本体ヒット後の跳ね返り動作を行います。
 */
// =======================================================================
void gmBoss5RocketInitFlyBounce(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// 空中移動フラグ設定
	obj_work->move_flag	|= OBD_MOVE_JUMP;
	obj_work->move_flag	&= ~(OBD_MOVE_UNDER | OBD_MOVE_FALL);
	
	fx32	cos_val	= FX_F32_TO_FX32(nnCos(GMD_BOSS5_RKT_BOUNCE_DIR_ANGLE));
	fx32	sin_val	= FX_F32_TO_FX32(nnSin(GMD_BOSS5_RKT_BOUNCE_DIR_ANGLE));
	BOOL	is_left	= FALSE;
	
	if (obj_work->spd.x < 0) {
		is_left	= TRUE;
	}
	
	obj_work->spd.x	= FX_Mul(GMD_BOSS5_RKT_BOUNCE_FLY_SPD, cos_val);
	obj_work->spd.y	= FX_Mul(GMD_BOSS5_RKT_BOUNCE_FLY_SPD, sin_val);
	obj_work->spd.z	= 0;
	
	if (is_left) {
		obj_work->spd.x	= -obj_work->spd.x;
	}
	
	// 回転方向設定
	if (obj_work->spd.x < 0) {
		rkt_work->rot_spd	= -GMD_BOSS5_RKT_BOUNCE_FLY_ROT_SPD;
	}
	else {
		rkt_work->rot_spd	= GMD_BOSS5_RKT_BOUNCE_FLY_ROT_SPD;
	}
	
}

// =======================================================================
// gmBoss5RocketUpdateFlyBounce
/*!
  跳ね返り移動処理 更新
  
  @param rkt_work	[io]	ロケットワーク
  
  @retval TRUE	画面外到達
  @retval FALSE	未到達
 */
// =======================================================================
BOOL gmBoss5RocketUpdateFlyBounce(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// 回転処理
	obj_work->dir.z	= (Uint16)(Angle16)(MTD_MATH_ANGLE_MASK & ((Angle32)obj_work->dir.z + rkt_work->rot_spd));
	
	// 画面外到達チェック
	if (obj_work->pos.y <= GMM_BOSS5_AREA_TOP() - GMD_BOSS5_RKT_HIDE_RADIUS) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss5RocketInitStuckLean
/*!
  突き刺さり状態ロケットの傾き処理 初期化
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  現在のヒット数に応じて傾きを変化させます。
 */
// =======================================================================
void gmBoss5RocketInitStuckLean(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	//基準角度保存
	rkt_work->stuck_dir	= obj_work->dir.z;
	
	// 傾き変化補間係数クリア
	rkt_work->stuck_lean_ratio	= 0.f;
}

// =======================================================================
// gmBoss5RocketUpdateStuckLean
/*!
  突き刺さり状態ロケットの傾き処理 更新
  
  @param rkt_work	[io]	ロケットワーク
  
  @retval TRUE	中断OK
  @retval FALSE	中断NG
 */
// =======================================================================
BOOL gmBoss5RocketUpdateStuckLean(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_rkt	= GMM_BS_OBJ(rkt_work);
	OBS_OBJECT_WORK	*obj_body	= obj_rkt->parent_obj;
	Float	ofst_deg	= 0.f;
	BOOL	is_updated	= TRUE;
	Float	vib_ofst_deg;
	fx32	move_ofst	= 0;
	
	// ヒット時振動更新
	rkt_work->hit_vib_amp_deg	-= GMD_BOSS5_RKT_GRD_STUCK_LEAN_HIT_VIB_AMP_DEG_SUBTRACT;
	if (rkt_work->hit_vib_amp_deg <= 0.f) {
		rkt_work->hit_vib_amp_deg	= 0.f;
	}
	rkt_work->hit_vib_sin_angle	=
		MTD_MATH_ANGLE_MASK & (rkt_work->hit_vib_sin_angle + GMD_BOSS5_RKT_GRD_STUCK_LEAN_HIT_VIB_SIN_ANGLE_ADD);
	vib_ofst_deg	= nnSin(rkt_work->hit_vib_sin_angle) * rkt_work->hit_vib_amp_deg;
	
	
	// 角度取得（ロケットが本体より右にあるとき基準）
	if (rkt_work->flag & GMD_BOSS5_RKT_FLAG_IS_DOWN) {
		// ダウン状態なら本体と逆側に一気に傾ける
		
		Float	prev_ratio	= rkt_work->stuck_lean_ratio;
		
		if (rkt_work->stuck_lean_ratio < 1.f) {
			rkt_work->stuck_lean_ratio	+= GMD_BOSS5_RKT_GRD_STUCK_LEAN_DOWN_RATIO_ADD;
			is_updated	= FALSE;
		}
		
		rkt_work->stuck_lean_ratio	= MTM_MATH_CLIP(rkt_work->stuck_lean_ratio, 0.f, 1.f);
		
		ofst_deg	= GMD_BOSS5_RKT_GRD_STUCK_LEAN_DOWN_OFST_DEG * rkt_work->stuck_lean_ratio - vib_ofst_deg;
		
		// 完全倒れ移動値更新
		move_ofst	= (fx32)(GMD_BOSS5_RKT_GRD_STUCK_LEAN_DOWN_MOVE_DISTANCE * (rkt_work->stuck_lean_ratio - prev_ratio));
		
		// (表示が)地面にめり込まないように、地形下端を延ばして垂直座標をせり上げる
		ObjObjectFieldRectSet(obj_rkt,
							  GMD_BOSS5_RKT_FIELD_RECT_SIZE_LEFT,
							  GMD_BOSS5_RKT_FIELD_RECT_SIZE_TOP,
							  GMD_BOSS5_RKT_FIELD_RECT_SIZE_RIGHT,
							  (Sint16)(GMD_BOSS5_RKT_FIELD_RECT_SIZE_BOTTOM +
									   (Sint16)(rkt_work->stuck_lean_ratio *
												GMD_BOSS5_RKT_GRD_STUCK_LEAN_DOWN_EXTEND_F_COL_HEIGHT)));
	}
	else {
		// ヒット回数に応じて少しずつ傾ける ＆ 振動分のオフセットを加える
		ofst_deg	= (Sint32)rkt_work->hit_count * GMD_BOSS5_RKT_GRD_STUCK_LEAN_OFST_DEG_ADD + vib_ofst_deg;
	}
	
	// ロケットが本体より左側なら逆周り, 移動も逆方向
	if (obj_rkt->pos.x < obj_body->pos.x) {
		ofst_deg	= -ofst_deg;
		move_ofst	= -move_ofst;
	}
	
	// 角度反映
	obj_rkt->dir.z	= (Uint16)(MTD_MATH_ANGLE_MASK & (rkt_work->stuck_dir + AKM_DEGtoA32(ofst_deg)));
	
	// 移動値反映
	obj_rkt->pos.x	+= move_ofst;
	
	return is_updated;
}

// =======================================================================
// gmBoss5RocketSetStuckLeanHitVib
/*!
  突き刺さり状態ロケットの傾き処理 ヒット振動設定
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  ヒット時の振動パラメータを初期化します。
 */
// =======================================================================
void gmBoss5RocketSetStuckLeanHitVib(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	rkt_work->hit_vib_amp_deg	= GMD_BOSS5_RKT_GRD_STUCK_LEAN_HIT_VIB_AMP_DEG_INIT;
	rkt_work->hit_vib_sin_angle	= AKM_DEGtoA32(0);
}

// =======================================================================
// gmBoss5RocketInitScatter
/*!
  飛散処理 初期化
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void gmBoss5RocketInitScatter(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	NNS_MATRIX	init_mtx;
	
	// 飛散パラメータ設定
	GmBoss5ScatterSetFlyParam(obj_work);
	
	// 初期姿勢のユーザマトリクスをクォータニオンにして退避
	AkMathNormalizeMtx(&init_mtx, &obj_work->obj_3d->user_obj_mtx_r);
	nnMakeRotateMatrixQuaternion(&rkt_work->sct_cur_quat, &init_mtx);
	
	
	// 既定回数ひねりを加える差分回転クォータニオンを設定
	nnMakeUnitQuaternion(&rkt_work->sct_spin_quat);
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
		nnMultiplyQuaternion(&rkt_work->sct_spin_quat, &diff_rot, &rkt_work->sct_spin_quat);
	}
}

// =======================================================================
// gmBoss5RocketUpdateScatter
/*!
  飛散処理 更新
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void gmBoss5RocketUpdateScatter(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// 回転更新
	nnMultiplyQuaternion(&rkt_work->sct_cur_quat,
						 &rkt_work->sct_spin_quat,
						 &rkt_work->sct_cur_quat);
	
	nnMakeQuaternionMatrix(&obj_work->obj_3d->user_obj_mtx_r,
						   &rkt_work->sct_cur_quat);
	
	obj_work->disp_flag	|= OBD_DISP_USERMTX_RIGHT;
}

// =======================================================================
// gmBoss5RocketReceiveSignalLaunch
/*!
  発射要求シグナル 受信処理
  
  @param rkt_work	[io]	ロケットワーク
  
  @retval TRUE	受信した
  @retval FALSE	受信していない
 */
// =======================================================================
inline BOOL gmBoss5RocketReceiveSignalLaunch(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	// ロケットタイプに対応したシグナルを受信する
	if (rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_LEFT &&
		parent_body->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_LAUNCH_L) {
		
		parent_body->flag	&= ~GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_LAUNCH_L;
		
		return TRUE;
	}
	else if (rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_RIGHT &&
			 parent_body->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_LAUNCH_R) {
		
		parent_body->flag	&= ~GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_LAUNCH_R;
		
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// gmBoss5RocketReceiveSignalReturn
/*!
  帰還要求シグナル 受信処理
  
  @param rkt_work	[io]	ロケットワーク
  
  @retval TRUE	受信した
  @retval FALSE	受信していない
 */
// =======================================================================
inline BOOL gmBoss5RocketReceiveSignalReturn(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	// ロケットタイプに対応したシグナルを受信する
	if (rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_LEFT &&
		parent_body->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_RETURN_L) {
		
		parent_body->flag	&= ~GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_RETURN_L;
		
		return TRUE;
	}
	else if (rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_RIGHT &&
			 parent_body->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_RETURN_R) {
		
		parent_body->flag	&= ~GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_RETURN_R;
		
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// gmBoss5RocketDispatchSignalReturned
/*!
  帰還通知シグナル 発行
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
inline void gmBoss5RocketDispatchSignalReturned(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	// ロケットタイプに対応したシグナルを発行する
	if (rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_LEFT) {
		MTM_ASSERT(!(parent_body->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_L));
		parent_body->flag	|= GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_L;
	}
	else if (rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_RIGHT) {
		MTM_ASSERT(!(parent_body->flag & GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_R));
		parent_body->flag	|= GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_R;
	}
}

// ============================================================================
// ノード処理関連
// ============================================================================
// =======================================================================
// gmBoss5RocketInitCallbacks
/*!
  ノードコールバック関連初期化
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  初期シーケンスを設定するより前に呼び出してください
 */
// =======================================================================
void gmBoss5RocketInitCallbacks(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// BMCBシステム初期化
	GmBsCmnInitBossMotionCBSystem(obj_work,
								  &rkt_work->bmcb_mgr);
	
	// ノードマトリクス取得初期化
	GmBsCmnCreateSNMWork(&rkt_work->snm_work,
						 obj_work->obj_3d->object,
						 GMD_BOSS5_RKT_NODE_SNM_NUM);
	// モーションコールバックを実行リストに追加
	GmBsCmnAppendBossMotionCallback(&rkt_work->bmcb_mgr,
									&rkt_work->snm_work.bmcb_link);
	
	// ノードマトリクス取得ノード追加
	rkt_work->drill_snm_reg_id	=
		GmBsCmnRegisterSNMNode(&rkt_work->snm_work,
							   GMD_BOSS5_RKT_NODE_IDX_DRILL);
}

// =======================================================================
// gmBoss5RocketReleaseCallbacks
/*!
  ノードコールバック関連解放
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
void gmBoss5RocketReleaseCallbacks(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// ボスモーションコールバックシステム解除・クリア
	GmBsCmnClearBossMotionCBSystem(obj_work);
	
	// ノードマトリクス取得関連解放
	GmBsCmnDeleteSNMWork(&rkt_work->snm_work);
}

// ============================================================================
// 処理関数
// ============================================================================
// =======================================================================
// gmBoss5RocketAtkPlyHitFunc
/*!
  ロケット 対プレイヤー攻撃ヒット時処理関数
  
  @param my_rect	[io]	自分の矩形
  @param your_rect	[io]	相手の矩形
 */
// =======================================================================
void gmBoss5RocketAtkPlyHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{
	GMS_BOSS5_ROCKET_WORK	*rkt_work	= (GMS_BOSS5_ROCKET_WORK*)my_rect->parent_obj;
	
	// ヒット済みを記録
	rkt_work->flag	|= GMD_BOSS5_RKT_FLAG_HIT_DONE;
	
	// エネミー標準攻撃ヒット処理
	GmEnemyDefaultAtkFunc(my_rect, your_rect);
}

// =======================================================================
// gmBoss5RocketAtkBossHitFunc
/*!
  ロケット 対ボス攻撃ヒット時処理関数
  
  @param my_rect	[io]	自分の矩形
  @param your_rect	[io]	相手の矩形
 */
// =======================================================================
void gmBoss5RocketAtkBossHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{
	OBS_OBJECT_WORK	*my_obj	= my_rect->parent_obj;
	OBS_OBJECT_WORK	*your_obj	= your_rect->parent_obj;
	GMS_BOSS5_ROCKET_WORK	*rkt_work	= (GMS_BOSS5_ROCKET_WORK*)my_obj;
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)your_obj;
	
	MTM_ASSERT(your_obj->obj_type == GMD_OBJTYPE_ENEMY);
	
	if (ene_com->eve_rec->id == GMD_EVENT_ID_BOSS5_BODY) {
		// 本体のみ当たる
		
		// これ移行、当たりチェックはしない
		my_obj->flag	|= OBD_OBJECT_NOHIT;
		
		// 跳ね返りシーケンスへ
		gmBoss5RocketBounceProcInit(rkt_work);
	}
	else {
		// 本体でなければ当たらなかったことにする
		ObjRectFuncNoHit(my_rect, your_rect);
	}
}

// =======================================================================
// gmBoss5RocketDamageDefFunc
/*!
  ロケット 喰らい時処理関数
  
  @param my_rect	[io]	自分の矩形
  @param your_rect	[io]	相手の矩形
 */
// =======================================================================
void gmBoss5RocketDamageDefFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{
	GMS_BOSS5_ROCKET_WORK	*rkt_work	= (GMS_BOSS5_ROCKET_WORK*)my_rect->parent_obj;
	OBS_OBJECT_WORK	*your_obj	= (OBS_OBJECT_WORK*)your_rect->parent_obj;
	
	if (your_obj && GMD_OBJTYPE_PLAYER == your_obj->obj_type) {
		GMS_PLAYER_WORK	*ply_work	= (GMS_PLAYER_WORK*)your_obj;
		BOOL	is_ply_bound_far;
		
		if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {
			// プレイヤーが接地状態で攻撃状態の時はロケットは食らわない
			// （スピンダッシュの時は当たらないようにするため。
			//   プレイヤーはダメージを受ける）
			return;
		}
		
		// プレイヤー跳ね返り
//#if _IPHONE
//		is_ply_bound_far    = TRUE; // 強制
//#else
		is_ply_bound_far	= gmBoss5RocketSetPlyRebound(my_rect, your_rect);
//#endif // _IPHONE
		
		if (rkt_work->flag & GMD_BOSS5_RKT_FLAG_GROUND_STUCK) {
			// 地面突き刺さり状態の時は既定回数耐える
			
			// プレイヤーが本体と反対側に弾かれたら、ロケットの「本体から遠い側」に当たったとみなす
			if (is_ply_bound_far) {
				// ヒット回数カウント
				rkt_work->hit_count++;
				
				if (rkt_work->hit_count >= GMD_BOSS5_RKT_BLOW_TRIGGER_HIT_NUM) {
					
					// 漏電クリア
					gmBoss5RocketClearLeakageFlicker(rkt_work);
					
					// 本体へ飛ばす
					gmBoss5RocketBlowProcInit(rkt_work);
				}
				else {
					// ヒット無効時間設定
					gmBoss5RocketSetNoHitTime(rkt_work);
					
					// ヒット時振動セット
					gmBoss5RocketSetStuckLeanHitVib(rkt_work);
				}
			}
			else {
				// 喰らい矩形解除（よってgmBoss5RocketSetNoHitTime()不要）
				gmBoss5RocketSetDmgEnable(rkt_work, FALSE);
				
				// ロケット回転火花エフェクト生成
				GmBoss5EfctCreateRocketRollSpark(rkt_work);
				
				// 完全倒れ状態オン
				rkt_work->flag	|= GMD_BOSS5_RKT_FLAG_IS_DOWN;
			}
		}
		else {
			// 落下中
			
			// 漏電クリア
			gmBoss5RocketClearLeakageFlicker(rkt_work);
			
			// 本体へ飛ばす
			gmBoss5RocketBlowProcInit(rkt_work);
		}
		
		// ダメージSE再生
		GmSoundPlaySE("Boss0_01");
	}
}

// =======================================================================
// gmBoss5RocketOutFunc
/*!
  ロケット 専用描画処理
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  通常の描画に加えて、矩形追随用基準座標の更新も行っています。
 */
// =======================================================================
void gmBoss5RocketOutFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_ROCKET_WORK	*rkt_work	= (GMS_BOSS5_ROCKET_WORK*)obj_work;
	
	// 標準描画処理
	ObjDrawActionSummary(obj_work);
	
	// 矩形追随用基準座標格納（SNM結果格納時と同じタイミングの値となるようにここで格納する）
	rkt_work->pivot_prev_pos	= obj_work->pos;
}

// ============================================================================
// 制御処理
// ============================================================================
// =======================================================================
// gmBoss5RocketMain
/*!
  ロケット メイン処理関数
 
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5RocketMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_ROCKET_WORK	*rkt_work	= (GMS_BOSS5_ROCKET_WORK*)obj_work;
	
	// ヒット無効時間更新
	if (rkt_work->flag & GMD_BOSS5_RKT_FLAG_GROUND_STUCK &&
		!(rkt_work->flag & GMD_BOSS5_RKT_FLAG_IS_DOWN)) {
		// 地面突き刺さり状態かつ、まだダウンしていない時のみ矩形復活する
		gmBoss5RocketUpdateNoHitTime(rkt_work);
	}
	
	// 更新処理
	if (rkt_work->proc_update) {
		rkt_work->proc_update(rkt_work);
	}
	
	// ロケット漏電エフェクト生成チェック
	if (rkt_work->flag & GMD_BOSS5_RKT_FLAG_LEAKAGE_NEEDED) {
		GmBoss5EfctTryStartRocketLeakage(rkt_work);
	}
	else {
		GmBoss5EfctEndRocketLeakage(rkt_work);
	}
	
	// 矩形位置を調整
	gmBoss5RocketUpdateMainRectPosition(rkt_work);
}


// ============================================================================
// シーケンス
// ============================================================================
// =======================================================================
// gmBoss5RocketNmlProc****
/*!
  通常ロケットパンチシーケンス
 
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
// 通常ロケットパンチシーケンス初期化
void gmBoss5RocketNmlProcInit(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	// 角度初期化
	gmBoss5RocketSetInitialDir(rkt_work);
	
	// 攻撃矩形を外側に
	gmBoss5RocketSetRectSize(rkt_work, GME_BOSS5_RKT_SIDES_ATK_OUTSIDE);
	
	// 攻撃矩形解除
	gmBoss5RocketSetAtkEnable(rkt_work, FALSE);
	
	// 喰らい矩形解除
	gmBoss5RocketSetDmgEnable(rkt_work, FALSE);
	
	// プレイヤーサーチ初期化
	gmBoss5RocketInitPlySearch(rkt_work, GMD_BOSS5_RKT_NML_SEARCH_DELAY);
	
	rkt_work->proc_update	= gmBoss5RocketNmlProcUpdateFace;
}

// 通常ロケットパンチシーケンス プレイヤーの方向を向く処理
void gmBoss5RocketNmlProcUpdateFace(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	VecFx32	targ_pos;
	
	// 腕にくっつける
	gmBoss5RocketUpdateRocketStuckWithArm(rkt_work, FALSE);
	
	// プレイヤーサーチ更新
	gmBoss5RocketUpdatePlySearch(rkt_work);
	
	// サーチ位置取得
	gmBoss5RocketGetPlySearchPos(rkt_work, &targ_pos);
	
	// プレイヤーの方を向く
	gmBoss5RocketUpdateDirPlyLockOn(rkt_work, &targ_pos);
	
	// 発射まで待機（自分のロケットタイプに対応した発射シグナルを待つ）
	if (gmBoss5RocketReceiveSignalLaunch(rkt_work)) {
		// 移動開始
		gmBoss5RocketInitFlyDestDistance(rkt_work,
										 GMD_BOSS5_RKT_NML_FLY_INIT_ACC,
										 GMD_BOSS5_RKT_NML_FLY_INIT_SPD,
										 GMD_BOSS5_RKT_NML_FLY_MAX_SPD,
										 &obj_work->pos,
										 obj_work->dir.z,
										 GMD_BOSS5_RKT_NML_FLY_DISTNACE);
		
		// 攻撃矩形有効化
		gmBoss5RocketSetAtkEnable(rkt_work, TRUE);
		
		// 発射エフェクト生成
		GmBoss5EfctCreateRocketLaunch(rkt_work);
		
		// 発射SE再生
		GmSoundPlaySE("FinalBoss07");
		
		// 噴射エフェクト開始
		GmBoss5EfctStartRocketJet(rkt_work);
		
		rkt_work->proc_update	= gmBoss5RocketNmlProcUpdateFly;
	}
}

// 通常ロケットパンチシーケンス 飛行処理
void gmBoss5RocketNmlProcUpdateFly(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	if (gmBoss5RocketUpdateFlyDest(rkt_work)) {
		gmBoss5RocketInitFlyReverse(rkt_work, GMD_BOSS5_RKT_NML_FLY_INIT_DECEL, FALSE);
		
		// 噴射エフェクト終了
		GmBoss5EfctEndRocketJet(rkt_work);
		
		// 逆噴射エフェクト開始
		GmBoss5EfctStartRocketJetReverse(rkt_work);
		
		rkt_work->proc_update	= gmBoss5RocketNmlProcUpdateWaitDecel;
	}
}

// 通常ロケットパンチシーケンス 減速処理
void gmBoss5RocketNmlProcUpdateWaitDecel(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	if (gmBoss5RocketUpdateFlyReverse(rkt_work)) {
		Angle32	angle;
		VecFx32	ret_pos;
		
		GmBsCmnSetObjSpdZero(obj_work);
		
		// 戻り先取得
		gmBoss5RocketGetArmNodePosFx(rkt_work, &ret_pos);
		
		// 移動初期化
		angle	= gmBoss5RocketInitFlyDestPos(rkt_work,
											  GMD_BOSS5_RKT_NML_RET_INIT_ACC,
											  0,//init_spd
											  GMD_BOSS5_RKT_NML_RET_MAX_SPD,
											  &obj_work->pos,
											  &ret_pos);
		
		// 表示角度設定
		obj_work->dir.z	= (Uint16)(Angle16)(MTD_MATH_ANGLE_MASK & (angle + AKM_DEGtoA32(180)));
		
		rkt_work->proc_update	= gmBoss5RocketNmlProcUpdateWaitReturn;
	}
}

// 通常ロケットパンチシーケンス 戻り処理
void gmBoss5RocketNmlProcUpdateWaitReturn(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	if (gmBoss5RocketUpdateFlyDest(rkt_work, TRUE)) {
		
		// 帰還完了シグナル設定
		gmBoss5RocketDispatchSignalReturned(rkt_work);
		
		// 逆噴射エフェクト終了
		// （親につられて直ちに消えるが、念のため）
		GmBoss5EfctEndRocketJet(rkt_work);
		
		// ドッキングエフェクト生成
		{
			GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
			GmBoss5EfctCreateRocketDock(parent_body, rkt_work->rkt_type);
		}
		
		// 攻撃矩形解除
		gmBoss5RocketSetAtkEnable(rkt_work, FALSE);
		
		rkt_work->proc_update	= gmBoss5RocketNmlProcUpdateFinalize;
	}
}

// 通常ロケットパンチシーケンス 終了処理
void gmBoss5RocketNmlProcUpdateFinalize(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	/* 帰還完了シグナルを発行してから腕が再度表示されるまで表示しておくため */
	
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	GmBsCmnSetObjSpdZero(obj_work);
	obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
}

// =======================================================================
// gmBoss5RocketStrProc****
/*!
  強化ロケットパンチシーケンス
 
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
// 強化ロケットパンチシーケンス初期化
void gmBoss5RocketStrProcInit(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	// 角度初期化
	gmBoss5RocketSetInitialDir(rkt_work);
	
	// 最初は攻撃矩形を外側に
	gmBoss5RocketSetRectSize(rkt_work, GME_BOSS5_RKT_SIDES_ATK_OUTSIDE);
	
	// 攻撃矩形解除
	gmBoss5RocketSetAtkEnable(rkt_work, FALSE);
	
	// 喰らい矩形解除
	gmBoss5RocketSetDmgEnable(rkt_work, FALSE);
	
	// プレイヤーサーチ初期化
	gmBoss5RocketInitPlySearch(rkt_work, GMD_BOSS5_RKT_STR_SEARCH_DELAY);
	
	rkt_work->proc_update	= gmBoss5RocketStrProcUpdateFace;
}

// 強化ロケットパンチシーケンス プレイヤーの方向を向く処理
void gmBoss5RocketStrProcUpdateFace(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	VecFx32	targ_pos;
	
	// 腕にくっつける
	gmBoss5RocketUpdateRocketStuckWithArm(rkt_work, FALSE);
	
	// プレイヤーサーチ更新
	gmBoss5RocketUpdatePlySearch(rkt_work);
	
	// サーチ位置取得
	gmBoss5RocketGetPlySearchPos(rkt_work, &targ_pos);
	
	// プレイヤーの方を向く
	gmBoss5RocketUpdateDirPlyLockOn(rkt_work, &targ_pos);
	
	// 帰還要求が発行されたら強制的に復帰処理へ
	if (gmBoss5RocketReceiveSignalReturn(rkt_work)) {
		
		// ノードの本来の回転姿勢に補間しつつ移行する
		gmBoss5RocketInitRocketStuckWithArmLerpRot(rkt_work,
												   GMD_BOSS5_RKT_STR_RECOVER_SLERP_SPD);
		// 初回反映
		gmBoss5RocketUpdateRocketStuckWithArmLerpRot(rkt_work);
		
		rkt_work->proc_update	= gmBoss5RocketStrProcUpdateRecover;
		return;
	}
	
	// 発射まで待機（自分のロケットタイプに対応した発射シグナルを待つ）
	if (gmBoss5RocketReceiveSignalLaunch(rkt_work)) {
		
		// 中心をオフセットする
		// （オフセットした分の距離が差し引かれてしまうので移動開始呼び出しの前にオフセットする）
		gmBoss5RocketSetDispOfst(rkt_work, GMD_BOSS5_RKT_STR_FLY_DISP_OFST, TRUE);
		
		// 移動開始
		gmBoss5RocketInitFlyDestDistance(rkt_work,
										 GMD_BOSS5_RKT_STR_FLY_INIT_ACC,
										 GMD_BOSS5_RKT_STR_FLY_INIT_SPD,
										 GMD_BOSS5_RKT_STR_FLY_MAX_SPD,
										 &obj_work->pos,
										 obj_work->dir.z,
										 GMD_BOSS5_RKT_STR_FLY_DITANCE);
		
		// 攻撃矩形有効化
		gmBoss5RocketSetAtkEnable(rkt_work, TRUE);
		
		// 発射エフェクト生成
		GmBoss5EfctCreateRocketLaunch(rkt_work);
		
		// 発射SE再生
		GmSoundPlaySE("FinalBoss07");
		
		// 噴射エフェクト開始
		GmBoss5EfctStartRocketJet(rkt_work);
		
		rkt_work->proc_update	= gmBoss5RocketStrProcUpdateFlyTarget;
	}
}

// 強化ロケットパンチシーケンス 目標への飛行処理
void gmBoss5RocketStrProcUpdateFlyTarget(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	if (gmBoss5RocketUpdateFlyDest(rkt_work)) {
		OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
		VecFx32	dest_pos;
		VecFx32	rvs_acc;
		
		VEC_Set(&dest_pos, obj_work->pos.x,
				GMM_BOSS5_AREA_TOP() - GMD_BOSS5_RKT_HIDE_RADIUS, 0);
		
		// 上方加速設定
		gmBoss5RocketRedirectFlyDestPos(rkt_work,
										GMD_BOSS5_RKT_STR_FLY_REDIR_ACC,
										&dest_pos);
		
		// 水平方向逆噴射設定
		VEC_Set(&rvs_acc, GMD_BOSS5_RKT_STR_FLY_REV_H_ACC_X, 0, 0);
		if (obj_work->disp_flag & OBD_DISP_HFLIP) {
			rvs_acc.x	= -rvs_acc.x;
		}
		gmBoss5RocketInitFlyReverseVec(rkt_work, &rvs_acc, TRUE);
		
		// 処理関数設定
		rkt_work->proc_update	= gmBoss5RocketStrProcUpdateFlyDecel;
	}
}

// 強化ロケットパンチシーケンス 飛行減速処理
void gmBoss5RocketStrProcUpdateFlyDecel(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	// 移動方向を向く
	gmBoss5RocketUpdateDirFollowingAccSpd(rkt_work, GMD_BOSS5_RKT_STR_FLY_REDIR_ROT_SPD_DEG, FALSE);
	
	
	// 逆噴射が終わったタイミングで上方加速を再設定
	if (gmBoss5RocketUpdateFlyReverse(rkt_work)) {
		OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
		VecFx32	dest_pos;
		
		// 移動目標（腕が画面外に隠れる位置に設定）
		VEC_Set(&dest_pos,
				obj_work->pos.x,
				GMM_BOSS5_AREA_TOP() - GMD_BOSS5_RKT_HIDE_RADIUS, 0);
		
		// 上方移動設定
		gmBoss5RocketInitFlyDestPos(rkt_work,
									GMD_BOSS5_RKT_STR_FLY_ABOVE_INIT_ACC,
									MTM_MATH_ABS(obj_work->spd.y),	// 水平方向速度は0のはずなのでY速度をそのまま採用
									GMD_BOSS5_RKT_STR_FLY_MAX_SPD,
									&obj_work->pos,
									&dest_pos);
		
		// 処理関数設定
		rkt_work->proc_update	= gmBoss5RocketStrProcUpdateFlyAbove;
	}
}

// 強化ロケットパンチシーケンス 上昇飛行処理
void gmBoss5RocketStrProcUpdateFlyAbove(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// 移動方向を向く
	gmBoss5RocketUpdateDirFollowingAccSpd(rkt_work,
										  GMD_BOSS5_RKT_STR_FLY_ABOVE_ROT_SPD_DEG,
										  FALSE);
	
	if (gmBoss5RocketUpdateFlyDest(rkt_work)) {
		// 停止
		GmBsCmnSetObjSpdZero(obj_work);
		
		// 攻撃矩形解除
		gmBoss5RocketSetAtkEnable(rkt_work, FALSE);
		
		// 画面外待機時間設定
		rkt_work->wait_timer	= gmBoss5RocketSeqGetWaitFallTime(rkt_work);
		
		// 噴射エフェクト終了
		GmBoss5EfctEndRocketJet(rkt_work);
		
		// 処理関数設定
		rkt_work->proc_update	= gmBoss5RocketStrProcUpdateWaitFall;
	}
}

// 強化ロケットパンチシーケンス 落下待ち処理
void gmBoss5RocketStrProcUpdateWaitFall(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// タイマ終了の既定時間前までプレイヤーサーチする
	if (rkt_work->wait_timer >= GMD_BOSS5_RKT_STR_NO_SEARCH_TIME) {
		obj_work->pos.x	= GmBsCmnGetPlayerObj()->pos.x;
	}
	
	// 既定時間待機
	if (rkt_work->wait_timer) {
		rkt_work->wait_timer--;
	}
	else {
		GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)obj_work;
		
		// 落下速度設定
		obj_work->spd.y	= GMD_BOSS5_RKT_STR_FALL_INIT_SPD;
		
		// 攻撃矩形を外側に
		gmBoss5RocketSetRectSize(rkt_work, GME_BOSS5_RKT_SIDES_ATK_OUTSIDE);
		
		// 攻撃矩形有効化
		gmBoss5RocketSetAtkEnable(rkt_work, TRUE);
		
		// 食らい矩形有効化
		gmBoss5RocketSetDmgEnable(rkt_work, TRUE);
		
		// 漏電ちらつき初期化
		gmBoss5RocketInitLeakageFlicker(rkt_work);
		
		// 黒煙エフェクト開始
		GmBoss5EfctStartRocketSmoke(rkt_work);
		
		// 落下時角度制御初期化
		gmBoss5RocketInitDirFalling(rkt_work);
		
		// ホーミングアタックの対象に設定
		ene_com->enemy_flag	&= ~GMD_ENEMY_FLAG_NOHOMING;
		
		// 落下開始までにヒットした分は無視
		rkt_work->flag	&= ~GMD_BOSS5_RKT_FLAG_HIT_DONE;
		
		rkt_work->proc_update	= gmBoss5RocketStrProcUpdateFall;
	}
}

// 強化ロケットパンチシーケンス 落下処理
void gmBoss5RocketStrProcUpdateFall(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// 落下時角度制御更新
	gmBoss5RocketUpdateDirFalling(rkt_work);
	
	// 漏電エフェクト＆当たりのオン・オフ制御
	if (gmBoss5RocketUpdateLeakageFlicker(rkt_work)) {
		// 漏電オンなら攻撃優先状態に
		gmBoss5RocketSetRectSize(rkt_work, GME_BOSS5_RKT_SIDES_ATK_OUTSIDE);
	}
	else {
		// 漏電オフなら攻撃食らい優先状態に
		gmBoss5RocketSetRectSize(rkt_work, GME_BOSS5_RKT_SIDES_DMG_OUTSIDE);
	}
	
	if (obj_work->pos.y >= GMM_BOSS5_AREA_TOP()) {
		// エリアの上端を過ぎたら地形当たりオン
		obj_work->move_flag	&= ~OBD_MOVE_NOCOL;
	}
	
	if (obj_work->move_flag & OBD_MOVE_UNDER) {
		// 速度停止
		GmBsCmnSetObjSpdZero(obj_work);
		
		// 地面突き刺さり状態設定
		rkt_work->flag	|= GMD_BOSS5_RKT_FLAG_GROUND_STUCK;
		
		// 食らい矩形を外側に
		gmBoss5RocketSetRectSize(rkt_work, GME_BOSS5_RKT_SIDES_DMG_OUTSIDE);
		
		// 攻撃矩形有効化（内側なので、プレイヤーが非攻撃状態なら当たる）
		gmBoss5RocketSetAtkEnable(rkt_work, TRUE);
		
		// 喰らい矩形有効化
		gmBoss5RocketSetDmgEnable(rkt_work, TRUE);
		
		if (rkt_work->flag & GMD_BOSS5_RKT_FLAG_HIT_DONE) {
			// プレイヤーに攻撃ヒット済み
			rkt_work->wait_timer	= GMD_BOSS5_RKT_GRD_STUCK_TIME_SHORT;
		}
		else {
			// プレイヤーに攻撃ヒットしていない
			rkt_work->wait_timer	= GMD_BOSS5_RKT_GRD_STUCK_TIME_LONG;
		}
		
		// 漏電ちらつき終了
		gmBoss5RocketClearLeakageFlicker(rkt_work);
		
		// 着地エフェクト生成
		GmBoss5EfctCreateRocketLandingShockwave(rkt_work);
		
		// 着地SE再生
		GmSoundPlaySE("FinalBoss13");
		
		// 落下時角度制御終了
		gmBoss5RocketEndDirFalling(rkt_work);
		
		// 倒れ処理初期化
		gmBoss5RocketInitStuckLean(rkt_work);
		
		rkt_work->proc_update	= gmBoss5RocketStrProcUpdateStuck;
	}
}

// 強化ロケットパンチシーケンス 地面刺さり処理
void gmBoss5RocketStrProcUpdateStuck(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	BOOL	is_lean_done	= FALSE;
	
	// 倒れ処理更新
	is_lean_done	= gmBoss5RocketUpdateStuckLean(rkt_work);
	
	// 倒れ状態になったら振動させる
	if (is_lean_done &&
		(rkt_work->flag & GMD_BOSS5_RKT_FLAG_IS_DOWN) &&
		!(rkt_work->flag & GMD_BOSS5_RKT_FLAG_DOWN_VIB_DONE)) {
		gmBoss5RocketSetStuckLeanHitVib(rkt_work);
		rkt_work->flag	|= GMD_BOSS5_RKT_FLAG_DOWN_VIB_DONE;
	}
	
	
	// 完全倒れ状態の時は常時漏電エフェクト有効
	if (rkt_work->flag & GMD_BOSS5_RKT_FLAG_IS_DOWN) {
		rkt_work->flag	|= GMD_BOSS5_RKT_FLAG_LEAKAGE_NEEDED;
	}
	else {
		rkt_work->flag	&= ~GMD_BOSS5_RKT_FLAG_LEAKAGE_NEEDED;
	}
	
	if (rkt_work->wait_timer) {
		rkt_work->wait_timer--;
	}
	else {
		if (is_lean_done) {
			GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)obj_work;
			VecFx32	ret_pos;
			
			// 漏電エフェクト解除
			rkt_work->flag	&= ~GMD_BOSS5_RKT_FLAG_LEAKAGE_NEEDED;
			
			// 黒煙エフェクト終了
			GmBoss5EfctEndRocketSmoke(rkt_work);
			
			// 戻り先取得
			gmBoss5RocketGetArmNodePosFx(rkt_work, &ret_pos);
			
			// 戻り移動開始
			gmBoss5RocketInitFlyDestPos(rkt_work,
										GMD_BOSS5_RKT_STR_RET_INIT_ACC,
										GMD_BOSS5_RKT_STR_RET_INIT_SPD,
										GMD_BOSS5_RKT_STR_FLY_MAX_SPD,
										&obj_work->pos,
										&ret_pos);
			
			// 地面突き刺さり状態解除
			rkt_work->flag	&= ~GMD_BOSS5_RKT_FLAG_GROUND_STUCK;
			
			// 攻撃矩形を外側に
			gmBoss5RocketSetRectSize(rkt_work, GME_BOSS5_RKT_SIDES_ATK_OUTSIDE);
			
			// 攻撃矩形有効化
			gmBoss5RocketSetAtkEnable(rkt_work, TRUE);
			
			// 喰らい矩形解除
			gmBoss5RocketSetDmgEnable(rkt_work, FALSE);
			
			// ホーミングアタックの対象から除外
			ene_com->enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
			
			rkt_work->proc_update	= gmBoss5RocketStrProcUpdateReturn;
		}
	}
}

// 強化ロケットパンチシーケンス 戻り処理
void gmBoss5RocketStrProcUpdateReturn(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// 移動方向を向く
	if (rkt_work->flag & GMD_BOSS5_RKT_FLAG_IS_DOWN) {
		gmBoss5RocketUpdateDirFollowingAccSpd(rkt_work, GMD_BOSS5_RKT_STR_DOWN_RET_ROT_SPD_DEG, TRUE);
	}
	else {
		gmBoss5RocketUpdateDirFollowingAccSpd(rkt_work, GMD_BOSS5_RKT_STR_RET_ROT_SPD_DEG, TRUE);
	}
	
	if (gmBoss5RocketUpdateFlyDest(rkt_work, TRUE)) {
		
		// ドッキングエフェクト生成
		{
			GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
			GmBoss5EfctCreateRocketDock(parent_body, rkt_work->rkt_type);
		}
		
		// 帰還完了シグナル設定
		gmBoss5RocketDispatchSignalReturned(rkt_work);
		
		rkt_work->proc_update	= gmBoss5RocketStrProcUpdateFinalize;
	}
}

// 強化ロケットパンチシーケンス 終了処理
void gmBoss5RocketStrProcUpdateFinalize(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	/* 帰還完了シグナルを発行してから腕が再度表示されるまで表示しておくため */
	
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	GmBsCmnSetObjSpdZero(obj_work);
	obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
}

// 強化ロケットパンチシーケンス 復帰処理（未発射状態からの復帰）
void gmBoss5RocketStrProcUpdateRecover(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	// 本体の腕ノードに位置調整
	gmBoss5RocketUpdateRocketStuckWithArmLerpRot(rkt_work);
	
	// 本体常時接続用の腕が表示状態になったら消す
	if ((rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_LEFT &&
		 !(parent_body->flag & GMD_BOSS5_BODY_FLAG_HIDE_ARM_L)) ||
		(rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_RIGHT &&
		 !(parent_body->flag & GMD_BOSS5_BODY_FLAG_HIDE_ARM_L))) {
		
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss5RocketBlowProc****
/*!
  ロケットパンチ 吹っ飛ばしシーケンス
 
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
// 吹っ飛ばしシーケンス初期化
void gmBoss5RocketBlowProcInit(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)obj_work;
	
	// スコア加算
	GmPlayerAddScore((GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj(),
					 GMD_PLY_SCORE_BOSS5_RKT,
					 obj_work->pos.x, obj_work->pos.y);
	
	// 黒煙エフェクト終了
	GmBoss5EfctEndRocketSmoke(rkt_work);
	
	// 地面突き刺さり状態を解除
	rkt_work->flag	&= ~GMD_BOSS5_RKT_FLAG_GROUND_STUCK;
	
	// ジャンプ状態にする
	obj_work->move_flag	|= OBD_MOVE_JUMP;
	obj_work->move_flag	&= ~OBD_MOVE_UNDER;
	
	// ホーミングアタックの対象から除外
	ene_com->enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
	
	// ボス攻撃用に矩形を設定
	gmBoss5RocketSetAtkBodyRect(rkt_work);
	
	// 攻撃矩形を外側に
	gmBoss5RocketSetRectSize(rkt_work, GME_BOSS5_RKT_SIDES_ATK_OUTSIDE);
	
	// 攻撃矩形有効化
	gmBoss5RocketSetAtkEnable(rkt_work, TRUE);
	
	// 喰らい矩形解除
	gmBoss5RocketSetDmgEnable(rkt_work, FALSE);
	
	// 吹っ飛び移動初期化
	gmBoss5RocketInitFlyBlow(rkt_work);
	
	// 処理関数設定
	rkt_work->proc_update	= gmBoss5RocketBlowProcUpdateFly;
}

// 吹っ飛ばしシーケンス 移動処理
void gmBoss5RocketBlowProcUpdateFly(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	if (gmBoss5RocketUpdateFlyBlow(rkt_work)) {
		
		// 目標地点を通過してしまったら、ヒット待ちに移行
		rkt_work->proc_update	= gmBoss5RocketBlowProcUpdateWaitHit;
	}
}

// 吹っ飛ばしシーケンス ヒット待ち処理
void gmBoss5RocketBlowProcUpdateWaitHit(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// 回転だけ行なっておく
	gmBoss5RocketUpdateFlyBlow(rkt_work);
	
	// 本体にヒットするまで目標地点に留まる
	// （本体食らい矩形の連続ヒット対策で矩形オフ中にすり抜けないようにするため）
	obj_work->pos.x	= rkt_work->dest_pos.x;
	obj_work->pos.y	= rkt_work->dest_pos.y;
	
	GmBsCmnSetObjSpdZero(obj_work);
}

// =======================================================================
// gmBoss5RocketBounceProcInit
/*!
  ロケットパンチ 跳ね返りシーケンス
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  ボス本体にヒットした後の跳ね返り動作です。
 */
// =======================================================================
// 跳ね返りシーケンス 初期化
void gmBoss5RocketBounceProcInit(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)obj_work;
	
	// ジャンプ状態にする
	obj_work->move_flag	|= OBD_MOVE_JUMP;
	obj_work->move_flag	&= ~OBD_MOVE_UNDER;
	
	// ホーミングアタックの対象から除外
	ene_com->enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
	
	// 両矩形無効なので、サイズ指定しない gmBoss5RocketSetRectSize
	
	// 攻撃矩形解除
	gmBoss5RocketSetAtkEnable(rkt_work, FALSE);
	
	// 喰らい矩形解除
	gmBoss5RocketSetDmgEnable(rkt_work, FALSE);
	
	// 跳ね返り移動開始
	gmBoss5RocketInitFlyBounce(rkt_work);
	
	rkt_work->proc_update	= gmBoss5RocketBounceProcUpdateFlyUp;
}

// 跳ね返りシーケンス 上方移動処理
void gmBoss5RocketBounceProcUpdateFlyUp(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// 画面外まで移動
	if (gmBoss5RocketUpdateFlyBounce(rkt_work)) {
		GmBsCmnSetObjSpdZero(obj_work);
		
		rkt_work->proc_update	= gmBoss5RocketBounceProcUpdateWait;
	}
}

// 跳ね返りシーケンス 画面外待機処理
void gmBoss5RocketBounceProcUpdateWait(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// 帰還シグナル待ち
	if (gmBoss5RocketReceiveSignalReturn(rkt_work)) {
		VecFx32	ret_pos;
		
		// 戻り先取得
		gmBoss5RocketGetArmNodePosFx(rkt_work, &ret_pos);
		
		// 移動初期化
		gmBoss5RocketInitFlyDestPos(rkt_work,
									GMD_BOSS5_RKT_BOUNCE_RET_INIT_ACC,
									GMD_BOSS5_RKT_BOUNCE_RET_INIT_SPD,//init_spd
									GMD_BOSS5_RKT_STR_FLY_MAX_SPD,
									&obj_work->pos,
									&ret_pos);
		
		rkt_work->proc_update	= gmBoss5RocketBounceProcUpdateReturn;
	}
}

// 跳ね返りシーケンス 戻り処理
void gmBoss5RocketBounceProcUpdateReturn(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	
	// 移動方向を向く
	gmBoss5RocketUpdateDirFollowingAccSpd(rkt_work, GMD_BOSS5_RKT_BOUNCE_RET_ROT_SPD_DEG, TRUE);
	
	if (gmBoss5RocketUpdateFlyDest(rkt_work, TRUE)) {
		
		// ドッキングエフェクト生成
		{
			GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
			GmBoss5EfctCreateRocketDock(parent_body, rkt_work->rkt_type);
		}
		
		// 帰還完了シグナル設定
		gmBoss5RocketDispatchSignalReturned(rkt_work);
		
		rkt_work->proc_update	= gmBoss5RocketBounceProcUpdateFinalize;
	}
}

// 跳ね返りシーケンス 終了処理
void gmBoss5RocketBounceProcUpdateFinalize(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	/* 帰還完了シグナルを発行してから腕が再度表示されるまで表示しておくため */
	
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	GmBsCmnSetObjSpdZero(obj_work);
	obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
}


// =======================================================================
// gmBoss5RocketCnctProcInit
/*!
  ロケットパンチ 本体接続シーケンス
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  ロケットパンチでないときに本体と一体となっている処理です。
 */
// =======================================================================
// 本体接続シーケンス初期化
void gmBoss5RocketCnctProcInit(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	// 本体の腕ノードに配置
	gmBoss5RocketUpdateRocketStuckWithArm(rkt_work, TRUE);
	
	// 攻撃矩形を外側に（このシーケンスでは食らわないので常に攻撃矩形が外側）
	gmBoss5RocketSetRectSize(rkt_work, GME_BOSS5_RKT_SIDES_ATK_OUTSIDE);
	
	// 攻撃矩形解除
	gmBoss5RocketSetAtkEnable(rkt_work, FALSE);
	
	// 喰らい矩形解除
	gmBoss5RocketSetDmgEnable(rkt_work, FALSE);
	
	// 処理関数設定
	rkt_work->proc_update	= gmBoss5RocketCnctProcUpdateIdle;
}

// 本体接続シーケンス 停滞処理
void gmBoss5RocketCnctProcUpdateIdle(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	// 本体の腕ノードに位置調整
	gmBoss5RocketUpdateRocketStuckWithArm(rkt_work, TRUE);
	
	// 腕非表示フラグチェック
	if ((rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_LEFT && (parent_body->flag & GMD_BOSS5_BODY_FLAG_HIDE_ARM_L)) ||
		(rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_RIGHT && (parent_body->flag & GMD_BOSS5_BODY_FLAG_HIDE_ARM_R))) {
		obj_work->disp_flag	|= OBD_DISP_NODISP;
		
		// 当たり無効化
		gmBoss5RocketSetAtkEnable(rkt_work, FALSE);
	}
	else {
		obj_work->disp_flag	&= ~OBD_DISP_NODISP;
		
		// 当たり設定
		if (parent_body->flag & GMD_BOSS5_BODY_FLAG_RKT_CNCT_ATK_ACTIVE) {
			gmBoss5RocketSetAtkEnable(rkt_work, TRUE);
		}
		else {
			gmBoss5RocketSetAtkEnable(rkt_work, FALSE);
		}
		
		// 飛散開始チェック
		if (parent_body->flag & GMD_BOSS5_BODY_FLAG_RKT_SCATTER_NEEDED) {
			
			// 飛散開始遅延時間を設定
			if (rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_LEFT) {
				rkt_work->wait_timer	= GMD_BOSS5_SCT_ARM_FLY_DELAY_LEFT;
			}
			else {
				MTM_ASSERT(rkt_work->rkt_type == GME_BOSS5_RKT_TYPE_RIGHT);
				rkt_work->wait_timer	= GMD_BOSS5_SCT_ARM_FLY_DELAY_RIGHT;
			}
			
			rkt_work->proc_update	= gmBoss5RocketCnctProcUpdateWaitScatterStart;
		}
	}
}

// 本体接続シーケンス 飛散開始待ち処理
void gmBoss5RocketCnctProcUpdateWaitScatterStart(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	// 本体の腕ノードに位置調整
	gmBoss5RocketUpdateRocketStuckWithArm(rkt_work, TRUE);
	
	// 移動開始まで既定時間待機
	if (rkt_work->wait_timer) {
		rkt_work->wait_timer--;
	}
	else {
		// 飛散処理開始
		gmBoss5RocketInitScatter(rkt_work);
		
		// 消去タイマ設定
		rkt_work->wait_timer	= GMD_BOSS5_SCT_PART_FLY_DELETE_TIME;
		
		rkt_work->proc_update	= gmBoss5RocketCnctProcUpdateScatter;
	}
}

// 本体接続シーケンス 飛散処理
void gmBoss5RocketCnctProcUpdateScatter(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(rkt_work);
	// 飛散処理更新
	gmBoss5RocketUpdateScatter(rkt_work);
	
	// 既定時間後に消去
	if (rkt_work->wait_timer) {
		rkt_work->wait_timer--;
	}
	else {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}


// ############################################################################
// ボスFINAL ロケット 動作シーケンス
// ############################################################################
// =======================================================================
// gmBoss5RocketSeqGetWaitFallTime
/*!
  画面外待機時間取得
  
  @param rkt_work	[io]	ロケットワーク
  
  @return 画面外待機時間
  
  @note
  残りライフ値に応じた待機時間を返します。
  呼出し毎に抽選処理が発生するため、毎回同じ結果になるとは限りません。
 */
// =======================================================================
Uint32 gmBoss5RocketSeqGetWaitFallTime(GMS_BOSS5_ROCKET_WORK *rkt_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)GMM_BS_OBJ(rkt_work)->parent_obj;
	Sint32	life	= parent_body->mgr_work->life;
	const GMS_BOSS5_RKT_SEQ_WAITFALL_INFO	*wfall_info	= NULL;
	fx32	rand_val;
	Sint32	choice_no;
	Sint32	last_valid_choice	= 0;
	fx32	threshold	= 0;
	
	for (Sint32 i = 0; i < GMD_BOSS5_RKT_SEQ_LIFE_LEVEL_NUM; ++i) {
		if (life <= gm_boss5_rkt_seq_wait_fall_time_tbl[i].life_threshold) {
			wfall_info	= &gm_boss5_rkt_seq_wait_fall_time_tbl[i];
			break;
		}
	}
	
	if (wfall_info == NULL) {
		MTM_ASSERT(FALSE);
		return	0;
	}
	
	rand_val	= AkMathRandFx();
	
	// どの選択肢が当たったかチェック
	for (choice_no = 0; choice_no < GMD_BOSS5_RKT_SEQ_WAITFALL_CHOICE_NUM; ++choice_no) {
		fx32	prob	= wfall_info->probability[choice_no];
		
		if (prob > 0) {
			last_valid_choice	= choice_no;
			
			threshold	+= prob;
			
			if (rand_val <= threshold) {
				return wfall_info->frame[choice_no];
			}
		}
	}
	
	// 閾値チェックに漏れた場合
	// 有効な確率値が設定されている選択肢のうち、
	// 最も後方に位置している選択肢を採用します。
	return wfall_info->frame[last_valid_choice];
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
