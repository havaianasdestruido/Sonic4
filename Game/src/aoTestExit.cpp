// ===========================================================================
/*!
	@file	aoTestExit.cpp
	@brief	ゲーム終了テスト

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
// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

static void aoTestExitTaskProcedureSelect(AMS_TCB* tcb);
static void aoTestExitTaskProcedureExit(AMS_TCB* tcb);

static void aoTestExitExit(void);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! ゲーム終了テスト
// ===========================================================================
void AoTestExitStart(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	AMS_TCB* tcb = amTaskMake(
		aoTestExitTaskProcedureSelect, NULL,
		0, 0, 0, "aoTestExit");
	u32* work = (u32*)amTaskGetWork(tcb);
	*work = 0;
	amTaskStart(tcb);
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ===========================================================================
//! ユーザ選択
// ===========================================================================
void aoTestExitTaskProcedureSelect(AMS_TCB* tcb)
{
	u32& count = *((u32*)amTaskGetWork(tcb));

	// 画面表示
	amPrintf(4, 4, "PLEASE PUSH KEY. %d", count++);
	amPrintf(4, 5, "%c:GAME EXIT", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:CANCEL", GsEnvDebugGetCancelKeyChar());

	// 終了判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {

		// 終了処理
		aoTestExitExit();

		// 終了へ遷移
		amTaskSetProcedure(tcb, aoTestExitTaskProcedureExit);
	}

	// キャンセル判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// タスク削除
		amTaskDelete(tcb);

		// デバッグランチャーに戻る
		SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
		SyChangeNextEvt();
	}
}

// ===========================================================================
//! 終了
// ===========================================================================
void aoTestExitTaskProcedureExit(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	u32& count = *((u32*)amTaskGetWork(tcb));

	amPrintf(4, 4, "EXIT. %d", count++);
}

// ===========================================================================
//! 終了処理
// ===========================================================================
void aoTestExitExit(void)
{
#if _PC

	amWinMainLoopQuit();

#elif _XBOX

	amXboxReqExit();

#elif _PS3

	amPs3ReqExit();

#elif _WII

	amWiiReqExit();

#elif _IPHONE

#endif
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
