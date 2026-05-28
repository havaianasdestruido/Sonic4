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

#if defined(AOD_PLATFORM_WII)

#include <revolution/enc.h>

// ----- Macros ------------------------------------------------（マクロ定義）

#define AOD_SYS_MSG_EFCT_TIME_IN		(16)	//!< 入り演出時間
#define AOD_SYS_MSG_EFCT_TIME_DECIDE	(16)	//!< 決定演出時間
#define AOD_SYS_MSG_EFCT_TIME_OUT		(16)	//!< 退出演出時間

#define AOD_SYS_MSG_FONT_SCALE			(1.0f)	//!< 文字描画スケール
#define AOD_SYS_MSG_FONT_CDIFF			(1.0f)	//!< 文字描画文字間スペース
#define AOD_SYS_MSG_FONT_LDIFF			(0.0f)	//!< 文字描画行間スペース
#define AOD_SYS_MSG_WIN_MARGIN			(16.0f)	//!< 文字ウインドウスペース
#define AOD_SYS_MSG_SEL_MARGIN			(48.0f)	//!< 選択項目間スペース
#define AOD_SYS_MSG_SFRM_MARGIN			(8.0f)	//!< 選択選択項目枠スペース

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
	const char*			str;			//!< メッセージ文字列
	s32					sel_no;			//!< 選択番号
	AOE_SYS_MSG_SELECT	select;			//!< 選択項目
	BOOL				is_cancel;		//!< 真：キャンセル
	u32					count;			//!< 汎用カウンタ
	char*				str_buf;		//!< 文字列バッファ

	u32					win_tex_id;		//!< ウインドウテクスチャID
	AOS_TEXTURE			win_tex;		//!< ウインドウテクスチャ
} AOS_SYS_MSG_WORK;

// ===========================================================================
//! 描画タスクワーク
// ===========================================================================
typedef struct tag_AOS_SYS_MSG_DRAW {
	u16					efct;		//!< 入り演出進行度
	u16					pad[1];		//!< パディング
	u32					sel_no;		//!< 選択番号
	AOE_SYS_MSG_SELECT	select;		//!< 選択項目
	u32					sel_count;	//!< 選択演出カウント
	const char*			str;		//!< 描画文字列

	NNS_TEXLIST*		win_tex;	//!< ウインドウテクスチャ
	u32					win_tex_id;	//!< ウインドウテクスチャID
} AOS_SYS_MSG_DRAW;

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

static void aoSysMsgStart(const char* str, AOE_SYS_MSG_SELECT select);
static void aoSysMsgStart(const char* str, int dec, AOE_SYS_MSG_SELECT select);

static void aoSysMsgTaskWait(AMS_TCB* tcb);
static void aoSysMsgTaskBuilding(AMS_TCB* tcb);
static void aoSysMsgTaskEfctIn(AMS_TCB* tcb);
static void aoSysMsgTaskSelect(AMS_TCB* tcb);
static void aoSysMsgTaskEfctDecide(AMS_TCB* tcb);
static void aoSysMsgTaskEfctOut(AMS_TCB* tcb);
static void aoSysMsgTaskFinish(AMS_TCB* tcb);

static void aoSysMsgDrawTask(AMS_TCB* tcb);
static void aoSysMsgDrawRect(f32 x, f32 y, f32 w, f32 h, u32 color);

static u32 aoSysMsgGetSelMsgId(AOE_SYS_MSG_SEL_TYPE type);
static u32 aoSysMsgGetSysMsgId(AOE_SYS_MSG_ID id);

static void aoSysMsgIsShowWaitCountTaskProcedure(AMS_TCB* tcb);
static void aoSysMsgIsShowWaitCountTaskDestructor(AMS_TCB* tcb);

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
//! FATALエラーメッセージファイル
// ===========================================================================
static const void* g_ao_sys_msg_fatal_file = NULL;

// ===========================================================================
//! タスクTCBポインタ
// ===========================================================================
static AMS_TCB* g_ao_sys_msg_tcb = NULL;

// ===========================================================================
//! 描画タスク優先度
// ===========================================================================
static u16 g_ao_sys_msg_draw_task_prio = AOD_SYS_MSG_DRAW_TASK_PRIO;

// ===========================================================================
//! ウインドウテクスチャAMB
// ===========================================================================
static void* g_ao_sys_msg_win_tex_amb = NULL;

// ===========================================================================
//! ウインドウテクスチャID
// ===========================================================================
static u32 g_ao_sys_msg_win_tex_id = 0;

// ===========================================================================
//! 決定キー
// ===========================================================================
static u16 g_ao_sys_msg_key_decide = KEY_R_RIGHT;

// ===========================================================================
//! キャンセルキー
// ===========================================================================
static u16 g_ao_sys_msg_key_cancel = KEY_R_DOWN;

// ===========================================================================
//! 左キー
// ===========================================================================
static u16 g_ao_sys_msg_key_left = KEY_L_UP;

// ===========================================================================
//! 右キー
// ===========================================================================
static u16 g_ao_sys_msg_key_right = KEY_L_DOWN;

// ===========================================================================
//! 選択項目数配列
// ===========================================================================
static const u32 g_ao_sys_msg_select_num_tbl[AOD_SYS_MSG_SELECT_NUM] = {
	1, 2, 2, 3, 0,
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
	NULL,
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
	NULL,
};

// ===========================================================================
//! メッセージ非表示結果返し待ち時間
// ===========================================================================
static const u32 g_ao_sys_msg_is_show_wait_time = 8;

// ===========================================================================
//! メッセージ非表示結果返し待ちカウント
// ===========================================================================
static u32 g_ao_sys_msg_is_show_wait_count = g_ao_sys_msg_is_show_wait_time;

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
//	AoSysMsgSetFatalMsgFile
/*!
	FATALエラーメッセージファイル設定

	@param file		[in] FATALエラーメッセージファイル
*/
// ===========================================================================
void AoSysMsgSetFatalMsgFile(const void* file)
{
	if (file) {
		amAssert(AoMsgGetCodeType(file) == AOD_MSG_CODE_UTF16BE);
		g_ao_sys_msg_fatal_file = file;
	}
	else {
		g_ao_sys_msg_fatal_file = NULL;
	}
}

// ===========================================================================
//	AoSysMsgGetFatalMsgFile
/*!
	FATALエラーメッセージファイル取得

	@return FATALエラーメッセージファイル
*/
// ===========================================================================
const void* AoSysMsgGetFatalMsgFile(void)
{
	return g_ao_sys_msg_fatal_file;
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
	システムメッセージ開始(数値指定あり)

	@param id		[in] メッセージID
	@param dec		[in] メッセージに挿入する数値
	@param select	[in] 選択項目
*/
// ===========================================================================
void AoSysMsgStart(AOE_SYS_MSG_ID id, int dec, AOE_SYS_MSG_SELECT select)
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
		dec, select);
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
void AoSysMsgStart(const char* str, AOE_SYS_MSG_SELECT select)
{
	if ((str == NULL) || ((u32)select >= AOD_SYS_MSG_SELECT_NUM)) {
		amAssert(0);
		return;
	}
	aoSysMsgStart(str, select);
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
	if (g_ao_sys_msg_is_show_wait_count >= g_ao_sys_msg_is_show_wait_time) {
		return AoSysMsgIsShowReal();
	}
	return TRUE;
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
	if (AoSysMsgIsFinished()) {
		return FALSE;
	}
	return TRUE;
}

// ===========================================================================
//	AoSysMsgSetDrawTaskPrio
/*!
	メッセージ描画タスク優先度設定

	@param prio		[in] メッセージ描画タスク優先度
	@note
	この関数はWii版でのみ有効となります。\n
	他のプラットフォームでも呼び出すこと自体は問題ありません。\n
*/
// ===========================================================================
void AoSysMsgSetDrawTaskPrio(u16 prio)
{
	g_ao_sys_msg_draw_task_prio = prio;
}

// ===========================================================================
//	AoSysMsgSetWinTexAmb
/*!
	ウインドウテクスチャAMB設定

	@param texlist	[in] テクスチャAMB
	@param tex_id	[in] テクスチャID
*/
// ===========================================================================
void AoSysMsgSetWinTexAmb(void* amb, u32 tex_id)
{
	g_ao_sys_msg_win_tex_amb = amb;
	g_ao_sys_msg_win_tex_id = tex_id;
}

// ===========================================================================
//	AoSysMsgGetDrawTaskPrio
/*!
	メッセージ描画タスク優先度取得

	@return メッセージ描画タスク優先度
*/
// ===========================================================================
u16 AoSysMsgGetDrawTaskPrio(void)
{
	return g_ao_sys_msg_draw_task_prio;
}

// ===========================================================================
//	AoSysMsgSetKeyDecide
/*!
	決定キー設定

	@param key		[in] 決定キー
*/
// ===========================================================================
void AoSysMsgSetKeyDecide(u32 key)
{
	g_ao_sys_msg_key_decide = (u16)key;
}

// ===========================================================================
//	AoSysMsgSetKeyCancel
/*!
	キャンセルキー設定

	@param key		[in] キャンセルキー
*/
// ===========================================================================
void AoSysMsgSetKeyCancel(u32 key)
{
	g_ao_sys_msg_key_cancel = (u16)key;
}

// ===========================================================================
//	AoSysMsgSetKeyLeft
/*!
	左キー設定

	@param key		[in] 左キー
*/
// ===========================================================================
void AoSysMsgSetKeyLeft(u32 key)
{
	g_ao_sys_msg_key_left = (u16)key;
}

// ===========================================================================
//	AoSysMsgSetKeyRight
/*!
	右キー設定

	@param key		[in] 右キー
*/
// ===========================================================================
void AoSysMsgSetKeyRight(u32 key)
{
	g_ao_sys_msg_key_right = (u16)key;
}

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
void AoSysMsgShowFatalError(AOE_SYS_MSG_FATAL_ID id)
{
	// メッセージ取得
	if ((u32)id >= AOD_SYS_MSG_FATAL_NUM) {
		id = AOD_SYS_MSG_FATAL_WARE_01;
	}
	const u16* str16 = AoMsgGetStr16(g_ao_sys_msg_fatal_file, (u32)id, 0);

	// 文字コード変換
	char str[1024];
	s32 len = 1024;
	if (OSGetFontEncode() == OS_FONT_ENCODE_ANSI) {
		ENCConvertStringUnicodeToLatin1((u8*)str, &len, str16, NULL);
	}
	else {
		ENCConvertStringUnicodeToSjis((u8*)str, &len, str16, NULL);
	}
	str[len] = '\0';

	// メッセージ描画
	GXColor fg;
	GXColor bg;
	fg.r = 255;
	fg.g = 255;
	fg.b = 255;
	fg.a = 255;
	bg.r = 0;
	bg.g = 0;
	bg.b = 0;
	bg.a = 255;
	OSFatal(fg, bg, str);
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ===========================================================================
//! システムメッセージ開始
// ===========================================================================
void aoSysMsgStart(const char* str, AOE_SYS_MSG_SELECT select)
{
	g_ao_sys_msg_tcb = amTaskMake(aoSysMsgTaskWait, NULL, 0, 0, 0, "aoSysMsg");

	AOS_SYS_MSG_WORK* work =
		(AOS_SYS_MSG_WORK*)amTaskGetWork(g_ao_sys_msg_tcb);

	work->str = str;
	if (g_ao_sys_msg_select_num_tbl[select] > 0) {
		work->sel_no = (s32)(g_ao_sys_msg_select_num_tbl[select] - 1);
	}
	else {
		work->sel_no = 0;
	}
	work->select = select;
	work->is_cancel = FALSE;
	work->count = 0;
	work->str_buf = NULL;
	work->win_tex_id = g_ao_sys_msg_win_tex_id;

	amTaskStart(g_ao_sys_msg_tcb);

#if AMD_DEBUG
	// チェック
	if (!AoFontIsBuilded()) {
		printf("aoSysMsg::Warning aoFont is not builded.\n");
	}
	if (!g_ao_sys_msg_win_tex_amb) {
		printf("aoSysMsg::Warning Window Texture nothing.\n");
	}
#endif // AMD_DEBUG
}

// ===========================================================================
//! システムメッセージ開始
// ===========================================================================
void aoSysMsgStart(
	const char* str, int dec, AOE_SYS_MSG_SELECT select)
{
	g_ao_sys_msg_tcb = amTaskMake(aoSysMsgTaskWait, NULL, 0, 0, 0, "aoSysMsg");

	AOS_SYS_MSG_WORK* work =
		(AOS_SYS_MSG_WORK*)amTaskGetWork(g_ao_sys_msg_tcb);

	work->str = str;
	if (g_ao_sys_msg_select_num_tbl[select] > 0) {
		work->sel_no = (s32)(g_ao_sys_msg_select_num_tbl[select] - 1);
	}
	else {
		work->sel_no = 0;
	}
	work->select = select;
	work->is_cancel = FALSE;
	work->count = 0;
	work->str_buf = NULL;
	work->win_tex_id = g_ao_sys_msg_win_tex_id;

	// 文字数算出
	u32 str_len = (u32)strlen(str);

	// バッファ確保
	work->str_buf = (char*)amMemAlloc(sizeof(char) * (str_len + 32));
	amCopyMemory(work->str_buf, str, sizeof(char) * str_len);

	// 数値設定
	sprintf(&work->str_buf[str_len], "\n(%d)", dec);

	work->str = work->str_buf;

	amTaskStart(g_ao_sys_msg_tcb);

#if AMD_DEBUG
	// チェック
	if (!AoFontIsBuilded()) {
		printf("aoSysMsg::Warning aoFont is not builded.\n");
	}
	if (!g_ao_sys_msg_win_tex_amb) {
		printf("aoSysMsg::Warning Window Texture nothing.\n");
	}
#endif // AMD_DEBUG
}

// ===========================================================================
//! 待ち
// ===========================================================================
void aoSysMsgTaskWait(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_SYS_MSG_WORK* work = (AOS_SYS_MSG_WORK*)amTaskGetWork(tcb);

	// フォントが構築され、テクスチャが設定されるまで待機
	if (AoFontIsBuilded() && g_ao_sys_msg_win_tex_amb) {

		// テクスチャ構築開始
		AoTexBuild(&work->win_tex, g_ao_sys_msg_win_tex_amb);
		AoTexLoad(&work->win_tex);

		// 構築開始
		work->count = 0;
		amTaskSetProcedure(tcb, aoSysMsgTaskBuilding);
	}
	else {
		work->count += 1;
		if (work->count >= (60 * 30)) {
			// 30秒たっても設定されないのであれば強制終了
			g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_SYS_CANCEL;
			amTaskDelete(tcb);
			g_ao_sys_msg_tcb = NULL;

			// 非表示結果待ちタスク生成
			g_ao_sys_msg_is_show_wait_count = 0;
			AMS_TCB* wait_tcb = amTaskMake(
				aoSysMsgIsShowWaitCountTaskProcedure,
				aoSysMsgIsShowWaitCountTaskDestructor,
				0, 0, 0, "aoSysMsgIsShowWait");
			amTaskStart(wait_tcb);
		}
	}
}

// ===========================================================================
//! 構築中
// ===========================================================================
void aoSysMsgTaskBuilding(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_SYS_MSG_WORK* work = (AOS_SYS_MSG_WORK*)amTaskGetWork(tcb);

	// テクスチャ構築判定
	if (AoTexIsLoaded(&work->win_tex)) {
		// 入り演出へ遷移
		work->count = 0;
		amTaskSetProcedure(tcb, aoSysMsgTaskEfctIn);
	}
}

// ===========================================================================
// 入り演出
// ===========================================================================
void aoSysMsgTaskEfctIn(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_SYS_MSG_WORK* work = (AOS_SYS_MSG_WORK*)amTaskGetWork(tcb);

	// キャンセル判定
	if (work->is_cancel) {
		// 終了へ遷移
		work->count = 0;
		amTaskSetProcedure(tcb, aoSysMsgTaskFinish);
		return;
	}

	// 描画
	AOS_SYS_MSG_DRAW* dwork =
		(AOS_SYS_MSG_DRAW*)amDrawMallocDataBuffer(sizeof(AOS_SYS_MSG_DRAW));
	dwork->efct = (u16)(
		(0x0000ffff * work->count) / AOD_SYS_MSG_EFCT_TIME_IN);
	dwork->sel_no = 0;
	dwork->select = work->select;
	dwork->sel_count = 0;
	dwork->str = work->str;
	dwork->win_tex = AoTexGetTexList(&work->win_tex);
	dwork->win_tex_id = work->win_tex_id;
	amDrawMakeTask(aoSysMsgDrawTask, g_ao_sys_msg_draw_task_prio, (u32)dwork);

	// 演出進行
	work->count += 1;
	if (work->count >= AOD_SYS_MSG_EFCT_TIME_IN) {
		// 選択中へ遷移
		work->count = 0;
		amTaskSetProcedure(tcb, aoSysMsgTaskSelect);
	}
}

// ===========================================================================
// 選択中
// ===========================================================================
void aoSysMsgTaskSelect(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_SYS_MSG_WORK* work = (AOS_SYS_MSG_WORK*)amTaskGetWork(tcb);

	// キャンセル判定
	if (work->is_cancel) {
		// 終了へ遷移
		work->count = 0;
		amTaskSetProcedure(tcb, aoSysMsgTaskFinish);
		return;
	}

	// 選択項目数取得
	u32 sel_num = g_ao_sys_msg_select_num_tbl[work->select];

	// 選択切り替え
	if (sel_num > 0) {
		if (AoPadMRepeat() & g_ao_sys_msg_key_left) {
			if ((u32)work->sel_no > 0) {
				work->sel_no -= 1;
				work->count = 0;
			}
		}
		if (AoPadMRepeat() & g_ao_sys_msg_key_right) {
			if ((u32)work->sel_no < (u32)(sel_num - 1)) {
				work->sel_no += 1;
				work->count = 0;
			}
		}
	}

	// 描画
	AOS_SYS_MSG_DRAW* dwork =
		(AOS_SYS_MSG_DRAW*)amDrawMallocDataBuffer(sizeof(AOS_SYS_MSG_DRAW));
	dwork->efct = 0xffff;
	dwork->sel_no = (u32)work->sel_no;
	dwork->select = work->select;
	dwork->sel_count = work->count;
	dwork->str = work->str;
	dwork->win_tex = AoTexGetTexList(&work->win_tex);
	dwork->win_tex_id = work->win_tex_id;
	amDrawMakeTask(aoSysMsgDrawTask, g_ao_sys_msg_draw_task_prio, (u32)dwork);

	// 演出進行
	work->count += 1;

	if (sel_num > 0) {
		// 決定判定
		if (AoPadMStand() & g_ao_sys_msg_key_decide) {
			// 決定演出へ遷移
			work->count = 0;
			amTaskSetProcedure(tcb, aoSysMsgTaskEfctOut);
		}

		// キャンセル判定
		else if (AoPadMStand() & g_ao_sys_msg_key_cancel) {

			work->sel_no = -1;

			// 退出演出へ遷移
			work->count = 0;
			amTaskSetProcedure(tcb, aoSysMsgTaskEfctOut);
		}
	}

#if defined(AOD_DEBUG)
	{
		// 文字幅が規定以上ならデバッグ文字で警告表示
		f32 str_w, str_h;
		AoFontUtilGetStringInfo(
			work->str, AOD_SYS_MSG_FONT_SCALE,
			AOD_SYS_MSG_FONT_CDIFF, AOD_SYS_MSG_FONT_LDIFF,
			&str_w, &str_h, NULL, NULL);
		if (str_w > ((f32)AOD_ACT_SCREEN_WIDTH * 0.84f)) {
			amPrint(8, 8, "WARNING : message outside the area.");
		}
	}
#endif // defined(AOD_DEBUG)
}

// ===========================================================================
// 決定演出
// ===========================================================================
void aoSysMsgTaskEfctDecide(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_SYS_MSG_WORK* work = (AOS_SYS_MSG_WORK*)amTaskGetWork(tcb);

	// キャンセル判定
	if (work->is_cancel) {
		// 終了へ遷移
		work->count = 0;
		amTaskSetProcedure(tcb, aoSysMsgTaskFinish);
		return;
	}

	// 描画
	AOS_SYS_MSG_DRAW* dwork =
		(AOS_SYS_MSG_DRAW*)amDrawMallocDataBuffer(sizeof(AOS_SYS_MSG_DRAW));
	dwork->efct = 0xffff;
	dwork->sel_no = (u32)work->sel_no;
	dwork->select = work->select;
	dwork->sel_count = (u32)(work->count * 4);
	dwork->str = work->str;
	dwork->win_tex = AoTexGetTexList(&work->win_tex);
	dwork->win_tex_id = work->win_tex_id;
	amDrawMakeTask(aoSysMsgDrawTask, g_ao_sys_msg_draw_task_prio, (u32)dwork);

	// 演出進行
	work->count += 1;
	if (work->count >= AOD_SYS_MSG_EFCT_TIME_DECIDE) {
		// 退出演出へ遷移
		work->count = 0;
		amTaskSetProcedure(tcb, aoSysMsgTaskEfctOut);
	}
}

// ===========================================================================
// 退出演出
// ===========================================================================
void aoSysMsgTaskEfctOut(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_SYS_MSG_WORK* work = (AOS_SYS_MSG_WORK*)amTaskGetWork(tcb);

	// キャンセル判定
	if (work->is_cancel) {
		// 終了へ遷移
		work->count = 0;
		amTaskSetProcedure(tcb, aoSysMsgTaskFinish);
		return;
	}

	// 演出進行
	work->count += 1;

	// 描画
	AOS_SYS_MSG_DRAW* dwork =
		(AOS_SYS_MSG_DRAW*)amDrawMallocDataBuffer(sizeof(AOS_SYS_MSG_DRAW));
	dwork->efct = (u16)(
		0x0000ffff - ((0x0000ffff * work->count) / AOD_SYS_MSG_EFCT_TIME_OUT));
	dwork->sel_no = 0;
	dwork->select = work->select;
	dwork->sel_count = 0;
	dwork->str = work->str;
	dwork->win_tex = AoTexGetTexList(&work->win_tex);
	dwork->win_tex_id = work->win_tex_id;
	amDrawMakeTask(aoSysMsgDrawTask, g_ao_sys_msg_draw_task_prio, (u32)dwork);

	// 演出進行
	work->count += 1;
	if (work->count >= AOD_SYS_MSG_EFCT_TIME_OUT) {
		// 終了へ遷移
		work->count = 0;
		amTaskSetProcedure(tcb, aoSysMsgTaskFinish);
	}
}

// ===========================================================================
// 終了
// ===========================================================================
void aoSysMsgTaskFinish(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_SYS_MSG_WORK* work = (AOS_SYS_MSG_WORK*)amTaskGetWork(tcb);

	// 何もせずに4フレーム待機してからテクスチャの解放を行い終了
	work->count += 1;
	if (work->count == 4) {
		// テクスチャ解放開始
		AoTexRelease(&work->win_tex);
	}
	if ((work->count >= 4) && AoTexIsReleased(&work->win_tex)) {

		// 結果格納
		if (work->is_cancel) {
			g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_SYS_CANCEL;
		}
		else {
			if (g_ao_sys_msg_select_num_tbl[work->select] > 0) {
				u32 result;
				if (work->sel_no >= 0) {
					result = (u32)work->sel_no;
				}
				else {
					result = (u32)(g_ao_sys_msg_select_num_tbl[work->select] - 1);
				}
				g_ao_sys_msg_result =
					g_ao_sys_msg_ret_tbl[work->select][result];
			}
			else {
				g_ao_sys_msg_result = AOD_SYS_MSG_RESULT_SYS_CANCEL;
			}
		}

		// 文字バッファ削除
		if (work->str_buf) {
			amMemFree(work->str_buf);
			work->str_buf = NULL;
		}

		// 終了
		amTaskDelete(tcb);
		g_ao_sys_msg_tcb = NULL;

		// 非表示結果待ちタスク生成
		g_ao_sys_msg_is_show_wait_count = 0;
		AMS_TCB* wait_tcb = amTaskMake(
			aoSysMsgIsShowWaitCountTaskProcedure,
			aoSysMsgIsShowWaitCountTaskDestructor,
			0, 0, 0, "aoSysMsgIsShowWait");
		amTaskStart(wait_tcb);
	}
}

// ===========================================================================
// 描画タスク
// ===========================================================================
void aoSysMsgDrawTask(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_SYS_MSG_DRAW* work = *((AOS_SYS_MSG_DRAW**)amTaskGetWork(tcb));

	if (work->efct == 0) {
		return;
	}

	// 文字列情報取得
	f32 str_w, str_h;
	AoFontUtilGetStringInfo(
		work->str, AOD_SYS_MSG_FONT_SCALE,
		AOD_SYS_MSG_FONT_CDIFF, AOD_SYS_MSG_FONT_LDIFF,
		&str_w, &str_h, NULL, NULL);

	// 選択肢数取得
	u32 sel_num = g_ao_sys_msg_select_num_tbl[work->select];

	// 選択肢文字列情報取得
	f32 sel_w[3];
	f32 sel_h[3];
	f32 sel_all_w = 0.0f;
	f32 sel_max_h = 0.0f;
	for (u32 i = 0; i < sel_num; ++i) {
		u32 sel_msg_id =
			aoSysMsgGetSelMsgId(g_ao_sys_msg_sel_tbl[work->select][i]);
		const char* str = AoMsgGetStr8(g_ao_sys_msg_file, sel_msg_id, 0);

		AoFontUtilGetStringInfo(
			str, AOD_SYS_MSG_FONT_SCALE,
			AOD_SYS_MSG_FONT_CDIFF, AOD_SYS_MSG_FONT_LDIFF,
			&sel_w[i], &sel_h[i], NULL, NULL);
		if (sel_h[i] > sel_max_h) {
			sel_max_h = sel_h[i];
		}
		sel_all_w += sel_w[i];
	}
	if (sel_num > 0) {
		sel_all_w += AOD_SYS_MSG_SEL_MARGIN * (f32)(sel_num - 1);
	}

	f32 msg_w = str_w;
	if (sel_all_w > msg_w) {
		msg_w = sel_all_w;
	}

	// ウインドウ描画
	f32 win_w = msg_w + (AOD_SYS_MSG_WIN_MARGIN * 2.0f);
	f32 win_h;
	if (sel_num > 0) {
		win_h = str_h + sel_max_h + (AOD_SYS_MSG_WIN_MARGIN * 4.0f);
	}
	else {
		win_h = str_h + (AOD_SYS_MSG_WIN_MARGIN * 2.0f);
	}
	if (work->efct < 0xffff) {
		f32 scale = (f32)work->efct / (f32)0xffff;
		win_w *= scale;
		win_h *= scale;
	}
	AoWinSysDraw(
		AOD_WIN_TYPE_A, work->win_tex, work->win_tex_id,
		(f32)AOD_ACT_SCREEN_WIDTH * 0.5f,
		(f32)AOD_ACT_SCREEN_HEIGHT * 0.5f,
		win_w, win_h);

	// 文字列描画
	f32 str_y =
		(((f32)AOD_ACT_SCREEN_HEIGHT - win_h) * 0.5f) + AOD_SYS_MSG_WIN_MARGIN;
	if (work->efct == 0xffff) {
		AoFontDrawString(
			(f32)AOD_ACT_SCREEN_WIDTH * 0.5f, str_y,
			AOD_FONT_ALIGN_CENTER, 0xffffffff,
			AOD_SYS_MSG_FONT_SCALE,
			AOD_SYS_MSG_FONT_CDIFF, AOD_SYS_MSG_FONT_LDIFF,
			(u32)AOD_ACT_CORW_CENTER, work->str);
	}

	// 選択項目描画
	if (work->efct == 0xffff) {
		f32 sel_x = ((f32)AOD_ACT_SCREEN_WIDTH - sel_all_w) * 0.5f;
		f32 sel_y = str_y + str_h + (AOD_SYS_MSG_WIN_MARGIN * 2.0f);
		for (u32 i = 0; i < sel_num; ++i) {

			u32 sel_msg_id =
				aoSysMsgGetSelMsgId(g_ao_sys_msg_sel_tbl[work->select][i]);
			const char* str = AoMsgGetStr8(g_ao_sys_msg_file, sel_msg_id, 0);

			// 選択しているなら枠表示
			if (i == work->sel_no) {
				s32 alpha = (s32)(work->sel_count % 64);
				if (alpha >= 32) {
					alpha = 64 - alpha;
				}
				alpha = 64 + (alpha * 4);
				u32 c = 0x00cf0000 | (u32)alpha;
				aoSysMsgDrawRect(
					sel_x - AOD_SYS_MSG_SFRM_MARGIN,
					sel_y - AOD_SYS_MSG_SFRM_MARGIN,
					sel_w[i] + (AOD_SYS_MSG_SFRM_MARGIN * 2.0f),
					sel_h[i] + (AOD_SYS_MSG_SFRM_MARGIN * 2.0f),
					c);
			}

			AoFontDrawString(
				sel_x, sel_y, AOD_FONT_ALIGN_LEFT, 0xffffffff,
				AOD_SYS_MSG_FONT_SCALE,
				AOD_SYS_MSG_FONT_CDIFF, AOD_SYS_MSG_FONT_LDIFF,
				(u32)AOD_ACT_CORW_CENTER, str);

			sel_x += sel_w[i] + AOD_SYS_MSG_SEL_MARGIN;
		}
	}
}

// ===========================================================================
// 矩形描画
// ===========================================================================
void aoSysMsgDrawRect(f32 x, f32 y, f32 w, f32 h, u32 color)
{
	// カメラ設定
	AoActDrawPre();

	// 描画ステート初期化
	amDrawPushState();
	amDrawInitState();

	// Z設定
	nnSetPrimitive2DZModeGC(GX_FALSE, GX_ALWAYS, GX_FALSE);

	// ブレンド設定
	nnSetPrimitiveBlend(NNE_PRIM_BLEND_BLEND);

	// フォグ設定
	amDrawSetFog(0);

	// テクスチャ設定
	nnSetPrimitiveTexNum(NULL, -1);

	// 頂点データ作成
	NNS_PRIM3D_PC v[4];

	// 座標
	v[0].Pos.x = v[1].Pos.x = x;
	v[2].Pos.x = v[3].Pos.x = x + w;
	v[0].Pos.y = v[2].Pos.y = y;
	v[1].Pos.y = v[3].Pos.y = y + h;
	v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = -2.0f;

	// 色
	v[0].Col = v[1].Col = v[2].Col = v[3].Col = color;

	// 頂点補正
	AoActDrawCorWide(v, 4, AOD_ACT_CORW_CENTER);

	// プリミティブ描画開始
	nnBeginDrawPrimitive3D(
		NNE_PRIM3D_FMT_PC,
		NNE_PRIM_ALPHABLEND_ON,
		NNE_PRIM_LIGHT_DISABLE,
		NNE_PRIM_CULL_NONE);

	// プリミティブ描画
	nnDrawPrimitive3D(NNE_PRIM_TRIANGLE_STRIP, v, 4);

	// プリミティブ描画終了
	nnEndDrawPrimitive3D();

	// 描画ステート復帰
	amDrawPopState();
}

// ===========================================================================
// 選択項目のメッセージID取得
// ===========================================================================
u32 aoSysMsgGetSelMsgId(AOE_SYS_MSG_SEL_TYPE type)
{
	return (u32)type;
}

// ===========================================================================
// システムメッセージのメッセージID取得
// ===========================================================================
u32 aoSysMsgGetSysMsgId(AOE_SYS_MSG_ID id)
{
	return (u32)id + (u32)AOD_SYS_MSG_SEL_TYPE_NUM;
}

// ===========================================================================
// メッセージ非表示結果返し待ちタスクプロシージャ
// ===========================================================================
void aoSysMsgIsShowWaitCountTaskProcedure(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	if (g_ao_sys_msg_is_show_wait_count < g_ao_sys_msg_is_show_wait_time) {
		g_ao_sys_msg_is_show_wait_count += 1;
	}
	else {
		amTaskDelete(tcb);
	}
}

// ===========================================================================
// メッセージ非表示結果返し待ちタスクデストラクタ
// ===========================================================================
void aoSysMsgIsShowWaitCountTaskDestructor(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	g_ao_sys_msg_is_show_wait_count = g_ao_sys_msg_is_show_wait_time;
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
