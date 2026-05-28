// ===========================================================================
/*!
	@file	aoSysMsg.cpp
	@brief	AoLibrary システムメッセージモジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"

#if defined(AOD_PLATFORM_PS3)

#include "sysutil/sysutil_msgdialog.h"

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//! タスクワーク
// ===========================================================================
typedef struct tag_AOS_SYS_MSG_WORK {
	AOT_SYS_MSG_CODE	str;			//!< メッセージ文字列
	int					sce_error;		//!< SCEエラーコード
	AOE_SYS_MSG_SELECT	select;			//!< 選択項目
	BOOL				is_cancel;		//!< 真：キャンセル
	BOOL				is_show;		//!< 真：表示中
} AOS_SYS_MSG_WORK;

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

static void aoSysMsgStart(AOT_SYS_MSG_CODE str, AOE_SYS_MSG_SELECT select);
static void aoSysMsgStart(int sce_error);

static void aoSysMsgTask00(AMS_TCB* tcb);
static void aoSysMsgTask01(AMS_TCB* tcb);

static void aoSysMsgCbMsgDlg(int button_type, void *userdata);

static u32 aoSysMsgGetSysMsgId(AOE_SYS_MSG_ID id);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//	AOE_SYS_MSG_RESULT g_ao_sys_msg_result
// ---------------------------------------------------------------------------
//!	結果
// ===========================================================================
static AOE_SYS_MSG_RESULT g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_NONE;

// ===========================================================================
//! メッセージファイル
// ===========================================================================
static const void* g_ao_sys_msg_file = NULL;

// ===========================================================================
//! タスクTCBポインタ
// ===========================================================================
static AMS_TCB* g_ao_sys_msg_tcb = NULL;

// ===========================================================================
//! 選択肢ごとのフラグ
// ===========================================================================
static const u32 g_ao_sys_msg_select_tbl[AOD_SYS_MSG_SELECT_NUM] = {
	CELL_MSGDIALOG_TYPE_BUTTON_TYPE_OK |
		CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_ON,
	CELL_MSGDIALOG_TYPE_BUTTON_TYPE_OK,
	CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO |
		CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_ON,
	CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO,
};

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

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
void AoSysMsgSetBaseMsgFile(const void* file)
{
	if (file) {
		amAssert(AoMsgGetCodeType(file) == AOD_MSG_CODE_UTF8);
		g_ao_sys_msg_file = file;
	}
	else {
		g_ao_sys_msg_file = NULL;
	}
}

// ===========================================================================
//	AoSysMsgGetBaseMsgFile
/*!
	メッセージファイル取得

	@return メッセージファイル
*/
// ===========================================================================
const void* AoSysMsgGetBaseMsgFile(void)
{
	return g_ao_sys_msg_file;
}

// ===========================================================================
//	AoSysMsgStart
/*!
	システムメッセージ開始

	@param id		[in] メッセージID
	@param select	[in] 選択項目
	@note
	指定IDのメッセージと選択項目を表示する
	システムメッセージの表示を開始します。\n
	すでにシステムメッセージの表示を行っている場合
	(AoSysMsgIsFinished関数がFALSEを返す場合)は
	アサートし何も行いません。\n
*/
// ===========================================================================
void AoSysMsgStart(AOE_SYS_MSG_ID id, AOE_SYS_MSG_SELECT select)
{
	if (((u32)id >= AOD_SYS_MSG_NUM) ||
		((u32)select >= AOD_SYS_MSG_SELECT_NUM) ||
		(g_ao_sys_msg_file == NULL))
	{
		amAssert(0);
		return;
	}
	aoSysMsgStart(
		AoMsgGetStr8(
			g_ao_sys_msg_file, aoSysMsgGetSysMsgId(id), 0),
		select);
}

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
void AoSysMsgStart(const void* file, u32 id, AOE_SYS_MSG_SELECT select)
{
	if ((file == NULL) || (id >= AoMsgGetMsgNum(file)) ||
		((u32)select >= AOD_SYS_MSG_SELECT_NUM))
	{
		amAssert(0);
		return;
	}
	aoSysMsgStart(AoMsgGetStr8(file, id, 0), select);
}

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
void AoSysMsgStart(AOT_SYS_MSG_CODE str, AOE_SYS_MSG_SELECT select)
{
	if ((str == NULL) || ((u32)select >= AOD_SYS_MSG_SELECT_NUM)) {
		amAssert(0);
		return;
	}
	aoSysMsgStart(str, select);
}

// ===========================================================================
//	AoSysMsgStartSceError
/*!
	SCE関数エラー表示システムメッセージ開始

	@param error	[in] SCEエラーコード
*/
// ===========================================================================
void AoSysMsgStartSceError(u32 error)
{
	if ((int)error >= 0) {
		amAssert(0);
		return;
	}
	aoSysMsgStart((int)error);
}

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
void AoSysMsgCancel(void)
{
	if (AoSysMsgIsFinished()) {
		amAssert(0);
		return;
	}
	if (g_ao_sys_msg_tcb) {
		AOS_SYS_MSG_WORK* work =
			(AOS_SYS_MSG_WORK*)amTaskGetWork(g_ao_sys_msg_tcb);
		work->is_cancel = TRUE;
	}
}

// ===========================================================================
//	AoSysMsgIsFinished
/*!
	システムメッセージ表示完了判定

	@return 真：完了　偽：表示中
	@note
	AoSysMsgStart関数で開始したメッセージ表示が完了したかどうかを判定します。\n
*/
// ===========================================================================
BOOL AoSysMsgIsFinished(void)
{
	if (g_ao_sys_msg_tcb) {
		return FALSE;
	}
	return TRUE;
}

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
AOE_SYS_MSG_RESULT AoSysMsgGetResult(void)
{
	if (!AoSysMsgIsFinished()) {
		amAssert(0);
		return AOD_SYS_MSG_RESULT_NONE;
	}
	return g_ao_sys_msg_result;
}

// ===========================================================================
//	AoSysMsgIsShow
/*!
	システムメッセージ表示中判定

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
BOOL AoSysMsgIsShow(void)
{
	return AoSysMsgIsShowReal();
}

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
BOOL AoSysMsgIsShowReal(void)
{
	if (g_ao_sys_msg_tcb) {
		return ((AOS_SYS_MSG_WORK*)amTaskGetWork(g_ao_sys_msg_tcb))->is_show;
	}
	return FALSE;
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ===========================================================================
//! システムメッセージ開始
// ===========================================================================
void aoSysMsgStart(AOT_SYS_MSG_CODE str, AOE_SYS_MSG_SELECT select)
{
	g_ao_sys_msg_tcb = amTaskMake(aoSysMsgTask00, NULL, 0, 0, 0, "aoSysMsg");

	AOS_SYS_MSG_WORK* work =
		(AOS_SYS_MSG_WORK*)amTaskGetWork(g_ao_sys_msg_tcb);

	work->sce_error = 0;
	work->str = str;
	work->select = select;
	work->is_cancel = FALSE;
	work->is_show = FALSE;

	amTaskStart(g_ao_sys_msg_tcb);
}

// ===========================================================================
//! システムメッセージ開始
// ===========================================================================
void aoSysMsgStart(int sce_error)
{
	g_ao_sys_msg_tcb = amTaskMake(aoSysMsgTask00, NULL, 0, 0, 0, "aoSysMsg");

	AOS_SYS_MSG_WORK* work =
		(AOS_SYS_MSG_WORK*)amTaskGetWork(g_ao_sys_msg_tcb);

	work->sce_error = sce_error;
	work->str = NULL;
	work->select = AOD_SYS_MSG_SELECT_OK;
	work->is_cancel = FALSE;
	work->is_show = FALSE;

	amTaskStart(g_ao_sys_msg_tcb);
}

// ===========================================================================
// タスクプロシージャ00
// ===========================================================================
void aoSysMsgTask00(AMS_TCB* tcb)
{
	AOS_SYS_MSG_WORK* work = (AOS_SYS_MSG_WORK*)amTaskGetWork(tcb);

	// キャンセル判定
	if (work->is_cancel) {
		g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_SYS_CANCEL;
		amTaskDelete(tcb);
		g_ao_sys_msg_tcb = NULL;
		return;
	}

	// メッセージウインドウ表示
	int ret;
	if (work->sce_error < 0) {
		ret = cellMsgDialogOpenErrorCode(
			(u32)work->sce_error, aoSysMsgCbMsgDlg, work, NULL);
	}
	else {
		ret = cellMsgDialogOpen2(
			CELL_MSGDIALOG_TYPE_SE_TYPE_NORMAL |
			g_ao_sys_msg_select_tbl[work->select],
			work->str,
			aoSysMsgCbMsgDlg, work, NULL);
	}

	// 別のメッセージウインドウが開いているなら閉じるまで繰り返し
	if (ret == (int)CELL_SYSUTIL_ERROR_BUSY) {
		return;
	}

	// 終了待ちへ遷移
	work->is_show = TRUE;
	amTaskSetProcedure(tcb, aoSysMsgTask01);
}

// ===========================================================================
// タスクプロシージャ01
// ===========================================================================
void aoSysMsgTask01(AMS_TCB* tcb)
{
	AOS_SYS_MSG_WORK* work = (AOS_SYS_MSG_WORK*)amTaskGetWork(tcb);

	// キャンセル判定
	if (work->is_cancel &&
		(g_ao_sys_msg_result != AOD_SYS_MSG_RESULT_SYS_CANCEL))
	{
		cellMsgDialogClose(0.0f);
		g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_SYS_CANCEL;
	}
}

// ===========================================================================
//! メッセージダイアログ終了コールバック
// ===========================================================================
void aoSysMsgCbMsgDlg(int button_type, void *userdata)
{
	AOS_SYS_MSG_WORK* work = (AOS_SYS_MSG_WORK*)userdata;

	// システムキャンセルなら何もしない
	if (work->is_cancel) {
		g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_SYS_CANCEL;
	}

	// 選択項目取得
	else {
		switch (work->select) {
		case AOD_SYS_MSG_SELECT_OK:
			g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_OK;
			break;

		case AOD_SYS_MSG_SELECT_OKCANCEL:
			if (button_type == CELL_MSGDIALOG_BUTTON_OK) {
				g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_OK;
			}
			else {
				g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_CANCEL;
			}
			break;

		case AOD_SYS_MSG_SELECT_YESNO:
			if (button_type == CELL_MSGDIALOG_BUTTON_YES) {
				g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_YES;
			}
			else {
				g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_NO;
			}
			break;

		case AOD_SYS_MSG_SELECT_YESNOCANCEL:
			if (button_type == CELL_MSGDIALOG_BUTTON_YES) {
				g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_YES;
			}
			else if (button_type == CELL_MSGDIALOG_BUTTON_NO) {
				g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_NO;
			}
			else {
				g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_CANCEL;
			}
			break;

		default:
			amAssert(0);
			g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_SYS_CANCEL;
			break;
		}
	}

	// タスク削除
	if (g_ao_sys_msg_tcb) {
		amTaskDelete(g_ao_sys_msg_tcb);
		g_ao_sys_msg_tcb = NULL;
	}
}

// ===========================================================================
// システムメッセージのメッセージID取得
// ===========================================================================
u32 aoSysMsgGetSysMsgId(AOE_SYS_MSG_ID id)
{
	return (u32)id;
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
