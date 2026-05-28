// ===========================================================================
/*!
	@file	gsTrialXbox360.cpp
	@brief	体験版管理モジュール定義(Xbox360)

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "gsTrial.h"
#include "ao.h"

#if defined(_DLC) && _XBOX

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

namespace gs {

// ===========================================================================
//! 体験版モジュール状態チェッククラス
// ===========================================================================
class CTrialCheck :
	public ao::CTask<CTrialCheck>, public ao::CThread<CTrialCheck>,
	public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CTrialCheck();

	//! デストラクタ
	virtual ~CTrialCheck();

protected:

	//! チェック待ちタスクプロシージャ
	void TaskProcWait();

	//! チェックスレッドプロシージャ
	void ThreadProcCheck();

	//! 体験版フラグ
	BOOL m_is_trial;
};

// ===========================================================================
//! 体験版モジュールストアクラス
// ===========================================================================
class CTrialStore : public ao::CTask<CTrialStore>, public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CTrialStore();

	//! デストラクタ
	virtual ~CTrialStore();

protected:

	// タスクプロシージャ
	void TaskProcWait();
	void TaskProcStore();
};

} // namespace gs

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// ===========================================================================
//! システム通知コールバック
// ===========================================================================
static void gsTrialSystemNoticeCallback(
	DWORD dwNotificationID, ULONG_PTR ulParam, void* arg);

// ===========================================================================
//! 状態チェック開始(サブ)
// ===========================================================================
static void gsTrialCheckStartSub(void);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//! 体験版フラグ
// ===========================================================================
static BOOL g_gs_trial_is_trial = TRUE;

// ===========================================================================
//! 体験版フラグ(サブ)
// ===========================================================================
static BOOL g_gs_trial_is_trial_sub = TRUE;

// ===========================================================================
//! 体験版チェックリクエストフラグ
// ===========================================================================
static BOOL g_gs_trial_is_check_request = FALSE;

// ===========================================================================
//! 体験版フラグ更新フラグ
// ===========================================================================
static BOOL g_gs_trial_is_trial_update = FALSE;

// ===========================================================================
//! 体験版モジュール状態チェッククラス
// ===========================================================================
static gs::CTrialCheck* g_gs_trial_check = NULL;

// ===========================================================================
//! 体験版モジュールストアクラス
// ===========================================================================
static gs::CTrialStore* g_gs_trial_store = NULL;

// ===========================================================================
//! 製品版のオファーID
// ===========================================================================
static const ULONGLONG g_gs_trial_offer_id =
	(((ULONGLONG)TITLEID_SONIC_4_EPISODE_I) << 32) | 0x00000001;

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
	// パラメータ初期化
	g_gs_trial_is_trial = TRUE;
	g_gs_trial_is_trial_sub = TRUE;
	g_gs_trial_is_check_request = FALSE;
	g_gs_trial_is_trial_update = FALSE;

	// システム通知コールバック登録
	AoSysSetSystemNoticeCallbackXbox360(::gsTrialSystemNoticeCallback, NULL);

	// 初回チェック開始
	GsTrialCheckStart();
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
	if (g_gs_trial_check) {
		return FALSE;
	}
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
	if (g_gs_trial_check) {
		delete g_gs_trial_check;
		g_gs_trial_check = NULL;
	}
	if (g_gs_trial_store) {
		delete g_gs_trial_store;
		g_gs_trial_store = NULL;
	}
}

// ===========================================================================
//	GsTrialIsTrial
/*!
	体験版判定

	@return 真：体験版　偽：製品版
	@note
	現在の状態が、体験版かどうか判定します。\n
	PS3とXbox360以外の場合は、必ずFALSEを返します。\n
	PS3の場合は、初期化以降、この関数の返す値が変わることはありません。\n
	Xbox360の場合は、GsTrialCheckStart関数による状態チェックを行なうたびに
	返す値が変化する可能性がありますが、
	一度FALSEを返した場合は以降TRUEを返すことはなくなります。\n
*/
// ===========================================================================
BOOL GsTrialIsTrial(void)
{
	return g_gs_trial_is_trial;
}

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
	return g_gs_trial_is_trial_sub;
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
	amAssert(g_gs_trial_is_trial);
	amAssert(!g_gs_trial_is_trial_sub);
	if (!g_gs_trial_is_trial_sub) {
		g_gs_trial_is_trial = FALSE;
	}
}

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
	// 体験版の時のみ処理する
	if (GsTrialIsTrial()) {

		// サブチェック済みなら処理しない
		if (!g_gs_trial_is_trial_sub) {
			g_gs_trial_is_trial = FALSE;
			return;
		}

		g_gs_trial_is_trial_update = TRUE;
		if (g_gs_trial_check == NULL) {
			g_gs_trial_is_check_request = FALSE;
			g_gs_trial_check = new gs::CTrialCheck;
		}
		else {
			g_gs_trial_is_check_request = TRUE;
		}
	}
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
	if (g_gs_trial_check) {
		return FALSE;
	}
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
	// 体験版の時のみ処理する
	if (GsTrialIsTrial()) {
		g_gs_trial_store = new gs::CTrialStore;
	}
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
	if (g_gs_trial_store) {
		return FALSE;
	}
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
#if defined(MTD_DEBUG)
	g_gs_trial_is_trial = is_trial;
#else
	UNREFERENCED_PARAMETER(is_trial);
	// empty
#endif // defined(MTD_DEBUG)
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace gs {

// ***************************************************************************
// 体験版モジュール状態チェッククラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CTrialCheck::CTrialCheck()
{
	// 体験版としてチェック開始
	m_is_trial = TRUE;

	// タスク作成
	MakeTask(0, "gsTrial::Check");

	// タスクプロシージャ設定
	SetTaskProc(0, &CTrialCheck::TaskProcWait);

	// スレッドプロシージャ設定
	SetThreadProc(0, &CTrialCheck::ThreadProcCheck);

	// スレッド開始
	StartThread(0, (AMD_CORE)0, (u32)THREAD_PRIORITY_BELOW_NORMAL);

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CTrialCheck::~CTrialCheck()
{
	amAssert(g_gs_trial_check);
	g_gs_trial_check = NULL;
}

// ===========================================================================
//! チェック待ちタスクプロシージャ
// ===========================================================================
void CTrialCheck::TaskProcWait()
{
	// スレッド終了判定
	if (IsEndThread(0)) {

		amSystemLog("XContentGetLicenseMask Called.\n");

		// 結果設定
		if (g_gs_trial_is_trial_sub) {
			g_gs_trial_is_trial_sub = m_is_trial;
		}

		// 本フラグに反映
		if (g_gs_trial_is_trial_update) {
			if (g_gs_trial_is_trial) {
				g_gs_trial_is_trial = g_gs_trial_is_trial_sub;
			}
		}

		// 製品版になっているならリクエストを無効化
		if (!g_gs_trial_is_trial_sub) {
			g_gs_trial_is_check_request = FALSE;
		}

		// リクエストチェック
		if (g_gs_trial_is_check_request) {

			// リクエストフラグクリア
			g_gs_trial_is_check_request = FALSE;

			// 再チェック開始
			SetThreadProc(0, &CTrialCheck::ThreadProcCheck);
			StartThread(0, (AMD_CORE)0, (u32)THREAD_PRIORITY_BELOW_NORMAL);
		}
		else {
			// 本フラグ更新フラグをOFF
			g_gs_trial_is_trial_update = FALSE;

			// 終了
			delete this;
		}
	}
}

// ===========================================================================
//! チェックスレッドプロシージャ
// ===========================================================================
void CTrialCheck::ThreadProcCheck()
{
	// 体験版として設定しておく
	m_is_trial = TRUE;

	DWORD mask;
	if (XContentGetLicenseMask(&mask, NULL) == ERROR_SUCCESS) {
		if ((mask & 0x01) == 0x01) {
			// 製品版
			m_is_trial = FALSE;
		}
	}
}


// ***************************************************************************
// 体験版モジュールストアクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CTrialStore::CTrialStore()
{
	// タスク作成
	MakeTask(0, "gsTrial::Store");

	// タスクプロシージャ設定
	SetTaskProc(0, &CTrialStore::TaskProcWait);

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CTrialStore::~CTrialStore()
{
	amAssert(g_gs_trial_store);
	g_gs_trial_store = NULL;
}

// ===========================================================================
//! ストア開始待ち
// ===========================================================================
void CTrialStore::TaskProcWait()
{
	// カレントユーザ有効判定
	if (!AoAccountIsCurrentEnable()) {
		delete this;
		return;
	}

	// プラットフォームUI表示完了待ち
	if (!AoSysIsShowPlatformUI()) {

		// カレントユーザID取得
		u32 cur_id = (u32)AoAccountGetCurrentId();
		if (cur_id >= 4) {
			delete this;
			return;
		}

		// カレントユーザ有効判定
		if (!AoAccountIsCurrentEnableRealXbox360()) {
			delete this;
			return;
		}

		// ストア表示開始
		DWORD result = XShowMarketplaceUI(
			cur_id,
			XSHOWMARKETPLACEUI_ENTRYPOINT_CONTENTITEM,
			g_gs_trial_offer_id,
			(DWORD)-1);
		if (result != ERROR_SUCCESS) {
			delete this;
			return;
		}

		// ストア完了待ちへ遷移
		SetOwnTaskProc(&CTrialStore::TaskProcStore);
	}
}

// ===========================================================================
//! ストア表示中
// ===========================================================================
void CTrialStore::TaskProcStore()
{
	// プラットフォームUI表示完了待ち
	if (!AoSysIsShowPlatformUI()) {
		delete this;
	}
}

} // namespace gs

// ===========================================================================
//! システム通知コールバック
// ===========================================================================
void gsTrialSystemNoticeCallback(
	DWORD dwNotificationID, ULONG_PTR ulParam, void* arg)
{
	UNREFERENCED_PARAMETER(ulParam);
	UNREFERENCED_PARAMETER(arg);

	// サインイン状態が変更されたのであれば完全版判定を行う
	if (dwNotificationID == XN_SYS_SIGNINCHANGED) {
		gsTrialCheckStartSub();
	}
}

// ===========================================================================
//! 状態チェック開始(サブ)
// ===========================================================================
void gsTrialCheckStartSub(void)
{
	// 体験版の時のみ処理する
	if (GsTrialIsTrial() && g_gs_trial_is_trial_sub) {
		if (g_gs_trial_check == NULL) {
			g_gs_trial_is_check_request = FALSE;
			g_gs_trial_check = new gs::CTrialCheck;
		}
		else {
			g_gs_trial_is_check_request = TRUE;
		}
	}
}

#endif // defined(_DLC) && _XBOX

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
