// ===========================================================================
/*!
	@file	aoNetRankXbox360Info.inc
	@brief	Xbox360ランキングボード情報

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//	AOS_NET_RANK_BOARD_INFO g_ao_net_rank_board_info[]
// ---------------------------------------------------------------------------
//!	ランキングボード情報配列
// ===========================================================================
static const AOS_NET_RANK_BOARD_INFO g_ao_net_rank_board_info[AOD_NET_RANK_BOARD_NUM] =
{
	// Z11TB
	{ STATS_VIEW_Z11TB, STATS_COLUMN_Z11TB_REGION, STATS_COLUMN_Z11TB_SONICTYPE, STATS_COLUMN_Z11TB_STIME },
	// Z11TN
	{ STATS_VIEW_Z11TN, STATS_COLUMN_Z11TN_REGION, -1,                           STATS_COLUMN_Z11TN_STIME },
	// Z11TS
	{ STATS_VIEW_Z11TS, STATS_COLUMN_Z11TS_REGION, -1,                           STATS_COLUMN_Z11TN_STIME },
	// Z11SB
	{ STATS_VIEW_Z11SB, STATS_COLUMN_Z11SB_REGION, STATS_COLUMN_Z11SB_SONICTYPE, -1                       },
	// Z11SN
	{ STATS_VIEW_Z11SN, STATS_COLUMN_Z11SN_REGION, -1,                           -1                       },
	// Z11SS
	{ STATS_VIEW_Z11SS, STATS_COLUMN_Z11SS_REGION, -1,                           -1                       },
	// Z12TB
	{ STATS_VIEW_Z12TB, STATS_COLUMN_Z12TB_REGION, STATS_COLUMN_Z12TB_SONICTYPE, STATS_COLUMN_Z12TB_STIME },
	// Z12TN
	{ STATS_VIEW_Z12TN, STATS_COLUMN_Z12TN_REGION, -1,                           STATS_COLUMN_Z12TN_STIME },
	// Z12TS
	{ STATS_VIEW_Z12TS, STATS_COLUMN_Z12TS_REGION, -1,                           STATS_COLUMN_Z12TN_STIME },
	// Z12SB
	{ STATS_VIEW_Z12SB, STATS_COLUMN_Z12SB_REGION, STATS_COLUMN_Z12SB_SONICTYPE, -1                       },
	// Z12SN
	{ STATS_VIEW_Z12SN, STATS_COLUMN_Z12SN_REGION, -1,                           -1                       },
	// Z12SS
	{ STATS_VIEW_Z12SS, STATS_COLUMN_Z12SS_REGION, -1,                           -1                       },
	// Z13TB
	{ STATS_VIEW_Z13TB, STATS_COLUMN_Z13TB_REGION, STATS_COLUMN_Z13TB_SONICTYPE, STATS_COLUMN_Z13TB_STIME },
	// Z13TN
	{ STATS_VIEW_Z13TN, STATS_COLUMN_Z13TN_REGION, -1,                           STATS_COLUMN_Z13TN_STIME },
	// Z13TS
	{ STATS_VIEW_Z13TS, STATS_COLUMN_Z13TS_REGION, -1,                           STATS_COLUMN_Z13TN_STIME },
	// Z13SB
	{ STATS_VIEW_Z13SB, STATS_COLUMN_Z13SB_REGION, STATS_COLUMN_Z13SB_SONICTYPE, -1                       },
	// Z13SN
	{ STATS_VIEW_Z13SN, STATS_COLUMN_Z13SN_REGION, -1,                           -1                       },
	// Z13SS
	{ STATS_VIEW_Z13SS, STATS_COLUMN_Z13SS_REGION, -1,                           -1                       },
	// Z1BTB
	{ STATS_VIEW_Z1BTB, STATS_COLUMN_Z1BTB_REGION, STATS_COLUMN_Z1BTB_SONICTYPE, STATS_COLUMN_Z1BTB_STIME },
	// Z1BTN
	{ STATS_VIEW_Z1BTN, STATS_COLUMN_Z1BTN_REGION, -1,                           STATS_COLUMN_Z1BTN_STIME },
	// Z1BTS
	{ STATS_VIEW_Z1BTS, STATS_COLUMN_Z1BTS_REGION, -1,                           STATS_COLUMN_Z1BTN_STIME },
	// Z1BSB
	{ STATS_VIEW_Z1BSB, STATS_COLUMN_Z1BSB_REGION, STATS_COLUMN_Z1BSB_SONICTYPE, -1                       },
	// Z1BSN
	{ STATS_VIEW_Z1BSN, STATS_COLUMN_Z1BSN_REGION, -1,                           -1                       },
	// Z1BSS
	{ STATS_VIEW_Z1BSS, STATS_COLUMN_Z1BSS_REGION, -1,                           -1                       },

	// Z21TB
	{ STATS_VIEW_Z21TB, STATS_COLUMN_Z21TB_REGION, STATS_COLUMN_Z21TB_SONICTYPE, STATS_COLUMN_Z21TB_STIME },
	// Z21TN
	{ STATS_VIEW_Z21TN, STATS_COLUMN_Z21TN_REGION, -1,                           STATS_COLUMN_Z21TN_STIME },
	// Z21TS
	{ STATS_VIEW_Z21TS, STATS_COLUMN_Z21TS_REGION, -1,                           STATS_COLUMN_Z21TN_STIME },
	// Z21SB
	{ STATS_VIEW_Z21SB, STATS_COLUMN_Z21SB_REGION, STATS_COLUMN_Z21SB_SONICTYPE, -1                       },
	// Z21SN
	{ STATS_VIEW_Z21SN, STATS_COLUMN_Z21SN_REGION, -1,                           -1                       },
	// Z21SS
	{ STATS_VIEW_Z21SS, STATS_COLUMN_Z21SS_REGION, -1,                           -1                       },
	// Z22TB
	{ STATS_VIEW_Z22TB, STATS_COLUMN_Z22TB_REGION, STATS_COLUMN_Z22TB_SONICTYPE, STATS_COLUMN_Z22TB_STIME },
	// Z22TN
	{ STATS_VIEW_Z22TN, STATS_COLUMN_Z22TN_REGION, -1,                           STATS_COLUMN_Z22TN_STIME },
	// Z22TS
	{ STATS_VIEW_Z22TS, STATS_COLUMN_Z22TS_REGION, -1,                           STATS_COLUMN_Z22TN_STIME },
	// Z22SB
	{ STATS_VIEW_Z22SB, STATS_COLUMN_Z22SB_REGION, STATS_COLUMN_Z22SB_SONICTYPE, -1                       },
	// Z22SN
	{ STATS_VIEW_Z22SN, STATS_COLUMN_Z22SN_REGION, -1,                           -1                       },
	// Z22SS
	{ STATS_VIEW_Z22SS, STATS_COLUMN_Z22SS_REGION, -1,                           -1                       },
	// Z23TB
	{ STATS_VIEW_Z23TB, STATS_COLUMN_Z23TB_REGION, STATS_COLUMN_Z23TB_SONICTYPE, STATS_COLUMN_Z23TB_STIME },
	// Z23TN
	{ STATS_VIEW_Z23TN, STATS_COLUMN_Z23TN_REGION, -1,                           STATS_COLUMN_Z23TN_STIME },
	// Z23TS
	{ STATS_VIEW_Z23TS, STATS_COLUMN_Z23TS_REGION, -1,                           STATS_COLUMN_Z23TN_STIME },
	// Z23SB
	{ STATS_VIEW_Z23SB, STATS_COLUMN_Z23SB_REGION, STATS_COLUMN_Z23SB_SONICTYPE, -1                       },
	// Z23SN
	{ STATS_VIEW_Z23SN, STATS_COLUMN_Z23SN_REGION, -1,                           -1                       },
	// Z23SS
	{ STATS_VIEW_Z23SS, STATS_COLUMN_Z23SS_REGION, -1,                           -1                       },
	// Z2BTB
	{ STATS_VIEW_Z2BTB, STATS_COLUMN_Z2BTB_REGION, STATS_COLUMN_Z2BTB_SONICTYPE, STATS_COLUMN_Z2BTB_STIME },
	// Z2BTN
	{ STATS_VIEW_Z2BTN, STATS_COLUMN_Z2BTN_REGION, -1,                           STATS_COLUMN_Z2BTN_STIME },
	// Z2BTS
	{ STATS_VIEW_Z2BTS, STATS_COLUMN_Z2BTS_REGION, -1,                           STATS_COLUMN_Z2BTN_STIME },
	// Z2BSB
	{ STATS_VIEW_Z2BSB, STATS_COLUMN_Z2BSB_REGION, STATS_COLUMN_Z2BSB_SONICTYPE, -1                       },
	// Z2BSN
	{ STATS_VIEW_Z2BSN, STATS_COLUMN_Z2BSN_REGION, -1,                           -1                       },
	// Z2BSS
	{ STATS_VIEW_Z2BSS, STATS_COLUMN_Z2BSS_REGION, -1,                           -1                       },

	// Z31TB
	{ STATS_VIEW_Z31TB, STATS_COLUMN_Z31TB_REGION, STATS_COLUMN_Z31TB_SONICTYPE, STATS_COLUMN_Z31TB_STIME },
	// Z31TN
	{ STATS_VIEW_Z31TN, STATS_COLUMN_Z31TN_REGION, -1,                           STATS_COLUMN_Z31TN_STIME },
	// Z31TS
	{ STATS_VIEW_Z31TS, STATS_COLUMN_Z31TS_REGION, -1,                           STATS_COLUMN_Z31TN_STIME },
	// Z31SB
	{ STATS_VIEW_Z31SB, STATS_COLUMN_Z31SB_REGION, STATS_COLUMN_Z31SB_SONICTYPE, -1                       },
	// Z31SN
	{ STATS_VIEW_Z31SN, STATS_COLUMN_Z31SN_REGION, -1,                           -1                       },
	// Z31SS
	{ STATS_VIEW_Z31SS, STATS_COLUMN_Z31SS_REGION, -1,                           -1                       },
	// Z32TB
	{ STATS_VIEW_Z32TB, STATS_COLUMN_Z32TB_REGION, STATS_COLUMN_Z32TB_SONICTYPE, STATS_COLUMN_Z32TB_STIME },
	// Z32TN
	{ STATS_VIEW_Z32TN, STATS_COLUMN_Z32TN_REGION, -1,                           STATS_COLUMN_Z32TN_STIME },
	// Z32TS
	{ STATS_VIEW_Z32TS, STATS_COLUMN_Z32TS_REGION, -1,                           STATS_COLUMN_Z32TN_STIME },
	// Z32SB
	{ STATS_VIEW_Z32SB, STATS_COLUMN_Z32SB_REGION, STATS_COLUMN_Z32SB_SONICTYPE, -1                       },
	// Z32SN
	{ STATS_VIEW_Z32SN, STATS_COLUMN_Z32SN_REGION, -1,                           -1                       },
	// Z32SS
	{ STATS_VIEW_Z32SS, STATS_COLUMN_Z32SS_REGION, -1,                           -1                       },
	// Z33TB
	{ STATS_VIEW_Z33TB, STATS_COLUMN_Z33TB_REGION, STATS_COLUMN_Z33TB_SONICTYPE, STATS_COLUMN_Z33TB_STIME },
	// Z33TN
	{ STATS_VIEW_Z33TN, STATS_COLUMN_Z33TN_REGION, -1,                           STATS_COLUMN_Z33TN_STIME },
	// Z33TS
	{ STATS_VIEW_Z33TS, STATS_COLUMN_Z33TS_REGION, -1,                           STATS_COLUMN_Z33TN_STIME },
	// Z33SB
	{ STATS_VIEW_Z33SB, STATS_COLUMN_Z33SB_REGION, STATS_COLUMN_Z33SB_SONICTYPE, -1                       },
	// Z33SN
	{ STATS_VIEW_Z33SN, STATS_COLUMN_Z33SN_REGION, -1,                           -1                       },
	// Z33SS
	{ STATS_VIEW_Z33SS, STATS_COLUMN_Z33SS_REGION, -1,                           -1                       },
	// Z3BTB
	{ STATS_VIEW_Z3BTB, STATS_COLUMN_Z3BTB_REGION, STATS_COLUMN_Z3BTB_SONICTYPE, STATS_COLUMN_Z3BTB_STIME },
	// Z3BTN
	{ STATS_VIEW_Z3BTN, STATS_COLUMN_Z3BTN_REGION, -1,                           STATS_COLUMN_Z3BTN_STIME },
	// Z3BTS
	{ STATS_VIEW_Z3BTS, STATS_COLUMN_Z3BTS_REGION, -1,                           STATS_COLUMN_Z3BTN_STIME },
	// Z3BSB
	{ STATS_VIEW_Z3BSB, STATS_COLUMN_Z3BSB_REGION, STATS_COLUMN_Z3BSB_SONICTYPE, -1                       },
	// Z3BSN
	{ STATS_VIEW_Z3BSN, STATS_COLUMN_Z3BSN_REGION, -1,                           -1                       },
	// Z3BSS
	{ STATS_VIEW_Z3BSS, STATS_COLUMN_Z3BSS_REGION, -1,                           -1                       },

	// Z41TB
	{ STATS_VIEW_Z41TB, STATS_COLUMN_Z41TB_REGION, STATS_COLUMN_Z41TB_SONICTYPE, STATS_COLUMN_Z41TB_STIME },
	// Z41TN
	{ STATS_VIEW_Z41TN, STATS_COLUMN_Z41TN_REGION, -1,                           STATS_COLUMN_Z41TN_STIME },
	// Z41TS
	{ STATS_VIEW_Z41TS, STATS_COLUMN_Z41TS_REGION, -1,                           STATS_COLUMN_Z41TN_STIME },
	// Z41SB
	{ STATS_VIEW_Z41SB, STATS_COLUMN_Z41SB_REGION, STATS_COLUMN_Z41SB_SONICTYPE, -1                       },
	// Z41SN
	{ STATS_VIEW_Z41SN, STATS_COLUMN_Z41SN_REGION, -1,                           -1                       },
	// Z41SS
	{ STATS_VIEW_Z41SS, STATS_COLUMN_Z41SS_REGION, -1,                           -1                       },
	// Z42TB
	{ STATS_VIEW_Z42TB, STATS_COLUMN_Z42TB_REGION, STATS_COLUMN_Z42TB_SONICTYPE, STATS_COLUMN_Z42TB_STIME },
	// Z42TN
	{ STATS_VIEW_Z42TN, STATS_COLUMN_Z42TN_REGION, -1,                           STATS_COLUMN_Z42TN_STIME },
	// Z42TS
	{ STATS_VIEW_Z42TS, STATS_COLUMN_Z42TS_REGION, -1,                           STATS_COLUMN_Z42TN_STIME },
	// Z42SB
	{ STATS_VIEW_Z42SB, STATS_COLUMN_Z42SB_REGION, STATS_COLUMN_Z42SB_SONICTYPE, -1                       },
	// Z42SN
	{ STATS_VIEW_Z42SN, STATS_COLUMN_Z42SN_REGION, -1,                           -1                       },
	// Z42SS
	{ STATS_VIEW_Z42SS, STATS_COLUMN_Z42SS_REGION, -1,                           -1                       },
	// Z43TB
	{ STATS_VIEW_Z43TB, STATS_COLUMN_Z43TB_REGION, STATS_COLUMN_Z43TB_SONICTYPE, STATS_COLUMN_Z43TB_STIME },
	// Z43TN
	{ STATS_VIEW_Z43TN, STATS_COLUMN_Z43TN_REGION, -1,                           STATS_COLUMN_Z43TN_STIME },
	// Z43TS
	{ STATS_VIEW_Z43TS, STATS_COLUMN_Z43TS_REGION, -1,                           STATS_COLUMN_Z43TN_STIME },
	// Z43SB
	{ STATS_VIEW_Z43SB, STATS_COLUMN_Z43SB_REGION, STATS_COLUMN_Z43SB_SONICTYPE, -1                       },
	// Z43SN
	{ STATS_VIEW_Z43SN, STATS_COLUMN_Z43SN_REGION, -1,                           -1                       },
	// Z43SS
	{ STATS_VIEW_Z43SS, STATS_COLUMN_Z43SS_REGION, -1,                           -1                       },
	// Z4BTB
	{ STATS_VIEW_Z4BTB, STATS_COLUMN_Z4BTB_REGION, STATS_COLUMN_Z4BTB_SONICTYPE, STATS_COLUMN_Z4BTB_STIME },
	// Z4BTN
	{ STATS_VIEW_Z4BTN, STATS_COLUMN_Z4BTN_REGION, -1,                           STATS_COLUMN_Z4BTN_STIME },
	// Z4BTS
	{ STATS_VIEW_Z4BTS, STATS_COLUMN_Z4BTS_REGION, -1,                           STATS_COLUMN_Z4BTN_STIME },
	// Z4BSB
	{ STATS_VIEW_Z4BSB, STATS_COLUMN_Z4BSB_REGION, STATS_COLUMN_Z4BSB_SONICTYPE, -1                       },
	// Z4BSN
	{ STATS_VIEW_Z4BSN, STATS_COLUMN_Z4BSN_REGION, -1,                           -1                       },
	// Z4BSS
	{ STATS_VIEW_Z4BSS, STATS_COLUMN_Z4BSS_REGION, -1,                           -1                       },

	// ZFBTB
	{ STATS_VIEW_ZFBTB, STATS_COLUMN_ZFBTB_REGION, STATS_COLUMN_ZFBTB_SONICTYPE, STATS_COLUMN_ZFBTB_STIME },
	// ZFBTN
	{ STATS_VIEW_ZFBTN, STATS_COLUMN_ZFBTN_REGION, -1,                           STATS_COLUMN_ZFBTN_STIME },
	// ZFBTS
	{ STATS_VIEW_ZFBTS, STATS_COLUMN_ZFBTS_REGION, -1,                           STATS_COLUMN_ZFBTN_STIME },
	// ZFBSB
	{ STATS_VIEW_ZFBSB, STATS_COLUMN_ZFBSB_REGION, STATS_COLUMN_ZFBSB_SONICTYPE, -1                       },
	// ZFBSN
	{ STATS_VIEW_ZFBSN, STATS_COLUMN_ZFBSN_REGION, -1,                           -1                       },
	// ZFBSS
	{ STATS_VIEW_ZFBSS, STATS_COLUMN_ZFBSS_REGION, -1,                           -1                       },

	// SS1TB
	{ STATS_VIEW_SS1T,  STATS_COLUMN_SS1T_REGION,  -1,                           STATS_COLUMN_SS1T_STIME  },
	// SS1TN
	{ STATS_VIEW_SS1T,  STATS_COLUMN_SS1T_REGION,  -1,                           STATS_COLUMN_SS1T_STIME  },
	// SS1TS
	{ STATS_VIEW_SS1T,  STATS_COLUMN_SS1T_REGION,  -1,                           STATS_COLUMN_SS1T_STIME  },
	// SS1SB
	{ STATS_VIEW_SS1S,  STATS_COLUMN_SS1S_REGION,  -1,                           -1                       },
	// SS1SN
	{ STATS_VIEW_SS1S,  STATS_COLUMN_SS1S_REGION,  -1,                           -1                       },
	// SS1SS
	{ STATS_VIEW_SS1S,  STATS_COLUMN_SS1S_REGION,  -1,                           -1                       },

	// SS2TB
	{ STATS_VIEW_SS2T,  STATS_COLUMN_SS2T_REGION,  -1,                           STATS_COLUMN_SS2T_STIME  },
	// SS2TN
	{ STATS_VIEW_SS2T,  STATS_COLUMN_SS2T_REGION,  -1,                           STATS_COLUMN_SS2T_STIME  },
	// SS2TS
	{ STATS_VIEW_SS2T,  STATS_COLUMN_SS2T_REGION,  -1,                           STATS_COLUMN_SS2T_STIME  },
	// SS2SB
	{ STATS_VIEW_SS2S,  STATS_COLUMN_SS2S_REGION,  -1,                           -1                       },
	// SS2SN
	{ STATS_VIEW_SS2S,  STATS_COLUMN_SS2S_REGION,  -1,                           -1                       },
	// SS2SS
	{ STATS_VIEW_SS2S,  STATS_COLUMN_SS2S_REGION,  -1,                           -1                       },

	// SS3TB
	{ STATS_VIEW_SS3T,  STATS_COLUMN_SS3T_REGION,  -1,                           STATS_COLUMN_SS3T_STIME  },
	// SS3TN
	{ STATS_VIEW_SS3T,  STATS_COLUMN_SS3T_REGION,  -1,                           STATS_COLUMN_SS3T_STIME  },
	// SS3TS
	{ STATS_VIEW_SS3T,  STATS_COLUMN_SS3T_REGION,  -1,                           STATS_COLUMN_SS3T_STIME  },
	// SS3SB
	{ STATS_VIEW_SS3S,  STATS_COLUMN_SS3S_REGION,  -1,                           -1                       },
	// SS3SN
	{ STATS_VIEW_SS3S,  STATS_COLUMN_SS3S_REGION,  -1,                           -1                       },
	// SS3SS
	{ STATS_VIEW_SS3S,  STATS_COLUMN_SS3S_REGION,  -1,                           -1                       },

	// SS4TB
	{ STATS_VIEW_SS4T,  STATS_COLUMN_SS4T_REGION,  -1,                           STATS_COLUMN_SS4T_STIME  },
	// SS4TN
	{ STATS_VIEW_SS4T,  STATS_COLUMN_SS4T_REGION,  -1,                           STATS_COLUMN_SS4T_STIME  },
	// SS4TS
	{ STATS_VIEW_SS4T,  STATS_COLUMN_SS4T_REGION,  -1,                           STATS_COLUMN_SS4T_STIME  },
	// SS4SB
	{ STATS_VIEW_SS4S,  STATS_COLUMN_SS4S_REGION,  -1,                           -1                       },
	// SS4SN
	{ STATS_VIEW_SS4S,  STATS_COLUMN_SS4S_REGION,  -1,                           -1                       },
	// SS4SS
	{ STATS_VIEW_SS4S,  STATS_COLUMN_SS4S_REGION,  -1,                           -1                       },

	// SS5TB
	{ STATS_VIEW_SS5T,  STATS_COLUMN_SS5T_REGION,  -1,                           STATS_COLUMN_SS5T_STIME  },
	// SS5TN
	{ STATS_VIEW_SS5T,  STATS_COLUMN_SS5T_REGION,  -1,                           STATS_COLUMN_SS5T_STIME  },
	// SS5TS
	{ STATS_VIEW_SS5T,  STATS_COLUMN_SS5T_REGION,  -1,                           STATS_COLUMN_SS5T_STIME  },
	// SS5SB
	{ STATS_VIEW_SS5S,  STATS_COLUMN_SS5S_REGION,  -1,                           -1                       },
	// SS5SN
	{ STATS_VIEW_SS5S,  STATS_COLUMN_SS5S_REGION,  -1,                           -1                       },
	// SS5SS
	{ STATS_VIEW_SS5S,  STATS_COLUMN_SS5S_REGION,  -1,                           -1                       },

	// SS6TB
	{ STATS_VIEW_SS6T,  STATS_COLUMN_SS6T_REGION,  -1,                           STATS_COLUMN_SS6T_STIME  },
	// SS6TN
	{ STATS_VIEW_SS6T,  STATS_COLUMN_SS6T_REGION,  -1,                           STATS_COLUMN_SS6T_STIME  },
	// SS6TS
	{ STATS_VIEW_SS6T,  STATS_COLUMN_SS6T_REGION,  -1,                           STATS_COLUMN_SS6T_STIME  },
	// SS6SB
	{ STATS_VIEW_SS6S,  STATS_COLUMN_SS6S_REGION,  -1,                           -1                       },
	// SS6SN
	{ STATS_VIEW_SS6S,  STATS_COLUMN_SS6S_REGION,  -1,                           -1                       },
	// SS6SS
	{ STATS_VIEW_SS6S,  STATS_COLUMN_SS6S_REGION,  -1,                           -1                       },

	// SS7TB
	{ STATS_VIEW_SS7T,  STATS_COLUMN_SS7T_REGION,  -1,                           STATS_COLUMN_SS7T_STIME  },
	// SS7TN
	{ STATS_VIEW_SS7T,  STATS_COLUMN_SS7T_REGION,  -1,                           STATS_COLUMN_SS7T_STIME  },
	// SS7TS
	{ STATS_VIEW_SS7T,  STATS_COLUMN_SS7T_REGION,  -1,                           STATS_COLUMN_SS7T_STIME  },
	// SS7SB
	{ STATS_VIEW_SS7S,  STATS_COLUMN_SS7S_REGION,  -1,                           -1                       },
	// SS7SN
	{ STATS_VIEW_SS7S,  STATS_COLUMN_SS7S_REGION,  -1,                           -1                       },
	// SS7SS
	{ STATS_VIEW_SS7S,  STATS_COLUMN_SS7S_REGION,  -1,                           -1                       },
};


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
