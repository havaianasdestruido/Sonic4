// ===========================================================================
/*!
	@file	aoTest.cpp
	@brief	テスト定義

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

#define AOD_SAVE_SIZE		(16 * 1024)	//!< セーブデータサイズ

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//! ワーク
// ===========================================================================
typedef struct tag_AOS_TEST_WORK {
	u8				data[AOD_SAVE_SIZE];	//!< セーブデータ
	u32				count;	//!< 汎用カウンタ
	u32				acount;	//!< アクセスカウント
	u32				select;	//!< 選択番号
	BOOL			is_save;	//!< セーブフラグ
	BOOL			is_font;	//!< フォント構築した
} AOS_TEST_WORK;

// ===========================================================================
//! タスクワーク(64byte以内)
// ===========================================================================
typedef struct tag_AOS_TEST_TASK_WORK {
	AOS_TEST_WORK*	work;	//!< ワーク
} AOS_TEST_TASK_WORK;

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// タスク
static void aoTestTaskBuildFont(AMS_TCB* tcb);
static void aoTestTaskMain0000(AMS_TCB* tcb);
static void aoTestTaskMain0000a(AMS_TCB* tcb);
static void aoTestTaskMain0001(AMS_TCB* tcb);
static void aoTestTaskMain0002(AMS_TCB* tcb);
static void aoTestTaskMain0003(AMS_TCB* tcb);
static void aoTestTaskMain0004(AMS_TCB* tcb);
static void aoTestTaskMain0005(AMS_TCB* tcb);
#if _WII
static void aoTestTaskMain0006(AMS_TCB* tcb);
static void aoTestTaskMain0007(AMS_TCB* tcb);
#endif // _WII
static void aoTestTaskMainError00(AMS_TCB* tcb);
static void aoTestTaskMainFinish00(AMS_TCB* tcb);
static void aoTestTaskMainFinish01(AMS_TCB* tcb);
static void aoTestTaskMainDestructor(AMS_TCB* tcb);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! テスト開始
// ===========================================================================
void AoTestSaveStart(void* arg)
{
#if 0
	AoTestAceStart(arg);
#else
	UNREFERENCED_PARAMETER(arg);

	AMS_TCB* tcb;

	// タスク作成
	tcb = amTaskMake(
		aoTestTaskBuildFont, aoTestTaskMainDestructor, 0, 0, 0, "aoTest::Main");

	// タスクワーク初期化
	AOS_TEST_TASK_WORK* twork = (AOS_TEST_TASK_WORK*)amTaskGetWork(tcb);
	amZeroMemory(twork, sizeof(AOS_TEST_TASK_WORK));
	twork->work = (AOS_TEST_WORK*)amMemAlloc(sizeof(AOS_TEST_WORK));
	AOS_TEST_WORK* work = twork->work;
	amZeroMemory(work, sizeof(AOS_TEST_WORK));

	// フォント構築開始
	if (!GsFontIsBuilded() && !GsFontIsBuilding()) {
		GsFontBuild();
		work->is_font = TRUE;
	}

	// タスク開始
	amTaskStart(tcb);
#endif // for aoTestAce
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ***************************************************************************
// タスク
// ***************************************************************************
// ===========================================================================
//! フォント構築
// ===========================================================================
void aoTestTaskBuildFont(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "NOW FONT BUILDING... %d", work->count++);

	// 構築完了待ち
	if (GsFontIsBuilded()) {

		// 選択へ遷移
		amTaskSetProcedure(tcb, aoTestTaskMain0000);
	}
}

// ===========================================================================
//! プロシージャ0000
// ===========================================================================
void aoTestTaskMain0000(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "PLEASE PUSH ANY KEY. %d", work->count++);
	amPrintf(4, 5, "%c:START", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	// 開始判定
	s32 decide = AoPadSomeoneStand(GSD_KEY_DECIDE);
	if (decide >= 0) {

		if (AoAccountIsCurrentSignin()) {
			// 次へ遷移
			work->select = 0;
			amTaskSetProcedure(tcb, aoTestTaskMain0001);
		}
		else {
			// カレントアカウント設定
			AoAccountSetCurrentIdStart((u32)decide);
			amTaskSetProcedure(tcb, aoTestTaskMain0000a);
		}
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
//! プロシージャ0000a
// ===========================================================================
void aoTestTaskMain0000a(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// カレントアカウント設定待ち
	if (AoAccountSetCurrentIdIsFinished()) {
		if (AoAccountIsCurrentSignin()) {
			// 次へ遷移
			work->select = 0;
			amTaskSetProcedure(tcb, aoTestTaskMain0001);
		}
		else {
			// 戻る
			amTaskSetProcedure(tcb, aoTestTaskMain0000);
		}
	}
}

// ===========================================================================
//! プロシージャ0001
// ===========================================================================
void aoTestTaskMain0001(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "SAVE? %d", work->count++);
	amPrintf(4, 5, "%c:OK", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:CANCEL", GsEnvDebugGetCancelKeyChar());

	// データ変更
	if (AoPadSomeoneRepeat(GSD_KEY_UP) >= 0) {
		if (work->select >= 255) {
			work->select = 0;
		}
		else {
			work->select += 1;
		}
	}
	if (AoPadSomeoneRepeat(GSD_KEY_DOWN) >= 0) {
		if (work->select == 0) {
			work->select = 255;
		}
		else {
			work->select -= 1;
		}
	}

	// データ表示
	amPrintf(4, 8, "DATA:%d", work->select);

	// キャンセル判定
	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// ロードへ遷移
		amTaskSetProcedure(tcb, aoTestTaskMain0003);
	}

	// セーブ開始判定
	else if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {

		// セーブデータ作成
		amZeroMemory(work->data, AOD_SAVE_SIZE);
		work->data[0] = (u8)work->select;
		work->select = 0;
		for (u32 i = 1; i < AOD_SAVE_SIZE; ++i) {
			work->data[i] = (u8)i;
		}

		// セーブ開始
		AoStorageSaveStart(work->data, AOD_SAVE_SIZE, TRUE);

		// 次へ遷移
		work->acount = 0;
		amTaskSetProcedure(tcb, aoTestTaskMain0002);
	}
}

// ===========================================================================
//! プロシージャ0002
// ===========================================================================
void aoTestTaskMain0002(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "SAVING... %d %d", work->count++, work->acount++);

	// セーブ完了判定
	if (AoStorageSaveIsFinished()) {

		// エラー判定
		if (!AoStorageSaveIsSuccessed()) {
			// エラーへ遷移
			amTaskSetProcedure(tcb, aoTestTaskMainError00);
			return;
		}

		// 次へ遷移
		amTaskSetProcedure(tcb, aoTestTaskMain0003);
	}
}

// ===========================================================================
//! プロシージャ0003
// ===========================================================================
void aoTestTaskMain0003(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "LOAD? %d %d", work->count++, work->acount);
	amPrintf(4, 5, "%c:OK", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:CANCEL", GsEnvDebugGetCancelKeyChar());

	// キャンセル判定
	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// 終了へ遷移
		amTaskSetProcedure(tcb, aoTestTaskMainFinish00);
	}

	// セーブ開始判定
	else if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {

		// ロード開始
		amZeroMemory(work->data, AOD_SAVE_SIZE);
		AoStorageLoadStart(work->data, AOD_SAVE_SIZE);

		// 次へ遷移
		work->acount = 0;
		amTaskSetProcedure(tcb, aoTestTaskMain0004);
	}
}

// ===========================================================================
//! プロシージャ0004
// ===========================================================================
void aoTestTaskMain0004(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "LOADING... %d %d", work->count++, work->acount++);

	// ロード完了判定
	if (AoStorageLoadIsFinished()) {

		// エラー判定
		if (!AoStorageLoadIsSuccessed()) {
			// エラーへ遷移
			amTaskSetProcedure(tcb, aoTestTaskMainError00);
			return;
		}

		for (u32 i = 1; i < AOD_SAVE_SIZE; ++i) {
			amAssert(work->data[i] == (u8)i);
		}

		// ロードデータ取得
		work->select = (u32)work->data[0];

		// 次へ遷移
		amTaskSetProcedure(tcb, aoTestTaskMain0005);
	}
}

// ===========================================================================
//! プロシージャ0005
// ===========================================================================
void aoTestTaskMain0005(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "SUCCESS. %d %d", work->count++, work->acount);
#if _WII
	amPrintf(4, 5, "%c:DELETE", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());
#else
	amPrintf(4, 5, "%c:FINISH", GsEnvDebugGetCancelKeyChar());
#endif // _WII

	// データ表示
	amPrintf(4, 8, "DATA:%d", work->select);

#if _WII
	// 削除開始判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {

		// 削除開始
		AoStorageDeleteStart();

		// 次へ遷移
		work->acount = 0;
		amTaskSetProcedure(tcb, aoTestTaskMain0006);
	}

	else
#endif // _WII

	{
		// 終了判定
		if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

			// 終了へ遷移
			amTaskSetProcedure(tcb, aoTestTaskMainFinish00);
		}
	}
}

#if _WII
// ===========================================================================
//! プロシージャ0006
// ===========================================================================
void aoTestTaskMain0006(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "DELETING... %d %d", work->count++, work->acount++);

	// 削除完了判定
	if (AoStorageDeleteIsFinished()) {

		// エラー判定
		if (!AoStorageDeleteIsSuccessed()) {
			// エラーへ遷移
			amTaskSetProcedure(tcb, aoTestTaskMainError00);
			return;
		}

		// 次へ遷移
		amTaskSetProcedure(tcb, aoTestTaskMain0007);
	}
}

// ===========================================================================
//! プロシージャ0007
// ===========================================================================
void aoTestTaskMain0007(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "SUCCESS. %d %d", work->count++, work->acount);
	amPrintf(4, 5, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	// 終了判定
	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// 終了へ遷移
		amTaskSetProcedure(tcb, aoTestTaskMainFinish00);
	}
}
#endif // _WII

// ===========================================================================
//! エラープロシージャ00
// ===========================================================================
void aoTestTaskMainError00(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "ERROR(%d). %d", AoStorageGetError(), work->count++);
	amPrintf(4, 5, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	// 終了判定
	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// エラークリア
		AoStorageClearError();

		// 終了へ遷移
		amTaskSetProcedure(tcb, aoTestTaskMainFinish00);
	}
}

// ===========================================================================
//! 終了プロシージャ00
// ===========================================================================
void aoTestTaskMainFinish00(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "NOW FINISHING... %d", work->count++);

	// 次へ遷移
	amTaskSetProcedure(tcb, aoTestTaskMainFinish01);
}

// ===========================================================================
//! 終了プロシージャ01
// ===========================================================================
void aoTestTaskMainFinish01(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_WORK* work = ((AOS_TEST_TASK_WORK*)amTaskGetWork(tcb))->work;

	// 画面表示
	amPrintf(4, 4, "NOW FINISHING... %d", work->count++);

	// 最初へ戻る
	amTaskSetProcedure(tcb, aoTestTaskMain0000);
}

// ===========================================================================
//! タスクデストラクタ
// ===========================================================================
void aoTestTaskMainDestructor(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_TASK_WORK* twork = (AOS_TEST_TASK_WORK*)amTaskGetWork(tcb);

	// フォント解放
	if (twork->work->is_font) {
		GsFontRelease();
	}

	// ワーク解放
	if (twork->work) {
		amMemFree(twork->work);
		twork->work = NULL;
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
