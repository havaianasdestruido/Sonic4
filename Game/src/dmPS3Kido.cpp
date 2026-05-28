// ==========================================================================
/*!
  @file dmPS3Kido.cpp
  @brief PS3起動時表示

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: dmPS3Kido.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"

#if (defined(HOG_RGN_JP) && _PS3) || _PC
#include "gs.h"
#include "gsMainSys.h"
#include "gsEnvironment.h"
#include "izFade.h"

#include "dmLogoCom.h"

#include "dmPS3Kido.h"

// データヘッダ
#include "PS3/arc/D_PS3_KIDO.HMB"
#include "PS3/ace/D_PS3_KIDO.HMA"

//----- Definitions ---------------------------------------------------------
/* タスク設定 */
#define DMD_PS3_KIDO_TASK_PRIO_DATA_LOAD		(0x1000)		//!< ロードタスクプライオリティ
#define DMD_PS3_KIDO_TASK_GROUP_DATA_LOAD		(0)				//!< ロードタスクグループ
#define DMD_PS3_KIDO_TASK_PRIO_DATA_BUILD		(0x1000)		//!< ビルドタスクプライオリティ
#define DMD_PS3_KIDO_TASK_GROUP_DATA_BUILD		(0)				//!< ビルドタスクグループ
#define DMD_PS3_KIDO_TASK_PRIO_DATA_FLUSH		(0x1000)		//!< フラッシュタスクプライオリティ
#define DMD_PS3_KIDO_TASK_GROUP_DATA_FLUSH		(0)				//!< フラッシュタスクグループ
#define DMD_PS3_KIDO_TASK_PRIO_DATA_RELEASE		(0x1000)		//!< リリースタスクプライオリティ
#define DMD_PS3_KIDO_TASK_GROUP_DATA_RELEASE	(0)				//!< リリースタスクグループ

#define DMD_PS3_KIDO_TASK_PRIO_MAIN				(0x1000)		//!< メイン処理プライオリティ
#define DMD_PS3_KIDO_TASK_GROUP_MAIN			(0)				//!< メイン処理グループ

/* 演出設定 */
#define DMD_PS3_KIDO_FADEIN_TIME			(30)		//!< フェードイン時間
#define DMD_PS3_KIDO_DISP_TIME				(300)		//!< ロゴ表示時間
#define DMD_PS3_KIDO_FADEOUT_TIME			(30)		//!< フェードアウト時間

#define DMD_PS3_KIDO_SKIP_KEY				(GSD_KEY_DECIDE)	//!< スキップキー

/* データ設定 */
/// 読み込みデータ
enum {
//	DMD_PS3_KIDO_DATA_COM_AMA_SET_AMB	= 0,		//!< 共通 AMAデータセットAMB

	DMD_PS3_KIDO_DATA_LOCAL_AMA_SET_AMB	= 0,	//!< ローカライズ AMAデータセットAMB

	DMD_PS3_KIDO_DATA_MAX
};

/// アクションデータ
enum {
	DMD_PS3_KIDO_ACT_BASE	= 0,

	DMD_PS3_KIDO_ACT_MAX
};

/// AOSテクスチャタイプ
enum {
	DMD_PS3_KIDO_AOSTEX_LOCAL	= 0,

	DMD_PS3_KIDO_AOSTEX_MAX
};

/// PS3起動時表示ワーク
typedef struct tag_DMS_PS3_KIDO_WORK {
	u32				flag;
	s32				timer;

//	AOS_TEXTURE		aos_tex[DMD_PS3_KIDO_AOSTEX_MAX];	//!< テクスチャ
	AOS_ACTION		*act[DMD_PS3_KIDO_ACT_MAX];		//!< アクション

	void (*func)(struct tag_DMS_PS3_KIDO_WORK*);

} DMS_PS3_KIDO_WORK;

//#define DMD_PS3_KIDO_FLAG_SKIP_OK		(0x00000001)	//!< スキップ許可フラグ
//#define DMD_PS3_KIDO_FLAG_SKIP			(0x00000002)	//!< スキップ実行済み
#define DMD_PS3_KIDO_FLAG_END			(0x00000004)	//!< 演出終了

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void dmPS3KidoLoadWait(MTS_TASK_TCB *tcb);
static void dmPS3KidoBuildWait(MTS_TASK_TCB *tcb);

static void dmPS3KidoActionCreate(DMS_PS3_KIDO_WORK *logo_work);
static void dmPS3KidoActionDelete(DMS_PS3_KIDO_WORK *logo_work);

static void dmPS3KidoStart(void);
static void dmPS3KidoMainFunc(MTS_TASK_TCB *tcb);
static void dmPS3KidoFadeInWaitFunc(DMS_PS3_KIDO_WORK *logo_work);
static void dmPS3KidoDispWaitFunc(DMS_PS3_KIDO_WORK *logo_work);
static void dmPS3KidoFadeOutWaitFunc(DMS_PS3_KIDO_WORK *logo_work);

static void dmPS3KidoPreEndWait(MTS_TASK_TCB *tcb);
static void dmPS3KidoFlushWaitFunc(MTS_TASK_TCB *tcb);
static void dmPS3KidoRelesehWaitFunc(MTS_TASK_TCB *tcb);

static void dmPS3KidoLoadPostFunc(DMS_LOGO_COM_LOAD_CONTEXT *context);
static void dmPS3KidoDataBuildMain(MTS_TASK_TCB *tcb);
static void dmPS3KidoDataBuildDest(MTS_TASK_TCB *tcb);
static void dmPS3KidoDataFlushMain(MTS_TASK_TCB *tcb);
static void dmPS3KidoDataFlushDest(MTS_TASK_TCB *tcb);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static MTS_TASK_TCB *dm_ps3_kido_load_tcb = NULL;		//!< データロードTCB
static MTS_TASK_TCB *dm_ps3_kido_build_tcb = NULL;		//!< データビルドTCB
static MTS_TASK_TCB *dm_ps3_kido_flush_tcb = NULL;		//!< データフラッシュTCB
//static MTS_TASK_TCB *dm_ps3_kido_release_tcb = NULL;	//!< データリリースTCB

static AOS_TEXTURE *dm_ps3_kido_aos_tex = NULL;	//!< テクスチャ
static BOOL dm_ps3_kido_build_state = FALSE;		//!< ビルド状況

/// データ
void *dm_ps3_kido_data[DMD_PS3_KIDO_DATA_MAX] = {NULL};

#if 0
/// 共通データファイル名リスト
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_ps3_kido_com_fileinfo_list[] = {
	{GSS_BASE_PATH "DEMO/PS3_KIDO/D_PS3_KIDO.AMB", dmPS3KidoLoadPostFunc},
	// 以下その他
};

/// データファイル数
static const s32 dm_ps3_kido_com_file_num = sizeof(dm_ps3_kido_com_fileinfo_list) / sizeof(DMS_LOGO_COM_LOAD_FILE_INFO);
#endif

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
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_ps3_kido_localize_fileinfo_list_jp[] = {
	{GSS_BASE_PATH "DEMO/PS3_KIDO/D_PS3_KIDO_JP.AMB", dmPS3KidoLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト 英語
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_ps3_kido_localize_fileinfo_list_us[] = {
	{GSS_BASE_PATH "DEMO/PS3_KIDO/D_PS3_KIDO_US.AMB", dmPS3KidoLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト フランス
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_ps3_kido_localize_fileinfo_list_fr[] = {
	{GSS_BASE_PATH "DEMO/PS3_KIDO/D_PS3_KIDO_FR.AMB", dmPS3KidoLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト イタリア
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_ps3_kido_localize_fileinfo_list_it[] = {
	{GSS_BASE_PATH "DEMO/PS3_KIDO/D_PS3_KIDO_IT.AMB", dmPS3KidoLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト ドイツ
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_ps3_kido_localize_fileinfo_list_ge[] = {
	{GSS_BASE_PATH "DEMO/PS3_KIDO/D_PS3_KIDO_GE.AMB", dmPS3KidoLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト スペイン
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_ps3_kido_localize_fileinfo_list_sp[] = {
	{GSS_BASE_PATH "DEMO/PS3_KIDO/D_PS3_KIDO_SP.AMB", dmPS3KidoLoadPostFunc},
};
/// ローカライズ関連データファイル名リストテーブル
static const DMS_LOGO_COM_LOAD_FILE_INFO *dm_ps3_kido_localize_fileinfo_list_tbl[] = {
	dm_ps3_kido_localize_fileinfo_list_jp,
	dm_ps3_kido_localize_fileinfo_list_us,
	dm_ps3_kido_localize_fileinfo_list_fr,
	dm_ps3_kido_localize_fileinfo_list_it,
	dm_ps3_kido_localize_fileinfo_list_ge,
	dm_ps3_kido_localize_fileinfo_list_sp,
};


/// ローカライズ関連データファイル名リスト 日本 ノーマル画面
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_ps3_kido_nd_localize_fileinfo_list_jp[] = {
	{GSS_BASE_PATH "DEMO/PS3_KIDO/D_PS3_KIDO_ND_JP.AMB", dmPS3KidoLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト 英語 ノーマル画面
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_ps3_kido_nd_localize_fileinfo_list_us[] = {
	{GSS_BASE_PATH "DEMO/PS3_KIDO/D_PS3_KIDO_ND_US.AMB", dmPS3KidoLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト フランス ノーマル画面
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_ps3_kido_nd_localize_fileinfo_list_fr[] = {
	{GSS_BASE_PATH "DEMO/PS3_KIDO/D_PS3_KIDO_ND_FR.AMB", dmPS3KidoLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト イタリア ノーマル画面
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_ps3_kido_nd_localize_fileinfo_list_it[] = {
	{GSS_BASE_PATH "DEMO/PS3_KIDO/D_PS3_KIDO_ND_IT.AMB", dmPS3KidoLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト ドイツ ノーマル画面
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_ps3_kido_nd_localize_fileinfo_list_ge[] = {
	{GSS_BASE_PATH "DEMO/PS3_KIDO/D_PS3_KIDO_ND_GE.AMB", dmPS3KidoLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト スペイン ノーマル画面
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_ps3_kido_nd_localize_fileinfo_list_sp[] = {
	{GSS_BASE_PATH "DEMO/PS3_KIDO/D_PS3_KIDO_ND_SP.AMB", dmPS3KidoLoadPostFunc},
};
/// ローカライズ関連データファイル名リストテーブル ノーマル画面
static const DMS_LOGO_COM_LOAD_FILE_INFO *dm_ps3_kido_nd_localize_fileinfo_list_tbl[] = {
	dm_ps3_kido_nd_localize_fileinfo_list_jp,
	dm_ps3_kido_nd_localize_fileinfo_list_us,
	dm_ps3_kido_nd_localize_fileinfo_list_fr,
	dm_ps3_kido_nd_localize_fileinfo_list_it,
	dm_ps3_kido_nd_localize_fileinfo_list_ge,
	dm_ps3_kido_nd_localize_fileinfo_list_sp,
};

/// ローカライズ関連データファイル数
static const s32 dm_ps3_kido_localize_file_num = sizeof(dm_ps3_kido_localize_fileinfo_list_jp) / sizeof(DMS_LOGO_COM_LOAD_FILE_INFO);


/// 描画オブジェクト
//OBS_ACTION3D_NN_WORK *dm_ps3_kido_obj_3d_list = NULL;

/// アクション テクスチャ割り当てテーブル
static u8 dm_ps3_kido_tex_id_tbl[DMD_PS3_KIDO_ACT_MAX] = {
	DMD_PS3_KIDO_AOSTEX_LOCAL,
};

	

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// 初期化
// ==========================================================================
// ==========================================================================
// DmPS3KidoInit
/*!
 *	PS3起動時表示 初期化
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void DmPS3KidoInit(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	if (DmPS3KidoBuildCheck()) {
		// データビルド済み
		// 演出開始
		dmPS3KidoStart();
	}
	else {
#if defined (MTD_DEBUG)
		if (DmPS3KidoLoadCheck()) {
			// 中途半端にロードが始まっている	
			MTM_ASSERT(0);
		}
#endif
		// データロード開始
		MTM_TASK_MAKE_TCB(dmPS3KidoLoadWait, NULL,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_PS3_KIDO_TASK_PRIO_DATA_LOAD, DMD_PS3_KIDO_TASK_GROUP_DATA_LOAD,
						0/*work_size*/, "DM_PS3_KIDO");

		DmPS3KidoLoad();
	}
}


// ==========================================================================
// データロード
// ==========================================================================
// ==========================================================================
// DmPS3KidoLoad
/*!
 *	PS3起動時表示データロード
 *
 */
// ==========================================================================
void DmPS3KidoLoad(void)
{
	GSE_LANGUAGE	language = GsEnvGetLanguage();

	// ロード処理生成
	DmLogoComLoadFileCreate(&dm_ps3_kido_load_tcb);

	//// 共通データ
	//DmLogoComLoadFileReg(dm_ps3_kido_load_tcb, &dm_ps3_kido_com_fileinfo_list[0], dm_ps3_kido_com_file_num);

	// ローカライズ関連データ
	if (_am_draw_video.wide_screen) {
		// ワイド
		DmLogoComLoadFileReg(dm_ps3_kido_load_tcb, dm_ps3_kido_localize_fileinfo_list_tbl[language], dm_ps3_kido_localize_file_num);
	}
	else {
		// ノーマル
		DmLogoComLoadFileReg(dm_ps3_kido_load_tcb, dm_ps3_kido_nd_localize_fileinfo_list_tbl[language], dm_ps3_kido_localize_file_num);
	}

	// ロードチェック開始
	DmLogoComLoadFileStart(dm_ps3_kido_load_tcb);
}

// ==========================================================================
// DmPS3KidoLoadCheck
/*!
 *	PS3起動時表示データロード ロード終了チェック
 *
 *	@return	TRUE : ロード終了
 */
// ==========================================================================
BOOL DmPS3KidoLoadCheck(void)
{
	if ((dm_ps3_kido_load_tcb == NULL) && (dm_ps3_kido_data[0] != NULL)) {
		// データロードタスクがなくて 1つ目のデータが読み込まれている
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データビルド
// ==========================================================================
// ==========================================================================
// DmPS3KidoBuild
/*!
 *	PS3起動時表示データビルド
 *
 */
// ==========================================================================
void DmPS3KidoBuild(void)
{
	s32			i;
	void		*tex_amb[DMD_PS3_KIDO_AOSTEX_MAX];
	AOS_TEXTURE	*aos_tex;

	MTM_ASSERT(DmPS3KidoLoadCheck() == TRUE);
	MTM_ASSERT(dm_ps3_kido_build_state == FALSE);
	MTM_ASSERT(dm_ps3_kido_aos_tex == NULL);

	// ビルド処理生成
	dm_ps3_kido_build_tcb = MTM_TASK_MAKE_TCB(dmPS3KidoDataBuildMain, dmPS3KidoDataBuildDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_PS3_KIDO_TASK_PRIO_DATA_BUILD, DMD_PS3_KIDO_TASK_GROUP_DATA_BUILD,
						0/*work_size*/, "DM_PS3K_BUILD");

	// テクスチャ管理バッファ取得
	dm_ps3_kido_aos_tex = (AOS_TEXTURE*)amMemAlloc(sizeof(AOS_TEXTURE) * DMD_PS3_KIDO_AOSTEX_MAX);
	MI_CpuClear8(dm_ps3_kido_aos_tex, sizeof(AOS_TEXTURE) * DMD_PS3_KIDO_AOSTEX_MAX);

	// テクスチャビルド
	tex_amb[DMD_PS3_KIDO_AOSTEX_LOCAL] =
				amBindGet((AMS_AMB_HEADER*)dm_ps3_kido_data[DMD_PS3_KIDO_DATA_LOCAL_AMA_SET_AMB],
									IDB_D_PS3_KIDO_JP_D_PS3_KIDO_JP_AMB);

	aos_tex = dm_ps3_kido_aos_tex;
	for (i = 0; i < DMD_PS3_KIDO_AOSTEX_MAX; i++, aos_tex++) {
		AoTexBuild(aos_tex, tex_amb[i]);
		AoTexLoad(aos_tex);
	}
}

// ==========================================================================
// DmPS3KidoBuildCheck
/*!
 *	PS3起動時表示データビルド ビルド終了チェック
 *
 *	@return	TRUE : ビルド終了
 */
// ==========================================================================
BOOL DmPS3KidoBuildCheck(void)
{
	if (dm_ps3_kido_build_state) {
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データフラッシュ
// ==========================================================================
// ==========================================================================
// DmPS3KidoFlush
/*!
 *	PS3起動時表示データフラッシュ
 *
 */
// ==========================================================================
void DmPS3KidoFlush(void)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;

	MTM_ASSERT(dm_ps3_kido_aos_tex);
	MTM_ASSERT(dm_ps3_kido_build_state == TRUE);
	MTM_ASSERT(dm_ps3_kido_flush_tcb == NULL);

	// フラッシュ処理生成
	dm_ps3_kido_flush_tcb = MTM_TASK_MAKE_TCB(dmPS3KidoDataFlushMain, dmPS3KidoDataFlushDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_PS3_KIDO_TASK_PRIO_DATA_FLUSH, DMD_PS3_KIDO_TASK_GROUP_DATA_FLUSH,
						0/*work_size*/, "DM_PS3K_FLUSH");

	// テクスチャフラッシュ
	aos_tex = dm_ps3_kido_aos_tex;
	for (i = 0; i < DMD_PS3_KIDO_AOSTEX_MAX; i++, aos_tex++) {
		AoTexRelease(aos_tex);
	}
}

// ==========================================================================
// DmPS3KidoFlushCheck
/*!
 *	PS3起動時表示データフラッシュ フラッシュ終了チェック
 *
 *	@return	TRUE : フラッシュ終了
 */
// ==========================================================================
BOOL DmPS3KidoFlushCheck(void)
{
	if (!dm_ps3_kido_build_state) {
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データリリース
// ==========================================================================
// ==========================================================================
// DmPS3KidoRelease
/*!
 *	PS3起動時表示データリリース
 */
// ==========================================================================
void DmPS3KidoRelease(void)
{
	s32	i;

	for (i = 0; i < DMD_PS3_KIDO_DATA_MAX; i++) {
		if (dm_ps3_kido_data[i]) {
			amMemFree(dm_ps3_kido_data[i]);
		}
		dm_ps3_kido_data[i] = NULL;
	}
}

// ==========================================================================
// DmPS3KidoReleaseCheck
/*!
 *	PS3起動時表示データリリース リリース終了チェック
 *
 *	@return	TRUE : リリース終了
 */
// ==========================================================================
BOOL DmPS3KidoReleaseCheck(void)
{
	if ((dm_ps3_kido_load_tcb == NULL) && (dm_ps3_kido_data[0] == NULL)) {
		// データロードタスクがなくて 1つ目のデータが読み込まれていない
		return (TRUE);
	}
	return (FALSE);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// データロード待機
// ==========================================================================
// ==========================================================================
// dmPS3KidoLoadWait
/*!
 *	データロード待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmPS3KidoLoadWait(MTS_TASK_TCB *tcb)
{
	if (DmPS3KidoLoadCheck()) {
		// データビルド開始
		DmPS3KidoBuild();

		mtTaskChangeTcbProcedure(tcb, dmPS3KidoBuildWait);
	}
}

// ==========================================================================
// dmPS3KidoBuildWait
/*!
 *	データビルド待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmPS3KidoBuildWait(MTS_TASK_TCB *tcb)
{
	if (DmPS3KidoBuildCheck()) {

		// データロード終了
		mtTaskClearTcb(tcb);

		// 演出開始
		dmPS3KidoStart();
	}
}

// ==========================================================================
// アクション生成・破棄
// ==========================================================================
// ==========================================================================
// dmPS3KidoActionCreate
/*!
 *	アクション生成
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmPS3KidoActionCreate(DMS_PS3_KIDO_WORK *logo_work)
{
	s32		i;
	void	*ama;

	// アクション構築
	ama = amBindGet((AMS_AMB_HEADER*)dm_ps3_kido_data[DMD_PS3_KIDO_DATA_LOCAL_AMA_SET_AMB],
				IDB_D_PS3_KIDO_JP_D_PS3_KIDO_AMA);

	for (i = 0; i < DMD_PS3_KIDO_ACT_MAX; i++) {
		// アクション構築
		AoActSetTexture(AoTexGetTexList(dm_ps3_kido_aos_tex + dm_ps3_kido_tex_id_tbl[i]));
		logo_work->act[i] = AoActCreate(ama, i);
	}
}

// ==========================================================================
// dmPS3KidoActionDelete
/*!
 *	アクション破棄
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmPS3KidoActionDelete(DMS_PS3_KIDO_WORK *logo_work)
{
	s32		i;

	for (i = 0; i < DMD_PS3_KIDO_ACT_MAX; i++) {
		AoActDelete(logo_work->act[i]);
	}
}

// ==========================================================================
// メイン処理
// ==========================================================================
// ==========================================================================
// dmPS3KidoStart
/*!
 *	PS3起動時表示開始
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmPS3KidoStart(void)
{
	MTS_TASK_TCB		*tcb;
	DMS_PS3_KIDO_WORK *logo_work;

	NNS_RGBA		diffuse = { 1.0f, 1.0f, 1.0f, 1.0f};
	NNS_RGB			ambient = { 1.0f, 1.0f, 1.0f};

	// メイン処理開始
	tcb = MTM_TASK_MAKE_TCB(dmPS3KidoMainFunc, NULL,
						0/*flag*/, 0/*task_pause_level*/,
						DMD_PS3_KIDO_TASK_PRIO_MAIN, DMD_PS3_KIDO_TASK_GROUP_MAIN,
						sizeof(DMS_PS3_KIDO_WORK), "DM_LESRB_MAIN");
	logo_work = (DMS_PS3_KIDO_WORK*)mtTaskGetTcbWork(tcb);
	MI_CpuClear8(logo_work, sizeof(DMS_PS3_KIDO_WORK));

	// 環境初期化
	nnSetPrimitive3DMaterial(&diffuse, &ambient, 1.0f);

	AoActSysSetDrawStateEnable(FALSE);

	// アクション初期化
	dmPS3KidoActionCreate(logo_work);

	// フェード開始
	IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_BLACK_FADEIN,
								DMD_PS3_KIDO_FADEIN_TIME, TRUE);

	// フェードイン待機へ
	logo_work->func = dmPS3KidoFadeInWaitFunc;
}


// ==========================================================================
// dmPS3KidoMainFunc
/*!
 *	メイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmPS3KidoMainFunc(MTS_TASK_TCB *tcb)
{
	s32					i;
	DMS_PS3_KIDO_WORK	*logo_work;

	logo_work = (DMS_PS3_KIDO_WORK*)mtTaskGetTcbWork(tcb);

	// 演出
	if (logo_work->func) {
		logo_work->func(logo_work);
	}

#if 0
	// スキップチェック
	if ((logo_work->flag & DMD_PS3_KIDO_FLAG_SKIP_OK) &&
				!(logo_work->flag & DMD_PS3_KIDO_FLAG_SKIP)) {
		if (AoPadSomeoneStand(DMD_PS3_KIDO_SKIP_KEY) >= 0) {
			// スキップ実行
			logo_work->flag |= DMD_PS3_KIDO_FLAG_SKIP;

			if (IzFadeIsEnd()) {
				// フェード未実行
				// フェード開始
				IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEOUT,
											DMD_PS3_KIDO_FADEOUT_TIME, TRUE);
			}
			// フェードアウトへ移行
			logo_work->func = dmPS3KidoFadeOutWaitFunc;
		}
	}
#endif

	if (logo_work->flag & DMD_PS3_KIDO_FLAG_END) {
		// 演出終了

		// 前終了待機処理へ
		mtTaskChangeTcbProcedure(tcb, dmPS3KidoPreEndWait);
		logo_work->timer = 0;

#if 0
		// アクション破棄
		dmPS3KidoActionDelete(logo_work);
		// メイン処理破棄
		mtTaskClearTcb(tcb);

		// データのFlush, Releaseはメニューを抜ける時に一括で行う

		// 次のイベントへ
		SyChangeNextEvt();
#endif
		return;
	}

	// 描画
	AoActSysSetDrawTaskPrio();	// 標準設定
	for (i = 0; i < DMD_PS3_KIDO_ACT_MAX; i++) {
		AoActUpdate(logo_work->act[i]);
		AoActDraw(logo_work->act[i]);
	}
}


// ==========================================================================
// 演出
// ==========================================================================
// ==========================================================================
// dmPS3KidoFadeInWaitFunc
/*!
 *	フェード待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmPS3KidoFadeInWaitFunc(DMS_PS3_KIDO_WORK *logo_work)
{
	if (IzFadeIsEnd()) {
		// 表示待機へ移行
		logo_work->func = dmPS3KidoDispWaitFunc;
		logo_work->timer = 0;
	}
}

// ==========================================================================
// dmPS3KidoDispWaitFunc
/*!
 *	表示待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmPS3KidoDispWaitFunc(DMS_PS3_KIDO_WORK *logo_work)
{
	logo_work->timer++;
	if (logo_work->timer >= DMD_PS3_KIDO_DISP_TIME) {
		// フェードアウトへ移行
		logo_work->func = dmPS3KidoFadeOutWaitFunc;

		// フェード開始
#if _PS3
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_BLACK_FADEOUT,
									DMD_PS3_KIDO_FADEOUT_TIME, TRUE);
#else
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEOUT,
									DMD_PS3_KIDO_FADEOUT_TIME, TRUE);
#endif

//		// スキップ許可OFF
//		logo_work->flag &= ~DMD_PS3_KIDO_FLAG_SKIP_OK;
		return;
	}

//	if (logo_work->timer == 30) {
//		// スキップ許可
//		logo_work->flag |= DMD_PS3_KIDO_FLAG_SKIP_OK;
//	}
}

// ==========================================================================
// dmPS3KidoFadeOutWaitFunc
/*!
 *	フェード待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmPS3KidoFadeOutWaitFunc(DMS_PS3_KIDO_WORK *logo_work)
{
	if (IzFadeIsEnd()) {
		// 演出終了
		logo_work->flag |= DMD_PS3_KIDO_FLAG_END;
	}
}

// ==========================================================================
// リリース待機
// ==========================================================================
// ==========================================================================
// dmPS3KidoPreEndWait
/*!
 *	データロード終了待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmPS3KidoPreEndWait(MTS_TASK_TCB *tcb)
{
	DMS_PS3_KIDO_WORK	*logo_work;
	logo_work = (DMS_PS3_KIDO_WORK*)mtTaskGetTcbWork(tcb);

	logo_work->timer++;
	if (logo_work->timer > 2) {
		// アクション破棄
		dmPS3KidoActionDelete(logo_work);

		// データフラッシュ
		DmPS3KidoFlush();

		// データフラッシュ待機へ
		mtTaskChangeTcbProcedure(tcb, dmPS3KidoFlushWaitFunc);
	}
}

// ==========================================================================
// dmPS3KidoFlushWaitFunc
/*!
 *	データフラッシュ待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmPS3KidoFlushWaitFunc(MTS_TASK_TCB *tcb)
{
	if (!DmPS3KidoFlushCheck()) {
		return;
	}

	// データフリリース
	DmPS3KidoRelease();
	// リリース処理待機へ
	mtTaskChangeTcbProcedure(tcb, dmPS3KidoRelesehWaitFunc);
}

// ==========================================================================
// dmPS3KidoRelesehWaitFunc
/*!
 *	データリリース待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmPS3KidoRelesehWaitFunc(MTS_TASK_TCB *tcb)
{
	if (!DmPS3KidoReleaseCheck()) {
		return;
	}
	// 終了待機破棄
	mtTaskClearTcb(tcb);

	// 次のイベントへ
	SyChangeNextEvt();
}



// ==========================================================================
// ファイルロード時後処理
// ==========================================================================
// ==========================================================================
// dmPS3KidoLoadPostFunc
/*!
 *	データロード後処理
 *
 *	@param	context	[in]	コンテキスト
 */
// ==========================================================================
void dmPS3KidoLoadPostFunc(DMS_LOGO_COM_LOAD_CONTEXT *context)
{
//	s32				i;
//	AMS_AMB_HEADER	*amb_header;

	dm_ps3_kido_data[context->no] = context->fs_req->buf;
	context->fs_req->buf = NULL;

	// コンバート
	amBindConvertAll((u8*)dm_ps3_kido_data[context->no]);

//	if (context->no == DMD_PS3_KIDO_DATA_LOCAL_AMA_SET_AMB) {
//		void	*tex_amb;
//		tex_amb = amBindGet((AMS_AMB_HEADER*)dm_ps3_kido_data[context->no],
//								IDB_D_LOGO_SONIC_D_LOGO_SONIC_AMB);
//		amBindConvertAll((u8*)tex_amb);
//	}
}

// ==========================================================================
// データビルド
// ==========================================================================
// ==========================================================================
// dmPS3KidoDataBuildMain
/*!
 *	データビルドメイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmPS3KidoDataBuildMain(MTS_TASK_TCB *tcb)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;
	BOOL		b_sts = TRUE;

	// テクスチャビルド
	aos_tex = dm_ps3_kido_aos_tex;
	for (i = 0; i < DMD_PS3_KIDO_AOSTEX_MAX; i++, aos_tex++) {
		if (!AoTexIsLoaded(aos_tex)) {
			b_sts = FALSE;
		}
	}

	// ビルド待機
	if (!b_sts) {
		return;
	}

	// ビルド終了
	mtTaskClearTcb(tcb);
	dm_ps3_kido_build_state = TRUE;
}

// ==========================================================================
// dmPS3KidoDataBuildDest
/*!
 *	データビルドデストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmPS3KidoDataBuildDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dm_ps3_kido_build_tcb = NULL;
}

// ==========================================================================
// データフラッシュ
// ==========================================================================
// ==========================================================================
// dmPS3KidoDataFlushMain
/*!
 *	データフラッシュメイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmPS3KidoDataFlushMain(MTS_TASK_TCB *tcb)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;
	BOOL		b_sts = TRUE;

	// テクスチャフラッシュ
	aos_tex = dm_ps3_kido_aos_tex;
	for (i = 0; i < DMD_PS3_KIDO_AOSTEX_MAX; i++, aos_tex++) {
		if (!AoTexIsReleased(aos_tex)) {
			b_sts = FALSE;
		}
	}

	// フラッシュ待機
	if (!b_sts) {
		return;
	}
	
	// テクスチャ管理バッファ解放
	amMemFree(dm_ps3_kido_aos_tex);
	dm_ps3_kido_aos_tex = NULL;

	// フラッシュ終了
	mtTaskClearTcb(tcb);
	dm_ps3_kido_build_state = FALSE;
}

// ==========================================================================
// dmPS3KidoDataFlushDest
/*!
 *	データフラッシュデストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmPS3KidoDataFlushDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dm_ps3_kido_flush_tcb = NULL;
}


#endif // #if (defined(HOG_RGN_JP) && _PS3) || _PC
// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
