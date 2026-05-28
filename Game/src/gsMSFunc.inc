// ================================================================
/*!
  @file gsMSFunc.inc
  @brief メインシステム イベント関数定義

  @author Ishizaki
                Copyright(c) 2009 Dimps

  $Id: gsMSFunc.inc 14077 2010-04-15 12:44:12Z kouji_hokazono $
  $Date:: 2010-04-15 21:44:12 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 *
 */

//----- Include Files -------------------------------------------------------
#include "dmTitle.h"
#if defined SONIC4_TRIAL
#include "dmStgSlct.h"
#endif // #if defined SONIC4_TRIAL
#include "dmOption.h"
#include "dmStaffRoll.h"
#include "gmMain.h"
#include "gmPauseMenu.h"
#include "dmRankSysWait.h"
#include "gmSpStageBra.h"
#include "evBuyScreen.h"
#include "dmLogoSonic.h"
#include "dmLogoSega.h"
#include "dmSaveAttentionPS3.h"
#include "dmMovie.hpp"

#if (defined(HOG_RGN_US) && !defined(HOG_RGN_KR)) || _XBOX || _PC
#include "dmLogoESRB.h"
#endif // #if (defined(HOG_RGN_US) && !defined(HOG_RGN_KR)) || _XBOX || _PC

#if _PC || _PS3 || _XBOX || _WII
#include "dmRanking.h"
#endif

#if _PC || _WII
#include "dmFileSlct.h"
#include "dmWiiExpOpe.h"
#endif

#if (defined(HOG_RGN_JP) && _PS3) || _PC
#include "dmPS3Kido.h"
#endif

#include "dbgDmyFunc.h"
#if defined(MTD_DEBUG)
#include "dbgEvtSelector.hpp"
#include "gsEnvironment.h"
#include "aoTest.h"
#include "gsTrial.h"
#include "gsStrapImage.h"
#include "dbgBuyScreen.h"
#if defined GSD_DEBUG_MODEL_BUILD
#include "dbgModelBuild.h"
#endif
#endif // defined(MTD_DEBUG)


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
#if defined(MTD_DEBUG)
/****************************************************************************/
// ランチャー
/****************************************************************************/
/*** ランチャーから呼び出す関数 ***/
#define GSD_LAUNCHER_UP_FUNC		(NULL)
#define GSD_LAUNCHER_DOWN_FUNC		(NULL)
#define GSD_LAUNCHER_LEFT_FUNC		(NULL)
#define GSD_LAUNCHER_RIGHT_FUNC		(NULL)
#define GSD_LAUNCHER_A_FUNC			(NULL)
#define GSD_LAUNCHER_B_FUNC			(NULL)
#endif // #if defined(MTD_DEBUG)



/****************************************************************************/
// イベントデータ
/*
	必要となるイベント(デモ, ゲーム等)のデータを登録する
	登録内容は以下の通り
		初期化関数
		終了時実行関数
		ソフトリセット時関数
		システム側初期化関数
		システム側終了関数
		次イベントID
		属性
	関数については最低限、初期化関数を登録する。
	次イベントは、通常イベント移行時に分岐する可能性のあるイベントIDを登録する。
	イベントID定義はヘッダにおいて定義し(USRE_EVT_ID)、SYS_EVT_DATAの内容が
	IDの並びと同じ様にしておく事。
*/	
/****************************************************************************/
static const SYS_EVT_DATA	_gs_evt_data[]	= {
//== 予備 ====================================================================
	{
		NULL,									/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{0, 0, 0, 0, 0, 0, 0, 0},				/* 次イベントID           */
		0,										/* 属性                   */
	},

//== システム初期化 (GSD_EVT_ID_SYS_INIT) ====================================
	{
		GsMainSysSystemInitEvent,				/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
#if defined(MTD_DEBUG) && ((defined (GSD_DEBUG_DEMO_SELECT) && !defined(HOG_PRESENT_ROM_IPHONE)) || TARGET_IPHONE_SIMULATOR)
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
#else

			// 通常遷移先
#if _PC || _XBOX

			GSD_EVT_ID_LOGO_SEGA,
			GSD_EVT_ID_LOGO_ESRB,
			// スキップ時遷移先
			GSD_EVT_ID_TITLE,

#elif _PS3

#if defined(HOG_RGN_JP)
			GSD_EVT_ID_PS3_KIDO,
#elif (defined(HOG_RGN_US) && !defined(HOG_RGN_KR))
			GSD_EVT_ID_LOGO_ESRB,
#else
			GSD_EVT_ID_SAVE_ATTENTION,
#endif
			// スキップ時遷移先
			GSD_EVT_ID_TITLE,
			GSD_EVT_ID_TITLE,	// Xboxとあわせる

#elif _WII

#if (defined(HOG_RGN_US) && !defined(HOG_RGN_KR))
			GSD_EVT_ID_LOGO_ESRB,
#else
			GSD_EVT_ID_WII_EXP_OPE,
#endif // (defined(HOG_RGN_US) && !defined(HOG_RGN_KR))
			// スキップ時遷移先
			GSD_EVT_ID_TITLE,
			GSD_EVT_ID_TITLE,	// Xboxとあわせる

#elif _IPHONE
			GSD_EVT_ID_LOGO_SEGA,
			GSD_EVT_ID_LOGO_SEGA,
			// スキップ時遷移先
			GSD_EVT_ID_TITLE,
#endif

#endif
#if defined(MTD_DEBUG)
			GSD_EVT_ID_DEBUG_DEMO,
#else
			0,
#endif
			0, 0, 0, 0,
		},
		0,										/* 属性                   */
	},

//== セガロゴ (GSD_EVT_ID_LOGO_SEGA) ===============================================
	{
		DmLogoSegaInit,							/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
#if defined (GSD_DEBUG_DEMO_SELECT)
			GSD_EVT_ID_DEBUG_DEMO,
#else
			GSD_EVT_ID_LOGO_SONIC,
#endif
				0, 0, 0, 0, 0, 0, 0,			/* 次イベントID           */
		},
		0,										/* 属性                   */
	},

//== タイトル (GSD_EVT_ID_TITLE) =============================================
	{
		DmTitleStart,							/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
#if defined(HOG_PRESENT_ROM_IPHONE)
			GSD_EVT_ID_MAINGAME,
			GSD_EVT_ID_MAP,
			GSD_EVT_ID_OPTION,
			GSD_EVT_ID_TITLE,
			GSD_EVT_ID_LOGO_SEGA,
			GSD_EVT_ID_TITLE,
#elif defined HOG_INLINE3_ROM
			GSD_EVT_ID_MAINGAME,
			0,
			0,
			0,
			0,
			0,
#elif defined HOG_ALPHA_ROM
			GSD_EVT_ID_MAINGAME,
			GSD_EVT_ID_MAP,
			GSD_EVT_ID_OPTION,
			GSD_EVT_ID_RANKING,
			GSD_EVT_ID_LOGO_SEGA,
			GSD_EVT_ID_TITLE,
#else
#if defined (GSD_DEBUG_DEMO_SELECT)
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
#else
			GSD_EVT_ID_MAINGAME,
			GSD_EVT_ID_MAP,
			GSD_EVT_ID_OPTION,
			GSD_EVT_ID_RANKING,
			GSD_EVT_ID_LOGO_SEGA,
			GSD_EVT_ID_TITLE,
#endif
#endif
			0, 0,							/* 次イベントID           */
		},
		0,										/* 属性                   */
	},

//== メインメニュー (GSD_EVT_ID_MAINMENU) ====================================
	{
		DmMainMenuStart,					/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
#if defined(HOG_PRESENT_ROM_IPHONE)
			GSD_EVT_ID_MAINGAME,
			GSD_EVT_ID_MAP,
			GSD_EVT_ID_OPTION,
			GSD_EVT_ID_TITLE,
			GSD_EVT_ID_LOGO_SEGA,
			GSD_EVT_ID_TITLE,
#elif defined HOG_INLINE3_ROM
			GSD_EVT_ID_MAINGAME,
			0,
			0,
			0,
			0,
			0,
#elif defined HOG_ALPHA_ROM
			GSD_EVT_ID_MAINGAME,
			GSD_EVT_ID_MAP,
			GSD_EVT_ID_OPTION,
			GSD_EVT_ID_RANKING,
			GSD_EVT_ID_LOGO_SEGA,
			GSD_EVT_ID_TITLE,
#else
#if defined (GSD_DEBUG_DEMO_SELECT)
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
#else
			GSD_EVT_ID_MAINGAME,
			GSD_EVT_ID_MAP,
			GSD_EVT_ID_OPTION,
			GSD_EVT_ID_RANKING,
			GSD_EVT_ID_LOGO_SEGA,
			GSD_EVT_ID_TITLE,
#endif
#endif
			0, 0,							/* 次イベントID           */
		},
		0,										/* 属性                   */
	},

//== 全体マップ(ステージセレクト) (GSD_EVT_ID_MAP) =============================================
	{
#if defined SONIC4_TRIAL
		NULL,
#else
		DmStgSlctStart,							/* 初期化関数             */
#endif // #if defined SONIC4_TRIAL
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
#if defined HOG_INLINE3_ROM
			GSD_EVT_ID_MAINGAME,
			GSD_EVT_ID_MAINGAME,
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
#elif defined HOG_ALPHA_ROM
			GSD_EVT_ID_MAINGAME,
			GSD_EVT_ID_SPSTAGE_BRANCH,
			GSD_EVT_ID_RANKING,
			GSD_EVT_ID_MAINMENU,
			GSD_EVT_ID_TITLE,
#else
#if defined (GSD_DEBUG_DEMO_SELECT)
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
#else
			GSD_EVT_ID_MAINGAME,
			GSD_EVT_ID_SPSTAGE_BRANCH,
			GSD_EVT_ID_RANKING,
			GSD_EVT_ID_MAINMENU,
			GSD_EVT_ID_TITLE,
#endif
#endif
		
			0, 0, 0,							/* 次イベントID           */
		},
		0,										/* 属性                   */
	},

//== メインゲーム (GSD_EVT_ID_MAINGAME) ======================================
	{
		GmMainInit,								/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
#if defined (HOG_INLINE3_ROM) || defined (HOG_PRESENT_ROM_IPHONE)
			GSD_EVT_ID_MAINGAME,
			GSD_EVT_ID_TITLE,
			GSD_EVT_ID_TITLE,
			GSD_EVT_ID_TITLE,
			GSD_EVT_ID_TITLE,
			GSD_EVT_ID_TITLE,
			GSD_EVT_ID_TITLE,
			0,									/* 次イベントID           */
#else //defined (HOG_INLINE3_ROM) || defined (HOG_PRESENT_ROM_IPHONE)
#if defined GSD_DEBUG_DEMO_SELECT
			GSD_EVT_ID_DEBUG_DEMO,
#else
			GSD_EVT_ID_MAP,						// 全体マップ
#endif
			GSD_EVT_ID_MAINGAME,				// メインゲーム
			GSD_EVT_ID_ENDING,					// エンディング
			GSD_EVT_ID_SPSTAGE_BRANCH,			// スペステ移行
#if defined GSD_DEBUG_DEMO_SELECT
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
#else
			GSD_EVT_ID_MAINMENU,				// メインメニュー(初回プレイ未クリア時戻り先)
			GSD_EVT_ID_TITLE,					// 強制システム移行
#endif
#if _IPHONE
			GSD_EVT_ID_MOVIE,
#elif !_WII
			GSD_EVT_ID_BUYSCREEN,
#else
			0,
#endif
			0,									/* 次イベントID           */
#endif //defined (HOG_INLINE3_ROM) || defined (HOG_PRESENT_ROM_IPHONE)
		},
		0,										/* 属性                   */
	},

//== ランキング (GSD_EVT_ID_RANKING) ============================================
	{

#if _PC || _PS3 || _XBOX || _WII
		DmRankingStart,							/* 初期化関数             */
#else
		DbgDummyInitResult,						/* 初期化関数             */
#endif
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
		// ※このイベント変数は参照しません。(前イベントを参照)
			GSD_EVT_ID_LOGO_SEGA,
			0, 0, 0, 0, 0, 0, 0,				/* 次イベントID           */
		},
		0,										/* 属性                   */
	},

//== オプション (GSD_EVT_ID_OPTION) ==========================================
	{
		DmOptionStart,							/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
			GSD_EVT_ID_STAFFROLL,
			GSD_EVT_ID_TITLE,
			0, 0, 0, 0, 0, 0,					/* 次イベントID           */
		},
		0,										/* 属性                   */
	},

//== エンディング (GSD_EVT_ID_ENDING) ========================================
	{
		GmMainInit,								/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
			GSD_EVT_ID_STAFFROLL,
			GSD_EVT_ID_TITLE,
			GSD_EVT_ID_STAFFROLL,				// エンディング(通常ゲーム時)
			GSD_EVT_ID_STAFFROLL,				// スペステ移行(通常ゲーム時)
#if defined (GSD_DEBUG_DEMO_SELECT)
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
#else
			GSD_EVT_ID_MAINMENU,				// メインメニュー(初回プレイ未クリア時戻り先)
			GSD_EVT_ID_TITLE,					// 強制システム移行
#endif
#if !_WII
			GSD_EVT_ID_STAFFROLL,				// 購入画面(通常ゲーム時)
#else
			GSD_EVT_ID_STAFFROLL,
#endif
			0,									/* 次イベントID           */
		},
		0,										/* 属性                   */
	},


//== スタッフロール (GSD_EVT_ID_STAFFROLL) ===================================
	{
//		DbgDummyInitStaffRoll,					/* 初期化関数             */
		DmStaffRollStart,						/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
#if !defined(MTD_DEBUG)
			GSD_EVT_ID_MAINMENU,
			GSD_EVT_ID_TITLE,
#else
#if defined (GSD_DEBUG_DEMO_SELECT)
			GSD_EVT_ID_DEBUG_DEMO,
			GSD_EVT_ID_DEBUG_DEMO,
#else
			GSD_EVT_ID_MAINMENU,
			GSD_EVT_ID_TITLE,
#endif
#endif
		
			0, 0, 0, 0, 0, 0,				/* 次イベントID           */
		},
		0,										/* 属性                   */
	},

//== スペシャルステージ分岐 (GSD_EVT_ID_SPSTAGE_BRANCH) ======================
	{
		GmSpStageBranchInit,					/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
			GSD_EVT_ID_MAINGAME,
			0, 0, 0, 0, 0, 0, 0,				/* 次イベントID           */
		},
		0,										/* 属性                   */
	},

#if _PC || _XBOX || _PS3 || _IPHONE
//== 製品版購入画面 (GSD_EVT_ID_BUYSCREEN) ===================================
	{
		EvBuyScreenStart,
		NULL, NULL, NULL, NULL,
		{
			GSD_EVT_ID_MAP,				// 製品版時遷移先
			GSD_EVT_ID_MAINMENU,		// 体験版時遷移先
			0, 0, 0, 0, 0, 0,
		},
		0,
	},
#endif // _PC || _XBOX || _PS3 || _IPHONE


//== ソニックチームロゴ (GSD_EVT_ID_LOGO_SONIC) ==============================
	{
		DmLogoSonicInit,						/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
#if defined (GSD_DEBUG_DEMO_SELECT)
			GSD_EVT_ID_DEBUG_DEMO,
#else
			GSD_EVT_ID_TITLE,
#endif
			0, 0, 0, 0, 0, 0, 0,				/* 次イベントID           */
		},
		0,										/* 属性                   */
	},

#if (defined(HOG_RGN_US) && !defined(HOG_RGN_KR)) || _XBOX || _PC
//== ESRBロゴ (GSD_EVT_ID_LOGO_ESRB) =========================================
	{
		DmLogoESRBInit,							/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
#if defined (GSD_DEBUG_DEMO_SELECT)
			GSD_EVT_ID_DEBUG_DEMO,
#else
#if _WII
			GSD_EVT_ID_WII_EXP_OPE,
#elif _PS3
			GSD_EVT_ID_SAVE_ATTENTION,
#else
			GSD_EVT_ID_LOGO_SEGA,
#endif
#endif
			0, 0, 0, 0, 0, 0, 0,				/* 次イベントID           */
		},
		0,										/* 属性                   */
	},
#endif // #if (defined(HOG_RGN_US) && !defined(HOG_RGN_KR)) || _XBOX || _PC

#if _PS3
//== 起動直後オートセーブ注意表示 (GSD_EVT_ID_SAVE_ATTENTION) ================
	{
		DmSaveAttentionStartPS3,
		NULL, NULL, NULL, NULL,
		{
			GSD_EVT_ID_LOGO_SEGA,
			0, 0, 0, 0, 0, 0, 0,
		},
		0,
	},
#endif // _PS3

#if _WII || _PC
//== Wii 横持ち促し画面 (GSD_EVT_ID_WII_EXP_OPE) =============================
	{
		DmWiiExpOpeInit,						/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
#if defined (GSD_DEBUG_DEMO_SELECT)
			GSD_EVT_ID_DEBUG_DEMO,
#else
			GSD_EVT_ID_LOGO_SEGA,
#endif
			0, 0, 0, 0, 0, 0, 0,				/* 次イベントID           */
		},
		0,										/* 属性                   */
	},
#endif // #if _WII || _PC

#if (defined(HOG_RGN_JP) && _PS3) || _PC
//== PS3起動直後画面 (GSD_EVT_ID_PS3_KIDO) ==================================
	{
		DmPS3KidoInit,							/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
#if defined (GSD_DEBUG_DEMO_SELECT)
			GSD_EVT_ID_DEBUG_DEMO,
#else
#if _PS3
			GSD_EVT_ID_SAVE_ATTENTION,
#else	// _PC その他例外
			GSD_EVT_ID_LOGO_ESRB,
#endif
#endif
			0, 0, 0, 0, 0, 0, 0,				/* 次イベントID           */
		},
		0,										/* 属性                   */
	},
#endif // #if (defined(HOG_RGN_JP) && _PS3) || _PC

#if _IPHONE
//== ムービー (GSD_EVT_ID_MOVIE) ==================================
	{
		DmMovieInit,							/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{
			GSD_EVT_ID_BUYSCREEN,
			0, 0, 0, 0, 0, 0, 0,				/* 次イベントID           */
		},
		0,										/* 属性                   */
	},
#endif //_IPHONE

// ===========================================================================
// 以降デバッグ
// ===========================================================================
#if defined (MTD_DEBUG)

//== デモデバッグ ============================================================
#if defined (GSD_DEBUG_DEMO_SELECT) || defined (GSD_DEBUG_DEMO_SELECT_TOP)
	{
		DbgEvtSelector,							/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{0, 0, 0, 0, 0, 0, 0, 0},				/* 次イベントID           */
		0,										/* 属性                   */
	},

#if !_IPHONE
	{ // GSD_EVT_ID_DEBUG_PAUSE_MENU
		GmPauseMenuDebugEventStart,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},
#endif	// #if !_IPHONE

	{ // GSD_EVT_ID_DEBUG_LANG_CHANGE
		GsEnvDebugEventSetting,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},
#endif	// #if defined (GSD_DEBUG_DEMO_SELECT) || defined (GSD_DEBUG_DEMO_SELECT_TOP)

#if defined GSD_DEBUG_MODEL_BUILD
	{
		DbgModelBuildInit,						/* 初期化関数             */
		NULL,									/* 終了時実行関数         */
		NULL,									/* ソフトリセット時関数   */
		NULL,									/* システム側初期化関数   */
		NULL,									/* システム側終了関数     */
		{0, 0, 0, 0, 0, 0, 0, 0},				/* 次イベントID           */
		0,										/* 属性                   */
	},
#endif // #if defined GSD_DEBUG_MODEL_BUILD

#if !_IPHONE
	{ // GSD_EVT_ID_AOTESTSYSMSG
		AoTestSysMsgStart,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},

	{ // GSD_EVT_ID_AOTESTSYSMSG2
		AoTestSysMsgStart2,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},

	{ // GSD_EVT_ID_AOTESTSAVE
		AoTestSaveStart,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},

	{ // GSD_EVT_ID_AOTESTTROPHY
		AoTestTrophyStart,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},

	{ // GSD_EVT_ID_AOTESTNET
		AoTestNetStart,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},

	{ // GSD_EVT_ID_AOTESTWAIT
		DmRankSysWaitTestStart,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},

	{ // GSD_EVT_ID_AOTESTPRESENCE
		AoTestPresenceStart,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},

	{ // GSD_EVT_ID_AOTESTEXIT
		AoTestExitStart,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},

	{ // GSD_EVT_ID_AOTESTAVATARAWARD
		AoTestAvatarAwardStart,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},

	{ // GSD_EVT_ID_AOTESTSAFEFRAME
		AoTestSafeFrameStart,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},

	{ // GSD_EVT_ID_AOTESTWIIFATAL
		AoTestWiiFatalError,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},

	{ // GSD_EVT_ID_GSTRIALTEST
		GsTrialDebugEvent,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},

	{ // GSD_EVT_ID_GSSTRAPIMAGETEST
		GsStrapImageDebugEvent,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},
#endif	// #if !_IPHONE

	{ // GSD_EVT_ID_DBGBUYSCREEN
		DbgBuyScreenEventStart,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},

#if _PS3
	{ // GSD_EVT_ID_SAVE_ATTENTION_DEBUG
		DmSaevAttentionStartPS3Debug,
		NULL, NULL, NULL, NULL,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		0,
	},
#endif // _PS3

#endif	// #if defined (MTD_DEBUG)
};

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
