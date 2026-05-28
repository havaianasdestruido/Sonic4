// ===========================================================================
/*!
	@file	aoTestPresence.cpp
	@brief	プレゼンステスト定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "aoTest.h"
#include "ao.h"
#include "aoPresence.h"
#include "gs.h"
#include "gsMainSys.h"

#if defined (MTD_DEBUG)

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

namespace ao {
namespace test {
namespace presence {

//! プレゼンステストクラス
class CMain : public CProc<CMain>, public CTask<CMain>, public CAllocAmNormal
{
public:

	//! コンストラクタ
	CMain();

protected:

	//! デストラクタ
	virtual ~CMain();

	// タスクプロシージャ
	void TaskProcMain();

	// プロシージャ
	void ProcReady();
	void ProcSelect();
	void ProcEnd();

	u32 m_count; //!< 汎用カウンタ
	u32 m_select; //!< 選択番号
};

} // namespace presence
} // namespace test
} // namespace ao

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! プレゼンステスト
// ===========================================================================
void AoTestPresenceStart(void* arg)
{
	UNREFERENCED_PARAMETER(arg);
	new ao::test::presence::CMain;
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace ao {
namespace test {
namespace presence {

// ***************************************************************************
// プレゼンステストクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CMain::CMain()
{
	m_count = 0;

	// タスク作成
	MakeTask(0, "aoTest::Presence");

	// プロシージャ設定
	SetProc(0, &CMain::ProcReady);

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
	// プロシージャ確認
	if (IsProcNone(0)) {
		// 終了
		DeleteOwnTask();
		delete this;
	}
	else {
		// 実行
		Call(0);
	}
}


// ***************************************************************************
// プロシージャ
// ***************************************************************************
// ===========================================================================
//! 準備
// ===========================================================================
void CMain::ProcReady()
{
	m_count = 0;
	m_select = 0;

	// 次へ遷移
	SetOwnProc(&CMain::ProcSelect);
}

// ===========================================================================
//! 選択
// ===========================================================================
void CMain::ProcSelect()
{
	// 画面表示
	amPrintf(4, 4, "PLEASE SELECT PRESENCE. %d", m_count++);
	amPrintf(4, 5, "%c:DECIDE", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	// 選択
	if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
		m_select =
			(u32)((m_select + (AOD_PRESENCE_NUM - 1)) % AOD_PRESENCE_NUM);
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
		m_select =
			(u32)((m_select + 1) % AOD_PRESENCE_NUM);
	}

	// 表示
	const char* name_tbl[AOD_PRESENCE_NUM] = {
		"STANDBY",
		"Z11T",
		"Z11S",
		"Z12T",
		"Z12S",
		"Z13T",
		"Z13S",
		"Z1BT",
		"Z1BS",
		"Z21T",
		"Z21S",
		"Z22T",
		"Z22S",
		"Z23T",
		"Z23S",
		"Z2BT",
		"Z2BS",
		"Z31T",
		"Z31S",
		"Z32T",
		"Z32S",
		"Z33T",
		"Z33S",
		"Z3BT",
		"Z3BS",
		"Z41T",
		"Z41S",
		"Z42T",
		"Z42S",
		"Z43T",
		"Z43S",
		"Z4BT",
		"Z4BS",
		"ZFBT",
		"ZFBS",
		"SS1T",
		"SS1S",
		"SS2T",
		"SS2S",
		"SS3T",
		"SS3S",
		"SS4T",
		"SS4S",
		"SS5T",
		"SS5S",
		"SS6T",
		"SS6S",
		"SS7T",
		"SS7S",
	};
	for (u32 i = 0; i < 9; ++i) {
		s32 no = (s32)m_select - 4 + (s32)i;
		if ((u32)no >= AOD_PRESENCE_NUM) {
			continue;
		}
		if ((i == 0) || (i == 8)) {
			amPrintColor(0xff7f7f7f);
		}
		if (i == 4) {
			amPrintColor(0xff0000ff);
			amPrint(4, (s32)(8 + i), ">");
		}
		else {
			amPrintColor(0xffffffff);
		}
		amPrintf(6, (s32)(8 + i), "%s", name_tbl[no]);
	}
	amPrintColor(0xffffffff);

	// 決定判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {

		// プレゼンス設定
		AoPresenceSet((AOE_PRESENCE)m_select, GsTrialIsTrial());
	}

	// 終了判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// 次へ遷移
		SetOwnProc(&CMain::ProcEnd);
	}
}

// ===========================================================================
//! 終了
// ===========================================================================
void CMain::ProcEnd()
{
	SetOwnProcNone();
}

} // namespace presence
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
