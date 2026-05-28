// ===========================================================================
/*!
	@file	dmSaveAttentionPS3.cpp
	@brief	PS3オートセーブに関する注意表示モジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#if _PS3

#include "dmSave.h"
#include "dmSaveAttentionPS3.h"
#include "ao.h"
#include "izFade.h"
#include "gs.h"
#include "gsMainSys.h"

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

namespace dm {
namespace save {
namespace attention {

// ===========================================================================
//! メインクラス
// ===========================================================================
class CMain :
	public ao::CProc<CMain>, public ao::CTask<CMain>, public ao::CAllocAmNormal
{
public:

#if defined(MTD_DEBUG)
	//! コンストラクタ
	CMain(BOOL is_debug = FALSE);
#else
	//! コンストラクタ
	CMain();
#endif // defined(MTD_DEBUG)

protected:

	//! デストラクタ
	virtual ~CMain();

	//! タスクプロシージャ
	void TaskProcMain();

	//! 準備
	void ProcReady();

	//! フェード待ち
	void ProcWaitFade();

	//! 表示
	void ProcShow();

	//! 終了
	void ProcEnd();

#if defined(MTD_DEBUG)
	//! デバッグフラグ
	BOOL m_is_debug;
#endif // defined(MTD_DEBUG)
};

} // namespace attention
} // namespace save
} // namespace dm

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! PS3オートセーブに関する注意表示イベント開始
// ===========================================================================
void DmSaveAttentionStartPS3(void* arg)
{
	new dm::save::attention::CMain();
}

#if defined(MTD_DEBUG)
// ===========================================================================
//! PS3オートセーブに関する注意表示確認用デバッグイベント開始
// ===========================================================================
void DmSaevAttentionStartPS3Debug(void* arg)
{
	new dm::save::attention::CMain(TRUE);
}
#endif // defined(MTD_DEBUG)

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace dm {
namespace save {
namespace attention {

// ***************************************************************************
// メインクラス
// ***************************************************************************
#if defined(MTD_DEBUG)
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CMain::CMain(BOOL is_debug)
{
	m_is_debug = is_debug;

	// タスク作成
	MakeTask(0, "dmSaveAttension");
	SetTaskProc(0, &CMain::TaskProcMain);

	// プロシージャ設定
	if (GsTrialIsTrial()) {
		SetProc(0, &CMain::ProcEnd);
	}
	else {
		SetProc(0, &CMain::ProcReady);
	}

	// タスク開始
	StartTask(0);
}
#else
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CMain::CMain()
{
	// タスク作成
	MakeTask(0, "dmSaveAttension");
	SetTaskProc(0, &CMain::TaskProcMain);

	// プロシージャ設定
	if (GsTrialIsTrial()) {
		SetProc(0, &CMain::ProcEnd);
	}
	else {
		SetProc(0, &CMain::ProcReady);
	}

	// タスク開始
	StartTask(0);
}
#endif // defined(MTD_DEBUG)

// ===========================================================================
//! デストラクタ
// ===========================================================================
CMain::~CMain()
{
	// アクション設定デフォルトへ戻す
	AoActSysSetDrawStateEnable();
	AoActSysSetDrawTaskPrio();
	AoActSysSetDrawState();

#if defined(MTD_DEBUG)
	if (m_is_debug) {
		// デバッグランチャーへ戻る
		SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
		SyChangeNextEvt();
		return;
	}
#endif // defined(MTD_DEBUG)

	// セガロゴへ遷移
	SyDecideEvtCase(0);
	SyChangeNextEvt();
}

// ===========================================================================
//! タスクプロシージャ
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

// ===========================================================================
//! 準備
// ===========================================================================
void CMain::ProcReady()
{
	// アクション設定
	AoActSysSetDrawStateEnable(FALSE);
	AoActSysSetDrawTaskPrio(IZD_FADE_DT_PRIO_DEF + 1);

	// 黒フェード開始
	IzFadeInitEasyTask(
		IZE_FADE_SET_TYPE_TAKEOEVER, 0, 0, 0, 255, 0, 0, 0, 255, 1.0f);

	// フェード待ちへ遷移
	SetOwnProc(&CMain::ProcWaitFade);
}

// ===========================================================================
//! フェード待ち
// ===========================================================================
void CMain::ProcWaitFade()
{
	// フェード終了待ち
	if (IzFadeIsEnd()) {
		// 表示へ遷移
		SetOwnProc(&CMain::ProcShow);
	}
}

// ===========================================================================
//! 表示
// ===========================================================================
void CMain::ProcShow()
{
	// 表示開始
	if (GetCount() == 0) {
		DmSaveAttenMsgStart();
	}

	// 表示完了待ち
	if (DmSaveIsExit()) {
		// 終了へ遷移
		SetOwnProc(&CMain::ProcEnd);
	}
}

// ===========================================================================
//! 終了
// ===========================================================================
void CMain::ProcEnd()
{
	// 白フェード開始
	if (GetCount() == 0) {
		IzFadeInitEasyTask(
			IZE_FADE_SET_TYPE_TAKEOEVER,
			0, 0, 0, 255, 255, 255, 255, 255, 60.0f);
	}

	// フェード終了待ち
	if (IzFadeIsEnd()) {
		// 終了
		SetOwnProcNone();
	}
}

} // namespace attention
} // namespace save
} // namespace dm

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
