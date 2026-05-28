// ===========================================================================
/*!
	@file	gsInit.cpp
	@brief	システム初期化モジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "gsInit.h"
#include "ao.h"
#include "aoPresence.h"
#include "aoAvatarAward.h"
#include "gs.h"
#include "gsMainSys.h"
#include "gsPresence.h"
#include "gsFont.h"
#include "dmRankSys.h"
#include "dmLoading.h"
#include "gsTrophy.h"
#include "izFade.h"
#include "gsStrapImage.h"
#include "gsReboot.h"
#include "gsSystemBgm.h"
#include "gsWiiCriError.h"
#include "gsDebug.h"

// ----- Macros ------------------------------------------------（マクロ定義）

// 2Dアクション初期化パラメータ
#define GSD_ACT_INIT_SPR_BUF_NUM		(256)	//!< スプライトバッファ数
#define GSD_ACT_INIT_ACT_BUF_NUM		(256)	//!< アクションバッファ数
#define GSD_ACT_INIT_SORT_BUF_NUM		(256)	//!< ソートバッファ数
#define GSD_ACT_INIT_ACM_BUF_NUM		(32)	//!< アキュムレートスタック数

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//! プロシージャ型
// ===========================================================================
typedef void (*GSF_INIT_FUNC)(void);

// ===========================================================================
//! 初期化タスクワーク
// ===========================================================================
typedef struct tag_GSS_INIT_WROK {
	u32				count;		//!< 汎用カウンタ
	AMS_FS*			fs;			//!< ファイル読み込み用
	void (*proc)(struct tag_GSS_INIT_WROK*);	//!< プロシージャ

#if _WII
	GSS_STRAP_IMAGE	strap;		//!< ストラップ管理
#endif // _WII
} GSS_INIT_WORK;

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// タスク
static void gsInitTaskProcedure(AMS_TCB* tcb);
static void gsInitTaskDestructor(AMS_TCB* tcb);

// プロシージャ
static void gsInitProcSysFirst(GSS_INIT_WORK* work);
static void gsInitProcStrapLoad(GSS_INIT_WORK* work);
static void gsInitProcLoadLoadingFile(GSS_INIT_WORK* work);
static void gsInitProcBuildLoadingFile(GSS_INIT_WORK* work);
static void gsInitProcLoadSysMsgFile(GSS_INIT_WORK* work);
static void gsInitProcLoadSaveMsgFile(GSS_INIT_WORK* work);
static void gsInitProcSysLast(GSS_INIT_WORK* work);
static void gsInitProcCheckTrial(GSS_INIT_WORK* work);
static void gsInitProcInitTorphy(GSS_INIT_WORK* work);
static void gsInitProcInstallTorphy(GSS_INIT_WORK* work);
static void gsInitProcPresence(GSS_INIT_WORK* work);
static void gsInitProcWaitPadEnable(GSS_INIT_WORK* work);
static void gsInitProcEnd(GSS_INIT_WORK* work);

#if _PS3
// aoStorageのフェード用コールバック
static void gsInitAoStorageFadeCallback(BOOL fadein, void* arg);
static BOOL gsInitAoStorageFadeIsEndCallback(BOOL fadein, void* arg);
#endif // _PS3

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//! 初期化タスクTCBのポインタ
// ===========================================================================
static AMS_TCB* g_gs_init_tcb = NULL;

// ===========================================================================
//! ローディングデータファイルパス
// ===========================================================================
static const char* g_gs_init_loading_file_path =
	GSS_BASE_PATH"DEMO/LOADING/D_LOADING.AMB";

#if _XBOX | _PS3 | _WII
// ===========================================================================
//! システムメッセージファイル
// ===========================================================================
static void * g_gs_init_sys_msg_file = NULL;

// ===========================================================================
//! セーブデータメッセージファイル
// ===========================================================================
static void * g_gs_init_save_msg_file = NULL;

// ===========================================================================
//! システムメッセージファイルパス1
// ===========================================================================
static const char* g_gs_init_sys_msg_file_path1 = "MSG/AO/AO_SYS_MSG_";

// ===========================================================================
//! セーブデータメッセージファイルパス1
// ===========================================================================
static const char* g_gs_init_save_msg_file_path1 = "MSG/AO/AO_SYS_SAVE_";

// ===========================================================================
//! メッセージファイルパス2(言語別)
// ===========================================================================
static const char* g_gs_init_msg_file_path2_tbl[GSD_LANGUAGE_NUM] = {
	"JP.MSG",
	"US.MSG",
	"FR.MSG",
	"IT.MSG",
	"GE.MSG",
	"SP.MSG",
};
#endif // _XBOX | _PS3 | _WII

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//	GsInitOtherStart
/*!
	初期化処理開始
*/
// ===========================================================================
void GsInitOtherStart(void)
{
	// GSS_MAIN_SYS_INFOの初期化
    	GSS_MAIN_SYS_INFO* info = GsGetMainSysInfo();
	info->is_save_run = 0; // 最初はセーブ無効

	// タスク作成
	g_gs_init_tcb = amTaskMake(
		gsInitTaskProcedure, gsInitTaskDestructor, 0, 0, 0, "gsInit");

	// ワーク初期化
	GSS_INIT_WORK* work = (GSS_INIT_WORK*)amTaskGetWork(g_gs_init_tcb);
	work->count = 0;
	work->fs = NULL;
	work->proc = gsInitProcSysFirst;

	// タスク開始
	amTaskStart(g_gs_init_tcb);
}

// ===========================================================================

//	GsInitOtherIsInitialized
/*!
	初期化処理完了判定

	@return 真：完了　偽：処理中
	@note
	初期化処理が完了したかではなく、
	GsInitOtherStart関数で開始した処理が終了したかを判定します。\n
	そのため、GsInitOtherStart関数呼び出し前にも
	TRUEを返すので注意して下さい。\n
*/
// ===========================================================================
BOOL GsInitOtherIsInitialized(void)
{
	if (g_gs_init_tcb) {
		return FALSE;
	}
	return TRUE;
}

// ===========================================================================
//	GsOtherExit
/*!
	終了処理
*/
// ===========================================================================
void GsOtherExit(void)
{
	// 終了時には振動をOFFにする
	amPadEnableVibration(-1, 0);

#if _XBOX
	AoAvatarAwardTerminate();
#endif // _XBOX

	GsTrialExit();
	GsTrophyExit();
#if !_WII
	// Wiiは時間がかかるので呼ばない
	DmRankSysExit();
#endif // !_WII
	GsPresenceWatchEnd();
	AoPresenceExit();
	AoTrophyAcquisitionTaskEnd();
	AoTrophyExit();
	AoStorageExit();
	AoAccountExit();
	GsRebootExit();
	GsSystemBgmExit();
	AoSysMsgSetBaseMsgFile(NULL);
	AoActSysExit();
#if _WII
	GsWiiCriErrorExit();
#endif // _WII
	AoSysExit();
	GsMemFileExit();
	GsFontExit();
#if _WII
	GsStrapImageExit();
#endif // _WII

#if _XBOX | _PS3 | _WII
	if (g_gs_init_sys_msg_file) {
		amMemFree(g_gs_init_sys_msg_file);
		g_gs_init_sys_msg_file = NULL;
	}
#endif // _XBOX | _PS3 | _WII
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ***************************************************************************
// タスク
// ***************************************************************************
// ===========================================================================
//! タスクプロシージャ
// ===========================================================================
void gsInitTaskProcedure(AMS_TCB* tcb)
{
	// ワーク取得
	GSS_INIT_WORK* work = (GSS_INIT_WORK*)amTaskGetWork(tcb);

	// プロシージャが無いなら終了
	if (work->proc == NULL) {
		amTaskDelete(tcb);
		return;
	}

	// プロシージャ取得
	u32 proc = (u32)work->proc;

	// プロシージャ呼び出し
	work->proc(work);

	// カウンタ更新
	if ((u32)work->proc != proc) {
		work->count = 0;
	}
	else {
		if (work->count < (u32)-1) {
			work->count += 1;
		}
	}
}

// ===========================================================================
//! タスクデストラクタ
// ===========================================================================
void gsInitTaskDestructor(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	g_gs_init_tcb = NULL;
}


// ***************************************************************************
// プロシージャ
// ***************************************************************************
// ===========================================================================
//! システム最初に行なう初期化処理
// ===========================================================================
void gsInitProcSysFirst(GSS_INIT_WORK* work)
{
	// 環境初期化
	GsEnvInit();

	// フォント初期化
	GsFontInit();

	// メモリファイル初期化
	GsMemFileInit();

#if _WII
	// FATALエラーメッセージファイル設定
	AoSysMsgSetFatalMsgFile(GsMemFileGetWiiFatalErrorMessageFile());
#endif // _WII

#if _WII
	// ストラップ画像モジュール初期化
	GsStrapImageInit();
	GsStrapImageInitWork(work->strap);
#endif // _WII

	// システム初期化
#if _PS3
	AoSysPS3SetSystemCallbackSlot(1);
#endif // _PS3
	AoSysInit();

#if _WII
	// CRIファイルシステムFatalエラー管理モジュール初期化
	GsWiiCriErrorInit();
#endif // _WII

	// 2Dアクション初期化
	AoActSysInit(
		GSD_ACT_INIT_SPR_BUF_NUM,
		GSD_ACT_INIT_ACT_BUF_NUM,
		GSD_ACT_INIT_SORT_BUF_NUM,
		GSD_ACT_INIT_ACM_BUF_NUM);

	// 2Dアクションファイルアドレス変換登録
	AoActRegAliceAmaConv();

	// メッセージファイルアドレス変換登録
	AoMsgRegAliceMsgConv();

	// システムBGMモジュール初期化
	GsSystemBgmInit();
	GsSystemBgmSetEnable(TRUE);

	// 再起動管理モジュール初期化
	GsRebootInit();

	// フェード開始
	BOOL is_white = TRUE;
	if (!GsRebootIsTitleReboot()) {
#if _PC || _XBOX

#if defined(HOG_RGB_US)
		is_white = FALSE;
#endif // defined(HOG_RGB_US)

#elif _PS3

		is_white = FALSE;

#endif
	}

	if (is_white) {
		IzFadeInitEasyTask(
			IZE_FADE_SET_TYPE_NORMAL,
			255, 255, 255, 255, 255, 255, 255, 255, 1.0f);
	}
	else {
		IzFadeInitEasyTask(
			IZE_FADE_SET_TYPE_NORMAL,
			0, 0, 0, 255, 0, 0, 0, 255, 1.0f);
	}

	// 次へ遷移
	work->proc = gsInitProcStrapLoad;
}

// ===========================================================================
//! ストラップ画面ファイル読み込み
// ===========================================================================
void gsInitProcStrapLoad(GSS_INIT_WORK* work)
{
#if _WII

	// 2度目の起動(タイトル以降)ならストラップ画面なし
	if (GsRebootIsTitleReboot()) {
		amWiiSetEnableIconHBM(1);
		work->proc = gsInitProcLoadLoadingFile;
		return;
	}

#if defined(MTD_DEBUG)
#if defined(GSD_DEBUG_DEMO_SELECT)
	// デバッグメニューからの起動ならストラップ画面なし(開発用)
	{
		amWiiSetEnableIconHBM(1);
		work->proc = gsInitProcLoadLoadingFile;
		return;
	}
#endif // defined(GSD_DEBUG_DEMO_SELECT)
#endif // defined(MTD_DEBUG)

	if (work->count == 0) {

		// 読み込み開始
		GsStrapImageLoadStart(work->strap);
	}

	// 読み込み完了判定
	if (GsStrapImageLoadIsFinished(work->strap)) {

		// ストラップ画面表示開始
		GsStrapImageShowStart(work->strap);

		// 次へ遷移
		work->proc = gsInitProcLoadLoadingFile;
	}

#else

	// 次へ遷移
	work->proc = gsInitProcLoadLoadingFile;

#endif
}

// ===========================================================================
//! ローディング画面ファイル読み込み
// ===========================================================================
void gsInitProcLoadLoadingFile(GSS_INIT_WORK* work)
{
	if (work->count == 0) {
		// ファイル読み込み開始
		char path[64];
		strncpy(path, g_gs_init_loading_file_path, 64);
		work->fs = amFsReadBackground(path);
	}

	// ファイル読み込み完了判定
	if (amFsIsComplete(work->fs)) {

		// データセット
		DmLoadingBuild(work->fs);

		// リクエストクリア
		work->fs->buf = NULL;
		amFsClearRequest(work->fs);
		work->fs = NULL;

		// 次へ遷移
		work->proc = gsInitProcBuildLoadingFile;
	}
}

// ===========================================================================
//! ローディング画面ファイル構築
// ===========================================================================
void gsInitProcBuildLoadingFile(GSS_INIT_WORK* work)
{
	// 構築完了判定
	if (DmLoadingBuildCheck()) {

		// 次へ遷移
		work->proc = gsInitProcLoadSysMsgFile;
	}
}

// ===========================================================================
//! システムメッセージファイル読み込み
// ===========================================================================
void gsInitProcLoadSysMsgFile(GSS_INIT_WORK* work)
{
#if _XBOX | _PS3 | _WII

	if (work->count == 0) {
		// ファイル読み込み開始
		char path[32];
		sprintf(
			path, "%s%s",
			g_gs_init_sys_msg_file_path1,
			g_gs_init_msg_file_path2_tbl[GsEnvGetLanguage()]);
		work->fs = amFsReadBackground(path);
	}

	// ファイル読み込み完了判定
	if (amFsIsComplete(work->fs)) {

		// ファイル取得
		g_gs_init_sys_msg_file = work->fs->buf;
		work->fs->buf = NULL;

		// リクエストクリア
		amFsClearRequest(work->fs);
		work->fs = NULL;

		// アドレス変換
		amConvertAddress(g_gs_init_sys_msg_file);

		// システムメッセージファイル設定
		AoSysMsgSetBaseMsgFile(g_gs_init_sys_msg_file);

		// 次へ遷移
		work->proc = gsInitProcLoadSaveMsgFile;
	}

#else

	// ファイル無し
	AoSysMsgSetBaseMsgFile(NULL);

	// 次へ遷移
	work->proc = gsInitProcLoadSaveMsgFile;

#endif
}

// ===========================================================================
//! セーブメッセージファイル読み込み
// ===========================================================================
void gsInitProcLoadSaveMsgFile(GSS_INIT_WORK* work)
{
#if _XBOX | _PS3 | _WII

	if (work->count == 0) {
		// ファイル読み込み開始
		char path[32];
		sprintf(
			path, "%s%s",
			g_gs_init_save_msg_file_path1,
			g_gs_init_msg_file_path2_tbl[GsEnvGetLanguage()]);
		work->fs = amFsReadBackground(path);
	}

	// ファイル読み込み完了判定
	if (amFsIsComplete(work->fs)) {

		// ファイル取得
		g_gs_init_save_msg_file = work->fs->buf;
		work->fs->buf = NULL;

		// リクエストクリア
		amFsClearRequest(work->fs);
		work->fs = NULL;

		// アドレス変換
		amConvertAddress(g_gs_init_save_msg_file);

		// セーブメッセージファイル設定
		AoStorageSetSaveMsgFile(g_gs_init_save_msg_file);

		// 次へ遷移
		work->proc = gsInitProcSysLast;
	}

#else

	// ファイル無し
	AoStorageSetSaveMsgFile(NULL);

	// 次へ遷移
	work->proc = gsInitProcSysLast;

#endif
}

// ===========================================================================
//! システム最後に行なう初期化処理
// ===========================================================================
void gsInitProcSysLast(GSS_INIT_WORK* work)
{
	// アカウントシステム初期化
	AoAccountInit();

	// ストレージシステム初期化
	AoStorageInit();

#if _XBOX
	AoStorageSetDataXbox360(
		GsMemFileGetXbox360SaveThumbPng(),
		GsMemFileGetXbox360SaveThumbPngSize());
#endif // _XBOX

#if _WII
	AoStorageSetDataWii(
		GsMemFileGetWiiSaveBannerTpl(),
		GsMemFileGetWiiSaveBannerTplSize());

	AoSysMsgSetKeyDecide(GSD_KEY_DECIDE);
	AoSysMsgSetKeyCancel(GSD_KEY_CANCEL);
	AoSysMsgSetKeyLeft(GSD_KEY_LEFT);
	AoSysMsgSetKeyRight(GSD_KEY_RIGHT);

	AoSysMsgSetWinTexAmb(GsMemFileGetWindowTextureAtypeAmb(), 0);
#endif // _WII

	// ランキングシステム初期化
	DmRankSysInit();

	// 次へ遷移
	work->proc = gsInitProcCheckTrial;
}

// ===========================================================================
//! 体験版管理初期化
// ===========================================================================
void gsInitProcCheckTrial(GSS_INIT_WORK* work)
{
	if (work->count == 0) {
		// 初期化開始
		GsTrialInitStart();
	}

	// 初期化完了判定
	if (GsTrialInitIsFinished()) {

		amSystemLog("\n================================================\n");
		if (GsTrialIsTrial()) {
			amSystemLog("gsInit - product mode : Trial\n");
		}
		else {
			amSystemLog("gsInit - product mode : Full\n");
		}
		amSystemLog("================================================\n\n");

		// 次へ遷移
		work->proc = gsInitProcInitTorphy;
	}
}

// ===========================================================================
//! トロフィー初期化
// ===========================================================================
void gsInitProcInitTorphy(GSS_INIT_WORK* work)
{
	// トロフィーシステムなど初期化
#if _PS3
	AoTrophyPs3SetComId(GsEnvGetPs3ComId());
	AoTrophyPs3SetComSignature(GsEnvGetPs3ComSignature());
	AoStorageSetTitleIdPS3(GsEnvGetPs3TitleId());
	AoStorageSetDataPS3(
		GsMemFileGetPS3SaveIcon0Png(),
		GsMemFileGetPS3SaveIcon0PngSize(),
		GsMemFileGetPS3SavePic1Png(),
		GsMemFileGetPS3SavePic1PngSize());
	AoStorageSetFadeCallbackPS3(gsInitAoStorageFadeCallback, NULL);
	AoStorageSetFadeIsEndCallbackPS3(gsInitAoStorageFadeIsEndCallback, NULL);
#endif // _PS3
	AoTrophyInit(GsTrialIsTrial());

	// GSトロフィーシステム初期化
	GsTrophyInit();

	// 次へ遷移
	work->proc = gsInitProcInstallTorphy;
}

// ===========================================================================
//! トロフィーインストール
// ===========================================================================
void gsInitProcInstallTorphy(GSS_INIT_WORK* work)
{
#if _PS3

	if (work->count == 0) {

		// インストール開始
		AoTrophyInstallStart();
	}

	// インストール完了判定
	if (AoTrophyInstallIsFinished()) {

		// 監視開始
		AoTrophyAcquisitionTaskStart();

		// インストール成功判定
		if (AoTrophyInstallIsSuccess()) {

			// 次へ遷移
			work->proc = gsInitProcPresence;
		}
		else {

			// インストール失敗
			// アプリケーション終了要求が出てるはずなので
			// そのまま進めてもすぐに終了される

			// 次へ遷移
			work->proc = gsInitProcPresence;
		}
	}

#else

	// 監視開始
	AoTrophyAcquisitionTaskStart();

	// 次へ遷移
	work->proc = gsInitProcPresence;

#endif
}

// ===========================================================================
//! プレゼンス初期化待ち
// ===========================================================================
void gsInitProcPresence(GSS_INIT_WORK* work)
{
	if (work->count == 0) {
		// 初期化開始
		AoPresenceInit();
	}

	// 初期化完了判定
	if (AoPresenceInitialized()) {

		// 監視開始
		GsPresenceWatchStart();

		// 次へ遷移
		work->proc = gsInitProcWaitPadEnable;
	}
}

// ===========================================================================
//! パッド有効待ち
// ===========================================================================
void gsInitProcWaitPadEnable(GSS_INIT_WORK* work)
{
#if defined(MTD_DEBUG)
	// パッドの0番が有効になるまで待機
	if ((work->count >= 120) ||
		((work->count >= 60) && AoPadIsConnected(0)))
	{
		amSystemLog("Pad Input Wait Count : %d\n", work->count);

		// 次へ遷移
		work->proc = gsInitProcEnd;
	}
#else
	// 次へ遷移
	work->proc = gsInitProcEnd;
#endif // defined(MTD_DEBUG)
}

// ===========================================================================
//! 終了
// ===========================================================================
void gsInitProcEnd(GSS_INIT_WORK* work)
{
#if _WII

	// Wii版のみストラップ画面終了まで待機
	if (!GsStrapImageShowIsFinished(work->strap)) {
		return;
	}

	// ストラップ画面ワーク解放
	GsStrapImageExitWork(work->strap);

	// HBM有効化
	amWiiSetEnableHBM(1);
	amWiiSetEnableIconHBM(1);

#endif // _WII

#if defined(MTD_DEBUG)
	// デバッグ版のみデフォルトでセーブフラグを有効にする
//	GsGetMainSysInfo()->is_save_run = 1;
#endif // defined(MTD_DEBUG)

#if defined(MTD_DEBUG)
	// デバッグ用タスク作成
	GsDebugCreateDebugTask();
#endif // defined(MTD_DEBUG)

	// 終了
	work->proc = NULL;
}

#if _PS3
#include "izFade.h"
// ***************************************************************************
// aoStorageのフェード用コールバック
// ***************************************************************************
// ===========================================================================
// aoStorageのフェード要求コールバック
// ===========================================================================
void gsInitAoStorageFadeCallback(BOOL fadein, void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	const u8 alpha = 255 - 16;
	u8 sa, ea;

	if (fadein) {
		sa = alpha;
		ea = 0;
	}
	else {
		sa = 0;
		ea = alpha;
	}
	IzFadeInitEasyTask(
		IZE_FADE_SET_TYPE_NORMAL,
		0, 0, 0, sa, 0, 0, 0, ea, 8.0f);
}

// ===========================================================================
// aoStorageのフェード完了判定コールバック
// ===========================================================================
BOOL gsInitAoStorageFadeIsEndCallback(BOOL fadein, void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	if (IzFadeIsEnd()) {
		if (fadein) {
			IzFadeExit();
		}
		return TRUE;
	}
	return FALSE;
}
#endif // _PS3

// ===========================================================================
//	function
/*!
	説明

	@param param0	[in] 入力引数0説明
	@param param1	[out] 出力ポインタ引数1説明
	@param param2	[io] 入出力ポインタ引数2説明
	@return 返値説明
	@note 補足説明
*/
// ===========================================================================

// ===========================================================================
//	int variable
// ---------------------------------------------------------------------------
//!	変数説明
// ===========================================================================

	// =======================================================================
	//	function
	/*!
		説明

		@param param0	[in] 入力引数0説明
		@param param1	[out] 出力ポインタ引数1説明
		@param param2	[io] 入出力ポインタ引数2説明
		@return 返値説明
		@note 補足説明
	*/
	// =======================================================================

// ***************************************************************************
// ラベル
// ***************************************************************************
