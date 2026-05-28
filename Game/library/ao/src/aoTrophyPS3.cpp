// ===========================================================================
/*!
	@file	aoTrophyPS3.cpp
	@brief	AoLibrary トロフィー管理モジュール定義(PS3)

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"

#if defined(AOD_PLATFORM_PS3)

// ----- Macros ------------------------------------------------（マクロ定義）

#define AOD_TROPHY_QUEUE_NUM	(16)	//!< トロフィー獲得待ちキュー数

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）

// ===========================================================================
//	struct AOE_TROPHY_STATE
// ---------------------------------------------------------------------------
//!	状態列挙
// ===========================================================================
typedef enum tag_AOE_TROPHY_STATE {
	AOD_TROPHY_STATE_IDLE		= 0,	//!< 待機中
	AOD_TROPHY_STATE_INSTALLING,		//!< インストール中
	AOD_TROPHY_STATE_ACQUISITION,		//!< トロフィー取得中

	AOD_TROPHY_STATE_NUM,				//!< 状態数
	AOD_TROPHY_STATE_NONE,				//!< 無効コード
} AOE_TROPHY_STATE;

// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct AOS_TROPHY
// ---------------------------------------------------------------------------
//!	モジュールグローバル構造体
// ===========================================================================
typedef struct AOS_TROPHY {
	BOOL				initialized;	//!< 初期化済みフラグ
	BOOL				is_trial;		//!< 体験版フラグ
	BOOL				installed;		//!< インストール済みフラグ
	AOE_TROPHY_STATE	state;			//!< 状態
	AOE_TROPHY_ERROR	error;			//!< エラー

	SceNpTrophyContext	context;		//!< コンテキスト
	SceNpTrophyHandle	handle;			//!< ハンドル

	BOOL				th_execute;		//!< スレッド実行中
	AMS_MUTEX			mutex;			//!< ミューテックス
	AMS_THREAD			th;				//!< スレッド

	s32					queue[AOD_TROPHY_QUEUE_NUM];	//!< 獲得待ちキュー
	u32					queue_i_s;		//!< 獲得待ちキュー始点インデックス
	u32					queue_i_e;		//!< 獲得待ちキュー終点インデックス
	s32					th_tno;			//!< 獲得するトロフィー番号

	AMS_TCB*			tcb;			//!< タスク

	BOOL				is_none_space;		//!< 空き容量不足
	BOOL				is_delete_noticed;	//!< HDD不要データ削除通知済み
} AOS_TROPHY;

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// スレッド
static void aoTrophyThreadInstall(uint64_t arg);
static void aoTrophyThreadAcquisition(uint64_t arg);

// コールバック
static int aoTrophyStatusCallback(
	SceNpTrophyContext context, SceNpTrophyStatus status,
	int completed, int total, void* arg);

// タスク
static void aoTrophyTaskWaitInstalled(AMS_TCB* tcb);
static void aoTrophyTaskHddDeleteMessage00(AMS_TCB* tcb);
static void aoTrophyTaskHddDeleteMessage01(AMS_TCB* tcb);
static void aoTrophyTaskWaitAcquisition(AMS_TCB* tcb);

// グローバルデータ
static AOS_TROPHY* aoTrophyGetGlobal(void);
static void aoTrophySetError(AOE_TROPHY_ERROR error);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//	AOS_TROPHY g_ao_trophy
// ---------------------------------------------------------------------------
//!	モジュールグローバルデータ
// ===========================================================================
static AOS_TROPHY g_ao_trophy = {
	FALSE, FALSE, FALSE, AOD_TROPHY_STATE_NONE, AOD_TROPHY_ERROR_NONE,
};

// ===========================================================================
//	SceNpCommunicationId* g_ao_trophy_com_id
// ---------------------------------------------------------------------------
//!	NPコミュニケーションID
// ===========================================================================
static const SceNpCommunicationId* g_ao_trophy_com_id = NULL;

// ===========================================================================
//	SceNpCommunicationSignature* g_ao_trophy_com_sig
// ---------------------------------------------------------------------------
//!	NPコミュニケーションシグネチャ
// ===========================================================================
static const SceNpCommunicationSignature* g_ao_trophy_com_sig = NULL;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ***************************************************************************
// 初期化&終了処理
// ***************************************************************************
// ===========================================================================
//	AoTrophyInit
/*!
	トロフィー管理モジュール初期化処理

	@param is_trial	[in] 真：体験版　偽：製品版
	@note
	アプリケーション起動時に1度だけ呼び出して下さい。\n
	あらかじめ、AoAccountInit関数が呼ばれている必要があります。\n
*/
// ===========================================================================
void AoTrophyInit(BOOL is_trial)
{
	int ret;

	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();

	// 初期化済み判定
	if (glb->initialized) {
		amAssert(0);
		return;
	}

	// グローバルデータ初期化
	glb->initialized = FALSE;
	glb->is_trial = is_trial;
	glb->installed = FALSE;
	glb->state = AOD_TROPHY_STATE_IDLE;
	glb->error = AOD_TROPHY_ERROR_NONE;
	glb->context = SCE_NP_TROPHY_INVALID_CONTEXT;
	glb->handle = SCE_NP_TROPHY_INVALID_HANDLE;
	glb->th_execute = FALSE;
	for (int i = 0; i < AOD_TROPHY_QUEUE_NUM; ++i) {
		glb->queue[i] = -1;
	}
	glb->queue_i_s = 0;
	glb->queue_i_e = 0;
	glb->th_tno = -1;
	glb->tcb = NULL;
	glb->is_none_space = FALSE;
	glb->is_delete_noticed = FALSE;

	// 体験版なら終了
	if (glb->is_trial) {
		return;
	}

	// ミューテックス作成
	amMutexCreate(&glb->mutex);

	// モジュールのロード
	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP_TROPHY);
	if (ret < 0) {
		amAssert(0);
	}

	// ユーティリティの初期化
	ret = sceNpTrophyInit(NULL, 0, SYS_MEMORY_CONTAINER_ID_INVALID, 0);
	if (ret < 0) {
		amAssert(0);
	}

	// コンテキストの初期化
	ret = sceNpTrophyCreateContext(
		&glb->context,
		g_ao_trophy_com_id,
		g_ao_trophy_com_sig,
		0);
	if (ret < 0) {
		amAssert(0);
	}

	// ハンドルの初期化
	ret = sceNpTrophyCreateHandle(&glb->handle);
	if (ret < 0) {
		amAssert(0);
	}

	// 初期化済み設定
	glb->initialized = TRUE;
}

// ===========================================================================
//	AoTrophyExit
/*!
	トロフィー管理モジュール終了処理

	@note
	アプリケーション終了時に1度だけ呼び出して下さい。\n
	AoTrophyInit関数が呼び出されていない状態では、
	この関数を呼び出さないようにして下さい。\n
	この関数を呼出し後、再度AoTrophyInit関数を呼び出すことはできません。\n
*/
// ===========================================================================
void AoTrophyExit(void)
{
	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();

	if (glb->initialized && (glb->is_trial == FALSE)) {

		// タスク終了
		if (glb->tcb) {
			amTaskDelete(glb->tcb);
			glb->tcb = NULL;
		}

		// スレッド終了
		if (glb->th_execute) {
			amThreadExit(&glb->th);
			amThreadWaitQuit(&glb->th);
			amThreadDelete(&glb->th);
			glb->th_execute = FALSE;
		}

		// ミューテックス削除
		amMutexDelete(&glb->mutex);

		int ret;

		// ハンドルの破棄
		ret = sceNpTrophyDestroyHandle(glb->handle);
		if ((ret < 0) && ((u32)ret != SCE_NP_TROPHY_ERROR_SHUTDOWN)) {
			amAssert(0);
		}

		// コンテキストの破棄
		ret = sceNpTrophyDestroyContext(glb->context);
		if ((ret < 0) && ((u32)ret != SCE_NP_TROPHY_ERROR_SHUTDOWN)) {
			amAssert(0);
		}

		// ユーティリティの終了
		ret = sceNpTrophyTerm();
		if ((ret < 0) && ((u32)ret != SCE_NP_TROPHY_ERROR_SHUTDOWN)) {
			amAssert(0);
		}

		// モジュールのアンロード
		ret = cellSysmoduleUnloadModule(CELL_SYSMODULE_SYSUTIL_NP_TROPHY);
		if (ret < 0) {
			amAssert(0);
		}

		// グローバルデータ初期化
		glb->installed = FALSE;
		glb->state = AOD_TROPHY_STATE_IDLE;
		glb->error = AOD_TROPHY_ERROR_NONE;
		glb->context = SCE_NP_TROPHY_INVALID_CONTEXT;
		glb->handle = SCE_NP_TROPHY_INVALID_HANDLE;
		glb->th_execute = FALSE;
		for (int i = 0; i < AOD_TROPHY_QUEUE_NUM; ++i) {
			glb->queue[i] = -1;
		}
		glb->queue_i_s = 0;
		glb->queue_i_e = 0;
		glb->th_tno = -1;
		glb->tcb = NULL;

		// 未初期化設定
		glb->initialized = FALSE;
	}
}

// ===========================================================================
//	AoTrophyPs3SetComId
/*!
	PS3 NPコミュニケーションID設定

	@param id		[in] NPコミュニケーションID
*/
// ===========================================================================
void AoTrophyPs3SetComId(const SceNpCommunicationId* id)
{
	g_ao_trophy_com_id = id;
}

// ===========================================================================
//	AoTrophyPs3SetComSignature
/*!
	PS3 NPコミュニケーションシグネチャ設定

	@param sig		[in] NPコミュニケーションシグネチャ
*/
// ===========================================================================
void AoTrophyPs3SetComSignature(const SceNpCommunicationSignature* sig)
{
	g_ao_trophy_com_sig = sig;
}


// ***************************************************************************
// エラー
// ***************************************************************************
// ===========================================================================
//	AoTrophyIsError
/*!
	エラー判定

	@return 真：エラーあり　偽：エラーなし
	@note
	このモジュール内でエラーが発生したかどうかを判定します。\n
	AoTrophyClearError関数を呼び出すと、エラーなしの状態となり、
	その後エラーが発生するとTRUEを返すようになります。\n
*/
// ===========================================================================
BOOL AoTrophyIsError(void)
{
	if (aoTrophyGetGlobal()->is_trial) {
		return FALSE;
	}
	if (aoTrophyGetGlobal()->error != AOD_TROPHY_ERROR_NONE) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//	AoTrophyGetError
/*!
	エラー取得

	@return 真：エラー
	@note
	最後に発生したエラー情報を返します。\n
	AoTrophyIsError関数がFALSEを返す状態で呼び出すと
	AOD_TROPHY_ERROR_NONEを返します。\n
*/
// ===========================================================================
AOE_TROPHY_ERROR AoTrophyGetError(void)
{
	if (aoTrophyGetGlobal()->is_trial) {
		return AOD_TROPHY_ERROR_NONE;
	}
	return aoTrophyGetGlobal()->error;
}

// ===========================================================================
//	AoTrophyClearError
/*!
	エラークリア
*/
// ===========================================================================
void AoTrophyClearError(void)
{
	aoTrophyGetGlobal()->error = AOD_TROPHY_ERROR_NONE;
}


// ***************************************************************************
// インストール
// ***************************************************************************
// ===========================================================================
//	AoTrophyInstallStart
/*!
	トロフィーのインストール開始

	@note
	アプリケーションでトロフィー関連の処理を行うために、
	(必要なプラットフォームのみ)
	トロフィーファイルのインストールを行います。\n
*/
// ===========================================================================
void AoTrophyInstallStart(void)
{
	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();

	// 体験版なら何もしない
	if (glb->is_trial) {
		return;
	}

	// 何かの処理中ならエラー
	if (glb->state != AOD_TROPHY_STATE_IDLE) {
		aoTrophySetError(AOD_TROPHY_ERROR_OPERATION);
		return;
	}
	amAssert(glb->th_execute == FALSE);
	amAssert(glb->tcb == NULL);

	// 状態変更
	glb->state = AOD_TROPHY_STATE_INSTALLING;
	glb->is_none_space = FALSE;
	glb->is_delete_noticed = FALSE;

	// インストールスレッド作成
	glb->th_execute = TRUE;
	s32 prio;
	sys_ppu_thread_t id;
	sys_ppu_thread_get_id(&id);
	sys_ppu_thread_get_priority(id, &prio);
	prio += 1;
	amThreadCreate(
		&glb->th, (void*)aoTrophyThreadInstall, NULL,
		(AMD_CORE)0, prio, 0x4000, "aoTrophy::Install");

	// インストール完了待ちタスク作成
	glb->tcb = amTaskMake(
		aoTrophyTaskWaitInstalled, NULL, 0, 0, 0, "aoTrophy::Install");

	// インストール完了待ちタスク起動
	amTaskStart(glb->tcb);
}

// ===========================================================================
//	AoTrophyInstallIsFinished
/*!
	トロフィーのインストール完了判定

	@return 真：完了済み　偽：インストール中
	@note
	AoTrophyInstallStart関数で開始したインストール処理が
	完了したかどうかを判定します。\n
	AoTrophyInstallStart関数を呼び出していない場合はTRUEを返します。\n
*/
// ===========================================================================
BOOL AoTrophyInstallIsFinished(void)
{
	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();

	// 体験版なら何もしない
	if (glb->is_trial) {
		return TRUE;
	}

	// インストール中でなければ完了と判定
	if (glb->state != AOD_TROPHY_STATE_INSTALLING) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//	AoTrophyInstallIsSuccess
/*!
	トロフィーのインストール成功判定

	@return 真：成功　偽：失敗
	@note
	AoTrophyInstallStart関数で開始したインストール処理が
	成功したかどうかを判定します。\n
	この関数はAoTrophyInstallIsFinished関数が
	TRUEを返す状態でのみ呼び出し可能です。\n
	インストールが失敗した場合の詳細は、
	AoTrophyGetError関数で取得して下さい。\n
	AoTrophyInstallStart関数を呼び出していない場合はFALSEを返します。\n
*/
// ===========================================================================
BOOL AoTrophyInstallIsSuccess(void)
{
	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();

	// 体験版なら何もしない
	if (glb->is_trial) {
		return TRUE;
	}

	amAssert(AoTrophyInstallIsFinished());

	// インストール済み判定
	amMutexLock(&glb->mutex);
	BOOL ret = glb->installed;
	amMutexUnlock(&glb->mutex);

	return ret;
}


// ***************************************************************************
// 獲得
// ***************************************************************************
// ===========================================================================
//	AoTrophyAcquisitionTaskStart
/*!
	トロフィー獲得監視タスク開始

	@note
	AoTrophyAcquisition関数によるトロフィーの獲得を監視し、
	必要な獲得処理を行うタスクを生成します。\n
	この関数でタスクの生成を行わないとトロフィーの獲得処理が行われません。\n
	すでに生成済みの場合は何も行いません。\n
*/
// ===========================================================================
void AoTrophyAcquisitionTaskStart(void)
{
	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();

	// 体験版なら何もしない
	if (glb->is_trial) {
		return;
	}

	// 既に監視タスクが動作しているなら何もしない
	if (glb->state == AOD_TROPHY_STATE_ACQUISITION) {
		return;
	}

	// 何かの処理中ならエラー
	if (glb->state != AOD_TROPHY_STATE_IDLE) {
		aoTrophySetError(AOD_TROPHY_ERROR_OPERATION);
		return;
	}
	amAssert(glb->th_execute == FALSE);
	amAssert(glb->tcb == NULL);

	// 状態変更
	glb->state = AOD_TROPHY_STATE_ACQUISITION;

	// 監視タスク作成
	glb->tcb = amTaskMake(
		aoTrophyTaskWaitAcquisition, NULL, 0, 0, 0, "aoTrophy::Acquisition");

	// 監視タスク起動
	amTaskStart(glb->tcb);
}

// ===========================================================================
//	AoTrophyAcquisitionTaskEnd
/*!
	トロフィー獲得監視タスク終了

	@note
	AoTrophyAcquisitionTaskStart関数で開始した監視タスクを終了させます。\n
	以降、AoTrophyAcquisition関数でトロフィー獲得を指示しても、
	再度監視タスクを生成するまで獲得処理は行われません。\n
	すでに終了済みの場合は何も行いません。\n
*/
// ===========================================================================
void AoTrophyAcquisitionTaskEnd(void)
{
	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();

	// 体験版なら何もしない
	if (glb->is_trial) {
		return;
	}

	// 既に監視タスクが動作していないなら何もしない
	if (glb->state != AOD_TROPHY_STATE_ACQUISITION) {
		return;
	}

	// タスク終了
	if (glb->tcb) {
		amTaskDelete(glb->tcb);
		glb->tcb = NULL;
	}

	// スレッド終了
	if (glb->th_execute) {
		amThreadExit(&glb->th);
		amThreadWaitQuit(&glb->th);
		amThreadDelete(&glb->th);
		glb->th_execute = FALSE;
	}

	// 状態変更
	glb->state = AOD_TROPHY_STATE_IDLE;
}

// ===========================================================================
//	AoTrophyAcquisition
/*!
	トロフィー獲得

	@param no	[in] トロフィー番号
*/
// ===========================================================================
void AoTrophyAcquisition(u32 no)
{
	// インストールされていないなら何も行わない
	if (!AoTrophyInstallIsSuccess()) {
		return;
	}

	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();

	// 体験版なら何もしない
	if (glb->is_trial) {
		return;
	}

	// キューに登録
	amMutexLock(&glb->mutex);
	amAssert(glb->queue[glb->queue_i_e] < 0);
	glb->queue[glb->queue_i_e] = (s32)no;
	glb->queue_i_e += 1;
	if (glb->queue_i_e >= AOD_TROPHY_QUEUE_NUM) {
		glb->queue_i_e = 0;
	}
	amMutexUnlock(&glb->mutex);
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ***************************************************************************
// スレッド
// ***************************************************************************
// ===========================================================================
//! インストールスレッド
// ===========================================================================
void aoTrophyThreadInstall(uint64_t arg)
{
	// スレッド開始
	amThreadOpen((AMS_THREAD*)arg);

	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();
	amAssert(glb->is_trial == FALSE);

	int ret;
	uint64_t need_space;

	// HDDの空き容量取得
	u32 free_space;
	char dirName[CELL_GAME_DIRNAME_SIZE];
	{
		unsigned int type = 0;
		unsigned int attributes = 0;
		CellGameContentSize content_size;
		char contentInfoPath[CELL_GAME_HDDGAMEPATH_SIZE];
		char usrdirPath[CELL_GAME_HDDGAMEPATH_SIZE];
		ret = cellGameBootCheck(&type, &attributes, &content_size, dirName);
		if (ret != CELL_GAME_RET_OK) {
			goto install;
		}
		ret = cellGameContentPermit(contentInfoPath, usrdirPath);
		if (ret != CELL_GAME_RET_OK) {
			goto install;
		}
		free_space = (u32)content_size.hddFreeSizeKB;
	}

	// 終了判定
	if (amThreadCheckExit(&glb->th)) {
		amMutexLock(&glb->mutex);
		glb->error = AOD_TROPHY_ERROR_OTHER;
		glb->installed = FALSE;
		amMutexUnlock(&glb->mutex);
		amThreadQuit(&glb->th);
		return;
	}

	// 必要な空き容量取得
	ret = sceNpTrophyGetRequiredDiskSpace(
		glb->context, glb->handle, &need_space, 0);
	if (ret < 0) {
		goto install;
	}
	need_space = (u32)((need_space + 1023) / 1024);

	// 空き容量不足判定
	if ((u32)need_space > free_space) {
		// 空き容量不足メッセージ表示
		ret = cellGameContentErrorDialog(
			CELL_GAME_ERRDIALOG_NOSPACE,
			(int)(need_space - free_space),
			dirName);

		// 空き容量不足通知
		amMutexLock(&glb->mutex);
		glb->is_none_space = TRUE;
		amMutexUnlock(&glb->mutex);

		// 空き容量不足通知完了判定
		while (1) {
			amMutexLock(&glb->mutex);
			BOOL flag = glb->is_delete_noticed;
			amMutexUnlock(&glb->mutex);

			if (flag) {
				break;
			}

			// 終了判定
			if (amThreadCheckExit(&glb->th)) {
				amMutexLock(&glb->mutex);
				glb->error = AOD_TROPHY_ERROR_OTHER;
				glb->installed = FALSE;
				amMutexUnlock(&glb->mutex);
				amThreadQuit(&glb->th);
				return;
			}

			// 100ミリ秒程度スリープ
			sys_timer_usleep(100 * 1000);
		}
	}

install:

	// 終了判定
	if (amThreadCheckExit(&glb->th)) {
		amMutexLock(&glb->mutex);
		glb->error = AOD_TROPHY_ERROR_OTHER;
		glb->installed = FALSE;
		amMutexUnlock(&glb->mutex);
		amThreadQuit(&glb->th);
		return;
	}

	// インストール&同期処理
	ret = sceNpTrophyRegisterContext(
		glb->context, glb->handle, aoTrophyStatusCallback, NULL,
		SCE_NP_TROPHY_OPTIONS_REGISTER_CONTEXT_SHOW_ERROR_EXIT);

	// 結果設定
	amMutexLock(&glb->mutex);
	if (ret < 0) {
		// エラー設定
		switch (ret) {
		case SCE_NP_TROPHY_ERROR_CONF_DOES_NOT_EXIST:
		case SCE_NP_TROPHY_ERROR_VERIFICATION_FAILURE:
		case SCE_NP_TROPHY_ERROR_TITLE_ICON_NOT_FOUND:
		case SCE_NP_TROPHY_ERROR_TROPHY_ICON_NOT_FOUND:
		case SCE_NP_TROPHY_ERROR_ILLEGAL_UPDATE:
			glb->error = AOD_TROPHY_ERROR_FILE_FAILURE;
			break;
		case SCE_NP_TROPHY_ERROR_INSUFFICIENT_DISK_SPACE:
			glb->error = AOD_TROPHY_ERROR_DISK_SPACE;
			break;
		default:
			glb->error = AOD_TROPHY_ERROR_OTHER;
			break;
		}
		glb->installed = FALSE;
	}
	else {
		// インストール完了設定
		glb->installed = TRUE;
	}
	amMutexUnlock(&glb->mutex);

	// スレッド終了
	amThreadQuit(&glb->th);
}

// ===========================================================================
//! トロフィー獲得スレッド
// ===========================================================================
void aoTrophyThreadAcquisition(uint64_t arg)
{
	UNREFERENCED_PARAMETER(arg);

	int ret;

	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();
	amAssert(glb->is_trial == FALSE);

	// 獲得スレッド開始
	amThreadOpen(&glb->th);

	// トロフィーID作成
	SceNpTrophyId tid = (SceNpTrophyId)(glb->th_tno);
	glb->th_tno = -1;

	// 指定IDのトロフィー情報取得->ロック判定
	SceNpTrophyDetails tdetails;
	SceNpTrophyData tdata;
	amZeroMemory(&tdetails, sizeof(SceNpTrophyDetails));
	amZeroMemory(&tdata, sizeof(SceNpTrophyData));
	ret = sceNpTrophyGetTrophyInfo(
		glb->context, glb->handle, tid, &tdetails, &tdata);
	if (ret < 0) {
		if ((u32)ret == SCE_NP_TROPHY_ERROR_LOCKED) {
			// ロックされている
		}
		else if ((u32)ret == SCE_NP_TROPHY_ERROR_HIDDEN) {
			// 隠されているならロックされているとみなす
		}
		else {
			// 普通にエラー
			amThreadQuit(&glb->th);
			return;
		}
	}

	// アンロック済み判定
	else if (tdata.unlocked) {
		amThreadQuit(&glb->th);
		return;
	}

	// アンロック
	SceNpTrophyId platinumId = SCE_NP_TROPHY_INVALID_TROPHY_ID;
	ret = sceNpTrophyUnlockTrophy(
		glb->context, glb->handle, tid, &platinumId);
	if (ret < 0) {
		amThreadQuit(&glb->th);
		return;
	}
	if (platinumId != SCE_NP_TROPHY_INVALID_TROPHY_ID) {
		// プラチナトロフィーアンロック
		// 未作成
	}

	// 終了
	amThreadQuit(&glb->th);
}


// ***************************************************************************
// コールバック
// ***************************************************************************
// ===========================================================================
//! インストール&同期処理コールバック
// ===========================================================================
int aoTrophyStatusCallback(
	SceNpTrophyContext context, SceNpTrophyStatus status,
	int completed, int total, void* arg)
{
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(status);
	UNREFERENCED_PARAMETER(completed);
	UNREFERENCED_PARAMETER(total);
	UNREFERENCED_PARAMETER(arg);

	return 0;
}


// ***************************************************************************
// タスク
// ***************************************************************************
// ===========================================================================
//! インストール完了待ちタスク
// ===========================================================================
void aoTrophyTaskWaitInstalled(AMS_TCB* tcb)
{
	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();
	amAssert(glb->is_trial == FALSE);

	// スレッド完了判定
	if (amThreadCheckQuit(&glb->th)) {

		// スレッド破棄
		amThreadDelete(&glb->th);

		// タスク終了
		amTaskDelete(tcb);

		// 完了設定
		glb->state = AOD_TROPHY_STATE_IDLE;
		glb->th_execute = FALSE;
		glb->tcb = NULL;
	}
	else {

		// 空き容量不足判定
		amMutexLock(&glb->mutex);
		if (!glb->is_delete_noticed) {
			if (glb->is_none_space) {
				// HDD空き容量確保メッセージ表示へ遷移
				amTaskSetProcedure(tcb, aoTrophyTaskHddDeleteMessage00);
			}
		}
		amMutexUnlock(&glb->mutex);
	}
}

// ===========================================================================
//! インストール中 HDD空き容量確保メッセージ表示00
// ===========================================================================
void aoTrophyTaskHddDeleteMessage00(AMS_TCB* tcb)
{
	// 既存メッセージ表示終了待ち
	if (AoSysMsgIsFinished()) {
		// メッセージ表示
		AoSysMsgStart(AOD_SYS_MSG_HDD_DATA_DELETE, AOD_SYS_MSG_SELECT_OK);

		// メッセージ表示待ちへ遷移
		amTaskSetProcedure(tcb, aoTrophyTaskHddDeleteMessage01);
	}
}

// ===========================================================================
//! インストール中 HDD空き容量確保メッセージ表示01
// ===========================================================================
void aoTrophyTaskHddDeleteMessage01(AMS_TCB* tcb)
{
	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();
	amAssert(glb->is_trial == FALSE);

	// メッセージ表示終了待ち
	if (AoSysMsgIsFinished()) {

		// 通知済みフラグON
		amMutexLock(&glb->mutex);
		glb->is_delete_noticed = TRUE;
		amMutexUnlock(&glb->mutex);

		// インストール完了待ちへ遷移
		amTaskSetProcedure(tcb, aoTrophyTaskWaitInstalled);
	}
}

// ===========================================================================
//! トロフィー獲得待ちタスク
// ===========================================================================
void aoTrophyTaskWaitAcquisition(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();
	amAssert(glb->is_trial == FALSE);

	// スレッド起動中なら終了待ち
	if (glb->th_execute) {
		if (!amThreadCheckQuit(&glb->th)) {
			return;
		}

		// スレッド破棄
		amThreadDelete(&glb->th);
		glb->th_execute = FALSE;
	}

	// 新規獲得判定
	amMutexLock(&glb->mutex);
	if (glb->queue[glb->queue_i_s] >= 0) {
		glb->th_tno = glb->queue[glb->queue_i_s];
		glb->queue[glb->queue_i_s] = -1;
		glb->queue_i_s += 1;
		if (glb->queue_i_s >= AOD_TROPHY_QUEUE_NUM) {
			glb->queue_i_s = 0;
		}

		// セーブデータの作者が自身か判定
		if (AoStorageLoadIsCreaterOwn()) {

			// 獲得スレッド作成
			glb->th_execute = TRUE;
			s32 prio;
			sys_ppu_thread_t id;
			sys_ppu_thread_get_id(&id);
			sys_ppu_thread_get_priority(id, &prio);
			prio += 1;
			amThreadCreate(
				&glb->th, (void*)aoTrophyThreadAcquisition, NULL,
				(AMD_CORE)0, prio, 0x4000, "aoTrophy::Acquisition");
		}
	}
	amMutexUnlock(&glb->mutex);
}


// ***************************************************************************
// グローバルデータ
// ***************************************************************************
// ===========================================================================
//! グローバル構造体取得
// ===========================================================================
AOS_TROPHY* aoTrophyGetGlobal(void)
{
	return &g_ao_trophy;
}

// ===========================================================================
//! エラー設定
// ===========================================================================
void aoTrophySetError(AOE_TROPHY_ERROR error)
{
	// 操作エラーならアサート
	amAssert(error != AOD_TROPHY_ERROR_OPERATION);
	g_ao_trophy.error = error;
}

#endif // defined(AOD_PLATFORM_PS3)

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
