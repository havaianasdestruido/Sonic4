// ==========================================================================
/*!
  @file dmLogoSonic.cpp
  @brief ソニックチームロゴ

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: dmLogoSonic.cpp 20 2011-04-22 12:46:46Z thamada $
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

#if _WII
#include "gmPlyLod.h"
#endif

#include "dmLogoCom.h"

#include "dmLogoSonic.h"

// データヘッダ
#include "common/arc/D_LOGO_SONIC.HMB"

//mpp -------------------------------
#include "mppUtil.h"
#include "mppCheckPointStorage.h"



//----- Definitions ---------------------------------------------------------
/* タスク設定 */
#define DMD_LOGO_SONIC_TASK_PRIO_DATA_LOAD		(0x1000)		//!< ロードタスクプライオリティ
#define DMD_LOGO_SONIC_TASK_GROUP_DATA_LOAD		(0)				//!< ロードタスクグループ
#define DMD_LOGO_SONIC_TASK_PRIO_DATA_BUILD		(0x1000)		//!< ビルドタスクプライオリティ
#define DMD_LOGO_SONIC_TASK_GROUP_DATA_BUILD	(0)				//!< ビルドタスクグループ
#define DMD_LOGO_SONIC_TASK_PRIO_DATA_FLUSH		(0x1000)		//!< フラッシュタスクプライオリティ
#define DMD_LOGO_SONIC_TASK_GROUP_DATA_FLUSH	(0)				//!< フラッシュタスクグループ
#define DMD_LOGO_SONIC_TASK_PRIO_DATA_RELEASE	(0x1000)		//!< リリースタスクプライオリティ
#define DMD_LOGO_SONIC_TASK_GROUP_DATA_RELEASE	(0)				//!< リリースタスクグループ

#define DMD_LOGO_SONIC_TASK_PRIO_MAIN			(0x1000)		//!< メイン処理プライオリティ
#define DMD_LOGO_SONIC_TASK_GROUP_MAIN			(0)				//!< メイン処理グループ

/* 演出設定 */
#define DMD_LOGO_SONIC_FADEIN_TIME			(60)		//!< フェードイン時間
#define DMD_LOGO_SONIC_DISP_TIME			(120)		//!< ロゴ表示時間
#define DMD_LOGO_SONIC_FADEOUT_TIME			(60)		//!< フェードアウト時間
#if _IPHONE
#define DMD_LOGO_SONIC_FADEOUT_TIME_SKIP	(DMD_LOGO_SONIC_FADEOUT_TIME / 6)	//!< スキップ時フェードアウト時間
#endif //_IPHONE

#define DMD_LOGO_SONIC_SKIP_KEY				(GSD_KEY_DECIDE)	//!< スキップキー

/* データ設定 */
/// 読み込みデータ
enum {
	DMD_LOGO_SONIC_DATA_COM_AMA_SET_AMB	= 0,	//!< 共通 AMAデータセットAMB

//	DMD_LOGO_SONIC_DATA_LOCAL_AMA_SET_AMB,		//!< ローカライズ AMAデータセットAMB

	DMD_LOGO_SONIC_DATA_MAX
};

/// アクションデータ
enum {
	DMD_LOGO_SONIC_ACT_COM_BASE	= 0,
	DMD_LOGO_SONIC_ACT_COM_LOGO,

	DMD_LOGO_SONIC_ACTMAX
};

/// AOSテクスチャタイプ
enum {
	DMD_LOGO_SONIC_AOSTEX_COM	= 0,

	DMD_LOGO_SONIC_AOSTEX_MAX
};

/// ソニックチームロゴワーク
typedef struct tag_DMS_LOGO_SONIC_WORK {
	u32				flag;
	s32				timer;

//	AOS_TEXTURE		aos_tex[DMD_LOGO_SONIC_AOSTEX_MAX];	//!< テクスチャ
	AOS_ACTION		*act[DMD_LOGO_SONIC_ACTMAX];		//!< アクション

	void (*func)(struct tag_DMS_LOGO_SONIC_WORK*);

} DMS_LOGO_SONIC_WORK;

#define DMD_LOGO_SONIC_FLAG_SKIP_OK		(0x00000001)	//!< スキップ許可フラグ
#define DMD_LOGO_SONIC_FLAG_SKIP		(0x00000002)	//!< スキップ実行済み
#define DMD_LOGO_SONIC_FLAG_END			(0x00000004)	//!< 演出終了

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void dmLogoSonicLoadWait(MTS_TASK_TCB *tcb);
static void dmLogoSonicBuildWait(MTS_TASK_TCB *tcb);

static void dmLogoSonicActionCreate(DMS_LOGO_SONIC_WORK *logo_work);
static void dmLogoSonicActionDelete(DMS_LOGO_SONIC_WORK *logo_work);

static void dmLogoSonicStart(void);
static void dmLogoSonicMainFunc(MTS_TASK_TCB *tcb);
static void dmLogoSonicFadeInWaitFunc(DMS_LOGO_SONIC_WORK *logo_work);
static void dmLogoSonicDispWaitFunc(DMS_LOGO_SONIC_WORK *logo_work);
static void dmLogoSonicFadeOutWaitFunc(DMS_LOGO_SONIC_WORK *logo_work);

static void dmLogoSonicPreEndWait(MTS_TASK_TCB *tcb);
static void dmLogoSonicFlushWaitFunc(MTS_TASK_TCB *tcb);
static void dmLogoSonicRelesehWaitFunc(MTS_TASK_TCB *tcb);

static void dmLogoSonicLoadPostFunc(DMS_LOGO_COM_LOAD_CONTEXT *context);
static void dmLogoSonicDataBuildMain(MTS_TASK_TCB *tcb);
static void dmLogoSonicDataBuildDest(MTS_TASK_TCB *tcb);
static void dmLogoSonicDataFlushMain(MTS_TASK_TCB *tcb);
static void dmLogoSonicDataFlushDest(MTS_TASK_TCB *tcb);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static MTS_TASK_TCB *dm_logo_sonic_load_tcb = NULL;		//!< データロードTCB
static MTS_TASK_TCB *dm_logo_sonic_build_tcb = NULL;		//!< データビルドTCB
static MTS_TASK_TCB *dm_logo_sonic_flush_tcb = NULL;		//!< データフラッシュTCB
//static MTS_TASK_TCB *dm_logo_sonic_release_tcb = NULL;	//!< データリリースTCB

static AOS_TEXTURE *dm_logo_sonic_aos_tex = NULL;	//!< テクスチャ
static BOOL dm_logo_sonic_build_state = FALSE;		//!< ビルド状況

/// 共通データファイル名リスト
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_sonic_com_fileinfo_list[] = {
	// Zone1遠景
	{GSS_BASE_PATH "DEMO/LOGO/D_LOGO_SONIC.AMB", dmLogoSonicLoadPostFunc},
	// 以下その他
};

/// データ
void *dm_logo_sonic_data[DMD_LOGO_SONIC_DATA_MAX] = {NULL};

/// データファイル数
static const s32 dm_logo_sonic_com_file_num = sizeof(dm_logo_sonic_com_fileinfo_list) / sizeof(DMS_LOGO_COM_LOAD_FILE_INFO);

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
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_sonic_localize_fileinfo_list_jp[] = {
	{"", NULL},
};
/// ローカライズ関連データファイル名リスト 英語
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_sonic_localize_fileinfo_list_us[] = {
	{"", NULL},
};
/// ローカライズ関連データファイル名リスト フランス
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_sonic_localize_fileinfo_list_fr[] = {
	{"", NULL},
};
/// ローカライズ関連データファイル名リスト イタリア
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_sonic_localize_fileinfo_list_it[] = {
	{"", NULL},
};
/// ローカライズ関連データファイル名リスト ドイツ
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_sonic_localize_fileinfo_list_ge[] = {
	{"", NULL},
};
/// ローカライズ関連データファイル名リスト スペイン
static const DMS_LOGO_COM_LOAD_FILE_INFO dm_logo_sonic_localize_fileinfo_list_sp[] = {
	{"", NULL},
};
/// ローカライズ関連データファイル名リストテーブル
static const DMS_LOGO_COM_LOAD_FILE_INFO *dm_logo_sonic_localize_fileinfo_list_tbl[] = {
	dm_logo_sonic_localize_fileinfo_list_jp,
	dm_logo_sonic_localize_fileinfo_list_us,
	dm_logo_sonic_localize_fileinfo_list_fr,
	dm_logo_sonic_localize_fileinfo_list_it,
	dm_logo_sonic_localize_fileinfo_list_ge,
	dm_logo_sonic_localize_fileinfo_list_sp,
};

/// ローカライズ関連データファイル数
static const s32 dm_logo_sonic_localize_file_num = sizeof(dm_logo_sonic_localize_fileinfo_list_jp) / sizeof(DMS_LOGO_COM_LOAD_FILE_INFO);
#endif

/// 描画オブジェクト
//OBS_ACTION3D_NN_WORK *dm_logo_sonic_obj_3d_list = NULL;

/// アクション テクスチャ割り当てテーブル
static const u8 dm_logo_sonic_tex_id_tbl[DMD_LOGO_SONIC_ACTMAX] = {
	DMD_LOGO_SONIC_AOSTEX_COM,
};

	

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// 初期化
// ==========================================================================
// ==========================================================================
// DmLogoSonicInit
/*!
 *	ソニックチームロゴ 初期化
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void DmLogoSonicInit(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	if (DmLogoSonicBuildCheck()) {
		// データビルド済み
		// 演出開始
		dmLogoSonicStart();
	}
	else {
#if defined (MTD_DEBUG)
		if (DmLogoSonicLoadCheck()) {
			// 中途半端にロードが始まっている	
			MTM_ASSERT(0);
		}
#endif
		// データロード開始
		MTM_TASK_MAKE_TCB(dmLogoSonicLoadWait, NULL,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_LOGO_SONIC_TASK_PRIO_DATA_LOAD, DMD_LOGO_SONIC_TASK_GROUP_DATA_LOAD,
						0/*work_size*/, "DM_LSONT_LW");

		DmLogoSonicLoad();
	}
}


// ==========================================================================
// データロード
// ==========================================================================
// ==========================================================================
// DmLogoSonicLoad
/*!
 *	ソニックチームロゴデータロード
 *
 */
// ==========================================================================
void DmLogoSonicLoad(void)
{
	// ロード処理生成
	DmLogoComLoadFileCreate(&dm_logo_sonic_load_tcb);

	// 共通データ
	DmLogoComLoadFileReg(dm_logo_sonic_load_tcb, &dm_logo_sonic_com_fileinfo_list[0], dm_logo_sonic_com_file_num);

	// ローカライズ関連データ
	//DmLogoComLoadFileReg(dm_logo_sonic_load_tcb, dm_logo_sonic_localize_fileinfo_list_tbl[language], dm_logo_sonic_localize_file_num);

	// ロードチェック開始
	DmLogoComLoadFileStart(dm_logo_sonic_load_tcb);
}

// ==========================================================================
// DmLogoSonicLoadCheck
/*!
 *	ソニックチームロゴデータロード ロード終了チェック
 *
 *	@return	TRUE : ロード終了
 */
// ==========================================================================
BOOL DmLogoSonicLoadCheck(void)
{
	if ((dm_logo_sonic_load_tcb == NULL) && (dm_logo_sonic_data[0] != NULL)) {
		// データロードタスクがなくて 1つ目のデータが読み込まれている
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データビルド
// ==========================================================================
// ==========================================================================
// DmLogoSonicBuild
/*!
 *	ソニックチームロゴデータビルド
 *
 */
// ==========================================================================
void DmLogoSonicBuild(void)
{
	s32			i;
	void		*tex_amb[DMD_LOGO_SONIC_AOSTEX_MAX];
	AOS_TEXTURE	*aos_tex;

	MTM_ASSERT(DmLogoSonicLoadCheck() == TRUE);
	MTM_ASSERT(dm_logo_sonic_build_state == FALSE);
	MTM_ASSERT(dm_logo_sonic_aos_tex == NULL);

	// ビルド処理生成
	dm_logo_sonic_build_tcb = MTM_TASK_MAKE_TCB(dmLogoSonicDataBuildMain, dmLogoSonicDataBuildDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_LOGO_SONIC_TASK_PRIO_DATA_BUILD, DMD_LOGO_SONIC_TASK_GROUP_DATA_BUILD,
						0/*work_size*/, "DM_LSONT_BUILD");

	// テクスチャ管理バッファ取得
	dm_logo_sonic_aos_tex = (AOS_TEXTURE*)amMemAlloc(sizeof(AOS_TEXTURE) * DMD_LOGO_SONIC_AOSTEX_MAX);
	MI_CpuClear8(dm_logo_sonic_aos_tex, sizeof(AOS_TEXTURE) * DMD_LOGO_SONIC_AOSTEX_MAX);

	// テクスチャビルド
	tex_amb[DMD_LOGO_SONIC_AOSTEX_COM] =
				amBindGet((AMS_AMB_HEADER*)dm_logo_sonic_data[DMD_LOGO_SONIC_DATA_COM_AMA_SET_AMB],
									IDB_D_LOGO_SONIC_D_LOGO_SONIC_AMB);

	aos_tex = dm_logo_sonic_aos_tex;
	for (i = 0; i < DMD_LOGO_SONIC_AOSTEX_MAX; i++, aos_tex++) {
		AoTexBuild(aos_tex, tex_amb[i]);
		AoTexLoad(aos_tex);
	}
}

// ==========================================================================
// DmLogoSonicBuildCheck
/*!
 *	ソニックチームロゴデータビルド ビルド終了チェック
 *
 *	@return	TRUE : ビルド終了
 */
// ==========================================================================
BOOL DmLogoSonicBuildCheck(void)
{
	if (dm_logo_sonic_build_state) {
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データフラッシュ
// ==========================================================================
// ==========================================================================
// DmLogoSonicFlush
/*!
 *	ソニックチームロゴデータフラッシュ
 *
 */
// ==========================================================================
void DmLogoSonicFlush(void)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;

	MTM_ASSERT(dm_logo_sonic_aos_tex);
	MTM_ASSERT(dm_logo_sonic_build_state == TRUE);
	MTM_ASSERT(dm_logo_sonic_flush_tcb == NULL);

	// フラッシュ処理生成
	dm_logo_sonic_flush_tcb = MTM_TASK_MAKE_TCB(dmLogoSonicDataFlushMain, dmLogoSonicDataFlushDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_LOGO_SONIC_TASK_PRIO_DATA_FLUSH, DMD_LOGO_SONIC_TASK_GROUP_DATA_FLUSH,
						0/*work_size*/, "DM_LSONT_FLUSH");

	// テクスチャフラッシュ
	aos_tex = dm_logo_sonic_aos_tex;
	for (i = 0; i < DMD_LOGO_SONIC_AOSTEX_MAX; i++, aos_tex++) {
		AoTexRelease(aos_tex);
	}
}

// ==========================================================================
// DmLogoSonicFlushCheck
/*!
 *	ソニックチームロゴデータフラッシュ フラッシュ終了チェック
 *
 *	@return	TRUE : フラッシュ終了
 */
// ==========================================================================
BOOL DmLogoSonicFlushCheck(void)
{
	if (!dm_logo_sonic_build_state) {
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// データリリース
// ==========================================================================
// ==========================================================================
// DmLogoSonicRelease
/*!
 *	ソニックチームロゴデータリリース
 */
// ==========================================================================
void DmLogoSonicRelease(void)
{
	s32	i;

	for (i = 0; i < DMD_LOGO_SONIC_DATA_MAX; i++) {
		if (dm_logo_sonic_data[i]) {
			amMemFree(dm_logo_sonic_data[i]);
		}
		dm_logo_sonic_data[i] = NULL;
	}
	
#ifndef SONIC4_TRIAL
	mppUtil::regAndDontShowBtn();
	mppUtil::startCommunityLoadingIndicator();
#else
  #ifdef SONIC4_TRIAL_EXIBITION
	mppUtil::showDemoSplashForTrial();
  #endif
#endif
}

// ==========================================================================
// DmLogoSonicReleaseCheck
/*!
 *	ソニックチームロゴデータリリース リリース終了チェック
 *
 *	@return	TRUE : リリース終了
 */
// ==========================================================================
BOOL DmLogoSonicReleaseCheck(void)
{
	if ((dm_logo_sonic_load_tcb == NULL) && (dm_logo_sonic_data[0] == NULL)) {
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
// dmLogoSonicLoadWait
/*!
 *	データロード待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSonicLoadWait(MTS_TASK_TCB *tcb)
{
	if (DmLogoSonicLoadCheck()) {
		// データビルド開始
		DmLogoSonicBuild();

		mtTaskChangeTcbProcedure(tcb, dmLogoSonicBuildWait);
	}
}

// ==========================================================================
// dmLogoSonicBuildWait
/*!
 *	データビルド待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSonicBuildWait(MTS_TASK_TCB *tcb)
{
	if (DmLogoSonicBuildCheck()) {

		// データロード終了
		mtTaskClearTcb(tcb);

		// 演出開始
		dmLogoSonicStart();
	}
}

// ==========================================================================
// アクション生成・破棄
// ==========================================================================
// ==========================================================================
// dmLogoSonicActionCreate
/*!
 *	アクション生成
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSonicActionCreate(DMS_LOGO_SONIC_WORK *logo_work)
{
	s32		i;
	void	*ama;

	// アクション構築
	ama = amBindGet((AMS_AMB_HEADER*)dm_logo_sonic_data[DMD_LOGO_SONIC_DATA_COM_AMA_SET_AMB],
				IDB_D_LOGO_SONIC_D_LOGO_SONIC_AMA);

	for (i = 0; i < DMD_LOGO_SONIC_ACTMAX; i++) {
		// アクション構築
		AoActSetTexture(AoTexGetTexList(dm_logo_sonic_aos_tex + dm_logo_sonic_tex_id_tbl[i]));
		logo_work->act[i] = AoActCreate(ama, i);
	}
}

// ==========================================================================
// dmLogoSonicActionDelete
/*!
 *	アクション破棄
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSonicActionDelete(DMS_LOGO_SONIC_WORK *logo_work)
{
	s32		i;

	for (i = 0; i < DMD_LOGO_SONIC_ACTMAX; i++) {
		AoActDelete(logo_work->act[i]);
	}
}

// ==========================================================================
// メイン処理
// ==========================================================================
// ==========================================================================
// dmLogoSonicStart
/*!
 *	ソニックチームロゴ開始
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSonicStart(void)
{
	MTS_TASK_TCB		*tcb;
	DMS_LOGO_SONIC_WORK *logo_work;

	NNS_RGBA		diffuse = { 1.0f, 1.0f, 1.0f, 1.0f};
	NNS_RGB			ambient = { 1.0f, 1.0f, 1.0f};

	// メイン処理開始
	tcb = MTM_TASK_MAKE_TCB(dmLogoSonicMainFunc, NULL,
						0/*flag*/, GMD_TASK_PAUSELEVEL_DEF,
						DMD_LOGO_SONIC_TASK_PRIO_MAIN, DMD_LOGO_SONIC_TASK_GROUP_MAIN,
						sizeof(DMS_LOGO_SONIC_WORK), "DM_LSONT_MAIN");
	logo_work = (DMS_LOGO_SONIC_WORK*)mtTaskGetTcbWork(tcb);
	MI_CpuClear8(logo_work, sizeof(DMS_LOGO_SONIC_WORK));

	// 環境初期化
	nnSetPrimitive3DMaterial(&diffuse, &ambient, 1.0f);

	AoActSysSetDrawStateEnable(FALSE);

	// アクション初期化
	dmLogoSonicActionCreate(logo_work);

	// フェード開始
	IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEIN,
								DMD_LOGO_SONIC_FADEIN_TIME, TRUE);

	// フェードイン待機へ
	logo_work->func = dmLogoSonicFadeInWaitFunc;
}


// ==========================================================================
// dmLogoSonicMainFunc
/*!
 *	メイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSonicMainFunc(MTS_TASK_TCB *tcb)
{
	s32					i;
	DMS_LOGO_SONIC_WORK	*logo_work;
	float				update_frame;

	logo_work = (DMS_LOGO_SONIC_WORK*)mtTaskGetTcbWork(tcb);

	// 演出
	if (AoSysIsShowPlatformUI()) {
		// フェード1フレーム停止
		if (IzFadeIsExe()) {
			IzFadeSetStopUpdate1Frame(NULL);
		}
	}
	else {
		if (logo_work->func) {
			logo_work->func(logo_work);
		}

		// スキップチェック
		if ((logo_work->flag & DMD_LOGO_SONIC_FLAG_SKIP_OK) &&
					!(logo_work->flag & DMD_LOGO_SONIC_FLAG_SKIP)) {
#if !_IPHONE
			if (AoPadSomeoneStand(DMD_LOGO_SONIC_SKIP_KEY) >= 0) {
#else //!_IPHONE
			if (amTpIsTouchPush(0)) {
#endif //!_IPHONE
				// スキップ実行
				logo_work->flag |= DMD_LOGO_SONIC_FLAG_SKIP;

				if (IzFadeIsEnd()) {
					// フェード未実行
					// フェード開始
#if !_IPHONE
					IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEOUT,
												DMD_LOGO_SONIC_FADEOUT_TIME, TRUE);
#else //!_IPHONE
					IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEOUT,
												DMD_LOGO_SONIC_FADEOUT_TIME_SKIP, TRUE);
#endif //!_IPHONE
				}
				// フェードアウトへ移行
				logo_work->func = dmLogoSonicFadeOutWaitFunc;
			}
		}

		if (logo_work->flag & DMD_LOGO_SONIC_FLAG_END) {
			// 演出終了

			// 前終了待機処理へ
			mtTaskChangeTcbProcedure(tcb, dmLogoSonicPreEndWait);
			logo_work->timer = 0;

#if 0
			// アクション破棄
			dmLogoSonicActionDelete(logo_work);
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
	for (i = 0; i < DMD_LOGO_SONIC_ACTMAX; i++) {
		AoActSetTexture(AoTexGetTexList(dm_logo_sonic_aos_tex + dm_logo_sonic_tex_id_tbl[i]));
		AoActUpdate(logo_work->act[i], update_frame);
		AoActDraw(logo_work->act[i]);
	}
}


// ==========================================================================
// 演出
// ==========================================================================
// ==========================================================================
// dmLogoSonicFadeInWaitFunc
/*!
 *	フェード待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSonicFadeInWaitFunc(DMS_LOGO_SONIC_WORK *logo_work)
{
	if (IzFadeIsEnd()) {
		// 表示待機へ移行
		logo_work->func = dmLogoSonicDispWaitFunc;
		logo_work->timer = 0;
	}
}

// ==========================================================================
// dmLogoSonicDispWaitFunc
/*!
 *	表示待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSonicDispWaitFunc(DMS_LOGO_SONIC_WORK *logo_work)
{
	logo_work->timer++;
	if (logo_work->timer >= DMD_LOGO_SONIC_DISP_TIME) {
		// フェードアウトへ移行
		logo_work->func = dmLogoSonicFadeOutWaitFunc;

		// フェード開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL, IZE_FADE_TYPE_WHITE_FADEOUT,
									DMD_LOGO_SONIC_FADEOUT_TIME, TRUE);

		// スキップ許可OFF
		logo_work->flag &= ~DMD_LOGO_SONIC_FLAG_SKIP_OK;
		return;
	}

	if (logo_work->timer == 30) {
		// スキップ許可
		logo_work->flag |= DMD_LOGO_SONIC_FLAG_SKIP_OK;
	}
}

// ==========================================================================
// dmLogoSonicFadeOutWaitFunc
/*!
 *	フェード待機
 *
 *	@param	logo_work	[in]	ロゴワーク
 */
// ==========================================================================
void dmLogoSonicFadeOutWaitFunc(DMS_LOGO_SONIC_WORK *logo_work)
{
	
	if (IzFadeIsEnd()) {
		static bool needToShowConfirmation = true;
		static bool needToWaitTheEndOfConfirmation = true;
		if(needToShowConfirmation) {//qqq
			needToShowConfirmation = false;
			if( mppCheckPointStorage::isStateExist()) {
				mppUtil::showLoadGameConfirmation();
			}
			else {
				needToWaitTheEndOfConfirmation = false;
			}
		}				
		if(!needToWaitTheEndOfConfirmation || mppCheckPointStorage::isNeedToLoadSavedGame()!=0)
		{
			// 演出終了
			logo_work->flag |= DMD_LOGO_SONIC_FLAG_END;
		}
/*		

 */
	}
}


// ==========================================================================
// リリース待機
// ==========================================================================
// ==========================================================================
// dmLogoSonicPreEndWait
/*!
 *	データロード終了待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSonicPreEndWait(MTS_TASK_TCB *tcb)
{
	DMS_LOGO_SONIC_WORK	*logo_work;
	logo_work = (DMS_LOGO_SONIC_WORK*)mtTaskGetTcbWork(tcb);

	logo_work->timer++;
	if (logo_work->timer > 2) {
		// アクション破棄
		dmLogoSonicActionDelete(logo_work);

		// データフラッシュ
		DmLogoSonicFlush();

		// データフラッシュ待機へ
		mtTaskChangeTcbProcedure(tcb, dmLogoSonicFlushWaitFunc);
	}
}

// ==========================================================================
// dmLogoSonicFlushWaitFunc
/*!
 *	データフラッシュ待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSonicFlushWaitFunc(MTS_TASK_TCB *tcb)
{
	if (!DmLogoSonicFlushCheck()) {
		return;
	}

	// データフリリース
	DmLogoSonicRelease();
	// リリース処理待機へ
	mtTaskChangeTcbProcedure(tcb, dmLogoSonicRelesehWaitFunc);
}

// ==========================================================================
// dmLogoSonicRelesehWaitFunc
/*!
 *	データリリース待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSonicRelesehWaitFunc(MTS_TASK_TCB *tcb)
{
	if (!DmLogoSonicReleaseCheck()) {
		return;
	}
	
#ifndef SONIC4_TRIAL
	if(mppUtil::isCommunityLoadedOrUnsupportedOrCancelled()==false)
	{
		return;
	}
	mppUtil::stopCommunityLoadingIndicator();
#else
  #ifdef SONIC4_TRIAL_EXIBITION
	if(mppUtil::isDemoSplashShown()) {
		return;
	}
  #endif
#endif
	
	// 終了待機破棄
	mtTaskClearTcb(tcb);

	// 次のイベントへ
	SyChangeNextEvt();
}



// ==========================================================================
// ファイルロード時後処理
// ==========================================================================
// ==========================================================================
// dmLogoSonicLoadPostFunc
/*!
 *	データロード後処理
 *
 *	@param	context	[in]	コンテキスト
 */
// ==========================================================================
void dmLogoSonicLoadPostFunc(DMS_LOGO_COM_LOAD_CONTEXT *context)
{
//	s32				i;
//	AMS_AMB_HEADER	*amb_header;

	dm_logo_sonic_data[context->no] = context->fs_req->buf;
	context->fs_req->buf = NULL;

	// コンバート
	amBindConvertAll((u8*)dm_logo_sonic_data[context->no]);

//	if (context->no == DMD_LOGO_SONIC_DATA_COM_AMA_SET_AMB) {
//		void	*tex_amb;
//		tex_amb = amBindGet((AMS_AMB_HEADER*)dm_logo_sonic_data[context->no],
//								IDB_D_LOGO_SONIC_D_LOGO_SONIC_AMB);
//		amBindConvertAll((u8*)tex_amb);
//	}
}

// ==========================================================================
// データビルド
// ==========================================================================
// ==========================================================================
// dmLogoSonicDataBuildMain
/*!
 *	データビルドメイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSonicDataBuildMain(MTS_TASK_TCB *tcb)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;
	BOOL		b_sts = TRUE;

	// テクスチャビルド
	aos_tex = dm_logo_sonic_aos_tex;
	for (i = 0; i < DMD_LOGO_SONIC_AOSTEX_MAX; i++, aos_tex++) {
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
	dm_logo_sonic_build_state = TRUE;
}

// ==========================================================================
// dmLogoSonicDataBuildDest
/*!
 *	データビルドデストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSonicDataBuildDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dm_logo_sonic_build_tcb = NULL;
}

// ==========================================================================
// データフラッシュ
// ==========================================================================
// ==========================================================================
// dmLogoSonicDataFlushMain
/*!
 *	データフラッシュメイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSonicDataFlushMain(MTS_TASK_TCB *tcb)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;
	BOOL		b_sts = TRUE;

	// テクスチャフラッシュ
	aos_tex = dm_logo_sonic_aos_tex;
	for (i = 0; i < DMD_LOGO_SONIC_AOSTEX_MAX; i++, aos_tex++) {
		if (!AoTexIsReleased(aos_tex)) {
			b_sts = FALSE;
		}
	}

	// フラッシュ待機
	if (!b_sts) {
		return;
	}
	
	// テクスチャ管理バッファ解放
	amMemFree(dm_logo_sonic_aos_tex);
	dm_logo_sonic_aos_tex = NULL;

	// フラッシュ終了
	mtTaskClearTcb(tcb);
	dm_logo_sonic_build_state = FALSE;
}

// ==========================================================================
// dmLogoSonicDataFlushDest
/*!
 *	データフラッシュデストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoSonicDataFlushDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dm_logo_sonic_flush_tcb = NULL;
}

// ==========================================================================
// DmLogoSonicStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void DmLogoSonicStaticVarInit(void)
{
	dm_logo_sonic_load_tcb  = NULL;		//!< データロードTCB
	dm_logo_sonic_build_tcb = NULL;		//!< データビルドTCB
	dm_logo_sonic_flush_tcb = NULL;		//!< データフラッシュTCB
	//dm_logo_sonic_release_tcb = NULL;	//!< データリリースTCB
	
	dm_logo_sonic_aos_tex = NULL;		//!< テクスチャ
	dm_logo_sonic_build_state = FALSE;	//!< ビルド状況
	
	/// データ
	memset(dm_logo_sonic_data, 0, sizeof(dm_logo_sonic_data));
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================

//	NNS_RGBA		diffuse = { 1.0f, 1.0f, 1.0f, 1.0f};
//	NNS_RGB			ambient = { 1.0f, 1.0f, 1.0f};
//
//	nnSetPrimitive3DMaterial(&diffuse, &ambient, 1.0f);