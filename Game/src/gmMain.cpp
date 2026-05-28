// ==========================================================================
/*!
  @file gmMain.cpp
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmMain.cpp 169 2011-05-25 09:42:34Z thamada $
  $Date:: 2011-05-25 18:42:34 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gs.h"
#include "gsMainSys.h"
#include "gsFont.h"

#include "aoAction.h"
#include "objObject.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmCamera.h"
#include "gmObj.h"
#include "gmTask.h"
#include "gmMap.h"
#include "gmMapFar.h"
#include "gmDeco.h"
#include "gmWaterSurface.h"
#include "gmEventMgr.h"
#include "gmRing.h"
#include "gmSound.h"
#include "gmClearDemo.h"
#include "gmStartDemo.h"
#include "gmFix.h"
#include "gmOver.h"
#include "gmPauseMenu.h"
#include "gmPause.h"
#include "gmPadVib.h"
#include "gmPlayer.h"
#include "gmPlyEfct.h"
#include "dmLoading.h"
#include "gmSplStage.h"
#include "gmEnding.h"
#include "gmStartMsg.h"

#include "gmGmkGear.h"
#include "gmBoss5.h"
#include "gmBoss5Land.h"
#include "gmGmkBreakLand.h"
#include "gmGmkNeedle.h"

#include "gmMain.h"


#include "izFade.h"

#if defined (MTD_DEBUG)
#include "dbgLightEdit.h"
#endif

#if _IPHONE
#include "gmTvx.h"
#include "dbgPadEmu.hpp"
#include "gmPadPolarHandle.hpp"
#include "gmPadVirtualPad.hpp"

#if SONIC4_TRIAL
#include "Sonic4_Utility.h"
#endif // SONIC4_TRIAL

#endif //_IPHONE

#include "mppCheckPointStorage.h"
#include "mppAchievementSupport.h"
#include "mppUtil.h"

//----- Definitions ---------------------------------------------------------
#define GMD_MAIN_DEBUG_LIGHT	(0 & MTD_DEBUG)
#define GMD_MAIN_DEBUG_COLOR	(0 & GMD_MAIN_DEBUG_LIGHT)

/// イベント移行先
#if defined (HOG_INLINE3_ROM) || defined (HOG_PRESENT_ROM_IPHONE)
enum {
	GMD_MAIN_NEXT_EVT_MAINGAME	= 0,// メインゲーム
	GMD_MAIN_NEXT_EVT_TITLE,		// タイトル
	GMD_MAIN_NEXT_EVT_DEBUGMENU,	// デバックメニュー
	//GMD_MAIN_NEXT_EVT_
	
	GMD_MAIN_NEXT_EVT_MAX
};
#else
enum {
	GMD_MAIN_NEXT_EVT_WORLDMAP	= 0,	// ワールドマップ
	GMD_MAIN_NEXT_EVT_MAINGAME,			// メインゲーム
	GMD_MAIN_NEXT_EVT_ENDING,			// エンディング
	GMD_MAIN_NEXT_EVT_SPSTAGE_BRA,		// スペステ分岐
	GMD_MAIN_NEXT_EVT_MAINMENU,			// メインメニュー
	GMD_MAIN_NEXT_EVT_TITLE,			// タイトル
#if !_WII
	GMD_MAIN_NEXT_EVT_BUYSCREEN,		// 購入画面
#endif
	//GMD_MAIN_NEXT_EVT_
	
	GMD_MAIN_NEXT_EVT_MAX
};
#endif

/* デバッグ */
#if defined (MTD_DEBUG)
#define GMD_DEBUG_LIGTH_MULTI	(0)
#endif
//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------
// alice デバック表示設定
#if defined (MTD_DEBUG)
extern Sint32 _am_dbg_display_mode;
#endif
#if _IPHONE
// AppMainより
extern int _am_sample_count;
extern BOOL _am_sample_draw_enable;
#endif // _IPHONE
//----- Static Declarations -------------------------------------------------
static void gmMainSysInit(void);

static void gmMainLoad(GME_GAMEDAT_LOAD_PROC load_proc);
static void gmMainDataLoadWait(MTS_TASK_TCB *tcb);
static void gmMainDataBuildWait(MTS_TASK_TCB *tcb);
static void gmMainDataLoadingEndWait(MTS_TASK_TCB *tcb);
static void gmMainDataLoadDest(MTS_TASK_TCB *tcb);

static void gmMainRebuild(void);
static void gmMainRebuildWait(MTS_TASK_TCB *tcb);
static void gmMainRebuildDest(MTS_TASK_TCB *tcb);

static void gmMainDataRelease(void);
static void gmMainDataFlushExitFinalClearObjWait(MTS_TASK_TCB *tcb);
static void gmMainDataFlushExitFinalLoadWait(MTS_TASK_TCB *tcb);
static void gmMainDataFlushExitFinalWait(MTS_TASK_TCB *tcb);
static void gmMainDataFlushExitWait(MTS_TASK_TCB *tcb);
static void gmMainDataFlushWait(MTS_TASK_TCB *tcb);
static void gmMainDataReleaseWait(MTS_TASK_TCB *tcb);
static void gmMainDataReleaseDest(MTS_TASK_TCB *tcb);

static void gmMainObjectRelease(void);
static void gmMainObjectReleaseFinalClearObjWait(MTS_TASK_TCB *tcb);
static void gmMainObjectReleaseFinalLoadWait(MTS_TASK_TCB *tcb);
static void gmMainObjectReleaseFinalWait(MTS_TASK_TCB *tcb);
static void gmMainObjectReleaseWait(MTS_TASK_TCB *tcb);
static void gmMainObjectReleaseDest(MTS_TASK_TCB *tcb);

static void gmMainGameStart(void);
static void gmMainInitLight(void);

//static void gmMainVFunc(void);
static void gmMainPre(MTS_TASK_TCB *tcb);
static void gmMainPost(MTS_TASK_TCB *tcb);

static void gmMainDecideNextEvt(void);

static BOOL gmMainCheckExeSyncTimerCount(void);


static void gmMainDataLoadBoosBattleMgr_LoadWait(MTS_TASK_TCB *tcb);
static void gmMainDataLoadBoosBattleMgr_BuildWait(MTS_TASK_TCB *tcb);
//static void gmMainDataLoadBoosBattleMgr_EndWait(MTS_TASK_TCB *tcb);
static void gmMainDataReleaseBoosBattleMgr_FlushWait(MTS_TASK_TCB *tcb);
static void gmMainDataReleaseBoosBattleMgr_ReleaseWait(MTS_TASK_TCB *tcb);
static void gmMainDataReleaseBoosBattleMgr_EndWait(MTS_TASK_TCB *tcb);

#if _IPHONE
static BOOL gmMainIsUseWaitUpCamera(void);

// サスペンドポーズ処理
static BOOL gmMainIsSuspendedPause(void);
static void gmMainUpdateSuspendedPause(void);
#endif //_IPHONE

//----- Global Variables ----------------------------------------------------
/// ゲームメインシステムワーク
GMS_MAIN_SYSTEM	g_gm_main_system = {0};

//----- Local Variables -----------------------------------------------------
static MTS_TASK_TCB	*gm_main_load_wait_tcb = NULL;
static MTS_TASK_TCB	*gm_main_release_wait_tcb = NULL;

static MTS_TASK_TCB	*gm_main_load_bossbattle_tcb = NULL;	//!< ボス連戦用データロードマネージャ
static MTS_TASK_TCB	*gm_main_release_bossbattle_tcb = NULL;	//!< ボス連戦用データリリースマネージャ
#if SONIC4_TRIAL
static u32 g_trial_game_time = 0;
#endif // SONIC4_TRIAL


#if defined (MTD_DEBUG)
#if 0
// テスト用
static NNS_LIGHT_PARALLEL	gm_test_light;
static NNS_RGBA				gm_test_light_col = {
		1.0f, 1.0f, 1.0f, 1.0f,
};
static float				gm_test_light_intensity = 1.0f;
static NNS_VECTOR			gm_test_light_dir = {
		0.0f, 0.0f, -1.0f,
};
#endif // #if !_PS3
#endif // #if defined (MTD_DEBUG)

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// 初期化・終了処理
// ==========================================================================
// ==========================================================================
// GmMainGSInit
/*!
 *	ゲーム 開始時 GS初期化
 */
// ==========================================================================
void GmMainGSInit(void)
{
	// GSS_MAIN_SYS_INFO の初期化
	g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_STARTINIT_MASK;

	g_gs_main_sys_info.clear_ring	= 0;
	g_gs_main_sys_info.clear_score	= 0;
	g_gs_main_sys_info.clear_time	= 0;
}

// ==========================================================================
// GmMainGSRetryInit
/*!
 *	ゲーム 開始時 リトライ時GS初期化
 */
// ==========================================================================
void GmMainGSRetryInit(void)
{
	// 残機数保存
	g_gs_main_sys_info.rest_player_num =
					g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P];

	// GSS_MAIN_SYS_INFO の初期化
	g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_RETRY_MASK;

	g_gs_main_sys_info.clear_ring	= 0;
	g_gs_main_sys_info.clear_score	= 0;
	g_gs_main_sys_info.clear_time	= 0;
}

// ==========================================================================
// GmMainInit
/*!
 *	ゲーム初期化
 */
// ==========================================================================
void GmMainInit(void *arg)
{
	UNREFERENCED_PARAMETER(arg);
	
	/* デモ用フォント解放 */
	GsFontRelease();
	
#if _IPHONE
	{
		//パッドトリガのモード切替設定
		dbg::CPadEmu &pad_emu = dbg::CPadEmu::CreateInstance();
		pad_emu.Create(dbg::CPadEmu::EMode::Game);
	}
#endif //_IPHONE

	/* ゲームメインシステムワーククリア */
	//MI_CpuClear8(&g_gm_main_system, sizeof(GMS_MAIN_SYSTEM));

	/* ゲームシステム初期化 */
	if (!(g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_RESTART)) {
		gmMainSysInit();		// リスタート時は行わない
	}

	/* オブジェクトシステム先行初期化 */
#if 0
	ObjInit(GMD_TASK_GROUP_OBJSYS, GMD_TASK_PRIO_OBJSYS, GMD_TASK_PAUSE_LEVEL_OBJSYS,
				GMD_OBJ_LCD_X, GMD_OBJ_LCD_Y, GSD_DISP_HEIGHT, GSD_DISP_HEIGHT);
#else
	//if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_2 ||
	//		(GSD_MAIN_STAGE_ID_SS1 <= g_gs_main_sys_info.stage_id &&
	//			g_gs_main_sys_info.stage_id <= GSD_MAIN_STAGE_ID_SS7)) {
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_2) {
		// 回転するステージ
		ObjInit(GMD_TASK_GROUP_OBJSYS, GMD_TASK_PRIO_OBJSYS, GMD_TASK_PAUSE_LEVEL_OBJSYS,
				(s16)(GMD_OBJ_LCD_X*1.42), (s16)(GMD_OBJ_LCD_X*1.42), GSD_DISP_HEIGHT, GSD_DISP_HEIGHT);
#if !_IPHONE
		// イベント生成範囲設定
		ObjObjectClipLCDSet((s16)(GMD_OBJ_CLIP_LCD_X*1.42), (s16)(GMD_OBJ_CLIP_LCD_X*1.42));
#endif
	}
	else {
		ObjInit(GMD_TASK_GROUP_OBJSYS, GMD_TASK_PRIO_OBJSYS, GMD_TASK_PAUSE_LEVEL_OBJSYS,
				GMD_OBJ_LCD_X, GMD_OBJ_LCD_Y, GSD_DISP_HEIGHT, GSD_DISP_HEIGHT);
#if !_IPHONE
		// イベント生成範囲設定
		ObjObjectClipLCDSet(GMD_OBJ_CLIP_LCD_X, GMD_OBJ_CLIP_LCD_Y);
#endif
	}
#endif
	ObjDataAlloc(GMD_DWORK_NO_MAX);							// データワーク数

	/* ライト設定 */
#if !GMD_DEBUG_LIGTH_MULTI
#if _WII
	g_obj.def_user_light_flag |= OBD_LIGHT_USE_FLAG_7/*スペキュラ*/;

	// Wii用トゥーンライト
	{
		// モデルビルド時に設定が必要な為先に初期化
		NNS_VECTOR	light_vec;
		switch (GMM_MAIN_GET_ZONE_TYPE()) {
		case GSD_MAIN_ZONE_TYPE_1:
		case GSD_MAIN_ZONE_TYPE_2:
		case GSD_MAIN_ZONE_TYPE_3:
			light_vec.x = -0.9f;
			light_vec.y = -0.8f;
			light_vec.z = -1.0f;
			break;
		case GSD_MAIN_ZONE_TYPE_4:
			if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_BOSS) {
				light_vec.x = -0.5f;
				light_vec.y =  0.27f;
				light_vec.z = -1.0f;
			}
			else {
				light_vec.x = -0.6f;
				light_vec.y =  0.5f;
				light_vec.z = -1.0f;
			}
			break;
		default:
		case GSD_MAIN_STAGE_ID_FINAL_1:
			light_vec.x = -1.8f;
			light_vec.y = -2.5f;
			light_vec.z = -2.0f;
			break;
		}
		nnNormalizeVector(&light_vec, &light_vec);
		g_obj.toon_light_vec = light_vec;
	}
#endif	// #if _WII

#else
#if !_WII
	g_obj.load_drawflag &= ~NND_DRAWOBJ_FRAGPARALIGHT1;
	g_obj.drawflag		&= ~NND_DRAWOBJ_FRAGPARALIGHT1;
	g_obj.load_drawflag |= (NND_DRAWOBJ_FRAGPARALIGHT3);
	g_obj.drawflag		|= (NND_DRAWOBJ_FRAGPARALIGHT3);
#endif

#if !_WII
	g_obj.def_user_light_flag = OBD_LIGHT_USE_FLAG_0 | OBD_LIGHT_USE_FLAG_1 | OBD_LIGHT_USE_FLAG_2;
#else
	g_obj.def_user_light_flag = OBD_LIGHT_USE_FLAG_0 | OBD_LIGHT_USE_FLAG_1 | OBD_LIGHT_USE_FLAG_2 |
								OBD_LIGHT_USE_FLAG_7/*スペキュラ*/;
#endif	// #if !_WII
#endif	// #if !GMD_DEBUG_LIGTH_MULTI


	// エフェクトシステム起動
	ObjDrawESEffectSystemInit(GMD_TASK_PAUSELEVEL_DEF,
							  GMD_TASK_PRIO_EFFECT_SERVER,
							  GMD_TASK_GROUP_EFFECT_SERVER);

	// 軌跡エフェクトシステム初期化
	amTrailEFInitialize();

	//描画順序設定
#if _IPHONE
	ObjDrawSetNNCommandStateTbl( 0, OBD_DRAW_CMD_STATE_PRE_MAPFAR, TRUE );
#else
	ObjDrawSetNNCommandStateTbl( 0, OBD_DRAW_CMD_STATE_PRE_MAPFAR, FALSE );
#endif
	ObjDrawSetNNCommandStateTbl( 1, OBD_DRAW_CMD_STATE_MAPFAR, FALSE );
	ObjDrawSetNNCommandStateTbl( 2, OBD_DRAW_CMD_STATE_POST_MAPFAR, TRUE );
	ObjDrawSetNNCommandStateTbl( 3, OBD_DRAW_CMD_STATE_WATER_BACK, TRUE );
	ObjDrawSetNNCommandStateTbl( 4, OBD_DRAW_CMD_STATE_MAPMID, TRUE );			// 中景
	ObjDrawSetNNCommandStateTbl( 5, OBD_DRAW_CMD_STATE_WATER_MAPMID, TRUE );	// 中景前水
#if _IPHONE
	// 3DNNを分割描画
	ObjDrawSetNNCommandStateTbl( 6, OBD_DRAW_CMD_STATE_3DNN_PRE, FALSE ); // 装飾とか
	ObjDrawSetNNCommandStateTbl( 7, OBD_DRAW_CMD_STATE_3DNN, FALSE );
	ObjDrawSetNNCommandStateTbl( 8, OBD_DRAW_CMD_STATE_3DNN_POST, TRUE ); // エフェクトとか
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_3 || g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		ObjDrawSetNNCommandStateTbl( 9, OBD_DRAW_CMD_STATE_3DNN_WS, TRUE ); // WaterSliderのしぶき
	}
	else {
		ObjDrawSetNNCommandStateTbl( 9, OBD_DRAW_CMD_STATE_INVALID, FALSE ); // WaterSliderのしぶきは使用しない
	}
	
	ObjDrawSetNNCommandStateTbl(10, OBD_DRAW_CMD_STATE_PRE_WATER, TRUE );
	ObjDrawSetNNCommandStateTbl(11, OBD_DRAW_CMD_STATE_WATER, TRUE );
	ObjDrawSetNNCommandStateTbl(12, OBD_DRAW_CMD_STATE_POST_WATER, TRUE );
	ObjDrawSetNNCommandStateTbl(13, OBD_DRAW_CMD_STATE_NEAR_MAP, TRUE );
	ObjDrawSetNNCommandStateTbl(14, OBD_DRAW_CMD_STATE_3DFIX, TRUE );	// 3D描画最前面 FIX系
	ObjDrawSetNNCommandStateTbl(15, OBD_DRAW_CMD_STATE_2DAMA, TRUE );	// 2D描画 必ず一番最後に
#else
	ObjDrawSetNNCommandStateTbl( 6, OBD_DRAW_CMD_STATE_3DNN, TRUE );
	ObjDrawSetNNCommandStateTbl( 7, OBD_DRAW_CMD_STATE_PRE_WATER, TRUE );
	ObjDrawSetNNCommandStateTbl( 8, OBD_DRAW_CMD_STATE_WATER, TRUE );
	ObjDrawSetNNCommandStateTbl( 9, OBD_DRAW_CMD_STATE_POST_WATER, TRUE );
	ObjDrawSetNNCommandStateTbl(10, OBD_DRAW_CMD_STATE_NEAR_MAP, TRUE );
	ObjDrawSetNNCommandStateTbl(11, OBD_DRAW_CMD_STATE_3DFIX, TRUE );	// 3D描画最前面 FIX系
	ObjDrawSetNNCommandStateTbl(12, OBD_DRAW_CMD_STATE_2DAMA, TRUE );	// 2D描画 必ず一番最後に
#endif // _IPHONE
	// aliceのEndSceneでソートが行われなくなったので、必ず最後にソート描画する事

	// 2Dアクションシステム設定
	// 各種最大使用量のクリア
	AoActSysClearPeak();
	//AoActSysSetDrawTaskPrio();

	/* データ読み込み */
	if (!(g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_DATALOADEND)) {
		// 通常開始時
		gmMainLoad(GMD_GAMEDAT_LOAD_PROC_NORMAL);
	}
	else {
		// リスタート時
		// データ再構築
		gmMainRebuild();
	}

	// データ読み込み待機へ
	// データ読み込み終了後、待機処理からゲーム初期化へ移行
}

// ==========================================================================
// GmMainEnd
/*!
 *	ゲーム 終了
 *
 *	@note
 *		ゲーム処理の終了のみ行う
 */
// ==========================================================================
void GmMainEnd(void)
{
	/* パッド振動終了 */
	GmPadVibExit();

	// メイン処理破棄
	if (g_gm_main_system.pre_tcb) {
		mtTaskClearTcb(g_gm_main_system.pre_tcb);
		g_gm_main_system.pre_tcb = NULL;
	}
	if (g_gm_main_system.post_tcb) {
		mtTaskClearTcb(g_gm_main_system.post_tcb);
		g_gm_main_system.post_tcb = NULL;
	}

	// 軌跡エフェクト破棄
	amTrailEFDeleteGroup(AMTRE_HANDLE_ACCELL);
	// 軌跡エフェクトシステム終了処理
	GmPlyEfctTrailSysExit();

	// オブジェクトシステム前処理クリア
	g_obj.ppPre			= NULL;

	// 管理オブジェクト破棄
	ObjObjectClearAllObject();
	// オブジェクトシステム終了前処理
	ObjPreExit();

	// マップ終了
	GmMapExit();

	// FIX終了
	GmFixExit();

	// ポーズ終了
	GmPauseExit();

	// リング終了処理
	GmRingExit();

	// カメラ終了
	GmCameraExit();

	// サウンド終了
	GmSoundExit();

	// 遠景終了
#if GMD_MAP_FAR_TEST
	GmMapFarExit();
#endif //GMD_MAP_FAR_TEST

	// 装飾終了
#if GMD_DECO_TEST
	GmDecoExit();
#endif //GMD_DECO_TEST

	// 水面終了
	GmWaterSurfaceExit();

	// イベントマネージャー終了
	GmEventMgrExit();

	// エフェクトシステム終了
	MTM_ASSERT(ObjDrawESEffectSystemIsActive());
	ObjDrawESEffectSystemExit();


	// その他ゲームタスク終了

#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	// クリアデモ終了
	GmClearDemoExit();
#endif	// #if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)

	// ゲーム／タイムオーバー終了
	GmOverExit();

	// スペシャルステージ終了
	GmSplStageExit();

	// エンディング終了
	GmEndingExit();
	
	// スタートデモ終了
#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	GmStartDemoExit();
#endif	// #if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)

	// ゲーム開始時メッセージ終了
	GmStartMsgExit();

#if defined (MTD_DEBUG)
	// デバック用 ライト編集
	DbgLightEditExit();
#endif

#if _IPHONE
	// パッドエミュ対応(開放)
	// VirtualPad
	gm::CPadVirtualPad &virtual_pad = gm::CPadVirtualPad::CreateInstance();
	virtual_pad.Release();
	// Polar
	gm::CPadPolarHandle &polar = gm::CPadPolarHandle::CreateInstance();
	polar.Release();
	
	// スリープ処理有効
	GsMainSysSetSleepFlag(TRUE);
	// 加速度センサー無効
	GsMainSysSetAccelFlag(FALSE);
#endif // _IPHONE

}

// ==========================================================================
// GmMainExit
/*!
 *	ゲーム メイン終了処理
 */
// ==========================================================================
void GmMainExit(void)
{
	// ゲーム終了処理
	GmMainEnd();

	// データ開放処理
	// Flushを行ってからRelease
	gmMainDataRelease();

#if 0
		// オブジェクトシステムデバック用設定終了
	// ObjDebugRectActionExit();	// ObjExit内で呼び出し

	// サバイバル
	// アイテムエリアの取得済みアイテムチェック
	gmMainGetSurvivalStageItemInfo();

	// 転送リクエストクリア
	mtChaClearAllRequest();
	mtScrClearAllRequest();
	mtPltClearAllRequest();

	// メイン処理破棄
	mtTaskClearTcb(g_gm_main_system.pre_tcb);
	mtTaskClearTcb(g_gm_main_system.post_tcb);
	mtTaskClearTcb(g_gm_main_system.key_tcb);
	g_gm_main_system.pre_tcb = NULL;
	g_gm_main_system.post_tcb = NULL;
	g_gm_main_system.key_tcb = NULL;

	/* キーレコード停止 */
	ObjKeyRecordStateSet(OBD_KEYRECORD_EXIT);
	if (g_gm_main_system.playdemo_key_dat) {
		// キーデータ解放
		mtMemFreeMain(g_gm_main_system.playdemo_key_dat);
		g_gm_main_system.playdemo_key_dat = NULL;
	}

	// 地震処理終了
	EfQuake(EFD_QUAKE_END);

	// 管理オブジェクト破棄
	//ObjObjectClearAllObject();
	// オブジェクトシステム終了
	ObjExit();

	// FIX終了処理
	GmFixExit();

	// イベントマネージャー終了
	GmEventMgrExit();

	// エフェクト終了処理
	GmEffectExit();

	// 遠景終了
	GmMapFarExit();

	// ステージ終了
	GmStageExit();

	// カメラ終了
	GmCameraExit();

	// サウンド終了
	GmSoundExit();

	// データ片付け
	GmGameDatFlushStandard();
	GmGameDatFlushArea();

	// データ開放
	GmGameDatReleaseStandard();
	GmGameDatReleaseArea();

	// オブジェクトシステム終了(データワークが保持しているデータを解放してから)
	ObjExit();

	// システム終了処理
	gmMainGameSysExit();

#endif
}

// ==========================================================================
// GmMainRestartExit
/*!
 *	ゲーム リスタート時メイン終了処理
 */
// ==========================================================================
void GmMainRestartExit(void)
{
	// ゲーム終了処理
	GmMainEnd();

	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_FINAL_1) {
		// オブジェクト破棄終了待機
		gmMainObjectRelease();

		// ObjExitは後で
	}
	else {
		// オブジェクトシステム終了
		g_obj.flag |= OBD_OBJ_SAVE_DATAWORK;	// データワーク退避
		ObjExit();

		// オブジェクト破棄終了待機
		gmMainObjectRelease();
	}
}



// ==========================================================================
// GmMainExitForStaffroll
/*!
 *	スタッフロールにて使用する メイン終了処理
  	エンディング終了時にデータのみ残したままとなっているため、
  	データ解放処理のみを行う関数を用意
 */
// ==========================================================================
void GmMainExitForStaffroll(void)
{
	// データ開放処理
	// Flushを行ってからRelease
	gmMainDataRelease();
}



// ==========================================================================
// タイマ関連
// ==========================================================================
// ==========================================================================
// GmMainCheckExeTimerCount
/*!
 *	タイマーカウント実行チェック
 *
 *	@return	TRUE : カウント実行
 *
 *	@note
 *		各種状況をチェックしてメインタイマーカウントアップを行うかチェックします
 */
// ==========================================================================
BOOL GmMainCheckExeTimerCount(void)
{
//	if (g_gm_main_system.game_flag & GMD_MAIN_GAME_FLAG_TIMERCOUNT_WAIT_MASK) {
//		return (FALSE);
//	}
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_COUNT_GAME_TIME) {
		return (TRUE);
	}
	return (FALSE);
}

#if _IPHONE
// ==========================================================================
// GmMainIsDrawEnable
/*!
 *	描画実行推奨チェック
 *
 *	@return	TRUE : 描画すべき
 *
 *	@note
 *		描画しないフレームを判定してくれます。
 */
// ==========================================================================
BOOL GmMainIsDrawEnable(void)
{
	BOOL is_flag = _am_sample_draw_enable;
//	if (PAD_DIRECT(0) & KEY_L_LEFT) {
//		is_flag = TRUE;
//	}
//	is_flag = TRUE;
	return is_flag;
}

// ==========================================================================
// GmMainGetDrawMotionSpeed
/*!
 *	モーション実行推奨チェック
 *
 *	@return	掛け合わせるべきモーション速度
 *
 *	@note
 *		スキップした分必要になるモーション速度を返します。。
 */
// ==========================================================================
float GmMainGetDrawMotionSpeed(void)
{
	return (float)_am_sample_count;
}

// ==========================================================================
// GmMainGetObjectRotation
/*!
 *	オブジェクト回転情報作成
 *
 *	@return	現在のステージにて回転させるべきオブジェクトの角度
 *
 *	@note
 *		iPhone版では<BR>
 *			・カメラが回転する<BR>
 *			・カメラ固定で本体の傾きにゲームが追随する<BR>
 *		の2パターンがあるため、<BR>
 *		それに対応した角度を返します。
 */
// ==========================================================================
u16 GmMainGetObjectRotation(void)
{
	u16 rotate = 0;
#ifdef GMD_MAIN_USE_BODY_ROTATE
	GSE_MAIN_STAGE_ID stage_id = (GSE_MAIN_STAGE_ID)g_gs_main_sys_info.stage_id;

	// SS or Zone3-2
	if (GSM_MAIN_STAGE_IS_SPSTAGE() || (stage_id == GSD_MAIN_STAGE_ID_3_2)) {
		GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
		if (ply_work) {
			rotate = (u16)-ply_work->key_rot_z;
		}
	}
	// other
	else {
		OBS_CAMERA	*obj_camera = ObjCameraGet(g_obj.glb_camera_id);
		if (obj_camera) {
			rotate = (u16)-obj_camera->roll;
		}
	}
	
#else
	OBS_CAMERA	*obj_camera = ObjCameraGet(g_obj.glb_camera_id);
	if (obj_camera) {
		rotate = (u16)-obj_camera->roll;
	}
#endif // GMD_MAIN_USE_BODY_ROTATE
	return rotate;
}

#if GMD_MAIN_DEBUG_COLOR
static u32 dbg_color_def[3] = {0xff, 0xff, 0xff};
static u32 dbg_color[3] = {0xcc, 0xcc, 0xcc};
static u32 dbg_color_idx = 0;
#endif // GMD_MAIN_DEBUG_COLOR
// ==========================================================================
// GmMainGetLightColor
/*!
 *	オブジェクトに設定する疑似ライト情報入手
 *
 *	@return	今のオブジェクトに設定すべき色(0xRRGGBBAA)
 *
 *	@note
 *		疑似ライトカラーとして頂点に設定してください。
 */
// ==========================================================================
u32 GmMainGetLightColor(void)
{
	// 疑似ライトカラー設定
	u32 color = 0xe0e0e0ff; // 基本色
	
	if (  (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3) 
		||(g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_BOSS)) {
		color = 0xe08A8Aff; // 夕方色
	}
	else if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		color = 0x9a9a9aff; // 4-3限定色
	}
	
	return color;
}
#endif // _IPHONE

// ==========================================================================
// ボス専用データロード
// ==========================================================================
// ==========================================================================
// GmMainDatLoadBossBattleStart
/*!
 *	ゲームデータロード 開始 ボス連戦用
 *
 *	@param	boss_type		[in]	ロードボスタイプ GME_GAMEDAT_LOAD_BOSS_TYPE
 *
 *	@note
 *		GmGameDatLoadCheck でロードチェック
 *		GmGameDatLoadExit でロード終了
 *		ビルドまで行います
 */
// ==========================================================================
typedef struct tag_GMS_GAMEDAT_LOAD_BB_MGR_WORK {
	GME_GAMEDAT_LOAD_BOSS_TYPE	boss_type;
	BOOL						b_end;
} GMS_MAIN_LOAD_BB_MGR_WORK;
void GmMainDatLoadBossBattleStart(s32 boss_type)
{
	// ◆ロード開始, 終了したら ポーズから終了できないように出来るか
	GMS_MAIN_LOAD_BB_MGR_WORK	*work;
	//MTM_ASSERT(gm_main_load_bossbattle_tcb == NULL);
	MTM_ASSERT((u32)boss_type < GMD_GAMEDAT_LOAD_BOSS_TYPE_MAX);
	MTM_ASSERT(g_gm_main_system.boss_load_no == -1 || g_gm_main_system.boss_load_no == boss_type);

	if (g_gm_main_system.boss_load_no == boss_type) {
		// 既にロード済み
#if 0
		// ロード済み終了待機状態のタスクをつくる
		// データロード監視タスク
		gm_main_load_bossbattle_tcb = MTM_TASK_MAKE_TCB(gmMainDataLoadBoosBattleMgr_EndWait, NULL,
							0/*flag*/, 0xFFFF/*pause_level*/,
							GMD_TASK_PRIO_DATA_LOAD, GMD_TASK_GROUP_DATA_LOAD,
							sizeof(GMS_MAIN_LOAD_BB_MGR_WORK)/*work_size*/, "GM_LOAD_BBM");
		work = (GMS_MAIN_LOAD_BB_MGR_WORK*)mtTaskGetTcbWork(gm_main_load_bossbattle_tcb);
		// ビルド終了
		work->b_end = TRUE;
#endif
		return;
	}

	// 前のデータの解放チェック
	if (GmMainDatReleaseBossBattleReleaseCheck()) {
		GmGameDatReleaseBossBattleExit();
	}
#if defined (MTD_DEBUG)
	else {
		MTM_ASSERT(!"gmMain.cpp::GmMainDatLoadBossBattleStart() Error! No release end");
	}
#endif

	// データロード初期化
	GmGameDatLoadBoosBattleInit((GME_GAMEDAT_LOAD_BOSS_TYPE)boss_type);

	// データロード監視タスク
	gm_main_load_bossbattle_tcb = MTM_TASK_MAKE_TCB(gmMainDataLoadBoosBattleMgr_LoadWait, NULL,
						0/*flag*/, 0xFFFF/*pause_level*/,
						GMD_TASK_PRIO_DATA_LOAD, GMD_TASK_GROUP_DATA_LOAD,
						sizeof(GMS_MAIN_LOAD_BB_MGR_WORK)/*work_size*/, "GM_LOAD_BBM");
	work = (GMS_MAIN_LOAD_BB_MGR_WORK*)mtTaskGetTcbWork(gm_main_load_bossbattle_tcb);
	// ボスタイプ保存
	work->boss_type = (GME_GAMEDAT_LOAD_BOSS_TYPE)boss_type;
	// 終了状態
	work->b_end		= FALSE;

	// データロード開始
	g_gm_main_system.game_flag |= GMD_GAME_FLAG_FINAL_DATA_LOAD;
}

// ==========================================================================
// GmMainDatLoadBossBattleLoadCheck
/*!
 *	ゲームデータロード チェック ボス連戦用
 *
 *	@return TRUE : 終了
 */
// ==========================================================================
BOOL GmMainDatLoadBossBattleLoadCheck(s32 boss_type/*=GMD_GAMEDAT_LOAD_BOSS_TYPE_1*/)
{
#if 0
	if (gm_main_load_bossbattle_tcb) {
		GMS_MAIN_LOAD_BB_MGR_WORK	*work;
		work = (GMS_MAIN_LOAD_BB_MGR_WORK*)mtTaskGetTcbWork(gm_main_load_bossbattle_tcb);

		return (work->b_end);
	}
#endif
	if (g_gm_main_system.boss_load_no != -1 &&
			!(g_gm_main_system.game_flag & GMD_GAME_FLAG_FINAL_DATA_RELEASE)) {
		if (boss_type == -1 ||
				boss_type == g_gm_main_system.boss_load_no) {
			return (TRUE);
		}
	}
	return (FALSE);
}

// ==========================================================================
// GmMainDatLoadBossBattleLoadNowCheck
/*!
 *	ゲームデータロード中 チェック ボス連戦用
 *
 *	@return TRUE : 終了
 */
// ==========================================================================
BOOL GmMainDatLoadBossBattleLoadNowCheck(void)
{
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_FINAL_DATA_LOAD) {
		// 現在ロード中
		return (TRUE);
	}
	return (FALSE);
}

// ==========================================================================
// GmGameDatLoadBossBattleExit
/*!
 *	ゲームデータロード 終了 ボス連戦用
 */
// ==========================================================================
void GmGameDatLoadBossBattleExit(void)
{
	if (gm_main_load_bossbattle_tcb) {
		mtTaskClearTcb(gm_main_load_bossbattle_tcb);
		gm_main_load_bossbattle_tcb = NULL;
	}
}

// ==========================================================================
// ボス連戦用 データリリース
// ==========================================================================
// ==========================================================================
// GmGameDatLoadBoosBattleStart
/*!
 *	ゲームデータリリース 開始 ボス連戦用
 *
 *	@param	boss_type		[in]	ロードボスタイプ GME_GAMEDAT_LOAD_BOSS_TYPE
 *
 *	@note
 *		Flushまで行います
 */
// ==========================================================================
void GmGameDatReleaseBossBattleStart(s32 boss_type)
{
	// ◆リリース開始, 終了したら ポーズから終了できないように出来るか
	GMS_MAIN_LOAD_BB_MGR_WORK	*work;
	MTM_ASSERT(gm_main_release_bossbattle_tcb == NULL);

	// データフラッシュ
	GmGameDatFlushBossBattleInit();
	GmGameDatFlushBossBattle(boss_type);

	// データロード監視タスク
	gm_main_release_bossbattle_tcb = MTM_TASK_MAKE_TCB(gmMainDataReleaseBoosBattleMgr_FlushWait, NULL,
						0/*flag*/, 0xFFFF/*pause_level*/,
						GMD_TASK_PRIO_DATA_LOAD, GMD_TASK_GROUP_DATA_LOAD,
						sizeof(GMS_MAIN_LOAD_BB_MGR_WORK)/*work_size*/, "GM_RELEASEBBM");
	work = (GMS_MAIN_LOAD_BB_MGR_WORK*)mtTaskGetTcbWork(gm_main_release_bossbattle_tcb);
	// ボスタイプ保存
	work->boss_type = (GME_GAMEDAT_LOAD_BOSS_TYPE)boss_type;
	// 終了状態
	work->b_end		= FALSE;

	// データ解放開始
	g_gm_main_system.game_flag |= GMD_GAME_FLAG_FINAL_DATA_RELEASE;
}

// ==========================================================================
// GmMainDatReleaseBossBattleReleaseCheck
/*!
 *	ゲームデータリリース チェック ボス連戦用
 *
 *	@return TRUE : 終了
 */
// ==========================================================================
BOOL GmMainDatReleaseBossBattleReleaseCheck(void)
{
#if 0
	if (gm_main_release_bossbattle_tcb) {
		GMS_MAIN_LOAD_BB_MGR_WORK	*work;
		work = (GMS_MAIN_LOAD_BB_MGR_WORK*)mtTaskGetTcbWork(gm_main_release_bossbattle_tcb);

		return (work->b_end);
	}
#endif
	if (g_gm_main_system.boss_load_no == -1 &&
			!(g_gm_main_system.game_flag & GMD_GAME_FLAG_FINAL_DATA_LOAD)) {
		return (TRUE);
	}
	return (FALSE);
}

// ==========================================================================
// GmMainDatReleaseBossBattleReleaseNowCheck
/*!
 *	ゲームデータリリース中 チェック ボス連戦用
 *
 *	@return TRUE : 終了
 */
// ==========================================================================
BOOL GmMainDatReleaseBossBattleReleaseNowCheck(void)
{
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_FINAL_DATA_RELEASE) {
		return (TRUE);
	}
	return (FALSE);
}

// ==========================================================================
// GmGameDatReleaseBossBattleExit
/*!
 *	ゲームデータリリース 終了 ボス連戦用
 */
// ==========================================================================
void GmGameDatReleaseBossBattleExit(void)
{
	if (gm_main_release_bossbattle_tcb) {
		mtTaskClearTcb(gm_main_release_bossbattle_tcb);
		gm_main_release_bossbattle_tcb = NULL;
	}
}

#if _IPHONE
// ==========================================================================
// Sint32 GmMainKeyCheckPauseKeyOn(void);
/*!
 *	ポーズキー On
 *
 *	@return	-1:入力なし　0～4:入力された番号
 *
 *	@note
 *		タッチの判定を返します。現在は領域を限定して判定しています。
 */
// ==========================================================================
Sint32 GmMainKeyCheckPauseKeyOn(void)
{
	Sint32 index = -1;
	// position setting
	Uint32 pos_x = 215;
	Uint32 pos_y =   0;
	if (GSM_MAIN_STAGE_IS_SPSTAGE()) {
		pos_x = 390;
		pos_y =   0;
	}
	else if (GsGetMainSysInfo()->game_mode == GSD_GAME_MODE_TIME_ATTACK) {
		pos_x = 275;
		pos_y =   0;
	}
	
	// search count
	for (int i = 0; i < AMD_TP_TOUCH_POS_MAX; i++) {
		if (amTpIsTouchOn(i)) {
			// 画面上のアイコンをタップ
			Uint16 px = _am_tp_touch[i].on[AMD_X];
			Uint16 py = _am_tp_touch[i].on[AMD_Y];
			// region check
			if ((px >= pos_x && px <= (pos_x + 115)) && (py >= pos_y && py <= (pos_y + 60)))
			{
				px = _am_tp_touch[i].push[AMD_X];
				py = _am_tp_touch[i].push[AMD_Y];
				// region check
				if ((px >= pos_x && px <= (pos_x + 115)) && (py >= pos_y && py <= (pos_y + 60)))
				{
					index = i;
					break;
				}
			}
		}
		
	}
	
	return index;
}

// ==========================================================================
// GmMainKeyCheckPauseKeyPush
/*!
 *	ポーズキー Push
 *
 *	@return	-1:入力なし　0～4:入力された番号
 *
 *	@note
 *		タッチの判定を返します。現在は領域を限定して判定しています。
 */
// ==========================================================================
Sint32 GmMainKeyCheckPauseKeyPush(void)
{
	Sint32 index = -1;
	// position setting
	Uint32 pos_x = 215;
	Uint32 pos_y =   0;
	if (GSM_MAIN_STAGE_IS_SPSTAGE()) {
		pos_x = 390;
		pos_y =   0;
	}
	else if (GsGetMainSysInfo()->game_mode == GSD_GAME_MODE_TIME_ATTACK) {
		pos_x = 275;
		pos_y =   0;
	}
	
	// search count
	for (int i = 0; i < AMD_TP_TOUCH_POS_MAX; i++) {
		if (amTpIsTouchPush(i)) {
			// 画面上のアイコンをタップ
			Uint16 px = _am_tp_touch[i].push[AMD_X];
			Uint16 py = _am_tp_touch[i].push[AMD_Y];
			// region check
			if ((px >= pos_x && px <= (pos_x + 115)) && (py >= pos_y && py <= (pos_y + 60)))
			{
				index = i;
				break;
			}
		}
		
	}
	
	return index;
}
#endif // _IPHONE

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// システム初期化
// ==========================================================================
// ==========================================================================
// gmMainSysInit
/*!
 *	ゲームシステム初期化
 *
 *	@note
 *		ゲーム起動時に一度だけ初期化
 */
// ==========================================================================
void gmMainSysInit(void)
{
	/* ゲームメインシステムワーククリア */
	MI_CpuClear8(&g_gm_main_system, sizeof(GMS_MAIN_SYSTEM));

	// 残機数コピー
	g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P] =
							g_gs_main_sys_info.rest_player_num;

	// この時点で0機だった場合は1にしておく
	if (g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P] <= 0) {
		g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P] = 1;
	}

	// ランダムシード設定(◆リプレイ時は保存値で)
	mtMathSRand((u32)(nnRandom() * 0x7FFF));
}

// ==========================================================================
// データロード
// ==========================================================================
// ==========================================================================
// gmMainLoad
/*!
 *	ゲーム データロード 通常用
 *
 *	@param	load_proc [in]	GMD_GAMEMAIN_DATA_PROC_***	ロード処理内容
 *
 *	@note
 *		データロードを行った後、データ構築してゲームを開始します
 */
// ==========================================================================
void gmMainLoad(GME_GAMEDAT_LOAD_PROC load_proc)
{
	s32	i;
	s16	char_id_list[GSD_MAIN_PLAYER_MAX];

#if _IPHONE
	// どのような経路でここにたどり着くかわからないので、
	// サスペンド情報を一度開放する
	GmMainClearSuspendedPause();
#endif // _IPHONE

	/* Loading画面用意 */
	DmLoadingStart();
	
#ifdef SONIC4_TRIAL		
	// Alartビュー表示開始
	Sonic4_StartAlertView();
#endif // SONIC4_TRIAL
	
#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	// ポーズメニューデータロード開始
	GmPauseMenuLoadStart();
#endif /* !defined(GMD_DEBUG_NO_CREATE_COCKPIT) */

	for (i = 0; i < GSD_MAIN_PLAYER_MAX; i++) {
		char_id_list[i] = (s16)g_gs_main_sys_info.char_id[i];
	}
	GmGameDatLoadInit(load_proc, g_gs_main_sys_info.stage_id, char_id_list);

	// データロード待機処理生成
	gm_main_load_wait_tcb = MTM_TASK_MAKE_TCB(gmMainDataLoadWait, gmMainDataLoadDest,
						0/*flag*/, 0xFFFF/*pause_level*/, 0x1000/*prio*/, 0/*group*/,
						0/*work_size*/, "GM_LOAD_WAIT");
}

// ==========================================================================
// gmMainDataLoadWait
/*!
 *	データロード待機
 */
// ==========================================================================
void gmMainDataLoadWait(MTS_TASK_TCB *tcb)
{
#if _IPHONE
	// サスペンド情報更新
	gmMainUpdateSuspendedPause();
#endif // _IPHONE
#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	// ポーズメニューデータロード待ち
	//  （将来ロード時間等の問題が生じた場合は GmPauseMenuLoad***()を使用せず、
	//    gmGameDatに組み込むように対応を入れる。）
	if (GmPauseMenuLoadIsFinished() == FALSE) {
		return;
	}
#endif /* !defined(GMD_DEBUG_NO_CREATE_COCKPIT) */

	if (GmGameDatLoadCheck() == GMD_GAMEDAT_LOAD_PROGRESS_COMPLETE) {

		// データビルド前初期化
		GmGameDatBuildInit();

		// データビルド
		GmGameDatBuildStandard();
		GmGameDatBuildArea();

		// データ構築待機へ
		mtTaskChangeTcbProcedure(tcb, gmMainDataBuildWait);
	}
}

// ==========================================================================
// gmMainDataBuildWait
/*!
 *	データ構築待機
 */
// ==========================================================================
void gmMainDataBuildWait(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

#if _IPHONE
	// サスペンド情報更新
	gmMainUpdateSuspendedPause();
#endif // _IPHONE

	// データ構築終了チェック
	if (GmGameDatBuildStandardCheck() == FALSE) {
		return;
	}
	if (GmGameDatBuildAreaCheck() == FALSE) {
		return;
	}
#if SONIC4_TRIAL
	// ここでAlartビュー待ちにする
	if (Sonic4_IsEnabledAlertView() == TRUE) {
		return;
	}
#endif // SONIC4_TRIAL
	// データロード終了
	GmGameDatLoadExit();

	// データロード済みフラグ設定
	g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_DATALOADEND;

#if 1
	// NOW LOADING 終了待機へ
	DmLoadingSetLoadComplete();
	mtTaskChangeTcbProcedure(tcb, gmMainDataLoadingEndWait);

#else
	// ゲーム初期化
	gmMainGameStart();

	// データ読み込み処理破棄
	mtTaskClearTcb(tcb);
#endif
}

// ==========================================================================
// gmMainDataLoadingEndWait
/*!
 *	データロード画面終了待機
 */
// ==========================================================================
void gmMainDataLoadingEndWait(MTS_TASK_TCB *tcb)
{
#if _IPHONE
	// サスペンド情報更新
	gmMainUpdateSuspendedPause();
#endif // _IPHONE

	if (DmLoadingIsExit()) {
		// 終了
		// ゲーム初期化
		gmMainGameStart();

		// データ読み込み処理破棄
		mtTaskClearTcb(tcb);
	}
}

// ==========================================================================
// gmMainDataLoadDest
/*!
 *	データ読み込みデストラクタ
 */
// ==========================================================================
void gmMainDataLoadDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	gm_main_load_wait_tcb = NULL;
}


// ==========================================================================
// データ再構築
// ==========================================================================
// ==========================================================================
// gmMainRebuild
/*!
 *	ゲーム データ 再構築
 *
 *	@note
 *		データロードは行われているものとして、データの再構築を行います。
 */
// ==========================================================================
void gmMainRebuild(void)
{
	// データロード待機処理生成
	gm_main_load_wait_tcb = MTM_TASK_MAKE_TCB(gmMainRebuildWait, gmMainRebuildDest,
						0/*flag*/, 0xFFFF/*pause_level*/, 0x1000/*prio*/, 0/*group*/,
						0/*work_size*/, "GM_REBUILD_WAIT");

	// データ再構築開始
	GmGameDatReBuildRestart();
}

// ==========================================================================
// gmMainRebuildWait
/*!
 *	データ再構築待機
 */
// ==========================================================================
void gmMainRebuildWait(MTS_TASK_TCB *tcb)
{
	if (GmGameDatReBuildRestartCheck() == FALSE) {
		return;
	}

	// ゲーム初期化
	gmMainGameStart();

	// データ読み込み処理破棄
	mtTaskClearTcb(tcb);
}

// ==========================================================================
// gmMainRebuildDest
/*!
 *	データ再構築デストラクタ
 */
// ==========================================================================
void gmMainRebuildDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	gm_main_load_wait_tcb = NULL;
}

// ==========================================================================
// 終了処理
// ==========================================================================
// ==========================================================================
// gmMainDataRelease
/*!
 *	ゲーム ゲーム終了 データ破棄処理
 */
// ==========================================================================
void gmMainDataRelease(void)
{
//	s32	i;
//	s16	char_id_list[GSD_MAIN_PLAYER_MAX];

//	for (i = 0; i < GSD_MAIN_PLAYER_MAX; i++) {
//		char_id_list[i] = (s16)g_gs_main_sys_info.char_id[i];
//	}
//	GmGameDatLoadInit(load_proc, g_gs_main_sys_info.stage_id, char_id_list);

	GSF_TASK_PROCEDURE proc;

	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_FINAL_1) {
		// データ開放待機処理生成
		proc = gmMainDataFlushExitWait;
	}
	else {
		// ファイナルステージ用
		proc = gmMainDataFlushExitFinalClearObjWait;
	}

	// データ解放処理生成
	gm_main_release_wait_tcb = MTM_TASK_MAKE_TCB(proc, gmMainDataReleaseDest,
						0/*flag*/, 0xFFFF/*pause_level*/, 0x1000/*prio*/, 0/*group*/,
						0/*work_size*/, "GM_UNLOAD_WAIT");
}

// ==========================================================================
// gmMainDataFlushExitFinalClearObjWait
/*!
 *	データ開放待機 終了処理待ち ファイナルステージ用 オブジェクト解放待ち
 */
// ==========================================================================
void gmMainDataFlushExitFinalClearObjWait(MTS_TASK_TCB *tcb)
{
	GSF_TASK_PROCEDURE proc;

	if (!ObjObjectCheckClearAllObject()) {
		// オブジェクトの終了待ち
		return;
	}

	// サウンドシステムリセット
	// （オブジェクトのデストラクタ呼び出し前にリセットすると
	//   SCB,SEハンドル二重解放の恐れがあるためここで呼び出し。）
	GsSoundReset();

	// ファイナルステージ用
	if (GmMainDatLoadBossBattleLoadCheck()) {
	// データロード済み
		proc = gmMainDataFlushExitFinalWait;
		// ロード処理終了
		GmGameDatLoadBossBattleExit();

		// 解放開始
		GmGameDatReleaseBossBattleStart(g_gm_main_system.boss_load_no);
	}
	else if (GmMainDatLoadBossBattleLoadNowCheck()) {
		// データロード読み込み処理中
		proc = gmMainDataFlushExitFinalLoadWait;
	}
	else if (GmMainDatReleaseBossBattleReleaseNowCheck()) {
		// 現在開放中 解放処理待機
		proc = gmMainDataFlushExitFinalWait;
		// ロード処理終了
		GmGameDatLoadBossBattleExit();		// 念のため
	}
	else {
		// 解放処理無し(通常タイプ)
		proc = gmMainDataFlushExitWait;

		// 解放処理終了
		GmGameDatReleaseBossBattleExit();
	}

	// 状態により各種待機状態へ
	mtTaskChangeTcbProcedure(tcb, proc);
}

// ==========================================================================
// gmMainDataFlushExitFinalLoadWait
/*!
 *	データ開放待機 終了処理待ち ファイナルステージ用 ロード終了後開放
 */
// ==========================================================================
void gmMainDataFlushExitFinalLoadWait(MTS_TASK_TCB *tcb)
{
	if (GmMainDatLoadBossBattleLoadCheck()) {
		// ロード終了
		// ロード処理終了
		GmGameDatLoadBossBattleExit();
		// 解放開始
		GmGameDatReleaseBossBattleStart(g_gm_main_system.boss_load_no);
		// 解放待機へ
		mtTaskChangeTcbProcedure(tcb, gmMainDataFlushExitFinalWait);
	}
}

// ==========================================================================
// gmMainDataFlushExitFinalWait
/*!
 *	データ開放待機 終了処理待ち ファイナルステージ用
 */
// ==========================================================================
void gmMainDataFlushExitFinalWait(MTS_TASK_TCB *tcb)
{
	if (GmMainDatReleaseBossBattleReleaseCheck()) {
		// 解放処理終了
		GmGameDatReleaseBossBattleExit();
		// 通常解放処理へ
		mtTaskChangeTcbProcedure(tcb, gmMainDataFlushExitWait);
	}
}

// ==========================================================================
// gmMainDataFlushExitWait
/*!
 *	データ開放待機 終了処理待ち
 */
// ==========================================================================
//static s32 gm_main_release_wait_cnt;
//static void gmMainDataFlushExitStartWait(MTS_TASK_TCB *tcb);
void gmMainDataFlushExitWait(MTS_TASK_TCB *tcb)
{
	if (!ObjObjectCheckClearAllObject()) {
		// オブジェクトの終了待ち
		return;
	}
//	if (ObjIsExitWait()) {
//		// オブジェクトシステム終了待ち
//		return;
//	}

	// サウンドシステムリセット
	// （オブジェクトのデストラクタ呼び出し前にリセットすると
	//   SCB,SEハンドル二重解放の恐れがあるためここで呼び出し。）
	GsSoundReset();

#if 0
	// データ開放開始待機へ
	mtTaskChangeTcbProcedure(tcb, gmMainDataFlushExitStartWait);
	gm_main_release_wait_cnt = 0;
#else
	// データ開放待機へ
	mtTaskChangeTcbProcedure(tcb, gmMainDataFlushWait);

	// データフラッシュ前初期化
	GmGameDatFlushInit();

	// データフラッシュ
	GmGameDatFlushArea();
	GmGameDatFlushStandard();
#endif
}

#if 0
// ==========================================================================
// gmMainDataFlushExitStartWait
/*!
 *	データ開放開始待機
 */
// ==========================================================================
void gmMainDataFlushExitStartWait(MTS_TASK_TCB *tcb)
{
	gm_main_release_wait_cnt++;
	if (gm_main_release_wait_cnt <= 10) {
		return;
	}


	// データ開放待機へ
	mtTaskChangeTcbProcedure(tcb, gmMainDataFlushWait);

	// データフラッシュ前初期化
	GmGameDatFlushInit();

	// データフラッシュ
	GmGameDatFlushArea();
	GmGameDatFlushStandard();
}
#endif

// ==========================================================================
// gmMainDataFlushWait
/*!
 *	データ開放待機
 */
// ==========================================================================
void gmMainDataFlushWait(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// データフラッシュ終了チェック
	if (GmGameDatFlushStandardCheck() == FALSE) {
		return;
	}
	if (GmGameDatFlushAreaCheck() == FALSE) {
		return;
	}

	// データワーク開放
	{
		s32	i;
		OBS_DATA_WORK	*data_work;
		data_work = g_obj.pData;
		for (i = 0; i < g_obj.data_max; i++, data_work++) {
			if (data_work->pData && !(data_work->num & OBD_DATA_ARCHIVE_FLAG)) {
				amMemFree(data_work->pData);
			}
		}
	}

	// オブジェクトシステム終了
	ObjExit();

	// データ開放
	GmGameDatReleaseStandard();
	GmGameDatReleaseArea();

#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	// ポーズメニューデータ解放
	GmPauseMenuRelease();
#endif /* !defined(GMD_DEBUG_NO_CREATE_COCKPIT) */

	// データ開放待機へ
	mtTaskChangeTcbProcedure(tcb, gmMainDataReleaseWait);
}

// ==========================================================================
// gmMainDataReleaseWait
/*!
 *	データロード待機
 */
// ==========================================================================
void gmMainDataReleaseWait(MTS_TASK_TCB *tcb)
{
	if (GmGameDatReleaseCheck() && !ObjIsExitWait()) {

		// データロード済みフラグOFF
		g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_DATALOADEND;

		// データ読み込み処理破棄
		mtTaskClearTcb(tcb);

		// イベント移行
		//SyDecideEvtCase(0);
#if defined (HOG_INLINE3_ROM) || defined (HOG_PRESENT_ROM_IPHONE)
		if (g_gm_main_system.game_flag & GMD_GAME_FLAG_CLEAR) {
			if (g_gs_main_sys_info.stage_id < GSD_MAIN_STAGE_ID_1_BOSS) {
				g_gs_main_sys_info.stage_id++;
			}
#if defined (HOG_PRESENT_ROM_IPHONE)
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_NEXTACT;
			g_gm_main_system.marker_pri = 0;
#endif // HOG_PRESENT_ROM_IPHONE
		}

		if (!(g_gm_main_system.game_flag & GMD_GAME_FLAG_CLEAR ||
				g_gm_main_system.game_flag & GMD_GAME_FLAG_GAMEOVER_END)) {
#if _IPHONE
			g_gm_main_system.marker_pri = 0;
#endif //_IPHONE
		//	if (AoPadDirect() & KEY_R_UP) {
		//		// デバックメニュー
		//		SyDecideEvtCase(2);
		//	}
		}
#endif

		// 残機数保存
		g_gs_main_sys_info.rest_player_num =
					g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P];

		// ゲームオーバーになっているので残機数を復帰
		if (g_gs_main_sys_info.rest_player_num <= 0) {
			g_gs_main_sys_info.rest_player_num = 3;
		}

		// ファイナルステージクリア時のエンディング移行設定
		if (g_gs_main_sys_info.game_mode == GSD_GAME_MODE_STORY &&
				g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_CLEAR &&
				g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_FINAL_1) {
			g_gs_main_sys_info.stage_id		= GSD_MAIN_STAGE_ID_ENDING;
			g_gs_main_sys_info.char_id[0]	= GSD_CHAR_ID_SONIC;
			g_gs_main_sys_info.game_mode	= GSD_GAME_MODE_ENDING;

			GmMainGSInit();
		}

		SyChangeNextEvt();
	}
}

// ==========================================================================
// gmMainDataReleaseDest
/*!
 *	データ開放デストラクタ
 */
// ==========================================================================
void gmMainDataReleaseDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	gm_main_release_wait_tcb = NULL;
}


// ==========================================================================
// リスタート時終了処理
// ==========================================================================
// ==========================================================================
// gmMainObjectRelease
/*!
 *	ゲーム オブジェクト破棄終了待機
 */
// ==========================================================================
void gmMainObjectRelease(void)
{
	GSF_TASK_PROCEDURE proc;

	// データ開放待機処理生成
	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_FINAL_1) {
		// 通常
		proc = gmMainObjectReleaseWait;
	}
	else {
		// ファイナルステージ用
		proc = gmMainObjectReleaseFinalClearObjWait;
	}

	gm_main_release_wait_tcb = MTM_TASK_MAKE_TCB(proc, gmMainObjectReleaseDest,
						0/*flag*/, 0xFFFF/*pause_level*/, 0x1000/*prio*/, 0/*group*/,
						0/*work_size*/, "GM_UNLOAD_OBJ_WAIT");
}

// ==========================================================================
// gmMainObjectReleaseFinalClearObjWait
/*!
 *	データ開放待機 終了処理待ち ファイナルステージ用 オブジェクト解放待ち
 */
// ==========================================================================
void gmMainObjectReleaseFinalClearObjWait(MTS_TASK_TCB *tcb)
{
	GSF_TASK_PROCEDURE proc;

	if (!ObjObjectCheckClearAllObject()) {
		// オブジェクトの終了待ち
		return;
	}

	// ファイナルステージ用
	if (GmMainDatLoadBossBattleLoadCheck()) {
	// データロード済み
		proc = gmMainObjectReleaseFinalWait;
		// ロード処理終了
		GmGameDatLoadBossBattleExit();

		// 解放開始
		GmGameDatReleaseBossBattleStart(g_gm_main_system.boss_load_no);
	}
	else if (GmMainDatLoadBossBattleLoadNowCheck()) {
		// データロード読み込み処理中
		proc = gmMainObjectReleaseFinalLoadWait;
	}
	else if (GmMainDatReleaseBossBattleReleaseNowCheck()) {
		// 現在開放中 解放処理待機
		proc = gmMainObjectReleaseFinalWait;
		// ロード処理終了
		GmGameDatLoadBossBattleExit();		// 念のため
	}
	else {
		// 解放処理無し(通常タイプ)
		proc = gmMainObjectReleaseWait;

		// 解放処理終了
		GmGameDatReleaseBossBattleExit();

		// オブジェクトシステム終了
		g_obj.flag |= OBD_OBJ_SAVE_DATAWORK;	// データワーク退避
		ObjExit();
	}
	// 状況により各待機へ遷移
	mtTaskChangeTcbProcedure(tcb, proc);
}

// ==========================================================================
// gmMainObjectReleaseFinalLoadWait
/*!
 *	データ開放待機 終了処理待ち ファイナルステージ用 ロード終了後開放
 */
// ==========================================================================
void gmMainObjectReleaseFinalLoadWait(MTS_TASK_TCB *tcb)
{
	if (GmMainDatLoadBossBattleLoadCheck()) {
		// ロード終了
		// ロード処理終了
		GmGameDatLoadBossBattleExit();
		// 解放開始
		GmGameDatReleaseBossBattleStart(g_gm_main_system.boss_load_no);
		// 解放待機へ
		mtTaskChangeTcbProcedure(tcb, gmMainObjectReleaseFinalWait);
	}
}

// ==========================================================================
// gmMainObjectReleaseFinalWait
/*!
 *	データ開放待機 終了処理待ち ファイナルステージ用
 */
// ==========================================================================
void gmMainObjectReleaseFinalWait(MTS_TASK_TCB *tcb)
{
	if (GmMainDatReleaseBossBattleReleaseCheck()) {
		// 解放処理終了
		GmGameDatReleaseBossBattleExit();
		// 通常解放処理へ
		mtTaskChangeTcbProcedure(tcb, gmMainObjectReleaseWait);

		// オブジェクトシステム終了
		g_obj.flag |= OBD_OBJ_SAVE_DATAWORK;	// データワーク退避
		ObjExit();
	}
}

// ==========================================================================
// gmMainObjectReleaseWait
/*!
 *	データ開放待機 終了処理待ち
 */
// ==========================================================================
void gmMainObjectReleaseWait(MTS_TASK_TCB *tcb)
{
	if (!ObjObjectCheckClearAllObject()) {
		// オブジェクトの終了待ち
		return;
	}

	if (ObjIsExitWait()) {
		// オブジェクトシステムの終了待ち
		return;
	}

	// リスタート用 データフラッシュ
	GmGameDatFlashRestart();

	// 終了待機破棄
	mtTaskClearTcb(tcb);

	// 次のイベントへ
	SyChangeNextEvt();
}

// ==========================================================================
// gmMainObjectReleaseDest
/*!
 *	データ開放デストラクタ
 */
// ==========================================================================
void gmMainObjectReleaseDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	gm_main_release_wait_tcb = NULL;
}



// ==========================================================================
// 初期化
// ==========================================================================
// ==========================================================================
// ゲームスタート
// ==========================================================================
// ==========================================================================
// gmMainGameStart
/*!
 *	ゲーム開始
 */
// ==========================================================================
void gmMainGameStart(void)
{
	bool mppCheckPointDataLoaded = false;
	//sss
	mppUtil::disableSpeedAchievementAfterTIMEOVER(false);
	if(mppCheckPointStorage::isStateExist()) {
		if(mppCheckPointStorage::isStateCompatibleWithCurrentGame()) {
			if(mppCheckPointStorage::loadState()) {
				mppCheckPointDataLoaded = true;
			}
		}
	}	
	
	
	s32	i;
	BOOL	is_time_reset	= FALSE;	//!< タイマリセット済みフラグ
	
#if _IPHONE
	
	//sss
	mppCheckPointStorage::postInitializeStateIfNecessary1_inputAndSound();
	
	// パッドエミュ対応(初期化)
	// タッチ入力初期化
	amIPhoneTouchCanceled(NULL, NULL, NULL);
	// VirtualPad
	gm::CPadVirtualPad &virtual_pad = gm::CPadVirtualPad::CreateInstance();
	{
		// Aタイプ
		float rect_a[4] = {-120.0f, 166.0f, 232.0f, 340.0f}; // add 20.0f
		//float rect_a[4] = {-120.0f-20, 166.0f-20, 232.0f+20, 340.0f+20}; // add 20.0f //test
		virtual_pad.Create(rect_a);
	}
	
	// Polar
	gm::CPadPolarHandle &polar = gm::CPadPolarHandle::CreateInstance();
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_2) {
		polar.Create(0.0f, 0.0f, AMD_SCREEN_2D_WIDTH * 4 / 5, AMD_SCREEN_2D_HEIGHT);
	}
	else {
		polar.Create();
	}
	polar.SetValue(0.0f);
	g_gm_main_system.polar_now  = 0;
	g_gm_main_system.polar_diff = 0;
	
	// 操作によるスリープ/加速度センサー処理の変更
	if (!(GsGetMainSysInfo()->game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC)) {
		// 傾斜操作ならばスリープ無効
		GsMainSysSetSleepFlag(FALSE);
		// 加速度センサー有効
		GsMainSysSetAccelFlag(TRUE);
	}
	else if ((!(GsGetMainSysInfo()->game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK)) && (GsGetMainSysInfo()->stage_id == GSD_MAIN_STAGE_ID_3_2 || GSM_MAIN_STAGE_IS_SPSTAGE())) {
		// フリック無効かつ3-2またはSS ならばスリープ無効
		GsMainSysSetSleepFlag(FALSE);
		// 加速度センサー有効
		GsMainSysSetAccelFlag(TRUE);
	}
	else {
		// それ以外は画面タッチで動作するのでスリープ有効
		GsMainSysSetSleepFlag(TRUE);
		// 加速度センサー無効
		GsMainSysSetAccelFlag(FALSE);
	}
#endif // _IPHONE
	
	/* パッド振動初期化 */
	GmPadVibInit();

    //sss[TIMEOVER]	
	// タイムオーバーからの復帰ゲームはタイムをクリア
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_TIMEOVER) {
		g_gm_main_system.game_time = 0;
		is_time_reset	= TRUE;
		mppUtil::disableSpeedAchievementAfterTIMEOVER(true);		
	}

	// ゲーム開始時 不要フラグクリア
	g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_START_CLEAR_MASK;

	// ステータス初期化
	g_gm_main_system.die_event_wait_time = 0;	// プレイヤー死亡時カウンタ
	g_gm_main_system.pseudofall_dir = 0;		// 擬似重力クリア
	//if(!mppCheckPointDataLoaded) 
	{ //qqq
		g_gm_main_system.boss_load_no = -1;			// ファイナルステージ用ボスデータロード状況
	}

	// メインシステム
	g_gm_main_system.pre_tcb = MTM_TASK_MAKE_TCB(gmMainPre, NULL,
									0, GMD_TASK_PAUSELEVEL_DEF, GMD_TASK_PRIO_MAIN_PRE, GMD_TASK_GROUP_MAIN_PRE,
									0, "GM_MAIN_PRE");
	g_gm_main_system.post_tcb = MTM_TASK_MAKE_TCB(gmMainPost, NULL,
									0, GMD_TASK_PAUSELEVEL_DEF, GMD_TASK_PRIO_MAIN_POST, GMD_TASK_GROUP_MAIN_POST,
									0, "GM_MAIN_POST");

	// オブジェクトシステム初期化
	//ObjInit(GMD_TASK_GROUP_OBJSYS, GMD_TASK_PRIO_OBJSYS);		// GmMainInit で初期化
	//ObjDataAlloc(GMD_DWORK_NO_MAX);	// データワーク数
#if 0
	/* オブジェクトパケットシステム初期化 */
	if (GsGetMainSysInfo()->game_flag & GSD_GAME_FLAG_WIFI) {
		// WiFi
		ObjPacketInit(NULL, OBD_PACKET_INIT_TYPE_WIFI, GMD_GAMEMAIN_CONTEST_WIFI_SEND_SIZE);
	}
	else {
		// ワイヤレス
		ObjPacketInit(NULL, OBD_PACKET_INIT_TYPE_WIRELESS, WH_DS_DATA_SIZE);
	}
#endif
#if defined _DS
	ObjObjectSetLightNum(3);						// ライト数
	//ObjObjectSetVramMapMode(OBD_OBJ_VRAM_MMODE_64);	// オブジェクトVRAMマッピングモード
#endif

	g_obj.flag = OBD_OBJ_CAMERA | OBD_OBJ_COL_DIFF | OBD_OBJ_RECT | OBD_OBJ_RECT_NOUSE_DRAWSCALE/* | OBD_OBJ_HS_FUNC_STOP*/;

	g_obj.ppPre			= GmObjPreFunc;				// システム前処理
	g_obj.ppPost		= NULL;						// システム後処理
	//g_obj.ppDrawSort	= GmObjDrawSort;			// 描画前オブジェクトソート
	g_obj.ppCollision	= GmObjCollision;			// あたり処理
	g_obj.ppObjPre		= GmObjObjPreFunc;			// オブジェクト共通前処理
	g_obj.ppObjPost		= GmObjObjPostFunc;			// オブジェクト共通後処理
	g_obj.ppRegRecAuto	= GmObjRegistRectAuto;		// 矩形自動登録処理

	// 描画スケール設定
	g_obj.draw_scale.x = g_obj.draw_scale.y = g_obj.draw_scale.z = GMD_OBJ_DRAW_SCALE_FX;
	// 逆数保存
	g_obj.inv_draw_scale.x = g_obj.inv_draw_scale.y = g_obj.inv_draw_scale.z = FX_Div(FX32_ONE, g_obj.draw_scale.x);
	// 画面奥行き度
	g_obj.depth = 0x0080;

	// オブジェクトシステムデバック用設定
#if	defined (MTD_DEBUG)
	g_obj.flag |= OBD_OBJ_RECT_D_NOVRAM_B;
#endif // #if	defined (MTD_DEBUG)
	ObjDebugRectActionInit();		// デバック矩形表示
	//ObjDebugSetRectDispGroup(GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER | GMD_OBJ_RECT_TARGET_GROUPFLAG_ENEMY);
									// 表示矩形グループ設定 OBD_RECT_TARGET_G_FLAG_***

	/* ライト初期化 */
	gmMainInitLight();


#if 0
	/* キー状態初期化 */
	mtKeyPadInitKeyStatus(&g_gm_main_system.key_status, NULL, NULL);
	mtKeyPadInitKeyStatus(&g_gm_main_system.ply_key_status, NULL, NULL);

	/* キープレイ用意 */
	if (g_gs_main_sys_info.game_mode == GSD_GAME_MODE_PLAYDEMO) {
		u32		size;

		g_gm_main_system.playdemo_key_dat = GmPlayDemoGetKeyFileData(&size);

		ObjKeyRecordDetailInit(OBD_KEYRECORD_TYPE_BUFFER_PLAY, &g_gm_main_system.key_status,
							NULL, g_gm_main_system.playdemo_key_dat, size, GMD_TASK_NO_CUTIN_SYS_PAUSE, GMD_TASK_PRIO_KEYREC_PLAY);

		// プレイデモマネージャー初期化
		GmPlayDemoMgrInit();

		// タイマカウント開始
		//g_gm_main_system.flag |= GMD_MAIN_FLAG_COUNT_TIME;

		// 白フェード速度変更
		//EfFadeInit(EFD_FADE_WHITE_IN, EFD_FADE_SPD >> 1);

		// キープレイ中フラグ設定
		g_gm_main_system.game_flag2 |= GMD_MAIN_GAME_FLAG2_KEY_PLAY;
	}
#if defined (MTD_DEBUG)
	else if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_KEY_SAVE) {
	// デバックキー保存開始
		if (ObjKeyRecordStateGet() == OBD_KEYRECORD_NONE) {
			// 新規
			ObjKeyRecordDetailInit(OBD_KEYRECORD_TYPE_DEBUG_REC, &_mt_pad_key, NULL,
					NULL, 0,
					GMD_TASK_NO_CUTIN_SYS_PAUSE, GMD_TASK_PRIO_KEYREC_REC);
		}
		else {
			ObjKeyRecordStateSet(OBD_KEYRECORD_REC);
		}
	}
	// デバックキー再生用意
	else if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_KEY_PLAY) {
		if (!ObjKeyRecordDebugRecCheck()) {
			// ファイルから
			MTM_ASSERT(0);
		}
		else {
			// メモリにデータあり
		//	ObjKeyRecordDetailInit(OBD_KEYRECORD_TYPE_DEBUG_PLAY, &_mt_pad_key, NULL,
		//						NULL, 0,
		//						GMD_TASK_PAUSELEVEL_DEF, GMD_TASK_PRIO_KEYREC_PLAY);
			ObjKeyRecordDetailInit(OBD_KEYRECORD_TYPE_DEBUG_PLAY, &g_gm_main_system.key_status, NULL,
								NULL, 0,
								GMD_TASK_NO_CUTIN_SYS_PAUSE, GMD_TASK_PRIO_KEYREC_PLAY);
		}

		// キープレイ中フラグ設定
		g_gm_main_system.game_flag2 |= GMD_MAIN_GAME_FLAG2_KEY_PLAY;
	}
#endif
#endif // #if 0

//	// ステージ初期化
//	//GmStageInit(0/*◆とりあえず*/);

	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_RESTART) {
		// ゲーム死亡による再開時はタイマー復帰
		g_gm_main_system.game_time = g_gm_main_system.time_save;
		
		// 実績用
		// ステージの最初から開始・復帰した場合はダメージカウントクリア
		if (g_gm_main_system.marker_pri == 0) {
			g_gm_main_system.ply_dmg_count	= 0;
			g_gm_main_system.game_flag	&= ~GMD_GAME_FLAG_RESUMED_FROM_MARKER;
		}
		else {
			// ポイントマーカーから開始したことを記録
			g_gm_main_system.game_flag	|= GMD_GAME_FLAG_RESUMED_FROM_MARKER;
			
			if (is_time_reset) {
				// ポイントマーカーからの再開時にタイマがリセットされたことを記録
				g_gs_main_sys_info.game_flag	|= GSD_MAINSYS_GAME_FLAG_TIME_RESET_AT_MARKER;
			}
		}
	}

	// マップ初期化
    GmMapInit();

#if _IPHONE
	// TVX描画初期化
	GmTvxInit();
#endif // _IPHONE

	// 遠景初期化
#if GMD_MAP_FAR_TEST
	GmMapFarInit();
#endif //GMD_MAP_FAR_TEST

	// 装飾初期化
#if GMD_DECO_TEST
	GmDecoInit();
#endif //GMD_DECO_TEST

	// 水面初期化
	GmWaterSurfaceInit();

	// エフェクト初期化
	//GmEffectInit();

	// サウンド初期化
	//GmSoundInit();

	// 軌跡エフェクトシステム初期化
	GmPlyEfctTrailSysInit();

#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	// FIX初期化
	GmFixInit();
#endif /* !defined(GMD_DEBUG_NO_CREATE_COCKPIT) */

	// カメラ初期化
	GmCameraInit();

	// 移動限界値取得
	//GmMainResetMoveLimit();

	// エリア範囲取得
	//GmMainResetAreaLimit();

	// サウンド初期化
	GmSoundInit();

	// リング初期化
	GmRingInit();

	// イベントマネージャー初期化
	GmEventMgrInit();

	// イベントマネージャースタート
	GmEventMgrStart();

	// プレイヤー生成
	for (i = 0; i < GSD_MAIN_PLAYER_MAX; i++) {
		if (g_gs_main_sys_info.char_id[i] == GSD_CHAR_ID_INVALID) {
			MTM_ASSERT(i != GSD_MAIN_PLAYER_1P);
			continue;
		}
#if !_IPHONE
		g_gm_main_system.ply_work[i] =
				GmPlayerInit(g_gs_main_sys_info.char_id[i],
					(u16)AoAccountGetCurrentId()/*ctrl_id*/, (u16)i/*player_id*/, 0/*camera_id*/);
#else // !_IPHONE
		g_gm_main_system.ply_work[i] =
				GmPlayerInit(g_gs_main_sys_info.char_id[i],
					(u16)0/*ctrl_id*/, (u16)i/*player_id*/, 0/*camera_id*/);
#endif // !_IPHONE
	}
	
	//sss
	mppCheckPointStorage::postInitializeStateIfNecessary2_player();	
	
	/*sss[101][105]
	{
		mpp_flushAchievementsToGlobalNet(false); //on start
		mppUtil::sendTotalTimeAttackTime();//on start
	}*/

	// イベント開始位置生成
	GmEveMgrCreateStateEvent();

	// カメラターゲット設定
	//GmCameraSetTargetObject(MTE_LCD_UP, (OBS_OBJECT_WORK*)g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]);		// OBD_OBJ_CAMERA_STICK 時は UP をメインターゲットに
	//GmCameraSetTargetObject(MTE_LCD_DOWN, (OBS_OBJECT_WORK*)g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]);
	// カメラ初期位置設定
	//GmCameraSetPosByTargetObj();


	// スタート位置設定後カメラ位置再設定等
	// カメラ位置再設定
	//GmCameraSetTargetLCDByTargetObj();
	//GmCameraSetPosByTargetObj();
	// 現在のカメラ位置でオブジェクト生成
	//GmEveMgrSearchEventLcdDS(GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE);
	

	// アナウンス終了までオブジェクトを停止
//	gmMainStartObjStop();

	// サウンド初期化
	//GmSoundPostInit();

	// 開始時はプレイヤー操作不能.
	//  開始デモ内で解除する
	//g_gm_main_ply_obj->player_flag	|= GMD_PLF_NOKEY;
	
	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_RESTART) {
		if (g_gm_main_system.marker_pri > 0) {
			mppCheckPointStorage::saveState(mppCheckPointStorage::SAVE_AFTER_RESPAWN);//sss
		}
	}
	else {
		if(mppCheckPointDataLoaded == false ) {//sss
			mppAchievementSupport::get()->event_LoseRingFinalLevelCounterClear(false); //for new level only (no respawn, no restore)
		}
	}

#if _IPHONE
	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_ENDING) {	// Wii HBM対応のためエンディングのみ別途BGM呼び出し
		// BGMをここで再生すると描画のせいで音が止まったりするので、
		// ここではBGM再生フラグを初期化するのみ
		g_gm_main_system.game_flag	|= GMD_GAME_FLAG_BGM_PLAY_WAIT;
		g_gm_main_system.game_flag	&= ~GMD_GAME_FLAG_BGM_PLAY_ENABLE;
	}
#else
	// BGM再生
	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_ENDING) {	// Wii HBM対応のためエンディングのみ別途BGM呼び出し
		GmSoundPlayStageBGM(0);
	}
#endif // !_IPHONE

	// 開始デモ呼び出し？

#if 0
	// ◆暫定ライト
	// ライトの初期設定
	//   全てのライトの初期化
	nnInitLight();
	//   環境光 RGB
	nnSetAmbientColor(0.8f, 0.8f, 0.8f);

	//   ライト０初期設定
	nnSetUpParallelLight(&gm_test_light, &gm_test_light_col, gm_test_light_intensity, &gm_test_light_dir);
	nnSetLight(NNE_LIGHT_0, &gm_test_light, NND_LIGHTTYPE_PARALLEL);	// パラレルライト
	nnSetLightSwitch(NNE_LIGHT_0, NNE_ON);

#if _WII
	// Wii用トゥーンライト
	_am_draw_toonDir = gm_test_light_dir;
#endif
#endif


	// 画面表示
	// フェードアウトはどうするか？

	/* 同期タイマ開始 */
	/* 敵シーケンス待機 */
//	g_gm_main_system.game_flag2 |= GMD_MAIN_GAME_FLAG2_COUNT_SYNC_TIME |
//									GMD_MAIN_GAME_FLAG2_ENE_SEQ_WAIT;
#if 0
	/*** 通信 ***/
	if ((main_info->game_flag & GSD_GAME_FLAG_WM) &&
			g_gm_main_system.ene_packet == NULL) {
		// 敵用パケットバッファ取得
		g_gm_main_system.ene_packet = mtMemAllocMain(GMD_GAMEMAIN_ENEMY_PACKET_BUF_NUM * sizeof(GMS_ENEMY_PACKET));
		MI_CpuClear8((void*)g_gm_main_system.ene_packet, GMD_GAMEMAIN_ENEMY_PACKET_BUF_NUM * sizeof(GMS_ENEMY_PACKET));
		g_gm_main_system.ene_packet_num = 0;
	}
#endif

	// ゲーム同期タイマー開始
	g_gm_main_system.game_flag &= ~(GMD_GAME_FLAG_COUNT_SYNC_TIME | GMD_GAME_FLAG_COUNT_GAME_TIME);
	g_gm_main_system.game_flag |= GMD_GAME_FLAG_COUNT_SYNC_TIME;

	if (GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// スペシャルステージ開始演出
		GmSplStageStart();
	} else if (g_gs_main_sys_info.game_mode == GSD_GAME_MODE_ENDING) {
		// エンディング開始
		GmEndingStart();
	} else {
		// 一般ステージスタート演出
//		if (g_gm_main_system.marker_pri == 0) {	// チェックポイントからの再開の時はスタートデモを行わない
#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
			GmStartDemoStart();
#else
			//フェード終了
			IzFadeExit();

			// ゲームタイマー開始
			g_gm_main_system.game_flag |= GMD_GAME_FLAG_COUNT_GAME_TIME;
#endif /* defined(GMD_DEBUG_NO_CREATE_COCKPIT) */
//		}
//		else {
//			// ゲームタイマー開始
//			g_gm_main_system.game_flag |= GMD_GAME_FLAG_COUNT_GAME_TIME;
//
//			// フェードイン開始
//			IzFadeInitEasy(IZE_FADE_SET_TYPE_TAKEOEVER,
//							IZE_FADE_TYPE_BLACK_FADEIN,
//							30.f);
//		}
	}
}

// ==========================================================================
// ライト設定
// ==========================================================================
// ==========================================================================
// gmMainInitLight
/*!
 *	ライト初期化
 */
// ==========================================================================
void gmMainInitLight(void)
{
	/* ライト割り当て */

	// NNE_LIGHT_0 : 標準ライト(固定)
	// NNE_LIGHT_1 : Zone1崩れる足場, Zone4歯車, スペステ用ライト, ファイナルボス足場用
	// NNE_LIGHT_2 : Zone1崩れる足場, Final窓用ハッチ装飾ライト, Zone4針
	// NNE_LIGHT_3 : 
	// NNE_LIGHT_4 : マップ用EXライト(固定)
	// NNE_LIGHT_5 : マップ用ライト(固定)
	// NNE_LIGHT_6 : プレイヤーライト(固定)
	// NNE_LIGHT_7 : Wii用スペキュラーGCライト(固定)

	/* ライト */
#if !GMD_DEBUG_LIGTH_MULTI	// - - - - - - - - - - - - - - - - - - - - - - - - -
	NNS_RGBA	light_col = {
		1.0f, 1.0f, 1.0f, 1.0f,
	};

	NNS_VECTOR	light_vec;
	float	intensity;
	
	// 標準使用ライト
	// GmMainInit で設定
//#if !_WII
//	g_obj.def_user_light_flag = OBD_LIGHT_USE_FLAG_0;
//#else
//	g_obj.def_user_light_flag = OBD_LIGHT_USE_FLAG_0 | OBD_LIGHT_USE_FLAG_7/*スペキュラ*/;
//#endif


	// アンビエントカラー
	if (  (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3) 
		||(g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_BOSS)) {
		// Zone1-3のみ夕焼け処理
		g_obj.ambient_color.r = 1.0f;
		g_obj.ambient_color.g = 0.0f;
		g_obj.ambient_color.b = 0.0f;
#if GMD_MAIN_DEBUG_COLOR
		dbg_color[0] = 0xff;
		dbg_color[1] = 0x00;
		dbg_color[2] = 0x00;
#endif // GMD_MAIN_DEBUG_COLOR
	}
	else if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		// Zone4-3のみ暗く
		g_obj.ambient_color.r = 0.1f;
		g_obj.ambient_color.g = 0.1f;
		g_obj.ambient_color.b = 0.1f;
#if GMD_MAIN_DEBUG_COLOR
		dbg_color[0] = 0x19;
		dbg_color[1] = 0x19;
		dbg_color[2] = 0x19;
#endif // GMD_MAIN_DEBUG_COLOR
	} else {
		g_obj.ambient_color.r = 0.8f;
		g_obj.ambient_color.g = 0.8f;
		g_obj.ambient_color.b = 0.8f;
#if GMD_MAIN_DEBUG_COLOR
		dbg_color[0] = 0xcc;
		dbg_color[1] = 0xcc;
		dbg_color[2] = 0xcc;
#endif // GMD_MAIN_DEBUG_COLOR
	}

	// パラレルライト
	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_4) {
		// ZONE4
		light_vec.x = -0.95f;
		light_vec.y = 0.25f;
		light_vec.z = -1.0f;
#if _PS3
		if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_BOSS) {
			light_vec.x = -2.0f;
			light_vec.y = 0.9f;
			light_vec.z = -1.5f;
		}
#elif _XBOX
		if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_BOSS) {
			light_vec.x = -2.6f;
			light_vec.y = 0.8f;
			light_vec.z = -2.25f;
		}
#endif
	}
	else if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_FINAL_1) {
#if _PS3
		light_vec.x = -1.8f;
		light_vec.y = -1.9f;
		light_vec.z = -0.3f;
#elif _XBOX
		light_vec.x = -1.5f;
		light_vec.y = -1.6f;
		light_vec.z = -0.2f;
//#elif _IPHONE
//		light_vec.x = -1.5f;
//		light_vec.y = -1.6f;
//		light_vec.z = -0.2f;
#else
		light_vec.x = -1.0f;
		light_vec.y = -1.0f;
		light_vec.z = -1.0f;
#endif
	}
	else {
#if _IPHONE
		light_vec.x =  0.0f;
		light_vec.y =  0.0f;
		light_vec.z = -1.0f;
#else
		light_vec.x = -1.0f;
		light_vec.y = -1.0f;
		light_vec.z = -1.0f;
#endif // _IPHONE
	}
	nnNormalizeVector(&light_vec, &light_vec);
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		// Zone4-3のみ暗く
		intensity = GMD_LIGHT_CMN_DARK_INTENSITY;
	} else {
		intensity = GMD_LIGHT_COMN_INTENSITY;
	}
	ObjDrawSetParallelLight(NNE_LIGHT_0, &light_col, intensity, &light_vec);

	// 標準ライト設定を保存
	g_gm_main_system.def_light_vec = light_vec;
	g_gm_main_system.def_light_col = light_col;


	// プレイヤー用パラレルライト(NNE_LIGHT_6を使用)
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		// Zone4-3のみ暗く
		intensity = GMD_LIGHT_PLY_DARK_INTENSITY;
	} else {
		intensity = GMD_LIGHT_PLY_CMN_INTENSITY;
	}
#if _PS3
	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_1 ||
			GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_2 ||
			GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_3) {
		light_vec.x = -1.0f;
		light_vec.y = -3.399f;
		light_vec.z = -1.1f;
		nnNormalizeVector(&light_vec, &light_vec);
	}
#elif _IPHONE
	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_4) {
		light_vec.x = +0.05f;
		light_vec.y = +0.15f;
		light_vec.z = -0.05f;
	}
	else {
		light_vec.x = -0.50f;
		light_vec.y = -0.40f;
		light_vec.z = -0.25f;
	}
	nnNormalizeVector(&light_vec, &light_vec);
#endif
	ObjDrawSetParallelLight(NNE_LIGHT_6, &light_col, intensity, &light_vec);

	// プレイヤーライト設定を保存
	g_gm_main_system.ply_light_vec = light_vec;
	g_gm_main_system.ply_light_col = light_col;

#if _WII
	// Wii用スペキュラーGCライト (NNE_LIGHT_7を使用)
	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_4) {
		// Zone4
		light_col.r = 0.5f;
		light_col.g = 0.5f;
		light_col.b = 0.5f;

		light_vec.x = -0.125f;
		light_vec.y = 0.125f;
		light_vec.z = -1.f;
	}
	else {
		// 通常
		light_col.r = 1.f;
		light_col.g = 1.f;
		light_col.b = 1.f;

		// light_vec はパラレルライトを調整
		light_vec.x /= 2.f;
		//light_vec.y = -1.f;
		light_vec.z = 0.f;
	}

	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetSpecularGCLight(NNE_LIGHT_7, &light_col, &light_vec);

	
#if 0
	// モデルビルド時に設定が必要な為
	// GmMainInit へ移動
	// Wii用トゥーンライト
	switch (GMM_MAIN_GET_ZONE_TYPE()) {
	case GSD_MAIN_ZONE_TYPE_1:
	case GSD_MAIN_ZONE_TYPE_2:
	case GSD_MAIN_ZONE_TYPE_3:
		light_vec.x = -0.9f;
		light_vec.y = -0.8f;
		light_vec.z = -1.0f;
		break;
	case GSD_MAIN_ZONE_TYPE_4:
		if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_BOSS) {
			light_vec.x = -0.5f;
			light_vec.y =  0.27f;
			light_vec.z = -1.0f;
		}
		else {
			light_vec.x = -0.6f;
			light_vec.y =  0.5f;
			light_vec.z = -1.0f;
		}
		break;
	default:
	case GSD_MAIN_STAGE_ID_FINAL_1:
		light_vec.x = -1.8f;
		light_vec.y = -2.5f;
		light_vec.z = -2.0f;
		break;
	}
	nnNormalizeVector(&light_vec, &light_vec);
	g_obj.toon_light_vec = light_vec;
#endif
#endif

	// マップ専用ライト NNE_LIGHT_5 を使用
	GmMapSetLight();	// 必要なステージのみ設定

	// その他オブジェクト別ライト
	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_1) {
	// 崩れる足場用(NNE_LIGHT_1, NNE_LIGHT_2を使用)
		GmGmkBreakLandSetLight();
	}
	else if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_4) {
		// 歯車用ライト(NNE_LIGHT_1を使用)
		GmGmkGearSetLight();
		
		// 針用ライト(NNE_LIGHT_2を使用)
		GmGmkNeedleSetLight();
	}
	else if (GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// スペステ用ライト(NNE_LIGHT_1を使用)
		GmSplStageSetLight();
	}
	else if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL) {
#if !SONIC4_TRIAL
		// ファイナルボス足場用ライト(NNE_LIGHT_1を使用)
		GmBoss5LandSetLight();
#endif // !SONIC4_TRIAL

		//窓用ハッチ装飾ライト(NNE_LIGHT_2を使用)
		GmDecoSetLightFinalZone();
	}

#else // #if GMD_DEBUG_LIGTH_MULTI// - - - - - - - - - - - - - - - - - - - - - - - - -

	// 平行光源テスト
	NNS_RGBA	light_col = {
		1.0f, 1.0f, 1.0f, 1.0f,
	};

	NNS_VECTOR	light_vec;

	// 標準使用ライト
	// GmMainInitで設定
//#if !_WII
//	g_obj.def_user_light_flag = OBD_LIGHT_USE_FLAG_0 | OBD_LIGHT_USE_FLAG_1 | OBD_LIGHT_USE_FLAG_2;
//#else
//	g_obj.def_user_light_flag = OBD_LIGHT_USE_FLAG_0 | OBD_LIGHT_USE_FLAG_1 | OBD_LIGHT_USE_FLAG_2 |
//							 BD_LIGHT_USE_FLAG_7/*スペキュラ*/;
//#endif

	// アンビエントカラー
	g_obj.ambient_color.r = 0.2f;
	g_obj.ambient_color.g = 0.2f;
	g_obj.ambient_color.b = 0.2f;

	// パラレルライト1
	light_vec.x = -1.0f;
	light_vec.y = 1.0f;
	light_vec.z = -1.0f;
	light_col.r = 1.f;
	light_col.g = 0.f;
	light_col.b = 0.f;
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_0, &light_col, 1.f, &light_vec);

#if _WII
	// Wii用トゥーンライト
	g_obj.toon_light_vec = light_vec;
#endif

	// パラレルライト2
	light_vec.x = 1.0f;
	light_vec.y = 0.0f;
	light_vec.z = -1.0f;
	light_col.r = 0.f;
	light_col.g = 1.f;
	light_col.b = 0.f;
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_1, &light_col, 1.f, &light_vec);

	// パラレルライト3
	light_vec.x = 0.0f;
	light_vec.y = -1.0f;
	light_vec.z = -1.0f;
	light_col.r = 0.f;
	light_col.g = 0.f;
	light_col.b = 1.f;
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_2, &light_col, 1.f, &light_vec);

#endif // #if GMD_DEBUG_LIGTH_MULTI// - - - - - - - - - - - - - - - - - - - - - - - - -
}

// ==========================================================================
// メイン処理
// ==========================================================================
// ==========================================================================
// gmMainVFunc
/*!
 */
// ==========================================================================
//void gmMainVFunc(void)
//{
//}

// ==========================================================================
// gmMainPre
/*!
 *	ゲーム 前処理
 */
// ==========================================================================
//		static NNS_RGBA		test_light_col = {
//			0.65f, 0.65f, 0.65f, 1.0f,
//		};
//		static NNS_VECTOR	test_light_vec = {
//			0.5f, 0.05f, -1.0f,
//		};
void gmMainPre(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

#if OBD_USE_ACTION3D_SS
	mtActSetupSS();
#endif // #if OBD_USE_ACTION3D_SS
#if OBD_USE_ACTION3D_SMA
	mtSmaSetCamera();
#endif // #if OBD_USE_ACTION3D_SMA

	/* 通信 */

	// 体押し合い登録状況クリア
	//GmHitSetBodyPushRegistClear();

	// カメラユーザーオフセットクリア
	//GmCameraClearUesrOffset();

#if _IPHONE
	// サスペンド情報更新
	gmMainUpdateSuspendedPause();

	GMS_MAIN_SYSTEM *main = &g_gm_main_system;

	// BGM再生
	if (main->game_flag & GMD_GAME_FLAG_BGM_PLAY_ENABLE) {
		g_gm_main_system.game_flag	&= ~GMD_GAME_FLAG_BGM_PLAY_ENABLE;
		if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_ENDING) {	// Wii HBM対応のためエンディングのみ別途BGM呼び出し
			GmSoundPlayStageBGM(0);
		}
	}

	// パッドエミュ対応(更新)
	// VirtualPad
	gm::CPadVirtualPad &virtual_pad = gm::CPadVirtualPad::CreateInstance();
	virtual_pad.Update();
	// Polar
	gm::CPadPolarHandle &polar = gm::CPadPolarHandle::CreateInstance();
	polar.Update();
	Angle32 prev = main->polar_now;
	main->polar_now  = polar.GetValue<Angle32>();
	main->polar_diff = main->polar_now - prev;
	
	// カメラ
	if (gmMainIsUseWaitUpCamera()) {
#if defined(MTD_DEBUG)
		if (main->debug_flag & GMD_GAME_DEBUG_FLAG_UP_SCREEN) {
			// デバッグ強制ズーム
			if (main->camscale_state == GMD_MAIN_CAMSCALE_STATE_NON) {
				main->camscale_state = GMD_MAIN_CAMSCALE_STATE_ZOOM;
			}
		} else 
#endif // defined(MTD_DEBUG)
		if (GmPlayerIsStateWait(main->ply_work[GSD_MAIN_PLAYER_1P])) {
			// 待機中カメラアップ
			if (main->camscale_state == GMD_MAIN_CAMSCALE_STATE_NON) {
				main->camscale_state = GMD_MAIN_CAMSCALE_STATE_ZOOM;
			}
		}
		else {
			// 通常カメラ
			main->camscale_state = GMD_MAIN_CAMSCALE_STATE_NON;
			main->camera_scale = GMD_CAMERA_SCALE;
		}
		
		
		if (main->camscale_state == GMD_MAIN_CAMSCALE_STATE_ZOOM) {
			main->camera_scale -= GMD_CAMERA_UP_SCALE_ADD;
			if (main->camera_scale < GMD_CAMERA_UP_SCALE_MAX) {
				main->camera_scale = GMD_CAMERA_UP_SCALE_MAX;
				main->camscale_state = GMD_MAIN_CAMSCALE_STATE_UP;
			}
		}
		for (int i = 0; i < GME_CAMERA_NO_MAX; i++) {
			OBS_CAMERA* obj_camera = ObjCameraGet(i);
			if (obj_camera) {
				obj_camera->scale = main->camera_scale;
			}
		}
	}
#endif // _IPHONE
	
	// その他前処理
#if defined (MTD_DEBUG)
//	{
//		// テスト
//		NNS_VECTOR	light_vec;
//		nnNormalizeVector(&light_vec, &test_light_vec);
//		ObjDrawSetParallelLight(NNE_LIGHT_1, &test_light_col, 1.f, &light_vec);
//	}
	
#if GMD_MAIN_DEBUG_LIGHT
	// ライトテスト
	{
		static NNS_RGBA	light_test_col = {
			1.0f, 1.0f, 1.0f, 1.0f,
		};

		static NNS_VECTOR	light_test_vec = {-1.0f, -1.0f, -1.0f};
		NNS_VECTOR	light_test_vec_temp;
		static float	light_test_inten = 1.0f;

#if _IPHONE
#if GMD_MAIN_DEBUG_COLOR
#define ADD_INDEX  (11)
#define ALL_INDEX  (12)
#else
#define ADD_INDEX  (3)
#define ALL_INDEX  (4)
#endif // GMD_MAIN_DEBUG_COLOR
		int add = 0;
		float add_f = 0.0f;
		static int light_test_index = 0;
		if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_DEBUG_PAUSE) {
			if (PAD_STAND(0) & KEY_L_LEFT) {
				light_test_index += ADD_INDEX;
			}
			if (PAD_STAND(0) & KEY_L_RIGHT) {
				light_test_index += (ALL_INDEX - ADD_INDEX);
			}
			light_test_index %= ALL_INDEX;
			
			if (PAD_STAND(0) & KEY_L_UP) {
				add   = 0x01;
				add_f = 0.05f;
			}
			if (PAD_STAND(0) & KEY_L_DOWN) {
				add   = 0xff;
				add_f = -0.05f;
			}
			
			switch (light_test_index) {
				case 0:
					light_test_vec.x += add_f;
					break;
				case 1:
					light_test_vec.y += add_f;
					break;
				case 2:
					light_test_vec.z += add_f;
					break;
				case 3:
					light_test_inten += add_f;
					break;
#if GMD_MAIN_DEBUG_COLOR
				case 4:
					dbg_color[0] = (dbg_color[0] + add) % 0x100;
					break;
				case 5:
					dbg_color[1] = (dbg_color[1] + add) % 0x100;
					break;
				case 6:
					dbg_color[2] = (dbg_color[2] + add) % 0x100;
					break;
				case 7:
					dbg_color[0] = (dbg_color[0] + add) % 0x100;
					dbg_color[1] = (dbg_color[1] + add) % 0x100;
					dbg_color[2] = (dbg_color[2] + add) % 0x100;
					break;
					
				case 8:
					dbg_color_def[0] = (dbg_color_def[0] + add) % 0x100;
					break;
				case 9:
					dbg_color_def[1] = (dbg_color_def[1] + add) % 0x100;
					break;
				case 10:
					dbg_color_def[2] = (dbg_color_def[2] + add) % 0x100;
					break;
				case 11:
					dbg_color_def[0] = (dbg_color_def[0] + add) % 0x100;
					dbg_color_def[1] = (dbg_color_def[1] + add) % 0x100;
					dbg_color_def[2] = (dbg_color_def[2] + add) % 0x100;
					break;
#endif // GMD_MAIN_DEBUG_COLOR
				default:
					break;
			}
			nnSetPrintSize(12.0f, 12.0f);
			amPrintColor(0xA0A0A0ff);
			amPrintf(3, 3, "x:%+1.2f/ y:%+1.2f/ z:%+1.2f/ int:%+1.2f", light_test_vec.x, light_test_vec.y, light_test_vec.z, light_test_inten);
#if GMD_MAIN_DEBUG_COLOR
			amPrintf(3, 4, "AMB R:%2x, G:%2x, B:%2x", dbg_color[0], dbg_color[1], dbg_color[2]);
			amPrintf(3, 5, "DIF R:%2x, G:%2x, B:%2x", dbg_color_def[0], dbg_color_def[1], dbg_color_def[2]);
#endif // GMD_MAIN_DEBUG_COLOR
			amPrintColor(0xffffffff);
			switch (light_test_index) {
				case 0:
					amPrintf(3, 3, "x:%+1.2f", light_test_vec.x);
					break;
				case 1:
					amPrintf(3, 3, "         y:%+1.2f", light_test_vec.y);
					break;
				case 2:
					amPrintf(3, 3, "                  z:%+1.2f", light_test_vec.z);
					break;
				case 3:
					amPrintf(3, 3, "                           int:%+1.2f", light_test_inten);
#if GMD_MAIN_DEBUG_COLOR
				case 4:
					amPrintf(3, 4, "AMB R:%2x", dbg_color[0]);
					break;
				case 5:
					amPrintf(3, 4, "AMB       G:%2x", dbg_color[1]);
					break;
				case 6:
					amPrintf(3, 4, "AMB             B:%2x", dbg_color[2]);
					break;
				case 7:
					amPrintf(3, 4, "AMB R:%2x, G:%2x, B:%2x", dbg_color[0], dbg_color[1], dbg_color[2]);
					break;
					
				case 8:
					amPrintf(3, 5, "DIF R:%2x", dbg_color_def[0]);
					break;
				case 9:
					amPrintf(3, 5, "DIF       G:%2x", dbg_color_def[1]);
					break;
				case 10:
					amPrintf(3, 5, "DIF             B:%2x", dbg_color_def[2]);
					break;
				case 11:
					amPrintf(3, 5, "DIF R:%2x, G:%2x, B:%2x", dbg_color_def[0], dbg_color_def[1], dbg_color_def[2]);
					break;
#endif // GMD_MAIN_DEBUG_COLOR
				default:
					break;
			}
		}
#else
		if (AoPadDirect() & KEY_R1) {
			if (AoPadStand() & KEY_L_LEFT) {
				light_test_vec.x += 0.05;
			}
			else if (AoPadStand() & KEY_L_RIGHT) {
				light_test_vec.x -= 0.05;
			}
			else if (AoPadStand() & KEY_L_UP) {
				light_test_vec.y -= 0.05;
			}
			else if (AoPadStand() & KEY_L_DOWN) {
				light_test_vec.y += 0.05;
			}
			else if (AoPadStand() & KEY_R_UP) {
				light_test_vec.z -= 0.05;
			} 
			else if (AoPadStand() & KEY_R_LEFT) {
				light_test_vec.z += 0.05;
			}
			if (AoPadDirect() & KEY_L1) {
				light_test_inten += 0.05f;
			}
		}
#endif // _IPHONE
		
#if GMD_MAIN_DEBUG_COLOR
		g_obj.ambient_color.r = (float)dbg_color[0] / 255.0f;
		g_obj.ambient_color.g = (float)dbg_color[1] / 255.0f;
		g_obj.ambient_color.b = (float)dbg_color[2] / 255.0f;
		light_test_col.r = (float)dbg_color_def[0] / 255.0f;
		light_test_col.g = (float)dbg_color_def[1] / 255.0f;
		light_test_col.b = (float)dbg_color_def[2] / 255.0f;
#endif // GMD_MAIN_DEBUG_COLOR
		nnNormalizeVector(&light_test_vec_temp, &light_test_vec);
		ObjDrawSetParallelLight(NNE_LIGHT_0, &light_test_col, light_test_inten, &light_test_vec_temp);
		ObjDrawSetParallelLight(NNE_LIGHT_6, &light_test_col, light_test_inten, &light_test_vec_temp);
//		ObjDrawSetParallelLight(NNE_LIGHT_2, &light_test_col, light_test_inten, &light_test_vec_temp);

#if _WII

#endif
	}
#if 0//_WII
	// スペキュラーGC設定
	{
		static NNS_RGBA	light_col = {
			1.0f, 1.0f, 1.0f, 1.0f,
		};
		static NNS_VECTOR	debug_light_vec = {-1.f, -1.f, -1.f};
		NNS_VECTOR	light_vec;

		if (AoPadDirect() & KEY_R1) {
			if (AoPadStand() & KEY_L_LEFT) {
				debug_light_vec.x += 0.05;
			}
			else if (AoPadStand() & KEY_L_RIGHT) {
				debug_light_vec.x -= 0.05;
			}
			else if (AoPadStand() & KEY_L_UP) {
				debug_light_vec.y -= 0.05;
			}
			else if (AoPadStand() & KEY_L_DOWN) {
				debug_light_vec.y += 0.05;
			}
			else if (AoPadStand() & KEY_R_UP) {
				debug_light_vec.z -= 0.05;
			}
			else if (AoPadStand() & KEY_R_LEFT) {
				debug_light_vec.z += 0.05;
			}
		}

	//	light_vec.x = 0.4f;
	//	light_vec.y = 0.0f;
	//	light_vec.z = -1.0f;
		nnNormalizeVector(&light_vec, &debug_light_vec);
		ObjDrawSetSpecularGCLight(NNE_LIGHT_7, &light_col, &light_vec);

		if (g_gm_main_system.ply_work[0]) {
			g_gm_main_system.ply_work[0]->obj_work.dir.y += 0x80;
		}
	}
#endif	// #if _WII
#endif
#endif // #if defined (MTD_DEBUG)
}

// ==========================================================================
// gmMainPost
/*!
 *	ゲーム 後処理
 */
// ==========================================================================
void gmMainPost(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

#if _IPHONE
	dbg::CPadEmu &pad_emu = dbg::CPadEmu::CreateInstance();
#endif

	/* 2D描画発行 */

	// 地形チェック開始(1フレームチェックなしにする為ここでONに)
	// g_obj.flag |= OBD_OBJ_COLMAP;

	/* 通信 */

	/* ゲーム終了チェック */
	// 暫定終了処理
//	if (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->player_flag & GMD_PLF_DIE) {
//		// プレイヤー死亡中
//		g_gm_main_system.debug_ply_die_time_cnt++;
//	}

	/* プレイヤー死亡チェック◆暫定 */
#if 1
	if ((g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->player_flag & GMD_PLF_DIE) &&
				(g_gm_main_system.die_event_wait_time < GMD_MAIN_GOVER_SWAIT_TIME*FX32_ONE) &&
		!(g_gm_main_system.game_flag & GMD_GAME_FLAG_PAUSE_DEMO)) {
#else
	if (!(g_gm_main_system.game_flag & (GMD_GAME_FLAG_GAMEOVER_START | GMD_GAME_FLAG_GAMESYS_RESTART)) &&
			g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->player_flag & GMD_PLF_DIE) {
#endif
		// プレイヤー死亡中
		g_gm_main_system.die_event_wait_time = ObjTimeCountUp(g_gm_main_system.die_event_wait_time);
		if (g_gm_main_system.die_event_wait_time >= GMD_MAIN_GOVER_SWAIT_TIME*FX32_ONE) {
			if (g_gs_main_sys_info.game_mode == GSD_GAME_MODE_TIME_ATTACK) {
			// タイムアタック
				GmClearDemoRetryStart();
			}
			else {
			// 通常
#if SONIC4_TRIAL
				g_trial_game_time += g_gm_main_system.game_time;
#endif
				// 残機数を減らす
				g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P]--;
				if ((s32)g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P] < 0) {
					g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P] = 0;
				}

				if (g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P] == 0) {
					// ゲームオーバー演出開始
					g_gm_main_system.game_flag |= GMD_GAME_FLAG_GAMEOVER_START;
					GmOverStart(GME_OVER_TYPE_GAMEOVER);

					// ゲームオーバージングル(BGM OFF込み)
					GmSoundPlayGameOver();
				}
				else {
					// 残機あり
					// リスタートに
					g_gm_main_system.game_flag |= GMD_GAME_FLAG_GAMESYS_RESTART;

					// リスタート発生を保存
					g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_RESTART;

					if (g_gm_main_system.game_flag & GMD_GAME_FLAG_TIMEOVER) {
						g_gm_main_system.game_flag |= GMD_GAME_FLAG_GAMEOVER_START;
						// タイムオーバー
						GmOverStart(GME_OVER_TYPE_TIMEOVER);
					}
					else {
						// ゲームを復帰
						// フェードアウト
						IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_BLACK_FADEOUT, 15.f);
					}
				}
			}
		}
	}	// プレイヤー死亡中チェック
#if SONIC4_TRIAL
	// 体験版は１分経過でタイムオーバーとする
	else
	if ((g_trial_game_time + g_gm_main_system.game_time >= 60 * 60 * 1)
	&& !(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->player_flag & GMD_PLF_DIE))
	{
		g_gm_main_system.game_time = 60 * 60 * 1 - 1 - g_trial_game_time;
		
		if (!(g_gm_main_system.game_flag & GMD_GAME_FLAG_TIMEOVER))
		{
			g_gm_main_system.die_event_wait_time = GMD_MAIN_GOVER_SWAIT_TIME*FX32_ONE;
			g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P] = 0;
			g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->player_flag |= GMD_PLF_DIE;
			g_gm_main_system.game_flag |= GMD_GAME_FLAG_TIMEOVER;
			g_gm_main_system.game_flag |= GMD_GAME_FLAG_GAMEOVER_START;
			// タイムオーバー
			GmOverStart(GME_OVER_TYPE_TIMEOVER);
			// Alertビュー表示
			Sonic4_TimeupAlertView();
		}
	}
#endif
		
	/* イベント移行チェック */
#if 1
	if ((g_gs_main_sys_info.game_mode == GSD_GAME_MODE_TIME_ATTACK &&			// タイムアタックモードでリトライ画面が終了
				(g_gm_main_system.game_flag & GMD_GAME_FLAG_RESULT_END)) ||

			(g_gs_main_sys_info.game_mode != GSD_GAME_MODE_TIME_ATTACK &&		// タイムアタック以外で
				((g_gm_main_system.game_flag & GMD_GAME_FLAG_GAMEOVER_END) ||	// ゲームオーバー, タイムオーバー
				(!(g_gm_main_system.game_flag & (GMD_GAME_FLAG_GAMEOVER_END | GMD_GAME_FLAG_TIMEOVER)) &&
						((g_gm_main_system.game_flag & GMD_GAME_FLAG_GAMESYS_RESTART) && IzFadeIsEnd())) ||	// リスタート時
				((g_gm_main_system.game_flag & (GMD_GAME_FLAG_CLEAR | GMD_GAME_FLAG_RESULT_END)) ==
#if !defined (MTD_DEBUG)
					(GMD_GAME_FLAG_CLEAR | GMD_GAME_FLAG_RESULT_END))) )) {					// クリアー時
#else
					(GMD_GAME_FLAG_CLEAR | GMD_GAME_FLAG_RESULT_END))) ) ||					// クリアー時
			((AoPadDirect() & (KEY_R_UP | KEY_R_DOWN | KEY_R_LEFT | KEY_R_RIGHT)) ==		// デバック用ゲーム終了時
				(KEY_R_UP | KEY_R_DOWN | KEY_R_LEFT | KEY_R_RIGHT)) ) {
#endif
//////
#else
	if ((g_gm_main_system.game_flag & GMD_GAME_FLAG_GAMEOVER_END) ||	// ゲームオーバー, タイムオーバー
			(!(g_gm_main_system.game_flag & (GMD_GAME_FLAG_GAMEOVER_END | GMD_GAME_FLAG_TIMEOVER)) &&
				((g_gm_main_system.game_flag & GMD_GAME_FLAG_GAMESYS_RESTART) && IzFadeIsEnd())) ||	// リスタート時
			((g_gm_main_system.game_flag & (GMD_GAME_FLAG_CLEAR | GMD_GAME_FLAG_RESULT_END)) ==
#if defined (MTD_DEBUG)
				(GMD_GAME_FLAG_CLEAR | GMD_GAME_FLAG_RESULT_END)) ||					// クリアー時
#if !_IPHONE
			((AoPadDirect() & (KEY_R_UP | KEY_R_DOWN | KEY_R_LEFT | KEY_R_RIGHT)) ==	// デバック用ゲーム終了時
				(KEY_R_UP | KEY_R_DOWN | KEY_R_LEFT | KEY_R_RIGHT))) {
#else // !_IPHONE
		((pad_emu[dbg::CPadEmu::ETrgPad::Free1][er::CTrgState::EState::Down])
		 && (pad_emu[dbg::CPadEmu::ETrgPad::Free2][er::CTrgState::EState::Down]))) {
#endif // !_IPHONE
#else
				(GMD_GAME_FLAG_CLEAR | GMD_GAME_FLAG_RESULT_END))) {					// クリアー時
#endif
#endif
		
#if SONIC4_TRIAL
		// ここでAlartビュー待ちにする
		if (Sonic4_IsEnabledAlertView() == TRUE) {
			return;
		}
#endif // SONIC4_TRIAL
		
		// ゲームリスタート
		// ゲームオーバー
		// タイムアップ
		// (デバック) ゲーム終了
		// ステージクリアフラグON

		// 遷移先イベント設定
		gmMainDecideNextEvt();

		if (g_gm_main_system.game_flag & GMD_GAME_FLAG_GAMESYS_RESTART) {
			// リスタート時
			GmMainRestartExit();

			if (g_gs_main_sys_info.game_mode == GSD_GAME_MODE_TIME_ATTACK) {
				// GS初期化
				GmMainGSRetryInit();
			}
		}
		else {
			// ゲーム終了時
			GmMainExit();
		}

		return;
	}

	/* システムからの強制終了チェック */
	if (!AoAccountIsCurrentEnable()) {
		// 終了してメインメニューへ戻る
		SyDecideEvtCase(GMD_MAIN_NEXT_EVT_TITLE);

		// 白画面で戻す
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEOUT, 1.f);

		// ゲーム終了
		GmMainExit();

		return;
	}

	// リザルトデモ開始チェック
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_CLEAR &&
				!(g_gm_main_system.game_flag & GMD_GAME_FLAG_RESULT_START)) {
		// サウンドOFF
		//GmSoundStopStageBGM(15);

		// システムクリアフラグ設定
		g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_CLEAR;

#if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
		// リザルト開始
		GmClearDemoStart();
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_RESULT_START;
#else	// #if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_RESULT_END;
#endif	// #if !defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	}

	/** タイマ **/
	if (GmMainCheckExeTimerCount()) {
		/* ゲームタイマ */
		if (!GSM_MAIN_STAGE_IS_SPSTAGE()) {
			// 一般ステージ用(カウントアップ)
			// 最大値停止は行わない
			// (タイマを使用する場所でクリッピングを行う事)
			g_gm_main_system.game_time++;
		} else {
			// スペシャルステージ用(カウントダウン)
			if (g_gm_main_system.game_time) {
				g_gm_main_system.game_time--;
#if defined (MTD_DEBUG)
				if (g_gm_main_system.game_time >= 60 * 60 * 59) {	// 59分以上ならカウント減算しない
					g_gm_main_system.game_time =  60 * 60 * 60;		// 60分キープ
				}
#endif
			}
		}
	}

	/* 同期タイマ */
	if (gmMainCheckExeSyncTimerCount()) {
		g_gm_main_system.sync_time++;
	}


	/* ゲームポーズ */
#if !_IPHONE
	if ((!AoPadIsConnected() || AoSysIsShowPlatformUI() || (AoPadStand() & KEY_START)) &&
		GmPauseCheckExecutable()) {
#else //!_IPHONE
	if (((GmMainKeyCheckPauseKeyPush() != -1) || gmMainIsSuspendedPause()) && GmPauseCheckExecutable()) {
		// サスペンド後の復帰フラグは管理されている。
		// gmMainPre、gmMainDataLoad***等...
		// 繰り返されると困るのでサスペンドポーズをクリア
		GmMainClearSuspendedPause();
#endif //!_IPHONE
		// ポーズ可能なときに、「パッド抜け」「システムUI表示」「スタート押下」のいずれかを検出したらポーズ開始
		
#if defined(MTD_DEBUG)
		// デバッグポーズ中は起動できないようにする
		if (!(g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_DEBUG_PAUSE)) {
			GmPauseInit();
		}
#else
		GmPauseInit();
#endif /* defined(MTD_DEBUG) */
	}
	else {
		if (g_gm_main_system.game_flag & GMD_GAME_FLAG_PAUSE_IS_DECIDED) {
			
			// 移行先決定
			gmMainDecideNextEvt();
			
			// gmMainDecideNextEvt()前にフラグオフしない
			g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_PAUSE_IS_DECIDED;
			
#if 1
			if (GmPauseMenuGetResult() == GME_PMENU_RESULT_RETRY) {
				// リスタート時
				GmMainRestartExit();

				// GS初期化
				GmMainGSRetryInit();
			}
			else {
				// ゲーム終了時
				GmMainExit();
			}
#else
			GmMainExit();
#endif
		}
	}

#if defined (MTD_DEBUG)
	if ((AoPadStand() & KEY_SELECT) &&
		!(g_gm_main_system.game_flag & GMD_GAME_FLAG_PAUSE_DEMO) &&
		GmPauseCheckExecutable()) {
		// デバックポーズ
		g_gm_main_system.debug_flag ^= GMD_GAME_DEBUG_FLAG_DEBUG_PAUSE;
		if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_DEBUG_PAUSE) {
			g_gm_main_system.debug_save_pause_level = g_obj.pause_level;
			ObjObjectPause(GMD_DEBUG_DEBUGPAUSE_LEVEL);
			
			// フラグ退避
			g_gm_main_system.debug_pause_time_flag_save	=
				(g_gm_main_system.game_flag & (GMD_GAME_FLAG_COUNT_GAME_TIME |
											   GMD_GAME_FLAG_COUNT_SYNC_TIME));
			// カウント停止
			g_gm_main_system.game_flag	&= ~(GMD_GAME_FLAG_COUNT_GAME_TIME |
											 GMD_GAME_FLAG_COUNT_SYNC_TIME);

#if _WII
			if (AoPadDirect() & KEY_R1) {
#else
			if (AoPadDirect() & KEY_L1) {
#endif
				// ライト編集開始
				DbgLightEditInit();
			}
		}
		else {
			if (g_gm_main_system.debug_save_pause_level <= 0) {
				ObjObjectPauseOut();
				g_gm_main_system.debug_save_pause_level = 0;
				
				// フラグ復帰
				g_gm_main_system.game_flag	|= (g_gm_main_system.debug_pause_time_flag_save &
												(GMD_GAME_FLAG_COUNT_GAME_TIME |
												 GMD_GAME_FLAG_COUNT_SYNC_TIME));
				g_gm_main_system.debug_pause_time_flag_save	= 0;
			}
			else {
				ObjObjectPause((u16)g_gm_main_system.debug_save_pause_level);
				
				// フラグ退避
				g_gm_main_system.debug_pause_time_flag_save	=
					(g_gm_main_system.game_flag & (GMD_GAME_FLAG_COUNT_GAME_TIME |
												   GMD_GAME_FLAG_COUNT_SYNC_TIME));
				// カウント停止
				g_gm_main_system.game_flag	&= ~(GMD_GAME_FLAG_COUNT_GAME_TIME |
												 GMD_GAME_FLAG_COUNT_SYNC_TIME);
			}

			// ライト編集終了
			if (DbgLightEditIsEdit()) {
				DbgLightEditExit();
			}
		}
	}
	else {
		if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_DEBUG_PAUSE) {
			
			if (!GmPauseCheckExecutable()) {
				// ポーズできない状態の場合はポーズ解除
				g_gm_main_system.debug_flag &= ~GMD_GAME_DEBUG_FLAG_DEBUG_PAUSE;
				if (g_gm_main_system.debug_save_pause_level <= 0) {
					ObjObjectPauseOut();
					g_gm_main_system.debug_save_pause_level = 0;
					
					// フラグ復帰
					g_gm_main_system.game_flag	|= (g_gm_main_system.debug_pause_time_flag_save &
													(GMD_GAME_FLAG_COUNT_GAME_TIME |
													 GMD_GAME_FLAG_COUNT_SYNC_TIME));
					g_gm_main_system.debug_pause_time_flag_save	= 0;
				}
				else {
					ObjObjectPause((u16)g_gm_main_system.debug_save_pause_level);
					
					// フラグ退避
					g_gm_main_system.debug_pause_time_flag_save	=
						(g_gm_main_system.game_flag & (GMD_GAME_FLAG_COUNT_GAME_TIME |
													   GMD_GAME_FLAG_COUNT_SYNC_TIME));
					// カウント停止
					g_gm_main_system.game_flag	&= ~(GMD_GAME_FLAG_COUNT_GAME_TIME |
													 GMD_GAME_FLAG_COUNT_SYNC_TIME);
				}
			}
			else {
				// デバックポーズ中
				if (AoPadRepeat() & KEY_R2) {
					if (!DbgLightEditIsEdit()) {
						// コマ送り
						ObjObjectPauseOut();
						
						// フラグ復帰
						g_gm_main_system.game_flag	|= (g_gm_main_system.debug_pause_time_flag_save &
														(GMD_GAME_FLAG_COUNT_GAME_TIME |
														 GMD_GAME_FLAG_COUNT_SYNC_TIME));
						g_gm_main_system.debug_pause_time_flag_save	= 0;
					}	// !DbgLightEditIsEdit()
				}
				else {
					if (g_obj.pause_level != GMD_DEBUG_DEBUGPAUSE_LEVEL) {
						// 停止に遷移する最初のフレームの時だけ
						// フラグ退避
						g_gm_main_system.debug_pause_time_flag_save	=
							(g_gm_main_system.game_flag & (GMD_GAME_FLAG_COUNT_GAME_TIME |
														   GMD_GAME_FLAG_COUNT_SYNC_TIME));
					}
					
					ObjObjectPause(GMD_DEBUG_DEBUGPAUSE_LEVEL);
					
					// カウント停止
					g_gm_main_system.game_flag	&= ~(GMD_GAME_FLAG_COUNT_GAME_TIME |
													 GMD_GAME_FLAG_COUNT_SYNC_TIME);
				}
			}
		}
	}

	// ステータス表示
#if !_IPHONE || !defined HOG_PRESENT_ROM_IPHONE
#if 0
	{
		s32	x = 30, y = 30;
		s32		alloc_size;
		AMS_CRIAUDIO_INTERFACE	*pCriaudio = amCriAudioGetGlobal();

		alloc_size = criHeap_DebugGetTotalAllocSize(pCriaudio->heap[AME_CRIAUDIO_HEAP_CSB]);
		amPrintf(x, y, " CSB HEAP : %x", alloc_size);
		y++;

		alloc_size = criHeap_DebugGetTotalAllocSize(pCriaudio->heap[AME_CRIAUDIO_HEAP_STREAM]);
		amPrintf(x, y, "STRM HEAP : %x", alloc_size);
		y++;
	}
#endif

	/* プレイヤーステータス */
	if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_DEBUG_DISP) {
		GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
		OBS_OBJECT_WORK	*ply_obj = (OBS_OBJECT_WORK*)ply_work;
		s32	y = 3;

		if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_PLY_POS_16) {
			// 16進表示
			amPrintf(2, y, "POS X : %x", ply_obj->pos.x);
			y++;
			amPrintf(2, y, "POS Y : %x", ply_obj->pos.y);
			y++;
			amPrintf(2, y, "POS Z : %x", ply_obj->pos.z);
			y++;
		}
		else if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_PLY_POS_10F) {
			// 10進小数点あり表示
			amPrintf(2, y, "POS X : %0.8f", FXM_FX32_TO_FLOAT(ply_obj->pos.x));
			y++;
			amPrintf(2, y, "POS Y : %0.8f", FXM_FX32_TO_FLOAT(ply_obj->pos.y));
			y++;
			amPrintf(2, y, "POS Z : %0.8f", FXM_FX32_TO_FLOAT(ply_obj->pos.z));
			y++;
		}
		else {
			// 10進表示
			amPrintf(2, y, "POS X : %d", (ply_obj->pos.x >> FX32_SHIFT));
			y++;
			amPrintf(2, y, "POS Y : %d", (ply_obj->pos.y >> FX32_SHIFT));
			y++;
			amPrintf(2, y, "POS Z : %d", (ply_obj->pos.z >> FX32_SHIFT));
			y++;
		}
		amPrintf(2, y, "SPD_M : %x", ply_obj->spd_m);
		y++;
		amPrintf(2, y, "SPD_X : %x", ply_obj->spd.x);
		y++;
		amPrintf(2, y, "SPD_Y : %x", ply_obj->spd.y);
		y++;
		amPrintf(2, y, "DIR_Z : %x", ply_obj->dir.z);
		y++;
		amPrintf(2, y, "RING  : %x", ply_work->ring_num);
		y++;

		amPrintf(2, y, "SEQ : %s", g_gm_player_seq_name_tbl[ply_work->seq_state]);
		y++;

		if (ply_obj->move_flag & OBD_MOVE_UNDER) {
			amPrintf(2, y, "UNDER : ON");
		}
		else {
			amPrintf(2, y, "UNDER : OFF");
		}
		y++;

		amPrintf(2, y, "DIR FALL : %x", ply_work->obj_work.dir_fall);
		y++;
		amPrintf(2, y, "PSEUDOFALL : %x", g_gm_main_system.pseudofall_dir);
		y++;
		amPrintf(2, y, "JUMPPSEUDOFALL : %x", ply_work->jump_pseudofall_dir);
		y++;
		amPrintf(2, y, "ENE KILL CNT : %d", g_gs_main_sys_info.ene_kill_count);
		y++;
	}
#endif //!_IPHONE || !defined HOG_PRESENT_ROM_IPHONE

	/* メモリでバック表示設定 */
	if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_DEBUG_DISP) {
		_am_dbg_display_mode = 2;
	}
	else {
		_am_dbg_display_mode = 0;
	}

	/* 強制クリアコマンド */
	if (!(g_gm_main_system.game_flag & (GMD_GAME_FLAG_CLEAR | GMD_GAME_FLAG_START_DEMO |
				GMD_GAME_FLAG_GAMEOVER_START | GMD_GAME_FLAG_GAMEOVER_END | GMD_GAME_FLAG_PAUSE_DEMO))) {
		// PS3:□ボタン
		// 360:Ｘボタン
		// Wii:Ａボタン
#if !_IPHONE
		if (AoPadDirect() & KEY_R_LEFT) {
#else
		if (AoPadDirect() & KEY_L2) {
#endif // !_IPHONE 
			g_gm_main_system.debug_force_clear_cnt++;
			if (g_gm_main_system.debug_force_clear_cnt > 300) {		// ボタン長押し：５秒
				g_gm_main_system.game_flag |= GMD_GAME_FLAG_CLEAR;
			}
		}
		else {
			g_gm_main_system.debug_force_clear_cnt = 0;
		}
	}


#endif // #if defined (MTD_DEBUG)
}



// =======================================================================
// gmMainDecideNextEvt
/*!
  イベント移行
 */
// =======================================================================
void gmMainDecideNextEvt(void)
{
	Sint16	next_evt	= 0;

#if defined (HOG_INLINE3_ROM) || defined (HOG_PRESENT_ROM_IPHONE)
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_PAUSE_IS_DECIDED) {
		switch (GmPauseMenuGetResult()) {
		case GME_PMENU_RESULT_RETRY:
			next_evt	= GMD_MAIN_NEXT_EVT_MAINGAME;
			break;
		case GME_PMENU_RESULT_BACK:
			next_evt	= GMD_MAIN_NEXT_EVT_TITLE;
			break;
		default:
			next_evt	= GMD_MAIN_NEXT_EVT_TITLE;
		}
	}
	else if (g_gm_main_system.game_flag & GMD_GAME_FLAG_CLEAR) {
		if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_BOSS) {
		// ボス戦終了
			// タイトル
			next_evt = GMD_MAIN_NEXT_EVT_TITLE;
		}
#if !_IPHONE
		else {
		// 次のアクト
			// ゲーム
			next_evt = GMD_MAIN_NEXT_EVT_MAINGAME;
		}
#else //!_IPHONE
		else if (g_gs_main_sys_info.stage_id < GSD_MAIN_STAGE_ID_1_BOSS) {
		// 次のアクト
			// ゲーム
			next_evt = GMD_MAIN_NEXT_EVT_MAINGAME;
		}
		else {
		// その他
			// タイトル
			next_evt = GMD_MAIN_NEXT_EVT_TITLE;
		}
#endif //!_IPHONE
	}
	else if (g_gm_main_system.game_flag & GMD_GAME_FLAG_GAMESYS_RESTART) {
	// リスタート
		// ゲーム
		next_evt = GMD_MAIN_NEXT_EVT_MAINGAME;
	}
	else {
	// 死亡・リセット
		// タイトル
		next_evt = GMD_MAIN_NEXT_EVT_TITLE;
	}
#else // defined (HOG_INLINE3_ROM) || defined (HOG_PRESENT_ROM_IPHONE)
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_PAUSE_IS_DECIDED) {
	// ポーズによる分岐
		switch (GmPauseMenuGetResult()) {
		case GME_PMENU_RESULT_RETRY:
			next_evt	= GMD_MAIN_NEXT_EVT_MAINGAME;
			break;
		case GME_PMENU_RESULT_BACK:
			next_evt	= GMD_MAIN_NEXT_EVT_WORLDMAP;
			break;
		case GME_PMENU_RESULT_MAINMENU:
			next_evt	= GMD_MAIN_NEXT_EVT_MAINMENU;
			break;
		default:
			next_evt	= GMD_MAIN_NEXT_EVT_MAINGAME;
		}
	}
#if !_WII
	else if (!(g_gm_main_system.game_flag & GMD_GAME_FLAG_GAMESYS_RESTART) &&
		GsTrialIsTrial()) {
	// 体験版
		// 購入画面へ
		next_evt = GMD_MAIN_NEXT_EVT_BUYSCREEN;
#if SONIC4_TRIAL
		// １分間の累積タイムをクリアしておく
		g_trial_game_time = 0;
#endif // SONIC4_TRIAL
	}
#endif
	else if (g_gs_main_sys_info.game_mode == GSD_GAME_MODE_TIME_ATTACK) {
	// タイムアタック
#if _IPHONE
		// ポーズをクリアしておく
		GmMainClearSuspendedPause();
#endif // _IPHONE
#if 1
		if (g_gm_main_system.game_flag & GMD_GAME_FLAG_GAMESYS_RESTART) {
			// リトライ時
			// ゲーム
			next_evt = GMD_MAIN_NEXT_EVT_MAINGAME;
		}
		else {
			// 死亡・リセット・メニューに戻る
			if (GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_1_1)) {
				// ワールドマップ
				next_evt	= GMD_MAIN_NEXT_EVT_WORLDMAP;
			}
			else {
				// メインメニュー (1-1未クリア時)
				next_evt	= GMD_MAIN_NEXT_EVT_MAINMENU;
			}
		}
#else
		if (g_gm_main_system.game_flag & GMD_GAME_FLAG_CLEAR) {
			if (g_gm_main_system.game_flag & GMD_GAME_FLAG_GAMESYS_RESTART) {
			// リトライ時
				// ゲーム
				next_evt = GMD_MAIN_NEXT_EVT_MAINGAME;
			}
			else {
			// マップへ戻る
				// ワールドマップへ
				next_evt = GMD_MAIN_NEXT_EVT_WORLDMAP;
			}
		}
		else if (g_gm_main_system.game_flag & GMD_GAME_FLAG_GAMESYS_RESTART) {
		// リスタート
			// ゲーム
			next_evt = GMD_MAIN_NEXT_EVT_MAINGAME;
		}
		else {
		// 死亡(ゲームオーバー)・リセット
			if (GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_1_1)) {
				// ワールドマップ
				next_evt	= GMD_MAIN_NEXT_EVT_WORLDMAP;
			}
			else {
				// メインメニュー (1-1未クリア時)
				next_evt	= GMD_MAIN_NEXT_EVT_MAINMENU;
			}
		}
#endif
	}
	else {
	// その他
		if (g_gm_main_system.game_flag & GMD_GAME_FLAG_CLEAR) {
		// クリア
			if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPECIAL_STAGE) {
				// スペステへ
				next_evt = GMD_MAIN_NEXT_EVT_SPSTAGE_BRA;
			}
			else if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_FINAL_1) {
				// エンディングへ
				// ◆GSD_MAIN_STAGE_ID_FINAL_5になるかも
				next_evt = GMD_MAIN_NEXT_EVT_ENDING;
			}
			else {
				// 通常クリア
				// ワールドマップへ
				next_evt = GMD_MAIN_NEXT_EVT_WORLDMAP;
			}
		}
		else if (g_gm_main_system.game_flag & GMD_GAME_FLAG_GAMESYS_RESTART) {
		// リスタート
			// ゲーム
			next_evt = GMD_MAIN_NEXT_EVT_MAINGAME;
		}
		else {
		// 死亡(ゲームオーバー)・リセット
			if (GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_1_1)) {
				// ワールドマップ
				next_evt	= GMD_MAIN_NEXT_EVT_WORLDMAP;
			}
			else {
				// メインメニュー (1-1未クリア時)
				next_evt	= GMD_MAIN_NEXT_EVT_MAINMENU;
			}
		}
	}
#endif // defined (HOG_INLINE3_ROM) || defined (HOG_PRESENT_ROM_IPHONE)

	// イベント移行先設定
	SyDecideEvtCase(next_evt);
}


// ==========================================================================
// タイマ関連
// ==========================================================================
// ==========================================================================
// gmMainCheckExeSyncTimerCount
/*!
 *	タイマーカウント実行チェック
 *
 *	@return	TRUE : カウント実行
 *
 *	@note
 *		各種状況をチェックしてメインタイマーカウントアップを行うかチェックします
 */
// ==========================================================================
BOOL gmMainCheckExeSyncTimerCount(void)
{
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_COUNT_SYNC_TIME) {
		return (TRUE);
	}

	return (FALSE);
}


// ==========================================================================
// ボス連戦用
// ==========================================================================
// ==========================================================================
// gmDataLoadBoosBattleMgr_****
/*!
 *	ゲームデータロードボス連戦用 監視タスク
 *
 *	@param	boss_type		[in]	ロードボスタイプ
 *
 *	@note
 *		ビルドまで行います
 */
// ==========================================================================
void gmMainDataLoadBoosBattleMgr_LoadWait(MTS_TASK_TCB *tcb)
{
	if (GmGameDatLoadCheck() == GMD_GAMEDAT_LOAD_PROGRESS_COMPLETE) {
		GMS_MAIN_LOAD_BB_MGR_WORK	*work;
		work = (GMS_MAIN_LOAD_BB_MGR_WORK*)mtTaskGetTcbWork(tcb);

		// ロード終了
		GmGameDatLoadExit();

		// ビルドへ移行
		GmGameDatBuildBossBattleInit();
		GmGameDatBuildBossBattle(work->boss_type);

		// ビルド待機へ
		mtTaskChangeTcbProcedure(tcb, gmMainDataLoadBoosBattleMgr_BuildWait);
	}
}
void gmMainDataLoadBoosBattleMgr_BuildWait(MTS_TASK_TCB *tcb)
{
	if (GmGameDatBuildBossBattleCheck()) {
		GMS_MAIN_LOAD_BB_MGR_WORK	*work;
		work = (GMS_MAIN_LOAD_BB_MGR_WORK*)mtTaskGetTcbWork(tcb);

		// ビルド終了
		work->b_end = TRUE;

		// ロード状態保存
		g_gm_main_system.boss_load_no = work->boss_type;

		// データロード終了
		g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_FINAL_DATA_LOAD;

		// タスク破棄
		mtTaskClearTcb(tcb);
		gm_main_load_bossbattle_tcb = NULL;

		//// 終了待機へ移行
		//mtTaskChangeTcbProcedure(tcb, gmMainDataLoadBoosBattleMgr_EndWait);
	}
}
//void gmMainDataLoadBoosBattleMgr_EndWait(MTS_TASK_TCB *tcb)
//{
//	UNREFERENCED_PARAMETER(tcb);
//}


// ==========================================================================
// gmDataReleaseBoosBattleMgr_****
/*!
 *	ゲームデータリリースボス連戦用 監視タスク
 *
 *	@param	boss_type		[in]	ロードボスタイプ
 *
 *	@note
 *		リリースまで行います
 */
// ==========================================================================
void gmMainDataReleaseBoosBattleMgr_FlushWait(MTS_TASK_TCB *tcb)
{
	if (GmGameDatFlushBossBattleCheck()) {
		GMS_MAIN_LOAD_BB_MGR_WORK	*work;
		work = (GMS_MAIN_LOAD_BB_MGR_WORK*)mtTaskGetTcbWork(tcb);

		// フラッシュ終了終了
		//GmGameDatLoadExit();

		// リリースへ移行
		GmGameDatBoosBattleRelease(work->boss_type);

		// リリース待機へ
		mtTaskChangeTcbProcedure(tcb, gmMainDataReleaseBoosBattleMgr_ReleaseWait);
	}
}
void gmMainDataReleaseBoosBattleMgr_ReleaseWait(MTS_TASK_TCB *tcb)
{
	if (GmGameDatReleaseCheck()) {
		GMS_MAIN_LOAD_BB_MGR_WORK	*work;
		work = (GMS_MAIN_LOAD_BB_MGR_WORK*)mtTaskGetTcbWork(tcb);

		// リリース終了
		work->b_end = TRUE;

		// 解放状態保存
		g_gm_main_system.boss_load_no = -1;

		// データ解放終了
		g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_FINAL_DATA_RELEASE;

		// 終了待機へ移行
		mtTaskChangeTcbProcedure(tcb, gmMainDataReleaseBoosBattleMgr_EndWait);
	}
}
void gmMainDataReleaseBoosBattleMgr_EndWait(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
}

#if _IPHONE
// ==========================================================================
// gmMainIsUseWaitUpCamera
/*!
 *	待機中にカメラがアップになるか否か
 *
 *	@return TRUE:アップになる FALSE:アップにならない
 */
// ==========================================================================
static BOOL gmMainIsUseWaitUpCamera(void)
{
	BOOL flag = TRUE;
	
	switch (g_gs_main_sys_info.stage_id) {
		case GSD_MAIN_STAGE_ID_1_BOSS:
		case GSD_MAIN_STAGE_ID_2_BOSS:
		case GSD_MAIN_STAGE_ID_3_BOSS:
		case GSD_MAIN_STAGE_ID_4_BOSS:
		case GSD_MAIN_STAGE_ID_FINAL_1:
		case GSD_MAIN_STAGE_ID_FINAL_2:
		case GSD_MAIN_STAGE_ID_FINAL_3:
		case GSD_MAIN_STAGE_ID_FINAL_4:
		case GSD_MAIN_STAGE_ID_FINAL_5:
		case GSD_MAIN_STAGE_ID_SS1:
		case GSD_MAIN_STAGE_ID_SS2:
		case GSD_MAIN_STAGE_ID_SS3:
		case GSD_MAIN_STAGE_ID_SS4:
		case GSD_MAIN_STAGE_ID_SS5:
		case GSD_MAIN_STAGE_ID_SS6:
		case GSD_MAIN_STAGE_ID_SS7:
			// BOSS戦/SSはアップにならない
			flag = FALSE;
			break;
		default:
			// リトライ演出中はアップにならない(リトライ対策)s
			if (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->player_flag & GMD_PLF_TATK_RETRY) {
				flag = FALSE;
			}
			break;
	}
	
	return flag;
}


// ==========================================================================
// gmMainIsSuspendedPause
/*!
 *	サスペンド後のポーズを行うべきか否かの判定
 *
 *	@return TRUE:ポーズ FALSE:ポーズしない
 */
// ==========================================================================
BOOL gmMainIsSuspendedPause(void)
{
	if (g_gs_main_sys_info.game_flag & GMD_GAME_FLAG_SUSPENDED_PAUSE) {
		return TRUE;
	}
	return FALSE;
}

// ==========================================================================
// gmMainUpdateSuspendedPause
/*!
 *	サスペンド後のポーズフラグの更新
 *
 */
// ==========================================================================
void gmMainUpdateSuspendedPause(void)
{
#define GMD_GAME_FLAG_SUSPEND_PAUSE_MASK (GMD_GAME_FLAG_START_DEMO | GMD_GAME_FLAG_RESULT_START)
	// サスペンドフラグかつポーズ発動待機マスクからスタートデモ以外の状態の場合
	if (GsMainSysGetSuspendedFlag() && (!(g_gm_main_system.game_flag & GMD_GAME_FLAG_PAUSE_WAIT_MASK & ~GMD_GAME_FLAG_SUSPEND_PAUSE_MASK))) {
		g_gs_main_sys_info.game_flag |= GMD_GAME_FLAG_SUSPENDED_PAUSE;
	}
}

#endif //_IPHONE

// ==========================================================================
// GmMainStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void GmMainStaticVarInit(void)
{
	/// ゲームメインシステムワーク
	memset(&g_gm_main_system, 0, sizeof(g_gm_main_system));
	
	//----- Local Variables -----------------------------------------------------
	gm_main_load_wait_tcb = NULL;
	gm_main_release_wait_tcb = NULL;
	
	gm_main_load_bossbattle_tcb = NULL;		//!< ボス連戦用データロードマネージャ
	gm_main_release_bossbattle_tcb = NULL;	//!< ボス連戦用データリリースマネージャ
#if SONIC4_TRIAL
	g_trial_game_time = 0;
#endif // SONIC4_TRIAL
	
#if defined (MTD_DEBUG)
#if 0
	// テスト用
	memset(&gm_test_light, 0, sizeof(gm_test_light));
	static const NNS_RGBA		sc_gm_test_light_col = {
		1.0f, 1.0f, 1.0f, 1.0f,
	};
	sc_gm_test_light_col = sc_gm_test_light_col;
	gm_test_light_intensity = 1.0f;
	static const NNS_VECTOR		sc_gm_test_light_dir = {
		0.0f, 0.0f, -1.0f,
	};
	gm_test_light_dir = sc_gm_test_light_dir;
#endif // #if !_PS3
#endif // #if defined (MTD_DEBUG)
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
