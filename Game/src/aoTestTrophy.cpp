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
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

namespace ao {
namespace test {
namespace trophy {

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

	// プロシージャ
	void ProcReady();
	void ProcInstall();
	void ProcSelect();
	void ProcError();

	enum {
		TROPHY_NUM	= 12,	//!< トロフィー数
	};

	u32 m_select; //!< 選択番号
};

} // namespace trophy
} // namespace test
} // namespace ao

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

#if _XBOX
// ===========================================================================
//! 実績ID配列
// ===========================================================================
static const u32 g_ao_test_trophy_id_tbl_xbox360[] = {
	ACHIEVEMENT_ACHIEVEMENT01,
	ACHIEVEMENT_ACHIEVEMENT02,
	ACHIEVEMENT_ACHIEVEMENT03,
	ACHIEVEMENT_ACHIEVEMENT04,
	ACHIEVEMENT_ACHIEVEMENT05,
	ACHIEVEMENT_ACHIEVEMENT06,
	ACHIEVEMENT_ACHIEVEMENT07,
	ACHIEVEMENT_ACHIEVEMENT08,
	ACHIEVEMENT_ACHIEVEMENT09,
	ACHIEVEMENT_ACHIEVEMENT10,
	ACHIEVEMENT_ACHIEVEMENT11,
	ACHIEVEMENT_ACHIEVEMENT12,
};
#endif // _XBOX

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! テスト開始
// ===========================================================================
void AoTestTrophyStart(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	new ao::test::trophy::CMain;
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace ao {
namespace test {
namespace trophy {

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
//! タスクプロシージャ メイン
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
	// トロフィーエラークリア
	AoTrophyClearError();

//	// インストール開始
//	AoTrophyInstallStart();

	// インストール完了待ちへ遷移
	SetOwnProc(&CMain::ProcInstall);
}

// ===========================================================================
//! インストール
// ===========================================================================
void CMain::ProcInstall()
{
	// 画面表示
	amPrint(4, 4, "NOW INSTALLING...");

	// インストール完了判定
//	if (AoTrophyInstallIsFinished()) {
	if (1) {

		// 成功判定
		if (AoTrophyInstallIsSuccess()) {

			// トロフィー獲得監視タスク生成
//			AoTrophyAcquisitionTaskStart();

			// 成功
			SetOwnProc(&CMain::ProcSelect);
		}
		else {
			// 失敗
			SetOwnProc(&CMain::ProcError);
		}
	}
}

// ===========================================================================
//! 選択
// ===========================================================================
void CMain::ProcSelect()
{
	// 初期化
	if (GetCount() == 0) {
		m_select = 0;
	}

	// 選択操作
	if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
		if (m_select == 0) {
			m_select = TROPHY_NUM;
		}
		else {
			m_select -= 1;
		}
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
		if (m_select >= TROPHY_NUM) {
			m_select = 0;
		}
		else {
			m_select += 1;
		}
	}

	// エラー判定
	if (AoTrophyIsError()) {
		// トロフィー獲得監視タスク削除
//		AoTrophyAcquisitionTaskEnd();

		// エラーへ遷移
		SetOwnProc(&CMain::ProcError);
		return;
	}

	// 画面表示
	amPrint(4, 4, "SELECT UNLOCK TROPHY.");
	amPrintf(4, 5, "%c:DECIDE", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());
	for (u32 i = 0; i < (u32)TROPHY_NUM; ++i) {
		if (i == m_select) {
			amPrint(5, (s32)(8 + i), ">");
		}
		amPrintf(6, (s32)(8 + i), "TROPHY%03d", i);
	}
	if (m_select == (u32)TROPHY_NUM) {
		amPrint(5, 8 + TROPHY_NUM + 1, ">");
	}
	amPrint(6, 8 + TROPHY_NUM + 1, "FINISH");

	// トロフィー取得判定
	if ((m_select < (u32)TROPHY_NUM) &&
		(AoPadSomeoneMRepeat(GSD_KEY_DECIDE) >= 0))
	{
		// トロフィー獲得
#if _XBOX
		AoTrophyAcquisition(g_ao_test_trophy_id_tbl_xbox360[m_select]);
#else
		AoTrophyAcquisition(m_select);
#endif // _XBOX
	}

	// 終了判定
	if (((m_select >= (u32)TROPHY_NUM) &&
		 (AoPadSomeoneMRepeat(GSD_KEY_DECIDE) >= 0)) ||
		(AoPadSomeoneMRepeat(GSD_KEY_CANCEL) >= 0))
	{
		// トロフィー獲得監視タスク削除
	//	AoTrophyAcquisitionTaskEnd();

		// 終了
		SetOwnProcNone();
	}
}

// ===========================================================================
//! エラー
// ===========================================================================
void CMain::ProcError()
{
	// 画面表示
	amPrintf(4, 4, "ERROR.(%d)", AoTrophyGetError());
	amPrintf(4, 5, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	// 終了判定
	if (AoPadSomeoneMRepeat(GSD_KEY_CANCEL) >= 0) {

		// 終了
		SetOwnProcNone();
	}
}

} // namespace trophy
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
