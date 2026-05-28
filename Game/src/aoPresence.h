// ===========================================================================
/*!
	@file	aoPresence.h
	@brief	プレゼンスシステム宣言

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================
#pragma once

// ----- Include Files ---------------------------------------（インクルード）
// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）

// ===========================================================================
//	enum AOE_PRESENCE
// ---------------------------------------------------------------------------
//!	プレゼンス列挙
// ===========================================================================
typedef enum tag_AOE_PRESENCE {
	AOD_PRESENCE_STANDBY	= 0,	//!< 待機中(メニューなど)
	AOD_PRESENCE_Z11T,				//!< Zone1-1タイムアタックプレイ中
	AOD_PRESENCE_Z11S,				//!< Zone1-1スコアアタックプレイ中
	AOD_PRESENCE_Z12T,				//!< Zone1-2タイムアタックプレイ中
	AOD_PRESENCE_Z12S,				//!< Zone1-2スコアアタックプレイ中
	AOD_PRESENCE_Z13T,				//!< Zone1-3タイムアタックプレイ中
	AOD_PRESENCE_Z13S,				//!< Zone1-3スコアアタックプレイ中
	AOD_PRESENCE_Z1BT,				//!< Zone1-Bタイムアタックプレイ中
	AOD_PRESENCE_Z1BS,				//!< Zone1-Bスコアアタックプレイ中
	AOD_PRESENCE_Z21T,				//!< Zone2-1タイムアタックプレイ中
	AOD_PRESENCE_Z21S,				//!< Zone2-1スコアアタックプレイ中
	AOD_PRESENCE_Z22T,				//!< Zone2-2タイムアタックプレイ中
	AOD_PRESENCE_Z22S,				//!< Zone2-2スコアアタックプレイ中
	AOD_PRESENCE_Z23T,				//!< Zone2-3タイムアタックプレイ中
	AOD_PRESENCE_Z23S,				//!< Zone2-3スコアアタックプレイ中
	AOD_PRESENCE_Z2BT,				//!< Zone2-Bタイムアタックプレイ中
	AOD_PRESENCE_Z2BS,				//!< Zone2-Bスコアアタックプレイ中
	AOD_PRESENCE_Z31T,				//!< Zone3-1タイムアタックプレイ中
	AOD_PRESENCE_Z31S,				//!< Zone3-1スコアアタックプレイ中
	AOD_PRESENCE_Z32T,				//!< Zone3-2タイムアタックプレイ中
	AOD_PRESENCE_Z32S,				//!< Zone3-2スコアアタックプレイ中
	AOD_PRESENCE_Z33T,				//!< Zone3-3タイムアタックプレイ中
	AOD_PRESENCE_Z33S,				//!< Zone3-3スコアアタックプレイ中
	AOD_PRESENCE_Z3BT,				//!< Zone3-Bタイムアタックプレイ中
	AOD_PRESENCE_Z3BS,				//!< Zone3-Bスコアアタックプレイ中
	AOD_PRESENCE_Z41T,				//!< Zone4-1タイムアタックプレイ中
	AOD_PRESENCE_Z41S,				//!< Zone4-1スコアアタックプレイ中
	AOD_PRESENCE_Z42T,				//!< Zone4-2タイムアタックプレイ中
	AOD_PRESENCE_Z42S,				//!< Zone4-2スコアアタックプレイ中
	AOD_PRESENCE_Z43T,				//!< Zone4-3タイムアタックプレイ中
	AOD_PRESENCE_Z43S,				//!< Zone4-3スコアアタックプレイ中
	AOD_PRESENCE_Z4BT,				//!< Zone4-Bタイムアタックプレイ中
	AOD_PRESENCE_Z4BS,				//!< Zone4-Bスコアアタックプレイ中
	AOD_PRESENCE_ZFBT,				//!< FinalZoneタイムアタックプレイ中
	AOD_PRESENCE_ZFBS,				//!< FinalZoneスコアアタックプレイ中
	AOD_PRESENCE_SS1T,				//!< SpecialStage1タイムアタックプレイ中
	AOD_PRESENCE_SS1S,				//!< SpecialStage1スコアアタックプレイ中
	AOD_PRESENCE_SS2T,				//!< SpecialStage2タイムアタックプレイ中
	AOD_PRESENCE_SS2S,				//!< SpecialStage2スコアアタックプレイ中
	AOD_PRESENCE_SS3T,				//!< SpecialStage3タイムアタックプレイ中
	AOD_PRESENCE_SS3S,				//!< SpecialStage3スコアアタックプレイ中
	AOD_PRESENCE_SS4T,				//!< SpecialStage4タイムアタックプレイ中
	AOD_PRESENCE_SS4S,				//!< SpecialStage4スコアアタックプレイ中
	AOD_PRESENCE_SS5T,				//!< SpecialStage5タイムアタックプレイ中
	AOD_PRESENCE_SS5S,				//!< SpecialStage5スコアアタックプレイ中
	AOD_PRESENCE_SS6T,				//!< SpecialStage6タイムアタックプレイ中
	AOD_PRESENCE_SS6S,				//!< SpecialStage6スコアアタックプレイ中
	AOD_PRESENCE_SS7T,				//!< SpecialStage7タイムアタックプレイ中
	AOD_PRESENCE_SS7S,				//!< SpecialStage7スコアアタックプレイ中

	AOD_PRESENCE_NUM,				//!< プレゼンス数
	AOD_PRESENCE_NONE,				//!< 無効コード

	AOD_PRESENCE_DEFAULT = AOD_PRESENCE_STANDBY,	//!< 初期値
} AOE_PRESENCE;

// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）

// ===========================================================================
//	AoPresenceInit
/*!
	プレゼンスシステム初期化開始

	@note
	アプリケーション起動時に一度だけ呼び出して下さい。\n
*/
// ===========================================================================
extern void AoPresenceInit(void);

// ===========================================================================
//	AoPresenceInit
/*!
	プレゼンスシステム初期化終了判定

	@return 真：初期化終了済み　偽：それ以外
	@note
	AoPresenceInit関数で開始した終了処理が完了したかどうかを判定します。\n
*/
// ===========================================================================
extern BOOL AoPresenceInitialized(void);

// ===========================================================================
//	AoPresenceInit
/*!
	プレゼンスシステム終了処理

	@note
	アプリケーション終了時に一度だけ呼び出して下さい。\n
*/
// ===========================================================================
extern void AoPresenceExit(void);

// ===========================================================================
//	AoPresenceSet
/*!
	プレゼンス設定

	@param presence	[in] 設定するプレゼンス
	@param is_trial	[in] 真：体験版　偽：製品版
	@note
	フレンドなどに公開する現在プレイ中のユーザのプレゼンスを設定します。\n
	この関数はゲーム中何度でも呼び出し可能です。\n
	既に同じプレゼンスが設定されている場合は何も行ないません。\n
	無効な引数を指定した場合は、アサートしAOD_PRESENCE_STANDBYを設定します。\n
*/
// ===========================================================================
extern void AoPresenceSet(AOE_PRESENCE presence, BOOL is_trial = FALSE);

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
