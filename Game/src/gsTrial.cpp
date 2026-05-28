// ===========================================================================
/*!
	@file	gsTrial.cpp
	@brief	体験版管理モジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "gsTrial.h"

#if !defined(_DLC) || (!_XBOX && !_PS3)

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

#if defined(MTD_DEBUG) && _XBOX
static void gsTrialDebugDirectCheckTask(AMS_TCB* tcb);
#endif // defined(MTD_DEBUG) && _XBOX

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

#if defined(MTD_DEBUG) && (_XBOX || _PS3)
// ===========================================================================
//! デバッグ用 体験版フラグ
// ===========================================================================
static BOOL g_gs_trial_debug_is_trial = FALSE;
#endif // defined(MTD_DEBUG) && (_XBOX || _PS3)

#if defined(MTD_DEBUG) && _XBOX
// ===========================================================================
//! デバッグ用 体験版フラグ(ダイレクト)
// ===========================================================================
static BOOL g_gs_trial_debug_is_trial_direct = FALSE;

// ===========================================================================
//! デバッグ用 体験版フラグ(ダイレクト)チェックタスクTCBポインタ
// ===========================================================================
static AMS_TCB* g_gs_trial_debug_direct_check_tcb = NULL;
#endif // defined(MTD_DEBUG) && _XBOX

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//	GsTrialInitStart
/*!
	モジュール初期化開始

	@note
	アプリケーション起動時に一度だけ呼び出すようにして下さい。\n
*/
// ===========================================================================
void GsTrialInitStart(void)
{
#if defined(MTD_DEBUG) && (_XBOX || _PS3)
	g_gs_trial_debug_is_trial = FALSE;
#endif // defined(MTD_DEBUG) && (_XBOX || _PS3)

#if defined(MTD_DEBUG) && _XBOX
	g_gs_trial_debug_is_trial_direct = g_gs_trial_debug_is_trial;

	// デバッグ用チェックタスク作成
	g_gs_trial_debug_direct_check_tcb =
		amTaskMake(gsTrialDebugDirectCheckTask, NULL, 0, 0, 0, "gsTrialDirect");
#endif // defined(MTD_DEBUG) && _XBOX
}

// ===========================================================================
//	GsTrialInitIsFinished
/*!
	モジュール初期化終了判定

	@return 真：初期化終了　偽：初期化中
	@note
	GsTrialInitStart関数で開始した初期化処理が完了したか判定します。\n
	初期化処理が完了するまでは、
	このモジュールの機能を使用することはできません。\n
*/
// ===========================================================================
BOOL GsTrialInitIsFinished(void)
{
	return TRUE;
}

// ===========================================================================
//	GsTrialExit
/*!
	モジュール終了処理

	@note
	アプリケーション終了時に一度だけ呼び出すようにして下さい。\n
*/
// ===========================================================================
void GsTrialExit(void)
{
#if defined(MTD_DEBUG) && _XBOX
	if (g_gs_trial_debug_direct_check_tcb) {
		amTaskDelete(g_gs_trial_debug_direct_check_tcb);
		g_gs_trial_debug_direct_check_tcb = NULL;
	}
#endif // defined(MTD_DEBUG) && _XBOX
}

// ===========================================================================
//	GsTrialIsTrial
/*!
	体験版判定

	@return 真：体験版　偽：製品版
	@note
	現在の状態が、体験版かどうか判定します。\n
	PS3とXbox360そしてiPhone以外の場合は、必ずFALSEを返します。\n
	PS3とiPhoneは、初期化以降、この関数の返す値が変わることはありません。\n
	Xbox360の場合は、GsTrialCheckStart関数による状態チェックを行なうたびに
	返す値が変化する可能性がありますが、
	一度FALSEを返した場合は以降TRUEを返すことはなくなります。\n
*/
// ===========================================================================
BOOL GsTrialIsTrial(void)
{
#if _IPHONE
	#if !SONIC4_TRIAL
		return FALSE;
	#else //!SONIC4_TRIAL
		return TRUE;
	#endif //!SONIC4_TRIAL
#elif defined(MTD_DEBUG) && (_XBOX || _PS3)
	return g_gs_trial_debug_is_trial;
#else
	return FALSE;
#endif // defined(MTD_DEBUG) && (_XBOX || _PS3)
}

#if _XBOX
// ===========================================================================
//	GsTrialIsTrialDirect
/*!
	体験版判定(ダイレクト)

	@return 真：体験版　偽：製品版
	@note
	GsTrialIsTrial関数と同じく、体験版か製品版かを判定する関数になりますが、
	GsTrialIsTrial関数は、GsTrialCheckStart関数を呼び出さなければ
	戻り値が変化しないのに対して、
	この関数は、リアルタイムに変化します。\n
	(製品版ライセンスを持ったプロフィールがサインインしたら即時にFALSEを返す)\n
*/
// ===========================================================================
BOOL GsTrialIsTrialDirect(void)
{
#if defined(MTD_DEBUG) && _XBOX
	return g_gs_trial_debug_is_trial_direct;
#else
	return FALSE;
#endif // defined(MTD_DEBUG) && _XBOX
}

// ===========================================================================
//	GsTrialApplyDirect
/*!
	製品版フラグ設定

	@note
	通常は、GsTrialCheckStart関数を呼び出すことで、
	GsTrialIsTrial関数の戻り値が変化しますが、
	GsTrialIsTrialDirect関数がFALSEを返し、
	GsTrialIsTrial関数がTRUEを返す状態でのみ、
	この関数を呼び出すことで、
	GsTrialIsTrial関数がFALSEを返すようになります。\n
	GsTrialCheckStart関数は即時にフラグが切り替わらない可能性があるのに対し、
	この関数は、正しく呼び出されたのであれば、
	即時にフラグが切り替わることが保証されます。\n
*/
// ===========================================================================
void GsTrialApplyDirect(void)
{
#if defined(MTD_DEBUG) && _XBOX
	amAssert(g_gs_trial_debug_is_trial);
	amAssert(!g_gs_trial_debug_is_trial_direct);
	g_gs_trial_debug_is_trial = FALSE;
	g_gs_trial_debug_is_trial_direct = FALSE;
#endif // defined(MTD_DEBUG) && _XBOX
}
#endif // _XBOX

// ===========================================================================
//	GsTrialCheckStart
/*!
	状態チェック開始

	@note
	Xbox360の場合は、体験版か製品版かの状態が動的に変化するので、
	体験版かどうかの判定を行う前に、
	この関数で現在の状態のチェックを行なうようにして下さい。\n
	この状態に変化がある場合は、
	この関数呼出し後GsTrialCheckIsFinished関数がTRUEを返した段階で
	GsTrialIsTrial関数の戻り値が変化します。\n
	既に製品版となっている場合と、Xbox360以外の場合は、
	この関数は何も行ないません。\n
*/
// ===========================================================================
void GsTrialCheckStart(void)
{
#if defined(MTD_DEBUG) && _XBOX
	if (g_gs_trial_debug_is_trial && !g_gs_trial_debug_is_trial_direct) {
		g_gs_trial_debug_is_trial = FALSE;
	}
#endif // defined(MTD_DEBUG) && _XBOX
}

// ===========================================================================
//	GsTrialCheckIsFinished
/*!
	状態チェック完了判定

	@return 真：チェック完了　偽：チェック中
	@note
	GsTrialCheckStart関数で開始した状態チェックが完了したか判定します。\n
*/
// ===========================================================================
BOOL GsTrialCheckIsFinished(void)
{
	// empty
	return TRUE;
}

// ===========================================================================
//	GsTrialStoreStart
/*!
	製品版の購入処理開始

	@note
	製品版の購入のためのインゲームストアの表示を開始します。\n
	既に製品版の状態で呼び出しても何も行ないません。\n
	Xbox360とPS3以外の場合は呼び出しても何も行ないません。\n
*/
// ===========================================================================
void GsTrialStoreStart(void)
{
	// empty
}

// ===========================================================================
//	GsTrialStoreIsFinished
/*!
	製品版の購入処理完了判定

	@return 真：完了　偽：処理中
	@note
	GsTrialStoreStart関数で開始した購入処理が完了したか判定します。\n
*/
// ===========================================================================
BOOL GsTrialStoreIsFinished(void)
{
	// empty
	return TRUE;
}

// ===========================================================================
//	GsTrialDebugSetTrial
/*!
	デバッグ用 体験版設定

	@param is_trial	[in] 真：体験版　偽：製品版
	@note
	デバッグを目的として、
	現在の体験版、製品版の状態を変更することができます。\n
	強制的に即時設定を行ないますので、
	初期化中や状態チェック中には呼び出さないことをお勧めします。\n
	Xbox360での「一度製品版になれば以降ずっと製品版」という制限に関わらず
	状態が変化します。\n
	MTD_DEBUGが定義されていない環境では何も行ないません。\n
*/
// ===========================================================================
void GsTrialDebugSetTrial(BOOL is_trial)
{
#if defined(MTD_DEBUG) && (_XBOX || _PS3)
	g_gs_trial_debug_is_trial = is_trial;
#if _XBOX
	g_gs_trial_debug_is_trial_direct = g_gs_trial_debug_is_trial;
#endif // _XBOX
#else
	UNREFERENCED_PARAMETER(is_trial);
	// empty
#endif // defined(MTD_DEBUG) && (_XBOX || _PS3)
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

#endif // !defined(_DLC) || (!_XBOX && !_PS3)

#if defined(MTD_DEBUG)
#include "ao.h"
#include "gs.h"
#include "gsMainSys.h"

namespace gs {

// ===========================================================================
//! デバッグ用イベントクラス
// ===========================================================================
class CTrialDebug :
	public ao::CProc<CTrialDebug>, public ao::CTask<CTrialDebug>
{
public:

	//! コンストラクタ
	CTrialDebug();

protected:

	//! デストラクタ
	virtual ~CTrialDebug();

	// タスクプロシージャ
	void TaskProcMain();

	// プロシージャ
	void ProcReady();
	void ProcSelect();
	void ProcCheck();
	void ProcStore();
	void ProcEnd();

	u32 m_select; //!< 選択番号
};

} // namespace gs

// ===========================================================================
//! デバッグ用イベント
// ===========================================================================
void GsTrialDebugEvent(void* arg)
{
	UNREFERENCED_PARAMETER(arg);
	new gs::CTrialDebug;
}

namespace gs {

// ***************************************************************************
// デバッグ用イベントクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CTrialDebug::CTrialDebug()
{
	// プロシージャ設定
	SetProc(0, &CTrialDebug::ProcReady);

	// タスク作成
	MakeTask(0, "gsTrial::Debug");

	// タスクプロシージャ設定
	SetTaskProc(0, &CTrialDebug::TaskProcMain);

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CTrialDebug::~CTrialDebug()
{
	// デバッグランチャーに戻る
	SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
	SyChangeNextEvt();
}

// ===========================================================================
//! タスク
// ===========================================================================
void CTrialDebug::TaskProcMain()
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

// ===========================================================================
//! 準備
// ===========================================================================
void CTrialDebug::ProcReady()
{
	m_select = 0;
	SetOwnProc(&CTrialDebug::ProcSelect);
}

// ===========================================================================
//! 選択
// ===========================================================================
void CTrialDebug::ProcSelect()
{
	amPrint(4, 4, "PLEASE SELECT.");
	amPrintf(4, 5, "%c:DECIDE", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());
	if (GsTrialIsTrial()) {
		amPrint(4, 7, "MODE : TRIAL");
	}
	else {
		amPrint(4, 7, "MODE : FULL");
	}

	// 選択
	if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
		m_select = (u32)((m_select + 4) % 5);
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
		m_select = (u32)((m_select + 1) % 5);
	}

	const char* cnt_name_tbl[5] = {
		"CHECK",
		"STORE",
		"SET TRIAL",
		"SET FULL",
		"FINISH",
	};
	for (u32 i = 0; i < 5; ++i) {
		if (i == m_select) {
			amPrintColor(0xff0000ff);
			amPrint(4, (s32)(9 + i), ">");
		}
		else {
			amPrintColor(0xffffffff);
		}
		amPrintf(5, (s32)(9 + i), "%s", cnt_name_tbl[i]);
	}
	amPrintColor(0xffffffff);

	// 開始判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
		switch (m_select) {
		case 0:
			SetOwnProc(&CTrialDebug::ProcCheck);
			break;
		case 1:
			SetOwnProc(&CTrialDebug::ProcStore);
			break;
		case 2:
			GsTrialDebugSetTrial(TRUE);
			break;
		case 3:
			GsTrialDebugSetTrial(FALSE);
			break;
		default:
			SetOwnProc(&CTrialDebug::ProcEnd);
			break;
		}
	}

	// 終了判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		SetOwnProc(&CTrialDebug::ProcEnd);
	}
}

// ===========================================================================
//! 確認
// ===========================================================================
void CTrialDebug::ProcCheck()
{
	amPrint(4, 4, "CHECK - PLEASE WAIT.");

	if (GetCount() == 0) {
		// チェック開始
		GsTrialCheckStart();
	}

	// チェック完了判定
	if (GsTrialCheckIsFinished()) {
		SetOwnProc(&CTrialDebug::ProcSelect);
	}
}

// ===========================================================================
//! インゲームストア
// ===========================================================================
void CTrialDebug::ProcStore()
{
	amPrint(4, 4, "STORE - PLEASE WAIT.");

	if (GetCount() == 0) {
		// ストア開始
		GsTrialStoreStart();
	}

	// ストア完了判定
	if (GsTrialStoreIsFinished()) {
		SetOwnProc(&CTrialDebug::ProcSelect);
	}
}

// ===========================================================================
//! 終了
// ===========================================================================
void CTrialDebug::ProcEnd()
{
	SetOwnProcNone();
}

} // namespace gs

#if defined(MTD_DEBUG) && _XBOX
// ===========================================================================
//! デバッグ用 体験版フラグ(ダイレクト)チェックタスク
// ===========================================================================
void gsTrialDebugDirectCheckTask(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

#if 0
	if (g_gs_trial_debug_is_trial_direct) {
		if (AoPadIsConnected(1)) {
			if (AoPadPortDirect(1) & KEY_L1) {
				g_gs_trial_debug_is_trial_direct = FALSE;
			}
		}
	}
	if (!g_gs_trial_debug_is_trial_direct) {
		if (AoPadIsConnected(1)) {
			if (AoPadPortDirect(1) & KEY_R1) {
				g_gs_trial_debug_is_trial = TRUE;
				g_gs_trial_debug_is_trial_direct = TRUE;
			}
		}
	}
#endif
}
#endif // defined(MTD_DEBUG) && _XBOX

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
