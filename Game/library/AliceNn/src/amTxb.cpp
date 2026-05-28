// ===========================================================================
/*!
	@file	amTxb.cpp
	@brief	TXBファイル操作モジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "alice.h"
#include "amTxb.h"

// ----- Macros ------------------------------------------------（マクロ定義）

#if AMD_ENDIAN_TARGET == AMD_ENDIAN_LITTLE
//! エンディアン変換
#define TxbConvertEndian(x)		(_amConvertEndian(x))
#else
//! エンディアン変換(ターゲットがビッグエンディアンなので処理なし)
#define TxbConvertEndian(x)		(x)
#endif

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）

// ===========================================================================
//	enum TXB_MAG_FILTER
// ---------------------------------------------------------------------------
//!	拡大フィルタ列挙
// ===========================================================================
typedef enum tag_TXB_MAG_FILTER {
	TXB_MAGF_N		= 0,		//!< ポイントサンプル
	TXB_MAGF_L,					//!< リニアフィルタ
	TXB_MAGF_A,					//!< 異方性フィルタ
	TXB_MAGF_NUM,				//!< 拡大フィルタ数
} TXB_MAG_FILTER;

// ===========================================================================
//	enum TXB_MIN_FILTER
// ---------------------------------------------------------------------------
//!	縮小フィルタ列挙
// ===========================================================================
typedef enum tag_TXB_MIN_FILTER {
	TXB_MINF_N		= 0,		//!< ポイントサンプル
	TXB_MINF_L,					//!< リニアフィルタ
	TXB_MINF_N_M_N,				//!< ポイントサンプル+補完なしミップマップ
	TXB_MINF_N_M_L,				//!< ポイントサンプル+補完ありミップマップ
	TXB_MINF_L_M_N,				//!< リニアフィルタ+補完なしミップマップ
	TXB_MINF_L_M_L,				//!< リニアフィルタ+補完ありミップマップ
	TXB_MINF_A2,				//!< 2x異方性フィルタ
	TXB_MINF_A2_M_N,			//!< 2x異方性フィルタ+補完なしミップマップ
	TXB_MINF_A2_M_L,			//!< 2x異方性フィルタ+補完ありミップマップ
	TXB_MINF_A4,				//!< 4x異方性フィルタ
	TXB_MINF_A4_M_N,			//!< 4x異方性フィルタ+補完なしミップマップ
	TXB_MINF_A4_M_L,			//!< 4x異方性フィルタ+補完ありミップマップ
	TXB_MINF_A8,				//!< 8x異方性フィルタ
	TXB_MINF_A8_M_N,			//!< 8x異方性フィルタ+補完なしミップマップ
	TXB_MINF_A8_M_L,			//!< 8x異方性フィルタ+補完ありミップマップ
	TXB_MINF_NUM,				//!< 縮小フィルタ数
} TXB_MIN_FILTER;

// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct TXB_TEXFILE
// ---------------------------------------------------------------------------
//!	TXBテクスチャファイル構造体
// ===========================================================================
typedef struct tag_TXB_TEXFILE {
	Uint32				type;			//!< タイプ(常に0)
	char*				name;			//!< ファイル名
	Uint16				min_filter;		//!< 縮小フィルタ(TXB_MIN_FILTER)
	Uint16				mag_filter;		//!< 拡大フィルタ(TXB_MAG_FILTER)
	Uint32				global_index;	//!< グローバルインデックス(常に0)
	Uint32				bank;			//!< パレットバンク番号(常に0)
} TXB_TEXFILE; // 20 byte

// ===========================================================================
//	struct TXB_TEXFILELIST
// ---------------------------------------------------------------------------
//!	TXBテクスチャファイルリスト構造体
// ===========================================================================
typedef struct tag_TXB_TEXFILELIST {
	Sint32				tex_num;		//!< テクスチャファイル数
	TXB_TEXFILE*		tex_list;		//!< テクスチャファイル配列
} TXB_TEXFILELIST; // 8 byte

// ===========================================================================
//	struct TXB_HEADER
// ---------------------------------------------------------------------------
//!	TXBファイルヘッダ構造体
// ===========================================================================
typedef struct tag_TXB_HEADER {
	char				file_id[4];		//!< ファイルID("#TXB")
	TXB_TEXFILELIST*	texfilelist;	//!< テクスチャファイルリスト
	Uint8				pad[8];			//!< パディング領域(0固定)
} TXB_HEADER; // 16 byte

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//! 縮小フィルタ変換テーブル
// ===========================================================================
static const Uint16 g_txb_min_filter[TXB_MINF_NUM] = {
	NND_MIN_NEAREST,
	NND_MIN_LINEAR,
	NND_MIN_NEAREST_MIPMAP_NEAREST,
	NND_MIN_NEAREST_MIPMAP_LINEAR,
	NND_MIN_LINEAR_MIPMAP_NEAREST,
	NND_MIN_LINEAR_MIPMAP_LINEAR,
	NND_MIN_ANISOTROPIC2,
	NND_MIN_ANISOTROPIC2_MIPMAP_NEAREST,
	NND_MIN_ANISOTROPIC2_MIPMAP_LINEAR,
	NND_MIN_ANISOTROPIC4,
	NND_MIN_ANISOTROPIC4_MIPMAP_NEAREST,
	NND_MIN_ANISOTROPIC4_MIPMAP_LINEAR,
	NND_MIN_ANISOTROPIC8,
	NND_MIN_ANISOTROPIC8_MIPMAP_NEAREST,
	NND_MIN_ANISOTROPIC8_MIPMAP_LINEAR,
};

// ===========================================================================
//! 拡大フィルタ変換テーブル
// ===========================================================================
static const Uint16 g_txb_mag_filter[TXB_MAGF_NUM] = {
	NND_MAG_NEAREST,
	NND_MAG_LINEAR,
	NND_MAG_ANISOTROPIC,
};

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//	amTxbConv
/*!
	TXBファイルのアドレス&エンディアン変換

	@param txb		[io] TXBファイルバッファへのポインタ
	@return 1:成功 0:失敗(TXBファイル以外)
	@note
	引数にNULLが指定された場合はアサートし何も行いません。\n
*/
// ===========================================================================
Sint32 amTxbConv(Uint8* txb)
{
	char		*cp, ch;

	if (txb == NULL) {
		amAssert(0);
		return 0;
	}

	// ヘッダ
	TXB_HEADER* head = (TXB_HEADER*)txb;
	if ((head->file_id[1] != 'T') ||
		(head->file_id[2] != 'X') ||
		(head->file_id[3] != 'B'))
	{
		return 0;
	}
	if (head->file_id[0] == AMD_CONVERTED_MARK) {
		return 0;
	}
	head->file_id[0] = AMD_CONVERTED_MARK;

	TxbConvertEndian(&((Uint32&)head->texfilelist));
	amConvert(head, TXB_TEXFILELIST*, head->texfilelist);

	// テクスチャファイルリスト
	TXB_TEXFILELIST* list = head->texfilelist;
	TxbConvertEndian(&list->tex_num);
	TxbConvertEndian(&((Uint32&)list->tex_list));
	amConvert(head, TXB_TEXFILE*, list->tex_list);

	// テクスチャファイル
	TXB_TEXFILE* tex = list->tex_list;
	for (Sint32 no = 0; no < list->tex_num; ++no, ++tex) {
		TxbConvertEndian(&tex->type);
		TxbConvertEndian(&((Uint32&)tex->name));
		TxbConvertEndian(&tex->min_filter);
		TxbConvertEndian(&tex->mag_filter);
		TxbConvertEndian(&tex->global_index);
		TxbConvertEndian(&tex->bank);
		amConvert(head, char*, tex->name);

		// フィルタ変換
		tex->min_filter = g_txb_min_filter[tex->min_filter];
		tex->mag_filter = g_txb_mag_filter[tex->mag_filter];

		// テクスチャファイル名大文字化（オフラインなら不要）
		cp			= tex->name;
		for (int j = strlen(cp); j > 0; j--, cp++) {
			ch			= *cp;
			if ((ch >= 'a') && (ch <= 'z'))
				*cp			= ch & 0xdf;
		}
	}

	return 1;
}

// ===========================================================================
//	amTxbGetTexFileList
/*!
	テクスチャファイルリスト取得

	@param txb		[in] TXBファイルバッファへのポインタ
	@return テクスチャファイルリスト
*/
// ===========================================================================
NNS_TEXFILELIST* amTxbGetTexFileList(void* txb)
{
	return (NNS_TEXFILELIST*)(((TXB_HEADER*)txb)->texfilelist);
}

// ===========================================================================
//	amTxbGetCount
/*!
	テクスチャ数取得

	@param txb		[in] TXBファイルバッファへのポインタ
	@return テクスチャ数
	@note
	引数にNULLが指定された場合はアサートし0を返します。\n
*/
// ===========================================================================
Uint32 amTxbGetCount(const void* txb)
{
	if (txb == NULL) {
		amAssert(0);
		return 0;
	}
	return (Uint32)(((const TXB_HEADER*)txb)->texfilelist->tex_num);
}

// ===========================================================================
//	amTxbGetMagFilter
/*!
	拡大フィルタ取得

	@param txb		[in] TXBファイルバッファへのポインタ
	@param tex_no	[in] テクスチャ番号
	@return 拡大フィルタ
	@note
	テクスチャ番号にamTxbGetCount関数の戻り値以上の値を指定した場合は
	アサートします。\n
*/
// ===========================================================================
NNF_TEXFILE_MAGFILTER amTxbGetMagFilter(const void* txb, Uint32 tex_no)
{
	if (tex_no >= amTxbGetCount(txb)) {
		amAssert(0);
		return g_txb_mag_filter[0];
	}

	return ((const TXB_HEADER*)txb)->texfilelist->tex_list[tex_no].mag_filter;
}

// ===========================================================================
//	amTxbGetMinFilter
/*!
	縮小フィルタ取得

	@param txb		[in] TXBファイルバッファへのポインタ
	@param tex_no	[in] テクスチャ番号
	@return 縮小フィルタ
	@note
	テクスチャ番号にamTxbGetCount関数の戻り値以上の値を指定した場合は
	アサートします。\n
*/
// ===========================================================================
NNF_TEXFILE_MINFILTER amTxbGetMinFilter(const void* txb, Uint32 tex_no)
{
	if (tex_no >= amTxbGetCount(txb)) {
		amAssert(0);
		return g_txb_min_filter[0];
	}

	return ((const TXB_HEADER*)txb)->texfilelist->tex_list[tex_no].min_filter;
}

// ===========================================================================
//	amTxbGetName
/*!
	テクスチャ名取得

	@param txb		[in] TXBファイルバッファへのポインタ
	@param tex_no	[in] テクスチャ番号
	@return テクスチャ名
	@note
	テクスチャ番号にamTxbGetCount関数の戻り値以上の値を指定した場合は
	アサートし0番目のテクスチャ名を返します。\n
	いかなる場合でも、戻り値は非NULLであることが保障されます。\n
*/
// ===========================================================================
const char* amTxbGetName(const void* txb, Uint32 tex_no)
{
	if (tex_no >= amTxbGetCount(txb)) {
		amAssert(0);
		tex_no = 0;
	}

	return ((const TXB_HEADER*)txb)->texfilelist->tex_list[tex_no].name;
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
