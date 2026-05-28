// ===========================================================================
/*!
	@file	aoTrophyXbox360.h
	@brief	AoLibrary トロフィー管理モジュール定義(Win32)

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"

#if defined(AOD_PLATFORM_XBOX360)

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
	BOOL				installed;		//!< インストール済みフラグ
	AOE_TROPHY_STATE	state;			//!< 状態
	AOE_TROPHY_ERROR	error;			//!< エラー

	BOOL				th_execute;		//!< スレッド実行中
	AMS_MUTEX			mutex;			//!< ミューテックス
	AMS_THREAD			th;				//!< スレッド

	s32					queue[AOD_TROPHY_QUEUE_NUM];	//!< 獲得待ちキュー
	u32					queue_i_s;		//!< 獲得待ちキュー始点インデックス
	u32					queue_i_e;		//!< 獲得待ちキュー終点インデックス
	s32					th_tno;			//!< 獲得するトロフィー番号

	AMS_TCB*			tcb;			//!< タスク
} AOS_TROPHY;

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// スレッド
static DWORD WINAPI aoTrophyThreadAcquisition(DWORD arg);

// タスク
static void aoTrophyTaskWaitAcquisition(AMS_TCB* tcb);

// グローバルデータ
static AOS_TROPHY* aoTrophyGetGlobal(void);
static void aoTrophySetError(AOE_TROPHY_ERROR error);

// 実績UI表示タスク
static void aoTrophyTaskAchievementUI00(AMS_TCB* tcb);
static void aoTrophyTaskAchievementUI01(AMS_TCB* tcb);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//	AOS_TROPHY g_ao_trophy
// ---------------------------------------------------------------------------
//!	モジュールグローバルデータ
// ===========================================================================
static AOS_TROPHY g_ao_trophy = {
	FALSE, FALSE, AOD_TROPHY_STATE_NONE, AOD_TROPHY_ERROR_NONE,
};

// ===========================================================================
//	AMS_TCB* g_ao_trophy_achievement_ui_tcb
// ---------------------------------------------------------------------------
//!	実績UI表示タスクTCBポインタ
// ===========================================================================
static AMS_TCB* g_ao_trophy_achievement_ui_tcb = NULL;

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
	UNREFERENCED_PARAMETER(is_trial);

	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();

	// 初期化済み判定
	if (glb->initialized) {
		amAssert(0);
		return;
	}

	// グローバルデータ初期化
	glb->initialized = FALSE;
	glb->installed = FALSE;
	glb->state = AOD_TROPHY_STATE_IDLE;
	glb->error = AOD_TROPHY_ERROR_NONE;
	glb->th_execute = FALSE;
	for (int i = 0; i < AOD_TROPHY_QUEUE_NUM; ++i) {
		glb->queue[i] = -1;
	}
	glb->queue_i_s = 0;
	glb->queue_i_e = 0;
	glb->th_tno = -1;
	glb->tcb = NULL;

	g_ao_trophy_achievement_ui_tcb = NULL;

	// ミューテックス作成
	amMutexCreate(&glb->mutex);

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

	if (glb->initialized) {

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

		// グローバルデータ初期化
		glb->installed = FALSE;
		glb->state = AOD_TROPHY_STATE_IDLE;
		glb->error = AOD_TROPHY_ERROR_NONE;
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
	// empty
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
	// empty
	return TRUE;
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
	// empty
	return TRUE;
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


// ***************************************************************************
// 実績UI表示
// ***************************************************************************
// ===========================================================================
//	AoTrophyShowAchievementUI
/*!
	Xbox360の実績UI表示開始

	@note
	既に表示済みの場合は何も行ないません。\n
*/
// ===========================================================================
void AoTrophyShowAchievementUI(void)
{
	if (g_ao_trophy_achievement_ui_tcb == NULL) {
		g_ao_trophy_achievement_ui_tcb = amTaskMake(
			aoTrophyTaskAchievementUI00, NULL,
			0, 0, 0, "aoTrophy::AchievementUI");
		amTaskStart(g_ao_trophy_achievement_ui_tcb);
	}
}

// ===========================================================================
//	AoTrophyIsHideAchievementUI
/*!
	Xbox360の実績UI非表示判定

	@return 真：非表示　偽：表示中
	@note
	AoTrophyShowAchievementUI関数で開始した
	実績UIの表示完了判定に使用してください。\n
*/
// ===========================================================================
BOOL AoTrophyIsHideAchievementUI(void)
{
	if (g_ao_trophy_achievement_ui_tcb) {
		return FALSE;
	}
	return TRUE;
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ***************************************************************************
// スレッド
// ***************************************************************************
// ===========================================================================
//! トロフィー獲得スレッド
// ===========================================================================
DWORD WINAPI aoTrophyThreadAcquisition(DWORD arg)
{
	// 引数取得
	AMS_THREAD* th = (AMS_THREAD*)arg;

	// 開始
	amThreadOpen(th);

	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();

	// カレントアカウントID取得
	s32 cur_id = AoAccountGetCurrentId();
	if ((u32)cur_id >= 4) {
		amThreadQuit(th);
		return 1;
	}

	// 実績解除情報作成
	XUSER_ACHIEVEMENT info;
	info.dwUserIndex = (DWORD)cur_id;
	info.dwAchievementId = (DWORD)glb->th_tno;
	glb->th_tno = -1;

	// カレントアカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		amThreadQuit(th);
		return 1;
	}

	// 実績解除
	DWORD ret = XUserWriteAchievements(1, &info, NULL);
	if (ret != ERROR_SUCCESS) {
		// エラー
		amThreadQuit(th);
		return 1;
	}

	// 終了
	amThreadQuit(th);

	return 0;
}


// ***************************************************************************
// タスク
// ***************************************************************************
// ===========================================================================
//! トロフィー獲得待ちタスク
// ===========================================================================
void aoTrophyTaskWaitAcquisition(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// グローバルデータ取得
	AOS_TROPHY* glb = aoTrophyGetGlobal();

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
			amThreadCreate(
				&glb->th, (void*)aoTrophyThreadAcquisition, NULL,
				(AMD_CORE)0, (u32)THREAD_PRIORITY_BELOW_NORMAL, 0x4000,
				"aoTrophy::Acquisition");
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


// ***************************************************************************
// 実績UI表示タスク
// ***************************************************************************
// ===========================================================================
//! 実績UI表示タスクプロシージャ00 - 表示開始待ち
// ===========================================================================
void aoTrophyTaskAchievementUI00(AMS_TCB* tcb)
{
	// 既存UI表示判定
	if (AoSysIsShowPlatformUI()) {
		amTaskDelete(tcb);
		g_ao_trophy_achievement_ui_tcb = NULL;
		return;
	}

	// アカウント有効判定
	if (!AoAccountIsCurrentEnable()) {
		amTaskDelete(tcb);
		g_ao_trophy_achievement_ui_tcb = NULL;
		return;
	}

	// カレントID取得
	u32 cur_id = (u32)AoAccountGetCurrentId();
	if (cur_id >= 4) {
		amTaskDelete(tcb);
		g_ao_trophy_achievement_ui_tcb = NULL;
		return;
	}

	// カレントアカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		amTaskDelete(tcb);
		g_ao_trophy_achievement_ui_tcb = NULL;
		return;
	}

	// UI表示開始
	if (XShowAchievementsUI(cur_id)!= ERROR_SUCCESS) {
		// エラー発生
		amTaskDelete(tcb);
		g_ao_trophy_achievement_ui_tcb = NULL;
		return;
	}

	// 表示終了待ちへ遷移
	amTaskSetProcedure(tcb, aoTrophyTaskAchievementUI01);
}

// ===========================================================================
//! 実績UI表示タスクプロシージャ01 - 表示終了待ち
// ===========================================================================
void aoTrophyTaskAchievementUI01(AMS_TCB* tcb)
{
	// 既存UI表示判定
	if (!AoSysIsShowPlatformUI()) {
		// 終了
		amTaskDelete(tcb);
		g_ao_trophy_achievement_ui_tcb = NULL;
	}
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
