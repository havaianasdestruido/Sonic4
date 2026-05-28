// ===========================================================================
/*!
	@file	aoAccountXBOX360.cpp
	@brief	AoLibrary アカウント管理モジュール定義(XBOX360用)

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"

#if defined(AOD_PLATFORM_XBOX360)

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct AOS_ACCOUNT
// ---------------------------------------------------------------------------
//!	アカウント管理タスクのワーク(64byte以内)
// ===========================================================================
typedef struct tag_AOS_ACCOUNT {
	XOVERLAPPED			ov;				//!< 非同期制御用
	MESSAGEBOX_RESULT	msg_result;		//!< メッセージ結果

	BOOL		enable;			//!< アカウント有効フラグ
	BOOL		enable_temp;	//!< アカウント有効フラグ(一時データ)

	BOOL		is_signin;		//!< サインインフラグ
	s32			current_id;		//!< カレントアカウントID
	s32			ready_id;		//!< 設定中カレントアカウントID
	u32			count;			//!< 汎用カウンタ
} AOS_ACCOUNT; // 48 byte

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// タスク
static void aoAccountTaskIdle(AMS_TCB* tcb);
static void aoAccountTaskCurrentSetting00(AMS_TCB* tcb);
static void aoAccountTaskCurrentSetting01(AMS_TCB* tcb);
static void aoAccountTaskCurrentSetting02(AMS_TCB* tcb);
static void aoAccountTaskCurrentDisable00(AMS_TCB* tcb);
static void aoAccountTaskCurrentDisable01(AMS_TCB* tcb);
static void aoAccountTaskDestructor(AMS_TCB* tcb);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//	AMS_TCB* g_ao_account_tcb
// ---------------------------------------------------------------------------
//!	アカウント管理タスクのTCBポインタ
// ===========================================================================
static AMS_TCB* g_ao_account_tcb = NULL;

// ===========================================================================
//	BOOL g_ao_account_enable_flag
// ---------------------------------------------------------------------------
//!	アカウント有効フラグ
// ===========================================================================
static BOOL g_ao_account_enable_flag = FALSE;

// ===========================================================================
//	AMS_MUTEX g_ao_account_enable_flag_mutex
// ---------------------------------------------------------------------------
//!	アカウント有効フラグアクセス用ミューテックス
// ===========================================================================
static AMS_MUTEX g_ao_account_enable_flag_mutex;

// ===========================================================================
//! 全アカウント対象サインイン状態変更フラグ
// ===========================================================================
static BOOL g_ao_account_is_any_signin_state_changed = TRUE;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//	AoAccountInit
/*!
	アカウント管理モジュール初期化処理

	@note
	アプリケーション起動時に1度だけ呼び出して下さい。\n
*/
// ===========================================================================
void AoAccountInit(void)
{
	g_ao_account_enable_flag = FALSE;
	amMutexCreate(&g_ao_account_enable_flag_mutex);
	g_ao_account_is_any_signin_state_changed = TRUE;

	// タスク生成
	g_ao_account_tcb = amTaskMake(
		aoAccountTaskIdle, aoAccountTaskDestructor, 0,
		0, 0, "aoAccount");

	// ワーク初期化
	AOS_ACCOUNT* work = (AOS_ACCOUNT*)amTaskGetWork(g_ao_account_tcb);
	work->enable = FALSE;
	work->enable_temp = FALSE;
	work->is_signin = FALSE;
	work->current_id = -1;
	work->ready_id = -1;

#if defined(AOD_DEBUG)
	AoAccountDebugInit();
#endif // defined(AOD_DEBUG)

	// タスク開始
	amTaskStart(g_ao_account_tcb);
}

// ===========================================================================
//	AoAccountDebugInit
/*!
	デバッグ用初期化処理

	@note
	デバッグ用にカレントアカウントID自動設定します。\n
	設定されるIDは、接続されている最も若い番号のパッドのIDとなります。\n
	パッドが接続されていない場合は0番が設定されます。\n
	デバッグ環境外の場合は何も行いません。\n
	デバッグ環境では、AoAccountInit関数内で呼び出されています。\n
*/
// ===========================================================================
void AoAccountDebugInit(void)
{
#if defined(AOD_DEBUG)
	amPadGetData();
	AOS_ACCOUNT* work = (AOS_ACCOUNT*)amTaskGetWork(g_ao_account_tcb);
	work->enable = TRUE;
	work->enable_temp = TRUE;
	work->is_signin = FALSE;
	work->current_id = 0;
	work->ready_id = -1;
	g_ao_account_enable_flag = TRUE;
	for (u32 i = 0; i < AOD_PAD_PORT_MAX; ++i) {
		if (AoPadIsConnected(i)) {
			work->current_id = (s32)i;
			XUSER_SIGNIN_STATE state =
				XUserGetSigninState((DWORD)work->current_id);
			if (state != eXUserSigninState_NotSignedIn) {
				work->is_signin = TRUE;
			}
			break;
		}
	}
#endif // defined(AOD_DEBUG)
}

// ===========================================================================
//	AoAccountExit
/*!
	アカウント管理モジュール終了処理

	@note
	アプリケーション終了時に1度だけ呼び出して下さい。\n
	AoAccountInit関数が呼び出されていない状態では、
	この関数を呼び出さないようにして下さい。\n
	この関数を呼出し後、再度AoAccountInit関数を呼び出すことはできません。\n
*/
// ===========================================================================
void AoAccountExit(void)
{
	// タスク削除
	if (g_ao_account_tcb) {
		amTaskDelete(g_ao_account_tcb);
		g_ao_account_tcb = NULL;
	}

	// ミューテックス削除
	amMutexDelete(&g_ao_account_enable_flag_mutex);
}

// ===========================================================================
//	AoAccountClearCurrentId
/*!
	カレントアカウントIDをクリア(初期状態)

	@note
	カレントのアカウントを未定の状態にし、
	AoAccountInit関数呼び出し直後の状態にします。\n
*/
// ===========================================================================
void AoAccountClearCurrentId(void)
{
	amAssert(g_ao_account_tcb);

	// ワーク取得
	AOS_ACCOUNT* work = (AOS_ACCOUNT*)amTaskGetWork(g_ao_account_tcb);

	// カレントIDクリア
	g_ao_account_is_any_signin_state_changed = TRUE;
	work->enable = FALSE;
	work->enable_temp = FALSE;
	work->is_signin = FALSE;
	work->current_id = -1;
	work->ready_id = -1;

	amMutexLock(&g_ao_account_enable_flag_mutex);
	g_ao_account_enable_flag = FALSE;
	amMutexUnlock(&g_ao_account_enable_flag_mutex);
}

// ===========================================================================
//	AoAccountSetCurrentIdStart
/*!
	カレントアカウントIDの設定開始

	@param id		[in] 設定するカレントアカウントID
	@note
	カレントのアカウントID(有効なパッドID)の設定を行います。\n
	この関数が呼び出されると、
	必要に応じてアカウントの選択処理やサインイン処理が行われるため、
	カレントアカウントの設定が完了するまでに時間が掛かる場合があります。\n
	カレントアカウントの設定が完了したかどうかは、
	AoAccountSetCurrentIdIsFinished関数で判定して下さい。\n
	また、設定がキャンセルされる場合もありますので、設定完了後
	AoAccountGetCurrentIdで有効なアカウントIDが取得できるかを
	判定するようにして下さい。\n
	カレントアカウントが設定されていない状態で呼び出して下さい。\n
*/
// ===========================================================================
void AoAccountSetCurrentIdStart(u32 id)
{
	amAssert(id < AOD_ACCOUNT_MAX);
	amAssert(g_ao_account_tcb);

	// ワーク取得
	AOS_ACCOUNT* work = (AOS_ACCOUNT*)amTaskGetWork(g_ao_account_tcb);
	work->enable = FALSE;
	work->enable_temp = FALSE;
	work->is_signin = FALSE;
	work->current_id = -1;
	work->ready_id = (s32)id;

	amMutexLock(&g_ao_account_enable_flag_mutex);
	g_ao_account_enable_flag = FALSE;
	amMutexUnlock(&g_ao_account_enable_flag_mutex);

	// カレントID設定タスクへ移行
	amTaskSetProcedure(g_ao_account_tcb, aoAccountTaskCurrentSetting00);
}

// ===========================================================================
//	AoAccountSetCurrentIdIsFinished
/*!
	カレントアカウントIDの設定完了判定

	@return 真：設定完了　偽：設定中
	@note
	AoAccountSetCurrentIdStart関数で開始した、
	カレントアカウントの設定処理が完了したかどうかを判定します。\n
*/
// ===========================================================================
BOOL AoAccountSetCurrentIdIsFinished(void)
{
	amAssert(g_ao_account_tcb);

	// ワーク取得
	AOS_ACCOUNT* work = (AOS_ACCOUNT*)amTaskGetWork(g_ao_account_tcb);

	// 設定中のIDが無効になれば完了
	if (work->ready_id < 0) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//	AoAccountGetCurrentId
/*!
	カレントアカウントIDの取得

	@return カレントアカウントID(負数：カレントなし)
	@note
	現在のカレントアカウントIDを返します。\n
	カレントアカウントIDが無効な場合は負数を返します。\n
	AoAccountSetCurrentIdStart関数を呼び出し、
	AoAccountSetCurrentIdIsFinished関数がTRUEを返した後に、
	この関数が負数を返すか正数を返すかで、
	カレントアカウントが設定されたか、キャンセルされたかを判定して下さい。\n
	一度、正常なアカウントIDを返すようになると、
	AoAccountClearCurrentId関数を呼び出すまで、
	返す値が変わることはありません。\n
*/
// ===========================================================================
s32 AoAccountGetCurrentId(void)
{
	amAssert(g_ao_account_tcb);

	// ワーク取得
	AOS_ACCOUNT* work = (AOS_ACCOUNT*)amTaskGetWork(g_ao_account_tcb);

	return work->current_id;
}

// ===========================================================================
//	AoAccountIsCurrentSignin
/*!
	カレントアカウントIDのサインイン済み判定

	@return 真：サインイン済み　偽：サインインしていない
	@note
	カレントアカウントIDが設定されている場合に、
	カレントアカウントがサインインしてるかどうかを判定します。\n
*/
// ===========================================================================
BOOL AoAccountIsCurrentSignin(void)
{
	amAssert(g_ao_account_tcb);

	// ワーク取得
	AOS_ACCOUNT* work = (AOS_ACCOUNT*)amTaskGetWork(g_ao_account_tcb);

	return work->is_signin;
}

// ===========================================================================
//	AoAccountIsCurrentOnline
/*!
	カレントアカウントIDのオンライン対応判定

	@return 真：オンライン対応　偽：非対応
	@note
	カレントアカウントIDが設定されている場合に、
	カレントアカウントがオンライン対応してるかどうかを判定します。\n
*/
// ===========================================================================
BOOL AoAccountIsCurrentOnline(void)
{
	s32 id = AoAccountGetCurrentId();
	if ((u32)id >= AOD_ACCOUNT_MAX) {
		return FALSE;
	}
	if (XUserGetSigninState((DWORD)id) == eXUserSigninState_SignedInToLive) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//	AoAccountIsCurrentEnable
/*!
	カレントアカウント有効判定

	@return 真：有効　偽：無効
	@note
	現在、カレントアカウントが有効であるかどうかを判定します。\n
	AoAccountSetCurrentIdStart関数でカレントアカウントを設定した後、
	この関数がFALSEを返す場合は、
	ユーザ操作によりアカウントが無効にされたことになるので、
	以降、アカウントを使用した各種処理は行えなくなります。\n
	タイトル画面へ戻るなどの対処を行うようにして下さい。\n
*/
// ===========================================================================
BOOL AoAccountIsCurrentEnable(void)
{
	amAssert(g_ao_account_tcb);

	// ワーク取得
	AOS_ACCOUNT* work = (AOS_ACCOUNT*)amTaskGetWork(g_ao_account_tcb);

	return work->enable;
}

// ===========================================================================
//	AoAccountIsCurrentEnableRealXbox360
/*!
	呼び出し時点でカレントアカウントが有効か判定(Xbox360のみ)

	@return 真：有効　偽：無効
	@note
	AoAccountIsCurrentEnable関数の呼び出しには遅延があるため、
	（SDKの関数呼び足をする際などに、）
	現時点でのアカウントが有効かどうかを判定するために
	この関数を呼び出して下さい。\n
	通常は、AoAccountIsCurrentEnable関数を使用すれば問題ありません。\n
*/
// ===========================================================================
BOOL AoAccountIsCurrentEnableRealXbox360(void)
{
	amAssert(g_ao_account_tcb);

	BOOL flag;

	amMutexLock(&g_ao_account_enable_flag_mutex);
	flag = g_ao_account_enable_flag;
	amMutexUnlock(&g_ao_account_enable_flag_mutex);

	if (!flag) {
		return FALSE;
	}

	u32 id = (u32)AoAccountGetCurrentId();
	if (id >= 4) {
		return FALSE;
	}
	if (XUserGetSigninState(id) == eXUserSigninState_NotSignedIn) {
		return FALSE;
	}

	return TRUE;
}

// ===========================================================================
//! 全アカウント対象サインイン状態変更判定(現状ではプレゼンス専用)
// ===========================================================================
BOOL AoAccountIsAnySigninStateChanged(void)
{
	return g_ao_account_is_any_signin_state_changed;
}

// ===========================================================================
//! 全アカウント対象サインイン状態変更クリア(現状ではプレゼンス専用)
// ===========================================================================
void AoAccountClearAnySigninStateChanged(void)
{
	g_ao_account_is_any_signin_state_changed = FALSE;
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ***************************************************************************
// タスク
// ***************************************************************************
// ===========================================================================
//! アイドル
// ===========================================================================
void aoAccountTaskIdle(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_ACCOUNT* work = (AOS_ACCOUNT*)amTaskGetWork(tcb);

	// アカウント状態変更判定
	BOOL is_state_changed = AoSysIsChangeSigninState();
	AoSysClearSigninState();

	// 変更記録
	if (is_state_changed) {
		g_ao_account_is_any_signin_state_changed = TRUE;
	}

	// アカウント無効判定
	if (work->enable) {

		// アカウント状態変更判定
		if (is_state_changed) {

			// カレントアカウントIDのサインイン判定
			XUSER_SIGNIN_STATE state =
				XUserGetSigninState((DWORD)work->current_id);
			if (work->is_signin) {
				if (state == eXUserSigninState_NotSignedIn) {
					work->enable_temp = FALSE;
				}
			}
			else {
				if (state != eXUserSigninState_NotSignedIn) {
					work->enable_temp = FALSE;
				}
			}
		}

		// アカウント無効通知メッセージ表示
		if (work->enable_temp == FALSE) {

			amMutexLock(&g_ao_account_enable_flag_mutex);
			g_ao_account_enable_flag = FALSE;
			amMutexUnlock(&g_ao_account_enable_flag_mutex);

			amTaskSetProcedure(tcb, aoAccountTaskCurrentDisable00);
		}
	}
}

// ===========================================================================
//! カレントアカウント設定00
// ===========================================================================
void aoAccountTaskCurrentSetting00(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_ACCOUNT* work = (AOS_ACCOUNT*)amTaskGetWork(tcb);

	// サインイン状態の取得
	XUSER_SIGNIN_STATE state = XUserGetSigninState((DWORD)work->ready_id);

	if (state == eXUserSigninState_NotSignedIn) {
		if (!AoSysIsShowPlatformUI()) {
			// サインインしていないならサインインを促す
			XShowSigninUI(1, 0);

			// サインイン待ちへ移行
			work->count = 0;
			amTaskSetProcedure(tcb, aoAccountTaskCurrentSetting01);
		}
	}
	else {
		// サインイン済みなのでそのまま継続
		AoSysClearSigninState();
		g_ao_account_is_any_signin_state_changed = TRUE;
		work->enable = TRUE;
		work->enable_temp = TRUE;
		work->is_signin = TRUE;
		work->current_id = work->ready_id;
		work->ready_id = -1;
		amTaskSetProcedure(tcb, aoAccountTaskIdle);

		amMutexLock(&g_ao_account_enable_flag_mutex);
		g_ao_account_enable_flag = TRUE;
		amMutexUnlock(&g_ao_account_enable_flag_mutex);
	}
}

// ===========================================================================
//! カレントアカウント設定01
// ===========================================================================
void aoAccountTaskCurrentSetting01(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_ACCOUNT* work = (AOS_ACCOUNT*)amTaskGetWork(tcb);

	// すぐにUIが表示されないこともあるため一定時間待機してから判定する
	if (work->count < 30) {
		work->count += 1;
		return;
	}

	// サインイン状態判別
	if (!AoSysIsShowPlatformUI() && AoSysMsgIsFinished()) {

		// サインイン判定
		XUSER_SIGNIN_STATE state = XUserGetSigninState((DWORD)work->ready_id);
		if (state == eXUserSigninState_NotSignedIn) {

			// サインイン促しメッセージ表示
			AoSysMsgStart(AOD_SYS_MSG_PLEASE_SIGNIN, AOD_SYS_MSG_SELECT_YESNO);

			// サインイン促しメッセージ待ちタスクへ遷移
			amTaskSetProcedure(tcb, aoAccountTaskCurrentSetting02);
		}
		else {
			// 完了
			AoSysClearSigninState();
			g_ao_account_is_any_signin_state_changed = TRUE;
			work->enable = TRUE;
			work->enable_temp = TRUE;
			work->is_signin = TRUE;
			work->current_id = work->ready_id;
			work->ready_id = -1;
			amTaskSetProcedure(tcb, aoAccountTaskIdle);

			amMutexLock(&g_ao_account_enable_flag_mutex);
			g_ao_account_enable_flag = TRUE;
			amMutexUnlock(&g_ao_account_enable_flag_mutex);
		}
	}
}

// ===========================================================================
//! カレントアカウント設定02
// ===========================================================================
void aoAccountTaskCurrentSetting02(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_ACCOUNT* work = (AOS_ACCOUNT*)amTaskGetWork(tcb);

	// メッセージ表示完了待ち
	if (AoSysMsgIsFinished()) {

		if (AoSysMsgGetResult() == AOD_SYS_MSG_RESULT_YES) {
			// カレントID設定タスクへ移行
			amTaskSetProcedure(tcb, aoAccountTaskCurrentSetting00);
		}
		else {
			// サインインキャンセル
			AoSysClearSigninState();
			g_ao_account_is_any_signin_state_changed = TRUE;
			work->enable = FALSE;
			work->enable_temp = FALSE;
			work->is_signin = FALSE;
			work->current_id = -1;
			work->ready_id = -1;
			amTaskSetProcedure(tcb, aoAccountTaskIdle);

			amMutexLock(&g_ao_account_enable_flag_mutex);
			g_ao_account_enable_flag = FALSE;
			amMutexUnlock(&g_ao_account_enable_flag_mutex);
		}
	}
}

// ===========================================================================
//! カレントアカウント無効通知00
// ===========================================================================
void aoAccountTaskCurrentDisable00(AMS_TCB* tcb)
{
	// 既存メッセージ表示が終わったら表示開始
	if (AoSysMsgIsFinished()) {
		AoSysMsgStart(AOD_SYS_MSG_SIGNINCHANGED, AOD_SYS_MSG_SELECT_OK);
		amTaskSetProcedure(tcb, aoAccountTaskCurrentDisable01);
	}
}

// ===========================================================================
//! カレントアカウント無効通知01
// ===========================================================================
void aoAccountTaskCurrentDisable01(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_ACCOUNT* work = (AOS_ACCOUNT*)amTaskGetWork(tcb);

	// メッセージ表示完了待ち
	if (AoSysMsgIsFinished()) {

		amMutexLock(&g_ao_account_enable_flag_mutex);
		g_ao_account_enable_flag = FALSE;
		amMutexUnlock(&g_ao_account_enable_flag_mutex);

		// アカウント無効化
		g_ao_account_is_any_signin_state_changed = TRUE;
		work->enable = FALSE;
		amTaskSetProcedure(tcb, aoAccountTaskIdle);
	}
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
void aoAccountTaskDestructor(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_ACCOUNT* work = (AOS_ACCOUNT*)amTaskGetWork(tcb);

	UNREFERENCED_PARAMETER(work);
}

#endif // defined(AOD_PLATFORM_XBOX360)

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
