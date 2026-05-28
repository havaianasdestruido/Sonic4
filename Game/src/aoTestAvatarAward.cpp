// ===========================================================================
/*!
	@file	aoTestAvatarAward.cpp
	@brief	アバターアワードテスト

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "aoTest.h"
#include "aoAvatarAward.h"
#include "ao.h"
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
namespace avataraward {

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

#if _XBOX
	// プロシージャ
	void ProcReady();
	void ProcSelect();
	void ProcWait();
	void ProcResult();

	u32 m_count; //!< 汎用カウンタ
	u32 m_select; //!< 選択番号
#endif // _XBOX
};

} // namespace avataraward
} // namespace test
} // namespace ao

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! アバターアワードテスト
// ===========================================================================
void AoTestAvatarAwardStart(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	new ao::test::avataraward::CMain;
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace ao {
namespace test {
namespace avataraward {

// ***************************************************************************
// メイン
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CMain::CMain()
{
	// タスク作成
	MakeTask(0, "aoTest::Trophy");

	// プロシージャ設定
#if _XBOX
	m_count = 0;
	m_select = 0;
	SetProc(0, &CMain::ProcReady);
#else
	SetProc(0, NULL);
#endif // _XBOX

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


#if _XBOX
// ***************************************************************************
// プロシージャ
// ***************************************************************************
// ===========================================================================
//! 準備
// ===========================================================================
void CMain::ProcReady()
{
	SetOwnProc(&CMain::ProcSelect);
}

// ===========================================================================
//! 選択
// ===========================================================================
void CMain::ProcSelect()
{
	// 画面表示
	amPrintf(4, 4, "PLEASE SELECT \'AVATAR AWARD\'. %d", m_count++);
	amPrintf(4, 5, "%c:DECIDE", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	// 選択
	if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
		m_select = (u32)((m_select + 2) % 3);
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
		m_select = (u32)((m_select + 1) % 3);
	}

	// 項目表示
	amPrintf(4, (s32)(8 + m_select), "%s", ">");
	amPrintf(6, 8, "%s", "AVATARITEM1");
	amPrintf(6, 9, "%s", "AVATARITEM2");
	amPrintf(6, 10, "%s", "FINISH");

	// 決定判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
		if (m_select < (u32)AOD_AVATARITEM_NUM) {
			AoAvatarAwardGetStart((AOE_AVATARITEM)m_select);

			// 待ちへ遷移
			SetOwnProc(&CMain::ProcWait);
		}
		else {
			SetOwnProcNone();
		}
	}

	// 終了判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		SetOwnProcNone();
	}
}

// ===========================================================================
//! 待ち
// ===========================================================================
void CMain::ProcWait()
{
	// 画面表示
	amPrintf(4, 4, "PLEASE WAIT... %d", m_count++);

	// 終了待ち
	if (AoAvatarAwardGetIsEnd()) {

		// 結果へ遷移
		SetOwnProc(&CMain::ProcResult);
	}
}

// ===========================================================================
//! 結果
// ===========================================================================
void CMain::ProcResult()
{
	const char* result_tbl[3] = {//AOD_AVATARAWARD_RESULT_NUM] = {
		"SUCCESS",
		"ERROR",
		"ALREADY",
	};

	// 画面表示
	amPrintf(
		4, 4, "RESULT:%s %d", result_tbl[AoAvatarAwardGetResult()], m_count++);
	amPrintf(4, 5, "%c:BACK", GsEnvDebugGetCancelKeyChar());

	// 戻り判定
	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		// 選択へ遷移
		SetOwnProc(&CMain::ProcSelect);
	}
}

#endif // _XBOX

} // namespace avataraward
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
