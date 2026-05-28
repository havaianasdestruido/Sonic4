// ===========================================================================
/*!
	@file	aoYsdFile.cpp
	@brief	YSDファイルモジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "aoYsdFile.h"

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ***************************************************************************
// YSDファイルフォーマット
// ***************************************************************************
// ===========================================================================
//	struct YSDS_HEADER
// ---------------------------------------------------------------------------
//!	YSDファイルヘッダ構造体
// ===========================================================================
typedef struct tag_YSDS_HEADER {
	char			masic[4];			//!< ファイル識別子"#YSD"
	u32				page_num;			//!< ページ数
} YSDS_HEADER; // 8 byte

// ===========================================================================
//	struct YSDS_PAGE
// ---------------------------------------------------------------------------
//!	YSDページ情報構造体
// ===========================================================================
typedef struct tag_YSDS_PAGE {
	u32				time;				//!< 時間
	s32				show;				//!< 表示画像番号
	s32				hide;				//!< 非表示画像番号
	u32				option;				//!< オプション
	u32				line_num;			//!< 行数
	u32				line_tbl_ofst;		//!< 行配列先頭へのオフセット
} YSDS_PAGE; // 24 byte

// ===========================================================================
//	struct YSDS_LINE
// ---------------------------------------------------------------------------
//!	YSDライン情報構造体
// ===========================================================================
typedef struct tag_YSDS_LINE {
	u32				id;					//!< 種別ID
	u32				str_ofst;			//!< 文字列先頭へのオフセット
} YSDS_LINE; // 8 byte

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//	AoYsdFileIsYsdFile
/*!
	YSDファイル判定

	@param file		[in] YSDファイル
	@return 真：YSDファイル　偽：それ以外
	@note
	指定したバッファに格納されているファイルがYSDファイルであるか判定します。\n
	ヘッダ情報のみをチェックして判定を行ないます。\n
*/
// ===========================================================================
BOOL AoYsdFileIsYsdFile(const void* file)
{
	if ((((const YSDS_HEADER*)file)->masic[1] != 'Y') ||
		(((const YSDS_HEADER*)file)->masic[2] != 'S') ||
		(((const YSDS_HEADER*)file)->masic[3] != 'D'))
	{
		return FALSE;
	}
	return TRUE;
}

// ===========================================================================
//	AoYsdFileGetPageNum
/*!
	ページ数取得

	@param file		[in] YSDファイル
	@return ページ数
	@note
	指定のYSDファイルに含まれているページ数を返します。\n
	不正な引数を指定した場合は、アサートし0を返します。\n
*/
// ===========================================================================
u32 AoYsdFileGetPageNum(const void* file)
{
	if (!AoYsdFileIsYsdFile(file)) {
		amAssert(0);
		return 0;
	}
	return ((const YSDS_HEADER*)file)->page_num;
}

// ===========================================================================
//	AoYsdFileGetPageTime
/*!
	ページ時間取得

	@param file		[in] YSDファイル
	@param page_no	[in] ページ番号
	@return ページ時間
	@note
	指定のYSDファイルの、指定番号のページに含まれている時間を返します。\n
	不正な引数を指定した場合は、アサートし0を返します。\n
*/
// ===========================================================================
u32 AoYsdFileGetPageTime(const void* file, u32 page_no)
{
	if (page_no >= AoYsdFileGetPageNum(file)) {
		amAssert(0);
		return 0;
	}
	const YSDS_PAGE* page = (const YSDS_PAGE*)(((const YSDS_HEADER*)file) + 1);
	return page[page_no].time;
}

// ===========================================================================
//	AoYsdFileIsPageShowImage
/*!
	ページ画像表示判定

	@param file		[in] YSDファイル
	@param page_no	[in] ページ番号
	@return 真：表示あり　偽：表示なし
	@note
	指定のYSDファイルの、指定番号のページに画像表示があるか判定します。\n
	不正な引数を指定した場合は、アサートしFALSEを返します。\n
*/
// ===========================================================================
BOOL AoYsdFileIsPageShowImage(const void* file, u32 page_no)
{
	if (page_no >= AoYsdFileGetPageNum(file)) {
		amAssert(0);
		return FALSE;
	}
	const YSDS_PAGE* page = (const YSDS_PAGE*)(((const YSDS_HEADER*)file) + 1);
	if (page[page_no].show < 0) {
		return FALSE;
	}
	return TRUE;
}

// ===========================================================================
//	AoYsdFileGetPageShowImageNo
/*!
	ページ表示画像番号取得

	@param file		[in] YSDファイル
	@param page_no	[in] ページ番号
	@return ページ表示画像番号
	@note
	指定のYSDファイルの、指定番号のページの表示画像番号を返します。\n
	AoYsdFileIsPageShowImage関数がTRUEを返す場合のみ呼び出し可能です。\n
	不正な引数を指定した場合は、アサートし0を返します。\n
*/
// ===========================================================================
u32 AoYsdFileGetPageShowImageNo(const void* file, u32 page_no)
{
	if (!AoYsdFileIsPageShowImage(file, page_no)) {
		amAssert(0);
		return 0;
	}
	const YSDS_PAGE* page = (const YSDS_PAGE*)(((const YSDS_HEADER*)file) + 1);
	return (u32)page[page_no].show;
}

// ===========================================================================
//	AoYsdFileIsPageHideImage
/*!
	ページ画像非表示判定

	@param file		[in] YSDファイル
	@param page_no	[in] ページ番号
	@return 真：非表示あり　偽：非表示なし
	@note
	指定のYSDファイルの、指定番号のページに画像非表示があるか判定します。\n
	不正な引数を指定した場合は、アサートしFALSEを返します。\n
*/
// ===========================================================================
BOOL AoYsdFileIsPageHideImage(const void* file, u32 page_no)
{
	if (page_no >= AoYsdFileGetPageNum(file)) {
		amAssert(0);
		return FALSE;
	}
	const YSDS_PAGE* page = (const YSDS_PAGE*)(((const YSDS_HEADER*)file) + 1);
	if (page[page_no].hide < 0) {
		return FALSE;
	}
	return TRUE;
}

// ===========================================================================
//	AoYsdFileGetPageOption
/*!
	ページオプション取得

	@param file		[in] YSDファイル
	@param page_no	[in] ページ番号
	@return ページオプション
	@note
	指定のYSDファイルの、指定番号のページに含まれているオプションを返します。\n
	不正な引数を指定した場合は、アサートし0を返します。\n
*/
// ===========================================================================
u32 AoYsdFileGetPageOption(const void* file, u32 page_no)
{
	if (page_no >= AoYsdFileGetPageNum(file)) {
		amAssert(0);
		return 0;
	}
	const YSDS_PAGE* page = (const YSDS_PAGE*)(((const YSDS_HEADER*)file) + 1);
	return page[page_no].option;
}

// ===========================================================================
//	AoYsdFileGetLineNum
/*!
	ライン数取得

	@param file		[in] YSDファイル
	@param page_no	[in] ページ番号
	@return ライン数
	@note
	指定のYSDファイルの、指定番号のページに含まれている行数を返します。\n
	不正な引数を指定した場合は、アサートし0を返します。\n
*/
// ===========================================================================
u32 AoYsdFileGetLineNum(const void* file, u32 page_no)
{
	amAssert(page_no < AoYsdFileGetPageNum(file));
	const YSDS_PAGE* page = (const YSDS_PAGE*)(((const YSDS_HEADER*)file) + 1);
	return page[page_no].line_num;
}

// ===========================================================================
//	AoYsdFileGetLineId
/*!
	ラインID取得

	@param file		[in] YSDファイル
	@param page_no	[in] ページ番号
	@param line_no	[in] ライン番号
	@return ラインID
	@note
	指定のYSDファイルの、指定番号のページに含まれている指定行のIDを返します。\n
	不正な引数を指定した場合は、アサートし0を返します。\n
*/
// ===========================================================================
u32 AoYsdFileGetLineId(const void* file, u32 page_no, u32 line_no)
{
	amAssert(line_no < AoYsdFileGetLineNum(file, page_no));
	const YSDS_PAGE* page = (const YSDS_PAGE*)(((const YSDS_HEADER*)file) + 1);
	const YSDS_LINE* line =
		(const YSDS_LINE*)((u32)file + page[page_no].line_tbl_ofst);
	return line[line_no].id;
}

// ===========================================================================
//	AoYsdFileGetLineString
/*!
	ライン文字列取得

	@param file		[in] YSDファイル
	@param page_no	[in] ページ番号
	@param line_no	[in] ライン番号
	@return ライン文字列(null終端文字列)
	@note
	指定のYSDファイルの、
	指定番号のページに含まれている指定行の文字列を返します。\n
	不正な引数を指定した場合は、アサートし0を返します。\n
*/
// ===========================================================================
const char* AoYsdFileGetLineString(const void* file, u32 page_no, u32 line_no)
{
	amAssert(line_no < AoYsdFileGetLineNum(file, page_no));
	const YSDS_PAGE* page = (const YSDS_PAGE*)(((const YSDS_HEADER*)file) + 1);
	const YSDS_LINE* line =
		(const YSDS_LINE*)((u32)file + page[page_no].line_tbl_ofst);
	return (const char*)((u32)file + line[line_no].str_ofst);
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
