// ===========================================================================
/*!
	@file	aoStorageWii.cpp
	@brief	AoLibrary セーブ＆ロード管理モジュール定義(Wii)

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"

#if defined(AOD_PLATFORM_WII)

// ----- Macros ------------------------------------------------（マクロ定義）

#define AOD_STORAGE_FBLOCK_SIZE		(16 * 1024)		//!< ファイルブロックサイズ
#define AOD_STORAGE_BLOCK_SIZE		(128 * 1024)	//!< ブロックサイズ

#define AOD_STORAGE_MASIC	(0x3c998bc1)	//!< セーブデータ識別子

#define AOD_STORAGE_MULTIPLE		(4)		//!< セーブデータ多重化数

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）

// ===========================================================================
//	struct AOE_STORAGE_STATE
// ---------------------------------------------------------------------------
//!	状態列挙
// ===========================================================================
typedef enum tag_AOE_STORAGE_STATE {
	AOD_STORAGE_STATE_IDLE		= 0,	//!< 待機中
	AOD_STORAGE_STATE_SAVING,			//!< セーブ中
	AOD_STORAGE_STATE_LOADING,			//!< ロード中
	AOD_STORAGE_STATE_DELETE,			//!< 削除中

	AOD_STORAGE_STATE_NUM,				//!< 状態数
	AOD_STORAGE_STATE_NONE,				//!< 無効コード
} AOE_STORAGE_STATE;

// ===========================================================================
//	struct AOE_STORAGE_LERROR
// ---------------------------------------------------------------------------
//!	ローカルエラー列挙
// ===========================================================================
typedef enum tag_AOE_STORAGE_LERROR {
	AOD_STORAGE_LERROR_NAND_02	= 0,	//!< 空き容量不足
	AOD_STORAGE_LERROR_NAND_03,			//!< 空きiノード数不足
	AOD_STORAGE_LERROR_NAND_07,			//!< データ破損
	AOD_STORAGE_LERROR_NAND_08,			//!< Wii本体保存メモリ破損
	AOD_STORAGE_LERROR_NAND_11,			//!< アクセス不能.
	AOD_STORAGE_LERROR_NAND_12,			//!< アクセス不能(UNKNOWN)

	AOD_STORAGE_LERROR_NUM,				//!< ローカルエラー数
	AOD_STORAGE_LERROR_NONE,			//!< 無効コード
} AOE_STORAGE_LERROR;

// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct AOS_STORAGE
// ---------------------------------------------------------------------------
//!	グローバルデータ
// ===========================================================================
typedef struct tag_AOS_STORAGE {
	BOOL				initialized;	//!< 初期化済みフラグ
	AOE_STORAGE_STATE	state;			//!< 状態
	AOE_STORAGE_ERROR	error;			//!< エラー

	BOOL				save_success;	//!< セーブ成功フラグ
	void*				save_buf;		//!< セーブデータバッファ
	u32					save_size;		//!< セーブデータサイズ
	BOOL				save_first;		//!< 真：初回セーブ　偽：上書きセーブ

	BOOL				load_success;	//!< ロード成功フラグ
	void*				load_buf;		//!< ロードデータバッファ
	u32					load_size;		//!< ロードデータサイズ

	BOOL				del_success;	//!< 削除成功フラグ

	BOOL				th_execute;		//!< スレッド実行中
	AMS_THREAD			th;				//!< スレッド

	AMS_TCB*			tcb;			//!< タスクTCBポインタ

	AMS_MUTEX	space_check_mutex;	//!< 空き容量チェックフラグ用ミューテックス
} AOS_STORAGE;

// ===========================================================================
//	struct AOS_STORAGE_SAVE
// ---------------------------------------------------------------------------
//!	セーブタスクワーク
// ===========================================================================
typedef struct tag_AOS_STORAGE_SAVE {
	AOE_STORAGE_LERROR	lerror;			//!< ローカルエラー
	u32					need_block;		//!< 本体メモリに不足しているブロック
	BOOL				space_check;	//!< 空き容量チェックフラグ
} AOS_STORAGE_SAVE;

// ===========================================================================
//	struct AOS_STORAGE_LOAD
// ---------------------------------------------------------------------------
//!	ロードタスクワーク
// ===========================================================================
typedef struct tag_AOS_STORAGE_LOAD {
	AOE_STORAGE_LERROR	lerror;			//!< ローカルエラー
	BOOL				remake_banner;	//!< バナー再作成フラグ
} AOS_STORAGE_LOAD;

// ===========================================================================
//	struct AOS_STORAGE_HEADER
// ---------------------------------------------------------------------------
//!	セーブデータヘッダ
// ===========================================================================
typedef struct tag_AOS_STORAGE_HEADER {
	u32					masic;			//!< セーブデータ識別子
	u32					file_size;		//!< ファイルサイズ
	u32					data_size;		//!< データサイズ
	u32					crc32;			//!< CRC32
} AOS_STORAGE_HEADER; // 16 byte

#if defined(AOD_DEBUG)
// ===========================================================================
//	struct AOS_STORAGE_HEADER
// ---------------------------------------------------------------------------
//!	デバッグ用の最初のロード＆セーブ タスクワーク
// ===========================================================================
typedef struct tag_AOS_STORAGE_DEBUG_LS {
	void*				data;			//!< セーブデータバッファ
	u32					size;			//!< セーブデータバッファサイズ
} AOS_STORAGE_DEBUG_LS;
#endif // defined(AOD_DEBUG)

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// セーブ
static void aoStorageSaveThread(void);
static void aoStorageSaveTaskProcedure(AMS_TCB* tcb);
static void aoStorageSaveTaskDestructor(AMS_TCB* tcb);
static void aoStorageSaveTaskLocalErrorStartMessage(AMS_TCB* tcb);
static void aoStorageSaveTaskErrorToWiiMenu00(AMS_TCB* tcb);
static void aoStorageSaveTaskErrorToWiiMenu01(AMS_TCB* tcb);
static void aoStorageSaveTaskErrorToStop00(AMS_TCB* tcb);
static void aoStorageSaveDeleteThread(void);
static void aoStorageSaveTaskDelete00(AMS_TCB* tcb);
static void aoStorageSaveTaskDelete01(AMS_TCB* tcb);
static void aoStorageSaveTaskDelete02(AMS_TCB* tcb);

// ロード
static void aoStorageLoadThread(void);
static void aoStorageLoadTaskProcedure(AMS_TCB* tcb);
static void aoStorageLoadSaveBannerThread(void);
static void aoStorageLoadSaveBannerTaskProcedure(AMS_TCB* tcb);
static void aoStorageLoadTaskLocalErrorStartMessage(AMS_TCB* tcb);
static void aoStorageLoadTaskErrorToDelete00(AMS_TCB* tcb);
static void aoStorageLoadTaskErrorToDelete01(AMS_TCB* tcb);
static void aoStorageLoadTaskErrorToWiiMenu00(AMS_TCB* tcb);
static void aoStorageLoadTaskErrorToWiiMenu01(AMS_TCB* tcb);
static void aoStorageLoadTaskErrorToStop00(AMS_TCB* tcb);
static void aoStorageLoadTaskDelete00(AMS_TCB* tcb);
static void aoStorageLoadTaskDelete01(AMS_TCB* tcb);
static void aoStorageLoadTaskDelete02(AMS_TCB* tcb);

// 削除
static void aoStorageDeleteTaskProcedure(AMS_TCB* tcb);
static void aoStorageDeleteTaskLocalErrorStartMessage(AMS_TCB* tcb);
static void aoStorageDeleteTaskErrorToStop00(AMS_TCB* tcb);

// グローバルデータ
static AOS_STORAGE* aoStorageGetGlobal(void);
static void aoStorageSetError(AOE_STORAGE_ERROR error);

// その他
static u32 aoStorageMakeBanner(NANDBanner* banner);
static u32 aoStorageGetBannerSize(void);
static void aoStorageMakeBannerFileName(char* fname);
static void aoStorageMakeDataFileName(char* fname);
static u32 aoStorageConvByteToFblock(u32 byte_size);
static u32 aoStorageConvFblockToBlock(u32 fb_size);
static AOE_STORAGE_LERROR aoStorageGetLocalError(s32 result);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//	AOS_STORAGE g_ao_storage
// ---------------------------------------------------------------------------
//!	グローバルデータ
// ===========================================================================
static AOS_STORAGE g_ao_storage = {
	FALSE, AOD_STORAGE_STATE_NONE, AOD_STORAGE_ERROR_NONE,
};

// ===========================================================================
//! バナーとアイコンのTPLファイル
// ===========================================================================
static u8* g_ao_storage_save_tpl = NULL;

// ===========================================================================
//! バナーとアイコンのTPLファイルのサイズ
// ===========================================================================
static u32 g_ao_storage_save_tpl_size = 0;

// ===========================================================================
//! セーブデータメッセージファイル
// ===========================================================================
static const void* g_ao_storage_save_msg_file = NULL;

#if defined(AOD_DEBUG)
// ===========================================================================
//! デバッグ用の最初のロード＆セーブ タスクTCB
// ===========================================================================
static AMS_TCB* g_ao_storage_debug_ls_tcb = NULL;

// ===========================================================================
//! デバッグ用の最初のロード＆セーブ エラーフラグ
// ===========================================================================
static BOOL g_ao_storage_debug_ls_is_error = FALSE;
#endif // defined(AOD_DEBUG)

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ***************************************************************************
// 初期化&終了処理
// ***************************************************************************
// ===========================================================================
//	AoStorageInit
/*!
	セーブ＆ロード管理モジュール初期化処理

	@note
	アプリケーション起動時に1度だけ呼び出して下さい。\n
	あらかじめ、AoAccountInit関数が呼ばれている必要があります。\n
*/
// ===========================================================================
void AoStorageInit(void)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// 初期化済み判定
	if (glb->initialized) {
		amAssert(0);
		return;
	}

	// グローバルデータ初期化
	glb->initialized = FALSE;
	glb->state = AOD_STORAGE_STATE_IDLE;
	glb->error = AOD_STORAGE_ERROR_NONE;
	glb->save_success = FALSE;
	glb->save_buf = NULL;
	glb->save_size = 0;
	glb->save_first = FALSE;
	glb->load_success = FALSE;
	glb->load_buf = NULL;
	glb->load_size = 0;
	glb->del_success = FALSE;
	glb->th_execute = FALSE;
	glb->tcb = NULL;

	// 初期化済み設定
	glb->initialized = TRUE;
}

// ===========================================================================
//	AoStorageExit
/*!
	セーブ＆ロード管理モジュール終了処理

	@note
	アプリケーション終了時に1度だけ呼び出して下さい。\n
	AoStorageInit関数が呼び出されていない状態では、
	この関数を呼び出さないようにして下さい。\n
	この関数を呼出し後、再度AoStorageInit関数を呼び出すことはできません。\n
*/
// ===========================================================================
void AoStorageExit(void)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

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

			// ゲーム終了を有効化
			amSystemExitEnable(1);
		}

		// グローバルデータ初期化
		glb->initialized = FALSE;
		glb->state = AOD_STORAGE_STATE_IDLE;
		glb->error = AOD_STORAGE_ERROR_NONE;
		glb->save_success = FALSE;
		glb->save_buf = NULL;
		glb->save_size = 0;
		glb->save_first = FALSE;
		glb->load_success = FALSE;
		glb->load_buf = NULL;
		glb->load_size = 0;
		glb->del_success = FALSE;
		glb->th_execute = FALSE;
		glb->tcb = NULL;

		// 未初期化設定
		glb->initialized = FALSE;
	}
}

// ===========================================================================
//	AoStorageSetDataWii
/*!
	Wii用のセーブに必要なデータ設定

	@param tpl		[in] バナーとアイコンのTPLファイル
	@param tpl_size	[in] バナーとアイコンのTPLファイルのサイズ
*/
// ===========================================================================
void AoStorageSetDataWii(void* tpl, u32 tpl_size)
{
	g_ao_storage_save_tpl = (u8*)tpl;
	g_ao_storage_save_tpl_size = tpl_size;
}

// ===========================================================================
//	AoStorageSetSaveMsgFile
/*!
	セーブデータメッセージファイル設定

	@param file		[in] セーブデータメッセージファイル
	@note
	セーブデータに格納する各種メッセージを格納した
	メッセージファイルを設定します。\n
	このモジュールを使用するためには、
	この関数で適切なメッセージファイルが設定されている必要があります。\n
	アプリケーション開始時に設定し、以降解放はしない使い方を想定しています。\n
*/
// ===========================================================================
void AoStorageSetSaveMsgFile(const void* file)
{
	g_ao_storage_save_msg_file = file;
}

// ===========================================================================
//	AoStorageGetSaveMsgFile
/*!
	セーブデータメッセージファイル取得

	@return セーブデータメッセージファイル
*/
// ===========================================================================
const void* AoStorageGetSaveMsgFile(void)
{
	return g_ao_storage_save_msg_file;
}


// ***************************************************************************
// エラー
// ***************************************************************************
// ===========================================================================
//	AoStorageIsError
/*!
	エラー判定

	@return 真：エラーあり　偽：エラーなし
	@note
	このモジュール内でエラーが発生したかどうかを判定します。\n
	AoStorageClearError関数を呼び出すと、エラーなしの状態となり、
	その後エラーが発生するとTRUEを返すようになります。\n
*/
// ===========================================================================
BOOL AoStorageIsError(void)
{
	if (aoStorageGetGlobal()->error != AOD_STORAGE_ERROR_NONE) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//	AoStorageGetError
/*!
	エラー取得

	@return 真：エラー
	@note
	最後に発生したエラー情報を返します。\n
	AoStorageIsError関数がFALSEを返す状態で呼び出すと
	AOD_STORAGE_ERROR_NONEを返します。\n
*/
// ===========================================================================
AOE_STORAGE_ERROR AoStorageGetError(void)
{
	return aoStorageGetGlobal()->error;
}

// ===========================================================================
//	AoStorageClearError
/*!
	エラークリア
*/
// ===========================================================================
void AoStorageClearError(void)
{
	aoStorageGetGlobal()->error = AOD_STORAGE_ERROR_NONE;
}


// ***************************************************************************
// セーブ
// ***************************************************************************
// ===========================================================================
//	AoStorageSaveStart
/*!
	セーブ開始

	@param data		[in] データバッファ
	@param size		[in] データバッファサイズ
	@param is_first	[in] 真：初回セーブ　偽：上書きセーブ
	@param is_new	[in] 真：ロードなしの初回セーブ　偽：通常セーブ
	@note
	セーブ処理を開始します。\n
	他の処理と同時に行うことはできません。\n
	AoStorageSaveIsFinished関数がFALSEを返す状態で呼び出すことはできません。\n
	即時には終了しないので、
	AoStorageSaveIsFinished関数で完了を判定するようにして下さい。\n
	引数のバッファはセーブが完了するまで破棄しないようにして下さい。\n
*/
// ===========================================================================
void AoStorageSaveStart(void* data, u32 size, BOOL is_first, BOOL is_new)
{
	UNREFERENCED_PARAMETER(is_new);

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// 何かの処理中なら操作エラーとする
	if (glb->state != AOD_STORAGE_STATE_IDLE) {
		aoStorageSetError(AOD_STORAGE_ERROR_OPERATION);
		return;
	}
	amAssert(glb->tcb == NULL);
	amAssert(glb->save_buf == NULL);
	amAssert(glb->save_size == 0);

	// エラークリア
	AoStorageClearError();

	// セーブ中設定
	glb->state = AOD_STORAGE_STATE_SAVING;
	glb->save_success = FALSE;

	// セーブ内容設定
	glb->save_buf = data;
	glb->save_size = size;
	glb->save_first = is_first;

	// 空き容量チェックフラグ用ミューテックス作成
	amMutexCreate(&glb->space_check_mutex);

	// セーブスレッド監視タスク作成
	glb->tcb = amTaskMake(
		aoStorageSaveTaskProcedure,
		aoStorageSaveTaskDestructor, 0, 0, 0, "aoStorage::Save");

	// セーブタスクワーク初期化
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(glb->tcb);
	work->lerror = AOD_STORAGE_LERROR_NONE;
	work->need_block = 0;
	if (is_first) {
		work->space_check = FALSE;
	}
	else {
		work->space_check = TRUE;
	}

	// ゲーム終了を無効化
	amSystemExitEnable(0);

	// セーブスレッド作成
	glb->th_execute = TRUE;
	OSThread* th = OSGetCurrentThread();
	OSPriority prio = OSGetThreadPriority(th);
	prio += 1;
	amThreadCreate(
		&glb->th, (void*)aoStorageSaveThread, NULL,
		(AMD_CORE)0, prio, 0x1000, "aoStorage::Save");

	// セーブタスク起動
	amTaskStart(glb->tcb);
}

// ===========================================================================
//	AoStorageSaveFreeSpaceIsEnough
/*!
	セーブ空き容量チェック済み判定

	@return 真：チェック済み　偽：それ以外
	@note
	AoStorageSaveStart関数で開始したセーブ処理において、
	適切な空き容量チェックを行い、空き容量が十分であることが確認されると
	TRUEを返すようになります。\n
	セーブ中以外に呼び出した場合は常にTRUEを返します。\n
*/
// ===========================================================================
BOOL AoStorageSaveFreeSpaceIsEnough(void)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// セーブ中判定
	if (glb->state != AOD_STORAGE_STATE_SAVING) {
		return TRUE;
	}
	if (glb->tcb == NULL) {
		return TRUE;
	}

	// セーブタスクワーク取得
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(glb->tcb);

	// 判定
	amMutexLock(&glb->space_check_mutex);
	BOOL check = work->space_check;
	amMutexUnlock(&glb->space_check_mutex);

	return check;
}

// ===========================================================================
//	AoStorageSaveIsFinished
/*!
	セーブ完了判定

	@return 真：完了済み　偽：セーブ中
	@note
	AoStorageSaveStart関数で開始したセーブ処理の完了を判定します。\n
	AoStorageSaveStart関数を呼び出していない状態ではTRUEを返します。\n
*/
// ===========================================================================
BOOL AoStorageSaveIsFinished(void)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// セーブ中判定
	if (glb->state == AOD_STORAGE_STATE_SAVING) {
		return FALSE;
	}

	return TRUE;
}

// ===========================================================================
//	AoStorageSaveIsSuccessed
/*!
	セーブ成功判定

	@return 真：成功　偽：失敗
	@note
	直前に行ったセーブ処理が成功したかを判定します。\n
	AoStorageSaveStart関数を呼び出していない状態ではFALSEを返します。\n
	AoStorageSaveIsFinished関数がFALSEを返す状態では
	呼び出すことができません。\n
	この関数がFALSEを返す場合は、
	セーブ中になんらかのエラーが発生したことになるので、
	AoStorageGetError関数でエラーの詳細を取得して下さい。\n
*/
// ===========================================================================
BOOL AoStorageSaveIsSuccessed(void)
{
	amAssert(AoStorageSaveIsFinished());
	return aoStorageGetGlobal()->save_success;
}


// ***************************************************************************
// ロード
// ***************************************************************************
// ===========================================================================
//	AoStorageLoadStart
/*!
	ロード開始

	@param data		[out] データバッファ
	@param size		[in]  データバッファサイズ
	@note
	ロード処理を開始します。\n
	他の処理と同時に行うことはできません。\n
	AoStorageLoadIsFinished関数がFALSEを返す状態で呼び出すことはできません。\n
	即時には終了しないので、
	AoStorageLoadIsFinished関数で完了を判定するようにして下さい。\n
	引数のバッファはロードが完了するまで破棄しないようにして下さい。\n
*/
// ===========================================================================
void AoStorageLoadStart(void* data, u32 size)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// 何かの処理中なら操作エラーとする
	if (glb->state != AOD_STORAGE_STATE_IDLE) {
		aoStorageSetError(AOD_STORAGE_ERROR_OPERATION);
		return;
	}
	amAssert(glb->tcb == NULL);
	amAssert(glb->load_buf == NULL);
	amAssert(glb->load_size == 0);

	// エラークリア
	AoStorageClearError();

	// ロード中設定
	glb->state = AOD_STORAGE_STATE_LOADING;
	glb->load_success = FALSE;

	// ロード内容設定
	glb->load_buf = data;
	glb->load_size = size;

	// ロードスレッド監視タスク作成
	glb->tcb = amTaskMake(
		aoStorageLoadTaskProcedure, NULL, 0, 0, 0, "aoStorage::Load");

	// ロードタスクワーク初期化
	AOS_STORAGE_LOAD* work = (AOS_STORAGE_LOAD*)amTaskGetWork(glb->tcb);
	work->lerror = AOD_STORAGE_LERROR_NONE;
	work->remake_banner = FALSE;

	// ゲーム終了を無効化
	amSystemExitEnable(0);

	// ロードスレッド作成
	glb->th_execute = TRUE;
	OSThread* th = OSGetCurrentThread();
	OSPriority prio = OSGetThreadPriority(th);
	prio += 1;
	amThreadCreate(
		&glb->th, (void*)aoStorageLoadThread, NULL,
		(AMD_CORE)0, prio, 0x1000, "aoStorage::Load");

	// ロードタスク起動
	amTaskStart(glb->tcb);
}

// ===========================================================================
//	AoStorageLoadIsFinished
/*!
	ロード完了判定

	@return 真：完了済み　偽：ロード中
	@note
	AoStorageLoadStart関数で開始したロード処理の完了を判定します。\n
	AoStorageLoadStart関数を呼び出していない状態ではTRUEを返します。\n
*/
// ===========================================================================
BOOL AoStorageLoadIsFinished(void)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// ロード中判定
	if (glb->state == AOD_STORAGE_STATE_LOADING) {
		return FALSE;
	}

	return TRUE;
}

// ===========================================================================
//	AoStorageLoadIsSuccessed
/*!
	ロード成功判定

	@return 真：成功　偽：失敗
	@note
	直前に行ったロード処理が成功したかを判定します。\n
	AoStorageLoadStart関数を呼び出していない状態ではFALSEを返します。\n
	AoStorageLoadIsFinished関数がFALSEを返す状態では
	呼び出すことができません。\n
	この関数がFALSEを返す場合は、
	ロード中になんらかのエラーが発生したことになるので、
	AoStorageGetError関数でエラーの詳細を取得して下さい。\n
*/
// ===========================================================================
BOOL AoStorageLoadIsSuccessed(void)
{
	amAssert(AoStorageLoadIsFinished());
	return aoStorageGetGlobal()->load_success;
}

// ===========================================================================
//	AoStorageLoadIsCreaterOwn
/*!
	読み込んだセーブデータの作者が自分自身か判定

	@return 真：自分自身　偽：それ以外
*/
// ===========================================================================
BOOL AoStorageLoadIsCreaterOwn(void)
{
	// 未作成
	return TRUE;
}


// ***************************************************************************
// 削除
// ***************************************************************************
// ===========================================================================
//	AoStorageDeleteStart
/*!
	削除開始

	@note
	削除処理を開始します。\n
	他の処理と同時に行うことはできません。\n
	AoStorageDeleteIsFinished関数がFALSEを返す状態で
	呼び出すことはできません。\n
	即時には終了しないので、
	AoStorageDeleteIsFinished関数で完了を判定するようにして下さい。\n
*/
// ===========================================================================
void AoStorageDeleteStart(void)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// 何かの処理中なら操作エラーとする
	if (glb->state != AOD_STORAGE_STATE_IDLE) {
		aoStorageSetError(AOD_STORAGE_ERROR_OPERATION);
		return;
	}
	amAssert(glb->tcb == NULL);

	// エラークリア
	AoStorageClearError();

	// 削除中設定
	glb->state = AOD_STORAGE_STATE_DELETE;
	glb->del_success = FALSE;

	// 削除スレッド監視タスク作成
	glb->tcb = amTaskMake(
		aoStorageDeleteTaskProcedure, NULL, 0, 0, 0, "aoStorage::Delete");

	// タスクワーク初期化
	AOS_STORAGE_LOAD* work = (AOS_STORAGE_LOAD*)amTaskGetWork(glb->tcb);
	work->lerror = AOD_STORAGE_LERROR_NONE;
	work->remake_banner = FALSE;

	// ゲーム終了を無効化
	amSystemExitEnable(0);

	// 削除スレッド作成
	glb->th_execute = TRUE;
	OSThread* th = OSGetCurrentThread();
	OSPriority prio = OSGetThreadPriority(th);
	prio += 1;
	amThreadCreate(
		&glb->th, (void*)aoStorageSaveDeleteThread, NULL,
		(AMD_CORE)0, prio, 0x1000, "aoStorage::Delete");

	// 削除タスク起動
	amTaskStart(glb->tcb);
}

// ===========================================================================
//	AoStorageDeleteIsFinished
/*!
	削除完了判定

	@return 真：完了済み　偽：削除中
	@note
	AoStorageDeleteStart関数で開始したセーブ処理の完了を判定します。\n
	AoStorageDeleteStart関数を呼び出していない状態ではTRUEを返します。\n
*/
// ===========================================================================
BOOL AoStorageDeleteIsFinished(void)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// 削除中判定
	if (glb->state == AOD_STORAGE_STATE_DELETE) {
		return FALSE;
	}

	return TRUE;
}

// ===========================================================================
//	AoStorageDeleteIsSuccessed
/*!
	削除成功判定

	@return 真：成功　偽：失敗
	@note
	直前に行った削除処理が成功したかを判定します。\n
	AoStorageDeleteStart関数を呼び出していない状態ではFALSEを返します。\n
	AoStorageDeleteIsFinished関数がFALSEを返す状態では
	呼び出すことができません。\n
	この関数がFALSEを返す場合は、
	削除中になんらかのエラーが発生したことになるので、
	AoStorageGetError関数でエラーの詳細を取得して下さい。\n
*/
// ===========================================================================
BOOL AoStorageDeleteIsSuccessed(void)
{
	amAssert(AoStorageDeleteIsFinished());
	return aoStorageGetGlobal()->del_success;
}


// ***************************************************************************
// Xbox360専用
// ***************************************************************************
// ===========================================================================
//	AoStorageIsExecuteReal
/*!
	セーブ&ロード処理が実際に行われているか判定

	@return 真：実際に行われている　偽：それ以外
*/
// ===========================================================================
BOOL AoStorageIsExecuteReal(void)
{
	return TRUE;
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ***************************************************************************
// セーブ
// ***************************************************************************
// ===========================================================================
//! セーブスレッド
// ===========================================================================
void aoStorageSaveThread(void)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// スレッド開始
	amThreadOpen(&glb->th);

	// セーブタスクワーク取得
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(glb->tcb);
	work->lerror = AOD_STORAGE_LERROR_NONE;

	char path[64];
	NANDFileInfo finfo;
	s32 result;

	BOOL exist_banner;
	BOOL exist_data;

	u8* real_buf = NULL;
	u32 real_size = 0;
	u32 one_size = 0;
	u32 crc32;

	NANDBanner* banner = NULL;

	// セーブデータのCRC算出
	crc32 = OSCalcCRC32(glb->save_buf, glb->save_size);

	// 実セーブサイズとバッファを作成
	one_size = (u32)(sizeof(AOS_STORAGE_HEADER) + glb->save_size);
	one_size = (u32)(((one_size + 31) / 32) * 32);
	real_size = one_size * AOD_STORAGE_MULTIPLE;
	real_buf = (u8*)amMemAllocTemp(real_size);
	amZeroMemory(real_buf, real_size);
	for (u32 i = 0; i < AOD_STORAGE_MULTIPLE; ++i) {
		AOS_STORAGE_HEADER* head =
			(AOS_STORAGE_HEADER*)((u32)real_buf + (one_size * i));
		head->masic = AOD_STORAGE_MASIC;
		head->file_size = one_size;
		head->data_size = glb->save_size;
		amCopyMemory(head + 1, glb->save_buf, glb->save_size);
		head->crc32 = crc32;
	}

	// ファイルのファイルブロックサイズ算出
	u32 data_fb_size = aoStorageConvByteToFblock(real_size);
	u32 banner_fb_size = aoStorageConvByteToFblock(aoStorageGetBannerSize());

	amSystemLog(
		"save data size : %dB(%dFBlock)\n", real_size, data_fb_size);
	amSystemLog(
		"save banner size : %dB(%dFBlock)\n",
		aoStorageGetBannerSize(), banner_fb_size);
	amSystemLog(
		"save size : %dBlock\n",
		aoStorageConvFblockToBlock(data_fb_size + banner_fb_size));

	// 初回セーブならば空き容量チェックと保存が必要なファイルのチェック
	if (glb->save_first) {

		// 必要ファイルブロック数とiノード数算出
		u32 need_fblock = 0;
		u32 need_inode = 0;

		// バナーファイル存在チェック
		aoStorageMakeBannerFileName(path);
		result = NANDOpen(path, &finfo, NAND_ACCESS_READ);
		if (result == NAND_RESULT_NOEXISTS) {
			// 存在しない
			exist_banner = FALSE;
			need_fblock += banner_fb_size;
			need_inode += 1;
		}
		else if (result == NAND_RESULT_OK) {
			exist_banner = TRUE;

			u32 length;
			result = NANDGetLength(&finfo, &length);
			if (result != NAND_RESULT_OK) {
				NANDClose(&finfo);
				work->lerror = aoStorageGetLocalError(result);
				goto end;
			}

			// サイズチェック
			if (length != aoStorageGetBannerSize()) {
				NANDClose(&finfo);
				// データ破損扱い
				work->lerror = AOD_STORAGE_LERROR_NAND_07;
				goto end;
			}

			result = NANDClose(&finfo);
			if (result != NAND_RESULT_OK) {
				work->lerror = aoStorageGetLocalError(result);
				goto end;
			}
		}
		else {
			// エラー
			work->lerror = aoStorageGetLocalError(result);
			goto end;
		}

		// スレッド終了判定
		if (amThreadCheckExit(&glb->th)) {
			aoStorageSetError(AOD_STORAGE_ERROR_APP_CANCEL);
			goto end;
		}

		// データファイル存在チェック
		if (!exist_banner) {
			// バナーがないならデータファイルもないものとして扱う
			exist_data = FALSE;
			need_fblock += data_fb_size;
			need_inode += 1;
		}
		else {
			aoStorageMakeDataFileName(path);
			result = NANDOpen(path, &finfo, NAND_ACCESS_READ);
			if (result == NAND_RESULT_NOEXISTS) {
				// 存在しない
				exist_data = FALSE;
				need_fblock += data_fb_size;
				need_inode += 1;
			}
			else if (result == NAND_RESULT_OK) {
				exist_data = TRUE;

				u32 length;
				result = NANDGetLength(&finfo, &length);
				if (result != NAND_RESULT_OK) {
					NANDClose(&finfo);
					work->lerror = aoStorageGetLocalError(result);
					goto end;
				}

				// サイズチェック
				if (length != real_size) {
					NANDClose(&finfo);
					// データ破損扱い
					work->lerror = AOD_STORAGE_LERROR_NAND_07;
					goto end;
				}

				result = NANDClose(&finfo);
				if (result != NAND_RESULT_OK) {
					work->lerror = aoStorageGetLocalError(result);
					goto end;
				}
			}
			else {
				// エラー
				work->lerror = aoStorageGetLocalError(result);
				goto end;
			}
		}

		// スレッド終了判定
		if (amThreadCheckExit(&glb->th)) {
			aoStorageSetError(AOD_STORAGE_ERROR_APP_CANCEL);
			goto end;
		}

		// 空き容量チェック
		if (need_fblock > 0) {
			u32 answer;
			result = NANDCheck(need_fblock, need_inode, &answer);
			if (result != NAND_RESULT_OK) {
				work->lerror = aoStorageGetLocalError(result);
				goto end;
			}
			if (answer != 0) {
				// 空き容量が足りない
				if (answer &
					(NAND_CHECK_HOME_INSSPACE | NAND_CHECK_SYS_INSSPACE))
				{
					// 空き容量不足
					work->lerror = AOD_STORAGE_LERROR_NAND_02;
					work->need_block = aoStorageConvFblockToBlock(need_fblock);
				}
				else {
					// 空きファイル数不足
					work->lerror = AOD_STORAGE_LERROR_NAND_03;
					work->need_block = 0;
				}
				goto end;
			}
		}

		// スレッド終了判定
		if (amThreadCheckExit(&glb->th)) {
			aoStorageSetError(AOD_STORAGE_ERROR_APP_CANCEL);
			goto end;
		}
	}
	else {
		exist_banner = TRUE;
		exist_data = TRUE;
	}

	// 空き容量チェックOK
	amMutexLock(&glb->space_check_mutex);
	work->space_check = TRUE;
	amMutexUnlock(&glb->space_check_mutex);

	// データ保存
	aoStorageMakeDataFileName(path);

	if (!exist_data) {
		NANDCreate(path, NAND_PERM_OWNER_READ | NAND_PERM_OWNER_WRITE, 0);
	}

	result = NANDOpen(path, &finfo, NAND_ACCESS_WRITE);
	if (result != NAND_RESULT_OK) {
		work->lerror = aoStorageGetLocalError(result);
		goto end;
	}

	result = NANDWrite(&finfo, real_buf, real_size);
	if ((u32)result != real_size) {
		work->lerror = aoStorageGetLocalError(result);
		goto end;
	}

	result = NANDClose(&finfo);
	if (result != NAND_RESULT_OK) {
		work->lerror = aoStorageGetLocalError(result);
		goto end;
	}

	// バナー保存
	if (!exist_banner) {

		banner = (NANDBanner*)amMemAlloc(sizeof(NANDBanner));
		amZeroMemory(banner, sizeof(NANDBanner));

		u32 size = aoStorageMakeBanner(banner);
		aoStorageMakeBannerFileName(path);

		NANDCreate(path, NAND_PERM_OWNER_READ | NAND_PERM_OWNER_WRITE, 0);

		result = NANDOpen(path, &finfo, NAND_ACCESS_WRITE);
		if (result != NAND_RESULT_OK) {
			work->lerror = aoStorageGetLocalError(result);
			goto end;
		}

		result = NANDWrite(&finfo, banner, size);
		if ((u32)result != size) {
			work->lerror = aoStorageGetLocalError(result);
			goto end;
		}

		result = NANDClose(&finfo);
		if (result != NAND_RESULT_OK) {
			work->lerror = aoStorageGetLocalError(result);
			goto end;
		}
	}

	// スレッド終了判定
	if (amThreadCheckExit(&glb->th)) {
		aoStorageSetError(AOD_STORAGE_ERROR_APP_CANCEL);
		goto end;
	}

	glb->save_success = TRUE;

end:

	if (banner) {
		amMemFree(banner);
		banner = NULL;
	}
	if (real_buf) {
		amMemFree(real_buf);
		real_buf = NULL;
	}

	amThreadQuit(&glb->th);
}

// ===========================================================================
//! セーブスレッド監視タスク
// ===========================================================================
void aoStorageSaveTaskProcedure(AMS_TCB* tcb)
{
	// セーブタスクワーク取得
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(tcb);

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// スレッド完了判定
	if (amThreadCheckQuit(&glb->th)) {

		// スレッド破棄
		amThreadDelete(&glb->th);
		glb->th_execute = FALSE;

		// ゲーム終了を有効化
		amSystemExitEnable(1);

		// ローカルエラー処理
		if ((u32)work->lerror < AOD_STORAGE_LERROR_NUM) {
			amTaskSetProcedure(tcb, aoStorageSaveTaskLocalErrorStartMessage);
			return;
		}

		// タスク終了
		amTaskDelete(tcb);
		glb->tcb = NULL;

		// 値初期化
		glb->save_buf = NULL;
		glb->save_size = 0;

		// 完了設定
		glb->state = AOD_STORAGE_STATE_IDLE;
	}
}

// ===========================================================================
//! セーブスレッド監視タスクデストラクタ
// ===========================================================================
void aoStorageSaveTaskDestructor(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// 空き容量チェックフラグ用ミューテックス削除
	amMutexDelete(&aoStorageGetGlobal()->space_check_mutex);
}

// ===========================================================================
//! セーブスレッド後ローカルエラー表示待ち
// ===========================================================================
void aoStorageSaveTaskLocalErrorStartMessage(AMS_TCB* tcb)
{
	// セーブタスクワーク取得
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(tcb);

	// 既存メッセージ終了判定
	if (AoSysMsgIsFinished()) {

		switch (work->lerror) {
		case AOD_STORAGE_LERROR_NAND_02:	// 空き容量不足

			// 空き容量は1ブロックを想定しているので、
			// それ以外になるようならメッセージを変更する必要があります。
			amAssert(work->need_block == 1);
			AoSysMsgStart(
				AOD_SYS_MSG_SAVE_LACK_SPACE, AOD_SYS_MSG_SELECT_OK);
			amTaskSetProcedure(tcb, aoStorageSaveTaskErrorToWiiMenu00);
			break;

		case AOD_STORAGE_LERROR_NAND_03:	// 空きiノード数不足
			AoSysMsgStart(
				AOD_SYS_MSG_SAVE_LACK_FILE, AOD_SYS_MSG_SELECT_OK);
			amTaskSetProcedure(tcb, aoStorageSaveTaskErrorToWiiMenu00);
			break;

		case AOD_STORAGE_LERROR_NAND_07:	// データ破損
			// ここにはこないはず
			// UNKNOWN扱い
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_12);
			amTaskSetProcedure(tcb, aoStorageSaveTaskErrorToStop00);
			break;

		case AOD_STORAGE_LERROR_NAND_08:	// Wii本体保存メモリ破損
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_08);
			amTaskSetProcedure(tcb, aoStorageSaveTaskErrorToStop00);
			break;

		case AOD_STORAGE_LERROR_NAND_11:	// アクセス不能.
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_11);
			amTaskSetProcedure(tcb, aoStorageSaveTaskErrorToStop00);
			break;

		case AOD_STORAGE_LERROR_NAND_12:	// アクセス不能(UNKNOWN)
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_12);
			amTaskSetProcedure(tcb, aoStorageSaveTaskErrorToStop00);
			break;

		default:
			amAssert(0);
			// ここにはこないはず
			// UNKNOWN扱い
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_12);
			amTaskSetProcedure(tcb, aoStorageSaveTaskErrorToStop00);
			break;
		}
	}
}

// ===========================================================================
//! セーブエラー Wiiメニュー移行確認タスク00
// ===========================================================================
void aoStorageSaveTaskErrorToWiiMenu00(AMS_TCB* tcb)
{
	// メッセージ終了判定
	if (AoSysMsgIsFinished()) {

		// 次へ遷移
		amTaskSetProcedure(tcb, aoStorageSaveTaskErrorToWiiMenu01);
	}
}

// ===========================================================================
//! セーブエラー Wiiメニュー移行確認タスク01
// ===========================================================================
void aoStorageSaveTaskErrorToWiiMenu01(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// リセット
	amWiiReqExit(AMD_WII_REQ_MANAGER);
}

// ===========================================================================
//! セーブエラー ゲーム停止
// ===========================================================================
void aoStorageSaveTaskErrorToStop00(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// 停止
}

// ===========================================================================
//! セーブデータ削除スレッド
// ===========================================================================
void aoStorageSaveDeleteThread(void)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// スレッド開始
	amThreadOpen(&glb->th);

	// セーブタスクワーク取得
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(glb->tcb);
	work->lerror = AOD_STORAGE_LERROR_NONE;

	char path[64];
	s32 result;

	// バナー削除
	aoStorageMakeBannerFileName(path);
	result = NANDDelete(path);
	switch (result) {
	case NAND_RESULT_OK:
	case NAND_RESULT_NOEXISTS:
		// 正常
		break;

	default:
		work->lerror = aoStorageGetLocalError(result);
		goto end;
		break;
	}

	// データ削除
	aoStorageMakeDataFileName(path);
	result = NANDDelete(path);
	switch (result) {
	case NAND_RESULT_OK:
	case NAND_RESULT_NOEXISTS:
		// 正常
		break;

	default:
		work->lerror = aoStorageGetLocalError(result);
		goto end;
		break;
	}

end:

	amThreadQuit(&glb->th);
}

// ===========================================================================
//! セーブデータ削除00
// ===========================================================================
void aoStorageSaveTaskDelete00(AMS_TCB* tcb)
{
	// セーブタスクワーク取得
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(tcb);

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// ローカルエラークリア
	work->lerror = AOD_STORAGE_LERROR_NONE;

	// ゲーム終了を無効化
	amSystemExitEnable(0);

	// セーブデータ削除スレッド作成
	glb->th_execute = TRUE;
	OSThread* th = OSGetCurrentThread();
	OSPriority prio = OSGetThreadPriority(th);
	prio += 1;
	amThreadCreate(
		&glb->th, (void*)aoStorageSaveDeleteThread, NULL,
		(AMD_CORE)0, prio, 0x1000, "aoStorage::Delete");

	// 終了待ちへ遷移
	amTaskSetProcedure(tcb, aoStorageSaveTaskDelete01);
}

// ===========================================================================
//! セーブデータ削除01
// ===========================================================================
void aoStorageSaveTaskDelete01(AMS_TCB* tcb)
{
	// セーブタスクワーク取得
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(tcb);

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// スレッド完了判定
	if (amThreadCheckQuit(&glb->th)) {

		// スレッド破棄
		amThreadDelete(&glb->th);
		glb->th_execute = FALSE;

		// ゲーム終了を有効化
		amSystemExitEnable(1);

		// ローカルエラー処理
		if ((u32)work->lerror < AOD_STORAGE_LERROR_NUM) {

			amTaskSetProcedure(tcb, aoStorageSaveTaskDelete02);
			return;
		}

		// ローカルエラークリア
		work->lerror = AOD_STORAGE_LERROR_NONE;

		// ゲーム終了を無効化
		amSystemExitEnable(0);

		// セーブスレッド作成
		glb->th_execute = TRUE;
		OSThread* th = OSGetCurrentThread();
		OSPriority prio = OSGetThreadPriority(th);
		prio += 1;
		amThreadCreate(
			&glb->th, (void*)aoStorageSaveThread, NULL,
			(AMD_CORE)0, prio, 0x1000, "aoStorage::Save");

		// セーブ待ちへ遷移
		amTaskSetProcedure(tcb, aoStorageSaveTaskProcedure);
	}
}

// ===========================================================================
//! セーブデータ削除02
// ===========================================================================
void aoStorageSaveTaskDelete02(AMS_TCB* tcb)
{
	// セーブタスクワーク取得
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(tcb);

	// 既存メッセージ終了判定
	if (AoSysMsgIsFinished()) {

		switch (work->lerror) {
		case AOD_STORAGE_LERROR_NAND_08:	// Wii本体保存メモリ破損
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_08);
			amTaskSetProcedure(tcb, aoStorageSaveTaskErrorToStop00);
			break;

		case AOD_STORAGE_LERROR_NAND_11:	// アクセス不能.
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_11);
			amTaskSetProcedure(tcb, aoStorageSaveTaskErrorToStop00);
			break;

		case AOD_STORAGE_LERROR_NAND_12:	// アクセス不能(UNKNOWN)
		default:
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_12);
			amTaskSetProcedure(tcb, aoStorageSaveTaskErrorToStop00);
			break;
		}
	}
}


// ***************************************************************************
// ロード
// ***************************************************************************
// ===========================================================================
//! ロードスレッド
// ===========================================================================
void aoStorageLoadThread(void)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// スレッド開始
	amThreadOpen(&glb->th);

	// ロードタスクワーク取得
	AOS_STORAGE_LOAD* work = (AOS_STORAGE_LOAD*)amTaskGetWork(glb->tcb);
	work->lerror = AOD_STORAGE_LERROR_NONE;
	work->remake_banner = FALSE;

	char path[64];
	NANDFileInfo finfo;
	u32 size;
	s32 result;

	u8* real_buf = NULL;
	u32 real_size = 0;
	u32 one_size = 0;
	const AOS_STORAGE_HEADER* head1;

#if AOD_STORAGE_MULTIPLE > 1
	const AOS_STORAGE_HEADER* head2;
	u32 crc32;
#endif // AOD_STORAGE_MULTIPLE > 1

	// 実セーブサイズとバッファを作成
	one_size = (u32)(sizeof(AOS_STORAGE_HEADER) + glb->load_size);
	one_size = (u32)(((one_size + 31) / 32) * 32);
	real_size = one_size * AOD_STORAGE_MULTIPLE;
	real_buf = (u8*)amMemAllocTemp(real_size);

	// バナーファイルチェック
	aoStorageMakeBannerFileName(path);
	result = NANDOpen(path, &finfo, NAND_ACCESS_READ);
	if (result != NAND_RESULT_OK) {
		// バナーファイルが無いならセーブデータは無効とする
		aoStorageSetError(AOD_STORAGE_ERROR_LOADDATA_NONE);
		goto end;
	}
	result = NANDGetLength(&finfo, &size);
	if (result != NAND_RESULT_OK) {
		NANDClose(&finfo);

		// 通常のエラーとして扱う
		work->lerror = aoStorageGetLocalError(result);
		goto end;
	}
	if (size != aoStorageGetBannerSize()) {
		// バナーが破損しているので修復が必要
		work->remake_banner = TRUE;
	}
	result = NANDClose(&finfo);
	if (result != NAND_RESULT_OK) {
		// 通常のエラーとして扱う
		work->lerror = aoStorageGetLocalError(result);
		goto end;
	}

	// データファイル読み込み
	aoStorageMakeDataFileName(path);
	result = NANDOpen(path, &finfo, NAND_ACCESS_READ);
	if (result != NAND_RESULT_OK) {
		aoStorageSetError(AOD_STORAGE_ERROR_LOADDATA_NONE);
		goto end;
	}
	result = NANDGetLength(&finfo, &size);
	if (result != NAND_RESULT_OK) {
		NANDClose(&finfo);
		work->lerror = aoStorageGetLocalError(result);
		goto end;
	}
	if (size != real_size) {
		NANDClose(&finfo);
		work->lerror = AOD_STORAGE_LERROR_NAND_07;
		goto end;
	}
	result = NANDRead(&finfo, real_buf, real_size);
	if (result != real_size) {
		NANDClose(&finfo);
		work->lerror = aoStorageGetLocalError(result);
		goto end;
	}
	result = NANDClose(&finfo);
	if (result != NAND_RESULT_OK) {
		work->lerror = aoStorageGetLocalError(result);
		goto end;
	}

	// チェック
#if AOD_STORAGE_MULTIPLE == 1

	head1 = (const AOS_STORAGE_HEADER*)real_buf;
	if ((head1->masic != AOD_STORAGE_MASIC) ||
		(head1->file_size != one_size) ||
		(head1->data_size != glb->load_size) ||
		(head1->crc32 != OSCalcCRC32(head1 + 1, head1->data_size)))
	{
		work->lerror = AOD_STORAGE_LERROR_NAND_07;
		goto end;
	}
	amCopyMemory(glb->load_buf, head1 + 1, glb->load_size);

#else

	work->lerror = AOD_STORAGE_LERROR_NAND_07;
	for (u32 i = 0; i < (u32)(AOD_STORAGE_MULTIPLE - 1); ++i) {
		head1 = (const AOS_STORAGE_HEADER*)((u32)real_buf + (one_size * i));
		crc32 = OSCalcCRC32(head1 + 1, glb->load_size);

		if ((head1->masic != AOD_STORAGE_MASIC) ||
			(head1->file_size != one_size) ||
			(head1->data_size != glb->load_size) ||
			(head1->crc32 != crc32))
		{
			continue;
		}

		u32 j;
		for (j = i + 1; j < AOD_STORAGE_MULTIPLE; ++j) {
			head2 = (const AOS_STORAGE_HEADER*)((u32)head1 + one_size);
			if (memcmp(head1, head2, one_size) == 0) {
				break;
			}
		}
		if (j >= AOD_STORAGE_MULTIPLE) {
			continue;
		}

		work->lerror = AOD_STORAGE_LERROR_NONE;

		amCopyMemory(glb->load_buf, head1 + 1, glb->load_size);
	}
	if (work->lerror < AOD_STORAGE_LERROR_NUM) {
		goto end;
	}

#endif // AOD_STORAGE_MULTIPLE == 1

	glb->load_success = TRUE;

end:

	if (real_buf) {
		amMemFree(real_buf);
		real_buf = NULL;
	}

	amThreadQuit(&glb->th);
}

// ===========================================================================
//! ロードスレッド監視タスク
// ===========================================================================
void aoStorageLoadTaskProcedure(AMS_TCB* tcb)
{
	// セーブタスクワーク取得
	AOS_STORAGE_LOAD* work = (AOS_STORAGE_LOAD*)amTaskGetWork(tcb);

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// スレッド完了判定
	if (amThreadCheckQuit(&glb->th)) {

		// スレッド破棄
		amThreadDelete(&glb->th);
		glb->th_execute = FALSE;

		// ゲーム終了を有効化
		amSystemExitEnable(1);

		// ローカルエラー処理
		if ((u32)work->lerror < AOD_STORAGE_LERROR_NUM) {

			amTaskSetProcedure(tcb, aoStorageLoadTaskLocalErrorStartMessage);
			return;
		}

		// バナー再作成確認
		if (work->remake_banner) {

			glb->load_success = FALSE;

			// ゲーム終了を無効化
			amSystemExitEnable(0);

			// バナー再作成スレッド作成
			glb->th_execute = TRUE;
			OSThread* th = OSGetCurrentThread();
			OSPriority prio = OSGetThreadPriority(th);
			prio += 1;
			amThreadCreate(
				&glb->th, (void*)aoStorageLoadSaveBannerThread, NULL,
				(AMD_CORE)0, prio, 0x1000, "aoStorage::LoadSaveBanner");

			amTaskSetProcedure(tcb, aoStorageLoadSaveBannerTaskProcedure);
			return;
		}

		// タスク終了
		amTaskDelete(tcb);
		glb->tcb = NULL;

		// 値初期化
		glb->load_buf = NULL;
		glb->load_size = 0;

		// 完了設定
		glb->state = AOD_STORAGE_STATE_IDLE;
	}
}

// ===========================================================================
//! バナー再作成スレッド
// ===========================================================================
void aoStorageLoadSaveBannerThread(void)
{
	char path[64];
	NANDFileInfo finfo;
	s32 result;
	NANDBanner* banner = NULL;
	u32 size;

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// スレッド開始
	amThreadOpen(&glb->th);

	// ロードタスクワーク取得
	AOS_STORAGE_LOAD* work = (AOS_STORAGE_LOAD*)amTaskGetWork(glb->tcb);
	work->lerror = AOD_STORAGE_LERROR_NONE;
	work->remake_banner = FALSE;

	// バナーファイル名作成
	aoStorageMakeBannerFileName(path);

	// バナー削除
	result = NANDDelete(path);
	switch (result) {
	case NAND_RESULT_OK:
	case NAND_RESULT_NOEXISTS:
		// 正常
		break;

	default:
		work->lerror = aoStorageGetLocalError(result);
		goto end;
		break;
	}

	// バナー保存
	banner = (NANDBanner*)amMemAlloc(sizeof(NANDBanner));
	amZeroMemory(banner, sizeof(NANDBanner));
	size = aoStorageMakeBanner(banner);

	NANDCreate(path, NAND_PERM_OWNER_READ | NAND_PERM_OWNER_WRITE, 0);

	result = NANDOpen(path, &finfo, NAND_ACCESS_WRITE);
	if (result != NAND_RESULT_OK) {
		work->lerror = aoStorageGetLocalError(result);
		goto end;
	}

	result = NANDWrite(&finfo, banner, size);
	if ((u32)result != size) {
		NANDClose(&finfo);
		if (result == NAND_RESULT_MAXBLOCKS) {
			work->lerror = AOD_STORAGE_LERROR_NAND_02;
		}
		else {
			work->lerror = aoStorageGetLocalError(result);
		}
		goto end;
	}

	result = NANDClose(&finfo);
	if (result != NAND_RESULT_OK) {
		work->lerror = aoStorageGetLocalError(result);
		goto end;
	}

	glb->load_success = TRUE;

end:

	if (banner) {
		amMemFree(banner);
		banner = NULL;
	}

	// スレッド終了
	amThreadQuit(&glb->th);
}

// ===========================================================================
//! バナー再作成スレッド監視タスク
// ===========================================================================
void aoStorageLoadSaveBannerTaskProcedure(AMS_TCB* tcb)
{
	// セーブタスクワーク取得
	AOS_STORAGE_LOAD* work = (AOS_STORAGE_LOAD*)amTaskGetWork(tcb);

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// スレッド完了判定
	if (amThreadCheckQuit(&glb->th)) {

		// スレッド破棄
		amThreadDelete(&glb->th);
		glb->th_execute = FALSE;

		// ゲーム終了を有効化
		amSystemExitEnable(1);

		// ローカルエラー処理
		if ((u32)work->lerror < AOD_STORAGE_LERROR_NUM) {

			amTaskSetProcedure(tcb, aoStorageLoadTaskLocalErrorStartMessage);
			return;
		}

		// タスク終了
		amTaskDelete(tcb);
		glb->tcb = NULL;

		// 値初期化
		glb->load_buf = NULL;
		glb->load_size = 0;

		// 完了設定
		glb->state = AOD_STORAGE_STATE_IDLE;
	}
}

// ===========================================================================
//! ロードスレッド後ローカルエラーメッセージ表示開始待ち
// ===========================================================================
void aoStorageLoadTaskLocalErrorStartMessage(AMS_TCB* tcb)
{
	// セーブタスクワーク取得
	AOS_STORAGE_LOAD* work = (AOS_STORAGE_LOAD*)amTaskGetWork(tcb);

	// 既存メッセージ表示完了判定
	if (AoSysMsgIsFinished()) {
		switch (work->lerror) {
		case AOD_STORAGE_LERROR_NAND_02:	// 空き容量不足
			AoSysMsgStart(
				AOD_SYS_MSG_SAVE_LACK_SPACE, AOD_SYS_MSG_SELECT_OK);
			amTaskSetProcedure(tcb, aoStorageLoadTaskErrorToWiiMenu00);
			break;
		case AOD_STORAGE_LERROR_NAND_03:	// 空きiノード数不足
			AoSysMsgStart(
				AOD_SYS_MSG_SAVE_LACK_FILE, AOD_SYS_MSG_SELECT_OK);
			amTaskSetProcedure(tcb, aoStorageLoadTaskErrorToWiiMenu00);
			break;
		case AOD_STORAGE_LERROR_NAND_07:	// データ破損
			AoSysMsgStart(
				AOD_SYS_MSG_SAVE_DATA_DESTROY, AOD_SYS_MSG_SELECT_OK);
			amTaskSetProcedure(tcb, aoStorageLoadTaskErrorToDelete00);
			break;
		case AOD_STORAGE_LERROR_NAND_08:	// Wii本体保存メモリ破損
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_08);
			amTaskSetProcedure(tcb, aoStorageLoadTaskErrorToStop00);
			break;
		case AOD_STORAGE_LERROR_NAND_11:	// アクセス不能.
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_11);
			amTaskSetProcedure(tcb, aoStorageLoadTaskErrorToStop00);
			break;
		case AOD_STORAGE_LERROR_NAND_12:	// アクセス不能(UNKNOWN)
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_12);
			amTaskSetProcedure(tcb, aoStorageLoadTaskErrorToStop00);
			break;
		default:
			amAssert(0);
			// データ破損と同じ扱いにする？
			AoSysMsgStart(
				AOD_SYS_MSG_SAVE_DATA_DESTROY, AOD_SYS_MSG_SELECT_OK);
			amTaskSetProcedure(tcb, aoStorageLoadTaskErrorToDelete00);
			break;
		}
	}
}

// ===========================================================================
//! ロードエラー ファイル削除確認タスク00
// ===========================================================================
void aoStorageLoadTaskErrorToDelete00(AMS_TCB* tcb)
{
	// メッセージ終了判定
	if (AoSysMsgIsFinished()) {

		// セーブデータ削除確認メッセージ表示
		AoSysMsgStart(
			AOD_SYS_MSG_SAVE_DESTROY_DELETE, AOD_SYS_MSG_SELECT_YESNO);

		amTaskSetProcedure(tcb, aoStorageLoadTaskErrorToDelete01);
	}
}

// ===========================================================================
//! ロードエラー ファイル削除確認タスク01
// ===========================================================================
void aoStorageLoadTaskErrorToDelete01(AMS_TCB* tcb)
{
	// セーブタスクワーク取得
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(tcb);

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// メッセージ終了判定
	if (AoSysMsgIsFinished()) {
		// 結果取得
		AOE_SYS_MSG_RESULT result = AoSysMsgGetResult();
		if (result == AOD_SYS_MSG_RESULT_YES) {
			// セーブデータ削除へ遷移
			amTaskSetProcedure(tcb, aoStorageLoadTaskDelete00);
		}
		else {
			// ロードキャンセル
			aoStorageSetError(AOD_STORAGE_ERROR_CANCEL);

			// タスク終了
			amTaskDelete(tcb);
			glb->tcb = NULL;

			// 値初期化
			glb->load_buf = NULL;
			glb->load_size = 0;

			// 完了設定
			glb->state = AOD_STORAGE_STATE_IDLE;
		}
	}
}

// ===========================================================================
//! ロードエラー Wiiメニュー移行確認タスク00
// ===========================================================================
void aoStorageLoadTaskErrorToWiiMenu00(AMS_TCB* tcb)
{
	// メッセージ終了判定
	if (AoSysMsgIsFinished()) {

		// 次へ遷移
		amTaskSetProcedure(tcb, aoStorageLoadTaskErrorToWiiMenu01);
	}
}

// ===========================================================================
//! ロードエラー Wiiメニュー移行確認タスク01
// ===========================================================================
void aoStorageLoadTaskErrorToWiiMenu01(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// リセット
	amWiiReqExit(AMD_WII_REQ_MANAGER);
}

// ===========================================================================
//! ロードエラー ゲーム停止
// ===========================================================================
void aoStorageLoadTaskErrorToStop00(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// 停止
}

// ===========================================================================
//! セーブデータ削除00
// ===========================================================================
void aoStorageLoadTaskDelete00(AMS_TCB* tcb)
{
	// セーブタスクワーク取得
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(tcb);

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// ローカルエラークリア
	work->lerror = AOD_STORAGE_LERROR_NONE;

	// ゲーム終了を無効化
	amSystemExitEnable(0);

	// セーブデータ削除スレッド作成
	glb->th_execute = TRUE;
	OSThread* th = OSGetCurrentThread();
	OSPriority prio = OSGetThreadPriority(th);
	prio += 1;
	amThreadCreate(
		&glb->th, (void*)aoStorageSaveDeleteThread, NULL,
		(AMD_CORE)0, prio, 0x1000, "aoStorage::Delete");

	// 終了待ちへ遷移
	amTaskSetProcedure(tcb, aoStorageLoadTaskDelete01);
}

// ===========================================================================
//! セーブデータ削除01
// ===========================================================================
void aoStorageLoadTaskDelete01(AMS_TCB* tcb)
{
	// セーブタスクワーク取得
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(tcb);

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// スレッド完了判定
	if (amThreadCheckQuit(&glb->th)) {

		// スレッド破棄
		amThreadDelete(&glb->th);
		glb->th_execute = FALSE;

		// ゲーム終了を有効化
		amSystemExitEnable(1);

		// ローカルエラー処理
		if ((u32)work->lerror < AOD_STORAGE_LERROR_NUM) {

			amTaskSetProcedure(tcb, aoStorageLoadTaskDelete02);
			return;
		}

		// ローカルエラークリア
		work->lerror = AOD_STORAGE_LERROR_NONE;

		// ロードデータなし
		aoStorageSetError(AOD_STORAGE_ERROR_LOADDATA_NONE);

		// タスク終了
		amTaskDelete(tcb);
		glb->tcb = NULL;

		// 値初期化
		glb->load_buf = NULL;
		glb->load_size = 0;

		// 完了設定
		glb->state = AOD_STORAGE_STATE_IDLE;
	}
}

// ===========================================================================
//! セーブデータ削除02
// ===========================================================================
void aoStorageLoadTaskDelete02(AMS_TCB* tcb)
{
	// セーブタスクワーク取得
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(tcb);

	// 既存メッセージ表示完了判定
	if (AoSysMsgIsFinished()) {

		switch (work->lerror) {
		case AOD_STORAGE_LERROR_NAND_08:	// Wii本体保存メモリ破損
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_08);
			amTaskSetProcedure(tcb, aoStorageLoadTaskErrorToStop00);
			break;

		case AOD_STORAGE_LERROR_NAND_11:	// アクセス不能.
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_11);
			amTaskSetProcedure(tcb, aoStorageLoadTaskErrorToStop00);
			break;

		case AOD_STORAGE_LERROR_NAND_12:	// アクセス不能(UNKNOWN)
		default:
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_12);
			amTaskSetProcedure(tcb, aoStorageLoadTaskErrorToStop00);
			break;
		}
	}
}


// ***************************************************************************
// 削除
// ***************************************************************************
// ===========================================================================
//! 削除スレッド監視タスク
// ===========================================================================
void aoStorageDeleteTaskProcedure(AMS_TCB* tcb)
{
	// セーブタスクワーク取得
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(tcb);

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// スレッド完了判定
	if (amThreadCheckQuit(&glb->th)) {

		// スレッド破棄
		amThreadDelete(&glb->th);
		glb->th_execute = FALSE;

		// ゲーム終了を有効化
		amSystemExitEnable(1);

		// ローカルエラー処理
		if ((u32)work->lerror < AOD_STORAGE_LERROR_NUM) {

			amTaskSetProcedure(tcb, aoStorageDeleteTaskLocalErrorStartMessage);
			return;
		}

		// ローカルエラークリア
		work->lerror = AOD_STORAGE_LERROR_NONE;

		// 成功
		glb->del_success = TRUE;

		// タスク終了
		amTaskDelete(tcb);
		glb->tcb = NULL;

		// 完了設定
		glb->state = AOD_STORAGE_STATE_IDLE;
	}
}

// ===========================================================================
//! 削除スレッド後ローカルエラーメッセージ表示待ち
// ===========================================================================
void aoStorageDeleteTaskLocalErrorStartMessage(AMS_TCB* tcb)
{
	// セーブタスクワーク取得
	AOS_STORAGE_SAVE* work = (AOS_STORAGE_SAVE*)amTaskGetWork(tcb);

	// 既存メッセージ終了判定
	if (AoSysMsgIsFinished()) {

		switch (work->lerror) {
		case AOD_STORAGE_LERROR_NAND_08:	// Wii本体保存メモリ破損
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_08);
			amTaskSetProcedure(tcb, aoStorageDeleteTaskErrorToStop00);
			break;

		case AOD_STORAGE_LERROR_NAND_11:	// アクセス不能.
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_11);
			amTaskSetProcedure(tcb, aoStorageDeleteTaskErrorToStop00);
			break;

		case AOD_STORAGE_LERROR_NAND_12:	// アクセス不能(UNKNOWN)
		default:
			AoSysMsgShowFatalError(AOD_SYS_MSG_FATAL_NAND_12);
			amTaskSetProcedure(tcb, aoStorageDeleteTaskErrorToStop00);
			break;
		}
	}
}

// ===========================================================================
//! 削除エラー ゲーム停止
// ===========================================================================
void aoStorageDeleteTaskErrorToStop00(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// 停止
}


// ***************************************************************************
// グローバルデータ
// ***************************************************************************
// ===========================================================================
//! グローバルデータ取得
// ===========================================================================
AOS_STORAGE* aoStorageGetGlobal(void)
{
	return &g_ao_storage;
}

// ===========================================================================
//! エラー設定
// ===========================================================================
void aoStorageSetError(AOE_STORAGE_ERROR error)
{
	// 操作エラーならアサート
	amAssert(error != AOD_STORAGE_ERROR_OPERATION);

	if ((g_ao_storage.error == AOD_STORAGE_ERROR_NONE) ||
		(g_ao_storage.error == AOD_STORAGE_ERROR_OPERATION)) {
		g_ao_storage.error = error;
	}
}

// ***************************************************************************
// その他
// ***************************************************************************
// ===========================================================================
//! バナー作成
// ===========================================================================
u32 aoStorageMakeBanner(NANDBanner* banner)
{
	amAssert(g_ao_storage_save_msg_file);

	// 初期化
	NANDInitBanner(
		banner,
		NAND_BANNER_FLAG_ANIM_LOOP |
		NAND_BANNER_FLAG_NOTCOPY, // WiFi対応なのでコピー禁止
		AoMsgGetStr16(g_ao_storage_save_msg_file, 0, 0),
		AoMsgGetStr16(g_ao_storage_save_msg_file, 1, 0));

	// TPLファイル
	TPLPalettePtr tpl = (TPLPalettePtr)amMemAlloc(g_ao_storage_save_tpl_size);
	memcpy(tpl, g_ao_storage_save_tpl, g_ao_storage_save_tpl_size);
	TPLBind(tpl);

	// バナー
	memcpy(
		banner->bannerTexture,
		TPLGet(tpl, 0)->textureHeader->data, NAND_BANNER_TEXTURE_SIZE);

	// アイコン
	memcpy(
		banner->iconTexture[0],
		TPLGet(tpl, 1)->textureHeader->data, NAND_BANNER_ICON_SIZE);
	memcpy(
		banner->iconTexture[1],
		TPLGet(tpl, 1)->textureHeader->data, NAND_BANNER_ICON_SIZE);
	memcpy(
		banner->iconTexture[2],
		TPLGet(tpl, 2)->textureHeader->data, NAND_BANNER_ICON_SIZE);
	memcpy(
		banner->iconTexture[3],
		TPLGet(tpl, 2)->textureHeader->data, NAND_BANNER_ICON_SIZE);
	NANDSetIconSpeed(banner, 0, NAND_BANNER_ICON_ANIM_SPEED_SLOW);
	NANDSetIconSpeed(banner, 1, NAND_BANNER_ICON_ANIM_SPEED_SLOW);
	NANDSetIconSpeed(banner, 2, NAND_BANNER_ICON_ANIM_SPEED_SLOW);
	NANDSetIconSpeed(banner, 3, NAND_BANNER_ICON_ANIM_SPEED_SLOW);
	NANDSetIconSpeed(banner, 4, NAND_BANNER_ICON_ANIM_SPEED_END);

	amMemFree(tpl);

	// サイズを返す
	return aoStorageGetBannerSize();
}

// ===========================================================================
//! バナーサイズ取得
// ===========================================================================
u32 aoStorageGetBannerSize(void)
{
	return NAND_BANNER_SIZE(4);
}

// ===========================================================================
//! バナーファイル名作成
// ===========================================================================
void aoStorageMakeBannerFileName(char* fname)
{
	strcpy(fname, "banner.bin");
}

// ===========================================================================
//! データファイル名作成
// ===========================================================================
void aoStorageMakeDataFileName(char* fname)
{
	sprintf(fname, "save0000.dat");
}

// ===========================================================================
//! バイト -> ファイルブロック数 への単位変換
// ===========================================================================
u32 aoStorageConvByteToFblock(u32 byte_size)
{
	return (u32)(
		byte_size + (AOD_STORAGE_FBLOCK_SIZE - 1)) / AOD_STORAGE_FBLOCK_SIZE;
}

// ===========================================================================
//! ファイルブロック数 -> ブロック数 への単位変換
// ===========================================================================
u32 aoStorageConvFblockToBlock(u32 fb_size)
{
	fb_size *= AOD_STORAGE_FBLOCK_SIZE;
	return (u32)(
		fb_size + (AOD_STORAGE_BLOCK_SIZE - 1)) / AOD_STORAGE_BLOCK_SIZE;
}

// ===========================================================================
//! 結果からローカルエラー取得
// ===========================================================================
AOE_STORAGE_LERROR aoStorageGetLocalError(s32 result)
{
	AOE_STORAGE_LERROR lerror;

	switch (result) {
	case NAND_RESULT_ALLOC_FAILED:
		lerror = AOD_STORAGE_LERROR_NAND_11;
		break;
	case NAND_RESULT_BUSY:
		lerror = AOD_STORAGE_LERROR_NAND_11;
		break;
	case NAND_RESULT_CORRUPT:
		lerror = AOD_STORAGE_LERROR_NAND_08;
		break;
	case NAND_RESULT_ECC_CRIT:
		lerror = AOD_STORAGE_LERROR_NAND_07;
		break;
	case NAND_RESULT_AUTHENTICATION:
		lerror = AOD_STORAGE_LERROR_NAND_07;
		break;
	case NAND_RESULT_UNKNOWN:
		lerror = AOD_STORAGE_LERROR_NAND_12;
		break;

	case NAND_RESULT_OK:
	case NAND_RESULT_ACCESS:
	case NAND_RESULT_EXISTS:
	case NAND_RESULT_INVALID:
	case NAND_RESULT_MAXBLOCKS:
	case NAND_RESULT_MAXFD:
	case NAND_RESULT_MAXFILES:
	case NAND_RESULT_NOEXISTS:
	case NAND_RESULT_NOTEMPTY:
	case NAND_RESULT_OPENFD:
	case NAND_RESULT_MAXDEPTH:
	case NAND_RESULT_FATAL_ERROR:
	default:
		// アプリケーション側の問題
		amAssert(0);
		lerror = AOD_STORAGE_LERROR_NAND_11;
		break;
	}

	return lerror;
}

#endif // defined(AOD_PLATFORM_WII)

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
