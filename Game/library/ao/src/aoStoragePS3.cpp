// ===========================================================================
/*!
	@file	aoStoragePS3.cpp
	@brief	AoLibrary セーブ＆ロード管理モジュール定義(PS3)

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"

#if defined(AOD_PLATFORM_PS3)

#include "sysutil/sysutil_savedata.h"
#include "cell/hash/libmd5.h"

// ----- Macros ------------------------------------------------（マクロ定義）

#define AOD_STORAGE_MASIC	(0x3c998bc1)	//!< セーブデータ識別子

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

// ===========================================================================
//	struct AOE_STORAGE_SFILE
// ---------------------------------------------------------------------------
//!	セーブファイル列挙
// ===========================================================================
typedef enum tag_AOE_STORAGE_SFILE {
	AOD_STORAGE_SFILE_ICON0		= 0,	//!< ICON0.PNG
	AOD_STORAGE_SFILE_PIC1,				//!< PIC1.PNG
	AOD_STORAGE_SFILE_SAVE_DATA,		//!< セーブデータ

	AOD_STORAGE_SFILE_NUM,				//!< セーブファイル数
	AOD_STORAGE_SFILE_NONE,				//!< 無効コード

	// 差分書き込み開始位置
	AOD_STORAGE_SFILE_DF_S = AOD_STORAGE_SFILE_SAVE_DATA,
} AOE_STORAGE_SFILE;

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
	BOOL				save_first;		//!< 初回セーブフラグ
	BOOL				hdd_less;		//!< HDD空き容量不足

	BOOL				load_success;	//!< ロード成功フラグ
	void*				load_buf;		//!< ロードデータバッファ
	u32					load_size;		//!< ロードデータサイズ
	BOOL				load_none;		//!< ロードファイルなしフラグ

	AOE_STORAGE_SFILE	file_index;		//!< 操作ファイルインデックス

	u8*					buf;			//!< 読み書きバッファ
	u32					buf_size;		//!< 読み書きバッファサイズ

	BOOL				th_execute;		//!< スレッド実行中
	AMS_THREAD			th;				//!< スレッド

	AMS_TCB*			tcb;			//!< タスクTCBポインタ

	CellSaveDataSystemFileParam	sys_param;	//!< システム情報
} AOS_STORAGE;

// ===========================================================================
//	struct AOS_STORAGE_HEADER
// ---------------------------------------------------------------------------
//!	セーブデータヘッダ
// ===========================================================================
typedef struct tag_AOS_STORAGE_HEADER {
	u32					masic;			//!< 識別子
	u32					file_size;		//!< セーブファイルサイズ
	u32					data_size;		//!< セーブデータサイズ
	u32					pad1[1];		//!< パディング
	u8					md5[16];		//!< MD5
} AOS_STORAGE_HEADER; // 32 byte

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
static void aoStorageSaveThread(uint64_t arg);
static void aoStorageSaveDeleteThread(uint64_t arg);
static void aoStorageSaveTaskProcedure(AMS_TCB* tcb);
static void aoStorageSaveTaskProcedureM00(AMS_TCB* tcb);
static void aoStorageSaveTaskProcedureD00(AMS_TCB* tcb);
static void aoStorageSaveTaskProcedureD01(AMS_TCB* tcb);
static void aoStorageSaveTaskProcedureD02(AMS_TCB* tcb);
static void aoStorageSaveTaskProcedureD02b(AMS_TCB* tcb);
static void aoStorageSaveTaskProcedureD02a(AMS_TCB* tcb);
static void aoStorageSaveTaskProcedureD03(AMS_TCB* tcb);
static void aoStorageSaveDataStatCallback(
	CellSaveDataCBResult* cbResult,
	CellSaveDataStatGet* get, CellSaveDataStatSet* set);
static void aoStorageSaveDataFileCallback(
	CellSaveDataCBResult* cbResult,
	CellSaveDataFileGet* get, CellSaveDataFileSet* set);

// ロード
static void aoStorageLoadThread(uint64_t arg);
static void aoStorageLoadTaskProcedure(AMS_TCB* tcb);
static void aoStorageLoadTaskStartMessage(AMS_TCB* tcb);
static void aoStorageLoadTaskSaveMessage(AMS_TCB* tcb);
static void aoStorageLoadTaskCancelMessage(AMS_TCB* tcb);
static void aoStorageLoadDataStatCallback(
	CellSaveDataCBResult* cbResult,
	CellSaveDataStatGet* get, CellSaveDataStatSet* set);
static void aoStorageLoadDataFileCallback(
	CellSaveDataCBResult* cbResult,
	CellSaveDataFileGet* get, CellSaveDataFileSet* set);

// グローバルデータ
static AOS_STORAGE* aoStorageGetGlobal(void);
static void aoStorageSetError(AOE_STORAGE_ERROR error);

// その他
static s32 aoStorageUtilConvBtoKB(s32 b_size);

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
//	char* g_ao_storage_dir_name
// ---------------------------------------------------------------------------
//!	ディレクトリ名
// ===========================================================================
static char g_ao_storage_dir_name[CELL_SAVEDATA_DIRNAME_SIZE] = "";

// ===========================================================================
//	char* g_ao_storage_file_icon0_name
// ---------------------------------------------------------------------------
//!	アイコンデータのファイル名
// ===========================================================================
static char* g_ao_storage_file_icon0_name = "ICON0.PNG";

// ===========================================================================
//	char* g_ao_storage_file_pic1_name
// ---------------------------------------------------------------------------
//!	背景データのファイル名
// ===========================================================================
static char* g_ao_storage_file_pic1_name = "PIC1.PNG";

// ===========================================================================
//	char* g_ao_storage_file_name
// ---------------------------------------------------------------------------
//!	セーブデータのファイル名
// ===========================================================================
static char* g_ao_storage_file_name = "SDATA000.DAT";

// ===========================================================================
//	CellSaveDataAutoIndicator g_ao_storage_indicator
// ---------------------------------------------------------------------------
//!	インジケータ
// ===========================================================================
static CellSaveDataAutoIndicator g_ao_storage_indicator = {
	CELL_SAVEDATA_INDICATORPOS_LOWER_RIGHT,
	CELL_SAVEDATA_INDICATORMODE_FIXED,
	NULL,
	0,
	NULL,
	NULL,
};

// ===========================================================================
//	char g_storage_secure_id[]
// ---------------------------------------------------------------------------
//!	セキュアファイルID
// ===========================================================================
static const char g_storage_secure_id[CELL_SAVEDATA_SECUREFILEID_SIZE] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};

// ===========================================================================
//! ICON0.PNG
// ===========================================================================
static u8* g_ao_storage_save_icon0_png = NULL;

// ===========================================================================
//! ICON0.PNGサイズ
// ===========================================================================
static u32 g_ao_storage_save_icon0_png_size = 0;

// ===========================================================================
//! PIC1.PNG
// ===========================================================================
static u8* g_ao_storage_save_pic1_png = NULL;

// ===========================================================================
//! PIC1.PNGサイズ
// ===========================================================================
static u32 g_ao_storage_save_pic1_png_size = 0;

// ===========================================================================
//! セーブデータメッセージファイル
// ===========================================================================
static const void* g_ao_storage_save_msg_file = NULL;

// ===========================================================================
//! フェード要求コールバック
// ===========================================================================
static AOF_STORAGE_FADE_CALLBACK g_ao_storage_fade_callback = NULL;

// ===========================================================================
//! フェード要求コールバック引数
// ===========================================================================
static void* g_ao_storage_fade_callback_arg = NULL;

// ===========================================================================
//! フェード完了判定コールバック
// ===========================================================================
static AOF_STORAGE_FADE_IS_END_CALLBACK
	g_ao_storage_fade_is_end_callback = NULL;

// ===========================================================================
//! フェード完了判定コールバック引数
// ===========================================================================
static void* g_ao_storage_fade_is_end_callback_arg = NULL;

// ===========================================================================
//! フェードイン要求済みフラグ
// ===========================================================================
static BOOL g_ao_storage_fade_in_flag = FALSE;

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
	glb->file_index = AOD_STORAGE_SFILE_NONE;
	glb->buf = NULL;
	glb->buf_size = 0;
	glb->th_execute = FALSE;
	glb->tcb = NULL;

	g_ao_storage_fade_callback = NULL;
	g_ao_storage_fade_callback_arg = NULL;
	g_ao_storage_fade_is_end_callback = NULL;
	g_ao_storage_fade_is_end_callback_arg = NULL;
	g_ao_storage_fade_in_flag = FALSE;

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

		// データバッファ解放
		if (glb->buf) {
			amMemFree(glb->buf);
			glb->buf = NULL;
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
		glb->file_index = AOD_STORAGE_SFILE_NONE;
		glb->buf = NULL;
		glb->buf_size = 0;
		glb->th_execute = FALSE;
		glb->tcb = NULL;

		// 未初期化設定
		glb->initialized = FALSE;
	}
}

// ===========================================================================
//	AoStorageSetTitleIdPS3
/*!
	PS3タイトルID設定

	@param titleid		[in] ICON0.PNGタイトルID
*/
// ===========================================================================
void AoStorageSetTitleIdPS3(const char* titleid)
{
	snprintf(
		g_ao_storage_dir_name, CELL_SAVEDATA_DIRNAME_SIZE, "%s_0", titleid);
	g_ao_storage_dir_name[CELL_SAVEDATA_DIRNAME_SIZE - 1] = '\0';
}

// ===========================================================================
//	AoStorageSetDataPS3
/*!
	PS3用のセーブに必要なデータ設定

	@param icon0		[in] ICON0.PNG
	@param icon0_size	[in] ICON0.PNGのサイズ
	@param pic1			[in] PIC1.PNG
	@param pic1_size	[in] PIC1.PNGのサイズ
*/
// ===========================================================================
void AoStorageSetDataPS3(
	void* icon0, u32 icon0_size, void* pic1, u32 pic1_size)
{
	g_ao_storage_save_icon0_png = (u8*)icon0;
	g_ao_storage_save_icon0_png_size = icon0_size;
	g_ao_storage_save_pic1_png = (u8*)pic1;
	g_ao_storage_save_pic1_png_size = pic1_size;
}

// ===========================================================================
//	AoStorageSetFadeCallbackPS3
/*!
	フェード要求コールバック設定

	@param cb			[in] コールバック関数
	@param arg			[in] コールバック関数呼び出し時の引数
*/
// ===========================================================================
void AoStorageSetFadeCallbackPS3(AOF_STORAGE_FADE_CALLBACK cb, void* arg)
{
	g_ao_storage_fade_callback = cb;
	g_ao_storage_fade_callback_arg = arg;
}

// ===========================================================================
//	AoStorageSetFadeIsEndCallbackPS3
/*!
	フェード完了判定コールバック設定

	@param cb			[in] コールバック関数
	@param arg			[in] コールバック関数呼び出し時の引数
*/
// ===========================================================================
void AoStorageSetFadeIsEndCallbackPS3(
	AOF_STORAGE_FADE_IS_END_CALLBACK cb, void* arg)
{
	g_ao_storage_fade_is_end_callback = cb;
	g_ao_storage_fade_is_end_callback_arg = arg;
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
	amAssert(glb->th_execute == FALSE);
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

	// ゲーム終了を無効化
	amSystemExitEnable(0);

	// セーブスレッド作成
	glb->th_execute = TRUE;
	s32 prio;
	sys_ppu_thread_t id;
	sys_ppu_thread_get_id(&id);
	sys_ppu_thread_get_priority(id, &prio);
	prio += 1;
	amThreadCreate(
		&glb->th, (void*)aoStorageSaveThread, NULL,
		(AMD_CORE)0, prio, 0x4000, "aoStorage::Save");

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

	// エラークリア
	AoStorageClearError();

	// ロード中設定
	glb->state = AOD_STORAGE_STATE_LOADING;
	glb->load_success = FALSE;

	// ロード内容設定
	glb->load_buf = data;
	glb->load_size = size;

	// ゲーム終了を無効化
	amSystemExitEnable(0);

	// ロードスレッド作成
	glb->th_execute = TRUE;
	s32 prio;
	sys_ppu_thread_t id;
	sys_ppu_thread_get_id(&id);
	sys_ppu_thread_get_priority(id, &prio);
	prio += 1;
	amThreadCreate(
		&glb->th, (void*)aoStorageLoadThread, NULL,
		(AMD_CORE)0, prio, 0x4000, "aoStorage::Load");

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
void aoStorageSaveThread(uint64_t arg)
{
	// スレッド開始
	amThreadOpen((AMS_THREAD*)arg);

	int ret;

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();
	glb->hdd_less = FALSE;

	// セーブデータ作成
	amAssert(glb->buf == NULL);
	glb->buf_size = sizeof(AOS_STORAGE_HEADER) + glb->save_size;
	glb->buf_size = ((glb->buf_size + 31) / 32) * 32;
	glb->buf = (u8*)amMemAllocTemp(glb->buf_size);
	amZeroMemory(glb->buf, glb->buf_size);
	AOS_STORAGE_HEADER* head = (AOS_STORAGE_HEADER*)glb->buf;
	head->masic = AOD_STORAGE_MASIC;
	head->file_size = glb->buf_size;
	head->data_size = glb->save_size;
	amCopyMemory(head + 1, glb->save_buf, glb->save_size);
	cellMd5Digest(head + 1, glb->save_size, head->md5);

	// 汎用バッファ作成
	CellSaveDataSetBuf buf;
	buf.dirListMax = 0;
	buf.fileListMax = 16;
	amZeroMemory(buf.reserved, sizeof(buf.reserved));
	buf.bufSize = sizeof(CellSaveDataFileStat) * 16;
	buf.buf = amMemAlloc(buf.bufSize);

	// セーブ開始
	ret = cellSaveDataAutoSave2(
		CELL_SAVEDATA_VERSION_CURRENT,
		g_ao_storage_dir_name,
		CELL_SAVEDATA_ERRDIALOG_ALWAYS,
		&buf,
		aoStorageSaveDataStatCallback,
		aoStorageSaveDataFileCallback,
		SYS_MEMORY_CONTAINER_ID_INVALID,
		glb);

	// バッファ解放
	amMemFree(buf.buf);

	// バッファ解放
	if (glb->buf) {
		amMemFree(glb->buf);
		glb->buf = NULL;
	}
	glb->buf_size = 0;

	if (ret == CELL_SAVEDATA_RET_OK) {
		// 成功
		glb->save_success = TRUE;
	}
	else {
		// 失敗
		glb->save_success = FALSE;
	}

	// 終了
	amThreadQuit(&glb->th);
}

// ===========================================================================
//! 全セーブデータ対象削除スレッド
// ===========================================================================
void aoStorageSaveDeleteThread(uint64_t arg)
{
	// スレッド開始
	amThreadOpen((AMS_THREAD*)arg);

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// 全セーブデータ対象削除実行
	cellSaveDataDelete2(SYS_MEMORY_CONTAINER_ID_INVALID);

	// 終了
	amThreadQuit(&glb->th);
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

		// フェード要求コールバック呼び出し
		if (g_ao_storage_fade_in_flag && g_ao_storage_fade_callback) {
			g_ao_storage_fade_callback(
				TRUE, g_ao_storage_fade_callback_arg);
			g_ao_storage_fade_in_flag = FALSE;
		}

		// ゲーム終了を有効化
		amSystemExitEnable(1);

		// 正常なら終了
		if (glb->save_success) {

			// タスク終了
			amTaskDelete(tcb);

			// 値初期化
			glb->save_buf = NULL;
			glb->save_size = 0;

			// 完了設定
			glb->state = AOD_STORAGE_STATE_IDLE;
			glb->tcb = NULL;
		}
		else {

			// メッセージ表示待ち
			amTaskSetProcedure(tcb, aoStorageSaveTaskProcedureM00);
		}
	}
}

// ===========================================================================
//! セーブスレッド終了後メッセージ表示待ちタスク
// ===========================================================================
void aoStorageSaveTaskProcedureM00(AMS_TCB* tcb)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// 既存メッセージ終了判定
	if (AoSysMsgIsFinished()) {

		// 空き容量不足判定
		if (glb->hdd_less) {

			// 全セーブデータ対象削除を行うか確認
			AoSysMsgStart(
				AOD_SYS_MSG_ALL_SAVE_DELETE, AOD_SYS_MSG_SELECT_YESNO);

			// 待ち
			amTaskSetProcedure(tcb, aoStorageSaveTaskProcedureD00);
		}
		else {
			// ここに来るのはプラットフォーム側のエラーが発生したとき

			// セーブ中止確認メッセージ表示
			AoSysMsgStart(
				AOD_SYS_MSG_STORAGE_SAVE_FAILURE, AOD_SYS_MSG_SELECT_YESNO);

			// 待ち
			amTaskSetProcedure(tcb, aoStorageSaveTaskProcedureD03);
		}
	}
}

// ===========================================================================
//! 全セーブデータ対象削除実行確認待ちタスク
// ===========================================================================
void aoStorageSaveTaskProcedureD00(AMS_TCB* tcb)
{
	// 終了待ち
	if (AoSysMsgIsFinished()) {
		if (AoSysMsgGetResult() == AOD_SYS_MSG_RESULT_YES) {

			// フェード要求コールバック呼び出し
			if (!g_ao_storage_fade_in_flag && g_ao_storage_fade_callback) {
				g_ao_storage_fade_callback(
					FALSE, g_ao_storage_fade_callback_arg);
				g_ao_storage_fade_in_flag = TRUE;
			}

			// フェード待ちへ遷移
			amTaskSetProcedure(tcb, aoStorageSaveTaskProcedureD02b);
		}
		else {

			// セーブキャンセル確認メッセージ表示
			AoSysMsgStart(AOD_SYS_MSG_SAVE_CANCEL, AOD_SYS_MSG_SELECT_YESNO);

			// メッセージ待ちへ遷移
			amTaskSetProcedure(tcb, aoStorageSaveTaskProcedureD01);
		}
	}
}

// ===========================================================================
//! キャンセル確認メッセージ
// ===========================================================================
void aoStorageSaveTaskProcedureD01(AMS_TCB* tcb)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// 終了待ち
	if (AoSysMsgIsFinished()) {

		// 結果判定
		if (AoSysMsgGetResult() == AOD_SYS_MSG_RESULT_YES) {
			// セーブ中止
			aoStorageSetError(AOD_STORAGE_ERROR_NO_SAVE);
		}
		else {
			if (glb->save_first) {
				// セーブキャンセル
				aoStorageSetError(AOD_STORAGE_ERROR_CANCEL);
			}
			else {
				// 全セーブデータ対象削除を行うか確認
				AoSysMsgStart(
					AOD_SYS_MSG_ALL_SAVE_DELETE, AOD_SYS_MSG_SELECT_YESNO);

				// 待ち
				amTaskSetProcedure(tcb, aoStorageSaveTaskProcedureD00);
				return;
			}
		}

		// タスク終了
		amTaskDelete(tcb);

		// 値初期化
		glb->save_buf = NULL;
		glb->save_size = 0;

		// 完了設定
		glb->state = AOD_STORAGE_STATE_IDLE;
		glb->th_execute = FALSE;
		glb->tcb = NULL;
	}
}

// ===========================================================================
//! 全セーブデータ対象削除スレッド終了待ちタスク
// ===========================================================================
void aoStorageSaveTaskProcedureD02(AMS_TCB* tcb)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// スレッド完了判定
	if (amThreadCheckQuit(&glb->th)) {

		// スレッド破棄
		amThreadDelete(&glb->th);
		glb->th_execute = FALSE;

		// フェード要求コールバック呼び出し
		if (g_ao_storage_fade_in_flag && g_ao_storage_fade_callback) {
			g_ao_storage_fade_callback(
				TRUE, g_ao_storage_fade_callback_arg);
			g_ao_storage_fade_in_flag = FALSE;
		}

		// ゲーム終了を有効化
		amSystemExitEnable(1);

		// フェード待ちへ遷移
		amTaskSetProcedure(tcb, aoStorageSaveTaskProcedureD02a);
	}
}

// ===========================================================================
//! 全セーブデータ対象削除スレッド終了待ちタスク前処理
// ===========================================================================
void aoStorageSaveTaskProcedureD02b(AMS_TCB* tcb)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	BOOL is_end = TRUE;
	if (g_ao_storage_fade_in_flag && g_ao_storage_fade_is_end_callback) {
		if (!g_ao_storage_fade_is_end_callback(
			FALSE, g_ao_storage_fade_is_end_callback_arg))
		{
			is_end = FALSE;
		}
	}
	if (is_end) {
		// ゲーム終了を無効化
		amSystemExitEnable(0);

		// 全セーブデータ対象削除スレッド開始
		glb->th_execute = TRUE;
		s32 prio;
		sys_ppu_thread_t id;
		sys_ppu_thread_get_id(&id);
		sys_ppu_thread_get_priority(id, &prio);
		prio += 1;
		amThreadCreate(
			&glb->th, (void*)aoStorageSaveDeleteThread, NULL,
			(AMD_CORE)0, prio, 0x4000, "aoStorage::Delete");

		// スレッド待ちへ遷移
		amTaskSetProcedure(tcb, aoStorageSaveTaskProcedureD02);
	}
}

// ===========================================================================
//! 全セーブデータ対象削除スレッド終了待ちタスク後処理
// ===========================================================================
void aoStorageSaveTaskProcedureD02a(AMS_TCB* tcb)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	BOOL is_end = TRUE;
	if (g_ao_storage_fade_is_end_callback) {
		if (!g_ao_storage_fade_is_end_callback(
			TRUE, g_ao_storage_fade_is_end_callback_arg))
		{
			is_end = FALSE;
		}
	}
	if (is_end) {
		// ゲーム終了を無効化
		amSystemExitEnable(0);

		// セーブスレッド作成
		glb->th_execute = TRUE;
		s32 prio;
		sys_ppu_thread_t id;
		sys_ppu_thread_get_id(&id);
		sys_ppu_thread_get_priority(id, &prio);
		prio += 1;
		amThreadCreate(
			&glb->th, (void*)aoStorageSaveThread, NULL,
			(AMD_CORE)0, prio, 0x4000, "aoStorage::Save");

		// スレッド待ちへ遷移
		amTaskSetProcedure(tcb, aoStorageSaveTaskProcedure);
	}
}

// ===========================================================================
//! セーブ中止確認メッセージ
// ===========================================================================
void aoStorageSaveTaskProcedureD03(AMS_TCB* tcb)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// 終了待ち
	if (AoSysMsgIsFinished()) {
		if (AoSysMsgGetResult() == AOD_SYS_MSG_RESULT_YES) {
			// 中止
			aoStorageSetError(AOD_STORAGE_ERROR_NO_SAVE);

			// タスク終了
			amTaskDelete(tcb);

			// 値初期化
			glb->save_buf = NULL;
			glb->save_size = 0;

			// 完了設定
			glb->state = AOD_STORAGE_STATE_IDLE;
			glb->th_execute = FALSE;
			glb->tcb = NULL;
		}
		else {
			// 再試行

			// ゲーム終了を無効化
			amSystemExitEnable(0);

			// セーブスレッド作成
			glb->th_execute = TRUE;
			s32 prio;
			sys_ppu_thread_t id;
			sys_ppu_thread_get_id(&id);
			sys_ppu_thread_get_priority(id, &prio);
			prio += 1;
			amThreadCreate(
				&glb->th, (void*)aoStorageSaveThread, NULL,
				(AMD_CORE)0, prio, 0x4000, "aoStorage::Save");

			// スレッド待ちへ遷移
			amTaskSetProcedure(tcb, aoStorageSaveTaskProcedure);
		}
	}
}

// ===========================================================================
//! セーブデータステータスコールバック
// ===========================================================================
void aoStorageSaveDataStatCallback(
	CellSaveDataCBResult* cbResult,
	CellSaveDataStatGet* get, CellSaveDataStatSet* set)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = (AOS_STORAGE*)cbResult->userdata;

	// 正常値設定
	cbResult->result = CELL_SAVEDATA_CBRESULT_OK_NEXT;
	cbResult->progressBarInc = 0;
	cbResult->errNeedSizeKB = 0;
	cbResult->invalidMsg = NULL;
	cbResult->userdata = glb;
	glb->hdd_less = FALSE;

	amZeroMemory(&glb->sys_param, sizeof(CellSaveDataSystemFileParam));

	set->setParam = &glb->sys_param;
	set->reCreateMode = CELL_SAVEDATA_RECREATE_NO;

	// インジケータ設定
	set->indicator = &g_ao_storage_indicator;

	// タイトル名設定
	strncpy(
		set->setParam->title,
		AoMsgGetStr8(g_ao_storage_save_msg_file, 0, 0),
		CELL_SAVEDATA_SYSP_TITLE_SIZE);
	set->setParam->title[CELL_SAVEDATA_SYSP_TITLE_SIZE - 1] = 0;

	// サブタイトル名設定
	strncpy(
		set->setParam->subTitle,
		AoMsgGetStr8(g_ao_storage_save_msg_file, 1, 0),
		CELL_SAVEDATA_SYSP_SUBTITLE_SIZE);
	set->setParam->subTitle[CELL_SAVEDATA_SYSP_SUBTITLE_SIZE - 1] = 0;

	// 詳細情報設定
	strncpy(
		set->setParam->detail,
		AoMsgGetStr8(g_ao_storage_save_msg_file, 2, 0),
		CELL_SAVEDATA_SYSP_DETAIL_SIZE);
	set->setParam->detail[CELL_SAVEDATA_SYSP_DETAIL_SIZE - 1] = 0;

	// 利用条件設定
	set->setParam->attribute = CELL_SAVEDATA_ATTR_NODUPLICATE;

	// リストパラメータ設定
	strncpy(
		set->setParam->listParam,
		"CONFIG",
		CELL_SAVEDATA_SYSP_LPARAM_SIZE);
	set->setParam->listParam[CELL_SAVEDATA_SYSP_LPARAM_SIZE - 1] = 0;

	// 新規セーブ判定
	if (glb->save_first ||
		(get->isNewData == CELL_SAVEDATA_ISNEWDATA_YES))
	{
		amAssert(g_ao_storage_save_msg_file);

		// 所有者情報クリア
		if (glb->save_first) {
			set->reCreateMode = CELL_SAVEDATA_RECREATE_YES_RESET_OWNER;
		}

		// 必要HDD容量算出
		s32 new_size_kb = 0;
		new_size_kb += get->sysSizeKB;
		new_size_kb += aoStorageUtilConvBtoKB(g_ao_storage_save_icon0_png_size);
		new_size_kb += aoStorageUtilConvBtoKB(g_ao_storage_save_pic1_png_size);
		new_size_kb += aoStorageUtilConvBtoKB(glb->buf_size);

		amSystemLog("new save data size : %dKB\n", new_size_kb);

		// HDD空き容量チェック
		s32 need_size_kb = get->hddFreeSizeKB - new_size_kb;
		if (need_size_kb < 0) {
			// HDD空き容量不足エラー
			cbResult->result = CELL_SAVEDATA_CBRESULT_ERR_NOSPACE;
			cbResult->errNeedSizeKB = need_size_kb;
			glb->hdd_less = TRUE;
			return;
		}

		// 書き込みファイルインデックス設定
		glb->file_index = (AOE_STORAGE_SFILE)0;
	}
	else {
		// 上書きセーブ
		s32 current_size_kb = get->hddFreeSizeKB;
		s32 minimum_size_kb = get->hddFreeSizeKB;

		for (s32 i = 0; i < AOD_STORAGE_SFILE_NUM; ++i) {

			// ファイル名とサイズを取得
			char* fname;
			s32 diff_size_kb;
			switch (i) {
			case AOD_STORAGE_SFILE_ICON0:
				fname = g_ao_storage_file_icon0_name;
				diff_size_kb = (s32)g_ao_storage_save_icon0_png_size;
				break;
			case AOD_STORAGE_SFILE_PIC1:
				fname = g_ao_storage_file_pic1_name;
				diff_size_kb = (s32)g_ao_storage_save_pic1_png_size;
				break;
			case AOD_STORAGE_SFILE_SAVE_DATA:
				fname = g_ao_storage_file_name;
				diff_size_kb = (s32)glb->buf_size;
				break;
			default:
				amAssert(0);
				cbResult->result = CELL_SAVEDATA_CBRESULT_ERR_INVALID;
				return;
			}
			diff_size_kb = aoStorageUtilConvBtoKB(diff_size_kb);

			// 既存のファイルがあるか判定
			u32 j;
			for (j = 0; j < get->fileListNum; ++j) {
				if (strcmp(fname, get->fileList[j].fileName) == 0) {
					break;
				}
			}
			if (j < get->fileListNum) {
				// 既存ファイルあり
				diff_size_kb -=
					aoStorageUtilConvBtoKB(get->fileList[j].st_size);
			}

			current_size_kb -= diff_size_kb;
			if (current_size_kb < minimum_size_kb) {
				minimum_size_kb = current_size_kb;
			}
		}

		// HDD空き容量チェック
		if (minimum_size_kb < 0) {
			// HDD空き容量不足エラー
			cbResult->result = CELL_SAVEDATA_CBRESULT_ERR_NOSPACE;
			cbResult->errNeedSizeKB = minimum_size_kb;
			glb->hdd_less = TRUE;
			return;
		}

		// 書き込みファイルインデックス設定
		glb->file_index = AOD_STORAGE_SFILE_DF_S;
	}
}

// ===========================================================================
//! セーブファイル操作コールバック
// ===========================================================================
void aoStorageSaveDataFileCallback(
	CellSaveDataCBResult* cbResult,
	CellSaveDataFileGet* get, CellSaveDataFileSet* set)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = (AOS_STORAGE*)cbResult->userdata;

	// 正常値設定
	cbResult->result = CELL_SAVEDATA_CBRESULT_OK_NEXT;

	// ファイル書き込み設定
	switch (glb->file_index) {
	case AOD_STORAGE_SFILE_ICON0:
		set->fileOperation = CELL_SAVEDATA_FILEOP_WRITE;
		set->reserved = NULL;
		set->fileType = CELL_SAVEDATA_FILETYPE_CONTENT_ICON0;
		set->fileName = NULL;
		set->fileOffset = 0;
		set->fileSize = g_ao_storage_save_icon0_png_size;
		set->fileBufSize = g_ao_storage_save_icon0_png_size;
		set->fileBuf = g_ao_storage_save_icon0_png;

		glb->file_index = AOD_STORAGE_SFILE_PIC1;
		break;

	case AOD_STORAGE_SFILE_PIC1:
		set->fileOperation = CELL_SAVEDATA_FILEOP_WRITE;
		set->reserved = NULL;
		set->fileType = CELL_SAVEDATA_FILETYPE_CONTENT_PIC1;
		set->fileName = NULL;
		set->fileOffset = 0;
		set->fileSize = g_ao_storage_save_pic1_png_size;
		set->fileBufSize = g_ao_storage_save_pic1_png_size;
		set->fileBuf = g_ao_storage_save_pic1_png;

		glb->file_index = AOD_STORAGE_SFILE_SAVE_DATA;
		break;

	case AOD_STORAGE_SFILE_SAVE_DATA:
		set->fileOperation = CELL_SAVEDATA_FILEOP_WRITE;
		set->reserved = NULL;
		set->fileType = CELL_SAVEDATA_FILETYPE_SECUREFILE;
		memcpy(
			set->secureFileId, g_storage_secure_id,
			CELL_SAVEDATA_SECUREFILEID_SIZE);
		set->fileName = g_ao_storage_file_name;
		set->fileOffset = 0;
		set->fileSize = glb->buf_size;
		set->fileBufSize = glb->buf_size;
		set->fileBuf = glb->buf;

		glb->file_index = AOD_STORAGE_SFILE_NONE;
		break;

	default:
		// 終了
		cbResult->result = CELL_SAVEDATA_CBRESULT_OK_LAST;
		break;
	}
}


// ***************************************************************************
// ロード
// ***************************************************************************
// ===========================================================================
//! ロードスレッド
// ===========================================================================
void aoStorageLoadThread(uint64_t arg)
{
	// スレッド開始
	amThreadOpen((AMS_THREAD*)arg);

	int ret;

	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// ロード領域作成
	amAssert(glb->buf == NULL);
	glb->buf_size = sizeof(AOS_STORAGE_HEADER) + glb->load_size;
	glb->buf_size = ((glb->buf_size + 31) / 32) * 32;
	glb->buf = (u8*)amMemAllocTemp(glb->buf_size);

	// 汎用バッファ作成
	CellSaveDataSetBuf buf;
	buf.dirListMax = 0;
	buf.fileListMax = 16;
	amZeroMemory(buf.reserved, sizeof(buf.reserved));
	buf.bufSize = sizeof(CellSaveDataFileStat) * 16;
	buf.buf = amMemAlloc(buf.bufSize);

	// ロード開始
	ret = cellSaveDataAutoLoad2(
		CELL_SAVEDATA_VERSION_CURRENT,
		g_ao_storage_dir_name,
		CELL_SAVEDATA_ERRDIALOG_NONE,
		&buf,
		aoStorageLoadDataStatCallback,
		aoStorageLoadDataFileCallback,
		SYS_MEMORY_CONTAINER_ID_INVALID,
		glb);

	// バッファ解放
	amMemFree(buf.buf);

	// バッファ解放
	if (glb->buf) {
		amMemFree(glb->buf);
		glb->buf = NULL;
	}
	glb->buf_size = 0;

	if (ret == CELL_SAVEDATA_RET_OK) {
		// 成功
		glb->load_success = TRUE;
	}
	else {
		// 失敗
		glb->load_success = FALSE;
	}

	// 終了
	amThreadQuit(&glb->th);
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
		glb->th_execute = FALSE;

		// フェード要求コールバック呼び出し
		if (g_ao_storage_fade_in_flag && g_ao_storage_fade_callback) {
			g_ao_storage_fade_callback(
				TRUE, g_ao_storage_fade_callback_arg);
			g_ao_storage_fade_in_flag = FALSE;
		}

		// ゲーム終了を有効化
		amSystemExitEnable(1);

		// 成功したなら終了
		if (glb->load_success) {

			// タスク終了
			amTaskDelete(tcb);

			// 値初期化
			glb->load_buf = NULL;
			glb->load_size = 0;

			// 完了設定
			glb->state = AOD_STORAGE_STATE_IDLE;
			glb->tcb = NULL;
		}
		else {

			// メッセージ表示開始待ちへ遷移
			amTaskSetProcedure(tcb, aoStorageLoadTaskStartMessage);
		}
	}
}

// ===========================================================================
//! ロードスレッド後メッセージ表示開始待ち
// ===========================================================================
void aoStorageLoadTaskStartMessage(AMS_TCB* tcb)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// 既存メッセージの終了待ち
	if (AoSysMsgIsFinished()) {

		if (glb->load_none) {
			// 新規セーブ確認メッセージ表示
			AoSysMsgStart(
				AOD_SYS_MSG_IS_NEW_SAVEDATA, AOD_SYS_MSG_SELECT_YESNO);
		}
		else {
			// データ破損通知メッセージ表示
			AoSysMsgStart(
				AOD_SYS_MSG_STORAGE_LOAD_FAILURE, AOD_SYS_MSG_SELECT_YESNO);
		}

		// メッセージ待ちへ遷移
		amTaskSetProcedure(tcb, aoStorageLoadTaskSaveMessage);
	}
}

// ===========================================================================
//! セーブ確認メッセージ
// ===========================================================================
void aoStorageLoadTaskSaveMessage(AMS_TCB* tcb)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// メッセージ終了待ち
	if (AoSysMsgIsFinished()) {
		// 結果取得
		if (AoSysMsgGetResult() == AOD_SYS_MSG_RESULT_YES) {
			// 終了
			aoStorageSetError(AOD_STORAGE_ERROR_LOADDATA_NONE);

			// タスク終了
			amTaskDelete(tcb);

			// 値初期化
			glb->load_buf = NULL;
			glb->load_size = 0;

			// 完了設定
			glb->state = AOD_STORAGE_STATE_IDLE;
			glb->tcb = NULL;
		}
		else {
			// キャンセル確認メッセージ表示
			AoSysMsgStart(AOD_SYS_MSG_SAVE_CANCEL, AOD_SYS_MSG_SELECT_YESNO);

			// メッセージ待ちへ遷移
			amTaskSetProcedure(tcb, aoStorageLoadTaskCancelMessage);
		}
	}
}

// ===========================================================================
//! キャンセル確認メッセージ
// ===========================================================================
void aoStorageLoadTaskCancelMessage(AMS_TCB* tcb)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = aoStorageGetGlobal();

	// メッセージ終了待ち
	if (AoSysMsgIsFinished()) {
		// 結果取得
		if (AoSysMsgGetResult() == AOD_SYS_MSG_RESULT_YES) {
			// キャンセル
			aoStorageSetError(AOD_STORAGE_ERROR_NO_SAVE);
		}
		else {
			// 繰り返し
			// 新規セーブ確認メッセージ表示
			AoSysMsgStart(
				AOD_SYS_MSG_IS_NEW_SAVEDATA, AOD_SYS_MSG_SELECT_YESNO);

			// メッセージ待ちへ遷移
			amTaskSetProcedure(tcb, aoStorageLoadTaskSaveMessage);
			return;
		}

		// タスク終了
		amTaskDelete(tcb);

		// 値初期化
		glb->load_buf = NULL;
		glb->load_size = 0;

		// 完了設定
		glb->state = AOD_STORAGE_STATE_IDLE;
		glb->tcb = NULL;
	}
}

// ===========================================================================
//! セーブデータステータスコールバック
// ===========================================================================
void aoStorageLoadDataStatCallback(
	CellSaveDataCBResult* cbResult,
	CellSaveDataStatGet* get, CellSaveDataStatSet* set)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = (AOS_STORAGE*)cbResult->userdata;

	// ロードデータなしに設定
	glb->load_none = TRUE;

	// セーブデータが存在するかチェック
	if (get->isNewData) {
		cbResult->result = CELL_SAVEDATA_CBRESULT_ERR_NODATA;
		return;
	}

	// リストオーバーチェック
	if (get->fileListNum < get->fileNum) {
		cbResult->result = CELL_SAVEDATA_CBRESULT_ERR_BROKEN;
		return;
	}

	// セーブデータが存在するかチェック
	u32 i;
	for (i = 0; i < get->fileListNum; ++i) {
		if (strcmp(get->fileList[i].fileName, g_ao_storage_file_name) == 0) {
			if (get->fileList[i].st_size != glb->buf_size) {
				cbResult->result = CELL_SAVEDATA_CBRESULT_ERR_BROKEN;
				return;
			}
			break;
		}
	}
	if (i >= get->fileListNum) {
		// セーブデータなし
		cbResult->result = CELL_SAVEDATA_CBRESULT_ERR_NODATA;
		return;
	}

	// 他人のセーブデータかチェック
	if ((get->bind & CELL_SAVEDATA_BINDSTAT_ERR_NOOWNER) ||
		(get->bind & CELL_SAVEDATA_BINDSTAT_ERR_OWNER))
	{
		// セーブデータなし扱い
		cbResult->result = CELL_SAVEDATA_CBRESULT_ERR_FAILURE;
		return;
	}

	glb->load_none = FALSE;

	// 正常値設定
	cbResult->result = CELL_SAVEDATA_CBRESULT_OK_NEXT;
	cbResult->progressBarInc = 0;
	cbResult->errNeedSizeKB = 0;
	cbResult->invalidMsg = NULL;
	cbResult->userdata = glb;
	set->reCreateMode = CELL_SAVEDATA_RECREATE_NO_NOBROKEN;
	set->setParam = NULL;
	set->indicator = &g_ao_storage_indicator;

	// 書き込みファイルインデックス設定
	glb->file_index = AOD_STORAGE_SFILE_DF_S;
}

// ===========================================================================
//! セーブファイル操作コールバック
// ===========================================================================
void aoStorageLoadDataFileCallback(
	CellSaveDataCBResult* cbResult,
	CellSaveDataFileGet* get, CellSaveDataFileSet* set)
{
	// グローバルデータ取得
	AOS_STORAGE* glb = (AOS_STORAGE*)cbResult->userdata;

	// 正常値設定
	cbResult->result = CELL_SAVEDATA_CBRESULT_OK_NEXT;

	// ファイル読み込み設定
	if (glb->file_index == AOD_STORAGE_SFILE_DF_S) {
		set->fileOperation = CELL_SAVEDATA_FILEOP_READ;
		set->reserved = NULL;
		set->fileType = CELL_SAVEDATA_FILETYPE_SECUREFILE;
		memcpy(
			set->secureFileId, g_storage_secure_id,
			CELL_SAVEDATA_SECUREFILEID_SIZE);
		set->fileName = g_ao_storage_file_name;
		set->fileOffset = 0;
		set->fileSize = glb->buf_size;
		set->fileBufSize = glb->buf_size;
		set->fileBuf = glb->buf;

		glb->file_index = AOD_STORAGE_SFILE_NONE;
	}

	// 読み込みファイルチェック
	else {
		if (get->excSize != glb->buf_size) {
			cbResult->result = CELL_SAVEDATA_CBRESULT_ERR_BROKEN;
			return;
		}

		// ヘッダ取得&チェック
		const AOS_STORAGE_HEADER* head = (const AOS_STORAGE_HEADER*)glb->buf;
		if ((head->masic != AOD_STORAGE_MASIC) ||
			(head->file_size != glb->buf_size) ||
			(head->data_size != glb->load_size))
		{
			cbResult->result = CELL_SAVEDATA_CBRESULT_ERR_BROKEN;
			return;
		}
		u8 md5[16];
		cellMd5Digest(head + 1, head->data_size, md5);
		if (memcmp(md5, head->md5, sizeof(u8) * 16) != 0) {
			cbResult->result = CELL_SAVEDATA_CBRESULT_ERR_BROKEN;
			return;
		}
		// 読み込みデータコピー
		amCopyMemory(glb->load_buf, head + 1, glb->load_size);

		// 終了
		cbResult->result = CELL_SAVEDATA_CBRESULT_OK_LAST;
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


// ***************************************************************************
// その他
// ***************************************************************************
// ===========================================================================
//! バイト単位の数値をKB単位の数値に変換
// ===========================================================================
s32 aoStorageUtilConvBtoKB(s32 b_size)
{
	return (b_size + 1023) / 1024;
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
