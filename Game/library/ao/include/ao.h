// ===========================================================================
/*!
	@file	ao.h
	@brief	AoLibrary

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================
#pragma once

// ----- Macros ------------------------------------------------（マクロ定義）

// プラットフォーム定義
#if _PC
#define AOD_PLATFORM_WIN32		(1)		//!< Win32プラットフォーム定義
#elif _XBOX
#define AOD_PLATFORM_XBOX360	(1)		//!< XBOX360プラットフォーム定義
#elif _PS3
#define AOD_PLATFORM_PS3		(1)		//!< PlayStation3プラットフォーム定義
#elif _WII
#define AOD_PLATFORM_WII		(1)		//!< Wiiプラットフォーム定義
#elif _IPHONE
#define AOD_PLATFORM_IPHONE		(1)		//!< iPhoneプラットフォーム定義
#else
// 未対応のプラットフォーム(コンパイルエラー)
#error
#endif

#if AMD_DEBUG
#define AOD_DEBUG				(1)		//!< デバッグ定義
#endif

// ----- Include Files ---------------------------------------（インクルード）

#if defined(AOD_PLATFORM_PS3)
#include "np.h"
#include "np/trophy.h"
#endif // defined(AOD_PLATFORM_PS3)

#include "aoSystem.h"
#include "aoAction.h"
#include "aoTexture.h"
#include "aoFont.h"
#include "aoMsg.h"
#include "aoWinSys.h"
#include "aoSysMsg.h"
#include "aoAccount.h"
#include "aoPad.h"
#include "aoStorage.h"
#include "aoTrophy.h"
#include "aoDebug.h"
#include "aoMemory.h"
#include "aoProc.h"
#include "aoTask.h"
#include "aoThread.h"

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）
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
