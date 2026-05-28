// ===========================================================================
/*!
	@file	aoSysMsg.h
	@brief	AoLibrary システムメッセージモジュール宣言

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================
#pragma once

// ----- Include Files ---------------------------------------（インクルード）
#include "ao.h"

// ----- Macros ------------------------------------------------（マクロ定義）

//! デフォルトのメッセージ描画タスク優先度
#define AOD_SYS_MSG_DRAW_TASK_PRIO		(0xffff)

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）

// ===========================================================================
//	enum AOE_SYS_MSG_ID
// ---------------------------------------------------------------------------
//!	システムメッセージID列挙
// ===========================================================================
typedef enum tag_AOE_SYS_MSG_ID {

#if defined(AOD_PLATFORM_WIN32)
	// Win32
	AOD_SYS_MSG_DUMMY				= 0,	//!< ダミー
#endif // defined(AOD_PLATFORM_WIN32)

#if defined(AOD_PLATFORM_XBOX360)
	// Xbox360
	AOD_SYS_MSG_OVERWRITE_SAVEDATA	= 0,	//!< セーブデータの上書き確認
	AOD_SYS_MSG_STORAGE_CHANGED,			//!< ストレージ機器の抜き差し通知
	AOD_SYS_MSG_STORAGE_NO_SPACE,			//!< ストレージ機器の容量不足通知
	AOD_SYS_MSG_STORAGE_CANCEL,				//!< セーブキャンセル確認
	AOD_SYS_MSG_STORAGE_DEVICE,				//!< ストレージ機器選択確認
	AOD_SYS_MSG_STORAGE_SAVE_FAILURE,		//!< セーブ失敗
	AOD_SYS_MSG_STORAGE_LOAD_FAILURE,		//!< ロード失敗
	AOD_SYS_MSG_NO_SIGNIN,					//!< 「サインインせずに進行」の確認
	AOD_SYS_MSG_SIGNINCHANGED,				//!< 「サインイン状態変更」の通知
	AOD_SYS_MSG_IS_NEW_SAVEDATA,			//!< 新規セーブデータ作成確認
	AOD_SYS_MSG_NET_ERROR_COMMON,			//!< 汎用通信エラー通知
	AOD_SYS_MSG_NET_ERROR_OFFLINE,			//!< 未サインイン通知
	AOD_SYS_MSG_PLEASE_SIGNIN,				//!< サインイン促し文言
	AOD_SYS_MSG_ERROR_PRIVILEGE,			//!< プロフィール閲覧権限無し

#endif // defined(AOD_PLATFORM_XBOX360)

#if defined(AOD_PLATFORM_PS3)
	// PS3
	AOD_SYS_MSG_SAVE_CANCEL,				//!< セーブ処理キャンセル確認
	AOD_SYS_MSG_STORAGE_SAVE_FAILURE,		//!< セーブ失敗通知メッセージ
	AOD_SYS_MSG_STORAGE_LOAD_FAILURE,		//!< データ破損通知メッセージ
	AOD_SYS_MSG_IS_NEW_SAVEDATA,			//!< 新規セーブ確認メッセージ
	AOD_SYS_MSG_ALL_SAVE_DELETE,			//!< 全セーブデータ対象削除確認
	AOD_SYS_MSG_NET_ERROR_OFFLINE,			//!< 未サインイン通知
	AOD_SYS_MSG_NET_ERROR_COMMON,			//!< 汎用通信エラー通知
	AOD_SYS_MSG_NET_ERROR_PARENTALLOCK,		//!< パレンタルロック
	AOD_SYS_MSG_HDD_DATA_DELETE,			//!< 不要なデータ削除

#endif // defined(AOD_PLATFORM_PS3)

#if defined(AOD_PLATFORM_WII)
	// Wii
	AOD_SYS_MSG_SAVE_LACK_SPACE,			//!< 空き容量不足
	AOD_SYS_MSG_SAVE_LACK_FILE,				//!< 空きファイル数不足
	AOD_SYS_MSG_SAVE_DATA_DESTROY,			//!< セーブデータ破損
	AOD_SYS_MSG_SAVE_DESTROY_DELETE,		//!< 破損データ削除確認
	AOD_SYS_MSG_SAVE_CANCEL,				//!< セーブ処理キャンセル確認
	AOD_SYS_MSG_MISS00,						//!< 欠番00
	AOD_SYS_MSG_NET_ERROR_NOSAVE,			//!< セーブデータなしエラー
	AOD_SYS_MSG_NET_ERROR_DWC0001,			//!< DWC定型文0001
	AOD_SYS_MSG_NET_ERROR_E001,				//!< 通信エラー001
	AOD_SYS_MSG_NET_ERROR_E002,				//!< 通信エラー002
	AOD_SYS_MSG_NET_ERROR_E003,				//!< 通信エラー003
	AOD_SYS_MSG_NET_ERROR_E004,				//!< 通信エラー004
	AOD_SYS_MSG_NET_ERROR_E005,				//!< 通信エラー005
	AOD_SYS_MSG_NET_ERROR_E006,				//!< 通信エラー006
	AOD_SYS_MSG_NET_ERROR_E007,				//!< 通信エラー007
	AOD_SYS_MSG_NET_ERROR_E008,				//!< 通信エラー008
	AOD_SYS_MSG_NET_ERROR_DWC0003,			//!< DWC定型文0003

#endif // defined(AOD_PLATFORM_WII)

	AOD_SYS_MSG_NUM,						//!< ID数
	AOD_SYS_MSG_NONE,						//!< 無効コード
} AOE_SYS_MSG_ID;

#if defined(AOD_PLATFORM_WII)
// ===========================================================================
//	enum AOE_SYS_MSG_FATAL_ID
// ---------------------------------------------------------------------------
//!	FATALエラーメッセージID列挙
// ===========================================================================
typedef enum tag_AOE_SYS_MSG_FATAL_ID {
	AOD_SYS_MSG_FATAL_WARE_01	= 0,		//!< WARE_01
	AOD_SYS_MSG_FATAL_NAND_08,				//!< NAND_08
	AOD_SYS_MSG_FATAL_NAND_11,				//!< NAND_11
	AOD_SYS_MSG_FATAL_NAND_12,				//!< NAND_12

	AOD_SYS_MSG_FATAL_NUM,					//!< ID数
	AOD_SYS_MSG_FATAL_NONE,					//!< 無効コード
} AOE_SYS_MSG_FATAL_ID;
#endif // defined(AOD_PLATFORM_WII)

// ===========================================================================
//	enum AOE_SYS_MSG_SELECT
// ---------------------------------------------------------------------------
//!	システムメッセージ選択項目列挙
// ===========================================================================
typedef enum tag_AOE_SYS_MSG_SELECT {
	AOD_SYS_MSG_SELECT_OK			= 0,	//!< OK
	AOD_SYS_MSG_SELECT_OKCANCEL,			//!< OK & キャンセル
	AOD_SYS_MSG_SELECT_YESNO,				//!< はい & いいえ
	AOD_SYS_MSG_SELECT_YESNOCANCEL,			//!< はい & いいえ & キャンセル

#if defined(AOD_PLATFORM_WII)
	AOD_SYS_MSG_SELECT_DISABLE,				//!< 選択無効
#endif // defined(AOD_PLATFORM_WII)

	AOD_SYS_MSG_SELECT_NUM,					//!< 選択項目数
	AOD_SYS_MSG_SELECT_NONE,				//!< 無効コード
} AOE_SYS_MSG_SELECT;

// ===========================================================================
//	enum AOE_SYS_MSG_RESULT
// ---------------------------------------------------------------------------
//!	システムメッセージ結果列挙
// ===========================================================================
typedef enum tag_AOE_SYS_MSG_RESULT {
	AOD_SYS_MSG_RESULT_OK			= 0,	//!< OK
	AOD_SYS_MSG_RESULT_CANCEL,				//!< キャンセル
	AOD_SYS_MSG_RESULT_YES,					//!< はい
	AOD_SYS_MSG_RESULT_NO,					//!< いいえ

	AOD_SYS_MSG_RESULT_SYS_CANCEL,			//!< システムによるキャンセル

	AOD_SYS_MSG_RESULT_NUM,					//!< 結果数
	AOD_SYS_MSG_RESULT_NONE,				//!< 無効コード
} AOE_SYS_MSG_RESULT;

// ----- Struct Definitions --------------------------------------（型の宣言）

#if defined(AOD_PLATFORM_WIN32)
typedef LPCWSTR		AOT_SYS_MSG_CODE;	//!< 文字コード型
#elif defined(AOD_PLATFORM_XBOX360)
typedef LPCWSTR		AOT_SYS_MSG_CODE;	//!< 文字コード型
#elif defined(AOD_PLATFORM_PS3)
typedef const char*	AOT_SYS_MSG_CODE;	//!< 文字コード型
#elif defined(AOD_PLATFORM_WII)
typedef const char*	AOT_SYS_MSG_CODE;	//!< 文字コード型
#elif defined(AOD_PLATFORM_IPHONE)
typedef const char*	AOT_SYS_MSG_CODE;	//!< 文字コード型
#endif

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）

// ===========================================================================
//	AoSysMsgSetBaseMsgFile
/*!
	メッセージファイル設定

	@param file		[in] メッセージファイル
	@note
	AOE_SYS_MSG_IDのメッセージ内容やAOE_SYS_MSG_SELECTの選択内容を記述した
	メッセージファイルを設定します。\n
	このモジュールを使用するためには、
	この関数で適切なメッセージファイルが設定されている必要があります。\n
	アプリケーション開始時に設定し、以降解放はしない使い方を想定しています。\n
*/
// ===========================================================================
extern void AoSysMsgSetBaseMsgFile(const void* file);

// ===========================================================================
//	AoSysMsgGetBaseMsgFile
/*!
	メッセージファイル取得

	@return メッセージファイル
*/
// ===========================================================================
extern const void* AoSysMsgGetBaseMsgFile(void);

#if defined(AOD_PLATFORM_WII)

// ===========================================================================
//	AoSysMsgSetFatalMsgFile
/*!
	FATALエラーメッセージファイル設定

	@param file		[in] FATALエラーメッセージファイル
*/
// ===========================================================================
extern void AoSysMsgSetFatalMsgFile(const void* file);

// ===========================================================================
//	AoSysMsgGetFatalMsgFile
/*!
	FATALエラーメッセージファイル取得

	@return FATALエラーメッセージファイル
*/
// ===========================================================================
extern const void* AoSysMsgGetFatalMsgFile(void);

#endif // defined(AOD_PLATFORM_WII)

// ===========================================================================
//	AoSysMsgStart
/*!
	システムメッセージ開始

	@param id		[in] メッセージID
	@param select	[in] 選択項目
	@note
	指定IDのメッセージと選択項目を表示するシステムメッセージの表示を
	開始します。\n
	すでにシステムメッセージの表示を行っている場合
	(AoSysMsgIsFinished関数がFALSEを返す場合)は
	アサートし何も行いません。\n
*/
// ===========================================================================
extern void AoSysMsgStart(AOE_SYS_MSG_ID id, AOE_SYS_MSG_SELECT select);

#if defined(AOD_PLATFORM_WII)
// ===========================================================================
//	AoSysMsgStart
/*!
	システムメッセージ開始(数値指定あり)

	@param id		[in] メッセージID
	@param dec		[in] メッセージに挿入する数値
	@param select	[in] 選択項目
*/
// ===========================================================================
extern void AoSysMsgStart(
	AOE_SYS_MSG_ID id, int dec, AOE_SYS_MSG_SELECT select);
#endif // defined(AOD_PLATFORM_WII)

// ===========================================================================
//	AoSysMsgStart
/*!
	システムメッセージ開始

	@param file		[in] メッセージファイル
	@param id		[in] メッセージID
	@param select	[in] 選択項目
	@note
	指定のメッセージファイル内の指定のIDのメッセージと
	選択項目を表示するシステムメッセージの表示を開始します。\n
	すでにシステムメッセージの表示を行っている場合
	(AoSysMsgIsFinished関数がFALSEを返す場合)は
	アサートし何も行いません。\n
	メッセージファイルは、
	メッセージの表示が終了するまで解放しないようにしてください。\n
*/
// ===========================================================================
extern void AoSysMsgStart(const void* file, u32 id, AOE_SYS_MSG_SELECT select);

// ===========================================================================
//	AoSysMsgStart
/*!
	システムメッセージ開始

	@param str		[in] メッセージ文字列
	@param select	[in] 選択項目
	@note
	指定のメッセージと選択項目を表示する
	システムメッセージの表示を開始します。\n
	すでにシステムメッセージの表示を行っている場合
	(AoSysMsgIsFinished関数がFALSEを返す場合)は
	アサートし何も行いません。\n
*/
// ===========================================================================
extern void AoSysMsgStart(AOT_SYS_MSG_CODE str, AOE_SYS_MSG_SELECT select);

#if defined(AOD_PLATFORM_PS3)
// ===========================================================================
//	AoSysMsgStartSceError
/*!
	SCE関数エラー表示システムメッセージ開始

	@param error	[in] SCEエラーコード
*/
// ===========================================================================
extern void AoSysMsgStartSceError(u32 error);
#endif // defined(AOD_PLATFORM_PS3)

// ===========================================================================
//	AoSysMsgCancel
/*!
	システムメッセージキャンセル

	@note
	現在表示しているシステムメッセージを
	ユーザの決定を待たずにキャンセルします。\n
	キャンセルした場合は、結果として
	必ずAOD_SYS_MSG_RESULT_SYS_CANCELを返します。\n
	この関数を呼び出しても、すぐには完了しないので、
	必ずAoSysMsgIsFinished関数で完了を判定するようにして下さい。\n
	AoSysMsgIsFinished関数がTRUEを返す状態で呼び出した場合は
	アサートし何も行いません。\n
*/
// ===========================================================================
extern void AoSysMsgCancel(void);

// ===========================================================================
//	AoSysMsgIsFinished
/*!
	システムメッセージ表示完了判定

	@return 真：完了　偽：表示中
	@note
	AoSysMsgStart関数で開始したメッセージ表示が完了したかどうかを判定します。\n
*/
// ===========================================================================
extern BOOL AoSysMsgIsFinished(void);

// ===========================================================================
//	AoSysMsgGetResult
/*!
	システムメッセージ結果取得

	@return 結果
	@note
	直前に表示したメッセージの結果を返します。\n
	AoSysMsgIsFinished関数がFALSEを返す状態で呼び出した場合は、
	アサートしAOD_SYS_MSG_RESULT_NONEを返します。\n
	AoSysMsgStart関数を呼び出した段階で
	以前の結果はクリアされるので注意して下さい。\n
*/
// ===========================================================================
extern AOE_SYS_MSG_RESULT AoSysMsgGetResult(void);

// ===========================================================================
//	AoSysMsgIsShow
/*!
	システムメッセージ表示中判定(数フレーム遅延あり)

	@return 真：完了　偽：表示中
	@note
	システムメッセージが現在表示されているかどうかを判定します。\n
	通常、AoSysMsgStart関数呼び出しにより即時にメッセージ表示されますが、
	既存のUI表示がされている場合などは、
	終了を待ってからメッセージの表示を行ないます。\n
	既存のUI表示の終了待ち中はFALSEを返し、実際に表示されている場合のみ
	TRUEを返します。\n
*/
// ===========================================================================
extern BOOL AoSysMsgIsShow(void);

// ===========================================================================
//	AoSysMsgIsShowReal
/*!
	システムメッセージ表示中判定

	@return 真：完了　偽：表示中
	@note
	AoSysMsgIsShow関数とほぼ同じ関数になりますが、
	AoSysMsgIsShow関数は、
	実際に非表示になった数フレーム後に非表示を返すのに対し、
	この関数は、非表示になった瞬間に非表示を返します。\n
*/
// ===========================================================================
extern BOOL AoSysMsgIsShowReal(void);

#if defined(AOD_PLATFORM_WII)

// ===========================================================================
//	AoSysMsgSetDrawTaskPrio
/*!
	メッセージ描画タスク優先度設定

	@param prio		[in] メッセージ描画タスク優先度
*/
// ===========================================================================
extern void AoSysMsgSetDrawTaskPrio(u16 prio = AOD_SYS_MSG_DRAW_TASK_PRIO);

// ===========================================================================
//	AoSysMsgGetDrawTaskPrio
/*!
	メッセージ描画タスク優先度取得

	@return メッセージ描画タスク優先度
*/
// ===========================================================================
extern u16 AoSysMsgGetDrawTaskPrio(void);

// ===========================================================================
//	AoSysMsgSetWinTexAmb
/*!
	ウインドウテクスチャAMB設定

	@param texlist	[in] テクスチャAMB
	@param tex_id	[in] テクスチャID
*/
// ===========================================================================
extern void AoSysMsgSetWinTexAmb(void* amb, u32 tex_id);

// ===========================================================================
//	AoSysMsgSetKeyDecide
/*!
	決定キー設定

	@param key		[in] 決定キー
*/
// ===========================================================================
extern void AoSysMsgSetKeyDecide(u32 key);

// ===========================================================================
//	AoSysMsgSetKeyCancel
/*!
	キャンセルキー設定

	@param key		[in] キャンセルキー
*/
// ===========================================================================
extern void AoSysMsgSetKeyCancel(u32 key);

// ===========================================================================
//	AoSysMsgSetKeyLeft
/*!
	左キー設定

	@param key		[in] 左キー
*/
// ===========================================================================
extern void AoSysMsgSetKeyLeft(u32 key);

// ===========================================================================
//	AoSysMsgSetKeyRight
/*!
	右キー設定

	@param key		[in] 右キー
*/
// ===========================================================================
extern void AoSysMsgSetKeyRight(u32 key);

// ===========================================================================
//	AoSysMsgShowFatalError
/*!
	Fatalエラー表示

	@param id		[in] FATALエラーメッセージID
	@note
	この関数を呼び出すと、指定のメッセージを画面表示した後に
	アプリケーションを停止し復帰しません。\n
*/
// ===========================================================================
extern void AoSysMsgShowFatalError(AOE_SYS_MSG_FATAL_ID id);

#endif // defined(AOD_PLATFORM_WII)

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ----- Static Functions --------------------（スタティック関数の定義：局所）

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
