// ===========================================================================
/*!
	@file	aoStorageWin32.cpp
	@brief	AoLibrary セーブ＆ロード管理モジュール定義(Win32)

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"

#if defined(AOD_PLATFORM_WIN32)

// ----- Macros ------------------------------------------------（マクロ定義）
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

	AOD_STORAGE_STATE_NUM,				//!< 状態数
	AOD_STORAGE_STATE_NONE,				//!< 無効コード
} AOE_STORAGE_STATE;

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

	BOOL				load_success;	//!< ロード成功フラグ
	void*				load_buf;		//!< ロードデータバッファ
	u32					load_size;		//!< ロードデータサイズ

	BOOL				th_execute;		//!< スレッド実行中
	AMS_MUTEX			mutex;			//!< ミューテックス
	AMS_THREAD			th;				//!< スレッド

	AMS_TCB*			tcb;			//!< タスクTCBポインタ
} AOS_STORAGE;

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// セーブ
static DWORD WINAPI aoStorageSaveThread(DWORD arg);
static void aoStorageSaveTaskProcedure(AMS_TCB* tcb);

// ロード
static DWORD WINAPI aoStorageLoadThread(DWORD arg);
static void aoStorageLoadTaskProcedure(AMS_TCB* tcb);

// グローバルデータ
static AOS_STORAGE* aoStorageGetGlobal(void);
static void aoStorageSetError(AOE_STORAGE_ERROR error);

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
//! セーブデータメッセージファイル
// ===========================================================================
static const void* g_ao_storage_save_msg_file = NULL;

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
	glb->save_success = FALSE;
	glb->save_buf = NULL;
	glb->save_size = 0;
	glb->load_success = FALSE;
	glb->load_buf = NULL;
	glb->load_size = 0;
	glb->th_execute = FALSE;
	amMutexCreate(&glb->mutex);
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
		}

		// ミューテックス削除
		amMutexDelete(&glb->mutex);

		// グローバルデータ初期化
		glb->initialized = FALSE;
		glb->state = AOD_STORAGE_STATE_IDLE;
		glb->save_success = FALSE;
		glb->save_buf = NULL;
		glb->save_size = 0;
		glb->load_success = FALSE;
		glb->load_buf = NULL;
		glb->load_size = 0;
		glb->th_execute = FALSE;
		glb->tcb = NULL;

		// 未初期化設定
		glb->initialized = FALSE;
	}
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
	UNREFERENCED_PARAMETER(is_first);
	UNREFERENCED_PARAMETER(is_new);

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// 何かの処理中なら操作エラーとする
	if (glb->state != AOD_STORAGE_STATE_IDLE) {
		aoStorageSetError(AOD_STORAGE_ERROR_OPERATION);
		return;
	}
	amAssert(glb->th_execute == FALSE);
	amAssert(glb->tcb == NULL);
	amAssert(glb->save_buf == NULL);
	amAssert(glb->save_size == 0);

	// セーブ中設定
	glb->state = AOD_STORAGE_STATE_SAVING;
	glb->save_success = FALSE;

	// セーブ内容設定
	glb->save_buf = data;
	glb->save_size = size;

	// セーブスレッド作成
	glb->th_execute = TRUE;
	amThreadCreate(
		&glb->th, (void*)aoStorageSaveThread, NULL,
		(AMD_CORE)0, THREAD_PRIORITY_NORMAL, 0x4000, "aoStorage::Save");

	// セーブスレッド開始
	amThreadOpen(&glb->th);

	// セーブスレッド監視タスク作成
	glb->tcb = amTaskMake(
		aoStorageSaveTaskProcedure, NULL, 0, 0, 0, "aoStorage::Save");

	// セーブスレッド監視タスク起動
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
	// empty
	return TRUE;
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
	amAssert(glb->th_execute == FALSE);
	amAssert(glb->tcb == NULL);
	amAssert(glb->load_buf == NULL);
	amAssert(glb->load_size == 0);

	// ロード中設定
	glb->state = AOD_STORAGE_STATE_LOADING;
	glb->load_success = FALSE;

	// ロード内容設定
	glb->load_buf = data;
	glb->load_size = size;

	// ロードスレッド作成
	glb->th_execute = TRUE;
	amThreadCreate(
		&glb->th, (void*)aoStorageLoadThread, NULL,
		(AMD_CORE)0, THREAD_PRIORITY_NORMAL, 0x4000, "aoStorage::Load");

	// ロードスレッド開始
	amThreadOpen(&glb->th);

	// ロードスレッド監視タスク作成
	glb->tcb = amTaskMake(
		aoStorageLoadTaskProcedure, NULL, 0, 0, 0, "aoStorage::Load");

	// ロードスレッド監視タスク起動
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
DWORD WINAPI aoStorageSaveThread(DWORD arg)
{
	UNREFERENCED_PARAMETER(arg);

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// exeファイルのパスからセーブファイル名作成
	char path1[256];
	DWORD len = GetModuleFileNameA(NULL, path1, 256);
	if (len < 4) {
		// エラー
		amAssert(0);
		aoStorageSetError(AOD_STORAGE_ERROR_NO_SAVE);
		amThreadQuit(&glb->th);
		return 0;
	}
	path1[len - 4] = '\0';
	char path2[256];
	sprintf_s(path2, 256, "%s_save.dat", path1);

	// ファイル保存
	FILE* fp = NULL;
	fopen_s(&fp, path2, "wb");
	if (fp == NULL) {
		// エラー
		amAssert(0);
		aoStorageSetError(AOD_STORAGE_ERROR_NO_SAVE);
		amThreadQuit(&glb->th);
		return 0;
	}
	fwrite(glb->save_buf, glb->save_size, 1, fp);
	fclose(fp);
	fp = NULL;

	// 成功
	glb->save_success = TRUE;

	// 終了
	amThreadQuit(&glb->th);

	return 0;
}

// ===========================================================================
//! セーブスレッド監視タスク
// ===========================================================================
void aoStorageSaveTaskProcedure(AMS_TCB* tcb)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// スレッド完了判定
	if (amThreadCheckQuit(&glb->th)) {

		// スレッド破棄
		amThreadDelete(&glb->th);
		glb->th_execute = FALSE;

		// タスク終了
		amTaskDelete(tcb);

		// 値初期化
		glb->save_buf = NULL;
		glb->save_size = 0;

		// 完了設定
		glb->state = AOD_STORAGE_STATE_IDLE;
		glb->tcb = NULL;
	}
}


// ***************************************************************************
// ロード
// ***************************************************************************
// ===========================================================================
//! ロードスレッド
// ===========================================================================
DWORD WINAPI aoStorageLoadThread(DWORD arg)
{
	UNREFERENCED_PARAMETER(arg);

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// exeファイルのパスからセーブファイル名作成
	char path1[256];
	DWORD len = GetModuleFileNameA(NULL, path1, 256);
	if (len < 4) {
		// エラー
		aoStorageSetError(AOD_STORAGE_ERROR_LOADDATA_NONE);
		amThreadQuit(&glb->th);
		return 0;
	}
	path1[len - 4] = '\0';
	char path2[256];
	sprintf_s(path2, 256, "%s_save.dat", path1);

	// ファイルオープン
	FILE* fp = NULL;
	fopen_s(&fp, path2, "rb");
	if (fp == NULL) {
		// エラー
		aoStorageSetError(AOD_STORAGE_ERROR_LOADDATA_NONE);
		amThreadQuit(&glb->th);
		return 0;
	}

	// ファイルサイズチェック
	fseek(fp, 0, SEEK_END);
	fpos_t fpos;
	fgetpos(fp, &fpos);
	fseek(fp, 0, SEEK_SET);
	if ((u32)fpos != glb->load_size) {
		// エラー
		fclose(fp);
		aoStorageSetError(AOD_STORAGE_ERROR_LOADDATA_NONE);
		amThreadQuit(&glb->th);
		return 0;
	}

	// ファイル読み込み
	if (fread(glb->load_buf, glb->load_size, 1, fp) != 1) {
		// エラー
		fclose(fp);
		aoStorageSetError(AOD_STORAGE_ERROR_LOADDATA_NONE);
		amThreadQuit(&glb->th);
		return 0;
	}
	fclose(fp);
	fp = NULL;

	// 成功
	glb->load_success = TRUE;

	// 終了
	amThreadQuit(&glb->th);

	return 0;
}

// ===========================================================================
//! ロードスレッド監視タスク
// ===========================================================================
void aoStorageLoadTaskProcedure(AMS_TCB* tcb)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// スレッド完了判定
	if (amThreadCheckQuit(&glb->th)) {

		// スレッド破棄
		amThreadDelete(&glb->th);

		// タスク終了
		amTaskDelete(tcb);

		// 値初期化
		glb->load_buf = NULL;
		glb->load_size = 0;

		// 完了設定
		glb->state = AOD_STORAGE_STATE_IDLE;
		glb->th_execute = FALSE;
		glb->tcb = NULL;
	}
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

#endif // defined(AOD_PLATFORM_WIN32)

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
