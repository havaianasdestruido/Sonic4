// ==========================================================================
/*!
  @file dmLogoSega.cpp
  @brief セガロゴ

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: dmLogoSega.cpp 186 2011-05-26 19:49:18Z thamada $
  $Date:: 2011-05-27 04:49:18 +0900#$
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
#include "gsEnvironment.h"
#include "gsSystemBgm.h"
#include "gsSound.h"
#include "izFade.h"
#include "objObject.h"
#include "gmMain.h"
#include "gmTask.h"
#include "gmGameDBuild.h"
#include "gmPlayer.h"

#if _WII
#include "gmPlyLod.h"
#endif

#include "dmLogoCom.h"

#include "dmLogoSega.h"

// データヘッダ
#include "common/arc/EFF_CMN.HMB"
#include "common/arc/D_LOGO_SEGA.HMB"
#include "common/ace/D_LOGO_SONIC.HMA"
#include "common/model/SON_MDL.hmb"
#include "common/model/SON_MTN.hmb"

//mpp
#include "mppUtil.h"


#include "Sonic4_Utility.h"

//----- Definitions ---------------------------------------------------------
/* タスク設定 */
#define DMD_LOGO_SEGA_TASK_PRIO_DATA_LOAD		(0x1000)		//!< ロードタスクプライオリティ
#define DMD_LOGO_SEGA_TASK_GROUP_DATA_LOAD		(0)				//!< ロードタスクグループ
#define DMD_LOGO_SEGA_TASK_PRIO_DATA_BUILD		(0x1000)		//!< ビルドタスクプライオリティ
#define DMD_LOGO_SEGA_TASK_GROUP_DATA_BUILD		(0)				//!< ビルドタスクグループ
#define DMD_LOGO_SEGA_TASK_PRIO_DATA_FLUSH		(0x1000)		//!< フラッシュタスクプライオリティ
#define DMD_LOGO_SEGA_TASK_GROUP_DATA_FLUSH		(0)				//!< フラッシュタスクグループ
#define DMD_LOGO_SEGA_TASK_PRIO_DATA_RELEASE	(0x1000)		//!< リリースタスクプライオリティ
#define DMD_LOGO_SEGA_TASK_GROUP_DATA_RELEASE	(0)				//!< リリースタスクグループ

#define DMD_LOGO_SEGA_TASK_PRIO_MAIN			(0x1000)		//!< メインタスクプライオリティ
#define DMD_LOGO_SEGA_TASK_GROUP_MAIN			(0)				//!< メインタスクグループ
#define DMD_LOGO_SEGA_TASK_PRIO_CAMERA			(0x1800)		//!< カメラタスクプライオリティ
#define DMD_LOGO_SEGA_TASK_GROUP_CAMERA			(0)				//!< カメラタスクグループ
#define DMD_LOGO_SEGA_TASK_PRIO_EFFECT_SERVER	(0x5000)		//!< エフェクトサーバータスクプライオリティ
#define DMD_LOGO_SEGA_TASK_GROUP_EFFECT_SERVER	(0)				//!< エフェクトサーバータスクグループ
#define DMD_LOGO_SEGA_TASK_PRIO_PLAYER			(0x2000)		//!< プレイヤータスクプライオリティ
#define DMD_LOGO_SEGA_TASK_GROUP_PLAYER			(0)				//!< プレイヤータスクグループ
#define DMD_LOGO_SEGA_TASK_PRIO_EFFECT			(0x3000)		//!< エフェクトタスクプライオリティ
#define DMD_LOGO_SEGA_TASK_GROUP_EFFECT			(0)				//!< エフェクトタスクグループ

#define DMD_LOGO_SEGA_TASK_PRIO_SOUND			(0x7fff)		//!< サウンドシステム
#define DMD_LOGO_SEGA_TASK_GROUP_SOUND			(0)				//!< サウンドシステム

#define	DMD_LOGO_SEGA_TASK_PAUSELEVEL_DEF		(0)				//!< 標準ポーズレベル
#define DMD_LOGO_SEGA_OBJ_OBJPAUSELEVEL_DEF		(0)				//!< オブジェクトポーズレベルデフォルト

/* サウンド設定 */
#ifdef SONIC4_TRIAL
#define SOUND_PATH	"SOUND/TRIAL/"
#else
#define SOUND_PATH	"SOUND/SOUND/"
#endif// SONIC4_TRIAL

#define DMD_LOGO_SEGA_SOUND_SE_FILE_PATH		(GSS_BASE_PATH SOUND_PATH"SND_FX.CSB")

/* 演出設定 */
#if !_IPHONE
#define DMD_LOGO_SEGA_START_WAIT_TIME		(30)				//!< 開始待機
#else //!_IPHONE
#define DMD_LOGO_SEGA_START_WAIT_TIME		(0)					//!< 開始待機
#endif //!_IPHONE
//#define DMD_LOGO_SEGA_FADEIN_TIME			(60)				//!< フェードイン時間
#define DMD_LOGO_SEGA_SONIC_RUN_LEFT_TIME	(10)				//!< ソニック左走り時間
#define DMD_LOGO_SEGA_SONIC_RUN_RIGHT_WAIT_TIME	(25)			//!< ソニック右走り待機時間
#define DMD_LOGO_SEGA_SONIC_RUN_RIGHT_TIME	(10)				//!< ソニック右走り時間
#define DMD_LOGO_SEGA_DISP_TIME				(180)				//!< ロゴ表示時間
#define DMD_LOGO_SEGA_FADEOUT_TIME			(60)				//!< フェードアウト時間

#define DMD_LOGO_SEGA_DISP_WIDTH			(960)

// プレイヤー
#define DMD_LOGO_SEGA_PLAYER_INIT_POS_X			(DMD_LOGO_SEGA_DISP_WIDTH/2+128)
#define DMD_LOGO_SEGA_PLAYER_INIT_POS_Y			(0)
#define DMD_LOGO_SEGA_PLAYER_INIT_POS_Z			(0)
#if !_IPHONE
#define DMD_LOGO_SEGA_PLAYER_POS_Y_ADJUST		(-48)
#else //!_IPHONE
#define DMD_LOGO_SEGA_PLAYER_POS_Y_ADJUST		(-36)
#endif //!_IPHONE
#define DMD_LOGO_SEGA_PLAYER_LEFT_RUN_S_POS_X	(DMD_LOGO_SEGA_DISP_WIDTH/2+128)
#define DMD_LOGO_SEGA_PLAYER_LEFT_RUN_SPD		(-(DMD_LOGO_SEGA_DISP_WIDTH+128+128)/DMD_LOGO_SEGA_SONIC_RUN_LEFT_TIME)
#define DMD_LOGO_SEGA_PLAYER_RIGHT_RUN_S_POS_X	(-(DMD_LOGO_SEGA_DISP_WIDTH/2+128))
#define DMD_LOGO_SEGA_PLAYER_RIGHT_RUN_SPD		((DMD_LOGO_SEGA_DISP_WIDTH+128+128)/DMD_LOGO_SEGA_SONIC_RUN_RIGHT_TIME)

// ダッシュエフェクト
#if 1
#define DMD_LOGO_SEGA_DASH_EFCT_OFST_POS_X			(-20)
#define DMD_LOGO_SEGA_DASH_EFCT_OFST_POS_Y			(100)
#define DMD_LOGO_SEGA_DASH_EFCT_OFST_POS_Z			(100)
#else
static fx32 test_pos_x = -20;
static fx32 test_pos_y = 100;
static fx32 test_pos_z = 100;
#define DMD_LOGO_SEGA_DASH_EFCT_OFST_POS_X			(test_pos_x)
#define DMD_LOGO_SEGA_DASH_EFCT_OFST_POS_Y			(test_pos_y)
#define DMD_LOGO_SEGA_DASH_EFCT_OFST_POS_Z			(test_pos_z)
#endif



/* オブジェクトタイプ */
#define DMD_LOGO_SEGA_OBJTYPE_PLAYER			(1)			//!< プレイヤー
#define DMD_LOGO_SEGA_OBJTYPE_EFFECT			(2)			//!< エフェクト

/* カメラ設定 */
#define DMD_LOGO_SEGA_CAMERA_ID					(0)
#if !_IPHONE
#define DMD_LOGO_SEGA_CAMERA_SCALE				(0.75f)//((float)DMD_LOGO_SEGA_DISP_WIDTH / (AMD_DISPLAY_WIDTH/AMD_SCREEN_WIDTH) / GSD_DISP_WIDTH) // 
#else //!_IPHONE
#define DMD_LOGO_SEGA_CAMERA_SCALE				(0.90f)
#endif //!_IPHONE

/* 描画ステート */
#define DMD_LOGO_SEGA_DRAW_CMD_STATE_3DNN			(0)
//#define DMD_LOGO_SEGA_DRAW_CMD_STATE_PRE_MAPFAR		(0)
//#define DMD_LOGO_SEGA_DRAW_CMD_STATE_MAPFAR_B		(1)
//#define DMD_LOGO_SEGA_DRAW_CMD_STATE_MAPFAR_SEA		(2)
//#define DMD_LOGO_SEGA_DRAW_CMD_STATE_MAPFAR_T			(3)
//#define DMD_LOGO_SEGA_DRAW_CMD_STATE_POST_MAPFAR		(4)



/// 読み込みデータ
enum {
	// 共有
	DMD_LOGO_SEGA_DATA_PLY_MDL_AMB	= 0,		//!< プレイヤーモデル
	DMD_LOGO_SEGA_DATA_PLY_TEX_AMB,				//!< プレイヤーテクスチャ
	DMD_LOGO_SEGA_DATA_PLY_MTN_AMB,				//!< プレイヤーモーション
	DMD_LOGO_SEGA_DATA_EFCT_AMB,				//!< エフェクトデータ

	// ローカライズ対応データ
	DMD_LOGO_SEGA_DATA_LOCAL_AMA_SET_AMB,		//!< セガロゴ等ローカライズ対応AMA

	DMD_LOGO_SEGA_DATA_MAX
};

/// AOSテクスチャタイプ
enum {
	DMD_LOGO_SEGA_AOSTEX_LOCAL_ACT	= 0,		//!< ローカライズ対応アクションテクスチャ
	DMD_LOGO_SEGA_AOSTEX_EFFECT_MDL,			//!< エフェクトモデル用

	DMD_LOGO_SEGA_AOSTEX_MAX
};


/// アクションデータ
enum {
	DMD_LOGO_SEGA_ACT_BG_WHITE	= 0,
	DMD_LOGO_SEGA_ACT_LOGO_SEGA,
	DMD_LOGO_SEGA_ACT_COVER,
	DMD_LOGO_SEGA_ACT_LINE01,
	DMD_LOGO_SEGA_ACT_LINE02,
	DMD_LOGO_SEGA_ACT_LINE03,
	DMD_LOGO_SEGA_ACT_LINE04,
	DMD_LOGO_SEGA_ACT_LINE05,

	DMD_LOGO_SEGA_ACT_MAX
};

/// エフェクト用モデル
enum {
	DMD_LOGO_SEGA_EFCT_MODEL_FOOT_L	= 0,
	DMD_LOGO_SEGA_EFCT_MODEL_FOOT_R,

	DMD_LOGO_SEGA_EFCT_MODEL_MAX
};

/// 3Dオブジェクトワーク
typedef struct tag_DMS_LOGO_SEGA_OBJ_3DNN_WORK {
	OBS_OBJECT_WORK			obj_work;
	OBS_ACTION3D_NN_WORK	obj_3d;
	OBS_DATA_WORK			data_work;
} DMS_LOGO_SEGA_OBJ_3DNN_WORK;

/// エフェクトワーク
typedef struct tag_DMS_LOGO_SEGA_OBJ_ES_WORK {
	OBS_OBJECT_WORK			obj_work;
	OBS_ACTION3D_ES_WORK	obj_3des;
	OBS_DATA_WORK			data_work_texamb;	// テクスチャAMB管理用
	OBS_DATA_WORK			data_work_texlist;	// テクスチャリスト管理用
	OBS_DATA_WORK			data_work_model;	// モデル管理用
} DMS_LOGO_SEGA_OBJ_ES_WORK;

/// セガロゴワーク
typedef struct tag_DMS_LOGO_SEGA_WORK {
	u32				flag;
	s32				timer;

	void (*func)(struct tag_DMS_LOGO_SEGA_WORK*);

	AOS_ACTION		*act[DMD_LOGO_SEGA_ACT_MAX];		//!< アクション
	
	OBS_OBJECT_WORK	*ply_obj;
	OBS_OBJECT_WORK	*efct_obj;

	GSS_SND_SE_HANDLE	*h_se;

} DMS_LOGO_SEGA_WORK;

#define DMD_LOGO_SEGA_FLAG_END					(0x00000001)	//!< 演出終了
#define DMD_LOGO_SEGA_FLAG_ACT_FRAME_UPDATE		(0x00000002)	//!< アクションフレーム更新あり

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void dmLogoSegaStart(void);
static void dmLogoSegaObjSysytemInit(void);
static void dmLogoSegaPreEnd(DMS_LOGO_SEGA_WORK *logo_work);
static void dmLogoSegaEnd(DMS_LOGO_SEGA_WORK *logo_work);

static void dmLogoSegaMainFunc(MTS_TASK_TCB *tcb);
static void gmLogoSegaEndWaitFunc(MTS_TASK_TCB *tcb);
static void gmLogoSegaFlushWaitFunc(MTS_TASK_TCB *tcb);
static void gmLogoSegaRelesehWaitFunc(MTS_TASK_TCB *tcb);
static void gmLogoSegaPreEndWaitFunc(MTS_TASK_TCB *tcb);
static void dmLogoSegaStartWaitFunc(DMS_LOGO_SEGA_WORK *logo_work);
static void dmLogoSegaRunLeftFunc(DMS_LOGO_SEGA_WORK *logo_work);
static void dmLogoSegaRunRightWaitFunc(DMS_LOGO_SEGA_WORK *logo_work);
static void dmLogoSegaRunRightFunc(DMS_LOGO_SEGA_WORK *logo_work);
static void dmLogoSegaDispWaitFunc(DMS_LOGO_SEGA_WORK *logo_work);
static void dmLogoSegaFadeOutWaitFunc(DMS_LOGO_SEGA_WORK *logo_work);

static void dmLogoSegaDataBuildMain(MTS_TASK_TCB *tcb);
static void dmLogoSegaDataBuildDest(MTS_TASK_TCB *tcb);
static void dmLogoSegaDataFlushMain(MTS_TASK_TCB *tcb);
static void dmLogoSegaDataFlushDest(MTS_TASK_TCB *tcb);
static void dmLogoSegaLoadPostFunc(DMS_LOGO_COM_LOAD_CONTEXT *context);

static void dmLogoSegaLoadWait(MTS_TASK_TCB *tcb);
static void dmLogoSegaBuildWait(MTS_TASK_TCB *tcb);

static void dmLogoSegaActionCreate(DMS_LOGO_SEGA_WORK *logo_work);
static void dmLogoSegaActionDelete(DMS_LOGO_SEGA_WORK *logo_work);

static OBS_OBJECT_WORK* dmLogoSegaCreatePlayer(void);
#if _WII
static NNE_BOOL dmLogoSegaPlayerMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param);
#endif
static void dmLogoSegaPlayerInitSeqRunLeft(OBS_OBJECT_WORK *obj_work);
static void dmLogoSegaPlayerSeqRunLeft(OBS_OBJECT_WORK *obj_work);
static void dmLogoSegaPlayerInitSeqRunRight(OBS_OBJECT_WORK *obj_work);
static void dmLogoSegaPlayerSeqRunRight(OBS_OBJECT_WORK *obj_work);

static OBS_OBJECT_WORK* dmLogoSegaCreateDashEffect(OBS_OBJECT_WORK	*parent_obj, s32 type);
static void dmLogoSegaEffectMain(OBS_OBJECT_WORK *obj_work);

static void dmLogoSegaCreateTrail(OBS_OBJECT_WORK *obj_work);

static void dmLogoSegaCamera(OBS_CAMERA *camera);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static MTS_TASK_TCB *dm_logo_sega_load_tcb = NULL;		//!< データロードTCB
static MTS_TASK_TCB *dm_logo_sega_build_tcb = NULL;		//!< データビルドTCB
static MTS_TASK_TCB *dm_logo_sega_flush_tcb = NULL;		//!< データフラッシュTCB
//static MTS_TASK_TCB *dm_logo_sega_release_tcb = NULL;	//!< データリリースTCB	// 一旦コメントアウト

static AOS_TEXTURE *dm_logo_sega_aos_tex = NULL;		//!< テクスチャ
static OBS_DATA_WORK dm_logo_sega_efct_mdl_data_work[DMD_LOGO_SEGA_EFCT_MODEL_MAX] =
								{{0, NULL}, {0, NULL}};	//!< エフェクト用モデル格納データワーク
static s32 dm_logo_sega_efct_mdl_state[DMD_LOGO_SEGA_EFCT_MODEL_MAX] =
								{-1, -1};				//!< ロードステート
static BOOL dm_logo_sega_build_state = FALSE;			//!< ビルド状況

static GSS_SND_DATA_WORK dm_sound_data_work_se	= {0};	//!< サウンドデータワーク


/// 共通データファイル名リスト
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_sega_com_fileinfo_list[] = {
	// プレイヤー
	{GSS_BASE_PATH "G_COM/PLY/SON_MDL.AMB", dmLogoSegaLoadPostFunc},
	{GSS_BASE_PATH "G_COM/PLY/SON_TEX.AMB", dmLogoSegaLoadPostFunc},
	{GSS_BASE_PATH "G_COM/PLY/SON_MTN.AMB", dmLogoSegaLoadPostFunc},
	// エフェクト
	{GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB", dmLogoSegaLoadPostFunc},
};

/// データファイル数
static const s32 dm_logo_sega_com_file_num = sizeof(dm_logo_sega_com_fileinfo_list) / sizeof(DMS_LOGO_COM_LOAD_FILE_INFO);


// GsEnvGetRegion() GSE_REGION
//	GSD_REGION_JP		= 0,				//!< 日本
//	GSD_REGION_US,							//!< 北米
//	GSD_REGION_EU,							//!< 欧州

//	GSD_REGION_NUM,							//!< リージョン数
//	GSD_REGION_DEF = GSD_REGION_US,			//!< デフォルトリージョン

/// ローカライズ関連データファイル名リスト 日本圏
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_sega_localize_fileinfo_list_region_jp[] = {
	// ロゴ
	{GSS_BASE_PATH "DEMO/LOGO/D_LOGO_SEGA_JP.AMB", dmLogoSegaLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト 北米圏
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_sega_localize_fileinfo_list_region_us[] = {
	// ロゴ
	{GSS_BASE_PATH "DEMO/LOGO/D_LOGO_SEGA_US_EU.AMB", dmLogoSegaLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト 欧州圏
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_sega_localize_fileinfo_list_region_eu[] = {
	// ロゴ
	{GSS_BASE_PATH "DEMO/LOGO/D_LOGO_SEGA_US_EU.AMB", dmLogoSegaLoadPostFunc},
};
/// ローカライズ関連データファイル名リストテーブル
static const DMS_LOGO_COM_LOAD_FILE_INFO *dm_logo_sega_localize_fileinfo_list_tbl[GSD_REGION_NUM] = {
	dm_logo_sega_localize_fileinfo_list_region_jp,
	dm_logo_sega_localize_fileinfo_list_region_us,
	dm_logo_sega_localize_fileinfo_list_region_eu,
};

/// ローカライズ関連データファイル数
static const s32 dm_logo_sega_localize_file_num = sizeof(dm_logo_sega_localize_fileinfo_list_region_jp) / sizeof(DMS_LOGO_COM_LOAD_FILE_INFO);

/// 使用エフェクトモデルデータIDテーブル
static const s32 dm_logo_sega_efct_mdl_id_tbl[DMD_LOGO_SEGA_EFCT_MODEL_MAX] = {
	IDB_EFF_CMN_EFF_SFOOT_L_ZNO,
	IDB_EFF_CMN_EFF_SFOOT_R_ZNO,
};

/// データ
static void *dm_logo_sega_data[DMD_LOGO_SEGA_DATA_MAX] = {NULL};

/// 描画オブジェクト
static OBS_ACTION3D_NN_WORK *dm_logo_sega_obj_3d_list = NULL;

/// アクション テクスチャ割り当てテーブル
static const u8 dm_logo_sega_tex_id_tbl[DMD_LOGO_SEGA_ACT_MAX] = {
	DMD_LOGO_SEGA_AOSTEX_LOCAL_ACT,
	DMD_LOGO_SEGA_AOSTEX_LOCAL_ACT,
	DMD_LOGO_SEGA_AOSTEX_LOCAL_ACT,
	DMD_LOGO_SEGA_AOSTEX_LOCAL_ACT,
	DMD_LOGO_SEGA_AOSTEX_LOCAL_ACT,
	DMD_LOGO_SEGA_AOSTEX_LOCAL_ACT,
	DMD_LOGO_SEGA_AOSTEX_LOCAL_ACT,
	DMD_LOGO_SEGA_AOSTEX_LOCAL_ACT,
};


//----- Global Functions ----------------------------------------------------
// ==========================================================================
// 初期化
// ==========================================================================
// ==========================================================================
// DmLogoSegaInit
/*!
 *	セガロゴ 初期化
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void DmLogoSegaInit(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	// カレントアカウントIDをクリア(タイトルより前にあるデモではクリアしておく必要があるため)
	AoAccountClearCurrentId();

	if (DmLogoSegaBuildCheck()) {
		// データビルド済み
		// 演出開始
		dmLogoSegaStart();
	}
	else {
#if defined (MTD_DEBUG)
		if (DmLogoSegaLoadCheck()) {
			// 中途半端にロードが始まっている	
			MTM_ASSERT(0);
		}
#endif
		// データロード開始
		MTM_TASK_MAKE_TCB(dmLogoSegaLoadWait, NULL,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_LOGO_SEGA_TASK_PRIO_DATA_LOAD, DMD_LOGO_SEGA_TASK_GROUP_DATA_LOAD,
						0/*work_size*/, "DM_LSEGA_LW");

		DmLogoSegaLoad();
	}
}


// ==========================================================================
// データロード
// ==========================================================================
// ==========================================================================
// DmLogoSegaLoad
/*!
 *	セガロゴデータロード
 *
 */
// ==========================================================================
void DmLogoSegaLoad(void)
{
	GSE_REGION	region = GsEnvGetRegion();

	// ロード処理生成
	DmLogoComLoadFileCreate(&dm_logo_sega_load_tcb);

	// 共通データ
	DmLogoComLoadFileReg(dm_logo_sega_load_tcb, &dm_logo_sega_com_fileinfo_list[0], dm_logo_sega_com_file_num);

	// ローカライズ関連データ
	DmLogoComLoadFileReg(dm_logo_sega_load_tcb, dm_logo_sega_localize_fileinfo_list_tbl[region], dm_logo_sega_localize_file_num);

	// ロードチェック開始
	DmLogoComLoadFileStart(dm_logo_sega_load_tcb);
}

// ==========================================================================
// DmLogoSegaLoadCheck
/*!
 *	セガロゴデータロード ロード終了チェック
 *
 *	@return	TRUE : ロード終了
 */
// ==========================================================================
BOOL DmLogoSegaLoadCheck(void)
{
	if ((dm_logo_sega_load_tcb == NULL) && (dm_logo_sega_data[0] != NULL)) {
		// データロードタスクがなくて 1つ目のデータが読み込まれている
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データビルド
// ==========================================================================
// ==========================================================================
// DmLogoSegaBuild
/*!
 *	セガロゴデータビルド
 *
 */
// ==========================================================================
void DmLogoSegaBuild(void)
{
	s32			i;
	void		*tex_amb[DMD_LOGO_SEGA_AOSTEX_MAX] = {NULL};
	AOS_TEXTURE	*aos_tex;

	MTM_ASSERT(DmLogoSegaLoadCheck() == TRUE);
	MTM_ASSERT(dm_logo_sega_aos_tex == NULL);

	// ビルド処理生成
	dm_logo_sega_build_tcb = MTM_TASK_MAKE_TCB(dmLogoSegaDataBuildMain, dmLogoSegaDataBuildDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_LOGO_SEGA_TASK_PRIO_DATA_BUILD, DMD_LOGO_SEGA_TASK_GROUP_DATA_BUILD,
						0/*work_size*/, "DM_LSEGA_BUILD");

//	/* オブジェクトシステム初期化 */
//	ObjInit(GMD_TASK_GROUP_OBJSYS, GMD_TASK_PRIO_OBJSYS, GMD_TASK_PAUSE_LEVEL_OBJSYS,
//				GMD_OBJ_LCD_X, GMD_OBJ_LCD_Y, GSD_DISP_HEIGHT, GSD_DISP_HEIGHT);
//
//	// データワーク数
//	ObjDataAlloc(10);

	// ライト先行設定(Build用)
	g_obj.def_user_light_flag = OBD_LIGHT_USE_FLAG_0;
#if _WII
	g_obj.def_user_light_flag |= OBD_LIGHT_USE_FLAG_7/*スペキュラ*/;
#endif	// #if _WII

	// drawflag 先行設定(Build用)
#if _PC | _XBOX | _PS3
	g_obj.load_drawflag = (NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND);
	g_obj.drawflag		= (NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND);
#elif _WII
	g_obj.load_drawflag = (0);
	g_obj.drawflag		= (0);
#endif

#if 0
	// エフェクトシステム起動
	ObjDrawESEffectSystemInit(DMD_LOGO_SEGA_TASK_PAUSELEVEL_DEF,
							  GMD_TASK_PRIO_EFFECT_SERVER,
							  GMD_TASK_GROUP_EFFECT_SERVER);

//	//描画順序設定
//	ObjDrawSetNNCommandStateTbl( 0, DMD_LOGO_SEGA_DRAW_CMD_STATE_3DNN,		TRUE );
	ObjDrawSetNNCommandStateTbl( 0, DMD_LOGO_SEGA_DRAW_CMD_STATE_PRE_MAPFAR,	TRUE );
	ObjDrawSetNNCommandStateTbl( 1, DMD_LOGO_SEGA_DRAW_CMD_STATE_MAPFAR_B,	TRUE );
	ObjDrawSetNNCommandStateTbl( 2, DMD_LOGO_SEGA_DRAW_CMD_STATE_MAPFAR_SEA,	TRUE );
	ObjDrawSetNNCommandStateTbl( 3, DMD_LOGO_SEGA_DRAW_CMD_STATE_MAPFAR_T,	TRUE );
	ObjDrawSetNNCommandStateTbl( 4, DMD_LOGO_SEGA_DRAW_CMD_STATE_POST_MAPFAR,	TRUE );
	ObjDrawSetNNCommandStateTbl( 5, DMD_LOGO_SEGA_DRAW_CMD_STATE_3DNN,		TRUE );

#endif

//	// 2Dアクションシステム設定
//	// 各種最大使用量のクリア
//	AoActSysClearPeak();


	/* ビルドシステム初期化 */
	GmGameDBuildModelBuildInit();


	/* データビルド */
	// プレイヤー
	dm_logo_sega_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)dm_logo_sega_data[DMD_LOGO_SEGA_DATA_PLY_MDL_AMB],
								(AMS_AMB_HEADER*)dm_logo_sega_data[DMD_LOGO_SEGA_DATA_PLY_TEX_AMB],
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);

	// テクスチャ管理バッファ取得
	dm_logo_sega_aos_tex = (AOS_TEXTURE*)amMemAlloc(sizeof(AOS_TEXTURE) * DMD_LOGO_SEGA_AOSTEX_MAX);
	MI_CpuClear8(dm_logo_sega_aos_tex, sizeof(AOS_TEXTURE) * DMD_LOGO_SEGA_AOSTEX_MAX);

	// テクスチャビルド
	// アクションテクスチャ
	tex_amb[DMD_LOGO_SEGA_AOSTEX_LOCAL_ACT] =
				amBindGet((AMS_AMB_HEADER*)dm_logo_sega_data[DMD_LOGO_SEGA_DATA_LOCAL_AMA_SET_AMB],
									IDB_D_LOGO_SEGA_JP_D_LOGO_SEGA_JP_AMB);
	// エフェクトモデルテクスチャ
	tex_amb[DMD_LOGO_SEGA_AOSTEX_EFFECT_MDL] =
				amBindGet((AMS_AMB_HEADER*)dm_logo_sega_data[DMD_LOGO_SEGA_DATA_EFCT_AMB],
									IDB_EFF_CMN_EFF_CMN_TEX_MD_RD_AMB);

	aos_tex = dm_logo_sega_aos_tex;
	for (i = 0; i < DMD_LOGO_SEGA_AOSTEX_MAX; i++, aos_tex++) {
		if (tex_amb[i] == NULL) {
			continue;		// データそろうまで
		}
		AoTexBuild(aos_tex, tex_amb[i]);
		AoTexLoad(aos_tex);
	}

	// エフェクト用モデル
	for (i = 0; i < DMD_LOGO_SEGA_EFCT_MODEL_MAX; i++) {
		dm_logo_sega_efct_mdl_state[i] = 
					ObjAction3dESModelLoadToDwork(&dm_logo_sega_efct_mdl_data_work[i],
								amBindGet((AMS_AMB_HEADER*)dm_logo_sega_data[DMD_LOGO_SEGA_DATA_EFCT_AMB],
										dm_logo_sega_efct_mdl_id_tbl[i]),
										0);
	}

#if 0
	/* サウンドビルド */
	GsSoundInitDataWork(&dm_sound_data_work_se);
	// SEビルド開始
	GsSoundBuildSeInit(&dm_sound_data_work_se,
					   AME_CRIAUDIO_CSB_GAME,
					   DMD_LOGO_SEGA_SOUND_SE_FILE_PATH,
					   DMD_LOGO_SEGA_TASK_PRIO_DATA_BUILD - 1);
#endif
}


// ==========================================================================
// DmLogoSegaBuildCheck
/*!
 *	セガロゴデータビルド ビルド終了チェック
 *
 *	@return	TRUE : ビルド終了
 */
// ==========================================================================
BOOL DmLogoSegaBuildCheck(void)
{
#if 1
	if (dm_logo_sega_build_state) {
#else
	if (dm_logo_sega_build_tcb == NULL &&
			dm_logo_sega_flush_tcb == NULL &&
			dm_logo_sega_obj_3d_list != NULL) {
#endif
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データフラッシュ
// ==========================================================================
// ==========================================================================
// DmLogoSegaFlush
/*!
 *	セガロゴデータフラッシュ
 *
 */
// ==========================================================================
void DmLogoSegaFlush(void)
{
	s32				i;
	AOS_TEXTURE		*aos_tex;
	AMS_AMB_HEADER	*amb;

	// フラッシュ処理生成
	dm_logo_sega_flush_tcb = MTM_TASK_MAKE_TCB(dmLogoSegaDataFlushMain, dmLogoSegaDataFlushDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_LOGO_SEGA_TASK_PRIO_DATA_FLUSH, DMD_LOGO_SEGA_TASK_GROUP_DATA_FLUSH,
						0/*work_size*/, "DM_TOP_FLUSH");

	// フラッシュシステム初期化
	GmGameDBuildModelFlushInit();
	
	// プレイヤー
	amb = (AMS_AMB_HEADER*)dm_logo_sega_data[DMD_LOGO_SEGA_DATA_PLY_MDL_AMB];
	GmGameDBuildRegFlushModel(dm_logo_sega_obj_3d_list, amb->file_num);

	// テクスチャフラッシュ
	aos_tex = dm_logo_sega_aos_tex;
	for (i = 0; i < DMD_LOGO_SEGA_AOSTEX_MAX; i++, aos_tex++) {
		AoTexRelease(aos_tex);
	}

	// エフェクト用モデル
	for (i = 0; i < DMD_LOGO_SEGA_EFCT_MODEL_MAX; i++) {
		dm_logo_sega_efct_mdl_state[i] =
				ObjAction3dESModelReleaseDwork(&dm_logo_sega_efct_mdl_data_work[i]);
	}

#if 0
	// サウンドフラッシュ
	GsSoundFlushSe(&dm_sound_data_work_se);
#endif
}

// ==========================================================================
// DmLogoSegaFlushCheck
/*!
 *	セガロゴデータフラッシュ フラッシュ終了チェック
 *
 *	@return	TRUE : フラッシュ終了
 */
// ==========================================================================
BOOL DmLogoSegaFlushCheck(void)
{
#if 1
	if (!dm_logo_sega_build_state) {
#else
	if (dm_logo_sega_build_tcb == NULL &&
			dm_logo_sega_flush_tcb == NULL &&
			dm_logo_sega_obj_3d_list == NULL) {
#endif
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データリリース
// ==========================================================================
// ==========================================================================
// DmLogoSegaRelease
/*!
 *	セガロゴデータリリース
 */
// ==========================================================================
void DmLogoSegaRelease(void)
{
	s32	i;

	for (i = 0; i < DMD_LOGO_SEGA_DATA_MAX; i++) {
		if (dm_logo_sega_data[i]) {
			amMemFree(dm_logo_sega_data[i]);
		}
		dm_logo_sega_data[i] = NULL;
	}
}

// ==========================================================================
// DmLogoSegaReleaseCheck
/*!
 *	セガロゴデータリリース リリース終了チェック
 *
 *	@return	TRUE : リリース終了
 */
// ==========================================================================
BOOL DmLogoSegaReleaseCheck(void)
{
	if ((dm_logo_sega_load_tcb == NULL) && (dm_logo_sega_data[0] == NULL)) {
		// データロードタスクがなくて 1つ目のデータが読み込まれていない
		return (TRUE);
	}
	return (FALSE);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// メイン処理
// ==========================================================================
// ==========================================================================
// dmLogoSegaStart
/*!
 *	セガロゴ開始
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSegaStart(void)
{
	MTS_TASK_TCB		*tcb;
	DMS_LOGO_SEGA_WORK *logo_work;

	NNS_RGBA		diffuse = { 1.0f, 1.0f, 1.0f, 1.0f};
	NNS_RGB			ambient = { 1.0f, 1.0f, 1.0f};

	// オブジェクトシステム初期化
	dmLogoSegaObjSysytemInit();

	// サウンド初期化
	GsSoundReset();
	GsSoundBegin(GMD_TASK_NO_GAME_PAUSE,
				 DMD_LOGO_SEGA_TASK_PRIO_SOUND,
				 DMD_LOGO_SEGA_TASK_GROUP_SOUND);

	// メイン処理開始
	tcb = MTM_TASK_MAKE_TCB(dmLogoSegaMainFunc, NULL,
						0/*flag*/, DMD_LOGO_SEGA_TASK_PAUSELEVEL_DEF,
						DMD_LOGO_SEGA_TASK_PRIO_MAIN, DMD_LOGO_SEGA_TASK_GROUP_MAIN,
						sizeof(DMS_LOGO_SEGA_WORK), "DM_LSEGA_MAIN");
	logo_work = (DMS_LOGO_SEGA_WORK*)mtTaskGetTcbWork(tcb);
	MI_CpuClear8(logo_work, sizeof(DMS_LOGO_SEGA_WORK));

	// 環境初期化
	nnSetPrimitive3DMaterial(&diffuse, &ambient, 1.0f);

	AoActSysSetDrawStateEnable(FALSE);

	// アクション初期化
	dmLogoSegaActionCreate(logo_work);

	// プレイヤーオブジェクト生成
	logo_work->ply_obj = dmLogoSegaCreatePlayer();

	// 開始待機へ
	logo_work->timer = 0;
	logo_work->func = dmLogoSegaStartWaitFunc;

	/* サウンドビルド */
	GsSoundInitDataWork(&dm_sound_data_work_se);
	// SEビルド開始
	GsSoundBuildSeInit(&dm_sound_data_work_se,
					   AME_CRIAUDIO_CSB_GAME,
					   DMD_LOGO_SEGA_SOUND_SE_FILE_PATH,
					   DMD_LOGO_SEGA_TASK_PRIO_DATA_BUILD - 1);

	// サウンドハンドル取得
	logo_work->h_se = GsSoundAllocSeHandle();
}

// ==========================================================================
// dmLogoSegaObjSysytemInit
/*!
 *	オブジェクトシステム初期化
 */
// ==========================================================================
void dmLogoSegaObjSysytemInit(void)
{
	NNS_RGBA	light_col = {
		1.0f, 1.0f, 1.0f, 1.0f,
	};
	NNS_VECTOR	light_vec;
	NNS_VECTOR	cam_pos = {0.f, 0.f, 50.f};
	OBS_CAMERA	*camera;

	/* オブジェクトシステム初期化 */
	ObjInit(GMD_TASK_GROUP_OBJSYS, GMD_TASK_PRIO_OBJSYS, GMD_TASK_PAUSE_LEVEL_OBJSYS,
				GMD_OBJ_LCD_X, GMD_OBJ_LCD_Y, GSD_DISP_HEIGHT, GSD_DISP_HEIGHT);

	// データワーク数
	ObjDataAlloc(10);

	// エフェクトシステム起動
	ObjDrawESEffectSystemInit(DMD_LOGO_SEGA_TASK_PAUSELEVEL_DEF,
							  DMD_LOGO_SEGA_TASK_PRIO_EFFECT_SERVER,
							  DMD_LOGO_SEGA_TASK_GROUP_EFFECT_SERVER);

	//描画順序設定
	ObjDrawSetNNCommandStateTbl( 0, DMD_LOGO_SEGA_DRAW_CMD_STATE_3DNN, TRUE );

	// 2Dアクションシステム設定
	// 各種最大使用量のクリア
	AoActSysClearPeak();

	/* オブジェクトシステム設定 */
	g_obj.flag = OBD_OBJ_CAMERA | OBD_OBJ_RECT_NOUSE_DRAWSCALE | OBD_OBJ_DEFAULT_NOCLIP;

	// ライト設定
#if _WII
	g_obj.def_user_light_flag |= OBD_LIGHT_USE_FLAG_7/*スペキュラ*/;
#endif	// #if _WII

	g_obj.ppPre			= NULL;//GmObjPreFunc;		// システム前処理
	g_obj.ppPost		= NULL;						// システム後処理
	//g_obj.ppDrawSort	= GmObjDrawSort;			// 描画前オブジェクトソート
	g_obj.ppCollision	= NULL;//GmObjCollision;	// あたり処理
	g_obj.ppObjPre		= NULL;//GmObjObjPreFunc;	// オブジェクト共通前処理
	g_obj.ppObjPost		= NULL;						// オブジェクト共通後処理
	g_obj.ppRegRecAuto	= NULL;						// 矩形自動登録処理

	// 描画スケール設定
	//g_obj.draw_scale.x = g_obj.draw_scale.y = g_obj.draw_scale.z = GMD_OBJ_DRAW_SCALE_FX;
	g_obj.draw_scale.x = g_obj.draw_scale.y = g_obj.draw_scale.z =
				(fx32)(((3.2f/* *3.2で64dot扱い */) * (1.f / DMD_LOGO_SEGA_CAMERA_SCALE * 2)) * FX32_ONE);
	// 逆数保存
	g_obj.inv_draw_scale.x = g_obj.inv_draw_scale.y = g_obj.inv_draw_scale.z = FX_Div(FX32_ONE, g_obj.draw_scale.x);
	// 画面奥行き度
	g_obj.depth = 0x0080;

	/* ライト設定 */
	// アンビエントカラー
	g_obj.ambient_color.r = 0.8f;
	g_obj.ambient_color.g = 0.8f;
	g_obj.ambient_color.b = 0.8f;

	// パラレルライト
	light_vec.x = -1.0f;
	light_vec.y = -1.0f;
	light_vec.z = -1.0f;
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_0, &light_col, 1.f, &light_vec);

#if _WII
	// Wii用スペキュラーGCライト
	light_col.r = 1.f;
	light_col.g = 1.f;
	light_col.b = 1.f;
	// light_vec はパラレルライトを調整
	light_vec.x /= 2.f;
	//light_vec.y = -1.f;
	light_vec.z = 0.f;
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetSpecularGCLight(NNE_LIGHT_7, &light_col, &light_vec);

	// Wii用トゥーンライト
	g_obj.toon_light_vec = light_vec;
#endif

	/* カメラ設定 */
	ObjCameraInit(DMD_LOGO_SEGA_CAMERA_ID, &cam_pos,
				DMD_LOGO_SEGA_TASK_GROUP_CAMERA,
				DMD_LOGO_SEGA_TASK_PAUSELEVEL_DEF,
				DMD_LOGO_SEGA_TASK_PRIO_CAMERA);
	ObjCamera3dInit(DMD_LOGO_SEGA_CAMERA_ID);
	g_obj.glb_camera_id = DMD_LOGO_SEGA_CAMERA_ID;
	g_obj.glb_camera_type = NNE_PROJECTION_TYPE_ORTHO;
	camera = ObjCameraGet(DMD_LOGO_SEGA_CAMERA_ID);
	camera->user_func = dmLogoSegaCamera;
	camera->command_state = DMD_LOGO_SEGA_DRAW_CMD_STATE_3DNN;
	camera->scale = DMD_LOGO_SEGA_CAMERA_SCALE; // 0.2997259375f;//
	camera->ofst.z = 1000.0f;
	//camera->fovy = NNM_DEGtoA32(40.0f);
	//camera->znear = 0.1f;
	//camera->zfar = 32768.0f;

	// 軌跡エフェクトシステム初期化
	amTrailEFInitialize();
}


// ==========================================================================
// 終了処理
// ==========================================================================
// ==========================================================================
// dmLogoSegaPreEnd
/*!
 *	セガロゴ終了 前処理
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSegaPreEnd(DMS_LOGO_SEGA_WORK *logo_work)
{
	OBS_OBJECT_WORK	*obj_work;

	UNREFERENCED_PARAMETER(logo_work);

	// 軌跡エフェクト破棄
	amTrailEFDeleteGroup(AMTRE_HANDLE_ACCELL);

	// 全オブジェクトppOutクリア
	// データ解放を行わないので、描画をとめて描画発行済みのものが終了するまで待つ
	obj_work = ObjObjectSearchRegistObject(NULL, 0xFFFF);
	while (obj_work) {
		obj_work->ppOut = NULL;
		obj_work = ObjObjectSearchRegistObject(obj_work, 0xFFFF);
	}
}

// ==========================================================================
// dmLogoSegaEnd
/*!
 *	セガロゴ終了
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSegaEnd(DMS_LOGO_SEGA_WORK *logo_work)
{
	// アクション破棄
	dmLogoSegaActionDelete(logo_work);

	// オブジェクトシステム前処理クリア
	g_obj.ppPre			= NULL;

	// 管理オブジェクト破棄
	ObjObjectClearAllObject();
	// オブジェクトシステム終了前処理
	ObjPreExit();
	// エフェクトシステム終了
	MTM_ASSERT(ObjDrawESEffectSystemIsActive());
	ObjDrawESEffectSystemExit();
	// オブジェクトシステム終了
	ObjExit();

	// サウンドハンドル解放
	GsSoundStopSeHandle(logo_work->h_se);
	GsSoundFreeSeHandle(logo_work->h_se);

	// サウンドシステム終了
	GsSoundHalt();
	GsSoundEnd();
	GsSoundReset();

	// サウンドフラッシュ (フラッシュ待機なし)
	GsSoundFlushSe(&dm_sound_data_work_se);
}


// ==========================================================================
// メイン処理
// ==========================================================================
// ==========================================================================
// dmLogoSegaMainFunc
/*!
 *	メイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSegaMainFunc(MTS_TASK_TCB *tcb)
{
	s32					i;
	DMS_LOGO_SEGA_WORK	*logo_work;
	float				update_frame;

	logo_work = (DMS_LOGO_SEGA_WORK*)mtTaskGetTcbWork(tcb);

	/* SEボリューム調整 */
	if (GsSystemBgmIsPlay()) {
		// システムBGM再生中
		logo_work->h_se->snd_ctrl_param.volume = 0.f;
	}
	else {
		logo_work->h_se->snd_ctrl_param.volume = 1.f;
	}

	if (AoSysIsShowPlatformUI()) {
		// フェード1フレーム停止
		if (IzFadeIsExe()) {
			IzFadeSetStopUpdate1Frame(NULL);
		}
	}
	else {
		/* 演出 */
		if (logo_work->func) {
			logo_work->func(logo_work);
		}

		/* 終了チェック */
		if (logo_work->flag & DMD_LOGO_SEGA_FLAG_END) {
			// 演出終了

		//	// セガロゴ終了
		//	dmLogoSegaEnd(logo_work);
			// 終了前処理
			dmLogoSegaPreEnd(logo_work);

			// オブジェクトシステム終了待機
			mtTaskChangeTcbProcedure(tcb, gmLogoSegaPreEndWaitFunc);
			logo_work->timer = 0;


	//		// メイン処理破棄
	//		mtTaskClearTcb(tcb);

	//		// データのFlush, Releaseはメニューを抜ける時に一括で行う
	//
	//		// 次のイベントへ
	//		SyChangeNextEvt();
			return;
		}
	}

	/* 描画 */
	update_frame = 0.f;
	if (!AoSysIsShowPlatformUI() &&	// システムメニュー未表示時のみ更新
			(logo_work->flag & DMD_LOGO_SEGA_FLAG_ACT_FRAME_UPDATE)) {
		update_frame = 1.f;
	}
	AoActSysSetDrawTaskPrio();	// 標準設定
	for (i = 0; i < DMD_LOGO_SEGA_ACT_MAX; i++) {
		AoActSetTexture(AoTexGetTexList(dm_logo_sega_aos_tex + dm_logo_sega_tex_id_tbl[i]));
		AoActUpdate(logo_work->act[i], update_frame);
		AoActDraw(logo_work->act[i]);
	}

	// 軌跡エフェクト
	{
		NNS_RGBA		diffuse = { 1.0f, 1.0f, 1.0f, 1.0f};
		NNS_RGB			ambient = { 1.0f, 1.0f, 1.0f};
		NNS_TEXLIST		*texlist;

		// 更新
		if (!AoSysIsShowPlatformUI()) {
			// システムメニュー未表示時のみ更新
			amTrailEFUpdate(AMTRE_HANDLE_ACCELL);
		}
		if (g_obj.glb_camera_id != -1) {
			// カメラ
			NNS_VECTOR	sort_cam_pos;
			NNS_VECTOR	cam_ofst;
			NNS_MATRIX	obj_mtx;

			nnMakeUnitMatrix(&obj_mtx);	// 変更必要？

			// 3DNNのカメラ設定
			ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, g_obj.glb_camera_type, DMD_LOGO_SEGA_DRAW_CMD_STATE_3DNN);
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
		}
		
		// 描画
		nnSetPrimitive3DMaterial(&diffuse, &ambient, 1.0f);
		texlist = dm_logo_sega_aos_tex[DMD_LOGO_SEGA_AOSTEX_LOCAL_ACT].texlist;
		amTrailEFDraw(AMTRE_HANDLE_ACCELL, texlist, DMD_LOGO_SEGA_DRAW_CMD_STATE_3DNN);
	}
}

// ==========================================================================
// gmLogoSegaPreEndWaitFunc
/*!
 *	オブジェクト破棄開始待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmLogoSegaPreEndWaitFunc(MTS_TASK_TCB *tcb)
{
	DMS_LOGO_SEGA_WORK	*logo_work;

	logo_work = (DMS_LOGO_SEGA_WORK*)mtTaskGetTcbWork(tcb);

	logo_work->timer++;
	if (logo_work->timer > 2) {
		// セガロゴ終了
		dmLogoSegaEnd(logo_work);
		// 終了処理待機へ
		mtTaskChangeTcbProcedure(tcb, gmLogoSegaEndWaitFunc);
	}
}

// ==========================================================================
// gmLogoSegaEndWaitFunc
/*!
 *	オブジェクト破棄終了待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmLogoSegaEndWaitFunc(MTS_TASK_TCB *tcb)
{
	if (!ObjObjectCheckClearAllObject()) {
		// オブジェクトの終了待ち
		return;
	}

	if (ObjIsExitWait()) {
		// オブジェクトシステムの終了待ち
		return;
	}

	// データフラッシュ
	DmLogoSegaFlush();
	// フラッシュ処理待機へ
	mtTaskChangeTcbProcedure(tcb, gmLogoSegaFlushWaitFunc);
//	// 終了待機破棄
//	mtTaskClearTcb(tcb);
//
//	// 次のイベントへ
//	SyChangeNextEvt();
}

// ==========================================================================
// gmLogoSegaFlushWaitFunc
/*!
 *	データフラッシュ待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmLogoSegaFlushWaitFunc(MTS_TASK_TCB *tcb)
{
	if (!DmLogoSegaFlushCheck()) {
		return;
	}

	// データフリリース
	DmLogoSegaRelease();
	// リリース処理待機へ
	mtTaskChangeTcbProcedure(tcb, gmLogoSegaRelesehWaitFunc);

}

// ==========================================================================
// gmLogoSegaRelesehWaitFunc
/*!
 *	データリリース待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmLogoSegaRelesehWaitFunc(MTS_TASK_TCB *tcb)
{
	if (!DmLogoSegaReleaseCheck()) {
		return;
	}
	// 終了待機破棄
	mtTaskClearTcb(tcb);
	
	// 次のイベントへ
	SyChangeNextEvt();
	
	// セガロゴのデモとして起動されたかどうか
	if (Sonic4_GetLogoDemoFlag())
	{
		// 終了フラグを設置
		Sonic4_SetLogoDemoEnd();
	}
}



// ==========================================================================
// 演出
// ==========================================================================
// ==========================================================================
// dmLogoSegaStartWaitFunc
/*!
 *	開始待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSegaStartWaitFunc(DMS_LOGO_SEGA_WORK *logo_work)
{
	BOOL	b_sts = TRUE;

	logo_work->timer++;

	// サウンドビルド
	if (GsSoundBuildSeUpdate(&dm_sound_data_work_se) == FALSE) {
		b_sts = FALSE;
	}

	if (logo_work->timer >= DMD_LOGO_SEGA_START_WAIT_TIME && b_sts) {
		// フェード終了
		IzFadeExit();

		// ソニック左走りへ
#if !_IPHONE
		logo_work->timer = 0;
#else //!_IPHONE
		logo_work->timer = 4; //ぎりぎりまで表示時間を早める
#endif //!_IPHONE
		logo_work->func = dmLogoSegaRunLeftFunc;

		// ソニックシーケンス切り替え
		dmLogoSegaPlayerInitSeqRunLeft(logo_work->ply_obj);

		// アクション更新開始へ
		logo_work->flag |= DMD_LOGO_SEGA_FLAG_ACT_FRAME_UPDATE;

		// エフェクト生成
		logo_work->efct_obj = dmLogoSegaCreateDashEffect(logo_work->ply_obj, DMD_LOGO_SEGA_EFCT_MODEL_FOOT_L);
	}
}

// ==========================================================================
// dmLogoSegaFadeInWaitFunc
/*!
 *	ソニック左走り
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSegaRunLeftFunc(DMS_LOGO_SEGA_WORK *logo_work)
{
	logo_work->timer++;
	if (logo_work->timer > DMD_LOGO_SEGA_SONIC_RUN_LEFT_TIME) {
		// ソニック右走り待機へ
		logo_work->timer = 0;
		logo_work->func = dmLogoSegaRunRightWaitFunc;

		// エフェクト破棄
		logo_work->efct_obj->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		logo_work->efct_obj = NULL;
	}
}

// ==========================================================================
// dmLogoSegaRunRightWaitFunc
/*!
 *	右走り開始待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSegaRunRightWaitFunc(DMS_LOGO_SEGA_WORK *logo_work)
{
	logo_work->timer++;
	if (logo_work->timer > DMD_LOGO_SEGA_SONIC_RUN_RIGHT_WAIT_TIME) {
		// ソニック右走りへ
		logo_work->timer = 0;
		logo_work->func = dmLogoSegaRunRightFunc;

		// ソニックシーケンス切り替え
		dmLogoSegaPlayerInitSeqRunRight(logo_work->ply_obj);

		// エフェクト生成
		logo_work->efct_obj = dmLogoSegaCreateDashEffect(logo_work->ply_obj, DMD_LOGO_SEGA_EFCT_MODEL_FOOT_R);
	}
}

// ==========================================================================
// dmLogoSegaRunRightFunc
/*!
 *	ソニック右走り
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSegaRunRightFunc(DMS_LOGO_SEGA_WORK *logo_work)
{
	logo_work->timer++;
	if (logo_work->timer > DMD_LOGO_SEGA_SONIC_RUN_LEFT_TIME) {
		// 表示待機へ
		logo_work->timer = 0;
		logo_work->func = dmLogoSegaDispWaitFunc;

		{// せ～が～
			// セガロゴのデモとして起動されたかどうか
			if (Sonic4_GetLogoDemoFlag())
			{
				// このときはCRIAudioライブラリは使用しない
				Sonic4_LogoDemoSoundPlay();
			}
			else {
				GsSoundPlaySe("Sega_Logo", logo_work->h_se);
				if (GsSystemBgmIsPlay()) {
					// システムBGM再生中
					logo_work->h_se->snd_ctrl_param.volume = 0.f;
				}
			}
		}
	}
}

// ==========================================================================
// dmLogoSegaDispWaitFunc
/*!
 *	表示待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSegaDispWaitFunc(DMS_LOGO_SEGA_WORK *logo_work)
{
	logo_work->timer++;
	if (logo_work->timer >= DMD_LOGO_SEGA_DISP_TIME) {
		// フェードアウトへ移行
		logo_work->func = dmLogoSegaFadeOutWaitFunc;

		// フェード開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEOUT,
									DMD_LOGO_SEGA_FADEOUT_TIME, TRUE);
		return;
	}
}

// ==========================================================================
// dmLogoSegaFadeOutWaitFunc
/*!
 *	フェード待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSegaFadeOutWaitFunc(DMS_LOGO_SEGA_WORK *logo_work)
{
	if (IzFadeIsEnd()) {
		// 演出終了
		logo_work->flag |= DMD_LOGO_SEGA_FLAG_END;
	}
}



// ==========================================================================
// プレイヤー
// ==========================================================================
// ==========================================================================
// dmLogoSegaCreatePlayer
/*!
 *	プレイヤー生成
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
OBS_OBJECT_WORK* dmLogoSegaCreatePlayer(void)
{
	DMS_LOGO_SEGA_OBJ_3DNN_WORK	*ls3d_work;
	OBS_OBJECT_WORK				*obj_work;
	OBS_ACTION3D_NN_WORK		*obj_3d;


	obj_work = OBM_OBJECT_TASK_DETAIL_INIT(DMD_LOGO_SEGA_TASK_PRIO_PLAYER,
							DMD_LOGO_SEGA_TASK_GROUP_PLAYER,
							DMD_LOGO_SEGA_TASK_PAUSELEVEL_DEF,
							DMD_LOGO_SEGA_OBJ_OBJPAUSELEVEL_DEF,
							sizeof(DMS_LOGO_SEGA_OBJ_3DNN_WORK), "DM_LSEGA_PLY");

	ls3d_work = (DMS_LOGO_SEGA_OBJ_3DNN_WORK*)obj_work;

	obj_work->obj_type = DMD_LOGO_SEGA_OBJTYPE_PLAYER;

	// 標準関数設定
	obj_work->ppOut			= ObjDrawActionSummary;///dmLogoSegaObjDraw;
	obj_work->ppOutSub		= NULL;
	obj_work->ppIn			= NULL;
	obj_work->ppMove		= NULL;
	obj_work->ppActCall		= NULL;
	obj_work->ppRec			= NULL;
	obj_work->ppLast		= NULL;
	obj_work->ppFunc		= NULL;

	// モデル初期化
	obj_3d = &ls3d_work->obj_3d;
	ObjObjectCopyAction3dNNModel(obj_work,
			&dm_logo_sega_obj_3d_list[IDB_SON_MDL_SON_MODEL_ZNO],
			obj_3d);

	// トゥーン設定
	ObjDrawSetToon(obj_3d);

	obj_3d->command_state = DMD_LOGO_SEGA_DRAW_CMD_STATE_3DNN;

#if _WII
	// Wiiは専用マテリアルコールバック設定
	obj_3d->material_cb_func = dmLogoSegaPlayerMaterialCallback;
#endif

	// モーションロード
	ObjDataSet(&ls3d_work->data_work, dm_logo_sega_data[DMD_LOGO_SEGA_DATA_PLY_MTN_AMB]);
	ls3d_work->data_work.num |= OBD_DATA_ARCHIVE_FLAG;
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, FALSE/*marge*/,
									&ls3d_work->data_work/*data_work*/, NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/,
									GMD_PLAYER_MTN_NUM/*motion_num*/, AMD_MOTION_MATERIAL_DEFAULT_MAX);

	obj_work->disp_flag |= OBD_DISP_3D_PARALLEL | OBD_DISP_USERMTX_RIGHT;

	// 表示サイズ調整
	obj_work->scale.x = obj_work->scale.y = obj_work->scale.z = 2*FX32_ONE;

	// user_obj_mtx_r設定
	nnMakeUnitMatrix(&obj_3d->user_obj_mtx_r);
	nnTranslateMatrix(&obj_3d->user_obj_mtx_r, &obj_3d->user_obj_mtx_r,
			0,		// Z
			((float)DMD_LOGO_SEGA_PLAYER_POS_Y_ADJUST / FXM_FX32_TO_FLOAT(g_obj.draw_scale.y)),
			0);		// X

	// 初期位置設定
	obj_work->pos.x = DMD_LOGO_SEGA_PLAYER_INIT_POS_X*FX32_ONE;
	obj_work->pos.y = DMD_LOGO_SEGA_PLAYER_INIT_POS_Y*FX32_ONE;
	obj_work->pos.z = DMD_LOGO_SEGA_PLAYER_INIT_POS_Z*FX32_ONE;

	return (obj_work);
}

#if _WII
// ==========================================================================
// dmLogoSegaPlayerMaterialCallback
/*!
 *	Wii用 プレイヤーマテリアルコールバック
 *
 *	@param val				[in]	NNS_DRAWCALLBACK_VAL構造体へのポインタ
 *	@param param			[in]	ユーザーパラメータ(GMS_PLAYER_MAT_CALLBACK_PARAM)
 *
 *	@return		NNE_BOOL
 */
// ==========================================================================
NNE_BOOL dmLogoSegaPlayerMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param)
{
	u32								user_data;

	// ユーザーデータ取得
	user_data = ObjDraw3DNNGetMaterialUserData(val);

	// グーにする
	if (!user_data ||
			(user_data & GMD_PLY_LOD_TYPE_R_MASK) == GMD_PLY_LOD_TYPE_R_GU ||
			(user_data & GMD_PLY_LOD_TYPE_L_MASK) == GMD_PLY_LOD_TYPE_L_GU) {
		// Toon汎用処理
		return (ObjDrawToonMaterialCallback(val, param));
	}

	return (NNE_FALSE);
}
#endif	// #if _WII

// ==========================================================================
// dmLogoSegaPlayerInitSeqRunLeft
/*!
 *	プレイヤー処理 左走り初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void dmLogoSegaPlayerInitSeqRunLeft(OBS_OBJECT_WORK *obj_work)
{
	// 座標設定
	obj_work->pos.x = DMD_LOGO_SEGA_PLAYER_LEFT_RUN_S_POS_X*FX32_ONE;

	// アクション設定
	ObjDrawObjectActionSet(obj_work, IDB_SON_MTN_SON_DASH2_L_ZNM);
	obj_work->disp_flag |= OBD_DISP_HFLIP | OBD_DISP_REPEAT;

	// エフェクト生成
	dmLogoSegaCreateTrail(obj_work);

	// 処理設定
	obj_work->ppFunc = dmLogoSegaPlayerSeqRunLeft;
}

// ==========================================================================
// dmLogoSegaPlayerSeqRunLeft
/*!
 *	プレイヤー処理 左走り
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void dmLogoSegaPlayerSeqRunLeft(OBS_OBJECT_WORK *obj_work)
{
	if (AoSysIsShowPlatformUI()) {
		obj_work->disp_flag |= OBD_DISP_NOUPDATE;
		return;
	}
	obj_work->disp_flag &= ~OBD_DISP_NOUPDATE;

	// 移動
	obj_work->pos.x += DMD_LOGO_SEGA_PLAYER_LEFT_RUN_SPD * FX32_ONE;
}

// ==========================================================================
// dmLogoSegaPlayerInitSeqRunRight
/*!
 *	プレイヤー処理 右走り初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void dmLogoSegaPlayerInitSeqRunRight(OBS_OBJECT_WORK *obj_work)
{
	// 座標設定
	obj_work->pos.x = DMD_LOGO_SEGA_PLAYER_RIGHT_RUN_S_POS_X*FX32_ONE;

	// アクション設定
	ObjDrawObjectActionSet(obj_work, IDB_SON_MTN_SON_DASH2_ZNM);
	obj_work->disp_flag &= ~OBD_DISP_HFLIP;
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// エフェクト生成
	dmLogoSegaCreateTrail(obj_work);

	// 処理設定
	obj_work->ppFunc = dmLogoSegaPlayerSeqRunRight;
}

// ==========================================================================
// dmLogoSegaPlayerSeqRunRight
/*!
 *	プレイヤー処理 右走り
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void dmLogoSegaPlayerSeqRunRight(OBS_OBJECT_WORK *obj_work)
{
	if (AoSysIsShowPlatformUI()) {
		obj_work->disp_flag |= OBD_DISP_NOUPDATE;
		return;
	}
	obj_work->disp_flag &= ~OBD_DISP_NOUPDATE;

	// 移動
	obj_work->pos.x += DMD_LOGO_SEGA_PLAYER_RIGHT_RUN_SPD * FX32_ONE;
}

// ==========================================================================
// ダッシュエフェクト
// ==========================================================================
// ==========================================================================
// dmLogoSegaCreateDashEffect
/*!
 *	ダッシュエフェクト生成
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
OBS_OBJECT_WORK* dmLogoSegaCreateDashEffect(OBS_OBJECT_WORK	*parent_obj, s32 type)
{
	DMS_LOGO_SEGA_OBJ_ES_WORK	*efct_work;
	OBS_OBJECT_WORK				*obj_work;
	s32							act_id;

	obj_work = OBM_OBJECT_TASK_DETAIL_INIT(DMD_LOGO_SEGA_TASK_PRIO_EFFECT,
							DMD_LOGO_SEGA_TASK_GROUP_EFFECT,
							DMD_LOGO_SEGA_TASK_PAUSELEVEL_DEF,
							DMD_LOGO_SEGA_OBJ_OBJPAUSELEVEL_DEF,
							sizeof(DMS_LOGO_SEGA_OBJ_ES_WORK), "DM_LSEGA_EFCT");
	efct_work = (DMS_LOGO_SEGA_OBJ_ES_WORK*)obj_work;

	obj_work->obj_type = DMD_LOGO_SEGA_OBJTYPE_EFFECT;

	// 標準関数設定
	obj_work->ppOut			= ObjDrawActionSummary;///dmLogoSegaObjDraw;
	obj_work->ppOutSub		= NULL;
	obj_work->ppIn			= NULL;
	obj_work->ppMove		= NULL;
	obj_work->ppActCall		= NULL;
	obj_work->ppRec			= NULL;
	obj_work->ppLast		= NULL;
	obj_work->ppFunc		= dmLogoSegaEffectMain;

	// 親オブジェクト保存
	obj_work->parent_obj = parent_obj;
	// 座標コピー
	obj_work->pos = parent_obj->pos;

	if (type == DMD_LOGO_SEGA_EFCT_MODEL_FOOT_R) {
		act_id = IDB_EFF_CMN_EFF_ROLLDASH_R_AME;
	}
	else {
		act_id = IDB_EFF_CMN_EFF_ROLLDASH_L_AME;
	}

	// エフェクトアクションロード
	ObjObjectAction3dESEffectLoad(obj_work, &efct_work->obj_3des,
								   NULL/*data_work*/, NULL/*filename*/,
								   act_id/*index*/,
								   dm_logo_sega_data[DMD_LOGO_SEGA_DATA_EFCT_AMB]/*archive*/,
								   0/*user_attr=0*/, 0/*ecb_prio=0*/);

	efct_work->obj_3des.command_state = DMD_LOGO_SEGA_DRAW_CMD_STATE_3DNN;

	// テクスチャセット
	ObjDataSet(&efct_work->data_work_texamb,
						amBindGet((AMS_AMB_HEADER*)dm_logo_sega_data[DMD_LOGO_SEGA_DATA_EFCT_AMB],
						IDB_EFF_CMN_EFF_CMN_TEX_AMB));
	efct_work->data_work_texamb.num |= OBD_DATA_ARCHIVE_FLAG;
	ObjDataSet(&efct_work->data_work_texlist, dm_logo_sega_aos_tex[DMD_LOGO_SEGA_AOSTEX_EFFECT_MDL].texlist);
	efct_work->data_work_texlist.num |= OBD_DATA_ARCHIVE_FLAG;

	ObjObjectAction3dESTextureLoad(obj_work, &efct_work->obj_3des,
								&efct_work->data_work_texamb, NULL/*filename*/,
								0/*index*/, NULL/*archive*/,
								FALSE);
	ObjObjectAction3dESTextureSetByDwork(obj_work, &efct_work->data_work_texlist);

	// モデル
	ObjDataSet(&efct_work->data_work_model,
			amBindGet((AMS_AMB_HEADER*)dm_logo_sega_data[DMD_LOGO_SEGA_DATA_EFCT_AMB],
										dm_logo_sega_efct_mdl_id_tbl[type]));
	efct_work->data_work_texlist.num |= OBD_DATA_ARCHIVE_FLAG;

	ObjObjectAction3dESModelLoad(obj_work, &efct_work->obj_3des,
									 &efct_work->data_work_model,
									 NULL/*filename*/,
									 0/*index*/, NULL/*archive*/,
									 0/*draw_flag*/, FALSE);

	ObjObjectAction3dESModelSetByDwork(obj_work, &dm_logo_sega_efct_mdl_data_work[type]);


	// 角度設定
	efct_work->obj_3des.disp_rot.x	= 0;//(u16)(-45 * 0x10000 / 360);
	efct_work->obj_3des.disp_rot.y	= 0;
	efct_work->obj_3des.disp_rot.z	= 0;

	// スケール設定
	obj_work->scale.x = (fx32)(FX_Mul(parent_obj->scale.x, g_obj.draw_scale.x));
	obj_work->scale.y = (fx32)(FX_Mul(parent_obj->scale.y, g_obj.draw_scale.y));
	obj_work->scale.z = (fx32)(FX_Mul(parent_obj->scale.z, g_obj.draw_scale.z));

	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	efct_work->obj_3des.flag |= OBD_ACTFLAG_3D_ES_SCALE_BY_MTX;

	return (obj_work);
}

// ==========================================================================
// dmLogoSegaEffectMain
/*!
 *	エフェクトメイン処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void dmLogoSegaEffectMain(OBS_OBJECT_WORK *obj_work)
{
	OBS_OBJECT_WORK	*parent_obj = obj_work->parent_obj;

	if (AoSysIsShowPlatformUI()) {
		obj_work->disp_flag |= OBD_DISP_NOUPDATE;
		return;
	}
	obj_work->disp_flag &= ~OBD_DISP_NOUPDATE;

	// 座標コピー
	obj_work->pos = parent_obj->pos;
	// オフセット加算
	if (obj_work->parent_obj->disp_flag & OBD_DISP_HFLIP) {
		obj_work->pos.x -= DMD_LOGO_SEGA_DASH_EFCT_OFST_POS_X*FX32_ONE;
	}
	else {
		obj_work->pos.x += DMD_LOGO_SEGA_DASH_EFCT_OFST_POS_X*FX32_ONE;
	}
	obj_work->pos.y += DMD_LOGO_SEGA_DASH_EFCT_OFST_POS_Y*FX32_ONE;
	obj_work->pos.z += DMD_LOGO_SEGA_DASH_EFCT_OFST_POS_Z*FX32_ONE;

	// フラグコピー
	obj_work->disp_flag &= ~OBD_DISP_HFLIP;
	obj_work->disp_flag |= parent_obj->disp_flag & OBD_DISP_HFLIP;
}


// ==========================================================================
// 軌跡エフェクト
// ==========================================================================
// ==========================================================================
// dmLogoSegaCreateTrail
/*!
 *	プレイヤー 軌跡エフェクト生成
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
#if 0
static float startColor_r = 0.0f;
static float startColor_g = 0.0f;
static float startColor_b = 1.0f;
static float startColor_a = 1.0f;
static float endColor_r = 0.0f;
static float endColor_g = 0.0f;
static float endColor_b = 1.0f;
static float endColor_a = 0.0f;
static float startSize = 88.0f;
static float endSize = 88.0f;
static float life = 35.0f;
static float vanish_time = 10.0f;
#endif
void dmLogoSegaCreateTrail(OBS_OBJECT_WORK *obj_work)
{
	// 軌跡エフェクト作成
	AMS_TRAIL_PARAM param;
	NNS_TEXLIST		*texlist = dm_logo_sega_aos_tex[DMD_LOGO_SEGA_AOSTEX_LOCAL_ACT].texlist;

	memset(&param, 0, sizeof(AMS_TRAIL_PARAM));
#if 1
	param.startColor.r = 0.0f;
	param.startColor.g = 0.0f;
	param.startColor.b = 1.0f;
	param.startColor.a = 1.0f;
	param.endColor.r = 0.0f;
	param.endColor.g = 0.0f;
	param.endColor.b = 1.0f;
	param.endColor.a = 0.0f;
	param.startSize		= 88.0f;
	param.endSize		= 88.0f;
	param.life			= 35.0f;
	param.vanish_time	= 10.0f;
#else
	param.startColor.r = startColor_r;
	param.startColor.g = startColor_g;
	param.startColor.b = startColor_b;
	param.startColor.a = startColor_a;
	param.endColor.r = endColor_r;
	param.endColor.g = endColor_g;
	param.endColor.b = endColor_b;
	param.endColor.a = endColor_a;
	param.startSize		= startSize;
	param.endSize		= endSize;
	param.life			= life;
	param.vanish_time	= vanish_time;
#endif
	param.trail_pos = (AMS_VECTOR3I*)((void*)&obj_work->pos);
	param.partsNum = AMD_TRAIL_PARTSMAX-1;
	param.zBias = -16*FX32_ONE;
	param.texId = texlist->nTex-1;
	param.blendType = AMDRAWE_BLENDTYPE_NORMAL;//AMDRAWE_BLENDTYPE_ADD;
	param.zTest = 1;
	amTrailMakeEffect(&param, AMTRE_HANDLE_ACCELL, AMTRE_FLAG_FXPOS);
}


// ==========================================================================
// カメラ処理
// ==========================================================================
// ==========================================================================
// dmLogoSegaCamera
/*!
 *	カメラ処理
 *
 *	@param	camera	[in]	カメラワーク
 */
// ==========================================================================
void dmLogoSegaCamera(OBS_CAMERA *camera)
{
	camera->disp_pos.x = 0.f;
	camera->disp_pos.y = 0.f;
	camera->disp_pos.z = 200.f;

	camera->target_pos.x = 0.f;
	camera->target_pos.y = 0.f;
	camera->target_pos.z = 0.f;
}


// ==========================================================================
// ファイルロード時後処理
// ==========================================================================
// ==========================================================================
// dmLogoSegaLoadPostFunc
/*!
 *	データロード後処理
 *
 *	@param	context	[in]	コンテキスト
 */
// ==========================================================================
void dmLogoSegaLoadPostFunc(DMS_LOGO_COM_LOAD_CONTEXT *context)
{
	dm_logo_sega_data[context->no] = context->fs_req->buf;
	context->fs_req->buf = NULL;

	// コンバート
	amBindConvertAll((u8*)dm_logo_sega_data[context->no]);

#if _WII
	if (context->no == DMD_LOGO_SEGA_DATA_PLY_MDL_AMB) {	
	//◆暫定処理
		// 無理やり NND_NODETYPE_RESET_SCALING_X _Y _Z を落とす
		AMS_AMB_HEADER	*amb;
		void			*data;
		NNS_OBJECT		*object;
		NNS_TEXFILELIST	*texfilelist;
		NNS_NODE		*node;
		s32				node_num;
		s32				i, amb_cnt;

		amb = (AMS_AMB_HEADER*)dm_logo_sega_data[context->no];
		for (amb_cnt = 0; amb_cnt < amb->file_num; amb_cnt++) {
			data = amBindGet(amb, amb_cnt);

			amObjectSetup(&object, &texfilelist, data);

			node	= object->pNodeList;
			node_num= object->nNode;
			for (i = 0; i < node_num; i++, node++) {
				node->fType &= ~(NND_NODETYPE_RESET_SCALING_X |
								NND_NODETYPE_RESET_SCALING_Y |
								NND_NODETYPE_RESET_SCALING_Z);
			}
		}
	}
#endif
}


// ==========================================================================
// データビルド
// ==========================================================================
// ==========================================================================
// dmLogoSegaDataBuildMain
/*!
 *	データビルドメイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSegaDataBuildMain(MTS_TASK_TCB *tcb)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;
	BOOL		b_sts = TRUE;

	// ビルド待機
	if (GmGameDBuildCheckBuildModel() == FALSE) {
		b_sts = FALSE;
	}

	// テクスチャビルド
	aos_tex = dm_logo_sega_aos_tex;
	for (i = 0; i < DMD_LOGO_SEGA_AOSTEX_MAX; i++, aos_tex++) {
		if (!AoTexIsLoaded(aos_tex)) {
			b_sts = FALSE;
		}
	}

	// エフェクト用モデルビルド
	for (i = 0; i < DMD_LOGO_SEGA_EFCT_MODEL_MAX; i++) {
		if (dm_logo_sega_efct_mdl_state[i] != -1) {
			if (!amDrawIsRegistComplete(dm_logo_sega_efct_mdl_state[i])) {
				b_sts = FALSE;
				dm_logo_sega_efct_mdl_state[i] = -1;
			}
		}
	}

#if 0
	// サウンドビルド
	if (GsSoundBuildSeUpdate(&dm_sound_data_work_se) == FALSE) {
		b_sts = FALSE;
	}
#endif

	// ビルド待機
	if (!b_sts) {
		return;
	}

	// ビルド終了
	mtTaskClearTcb(tcb);
	dm_logo_sega_build_state = TRUE;
}

// ==========================================================================
// dmLogoSegaDataBuildDest
/*!
 *	データビルドデストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSegaDataBuildDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dm_logo_sega_build_tcb = NULL;
}

// ==========================================================================
// データフラッシュ
// ==========================================================================
// ==========================================================================
// dmLogoSegaDataFlushMain
/*!
 *	データフラッシュメイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSegaDataFlushMain(MTS_TASK_TCB *tcb)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;
	BOOL		b_sts = TRUE;

	// モデル
	if (GmGameDBuildCheckFlushModel() == FALSE) {
		b_sts = FALSE;
	}

	// テクスチャフラッシュ
	aos_tex = dm_logo_sega_aos_tex;
	for (i = 0; i < DMD_LOGO_SEGA_AOSTEX_MAX; i++, aos_tex++) {
		if (!AoTexIsReleased(aos_tex)) {
			b_sts = FALSE;
		}
	}

	// エフェクト用モデルフラッシュ
	for (i = 0; i < DMD_LOGO_SEGA_EFCT_MODEL_MAX; i++) {
		if (dm_logo_sega_efct_mdl_state[i] != -1) {
			if (ObjAction3dESModelReleaseDworkCheck(&dm_logo_sega_efct_mdl_data_work[i], dm_logo_sega_efct_mdl_state[i])) {
				dm_logo_sega_efct_mdl_state[i] = -1;
			}
			else {
				b_sts = FALSE;
			}
		}
	}

	// サウンドフラッシュ
	// 待機なし

	// フラッシュ待機
	if (!b_sts) {
		return;
	}

	amMemFree(dm_logo_sega_aos_tex);
	dm_logo_sega_aos_tex = NULL;

	dm_logo_sega_obj_3d_list = NULL;

	// フラッシュ終了
	mtTaskClearTcb(tcb);
	dm_logo_sega_build_state = FALSE;
}

// ==========================================================================
// dmLogoSegaDataFlushDest
/*!
 *	データフラッシュデストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSegaDataFlushDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dm_logo_sega_flush_tcb = NULL;
}


// ==========================================================================
// データロード待機
// ==========================================================================
// ==========================================================================
// dmLogoSegaLoadWait
/*!
 *	データロード待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSegaLoadWait(MTS_TASK_TCB *tcb)
{
	if (DmLogoSegaLoadCheck()) {
		// データビルド開始
		DmLogoSegaBuild();

		mtTaskChangeTcbProcedure(tcb, dmLogoSegaBuildWait);
	}
}

// ==========================================================================
// dmLogoSegaBuildWait
/*!
 *	データビルド待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSegaBuildWait(MTS_TASK_TCB *tcb)
{
	if (DmLogoSegaBuildCheck()) {

		// データロード終了
		mtTaskClearTcb(tcb);

		// 演出開始
		dmLogoSegaStart();
	}
}


// ==========================================================================
// アクション生成・破棄
// ==========================================================================
// ==========================================================================
// dmLogoSegaActionCreate
/*!
 *	アクション生成
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSegaActionCreate(DMS_LOGO_SEGA_WORK *logo_work)
{
	u32		i;
	void	*ama;

	// アクション構築
	ama = amBindGet((AMS_AMB_HEADER*)dm_logo_sega_data[DMD_LOGO_SEGA_DATA_LOCAL_AMA_SET_AMB],
				IDB_D_LOGO_SEGA_JP_D_LOGO_SEGA_AMA);

	for (i = 0; i < DMD_LOGO_SEGA_ACT_MAX; i++) {
		AoActSetTexture(AoTexGetTexList(dm_logo_sega_aos_tex + dm_logo_sega_tex_id_tbl[i]));
		logo_work->act[i] = AoActCreate(ama, i);
	}
}

// ==========================================================================
// dmLogoSegaActionDelete
/*!
 *	アクション破棄
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSegaActionDelete(DMS_LOGO_SEGA_WORK *logo_work)
{
	s32		i;

	for (i = 0; i < DMD_LOGO_SEGA_ACT_MAX; i++) {
		AoActDelete(logo_work->act[i]);
	}
/*	
#ifndef SONIC4_TRIAL
	mppUtil::regAndDontShowBtn();
#endif	*/
}

// ==========================================================================
// DmLogoSegaStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void DmLogoSegaStaticVarInit(void)
{
	dm_logo_sega_load_tcb = NULL;		//!< データロードTCB
	dm_logo_sega_build_tcb = NULL;		//!< データビルドTCB
	dm_logo_sega_flush_tcb = NULL;		//!< データフラッシュTCB
	//dm_logo_sega_release_tcb = NULL;	//!< データリリースTCB	// 一旦コメントアウト
	
	dm_logo_sega_aos_tex = NULL;		//!< テクスチャ
	memset(dm_logo_sega_efct_mdl_data_work, 0,
		   sizeof(dm_logo_sega_efct_mdl_data_work));	//!< エフェクト用モデル格納データワーク
	memset(dm_logo_sega_efct_mdl_state, -1,
		   sizeof(dm_logo_sega_efct_mdl_state));		//!< ロードステート
	dm_logo_sega_build_state = FALSE;					//!< ビルド状況
	
	memset(&dm_sound_data_work_se, 0, sizeof(dm_sound_data_work_se));	//!< サウンドデータワーク
	/// データ
	memset(dm_logo_sega_data, 0, sizeof(dm_logo_sega_data));
	
	/// 描画オブジェクト
	dm_logo_sega_obj_3d_list = NULL;
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
