// ===========================================================================
/*!
	@file	aoTestSysMsg.cpp
	@brief	aoSysMsgテスト

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "aoTest.h"
#include "ao.h"
#include "gs.h"
#include "gsMainSys.h"

#if defined (MTD_DEBUG)

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//! ワーク
// ===========================================================================
typedef struct tag_AOS_TEST_WORK {
	u32				count;			//!< 汎用カウンタ

	u32				language;		//!< 言語

	AMS_FS*			msg_fs;			//!< メッセージファイル読み込み用
	void*			msg_file;		//!< メッセージファイル
	u32				msg_id;			//!< メッセージID
	u32				sel_id;			//!< 選択項目種別

	BOOL			is_font;		//!< フォント構築した
} AOS_TEST_WORK;

// ===========================================================================
//! タスクワーク(64byte以内)
// ===========================================================================
typedef struct tag_AOS_TEST_TASK_WORK {
	AOS_TEST_WORK*	work;	//!< ワーク
} AOS_TEST_TASK_WORK;

// ===========================================================================
//! ワーク2
// ===========================================================================
typedef struct tag_AOS_TEST_WORK2 {
	u32				count;			//!< 汎用カウンタ
	u32				msg_id;			//!< メッセージID
	BOOL			is_font;		//!< フォント構築した
} AOS_TEST_WORK2;

// ===========================================================================
//! タスクワーク2(64byte以内)
// ===========================================================================
typedef struct tag_AOS_TEST_TASK_WORK2 {
	AOS_TEST_WORK2*	work;	//!< ワーク
} AOS_TEST_TASK_WORK2;

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// タスク
static void aoTestTaskProcedureInit(AMS_TCB* tcb);
static void aoTestTaskProcedureBuildFont(AMS_TCB* tcb);
static void aoTestTaskProcedureSelectLanguage(AMS_TCB* tcb);
static void aoTestTaskProcedureLoadMsgFile(AMS_TCB* tcb);
static void aoTestTaskProcedureSelect(AMS_TCB* tcb);
static void aoTestTaskProcedureShowMsg(AMS_TCB* tcb);
static void aoTestTaskDestructor(AMS_TCB* tcb);

// タスク2
static void aoTestTask2ProcedureInit(AMS_TCB* tcb);
static void aoTestTask2ProcedureBuildFont(AMS_TCB* tcb);
static void aoTestTask2ProcedureSelect(AMS_TCB* tcb);
static void aoTestTask2ProcedureShowMsg(AMS_TCB* tcb);
static void aoTestTask2Destructor(AMS_TCB* tcb);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//! システムメッセージファイルパス1
// ===========================================================================
static const char* g_ao_test_msg_file_path1 = "MSG/AO/AO_SYS_MSG_";

// ===========================================================================
//! システムメッセージファイルパス2
// ===========================================================================
static const char* g_ao_test_msg_file_path2 = ".MSG";

// ===========================================================================
//! 言語名配列
// ===========================================================================
static const char* g_ao_test_lang_name_tbl[GSD_LANGUAGE_NUM] = {
	"JP",
	"US",
	"FR",
	"IT",
	"GE",
	"SP",
};

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! aoSysMsgテスト
// ===========================================================================
void AoTestSysMsgStart(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	AMS_TCB* tcb;

	// タスク作成
	tcb = amTaskMake(
		aoTestTaskProcedureInit, aoTestTaskDestructor,
		0, 0, 0, "aoTest::Main");

	// タスクワーク初期化
	AOS_TEST_TASK_WORK* twork = (AOS_TEST_TASK_WORK*)amTaskGetWork(tcb);
	amZeroMemory(twork, sizeof(AOS_TEST_TASK_WORK));
	twork->work = (AOS_TEST_WORK*)amMemAlloc(sizeof(AOS_TEST_WORK));
	AOS_TEST_WORK* work = twork->work;
	amZeroMemory(work, sizeof(AOS_TEST_WORK));

	// ワーク初期化
	work->count = 0;
	work->language = 0;
	work->msg_fs = NULL;
	work->msg_file = NULL;
	work->msg_id = 0;
	work->sel_id = 0;
	work->is_font = FALSE;

	// タスク開始
	amTaskStart(tcb);
}

// ===========================================================================
//! aoSysMsgテスト
// ===========================================================================
void AoTestSysMsgStart2(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	AMS_TCB* tcb;

	// タスク作成
	tcb = amTaskMake(
		aoTestTask2ProcedureInit, aoTestTask2Destructor,
		0, 0, 0, "aoTest::Main2");

	// タスクワーク初期化
	AOS_TEST_TASK_WORK2* twork = (AOS_TEST_TASK_WORK2*)amTaskGetWork(tcb);
	amZeroMemory(twork, sizeof(AOS_TEST_TASK_WORK2));
	twork->work = (AOS_TEST_WORK2*)amMemAlloc(sizeof(AOS_TEST_WORK2));
	AOS_TEST_WORK2* work = twork->work;
	amZeroMemory(work, sizeof(AOS_TEST_WORK2));

	// ワーク初期化
	work->count = 0;
	work->msg_id = 0;
	work->is_font = FALSE;

	// タスク開始
	amTaskStart(tcb);
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ***************************************************************************
// タスク
// ***************************************************************************
// ===========================================================================
//! 初期化
// ===========================================================================
void aoTestTaskProcedureInit(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// フォント構築開始
	if (!GsFontIsBuilded() && !GsFontIsBuilding()) {
		GsFontBuild();
		work->is_font = TRUE;
	}

	// 構築待ちへ遷移
	amTaskSetProcedure(tcb, aoTestTaskProcedureBuildFont);
}

// ===========================================================================
//! フォント構築
// ===========================================================================
void aoTestTaskProcedureBuildFont(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "NOW FONT BUILDING... %d", work->count++);

	// 構築完了待ち
	if (GsFontIsBuilded()) {

		// 言語選択へ遷移
		amTaskSetProcedure(tcb, aoTestTaskProcedureSelectLanguage);
	}
}

// ===========================================================================
//! 言語選択
// ===========================================================================
void aoTestTaskProcedureSelectLanguage(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "PLEASE PUSH ANY KEY. %d", work->count++);
	amPrintf(4, 5, "%c:START", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	// 選択
	if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
		work->language = (work->language + 5) % 6;
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
		work->language = (work->language + 1) % 6;
	}

	// 表示
	for (u32 i = 0; i < 6; ++i) {
		if (i == work->language) {
			amPrintColor(0xff0000ff);
			amPrint(4, (s32)(8 + i), ">");
		}
		else {
			amPrintColor(0xffffffff);
		}
		amPrintf(6, (s32)(8 + i), "%s", g_ao_test_lang_name_tbl[i]);
	}
	amPrintColor(0xffffffff);

	// 決定判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {

		// メッセージファイル読み込み開始
		char path[64];
		sprintf(
			path, "%s%s%s",
			g_ao_test_msg_file_path1,
			g_ao_test_lang_name_tbl[work->language],
			g_ao_test_msg_file_path2);
		work->msg_fs = amFsReadBackground(path);

		// 読み込み待ちへ遷移
		amTaskSetProcedure(tcb, aoTestTaskProcedureLoadMsgFile);
	}

	// 終了判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// 自タスク削除
		amTaskDelete(tcb);

		// デバッグランチャーに戻る
		SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
		SyChangeNextEvt();
	}
}

// ===========================================================================
//! メッセージファイル読み込み
// ===========================================================================
void aoTestTaskProcedureLoadMsgFile(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "NOW LOADING... %d", work->count++);

	// 読み込み完了判定
	if (amFsIsComplete(work->msg_fs)) {

		// ファイル取得
		work->msg_file = work->msg_fs->buf;
		work->msg_fs->buf = NULL;

		// リクエストクリア
		amFsClearRequest(work->msg_fs);
		work->msg_fs = NULL;

		// アドレス変換
		amConvertAddress(work->msg_file);

		// 選択へ遷移
		work->msg_id = 0;
		amTaskSetProcedure(tcb, aoTestTaskProcedureSelect);
	}
}

// ===========================================================================
//! 選択
// ===========================================================================
void aoTestTaskProcedureSelect(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "PLEASE PUSH ANY KEY. %d", work->count++);
	amPrintf(4, 5, "%c:START", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:BACK", GsEnvDebugGetCancelKeyChar());

	// メッセージID変更
	u32 msg_num = AoMsgGetMsgNum(work->msg_file);
	if ((AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) ||
		(AoPadSomeoneMRepeat(GSD_KEY_LEFT) >= 0))
	{
		if (work->msg_id == 0) {
			work->msg_id = (u32)(msg_num - 1);
		}
		else {
			work->msg_id -= 1;
		}
	}
	if ((AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) ||
		(AoPadSomeoneMRepeat(GSD_KEY_RIGHT) >= 0))
	{
		if (work->msg_id >= (u32)(msg_num - 1)) {
			work->msg_id = 0;
		}
		else {
			work->msg_id += 1;
		}
	}

	// メッセージID表示
	amPrintf(4, 8, "MSG-ID:%d", work->msg_id);

	// 開始判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {

		// メッセージ表示開始
		AOE_SYS_MSG_SELECT sel;
		do {
			sel = (AOE_SYS_MSG_SELECT)(
				(work->sel_id++) % AOD_SYS_MSG_SELECT_NUM);
#if  defined(AOD_PLATFORM_WII)
		} while (sel == AOD_SYS_MSG_SELECT_DISABLE);
#else
		} while (0);
#endif // defined(AOD_PLATFORM_WII)
		AoSysMsgStart(work->msg_file, work->msg_id, sel);

		// 次へ遷移
		amTaskSetProcedure(tcb, aoTestTaskProcedureShowMsg);
	}

	// 終了判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// メッセージファイル解放
		amMemFree(work->msg_file);
		work->msg_file = NULL;

		// 言語選択へ戻る
		amTaskSetProcedure(tcb, aoTestTaskProcedureSelectLanguage);
	}
}

// ===========================================================================
//! メッセージ表示
// ===========================================================================
void aoTestTaskProcedureShowMsg(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "SHOW MESSAGE. %d", work->count++);

	// メッセージ終了待ち
	if (AoSysMsgIsFinished()) {

		// 選択へ遷移
		amTaskSetProcedure(tcb, aoTestTaskProcedureSelect);
	}
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
void aoTestTaskDestructor(AMS_TCB* tcb)
{
	// タスクワーク取得
	AOS_TEST_TASK_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb));

	// フォント解放
	if (work->work->is_font) {
		GsFontRelease();
		work->work->is_font = FALSE;
	}

	// ワーク解放
	if (work->work) {
		amMemFree(work->work);
		work->work = NULL;
	}
}


// ***************************************************************************
// タスク2
// ***************************************************************************
// ===========================================================================
//! 初期化
// ===========================================================================
void aoTestTask2ProcedureInit(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK2* work = ((AOS_TEST_TASK_WORK2*)amTaskGetWork(tcb))->work;

	// フォント構築開始
	if (!GsFontIsBuilded() && !GsFontIsBuilding()) {
		GsFontBuild();
		work->is_font = TRUE;
	}

	// 構築待ちへ遷移
	amTaskSetProcedure(tcb, aoTestTask2ProcedureBuildFont);
}

// ===========================================================================
//! フォント構築
// ===========================================================================
void aoTestTask2ProcedureBuildFont(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK2* work = ((AOS_TEST_TASK_WORK2*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "NOW FONT BUILDING... %d", work->count++);

	// 構築完了待ち
	if (GsFontIsBuilded()) {

		// 選択へ遷移
		amTaskSetProcedure(tcb, aoTestTask2ProcedureSelect);
	}
}

// ===========================================================================
//! 選択
// ===========================================================================
void aoTestTask2ProcedureSelect(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK2* work = ((AOS_TEST_TASK_WORK2*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "PLEASE PUSH ANY KEY. %d", work->count++);
	amPrintf(4, 5, "%c:START", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:BACK", GsEnvDebugGetCancelKeyChar());

	// メッセージID変更
	if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
		work->msg_id = (u32)((work->msg_id + 1) % AOD_SYS_MSG_NUM);
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
		work->msg_id =
			(u32)((work->msg_id + (AOD_SYS_MSG_NUM - 1)) % AOD_SYS_MSG_NUM);
	}

	// メッセージID表示
#if _PC
	const char* msg_id_name_tbl[1] = {
		"Dummy",
	};
#elif _XBOX
	const char* msg_id_name_tbl[AOD_SYS_MSG_NUM] = {
		"OVERWRITE_SAVEDATA",
		"STORAGE_CHANGED",
		"STORAGE_NO_SPACE",
		"STORAGE_CANCEL",
		"STORAGE_DEVICE",
		"STORAGE_SAVE_FAILURE",
		"STORAGE_LOAD_FAILURE",
		"NO_SIGNIN",
		"SIGNINCHANGED",
		"IS_NEW_SAVEDATA",
		"NET_ERROR_COMMON",
		"NET_ERROR_OFFLINE",
		"PLEASE_SIGNIN",
		"ERROR_PRIVILEGE",
	};
#elif _PS3
	const char* msg_id_name_tbl[AOD_SYS_MSG_NUM] = {
		"SAVE_CANCEL",
		"STORAGE_SAVE_FAILURE",
		"STORAGE_LOAD_FAILURE",
		"IS_NEW_SAVEDATA",
		"ALL_SAVE_DELETE",
		"NET_ERROR_OFFLINE",
		"NET_ERROR_COMMON",
		"NET_ERROR_PARENTALLOCK",
		"HDD_DATA_DELETE",
	};
#elif _WII
	const char* msg_id_name_tbl[AOD_SYS_MSG_NUM] = {
		"SAVE_LACK_SPACE",
		"SAVE_LACK_FILE",
		"SAVE_DATA_DESTROY",
		"SAVE_DESTROY_DELETE",
		"SAVE_CANCEL",
		"SAVE_TO_WII_DATA_MENU",
		"NET_ERROR_NOSAVE",
		"NET_ERROR_DWC0001",
		"NET_ERROR_E001",
		"NET_ERROR_E002",
		"NET_ERROR_E003",
		"NET_ERROR_E004",
		"NET_ERROR_E005",
		"NET_ERROR_E006",
		"NET_ERROR_E007",
		"NET_ERROR_E008",
		"NET_ERROR_DWC0003",
	};
#elif _IPHONE
	const char* msg_id_name_tbl[1] = {
		"Dummy", //本当はこの項目すら無い
	};
#endif
	for (u32 i = 0; i < AOD_SYS_MSG_NUM; ++i) {
		if (i == work->msg_id) {
			amPrintColor(0xff0000ff);
			amPrint(4, (s32)(8 + i), ">");
		}
		else {
			amPrintColor(0xffffffff);
		}
		amPrintf(6, (s32)(8 + i), "%s", msg_id_name_tbl[i]);
	}
	amPrintColor(0xffffffff);

	// 開始判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {

		// 開始
		AoSysMsgStart((AOE_SYS_MSG_ID)work->msg_id, AOD_SYS_MSG_SELECT_OK);

		// 次へ遷移
		amTaskSetProcedure(tcb, aoTestTask2ProcedureShowMsg);
	}

	// 終了判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// 自タスク削除
		amTaskDelete(tcb);

		// デバッグランチャーに戻る
		SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
		SyChangeNextEvt();
	}
}

// ===========================================================================
//! メッセージ表示
// ===========================================================================
void aoTestTask2ProcedureShowMsg(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK2* work = ((AOS_TEST_TASK_WORK2*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "SHOW MESSAGE. %d", work->count++);

	// メッセージ終了待ち
	if (AoSysMsgIsFinished()) {

		// 選択へ遷移
		amTaskSetProcedure(tcb, aoTestTask2ProcedureSelect);
	}
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
void aoTestTask2Destructor(AMS_TCB* tcb)
{
	// タスクワーク取得
	AOS_TEST_TASK_WORK2* work = ((AOS_TEST_TASK_WORK2*)amTaskGetWork(tcb));

	// フォント解放
	if (work->work->is_font) {
		GsFontRelease();
		work->work->is_font = FALSE;
	}

	// ワーク解放
	if (work->work) {
		amMemFree(work->work);
		work->work = NULL;
	}
}

#endif // defined (MTD_DEBUG)

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
