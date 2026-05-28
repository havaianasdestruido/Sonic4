// ==========================================================================
/*!
  @file gmGameDBuild.cpp
  @brief ゲームデータ構築

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmGameDBuild.cpp 89 2011-05-11 02:53:59Z thamada $
  $Date:: 2011-05-11 11:53:59 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gmMain.h"
#include "gmMainDat.h"
#include "gmGameDat.h"
#include "gmPlayer.h"
#include "gmMap.h"
#include "gmMapFar.h"
#include "gmDeco.h"
#include "gmWaterSurface.h"
#include "gmEventMgr.h"
#include "gmRing.h"
#include "gmSound.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectZone.h"
#include "gmEffectEnemy.h"
#if !defined(SONIC4_TRIAL)
#include "gmEffectBoss.h"
#endif // !defined(SONIC4_TRIAL)

#include "gmClearDemo.h"
#include "gmStartDemo.h"
#include "gmFix.h"
#include "gmOver.h"
#include "gmPauseMenu.h"
#include "gmStartMsg.h"

#include "gmGameDBuild.h"

#include "gmGmkSpring.h"
#include "gmGmkNeedle.h"
#include "gmGmkLand.h"
#include "gmGmkDashPanel.h"
#include "gmGmkGoalPanel.h"
#include "gmGmkBreakLand.h"
#include "gmGmkBreakObj.h"
#include "gmGmkBreakWall.h"
#include "gmGmkPointMarker.h"
#include "gmGmkItem.h"
#include "gmGmkCapsule.h"
#include "gmGmkAnimal.h"
#include "gmGmkSplRing.h"
#include "gmGmkSwitch.h"		/* zone3, zone4 */

//  zone1
#include "gmGmkPulley.h"
#include "gmGmkTarzanRope.h"
#include "gmGmkBridge.h"

//	zone2
#include "gmGmkStopper.h"
#include "gmGmkBumper.h"
#include "gmGmkEnBmpr.h"
#include "gmGmkBobbin.h"
#include "gmGmkFlipper.h"
#include "gmGmkSlot.h"
#include "gmGmkCannon.h"
#include "gmGmkSpCtplt.h"
#include "gmGmkShutter.h"
#include "gmGmkNeedleNeon.h"

//  zone3
#include "gmGmkWaterSlider.h"
#include "gmGmkRock.h"
#include "gmGmkSpear.h"			/* +zone4 */
#include "gmGmkPressWall.h"		/* +zone4 */
#include "gmGmkDrainTank.h"
#include "gmGmkTruck.h"
#include "gmGmkRockRide.h"
#include "gmGmkSwWall.h"		/* +zone4 */
#if !defined(SONIC4_TRIAL)
#include "gmGmkBoss3Pillar.h"
#endif // !defined(SONIC4_TRIAL)
#include "gmGmkDSign.h"

//	zone4
#include "gmGmkPiston.h"
#include "gmGmkBeltConveyor.h"
#include "gmGmkUpBumper.h"
#include "gmGmkSeesaw.h"
#include "gmGmkGear.h"
#include "gmGmkSteamPipe.h"
#include "gmGmkPopSteam.h"
#include "gmGmkPressPillar.h"

#include "gmEneHari.h"
#include "gmEneMotora.h"
#include "gmEneGabu.h"
#include "gmEneSting.h"
#include "gmEneMereon.h"
#include "gmEneMogurin.h"
#include "gmEneGardon.h"
#include "gmEneTeruStar.h"
#include "gmEneKaniPunch.h"
#include "gmEneHarogen.h"
#include "gmEneUnides.h"
#include "gmEneUniuni.h"
#include "gmEneBukubuku.h"
#include "gmEneKama.h"

#if !defined(SONIC4_TRIAL)
// boss
#include "gmBoss1.h"
#include "gmBoss2.h"
#include "gmBoss3.h"
#include "gmBoss4.h"
#include "gmBoss5.h"
#endif // !defined(SONIC4_TRIAL)

// SpecialStage
#include "gmGmkSsSquare.h"
#include "gmGmkSsCircle.h"
#include "gmGmkSsEndurance.h"
#include "gmGmkSsGoal.h"
#include "gmGmkSsEmerald.h"
#include "gmGmkSsTime.h"
#include "gmGmkSsRingGate.h"
#include "gmGmkSsArrow.h"
#include "gmGmkSsOblong.h"

//Ending
#include "GmEnding.h"
#include "dmStaffRollMdlCtrl.h"

#if _IPHONE
#include "gmTvx.h"
#endif // _IPHONE

//----- Definitions ---------------------------------------------------------
#define GMS_GAME_DBUILD_BUILD_MDL_WORK_NUM	(64)

/// モデル構築･片付けステータス
typedef enum tag_GME_GAME_DBUILD_MDL_STATE {
	GME_GAME_DBUILD_MDL_STATE_REG_WAIT	= 0,	//!< ビルド登録待機中
	GME_GAME_DBUILD_MDL_STATE_BUILD_WAIT,		//!< ビルド終了待機中

	GME_GAME_DBUILD_MDL_STATE_REG_FLUSH_WAIT,	//!< 開放登録待機中
	GME_GAME_DBUILD_MDL_STATE_FLUSH_WAIT,		//!< 開放終了待機中

	GME_GAME_DBUILD_MDL_STATE_MAX
} GME_GAME_DBUILD_MDL_STATE;
/// モデル構築･片付けワーク
typedef struct tag_GMS_GDBUILD_BUILD_MDL_WORK {
	GME_GAME_DBUILD_MDL_STATE	build_state;	//!< ビルドステート
	OBS_ACTION3D_NN_WORK		*obj_3d_list;	//!< 構築中の3Dモデルワーク
	s32							num;			//!< 構築中ワークの数
	s32							reg_num;		//!< 構築がすんだモデルの数
	AMS_AMB_HEADER				*mdl_amb;		//!< モデルAMB
	AMS_AMB_HEADER				*tex_amb;		//!< モデルTEX
	NNF_DRAWOBJ					draw_flag;		//!< モデル構築時のNNF_DRAWOBJ
	void						*txb;			//!< 差し替えテクスチャ用TXB (NULL可)
} GMS_GDBUILD_BUILD_MDL_WORK;


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGameDatBuildStage11(void);
static void gmGameDatBuildStage12(void);
static void gmGameDatBuildStage13(void);
static void gmGameDatBuildStage1Boss(void);
static void gmGameDatBuildStage21(void);
static void gmGameDatBuildStage22(void);
static void gmGameDatBuildStage23(void);
static void gmGameDatBuildStage2Boss(void);
static void gmGameDatBuildStage31(void);
static void gmGameDatBuildStage32(void);
static void gmGameDatBuildStage33(void);
static void gmGameDatBuildStage3Boss(void);
static void gmGameDatBuildStage41(void);
static void gmGameDatBuildStage42(void);
static void gmGameDatBuildStage43(void);
static void gmGameDatBuildStage4Boss(void);
static void gmGameDatBuildStageFinalBoss01(void);
static void gmGameDatBuildStageFinalBoss02(void);
static void gmGameDatBuildStageFinalBoss03(void);
static void gmGameDatBuildStageFinalBoss04(void);
static void gmGameDatBuildStageFinalBoss05(void);
static void gmGameDatBuildSS01(void);
static void gmGameDatBuildEnding(void);

static void gmGameDatFlushStage11(void);
static void gmGameDatFlushStage12(void);
static void gmGameDatFlushStage13(void);
static void gmGameDatFlushStage1Boss(void);
static void gmGameDatFlushStage21(void);
static void gmGameDatFlushStage22(void);
static void gmGameDatFlushStage23(void);
static void gmGameDatFlushStage2Boss(void);
static void gmGameDatFlushStage31(void);
static void gmGameDatFlushStage32(void);
static void gmGameDatFlushStage33(void);
static void gmGameDatFlushStage3Boss(void);
static void gmGameDatFlushStage41(void);
static void gmGameDatFlushStage42(void);
static void gmGameDatFlushStage43(void);
static void gmGameDatFlushStage4Boss(void);
static void gmGameDatFlushStageFinalBoss01(void);
static void gmGameDatFlushStageFinalBoss02(void);
static void gmGameDatFlushStageFinalBoss03(void);
static void gmGameDatFlushStageFinalBoss04(void);
static void gmGameDatFlushStageFinalBoss05(void);
static void gmGameDatFlushSS01(void);
static void gmGameDatFlushEnding(void);

static void gmGameDatBuildStageF_BB1(void);
static void gmGameDatBuildStageF_BB2(void);
static void gmGameDatBuildStageF_BB3(void);
static void gmGameDatBuildStageF_BB4(void);
static void gmGameDatBuildStageF_BBF(void);
static void gmGameDatFlushStageF_BB1(void);
static void gmGameDatFlushStageF_BB2(void);
static void gmGameDatFlushStageF_BB3(void);
static void gmGameDatFlushStageF_BB4(void);
static void gmGameDatFlushStageF_BBF(void);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
/// ビルド関数テーブル
static void (*const gm_gamedat_build_area_tbl[GSD_MAIN_STAGE_ID_MAX])(void) = {
	// Zone1
	gmGameDatBuildStage11, gmGameDatBuildStage12, gmGameDatBuildStage13, gmGameDatBuildStage1Boss,
	// Zone2
	gmGameDatBuildStage21, gmGameDatBuildStage22, gmGameDatBuildStage23, gmGameDatBuildStage2Boss,
	// Zone3
	gmGameDatBuildStage31, gmGameDatBuildStage32, gmGameDatBuildStage33, gmGameDatBuildStage3Boss,
	// Zone4
	gmGameDatBuildStage41, gmGameDatBuildStage42, gmGameDatBuildStage43, gmGameDatBuildStage4Boss,
	// ZoneFinal
	gmGameDatBuildStageFinalBoss01, gmGameDatBuildStageFinalBoss02, gmGameDatBuildStageFinalBoss03,
	gmGameDatBuildStageFinalBoss04, gmGameDatBuildStageFinalBoss05,
	// SpecialStage
	gmGameDatBuildSS01, gmGameDatBuildSS01, gmGameDatBuildSS01, gmGameDatBuildSS01, 
	gmGameDatBuildSS01, gmGameDatBuildSS01, gmGameDatBuildSS01, 
	// Ending
	gmGameDatBuildEnding,
};

/// フラッシュ関数テーブル
static void (*const gm_gamedat_flush_area_tbl[GSD_MAIN_STAGE_ID_MAX])(void) = {
	// Zone1
	gmGameDatFlushStage11, gmGameDatFlushStage12, gmGameDatFlushStage13, gmGameDatFlushStage1Boss,
	// Zone2
	gmGameDatFlushStage21, gmGameDatFlushStage22, gmGameDatFlushStage23, gmGameDatFlushStage2Boss,
	// Zone3
	gmGameDatFlushStage31, gmGameDatFlushStage32, gmGameDatFlushStage33, gmGameDatFlushStage3Boss,
	// Zone4
	gmGameDatFlushStage41, gmGameDatFlushStage42, gmGameDatFlushStage43, gmGameDatFlushStage4Boss,
	// ZoneFinal
	gmGameDatFlushStageFinalBoss01, gmGameDatFlushStageFinalBoss02, gmGameDatFlushStageFinalBoss03,
	gmGameDatFlushStageFinalBoss04, gmGameDatFlushStageFinalBoss05,
	// SpecialStage
	gmGameDatFlushSS01, gmGameDatFlushSS01, gmGameDatFlushSS01, gmGameDatFlushSS01,
	gmGameDatFlushSS01, gmGameDatFlushSS01, gmGameDatFlushSS01, 
	// Ending
	gmGameDatFlushEnding,
};


/// モデル構築登録ワーク
static GMS_GDBUILD_BUILD_MDL_WORK gm_obj_build_model_work_buf[GMS_GAME_DBUILD_BUILD_MDL_WORK_NUM];
/// モデル構築登録数
static s32	gm_obj_build_model_work_reg_num = 0;


//----- Global Functions ----------------------------------------------------
// ==========================================================================
// データビルド
// ==========================================================================
// ==========================================================================
// GmGameDatBuildInit
/*!
 *	ゲームデータ構築 初期化
 *
 *	@note
 *		データを初期化を行う前の初期化を行います。
 */
// ==========================================================================
void GmGameDatBuildInit(void)
{
	s32				i;
	OBS_DATA_WORK	*data_work;

#if OBD_LOAD_INITIAL_DRAW
	ObjLoadSetInitDrawFlag(TRUE);	//	初回描画登録開始
#endif // OBD_LOAD_INITIAL_DRAW
	// モデルビルド初期化
	GmGameDBuildModelBuildInit();

	// データワークデータ登録 (開放はObjectシステムで)
	// エネミー
	for (i = 0; i < GMD_DWORK_NO_ENEMY_END - GMD_DWORK_NO_ENEMY_START; i++) {
		if (!g_gm_gamedat_enemy[i]) {
			continue;
		}
		data_work = ObjDataGet(GMD_DWORK_NO_ENEMY_START + i);
		ObjDataSet(data_work, g_gm_gamedat_enemy[i]);
		data_work->num |= OBD_DATA_ARCHIVE_FLAG;	// アーカイブ扱い
	}
	// ギミック
	for (i = 0; i < GMD_DWORK_NO_GMK_END - GMD_DWORK_NO_GMK_START; i++) {
		if (!g_gm_gamedat_gimmick[i]) {
			continue;
		}
		data_work = ObjDataGet(GMD_DWORK_NO_GMK_START + i);
		ObjDataSet(data_work, g_gm_gamedat_gimmick[i]);
		data_work->num |= OBD_DATA_ARCHIVE_FLAG;	// アーカイブ扱い
	}
	// リング
	for (i = 0; i < GMD_DWORK_NO_RING_END - GMD_DWORK_NO_RING_START; i++) {
		if (!g_gm_gamedat_ring[i]) {
			continue;
		}
		data_work = ObjDataGet(GMD_DWORK_NO_RING_START + i);
		ObjDataSet(data_work, g_gm_gamedat_ring[i]);
		data_work->num |= OBD_DATA_ARCHIVE_FLAG;	// アーカイブ扱い
	}
	// エフェクト
	for (i = 0; i < GMD_DWORK_NO_EFFECT_ARC_END - GMD_DWORK_NO_EFFECT_ARC_START; ++i) {
		if (!g_gm_gamedat_effect[i]) {
			continue;
		}
		data_work = ObjDataGet(GMD_DWORK_NO_EFFECT_ARC_START + i);
		ObjDataSet(data_work, g_gm_gamedat_effect[i]);
		data_work->num	|= OBD_DATA_ARCHIVE_FLAG;	// アーカイブ扱い
	}
}

// ==========================================================================
// GmGameDatBuildStandard
/*!
 *	ゲームデータ構築 標準データ
 *
 *	@note
 *		読み込み済みのデータを初期化します。\n
 *		ゲーム構造体に対象のステージを設定しておいてください。\n
 *		g_gs_main_sys_info のゲーム設定情報が正常に設定されている必要があります。
 */
// ==========================================================================
void GmGameDatBuildStandard(void)
{
	/* プレイヤー */
	GmPlayerBuild();

	/* サウンド */
	GmSoundBuild();

	/* 共通データ */
#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	// FIX
	GmFixBuildDataInit();
//#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	// クリアデモ
	GmClearDemoBuild();

	// スタートデモ
	GmStartDemoBuild();
	
	// ゲーム／タイムオーバー
	GmOverBuildDataInit();
	
	// ポーズメニュー
	GmPauseMenuBuildStart();
#endif /* !defined(GMD_DEBUG_NO_CREATE_COCKPIT) */
	
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// 共通エフェクト
	GmEfctCmnBuildDataInit();
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
	
#if !defined GMD_DEBUG_NO_CREATE_RING
	// リング
	GmRingBuild();
#endif
}

// ==========================================================================
// GmGameDatBuildArea
/*!
 *	ゲームデータ構築 エリアデータ
 *
 *	@note
 *		読み込み済みのデータを初期化します。\n
 *		ゲーム構造体に対象のステージを設定しておいてください。\n
 *		g_gs_main_sys_info のゲーム設定情報が正常に設定されている必要があります。
 */
// ==========================================================================
void GmGameDatBuildArea(void)
{
#if _IPHONE
	GmTvxBuild();
#endif // _IPHONE
	/* 背景 */
	// MAPデータ
	GmMapBuildDataInit();
	// MAP地形データ
	GmMapBuildColData();
	// 遠景
#if GMD_MAP_FAR_TEST
	GmMapFarBuildData();
#endif	//GMD_MAP_FAR_TEST
	// 装飾
#if GMD_DECO_TEST
	GmDecoBuildData();
#endif	//GMD_DECO_TEST
	//水面
	GmWaterSurfaceBuildData();
	// イベント
	GmEventDataBuild();
	// g_gm_gamedat_map_arc を開放
//	if (g_gm_gamedat_map_arc != NULL) {
//		mtMemFreeMain(g_gm_gamedat_map_arc);
//		g_gm_gamedat_map_arc = NULL;
//	}

	/* 共通ビルド */
	// 装飾
	//GmDecoBuild();

#if !defined GMD_DEBUG_NO_CREATE_GIMMICK

	// 共通ギミック
	GmGmkSpringBuild();		// スプリング
	GmGmkDashPanelBuild();	// ダッシュパネル
	GmGmkGoalPanelBuild();	// ゴールパネル

	GmGmkItemBuild();		// アイテム
	
	GmGmkNeedleBuild();		// トゲ
		
	GmGmkPointMarkerBuild();// ポイントマーカー
	GmGmkAnimalBuild();		// 動物たち
	GmGmkSplRingBuild();	// SPLリング
#endif


#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// 共通エネミー
	if ( g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_BOSS ||
			((!GMM_MAIN_STAGE_IS_BOSS()) && (!GMM_MAIN_STAGE_IS_SS()) && (!GMM_MAIN_STAGE_IS_ENDING())) ) {
		GmEneHariSenboBuild();	// ハリセンボ
	}
#endif
	
#if !defined(SONIC4_TRIAL)
	// ボス専用エフェクト構築初期化
	// （ロードは開始しません。ボスの有無に関わらず、エリア別ビルド前のタイミングで呼ぶ）
	GmEfctBossBuildSingleDataInit();
#endif // !defined(SONIC4_TRIAL)

	/* エリア別ビルド */
	// 共通
	// 敵
	// ギミック
	if (gm_gamedat_build_area_tbl[g_gs_main_sys_info.stage_id]) {
		gm_gamedat_build_area_tbl[g_gs_main_sys_info.stage_id]();
	}

}

// ==========================================================================
// GmGameDatBuildStandardCheck
/*!
 *	ゲームデータ構築 標準データ 構築終了チェック
 *
 *	@return	TRUE : 構築終了
 *
 *	@note
 *		標準データが構築終了済みかをチェックします。
 */
// ==========================================================================
BOOL GmGameDatBuildStandardCheck(void)
{
	// プレイヤー
	if (GmPlayerBuildCheck() == FALSE) {
		return (FALSE);
	}

	// サウンド
	if (GmSoundBuildCheck() == FALSE) {
		return (FALSE);
	}
#if !defined GMD_DEBUG_NO_CREATE_RING
	// リング
	if (GmRingBuildCheck() == FALSE) {
		return (FALSE);
	}
#endif

#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	// FIX
	if (GmFixBuildDataLoop() == FALSE) {
		return (FALSE);
	}
//#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	// クリアデモ
	if (GmClearDemoBuildCheck() == FALSE) {
		return (FALSE);
	}

	// スタートデモ
	if (GmStartDemoBuildCheck() == FALSE) {
		return (FALSE);
	}

	// ゲーム／タイムオーバー
	if (GmOverBuildDataLoop() == FALSE) {
		return (FALSE);
	}

	// ポーズメニュー
	if (GmPauseMenuBuildIsFinished() == FALSE) {
		return (FALSE);
	}
#endif /* !defined(GMD_DEBUG_NO_CREATE_COCKPIT) */
	
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// 共通エフェクト
	if (GmEfctCmnBuildDataLoop() == FALSE) {
		return (FALSE);
	}
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */

	return (TRUE);
}

// ==========================================================================
// GmGameDatBuildAreaCheck
/*!
 *	ゲームデータ構築 エリアデータ 構築終了チェック
 *
 *	@return	TRUE : 構築終了
 *
 *	@note
 *		エリアデータが構築終了済みかをチェックします。
 */
// ==========================================================================
BOOL GmGameDatBuildAreaCheck(void)
{
	BOOL	b_finish = TRUE;

	// MAPデータ
	if (!GmMapBuildDataLoop()) {
		b_finish = FALSE;
	}
	// MAP地形データ
	// 完了復帰のためチェックなし

	//遠景
#if	GMD_MAP_FAR_TEST
	if ( !GmMapFarCheckLoading() ){
		b_finish = FALSE;
	}
#endif	//GMD_MAP_FAR_TEST

	//装飾
#if	GMD_DECO_TEST
	if ( !GmDecoCheckLoading() ){
		b_finish = FALSE;
	}
#endif	//GMD_DECO_TEST

	//水面
	if ( !GmWaterSurfaceCheckLoading() ){
		b_finish = FALSE;
	}

	// イベント
	// 完了復帰のためチェックなし

	// モデル構築(エネミー ギミック等)
	if (!GmGameDBuildCheckBuildModel()) {
		b_finish = FALSE;
	}

	// ゾーン専用エフェクト構築
	if (!GmEfctZoneBuildDataLoop()) {
		b_finish = FALSE;
	}
	
	// エネミー専用エフェクト構築
	if (!GmEfctEneBuildDataLoop()) {
		b_finish = FALSE;
	}
	
#if !defined(SONIC4_TRIAL)
	// ボス共通エフェクト構築
	if (!GmEfctBossCmnBuildDataLoop()) {
		b_finish = FALSE;
	}
	
	// ボス専用エフェクト構築
	if (!GmEfctBossBuildSingleDataLoop()) {
		b_finish = FALSE;
	}
#endif // !defined(SONIC4_TRIAL)
	
	// 開始時メッセージ
	if (!GmStartMsgBuildCheck()) {
		b_finish = FALSE;
	}

	return (b_finish);
}

// ==========================================================================
// データフラッシュ
// ==========================================================================
// ==========================================================================
// GmGameDatFlushInit
/*!
 *	ゲームデータ片付け 初期化
 *
 *	@note
 *		データ片付けを行う前の初期化を行います。
 */
// ==========================================================================
void GmGameDatFlushInit(void)
{
#if OBD_LOAD_INITIAL_DRAW
	ObjLoadSetInitDrawFlag(FALSE);	//	初回描画登録終了
#endif // OBD_LOAD_INITIAL_DRAW
	GmGameDBuildModelFlushInit();
}

// ==========================================================================
// GmGameDatFlushStandard
/*!
 *	ゲームデータ片付け 標準データ
 *
 *	@note
 *		構築したデータを片付けます。\n
 *		g_gs_main_sys_info のゲーム設定情報が正常に設定されている必要があります。
 */
// ==========================================================================
void GmGameDatFlushStandard(void)
{
	// Build と逆に
#if !defined GMD_DEBUG_NO_CREATE_RING
	// リング
	GmRingFlush();
#endif

#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// 共通エフェクト
	GmEfctCmnFlushDataInit();
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */

#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	// ポーズメニュー
	GmPauseMenuFlushStart();

	// ゲーム／タイムオーバー
	GmOverFlushDataInit();

	// スタートデモ
	GmStartDemoFlush();

	// クリアデモ
	GmClearDemoFlush();
//#endif /* !defined(GMD_DEBUG_NO_CREATE_COCKPIT) */
	/* 共通データ */
	// FIX
	GmFixFlushDataInit();
#endif /* !defined(GMD_DEBUG_NO_CREATE_COCKPIT) */

	/* サウンド */
	GmSoundFlush();

	/* プレイヤー */
	GmPlayerFlush();
}

// ==========================================================================
// GmGameDatFlushArea
/*!
 *	ゲームデータ片付け エリアデータ
 *
 *	@note
 *		構築したデータを片付けます。\n
 *		g_gs_main_sys_info のゲーム設定情報が正常に設定されている必要があります。
 */
// ==========================================================================
void GmGameDatFlushArea(void)
{
	// Buildと逆に
	/* エリア毎フラッシュ処理 */
	// 敵
	// ギミック
	if (gm_gamedat_flush_area_tbl[g_gs_main_sys_info.stage_id]) {
		gm_gamedat_flush_area_tbl[g_gs_main_sys_info.stage_id]();
	}

#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// 共通エネミー
	if ( g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_BOSS ||
			((!GMM_MAIN_STAGE_IS_BOSS()) && (!GMM_MAIN_STAGE_IS_SS()) && (!GMM_MAIN_STAGE_IS_ENDING())) ) {
		GmEneHariSenboFlush();	// ハリセンボ
	}
#endif

	// Zone1-1フラッシュへ移動
//	GmGmkPistonFlush();			// ピストン(ゾーン４)
//	GmGmkBeltConveyorFlush();	// ベルトコンベヤー(ゾーン４)
//	GmGmkStopperFlush();		// ストッパー(ゾーン２)
//	GmGmkUpBumperFlush();		// 登るバンパー(ゾーン４)
//	GmGmkSpearFlush();			// 槍(ゾーン２)

#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	/* 共通フラッシュ処理 */
	// 共通ギミック
	GmGmkSplRingFlush();	// SPLリング
	GmGmkAnimalFlush();		// 動物たち
	GmGmkPointMarkerFlush();// ポイントマーカー
	
	GmGmkNeedleFlush();		// トゲ

	GmGmkItemFlush();		// アイテム

	GmGmkGoalPanelFlush();	// ゴールパネル
	GmGmkDashPanelFlush();	// ダッシュパネル
	GmGmkSpringFlush();		// スプリング
#endif

	/* 背景 */
	// イベント
	GmEventDataFlush();
	// 水面
	GmWaterSurfaceFlushData();
	// 装飾
#if	GMD_DECO_TEST
	GmDecoFlushData();
#endif	//GMD_DECO_TEST
	// 遠景
#if	GMD_MAP_FAR_TEST
	GmMapFarFlushData();
#endif	//GMD_MAP_FAR_TEST
	// MAP地形データ
	GmMapFlushColData();
	// MAPデータ
	GmMapFlushData();
#if _IPHONE
	GmTvxFlush();
#endif // _IPHONE
}

// ==========================================================================
// GmGameDatFlushStandardCheck
/*!
 *	ゲームデータ片付け 標準データ 片付け終了チェック
 *
 *	@return	TRUE : 終了
 *
 *	@note
 *		標準データが片付け終了済みかをチェックします。
 */
// ==========================================================================
BOOL GmGameDatFlushStandardCheck(void)
{
#if !defined GMD_DEBUG_NO_CREATE_RING
	// リング
	if (!GmRingFlushCheck()) {
		return (FALSE);
	}
#endif

#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// 共通エフェクト
	if (GmEfctCmnFlushDataLoop() == FALSE) {
		return (FALSE);
	}
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */

#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	// FIX
	if (GmFixFlushDataLoop() == FALSE) {
		return (FALSE);
	}
//#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	// スタートデモ
	if (GmStartDemoFlushCheck() == FALSE) {
		return (FALSE);
	}

	// クリアデモ
	if (GmClearDemoFlushCheck() == FALSE) {
		return (FALSE);
	}

	// ゲーム／タイムオーバー
	if (GmOverFlushDataLoop() == FALSE) {
		return (FALSE);
	}

	// ポーズメニュー
	if (GmPauseMenuFlushIsFinished() == FALSE) {
		return (FALSE);
	}
#endif /* !defined(GMD_DEBUG_NO_CREATE_COCKPIT) */

	// モデル開放(エネミー ギミック等)
	if (!GmGameDBuildCheckFlushModel()) {
		return (FALSE);
	}

	// 水面
#if GMD_WATER_SURFACE_USE_RENDER
	if (GmWaterSurfaceCheckFlush() == FALSE) {
		return (FALSE);
	}
#endif	// GMD_WATER_SURFACE_USE_RENDER

	// サウンド
	// 完了復帰のためチェックなし

	// プレイヤー
	if (GmPlayerFlushCheck() == FALSE) {
		return (FALSE);
	}

	return (TRUE);
}

// ==========================================================================
// GmGameDatFlushAreaCheck
/*!
 *	ゲームデータ片付け エリアデータ 片付け終了チェック
 *
 *	@return	TRUE : 終了
 *
 *	@note
 *		エリアデータが片付け終了済みかをチェックします。
 */
// ==========================================================================
BOOL GmGameDatFlushAreaCheck(void)
{
	BOOL	b_finish = TRUE;

	// 開始時メッセージ
	if (!GmStartMsgFlushCheck()) {
		b_finish = FALSE;
	}

	// ゾーン専用エフェクトフラッシュ
	if (!GmEfctZoneFlushDataLoop()) {
		b_finish = FALSE;
	}
	
	// エネミー専用エフェクトフラッシュ
	if (!GmEfctEneFlushDataLoop()) {
		b_finish = FALSE;
	}
	
#if !defined(SONIC4_TRIAL)
	// ボス共通エフェクトフラッシュ
	if (!GmEfctBossCmnFlushDataLoop()) {
		b_finish = FALSE;
	}
	
	// ボス専用エフェクトフラッシュ
	if (!GmEfctBossFlushSingleDataLoop()) {
		b_finish = FALSE;
	}
#endif // !defined(SONIC4_TRIAL)

	if (!GmMapFlushDataLoop()) {
		b_finish = FALSE;
	}

#if _IPHONE
	// 装飾フラッシュ
#if	GMD_DECO_TEST
	if (!GmDecoCheckFlushing()) {
		b_finish = FALSE;
	}
#endif	//GMD_DECO_TEST
#endif // _IPHONE

	return (b_finish);
}


//////////////////////////////////////////////////////////
// ==========================================================================
// データビルド ボス連戦用
// ==========================================================================
/// ビルド関数テーブル
static void (*const gm_gamedat_build_boss_buttle_tbl[GMD_GAMEDAT_LOAD_BOSS_TYPE_MAX])(void) = {
	// Zone1
	gmGameDatBuildStageF_BB1,
	gmGameDatBuildStageF_BB2,
	gmGameDatBuildStageF_BB3,
	gmGameDatBuildStageF_BB4,
	gmGameDatBuildStageF_BBF,
};

// ==========================================================================
// GmGameDatBuildBossBattleInit
/*!
 *	ゲームデータ構築 初期化 ボス連戦用
 *
 *	@note
 *		データを初期化を行う前の初期化を行います。
 */
// ==========================================================================
void GmGameDatBuildBossBattleInit(void)
{
#if !defined(SONIC4_TRIAL)
	s32				i;
	OBS_DATA_WORK	*data_work;

	// モデルビルド初期化
	GmGameDBuildModelBuildInit();

	// データワークデータ登録 (開放はObjectシステムで)
	// エネミー
	for (i = 0; i < GMD_DWORK_NO_ENEMY_END - GMD_DWORK_NO_ENEMY_START; i++) {
		if (!g_gm_gamedat_enemy[i]) {
			continue;
		}
		data_work = ObjDataGet(GMD_DWORK_NO_ENEMY_START + i);
		ObjDataSet(data_work, g_gm_gamedat_enemy[i]);
		data_work->num |= OBD_DATA_ARCHIVE_FLAG;	// アーカイブ扱い
	}
#if 0
	// ギミック
	for (i = 0; i < GMD_DWORK_NO_GMK_END - GMD_DWORK_NO_GMK_START; i++) {
		if (!g_gm_gamedat_gimmick[i]) {
			continue;
		}
		data_work = ObjDataGet(GMD_DWORK_NO_GMK_START + i);
		ObjDataSet(data_work, g_gm_gamedat_gimmick[i]);
		data_work->num |= OBD_DATA_ARCHIVE_FLAG;	// アーカイブ扱い
	}
	// エフェクト
	for (i = 0; i < GMD_DWORK_NO_EFFECT_ARC_END - GMD_DWORK_NO_EFFECT_ARC_START; ++i) {
		if (!g_gm_gamedat_effect[i]) {
			continue;
		}
		data_work = ObjDataGet(GMD_DWORK_NO_EFFECT_ARC_START + i);
		ObjDataSet(data_work, g_gm_gamedat_effect[i]);
		data_work->num	|= OBD_DATA_ARCHIVE_FLAG;	// アーカイブ扱い
	}
#endif
#endif // !defined(SONIC4_TRIAL)
}

// ==========================================================================
// GmGameDatBuildBossBattle
/*!
 *	ゲームデータ構築 ボス連戦用
 *
 *	@param	boss_type	[in]	ビルドタイプ	GME_GAMEDAT_LOAD_BOSS_TYPE
 *
 *	@note
 *		読み込み済みのデータを初期化します。\n
 *		ゲーム構造体に対象のステージを設定しておいてください。\n
 *		g_gs_main_sys_info のゲーム設定情報が正常に設定されている必要があります。
 */
// ==========================================================================
void GmGameDatBuildBossBattle(s32 boss_type)
{
#if !defined(SONIC4_TRIAL)
	s32	stage_id;

	MTM_ASSERT((u32)boss_type < GMD_GAMEDAT_LOAD_BOSS_TYPE_MAX);

	// ステージID取得
	stage_id = g_gm_gamedat_bossbattle_stage_id_tbl[boss_type];

#if 0
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK

	// 共通ギミック
	GmGmkSpringBuild();		// スプリング
	GmGmkDashPanelBuild();	// ダッシュパネル
	GmGmkGoalPanelBuild();	// ゴールパネル

	GmGmkItemBuild();		// アイテム
	
	GmGmkNeedleBuild();		// トゲ
		
	GmGmkPointMarkerBuild();// ポイントマーカー
	GmGmkAnimalBuild();		// 動物たち
	GmGmkSplRingBuild();	// SPLリング
#endif


#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// 共通エネミー
	if ( g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_BOSS ||
			((!GMM_MAIN_STAGE_IS_BOSS()) && (!GMM_MAIN_STAGE_IS_SS()) && (!GMM_MAIN_STAGE_IS_ENDING())) ) {
		GmEneHariSenboBuild();	// ハリセンボ
	}
#endif
#endif
	
	// ボス専用エフェクト構築初期化
	// （ロードは開始しません。ボスの有無に関わらず、エリア別ビルド前のタイミングで呼ぶ）
	GmEfctBossBuildSingleDataInit();

	/* エリア別ビルド */
	// 共通
	// 敵
	// ギミック
	if (gm_gamedat_build_boss_buttle_tbl[boss_type]) {
		gm_gamedat_build_boss_buttle_tbl[boss_type]();
	}
#endif // !defined(SONIC4_TRIAL)
}

// ==========================================================================
// GmGameDatBuildBossBattleCheck
/*!
 *	ゲームデータ構築 構築終了チェック ボス連戦用
 *
 *	@return	TRUE : 構築終了
 *
 *	@note
 *		データが構築終了済みかをチェックします。
 */
// ==========================================================================
BOOL GmGameDatBuildBossBattleCheck(void)
{
#if !defined(SONIC4_TRIAL)
	BOOL	b_finish = TRUE;

	// モデル構築(エネミー ギミック等)
	if (!GmGameDBuildCheckBuildModel()) {
		b_finish = FALSE;
	}

	// ゾーン専用エフェクト構築
	if (!GmEfctZoneBuildDataLoop()) {
		b_finish = FALSE;
	}
	
	// エネミー専用エフェクト構築
	if (!GmEfctEneBuildDataLoop()) {
		b_finish = FALSE;
	}
	
	// ボス共通エフェクト構築
	if (!GmEfctBossCmnBuildDataLoop()) {
		b_finish = FALSE;
	}
	
	// ボス専用エフェクト構築
	if (!GmEfctBossBuildSingleDataLoop()) {
		b_finish = FALSE;
	}

	return (b_finish);
	
#else
	return TRUE;
#endif // !defined(SONIC4_TRIAL)
}

// ==========================================================================
// データフラッシュ
// ==========================================================================
/// ビルド関数テーブル
static void (*const gm_gamedat_flush_boss_buttle_tbl[GMD_GAMEDAT_LOAD_BOSS_TYPE_MAX])(void) = {
	// Zone1
	gmGameDatFlushStageF_BB1,
	gmGameDatFlushStageF_BB2,
	gmGameDatFlushStageF_BB3,
	gmGameDatFlushStageF_BB4,
	gmGameDatFlushStageF_BBF,
};
// ==========================================================================
// GmGameDatFlushBossBattleInit
/*!
 *	ゲームデータ片付け ボス連戦用
 *
 *	@note
 *		データ片付けを行う前の初期化を行います。
 */
// ==========================================================================
void GmGameDatFlushBossBattleInit(void)
{
#if !defined(SONIC4_TRIAL)
	GmGameDBuildModelFlushInit();
#endif // !defined(SONIC4_TRIAL)
}

// ==========================================================================
// GmGameDatFlushBossBattle
/*!
 *	ゲームデータ片付け ボス連戦用
 *
 *	@param	boss_type	[in]	フラッシュタイプ GME_GAMEDAT_LOAD_BOSS_TYPE
 *
 *	@note
 *		構築したデータを片付けます。\n
 *		g_gs_main_sys_info のゲーム設定情報が正常に設定されている必要があります。
 */
// ==========================================================================
void GmGameDatFlushBossBattle(s32 boss_type)
{
#if !defined(SONIC4_TRIAL)
	s32	stage_id;

	MTM_ASSERT((u32)boss_type < GMD_GAMEDAT_LOAD_BOSS_TYPE_MAX);

	// ステージID取得
	stage_id = g_gm_gamedat_bossbattle_stage_id_tbl[boss_type];

	// Buildと逆に
	/* エリア毎フラッシュ処理 */
	// 敵
	// ギミック
	if (gm_gamedat_flush_boss_buttle_tbl[boss_type]) {
		gm_gamedat_flush_boss_buttle_tbl[boss_type]();
	}

#if 0
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// 共通エネミー
	if ( g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_BOSS ||
			((!GMM_MAIN_STAGE_IS_BOSS()) && (!GMM_MAIN_STAGE_IS_SS()) && (!GMM_MAIN_STAGE_IS_ENDING())) ) {
		GmEneHariSenboFlush();	// ハリセンボ
	}
#endif

#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	/* 共通フラッシュ処理 */
	// 共通ギミック
	GmGmkSplRingFlush();	// SPLリング
	GmGmkAnimalFlush();		// 動物たち
	GmGmkPointMarkerFlush();// ポイントマーカー
	
	GmGmkNeedleFlush();		// トゲ

	GmGmkItemFlush();		// アイテム

	GmGmkGoalPanelFlush();	// ゴールパネル
	GmGmkDashPanelFlush();	// ダッシュパネル
	GmGmkSpringFlush();		// スプリング
#endif
#endif
#endif // !defined(SONIC4_TRIAL)
}

// ==========================================================================
// GmGameDatFlushBossBattleCheck
/*!
 *	ゲームデータ片付け 片付け終了チェック ボス連戦用
 *
 *	@return	TRUE : 終了
 *
 *	@note
 *		データが片付け終了済みかをチェックします。
 */
// ==========================================================================
BOOL GmGameDatFlushBossBattleCheck(void)
{
#if !defined(SONIC4_TRIAL)
	BOOL	b_finish = TRUE;

	// モデル開放(エネミー ギミック等)
	if (!GmGameDBuildCheckFlushModel()) {
		b_finish = FALSE;
	}

	// ゾーン専用エフェクトフラッシュ
	if (!GmEfctZoneFlushDataLoop()) {
		b_finish = FALSE;
	}

#if 0
	// エネミー専用エフェクトフラッシュ
	if (!GmEfctEneFlushDataLoop()) {
		b_finish = FALSE;
	}
#endif
	
#if 0
	// ボス共通エフェクトフラッシュ
	if (!GmEfctBossCmnFlushDataLoop()) {
		b_finish = FALSE;
	}
#endif
	
	// ボス専用エフェクトフラッシュ
	if (!GmEfctBossFlushSingleDataLoop()) {
		b_finish = FALSE;
	}

	return (b_finish);
	
#else
	return TRUE;
#endif // !defined(SONIC4_TRIAL)
}



////////////////////////////////////////////////////////////

// ==========================================================================
// モデルデータ構築
// ==========================================================================
// ==========================================================================
// GmGameDBuildModelBuildInit
/*!
 *	3Dモデルデータ構築処理用初期化
 *
 *	@note
 *		GmObjのデータ構築処理を開始する前に初期化します。
 */
// ==========================================================================
void GmGameDBuildModelBuildInit(void)
{
	MTM_ASSERT(gm_obj_build_model_work_reg_num == 0);

	// モデル構築ワーク初期化
	MI_CpuClear8(gm_obj_build_model_work_buf, sizeof(gm_obj_build_model_work_buf));
	gm_obj_build_model_work_reg_num = 0;
}

// ==========================================================================
// GmGameDBuildRegBuildModel
/*!
 *	モデル構築登録
 *
 *	@param mdl_amb		[in] モデルAMB
 *	@param tex_amb		[in] テクスチャAMB
 *	@param draw_flag	[in] 描画フラグ
 *	@param txb			[in] 差し替えテクスチャ用TXB (NULL可)
 *
 *	@return	構築中 OBS_ACTION3D_NN_WORK * モデル数
 *
 *	@note
 *		mdl_amb に含まれるモデルをすべてOBS_ACTION3D_NN_WORKで初期化します。\n
 *		GmObjBuildModelで登録した後、GmGameDBuildCheckBuildModel でオブジェクト登録と\n
 *		登録終了確認を行う必要があります。\n
 *		取得される OBS_ACTION3D_NN_WORK はモデルの個数分です。\n
 *		GmGameDBuildCheckBuildModel でTRUEが返るまでは、構築は保障されません。
 */
// ==========================================================================
OBS_ACTION3D_NN_WORK* GmGameDBuildRegBuildModel(AMS_AMB_HEADER *mdl_amb, AMS_AMB_HEADER *tex_amb, NNF_DRAWOBJ draw_flag, void *txb/*=NULL*/)
{
	GMS_GDBUILD_BUILD_MDL_WORK	*build_work;

	MTM_ASSERT(mdl_amb);
	MTM_ASSERT(tex_amb);
	MTM_ASSERT(gm_obj_build_model_work_reg_num < GMS_GAME_DBUILD_BUILD_MDL_WORK_NUM);
	MTM_ASSERT(mdl_amb->file_num);

	// 登録ワーク取得
	build_work = &gm_obj_build_model_work_buf[gm_obj_build_model_work_reg_num];
	gm_obj_build_model_work_reg_num++;

	// モデル数取得
	build_work->num= mdl_amb->file_num;
	// 3Dモデルワーク取得
	build_work->obj_3d_list = (OBS_ACTION3D_NN_WORK*)amMemAlloc(sizeof(OBS_ACTION3D_NN_WORK) * build_work->num);
	MI_CpuClear8(build_work->obj_3d_list, sizeof(OBS_ACTION3D_NN_WORK) * build_work->num);
	// データ保存
	build_work->mdl_amb = mdl_amb;
	build_work->tex_amb = tex_amb;
	// 描画フラグ保存
	build_work->draw_flag = draw_flag;
	// TXB保存
	build_work->txb		= txb;
	// ビルドステート設定
	build_work->build_state = GME_GAME_DBUILD_MDL_STATE_REG_WAIT;

	return (build_work->obj_3d_list);
}

// ==========================================================================
// GmGameDBuildCheckBuildModel
/*!
 *	モデル構築登録 及び 終了チェック
 *
 *	@return	TRUE : 登録されたモデルの構築を終了
 *
 *	@note
 *		GmGameDBuildRegBuildModelで登録されたモデルの構築命令発行と\n
 *		終了チェックを行います。\n
 *		登録が終了するまで毎フレーム呼び出す必要があります。
 */
// ==========================================================================
BOOL GmGameDBuildCheckBuildModel(void)
{
	s32						i, j;
	GMS_GDBUILD_BUILD_MDL_WORK	*build_work;
	OBS_ACTION3D_NN_WORK	*obj_3d;
	BOOL					b_build_end = TRUE;

	if (gm_obj_build_model_work_reg_num) {
		for (i = gm_obj_build_model_work_reg_num - 1, build_work = &gm_obj_build_model_work_buf[i];
					i >= 0; i--, build_work--) {

			if (build_work->build_state == GME_GAME_DBUILD_MDL_STATE_REG_WAIT) {
				// 登録待機中
#if 1
				j = build_work->reg_num;	// 現在の登録状況
				obj_3d = build_work->obj_3d_list + j;
				if (!build_work->txb) {
					while ((GsMainSysGetDisplayListRegistNum() <= AMD_REGISTLIST_NUM - 4/*最低必要数*/ - 64/*余裕*/) &&
								j < build_work->num) {
						// 登録
						ObjAction3dNNModelLoad(obj_3d,
									NULL/*data_work*/, NULL/*file_name*/,
									j/*index*/, build_work->mdl_amb,
									NULL/*filename_tex*/, build_work->tex_amb,
									build_work->draw_flag/*NNF_DRAWOBJ drawflag*/);

						j++;
						obj_3d++;
					}

					if (j == build_work->reg_num) {
						// 登録バッファが空くまで待つ
						return (FALSE);
					}

					// 登録状況保存
					build_work->reg_num = j;

					// ビルド待機移行チェック
					if (build_work->reg_num == build_work->num) {
						build_work->build_state = GME_GAME_DBUILD_MDL_STATE_BUILD_WAIT;
					}
				}
				else {
					while ((GsMainSysGetDisplayListRegistNum() <= AMD_REGISTLIST_NUM - 4/*最低必要数*/ - 64/*余裕*/) &&
								j < build_work->num) {
						// 登録
						// TXBあり
						ObjAction3dNNModelLoadTxb(obj_3d,
									NULL/*data_work*/, NULL/*file_name*/,
									j/*index*/, build_work->mdl_amb,
									NULL/*filename_tex*/, build_work->tex_amb,
									build_work->draw_flag/*NNF_DRAWOBJ drawflag*/,
									build_work->txb);

						j++;
						obj_3d++;
					}

					if (j == build_work->reg_num) {
						// 登録バッファが空くまで待つ
						return (FALSE);
					}

					// 登録状況保存
					build_work->reg_num = j;

					// ビルド待機移行チェック
					if (build_work->reg_num == build_work->num) {
						build_work->build_state = GME_GAME_DBUILD_MDL_STATE_BUILD_WAIT;
					}
				}
				// 登録中
				b_build_end = FALSE;
#else
				if (GsMainSysGetDisplayListRegistNum() >= AMD_REGISTLIST_NUM - build_work->num*3 - 64/*余裕*/) {
					// 登録バッファが空くまで待つ
					return (FALSE);
				}

				// 登録
				if (!build_work->txb) {
					for (j = 0, obj_3d = build_work->obj_3d_list; j < build_work->num; j++, obj_3d++) {
						ObjAction3dNNModelLoad(obj_3d,
									NULL/*data_work*/, NULL/*file_name*/,
									j/*index*/, build_work->mdl_amb,
									NULL/*filename_tex*/, build_work->tex_amb,
									build_work->draw_flag/*NNF_DRAWOBJ drawflag*/);
					}
				}
				else {
					// TXBあり
					for (j = 0, obj_3d = build_work->obj_3d_list; j < build_work->num; j++, obj_3d++) {
						ObjAction3dNNModelLoadTxb(obj_3d,
									NULL/*data_work*/, NULL/*file_name*/,
									j/*index*/, build_work->mdl_amb,
									NULL/*filename_tex*/, build_work->tex_amb,
									build_work->draw_flag/*NNF_DRAWOBJ drawflag*/,
									build_work->txb);
					}
				}
				build_work->build_state = GME_GAME_DBUILD_MDL_STATE_BUILD_WAIT;

				b_build_end = FALSE;
#endif
			}
			else if (build_work->build_state == GME_GAME_DBUILD_MDL_STATE_BUILD_WAIT) {
				// ビルド終了待機中
				for (j = 0, obj_3d = build_work->obj_3d_list; j < build_work->num; j++, obj_3d++) {
					if (ObjAction3dNNModelLoadCheck(obj_3d) == FALSE) {
						// ビルド待機中 登録待ちへ渡す
						b_build_end = FALSE;
						break;
					}
				}

				// ビルド終了チェック
				if (j >= build_work->num && i == gm_obj_build_model_work_reg_num - 1) {
					// チェックをひとつ減らす
					gm_obj_build_model_work_reg_num--;
					MTM_ASSERT(gm_obj_build_model_work_reg_num >= 0);
				}
			}
#if defined (MTD_DEBUG)
			else {
				MTM_ASSERT(!"gmObj.cpp::GmGameDBuildCheckBuildModel() error build state\n");
			}
#endif
		}
	}

	return (b_build_end);
}

// ==========================================================================
// GmGameDBuildModelFlushInit
/*!
 *	GmObjのデータ片付け処理用初期化。
 *
 *	@note
 *		GmObjのデータ片付け処理を開始する前に初期化します。
 */
// ==========================================================================
void GmGameDBuildModelFlushInit(void)
{
	MTM_ASSERT(gm_obj_build_model_work_reg_num == 0);

	// モデル構築ワーク初期化
	MI_CpuClear8(gm_obj_build_model_work_buf, sizeof(gm_obj_build_model_work_buf));
	gm_obj_build_model_work_reg_num = 0;
}

// ==========================================================================
// GmGameDBuildRegFlushModel
/*!
 *	モデル片付け登録
 *
 *	@param mdl_amb	[in] モデルAMB
 *	@param tex_amb	[in] テクスチャAMB
 *
 *	@return	構築中 OBS_ACTION3D_NN_WORK * モデル数
 *
 *	@note
 *		mdl_amb に含まれるモデルをすべてOBS_ACTION3D_NN_WORKで初期化します。\n
 *		GmObjBuildModelで登録した後、GmGameDBuildCheckBuildModel でオブジェクト登録と\n
 *		登録終了確認を行う必要があります。\n
 *		取得される OBS_ACTION3D_NN_WORK はモデルの個数分です。\n
 *		GmGameDBuildCheckBuildModel でTRUEが返るまでは、構築は保障されません。
 */
// ==========================================================================
void GmGameDBuildRegFlushModel(OBS_ACTION3D_NN_WORK *obj_3d_list, s32 num)
{
	GMS_GDBUILD_BUILD_MDL_WORK	*build_work;

	MTM_ASSERT(obj_3d_list);
	MTM_ASSERT(num > 0);
	MTM_ASSERT(gm_obj_build_model_work_reg_num < GMS_GAME_DBUILD_BUILD_MDL_WORK_NUM);

	// 登録ワーク取得
	build_work = &gm_obj_build_model_work_buf[gm_obj_build_model_work_reg_num];
	gm_obj_build_model_work_reg_num++;

	// モデル数取得
	build_work->num = num;
	// 3Dモデルワーク保存
	build_work->obj_3d_list = obj_3d_list;
	// データ保存
	//build_work->mdl_amb = mdl_amb;
	//build_work->tex_amb = tex_amb;
	// 描画フラグ保存
	//build_work->draw_flag = draw_flag;
	// ビルドステート設定
	build_work->build_state = GME_GAME_DBUILD_MDL_STATE_REG_FLUSH_WAIT;
}

// ==========================================================================
// GmGameDBuildCheckFlushModel
/*!
 *	モデル片付け登録 及び 終了チェック
 *
 *	@return	TRUE : 登録されたモデルの片付けを終了
 *
 *	@note
 *		GmGameDBuildRegFlushModelで登録されたモデルの開放命令発行と\n
 *		終了チェックを行います。\n
 *		開放が終了するまで毎フレーム呼び出す必要があります。
 */
// ==========================================================================
BOOL GmGameDBuildCheckFlushModel(void)
{
	s32						i, j;
	GMS_GDBUILD_BUILD_MDL_WORK	*build_work;
	OBS_ACTION3D_NN_WORK	*obj_3d;
	BOOL					b_flush_end = TRUE;

	if (gm_obj_build_model_work_reg_num) {
		for (i = gm_obj_build_model_work_reg_num - 1, build_work = &gm_obj_build_model_work_buf[i];
					i >= 0; i--, build_work--) {

			if (build_work->build_state == GME_GAME_DBUILD_MDL_STATE_REG_FLUSH_WAIT) {
#if 1
				j = build_work->reg_num;	// 現在の解放状況
				obj_3d = build_work->obj_3d_list + j;

				while ((GsMainSysGetDisplayListRegistNum() <= AMD_REGISTLIST_NUM - 4/*最低必要数*/ - 64/*余裕*/) &&
							j < build_work->num) {
					// 開放登録
					ObjAction3dNNModelRelease(obj_3d);

					j++;
					obj_3d++;
				}
				
				if (j == build_work->reg_num) {
					// 登録バッファが空くまで待つ
					return (FALSE);
				}

				// 登録状況保存
				build_work->reg_num = j;

				// フラッシュ待機移行チェック
				if (build_work->reg_num == build_work->num) {
					build_work->build_state = GME_GAME_DBUILD_MDL_STATE_FLUSH_WAIT;
				}
				// 登録中
				b_flush_end = FALSE;
#else
				// 登録待機中
				if (GsMainSysGetDisplayListRegistNum() >= AMD_REGISTLIST_NUM - build_work->num*3 - 64/*余裕*/) {
					// 登録バッファが空くまで待つ
					return (FALSE);
				}

				// 開放登録
				for (j = 0, obj_3d = build_work->obj_3d_list; j < build_work->num; j++, obj_3d++) {
					ObjAction3dNNModelRelease(obj_3d);
				}
				build_work->build_state = GME_GAME_DBUILD_MDL_STATE_FLUSH_WAIT;

				b_flush_end = FALSE;
#endif
			}
			else if (build_work->build_state == GME_GAME_DBUILD_MDL_STATE_FLUSH_WAIT) {
				// 開放終了待機中
				for (j = 0, obj_3d = build_work->obj_3d_list; j < build_work->num; j++, obj_3d++) {
					if (ObjAction3dNNModelReleaseCheck(obj_3d) == FALSE) {
						// ビルド待機中 登録待ちへ渡す
						b_flush_end = FALSE;
						break;
					}
				}

				// 開放終了
				if (j >= build_work->num && i == gm_obj_build_model_work_reg_num - 1) {
					// 描画ワークを開放
					amMemFree(build_work->obj_3d_list);

					// チェックをひとつ減らす
					gm_obj_build_model_work_reg_num--;
					MTM_ASSERT(gm_obj_build_model_work_reg_num >= 0);
				}
			}
#if defined (MTD_DEBUG)
			else {
				MTM_ASSERT(!"gmObj.cpp::GmGameDBuildCheckFlushModel() error build state\n");
			}
#endif
		}
	}

	return (b_flush_end);
}


// ==========================================================================
// データリビルド
// ==========================================================================
// ==========================================================================
// GmGameDatFlashRestart
/*!
 *	ゲームデータ片付け リスタート用
 *
 *	@note
 *		ビルド済みのデータをフラッシュします。
 */
// ==========================================================================
void GmGameDatFlashRestart(void)
{
	// イベントデータ
	GmEventDataFlush();

	// MAP地形データ
	// リスタート時のフラッシュ不要
}

// ==========================================================================
// GmGameDatReBuildRestart
/*!
 *	ゲームデータ構築 リスタート用
 *
 *	@note
 *		ビルド済みのデータを再構築します。
 */
// ==========================================================================
void GmGameDatReBuildRestart(void)
{
	// イベントデータ
	GmEventDataBuild();
	
	// MAP地形データ
	GmMapBuildColData();

	// ギミック
	GmGmkSwitchReBuild();		// ステータス情報クリア
	GmGmkPressPillarClear();	// ステータス情報クリア
}

// ==========================================================================
// GmGameDatReBuildRestartCheck
/*!
 *	ゲームデータ構築 リスタート用 再構築終了チェック
 *
 *	@return	TRUE : 終了
 *
 *	@note
 *		データ再構築が終了済みかをチェックします。
 */
// ==========================================================================
BOOL GmGameDatReBuildRestartCheck(void)
{
	// 再構築終了チェック

	return (TRUE);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// ステージ別ビルド処理
// ==========================================================================
// Zone1
void gmGameDatBuildStage11(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// エネミー
	GmEneMotoraBuild();		// モトラ

	GmEneGabuBuild();		// ガブッチョ
	GmEneStingBuild();		// スティンガー
	GmEneMereonBuild();		// メレオン
#endif

#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandBuild();		// 浮島

#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkTarzanRopeBuild();	// ターザンロープ
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBridgeBuild();		// 丸太橋
	GmGmkBreakLandBuild();	// 崩壊足場
	GmGmkBreakWallBuild();	// 破壊可能壁
	GmGmkBreakObjBuild();	// 破壊可能オブジェ
#endif

#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクト
	GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_1);	// エネミー専用
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_1);	// ゾーン専用
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
}
void gmGameDatBuildStage12(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// エネミー
	GmEneMotoraBuild();		// モトラ

	GmEneGabuBuild();		// ガブッチョ
	GmEneStingBuild();		// スティンガー
	GmEneMereonBuild();		// メレオン
#endif //!defined GMD_DEBUG_NO_CREATE_ENEMY
	
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkTarzanRopeBuild();	// ターザンロープ
	GmGmkBridgeBuild();		// 丸太橋
	GmGmkBreakLandBuild();	// 崩壊足場
	GmGmkBreakWallBuild();	// 破壊可能壁
	GmGmkBreakObjBuild();	// 破壊可能オブジェ
#endif
	
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクト
	GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_1);	// エネミー専用
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_1);	// ゾーン専用
#endif //!defined(GMD_DEBUG_NO_CREATE_EFFECT)
}
void gmGameDatBuildStage13(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// エネミー
	GmEneMotoraBuild();		// モトラ

	GmEneGabuBuild();		// ガブッチョ
	GmEneStingBuild();		// スティンガー
	GmEneMereonBuild();		// メレオン
#endif //!defined GMD_DEBUG_NO_CREATE_ENEMY
	
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkPulleyBuild();		// 滑車
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkTarzanRopeBuild();	// ターザンロープ
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBridgeBuild();		// 丸太橋
	GmGmkBreakLandBuild();	// 崩壊足場
	GmGmkBreakWallBuild();	// 破壊可能壁
	GmGmkBreakObjBuild();	// 破壊可能オブジェ
#endif
	
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクト
	GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_1);	// エネミー専用
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_1);	// ゾーン専用
#endif //!defined(GMD_DEBUG_NO_CREATE_EFFECT)
}
void gmGameDatBuildStage1Boss(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss1Build();			// ボス１
	
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkCapsuleBuild();	// カプセル
#endif //!defined GMD_DEBUG_NO_CREATE_GIMMICK
	
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクト
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_1);	// ゾーン専用
	GmEfctBossCmnBuildDataInit();	// ボス共通
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
#endif // !defined(SONIC4_TRIAL)
}

// Zone2
void gmGameDatBuildStage21(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// エネミー
	GmEneGardonBuild();		// ガードン
	GmEneHaroBuild();		// ハロゲン
#endif //!defined GMD_DEBUG_NO_CREATE_ENEMY
	
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkBumperBuild();		//バンパー
	GmGmkEnBmprBuild();		//３耐バンパー
	GmGmkBobbinBuild();		//ボビン
	GmGmkFlipperBuild();	//フリッパー
	GmGmkStopperBuild();	// ストッパー(ゾーン２)
	GmGmkSlotBuild();		// スロット(ゾーン２)
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCannonBuild();		// 大砲(ゾーン２)
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSpCtpltBuild();	// スプリングカタパルト(ゾーン２)
	
	GmGmkBreakWallBuild();	// 破壊可能壁
	GmGmkBreakObjBuild();	// 破壊可能オブジェ
#endif //!defined GMD_DEBUG_NO_CREATE_GIMMICK
	
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクト
	GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_2);	// エネミー専用
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_2);	// ゾーン専用
#endif //!defined(GMD_DEBUG_NO_CREATE_EFFECT)
}
void gmGameDatBuildStage22(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	// エネミー
	GmEneGardonBuild();		// ガードン
	GmEneHaroBuild();		// ハロゲン
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
#endif //!defined GMD_DEBUG_NO_CREATE_ENEMY
	
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkLandBuild();		// 浮島
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBumperBuild();		//バンパー
	GmGmkEnBmprBuild();		//３耐バンパー
	GmGmkBobbinBuild();		//ボビン
	GmGmkFlipperBuild();	//フリッパー
	GmGmkStopperBuild();	// ストッパー(ゾーン２)
	GmGmkSlotBuild();		// スロット(ゾーン２)
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCannonBuild();		// 大砲(ゾーン２)
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSpCtpltBuild();	// スプリングカタパルト(ゾーン２)
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBreakWallBuild();	// 破壊可能壁
	GmGmkBreakObjBuild();	// 破壊可能オブジェ
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSsArrowBuild();	// SpecialStage 矢印
#endif //!defined GMD_DEBUG_NO_CREATE_GIMMICK
	
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクト
	GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_2);	// エネミー専用
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_2);	// ゾーン専用
#endif //!defined(GMD_DEBUG_NO_CREATE_EFFECT)

	// ゲーム開始メッセージ
	GmStartMsgBuild();
}
void gmGameDatBuildStage23(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// エネミー
	GmEneGardonBuild();		// ガードン
	GmEneHaroBuild();		// ハロゲン
#endif //!defined GMD_DEBUG_NO_CREATE_ENEMY
	
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkBumperBuild();		//バンパー
	GmGmkEnBmprBuild();		//３耐バンパー
	GmGmkBobbinBuild();		//ボビン
	GmGmkFlipperBuild();	//フリッパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkStopperBuild();	// ストッパー(ゾーン２)
	GmGmkSlotBuild();		// スロット(ゾーン２)
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCannonBuild();		// 大砲(ゾーン２)
	GmGmkSpCtpltBuild();	// スプリングカタパルト(ゾーン２)
	
	GmGmkBreakWallBuild();	// 破壊可能壁
	GmGmkBreakObjBuild();	// 破壊可能オブジェ
#if _IPHONE
	GmGmkSsArrowBuild();	// SpecialStage 矢印
#endif // _IPHONE
#endif //!defined GMD_DEBUG_NO_CREATE_GIMMICK
	
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクト
	GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_2);	// エネミー専用
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_2);	// ゾーン専用
#endif //!defined(GMD_DEBUG_NO_CREATE_EFFECT)
}
void gmGameDatBuildStage2Boss(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss2Build();			// ボス2
	// ギミック
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkLandBuild();		// 浮島
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCapsuleBuild();	// カプセル
	GmGmkBumperBuild();		//バンパー
	GmGmkEnBmprBuild();		//３耐バンパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBobbinBuild();		//ボビン
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkFlipperBuild();	//フリッパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkStopperBuild();	// ストッパー(ゾーン２)
	GmGmkSlotBuild();		// スロット(ゾーン２)
	GmGmkCannonBuild();		// 大砲(ゾーン２)
	GmGmkSpCtpltBuild();	// スプリングカタパルト(ゾーン２)
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkShutterBuild();	// シャッター
	GmGmkNeedleNeonBuild();	// ネオン針
	
	// エフェクト
	GmEfctBossCmnBuildDataInit();	// ボス共通
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_2);	// ゾーン専用
#endif // !defined(SONIC4_TRIAL)
}

// Zone3
void gmGameDatBuildStage31(void)
{
	// エネミー
	GmEneMoguBuild();			// モグリン
	GmEneUnidesBuild();			// ウニデス
	GmEneUniuniBuild();			// ウニウニ
	GmEneBukuBuild();			// ブクブク

	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkRockBuild();			//大岩
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkWaterSliderBuild();	//ウォータースライダー
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSpearBuild();			// 槍(ゾーン3)
	GmGmkBreakWallBuild();	// 破壊可能壁
	GmGmkBreakObjBuild();	// 破壊可能オブジェ
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkDrainTankBuild();	//排液装置
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	//GmGmkTruckBuild();		// トロッコ
	GmGmkRockRideBuild();		//大岩（傾斜）
	GmGmkSwitchBuildTypeZone3();// スイッチ
	GmGmkSwWallBuild();			// スイッチ壁
	
	// エフェクト
	GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_3);	// エネミー専用
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_3);
}
void gmGameDatBuildStage32(void)
{
	// エネミー
	GmEneMoguBuild();			// モグリン
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmEneUnidesBuild();			// ウニデス
	GmEneUniuniBuild();			// ウニウニ
	GmEneBukuBuild();			// ブクブク
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA

	// ギミック
	GmGmkLandBuild();		// 浮島
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkRockBuild();	//大岩
	GmGmkWaterSliderBuild();	//ウォータースライダー
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSpearBuild();			// 槍(ゾーン3)
	GmGmkBreakWallBuild();	// 破壊可能壁
	GmGmkBreakObjBuild();	// 破壊可能オブジェ
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkDrainTankBuild();	//排液装置
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkTruckBuild();		// トロッコ
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkRockRideBuild();		//大岩（傾斜）
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSwitchBuildTypeZone3();// スイッチ
	GmGmkSwWallBuild();			// スイッチ壁
	GmGmkDSignBuild();			// 危険告知看板
	
	// エフェクト
	GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_3);	// エネミー専用
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_3);

	// ゲーム開始メッセージ
	GmStartMsgBuild();
}
void gmGameDatBuildStage33(void)
{
	// エネミー
	GmEneMoguBuild();			// モグリン
	GmEneUnidesBuild();			// ウニデス
	GmEneUniuniBuild();			// ウニウニ
	GmEneBukuBuild();			// ブクブク

	// ギミック
	GmGmkLandBuild();		// 浮島
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkRockBuild();	//大岩
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkWaterSliderBuild();	//ウォータースライダー
	GmGmkSpearBuild();			// 槍(ゾーン3)
	GmGmkPressWallBuild();		// 迫る壁(ゾーン３)
	GmGmkBreakWallBuild();	// 破壊可能壁
	GmGmkBreakObjBuild();	// 破壊可能オブジェ
	GmGmkDrainTankBuild();	//排液装置
	//GmGmkTruckBuild();		// トロッコ
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkRockRideBuild();		//大岩（傾斜）
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSwitchBuildTypeZone3();// スイッチ
	GmGmkSwWallBuild();			// スイッチ壁
	
	// エフェクト
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_3);	// エネミー専用
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_3);
#endif
}
void gmGameDatBuildStage3Boss(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss3Build();			// ボス3
	GmEneMoguBuild();			// モグリン
	GmEneUnidesBuild();			// ウニデス
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkCapsuleBuild();		// カプセル
	GmGmkSpearBuild();			// 槍(ゾーン3)
	GmGmkBoss3PillarBuild();	// ボス3用柱

	// エフェクト
	GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_3);	// エネミー専用
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_3);
	GmEfctBossCmnBuildDataInit();	// ボス共通
#endif // !defined(SONIC4_TRIAL)
}

// Zone4
void gmGameDatBuildStage41(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// エネミー
	GmEneTStarBuild();		// テルスター
	GmEneKaniBuild();		// かにパンチ
	GmEneKamaBuild();		// カマキラー
#endif //!defined GMD_DEBUG_NO_CREATE_ENEMY
	
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkPistonBuild();			// ピストン(ゾーン４)
	GmGmkBeltConveyorBuild();	// ベルトコンベヤー(ゾーン４)
	GmGmkUpBumperBuild();		// 登るバンパー(ゾーン４)
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSeesawBuild();			// シーソー(ゾーン４)
	GmGmkSpearBuild();			// 槍(ゾーン４用がまだなので３で代用)
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSteamPipeBuild();		// スチームパイプ(ゾーン４)
	GmGmkPopSteamBuild();		// ポップスチーム(ゾーン４)

	GmGmkBreakWallBuild();	// 破壊可能壁
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBreakObjBuild();	// 破壊可能オブジェ
	GmGmkGearBuild();			// 歯車
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSwitchBuildTypeZone4();// スイッチ
	GmGmkSwWallBuild();			// スイッチ壁
#endif //!defined GMD_DEBUG_NO_CREATE_GIMMICK
	
	// エフェクト
	GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_4);	// エネミー専用
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_4);	// ゾーン専用

}
void gmGameDatBuildStage42(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// エネミー
	GmEneTStarBuild();		// テルスター
	GmEneKaniBuild();		// かにパンチ
	GmEneKamaBuild();		// カマキラー
#endif //!defined GMD_DEBUG_NO_CREATE_ENEMY
	
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkPistonBuild();			// ピストン(ゾーン４)
	GmGmkBeltConveyorBuild();	// ベルトコンベヤー(ゾーン４)
	GmGmkUpBumperBuild();		// 登るバンパー(ゾーン４)
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSeesawBuild();			// シーソー(ゾーン４)
	GmGmkSpearBuild();			// 槍(ゾーン４用がまだなので３で代用)
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSteamPipeBuild();		// スチームパイプ(ゾーン４)
	GmGmkPopSteamBuild();		// ポップスチーム(ゾーン４)

	GmGmkBreakWallBuild();	// 破壊可能壁
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBreakObjBuild();	// 破壊可能オブジェ
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkGearBuild();			// 歯車
	GmGmkSwitchBuildTypeZone4();// スイッチ
	GmGmkSwWallBuild();			// スイッチ壁
#endif //!defined GMD_DEBUG_NO_CREATE_GIMMICK
	
	// エフェクト
	GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_4);	// エネミー専用
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_4);	// ゾーン専用
}
void gmGameDatBuildStage43(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// エネミー
	GmEneTStarBuild();		// テルスター
	GmEneKaniBuild();		// かにパンチ
	GmEneKamaBuild();		// カマキラー
#endif //!defined GMD_DEBUG_NO_CREATE_ENEMY
	
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkPistonBuild();			// ピストン(ゾーン４)
	GmGmkBeltConveyorBuild();	// ベルトコンベヤー(ゾーン４)
	GmGmkUpBumperBuild();		// 登るバンパー(ゾーン４)
	GmGmkSeesawBuild();			// シーソー(ゾーン４)
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSpearBuild();			// 槍(ゾーン４用がまだなので３で代用)
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSteamPipeBuild();		// スチームパイプ(ゾーン４)
	GmGmkPopSteamBuild();		// ポップスチーム(ゾーン４)
	GmGmkPressWallBuild();		// 迫る壁(ゾーン４)
	GmGmkPressPillarBuild();	// 迫り出す柱

	GmGmkBreakWallBuild();	// 破壊可能壁
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBreakObjBuild();	// 破壊可能オブジェ
#endif //!GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkGearBuild();			// 歯車
	GmGmkSwitchBuildTypeZone4();// スイッチ
	GmGmkSwWallBuild();			// スイッチ壁
#endif //!defined GMD_DEBUG_NO_CREATE_GIMMICK
	
	// エフェクト
	GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_4);	// エネミー専用
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_4);	// ゾーン専用
}
void gmGameDatBuildStage4Boss(void)
{
#if !defined(SONIC4_TRIAL)
/*
	// エネミー	
	// ギミック
	GmGmkCapsuleBuild();	// カプセル
	
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkCapsuleBuild();		// カプセル
	GmGmkPistonBuild();			// ピストン(ゾーン４)
	GmGmkBeltConveyorBuild();	// ベルトコンベヤー(ゾーン４)
	GmGmkUpBumperBuild();		// 登るバンパー(ゾーン４)
	GmGmkSeesawBuild();			// シーソー(ゾーン４)
	GmGmkSpearBuild();			// 槍(ゾーン４用がまだなので３で代用)
	GmGmkSteamPipeBuild();		// スチームパイプ(ゾーン４)
	GmGmkPopSteamBuild();		// ポップスチーム(ゾーン４)

	GmGmkGearBuild();			// 歯車
	
	// エフェクト
	GmEfctBossCmnBuildDataInit();	// ボス共通
*/
//sfr
// エネミー
GmBoss4Build();			// ボス4

// ギミック
GmGmkLandBuild();		// 浮島
GmGmkCapsuleBuild();	// カプセル

// エフェクト
//GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_4);	// エネミー専用
//GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_4);	// ゾーン専用

GmEfctBossCmnBuildDataInit();	// ボス共通
#endif // !defined(SONIC4_TRIAL)
}

// ZoneFinal
void gmGameDatBuildStageFinalBoss01(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
//	GmBoss5Build();			// ボスファイナル  （仮）
	// ギミック
	GmGmkLandBuild();		// 浮島
#if 1	// 仮設定
	GmGmkSteamPipeBuild();		// スチームパイプ
	GmGmkBumperBuild();			// バンパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBobbinBuild();			// ボビン
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkFlipperBuild();		// フリッパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCapsuleBuild();		// カプセル
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkShutterBuild();		// シャッター
	GmGmkBoss3PillarBuild();	// ボス3用柱
	GmGmkNeedleNeonBuild();	// ネオン針
#endif /* 1 */
	// エフェクト
	GmEfctBossCmnBuildDataInit();	// ボス共通
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_FINAL);	// ゾーン専用
#endif // !defined(SONIC4_TRIAL)
}
void gmGameDatBuildStageFinalBoss02(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkSteamPipeBuild();		// スチームパイプ
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCapsuleBuild();		// カプセル
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBumperBuild();			// バンパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBobbinBuild();			// ボビン
#endif /// !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkFlipperBuild();		// フリッパー
	GmGmkShutterBuild();	// シャッター
	GmGmkBoss3PillarBuild();	// ボス3用柱
	GmGmkNeedleNeonBuild();	// ネオン針
	
	// エフェクト
	GmEfctBossCmnBuildDataInit();	// ボス共通
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_FINAL);	// ゾーン専用
#endif // !defined(SONIC4_TRIAL)
}
void gmGameDatBuildStageFinalBoss03(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkSteamPipeBuild();		// スチームパイプ
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCapsuleBuild();		// カプセル
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBumperBuild();			// バンパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBobbinBuild();			// ボビン
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkFlipperBuild();		// フリッパー
	GmGmkShutterBuild();	// シャッター
	GmGmkBoss3PillarBuild();	// ボス3用柱
	GmGmkNeedleNeonBuild();	// ネオン針

	// エフェクト
	GmEfctBossCmnBuildDataInit();	// ボス共通
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_FINAL);	// ゾーン専用
#endif // !defined(SONIC4_TRIAL)
}
void gmGameDatBuildStageFinalBoss04(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	// ギミック
	GmGmkLandBuild();		// 浮島
	
	// エフェクト
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_FINAL);	// ゾーン専用
#endif // !defined(SONIC4_TRIAL)
}
void gmGameDatBuildStageFinalBoss05(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss5Build();			// ボスファイナル
	
	// ギミック
	GmGmkLandBuild();		// 浮島
#if 1	// 仮設定
	GmGmkSteamPipeBuild();		// スチームパイプ
	GmGmkBumperBuild();			// バンパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBobbinBuild();			// ボビン
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkFlipperBuild();		// フリッパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCapsuleBuild();		// カプセル
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkShutterBuild();	// シャッター
	GmGmkBoss3PillarBuild();	// ボス3用柱
	GmGmkNeedleNeonBuild();	// ネオン針
#endif /* 1 */
	
	// エフェクト
	GmEfctBossCmnBuildDataInit();	// ボス共通
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_FINAL);	// ゾーン専用
#endif // !defined(SONIC4_TRIAL)
}

void gmGameDatBuildSS01(void)
{
	// ギミック
	GmGmkSsSquareBuild();	// SpecialStage 四角柱
	GmGmkSsCircleBuild();	// SpecialStage 丸柱
	GmGmkSsEnduranceBuild();// SpecialStage 耐久柱
	GmGmkSsGoalBuild();		// SpecialStage ゴール
	GmGmkSsEmeraldBuild();	// SpecialStage カオスエメラルド
	GmGmkSsTimeBuild();		// SpecialStage 時間パネル
	GmGmkSsRingGateBuild();	// SpecialStage リングゲート
	GmGmkSsArrowBuild();	// SpecialStage 矢印
	GmGmkSsOblongBuild();	// SpecialStage RingGate終端柱
	GmGmkBobbinBuild();		//ボビン
	
	// エフェクト
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_SS);	// ゾーン専用

#if _IPHONE
	// ゲーム開始メッセージ
	// どのステージでもメッセージが表示可能にする
	GmStartMsgBuild();
#else
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_SS1) {
		// ゲーム開始メッセージ
		GmStartMsgBuild();
	}
#endif // _IPHONE
}

// Ending
void gmGameDatBuildEnding(void)
{
#if !defined(SONIC4_TRIAL)
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandBuild();		// 浮島
#endif

#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクト
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_1);	// ゾーン専用
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */

	GmEndingBuild();		// エンディング専用OBJ
	
	DmStfrlMdlCtrlRingBuild();
	// エネミー
	DmStfrlMdlCtrlBoss1Build();	// ボス１
	
	DmStfrlMdlCtrlSonicBuild();	// ソニック
#endif // !defined(SONIC4_TRIAL)
}


// ==========================================================================
// ステージ別フラッシュ処理
// ==========================================================================
// Zone1
void gmGameDatFlushStage11(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// エネミー
	GmEneMotoraFlush();		// モトラ

	GmEneGabuFlush();		// ガブッチョ
	GmEneStingFlush();		// スティンガー
	GmEneMereonFlush();		// メレオン
#endif

#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandFlush();		// 浮島
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkTarzanRopeFlush();	// ターザンロープ
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBridgeFlush();		// 丸太橋
	GmGmkBreakObjFlush();	// 破壊可能オブジェ
	GmGmkBreakWallFlush();	// 破壊可能壁
	GmGmkBreakLandFlush();	// 崩壊足場
#endif
	
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクト
	GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_1);	// エネミー専用
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_1);	// ゾーン専用
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
}
void gmGameDatFlushStage12(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// エネミー
	GmEneMotoraFlush();		// モトラ

	GmEneGabuFlush();		// ガブッチョ
	GmEneStingFlush();		// スティンガー
	GmEneMereonFlush();		// メレオン
#endif

#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkTarzanRopeFlush();	// ターザンロープ
	GmGmkBridgeFlush();		// 丸太橋
	GmGmkBreakObjFlush();	// 破壊可能オブジェ
	GmGmkBreakWallFlush();	// 破壊可能壁
	GmGmkBreakLandFlush();	// 崩壊足場
#endif
	
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクト
	GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_1);	// エネミー専用
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_1);	// ゾーン専用
#endif
}
void gmGameDatFlushStage13(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// エネミー
	GmEneMotoraFlush();		// モトラ

	GmEneGabuFlush();		// ガブッチョ
	GmEneStingFlush();		// スティンガー
	GmEneMereonFlush();		// メレオン
#endif

#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkPulleyFlush();		// 滑車
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkTarzanRopeFlush();	// ターザンロープ
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBridgeFlush();		// 丸太橋
	GmGmkBreakObjFlush();	// 破壊可能オブジェ
	GmGmkBreakWallFlush();	// 破壊可能壁
	GmGmkBreakLandFlush();	// 崩壊足場
#endif
	
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクト
	GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_1);	// エネミー専用
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_1);	// ゾーン専用
#endif
}
void gmGameDatFlushStage1Boss(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss1Flush();			// ボス１
	
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkCapsuleFlush();	// カプセル
#endif
	
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクト
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_1);	// ゾーン専用
	GmEfctBossCmnFlushDataInit();	// ボス共通
#endif
#endif // !defined(SONIC4_TRIAL)
}

// Zone2
void gmGameDatFlushStage21(void)
{
	// エネミー
	GmEneGardonFlush();		// ガードン
	GmEneHaroFlush();		// ハロゲン

	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkBumperFlush();		//バンパー
	GmGmkEnBmprFlush();		//３耐バンパー
	GmGmkBobbinFlush();		//ボビン
	GmGmkFlipperFlush();	//フリッパー
	GmGmkStopperFlush();	// ストッパー(ゾーン２)
	GmGmkSlotFlush();		// スロット(ゾーン２)
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCannonFlush();		// 大砲(ゾーン２)
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSpCtpltFlush();	// スプリングカタパルト(ゾーン２)	
	GmGmkBreakObjFlush();	// 破壊可能オブジェ
	GmGmkBreakWallFlush();	// 破壊可能壁
	
	// エフェクト
	GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_2);	// エネミー専用
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_2);	// ゾーン専用
}
void gmGameDatFlushStage22(void)
{
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	// エネミー
	GmEneGardonFlush();		// ガードン
	GmEneHaroFlush();		// ハロゲン
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA

	// ギミック
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkLandFlush();		// 浮島
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBumperFlush();		//バンパー
	GmGmkEnBmprFlush();		//３耐バンパー
	GmGmkBobbinFlush();		//ボビン
	GmGmkFlipperFlush();	//フリッパー
	GmGmkStopperFlush();	// ストッパー(ゾーン２)
	GmGmkSlotFlush();		// スロット(ゾーン２)
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCannonFlush();		// 大砲(ゾーン２)
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSpCtpltFlush();	// スプリングカタパルト(ゾーン２)
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBreakObjFlush();	// 破壊可能オブジェ
	GmGmkBreakWallFlush();	// 破壊可能壁
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSsArrowFlush();	// SpecialStage 矢印
	
	// エフェクト
	GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_2);	// エネミー専用
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_2);	// ゾーン専用

	// ゲーム開始メッセージ
	GmStartMsgFlush();
}
void gmGameDatFlushStage23(void)
{
	// エネミー
	GmEneGardonFlush();		// ガードン
	GmEneHaroFlush();		// ハロゲン

	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkBumperFlush();		//バンパー
	GmGmkEnBmprFlush();		//３耐バンパー
	GmGmkBobbinFlush();		//ボビン
	GmGmkFlipperFlush();	//フリッパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkStopperFlush();	// ストッパー(ゾーン２)
	GmGmkSlotFlush();		// スロット(ゾーン２)
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCannonFlush();		// 大砲(ゾーン２)
	GmGmkSpCtpltFlush();	// スプリングカタパルト(ゾーン２)	
	GmGmkBreakObjFlush();	// 破壊可能オブジェ
	GmGmkBreakWallFlush();	// 破壊可能壁
#if _IPHONE
	GmGmkSsArrowFlush();	// SpecialStage 矢印
#endif // _IPHONE
	
	// エフェクト
	GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_2);	// エネミー専用
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_2);	// ゾーン専用
}
void gmGameDatFlushStage2Boss(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss2Flush();			// ボス2
	// ギミック
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkLandFlush();		// 浮島
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCapsuleFlush();	// カプセル
	GmGmkBumperFlush();		//バンパー
	GmGmkEnBmprFlush();		//３耐バンパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBobbinFlush();		//ボビン
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkFlipperFlush();	//フリッパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkStopperFlush();	// ストッパー(ゾーン２)
	GmGmkSlotFlush();		// スロット(ゾーン２)
	GmGmkCannonFlush();		// 大砲(ゾーン２)
	GmGmkSpCtpltFlush();	// スプリングカタパルト(ゾーン２)	
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkShutterFlush();	// シャッター
	GmGmkNeedleNeonFlush();	// ネオン針
	
	// エフェクト
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_2);	// ゾーン専用
	GmEfctBossCmnFlushDataInit();	// ボス共通
#endif // !defined(SONIC4_TRIAL)
}

// Zone3
void gmGameDatFlushStage31(void)
{
	// エネミー
	GmEneMoguFlush();			// モグリン
	GmEneUnidesFlush();			// ウニデス
	GmEneUniuniFlush();			// ウニウニ
	GmEneBukuFlush();			// ブクブク

	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkRockFlush();	//大岩
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkWaterSliderFlush();	//ウォータースライダー
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSpearFlush();			// 槍(ゾーン3)
	GmGmkBreakObjFlush();	// 破壊可能オブジェ
	GmGmkBreakWallFlush();	// 破壊可能壁
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkDrainTankFlush();	//排液装置
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	//GmGmkTruckFlush();		// トロッコ
	GmGmkRockRideFlush();	//大岩（傾斜）
	GmGmkSwitchFlush();		// スイッチ
	GmGmkSwWallFlush();		// スイッチ壁
	
	// エフェクト
	GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_3);	// エネミー専用
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_3);
}
void gmGameDatFlushStage32(void)
{
	// エネミー
	GmEneMoguFlush();			// モグリン
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmEneUnidesFlush();			// ウニデス
	GmEneUniuniFlush();			// ウニウニ
	GmEneBukuFlush();			// ブクブク
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA

	// ギミック
	GmGmkLandFlush();		// 浮島
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkRockFlush();	//大岩
	GmGmkWaterSliderFlush();	//ウォータースライダー
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSpearFlush();			// 槍(ゾーン3)
	GmGmkBreakObjFlush();	// 破壊可能オブジェ
	GmGmkBreakWallFlush();	// 破壊可能壁
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkDrainTankFlush();	//排液装置
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkTruckFlush();		// トロッコ
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkRockRideFlush();	//大岩（傾斜）
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSwitchFlush();		// スイッチ
	GmGmkSwWallFlush();		// スイッチ壁
	GmGmkDSignFlush();		// 危険告知看板
	
	// エフェクト
	GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_3);	// エネミー専用
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_3);

	// ゲーム開始メッセージ
	GmStartMsgFlush();
}
void gmGameDatFlushStage33(void)
{
	// エネミー
	GmEneMoguFlush();			// モグリン
	GmEneUnidesFlush();			// ウニデス
	GmEneUniuniFlush();			// ウニウニ
	GmEneBukuFlush();			// ブクブク

	// ギミック
	GmGmkLandFlush();		// 浮島
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkRockFlush();	//大岩
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkWaterSliderFlush();	//ウォータースライダー
	GmGmkSpearFlush();			// 槍(ゾーン3)
	GmGmkPressWallFlush();		// 迫る壁(ゾーン３)
	GmGmkBreakObjFlush();	// 破壊可能オブジェ
	GmGmkBreakWallFlush();	// 破壊可能壁
	GmGmkDrainTankFlush();	//排液装置
	//GmGmkTruckFlush();		// トロッコ
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkRockRideFlush();	//大岩（傾斜）
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSwitchFlush();		// スイッチ
	GmGmkSwWallFlush();		// スイッチ壁
	
	// エフェクト
	GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_3);	// エネミー専用
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_3);
}
void gmGameDatFlushStage3Boss(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss3Flush();			// ボス3
	GmEneMoguFlush();			// モグリン
	GmEneUnidesFlush();			// ウニデス
	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkCapsuleFlush();		// カプセル
	GmGmkSpearFlush();			// 槍(ゾーン3)
	GmGmkBoss3PillarFlush();	// ボス3用柱

	// エフェクト
	GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_3);	// エネミー専用
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_3);	// ゾーン専用
	GmEfctBossCmnFlushDataInit();	// ボス共通
#endif // !defined(SONIC4_TRIAL)
}

// Zone4
void gmGameDatFlushStage41(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// エネミー
	GmEneTStarFlush();			// テルスター
	GmEneKaniFlush();			// かにパンチ
	GmEneKamaFlush();			// カマキラー
#endif //!defined GMD_DEBUG_NO_CREATE_ENEMY
	
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkPistonFlush();			// ピストン(ゾーン４)
	GmGmkBeltConveyorFlush();	// ベルトコンベヤー(ゾーン４)
	GmGmkUpBumperFlush();		// 登るバンパー(ゾーン４)
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSeesawFlush();			// シーソー(ゾーン４)
	GmGmkSpearFlush();			// 槍(ゾーン４)
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSteamPipeFlush();		// スチームパイプ(ゾーン４)
	GmGmkPopSteamFlush();		// ポップスチーム(ゾーン４)

#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBreakObjFlush();	// 破壊可能オブジェ
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBreakWallFlush();	// 破壊可能壁
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkGearFlush();			// 歯車
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSwitchFlush();			// スイッチ
	GmGmkSwWallFlush();			// スイッチ壁
#endif //!defined GMD_DEBUG_NO_CREATE_GIMMICK
	
	// エフェクト
	GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_4);	// エネミー専用
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_4);	// ゾーン専用
}
void gmGameDatFlushStage42(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// エネミー
	GmEneTStarFlush();			// テルスター
	GmEneKaniFlush();			// かにパンチ
	GmEneKamaFlush();			// カマキラー
#endif //!defined GMD_DEBUG_NO_CREATE_ENEMY
	
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkPistonFlush();			// ピストン(ゾーン４)
	GmGmkBeltConveyorFlush();	// ベルトコンベヤー(ゾーン４)
	GmGmkUpBumperFlush();		// 登るバンパー(ゾーン４)
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSeesawFlush();			// シーソー(ゾーン４)
	GmGmkSpearFlush();			// 槍(ゾーン４)
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSteamPipeFlush();		// スチームパイプ(ゾーン４)
	GmGmkPopSteamFlush();		// ポップスチーム(ゾーン４)

#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBreakObjFlush();	// 破壊可能オブジェ
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBreakWallFlush();	// 破壊可能壁
	GmGmkGearFlush();			// 歯車
	GmGmkSwitchFlush();			// スイッチ
	GmGmkSwWallFlush();			// スイッチ壁
#endif //!defined GMD_DEBUG_NO_CREATE_GIMMICK
	
	// エフェクト
	GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_4);	// エネミー専用
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_4);	// ゾーン専用
}
void gmGameDatFlushStage43(void)
{
#if !defined GMD_DEBUG_NO_CREATE_ENEMY
	// エネミー
	GmEneTStarFlush();			// テルスター
	GmEneKaniFlush();			// かにパンチ
	GmEneKamaFlush();			// カマキラー
#endif //!defined GMD_DEBUG_NO_CREATE_ENEMY
	
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkPistonFlush();			// ピストン(ゾーン４)
	GmGmkBeltConveyorFlush();	// ベルトコンベヤー(ゾーン４)
	GmGmkUpBumperFlush();		// 登るバンパー(ゾーン４)
	GmGmkSeesawFlush();			// シーソー(ゾーン４)
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSpearFlush();			// 槍(ゾーン４)
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkSteamPipeFlush();		// スチームパイプ(ゾーン４)
	GmGmkPopSteamFlush();		// ポップスチーム(ゾーン４)
	GmGmkPressWallFlush();		// 迫る壁(ゾーン４)
	GmGmkPressPillarFlush();	// 迫り出す柱

#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBreakObjFlush();	// 破壊可能オブジェ
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBreakWallFlush();	// 破壊可能壁
	GmGmkGearFlush();			// 歯車
	GmGmkSwitchFlush();			// スイッチ
	GmGmkSwWallFlush();			// スイッチ壁
#endif //!defined GMD_DEBUG_NO_CREATE_GIMMICK
	
	// エフェクト
	GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_4);	// エネミー専用
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_4);	// ゾーン専用
}
void gmGameDatFlushStage4Boss(void)
{
#if !defined(SONIC4_TRIAL)
/*
	// エネミー
	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkCapsuleFlush();		// カプセル
	GmGmkPistonFlush();			// ピストン(ゾーン４)
	GmGmkBeltConveyorFlush();	// ベルトコンベヤー(ゾーン４)
	GmGmkUpBumperFlush();		// 登るバンパー(ゾーン４)
	GmGmkSeesawFlush();			// シーソー(ゾーン４)
	GmGmkSpearFlush();			// 槍(ゾーン４)
	GmGmkSteamPipeFlush();		// スチームパイプ(ゾーン４)
	GmGmkPopSteamFlush();		// ポップスチーム(ゾーン４)

	GmGmkGearFlush();			// 歯車

	// エフェクト
	GmEfctBossCmnFlushDataInit();	// ボス共通
*/
//sfr
// エネミー
GmBoss4Flush();			// ボス１

// ギミック
GmGmkLandFlush();		// 浮島
GmGmkCapsuleFlush();	// カプセル

// エフェクト
//GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_4);	// ゾーン専用
//GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_4);	// エネミー専用
GmEfctBossCmnFlushDataInit();	// ボス共通
#endif // !defined(SONIC4_TRIAL)
}

// ZoneFinal
void gmGameDatFlushStageFinalBoss01(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
//	GmBoss5Flush();				// ボスファイナル
	
	// ギミック
	GmGmkLandFlush();		// 浮島
#if 1	// 仮設定
	GmGmkSteamPipeFlush();		// スチームパイプ(ゾーン４)
	GmGmkBumperFlush();			// バンパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBobbinFlush();			// ボビン
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkFlipperFlush();		// フリッパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCapsuleFlush();		// カプセル
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkShutterFlush();		// シャッター
	GmGmkBoss3PillarFlush();	// ボス3用柱
	GmGmkNeedleNeonFlush();		//ネオン針
#endif /* 1 */
	// エフェクト
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_FINAL);	// ゾーン専用
	GmEfctBossCmnFlushDataInit();	// ボス共通
#endif // !defined(SONIC4_TRIAL)
}
void gmGameDatFlushStageFinalBoss02(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkSteamPipeFlush();		// スチームパイプ(ゾーン４)
	GmGmkBumperFlush();			// バンパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBobbinFlush();			// ボビン
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkFlipperFlush();		// フリッパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCapsuleFlush();		// カプセル
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkShutterFlush();		// シャッター
	GmGmkBoss3PillarFlush();	// ボス3用柱
	GmGmkNeedleNeonFlush();		//ネオン針
	
	// エフェクト
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_FINAL);	// ゾーン専用
	GmEfctBossCmnFlushDataInit();	// ボス共通
#endif // !defined(SONIC4_TRIAL)
}
void gmGameDatFlushStageFinalBoss03(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー	
	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkSteamPipeFlush();		// スチームパイプ(ゾーン４)
	GmGmkBumperFlush();			// バンパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBobbinFlush();			// ボビン
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkFlipperFlush();		// フリッパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCapsuleFlush();		// カプセル
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkShutterFlush();		// シャッター
	GmGmkBoss3PillarFlush();	// ボス3用柱
	GmGmkNeedleNeonFlush();		//ネオン針

	// エフェクト
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_FINAL);	// ゾーン専用
	GmEfctBossCmnFlushDataInit();	// ボス共通
#endif // !defined(SONIC4_TRIAL)
}
void gmGameDatFlushStageFinalBoss04(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	// ギミック
	GmGmkLandFlush();		// 浮島
	
	// エフェクト
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_FINAL);	// ゾーン専用
#endif // !defined(SONIC4_TRIAL)
}
void gmGameDatFlushStageFinalBoss05(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss5Flush();				// ボスファイナル
	
	// ギミック
	GmGmkLandFlush();		// 浮島
#if 1	// 仮設定
	GmGmkSteamPipeFlush();		// スチームパイプ(ゾーン４)
	GmGmkBumperFlush();			// バンパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkBobbinFlush();			// ボビン
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkFlipperFlush();		// フリッパー
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkCapsuleFlush();		// カプセル
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	GmGmkShutterFlush();		// シャッター
	GmGmkBoss3PillarFlush();	// ボス3用柱
	GmGmkNeedleNeonFlush();		//ネオン針
#endif /* 1 */
	
	// エフェクト
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_FINAL);	// ゾーン専用
	GmEfctBossCmnFlushDataInit();	// ボス共通
#endif // !defined(SONIC4_TRIAL)
}

void gmGameDatFlushSS01(void)
{
	// ギミック
	GmGmkSsSquareFlush();	// SpecialStage 四角柱
	GmGmkSsCircleFlush();	// SpecialStage 丸柱
	GmGmkSsEnduranceFlush();// SpecialStage 耐久柱
	GmGmkSsGoalFlush();		// SpecialStage ゴール
	GmGmkSsEmeraldFlush();	// SpecialStage カオスエメラルド
	GmGmkSsTimeFlush();		// SpecialStage 時間パネル
	GmGmkSsRingGateFlush();	// SpecialStage リングゲート
	GmGmkSsArrowFlush();	// SpecialStage 矢印
	GmGmkSsOblongFlush();	// SpecialStage RingGate終端柱
	GmGmkBobbinFlush();		//ボビン
	
	// エフェクト
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_SS);	// ゾーン専用

#if _IPHONE
	// ゲーム開始メッセージ
	// どのステージでもメッセージが表示可能にする
	GmStartMsgFlush();
#else
	// ゲーム開始メッセージ
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_SS1) {
		GmStartMsgFlush();
	}
#endif // _IPHONE
}

// Ending
void gmGameDatFlushEnding(void)
{
#if !defined(SONIC4_TRIAL)
#if !defined GMD_DEBUG_NO_CREATE_GIMMICK
	// ギミック
	GmGmkLandFlush();		// 浮島
#endif
	
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクト
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_1);	// ゾーン専用
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */

	GmEndingFlush();		// エンディング専用OBJ
	
	DmStfrlMdlCtrlSonicFlush();	// スタッフロール用ソニック
	
	// スタッフロール用データ
	DmStfrlMdlCtrlRingFlush();
	
	// エネミー
	DmStfrlMdlCtrlBoss1Flush();	// ボス１
#endif // !defined(SONIC4_TRIAL)
}



// ==========================================================================
// ボス連戦用ビルド処理
// ==========================================================================
void gmGameDatBuildStageF_BB1(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss1Build();			// ボス１
#if 0
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkCapsuleBuild();	// カプセル
	// エフェクト
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_1);	// ゾーン専用
	GmEfctBossCmnBuildDataInit();	// ボス共通
#endif
#endif // !defined(SONIC4_TRIAL)
}
void gmGameDatBuildStageF_BB2(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss2Build();			// ボス2
#endif // !defined(SONIC4_TRIAL)
#if 0
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkCapsuleBuild();	// カプセル
	GmGmkBumperBuild();		//バンパー
	GmGmkEnBmprBuild();		//３耐バンパー
	GmGmkBobbinBuild();		//ボビン
	GmGmkFlipperBuild();	//フリッパー
	GmGmkStopperBuild();	// ストッパー(ゾーン２)
	GmGmkSlotBuild();		// スロット(ゾーン２)
	GmGmkCannonBuild();		// 大砲(ゾーン２)
	GmGmkSpCtpltBuild();	// スプリングカタパルト(ゾーン２)
	GmGmkShutterBuild();	// シャッター
	GmGmkNeedleNeonBuild();	// ネオン針
	// エフェクト
	GmEfctBossCmnBuildDataInit();	// ボス共通
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_2);	// ゾーン専用
#endif
}
void gmGameDatBuildStageF_BB3(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss3Build();			// ボス3
#endif // !defined(SONIC4_TRIAL)
#if 0
	GmEneMoguBuild();			// モグリン
	GmEneUnidesBuild();			// ウニデス
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkCapsuleBuild();		// カプセル
	GmGmkSpearBuild();			// 槍(ゾーン3)
	GmGmkBoss3PillarBuild();	// ボス3用柱
	// エフェクト
	GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_3);	// エネミー専用
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_3);
	GmEfctBossCmnBuildDataInit();	// ボス共通
#endif
}
void gmGameDatBuildStageF_BB4(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss4Build();			// ボス4
#endif // !defined(SONIC4_TRIAL)
#if 0
	// ギミック
	GmGmkLandBuild();		// 浮島
	GmGmkCapsuleBuild();	// カプセル
	// エフェクト
	//GmEfctEneBuildDataInit(GSD_MAIN_ZONE_TYPE_4);	// エネミー専用
	//GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_4);	// ゾーン専用
	GmEfctBossCmnBuildDataInit();	// ボス共通
#endif
}
void gmGameDatBuildStageF_BBF(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss5Build();			// ボスファイナル  （仮）
#endif // !defined(SONIC4_TRIAL)
#if 0
	// ギミック
	GmGmkLandBuild();		// 浮島
#if 1	// 仮設定
	GmGmkSteamPipeBuild();		// スチームパイプ
	GmGmkBumperBuild();			// バンパー
	GmGmkBobbinBuild();			// ボビン
	GmGmkFlipperBuild();		// フリッパー
	GmGmkCapsuleBuild();		// カプセル
#endif /* 1 */
	// エフェクト
	GmEfctBossCmnBuildDataInit();	// ボス共通
	GmEfctZoneBuildDataInit(GSD_MAIN_ZONE_TYPE_FINAL);	// ゾーン専用
#endif
}


// ==========================================================================
// ボス連戦用フラッシュ処理
// ==========================================================================
void gmGameDatFlushStageF_BB1(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss1Flush();			// ボス2
#endif // !defined(SONIC4_TRIAL)
#if 0
	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkCapsuleFlush();	// カプセル
	GmGmkBumperFlush();		//バンパー
	GmGmkEnBmprFlush();		//３耐バンパー
	GmGmkBobbinFlush();		//ボビン
	GmGmkFlipperFlush();	//フリッパー
	GmGmkStopperFlush();	// ストッパー(ゾーン２)
	GmGmkSlotFlush();		// スロット(ゾーン２)
	GmGmkCannonFlush();		// 大砲(ゾーン２)
	GmGmkSpCtpltFlush();	// スプリングカタパルト(ゾーン２)	
	GmGmkShutterFlush();	// シャッター
	GmGmkNeedleNeonFlush();	// ネオン針
	// エフェクト
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_2);	// ゾーン専用
	GmEfctBossCmnFlushDataInit();	// ボス共通
#endif
}
void gmGameDatFlushStageF_BB2(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss2Flush();			// ボス１
#endif // !defined(SONIC4_TRIAL)
#if 0
	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkCapsuleFlush();	// カプセル
	// エフェクト
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_1);	// ゾーン専用
	GmEfctBossCmnFlushDataInit();	// ボス共通
#endif
}
void gmGameDatFlushStageF_BB3(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss3Flush();			// ボス3
#endif // !defined(SONIC4_TRIAL)
//	GmEneMoguFlush();			// モグリン
//	GmEneUnidesFlush();			// ウニデス
#if 0
	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkCapsuleFlush();		// カプセル
	GmGmkSpearFlush();			// 槍(ゾーン3)
	GmGmkBoss3PillarFlush();	// ボス3用柱
	// エフェクト
	GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_3);	// エネミー専用
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_3);	// ゾーン専用
	GmEfctBossCmnFlushDataInit();	// ボス共通
#endif
}
void gmGameDatFlushStageF_BB4(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss4Flush();			// ボス１
#endif // !defined(SONIC4_TRIAL)
#if 0
	// ギミック
	GmGmkLandFlush();		// 浮島
	GmGmkCapsuleFlush();	// カプセル
	// エフェクト
	//GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_4);	// ゾーン専用
	//GmEfctEneFlushDataInit(GSD_MAIN_ZONE_TYPE_4);	// エネミー専用
	GmEfctBossCmnFlushDataInit();	// ボス共通
#endif
}
void gmGameDatFlushStageF_BBF(void)
{
#if !defined(SONIC4_TRIAL)
	// エネミー
	GmBoss5Flush();				// ボスファイナル
#endif // !defined(SONIC4_TRIAL)
#if 0
	// ギミック
	GmGmkLandFlush();		// 浮島
#if 1	// 仮設定
	GmGmkSteamPipeFlush();		// スチームパイプ(ゾーン４)
	GmGmkBumperFlush();			// バンパー
	GmGmkBobbinFlush();			// ボビン
	GmGmkFlipperFlush();		// フリッパー
	GmGmkCapsuleFlush();		// カプセル
#endif /* 1 */
	// エフェクト
	GmEfctZoneFlushDataInit(GSD_MAIN_ZONE_TYPE_FINAL);	// ゾーン専用
	GmEfctBossCmnFlushDataInit();	// ボス共通
#endif
}

// ==========================================================================
// GmPlayerStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void GmGameDatStaticVarInit(void)
{
	/// モデル構築登録ワーク
	memset(gm_obj_build_model_work_buf, 0, sizeof(gm_obj_build_model_work_buf));
	/// モデル構築登録数
	gm_obj_build_model_work_reg_num = 0;
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
