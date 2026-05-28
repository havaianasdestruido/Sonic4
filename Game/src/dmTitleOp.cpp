// ==========================================================================
/*!
  @file dmTitleOp.cpp
  @brief タイトル オープニング演出

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: dmTitleOp.cpp 20 2011-04-22 12:46:46Z thamada $
  $Date:: 2011-04-22 21:46:46 +0900#$
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
#include "izFade.h"
#include "objObject.h"
#include "gmMain.h"
#include "gmTask.h"
#include "gmGameDBuild.h"
#include "gmMapFar.h"

#include "dmLogoCom.h"

#include "dmTitleOp.h"

// データヘッダ
// Zone1
#if _IPHONE
#include "iPhone/model/mapfar_zone1_mdl.hmb"
#else
#include "common/model/mapfar_zone1_mdl.hmb"
#endif
#include "common/model/mapfar_zone1_render_mdl.hmb"
#include "common/model/mapfar_zone1_mat.hmb"
#include "common/model/zone1_mapfar.hmb"
#include "common/ace/D_TITLE_OP.HMA"

//----- Definitions ---------------------------------------------------------
#if defined (MTD_DEBUG)
#define DMD_TITLEOP_DEBUG_TEST				(0)
#define DMD_TITLEOP_DEBUG_TEST_FOG			(0)
#define DMD_TITLEOP_DEBUG_TEST_CAMERA		(0)
#define DMD_TITLEOP_DEBUG_TEST_EXIT			(0)
#define DMD_TITLEOP_DEBUG_TEST_ROCK_EDIT	(0)
#else
#define DMD_TITLEOP_DEBUG_TEST				(0)
#define DMD_TITLEOP_DEBUG_TEST_FOG			(0)
#define DMD_TITLEOP_DEBUG_TEST_CAMERA		(0)
#define DMD_TITLEOP_DEBUG_TEST_EXIT			(0)
#define DMD_TITLEOP_DEBUG_TEST_ROCK_EDIT	(0)
#endif


/* タスク設定 */
#define DMD_TITLEOP_TASK_PRIO_DATA_LOAD		(0x1000)		//!< ロードタスクプライオリティ
#define DMD_TITLEOP_TASK_GROUP_DATA_LOAD	(0)				//!< ロードタスクグループ
#define DMD_TITLEOP_TASK_PRIO_DATA_BUILD	(0x1000)		//!< ビルドタスクプライオリティ
#define DMD_TITLEOP_TASK_GROUP_DATA_BUILD	(0)				//!< ビルドタスクグループ
#define DMD_TITLEOP_TASK_PRIO_DATA_FLUSH	(0x1000)		//!< フラッシュタスクプライオリティ
#define DMD_TITLEOP_TASK_GROUP_DATA_FLUSH	(0)				//!< フラッシュタスクグループ
#define DMD_TITLEOP_TASK_PRIO_DATA_RELEASE	(0x1000)		//!< リリースタスクプライオリティ
#define DMD_TITLEOP_TASK_GROUP_DATA_RELEASE	(0)				//!< リリースタスクグループ

#define DMD_TITLEOP_TASK_PRIO_MGR			(0x3000)		//!< マネージャタスクプライオリティ
#define DMD_TITLEOP_TASK_GROUP_MGR			(0)				//!< マネージャタスクグループ
#define DMD_TITLEOP_TASK_PRIO_CAMERA		(0x3000)		//!< カメラタスクプライオリティ
#define DMD_TITLEOP_TASK_GROUP_CAMERA		(0)				//!< カメラタスクグループ
#define DMD_TITLEOP_TASK_PRIO_FAR_SKY_T		(0x6000)		//!< 遠景上空オブジェクトタスクプライオリティ
#define DMD_TITLEOP_TASK_GROUP_FAR_SKY_T	(0)				//!< 遠景上空オブジェクトタスクグループ
#define DMD_TITLEOP_TASK_PRIO_FAR_SKY_B		(0x4000)		//!< 遠景下空オブジェクトタスクプライオリティ
#define DMD_TITLEOP_TASK_GROUP_FAR_SKY_B	(0)				//!< 遠景下空オブジェクトタスクグループ
#define DMD_TITLEOP_TASK_PRIO_FAR_ROCK_T	(0x6100)		//!< 遠景上岩オブジェクトタスクプライオリティ
#define DMD_TITLEOP_TASK_GROUP_FAR_ROCK_T	(0)				//!< 遠景上岩オブジェクトタスクグループ
#define DMD_TITLEOP_TASK_PRIO_FAR_ROCK_B	(0x4100)		//!< 遠景下岩オブジェクトタスクプライオリティ
#define DMD_TITLEOP_TASK_GROUP_FAR_ROCK_B	(0)				//!< 遠景下岩オブジェクトタスクグループ
#define DMD_TITLEOP_TASK_PRIO_FAR_SEA		(0x5000)		//!< 遠景上海オブジェクトタスクプライオリティ
#define DMD_TITLEOP_TASK_GROUP_FAR_SEA		(0)				//!< 遠景上海オブジェクトタスクグループ

#define	DMD_TITLEOP_TASK_PAUSELEVEL_DEF		(0)				//!< 標準ポーズレベル
#define DMD_TITLEOP_OBJ_OBJPAUSELEVEL_DEF	(0)			//!< オブジェクトポーズレベルデフォルト

/* オブジェクトタイプ */
#define DMD_TITLEOP_OBJTYPE_MAPFAR			(1)			//!< 遠景

/* カメラ設定 */
#define DMD_TITLEOP_CAMERA_ID				(0)

/* 描画ステート */
#define DMD_TITLEOP_DRAW_CMD_STATE_PRE_MAPFAR		(0)
#define DMD_TITLEOP_DRAW_CMD_STATE_MAPFAR_B			(1)
#define DMD_TITLEOP_DRAW_CMD_STATE_MAPFAR_SEA		(2)
#define DMD_TITLEOP_DRAW_CMD_STATE_MAPFAR_T			(3)
#define DMD_TITLEOP_DRAW_CMD_STATE_POST_MAPFAR		(4)
#define DMD_TITLEOP_DRAW_CMD_STATE_3DNN				(5)



#define DMD_TITLEOP_ROCK_TYPE_NUM			(3)					//!< 岩のタイプ数
#define DMD_TITLEOP_ROCK_SETTING_NUM		(6)					//!< 岩1タイプあたりのセッティング情報数
#define DMD_TITLEOP_ROCK_DISP_CLIP_DIST		(0x120*FX32_ONE)	//!< 岩表示クリッピング距離

/* 画面スクロール */
#if DMD_TITLEOP_DEBUG_TEST_ROCK_EDIT
fx32 dm_titleop_test_edit_map_scrl_spd = -0x080;
#define DMD_TITLEOP_MAP_SCRL_SPD			(dm_titleop_test_edit_map_scrl_spd)
#else
#define DMD_TITLEOP_MAP_SCRL_SPD			(-0x0080)
#endif

#define DMD_TITLEOP_MAP_SCRL_LOOP_DIST		(FX32_ONE * 0x300)	//!< ループ距離



/* アクション演出設定 */
#define DMD_TITLEOP_ACT_LOGO_START_FRANE	(30)		//!< ロゴ表示開始フレーム
#define DMD_TITLEOP_ACT_RIGHT_START_FRANE	(45 + DMD_TITLEOP_ACT_LOGO_START_FRANE)	//!< 権利表記開始フレーム
#define DMD_TITLEOP_ACT_ALL_SET_END_FRAME	(75 + DMD_TITLEOP_ACT_LOGO_START_FRANE)	//!< 全アクションセット終了フレーム

#define DMD_TITLEOP_ACT_FINGER_WAVE_S_FRAME	(60)	//!< 指振り開始フレーム
#define DMD_TITLEOP_ACT_FINGER_WAVE_E_FRAME	(84)	//!< 指振り終了フレーム



/// 読み込みデータ
enum {
	DMD_TITLEOP_DATA_FAR_AMB	= 0,		//!< 遠景データセット

	DMD_TITLEOP_DATA_COM_AMA_SET_AMB,		//!< タイトルロゴ関連
//	DMD_TITLEOP_DATA_LOCAL_AMA_SET_AMB,		//!< タイトルロゴ関連 ローカライズ対象データ

	DMD_TITLEOP_DATA_MAX
};

/// 遠景データ
enum {
	DMD_TITLEOP_MAPFAR_DATA_MDL	= 0,		//!< 通常モデル
	DMD_TITLEOP_MAPFAR_DATA_TEX,			//!< テクスチャ
	DMD_TITLEOP_MAPFAR_DATA_MAT,			//!< マテリアルモーション
	DMD_TITLEOP_MAPFAR_DATA_MDL_WATER,		//!< 水モデル

	DMD_TITLEOP_MAPFAR_DATA_MAX
};

/// AMAデータセットデータ並び
enum {
	DMD_TITLEOP_AMA_DATA_SET_AMA	= 0,
	DMD_TITLEOP_AMA_DATA_SET_TEX_AMB,

	DMD_TITLEOP_AMA_DATA_SET_MAX
};

/// オブジェクト
enum {
#if _IPHONE
	DMD_TITLEOP_OBJWORK_SKY_T = 0,		//!< 空上側
	DMD_TITLEOP_OBJWORK_ROCK_A_T,		//!< 岩上側 A
	DMD_TITLEOP_OBJWORK_ROCK_B_T,		//!< 岩上側 B
	DMD_TITLEOP_OBJWORK_ROCK_C_T,		//!< 岩上側 C
	DMD_TITLEOP_OBJWORK_SEW,			//!< 海
#else
	DMD_TITLEOP_OBJWORK_SKY_B	= 0,	//!< 空下側
	DMD_TITLEOP_OBJWORK_ROCK_A_B,		//!< 岩下側 A
	DMD_TITLEOP_OBJWORK_ROCK_B_B,		//!< 岩下側 B
	DMD_TITLEOP_OBJWORK_ROCK_C_B,		//!< 岩下側 C
	DMD_TITLEOP_OBJWORK_SKY_T,			//!< 空上側
	DMD_TITLEOP_OBJWORK_ROCK_A_T,		//!< 岩上側 A
	DMD_TITLEOP_OBJWORK_ROCK_B_T,		//!< 岩上側 B
	DMD_TITLEOP_OBJWORK_ROCK_C_T,		//!< 岩上側 C
	DMD_TITLEOP_OBJWORK_SEW,			//!< 海
#endif

	DMD_TITLEOP_OBJWORK_MAX
};

/// AOSテクスチャタイプ
enum {
	DMD_TITLEOP_AOSTEX_COM	= 0,	//!< 共通データ
//	DMD_TITLEOP_AOSTEX_LOCAL,		//!< ローカライズデータ

	DMD_TITLEOP_AOSTEX_MAX
};

/// AOSアクション
enum {
	DMD_TITLEOP_AOS_ACT_LOGO_1	= 0,		//!< ロゴ1(本体部)
	DMD_TITLEOP_AOS_ACT_FINGER,				//!< ソニック指
	DMD_TITLEOP_AOS_ACT_LOGO_2,				//!< ロゴ2(エピソード)
	DMD_TITLEOP_AOS_ACT_LOGO_3,				//!< ロゴ3(SONIC 4)
	DMD_TITLEOP_AOS_ACT_RIGHT,				//!< 権利表記
#if SONIC4_TRIAL
	DMD_TITLEOP_AOS_ACT_FREE,				//!< FREEロゴ
	DMD_TITLEOP_AOS_ACT_FREE_RED,			//!< FREEロゴ(赤色)
#endif //SONIC4_TRIAL

	DMD_TITLEOP_AOS_ACT_MAX
};


/// 岩配置情報
typedef struct tag_DMS_TITLEOP_ROCK_SETTING {
	VecFx32		pos;
	VecFx32		scale;
} DMS_TITLEOP_ROCK_SETTING;


/// タイトルオープニングオブジェクト3Dワーク
typedef struct tag_DMS_TITLEOP_OBJ_3DNN_WORK {
	OBS_OBJECT_WORK			obj_work;
	OBS_ACTION3D_NN_WORK	obj_3d;

	// 岩用設定データ
	const DMS_TITLEOP_ROCK_SETTING	*rock_setting;
	const s32						*rock_setting_num;

	// 空用設定データ
	float						sky_rot;
} DMS_TITLEOP_OBJ_3DNN_WORK;

/// マネージャーワーク
typedef struct tag_DMS_TITLEOP_WORK {
	s32					frame;
	u32					flag;

	OBS_OBJECT_WORK		*obj_work[DMD_TITLEOP_OBJWORK_MAX];

	AOS_ACTION			*act[DMD_TITLEOP_AOS_ACT_MAX];
	float				finger_frame;
} DMS_TITLEOP_MGR_WORK;

// DMS_TITLEOP_MGR_WORK : flag
#define DMD_TITLEOP_FLAG_ACT_DISP_LOGO_1		(0x00000001)	//!< 表示設定 以下連番の事
#define DMD_TITLEOP_FLAG_ACT_DISP_FINGER		(0x00000002)
#define DMD_TITLEOP_FLAG_ACT_DISP_LOGO_2		(0x00000004)
#define DMD_TITLEOP_FLAG_ACT_DISP_LOGO_3		(0x00000008)
#define DMD_TITLEOP_FLAG_ACT_DISP_RIGHT			(0x00000010)
#if SONIC4_TRIAL
#define DMD_TITLEOP_FLAG_ACT_DISP_FREE			(1 << 5)
#define DMD_TITLEOP_FLAG_ACT_DISP_FREE_RED		(1 << 6)
#endif //SONIC4_TRIAL

#define DMD_TITLEOP_FLAG_ACT_USER_NODISP_LOGO_1	(0x00000100)	//!< 表示設定 ユーザー設定
#define DMD_TITLEOP_FLAG_ACT_USER_NODISP_FINGER	(0x00000200)
#define DMD_TITLEOP_FLAG_ACT_USER_NODISP_LOGO_2	(0x00000400)
#define DMD_TITLEOP_FLAG_ACT_USER_NODISP_LOGO_3	(0x00000800)
#define DMD_TITLEOP_FLAG_ACT_USER_NODISP_RIGHT	(0x00001000)

#define DMD_TITLEOP_FLAG_SCRL				(0x10000000)	//!< 背景(岩)スクロール
#define DMD_TITLEOP_FLAG_EXIT				(0x20000000)	//!< 終了処理開始
#define DMD_TITLEOP_FLAG_ALL_ACT_END		(0x40000000)	//!< 全演出終了
#define DMD_TITLEOP_FLAG_2DACT_READY		(0x80000000)	//!< 2Dアクション準備済み



//----- Macros --------------------------------------------------------------
// ==========================================================================
// DMM_TITLEOP_CREATE_3D_OBJ
/*!
 *	3Dオブジェクト生成
 *
 *	@return	オブジェクトワーク
 */
// ==========================================================================
#if defined (MTD_DEBUG)
#define DMM_TITLEOP_CREATE_3D_OBJ(prio, group, work_size, name)		dmTitleOpCreate3DObj(prio, group, work_size, name)
#else
#define DMM_TITLEOP_CREATE_3D_OBJ(prio, group, work_size, name)		dmTitleOpCreate3DObj(prio, group, work_size)
#endif

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void dmTitleOpLoadPostFuncMapFar(DMS_LOGO_COM_LOAD_CONTEXT *context);
static void dmTitleOpLoadPostFuncTitleLogo(DMS_LOGO_COM_LOAD_CONTEXT *context);

static void dmTitleOpMgrInit(void);
static void dmTitleOpDataBuildMain(MTS_TASK_TCB *tcb);
static void dmTitleOpDataBuildDest(MTS_TASK_TCB *tcb);
static void dmTitleOpDataFlushMain(MTS_TASK_TCB *tcb);
static void dmTitleOpDataFlushDest(MTS_TASK_TCB *tcb);

static void dmTitleOpMgrDest(MTS_TASK_TCB *tcb);
static void dmTitleOpMgrMain(MTS_TASK_TCB *tcb);

static void dmTitleOpCreateObjFarSky(void);
static void dmTitleOpFarSkyFunc(OBS_OBJECT_WORK *obj_work);
static void dmTitleOpCreateObjFarRock(u32 type);
static void dmTitleOpCreateObjFarSea(void);

#if defined (MTD_DEBUG)
static DMS_TITLEOP_OBJ_3DNN_WORK* dmTitleOpCreate3DObj(u16 prio, u8 group, u32 work_size, const char *name);
#else
static DMS_TITLEOP_OBJ_3DNN_WORK* dmTitleOpCreate3DObj(u16 prio, u8 group, u32 work_size);
#endif

static void dmTitleOpCreateAction(DMS_TITLEOP_MGR_WORK *top_mgr_work);
static void dmTitleOpDeleteAction(DMS_TITLEOP_MGR_WORK *top_mgr_work);

static void dmTitleOpEndStart(MTS_TASK_TCB *tcb);
static void dmTitleOpPreEnd(DMS_TITLEOP_MGR_WORK *top_mgr_work);
static void dmTitleOpPreEndWait(MTS_TASK_TCB *tcb);
static void dmTitleOpEnd(DMS_TITLEOP_MGR_WORK *top_mgr_work);
static void dmTitleOpEndWait(MTS_TASK_TCB *tcb);

static void dmTitleOpPreDrawDT(void *data);
static void dmTitleOpObjDraw(OBS_OBJECT_WORK *obj_work);
static void dmTitleOpObjRockDraw(OBS_OBJECT_WORK *obj_work);
#if _WII
static NNE_BOOL dmTitleOpFallMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param);
#endif
static void dmTitleOpFallShaderPreRenderUserFunc(void *data);
static void dmTitleOpDrawFallShaderPreSettingUserFunc(void *data);

static void dmTitleOpCamera(OBS_CAMERA *camera);

#if DMD_TITLEOP_DEBUG_TEST
static void dmTitleOpTestTaskDraw(AMS_TCB* tcb);
#endif
//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static MTS_TASK_TCB *dm_titleop_load_tcb = NULL;	//!< データロードTCB
static MTS_TASK_TCB *dm_titleop_build_tcb = NULL;	//!< データビルドTCB
static MTS_TASK_TCB *dm_titleop_flush_tcb = NULL;	//!< データフラッシュTCB
//static MTS_TASK_TCB *dm_titleop_release_tcb = NULL;	//!< データリリースTCB

static MTS_TASK_TCB *dm_titleop_mgr_tcb = NULL;	//!< メイン処理TCB

static BOOL dm_titleop_build_state = FALSE;		//!< ビルドステータス

/// 共通データファイル名リスト
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_titleop_com_fileinfo_list[] = {
	// Zone1遠景
	{GSS_BASE_PATH "G_ZONE1/MAPFAR/ZONE1_MAPFAR.AMB", dmTitleOpLoadPostFuncMapFar},
	// 以下その他
	{GSS_BASE_PATH "DEMO/TITLE/D_TITLEOP.AMB", dmTitleOpLoadPostFuncTitleLogo},
};

/// データファイル数
static const s32 dm_titleop_com_file_num = sizeof(dm_titleop_com_fileinfo_list) / sizeof(DMS_LOGO_COM_LOAD_FILE_INFO);

#if 0
// GsEnvGetLanguage() GSE_LANGUAGE
//	GSD_LANGUAGE_JP		= 0,				//!< 日本語
//	GSD_LANGUAGE_US,						//!< 英語
//	GSD_LANGUAGE_FR,						//!< フランス語
//	GSD_LANGUAGE_IT,						//!< イタリア語
//	GSD_LANGUAGE_GE,						//!< ドイツ語
//	GSD_LANGUAGE_SP,						//!< スペイン語
//
//	GSD_LANGUAGE_NUM,						//!< 言語数
//	GSD_LANGUAGE_DEF = GSD_LANGUAGE_US,		//!< デフォルト言語

/// ローカライズ関連データファイル名リスト 日本
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_titleop_localize_fileinfo_list_jp[] = {
	{GSS_BASE_PATH "DEMO/TITLE/D_TITLE_OP_JP.AMB", dmTitleOpLoadPostFuncTitleLogo},
};
/// ローカライズ関連データファイル名リスト 英語
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_titleop_localize_fileinfo_list_us[] = {
	{GSS_BASE_PATH "DEMO/TITLE/D_TITLE_OP_US.AMB", dmTitleOpLoadPostFuncTitleLogo},
};
/// ローカライズ関連データファイル名リスト フランス
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_titleop_localize_fileinfo_list_fr[] = {
	{GSS_BASE_PATH "DEMO/TITLE/D_TITLE_OP_FR.AMB", dmTitleOpLoadPostFuncTitleLogo},
};
/// ローカライズ関連データファイル名リスト イタリア
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_titleop_localize_fileinfo_list_it[] = {
	{GSS_BASE_PATH "DEMO/TITLE/D_TITLE_OP_IT.AMB", dmTitleOpLoadPostFuncTitleLogo},
};
/// ローカライズ関連データファイル名リスト ドイツ
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_titleop_localize_fileinfo_list_ge[] = {
	{GSS_BASE_PATH "DEMO/TITLE/D_TITLE_OP_GE.AMB", dmTitleOpLoadPostFuncTitleLogo},
};
/// ローカライズ関連データファイル名リスト スペイン
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_titleop_localize_fileinfo_list_sp[] = {
	{GSS_BASE_PATH "DEMO/TITLE/D_TITLE_OP_SP.AMB", dmTitleOpLoadPostFuncTitleLogo},
};
/// ローカライズ関連データファイル名リストテーブル
static const DMS_LOGO_COM_LOAD_FILE_INFO *dm_titleop_localize_fileinfo_list_tbl[] = {
	dm_titleop_localize_fileinfo_list_jp,
	dm_titleop_localize_fileinfo_list_us,
	dm_titleop_localize_fileinfo_list_fr,
	dm_titleop_localize_fileinfo_list_it,
	dm_titleop_localize_fileinfo_list_ge,
	dm_titleop_localize_fileinfo_list_sp,
};

/// ローカライズ関連データファイル数
static const s32 dm_titleop_localize_file_num = sizeof(dm_titleop_localize_fileinfo_list_jp) / sizeof(DMS_LOGO_COM_LOAD_FILE_INFO);
#endif

/// クリアカラー設定
//static NNS_RGBA_U8 dm_titleop_clear_color = {0x44, 0x66, 0xff, 0xff};
static const NNS_RGBA_U8 dm_titleop_clear_color = {0xff, 0xff, 0xff, 0xff};


/// 岩用設定データ
static const DMS_TITLEOP_ROCK_SETTING dm_titleop_rock_setting[DMD_TITLEOP_ROCK_TYPE_NUM][DMD_TITLEOP_ROCK_SETTING_NUM] = {
	// 岩A
	{	// pos, scale
		{{0x0003C800, 0x00000000, 0x0004F000}, {0x2200, 0x2200, 0x2200}},
		{{0xFFFC6800, 0x00000000, 0xFFFDD000}, {0x1000, 0x1000, 0x1000}},
		{{0x001BD000, 0x00000000, 0xFFF88000}, {0x5280, 0x5280, 0x5280}},
		{{0x0007A800, 0x00000000, 0xFFF93000}, {0x1CB0, 0x1CB0, 0x1CB0}},
		{{0xFFF8C000, 0x00000000, 0xFFF8E000}, {0x4530, 0x4530, 0x4530}},
		{{0x00000000, 0x00000000, 0x00000000}, {0x1000, 0x1000, 0x1000}},
	},
	// 岩B
	{
		{{0xFFFCF800, 0x00000000, 0x0004A800}, {0x1270, 0x1270, 0x1270}},
		{{0x00079800, 0x00000000, 0x0003D000}, {0x2BD0, 0x2BD0, 0x2BD0}},
		{{0x00060000, 0x00000000, 0xFFF25800}, {0x25F3, 0x25F3, 0x25F3}},
		{{0xFFF4D000, 0x00000000, 0x00020800}, {0x2230, 0x2230, 0x2230}},
		{{0x00041000, 0x00000000, 0x00063000}, {0x0FC0, 0x0FC0, 0x0FC0}},
		{{0x00000000, 0x00000000, 0x00000000}, {0x1000, 0x1000, 0x1000}},	// 空き
	},
	// 岩C
	{
		{{0x00036800, 0x00000000, 0x00018000}, {0x11A0, 0x11A0, 0x11A0}},
		{{0xFFE73000, 0x00000000, 0xFFF56800}, {0x4460, 0x4460, 0x4460}},
		{{0x000E1800, 0x00000000, 0xFFFDC800}, {0x4AE0, 0x4AE0, 0x4AE0}},
		{{0xFFEB0000, 0x00000000, 0x00056800}, {0x20C0, 0x20C0, 0x20C0}},
		{{0xFFF6B800, 0x00000000, 0x00019000}, {0x1530, 0x1530, 0x1530}},
		{{0x0009E800, 0x00000000, 0x00025000}, {0x10D0, 0x10D0, 0x10D0}},
	},
};

/// 岩用設定データ使用数
static const s32 dm_titleop_rock_setting_num[DMD_TITLEOP_ROCK_TYPE_NUM] = {6, 5, 6};



// データ
void *dm_titleop_data[DMD_TITLEOP_DATA_MAX] = {NULL};
void *dm_titleop_mapfar_data[DMD_TITLEOP_MAPFAR_DATA_MAX] = {NULL};

// テクスチャ
static AOS_TEXTURE *dm_titleop_aos_tex = NULL;	//!< テクスチャ

// 描画オブジェクト
static OBS_ACTION3D_NN_WORK *dm_titleop_obj_3d_list = NULL;
static OBS_ACTION3D_NN_WORK *dm_titleop_water_obj_3d_list = NULL;

// 画面スクロール
static fx32 dm_titleop_scrl_x_ofst;




/* 以下デバッグ */
#if defined (MTD_DEBUG)

#if DMD_TITLEOP_DEBUG_TEST_CAMERA
static NNS_VECTOR	test_disp_pos = {0, 5, 160};
#endif

#if DMD_TITLEOP_DEBUG_TEST_FOG
static float fog_near = 1.f;
static float fog_far = 500.f;
#endif

#if DMD_TITLEOP_DEBUG_TEST_ROCK_EDIT
enum {
	DMD_TITLEOP_DEBUG_ROCK_EDIT_MODE_EDIT	= 0,	//!< 編集
	DMD_TITLEOP_DEBUG_ROCK_EDIT_MODE_CAMERA,		//!< カメラ移動
	DMD_TITLEOP_DEBUG_ROCK_EDIT_MODE_ACT,			//!< 演出チェック
	
	DMD_TITLEOP_DEBUG_ROCK_EDIT_MODE_MAX
};
enum {
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_NUM = 0,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE1,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE2,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE3,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE4,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE5,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE6,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_NUM,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_STATE1,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_STATE2,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_STATE3,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_STATE4,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_STATE5,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_STATE6,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_NUM,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_STATE1,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_STATE2,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_STATE3,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_STATE4,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_STATE5,
	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_STATE6,

	DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_MAX
};
static s32 dm_titleop_debug_rock_edit_mode = 0;
static s32 dm_titleop_debug_test_rock_edit_cursol[DMD_TITLEOP_DEBUG_ROCK_EDIT_MODE_MAX] = {0};
static s32 dm_titleop_debug_test_rock_edit_item_sel = 0;
static BOOL dm_titleop_debug_test_rock_edit_disp = TRUE;	// デバッグ表示
static float dm_titleop_debug_test_cam_ofst_y = 0.f;

static void dmTitleOpDebugTestRockEdit(DMS_TITLEOP_MGR_WORK *top_mgr_work);
#endif // #if DMD_TITLEOP_DEBUG_TEST_ROCK_EDIT
#endif // #if defined (MTD_DEBUG)

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// データロード
// ==========================================================================
// ==========================================================================
// DmTitleOpLoad
/*!
 *	オープニングタイトルデモデータロード
 *
 */
// ==========================================================================
void DmTitleOpLoad(void)
{
//	GSE_LANGUAGE	language = GsEnvGetLanguage();

	// ロード処理生成
	DmLogoComLoadFileCreate(&dm_titleop_load_tcb);

	// 共通データ
	DmLogoComLoadFileReg(dm_titleop_load_tcb, &dm_titleop_com_fileinfo_list[0], dm_titleop_com_file_num);

//	// ローカライズ関連データ
//	DmLogoComLoadFileReg(dm_titleop_load_tcb, dm_titleop_localize_fileinfo_list_tbl[language], dm_titleop_localize_file_num);

	// ロードチェック開始
	DmLogoComLoadFileStart(dm_titleop_load_tcb);
}

// ==========================================================================
// DmTitleOpLoadCheck
/*!
 *	オープニングタイトルデモデータロード ロード終了チェック
 *
 *	@return	TRUE : ロード終了
 */
// ==========================================================================
BOOL DmTitleOpLoadCheck(void)
{
	if ((dm_titleop_load_tcb == NULL) && (dm_titleop_data[0] != NULL)) {
		// データロードタスクがなくて 1つ目のデータが読み込まれている
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データビルド
// ==========================================================================
// ==========================================================================
// DmTitleOpBuild
/*!
 *	オープニングタイトルデモデータビルド
 *
 */
// ==========================================================================
void DmTitleOpBuild(void)
{
	s32			i;
	void		*tex_amb[DMD_TITLEOP_AOSTEX_MAX];
	AOS_TEXTURE	*aos_tex;

	MTM_ASSERT(DmTitleOpLoadCheck() == TRUE);
	MTM_ASSERT(dm_titleop_aos_tex == NULL);

	// ビルド処理生成
	dm_titleop_build_tcb = MTM_TASK_MAKE_TCB(dmTitleOpDataBuildMain, dmTitleOpDataBuildDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_TITLEOP_TASK_PRIO_DATA_BUILD, DMD_TITLEOP_TASK_GROUP_DATA_BUILD,
						0/*work_size*/, "DM_TOP_BUILD");

	// テクスチャ管理バッファ取得
	dm_titleop_aos_tex = (AOS_TEXTURE*)amMemAlloc(sizeof(AOS_TEXTURE) * DMD_TITLEOP_AOSTEX_MAX);
	MI_CpuClear8(dm_titleop_aos_tex, sizeof(AOS_TEXTURE) * DMD_TITLEOP_AOSTEX_MAX);

	// テクスチャビルド
	tex_amb[DMD_TITLEOP_AOSTEX_COM] =
				amBindGet((AMS_AMB_HEADER*)dm_titleop_data[DMD_TITLEOP_DATA_COM_AMA_SET_AMB],
									DMD_TITLEOP_AMA_DATA_SET_TEX_AMB);
//	tex_amb[DMD_TITLEOP_AOSTEX_LOCAL] =
//				amBindGet((AMS_AMB_HEADER*)dm_titleop_data[DMD_TITLEOP_DATA_LOCAL_AMA_SET_AMB],
//									DMD_TITLEOP_AMA_DATA_SET_TEX_AMB);

	aos_tex = dm_titleop_aos_tex;
	for (i = 0; i < DMD_TITLEOP_AOSTEX_MAX; i++, aos_tex++) {
		AoTexBuild(aos_tex, tex_amb[i]);
		AoTexLoad(aos_tex);
	}


	/* オブジェクトシステム初期化 */
	ObjInit(GMD_TASK_GROUP_OBJSYS, GMD_TASK_PRIO_OBJSYS, GMD_TASK_PAUSE_LEVEL_OBJSYS,
				GMD_OBJ_LCD_X, GMD_OBJ_LCD_Y, GSD_DISP_HEIGHT, GSD_DISP_HEIGHT);

	// データワーク数
	ObjDataAlloc(10);

	// ライト設定
#if _WII
	g_obj.def_user_light_flag |= OBD_LIGHT_USE_FLAG_7/*スペキュラ*/;
#endif	// #if _WII

//	// エフェクトシステム起動
//	ObjDrawESEffectSystemInit(DMD_TITLEOP_TASK_PAUSELEVEL_DEF,
//							  GMD_TASK_PRIO_EFFECT_SERVER,
//							  GMD_TASK_GROUP_EFFECT_SERVER);

	//描画順序設定
#if 1
	ObjDrawSetNNCommandStateTbl( 0, DMD_TITLEOP_DRAW_CMD_STATE_PRE_MAPFAR,	TRUE );
	ObjDrawSetNNCommandStateTbl( 1, DMD_TITLEOP_DRAW_CMD_STATE_MAPFAR_B,	TRUE );
	ObjDrawSetNNCommandStateTbl( 2, DMD_TITLEOP_DRAW_CMD_STATE_MAPFAR_SEA,	TRUE );
	ObjDrawSetNNCommandStateTbl( 3, DMD_TITLEOP_DRAW_CMD_STATE_MAPFAR_T,	TRUE );
	ObjDrawSetNNCommandStateTbl( 4, DMD_TITLEOP_DRAW_CMD_STATE_POST_MAPFAR,	TRUE );
	ObjDrawSetNNCommandStateTbl( 5, DMD_TITLEOP_DRAW_CMD_STATE_3DNN,		TRUE );

#else
#if _IPHONE
	ObjDrawSetNNCommandStateTbl( 0, OBD_DRAW_CMD_STATE_PRE_MAPFAR, TRUE );
#else
	ObjDrawSetNNCommandStateTbl( 0, OBD_DRAW_CMD_STATE_PRE_MAPFAR, TRUE );
#endif
	ObjDrawSetNNCommandStateTbl( 1, OBD_DRAW_CMD_STATE_MAPFAR, FALSE );
	ObjDrawSetNNCommandStateTbl( 2, OBD_DRAW_CMD_STATE_POST_MAPFAR, TRUE );
	ObjDrawSetNNCommandStateTbl( 3, OBD_DRAW_CMD_STATE_3DNN, TRUE );
#endif

	// 2Dアクションシステム設定
	// 各種最大使用量のクリア
	AoActSysClearPeak();


	/* ビルドシステム初期化 */
	GmGameDBuildModelBuildInit();


	/* データビルド */
	// 通常モデル
	dm_titleop_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)dm_titleop_mapfar_data[DMD_TITLEOP_MAPFAR_DATA_MDL],
								(AMS_AMB_HEADER*)dm_titleop_mapfar_data[DMD_TITLEOP_MAPFAR_DATA_TEX],
								0/*draw_flag*/);
	// 水モデル
	dm_titleop_water_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)dm_titleop_mapfar_data[DMD_TITLEOP_MAPFAR_DATA_MDL_WATER],
								(AMS_AMB_HEADER*)dm_titleop_mapfar_data[DMD_TITLEOP_MAPFAR_DATA_TEX],
#if !_WII && !_IPHONE
								NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1);
#else
								0/*draw_flag*/);
#endif
}


// ==========================================================================
// DmTitleOpBuildCheck
/*!
 *	オープニングタイトルデモデータビルド ビルド終了チェック
 *
 *	@return	TRUE : ビルド終了
 */
// ==========================================================================
BOOL DmTitleOpBuildCheck(void)
{
	if (dm_titleop_build_state) {
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データフラッシュ
// ==========================================================================
// ==========================================================================
// DmTitleOpFlush
/*!
 *	オープニングタイトルデモデータフラッシュ
 *
 */
// ==========================================================================
void DmTitleOpFlush(void)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;

//	// エフェクトシステム終了
//	MTM_ASSERT(ObjDrawESEffectSystemIsActive());
//	ObjDrawESEffectSystemExit();
	AMS_AMB_HEADER	*amb;

	// フラッシュ処理生成
	dm_titleop_flush_tcb = MTM_TASK_MAKE_TCB(dmTitleOpDataFlushMain, dmTitleOpDataFlushDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_TITLEOP_TASK_PRIO_DATA_FLUSH, DMD_TITLEOP_TASK_GROUP_DATA_FLUSH,
						0/*work_size*/, "DM_TOP_FLUSH");

	// テクスチャフラッシュ
	aos_tex = dm_titleop_aos_tex;
	for (i = 0; i < DMD_TITLEOP_AOSTEX_MAX; i++, aos_tex++) {
		AoTexRelease(aos_tex);
	}

	// フラッシュシステム初期化
	GmGameDBuildModelFlushInit();
	
	// 通常モデル
	amb = (AMS_AMB_HEADER*)dm_titleop_mapfar_data[DMD_TITLEOP_MAPFAR_DATA_MDL];
	GmGameDBuildRegFlushModel(dm_titleop_obj_3d_list, amb->file_num);
	// 水モデル
	amb = (AMS_AMB_HEADER*)dm_titleop_mapfar_data[DMD_TITLEOP_MAPFAR_DATA_MDL_WATER];
	GmGameDBuildRegFlushModel(dm_titleop_water_obj_3d_list, amb->file_num);
}

// ==========================================================================
// DmTitleOpFlushCheck
/*!
 *	オープニングタイトルデモデータフラッシュ フラッシュ終了チェック
 *
 *	@return	TRUE : フラッシュ終了
 */
// ==========================================================================
BOOL DmTitleOpFlushCheck(void)
{
	if (!dm_titleop_build_state) {
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データリリース
// ==========================================================================
// ==========================================================================
// DmTitleOpRelease
/*!
 *	オープニングタイトルデモデータリリース
 */
// ==========================================================================
void DmTitleOpRelease(void)
{
	s32	i;

	for (i = 0; i < DMD_TITLEOP_DATA_MAX; i++) {
		if (dm_titleop_data[i]) {
			amMemFree(dm_titleop_data[i]);
		}
		dm_titleop_data[i] = NULL;
	}

	// 遠景データワーククリア
	for (i = 0; i < DMD_TITLEOP_MAPFAR_DATA_MAX; i++) {
		dm_titleop_mapfar_data[i] = NULL;
	}
}

// ==========================================================================
// DmTitleOpReleaseCheck
/*!
 *	オープニングタイトルデモデータリリース リリース終了チェック
 *
 *	@return	TRUE : リリース終了
 */
// ==========================================================================
BOOL DmTitleOpReleaseCheck(void)
{
	if ((dm_titleop_load_tcb == NULL) && (dm_titleop_data[0] == NULL)) {
		// データロードタスクがなくて 1つ目のデータが読み込まれていない
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// 初期化
// ==========================================================================
// ==========================================================================
// DmTitleOpInit
/*!
 *	タイトルオープニング演出開始
 */
// ==========================================================================
void DmTitleOpInit(void)
{
	NNS_RGBA	light_col = {
		1.0f, 1.0f, 1.0f, 1.0f,
	};
	NNS_VECTOR	light_vec;
	NNS_VECTOR	cam_pos = {0.f, 0.f, 50.f};
	OBS_CAMERA	*camera;

#if DMD_TITLEOP_DEBUG_TEST
	// フェードイン
	IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEIN, 16.0f, TRUE);
#endif

	/* オブジェクトシステム設定 */
	g_obj.flag = OBD_OBJ_CAMERA | OBD_OBJ_RECT_NOUSE_DRAWSCALE | OBD_OBJ_DEFAULT_NOCLIP;

	g_obj.ppPre			= NULL;//GmObjPreFunc;		// システム前処理
	g_obj.ppPost		= NULL;						// システム後処理
	//g_obj.ppDrawSort	= GmObjDrawSort;			// 描画前オブジェクトソート
	g_obj.ppCollision	= NULL;//GmObjCollision;	// あたり処理
	g_obj.ppObjPre		= NULL;//GmObjObjPreFunc;	// オブジェクト共通前処理
	g_obj.ppObjPost		= NULL;						// オブジェクト共通後処理
	g_obj.ppRegRecAuto	= NULL;						// 矩形自動登録処理

	// 描画スケール設定
	g_obj.draw_scale.x = g_obj.draw_scale.y = g_obj.draw_scale.z = GMD_OBJ_DRAW_SCALE_FX;
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
	ObjCameraInit(DMD_TITLEOP_CAMERA_ID, &cam_pos, DMD_TITLEOP_TASK_GROUP_CAMERA, DMD_TITLEOP_TASK_PAUSELEVEL_DEF, DMD_TITLEOP_TASK_PRIO_CAMERA);
	ObjCamera3dInit(DMD_TITLEOP_CAMERA_ID);
	g_obj.glb_camera_id = DMD_TITLEOP_CAMERA_ID;
	g_obj.glb_camera_type = NNE_PROJECTION_TYPE_PERSPECTIVE;
	camera = ObjCameraGet(DMD_TITLEOP_CAMERA_ID);
	camera->user_func = dmTitleOpCamera;
	camera->command_state = DMD_TITLEOP_DRAW_CMD_STATE_PRE_MAPFAR;
//	camera->scale = GMD_CAMERA_SCALE;
//	camera->ofst.z = 1000.0f;
	camera->fovy = NNM_DEGtoA32(40.0f);
	camera->znear = 0.1f;
	camera->zfar = 32768.0f;
	/* マネージャー初期化 */
	dmTitleOpMgrInit();
}


// ==========================================================================
// 終了処理
// ==========================================================================
// ==========================================================================
// DmTitleOpExit
/*!
 *	タイトルオープニング演出終了
 */
// ==========================================================================
void DmTitleOpExit(void)
{
	DMS_TITLEOP_MGR_WORK	*top_mgr_work;
	if (dm_titleop_mgr_tcb) {
		top_mgr_work = (DMS_TITLEOP_MGR_WORK*)mtTaskGetTcbWork(dm_titleop_mgr_tcb);

		// マネージャー終了処理開始
		top_mgr_work->flag |= DMD_TITLEOP_FLAG_EXIT;
	}
}

// ==========================================================================
// DmTitleOpExitEndCheck
/*!
 *	タイトルオープニング終了処理 終了チェック
 */
// ==========================================================================
BOOL DmTitleOpExitEndCheck(void)
{
	if (dm_titleop_mgr_tcb) {
		return (FALSE);
	}
	return (TRUE);
}


// ==========================================================================
// 2Dアクション描画
// ==========================================================================
// ==========================================================================
// DmTitleOpDraw2D
/*!
 *	2Dアクション描画
 */
// ==========================================================================
void DmTitleOpDraw2D(void)
{
	s32						i;
	DMS_TITLEOP_MGR_WORK	*top_mgr_work;

	if (dm_titleop_mgr_tcb == NULL) {
		MTM_ASSERT(0);
		return;
	}

	top_mgr_work = (DMS_TITLEOP_MGR_WORK*)mtTaskGetTcbWork(dm_titleop_mgr_tcb);

	if (!(top_mgr_work->flag & DMD_TITLEOP_FLAG_2DACT_READY)) {
		return;
	}

	// 描画
	AoActSetTexture(AoTexGetTexList(dm_titleop_aos_tex + DMD_TITLEOP_AOSTEX_COM));
	for (i = 0; i < DMD_TITLEOP_AOS_ACT_MAX; i++) {
		if (!(top_mgr_work->flag & (DMD_TITLEOP_FLAG_ACT_DISP_LOGO_1 << i)) ||
				(top_mgr_work->flag & (DMD_TITLEOP_FLAG_ACT_USER_NODISP_LOGO_1 << i))) {
			continue;
		}
		AoActAcmPush();
		AoActAcmInit();
		if (i == DMD_TITLEOP_AOS_ACT_FINGER) {
			// 指用
			AoActSetFrame(top_mgr_work->act[i], top_mgr_work->finger_frame);
			AoActUpdate(top_mgr_work->act[i], 0.0f);

			// ループ設定
			top_mgr_work->finger_frame += 1.f;
			if (top_mgr_work->finger_frame > DMD_TITLEOP_ACT_FINGER_WAVE_E_FRAME) {
				top_mgr_work->finger_frame = DMD_TITLEOP_ACT_FINGER_WAVE_S_FRAME;
			}
		}
		else {
			// 通常更新
			AoActUpdate(top_mgr_work->act[i]);
		}
		AoActSortRegAction(top_mgr_work->act[i]);
		AoActAcmPop(1);
	}
}


// ==========================================================================
// 演出終了状況
// ==========================================================================
// ==========================================================================
// DmTitleOpIsLogoActFinish
/*!
 *	2Dアクション描画
 *
 *	@return	TRUE : ロゴ演出終了
 */
// ==========================================================================
BOOL DmTitleOpIsLogoActFinish(void)
{
	DMS_TITLEOP_MGR_WORK	*top_mgr_work;

	if (dm_titleop_mgr_tcb == NULL) {
		MTM_ASSERT(0);
		return (FALSE);
	}

	top_mgr_work = (DMS_TITLEOP_MGR_WORK*)mtTaskGetTcbWork(dm_titleop_mgr_tcb);

	if (top_mgr_work->flag & DMD_TITLEOP_FLAG_ALL_ACT_END) {
		return (TRUE);
	}

	return (FALSE);
}

// ==========================================================================
// 表示設定
// ==========================================================================
// ==========================================================================
// DmTitleOpDispRightEnable
/*!
 *	権利表記表示設定
 *
 *	@param	disp	[in]	TRUE : 表示		FALSE : 非表示
 *
 *	@note
 *		演出の流れで非表示になっている場合は、TRUEを設定しても
 *		表示タイミングになるまで表示されません
 *		非表示にしたアクションはアップデートも行いません
 */
// ==========================================================================
void DmTitleOpDispRightEnable(BOOL disp)
{
	DMS_TITLEOP_MGR_WORK	*top_mgr_work;

	if (dm_titleop_mgr_tcb == NULL) {
		MTM_ASSERT(0);
		return;
	}

	top_mgr_work = (DMS_TITLEOP_MGR_WORK*)mtTaskGetTcbWork(dm_titleop_mgr_tcb);

	if (disp) {
		top_mgr_work->flag &= ~DMD_TITLEOP_FLAG_ACT_USER_NODISP_RIGHT;
	}
	else {
		top_mgr_work->flag |= DMD_TITLEOP_FLAG_ACT_USER_NODISP_RIGHT;
	}
}

// ==========================================================================
// DmTitleOpSetRetOptionState
/*!
 *	タイトルオープニング オプションからの復帰状態に設定
 */
// ==========================================================================
void DmTitleOpSetRetOptionState(void)
{
	s32						i;
	DMS_TITLEOP_MGR_WORK	*top_mgr_work;

	if (dm_titleop_mgr_tcb == NULL) {
		MTM_ASSERT(0);
		return;
	}

	top_mgr_work = (DMS_TITLEOP_MGR_WORK*)mtTaskGetTcbWork(dm_titleop_mgr_tcb);

	// 全アイテムセット終了状態に
	top_mgr_work->flag |= DMD_TITLEOP_FLAG_ACT_DISP_LOGO_1 |
							DMD_TITLEOP_FLAG_ACT_DISP_FINGER |
#if SONIC4_TRIAL
							DMD_TITLEOP_FLAG_ACT_DISP_FREE |
							DMD_TITLEOP_FLAG_ACT_DISP_FREE_RED |
#endif //SONIC4_TRIAL
							DMD_TITLEOP_FLAG_ACT_DISP_LOGO_2 |
							DMD_TITLEOP_FLAG_ACT_DISP_LOGO_3 |
							DMD_TITLEOP_FLAG_ACT_DISP_RIGHT |
							DMD_TITLEOP_FLAG_ALL_ACT_END;

	// フレームを全アイテムセット時フレームに
	top_mgr_work->frame = DMD_TITLEOP_ACT_ALL_SET_END_FRAME;

	top_mgr_work->finger_frame = DMD_TITLEOP_ACT_ALL_SET_END_FRAME - DMD_TITLEOP_ACT_LOGO_START_FRANE;

	// アクションフレーム更新
	AoActSetTexture(AoTexGetTexList(dm_titleop_aos_tex + DMD_TITLEOP_AOSTEX_COM));
	for (i = 0; i < DMD_TITLEOP_AOS_ACT_MAX; i++) {
		AoActAcmPush();
		AoActAcmInit();
		AoActSetFrame(top_mgr_work->act[i], DMD_TITLEOP_ACT_ALL_SET_END_FRAME - DMD_TITLEOP_ACT_LOGO_START_FRANE);
		AoActUpdate(top_mgr_work->act[i], 0.0f);
		AoActAcmPop(1);
	}
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// ファイルロード時後処理
// ==========================================================================
// ==========================================================================
// dmTitleOpLoadPostFuncMapFar
/*!
 *	データロード後処理 遠景
 *
 *	@param	context	[in]	コンテキスト
 */
// ==========================================================================
void dmTitleOpLoadPostFuncMapFar(DMS_LOGO_COM_LOAD_CONTEXT *context)
{
	s32				i;
	AMS_AMB_HEADER	*amb_header;

	// 保存
	dm_titleop_data[context->no] = context->fs_req->buf;
	context->fs_req->buf = NULL;

	// AMBコンバート
	amBindConv((u8*)dm_titleop_data[context->no]);

	// データ取得
	amb_header = (AMS_AMB_HEADER*)dm_titleop_data[context->no];
	MTM_ASSERT(amb_header->file_num <= DMD_TITLEOP_MAPFAR_DATA_MAX);
	for (i = 0; i < amb_header->file_num; i++) {
		dm_titleop_mapfar_data[i] = amBindGet(amb_header, i, NULL);
	}
}

// ==========================================================================
// dmTitleOpLoadPostFuncTitleLogo
/*!
 *	データロード後処理 タイトルロゴデータ
 *
 *	@param	context	[in]	コンテキスト
 */
// ==========================================================================
void dmTitleOpLoadPostFuncTitleLogo(DMS_LOGO_COM_LOAD_CONTEXT *context)
{
	// 保存
	dm_titleop_data[context->no] = context->fs_req->buf;
	context->fs_req->buf = NULL;

	// AMBコンバートオール
	amBindConvertAll((u8*)dm_titleop_data[context->no]);
}

// ==========================================================================
// データビルド
// ==========================================================================
// ==========================================================================
// dmTitleOpDataBuildMain
/*!
 *	データビルドメイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmTitleOpDataBuildMain(MTS_TASK_TCB *tcb)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;
	BOOL		b_sts = TRUE;

	// テクスチャビルド
	aos_tex = dm_titleop_aos_tex;
	for (i = 0; i < DMD_TITLEOP_AOSTEX_MAX; i++, aos_tex++) {
		if (!AoTexIsLoaded(aos_tex)) {
			b_sts = FALSE;
		}
	}

	// ビルド待機
	if (GmGameDBuildCheckBuildModel() == FALSE) {
		b_sts = FALSE;
	}

	// ビルド待機
	if (!b_sts) {
		return;
	}

	// ビルド終了
	mtTaskClearTcb(tcb);
	dm_titleop_build_state = TRUE;
}

// ==========================================================================
// dmTitleOpDataBuildDest
/*!
 *	データビルドデストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmTitleOpDataBuildDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dm_titleop_build_tcb = NULL;
}

// ==========================================================================
// データフラッシュ
// ==========================================================================
// ==========================================================================
// dmTitleOpDataFlushMain
/*!
 *	データフラッシュメイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmTitleOpDataFlushMain(MTS_TASK_TCB *tcb)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;
	BOOL		b_sts = TRUE;

	// テクスチャフラッシュ
	aos_tex = dm_titleop_aos_tex;
	for (i = 0; i < DMD_TITLEOP_AOSTEX_MAX; i++, aos_tex++) {
		if (!AoTexIsReleased(aos_tex)) {
			b_sts = FALSE;
		}
	}

	// ビルド待機
	if (GmGameDBuildCheckFlushModel() == FALSE) {
		b_sts = FALSE;
	}

	// フラッシュ待機
	if (!b_sts) {
		return;
	}

	amMemFree(dm_titleop_aos_tex);
	dm_titleop_aos_tex = NULL;

	dm_titleop_obj_3d_list = NULL;
	dm_titleop_water_obj_3d_list = NULL;

	// ビルド終了
	mtTaskClearTcb(tcb);
	dm_titleop_build_state = FALSE;
}

// ==========================================================================
// dmTitleOpDataFlushDest
/*!
 *	データフラッシュデストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmTitleOpDataFlushDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dm_titleop_flush_tcb = NULL;
}

// ==========================================================================
// マネージャー
// ==========================================================================
// ==========================================================================
// dmTitleOpMgrInit
/*!
 *	タイトルオープニング マネージャー 初期化
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmTitleOpMgrInit(void)
{
	DMS_TITLEOP_MGR_WORK	*top_mgr_work;

	/* メイン処理生成 */
	dm_titleop_mgr_tcb = MTM_TASK_MAKE_TCB(dmTitleOpMgrMain, dmTitleOpMgrDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_TITLEOP_TASK_PRIO_MGR, DMD_TITLEOP_TASK_GROUP_MGR,
						sizeof(DMS_TITLEOP_MGR_WORK)/*work_size*/, "DM_TOP_MGR");

	top_mgr_work = (DMS_TITLEOP_MGR_WORK*)mtTaskGetTcbWork(dm_titleop_mgr_tcb);
    MI_CpuClear8(top_mgr_work, sizeof(DMS_TITLEOP_MGR_WORK));

	/* オブジェクト生成 */
	// 空
	dmTitleOpCreateObjFarSky();
	// 岩
	dmTitleOpCreateObjFarRock(0);
	dmTitleOpCreateObjFarRock(1);
	dmTitleOpCreateObjFarRock(2);
	// 海
	dmTitleOpCreateObjFarSea();

	/* ロゴアクション生成 */
	dmTitleOpCreateAction(top_mgr_work);

	// マップスクロール開始
	top_mgr_work->flag |= DMD_TITLEOP_FLAG_SCRL;

	/* 画面スクロール量初期化 */
	dm_titleop_scrl_x_ofst = 0;
}

// ==========================================================================
// dmTitleOpMgrDest
/*!
 *	タイトルオープニング マネージャー デストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmTitleOpMgrDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	dm_titleop_mgr_tcb = NULL;
}

// ==========================================================================
// dmTitleOpMgrMain
/*!
 *	タイトルオープニング マネージャー メイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmTitleOpMgrMain(MTS_TASK_TCB *tcb)
{
	DMS_TITLEOP_MGR_WORK	*top_mgr_work;

	top_mgr_work = (DMS_TITLEOP_MGR_WORK*)mtTaskGetTcbWork(tcb);

	// 終了チェック
	if (top_mgr_work->flag & DMD_TITLEOP_FLAG_EXIT) {
		// 終了処理開始
		dmTitleOpEndStart(tcb);
		return;
	}

	top_mgr_work->frame++;

	if (top_mgr_work->frame == DMD_TITLEOP_ACT_LOGO_START_FRANE) {
		// ロゴ表示開始
		top_mgr_work->flag |= DMD_TITLEOP_FLAG_ACT_DISP_LOGO_1 |
								DMD_TITLEOP_FLAG_ACT_DISP_FINGER |
#if SONIC4_TRIAL
								DMD_TITLEOP_FLAG_ACT_DISP_FREE |
								DMD_TITLEOP_FLAG_ACT_DISP_FREE_RED |
#endif //SONIC4_TRIAL
								DMD_TITLEOP_FLAG_ACT_DISP_LOGO_2 |
								DMD_TITLEOP_FLAG_ACT_DISP_LOGO_3;
	}
	if (top_mgr_work->frame == DMD_TITLEOP_ACT_RIGHT_START_FRANE) {
		// 権利表示表示開始
		top_mgr_work->flag |= DMD_TITLEOP_FLAG_ACT_DISP_RIGHT;
	}
	else if (top_mgr_work->frame >= DMD_TITLEOP_ACT_ALL_SET_END_FRAME) {
		// 全アイテムセット終了
		top_mgr_work->flag |= DMD_TITLEOP_FLAG_ALL_ACT_END;
	}

#if DMD_TITLEOP_DEBUG_TEST_EXIT

	if (top_mgr_work->frame > 500) {
		DmTitleTestExit();
	}
#endif

	// スクロール処理
	if (top_mgr_work->flag & DMD_TITLEOP_FLAG_SCRL) {
		s32	div;
		dm_titleop_scrl_x_ofst += DMD_TITLEOP_MAP_SCRL_SPD;

		div = dm_titleop_scrl_x_ofst / DMD_TITLEOP_MAP_SCRL_LOOP_DIST;
		dm_titleop_scrl_x_ofst -= div * DMD_TITLEOP_MAP_SCRL_LOOP_DIST;
	}

	// 描画前処理を登録
	ObjDraw3DNNUserFunc(dmTitleOpPreDrawDT, NULL, 0, DMD_TITLEOP_DRAW_CMD_STATE_PRE_MAPFAR);

	// 滝シェーダー設定
	ObjDraw3DNNUserFunc(dmTitleOpFallShaderPreRenderUserFunc, NULL, 0, DMD_TITLEOP_DRAW_CMD_STATE_MAPFAR_SEA);

	// フォグ開始設定
	amDrawSetFog( DMD_TITLEOP_DRAW_CMD_STATE_PRE_MAPFAR, TRUE );
	amDrawSetFogColor( DMD_TITLEOP_DRAW_CMD_STATE_PRE_MAPFAR, 0.7f, 0.95f, 1.0f );
#if DMD_TITLEOP_DEBUG_TEST_FOG
	amDrawSetFogRange( DMD_TITLEOP_DRAW_CMD_STATE_PRE_MAPFAR, fog_near, fog_far );
#else
	amDrawSetFogRange( DMD_TITLEOP_DRAW_CMD_STATE_PRE_MAPFAR, 1.0f, 500.0f );
#endif
	// フォグ終了設定
	amDrawSetFog( DMD_TITLEOP_DRAW_CMD_STATE_POST_MAPFAR, FALSE );

#if DMD_TITLEOP_DEBUG_TEST_FOG
	if (AoPadSomeoneRepeat(KEY_R_UP) >= 0) {
		if (AoPadSomeoneRepeat(KEY_L_UP) >= 0) {
			if (AoPadSomeoneRepeat(KEY_R1) >= 0) {
				fog_near += 10.f;
			}
			else if (AoPadSomeoneRepeat(KEY_L1) >= 0) {
				fog_near += 100.f;
			}
			else {
				fog_near += 1.f;
			}
		}
		else if (AoPadSomeoneRepeat(KEY_L_DOWN) >= 0) {
			if (AoPadSomeoneRepeat(KEY_R1) >= 0) {
				fog_near -= 10.f;
			}
			else if (AoPadSomeoneRepeat(KEY_L1) >= 0) {
				fog_near -= 100.f;
			}
			else {
				fog_near -= 1.f;
			}
		}
	}
	else if (AoPadSomeoneRepeat(KEY_R_LEFT) >= 0) {
		if (AoPadSomeoneRepeat(KEY_L_UP) >= 0) {
			if (AoPadSomeoneRepeat(KEY_R1) >= 0) {
				fog_far += 10.f;
			}
			else if (AoPadSomeoneRepeat(KEY_L1) >= 0) {
				fog_far += 100.f;
			}
			else {
				fog_far += 1.f;
			}
		}
		else if (AoPadSomeoneRepeat(KEY_L_DOWN) >= 0) {
			if (AoPadSomeoneRepeat(KEY_R1) >= 0) {
				fog_far -= 10.f;
			}
			else if (AoPadSomeoneRepeat(KEY_L1) >= 0) {
				fog_far -= 100.f;
			}
			else {
				fog_far -= 1.f;
			}
		}
	}
#endif

#if DMD_TITLEOP_DEBUG_TEST
	{
		DMS_TITLEOP_MGR_WORK	*top_mgr_work;
		AoActSysSetDrawState(10);
		AoActSysSetDrawStateEnable(TRUE);
		top_mgr_work = (DMS_TITLEOP_MGR_WORK*)mtTaskGetTcbWork(tcb);
		AoActSysSetDrawTaskPrio();	// 標準設定
		DmTitleOpDraw2D();
		// ソート描画
		//ObjDrawAction2DAMADrawStart();	// とりあえず...
		
		// 前処理登録
		//ObjDraw3DNNUserFunc(objDraw2DAMAPre_DT,
		///					NULL, 0, OBD_DRAW_CMD_STATE_2DAMA);
		amDrawMakeTask(dmTitleOpTestTaskDraw, (u16)0x8000, (u32)0);

		// アクションのソート
		AoActSortExecute();

		// 描画
		AoActSortDraw();

		// 登録解除
		AoActSortUnregAll();
	}
#endif

#if DMD_TITLEOP_DEBUG_TEST_ROCK_EDIT
	dmTitleOpDebugTestRockEdit(top_mgr_work);
#endif
}


// ==========================================================================
// 背景オブジェクト
// ==========================================================================
// ==========================================================================
// dmTitleOpCreateObjFarSky
/*!
 *	遠景 空オブジェクト生成
 *
 *	@return	オブジェクトワーク
 */
// ==========================================================================
void dmTitleOpCreateObjFarSky(void)
{
	OBS_OBJECT_WORK				*obj_work;
	DMS_TITLEOP_OBJ_3DNN_WORK	*top3d_work;

	/* 上空 */
	// オブジェクト生成
	top3d_work = DMM_TITLEOP_CREATE_3D_OBJ(DMD_TITLEOP_TASK_PRIO_FAR_SKY_T, DMD_TITLEOP_TASK_GROUP_FAR_SKY_T,
						sizeof(DMS_TITLEOP_OBJ_3DNN_WORK), "DM_TOP_SKYT");
	obj_work = (OBS_OBJECT_WORK*)top3d_work;
	obj_work->obj_type = DMD_TITLEOP_OBJTYPE_MAPFAR;
	// モデルロード
	ObjObjectCopyAction3dNNModel(obj_work,
#if !_IPHONE
					&dm_titleop_obj_3d_list[IDB_MAPFAR_ZONE1_MDL_Z1_SKYL_T_D_ZNO],
#else //!_IPHONE
					&dm_titleop_obj_3d_list[IDB_MAPFAR_ZONE1_MDL_Z1_SKYL_T_D_INO],
#endif //!_IPHONE
					&top3d_work->obj_3d);
	obj_work->obj_3d->command_state = DMD_TITLEOP_DRAW_CMD_STATE_MAPFAR_T;
	// マテリアルモーション
	ObjAction3dNNMaterialMotionLoad(&top3d_work->obj_3d,
	                                 0,				// reg_file_id
	                                 NULL,			// data_work
	                                 NULL,			// filename
	                                 IDB_MAPFAR_ZONE1_MAT_Z1_SKYL_T_D_ZNV,
	                                 (void*)dm_titleop_mapfar_data[DMD_TITLEOP_MAPFAR_DATA_MAT]);
	ObjDrawObjectActionSet3DNNMaterial(obj_work, 0);
	obj_work->disp_flag |= OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP | OBD_DISP_NOSCALE |
							OBD_DISP_NODRAWSCALE | OBD_DISP_USERMTX;
	// メイン処理設定
	obj_work->ppFunc = dmTitleOpFarSkyFunc;

	/* 下空 */
	// オブジェクト生成
	top3d_work = DMM_TITLEOP_CREATE_3D_OBJ(DMD_TITLEOP_TASK_PRIO_FAR_SKY_B, DMD_TITLEOP_TASK_GROUP_FAR_SKY_B,
						sizeof(DMS_TITLEOP_OBJ_3DNN_WORK), "DM_TOP_SKYB");
	obj_work = (OBS_OBJECT_WORK*)top3d_work;
	obj_work->obj_type = DMD_TITLEOP_OBJTYPE_MAPFAR;
	// モデルロード
	ObjObjectCopyAction3dNNModel(obj_work,
#if !_IPHONE
					&dm_titleop_obj_3d_list[IDB_MAPFAR_ZONE1_MDL_Z1_SKYL_B_D_ZNO],
#else //!_IPHONE
					//定義が無い
					//&dm_titleop_obj_3d_list[IDB_MAPFAR_ZONE1_MDL_Z1_SKYL_B_D_INO],
					&dm_titleop_obj_3d_list[0],
#endif //!_IPHONE
					&top3d_work->obj_3d);
	obj_work->obj_3d->command_state = DMD_TITLEOP_DRAW_CMD_STATE_MAPFAR_B;
	// マテリアルモーション
	ObjAction3dNNMaterialMotionLoad(&top3d_work->obj_3d,
	                                 0,				// reg_file_id
	                                 NULL,			// data_work
	                                 NULL,			// filename
	                                 IDB_MAPFAR_ZONE1_MAT_Z1_SKYL_T_D_ZNV,
	                                 (void*)dm_titleop_mapfar_data[DMD_TITLEOP_MAPFAR_DATA_MAT]);
	ObjDrawObjectActionSet3DNNMaterial(obj_work, 0);
	obj_work->disp_flag |= OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP | OBD_DISP_NOSCALE |
							OBD_DISP_NODRAWSCALE | OBD_DISP_USERMTX;
	// メイン処理設定
	obj_work->ppFunc = dmTitleOpFarSkyFunc;
}

// ==========================================================================
// dmTitleOpCreateObjFarSky
/*!
 *	遠景 空オブジェクト処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void dmTitleOpFarSkyFunc(OBS_OBJECT_WORK *obj_work)
{
	DMS_TITLEOP_OBJ_3DNN_WORK	*top3d_work = (DMS_TITLEOP_OBJ_3DNN_WORK*)obj_work;

	// 天球回転
	top3d_work->sky_rot += amSystemGetFrameRateMain() * 0.01f;
	if (top3d_work->sky_rot > 360.f) {
		top3d_work->sky_rot -= 360.f;
	}

	nnMakeUnitMatrix(&obj_work->obj_3d->user_obj_mtx);
	nnRotateYMatrix(&obj_work->obj_3d->user_obj_mtx, &obj_work->obj_3d->user_obj_mtx, (u16)NNM_DEGtoA16(top3d_work->sky_rot));
}

// ==========================================================================
// dmTitleOpCreateObjFarRock
/*!
 *	遠景 岩オブジェクト生成
 *
 *	@param	type	[in]	岩タイプ
 *
 *	@return	オブジェクトワーク
 */
// ==========================================================================
void dmTitleOpCreateObjFarRock(u32 type)
{
	OBS_OBJECT_WORK				*obj_work;
	DMS_TITLEOP_OBJ_3DNN_WORK	*top3d_work;

	MTM_ASSERT(type < DMD_TITLEOP_ROCK_TYPE_NUM);

	// 上岩
	top3d_work = DMM_TITLEOP_CREATE_3D_OBJ(DMD_TITLEOP_TASK_PRIO_FAR_ROCK_T, DMD_TITLEOP_TASK_GROUP_FAR_ROCK_T,
						sizeof(DMS_TITLEOP_OBJ_3DNN_WORK), "DM_TOP_ROCKT");
	obj_work = (OBS_OBJECT_WORK*)top3d_work;
	obj_work->obj_type = DMD_TITLEOP_OBJTYPE_MAPFAR;
	// モデルロードlll
	ObjObjectCopyAction3dNNModel(obj_work,
#if !_IPHONE
					&dm_titleop_obj_3d_list[IDB_MAPFAR_ZONE1_MDL_Z1_ROCK_A_T_D_ZNO + type],
#else //!_IPHONE
					&dm_titleop_obj_3d_list[IDB_MAPFAR_ZONE1_MDL_Z1_ROCK_A_T_D_INO + type],
#endif //!_IPHONE
					&top3d_work->obj_3d);
	obj_work->obj_3d->command_state = DMD_TITLEOP_DRAW_CMD_STATE_MAPFAR_T;
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE;
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_NODRAWSCALE;
#if _IPHONE
	obj_work->disp_flag |= OBD_DISP_NOCLIP;
#endif //_IPHONE
	//obj_work->pos = *pos;
	// 描画処理変更
	obj_work->ppOut = dmTitleOpObjRockDraw;
	// 設定情報取得
	top3d_work->rock_setting	= &dm_titleop_rock_setting[type][0];
	top3d_work->rock_setting_num= &dm_titleop_rock_setting_num[type];

#if !_IPHONE
	// 下岩
	top3d_work = DMM_TITLEOP_CREATE_3D_OBJ(DMD_TITLEOP_TASK_PRIO_FAR_ROCK_B, DMD_TITLEOP_TASK_GROUP_FAR_ROCK_B,
						sizeof(DMS_TITLEOP_OBJ_3DNN_WORK), "DM_TOP_ROCKB");
	obj_work = (OBS_OBJECT_WORK*)top3d_work;
	obj_work->obj_type = DMD_TITLEOP_OBJTYPE_MAPFAR;
	// モデルロード
	ObjObjectCopyAction3dNNModel(obj_work,
					&dm_titleop_obj_3d_list[IDB_MAPFAR_ZONE1_MDL_Z1_ROCK_A_B_D_ZNO + type],
					&top3d_work->obj_3d);
	obj_work->obj_3d->command_state = DMD_TITLEOP_DRAW_CMD_STATE_MAPFAR_B;
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE;
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_NODRAWSCALE;
	//obj_work->pos = *pos;
	// 描画処理変更
	obj_work->ppOut = dmTitleOpObjRockDraw;
	// 設定情報取得
	top3d_work->rock_setting	= &dm_titleop_rock_setting[type][0];
	top3d_work->rock_setting_num= &dm_titleop_rock_setting_num[type];
#endif //!_IPHONE
}

// ==========================================================================
// dmTitleOpCreateObjFarSea
/*!
 *	遠景 海オブジェクト生成
 *
 *	@return	オブジェクトワーク
 */
// ==========================================================================
void dmTitleOpCreateObjFarSea(void)
{
	OBS_OBJECT_WORK				*obj_work;
	DMS_TITLEOP_OBJ_3DNN_WORK	*top3d_work;

	// 海
	top3d_work = DMM_TITLEOP_CREATE_3D_OBJ(DMD_TITLEOP_TASK_PRIO_FAR_SEA, DMD_TITLEOP_TASK_GROUP_FAR_SEA,
						sizeof(DMS_TITLEOP_OBJ_3DNN_WORK), "DM_TOP_SEA");
	obj_work = (OBS_OBJECT_WORK*)top3d_work;
	obj_work->obj_type = DMD_TITLEOP_OBJTYPE_MAPFAR;

	// モデルロード
	ObjObjectCopyAction3dNNModel(obj_work,
					&dm_titleop_water_obj_3d_list[IDB_MAPFAR_ZONE1_RENDER_MDL_Z1_SEA_D_ZNO],
					&top3d_work->obj_3d);
	obj_work->obj_3d->command_state = DMD_TITLEOP_DRAW_CMD_STATE_MAPFAR_SEA;
	// マテリアルモーション
	ObjAction3dNNMaterialMotionLoad(&top3d_work->obj_3d,
	                                 0,				// reg_file_id
	                                 NULL,			// data_work
	                                 NULL,			// filename
	                                 IDB_MAPFAR_ZONE1_MAT_Z1_SEA_D_ZNV,
	                                 (void*)dm_titleop_mapfar_data[DMD_TITLEOP_MAPFAR_DATA_MAT]);
	ObjDrawObjectActionSet3DNNMaterial(obj_work, 0);
#if _WII
	// マテリアルコールバック設定
	obj_work->obj_3d->material_cb_func = dmTitleOpFallMaterialCallback;
	obj_work->obj_3d->material_cb_param = NULL;
#endif
	obj_work->obj_3d->mat_speed = 0.2f;
	obj_work->dir.y = 0xC000;
	obj_work->disp_flag |= OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP | OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE;
}

// ==========================================================================
// dmTitleOpCreateObjFarSea
/*!
 *	遠景 海オブジェクト生成
 *
 *	@return	オブジェクトワーク
 */
// ==========================================================================
#if defined (MTD_DEBUG)
DMS_TITLEOP_OBJ_3DNN_WORK* dmTitleOpCreate3DObj(u16 prio, u8 group, u32 work_size, const char *name)
#else
DMS_TITLEOP_OBJ_3DNN_WORK* dmTitleOpCreate3DObj(u16 prio, u8 group, u32 work_size)
#endif
{
	OBS_OBJECT_WORK				*obj_work;
	DMS_TITLEOP_OBJ_3DNN_WORK	*top3d_work;

	MTM_ASSERT(work_size >= sizeof(DMS_TITLEOP_OBJ_3DNN_WORK));

#if defined (MTD_DEBUG)
	obj_work = OBM_OBJECT_TASK_DETAIL_INIT(prio, group,
					DMD_TITLEOP_TASK_PAUSELEVEL_DEF, DMD_TITLEOP_OBJ_OBJPAUSELEVEL_DEF, work_size, name);
#else
	obj_work = OBM_OBJECT_TASK_DETAIL_INIT(prio, group,
					DMD_TITLEOP_TASK_PAUSELEVEL_DEF, DMD_TITLEOP_OBJ_OBJPAUSELEVEL_DEF, work_size, NULL);
#endif
	top3d_work = (DMS_TITLEOP_OBJ_3DNN_WORK*)obj_work;

	//obj_work->obj_type = DMD_TITLEOP_OBJTYPE_MAPFAR;

	// 標準関数設定
	obj_work->ppOut			= dmTitleOpObjDraw;
	obj_work->ppOutSub		= NULL;
	obj_work->ppIn			= NULL;
	obj_work->ppMove		= NULL;
	obj_work->ppActCall		= NULL;
	obj_work->ppRec			= NULL;
	obj_work->ppLast		= NULL;
	obj_work->ppFunc		= NULL;

	return (top3d_work);
}

// ==========================================================================
// ロゴアクション
// ==========================================================================
// ==========================================================================
// dmTitleOpCreateAction
/*!
 *	アクション生成
 *
 *	@return	オブジェクトワーク
 */
// ==========================================================================
void dmTitleOpCreateAction(DMS_TITLEOP_MGR_WORK *top_mgr_work)
{
	s32		i;
	void	*ama;

	MTM_ASSERT(top_mgr_work);

	// アクション構築
	ama = amBindGet((AMS_AMB_HEADER*)dm_titleop_data[DMD_TITLEOP_DATA_COM_AMA_SET_AMB],
				DMD_TITLEOP_AMA_DATA_SET_AMA);

	for (i = 0; i < DMD_TITLEOP_AOS_ACT_MAX; i++) {
		// アクション構築
		AoActSetTexture(AoTexGetTexList(dm_titleop_aos_tex + DMD_TITLEOP_AOSTEX_COM));
		top_mgr_work->act[i] = AoActCreate(ama, i);
	}

	// アクション準備終了
	top_mgr_work->flag |= DMD_TITLEOP_FLAG_2DACT_READY;
}

// ==========================================================================
// dmTitleOpDeleteAction
/*!
 *	アクション破棄
 *
 *	@return	オブジェクトワーク
 */
// ==========================================================================
void dmTitleOpDeleteAction(DMS_TITLEOP_MGR_WORK *top_mgr_work)
{
	s32		i;

	for (i = 0; i < DMD_TITLEOP_AOS_ACT_MAX; i++) {
		AoActDelete(top_mgr_work->act[i]);
	}
}


// ==========================================================================
// 終了処理
// ==========================================================================
// ==========================================================================
// dmTitleOpEndStart
/*!
 *	タイトルオープニング 終了開始
 *
 *	@param	tcb	[in]	TCBワーク
 */
// ==========================================================================
void dmTitleOpEndStart(MTS_TASK_TCB *tcb)
{
	DMS_TITLEOP_MGR_WORK	*top_mgr_work;

	top_mgr_work = (DMS_TITLEOP_MGR_WORK*)mtTaskGetTcbWork(tcb);

	// 終了前処理
	dmTitleOpPreEnd(top_mgr_work);

	// 処理移行
	top_mgr_work->frame = 0;
	mtTaskChangeTcbProcedure(tcb, dmTitleOpPreEndWait);
}

// ==========================================================================
// dmTitleOpPreEnd
/*!
 *	タイトルオープニング 終了 前処理
 *
 *	@param	top_mgr_work	[in]	マネージャーワーク
 */
// ==========================================================================
void dmTitleOpPreEnd(DMS_TITLEOP_MGR_WORK *top_mgr_work)
{
	OBS_OBJECT_WORK	*obj_work;

	UNREFERENCED_PARAMETER(top_mgr_work);

	// 全オブジェクトppOutクリア
	// データ解放を行わないので、描画をとめて描画発行済みのものが終了するまで待つ
	obj_work = ObjObjectSearchRegistObject(NULL, 0xFFFF);
	while (obj_work) {
		obj_work->ppOut = NULL;
		obj_work = ObjObjectSearchRegistObject(obj_work, 0xFFFF);
	}
}

// ==========================================================================
// dmTitleOpPreEndWait
/*!
 *	タイトルオープニング 終了前処理待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmTitleOpPreEndWait(MTS_TASK_TCB *tcb)
{
	DMS_TITLEOP_MGR_WORK	*top_mgr_work;

	top_mgr_work = (DMS_TITLEOP_MGR_WORK*)mtTaskGetTcbWork(tcb);

	top_mgr_work->frame++;
	if (top_mgr_work->frame > 2) {
		// タイトルオープニング終了
		dmTitleOpEnd(top_mgr_work);
		// 終了処理待機へ
		mtTaskChangeTcbProcedure(tcb, dmTitleOpEndWait);
	}
}

// ==========================================================================
// dmTitleOpEnd
/*!
 *	タイトルオープニング 終了処理
 *
 *	@param	top_mgr_work	[in]	マネージャーワーク
 */
// ==========================================================================
void dmTitleOpEnd(DMS_TITLEOP_MGR_WORK *top_mgr_work)
{
	// アクション破棄
	dmTitleOpDeleteAction(top_mgr_work);

	// オブジェクトシステム前処理クリア
	g_obj.ppPre			= NULL;

	// 管理オブジェクト破棄
	ObjObjectClearAllObject();
	// オブジェクトシステム終了前処理
	ObjPreExit();
	// エフェクトシステム終了
//	MTM_ASSERT(ObjDrawESEffectSystemIsActive());
//	ObjDrawESEffectSystemExit();
	// オブジェクトシステム終了
	ObjExit();
}

// ==========================================================================
// dmTitleOpPreEndWait
/*!
 *	タイトルオープニング 終了前処理待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmTitleOpEndWait(MTS_TASK_TCB *tcb)
{
	if (!ObjObjectCheckClearAllObject()) {
		// オブジェクトの終了待ち
		return;
	}

	if (ObjIsExitWait()) {
		// オブジェクトシステムの終了待ち
		return;
	}

	// 終了処理終了
	// タスククリア
	mtTaskClearTcb(tcb);
}


// ==========================================================================
// 描画処理
// ==========================================================================
// ==========================================================================
// dmTitleOpPreDrawDT
/*!
 * 描画前処理
 *
 */
// ==========================================================================
void dmTitleOpPreDrawDT(void *data)
{
	UNREFERENCED_PARAMETER(data);

	// 画面をクリア
	amDrawSetBGColor((NNS_RGBA_U8*)&dm_titleop_clear_color);
}

// ==========================================================================
// dmTitleOpObjDraw
/*!
 * 描画処理
 *
 */
// ==========================================================================
void dmTitleOpObjDraw(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->obj_3d) {
		// 滝シェーダー設定
#if _IPHONE
		if (false) {
#elif !_WII
		if (obj_work->obj_3d->drawflag & NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1) {
#else
		if (obj_work->obj_3d->material_cb_func == dmTitleOpFallMaterialCallback) {
#endif
			ObjDraw3DNNUserFunc(dmTitleOpDrawFallShaderPreSettingUserFunc,
					NULL, 0, obj_work->obj_3d->command_state);
		}
	}

	ObjDrawActionSummary(obj_work);
}

// ==========================================================================
// dmTitleOpObjRockDraw
/*!
 * 岩描画処理
 *
 */
// ==========================================================================
void dmTitleOpObjRockDraw(OBS_OBJECT_WORK *obj_work)
{
	s32							i;
	DMS_TITLEOP_OBJ_3DNN_WORK	*top3d_work;

	top3d_work = (DMS_TITLEOP_OBJ_3DNN_WORK*)obj_work;

	for (i = 0; i < *top3d_work->rock_setting_num; i++) {
		obj_work->pos	= (top3d_work->rock_setting + i)->pos;
		obj_work->pos.x += dm_titleop_scrl_x_ofst;

		// ループチェック
		if (obj_work->pos.x < -(DMD_TITLEOP_MAP_SCRL_LOOP_DIST - DMD_TITLEOP_ROCK_DISP_CLIP_DIST)) {
			obj_work->pos.x += DMD_TITLEOP_MAP_SCRL_LOOP_DIST;
		}
		else if (obj_work->pos.x > (DMD_TITLEOP_MAP_SCRL_LOOP_DIST - DMD_TITLEOP_ROCK_DISP_CLIP_DIST)) {
			obj_work->pos.x -= DMD_TITLEOP_MAP_SCRL_LOOP_DIST;
		}

		// クリッピングチェック
		if (obj_work->pos.x < -DMD_TITLEOP_ROCK_DISP_CLIP_DIST ||
				obj_work->pos.x > DMD_TITLEOP_ROCK_DISP_CLIP_DIST) {
			continue;
		}

		if (obj_work->obj_3d) {
			// 滝シェーダー設定
#if _IPHONE
			if (false) {
#elif !_WII
			if (obj_work->obj_3d->drawflag & NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1) {
#else
			if (obj_work->obj_3d->material_cb_func == dmTitleOpFallMaterialCallback) {
#endif
				ObjDraw3DNNUserFunc(dmTitleOpDrawFallShaderPreSettingUserFunc,
						NULL, 0, obj_work->obj_3d->command_state);
			}
		}

		obj_work->scale	= (top3d_work->rock_setting + i)->scale;
		ObjDrawActionSummary(obj_work);
	}
}

#if _WII
// ==========================================================================
// dmTitleOpFallMaterialCallback
/*!
 *	滝シェーダ用 マテリアルコールバック(OBF_MATERIAL_CB)
 *
 *	@param	val		[in]	ドローコールバック変数
 *	@param	param	[in]	ユーザーパラメータ
 */
// ==========================================================================
NNE_BOOL dmTitleOpFallMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param)
{
	UNREFERENCED_PARAMETER(param);
	return (amDrawWaterFallMaterial(val));
}
#endif	//_WII

// ==========================================================================
// dmTitleOpFallShaderPreRenderUserFunc
/*!
 *	滝シェーダー レンダリング
 */
// ==========================================================================
void dmTitleOpFallShaderPreRenderUserFunc(void *data)
{
	AMS_RENDER_TARGET	*render_target;
	NNS_RGBA_U8			color = {0x00, 0x00, 0x00, 0xFF};

	UNREFERENCED_PARAMETER( data );

	// レンダターゲットを取得
	render_target = _am_render_manager.targetp;
	amAssert(render_target);

	if ( render_target == &_gm_mapFar_render_work ){
		render_target = &_am_draw_target;
	}
	else {
		render_target = &_gm_mapFar_render_work;
	}
	// レンダターゲットが作成されていない
	if (render_target->width == 0 ) {
		return;
	}

	// レンダリングターゲットのコピー
	amRenderCopyTarget(render_target, &color);
}

// ==========================================================================
// dmTitleOpDrawFallShaderPreSettingUserFunc
/*!
 *	滝シェーダーモデル 描画時設定
 */
// ==========================================================================
void dmTitleOpDrawFallShaderPreSettingUserFunc(void *data)
{
	NNS_MATRIX44		*proj_mtx;
	AMS_RENDER_TARGET	*render_target;

	UNREFERENCED_PARAMETER(data);

	// レンダターゲットを取得
	render_target = _am_render_manager.targetp;
	amAssert(render_target);

	if ( render_target == &_gm_mapFar_render_work ){
		render_target = &_am_draw_target;
	}
	else {
		render_target = &_gm_mapFar_render_work;
	}
	// レンダターゲットが作成されていない
	if (render_target->width == 0 ) {
		return;
	}

	proj_mtx = amDrawGetProjectionMatrix();

#if _WII
	if (proj_mtx != NULL) {
		memcpy(_am_draw_fall_projmtx, proj_mtx, sizeof(NNS_MATRIX44));
	}
#endif	//_WII

	// テクスチャ設定
#if _PC | _XBOX
	{
		Uint32	state[NNE_SAMPLERSTATETYPE_MAX];
		NNS_MATRIX44	mtx;
		nnMakeUnitMatrix(&mtx);
		nnInitMaterialControlUserSamplerDXG20();
		nnGetMaterialControlUserSamplerDefaultStateDXG20(state);
		state[NNE_SAMPLERSTATETYPE_ADDRESSU]	= D3DTADDRESS_CLAMP;
		state[NNE_SAMPLERSTATETYPE_ADDRESSV]	= D3DTADDRESS_CLAMP;
		state[NNE_SAMPLERSTATETYPE_ADDRESSW]	= D3DTADDRESS_CLAMP;
		state[NNE_SAMPLERSTATETYPE_MAGFILTER]	= D3DTEXF_LINEAR;
		state[NNE_SAMPLERSTATETYPE_MINFILTER]	= D3DTEXF_LINEAR;
		state[NNE_SAMPLERSTATETYPE_MIPFILTER]	= D3DTEXF_NONE;
		nnSetMaterialControlUserSamplerDXG20(NNE_USER_SAMPLER_2D_1,
				render_target->texture_color[0], &mtx, state);

		nnSetUserUniformDXG20(0,
				NNM_MTX(*proj_mtx, 0, 0), NNM_MTX(*proj_mtx, 1, 0),
				NNM_MTX(*proj_mtx, 2, 0), NNM_MTX(*proj_mtx, 3, 0));
		nnSetUserUniformDXG20(1,
				NNM_MTX(*proj_mtx, 0, 1), NNM_MTX(*proj_mtx, 1, 1),
				NNM_MTX(*proj_mtx, 2, 1), NNM_MTX(*proj_mtx, 3, 1));
		nnSetUserUniformDXG20(2,
				NNM_MTX(*proj_mtx, 0, 2), NNM_MTX(*proj_mtx, 1, 2),
				NNM_MTX(*proj_mtx, 2, 2), NNM_MTX(*proj_mtx, 3, 2));
		nnSetUserUniformDXG20(3,
				NNM_MTX(*proj_mtx, 0, 3), NNM_MTX(*proj_mtx, 1, 3),
				NNM_MTX(*proj_mtx, 2, 3), NNM_MTX(*proj_mtx, 3, 3));
	}
#elif _PS3
	{

		Uint32	state[NNE_SAMPLERSTATETYPE_MAX];
		NNS_MATRIX44	mtx;

		nnMakeUnitMatrix(&mtx);
		nnInitMaterialControlUserSamplerPS3();
		nnGetMaterialControlUserSamplerDefaultStatePS3(state);
		state[NNE_SAMPLERSTATETYPE_ADDRESSU]	= CELL_GCM_TEXTURE_CLAMP_TO_EDGE;
		state[NNE_SAMPLERSTATETYPE_ADDRESSV]	= CELL_GCM_TEXTURE_CLAMP_TO_EDGE;
		state[NNE_SAMPLERSTATETYPE_ADDRESSW]	= CELL_GCM_TEXTURE_CLAMP_TO_EDGE;
		state[NNE_SAMPLERSTATETYPE_MAGFILTER]	= CELL_GCM_TEXTURE_LINEAR;
		state[NNE_SAMPLERSTATETYPE_MINFILTER]	= CELL_GCM_TEXTURE_LINEAR;
		state[NNE_SAMPLERSTATETYPE_MIPFILTER]	= CELL_GCM_TEXTURE_NEAREST;
		nnSetMaterialControlUserSamplerPS3(NNE_USER_SAMPLER_2D_1,
				render_target->texture_color[0], &mtx, state);

		nnSetUserUniformPS3(0,
				NNM_MTX(*proj_mtx, 0, 0), NNM_MTX(*proj_mtx, 1, 0),
				NNM_MTX(*proj_mtx, 2, 0), NNM_MTX(*proj_mtx, 3, 0));
		nnSetUserUniformPS3(1,
				NNM_MTX(*proj_mtx, 0, 1), NNM_MTX(*proj_mtx, 1, 1),
				NNM_MTX(*proj_mtx, 2, 1), NNM_MTX(*proj_mtx, 3, 1));
		nnSetUserUniformPS3(2,
				NNM_MTX(*proj_mtx, 0, 2), NNM_MTX(*proj_mtx, 1, 2),
				NNM_MTX(*proj_mtx, 2, 2), NNM_MTX(*proj_mtx, 3, 2));
		nnSetUserUniformPS3(3,
				NNM_MTX(*proj_mtx, 0, 3), NNM_MTX(*proj_mtx, 1, 3),
				NNM_MTX(*proj_mtx, 2, 3), NNM_MTX(*proj_mtx, 3, 3));
	}
#elif _WII
	{
		memcpy(&_am_draw_render_work, render_target, sizeof(AMS_RENDER_TARGET));
	}
#endif
}




// ==========================================================================
// カメラ処理
// ==========================================================================
// ==========================================================================
// dmTitleOpCamera
/*!
 *	カメラ処理
 *
 *	@param	camera	[in]	カメラワーク
 */
// ==========================================================================
void dmTitleOpCamera(OBS_CAMERA *camera)
{
	//UNREFERENCED_PARAMETER(camera);

#if DMD_TITLEOP_DEBUG_TEST_CAMERA
	if (AoPadDirect() & KEY_R_UP) {
		test_disp_pos.z -= 1.f;
	}
	else if (AoPadDirect() & KEY_R_LEFT) {
		test_disp_pos.z += 1.f;
	}
	if (AoPadDirect() & KEY_L_LEFT) {
		test_disp_pos.x -= 1.f;
	}
	else if (AoPadDirect() & KEY_L_RIGHT) {
		test_disp_pos.x += 1.f;
	}
	if (AoPadDirect() & KEY_L_DOWN) {
		test_disp_pos.y -= 1.f;
	}
	else if (AoPadDirect() & KEY_L_UP) {
		test_disp_pos.y += 1.f;
	}

	if (AoPadStand() & KEY_SELECT) {
		test_disp_pos.x = 0.f;
		test_disp_pos.y = 5.f;
		test_disp_pos.z = 160.f;
	}

	camera->disp_pos = test_disp_pos;

	camera->target_pos.x = 0.f;
	camera->target_pos.y = 0.f;
	camera->target_pos.z = 0.f;

#else
	camera->disp_pos.x = 0.f;
#if DMD_TITLEOP_DEBUG_TEST_ROCK_EDIT
	camera->disp_pos.y = 5.f + dm_titleop_debug_test_cam_ofst_y;
#else
	camera->disp_pos.y = 5.f;
#endif
	camera->disp_pos.z = 160.f;

	camera->target_pos.x = 0.f;
#if DMD_TITLEOP_DEBUG_TEST_ROCK_EDIT
	camera->target_pos.y = 0.f + dm_titleop_debug_test_cam_ofst_y;
#else
	camera->target_pos.y = 0.f;
#endif
	camera->target_pos.z = 0.f;
#endif
}


// ==========================================================================
// デバック処理
// ==========================================================================
#if DMD_TITLEOP_DEBUG_TEST_ROCK_EDIT
// ==========================================================================
// dmTitleOpDebugTestRockEdit
/*!
 *	岩配置編集 (デバック処理)
 *	
 *	@param	top_mgr_work	[in]
 */
// ==========================================================================
void dmTitleOpDebugTestRockEdit(DMS_TITLEOP_MGR_WORK *top_mgr_work)
{
	{
		s32	y;
		s32	type, no;
		s32	i;

		// モードセレクト
		if (AoPadSomeoneStand(KEY_SELECT) >= 0) {
			dm_titleop_debug_rock_edit_mode++;
			if (dm_titleop_debug_rock_edit_mode >= DMD_TITLEOP_DEBUG_ROCK_EDIT_MODE_MAX) {
				dm_titleop_debug_rock_edit_mode = 0;
			}

			if (dm_titleop_debug_rock_edit_mode == DMD_TITLEOP_DEBUG_ROCK_EDIT_MODE_ACT) {
				// 演出設定
				top_mgr_work->flag |= DMD_TITLEOP_FLAG_SCRL;
			}
			else {
				top_mgr_work->flag &= ~DMD_TITLEOP_FLAG_SCRL;
			}
		}

		// デバック表示ON・OFF
	//	if (AoPadSomeoneStand(KEY_SELECT) >= 0) {
	//		dm_titleop_debug_test_rock_edit_disp ^= 0x01;
	//	}
	//	// カメラ位置リセット
		if (AoPadSomeoneStand(KEY_R_RIGHT) >= 0) {
			dm_titleop_scrl_x_ofst = 0;
			dm_titleop_debug_test_cam_ofst_y = 0.0f;
		}
		// ロゴON・OFF
		if (AoPadSomeoneStand(KEY_START) >= 0) {
			top_mgr_work->flag ^= (DMD_TITLEOP_FLAG_ACT_DISP_LOGO_1 | DMD_TITLEOP_FLAG_ACT_DISP_FINGER |
						DMD_TITLEOP_FLAG_ACT_DISP_LOGO_2 | DMD_TITLEOP_FLAG_ACT_DISP_RIGHT);
#if SONIC4_TRIAL
			top_mgr_work->flag ^= (DMD_TITLEOP_FLAG_ACT_DISP_FREE | DMD_TITLEOP_FLAG_ACT_DISP_FREE_RED);
#endif //SONIC4_TRIAL
		}

		// 入力
		switch (dm_titleop_debug_rock_edit_mode) {
		case DMD_TITLEOP_DEBUG_ROCK_EDIT_MODE_EDIT:
			if (AoPadSomeoneRepeat(KEY_L_UP) >= 0) {
				dm_titleop_debug_test_rock_edit_item_sel = 0;
				dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode]--;
				if (dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] < 0) {
					dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] = DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_MAX-1;
				}
			}
			else if (AoPadSomeoneRepeat(KEY_L_DOWN) >= 0) {
				dm_titleop_debug_test_rock_edit_item_sel = 0;
				dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode]++;
				if (dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] >= DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_MAX) {
					dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] = 0;
				}
			}

			if (DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_NUM <= dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] &&
					dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] <= DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE5) {
				type = 0;
			}
			else if (DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_NUM <= dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] &&
					dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] <= DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_STATE5) {
				type = 1;
			}
			else {
				type = 2;
			}

			for (i = 0; i < DMD_TITLEOP_ROCK_SETTING_NUM; i++) {
				if (DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE1 + i == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ||
					DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_STATE1 + i == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ||
					DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_STATE1 + i == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode]) {
						no = i;
						break;
				}
			}
#if 0
			if (DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE1 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ||
					DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_STATE1 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ||
					DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_STATE1 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode]) {
				no = 0;
			}
			else if (DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE2 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ||
					DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_STATE2 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ||
					DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_STATE2 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode]) {
				no = 1;
			}
			else if (DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE3 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ||
					DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_STATE3 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ||
					DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_STATE3 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode]) {
				no = 2;
			}
			else if (DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE4 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ||
					DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_STATE4 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ||
					DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_STATE4 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode]) {
				no = 3;
			}
			else if (DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE5 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ||
					DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_STATE5 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ||
					DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_STATE5 == dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode]) {
				no = 3;
			}
			else {
				no = 4;
			}
#endif
			switch (dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode]) {
			case DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_NUM:
			case DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_NUM:
			case DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_NUM:
				if (AoPadSomeoneStand(KEY_L_LEFT) >= 0) {
					dm_titleop_rock_setting_num[type]--;
					if (dm_titleop_rock_setting_num[type] < 0) {
						dm_titleop_rock_setting_num[type] = 0;
					}
				}
				else if (AoPadSomeoneStand(KEY_L_RIGHT) >= 0) {
					dm_titleop_rock_setting_num[type]++;
					if (dm_titleop_rock_setting_num[type] > DMD_TITLEOP_ROCK_SETTING_NUM) {
						dm_titleop_rock_setting_num[type] = DMD_TITLEOP_ROCK_SETTING_NUM;
					}
				}
				break;
			default:
			//case DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE1 ～ DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_STATE*:
				if (AoPadSomeoneStand(KEY_R_UP) >= 0) {
					dm_titleop_debug_test_rock_edit_item_sel++;
					if (dm_titleop_debug_test_rock_edit_item_sel >= 3) {
						dm_titleop_debug_test_rock_edit_item_sel = 0;
					}
				}

				switch (dm_titleop_debug_test_rock_edit_item_sel) {
				case 0:	// POSX
					if (AoPadSomeoneRepeat(KEY_L_LEFT) >= 0) {
						if (AoPadSomeoneDirect(KEY_R1) >= 0) {
							dm_titleop_rock_setting[type][no].pos.x -= 0x8000;
						}
						else if (AoPadSomeoneDirect(KEY_L1) >= 0) {
							dm_titleop_rock_setting[type][no].pos.x -= 0x80000;
						}
						else {
							dm_titleop_rock_setting[type][no].pos.x -= 0x800;
						}
					}
					else if (AoPadSomeoneRepeat(KEY_L_RIGHT) >= 0) {
						if (AoPadSomeoneDirect(KEY_R1) >= 0) {
							dm_titleop_rock_setting[type][no].pos.x += 0x8000;
						}
						else if (AoPadSomeoneDirect(KEY_L1) >= 0) {
							dm_titleop_rock_setting[type][no].pos.x += 0x80000;
						}
						else {
							dm_titleop_rock_setting[type][no].pos.x += 0x800;
						}
					}
					break;
				case 1:	// POSZ
					if (AoPadSomeoneRepeat(KEY_L_LEFT) >= 0) {
						if (AoPadSomeoneDirect(KEY_R1) >= 0) {
							dm_titleop_rock_setting[type][no].pos.z -= 0x8000;
						}
						else if (AoPadSomeoneDirect(KEY_L1) >= 0) {
							dm_titleop_rock_setting[type][no].pos.z -= 0x80000;
						}
						else {
							dm_titleop_rock_setting[type][no].pos.z -= 0x800;
						}
					}
					else if (AoPadSomeoneRepeat(KEY_L_RIGHT) >= 0) {
						if (AoPadSomeoneDirect(KEY_R1) >= 0) {
							dm_titleop_rock_setting[type][no].pos.z += 0x8000;
						}
						else if (AoPadSomeoneDirect(KEY_L1) >= 0) {
							dm_titleop_rock_setting[type][no].pos.z += 0x80000;
						}
						else {
							dm_titleop_rock_setting[type][no].pos.z += 0x800;
						}
					}
					break;
				case 2: // SCALE
					if (AoPadSomeoneRepeat(KEY_L_LEFT) >= 0) {
						if (AoPadSomeoneDirect(KEY_R1) >= 0) {
							dm_titleop_rock_setting[type][no].scale.x -= 0x80;
						}
						else if (AoPadSomeoneDirect(KEY_L1) >= 0) {
							dm_titleop_rock_setting[type][no].scale.x -= 0x800;
						}
						else {
							dm_titleop_rock_setting[type][no].scale.x -= 0x10;
						}
						if (dm_titleop_rock_setting[type][no].scale.x < 0x10) {
							dm_titleop_rock_setting[type][no].scale.x = 0x10;
						}
					}
					else if (AoPadSomeoneRepeat(KEY_L_RIGHT) >= 0) {
						if (AoPadSomeoneDirect(KEY_R1) >= 0) {
							dm_titleop_rock_setting[type][no].scale.x += 0x80;
						}
						else if (AoPadSomeoneDirect(KEY_L1) >= 0) {
							dm_titleop_rock_setting[type][no].scale.x += 0x800;
						}
						else {
							dm_titleop_rock_setting[type][no].scale.x += 0x10;
						}
					}
					dm_titleop_rock_setting[type][no].scale.y =
						dm_titleop_rock_setting[type][no].scale.z =
						dm_titleop_rock_setting[type][no].scale.x;
					break;
				}
			}
			break;
		case DMD_TITLEOP_DEBUG_ROCK_EDIT_MODE_CAMERA:
			if (AoPadSomeoneRepeat(KEY_L_LEFT) >= 0) {
				if (AoPadSomeoneDirect(KEY_R1) >= 0) {
					dm_titleop_scrl_x_ofst -= 0x800;
				}
				else if (AoPadSomeoneDirect(KEY_L1) >= 0) {
					dm_titleop_scrl_x_ofst -= 0x8000;
				}
				else {
					dm_titleop_scrl_x_ofst -= 0x80;
				}
			}
			else if (AoPadSomeoneRepeat(KEY_L_RIGHT) >= 0) {
				if (AoPadSomeoneDirect(KEY_R1) >= 0) {
					dm_titleop_scrl_x_ofst += 0x800;
				}
				else if (AoPadSomeoneDirect(KEY_L1) >= 0) {
					dm_titleop_scrl_x_ofst += 0x8000;
				}
				else {
					dm_titleop_scrl_x_ofst += 0x80;
				}
			}
			{
				s32 div = dm_titleop_scrl_x_ofst / DMD_TITLEOP_MAP_SCRL_LOOP_DIST;
				dm_titleop_scrl_x_ofst -= div * DMD_TITLEOP_MAP_SCRL_LOOP_DIST;
			}
			if (AoPadSomeoneRepeat(KEY_L_DOWN) >= 0) {
				if (AoPadSomeoneDirect(KEY_R1) >= 0) {
					dm_titleop_debug_test_cam_ofst_y -= 0.1f;
				}
				else if (AoPadSomeoneDirect(KEY_L1) >= 0) {
					dm_titleop_debug_test_cam_ofst_y -= 0.5f;
				}
				else {
					dm_titleop_debug_test_cam_ofst_y -= 0.05f;
				}
			}
			else if (AoPadSomeoneRepeat(KEY_L_UP) >= 0) {
				if (AoPadSomeoneDirect(KEY_R1) >= 0) {
					dm_titleop_debug_test_cam_ofst_y += 0.1f;
				}
				else if (AoPadSomeoneDirect(KEY_L1) >= 0) {
					dm_titleop_debug_test_cam_ofst_y += 0.5f;
				}
				else {
					dm_titleop_debug_test_cam_ofst_y += 0.05f;
				}
			}
			break;
		case DMD_TITLEOP_DEBUG_ROCK_EDIT_MODE_ACT:
			break;
		}

		// 表示
		y = 2;
		if (dm_titleop_debug_test_rock_edit_disp) {
			switch (dm_titleop_debug_rock_edit_mode) {
			case DMD_TITLEOP_DEBUG_ROCK_EDIT_MODE_EDIT:
				// ROCK A
				amPrintf(2, y, "ROCK A");
				y++;
				amPrintf(2, y, "USE NUM:%d", dm_titleop_rock_setting_num[0]);	// 使用数
				// カーソルチェック
				if (dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] == DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_NUM) {
					// カーソル
					amPrintf(1, y, ">");
				}
				y++;
				for (i = 0; i < (s32)dm_titleop_rock_setting_num[0]; i++) {			// ステート
					if (dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ==
							DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_A_STATE1 + i) {
						// カーソル
						amPrintf(1, y, ">");
						switch (dm_titleop_debug_test_rock_edit_item_sel) {
						default:
						case 0:
							amPrintf(2, y, ">POS_X:%x  POS_Z %x  SCALE %x",
									dm_titleop_rock_setting[0][i].pos.x,
									dm_titleop_rock_setting[0][i].pos.z,
									dm_titleop_rock_setting[0][i].scale.x);
							break;
						case 1:
							amPrintf(2, y, " POS_X:%x >POS_Z %x  SCALE %x",
									dm_titleop_rock_setting[0][i].pos.x,
									dm_titleop_rock_setting[0][i].pos.z,
									dm_titleop_rock_setting[0][i].scale.x);
							break;
						case 2:
							amPrintf(2, y, " POS_X:%x  POS_Z %x >SCALE %x",
									dm_titleop_rock_setting[0][i].pos.x,
									dm_titleop_rock_setting[0][i].pos.z,
									dm_titleop_rock_setting[0][i].scale.x);
							break;
						}
					}
					else {
						amPrintf(2, y, " POS_X:%x  POS_Z %x  SCALE %x",
								dm_titleop_rock_setting[0][i].pos.x,
								dm_titleop_rock_setting[0][i].pos.z,
								dm_titleop_rock_setting[0][i].scale.x);
					}
					y++;
				}
				y += DMD_TITLEOP_ROCK_SETTING_NUM - dm_titleop_rock_setting_num[0];

				// ROCK B
				amPrintf(2, y, "ROCK B");
				y++;
				amPrintf(2, y, "USE NUM:%d", dm_titleop_rock_setting_num[1]);
				// カーソルチェック
				if (dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] == DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_NUM) {
					// カーソル
					amPrintf(1, y, ">");
				}
				y++;
				for (i = 0; i < (s32)dm_titleop_rock_setting_num[1]; i++) {			// ステート
					if (dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ==
							DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_B_STATE1 + i) {
						// カーソル
						amPrintf(1, y, ">");
						switch (dm_titleop_debug_test_rock_edit_item_sel) {
						default:
						case 0:
							amPrintf(2, y, ">POS_X:%x  POS_Z %x  SCALE %x",
									dm_titleop_rock_setting[1][i].pos.x,
									dm_titleop_rock_setting[1][i].pos.z,
									dm_titleop_rock_setting[1][i].scale.x);
							break;
						case 1:
							amPrintf(2, y, " POS_X:%x >POS_Z %x  SCALE %x",
									dm_titleop_rock_setting[1][i].pos.x,
									dm_titleop_rock_setting[1][i].pos.z,
									dm_titleop_rock_setting[1][i].scale.x);
							break;
						case 2:
							amPrintf(2, y, " POS_X:%x  POS_Z %x >SCALE %x",
									dm_titleop_rock_setting[1][i].pos.x,
									dm_titleop_rock_setting[1][i].pos.z,
									dm_titleop_rock_setting[1][i].scale.x);
							break;
						}
					}
					else {
						amPrintf(2, y, " POS_X:%x  POS_Z %x >SCALE %x",
								dm_titleop_rock_setting[1][i].pos.x,
								dm_titleop_rock_setting[1][i].pos.z,
								dm_titleop_rock_setting[1][i].scale.x);
					}
					y++;
				}
				y += DMD_TITLEOP_ROCK_SETTING_NUM - dm_titleop_rock_setting_num[1];

				// ROCK C
				amPrintf(2, y, "ROCK C");
				y++;
				amPrintf(2, y, "USE NUM:%d", dm_titleop_rock_setting_num[2]);
				// カーソルチェック
				if (dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] == DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_NUM) {
					// カーソル
					amPrintf(1, y, ">");
				}
				y++;
				for (i = 0; i < (s32)dm_titleop_rock_setting_num[2]; i++) {			// ステート
					if (dm_titleop_debug_test_rock_edit_cursol[dm_titleop_debug_rock_edit_mode] ==
							DMD_TITLEOP_DEBUG_ROCK_EDIT_CURSOL_EDIT_C_STATE1 + i) {
						// カーソル
						amPrintf(1, y, ">");
						switch (dm_titleop_debug_test_rock_edit_item_sel) {
						default:
						case 0:
							amPrintf(2, y, ">POS_X:%x  POS_Z %x  SCALE %x",
									dm_titleop_rock_setting[2][i].pos.x,
									dm_titleop_rock_setting[2][i].pos.z,
									dm_titleop_rock_setting[2][i].scale.x);
							break;
						case 1:
							amPrintf(2, y, " POS_X:%x >POS_Z %d  SCALE %x",
									dm_titleop_rock_setting[2][i].pos.x,
									dm_titleop_rock_setting[2][i].pos.z,
									dm_titleop_rock_setting[2][i].scale.x);
							break;
						case 2:
							amPrintf(2, y, " POS_X:%x  POS_Z %x >SCALE %x",
									dm_titleop_rock_setting[2][i].pos.x,
									dm_titleop_rock_setting[2][i].pos.z,
									dm_titleop_rock_setting[2][i].scale.x);
							break;
						}
					}
					else {
						amPrintf(2, y, " POS_X:%x  POS_Z %x  SCALE %x",
								dm_titleop_rock_setting[2][i].pos.x,
								dm_titleop_rock_setting[2][i].pos.z,
								dm_titleop_rock_setting[2][i].scale.x);
					}
					y++;
				}
				y += DMD_TITLEOP_ROCK_SETTING_NUM - dm_titleop_rock_setting_num[2];
				break;

			case DMD_TITLEOP_DEBUG_ROCK_EDIT_MODE_CAMERA:
				amPrintf(2, y, "CAMERA X %x", dm_titleop_scrl_x_ofst);
				y++;
				amPrintf(2, y, "CAMERA Y %f", dm_titleop_debug_test_cam_ofst_y);
				break;

			case DMD_TITLEOP_DEBUG_ROCK_EDIT_MODE_ACT:
				amPrintf(2, y, "RUN");
				break;
			}
		}
	}
}

#endif	// #if DMD_TITLEOP_DEBUG_TEST_ROCK_EDIT


// ==========================================================================
// dmTitleOpTestTaskDraw
/*!
 *	デバック用
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
#if DMD_TITLEOP_DEBUG_TEST
void dmTitleOpTestTaskDraw(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	AoActDrawPre();
	amDrawExecCommand(10);
	amDrawEndScene();
}
#endif

// ==========================================================================
// DmTitleOpStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void DmTitleOpStaticVarInit(void)
{
	dm_titleop_load_tcb = NULL;			//!< データロードTCB
	dm_titleop_build_tcb = NULL;		//!< データビルドTCB
	dm_titleop_flush_tcb = NULL;		//!< データフラッシュTCB
	//dm_titleop_release_tcb = NULL;	//!< データリリースTCB
	
	dm_titleop_mgr_tcb = NULL;			//!< メイン処理TCB
	
	dm_titleop_build_state = FALSE;		//!< ビルドステータス
	
	// データ
	memset(dm_titleop_data, 0, sizeof(dm_titleop_data));
	memset(dm_titleop_mapfar_data, 0, sizeof(dm_titleop_mapfar_data));
	
	// テクスチャ
	dm_titleop_aos_tex = NULL;	//!< テクスチャ
	
	// 描画オブジェクト
	dm_titleop_obj_3d_list = NULL;
	dm_titleop_water_obj_3d_list = NULL;
	
	// 画面スクロール
	dm_titleop_scrl_x_ofst = 0.0f;
	
	/* 以下デバッグ */
#if defined (MTD_DEBUG)
	
#if DMD_TITLEOP_DEBUG_TEST_CAMERA
	static const NNS_VECTOR	sc_test_disp_pos = {0, 5, 160};
	memcpy(&test_disp_pos, &sc_test_disp_pos, sizeof(test_disp_pos));
#endif
	
#if DMD_TITLEOP_DEBUG_TEST_FOG
	fog_near = 1.0f;
	fog_far = 500.0f;
#endif
	
#if DMD_TITLEOP_DEBUG_TEST_ROCK_EDIT
	dm_titleop_debug_rock_edit_mode = 0;
	memset(dm_titleop_debug_test_rock_edit_cursol, 0, sizeof(dm_titleop_debug_test_rock_edit_cursol));
	dm_titleop_debug_test_rock_edit_item_sel = 0;
	dm_titleop_debug_test_rock_edit_disp = TRUE;	// デバッグ表示
	dm_titleop_debug_test_cam_ofst_y = 0.0f;
#endif // #if DMD_TITLEOP_DEBUG_TEST_ROCK_EDIT
#endif // #if defined (MTD_DEBUG)
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
