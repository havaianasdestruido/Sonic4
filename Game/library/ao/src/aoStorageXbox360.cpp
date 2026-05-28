// ===========================================================================
/*!
	@file	aoStorageXbox360.cpp
	@brief	AoLibrary セーブ＆ロード管理モジュール定義(Xbox360)

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
// ----- Class Definitions -------------------------------------（クラス宣言）

// ===========================================================================
//! ストレージクラス
// ===========================================================================
class CAoStorage :
	public ao::CProc<CAoStorage>,
	public ao::CTask<CAoStorage>,
	public ao::CThread<CAoStorage>,
	public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CAoStorage();

	//! デストラクタ
	virtual ~CAoStorage();

	//! セーブ開始
	void SaveStart(
		void* data, u32 data_size, BOOL is_first, BOOL is_new = FALSE);

	//! ロード開始
	void LoadStart(void* data, u32 data_size);

protected:

	// タスクプロシージャ
	void TaskProcMain();

	// セーブプロシージャ
	void ProcSaveReady();
	void ProcSaveReadyNew();
	void ProcSaveDeviceChangeMessage();
	void ProcSaveSaveCancelMessage();
	void ProcSaveSaveMessage();
	void ProcSaveWaitEnumData();
	void ProcSaveSaveStart();
	void ProcSaveSaveWait();

	// ロードプロシージャ
	void ProcLoadReady();
	void ProcLoadWaitEnumData();
	void ProcLoadLoadStart();
	void ProcLoadLoadWait();
	void ProcLoadSaveMessage();
	void ProcLoadCancelMessage();

	// スレッド
	void ThreadProcEnum();
	void ThreadProcSave();
	void ThreadProcLoad();

	// その他
	void Clear();
	BOOL IsEnd_SignInChangedSave(void);
	BOOL IsEnd_SignInChangedLoad(void);

	enum {
		ENUM_DATA_MAX				= 8,	//!< 列挙データ数

		// 列挙スレッド結果
		ENUM_RESULT_DATA_DISABLE	= 0,	//!< 既存セーブデータなし
		ENUM_RESULT_DATA_ENABLE,			//!< 既存セーブデータあり
		ENUM_RESULT_CANCEL,					//!< ユーザ操作によりキャンセル
		ENUM_RESULT_ERROR,					//!< エラー発生

		// セーブスレッド結果
		SAVE_RESULT_SUCCESS			= 0,	//!< 成功
		SAVE_RESULT_DISCONNECT,				//!< デバイス切断
		SAVE_RESULT_ERROR,					//!< エラー発生

		// ロードスレッド結果
		LOAD_RESULT_SUCCESS			= 0,	//!< 成功
		LOAD_RESULT_ERROR,					//!< エラー発生
	};

	void* m_data_buf;					//!< セーブorロードデータ
	u32 m_data_size;					//!< セーブorロードデータサイズ
	BOOL m_is_first;					//!< 初回セーブフラグ

	u32 m_th_result;					//!< 既存セーブデータ列挙結果

	XCONTENTDEVICEID m_device_id;		//!< デバイスID
	u32 m_free_space;					//!< 空き容量
	HANDLE m_h_enum;					//!< イナムレータハンドル
	XCONTENT_DATA* m_enum_buf;			//!< イナムレートバッファ
	u32 m_enum_buf_size;				//!< イナムレートバッファサイズ

	BOOL m_is_save;						//!< 真：セーブ中　偽：ロード中
};

// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct AOS_STORAGE
// ---------------------------------------------------------------------------
//!	グローバルデータ
// ===========================================================================
typedef struct tag_AOS_STORAGE {
	BOOL				initialized;	//!< 初期化済みフラグ
	AOE_STORAGE_ERROR	error;			//!< エラー
	CAoStorage*			storage;		//!< ストレージクラス
	XCONTENTDEVICEID	device_id;		//!< デバイスID

	u32					data_size;		//!< セーブorロードデータサイズ
	BOOL				save_success;	//!< セーブ成功フラグ
	BOOL				load_success;	//!< ロード成功フラグ
} AOS_STORAGE;

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

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// グローバルデータ
static AOS_STORAGE* aoStorageGetGlobal(void);
static void aoStorageSetError(AOE_STORAGE_ERROR error);
static u32 aoStorageGetSaveDataSize(void);
static void aoStorageSetExecuteFlag(BOOL is_execute);

#if defined(AOD_DEBUG)
// デバッグ
static void aoStorageDebugTaskLoadSave0000(AMS_TCB* tcb);
static void aoStorageDebugTaskLoadSave0100(AMS_TCB* tcb);
static void aoStorageDebugTaskLoadSave0200(AMS_TCB* tcb);
#endif // defined(AOD_DEBUG)

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//	AOS_STORAGE g_ao_storage
// ---------------------------------------------------------------------------
//!	グローバルデータ
// ===========================================================================
static AOS_STORAGE g_ao_storage = {
	FALSE, AOD_STORAGE_ERROR_NONE, NULL, XCONTENTDEVICE_ANY,
};

// ===========================================================================
//! セーブデータメッセージファイル
// ===========================================================================
static const void* g_ao_storage_save_msg_file = NULL;

// ===========================================================================
//! セーブサムネイルPNGファイル
// ===========================================================================
static void* g_ao_storage_save_thumb_png = NULL;

// ===========================================================================
//! セーブサムネイルPNGファイルサイズ
// ===========================================================================
static u32 g_ao_storage_save_thumb_png_size = 0;

// ===========================================================================
//! セーブファイルルート名
// ===========================================================================
static const char* g_ao_storage_save_root = "save";

// ===========================================================================
//! セーブファイル名
// ===========================================================================
static const char* g_ao_storage_save_file_name = "save0000.dat";

// ===========================================================================
//! セーブファイルパス
// ===========================================================================
static const char* g_ao_storage_save_file_path = "save:\\savegame.txt";

// ===========================================================================
//! セーブ/ロード実行中フラグ
// ===========================================================================
static BOOL g_ao_storage_is_execute_real = TRUE;

// ===========================================================================
//! セーブ/ロード実行中フラグ用ミューテックス
// ===========================================================================
static AMS_MUTEX g_ao_storage_is_execute_real_mutex;

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

	// ミューテックス作成
	amMutexCreate(&g_ao_storage_is_execute_real_mutex);
	g_ao_storage_is_execute_real = TRUE;

	// グローバルデータ初期化
	glb->initialized = FALSE;
	glb->error = AOD_STORAGE_ERROR_NONE;
	glb->storage = NULL;
	glb->device_id = XCONTENTDEVICE_ANY;
	glb->data_size = 0;
	glb->save_success = FALSE;
	glb->load_success = FALSE;

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

		// ストレージクラス解放
		if (glb->storage) {
			delete glb->storage;
			glb->storage = NULL;
		}

		// グローバルデータ初期化
		glb->initialized = FALSE;
		glb->error = AOD_STORAGE_ERROR_NONE;
		glb->storage = NULL;
		glb->device_id = XCONTENTDEVICE_ANY;
		glb->data_size = 0;
		glb->save_success = FALSE;
		glb->load_success = FALSE;

		// ミューテックス削除
		amMutexDelete(&g_ao_storage_is_execute_real_mutex);

		// 未初期化設定
		glb->initialized = FALSE;
	}
}

// ===========================================================================
//	AoStorageSetDataXbox360
/*!
	Xbox360用のセーブに必要なデータ設定

	@param thumb_png		[in] サムネイルPNG
	@param thumb_png_size	[in] サムネイルPNGサイズ
*/
// ===========================================================================
void AoStorageSetDataXbox360(void* thumb_png, u32 thumb_png_size)
{
	g_ao_storage_save_thumb_png = thumb_png;
	g_ao_storage_save_thumb_png_size = thumb_png_size;
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
	amAssert(AoStorageSaveIsFinished());

	// エラークリア
	AoStorageClearError();

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// 初期化
	glb->data_size = size;
	glb->save_success = FALSE;
	aoStorageSetExecuteFlag(FALSE);

	// ストレージクラス生成
	glb->storage = new CAoStorage;

	// セーブ開始
	glb->storage->SaveStart(data, size, is_first, is_new);
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
	if (aoStorageGetGlobal()->storage) {
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
	amAssert(AoStorageSaveIsFinished());

	// エラークリア
	AoStorageClearError();

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// 初期化
	glb->data_size = size;
	glb->load_success = FALSE;
	aoStorageSetExecuteFlag(FALSE);

	// ストレージクラス生成
	glb->storage = new CAoStorage;

	// ロード開始
	glb->storage->LoadStart(data, size);
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
	if (aoStorageGetGlobal()->storage) {
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
	amMutexLock(&g_ao_storage_is_execute_real_mutex);
	BOOL ret = g_ao_storage_is_execute_real;
	amMutexUnlock(&g_ao_storage_is_execute_real_mutex);
	return ret;
}

// ===========================================================================
//! 実行中フラグ設定
// ===========================================================================
void aoStorageSetExecuteFlag(BOOL is_execute)
{
	amMutexLock(&g_ao_storage_is_execute_real_mutex);
	g_ao_storage_is_execute_real = is_execute;
	amMutexUnlock(&g_ao_storage_is_execute_real_mutex);
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

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

// ===========================================================================
//! セーブデータサイズ取得
// ===========================================================================
u32 aoStorageGetSaveDataSize(void)
{
	return (u32)XContentCalculateSize(aoStorageGetGlobal()->data_size, 1);
}


// ***************************************************************************
// ストレージクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CAoStorage::CAoStorage()
{
	m_h_enum = NULL;
	m_enum_buf = NULL;
	Clear();

	// ゲーム終了を無効化
	amSystemExitEnable(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CAoStorage::~CAoStorage()
{
	// ゲーム終了を有効化
	amSystemExitEnable(1);

	// グローバルデータ編集
	AOS_STORAGE* glb = aoStorageGetGlobal();
	glb->device_id = m_device_id;
	glb->storage = NULL;

	Clear();
}

// ===========================================================================
//! セーブ開始
// ===========================================================================
void CAoStorage::SaveStart(
	void* data, u32 data_size, BOOL is_first, BOOL is_new)
{
	Clear();

	m_is_save = TRUE;
	m_data_buf = data;
	m_data_size = data_size;
	m_is_first = is_first;

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();
	m_device_id = glb->device_id;

	// タスク作成
	MakeTask(0, "aoStorage::Save");

	// タスクプロシージャ設定
	SetTaskProc(0, &CAoStorage::TaskProcMain);

	// プロシージャ設定
	if (is_new) {
		SetProc(0, &CAoStorage::ProcSaveReadyNew);
	}
	else {
		SetProc(0, &CAoStorage::ProcSaveReady);
	}

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! ロード開始
// ===========================================================================
void CAoStorage::LoadStart(void* data, u32 data_size)
{
	Clear();

	m_is_save = FALSE;
	m_data_buf = data;
	m_data_size = data_size;

	// タスク作成
	MakeTask(0, "aoStorage::Load");

	// タスクプロシージャ設定
	SetTaskProc(0, &CAoStorage::TaskProcMain);

	// プロシージャ設定
	SetProc(0, &CAoStorage::ProcLoadReady);

	// タスク開始
	StartTask(0);
}


// ***************************************************************************
// ストレージクラス タスクプロシージャ
// ***************************************************************************
// ===========================================================================
//! タスクプロシージャ
// ===========================================================================
void CAoStorage::TaskProcMain()
{
	// プロシージャ判定
	if (IsProcNone(0)) {
		// 終了
		DeleteOwnTask();
		delete this;
		aoStorageGetGlobal()->storage = NULL;
	}
	else {
		// 実行
		Call(0);
	}
}


// ***************************************************************************
// ストレージクラス セーブプロシージャ
// ***************************************************************************
// ===========================================================================
//! セーブ 前処理
// ===========================================================================
void CAoStorage::ProcSaveReady()
{
	// サインイン状態変更チェック
	if (IsEnd_SignInChangedSave()) {
		return;
	}

	// デバイス確認
	if (m_device_id == XCONTENTDEVICE_ANY) {
		// 既存メッセージ表示完了判定
		if (AoSysMsgIsFinished()) {

			// デバイス抜き差し確認メッセージ表示
			AoSysMsgStart(
				AOD_SYS_MSG_STORAGE_CHANGED, AOD_SYS_MSG_SELECT_OK);

			// メッセージ待ちへ遷移
			SetOwnProc(&CAoStorage::ProcSaveDeviceChangeMessage);
		}
	}
	else {
		// セーブ処理へ遷移
		SetOwnProc(&CAoStorage::ProcSaveSaveStart);
	}
}

// ===========================================================================
//! セーブ 前処理(NEW指定時)
// ===========================================================================
void CAoStorage::ProcSaveReadyNew()
{
	// サインイン状態変更チェック
	if (IsEnd_SignInChangedSave()) {
		return;
	}

	m_device_id = XCONTENTDEVICE_ANY;

	// 既存セーブデータ列挙スレッド起動
	SetThreadProc(0, &CAoStorage::ThreadProcEnum);
	StartThread(0, (AMD_CORE)0, (u32)THREAD_PRIORITY_BELOW_NORMAL);

	// 既存セーブデータ列挙待ちへ遷移
	SetOwnProc(&CAoStorage::ProcSaveWaitEnumData);
}

// ===========================================================================
//! セーブ デバイス変更通知メッセージ
// ===========================================================================
void CAoStorage::ProcSaveDeviceChangeMessage()
{
	// メッセージ終了待ち
	if (AoSysMsgIsFinished()) {

		// サインイン状態変更チェック
		if (IsEnd_SignInChangedSave()) {
			return;
		}

		// 結果取得
		if (AoSysMsgGetResult() == AOD_SYS_MSG_RESULT_OK) {
			// 既存セーブデータ列挙スレッド起動
			SetThreadProc(0, &CAoStorage::ThreadProcEnum);
			StartThread(0, (AMD_CORE)0, (u32)THREAD_PRIORITY_BELOW_NORMAL);

			// 既存セーブデータ列挙待ちへ遷移
			SetOwnProc(&CAoStorage::ProcSaveWaitEnumData);
		}
		else {
			// セーブ中止確認メッセージ表示
			AoSysMsgStart(
				AOD_SYS_MSG_STORAGE_CANCEL, AOD_SYS_MSG_SELECT_YESNO);

			// メッセージ待ちへ遷移
			SetOwnProc(&CAoStorage::ProcSaveSaveCancelMessage);
		}
	}
}

// ===========================================================================
//! セーブ セーブキャンセルメッセージ
// ===========================================================================
void CAoStorage::ProcSaveSaveCancelMessage()
{
	// メッセージ終了待ち
	if (AoSysMsgIsFinished()) {

		// サインイン状態変更チェック
		if (IsEnd_SignInChangedSave()) {
			return;
		}

		// 結果取得
		if (AoSysMsgGetResult() == AOD_SYS_MSG_RESULT_YES) {
			// セーブなしで終了 -> 継続
			aoStorageSetError(AOD_STORAGE_ERROR_NO_SAVE);
			SetOwnProcNone();
		}
		else {
			if (m_is_first) {
				// セーブなしで終了 -> 戻る
				aoStorageSetError(AOD_STORAGE_ERROR_CANCEL);
				SetOwnProcNone();
			}
			else {
				// デバイス選択確認メッセージ表示
				AoSysMsgStart(
					AOD_SYS_MSG_STORAGE_DEVICE, AOD_SYS_MSG_SELECT_OK);

				// メッセージ待ちへ遷移
				SetOwnProc(&CAoStorage::ProcSaveDeviceChangeMessage);
			}
		}
	}
}

// ===========================================================================
//! セーブ セーブ確認メッセージ
// ===========================================================================
void CAoStorage::ProcSaveSaveMessage()
{
	// メッセージ終了待ち
	if (AoSysMsgIsFinished()) {

		// サインイン状態変更チェック
		if (IsEnd_SignInChangedSave()) {
			return;
		}

		// 結果取得
		if (AoSysMsgGetResult() == AOD_SYS_MSG_RESULT_YES) {

			// セーブ処理開始へ遷移
			SetOwnProc(&CAoStorage::ProcSaveSaveStart);
		}
		else {
			// セーブ中止確認メッセージ表示
			AoSysMsgStart(
				AOD_SYS_MSG_STORAGE_CANCEL, AOD_SYS_MSG_SELECT_YESNO);

			// メッセージ待ちへ遷移
			SetOwnProc(&CAoStorage::ProcSaveSaveCancelMessage);
		}
	}
}

// ===========================================================================
//! セーブ 既存セーブデータ列挙待ち
// ===========================================================================
void CAoStorage::ProcSaveWaitEnumData()
{
	// スレッド終了判定
	if (IsEndThread(0)) {

		// サインイン状態変更チェック
		if (IsEnd_SignInChangedSave()) {
			return;
		}

		// 既存メッセージ表示完了待ち
		if (!AoSysMsgIsFinished()) {
			return;
		}

		// 結果判定
		switch (m_th_result) {
		case CAoStorage::ENUM_RESULT_DATA_DISABLE:
			// 既存データなし

			// 空き容量チェック
			if (m_free_space < aoStorageGetSaveDataSize()) {
				// 空き容量不足メッセージ表示
				AoSysMsgStart(
					AOD_SYS_MSG_STORAGE_NO_SPACE, AOD_SYS_MSG_SELECT_YESNO);

				// メッセージ待ちへ遷移
				SetOwnProc(&CAoStorage::ProcSaveSaveCancelMessage);
				return;
			}

			// 新規セーブ確認メッセージ表示
			AoSysMsgStart(
				AOD_SYS_MSG_IS_NEW_SAVEDATA, AOD_SYS_MSG_SELECT_YESNO);

			// メッセージ待ちへ遷移
			SetOwnProc(&CAoStorage::ProcSaveSaveMessage);
			break;

		case CAoStorage::ENUM_RESULT_DATA_ENABLE:
			// 既存データあり
			// 上書きセーブ確認メッセージ表示
			AoSysMsgStart(
				AOD_SYS_MSG_OVERWRITE_SAVEDATA, AOD_SYS_MSG_SELECT_YESNO);

			// メッセージ待ちへ遷移
			SetOwnProc(&CAoStorage::ProcSaveSaveMessage);
			break;

		default:
			// キャンセル
			// セーブ中止確認メッセージ表示
			AoSysMsgStart(
				AOD_SYS_MSG_STORAGE_CANCEL, AOD_SYS_MSG_SELECT_YESNO);

			// メッセージ待ちへ遷移
			SetOwnProc(&CAoStorage::ProcSaveSaveCancelMessage);
			break;
		}
	}
}

// ===========================================================================
//! セーブ セーブ開始処理
// ===========================================================================
void CAoStorage::ProcSaveSaveStart()
{
	// サインイン状態変更チェック
	if (IsEnd_SignInChangedSave()) {
		return;
	}

	// セーブスレッド起動
	SetThreadProc(0, &CAoStorage::ThreadProcSave);
	StartThread(0, (AMD_CORE)0, (u32)THREAD_PRIORITY_BELOW_NORMAL);

	// セーブ完了待ちへ遷移
	SetOwnProc(&CAoStorage::ProcSaveSaveWait);
}

// ===========================================================================
//! セーブ セーブ完了待ち
// ===========================================================================
void CAoStorage::ProcSaveSaveWait()
{
	// スレッド終了判定
	if (IsEndThread(0)) {

		// サインイン状態変更チェック
		if (IsEnd_SignInChangedSave()) {
			return;
		}

		// 結果判定
		switch (m_th_result) {
		case CAoStorage::SAVE_RESULT_SUCCESS:
			// 正常終了
			aoStorageGetGlobal()->save_success = TRUE;
			SetOwnProcNone();
			break;

		case CAoStorage::SAVE_RESULT_DISCONNECT:
			// デバイス切断

			// 既存メッセージ表示完了判定
			if (AoSysMsgIsFinished()) {

				// デバイス抜き差し確認メッセージ表示
				AoSysMsgStart(
					AOD_SYS_MSG_STORAGE_CHANGED, AOD_SYS_MSG_SELECT_OK);

				// メッセージ待ちへ遷移
				SetOwnProc(&CAoStorage::ProcSaveDeviceChangeMessage);
			}
			break;

		case CAoStorage::SAVE_RESULT_ERROR:
		default:
			// エラー発生

			// 既存メッセージ表示完了待ち
			if (AoSysMsgIsFinished()) {

				// セーブ失敗メッセージ表示
				AoSysMsgStart(
					AOD_SYS_MSG_STORAGE_SAVE_FAILURE,
					AOD_SYS_MSG_SELECT_YESNO);

				// メッセージ待ちへ遷移
				SetOwnProc(&CAoStorage::ProcSaveSaveCancelMessage);
			}
			break;
		}
	}
}


// ***************************************************************************
// ストレージクラス ロードプロシージャ
// ***************************************************************************
// ===========================================================================
//! ロード 前処理
// ===========================================================================
void CAoStorage::ProcLoadReady()
{
	// サインイン状態変更チェック
	if (IsEnd_SignInChangedLoad()) {
		return;
	}

	// 既存セーブデータ列挙スレッド起動
	SetThreadProc(0, &CAoStorage::ThreadProcEnum);
	StartThread(0, (AMD_CORE)0, (u32)THREAD_PRIORITY_BELOW_NORMAL);

	// 既存セーブデータ列挙待ちへ遷移
	SetOwnProc(&CAoStorage::ProcLoadWaitEnumData);
}

// ===========================================================================
//! ロード 既存セーブデータ列挙待ち
// ===========================================================================
void CAoStorage::ProcLoadWaitEnumData()
{
	// スレッド終了判定
	if (IsEndThread(0)) {

		// サインイン状態変更チェック
		if (IsEnd_SignInChangedLoad()) {
			return;
		}

		// 既存セーブデータがあるならロード処理へ遷移
		if (m_th_result == CAoStorage::ENUM_RESULT_DATA_ENABLE) {
			SetOwnProc(&CAoStorage::ProcLoadLoadStart);
			return;
		}

		// 既存メッセージ表示完了待ち
		if (AoSysMsgIsFinished()) {

			// エラーとして結果を設定
			switch (m_th_result) {
			case CAoStorage::ENUM_RESULT_DATA_DISABLE:
				// 新規セーブ確認メッセージ表示
				AoSysMsgStart(
					AOD_SYS_MSG_IS_NEW_SAVEDATA, AOD_SYS_MSG_SELECT_YESNO);

				// メッセージ待ちへ遷移
				SetOwnProc(&CAoStorage::ProcLoadSaveMessage);
				break;

			default:
				// キャンセル確認メッセージ表示
				AoSysMsgStart(
					AOD_SYS_MSG_STORAGE_CANCEL, AOD_SYS_MSG_SELECT_YESNO);

				// メッセージ待ちへ遷移
				SetOwnProc(&CAoStorage::ProcLoadCancelMessage);
				break;
			}
		}
	}
}

// ===========================================================================
//! ロード 開始処理
// ===========================================================================
void CAoStorage::ProcLoadLoadStart()
{
	// サインイン状態変更チェック
	if (IsEnd_SignInChangedLoad()) {
		return;
	}

	// ロードスレッド起動
	SetThreadProc(0, &CAoStorage::ThreadProcLoad);
	StartThread(0, (AMD_CORE)0, (u32)THREAD_PRIORITY_BELOW_NORMAL);

	// ロード完了待ちへ遷移
	SetOwnProc(&CAoStorage::ProcLoadLoadWait);
}

// ===========================================================================
//! ロード 完了待ち
// ===========================================================================
void CAoStorage::ProcLoadLoadWait()
{
	// スレッド終了判定
	if (IsEndThread(0)) {

		// サインイン状態変更チェック
		if (IsEnd_SignInChangedLoad()) {
			return;
		}

		// 結果判定
		switch (m_th_result) {
		case CAoStorage::LOAD_RESULT_SUCCESS:
			// 正常終了
			aoStorageGetGlobal()->load_success = TRUE;
			SetOwnProcNone();
			break;

		case CAoStorage::LOAD_RESULT_ERROR:
		default:

			// 既存メッセージ表示完了待ち
			if (AoSysMsgIsFinished()) {

				// ロード失敗メッセージ表示
				AoSysMsgStart(
					AOD_SYS_MSG_STORAGE_LOAD_FAILURE,
					AOD_SYS_MSG_SELECT_YESNO);

				// メッセージ待ちへ遷移
				SetOwnProc(&CAoStorage::ProcLoadSaveMessage);
			}
			break;
		}
	}
}

// ===========================================================================
//! ロード セーブ確認メッセージ
// ===========================================================================
void CAoStorage::ProcLoadSaveMessage()
{
	// メッセージ終了待ち
	if (AoSysMsgIsFinished()) {

		// サインイン状態変更チェック
		if (IsEnd_SignInChangedLoad()) {
			return;
		}

		// 結果取得
		if (AoSysMsgGetResult() == AOD_SYS_MSG_RESULT_YES) {
			// 新規セーブ
			aoStorageSetError(AOD_STORAGE_ERROR_LOADDATA_NONE);
			SetOwnProcNone();
		}
		else {
			// キャンセル確認メッセージ表示
			AoSysMsgStart(
				AOD_SYS_MSG_STORAGE_CANCEL, AOD_SYS_MSG_SELECT_YESNO);

			// メッセージ待ちへ遷移
			SetOwnProc(&CAoStorage::ProcLoadCancelMessage);
		}
	}
}

// ===========================================================================
//! ロード キャンセル確認メッセージ
// ===========================================================================
void CAoStorage::ProcLoadCancelMessage()
{
	// メッセージ終了待ち
	if (AoSysMsgIsFinished()) {

		// サインイン状態変更チェック
		if (IsEnd_SignInChangedLoad()) {
			return;
		}

		// 結果取得
		if (AoSysMsgGetResult() == AOD_SYS_MSG_RESULT_YES) {
			// キャンセル
			m_device_id = XCONTENTDEVICE_ANY;
			aoStorageSetError(AOD_STORAGE_ERROR_NO_SAVE);
			SetOwnProcNone();
		}
		else {
			// 既存セーブデータ列挙スレッド起動
			SetThreadProc(0, &CAoStorage::ThreadProcEnum);
			StartThread(0, (AMD_CORE)0, (u32)THREAD_PRIORITY_BELOW_NORMAL);

			// 既存セーブデータ列挙待ちへ遷移
			SetOwnProc(&CAoStorage::ProcLoadWaitEnumData);
		}
	}
}


// ***************************************************************************
// スレッド
// ***************************************************************************
// ===========================================================================
//! 列挙スレッド
// ===========================================================================
void CAoStorage::ThreadProcEnum()
{
	DWORD result;
	XOVERLAPPED overlapped;
	amZeroMemory(&overlapped, sizeof(XOVERLAPPED));

	// 値クリア
	m_free_space = 0;
	m_device_id = XCONTENTDEVICE_ANY;

	// カレントのアカウントインデックス取得
	s32 cur_index = AoAccountGetCurrentId();
	if ((cur_index < 0) || !AoAccountIsCurrentSignin()) {
		// エラー
		m_th_result = CAoStorage::ENUM_RESULT_ERROR;
		return;
	}

	// カレントアカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		// エラー
		m_th_result = CAoStorage::ENUM_RESULT_ERROR;
		return;
	}

	// イナムレータ作成
	result = XContentCreateEnumerator(
		(DWORD)cur_index,
		XCONTENTDEVICE_ANY,
		XCONTENTTYPE_SAVEDGAME,
		XCONTENTFLAG_ENUM_EXCLUDECOMMON,
		CAoStorage::ENUM_DATA_MAX,
		&m_enum_buf_size,
		&m_h_enum);
	switch (result) {
	case ERROR_SUCCESS: // 成功
		// empty
		break;

	case ERROR_NO_MORE_FILES: // 既存データなし
		m_th_result = CAoStorage::ENUM_RESULT_DATA_DISABLE;
		return;
		break;

	default: // その他（エラー扱い）
		m_th_result = CAoStorage::ENUM_RESULT_ERROR;
		return;
		break;
	}

	// 列挙バッファ確保
	m_enum_buf = (XCONTENT_DATA*)amMemAlloc(m_enum_buf_size);

	// 列挙開始
	DWORD ret_count;
	result = XEnumerate(
		m_h_enum, m_enum_buf, m_enum_buf_size, &ret_count, NULL);

	// 解放処理
	if (m_h_enum) {
		CloseHandle(m_h_enum);
		m_h_enum = NULL;
	}
	if (m_enum_buf) {
		amMemFree(m_enum_buf);
		m_enum_buf = NULL;
	}
	m_enum_buf_size = 0;

	// UI表示が可能になるまで待機
	while (AoSysIsShowPlatformUI()) {
		Sleep(10);
	}

	// カレントアカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		// エラー
		m_th_result = CAoStorage::ENUM_RESULT_ERROR;
		return;
	}

	// 必要なデータサイズ算出
	ULARGE_INTEGER iBytesRequested = { 0 };
	iBytesRequested.QuadPart = aoStorageGetSaveDataSize();

	// デバイス選択UI表示
	result = XShowDeviceSelectorUI(
		cur_index,
		XCONTENTTYPE_SAVEDGAME,
		XCONTENTFLAG_FORCE_SHOW_UI,
		iBytesRequested,
		&m_device_id,
		&overlapped);
	if (result != ERROR_IO_PENDING) {
		// エラー
		m_th_result = CAoStorage::ENUM_RESULT_ERROR;
		return;
	}

	// UI完了待ち
	while (!XHasOverlappedIoCompleted(&overlapped)) {
		Sleep(10);
	}

	// 結果取得
	result = XGetOverlappedExtendedError(&overlapped);
	if ((result != ERROR_SUCCESS) || (m_device_id == XCONTENTDEVICE_ANY)) {
		// キャンセル
		m_th_result = CAoStorage::ENUM_RESULT_CANCEL;
		return;
	}

	// デバイス空き容量取得
	XDEVICE_DATA ddata;
	if (XContentGetDeviceData(m_device_id, &ddata) != ERROR_SUCCESS) {
		// キャンセル扱い
		m_th_result = CAoStorage::ENUM_RESULT_CANCEL;
		return;
	}
	if (ddata.ulDeviceFreeBytes > (ULONGLONG)((u32)-1)) {
		m_free_space = (u32)-1;
	}
	else {
		m_free_space = (u32)ddata.ulDeviceFreeBytes;
	}

	// イナムレータ作成
	result = XContentCreateEnumerator(
		(DWORD)cur_index,
		m_device_id,
		XCONTENTTYPE_SAVEDGAME,
		XCONTENTFLAG_ENUM_EXCLUDECOMMON,
		CAoStorage::ENUM_DATA_MAX,
		&m_enum_buf_size,
		&m_h_enum);
	switch (result) {
	case ERROR_SUCCESS: // 成功
		// empty
		break;

	case ERROR_NO_MORE_FILES: // 既存データなし
		m_th_result = CAoStorage::ENUM_RESULT_DATA_DISABLE;
		return;
		break;

	default: // その他（エラー扱い）
		m_th_result = CAoStorage::ENUM_RESULT_ERROR;
		return;
		break;
	}

	// 列挙バッファ確保
	m_enum_buf = (XCONTENT_DATA*)amMemAlloc(m_enum_buf_size);

	// 列挙開始
	result = XEnumerate(
		m_h_enum, m_enum_buf, m_enum_buf_size, &ret_count, NULL);
	if (result == ERROR_SUCCESS) {
		// 既存データあり
		m_th_result = CAoStorage::ENUM_RESULT_DATA_ENABLE;
	}
	else {
		// 既存データなし
		m_th_result = CAoStorage::ENUM_RESULT_DATA_DISABLE;
	}

	// 解放処理
	if (m_h_enum) {
		CloseHandle(m_h_enum);
		m_h_enum = NULL;
	}
	if (m_enum_buf) {
		amMemFree(m_enum_buf);
		m_enum_buf = NULL;
	}
	m_enum_buf_size = 0;
}

// ===========================================================================
//! セーブスレッド
// ===========================================================================
void CAoStorage::ThreadProcSave()
{
	// エラーとして結果を設定しておく
	m_th_result = CAoStorage::SAVE_RESULT_ERROR;

	HANDLE hFile = NULL;
	XCONTENT_DATA content;
	DWORD result;
	BOOL wresult;
	DWORD write_size;
	BOOL create_content_flag = FALSE;

	// カレントのアカウントインデックス取得
	s32 cur_index = AoAccountGetCurrentId();
	if ((cur_index < 0) || !AoAccountIsCurrentSignin()) {
		goto end;
	}

	// カレントアカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		goto end;
	}

	// デバイス接続判定
	if (XContentGetDeviceState(m_device_id, NULL) ==
		ERROR_DEVICE_NOT_CONNECTED)
	{
		m_th_result = CAoStorage::SAVE_RESULT_DISCONNECT;
		goto end;
	}

	// 実行中フラグON
	aoStorageSetExecuteFlag(TRUE);

	// コンテント構造体作成
	amZeroMemory(&content, sizeof(XCONTENT_DATA));
	strcpy_s(content.szFileName, g_ao_storage_save_file_name);
	wcscpy_s(
		content.szDisplayName,
		AoMsgGetStr16(g_ao_storage_save_msg_file, 0, 0));
    content.dwContentType = XCONTENTTYPE_SAVEDGAME;
    content.DeviceID = m_device_id;

	// カレントアカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		goto end;
	}

	// コンテンツ作成
	result = XContentCreate(
		(DWORD)cur_index,
		g_ao_storage_save_root,
		&content,
		XCONTENTFLAG_CREATEALWAYS | XCONTENTFLAG_NOPROFILE_TRANSFER,
		NULL,
		NULL,
		NULL);
	if (result != ERROR_SUCCESS) {
		goto end;
	}
	create_content_flag = TRUE;

	// ファイル作成
	hFile = CreateFile(
		g_ao_storage_save_file_path,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		goto end;
	}

	// ファイル書き込み
	wresult = WriteFile(hFile, m_data_buf, m_data_size, &write_size, NULL);
	if (!wresult || (write_size != m_data_size)) {
		goto end;
	}

	// ファイルクローズ
	CloseHandle(hFile);
	hFile = NULL;

	// コンテントクローズ
	result = XContentClose(g_ao_storage_save_root, NULL);
	create_content_flag = FALSE;
	if (result != ERROR_SUCCESS) {
		goto end;
	}

	// コンテント構造体作成
	amZeroMemory(&content, sizeof(XCONTENT_DATA));
	strcpy_s(content.szFileName, g_ao_storage_save_file_name);
    content.dwContentType = XCONTENTTYPE_SAVEDGAME;
    content.DeviceID = m_device_id;

	// サムネイル適用
	result = XContentSetThumbnail(
		(DWORD)cur_index,
		&content,
		(const BYTE*)g_ao_storage_save_thumb_png,
		g_ao_storage_save_thumb_png_size,
		NULL);
	if (result != ERROR_SUCCESS) {
		goto end;
	}

	// 正常終了
	m_th_result = CAoStorage::SAVE_RESULT_SUCCESS;
	goto success;

end:

	// 実行中フラグクリア
	aoStorageSetExecuteFlag(FALSE);

success:

	if (hFile) {
		CloseHandle(hFile);
		hFile = NULL;
	}
	if (create_content_flag) {
		XContentClose(g_ao_storage_save_root, NULL);
		create_content_flag = FALSE;
	}
}

// ===========================================================================
//! ロードスレッド
// ===========================================================================
void CAoStorage::ThreadProcLoad()
{
	// エラーとして結果を設定しておく
	m_th_result = CAoStorage::LOAD_RESULT_ERROR;

	HANDLE hFile = NULL;
	XCONTENT_DATA content;
	DWORD result;
	DWORD fsize;
	BOOL rresult;
	DWORD read_size;
	BOOL create_content_flag = FALSE;
	BOOL is_creater;

	// カレントのアカウントインデックス取得
	s32 cur_index = AoAccountGetCurrentId();
	if ((cur_index < 0) || !AoAccountIsCurrentSignin()) {
		goto end;
	}

	// コンテント構造体作成
	amZeroMemory(&content, sizeof(XCONTENT_DATA));
	strcpy_s(content.szFileName, g_ao_storage_save_file_name);
	wcscpy_s(
		content.szDisplayName,
		AoMsgGetStr16(g_ao_storage_save_msg_file, 0, 0));
    content.dwContentType = XCONTENTTYPE_SAVEDGAME;
    content.DeviceID = m_device_id;

	// カレントアカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		goto end;
	}

	// コンテンツ作成
	result = XContentCreate(
		(DWORD)cur_index,
		g_ao_storage_save_root,
		&content,
		XCONTENTFLAG_OPENEXISTING,
		NULL,
		NULL,
		NULL);
	if (result != ERROR_SUCCESS) {
		goto end;
	}
	create_content_flag = TRUE;

	// カレントアカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		goto end;
	}

	// 実行中フラグON
	aoStorageSetExecuteFlag(TRUE);

	// 所有者判定
	result = XContentGetCreator(
		(DWORD)cur_index,
		&content,
		&is_creater,
		NULL,
		NULL);
	if (result != ERROR_SUCCESS) {
		goto end;
	}
	if (!is_creater) {
		goto end;
	}

	// ファイル作成
	hFile = CreateFile(
		g_ao_storage_save_file_path,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		goto end;
	}

	// ファイルサイズ取得
	fsize = GetFileSize(hFile, NULL);
	if (fsize != m_data_size) {
		goto end;
	}

	// ファイル読み込み
	rresult = ReadFile(hFile, m_data_buf, m_data_size, &read_size, NULL);
	if (!rresult || (read_size != m_data_size)) {
		goto end;
	}

	// ファイルクローズ
	CloseHandle(hFile);
	hFile = NULL;

	// コンテントクローズ
	result = XContentClose(g_ao_storage_save_root, NULL);
	create_content_flag = FALSE;
	if (result != ERROR_SUCCESS) {
		goto end;
	}

	// 正常終了
	m_th_result = CAoStorage::LOAD_RESULT_SUCCESS;
	goto success;

end:

	// 実況中フラグクリア
	aoStorageSetExecuteFlag(FALSE);

success:

	if (hFile) {
		CloseHandle(hFile);
		hFile = NULL;
	}
	if (create_content_flag) {
		XContentClose(g_ao_storage_save_root, NULL);
		create_content_flag = FALSE;
	}
}


// ***************************************************************************
// その他
// ***************************************************************************
// ===========================================================================
//! 内部パラメータの初期化
// ===========================================================================
void CAoStorage::Clear()
{
	DeleteTask(0);
	WaitEndThread(0);
	SetProcNone(0);
	SetTaskProcNone(0);
	SetThreadProcNone(0);

	if (m_h_enum) {
		CloseHandle(m_h_enum);
		m_h_enum = NULL;
	}
	if (m_enum_buf) {
		amMemFree(m_enum_buf);
		m_enum_buf = NULL;
	}

	m_data_buf = NULL;
	m_data_size = 0;
	m_is_first = FALSE;

	m_device_id = XCONTENTDEVICE_ANY;
	m_free_space = 0;
	m_h_enum = NULL;
	m_enum_buf = NULL;
	m_enum_buf_size = 0;
}

// ===========================================================================
//! サインイン状態変更による終了判定(セーブ時)
// ===========================================================================
BOOL CAoStorage::IsEnd_SignInChangedSave(void)
{
	BOOL is_enable = TRUE;

	// カレントアカウントのインデックス取得
	u32 cur_index = (u32)AoAccountGetCurrentId();
	if (cur_index >= 4) {
		is_enable = FALSE;
		goto end;
	}

	// カレントアカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		is_enable = FALSE;
		goto end;
	}

end:

	if (!is_enable) {
		aoStorageSetError(AOD_STORAGE_ERROR_NO_SAVE);
		SetOwnProcNone();
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! サインイン状態変更による終了判定(ロード時)
// ===========================================================================
BOOL CAoStorage::IsEnd_SignInChangedLoad(void)
{
	BOOL is_enable = TRUE;

	// カレントアカウントのインデックス取得
	u32 cur_index = (u32)AoAccountGetCurrentId();
	if (cur_index >= 4) {
		is_enable = FALSE;
		goto end;
	}

	// カレントアカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		is_enable = FALSE;
		goto end;
	}

end:

	if (!is_enable) {
		m_device_id = XCONTENTDEVICE_ANY;
		aoStorageSetError(AOD_STORAGE_ERROR_NO_SAVE);
		SetOwnProcNone();
		return TRUE;
	}
	return FALSE;
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
