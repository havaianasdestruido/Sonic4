// ===========================================================================
/*!
	@file	aoMsg.cpp
	@brief	AoLibrary メッセージモジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"
#include "private/msg_file_format.h"

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//	AoMsgGetCodeType
/*!
	文字コード取得

	@param file		[in] メッセージファイル
	@return 文字コード
	@note
	メッセージファイルに含まれている文字の文字コードを返します。\n
	内部での文字コード変換機能はありませんので、
	使用できないコードが返されるようであれば、
	ファイルの作り方から見直しが必要となります。\n
	不正な引数が指定された場合は、アサートしAOD_MSG_CODE_NONEを返します。\n
*/
// ===========================================================================
AOE_MSG_CODE AoMsgGetCodeType(const void* file)
{
	if (file == NULL) {
		amAssert(0);
		return AOD_MSG_CODE_NONE;
	}

	// ヘッダ取得
	const AOS_MSG_FILE_HEADER* head = (const AOS_MSG_FILE_HEADER*)file;

	// 文字コードを返す
	return (AOE_MSG_CODE)head->code_type;
}

// ===========================================================================
//	AoMsgGetStrNum
/*!
	メッセージ数取得

	@param file		[in] メッセージファイル
	@return メッセージ数
	@note
	メッセージファイルに含まれるメッセージ数を返します。\n
	不正な引数が指定された場合は、アサートし0を返します。\n
*/
// ===========================================================================
u32 AoMsgGetMsgNum(const void* file)
{
	if (file == NULL) {
		amAssert(0);
		return 0;
	}

	// ヘッダ取得
	const AOS_MSG_FILE_HEADER* head = (const AOS_MSG_FILE_HEADER*)file;

	// メッセージ数を返す
	return head->msg_num;
}

// ===========================================================================
//	AoMsgGetStrNum
/*!
	文字列数取得

	@param file		[in] メッセージファイル
	@param msg_no	[in] メッセージ番号
	@return 文字列数
	@note
	指定のメッセージに含まれる文字列数を返します。\n
	不正な引数が指定された場合は、アサートし0を返します。\n
*/
// ===========================================================================
u32 AoMsgGetStrNum(const void* file, u32 msg_no)
{
	if (file == NULL) {
		amAssert(0);
		return 0;
	}

	// ヘッダ取得
	const AOS_MSG_FILE_HEADER* head = (const AOS_MSG_FILE_HEADER*)file;

	// メッセージ数チェック
	if (msg_no >= head->msg_num) {
		amAssert(0);
		return 0;
	}

	// メッセージ取得
	const AOS_MSG_FILE_MSG* msg =
		((const AOS_MSG_FILE_MSG*)((u32)file + head->msg_tbl_ofst)) + msg_no;

	// 文字列数を返す
	return msg->str_num;
}

// ===========================================================================
//	AoMsgGetStr
/*!
	文字列取得

	@param file		[in] メッセージファイル
	@param msg_no	[in] メッセージ番号
	@param str_no	[in] 文字列番号
	@return 文字列(終端文字あり)
	@note
	指定の文字列の先頭ポインタを返します。\n
	このポインタはAoMsgGetCodeType関数の返す文字コードの文字列の
	先頭ポインタとして扱えます。\n
	不正な引数が指定された場合は、アサートしNULLを返します。\n
*/
// ===========================================================================
const void* AoMsgGetStr(const void* file, u32 msg_no, u32 str_no)
{
	if (file == NULL) {
		amAssert(0);
		return 0;
	}

	// ヘッダ取得
	const AOS_MSG_FILE_HEADER* head = (const AOS_MSG_FILE_HEADER*)file;

	// メッセージ数チェック
	if (msg_no >= head->msg_num) {
		amAssert(0);
		return 0;
	}

	// メッセージ取得
	const AOS_MSG_FILE_MSG* msg =
		((const AOS_MSG_FILE_MSG*)((u32)file + head->msg_tbl_ofst)) + msg_no;

	// 文字列数チェック
	if (str_no >= msg->str_num) {
		amAssert(0);
		return 0;
	}

	// 文字列取得
	const AOS_MSG_FILE_STR* str =
		((const AOS_MSG_FILE_STR*)((u32)file + msg->str_tbl_ofst)) + str_no;

	// 文字列を返す
	return (const void*)((u32)file + str->str_ofst);
}

// ===========================================================================
//	AoMsgGetStr16
/*!
	文字列取得(16bit)

	@param file		[in] メッセージファイル
	@param msg_no	[in] メッセージ番号
	@param str_no	[in] 文字列番号
	@return 文字列(終端文字あり)
	@note
	返す値としては、AoMsgGetStrと全く同じとなり、型が違うのみとなります。\n
*/
// ===========================================================================
AOT_MSG_U16_CODE AoMsgGetStr16(const void* file, u32 msg_no, u32 str_no)
{
	return (AOT_MSG_U16_CODE)AoMsgGetStr(file, msg_no, str_no);
}

// ===========================================================================
//	AoMsgGetStr8
/*!
	文字列取得(8bit)

	@param file		[in] メッセージファイル
	@param msg_no	[in] メッセージ番号
	@param str_no	[in] 文字列番号
	@return 文字列(終端文字あり)
	@note
	返す値としては、AoMsgGetStrと全く同じとなり、型が違うのみとなります。\n
*/
// ===========================================================================
const char* AoMsgGetStr8(const void* file, u32 msg_no, u32 str_no)
{
	return (const char*)AoMsgGetStr(file, msg_no, str_no);
}

// ===========================================================================
//	AoMsgIsMsgFile
/*!
	メッセージファイル判定

	@param file				[in] 判定するファイル
	@return 真：メッセージファイル　偽：それ以外
	@note
	引数で指定したファイルがメッセージファイルであるか判定します。\n
	ヘッダ情報のみを判定するので、
	全てのデータが正常であるかの判定は行いません。\n
	AoMsgConvertAddress関数を呼び出した後のメッセージファイルであっても、
	メッセージファイルと判定されます。\n
*/
// ===========================================================================
BOOL AoMsgIsMsgFile(const void* file)
{
	// ヘッダ(識別子)のみチェック
	const char* masic = (const char*)file;
	const char* base = AOD_MSG_FILE_MASIC;
	if ((masic[1] == base[1]) &&
		(masic[2] == base[2]) &&
		(masic[3] == base[3]))
	{
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//	AoMsgConvertAddress
/*!
	アドレス変換

	@param file				[io] アドレス変換するメッセージファイル
	@return 1:正常に変換 0:既に変換済みorそれ以外
	@note
	引数で指定したメッセージファイルのアドレス変換を行います。\n
	正常に変換が完了した場合は1を返します。\n
	既に変換済みの場合は何も行わずに0を返します。\n
	メッセージファイル以外のファイルを指定した場合は、
	アサートし何も行わず0を返します。\n
*/
// ===========================================================================
s32 AoMsgConvertAddress(u8* file)
{
	if (!AoMsgIsMsgFile(file)) {
		amAssert(0);
		return 0;
	}

	// ヘッダ取得
	AOS_MSG_FILE_HEADER* head = (AOS_MSG_FILE_HEADER*)file;

	// 変換済み判定
	if (head->masic[0] == AMD_CONVERTED_MARK) {
		return 0;
	}

	// 変換済み設定
	head->masic[0] = AMD_CONVERTED_MARK;

	// エンディアンチェック
#if AMD_ENDIAN_TARGET == AMD_ENDIAN_LITTLE
	if (head->endian_type != AOD_MSG_ENDIAN_TYPE_LITTLE) {
		amAssert(0);
	}
#else
	if (head->endian_type != AOD_MSG_ENDIAN_TYPE_BIG) {
		amAssert(0);
	}
#endif // AMD_ENDIAN_TARGET == AMD_ENDIAN_LITTLE

	// 実際のアドレス変換は行わず、毎回オフセット計算で処理をする
	return 1;
}

// ===========================================================================
//	AoMsgRegAliceMsgConv
/*!
	Aliceへメッセージファイルのアドレス変換登録

	@note
	amConvertAddress関数でメッセージファイルのアドレス変換を行えるように、
	Aliceに登録します。\n
*/
// ===========================================================================
void AoMsgRegAliceMsgConv(void)
{
	amConvertRegist("ASG", AoMsgConvertAddress);
}

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
