// ==========================================================================
/*!
  @file dmWiiExpOpe.cpp
  @brief Wii 横持ち促し画面

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: dmWiiExpOpe.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
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

#include "dmLogoCom.h"

#include "dmWiiExpOpe.h"

// データヘッダ
#include "Wii/arc/D_WII_EXP_OPE.HMB"
#include "Wii/ace/D_WII_EXP_OPE.HMA"

//----- Definitions ---------------------------------------------------------
/* タスク設定 */
#define DMD_WII_EO_TASK_PRIO_DATA_LOAD		(0x1000)		//!< ロードタスクプライオリティ
#define DMD_WII_EO_TASK_GROUP_DATA_LOAD		(0)				//!< ロードタスクグループ
#define DMD_WII_EO_TASK_PRIO_DATA_BUILD		(0x1000)		//!< ビルドタスクプライオリティ
#define DMD_WII_EO_TASK_GROUP_DATA_BUILD	(0)				//!< ビルドタスクグループ
#define DMD_WII_EO_TASK_PRIO_DATA_FLUSH		(0x1000)		//!< フラッシュタスクプライオリティ
#define DMD_WII_EO_TASK_GROUP_DATA_FLUSH	(0)				//!< フラッシュタスクグループ
#define DMD_WII_EO_TASK_PRIO_DATA_RELEASE	(0x1000)		//!< リリースタスクプライオリティ
#define DMD_WII_EO_TASK_GROUP_DATA_RELEASE	(0)				//!< リリースタスクグループ

#define DMD_WII_EO_TASK_PRIO_MAIN			(0x1000)		//!< メイン処理プライオリティ
#define DMD_WII_EO_TASK_GROUP_MAIN			(0)				//!< メイン処理グループ

/* 演出設定 */
#define DMD_WII_EO_FADEIN_TIME			(30)		//!< フェードイン時間
#define DMD_WII_EO_DISP_MIN_TIME		(60)		//!< 最低ロゴ表示時間
#define DMD_WII_EO_FADEOUT_TIME			(30)		//!< フェードアウト時間

#define DMD_WII_EO_SKIP_KEY				(KEY_R_UP | KEY_R_DOWN | KEY_R_LEFT | KEY_R_RIGHT)	//!< スキップキー

/* データ設定 */
/// 読み込みデータ
enum {
//	DMD_WII_EO_DATA_COM_AMA_SET_AMB	= 0,		//!< 共通 AMAデータセットAMB

	DMD_WII_EO_DATA_LOCAL_AMA_SET_AMB	= 0,	//!< ローカライズ AMAデータセットAMB

	DMD_WII_EO_DATA_MAX
};

/// アクションデータ
enum {
	DMD_WII_EO_ACT_BASE	= 0,
	DMD_WII_EO_ACT_TEXT,
	DMD_WII_EO_ACT_HAND,

	DMD_WII_EO_ACT_MAX
};

/// AOSテクスチャタイプ
enum {
	DMD_WII_EO_AOSTEX_LOCAL	= 0,

	DMD_WII_EO_AOSTEX_MAX
};

/// Wii 横持ち促し画面ワーク
typedef struct tag_DMS_WII_EO_WORK {
	u32				flag;
	s32				timer;

	AOS_ACTION		*act[DMD_WII_EO_ACT_MAX];		//!< アクション

	void (*func)(struct tag_DMS_WII_EO_WORK*);

} DMS_WII_EO_WORK;

#define DMD_WII_EO_FLAG_SKIP_OK		(0x00000001)	//!< スキップ許可フラグ
#define DMD_WII_EO_FLAG_SKIP		(0x00000002)	//!< スキップ実行済み
#define DMD_WII_EO_FLAG_END			(0x00000004)	//!< 演出終了

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void dmWiiExpOpeLoadWait(MTS_TASK_TCB *tcb);
static void dmWiiExpOpeBuildWait(MTS_TASK_TCB *tcb);

static void dmWiiExpOpeActionCreate(DMS_WII_EO_WORK *logo_work);
static void dmWiiExpOpeActionDelete(DMS_WII_EO_WORK *logo_work);

static void dmWiiExpOpeStart(void);
static void dmWiiExpOpeMainFunc(MTS_TASK_TCB *tcb);
static void dmWiiExpOpeFadeInWaitFunc(DMS_WII_EO_WORK *logo_work);
static void dmWiiExpOpeDispWaitFunc(DMS_WII_EO_WORK *logo_work);
static void dmWiiExpOpeFadeOutWaitFunc(DMS_WII_EO_WORK *logo_work);

static void dmWiiExpOpePreEndWait(MTS_TASK_TCB *tcb);
static void dmWiiExpOpeFlushWaitFunc(MTS_TASK_TCB *tcb);
static void dmWiiExpOpeRelesehWaitFunc(MTS_TASK_TCB *tcb);

static void dmWiiExpOpeLoadPostFunc(DMS_LOGO_COM_LOAD_CONTEXT *context);
static void dmWiiExpOpeDataBuildMain(MTS_TASK_TCB *tcb);
static void dmWiiExpOpeDataBuildDest(MTS_TASK_TCB *tcb);
static void dmWiiExpOpeDataFlushMain(MTS_TASK_TCB *tcb);
static void dmWiiExpOpeDataFlushDest(MTS_TASK_TCB *tcb);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static MTS_TASK_TCB *dm_wii_exp_ope_load_tcb = NULL;		//!< データロードTCB
static MTS_TASK_TCB *dm_wii_exp_ope_build_tcb = NULL;		//!< データビルドTCB
static MTS_TASK_TCB *dm_wii_exp_ope_flush_tcb = NULL;		//!< データフラッシュTCB
static MTS_TASK_TCB *dm_wii_exp_ope_release_tcb = NULL;		//!< データリリースTCB

static AOS_TEXTURE *dm_wii_exp_ope_aos_tex = NULL;	//!< テクスチャ
static BOOL dm_wii_exp_ope_build_state = FALSE;		//!< ビルド状況

/// データ
void *dm_wii_exp_ope_data[DMD_WII_EO_DATA_MAX] = {NULL};

#if 0
/// 共通データファイル名リスト
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_wii_exp_ope_com_fileinfo_list[] = {
	{GSS_BASE_PATH "DEMO/LOGO/D_LOGO_ESRB.AMB", dmWiiExpOpeLoadPostFunc},
	// 以下その他
};

/// データファイル数
static const s32 dm_wii_exp_ope_com_file_num = sizeof(dm_wii_exp_ope_com_fileinfo_list) / sizeof(DMS_LOGO_COM_LOAD_FILE_INFO);
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
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_wii_exp_ope_localize_fileinfo_list_jp[] = {
	{GSS_BASE_PATH "DEMO/EXP_OPE/D_WII_EXP_OPE_JP.AMB", dmWiiExpOpeLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト 英語
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_wii_exp_ope_localize_fileinfo_list_us[] = {
	{GSS_BASE_PATH "DEMO/EXP_OPE/D_WII_EXP_OPE_US.AMB", dmWiiExpOpeLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト フランス
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_wii_exp_ope_localize_fileinfo_list_fr[] = {
	{GSS_BASE_PATH "DEMO/EXP_OPE/D_WII_EXP_OPE_FR.AMB", dmWiiExpOpeLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト イタリア
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_wii_exp_ope_localize_fileinfo_list_it[] = {
	{GSS_BASE_PATH "DEMO/EXP_OPE/D_WII_EXP_OPE_IT.AMB", dmWiiExpOpeLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト ドイツ
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_wii_exp_ope_localize_fileinfo_list_ge[] = {
	{GSS_BASE_PATH "DEMO/EXP_OPE/D_WII_EXP_OPE_GE.AMB", dmWiiExpOpeLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト スペイン
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_wii_exp_ope_localize_fileinfo_list_sp[] = {
	{GSS_BASE_PATH "DEMO/EXP_OPE/D_WII_EXP_OPE_SP.AMB", dmWiiExpOpeLoadPostFunc},
};
/// ローカライズ関連データファイル名リストテーブル
static const DMS_LOGO_COM_LOAD_FILE_INFO *dm_wii_exp_ope_localize_fileinfo_list_tbl[] = {
	dm_wii_exp_ope_localize_fileinfo_list_jp,
	dm_wii_exp_ope_localize_fileinfo_list_us,
	dm_wii_exp_ope_localize_fileinfo_list_fr,
	dm_wii_exp_ope_localize_fileinfo_list_it,
	dm_wii_exp_ope_localize_fileinfo_list_ge,
	dm_wii_exp_ope_localize_fileinfo_list_sp,
};

/// ローカライズ関連データファイル数
static const s32 dm_wii_exp_ope_localize_file_num = sizeof(dm_wii_exp_ope_localize_fileinfo_list_jp) / sizeof(DMS_LOGO_COM_LOAD_FILE_INFO);


/// 描画オブジェクト
//OBS_ACTION3D_NN_WORK *dm_wii_exp_ope_obj_3d_list = NULL;

/// アクション テクスチャ割り当てテーブル
static u8 dm_wii_exp_ope_tex_id_tbl[DMD_WII_EO_ACT_MAX] = {
	DMD_WII_EO_AOSTEX_LOCAL,
	DMD_WII_EO_AOSTEX_LOCAL,
	DMD_WII_EO_AOSTEX_LOCAL,
};

	

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// 初期化
// ==========================================================================
// ==========================================================================
// DmWiiExpOpeInit
/*!
 *	Wii 横持ち促し画面 初期化
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void DmWiiExpOpeInit(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	if (DmWiiExpOpeBuildCheck()) {
		// データビルド済み
		// 演出開始
		dmWiiExpOpeStart();
	}
	else {
#if defined (MTD_DEBUG)
		if (DmWiiExpOpeLoadCheck()) {
			// 中途半端にロードが始まっている	
			MTM_ASSERT(0);
		}
#endif
		// データロード開始
		MTM_TASK_MAKE_TCB(dmWiiExpOpeLoadWait, NULL,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_WII_EO_TASK_PRIO_DATA_LOAD, DMD_WII_EO_TASK_GROUP_DATA_LOAD,
						0/*work_size*/, "DM_WEO_LW");

		DmWiiExpOpeLoad();
	}
}


// ==========================================================================
// データロード
// ==========================================================================
// ==========================================================================
// DmWiiExpOpeLoad
/*!
 *	Wii 横持ち促し画面データロード
 *
 */
// ==========================================================================
void DmWiiExpOpeLoad(void)
{
	GSE_LANGUAGE	language = GsEnvGetLanguage();

	// ロード処理生成
	DmLogoComLoadFileCreate(&dm_wii_exp_ope_load_tcb);

	//// 共通データ
	//DmLogoComLoadFileReg(dm_wii_exp_ope_load_tcb, &dm_wii_exp_ope_com_fileinfo_list[0], dm_wii_exp_ope_com_file_num);

	// ローカライズ関連データ
	DmLogoComLoadFileReg(dm_wii_exp_ope_load_tcb, dm_wii_exp_ope_localize_fileinfo_list_tbl[language], dm_wii_exp_ope_localize_file_num);

	// ロードチェック開始
	DmLogoComLoadFileStart(dm_wii_exp_ope_load_tcb);
}

// ==========================================================================
// DmWiiExpOpeLoadCheck
/*!
 *	Wii 横持ち促し画面データロード ロード終了チェック
 *
 *	@return	TRUE : ロード終了
 */
// ==========================================================================
BOOL DmWiiExpOpeLoadCheck(void)
{
	if ((dm_wii_exp_ope_load_tcb == NULL) && (dm_wii_exp_ope_data[0] != NULL)) {
		// データロードタスクがなくて 1つ目のデータが読み込まれている
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データビルド
// ==========================================================================
// ==========================================================================
// DmWiiExpOpeBuild
/*!
 *	Wii 横持ち促し画面データビルド
 *
 */
// ==========================================================================
void DmWiiExpOpeBuild(void)
{
	s32			i;
	void		*tex_amb[DMD_WII_EO_AOSTEX_MAX];
	AOS_TEXTURE	*aos_tex;

	MTM_ASSERT(DmWiiExpOpeLoadCheck() == TRUE);
	MTM_ASSERT(dm_wii_exp_ope_build_state == FALSE);
	MTM_ASSERT(dm_wii_exp_ope_aos_tex == NULL);

	// ビルド処理生成
	dm_wii_exp_ope_build_tcb = MTM_TASK_MAKE_TCB(dmWiiExpOpeDataBuildMain, dmWiiExpOpeDataBuildDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_WII_EO_TASK_PRIO_DATA_BUILD, DMD_WII_EO_TASK_GROUP_DATA_BUILD,
						0/*work_size*/, "DM_WEO_BUILD");

	// テクスチャ管理バッファ取得
	dm_wii_exp_ope_aos_tex = (AOS_TEXTURE*)amMemAlloc(sizeof(AOS_TEXTURE) * DMD_WII_EO_AOSTEX_MAX);
	MI_CpuClear8(dm_wii_exp_ope_aos_tex, sizeof(AOS_TEXTURE) * DMD_WII_EO_AOSTEX_MAX);

	// テクスチャビルド
	tex_amb[DMD_WII_EO_AOSTEX_LOCAL] =
				amBindGet((AMS_AMB_HEADER*)dm_wii_exp_ope_data[DMD_WII_EO_DATA_LOCAL_AMA_SET_AMB],
									IDB_D_WII_EXP_OPE_JP_D_WII_EXP_OPE_JP_AMB);

	aos_tex = dm_wii_exp_ope_aos_tex;
	for (i = 0; i < DMD_WII_EO_AOSTEX_MAX; i++, aos_tex++) {
		AoTexBuild(aos_tex, tex_amb[i]);
		AoTexLoad(aos_tex);
	}
}

// ==========================================================================
// DmWiiExpOpeBuildCheck
/*!
 *	Wii 横持ち促し画面データビルド ビルド終了チェック
 *
 *	@return	TRUE : ビルド終了
 */
// ==========================================================================
BOOL DmWiiExpOpeBuildCheck(void)
{
	if (dm_wii_exp_ope_build_state) {
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データフラッシュ
// ==========================================================================
// ==========================================================================
// DmWiiExpOpeFlush
/*!
 *	Wii 横持ち促し画面データフラッシュ
 *
 */
// ==========================================================================
void DmWiiExpOpeFlush(void)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;

	MTM_ASSERT(dm_wii_exp_ope_aos_tex);
	MTM_ASSERT(dm_wii_exp_ope_build_state == TRUE);
	MTM_ASSERT(dm_wii_exp_ope_flush_tcb == NULL);

	// フラッシュ処理生成
	dm_wii_exp_ope_flush_tcb = MTM_TASK_MAKE_TCB(dmWiiExpOpeDataFlushMain, dmWiiExpOpeDataFlushDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_WII_EO_TASK_PRIO_DATA_FLUSH, DMD_WII_EO_TASK_GROUP_DATA_FLUSH,
						0/*work_size*/, "DM_WEO_FLUSH");

	// テクスチャフラッシュ
	aos_tex = dm_wii_exp_ope_aos_tex;
	for (i = 0; i < DMD_WII_EO_AOSTEX_MAX; i++, aos_tex++) {
		AoTexRelease(aos_tex);
	}
}

// ==========================================================================
// DmWiiExpOpeFlushCheck
/*!
 *	Wii 横持ち促し画面データフラッシュ フラッシュ終了チェック
 *
 *	@return	TRUE : フラッシュ終了
 */
// ==========================================================================
BOOL DmWiiExpOpeFlushCheck(void)
{
	if (!dm_wii_exp_ope_build_state) {
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データリリース
// ==========================================================================
// ==========================================================================
// DmWiiExpOpeRelease
/*!
 *	Wii 横持ち促し画面データリリース
 */
// ==========================================================================
void DmWiiExpOpeRelease(void)
{
	s32	i;

	for (i = 0; i < DMD_WII_EO_DATA_MAX; i++) {
		if (dm_wii_exp_ope_data[i]) {
			amMemFree(dm_wii_exp_ope_data[i]);
		}
		dm_wii_exp_ope_data[i] = NULL;
	}
}

// ==========================================================================
// DmWiiExpOpeReleaseCheck
/*!
 *	Wii 横持ち促し画面データリリース リリース終了チェック
 *
 *	@return	TRUE : リリース終了
 */
// ==========================================================================
BOOL DmWiiExpOpeReleaseCheck(void)
{
	if ((dm_wii_exp_ope_load_tcb == NULL) && (dm_wii_exp_ope_data[0] == NULL)) {
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
// dmWiiExpOpeLoadWait
/*!
 *	データロード待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmWiiExpOpeLoadWait(MTS_TASK_TCB *tcb)
{
	if (DmWiiExpOpeLoadCheck()) {
		// データビルド開始
		DmWiiExpOpeBuild();

		mtTaskChangeTcbProcedure(tcb, dmWiiExpOpeBuildWait);
	}
}

// ==========================================================================
// dmWiiExpOpeBuildWait
/*!
 *	データビルド待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmWiiExpOpeBuildWait(MTS_TASK_TCB *tcb)
{
	if (DmWiiExpOpeBuildCheck()) {

		// データロード終了
		mtTaskClearTcb(tcb);

		// 演出開始
		dmWiiExpOpeStart();
	}
}

// ==========================================================================
// アクション生成・破棄
// ==========================================================================
// ==========================================================================
// dmWiiExpOpeActionCreate
/*!
 *	アクション生成
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmWiiExpOpeActionCreate(DMS_WII_EO_WORK *logo_work)
{
	s32		i;
	void	*ama;

	// アクション構築
	ama = amBindGet((AMS_AMB_HEADER*)dm_wii_exp_ope_data[DMD_WII_EO_DATA_LOCAL_AMA_SET_AMB],
				IDB_D_WII_EXP_OPE_JP_D_WII_EXP_OPE_AMA);

	for (i = 0; i < DMD_WII_EO_ACT_MAX; i++) {
		// アクション構築
		AoActSetTexture(AoTexGetTexList(dm_wii_exp_ope_aos_tex + dm_wii_exp_ope_tex_id_tbl[i]));
		logo_work->act[i] = AoActCreate(ama, i);
	}
}

// ==========================================================================
// dmWiiExpOpeActionDelete
/*!
 *	アクション破棄
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmWiiExpOpeActionDelete(DMS_WII_EO_WORK *logo_work)
{
	s32		i;

	for (i = 0; i < DMD_WII_EO_ACT_MAX; i++) {
		AoActDelete(logo_work->act[i]);
	}
}

// ==========================================================================
// メイン処理
// ==========================================================================
// ==========================================================================
// dmWiiExpOpeStart
/*!
 *	Wii 横持ち促し画面開始
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmWiiExpOpeStart(void)
{
	MTS_TASK_TCB		*tcb;
	DMS_WII_EO_WORK *logo_work;

	NNS_RGBA		diffuse = { 1.0f, 1.0f, 1.0f, 1.0f};
	NNS_RGB			ambient = { 1.0f, 1.0f, 1.0f};

	// メイン処理開始
	tcb = MTM_TASK_MAKE_TCB(dmWiiExpOpeMainFunc, NULL,
						0/*flag*/, 0/*task_pause_level*/,
						DMD_WII_EO_TASK_PRIO_MAIN, DMD_WII_EO_TASK_GROUP_MAIN,
						sizeof(DMS_WII_EO_WORK), "DM_WEO_MAIN");
	logo_work = (DMS_WII_EO_WORK*)mtTaskGetTcbWork(tcb);
	MI_CpuClear8(logo_work, sizeof(DMS_WII_EO_WORK));

	// 環境初期化
	nnSetPrimitive3DMaterial(&diffuse, &ambient, 1.0f);

	AoActSysSetDrawStateEnable(FALSE);

	// アクション初期化
	dmWiiExpOpeActionCreate(logo_work);

	// フェード開始
	IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEIN,
								DMD_WII_EO_FADEIN_TIME, TRUE);

	// フェードイン待機へ
	logo_work->func = dmWiiExpOpeFadeInWaitFunc;
}


// ==========================================================================
// dmWiiExpOpeMainFunc
/*!
 *	メイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmWiiExpOpeMainFunc(MTS_TASK_TCB *tcb)
{
	s32					i;
	DMS_WII_EO_WORK	*logo_work;

	logo_work = (DMS_WII_EO_WORK*)mtTaskGetTcbWork(tcb);

	// 演出
	if (logo_work->func) {
		logo_work->func(logo_work);
	}

	// スキップチェック
	if ((logo_work->flag & DMD_WII_EO_FLAG_SKIP_OK) &&
				!(logo_work->flag & DMD_WII_EO_FLAG_SKIP)) {
		if (AoPadSomeoneStand(DMD_WII_EO_SKIP_KEY) >= 0) {
			// スキップ実行
			logo_work->flag |= DMD_WII_EO_FLAG_SKIP;
			// スキップ許可OFF
			logo_work->flag &= ~DMD_WII_EO_FLAG_SKIP_OK;

			if (IzFadeIsEnd()) {
				// フェード未実行
				// フェード開始
				IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEOUT,
											DMD_WII_EO_FADEOUT_TIME, TRUE);
			}
			// フェードアウトへ移行
			logo_work->func = dmWiiExpOpeFadeOutWaitFunc;
		}
	}

	if (logo_work->flag & DMD_WII_EO_FLAG_END) {
		// 演出終了

		// 前終了待機処理へ
		mtTaskChangeTcbProcedure(tcb, dmWiiExpOpePreEndWait);
		logo_work->timer = 0;

#if 0
		// アクション破棄
		dmWiiExpOpeActionDelete(logo_work);
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
	for (i = 0; i < DMD_WII_EO_ACT_MAX; i++) {
		AoActSetTexture(AoTexGetTexList(dm_wii_exp_ope_aos_tex + dm_wii_exp_ope_tex_id_tbl[i]));
		AoActUpdate(logo_work->act[i]);
		AoActDraw(logo_work->act[i]);
	}
}


// ==========================================================================
// 演出
// ==========================================================================
// ==========================================================================
// dmWiiExpOpeFadeInWaitFunc
/*!
 *	フェード待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmWiiExpOpeFadeInWaitFunc(DMS_WII_EO_WORK *logo_work)
{
	if (IzFadeIsEnd()) {
		// 表示待機へ移行
		logo_work->func = dmWiiExpOpeDispWaitFunc;
		logo_work->timer = 0;
	}
}

// ==========================================================================
// dmWiiExpOpeDispWaitFunc
/*!
 *	表示待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmWiiExpOpeDispWaitFunc(DMS_WII_EO_WORK *logo_work)
{
	logo_work->timer++;
#if 0
	//if (logo_work->timer >= DMD_WII_EO_DISP_TIME) {
	if (logo_work->timer >= DMD_WII_EO_DISP_TIME) {
		// フェードアウトへ移行
		logo_work->func = dmWiiExpOpeFadeOutWaitFunc;

		// フェード開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEOUT,
									DMD_WII_EO_FADEOUT_TIME, TRUE);

		// スキップ許可OFF
		logo_work->flag &= ~DMD_WII_EO_FLAG_SKIP_OK;
		return;
	}
#endif

	if (logo_work->timer == DMD_WII_EO_DISP_MIN_TIME) {
		// スキップ許可
		logo_work->flag |= DMD_WII_EO_FLAG_SKIP_OK;
	}
}

// ==========================================================================
// dmWiiExpOpeFadeOutWaitFunc
/*!
 *	フェード待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmWiiExpOpeFadeOutWaitFunc(DMS_WII_EO_WORK *logo_work)
{
	if (IzFadeIsEnd()) {
		// 演出終了
		logo_work->flag |= DMD_WII_EO_FLAG_END;
	}
}


// ==========================================================================
// リリース待機
// ==========================================================================
// ==========================================================================
// dmWiiExpOpePreEndWait
/*!
 *	データロード終了待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmWiiExpOpePreEndWait(MTS_TASK_TCB *tcb)
{
	DMS_WII_EO_WORK	*logo_work;
	logo_work = (DMS_WII_EO_WORK*)mtTaskGetTcbWork(tcb);

	logo_work->timer++;
	if (logo_work->timer > 2) {
		// アクション破棄
		dmWiiExpOpeActionDelete(logo_work);

		// データフラッシュ
		DmWiiExpOpeFlush();

		// データフラッシュ待機へ
		mtTaskChangeTcbProcedure(tcb, dmWiiExpOpeFlushWaitFunc);
	}
}

// ==========================================================================
// dmWiiExpOpeFlushWaitFunc
/*!
 *	データフラッシュ待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmWiiExpOpeFlushWaitFunc(MTS_TASK_TCB *tcb)
{
	if (!DmWiiExpOpeFlushCheck()) {
		return;
	}

	// データフリリース
	DmWiiExpOpeRelease();
	// リリース処理待機へ
	mtTaskChangeTcbProcedure(tcb, dmWiiExpOpeRelesehWaitFunc);
}

// ==========================================================================
// dmWiiExpOpeRelesehWaitFunc
/*!
 *	データリリース待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmWiiExpOpeRelesehWaitFunc(MTS_TASK_TCB *tcb)
{
	if (!DmWiiExpOpeReleaseCheck()) {
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
// dmWiiExpOpeLoadPostFunc
/*!
 *	データロード後処理
 *
 *	@param	context	[in]	コンテキスト
 */
// ==========================================================================
void dmWiiExpOpeLoadPostFunc(DMS_LOGO_COM_LOAD_CONTEXT *context)
{
//	s32				i;
//	AMS_AMB_HEADER	*amb_header;

	dm_wii_exp_ope_data[context->no] = context->fs_req->buf;
	context->fs_req->buf = NULL;

	// コンバート
	amBindConvertAll((u8*)dm_wii_exp_ope_data[context->no]);

//	if (context->no == DMD_WII_EO_DATA_LOCAL_AMA_SET_AMB) {
//		void	*tex_amb;
//		tex_amb = amBindGet((AMS_AMB_HEADER*)dm_wii_exp_ope_data[context->no],
//								IDB_D_LOGO_SONIC_D_LOGO_SONIC_AMB);
//		amBindConvertAll((u8*)tex_amb);
//	}
}

// ==========================================================================
// データビルド
// ==========================================================================
// ==========================================================================
// dmWiiExpOpeDataBuildMain
/*!
 *	データビルドメイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmWiiExpOpeDataBuildMain(MTS_TASK_TCB *tcb)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;
	BOOL		b_sts = TRUE;

	// テクスチャビルド
	aos_tex = dm_wii_exp_ope_aos_tex;
	for (i = 0; i < DMD_WII_EO_AOSTEX_MAX; i++, aos_tex++) {
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
	dm_wii_exp_ope_build_state = TRUE;
}

// ==========================================================================
// dmWiiExpOpeDataBuildDest
/*!
 *	データビルドデストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmWiiExpOpeDataBuildDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dm_wii_exp_ope_build_tcb = NULL;
}

// ==========================================================================
// データフラッシュ
// ==========================================================================
// ==========================================================================
// dmWiiExpOpeDataFlushMain
/*!
 *	データフラッシュメイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmWiiExpOpeDataFlushMain(MTS_TASK_TCB *tcb)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;
	BOOL		b_sts = TRUE;

	// テクスチャフラッシュ
	aos_tex = dm_wii_exp_ope_aos_tex;
	for (i = 0; i < DMD_WII_EO_AOSTEX_MAX; i++, aos_tex++) {
		if (!AoTexIsReleased(aos_tex)) {
			b_sts = FALSE;
		}
	}

	// フラッシュ待機
	if (!b_sts) {
		return;
	}
	
	// テクスチャ管理バッファ解放
	amMemFree(dm_wii_exp_ope_aos_tex);
	dm_wii_exp_ope_aos_tex = NULL;

	// フラッシュ終了
	mtTaskClearTcb(tcb);
	dm_wii_exp_ope_build_state = FALSE;
}

// ==========================================================================
// dmWiiExpOpeDataFlushDest
/*!
 *	データフラッシュデストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmWiiExpOpeDataFlushDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dm_wii_exp_ope_flush_tcb = NULL;
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
