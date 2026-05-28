// ===========================================================================
/*!
	@file	aoTexture.cpp
	@brief	AoLibrary テクスチャユーティリティ定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

static void aoTexInitTex(AOS_TEXTURE* tex);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//	AoTexBuild
/*!
	AMBファイルからテクスチャ構築

	@param tex		[out] テクスチャ構造体
	@param amb		[io]  AMBファイル(要アドレス変換)
	@note
	1つのTXBファイルと1つ以上のテクスチャファイルで構成されたAMBを元に
	テクスチャを使用できるように構築処理を行います。\n
	この関数呼出し後、AoTexLoad関数を呼び出すことで、
	テクスチャの読み込み処理が開始されます。\n
	(AMB内のテクスチャを使用するのでファイルアクセスは発生しません。)\n
	指定するAMBファイルは、アドレス変換済みである必要があります。\n
	不正な引数を指定した場合は、アサートし何も行いません。\n
*/
// ===========================================================================
void AoTexBuild(AOS_TEXTURE* tex, void* amb)
{
	// 不正な引数チェック
	if (tex == NULL) {
		amAssert(0);
		return;
	}
	if (amb == NULL) {
		amAssert(0);
		return;
	}

	// テクスチャ構造体初期化
	aoTexInitTex(tex);

	// ファイル設定
	tex->amb = amb;
	tex->txb = amBindSearchEx((AMS_AMB_HEADER*)amb, "txb");
	if (tex->txb == NULL) {
		tex->txb = amBindSearchEx((AMS_AMB_HEADER*)amb, "TXB");
	}
	if (tex->txb == NULL) {
		amAssert(0);
		return;
	}
}

// ===========================================================================
//	AoTexLoad
/*!
	テクスチャ読み込み開始

	@param tex		[io] テクスチャ構造体
	@note
	AoTexBuild***関数で構築したテクスチャ構造体を元に、
	必要な読み込み処理を行います。\n
	この関数は即時復帰です。\n
	読み込みの完了はAoTexIsLoaded関数で判別してください。\n
	未構築のテクスチャ構造体を指定した場合、アサートし何も行いません。\n
	既に読み込み中の場合はアサートし何も行いません。\n
	AoTexRelease関数呼び出し以降の場合は、アサートし何も行いません。\n
*/
// ===========================================================================
void AoTexLoad(AOS_TEXTURE* tex)
{
	// 不正な呼び出しチェック
	if ((tex == NULL) || (tex->txb == NULL) || (tex->amb == NULL) ||
		tex->texlist || (tex->reg_id >= 0))
	{
		amAssert(0);
		return;
	}

	// テクスチャ数取得
	u32 tex_num = amTxbGetCount(tex->txb);

	// テクスチャリスト作成
	tex->texlist_buf = amMemAlloc(nnEstimateTexlistSize((s32)tex_num));
	nnSetUpTexlist(&tex->texlist, (s32)tex_num, tex->texlist_buf);

	// テクスチャファイルリスト取得
	NNS_TEXFILELIST* texfilelist = amTxbGetTexFileList(tex->txb);

	// テクスチャ登録開始
	tex->reg_id = amTextureLoad(
		tex->texlist, texfilelist, NULL, (AMS_AMB_HEADER*)tex->amb);
}

// ===========================================================================
//	AoTexIsLoaded
/*!
	テクスチャ読み込み完了判定

	@param tex		[io] テクスチャ構造体
	@return 真：読み込み完了　偽：それ以外
	@note
	AoTexLoad関数で開始した読み込み処理が完了したかどうかを判定します。\n
	AoTexLoad関数が正常に呼び出されていない状態の場合は、
	アサートしFALSEを返します。\n
*/
// ===========================================================================
BOOL AoTexIsLoaded(AOS_TEXTURE* tex)
{
	// 不正な呼び出しチェック
	if ((tex == NULL) || (tex->texlist == NULL)) {
		amAssert(0);
		return FALSE;
	}

	// 読み込み済み判定
	if (tex->reg_id >= 0) {
		if (amDrawIsRegistComplete(tex->reg_id)) {
			tex->reg_id = -1;
		}
	}
	if (tex->reg_id < 0) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//	AoTexGetTexList
/*!
	テクスチャリスト取得

	@param tex		[io] テクスチャ構造体
	@return テクスチャリスト
	@note
	この関数はAoTexIsLoaded関数が正常にTRUEを返す状態でのみ
	呼び出すことができます。\n
	それ以外の状態で呼び出された場合は、アサートしNULLを返します。
*/
// ===========================================================================
NNS_TEXLIST* AoTexGetTexList(const AOS_TEXTURE* tex)
{
	// 不正な呼び出しチェック
	if ((tex == NULL) || (tex->texlist == NULL) || (tex->reg_id >= 0)) {
		amAssert(0);
		return NULL;
	}
	return tex->texlist;
}

// ===========================================================================
//	AoTexGetTxb
/*!
	TXBファイル取得

	@param tex		[io] テクスチャ構造体
	@return TXBファイル
	@note
	この関数はAoTexIsLoaded関数が正常にTRUEを返す状態でのみ
	呼び出すことができます。\n
	それ以外の状態で呼び出された場合は、アサートしNULLを返します。
*/
// ===========================================================================
const void* AoTexGetTxb(const AOS_TEXTURE* tex)
{
	// 不正な呼び出しチェック
	if ((tex == NULL) || (tex->texlist == NULL) || (tex->reg_id >= 0)) {
		amAssert(0);
		return NULL;
	}
	return tex->txb;
}

// ===========================================================================
//	AoTexRelease
/*!
	テクスチャ解放開始

	@param tex		[io] テクスチャ構造体
	@note
	テクスチャ構造体が現在確保しているリソースを解放し、
	AoTexBuild関数呼び出し以前の状態に戻します。\n
	AoTexIsLoaded関数がTRUEを返す状態でしか呼び出すことができません。\n
	解放処理の官僚はAoTexIsReleased関数で判別してください。\n
	この関数を呼び出した場合、
	時間経過により必ずAoTexIsReleased関数がTRUEを返すようになります。\n
	内部でのアサートなどはありません。\n
*/
// ===========================================================================
void AoTexRelease(AOS_TEXTURE* tex)
{
	// 不正な呼び出しチェック
	if (!AoTexIsLoaded(tex)) {
		amAssert(0);
		return;
	}

	// 解放要求
	tex->reg_id = amTextureRelease(tex->texlist);

	// テクスチャリスト無効化
	tex->texlist = NULL;
}

// ===========================================================================
//	AoTexIsReleased
/*!
	テクスチャ解放完了判定

	@param tex		[io] テクスチャ構造体
	@return 真：解放完了　偽：それ以外
	@note
	AoTexRelease関数で開始した解放処理が完了したかどうかを判定します。\n
	すでに解放済みの場合は何も行わずTRUEを返します。\n
*/
// ===========================================================================
BOOL AoTexIsReleased(AOS_TEXTURE* tex)
{
	if ((tex == NULL) || (tex->texlist_buf == NULL) || (tex->reg_id < 0)) {
		return TRUE;
	}

	// 解放済み判定
	if (amDrawIsRegistComplete(tex->reg_id)) {

		// テクスチャリストバッファ解放
		if (tex->texlist_buf) {
			amMemFree(tex->texlist_buf);
			tex->texlist_buf = NULL;
		}

		// テクスチャ構造体初期化
		aoTexInitTex(tex);

		return TRUE;
	}
	return FALSE;
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ===========================================================================
//! テクスチャ構造体初期化
// ===========================================================================
void aoTexInitTex(AOS_TEXTURE* tex)
{
	tex->texlist = NULL;
	tex->texlist_buf = NULL;
	tex->reg_id = -1;
	tex->amb = NULL;
	tex->txb = NULL;
}

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
