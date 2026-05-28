// ===========================================================================
/*!~GSD_
	@file	dmTit~DME_le.cpp
	@brief	デモ・タイトル画面

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: dmTitle.cpp 207 2011-05-30 16:41:42Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 *
 *
 */

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#include "dmTitle.h"
#include "../library/ao/include/aoTexture.h"
#include "../library/ao/include/aoAction.h"

#include "gs.h"
#include "gsMainSys.h"
#include "gmMain.h"
#include "syEvtSys.h"

#include "gsBackup.hpp"

#include "gmTask.h"
#include "gmSound.h"
#include "izFade.h"

#include "dmCmnBackup.h"
#include "dmSave.h"
#include "dmLoading.h"
#include "dmTitleOp.h"

#if _WII
#include "dmFileSlct.h"
#include "dmUserName.h"
#endif // #if _PC || _WII

#include "dmBuyScreen.h"
#include "gsReboot.h"

#include "gsTrophy.h"

#include "gsSound.h"
#include "dmSound.h"
#include "dmSndBgmPlayer.h"

#if _IPHONE
#include "erWeb.hpp"
#include "erTrgAoAction.hpp"
#include "dbgPadEmu.hpp"
#endif //_IPHONE

// データヘッダ
#if !_IPHONE
#include "common/ace/D_TITLE.HMA"
#else //!_IPHONE
#include "ace/D_TITLE.HMA"
#endif //!_IPHONE

#if _PC || _XBOX
#include "common/ace/D_TITLE_JP.HMA"
#elif _PS3
#include "ps3/ace/D_TITLE_JP.HMA"
#elif _WII
#include "wii/ace/D_TITLE_JP.HMA"
#elif _IPHONE
#include "ace/D_TITLE_JP.HMA"
#endif


// 共通データヘッダ
#if !_IPHONE
#include "common/ace/D_CMN_BTN.HMA"
#include "common/ace/D_CMN_OBI.HMA"
#include "common/ace/D_CMN_WIN.HMA"
#include "common/ace/D_CMN_MSG_JP.HMA"
#else //!_IPHONE
#include "ace/D_CMN_BTN.HMA"
#include "ace/D_CMN_OBI.HMA"
#include "ace/D_CMN_WIN.HMA"
#include "ace/D_CMN_MSG_JP.HMA"
#endif //!_IPHONE

//mpp -------------------------------
#include "mppUtil.h"
#include "mppCheckPointStorage.h"
#include "mppTimeScores.h"

static bool mpp_newGameButNotFirstGame;

// ----- Macros ------------------------------------------------（マクロ定義）

#define DMD_TITLE_TASK_PAUSELEVEL	(0)
#define DMD_TITLE_TASK_PRIO_MAIN	(0x2000)
#define DMD_TITLE_TASK_GROUP_MAIN	(10)

#define DMD_TITLE_DRAW_PRIO_ACT		(0x8400)
#define DMD_TITLE_DRAW_PRIO_WIN		(0x8800)
#define DMD_TITLE_DRAW_PRIO_WIN_FIX	(0x8c00)

#define DMD_TITLE_FILE_PATH_NUM_MAX	(60)

#define DMD_TITLE_IDLE_DISP_TIME	(40)
#define DMD_TITLE_IDLE_NONE_TIME	(30)

#define DMD_TITLE_DECIDE_EFCT_TIME	(60)
#define DMD_TITLE_DECIDE_BLINK_TIME	(4)

#define DMD_TITLE_AMA_DATA_ARC_ID	(0)
#define DMD_TITLE_AMB_DATA_ARC_ID	(1)

#if _PC || _XBOX
#define DMD_TITLE_CRSR_DFLT_POS_X		(264.f)
#define DMD_TITLE_SLCT_MENU_NUM_FIRST	(5)
#define DMD_TITLE_SLCT_MENU_NUM_PLAYED	(6)
#define DMD_TITLE_SLCT_MENU_NUM_TRIAL	(6)
#elif _PS3
#define DMD_TITLE_CRSR_DFLT_POS_X		(264.f)
#define DMD_TITLE_SLCT_MENU_NUM_FIRST	(3)
#define DMD_TITLE_SLCT_MENU_NUM_PLAYED	(4)
#define DMD_TITLE_SLCT_MENU_NUM_TRIAL	(4)
#else
#define DMD_TITLE_CRSR_DFLT_POS_X		(304.f)
#define DMD_TITLE_SLCT_MENU_NUM_FIRST	(3)
#define DMD_TITLE_SLCT_MENU_NUM_PLAYED	(3)
#define DMD_TITLE_SLCT_MENU_NUM_TRIAL	(3)
#endif

#define DMD_TITLE_CRSR_MOVE_TIME		(8.f)

#define DMD_TITLE_DRAW_STATE_ID			(10)

#define DMD_TITLE_SIZE_WIDTH			(960.0f)
#define DMD_TITLE_SIZE_HEIGHT			(720.0f)
#define DMD_TITLE_SIZE_HALF_WIDTH		(480.0f)
#define DMD_TITLE_SIZE_HALF_HEIGHT		(360.0f)

#define DMD_TITLE_WIN_EFCT_TIME			(8.0f)
#if !_IPHONE
#define DMD_TITLE_WINDOW_SIZE_W			(380.f)
#else //!_IPHONE
#define DMD_TITLE_WINDOW_SIZE_W			(420.f)
#endif //!_IPHONE
#define DMD_TITLE_WINDOW_SIZE_H			(180.f)
#define DMD_TITLE_WIN_DEF_RATE			(1.0f)

#if !_IPHONE
#define DMD_TITLE_DISP_SCALE_TEXT		(1.0f)
#else //!_IPHONE
#define DMD_TITLE_DISP_SCALE_TEXT		(1.5f * 1.125f)
#endif //!_IPHONE

#if _PC || _XBOX
#define DMD_TITLE_MMENU_WIN_SIZE_W		(486.f)
#define DMD_TITLE_MMENU_WIN_SIZE_H		(288.f)
#elif _PS3
#define DMD_TITLE_MMENU_WIN_SIZE_W		(486.f)
#define DMD_TITLE_MMENU_WIN_SIZE_H		(188.f)
#elif _IPHONE
#define DMD_TITLE_MMENU_WIN_SIZE_W		(800.0f)
#define DMD_TITLE_MMENU_WIN_SIZE_H		(540.0f)
#define DMD_TITLE_MMENU_WIN_SIZE_W_2	(DMD_TITLE_MMENU_WIN_SIZE_W)
#define DMD_TITLE_MMENU_WIN_SIZE_H_2	(DMD_TITLE_MMENU_WIN_SIZE_H - 180.0f)
#else
#define DMD_TITLE_MMENU_WIN_SIZE_W		(486.f)
#define DMD_TITLE_MMENU_WIN_SIZE_H		(138.f)
#endif

#define DMD_TITLE_PUSH_START_DISP_TIME	(3600)

#define DMD_TITLE_END_GAME_WAIT_TIME	(60)

// フェード関連
#define DMD_TITLE_FADEIN_TIME			(32.0f)
#define DMD_TITLE_FADEOUT_TIME			(32.0f)

#define DMD_TITLE_BGM_FADEIN_TIME		(32)
#define DMD_TITLE_BGM_FADEOUT_TIME		(32)


// フラグ関連
#define DMD_TITLE_FLAG_EXIT					(1 << 0)	//!< 終了フラグ
#define DMD_TITLE_FLAG_CANCEL				(1 << 1)	//!< キャンセル
#define DMD_TITLE_FLAG_DECIDE				(1 << 2)	//!< 決定フラグ
#define DMD_TITLE_FLAG_DISP_TEXT			(1 << 3)	//!< テキスト表示フラグ
#define DMD_TITLE_FLAG_DECIDE_EFCT			(1 << 4)	//!< ゲームスタート押した際の決定演出中
#define DMD_TITLE_FLAG_GAME_START			(1 << 5)	//!< 

#define DMD_TITLE_FLAG_MENU_UP_INPUT		(1 << 6)	//!< 
#define DMD_TITLE_FLAG_MENU_DOWN_INPUT		(1 << 7)	//!< 
#define DMD_TITLE_FLAG_MENU_CRSR_EFCT		(1 << 8)	//!< 

#define DMD_TITLE_FLAG_DISP_TITLE			(1 << 9)	//!< 
#define DMD_TITLE_FLAG_DISP_MAINMENU		(1 << 10)	//!<

#define DMD_TITLE_FLAG_BUY_COMP				(1 << 11)	//!<
#define DMD_TITLE_FLAG_EXIST_SAVE_DATA		(1 << 12)	//!<

#define DMD_TITLE_FLAG_BACK_DEL_SAVE		(1 << 13)	//!< 
#define DMD_TITLE_FLAG_DEL_SAVE_DATA		(1 << 14)	//!<
#define DMD_TITLE_FLAG_NEW_GAME_START		(1 << 15)	//!<

#define DMD_TITLE_FLAG_WIN_EFCT_END			(1 << 16)	//!< 
#define DMD_TITLE_FLAG_WIN_DRAW_START		(1 << 17)	//!<

#define DMD_TITLE_FLAG_COMP_NO_DISP_END		(1 << 18)	//!<
#define DMD_TITLE_FLAG_DEMO_SND_END			(1 << 19)	//!<
#define DMD_TITLE_FLAG_DISP_LOOP_END		(1 << 20)	//!< ロゴへの遷移待ち時間経過を示すフラグ

#define DMD_TITLE_FLAG_DISP_MMENU_WIN		(1 << 21)	//!<
#define DMD_TITLE_FLAG_MMENU_WIN_EFCT_END	(1 << 22)	//!< 

#define DMD_TITLE_FLAG_DEL_DATA_BACK_TITLE	(1 << 23)	//!< 
#define DMD_TITLE_FLAG_DEL_DATA_BACK_MMENU	(1 << 24)	//!< 
#define DMD_TITLE_FLAG_DEL_DATA_INIT_START	(1 << 25)	//!< 
#define DMD_TITLE_FLAG_DEL_DATA_GAME_END	(1 << 26)	//!<

#if _XBOX
#define DMD_TITLE_FLAG_CHNG_TRIAL_COMP		(1 << 29)	//!<
#endif

#define DME_TITLE_FLAG_NEXT_EVT_TITLE		(1 << 30)	//!<
#define DMD_TITLE_FLAG_SIGN_OUT_EXIT		(1 << 31)	//!<

// 表示フラグ関連
#define DMD_TITLE_DISP_FLAG_WIN_ACT			(1 << 0)	//!< 
#define DMD_TITLE_DISP_FLAG_ALL_ACTION		(1 << 1)	//!< 

// ----- Macro Functions -----------------------------------（処理マクロ定義）


// ----- Definitions -------------------------------------------（定数の宣言）

typedef enum tag_DME_TITLE_DATA_TYPE
{
	DME_TITLE_DATA_TYPE_CMN_DATA = 0,		//!< 共通データ
	DME_TITLE_DATA_TYPE_LANG_DATA,			//!< 言語別データ
	
	DME_TITLE_DATA_TYPE_MAX,
	DME_TITLE_DATA_TYPE_NONE
} DME_TITLE_DATA_TYPE;


typedef enum tag_DME_TITLE_NEXT_EVT
{
	DME_TITLE_NEXT_EVT_MAINGAME_1_1 = 0,		//!< 1-1スタート
	DME_TITLE_NEXT_EVT_STAGESELECT,			//!< ステージ選択
	DME_TITLE_NEXT_EVT_OPTION,			//!< オプション
	DME_TITLE_NEXT_EVT_RANKING,			//!< ランキング
	DME_TITLE_NEXT_EVT_LOGO,			//!< ロゴ
	DME_TITLE_NEXT_EVT_TITLE,			//!< タイトル(再起動)
	
	DME_TITLE_NEXT_EVT_MAX,
	DME_TITLE_NEXT_EVT_NONE
} DME_TITLE_NEXT_EVT;


typedef enum tag_DME_TITLE_MENU_TYPE
{
	DME_TITLE_MENU_TYPE_START = 0,	//!< ゲームスタート
	
#if !_WII
	DME_TITLE_MENU_TYPE_TUDUKI,		//!< 続き
#endif
	
	DME_TITLE_MENU_TYPE_OPTION,		//!< オプション
#if !_IPHONE
	DME_TITLE_MENU_TYPE_RANK,		//!< ランキング
#endif
	
#if _PS3
	DME_TITLE_MENU_TYPE_BUY,		//!< 製品版購入
#elif _PC || _XBOX
	DME_TITLE_MENU_TYPE_ZISSEKI,	//!< 実績
	DME_TITLE_MENU_TYPE_LIBRARY,	//!< ライブラリに戻る
	DME_TITLE_MENU_TYPE_BUY,		//!< 完全版購入
#endif
	
	DME_TITLE_MENU_TYPE_NUM,
	DME_TITLE_MENU_TYPE_NONE
} DME_TITLE_MENU_TYPE;


typedef enum tag_DME_TITLE_WIN_TYPE
{
	DME_TITLE_WIN_TYPE_DEL_DATA1 = 0,	//!< セーブ削除確認メッセージ１
	DME_TITLE_WIN_TYPE_DEL_DATA2,		//!< セーブ削除確認メッセージ２
	DME_TITLE_WIN_TYPE_DEL_DATA3,		//!< セーブ無効時のセーブ削除チェック
	DME_TITLE_WIN_TYPE_DEL_DATA4,		//!< セーブ無効時のセーブ削除チェック
	DME_TITLE_WIN_TYPE_DEL_DATA5,		//!< セーブ無効時のデータ損失チェック(ゲーム終了時)
	
	DME_TITLE_WIN_TYPE_MAX,
	DME_TITLE_WIN_TYPE_NONE
} DME_TITLE_WIN_TYPE;


//! アクションテーブル
typedef enum tag_DME_TITLE_ACT
{
	// 言語共通
#if !_IPHONE
	ACT_MENU_CRSR = 0,
#else //!_IPHONE
	ACT_BTN_L = 0,		//<選択肢台座
	ACT_BTN_C,			//<選択肢台座
	ACT_BTN_R,			//<選択肢台座
	ACT_BTN_L2,			//<選択肢台座
	ACT_BTN_C2,			//<選択肢台座
	ACT_BTN_R2,			//<選択肢台座
	ACT_BTN_L3,			//<選択肢台座
	ACT_BTN_C3,			//<選択肢台座
	ACT_BTN_R3,			//<選択肢台座
	ACT_BACK_BTN_L,		//<戻る台紙
	ACT_BACK_BTN_R,		//<戻る台紙
	ACT_GAME_BTN_L,		//<他のゲーム台紙
	ACT_GAME_BTN_R,		//<他のゲーム台紙
#endif //!_IPHONE

#if _PC || _XBOX || _PS3
//	ACT_WIN_LINE,
#endif

#if _PC || _XBOX
//	ACT_BTN_BUY,
#endif

#if _PC || _XBOX
	ACT_TEX_START,
	ACT_TEX_GAME,
	ACT_TEX_GAME_L,
	ACT_TEX_TUDUKI,
	ACT_TEX_TUDUKI_L,
	ACT_TEX_OPTION,
	ACT_TEX_OPTION_L,
	ACT_TEX_RANK,
	ACT_TEX_RANK_L,
	ACT_TEX_ZISSEKI,
	ACT_TEX_ZISSEKI_L,
	ACT_TEX_LIBRARY,
	ACT_TEX_LIBRARY_L,
	ACT_TEX_BUY,
	ACT_TEX_BUY_L,
	ACT_WIN_TEX_MSG1,
	ACT_WIN_TEX_MSG2,
	ACT_WIN_TEX_MSG3,
	
#elif _PS3
	ACT_TEX_START,
	ACT_TEX_GAME,
	ACT_TEX_GAME_L,
	ACT_TEX_TUDUKI,
	ACT_TEX_TUDUKI_L,
	ACT_TEX_OPTION,
	ACT_TEX_OPTION_L,
	ACT_TEX_RANK,
	ACT_TEX_RANK_L,
	ACT_TEX_ZISSEKI,
	ACT_TEX_ZISSEKI_L,
	ACT_WIN_TEX_MSG1,
	ACT_WIN_TEX_MSG2,
	ACT_WIN_TEX_MSG3,
	
#elif _IPHONE
	ACT_TEX_START,
	ACT_TEX_GAME,
	ACT_TEX_TUDUKI,
	ACT_TEX_OPTION,
	ACT_TEX_KANZENBAN,
	ACT_TEX_TOP_BACK,
	ACT_TEX_TOP_GAME,
	ACT_WIN_TEX_MSG1,
	ACT_WIN_TEX_MSG2,

#else
	ACT_TEX_START,
	ACT_TEX_GAME,
	ACT_TEX_GAME_L,
	ACT_TEX_OPTION,
	ACT_TEX_OPTION_L,
	ACT_TEX_RANK,
	ACT_TEX_RANK_L,
#endif


	// メニュー共通データ
	ACT_BTN_CANCEL,			//!< 
	ACT_BTN_X,				//!< 
	
#if !_IPHONE
	ACT_WIN_LINE,			//!< 
	
	ACT_TEX_WINTITLE,		//!< 
#else //!_IPHONE
	ACT_WIN_NO_BTN_L,		//<ウインドウ・いいえ台座
	ACT_WIN_NO_BTN_C,		//<ウインドウ・いいえ台座
	ACT_WIN_NO_BTN_R,		//<ウインドウ・いいえ台座
	ACT_WIN_YES_BTN_L,		//<ウインドウ・はい台座
	ACT_WIN_YES_BTN_C,		//<ウインドウ・はい台座
	ACT_WIN_YES_BTN_R,		//<ウインドウ・はい台座
#endif //!_IPHONE
	ACT_TEX_YES,			//!< 
	ACT_TEX_NO,				//!< 
	ACT_TEX_BACK,			//!< 
#if !_IPHONE
	ACT_TEX_OK,				//!< 
#endif //!_IPHONE
	
	
	ACT_NUM,
	
	ACT_LANG = ACT_TEX_START,
	
	ACT_NONE
} DME_TITLE_ACT;



typedef struct tag_DMS_TITLE_MAIN_WORK	DMS_TITLE_MAIN_WORK;

//! メインタスクワーク
struct tag_DMS_TITLE_MAIN_WORK {
	
	AMS_FS			*arc_cmn_amb_fs[4];					//!< 共通アーカイブAMBファイル
	void			*arc_cmn_amb[4];					//!< 共通アーカイブAMBファイル
	void			*cmn_ama[4];						//!< AMAファイル
	void			*cmn_amb[4];						//!< AMBファイル
	AOS_TEXTURE		cmn_tex[4];							//!< メニュー共通テクスチャ
	
#if _PC || _PS3 || _XBOX || _IPHONE
	// 完全版購入画面データワーク
	DMS_BUY_SCR_WORK buy_scr_work;						//!<
#endif //_PC || _PS3 || _XBOX || _IPHONE
	
	AMS_FS			*arc_amb_fs[DME_TITLE_DATA_TYPE_MAX];	//!< アーカイブAMBファイル
	AMS_FS			*file_arc_amb_fs[2];					//!< 
	AMS_FS			*user_arc_amb_fs[2];					//!< 
	AMS_FS			*cmn_win_amb_fs;						//!< 
	AMS_FS			*win_amb_fs;							//!< 
	
	void			*arc_amb[DME_TITLE_DATA_TYPE_MAX];	//!< アーカイブAMBファイル
	void			*file_arc_amb[2];					//!< 
	void			*user_arc_amb[2];					//!< 
	void			*cmn_win_amb;						//!< 
	void			*win_amb;
	
	void			*ama[DME_TITLE_DATA_TYPE_MAX];		//!< AMAファイル
	void			*amb[DME_TITLE_DATA_TYPE_MAX];		//!< AMBファイル
	
	AOS_TEXTURE		tex[DME_TITLE_DATA_TYPE_MAX];		//!< テクスチャ
	AOS_TEXTURE		win_tex;							//!< ウインドウ用テクスチャ
	
	AOS_ACTION		*act[ACT_NUM];						//!< アクション
	
	void (*proc_input)(DMS_TITLE_MAIN_WORK *);		//!< 入力処理関数
	void (*proc_update)(DMS_TITLE_MAIN_WORK *);		//!< 更新処理関数
	void (*proc_win_input)(DMS_TITLE_MAIN_WORK *);
	void (*proc_win_update)(DMS_TITLE_MAIN_WORK *);	//!< 更新処理関数
	void (*proc_draw)(DMS_TITLE_MAIN_WORK *);		//!< 表示処理関数
	
	float timer;									//!< 汎用タイマー
	float disp_timer;								//!< 表示切替用タイマー
	float win_timer;								//!< 
	float mmenu_win_timer;							//!< 
	u32	flag;										//!< 汎用フラグ
	s32 disp_change_time;							//!< 表示切替時間
	u32 announce_flag;								//!< ウインドウメッセージフラグ
	s32 win_mode;									//!< 
	
	s32 cur_slct_menu;								//!< 選択中のメニュー項目
	s32 prev_slct_menu;								//!< 
	
	s32 win_cur_slct;								//!< 
	
	s32 next_evt;									//!< 次のイベント
	BOOL is_init_play;								//!< 
	BOOL is_jp_region;								//!< 
	BOOL is_no_save_data;							//!< 
	
	float cur_crsr_pos_y;							//!< 現在のカーソル表示位置Y
	float src_crsr_pos_y;							//!< 一つ前のカーソル表示位置Y
	float dst_crsr_pos_y;							//!< 一つ前のカーソル表示位置Y
	
	float decide_menu_frm[DME_TITLE_MENU_TYPE_NUM];	//!< 
	s32 slct_menu_num;								//!< 
	
	float mmenu_win_size_rate[2];								//!< 
	float win_size_rate[2];								//!< 
	u32 disp_flag;

#if _IPHONE
	er::CTrgAoAction	trg_slct[3];	//<選択肢トリガ
	er::CTrgAoAction	trg_answer[2];	//<ウインドウ/はい・いいえ用の判定
	er::CTrgAoAction	trg_return;		//<戻るトリガ
#ifndef SONIC4_TRIAL_EXIBITION	
	er::CTrgAoAction	trg_game;		//<他のゲームトリガ
#endif
	u32					flag_prev;		//<1f前のフラグ状況
#endif //_IPHONE
};


// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static void dmTitleInit(void);
static void dmTitleProcMain(MTS_TASK_TCB *tcb);
static void dmTitleDest(MTS_TASK_TCB *tcb);

static void dmTitleSetInitDispData(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleSetNextEvent(DMS_TITLE_MAIN_WORK *main_work);

static void dmTitleLoadFontData(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleIsLoadFontData(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleLoadRequest(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcLoadWait(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcTexBuildWait(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcCheckLoadingEnd(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcFadeIn(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcWaitInput(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcDecideEfct(DMS_TITLE_MAIN_WORK *main_work);

static void dmTitleProcMainMenuOpenWin(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcMainMenuCloseWin(DMS_TITLE_MAIN_WORK *main_work);

static void dmTitleProcMainMenuIdle(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcMainMenuDecideEfct(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcFadeOut(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcDataRelease(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcFinish(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcWaitFinished(DMS_TITLE_MAIN_WORK *main_work);

static void dmTitleProcFileSlctWaitDataLoad(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcFileSlctWaitDataSave(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcFileSlctSaveStartWait(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcCheckTrialIdle(DMS_TITLE_MAIN_WORK *main_work);

static void mppDT_dmTitleForceLoadSavedGame(DMS_TITLE_MAIN_WORK *main_work, int stage_id);//qqq

#if _WII
static void dmTitleProcFileSlctIdle(DMS_TITLE_MAIN_WORK *main_work);
#endif

#if _PS3 || _XBOX || _IPHONE
static void dmTitleProcMainMenuDelSaveWin(DMS_TITLE_MAIN_WORK *main_work);
#endif //_PS3 || _XBOX || _IPHONE
#if _PS3 || _XBOX || _IPHONE// || _PC
static void dmTitleProcMainMenuCompBuyFadeOut(DMS_TITLE_MAIN_WORK *main_work);
#endif //_PS3 || _XBOX || _IPHONE// || _PC
#if _PS3 || _XBOX// || _PC
static void dmTitleProcNoSaveCheckInitStart(DMS_TITLE_MAIN_WORK *main_work);
#endif //_PS3 || _XBOX// || _PC

#if _PC || _PS3 || _XBOX || _IPHONE
static void dmTitleProcMainMenuCompBuyIdle(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcMainMenuCompBuyFadeIn(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcSaveInitData(DMS_TITLE_MAIN_WORK *main_work);
#endif //_PC || _PS3 || _XBOX || _IPHONE
#if _PC || _PS3 || _XBOX
static void dmTitleProcNoSaveCheckDelData(DMS_TITLE_MAIN_WORK *main_work);
#endif

#if _XBOX
static void dmTitleProcPreFadeTrialCheck(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcPreFadeCheckTrialToComp(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcMainMenuCheckTrialToComp(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleSetMainMenuTrialToComp(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcNoSaveCheckDelEndGame(DMS_TITLE_MAIN_WORK *main_work);
#endif

#if _XBOX
static void dmTitleProcGameExitIdle(DMS_TITLE_MAIN_WORK *main_work);
#endif

#if _PC || _PS3 || _XBOX || _IPHONE
static void dmTitleProcWindowNodispIdle(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcWindowOpenEfct(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcWindowAnnounceIdle(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleProcWindowCloseEfct(DMS_TITLE_MAIN_WORK *main_work);

static void dmTitleInputProcWindow(DMS_TITLE_MAIN_WORK *main_work);
#endif //_PC || _PS3 || _XBOX || _IPHONE

static void dmTitleInputProcTitle(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleInputProcMainMenu(DMS_TITLE_MAIN_WORK *main_work);

static void dmTitleDrawSetProcDispData(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleDrawProcTitle(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleDrawProcMainMenu(DMS_TITLE_MAIN_WORK *main_work);
#if !_IPHONE
static void dmTitleDrawProcMainMenuTrial(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleDrawProcMainMenuFirst(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleDrawProcMainMenuPlayed(DMS_TITLE_MAIN_WORK *main_work);
#else //!_IPHONE
static void dmTitleDrawProcMainMenuIphone(DMS_TITLE_MAIN_WORK *main_work, bool has_save_data, bool is_trial);
#endif //!_IPHONE

#if _PC || _PS3 || _XBOX || _IPHONE
static void dmTitleWinSelectDraw(DMS_TITLE_MAIN_WORK *main_work);
#endif //_PC || _PS3 || _XBOX || _IPHONE

static void dmTitleTaskDraw(AMS_TCB* tcb);

static BOOL dmTitleIsDataLoad(DMS_TITLE_MAIN_WORK *main_work);
static BOOL dmTitleIsTexLoad(DMS_TITLE_MAIN_WORK *main_work);
static BOOL dmTitleIsTexRelease(DMS_TITLE_MAIN_WORK *main_work);

static void dmTitleSetChngFocusCrsrData(DMS_TITLE_MAIN_WORK *main_work);
static s32 dmTitleGetRevisedMenuFocus(s32 idx, s32 diff, s32 menu_num);

static void dmTitleSetCtrlFocusChangeEfct(DMS_TITLE_MAIN_WORK *main_work);
static BOOL dmTitleIsCtrlFocusChangeEfctEnd(DMS_TITLE_MAIN_WORK *main_work);

static void dmTitleSetWinOpenEfct(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleSetWinCloseEfct(DMS_TITLE_MAIN_WORK *main_work);

static void dmTitleSetMMenuWinOpenEfct(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleSetMMenuWinCloseEfct(DMS_TITLE_MAIN_WORK *main_work);

static void dmTitleSetLoadSysData(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleSetInitSysData(DMS_TITLE_MAIN_WORK *main_work);

static void dmTitleSetMenuInfo(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleSetInitSaveData(DMS_TITLE_MAIN_WORK *main_work);
static void dmTitleSetFirstPlayData(DMS_TITLE_MAIN_WORK *main_work);

#if _PC || _PS3 || _XBOX || _IPHONE
static BOOL dmTitleIsSaveRunning(void);
static BOOL dmTitleIsChangeOptVol(void);
#endif //_PC || _PS3 || _XBOX || _IPHONE

// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ----- Static Variables --------------------（スタティック変数の定義：局所）

// 各国別AMBファイルパステーブル
const static char *dm_title_file_lng_amb_name_tbl[GSD_LANGUAGE_NUM] = {
	GSS_BASE_PATH"DEMO/TITLE/D_TITLE_JP.AMB",
	GSS_BASE_PATH"DEMO/TITLE/D_TITLE_US.AMB",
	GSS_BASE_PATH"DEMO/TITLE/D_TITLE_FR.AMB",
	GSS_BASE_PATH"DEMO/TITLE/D_TITLE_IT.AMB",
	GSS_BASE_PATH"DEMO/TITLE/D_TITLE_GE.AMB",
	GSS_BASE_PATH"DEMO/TITLE/D_TITLE_SP.AMB",
};

// メニュー共通AMBファイルパステーブル
const static char *dm_title_menu_cmn_amb_name_tbl[3] = {
	GSS_BASE_PATH"DEMO/CMN/D_CMN_BTN.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_OBI.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_WIN.AMB",
};

// 各国別メニュー共通AMBファイルパステーブル
const static char *dm_title_menu_cmn_lng_amb_name_tbl[GSD_LANGUAGE_NUM] = {
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_JP.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_US.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_FR.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_IT.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_GE.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_SP.AMB",
};

#if _WII
// 各国別AMBファイルパステーブル
const static char *dm_title_fileslct_lng_amb_name_tbl[GSD_LANGUAGE_NUM] = {
	GSS_BASE_PATH"DEMO/FILESLCT/D_FILESLCT_JP.AMB",
	GSS_BASE_PATH"DEMO/FILESLCT/D_FILESLCT_US.AMB",
	GSS_BASE_PATH"DEMO/FILESLCT/D_FILESLCT_FR.AMB",
	GSS_BASE_PATH"DEMO/FILESLCT/D_FILESLCT_IT.AMB",
	GSS_BASE_PATH"DEMO/FILESLCT/D_FILESLCT_GE.AMB",
	GSS_BASE_PATH"DEMO/FILESLCT/D_FILESLCT_SP.AMB",
};

// 各国別AMBファイルパステーブル
const static char *dm_title_user_lng_amb_name_tbl[GSD_LANGUAGE_NUM] = {
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER_JP.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER_US.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER_FR.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER_IT.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER_GE.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER_SP.AMB",
};
#endif // #if _WII


const static float dm_title_crsr_pos_y_tbl[2] = {
#if _PC || _XBOX
	372.f,		// トップ項目の高さ
	50.f,		// 一つ一つの項目の高さ
#elif _PS3
	422.f,		// トップ項目の高さ
	50.f,		// 一つ一つの項目の高さ
#elif _WII
	445.f,		// トップ項目の高さ
	50.f,		// 一つ一つの項目の高さ
#else
	372.f,		// トップ項目の高さ
	50.f,		// 一つ一つの項目の高さ
#endif
};


const static float dm_title_crsr_trial_pos_y_tbl[DME_TITLE_MENU_TYPE_NUM] = {
#if _PC || _XBOX
	422.f,	// はじめ
//	422.f,	// つづき
	472.f,	// オプション
	522.f,	// ランキング
	572.f,	// 実績
	622.f,	// ゲームライブラリへ戻る
	372.f,	// 完全版
#elif _PS3
	472.f,	// はじめ
//	422.f,	// つづき
	522.f,	// オプション
	572.f,	// ランキング
	422.f,	// 製品版
#else // _WII
	445.f,	// GAME START
	495.f,	// オプション
	545.f,	// ランキング
#endif
};

const static float dm_title_win_act_pos_tbl[7][2] = {
//	{0.0f, 0.0f},		// ウインドウ背景
	{DMD_TITLE_SIZE_HALF_WIDTH + 42.f, 280.0f},		// ウインドウ内のライン
#if !_IPHONE
	{DMD_TITLE_SIZE_HALF_WIDTH, 336.0f},			// メッセージ
	{DMD_TITLE_SIZE_HALF_WIDTH, 336.0f},			// メッセージ
	{DMD_TITLE_SIZE_HALF_WIDTH, 336.0f},			// メッセージ
	{DMD_TITLE_SIZE_HALF_WIDTH - 80.f, 274.0f},	// タイトルテキスト
	{DMD_TITLE_SIZE_HALF_WIDTH - 88.f, 420.0f},		// YES
	{DMD_TITLE_SIZE_HALF_WIDTH + 88.f, 420.0f},		// NO
#else //!_IPHONE
	{DMD_TITLE_SIZE_HALF_WIDTH, 336.0f - 60.0f},	// メッセージ
	{DMD_TITLE_SIZE_HALF_WIDTH, 336.0f - 60.0f},	// メッセージ
	{DMD_TITLE_SIZE_HALF_WIDTH, 336.0f - 60.0f},	// メッセージ
	{DMD_TITLE_SIZE_HALF_WIDTH - 80.f, 274.0f},		// タイトルテキスト
	{DMD_TITLE_SIZE_HALF_WIDTH - 176.f, 420.0f},	// YES
	{DMD_TITLE_SIZE_HALF_WIDTH + 176.f, 420.0f},	// NO
#endif //!_IPHONE
//	{DMD_TITLE_SIZE_HALF_WIDTH + 112.f, 264.0f},	// キャンセルボタン
//	{DMD_TITLE_SIZE_HALF_WIDTH + 158.f, 268.0f},	// 戻るテキスト
};


// アクションIDテーブル(初期状態)
const static u32 g_dm_act_id_tbl[ACT_NUM] = {
#if !_IPHONE
	IDA_D_TITLE_ACT_CURSOR,
#else //!_IPHONE
	IDA_D_TITLE_ACT_BTN_L,		//<選択肢台座
	IDA_D_TITLE_ACT_BTN_C,		//<選択肢台座
	IDA_D_TITLE_ACT_BTN_R,		//<選択肢台座
	IDA_D_TITLE_ACT_BTN_L,		//<選択肢台座
	IDA_D_TITLE_ACT_BTN_C,		//<選択肢台座
	IDA_D_TITLE_ACT_BTN_R,		//<選択肢台座
	IDA_D_TITLE_ACT_BTN_L,		//<選択肢台座
	IDA_D_TITLE_ACT_BTN_C,		//<選択肢台座
	IDA_D_TITLE_ACT_BTN_R,		//<選択肢台座
	IDA_D_TITLE_ACT_BTN02_L,	//<戻る台紙
	IDA_D_TITLE_ACT_BTN02_C,	//<戻る台紙
	IDA_D_TITLE_ACT_GAME_BTN_L,	//<他のゲーム台紙
	IDA_D_TITLE_ACT_GAME_BTN_C,	//<他のゲーム台紙
#endif //!_IPHONE

#if _PC || _XBOX || _PS3
//	IDA_D_TITLE_ACT_WIN_LINE,
#endif

#if _PC || _XBOX
//	IDA_D_TITLE_ACT_BUTTON,
#endif
	
#if _PC || _XBOX
	IDA_D_TITLE_JP_ACT_TEX_START,
	IDA_D_TITLE_JP_ACT_TEX_HAZIME,
	IDA_D_TITLE_JP_ACT_TEX_HAZIME2,
	IDA_D_TITLE_JP_ACT_TEX_TUZUKI,
	IDA_D_TITLE_JP_ACT_TEX_TUZUKI2,
	IDA_D_TITLE_JP_ACT_TEX_OPTION,
	IDA_D_TITLE_JP_ACT_TEX_OPTION2,
	IDA_D_TITLE_JP_ACT_TEX_RANKING,
	IDA_D_TITLE_JP_ACT_TEX_RANKING2,
	IDA_D_TITLE_JP_ACT_TEX_ZISSEKI,
	IDA_D_TITLE_JP_ACT_TEX_ZISSEKI2,
	IDA_D_TITLE_JP_ACT_TEX_LIBRARY,
	IDA_D_TITLE_JP_ACT_TEX_LIBRARY2,
	IDA_D_TITLE_JP_ACT_TEX_KANZENBAN,
	IDA_D_TITLE_JP_ACT_TEX_KANZENBAN2,
//	IDA_D_TITLE_JP_ACT_TEX_WINTITLE,
	IDA_D_TITLE_JP_ACT_TEX_WIN_MSG1,
	IDA_D_TITLE_JP_ACT_TEX_WIN_MSG2,
	IDA_D_TITLE_JP_ACT_TEX_WIN_MSG3,
//	IDA_D_TITLE_JP_ACT_TEX_HAI,
//	IDA_D_TITLE_JP_ACT_TEX_IIE,
#elif _PS3
	IDA_D_TITLE_JP_ACT_TEX_START,
	IDA_D_TITLE_JP_ACT_TEX_HAZIME,
	IDA_D_TITLE_JP_ACT_TEX_HAZIME2,
	IDA_D_TITLE_JP_ACT_TEX_TUZUKI,
	IDA_D_TITLE_JP_ACT_TEX_TUZUKI2,
	IDA_D_TITLE_JP_ACT_TEX_OPTION,
	IDA_D_TITLE_JP_ACT_TEX_OPTION2,
	IDA_D_TITLE_JP_ACT_TEX_RANKING,
	IDA_D_TITLE_JP_ACT_TEX_RANKING2,
	IDA_D_TITLE_JP_ACT_TEX_SEHIN,
	IDA_D_TITLE_JP_ACT_TEX_SEHIN2,
//	IDA_D_TITLE_JP_ACT_TEX_WINTITLE,
	IDA_D_TITLE_JP_ACT_TEX_WIN_MSG1,
	IDA_D_TITLE_JP_ACT_TEX_WIN_MSG2,
	IDA_D_TITLE_JP_ACT_TEX_WIN_MSG3,
//	IDA_D_TITLE_JP_ACT_TEX_HAI,
//	IDA_D_TITLE_JP_ACT_TEX_IIE,
#elif _IPHONE
	IDA_D_TITLE_JP_ACT_TEX_START,
	IDA_D_TITLE_JP_ACT_TEX_HAZIME,
	IDA_D_TITLE_JP_ACT_TEX_TUZUKI,
	IDA_D_TITLE_JP_ACT_TEX_OPTION,
	IDA_D_TITLE_JP_ACT_TEX_KANZENBAN,
	IDA_D_TITLE_JP_ACT_TEX_MODORU,
	IDA_D_TITLE_JP_ACT_TEX_GAME,
	IDA_D_TITLE_JP_ACT_TEX_WIN_MSG1,
	IDA_D_TITLE_JP_ACT_TEX_WIN_MSG2,
#else
	IDA_D_TITLE_JP_ACT_TEX_START,
	IDA_D_TITLE_JP_ACT_TEX_GAME,
	IDA_D_TITLE_JP_ACT_TEX_GAME2,
	IDA_D_TITLE_JP_ACT_TEX_OPTION,
	IDA_D_TITLE_JP_ACT_TEX_OPTION2,
	IDA_D_TITLE_JP_ACT_TEX_RANKING,
	IDA_D_TITLE_JP_ACT_TEX_RANKING2,
#endif

	// メニュー共通データ
	IDA_D_CMN_BTN_ACT_BACK_BTN,				//!< 
	IDA_D_CMN_BTN_ACT_BTN_X_TITLE,			//!< 
	
#if !_IPHONE
	IDA_D_CMN_WIN_ACT_WIN_LINE,				//!< 
	
	IDA_D_CMN_MSG_JP_ACT_TEX_WINTITLE,		//!< 
#else //!_IPHONE
	IDA_D_CMN_WIN_ACT_BTN01_L,		//<ウインドウ・いいえ台座
	IDA_D_CMN_WIN_ACT_BTN01_C,		//<ウインドウ・いいえ台座
	IDA_D_CMN_WIN_ACT_BTN01_R,		//<ウインドウ・いいえ台座
	IDA_D_CMN_WIN_ACT_BTN01_L,		//<ウインドウ・はい台座
	IDA_D_CMN_WIN_ACT_BTN01_C,		//<ウインドウ・はい台座
	IDA_D_CMN_WIN_ACT_BTN01_R,		//<ウインドウ・はい台座
#endif //!_IPHONE
	IDA_D_CMN_MSG_JP_ACT_TEX_YES,			//!< 
	IDA_D_CMN_MSG_JP_ACT_TEX_NO,			//!< 
	IDA_D_CMN_MSG_JP_ACT_TEX_BACK,			//!< 
#if !_IPHONE
	IDA_D_CMN_MSG_JP_ACT_TEX_OK,			//!< 
#endif //!_IPHONE
	
};


static BOOL dm_title_is_title_start = FALSE;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// DmTitleStart
/*!
	タイトル画面開始処理
 */
// ==========================================================================
void DmTitleStart(void* arg)
{
	s16 tmp_prev_evt = 0;
	UNREFERENCED_PARAMETER(arg);
	
	// バックアップ初期化
//	GSS_MAIN_SYS_INFO *main_info = GsGetMainSysInfo();
//	GSS_BACKUP *backup = &main_info->backup;
//	backup->Init();
//	GsTrialDebugSetTrial(TRUE);
	
	dm_title_is_title_start = TRUE;
	
	tmp_prev_evt = SyGetEvtInfo()->old_evt_id;
	
	if (dm_title_is_title_start) {
		// バックアップ初期化
		gs::backup::SBackup &backup = gs::backup::SBackup::CreateInstance();
		backup.Init();
	}
	
	dmTitleInit();
}


// ==========================================================================
// DmMainMenuStart
/*!
	メインメニューから開始する処理
 */
// ==========================================================================
void DmMainMenuStart(void* arg)
{
	UNREFERENCED_PARAMETER(arg);
	
	dm_title_is_title_start = FALSE;
	
	dmTitleInit();
}




// ----- Static Functions --------------------（スタティック関数の定義：局所）
// ==========================================================================
// dmTitleInit
/*!
	タイトル画面初期化処理
 */
// ==========================================================================
void dmTitleInit(void)
{
	MTS_TASK_TCB		*tcb;
	DMS_TITLE_MAIN_WORK	*main_work;

//	GsTrialDebugSetTrial(TRUE);

	// アクションシステム初期化
	AoActSysSetDrawStateEnable(TRUE);		// どのデモを開始する際も必ず設定
	AoActSysSetDrawState(DMD_TITLE_DRAW_STATE_ID);
	
	// メインタスク作成
	tcb = MTM_TASK_MAKE_TCB(dmTitleProcMain
							, dmTitleDest
							, 0
							, DMD_TITLE_TASK_PAUSELEVEL
							, DMD_TITLE_TASK_PRIO_MAIN
							, DMD_TITLE_TASK_GROUP_MAIN
							, sizeof(DMS_TITLE_MAIN_WORK)
							, "TITLE_MAIN"
							);
	
	// ワーク初期化
	main_work = (DMS_TITLE_MAIN_WORK *)mtTaskGetTcbWork(tcb);
	
	// 初期化処理があればここに記述
	dmTitleSetInitDispData(main_work);

	// タイトルから開始した場合のみこれを呼び出す
	if (dm_title_is_title_start) {
		AoAccountClearCurrentId();
	}
	
	// 初期化処理があればここに記述
	// リージョンデータ取得(ボタン表示切り替え用)
	if (GeEnvGetDecideKey() == GSD_DECIDE_KEY_O) {
		main_work->is_jp_region = TRUE;
	}
	else {
		main_work->is_jp_region = FALSE;
	}
	
	// 現状、仮でローディング画面表示
//	DmLoadingStart();
	
	// プロシージャ設定
	main_work->proc_update = dmTitleLoadFontData;
}



// ==========================================================================
// dmTitleSetInitDispData
/*!
	表示物初期化処理
 */
// ==========================================================================
void dmTitleSetInitDispData(DMS_TITLE_MAIN_WORK *main_work)
{
	s16 tmp_prev_evt = 0;
	
	tmp_prev_evt = SyGetEvtInfo()->old_evt_id;
	
	switch (tmp_prev_evt) {
	case GSD_EVT_ID_MAINGAME:
		main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_START;
		break;
	case GSD_EVT_ID_MAP:
#if !_WII
		main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_TUDUKI;
#else
		main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_START;
#endif
		
		break;
#if !_IPHONE
	case GSD_EVT_ID_RANKING:
		if (GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_1_1) == FALSE
			|| GsTrialIsTrial()) {
#if !_WII
			main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_RANK - 1;
#else
			main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_RANK;
#endif
		}
		else {
			main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_RANK;
		}
		
		break;
#endif //!_IPHONE
	case GSD_EVT_ID_OPTION:
		if (GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_1_1) == FALSE
			|| GsTrialIsTrial()) {
#if !_WII
			main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_OPTION - 1;
#else
			main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_OPTION;
#endif
		}
		else {
			main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_OPTION;
		}
		
		break;
	case GSD_EVT_ID_STAFFROLL:
#if !_WII
		main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_TUDUKI;
#else
		main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_START;
#endif
		
		break;
	default:
		main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_START;
		break;
	}
	
	// 体験版でゲームプレイ後は必ずカーソルを完全版購入へ
#if _PC || _XBOX || _PS3
	if (tmp_prev_evt == GSD_EVT_ID_BUYSCREEN
		&& GsTrialIsTrial()) {
		main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_BUY - 1;
	}
#endif
	
	
	// タイトル全体表示フラグON(これがOFFになるときは完全版購入画面の表示時のみ)
	main_work->disp_flag |= DMD_TITLE_DISP_FLAG_ALL_ACTION;

	// カーソルを初期表示位置に設定
	if (GsTrialIsTrial()) {
		main_work->cur_crsr_pos_y = dm_title_crsr_trial_pos_y_tbl[main_work->cur_slct_menu];
	}
	else {
		main_work->cur_crsr_pos_y = main_work->cur_slct_menu * dm_title_crsr_pos_y_tbl[1]
								   		+ dm_title_crsr_pos_y_tbl[0];
	}
	
}



// ==========================================================================
// dmTitleProcMain
/*!
	タイトル画面メインプロシージャ処理
 */
// ==========================================================================
void dmTitleProcMain(MTS_TASK_TCB *tcb)
{
	DMS_TITLE_MAIN_WORK	*main_work;

	// ワーク取得
	main_work = (DMS_TITLE_MAIN_WORK *)mtTaskGetTcbWork(tcb);

	
#if _IPHONE
	main_work->flag_prev = main_work->flag;
#endif //_IPHONE
	
	static bool needToCheckOnly1Time = true;
	//if(main_work->proc_update == dmTitleProcCheckTrialIdle && needToCheckOnly1Time) {
	//if(main_work->proc_update == dmTitleProcWaitInput && needToCheckOnly1Time) {
	if(main_work->proc_update == dmTitleProcFadeIn && needToCheckOnly1Time) {
	//if(main_work->proc_update == dmTitleProcCheckLoadingEnd && needToCheckOnly1Time) {	
		needToCheckOnly1Time = false;
		if(mppCheckPointStorage::isNeedToLoadSavedGame()>0) {//qqq
			main_work->cur_slct_menu = +111; //special selector
			//main_work->flag |= DMD_TITLE_FLAG_SIGN_OUT_EXIT;
			
			/*{{ 
				main_work->proc_update = dmTitleProcFadeOut;
				main_work->proc_input = NULL;
				
				// フェードアウト開始
				IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
							   , IZE_FADE_TYPE_BLACK_FADEOUT
							   , DMD_TITLE_FADEOUT_TIME
							   );
			}}*/
			
			{{
				// オープニング終了処理
				DmTitleOpExit();
				
				// 次へ遷移
				main_work->proc_update = dmTitleProcDataRelease;
				main_work->proc_draw = NULL;
			}}
			//
			main_work->timer = 0;
			
			// 決定演出フレームを元に戻す
			for (int i = 0; i < DME_TITLE_MENU_TYPE_NUM; i++) {
				main_work->decide_menu_frm[i] = 0;
			}
		
			
			DmSndBgmPlayerBgmStop();
			 DmSndBgmPlayerExit();
			 main_work->flag |= DMD_TITLE_FLAG_DEMO_SND_END;	
			
			//------------------------------ (posle perenosa potrebovalis takie deystviya)
			//additional 1:
			int pad_id = 0;
			AoAccountSetCurrentIdStart((u16)pad_id);
			//additional 2:
#ifndef SONIC4_TRIAL			
			DmSaveStart(1 << DME_SAVE_WIN_DATA_LOADING, FALSE);
#endif
			 
		}	
	}
	
	// 終了処理
	if (main_work->flag & DMD_TITLE_FLAG_EXIT) {
		// タスククリア
		mtTaskClearTcb(tcb);
		
		// 次のイベント設定
		if(main_work->cur_slct_menu == +111) {//qqq
			main_work->cur_slct_menu = 0;
			MPP_CHECKPOINT_STATE_HEADER s_hdr;
			mppCheckPointStorage::loadStateHeader(&s_hdr);
			//dmTitleSetNextEvent(main_work); //test
			mppDT_dmTitleForceLoadSavedGame(main_work, s_hdr.stage_id);
		}
		else {
			dmTitleSetNextEvent(main_work);
		}
	}

	
	
	// システム関連処理(サインアウト時はタイトルへ戻す)
	if (main_work->flag & DMD_TITLE_FLAG_SIGN_OUT_EXIT
		&& !AoAccountIsCurrentEnable()) {
		
		main_work->proc_update = dmTitleProcFadeOut;
		
		// サインアウト終了フラグOFF
		main_work->flag &= ~DMD_TITLE_FLAG_SIGN_OUT_EXIT;
		main_work->flag |= DME_TITLE_FLAG_NEXT_EVT_TITLE;
		
		// フェード終了		※問題あれば有効にする
//		IzFadeExit();
		
		// フェードアウト開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_TAKEOEVER
					   , IZE_FADE_TYPE_BLACK_FADEOUT
					   , DMD_TITLE_FADEOUT_TIME
					   );
		
		// BGMフェードアウト開始
		DmSndBgmPlayerExit();
		main_work->flag |= DMD_TITLE_FLAG_DEMO_SND_END;
		
		// ウインドウ遷移関連初期化設定
		main_work->flag &= ~DMD_TITLE_FLAG_DECIDE;
		main_work->flag &= ~DMD_TITLE_FLAG_CANCEL;
//		main_work->proc_win_update = NULL;	// 必要ならコメントをはずす
		main_work->proc_input = NULL;
		main_work->proc_win_input = NULL;
		main_work->win_timer = 0;
		main_work->win_cur_slct = 0;
		main_work->win_mode = DME_TITLE_WIN_TYPE_DEL_DATA1;
	}
	
	
	// ウインドウ更新プロシージャ
	if (main_work->proc_win_update) {
		main_work->proc_win_update(main_work);
	}

	// 更新プロシージャ
	if (main_work->proc_update) {
		main_work->proc_update(main_work);
	}
	
	// 描画設定プロシージャ
	if (main_work->proc_draw) {
		main_work->proc_draw(main_work);
	}
}


// ==========================================================================
// dmTitleDest
/*!
	タイトル画面終了処理
 */
// ==========================================================================
void dmTitleDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	
}



// ==========================================================================
// dmTitleSetNextEvent
/*!
	次のイベントへの遷移設定処理
 */
// ==========================================================================
void dmTitleSetNextEvent(DMS_TITLE_MAIN_WORK *main_work)
{
	s16 next_evt = 0;
	s32 now_slct_menu = 0;
	
	// ログアウト時のイベント遷移
	if (main_work->flag & DME_TITLE_FLAG_NEXT_EVT_TITLE) {
		next_evt = (s16)DME_TITLE_NEXT_EVT_TITLE;
		
		main_work->flag &= ~DME_TITLE_FLAG_NEXT_EVT_TITLE;
		
		// 次のイベント設定
		SyDecideEvtCase(next_evt);
		SyChangeNextEvt();
		
		return;
	}
	
	// タイトルのループ遷移時間経過後の場合
	if (main_work->flag & DMD_TITLE_FLAG_DISP_LOOP_END) {
		next_evt = (s16)DME_TITLE_NEXT_EVT_LOGO;
		
		main_work->flag &= ~DMD_TITLE_FLAG_DISP_LOOP_END;
		
		// 次のイベント設定
		SyDecideEvtCase(next_evt);
		SyChangeNextEvt();
		
		return;
	}
	
	// 体験版・初期データ・データありで項目を切り替える
#if !(_WII || _IPHONE)
	if (main_work->cur_slct_menu > 0) {
		if (GsTrialIsTrial()
			|| main_work->is_init_play) {
			now_slct_menu = main_work->cur_slct_menu + 1;
		}
		else {
			now_slct_menu = main_work->cur_slct_menu;
		}
	}
#else
	now_slct_menu = main_work->cur_slct_menu;
#endif
	
	
	switch (now_slct_menu) {
	case DME_TITLE_MENU_TYPE_START:
#if !_WII
		// もしセーブデータがないデータ、または1-1未クリアならば
		next_evt = (s16)DME_TITLE_NEXT_EVT_MAINGAME_1_1;
		
		// セーブデータ初期化処理
		dmTitleSetFirstPlayData(main_work);
		
		// ゲーム開始時は下記の設定が必須
		// ステージID設定
		g_gs_main_sys_info.stage_id = GSD_MAIN_STAGE_ID_1_1;
		
		// 初期状態のキャラ設定(ノーマルソニック)
		g_gs_main_sys_info.char_id[0] = GSD_CHAR_ID_SONIC;
		
		// ゲームモード設定(スコアかタイムか)
		g_gs_main_sys_info.game_mode = GSD_GAME_MODE_STORY;
		
		// デフォルト残機設定(体験版のみ)
		if (GsTrialIsTrial()) {
			g_gs_main_sys_info.rest_player_num = GSD_MAINSYS_PLAYER_REST_DEF;
		}
		
		// スペステフラグOFF
		g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_SPECIAL_STAGE;
		
		// コントロール設定
#if !_IPHONE
		g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
#endif //!_IPHONE
		
		// ゲーム開始前初期化
		GmMainGSInit();
#else
		// もしセーブデータがないデータ、または1-1未クリアならば
		if (main_work->is_init_play) {
			next_evt = (s16)DME_TITLE_NEXT_EVT_MAINGAME_1_1;
			
			// ゲーム開始時は下記の設定が必須
			// ステージID設定
			g_gs_main_sys_info.stage_id = GSD_MAIN_STAGE_ID_1_1;
			
			// 初期状態のキャラ設定(ノーマルソニック)
			g_gs_main_sys_info.char_id[0] = GSD_CHAR_ID_SONIC;
			
			// ゲームモード設定(スコアかタイムか)
			g_gs_main_sys_info.game_mode = GSD_GAME_MODE_STORY;
			
			// システムデータインスタンス作成
			gs::backup::SSystem &sys_data
				= gs::backup::SSystem::CreateInstance();
			
			// 残機初期化設定
			g_gs_main_sys_info.rest_player_num = GSD_MAINSYS_PLAYER_REST_DEF;
			
			// 残機数設定
			sys_data.SetPlayerStock(g_gs_main_sys_info.rest_player_num);
			
			// コントロール設定
#if !_IPHONE
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
#endif //!_IPHONE
			
			// スペステフラグOFF
			g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_SPECIAL_STAGE;
			
			main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_FIRST;
			
			// ゲーム開始前初期化
			GmMainGSInit();
		}
		
		// それ以外はステセレへ遷移
		else {
			next_evt = (s16)DME_TITLE_NEXT_EVT_STAGESELECT;
		}
#endif
		
		break;
#if !_WII
	case DME_TITLE_MENU_TYPE_TUDUKI:
#if defined(AMD_DEBUG)
#if _IPHONE
		//3点タップにて全解放
		if (amTpIsTouchOn(2)) {
			using namespace gs::backup;
			SStageSolo &z1a1 = SStage::CreateInstance()[EStage::Zone1Act1];
			if (SStageSolo::c_high_score_max_limit == z1a1.GetHighScore(false)) {
				z1a1.SetHighScore(false, 0);
			}
			main_work->is_init_play = FALSE;
		}
#endif //_IPHONE
#endif //defined(AMD_DEBUG)
		// もしセーブデータがないデータ、または1-1未クリアならば
		if (!GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_1_1)
			|| main_work->is_init_play) {
			next_evt = (s16)DME_TITLE_NEXT_EVT_MAINGAME_1_1;
			
			// ゲーム開始時は下記の設定が必須
			// ステージID設定
			g_gs_main_sys_info.stage_id = GSD_MAIN_STAGE_ID_1_1;
			
			// 初期状態のキャラ設定(ノーマルソニック)
			g_gs_main_sys_info.char_id[0] = GSD_CHAR_ID_SONIC;
			
			// ゲームモード設定(スコアかタイムか)
			g_gs_main_sys_info.game_mode = GSD_GAME_MODE_STORY;
			
			// スペステフラグOFF
			g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_SPECIAL_STAGE;
			
			// コントロール設定
#if !_IPHONE
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
#endif //!_IPHONE
			
			// ゲーム開始前初期化
			GmMainGSInit();
		}
		
		// それ以外はステセレへ遷移
		else {
			next_evt = (s16)DME_TITLE_NEXT_EVT_STAGESELECT;
		}
		
		break;
#endif
	case DME_TITLE_MENU_TYPE_OPTION:
		// もしセーブデータがないデータ、または1-1未クリアならば
		next_evt = (s16)DME_TITLE_NEXT_EVT_OPTION;
#if _IPHONE && defined(AMD_DEBUG)
		if (amTpIsTouchOn(2)) {
			SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
			SyChangeNextEvt();
			return;
		}
#endif //_IPHONE && defined(AMD_DEBUG)
		
		break;
#if !_IPHONE
	case DME_TITLE_MENU_TYPE_RANK:
		// もしセーブデータがないデータ、または1-1未クリアならば
		next_evt = (s16)DME_TITLE_NEXT_EVT_RANKING;
		
		break;
#endif //!_IPHONE
	default:
		
		break;
	}
	
	// 次のイベント設定
	SyDecideEvtCase(next_evt);
	SyChangeNextEvt();
}


void mppDT_dmTitleForceLoadSavedGame(DMS_TITLE_MAIN_WORK *main_work, int stage_id_) //qqq -- iphone only
{
	s16 next_evt = 0;
	
	OS_TPrintf("[!!!!!] Try to load saved game for stage_id=%i¥n", stage_id_);
	
	// 体験版・初期データ・データありで項目を切り替える

	
	//as case DME_TITLE_MENU_TYPE_START:
	{

			// もしセーブデータがないデータ、または1-1未クリアならば
			next_evt = (s16)DME_TITLE_NEXT_EVT_MAINGAME_1_1;
			
			// セーブデータ初期化処理
			dmTitleSetFirstPlayData(main_work);
			
			// ゲーム開始時は下記の設定が必須
			// ステージID設定
			g_gs_main_sys_info.stage_id = stage_id_;
			
			// 初期状態のキャラ設定(ノーマルソニック)
			g_gs_main_sys_info.char_id[0] = GSD_CHAR_ID_SONIC;
			
			// ゲームモード設定(スコアかタイムか)
			g_gs_main_sys_info.game_mode = GSD_GAME_MODE_STORY;
			
			// デフォルト残機設定(体験版のみ)
			if (GsTrialIsTrial()) {
				g_gs_main_sys_info.rest_player_num = GSD_MAINSYS_PLAYER_REST_DEF;
			}
			
			// スペステフラグOFF
			g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_SPECIAL_STAGE;
			
			// コントロール設定
/*#if !_IPHONE
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
#endif //!_IPHONE*/
			
			// ゲーム開始前初期化
			GmMainGSInit();
			
	}
	
	// 次のイベント設定
	SyDecideEvtCase(next_evt);
	SyChangeNextEvt();
}



// ==========================================================================
// dmTitleLoadFontData
/*!
	フォントデータ読み込みリクエスト処理
 */
// ==========================================================================
void dmTitleLoadFontData(DMS_TITLE_MAIN_WORK *main_work)
{
	// gsFont構築
	GsFontBuild();
	
	main_work->proc_update = dmTitleIsLoadFontData;
}



// ==========================================================================
// dmTitleIsLoadFontData
/*!
	フォントデータ読み込み終了チェック処理
 */
// ==========================================================================
void dmTitleIsLoadFontData(DMS_TITLE_MAIN_WORK *main_work)
{
	// gsFont構築
	if (GsFontIsBuilded()) {
		main_work->proc_update = dmTitleLoadRequest;
		
		return;
	}
}



// ==========================================================================
// dmTitleLoadRequest
/*!
	ファイル読み込みリクエスト処理
 */
// ==========================================================================
void dmTitleLoadRequest(DMS_TITLE_MAIN_WORK *main_work)
{
	// ファイル読み込み開始
	main_work->arc_amb_fs[0] = amFsReadBackground(GSS_BASE_PATH "DEMO/TITLE/D_TITLE.AMB");
	main_work->arc_amb_fs[1] = amFsReadBackground((char *)dm_title_file_lng_amb_name_tbl[GsEnvGetLanguage()]);
	
	// メニュー共通データ読み込み
	for (int i = 0; i < 3; i++) {
		main_work->arc_cmn_amb_fs[i] = amFsReadBackground((char *)dm_title_menu_cmn_amb_name_tbl[i]);
	}

	main_work->arc_cmn_amb_fs[3] = amFsReadBackground((char *)dm_title_menu_cmn_lng_amb_name_tbl[GsEnvGetLanguage()]);

#if _WII
	// 
	main_work->file_arc_amb_fs[0] = amFsReadBackground(GSS_BASE_PATH "DEMO/FILESLCT/D_FILESLCT.AMB");
	main_work->file_arc_amb_fs[1] = amFsReadBackground((char *)dm_title_fileslct_lng_amb_name_tbl[GsEnvGetLanguage()]);
	
	main_work->user_arc_amb_fs[0] = amFsReadBackground(GSS_BASE_PATH "DEMO/OPTION/D_OPTION_USER.AMB");
	main_work->user_arc_amb_fs[1] = amFsReadBackground((char *)dm_title_user_lng_amb_name_tbl[GsEnvGetLanguage()]);
#endif
//	main_work->cmn_win_amb_fs = amFsReadBackground(GSS_BASE_PATH "DEMO/CMN/D_CMN_WIN.AMB");
//#endif
	
#if _PC || _PS3 || _XBOX || _IPHONE
	DmBuyScreenLoadStart(&main_work->buy_scr_work);
#endif // #if _PC || _PS3 || _XBOX || _IPHONE
	
	// タイトルOPデータロード
	DmTitleOpLoad();
	
	// 次へ遷移
	main_work->proc_update = dmTitleProcLoadWait;
}



// ==========================================================================
// dmTitleProcLoadWait
/*!
	ファイル読み込み待ち処理
 */
// ==========================================================================
void dmTitleProcLoadWait(DMS_TITLE_MAIN_WORK *main_work)
{
	// ファイル読み込み完了待ち
	if (dmTitleIsDataLoad(main_work)) {		// ファイル読込み完了チェック関数にする

		// ファイル取得
		for (int i = 0; i < DME_TITLE_DATA_TYPE_MAX; i++) {
			main_work->arc_amb[i] = (void *)main_work->arc_amb_fs[i]->buf;
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
		for (int i = 0; i < 4; i++) {
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
		
		
#if _WII
		for (int i = 0; i < 2; i++) {
			main_work->file_arc_amb[i] = main_work->file_arc_amb_fs[i]->buf;
			main_work->file_arc_amb_fs[i]->buf = NULL;
			
			// リクエストクリア
			amFsClearRequest(main_work->file_arc_amb_fs[i]);
			main_work->file_arc_amb_fs[i] = NULL;
		}
		
		// ファイル選択へ渡すデータ構造体
		DMS_FILESLCT_DATA_MGR data_mgr;
		amZeroMemory(&data_mgr, sizeof(DMS_FILESLCT_DATA_MGR));
		
		for (int i = 0; i < 4; i++) {
			data_mgr.arc_cmn_amb[i] = main_work->arc_cmn_amb[i];
		}
		
		// ファイル選択構築
		DmFileSlctBuild(main_work->file_arc_amb, &data_mgr);
		
		for (int i = 0; i < 2; i++) {
			main_work->user_arc_amb[i] = main_work->user_arc_amb_fs[i]->buf;
			main_work->user_arc_amb_fs[i]->buf = NULL;
			
			// リクエストクリア
			amFsClearRequest(main_work->user_arc_amb_fs[i]);
			main_work->user_arc_amb_fs[i] = NULL;
		}
		
		DmUserNameBuild(main_work->user_arc_amb);
		
#endif
		
#if _PC || _PS3 || _XBOX || _IPHONE
		// 完全版購入画面テクスチャ構築
		DmBuyScreenBuildStart(&main_work->buy_scr_work);
		
#endif // #if _PC || _PS3 || _XBOX || _IPHONE
		
		// タイトルOPテクスチャ構築
		DmTitleOpBuild();
		
		// サウンド構築
		DmSndBgmPlayerInit();
		
		// 次へ遷移
		main_work->proc_update = dmTitleProcTexBuildWait;
	}
}


// ==========================================================================
// dmTitleProcTexBuildWait
/*!
	テクスチャ構築待ち処理
 */
// ==========================================================================
void dmTitleProcTexBuildWait(DMS_TITLE_MAIN_WORK *main_work)
{
	// テクスチャ構築完了判定
	if (dmTitleIsTexLoad(main_work) == TRUE
		&& DmSndBgmPlayerIsSndSysBuild()) {
		
		for (int i = 0; i < ACT_NUM; i++) {
			const void *ama = NULL;
			AOS_TEXTURE *tex = NULL;

#if _IPHONE
			const int ACT_TEX_WINTITLE = ACT_TEX_YES;
			const int ACT_WIN_LINE = ACT_WIN_NO_BTN_L;
#endif //_IPHONE
			if (i >= ACT_TEX_WINTITLE) {
				ama = main_work->cmn_ama[3];
				tex = &main_work->cmn_tex[3];
			}
			else if (i >= ACT_WIN_LINE) {
				ama = main_work->cmn_ama[2];
				tex = &main_work->cmn_tex[2];
			}
			else if (i >= ACT_BTN_CANCEL) {
				ama = main_work->cmn_ama[0];
				tex = &main_work->cmn_tex[0];
			}
			
			else if (i < ACT_LANG) {
				ama = main_work->ama[DME_TITLE_DATA_TYPE_CMN_DATA];
				tex = &main_work->tex[DME_TITLE_DATA_TYPE_CMN_DATA];
			}
			else {
				ama = main_work->ama[DME_TITLE_DATA_TYPE_LANG_DATA];
				tex = &main_work->tex[DME_TITLE_DATA_TYPE_LANG_DATA];
			}
			
			// 構築
			AoActSetTexture(AoTexGetTexList(tex));
			main_work->act[i] = AoActCreate(ama, g_dm_act_id_tbl[i]);
		}
#if _IPHONE	//当たり判定構築
		{	//選択肢
			const int c_act_id_table[] = {ACT_BTN_C, ACT_BTN_C2, ACT_BTN_C3};
			for (int i = 0, max = arrayof(c_act_id_table); i < max; ++i) {
				er::CTrgAoAction &slct = main_work->trg_slct[i];
				new(&slct) er::CTrgAoAction();
				slct.Create(main_work->act[c_act_id_table[i]]);
			}
		}
		{	//ウインドウ・はい/いいえ
			const int c_act_id_table[] = {ACT_WIN_NO_BTN_C, ACT_WIN_YES_BTN_C};
			for (int i = 0, max = arrayof(c_act_id_table); i < max; ++i) {
				er::CTrgAoAction &answer = main_work->trg_answer[i];
				new(&answer) er::CTrgAoAction();
				answer.Create(main_work->act[c_act_id_table[i]]);
			}
		}
		{	//戻る
			const int c_act_id = ACT_BACK_BTN_R;
			er::CTrgAoAction &ret = main_work->trg_return;
			new(&ret) er::CTrgAoAction();
			ret.Create(main_work->act[c_act_id]);
		}
		{	//他のゲーム
#ifndef SONIC4_TRIAL_EXIBITION				
			const int c_act_id = ACT_GAME_BTN_R;
			er::CTrgAoAction &ret = main_work->trg_game;
			new(&ret) er::CTrgAoAction();
			ret.Create(main_work->act[c_act_id]);
#endif
		}
#endif //_IPHONE	//当たり判定構築
	
		
		main_work->proc_update = dmTitleProcCheckLoadingEnd;
		
		DmTitleOpInit();
		
#if _WII
		// ユーザーネーム側にメニュー共通データを渡す(ボタン、ウインドウ、テキストの順)
		DmUserNameSetMenuCmnAmaData(main_work->cmn_ama[0]
									, main_work->cmn_ama[2]
									, main_work->cmn_ama[3]
									);
		
		DmUserNameSetMenuCmnAmbData(main_work->cmn_amb[0]
									, main_work->cmn_amb[2]
									, main_work->cmn_amb[3]
									);
		
		DmUserNameSetMenuCmnTexData(&main_work->cmn_tex[0]
									, &main_work->cmn_tex[2]
									, &main_work->cmn_tex[3]
									);
#endif
		
		// オープニング側の初期化内にObjInitがあり、そこでSTATE上書きされるので、再度設定
		AoActSysSetDrawState(DMD_TITLE_DRAW_STATE_ID);
		AoActSysSetDrawStateEnable(TRUE);
		
	}
}



// ==========================================================================
// dmTitleProcCheckLoadingEnd
/*!
	ローディング終了待ち処理
 */
// ==========================================================================
void dmTitleProcCheckLoadingEnd(DMS_TITLE_MAIN_WORK *main_work)
{
	// ローディング終了判定
//	if (DmLoadingIsExit()) {
		// タイトルから開始の場合
		if (dm_title_is_title_start) {
			// PRESS〜の初期表示状態
			main_work->disp_change_time = DMD_TITLE_IDLE_DISP_TIME;
			main_work->flag |= DMD_TITLE_FLAG_DISP_TEXT;

			// 次へ遷移
			main_work->proc_update = dmTitleProcFadeIn;
			
			// 描画設定処理
			main_work->proc_draw = dmTitleDrawSetProcDispData;
			
			main_work->flag |= DMD_TITLE_FLAG_DISP_TITLE;
			
			// 権利表示ON
			DmTitleOpDispRightEnable(TRUE);
			
			for (int i = 0; i < 2; i++) {
				main_work->mmenu_win_size_rate[i] = 0.f;
			}
			
			// フェード処理開始
			IzFadeInitEasy(IZE_FADE_SET_TYPE_TAKEOEVER
						   , IZE_FADE_TYPE_BLACK_FADEIN
						   , DMD_TITLE_FADEIN_TIME
						   );
		}
		// メインメニューから開始の場合
		else {
#if _XBOX
			// 次へ遷移
			main_work->proc_update = dmTitleProcPreFadeTrialCheck;
#else
			// 次へ遷移
			main_work->proc_update = dmTitleProcFadeIn;
			
			// 初期メニュー情報設定
			dmTitleSetMenuInfo(main_work);
			
			// ロゴ周りの表示の演出なし設定
			DmTitleOpSetRetOptionState();
			
			// 描画設定処理
			main_work->proc_draw = dmTitleDrawSetProcDispData;
			
			mppUtil::showCommunityButton(true);//kolya
			main_work->flag |= DMD_TITLE_FLAG_DISP_MAINMENU;
			
			main_work->flag |= DMD_TITLE_FLAG_DISP_MMENU_WIN;
			
			// 権利表示OFF
			DmTitleOpDispRightEnable(FALSE);
#endif
			for (int i = 0; i < 2; i++) {
				main_work->mmenu_win_size_rate[i] = 1.f;
			}
		}
//	}
}


#if _XBOX
// ==========================================================================
// dmTitleProcPreFadeTrialCheck
/*!
	フェードイン中処理
 */
// ==========================================================================
void dmTitleProcPreFadeTrialCheck(DMS_TITLE_MAIN_WORK *main_work)
{
	s16 tmp_prev_evt = 0;
	
	tmp_prev_evt = SyGetEvtInfo()->old_evt_id;
	
	if (GsTrialIsTrial() && !GsTrialIsTrialDirect()) {
		DmSaveMenuStart();
		
		// ここで完全版に変更するための必要な設定を行う
		
		if (GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_1_1)) {
			// 初プレイフラグOFF
			main_work->is_init_play = FALSE;
			
			// 初期表示項目設定
			main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_PLAYED;
			
			if (tmp_prev_evt == GSD_EVT_ID_RANKING) {
				main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_RANK;
			}
			else {
				// カーソルを「つづきから」の表示位置に設定
				main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_TUDUKI;
			}
		}
		
		else {
			main_work->is_init_play = TRUE;
			
			// 初期表示項目設定
			main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_FIRST;
			
			if (tmp_prev_evt == GSD_EVT_ID_RANKING) {
				main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_RANK - 1;
			}
			else {
				// カーソルを「初めから」の表示位置に設定
				main_work->cur_slct_menu = 0;
			}
		}
		
		if (GsTrialIsTrialDirect()) {
			// 体験版表示
			main_work->cur_crsr_pos_y = dm_title_crsr_trial_pos_y_tbl[main_work->cur_slct_menu];
		}
		else {
			// 製品版表示
			main_work->cur_crsr_pos_y = main_work->cur_slct_menu * dm_title_crsr_pos_y_tbl[1]
								   		+ dm_title_crsr_pos_y_tbl[0];
		}
		
		// セーブ終了待ちへ遷移
		main_work->proc_update = dmTitleProcPreFadeCheckTrialToComp;
	}
	else {
		// 次へ遷移
		main_work->proc_update = dmTitleProcFadeIn;
		
		// フェード処理開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_TAKEOEVER
					   , IZE_FADE_TYPE_BLACK_FADEIN
					   , DMD_TITLE_FADEIN_TIME
					   );
		
		// 初期メニュー情報設定
		dmTitleSetMenuInfo(main_work);
		
		// ロゴ周りの表示の演出なし設定
		DmTitleOpSetRetOptionState();
		
		// 描画設定処理
		main_work->proc_draw = dmTitleDrawSetProcDispData;
		
		main_work->flag |= DMD_TITLE_FLAG_DISP_MAINMENU;
		
		main_work->flag |= DMD_TITLE_FLAG_DISP_MMENU_WIN;
		
		// 権利表示OFF
		DmTitleOpDispRightEnable(FALSE);
	}
}



// ==========================================================================
// dmTitleProcPreFadeCheckTrialToComp
/*!
	フェードイン中処理
 */
// ==========================================================================
void dmTitleProcPreFadeCheckTrialToComp(DMS_TITLE_MAIN_WORK *main_work)
{
	if (DmSaveIsExit()) {
		// 次へ遷移
		main_work->proc_update = dmTitleProcFadeIn;
		
		// フェード処理開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_TAKEOEVER
					   , IZE_FADE_TYPE_BLACK_FADEIN
					   , DMD_TITLE_FADEIN_TIME
					   );
		
		
		// 初期メニュー情報設定
		dmTitleSetMenuInfo(main_work);
		
		// ロゴ周りの表示の演出なし設定
		DmTitleOpSetRetOptionState();
		
		// 描画設定処理
		main_work->proc_draw = dmTitleDrawSetProcDispData;
		
		main_work->flag |= DMD_TITLE_FLAG_DISP_MAINMENU;
		
		main_work->flag |= DMD_TITLE_FLAG_DISP_MMENU_WIN;
		
		// 権利表示OFF
		DmTitleOpDispRightEnable(FALSE);
	}
}


#endif


// ==========================================================================
// dmTitleProcFadeIn
/*!
	フェードイン中処理
 */
// ==========================================================================
void dmTitleProcFadeIn(DMS_TITLE_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
		IzFadeExit();

		if (dm_title_is_title_start) {
			// 次へ遷移
			main_work->proc_update = dmTitleProcWaitInput;
			main_work->proc_input = dmTitleInputProcTitle;
			
			DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_TITLE);
		}
		else {
			// メインメニューへ
			main_work->proc_update = dmTitleProcMainMenuIdle;
			main_work->proc_input = dmTitleInputProcMainMenu;
			
			main_work->flag |= DMD_TITLE_FLAG_SIGN_OUT_EXIT;
//#ifndef SONIC4_TRIAL			
			DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_MENU);
//#endif
		}
		
#if _PC || _PS3 || _XBOX || _IPHONE
		// ウインドウ処理設定
		main_work->proc_win_update = dmTitleProcWindowNodispIdle;
#endif //_PC || _PS3 || _XBOX || _IPHONE
	}
}


// ==========================================================================
// dmTitleProcWaitInput
/*!
	入力待ち中処理
 */
// ==========================================================================
void dmTitleProcWaitInput(DMS_TITLE_MAIN_WORK *main_work)
{
	// タイマー更新
	main_work->timer++;
	
	if (main_work->timer > DMD_TITLE_PUSH_START_DISP_TIME) {
		main_work->proc_update = dmTitleProcFadeOut;
		main_work->proc_input = NULL;
		
		main_work->timer = 0;
		
		// ロゴへの遷移フラグON
		main_work->flag |= DMD_TITLE_FLAG_DISP_LOOP_END;
		
		// フェードアウト開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEOUT
					   , DMD_TITLE_FADEOUT_TIME
					   );
		
		// BGMフェードアウト開始
		DmSndBgmPlayerExit();
		main_work->flag |= DMD_TITLE_FLAG_DEMO_SND_END;
		
		return;
	}
	
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}
	
	// 終了判定(次のイベント遷移決定)
	if (main_work->flag & DMD_TITLE_FLAG_GAME_START) {
		// 決定演出処理へ
		main_work->proc_update = dmTitleProcDecideEfct;
		main_work->proc_input = NULL;
		
		main_work->timer = 0;
		
		main_work->flag &= ~DMD_TITLE_FLAG_GAME_START;
		
		// クラシック操作		※仮
#if !_IPHONE
		g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
#endif //!_IPHONE
		
		DmSoundPlaySE("Ok");
		
		DmSndBgmPlayerBgmStop();
		
		return;
	}
}


// ==========================================================================
// dmTitleProcDecideEfct
/*!
	決定演出中処理
 */
// ==========================================================================
void dmTitleProcDecideEfct(DMS_TITLE_MAIN_WORK *main_work)
{
	if (main_work->timer >= DMD_TITLE_DECIDE_EFCT_TIME//) {
		&& AoAccountSetCurrentIdIsFinished()) {
		if (AoAccountGetCurrentId() >= 0) {
			// トロフィーデータリセット	※ゲーム始める前に必ず設定
			GsTrophyResetForAccount();
			
			// 体験版開始
			GsTrialCheckStart();
			main_work->flag &= ~DMD_TITLE_FLAG_DISP_TITLE;
			
			main_work->proc_update = dmTitleProcCheckTrialIdle;
			
			// タイトル遷移設定	※問題あれば移動する
			GsRebootSetTitle();
		}
		else {
			main_work->proc_update = dmTitleProcWaitInput;
			main_work->proc_input = dmTitleInputProcTitle;
			
			DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_TITLE);
		}
		
		main_work->timer = 0;
	}
	
	else {
		main_work->timer++;
	}
}



// ==========================================================================
// dmTitleProcCheckTrialIdle
/*!
	体験版チェック中処理
 */
// ==========================================================================
void dmTitleProcCheckTrialIdle(DMS_TITLE_MAIN_WORK *main_work)
{
	if (GsTrialCheckIsFinished()) {
		// 体験版の場合
		if (GsTrialIsTrial()) {
			// 体験版はデータロードをせずにメインメニューへ
			// メインメニューへ
			main_work->proc_update = dmTitleProcMainMenuOpenWin;
			main_work->proc_input = NULL;
			
			main_work->flag |= DMD_TITLE_FLAG_SIGN_OUT_EXIT;
			
			main_work->flag |= DMD_TITLE_FLAG_EXIST_SAVE_DATA;
			
			main_work->flag |= DMD_TITLE_FLAG_DISP_MMENU_WIN;
			
			// 初プレイフラグOFF
			main_work->is_init_play = FALSE;
			
			// 初期表示項目設定
			main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_TRIAL;
			
			// カーソルを「はじめから」の表示位置に設定
			main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_START;
			main_work->cur_crsr_pos_y = dm_title_crsr_trial_pos_y_tbl[main_work->cur_slct_menu];
			
			// メニューBGM再生開始
			DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_MENU);
		}
		// 製品版の場合
		else {
			// アカウントが確定したため、ロード開始
			DmSaveStart(1 << DME_SAVE_WIN_DATA_LOADING, FALSE);
			
			main_work->proc_update = dmTitleProcFileSlctWaitDataLoad;
			
			// メニューBGM再生開始
			DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_MENU);
		}
	}
}



// ==========================================================================
// dmTitleProcFileSlctWaitDataLoad
/*!
	ファイルロード待ち中処理
 */
// ==========================================================================
void dmTitleProcFileSlctWaitDataLoad(DMS_TITLE_MAIN_WORK *main_work)
{
	AOE_STORAGE_ERROR is_error = AOD_STORAGE_ERROR_NONE;
	
	// ファイルロードが完了したら
	if (DmSaveIsExit()) {
		if (DmCmnBackupIsLoadSuccessed()) {
#if _WII
			// セーブファイル選択へ
			main_work->proc_update = dmTitleProcFileSlctIdle;
			main_work->proc_input = NULL;
			
			// 初期表示項目設定
			main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_PLAYED;
			
			// セーブファイル選択実行開始
			DmFileSlctStart(NULL);
#else
			// メインメニューへ
			main_work->proc_update = dmTitleProcMainMenuOpenWin;
			main_work->proc_input = NULL;
			
			main_work->flag |= DMD_TITLE_FLAG_SIGN_OUT_EXIT;
			
			main_work->flag |= DMD_TITLE_FLAG_EXIST_SAVE_DATA;
			
			main_work->flag |= DMD_TITLE_FLAG_DISP_MMENU_WIN;
			
			
			if (GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_1_1)) {
				// 初プレイフラグOFF
				main_work->is_init_play = FALSE;
				
				// 初期表示項目設定
				main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_PLAYED;
				
				// カーソルを「つづきから」の表示位置に設定
				main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_TUDUKI;
			}
			
			else {
				main_work->is_init_play = TRUE;
				
				// 初期表示項目設定
				main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_FIRST;
				
				// カーソルを「初めから」の表示位置に設定
				main_work->cur_slct_menu = 0;
			}
			
			// メインメニューウインドウ表示フラグON
			main_work->flag |= DMD_TITLE_FLAG_DISP_MMENU_WIN;

			
			if (GsTrialIsTrial()) {
				main_work->cur_crsr_pos_y = dm_title_crsr_trial_pos_y_tbl[main_work->cur_slct_menu];
			}
			else {
				main_work->cur_crsr_pos_y = main_work->cur_slct_menu * dm_title_crsr_pos_y_tbl[1]
										   		+ dm_title_crsr_pos_y_tbl[0];
			}
			
			// ロードしたセーブデータのデータをGSに設定
			dmTitleSetLoadSysData(main_work);
#endif
		}
		
		else {
			is_error = AoStorageGetError();
			
			main_work->is_init_play = TRUE;
			
			// 初期表示項目設定
			main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_FIRST;
			
			if (is_error == AOD_STORAGE_ERROR_LOADDATA_NONE) {
				
				main_work->proc_update = dmTitleProcFileSlctSaveStartWait;
			}
#if _WII
			else {
				// タイトルへ強制的に戻す
				main_work->proc_update = dmTitleProcWaitInput;
				main_work->proc_input = dmTitleInputProcTitle;
				
				DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_TITLE);
				
				main_work->flag |= DMD_TITLE_FLAG_DISP_TITLE;
				main_work->flag &= ~DMD_TITLE_FLAG_SIGN_OUT_EXIT;
				
				main_work->timer = 0;
				
				return;
			}
#else
			
			else {
				// メインメニューへ
				main_work->proc_update = dmTitleProcMainMenuOpenWin;
				main_work->proc_input = NULL;
				
				main_work->flag |= DMD_TITLE_FLAG_SIGN_OUT_EXIT;
				
				main_work->flag |= DMD_TITLE_FLAG_EXIST_SAVE_DATA;
				
				// メインメニューウインドウ表示フラグON
				main_work->flag |= DMD_TITLE_FLAG_DISP_MMENU_WIN;
				
				// タイトルからセーブ無効で進んだ場合は必ず初期状態
				dmTitleSetInitSaveData(main_work);
				
				// 初期データ設定
				dmTitleSetInitSysData(main_work);
				
				return;
			}
#endif
		}
	}
}



// ==========================================================================
// dmTitleProcFileSlctSaveStartWait
/*!
	ファイルセーブ(新規)待ち中処理
  	ここを通過する場合は新規データ作成時のみ
 */
// ==========================================================================
void dmTitleProcFileSlctSaveStartWait(DMS_TITLE_MAIN_WORK *main_work)
{
	// ロード画面が終了したら
	if (DmSaveIsExit()) {
		// 新規データセーブ
		DmSaveStart(1 << DME_SAVE_WIN_NOW_SAVING, TRUE);
		
		main_work->proc_update = dmTitleProcFileSlctWaitDataSave;
	}
}



// ==========================================================================
// dmTitleProcFileSlctWaitDataSave
/*!
	ファイルセーブ(新規)待ち中処理
  	ここを通過する場合は新規データ作成時のみ
 */
// ==========================================================================
void dmTitleProcFileSlctWaitDataSave(DMS_TITLE_MAIN_WORK *main_work)
{
	if (DmSaveIsExit()) {
		if (!DmCmnBackupIsSaveSuccessed()) {
			// タイトルへ強制的に戻す		※◆ここもメインメニューへいくように要修正(但しセーブはなしで)
			main_work->proc_update = dmTitleProcWaitInput;
			main_work->proc_input = dmTitleInputProcTitle;
			
			main_work->flag |= DMD_TITLE_FLAG_DISP_TITLE;
			main_work->flag &= ~DMD_TITLE_FLAG_DISP_MAINMENU;
			main_work->flag &= ~DMD_TITLE_FLAG_SIGN_OUT_EXIT;
			
			main_work->timer = 0;
			
			DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_TITLE);
			
			return;
		}
		
#if _WII
		// セーブファイル選択へ
		main_work->proc_update = dmTitleProcFileSlctIdle;
		main_work->proc_input = NULL;
		
		// セーブファイル選択実行開始
		DmFileSlctStart(NULL);
#else
		// データ新規作成時はデータを全て初期化する
		gs::backup::SBackup &backup = gs::backup::SBackup::CreateInstance();
		backup.Init();
		
		// 初プレイフラグON
		main_work->is_init_play = TRUE;
		
		// 初期データ設定
		dmTitleSetInitSysData(main_work);
		
		// メインメニューへ
		main_work->proc_update = dmTitleProcMainMenuOpenWin;
		main_work->proc_input = NULL;
		
		main_work->flag |= DMD_TITLE_FLAG_SIGN_OUT_EXIT;
		
		// メインメニューウインドウ表示フラグON
		main_work->flag |= DMD_TITLE_FLAG_DISP_MMENU_WIN;
#endif
	}
	
}



// ==========================================================================
// dmTitleProcFileSlctIdle
/*!
	ファイル選択待ち中処理
 */
// ==========================================================================
#if _WII
void dmTitleProcFileSlctIdle(DMS_TITLE_MAIN_WORK *main_work)
{
#if _WII
	OSTime tmp_play_time = 0;
#endif
	
	// ファイル選択で決定した場合
	if (DmFileSlctIsExit() == 1) {
		// メインメニューへ
		main_work->proc_update = dmTitleProcMainMenuOpenWin;
		main_work->proc_input = NULL;
		
		main_work->flag |= DMD_TITLE_FLAG_DISP_MMENU_WIN;
		
		if (GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_1_1)) {
			// 初プレイフラグOFF
			main_work->is_init_play = FALSE;
			
			// ロードしたセーブデータのデータをGSに設定
			dmTitleSetLoadSysData(main_work);
		}
		else {
			// 初プレイフラグON
			main_work->is_init_play = TRUE;
			
			// ロードしたセーブデータのデータをGSに設定
//			dmTitleSetLoadSysData(main_work);
			// 初期データ設定
			dmTitleSetInitSysData(main_work);
		}
		
		
#if _WII
		gs::backup::SBackup &backup = gs::backup::SBackup::CreateInstance();
		
		gs::backup::SSystem &sys_data = backup.GetSystem(backup.GetSaveIndex());
		
		tmp_play_time = (OSTime)sys_data.GetLastSaveChrono();
#endif
	}
	// ファイル選択でキャンセルの場合
	else if (DmFileSlctIsExit() == -1) {
		main_work->proc_update = dmTitleProcWaitInput;
		main_work->proc_input = dmTitleInputProcTitle;

		// 表示切り替え設定
		main_work->flag |= DMD_TITLE_FLAG_DISP_TITLE;
		main_work->flag &= ~DMD_TITLE_FLAG_DISP_MAINMENU;
		
		// メインメニューウインドウ表示フラグON
		main_work->flag &= ~DMD_TITLE_FLAG_DISP_MMENU_WIN;
		
		main_work->flag &= ~DMD_TITLE_FLAG_SIGN_OUT_EXIT;
		
		// タイトルへ戻る際はバックアップ初期化
		gs::backup::SBackup &backup = gs::backup::SBackup::CreateInstance();
		backup.Init();
		
		// 権利表示ON
		DmTitleOpDispRightEnable(TRUE);
		
		// タイトルへ戻ったら再度タイトルジングル再生
		DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_TITLE);
	}
	
}
#endif



// ==========================================================================
// dmTitleProcMainMenuOpenWin
/*!
	メインメニューウインドウオープン待ち処理
 */
// ==========================================================================
void dmTitleProcMainMenuOpenWin(DMS_TITLE_MAIN_WORK *main_work)
{
	// 演出終了チェック
	if (main_work->flag & DMD_TITLE_FLAG_MMENU_WIN_EFCT_END) {
		
#ifndef SONIC4_TRIAL
		mppTimeScores::timeScores.SetLeaderboardTimesFromGameAtFirstLaunchIfNecessary();

		mppUtil::showCommunityButton(true);//kolya
#endif
		
		/*qqq - old - before menu
		static bool needToShowConfirmation = true;
		if(needToShowConfirmation) {//qqq
			needToShowConfirmation = false;
			if(mppCheckPointStorage::isStateExist()) {
				mppUtil::showLoadGameConfirmation();
			}
		}*/
		// ウインドウのプロシージャ設定
		// メインメニューへ
		main_work->proc_update = dmTitleProcMainMenuIdle;
		main_work->proc_input = dmTitleInputProcMainMenu;
		
		main_work->mmenu_win_timer = 0;
		
		// メインメニューアクション表示フラグON
		main_work->flag |= DMD_TITLE_FLAG_DISP_MAINMENU;
		main_work->flag &= ~DMD_TITLE_FLAG_DISP_TITLE;
		
		// ウインドウ演出中フラグOFF
		main_work->flag &= ~DMD_TITLE_FLAG_MMENU_WIN_EFCT_END;
		
		// 権利表示ON
		DmTitleOpDispRightEnable(FALSE);
		
#if !_WII
		if (GsTrialIsTrial()
			|| main_work->is_init_play) {
			main_work->cur_slct_menu = 0;
		}
		else {
			main_work->cur_slct_menu = 1;
		}
#endif
		
		// カーソルを初期表示位置に設定
		if (GsTrialIsTrial()) {
			main_work->cur_crsr_pos_y = dm_title_crsr_trial_pos_y_tbl[main_work->cur_slct_menu];
		}
		else {
			main_work->cur_crsr_pos_y = main_work->cur_slct_menu * dm_title_crsr_pos_y_tbl[1]
									   		+ dm_title_crsr_pos_y_tbl[0];
		}
		
		return;
	}
	else {
		// ウインドウオープン演出処理
		dmTitleSetMMenuWinOpenEfct(main_work);
	}
}



// ==========================================================================
// dmTitleProcMainMenuCloseWin
/*!
	メインメニューウインドウクローズ待ち処理
 */
// ==========================================================================
void dmTitleProcMainMenuCloseWin(DMS_TITLE_MAIN_WORK *main_work)
{
#if _WII
	GSS_MAIN_SYS_INFO *gs_main = GsGetMainSysInfo();
#endif
	
	mppUtil::showCommunityButton(false);//kolya (back to splash)
	
	// 演出終了チェック
	if (main_work->flag & DMD_TITLE_FLAG_MMENU_WIN_EFCT_END) {
#if _WII
		// ウインドウのプロシージャ設定
		main_work->proc_update = dmTitleProcFileSlctIdle;
		main_work->proc_input = NULL;
		
		// メインシステムのボリュームに初期値を設定
		gs_main->bgm_volume	= 1.0f;
		gs_main->se_volume	= 1.0f;
		
		// ボリューム初期化
		for (Sint32 i = 0; i < GSE_SND_TYPE_MAX; ++i) {
			GsSoundSetVolume((GSE_SND_TYPE)i, 1.f);
		}
		
		// ファイル選択起動
		DmFileSlctStart(NULL);
#else
		// タイトルへ戻る
		AoAccountClearCurrentId();		// ※クリア
		
		// タイトルへ戻る際はバックアップ初期化
		gs::backup::SBackup &backup = gs::backup::SBackup::CreateInstance();
		backup.Init();
		
		// アカウントチェッククリア
		main_work->flag &= ~DMD_TITLE_FLAG_SIGN_OUT_EXIT;
		
		main_work->proc_update = dmTitleProcWaitInput;
		main_work->proc_input = dmTitleInputProcTitle;

		// 表示切り替え設定
		main_work->flag |= DMD_TITLE_FLAG_DISP_TITLE;
		main_work->flag &= ~DMD_TITLE_FLAG_DISP_MAINMENU;
		
		// メインメニューウインドウ表示フラグON
		main_work->flag &= ~DMD_TITLE_FLAG_DISP_MMENU_WIN;
		
		// 権利表示ON
		DmTitleOpDispRightEnable(TRUE);
		
		// タイトルへ戻ったら再度タイトルジングル再生
		DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_TITLE);
		
#endif
		main_work->mmenu_win_timer = 0;
		
		// ウインドウ演出中フラグON
		main_work->flag &= ~DMD_TITLE_FLAG_DISP_MMENU_WIN;
		
		// ウインドウ演出中フラグOFF
		main_work->flag &= ~DMD_TITLE_FLAG_MMENU_WIN_EFCT_END;
		
		return;
	}
	
	// ウインドウオープン演出処理
	dmTitleSetMMenuWinCloseEfct(main_work);
}



// ==========================================================================
// dmTitleProcMainMenuIdle
/*!
	メインメニュー入力待ち中処理
 */
// ==========================================================================
void dmTitleProcMainMenuIdle(DMS_TITLE_MAIN_WORK *main_work)
{
	// メインメニュー入力待ち中は常時、完全版への切り替えチェックを行う
#if _XBOX
	if (GsTrialIsTrial() && !GsTrialIsTrialDirect()) {
		dmTitleSetMainMenuTrialToComp(main_work);
		
		main_work->proc_update = dmTitleProcMainMenuCheckTrialToComp;
		
		return;
	}
#endif
	
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}
	
	// キャンセルフラグONならば
	if (main_work->flag & DMD_TITLE_FLAG_CANCEL) {
#if _PC || _PS3 || _XBOX
		// セーブ有効状態の場合
		if (dmTitleIsSaveRunning()) {
			// メインメニューウインドウクローズ処理へ
			main_work->proc_update = dmTitleProcMainMenuCloseWin;
			main_work->proc_input = NULL;
			
			main_work->mmenu_win_timer = DMD_TITLE_WIN_EFCT_TIME;
			
			main_work->flag &= ~DMD_TITLE_FLAG_DISP_MAINMENU;
		}
		// セーブ無効状態の場合
		else {
			if (!GsTrialIsTrial()) {
				if (!main_work->is_init_play
					|| dmTitleIsChangeOptVol()) {
					// データ削除チェック処理へ
					main_work->proc_update = dmTitleProcNoSaveCheckDelData;
					main_work->proc_input = NULL;
					
					main_work->timer = 0;
					
					// セーブ削除チェックウインドウメッセージフラグON
					main_work->announce_flag |= (1 << DME_TITLE_WIN_TYPE_DEL_DATA3);
				}
				else {
					// メインメニューウインドウクローズ処理へ
					main_work->proc_update = dmTitleProcMainMenuCloseWin;
					main_work->proc_input = NULL;
					
					main_work->mmenu_win_timer = DMD_TITLE_WIN_EFCT_TIME;
					
					main_work->flag &= ~DMD_TITLE_FLAG_DISP_MAINMENU;
				}
			}
			else {
				// メインメニューウインドウクローズ処理へ
				main_work->proc_update = dmTitleProcMainMenuCloseWin;
				main_work->proc_input = NULL;
				
				main_work->mmenu_win_timer = DMD_TITLE_WIN_EFCT_TIME;
				
				main_work->flag &= ~DMD_TITLE_FLAG_DISP_MAINMENU;
			}
		}
#else
		// メインメニューウインドウクローズ処理へ
		main_work->proc_update = dmTitleProcMainMenuCloseWin;
		main_work->proc_input = NULL;
		
		main_work->mmenu_win_timer = DMD_TITLE_WIN_EFCT_TIME;
		
		main_work->flag &= ~DMD_TITLE_FLAG_DISP_MAINMENU;
#endif
		
		main_work->flag &= ~DMD_TITLE_FLAG_CANCEL;
		main_work->flag &= ~DMD_TITLE_FLAG_DECIDE;
		
		DmSoundPlaySE("Cancel");
		
		return;
	}
	
	// 決定フラグONならば
	if (main_work->flag & DMD_TITLE_FLAG_DECIDE) {
		// 決定演出		現在は仕様未確定により空
		main_work->proc_update = dmTitleProcMainMenuDecideEfct;
		main_work->proc_input = NULL;
		
		main_work->timer = 0.f;
		
		main_work->flag &= ~DMD_TITLE_FLAG_CANCEL;
		main_work->flag &= ~DMD_TITLE_FLAG_DECIDE;
		
		DmSoundPlaySE("Ok");
		
		return;
	}

#if _PC || _PS3 || _XBOX || _IPHONE
	// 購入フラグONならば
	if (main_work->flag & DMD_TITLE_FLAG_BUY_COMP) {
		// 決定演出
		main_work->proc_update = dmTitleProcMainMenuCompBuyIdle;
		main_work->proc_input = NULL;
		
		main_work->timer = 0.f;
		
		main_work->flag |= DMD_TITLE_FLAG_COMP_NO_DISP_END;
		
		if(true) {
			mppUtil::launchUpsellScreen(0);//sss - new
		}
		else {			
			// 完全版購入画面起動
			DmBuyScreenStart(&main_work->buy_scr_work, FALSE, TRUE);
		}
		
		main_work->flag &= ~DMD_TITLE_FLAG_BUY_COMP;
		
		return;
	}
#endif // #if _PC || _PS3 || _XBOX || _IPHONE
	
	// カーソル移動ならば
	if (main_work->flag & DMD_TITLE_FLAG_MENU_UP_INPUT
		|| main_work->flag & DMD_TITLE_FLAG_MENU_DOWN_INPUT) {
		// 移動演出設定
		dmTitleSetChngFocusCrsrData(main_work);
		
		DmSoundPlaySE("Cursol");
		
		main_work->flag &= ~DMD_TITLE_FLAG_MENU_UP_INPUT;
		main_work->flag &= ~DMD_TITLE_FLAG_MENU_DOWN_INPUT;
	}

	// カーソル移動演出
	if (main_work->flag & DMD_TITLE_FLAG_MENU_CRSR_EFCT) {
		dmTitleSetCtrlFocusChangeEfct(main_work);
		
		if (dmTitleIsCtrlFocusChangeEfctEnd(main_work)) {
			main_work->flag &= ~DMD_TITLE_FLAG_MENU_CRSR_EFCT;
		}
	}
}



#if _XBOX
// ==========================================================================
// dmTitleProcMainMenuCheckTrialToComp
/*!
	フェードイン中処理
 */
// ==========================================================================
void dmTitleProcMainMenuCheckTrialToComp(DMS_TITLE_MAIN_WORK *main_work)
{
	if (DmSaveIsExit()) {
		// プロシージャを元に戻す
		main_work->proc_update = dmTitleProcMainMenuIdle;
	}
}


// ==========================================================================
// dmTitleSetMainMenuTrialToComp
/*!
	フェードイン中処理
 */
// ==========================================================================
void dmTitleSetMainMenuTrialToComp(DMS_TITLE_MAIN_WORK *main_work)
{
	// メインメニュー：ユーザ選択時
	if (GsTrialIsTrial() && !GsTrialIsTrialDirect()) {
		DmSaveMenuStart();
		
		// ここで完全版に変更するための必要な設定を行う
		
		if (GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_1_1)) {
			// 初プレイフラグOFF
			main_work->is_init_play = FALSE;
			
			// 初期表示項目設定
			main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_PLAYED;
			
			// カーソルを「つづきから」の表示位置に設定
			main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_TUDUKI;
		}
		
		else {
			main_work->is_init_play = TRUE;
			
			// 初期表示項目設定
			main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_FIRST;
			
			// カーソルを「初めから」の表示位置に設定
			main_work->cur_slct_menu = 0;
		}
		
	}
	
	
	if (GsTrialIsTrialDirect()) {
		// 体験版表示
		main_work->cur_crsr_pos_y = dm_title_crsr_trial_pos_y_tbl[main_work->cur_slct_menu];
	}
	else {
		// 製品版表示
		main_work->cur_crsr_pos_y = main_work->cur_slct_menu * dm_title_crsr_pos_y_tbl[1]
							   		+ dm_title_crsr_pos_y_tbl[0];
	}
}

#endif


#if _PC || _PS3 || _XBOX || _IPHONE
// ==========================================================================
// dmTitleProcMainMenuCompBuyFadeOut
/*!
	メインメニューの購入画面への処理
 */
// ==========================================================================
void dmTitleProcMainMenuCompBuyFadeOut(DMS_TITLE_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		main_work->proc_update = dmTitleProcMainMenuCompBuyIdle;
		
		if(true) {
			mppUtil::launchUpsellScreen(true);//sss - new (from menu)
		}
		else {		
		// 完全版購入画面起動
			DmBuyScreenStart(&main_work->buy_scr_work, TRUE, TRUE); //sss - old
		}
		
		main_work->flag &= ~DMD_TITLE_FLAG_SIGN_OUT_EXIT;
		
		// タイトル側全非表示
		main_work->disp_flag &= ~DMD_TITLE_DISP_FLAG_ALL_ACTION;
	}

	// カーソル移動演出(フェードアウト中も移動するように)
	if (main_work->flag & DMD_TITLE_FLAG_MENU_CRSR_EFCT) {
		dmTitleSetCtrlFocusChangeEfct(main_work);
		
		if (dmTitleIsCtrlFocusChangeEfctEnd(main_work)) {
			main_work->flag &= ~DMD_TITLE_FLAG_MENU_CRSR_EFCT;
		}
	}
}



// ==========================================================================
// dmTitleProcMainMenuCompBuyIdle
/*!
	メインメニューの決定演出中処理
 */
// ==========================================================================
void dmTitleProcMainMenuCompBuyIdle(DMS_TITLE_MAIN_WORK *main_work)
{
#if _XBOX
	if (!GsTrialIsTrial()) {
		// ここで完全版に変更するための必要な設定を行う
		if (GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_1_1)) {
			// 初プレイフラグOFF
			main_work->is_init_play = FALSE;
			
			// 初期表示項目設定
			main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_PLAYED;
			
			// カーソルを「つづきから」の表示位置に設定
			main_work->cur_slct_menu = DME_TITLE_MENU_TYPE_TUDUKI;
		}
		
		else {
			main_work->is_init_play = TRUE;
			
			// 初期表示項目設定
			main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_FIRST;
			
			// カーソルを「初めから」の表示位置に設定
			main_work->cur_slct_menu = 0;
		}
		
		if (GsTrialIsTrialDirect()) {
			// 体験版表示
			main_work->cur_crsr_pos_y = dm_title_crsr_trial_pos_y_tbl[main_work->cur_slct_menu];
		}
		else {
			// 製品版表示
			main_work->cur_crsr_pos_y = main_work->cur_slct_menu * dm_title_crsr_pos_y_tbl[1]
								   		+ dm_title_crsr_pos_y_tbl[0];
		}
	}
#endif
	
	if (DmBuyScreenIsFinished((const DMS_BUY_SCR_WORK *)&main_work->buy_scr_work)) {
		// ここでゲーム終了かメインメニューへ戻るかを分岐させる
		if (DmBuyScreenGetResult((const DMS_BUY_SCR_WORK *)&main_work->buy_scr_work)
				== DMD_BUY_SCR_RESULT_BUY) {
			// フェード処理開始			※現在の
			if (main_work->flag & DMD_TITLE_FLAG_COMP_NO_DISP_END) {
				main_work->flag &= ~DMD_TITLE_FLAG_COMP_NO_DISP_END;
			}
			else {
				IzFadeInitEasy(IZE_FADE_SET_TYPE_TAKEOEVER
							   , IZE_FADE_TYPE_BLACK_FADEIN
							   , DMD_TITLE_FADEIN_TIME
							   );
			}
			
			DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_MENU);
//			DmSoundPlayMenuBGM(DME_SOUND_BGM_IDX_MENU
//							   , DMD_TITLE_BGM_FADEIN_TIME);
			
			main_work->proc_update = dmTitleProcMainMenuCompBuyFadeIn;
			
#if _XBOX || _PC
			if (GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_1_1)) {
				// 初プレイフラグOFF
				main_work->is_init_play = FALSE;
				
				// 初期表示項目設定
				main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_PLAYED;
				
				// カーソルを「つづきから」の表示位置に設定
				if (main_work->cur_slct_menu == DME_TITLE_MENU_TYPE_LIBRARY - 1) {
					main_work->cur_slct_menu = main_work->cur_slct_menu + 1;
				}
				else {
					main_work->cur_slct_menu = 1;
				}
			}
			
			else {
				main_work->is_init_play = TRUE;
				
				// 初期表示項目設定
				main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_FIRST;
				
				// カーソルを「初めから」の表示位置に設定
				if (main_work->cur_slct_menu == DME_TITLE_MENU_TYPE_LIBRARY - 1) {
					main_work->cur_slct_menu = main_work->cur_slct_menu;
				}
				else {
					main_work->cur_slct_menu = 0;
				}
			}
			
			if (GsTrialIsTrial()) {
				main_work->cur_crsr_pos_y = dm_title_crsr_trial_pos_y_tbl[main_work->cur_slct_menu];
			}
			else {
				main_work->cur_crsr_pos_y = main_work->cur_slct_menu * dm_title_crsr_pos_y_tbl[1]
										   		+ dm_title_crsr_pos_y_tbl[0];
			}
#endif
			
			// 表示フラグON
			main_work->disp_flag |= DMD_TITLE_DISP_FLAG_ALL_ACTION;
		}
		
#if _XBOX
		// ゲーム終了時
		else if (DmBuyScreenGetResult((const DMS_BUY_SCR_WORK *)&main_work->buy_scr_work)
				 	== DMD_BUY_SCR_RESULT_BACK) {
			main_work->proc_update = NULL;
			
			GsReqExit();
		}
#endif
		
		// キャンセルでメニューに戻るとき	(例外時は全てキャンセル扱いとする)
		else {
			// フェード処理開始			※現在の
			if (main_work->flag & DMD_TITLE_FLAG_COMP_NO_DISP_END) {
				main_work->flag &= ~DMD_TITLE_FLAG_COMP_NO_DISP_END;
			}
			else {
				IzFadeInitEasy(IZE_FADE_SET_TYPE_TAKEOEVER
							   , IZE_FADE_TYPE_BLACK_FADEIN
							   , DMD_TITLE_FADEIN_TIME
							   );
			}
			
			DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_MENU);
//			DmSoundPlayMenuBGM(DME_SOUND_BGM_IDX_MENU
//							   , DMD_TITLE_BGM_FADEIN_TIME);
			
			main_work->proc_update = dmTitleProcMainMenuCompBuyFadeIn;
			
			// 表示フラグON
			main_work->disp_flag |= DMD_TITLE_DISP_FLAG_ALL_ACTION;
		}
	}
}


//mpp --------------------------------
void mppUS_UpsellScreenFinished(int result)//0/1 - back/goto url	
{
	IzFadeInitEasy(IZE_FADE_SET_TYPE_TAKEOEVER
				   , IZE_FADE_TYPE_BLACK_FADEIN
				   , DMD_TITLE_FADEIN_TIME
				   );
	
	DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_MENU);
	/*
	
	if (main_work->flag & DMD_TITLE_FLAG_COMP_NO_DISP_END) {
		main_work->flag &= ~DMD_TITLE_FLAG_COMP_NO_DISP_END;
	}
	else {
		IzFadeInitEasy(IZE_FADE_SET_TYPE_TAKEOEVER
					   , IZE_FADE_TYPE_BLACK_FADEIN
					   , DMD_TITLE_FADEIN_TIME
					   );
	}
	
	DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_MENU);
	//			DmSoundPlayMenuBGM(DME_SOUND_BGM_IDX_MENU
	//							   , DMD_TITLE_BGM_FADEIN_TIME);
	
	main_work->proc_update = dmTitleProcMainMenuCompBuyFadeIn;
	
	// 表示フラグON
	main_work->disp_flag |= DMD_TITLE_DISP_FLAG_ALL_ACTION;	*/
}

// ==========================================================================
// dmTitleProcMainMenuCompBuyFadeIn
/*!
	メインメニューの決定演出中処理
 */
// ==========================================================================
void dmTitleProcMainMenuCompBuyFadeIn(DMS_TITLE_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		if (DmBuyScreenGetResult((const DMS_BUY_SCR_WORK *)&main_work->buy_scr_work)
				== DMD_BUY_SCR_RESULT_BUY) {
			main_work->proc_update = dmTitleProcMainMenuIdle;
			main_work->proc_input = dmTitleInputProcMainMenu;
			
			// アカウントチェックON
			main_work->flag |= DMD_TITLE_FLAG_SIGN_OUT_EXIT;
		}
		else {
			main_work->proc_update = dmTitleProcMainMenuIdle;
			main_work->proc_input = dmTitleInputProcMainMenu;
			
			// アカウントチェックON
			main_work->flag |= DMD_TITLE_FLAG_SIGN_OUT_EXIT;
		}
		
		// 決定演出フレームを元に戻す
		for (int i = 0; i < DME_TITLE_MENU_TYPE_NUM; i++) {
			main_work->decide_menu_frm[i] = 0;
		}
	}
}
#endif // #if _PC || _PS3 || _XBOX || _IPHONE


#if _XBOX
// ==========================================================================
// dmTitleProcMainMenuTrophyIdle
/*!
	トロフィーウインドウ表示中の処理
 */
// ==========================================================================
void dmTitleProcMainMenuTrophyIdle(DMS_TITLE_MAIN_WORK *main_work)
{
	if (AoTrophyIsHideAchievementUI()) {
		
		main_work->proc_update = dmTitleProcMainMenuIdle;
		main_work->proc_input = dmTitleInputProcMainMenu;
	}
}

#endif


// ==========================================================================
// dmTitleProcMainMenuDecideEfct
/*!
	メインメニューの決定演出中処理
 */
// ==========================================================================
void dmTitleProcMainMenuDecideEfct(DMS_TITLE_MAIN_WORK *main_work)
{
	s32 now_slct_menu = 0;
	
	// カーソル移動演出(決定演出中も移動するように)
	if (main_work->flag & DMD_TITLE_FLAG_MENU_CRSR_EFCT) {
		dmTitleSetCtrlFocusChangeEfct(main_work);
		
		if (dmTitleIsCtrlFocusChangeEfctEnd(main_work)) {
			main_work->flag &= ~DMD_TITLE_FLAG_MENU_CRSR_EFCT;
		}
	}
	
#if !(_WII || _IPHONE)
	// 体験版・初期データ・データありで項目を切り替える
	if (main_work->cur_slct_menu > 0) {
		if (GsTrialIsTrial()
			|| main_work->is_init_play) {
			now_slct_menu = main_work->cur_slct_menu + 1;
		}
		else {
			now_slct_menu = main_work->cur_slct_menu;
		}
	}
#else
	now_slct_menu = main_work->cur_slct_menu;
#endif
	
#if !_IPHONE
	if (main_work->timer > 60.f) {
#else //!_IPHONE
	if (main_work->timer > 15.f) {
#endif //!_IPHONE
#if _PS3
		// 製品版購入を選んだ場合
		if (now_slct_menu == DME_TITLE_MENU_TYPE_BUY) {
			main_work->proc_update = dmTitleProcMainMenuCompBuyFadeOut;
			
			main_work->proc_input = NULL;
			
			main_work->timer = 0.f;
			
			// フェードアウト開始
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_TITLE_FADEOUT_TIME
						   );
			
			// BGMフェードアウト開始
			DmSndBgmPlayerBgmStop();
			
			return;
		}
		
		else if (now_slct_menu == DME_TITLE_MENU_TYPE_START
				 && !main_work->is_init_play
				 && !GsTrialIsTrial()) {
			// セーブが有効な場合
			if (dmTitleIsSaveRunning()) {
				main_work->proc_update = dmTitleProcMainMenuDelSaveWin;
				
				main_work->announce_flag |= (1 << DME_TITLE_WIN_TYPE_DEL_DATA1);
			}
			// セーブが無効な場合
			else {
				main_work->proc_update = dmTitleProcNoSaveCheckInitStart;
				
				main_work->announce_flag |= (1 << DME_TITLE_WIN_TYPE_DEL_DATA4);
			}
			
			return;
		}
		
		// それ以外
		else {
			main_work->proc_update = dmTitleProcFadeOut;
		}
#elif _XBOX
		// 製品版購入を選んだ場合
		if (now_slct_menu == DME_TITLE_MENU_TYPE_LIBRARY) {
			
			if (GsTrialIsTrial()) {
				main_work->proc_update = dmTitleProcMainMenuCompBuyFadeOut;
				main_work->proc_input = NULL;
				
				main_work->timer = 0.f;
				
				// フェードアウト開始
				IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
							   , IZE_FADE_TYPE_BLACK_FADEOUT
							   , DMD_TITLE_FADEOUT_TIME
							   );
				
				// BGMフェードアウト開始
				DmSndBgmPlayerBgmStop();
				
				main_work->flag &= ~DMD_TITLE_FLAG_BUY_COMP;
			}
			else {
				// セーブ有効の場合はメッセージなしでゲーム終了へ
				if (dmTitleIsSaveRunning()) {
					// ゲーム終了シーケンスへ
					main_work->proc_update = dmTitleProcGameExitIdle;
					
					main_work->timer = 0;
					
					// BGMフェードアウト開始
					DmSndBgmPlayerBgmStop();
					
					// フェードアウト開始
					IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
								   , IZE_FADE_TYPE_BLACK_FADEOUT
								   , DMD_TITLE_FADEOUT_TIME
								   );
				}
				// セーブ無効の場合はメッセージを表示させて、OKならゲーム終了へ
				else {
					if (!main_work->is_init_play
						|| dmTitleIsChangeOptVol()) {
						main_work->proc_update = dmTitleProcNoSaveCheckDelEndGame;
						
						main_work->announce_flag |= (1 << DME_TITLE_WIN_TYPE_DEL_DATA5);
					}
					else {
						// ゲーム終了シーケンスへ
						main_work->proc_update = dmTitleProcGameExitIdle;
						
						main_work->timer = 0;
						
						// BGMフェードアウト開始
						DmSndBgmPlayerBgmStop();
						
						// フェードアウト開始
						IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
									   , IZE_FADE_TYPE_BLACK_FADEOUT
									   , DMD_TITLE_FADEOUT_TIME
									   );
					}
				}
			}
			
			return;
		}
		
		else if (now_slct_menu == DME_TITLE_MENU_TYPE_BUY) {
			// 決定演出
			main_work->proc_update = dmTitleProcMainMenuCompBuyIdle;
			main_work->proc_input = NULL;
			
			main_work->timer = 0.f;
			
			main_work->flag |= DMD_TITLE_FLAG_COMP_NO_DISP_END;
			
			if(true) {
				mppUtil::launchUpsellScreen();//sss - new
			}
			else {	
				// 完全版購入画面起動
				DmBuyScreenStart(&main_work->buy_scr_work, FALSE, TRUE);
			}
			
			main_work->decide_menu_frm[DME_TITLE_MENU_TYPE_BUY] = 0;
			
			main_work->flag &= ~DMD_TITLE_FLAG_BUY_COMP;
			
			return;
		}
		
		else if (now_slct_menu == DME_TITLE_MENU_TYPE_ZISSEKI) {
			main_work->proc_update = dmTitleProcMainMenuTrophyIdle;
			
			AoTrophyShowAchievementUI();
			
			main_work->decide_menu_frm[DME_TITLE_MENU_TYPE_ZISSEKI] = 0;
			
			return;
		}
		
		else if (now_slct_menu == DME_TITLE_MENU_TYPE_START
				 && !main_work->is_init_play
				 && !GsTrialIsTrial()) {
			// セーブが有効な場合
			if (dmTitleIsSaveRunning()) {
				main_work->proc_update = dmTitleProcMainMenuDelSaveWin;
				
				main_work->announce_flag |= (1 << DME_TITLE_WIN_TYPE_DEL_DATA1);
			}
			// セーブが無効な場合
			else {
				main_work->proc_update = dmTitleProcNoSaveCheckInitStart;
				
				main_work->announce_flag |= (1 << DME_TITLE_WIN_TYPE_DEL_DATA4);
			}
			
			return;
		}
		
		// それ以外
		else {
			main_work->proc_update = dmTitleProcFadeOut;
		}
#elif _IPHONE
		if (now_slct_menu == DME_TITLE_MENU_TYPE_START
				 && !main_work->is_init_play
				 && !GsTrialIsTrial()) {
			// セーブが有効な場合
			if (dmTitleIsSaveRunning()) {
				main_work->proc_update = dmTitleProcMainMenuDelSaveWin;
				
				main_work->announce_flag |= (1 << DME_TITLE_WIN_TYPE_DEL_DATA1);

				//メインメニューを隠す
				main_work->flag &= ~(DMD_TITLE_FLAG_DISP_MAINMENU | DMD_TITLE_FLAG_DISP_MMENU_WIN);
				mppUtil::showCommunityButton(false); //sss (new game but not first)
			}
			// セーブが無効な場合
			else {
				main_work->proc_update = dmTitleProcFadeOut;
				//mppUtil::hideCommunityButtonWithAnimation();
			}
			
			return;
		}
#if SONIC4_TRIAL
		else if (now_slct_menu == DME_TITLE_MENU_TYPE_TUDUKI) {
			//体験版では2項目が購入フロー
			main_work->proc_update = dmTitleProcMainMenuCompBuyFadeOut;
			main_work->proc_input = NULL;
			main_work->timer = 0.f;
			
			// フェードアウト開始
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_TITLE_FADEOUT_TIME
						   );
			
			// BGMフェードアウト開始
			DmSndBgmPlayerBgmStop();
			return;
		}
#endif //SONIC4_TRIAL
		
		// それ以外
		else {
			main_work->proc_update = dmTitleProcFadeOut;
		}
#else
		// 次へ遷移
		main_work->proc_update = dmTitleProcFadeOut;
#endif

		main_work->timer = 0;
		
		// 決定演出フレームを元に戻す
		for (int i = 0; i < DME_TITLE_MENU_TYPE_NUM; i++) {
			main_work->decide_menu_frm[i] = 0;
		}

		// フェードアウト開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEOUT
					   , DMD_TITLE_FADEOUT_TIME
					   );
		
		
		// BGMフェードアウト開始
#if !_WII
		if (now_slct_menu == DME_TITLE_MENU_TYPE_START) {
			DmSndBgmPlayerExit();
			main_work->flag |= DMD_TITLE_FLAG_DEMO_SND_END;
		}
#if defined (GSD_DEBUG_DEMO_SELECT) || defined (GSD_DEBUG_DEMO_SELECT_TOP)
		else {
			DmSndBgmPlayerExit();
			main_work->flag |= DMD_TITLE_FLAG_DEMO_SND_END;
		}
#endif
		
#else
		if (now_slct_menu == DME_TITLE_MENU_TYPE_START
			&& main_work->is_init_play) {
			DmSndBgmPlayerExit();
			main_work->flag |= DMD_TITLE_FLAG_DEMO_SND_END;
		}
#if defined (GSD_DEBUG_DEMO_SELECT) || defined (GSD_DEBUG_DEMO_SELECT_TOP)
		else {
			DmSndBgmPlayerExit();
			main_work->flag |= DMD_TITLE_FLAG_DEMO_SND_END;
		}
#endif
#endif

		return;
	}
	
	main_work->decide_menu_frm[now_slct_menu]++;
	
	main_work->timer++;
}


#if _PC || _PS3 || _XBOX || _IPHONE
// ==========================================================================
// dmTitleProcMainMenuDelSaveWin
/*!
	メインメニューのセーブ削除チェックウインドウ表示中処理
 */
// ==========================================================================
void dmTitleProcMainMenuDelSaveWin(DMS_TITLE_MAIN_WORK *main_work)
{
	if (main_work->flag & DMD_TITLE_FLAG_BACK_DEL_SAVE) {
		// メインメニューアイドルへ戻す
		main_work->proc_update = dmTitleProcMainMenuIdle;
		main_work->proc_input = dmTitleInputProcMainMenu;
		
		main_work->timer = 0;

		main_work->flag &= ~DMD_TITLE_FLAG_BACK_DEL_SAVE;
		
		// メインメニューへ戻す際は決定演出フレームを戻す
		for (int i = 0; i < DME_TITLE_MENU_TYPE_NUM; i++) {
			main_work->decide_menu_frm[i] = 0.f;
		}
#if _IPHONE
		//メインメニューを戻す
		mppUtil::showCommunityButton(true);//kolya
		main_work->flag |= (DMD_TITLE_FLAG_DISP_MAINMENU | DMD_TITLE_FLAG_DISP_MMENU_WIN);
#endif //_IPHONE
	}
	
	else if (main_work->flag & DMD_TITLE_FLAG_DEL_SAVE_DATA) {
		// セーブデータ初期化処理
#if _IPHONE
		//セーブがある状態で“はじめから”を選択した場合は、コントローラ操作を引き継ぐ
		using gs::backup::SOption;
		SOption &option = SOption::CreateInstance();
		SOption::EControl::Type ctrl = option.GetControl();
		Uint32 volBgm = option.GetVolumeBgm();//sss -- remember sound vol before new game
		Uint32 volEff = option.GetVolumeSe();//sss -- remember eff vol before new game
#endif //_IPHONE
		dmTitleSetInitSaveData(main_work);
#if _IPHONE
		option.SetControl(ctrl);
		option.SetVolumeBgm(volBgm);//sss -- restore
		option.SetVolumeSe(volEff);//sss -- restore
#endif //_IPHONE		
		main_work->proc_update = dmTitleProcSaveInitData;
		
		// データセーブ
		DmSaveMenuStart();
		
		main_work->flag &= ~DMD_TITLE_FLAG_DEL_SAVE_DATA;
	}
	
	// カーソル移動演出
	if (main_work->flag & DMD_TITLE_FLAG_MENU_CRSR_EFCT) {
		dmTitleSetCtrlFocusChangeEfct(main_work);
		
		if (dmTitleIsCtrlFocusChangeEfctEnd(main_work)) {
			main_work->flag &= ~DMD_TITLE_FLAG_MENU_CRSR_EFCT;
		}
	}
}


// ==========================================================================
// dmTitleProcSaveInitData
/*!
	セーブ削除(初期化データセーブ)中処理
 */
// ==========================================================================
void dmTitleProcSaveInitData(DMS_TITLE_MAIN_WORK *main_work)
{
	if (DmSaveIsExit()) {
		if (!GsTrialIsTrial()) {
			if (!DmCmnBackupIsSaveSuccessed()) {		// ◆暫定対応
				// タイトルへ強制的に戻す
				main_work->proc_update = dmTitleProcWaitInput;
				main_work->proc_input = dmTitleInputProcTitle;
				
				main_work->flag |= DMD_TITLE_FLAG_DISP_TITLE;
				main_work->flag &= ~DMD_TITLE_FLAG_DISP_MAINMENU;
				
				// アカウントチェッククリア
				main_work->flag &= ~DMD_TITLE_FLAG_SIGN_OUT_EXIT;
				
				DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_TITLE);
				
				return;
			}
		}
		
		// 初期化後はフェードアウトして1-1プレイ開始へ遷移させる
		main_work->proc_update = dmTitleProcFadeOut;
		
		main_work->timer = 0;
		
		// フェードアウト開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEOUT
					   , DMD_TITLE_FADEOUT_TIME
					   );
		
		// BGMフェードアウト開始
		DmSndBgmPlayerExit();
		main_work->flag |= DMD_TITLE_FLAG_DEMO_SND_END;
		
		return;
	}
}
#endif //_PC || _PS3 || _XBOX || _IPHONE



#if _PC || _PS3 || _XBOX
// ==========================================================================
// dmTitleProcNoSaveCheckDelData
/*!
	タイトルへ戻る際のデータ削除チェックウインドウ表示中処理
 */
// ==========================================================================
void dmTitleProcNoSaveCheckDelData(DMS_TITLE_MAIN_WORK *main_work)
{
	if (main_work->flag & DMD_TITLE_FLAG_DEL_DATA_BACK_MMENU) {
		// メインメニューアイドルへ戻す
		main_work->proc_update = dmTitleProcMainMenuIdle;
		main_work->proc_input = dmTitleInputProcMainMenu;
		
		main_work->timer = 0;
		
		main_work->flag &= ~DMD_TITLE_FLAG_DEL_DATA_BACK_MMENU;
		
		// メインメニューへ戻す際は決定演出フレームを戻す
		for (int i = 0; i < DME_TITLE_MENU_TYPE_NUM; i++) {
			main_work->decide_menu_frm[i] = 0.f;
		}
	}
	
	else if (main_work->flag & DMD_TITLE_FLAG_DEL_DATA_BACK_TITLE) {
		// メインメニューウインドウクローズ処理へ
		main_work->proc_update = dmTitleProcMainMenuCloseWin;
		main_work->proc_input = NULL;
		
		main_work->mmenu_win_timer = DMD_TITLE_WIN_EFCT_TIME;
		
		main_work->flag &= ~DMD_TITLE_FLAG_DISP_MAINMENU;
		
		main_work->flag &= ~DMD_TITLE_FLAG_DEL_DATA_BACK_TITLE;
	}
	
	// カーソル移動演出
	if (main_work->flag & DMD_TITLE_FLAG_MENU_CRSR_EFCT) {
		dmTitleSetCtrlFocusChangeEfct(main_work);
		
		if (dmTitleIsCtrlFocusChangeEfctEnd(main_work)) {
			main_work->flag &= ~DMD_TITLE_FLAG_MENU_CRSR_EFCT;
		}
	}
}



// ==========================================================================
// dmTitleProcNoSaveCheckInitStart
/*!
	1-1を開始する際のデータ削除チェックウインドウ表示中処理
 */
// ==========================================================================
void dmTitleProcNoSaveCheckInitStart(DMS_TITLE_MAIN_WORK *main_work)
{
	if (main_work->flag & DMD_TITLE_FLAG_DEL_DATA_BACK_MMENU) {
		// メインメニューアイドルへ戻す
		main_work->proc_update = dmTitleProcMainMenuIdle;
		main_work->proc_input = dmTitleInputProcMainMenu;
		
		main_work->timer = 0;
		
		main_work->flag &= ~DMD_TITLE_FLAG_DEL_DATA_BACK_MMENU;
		
		// メインメニューへ戻す際は決定演出フレームを戻す
		for (int i = 0; i < DME_TITLE_MENU_TYPE_NUM; i++) {
			main_work->decide_menu_frm[i] = 0.f;
		}
	}
	
	else if (main_work->flag & DMD_TITLE_FLAG_DEL_DATA_INIT_START) {
		// セーブデータ初期化処理
		dmTitleSetInitSaveData(main_work);
		
		main_work->proc_update = dmTitleProcSaveInitData;
		
		main_work->flag &= ~DMD_TITLE_FLAG_DEL_SAVE_DATA;
	}
	
	// カーソル移動演出
	if (main_work->flag & DMD_TITLE_FLAG_MENU_CRSR_EFCT) {
		dmTitleSetCtrlFocusChangeEfct(main_work);
		
		if (dmTitleIsCtrlFocusChangeEfctEnd(main_work)) {
			main_work->flag &= ~DMD_TITLE_FLAG_MENU_CRSR_EFCT;
		}
	}
}

#endif


#if _XBOX
// ==========================================================================
// dmTitleProcNoSaveCheckDelEndGame
/*!
	タイトルへ戻る際のデータ削除チェックウインドウ表示中処理
 */
// ==========================================================================
void dmTitleProcNoSaveCheckDelEndGame(DMS_TITLE_MAIN_WORK *main_work)
{
	if (main_work->flag & DMD_TITLE_FLAG_DEL_DATA_BACK_MMENU) {
		// メインメニューアイドルへ戻す
		main_work->proc_update = dmTitleProcMainMenuIdle;
		main_work->proc_input = dmTitleInputProcMainMenu;
		
		main_work->timer = 0;
		
		main_work->flag &= ~DMD_TITLE_FLAG_DEL_DATA_BACK_MMENU;
		
		// メインメニューへ戻す際は決定演出フレームを戻す
		for (int i = 0; i < DME_TITLE_MENU_TYPE_NUM; i++) {
			main_work->decide_menu_frm[i] = 0.f;
		}
	}
	
	else if (main_work->flag & DMD_TITLE_FLAG_DEL_DATA_GAME_END) {
		// ゲーム終了待ち処理へ
		main_work->proc_update = dmTitleProcGameExitIdle;
		
		main_work->timer = 0;
		
		// BGMフェードアウト開始
		DmSndBgmPlayerBgmStop();
		
		// フェードアウト開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEOUT
					   , DMD_TITLE_FADEOUT_TIME
					   );
	}
	
	// カーソル移動演出
	if (main_work->flag & DMD_TITLE_FLAG_MENU_CRSR_EFCT) {
		dmTitleSetCtrlFocusChangeEfct(main_work);
		
		if (dmTitleIsCtrlFocusChangeEfctEnd(main_work)) {
			main_work->flag &= ~DMD_TITLE_FLAG_MENU_CRSR_EFCT;
		}
	}
}


#endif


// ==========================================================================
// dmTitleProcFadeOut
/*!
	フェードアウト中処理
 */
// ==========================================================================
void dmTitleProcFadeOut(DMS_TITLE_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// オープニング終了処理
		DmTitleOpExit();
		
		// 次へ遷移
		main_work->proc_update = dmTitleProcDataRelease;
		main_work->proc_draw = NULL;

		return;
	}
}



#if _PC || _PS3 || _XBOX || _IPHONE
//#if 0//_PS3 || _XBOX || _PC
// ==========================================================================
// dmTitleProcWindowNodispIdle
/*!
	ウインドウ非表示待ち中処理
 */
// ==========================================================================
void dmTitleProcWindowNodispIdle(DMS_TITLE_MAIN_WORK *main_work)
{
	// ウインドウ入力処理
	if (main_work->proc_win_input) {
		main_work->proc_win_input(main_work);
	}

	// メニュー遷移フラグONならば
	if (main_work->announce_flag) {
		main_work->proc_win_update = dmTitleProcWindowOpenEfct;

		// 通常処理の入力処理をなくす(二重入力を防ぐため)
		main_work->proc_input = NULL;

		// ウインドウ開閉演出時は入力処理なし
		main_work->proc_win_input = NULL;

		// ウインドウ演出用タイマー初期化
		main_work->win_timer = 0;

		// ウインドウ選択変数設定
		for (u32 i = DME_TITLE_WIN_TYPE_DEL_DATA1; i < DME_TITLE_WIN_TYPE_MAX; i++) {
			if (main_work->announce_flag & 1 << i) {
				main_work->win_mode = (s32)i;
				break;
			}
		}
		main_work->win_cur_slct = 1;
		
		DmSoundPlaySE("Window");

		// ウインドウ演出中フラグON
		main_work->flag |= DMD_TITLE_FLAG_WIN_DRAW_START;
	}

	
}




// ==========================================================================
// dmTitleProcWindowOpenEfct
/*!
	ウインドウオープン中処理
 */
// ==========================================================================
void dmTitleProcWindowOpenEfct(DMS_TITLE_MAIN_WORK *main_work)
{
	// 演出終了チェック
	if (main_work->flag & DMD_TITLE_FLAG_WIN_EFCT_END) {
		// ウインドウのプロシージャ設定
		main_work->proc_win_update = dmTitleProcWindowAnnounceIdle;
		
		main_work->proc_win_input = dmTitleInputProcWindow;
		
		// ウインドウ内アクション表示フラグON
		main_work->disp_flag |= DMD_TITLE_DISP_FLAG_WIN_ACT;
		
		// ウインドウ演出中フラグOFF
		main_work->flag &= ~DMD_TITLE_FLAG_WIN_EFCT_END;
	}
	else {
		// ウインドウオープン演出処理
		dmTitleSetWinOpenEfct(main_work);
	}

	// ウインドウ描画
//	dmTitleWinSelectDraw(main_work);
}



// ==========================================================================
// dmTitleProcWindowAnnounceIdle
/*!
	ウインドウ入力待ち処理
 */
// ==========================================================================
void dmTitleProcWindowAnnounceIdle(DMS_TITLE_MAIN_WORK *main_work)
{
	// ウインドウ入力処理
	if (main_work->proc_win_input) {
		main_work->proc_win_input(main_work);
	}

	
	if (main_work->win_mode == DME_TITLE_WIN_TYPE_DEL_DATA1) {
		// 
		if (main_work->flag & DMD_TITLE_FLAG_DECIDE
			&& main_work->win_cur_slct == 0) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_TITLE_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_TITLE_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmTitleProcWindowCloseEfct;
			
			DmSoundPlaySE("Ok");

			// フラグOFF
			main_work->flag &= ~DMD_TITLE_FLAG_DECIDE;
			main_work->flag &= ~DMD_TITLE_FLAG_CANCEL;
		}
		
		else if (main_work->flag & DMD_TITLE_FLAG_CANCEL
				 || (main_work->win_cur_slct == 1
					 && main_work->flag & DMD_TITLE_FLAG_DECIDE)) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_TITLE_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_TITLE_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmTitleProcWindowCloseEfct;

#if !_IPHONE
			if (main_work->flag & DMD_TITLE_FLAG_CANCEL) {
#else //_IPHONE
			if (true) {
#endif //_IPHONE
				DmSoundPlaySE("Cancel");
			}
			else {
				DmSoundPlaySE("Ok");
			}
			
			main_work->flag |= DMD_TITLE_FLAG_CANCEL;

			// フラグOFF
			main_work->flag &= ~DMD_TITLE_FLAG_DECIDE;
		}
		
	}
	
	else if (main_work->win_mode == DME_TITLE_WIN_TYPE_DEL_DATA2) {
		// 
		if (main_work->flag & DMD_TITLE_FLAG_DECIDE
			&& main_work->win_cur_slct == 0) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_TITLE_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_TITLE_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmTitleProcWindowCloseEfct;

			DmSoundPlaySE("Ok");
			
			// フラグOFF
			main_work->flag &= ~DMD_TITLE_FLAG_DECIDE;
			main_work->flag &= ~DMD_TITLE_FLAG_CANCEL;
		}
		
		else if (main_work->flag & DMD_TITLE_FLAG_CANCEL
				 || (main_work->win_cur_slct == 1
					 && main_work->flag & DMD_TITLE_FLAG_DECIDE)) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_TITLE_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_TITLE_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmTitleProcWindowCloseEfct;
			
#if !_IPHONE
			if (main_work->flag & DMD_TITLE_FLAG_CANCEL) {
#else //_IPHONE
			if (true) {
#endif //_IPHONE
				DmSoundPlaySE("Cancel");
			}
			else {
				DmSoundPlaySE("Ok");
			}
			
			main_work->flag |= DMD_TITLE_FLAG_CANCEL;

			// フラグOFF
			main_work->flag &= ~DMD_TITLE_FLAG_DECIDE;
		}
	}
	
#if !_IPHONE
	else if (main_work->win_mode == DME_TITLE_WIN_TYPE_DEL_DATA3) {
		// 
		if (main_work->flag & DMD_TITLE_FLAG_DECIDE
			&& main_work->win_cur_slct == 0) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_TITLE_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_TITLE_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmTitleProcWindowCloseEfct;

			DmSoundPlaySE("Ok");
			
			// フラグOFF
			main_work->flag &= ~DMD_TITLE_FLAG_DECIDE;
			main_work->flag &= ~DMD_TITLE_FLAG_CANCEL;
		}
		
		else if (main_work->flag & DMD_TITLE_FLAG_CANCEL
				 || (main_work->win_cur_slct == 1
					 && main_work->flag & DMD_TITLE_FLAG_DECIDE)) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_TITLE_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_TITLE_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmTitleProcWindowCloseEfct;
			
			if (main_work->flag & DMD_TITLE_FLAG_CANCEL) {
				DmSoundPlaySE("Cancel");
			}
			else {
				DmSoundPlaySE("Ok");
			}
			
			main_work->flag |= DMD_TITLE_FLAG_CANCEL;

			// フラグOFF
			main_work->flag &= ~DMD_TITLE_FLAG_DECIDE;
		}
	}
	
	
	else if (main_work->win_mode == DME_TITLE_WIN_TYPE_DEL_DATA4) {
		// 
		if (main_work->flag & DMD_TITLE_FLAG_DECIDE
			&& main_work->win_cur_slct == 0) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_TITLE_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_TITLE_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmTitleProcWindowCloseEfct;

			DmSoundPlaySE("Ok");
			
			// フラグOFF
			main_work->flag &= ~DMD_TITLE_FLAG_DECIDE;
			main_work->flag &= ~DMD_TITLE_FLAG_CANCEL;
		}
		
		else if (main_work->flag & DMD_TITLE_FLAG_CANCEL
				 || (main_work->win_cur_slct == 1
					 && main_work->flag & DMD_TITLE_FLAG_DECIDE)) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_TITLE_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_TITLE_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmTitleProcWindowCloseEfct;
			
			if (main_work->flag & DMD_TITLE_FLAG_CANCEL) {
				DmSoundPlaySE("Cancel");
			}
			else {
				DmSoundPlaySE("Ok");
			}
			
			main_work->flag |= DMD_TITLE_FLAG_CANCEL;

			// フラグOFF
			main_work->flag &= ~DMD_TITLE_FLAG_DECIDE;
		}
	}
	
	
	else if (main_work->win_mode == DME_TITLE_WIN_TYPE_DEL_DATA5) {
		// 
		if (main_work->flag & DMD_TITLE_FLAG_DECIDE
			&& main_work->win_cur_slct == 0) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_TITLE_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_TITLE_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmTitleProcWindowCloseEfct;

			DmSoundPlaySE("Ok");
			
			// フラグOFF
			main_work->flag &= ~DMD_TITLE_FLAG_DECIDE;
			main_work->flag &= ~DMD_TITLE_FLAG_CANCEL;
		}
		
		else if (main_work->flag & DMD_TITLE_FLAG_CANCEL
				 || (main_work->win_cur_slct == 1
					 && main_work->flag & DMD_TITLE_FLAG_DECIDE)) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_TITLE_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_TITLE_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmTitleProcWindowCloseEfct;
			
			if (main_work->flag & DMD_TITLE_FLAG_CANCEL) {
				DmSoundPlaySE("Cancel");
			}
			else {
				DmSoundPlaySE("Ok");
			}
			
			main_work->flag |= DMD_TITLE_FLAG_CANCEL;

			// フラグOFF
			main_work->flag &= ~DMD_TITLE_FLAG_DECIDE;
		}
	}
#endif //!_IPHONE

	// ウインドウ描画
//	dmTitleWinSelectDraw(main_work);
}



// ==========================================================================
// dmTitleProcWindowCloseEfct
/*!
	ウインドウクローズ中処理
 */
// ==========================================================================
void dmTitleProcWindowCloseEfct(DMS_TITLE_MAIN_WORK *main_work)
{
	// 演出終了チェック
	if (main_work->flag & DMD_TITLE_FLAG_WIN_EFCT_END) {
		// ウインドウのプロシージャ設定
		main_work->proc_win_update = dmTitleProcWindowNodispIdle;
		
		// アナウンス分のフラグOFF
		main_work->announce_flag &= ~(1 << main_work->win_mode);
		
		if (main_work->win_mode == DME_TITLE_WIN_TYPE_DEL_DATA1
			|| main_work->win_mode == DME_TITLE_WIN_TYPE_DEL_DATA2) {
			if (main_work->flag & DMD_TITLE_FLAG_CANCEL) {
				main_work->flag |= DMD_TITLE_FLAG_BACK_DEL_SAVE;
				main_work->flag &= ~DMD_TITLE_FLAG_CANCEL;
			}
			else {
				if (main_work->win_mode == DME_TITLE_WIN_TYPE_DEL_DATA1) {
					main_work->announce_flag |= (1 << DME_TITLE_WIN_TYPE_DEL_DATA2);
				}
				else if (main_work->win_mode == DME_TITLE_WIN_TYPE_DEL_DATA2) {
					main_work->flag |= DMD_TITLE_FLAG_DEL_SAVE_DATA;
				}
			}
		}
		else {
			if (main_work->flag & DMD_TITLE_FLAG_CANCEL) {
				main_work->flag |= DMD_TITLE_FLAG_DEL_DATA_BACK_MMENU;
				main_work->flag &= ~DMD_TITLE_FLAG_CANCEL;
			}
			else {
				if (main_work->win_mode == DME_TITLE_WIN_TYPE_DEL_DATA3) {
					main_work->flag |= DMD_TITLE_FLAG_DEL_DATA_BACK_TITLE;
				}
				else if (main_work->win_mode == DME_TITLE_WIN_TYPE_DEL_DATA4) {
					main_work->flag |= DMD_TITLE_FLAG_DEL_DATA_INIT_START;
				}
				else {	// if (main_work->win_mode == DME_TITLE_WIN_TYPE_DEL_DATA5)
					main_work->flag |= DMD_TITLE_FLAG_DEL_DATA_GAME_END;
				}
			}
		}
		
		// ウインドウ演出中フラグON
		main_work->flag &= ~DMD_TITLE_FLAG_WIN_DRAW_START;
		
		// ウインドウ演出中フラグOFF
		main_work->flag &= ~DMD_TITLE_FLAG_WIN_EFCT_END;
	}
	
	// ウインドウオープン演出処理
	dmTitleSetWinCloseEfct(main_work);
	
	// ウインドウ描画
//	dmTitleWinSelectDraw(main_work);
}
#endif //_PC || _PS3 || _XBOX || _IPHONE



// ==========================================================================
// dmTitleProcDataRelease
/*!
	ファイル解放リクエスト処理
 */
// ==========================================================================
void dmTitleProcDataRelease(DMS_TITLE_MAIN_WORK *main_work)
{
	if (DmTitleOpExitEndCheck()) {
		// テクスチャ解放
		for (int i = 0; i < DME_TITLE_DATA_TYPE_MAX; i++) {
			AoTexRelease(&main_work->tex[i]);
		}
		
		// メニュー共通テクスチャ解放
		for (int i = 0; i < 4; i++) {
			AoTexRelease(&main_work->cmn_tex[i]);
		}
		
		// オープニング解放処理
		DmTitleOpFlush();
		
#if _WII
		DmFileSlctFlush();
		DmUserNameFlush();
#endif

#if _PC || _PS3 || _XBOX || _IPHONE
		DmBuyScreenFlushStart(&main_work->buy_scr_work);
#endif //_PC || _PS3 || _XBOX || _IPHONE

		// 次へ遷移
		main_work->proc_update = dmTitleProcFinish;
	}
}


// ==========================================================================
// dmTitleProcFinish
/*!
	終了処理
 */
// ==========================================================================
void dmTitleProcFinish(DMS_TITLE_MAIN_WORK *main_work)
{
	// テクスチャ解放完了判定
	if (dmTitleIsTexRelease(main_work)) {
		
#if _IPHONE	//当たり判定解放
		{	//選択肢
			for (er::CTrgAoAction *slct = main_work->trg_slct, *slct_end = main_work->trg_slct + arrayof(main_work->trg_slct); slct != slct_end; ++slct) {
				slct->Release();
				slct->~CTrgAoAction();
			}
		}
		{	//ウインドウ・はい/いいえ
			for (er::CTrgAoAction *answer = main_work->trg_answer, *answer_end = main_work->trg_answer + arrayof(main_work->trg_answer); answer != answer_end; ++answer) {
				answer->Release();
				answer->~CTrgAoAction();
			}
		}
		{	//戻る
			er::CTrgAoAction &ret = main_work->trg_return;
			ret.Release();
			ret.~CTrgAoAction();
		}
		{	//他のゲーム
#ifndef SONIC4_TRIAL_EXIBITION				
			er::CTrgAoAction &ret = main_work->trg_game;
			ret.Release();
			ret.~CTrgAoAction();
#endif
		}
#endif //_IPHONE	//当たり判定解放

		for (int i = 0; i < ACT_NUM; i++) {
			if (main_work->act[i]) {
				AoActDelete(main_work->act[i]);
				main_work->act[i] = NULL;
			}
		}

		// アクション解放
		for (int i = 0; i < DME_TITLE_DATA_TYPE_MAX; i++) {
			// ファイル解放
			if (main_work->arc_amb[i]) {
				amMemFree(main_work->arc_amb[i]);
				main_work->arc_amb[i] = NULL;
			}
		}
		
		// アクション解放
		for (int i = 0; i < 4; i++) {
			// ファイル解放
			if (main_work->arc_cmn_amb[i]) {
				amMemFree(main_work->arc_cmn_amb[i]);
				main_work->arc_cmn_amb[i] = NULL;
			}
		}
		
#if _WII
		for (int i = 0; i < DME_TITLE_DATA_TYPE_MAX; i++) {
			if (main_work->file_arc_amb[i]) {
				amMemFree(main_work->file_arc_amb[i]);
				main_work->file_arc_amb[i] = NULL;
			}
			
			if (main_work->user_arc_amb[i]) {
				amMemFree(main_work->user_arc_amb[i]);
				main_work->user_arc_amb[i] = NULL;
			}
		}

		if (main_work->cmn_win_amb) {
			amMemFree(main_work->cmn_win_amb);
			main_work->cmn_win_amb = NULL;
		}
#endif
		
#if _PC || _PS3 || _XBOX || _IPHONE
		DmBuyScreenRelease(&main_work->buy_scr_work);
#endif //_PC || _PS3 || _XBOX || _IPHONE
		// オープニング解放
		DmTitleOpRelease();
		
		// 終了処理へ
		main_work->proc_update = dmTitleProcWaitFinished;
	}
}


// ==========================================================================
// dmTitleProcWaitFinished
/*!
	終了処理
 */
// ==========================================================================
void dmTitleProcWaitFinished(DMS_TITLE_MAIN_WORK *main_work)
{
	if (DmTitleOpReleaseCheck()) {
		if (main_work->flag & DMD_TITLE_FLAG_DEMO_SND_END) {
			if (!DmSndBgmPlayerIsTaskExit()) {
				return;
			}
			
			main_work->flag &= ~DMD_TITLE_FLAG_DEMO_SND_END;
		}
		
		// 終了処理へ
		main_work->flag |= DMD_TITLE_FLAG_EXIT;
		main_work->proc_update = NULL;
	}
}



#if _XBOX
// ==========================================================================
// dmTitleProcGameExitIdle
/*!
	XBOX360専用のゲーム終了処理
 */
// ==========================================================================
void dmTitleProcGameExitIdle(DMS_TITLE_MAIN_WORK *main_work)
{
	main_work->timer++;
	
	// 一定時間、ゲーム終了待ちまで待つ
	if (main_work->timer > DMD_TITLE_END_GAME_WAIT_TIME
		&& IzFadeIsEnd()) {
		main_work->timer = 0;
		
		main_work->proc_update = NULL;
		
		// ゲーム終了処理開始
		GsReqExit();
	}
}
#endif



// ==========================================================================
// dmTitleInputProcTitle
/*!
	タイトル入力プロシージャ処理
 */
// ==========================================================================
void dmTitleInputProcTitle(DMS_TITLE_MAIN_WORK *main_work)
{
	u16 pad_key = 0;
	s32 pad_id = 0;
	
#if _WII
	pad_key = GSD_KEY_DECIDE;
#else
	pad_key = KEY_START;
#endif
	
	// 終了判定(次のイベント遷移決定)
#if !_IPHONE
	if (AoPadSomeoneStand(pad_key) >= 0) {
		// スタートボタンを押したパッド番号を取得
		pad_id = AoPadSomeoneStand(pad_key);
#else //!_IPHONE
	if (amTpIsTouchPush(0)) {
		pad_id = 0;
#endif //!_IPHONE

		// カレントパッドIDを設定
		AoAccountSetCurrentIdStart((u16)pad_id);
		
		main_work->flag |= DMD_TITLE_FLAG_GAME_START;
	}
}



// ==========================================================================
// dmTitleInputProcMainMenu
/*!
	メインメニュー入力プロシージャ処理
 */
// ==========================================================================
void dmTitleInputProcMainMenu(DMS_TITLE_MAIN_WORK *main_work)
{
#if !_IPHONE
	s32 now_slct_menu = 0;
	s32 cur_edge_menu_top = 0;
	s32 cur_edge_menu_bottom = 0;
	
	// 体験版・初期データ・データありで項目を切り替える
	if (main_work->cur_slct_menu > 0) {
#if !_WII
		if (GsTrialIsTrial()
			|| main_work->is_init_play) {
			now_slct_menu = main_work->cur_slct_menu + 1;
		}
		else {
			now_slct_menu = main_work->cur_slct_menu;
		}
#else
		now_slct_menu = main_work->cur_slct_menu;
#endif
	}
	
	// キャンセル処理
	if (AoPadStand() & GSD_KEY_CANCEL) {
		main_work->flag |= DMD_TITLE_FLAG_CANCEL;

		return;
	}
	
	// 決定処理
	if (AoPadStand() & GSD_KEY_DECIDE) {
		main_work->flag |= DMD_TITLE_FLAG_DECIDE;
		
		return;
	}
	
	
#if !_WII
	// 体験版時の上下の端に当たるメニュー項目
	if (GsTrialIsTrial()) {
		cur_edge_menu_top = main_work->slct_menu_num - 1;
		cur_edge_menu_bottom = main_work->slct_menu_num - 2;
	}
	// 製品版時の上下の端に当たるメニュー項目
	else {
		cur_edge_menu_top = 0;
		cur_edge_menu_bottom = main_work->slct_menu_num - 1;
	}
#else
	cur_edge_menu_top = 0;
	cur_edge_menu_bottom = main_work->slct_menu_num - 1;
#endif
	
	// 十字キー操作
	if (AoPadMRepeat() & GSD_KEY_UP) {
		if (AoPadMStand() & GSD_KEY_UP
			|| main_work->cur_slct_menu != cur_edge_menu_top) {
			// 選択ZONE切り替え
			main_work->flag |= DMD_TITLE_FLAG_MENU_UP_INPUT;
		}
	}
	else if (AoPadMRepeat() & GSD_KEY_DOWN) {
		if (AoPadMStand() & GSD_KEY_DOWN
			|| main_work->cur_slct_menu != cur_edge_menu_bottom) {
			// 選択ZONE切り替え
			main_work->flag |= DMD_TITLE_FLAG_MENU_DOWN_INPUT;
		}
	}
#else //!_IPHONE
	for (int i = 0, max = arrayof(main_work->trg_slct); i < max; ++i) {
		er::CTrgAoAction &slct = main_work->trg_slct[i];
		if (mppUtil::isCommunityButtonPressed()==false && slct.GetState(0)[er::CTrgState::EState::Up] && slct.GetState(0)[er::CTrgState::EState::Prev]) {
			//選択
			main_work->cur_slct_menu = i;
			main_work->flag |= DMD_TITLE_FLAG_DECIDE;
			mpp_newGameButNotFirstGame = (!main_work->is_init_play && (i==0));
			if(!mpp_newGameButNotFirstGame) {
				mppUtil::hideCommunityButtonWithAnimation(); //sss (help, continue, new game)
			}
			else {
				//mppUtil::showCommunityButton(false);//kolya (help, continue, new game)//
			}
			break;
		}
	}
	if (!(DMD_TITLE_FLAG_DECIDE & main_work->flag)) {
		er::CTrgAoAction &ret = main_work->trg_return;
		if (mppUtil::isCommunityButtonPressed()==false && ret.GetState(0)[er::CTrgState::EState::Up] && ret.GetState(0)[er::CTrgState::EState::Prev]) {
			main_work->flag |= DMD_TITLE_FLAG_CANCEL;
		}
		
#ifndef SONIC4_TRIAL_EXIBITION	
		er::CTrgAoAction &game = main_work->trg_game;
		if (mppUtil::isCommunityButtonPressed()==false && game.GetState(0)[er::CTrgState::EState::Up] && game.GetState(0)[er::CTrgState::EState::Prev]) {
			//ウェブ転送動作
			/*qqq
			const char *c_url = "http://sega.com/apps";
			DmSoundPlaySE("Ok");
			er::web::StartWeb(c_url);*/
			//qqq - sega more games
		
			DmSoundPlaySE("Ok");
			mppUtil::launchMoreGames();
		}
#endif
	}
#endif //!_IPHONE
}



#if _PC || _XBOX || _PS3 || _IPHONE
// ==========================================================================
// dmTitleInputProcWindow
/*!
	ウインドウ非表示用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmTitleInputProcWindow(DMS_TITLE_MAIN_WORK *main_work)
{
#if !_IPHONE
	// キャンセル判定
	if (AoPadStand() & GSD_KEY_CANCEL) {
		// フラグON
		main_work->flag |= DMD_TITLE_FLAG_CANCEL;
	}
	
	// 決定判定
	if (AoPadStand() & GSD_KEY_DECIDE) {
		// フラグON
		main_work->flag |= DMD_TITLE_FLAG_DECIDE;
	}
	
	// 十字キー操作
	if (AoPadMRepeat() & GSD_KEY_LEFT) {
		if (main_work->win_cur_slct != 0) {
			// 選択ZONE切り替え
			main_work->win_cur_slct = 0;
			
			// カーソルSE再生
			DmSoundPlaySE("Cursol");
		}
	}
	else if (AoPadMRepeat() & GSD_KEY_RIGHT) {
		if (main_work->win_cur_slct != 1) {
			// 選択ZONE切り替え
			main_work->win_cur_slct = 1;
			
			// カーソルSE再生
			DmSoundPlaySE("Cursol");
		}
	}
#else //!_IPHONE
	// キャンセル判定
	const er::CTrgAoAction *answer = main_work->trg_answer;
	if (answer[0].GetState(0)[er::CTrgState::EState::Up] && answer[0].GetState(0)[er::CTrgState::EState::Prev]) {
		// フラグON
#if 0 //戻るの決定エミュレート
		main_work->flag |= DMD_TITLE_FLAG_CANCEL;
#else //0 //戻るの決定エミュレート
		main_work->win_cur_slct = 1;
		main_work->flag |= DMD_TITLE_FLAG_DECIDE;
#endif //0 //戻るの決定エミュレート
	}
	
	// 決定判定
	if (answer[1].GetState(0)[er::CTrgState::EState::Up] && answer[1].GetState(0)[er::CTrgState::EState::Prev]) {
		// フラグON
		main_work->win_cur_slct = 0;
		main_work->flag |= DMD_TITLE_FLAG_DECIDE;
	}
#endif //!_IPHONE
}
#endif //_PC || _XBOX || _PS3 || _IPHONE



// ==========================================================================
// dmTitleDrawSetProcDispData
/*!
	表示設定処理
 */
// ==========================================================================
void dmTitleDrawSetProcDispData(DMS_TITLE_MAIN_WORK *main_work)
{
	float tmp_win_size = 0;
	
	// 描画フラグONならば
	if(
/*	   
#ifndef	SONIC4_TRIAL	
	   mppUtil::isDrawMainMenu() &&
#endif*/
		main_work->disp_flag & DMD_TITLE_DISP_FLAG_ALL_ACTION) {
		// 共通描画設定部
		AoActSysSetDrawTaskPrio(0x8000);
		DmTitleOpDraw2D();
		
		AoActSortExecute();
		AoActSortDraw();
		AoActSortUnregAll();
				
		// 場面別描画設定部
		if (main_work->flag & DMD_TITLE_FLAG_DISP_TITLE
			&& DmTitleOpIsLogoActFinish()) {
			dmTitleDrawProcTitle(main_work);
		}		
	
		// メインメニュー用ウインドウ
		if (main_work->flag & DMD_TITLE_FLAG_DISP_MMENU_WIN) {
			
			// ウインドウ用AO描画プライオリティ設定
			AoActSysSetDrawTaskPrio(DMD_TITLE_DRAW_PRIO_WIN_FIX);
			
#if _PC || _XBOX || _PS3
			if (!GsTrialIsTrial()
			&& main_work->is_init_play) {
				tmp_win_size = -50;
			}
			else {
				tmp_win_size = 0;
			}
#endif
			
			// サインアウト中に開閉演出中だった場合はウインドウを非表示
			if (main_work->mmenu_win_size_rate[0] < 0.9
				&& !AoAccountIsCurrentEnable()) {
				// 空処理
			}
			else {
				// ウインドウ描画
#if !_IPHONE
				AoWinSysDrawState(AOD_WIN_TYPE_A
								 , AoTexGetTexList(&main_work->cmn_tex[2])			// ◆
								 , 0
								 , DMD_TITLE_SIZE_WIDTH / 2.0f			// ウインドウ中心X
								 , DMD_TITLE_SIZE_HEIGHT / 2.0f + 130.f			// ウインドウ中心Y
								 , DMD_TITLE_MMENU_WIN_SIZE_W * main_work->mmenu_win_size_rate[0]					// ウインドウ横サイズ
								 , DMD_TITLE_MMENU_WIN_SIZE_H * main_work->mmenu_win_size_rate[1] + tmp_win_size	// ウインドウ縦サイズ
								 , DMD_TITLE_DRAW_STATE_ID				// 描画STATE
								 );
#else //!_IPHONE
				s32 win_size[AMD_XY] = {DMD_TITLE_MMENU_WIN_SIZE_W, DMD_TITLE_MMENU_WIN_SIZE_H};
				bool is_trial = GsTrialIsTrial();
				bool has_save_data = !main_work->is_init_play;
				bool is_disp_2nd_elm = ((is_trial)? true: has_save_data); //for SONIC4_TRIAL_EXIBITION set it false
				if (!is_disp_2nd_elm) {
					win_size[AMD_X] = DMD_TITLE_MMENU_WIN_SIZE_W_2;
					win_size[AMD_Y] = DMD_TITLE_MMENU_WIN_SIZE_H_2;
				}
#ifndef	SONIC4_TRIAL				
				if(mppUtil::isDrawMainMenu())
#endif
				{//black bg rect
					AoWinSysDrawState(AOD_WIN_TYPE_A
								 , AoTexGetTexList(&main_work->cmn_tex[2])			// ◆
								 , 0
								 , DMD_TITLE_SIZE_WIDTH / 2.0f			// ウインドウ中心X
								 , 311.f										// ウインドウ中心Y
								 , win_size[AMD_X] * main_work->mmenu_win_size_rate[0]					// ウインドウ横サイズ
								 , win_size[AMD_Y] * main_work->mmenu_win_size_rate[1] + tmp_win_size	// ウインドウ縦サイズ
								 , DMD_TITLE_DRAW_STATE_ID				// 描画STATE
								 );
				}


#endif //!_IPHONE
			}
		}
#ifndef	SONIC4_TRIAL	
		if(mppUtil::isDrawMainMenu())
#endif
		{
			if (main_work->flag & DMD_TITLE_FLAG_DISP_MAINMENU) {
#if _IPHONE
				if (!(main_work->flag_prev & DMD_TITLE_FLAG_DISP_MAINMENU)) {
					//DMD_TITLE_FLAG_DISP_MAINMENUのONエッジ					
					main_work->trg_return.ResetState();
					for (int k = ACT_BACK_BTN_L, max = ACT_BACK_BTN_R + 1; k < max; ++k) {
						AoActSetFrame(main_work->act[k], 0.0f);
					}
					//戻るボタンの演出を戻す
#ifndef SONIC4_TRIAL_EXIBITION			
					main_work->trg_game.ResetState();
					for (int k = ACT_GAME_BTN_L, max = ACT_GAME_BTN_R + 1; k < max; ++k) {
						AoActSetFrame(main_work->act[k], 0.0f);
					}
#endif					
				}
#endif //_IPHONE
				dmTitleDrawProcMainMenu(main_work);//draw softkeys and menu items
			}
		}
			
#if _PC || _XBOX || _PS3 || _IPHONE
		if (main_work->flag & DMD_TITLE_FLAG_WIN_DRAW_START) {
			// ウインドウ描画
			dmTitleWinSelectDraw(main_work);
		}
#endif //_PC || _XBOX || _PS3 || _IPHONE
	}
	
	
	// 描画タスク生成
	amDrawMakeTask(dmTitleTaskDraw, (u16)0x8000, (u32)0);
}



// ==========================================================================
// dmTitleDrawProcTitle
/*!
	タイトル時表示設定処理
 */
// ==========================================================================
void dmTitleDrawProcTitle(DMS_TITLE_MAIN_WORK *main_work)
{
	AoActSysSetDrawTaskPrio(DMD_TITLE_DRAW_PRIO_ACT);
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
	if (main_work->flag & DMD_TITLE_FLAG_DISP_TEXT) {
		AoActSortRegAction(main_work->act[ACT_TEX_START]);
	}
	
	if (main_work->flag & DMD_TITLE_FLAG_DECIDE_EFCT) {
		if (main_work->disp_timer >= DMD_TITLE_DECIDE_BLINK_TIME) {
			main_work->disp_timer = 0;
			main_work->flag ^= DMD_TITLE_FLAG_DISP_TEXT;
		}
		else {
			// タイマー更新
			main_work->disp_timer++;
		}
	}
	
	else {
		if (main_work->disp_timer >= main_work->disp_change_time) {
			main_work->flag ^= DMD_TITLE_FLAG_DISP_TEXT;
	
			if (main_work->flag & DMD_TITLE_FLAG_DISP_TEXT) {
				main_work->disp_change_time = DMD_TITLE_IDLE_DISP_TIME;
			}
			else {
				main_work->disp_change_time = DMD_TITLE_IDLE_NONE_TIME;
			}
			
			main_work->disp_timer = 0;
		}
		else {
			// タイマー更新
			main_work->disp_timer++;
		}
	}
	
	//
	if (GeEnvGetDecideKey() == GSD_DECIDE_KEY_O) {
		AoActSetFrame(main_work->act[ACT_TEX_START], 0.f);
	}
	else {
		AoActSetFrame(main_work->act[ACT_TEX_START], 1.f);
	}
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
	AoActUpdate(main_work->act[ACT_TEX_START], 0.f);
	
	// ソート描画
	AoActSortExecute();
	AoActSortDraw();
	AoActSortUnregAll();
}



// ==========================================================================
// dmTitleDrawProcMainMenu
/*!
	メインメニュー時表示設定処理
 */
// ==========================================================================
void dmTitleDrawProcMainMenu(DMS_TITLE_MAIN_WORK *main_work)
{
#if !_IPHONE
	float init_slct_dist = 0.f;
	int init_dst_menu_data = 0;
	
	AoActSysSetDrawTaskPrio(DMD_TITLE_DRAW_PRIO_ACT);
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	AoActSortRegAction(main_work->act[ACT_MENU_CRSR]);
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
	if (GsTrialIsTrial()) {
		dmTitleDrawProcMainMenuTrial(main_work);
	}
	
	else if (main_work->is_init_play) {
		dmTitleDrawProcMainMenuFirst(main_work);
	}
	
	else {
		dmTitleDrawProcMainMenuPlayed(main_work);
	}
	
	
	for (int i = ACT_TEX_GAME; i < ACT_BTN_CANCEL; i++) {
		AoActSetFrame(main_work->act[i], 1.f);
	}
	
	// 決定演出用フレーム設定
	AoActSetFrame(main_work->act[ACT_TEX_GAME_L]
				  , main_work->decide_menu_frm[DME_TITLE_MENU_TYPE_START]);
#if _PC || _XBOX || _PS3
	AoActSetFrame(main_work->act[ACT_TEX_TUDUKI_L]
				  , main_work->decide_menu_frm[DME_TITLE_MENU_TYPE_TUDUKI]);
#endif //_PC || _XBOX || _PS3
	AoActSetFrame(main_work->act[ACT_TEX_OPTION_L]
				  , main_work->decide_menu_frm[DME_TITLE_MENU_TYPE_OPTION]);
	AoActSetFrame(main_work->act[ACT_TEX_RANK_L]
				  , main_work->decide_menu_frm[DME_TITLE_MENU_TYPE_RANK]);
#if _PS3
	AoActSetFrame(main_work->act[ACT_TEX_ZISSEKI_L]
				  , main_work->decide_menu_frm[DME_TITLE_MENU_TYPE_BUY]);
#elif _PC || _XBOX
	AoActSetFrame(main_work->act[ACT_TEX_ZISSEKI_L]
				  , main_work->decide_menu_frm[DME_TITLE_MENU_TYPE_ZISSEKI]);
	AoActSetFrame(main_work->act[ACT_TEX_LIBRARY_L]
				  , main_work->decide_menu_frm[DME_TITLE_MENU_TYPE_LIBRARY]);
	AoActSetFrame(main_work->act[ACT_TEX_BUY_L]
				  , main_work->decide_menu_frm[DME_TITLE_MENU_TYPE_BUY]);
#endif
	
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	AoActAcmPush();
	AoActAcmInit();
	AoActAcmApplyTrans(DMD_TITLE_CRSR_DFLT_POS_X
					   , main_work->cur_crsr_pos_y
					   , 0.f
					   );
	
#if _PC || _XBOX || _PS3
	if (!GsTrialIsTrial()
	&& main_work->is_init_play) {
		AoActAcmApplyTrans(0.f, 25.f, 0.f
					   );
	}
#endif
	
	AoActUpdate(main_work->act[ACT_MENU_CRSR], 0.0f);
	AoActAcmPop();
	
#if _PC || _XBOX
//	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[0]));
//	AoActUpdate(main_work->act[ACT_BTN_X], 0.0f);
#endif
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
	// ここでPFごとの表示アクションを設定
#if _PC || _XBOX
	init_dst_menu_data = ACT_TEX_BUY_L;
#elif _PS3
	init_dst_menu_data = ACT_TEX_ZISSEKI_L;
#elif _WII
	init_dst_menu_data = ACT_TEX_RANK_L;
#endif
	
	if (GsTrialIsTrial()) {
		AoActAcmPush();
		
		for (int i = ACT_TEX_GAME; i < ACT_BTN_CANCEL; i++) {
			AoActAcmInit();
			
			if (i == ACT_TEX_GAME_L
				|| i == ACT_TEX_GAME) {
				init_slct_dist = 50.f;
			}
			else {
				init_slct_dist = 0.f;
			}
			
#if _PC || _XBOX || _PS3
			AoActAcmApplyTrans(0.f
							   , init_slct_dist
							   , 0.f
							   );
#endif
			
			AoActUpdate(main_work->act[i], 0.f);
		}
		AoActAcmPop();
	}
	else if (GsTrialIsTrial()
		|| main_work->is_init_play) {
		AoActAcmPush();
		
		for (int i = ACT_TEX_GAME; i < ACT_BTN_CANCEL; i++) {
			AoActAcmInit();
			
			if (i >= ACT_TEX_OPTION && i <= init_dst_menu_data) {
				init_slct_dist = -50.f;
			}
			else {
				init_slct_dist = 0.f;
			}
			
#if _PC || _XBOX || _PS3
			AoActAcmApplyTrans(0.f
							   , init_slct_dist
							   , 0.f
							   );
			
			if (!GsTrialIsTrial()
			&& main_work->is_init_play) {
				AoActAcmApplyTrans(0.f, 25.f, 0.f
							   );
			}
#endif
			
			AoActUpdate(main_work->act[i], 0.f);
		}
		AoActAcmPop();
	}
	else {
		for (int i = ACT_TEX_GAME; i < ACT_BTN_CANCEL; i++) {
			AoActUpdate(main_work->act[i], 0.f);
		}
	}
	
	// ソート描画
	AoActSortExecute();
	AoActSortDraw();
	AoActSortUnregAll();

#else //!_IPHONE
	bool is_trial = GsTrialIsTrial();
	bool has_save_data = !main_work->is_init_play;
#if defined(AMD_DEBUG)
	//暫定的に“つづき”を強制表示(20100209-1230)
#if TARGET_IPHONE_SIMULATOR
	if (amTpIsTouchOn(0)) {
#else //TARGET_IPHONE_SIMULATOR
	if (amTpIsTouchOn(2)) {
#endif //TARGET_IPHONE_SIMULATOR
		has_save_data = true;
	}
#endif
	dmTitleDrawProcMainMenuIphone(main_work, has_save_data, is_trial);
#endif //!_IPHONE
}



#if !_IPHONE
// ==========================================================================
// dmTitleDrawProcMainMenuTrial
/*!
	メインメニュー時表示設定処理
 */
// ==========================================================================
void dmTitleDrawProcMainMenuTrial(DMS_TITLE_MAIN_WORK *main_work)
{
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
	switch (main_work->cur_slct_menu) {
	case 0:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME_L]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
#if !_WII
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI]);
#if _PC || _XBOX
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
		AoActSortRegAction(main_work->act[ACT_TEX_BUY]);
#endif
#endif
		AoActSetFrame(main_work->act[ACT_TEX_GAME], 0.f);
		break;
	case 1:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION_L]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
#if !_WII
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI]);
#if _PC || _XBOX
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
		AoActSortRegAction(main_work->act[ACT_TEX_BUY]);
#endif
#endif
		AoActSetFrame(main_work->act[ACT_TEX_OPTION], 0.f);
		break;
	case 2:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK_L]);
#if !_WII
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI]);
#if _PC || _XBOX
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
		AoActSortRegAction(main_work->act[ACT_TEX_BUY]);
#endif
#endif
		AoActSetFrame(main_work->act[ACT_TEX_RANK], 0.f);
		break;

#if _PS3
	case 3:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI_L]);
#if _PC || _XBOX
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
		AoActSortRegAction(main_work->act[ACT_TEX_BUY]);
#endif
		break;
#endif
#if _PC || _XBOX
	case 3:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI_L]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
		AoActSortRegAction(main_work->act[ACT_TEX_BUY]);
		AoActSetFrame(main_work->act[ACT_TEX_ZISSEKI], 0.f);
		break;
	case 4:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY_L]);
		AoActSortRegAction(main_work->act[ACT_TEX_BUY]);
		AoActSetFrame(main_work->act[ACT_TEX_LIBRARY], 0.f);
		break;
	case 5:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
		AoActSortRegAction(main_work->act[ACT_TEX_BUY_L]);
		AoActSetFrame(main_work->act[ACT_TEX_BUY_L], 0.f);
		break;
#endif
	default:
		break;
	}
}
#endif //!_IPHONE



#if !_IPHONE
// ==========================================================================
// dmTitleDrawProcMainMenuFirst
/*!
	メインメニュー時表示設定処理
 */
// ==========================================================================
void dmTitleDrawProcMainMenuFirst(DMS_TITLE_MAIN_WORK *main_work)
{
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
	switch (main_work->cur_slct_menu) {
	case 0:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME_L]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
#if _PC || _XBOX
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
#endif
		AoActSetFrame(main_work->act[ACT_TEX_GAME], 0.f);
		break;
	case 1:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION_L]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
#if _PC || _XBOX
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
#endif
		AoActSetFrame(main_work->act[ACT_TEX_OPTION], 0.f);
		break;
	case 2:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK_L]);
#if _PC || _XBOX
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
#endif
		AoActSetFrame(main_work->act[ACT_TEX_RANK], 0.f);
		break;

#if _PS3
	case 3:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
#if _PC || _XBOX
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI_L]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
#endif
		break;
#endif
#if _PC || _XBOX
	case 3:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI_L]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
		AoActSetFrame(main_work->act[ACT_TEX_ZISSEKI], 0.f);
		break;
	case 4:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY_L]);
		AoActSetFrame(main_work->act[ACT_TEX_LIBRARY], 0.f);
		break;
#endif
	default:
		break;
	}
	
}
#endif //!_IPHONE



#if !_IPHONE
// ==========================================================================
// dmTitleDrawProcMainMenuPlayed
/*!
	メインメニュー時表示設定処理
 */
// ==========================================================================
void dmTitleDrawProcMainMenuPlayed(DMS_TITLE_MAIN_WORK *main_work)
{
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
	switch (main_work->cur_slct_menu) {
	case DME_TITLE_MENU_TYPE_START:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME_L]);
#if !_WII
		AoActSortRegAction(main_work->act[ACT_TEX_TUDUKI]);
#endif
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
#if _PC || _XBOX
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
#endif
		AoActSetFrame(main_work->act[ACT_TEX_GAME], 0.f);
		break;
#if !_WII
	case DME_TITLE_MENU_TYPE_TUDUKI:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_TUDUKI_L]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
#if _PC || _XBOX
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
#endif
		AoActSetFrame(main_work->act[ACT_TEX_OPTION], 0.f);
		break;
#endif
	case DME_TITLE_MENU_TYPE_OPTION:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
#if !_WII
		AoActSortRegAction(main_work->act[ACT_TEX_TUDUKI]);
#endif
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION_L]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
#if _PC || _XBOX
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
#endif
		AoActSetFrame(main_work->act[ACT_TEX_OPTION], 0.f);
		break;
	case DME_TITLE_MENU_TYPE_RANK:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
#if !_WII
		AoActSortRegAction(main_work->act[ACT_TEX_TUDUKI]);
#endif
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK_L]);
#if _PC || _XBOX
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
#endif
		AoActSetFrame(main_work->act[ACT_TEX_RANK], 0.f);
		break;

#if _PS3
	case DME_TITLE_MENU_TYPE_BUY:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_TUDUKI]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI_L]);
#if _PC || _XBOX
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
#endif
		break;
#endif
#if _PC || _XBOX
	case DME_TITLE_MENU_TYPE_ZISSEKI:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_TUDUKI]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI_L]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY]);
		AoActSetFrame(main_work->act[ACT_TEX_ZISSEKI], 0.f);
		break;
	case DME_TITLE_MENU_TYPE_LIBRARY:
		AoActSortRegAction(main_work->act[ACT_TEX_GAME]);
		AoActSortRegAction(main_work->act[ACT_TEX_TUDUKI]);
		AoActSortRegAction(main_work->act[ACT_TEX_OPTION]);
		AoActSortRegAction(main_work->act[ACT_TEX_RANK]);
		AoActSortRegAction(main_work->act[ACT_TEX_ZISSEKI]);
		AoActSortRegAction(main_work->act[ACT_TEX_LIBRARY_L]);
		AoActSetFrame(main_work->act[ACT_TEX_LIBRARY], 0.f);
		break;
#endif
	default:
		break;
	}
	
}
#endif //!_IPHONE



#if _IPHONE
// ==========================================================================
// dmTitleDrawProcMainMenuIphone
/*!
	メインメニュー時表示設定処理(iPhone)

	@note
								is_trial
								true		false
		has_save_data	true	ASSERT		製品続き	
						false	体験版		製品初回
	
		製品初回	はじめ・遊び方&オプション
		製品続き	はじめ・つづきから・遊び方&オプション	
		体験版	はじめ・購入・遊び方&オプション	
 */
// ==========================================================================
void dmTitleDrawProcMainMenuIphone(DMS_TITLE_MAIN_WORK *main_work, bool has_save_data, bool is_trial)
{
	//ボタン台紙の設定
	struct CLocalBtnBase {
		struct EBtn {
			enum Type {
				Left,
				Center,
				Right,
			
				Max,
				None,
			};
		};
		typedef AOS_ACTION *TBtn[EBtn::Max];
		static void Update(const AOS_ACTION *src, TBtn btn, er::CTrgAoAction &trg, f32 frame = 0.0f) {
			AoActAcmPush();
			AoActAcmInit();
			fx32 pos[AMD_XY] = {src->sprite->center_x, src->sprite->center_y};
			AoActAcmApplyTrans(pos[AMD_X], pos[AMD_Y], 1.0f);
			AoActUpdate(btn[0], frame);
			AoActUpdate(btn[1], frame);
			AoActUpdate(btn[2], frame);
			trg.Update();
			AoActAcmPop();
		}
		static void Draw(TBtn btn) {
			AoActSortRegAction(btn[0]);
			AoActSortRegAction(btn[1]);
			AoActSortRegAction(btn[2]);
		}
		static void SetFrame(TBtn btn, f32 frame) {
			AoActSetFrame(btn[0], frame);
			AoActSetFrame(btn[1], frame);
			AoActSetFrame(btn[2], frame);
		}
	};
	struct EType {
		enum Type {
			Dark,
			Normal,
			
			Max,
			None,
		};
	};
	
	//文字の設定
	struct EAct {
		enum Type {
			Game,
			Tuduki,
			Option,
			
			Max,
			None,
		};
	};
	
	//描画前処理
	AoActSysSetDrawTaskPrio(DMD_TITLE_DRAW_PRIO_ACT);

	//準備
	int act_id[EAct::Max] = {ACT_TEX_GAME, ACT_TEX_TUDUKI, ACT_TEX_OPTION};
	bool is_disp_2nd_elm = has_save_data;
	int base_id[EAct::Max] = {ACT_BTN_L, ACT_BTN_L2, ACT_BTN_L3};
	f32 update_frame[EAct::Max] = {0.0f, 0.0f, 0.0f};
	typedef er::CTrgState::EState EState;

	//体験版判定
	if (is_trial) {
		//体験版なら
		act_id[EAct::Tuduki] = ACT_TEX_KANZENBAN;
		is_disp_2nd_elm = true; //for SONIC4_TRIAL_EXIBITION set it false
	}


	if (dmTitleProcMainMenuDecideEfct == main_work->proc_update) {
		//決定演出中なら決定項目のみフレームを進め、新規演出は開始しない
		update_frame[main_work->cur_slct_menu] = 1.0f;
	} else if (IzFadeIsExe() && !IzFadeIsEnd()) {
		//フェードアウト中なら演出を止める
	} else if (dmTitleProcSaveInitData == main_work->proc_update) {
		//セーブデータ初期化中なら演出を止める
	} else if (DMD_TITLE_FLAG_WIN_DRAW_START & main_work->flag) {
		//ウインドウ表示中なら演出を止める
	} else {
		//選択中なら選択状態を更新する
		f32 frame;
		for (int i = 0; i < EAct::Max; ++i) {
			if (main_work->trg_slct[i].GetState(0)[EState::Up] && main_work->trg_slct[i].GetState(0)[EState::Prev]) {
				//決定
				frame = 2.0f;
			} else if (main_work->trg_slct[i].GetState(0)[EState::On]) {
				//ON
				frame = 1.0f;
			} else {
				//OFF
				frame = 0.0f;
			}

			AoActSetFrame(main_work->act[act_id[i]], frame);
			CLocalBtnBase::SetFrame(&main_work->act[base_id[i]], frame);
		}
		//戻るボタン
		{
			if (main_work->trg_return.GetState(0)[EState::Up] && main_work->trg_return.GetState(0)[EState::Prev]) {
				//決定
				frame = 2.0f;
			} else if (main_work->trg_return.GetState(0)[EState::On]) {
				//ON
				frame = 1.0f;
			} else if (2.0f <= main_work->act[ACT_BACK_BTN_L]->frame) {
				//決定演出中
				frame = main_work->act[ACT_BACK_BTN_L]->frame;
			} else {
				//OFF
				frame = 0.0f;
			}

			AoActSetFrame(main_work->act[ACT_TEX_TOP_BACK], frame);
			for (int k = ACT_BACK_BTN_L, max = ACT_BACK_BTN_R + 1; k < max; ++k) {
				AoActSetFrame(main_work->act[k], frame);
			}
		}
		//他のゲームボタン
		{
#ifndef SONIC4_TRIAL_EXIBITION				
			if (main_work->trg_game.GetState(0)[EState::Up] && main_work->trg_game.GetState(0)[EState::Prev]) {
				//決定
				frame = 2.0f;
			} else if (main_work->trg_game.GetState(0)[EState::On]) {
				//ON
				frame = 1.0f;
			} else if (2.0f <= main_work->act[ACT_GAME_BTN_L]->frame) {
				//決定演出中
				frame = main_work->act[ACT_GAME_BTN_L]->frame;
			} else {
				//OFF
				frame = 0.0f;
			}

			AoActSetFrame(main_work->act[ACT_TEX_TOP_GAME], frame);
			for (int k = ACT_GAME_BTN_L, max = ACT_GAME_BTN_R + 1; k < max; ++k) {
				AoActSetFrame(main_work->act[k], frame);
			}
#endif
		}
	}

	//更新
	if (is_disp_2nd_elm) {
		//3択
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		AoActUpdate(main_work->act[act_id[EAct::Game]], update_frame[EAct::Game]);
		AoActUpdate(main_work->act[act_id[EAct::Tuduki]], update_frame[EAct::Tuduki]);
		AoActUpdate(main_work->act[act_id[EAct::Option]], update_frame[EAct::Option]);
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
		CLocalBtnBase::Update(main_work->act[act_id[EAct::Game]], &main_work->act[base_id[EAct::Game]], main_work->trg_slct[EAct::Game], update_frame[EAct::Game]);
		CLocalBtnBase::Update(main_work->act[act_id[EAct::Tuduki]], &main_work->act[base_id[EAct::Tuduki]], main_work->trg_slct[EAct::Tuduki], update_frame[EAct::Tuduki]);
		CLocalBtnBase::Update(main_work->act[act_id[EAct::Option]], &main_work->act[base_id[EAct::Option]], main_work->trg_slct[EAct::Option], update_frame[EAct::Option]);
	} else {
		//2択
		AOS_ACTION *center = main_work->act[act_id[EAct::Tuduki]];
		AoActUpdate(center, 0.0f); //位置確定用更新
		const fx32 center_pos[AMD_XY] = {center->sprite->center_x, center->sprite->center_y};

#if !defined(AMD_DEBUG)
		static const float c_2elm_space_revise[AMD_XY] = {0.5f, 0.5f};	//2択時の空間補正
#else //!defined(AMD_DEBUG)
		//両端タップで幅を変更出来る
		static float c_2elm_space_revise[AMD_XY] = {0.5f, 0.5f};	//2択時の空間補正
		static bool is_init = false;
		static er::CTrgRect trg[2];
		if (!is_init) {
			trg[0].Create(0,0,40,360);
			trg[1].Create(480-40,0,480,360);
			is_init = true;
		}
		trg[0].Update();
		trg[1].Update();
		if (trg[0].GetState(0)[er::CTrgState::EState::Lock] && trg[0].GetState(0)[er::CTrgState::EState::Repeat]) {
			c_2elm_space_revise[AMD_X] += 0.001f;
			c_2elm_space_revise[AMD_Y] += 0.001f;
		} else if (trg[1].GetState(0)[er::CTrgState::EState::Lock] && trg[1].GetState(0)[er::CTrgState::EState::Repeat]) {
			c_2elm_space_revise[AMD_X] -= 0.001f;
			c_2elm_space_revise[AMD_Y] -= 0.001f;
		}
		amPrintf(60, 44, "%6.2f  %6.2f", -(c_2elm_space_revise[AMD_X] - 0.5f) * 100.0f, -(c_2elm_space_revise[AMD_Y] - 0.5f) * 100.0f);
#endif //!defined(AMD_DEBUG)
		AoActAcmPush();
		AoActAcmInit();
		{
			AoActUpdate(main_work->act[act_id[EAct::Game]], 0.0f); //位置確定用更新
			const fx32 pos[AMD_XY] = {(center_pos[AMD_X] - main_work->act[act_id[EAct::Game]]->sprite->center_x) * c_2elm_space_revise[AMD_X]
									, (center_pos[AMD_Y] - main_work->act[act_id[EAct::Game]]->sprite->center_y) * c_2elm_space_revise[AMD_Y]};
			AoActAcmApplyTrans(pos[AMD_X], pos[AMD_Y], 0.0f);
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			AoActUpdate(main_work->act[act_id[EAct::Game]], update_frame[EAct::Game]);
			AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
			CLocalBtnBase::Update(main_work->act[act_id[EAct::Game]], &main_work->act[base_id[EAct::Game]], main_work->trg_slct[EAct::Game], update_frame[EAct::Game]);
		}

		AoActAcmInit();
		{
			AoActUpdate(main_work->act[act_id[EAct::Option]], 0.0f); //位置確定用更新
			const fx32 pos[AMD_XY] = {(center_pos[AMD_X] - main_work->act[act_id[EAct::Option]]->sprite->center_x) * c_2elm_space_revise[AMD_X]
									, (center_pos[AMD_Y] - main_work->act[act_id[EAct::Option]]->sprite->center_y) * c_2elm_space_revise[AMD_Y]};
			AoActAcmApplyTrans(pos[AMD_X], pos[AMD_Y], 0.0f);
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			AoActUpdate(main_work->act[act_id[EAct::Option]], update_frame[EAct::Option]);
			AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
			CLocalBtnBase::Update(main_work->act[act_id[EAct::Option]], &main_work->act[base_id[EAct::Option]], main_work->trg_slct[EAct::Option], update_frame[EAct::Option]);
		}

		AoActAcmPop();
	}
	{	//戻るボタン
		float frame = ((2.0f <= main_work->act[ACT_BACK_BTN_L]->frame)? 1.0f: 0.0f);
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		AoActUpdate(main_work->act[ACT_TEX_TOP_BACK], frame);
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
		for (int k = ACT_BACK_BTN_L, max = ACT_BACK_BTN_R + 1; k < max; ++k) {
			AoActUpdate(main_work->act[k], frame);
		}
		main_work->trg_return.Update();
	}
#ifndef SONIC4_TRIAL_EXIBITION
	{	//他のゲームボタン
		float frame = ((2.0f <= main_work->act[ACT_GAME_BTN_L]->frame)? 1.0f: 0.0f);
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		AoActUpdate(main_work->act[ACT_TEX_TOP_GAME], frame);
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
		for (int k = ACT_GAME_BTN_L, max = ACT_GAME_BTN_R + 1; k < max; ++k) {
			AoActUpdate(main_work->act[k], frame);
		}
		main_work->trg_game.Update();
	}
#endif

	//描画
	CLocalBtnBase::Draw(&main_work->act[base_id[EAct::Game]]);
	AoActSortRegAction(main_work->act[act_id[EAct::Game]]);
	if (is_disp_2nd_elm) {
		CLocalBtnBase::Draw(&main_work->act[base_id[EAct::Tuduki]]);
		AoActSortRegAction(main_work->act[act_id[EAct::Tuduki]]);
	}
	CLocalBtnBase::Draw(&main_work->act[base_id[EAct::Option]]);
	AoActSortRegAction(main_work->act[act_id[EAct::Option]]);
	//枠外ボタン
	if ((DMD_TITLE_FLAG_DISP_MMENU_WIN & main_work->flag) && !(DMD_TITLE_FLAG_WIN_DRAW_START & main_work->flag)) {
		//戻るボタン
		for (int k = ACT_BACK_BTN_L, max = ACT_BACK_BTN_R + 1; k < max; ++k) {
			AoActSortRegAction(main_work->act[k]);
		}
		AoActSortRegAction(main_work->act[ACT_TEX_TOP_BACK]);
		//外のゲームボタン
#ifndef SONIC4_TRIAL_EXIBITION	
		for (int k = ACT_GAME_BTN_L, max = ACT_GAME_BTN_R + 1; k < max; ++k) {
			AoActSortRegAction(main_work->act[k]);
		}
		AoActSortRegAction(main_work->act[ACT_TEX_TOP_GAME]);
#endif
	}

	//描画後処理
	AoActSortExecute();
	AoActSortDraw();
	AoActSortUnregAll();
}
#endif //_IPHONE



#if _PC || _XBOX || _PS3 || _IPHONE
// ==========================================================================
// dmTitleWinSelectDraw
/*!
	ウインドウ用描画設定処理
 */
// ==========================================================================
void dmTitleWinSelectDraw(DMS_TITLE_MAIN_WORK *main_work)
{
	// ウインドウ用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_TITLE_DRAW_PRIO_WIN_FIX);
	
	// ウインドウ描画
	AoWinSysDrawState(AOD_WIN_TYPE_A
					 , AoTexGetTexList(&main_work->cmn_tex[2])			// ◆
					 , 0
					 , DMD_TITLE_SIZE_WIDTH / 2.0f			// ウインドウ中心X
					 , DMD_TITLE_SIZE_HEIGHT / 2.0f			// ウインドウ中心Y
					 , DMD_TITLE_WINDOW_SIZE_W * DMD_TITLE_DISP_SCALE_TEXT * main_work->win_size_rate[0]			// ウインドウ横サイズ
					 , DMD_TITLE_WINDOW_SIZE_H * DMD_TITLE_DISP_SCALE_TEXT * main_work->win_size_rate[1]			// ウインドウ縦サイズ
					 , DMD_TITLE_DRAW_STATE_ID				// 描画STATE
					 );
	
	
	// ウインドウ内の項目描画
	if (main_work->disp_flag & DMD_TITLE_DISP_FLAG_WIN_ACT) {
		// 共通ウインドウACT登録
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[2]));
//		AoActSortRegAction(main_work->act[ACT_WIN_LINE]);
		
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
//		AoActSortRegAction(main_work->act[ACT_TEX_WINTITLE]);
		
		switch (main_work->win_mode) {
		case DME_TITLE_WIN_TYPE_DEL_DATA1:
			// アクション登録
#if _IPHONE
			for (int i = ACT_WIN_NO_BTN_L, max = ACT_WIN_YES_BTN_R + 1; i < max; ++i) {
				AoActSortRegAction(main_work->act[i]);
			}
#endif //_IPHONE
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			AoActSortRegAction(main_work->act[ACT_WIN_TEX_MSG1]);
//			AoActSortRegAction(main_work->act[ACT_WIN_TEX_MSG2]);
			
			AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
			AoActSortRegAction(main_work->act[ACT_TEX_YES]);
			AoActSortRegAction(main_work->act[ACT_TEX_NO]);

#if !_IPHONE
			// 初期表示フレーム設定
			if (main_work->win_cur_slct == 0) {
				AoActSetFrame(main_work->act[ACT_TEX_YES], 0.f);
				AoActSetFrame(main_work->act[ACT_TEX_NO], 1.f);
			}
			
			else {
				AoActSetFrame(main_work->act[ACT_TEX_YES], 1.f);
				AoActSetFrame(main_work->act[ACT_TEX_NO], 0.f);
			}
#endif //!_IPHONE			
			
			break;
			
		case DME_TITLE_WIN_TYPE_DEL_DATA2:
#if _IPHONE
			for (int i = ACT_WIN_NO_BTN_L, max = ACT_WIN_YES_BTN_R + 1; i < max; ++i) {
				AoActSortRegAction(main_work->act[i]);
			}
#endif //_IPHONE
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			AoActSortRegAction(main_work->act[ACT_WIN_TEX_MSG2]);
			
			AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
			AoActSortRegAction(main_work->act[ACT_TEX_YES]);
			AoActSortRegAction(main_work->act[ACT_TEX_NO]);

#if !_IPHONE
			// 初期表示フレーム設定
			if (main_work->win_cur_slct == 0) {
				AoActSetFrame(main_work->act[ACT_TEX_YES], 0.f);
				AoActSetFrame(main_work->act[ACT_TEX_NO], 1.f);
			}
			
			else {
				AoActSetFrame(main_work->act[ACT_TEX_YES], 1.f);
				AoActSetFrame(main_work->act[ACT_TEX_NO], 0.f);
			}
#endif //!_IPHONE
			
			break;
#if !_IPHONE
		case DME_TITLE_WIN_TYPE_DEL_DATA3:
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			AoActSortRegAction(main_work->act[ACT_WIN_TEX_MSG3]);
			
			AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
			AoActSortRegAction(main_work->act[ACT_TEX_YES]);
			AoActSortRegAction(main_work->act[ACT_TEX_NO]);

			// 初期表示フレーム設定
			if (main_work->win_cur_slct == 0) {
				AoActSetFrame(main_work->act[ACT_TEX_YES], 0.f);
				AoActSetFrame(main_work->act[ACT_TEX_NO], 1.f);
			}
			
			else {
				AoActSetFrame(main_work->act[ACT_TEX_YES], 1.f);
				AoActSetFrame(main_work->act[ACT_TEX_NO], 0.f);
			}
			
			break;
			
		case DME_TITLE_WIN_TYPE_DEL_DATA4:
		case DME_TITLE_WIN_TYPE_DEL_DATA5:
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			AoActSortRegAction(main_work->act[ACT_WIN_TEX_MSG3]);
			
			AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
			AoActSortRegAction(main_work->act[ACT_TEX_YES]);
			AoActSortRegAction(main_work->act[ACT_TEX_NO]);

			// 初期表示フレーム設定
			if (main_work->win_cur_slct == 0) {
				AoActSetFrame(main_work->act[ACT_TEX_YES], 0.f);
				AoActSetFrame(main_work->act[ACT_TEX_NO], 1.f);
			}
			
			else {
				AoActSetFrame(main_work->act[ACT_TEX_YES], 1.f);
				AoActSetFrame(main_work->act[ACT_TEX_NO], 0.f);
			}
			
			break;
#endif //!_IPHONE
		default:
			// 例外
			MTM_ASSERT(0);
			break;
		}
		
#if !_IPHONE
		// ACCUMURATEでトランスさせる
		AoActAcmPush();
		
		AoActAcmInit();
		AoActAcmApplyTrans(dm_title_win_act_pos_tbl[0][0]
						   , dm_title_win_act_pos_tbl[0][1]
						   , 0
						   );
		
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[2]));
		
		// フレーム更新はSetFrameのみで行う
		AoActUpdate(main_work->act[ACT_WIN_LINE], 0.0f);
		
		AoActAcmPop();
#endif //!_IPHONE
		
		
		AoActAcmPush();
		
		for (int i = 0; i < 3; i++) {
			AoActAcmInit();
			AoActAcmApplyTrans(dm_title_win_act_pos_tbl[i + 1][0]
							   , dm_title_win_act_pos_tbl[i + 1][1]
							   , 0
							   );
#if _IPHONE
			if (i <= ACT_WIN_TEX_MSG2 - ACT_WIN_TEX_MSG1) {
				AoActAcmApplyScale(DMD_TITLE_DISP_SCALE_TEXT
							   , DMD_TITLE_DISP_SCALE_TEXT
							   );
			}
#endif //_IPHONE
			
			// フレーム更新はSetFrameのみで行う
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			AoActUpdate(main_work->act[ACT_WIN_TEX_MSG1 + i], 0.0f);
		}
		
#if !_IPHONE
		for (int i = 0; i < 3; i++) {
#else //!_IPHONE
		for (int i = 1; i < 3; i++) {
			const int ACT_TEX_WINTITLE = ACT_TEX_YES - 1; //ループが1から始まるのでYES/NOが表示される様に相殺する
#endif //!_IPHONE
			AoActAcmInit();
			AoActAcmApplyTrans(dm_title_win_act_pos_tbl[i + 4][0]
							   , dm_title_win_act_pos_tbl[i + 4][1]
							   , 0
							   );
			
			// フレーム更新はSetFrameのみで行う
			AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
			AoActUpdate(main_work->act[ACT_TEX_WINTITLE + i], 0.0f);

#if _IPHONE
			AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[2]));
			const int c_btn_base_table[3][2] = { {0, -1}
												, {ACT_WIN_YES_BTN_L, ACT_WIN_YES_BTN_R}
												, {ACT_WIN_NO_BTN_L, ACT_WIN_NO_BTN_R}
												};
			AoActUpdate(main_work->act[c_btn_base_table[i][0] + 1], 0.0f); //当たり判定更新
			const int c_btn_trg[3] = {-1, 1, 0};
			er::CTrgAoAction &answer = main_work->trg_answer[c_btn_trg[i]];
			if (0 <= c_btn_trg[i]) {
				answer.Update();
			}
			float set_frame = ((answer.GetState(0)[er::CTrgState::EState::On])? 1.0f: 0.0f);
			for (int k = c_btn_base_table[i][0], max = c_btn_base_table[i][1] + 1; k < max; ++k) {
				AoActSetFrame(main_work->act[k], set_frame);
				AoActUpdate(main_work->act[k], 0.0f);
			}
#endif //_IPHONE
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
#endif //_PC || _XBOX || _PS3 || _IPHONE



// ==========================================================================
// dmTitleTaskDraw
/*!
	タイトル用描画タスク
 */
// ==========================================================================
void dmTitleTaskDraw(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	AoActDrawPre();
	amDrawExecCommand(DMD_TITLE_DRAW_STATE_ID);
	amDrawEndScene();
}



// ==========================================================================
// dmTitleIsDataLoad
/*!
	データ読み込み完了チェック処理
 */
// ==========================================================================
BOOL dmTitleIsDataLoad(DMS_TITLE_MAIN_WORK *main_work)
{
	for (int i = 0; i < DME_TITLE_DATA_TYPE_MAX; i++) {
		if (!amFsIsComplete(main_work->arc_amb_fs[i])) {
			return FALSE;
		}
	}

	// メニュー共通データ読み込み
	for (int i = 0; i < 4; i++) {
		if (!amFsIsComplete(main_work->arc_cmn_amb_fs[i])) {
			return 0;
		}
	}

#if _WII
	for (int i = 0; i < DME_TITLE_DATA_TYPE_MAX; i++) {
		if (!amFsIsComplete(main_work->file_arc_amb_fs[i])) {
			return FALSE;
		}
		
		if (!amFsIsComplete(main_work->user_arc_amb_fs[i])) {
			return FALSE;
		}
	}
	
#endif // #if _WII || _PC
	
#if _PC || _PS3 || _XBOX || _IPHONE
	if (!DmBuyScreenLoadIsFinished((const DMS_BUY_SCR_WORK *)&main_work->buy_scr_work)) {
		return FALSE;
	}
	
#endif // #if _PC || _PS3 || _XBOX || _IPHONE
	
	if (!DmTitleOpLoadCheck()) {
		return FALSE;
	}
	
	return TRUE;
}


// ==========================================================================
// dmTitleIsTexLoad
/*!
	テクスチャ構築完了チェック処理
 */
// ==========================================================================
BOOL dmTitleIsTexLoad(DMS_TITLE_MAIN_WORK *main_work)
{
	for (int i = 0; i < DME_TITLE_DATA_TYPE_MAX; i++) {
		if (!AoTexIsLoaded(&main_work->tex[i])) {
			// フラグ扱いでON
			return FALSE;
		}
	}
	
	// メニュー共通データ
	for (int i = 0; i < 4; i++) {
		if (!AoTexIsLoaded(&main_work->cmn_tex[i])) {
			// フラグ扱いでON
			return FALSE;
		}
	}
	
#if _WII
	if (!DmFileSlctBuildCheck()) {
		return FALSE;
	}
	if (!DmUserNameBuildCheck()) {
		return FALSE;
	}
#endif // #if _WII || _PC
	
#if _PC || _PS3 || _XBOX || _IPHONE
	if (!DmBuyScreenBuildIsFinished((const DMS_BUY_SCR_WORK *)&main_work->buy_scr_work)) {
		return FALSE;
	}
#endif // #if _PC || _PS3 || _XBOX || _IPHONE
	
	if (!GsFontIsBuilded()) {
		return FALSE;
	}
	
	if (!DmTitleOpBuildCheck()) {
		return FALSE;
	}

	return TRUE;
}


// ==========================================================================
// dmTitleIsTexRelease
/*!
	テクスチャ解放完了チェック処理
 */
// ==========================================================================
BOOL dmTitleIsTexRelease(DMS_TITLE_MAIN_WORK *main_work)
{
	for (int i = 0; i < DME_TITLE_DATA_TYPE_MAX; i++) {
		if (!AoTexIsReleased(&main_work->tex[i])) {
			// フラグ扱いでON
			return FALSE;
		}
	}
	
	// メニュー共通データ
	for (int i = 0; i < 4; i++) {
		if (!AoTexIsReleased(&main_work->cmn_tex[i])) {
			return FALSE;
		}
	}
	
	//
	if (!DmTitleOpFlushCheck()) {
		return FALSE;;
	}
	
#if _WII
	if (!DmFileSlctFlushCheck()) {
		return FALSE;
	}
	if (!DmUserNameFlushCheck()) {
		return FALSE;
	}
#endif
	
#if _PC || _PS3 || _XBOX || _IPHONE
	if (!DmBuyScreenFlushIsFinished((const DMS_BUY_SCR_WORK *)&main_work->buy_scr_work)) {
		return FALSE;
	}
	
#endif // #if _PC || _PS3 || _XBOX || _IPHONE

	return TRUE;
}




// ==========================================================================
// dmTitleSetChngFocusCrsrData
/*!
	表示設定処理
 */
// ==========================================================================
void dmTitleSetChngFocusCrsrData(DMS_TITLE_MAIN_WORK *main_work)
{
	if (main_work->flag & DMD_TITLE_FLAG_MENU_UP_INPUT) {
		main_work->prev_slct_menu = main_work->cur_slct_menu;
		main_work->cur_slct_menu = dmTitleGetRevisedMenuFocus(main_work->cur_slct_menu
															  , -1
															  , main_work->slct_menu_num
															  );
		
		// カーソル移動開始位置・移動先設定
		main_work->src_crsr_pos_y = main_work->cur_crsr_pos_y;
		
		
		if (GsTrialIsTrial()) {
			main_work->dst_crsr_pos_y = dm_title_crsr_trial_pos_y_tbl[main_work->cur_slct_menu];
		}
		else {
			main_work->dst_crsr_pos_y = dm_title_crsr_pos_y_tbl[0]
										+ main_work->cur_slct_menu * dm_title_crsr_pos_y_tbl[1];
		}
		
		main_work->flag |= DMD_TITLE_FLAG_MENU_CRSR_EFCT;
	}

	if (main_work->flag & DMD_TITLE_FLAG_MENU_DOWN_INPUT) {
		main_work->prev_slct_menu = main_work->cur_slct_menu;
		main_work->cur_slct_menu = dmTitleGetRevisedMenuFocus(main_work->cur_slct_menu
															  , 1
															  , main_work->slct_menu_num
															  );
		
		// カーソル移動開始位置・移動先設定
		main_work->src_crsr_pos_y = main_work->cur_crsr_pos_y;
		
		if (GsTrialIsTrial()) {
			main_work->dst_crsr_pos_y = dm_title_crsr_trial_pos_y_tbl[main_work->cur_slct_menu];
		}
		else {
			main_work->dst_crsr_pos_y = dm_title_crsr_pos_y_tbl[0]
										+ main_work->cur_slct_menu * dm_title_crsr_pos_y_tbl[1];
		}
		
		main_work->flag |= DMD_TITLE_FLAG_MENU_CRSR_EFCT;
	}
}




// ===========================================================================
//	dmTitleGetRevisedRankTabFocus
/*!
	項目の場所を示す変数が最大・最小値を超えた際に補正した(回り込みさせた)値を取得する関数
	通常時のカーソル移動の選択項目に使用。

	@param id		[in] 項目番号
	@param diff		[in] 変化量(移動における)
	@return 補正された項目番号
*/
// ===========================================================================
s32 dmTitleGetRevisedMenuFocus(s32 idx, s32 diff, s32 menu_num)
{
	s32 result;
	
	result = (int)idx + diff;
	
	// 先頭から一つ戻ると最後に移動
	if (result < 0) {
		result = (int)(menu_num - 1);
	}
	
	// 最後から一つ進むと先頭に移動
	if (result >= menu_num) {
		result = 0;
	}
	
	MTM_ASSERT(result >= 0 && result < menu_num);
	
	return result;
}



// ==========================================================================
// dmTitleSetCrsrFocusChangeEfct
/*!
	メインメニュー時のカーソル切り替え時演出中処理
 */
// ==========================================================================
void dmTitleSetCtrlFocusChangeEfct(DMS_TITLE_MAIN_WORK *main_work)
{
	float move_dist[2] = {0.f, 0.f};
	float distance[2] = {0.f, 0.f};

	// 縦移動する場合
	distance[1] = main_work->dst_crsr_pos_y - main_work->src_crsr_pos_y;
	
	move_dist[1] = distance[1] / DMD_TITLE_CRSR_MOVE_TIME;
	
	main_work->cur_crsr_pos_y += move_dist[1];
}



// ==========================================================================
// dmTitleSetWinOpenEfct
/*!
	ウインドウ入り演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmTitleSetWinOpenEfct(DMS_TITLE_MAIN_WORK *main_work)
{
	if (main_work->win_timer > DMD_TITLE_WIN_EFCT_TIME) {
		// ウインドウ演出終了
		main_work->flag |= DMD_TITLE_FLAG_WIN_EFCT_END;

		main_work->win_timer = 0.0f;

		for (u32 i = 0; i < 2; i++) {
			main_work->win_size_rate[i] = DMD_TITLE_WIN_DEF_RATE;
		}
	}
	else {
		// タイマー更新(但しフレームレートが可変になるように修正すること)	◆
		main_work->win_timer++;
	}
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->win_size_rate[i] = main_work->win_timer / DMD_TITLE_WIN_EFCT_TIME;
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
// dmTitleSetWinCloseEfct
/*!
	ウインドウ閉め演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmTitleSetWinCloseEfct(DMS_TITLE_MAIN_WORK *main_work)
{
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->win_size_rate[i] = main_work->win_timer / DMD_TITLE_WIN_EFCT_TIME;
		}
		else {
			main_work->win_size_rate[i] = 0.0f;
		}
	}

	if (main_work->win_timer < 0.0f) {
		// ウインドウ演出終了
		main_work->flag |= DMD_TITLE_FLAG_WIN_EFCT_END;

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
// dmTitleSetMMenuWinOpenEfct
/*!
	メインメニューウインドウ入り演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmTitleSetMMenuWinOpenEfct(DMS_TITLE_MAIN_WORK *main_work)
{
#if _IPHONE
	if (0.0f == main_work->mmenu_win_timer) {
		DmSoundPlaySE("Window");
	}

#endif //_IPHONE
	if (main_work->mmenu_win_timer > DMD_TITLE_WIN_EFCT_TIME) {
		// ウインドウ演出終了
		main_work->flag |= DMD_TITLE_FLAG_MMENU_WIN_EFCT_END;

		main_work->mmenu_win_timer = 0.0f;

		for (u32 i = 0; i < 2; i++) {
			main_work->mmenu_win_size_rate[i] = DMD_TITLE_WIN_DEF_RATE;
		}
	}
	else {
		// タイマー更新(但しフレームレートが可変になるように修正すること)	◆
		main_work->mmenu_win_timer++;
	}
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->mmenu_win_timer) {
			main_work->mmenu_win_size_rate[i] = main_work->mmenu_win_timer / DMD_TITLE_WIN_EFCT_TIME;
		}
		else {
			main_work->mmenu_win_size_rate[i] = 1.0f;
		}

		if (main_work->mmenu_win_size_rate[i] > 1.0f) {
			main_work->mmenu_win_size_rate[i] = 1.0f;
		}
	}
}



// ==========================================================================
// dmTitleSetMMenuWinCloseEfct
/*!
	メインメニューウインドウ閉め演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmTitleSetMMenuWinCloseEfct(DMS_TITLE_MAIN_WORK *main_work)
{
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->mmenu_win_timer) {
			main_work->mmenu_win_size_rate[i] = main_work->mmenu_win_timer / DMD_TITLE_WIN_EFCT_TIME;
		}
		else {
			main_work->mmenu_win_size_rate[i] = 0.0f;
		}
	}

	if (main_work->mmenu_win_timer < 0.0f) {
		// ウインドウ演出終了
		main_work->flag |= DMD_TITLE_FLAG_MMENU_WIN_EFCT_END;

		main_work->mmenu_win_timer = 0.0f;

		for (u32 i = 0; i < 2; i++) {
			main_work->mmenu_win_size_rate[i] = 0.0f;
		}
	}
	else {
		// タイマー更新(但しフレームレートが可変になるように修正すること)	◆
		main_work->mmenu_win_timer--;
	}
}



// ==========================================================================
// dmTitleIsCrsrFocusChangeEfctEnd
/*!
	メインメニュー時のカーソル切り替え時演出中処理
 */
// ==========================================================================
BOOL dmTitleIsCtrlFocusChangeEfctEnd(DMS_TITLE_MAIN_WORK *main_work)
{
	float move_direct = 0.f;
	
	move_direct = main_work->dst_crsr_pos_y
					- main_work->src_crsr_pos_y;
	
	// 掃け演出終了チェック
	if (main_work->cur_crsr_pos_y >= main_work->dst_crsr_pos_y
		 && move_direct >= 0) {
		main_work->cur_crsr_pos_y = main_work->dst_crsr_pos_y;
		
		main_work->timer = 0;
		
		return TRUE;
	}
	
	else if (main_work->cur_crsr_pos_y <= main_work->dst_crsr_pos_y
		 && move_direct <= 0) {
		main_work->cur_crsr_pos_y = main_work->dst_crsr_pos_y;
		
		main_work->timer = 0;
		
		return TRUE;
	}
	
	main_work->timer++;
	
	return FALSE;
}



// ==========================================================================
// dmTitleSetLoadSysData
/*!
	ロードデータ設定処理
 */
// ==========================================================================
void dmTitleSetLoadSysData(DMS_TITLE_MAIN_WORK *main_work)
{
	GSS_MAIN_SYS_INFO *gs_main = GsGetMainSysInfo();
	int vol_se = 0;
	int vol_bgm = 0;
	u32 eme_flag = 0;
	
	UNREFERENCED_PARAMETER(main_work);
	
	// バックアップデータ取得
	gs::backup::SOption &data = gs::backup::SOption::CreateInstance();
	
	vol_bgm = (int)data.GetVolumeBgm();
	vol_se = (int)data.GetVolumeSe();
	
	// 初期ボリューム設定
	gs_main->bgm_volume = (Float)(vol_bgm / 10);
	gs_main->se_volume = (Float)(vol_se / 10);
	
	// ボリューム設定(ゲーム側への反映)
	DmSoundSetVolumeBGM(gs_main->bgm_volume);
	DmSoundSetVolumeSE(gs_main->se_volume);
	
	// 振動設定ON
	gs_main->game_flag |= GSD_MAINSYS_GAME_FLAG_PAD_VIB_ENABLE;
	
	// スペシャルステージデータインスタンス作成
	gs::backup::SSpecial &spe_data
		= gs::backup::SSpecial::CreateInstance();
	
	// 各スペステでエメラルドを取得しているかどうかをチェック
	for (int i = 0; i < 7; i++) {
		if (spe_data[(u16)i].IsGetEmerald()) {
			eme_flag |= 1 << i;
		}
	}
	
	// カオスエメラルド取得状況
	if (eme_flag == 127) {
		gs_main->game_flag |= GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD;
	}
	else {
		gs_main->game_flag &= ~GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD;
	}
	
	/////gs_main->game_flag |= GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD; //sss!!!//test

	
	
	gs::backup::SSystem &sys_data = gs::backup::SSystem::CreateInstance();
	
	// 残機設定
	gs_main->rest_player_num = sys_data.GetPlayerStock();
	
#if _PS3 || _XBOX
	// 累計敵討伐数設定
	gs_main->ene_kill_count = sys_data.GetKilled();
	
	// クリア回数の設定
	gs_main->final_clear_count = sys_data.GetClearCount();
#endif
#if _IPHONE
	{
		using namespace gs::backup;
		switch (SOption::CreateInstance().GetControl()) {
		case SOption::EControl::VirtualPadDown:	//<バーチャルパッド(下)
			gs_main->game_flag &= ~GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK;
			gs_main->game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
			break;
		case SOption::EControl::VirtualPadUp:	//<バーチャルパッド(上)
			gs_main->game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK;
			gs_main->game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
			break;
		case SOption::EControl::Tilt:	//<傾斜
		default:
			gs_main->game_flag &= ~GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK;
			gs_main->game_flag &= ~GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
			break;
		}
	}
#endif //_IPHONE
	
	// その他初期化設定
	gs_main->is_spe_clear = FALSE;
	gs_main->is_first_play = FALSE;
}

	
void mppDT_dmTitleForceApplyLoadSysData() {
	dmTitleSetLoadSysData(NULL);
}


// ==========================================================================
// dmTitleSetInitSysData
/*!
	初期状態セーブデータ設定処理
 */
// ==========================================================================
void dmTitleSetInitSysData(DMS_TITLE_MAIN_WORK *main_work)
{
	GSS_MAIN_SYS_INFO *gs_main = GsGetMainSysInfo();
	int vol_se = 0;
	int vol_bgm = 0;
	bool is_vib = false;
	
	UNREFERENCED_PARAMETER(main_work);
	
	// バックアップデータ取得
	gs::backup::SOption &data = gs::backup::SOption::CreateInstance();
	
	vol_bgm = (int)data.GetVolumeBgm();
	vol_se = (int)data.GetVolumeSe();
	
	// 初期ボリューム設定
	gs_main->bgm_volume = (Float)(vol_bgm / 10.f);
	gs_main->se_volume = (Float)(vol_se / 10.f);
	
	// ボリューム設定(ゲーム側への反映)
	DmSoundSetVolumeBGM(gs_main->bgm_volume);
	DmSoundSetVolumeSE(gs_main->se_volume);
	
	// セーブデータから振動設定を取得
	is_vib = data.IsVibration();
	
	// 振動設定ON
	gs_main->game_flag |= GSD_MAINSYS_GAME_FLAG_PAD_VIB_ENABLE;
	
	gs_main->game_flag &= ~GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD;
	
	// 残機設定
	gs_main->rest_player_num = GSD_MAINSYS_PLAYER_REST_DEF;
	
#if _PS3 || _XBOX
	// 累計敵討伐数設定
	gs_main->ene_kill_count = 0;
	
	// クリア回数の設定
	gs_main->final_clear_count = 0;
#endif
	
	// その他初期化設定
	gs_main->is_spe_clear = FALSE;
	gs_main->is_first_play = FALSE;
}



// ==========================================================================
// dmTitleSetMenuInfo
/*!
	メニュー情報設定処理
 */
// ==========================================================================
void dmTitleSetMenuInfo(DMS_TITLE_MAIN_WORK *main_work)
{
	// メインメニューから開始の場合、メニュー数をここで設定
	if (GsTrialIsTrial()) {
		main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_TRIAL;
		
		// 初プレイフラグON
		main_work->is_init_play = TRUE;
	}
	else if (!GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_1_1)) {
		main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_FIRST;
		mppUtil::showCommunityButton(true);
		// 初プレイフラグON
		main_work->is_init_play = TRUE;
	}
	else {
		main_work->slct_menu_num = DMD_TITLE_SLCT_MENU_NUM_PLAYED;
		
		// 初プレイフラグOFF
		main_work->is_init_play = FALSE;
	}
}



// ==========================================================================
// dmTitleSetFirstPlayData
/*!
	セーブデータ初期化設定処理
 */
// ==========================================================================
void dmTitleSetFirstPlayData(DMS_TITLE_MAIN_WORK *main_work)
{
	GSS_MAIN_SYS_INFO *main_info = GsGetMainSysInfo();
	
	// システムデータインスタンス作成
	gs::backup::SSystem &sys_data
		= gs::backup::SSystem::CreateInstance();
	
	// 残機初期化設定
	main_info->rest_player_num = GSD_MAINSYS_PLAYER_REST_DEF;
	
	// 残機数設定
	sys_data.SetPlayerStock(main_info->rest_player_num);
	
	// ここで初期化したデータを設定
	dmTitleSetLoadSysData(main_work);
}



// ==========================================================================
// dmTitleSetInitSaveData
/*!
	セーブデータ初期化設定処理
 */
// ==========================================================================
void dmTitleSetInitSaveData(DMS_TITLE_MAIN_WORK *main_work)
{
	UNREFERENCED_PARAMETER(main_work);
	
	GSS_MAIN_SYS_INFO *main_info = GsGetMainSysInfo();
	
	// セーブデータ初期化処理
	GSS_BACKUP *backup = &main_info->backup;
	backup->Init();
}



#if _PC || _PS3 || _XBOX || _IPHONE
// ==========================================================================
// dmTitleIsSaveRunning
/*!
	セーブが有効かどうかを取得する処理
 */
// ==========================================================================
BOOL dmTitleIsSaveRunning(void)
{
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
// dmTitleIsChangeOptVol
/*!
	オプションの設定が初期状態かどうかを返す処理
 */
// ==========================================================================
BOOL dmTitleIsChangeOptVol(void)
{
	BOOL result = FALSE;
	
	// オプションデータインスタンス取得
	gs::backup::SOption &opt_data
		= gs::backup::SOption::CreateInstance();
	
	if (opt_data.GetVolumeBgm() != 100) {
		result = TRUE;
	}
	
	else if (opt_data.GetVolumeSe() != 100) {
		result = TRUE;
	}
	
	else {
		result = FALSE;
	}
	
	return result;
}


#endif //_PC || _PS3 || _XBOX || _IPHONE

// ==========================================================================
// DmTitleStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void DmTitleStaticVarInit(void)
{
	mpp_newGameButNotFirstGame = false;
	dm_title_is_title_start = FALSE;
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
