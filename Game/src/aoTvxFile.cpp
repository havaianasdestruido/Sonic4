// ===========================================================================
/*!
	@file	aoTvxFile.cpp
	@brief	TVXファイルモジュール宣言

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "aoTvxFile.h"

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ***************************************************************************
// TVXフォーマット
// ***************************************************************************
// ===========================================================================
//	struct TVXS_HEADER
// ---------------------------------------------------------------------------
//!	ファイルヘッダ構造体
// ===========================================================================
typedef struct tag_TVXS_HEADER {
	char			masic[4];		//!< 識別子"#TVX"
	u32				tex_num;		//!< テクスチャ情報数
	u32				tex_tbl_ofst;	//!< テクスチャ情報配列へのオフセット
	u32				pad[1];			//!< パディング
} TVXS_HEADER; // 16 byte

// ===========================================================================
//	struct TVXS_TEXTURE
// ---------------------------------------------------------------------------
//!	テクスチャ情報構造体
// ===========================================================================
typedef struct tag_TVXS_TEXTURE {
	s32				tex_id;			//!< テクスチャID
	u32				vtx_num;		//!< 頂点情報数
	u32				vtx_tbl_ofst;	//!< 頂点情報配列へのオフセット
	u32				prim_type;		//!< プリミティブタイプ
} TVXS_TEXTURE; // 16 byte

// ===========================================================================
//	struct TVXS_VERTEX
// ---------------------------------------------------------------------------
//!	頂点情報構造体
// ===========================================================================
typedef struct tag_TVXS_VERTEX {
	f32				x;				//!< X座標
	f32				y;				//!< Y座標
	f32				z;				//!< Z座標
	u32				c;				//!< 頂点カラー
	f32				u;				//!< U座標
	f32				v;				//!< V座標
	f32				pad[2];			//!< パディング
} TVXS_VERTEX; // 32 byte

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//	AoTvxIsTvxFile
/*!
	TVXファイル判定

	@param file		[in] ファイル
	@return 真：TVXファイル　偽：それ以外
	@note
	指定されたファイルがTVXファイルであるか判定します。\n
	ヘッダ情報のみを参照して判定を行ないます。\n
*/
// ===========================================================================
BOOL AoTvxIsTvxFile(const void* file)
{
	amAssert(file);
	const TVXS_HEADER* head = (const TVXS_HEADER*)file;
	if ((head->masic[0] != '#') ||
		(head->masic[1] != 'T') ||
		(head->masic[2] != 'V') ||
		(head->masic[3] != 'X'))
	{
		return FALSE;
	}
	return TRUE;
}

// ===========================================================================
//	AoTvxGetTextureNum
/*!
	テクスチャ数取得

	@param file		[in] ファイル
	@return テクスチャ数
*/
// ===========================================================================
u32 AoTvxGetTextureNum(const void* file)
{
	amAssert(AoTvxIsTvxFile(file));
	const TVXS_HEADER* head = (const TVXS_HEADER*)file;
	return head->tex_num;
}

// ===========================================================================
//	AoTvxGetTextureId
/*!
	テクスチャID取得

	@param file		[in] ファイル
	@param tex_no	[in] テクスチャ番号
	@return テクスチャID
*/
// ===========================================================================
s32 AoTvxGetTextureId(const void* file, u32 tex_no)
{
	amAssert(tex_no < AoTvxGetTextureNum(file));
	const TVXS_HEADER* head = (const TVXS_HEADER*)file;
	const TVXS_TEXTURE* tex =
		((const TVXS_TEXTURE*)((u32)file + head->tex_tbl_ofst)) + tex_no;
	return tex->tex_id;
}

// ===========================================================================
//	AoTvxGetPrimitiveType
/*!
	プリミティブタイプ取得

	@param file		[in] ファイル
	@param tex_no	[in] テクスチャ番号
	@return プリミティブタイプ
*/
// ===========================================================================
extern AOE_TVX_PRIMTYPE AoTvxGetPrimitiveType(const void* file, u32 tex_no)
{
	amAssert(tex_no < AoTvxGetTextureNum(file));
	const TVXS_HEADER* head = (const TVXS_HEADER*)file;
	const TVXS_TEXTURE* tex =
		((const TVXS_TEXTURE*)((u32)file + head->tex_tbl_ofst)) + tex_no;
	return (AOE_TVX_PRIMTYPE)tex->prim_type;
}

// ===========================================================================
//	AoTvxGetVertexNum
/*!
	頂点数取得

	@param file		[in] ファイル
	@param tex_no	[in] テクスチャ番号
	@return 頂点数
*/
// ===========================================================================
u32 AoTvxGetVertexNum(const void* file, u32 tex_no)
{
	amAssert(tex_no < AoTvxGetTextureNum(file));
	const TVXS_HEADER* head = (const TVXS_HEADER*)file;
	const TVXS_TEXTURE* tex =
		((const TVXS_TEXTURE*)((u32)file + head->tex_tbl_ofst)) + tex_no;
	return tex->vtx_num;
}

// ===========================================================================
//	AoTvxGetVertex
/*!
	頂点配列取得

	@param file		[in] ファイル
	@param tex_no	[in] テクスチャ番号
	@return 頂点配列先頭ポインタ
*/
// ===========================================================================
const AOS_TVX_VERTEX* AoTvxGetVertex(const void* file, u32 tex_no)
{
	amAssert(tex_no < AoTvxGetTextureNum(file));
	const TVXS_HEADER* head = (const TVXS_HEADER*)file;
	const TVXS_TEXTURE* tex =
		((const TVXS_TEXTURE*)((u32)file + head->tex_tbl_ofst)) + tex_no;
	return (const AOS_TVX_VERTEX*)((u32)file + tex->vtx_tbl_ofst);
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
