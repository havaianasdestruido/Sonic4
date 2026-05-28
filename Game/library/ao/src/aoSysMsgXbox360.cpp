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

#if defined(AOD_PLATFORM_XBOX360)

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）

// ===========================================================================
//! 選択項目タイプ列挙
// ===========================================================================
typedef enum tag_AOE_SYS_MSG_SEL_TYPE {
	AOD_SYS_MSG_SEL_TYPE_OK		= 0,	//!< OK
	AOD_SYS_MSG_SEL_TYPE_CANCEL,		//!< キャンセル
	AOD_SYS_MSG_SEL_TYPE_YES,			//!< はい
	AOD_SYS_MSG_SEL_TYPE_NO,			//!< いいえ

	AOD_SYS_MSG_SEL_TYPE_NUM,			//!< タイプ数
} AOE_SYS_MSG_SEL_TYPE;

// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//! タスクワーク
// ===========================================================================
typedef struct tag_AOS_SYS_MSG_WORK {
	XOVERLAPPED			ov;				//!< 非同期制御用
	MESSAGEBOX_RESULT	result;			//!< メッセージ結果
	AOT_MSG_U16_CODE	str;			//!< メッセージ文字列
	AOE_SYS_MSG_ID		sys_id;			//!< メッセージID
	AOE_SYS_MSG_SELECT	select;			//!< 選択項目
	BOOL				is_cancel;		//!< 真：キャンセル
	BOOL				is_show;		//!< 真：表示中
} AOS_SYS_MSG_WORK;

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

static void aoSysMsgStart(
	AOT_MSG_U16_CODE str, AOE_SYS_MSG_ID sys_id, AOE_SYS_MSG_SELECT select);

static void aoSysMsgTask00(AMS_TCB* tcb);
static void aoSysMsgTask01(AMS_TCB* tcb);

static u32 aoSysMsgGetTitleMsgId(void);
static u32 aoSysMsgGetSelectMsgId(AOE_SYS_MSG_SEL_TYPE select);
static u32 aoSysMsgGetSysMsgId(AOE_SYS_MSG_ID id);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
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
//!	メッセージフラグ配列
// ===========================================================================
static const DWORD g_ao_sys_msg_flag_tbl[AOD_SYS_MSG_NUM] = {
	XMB_WARNINGICON,
	XMB_WARNINGICON,
	XMB_WARNINGICON,
	0,
	0,
	XMB_ERRORICON,
	XMB_ERRORICON,
	0,
	XMB_ERRORICON,
	0,
	XMB_ERRORICON,
	XMB_WARNINGICON,
};

// ===========================================================================
//! 選択項目数配列
// ===========================================================================
static const u32 g_ao_sys_msg_select_num_tbl[AOD_SYS_MSG_SELECT_NUM] = {
	1, 2, 2, 3,
};

// ===========================================================================
//! 「OK」選択項目
// ===========================================================================
static const AOE_SYS_MSG_SEL_TYPE g_ao_sys_msg_sel_o[1] = {
	AOD_SYS_MSG_SEL_TYPE_OK,
};

// ===========================================================================
//! 「OK & キャンセル」選択項目
// ===========================================================================
static const AOE_SYS_MSG_SEL_TYPE g_ao_sys_msg_sel_oc[2] = {
	AOD_SYS_MSG_SEL_TYPE_OK,
	AOD_SYS_MSG_SEL_TYPE_CANCEL,
};

// ===========================================================================
//! 「はい & いいえ」選択項目
// ===========================================================================
static const AOE_SYS_MSG_SEL_TYPE g_ao_sys_msg_sel_yn[2] = {
	AOD_SYS_MSG_SEL_TYPE_YES,
	AOD_SYS_MSG_SEL_TYPE_NO,
};

// ===========================================================================
//! 「はい & いいえ & キャンセル」選択項目
// ===========================================================================
static const AOE_SYS_MSG_SEL_TYPE g_ao_sys_msg_sel_ync[3] = {
	AOD_SYS_MSG_SEL_TYPE_YES,
	AOD_SYS_MSG_SEL_TYPE_NO,
	AOD_SYS_MSG_SEL_TYPE_CANCEL,
};

// ===========================================================================
//! 選択項目配列
// ===========================================================================
static const AOE_SYS_MSG_SEL_TYPE*
	g_ao_sys_msg_sel_tbl[AOD_SYS_MSG_SELECT_NUM] =
{
	g_ao_sys_msg_sel_o,
	g_ao_sys_msg_sel_oc,
	g_ao_sys_msg_sel_yn,
	g_ao_sys_msg_sel_ync,
};

// ===========================================================================
//! 「OK」選択項目結果
// ===========================================================================
static const AOE_SYS_MSG_RESULT g_ao_sys_msg_ret_o[1] = {
	AOD_SYS_MSG_RESULT_OK,
};

// ===========================================================================
//! 「OK & キャンセル」選択項目結果
// ===========================================================================
static const AOE_SYS_MSG_RESULT g_ao_sys_msg_ret_oc[2] = {
	AOD_SYS_MSG_RESULT_OK,
	AOD_SYS_MSG_RESULT_CANCEL,
};

// ===========================================================================
//! 「はい & いいえ」選択項目結果
// ===========================================================================
static const AOE_SYS_MSG_RESULT g_ao_sys_msg_ret_yn[2] = {
	AOD_SYS_MSG_RESULT_YES,
	AOD_SYS_MSG_RESULT_NO,
};

// ===========================================================================
//! 「はい & いいえ & キャンセル」選択項目結果
// ===========================================================================
static const AOE_SYS_MSG_RESULT g_ao_sys_msg_ret_ync[3] = {
	AOD_SYS_MSG_RESULT_YES,
	AOD_SYS_MSG_RESULT_NO,
	AOD_SYS_MSG_RESULT_CANCEL,
};

// ===========================================================================
//! 選択項目結果配列
// ===========================================================================
static const AOE_SYS_MSG_RESULT*
	g_ao_sys_msg_ret_tbl[AOD_SYS_MSG_SELECT_NUM] =
{
	g_ao_sys_msg_ret_o,
	g_ao_sys_msg_ret_oc,
	g_ao_sys_msg_ret_yn,
	g_ao_sys_msg_ret_ync,
};

// ===========================================================================
//! 選択項目文字列ポインタ配列
// ===========================================================================
static const wchar_t* g_ao_sys_msg_select_str_tbl[3] = { 0 };

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
		amAssert(AoMsgGetCodeType(file) == AOD_MSG_CODE_UTF16BE);
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
		AoMsgGetStr16(
			g_ao_sys_msg_file, aoSysMsgGetSysMsgId(id), 0),
		id, select);
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
	aoSysMsgStart(AoMsgGetStr16(file, id, 0), AOD_SYS_MSG_NONE, select);
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
	aoSysMsgStart(str, AOD_SYS_MSG_NONE, select);
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
void aoSysMsgStart(
	AOT_MSG_U16_CODE str, AOE_SYS_MSG_ID sys_id, AOE_SYS_MSG_SELECT select)
{
	amAssert(g_ao_sys_msg_tcb == NULL);

	g_ao_sys_msg_tcb = amTaskMake(aoSysMsgTask00, NULL, 0, 0, 0, "aoSysMsg");

	AOS_SYS_MSG_WORK* work =
		(AOS_SYS_MSG_WORK*)amTaskGetWork(g_ao_sys_msg_tcb);

	work->sys_id = sys_id;
	work->str = str;
	work->select = select;
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
	}

	// 表示可能判定
	else if (!AoSysIsShowPlatformUI()) {

		u32 sel_num = g_ao_sys_msg_select_num_tbl[work->select];

		amAssert(g_ao_sys_msg_file);

		// 選択項目配列作成
		for (u32 i = 0; i < sel_num; ++i) {
			g_ao_sys_msg_select_str_tbl[i] = AoMsgGetStr16(
				g_ao_sys_msg_file,
				aoSysMsgGetSelectMsgId(g_ao_sys_msg_sel_tbl[work->select][i]),
				0);
		}

		// フラグ作成
		DWORD flag = 0;
		if ((u32)work->sys_id < (u32)AOD_SYS_MSG_NUM) {
			flag = g_ao_sys_msg_flag_tbl[work->sys_id];
		}

		// 表示
		u32 def_sel = 0;
		if (sel_num > 0) {
			def_sel = (u32)(sel_num - 1);
		}
		amZeroMemory(&work->ov, sizeof(XOVERLAPPED));
		DWORD result = XShowMessageBoxUI(
			XUSER_INDEX_ANY,
			AoMsgGetStr16(g_ao_sys_msg_file, aoSysMsgGetTitleMsgId(), 0),
			work->str,
			sel_num,
			g_ao_sys_msg_select_str_tbl,
			def_sel,
			flag,
			&work->result,
			&work->ov);
		if (result == ERROR_IO_PENDING) {
			work->is_show = TRUE;
			amTaskSetProcedure(tcb, aoSysMsgTask01);
		}
	}
}

// ===========================================================================
// タスクプロシージャ01
// ===========================================================================
void aoSysMsgTask01(AMS_TCB* tcb)
{
	AOS_SYS_MSG_WORK* work = (AOS_SYS_MSG_WORK*)amTaskGetWork(tcb);

	// メッセージ表示完了待ち
	if (XHasOverlappedIoCompleted(&work->ov)) {

		if (work->is_cancel) {
			g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_SYS_CANCEL;
			amTaskDelete(tcb);
			g_ao_sys_msg_tcb = NULL;
			return;
		}

		// 結果取得
		if (XGetOverlappedResult(&work->ov, NULL, TRUE) == ERROR_SUCCESS) {
			u32 index = work->result.dwButtonPressed;
			if (g_ao_sys_msg_select_num_tbl[work->select] > 0) {
				if (index >= g_ao_sys_msg_select_num_tbl[work->select]) {
					index = g_ao_sys_msg_select_num_tbl[work->select] - 1;
				}
				g_ao_sys_msg_result =
					g_ao_sys_msg_ret_tbl[work->select][index];
			}
			else {
				g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_SYS_CANCEL;
			}
		}
		else {
			g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_SYS_CANCEL;
		}

		// 終了
		amTaskDelete(tcb);
		g_ao_sys_msg_tcb = NULL;
	}
}

// ===========================================================================
// タイトル文字のメッセージID取得
// ===========================================================================
u32 aoSysMsgGetTitleMsgId(void)
{
	return 0;
}

// ===========================================================================
// 選択項目のメッセージID取得
// ===========================================================================
u32 aoSysMsgGetSelectMsgId(AOE_SYS_MSG_SEL_TYPE select)
{
	return (u32)select + 1;
}

// ===========================================================================
// システムメッセージのメッセージID取得
// ===========================================================================
u32 aoSysMsgGetSysMsgId(AOE_SYS_MSG_ID id)
{
	return (u32)id + (u32)AOD_SYS_MSG_SEL_TYPE_NUM + 1;
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
