// ===========================================================================
/*!
	@file	gsReboot.cpp
	@brief	再起動管理モジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "gsReboot.h"

// ----- Macros ------------------------------------------------（マクロ定義）

#if _PS3 || _WII
#define GSD_REBOOT_ENABLE	(1)		//!< 再起動管理有効
#endif // _PS3 || _WII

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

#if defined(GSD_REBOOT_ENABLE)

// ===========================================================================
//! 再起動フラグ
// ===========================================================================
static BOOL g_reboot_is_reboot = FALSE;

#if _WII
// ===========================================================================
//! タイトル遷移済みフラグ
// ===========================================================================
static BOOL g_reboot_is_trans_title = FALSE;
#endif // _WII

#endif // defined(GSD_REBOOT_ENABLE)

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! モジュール初期化処理(アプリケーション起動時呼び出し)
// ===========================================================================
void GsRebootInit(void)
{
	amSystemLog("gsReboot Initializing...\n");

#if defined(GSD_REBOOT_ENABLE)

	// グローバル変数初期化
	g_reboot_is_reboot = FALSE;

#if _PS3
// ---------------------------------------------------------------------------
// ↓PS3↓
// ---------------------------------------------------------------------------

	// 購入画面後判定
	int result = 0;
	unsigned int type = 0;
	unsigned int attributes = 0;
	CellGameContentSize content_size;
	char contentInfoPath[CELL_GAME_HDDGAMEPATH_SIZE];
	char usrdirPath[CELL_GAME_HDDGAMEPATH_SIZE];
	result = cellGameBootCheck(&type, &attributes, &content_size, NULL);
	amAssert(result == CELL_GAME_RET_OK);
	if (result == CELL_GAME_RET_OK) {
		if (attributes & CELL_GAME_ATTRIBUTE_XMBBUY) {
			g_reboot_is_reboot = TRUE;
		}
	}
	result = cellGameContentPermit(contentInfoPath, usrdirPath);
	amAssert(result == CELL_GAME_RET_OK);

#if defined(MTD_DEBUG)
	if (g_reboot_is_reboot) {
		amSystemLog("*gsReboot reboot*\n");
	}
	else {
		amSystemLog("*gsReboot first boot*\n");
	}
#endif // defined(MTD_DEBUG)

// ---------------------------------------------------------------------------
// ↑PS3↑
// ---------------------------------------------------------------------------
#elif _WII
// ---------------------------------------------------------------------------
// ↓Wii↓
// ---------------------------------------------------------------------------

	// グローバル変数初期化
	g_reboot_is_trans_title = FALSE;

	// 再起動判定
	if (OSIsRestart()) {
		u32 code = OSGetResetCode();
		if (code & OS_RESETCODE_RESTART) {
			g_reboot_is_reboot = TRUE;
			if (code & 0x00000001) {
				g_reboot_is_trans_title = TRUE;
			}
		}
	}

#if defined(MTD_DEBUG)
	if (g_reboot_is_reboot) {
		if (g_reboot_is_trans_title) {
			amSystemLog("*gsReboot reboot(title)*\n");
		}
		else {
			amSystemLog("*gsReboot reboot*\n");
		}
	}
	else {
		amSystemLog("*gsReboot first boot*\n");
	}
#endif // defined(MTD_DEBUG)

// ---------------------------------------------------------------------------
// ↑Wii↑
// ---------------------------------------------------------------------------
#elif
#error
#endif

#else

	// empty

#endif // defined(GSD_REBOOT_ENABLE)

	amSystemLog("gsReboot Initialized.\n");
}

// ===========================================================================
//! モジュール終了処理(アプリケーション終了時呼び出し)
// ===========================================================================
void GsRebootExit(void)
{
#if defined(GSD_REBOOT_ENABLE)

	// 未作成

#else

	// empty

#endif // defined(GSD_REBOOT_ENABLE)
}

// ===========================================================================
//! 再起動判定
// ===========================================================================
BOOL GsRebootIsReboot(void)
{
#if defined(GSD_REBOOT_ENABLE)

	return g_reboot_is_reboot;

#else

	return FALSE;

#endif // defined(GSD_REBOOT_ENABLE)
}

// ===========================================================================
//! タイトル遷移以降の再起動判定
// ===========================================================================
BOOL GsRebootIsTitleReboot(void)
{
#if defined(GSD_REBOOT_ENABLE)

#if _PS3
	if (GsRebootIsReboot()) {
		return TRUE;
	}
#elif _WII
	if (GsRebootIsReboot() && g_reboot_is_trans_title) {
		return TRUE;
	}
#elif
#error
#endif

#endif // defined(GSD_REBOOT_ENABLE)

	return FALSE;
}

// ===========================================================================
//! タイトル遷移済み設定
// ===========================================================================
void GsRebootSetTitle(void)
{
#if defined(GSD_REBOOT_ENABLE)

#if _WII

	g_reboot_is_trans_title = TRUE;

#else

	// empty

#endif // _WII

#else

	// empty

#endif // defined(GSD_REBOOT_ENABLE)
}

// ===========================================================================
//! タイトル遷移済み判定
// ===========================================================================
BOOL GsRebootIsTitle(void)
{
#if defined(GSD_REBOOT_ENABLE)

#if _WII

	return g_reboot_is_trans_title;

#else

	return FALSE;

#endif // _WII

#else

	return FALSE;

#endif // defined(GSD_REBOOT_ENABLE)
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
