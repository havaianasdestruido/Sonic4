// ===========================================================================
/*!
	@file	aoSystem.cpp
	@brief	AoLibrary システムモジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"

#if defined(AOD_PLATFORM_PS3)
#include <sysutil/sysutil_bgmplayback.h>
#endif // defined(AOD_PLATFORM_PS3)

// ----- Macros ------------------------------------------------（マクロ定義）

#if defined(AOD_PLATFORM_PS3)
#define AOD_SYS_PS3_SYSCB_INFO_NUM		(8)	//!< システムコールバック通知数
#endif // defined(AOD_PLATFORM_PS3)

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct AOS_SYS_GLOBAL
// ---------------------------------------------------------------------------
//!	システムグローバルパラメータ
// ===========================================================================
typedef struct tag_AOS_SYS_GLOBAL {
	BOOL	is_show_ui;			//!< プラットフォーム固有UI表示の有無
	BOOL	is_signin_changed;	//!< サインイン状態変更の有無

#if defined(AOD_PLATFORM_XBOX360)
	BOOL	is_play_sysbgm;		//!< システムBGM再生中
#endif // defined(AOD_PLATFORM_XBOX360)
} AOS_SYS_GLOBAL;

#if defined(AOD_PLATFORM_PS3)
// ===========================================================================
//	struct AOS_SYS_CB_INFO
// ---------------------------------------------------------------------------
//!	PS3システムコールバック情報
// ===========================================================================
typedef struct tag_AOS_SYS_CB_INFO {
	AOF_PS3_SYSCALLBACK_NOTICE	cb;		//!< コールバック関数
	void*						arg;	//!< コールバック引数
} AOS_SYS_CB_INFO;
#endif // defined(AOD_PLATFORM_PS3)

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

static void aoSysInit(void);
static void aoSysExit(void);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//	AOS_SYS_GLOBAL g_ao_sys_global
// ---------------------------------------------------------------------------
//!	システムグローバルパラメータ
// ===========================================================================
static AOS_SYS_GLOBAL g_ao_sys_global;

#if defined(AOD_PLATFORM_PS3)
// ===========================================================================
//	int g_ao_sys_ps3_syscb_slot
// ---------------------------------------------------------------------------
//!	共通イベントコールバックのスロット番号
// ===========================================================================
static int g_ao_sys_ps3_syscb_slot = -1;

// ===========================================================================
//	int g_ao_sys_ps3_syscb_slot
// ---------------------------------------------------------------------------
//!	PS3システムコールバック情報配列
// ===========================================================================
static AOS_SYS_CB_INFO g_ao_sys_ps3_syscb_info_tbl[AOD_SYS_PS3_SYSCB_INFO_NUM];

// ===========================================================================
//	int g_ao_sys_ps3_syscb_info_mutex
// ---------------------------------------------------------------------------
//!	PS3システムコールバック情報用ミューテックス
// ===========================================================================
static AMS_MUTEX g_ao_sys_ps3_syscb_info_mutex;
#endif // defined(AOD_PLATFORM_PS3)

#if defined(AOD_PLATFORM_XBOX360)
// ===========================================================================
//	AOF_XBOX360_SYSCALLBACK_NOTICE g_ao_sys_xbox360_syscb
// ---------------------------------------------------------------------------
//!	Xbox360システム通知コールバック関数
// ===========================================================================
static AOF_XBOX360_SYSCALLBACK_NOTICE g_ao_sys_xbox360_syscb = NULL;

// ===========================================================================
//	void* g_ao_sys_xbox360_syscb_arg
// ---------------------------------------------------------------------------
//!	Xbox360システム通知コールバック関数の引数
// ===========================================================================
static void* g_ao_sys_xbox360_syscb_arg = NULL;
#endif // defined(AOD_PLATFORM_XBOX360)

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

#if defined(AOD_PLATFORM_PS3)
// ===========================================================================
//	AoSysPS3SetSystemCallbackSlot
/*!
	共通イベントコールバックのスロット番号設定

	@param slot	[in] スロット番号(0～3)
	@note
	内部でcellSysutilRegisterCallback関数により設定する
	共通イベントコールバックのスロット番号を指定してください。\n
	この関数はAoSysInit関数を呼び出す前に呼び出し、
	AoSysExit関数呼び出しまでの間は呼び出さないでください。\n
*/
// ===========================================================================
void AoSysPS3SetSystemCallbackSlot(int slot)
{
	amAssert((u32)slot < 4);
	g_ao_sys_ps3_syscb_slot = slot;
}

// ===========================================================================
//	AoSysPS3AddSystemCallbackNotice
/*!
	共通イベントコールバックの通知関数登録追加

	@param cb	[in] 通知関数
	@param arg	[in] 通知関数引数
	@return ID
	@note
	システムコールバックが呼び出された場合に、
	呼び出される通知関数を登録します。\n
	不必要になった場合はAoSysPS3DelSystemCallbackNotice関数で
	通知関数の登録を削除してください。\n
*/
// ===========================================================================
u32 AoSysPS3AddSystemCallbackNotice(AOF_PS3_SYSCALLBACK_NOTICE cb, void* arg)
{
	amMutexLock(&g_ao_sys_ps3_syscb_info_mutex);
	for (u32 i = 0; i < AOD_SYS_PS3_SYSCB_INFO_NUM; ++i) {
		if (g_ao_sys_ps3_syscb_info_tbl[i].cb == NULL) {
			g_ao_sys_ps3_syscb_info_tbl[i].cb = cb;
			g_ao_sys_ps3_syscb_info_tbl[i].arg = arg;
			amMutexUnlock(&g_ao_sys_ps3_syscb_info_mutex);
			return i;
		}
	}
	amAssert(0);
	amMutexUnlock(&g_ao_sys_ps3_syscb_info_mutex);
	return AOD_SYS_PS3_SYSCB_INFO_NUM;
}

// ===========================================================================
//	AoSysPS3DelSystemCallbackNotice
/*!
	共通イベントコールバックの通知関数登録削除

	@param id	[in] ID
*/
// ===========================================================================
void AoSysPS3DelSystemCallbackNotice(u32 id)
{
	amMutexLock(&g_ao_sys_ps3_syscb_info_mutex);
	if (id < AOD_SYS_PS3_SYSCB_INFO_NUM) {
		g_ao_sys_ps3_syscb_info_tbl[id].cb = NULL;
		g_ao_sys_ps3_syscb_info_tbl[id].arg = NULL;
	}
	amMutexUnlock(&g_ao_sys_ps3_syscb_info_mutex);
}
#endif // defined(AOD_PLATFORM_PS3)

#if defined(AOD_PLATFORM_XBOX360)
// ===========================================================================
//	AoSysPS3AddSystemCallbackNotice
/*!
	XNotifyGetNext関数によるシステム通知発生時コールバック関数登録

	@param cb	[in] 通知関数
	@param arg	[in] 通知関数引数
*/
// ===========================================================================
void AoSysSetSystemNoticeCallbackXbox360(
	AOF_XBOX360_SYSCALLBACK_NOTICE cb, void* arg)
{
	g_ao_sys_xbox360_syscb = cb;
	g_ao_sys_xbox360_syscb_arg = arg;
}
#endif // defined(AOD_PLATFORM_XBOX360)

// ===========================================================================
//	AoSysInit
/*!
	システム初期化処理

	@note
	アプリケーション起動時に1度だけ呼び出して下さい。\n
	AoLibraryの各モジュールを使用するためには、
	この関数が呼び出されている必要があります。\n
*/
// ===========================================================================
void AoSysInit(void)
{
	// グローバルパラメータ初期化
	g_ao_sys_global.is_show_ui = FALSE;
	g_ao_sys_global.is_signin_changed = FALSE;
#if defined(AOD_PLATFORM_XBOX360)
	g_ao_sys_global.is_play_sysbgm = FALSE;
	g_ao_sys_xbox360_syscb = NULL;
	g_ao_sys_xbox360_syscb_arg = NULL;
#endif // defined(AOD_PLATFORM_XBOX360)

	// プラットフォーム固有の初期化処理
	aoSysInit();
}

// ===========================================================================
//	AoSysExit
/*!
	システム終了処理

	@note
	アプリケーション終了時に1度だけ呼び出して下さい。\n
	AoSysInit関数が呼び出されていない状態では、
	この関数を呼び出さないようにして下さい。\n
	この関数を呼出し後、再度AoSysInit関数を呼び出すことはできません。\n
*/
// ===========================================================================
void AoSysExit(void)
{
	// プラットフォーム固有の終了処理
	aoSysExit();
}

// ===========================================================================
//	AoSysIsShowPlatformUI
/*!
	プラットフォーム固有UI表示の有無を判定

	@return 真：表示中　偽：非表示
	@note
	画面全体を覆う形でのプラットフォーム固有UIが
	表示されているかどうかを判定します。\n
	この関数がTRUEを返す状態では、
	ユーザがゲーム画面を認識することができないため、
	ポーズをかける、操作を無効にするなどの対処を行って下さい。\n
*/
// ===========================================================================
BOOL AoSysIsShowPlatformUI(void)
{
	if (AoSysMsgIsShowReal() || g_ao_sys_global.is_show_ui) {
		return TRUE;
	}
#if defined(AOD_PLATFORM_WII)
	if (amWiiIsDrawHBM()) {
		return TRUE;
	}
#endif // defined(AOD_PLATFORM_WII)
	return FALSE;
}

// ===========================================================================
//	AoSysIsChangeSigninState
/*!
	ユーザのサインイン状態の変更判定

	@return 真：変更された　偽：変更無し
	@note
	直前にAoSysClearSigninState関数が呼ばれてから、
	ユーザのサインイン状態が変更されたかどうかを判定します。\n
	サインイン状態の詳細については、
	個別の状態取得関数を呼び出すようにして下さい。\n
	一度TRUEを返すようになると、
	AoSysClearSigninState関数を呼び出すまでTRUEを返し続けます。\n
*/
// ===========================================================================
BOOL AoSysIsChangeSigninState(void)
{
	return g_ao_sys_global.is_signin_changed;
}

// ===========================================================================
//	AoSysIsChangeSigninState
/*!
	ユーザのサインイン状態の変更クリア

	@note
	AoSysIsChangeSigninStateで判定するサインイン状態の変更状態をクリアし、
	変更なしの状態にします。\n
	以降、サインイン状態に変更があるまで
	AoSysIsChangeSigninState関数はFALSEを返し続けます。\n
*/
// ===========================================================================
void AoSysClearSigninState(void)
{
	g_ao_sys_global.is_signin_changed = FALSE;
}

// ===========================================================================
//	AoSysClearStorageDeviceState
/*!
	システムBGM再生中判定

	@return 真：再生中　偽：停止中
*/
// ===========================================================================
BOOL AoSysIsPlaySystemBgm(void)
{
#if defined(AOD_PLATFORM_XBOX360)

	return g_ao_sys_global.is_play_sysbgm;

#elif defined(AOD_PLATFORM_PS3)

	CellSysutilBgmPlaybackStatus s;
	if (cellSysutilGetBgmPlaybackStatus(&s) == CELL_SYSUTIL_BGMPLAYBACK_OK) {
		if (s.playerState != CELL_SYSUTIL_BGMPLAYBACK_STATUS_STOP) {
			return TRUE;
		}
	}
	return FALSE;

#else

	return FALSE;

#endif
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

#if defined(AOD_PLATFORM_WIN32)

// ***************************************************************************
// Win32
// ***************************************************************************
// ===========================================================================
//! 初期化処理(Win32)
// ===========================================================================
void aoSysInit(void)
{
	// empty
}

// ===========================================================================
//! 終了処理(Win32)
// ===========================================================================
void aoSysExit(void)
{
	// empty
}

#elif defined(AOD_PLATFORM_XBOX360)

#include <xmp.h>

// ***************************************************************************
// XBOX360
// ***************************************************************************
//! システム通知タスクのワーク(64byte以内)
typedef struct tag_AOS_NOTICE {
	HANDLE		handle;		//!< 通知ハンドル
} AOS_NOTICE; // 4 byte

//! システム通知タスク
static void aoSysTaskWaitNotice(AMS_TCB* tcb);
static void aoSysTaskWaitNoticeDestructor(AMS_TCB* tcb);

//! システム通知タスクTCBポインタ
static AMS_TCB* g_ao_sys_notice_tcb = NULL;

// ===========================================================================
//! 初期化処理(XBOX360)
// ===========================================================================
void aoSysInit(void)
{
	// Xbox LIVE 初期化
	if (XNetStartup(NULL) != 0) {
		amAssert(0);
	}
	if (XOnlineStartup() != ERROR_SUCCESS) {
		amAssert(0);
	}

	// システム通知待ちタスク作成
	g_ao_sys_notice_tcb = amTaskMake(
		aoSysTaskWaitNotice, aoSysTaskWaitNoticeDestructor,
		0, 0, 0, "aoSys::WaitNotice");

	// ワーク取得
	AOS_NOTICE* work = (AOS_NOTICE*)amTaskGetWork(g_ao_sys_notice_tcb);

	// システム通知開始
	work->handle = XNotifyCreateListener(XNOTIFY_ALL);
	amAssert(work->handle);

	// XMP再生制御権判定
	BOOL xmp;
	if (XMPTitleHasPlaybackControl(&xmp) == ERROR_SUCCESS) {
		if (xmp) {
			g_ao_sys_global.is_play_sysbgm = FALSE;
		}
		else {
			g_ao_sys_global.is_play_sysbgm = TRUE;
		}
	}
	else {
		g_ao_sys_global.is_play_sysbgm = FALSE;
	}

	// タスク開始
	amTaskStart(g_ao_sys_notice_tcb);
}

// ===========================================================================
//! 終了処理(XBOX360)
// ===========================================================================
void aoSysExit(void)
{
	// システム通知タスク終了
	if (g_ao_sys_notice_tcb) {
		amTaskDelete(g_ao_sys_notice_tcb);
		g_ao_sys_notice_tcb = NULL;
	}

	// Xbox LIVE 終了
	if (XOnlineCleanup() != ERROR_SUCCESS) {
		amAssert(0);
	}
	if (XNetCleanup() != 0) {
		amAssert(0);
	}
}

// ===========================================================================
//! システム通知待ちタスクプロシージャ
// ===========================================================================
void aoSysTaskWaitNotice(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_NOTICE* work = (AOS_NOTICE*)amTaskGetWork(tcb);

	DWORD dwNotificationID;
	ULONG_PTR ulParam;

	// システム通知の有無を判定
	while (XNotifyGetNext(work->handle, 0, &dwNotificationID, &ulParam)) {

		if (g_ao_sys_xbox360_syscb) {
			g_ao_sys_xbox360_syscb(
				dwNotificationID, ulParam, g_ao_sys_xbox360_syscb_arg);
		}

		// 通知の種類別に処理
		switch (dwNotificationID) {
		case XN_SYS_UI: // UI表示変更
			g_ao_sys_global.is_show_ui = (BOOL)ulParam;
			break;

		case XN_SYS_SIGNINCHANGED: // サインイン状態変更
			g_ao_sys_global.is_signin_changed = TRUE;
			break;

		case XN_XMP_PLAYBACKCONTROLLERCHANGED: // XMP再生制御権変更
			if (ulParam) {
				g_ao_sys_global.is_play_sysbgm = FALSE;
			}
			else {
				g_ao_sys_global.is_play_sysbgm = TRUE;
			}
			break;

		default:
			// empty
			break;
		}
	}
}

// ===========================================================================
//! システム通知待ちタスクデストラクタ
// ===========================================================================
void aoSysTaskWaitNoticeDestructor(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_NOTICE* work = (AOS_NOTICE*)amTaskGetWork(tcb);

	// システム通知終了
	if (work->handle) {
		XCloseHandle(work->handle);
		work->handle = NULL;
	}
}

#elif defined(AOD_PLATFORM_PS3)

// ***************************************************************************
// PlayStation3
// ***************************************************************************

//! 共通イベントコールバック
static void aoSysSysutilCallback(
	uint64_t status, uint64_t param, void* userdata);

//! システム監視タスク
static void aoSysTaskWatch(AMS_TCB* tcb);

//! システム監視タスクデストラクタ
static void aoSysTaskWatchDestructor(AMS_TCB* tcb);

//! システム監視タスクTCBポインタ
static AMS_TCB* g_ao_sys_watch_tcb = NULL;

// ===========================================================================
//! 初期化処理(PlayStation3)
// ===========================================================================
void aoSysInit(void)
{
	amAssert((u32)g_ao_sys_ps3_syscb_slot < 4);

	// システム監視タスク作成
	g_ao_sys_watch_tcb = amTaskMake(
		aoSysTaskWatch, aoSysTaskWatchDestructor, 0, 0, 0, "aoSys::Watch");

	// システム監視タスク開始
	amTaskStart(g_ao_sys_watch_tcb);

	// コールバック情報初期化
	amZeroMemory(
		g_ao_sys_ps3_syscb_info_tbl,
		sizeof(AOS_SYS_CB_INFO) * AOD_SYS_PS3_SYSCB_INFO_NUM);

	// コールバック情報用ミューテックス作成
	amMutexCreate(&g_ao_sys_ps3_syscb_info_mutex);

	// 共通イベントコールバック設定
	cellSysutilRegisterCallback(
		g_ao_sys_ps3_syscb_slot, aoSysSysutilCallback, &g_ao_sys_global);
}

// ===========================================================================
//! 終了処理(PlayStation3)
// ===========================================================================
void aoSysExit(void)
{
	// 共通イベントコールバック解除
	cellSysutilUnregisterCallback(g_ao_sys_ps3_syscb_slot);

	// システム監視タスク終了
	if (g_ao_sys_watch_tcb) {
		amTaskDelete(g_ao_sys_watch_tcb);
		g_ao_sys_watch_tcb = NULL;
	}

	// コールバック情報用ミューテックス削除
	amMutexDelete(&g_ao_sys_ps3_syscb_info_mutex);
}

// ===========================================================================
//! 共通イベントコールバック
// ===========================================================================
void aoSysSysutilCallback(uint64_t status, uint64_t param, void* userdata)
{
	AOS_SYS_GLOBAL* global = (AOS_SYS_GLOBAL*)userdata;

	switch (status) {
	case CELL_SYSUTIL_SYSTEM_MENU_OPEN:
		// XMB表示開始
		global->is_show_ui = TRUE;
		break;

	case CELL_SYSUTIL_SYSTEM_MENU_CLOSE:
		// XMB表示終了
		global->is_show_ui = FALSE;
		break;

	default:
		// empty
		break;
	}

	// 通知
	amMutexLock(&g_ao_sys_ps3_syscb_info_mutex);
	for (u32 i = 0; i < AOD_SYS_PS3_SYSCB_INFO_NUM; ++i) {
		if (g_ao_sys_ps3_syscb_info_tbl[i].cb) {
			amMutexUnlock(&g_ao_sys_ps3_syscb_info_mutex);
			g_ao_sys_ps3_syscb_info_tbl[i].cb(
				status, param, g_ao_sys_ps3_syscb_info_tbl[i].arg);
			amMutexLock(&g_ao_sys_ps3_syscb_info_mutex);
		}
	}
	amMutexUnlock(&g_ao_sys_ps3_syscb_info_mutex);
}

// ===========================================================================
//! システム監視タスク
// ===========================================================================
void aoSysTaskWatch(AMS_TCB* tcb)
{
	// システムUI表示の有無によってパッド入力の有無を設定
	if (AoSysIsShowPlatformUI()) {
		amPadEnableInput(-1, 0);
	}
	else {
		amPadEnableInput(-1, 1);
	}
}

// ===========================================================================
//! システム監視タスクデストラクタ
// ===========================================================================
void aoSysTaskWatchDestructor(AMS_TCB* tcb)
{
	g_ao_sys_watch_tcb = NULL;
}

#elif defined(AOD_PLATFORM_WII)

// ***************************************************************************
// Wii
// ***************************************************************************
// ===========================================================================
//! 初期化処理(Wii)
// ===========================================================================
void aoSysInit(void)
{
	// empty
}

// ===========================================================================
//! 終了処理(Wii)
// ===========================================================================
void aoSysExit(void)
{
	// empty
}

#elif defined(AOD_PLATFORM_IPHONE)

// ***************************************************************************
// iPhone
// ***************************************************************************
// ===========================================================================
//! 初期化処理(iPhone)
// ===========================================================================
void aoSysInit(void)
{
	// empty
}

// ===========================================================================
//! 終了処理(iPhone)
// ===========================================================================
void aoSysExit(void)
{
	// empty
}

#endif

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
