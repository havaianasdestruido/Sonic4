// ===========================================================================
/*!
	@file	dbgBuyScreen.cpp
	@brief	製品版（完全版）購入画面デバッグ用イベント定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#if defined(MTD_DEBUG)

#include "dbgBuyScreen.h"
#include "dmBuyScreen.h"
#include "ao.h"
#include "gs.h"
#include "gsMainSys.h"
#include "izFade.h"
#if _IPHONE
#include "dbgPadEmu.hpp"
#endif //_IPHONE

#include "mppUtil.h"

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

namespace debug {
namespace buyscreen {

// ===========================================================================
//! メインクラス
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

	//! タスクプロシージャ
	void TaskProcMain();

#if _PC || _XBOX || _PS3 || _IPHONE

	//! 準備
	void ProcReady();

	//! ファイル読み込み
	void ProcLoad();

	//! ファイル読み込み済み
	void ProcLoaded();

	//! テクスチャ構築
	void ProcBuild();

	//! テクスチャ構築済み
	void ProcBuilded();

	//! 実行中
	void ProcExecute();

	//! 結果表示
	void ProcResult();

	//! テクスチャ解放
	void ProcFlush();

	//! テクスチャ解放済み
	void ProcFlushed();

	//! 解放
	void ProcRelease();

	//! ワーク
	DMS_BUY_SCR_WORK m_work;

	//! 汎用カウンタ
	u32 m_count;

	//! 選択番号
	u32 m_select;

#endif // _PC || _XBOX || _PS3 || _IPHONE
};

} // namespace buyscreen
} // namespace debug

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! 製品版（完全版）購入画面デバッグ用イベント開始
// ===========================================================================
void DbgBuyScreenEventStart(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	new debug::buyscreen::CMain;
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace debug {
namespace buyscreen {

// ===========================================================================
//! コンストラクタ
// ===========================================================================
CMain::CMain()
{
	// 体験版として設定
	GsTrialDebugSetTrial(TRUE);

	// タスク作成
	MakeTask(0, "dbgBuyScreen::Event");
	SetTaskProc(0, &CMain::TaskProcMain);

	// プロシージャ設定
#if _PC || _XBOX || _PS3 || _IPHONE
	DmBuyScreenInit(&m_work);
	m_count = 0;
	SetProc(0, &CMain::ProcReady);
#else
	SetProc(0, NULL);
#endif // _PC || _XBOX || _PS3 || _IPHONE

	// タスク開始
	StartTask(0);

	// フェード開始
	IzFadeInitEasyTask(
		IZE_FADE_SET_TYPE_NORMAL,
		0, 0, 0, 255, 0, 0, 0, 255, 1.0f);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CMain::~CMain()
{
	// フェード終了
	IzFadeExit();

	// 製品版として設定
	GsTrialDebugSetTrial(FALSE);

	// デバッグランチャーに戻る
	SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
	SyChangeNextEvt();
}

// ===========================================================================
//! タスクプロシージャ
// ===========================================================================
void CMain::TaskProcMain()
{
	// プロシージャ確認
	if (IsProcNone(0)) {
		// 終了
		delete this;
	}
	else {
		// 実行
		Call(0);
	}
}

#if _PC || _XBOX || _PS3 || _IPHONE

// ===========================================================================
//! 準備
// ===========================================================================
void CMain::ProcReady()
{
	// 画面表示
	amPrintf(4, 4, "PLEASE SELECT. %d", m_count++);
	amPrintf(4, 5, "%c:READY", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	// 決定判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {

		// ファイル読み込みへ遷移
		SetOwnProc(&CMain::ProcLoad);
	}

	// キャンセル判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// 終了
		SetOwnProcNone();
	}
}

// ===========================================================================
//! ファイル読み込み
// ===========================================================================
void CMain::ProcLoad()
{
	// 画面表示
	amPrintf(4, 4, "NOW LOADING... %d", m_count++);

	// ファイル読み込み開始
	if (GetCount() == 0) {
		DmBuyScreenLoadStart(&m_work);
	}

	// ファイル読み込み完了待ち
	if (DmBuyScreenLoadIsFinished(&m_work)) {

		// ファイル読み込み済みに遷移
		SetOwnProc(&CMain::ProcLoaded);
	}
}

// ===========================================================================
//! ファイル読み込み済み
// ===========================================================================
void CMain::ProcLoaded()
{
	// 画面表示
	amPrintf(4, 4, "LOADED. %d", m_count++);

	// 自動でテクスチャ構築へ遷移
	SetOwnProc(&CMain::ProcBuild);
}

// ===========================================================================
//! テクスチャ構築
// ===========================================================================
void CMain::ProcBuild()
{
	// 画面表示
	amPrintf(4, 4, "NOW BUILDING... %d", m_count++);

	// テクスチャ構築開始
	if (GetCount() == 0) {
		DmBuyScreenBuildStart(&m_work);
	}

	// テクスチャ構築完了待ち
	if (DmBuyScreenBuildIsFinished(&m_work)) {

		// テクスチャ構築済みに遷移
		SetOwnProc(&CMain::ProcBuilded);
	}
}

// ===========================================================================
//! テクスチャ構築済み
// ===========================================================================
void CMain::ProcBuilded()
{
	// 画面表示
	amPrintf(4, 4, "BUILDED. %d", m_count++);
	amPrintf(4, 5, "%c:START", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:RELEASE", GsEnvDebugGetCancelKeyChar());

	// 選択番号初期化
	if (GetCount() == 0) {
		m_select = 0;
	}

	// 選択
	if ((AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) ||
		(AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0)) {
		m_select = (u32)((m_select + 1) % 2);
	}

	// 選択項目表示
	amPrintColor(0xffffff00);
	amPrint(4, 8, "UI");
	const char* select_str_tbl[2] = {
		"HIDE", "SHOW",
	};
	for (u32 i = 0; i < 2; ++i) {
		if (i == m_select) {
			amPrintColor(0xff0000ff);
			amPrint(4, (s32)(9 + i), ">");
		}
		else {
			amPrintColor(0xffffffff);
		}
		amPrintf(6, (s32)(9 + i), "%s", select_str_tbl[i]);
	}
	amPrintColor(0xffffffff);

	// 決定判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {

		// 実行中へ遷移
		SetOwnProc(&CMain::ProcExecute);
	}

	// キャンセル判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// テクスチャ解放へ遷移
		SetOwnProc(&CMain::ProcFlush);
	}
}

// ===========================================================================
//! 実行中
// ===========================================================================
void CMain::ProcExecute()
{
	// 画面表示
	amPrintf(4, 4, "%d", m_count++);

	// 開始
	if (GetCount() == 0) {

		AoActSysSetDrawTaskPrio();
		AoActSysSetDrawStateEnable();

		if(true) {
			mppUtil::launchUpsellScreen(0);//sss - new
		}
		else {		
			DmBuyScreenStart(&m_work, (BOOL)m_select, TRUE);
		}
	}

	// 完了判定
	if (DmBuyScreenIsFinished(&m_work)) {

#if !_PS3
		// ゲーム終了判定
		if (DmBuyScreenGetResult(&m_work) == DMD_BUY_SCR_RESULT_BACK) {
#if _XBOX
			amXboxReqExit();
#elif _PC
			amWinMainLoopQuit();
#endif
		}
#endif // !_PS3

		// 結果表示へ遷移
		SetOwnProc(&CMain::ProcResult);
	}
}

// ===========================================================================
//! 結果表示
// ===========================================================================
void CMain::ProcResult()
{
	// 画面表示
	amPrintf(4, 4, "RESULT:%d %d", DmBuyScreenGetResult(&m_work), m_count++);
	amPrintf(4, 5, "%c:RESTART", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:RELEASE", GsEnvDebugGetCancelKeyChar());

	// 選択
	if ((AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) ||
		(AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0)) {
		m_select = (u32)((m_select + 1) % 2);
	}

	// 選択項目表示
	amPrintColor(0xffffff00);
	amPrint(4, 8, "UI");
	const char* select_str_tbl[2] = {
		"HIDE", "SHOW",
	};
	for (u32 i = 0; i < 2; ++i) {
		if (i == m_select) {
			amPrintColor(0xff0000ff);
			amPrint(4, (s32)(9 + i), ">");
		}
		else {
			amPrintColor(0xffffffff);
		}
		amPrintf(6, (s32)(9 + i), "%s", select_str_tbl[i]);
	}
	amPrintColor(0xffffffff);

	// 決定判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {

		// 実行中へ遷移
		SetOwnProc(&CMain::ProcExecute);
	}

	// キャンセル判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// テクスチャ解放へ遷移
		SetOwnProc(&CMain::ProcFlush);
	}
}

// ===========================================================================
//! テクスチャ解放
// ===========================================================================
void CMain::ProcFlush()
{
	// 画面表示
	amPrintf(4, 4, "NOW FLUSHING... %d", m_count++);

	// テクスチャ解放開始
	if (GetCount() == 0) {
		DmBuyScreenFlushStart(&m_work);
	}

	// テクスチャ解放完了待ち
	if (DmBuyScreenFlushIsFinished(&m_work)) {

		// テクスチャ解放済みに遷移
		SetOwnProc(&CMain::ProcFlushed);
	}
}

// ===========================================================================
//! テクスチャ解放済み
// ===========================================================================
void CMain::ProcFlushed()
{
	// 画面表示
	amPrintf(4, 4, "FLUSHED. %d", m_count++);

	// 自動で解放へ遷移
	SetOwnProc(&CMain::ProcRelease);
}

// ===========================================================================
//! 解放
// ===========================================================================
void CMain::ProcRelease()
{
	// 画面表示
	amPrintf(4, 4, "RELEASE. %d", m_count++);

	// 解放
	DmBuyScreenRelease(&m_work);

	// 準備へ遷移
	SetOwnProc(&CMain::ProcReady);
}

#endif // _PC || _XBOX || _PS3 || _IPHONE

} // namespace buyscreen
} // namespace debug

#endif // defined(MTD_DEBUG)

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
