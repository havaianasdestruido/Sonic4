// ===========================================================================
/*!
	@file	gsEnvironment.cpp
	@brief	環境別システム設定モジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "gsEnvironment.h"
#if _IPHONE
#include "gsEnvironment_i.h"
#endif
#include "gs.h"
#include "gsMainSys.h"
#include "ao.h"

#if _WII
#include <revolution/sc.h>
#endif // _WII

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

#if defined(MTD_DEBUG)
// デバッグイベント
static void gsEnvDebugEvTask00(AMS_TCB* tcb);
#endif // defined(MTD_DEBUG)

// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ***************************************************************************
// キー
// ***************************************************************************
// ===========================================================================
//	u16 g_gs_env_key_decide
// ---------------------------------------------------------------------------
//!	決定キー
// ===========================================================================
u16 g_gs_env_key_decide = KEY_R_DOWN;

// ===========================================================================
//	u16 g_gs_env_key_cancel
// ---------------------------------------------------------------------------
//!	キャンセルキー
// ===========================================================================
u16 g_gs_env_key_cancel = KEY_R_RIGHT;

// ===========================================================================
//	u16 g_gs_env_key_up
// ---------------------------------------------------------------------------
//!	上方向キー
// ===========================================================================
u16 g_gs_env_key_up = KEY_L_UP;

// ===========================================================================
//	u16 g_gs_env_key_down
// ---------------------------------------------------------------------------
//!	下方向キー
// ===========================================================================
u16 g_gs_env_key_down = KEY_L_DOWN;

// ===========================================================================
//	u16 g_gs_env_key_left
// ---------------------------------------------------------------------------
//!	左方向キー
// ===========================================================================
u16 g_gs_env_key_left = KEY_L_LEFT;

// ===========================================================================
//	u16 g_gs_env_key_right
// ---------------------------------------------------------------------------
//!	右方向キー
// ===========================================================================
u16 g_gs_env_key_right = KEY_L_RIGHT;

// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ***************************************************************************
// 本体設定
// ***************************************************************************
// ===========================================================================
//	GSE_REGION g_gs_env_region
// ---------------------------------------------------------------------------
//!	リージョン
// ===========================================================================
static GSE_REGION g_gs_env_region = GSD_REGION_DEF;

// ===========================================================================
//	BOOL g_gs_env_is_asia
// ---------------------------------------------------------------------------
//!	アジアフラグ
// ===========================================================================
static BOOL g_gs_env_is_asia = FALSE;

// ===========================================================================
//	GSE_LANGUAGE g_gs_env_language
// ---------------------------------------------------------------------------
//!	言語
// ===========================================================================
static GSE_LANGUAGE g_gs_env_language = GSD_LANGUAGE_DEF;

// ===========================================================================
//	GSE_DECIDE_KEY g_gs_env_decide_key
// ---------------------------------------------------------------------------
//!	決定キー
// ===========================================================================
static GSE_DECIDE_KEY g_gs_env_decide_key = GSD_DECIDE_KEY_DEF;

#if _PS3

// ===========================================================================
//	SceNpCommunicationId g_gs_env_np_com_id
// ---------------------------------------------------------------------------
//!	タイトルID
// ===========================================================================
static const char* g_gs_env_ps3_titleid =
#if defined(HOG_RGN_JP)
	"NPJB00035"; // 日本版
#elif defined(HOG_RGN_US)
#if defined(HOG_RGN_KR)
	"NPHB00187"; // 韓国版
#else
	"NPUB30127"; // 北米版
#endif
#elif defined(HOG_RGN_EU)
	"NPEB00153"; // 欧州版
#endif

// ===========================================================================
//	char* g_gs_env_license_np_content_id
// ---------------------------------------------------------------------------
//!	購入ライセンス NPコンテンツID
// ===========================================================================
static const char* g_gs_env_license_np_content_id =
#if defined(HOG_RGN_JP)
	"JP0177-NPJB00035_00-SONICDLLICENSE00"; // 日本版
#elif defined(HOG_RGN_US)
#if defined(HOG_RGN_KR)
	"HP0177-NPHB00187_00-SONICDLLICENSE00"; // 韓国版
#else
	"UP0177-NPUB30127_00-SONICDLLICENSE00"; // 北米版
#endif
#elif defined(HOG_RGN_EU)
	"EP0177-NPEB00153_00-SONICDLLICENSE00"; // 欧州版
#endif

// ===========================================================================
//	SceNpCommunicationId g_gs_env_np_com_id
// ---------------------------------------------------------------------------
//!	NPコミュニケーションID
// ===========================================================================
static const SceNpCommunicationId g_gs_env_np_com_id = {
	{ 'N', 'P', 'W', 'R', '0', '0', '9', '5', '4' },
	'\0', 0, 0
};

// ===========================================================================
//	SceNpCommunicationPassphrase g_gs_np_com_passphrase
// ---------------------------------------------------------------------------
//!	NPコミュニケーションパスフレーズ
// ===========================================================================
static const SceNpCommunicationPassphrase g_gs_np_com_passphrase = {
	{
		0x0a,0xc3,0xd7,0x57,0x26,0xef,0xba,0xa4,
		0xdf,0x33,0xe2,0x5d,0xe6,0xaa,0xae,0x89,
		0x5a,0x89,0xe4,0xe7,0xd7,0x69,0x5c,0x86,
		0x75,0xb1,0xec,0xe4,0xf0,0x4f,0xd1,0x90,
		0xe9,0xdb,0x8a,0x9e,0xc7,0x19,0xf0,0x12,
		0xd3,0x49,0x7a,0xd4,0x91,0x7f,0x2d,0xfd,
		0xaf,0x86,0xd1,0x25,0xfb,0x76,0xea,0x8d,
		0x38,0xc5,0x29,0x71,0x08,0x65,0x95,0xe7,
		0xf9,0x6d,0x72,0x31,0xfb,0xd8,0xe9,0x31,
		0xa0,0x65,0x89,0x17,0x2f,0x61,0x39,0x25,
		0xa6,0x1b,0x42,0xe7,0x2d,0xc1,0x9c,0x8e,
		0xef,0x1a,0xdb,0xac,0x81,0xd3,0x09,0x5e,
		0x8d,0x4e,0xbc,0x8a,0x87,0x61,0x53,0x81,
		0x3c,0xec,0xb5,0x93,0xd7,0x42,0x11,0xd2,
		0x31,0xa6,0xf1,0xed,0x35,0xb4,0x2e,0xe9,
		0x86,0x5f,0xd9,0xa5,0x09,0x96,0xb7,0x4f
	}
};

// ===========================================================================
//	SceNpCommunicationSignature g_gs_np_com_signature
// ---------------------------------------------------------------------------
//!	NPコミュニケーションシグネチャ
// ===========================================================================
static const SceNpCommunicationSignature g_gs_np_com_signature = {
	{
		0xb9,0xdd,0xe1,0x3b,0x01,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x8c,0x1d,0x91,0xa7,
		0x86,0x00,0x8c,0xf0,0x1d,0x3e,0x9c,0x4b,
		0x9c,0x8a,0x99,0x27,0x60,0xdb,0x5f,0xb5,
		0x7b,0xfc,0xd4,0x9e,0x38,0x45,0x99,0xa7,
		0xdb,0x22,0x67,0x04,0xd4,0x8a,0xd1,0xea,
		0x83,0x07,0x9d,0x9d,0xd6,0xb6,0xcd,0x54,
		0x2e,0x7e,0x00,0x5a,0xf8,0x21,0xa2,0x07,
		0x1e,0xc0,0x57,0x5e,0x04,0x5f,0xe9,0xab,
		0xbb,0x97,0x84,0x2c,0x9a,0x83,0xb6,0x6d,
		0xf6,0x14,0xbc,0x8f,0xfe,0x00,0xfc,0x91,
		0xd5,0x08,0x3c,0xd7,0x80,0xee,0x6b,0x3f,
		0xb8,0x92,0xbf,0xc3,0xac,0x58,0xbd,0x76,
		0xae,0x6c,0x31,0x2c,0xde,0xa6,0x82,0xdd,
		0x1a,0x46,0x54,0x68,0x74,0x49,0x7a,0xe2,
		0xec,0x76,0x46,0x96,0x7f,0xdf,0xbf,0xc5,
		0x3f,0x2e,0xd6,0x2f,0xfc,0xbd,0x90,0x5a,
		0x17,0x97,0xcd,0xc7,0xbf,0xe6,0x4d,0x87,
		0xe8,0x72,0x7a,0x22,0x72,0xba,0xd4,0xc2,
		0xda,0xbf,0xad,0x31,0x6b,0xb9,0xaa,0xfa
	}
};

// ===========================================================================
//	int g_gs_np_parentallock_age
// ---------------------------------------------------------------------------
//! パレンタルロックする年齢
// ===========================================================================
static const int g_gs_np_parentallock_age =
#if defined(HOG_RGN_JP)
	0;
#elif defined(HOG_RGN_US)
#if defined(HOG_RGN_KR)
	0;
#else
	6;
#endif
#elif defined(HOG_RGN_EU)
	7;
#endif

#endif // _PS3

#if _WII

// ===========================================================================
//	GSE_WII_LANGUAGE g_gs_env_wii_language
// ---------------------------------------------------------------------------
//!	Wiiシステム言語
// ===========================================================================
static GSE_WII_LANGUAGE g_gs_env_wii_language = GSD_WII_LANGUAGE_DEF;

#endif // _WII

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ***************************************************************************
// 初期化
// ***************************************************************************
// ===========================================================================
//	GsEnvInit
/*!
	システム初期化処理

	@note
	各種本体設定などを参照し、言語設定やキー設定などを行います。\n
	このモジュールの関数を使用する前に呼び出すようにして下さい。\n
*/
// ===========================================================================
void GsEnvInit(void)
{
	// 初期値設定
	g_gs_env_region = GSD_REGION_DEF;
	g_gs_env_language = GSD_LANGUAGE_DEF;
	g_gs_env_decide_key = GSD_DECIDE_KEY_DEF;
	g_gs_env_key_decide = KEY_R_DOWN;
	g_gs_env_key_cancel = KEY_R_RIGHT;
	g_gs_env_key_up = KEY_L_UP;
	g_gs_env_key_down = KEY_L_DOWN;
	g_gs_env_key_left = KEY_L_LEFT;
	g_gs_env_key_right = KEY_L_RIGHT;

	// プラットフォーム別に各種設定取得
#if _PC
	{
		// 全て固定
#if defined(HOG_RGN_JP)
		g_gs_env_region = GSD_REGION_JP;
		g_gs_env_is_asia = FALSE;
#elif defined(HOG_RGN_US)
		g_gs_env_region = GSD_REGION_US;
#if defined(HOG_RGN_KR)
		g_gs_env_is_asia = TRUE;
#else
		g_gs_env_is_asia = FALSE;
#endif // defined(HOG_RGN_KR)
#elif defined(HOG_RGN_EU)
		g_gs_env_region = GSD_REGION_EU;
		g_gs_env_is_asia = FALSE;
#else
#error
#endif
		g_gs_env_language = GSD_LANGUAGE_JP;
		g_gs_env_decide_key = GSD_DECIDE_KEY_X;
		g_gs_env_key_decide = KEY_R_DOWN;
		g_gs_env_key_cancel = KEY_R_RIGHT;
		g_gs_env_key_up = KEY_L_UP;
		g_gs_env_key_down = KEY_L_DOWN;
		g_gs_env_key_left = KEY_L_LEFT;
		g_gs_env_key_right = KEY_L_RIGHT;
	}
#elif _XBOX
	{
		// リージョン取得
		switch (XGetGameRegion()) {
		case XC_GAME_REGION_ASIA_JAPAN:
			g_gs_env_region = GSD_REGION_JP;
			g_gs_env_is_asia = FALSE;
			break;

		case XC_GAME_REGION_NA_ALL:
			g_gs_env_region = GSD_REGION_US;
			g_gs_env_is_asia = FALSE;
			break;

		case XC_GAME_REGION_EUROPE_ALL:
		case XC_GAME_REGION_EUROPE_AUNZ:
		case XC_GAME_REGION_EUROPE_REST:
			g_gs_env_region = GSD_REGION_EU;
			g_gs_env_is_asia = FALSE;
			break;

		case XC_GAME_REGION_ASIA_ALL:
		case XC_GAME_REGION_ASIA_CHINA:
		case XC_GAME_REGION_ASIA_REST:
		case XC_GAME_REGION_RESTOFWORLD_ALL:
		default:
			g_gs_env_region = GSD_REGION_US;
			g_gs_env_is_asia = TRUE;
			break;
		}

		// 言語取得
		switch (XGetLanguage()) {
		case XC_LANGUAGE_JAPANESE:
			g_gs_env_language = GSD_LANGUAGE_JP;
			break;
		case XC_LANGUAGE_FRENCH:
			g_gs_env_language = GSD_LANGUAGE_FR;
			break;
		case XC_LANGUAGE_ITALIAN:
			g_gs_env_language = GSD_LANGUAGE_IT;
			break;
		case XC_LANGUAGE_GERMAN:
			g_gs_env_language = GSD_LANGUAGE_GE;
			break;
		case XC_LANGUAGE_SPANISH:
			g_gs_env_language = GSD_LANGUAGE_SP;
			break;
		default:
			g_gs_env_language = GSD_LANGUAGE_US;
			break;
		}

		// キーは固定
		g_gs_env_decide_key = GSD_DECIDE_KEY_X;
		g_gs_env_key_decide = KEY_R_DOWN;
		g_gs_env_key_cancel = KEY_R_RIGHT;
		g_gs_env_key_up = KEY_L_UP;
		g_gs_env_key_down = KEY_L_DOWN;
		g_gs_env_key_left = KEY_L_LEFT;
		g_gs_env_key_right = KEY_L_RIGHT;
	}
#elif _PS3
	{
		// リージョン取得
#if defined(HOG_RGN_JP)
		g_gs_env_region = GSD_REGION_JP;
		g_gs_env_is_asia = FALSE;
#elif defined(HOG_RGN_US)
		g_gs_env_region = GSD_REGION_US;
#if defined(HOG_RGN_KR)
		g_gs_env_is_asia = TRUE;
#else
		g_gs_env_is_asia = FALSE;
#endif
#elif defined(HOG_RGN_EU)
		g_gs_env_region = GSD_REGION_EU;
		g_gs_env_is_asia = FALSE;
#else
#error
#endif

		// 言語取得
		int value;
		if (cellSysutilGetSystemParamInt(
			CELL_SYSUTIL_SYSTEMPARAM_ID_LANG, &value) >= 0)
		{
			switch (value) {
			case CELL_SYSUTIL_LANG_JAPANESE:
				g_gs_env_language = GSD_LANGUAGE_JP;
				break;
			case CELL_SYSUTIL_LANG_FRENCH:
				g_gs_env_language = GSD_LANGUAGE_FR;
				break;
			case CELL_SYSUTIL_LANG_ITALIAN:
				g_gs_env_language = GSD_LANGUAGE_IT;
				break;
			case CELL_SYSUTIL_LANG_GERMAN:
				g_gs_env_language = GSD_LANGUAGE_GE;
				break;
			case CELL_SYSUTIL_LANG_SPANISH:
				g_gs_env_language = GSD_LANGUAGE_SP;
				break;
			default:
				g_gs_env_language = GSD_LANGUAGE_US;
				break;
			}
		}
		else {
			// エラー(デフォルトのまま)
		//	amAssert(0);
		}

		// 決定キー取得
		if (cellSysutilGetSystemParamInt(
			CELL_SYSUTIL_SYSTEMPARAM_ID_ENTER_BUTTON_ASSIGN, &value) >= 0)
		{
			switch (value) {
			case CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CIRCLE:
				g_gs_env_decide_key = GSD_DECIDE_KEY_O;
				g_gs_env_key_decide = KEY_R_RIGHT;
				g_gs_env_key_cancel = KEY_R_DOWN;
				break;
			default:
				g_gs_env_decide_key = GSD_DECIDE_KEY_X;
				g_gs_env_key_decide = KEY_R_DOWN;
				g_gs_env_key_cancel = KEY_R_RIGHT;
				break;
			}
		}
		else {
			// エラー(デフォルトのまま)
		//	amAssert(0);
		}

		// 方向キーは固定
		g_gs_env_key_up = KEY_L_UP;
		g_gs_env_key_down = KEY_L_DOWN;
		g_gs_env_key_left = KEY_L_LEFT;
		g_gs_env_key_right = KEY_L_RIGHT;
	}
#elif _WII
	{
		// リージョン取得
#if defined(HOG_RGN_JP)
		g_gs_env_region = GSD_REGION_JP;
		g_gs_env_is_asia = FALSE;
#elif defined(HOG_RGN_US)
		g_gs_env_region = GSD_REGION_US;
#if defined(HOG_RGN_KR)
		g_gs_env_is_asia = TRUE;
#else
		g_gs_env_is_asia = FALSE;
#endif
#elif defined(HOG_RGN_EU)
		g_gs_env_region = GSD_REGION_EU;
		g_gs_env_is_asia = FALSE;
#else
#error
#endif

		// 言語取得
		switch (SCGetLanguage()) {
		case SC_LANG_JAPANESE:
			g_gs_env_language = GSD_LANGUAGE_JP;
			g_gs_env_wii_language = GSD_WII_LANGUAGE_JP;
			break;
		case SC_LANG_ENGLISH:
			g_gs_env_language = GSD_LANGUAGE_US;
			g_gs_env_wii_language = GSD_WII_LANGUAGE_EN;
			break;
		case SC_LANG_FRENCH:
			g_gs_env_language = GSD_LANGUAGE_FR;
			g_gs_env_wii_language = GSD_WII_LANGUAGE_FR;
			break;
		case SC_LANG_ITALIAN:
			g_gs_env_language = GSD_LANGUAGE_IT;
			g_gs_env_wii_language = GSD_WII_LANGUAGE_IT;
			break;
		case SC_LANG_GERMAN:
			g_gs_env_language = GSD_LANGUAGE_GE;
			g_gs_env_wii_language = GSD_WII_LANGUAGE_GE;
			break;
		case SC_LANG_SPANISH:
			g_gs_env_language = GSD_LANGUAGE_SP;
			g_gs_env_wii_language = GSD_WII_LANGUAGE_SP;
			break;
		case SC_LANG_DUTCH:
			g_gs_env_language = GSD_LANGUAGE_DEF;
			g_gs_env_wii_language = GSD_WII_LANGUAGE_DU;
			break;
		default:
			g_gs_env_language = GSD_LANGUAGE_DEF;
			g_gs_env_wii_language = GSD_WII_LANGUAGE_DEF;
			break;
		}

		// 決定キーは固定
		g_gs_env_decide_key = GSD_DECIDE_KEY_O;
		g_gs_env_key_decide = KEY_R_RIGHT;
		g_gs_env_key_cancel = KEY_R_DOWN;

		// 方向キーは固定
//		g_gs_env_key_up = KEY_L_RIGHT;
//		g_gs_env_key_down = KEY_L_LEFT;
//		g_gs_env_key_left = KEY_L_UP;
//		g_gs_env_key_right = KEY_L_DOWN;
		// amPad内で処理できるので回す必要ない
		g_gs_env_key_up = KEY_L_UP;
		g_gs_env_key_down = KEY_L_DOWN;
		g_gs_env_key_left = KEY_L_LEFT;
		g_gs_env_key_right = KEY_L_RIGHT;
	}
#elif _IPHONE
	{
		// リージョン取得
		g_gs_env_region = GsEnvGetRegionIphone();
		// 言語取得
		g_gs_env_language = GsEnvGetLanguageIphone();
	}
#endif
}


// ***************************************************************************
// 本体設定取得
// ***************************************************************************
// ===========================================================================
//	GsEnvGetRegion
/*!
	リージョン取得

	@return リージョン
	@note
	本体に設定されているリージョンを返します。\n
*/
// ===========================================================================
GSE_REGION GsEnvGetRegion(void)
{
	return g_gs_env_region;
}

// ===========================================================================
//	GsEnvIsRegionAsia
/*!
	リージョンアジア判定

	@return 真：アジア　偽：それ以外
*/
// ===========================================================================
BOOL GsEnvIsRegionAsia(void)
{
	return g_gs_env_is_asia;
}

// ===========================================================================
//	GsEnvGetLanguage
/*!
	言語取得

	@return 言語
	@note
	本体に設定されている言語を返します。\n
	ゲーム中で表示する言語は、全てこの関数の戻り値を元に決定して下さい。\n
*/
// ===========================================================================
GSE_LANGUAGE GsEnvGetLanguage(void)
{
	return g_gs_env_language;
}

// ===========================================================================
//	GeEnvGetDecideKey
/*!
	決定キー取得

	@return 決定キー
	@note
	本体設定にある決定キーを返します。\n
	■PS3\n
	GSD_DECIDE_KEY_O : ○キー\n
	GSD_DECIDE_KEY_X : ×キー\n
	■Xbox360
	GSD_DECIDE_KEY_O : Bキー\n
	GSD_DECIDE_KEY_X : Aキー\n
	■Wii
	GSD_DECIDE_KEY_O : \n
	GSD_DECIDE_KEY_X : \n
	amPadなどのキーIDとは互換がありませんので注意して下さい。\n
	メニューなどで、ヘルプ表示する画像の切り替えに使用して下さい。\n
*/
// ===========================================================================
GSE_DECIDE_KEY GeEnvGetDecideKey(void)
{
	return g_gs_env_decide_key;
}


#if _PS3
// ***************************************************************************
// PS3
// ***************************************************************************
// ===========================================================================
//	GsEnvGetPs3TitleId
/*!
	タイトルID取得

	@return タイトルID
*/
// ===========================================================================
const char* GsEnvGetPs3TitleId(void)
{
	return g_gs_env_ps3_titleid;
}

// ===========================================================================
//	GsEnvGetPs3LicenseNpContentId
/*!
	購入ライセンスNPコンテンツID取得

	@return 購入ライセンスNPコンテンツID
*/
// ===========================================================================
const char* GsEnvGetPs3LicenseNpContentId(void)
{
	return g_gs_env_license_np_content_id;
}

// ===========================================================================
//	GsEnvGetPs3ComId
/*!
	NPコミュニケーションID取得

	@return NPコミュニケーションID
*/
// ===========================================================================
const SceNpCommunicationId* GsEnvGetPs3ComId(void)
{
	return &g_gs_env_np_com_id;
}

// ===========================================================================
//	GsEnvGetPs3ComPassphrase
/*!
	NPコミュニケーションパスフレーズ取得

	@return NPコミュニケーションパスフレーズ
*/
// ===========================================================================
const SceNpCommunicationPassphrase* GsEnvGetPs3ComPassphrase(void)
{
	return &g_gs_np_com_passphrase;
}

// ===========================================================================
//	GsEnvGetPs3ComSignature
/*!
	NPコミュニケーションシグネチャ取得

	@return NPコミュニケーションシグネチャ
*/
// ===========================================================================
const SceNpCommunicationSignature* GsEnvGetPs3ComSignature(void)
{
	return &g_gs_np_com_signature;
}

// ===========================================================================
//	GsEnvGetPs3ParentalLockAge
/*!
	パレンタルロックを行う年齢取得

	@return パレンタルロックを行う年齢
	@note
	この関数の返す値よりもユーザの年齢が低かった場合（同じはOK）は
	パレンタルロックを行う必要があります。\n
*/
// ===========================================================================
int GsEnvGetPs3ParentalLockAge(void)
{
	return g_gs_np_parentallock_age;
}
#endif // _PS3


#if _WII
// ***************************************************************************
// PS3
// ***************************************************************************
// ===========================================================================
//	GsEnvGetWiiSystemLanguage
/*!
	Wiiシステム言語取得

	@return Wiiシステム言語
	@note
	ストラップ画面の言語などは、GsEnvGetLanguage関数ではなく
	この関数の戻り値に従って切り替えてください。\n
*/
// ===========================================================================
GSE_WII_LANGUAGE GsEnvGetWiiSystemLanguage(void)
{
	return g_gs_env_wii_language;
}
#endif // _WII


#if defined(MTD_DEBUG)
// ***************************************************************************
// デバッグ
// ***************************************************************************
// ===========================================================================
//	GsEnvDebugEventSetting
/*!
	設定変更イベント

	@note
	デバッグを目的として、
	本来は本体設定から取得する各種設定を任意に切り替える機能を提供します。\n
	このイベントを呼び出す前に、GsEnvInit関数を呼び出して有る必要があります。\n
*/
// ===========================================================================
void GsEnvDebugEventSetting(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	// タスク作成
	AMS_TCB* tcb =
		amTaskMake(gsEnvDebugEvTask00, NULL, 0, 0, 0, "gsEnv::Debug");

	// ワーク初期化
	u32* work = (u32*)amTaskGetWork(tcb);
	*work = 0;

	// タスク開始
	amTaskStart(tcb);
}

// ===========================================================================
//	GsEnvDebugGetDecideKeyChar
/*!
	デバッグ表示用の決定キー文字取得

	@return 決定キー文字
	@note
	デバッグ用途として、決定キーを識別するための文字列を返します。\n
	デバッグ文字で画面に操作説明を書く際の使用を想定してます。\n
	Xbox360では'A'、PS3では'X'など\n
*/
// ===========================================================================
char GsEnvDebugGetDecideKeyChar(void)
{
	char ret;
	if (g_gs_env_decide_key == GSD_DECIDE_KEY_X) {
#if _PC || _XBOX
		ret = 'A';
#elif _PS3
		ret = 'X';
#elif _WII
		ret = '1';
#elif _IPHONE
		// 未作成
		ret = 'A';
#endif
	}
	else {
#if _PC || _XBOX
		ret = 'B';
#elif _PS3
		ret = 'O';
#elif _WII
		ret = '2';
#elif _IPHONE
		// 未作成
		ret = 'B';
#endif
	}
	return ret;
}

// ===========================================================================
//	GsEnvDebugGetCancelKeyChar
/*!
	デバッグ表示用のキャンセルキー文字取得

	@return キャンセルキー文字
*/
// ===========================================================================
char GsEnvDebugGetCancelKeyChar(void)
{
	char ret;
	if (g_gs_env_decide_key != GSD_DECIDE_KEY_X) {
#if _PC || _XBOX
		ret = 'A';
#elif _PS3
		ret = 'X';
#elif _WII
		ret = '1';
#elif _IPHONE
		// 未作成
		ret = 'A';
#endif
	}
	else {
#if _PC || _XBOX
		ret = 'B';
#elif _PS3
		ret = 'O';
#elif _WII
		ret = '2';
#elif _IPHONE
		// 未作成
		ret = 'B';
#endif
	}
	return ret;
}
#endif // defined(MTD_DEBUG)

// ----- Static Functions --------------------（スタティック関数の定義：局所）

#if defined(MTD_DEBUG)
// ***************************************************************************
// デバッグイベント
// ***************************************************************************
// ===========================================================================
//! タスク00
// ===========================================================================
void gsEnvDebugEvTask00(AMS_TCB* tcb)
{
	// ワーク取得
	u32& select = *((u32*)amTaskGetWork(tcb));

	// 選択切り替え
	if (AoPadSomeoneMStand(GSD_KEY_UP) >= 0) {
		if (select > 0) {
			select -= 1;
		}
		else {
			select = 2;
		}
	}
	if (AoPadSomeoneMStand(GSD_KEY_DOWN) >= 0) {
		if (select < 2) {
			select += 1;
		}
		else {
			select = 0;
		}
	}

	// 項目変更
	if (select == 0) {
		// リージョン
		u32 region = (u32)g_gs_env_region;
		if (AoPadSomeoneMStand(GSD_KEY_LEFT) >= 0) {
			if (region > 0) {
				region -= 1;
			}
			else {
				region = (u32)(GSD_REGION_NUM - 1);
			}
		}
		if (AoPadSomeoneMStand(GSD_KEY_RIGHT) >= 0) {
			if (region < (u32)(GSD_REGION_NUM - 1)) {
				region += 1;
			}
			else {
				region = 0;
			}
		}
		g_gs_env_region = (GSE_REGION)region;
	}
	else if (select == 1) {
		// 言語
		u32 language = (u32)g_gs_env_language;
		if (AoPadSomeoneMStand(GSD_KEY_LEFT) >= 0) {
			if (language > 0) {
				language -= 1;
			}
			else {
				language = (u32)(GSD_LANGUAGE_NUM - 1);
			}
		}
		if (AoPadSomeoneMStand(GSD_KEY_RIGHT) >= 0) {
			if (language < (u32)(GSD_LANGUAGE_NUM - 1)) {
				language += 1;
			}
			else {
				language = 0;
			}
		}
		g_gs_env_language = (GSE_LANGUAGE)language;
	}
	else if (select == 2) {
		// 決定キー
		u32 decide = (u32)g_gs_env_decide_key;
		if (AoPadSomeoneMStand(GSD_KEY_LEFT) >= 0) {
			if (decide > 0) {
				decide -= 1;
			}
			else {
				decide = (u32)(GSD_DECIDE_KEY_NUM - 1);
			}
		}
		if (AoPadSomeoneMStand(GSD_KEY_RIGHT) >= 0) {
			if (decide < (u32)(GSD_DECIDE_KEY_NUM - 1)) {
				decide += 1;
			}
			else {
				decide = 0;
			}
		}
		g_gs_env_decide_key = (GSE_DECIDE_KEY)decide;
	}

	// 反映
	if (g_gs_env_decide_key == GSD_DECIDE_KEY_O) {
		g_gs_env_key_decide = KEY_R_RIGHT;
		g_gs_env_key_cancel = KEY_R_DOWN;
	}
	else {
		g_gs_env_key_decide = KEY_R_DOWN;
		g_gs_env_key_cancel = KEY_R_RIGHT;
	}

	// 終了
	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		amTaskDelete(tcb);
		SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
		SyChangeNextEvt();
	}

	// タイトル表示
	amPrint(4, 4, "ENVIRONMENT SETTIONG.");

	// 操作方法表示
#if _PC || _XBOX
	amPrint(4, 6, "DECIDE : A or B");
#elif _PS3
	amPrint(4, 6, "DECIDE : O or X");
#elif _WII
	amPrint(4, 6, "DECIDE : 1 or 2");
#elif _IPHONE
	// 未作成
#endif

	// 項目表示
	amPrint(4, (s32)(9 + select), ">");
	amPrint(5, 9, "REGION     :");
	amPrint(5, 10, "LANGUAGE   :");
	amPrint(5, 11, "DECIDE KEY :");
	switch (g_gs_env_region) {
	case GSD_REGION_JP:
		amPrint(18, 9, "JP");
		break;
	case GSD_REGION_US:
		amPrint(18, 9, "US");
		break;
	case GSD_REGION_EU:
		amPrint(18, 9, "EU");
		break;
	default:
		amAssert(0);
		break;
	}
	switch (g_gs_env_language) {
	case GSD_LANGUAGE_JP:
		amPrint(18, 10, "JP");
		break;
	case GSD_LANGUAGE_US:
		amPrint(18, 10, "US");
		break;
	case GSD_LANGUAGE_FR:
		amPrint(18, 10, "FR");
		break;
	case GSD_LANGUAGE_IT:
		amPrint(18, 10, "IT");
		break;
	case GSD_LANGUAGE_GE:
		amPrint(18, 10, "GE");
		break;
	case GSD_LANGUAGE_SP:
		amPrint(18, 10, "SP");
		break;
	default:
		amAssert(0);
		break;
	}
	switch (g_gs_env_decide_key) {
	case GSD_DECIDE_KEY_O:
#if _PC || _XBOX
		amPrint(18, 11, "B");
#elif _PS3
		amPrint(18, 11, "O");
#elif _WII
		amPrint(18, 11, "2");
#elif _IPHONE
	// 未作成
#endif
		break;
	case GSD_DECIDE_KEY_X:
#if _PC || _XBOX
		amPrint(18, 11, "A");
#elif _PS3
		amPrint(18, 11, "X");
#elif _WII
		amPrint(18, 11, "1");
#elif _IPHONE
	// 未作成
#endif
		break;
	default:
		amAssert(0);
	}
}
#endif // defined(MTD_DEBUG)


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
