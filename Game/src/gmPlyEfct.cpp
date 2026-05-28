// ==========================================================================
/*!
  @file gmPlyEfct.cpp
  @brief プレイヤーエフェクト

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmPlyEfct.cpp 20 2011-04-22 12:46:46Z thamada $
  $Date:: 2011-04-22 21:46:46 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"

#include "objObject.h"
#include "gmMain.h"
#include "gmGameDat.h"
#include "gmObj.h"
#include "gmPlayer.h"
#include "gmPlySeq.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectZone.h"
#include "gmComEfct.h"
#include "gmEnemy.h"

#include "gmPlyEfct.h"

//----- Definitions ---------------------------------------------------------
#define GMD_PLY_EFCT_OFST_FRONT_PLAYER	(16.f)
#define GMD_PLY_EFCT_OFST_FRONT_A		((float)(GMD_OBJ_DEFAULT_POS_Z_A_FRONT/FX32_ONE))

// GmPlyEfctCreateBarrier
#define GMD_PLY_EFCT_BARRIER_ADD_OFST_Z						(15)		// 表示優先調整の為


// GmPlyEfctCreateSpinDashCircleBlur
#define GMD_PLY_EFCT_SPIN_DASH_CIRCLE_BLUR_BASE_OFST_Y			(0)
#define GMD_PLY_EFCT_SPIN_DASH_CIRCLE_BLUR_BASE_OFST_Y_PINBALL	(0)		// ピンボールの時用
#define GMD_PLY_EFCT_SPIN_DASH_CIRCLE_BLUR_BASE_OFST_Z			(0)//(15)
//#define GMD_PLY_EFCT_SPIN_DASH_CIRCLE_BLUR_DIST_MAX	(8)
//#define GMD_PLY_EFCT_SPIN_DASH_CIRCLE_BLUR_DIST_MIN	(1)

// GmPlyEfctCreateSpinDashBlur
#define GMD_PLY_EFCT_SPIN_DASH_BLUR_BASE_OFST_Y			(0)
#define GMD_PLY_EFCT_SPIN_DASH_BLUR_BASE_OFST_Y_PINBALL	(1)		// ピンボールの時用
//#define GMD_PLY_EFCT_SPIN_DASH_BLUR_DIST_MAX	(8)
//#define GMD_PLY_EFCT_SPIN_DASH_BLUR_DIST_MIN	(1)

// GmPlyEfctCreateSpinStartBlur
#define GMD_PLY_EFCT_SPIN_START_BLUR_BASE_OFST_Y			(2)
#define GMD_PLY_EFCT_SPIN_START_BLUR_BASE_OFST_Y_PINBALL	(1)		// ピンボールの時用
#define GMD_PLY_EFCT_SPIN_START_BLUR_BASE_OFST_Z			(14)
#define GMD_PLY_EFCT_SPIN_START_BLUR_FRAME					(15)	// 演出時間

// GmPlyEfctCreateSpinJumpBlur
#define GMD_PLY_EFCT_SPIN_JUMP_BLUR_BASE_OFST_Y			(-5)
#define GMD_PLY_EFCT_SPIN_JUMP_BLUR_BASE_OFST_Y_PINBALL	(1)		// ピンボールの時用
#define GMD_PLY_EFCT_SPIN_JUMP_BLUR_DIST_MAX	(8)
#define GMD_PLY_EFCT_SPIN_JUMP_BLUR_DIST_MIN	(1)
#define GMD_PLY_EFCT_SPIN_JUMP_BLUR_PLY_MTN_FRAME		(20)	//!< プレイヤーモーションフレーム

// GmPlyEfctCreateBubble
#define GMD_PLY_EFCT_BUBBLE_SPD_Y_ACC		(-0x0040)		//!< 泡加速度
#define GMD_PLY_EFCT_BUBBLE_SPD_Y_MAX		(-0x10000)		//!< 泡最大速度
#define GMD_PLY_EFCT_BUBBLE_SURFACE_ADJUST	(8*FX32_ONE)	//!< 泡水面クリッピング補正値

// GmPlyEfctCreateRunSpray
#define GMD_PLF_EFCT_RUN_SPRAY_MIN_SPD		(0x1000)			//!< エフェクト生成最低速度
#define GMD_PLF_EFCT_RUN_SPRAY_BIG_SPD		(0x4000)			//!< エフェクト大タイプ生成速度
#define GMD_PLF_EFCT_RUN_SPRAY_OFST_Z		(15)				//!< オフセット表示位置

// GmPlyEfctCreateTrail
/// 軌跡エフェクトカラー
typedef struct tag_GMS_PLY_EFCT_TRAIL_COLOR {
	NNS_RGBA	start_col;
	NNS_RGBA	end_col;
} GMS_PLY_EFCT_TRAIL_COLOR;
/// 軌跡エフェクト設定
typedef struct tag_GMS_PLY_EFCT_TRAIL_SETTING {
	float	start_size;
	float	end_size;
	float	life;
	float	vanish_time;
} GMS_PLY_EFCT_TRAIL_SETTING;
//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmPlyEfctTrailSysMain(MTS_TASK_TCB *tcb);
static void gmPlyEfctBarrierMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctCreateBarrierLost(GMS_PLAYER_WORK *ply_work);
static void gmPlyEfctInvincibleMgrMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctCreateInvincibleCircle(GMS_PLAYER_WORK *ply_work);
static void gmPlyEfctInvincibleCircleMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctCreateInvincibleTail(GMS_PLAYER_WORK *ply_work);
static void gmPlyEfctInvincibleTailMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctInvincibleMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctRollDashMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctSweatMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctRunDustMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctDash1DustMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctDash2DustMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctDash2ImpactMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctSpinDustMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctSpinAddDustMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctSpinDashImpactMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctSpinDashDustMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctSpinDashCircleBlurMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctSpinDashBlurMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctSpinDashBlurDest(MTS_TASK_TCB *tcb);
static void gmPlyEfctBrakeDustMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctJumpDustMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctBubbleMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctWaterCountMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctRunSprayMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctRunSprayDest(MTS_TASK_TCB *tcb);
static void gmPlyEfctHomingImpact01Main(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctHomingCursolMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctSpinStartBlurMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctSpinStartBlurDest(MTS_TASK_TCB *tcb);
static void gmPlyEfctSpinJumpBlurMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctSpinJumpBlurPosAdj(GMS_PLAYER_WORK *ply_work);
static void gmPlyEfctSpinJumpBlurDest(MTS_TASK_TCB *tcb);
static void gmPlyEfctSuperAuraMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctSuperAuraSpinMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctSuperAuraDashMain(OBS_OBJECT_WORK *obj_work);
static void gmPlyEfctSteamPipeMain(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static MTS_TASK_TCB			*gm_ply_efct_trail_sys_tcb = NULL;

// GmPlyEfctCreateTrail
/// ソニック軌跡エフェクトカラー
static const GMS_PLY_EFCT_TRAIL_COLOR gm_ply_efct_trail_color_son = {
	{0.4f, 0.4f, 1.0f, 1.0f},		// スタートカラー
	{0.0f, 0.0f, 1.0f, 1.0f},		// エンドカラー
};
/// スーパーソニック軌跡エフェクトカラー
static const GMS_PLY_EFCT_TRAIL_COLOR gm_ply_efct_trail_color_sson = {
	{0.9f, 0.9f, 0.2f, 0.8f},		// スタートカラー
	{0.5f, 0.5f, 0.0f, 0.6f},		// エンドカラー
};
/// 軌跡エフェクト設定
static const GMS_PLY_EFCT_TRAIL_SETTING gm_ply_efct_trail_setting[GME_PLY_EFCT_TRAIL_TYPE_MAX] = {
	// start_size, end_size, life, vanish_time;
	// ホーミング
	{14.0f, 13.0f, 50.0f, 50.0f},
	// スピンダッシュ
	{14.0f, 13.0f, 50.0f, 50.0f},
//	// ホーミング
//	{20.0f, 5.0f, 50.0f, 15.0f},
//	// スピンダッシュ
//	{20.0f, 5.0f, 50.0f, 15.0f},
};

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// 軌跡エフェクト
// ==========================================================================
// ==========================================================================
// GmPlyEfctTrailSysInit
/*!
 *	プレイヤー 軌跡エフェクトシステム初期化
 */
// ==========================================================================
void GmPlyEfctTrailSysInit(void)
{
#if defined(GMD_DEBUG_NO_CREATE_EFFECT)
	return;
#endif

	MTM_ASSERT(gm_ply_efct_trail_sys_tcb == NULL);

	if (gm_ply_efct_trail_sys_tcb == NULL) {
		gm_ply_efct_trail_sys_tcb = MTM_TASK_MAKE_TCB(gmPlyEfctTrailSysMain, NULL/*dest_func*/,
					0, GMD_TASK_PAUSELEVEL_DEF, GMD_TASK_PRIO_TRAIL_SYS, GMD_TASK_GROUP_TRAIL_SYS,
					0/*work_size*/, "GM_PLY_EF_TRAIL");
	}
}

// ==========================================================================
// GmPlyEfctTrailSysExit
/*!
 *	プレイヤー 軌跡エフェクトシステム終了処理
 */
// ==========================================================================
void GmPlyEfctTrailSysExit(void)
{
	if (gm_ply_efct_trail_sys_tcb) {
		mtTaskClearTcb(gm_ply_efct_trail_sys_tcb);
		gm_ply_efct_trail_sys_tcb = NULL;
	}
}

// ==========================================================================
// GmPlyEfctCreateTrail
/*!
 *	プレイヤー 軌跡エフェクト生成
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	efct_type	[in]	エフェクトタイプ GME_PLY_EFCT_TRAIL_TYPE
 */
// ==========================================================================
void GmPlyEfctCreateTrail(GMS_PLAYER_WORK *ply_work, GME_PLY_EFCT_TRAIL_TYPE efct_type)
{
#if defined(GMD_DEBUG_NO_CREATE_EFFECT)
	return;
#endif

	// 軌跡エフェクト作成
	AMS_TRAIL_PARAM param;
	OBS_DATA_WORK*	obj_work = ObjDataGet(GMD_DWORK_NO_EFFECT_CMN_TEXLIST);
	NNS_TEXLIST*	texlist = (NNS_TEXLIST*)obj_work->pData;
	const GMS_PLY_EFCT_TRAIL_COLOR	*color;

	MTM_ASSERT(ply_work);
	MTM_ASSERT((u32)efct_type < GME_PLY_EFCT_TRAIL_TYPE_MAX);

	// カラー取得
	if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
		color = &gm_ply_efct_trail_color_sson;
	}
	else {
		color = &gm_ply_efct_trail_color_son;
	}

	memset(&param, 0, sizeof(AMS_TRAIL_PARAM));
	param.startColor.r = color->start_col.r;
	param.startColor.g = color->start_col.g;
	param.startColor.b = color->start_col.b;
	param.startColor.a = color->start_col.a;
	param.endColor.r = color->end_col.r;
	param.endColor.g = color->end_col.g;
	param.endColor.b = color->end_col.b;
	param.endColor.a = color->end_col.a;
	param.startSize		= gm_ply_efct_trail_setting[efct_type].start_size;
	param.endSize		= gm_ply_efct_trail_setting[efct_type].end_size;
	param.life			= gm_ply_efct_trail_setting[efct_type].life;
	param.vanish_time	= gm_ply_efct_trail_setting[efct_type].vanish_time;
	param.trail_pos = (AMS_VECTOR3I*)((void*)&ply_work->obj_work.pos);
	param.partsNum = AMD_TRAIL_PARTSMAX-1;
	param.zBias = GMD_OBJ_DEFAULT_POS_Z_B_FRONT;
	param.texId = texlist->nTex-1;
	param.blendType = AMDRAWE_BLENDTYPE_ADD;
	param.zTest = 1;
	amTrailMakeEffect(&param, AMTRE_HANDLE_ACCELL, AMTRE_FLAG_FXPOS);
}


// ==========================================================================
// バリアエフェクト
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateBarrier
/*!
 *	プレイヤー バリアエフェクト
 *
 *	@note
 *		user_work : ID保存
 */
// ==========================================================================
void GmPlyEfctCreateBarrier(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_BARRIER);

	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctBarrierMain;
	efct_work->efct_com.obj_work.user_work = GME_EFCT_CMN_IDX_BARRIER;	// ID保存

	// 表示位置調整
	GmComEfctAddDispOffset(efct_work, 0,
									0,
									GMD_PLY_EFCT_BARRIER_ADD_OFST_Z*FX32_ONE);

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_BARRIER_01);

	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctBarrierMain;
	efct_work->efct_com.obj_work.user_work = GME_EFCT_CMN_IDX_BARRIER_01;	// ID保存

	// 表示位置調整
	GmComEfctAddDispOffset(efct_work, 0,
									0,
						   GMD_PLY_EFCT_BARRIER_ADD_OFST_Z*FX32_ONE);

	// 表示OFFチェック
	if (ply_work->gmk_flag2 & GMD_PLGF2_BARRIER_DISP_OFF) {
		efct_work->efct_com.obj_work.disp_flag |= OBD_DISP_NODISP;
	}
}

// ==========================================================================
// 無敵エフェクト
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateInvincible
/*!
 *	プレイヤー 無敵エフェクト
 *
 *	@note
 *		user_work	: GME_EFCT_CMN_IDX_MUTEKI  生成カウンタ
 *		user_timer	: GME_EFCT_CMN_IDX_MUTEKI2 生成カウンタ
 */
// ==========================================================================
void GmPlyEfctCreateInvincible(GMS_PLAYER_WORK *ply_work)
{
	OBS_OBJECT_WORK	*obj_work;

	// GME_EFCT_CMN_IDX_MUTEKI生成
	gmPlyEfctCreateInvincibleCircle(ply_work);

	// GME_EFCT_CMN_IDX_MUTEKI2生成
	gmPlyEfctCreateInvincibleTail(ply_work);

	// マネージャ生成
	obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_COM_WORK), (OBS_OBJECT_WORK*)ply_work,
							0/*sort_prio*/, "GM_PLY_INV_MGR");

	// メイン処理設定
	obj_work->ppFunc = gmPlyEfctInvincibleMgrMain;
}

// ==========================================================================
// 最速ダッシュ 回転足
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateRollDash
/*!
 *	最速ダッシュ 回転足
 */
// ==========================================================================
void GmPlyEfctCreateRollDash(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (ply_work->player_flag & (GMD_PLF_SUPER_SONIC | GMD_PLF_PINBALL_SONIC)) {
		// スーパーソニックのときは生成しない
		// ピンボール中は生成しない
		return;
	}

	if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
		// 左
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_ROLLDASH_L);
		// 位置調整
		GmComEfctSetDispOffsetF(efct_work, 1.5f, 5.f, GMD_PLY_EFCT_OFST_FRONT_PLAYER);
		//GmComEfctSetDispOffsetF(efct_work, dash_pos_x, dash_pos_y, dash_pos_z);
#if _IPHONE
		efct_work->obj_3des.ecb->drawObjState = OBD_DRAW_CMD_STATE_3DNN; // 描画コマンドを通常へ
#endif // _IPHONE
	}
	else {
		// 右
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_ROLLDASH_R);
		// 位置調整
		GmComEfctSetDispOffsetF(efct_work, -1.5f, 5.f, GMD_PLY_EFCT_OFST_FRONT_PLAYER);
		//GmComEfctSetDispOffsetF(efct_work, -dash_pos_x, dash_pos_y, dash_pos_z);
#if _IPHONE
		efct_work->obj_3des.ecb->drawObjState = OBD_DRAW_CMD_STATE_3DNN; // 描画コマンドを通常へ
#endif // _IPHONE
	}

	//GmEffect3DESSetDispOffset(efct_work, 0, -5.f, 8);



	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctRollDashMain;
}

// ==========================================================================
// 汗
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateSweat
/*!
 *	汗
 *
 *	@note
 *		user_timer	: エフェクト生成間隔
 */
// ==========================================================================
void GmPlyEfctCreateSweat(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_SWEAT);

	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSweatMain;

	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, 10.f, -5.f);
	//GmComEfctSetDispOffset(efct_work, -5*FX32_ONE, -10*FX32_ONE, 8*FX32_ONE);
	GmComEfctSetDispOffsetF(efct_work, -5.f, -10.f, GMD_PLY_EFCT_OFST_FRONT_PLAYER);
}

// ==========================================================================
// 砂煙
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateRunDust
/*!
 *	走り 砂煙
 */
// ==========================================================================
void GmPlyEfctCreateRunDust(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (ply_work->player_flag & GMD_PLF_PINBALL_SONIC) {
		// ピンボール中は生成しない
		return;
	}
//	if (ply_work->player_flag & GMD_PLF_WATER) {
//		// 水中では生成しない
//		return;
//	}

	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_3 &&
			ply_work->player_flag & GMD_PLF_WATER &&
			((ply_work->obj_work.pos.y >> FX32_SHIFT)-GMD_PLAYER_WATER_FACE_UP_OFST >= g_gm_main_system.water_level)) {	// 頭が水中
		efct_work = GmEfctZoneEsCreate(&ply_work->obj_work,
										 GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_RUN_Z3);
	}
	else {
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_RUN);
	}

	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctRunDustMain;

	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, -16.f, -8.f);
	GmComEfctSetDispOffsetF(efct_work, -8.f, 16.f, 0);
	efct_work->efct_com.obj_work.parent_ofst.z = FXM_FLOAT_TO_FX32(GMD_PLY_EFCT_OFST_FRONT_PLAYER);
}

// ==========================================================================
// GmPlyEfctCreateDash1Dust
/*!
 *	ダッシュ1 砂煙
 */
// ==========================================================================
void GmPlyEfctCreateDash1Dust(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (ply_work->player_flag & (GMD_PLF_PINBALL_SONIC | GMD_PLF_SUPER_SONIC)) {
		// ピンボール中は生成しない
		// スーパーソニックのときは生成しない
		return;
	}

	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_3 &&
			ply_work->player_flag & GMD_PLF_WATER &&
			((ply_work->obj_work.pos.y >> FX32_SHIFT)-GMD_PLAYER_WATER_FACE_UP_OFST >= g_gm_main_system.water_level)) {	// 頭が水中
		efct_work = GmEfctZoneEsCreate(&ply_work->obj_work,
										 GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_DASH_Z3);
	}
	else {
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_DASH);
	}

	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctDash1DustMain;

	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, -16.f, -8.f);
	GmComEfctSetDispOffsetF(efct_work, -8.f, 16.f, 0);
	efct_work->efct_com.obj_work.parent_ofst.z = FXM_FLOAT_TO_FX32(GMD_PLY_EFCT_OFST_FRONT_PLAYER);
}

// ==========================================================================
// GmPlyEfctCreateDash2Dust
/*!
 *	ダッシュ2 砂煙
 */
// ==========================================================================
void GmPlyEfctCreateDash2Dust(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (ply_work->player_flag & (GMD_PLF_PINBALL_SONIC | GMD_PLF_SUPER_SONIC)) {
		// ピンボール中は生成しない
		// スーパーソニックのときは生成しない
		return;
	}

	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_3 &&
			ply_work->player_flag & GMD_PLF_WATER &&
			((ply_work->obj_work.pos.y >> FX32_SHIFT)-GMD_PLAYER_WATER_FACE_UP_OFST >= g_gm_main_system.water_level)) {	// 頭が水中
		efct_work = GmEfctZoneEsCreate(&ply_work->obj_work,
										 GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_ROLLDASH_S_Z3);
	}
	else {
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_ROLLDASH_S);
	}

	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctDash2DustMain;

	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, -16.f, -8.f);
	GmComEfctSetDispOffsetF(efct_work, -8.f, 16.f, 0);
	efct_work->efct_com.obj_work.parent_ofst.z = FXM_FLOAT_TO_FX32(GMD_PLY_EFCT_OFST_FRONT_PLAYER);
}

// ==========================================================================
// GmPlyEfctCreateDash2Impact
/*!
 *	ダッシュ2 衝撃波
 */
// ==========================================================================
void GmPlyEfctCreateDash2Impact(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (ply_work->player_flag & (GMD_PLF_PINBALL_SONIC | GMD_PLF_SUPER_SONIC)) {
		// ピンボール中は生成しない
		// スーパーソニックのときは生成しない
		return;
	}

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_ROLLDASH);

//	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctDash2ImpactMain;

	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, -16.f, -8.f);
	GmComEfctSetDispOffsetF(efct_work, -8.f, 16.f, 0);
	efct_work->efct_com.obj_work.parent_ofst.z = FXM_FLOAT_TO_FX32(GMD_PLY_EFCT_OFST_FRONT_PLAYER);
}

// ==========================================================================
// GmPlyEfctCreateSpinDust
/*!
 *	その場スピン 砂煙
 */
// ==========================================================================
void GmPlyEfctCreateSpinDust(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_3 &&
			ply_work->player_flag & GMD_PLF_WATER &&
			((ply_work->obj_work.pos.y >> FX32_SHIFT)-GMD_PLAYER_WATER_FACE_UP_OFST >= g_gm_main_system.water_level)) {	// 頭が水中
		efct_work = GmEfctZoneEsCreate(&ply_work->obj_work, GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_SPIN_02);
	}
	else {
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_SPIN_00);
	}

//	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSpinDustMain;

	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, -16.f, -8.f);
	GmComEfctSetDispOffsetF(efct_work, -8.f, 16.f, 0);
	efct_work->efct_com.obj_work.parent_ofst.z = FXM_FLOAT_TO_FX32(GMD_PLY_EFCT_OFST_FRONT_PLAYER);
}

// ==========================================================================
// GmPlyEfctCreateSpinAddDust
/*!
 *	スピン加速 砂煙
 */
// ==========================================================================
void GmPlyEfctCreateSpinAddDust(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_3 &&
			ply_work->player_flag & GMD_PLF_WATER &&
			((ply_work->obj_work.pos.y >> FX32_SHIFT)-GMD_PLAYER_WATER_FACE_UP_OFST >= g_gm_main_system.water_level)) {	// 頭が水中
		efct_work = GmEfctZoneEsCreate(&ply_work->obj_work, GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_SPIN_03);
	}
	else {
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_SPIN_01);
	}

//	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
//	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSpinAddDustMain;

	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, -16.f, -8.f);
	GmComEfctSetDispOffsetF(efct_work, -8.f, 16.f, 0);
	efct_work->efct_com.obj_work.parent_ofst.z = FXM_FLOAT_TO_FX32(GMD_PLY_EFCT_OFST_FRONT_PLAYER);
}

// ==========================================================================
// GmPlyEfctCreateSpinDashImpact
/*!
 *	スピンダッシュ 衝撃波
 */
// ==========================================================================
void GmPlyEfctCreateSpinDashImpact(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_ROLLDASH);

//	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
//	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSpinDashImpactMain;

	efct_work->efct_com.obj_work.user_timer = 30*FX32_ONE;

	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, -16.f, -8.f);
	GmComEfctSetDispOffsetF(efct_work, -6.f, 16.f, 0);
	efct_work->efct_com.obj_work.parent_ofst.z = FXM_FLOAT_TO_FX32(GMD_PLY_EFCT_OFST_FRONT_PLAYER);
}

// ==========================================================================
// GmPlyEfctCreateSpinDashDust
/*!
 *	スピンダッシュ 砂煙
 */
// ==========================================================================
void GmPlyEfctCreateSpinDashDust(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (ply_work->player_flag & GMD_PLF_PINBALL_SONIC) {
		// ピンボール中は生成しない
		return;
	}

	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_3 &&
			ply_work->player_flag & GMD_PLF_WATER &&
			((ply_work->obj_work.pos.y >> FX32_SHIFT)-GMD_PLAYER_WATER_FACE_UP_OFST >= g_gm_main_system.water_level)) {	// 頭が水中
		efct_work = GmEfctZoneEsCreate(&ply_work->obj_work,
										 GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_ROLLDASH_S_Z3);
	}
	else {
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_ROLLDASH_S);
	}

//	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
//	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSpinDashDustMain;

	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, -16.f, -8.f);
	GmComEfctSetDispOffsetF(efct_work, -8.f, 16.f, 0);
	efct_work->efct_com.obj_work.parent_ofst.z = FXM_FLOAT_TO_FX32(GMD_PLY_EFCT_OFST_FRONT_PLAYER);
}

// ==========================================================================
// GmPlyEfctCreateSpinDashCircleBlur
/*!
 *	スピンダッシュ 円ブラー
 *
 *	@return	GMS_EFFECT_3DES_WORKワーク
 *
 *	@note
 *		user_timer	: アクションタイプ
 */
// ==========================================================================
void* GmPlyEfctCreateSpinDashCircleBlur(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (ply_work->efct_spin_dash_cir_blur &&
			!(ply_work->efct_spin_dash_cir_blur->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST))) {
		// 既に生成済み
		return (NULL);
	}

	if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work,  GME_EFCT_CMN_IDX_SS_SPIN_D);

		// アクションタイプ保存
		efct_work->efct_com.obj_work.user_timer = GME_EFCT_CMN_IDX_SS_SPIN_D;
	}
	else {
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_SPIN_D);

		// アクションタイプ保存
		efct_work->efct_com.obj_work.user_timer = GME_EFCT_CMN_IDX_SPIN_D;
	}
	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSpinDashCircleBlurMain;
//	// 距離設定
//	efct_work->efct_com.obj_work.user_work	= (u32)((float)GMD_PLY_EFCT_SPIN_DASH_CIRCLE_BLUR_DIST_MAX);
//	efct_work->efct_com.obj_work.user_timer	= (s32)((float)GMD_PLY_EFCT_SPIN_DASH_CIRCLE_BLUR_DIST_MIN);
	// 位置調整
	if (  (ply_work->player_flag & GMD_PLF_PINBALL_SONIC) 
		||(GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) ) {
		// スペステとピンボールはオフセット
		GmComEfctSetDispOffset(efct_work, 0,
				GMD_PLY_EFCT_SPIN_DASH_CIRCLE_BLUR_BASE_OFST_Y_PINBALL*FX32_ONE,
				GMD_PLY_EFCT_SPIN_DASH_CIRCLE_BLUR_BASE_OFST_Z*FX32_ONE);
	}
	else {
		GmComEfctSetDispOffset(efct_work, 0,
			GMD_PLY_EFCT_SPIN_DASH_CIRCLE_BLUR_BASE_OFST_Y*FX32_ONE,
			GMD_PLY_EFCT_SPIN_DASH_CIRCLE_BLUR_BASE_OFST_Z*FX32_ONE);
	}

	// 速度取得
	efct_work->efct_com.obj_work.obj_3des->speed = ply_work->obj_work.obj_3d->speed[0];

	// 終了処理差し替え
	mtTaskChangeTcbDestructor(efct_work->efct_com.obj_work.tcb, gmPlyEfctSpinDashBlurDest);

	// 保存
	ply_work->efct_spin_dash_cir_blur = (OBS_OBJECT_WORK*)efct_work;

	return (efct_work);
}


// ==========================================================================
// GmPlyEfctCreateSpinDashBlur
/*!
 *	スピンダッシュ ブラー
 *
 *	@param	type	[in]	0 : 大   1 : 小
 *
 *	@return	GMS_EFFECT_3DES_WORKワーク
 *
 *	@note
 *		user_timer	: アクションタイプ
 */
// ==========================================================================
void* GmPlyEfctCreateSpinDashBlur(GMS_PLAYER_WORK *ply_work, u32 type)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (ply_work->efct_spin_dash_blur &&
			!(ply_work->efct_spin_dash_blur->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST))) {
		// 既に生成済み
		return (NULL);
	}

	if (type) {
		// 小
		if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
			efct_work = GmEfctCmnEsCreate(&ply_work->obj_work,  GME_EFCT_CMN_IDX_SS_SPIN_D_B);

			// アクションタイプ保存
			efct_work->efct_com.obj_work.user_timer = GME_EFCT_CMN_IDX_SS_SPIN_D_B;
		}
		else {
			efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_SPIN_D_B);

			// アクションタイプ保存
			efct_work->efct_com.obj_work.user_timer = GME_EFCT_CMN_IDX_SPIN_D_B;
		}
	}
	else {
		// 大
		if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
			efct_work = GmEfctCmnEsCreate(&ply_work->obj_work,  GME_EFCT_CMN_IDX_SS_SPIN);

			// アクションタイプ保存
			efct_work->efct_com.obj_work.user_timer = GME_EFCT_CMN_IDX_SS_SPIN;
		}
		else {
			efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_SPIN);

			// アクションタイプ保存
			efct_work->efct_com.obj_work.user_timer = GME_EFCT_CMN_IDX_SPIN;
		}
	}

	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSpinDashBlurMain;
	// 距離設定
//	efct_work->efct_com.obj_work.user_work	= (u32)((float)GMD_PLY_EFCT_SPIN_DASH_BLUR_DIST_MAX);
//	efct_work->efct_com.obj_work.user_timer	= (s32)((float)GMD_PLY_EFCT_SPIN_DASH_BLUR_DIST_MIN);
	// 位置調整
	if (  (ply_work->player_flag & GMD_PLF_PINBALL_SONIC) 
		||(GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) ) {
		// スペステとピンボールはオフセット
		GmComEfctSetDispOffset(efct_work, 0,
				GMD_PLY_EFCT_SPIN_DASH_BLUR_BASE_OFST_Y_PINBALL*FX32_ONE,
				0);
	}
	else {
		GmComEfctSetDispOffset(efct_work, 0,
			GMD_PLY_EFCT_SPIN_DASH_BLUR_BASE_OFST_Y*FX32_ONE,
			0);
	}

	// 速度取得
	efct_work->efct_com.obj_work.obj_3des->speed = ply_work->obj_work.obj_3d->speed[0];

	// 終了処理差し替え
	mtTaskChangeTcbDestructor(efct_work->efct_com.obj_work.tcb, gmPlyEfctSpinDashBlurDest);

	// 保存
	ply_work->efct_spin_dash_blur = (OBS_OBJECT_WORK*)efct_work;

	return (efct_work);
}



// ==========================================================================
// GmPlyEfctCreateBrakeImpact
/*!
 *	ブレーキ 衝撃波
 */
// ==========================================================================
void GmPlyEfctCreateBrakeImpact(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (ply_work->player_flag & GMD_PLF_PINBALL_SONIC) {
		// ピンボール中は生成しない
		return;
	}

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_BRAKE);

//	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
//	// メイン処理差し替え
//	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSweatMain;
	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, -16.f, -8.f);
	GmComEfctSetDispOffsetF(efct_work, -8.f, 16.f, 0);
	efct_work->efct_com.obj_work.parent_ofst.z = FXM_FLOAT_TO_FX32(GMD_PLY_EFCT_OFST_FRONT_PLAYER);
}

// ==========================================================================
// GmPlyEfctCreateBrakeDust
/*!
 *	ブレーキ 砂煙
 */
// ==========================================================================
void GmPlyEfctCreateBrakeDust(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (ply_work->player_flag & GMD_PLF_PINBALL_SONIC) {
		// ピンボール中は生成しない
		return;
	}
	if (ply_work->player_flag & GMD_PLF_WATER) {
		// 水中では生成しない
		return;
	}

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_BRAKE_S);

//	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
//	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctBrakeDustMain;
	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, -16.f, -8.f);
	GmComEfctSetDispOffsetF(efct_work, -8.f, 16.f, 0);
	efct_work->efct_com.obj_work.parent_ofst.z = FXM_FLOAT_TO_FX32(GMD_PLY_EFCT_OFST_FRONT_PLAYER);
}

// ==========================================================================
// GmPlyEfctCreateJumpDust
/*!
 *	ジャンプ 砂煙
 */
// ==========================================================================
void GmPlyEfctCreateJumpDust(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {
		// スペステでは生成しない
		return;
	}
	if (ply_work->player_flag & GMD_PLF_WATER) {
		// 水中では生成しない
		return;
	}

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_JUMP);

//	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
//	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctJumpDustMain;
	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, -16.f, -8.f);
	GmComEfctSetDispOffsetF(efct_work, 0, 16.f, GMD_PLY_EFCT_OFST_FRONT_PLAYER);
}

// ==========================================================================
// ZONE依存足元砂煙
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateInvincible
/*!
 *	プレイヤー ZONE依存足元砂煙
 */
// ==========================================================================
void GmPlyEfctCreateFootSmoke(GMS_PLAYER_WORK *ply_work)
{
#if !_IPHONE
	GMS_EFFECT_3DES_WORK	*efct_work = NULL;

	switch (g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id]) {
	case GSD_MAIN_ZONE_TYPE_1:
		efct_work = GmEfctZoneEsCreate(&ply_work->obj_work,
						GSD_MAIN_ZONE_TYPE_1, GME_EFCT_Z01_IDX_SMORK_S_Z1);
		break;
	case GSD_MAIN_ZONE_TYPE_2:
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_SMORK_S_Z2);
		break;
	case GSD_MAIN_ZONE_TYPE_3:
		if (!(ply_work->player_flag & GMD_PLF_WATER)) {
			efct_work = GmEfctZoneEsCreate(&ply_work->obj_work,
							GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_SMORK_S_Z3);
		}
		break;
	case GSD_MAIN_ZONE_TYPE_4:
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_SMORK_S_Z4);
		break;
	case GSD_MAIN_ZONE_TYPE_FINAL:
		break;
	default:
		break;
	}

	if (efct_work) {
		GmComEfctAddDispOffset(efct_work, -4*FX32_ONE, 15*FX32_ONE, 0);
	}
#endif // !_IPHONE
}

// ==========================================================================
// 水しぶき
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateSpray
/*!
 *	水しぶき
 *
 *	@note
 *		user_timer	: エフェクト生成間隔
 */
// ==========================================================================
void GmPlyEfctCreateSpray(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_SPRAY);
	efct_work->efct_com.obj_work.pos.y = g_gm_main_system.water_level << FX32_SHIFT;

	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	//efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSweatMain;

	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, 10.f, -5.f);
}

// ==========================================================================
// 泡
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateBubble
/*!
 *	泡
 *
 *	@note
 *		user_timer	: エフェクト生成間隔
 */
// ==========================================================================
void GmPlyEfctCreateBubble(GMS_PLAYER_WORK *ply_work)
{
#if !_IPHONE
	GMS_EFFECT_3DES_WORK	*efct_work;

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_BUBBLE);

	// 親クリア
	efct_work->efct_com.obj_work.parent_obj = NULL;

	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctBubbleMain;

	efct_work->efct_com.obj_work.move_flag &= ~OBD_MOVE_NOMOVE;

	efct_work->obj_3des.flag |= OBD_ACTFLAG_3D_ES_PARTICLE_DEPEND;

	// 初期速度設定
	//efct_work->efct_com.obj_work.spd.y = (mtMathRand() & 0x1F) << 8;

	// 位置調整
	GmEffect3DESAddDispOffset(efct_work, 0, 0.f, 10.f);
#endif // !_IPHONE
}

// ==========================================================================
// 水中死亡前カウンタ
// ==========================================================================
// ==========================================================================
// GmPlyEfctWaterCount
/*!
 *	水中死亡前カウンタ
 *
 *	@param	no	[in]	生成するNO 0 ～ 5
 *
 *	@note
 *		user_timer	: 生存時間
 */
// ==========================================================================
void GmPlyEfctWaterCount(GMS_PLAYER_WORK *ply_work, u32 no)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	MTM_ASSERT(ply_work);
	MTM_ASSERT(no <= 5);

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, (GME_EFCT_CMN_IDX)(GME_EFCT_CMN_IDX_COUNT0 + no));

	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctWaterCountMain;

	// 描画コマンドステート変更
	efct_work->obj_3des.command_state = OBD_DRAW_CMD_STATE_3DFIX;

	efct_work->efct_com.obj_work.user_timer = 2*60*FX32_ONE - FX32_ONE/* 心持ち早く消える */;

	// 位置調整
	GmComEfctAddDispOffset(efct_work, 0, -16*FX32_ONE, GMD_OBJ_DEFAULT_POS_Z_N_FRONT);
}

// ==========================================================================
// 水中死亡
// ==========================================================================
// ==========================================================================
// GmPlyEfctWaterDeath
/*!
 *	水中死亡
 */
// ==========================================================================
void GmPlyEfctWaterDeath(GMS_PLAYER_WORK *ply_work)
{
	//GMS_EFFECT_3DES_WORK	*efct_work;

	MTM_ASSERT(ply_work);

	GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_DEATH);

	// 位置調整
	//GmComEfctAddDispOffset(efct_work, 0, -16*FX32_ONE, 128*FX32_ONE);
}

// ==========================================================================
// 走り水しぶき
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateRunSpray
/*!
 *	走り水しぶき
 *
 *	@note
 *		user_work	0 : 低速タイプ   1 : 高速タイプ
 */
// ==========================================================================
void GmPlyEfctCreateRunSpray(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	MTM_ASSERT(ply_work);

	if (ply_work->efct_run_spray ||
			MTM_MATH_ABS(ply_work->obj_work.spd_m) < GMD_PLF_EFCT_RUN_SPRAY_MIN_SPD ||
			!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER)) {
		return;
	}

	if (MTM_MATH_ABS(ply_work->obj_work.spd_m) >= GMD_PLF_EFCT_RUN_SPRAY_BIG_SPD) {
		// 高速時
		efct_work = GmEfctZoneEsCreate(&ply_work->obj_work,
									GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_SPRAY_L_Z3);
		efct_work->efct_com.obj_work.user_work = 1;
	}
	else {
		// 低速時
		efct_work = GmEfctZoneEsCreate(&ply_work->obj_work,
									GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_SPRAY_S_Z3);
		efct_work->efct_com.obj_work.user_work = 0;
	}

	efct_work->efct_com.obj_work.flag &= ~OBD_OBJECT_PARENT_FIX;

	efct_work->efct_com.obj_work.pos.y = (s32)g_gm_main_system.water_level << FX32_SHIFT;
	//efct_work->efct_com.obj_work.pos.z += GMD_PLF_EFCT_RUN_SPRAY_OFST_Z * FX32_ONE;

	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctRunSprayMain;

	// 保存
	ply_work->efct_run_spray = (OBS_OBJECT_WORK*)efct_work;

	// 終了処理差し替え
	mtTaskChangeTcbDestructor(efct_work->efct_com.obj_work.tcb, gmPlyEfctRunSprayDest);
}

// ==========================================================================
// ホーミング
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateHomingImpact
/*!
 *	ホーミング衝撃波
 *
 *	@note
 *		user_timer	: エフェクト生成間隔
 */
// ==========================================================================
void GmPlyEfctCreateHomingImpact(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	float	dist_x, dist_y;
	Angle32	dir = 0;

	if (ply_work->enemy_obj) {

		dist_x = FXM_FX32_TO_FLOAT(ply_work->enemy_obj->pos.x - ply_work->obj_work.pos.x);
		dist_y = FXM_FX32_TO_FLOAT(ply_work->enemy_obj->pos.y - ply_work->obj_work.pos.y);

		dir = nnArcTan2(-dist_y, dist_x);
	}

	// その場発生
	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_H_ATTACK_00);
	// GmEffectDefaultMainFuncDeleteAtEnd

	GmComEfctAddDispRotation(efct_work, 0, 0, (u16)dir);

#if _IPHONE
	efct_work->obj_3des.command_state = OBD_DRAW_CMD_STATE_3DNN_POST; // 描画コマンドを事後へ
#endif // _IPHONE

	// その場発生ライン
	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_H_ATTACK_02);
	// GmEffectDefaultMainFuncDeleteAtEnd

	GmComEfctAddDispRotation(efct_work, 0, 0, (u16)dir);
	
#if _IPHONE
	efct_work->obj_3des.command_state = OBD_DRAW_CMD_STATE_3DNN_POST; // 描画コマンドを事後へ
#endif // _IPHONE

	// 本体付着
	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_H_ATTACK_01);
	//GmComEfctAddDispOffset(efct_work, 0, 0, 32*FX32_ONE);
	efct_work->efct_com.obj_work.parent_ofst.z = 32*FX32_ONE;

	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctHomingImpact01Main;
	
#if _IPHONE
	efct_work->obj_3des.command_state = OBD_DRAW_CMD_STATE_3DNN_POST; // 描画コマンドを事後へ
#endif // _IPHONE

//	efct_work->efct_com.obj_work.user_timer = (120 + (mtMathRand() & 0x3F)) << FX32_SHIFT;

	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, 10.f, -5.f);
}

// ==========================================================================
// GmPlyEfctCreateHomingCursol
/*!
 *	ホーミング カーソル
 *
 *	@note
 *		user_timer	: ofst_x \n
 *		user_work	: ofst_y
 */
// ==========================================================================
void GmPlyEfctCreateHomingCursol(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	GMS_ENEMY_COM_WORK		*target_ene;
	OBS_RECT_WORK			*rect_work;

	target_ene = (GMS_ENEMY_COM_WORK*)ply_work->enemy_obj;
	if (target_ene == NULL ||
			!(target_ene->obj_work.obj_type == GMD_OBJTYPE_ENEMY ||
				target_ene->obj_work.obj_type == GMD_OBJTYPE_GIMMICK)) {
		return;
	}

	efct_work = GmEfctCmnEsCreate((OBS_OBJECT_WORK*)target_ene, GME_EFCT_CMN_IDX_TARGET_S);
	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctHomingCursolMain;

	// オフセット設定
	rect_work = &target_ene->rect_work[GMD_ENEMY_RECT_BODY];
	efct_work->efct_com.obj_work.user_timer	= (s32)(((rect_work->rect.left + rect_work->rect.right)>>1) << FX32_SHIFT);
	efct_work->efct_com.obj_work.user_work	= (u32)(((rect_work->rect.top + rect_work->rect.bottom)>>1) << FX32_SHIFT);
	if (target_ene->obj_work.disp_flag & OBD_DISP_HFLIP) {
		efct_work->efct_com.obj_work.user_timer = -efct_work->efct_com.obj_work.user_timer;
	}
	if (target_ene->obj_work.disp_flag & OBD_DISP_VFLIP) {
		efct_work->efct_com.obj_work.user_work = (u32)-(s32)efct_work->efct_com.obj_work.user_work;
	}

	// 位置調整
	GmComEfctSetDispOffset(efct_work, 
		(fx32)efct_work->efct_com.obj_work.user_timer,
		(fx32)efct_work->efct_com.obj_work.user_work,
		128*FX32_ONE);

}

// ==========================================================================
// ジャンプダッシュ
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateJumpDash
/*!
 *	ジャンプダッシュエフェクト
 *
 *	@note
 *		移動速度設定後に呼び出し
 */
// ==========================================================================
void GmPlyEfctCreateJumpDash(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	Angle32	dir;

	dir = nnArcTan2(-ply_work->obj_work.spd.y, ply_work->obj_work.spd.x);

	// その場発生
	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_H_ATTACK_03);
	// GmEffectDefaultMainFuncDeleteAtEnd

	GmComEfctAddDispRotation(efct_work, 0, 0, (u16)dir);
}

// ==========================================================================
// スピン移行
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateSpinStartBlur
/*!
 *	スピン移行ブラー
 *
 *	@return	GMS_EFFECT_3DES_WORKワーク
 *
 *	@note
 *		user_timer	: スピンブラーフレーム
 *		user_work	: アクションタイプ
 */
// ==========================================================================
void* GmPlyEfctCreateSpinStartBlur(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	fx32					ofst_z;

	if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work,  GME_EFCT_CMN_IDX_SS_SPIN_START);

		// アクションタイプ保存
		efct_work->efct_com.obj_work.user_work = GME_EFCT_CMN_IDX_SS_SPIN_START;
	}
	else {
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_SPIN_START);

		// アクションタイプ保存
		efct_work->efct_com.obj_work.user_work = GME_EFCT_CMN_IDX_SPIN_START;
	}
	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSpinStartBlurMain;
	// 演出設定
	efct_work->efct_com.obj_work.user_timer = GMD_PLY_EFCT_SPIN_START_BLUR_FRAME * FX32_ONE;
	// 位置調整
	if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
		ofst_z = -GMD_PLY_EFCT_SPIN_START_BLUR_BASE_OFST_Z*FX32_ONE;
	}
	else {
		ofst_z = GMD_PLY_EFCT_SPIN_START_BLUR_BASE_OFST_Z*FX32_ONE;
	}
	if (  (ply_work->player_flag & GMD_PLF_PINBALL_SONIC) 
		||(GSM_MAIN_STAGE_IS_SPSTAGE()) ) {
		// スペステとピンボールはオフセット
		GmComEfctSetDispOffset(efct_work, 0,
				GMD_PLY_EFCT_SPIN_START_BLUR_BASE_OFST_Y_PINBALL*FX32_ONE,
				ofst_z);
	}
	else {
		GmComEfctSetDispOffset(efct_work, 0,
				GMD_PLY_EFCT_SPIN_START_BLUR_BASE_OFST_Y*FX32_ONE,
				ofst_z);
	}

	// 保存
	ply_work->efct_spin_start_blur = (OBS_OBJECT_WORK*)efct_work;

	// 終了処理差し替え
	mtTaskChangeTcbDestructor(efct_work->efct_com.obj_work.tcb, gmPlyEfctSpinStartBlurDest);

	return (efct_work);
}

// ==========================================================================
// スピンジャンプ
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateSpinJumpBlur
/*!
 *	スピンジャンプ ブラー
 *
 *	@return	GMS_EFFECT_3DES_WORKワーク
 *
 *	@note
 *		user_work	: アクションタイプ
 *		user_timer	: フレームチェック用カウンタ
 */
// ==========================================================================
void* GmPlyEfctCreateSpinJumpBlur(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (ply_work->efct_spin_jump_blur &&
			!(ply_work->efct_spin_jump_blur->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST))) {
		// 既に生成済み
		// 位置調整
		gmPlyEfctSpinJumpBlurPosAdj(ply_work);
		return (NULL);
	}

	if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work,  GME_EFCT_CMN_IDX_SS_SPIN);

		// アクションタイプ保存
		efct_work->efct_com.obj_work.user_work = GME_EFCT_CMN_IDX_SS_SPIN;
	}
	else {
		efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_SPIN);

		// アクションタイプ保存
		efct_work->efct_com.obj_work.user_work = GME_EFCT_CMN_IDX_SPIN;
	}
	// 保存
	ply_work->efct_spin_jump_blur = (OBS_OBJECT_WORK*)efct_work;

	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSpinJumpBlurMain;
	// 距離設定
	//efct_work->efct_com.obj_work.user_work	= (u32)((float)GMD_PLY_EFCT_SPIN_JUMP_BLUR_DIST_MAX);
	//efct_work->efct_com.obj_work.user_timer	= (s32)((float)GMD_PLY_EFCT_SPIN_JUMP_BLUR_DIST_MIN);
	// 位置調整
	gmPlyEfctSpinJumpBlurPosAdj(ply_work);

	// 終了処理差し替え
	mtTaskChangeTcbDestructor(efct_work->efct_com.obj_work.tcb, gmPlyEfctSpinJumpBlurDest);

	return (efct_work);

	
#if 0
	// 2
	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_SPIN);
	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSpinJumpBlurMain;
	// 距離設定
	efct_work->efct_com.obj_work.user_work	= (u32)((float)GMD_PLY_EFCT_SPIN_JUMP_BLUR_DIST_MAX*1.5);
	efct_work->efct_com.obj_work.user_timer	= (s32)((float)GMD_PLY_EFCT_SPIN_JUMP_BLUR_DIST_MIN*1.5);
	// 位置調整
	GmComEfctSetDispOffset(efct_work, 0, GMD_PLY_EFCT_SPIN_JUMP_BLUR_BASE_OFST_Y*FX32_ONE, 32*FX32_ONE);
#endif
}

// ==========================================================================
// スーパーソニックオーラ
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateSuperStart
/*!
 *	スーパーソニック開始
 */
// ==========================================================================
void GmPlyEfctCreateSuperStart(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_SS_START);

	// 位置調整
	GmComEfctSetDispOffset(efct_work, 0,
				-3*FX32_ONE,
				0);
}

// ==========================================================================
// GmPlyEfctCreateSuperEnd
/*!
 *	スーパーソニック終了
 */
// ==========================================================================
void GmPlyEfctCreateSuperEnd(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work,  GME_EFCT_CMN_IDX_SS_END);

	// トロッコの時は座標を調整する
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		efct_work->efct_com.obj_work.flag &= ~OBD_OBJECT_PARENT_FIX;
		efct_work->efct_com.obj_work.pos.x = FXM_FLOAT_TO_FX32(NNM_MTX(ply_work->truck_mtx_ply_mtn_pos, 0, 3));
		efct_work->efct_com.obj_work.pos.y = FXM_FLOAT_TO_FX32(-NNM_MTX(ply_work->truck_mtx_ply_mtn_pos, 1, 3));
		efct_work->efct_com.obj_work.pos.z = FXM_FLOAT_TO_FX32(NNM_MTX(ply_work->truck_mtx_ply_mtn_pos, 2, 3));
	}
}

// ==========================================================================
// GmPlyEfctCreateSuperAuraDeco
/*!
 *	スーパーソニックオーラ 装飾
 */
// ==========================================================================
void GmPlyEfctCreateSuperAuraDeco(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (!(ply_work->player_flag & GMD_PLF_SUPER_SONIC)) {
		// スーパーソニックでない時は生成しない
		return;
	}

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_AURA_00);

	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSuperAuraMain;
	// 位置調整
	//GmComEfctSetDispOffset(efct_work, 0, GMD_PLY_EFCT_SPIN_JUMP_BLUR_BASE_OFST_Y_PINBALL*FX32_ONE, 0);

	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// トロッコ中は座標をユーザー処理で設定
		efct_work->efct_com.obj_work.flag &= ~OBD_OBJECT_PARENT_FIX;
	}
}

// ==========================================================================
// GmPlyEfctCreateSuperAuraBase
/*!
 *	スーパーソニックオーラ ベース
 */
// ==========================================================================
void GmPlyEfctCreateSuperAuraBase(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (!(ply_work->player_flag & GMD_PLF_SUPER_SONIC)) {
		// スーパーソニック出ない時は生成しない
		return;
	}

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_AURA_01);

	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSuperAuraMain;
	// 位置調整
	//GmComEfctSetDispOffset(efct_work, 0, GMD_PLY_EFCT_SPIN_JUMP_BLUR_BASE_OFST_Y_PINBALL*FX32_ONE, 0);

	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// トロッコ中は座標をユーザー処理で設定
		efct_work->efct_com.obj_work.flag &= ~OBD_OBJECT_PARENT_FIX;
	}
}

// ==========================================================================
// GmPlyEfctCreateSuperAuraSpin
/*!
 *	スーパーソニックオーラ スピン
 */
// ==========================================================================
void GmPlyEfctCreateSuperAuraSpin(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (!(ply_work->player_flag & GMD_PLF_SUPER_SONIC)) {
		// スーパーソニック出ない時は生成しない
		return;
	}

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_AURA_02);

	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSuperAuraSpinMain;
	// 位置調整
	//GmComEfctSetDispOffset(efct_work, 0, GMD_PLY_EFCT_SPIN_JUMP_BLUR_BASE_OFST_Y_PINBALL*FX32_ONE, 0);

	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// トロッコ中は座標をユーザー処理で設定
		efct_work->efct_com.obj_work.flag &= ~OBD_OBJECT_PARENT_FIX;
	}
}

// ==========================================================================
// GmPlyEfctCreateSuperAuraDash
/*!
 *	スーパーソニックオーラ ダッシュ
 */
// ==========================================================================
void GmPlyEfctCreateSuperAuraDash(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	if (!(ply_work->player_flag & GMD_PLF_SUPER_SONIC)) {
		// スーパーソニック出ない時は生成しない
		return;
	}

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_AURA_03);

	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSuperAuraDashMain;
	// 位置調整
	//GmComEfctSetDispOffset(efct_work, 0, GMD_PLY_EFCT_SPIN_JUMP_BLUR_BASE_OFST_Y_PINBALL*FX32_ONE, 0);

	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// トロッコ中は座標をユーザー処理で設定
		efct_work->efct_com.obj_work.flag &= ~OBD_OBJECT_PARENT_FIX;
	}
}


// ==========================================================================
// スチームパイプエフェクト
// ==========================================================================
// ==========================================================================
// GmPlyEfctCreateSteamPipe
/*!
 *	プレイヤー スチームパイプ導入時エフェクト
 */
// ==========================================================================
void GmPlyEfctCreateSteamPipe(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK *efct_work;
	GME_EFCT_CMN_IDX	efct_idx = GME_EFCT_CMN_IDX_STEAM_SET;	// 通常ソニック用
	if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
		efct_idx = GME_EFCT_CMN_IDX_STEAM_SET_SS;				// スーパーソニック用
	}
	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, efct_idx);
	GmComEfctSetDispOffset(efct_work, 0, 0, 10*FX32_ONE);

	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSteamPipeMain;
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// 軌跡エフェクト
// ==========================================================================
// ==========================================================================
// gmPlyEfctTrailSysMain
/*!
 *	プレイヤー 軌跡エフェクトシステムメイン処理
 */
// ==========================================================================
void gmPlyEfctTrailSysMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	OBS_DATA_WORK	*obj_work;
	NNS_TEXLIST		*texlist;

	if (!ObjObjectPauseCheck(0)) {
		// 軌跡エフェクト更新
		amTrailEFUpdate( AMTRE_HANDLE_ACCELL );
	}
	if (g_obj.glb_camera_id != -1 && ObjCameraGet(g_obj.glb_camera_id)) {
		// メインカメラで軌跡エフェクトカメラを設定
		NNS_VECTOR	sort_cam_pos;
		NNS_VECTOR	cam_ofst;
		NNS_MATRIX	obj_mtx;
		NNS_RGBA	diffuse = { 1.0f, 1.0f, 1.0f, 1.0f};
		NNS_RGB		ambient = { 1.0f, 1.0f, 1.0f};

		nnMakeUnitMatrix(&obj_mtx);	// 変更必要？

		// 3DNNのカメラ設定
		ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, g_obj.glb_camera_type, OBD_DRAW_CMD_STATE_3DNN);
		// カメラ座標を取得
		ObjCameraDispPosGet(g_obj.glb_camera_id, &sort_cam_pos);
		// カメラとエフェクトとの本来の位置関係になるように
		// ソート用カメラの位置を調整する
		amVectorSet(&cam_ofst,
					-NNM_MTX(obj_mtx, 0, 3),
					-NNM_MTX(obj_mtx, 1, 3),
					-NNM_MTX(obj_mtx, 2, 3));
		nnAddVector(&sort_cam_pos, &cam_ofst, &sort_cam_pos);
		
		// ソート用カメラ座標設定
		amEffectSetCameraPos(&sort_cam_pos);


		// 軌跡描画
		nnSetPrimitive3DMaterial(&diffuse, &ambient, 1.0f);
		obj_work = ObjDataGet(GMD_DWORK_NO_EFFECT_CMN_TEXLIST);
		texlist = (NNS_TEXLIST*)obj_work->pData;
		amTrailEFDraw( AMTRE_HANDLE_ACCELL, texlist, OBD_DRAW_CMD_STATE_3DNN);
		//amTrailEFDraw( AMTRE_HANDLE_ACCELL, texlist, OBD_DRAW_CMD_STATE_POST_WATER);
	}
}

// ==========================================================================
// バリアエフェクト
// ==========================================================================
// ==========================================================================
// gmPlyEfctBarrierMain
/*!
 *	プレイヤー バリアエフェクト メイン処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_work : ID保存
 */
// ==========================================================================
void gmPlyEfctBarrierMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (!(ply_work->player_flag & GMD_PLF_BARRIER)) {
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;	// 両方エミッタ使用でないパーティクルがある模様
		//if (obj_work->user_work == GME_EFCT_CMN_IDX_BARRIER) {
		//	// 即時破棄
		//	obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		//}
		//else {
		//	obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		//	// 終了
		//	//ObjDrawKillAction3DES(obj_work);
		//}

		if (obj_work->user_work == GME_EFCT_CMN_IDX_BARRIER) {
			// 終了エフェクト生成
			gmPlyEfctCreateBarrierLost(ply_work);
		}

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEnd;
	}

	// 表示OFFチェック
	if (ply_work->gmk_flag2 & GMD_PLGF2_BARRIER_DISP_OFF) {
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
	else {
		obj_work->disp_flag &= ~OBD_DISP_NODISP;
	}
}

// ==========================================================================
// gmPlyEfctCreateBarrierLost
/*!
 *	プレイヤー バリア終了エフェクト
 */
// ==========================================================================
void gmPlyEfctCreateBarrierLost(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	MTM_ASSERT(ply_work);

	// 表示OFFチェック
	if (ply_work->gmk_flag2 & GMD_PLGF2_BARRIER_DISP_OFF) {
		// はじめから表示OFFの時は生成しない
		return;
	}

	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_BARRIER_LOST);

	// 表示位置調整
	GmComEfctAddDispOffset(efct_work, 0,
									0,
									GMD_PLY_EFCT_BARRIER_ADD_OFST_Z*FX32_ONE);
}


// ==========================================================================
// 無敵エフェクト
// ==========================================================================
// ==========================================================================
// gmPlyEfctInvincibleMgrMain
/*!
 *	プレイヤー 無敵エフェクト 管理メイン処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_work	: GME_EFCT_CMN_IDX_MUTEKI  生成カウンタ
 *		user_timer	: GME_EFCT_CMN_IDX_MUTEKI2 生成カウンタ
 */
// ==========================================================================
void gmPlyEfctInvincibleMgrMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;
	if (!ply_work->genocide_timer) {
		// 終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		return;
	}

	// エフェクト生成チェック
	obj_work->user_timer++;
	if (obj_work->user_timer >= 15) {
		obj_work->user_timer = 0;

		// GME_EFCT_CMN_IDX_MUTEKI2生成
		gmPlyEfctCreateInvincibleTail((GMS_PLAYER_WORK*)obj_work->parent_obj);
	}

	obj_work->user_work++;
	if (obj_work->user_work >= 70) {
		obj_work->user_work = 0;

		// GME_EFCT_CMN_IDX_MUTEKI生成
		gmPlyEfctCreateInvincibleCircle((GMS_PLAYER_WORK*)obj_work->parent_obj);
	}
}

// ==========================================================================
// gmPlyEfctCreateInvincibleCircle
/*!
 *	プレイヤー 無敵エフェクト Circle
 *
 *	@param	ply_work	[in]	親オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctCreateInvincibleCircle(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	// GME_EFCT_CMN_IDX_MUTEKI2生成
	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_MUTEKI);

	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctInvincibleCircleMain;
	// 位置調整
	GmComEfctAddDispOffsetF(efct_work, 0.f, 0.f, GMD_PLY_EFCT_OFST_FRONT_PLAYER);

	// 表示OFFチェック
	if (ply_work) {
		if (ply_work->gmk_flag2 & GMD_PLGF2_INVINCIBLE_DISP_OFF) {
			efct_work->efct_com.obj_work.disp_flag |= OBD_DISP_NODISP;
		}
	}
}

// ==========================================================================
// gmPlyEfctInvincibleCircleMain
/*!
 *	プレイヤー 無敵エフェクト メイン処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctInvincibleCircleMain(OBS_OBJECT_WORK *obj_work)
{
	// 更新
	GmEfctCmnUpdateInvincibleMainPart((GMS_EFFECT_3DES_WORK*)obj_work);

	// 共通処理
	gmPlyEfctInvincibleMain(obj_work);
}

// ==========================================================================
// gmPlyEfctCreateInvincibleTail
/*!
 *	プレイヤー 無敵エフェクト Tail
 *
 *	@param	ply_work	[in]	親オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctCreateInvincibleTail(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	// GME_EFCT_CMN_IDX_MUTEKI2生成
	efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_MUTEKI2);

	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmPlyEfctInvincibleTailMain;
	// 位置調整
	GmComEfctAddDispOffsetF(efct_work, 0.f, 0.f, GMD_PLY_EFCT_OFST_FRONT_PLAYER);

	// 表示OFFチェック
	if (ply_work) {
		if (ply_work->gmk_flag2 & GMD_PLGF2_INVINCIBLE_DISP_OFF) {
			efct_work->efct_com.obj_work.disp_flag |= OBD_DISP_NODISP;
		}
	}
}

// ==========================================================================
// gmPlyEfctInvincibleTailMain
/*!
 *	プレイヤー 無敵エフェクト メイン処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctInvincibleTailMain(OBS_OBJECT_WORK *obj_work)
{
	// 更新
	GmEfctCmnUpdateInvincibleSubPart((GMS_EFFECT_3DES_WORK*)obj_work, obj_work->parent_obj);
	
	// 共通処理
	gmPlyEfctInvincibleMain(obj_work);
}

// ==========================================================================
// gmPlyEfctInvincibleMain
/*!
 *	プレイヤー 無敵エフェクト メイン処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctInvincibleMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (!ply_work->genocide_timer) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}

	// 表示OFFチェック
	if (ply_work->gmk_flag2 & GMD_PLGF2_INVINCIBLE_DISP_OFF) {
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
	else {
		obj_work->disp_flag &= ~OBD_DISP_NODISP;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEnd(obj_work);
}

// ==========================================================================
// 最速ダッシュ 回転足
// ==========================================================================
// ==========================================================================
// gmPlyEfctRollDashMain
/*!
 *	最速ダッシュ 回転足
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctRollDashMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (ply_work->act_state != GME_PLY_ACT_STATE_DASH_2) {
		// 終了
		//ObjDrawKillAction3DES(obj_work);

		// 即時終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;

		return;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
}

// ==========================================================================
// 汗
// ==========================================================================
// ==========================================================================
// gmPlyEfctSweatMain
/*!
 *	汗
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctSweatMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (!(GME_PLY_SEQ_STATE_STAGGER_F <= ply_work->seq_state &&
				ply_work->seq_state <= GME_PLY_SEQ_STATE_STAGGER_D)) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEnd;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEnd(obj_work);
}

// ==========================================================================
// 砂煙
// ==========================================================================
// ==========================================================================
// gmPlyEfctSweatMain
/*!
 *	走り 砂煙
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctRunDustMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (ply_work->act_state != GME_PLY_ACT_STATE_RUN) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
}

// ==========================================================================
// gmPlyEfctDash1DustMain
/*!
 *	ダッシュ1 砂煙
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctDash1DustMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (ply_work->act_state != GME_PLY_ACT_STATE_DASH_1) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
}

// ==========================================================================
// gmPlyEfctDash2DustMain
/*!
 *	ダッシュ2 砂煙
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctDash2DustMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (ply_work->act_state != GME_PLY_ACT_STATE_DASH_2) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
}

// ==========================================================================
// gmPlyEfctDash2ImpactMain
/*!
 *	ダッシュ2 衝撃波
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctDash2ImpactMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (ply_work->act_state != GME_PLY_ACT_STATE_DASH_2) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
}

// ==========================================================================
// gmPlyEfctSpinDustMain
/*!
 *	その場スピン 砂煙
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctSpinDustMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (ply_work->seq_state != GME_PLY_SEQ_STATE_SPIN_DASH) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
}

// ==========================================================================
// gmPlyEfctSpinAddDustMain
/*!
 *	スピン加速 砂煙
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctSpinAddDustMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (ply_work->seq_state != GME_PLY_SEQ_STATE_SPIN_DASHACC) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
}

// ==========================================================================
// gmPlyEfctSpinDashImpactMain
/*!
 *	スピンダッシュ 衝撃波
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctSpinDashImpactMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;
	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;
#if 1
	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);
	if (!obj_work->user_timer) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}
	else if (ply_work->seq_state != GME_PLY_SEQ_STATE_SPIN) {
		// 即時終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}
#else
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (ply_work->seq_state != GME_PLY_SEQ_STATE_SPIN_DASHACC) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}
#endif

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
}

// ==========================================================================
// gmPlyEfctSpinDashDustMain
/*!
 *	スピンダッシュ 砂煙
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctSpinDashDustMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (ply_work->seq_state != GME_PLY_SEQ_STATE_SPIN) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
}

// ==========================================================================
// gmPlyEfctSpinDashCircleBlurMain
/*!
 *	スピンダッシュ 円ブラー
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_timer	: アクションタイプ
 */
// ==========================================================================
void gmPlyEfctSpinDashCircleBlurMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK			*ply_work;
	GMS_EFFECT_3DES_WORK	*efct_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	efct_work = (GMS_EFFECT_3DES_WORK*)obj_work;
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {
		// [スペステ]プレイヤーの表示オフセットに合わせる
		obj_work->ofst = ply_work->obj_work.ofst;
	}

	// 速度取得
	obj_work->obj_3des->speed = ply_work->obj_work.obj_3d->speed[0];

	// プレイヤーダメージ時点滅
	obj_work->disp_flag &= ~OBD_DISP_NODISP;
	obj_work->disp_flag |= ply_work->obj_work.disp_flag & OBD_DISP_NODISP;

	// 終了確認
	if ( !(ply_work->seq_state == GME_PLY_SEQ_STATE_SPIN ||
			ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_DASHPANEL ||
			ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_SEESAW ||
			ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_SPIPE) &&
		!GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY() ) {

		// 終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;	// エミッタ使用でないパーティクル
		//ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);

	if (!(obj_work->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST)) &&
			efct_work->efct_com.obj_work.user_timer == GME_EFCT_CMN_IDX_SS_SPIN_D &&
			!(ply_work->player_flag & GMD_PLF_SUPER_SONIC)) {
		// スーパーソニックタイプエフェクトでスーパーソニックでなくなったとき
		GMS_EFFECT_3DES_WORK	*efct_work_new;
		float					save_speed;

		// 自分を終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;	// エミッタ使用でないパーティクル

		// エフェクト再生成
		efct_work_new = (GMS_EFFECT_3DES_WORK*)GmPlyEfctCreateSpinDashCircleBlur(ply_work);

		if (efct_work_new) {
			// 位置をあわせる
			save_speed = amEffectGetUnitFrame();
			amEffectSetUnitTime(ply_work->obj_work.obj_3d->frame[0], 60);
			amEffectUpdate(efct_work_new->obj_3des.ecb);
			amEffectSetUnitTime(save_speed, 60);

			// 速度取得
			efct_work_new->obj_3des.speed = ply_work->obj_work.obj_3d->speed[0];
		}
	}
}

// ==========================================================================
// gmPlyEfctSpinDashBlurMain
/*!
 *	スピンダッシュ ブラー
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_timer	: アクションタイプ
 */
// ==========================================================================
void gmPlyEfctSpinDashBlurMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK			*ply_work;
	GMS_EFFECT_3DES_WORK	*efct_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	efct_work = (GMS_EFFECT_3DES_WORK*)obj_work;
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {
		// [スペステ]プレイヤーの表示オフセットに合わせる
		obj_work->ofst = ply_work->obj_work.ofst;
	}

	// 速度取得
	obj_work->obj_3des->speed = ply_work->obj_work.obj_3d->speed[0];

	// 終了確認
#if 1
	if ( !(ply_work->seq_state == GME_PLY_SEQ_STATE_SPIN ||
			ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_DASHPANEL ||
			ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_SPIPE ||
			ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_SEESAW ||
			ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_FORCESPIN ||
			ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_FORCESPIN_DEC ||
			ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_FORCESPIN_FALL ||
			ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_SPRINGCTPLT_HOLD ||
			ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_STEAMPIPE) &&
		!GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY() ) {
#else
	if ( ((ply_work->player_flag & GMD_PLF_PINBALL_SONIC) &&
			!(ply_work->seq_state == GME_PLY_SEQ_STATE_SPIN ||
			  ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_DASHPANEL)) ||
		(!(ply_work->player_flag & GMD_PLF_PINBALL_SONIC) &&
			!(ply_work->seq_state == GME_PLY_SEQ_STATE_SPIN ||
			  ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_DASHPANEL)) &&
		(!GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) ) {
#endif

		// 終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;	// エミッタ使用でないパーティクル
		//ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
	
	if (!(obj_work->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST)) &&
			(efct_work->efct_com.obj_work.user_timer == GME_EFCT_CMN_IDX_SS_SPIN_D_B ||
				efct_work->efct_com.obj_work.user_timer == GME_EFCT_CMN_IDX_SS_SPIN) &&
			!(ply_work->player_flag & GMD_PLF_SUPER_SONIC)) {
		// スーパーソニックタイプエフェクトでスーパーソニックでなくなったとき
		GMS_EFFECT_3DES_WORK	*efct_work_new;
		float					save_speed;

		// 自分を終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;	// エミッタ使用でないパーティクル

		// エフェクト再生成
		efct_work_new = (GMS_EFFECT_3DES_WORK*)GmPlyEfctCreateSpinDashBlur(ply_work,
							efct_work->efct_com.obj_work.user_timer == GME_EFCT_CMN_IDX_SS_SPIN ? 0 : 1);

		if (efct_work_new) {
			// 位置をあわせる
			save_speed = amEffectGetUnitFrame();
			amEffectSetUnitTime(ply_work->obj_work.obj_3d->frame[0], 60);
			amEffectUpdate(efct_work_new->obj_3des.ecb);
			amEffectSetUnitTime(save_speed, 60);

			// 速度取得
			efct_work_new->obj_3des.speed = ply_work->obj_work.obj_3d->speed[0];
		}
	}
}

// ==========================================================================
// gmPlyEfctSpinDashBlurDest
/*!
 *	スピンダッシュ ブラー デストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmPlyEfctSpinDashBlurDest(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_PLAYER_WORK	*ply_work;

	if (obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER) {
		ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;
	}
	else {
		ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	}

	if (ply_work->efct_spin_dash_blur == obj_work) {
		// 管理からはずす
		ply_work->efct_spin_dash_blur = NULL;
	}
	if (ply_work->efct_spin_dash_cir_blur == obj_work) {
		// 管理からはずす
		ply_work->efct_spin_dash_cir_blur = NULL;
	}

	// 汎用終了処理
	ObjObjectExit(tcb);
}

// ==========================================================================
// gmPlyEfctBrakeDustMain
/*!
 *	ブレーキ 砂煙
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctBrakeDustMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (ply_work->seq_state != GME_PLY_SEQ_STATE_BRAKE) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
}

// ==========================================================================
// gmPlyEfctJumpDustMain
/*!
 *	ジャンプ 砂煙
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctJumpDustMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (ply_work->seq_state != GME_PLY_SEQ_STATE_JUMP) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
}

// ==========================================================================
// 泡
// ==========================================================================
// ==========================================================================
// gmPlyEfctBubbleMain
/*!
 *	泡
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_timer	: 演出タイマー
 */
// ==========================================================================
void gmPlyEfctBubbleMain(OBS_OBJECT_WORK *obj_work)
{
	BOOL	b_delete = TRUE;

	if (GmMainIsWaterLevel()) {
		// 水面チェック
		if ((obj_work->pos.y >> FX32_SHIFT) > g_gm_main_system.water_level + GMD_PLY_EFCT_BUBBLE_SURFACE_ADJUST/FX32_ONE) {
			// まだ水中
			b_delete = FALSE;

			// 速度設定
			obj_work->spd.y += GMD_PLY_EFCT_BUBBLE_SPD_Y_ACC;
			if (obj_work->spd.y < GMD_PLY_EFCT_BUBBLE_SPD_Y_MAX) {
				obj_work->spd.y = GMD_PLY_EFCT_BUBBLE_SPD_Y_MAX;
			}
			if ((obj_work->pos.y + obj_work->spd.y) <
					(((s32)g_gm_main_system.water_level << FX32_SHIFT) + GMD_PLY_EFCT_BUBBLE_SURFACE_ADJUST)) {
				obj_work->spd.y =
						((s32)g_gm_main_system.water_level << FX32_SHIFT) + GMD_PLY_EFCT_BUBBLE_SURFACE_ADJUST - obj_work->pos.y;
			}

			obj_work->user_timer = ObjTimeCountUp(obj_work->user_timer);

			if ((obj_work->user_timer >> FX32_SHIFT) & 0x03) {
				obj_work->spd.x = (mtMathRand() & 0xFFF) - 0x800;
			}
		}
	}

	if (b_delete) {
		// 即時終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		return;
		//ObjDrawKillAction3DES(obj_work);
		//
		//// 終了処理に任せる
		//obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEnd;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEnd(obj_work);
}

// ==========================================================================
// 水中死亡前カウンタ
// ==========================================================================
// ==========================================================================
// gmPlyEfctWaterCountMain
/*!
 *	水中死亡前カウンタ
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_timer	: 生存時間
 */
// ==========================================================================
void gmPlyEfctWaterCountMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);


	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);

	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;
	if (!obj_work->user_timer ||
			(ply_work->player_flag & GMD_PLF_DIE) ||
			(GmMainIsWaterLevel() && ((ply_work->obj_work.pos.y >> FX32_SHIFT) <= g_gm_main_system.water_level)) ||
			!GmMainIsWaterLevel() ||
			(ply_work->time_air - ply_work->water_timer > (10*60)*FX32_ONE+GMD_PLAYER_WATER_COUNT_TIME_OFST/*息継ぎした*/)) {
		// 終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;	// エミッタ使用でないパーティクル
		return;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEnd(obj_work);
}

// ==========================================================================
// 走り水しぶき
// ==========================================================================
// ==========================================================================
// gmPlyEfctRunSprayMain
/*!
 *	走り水しぶき
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_work	0 : 低速タイプ   1 : 高速タイプ
 */
// ==========================================================================
void gmPlyEfctRunSprayMain(OBS_OBJECT_WORK *obj_work)
{
	OBS_OBJECT_WORK	*ply_obj;
	BOOL			b_end;
	BOOL			b_ply_end;

	ply_obj = obj_work->parent_obj;

	if (ply_obj) {
		// 終了チェック
		b_end = FALSE;
		b_ply_end = FALSE;
		// 速度による終了
		if (obj_work->user_work == 0) {
			if (MTM_MATH_ABS(ply_obj->spd_m) < GMD_PLF_EFCT_RUN_SPRAY_MIN_SPD) {
				// 最低速度より小
				b_end = TRUE;
			}
			else if (MTM_MATH_ABS(ply_obj->spd_m) >= GMD_PLF_EFCT_RUN_SPRAY_BIG_SPD) {
				// 高速域
				b_end = TRUE;
			}
		}
		else {//if (obj_work->user_work == 1) {
			if (MTM_MATH_ABS(ply_obj->spd_m) < GMD_PLF_EFCT_RUN_SPRAY_BIG_SPD) {
				b_end = TRUE;
			}
		}
		// プレイヤーによる終了
		if (!(ply_obj->move_flag & OBD_MOVE_UNDER) ||
				((ply_obj->pos.y >> FX32_SHIFT)-GMD_PLAYER_WATER_FACE_UP_OFST >=
						g_gm_main_system.water_level)) {
			b_ply_end = TRUE;
		}

		if (b_end | b_ply_end) {
			// 終了
			ObjDrawKillAction3DES(obj_work);

			// 終了処理に任せる
			obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEnd;

			// 次のエフェクト生成チェック
			if (!b_ply_end && MTM_MATH_ABS(ply_obj->spd_m) >= GMD_PLF_EFCT_RUN_SPRAY_MIN_SPD) {
				((GMS_PLAYER_WORK*)ply_obj)->efct_run_spray = NULL;
				GmPlyEfctCreateRunSpray((GMS_PLAYER_WORK*)ply_obj);
			}
		}

		// 座標設定
		obj_work->pos.x = ply_obj->pos.x;
		obj_work->pos.y = (s32)g_gm_main_system.water_level << FX32_SHIFT;

		// 向きコピー
		obj_work->disp_flag &= ~(OBD_DISP_HFLIP | OBD_DISP_VFLIP);
		obj_work->disp_flag |= ply_obj->disp_flag & (OBD_DISP_HFLIP | OBD_DISP_VFLIP);
	}

	// 汎用処理を行わない
	//GmEffectDefaultMainFuncDeleteAtEnd(obj_work);
}

// ==========================================================================
// gmPlyEfctRunSprayDest
/*!
 *	走り水しぶき デストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmPlyEfctRunSprayDest(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_PLAYER_WORK	*ply_work;

	if (obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER) {
		ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;
	}
	else {
		ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	}

	if (ply_work->efct_run_spray == obj_work) {
		// 管理からはずす
		ply_work->efct_run_spray = NULL;
	}

	// 汎用終了処理
	ObjObjectExit(tcb);
}

// ==========================================================================
// ホーミング
// ==========================================================================
// ==========================================================================
// gmPlyEfctHomingImpact01Main
/*!
 *	ホーミング衝撃波
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctHomingImpact01Main(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	// 終了確認
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (ply_work->seq_state != GME_PLY_SEQ_STATE_HOMING) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEnd;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEnd(obj_work);
}

// ==========================================================================
// gmPlyEfctHomingCursolMain
/*!
 *	ホーミング カーソル
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_timer	: ofst_x \n
 *		user_work	: ofst_y
 */
// ==========================================================================
void gmPlyEfctHomingCursolMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	MTM_ASSERT(ply_work);

	// 終了確認
	if (ply_work->enemy_obj != obj_work->parent_obj ||
			GmPlySeqCheckAcceptHoming(ply_work) == FALSE) {
		GMS_EFFECT_3DES_WORK	*efct_work;

		// 終了用エフェクト生成
		efct_work = GmEfctCmnEsCreate(obj_work->parent_obj, GME_EFCT_CMN_IDX_TARGET_E);
		if (obj_work->parent_obj->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST)) {
			// 親死亡時はクリアしておく
			efct_work->efct_com.obj_work.parent_obj = NULL;	// 親クリア
		}
		// 位置調整
		GmComEfctSetDispOffset(efct_work, 
			(fx32)obj_work->user_timer, (fx32)obj_work->user_work, 32*FX32_ONE);

		// 終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;	// エミッタ使用でないパーティクル

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEnd;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEnd(obj_work);
}


// ==========================================================================
// スピン移行
// ==========================================================================
// ==========================================================================
// gmPlyEfctSpinStartBlurMain
/*!
 *	スピン移行ブラー
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_timer	: スピンブラーフレーム
 *		user_work	: アクションタイプ
 */
// ==========================================================================
void gmPlyEfctSpinStartBlurMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	GMS_PLAYER_WORK			*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	efct_work = (GMS_EFFECT_3DES_WORK*)obj_work;
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);

	// アニメーション速度UP
	efct_work->obj_3des.speed += 0.05;

	// 半透明設定
	if (efct_work->obj_3des.ecb) {
		efct_work->obj_3des.ecb->transparency =
			(FX_Div(obj_work->user_timer, GMD_PLY_EFCT_SPIN_START_BLUR_FRAME*FX32_ONE) * 256) >> FX32_SHIFT;
		if (efct_work->obj_3des.ecb->transparency > 256) {
			efct_work->obj_3des.ecb->transparency = 256;
		}
	}

	if (!obj_work->user_timer ||
			!(ply_work->act_state == GME_PLY_ACT_STATE_SPIN_SHIFT)) {
		// 終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;	// エミッタ使用でないパーティクル
		//ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;
	}

	if (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {
		// [スペステ]プレイヤーの表示オフセットに合わせる
		obj_work->ofst = ply_work->obj_work.ofst;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
	
	if (!(obj_work->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST)) &&
			efct_work->efct_com.obj_work.user_work == GME_EFCT_CMN_IDX_SS_SPIN_START &&
			!(ply_work->player_flag & GMD_PLF_SUPER_SONIC)) {
		// スーパーソニックタイプエフェクトでスーパーソニックでなくなったとき
		GMS_EFFECT_3DES_WORK	*efct_work_new;
		float					save_speed;

		// 自分を終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;	// エミッタ使用でないパーティクル

		// エフェクト再生成
		efct_work_new = (GMS_EFFECT_3DES_WORK*)GmPlyEfctCreateSpinStartBlur(ply_work);

		if (efct_work_new) {
			// 位置をあわせる
			save_speed = amEffectGetUnitFrame();
			amEffectSetUnitTime(ply_work->obj_work.obj_3d->frame[0], 60);
			amEffectUpdate(efct_work_new->obj_3des.ecb);
			amEffectSetUnitTime(save_speed, 60);

			// 速度取得
			efct_work_new->obj_3des.speed = efct_work->obj_3des.speed;

			// その他ステータス移行
			efct_work_new->efct_com.obj_work.user_timer = obj_work->user_timer;
		}
	}
}

// ==========================================================================
// gmPlyEfctSpinStartBlurDest
/*!
 *	スピン移行ブラー デストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmPlyEfctSpinStartBlurDest(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_PLAYER_WORK	*ply_work;

	if (obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER) {
		ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;
	}
	else {
		ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	}

	if (ply_work->efct_spin_start_blur == obj_work) {
		// 管理からはずす
		ply_work->efct_spin_start_blur = NULL;
	}

	// 汎用終了処理
	ObjObjectExit(tcb);
}


// ==========================================================================
// スピンジャンプ
// ==========================================================================
// ==========================================================================
// gmPlyEfctSpinJumpBlurMain
/*!
 *	スピンジャンプ ブラー
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_work	: アクションタイプ
 *		user_timer	: フレームチェック用カウンタ
 */
// ==========================================================================
void gmPlyEfctSpinJumpBlurMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;
	GMS_EFFECT_3DES_WORK	*efct_work;
#if 0
	fx32			move;
	float			move_f;
	Angle32			dir;
	float			draw_dist;
	float			dist_min, dist_max;
#endif

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	efct_work = (GMS_EFFECT_3DES_WORK*)obj_work;
	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {
		// [スペステ]プレイヤーの表示オフセットに合わせる
		obj_work->ofst = ply_work->obj_work.ofst;
	}

	// 終了確認
	if ( ((ply_work->player_flag & GMD_PLF_PINBALL_SONIC) &&					// ピンボール中
		!(ply_work->seq_state == GME_PLY_SEQ_STATE_FW ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_WALK ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_JUMP ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_FALL ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_JUMPDASH ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_HOMING ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_SPIN_FALL ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_PINBALL ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_PINBALL_AIR ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_SPRINGCTPLT_UP ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_SPRINGCTPLT_LR ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_FLIPPER)) ||
		(!(ply_work->player_flag & GMD_PLF_PINBALL_SONIC) &&					// ピンボール以外
			!((ply_work->seq_state == GME_PLY_SEQ_STATE_JUMP ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_SPIN_FALL ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_PINBALL ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_PINBALL_AIR ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_SPRINGCTPLT_UP ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_SPRINGCTPLT_LR ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_CANNON_SHOOT ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_FLIPPER) &&
					(ply_work->act_state == GME_PLY_ACT_STATE_JUMP_SPIN ||
						ply_work->act_state == GME_PLY_ACT_STATE_SPIN ||
						ply_work->act_state == GME_PLY_ACT_STATE_GMK_CANNON_SHOOT ||
						ply_work->act_state == GME_PLY_ACT_STATE_SPIN_SMALL))) &&
		(!GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) ) {

		// 終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;	// エミッタ使用でないパーティクル
		//ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;

		// 汎用処理
		GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
		return;
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);

	// 重力方向回転をあわせる
	if (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {
		obj_work->dir.z = (u16)(obj_work->dir.z + ply_work->obj_work.dir_fall);
	}

	if (!(obj_work->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST)) &&
			efct_work->efct_com.obj_work.user_work == GME_EFCT_CMN_IDX_SS_SPIN &&
			!(ply_work->player_flag & GMD_PLF_SUPER_SONIC)) {
		// スーパーソニックタイプエフェクトでスーパーソニックでなくなったとき
		GMS_EFFECT_3DES_WORK	*efct_work_new;
		float					save_speed;

		// 自分を終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;	// エミッタ使用でないパーティクル

		// エフェクト再生成
		efct_work_new = (GMS_EFFECT_3DES_WORK*)GmPlyEfctCreateSpinJumpBlur(ply_work);

		if (efct_work_new) {
			// 位置をあわせる
			save_speed = amEffectGetUnitFrame();
			amEffectSetUnitTime(ply_work->obj_work.obj_3d->frame[0], 60);
			amEffectUpdate(efct_work_new->obj_3des.ecb);
			amEffectSetUnitTime(save_speed, 60);

			// 速度取得
			efct_work_new->obj_3des.speed = ply_work->obj_work.obj_3d->speed[0];
		}
	}
	else {
		fx32	ply_frame = FXM_FLOAT_TO_FX32(ply_work->obj_work.obj_3d->frame[0]);
		float	save_speed;
		if ((obj_work->user_timer & 0xFFFFF000) != (ply_frame & 0xFFFFF000)) {
			// 何らかの要因でフレームがずれたので修正する
			s32	adjust_frame = (ply_frame & 0xFFFFF000) - (obj_work->user_timer & 0xFFFFF000);

			adjust_frame %= GMD_PLY_EFCT_SPIN_JUMP_BLUR_PLY_MTN_FRAME*FX32_ONE;
			if (adjust_frame < 0) {
				adjust_frame += GMD_PLY_EFCT_SPIN_JUMP_BLUR_PLY_MTN_FRAME*FX32_ONE;
				adjust_frame %= GMD_PLY_EFCT_SPIN_JUMP_BLUR_PLY_MTN_FRAME*FX32_ONE;
			}
			// フレーム修正
			save_speed = amEffectGetUnitFrame();
			amEffectSetUnitTime(FXM_FX32_TO_FLOAT(adjust_frame), 60);
			efct_work->obj_3des.ecb->reserved[0] = 0;	// 強制更新クリア
			amEffectUpdate(efct_work->obj_3des.ecb);
			amEffectSetUnitTime(save_speed, 60);

			obj_work->user_timer = ply_frame;
		}
	}

	obj_work->user_timer = ObjTimeCountUp(obj_work->user_timer);
	if (obj_work->user_timer >= GMD_PLY_EFCT_SPIN_JUMP_BLUR_PLY_MTN_FRAME * FX32_ONE) {
		obj_work->user_timer -= GMD_PLY_EFCT_SPIN_JUMP_BLUR_PLY_MTN_FRAME * FX32_ONE;
	}


}

// ==========================================================================
// gmPlyEfctSpinJumpBlurPosAdj
/*!
 *	スピンジャンプ ブラー位置調整
 *
 */
// ==========================================================================
void gmPlyEfctSpinJumpBlurPosAdj(GMS_PLAYER_WORK *ply_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work = (GMS_EFFECT_3DES_WORK*)ply_work->efct_spin_jump_blur;

	if (  (ply_work->player_flag & GMD_PLF_PINBALL_SONIC) 
		||(ply_work->act_state == GME_PLY_ACT_STATE_SPIN)
		||(GSM_MAIN_STAGE_IS_SPSTAGE()) ) {
		// スペステ、ピンボール、地上用スピンactはそれ用のオフセット
		GmComEfctSetDispOffset(efct_work, 0, GMD_PLY_EFCT_SPIN_JUMP_BLUR_BASE_OFST_Y_PINBALL*FX32_ONE, 0);
	}
	else {
		GmComEfctSetDispOffset(efct_work, 0, GMD_PLY_EFCT_SPIN_JUMP_BLUR_BASE_OFST_Y*FX32_ONE, 0);
	}
}
// ==========================================================================
// gmPlyEfctSpinJumpBlurDest
/*!
 *	スピンジャンプ ブラー デストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmPlyEfctSpinJumpBlurDest(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_PLAYER_WORK	*ply_work;

	if (obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER) {
		ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;
	}
	else {
		ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	}

	if (ply_work->efct_spin_jump_blur == obj_work) {
		// 管理からはずす
		ply_work->efct_spin_jump_blur = NULL;
	}

	// 汎用終了処理
	ObjObjectExit(tcb);
}

// ==========================================================================
// スーパーソニックオーラ
// ==========================================================================
// ==========================================================================
// gmPlyEfctSuperAuraMain
/*!
 *	スーパーソニックオーラ 装飾, ベース
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctSuperAuraMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;
	if (!ply_work || !(ply_work->player_flag & GMD_PLF_SUPER_SONIC) || (ply_work->player_flag & GMD_PLF_DIE) ||
			(ply_work->act_state == GME_PLY_ACT_STATE_GMK_ENDING_FNS2)) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;

		// 汎用処理
		GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
		return;
	}

	// 表示OFFチェック
	if (ply_work->gmk_flag2 & GMD_PLGF2_SUPEREFCT_DISP_OFF) {
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
	else {
		obj_work->disp_flag &= ~OBD_DISP_NODISP;
	}

	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// プレイヤートロッコ用中心座標を反映
		obj_work->pos.x = FXM_FLOAT_TO_FX32(NNM_MTX(ply_work->truck_mtx_ply_mtn_pos, 0, 3));
		obj_work->pos.y = FXM_FLOAT_TO_FX32(-NNM_MTX(ply_work->truck_mtx_ply_mtn_pos, 1, 3));
		obj_work->pos.z = FXM_FLOAT_TO_FX32(NNM_MTX(ply_work->truck_mtx_ply_mtn_pos, 2, 3));
	}

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);

	// エンディングならスケールもソニックに合わせる
	if (GMM_MAIN_STAGE_IS_ENDING()) {
		obj_work->scale = ply_work->obj_work.scale;
	}
}

// ==========================================================================
// gmPlyEfctSuperAuraSpinMain
/*!
 *	スーパーソニックオーラ スピン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctSuperAuraSpinMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	// 終了確認
	if ( !ply_work ||
		(((ply_work->player_flag & GMD_PLF_PINBALL_SONIC) &&
			!(ply_work->seq_state == GME_PLY_SEQ_STATE_FW ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_WALK ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_JUMP ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_FALL ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_JUMPDASH ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_HOMING ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_PINBALL ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_PINBALL_AIR ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_FLIPPER)) ||
		(!(ply_work->player_flag & GMD_PLF_PINBALL_SONIC) &&
			!((ply_work->seq_state == GME_PLY_SEQ_STATE_JUMP ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_PINBALL ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_PINBALL_AIR ||
				ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_FLIPPER) &&
					(ply_work->act_state == GME_PLY_ACT_STATE_JUMP_SPIN ||
						ply_work->act_state == GME_PLY_ACT_STATE_SPIN ||
						ply_work->act_state == GME_PLY_ACT_STATE_SPIN_SMALL)))) &&
		(!GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) ) {

		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;

		// 汎用処理
		GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
		return;
	}

	// 表示OFFチェック
	if (ply_work->gmk_flag2 & GMD_PLGF2_SUPEREFCT_DISP_OFF) {
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
	else {
		obj_work->disp_flag &= ~OBD_DISP_NODISP;
	}

	// 通常チェック
	gmPlyEfctSuperAuraMain(obj_work);
}

// ==========================================================================
// gmPlyEfctSuperAuraDashMain
/*!
 *	スーパーソニックオーラ ダッシュ
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlyEfctSuperAuraDashMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	// 終了確認
	if (!ply_work || ply_work->act_state != GME_PLY_ACT_STATE_DASH_2) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEndCopyDirZ;

		// 汎用処理
		GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
		return;
	}

	// 表示OFFチェック
	if (ply_work->gmk_flag2 & GMD_PLGF2_SUPEREFCT_DISP_OFF) {
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
	else {
		obj_work->disp_flag &= ~OBD_DISP_NODISP;
	}

	// 通常チェック
	gmPlyEfctSuperAuraMain(obj_work);
}

// ==========================================================================
// gmPlyEfctSteamPipeMain
/*!
 *	プレイヤー スチームパイプ導入時エフェクト
 */
// ==========================================================================
void gmPlyEfctSteamPipeMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	MTM_ASSERT(obj_work->parent_obj && obj_work->parent_obj->obj_type == GMD_OBJTYPE_PLAYER);

	ply_work = (GMS_PLAYER_WORK*)obj_work->parent_obj;

	if (ply_work->obj_work.spd.x) {
		// 終了
		ObjDrawKillAction3DES(obj_work);

		// 終了処理に任せる
		obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEnd;
	}
}

// ==========================================================================
// GmPlyEfctStaticVarInit
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void GmPlyEfctStaticVarInit(void)
{
	gm_ply_efct_trail_sys_tcb = NULL;
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
