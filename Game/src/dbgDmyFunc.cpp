// ==========================================================================
/*!
  @file dbgDmyFunc.inc
  @brief デバック用ダミー処理

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: dbgDmyFunc.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "dbgDmyFunc.h"

// #if defined(MTD_DEBUG) ※いくつか未実装イベントがあるためリリース版でも有効にする
// 全文デバックコード


//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void dbgDummyExitLogo(void);
static void dbgDummyLogoMain(MTS_TASK_TCB *tcb);
static void dbgDummyLogoDest(MTS_TASK_TCB *tcb);

static void dbgDummyExitTitle(void);
static void dbgDummyTitleMain(MTS_TASK_TCB *tcb);
static void dbgDummyTitleDest(MTS_TASK_TCB *tcb);

static void dbgDummyExitMainMenu(void);
static void dbgDummyMainMenuMain(MTS_TASK_TCB *tcb);
static void dbgDummyMainMenuDest(MTS_TASK_TCB *tcb);

static void dbgDummyExitMap(void);
static void dbgDummyMapMain(MTS_TASK_TCB *tcb);
static void dbgDummyMapDest(MTS_TASK_TCB *tcb);

static void dbgDummyExitMainGame(void);
static void dbgDummyMainGameMain(MTS_TASK_TCB *tcb);
static void dbgDummyMainGameDest(MTS_TASK_TCB *tcb);

static void dbgDummyExitResult(void);
static void dbgDummyResultMain(MTS_TASK_TCB *tcb);
static void dbgDummyResultDest(MTS_TASK_TCB *tcb);

static void dbgDummyExitOption(void);
static void dbgDummyOptionMain(MTS_TASK_TCB *tcb);
static void dbgDummyOptionDest(MTS_TASK_TCB *tcb);

static void dbgDummyExitEnding(void);
static void dbgDummyEndingMain(MTS_TASK_TCB *tcb);
static void dbgDummyEndingDest(MTS_TASK_TCB *tcb);

static void dbgDummyExitStaffRoll(void);
static void dbgDummyStaffRollMain(MTS_TASK_TCB *tcb);
static void dbgDummyStaffRollDest(MTS_TASK_TCB *tcb);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static s32				dbg_dummy_timer = 0;		//!< ダミールーチン タイマ
//static s32				dbg_dummy_evt_cnt = 0;		//!< ダミールーチン イベントカウンタ
static MTS_TASK_TCB		*dbg_dummy_tcb = NULL;		//!< ダミールーチン タスク

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// ロゴ
// ==========================================================================
// ==========================================================================
// DbgDummyInitLogo
/*!
 *	ダミールーチン ロゴ初期化
 */
// ==========================================================================
void DbgDummyInitLogo(void *arg)
{
	UNREFERENCED_PARAMETER(arg);

	dbg_dummy_timer = 60 * 5;
	dbg_dummy_tcb = MTM_TASK_MAKE_TCB(dbgDummyLogoMain, dbgDummyLogoDest,
					0, 0, 0x2000, 0,
					0, "LOGO");
}

// ==========================================================================
// dbgDummyExitLogo
/*!
 *	ダミールーチン ロゴ終了処理
 */
// ==========================================================================
void dbgDummyExitLogo(void)
{
	mtTaskClearTcb(dbg_dummy_tcb);
}

// ==========================================================================
// dbgDummyLogoMain
/*!
 *	ダミールーチン ロゴメイン
 */
// ==========================================================================
void dbgDummyLogoMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dbg_dummy_timer--;

	amPrintf(2, 2, "LOGO");
	amPrintf(2, 3, "frame : %d", dbg_dummy_timer);

	if (dbg_dummy_timer <= 0) {
		u8	test_arg[8] = {
			0, 1, 2, 3, 4, 5, 6, 7
		};

		/* ロゴ終了 */
		dbgDummyExitLogo();
		/* 次のイベント確定 */
		SyDecideEvtCase(0);
		/* 次のイベントへ移行 */
		//syChangeNextEvt();();
		SyChangeNextEvtArg(8, test_arg);
	}
}

// ==========================================================================
// dbgDummyLogoDest
/*!
 *	ダミールーチン ロゴデストラクタ
 */
// ==========================================================================
void dbgDummyLogoDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	dbg_dummy_tcb = NULL;
}


// ==========================================================================
// タイトル
// ==========================================================================
// ==========================================================================
// DbgDummyInitTitle
/*!
 *	ダミールーチン タイトル初期化
 */
// ==========================================================================
void DbgDummyInitTitle(void *arg)
{
	UNREFERENCED_PARAMETER(arg);

	dbg_dummy_timer = 60 * 5;
	dbg_dummy_tcb = MTM_TASK_MAKE_TCB(dbgDummyTitleMain, dbgDummyTitleDest,
					0, 0, 0x2000, 0,
					0, "TITLE");
}

// ==========================================================================
// dbgDummyExitTitle
/*!
 *	ダミールーチン タイトル終了処理
 */
// ==========================================================================
void dbgDummyExitTitle(void)
{
	mtTaskClearTcb(dbg_dummy_tcb);
}

// ==========================================================================
// dbgDummyTitleMain
/*!
 *	ダミールーチン タイトルメイン
 */
// ==========================================================================
void dbgDummyTitleMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dbg_dummy_timer--;

	amPrintf(2, 2, "TITLE");
	amPrintf(2, 3, "frame : %d", dbg_dummy_timer);

	if (dbg_dummy_timer <= 0) {
		u8	test_arg[8] = {
			0, 1, 2, 3, 4, 5, 6, 7
		};

		/* ロゴ終了 */
		dbgDummyExitTitle();
		/* 次のイベント確定 */
		SyDecideEvtCase(0);
		/* 次のイベントへ移行 */
		//syChangeNextEvt();();
		SyChangeNextEvtArg(8, test_arg);
	}
}

// ==========================================================================
// dbgDummyTitleDest
/*!
 *	ダミールーチン タイトルデストラクタ
 */
// ==========================================================================
void dbgDummyTitleDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	dbg_dummy_tcb = NULL;
}


// ==========================================================================
// メインメニュー
// ==========================================================================
// ==========================================================================
// DbgDummyInitMainMenu
/*!
 *	ダミールーチン メインメニュー初期化
 */
// ==========================================================================
void DbgDummyInitMainMenu(void *arg)
{
	UNREFERENCED_PARAMETER(arg);

	dbg_dummy_timer = 60 * 5;
	dbg_dummy_tcb = MTM_TASK_MAKE_TCB(dbgDummyMainMenuMain, dbgDummyMainMenuDest,
					0, 0, 0x2000, 0,
					0, "MAIN MANU");
}

// ==========================================================================
// dbgDummyExitMainMenu
/*!
 *	ダミールーチン メインメニュー終了処理
 */
// ==========================================================================
void dbgDummyExitMainMenu(void)
{
	mtTaskClearTcb(dbg_dummy_tcb);
}

// ==========================================================================
// dbgDummyMainMenuMain
/*!
 *	ダミールーチン メインメニューメイン
 */
// ==========================================================================
void dbgDummyMainMenuMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dbg_dummy_timer--;

	amPrintf(2, 2, "MAIN MANU");
	amPrintf(2, 3, "frame : %d", dbg_dummy_timer);

	if (dbg_dummy_timer <= 0) {
		u8	test_arg[8] = {
			0, 1, 2, 3, 4, 5, 6, 7
		};

		/* ロゴ終了 */
		dbgDummyExitMainMenu();
		/* 次のイベント確定 */
		SyDecideEvtCase(0);
		/* 次のイベントへ移行 */
		//syChangeNextEvt();();
		SyChangeNextEvtArg(8, test_arg);
	}
}

// ==========================================================================
// dbgDummyMainMenuDest
/*!
 *	ダミールーチン メインメニューデストラクタ
 */
// ==========================================================================
void dbgDummyMainMenuDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	dbg_dummy_tcb = NULL;
}


// ==========================================================================
// マップ
// ==========================================================================
// ==========================================================================
// DbgDummyInitMap
/*!
 *	ダミールーチン マップ初期化
 */
// ==========================================================================
void DbgDummyInitMap(void *arg)
{
	UNREFERENCED_PARAMETER(arg);

	dbg_dummy_timer = 60 * 5;
	dbg_dummy_tcb = MTM_TASK_MAKE_TCB(dbgDummyMapMain, dbgDummyMapDest,
					0, 0, 0x2000, 0,
					0, "MAP");
}

// ==========================================================================
// dbgDummyExitMap
/*!
 *	ダミールーチン マップ終了処理
 */
// ==========================================================================
void dbgDummyExitMap(void)
{
	mtTaskClearTcb(dbg_dummy_tcb);
}

// ==========================================================================
// dbgDummyMapMain
/*!
 *	ダミールーチン マップメイン
 */
// ==========================================================================
void dbgDummyMapMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dbg_dummy_timer--;

	amPrintf(2, 2, "MAP");
	amPrintf(2, 3, "frame : %d", dbg_dummy_timer);

	if (dbg_dummy_timer <= 0) {
		u8	test_arg[8] = {
			0, 1, 2, 3, 4, 5, 6, 7
		};

		/* ロゴ終了 */
		dbgDummyExitMap();
		/* 次のイベント確定 */
		SyDecideEvtCase(0);
		/* 次のイベントへ移行 */
		//syChangeNextEvt();();
		SyChangeNextEvtArg(8, test_arg);
	}
}

// ==========================================================================
// dbgDummyMapDest
/*!
 *	ダミールーチン マップデストラクタ
 */
// ==========================================================================
void dbgDummyMapDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	dbg_dummy_tcb = NULL;
}


// ==========================================================================
// メインゲーム
// ==========================================================================
// ==========================================================================
// DbgDummyInitMainGame
/*!
 *	ダミールーチン メインゲーム初期化
 */
// ==========================================================================
void DbgDummyInitMainGame(void *arg)
{
	UNREFERENCED_PARAMETER(arg);

	dbg_dummy_timer = 60 * 5;
	dbg_dummy_tcb = MTM_TASK_MAKE_TCB(dbgDummyMainGameMain, dbgDummyMainGameDest,
					0, 0, 0x2000, 0,
					0, "MAIN GAME");
}

// ==========================================================================
// dbgDummyExitMainGame
/*!
 *	ダミールーチン メインゲーム終了処理
 */
// ==========================================================================
void dbgDummyExitMainGame(void)
{
	mtTaskClearTcb(dbg_dummy_tcb);
}

// ==========================================================================
// dbgDummyMainGameMain
/*!
 *	ダミールーチン メインゲームメイン
 */
// ==========================================================================
void dbgDummyMainGameMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dbg_dummy_timer--;

	amPrintf(2, 2, "MAIN GAME");
	amPrintf(2, 3, "frame : %d", dbg_dummy_timer);

	if (dbg_dummy_timer <= 0) {
		u8	test_arg[8] = {
			0, 1, 2, 3, 4, 5, 6, 7
		};

		/* ロゴ終了 */
		dbgDummyExitMainGame();
		/* 次のイベント確定 */
		SyDecideEvtCase(0);
		/* 次のイベントへ移行 */
		//syChangeNextEvt();();
		SyChangeNextEvtArg(8, test_arg);
	}
}

// ==========================================================================
// dbgDummyMainGameDest
/*!
 *	ダミールーチン メインゲームデストラクタ
 */
// ==========================================================================
void dbgDummyMainGameDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	dbg_dummy_tcb = NULL;
}


// ==========================================================================
// リザルト
// ==========================================================================
// ==========================================================================
// DbgDummyInitResult
/*!
 *	ダミールーチン リザルト初期化
 */
// ==========================================================================
void DbgDummyInitResult(void *arg)
{
	UNREFERENCED_PARAMETER(arg);

	dbg_dummy_timer = 60 * 5;
	dbg_dummy_tcb = MTM_TASK_MAKE_TCB(dbgDummyResultMain, dbgDummyResultDest,
					0, 0, 0x2000, 0,
					0, "RESULT");
}

// ==========================================================================
// dbgDummyExitResult
/*!
 *	ダミールーチン リザルト終了処理
 */
// ==========================================================================
void dbgDummyExitResult(void)
{
	mtTaskClearTcb(dbg_dummy_tcb);
}

// ==========================================================================
// dbgDummyResultMain
/*!
 *	ダミールーチン リザルトメイン
 */
// ==========================================================================
void dbgDummyResultMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dbg_dummy_timer--;

	amPrintf(2, 2, "RESULT");
	amPrintf(2, 3, "frame : %d", dbg_dummy_timer);

	if (dbg_dummy_timer <= 0) {
		u8	test_arg[8] = {
			0, 1, 2, 3, 4, 5, 6, 7
		};

		/* ロゴ終了 */
		dbgDummyExitResult();
		/* 次のイベント確定 */
		SyDecideEvtCase(0);
		/* 次のイベントへ移行 */
		//syChangeNextEvt();();
		SyChangeNextEvtArg(8, test_arg);
	}
}

// ==========================================================================
// dbgDummyResultDest
/*!
 *	ダミールーチン リザルトデストラクタ
 */
// ==========================================================================
void dbgDummyResultDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	dbg_dummy_tcb = NULL;
}


// ==========================================================================
// オプション
// ==========================================================================
// ==========================================================================
// DbgDummyInitOption
/*!
 *	ダミールーチン オプション初期化
 */
// ==========================================================================
void DbgDummyInitOption(void *arg)
{
	UNREFERENCED_PARAMETER(arg);

	dbg_dummy_timer = 60 * 5;
	dbg_dummy_tcb = MTM_TASK_MAKE_TCB(dbgDummyOptionMain, dbgDummyOptionDest,
					0, 0, 0x2000, 0,
					0, "OPTION");
}

// ==========================================================================
// dbgDummyExitOption
/*!
 *	ダミールーチン オプション終了処理
 */
// ==========================================================================
void dbgDummyExitOption(void)
{
	mtTaskClearTcb(dbg_dummy_tcb);
}

// ==========================================================================
// dbgDummyOptionMain
/*!
 *	ダミールーチン オプションメイン
 */
// ==========================================================================
void dbgDummyOptionMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dbg_dummy_timer--;

	amPrintf(2, 2, "OPTION");
	amPrintf(2, 3, "frame : %d", dbg_dummy_timer);

	if (dbg_dummy_timer <= 0) {
		u8	test_arg[8] = {
			0, 1, 2, 3, 4, 5, 6, 7
		};

		/* ロゴ終了 */
		dbgDummyExitOption();
		/* 次のイベント確定 */
		SyDecideEvtCase(0);
		/* 次のイベントへ移行 */
		//syChangeNextEvt();();
		SyChangeNextEvtArg(8, test_arg);
	}
}

// ==========================================================================
// dbgDummyOptionDest
/*!
 *	ダミールーチン オプションデストラクタ
 */
// ==========================================================================
void dbgDummyOptionDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	dbg_dummy_tcb = NULL;
}


// ==========================================================================
// エンディング
// ==========================================================================
// ==========================================================================
// DbgDummyInitEnding
/*!
 *	ダミールーチン エンディング初期化
 */
// ==========================================================================
void DbgDummyInitEnding(void *arg)
{
	UNREFERENCED_PARAMETER(arg);

	dbg_dummy_timer = 60 * 2;
	dbg_dummy_tcb = MTM_TASK_MAKE_TCB(dbgDummyEndingMain, dbgDummyEndingDest,
					0, 0, 0x2000, 0,
					0, "ENDING");
}

// ==========================================================================
// dbgDummyExitEnding
/*!
 *	ダミールーチン エンディング終了処理
 */
// ==========================================================================
void dbgDummyExitEnding(void)
{
	mtTaskClearTcb(dbg_dummy_tcb);
}

// ==========================================================================
// dbgDummyEndingMain
/*!
 *	ダミールーチン エンディングメイン
 */
// ==========================================================================
void dbgDummyEndingMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dbg_dummy_timer--;

	amPrintf(2, 2, "ENDING");
	amPrintf(2, 3, "frame : %d", dbg_dummy_timer);

	if (dbg_dummy_timer <= 0) {
		u8	test_arg[8] = {
			0, 1, 2, 3, 4, 5, 6, 7
		};

		/* ロゴ終了 */
		dbgDummyExitEnding();
		/* 次のイベント確定 */
		SyDecideEvtCase(0);
		/* 次のイベントへ移行 */
		//syChangeNextEvt();();
		SyChangeNextEvtArg(8, test_arg);
	}
}

// ==========================================================================
// dbgDummyEndingDest
/*!
 *	ダミールーチン エンディングデストラクタ
 */
// ==========================================================================
void dbgDummyEndingDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	dbg_dummy_tcb = NULL;
}


// ==========================================================================
// スタッフロール
// ==========================================================================
// ==========================================================================
// DbgDummyInitStaffRoll
/*!
 *	ダミールーチン スタッフロール初期化
 */
// ==========================================================================
void DbgDummyInitStaffRoll(void *arg)
{
	UNREFERENCED_PARAMETER(arg);

	dbg_dummy_timer = 60 * 2;
	dbg_dummy_tcb = MTM_TASK_MAKE_TCB(dbgDummyStaffRollMain, dbgDummyStaffRollDest,
					0, 0, 0x2000, 0,
					0, "STAFFROLL");
}

// ==========================================================================
// dbgDummyExitStaffRoll
/*!
 *	ダミールーチン スタッフロール終了処理
 */
// ==========================================================================
void dbgDummyExitStaffRoll(void)
{
	mtTaskClearTcb(dbg_dummy_tcb);
}

// ==========================================================================
// dbgDummyStaffRollMain
/*!
 *	ダミールーチン スタッフロールメイン
 */
// ==========================================================================
void dbgDummyStaffRollMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	dbg_dummy_timer--;

	amPrintf(2, 2, "STAFFROLL");
	amPrintf(2, 3, "frame : %d", dbg_dummy_timer);

	if (dbg_dummy_timer <= 0) {
		u8	test_arg[8] = {
			0, 1, 2, 3, 4, 5, 6, 7
		};

		/* ロゴ終了 */
		dbgDummyExitStaffRoll();
		/* 次のイベント確定 */
		SyDecideEvtCase(0);
		/* 次のイベントへ移行 */
		//syChangeNextEvt();();
		SyChangeNextEvtArg(8, test_arg);
	}
}

// ==========================================================================
// dbgDummyStaffRollDest
/*!
 *	ダミールーチン スタッフロールデストラクタ
 */
// ==========================================================================
void dbgDummyStaffRollDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	dbg_dummy_tcb = NULL;
}

//----- Local Functions -----------------------------------------------------



// #endif // #if defined(MTD_DEBUG)

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
