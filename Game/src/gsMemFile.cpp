// ===========================================================================
/*!
	@file	gsMemFile.cpp
	@brief	メモリ常駐ファイル管理モジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "gsMemFile.h"
#include "gsEnvironment.h"

#if _PC

#include "binary/bin_win_a_win32.inc"

#elif _XBOX

#include "binary/bin_win_a_xbox360.inc"
#include "binary/bin_xbox360_save_thumb_png.inc"

#elif _PS3

#include "binary/bin_win_a_ps3.inc"
#include "binary/bin_ps3_save_icon0_png.inc"
#include "binary/bin_ps3_save_pic1_png.inc"

#elif _WII

#include "binary/bin_win_a_wii.inc"
#include "binary/bin_wii_save_banner_tpl.inc"
#include "binary/bin_ao_sys_msg_fatal_jp_msg.inc"
#include "binary/bin_ao_sys_msg_fatal_us_msg.inc"
#include "binary/bin_ao_sys_msg_fatal_fr_msg.inc"
#include "binary/bin_ao_sys_msg_fatal_it_msg.inc"
#include "binary/bin_ao_sys_msg_fatal_ge_msg.inc"
#include "binary/bin_ao_sys_msg_fatal_sp_msg.inc"

#elif _IPHONE

#include "binary/bin_win_a_win32.inc"

#endif

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
//	GsMemFileInit
/*!
	メモリ常駐ファイル初期化処理
*/
// ===========================================================================
void GsMemFileInit(void)
{
	amConvertAddress(g_gs_bin_win_a_amb);
}

// ===========================================================================
//	GsMemFileInit
/*!
	メモリ常駐ファイル終了処理
*/
// ===========================================================================
void GsMemFileExit(void)
{
	// empty
}

// ===========================================================================
//	GsMemFileGetWindowTextureAtypeAmb
/*!
	ウインドウテクスチャAタイプAMB取得

	@return ウインドウテクスチャAタイプAMB
*/
// ===========================================================================
void* GsMemFileGetWindowTextureAtypeAmb(void)
{
	return g_gs_bin_win_a_amb;
}

#if _XBOX
// ===========================================================================
//	GsMemFileGetXbox360SaveThumbPng
/*!
	セーブサムネイルPNGファイル取得

	@return セーブサムネイルPNGファイル
*/
// ===========================================================================
void* GsMemFileGetXbox360SaveThumbPng(void)
{
	return g_gs_bin_xbox360_save_thumb_png;
}

// ===========================================================================
//	GsMemFileGetXbox360SaveThumbPngSize
/*!
	セーブサムネイルPNGファイルサイズ取得

	@return セーブサムネイルPNGファイルサイズ
*/
// ===========================================================================
u32 GsMemFileGetXbox360SaveThumbPngSize(void)
{
	return g_gs_bin_xbox360_save_thumb_png_size;
}
#endif // _XBOX

#if _WII
// ===========================================================================
//	GsMemFileGetWiiSaveBannerTpl
/*!
	Wii用セーブバナーTPLファイル取得

	@return Wii用セーブバナーTPLファイル
*/
// ===========================================================================
void* GsMemFileGetWiiSaveBannerTpl(void)
{
	return g_gs_bin_wii_save_banner_tpl;
}

// ===========================================================================
//	GsMemFileGetWiiSaveBannerTplSize
/*!
	Wii用セーブバナーTPLファイルサイズ取得

	@return Wii用セーブバナーTPLファイルサイズ
*/
// ===========================================================================
u32 GsMemFileGetWiiSaveBannerTplSize(void)
{
	return g_gs_bin_wii_save_banner_tpl_size;
}

// ===========================================================================
//	GsMemFileGetWiiFatalErrorMessageFile
/*!
	Wii用Fatalエラーメッセージファイル取得

	@return Wii用Fatalエラーメッセージファイル
	@note
	システム設定言語のファイルを返します。\n
*/
// ===========================================================================
const void* GsMemFileGetWiiFatalErrorMessageFile(void)
{
	return GsMemFileGetWiiFatalErrorMessageFile(GsEnvGetLanguage());
}

// ===========================================================================
//	GsMemFileGetWiiFatalErrorMessageFile
/*!
	Wii用Fatalエラーメッセージファイル取得

	@param lang	[in] 言語
	@return Wii用Fatalエラーメッセージファイル
*/
// ===========================================================================
const void* GsMemFileGetWiiFatalErrorMessageFile(GSE_LANGUAGE lang)
{
	const void* ret = NULL;
	switch (lang) {
	case GSD_LANGUAGE_JP:
		ret = bin_ao_sys_msg_fatal_jp;
		break;
	case GSD_LANGUAGE_US:
		ret = bin_ao_sys_msg_fatal_us;
		break;
	case GSD_LANGUAGE_FR:
		ret = bin_ao_sys_msg_fatal_fr;
		break;
	case GSD_LANGUAGE_IT:
		ret = bin_ao_sys_msg_fatal_it;
		break;
	case GSD_LANGUAGE_GE:
		ret = bin_ao_sys_msg_fatal_ge;
		break;
	case GSD_LANGUAGE_SP:
		ret = bin_ao_sys_msg_fatal_sp;
		break;
	default:
		ret = bin_ao_sys_msg_fatal_us;
		break;
	}
	return ret;
}
#endif // _WII


#if _PS3
// ===========================================================================
//	GsMemFileGetPS3SaveIcon0Png
/*!
	PS3用セーブアイコンIOCN0.PNG取得

	@return PS3用セーブアイコンIOCN0.PNG
*/
// ===========================================================================
void* GsMemFileGetPS3SaveIcon0Png(void)
{
	return g_gs_bin_ps3_save_icon0_png;
}

// ===========================================================================
//	GsMemFileGetPS3SaveIcon0PngSize
/*!
	PS3用セーブアイコンIOCN0.PNGサイズ取得

	@return PS3用セーブアイコンIOCN0.PNGサイズ
*/
// ===========================================================================
u32 GsMemFileGetPS3SaveIcon0PngSize(void)
{
	return g_gs_bin_ps3_save_icon0_png_size;
}

// ===========================================================================
//	GsMemFileGetPS3SavePic1Png
/*!
	PS3用セーブ背景PIC1.PNG取得

	@return PS3用セーブ背景PIC1.PNG
*/
// ===========================================================================
void* GsMemFileGetPS3SavePic1Png(void)
{
	return g_gs_bin_ps3_save_pic1_png;
}

// ===========================================================================
//	GsMemFileGetPS3SavePic1PngSize
/*!
	PS3用セーブ背景PIC1.PNGサイズ取得

	@return PS3用セーブ背景PIC1.PNGサイズ
*/
// ===========================================================================
u32 GsMemFileGetPS3SavePic1PngSize(void)
{
	return g_gs_bin_ps3_save_pic1_png_size;
}
#endif // _PS3

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
