// ===========================================================================
/*!
	@file	dmRanking.cpp
	@brief	デモ・ランキング画面

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: dmRanking.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 *
 *
 */
#if !_IPHONE
// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#include "dmRanking.h"
#include "../library/ao/include/aoTexture.h"
#include "../library/ao/include/aoAction.h"

#include "gs.h"
#include "gsMainSys.h"
#include "gsEnvironment.h"
#include "izFade.h"
#include "aoWinSys.h"
#include "akUtil.h"
#include "mtTask.h"
#include "gmTask.h"

#include "dmLoading.h"
#include "dmRankSys.h"
#include "objObject.h"
#include "gmPlayer.h"
#include "hgTrophy.h"
#include "dmSave.h"

#include "gsSound.h"
#include "dmSound.h"
#include "dmSndBgmPlayer.h"

// データヘッダ
#include "common/ace/D_RANK.HMA"
#include "common/ace/D_RANK_JP.HMA"

// 共通データヘッダ
#include "common/ace/D_CMN_BG.HMA"
#include "common/ace/D_CMN_BTN.HMA"
#include "common/ace/D_CMN_OBI.HMA"
#include "common/ace/D_CMN_WIN.HMA"
#include "common/ace/D_CMN_MSG_JP.HMA"

// ----- Macros ------------------------------------------------（マクロ定義）

#define DMD_RANK_TASK_PAUSELEVEL	(0x7fff)
#define DMD_RANK_TASK_PRIO_MAIN		(0x2000)
#define DMD_RANK_TASK_GROUP_MAIN	(0)

#define DMD_RANK_FILE_PATH_NUM_MAX	(60)

#define DMD_RANK_CMN_DATA_FILENAME	(GSS_BASE_PATH"DEMO/CMN/D_CMN_WIN.AMB")
#define DMD_RANK_DATA_FILENAME		(GSS_BASE_PATH"DEMO/RANKING/D_RANK.AMB")

#define DMD_RANK_SIZE_WIDTH			(960.0f)
#define DMD_RANK_SIZE_HEIGHT		(720.0f)
#define DMD_RANK_SIZE_HALF_WIDTH	(480.0f)
#define DMD_RANK_SIZE_HALF_HEIGHT	(360.0f)


// プライオリティ設定
#define DMD_RANK_DRAW_PRIO_ACT_SLCT	(0x2000)
#define DMD_RANK_DRAW_PRIO_RANKING	(0x2e00)
#define DMD_RANK_DRAW_PRIO_BG		(0x1000)
#define DMD_RANK_DRAW_PRIO_RIDE_BG	(0x2c00)
#define DMD_RANK_DRAW_PRIO_FIX		(0x3000)
#define DMD_RANK_DRAW_PRIO_WIN		(0x3000)
#define DMD_RANK_DRAW_PRIO_WIN_FIX	(0x4000)

// 表示関連
#define DMD_RANK_DISP_RANKING_NUM	(10)
#define DMD_RANK_DISP_NAMECHAR_NUM	(16)		// ここはプラットフォームごとに文字数を変える

//#define DMD_RANK_ACT_TABLE_		(60)
#define DMD_RANK_ZONE_TABLE_TOP_POS_X	(138.0f)
#define DMD_RANK_ZONE_TABLE_TOP_POS_Y	(160.0f)
#define DMD_RANK_ACT_TABLE_DIST_Y	(96.0f)

//#define DMD_RANK_STAGE_TAB_DISP_POS_X	()
#define DMD_RANK_STAGE_TAB_NODISP_POS_X	(1120.f)

#define DMD_RANK_NO_ACTIVE_NUM_ID	(9)

#define DMD_RANK_DRAW_STATE_ID		(10)

#define DMD_RANK_DISP_ACT_NUM_POS_X	(8.f)
#define DMD_RANK_DISP_STG_NUM_POS_X	(20.f)

#define DMD_RANK_DISP_MENU_CRSR_BASE_POS_X	(349.f)
#define DMD_RANK_DISP_MENU_CRSR_BASE_POS_Y	(206.f)


#if !_WII
#define DMD_RANK_DISP_LIST_NAME_CHAR_NUM	(16)
#else
#define DMD_RANK_DISP_LIST_NAME_CHAR_NUM	(10)
#endif

// ウインドウ関連
#define DMD_RANK_WINDOW_SIZE_W		(380.f)
#define DMD_RANK_WINDOW_SIZE_H		(180.f)
#define DMD_RANK_WIN_DEF_RATE		(1.0f)
#define DMD_RANK_WIN_DISP_TIME		(60)

// フェード関連
#define DMD_RANK_FADEIN_TIME		(32.0f)
#define DMD_RANK_FADEOUT_TIME		(32.0f)

#define DMD_RANK_BGM_FADEIN_TIME	(32)
#define DMD_RANK_BGM_FADEOUT_TIME	(32)

// 演出関連
#define DMD_RANK_ZONE_EFCT_TIME		(16.0f)
#define DMD_RANK_ACT_EFCT_TIME		(16.0f)
#define DMD_RANK_WIN_EFCT_TIME		(8.0f)
#define DMD_RANK_OBI_MOVE_START_POS	(1120.f)//(1216.f)
#define DMD_RANK_OBI_MOVE_END_POS	(-1120.f)
#define DMD_RANK_OBI_MOVE_SPEED		(-3.f)
#define DMD_RANK_ACT_VRTCL_CHNG_DIST	(128.f)
#define DMD_RANK_ACT_VRTCL_CHNG_NUM	(3)
#define DMD_RANK_CRSR_MOVE_TIME		(8.0f)

#define DMD_RANK_OBI_EFCT_TIME			(16.0f)
#define DMD_RANK_OBI_NODISP_POS_Y		(192.f)
#define DMD_RANK_OBI_DISP_POS_Y			(0.f)

#define DMD_RANK_MENU_TAB_EFCT_TIME		(32.f)	
#define DMD_RANK_MENU_TAB_NODISP_POS	(720.f)	
#define DMD_RANK_MENU_TAB_DISP_POS		(0.f)	

#define DMD_RANK_VIEW_TAB_EFCT_TIME		(32.f)	
#define DMD_RANK_VIEW_TAB_NODISP_POS	(1000.f)	
#define DMD_RANK_VIEW_TAB_DISP_POS		(0.f)	

#define DMD_RANK_VIEW_RANK_MAX_NUM		(1000000)
#define DMD_RANK_VIEW_SCORE_MAX_NUM		(1000000000)
#define DMD_RANK_VIEW_TIME_MAX_NUM		(36000)
#define DMD_RANK_VIEW_RANK_DIGIT_NUM	(7)
#define DMD_RANK_VIEW_SCORE_DIGIT_NUM	(9)

#define DMD_RANK_VIEW_CHNG_PAGE_NUM		(10)

#define DMD_RANK_VIEW_INDEX_ANIME_FRM	(12)
#define DMD_RANK_VIEW_SONIC_ICON_FRM	(30)

#define DMD_RANK_ZONE_CHNG_BTN_END_FRM	(12)

#if !_WII
#define DMD_RANK_VIEW_TAB_MAX_NUM		(3)
#else
#define DMD_RANK_VIEW_TAB_MAX_NUM		(2)
#endif

#if _WII
#define DMD_RANK_DISP_SCALE_TEXT		(1.4f)
#endif

#if _PS3 || _XBOX || _PC
#define DMD_RANKT_OBI_MSG_SCALE_SIZE	(1.2f)
#endif

// フラグ関連
#define DMD_RANK_FLAG_EXIT					(1 << 0)		//!< 終了フラグ
#define DMD_RANK_FLAG_CANCEL				(1 << 1)		//!< キャンセル
#define DMD_RANK_FLAG_DECIDE				(1 << 2)		//!< 決定フラグ
#define DMD_RANK_FLAG_DISP_MENU				(1 << 3)
#define DMD_RANK_FLAG_WIN_EFCT_END			(1 << 4)
#define DMD_RANK_FLAG_MENU_UP_INPUT			(1 << 5)
#define DMD_RANK_FLAG_MENU_DOWN_INPUT		(1 << 6)
#define DMD_RANK_FLAG_MENU_LEFT_INPUT		(1 << 7)
#define DMD_RANK_FLAG_MENU_RIGHT_INPUT		(1 << 8)
#define DMD_RANK_FLAG_MENU_CHNG_CRSR_EFCT	(1 << 9)
#define DMD_RANK_FLAG_RANK_CHNG_CRSR_EFCT	(1 << 10)
#define DMD_RANK_FLAG_RANK_CHNG_ACT_LEFT	(1 << 11)
#define DMD_RANK_FLAG_RANK_CHNG_ACT_RIGHT	(1 << 12)
#define DMD_RANK_FLAG_RANK_CHNG_TAB_LEFT	(1 << 13)
#define DMD_RANK_FLAG_RANK_CHNG_TAB_RIGHT	(1 << 14)
#define DMD_RANK_FLAG_RANK_CHNG_RANK_UP		(1 << 15)
#define DMD_RANK_FLAG_RANK_CHNG_RANK_DOWN	(1 << 16)
#define DMD_RANK_FLAG_RANK_CHNG_TIME		(1 << 17)
#define DMD_RANK_FLAG_RANK_CHNG_SONIC		(1 << 18)
#define DMD_RANK_FLAG_WIN_MSG_END			(1 << 19)

#define DMD_RANK_FLAG_NET_CNCT_END			(1 << 20)
#define DMD_RANK_FLAG_IS_UPLOAD_CANCEL		(1 << 21)
#define DMD_RANK_FLAG_IS_UPLOAD_START		(1 << 22)
#define DMD_RANK_FLAG_UPLOAD_END			(1 << 23)
#define DMD_RANK_FLAG_DOWNLOAD_END			(1 << 24)

#define DMD_RANK_FLAG_SET_DISP_RANK_NEAR	(1 << 25)

#define DMD_RANK_FLAG_CHNG_GAME_MODE_EFCT	(1 << 26)
#define DMD_RANK_FLAG_CHNG_SONIC_EFCT		(1 << 27)

#define DMD_RANK_FLAG_CHNG_ZONE_WIN_EFCT	(1 << 28)

#define DMD_RANK_FLAG_NEXT_EVT_TITLE		(1 << 29)
#define DMD_RANK_FLAG_SIGN_OUT_EXIT			(1 << 30)	// サインアウト時の終了フラグ

#if _XBOX
#define DMD_RANK_FLAG_OPEN_GAMER_TAG		(1 << 31)	// XBOXのみなので分ける
#endif

// アクション表示フラグ関連
#define DMD_RANK_DISP_FLAG_WIN_ACT			(1 << 0)
#define DMD_RANK_DISP_FLAG_RANK_MENU		(1 << 1)
#define DMD_RANK_DISP_FLAG_RANK_VIEW		(1 << 2)
#define DMD_RANK_DISP_FLAG_MENU_TAB			(1 << 3)
#define DMD_RANK_DISP_FLAG_RANK_LIST		(1 << 4)
#define DMD_RANK_DISP_FLAG_NOW_UPDATE		(1 << 5)
#define DMD_RANK_DISP_FLAG_WIN_DRAW			(1 << 6)

#define DMD_RANK_DISP_FLAG_CHECK_NET_ERROR	(1 << 31)


// ----- Macro Functions -----------------------------------（処理マクロ定義）


// ----- Definitions -------------------------------------------（定数の宣言）
//! 次のイベント
typedef enum tag_DME_RANK_NEXT_EVT
{
	DME_RANK_NEXT_EVT_MAINMENU = 0,	//!< メインメニュー
	DME_RANK_NEXT_EVT_MAINGAME,		//!< ポーズメニュー

	DME_RANK_NEXT_EVT_MAX
} DME_RANK_NEXT_EVT;


typedef enum tag_DME_RANK_DATA_TYPE
{
	DME_RANK_DATA_TYPE_CMN_DATA = 0,	//!< 共通データ
	DME_RANK_DATA_TYPE_LANG_DATA,		//!< 言語別データ
	
	DME_RANK_DATA_TYPE_MAX,
	DME_RANK_DATA_TYPE_NONE
} DME_RANK_DATA_TYPE;

//! ZONEタイプ
typedef enum tag_DME_RANK_PLATFORM_TYPE
{
	DME_RANK_ZONE_TYPE_1 = 0,		//!< ZONE1
	DME_RANK_ZONE_TYPE_2,			//!< ZONE2
	DME_RANK_ZONE_TYPE_3,			//!< ZONE3
	DME_RANK_ZONE_TYPE_4,			//!< ZONE4
	DME_RANK_ZONE_TYPE_FINAL,		//!< FINAL
	DME_RANK_ZONE_TYPE_SPECIAL,		//!< スペステ
	
	DME_RANK_ZONE_TYPE_MAX,
	DME_RANK_ZONE_TYPE_NONE
} DME_RANK_ZONE_TYPE;


//! 現在のモードSTATE
typedef enum tag_DME_RANK_MODE_STATE
{
	DME_RANK_MODE_STATE_RANK_MENU = 0,	//!< ランキングメニュー
	DME_RANK_MODE_STATE_RANK_MAIN,		//!< メインのランキング画面
	
	DME_RANK_MODE_STATE_NUM,
	DME_RANK_MODE_STATE_NONE
} DME_RANK_MODE_STATE;


//! ランキング画面の表示するランキングの種類
typedef enum tag_DME_RANK_PLAY_MODE
{
	DME_RANK_DISP_RANK_SCORE = 0,	//!< スコア
	DME_RANK_DISP_RANK_TIME,		//!< タイム
	
	DME_RANK_DISP_RANK_NUM,
	DME_RANK_DISP_RANK_NONE
} DME_RANK_DISP_RANK;


//! ウインドウ表示パターンタイプ
typedef enum tag_DME_RANK_WIN
{
	DME_RANK_WIN_NET_CONNECT = 0,	//!< 
	DME_RANK_WIN_DO_YOU_REGIST,		//!< 
	DME_RANK_WIN_NOW_REGIST,		//!< 
	DME_RANK_WIN_NOW_UPDATE,		//!< 
	
	DME_RANK_WIN_NUM,
	DME_RANK_WIN_NONE
} DME_RANK_WIN;


//! スコアソニックタイプ
typedef enum tag_DME_RANK_SONIC_TYPE
{
	DME_RANK_SONIC_TYPE_ALL = 0,	//!< 
	DME_RANK_SONIC_TYPE_NORMAL,		//!< 
	DME_RANK_SONIC_TYPE_SUPER,		//!< 
	
	DME_RANK_SONIC_TYPE_NUM,
	DME_RANK_SONIC_TYPE_NONE
} DME_RANK_SONIC_TYPE;


//! スコアタイプ
typedef enum tag_DME_RANK_SCORE_TYPE
{
	DME_RANK_SCORE_TYPE_ALL = 0,	//!< 
	DME_RANK_SCORE_TYPE_MYSCORE,	//!< 
	DME_RANK_SCORE_TYPE_FRIENDS,	//!< 
	
	DME_RANK_SCORE_TYPE_NUM,
	DME_RANK_SCORE_TYPE_NONE
} DME_RANK_SCORE_TYPE;


//! 地域タイプ
typedef enum tag_DME_RANK_AREA_TYPE
{
	DME_RANK_AREA_TYPE_JP = 0,		//!< 
	DME_RANK_AREA_TYPE_US,			//!< 
	DME_RANK_AREA_TYPE_EU,			//!< 
	DME_RANK_AREA_TYPE_OTHER,		//!< 
	
	DME_RANK_AREA_TYPE_NUM,
	DME_RANK_AREA_TYPE_NONE
} DME_RANK_AREA_TYPE;


//! 表示中のページを示す
typedef enum tag_DME_RANK_STAGE
{
	DME_RANK_STAGE_1_1 = 0,		//!< 
	DME_RANK_STAGE_1_2,			//!< 
	DME_RANK_STAGE_1_3,			//!< 
	DME_RANK_STAGE_1_B,			//!< 
	DME_RANK_STAGE_2_1,			//!< 
	DME_RANK_STAGE_2_2,			//!< 
	DME_RANK_STAGE_2_3,			//!< 
	DME_RANK_STAGE_2_B,			//!< 
	DME_RANK_STAGE_3_1,			//!< 
	DME_RANK_STAGE_3_2,			//!< 
	DME_RANK_STAGE_3_3,			//!< 
	DME_RANK_STAGE_3_B,			//!< 
	DME_RANK_STAGE_4_1,			//!< 
	DME_RANK_STAGE_4_2,			//!< 
	DME_RANK_STAGE_4_3,			//!< 
	DME_RANK_STAGE_4_B,			//!< 
	DME_RANK_STAGE_F_1,			//!< 
	DME_RANK_STAGE_S_1,			//!< 
	DME_RANK_STAGE_S_2,			//!< 
	DME_RANK_STAGE_S_3,			//!< 
	DME_RANK_STAGE_S_4,			//!< 
	DME_RANK_STAGE_S_5,			//!< 
	DME_RANK_STAGE_S_6,			//!< 
	DME_RANK_STAGE_S_7,			//!< 
	
	DME_RANK_STAGE_NUM,
	DME_RANK_STAGE_NONE
} DME_RANK_STAGE;




//! アクションテーブル

#if 1
typedef enum tag_DME_RANK_ACT
{
	// モード共通・言語共通
	ACT_TAB_TITLE1 = 0,		//!< 
	ACT_VIEW_TITLE_LINE_L,	//!< 
	ACT_VIEW_TITLE_LINE_R,	//!< 
	ACT_VIEW_MODE_LINE_L,	//!< 
	ACT_VIEW_MODE_LINE_C,	//!< 
	ACT_VIEW_MODE_LINE_R,	//!< 
	ACT_VIEW_MODE_LINE_L2,	//!< 
	ACT_VIEW_MODE_LINE_C2,	//!< 
	ACT_VIEW_MODE_LINE_R2,	//!< 
	ACT_TAB_BACK_RANK,	//!< ランク用ボタン背景テーブル

	// ランキングメニュー・言語共通
	ACT_MENU_TAB_WIN_1,		//!< 
	ACT_MENU_TAB_WIN_2,		//!< 
	ACT_MENU_TAB_WIN_3,		//!< 
	ACT_MENU_TAB_TONE1,		//!< 
	ACT_MENU_TAB_TONE2,		//!< 
	ACT_MENU_TAB_1,			//!< 
	ACT_MENU_TAB_2,			//!< 
	ACT_MENU_TAB_3,			//!< 
	ACT_MENU_TAB_STAGE,		//!< 
	ACT_MENU_ARROW,			//!< 
	ACT_MENU_ACT_NUM_1,		//!< 
	ACT_MENU_ACT_NUM_2,		//!< 
	ACT_MENU_ACT_NUM_3,		//!< 
	ACT_MENU_ACT_NUM_4,		//!< 
	ACT_MENU_ACT_NUM_5,		//!< 
	ACT_MENU_ACT_NUM_6,		//!< 
	ACT_MENU_ACT_NUM_7,		//!< 
	ACT_MENU_TEX_ZONE,		//!< 
	ACT_MENU_TAB_COVER1,	//!< 
	ACT_MENU_TAB_COVER2,	//!< 
	ACT_MENU_TAB_COVER3,	//!< 
	ACT_MENU_TAB_CRSR,		//!< 
	ACT_MENU_CRSR1,			//!< 
	ACT_MENU_CRSR2,			//!< 
	ACT_MENU_CRSR3,			//!< 

	// ランキングメイン・言語共通
	ACT_VIEW_ACT_NUM,		//!< 
//#if !_WII
	ACT_VIEW_CRSR_UP,		//!< 
	ACT_VIEW_CRSR_LEFT,		//!< 
	ACT_VIEW_CRSR_RIGHT,	//!< 
	ACT_VIEW_CRSR_DOWN,		//!< 
	ACT_VIEW_BTN_LEFT,		//!< 
	ACT_VIEW_BTN_RIGHT,		//!< 
//#endif
	ACT_VIEW_TAB_CRSR1_L,	//!< 
	ACT_VIEW_TAB_CRSR1_C,	//!< 
	ACT_VIEW_TAB_CRSR1_R,	//!< 
	ACT_VIEW_TAB_CRSR2,		//!< 
	ACT_VIEW_TAB_ALL,		//!< 
	ACT_VIEW_TAB_MYSCORE,	//!< 
//#if !_WII
	ACT_VIEW_TAB_FRIENDS,	//!< 
//#endif
	ACT_VIEW_TAB_INDEX,		//!< 
	ACT_VIEW_TAB_LIST,		//!< 
	ACT_VIEW_TAB_BOTTOM,	//!< 
//#if !_WII
	ACT_VIEW_LIGHT_SCORE,	//!< 
	ACT_VIEW_LIGHT_SONIC,	//!< 
	ACT_VIEW_ICON_SONIC1,	//!< 
	ACT_VIEW_ICON_SONIC2,	//!< 
	ACT_VIEW_ICON_SONIC_INFO1,	//!< 
	ACT_VIEW_ICON_SONIC_INFO2,	//!< 
//#endif
	

	// ランキングリスト・言語共通
	ACT_LIST_RANK_1,		//!< 
	ACT_LIST_RANK_2,		//!< 
	ACT_LIST_RANK_3,		//!< 
	ACT_LIST_RANK_4,		//!< 
	ACT_LIST_RANK_5,		//!< 
	ACT_LIST_RANK_6,		//!< 
	ACT_LIST_RANK_7,		//!< 
	ACT_LIST_NAME_1,		//!< 
	ACT_LIST_NAME_2,		//!< 
	ACT_LIST_NAME_3,		//!< 
	ACT_LIST_NAME_4,		//!< 
	ACT_LIST_NAME_5,		//!< 
	ACT_LIST_NAME_6,		//!< 
	ACT_LIST_NAME_7,		//!< 
	ACT_LIST_NAME_8,		//!< 
	ACT_LIST_NAME_9,		//!< 
	ACT_LIST_NAME_10,		//!< 
//#if !_WII
	ACT_LIST_NAME_11,		//!< 
	ACT_LIST_NAME_12,		//!< 
	ACT_LIST_NAME_13,		//!< 
	ACT_LIST_NAME_14,		//!< 
	ACT_LIST_NAME_15,		//!< 
	ACT_LIST_NAME_16,		//!< 
//#endif
	ACT_LIST_TIME_1,		//!< 
	ACT_LIST_TIME_CLN_1,	//!< 
	ACT_LIST_TIME_2,		//!< 
	ACT_LIST_TIME_3,		//!< 
	ACT_LIST_TIME_CLN_2,	//!< 
	ACT_LIST_TIME_4,		//!< 
	ACT_LIST_TIME_5,		//!< 
	ACT_LIST_SCORE_1,		//!< 
	ACT_LIST_SCORE_2,		//!< 
	ACT_LIST_SCORE_3,		//!< 
	ACT_LIST_SCORE_4,		//!< 
	ACT_LIST_SCORE_5,		//!< 
	ACT_LIST_SCORE_6,		//!< 
	ACT_LIST_SCORE_7,		//!< 
	ACT_LIST_SCORE_8,		//!< 
	ACT_LIST_SCORE_9,		//!< 
	ACT_LIST_ICON_SONIC3,	//!< 
	ACT_TEX_LIST_ZONE,		//!< 
	
	// モード共通・言語別
	ACT_TEX_TITLE,			//!< 
	ACT_TEX_OBI1,			//!< 
	ACT_TEX_OBI2,			//!< 
	
//#if !_WII
	ACT_TEX_TIMERANK,		//!< 
//#endif
	ACT_TEX_LIST_STAGE,		//!< 
	ACT_TEX_LIST_ZONE_S,	//!< 
	ACT_TEX_LIST_ACT,		//!< 
	ACT_TEX_LIST_BOSS,		//!< 

	// ランキングメニュー・言語別
	ACT_MENU_TEX_ZONE_S,	//!< 
	ACT_MENU_TEX_ACT,		//!< 
	ACT_MENU_TAB_TEX_BOSS,	//!< 
	ACT_MENU_TAB_TEX_F_BOSS,//!< 

	// ランキングメイン・言語別
	ACT_VIEW_TEX_ALL,		//!< 
	ACT_VIEW_TEX_MYSCORE,	//!< 
//#if !_WII
	ACT_VIEW_TEX_FRIENDS,	//!< 
//#endif
	ACT_VIEW_TEX_RANK,		//!< 
	ACT_VIEW_TEX_NAME,		//!< 
	ACT_VIEW_TEX_TIME,		//!< 
	ACT_VIEW_TEX_AREA,		//!< 
//#if !_WII
	ACT_VIEW_TEX_SONIC1,	//!< 
	ACT_VIEW_TEX_SONIC2,	//!< 
	ACT_VIEW_TEX_SONIC3,	//!< 
//#endif
	ACT_VIEW_TEX_BIG_TIME,	//!< 
	ACT_VIEW_TEX_BIG_SCORE,	//!< 
	ACT_VIEW_TEX_TIME_EFCT,	//!< 
	ACT_VIEW_TEX_SCORE_EFCT,//!< 

	// ランキングリスト・言語別
	ACT_LIST_TEX_COUNTRY,	//!< 

//#if !_WII
//	ACT_LIST_MSG_UPDATE,	//!< 
//	ACT_LIST_ICON_PROD,		//!< 
//#endif

	// ウインドウ関連・言語別
	ACT_WIN_TEX_MSG0,		//!< 接続中メッセージ
	ACT_WIN_TEX_MSG1,		//!< アップロード確認メッセージ
	ACT_WIN_TEX_MSG2,		//!< ランキング登録中
	ACT_WIN_TEX_MSG3,		//!< ランキング更新中
	
	// メニュー共通データ
	ACT_WAVE_BG,			//!< 
	ACT_DOWN_BG,			//!< 
	ACT_BLUE_BG,			//!< 
	
	ACT_BTN_CANCEL1,		//!< 
	ACT_BTN_CANCEL_WIN,		//!< 
	ACT_BTN_MODE,			//!< 
	ACT_BTN_SONIC,			//!< 
	
	ACT_OBI_C,				//!< 
	ACT_OBI_L,				//!< 
	ACT_OBI_R,				//!< 
	ACT_OBI_R2,				//!< 
	
	ACT_WIN_LINE,			//!< 
	
	ACT_TEX_BACK_FIX,		//!< 
	ACT_TEX_BACK_WIN,		//!< 
	ACT_TEX_WINTITLE,		//!< 
	ACT_TEX_YES,			//!< 
	ACT_TEX_NO,				//!< 
	
	ACT_NUM,

	
	
	ACT_NONE
} DME_RANK_ACT;

#endif


typedef struct tag_DMS_RANK_DISP_DATA_BUF {
	
	// ランキングデータ関連
	u32 disp_top_rank_no;
	u32 disp_rank_no[DMD_RANK_DISP_RANKING_NUM];		//!< ランキング表示用順位番号
	char *disp_name[DMD_RANK_DISP_RANKING_NUM];		//!< ランキング表示用の名前保存変数
	u32 disp_time[DMD_RANK_DISP_RANKING_NUM];			//!< ランキング表示用スコアタイム

	u32 disp_score[DMD_RANK_DISP_RANKING_NUM];			//!< ランキング表示用スコア
	
	s32 disp_area[DMD_RANK_DISP_RANKING_NUM];			//!< ランキング表示用地域データ
	s32 disp_sonic[DMD_RANK_DISP_RANKING_NUM];			//!< ランキング表示用ソニックアイコンデータ
	
	s32 disp_list_num;
	
	u32 my_rank_data;
	
} DMS_RANK_DISP_DATA_BUF;


typedef struct tag_DMS_RANK_TAB_TYPE_DATA_BUF {
	// タブ数分
	DMS_RANK_DISP_DATA_BUF data_buf[DME_RANK_SCORE_TYPE_NUM];
	
} DMS_RANK_TAB_TYPE_DATA_BUF;


typedef struct tag_DMS_RANK_SONIC_TYPE_DATA_BUF {
	// ソニックタイプ数分
	DMS_RANK_TAB_TYPE_DATA_BUF tab_data_buf[DME_RANK_SONIC_TYPE_NUM];
	
} DMS_RANK_SONIC_TYPE_DATA_BUF;


typedef struct tag_DMS_RANK_GAME_MODE_DATA_BUF {
	// ゲームモード数分
	DMS_RANK_SONIC_TYPE_DATA_BUF sonic_data_buf[DME_RANK_DISP_RANK_NUM];
	
} DMS_RANK_GAME_MODE_DATA_BUF;



typedef struct tag_DMS_RANK_MAIN_WORK	DMS_RANK_MAIN_WORK;

//! メインタスクワーク
struct tag_DMS_RANK_MAIN_WORK {
	
	AMS_FS			*arc_cmn_amb_fs[5];					//!< 共通アーカイブAMBファイル
	void			*arc_cmn_amb[5];					//!< 共通アーカイブAMBファイル
	void			*cmn_ama[5];						//!< AMAファイル
	void			*cmn_amb[5];						//!< AMBファイル
	AOS_TEXTURE		cmn_tex[5];							//!< メニュー共通テクスチャ
	
	AMS_FS			*arc_amb_fs[DME_RANK_DATA_TYPE_MAX];	//!< アーカイブAMBファイル
	void			*arc_amb[DME_RANK_DATA_TYPE_MAX];	//!< アーカイブAMBファイル
	void			*ama[DME_RANK_DATA_TYPE_MAX];		//!< AMAファイル
	void			*amb[DME_RANK_DATA_TYPE_MAX];		//!< AMBファイル
	
	AOS_TEXTURE		tex[DME_RANK_DATA_TYPE_MAX];		//!< テクスチャ
	
	// ウインドウ用アクション

	// メニュー用アクション
	AOS_ACTION 		*act[ACT_NUM];
	

	// プロシージャ設定変数
	void (*proc_win_input)(DMS_RANK_MAIN_WORK *);		//!< ウインドウ用入力処理関数
	void (*proc_win_update)(DMS_RANK_MAIN_WORK *);		//!< ウインドウ用プロシージャ
	void (*proc_input)(DMS_RANK_MAIN_WORK *);			//!< メイン入力処理関数
	void (*proc_menu_update)(DMS_RANK_MAIN_WORK *);		//!< メインプロシージャ
	void (*proc_draw)(DMS_RANK_MAIN_WORK *);			//!< 描画用プロシージャ

	// イベント関連
	s32 next_evt;										//!< 次のイベント
	s32 prev_evt;										//!< 前のイベント

	// フラグ関連
	u32	flag;											//!< 汎用フラグ
	u32 disp_flag;										//!< 表示切替用フラグ
	u32 announce_flag;									//!< 0ならアナウンスなし、それ以外はフラグがあるだけ表示する
	u32 efct_out_flag;									//!< 

	// 状態変数
	s32 state;											//!< メニューかランキング画面か
	
	// タイマー関連
	float timer;										//!< 汎用タイマー
	float win_timer;									//!< ウインドウ演出用タイマー
	float tab_efct_timer;
	int wait_timer;
	float efct_timer[DME_RANK_ZONE_TYPE_MAX];			//!< ZONEテーブル演出用タイマー

	// ランキングデータ関連
	u32 disp_top_rank_no;
	u32 disp_rank_no[DMD_RANK_DISP_RANKING_NUM];		//!< ランキング表示用順位番号
	char *disp_name[DMD_RANK_DISP_RANKING_NUM];		//!< ランキング表示用の名前保存変数
	u32 disp_time[DMD_RANK_DISP_RANKING_NUM];			//!< ランキング表示用スコアタイム

	u32 disp_score[DMD_RANK_DISP_RANKING_NUM];			//!< ランキング表示用スコア
	
	s32 disp_area[DMD_RANK_DISP_RANKING_NUM];			//!< ランキング表示用地域データ
	s32 disp_sonic[DMD_RANK_DISP_RANKING_NUM];			//!< ランキング表示用ソニックアイコンデータ
	
	s32 disp_list_num;

	// FOCUS選択変数(ランキングメイン用)
	u32 cur_game_mode;									//!< スコアランキングかタイムランキングか
	u32 cur_slct_page;									//!< 現在表示しているランキングページ
	u32 cur_slct_tab;									//!< 選択中のタブページ(ALL,MYSCORE,FRIENDS)
	u32 prev_slct_tab;									//!< 選択中のタブページ(ALL,MYSCORE,FRIENDS)
	u32 cur_slct_data;									//!< 現在選択しているランキング順位
	u32 prev_slct_data;									//!< 一つ前に選択しているランキング順位
	u32 cur_page_top_rank;								//!< 表示しているページの一番上にある順位
	u32 prev_page_top_rank;								//!< 一つ前に表示していたページの一番上にある順位
	u32 my_rank_data;									//!< 自分のランキング順位
	s32 cur_sonic_type;									//!< 
	float sonic_icon_pos[2][2];							//!< 
	u32 cur_view_stage;									//!<
	
	BOOL is_data_upload;
	BOOL is_snd_exit;
	
	// FOCUS選択変数(ランキングメニュー用)
	u32 cur_zone;
	u32 prev_zone;
	u32 cur_stage;
	u32 prev_stage;
	
	// WINDOW専用(Wiiのみ？)
	float win_act_pos[14-1][2];							//!< 
	float win_size_rate[2];								//!< 
	s32 win_mode;										//!< 
	s32 win_cur_slct;									//!< ウインドウでの現在の選択項目
	
	// ZONEテーブル表示関連
	float zone_pos[DME_RANK_ZONE_TYPE_MAX][2];			//!< ZONEの現在の表示位置
	float move_spd[2];									//!< 演出時の移動速度
	// 演出用変数
	float act_tab_scale[DME_RANK_ZONE_TYPE_MAX][2];		//!< ACT表示部分のテーブルSCALE値(X軸Y軸)
	float zone_tab_scale[DME_RANK_ZONE_TYPE_MAX][2];	//!< ZONE表示部分のテーブルSCALE値(X軸のみ)
	
	float zone_inout_efct_pos[2];						//!< メニュー側出入り演出用オフセット座標
	float rank_inout_efct_pos[2];						//!< ランキング側出入り演出用オフセット座標
	float src_zone_inout_efct_pos[2];					//!< メニュー側出入り演出用オフセット座標
	float src_rank_inout_efct_pos[2];					//!< ランキング側出入り演出用オフセット座標
	float dst_zone_inout_efct_pos[2];					//!< メニュー側出入り演出用オフセット座標
	float dst_rank_inout_efct_pos[2];					//!< ランキング側出入り演出用オフセット座標
	
	float rank_win_main_move_pos;
	float rank_win_sub_move_pos;
	float src_rank_main_chng_pos[2];
	float dst_rank_main_chng_pos[2];
	
	// ACT専用
	float act_tab_pos[DME_RANK_ZONE_TYPE_MAX][2];
	float zone_move_src[DME_RANK_ZONE_TYPE_MAX][2];
	float zone_move_dest[DME_RANK_ZONE_TYPE_MAX][2];
	float zone_move_pos_src[DME_RANK_ZONE_TYPE_MAX][2];
	float zone_move_pos_dst[DME_RANK_ZONE_TYPE_MAX][2];
	float zone_scale_src[DME_RANK_ZONE_TYPE_MAX][2];
	float zone_scale_dst[DME_RANK_ZONE_TYPE_MAX][2];

	int mode_tex_efct_frm;								//!< 
	int index_act_anime_frm[2];							//!< 
	int sonic_icon_efct_frm;							//!< 
	int btn_l_disp_frm;									//!< 
	int btn_r_disp_frm;									//!< 
	
	float menu_crsr_pos[2];								//!< ランキングメニューのカーソル表示位置
	float rank_crsr_pos[2];								//!< ランキングメインのカーソル表示位置
	float rank_myscore_pos;							//!< ランキングメインのカーソル表示位置
	
	// 帯用
	float obi_pos[2];									//!< 帯の移動用座標変数
	float obi_pos_y;
	
	u32 draw_state;

	BOOL is_jp_region;
	BOOL is_rank_view_only;
	BOOL is_upload_cancel;
	BOOL is_download_cancel;
	
	// メインゲーム中用のサウンドSCBファイルポインタ
	GSS_SND_SCB *bgm_scb;
	
	AMS_PARAM_DRAW_PRIMITIVE up_bg_vrtx;
	
	float tex_u[2];
	float tex_v[2];
	
	GSS_SND_SE_HANDLE *se_handle;
	
	DMS_RANK_GAME_MODE_DATA_BUF tmp_strg_data;
};


//! 管理構造体
typedef struct tag_DMS_RANK_MGR {
	MTS_TASK_TCB *tcb;	//!< TCB
} DMS_RANK_MGR;



// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static void dmRankInit(void);
static void dmRankProcMain(MTS_TASK_TCB *tcb);
static void dmRankDest(MTS_TASK_TCB *tcb);

// 初期化設定関連
static void dmRankSetInitActPos(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetClearInfo(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetNextEvt(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetTypeRankingMode(DMS_RANK_MAIN_WORK *main_work);

static void dmRankLoadFontData(DMS_RANK_MAIN_WORK *main_work);
static void dmRankIsLoadFontData(DMS_RANK_MAIN_WORK *main_work);
static void dmRankLoadRequest(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcLoadWait(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcTexBuildWait(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcCheckLoadingEnd(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcCreateAct(DMS_RANK_MAIN_WORK *main_work);

// メニュー用プロシージャ
static void dmRankProcFadeIn(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcCheckConnectNetwork(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcRankViewDataDownload(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcCheckEndErrorWin(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcCheckDisconnectNetwork(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcFadeOut(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcStopDraw(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcDataRelease(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcFinish(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcWaitFinished(DMS_RANK_MAIN_WORK *main_work);

static void dmRankProcRankMenuInEfct(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcRankMenuIdle(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcRankMenuOutEfct(DMS_RANK_MAIN_WORK *main_work);

static void dmRankProcRankSetDataUpload(DMS_RANK_MAIN_WORK *main_work);

#if _WII
static void dmRankProcRankIsMyDataUpload(DMS_RANK_MAIN_WORK *main_work);
#endif

static void dmRankProcRankMyDataUpload(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcRankSuccessUpload(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcRankViewDataDownload(DMS_RANK_MAIN_WORK *main_work);


static void dmRankProcRankViewInEfct(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcRankViewIdle(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcRankViewOutEfct(DMS_RANK_MAIN_WORK *main_work);

// ウインドウ用プロシージャ
static void dmRankProcWindowNodispIdle(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcWindowOpenEfct(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcWindowAnnounceIdle(DMS_RANK_MAIN_WORK *main_work);
static void dmRankProcWindowCloseEfct(DMS_RANK_MAIN_WORK *main_work);

// 入力処理用プロシージャ
static void dmRankInputProcRankMenu(DMS_RANK_MAIN_WORK *main_work);
static void dmRankInputProcRankView(DMS_RANK_MAIN_WORK *main_work);

static void dmRankInputProcWinDoYouUpload(DMS_RANK_MAIN_WORK *main_work);
#if _WII || _PS3 || _XBOX
static void dmRankInputProcWinUploadCancel(DMS_RANK_MAIN_WORK *main_work);
#endif

// 描画関連処理
static void dmRankProcActDraw(DMS_RANK_MAIN_WORK *main_work);
static void dmRankCommonDraw(DMS_RANK_MAIN_WORK *main_work);
static void dmRankCommonFixDraw(DMS_RANK_MAIN_WORK *main_work);
static void dmRankRankMenuDraw(DMS_RANK_MAIN_WORK *main_work);
static void dmRankRankViewDraw(DMS_RANK_MAIN_WORK *main_work);
static void dmRankRankSubViewDraw(DMS_RANK_MAIN_WORK *main_work);
static void dmRankRankListDraw(DMS_RANK_MAIN_WORK *main_work);

static void dmRankCmnViewDraw(DMS_RANK_MAIN_WORK *main_work);

static void dmRankTaskDraw(AMS_TCB* tcb);

static void dmRankWinSelectDraw(DMS_RANK_MAIN_WORK *main_work);

static void dmRankHideActTableDraw(DMS_RANK_MAIN_WORK *main_work);
static void dmRankDrawHideActBg(AMS_TCB *tcb_p);

// 演出関連設定処理
static void dmRankSetRankMenuOutEfct(DMS_RANK_MAIN_WORK *main_work);
static BOOL dmRankIsRankMenuOutEfct(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetRankMenuInEfct(DMS_RANK_MAIN_WORK *main_work);
static BOOL dmRankIsRankMenuInEfct(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetRankViewInEfct(DMS_RANK_MAIN_WORK *main_work);
static BOOL dmRankIsRankViewInEfct(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetRankViewOutEfct(DMS_RANK_MAIN_WORK *main_work);
static BOOL dmRankIsRankViewOutEfct(DMS_RANK_MAIN_WORK *main_work);

static void dmRankSetRankMenuChangeFocus(DMS_RANK_MAIN_WORK *main_work);
static BOOL dmRankIsRankMenuChangeFocus(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetRankViewCrsrChangeEfct(DMS_RANK_MAIN_WORK *main_work);
static BOOL dmRankIsRankViewCrsrChangeEfct(DMS_RANK_MAIN_WORK *main_work);

static void dmRankSetObiOutEfct(DMS_RANK_MAIN_WORK *main_work);
static BOOL dmRankIsObiOutEfctEnd(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetObiInEfct(DMS_RANK_MAIN_WORK *main_work);
static BOOL dmRankIsObiInEfctEnd(DMS_RANK_MAIN_WORK *main_work);

static void dmRankSetWinOpenEfct(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetWinCloseEfct(DMS_RANK_MAIN_WORK *main_work);

static void dmRankSetMenuChngFocusStage(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetMenuChngFocusZone(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetMenuDispZoneTablePos(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetViewChngFocusRank(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetViewChngFocusTab(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetViewChngFocusStage(DMS_RANK_MAIN_WORK *main_work);

static void dmRankSetViewChngZoneEfctInit(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetZoneChangeEfct(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetZoneChangeEfctPos(DMS_RANK_MAIN_WORK *main_work);
static BOOL dmRankIsZoneChangeEfct(DMS_RANK_MAIN_WORK *main_work);

static void dmRankSetObiEfctPos(DMS_RANK_MAIN_WORK *main_work);

static void dmRankSetDownloadRankData(DMS_RANK_MAIN_WORK *main_work);

static void dmRankActAnimeControl(DMS_RANK_MAIN_WORK *main_work);

static s32 dmRankIsDataLoad(DMS_RANK_MAIN_WORK *main_work);
static s32 dmRankIsTexLoad(DMS_RANK_MAIN_WORK *main_work);
static s32 dmRankIsTexRelease(DMS_RANK_MAIN_WORK *main_work);

static u32 dmRankGetRevisedMenuZoneNo(s32 idx, s32 diff);
static u32 dmRankGetRevisedMenuStageNo(s32 idx, s32 diff, s32 zone_no);
static u32 dmRankGetRevisedRankCrsrFocus(s32 idx, s32 diff);
static u32 dmRankGetRevisedRankTabFocus(s32 idx, s32 diff);

static void dmRankSetRankingEndParam(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetRankViewEndParam(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetStageNoForViewOnly(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetDataUploadCheck(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetDispRankInitData(DMS_RANK_MAIN_WORK *main_work);

static void dmRankInitAllocRankTmpStrg(DMS_RANK_MAIN_WORK *main_work);
static void dmRankReleaseFreeRankTmpStrg(DMS_RANK_MAIN_WORK *main_work);
static DMS_RANK_DISP_DATA_BUF *dmRankGetDispRankTmpStrg(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetSaveDispRankTmpStrg(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetDispRankTmpStrg(DMS_RANK_MAIN_WORK *main_work);
static void dmRankSetClearDispRankTmpStrg(DMS_RANK_MAIN_WORK *main_work);

static BOOL dmRankIsSonicOnlyStage(DMS_RANK_MAIN_WORK *main_work);
static BOOL dmRankIsSaveRunData(DMS_RANK_MAIN_WORK *main_work);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ----- Static Variables --------------------（スタティック変数の定義：局所）
// 各国別AMBファイルパステーブル
const static char *dm_rank_lng_amb_name_tbl[GSD_LANGUAGE_NUM] = {
	GSS_BASE_PATH"DEMO/RANKING/D_RANK_JP.AMB",
	GSS_BASE_PATH"DEMO/RANKING/D_RANK_US.AMB",
	GSS_BASE_PATH"DEMO/RANKING/D_RANK_FR.AMB",
	GSS_BASE_PATH"DEMO/RANKING/D_RANK_IT.AMB",
	GSS_BASE_PATH"DEMO/RANKING/D_RANK_GE.AMB",
	GSS_BASE_PATH"DEMO/RANKING/D_RANK_SP.AMB",
};


// メニュー共通AMBファイルパステーブル
const static char *dm_rank_menu_cmn_amb_name_tbl[4] = {
	GSS_BASE_PATH"DEMO/CMN/D_CMN_BG.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_BTN.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_OBI.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_WIN.AMB",
};

// 各国別メニュー共通AMBファイルパステーブル
const static char *dm_rank_menu_cmn_lng_amb_name_tbl[GSD_LANGUAGE_NUM] = {
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_JP.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_US.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_FR.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_IT.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_GE.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_SP.AMB",
};


// ZONE毎のZONEテーブル表示フレーム
const static float dm_rank_zone_tab_disp_frm_tbl[DME_RANK_ZONE_TYPE_MAX] = {
	0.f,
	0.f,
	0.f,
	0.f,
	1.f,
	2.f,
};


// ZONE毎のACTテキスト表示フレーム
const static float dm_rank_act_tex_disp_frm_tbl[DME_RANK_ZONE_TYPE_MAX] = {
	0.f,
	0.f,
	0.f,
	0.f,
	0.f,
	1.f,
};


// ZONEごとのACT数テーブル
const static u32 dm_rank_zone_act_num_tbl[6][2] = {
	{0,  4},
	{4,  4},
	{8,  4},
	{12, 4},
	{16, 1},
	{17, 7},
};



// ZONEごとの表示移動値テーブル
const static float dm_rank_zone_pos_offset_tbl[6][2] = {
	{160.f, 4},
	{160.f, 4},
	{160.f, 4},
	{160.f, 4},
	{160.f, 1},
	{160.f, 7},
};



// ZONEごとのACT縦切り替えテーブル
const static u32 dm_rank_act_chng_vrtcl_num_tbl[6] = {
	0,
	0,
	0,
	0,
	0,
	4,
};


// STAGEごとのZONE番号テーブル
const static u32 dm_rank_act_zone_no_tbl[24] = {
	0, 0, 0, 0,
	1, 1, 1, 1,
	2, 2, 2, 2,
	3, 3, 3, 3,
	4,
	5, 5, 5, 5, 5, 5, 5,
};


// 各STAGEのカーソル表示位置X座標テーブル
const static s32 dm_rank_menu_crsr_pos_x_tbl[6][7] = {
	{  0, 50, 100, 176,  -1,  -1,  -1},
	{  0, 50, 100, 176,  -1,  -1,  -1},
	{  0, 50, 100, 176,  -1,  -1,  -1},
	{  0, 50, 100, 176,  -1,  -1,  -1},
	{ 26, -1,  -1,  -1,  -1,  -1,  -1},
	{  0, 50, 100, 150, 200, 250, 300},
};


// 各STAGEのカーソル表示拡大率(X軸)テーブル
const static f32 dm_rank_menu_crsr_scale_x_tbl[6][7] = {
	{1.0f, 	1.0f,  1.0f,  2.0f, -1.0f, -1.0f, -1.0f},
	{1.0f, 	1.0f,  1.0f,  2.0f, -1.0f, -1.0f, -1.0f},
	{1.0f, 	1.0f,  1.0f,  2.0f, -1.0f, -1.0f, -1.0f},
	{1.0f, 	1.0f,  1.0f,  2.0f, -1.0f, -1.0f, -1.0f},
	{2.3f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f},
	{1.0f, 	1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f},
};


const static float dm_rank_back_text_length_tbl[6] = {
	-39.f,
	-53.f,
	-69.f,
	-77.f,
	-70.f,
	-55.f,
};

#if !_WII
const static float dm_rank_win_act_pos_tbl[11][2] = {
//	{0.0f, 0.0f},		// ウインドウ背景
	{DMD_RANK_SIZE_HALF_WIDTH + 42.f, 280.0f},	// ウインドウ内のライン
	{DMD_RANK_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
	{DMD_RANK_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
	{DMD_RANK_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
	{DMD_RANK_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
	{DMD_RANK_SIZE_HALF_WIDTH - 80.f, 274.0f},	// タイトルテキスト
	{DMD_RANK_SIZE_HALF_WIDTH - 88.f, 420.0f},	// YES
	{DMD_RANK_SIZE_HALF_WIDTH + 88.f, 420.0f},	// NO
	{DMD_RANK_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
	{DMD_RANK_SIZE_HALF_WIDTH + 182.f, 264.0f},	// キャンセルボタン
	{DMD_RANK_SIZE_HALF_WIDTH + 202.f, 264.0f},	// 戻る
//	{DMD_RANK_SIZE_HALF_WIDTH, 420.0f},			// OK
};
#else	// #if _WII
const static float dm_rank_win_act_pos_tbl[11][2] = {
//	{0.0f, 0.0f},		// ウインドウ背景
	{DMD_RANK_SIZE_HALF_WIDTH + 42.f, 280.0f},	// ウインドウ内のライン
	{DMD_RANK_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
	{DMD_RANK_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
	{DMD_RANK_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
	{DMD_RANK_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
	{DMD_RANK_SIZE_HALF_WIDTH - 80.f, 274.0f},	// タイトルテキスト
	{DMD_RANK_SIZE_HALF_WIDTH - 88.f, 436.0f},	// YES
	{DMD_RANK_SIZE_HALF_WIDTH + 88.f, 436.0f},	// NO
	{DMD_RANK_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
	{DMD_RANK_SIZE_HALF_WIDTH + 258.f, 232.0f},	// キャンセルボタン
	{DMD_RANK_SIZE_HALF_WIDTH + 278.f, 232.0f},	// 戻る
//	{DMD_RANK_SIZE_HALF_WIDTH, 420.0f},			// OK
};
#endif


// アクションIDテーブル(初期状態)
#if 1
const static u32 g_dm_act_id_tbl[ACT_NUM] = {
	// モード共通・言語共通
	IDA_D_RANK_ACT_TAB_TITLE1,		//!< 
	IDA_D_RANK_ACT_TAB_ZONE_L,
	IDA_D_RANK_ACT_TAB_ZONE_R,
	IDA_D_RANK_ACT_TAB_TIME_L,
	IDA_D_RANK_ACT_TAB_TIME_C,
	IDA_D_RANK_ACT_TAB_TIME_R, 
	IDA_D_RANK_ACT_TAB_TIME_L2,
	IDA_D_RANK_ACT_TAB_TIME_C2,
	IDA_D_RANK_ACT_TAB_TIME_R2, 
	IDA_D_RANK_ACT_TAB_BACK_LIST,	//!<

	// ランキングメニュー・言語共通
	IDA_D_RANK_ACT_TAB07_LEFT,		//!< 
	IDA_D_RANK_ACT_TAB07_CENTER,	//!< 
	IDA_D_RANK_ACT_TAB07_RIGHT,		//!< 
	IDA_D_RANK_ACT_TAB08_LEFT02,	//!< 
	IDA_D_RANK_ACT_TAB08_RIGHT02,	//!< 
	IDA_D_RANK_ACT_TAB_A_LEFT,		//!< 
	IDA_D_RANK_ACT_TAB_A_CENTER,	//!< 
	IDA_D_RANK_ACT_TAB_A_RIGHT,		//!< 
	IDA_D_RANK_ACT_TAB06,			//!< 
	IDA_D_RANK_ACT_ARROW,			//!< 
	IDA_D_RANK_ACT_TEX_ACT1,		//!< 
	IDA_D_RANK_ACT_TEX_ACT2,		//!< 
	IDA_D_RANK_ACT_TEX_ACT3,		//!< 
	IDA_D_RANK_ACT_TEX_ACT4,		//!< 
	IDA_D_RANK_ACT_TEX_ACT5,		//!< 
	IDA_D_RANK_ACT_TEX_ACT6,		//!< 
	IDA_D_RANK_ACT_TEX_ACT7,		//!< 
	IDA_D_RANK_ACT_TEX_ZONE,		//!< 
	IDA_D_RANK_ACT_TAB_NONACT_LEFT,	//!< 
	IDA_D_RANK_ACT_TAB_NONACT_CENTER,	//!< 
	IDA_D_RANK_ACT_TAB_NONACT_RIGHT,	//!< 
	IDA_D_RANK_ACT_MENU_CURSOL,			//!< 
	IDA_D_RANK_ACT_TAB_ACT_LEFT,		//!< 
	IDA_D_RANK_ACT_TAB_ACT_CENTER,		//!< 
	IDA_D_RANK_ACT_TAB_ACT_RIGHT,		//!<

	// ランキングメイン・言語共通
	IDA_D_RANK_ACT_NUM_ACT,			//!< 
//#if !_WII
	IDA_D_RANK_ACT_CUR01_UE,		//!< 	◆
	IDA_D_RANK_ACT_CUR01_LEFT,		//!< 	◆
	IDA_D_RANK_ACT_CUR01_RIGHT,		//!< 	◆
	IDA_D_RANK_ACT_CUR01_SHITA,		//!< 	◆
	IDA_D_RANK_ACT_BUT04_LEFT,		//!< 	◆
	IDA_D_RANK_ACT_BUT04_RIGHT,		//!< 	◆
//#endif
	IDA_D_RANK_ACT_CUR02_L,			//!< 
	IDA_D_RANK_ACT_CUR02_C,			//!< 
	IDA_D_RANK_ACT_CUR02_R,			//!< 
	IDA_D_RANK_ACT_CUR03,			//!< 
	IDA_D_RANK_ACT_INDEX01_A,		//!< 
	IDA_D_RANK_ACT_INDEX01_B,		//!< 
//#if !_WII
	IDA_D_RANK_ACT_INDEX01_C,		//!< 
//#endif
	IDA_D_RANK_ACT_INDEX02,			//!< 
	IDA_D_RANK_ACT_TAB_LIST,		//!< 
	IDA_D_RANK_ACT_TAB_BOTTOM,		//!< 
//#if !_WII
	IDA_D_RANK_ACT_LIGHT_SCORE,		//!< 
	IDA_D_RANK_ACT_LIGHT_SONIC,		//!< 
	IDA_D_RANK_ACT_ICON_SONIC01,	//!< 
	IDA_D_RANK_ACT_ICON_SONIC01,	//!< 
	IDA_D_RANK_ACT_ICON_SONIC03,	//!< 
	IDA_D_RANK_ACT_ICON_SONIC04,	//!< 
//#endif
	

	// ランキングリスト・言語共通
	IDA_D_RANK_ACT_JUN01,			//!< 
	IDA_D_RANK_ACT_JUN02,			//!< 
	IDA_D_RANK_ACT_JUN03,			//!< 
	IDA_D_RANK_ACT_JUN04,			//!< 
	IDA_D_RANK_ACT_JUN05,			//!< 
	IDA_D_RANK_ACT_JUN06,			//!< 
	IDA_D_RANK_ACT_JUN07,			//!< 
	IDA_D_RANK_ACT_TEX_ALPHA01,		//!< 
	IDA_D_RANK_ACT_TEX_ALPHA02,		//!< 
	IDA_D_RANK_ACT_TEX_ALPHA03,		//!< 
	IDA_D_RANK_ACT_TEX_ALPHA04,		//!< 
	IDA_D_RANK_ACT_TEX_ALPHA05,		//!< 
	IDA_D_RANK_ACT_TEX_ALPHA06,		//!< 
	IDA_D_RANK_ACT_TEX_ALPHA07,		//!< 
	IDA_D_RANK_ACT_TEX_ALPHA08,		//!< 
	IDA_D_RANK_ACT_TEX_ALPHA09,		//!< 
	IDA_D_RANK_ACT_TEX_ALPHA010,	//!< 
//#if !_WII
	IDA_D_RANK_ACT_TEX_ALPHA011,	//!< 
	IDA_D_RANK_ACT_TEX_ALPHA012,	//!< 
	IDA_D_RANK_ACT_TEX_ALPHA013,	//!< 
	IDA_D_RANK_ACT_TEX_ALPHA014,	//!< 
	IDA_D_RANK_ACT_TEX_ALPHA015,	//!< 
	IDA_D_RANK_ACT_TEX_ALPHA016,	//!< 
//#endif
	IDA_D_RANK_ACT_TIME01,			//!< 
	IDA_D_RANK_ACT_TIME_COLON1,		//!< 
	IDA_D_RANK_ACT_TIME02,			//!< 
	IDA_D_RANK_ACT_TIME03,			//!< 
	IDA_D_RANK_ACT_TIME_COLON2,		//!< 
	IDA_D_RANK_ACT_TIME04,			//!< 
	IDA_D_RANK_ACT_TIME05,			//!< 
	IDA_D_RANK_ACT_SCORE01,			//!< 
	IDA_D_RANK_ACT_SCORE02,			//!< 
	IDA_D_RANK_ACT_SCORE03,			//!< 
	IDA_D_RANK_ACT_SCORE04,			//!< 
	IDA_D_RANK_ACT_SCORE05,			//!< 
	IDA_D_RANK_ACT_SCORE06,			//!< 
	IDA_D_RANK_ACT_SCORE07,			//!< 
	IDA_D_RANK_ACT_SCORE08,			//!< 
	IDA_D_RANK_ACT_SCORE09,			//!< 
	IDA_D_RANK_ACT_ICON_SONIC02,	//!< 
	IDA_D_RANK_ACT_TEXT_LIST_ZONE,	//!< 

	// モード共通・言語別
	IDA_D_RANK_JP_ACT_TEX_TITLE,		//!< 
	IDA_D_RANK_JP_ACT_TEX_OBI1,			//!< 
	IDA_D_RANK_JP_ACT_TEX_OBI1,			//!< 

//#if !_WII
	IDA_D_RANK_JP_ACT_TEX_TIMERANK,		//!< 	◆
//#endif
	IDA_D_RANK_JP_ACT_TEX_LIST_STAGE,		//!< 
	IDA_D_RANK_JP_ACT_TEXT_LIST_ZONE,		//!< 
	IDA_D_RANK_JP_ACT_TEX_LIST_ACT,		//!< 
	IDA_D_RANK_JP_ACT_TEX_LIST_BOSS,		//!< 

	// ランキングメニュー・言語別
	IDA_D_RANK_JP_ACT_TEX_ZONE,		//!< 
	IDA_D_RANK_JP_ACT_TEX_ACT,		//!< 
	IDA_D_RANK_JP_ACT_TEX_BOSS,		//!< 
	IDA_D_RANK_JP_ACT_TEX_F_BOSS,	//!< 

	// ランキングメイン・言語別
	IDA_D_RANK_JP_ACT_TEX_ALL,
	IDA_D_RANK_JP_ACT_TEX_MYSCORE,
//#if !_WII
	IDA_D_RANK_JP_ACT_TEX_FRIENDS,	//!< ◆
//#endif
	IDA_D_RANK_JP_ACT_TEX_JUNI,
	IDA_D_RANK_JP_ACT_TEX_NAMAE,
	IDA_D_RANK_JP_ACT_TEX_TIME,
	IDA_D_RANK_JP_ACT_TEX_CHIIKI,
//#if !_WII
	IDA_D_RANK_JP_ACT_TEX_SONIC1,	//!< 
	IDA_D_RANK_JP_ACT_TEX_SONIC2,	//!< 
	IDA_D_RANK_JP_ACT_TEX_SONIC3,	//!< 
//#endif
	IDA_D_RANK_JP_ACT_TEX_SCTM01A,	//!< 
	IDA_D_RANK_JP_ACT_TEX_SCTM02A,	//!< 
	IDA_D_RANK_JP_ACT_TEX_SCTM01B,	//!< 
	IDA_D_RANK_JP_ACT_TEX_SCTM02B,	//!< 

	// ランキングリスト・言語別
	IDA_D_RANK_JP_ACT_TEX_AREA,

//#if !_WII
//	IDA_D_RANK_JP_ACT_TEX_KOSHIN,	//!< ◆
//	IDA_D_RANK_JP_ACT_ICON_PERIOD,	//!< ◆
//#endif

	// ウインドウ関連・言語別
	IDA_D_RANK_JP_ACT_TEX_WIN00,		//!< 接続中メッセージ
	IDA_D_RANK_JP_ACT_TEX_WIN01,		//!< アップロード確認メッセージ
	IDA_D_RANK_JP_ACT_TEX_WIN02,		//!< ランキング登録中
	IDA_D_RANK_JP_ACT_TEX_WIN03,		//!< ランキング更新中
	
	// メニュー共通データ
	IDA_D_CMN_BG_ACT_BG_WAVE,				//!< 
	IDA_D_CMN_BG_ACT_BG_DOWN_WHITE,			//!< 
	IDA_D_CMN_BG_ACT_BG_BLUE,				//!< 
	
	IDA_D_CMN_BTN_ACT_BTN_BACK_RANK,		//!< 
	IDA_D_CMN_BTN_ACT_BACK_BTN,				//!< 
	IDA_D_CMN_BTN_ACT_BTN_Y_RANK,			//!< 
	IDA_D_CMN_BTN_ACT_BTN_X_RANK,			//!< 
	
	IDA_D_CMN_OBI_ACT_OBI_CENTER,			//!< 
	IDA_D_CMN_OBI_ACT_OBI_LEFT,				//!< 
	IDA_D_CMN_OBI_ACT_OBI_RIGHT2_R,			//!< 
	IDA_D_CMN_OBI_ACT_OBI_RIGHT2_L,			//!< 
	
	IDA_D_CMN_WIN_ACT_WIN_LINE,				//!< 
	
	IDA_D_CMN_MSG_JP_ACT_TEX_BACK_RANK,		//!< 
	IDA_D_CMN_MSG_JP_ACT_TEX_BACK,			//!< 
	IDA_D_CMN_MSG_JP_ACT_TEX_WINTITLE,		//!< 
	IDA_D_CMN_MSG_JP_ACT_TEX_YES,			//!< 
	IDA_D_CMN_MSG_JP_ACT_TEX_NO,			//!< 
	
};
#endif


//管理情報
static DMS_RANK_MGR dm_rank_mgr;
static DMS_RANK_MGR *dm_rank_mgr_p = NULL;

static BOOL dm_rank_is_pause_maingame = FALSE;
static u32 dm_rank_draw_state = 0;


// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// DmRankingStart
/*!
	ランキング画面開始処理
 */
// ==========================================================================
void DmRankingStart(void *arg)
{
	s16 tmp_cur_evt = 0;
	
	UNREFERENCED_PARAMETER(arg);
	
	// 管理情報初期化設定
	amZeroMemory(&dm_rank_mgr, sizeof(DMS_RANK_MGR));
	dm_rank_mgr_p = &dm_rank_mgr;
	
	// 現在ロードしているイベントがメインゲームかどうかの設定
	tmp_cur_evt = SyGetEvtInfo()->cur_evt_id;
	
	if (tmp_cur_evt == GSD_EVT_ID_MAINGAME
		|| tmp_cur_evt == GSD_EVT_ID_SPSTAGE_BRANCH) {
		// ポーズメニューからの遷移フラグON
		dm_rank_is_pause_maingame = TRUE;
		
		mtTaskStartPause(GMD_TASK_GAME_PAUSE_LEVEL);
	}
	else {
		// フラグクリア
		dm_rank_is_pause_maingame = FALSE;
	}
	
	dmRankInit();
}



// ==========================================================================
// DmRankingIsExit
/*!
	ランキング画面終了チェック処理
 */
// ==========================================================================
BOOL DmRankingIsExit(void)
{
	if (dm_rank_mgr_p != NULL) {
		if (dm_rank_mgr_p->tcb == NULL) {
			return TRUE;
		}
	}
	else {
		return TRUE;
	}

	return FALSE;
}


// ----- Static Functions --------------------（スタティック関数の定義：局所）
// ==========================================================================
// dmRankInit
/*!
	ランキング画面初期化処理
 */
// ==========================================================================
void dmRankInit(void)
{
	DMS_RANK_MAIN_WORK	*main_work;

	// メインタスク作成
	dm_rank_mgr_p->tcb = MTM_TASK_MAKE_TCB(dmRankProcMain
											, dmRankDest
											, 0
											, DMD_RANK_TASK_PAUSELEVEL
											, DMD_RANK_TASK_PRIO_MAIN
											, DMD_RANK_TASK_GROUP_MAIN
											, sizeof(DMS_RANK_MAIN_WORK)
											, "RANK_MAIN"
											);
	
	// ワーク初期化
	main_work = (DMS_RANK_MAIN_WORK *)mtTaskGetTcbWork(dm_rank_mgr_p->tcb);
	
	// 初期化処理があればここに記述
//	main_work->state = DME_RANK_MODE_STATE_ACT_SLCT;

	// アクションシステム描画設定
	if (dm_rank_is_pause_maingame) {
		main_work->draw_state = (u32)AoActSysGetDrawStateEnable();
		
		if (main_work->draw_state) {
			dm_rank_draw_state = AoActSysGetDrawState();
		}
	}
	else {
		main_work->draw_state = FALSE;
		dm_rank_draw_state = 0;
		AoActSysSetDrawTaskPrio();
	}
	
	
	AoActSysSetDrawStateEnable((int)main_work->draw_state);
	
	if (main_work->draw_state) {
		dm_rank_draw_state = AoActSysGetDrawState();
	}
	else {
		AoActSysSetDrawTaskPrio();
	}
	
	dmRankSetTypeRankingMode(main_work);

	// 各アクションの初期表示位置設定
	dmRankSetInitActPos(main_work);

	// 各ステージのクリア情報設定(ここからどのステージが選択できるかを判別)
	dmRankSetClearInfo(main_work);
	
	main_work->cur_game_mode = DME_RANK_DISP_RANK_TIME;
	
//	DmLoadingStart();

	// プロシージャ設定
	main_work->proc_menu_update = dmRankLoadFontData;
}



// ==========================================================================
// dmRankSetTypeRankingMode
/*!
	ランキング表示タイプ設定処理
  	(ランキングビューだけか、ランキングメニューからなのかを設定)
 */
// ==========================================================================
void dmRankSetTypeRankingMode(DMS_RANK_MAIN_WORK *main_work)
{
	s16 tmp_prev_evt = 0;
	
	// 一つ前のイベントがタイトルまたはメインメニューかどうかの設定
	tmp_prev_evt = SyGetEvtInfo()->old_evt_id;
	
	if (tmp_prev_evt == GSD_EVT_ID_TITLE
		|| tmp_prev_evt == GSD_EVT_ID_MAINMENU) {
		// ポーズメニューからの遷移フラグON
		main_work->is_rank_view_only = FALSE;
	}
	else {
		// フラグクリア
		main_work->is_rank_view_only = TRUE;
	}
	
#if defined(MTD_DEBUG)
	if (tmp_prev_evt == GSD_EVT_ID_DEBUG_DEMO) {
		// ポーズメニューからの遷移フラグON
		main_work->is_rank_view_only = FALSE;
	}
#endif
}



// ==========================================================================
// dmRankSetInitActPos
/*!
	各アクションの初期表示位置設定処理
 */
// ==========================================================================
void dmRankSetInitActPos(DMS_RANK_MAIN_WORK *main_work)
{
//	UNREFERENCED_PARAMETER(main_work);
	
	// リージョンデータ取得(ボタン表示切り替え用)
	if (GeEnvGetDecideKey() == GSD_DECIDE_KEY_O) {
		main_work->is_jp_region = TRUE;
	}
	else {
		main_work->is_jp_region = FALSE;
	}
	
	// 帯テキストの初期表示位置設定
	main_work->obi_pos[0] = 0.f;
	main_work->obi_pos[1] = DMD_RANK_OBI_MOVE_START_POS;
}



// ==========================================================================
// dmRankSetClearInfo
/*!
	クリア情報設定処理
 */
// ==========================================================================
void dmRankSetClearInfo(DMS_RANK_MAIN_WORK *main_work)
{
	UNREFERENCED_PARAMETER(main_work);
/*	u32 no_active = 0;
	u32 i = 0;
	u32 j = 0;

	for (s32 i = 0; i < DME_RANK_STAGE_NUM; i++) {
		// 仮の設定処理
		main_work->is_clear_stage[i] = 1;
		
//		main_work->is_clear_stage = // クリア情報配列か何かから取得 
	}
	
	
	for (i = 0; i < DME_RANK_STAGE_NUM; i++) {
		
		// スペシャルステージ
		if (i > DME_RANK_STAGE_F_1) {
			if (!main_work->is_clear_stage[i]) {
				// スペシャルステージを選択不可にする
				main_work->is_clear_stage[i] = -1;
			}
		}
		if (i == 17) {
			main_work->is_clear_stage[i] = 1;
		}
		
		// FINALステージ
		else if (i == DME_RANK_STAGE_F_1) {
			for (j = DME_RANK_STAGE_1_B; j < DME_RANK_STAGE_F_1; j += 4) {
				if (main_work->is_clear_stage[j] != 1) {
					no_active = 1;
				}
			}
			
			if (no_active) {
				// FINALステージを選択不可にする(表示させない)
				main_work->is_clear_stage[i] = -1;
			}
		}
		// BOSSステージ
		else if (i < DME_RANK_STAGE_F_1 && ((i + 1) % 4 == 0)) {
			for (j = 0; j < 3; j++) {
				if (!main_work->is_clear_stage[i - 3 + j]) {
					no_active = 1;
				}
			}
			
			if (no_active) {
				main_work->is_clear_stage[i] = -1;
			}
		}
		
		no_active = 0;
	}
	
	
	if (main_work->is_clear_stage[DME_RANK_STAGE_1_B] == 1
		&& main_work->is_clear_stage[DME_RANK_STAGE_2_B] == 1
		&& main_work->is_clear_stage[DME_RANK_STAGE_3_B] == 1
		&& main_work->is_clear_stage[DME_RANK_STAGE_4_B] == 1) {
		main_work->is_final_open = 1;
	}
*/
}



// ==========================================================================
// dmRankProcMain
/*!
	ランキング画面メインプロシージャ処理
 */
// ==========================================================================
void dmRankProcMain(MTS_TASK_TCB *tcb)
{
	DMS_RANK_MAIN_WORK	*main_work;

	// ワーク取得
	main_work = (DMS_RANK_MAIN_WORK *)mtTaskGetTcbWork(tcb);

	// 終了処理
	if (main_work->flag & DMD_RANK_FLAG_EXIT) {
		// タスククリア
		mtTaskClearTcb(tcb);
		
		dm_rank_mgr_p = NULL;
		
		// メインゲーム中の場合、タスクポーズ解除
		if (dm_rank_is_pause_maingame) {
			mtTaskEndPause();
		}
		
		// イベント遷移用設定
		dmRankSetNextEvt(main_work);
	}
	
	// 接続エラー判定
	if (DmRankSysIsError()
		&& main_work->flag & DMD_RANK_DISP_FLAG_CHECK_NET_ERROR) {
		DmRankSysEnd();
		
		// 切断ミス＝切断開始となるので、遷移
		main_work->proc_menu_update = dmRankProcCheckEndErrorWin;
		
		main_work->flag &= ~DMD_RANK_DISP_FLAG_CHECK_NET_ERROR;
		
		main_work->flag &= ~DMD_RANK_FLAG_DECIDE;
		main_work->flag &= ~DMD_RANK_FLAG_CANCEL;
		main_work->proc_input = NULL;
		main_work->proc_win_input = NULL;
		main_work->win_timer = 0;
		
//		return;
	}
	
	// システム関連処理(サインアウト時はタイトルへ戻す)
	if (main_work->flag & DMD_RANK_FLAG_SIGN_OUT_EXIT
		&& !AoAccountIsCurrentEnable()) {
		
		main_work->proc_menu_update = dmRankProcCheckDisconnectNetwork;
		
		DmRankSysEnd();
		
		// サインアウト終了フラグOFF
		main_work->flag &= ~DMD_RANK_FLAG_SIGN_OUT_EXIT;
		main_work->flag |= DMD_RANK_FLAG_NEXT_EVT_TITLE;
		
		// BGMフェードアウト開始
		DmSndBgmPlayerExit();
		main_work->is_snd_exit = TRUE;
//		DmSoundStopBGM(DMD_STGSLCT_BGM_FADEOUT_TIME);
		
		// ウインドウ遷移関連設定
		main_work->flag &= ~DMD_RANK_FLAG_DECIDE;
		main_work->flag &= ~DMD_RANK_FLAG_CANCEL;
		main_work->proc_input = NULL;
		main_work->proc_win_input = NULL;
		main_work->win_timer = 0;
	}
	
	
	// ウインドウ処理用プロシージャ
	if (main_work->proc_win_update) {
		main_work->proc_win_update(main_work);
	}

	// メニュー処理用プロシージャ
	if (main_work->proc_menu_update) {
		main_work->proc_menu_update(main_work);
	}

	// 描画設定プロシージャ
	if (main_work->proc_draw) {
		main_work->proc_draw(main_work);
	}

}


// ==========================================================================
// dmRankDest
/*!
	ランキング画面終了処理
 */
// ==========================================================================
void dmRankDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	
}



// ==========================================================================
// dmRankSetNextEvt
/*!
	次のイベント設定処理
 */
// ==========================================================================
void dmRankSetNextEvt(DMS_RANK_MAIN_WORK *main_work)
{
	s16 tmp_prev_evt = 0;
	
	UNREFERENCED_PARAMETER(main_work);
	
	tmp_prev_evt = SyGetEvtInfo()->old_evt_id;
	
	// ここで一つ前のイベントがタイトルの場合、メインメニューへ切り替える(暫定)
	if (tmp_prev_evt == GSD_EVT_ID_TITLE) {
		tmp_prev_evt = GSD_EVT_ID_MAINMENU;
	}
	
	if (main_work->flag & DMD_RANK_FLAG_NEXT_EVT_TITLE) {
		tmp_prev_evt = GSD_EVT_ID_TITLE;
	}
	
	// メインゲーム中以外の場合、イベント遷移
	if (!dm_rank_is_pause_maingame) {
		SyDecideEvt(tmp_prev_evt);
		SyChangeNextEvt();
	}
}



// ==========================================================================
// dmRankLoadFontData
/*!
	フォントデータ読み込みリクエスト処理
 */
// ==========================================================================
void dmRankLoadFontData(DMS_RANK_MAIN_WORK *main_work)
{
	// gsFont構築
	GsFontBuild();
	
	main_work->proc_menu_update = dmRankIsLoadFontData;
}



// ==========================================================================
// dmRankIsLoadFontData
/*!
	フォントデータ読み込み終了チェック処理
 */
// ==========================================================================
void dmRankIsLoadFontData(DMS_RANK_MAIN_WORK *main_work)
{
	// gsFont構築
	if (GsFontIsBuilded()) {
		main_work->proc_menu_update = dmRankLoadRequest;
		
		return;
	}
}



// ==========================================================================
// dmRankLoadRequest
/*!
	ファイル読み込みリクエスト処理
 */
// ==========================================================================
void dmRankLoadRequest(DMS_RANK_MAIN_WORK *main_work)
{
	// ファイル読み込み開始
	main_work->arc_amb_fs[0] = amFsReadBackground(GSS_BASE_PATH "DEMO/RANKING/D_RANK.AMB");
	main_work->arc_amb_fs[1] = amFsReadBackground((char *)dm_rank_lng_amb_name_tbl[GsEnvGetLanguage()]);
	
	// メニュー共通データ読み込み
	for (int i = 0; i < 4; i++) {
		main_work->arc_cmn_amb_fs[i] = amFsReadBackground((char *)dm_rank_menu_cmn_amb_name_tbl[i]);
	}
	
	main_work->arc_cmn_amb_fs[4] = amFsReadBackground((char *)dm_rank_menu_cmn_lng_amb_name_tbl[GsEnvGetLanguage()]);
	
	// 次へ遷移
	main_work->proc_menu_update = dmRankProcLoadWait;
}


// ==========================================================================
// dmRankProcLoadWait
/*!
	ファイル読み込み待ち処理
 */
// ==========================================================================
void dmRankProcLoadWait(DMS_RANK_MAIN_WORK *main_work)
{
	// ファイル読み込み完了待ち
	if (dmRankIsDataLoad(main_work)) {		// ファイル読込み完了チェック関数にする

		// ファイル取得
		for (int i = 0; i < DME_RANK_DATA_TYPE_MAX; i++) {
			main_work->arc_amb[i] = main_work->arc_amb_fs[i]->buf;
			main_work->arc_amb_fs[i]->buf = NULL;
			
			amBindConv((u8 *)main_work->arc_amb[i]);
			
			// AMBファイルロード
			main_work->ama[i] = amBindGet((AMS_AMB_HEADER*)main_work->arc_amb[i]
										  , 0
										  );
			
			main_work->amb[i] = amBindGet((AMS_AMB_HEADER*)main_work->arc_amb[i]
										  , 1
										  );
			
			
			// リクエストクリア
			amFsClearRequest(main_work->arc_amb_fs[i]);
			main_work->arc_amb_fs[i] = NULL;

			// アドレス変換
			amConvertAddress(main_work->ama[i]);
			amConvertAddress(main_work->amb[i]);

			// テクスチャ構築開始
			AoTexBuild(&main_work->tex[i], main_work->amb[i]);
			AoTexLoad(&main_work->tex[i]);
		}
		
		// メニュー共通データ
		for (int i = 0; i < 5; i++) {
			main_work->arc_cmn_amb[i] = (void *)main_work->arc_cmn_amb_fs[i]->buf;
			main_work->arc_cmn_amb_fs[i]->buf = NULL;
			
			amBindConv((u8 *)main_work->arc_cmn_amb[i]);
			
			// AMBファイルロード
			main_work->cmn_ama[i] = amBindGet((AMS_AMB_HEADER*)main_work->arc_cmn_amb[i]
											  , 0
											  );
			
			main_work->cmn_amb[i] = amBindGet((AMS_AMB_HEADER*)main_work->arc_cmn_amb[i]
											  , 1
											  );
			
			// リクエストクリア
			amFsClearRequest(main_work->arc_cmn_amb_fs[i]);
			main_work->arc_cmn_amb_fs[i] = NULL;
			
			// アドレス変換
			amConvertAddress(main_work->cmn_ama[i]);
			amConvertAddress(main_work->cmn_amb[i]);

			// テクスチャ構築開始
			AoTexBuild(&main_work->cmn_tex[i], main_work->cmn_amb[i]);
			AoTexLoad(&main_work->cmn_tex[i]);
		}
		
		
		if (!dm_rank_is_pause_maingame) {
			// サウンド構築
			DmSndBgmPlayerInit();
//			DmSoundBuild();
		}
		
		// ランキングリストのユーザー名保存領域確保
		for (int i = 0; i < DMD_RANK_DISP_RANKING_NUM; i++) {
			main_work->disp_name[i] = (char *)amMemAlloc(sizeof(s8) * DMD_RANK_DISP_LIST_NAME_CHAR_NUM);
			amZeroMemory(main_work->disp_name[i], (sizeof(s8) * DMD_RANK_DISP_LIST_NAME_CHAR_NUM));
		}
		
		// ランキングリストデータの一時保存用のメモリ確保
		dmRankInitAllocRankTmpStrg(main_work);
		
		// 次へ遷移
		main_work->proc_menu_update = dmRankProcTexBuildWait;
	}
}


// ==========================================================================
// dmRankProcTexBuildWait
/*!
	テクスチャ構築待ち処理
 */
// ==========================================================================
void dmRankProcTexBuildWait(DMS_RANK_MAIN_WORK *main_work)
{
	// テクスチャ構築完了判定
	if (dmRankIsTexLoad(main_work) == 1) {
		// 次へ遷移
		main_work->proc_menu_update = dmRankProcCheckLoadingEnd;
		
		// ローディング終了設定
//		DmLoadingSetLoadComplete();
		
		if (!dm_rank_is_pause_maingame) {
//			DmSoundInit();
		}
		else {
			// 現状、Wii版はメインゲーム中にメニューが入ることはないため、Wii版は未対応
			main_work->bgm_scb = GsSoundAssignScb(GSE_SND_DATA_TYPE_CRIAUDIO);
			
			// ユーザーBGM再生時のミュート対応
			main_work->bgm_scb->flag |= GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM;
			
			// SEハンドル確保
			main_work->se_handle = GsSoundAllocSeHandle();
		}
	}
}



// ==========================================================================
// dmRankProcCheckLoadingEnd
/*!
	ローディング終了待ち処理
 */
// ==========================================================================
void dmRankProcCheckLoadingEnd(DMS_RANK_MAIN_WORK *main_work)
{
	// テクスチャ構築完了判定
//	if (DmLoadingIsExit()) {
		// 次へ遷移
		main_work->proc_menu_update = dmRankProcCreateAct;
		
		// フェード処理開始
		if (dm_rank_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_BLACK_FADEIN
								, DMD_RANK_FADEIN_TIME
								, TRUE
								);
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEIN
						   , DMD_RANK_FADEIN_TIME
						   );
		}
//	}
}



// ==========================================================================
// dmRankProcCreateAct
/*!
	アクション生成処理
 */
// ==========================================================================
void dmRankProcCreateAct(DMS_RANK_MAIN_WORK *main_work)
{
	// アクション構築
	for (u32 i = 0; i < ACT_NUM; ++i) {
		// ファイル選別
		const void *ama;
		AOS_TEXTURE *tex;
		
		if (i >= ACT_TEX_BACK_FIX) {
			ama = main_work->cmn_ama[4];
			tex = &main_work->cmn_tex[4];
		}
		else if (i >= ACT_WIN_LINE) {
			ama = main_work->cmn_ama[3];
			tex = &main_work->cmn_tex[3];
		}
		else if (i >= ACT_OBI_C) {
			ama = main_work->cmn_ama[2];
			tex = &main_work->cmn_tex[2];
		}
		else if (i >= ACT_BTN_CANCEL1) {
			ama = main_work->cmn_ama[1];
			tex = &main_work->cmn_tex[1];
		}
		else if (i >= ACT_WAVE_BG) {
			ama = main_work->cmn_ama[0];
			tex = &main_work->cmn_tex[0];
		}
		
		else if (i >= ACT_TEX_TITLE) {
			ama = main_work->ama[DME_RANK_DATA_TYPE_LANG_DATA];
			tex = &main_work->tex[DME_RANK_DATA_TYPE_LANG_DATA];
		}
		else {
			ama = main_work->ama[DME_RANK_DATA_TYPE_CMN_DATA];
			tex = &main_work->tex[DME_RANK_DATA_TYPE_CMN_DATA];
		}
		
		// 構築
		AoActSetTexture(AoTexGetTexList(tex));
		main_work->act[i] = AoActCreate(ama, g_dm_act_id_tbl[i]);
	}
	
	
	// テクスチャセットまで出来たので描画プロシージャを設定
	main_work->proc_draw = dmRankProcActDraw;
	
#if !_WII
	if (main_work->is_rank_view_only) {
		main_work->disp_flag |= DMD_RANK_DISP_FLAG_RANK_VIEW;
		
		main_work->disp_flag |= DMD_RANK_DISP_FLAG_MENU_TAB;
		main_work->state = DME_RANK_MODE_STATE_RANK_MAIN;
		
		main_work->index_act_anime_frm[0] = DMD_RANK_VIEW_INDEX_ANIME_FRM;
		main_work->index_act_anime_frm[1] = DMD_RANK_VIEW_INDEX_ANIME_FRM;
		
		main_work->obi_pos_y = DMD_RANK_OBI_NODISP_POS_Y;
		
		// ここでステージID設定
		dmRankSetStageNoForViewOnly(main_work);
	}
	else {
		main_work->disp_flag |= DMD_RANK_DISP_FLAG_RANK_MENU;
	}
#else
	main_work->disp_flag |= DMD_RANK_DISP_FLAG_RANK_MENU;
#endif
	
	// イベント遷移
	main_work->proc_menu_update = dmRankProcFadeIn;
	main_work->proc_win_update = dmRankProcWindowNodispIdle;
	
	// BGM再生開始
	if (!dm_rank_is_pause_maingame) {
		DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_MENU);
//		DmSoundPlayMenuBGM(DME_SOUND_BGM_IDX_MENU
//						   , DMD_RANK_BGM_FADEIN_TIME);
	}
	else {
		// ゲーム中のBGM再生
		GsSoundPlayBgm(main_work->bgm_scb
					   , "snd_sng_menu"
					   , DMD_RANK_BGM_FADEIN_TIME
					   );
	}
}



// ==========================================================================
// dmRankProcFadeIn
/*!
	フェードイン中処理
 */
// ==========================================================================
void dmRankProcFadeIn(DMS_RANK_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
		IzFadeExit();

		// サーバ接続中処理へ
		main_work->proc_menu_update = dmRankProcCheckConnectNetwork;

		// サーバ接続開始
		DmRankSysStart();

#if _WII
		main_work->announce_flag |= 1 << DME_RANK_WIN_NET_CONNECT;
#endif

		// サーバ接続中フラグON(ここから接続エラーチェック開始
		main_work->flag |= DMD_RANK_DISP_FLAG_CHECK_NET_ERROR;
		
	}
}



// ==========================================================================
// dmRankProcCheckConnectNetwork
/*!
	ネットワーク接続チェック中処理
 */
// ==========================================================================
void dmRankProcCheckConnectNetwork(DMS_RANK_MAIN_WORK *main_work)
{
#if !_WII
	// サーバ接続が正しく完了した場合
	if (DmRankSysIsConnected()) {
		
		if (main_work->is_rank_view_only) {
			// ランキングビューへ
			main_work->proc_menu_update = dmRankProcRankSetDataUpload;
			
			// ランキングデータアップロードチェック
			dmRankSetDataUploadCheck(main_work);
			
			// ランキングデータ更新が終るまで入力プロシージャは設定しない
			main_work->proc_input = NULL;
		}
		else {
			// ランキングメニュー中へ
			main_work->proc_menu_update = dmRankProcRankMenuIdle;
			
			// ランキングメニュ用入力プロシージャに設定
			main_work->proc_input = dmRankInputProcRankMenu;
		}
		
		// サインアウト終了フラグON
		main_work->flag |= DMD_RANK_FLAG_SIGN_OUT_EXIT;
		
		return;
	}
#else
	// サーバ接続が正しく完了してウインドウが閉じたら
	if (main_work->flag & DMD_RANK_FLAG_NET_CNCT_END
		&& DmRankSysIsConnected()) {
		// ランキングメニュー中へ
		main_work->proc_menu_update = dmRankProcRankMenuIdle;
		
		// ランキングメニュ用入力プロシージャに設定
		main_work->proc_input = dmRankInputProcRankMenu;
		
		main_work->flag &= ~DMD_RANK_FLAG_NET_CNCT_END;
		
		// サインアウト終了フラグON
		main_work->flag |= DMD_RANK_FLAG_SIGN_OUT_EXIT;
		
		return;
	}
#endif
}



// ==========================================================================
// dmRankProcRankMenuIdle
/*!
	ランクメニュー画面の入力待ち中処理
 */
// ==========================================================================
void dmRankProcRankMenuIdle(DMS_RANK_MAIN_WORK *main_work)
{
	// 入力処理
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}

	// キャンセルフラグONならば
	if (main_work->flag & DMD_RANK_FLAG_CANCEL) {
		// フラグOFF
		main_work->flag &= ~DMD_RANK_FLAG_DECIDE;
		main_work->flag &= ~DMD_RANK_FLAG_CANCEL;
		
		// ランキング終了設定処理
		dmRankSetRankingEndParam(main_work);
		
		return;
	}

	// 決定フラグONならば
	if (main_work->flag & DMD_RANK_FLAG_DECIDE) {
		main_work->proc_menu_update = dmRankProcRankMenuOutEfct;
		
		main_work->flag &= ~DMD_RANK_FLAG_DECIDE;
		
		main_work->proc_input = NULL;
		
		if (!dm_rank_is_pause_maingame) {
			DmSoundPlaySE("Ok");
		}
		else {
			GsSoundPlaySe("Ok", main_work->se_handle);
		}
		
		// 表示データ初期化
		dmRankSetDispRankInitData(main_work);
		main_work->mode_tex_efct_frm = 1;
		
		// 指定座標に設定
		main_work->zone_inout_efct_pos[1] = DMD_RANK_MENU_TAB_DISP_POS;
		main_work->dst_zone_inout_efct_pos[1] = DMD_RANK_MENU_TAB_NODISP_POS;
		main_work->src_zone_inout_efct_pos[1] = DMD_RANK_MENU_TAB_DISP_POS;
		
		// フラグOFF
		main_work->flag &= ~DMD_RANK_FLAG_DECIDE;
		main_work->flag &= ~DMD_RANK_FLAG_CANCEL;
		
		return;
	}

	// 縦カーソル切り替え演出
	if (main_work->flag & DMD_RANK_FLAG_MENU_UP_INPUT
		|| main_work->flag & DMD_RANK_FLAG_MENU_DOWN_INPUT) {
		dmRankSetMenuChngFocusZone(main_work);
		
		if (!dm_rank_is_pause_maingame) {
			DmSoundPlaySE("Cursol");
		}
		else {
			GsSoundPlaySe("Cursol", main_work->se_handle);
		}
		
		// 入力フラグOFF
		main_work->flag &= ~DMD_RANK_FLAG_MENU_UP_INPUT;
		main_work->flag &= ~DMD_RANK_FLAG_MENU_DOWN_INPUT;
	}
	
	// 横FOCUS項目切り替え
	if (main_work->flag & DMD_RANK_FLAG_MENU_LEFT_INPUT
		|| main_work->flag & DMD_RANK_FLAG_MENU_RIGHT_INPUT) {
		dmRankSetMenuChngFocusStage(main_work);
		
		if (!dm_rank_is_pause_maingame) {
			DmSoundPlaySE("Cursol");
		}
		else {
			GsSoundPlaySe("Cursol", main_work->se_handle);
		}
		
		// 入力フラグOFF
		main_work->flag &= ~DMD_RANK_FLAG_MENU_LEFT_INPUT;
		main_work->flag &= ~DMD_RANK_FLAG_MENU_RIGHT_INPUT;
	}
	
	
	// カーソル移動
	if (main_work->flag & DMD_RANK_FLAG_MENU_CHNG_CRSR_EFCT) {
		dmRankSetRankMenuChangeFocus(main_work);
		
		if (dmRankIsRankMenuChangeFocus(main_work)) {
			main_work->flag &= ~DMD_RANK_FLAG_MENU_CHNG_CRSR_EFCT;
		}
	}
	
}



// ==========================================================================
// dmRankProcRankMenuOutEfct
/*!
	ランクメニューテーブル掃け演出中処理
 */
// ==========================================================================
void dmRankProcRankMenuOutEfct(DMS_RANK_MAIN_WORK *main_work)
{
	// 掃け演出が終了したら
	if (dmRankIsRankMenuOutEfct(main_work)
		&& dmRankIsObiOutEfctEnd(main_work)) {
		main_work->proc_menu_update = dmRankProcRankViewInEfct;

		// 指定座標に設定
		main_work->rank_inout_efct_pos[0] = DMD_RANK_VIEW_TAB_NODISP_POS;
		main_work->dst_rank_inout_efct_pos[0] = DMD_RANK_VIEW_TAB_DISP_POS;
		main_work->src_rank_inout_efct_pos[0] = DMD_RANK_VIEW_TAB_NODISP_POS;

		// ACT側も演出用の座標に設定
		
		main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_RANK_MENU;
		
		main_work->disp_flag |= DMD_RANK_DISP_FLAG_RANK_VIEW;
		
		main_work->disp_flag |= DMD_RANK_DISP_FLAG_MENU_TAB;
		main_work->state = DME_RANK_MODE_STATE_RANK_MAIN;
		
		main_work->index_act_anime_frm[0] = DMD_RANK_VIEW_INDEX_ANIME_FRM;
		main_work->index_act_anime_frm[1] = DMD_RANK_VIEW_INDEX_ANIME_FRM;

		return;
	}
	
	// 掃け演出処理
	dmRankSetRankMenuOutEfct(main_work);
	
	if (!dmRankIsObiOutEfctEnd(main_work)) {
		dmRankSetObiOutEfct(main_work);
	}

	// アクティブ分の演出開始フラグON
//	if (main_work->efct_time == 20) {
//		main_work->efct_out_flag |= 1 << main_work->cur_zone;
//	}

}



// ==========================================================================
// dmRankProcRankViewInEfct
/*!
	ランキング閲覧画面のテーブル入り演出中処理
 */
// ==========================================================================
void dmRankProcRankViewInEfct(DMS_RANK_MAIN_WORK *main_work)
{
	// 入り演出が終了したら
	if (dmRankIsRankViewInEfct(main_work)) {
		// ランキングデータアップロードチェック
		dmRankSetDataUploadCheck(main_work);
		
		return;
	}
	
	// 入り演出処理
	dmRankSetRankViewInEfct(main_work);

	// 座標再設定
	
}


#if _WII
// ==========================================================================
// dmRankProcRankIsMyDataUpload
/*!
	ランキング閲覧画面のデータアップロード待ち中処理
 */
// ==========================================================================
void dmRankProcRankIsMyDataUpload(DMS_RANK_MAIN_WORK *main_work)
{
	if (!main_work->announce_flag) {
		
		// キャンセル処理終了判定
		if (main_work->flag & DMD_RANK_FLAG_IS_UPLOAD_START) {
			// ランキングデータUL中処理へ
			main_work->proc_menu_update = dmRankProcRankSetDataUpload;
			
			main_work->proc_win_input = NULL;
			main_work->proc_input = NULL;
			
			main_work->is_data_upload = TRUE;
			
			main_work->flag &= ~DMD_RANK_FLAG_IS_UPLOAD_START;
		}
		
		// キャンセル判定
		if (main_work->flag & DMD_RANK_FLAG_IS_UPLOAD_CANCEL) {
			main_work->flag &= ~DMD_RANK_FLAG_CANCEL;
			
			// アップロードなしでボードDL
			main_work->proc_menu_update = dmRankProcRankSetDataUpload;
			
			main_work->proc_win_input = NULL;
			main_work->proc_input = NULL;
			
			main_work->is_data_upload = FALSE;
			
			main_work->flag &= ~DMD_RANK_FLAG_IS_UPLOAD_CANCEL;
		}
	}
	
	// 切り替え演出
	if (main_work->flag & DMD_RANK_FLAG_CHNG_ZONE_WIN_EFCT) {
		dmRankSetZoneChangeEfct(main_work);
	}
}
#endif



// ==========================================================================
// dmRankProcRankSetDataUpload
/*!
	ランキング閲覧画面のアップロードデータ設定処理
 */
// ==========================================================================
void dmRankProcRankSetDataUpload(DMS_RANK_MAIN_WORK *main_work)
{
	bool is_friend_tab = FALSE;
	BOOL is_data_upload = FALSE;
	DME_RANK_SYS_SS tmp_sonic_type = DMD_RANK_SYS_SS_BOTH;
	DME_RANK_SYS_RANK set_rank_mode = DMD_RANK_SYS_RANK_TIME;
	
	// ※WIIはボード設定の前にこれで
	// これはアップロードが必要かを通知するチェック処理
//	DmRankSysOwnRecodeIsUpdate();
	
	// そしてユーザーにアップロードするかを聞いて
	// その結果をボード設定時のis_uploadを
	// TRUEにするかFALSEにするかを決めて
	// ボード設定をする
	
	// フレンドタブを選択中ならばTRUE
	if (main_work->cur_slct_tab == 2) {
		is_friend_tab = TRUE;
	}
	else {
		is_friend_tab = FALSE;
	}
	
	// PS3とXBOX360は常にTRUE	※WIIのみ選択させるためこれがある
	is_data_upload = main_work->is_data_upload;
	
	if (main_work->cur_game_mode == DME_RANK_DISP_RANK_SCORE) {
		set_rank_mode = DMD_RANK_SYS_RANK_SCORE;
	}
	else {
		set_rank_mode = DMD_RANK_SYS_RANK_TIME;
	}
	
	// スペステ以外はソニックタイプを自由に指定
	if (!dmRankIsSonicOnlyStage(main_work)) {	// 保険
		tmp_sonic_type = (DME_RANK_SYS_SS)main_work->cur_sonic_type;
	}
	
	// スペステでは通常ソニックのみ指定
	else {
		tmp_sonic_type = DMD_RANK_SYS_SS_DISABLE;
	}
	
	// ボード設定(実質のアップロード開始)	※必ず行う
	DmRankSysNoticeBoard((DME_RANK_SYS_BOARD)main_work->cur_view_stage
						 , set_rank_mode
						 , tmp_sonic_type
						 , is_friend_tab
						 , is_data_upload
						 );
	
	// ボードが変更されなければアップロードは行われない
	// 上記のm_boardがそれに当たる
	// 但し、アップロード終了判定は必ずすること
	
	// 切り替え後のタブがMYSCOREの場合
	if (main_work->cur_slct_tab == DME_RANK_SCORE_TYPE_MYSCORE) {
		main_work->flag |= DMD_RANK_FLAG_SET_DISP_RANK_NEAR;
	}
	
	// 更新中表示
	main_work->disp_flag |= DMD_RANK_DISP_FLAG_NOW_UPDATE;
	
	if (is_data_upload
		&& dmRankIsSaveRunData(main_work)) {
		// ランキングデータUL中処理へ(キャンセル判定)
		main_work->announce_flag |= 1 << DME_RANK_WIN_NOW_REGIST;
		main_work->proc_menu_update = dmRankProcRankMyDataUpload;
	}
	else {
		// ランキングデータDL中処理へ(キャンセル判定)
		main_work->announce_flag |= 1 << DME_RANK_WIN_NOW_UPDATE;
		main_work->proc_menu_update = dmRankProcRankSuccessUpload;
	}
	
	
	// 切り替え演出
	if (main_work->flag & DMD_RANK_FLAG_CHNG_ZONE_WIN_EFCT) {
		dmRankSetZoneChangeEfct(main_work);
	}
}



// ==========================================================================
// dmRankProcRankMyDataUpload
/*!
	ランキング閲覧画面のデータアップロード待ち中処理
 */
// ==========================================================================
void dmRankProcRankMyDataUpload(DMS_RANK_MAIN_WORK *main_work)
{
	// ダウンロードがキャンセルされた場合
	if (main_work->is_upload_cancel
		&& !DmRankSysIsUpLoading()) {
		main_work->flag &= ~DMD_RANK_FLAG_UPLOAD_END;
		// アップロード成功へ遷移
		main_work->proc_menu_update = dmRankProcRankSuccessUpload;
	}
	
	// アップロード終了判定		※必ず行う(数フレームはアップロードにかかるため)
	else if (main_work->flag & DMD_RANK_FLAG_UPLOAD_END
		&& !DmRankSysIsUpLoading()) {
		main_work->flag &= ~DMD_RANK_FLAG_UPLOAD_END;
		// アップロード成功へ遷移
		main_work->proc_menu_update = dmRankProcRankSuccessUpload;
	}
	
	
	// 切り替え演出
	if (main_work->flag & DMD_RANK_FLAG_CHNG_ZONE_WIN_EFCT) {
		dmRankSetZoneChangeEfct(main_work);
	}
}



// ==========================================================================
// dmRankProcRankSuccessUpload
/*!
	ランキング閲覧画面のデータアップロード成功時処理
 */
// ==========================================================================
void dmRankProcRankSuccessUpload(DMS_RANK_MAIN_WORK *main_work)
{
	BOOL is_near_no = FALSE;
	
	if (main_work->flag & DMD_RANK_FLAG_SET_DISP_RANK_NEAR) {
	// ここに一つ前のボードと同じかどうかのチェックも入れる		※
		is_near_no = TRUE;
	}
	else {
		is_near_no = FALSE;
	}
	
	// フラグOFF
	main_work->flag &= ~DMD_RANK_FLAG_SET_DISP_RANK_NEAR;
	
	// 何位から表示したいかを設定
	// ダウンロード開始		※NEAR設定は初めの一回だけ
	if (is_near_no) {
		DmRankSysNoticeShowRankNo(DMD_RANK_SYS_OWN_NEAR);
	}
	else {
		DmRankSysNoticeShowRankNo(main_work->disp_top_rank_no);
	}
	
	main_work->disp_flag |= DMD_RANK_DISP_FLAG_NOW_UPDATE;
	
	main_work->announce_flag |= 1 << DME_RANK_WIN_NOW_UPDATE;
	
	main_work->proc_menu_update = dmRankProcRankViewDataDownload;
	
	
	// 切り替え演出
	if (main_work->flag & DMD_RANK_FLAG_CHNG_ZONE_WIN_EFCT) {
		dmRankSetZoneChangeEfct(main_work);
	}
}



// ==========================================================================
// dmRankProcRankViewDataDownload
/*!
	ランキング閲覧画面のデータダウンロード待ち中処理
 */
// ==========================================================================
void dmRankProcRankViewDataDownload(DMS_RANK_MAIN_WORK *main_work)
{
	u32 tmp_cmp_list_num = 0;
	
	// ダウンロード終了判定
	if (main_work->flag & DMD_RANK_FLAG_DOWNLOAD_END
		&& !(main_work->flag & DMD_RANK_FLAG_CHNG_ZONE_WIN_EFCT)) {
		
		// 受信成功へ遷移
		main_work->proc_menu_update = dmRankProcRankViewIdle;
		main_work->proc_input = dmRankInputProcRankView;
		
		// データアップロード時の実績設定
		HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING_UPLOAD_RECORD);
		
		// この関数で表示数を取得
		main_work->disp_list_num = (int)DmRankSysGetShowNum();
		
		// 表示される順番(実順位)	※周辺取得の場合、トップが何位か分からないため
		if (DmRankSysGetShowNum() > 0) {
			main_work->disp_top_rank_no = DmRankSysGetRealRankNo(0);
		}
		
		main_work->flag &= ~DMD_RANK_FLAG_DOWNLOAD_END;
		
		if (main_work->disp_list_num > 0) {
			tmp_cmp_list_num = (u32)(main_work->disp_list_num - 1);
		}
		else {
			tmp_cmp_list_num = 0;
		}
		
		if (main_work->cur_slct_data > tmp_cmp_list_num) {
			main_work->cur_slct_data = tmp_cmp_list_num;
			
			// カーソル移動先設定		※今は移動演出抜きの仮設定(直接表示位置を変更)
			main_work->rank_crsr_pos[0] = 32.f * main_work->cur_slct_data;
			main_work->rank_crsr_pos[1] = 32.f * main_work->cur_slct_data;
		}
		
		// ダウンロードがキャンセルされた場合
		if (main_work->is_download_cancel) {
			dmRankSetDispRankTmpStrg(main_work);
		}
		
		// 先頭取得でなく、表示数０の場合
		else if (main_work->disp_top_rank_no != 0
				 && main_work->disp_list_num == 0) {
			dmRankSetDispRankTmpStrg(main_work);
		}
		
		else {
			// 保険
			if (DmRankSysGetShowNum() == 0) {
				main_work->disp_top_rank_no = 0;
			}
			
			// ここで仮としてダウンロードしたデータを設定
			dmRankSetDownloadRankData(main_work);
			
			// ここでダウンロードしたデータを一時保存
			dmRankSetSaveDispRankTmpStrg(main_work);
		}
		
		main_work->disp_flag |= DMD_RANK_DISP_FLAG_RANK_LIST;
		main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_NOW_UPDATE;
	}
	
	
	// 切り替え演出
	if (main_work->flag & DMD_RANK_FLAG_CHNG_ZONE_WIN_EFCT) {
		dmRankSetZoneChangeEfct(main_work);
	}
}



// ==========================================================================
// dmRankProcRankViewIdle
/*!
	ランクビュー画面の入力待ち中処理
 */
// ==========================================================================
void dmRankProcRankViewIdle(DMS_RANK_MAIN_WORK *main_work)
{
	// 入力処理
	if (main_work->proc_input) {
#if _XBOX
		if (DmRankSysCamerCardShowIsFinished()) {
			main_work->proc_input(main_work);
		}
#else
		main_work->proc_input(main_work);
#endif
	}

	// キャンセルフラグONならば
	if (main_work->flag & DMD_RANK_FLAG_CANCEL) {
		// フラグOFF
		main_work->flag &= ~DMD_RANK_FLAG_DECIDE;
		main_work->flag &= ~DMD_RANK_FLAG_CANCEL;
		
		// ここで直接ランクビューに来たかどうかを判別
		if (main_work->is_rank_view_only) {
			// 直接来た場合、そのまま終了
			dmRankSetRankingEndParam(main_work);
		}
		else {
			// ランクメニューを通った場合、ランクメニューへ戻る
			dmRankSetRankViewEndParam(main_work);
		}
		
		return;
	}

#if _XBOX
	// 決定フラグONならばゲーマータグ表示※XBOXのみ必要
	if (main_work->flag & DMD_RANK_FLAG_DECIDE) {
		DmRankSysGamerCardShow(main_work->cur_slct_data);
		
		main_work->flag &= ~DMD_RANK_FLAG_DECIDE;
		main_work->flag &= ~DMD_RANK_FLAG_CANCEL;
		
		if (!dm_rank_is_pause_maingame) {
			DmSoundPlaySE("Ok");
		}
		else {
			GsSoundPlaySe("Ok", main_work->se_handle);
		}
		
		return;
	}
#endif	// #if _XBOX


	// カーソル切り替え		・ゾーン・タブ・順位・タイムとスコア・ソニックタイプ
	// 縦カーソル切り替え演出	※最上下の場合のみで順位切り替え
	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_RANK_UP
		|| main_work->flag & DMD_RANK_FLAG_RANK_CHNG_RANK_DOWN) {
		
		dmRankSetViewChngFocusRank(main_work);
		
		// 入力フラグOFF
		main_work->flag &= ~DMD_RANK_FLAG_RANK_CHNG_RANK_UP;
		main_work->flag &= ~DMD_RANK_FLAG_RANK_CHNG_RANK_DOWN;
		
		return;
	}
	
	// 横FOCUS項目切り替え		タブ切り替え
	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_TAB_LEFT
		|| main_work->flag & DMD_RANK_FLAG_RANK_CHNG_TAB_RIGHT) {
		dmRankSetViewChngFocusTab(main_work);
		
		if (!dm_rank_is_pause_maingame) {
			DmSoundPlaySE("Cursol");
		}
		else {
			GsSoundPlaySe("Cursol", main_work->se_handle);
		}
		
		// 入力フラグOFF
		main_work->flag &= ~DMD_RANK_FLAG_RANK_CHNG_TAB_LEFT;
		main_work->flag &= ~DMD_RANK_FLAG_RANK_CHNG_TAB_RIGHT;
		
		main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_RANK_LIST;
		
		return;
	}
	
	// LB/RBにてACT切り替え
	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_ACT_LEFT
		|| main_work->flag & DMD_RANK_FLAG_RANK_CHNG_ACT_RIGHT) {
		dmRankSetViewChngFocusStage(main_work);
		
		dmRankSetViewChngZoneEfctInit(main_work);
		
		if (!dm_rank_is_pause_maingame) {
			DmSoundPlaySE("Cursol");
		}
		else {
			GsSoundPlaySe("Cursol", main_work->se_handle);
		}
		
		// データダウンロード開始
		main_work->proc_menu_update = dmRankProcRankSetDataUpload;
		
		// ランキングの一時保存データをクリア
		dmRankSetClearDispRankTmpStrg(main_work);
		
		// 表示データ初期化
		dmRankSetDispRankInitData(main_work);
		
		// ランキングデータアップロード設定
		dmRankSetDataUploadCheck(main_work);
		
		main_work->flag &= ~DMD_RANK_FLAG_RANK_CHNG_ACT_LEFT;
		main_work->flag &= ~DMD_RANK_FLAG_RANK_CHNG_ACT_RIGHT;
		
		main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_RANK_LIST;
		
		return;
	}
	
	// タイム・スコア切り替え
	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_TIME) {
		// モード変数切り替え
		main_work->cur_game_mode ^= DME_RANK_DISP_RANK_TIME;
		
		main_work->flag |= DMD_RANK_FLAG_CHNG_GAME_MODE_EFCT;
		
		// 切り替え時は必ず0から始まるように設定
		main_work->disp_top_rank_no = 0;
		
		main_work->cur_slct_data = 0;
		
		// カーソル移動先設定
		main_work->rank_crsr_pos[0] = 32.f * main_work->cur_slct_data;
		main_work->rank_crsr_pos[1] = 32.f * main_work->cur_slct_data;
		
		// データダウンロード開始
		main_work->proc_menu_update = dmRankProcRankSetDataUpload;
		
		// ランキングデータアップロードチェック
		dmRankSetDataUploadCheck(main_work);
		
		if (!dm_rank_is_pause_maingame) {
			DmSoundPlaySE("Cursol");
		}
		else {
			GsSoundPlaySe("Cursol", main_work->se_handle);
		}
		
		main_work->flag &= ~DMD_RANK_FLAG_RANK_CHNG_TIME;
		
		main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_RANK_LIST;
		
		return;
	}
	
	// ソニック状態切り替え
	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_SONIC) {
		// 表示ソニック状態切り替え
		main_work->cur_sonic_type++;
		
		if (!dm_rank_is_pause_maingame) {
			DmSoundPlaySE("Cursol");
		}
		else {
			GsSoundPlaySe("Cursol", main_work->se_handle);
		}
		
		// 切り替え時は必ず0から始まるように設定
		main_work->disp_top_rank_no = 0;
		
		main_work->cur_slct_data = 0;
		
		// カーソル移動先設定
		main_work->rank_crsr_pos[0] = 32.f * main_work->cur_slct_data;
		main_work->rank_crsr_pos[1] = 32.f * main_work->cur_slct_data;
		
		if (main_work->cur_sonic_type >= DME_RANK_SONIC_TYPE_NUM) {
			main_work->cur_sonic_type = 0;
		}
		
		MTM_ASSERT(main_work->cur_sonic_type >= 0 && main_work->cur_sonic_type < DME_RANK_SONIC_TYPE_NUM);
		
		// データダウンロード開始
		main_work->proc_menu_update = dmRankProcRankSetDataUpload;
		
		// ランキングデータアップロードチェック
		dmRankSetDataUploadCheck(main_work);
		
		main_work->flag &= ~DMD_RANK_FLAG_RANK_CHNG_SONIC;
		main_work->flag |= DMD_RANK_FLAG_CHNG_SONIC_EFCT;
		
		main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_RANK_LIST;
		
		return;
	}
	
	
	// カーソル移動
	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_CRSR_EFCT) {
		dmRankSetRankViewCrsrChangeEfct(main_work);
		
		if (dmRankIsRankViewCrsrChangeEfct(main_work)) {
			main_work->flag &= ~DMD_RANK_FLAG_RANK_CHNG_CRSR_EFCT;
		}
	}
	
	
	// ここで仮としてダウンロードしたデータを設定
//	dmRankSetDownloadRankData(main_work);
	
	
}



// ==========================================================================
// dmRankProcRankViewOutEfct
/*!
	ランキング閲覧画面のテーブル掃け演出中処理
 */
// ==========================================================================
void dmRankProcRankViewOutEfct(DMS_RANK_MAIN_WORK *main_work)
{
	// 掃け演出が終了したら
	if (dmRankIsRankViewOutEfct(main_work)) {
		// ここの遷移はキャンセルか決定かで分岐する
		main_work->proc_menu_update = dmRankProcRankMenuInEfct;

		// STATEをZONE選択に設定
		main_work->disp_flag |= DMD_RANK_DISP_FLAG_RANK_MENU;
		main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_RANK_VIEW;
		main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_RANK_LIST;
		main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_MENU_TAB;
		main_work->state = DME_RANK_MODE_STATE_RANK_MENU;

		// 指定座標に設定
		main_work->zone_inout_efct_pos[1] = DMD_RANK_MENU_TAB_NODISP_POS;
		main_work->dst_zone_inout_efct_pos[1] = DMD_RANK_MENU_TAB_DISP_POS;
		main_work->src_zone_inout_efct_pos[1] = DMD_RANK_MENU_TAB_NODISP_POS;
		
		// ランキングの一時保存データをクリア
		dmRankSetClearDispRankTmpStrg(main_work);
		
		return;
	}
	
	// 掃け演出処理
	dmRankSetRankViewOutEfct(main_work);

	// 座標再設定
	
}



// ==========================================================================
// dmRankProcRankMenuInEfct
/*!
	ランクメニューのテーブル入り演出中処理
 */
// ==========================================================================
void dmRankProcRankMenuInEfct(DMS_RANK_MAIN_WORK *main_work)
{
	// 入り演出処理
	dmRankSetRankMenuInEfct(main_work);
	
	if (!dmRankIsObiInEfctEnd(main_work)) {
		dmRankSetObiInEfct(main_work);
	}

	// 入り演出が終了したら
	if (dmRankIsRankMenuInEfct(main_work)
		&& dmRankIsObiInEfctEnd(main_work)) {
		main_work->proc_menu_update = dmRankProcRankMenuIdle;
		main_work->proc_input = dmRankInputProcRankMenu;

		// 座標設定(保険)
		
		return;
	}
}



// ==========================================================================
// dmRankProcWindowNodispIdle
/*!
	ウインドウ非表示待ち中処理
 */
// ==========================================================================
void dmRankProcWindowNodispIdle(DMS_RANK_MAIN_WORK *main_work)
{
	// ウインドウ入力処理
	if (main_work->proc_win_input) {
		main_work->proc_win_input(main_work);
	}

	// メニュー遷移フラグONならば
	if (main_work->flag & DMD_RANK_FLAG_DISP_MENU
		|| main_work->announce_flag) {
		main_work->proc_win_update = dmRankProcWindowOpenEfct;

		// 通常処理の入力処理をなくす(二重入力を防ぐため)
		main_work->proc_input = NULL;

		// ウインドウ開閉演出時は入力処理なし
		main_work->proc_win_input = NULL;

		// ウインドウ演出用タイマー初期化
		main_work->win_timer = 0;

		// ウインドウ選択変数設定
		for (u32 i = DME_RANK_WIN_NET_CONNECT; i < DME_RANK_WIN_NUM; i++) {
			if (main_work->announce_flag & 1 << i) {
				main_work->win_mode = (s32)i;
				break;
			}
		}
//		main_work->win_cur_slct = 0;
		
		if (!dm_rank_is_pause_maingame) {
			DmSoundPlaySE("Window");
		}
		else {
			GsSoundPlaySe("Window", main_work->se_handle);
		}
		
		// ウインドウ演出中フラグON
		main_work->flag &= ~DMD_RANK_FLAG_DISP_MENU;
		main_work->disp_flag |= DMD_RANK_DISP_FLAG_WIN_DRAW;
	}
	
	
	if (main_work->disp_flag & DMD_RANK_DISP_FLAG_WIN_DRAW) {
		main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_WIN_DRAW;
	}
}




// ==========================================================================
// dmRankProcWindowOpenEfct
/*!
	ウインドウオープン中処理
 */
// ==========================================================================
void dmRankProcWindowOpenEfct(DMS_RANK_MAIN_WORK *main_work)
{
	// 演出終了チェック
	if (main_work->flag & DMD_RANK_FLAG_WIN_EFCT_END) {
		// ウインドウのプロシージャ設定
		main_work->proc_win_update = dmRankProcWindowAnnounceIdle;
		
		if (main_work->win_mode == DME_RANK_WIN_DO_YOU_REGIST) {
			main_work->proc_win_input = dmRankInputProcWinDoYouUpload;
		}
		else if (main_work->win_mode == DME_RANK_WIN_NOW_UPDATE
				 || main_work->win_mode == DME_RANK_WIN_NOW_REGIST) {
#if _WII || _PS3 || _XBOX
			main_work->proc_win_input = dmRankInputProcWinUploadCancel;
#endif
		}
		
		// ウインドウ内アクション表示フラグON
		main_work->disp_flag |= DMD_RANK_DISP_FLAG_WIN_ACT;
		
		// ウインドウ演出中フラグOFF
		main_work->flag &= ~DMD_RANK_FLAG_WIN_EFCT_END;
	}
	else {
		// ウインドウオープン演出処理
		dmRankSetWinOpenEfct(main_work);
	}
	
	
	if (!(main_work->disp_flag & DMD_RANK_DISP_FLAG_WIN_DRAW)) {
		main_work->disp_flag |= DMD_RANK_DISP_FLAG_WIN_DRAW;
	}

	// ウインドウ描画
//	dmRankWinSelectDraw(main_work);
}



// ==========================================================================
// dmRankProcWindowAnnounceIdle
/*!
	ウインドウ入力待ち処理
 */
// ==========================================================================
void dmRankProcWindowAnnounceIdle(DMS_RANK_MAIN_WORK *main_work)
{
	// タイマー更新
	main_work->wait_timer++;
	
	if (DmRankSysIsError()) {
		// 通常処理の入力処理をなくす(二重入力を防ぐため)
		main_work->proc_input = NULL;

		// ウインドウ開閉演出時は入力処理なし
		main_work->proc_win_input = NULL;

		// ウインドウ演出時間設定
		main_work->win_timer = DMD_RANK_WIN_EFCT_TIME;
		
		// ウインドウ内アクション表示フラグOFF
		main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_WIN_ACT;

		main_work->proc_win_update = dmRankProcWindowCloseEfct;

		// フラグOFF
		main_work->flag &= ~DMD_RANK_FLAG_DECIDE;
		main_work->flag &= ~DMD_RANK_FLAG_CANCEL;
		
		main_work->wait_timer = 0;
		
		return;
	}
	
	// ウインドウ入力処理
	if (main_work->proc_win_input) {
		main_work->proc_win_input(main_work);
	}

	// ウインドウのパターン分の処理をここに記述
	// ACT決定ウインドウ以外の場合
	if (main_work->win_mode == DME_RANK_WIN_NET_CONNECT) {
		if (DmRankSysIsSaveAfter()) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = 0;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmRankProcWindowCloseEfct;

			// フラグOFF
			main_work->flag &= ~DMD_RANK_FLAG_DECIDE;
			main_work->flag &= ~DMD_RANK_FLAG_CANCEL;
			
			main_work->wait_timer = 0;
			
			main_work->flag |= DMD_RANK_FLAG_WIN_EFCT_END;
		}
		
		else if (DmRankSysIsConnected()
			&& main_work->wait_timer > DMD_RANK_WIN_DISP_TIME) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_RANK_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmRankProcWindowCloseEfct;

			// フラグOFF
			main_work->flag &= ~DMD_RANK_FLAG_DECIDE;
			main_work->flag &= ~DMD_RANK_FLAG_CANCEL;
			
			main_work->wait_timer = 0;
		}
	}
	
	
	else if (main_work->win_mode == DME_RANK_WIN_DO_YOU_REGIST) {
		// メニュー遷移フラグONならば
		if (main_work->flag & DMD_RANK_FLAG_CANCEL
			|| (main_work->flag & DMD_RANK_FLAG_DECIDE
				&& main_work->win_cur_slct == 1)) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_RANK_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_WIN_ACT;

			// キャンセル設定
//			DmRankSysNoticeCancel();
			
			main_work->proc_win_update = dmRankProcWindowCloseEfct;
			
			if (!dm_rank_is_pause_maingame) {
				if (main_work->flag & DMD_RANK_FLAG_CANCEL) {
					DmSoundPlaySE("Cancel");
				}
				else {
					DmSoundPlaySE("Ok");
				}
			}
			else {
				if (main_work->flag & DMD_RANK_FLAG_CANCEL) {
					GsSoundPlaySe("Cancel", main_work->se_handle);
				}
				else {
					GsSoundPlaySe("Ok", main_work->se_handle);
				}
			}
			
//			main_work->announce_flag |= (1 << DME_RANK_WIN_NOW_UPDATE);
			
			main_work->flag |= DMD_RANK_FLAG_IS_UPLOAD_CANCEL;
			
			main_work->wait_timer = 0;
			
			// フラグOFF
			main_work->flag &= ~DMD_RANK_FLAG_DECIDE;
			main_work->flag &= ~DMD_RANK_FLAG_CANCEL;
		}
		
		if (main_work->flag & DMD_RANK_FLAG_DECIDE
			&& main_work->win_cur_slct == 0) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_RANK_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmRankProcWindowCloseEfct;
			
			if (!dm_rank_is_pause_maingame) {
				DmSoundPlaySE("Ok");
			}
			else {
				GsSoundPlaySe("Ok", main_work->se_handle);
			}
			
//			main_work->announce_flag |= (1 << DME_RANK_WIN_NOW_REGIST);
			
			main_work->flag |= DMD_RANK_FLAG_IS_UPLOAD_START;
			
			main_work->wait_timer = 0;
			
			// フラグOFF
			main_work->flag &= ~DMD_RANK_FLAG_DECIDE;
			main_work->flag &= ~DMD_RANK_FLAG_CANCEL;
		}
		
	}
	
	else if (main_work->win_mode == DME_RANK_WIN_NOW_REGIST) {
		if (main_work->flag & DMD_RANK_FLAG_CANCEL) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;
			
			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;
			
			// ウインドウ演出時間設定
			main_work->win_timer = DMD_RANK_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_WIN_ACT;
			
			// キャンセル設定
			DmRankSysNoticeCancel();
			
			main_work->proc_win_update = dmRankProcWindowCloseEfct;
			
			if (!dm_rank_is_pause_maingame) {
				DmSoundPlaySE("Cancel");
			}
			else {
				GsSoundPlaySe("Cancel", main_work->se_handle);
			}
			
			main_work->is_upload_cancel = TRUE;
			
			main_work->wait_timer = 0;
			
			// フラグOFF
			main_work->flag &= ~DMD_RANK_FLAG_DECIDE;
			main_work->flag &= ~DMD_RANK_FLAG_CANCEL;
		}
		
		else if (!DmRankSysIsUpLoading()
			&& main_work->wait_timer > DMD_RANK_WIN_DISP_TIME) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_RANK_WIN_EFCT_TIME;
//			main_work->flag |= DMD_RANK_FLAG_WIN_EFCT;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_WIN_ACT;
			
			main_work->wait_timer = 0;
			
			main_work->is_upload_cancel = FALSE;
			
			main_work->proc_win_update = dmRankProcWindowCloseEfct;
		}
		
#if _XBOX
		else if (!DmSaveIsExit()) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_RANK_WIN_EFCT_TIME;
//			main_work->flag |= DMD_RANK_FLAG_WIN_EFCT;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_WIN_ACT;
			
			main_work->wait_timer = 0;
			
			main_work->is_upload_cancel = FALSE;
			
			main_work->proc_win_update = dmRankProcWindowCloseEfct;
		}
#endif
	}
	
	else if (main_work->win_mode == DME_RANK_WIN_NOW_UPDATE) {
		if (main_work->flag & DMD_RANK_FLAG_CANCEL) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;
			
			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;
			
			// ウインドウ演出時間設定
			main_work->win_timer = DMD_RANK_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_WIN_ACT;
			
			// キャンセル設定
			DmRankSysNoticeCancel();
			
			main_work->proc_win_update = dmRankProcWindowCloseEfct;
			
			if (!dm_rank_is_pause_maingame) {
				DmSoundPlaySE("Cancel");
			}
			else {
				GsSoundPlaySe("Cancel", main_work->se_handle);
			}
			
			main_work->is_download_cancel = TRUE;
			
			main_work->wait_timer = 0;
			
			// フラグOFF
			main_work->flag &= ~DMD_RANK_FLAG_DECIDE;
			main_work->flag &= ~DMD_RANK_FLAG_CANCEL;
		}
		
		else if (!DmRankSysIsDownloading()
			&& main_work->wait_timer > DMD_RANK_WIN_DISP_TIME) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;
			
			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;
			
			// ウインドウ演出時間設定
			main_work->win_timer = DMD_RANK_WIN_EFCT_TIME;
//			main_work->flag |= DMD_RANK_FLAG_WIN_EFCT;
			
			main_work->is_download_cancel = FALSE;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_WIN_ACT;
			
			main_work->wait_timer = 0;
			
			main_work->proc_win_update = dmRankProcWindowCloseEfct;
		}
	}
	
	
	if (!(main_work->disp_flag & DMD_RANK_DISP_FLAG_WIN_DRAW)) {
		main_work->disp_flag |= DMD_RANK_DISP_FLAG_WIN_DRAW;
	}
	
	// ウインドウ描画
//	dmRankWinSelectDraw(main_work);
}



// ==========================================================================
// dmRankProcWindowCloseEfct
/*!
	ウインドウクローズ中処理
 */
// ==========================================================================
void dmRankProcWindowCloseEfct(DMS_RANK_MAIN_WORK *main_work)
{
	if (!(main_work->disp_flag & DMD_RANK_DISP_FLAG_WIN_DRAW)) {
		main_work->disp_flag |= DMD_RANK_DISP_FLAG_WIN_DRAW;
	}
	
	// 演出終了チェック
	if (main_work->flag & DMD_RANK_FLAG_WIN_EFCT_END) {
		// ウインドウのプロシージャ設定
		main_work->proc_win_update = dmRankProcWindowNodispIdle;
		
		// アナウンス分のフラグOFF
		main_work->announce_flag &= ~(1 << main_work->win_mode);

		if (main_work->win_mode == DME_RANK_WIN_NET_CONNECT) {
			// 入力処理設定
			main_work->flag |= DMD_RANK_FLAG_NET_CNCT_END;
		}
//		else if (main_work->win_mode == DME_RANK_WIN_NOW_REGIST) {
//			main_work->flag |= DMD_RANK_FLAG_IS_UPLOAD_END;
//		}
		else if (main_work->win_mode == DME_RANK_WIN_NOW_REGIST) {
			main_work->flag |= DMD_RANK_FLAG_UPLOAD_END;
		}
		else if (main_work->win_mode == DME_RANK_WIN_NOW_UPDATE) {
			// 入力処理設定
			main_work->flag |= DMD_RANK_FLAG_DOWNLOAD_END;
		}
		
		if (main_work->disp_flag & DMD_RANK_DISP_FLAG_WIN_DRAW) {
			main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_WIN_DRAW;
		}
		
		// ウインドウ演出中フラグOFF
		main_work->flag &= ~DMD_RANK_FLAG_WIN_EFCT_END;
	}
	
	// ウインドウオープン演出処理
	dmRankSetWinCloseEfct(main_work);
	
	
	// ウインドウ描画
//	dmRankWinSelectDraw(main_work);
}



// ==========================================================================
// dmRankProcCheckEndErrorWin
/*!
	サーバー切断中処理
 */
// ==========================================================================
void dmRankProcCheckEndErrorWin(DMS_RANK_MAIN_WORK *main_work)
{
	// 切断完了判定
	if (DmRankSysIsFinished()) {
		main_work->proc_menu_update = dmRankProcCheckDisconnectNetwork;
	}
}



// ==========================================================================
// dmRankProcCheckDisconnectNetwork
/*!
	サーバー切断中処理
 */
// ==========================================================================
void dmRankProcCheckDisconnectNetwork(DMS_RANK_MAIN_WORK *main_work)
{
	// 切断完了判定
	if (DmRankSysIsFinished()) {
		// 切断済みなのでへフェードアウト遷移
		main_work->proc_menu_update = dmRankProcFadeOut;
		
		// フェードアウト開始
		if (dm_rank_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_TAKEOEVER
								, IZE_FADE_TYPE_BLACK_FADEOUT
								, DMD_RANK_FADEOUT_TIME
								, TRUE
								);
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_TAKEOEVER
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_RANK_FADEOUT_TIME
						   );
		}
		
		// BGMフェードアウト開始
		if (!dm_rank_is_pause_maingame) {
#if defined (GSD_DEBUG_DEMO_SELECT) || defined (GSD_DEBUG_DEMO_SELECT_TOP)
			DmSndBgmPlayerExit();
			main_work->is_snd_exit = TRUE;
#endif
//			DmSoundStopBGM(DMD_RANK_BGM_FADEOUT_TIME);
		}
		else {
			GsSoundStopBgm(main_work->bgm_scb
						   , DMD_RANK_BGM_FADEOUT_TIME
						   );
			
			if (main_work->se_handle) {
				GsSoundFreeSeHandle(main_work->se_handle);
				main_work->se_handle = NULL;
			}
		}
	}
	
	if (main_work->disp_flag & DMD_RANK_DISP_FLAG_WIN_DRAW) {
		main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_WIN_DRAW;
	}
}



// ==========================================================================
// dmRankProcFadeOut
/*!
	フェードアウト中処理
 */
// ==========================================================================
void dmRankProcFadeOut(DMS_RANK_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
//		IzFadeExit();

		if (!dm_rank_is_pause_maingame) {
//			DmSoundExit();
		}
		else {
			GsSoundStopBgm(main_work->bgm_scb, 0);
			GsSoundResignScb(main_work->bgm_scb);
			main_work->bgm_scb	= NULL;
			
			if (main_work->se_handle) {
				GsSoundFreeSeHandle(main_work->se_handle);
				main_work->se_handle = NULL;
			}
		}
		
		// 遷移先なし
		main_work->proc_win_update = NULL;
		main_work->proc_menu_update = dmRankProcStopDraw;
		main_work->proc_draw = NULL;

		main_work->timer = 0;

		return;
	}
}

// ==========================================================================
// dmRankProcStopDraw
/*!
	描画停止処理
 */
// ==========================================================================
void dmRankProcStopDraw(DMS_RANK_MAIN_WORK *main_work)
{
	main_work->proc_menu_update = dmRankProcDataRelease;
}

// ==========================================================================
// dmRankProcDataRelease
/*!
	ファイル解放リクエスト処理
 */
// ==========================================================================
void dmRankProcDataRelease(DMS_RANK_MAIN_WORK *main_work)
{
	// テクスチャ解放
	for (int i = 0; i < DME_RANK_DATA_TYPE_MAX; i++) {
		AoTexRelease(&main_work->tex[i]);
	}
	
	// メニュー共通テクスチャ解放
	for (int i = 0; i < 5; i++) {
		AoTexRelease(&main_work->cmn_tex[i]);
	}
	
	if (!dm_rank_is_pause_maingame) {
//		DmSoundFlush();
	}
	
	// 次へ遷移
	main_work->proc_menu_update = dmRankProcFinish;
}


// ==========================================================================
// dmRankProcFinish
/*!
	終了処理
 */
// ==========================================================================
void dmRankProcFinish(DMS_RANK_MAIN_WORK *main_work)
{
	// 画面表示
//	amPrint(4, 4, "NOW FINISHING...");

	// テクスチャ解放完了判定
	if (dmRankIsTexRelease(main_work) == 1) {
		for (int i = 0; i < ACT_NUM; i++) {
			if (main_work->act[i]) {
				AoActDelete(main_work->act[i]);
				main_work->act[i] = NULL;
			}
		}
		
		// アクション解放
		for (int i = 0; i < DME_RANK_DATA_TYPE_MAX; i++) {
			// ファイル解放
			if (main_work->arc_amb[i]) {
				amMemFree(main_work->arc_amb[i]);
				main_work->arc_amb[i] = NULL;
			}
		}
		
		// アクション解放
		for (int i = 0; i < 5; i++) {
			// ファイル解放
			if (main_work->arc_cmn_amb[i]) {
				amMemFree(main_work->arc_cmn_amb[i]);
				main_work->arc_cmn_amb[i] = NULL;
			}
		}
		
		// ランキングリストのユーザー名保存領域解放
		for (int i = 0; i < DMD_RANK_DISP_RANKING_NUM; i++) {
			if (main_work->disp_name[i]) {
				amMemFree(main_work->disp_name[i]);
				main_work->disp_name[i] = NULL;
			}
		}
		
		// ランキングリストデータの一時保存用メモリの解放処理
		dmRankReleaseFreeRankTmpStrg(main_work);
		
		// 終了処理へ
//		main_work->flag |= DMD_RANK_FLAG_EXIT;
		main_work->proc_win_update = NULL;
		main_work->proc_menu_update = dmRankProcWaitFinished;
	}
}



// ==========================================================================
// dmRankProcWaitFinished
/*!
	終了処理
 */
// ==========================================================================
void dmRankProcWaitFinished(DMS_RANK_MAIN_WORK *main_work)
{
	// サウンド終了フラグONならば
	if (main_work->is_snd_exit) {
		// サウンド終了待ち
		if (DmSndBgmPlayerIsTaskExit()) {
			// 終了処理へ
			main_work->flag |= DMD_RANK_FLAG_EXIT;
			main_work->proc_win_update = NULL;
			main_work->proc_menu_update = NULL;
			
			main_work->is_snd_exit = FALSE;
		}
	}
	else {
		// 終了処理へ
		main_work->flag |= DMD_RANK_FLAG_EXIT;
		main_work->proc_win_update = NULL;
		main_work->proc_menu_update = NULL;
	}
}



// ==========================================================================
// dmRankInputProcRankMenu
/*!
	ランクメニュー用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmRankInputProcRankMenu(DMS_RANK_MAIN_WORK *main_work)
{
	u32 stage_dst = 0;
	
	stage_dst = dm_rank_zone_act_num_tbl[main_work->cur_zone][1];
	
	// キャンセル処理
	if (AoPadStand() & GSD_KEY_CANCEL) {
		main_work->flag |= DMD_RANK_FLAG_CANCEL;

		return;
	}
	
	// 決定処理
	if (AoPadStand() & GSD_KEY_DECIDE) {
		main_work->flag |= DMD_RANK_FLAG_DECIDE;

		return;
	}
	
	// 十字キー操作
	if (AoPadMRepeat() & GSD_KEY_UP) {
		if (AoPadMStand() & GSD_KEY_UP
			|| main_work->cur_zone != 0) {
			// 選択ZONE切り替え
			main_work->flag |= DMD_RANK_FLAG_MENU_UP_INPUT;
		}
	}
	else if (AoPadMRepeat() & GSD_KEY_DOWN) {
		if (AoPadMStand() & GSD_KEY_DOWN
			|| main_work->cur_zone != 6 - 1) {
			// 選択ZONE切り替え
			main_work->flag |= DMD_RANK_FLAG_MENU_DOWN_INPUT;
		}
	}
	else if (AoPadMRepeat() & GSD_KEY_LEFT) {
		if (AoPadMStand() & GSD_KEY_LEFT
			|| main_work->cur_stage != 0) {
			// 選択STAGE切り替え
			main_work->flag |= DMD_RANK_FLAG_MENU_LEFT_INPUT;
		}
	}
	else if (AoPadMRepeat() & GSD_KEY_RIGHT) {
		if (AoPadMStand() & GSD_KEY_RIGHT
			|| main_work->cur_stage != (u32)(stage_dst - 1)) {
			// 選択STAGE切り替え
			main_work->flag |= DMD_RANK_FLAG_MENU_RIGHT_INPUT;
		}
	}

}



// ==========================================================================
// dmRankInputProcRankView
/*!
	ランキング閲覧用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmRankInputProcRankView(DMS_RANK_MAIN_WORK *main_work)
{
	// キャンセル処理
	if (AoPadStand() & GSD_KEY_CANCEL) {
		main_work->flag |= DMD_RANK_FLAG_CANCEL;

		return;
	}
	
	// ステージ決定処理
	if (AoPadStand() & GSD_KEY_DECIDE) {
		main_work->flag |= DMD_RANK_FLAG_DECIDE;

		return;
	}

#if !_WII
	// タイム⇔スコア切り替え		// WIIのみボタンを変えるようにする
	if (AoPadStand() & KEY_R_LEFT) {
		main_work->flag |= DMD_RANK_FLAG_RANK_CHNG_TIME;

		return;
	}

	// ソニック状態切り替え
	if (AoPadStand() & KEY_R_UP) {
		if (!dmRankIsSonicOnlyStage(main_work)) {
//		if (main_work->cur_zone != DME_RANK_ZONE_TYPE_SPECIAL) {
			main_work->flag |= DMD_RANK_FLAG_RANK_CHNG_SONIC;
		}

		return;
	}

	// ステージ切り替え
	if (AoPadStand() & KEY_L1) {
		main_work->flag |= DMD_RANK_FLAG_RANK_CHNG_ACT_LEFT;
		
		main_work->btn_l_disp_frm = 0;
		
		return;
	}

	// ステージ切り替え
	if (AoPadStand() & KEY_R1) {
		main_work->flag |= DMD_RANK_FLAG_RANK_CHNG_ACT_RIGHT;
		
		main_work->btn_r_disp_frm = 0;
		
		return;
	}
#endif // #if !_WII

	// 十字キー操作
	if (AoPadMRepeat() & GSD_KEY_UP) {
		if (AoPadMStand() & GSD_KEY_UP
			|| main_work->cur_slct_data != 0) {
			main_work->flag |= DMD_RANK_FLAG_RANK_CHNG_RANK_UP;
		}

		return;
	}
	
	else if (AoPadMRepeat() & GSD_KEY_DOWN) {
		if (AoPadMStand() & GSD_KEY_DOWN
			|| main_work->cur_slct_data != 10 - 1) {
			main_work->flag |= DMD_RANK_FLAG_RANK_CHNG_RANK_DOWN;
		}

		return;
	}

	
	else if (AoPadMStand() & GSD_KEY_LEFT) {
		main_work->flag |= DMD_RANK_FLAG_RANK_CHNG_TAB_LEFT;

		return;
	}
		
	else if (AoPadMStand() & GSD_KEY_RIGHT) {
		main_work->flag |= DMD_RANK_FLAG_RANK_CHNG_TAB_RIGHT;

		return;
	}
}



// ==========================================================================
// dmRankInputProcWinDoYouUpload
/*!
	ウインドウ非表示用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmRankInputProcWinDoYouUpload(DMS_RANK_MAIN_WORK *main_work)
{
	// キャンセル判定
	if (AoPadStand() & GSD_KEY_CANCEL) {
		// フラグON
		main_work->flag |= DMD_RANK_FLAG_CANCEL;
	}
	
	// 決定判定
	if (AoPadStand() & GSD_KEY_DECIDE) {
		// フラグON
		main_work->flag |= DMD_RANK_FLAG_DECIDE;
	}
	
	// 十字キー操作
	if (AoPadMRepeat() & GSD_KEY_LEFT) {
		if (main_work->win_cur_slct != 0) {
			// 選択ZONE切り替え
			main_work->win_cur_slct = 0;
			
			// カーソルSE再生
			if (!dm_rank_is_pause_maingame) {
				DmSoundPlaySE("Cursol");
			}
			else {
				GsSoundPlaySe("Cursol", main_work->se_handle);
			}
		}
	}
	else if (AoPadMRepeat() & GSD_KEY_RIGHT) {
		if (main_work->win_cur_slct != 1) {
			// 選択ZONE切り替え
			main_work->win_cur_slct = 1;
			
			// カーソルSE再生
			if (!dm_rank_is_pause_maingame) {
				DmSoundPlaySE("Cursol");
			}
			else {
				GsSoundPlaySe("Cursol", main_work->se_handle);
			}
		}
	}
}



#if _WII || _PS3 || _XBOX
// ==========================================================================
// dmRankInputProcWinUploadCancel
/*!
	ウインドウ非表示用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmRankInputProcWinUploadCancel(DMS_RANK_MAIN_WORK *main_work)
{
	// キャンセル判定
	if (DmRankSysIsCancelable()
		&& (AoPadStand() & GSD_KEY_CANCEL)) {
		// フラグOFF
		main_work->flag |= DMD_RANK_FLAG_CANCEL;
	}
}
#endif



// ==========================================================================
// dmRankProcActDraw
/*!
	描画設定プロシージャ処理
 */
// ==========================================================================
void dmRankProcActDraw(DMS_RANK_MAIN_WORK *main_work)
{
	// 帯移動演出用		※常に移動しつづけるのでここに配置
	dmRankSetObiEfctPos(main_work);
	
	// ACTアニメーション制御処理
	dmRankActAnimeControl(main_work);
	
	// 共通描画処理は描画時は常に設定
	dmRankCommonDraw(main_work);
	
	// FIX部分描画設定
	dmRankCommonFixDraw(main_work);
	
	// アニメーションを隠す半透明背景描画設定
//	dmRankHideActTableDraw(main_work);
	
	// ランクメニュー描画設定
	if (main_work->disp_flag & DMD_RANK_DISP_FLAG_RANK_MENU) {
		dmRankRankMenuDraw(main_work);
		
		// アニメーションを隠す半透明背景描画設定
		dmRankHideActTableDraw(main_work);
	}
	
	// ランクビュー描画設定
	if (main_work->disp_flag & DMD_RANK_DISP_FLAG_RANK_VIEW) {
		dmRankRankViewDraw(main_work);
		
		if (main_work->flag & DMD_RANK_FLAG_CHNG_ZONE_WIN_EFCT) {
			dmRankRankSubViewDraw(main_work);
		}
	}
	
	// ランクリスト描画設定
	if (main_work->disp_flag & DMD_RANK_DISP_FLAG_RANK_LIST) {
		dmRankRankListDraw(main_work);
	}
	
	// ランクビュー描画設定
	if (main_work->disp_flag & DMD_RANK_DISP_FLAG_RANK_VIEW) {
		dmRankCmnViewDraw(main_work);
	}
	
	// ウインドウ描画設定
#if _WII
	if (main_work->disp_flag & DMD_RANK_DISP_FLAG_WIN_DRAW
		&& !DmSaveIsDraw()
		&& !(AoSysMsgIsShow())) {
		dmRankWinSelectDraw(main_work);
	}
#elif !_PS3
	if (main_work->disp_flag & DMD_RANK_DISP_FLAG_WIN_DRAW
		&& !DmSaveIsDraw()) {
		dmRankWinSelectDraw(main_work);
	}
#else
	if (main_work->disp_flag & DMD_RANK_DISP_FLAG_WIN_DRAW) {
		dmRankWinSelectDraw(main_work);
	}
#endif
	
	// 描画タスク生成
	if (dm_rank_is_pause_maingame
		&& main_work->draw_state) {
		amDrawMakeTask(dmRankTaskDraw, (u16)0x8000, (u32)0);
	}
}



// ==========================================================================
// dmRankTaskDraw
/*!
	ランキング画面の描画タスク
 */
// ==========================================================================
void dmRankTaskDraw(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	AoActDrawPre();
	amDrawExecCommand(dm_rank_draw_state);
	amDrawEndScene();
}



// ==========================================================================
// dmRankCommonDraw
/*!
	共通描画設定処理
 */
// ==========================================================================
void dmRankCommonDraw(DMS_RANK_MAIN_WORK *main_work)
{
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_RANK_DRAW_PRIO_BG);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[0]));

	// アクション登録
	AoActSortRegAction(main_work->act[ACT_WAVE_BG]);
	
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[0]));
	// アクション更新
	AoActUpdate(main_work->act[ACT_WAVE_BG], 1.0f);

	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();
	
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_RANK_DRAW_PRIO_RIDE_BG);
	
	
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[0]));
	
	// アクション登録
	for (int i = ACT_DOWN_BG; i <= ACT_BLUE_BG; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	
	// アクション登録
	for (int i = ACT_DOWN_BG; i <= ACT_BLUE_BG; i++) {
		AoActUpdate(main_work->act[i], 0.0f);
	}
	
	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();

}



// ==========================================================================
// dmRankCommonFixDraw
/*!
	共通描画設定処理
 */
// ==========================================================================
void dmRankCommonFixDraw(DMS_RANK_MAIN_WORK *main_work)
{
	float tmp_disp_num_dist = 0.f;
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_RANK_DRAW_PRIO_FIX);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	AoActSortRegAction(main_work->act[ACT_TAB_TITLE1]);
	
	// 帯の台部分のみ
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[2]));
	AoActSortRegAction(main_work->act[ACT_OBI_C]);
	
	if (main_work->state == DME_RANK_MODE_STATE_RANK_MAIN) {
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
		
		if (main_work->cur_zone == DME_RANK_ZONE_TYPE_SPECIAL) {
			AoActSortRegAction(main_work->act[ACT_VIEW_ACT_NUM]);
		}
		else if (main_work->cur_zone == DME_RANK_ZONE_TYPE_FINAL) {
			// FINALはBOSS表示のため、数字は非表示
		}
		else {
			if (main_work->cur_stage != 3) {
				AoActSortRegAction(main_work->act[ACT_VIEW_ACT_NUM]);
			}
		}
	}
	else {
	}
	
	// 戻るボタンはウインドウが表示されていないときは常に表示
	if (!main_work->win_size_rate[0] && !main_work->win_size_rate[1]) {
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[1]));
		AoActSortRegAction(main_work->act[ACT_BTN_CANCEL1]);
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[4]));
		AoActSortRegAction(main_work->act[ACT_TEX_BACK_FIX]);
	}
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	// アクション登録
	for (int i = ACT_TEX_TITLE; i <= ACT_TEX_OBI2; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	
	// 半透明の被せ部分
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[2]));
	for (int i = ACT_OBI_L; i <= ACT_OBI_R2; i++) {
		AoActSortRegAction(main_work->act[i]);
	}

	if (main_work->state == DME_RANK_MODE_STATE_RANK_MAIN) {
		
		if (main_work->cur_zone != DME_RANK_ZONE_TYPE_SPECIAL) {
			AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
			AoActSortRegAction(main_work->act[ACT_TEX_LIST_ZONE]);
		}
		else {
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			AoActSortRegAction(main_work->act[ACT_TEX_LIST_ZONE_S]);
		}
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		
		if (main_work->cur_zone == DME_RANK_ZONE_TYPE_SPECIAL) {
			AoActSortRegAction(main_work->act[ACT_TEX_LIST_STAGE]);
		}
		
		else if (main_work->cur_zone == DME_RANK_ZONE_TYPE_FINAL) {
			AoActSortRegAction(main_work->act[ACT_TEX_LIST_BOSS]);
		}
		
		else {
			if (main_work->cur_stage == 3) {
				AoActSortRegAction(main_work->act[ACT_TEX_LIST_BOSS]);
			}
			else {
				AoActSortRegAction(main_work->act[ACT_TEX_LIST_ACT]);
			}
		}
	}
	
	
	// フレーム設定部
	if (main_work->is_jp_region) {
		AoActSetFrame(main_work->act[ACT_BTN_CANCEL1], (f32)0.f);
	}
	else {
		AoActSetFrame(main_work->act[ACT_BTN_CANCEL1], (f32)1.f);
	}

	
	
	if (main_work->state == DME_RANK_MODE_STATE_RANK_MAIN) {
		AoActSetFrame(main_work->act[ACT_VIEW_ACT_NUM], (float)main_work->cur_stage);
		
		AoActSetFrame(main_work->act[ACT_TEX_LIST_ZONE], (float)main_work->cur_zone);
	}
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	AoActUpdate(main_work->act[ACT_TAB_TITLE1], 0.0f);
	
	
	AoActAcmPush();
	
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[2]));
	for (int i = ACT_OBI_C; i <= ACT_OBI_R2; i++) {
		AoActAcmInit();
		AoActAcmApplyTrans(0.f
						   , main_work->obi_pos_y
						   , 0
						   );
		
		AoActUpdate(main_work->act[i], 0.0f);
	}
	
	AoActAcmPop();
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[1]));
	AoActUpdate(main_work->act[ACT_BTN_CANCEL1], 0.0f);
	
	
	AoActAcmPush();
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	if (main_work->cur_zone != DME_RANK_ZONE_TYPE_SPECIAL) {
		tmp_disp_num_dist = DMD_RANK_DISP_ACT_NUM_POS_X;
	}
	else {
		tmp_disp_num_dist = DMD_RANK_DISP_STG_NUM_POS_X;
	}
	
	// 帯テキスト部分
	AoActAcmInit();
	AoActAcmApplyTrans(tmp_disp_num_dist
					   , 0.f
					   , 0.f
					   );
	
//	AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
//					   , main_work->rank_inout_efct_pos[1]
//					   , 0
//					   );
	
	AoActUpdate(main_work->act[ACT_VIEW_ACT_NUM], 0.0f);
	
	AoActAcmPop();
	
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[4]));
	AoActUpdate(main_work->act[ACT_TEX_BACK_FIX], 0.0f);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	// アクション登録
	for (int i = ACT_TEX_TITLE; i < ACT_TEX_OBI1; i++) {
		AoActUpdate(main_work->act[i], 0.0f);
	}
	
	
	
	AoActAcmPush();
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	AoActAcmInit();
//	AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
//					   , main_work->rank_inout_efct_pos[1]
//					   , 0
//					   );
	
	
	AoActUpdate(main_work->act[ACT_TEX_LIST_ZONE], 0.0f);
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	for (int j = ACT_TEX_LIST_STAGE; j <= ACT_TEX_LIST_BOSS; j++) {
		AoActAcmInit();
//		AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
//						   , main_work->rank_inout_efct_pos[1]
//						   , 0
//						   );
		
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
	
	AoActAcmPop();
	
	// ACCUMURATEでトランスさせる
	AoActAcmPush();
	
	for (int i = 0; i < 2; i++) {
		// 帯テキスト部分
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->obi_pos[i]
						   , main_work->obi_pos_y
						   , 0
						   );
		
#if _PS3 || _XBOX || _PC
		AoActAcmApplyScale(DMD_RANKT_OBI_MSG_SCALE_SIZE
						   , DMD_RANKT_OBI_MSG_SCALE_SIZE);
#endif
		
		// フレーム更新はSetFrameのみで行う
		AoActUpdate(main_work->act[ACT_TEX_OBI1 + i], 0.0f);
	}
	
	AoActAcmPop();

	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();
}



// ==========================================================================
// dmRankRankMenuDraw
/*!
	ゾーン選択用描画設定処理
 */
// ==========================================================================
void dmRankRankMenuDraw(DMS_RANK_MAIN_WORK *main_work)
{
	u32 i = 0;
	
	
	// 仮でここで表示位置設定処理
	dmRankSetMenuDispZoneTablePos(main_work);
	
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_RANK_DRAW_PRIO_ACT_SLCT);

	// ゾーン一つ分のテーブル表示		※ここは将来的にFINALとスペステを隠すかもしれないことを考慮しておく
	for (i = 0; i < DME_RANK_ZONE_TYPE_MAX; i++) {
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
		
		// ゾーンテーブル台紙
		for (u32 j = ACT_MENU_TAB_WIN_1; j <= ACT_MENU_TAB_3; j++) {
			AoActSortRegAction(main_work->act[j]);
		}
		
		// ゾーンテーブルウインドウ(ウインドウ内の影も含む)
		for (u32 j = ACT_MENU_TAB_WIN_1; j <= ACT_MENU_TAB_TONE2; j++) {
			AoActSortRegAction(main_work->act[j]);
		}
		
		// ゾーンテーブル内表示物
		if (main_work->disp_flag & DMD_RANK_DISP_FLAG_MENU_TAB
			&& main_work->cur_zone == i) {
			for (u32 j = ACT_MENU_TAB_STAGE; j <= ACT_MENU_ARROW; j++) {
				AoActSortRegAction(main_work->act[j]);
			}
			
			// ZONE1～4の場合
			if (main_work->cur_zone >= 0
				&& main_work->cur_zone <= DME_RANK_ZONE_TYPE_4) {
				AoActSortRegAction(main_work->act[ACT_MENU_ACT_NUM_1]);
				AoActSortRegAction(main_work->act[ACT_MENU_ACT_NUM_2]);
				AoActSortRegAction(main_work->act[ACT_MENU_ACT_NUM_3]);
			}
			else if (main_work->cur_zone == DME_RANK_ZONE_TYPE_FINAL) {
			}
			else {
				for (u32 k = ACT_MENU_ACT_NUM_1; k <= ACT_MENU_ACT_NUM_7; k++) {
					AoActSortRegAction(main_work->act[k]);
				}
			}
		}
		
		// ZONE名
		if (i != DME_RANK_ZONE_TYPE_SPECIAL) {
			AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
			AoActSortRegAction(main_work->act[ACT_MENU_TEX_ZONE]);
		}
		else {
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			AoActSortRegAction(main_work->act[ACT_MENU_TEX_ZONE_S]);
		}
		
		if (main_work->disp_flag & DMD_RANK_DISP_FLAG_MENU_TAB
			&& main_work->cur_zone == i) {
			AoActSortRegAction(main_work->act[ACT_MENU_TEX_ACT]);
			
			if (main_work->cur_zone >= 0
				&& main_work->cur_zone <= DME_RANK_ZONE_TYPE_4) {
				AoActSortRegAction(main_work->act[ACT_MENU_TAB_TEX_BOSS]);
			}
			
			if (main_work->cur_zone == DME_RANK_ZONE_TYPE_FINAL) {
				AoActSortRegAction(main_work->act[ACT_MENU_TAB_TEX_F_BOSS]);
			}
		}
		
		// ここでiを見て、アクティブ以外は登録
		if (main_work->cur_zone != i) {
			AoActSortRegAction(main_work->act[ACT_MENU_TAB_COVER1]);
			AoActSortRegAction(main_work->act[ACT_MENU_TAB_COVER2]);
			AoActSortRegAction(main_work->act[ACT_MENU_TAB_COVER3]);
		}
		
		
		// ゾーンテーブル台紙
		for (u32 j = ACT_MENU_TAB_WIN_1; j <= ACT_MENU_TAB_3; j++) {
			AoActSetFrame(main_work->act[j], dm_rank_zone_tab_disp_frm_tbl[i]);
		}
		
		
		AoActSetFrame(main_work->act[ACT_MENU_TEX_ACT]
					  , dm_rank_act_tex_disp_frm_tbl[i]);
		
		AoActSetFrame(main_work->act[ACT_MENU_TEX_ZONE], (f32)i);
		
		AoActSetFrame(main_work->act[ACT_MENU_TAB_TEX_F_BOSS], 1.f);
		
		
		for (u32 j = 0; j < dm_rank_zone_act_num_tbl[i][1]; j++) {
			if (main_work->cur_zone != DME_RANK_ZONE_TYPE_SPECIAL) {
				if (j != 3) {
					if (main_work->cur_stage == j) {
						AoActSetFrame(main_work->act[ACT_MENU_ACT_NUM_1 + j], 1.f);
					}
					else {
						AoActSetFrame(main_work->act[ACT_MENU_ACT_NUM_1 + j], 0.f);
					}
				}
				else {
					if (main_work->cur_stage == j) {
						AoActSetFrame(main_work->act[ACT_MENU_TAB_TEX_BOSS], 1.f);
					}
					else {
						AoActSetFrame(main_work->act[ACT_MENU_TAB_TEX_BOSS], 0.f);
					}
				}
			}
			else {
				if (main_work->cur_stage == j) {
					AoActSetFrame(main_work->act[ACT_MENU_ACT_NUM_1 + j], 1.f);
				}
				else {
					AoActSetFrame(main_work->act[ACT_MENU_ACT_NUM_1 + j], 0.f);
				}
			}
			
			if (main_work->cur_stage == j) {
				AoActSetFrame(main_work->act[ACT_MENU_TEX_ZONE], (f32)i);
			}
		}
		
		
		// ACCUMURATEでトランスさせる
		AoActAcmPush();
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
		
		for (u32 j = ACT_MENU_TAB_WIN_1; j <= ACT_MENU_TAB_COVER3; j++) {
			AoActAcmInit();
			
			AoActAcmApplyTrans(0.f
							   , main_work->zone_inout_efct_pos[1]
							   , 0
							   );
			
			AoActAcmApplyTrans(main_work->zone_pos[i][0]
							   , main_work->zone_pos[i][1]
							   , 0
							   );
			
			if (j >= ACT_MENU_TAB_WIN_1 && j <= ACT_MENU_TAB_TONE2) {
				AoActAcmApplyScale(1.f//main_work->zone_tab_scale[i][0]
								   , main_work->zone_tab_scale[i][1]);
			}
			
			
			AoActUpdate(main_work->act[j], 0.0f);
		}
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		
		for (u32 j = ACT_MENU_TEX_ZONE_S; j <= ACT_MENU_TAB_TEX_F_BOSS; j++) {
			AoActAcmInit();
			
			AoActAcmApplyTrans(0.f
							   , main_work->zone_inout_efct_pos[1]
							   , 0
							   );
			
			AoActAcmApplyTrans(main_work->zone_pos[i][0]
							   , main_work->zone_pos[i][1]
							   , 0
							   );
			
			AoActUpdate(main_work->act[j], 0.0f);
		}
		
		
		AoActAcmPop();
		
		// ソート実行
		AoActSortExecute();

		// ソートバッファ描画
		AoActSortDraw();

		// ソートバッファ全解除
		AoActSortUnregAll();
	}
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	// メニューカーソル表示
	AoActSortRegAction(main_work->act[ACT_MENU_TAB_CRSR]);
	
	AoActAcmPush();
	
	AoActAcmInit();
	
	
	AoActAcmApplyScale(dm_rank_menu_crsr_scale_x_tbl[main_work->cur_zone][main_work->cur_stage]
					   , 1.f
					   );
	
	AoActAcmApplyTrans(0.f
					   , main_work->zone_inout_efct_pos[1]
					   , 0
					   );
	
	AoActAcmApplyTrans(dm_rank_menu_crsr_pos_x_tbl[main_work->cur_zone][main_work->cur_stage] + DMD_RANK_DISP_MENU_CRSR_BASE_POS_X
					   , main_work->cur_zone * 60 + DMD_RANK_DISP_MENU_CRSR_BASE_POS_Y
					   , 0
					   );
	
	AoActUpdate(main_work->act[ACT_MENU_TAB_CRSR], 0.f);
	
	AoActAcmPop();
	
	
	// ソート実行
	AoActSortExecute();
	
	// ソートバッファ描画
	AoActSortDraw();
	
	// ソートバッファ全解除
	AoActSortUnregAll();
	
#if 0
	// カーソル描画
//	if (main_work->state == DME_RANK_MODE_STATE_ZONE_SLCT) {
		for (i = 0; i < 3; i++) {
//			AoActSortRegAction(main_work->act[ACT_TAB_ZONE_CURSOR2 + i]);
		}
	
		AoActAcmPush();
			
		AoActAcmInit();
//		AoActAcmApplyTrans(main_work->zone_pos[main_work->cur_zone][0]
//						   , main_work->zone_pos[main_work->cur_zone][1]
//						   , 0
//						   );
		
//		for (i = 0; i < 3; i++) {
//			AoActUpdate(main_work->act[ACT_TAB_ZONE_CURSOR2 + i]);
//		}

		AoActAcmPop();

		// ソート実行
		AoActSortExecute();

		// ソートバッファ描画
		AoActSortDraw();

		// ソートバッファ全解除
		AoActSortUnregAll();
//	}
#endif
}



// ==========================================================================
// dmRankCmnViewDraw
/*!
	ランキング用共通ビュー描画設定処理
 */
// ==========================================================================
void dmRankCmnViewDraw(DMS_RANK_MAIN_WORK *main_work)
{
	s32 tmp_disp_sonic_type = 0;
	
	tmp_disp_sonic_type = main_work->cur_sonic_type + 1;
	
	if (tmp_disp_sonic_type >= DME_RANK_SONIC_TYPE_NUM) {
		tmp_disp_sonic_type = 0;
	}
	
	
	// ランキングのFIX関連
	AoActSysSetDrawTaskPrio(DMD_RANK_DRAW_PRIO_RANKING);
	
	// 言語共通
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	

#if !_WII
	// スペステでは非表示(ソニックのみ使用可のため)
	if (!dmRankIsSonicOnlyStage(main_work)) {
//	if (main_work->cur_zone != DME_RANK_ZONE_TYPE_SPECIAL) {
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
		for (int i = ACT_VIEW_ICON_SONIC1; i <= ACT_VIEW_ICON_SONIC1; i++) {
			AoActSortRegAction(main_work->act[i]);
		}
		
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[1]));
		AoActSortRegAction(main_work->act[ACT_BTN_SONIC]);
	}
	
	AoActSortRegAction(main_work->act[ACT_BTN_MODE]);		// ???
	AoActSortRegAction(main_work->act[ACT_TEX_TIMERANK]);
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	for (int i = ACT_VIEW_CRSR_UP; i <= ACT_VIEW_BTN_RIGHT; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	
	AoActSortRegAction(main_work->act[ACT_VIEW_TAB_CRSR1_L]);
	AoActSortRegAction(main_work->act[ACT_VIEW_TAB_CRSR1_C]);
	AoActSortRegAction(main_work->act[ACT_VIEW_TAB_CRSR1_R]);
	
#else
	AoActSortRegAction(main_work->act[ACT_VIEW_TAB_CRSR1_L]);
	AoActSortRegAction(main_work->act[ACT_VIEW_TAB_CRSR1_C]);
	AoActSortRegAction(main_work->act[ACT_VIEW_TAB_CRSR1_R]);
#endif
	
	AoActSortRegAction(main_work->act[ACT_VIEW_TITLE_LINE_L]);
	AoActSortRegAction(main_work->act[ACT_VIEW_TITLE_LINE_R]);
	
#if !_WII
	if (main_work->cur_game_mode != DME_RANK_DISP_RANK_TIME) {
		AoActSortRegAction(main_work->act[ACT_VIEW_MODE_LINE_L]);
		AoActSortRegAction(main_work->act[ACT_VIEW_MODE_LINE_C]);
		AoActSortRegAction(main_work->act[ACT_VIEW_MODE_LINE_R]);
		
		AoActSetFrame(main_work->act[ACT_VIEW_MODE_LINE_L], (f32)main_work->mode_tex_efct_frm);
		AoActSetFrame(main_work->act[ACT_VIEW_MODE_LINE_C], (f32)main_work->mode_tex_efct_frm);
		AoActSetFrame(main_work->act[ACT_VIEW_MODE_LINE_R], (f32)main_work->mode_tex_efct_frm);
	}
	else {
		AoActSortRegAction(main_work->act[ACT_VIEW_MODE_LINE_L2]);
		AoActSortRegAction(main_work->act[ACT_VIEW_MODE_LINE_C2]);
		AoActSortRegAction(main_work->act[ACT_VIEW_MODE_LINE_R2]);
		
		AoActSetFrame(main_work->act[ACT_VIEW_MODE_LINE_L2], (f32)main_work->mode_tex_efct_frm);
		AoActSetFrame(main_work->act[ACT_VIEW_MODE_LINE_C2], (f32)main_work->mode_tex_efct_frm);
		AoActSetFrame(main_work->act[ACT_VIEW_MODE_LINE_R2], (f32)main_work->mode_tex_efct_frm);
	}

	// ソニックとスーパーソニック混合の場合
	if (!dmRankIsSonicOnlyStage(main_work)) {
//	if (main_work->cur_zone != DME_RANK_ZONE_TYPE_SPECIAL) {
		if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_ALL) {
			AoActSortRegAction(main_work->act[ACT_VIEW_ICON_SONIC1]);
			AoActSortRegAction(main_work->act[ACT_VIEW_ICON_SONIC2]);
		}
		else {
			AoActSortRegAction(main_work->act[ACT_VIEW_ICON_SONIC1]);
		}
	}
#endif
	
	
	// 言語共通
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
#if !_WII
	// ソニックテキストはスペステでは表示しない
	if (!dmRankIsSonicOnlyStage(main_work)) {
//	if (main_work->cur_zone != DME_RANK_ZONE_TYPE_SPECIAL) {
		if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_ALL) {
			AoActSortRegAction(main_work->act[ACT_VIEW_TEX_SONIC1]);
		}
		else if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_NORMAL) {
			AoActSortRegAction(main_work->act[ACT_VIEW_TEX_SONIC2]);
		}
		else {
			AoActSortRegAction(main_work->act[ACT_VIEW_TEX_SONIC3]);
		}
	}
#else
#endif
	
	
#if !_WII
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
	if (main_work->cur_game_mode == DME_RANK_DISP_RANK_TIME) {
		AoActSortRegAction(main_work->act[ACT_VIEW_TEX_BIG_TIME]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TEX_TIME_EFCT]);
	}
	else {
		AoActSortRegAction(main_work->act[ACT_VIEW_TEX_BIG_SCORE]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TEX_SCORE_EFCT]);
	}
	
	if (main_work->disp_flag & DMD_RANK_DISP_FLAG_NOW_UPDATE) {
//		AoActSortRegAction(main_work->act[ACT_LIST_MSG_UPDATE]);
	}
	
	if (main_work->disp_flag & DMD_RANK_DISP_FLAG_NOW_UPDATE) {
//		AoActSortRegAction(main_work->act[ACT_LIST_ICON_PROD]);
	}
#endif
	
	AoActSetFrame(main_work->act[ACT_VIEW_BTN_LEFT], (f32)main_work->btn_l_disp_frm);
	AoActSetFrame(main_work->act[ACT_VIEW_BTN_RIGHT], (f32)main_work->btn_r_disp_frm);
	
#if !_WII
//	AoActSetFrame(main_work->act[ACT_VIEW_TEX_SONIC1], (f32)tmp_disp_sonic_type);

	if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_ALL) {
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC1], 0.f);
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC2], 1.f);
		
		main_work->sonic_icon_pos[0][0] = -8.f;
		main_work->sonic_icon_pos[0][1] = -8.f;
		main_work->sonic_icon_pos[1][0] = 8.f;
		main_work->sonic_icon_pos[1][1] = 8.f;
	}
	else if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_NORMAL) {
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC1], 0.f);
		
		
		main_work->sonic_icon_pos[0][0] = 0.f;
		main_work->sonic_icon_pos[0][1] = 0.f;
	}
	else {
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC1], 1.f);
		
		
		main_work->sonic_icon_pos[0][0] = 0.f;
		main_work->sonic_icon_pos[0][1] = 0.f;
	}
	
	
	if (main_work->cur_game_mode == DME_RANK_DISP_RANK_TIME) {
		AoActSetFrame(main_work->act[ACT_TEX_TIMERANK], 1.f);
	}
	else {
		AoActSetFrame(main_work->act[ACT_TEX_TIMERANK], 0.f);
	}
	
#endif
	
	if (main_work->cur_game_mode == DME_RANK_DISP_RANK_TIME) {
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_TIME_EFCT]
					  , (float)main_work->mode_tex_efct_frm);
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_BIG_TIME]
					  , (float)main_work->mode_tex_efct_frm);
	}
	else {
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_SCORE_EFCT]
					  , (float)main_work->mode_tex_efct_frm);
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_BIG_SCORE]
					  , (float)main_work->mode_tex_efct_frm);
	}
	
	if (main_work->mode_tex_efct_frm < 100) {
		main_work->mode_tex_efct_frm++;
	}
	
	if (main_work->flag & DMD_RANK_FLAG_CHNG_GAME_MODE_EFCT) {
		main_work->mode_tex_efct_frm = 0;
		
		main_work->flag &= ~DMD_RANK_FLAG_CHNG_GAME_MODE_EFCT;
	}
	
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	AoActUpdate(main_work->act[ACT_VIEW_TITLE_LINE_L], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_TITLE_LINE_R], 0.f);
	
#if !_WII
	AoActUpdate(main_work->act[ACT_VIEW_MODE_LINE_L], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_MODE_LINE_C], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_MODE_LINE_R], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_MODE_LINE_L2], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_MODE_LINE_C2], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_MODE_LINE_R2], 0.f);

	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	// 下側に表示するゲームモードテキスト
	AoActUpdate(main_work->act[ACT_VIEW_TEX_BIG_TIME], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_TEX_BIG_SCORE], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_TEX_TIME_EFCT], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_TEX_SCORE_EFCT], 0.f);
#endif
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	// ACCUMURATEで全体をトランスさせる
	AoActAcmPush();
	
#if !_WII
	for (int j = ACT_VIEW_CRSR_UP; j <= ACT_VIEW_CRSR_DOWN; j++) {
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
						   , main_work->rank_inout_efct_pos[1]
						   , 0
						   );
		
		AoActUpdate(main_work->act[j], 1.0f);
	}
	
	for (u32 j = ACT_VIEW_BTN_LEFT; j <= ACT_VIEW_BTN_RIGHT; j++) {
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
						   , main_work->rank_inout_efct_pos[1]
						   , 0
						   );
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
#endif
	
	
#if !_WII
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[1]));
	AoActAcmInit();
	AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
					   , main_work->rank_inout_efct_pos[1]
					   , 0
					   );
	
	AoActUpdate(main_work->act[ACT_BTN_MODE], 0.0f);
	AoActUpdate(main_work->act[ACT_BTN_SONIC], 0.0f);
	
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	for (u32 j = ACT_VIEW_ICON_SONIC1; j <= ACT_VIEW_ICON_SONIC2; j++) {
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
						   , main_work->rank_inout_efct_pos[1]
						   , 0
						   );
		
		AoActAcmApplyTrans(main_work->sonic_icon_pos[j - ACT_VIEW_ICON_SONIC1][0]
						   , main_work->sonic_icon_pos[j - ACT_VIEW_ICON_SONIC1][1]
						   , 0
						   );
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
	
#else
#endif
	
	AoActAcmPop();
	
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
	AoActAcmPush();
	
	
	AoActAcmInit();
	AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
					   , main_work->rank_inout_efct_pos[1]
					   , 0
					   );
	
	AoActUpdate(main_work->act[ACT_TEX_TIMERANK], 0.0f);
	
	
	
#if !_WII
	for (int j = ACT_VIEW_TEX_SONIC1; j <= ACT_VIEW_TEX_SONIC3; j++) {
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
						   , main_work->rank_inout_efct_pos[1]
						   , 0
						   );
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
#endif
	
	AoActAcmPop();
	
	
	// 以下のカーソルは他の表示物とは別扱い
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	AoActAcmPush();
	
	// カーソル部分
	AoActAcmInit();
	AoActAcmApplyTrans(0.f//main_work->rank_crsr_pos[0]
					   , main_work->rank_crsr_pos[1]
					   , 0
					   );
	
	AoActUpdate(main_work->act[ACT_VIEW_TAB_CRSR1_L], 1.0f);
	AoActUpdate(main_work->act[ACT_VIEW_TAB_CRSR1_C], 1.0f);
	AoActUpdate(main_work->act[ACT_VIEW_TAB_CRSR1_R], 1.0f);
	
	AoActAcmPop();
	
	
	
	// ソート実行
	AoActSortExecute();
	
	// ソートバッファ描画
	AoActSortDraw();
	
	// ソートバッファ全解除
	AoActSortUnregAll();
}



// ==========================================================================
// dmRankRankViewDraw
/*!
	ランキング用描画設定処理
 */
// ==========================================================================
void dmRankRankViewDraw(DMS_RANK_MAIN_WORK *main_work)
{
	float tab_disp_pos[3] = {0.f, 0.f, 0.f};
	int tmp_dst = 0;
	s32 tmp_disp_sonic_type = 0;
	
	if (!dmRankIsSonicOnlyStage(main_work)) {
		tmp_disp_sonic_type = main_work->cur_sonic_type;
	}
	else {
		tmp_disp_sonic_type = DME_RANK_SONIC_TYPE_NORMAL;
	}
	
	// ランキングのFIX関連
	AoActSysSetDrawTaskPrio(DMD_RANK_DRAW_PRIO_RANKING);
	
	// 言語共通
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	

#if !_WII
	if (main_work->cur_slct_tab == 0) {
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_FRIENDS]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_MYSCORE]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_ALL]);
	}
	else if (main_work->cur_slct_tab == 1) {
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_FRIENDS]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_ALL]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_MYSCORE]);
	}
	else {
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_ALL]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_MYSCORE]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_FRIENDS]);
	}
	
	for (int i = ACT_VIEW_TAB_INDEX; i <= ACT_VIEW_TAB_BOTTOM; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	
#else
	if (main_work->cur_slct_tab == 0) {
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_MYSCORE]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_ALL]);
	}
	else {
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_ALL]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_MYSCORE]);
	}
	
	for (int i = ACT_VIEW_TAB_INDEX; i <= ACT_VIEW_TAB_BOTTOM; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	
#endif
	
	// 自分の順位を示す台紙の表示ON/OFF
	if (main_work->my_rank_data < 10
		&& main_work->disp_flag & DMD_RANK_DISP_FLAG_RANK_LIST) {
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_CRSR2]);
//		main_work->rank_myscore_pos = i;
	}
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_ALL
		|| tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_NORMAL) {
		AoActSortRegAction(main_work->act[ACT_VIEW_ICON_SONIC_INFO1]);
		
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC_INFO1]
					  , (f32)main_work->sonic_icon_efct_frm);
	}
	
	if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_ALL
		|| tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_SUPER) {
		AoActSortRegAction(main_work->act[ACT_VIEW_ICON_SONIC_INFO2]);
		
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC_INFO2]
					  , (f32)main_work->sonic_icon_efct_frm);
	}
	
	// 言語共通
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
#if !_WII
	for (int i = ACT_VIEW_TEX_ALL; i <= ACT_VIEW_TEX_AREA; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	
#else
	
	for (int i = ACT_VIEW_TEX_ALL; i <= ACT_VIEW_TEX_MYSCORE; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	for (int i = ACT_VIEW_TEX_RANK; i <= ACT_VIEW_TEX_AREA; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
#endif
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	// 見出しアニメーションアクション
	for (int i = ACT_VIEW_LIGHT_SCORE; i <= ACT_VIEW_LIGHT_SONIC; i++) {
		AoActSortRegAction(main_work->act[i]);
		
		AoActSetFrame(main_work->act[i], (f32)main_work->index_act_anime_frm[i - ACT_VIEW_LIGHT_SCORE]);
	}
	
	
	AoActSetFrame(main_work->act[ACT_VIEW_TAB_INDEX], (f32)main_work->cur_slct_tab);
	AoActSetFrame(main_work->act[ACT_VIEW_TAB_BOTTOM], (f32)main_work->cur_slct_tab);
	
	for (int i = ACT_VIEW_TEX_RANK; i <= ACT_VIEW_TEX_AREA; i++) {
		if (i != ACT_VIEW_TEX_TIME) {
			AoActSetFrame(main_work->act[i], (f32)main_work->cur_slct_tab);
		}
	}
	
	// フレーム設定
	if (main_work->cur_game_mode == DME_RANK_DISP_RANK_TIME) {
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_TIME], (f32)(0 + main_work->cur_slct_tab));
	}
	else {
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_TIME], (f32)(3 + main_work->cur_slct_tab));
	}
	
#if !_WII
//	AoActSetFrame(main_work->act[ACT_VIEW_TEX_SONIC1], (f32)tmp_disp_sonic_type);

	if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_ALL) {
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC1], 0.f);
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC2], 1.f);
		
		main_work->sonic_icon_pos[0][0] = -8.f;
		main_work->sonic_icon_pos[0][1] = -8.f;
		main_work->sonic_icon_pos[1][0] = 8.f;
		main_work->sonic_icon_pos[1][1] = 8.f;
	}
	else if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_NORMAL) {
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC1], 0.f);
		
		
		main_work->sonic_icon_pos[0][0] = 0.f;
		main_work->sonic_icon_pos[0][1] = 0.f;
	}
	else {
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC1], 1.f);
		
		
		main_work->sonic_icon_pos[0][0] = 0.f;
		main_work->sonic_icon_pos[0][1] = 0.f;
	}
	
	
//	if (main_work->cur_game_mode == DME_RANK_DISP_RANK_TIME) {
//		AoActSetFrame(main_work->act[ACT_TEX_TIMERANK], 1.f);
//	}
//	else {
//		AoActSetFrame(main_work->act[ACT_TEX_TIMERANK], 0.f);
//	}
	
#else
	main_work->sonic_icon_pos[0][0] = -8.f;
	main_work->sonic_icon_pos[0][1] = -8.f;
	main_work->sonic_icon_pos[1][0] = 8.f;
	main_work->sonic_icon_pos[1][1] = 8.f;
#endif
	
	if (main_work->cur_game_mode == DME_RANK_DISP_RANK_TIME) {
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_TIME_EFCT]
					  , (float)main_work->mode_tex_efct_frm);
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_BIG_TIME]
					  , (float)main_work->mode_tex_efct_frm);
	}
	else {
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_SCORE_EFCT]
					  , (float)main_work->mode_tex_efct_frm);
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_BIG_SCORE]
					  , (float)main_work->mode_tex_efct_frm);
	}
	
	
#if !_WII
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	// 下側に表示するゲームモードテキスト
	AoActUpdate(main_work->act[ACT_VIEW_TEX_BIG_TIME], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_TEX_BIG_SCORE], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_TEX_TIME_EFCT], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_TEX_SCORE_EFCT], 0.f);
#endif
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	// ACCUMURATEで全体をトランスさせる
	AoActAcmPush();
	
#if !_WII
	tmp_dst = ACT_VIEW_TAB_FRIENDS;
#else
	tmp_dst = ACT_VIEW_TAB_MYSCORE;
#endif
	
	for (int j = ACT_VIEW_TAB_ALL; j <= tmp_dst; j++) {
		
		if ((u32)(j - ACT_VIEW_TAB_ALL) == main_work->cur_slct_tab) {
			tab_disp_pos[0] = 0.f;
			tab_disp_pos[1] = -8.f;
			tab_disp_pos[2] = -5.f;
		}
		else {
			tab_disp_pos[0] = 0.f;
			tab_disp_pos[1] = 0.f;
			tab_disp_pos[2] = 0.f;
		}
		
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
						   , main_work->rank_inout_efct_pos[1]
						   , 0
						   );
		
		AoActAcmApplyTrans(main_work->rank_win_main_move_pos
						   , 0
						   , 0
						   );
		
		AoActAcmApplyTrans(tab_disp_pos[0]
						   , tab_disp_pos[1]
						   , tab_disp_pos[2]
						   );
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
	
	
#if !_WII
	tmp_dst = ACT_VIEW_TAB_BOTTOM;//ACT_VIEW_TAB_SONIC;
#else
	tmp_dst = ACT_VIEW_TAB_BOTTOM;
#endif
	
	for (int j = ACT_VIEW_TAB_INDEX; j <= tmp_dst; j++) {
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
						   , main_work->rank_inout_efct_pos[1]
						   , 0
						   );
		
		AoActAcmApplyTrans(main_work->rank_win_main_move_pos
						   , 0
						   , 0
						   );
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
	
	
#if !_WII
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	for (u32 j = ACT_VIEW_ICON_SONIC_INFO1; j <= ACT_VIEW_ICON_SONIC_INFO2; j++) {
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
						   , main_work->rank_inout_efct_pos[1]
						   , 0
						   );
		
		AoActAcmApplyTrans(main_work->rank_win_main_move_pos
						   , 0
						   , 0
						   );
		
		if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_ALL) {
			AoActAcmApplyTrans(main_work->sonic_icon_pos[j - ACT_VIEW_ICON_SONIC_INFO1][0] / 2
							   , main_work->sonic_icon_pos[j - ACT_VIEW_ICON_SONIC_INFO1][1] / 2
							   , 0
							   );
		}
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
#else
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	for (u32 j = ACT_VIEW_ICON_SONIC_INFO1; j <= ACT_VIEW_ICON_SONIC_INFO2; j++) {
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
						   , main_work->rank_inout_efct_pos[1]
						   , 0
						   );
		
		AoActAcmApplyTrans(main_work->rank_win_main_move_pos
						   , 0
						   , 0
						   );
		
		AoActAcmApplyTrans(main_work->sonic_icon_pos[j - ACT_VIEW_ICON_SONIC_INFO1][0] / 2
						   , main_work->sonic_icon_pos[j - ACT_VIEW_ICON_SONIC_INFO1][1] / 2
						   , 0
						   );
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
	
	
#endif
	
	AoActAcmPop();
	
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
	AoActAcmPush();
	
	
#if 0
	for (int j = ACT_TEX_LIST_STAGE; j <= ACT_TEX_LIST_BOSS; j++) {
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
						   , main_work->rank_inout_efct_pos[1]
						   , 0
						   );
		
		AoActAcmApplyTrans(main_work->rank_win_main_move_pos
						   , 0
						   , 0
						   );
		
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
#endif
	
	
#if !_WII
	tmp_dst = ACT_VIEW_TEX_FRIENDS;
#else
	tmp_dst = ACT_VIEW_TEX_MYSCORE;
#endif
	
	for (int j = ACT_VIEW_TEX_ALL; j <= tmp_dst; j++) {
		
		if ((u32)(j - ACT_VIEW_TEX_ALL) == main_work->cur_slct_tab) {
			tab_disp_pos[1] = -8.f;
		}
		else {
			tab_disp_pos[1] = 0.f;
		}
		
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
						   , main_work->rank_inout_efct_pos[1]
						   , 0
						   );
		
		AoActAcmApplyTrans(main_work->rank_win_main_move_pos
						   , 0
						   , 0
						   );
		
		AoActAcmApplyTrans(0.f
						   , tab_disp_pos[1]
						   , 0
						   );
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
	
	
	
#if !_WII
	tmp_dst = ACT_VIEW_TEX_AREA;//ACT_VIEW_TEX_SONIC3;
#else
	tmp_dst = ACT_VIEW_TEX_AREA;
#endif
	
	for (int j = ACT_VIEW_TEX_RANK; j <= tmp_dst; j++) {
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_inout_efct_pos[0]
						   , main_work->rank_inout_efct_pos[1]
						   , 0
						   );
		
		
		AoActAcmApplyTrans(main_work->rank_win_main_move_pos
						   , 0
						   , 0
						   );
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
	AoActAcmPop();
	
#if !_WII
//	AoActUpdate(main_work->act[ACT_LIST_MSG_UPDATE], 0.0f);
//	AoActUpdate(main_work->act[ACT_LIST_ICON_PROD], 1.0f);
#endif
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	// 見出しアニメーションアクション
	for (int i = ACT_VIEW_LIGHT_SCORE; i <= ACT_VIEW_LIGHT_SONIC; i++) {
		AoActUpdate(main_work->act[i], 0.f);
	}
	
	AoActAcmPush();
	// 自分のスコア表示用台紙
	AoActAcmInit();
	AoActAcmApplyTrans(0.f
					   , main_work->my_rank_data * 32.f
					   , 0
					   );
	
	AoActUpdate(main_work->act[ACT_VIEW_TAB_CRSR2], 0.0f);
	
	AoActAcmPop();
	
	
	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();
}



// ==========================================================================
// dmRankRankSubViewDraw
/*!
	ランキング用描画設定処理
 */
// ==========================================================================
void dmRankRankSubViewDraw(DMS_RANK_MAIN_WORK *main_work)
{
	float tab_disp_pos[3] = {0.f, 0.f, 0.f};
	int tmp_dst = 0;
	s32 tmp_disp_sonic_type = 0;
	
	if (!dmRankIsSonicOnlyStage(main_work)) {
		tmp_disp_sonic_type = main_work->cur_sonic_type;
	}
	else {
		tmp_disp_sonic_type = DME_RANK_SONIC_TYPE_NORMAL;
	}
	
	
	// ランキングのFIX関連
	
	AoActSysSetDrawTaskPrio(DMD_RANK_DRAW_PRIO_RANKING);
	
	// 言語共通
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	

#if !_WII
	if (main_work->cur_slct_tab == 0) {
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_FRIENDS]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_MYSCORE]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_ALL]);
	}
	else if (main_work->cur_slct_tab == 1) {
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_FRIENDS]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_ALL]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_MYSCORE]);
	}
	else {
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_ALL]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_MYSCORE]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_FRIENDS]);
	}
	
	for (int i = ACT_VIEW_TAB_INDEX; i <= ACT_VIEW_TAB_BOTTOM; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	
#else
	if (main_work->cur_slct_tab == 0) {
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_MYSCORE]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_ALL]);
	}
	else {
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_ALL]);
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_MYSCORE]);
	}
	
	for (int i = ACT_VIEW_TAB_INDEX; i <= ACT_VIEW_TAB_BOTTOM; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	
#endif
	
	// 自分の順位を示す台紙の表示ON/OFF
	if (main_work->my_rank_data < 10
		&& main_work->disp_flag & DMD_RANK_DISP_FLAG_RANK_LIST) {
		AoActSortRegAction(main_work->act[ACT_VIEW_TAB_CRSR2]);
//		main_work->rank_myscore_pos = i;
	}
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_ALL
		|| tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_NORMAL) {
		AoActSortRegAction(main_work->act[ACT_VIEW_ICON_SONIC_INFO1]);
		
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC_INFO1]
					  , (f32)main_work->sonic_icon_efct_frm);
	}
	
	if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_ALL
		|| tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_SUPER) {
		AoActSortRegAction(main_work->act[ACT_VIEW_ICON_SONIC_INFO2]);
		
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC_INFO2]
					  , (f32)main_work->sonic_icon_efct_frm);
	}
	
	// 言語共通
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
#if !_WII
	for (int i = ACT_VIEW_TEX_ALL; i <= ACT_VIEW_TEX_AREA; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	
#else
	
	for (int i = ACT_VIEW_TEX_ALL; i <= ACT_VIEW_TEX_MYSCORE; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	for (int i = ACT_VIEW_TEX_RANK; i <= ACT_VIEW_TEX_AREA; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
#endif
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	// 見出しアニメーションアクション
	for (int i = ACT_VIEW_LIGHT_SCORE; i <= ACT_VIEW_LIGHT_SONIC; i++) {
		AoActSortRegAction(main_work->act[i]);
		
		AoActSetFrame(main_work->act[i], (f32)main_work->index_act_anime_frm[i - ACT_VIEW_LIGHT_SCORE]);
	}
	
	
	AoActSetFrame(main_work->act[ACT_VIEW_TAB_INDEX], (f32)main_work->cur_slct_tab);
	AoActSetFrame(main_work->act[ACT_VIEW_TAB_BOTTOM], (f32)main_work->cur_slct_tab);
	
	for (int i = ACT_VIEW_TEX_RANK; i <= ACT_VIEW_TEX_AREA; i++) {
		if (i != ACT_VIEW_TEX_TIME) {
			AoActSetFrame(main_work->act[i], (f32)main_work->cur_slct_tab);
		}
	}
	
	// フレーム設定
	if (main_work->cur_game_mode == DME_RANK_DISP_RANK_TIME) {
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_TIME], (f32)(0 + main_work->cur_slct_tab));
	}
	else {
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_TIME], (f32)(3 + main_work->cur_slct_tab));
	}
	
#if !_WII
//	AoActSetFrame(main_work->act[ACT_VIEW_TEX_SONIC1], (f32)tmp_disp_sonic_type);

	if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_ALL) {
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC1], 0.f);
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC2], 1.f);
		
		main_work->sonic_icon_pos[0][0] = -8.f;
		main_work->sonic_icon_pos[0][1] = -8.f;
		main_work->sonic_icon_pos[1][0] = 8.f;
		main_work->sonic_icon_pos[1][1] = 8.f;
	}
	else if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_NORMAL) {
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC1], 0.f);
		
		
		main_work->sonic_icon_pos[0][0] = 0.f;
		main_work->sonic_icon_pos[0][1] = 0.f;
	}
	else {
		AoActSetFrame(main_work->act[ACT_VIEW_ICON_SONIC1], 1.f);
		
		
		main_work->sonic_icon_pos[0][0] = 0.f;
		main_work->sonic_icon_pos[0][1] = 0.f;
	}
	
	
//	if (main_work->cur_game_mode == DME_RANK_DISP_RANK_TIME) {
//		AoActSetFrame(main_work->act[ACT_TEX_TIMERANK], 1.f);
//	}
//	else {
//		AoActSetFrame(main_work->act[ACT_TEX_TIMERANK], 0.f);
//	}
	
#endif
	
	if (main_work->cur_game_mode == DME_RANK_DISP_RANK_TIME) {
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_TIME_EFCT]
					  , (float)main_work->mode_tex_efct_frm);
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_BIG_TIME]
					  , (float)main_work->mode_tex_efct_frm);
	}
	else {
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_SCORE_EFCT]
					  , (float)main_work->mode_tex_efct_frm);
		AoActSetFrame(main_work->act[ACT_VIEW_TEX_BIG_SCORE]
					  , (float)main_work->mode_tex_efct_frm);
	}
	
#if !_WII
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	// 下側に表示するゲームモードテキスト
	AoActUpdate(main_work->act[ACT_VIEW_TEX_BIG_TIME], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_TEX_BIG_SCORE], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_TEX_TIME_EFCT], 0.f);
	AoActUpdate(main_work->act[ACT_VIEW_TEX_SCORE_EFCT], 0.f);
#endif
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	// ACCUMURATEで全体をトランスさせる
	AoActAcmPush();
	
#if !_WII
	tmp_dst = ACT_VIEW_TAB_FRIENDS;
#else
	tmp_dst = ACT_VIEW_TAB_MYSCORE;
#endif
	
	for (int j = ACT_VIEW_TAB_ALL; j <= tmp_dst; j++) {
		
		if ((u32)(j - ACT_VIEW_TAB_ALL) == main_work->cur_slct_tab) {
			tab_disp_pos[0] = 0.f;
			tab_disp_pos[1] = -8.f;
			tab_disp_pos[2] = -5.f;
		}
		else {
			tab_disp_pos[0] = 0.f;
			tab_disp_pos[1] = 0.f;
			tab_disp_pos[2] = 0.f;
		}
		
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_win_sub_move_pos
						   , 0
						   , 0
						   );
		
		AoActAcmApplyTrans(tab_disp_pos[0]
						   , tab_disp_pos[1]
						   , tab_disp_pos[2]
						   );
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
	
	
#if !_WII
	tmp_dst = ACT_VIEW_TAB_BOTTOM;//ACT_VIEW_TAB_SONIC;
#else
	tmp_dst = ACT_VIEW_TAB_BOTTOM;
#endif
	
	for (int j = ACT_VIEW_TAB_INDEX; j <= tmp_dst; j++) {
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_win_sub_move_pos
						   , 0
						   , 0
						   );
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
	
	
#if !_WII
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	for (u32 j = ACT_VIEW_ICON_SONIC_INFO1; j <= ACT_VIEW_ICON_SONIC_INFO2; j++) {
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_win_sub_move_pos
						   , 0
						   , 0
						   );
		
		if (tmp_disp_sonic_type == DME_RANK_SONIC_TYPE_ALL) {
			AoActAcmApplyTrans(main_work->sonic_icon_pos[j - ACT_VIEW_ICON_SONIC_INFO1][0] / 2
							   , main_work->sonic_icon_pos[j - ACT_VIEW_ICON_SONIC_INFO1][1] / 2
							   , 0
							   );
		}
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
#else
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	for (u32 j = ACT_VIEW_ICON_SONIC_INFO1; j <= ACT_VIEW_ICON_SONIC_INFO2; j++) {
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_win_sub_move_pos
						   , 0
						   , 0
						   );
		
		AoActAcmApplyTrans(main_work->sonic_icon_pos[j - ACT_VIEW_ICON_SONIC_INFO1][0] / 2
						   , main_work->sonic_icon_pos[j - ACT_VIEW_ICON_SONIC_INFO1][1] / 2
						   , 0
						   );
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
	
	
#endif
	
	AoActAcmPop();
	
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
	AoActAcmPush();
	
	
#if 0
	for (int j = ACT_TEX_LIST_STAGE; j <= ACT_TEX_LIST_BOSS; j++) {
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_win_sub_move_pos
						   , 0
						   , 0
						   );
		
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
#endif
	
	
#if !_WII
	tmp_dst = ACT_VIEW_TEX_FRIENDS;
#else
	tmp_dst = ACT_VIEW_TEX_MYSCORE;
#endif
	
	for (int j = ACT_VIEW_TEX_ALL; j <= tmp_dst; j++) {
		
		if ((u32)(j - ACT_VIEW_TEX_ALL) == main_work->cur_slct_tab) {
			tab_disp_pos[1] = -8.f;
		}
		else {
			tab_disp_pos[1] = 0.f;
		}
		
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->rank_win_sub_move_pos
						   , 0
						   , 0
						   );
		
		AoActAcmApplyTrans(0.f
						   , tab_disp_pos[1]
						   , 0
						   );
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
	
	
	
#if !_WII
	tmp_dst = ACT_VIEW_TEX_AREA;//ACT_VIEW_TEX_SONIC3;
#else
	tmp_dst = ACT_VIEW_TEX_AREA;
#endif
	
	for (int j = ACT_VIEW_TEX_RANK; j <= tmp_dst; j++) {
		AoActAcmInit();
		
		AoActAcmApplyTrans(main_work->rank_win_sub_move_pos
						   , 0
						   , 0
						   );
		
		AoActUpdate(main_work->act[j], 0.0f);
	}
	AoActAcmPop();
	
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	// 見出しアニメーションアクション
	for (int i = ACT_VIEW_LIGHT_SCORE; i <= ACT_VIEW_LIGHT_SONIC; i++) {
		AoActUpdate(main_work->act[i], 0.f);
	}
	
	
#if 0	
	AoActAcmPush();
	// 自分のスコア表示用台紙
	AoActAcmInit();
	AoActAcmApplyTrans(0.f
					   , main_work->my_rank_data * 32.f
					   , 0
					   );
	
	AoActUpdate(main_work->act[ACT_VIEW_TAB_CRSR2], 0.0f);
	
	AoActAcmPop();
#endif
	
	
	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();
}



// ==========================================================================
// dmRankRankListDraw
/*!
	ランキングリスト用描画設定処理
 */
// ==========================================================================
void dmRankRankListDraw(DMS_RANK_MAIN_WORK *main_work)
{
	u16 tmp_min = 0;		// 分
	u16 tmp_sec = 0;		// 秒
	u16 tmp_msec = 0;		// ミリ秒
	u16 tmp_time_calc = 0;
	
	int tmp_digit[DMD_RANK_VIEW_RANK_DIGIT_NUM];		// 各桁の値(要素数は仮)
	int tmp_disp_digit[DMD_RANK_VIEW_SCORE_DIGIT_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	int tmp_digit_calc = 0;
	int tmp_calc_data = 0;
	int tmp_score_digit = 0;
	int tmp_calc_score = 1;
//	u16 tmp_save_val = 0;
	int tmp_digit_cnt = 0;
	BOOL disp_zero_num = FALSE;
	
	amZeroMemory(&tmp_digit, sizeof(int) * DMD_RANK_VIEW_RANK_DIGIT_NUM);
	amZeroMemory(&tmp_disp_digit, sizeof(int) * DMD_RANK_VIEW_SCORE_DIGIT_NUM);
	
	AoActSysSetDrawTaskPrio(DMD_RANK_DRAW_PRIO_RANKING);
	
	for (s32 i = 0; i < main_work->disp_list_num; i++) {
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
		
		tmp_digit_calc = (s32)main_work->disp_rank_no[i];
		tmp_calc_data = DMD_RANK_VIEW_RANK_MAX_NUM;
		tmp_digit_cnt = 0;
		
		while (tmp_calc_data > 0) {
			if (tmp_digit_calc >= tmp_calc_data) {
				AoActSortRegAction(main_work->act[ACT_LIST_RANK_1 + tmp_digit_cnt]);
				tmp_digit[tmp_digit_cnt] = tmp_digit_calc / tmp_calc_data;
				tmp_digit[tmp_digit_cnt] %= 10;
			}
			else {
				tmp_digit[tmp_digit_cnt] = 0;
			}
			
			tmp_digit_cnt++;
			
			if (tmp_calc_data < 10) {
				tmp_calc_data = 0;
			}
			else {
				tmp_calc_data /= 10;
			}
		}
		
		// 名前描画
		for (u32 j = 0; j < DMD_RANK_DISP_LIST_NAME_CHAR_NUM; j++) {	// 1桁目から描画設定
			if (main_work->disp_name[i][j] != '\0') {		// この-1は文字データがないことを示す
				AoActSortRegAction(main_work->act[ACT_LIST_NAME_1 + j]);
			}
			else {
				// ここで文字表示登録をやめる(終端以降は描画させない(不定のため))
				break;
			}
		}
		
		if (main_work->cur_game_mode == DME_RANK_DISP_RANK_TIME) {
			// タイム描画
			for (u32 j = ACT_LIST_TIME_1; j <= ACT_LIST_TIME_5; j++) {
				AoActSortRegAction(main_work->act[j]);
			}
			
			// まずu32の時間の変数を分・秒・ミリ秒に変換
			AkUtilFrame60ToTime(main_work->disp_time[i], &tmp_min, &tmp_sec, &tmp_msec);
		}
		
		else {
			// スコア算出・描画設定
			tmp_score_digit = (int)main_work->disp_score[i];
			tmp_calc_score = 1;
			
			for (int l = 0; l < DMD_RANK_VIEW_SCORE_DIGIT_NUM; l++) {
				tmp_disp_digit[l] = 0;
			}
			
			disp_zero_num = FALSE;
			
			// スコア描画
			for (u32 j = 0; j < DMD_RANK_VIEW_SCORE_DIGIT_NUM; j++) {
				
				if (DMD_RANK_VIEW_SCORE_DIGIT_NUM - j - 1 <= 0) {
					tmp_score_digit = 1;
				}
				else {
					for (u32 k = 0; k < DMD_RANK_VIEW_SCORE_DIGIT_NUM - j - 1; k++) {
						tmp_calc_score = tmp_calc_score * 10;
					}
				}
				
				if (tmp_score_digit >= tmp_calc_score) {
					AoActSortRegAction(main_work->act[ACT_LIST_SCORE_1 + j]);
					if (j >= DMD_RANK_VIEW_SCORE_DIGIT_NUM - 1) {
						tmp_disp_digit[j] = 0;
					}
					else {
						tmp_disp_digit[j] = (s32)(tmp_score_digit / tmp_calc_score);
						tmp_score_digit -= (int)((tmp_disp_digit[j]) * tmp_calc_score - 1);
					}
					
					disp_zero_num = TRUE;
				}
				else {
					if (disp_zero_num) {
						AoActSortRegAction(main_work->act[ACT_LIST_SCORE_1 + j]);
					}
					
					tmp_disp_digit[j] = 0;
				}
				
				tmp_calc_score = 1;
			}
		}
		
		// レコード取得時のソニック状態アイコン描画		※リスト描画時は必ず描画
		AoActSortRegAction(main_work->act[ACT_LIST_ICON_SONIC3]);
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		
		// 地域描画		※リスト描画時は必ず描画
		AoActSortRegAction(main_work->act[ACT_LIST_TEX_COUNTRY]);
		
		// フレーム設定部
		
		// 順位設定
		for (u32 j = 0; j < DMD_RANK_VIEW_RANK_DIGIT_NUM; j++) {
			AoActSetFrame(main_work->act[ACT_LIST_RANK_1 + j], (float)tmp_digit[j]);
		}
		
		// 名前文字設定
		for (u32 j = 0; j < DMD_RANK_DISP_NAMECHAR_NUM; j++) {
			AoActSetFrame(main_work->act[ACT_LIST_NAME_1 + j]
						  , (f32)(main_work->disp_name[i][j] + 1)); // 奥川修正(+1) /////////////////////////////////////////
		}
		
		// タイム値設定
		tmp_time_calc = (u16)(tmp_min % 10);		// 分
		AoActSetFrame(main_work->act[ACT_LIST_TIME_1], tmp_time_calc);
		tmp_time_calc = (u16)(tmp_sec / 10);		// 秒(十の位)
		AoActSetFrame(main_work->act[ACT_LIST_TIME_2], tmp_time_calc);
		tmp_time_calc = (u16)(tmp_sec % 10);		// 秒(一の位)
		AoActSetFrame(main_work->act[ACT_LIST_TIME_3], tmp_time_calc);
		tmp_time_calc = (u16)(tmp_msec / 10);		// ミリ秒(十の位)
		AoActSetFrame(main_work->act[ACT_LIST_TIME_4], tmp_time_calc);
		tmp_time_calc = (u16)(tmp_msec % 10);		// ミリ秒(一の位)
		AoActSetFrame(main_work->act[ACT_LIST_TIME_5], tmp_time_calc);
		
		
		// コロン設定
		AoActSetFrame(main_work->act[ACT_LIST_TIME_CLN_1], 0.f);
		AoActSetFrame(main_work->act[ACT_LIST_TIME_CLN_2], 0.f);
		
		// スコア設定
		for (u32 j = 0; j < DMD_RANK_VIEW_SCORE_DIGIT_NUM; j++) {
			AoActSetFrame(main_work->act[ACT_LIST_SCORE_1 + j]
						  , (f32)tmp_disp_digit[j]);
		}
		
		// 地域設定
		AoActSetFrame(main_work->act[ACT_LIST_TEX_COUNTRY], (f32)main_work->disp_area[i]);
		
		// ソニック状態設定
		AoActSetFrame(main_work->act[ACT_LIST_ICON_SONIC3]
					  , (f32)main_work->disp_sonic[i]);
		
		
		
		// 更新関連設定
		AoActAcmPush();
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
		
		for (u32 j = ACT_LIST_RANK_1; j <= ACT_LIST_ICON_SONIC3; j++) {
			AoActAcmInit();
			AoActAcmApplyTrans(0.f
							   , i * 32.f
							   , 0.f
							   );
			
//			if (j >= ACT_MENU_TAB_WIN_1 && j <= ACT_MENU_TAB_TONE2) {
//				AoActAcmApplyScale(1.f//main_work->zone_tab_scale[i][0]
//								   , main_work->zone_tab_scale[i][1]);
//			}
			
			
			AoActUpdate(main_work->act[j], 0.0f);
		}
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		
		for (u32 j = ACT_LIST_TEX_COUNTRY; j <= ACT_LIST_TEX_COUNTRY; j++) {
			AoActAcmInit();
			AoActAcmApplyTrans(0.f
							   , i * 32.f
							   , 0.f
							   );
			
//			if (j >= ACT_MENU_TAB_WIN_1 && j <= ACT_MENU_TAB_TONE2) {
//				AoActAcmApplyScale(1.f//main_work->zone_tab_scale[i][0]
//								   , main_work->zone_tab_scale[i][1]);
//			}
			
			
			AoActUpdate(main_work->act[j], 0.0f);
		}
		
		
		AoActAcmPop();
		
		// ソート実行
		AoActSortExecute();

		// ソートバッファ描画
		AoActSortDraw();

		// ソートバッファ全解除
		AoActSortUnregAll();
	}
	
}



// ==========================================================================
// dmRankWinSelectDraw
/*!
	ウインドウ用描画設定処理
 */
// ==========================================================================
void dmRankWinSelectDraw(DMS_RANK_MAIN_WORK *main_work)
{
	f32 tmp_win_size[2] = {0.f, 0.f};
//	u32 i = 0;
	
	// ウインドウ用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_RANK_DRAW_PRIO_WIN_FIX);
	
	// 背景描画
#if _WII
	tmp_win_size[0] = DMD_RANK_WINDOW_SIZE_W * DMD_RANK_DISP_SCALE_TEXT;
	tmp_win_size[1] = DMD_RANK_WINDOW_SIZE_H * DMD_RANK_DISP_SCALE_TEXT;
#else
	tmp_win_size[0] = DMD_RANK_WINDOW_SIZE_W;
	tmp_win_size[1] = DMD_RANK_WINDOW_SIZE_H;
#endif
	
	// ウインドウ描画
	if (main_work->draw_state) {
		AoWinSysDrawState(AOD_WIN_TYPE_A
						 , AoTexGetTexList(&main_work->cmn_tex[3])
						 , 0
						 , DMD_RANK_SIZE_WIDTH / 2.0f		// ウインドウ中心X
						 , DMD_RANK_SIZE_HEIGHT / 2.0f		// ウインドウ中心Y
						 , tmp_win_size[0] * main_work->win_size_rate[0]			// ウインドウ横サイズ
						 , tmp_win_size[1] * main_work->win_size_rate[1]			// ウインドウ縦サイズ
						 , dm_rank_draw_state				// 描画STATE
						 );
	}
	else {
		AoWinSysDrawTask(AOD_WIN_TYPE_A
						 , AoTexGetTexList(&main_work->cmn_tex[3])
						 , 0
						 , DMD_RANK_SIZE_WIDTH / 2.0f		// ウインドウ中心X
						 , DMD_RANK_SIZE_HEIGHT / 2.0f		// ウインドウ中心Y
						 , tmp_win_size[0] * main_work->win_size_rate[0]			// ウインドウ横サイズ
						 , tmp_win_size[1] * main_work->win_size_rate[1]			// ウインドウ縦サイズ
						 , DMD_RANK_DRAW_PRIO_WIN			// 描画タスク優先度
						 );
	}
	
	// ウインドウ内の項目描画
	if (main_work->disp_flag & DMD_RANK_DISP_FLAG_WIN_ACT) {		// ウインドウが表示しきっているならば
//		AoActSetTexture(AoTexGetTexList(&main_work->tex[DME_RANK_DATA_TYPE_WIN_CMN_DATA]));

		// 共通ウインドウACT登録
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
//		AoActSortRegAction(main_work->act[ACT_WIN_LINE]);
		
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[4]));
//		AoActSortRegAction(main_work->act[ACT_TEX_WINTITLE]);
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		
		switch (main_work->win_mode) {
		case DME_RANK_WIN_NET_CONNECT:
			// アクション登録
#if _WII
			AoActSortRegAction(main_work->act[ACT_WIN_TEX_MSG0]);
			// 初期表示フレーム設定
			AoActSetFrame(main_work->act[ACT_WIN_TEX_MSG0], 0.f);
#endif
			break;
			
		case DME_RANK_WIN_DO_YOU_REGIST:
			AoActSortRegAction(main_work->act[ACT_WIN_TEX_MSG1]);
			
			AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[4]));
			AoActSortRegAction(main_work->act[ACT_TEX_YES]);
			AoActSortRegAction(main_work->act[ACT_TEX_NO]);

			// 初期表示フレーム設定
			AoActSetFrame(main_work->act[ACT_WIN_TEX_MSG1], 0.f);
			
			if (main_work->win_cur_slct) {
				AoActSetFrame(main_work->act[ACT_TEX_YES], 1.f);
				AoActSetFrame(main_work->act[ACT_TEX_NO], 0.f);
			}
			else {
				AoActSetFrame(main_work->act[ACT_TEX_YES], 0.f);
				AoActSetFrame(main_work->act[ACT_TEX_NO], 1.f);
			}
			
			break;
			
		case DME_RANK_WIN_NOW_REGIST:
			AoActSortRegAction(main_work->act[ACT_WIN_TEX_MSG2]);
			
#if _WII || _PS3 || _XBOX
			if (DmRankSysIsCancelable()) {
				AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[4]));
				AoActSortRegAction(main_work->act[ACT_TEX_BACK_WIN]);
				
				AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[1]));
				AoActSortRegAction(main_work->act[ACT_BTN_CANCEL_WIN]);
				
				if (main_work->is_jp_region) {
					AoActSetFrame(main_work->act[ACT_BTN_CANCEL_WIN], (f32)0.f);
				}
				else {
					AoActSetFrame(main_work->act[ACT_BTN_CANCEL_WIN], (f32)1.f);
				}
			}
#endif
			
			// 初期表示フレーム設定
			AoActSetFrame(main_work->act[ACT_WIN_TEX_MSG2], 0.f);
			
			break;
			
		case DME_RANK_WIN_NOW_UPDATE:
			// アクション登録
			AoActSortRegAction(main_work->act[ACT_WIN_TEX_MSG3]);
			
#if _WII || _PS3 || _XBOX
			if (DmRankSysIsCancelable()) {
				AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[4]));
				AoActSortRegAction(main_work->act[ACT_TEX_BACK_WIN]);
				
				AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[1]));
				AoActSortRegAction(main_work->act[ACT_BTN_CANCEL_WIN]);
				
				if (main_work->is_jp_region) {
					AoActSetFrame(main_work->act[ACT_BTN_CANCEL_WIN], (f32)0.f);
				}
				else {
					AoActSetFrame(main_work->act[ACT_BTN_CANCEL_WIN], (f32)1.f);
				}
			}
#endif
			
			// 初期表示フレーム設定
			AoActSetFrame(main_work->act[ACT_WIN_TEX_MSG3], 0.f);
			
			break;
			
		default:
			// 例外
			MTM_ASSERT(0);
			break;
		}
		
		
		// ACCUMURATEでトランスさせる
		AoActAcmPush();
		
		AoActAcmInit();
		AoActAcmApplyTrans(dm_rank_win_act_pos_tbl[0][0]
						   , dm_rank_win_act_pos_tbl[0][1]
						   , 0
						   );
		
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
		
		// フレーム更新はSetFrameのみで行う
		AoActUpdate(main_work->act[ACT_WIN_LINE], 0.0f);
		
		
		AoActAcmPop();
		
		AoActAcmPush();
		
		for (int i = 0; i < 4; i++) {
			AoActAcmInit();
			
#if _WII
			AoActAcmApplyScale(DMD_RANK_DISP_SCALE_TEXT
							   , DMD_RANK_DISP_SCALE_TEXT);
#endif
			
			AoActAcmApplyTrans(dm_rank_win_act_pos_tbl[i + 1][0]
							   , dm_rank_win_act_pos_tbl[i + 1][1]
							   , 0
							   );
			
			// フレーム更新はSetFrameのみで行う
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			AoActUpdate(main_work->act[ACT_WIN_TEX_MSG0 + i], 0.0f);
		}
		
		for (int i = 0; i < 3; i++) {
			AoActAcmInit();
			
#if _WII
			AoActAcmApplyScale(DMD_RANK_DISP_SCALE_TEXT
							   , DMD_RANK_DISP_SCALE_TEXT);
#endif
			
			AoActAcmApplyTrans(dm_rank_win_act_pos_tbl[i + 5][0]
							   , dm_rank_win_act_pos_tbl[i + 5][1]
							   , 0
							   );
			
			// フレーム更新はSetFrameのみで行う
			AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[4]));
			AoActUpdate(main_work->act[ACT_TEX_WINTITLE + i], 0.0f);
		}
		
		// 戻るボタン・戻るテキスト
		AoActAcmInit();
		AoActAcmApplyTrans(dm_rank_win_act_pos_tbl[10][0]
						   , dm_rank_win_act_pos_tbl[10][1]
						   , 0
						   );
		
		// フレーム更新はSetFrameのみで行う
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[4]));
		AoActUpdate(main_work->act[ACT_TEX_BACK_WIN], 0.0f);
		
		AoActAcmInit();
		AoActAcmApplyTrans(dm_rank_win_act_pos_tbl[9][0]
						   , dm_rank_win_act_pos_tbl[9][1]
						   , 0
						   );
		
		AoActAcmApplyTrans(dm_rank_back_text_length_tbl[GsEnvGetLanguage()]
						   , 0
						   , 0
						   );
		
#if _WII		// Wii版のみ左へ10ピクセルさらにずらせる
		AoActAcmApplyTrans(-10.f, 0, 0);
#endif
		
		// フレーム更新はSetFrameのみで行う
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[1]));
		AoActUpdate(main_work->act[ACT_BTN_CANCEL_WIN], 0.0f);
		
		
		AoActAcmPop();
		
		// ソート実行
		AoActSortExecute();

		// ソートバッファ描画
		AoActSortDraw();

		// ソートバッファ全解除
		AoActSortUnregAll();
	}
}



// ==========================================================================
// dmRankHideActTableDraw
/*!
	ACTテーブルを隠す半透明背景の描画設定処理
 */
// ==========================================================================
void dmRankHideActTableDraw(DMS_RANK_MAIN_WORK *main_work)
{
	AMS_PARAM_DRAW_PRIMITIVE *param = NULL;
	
	param = &main_work->up_bg_vrtx;
	
	param->mtx = NULL;
	param->vtxPCT3D = (NNS_PRIM3D_PCT*)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 6);
	NNS_PRIM3D_PCT* v = param->vtxPCT3D;

	v[0].Pos.x = v[2].Pos.x = v[4].Pos.x = -160.0f;
	v[1].Pos.x = v[3].Pos.x = v[5].Pos.x = v[0].Pos.x + 1280.0f;
	v[0].Pos.y = v[1].Pos.y = 720.0f;
	v[2].Pos.y = v[3].Pos.y = v[0].Pos.y - 64.0f;
	v[4].Pos.y = v[5].Pos.y = v[0].Pos.y - 128.0f;
	v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = v[4].Pos.z = v[5].Pos.z = -2.0f;
	v[0].Col = v[2].Col = AMD_RGBA8888(255, 255, 255, 255);
	v[1].Col = v[3].Col = AMD_RGBA8888(255, 255, 255, 255);
	v[4].Col = v[5].Col = AMD_RGBA8888(255, 255, 255, 0);
	v[0].Tex.u = v[2].Tex.u = v[4].Tex.u = -0.02f + main_work->tex_u[1];
	v[1].Tex.u = v[3].Tex.u = v[5].Tex.u = 19.98f + main_work->tex_u[1];
//	v[0].Tex.v = v[1].Tex.v = 1.005f - main_work->tex_v[1];
//	v[2].Tex.v = v[3].Tex.v = 0.755f - main_work->tex_v[1];
//	v[4].Tex.v = v[5].Tex.v = 0.455f - main_work->tex_v[1];
	v[0].Tex.v = v[1].Tex.v = 0.0f + main_work->tex_v[1];
	v[2].Tex.v = v[3].Tex.v = 0.25f + main_work->tex_v[1];
	v[4].Tex.v = v[5].Tex.v = 0.50f + main_work->tex_v[1];

	param->format3D = NNE_PRIM3D_FMT_PCT;
	param->type = NNE_PRIM_TRIANGLE_STRIP;
	param->count = 6;
	
	param->texlist = AoTexGetTexList(&main_work->cmn_tex[0]);
	param->texId = 2;
	param->ablend = NNE_PRIM_ALPHABLEND_ON;
	param->zOffset = -1.0f;

#if _PC | _XBOX
	param->bldSrc = NNE_BLENDMODE_SRCALPHA;
	param->bldDst = NNE_BLENDMODE_INVSRCALPHA;
	param->bldMode = NNE_BLENDOP_ADD;
#elif _PS3
	param->bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
	param->bldDst = NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA;
	param->bldMode = NND_BLENDOP_PS3_FUNC_ADD;
#elif _WII
	param->bldSrc = GX_BL_SRCALPHA;
	param->bldDst = GX_BL_INVSRCALPHA;
	param->bldMode = GX_BM_BLEND;
#endif
	param->aTest = 0;
	param->zMask = 1;
	param->zTest = 0;
	
	
	AoActDrawCorWide(v, 6, AOD_ACT_CORW_CENTER);

	
	amDrawPrimitive3D(120, param);
	
	// 
	main_work->tex_u[0] -= 0.020f;
	main_work->tex_u[1] -= 0.020f;
	main_work->tex_v[0] -= 0.005f;
	main_work->tex_v[1] -= 0.005f;
	
	
	amDrawMakeTask(dmRankDrawHideActBg, (u16)(0x2800), (u32)0);
}



// ==========================================================================
// dmRankDrawHideActBg
/*!
	ACTテーブルの上に被さる部分の描画設定処理
 */
// ==========================================================================
void dmRankDrawHideActBg(AMS_TCB *tcb_p)
{
	UNREFERENCED_PARAMETER(tcb_p);
	
	// 前処理
	AoActDrawPre();
	amDrawExecCommand(120);

	// シーン描画終了(半透明描画開始)
	amDrawEndScene();
}



// ==========================================================================
// dmRankSetDownloadRankData
/*!
	ダウンロードしたデータを表示変数に設定する処理
 */
// ==========================================================================
void dmRankSetDownloadRankData(DMS_RANK_MAIN_WORK *main_work)
{
	u32 tmp_disp_rank_no = 0;
	char *tmp_disp_name = NULL;
	u32 tmp_disp_time = 0;
	u32 tmp_disp_score = 0;
	u32 tmp_disp_area = 0;
	u32 tmp_disp_sonic = 0;
	u32 tmp_cnt = 0;
	
	// 非表示にする初期化値設定
	main_work->my_rank_data = 10;
	
	// ダウンロードしたデータをワークに格納
	for (u32 i = 0; i < (u32)main_work->disp_list_num; i++) {
		// 一時変数初期化
		tmp_disp_rank_no = 0;
		tmp_disp_time = 0;
		tmp_disp_score = 0;
		tmp_disp_area = 0;
		tmp_disp_sonic = 0;
		tmp_cnt = 0;
		
		// 実際に表示する順位の値(但し、0番からの順位なので、「＋１」で表示すること)
		tmp_disp_rank_no = (u32)(DmRankSysGetShowRankNo(i) + 1);
		
		if (tmp_disp_rank_no >= DMD_RANK_VIEW_RANK_MAX_NUM) {
			tmp_disp_rank_no = DMD_RANK_VIEW_RANK_MAX_NUM;
		}
		
		main_work->disp_rank_no[i] = tmp_disp_rank_no;
		
		
		// ASCIIで返す名前文字列ポインタ(16文字、または終端文字まで)
//		main_work->disp_name[i] = DmRankSysGetShowName(i);
		tmp_disp_name = (char *)DmRankSysGetShowName(i);
		
		while (tmp_disp_name[tmp_cnt] != '\0') {
			// ASCII以外の文字コードが入っていた場合
			if (tmp_disp_name[tmp_cnt] <= (char)0) {
				tmp_disp_name[tmp_cnt] = (char)0x20;	// スペースに設定
			}
			
			tmp_cnt++;
		}
		
		for (int j = 0; j < DMD_RANK_DISP_LIST_NAME_CHAR_NUM; j++) {
			main_work->disp_name[i][j] = (char)tmp_disp_name[j];
		}
		
		
//		amCopyMemory(
//			main_work->disp_name[i],
//			DmRankSysGetShowName(i), 16);
//		main_work->disp_name[i][16] = '\0';
		
		// タイム取得(同じ関数で取得できる)
		tmp_disp_time = DmRankSysGetShowScore(i);
		
		if (tmp_disp_time >= DMD_RANK_VIEW_TIME_MAX_NUM) {
			tmp_disp_time = DMD_RANK_VIEW_TIME_MAX_NUM - 1;
		}
		main_work->disp_time[i] = tmp_disp_time;
		
		// スコア取得
		tmp_disp_score = DmRankSysGetShowScore(i);
		
		if (tmp_disp_score >= DMD_RANK_VIEW_SCORE_MAX_NUM) {
			tmp_disp_score = DMD_RANK_VIEW_SCORE_MAX_NUM - 10;
		}
		main_work->disp_score[i] = tmp_disp_score;
		
		
		// リージョン取得
		tmp_disp_area = DmRankSysGetShowRegion(i);
		
		if (tmp_disp_area >= DMD_RANK_SYS_REGION_NUM) {
			tmp_disp_area = DMD_RANK_SYS_REGION_NUM - 1;
		}
		main_work->disp_area[i] = (s32)tmp_disp_area;

		
		// 通常ソニックかスーパーかを返す
		tmp_disp_sonic = (u32)DmRankSysGetShowSs(i);
		
		if (tmp_disp_sonic >= DMD_RANK_SYS_SS_NUM) {
			tmp_disp_sonic = DMD_RANK_SYS_SS_NUM - 1;
		}
		main_work->disp_sonic[i] = (s32)tmp_disp_sonic;
		
		
		// 自分の記録かどうかを判定する
		if (DmRankSysIsShowOwn(i)) {
			main_work->my_rank_data = i;
		}
//		main_work->disp_rank_no[i] = (u32)DmRankSysIsShowOwn(i);
	}
}



// ==========================================================================
// dmRankIsDataLoad
/*!
	データ読み込み完了チェック処理
 */
// ==========================================================================
s32 dmRankIsDataLoad(DMS_RANK_MAIN_WORK *main_work)
{
	for (int i = 0; i < DME_RANK_DATA_TYPE_MAX; i++) {
		if (!amFsIsComplete(main_work->arc_amb_fs[i])) {
			return 0;
		}
	}
	
	// メニュー共通データ読み込み
	for (int i = 0; i < 5; i++) {
		if (!amFsIsComplete(main_work->arc_cmn_amb_fs[i])) {
			return 0;
		}
	}
	
	return 1;
}


// ==========================================================================
// dmRankIsTexLoad
/*!
	テクスチャ構築完了チェック処理
 */
// ==========================================================================
s32 dmRankIsTexLoad(DMS_RANK_MAIN_WORK *main_work)
{
	
	for (int i = 0; i < DME_RANK_DATA_TYPE_MAX; i++) {
		if (!AoTexIsLoaded(&main_work->tex[i])) {
			// フラグ扱いでON
			return 0;
		}
	}
	
	// メニュー共通データ
	for (int i = 0; i < 5; i++) {
		if (!AoTexIsLoaded(&main_work->cmn_tex[i])) {
			// フラグ扱いでON
			return 0;
		}
	}
	
	if (!GsFontIsBuilded()) {
		return 0;
	}
	
	// メインゲームではサウンドビルドチェックは行わない
	if (!dm_rank_is_pause_maingame) {
		if (!DmSndBgmPlayerIsSndSysBuild()) {
			return 0;
		}
	}
	
	return 1;
}


// ==========================================================================
// dmRankIsTexRelease
/*!
	テクスチャ解放完了チェック処理
 */
// ==========================================================================
s32 dmRankIsTexRelease(DMS_RANK_MAIN_WORK *main_work)
{

	for (int i = 0; i < DME_RANK_DATA_TYPE_MAX; i++) {
		if (!AoTexIsReleased(&main_work->tex[i])) {
			// フラグ扱いでON
			return 0;
		}
	}
	
	// メニュー共通データ
	for (int i = 0; i < 5; i++) {
		if (!AoTexIsReleased(&main_work->cmn_tex[i])) {
			return 0;
		}
	}
	
	return 1;
}



// ==========================================================================
// dmRankSetRankMenuOutEfct
/*!
	ゾーン選択時のゾーンテーブル掃け演出中処理
 */
// ==========================================================================
void dmRankSetRankMenuOutEfct(DMS_RANK_MAIN_WORK *main_work)
{
	float move_dist = 0;
	float distance = 0;

	// 縦移動する場合
	distance = main_work->dst_zone_inout_efct_pos[1]
				- main_work->src_zone_inout_efct_pos[1];
	
	move_dist = distance / DMD_RANK_MENU_TAB_EFCT_TIME;
	
	main_work->zone_inout_efct_pos[1] += move_dist + main_work->tab_efct_timer * 2.0f;
	
	main_work->tab_efct_timer += 1.0f;		// ◆
}




// ==========================================================================
// dmRankIsRankMenuOutEfct
/*!
	ゾーン選択時のゾーンテーブル掃け演出中処理
 */
// ==========================================================================
BOOL dmRankIsRankMenuOutEfct(DMS_RANK_MAIN_WORK *main_work)
{
	float move_direct = 0.f;
	
	move_direct = main_work->dst_zone_inout_efct_pos[1]
					- main_work->src_zone_inout_efct_pos[1];
	
	// 掃け演出終了チェック
	if (main_work->zone_inout_efct_pos[1] >= main_work->dst_zone_inout_efct_pos[1]
		&& move_direct >= 0) {
		main_work->zone_inout_efct_pos[1] = main_work->dst_zone_inout_efct_pos[1];
		
		main_work->tab_efct_timer = 0.0f;
		
		return TRUE;
	}
	else if (main_work->zone_inout_efct_pos[1] <= main_work->dst_zone_inout_efct_pos[1]
		&& move_direct <= 0) {
		main_work->zone_inout_efct_pos[1] = main_work->dst_zone_inout_efct_pos[1];
		
		main_work->tab_efct_timer = 0.0f;
		
		return TRUE;
	}
	
	return FALSE;
}


// ==========================================================================
// dmRankSetRankViewInEfct
/*!
	ランキング時のステージテーブル入り演出中処理
 */
// ==========================================================================
void dmRankSetRankViewInEfct(DMS_RANK_MAIN_WORK *main_work)
{
	float move_dist = 0;
	float distance = 0;

	// 縦移動する場合
	distance = main_work->dst_rank_inout_efct_pos[0]
				- main_work->src_rank_inout_efct_pos[0];
	
	move_dist = distance / DMD_RANK_VIEW_TAB_EFCT_TIME;
	
	main_work->rank_inout_efct_pos[0] += move_dist - main_work->tab_efct_timer * 2.0f;
	
	main_work->tab_efct_timer += 1.0f;		// ◆
}


// ==========================================================================
// dmRankIsRankViewInEfct
/*!
	ランキング時のステージテーブル入り演出中処理
 */
// ==========================================================================
BOOL dmRankIsRankViewInEfct(DMS_RANK_MAIN_WORK *main_work)
{
	float move_direct = 0.f;
	
	move_direct = main_work->dst_rank_inout_efct_pos[0]
					- main_work->src_rank_inout_efct_pos[0];
	
	
	// 掃け演出終了チェック
	if (main_work->rank_inout_efct_pos[0] >= main_work->dst_rank_inout_efct_pos[0]
		&& move_direct >= 0) {
		main_work->rank_inout_efct_pos[0] = main_work->dst_rank_inout_efct_pos[0];
		
		main_work->tab_efct_timer = 0.0f;
		
		return TRUE;
	}
	else if (main_work->rank_inout_efct_pos[0] <= main_work->dst_rank_inout_efct_pos[0]
		&& move_direct <= 0) {
		main_work->rank_inout_efct_pos[0] = main_work->dst_rank_inout_efct_pos[0];
		
		main_work->tab_efct_timer = 0.0f;
		
		return TRUE;
	}
	
	return FALSE;
}



// ==========================================================================
// dmRankSetZoneChangeEfct
/*!
	ランキングビュー時のゾーン切り替え時演出処理
 */
// ==========================================================================
void dmRankSetZoneChangeEfct(DMS_RANK_MAIN_WORK *main_work)
{
	dmRankSetZoneChangeEfctPos(main_work);
	
	if (dmRankIsZoneChangeEfct(main_work)) {
		// フラグOFF
		main_work->flag &= ~DMD_RANK_FLAG_CHNG_ZONE_WIN_EFCT;
	}
}



// ==========================================================================
// dmRankSetZoneChangeEfctPos
/*!
	ランキングビュー時のゾーン切り替え時演出の座標設定処理
 */
// ==========================================================================
void dmRankSetZoneChangeEfctPos(DMS_RANK_MAIN_WORK *main_work)
{
	float move_dist[2] = {0, 0};
	float distance[2] = {0, 0};
	
	// 入り演出分の座標更新
	distance[0] = main_work->dst_rank_main_chng_pos[0] - main_work->src_rank_main_chng_pos[0];
	
	move_dist[0] = distance[0] / DMD_RANK_ZONE_EFCT_TIME;
	
	main_work->rank_win_main_move_pos += move_dist[0];
	
	distance[1] = main_work->dst_rank_main_chng_pos[1] - main_work->src_rank_main_chng_pos[1];
	
	move_dist[1] = distance[1] / DMD_RANK_ZONE_EFCT_TIME;
	
	main_work->rank_win_sub_move_pos += move_dist[1];
	
	
	
#if 0
	// 縦移動する場合
	distance = main_work->dst_rank_inout_efct_pos[0]
				- main_work->src_rank_inout_efct_pos[0];
	
	move_dist = distance / DMD_RANK_VIEW_TAB_EFCT_TIME;
	
	main_work->rank_inout_efct_pos[0] += move_dist - main_work->tab_efct_timer * 2.0f;
	
	main_work->tab_efct_timer += 1.0f;		// ◆
#endif
}



// ==========================================================================
// dmRankIsZoneChangeEfct
/*!
	ランキングビュー時のゾーン切り替え時演出中チェック処理
 */
// ==========================================================================
BOOL dmRankIsZoneChangeEfct(DMS_RANK_MAIN_WORK *main_work)
{
	u8 result = 0;
//	u32 is_efct_end = 0;
	
	// 掃け演出終了チェック
	if (main_work->dst_rank_main_chng_pos[0] > 0) {
		if (main_work->rank_win_main_move_pos >= main_work->dst_rank_main_chng_pos[0]) {
			main_work->rank_win_main_move_pos = main_work->dst_rank_main_chng_pos[1];
			main_work->rank_win_sub_move_pos = main_work->dst_rank_main_chng_pos[0];
			
			result |= 1 << 0;
		}
	}
	else if (main_work->dst_rank_main_chng_pos[0] <= 0) {
		if (main_work->rank_win_main_move_pos <= main_work->dst_rank_main_chng_pos[0]) {
			main_work->rank_win_main_move_pos = main_work->dst_rank_main_chng_pos[1];
			main_work->rank_win_sub_move_pos = main_work->dst_rank_main_chng_pos[0];
			
			result |= 1 << 0;
		}
	}
	
	if (result != 1) {
		return FALSE;
	}
	
	return TRUE;
}



// ==========================================================================
// dmRankSetRankViewOutEfct
/*!
	ランキング時のステージテーブル掃け演出中処理
 */
// ==========================================================================
void dmRankSetRankViewOutEfct(DMS_RANK_MAIN_WORK *main_work)
{
	float move_dist = 0;
	float distance = 0;

	// 縦移動する場合
	distance = main_work->dst_rank_inout_efct_pos[0]
				- main_work->src_rank_inout_efct_pos[0];
	
	move_dist = distance / DMD_RANK_VIEW_TAB_EFCT_TIME;
	
	main_work->rank_inout_efct_pos[0] += move_dist + main_work->tab_efct_timer * 2.0f;
	
	main_work->tab_efct_timer += 1.0f;		// ◆
}



// ==========================================================================
// dmRankIsRankViewOutEfct
/*!
	ランキング時のステージテーブル掃け演出中処理
 */
// ==========================================================================
BOOL dmRankIsRankViewOutEfct(DMS_RANK_MAIN_WORK *main_work)
{
	float move_direct = 0.f;
	
	move_direct = main_work->dst_rank_inout_efct_pos[0]
					- main_work->src_rank_inout_efct_pos[0];
	
	
	// 掃け演出終了チェック
	if (main_work->rank_inout_efct_pos[0] >= main_work->dst_rank_inout_efct_pos[0]
		&& move_direct >= 0) {
		main_work->rank_inout_efct_pos[0] = main_work->dst_rank_inout_efct_pos[0];
		
		main_work->tab_efct_timer = 0.0f;
		
		return TRUE;
	}
	else if (main_work->rank_inout_efct_pos[0] <= main_work->dst_rank_inout_efct_pos[0]
		&& move_direct <= 0) {
		main_work->rank_inout_efct_pos[0] = main_work->dst_rank_inout_efct_pos[0];
		
		main_work->tab_efct_timer = 0.0f;
		
		return TRUE;
	}
	
	return FALSE;
}



// ==========================================================================
// dmRankSetRankMenuInEfct
/*!
	ゾーン選択時のゾーンテーブル入り演出中処理
 */
// ==========================================================================
void dmRankSetRankMenuInEfct(DMS_RANK_MAIN_WORK *main_work)
{
	float move_dist = 0;
	float distance = 0;

	// 縦移動する場合
	distance = main_work->dst_zone_inout_efct_pos[1]
				- main_work->src_zone_inout_efct_pos[1];
	
	move_dist = distance / DMD_RANK_MENU_TAB_EFCT_TIME;
	
	main_work->zone_inout_efct_pos[1] += move_dist - main_work->tab_efct_timer * 2.0f;
	
	main_work->tab_efct_timer += 1.0f;		// ◆
}




// ==========================================================================
// dmRankIsRankMenuInEfct
/*!
	ゾーン選択時のゾーンテーブル入り演出中処理
 */
// ==========================================================================
BOOL dmRankIsRankMenuInEfct(DMS_RANK_MAIN_WORK *main_work)
{
	float move_direct = 0.f;
	
	move_direct = main_work->dst_zone_inout_efct_pos[1]
					- main_work->src_zone_inout_efct_pos[1];
	
	// 掃け演出終了チェック
	if (main_work->zone_inout_efct_pos[1] >= main_work->dst_zone_inout_efct_pos[1]
		&& move_direct >= 0) {
		main_work->zone_inout_efct_pos[1] = main_work->dst_zone_inout_efct_pos[1];
		
		main_work->tab_efct_timer = 0.0f;
		
		return TRUE;
	}
	else if (main_work->zone_inout_efct_pos[1] <= main_work->dst_zone_inout_efct_pos[1]
		&& move_direct <= 0) {
		main_work->zone_inout_efct_pos[1] = main_work->dst_zone_inout_efct_pos[1];
		
		main_work->tab_efct_timer = 0.0f;
		
		return TRUE;
	}
	
	return FALSE;
}



// ==========================================================================
// dmRankSetRankMenuChangeFocus
/*!
	ランキング時のゾーン切り替え時演出中処理
 */
// ==========================================================================
void dmRankSetRankMenuChangeFocus(DMS_RANK_MAIN_WORK *main_work)
{
	
	UNREFERENCED_PARAMETER(main_work);
	
}



// ==========================================================================
// dmRankIsRankMenuChangeFocus
/*!
	ランキング時のゾーン切り替え時演出中処理
 */
// ==========================================================================
BOOL dmRankIsRankMenuChangeFocus(DMS_RANK_MAIN_WORK *main_work)
{
//	u8 result = 0;
	
	UNREFERENCED_PARAMETER(main_work);
	
	
//	if (result != 3) {
//		return FALSE;
//	}
	
	return TRUE;
}



// ==========================================================================
// dmRankSetRankViewCrsrChangeEfct
/*!
	ランキング時の縦のACT切り替え時演出中処理
 */
// ==========================================================================
void dmRankSetRankViewCrsrChangeEfct(DMS_RANK_MAIN_WORK *main_work)
{
	UNREFERENCED_PARAMETER(main_work);
	
//	float move_dist = 0;
//	float distance = 0;

	// 縦移動する場合
//	distance = main_work->crsr_move_dst - main_work->crsr_move_src;
	
//	move_dist = distance / DMD_RANK_CRSR_MOVE_TIME;
	
//	main_work->crsr_pos_y += move_dist;
}



// ==========================================================================
// dmRankIsRankViewCrsrChangeEfct
/*!
	ランキング時の縦のACT切り替え時演出中処理
 */
// ==========================================================================
BOOL dmRankIsRankViewCrsrChangeEfct(DMS_RANK_MAIN_WORK *main_work)
{
	// 掃け演出終了チェック
	if (main_work->timer >= DMD_RANK_CRSR_MOVE_TIME) {
//		main_work->crsr_pos_y = main_work->crsr_move_dst;
		
		return TRUE;
	}

	return FALSE;
}



// ==========================================================================
// dmRankSetObiOutEfct
/*!
	ファイルテーブル掃け演出用座標設定処理

  	ここでは基準点(一つ目のファイル)となる座標のみを移動させて
  	それ以外は一定の距離で表示位置をずらすようにする
 */
// ==========================================================================
void dmRankSetObiOutEfct(DMS_RANK_MAIN_WORK *main_work)
{
	float move_dist = 0;
	float distance = 0;

	// 帯移動分
	distance = DMD_RANK_OBI_NODISP_POS_Y - DMD_RANK_OBI_DISP_POS_Y;
	
	move_dist = distance / DMD_RANK_OBI_EFCT_TIME;
	
	main_work->obi_pos_y += move_dist;
	
}



// ==========================================================================
// dmRankIsObiOutEfctEnd
/*!
	帯掃け演出終了チェック処理
 */
// ==========================================================================
BOOL dmRankIsObiOutEfctEnd(DMS_RANK_MAIN_WORK *main_work)
{
	if (main_work->obi_pos_y >= DMD_RANK_OBI_NODISP_POS_Y) {
//	if (main_work->timer >= DMD_RANK_OBI_EFCT_TIME) {

		main_work->obi_pos_y = DMD_RANK_OBI_NODISP_POS_Y;

		return TRUE;
	}

	return FALSE;
}


// ==========================================================================
// dmRankSetObiInEfct
/*!
	ファイルテーブル掃け演出用座標設定処理

  	ここでは基準点(一つ目のファイル)となる座標のみを移動させて
  	それ以外は一定の距離で表示位置をずらすようにする
 */
// ==========================================================================
void dmRankSetObiInEfct(DMS_RANK_MAIN_WORK *main_work)
{
	float move_dist = 0;
	float distance = 0;

	// 帯移動分
	distance = DMD_RANK_OBI_DISP_POS_Y - DMD_RANK_OBI_NODISP_POS_Y;
	
	move_dist = distance / DMD_RANK_OBI_EFCT_TIME;
	
	main_work->obi_pos_y += move_dist;
	
}



// ==========================================================================
// dmRankIsObiInEfctEnd
/*!
	帯掃け演出終了チェック処理
 */
// ==========================================================================
BOOL dmRankIsObiInEfctEnd(DMS_RANK_MAIN_WORK *main_work)
{
	if (main_work->obi_pos_y <= DMD_RANK_OBI_DISP_POS_Y) {
//	if (main_work->timer >= DMD_RANK_OBI_EFCT_TIME) {

		main_work->obi_pos_y = DMD_RANK_OBI_DISP_POS_Y;

		return TRUE;
	}

	return FALSE;
}



// ==========================================================================
// dmRankSetWinOpenEfct
/*!
	ウインドウ入り演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmRankSetWinOpenEfct(DMS_RANK_MAIN_WORK *main_work)
{
	if (main_work->win_timer > DMD_RANK_WIN_EFCT_TIME) {
		// ウインドウ演出終了
		main_work->flag |= DMD_RANK_FLAG_WIN_EFCT_END;

		main_work->win_timer = 0.0f;

		for (u32 i = 0; i < 2; i++) {
			main_work->win_size_rate[i] = DMD_RANK_WIN_DEF_RATE;
		}
	}
	else {
		// タイマー更新(但しフレームレートが可変になるように修正すること)	◆
		main_work->win_timer++;
	}
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->win_size_rate[i] = main_work->win_timer / DMD_RANK_WIN_EFCT_TIME;
		}
		else {
			main_work->win_size_rate[i] = 1.0f;
		}

		if (main_work->win_size_rate[i] > 1.0f) {
			main_work->win_size_rate[i] = 1.0f;
		}
	}

	

}



// ==========================================================================
// dmRankSetWinCloseEfct
/*!
	ウインドウ閉め演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmRankSetWinCloseEfct(DMS_RANK_MAIN_WORK *main_work)
{
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->win_size_rate[i] = main_work->win_timer / DMD_RANK_WIN_EFCT_TIME;
		}
		else {
			main_work->win_size_rate[i] = 0.0f;
		}
	}

	if (main_work->win_timer < 0.0f) {
		// ウインドウ演出終了
		main_work->flag |= DMD_RANK_FLAG_WIN_EFCT_END;

		main_work->win_timer = 0.0f;

		for (u32 i = 0; i < 2; i++) {
			main_work->win_size_rate[i] = 0.0f;
		}
	}
	else {
		// タイマー更新(但しフレームレートが可変になるように修正すること)	◆
		main_work->win_timer--;
	}
}



// ==========================================================================
// dmRankSetMenuChngFocusStage
/*!
	ランキングメニューのFOCUSステージ切り替え設定処理
 */
// ==========================================================================
void dmRankSetMenuChngFocusStage(DMS_RANK_MAIN_WORK *main_work)
{
	
	if (main_work->flag & DMD_RANK_FLAG_MENU_LEFT_INPUT) {
		main_work->prev_stage = main_work->cur_stage;
		
		main_work->cur_stage = dmRankGetRevisedMenuStageNo((s32)main_work->cur_stage
														   , -1
														   , (s32)main_work->cur_zone);
	}
	
	if (main_work->flag & DMD_RANK_FLAG_MENU_RIGHT_INPUT) {
		main_work->prev_stage = main_work->cur_stage;
		
		main_work->cur_stage = dmRankGetRevisedMenuStageNo((s32)main_work->cur_stage
														   , 1
														   , (s32)main_work->cur_zone);
	}
	
}



// ==========================================================================
// dmRankSetMenuChngFocusZone
/*!
	ランキングメニューのFOCUSゾーン切り替え設定処理
 */
// ==========================================================================
void dmRankSetMenuChngFocusZone(DMS_RANK_MAIN_WORK *main_work)
{
	
	if (main_work->flag & DMD_RANK_FLAG_MENU_UP_INPUT) {
		main_work->prev_zone = main_work->cur_zone;
		
		main_work->cur_zone = dmRankGetRevisedMenuZoneNo((s32)main_work->cur_zone
														 , -1
														 );
	}
	
	if (main_work->flag & DMD_RANK_FLAG_MENU_DOWN_INPUT) {
		main_work->prev_zone = main_work->cur_zone;
		
		main_work->cur_zone = dmRankGetRevisedMenuZoneNo((s32)main_work->cur_zone
														 , 1
														 );
	}
	
	if (main_work->prev_zone == DME_RANK_ZONE_TYPE_SPECIAL
		&& main_work->cur_stage > 3) {
		main_work->prev_stage = main_work->cur_stage;
		
		main_work->cur_stage = 3;
	}
	
	if (main_work->cur_zone == DME_RANK_ZONE_TYPE_FINAL) {
		main_work->cur_stage = 0;
	}
	
	// FOCUSゾーン切り替え演出フラグON
	main_work->flag |= DMD_RANK_FLAG_MENU_CHNG_CRSR_EFCT;
}



// ==========================================================================
// dmRankSetViewChngFocusRank
/*!
	ランキングビューのカーソル切り替え設定処理
 */
// ==========================================================================
void dmRankSetViewChngFocusRank(DMS_RANK_MAIN_WORK *main_work)
{
	u32 tmp_cmp_list_num = 0;
	s32 tmp_top_rank_no = 0;
	
	// 計算用一時保存
	tmp_top_rank_no = (s32)main_work->disp_top_rank_no;
	
	if (main_work->disp_list_num > 0) {
		tmp_cmp_list_num = (u32)(main_work->disp_list_num - 1);
	}
	else {
		tmp_cmp_list_num = 0;
	}
	
	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_RANK_UP
		&& main_work->cur_slct_data == 0) {
		if (tmp_top_rank_no == 0) {
			return;
		}
	}
	else if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_RANK_DOWN
			 && main_work->cur_slct_data >= tmp_cmp_list_num) {
		if (main_work->disp_list_num < DMD_RANK_VIEW_CHNG_PAGE_NUM) {
			return;
		}
	}
	
#if !_WII
	// 順位が最も上か下にあった場合、データを改めてダウンロードする(PS3/XBOX360対応)
	if ((main_work->flag & DMD_RANK_FLAG_RANK_CHNG_RANK_UP
		 && main_work->cur_slct_data == 0)
		|| (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_RANK_DOWN
			&& main_work->cur_slct_data == 10 - 1)) {
		// データダウンロード開始(ボードはそのままで順位のみ変更)
		main_work->proc_menu_update = dmRankProcRankSuccessUpload;
		
		main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_RANK_LIST;
		
		
		if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_RANK_UP) {
			tmp_top_rank_no -= DMD_RANK_VIEW_CHNG_PAGE_NUM;
		}
		
		else if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_RANK_DOWN) {
			tmp_top_rank_no += DMD_RANK_VIEW_CHNG_PAGE_NUM;
		}
	}
#endif
	
	
	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_RANK_UP) {
		main_work->prev_slct_data = main_work->cur_slct_data;
		
		main_work->cur_slct_data = dmRankGetRevisedRankCrsrFocus((s32)main_work->cur_slct_data
																 , -1
																 );
	}
	
	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_RANK_DOWN) {
		main_work->prev_slct_data = main_work->cur_slct_data;
		
		main_work->cur_slct_data = dmRankGetRevisedRankCrsrFocus((s32)main_work->cur_slct_data
																 , 1
																 );
	}
	
	// 以下は範囲外チェック
	if (tmp_top_rank_no < 0) {
		tmp_top_rank_no = 0;
	}
	
	if (tmp_top_rank_no >= DMD_RANK_VIEW_RANK_MAX_NUM) {
		tmp_top_rank_no = DMD_RANK_VIEW_RANK_MAX_NUM - 1;
	}
	
	// 再度、格納(計算結果)
	main_work->disp_top_rank_no = (u32)tmp_top_rank_no;
	
	if (!dm_rank_is_pause_maingame) {
		DmSoundPlaySE("Cursol");
	}
	else {
		GsSoundPlaySe("Cursol", main_work->se_handle);
	}
	
	// カーソル移動先設定		※今は移動演出抜きの仮設定(直接表示位置を変更)
	main_work->rank_crsr_pos[0] = 32.f * main_work->cur_slct_data;
	main_work->rank_crsr_pos[1] = 32.f * main_work->cur_slct_data;
	
}



// ==========================================================================
// dmRankSetViewChngFocusTab
/*!
	ランキングビューのカーソル切り替え設定処理
 */
// ==========================================================================
void dmRankSetViewChngFocusTab(DMS_RANK_MAIN_WORK *main_work)
{
	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_TAB_LEFT) {
		main_work->prev_slct_tab = main_work->cur_slct_tab;
		
		main_work->cur_slct_tab = dmRankGetRevisedRankTabFocus((s32)main_work->cur_slct_tab
															   , -1
															   );
	}
	
	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_TAB_RIGHT) {
		main_work->prev_slct_tab = main_work->cur_slct_tab;
		
		main_work->cur_slct_tab = dmRankGetRevisedRankTabFocus((s32)main_work->cur_slct_tab
															   , 1
															   );
	}
	
	// 切り替え後のタブがALLの場合
	if (main_work->cur_slct_tab == DME_RANK_SCORE_TYPE_ALL) {
		main_work->disp_top_rank_no = 0;
	}
	
	// 切り替え後のタブがMYSCOREの場合
	if (main_work->cur_slct_tab == DME_RANK_SCORE_TYPE_MYSCORE) {
		main_work->flag |= DMD_RANK_FLAG_SET_DISP_RANK_NEAR;
	}
	
	// フレンドからそれ以外、またはフレンド以外からフレンドへ切り替わるときはボード変更
	if (main_work->cur_slct_tab != DME_RANK_SCORE_TYPE_FRIENDS
		&& main_work->prev_slct_tab == DME_RANK_SCORE_TYPE_FRIENDS) {
		// データダウンロード開始(ボード設定から行う
		main_work->proc_menu_update = dmRankProcRankSetDataUpload;
		
		// ランキングデータアップロードチェック
		dmRankSetDataUploadCheck(main_work);
	}
	else if (main_work->prev_slct_tab != DME_RANK_SCORE_TYPE_FRIENDS
		&& main_work->cur_slct_tab == DME_RANK_SCORE_TYPE_FRIENDS) {
		// データダウンロード開始(ボード設定から行う
		main_work->proc_menu_update = dmRankProcRankSetDataUpload;
		
		// ランキングデータアップロードチェック
		dmRankSetDataUploadCheck(main_work);
		
		// フレンドタブに切り替えた際も必ず0から始まるように設定
		main_work->disp_top_rank_no = 0;
	}
	// それ以外はランキング番号のみ変更
	else {
		// データダウンロード開始(ボードはそのままで順位のみ変更)
		main_work->proc_menu_update = dmRankProcRankSuccessUpload;
	}
	
	
/*	
	// FOCUSゾーン切り替え演出フラグON
	main_work->flag |= DMD_RANK_FLAG_MENU_CHNG_CRSR_EFCT;
*/
}



// ==========================================================================
// dmRankSetViewChngFocusStage
/*!
	ランキングビューのカーソル切り替え設定処理
 */
// ==========================================================================
void dmRankSetViewChngFocusStage(DMS_RANK_MAIN_WORK *main_work)
{
	u32 act_num = 0;
	
	// 画面外へ掃ける側
	act_num = dm_rank_zone_act_num_tbl[main_work->cur_zone][1];
	
	
	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_ACT_LEFT) {
		
		if (main_work->cur_stage - 1 >= act_num) {
			main_work->prev_zone = main_work->cur_zone;
			main_work->cur_zone = dmRankGetRevisedMenuZoneNo((s32)main_work->cur_zone
															 , -1
															 );
			
			main_work->prev_stage = main_work->cur_stage;
			main_work->cur_stage = dm_rank_zone_act_num_tbl[main_work->cur_zone][1] - 1;
		}
		else {
			main_work->cur_stage -= 1;
		}
		
	}
	
	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_ACT_RIGHT) {
		
		if (main_work->cur_stage + 1 >= act_num) {
			main_work->prev_zone = main_work->cur_zone;
			main_work->cur_zone = dmRankGetRevisedMenuZoneNo((s32)main_work->cur_zone
															 , 1
															 );
			
			main_work->prev_stage = main_work->cur_stage;
			main_work->cur_stage = 0;
		}
		
		else {
			main_work->cur_stage += 1;
		}
	}
}



// ==========================================================================
// dmRankSetViewChngZoneEfctInit
/*!
	ランキングウインドウの切り替え演出初期化設定処理
 */
// ==========================================================================
void dmRankSetViewChngZoneEfctInit(DMS_RANK_MAIN_WORK *main_work)
{
	// 右へ移動
	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_ACT_LEFT) {
		// 0は真ん中に表示される方
		main_work->dst_rank_main_chng_pos[0] = 1000.f;
		main_work->src_rank_main_chng_pos[0] = 0.f;
		
		// 1は画面外に配置される方
		main_work->dst_rank_main_chng_pos[1] = 0.f;
		main_work->src_rank_main_chng_pos[1] = -1000.f;
		
		// 移動開始位置を設定
		main_work->rank_win_main_move_pos = 0.f;
		main_work->rank_win_sub_move_pos = -1000.f;
	}
	
	// 左へ移動
	else {
//	if (main_work->flag & DMD_RANK_FLAG_RANK_CHNG_TAB_RIGHT) {
		// 0は真ん中に表示される方
		main_work->dst_rank_main_chng_pos[0] = -1000.f;
		main_work->src_rank_main_chng_pos[0] = 0.f;
		
		// 1は画面外に配置される方
		main_work->dst_rank_main_chng_pos[1] = 0.f;
		main_work->src_rank_main_chng_pos[1] = 1000.f;
		
		// 移動開始位置を設定
		main_work->rank_win_main_move_pos = 0.f;
		main_work->rank_win_sub_move_pos = 1000.f;
	}
	
	// 移動演出フラグON
	main_work->flag |= DMD_RANK_FLAG_CHNG_ZONE_WIN_EFCT;
}



// ===========================================================================
//	dmRankGetRevisedMenuStageNo
/*!
	項目の場所を示す変数が最大・最小値を超えた際に補正した(回り込みさせた)値を取得する関数
	通常時のカーソル移動の選択項目に使用。

	@param id		[in] 項目番号
	@param diff		[in] 変化量(移動における)
	@return 補正された項目番号
*/
// ===========================================================================
u32 dmRankGetRevisedMenuStageNo(s32 idx, s32 diff, s32 zone_no)
{
	s32 result;
	s32 stage_max = 0;
	
	result = (int)idx + diff;
	
	stage_max = (s32)dm_rank_zone_act_num_tbl[zone_no][1];
	
	// 先頭から一つ戻ると最後に移動
	if (result < 0) {
		result = (int)(stage_max - 1);
	}
	
	// 最後から一つ進むと先頭に移動
	if (result >= stage_max) {
		result = 0;
	}
	
	MTM_ASSERT(result >= 0 && result < stage_max);
	
	return (u32)result;
}



// ===========================================================================
//	dmRankGetRevisedMenuZoneNo
/*!
	項目の場所を示す変数が最大・最小値を超えた際に補正した(回り込みさせた)値を取得する関数
	通常時のカーソル移動の選択項目に使用。

	@param id		[in] 項目番号
	@param diff		[in] 変化量(移動における)
	@return 補正された項目番号
*/
// ===========================================================================
u32 dmRankGetRevisedMenuZoneNo(s32 idx, s32 diff)
{
	s32 result;
	
	result = (int)idx + diff;
	
	// 先頭から一つ戻ると最後に移動
	if (result < 0) {
		result = (int)(6 - 1);
	}
	
	// 最後から一つ進むと先頭に移動
	if (result >= 6) {
		result = 0;
	}
	
	MTM_ASSERT(result >= 0 && result < 6);
	
	return (u32)result;
}



// ===========================================================================
//	dmRankGetRevisedRankCrsrFocus
/*!
	項目の場所を示す変数が最大・最小値を超えた際に補正した(回り込みさせた)値を取得する関数
	通常時のカーソル移動の選択項目に使用。

	@param id		[in] 項目番号
	@param diff		[in] 変化量(移動における)
	@return 補正された項目番号
*/
// ===========================================================================
u32 dmRankGetRevisedRankCrsrFocus(s32 idx, s32 diff)
{
	s32 result;
	
	result = (int)idx + diff;
	
	// 先頭から一つ戻ると最後に移動
	if (result < 0) {
		result = (int)(10 - 1);
	}
	
	// 最後から一つ進むと先頭に移動
	if (result >= 10) {
		result = 0;
	}
	
	MTM_ASSERT(result >= 0 && result < 10);
	
	return (u32)result;
}



// ===========================================================================
//	dmRankGetRevisedRankTabFocus
/*!
	項目の場所を示す変数が最大・最小値を超えた際に補正した(回り込みさせた)値を取得する関数
	通常時のカーソル移動の選択項目に使用。

	@param id		[in] 項目番号
	@param diff		[in] 変化量(移動における)
	@return 補正された項目番号
*/
// ===========================================================================
u32 dmRankGetRevisedRankTabFocus(s32 idx, s32 diff)
{
	s32 result;
	
	result = (int)idx + diff;
	
	// 先頭から一つ戻ると最後に移動
	if (result < 0) {
		result = (int)(DMD_RANK_VIEW_TAB_MAX_NUM - 1);
	}
	
	// 最後から一つ進むと先頭に移動
	if (result >= DMD_RANK_VIEW_TAB_MAX_NUM) {
		result = 0;
	}
	
	MTM_ASSERT(result >= 0 && result < DMD_RANK_VIEW_TAB_MAX_NUM);
	
	return (u32)result;
}






// ==========================================================================
// dmRankSetObiEfctPos
/*!
	帯アクションの演出用座標設定処理
 */
// ==========================================================================
void dmRankSetObiEfctPos(DMS_RANK_MAIN_WORK *main_work)
{
	for (u32 i = 0; i < 2; i++) {
		
		// 一定位置に来たら戻る
		if (main_work->obi_pos[i] < DMD_RANK_OBI_MOVE_END_POS) {
			main_work->obi_pos[i] = DMD_RANK_OBI_MOVE_START_POS;
		}
		
		// 移動分座標加算
		main_work->obi_pos[i] += DMD_RANK_OBI_MOVE_SPEED;
	}
}



// ==========================================================================
// dmRankActAnimeControl
/*!
	アクションアニメーション関連の制御処理
 */
// ==========================================================================
void dmRankActAnimeControl(DMS_RANK_MAIN_WORK *main_work)
{
	
	
	for (int i = 0; i < 2; i++) {
		if (main_work->index_act_anime_frm[i] <= DMD_RANK_VIEW_INDEX_ANIME_FRM) {
			main_work->index_act_anime_frm[i]++;
		}
		else {
			main_work->index_act_anime_frm[i] = DMD_RANK_VIEW_INDEX_ANIME_FRM;
		}
	}
	
	
	if (main_work->sonic_icon_efct_frm <= DMD_RANK_VIEW_SONIC_ICON_FRM) {
		main_work->sonic_icon_efct_frm++;
	}
	else {
		main_work->sonic_icon_efct_frm = DMD_RANK_VIEW_SONIC_ICON_FRM;
	}

	
	
	if (main_work->flag & DMD_RANK_FLAG_CHNG_GAME_MODE_EFCT) {
		main_work->index_act_anime_frm[0] = 0;
	}
	
	if (main_work->flag & DMD_RANK_FLAG_CHNG_SONIC_EFCT) {
		main_work->index_act_anime_frm[1] = 0;
		main_work->sonic_icon_efct_frm = 0;
		
		main_work->flag &= ~DMD_RANK_FLAG_CHNG_SONIC_EFCT;
	}
	
	
	
	
	// L1、R1ボタン押下演出
	if (main_work->btn_l_disp_frm < DMD_RANK_ZONE_CHNG_BTN_END_FRM) {
		main_work->btn_l_disp_frm++;
	}
	else {
		main_work->btn_l_disp_frm = DMD_RANK_ZONE_CHNG_BTN_END_FRM;
	}
	
	
	if (main_work->btn_r_disp_frm < DMD_RANK_ZONE_CHNG_BTN_END_FRM) {
		main_work->btn_r_disp_frm++;
	}
	else {
		main_work->btn_r_disp_frm = DMD_RANK_ZONE_CHNG_BTN_END_FRM;
	}
	
}



// ==========================================================================
// dmRankSetMenuDispZoneTablePos
/*!
	ランキングメニューのFOCUSZONE表示位置設定処理
 */
// ==========================================================================
void dmRankSetMenuDispZoneTablePos(DMS_RANK_MAIN_WORK *main_work)
{
	
	main_work->zone_pos[0][0] = DMD_RANK_ZONE_TABLE_TOP_POS_X;
	main_work->zone_pos[0][1] = DMD_RANK_ZONE_TABLE_TOP_POS_Y;
	
	for (u32 i = 1; i < DME_RANK_ZONE_TYPE_MAX; i++) {
		main_work->zone_pos[i][0] = DMD_RANK_ZONE_TABLE_TOP_POS_X;
		main_work->zone_pos[i][1] = main_work->zone_pos[i-1][1] + 60.f;
		
		if (main_work->cur_zone == i - 1) {
			main_work->zone_pos[i][1] += 60.f;
		}
	}
	
	main_work->disp_flag |= DMD_RANK_DISP_FLAG_MENU_TAB;
	
	for (u32 i = 0; i < 6; i++) {
		if (main_work->cur_zone == i) {
			main_work->zone_tab_scale[i][1] = 1.f;
		}
		else {
			main_work->zone_tab_scale[i][1] = 0.f;
		}
	}
	
}



// ==========================================================================
// dmRankSetRankingEndParam
/*!
	ランキング画面終了時の必要な設定処理(フェード処理含む)
 */
// ==========================================================================
void dmRankSetRankingEndParam(DMS_RANK_MAIN_WORK *main_work)
{
	main_work->proc_win_input = NULL;
	main_work->proc_input = NULL;
	
	main_work->proc_menu_update = dmRankProcCheckDisconnectNetwork;
	
	// 切断開始
	DmRankSysEnd();
	
	// エラーチェックフラグOFF
	main_work->flag &= ~DMD_RANK_DISP_FLAG_CHECK_NET_ERROR;
	
	// メインメニューへイベント遷移先設定
	main_work->next_evt = DME_RANK_NEXT_EVT_MAINMENU;
	
	if (!dm_rank_is_pause_maingame) {
		DmSoundPlaySE("Cancel");
	}
	else {
		GsSoundPlaySe("Cancel", main_work->se_handle);
	}
}



// ==========================================================================
// dmRankSetRankViewEndParam
/*!
	ランキングビュー画面終了時の必要な設定処理(フェード処理含む)
  	(ランキングメニューへ戻る際の演出設定含む)
 */
// ==========================================================================
void dmRankSetRankViewEndParam(DMS_RANK_MAIN_WORK *main_work)
{
	main_work->proc_win_input = NULL;
	main_work->proc_input = NULL;
	
	main_work->proc_menu_update = dmRankProcRankViewOutEfct;
	
	// 指定座標に設定
	main_work->rank_inout_efct_pos[0] = DMD_RANK_VIEW_TAB_DISP_POS;
	main_work->dst_rank_inout_efct_pos[0] = DMD_RANK_VIEW_TAB_NODISP_POS;
	main_work->src_rank_inout_efct_pos[0] = DMD_RANK_VIEW_TAB_DISP_POS;
	
	if (!dm_rank_is_pause_maingame) {
		DmSoundPlaySE("Cancel");
	}
	else {
		GsSoundPlaySe("Cancel", main_work->se_handle);
	}
	
	main_work->disp_flag &= ~DMD_RANK_DISP_FLAG_RANK_LIST;
	
}



// ==========================================================================
// dmRankSetStageNoForViewOnly
/*!
	ランキングビュー用ステージID設定処理
 */
// ==========================================================================
void dmRankSetStageNoForViewOnly(DMS_RANK_MAIN_WORK *main_work)
{
	GSS_MAIN_SYS_INFO *gs_main = GsGetMainSysInfo();
	u32 tmp_stage_id = 0;
	s32 result_stage_id = 0;
	s32 result_zone_id = 0;
	
	tmp_stage_id = gs_main->stage_id;
	
	
	// 通常、ありえないがもし設定されていたら1-1へ
	if (tmp_stage_id >= GSD_MAIN_STAGE_ID_ENDING) {
		MTM_ASSERT(0);
		tmp_stage_id = 0;
	}
	
	if (tmp_stage_id >= GSD_MAIN_STAGE_ID_SS1) {
		// FINALの2～5の分を補正(SS1をF-1の次になるように補正)
		tmp_stage_id -= GSD_MAIN_STAGE_ID_SS1 - GSD_MAIN_STAGE_ID_FINAL_2;
	}
	
	else if (tmp_stage_id >= GSD_MAIN_STAGE_ID_FINAL_1) {
		tmp_stage_id = GSD_MAIN_STAGE_ID_FINAL_1;
	}
	
	else {
		// 通常ACTはそのまま
	}
	
	for (int i = DME_RANK_ZONE_TYPE_MAX - 1; i >= 0; i--) {
		if (tmp_stage_id >= dm_rank_zone_act_num_tbl[i][0]) {
			result_zone_id = i;
			
			result_stage_id = (s32)(tmp_stage_id - dm_rank_zone_act_num_tbl[i][0]);
			
			if (result_stage_id >= (s32)dm_rank_zone_act_num_tbl[i][1]) {
				result_stage_id = (s32)dm_rank_zone_act_num_tbl[i][1] - 1;
			}
			
			break;
		}
	}
	
	// ゾーンIDクリップ(保険)
	if (result_zone_id < 0
		|| result_zone_id >= DME_RANK_ZONE_TYPE_MAX) {
		result_zone_id = 0;
	}
	
	main_work->cur_zone = 
	main_work->prev_zone = (u32)result_zone_id;
	main_work->cur_stage = 
	main_work->prev_stage = (u32)result_stage_id;
	
	
	if (gs_main->game_mode == GSD_GAME_MODE_STORY) {
		main_work->cur_game_mode = DME_RANK_DISP_RANK_SCORE;
	}
	else {
		main_work->cur_game_mode = DME_RANK_DISP_RANK_TIME;
	}
	
	// アニメーション初期化
	main_work->btn_l_disp_frm = DMD_RANK_ZONE_CHNG_BTN_END_FRM;
	main_work->btn_r_disp_frm = DMD_RANK_ZONE_CHNG_BTN_END_FRM;
}



// ==========================================================================
// dmRankSetDataUploadCheck
/*!
	ランキングデータのアップロードチェック設定処理
 */
// ==========================================================================
void dmRankSetDataUploadCheck(DMS_RANK_MAIN_WORK *main_work)
{
	// ここで表示するステージを設定
	if (main_work->cur_zone == DME_RANK_ZONE_TYPE_SPECIAL) {
		main_work->cur_view_stage = DMD_RANK_SYS_BOARD_SS1 + main_work->cur_stage;
	}
	else if (main_work->cur_zone == DME_RANK_ZONE_TYPE_FINAL) {
		main_work->cur_view_stage = DMD_RANK_SYS_BOARD_ZF;
	}
	else {
		main_work->cur_view_stage = main_work->cur_stage + main_work->cur_zone * 4;
	}
	
	// 例外ケース保険
	if (main_work->cur_view_stage >= DMD_RANK_SYS_BOARD_NUM) {
		MTM_ASSERT(0);
		main_work->cur_view_stage = 0;
	}
	
	// ランキングデータDL中処理へ
	if (DmRankSysOwnRecodeIsUpdate((DME_RANK_SYS_BOARD)main_work->cur_view_stage)) {
#if !_WII
		main_work->proc_menu_update = dmRankProcRankSetDataUpload;
		main_work->is_data_upload = TRUE;
#else
		main_work->proc_menu_update = dmRankProcRankIsMyDataUpload;
		main_work->is_data_upload = TRUE;
		main_work->announce_flag |= 1 << DME_RANK_WIN_DO_YOU_REGIST;
#endif
	}
	else {
		main_work->proc_menu_update = dmRankProcRankSetDataUpload;
		main_work->is_data_upload = FALSE;
	}
	
}



// ==========================================================================
// dmRankSetDispRankInitData
/*!
	ランキングデータの表示初期化設定処理
 */
// ==========================================================================
void dmRankSetDispRankInitData(DMS_RANK_MAIN_WORK *main_work)
{
	// 切り替え時は必ず0から始まるように設定
	main_work->disp_top_rank_no = 0;
	
#if 0
	if (main_work->cur_game_mode != DME_RANK_DISP_RANK_TIME) {
		main_work->cur_game_mode = DME_RANK_DISP_RANK_TIME;
		main_work->flag |= DMD_RANK_FLAG_CHNG_GAME_MODE_EFCT;
	}
	else {
		main_work->cur_game_mode = DME_RANK_DISP_RANK_TIME;
	}
	
	main_work->cur_sonic_type = 0;
	
	main_work->cur_slct_tab = 0;
	
	main_work->flag |= DMD_RANK_FLAG_CHNG_SONIC_EFCT;
#endif
	
	main_work->cur_slct_data = 0;
	
	// カーソル移動先設定
	main_work->rank_crsr_pos[0] = 32.f * main_work->cur_slct_data;
	main_work->rank_crsr_pos[1] = 32.f * main_work->cur_slct_data;
}



// ==========================================================================
// dmRankIsSonicOnlyStage
/*!
	ソニックのみ使用できるステージかどうかの判定処理
 */
// ==========================================================================
BOOL dmRankIsSonicOnlyStage(DMS_RANK_MAIN_WORK *main_work)
{
	BOOL result = FALSE;
	
	if (main_work->cur_zone == DME_RANK_ZONE_TYPE_SPECIAL) {
		result = TRUE;
	}
	
	else if (main_work->cur_zone == DME_RANK_ZONE_TYPE_FINAL) {
		result = TRUE;
	}
	
	else if (main_work->cur_stage == 3) {
		result = TRUE;
	}
	
	else {
		result = FALSE;
	}
	
	return result;
}



// ==========================================================================
// dmRankSetSaveDispRankTmpStrg
/*!
	取得したランキングデータを一時バッファに保存する設定処理
 */
// ==========================================================================
void dmRankSetSaveDispRankTmpStrg(DMS_RANK_MAIN_WORK *main_work)
{
	DMS_RANK_DISP_DATA_BUF *tmp_data_buf = NULL;
	
	// ここでランキングデータのポインタ取得
	tmp_data_buf = dmRankGetDispRankTmpStrg(main_work);
	
	tmp_data_buf->disp_top_rank_no
		= main_work->disp_top_rank_no;
	
	tmp_data_buf->disp_list_num
		= main_work->disp_list_num;
	
	tmp_data_buf->my_rank_data
		= main_work->my_rank_data;
	
	for (int i = 0; i < DMD_RANK_DISP_RANKING_NUM; i++) {
		tmp_data_buf->disp_rank_no[i]
			= main_work->disp_rank_no[i];
		
		tmp_data_buf->disp_time[i]
			= main_work->disp_time[i];
		
		tmp_data_buf->disp_score[i]
			= main_work->disp_score[i];
		
		amCopyMemory(tmp_data_buf->disp_name[i]
					 , main_work->disp_name[i]
					 , sizeof(s8) * DMD_RANK_DISP_LIST_NAME_CHAR_NUM);
		
		tmp_data_buf->disp_area[i]
			= main_work->disp_area[i];
		
		tmp_data_buf->disp_sonic[i]
			= main_work->disp_sonic[i];
	}
	
}



// ==========================================================================
// dmRankSetDispRankTmpStrg
/*!
	ランキングビューに一度取得済みのデータを表示する場合の設定処理
 */
// ==========================================================================
void dmRankSetDispRankTmpStrg(DMS_RANK_MAIN_WORK *main_work)
{
	DMS_RANK_DISP_DATA_BUF *tmp_data_buf = NULL;
	
	tmp_data_buf = dmRankGetDispRankTmpStrg(main_work);
	
	main_work->disp_top_rank_no
		= tmp_data_buf->disp_top_rank_no;
	
	main_work->disp_list_num
		= tmp_data_buf->disp_list_num;
	
	main_work->my_rank_data
		= tmp_data_buf->my_rank_data;
	
	for (int i = 0; i < DMD_RANK_DISP_RANKING_NUM; i++) {
		
		main_work->disp_rank_no[i]
			= tmp_data_buf->disp_rank_no[i];
		
		main_work->disp_time[i]
			= tmp_data_buf->disp_time[i];
		
		main_work->disp_score[i]
			= tmp_data_buf->disp_score[i];
		
		amCopyMemory(main_work->disp_name[i]
					 , tmp_data_buf->disp_name[i]
					 , sizeof(s8) * DMD_RANK_DISP_LIST_NAME_CHAR_NUM);
		
		main_work->disp_area[i]
			= tmp_data_buf->disp_area[i];
		
		main_work->disp_sonic[i]
			= tmp_data_buf->disp_sonic[i];
	}
	
}



// ==========================================================================
// dmRankGetDispRankTmpStrg
/*!
	ランキングビューに一度取得済みのデータを表示する場合の設定処理
 */
// ==========================================================================
DMS_RANK_DISP_DATA_BUF *dmRankGetDispRankTmpStrg(DMS_RANK_MAIN_WORK *main_work)
{
	DMS_RANK_DISP_DATA_BUF *result = NULL;
	
	DMS_RANK_TAB_TYPE_DATA_BUF *tmp_tab_data_p = NULL;
	DMS_RANK_SONIC_TYPE_DATA_BUF *tmp_sonic_data_p = NULL;
	DMS_RANK_GAME_MODE_DATA_BUF *tmp_mode_data_p = NULL;
	
	tmp_mode_data_p = &main_work->tmp_strg_data;
	
	tmp_sonic_data_p = &tmp_mode_data_p->sonic_data_buf[main_work->cur_game_mode];
	
	tmp_tab_data_p = &tmp_sonic_data_p->tab_data_buf[main_work->cur_sonic_type];
	
	result = &tmp_tab_data_p->data_buf[main_work->cur_slct_tab];
	
	return result;
}



// ==========================================================================
// dmRankInitAllocRankTmpStrg
/*!
	ランキングビューの一時保存データのメモリ確保を行う処理
 */
// ==========================================================================
void dmRankInitAllocRankTmpStrg(DMS_RANK_MAIN_WORK *main_work)
{
	// 一時保存領域のメモリ確保
	for (int i = 0; i < DME_RANK_DISP_RANK_NUM; i++) {
		for (int j = 0; j < DME_RANK_SONIC_TYPE_NUM; j++) {
			for (int k = 0; k < DME_RANK_SCORE_TYPE_NUM; k++) {
				main_work->tmp_strg_data.sonic_data_buf[i]
					.tab_data_buf[j].data_buf[k].my_rank_data = 10;
				
				for (int l = 0; l < DMD_RANK_DISP_RANKING_NUM; l++) {
					main_work->tmp_strg_data.sonic_data_buf[i].tab_data_buf[j].data_buf[k].disp_name[l]
						= (char *)amMemAlloc(sizeof(s8) * DMD_RANK_DISP_LIST_NAME_CHAR_NUM);
					
					amZeroMemory(main_work->tmp_strg_data.sonic_data_buf[i].tab_data_buf[j].data_buf[k].disp_name[l]
								 , (sizeof(s8) * DMD_RANK_DISP_LIST_NAME_CHAR_NUM));
				}
			}
		}
	}
}



// ==========================================================================
// dmRankReleaseFreeRankTmpStrg
/*!
	ランキングビューの一時保存データのメモリ解放を行う処理
 */
// ==========================================================================
void dmRankReleaseFreeRankTmpStrg(DMS_RANK_MAIN_WORK *main_work)
{
	// 一時保存領域のメモリ解放
	for (int i = 0; i < DME_RANK_DISP_RANK_NUM; i++) {
		for (int j = 0; j < DME_RANK_SONIC_TYPE_NUM; j++) {
			for (int k = 0; k < DME_RANK_SCORE_TYPE_NUM; k++) {
				for (int l = 0; l < DMD_RANK_DISP_RANKING_NUM; l++) {
					amMemFree(main_work->tmp_strg_data.sonic_data_buf[i].tab_data_buf[j].data_buf[k].disp_name[l]);
					
					main_work->tmp_strg_data.sonic_data_buf[i].tab_data_buf[j].data_buf[k].disp_name[l] = NULL;
				}
			}
		}
	}
}



// ==========================================================================
// dmRankSetClearDispRankTmpStrg
/*!
	ランキングビューに一度取得済みのデータをクリアする場合の設定処理
 */
// ==========================================================================
void dmRankSetClearDispRankTmpStrg(DMS_RANK_MAIN_WORK *main_work)
{
	// 一時保存領域のメモリ確保
	for (int i = 0; i < DME_RANK_DISP_RANK_NUM; i++) {
		for (int j = 0; j < DME_RANK_SONIC_TYPE_NUM; j++) {
			for (int k = 0; k < DME_RANK_SCORE_TYPE_NUM; k++) {
				main_work->tmp_strg_data.sonic_data_buf[i]
					.tab_data_buf[j].data_buf[k].disp_top_rank_no = 0;
				main_work->tmp_strg_data.sonic_data_buf[i]
					.tab_data_buf[j].data_buf[k].disp_list_num = 0;
				main_work->tmp_strg_data.sonic_data_buf[i]
					.tab_data_buf[j].data_buf[k].my_rank_data = 10;
				
				for (int l = 0; l < DMD_RANK_DISP_RANKING_NUM; l++) {
					main_work->tmp_strg_data.sonic_data_buf[i]
						.tab_data_buf[j].data_buf[k].disp_rank_no[l] = 0;
					
					main_work->tmp_strg_data.sonic_data_buf[i]
						.tab_data_buf[j].data_buf[k].disp_time[l] = 0;
					
					main_work->tmp_strg_data.sonic_data_buf[i]
						.tab_data_buf[j].data_buf[k].disp_score[l] = 0;
					
					for (int m = 0; m < DMD_RANK_DISP_LIST_NAME_CHAR_NUM; m++) {
						main_work->tmp_strg_data.sonic_data_buf[i]
							.tab_data_buf[j].data_buf[k].disp_name[l][m] = 0;
					}
					
					main_work->tmp_strg_data.sonic_data_buf[i]
						.tab_data_buf[j].data_buf[k].disp_area[l] = 0;
					
					main_work->tmp_strg_data.sonic_data_buf[i]
						.tab_data_buf[j].data_buf[k].disp_sonic[l] = 0;
				}
			}
		}
	}
}



// ==========================================================================
// dmRankIsSaveRunData
/*!
	ランキング登録が必要かどうかをセーブフラグを見て判定する処理
 */
// ==========================================================================
BOOL dmRankIsSaveRunData(DMS_RANK_MAIN_WORK *main_work)
{
	UNREFERENCED_PARAMETER(main_work);
	
	GSS_MAIN_SYS_INFO *gs_main = GsGetMainSysInfo();
	BOOL result = FALSE;
	
	if (gs_main->is_save_run) {
		result = TRUE;
	}
	else {
		result = FALSE;
	}
	
	return result;
}



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
#endif