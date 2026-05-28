// ==========================================================================
/*!
  @file dmLogoESRB.cpp
  @brief ESRB(レーティング)ロゴ リージョン:北米

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: dmLogoESRB.cpp 20 2011-04-22 12:46:46Z thamada $
  $Date:: 2011-04-22 21:46:46 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"

#if (defined(HOG_RGN_US) && !defined(HOG_RGN_KR)) || _XBOX || _PC
#include "gs.h"
#include "gsMainSys.h"
#include "gsEnvironment.h"
#include "gsTrial.h"
#include "izFade.h"

#include "dmLogoCom.h"

#include "dmLogoESRB.h"

// データヘッダ
#include "common/arc/D_LOGO_ESRB.HMB"
#include "common/ace/D_LOGO_ESRB.HMA"

//----- Definitions ---------------------------------------------------------
/* タスク設定 */
#define DMD_LOGO_ESRB_TASK_PRIO_DATA_LOAD		(0x1000)		//!< ロードタスクプライオリティ
#define DMD_LOGO_ESRB_TASK_GROUP_DATA_LOAD		(0)				//!< ロードタスクグループ
#define DMD_LOGO_ESRB_TASK_PRIO_DATA_BUILD		(0x1000)		//!< ビルドタスクプライオリティ
#define DMD_LOGO_ESRB_TASK_GROUP_DATA_BUILD		(0)				//!< ビルドタスクグループ
#define DMD_LOGO_ESRB_TASK_PRIO_DATA_FLUSH		(0x1000)		//!< フラッシュタスクプライオリティ
#define DMD_LOGO_ESRB_TASK_GROUP_DATA_FLUSH		(0)				//!< フラッシュタスクグループ
#define DMD_LOGO_ESRB_TASK_PRIO_DATA_RELEASE	(0x1000)		//!< リリースタスクプライオリティ
#define DMD_LOGO_ESRB_TASK_GROUP_DATA_RELEASE	(0)				//!< リリースタスクグループ

#define DMD_LOGO_ESRB_TASK_PRIO_MAIN			(0x1000)		//!< メイン処理プライオリティ
#define DMD_LOGO_ESRB_TASK_GROUP_MAIN			(0)				//!< メイン処理グループ

/* 演出設定 */
#define DMD_LOGO_ESRB_PRE_FADEIN_TIME		(4)			//!< 色変更フェードイン時間
#define DMD_LOGO_ESRB_FADEIN_TIME			(60)		//!< フェードイン時間
#define DMD_LOGO_ESRB_DISP_TIME				(240)		//!< ロゴ表示時間
#define DMD_LOGO_ESRB_FADEOUT_TIME			(60)		//!< フェードアウト時間
#define DMD_LOGO_ESRB_POST_FADEOUT_TIME		(4)			//!< 色変更フェードアウト時間

#define DMD_LOGO_ESRB_SKIP_KEY				(GSD_KEY_DECIDE)	//!< スキップキー

/* データ設定 */
/// 読み込みデータ
enum {
//	DMD_LOGO_ESRB_DATA_COM_AMA_SET_AMB	= 0,		//!< 共通 AMAデータセットAMB

	DMD_LOGO_ESRB_DATA_LOCAL_AMA_SET_AMB	= 0,	//!< ローカライズ AMAデータセットAMB

	DMD_LOGO_ESRB_DATA_MAX
};

/// アクションデータ
enum {
	DMD_LOGO_ESRB_ACT_BASE	= 0,
	DMD_LOGO_ESRB_ACT_LOGO,

	DMD_LOGO_ESRB_ACT_MAX
};

/// AOSテクスチャタイプ
enum {
	DMD_LOGO_ESRB_AOSTEX_LOCAL	= 0,

	DMD_LOGO_ESRB_AOSTEX_MAX
};

/// ESRBロゴワーク
typedef struct tag_DMS_LOGO_ESRB_WORK {
	u32				flag;
	s32				timer;

//	AOS_TEXTURE		aos_tex[DMD_LOGO_ESRB_AOSTEX_MAX];	//!< テクスチャ
	AOS_ACTION		*act[DMD_LOGO_ESRB_ACT_MAX];		//!< アクション

	void (*func)(struct tag_DMS_LOGO_ESRB_WORK*);

} DMS_LOGO_ESRB_WORK;

//#define DMD_LOGO_ESRB_FLAG_SKIP_OK		(0x00000001)	//!< スキップ許可フラグ
//#define DMD_LOGO_ESRB_FLAG_SKIP			(0x00000002)	//!< スキップ実行済み
#define DMD_LOGO_ESRB_FLAG_END			(0x00000004)	//!< 演出終了

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void dmLogoESRBLoadWait(MTS_TASK_TCB *tcb);
static void dmLogoESRBBuildWait(MTS_TASK_TCB *tcb);

static void dmLogoESRBActionCreate(DMS_LOGO_ESRB_WORK *logo_work);
static void dmLogoESRBActionDelete(DMS_LOGO_ESRB_WORK *logo_work);

static void dmLogoESRBStart(void);
static void dmLogoESRBMainFunc(MTS_TASK_TCB *tcb);
#if _WII
static void dmLogoESRBPreFadeInWaitFunc(DMS_LOGO_ESRB_WORK *logo_work);
#endif
static void dmLogoESRBFadeInWaitFunc(DMS_LOGO_ESRB_WORK *logo_work);
static void dmLogoESRBDispWaitFunc(DMS_LOGO_ESRB_WORK *logo_work);
static void dmLogoESRBFadeOutWaitFunc(DMS_LOGO_ESRB_WORK *logo_work);

static void dmLogoESRBPreEndWait(MTS_TASK_TCB *tcb);
static void dmLogoESRBFlushWaitFunc(MTS_TASK_TCB *tcb);
static void dmLogoESRBRelesehWaitFunc(MTS_TASK_TCB *tcb);

static void dmLogoESRBLoadPostFunc(DMS_LOGO_COM_LOAD_CONTEXT *context);
static void dmLogoESRBDataBuildMain(MTS_TASK_TCB *tcb);
static void dmLogoESRBDataBuildDest(MTS_TASK_TCB *tcb);
static void dmLogoESRBDataFlushMain(MTS_TASK_TCB *tcb);
static void dmLogoESRBDataFlushDest(MTS_TASK_TCB *tcb);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static MTS_TASK_TCB *dm_logo_esrb_load_tcb = NULL;		//!< データロードTCB
static MTS_TASK_TCB *dm_logo_esrb_build_tcb = NULL;		//!< データビルドTCB
static MTS_TASK_TCB *dm_logo_esrb_flush_tcb = NULL;		//!< データフラッシュTCB
//static MTS_TASK_TCB *dm_logo_esrb_release_tcb = NULL;	//!< データリリースTCB

static AOS_TEXTURE *dm_logo_esrb_aos_tex = NULL;	//!< テクスチャ
static BOOL dm_logo_esrb_build_state = FALSE;		//!< ビルド状況

/// データ
void *dm_logo_esrb_data[DMD_LOGO_ESRB_DATA_MAX] = {NULL};

#if 0
/// 共通データファイル名リスト
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_esrb_com_fileinfo_list[] = {
	{GSS_BASE_PATH "DEMO/LOGO/D_LOGO_ESRB.AMB", dmLogoESRBLoadPostFunc},
	// 以下その他
};

/// データファイル数
static const s32 dm_logo_esrb_com_file_num = sizeof(dm_logo_esrb_com_fileinfo_list) / sizeof(DMS_LOGO_COM_LOAD_FILE_INFO);
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
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_esrb_localize_fileinfo_list_jp[] = {
	{GSS_BASE_PATH "DEMO/LOGO/D_LOGO_ESRB_US.AMB", dmLogoESRBLoadPostFunc},		// この言語の割り当てはないため、英語にしておく
};
/// ローカライズ関連データファイル名リスト 英語
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_esrb_localize_fileinfo_list_us[] = {
	{GSS_BASE_PATH "DEMO/LOGO/D_LOGO_ESRB_US.AMB", dmLogoESRBLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト フランス
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_esrb_localize_fileinfo_list_fr[] = {
	{GSS_BASE_PATH "DEMO/LOGO/D_LOGO_ESRB_FR.AMB", dmLogoESRBLoadPostFunc},
};
/// ローカライズ関連データファイル名リスト イタリア
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_esrb_localize_fileinfo_list_it[] = {
	{GSS_BASE_PATH "DEMO/LOGO/D_LOGO_ESRB_US.AMB", dmLogoESRBLoadPostFunc},		// この言語の割り当てはないため、英語にしておく
};
/// ローカライズ関連データファイル名リスト ドイツ
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_esrb_localize_fileinfo_list_ge[] = {
	{GSS_BASE_PATH "DEMO/LOGO/D_LOGO_ESRB_US.AMB", dmLogoESRBLoadPostFunc},		// この言語の割り当てはないため、英語にしておく
};
/// ローカライズ関連データファイル名リスト スペイン
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_esrb_localize_fileinfo_list_sp[] = {
	{GSS_BASE_PATH "DEMO/LOGO/D_LOGO_ESRB_SP.AMB", dmLogoESRBLoadPostFunc},
};
/// ローカライズ関連データファイル名リストテーブル
static const DMS_LOGO_COM_LOAD_FILE_INFO *dm_logo_esrb_localize_fileinfo_list_tbl[] = {
	dm_logo_esrb_localize_fileinfo_list_jp,
	dm_logo_esrb_localize_fileinfo_list_us,
	dm_logo_esrb_localize_fileinfo_list_fr,
	dm_logo_esrb_localize_fileinfo_list_it,
	dm_logo_esrb_localize_fileinfo_list_ge,
	dm_logo_esrb_localize_fileinfo_list_sp,
};

/// ローカライズ関連データファイル数
static const s32 dm_logo_esrb_localize_file_num = sizeof(dm_logo_esrb_localize_fileinfo_list_jp) / sizeof(DMS_LOGO_COM_LOAD_FILE_INFO);


/// 描画オブジェクト
//OBS_ACTION3D_NN_WORK *dm_logo_esrb_obj_3d_list = NULL;

/// アクション テクスチャ割り当てテーブル
static const u8 dm_logo_esrb_tex_id_tbl[DMD_LOGO_ESRB_ACT_MAX] = {
	DMD_LOGO_ESRB_AOSTEX_LOCAL,
	DMD_LOGO_ESRB_AOSTEX_LOCAL,
};

	

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// 初期化
// ==========================================================================
// ==========================================================================
// DmLogoESRBInit
/*!
 *	ESRBロゴ 初期化
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void DmLogoESRBInit(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	if (DmLogoESRBBuildCheck()) {
		// データビルド済み
		// 演出開始
		dmLogoESRBStart();
	}
	else {
#if defined (MTD_DEBUG)
		if (DmLogoESRBLoadCheck()) {
			// 中途半端にロードが始まっている	
			MTM_ASSERT(0);
		}
#endif
		// データロード開始
		MTM_TASK_MAKE_TCB(dmLogoESRBLoadWait, NULL,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_LOGO_ESRB_TASK_PRIO_DATA_LOAD, DMD_LOGO_ESRB_TASK_GROUP_DATA_LOAD,
						0/*work_size*/, "DM_LESRB_LW");

		DmLogoESRBLoad();
	}
}


// ==========================================================================
// データロード
// ==========================================================================
// ==========================================================================
// DmLogoESRBLoad
/*!
 *	ESRBロゴデータロード
 *
 */
// ==========================================================================
void DmLogoESRBLoad(void)
{
	GSE_LANGUAGE	language = GsEnvGetLanguage();

	// ロード処理生成
	DmLogoComLoadFileCreate(&dm_logo_esrb_load_tcb);

	//// 共通データ
	//DmLogoComLoadFileReg(dm_logo_esrb_load_tcb, &dm_logo_esrb_com_fileinfo_list[0], dm_logo_esrb_com_file_num);

	// ローカライズ関連データ
	DmLogoComLoadFileReg(dm_logo_esrb_load_tcb, dm_logo_esrb_localize_fileinfo_list_tbl[language], dm_logo_esrb_localize_file_num);

	// ロードチェック開始
	DmLogoComLoadFileStart(dm_logo_esrb_load_tcb);
}

// ==========================================================================
// DmLogoESRBLoadCheck
/*!
 *	ESRBロゴデータロード ロード終了チェック
 *
 *	@return	TRUE : ロード終了
 */
// ==========================================================================
BOOL DmLogoESRBLoadCheck(void)
{
	if ((dm_logo_esrb_load_tcb == NULL) && (dm_logo_esrb_data[0] != NULL)) {
		// データロードタスクがなくて 1つ目のデータが読み込まれている
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データビルド
// ==========================================================================
// ==========================================================================
// DmLogoESRBBuild
/*!
 *	ESRBロゴデータビルド
 *
 */
// ==========================================================================
void DmLogoESRBBuild(void)
{
	s32			i;
	void		*tex_amb[DMD_LOGO_ESRB_AOSTEX_MAX];
	AOS_TEXTURE	*aos_tex;

	MTM_ASSERT(DmLogoESRBLoadCheck() == TRUE);
	MTM_ASSERT(dm_logo_esrb_build_state == FALSE);
	MTM_ASSERT(dm_logo_esrb_aos_tex == NULL);

	// ビルド処理生成
	dm_logo_esrb_build_tcb = MTM_TASK_MAKE_TCB(dmLogoESRBDataBuildMain, dmLogoESRBDataBuildDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_LOGO_ESRB_TASK_PRIO_DATA_BUILD, DMD_LOGO_ESRB_TASK_GROUP_DATA_BUILD,
						0/*work_size*/, "DM_LESRB_BUILD");

	// テクスチャ管理バッファ取得
	dm_logo_esrb_aos_tex = (AOS_TEXTURE*)amMemAlloc(sizeof(AOS_TEXTURE) * DMD_LOGO_ESRB_AOSTEX_MAX);
	MI_CpuClear8(dm_logo_esrb_aos_tex, sizeof(AOS_TEXTURE) * DMD_LOGO_ESRB_AOSTEX_MAX);

	// テクスチャビルド
	tex_amb[DMD_LOGO_ESRB_AOSTEX_LOCAL] =
				amBindGet((AMS_AMB_HEADER*)dm_logo_esrb_data[DMD_LOGO_ESRB_DATA_LOCAL_AMA_SET_AMB],
									IDB_D_LOGO_ESRB_US_D_LOGO_ESRB_US_AMB);

	aos_tex = dm_logo_esrb_aos_tex;
	for (i = 0; i < DMD_LOGO_ESRB_AOSTEX_MAX; i++, aos_tex++) {
		AoTexBuild(aos_tex, tex_amb[i]);
		AoTexLoad(aos_tex);
	}
}

// ==========================================================================
// DmLogoESRBBuildCheck
/*!
 *	ESRBロゴデータビルド ビルド終了チェック
 *
 *	@return	TRUE : ビルド終了
 */
// ==========================================================================
BOOL DmLogoESRBBuildCheck(void)
{
	if (dm_logo_esrb_build_state) {
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データフラッシュ
// ==========================================================================
// ==========================================================================
// DmLogoESRBFlush
/*!
 *	ESRBロゴデータフラッシュ
 *
 */
// ==========================================================================
void DmLogoESRBFlush(void)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;

	MTM_ASSERT(dm_logo_esrb_aos_tex);
	MTM_ASSERT(dm_logo_esrb_build_state == TRUE);
	MTM_ASSERT(dm_logo_esrb_flush_tcb == NULL);

	// フラッシュ処理生成
	dm_logo_esrb_flush_tcb = MTM_TASK_MAKE_TCB(dmLogoESRBDataFlushMain, dmLogoESRBDataFlushDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_LOGO_ESRB_TASK_PRIO_DATA_FLUSH, DMD_LOGO_ESRB_TASK_GROUP_DATA_FLUSH,
						0/*work_size*/, "DM_LESRB_FLUSH");

	// テクスチャフラッシュ
	aos_tex = dm_logo_esrb_aos_tex;
	for (i = 0; i < DMD_LOGO_ESRB_AOSTEX_MAX; i++, aos_tex++) {
		AoTexRelease(aos_tex);
	}
}

// ==========================================================================
// DmLogoESRBFlushCheck
/*!
 *	ESRBロゴデータフラッシュ フラッシュ終了チェック
 *
 *	@return	TRUE : フラッシュ終了
 */
// ==========================================================================
BOOL DmLogoESRBFlushCheck(void)
{
	if (!dm_logo_esrb_build_state) {
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データリリース
// ==========================================================================
// ==========================================================================
// DmLogoESRBRelease
/*!
 *	ESRBロゴデータリリース
 */
// ==========================================================================
void DmLogoESRBRelease(void)
{
	s32	i;

	for (i = 0; i < DMD_LOGO_ESRB_DATA_MAX; i++) {
		if (dm_logo_esrb_data[i]) {
			amMemFree(dm_logo_esrb_data[i]);
		}
		dm_logo_esrb_data[i] = NULL;
	}
}

// ==========================================================================
// DmLogoESRBReleaseCheck
/*!
 *	ESRBロゴデータリリース リリース終了チェック
 *
 *	@return	TRUE : リリース終了
 */
// ==========================================================================
BOOL DmLogoESRBReleaseCheck(void)
{
	if ((dm_logo_esrb_load_tcb == NULL) && (dm_logo_esrb_data[0] == NULL)) {
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
// dmLogoESRBLoadWait
/*!
 *	データロード待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoESRBLoadWait(MTS_TASK_TCB *tcb)
{
	if (DmLogoESRBLoadCheck()) {
		// データビルド開始
		DmLogoESRBBuild();

		mtTaskChangeTcbProcedure(tcb, dmLogoESRBBuildWait);
	}
}

// ==========================================================================
// dmLogoESRBBuildWait
/*!
 *	データビルド待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoESRBBuildWait(MTS_TASK_TCB *tcb)
{
	if (DmLogoESRBBuildCheck()) {

		// データロード終了
		mtTaskClearTcb(tcb);

		// 演出開始
		dmLogoESRBStart();
	}
}

// ==========================================================================
// アクション生成・破棄
// ==========================================================================
// ==========================================================================
// dmLogoESRBActionCreate
/*!
 *	アクション生成
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoESRBActionCreate(DMS_LOGO_ESRB_WORK *logo_work)
{
	s32		i;
	void	*ama;

	// アクション構築
	ama = amBindGet((AMS_AMB_HEADER*)dm_logo_esrb_data[DMD_LOGO_ESRB_DATA_LOCAL_AMA_SET_AMB],
				IDB_D_LOGO_ESRB_US_D_LOGO_ESRB_AMA);

	for (i = 0; i < DMD_LOGO_ESRB_ACT_MAX; i++) {
		// アクション構築
		AoActSetTexture(AoTexGetTexList(dm_logo_esrb_aos_tex + dm_logo_esrb_tex_id_tbl[i]));
		logo_work->act[i] = AoActCreate(ama, i);
	}
}

// ==========================================================================
// dmLogoESRBActionDelete
/*!
 *	アクション破棄
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoESRBActionDelete(DMS_LOGO_ESRB_WORK *logo_work)
{
	s32		i;

	for (i = 0; i < DMD_LOGO_ESRB_ACT_MAX; i++) {
		AoActDelete(logo_work->act[i]);
	}
}

// ==========================================================================
// メイン処理
// ==========================================================================
// ==========================================================================
// dmLogoESRBStart
/*!
 *	ESRBロゴ開始
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoESRBStart(void)
{
	MTS_TASK_TCB		*tcb;
	DMS_LOGO_ESRB_WORK *logo_work;

	NNS_RGBA		diffuse = { 1.0f, 1.0f, 1.0f, 1.0f};
	NNS_RGB			ambient = { 1.0f, 1.0f, 1.0f};

	// メイン処理開始
	tcb = MTM_TASK_MAKE_TCB(dmLogoESRBMainFunc, NULL,
						0/*flag*/, 0/*task_pause_level*/,
						DMD_LOGO_ESRB_TASK_PRIO_MAIN, DMD_LOGO_ESRB_TASK_GROUP_MAIN,
						sizeof(DMS_LOGO_ESRB_WORK), "DM_LESRB_MAIN");
	logo_work = (DMS_LOGO_ESRB_WORK*)mtTaskGetTcbWork(tcb);
	MI_CpuClear8(logo_work, sizeof(DMS_LOGO_ESRB_WORK));

	// 環境初期化
	nnSetPrimitive3DMaterial(&diffuse, &ambient, 1.0f);

	AoActSysSetDrawStateEnable(FALSE);

	// アクション初期化
	dmLogoESRBActionCreate(logo_work);

#if _WII
	// 色合わせフェード開始
	IzFadeInitEasyTask(IZE_FADE_SET_TYPE_TAKEOEVER,
				0xFF, 0xFF, 0xFF, 0xFF,
				0x00, 0x00, 0x00, 0xFF,
				DMD_LOGO_ESRB_PRE_FADEIN_TIME, TRUE);

	// 色合わせフェードイン待機へ
	logo_work->func = dmLogoESRBPreFadeInWaitFunc;
#else
	// フェード開始
	IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_BLACK_FADEIN,
								DMD_LOGO_ESRB_FADEIN_TIME, TRUE);

	// フェードイン待機へ
	logo_work->func = dmLogoESRBFadeInWaitFunc;
#endif
}


// ==========================================================================
// dmLogoESRBMainFunc
/*!
 *	メイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoESRBMainFunc(MTS_TASK_TCB *tcb)
{
	s32					i;
	DMS_LOGO_ESRB_WORK	*logo_work;
	float				update_frame;

	logo_work = (DMS_LOGO_ESRB_WORK*)mtTaskGetTcbWork(tcb);

	// 演出
	if (AoSysIsShowPlatformUI()) {
		if (IzFadeIsExe()) {
			IzFadeSetStopUpdate1Frame(NULL);
		}
	}
	else {
		if (logo_work->func) {
			logo_work->func(logo_work);
		}

#if 0
	// スキップチェック
	if ((logo_work->flag & DMD_LOGO_ESRB_FLAG_SKIP_OK) &&
				!(logo_work->flag & DMD_LOGO_ESRB_FLAG_SKIP)) {
		if (AoPadSomeoneStand(DMD_LOGO_ESRB_SKIP_KEY) >= 0) {
			// スキップ実行
			logo_work->flag |= DMD_LOGO_ESRB_FLAG_SKIP;

			if (IzFadeIsEnd()) {
				// フェード未実行
				// フェード開始
				IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEOUT,
											DMD_LOGO_ESRB_FADEOUT_TIME, TRUE);
			}
			// フェードアウトへ移行
			logo_work->func = dmLogoESRBFadeOutWaitFunc;
		}
	}
#endif

		if (logo_work->flag & DMD_LOGO_ESRB_FLAG_END) {
			// 演出終了

			// 前終了待機処理へ
			mtTaskChangeTcbProcedure(tcb, dmLogoESRBPreEndWait);
			logo_work->timer = 0;

#if 0
			// アクション破棄
			dmLogoESRBActionDelete(logo_work);
			// メイン処理破棄
			mtTaskClearTcb(tcb);

			// データのFlush, Releaseはメニューを抜ける時に一括で行う

			// 次のイベントへ
			SyChangeNextEvt();
#endif
			return;
		}
	}	// AoSysIsShowPlatformUI()

	// 描画
	update_frame = 0.f;
	if (!AoSysIsShowPlatformUI()) {
		// システムメニュー未表示時のみ更新
		update_frame = 1.f;
	}
	AoActSysSetDrawTaskPrio();	// 標準設定
	for (i = 0; i < DMD_LOGO_ESRB_ACT_MAX; i++) {
		AoActSetTexture(AoTexGetTexList(dm_logo_esrb_aos_tex + dm_logo_esrb_tex_id_tbl[i]));
		AoActUpdate(logo_work->act[i], update_frame);
		AoActDraw(logo_work->act[i]);
	}
}


// ==========================================================================
// 演出
// ==========================================================================
#if _WII
// ==========================================================================
// dmLogoESRBPreFadeInWaitFunc
/*!
 *	フェード待機 白から黒へ
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoESRBPreFadeInWaitFunc(DMS_LOGO_ESRB_WORK *logo_work)
{
	if (IzFadeIsEnd()) {
		// フェードイン待機
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_BLACK_FADEIN,
									DMD_LOGO_ESRB_FADEIN_TIME, TRUE);
		logo_work->func = dmLogoESRBFadeInWaitFunc;
	}
}
#endif

// ==========================================================================
// dmLogoESRBFadeInWaitFunc
/*!
 *	フェード待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoESRBFadeInWaitFunc(DMS_LOGO_ESRB_WORK *logo_work)
{
	if (IzFadeIsEnd()) {
		// 表示待機へ移行
		logo_work->func = dmLogoESRBDispWaitFunc;
		logo_work->timer = 0;
	}
}

// ==========================================================================
// dmLogoESRBDispWaitFunc
/*!
 *	表示待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoESRBDispWaitFunc(DMS_LOGO_ESRB_WORK *logo_work)
{
	logo_work->timer++;
	if (logo_work->timer >= DMD_LOGO_ESRB_DISP_TIME) {
		// フェードアウトへ移行
		logo_work->func = dmLogoESRBFadeOutWaitFunc;

		// フェード開始
#if _PS3
		if (GsTrialIsTrial()) {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEOUT,
										DMD_LOGO_ESRB_FADEOUT_TIME, TRUE);
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_BLACK_FADEOUT,
										DMD_LOGO_ESRB_FADEOUT_TIME, TRUE);
		}
#else
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEOUT,
									DMD_LOGO_ESRB_FADEOUT_TIME, TRUE);
#endif

//		// スキップ許可OFF
//		logo_work->flag &= ~DMD_LOGO_ESRB_FLAG_SKIP_OK;
		return;
	}

//	if (logo_work->timer == 30) {
//		// スキップ許可
//		logo_work->flag |= DMD_LOGO_ESRB_FLAG_SKIP_OK;
//	}
}

// ==========================================================================
// dmLogoESRBFadeOutWaitFunc
/*!
 *	フェード待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoESRBFadeOutWaitFunc(DMS_LOGO_ESRB_WORK *logo_work)
{
	if (IzFadeIsEnd()) {
		// 演出終了
		logo_work->flag |= DMD_LOGO_ESRB_FLAG_END;
	}
}

// ==========================================================================
// リリース待機
// ==========================================================================
// ==========================================================================
// dmLogoESRBPreEndWait
/*!
 *	データロード終了待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoESRBPreEndWait(MTS_TASK_TCB *tcb)
{
	DMS_LOGO_ESRB_WORK	*logo_work;
	logo_work = (DMS_LOGO_ESRB_WORK*)mtTaskGetTcbWork(tcb);

	logo_work->timer++;
	if (logo_work->timer > 2) {
		// アクション破棄
		dmLogoESRBActionDelete(logo_work);

		// データフラッシュ
		DmLogoESRBFlush();

		// データフラッシュ待機へ
		mtTaskChangeTcbProcedure(tcb, dmLogoESRBFlushWaitFunc);
	}
}

// ==========================================================================
// dmLogoESRBFlushWaitFunc
/*!
 *	データフラッシュ待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoESRBFlushWaitFunc(MTS_TASK_TCB *tcb)
{
	if (!DmLogoESRBFlushCheck()) {
		return;
	}

	// データフリリース
	DmLogoESRBRelease();
	// リリース処理待機へ
	mtTaskChangeTcbProcedure(tcb, dmLogoESRBRelesehWaitFunc);
}

// ==========================================================================
// dmLogoESRBRelesehWaitFunc
/*!
 *	データリリース待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoESRBRelesehWaitFunc(MTS_TASK_TCB *tcb)
{
	if (!DmLogoESRBReleaseCheck()) {
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
// dmLogoESRBLoadPostFunc
/*!
 *	データロード後処理
 *
 *	@param	context	[in]	コンテキスト
 */
// ==========================================================================
void dmLogoESRBLoadPostFunc(DMS_LOGO_COM_LOAD_CONTEXT *context)
{
//	s32				i;
//	AMS_AMB_HEADER	*amb_header;

	dm_logo_esrb_data[context->no] = context->fs_req->buf;
	context->fs_req->buf = NULL;

	// コンバート
	amBindConvertAll((u8*)dm_logo_esrb_data[context->no]);

//	if (context->no == DMD_LOGO_ESRB_DATA_LOCAL_AMA_SET_AMB) {
//		void	*tex_amb;
//		tex_amb = amBindGet((AMS_AMB_HEADER*)dm_logo_esrb_data[context->no],
//								IDB_D_LOGO_SONIC_D_LOGO_SONIC_AMB);
//		amBindConvertAll((u8*)tex_amb);
//	}
}

// ==========================================================================
// データビルド
// ==========================================================================
// ==========================================================================
// dmLogoESRBDataBuildMain
/*!
 *	データビルドメイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoESRBDataBuildMain(MTS_TASK_TCB *tcb)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;
	BOOL		b_sts = TRUE;

	// テクスチャビルド
	aos_tex = dm_logo_esrb_aos_tex;
	for (i = 0; i < DMD_LOGO_ESRB_AOSTEX_MAX; i++, aos_tex++) {
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
	dm_logo_esrb_build_state = TRUE;
}

// ==========================================================================
// dmLogoESRBDataBuildDest
/*!
 *	データビルドデストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoESRBDataBuildDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dm_logo_esrb_build_tcb = NULL;
}

// ==========================================================================
// データフラッシュ
// ==========================================================================
// ==========================================================================
// dmLogoESRBDataFlushMain
/*!
 *	データフラッシュメイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoESRBDataFlushMain(MTS_TASK_TCB *tcb)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;
	BOOL		b_sts = TRUE;

	// テクスチャフラッシュ
	aos_tex = dm_logo_esrb_aos_tex;
	for (i = 0; i < DMD_LOGO_ESRB_AOSTEX_MAX; i++, aos_tex++) {
		if (!AoTexIsReleased(aos_tex)) {
			b_sts = FALSE;
		}
	}

	// フラッシュ待機
	if (!b_sts) {
		return;
	}
	
	// テクスチャ管理バッファ解放
	amMemFree(dm_logo_esrb_aos_tex);
	dm_logo_esrb_aos_tex = NULL;

	// フラッシュ終了
	mtTaskClearTcb(tcb);
	dm_logo_esrb_build_state = FALSE;
}

// ==========================================================================
// dmLogoESRBDataFlushDest
/*!
 *	データフラッシュデストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoESRBDataFlushDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dm_logo_esrb_flush_tcb = NULL;
}


void DmLogoESRBStaticVarInit(void)
{
	dm_logo_esrb_load_tcb = NULL;		//!< データロードTCB
	dm_logo_esrb_build_tcb = NULL;		//!< データビルドTCB
	dm_logo_esrb_flush_tcb = NULL;		//!< データフラッシュTCB
	//dm_logo_esrb_release_tcb = NULL;	//!< データリリースTCB
	
	dm_logo_esrb_aos_tex = NULL;		//!< テクスチャ
	dm_logo_esrb_build_state = FALSE;	//!< ビルド状況
	
	/// データ
	memset(dm_logo_esrb_data, 0, sizeof(dm_logo_esrb_data));
}


#endif // #if (defined(HOG_RGN_US) && !defined(HOG_RGN_KR)) || _XBOX || _PC
// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
