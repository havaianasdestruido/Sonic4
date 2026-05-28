// ===========================================================================
/*!
	@file	gsTrialPS3.cpp
	@brief	体験版管理モジュール定義(PlayStation3)

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "gsTrial.h"
#include "gsEnvironment.h"
#include "ao.h"

#if defined(_DLC) && _PS3

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

namespace gs {

// ===========================================================================
//! 体験版モジュール初期化クラス
// ===========================================================================
class CTrialInit :
	public ao::CTask<CTrialInit>, public ao::CThread<CTrialInit>,
	public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CTrialInit();

	//! デストラクタ
	virtual ~CTrialInit();

protected:

	//! 初期化待ちタスクプロシージャ
	void TaskProcWait();

	//! 初期化スレッドプロシージャ
	void ThreadProcInit();

	//! 体験版フラグ
	BOOL m_is_trial;
};

// ===========================================================================
//! 体験版モジュールストアクラス
// ===========================================================================
class CTrialStore :
	public ao::CTask<CTrialStore>, public ao::CThread<CTrialStore>,
	public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CTrialStore();

	//! デストラクタ
	virtual ~CTrialStore();

	//! ネットワーク接続開始ダイアログ終了通知
	void NoticeNetStartDialogEnd();

protected:

	//! 終了待ちタスクプロシージャ
	void TaskProcWait();

	//! メッセージ表示待ちタスクプロシージャ
	void TaskProcWaitMessage();

	//! ストアスレッドプロシージャ
	void ThreadProcStore();

	//! パレンタルロックフラグ
	BOOL m_is_parentallock;

	//! ネットワーク接続開始ダイアログ終了フラグ
	BOOL m_is_net_start_dlg_end;

	//! ネットワーク接続開始ダイアログ終了フラグ用ミューテックス
	AMS_MUTEX m_mutex_is_net_start_dlg_end;
};

} // namespace gs

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

void gsTrialSysCallbackNotice(uint64_t status, uint64_t param, void* arg);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//! 体験版モジュール初期化クラス
// ===========================================================================
gs::CTrialInit* g_gs_trial_init = NULL;

// ===========================================================================
//! 体験版モジュールストアクラス
// ===========================================================================
gs::CTrialStore* g_gs_trial_store = NULL;

// ===========================================================================
//! 体験版フラグ
// ===========================================================================
static BOOL g_gs_trial_is_trial = TRUE;

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
#if defined(HOG_RGN_EU)
	// EU版は常に製品版
	g_gs_trial_is_trial = FALSE;
#else
	// 体験版として設定
	g_gs_trial_is_trial = TRUE;
#endif // defined(HOG_RGN_EU)

	// 初期化開始
	g_gs_trial_init = new gs::CTrialInit;
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
	if (g_gs_trial_init) {
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
	if (g_gs_trial_init) {
		delete g_gs_trial_init;
		g_gs_trial_init = NULL;
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
#if defined(HOG_RGN_EU)
	// EU版は常に製品版
	return FALSE;
#else
	return g_gs_trial_is_trial;
#endif // defined(HOG_RGN_EU)
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
	// empty
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

namespace ao {

#if 1
// ===========================================================================
//! いろいろと問題があるのでスレッド関数を特殊化
// ===========================================================================
template <>
void CThread<gs::CTrialInit>::threadFunc(uint64_t arg)
{
	while (g_gs_trial_init == NULL) {
		// 10ミリ秒程度スリープ
		sys_timer_usleep(10 * 1000);
	}

	g_gs_trial_init->CallThreadProcedure(0);
}

// ===========================================================================
//! いろいろと問題があるのでスレッド関数を特殊化
// ===========================================================================
template <>
void CThread<gs::CTrialStore>::threadFunc(uint64_t arg)
{
	while (g_gs_trial_store == NULL) {
		// 10ミリ秒程度スリープ
		sys_timer_usleep(10 * 1000);
	}

	g_gs_trial_store->CallThreadProcedure(0);
}
#endif

} // namespace ao

namespace gs {

// ***************************************************************************
// 体験版モジュール初期化クラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CTrialInit::CTrialInit()
{
	// 体験版としてチェック開始
	m_is_trial = TRUE;

	// タスク作成
	MakeTask(0, "gsTrial::Init");

	// タスクプロシージャ設定
	SetTaskProc(0, &CTrialInit::TaskProcWait);

	// スレッドプロシージャ設定
	SetThreadProc(0, &CTrialInit::ThreadProcInit);

	// スレッド開始
	s32 prio;
	sys_ppu_thread_t id;
	sys_ppu_thread_get_id(&id);
	sys_ppu_thread_get_priority(id, &prio);
	prio += 1;
	StartThread(0, (AMD_CORE)0, (u32)prio);

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CTrialInit::~CTrialInit()
{
	amAssert(g_gs_trial_init);
	g_gs_trial_init = NULL;
}

// ===========================================================================
//! 初期化待ちタスクプロシージャ
// ===========================================================================
void CTrialInit::TaskProcWait()
{
	// スレッド終了判定
	if (IsEndThread(0)) {

		// 結果設定(念のため体験版のときのみ設定するようにする)
		if (g_gs_trial_is_trial) {
			g_gs_trial_is_trial = m_is_trial;
		}

		// 終了
		delete this;
	}
}

// ===========================================================================
//! 初期化スレッドプロシージャ
// ===========================================================================
void CTrialInit::ThreadProcInit()
{
#if defined(HOG_RGN_EU)

	// EU版は常に製品版として扱う
	m_is_trial = FALSE;

#else

	int result;
	BOOL is_load_module = FALSE;
	BOOL is_init_np = FALSE;
	void* np_pool = NULL;
	const u32 np_pool_size = 128 * 1024;

	// 初期状態は体験版とする
	m_is_trial = TRUE;

	// モジュールロード
	if (cellSysmoduleIsLoaded(CELL_SYSMODULE_SYSUTIL_NP) != 0) {
		result = cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP);
		if (result < 0) {
			goto end;
		}
		is_load_module = TRUE;
	}

	// NPプール確保
	np_pool = amMemAlloc(np_pool_size);

	// NP初期化
	result = sceNpInit(np_pool_size, np_pool);
	if (result < 0) {
		goto end;
	}
	is_init_np = TRUE;

	// 体験版検証
	result = sceNpDrmVerifyUpgradeLicense2(GsEnvGetPs3LicenseNpContentId());
	if (result < 0) {
		// 体験版
		m_is_trial = TRUE;
	}
	else {
		// 製品版
		m_is_trial = FALSE;
	}

end:

	// NP終了
	if (is_init_np) {
		sceNpTerm();
		is_init_np = FALSE;
	}

	// NPプール解放
	if (np_pool) {
		amMemFree(np_pool);
		np_pool = NULL;
	}

	// モジュールアンロード
	if (is_load_module) {
		cellSysmoduleUnloadModule(CELL_SYSMODULE_SYSUTIL_NP);
		is_load_module = FALSE;
	}

#endif // defined(HOG_RGN_EU)
}


// ***************************************************************************
// 体験版モジュールストアクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CTrialStore::CTrialStore()
{
	m_is_parentallock = FALSE;
	m_is_net_start_dlg_end = FALSE;
	amMutexCreate(&m_mutex_is_net_start_dlg_end);

	// タスク作成
	MakeTask(0, "gsTrial::Store");

	// タスクプロシージャ設定
	SetTaskProc(0, &CTrialStore::TaskProcWait);

	// スレッドプロシージャ設定
	SetThreadProc(0, &CTrialStore::ThreadProcStore);

	// スレッド開始
	s32 prio;
	sys_ppu_thread_t id;
	sys_ppu_thread_get_id(&id);
	sys_ppu_thread_get_priority(id, &prio);
	prio += 1;
	StartThread(0, (AMD_CORE)0, (u32)prio);

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
	amMutexDelete(&m_mutex_is_net_start_dlg_end);
}

// ===========================================================================
//! ネットワーク接続開始ダイアログ終了通知
// ===========================================================================
void CTrialStore::NoticeNetStartDialogEnd()
{
	amMutexLock(&m_mutex_is_net_start_dlg_end);
	m_is_net_start_dlg_end = TRUE;
	amMutexUnlock(&m_mutex_is_net_start_dlg_end);
}

// ===========================================================================
//! 終了待ちタスクプロシージャ
// ===========================================================================
void CTrialStore::TaskProcWait()
{
	// スレッド終了判定
	if (AoSysMsgIsFinished() && IsEndThread(0)) {

		// パレンタルロック判定
		if (m_is_parentallock) {
			// メッセージ表示
			AoSysMsgStart(
				AOD_SYS_MSG_NET_ERROR_PARENTALLOCK, AOD_SYS_MSG_SELECT_OK);
			SetOwnTaskProc(&CTrialStore::TaskProcWaitMessage);
			return;
		}

		// 終了
		delete this;
	}
}

// ===========================================================================
//! メッセージ表示待ちタスクプロシージャ
// ===========================================================================
void CTrialStore::TaskProcWaitMessage()
{
	// メッセージ表示終了待ち
	if (AoSysMsgIsFinished()) {
		// 終了
		delete this;
	}
}

// ===========================================================================
//! ストアスレッドプロシージャ
// ===========================================================================
void CTrialStore::ThreadProcStore()
{
#if !defined(HOG_RGN_EU)

	int result;
	BOOL is_load_module_libnetctl = FALSE;
	BOOL is_init_module_libnetctl = FALSE;
	BOOL is_load_module = FALSE;
	BOOL is_init_np = FALSE;
	void* np_pool = NULL;
	const u32 np_pool_size = 128 * 1024;
	CellNetCtlNetStartDialogParam net_dlg_param;
	CellNetCtlNetStartDialogResult net_dlg_result;
	u32 sys_cb_id = (u32)-1;
	int isRestricted, age;

	m_is_parentallock = FALSE;

	// libnetctlモジュールロード
	if (cellSysmoduleIsLoaded(CELL_SYSMODULE_NET) != 0) {
		result = cellSysmoduleLoadModule(CELL_SYSMODULE_NET);
		if (result < 0) {
			// エラー
			goto end;
		}
		is_load_module_libnetctl = TRUE;
	}

	// libnetctl初期化
	result = cellNetCtlInit();
	if ((u32)result != CELL_NET_CTL_ERROR_NOT_TERMINATED) {
		if (result != 0) {
			// エラー
			goto end;
		}
		is_init_module_libnetctl = TRUE;
	}

	// ネットワーク開始ダイアログ用システムコールバック登録
	m_is_net_start_dlg_end = FALSE;
	sys_cb_id = AoSysPS3AddSystemCallbackNotice(
		gsTrialSysCallbackNotice, this);

	// ネットワーク開始ダイアログ表示
	amZeroMemory(&net_dlg_param, sizeof(CellNetCtlNetStartDialogParam));
	net_dlg_param.size = sizeof(CellNetCtlNetStartDialogParam);
	net_dlg_param.type = CELL_NET_CTL_NETSTART_TYPE_NP;
	result = cellNetCtlNetStartDialogLoadAsync(&net_dlg_param);
	if (result != 0) {
		// エラー
		goto end;
	}

	// ネットワーク開始ダイアログ終了待ち
	while (1) {
		amMutexLock(&m_mutex_is_net_start_dlg_end);
		BOOL flag = m_is_net_start_dlg_end;
		amMutexUnlock(&m_mutex_is_net_start_dlg_end);
		if (flag) {
			break;
		}
		if (IsRequestEndThread()) {
			// ネットワーク開始ダイアログ強制終了
			cellNetCtlNetStartDialogAbortAsync();
			break;
		}

		// 100ミリ秒程度スリープ
		sys_timer_usleep(100 * 1000);
	}

	// ネットワーク開始ダイアログ終了
	amZeroMemory(&net_dlg_result, sizeof(CellNetCtlNetStartDialogResult));
	net_dlg_result.size = sizeof(CellNetCtlNetStartDialogResult);
	result = cellNetCtlNetStartDialogUnloadAsync(&net_dlg_result);
	if (result != 0) {
		// エラー
		goto end;
	}
	if (net_dlg_result.result != 0) {
		// 未サインイン
		goto end;
	}

	// ネットワーク開始ダイアログ用システムコールバック登録解除
	AoSysPS3DelSystemCallbackNotice(sys_cb_id);
	sys_cb_id = (u32)-1;

	// モジュールロード
	if (cellSysmoduleIsLoaded(CELL_SYSMODULE_SYSUTIL_NP) != 0) {
		result = cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP);
		if (result < 0) {
			goto end;
		}
		is_load_module = TRUE;
	}

	// NPプール確保
	np_pool = amMemAlloc(np_pool_size);

	// NP初期化
	result = sceNpInit(np_pool_size, np_pool);
	if (result < 0) {
		goto end;
	}
	is_init_np = TRUE;

	// パレンタルロック
	result = sceNpManagerGetContentRatingFlag(&isRestricted, &age);
	if (result < 0) {
		// エラー
		goto end;
	}
	if (isRestricted && (age < GsEnvGetPs3ParentalLockAge())) {
		// 年齢制限
		m_is_parentallock = TRUE;
		goto end;
	}

	// ストア実行
	sceNpDrmExecuteGamePurchase();

end:

	// NP終了
	if (is_init_np) {
		sceNpTerm();
		is_init_np = FALSE;
	}

	// NPプール解放
	if (np_pool) {
		amMemFree(np_pool);
		np_pool = NULL;
	}

	// モジュールアンロード
	if (is_load_module) {
		cellSysmoduleUnloadModule(CELL_SYSMODULE_SYSUTIL_NP);
		is_load_module = FALSE;
	}

	// ネットワーク開始ダイアログ用システムコールバック登録解除
	if (sys_cb_id != (u32)-1) {
		AoSysPS3DelSystemCallbackNotice(sys_cb_id);
		sys_cb_id = (u32)-1;
	}

	// libnetctl終了処理
	if (is_init_module_libnetctl || is_load_module_libnetctl) {
		cellNetCtlTerm();
		is_init_module_libnetctl = FALSE;
	}

	// libnetctlモジュールアンロード
	if (is_load_module_libnetctl) {
		cellSysmoduleUnloadModule(CELL_SYSMODULE_NET);
		is_load_module_libnetctl = FALSE;
	}

#endif // !defined(HOG_RGN_EU)
}

} // namespace gs

// ===========================================================================
//! システムコールバック
// ===========================================================================
void gsTrialSysCallbackNotice(uint64_t status, uint64_t param, void* arg)
{
	if (status == CELL_SYSUTIL_NET_CTL_NETSTART_FINISHED) {
		((gs::CTrialStore*)arg)->NoticeNetStartDialogEnd();
	}
}

#endif // defined(_DLC) && _PS3

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
