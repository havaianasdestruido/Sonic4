// ===========================================================================
/*!
	@file	gsSystemBgm.cpp
	@brief	システムBGM管理モジュール定義(無効)

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "gsSystemBgm.h"

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
//	GsSystemBgmInit
/*!
	モジュール初期化処理

	@note
	アプリケーション起動時に一度だけ呼び出し。\n
*/
// ===========================================================================
void GsSystemBgmInit(void)
{
	// empty
}

// ===========================================================================
//	GsSystemBgmExit
/*!
	モジュール終了処理

	@note
	アプリケーション終了時に一度だけ呼び出し。\n
*/
// ===========================================================================
void GsSystemBgmExit(void)
{
	// empty
}

// ===========================================================================
//	GsSystemBgmIsPlay
/*!
	システムBGM再生中判定

	@return 真：再生中　偽：それ以外
	@note
	システムBGMが再生されているか判定します。\n
	この関数がTRUEを返している間は、
	ゲームBGMを停止(ミュート)しておく必要があります。\n
*/
// ===========================================================================
BOOL GsSystemBgmIsPlay(void)
{
	return AoSysIsPlaySystemBgm();
}

// ===========================================================================
//	GsSystemBgmSetEnable
/*!
	システムBGM有効設定

	@param is_enable	[in] 真：有効　偽：無効
	@note
	TRUEを指定した場合は、システムBGMを有効にします。\n
	FALSEを指定した場合は、システムBGMを無効にします。\n
	この関数でシステムBGMを無効にしたとしても、
	GsSystemBgmIsPlay関数が必ずFALSEを返すわけではない点に注意して下さい。\n
*/
// ===========================================================================
void GsSystemBgmSetEnable(BOOL is_enable)
{
#if _PS3

	CellSysutilBgmPlaybackExtraParam param;
	amZeroMemory(&param, sizeof(CellSysutilBgmPlaybackExtraParam));
	param.systemBgmFadeInTime = CELL_SYSUTIL_BGMPLAYBACK_FADE_INVALID;
	param.systemBgmFadeOutTime = CELL_SYSUTIL_BGMPLAYBACK_FADE_INVALID;
	param.gameBgmFadeInTime = CELL_SYSUTIL_BGMPLAYBACK_FADE_INVALID;
	param.gameBgmFadeOutTime = CELL_SYSUTIL_BGMPLAYBACK_FADE_INVALID;
	if (is_enable) {
		cellSysutilEnableBgmPlaybackEx(&param);
	}
	else {
		cellSysutilDisableBgmPlaybackEx(&param);
	}

#elif _XBOX

	if (is_enable) {
		XMPRestoreBackgroundMusic();
	}
	else {
		XMPOverrideBackgroundMusic();
	}

#else

	UNREFERENCED_PARAMETER(is_enable);
	// empty

#endif
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
