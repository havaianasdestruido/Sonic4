// ===========================================================================
/*!
	@file	aoTestWiiFatalError.cpp
	@brief	Wii Fatalエラーテスト

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "aoTest.h"
#include "ao.h"
#include "gs.h"
#include "gsMainSys.h"
#include "gsEnvironment.h"

#if defined (MTD_DEBUG)

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

namespace ao {
namespace test {
namespace wiifatal {

// ===========================================================================
//! メイン
// ===========================================================================
class CMain :
	public ao::CProc<CMain>, public ao::CTask<CMain>, public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CMain();

protected:

	//! デストラクタ
	virtual ~CMain();

	// タスクプロシージャ
	void TaskProcMain();

#if _WII
	// プロシージャ
	void ProcSelectLanguage();
	void ProcSelectError();
	void ProcShowMessage();

	u32 m_language;	//!< 選択言語
	u32 m_message;	//!< 選択メッセージ

#endif // _WII
};

} // namespace wiifatal
} // namespace test
} // namespace ao

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! WiiFatalエラーテストテスト
// ===========================================================================
void AoTestWiiFatalError(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	new ao::test::wiifatal::CMain;
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace ao {
namespace test {
namespace wiifatal {

// ***************************************************************************
// メイン
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CMain::CMain()
{
	// タスク作成
	MakeTask(0, "aoTest::WiiFatalError");

	// プロシージャ設定
#if _WII
	m_language = 0;
	m_message = 0;
	SetProc(0, &CMain::ProcSelectLanguage);
#else
	SetProcNone(0);
#endif // _WII

	// タスクプロシージャ設定
	SetTaskProc(0, &CMain::TaskProcMain);

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CMain::~CMain()
{
	// デバッグランチャーに戻る
	SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
	SyChangeNextEvt();
}


// ***************************************************************************
// タスクプロシージャ
// ***************************************************************************
// ===========================================================================
//! メインタスク
// ===========================================================================
void CMain::TaskProcMain()
{
	if (IsProcNone(0)) {
		delete this;
	}
	else {
		Call(0);
	}
}


#if _WII
// ***************************************************************************
// プロシージャ
// ***************************************************************************
// ===========================================================================
//! 言語選択
// ===========================================================================
void CMain::ProcSelectLanguage()
{
	// 画面表示
	amPrintf(4, 4, "PLEASE SELECT LANGUAGE.");
	amPrintf(4, 5, "%c:DECIDE", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	// 選択
	if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
		m_language = (u32)(
			(m_language + (GSD_LANGUAGE_NUM - 1)) % GSD_LANGUAGE_NUM);
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
		m_language = (u32)((m_language + 1) % GSD_LANGUAGE_NUM);
	}

	// 項目表示
	amPrintf(4, (s32)(8 + m_language), "%s", ">");
	amPrintf(6,  8, "%s", "JP");
	amPrintf(6,  9, "%s", "US");
	amPrintf(6, 10, "%s", "FR");
	amPrintf(6, 11, "%s", "IT");
	amPrintf(6, 12, "%s", "GE");
	amPrintf(6, 13, "%s", "SP");

	// 決定判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
		SetOwnProc(&CMain::ProcSelectError);
	}

	// 終了判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		SetOwnProcNone();
	}
}

// ===========================================================================
//! メッセージ選択
// ===========================================================================
void CMain::ProcSelectError()
{
	// 画面表示
	amPrintf(4, 4, "PLEASE SELECT MESSAGE.");
	amPrintf(4, 5, "%c:DECIDE", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	// 選択
	if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
		m_message = (u32)(
			(m_message + (AOD_SYS_MSG_FATAL_NUM - 1)) %
			AOD_SYS_MSG_FATAL_NUM);
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
		m_message = (u32)((m_message + 1) % AOD_SYS_MSG_FATAL_NUM);
	}

	// 項目表示
	amPrintf(4, (s32)(8 + m_message), "%s", ">");
	amPrintf(6,  8, "%s", "WARE_01");
	amPrintf(6,  9, "%s", "NAND_08");
	amPrintf(6, 10, "%s", "NAND_11");
	amPrintf(6, 11, "%s", "NAND_12");

	// 決定判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
		SetOwnProc(&CMain::ProcShowMessage);
	}

	// 終了判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		SetOwnProcNone();
	}
}

// ===========================================================================
//! メッセージ表示
// ===========================================================================
void CMain::ProcShowMessage()
{
	AoSysMsgSetFatalMsgFile(
		GsMemFileGetWiiFatalErrorMessageFile((GSE_LANGUAGE)m_language));
	AoSysMsgShowFatalError((AOE_SYS_MSG_FATAL_ID)m_message);
}
#endif // _WII

} // namespace wiifatal
} // namespace test
} // namespace ao

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
