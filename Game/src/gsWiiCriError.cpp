// ===========================================================================
/*!
	@file	gsWiiCriError.cpp
	@brief	WiiWare CRIファイルシステム Fatalエラー管理モジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#if _WII

#include "gsWiiCriError.h"
#include "gsMemFile.h"
#include "ao.h"


// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

static void gsWiiCriErrorTask(AMS_TCB* tcb);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//! CNT結果コード
// ===========================================================================
static s32 g_gs_wii_cri_error_cnt_result = CNT_RESULT_OK;

// ===========================================================================
//! エラー監視タスクTCBポインタ
// ===========================================================================
static AMS_TCB* g_gs_wii_cri_error_tcb = NULL;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! 初期化処理2(アプリケーション起動時呼び出し)
// ===========================================================================
void GsWiiCriErrorInit(void)
{
	if (g_gs_wii_cri_error_tcb == NULL) {

		// CNT結果コード初期化
		g_gs_wii_cri_error_cnt_result = CNT_RESULT_OK;

		// エラー監視タスク作成
		g_gs_wii_cri_error_tcb = amTaskMake(
			gsWiiCriErrorTask, NULL, 0, 0, 0, "GsWiiCriError");
		amTaskStart(g_gs_wii_cri_error_tcb);
	}
}

// ===========================================================================
//! 終了処理(アプリケーション終了時呼び出し)
// ===========================================================================
void GsWiiCriErrorExit(void)
{
	// エラー監視タスク削除
	if (g_gs_wii_cri_error_tcb) {
		amTaskDelete(g_gs_wii_cri_error_tcb);
		g_gs_wii_cri_error_tcb = NULL;
	}
}

// ===========================================================================
//! 外部からのエラー通知
// ===========================================================================
void GsWiiCriErrorNotice(s32 error)
{
	g_gs_wii_cri_error_cnt_result = error;
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

extern AMS_WII_HOMEMENU _am_wii_homemenu;

// ===========================================================================
//! エラー監視タスク
// ===========================================================================
void gsWiiCriErrorTask(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// エラー判定
#if defined(_DLC)

	// CRIファイルシステムのエラー取得
	s32 error;
	criFs_GetCntError_WII(&error);
	if (error < 0) {
		GsWiiCriErrorNotice(error);
	}

	// AliceNNのエラー取得
	if (_am_wii_homemenu.read_err < 0) {
		GsWiiCriErrorNotice(_am_wii_homemenu.read_err);
	}

#endif // defined(_DLC)

#if defined(MTD_DEBUG)
#define	GSD_ERR_INPUT	(WPAD_BUTTON_Z | WPAD_BUTTON_C)	// 左記ボタン入力+しつつリモコンを縦に振るとエラー画面
#define	GSD_ERR_CROSS	(WPAD_BUTTON_LEFT | WPAD_BUTTON_RIGHT | WPAD_BUTTON_DOWN | WPAD_BUTTON_UP)	//十字ボタン
	// エラー画面デバッグ出力操作チェック
//	u32	fmt_type = WPADGetDataFormat(WPAD_CHAN0);
	WPADStatus	pad;
	WPADRead(WPAD_CHAN0, &pad);
	if (pad.err == WPAD_ERR_NONE) {
		if ((pad.button & GSD_ERR_INPUT) == GSD_ERR_INPUT) {
			if (pad.accZ < -384) {
				// 強制的にエラー出力
				switch(pad.button & GSD_ERR_CROSS) {
					case WPAD_BUTTON_UP:
						GsWiiCriErrorNotice(CNT_RESULT_ECC_CRIT);
						break;
					case WPAD_BUTTON_RIGHT:
						GsWiiCriErrorNotice(CNT_RESULT_CORRUPT);
						break;
					case WPAD_BUTTON_DOWN:
						GsWiiCriErrorNotice(CNT_RESULT_OUT_OF_MEMORY);
						break;
					case WPAD_BUTTON_LEFT:
						GsWiiCriErrorNotice(CNT_RESULT_UNKNOWN);
						break;
					default:
						break;
				}
			}
		}
	}
#endif

	// エラー未発生なら何もしない
	if (g_gs_wii_cri_error_cnt_result == CNT_RESULT_OK) {
		return;
	}

	// エラーに応じてメッセージ表示
	AOE_SYS_MSG_FATAL_ID id;
	switch (g_gs_wii_cri_error_cnt_result) {
	case CNT_RESULT_ECC_CRIT:
	case CNT_RESULT_AUTHENTICATION:
		id = AOD_SYS_MSG_FATAL_WARE_01;
		break;

	case CNT_RESULT_CORRUPT:
		id = AOD_SYS_MSG_FATAL_NAND_08;
		break;

	case CNT_RESULT_OUT_OF_MEMORY:
		id = AOD_SYS_MSG_FATAL_NAND_11;
		break;

	case CNT_RESULT_UNKNOWN:
		id = AOD_SYS_MSG_FATAL_NAND_12;
		break;

	default:
		id = AOD_SYS_MSG_FATAL_NAND_12;
		break;
	}
	AoSysMsgShowFatalError(id);
}

#endif // _WII

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
