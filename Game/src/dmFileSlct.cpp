// ===========================================================================
/*!
	@file	dmFileSlct.cpp
	@brief	デモ・セーブファイル選択画面

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: dmFileSlct.cpp 20 2011-04-22 12:46:46Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 *
 *
 */

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"


#if _WII || _PC

#include "dmFileSlct.h"
#include "../library/ao/include/aoTexture.h"
#include "../library/ao/include/aoAction.h"

#include "gs.h"
#include "gsMainSys.h"
#include "izFade.h"
#include "aoWinSys.h"
#include "gsEnvironment.h"

#include "gsBackup.hpp"

#include "dmSound.h"
#include "dmCmnBackup.h"
#include "dmSave.h"

#include "dmUserName.h"

// データヘッダ
#include "common/ace/D_FILESLCT.HMA"
#include "common/ace/D_FILESLCT_JP.HMA"
#include "common/ace/D_OPTION_USER_JP.HMA"

// 共通データヘッダ
#include "common/ace/D_CMN_BTN.HMA"
#include "common/ace/D_CMN_OBI.HMA"
#include "common/ace/D_CMN_WIN.HMA"
#include "common/ace/D_CMN_MSG_JP.HMA"

// ----- Macros ------------------------------------------------（マクロ定義）

#define DMD_FILESLCT_TASK_PAUSELEVEL		(0)
#define DMD_FILESLCT_TASK_PRIO_MAIN			(0x2000)
#define DMD_FILESLCT_TASK_GROUP_MAIN		(0)

#define DMD_FILESLCT_FILE_PATH_NUM_MAX		(60)

#define DMD_FILESLCT_CMN_DATA_FILENAME		(GSS_BASE_PATH"DEMO/CMN/D_CMN_WIN.AMB")
#define DMD_FILESLCT_DATA_FILENAME			(GSS_BASE_PATH"DEMO/FILESLCT/D_FILESLCT.AMB")

#define DMD_FILESLCT_SIZE_WIDTH				(960.0f)
#define DMD_FILESLCT_SIZE_HEIGHT			(720.0f)
#define DMD_FILESLCT_SIZE_HALF_WIDTH		(480.0f)
#define DMD_FILESLCT_SIZE_HALF_HEIGHT		(360.0f)


// プライオリティ設定
#define DMD_FILESLCT_DRAW_PRIO_ZONE			(0x2000)
#define DMD_FILESLCT_DRAW_PRIO_STAGE		(0x2000)
#define DMD_FILESLCT_DRAW_PRIO_SAVEFILE		(0x2000)
#define DMD_FILESLCT_DRAW_PRIO_BG			(0x1000)
#define DMD_FILESLCT_DRAW_PRIO_FIX			(0x3000)
#define DMD_FILESLCT_DRAW_PRIO_WIN			(0x3000)
#define DMD_FILESLCT_DRAW_PRIO_WIN_FIX		(0x4000)

// 表示位置
//#define DMD_FILESLCT_ACT_TABLE_		(60)
#define DMD_FILESLCT_FILE_TABLE_TOP_POS_X	(128.0f)
#define DMD_FILESLCT_FILE_TABLE_TOP_POS_Y	(100.0f)
#define DMD_FILESLCT_FILE_TABLE_DIST_Y		(160.0f)//(160.0f)
#define DMD_FILESLCT_ALPHA_ONE_DIST			(20.f)

//#define DMD_FILESLCT_STAGE_TAB_DISP_POS_X	()
#define DMD_FILESLCT_STAGE_TAB_NODISP_POS_X	(1120.f)

#define DMD_FILESLCT_SAVE_FILE_NUM			(6)
#define DMD_FILESLCT_FILE_POS_NUM			(4)
#define DMD_FILESLCT_CRSR_POS_NUM			(3)

#define DMD_FILESLCT_DRAW_STATE_ID			(10)
// ウインドウ関連
#define DMD_FILESLCT_WINDOW_SIZE_W			(428.f)
#define DMD_FILESLCT_WINDOW_SIZE_H			(180.f)
#define DMD_FILESLCT_WIN_DEF_RATE			(1.0f)

// フェード関連
#define DMD_FILESLCT_FADEIN_TIME			(32.0f)
#define DMD_FILESLCT_FADEOUT_TIME			(32.0f)

// 演出関連
#define DMD_FILESLCT_FOCUS_FILE_CHNG_TIME	(12.0f)
#define DMD_FILESLCT_IN_EFCT_START_POS		(720.0f)
#define DMD_FILESLCT_IN_EFCT_TIME			(32.0f)
#define DMD_FILESLCT_OUT_EFCT_END_POS		(720.0f)
#define DMD_FILESLCT_OUT_EFCT_TIME			(32.0f)
#define DMD_FILESLCT_WIN_EFCT_TIME			(8.0f)
#define DMD_FILESLCT_OBI_MOVE_START_POS		(1120.f)//(1216.f)
#define DMD_FILESLCT_OBI_MOVE_END_POS		(-1120.f)
#define DMD_FILESLCT_OBI_MOVE_SPEED			(-3.f)
#define DMD_FILESLCT_OBI_EFCT_TIME			(16.f)
#define DMD_FILESLCT_OBI_NODISP_POS_Y		(192.f)
#define DMD_FILESLCT_OBI_DISP_POS_Y			(0.f)
#define DMD_FILESLCT_ACT_VRTCL_CHNG_DIST	(128.f)
#define DMD_FILESLCT_ACT_VRTCL_CHNG_NUM		(3)
#define DMD_FILESLCT_CRSR_MOVE_TIME			(8.0f)
#define DMD_FILESLCT_DECIDE_EFCT_TIME		(32.0f)
#define DMD_FILESLCT_DECIDE_EFCT_WAIT_TIME	(48.0f)
#define DMD_FILESLCT_DECIDE_EFCT_END_UP_POS		(-960.0f)
#define DMD_FILESLCT_DECIDE_EFCT_END_DOWN_POS	(720.0f)
#define DMD_FILESLCT_DEL_DATA_WIN_DISP_TIME	(60)

// フラグ関連
#define DMD_FILESLCT_FLAG_EXIT				(1 << 0)		//!< 終了フラグ
#define DMD_FILESLCT_FLAG_CANCEL			(1 << 1)		//!< キャンセル
#define DMD_FILESLCT_FLAG_DECIDE			(1 << 2)		//!< 決定フラグ
#define DMD_FILESLCT_FLAG_DISP_MENU			(1 << 3)
#define DMD_FILESLCT_FLAG_WIN_EFCT_END		(1 << 4)
#define DMD_FILESLCT_FLAG_CHNG_ZONE			(1 << 5)
#define DMD_FILESLCT_FLAG_CHNG_VRTCL		(1 << 6)
#define DMD_FILESLCT_FLAG_CHNG_CRSR			(1 << 7)
#define DMD_FILESLCT_FLAG_RE_CHNG_ZONE		(1 << 8)
#define DMD_FILESLCT_FLAG_RE_CHNG_VRTCL		(1 << 9)
#define DMD_FILESLCT_FLAG_RE_CHNG_CRSR		(1 << 10)
#define DMD_FILESLCT_FLAG_UP_CHNG_CRSR		(1 << 11)
#define DMD_FILESLCT_FLAG_DOWN_CHNG_CRSR	(1 << 12)
#define DMD_FILESLCT_FLAG_DEL_SAVE_FILE		(1 << 13)
#define DMD_FILESLCT_FLAG_IS_FADEIN			(1 << 14)
#define DMD_FILESLCT_FLAG_IS_FADEOUT		(1 << 15)
#define DMD_FILESLCT_FLAG_SET_USERNAME		(1 << 16)		//!< ユーザー名設定中フラグ
#define DMD_FILESLCT_FLAG_MOVE_CRSR			(1 << 17)
#define DMD_FILESLCT_FLAG_MOVE_VRTCL		(1 << 18)

// アクション表示フラグ関連
#define DMD_FILESLCT_DISP_FLAG_WIN_ACT	(1 << 0)
#define DMD_FILESLCT_DISP_FLAG_TB_ARROW	(1 << 1)
#define DMD_FILESLCT_DISP_FLAG_ACT_CRSR	(1 << 2)
#define DMD_FILESLCT_DISP_FLAG_OBI_TEX	(1 << 3)


#if _WII
#define DMD_FILESLCT_DISP_SCALE_TEXT		(1.4f)
#endif


// ----- Macro Functions -----------------------------------（処理マクロ定義）


// ----- Definitions -------------------------------------------（定数の宣言）
//! 次のイベント
typedef enum tag_DME_FILESLCT_NEXT_EVT
{
	DME_FILESLCT_NEXT_EVT_MAINGAME = 0,	//!< メインゲーム
	DME_FILESLCT_NEXT_EVT_OPTION,		//!< オプション
	DME_FILESLCT_NEXT_EVT_MAINMENU,		//!< メインメニュー
	DME_FILESLCT_NEXT_EVT_TITLE,			//!< タイトル

	DME_FILESLCT_NEXT_EVT_MAX
} DME_FILESLCT_NEXT_EVT;


typedef enum tag_DME_FILESLCT_DATA_TYPE
{
	DME_FILESLCT_DATA_TYPE_CMN_DATA = 0,	//!< 共通データ
	DME_FILESLCT_DATA_TYPE_LANG_DATA,		//!< 言語別データ
//	DME_FILESLCT_DATA_TYPE_WIN_CMN_DATA,	//!< ウインドウ用データ
//	DME_FILESLCT_DATA_TYPE_WIN_LANG_DATA,	//!< ウインドウ用言語別データ
	
	DME_FILESLCT_DATA_TYPE_MAX,
	DME_FILESLCT_DATA_TYPE_NONE
} DME_FILESLCT_DATA_TYPE;


//! ウインドウ表示パターンタイプ
typedef enum tag_DME_FILESLCT_WIN
{
	DME_FILESLCT_WIN_IS_DEL_FILE = 0,	//!< 
	DME_FILESLCT_WIN_DELETING_FILE,		//!< 
	
	DME_FILESLCT_WIN_NUM,
	DME_FILESLCT_WIN_NONE
} DME_FILESLCT_WIN;


//! セーブファイルNO
typedef enum tag_DME_FILESLCT_SAVE_FILE
{
	DME_FILESLCT_SAVE_FILE_1 = 0,		//!< 
	DME_FILESLCT_SAVE_FILE_2,			//!< 
	DME_FILESLCT_SAVE_FILE_3,			//!< 
	DME_FILESLCT_SAVE_FILE_4,			//!< 
	DME_FILESLCT_SAVE_FILE_5,		//!< 
	DME_FILESLCT_SAVE_FILE_6,		//!< 
	
	DME_FILESLCT_SAVE_FILE_NUM,
	DME_FILESLCT_SAVE_FILE_NONE
} DME_FILESLCT_SAVE_FILE;




//! アクションテーブル(ノード含む)
typedef enum tag_DME_FILESLCT_ACT
{
	// モード共通・言語共通
	ACT_BACK_BG = 0,	//!< 
	ACT_ARROW_UP,		//!< 
	ACT_ARROW_DOWN,		//!< 
	ACT_UP_HIDE_BG,		//!< 
	ACT_DOWN_HIDE_BG,	//!< 

	// モード共通・言語別
	ACT_TEX_DEL,		//!< 
	ACT_TEX_EXP1,		//!< 
	ACT_TEX_EXP2,		//!< 

	// セーブファイルテーブル・言語共通
	ACT_FILE_TAB1,
	ACT_FILE_SCR_BASE,
	ACT_FILE_TAB_NUM,
	ACT_FILE_TAB_EMER,
	ACT_FILE_SCR,
	ACT_FILE_TIME_H_1,
	ACT_FILE_TIME_H_2,
	ACT_FILE_TIME_COLON,
	ACT_FILE_TIME_M_1,
	ACT_FILE_TIME_M_2,
	ACT_FILE_ICON_EMER1,
	ACT_FILE_ICON_EMER2,
	ACT_FILE_ICON_EMER3,
	ACT_FILE_ICON_EMER4,
	ACT_FILE_ICON_EMER5,
	ACT_FILE_ICON_EMER6,
	ACT_FILE_ICON_EMER7,
	ACT_FILE_TEX_ALPHA1,
	ACT_FILE_TEX_ALPHA2,
	ACT_FILE_TEX_ALPHA3,
	ACT_FILE_TEX_ALPHA4,
	ACT_FILE_TEX_ALPHA5,
	ACT_FILE_TEX_ALPHA6,
	ACT_FILE_TEX_ALPHA7,
	ACT_FILE_TEX_ALPHA8,
	ACT_FILE_TEX_ALPHA9,
	ACT_FILE_TEX_ALPHA10,
	ACT_FILE_TAB3_B,
	ACT_FILE_TAB3_A,
	ACT_FILE_TAB3_C,
	ACT_FILE_TAB4_B,
	ACT_FILE_TAB4_A,
	ACT_FILE_TAB4_C,
	
	ACT_FILE_YEAR1,
	ACT_FILE_YEAR2,
	ACT_FILE_YEAR3,
	ACT_FILE_YEAR4,
	ACT_FILE_SLASH1,
	ACT_FILE_MON1,
	ACT_FILE_MON2,
	ACT_FILE_SLASH2,
	ACT_FILE_DAY1,
	ACT_FILE_DAY2,
	
	ACT_FILE_YEAR1_US,
	ACT_FILE_YEAR2_US,
	ACT_FILE_YEAR3_US,
	ACT_FILE_YEAR4_US,
	ACT_FILE_SLASH1_US,
	ACT_FILE_MON1_US,
	ACT_FILE_MON2_US,
	ACT_FILE_SLASH2_US,
	ACT_FILE_DAY1_US,
	ACT_FILE_DAY2_US,
	
	ACT_FILE_YEAR1_EU,
	ACT_FILE_YEAR2_EU,
	ACT_FILE_YEAR3_EU,
	ACT_FILE_YEAR4_EU,
	ACT_FILE_SLASH1_EU,
	ACT_FILE_MON1_EU,
	ACT_FILE_MON2_EU,
	ACT_FILE_SLASH2_EU,
	ACT_FILE_DAY1_EU,
	ACT_FILE_DAY2_EU,
	
	// セーブファイルテーブル・言語別
	ACT_FILE_TEX_NAME,
	ACT_FILE_TEX_NEWGAME,
	
#if _WII
	ACT_TEX_OBI_USER1,
	ACT_TEX_OBI_USER2,
#endif
	
	// ウインドウ関連・言語
	ACT_WIN_TEX_BACK,		//!< 
	ACT_WIN_TEX_MSG,		//!< 
	
	
	// メニュー共通データ
	ACT_FILESLCT_BACK_BTN,	//!< 
	ACT_BTN_CANCEL,			//!< 
	ACT_DEL_BTN,			//!< 
	
	ACT_OBI_C,				//!< 
	ACT_OBI_L,				//!< 
	ACT_OBI_R,				//!< 
	ACT_OBI_R2,				//!< 
	
	ACT_WIN_LINE,			//!< 
	
	ACT_TEX_WINTITLE,		//!< 
	ACT_TEX_YES,			//!< 
	ACT_TEX_NO,				//!< 
	ACT_TEX_BACK,			//!< 
	ACT_TEX_FIX_BACK,		//!< 
	
	
	ACT_NUM,

	ACT_TAB_ALL_SRC = ACT_FILE_TAB1,
	ACT_TAB_ALL_DST = ACT_FILE_TAB3_C,

	ACT_NONE
} DME_FILESLCT_ACT;


typedef struct tag_DMS_FILESLCT_MAIN_WORK	DMS_FILESLCT_MAIN_WORK;

//! メインタスクワーク
struct tag_DMS_FILESLCT_MAIN_WORK {

	AMS_FS			*arc_amb[DME_FILESLCT_DATA_TYPE_MAX];	//!< アーカイブAMBファイル
	AMS_FS			*win_amb_fs;						//!< ウインドウAMBファイル
	void			*ama[DME_FILESLCT_DATA_TYPE_MAX];	//!< AMAファイル
	void			*amb[DME_FILESLCT_DATA_TYPE_MAX];	//!< AMBファイル
	void			*win_amb;							//!< ウインドウ用AMBファイル
	
	AOS_TEXTURE		tex[DME_FILESLCT_DATA_TYPE_MAX];		//!< テクスチャ
	AOS_TEXTURE		win_tex;							//!< ウインドウテクスチャ


	void			*arc_cmn_amb[4];					//!< 共通アーカイブAMBファイル
	void			*cmn_ama[4];						//!< AMAファイル
	void			*cmn_amb[4];						//!< AMBファイル
	AOS_TEXTURE		cmn_tex[4];							//!< メニュー共通テクスチャ
	
	// ウインドウ用アクション

	// メニュー用アクション
	AOS_ACTION 		*act[ACT_NUM];
	
	AOS_ACTION 		*file_act[DMD_FILESLCT_SAVE_FILE_NUM];

	void (*proc_win_input)(DMS_FILESLCT_MAIN_WORK *);	//!< 入力処理関数
	void (*proc_input)(DMS_FILESLCT_MAIN_WORK *);		//!< 入力処理関数
	void (*proc_win_update)(DMS_FILESLCT_MAIN_WORK *);	//!< ウインドウ用プロシージャ
	void (*proc_menu_update)(DMS_FILESLCT_MAIN_WORK *);	//!< メニュー用プロシージャ
	void (*proc_draw)(DMS_FILESLCT_MAIN_WORK *);			//!< 描画用プロシージャ

	float timer;											//!< 汎用タイマー
	u32	flag;											//!< 汎用フラグ
	s32 state;											//!< ZONE選択中かSTAGE選択中か
	float win_timer;									//!< ウインドウ演出用タイマー
	u32 disp_flag;										//!< 表示切替用フラグ
	s32 del_timer;

	u32 announce_flag;									//!< 0ならアナウンスなし、それ以外はフラグがあるだけ表示する

	s32 next_evt;										//!< 次のイベント
	s32 prev_evt;										//!< 前のイベント

	u32 last_clear_act[DME_FILESLCT_SAVE_FILE_NUM];		//!< 最後にプレイしたステージ番号
	s32 play_time[DME_FILESLCT_SAVE_FILE_NUM];			//!< 各ステージのレコードタイム
	u32 has_emerald[DME_FILESLCT_SAVE_FILE_NUM];		//!< 各ステージで取得したエメラルドの情報
	s32 is_clear_file[DME_FILESLCT_SAVE_FILE_NUM];		//!< 各ステージでクリアしたかの情報
	const char *user_name[DME_FILESLCT_SAVE_FILE_NUM];	//!<
	
#if _WII
	OSTime save_time[DME_FILESLCT_SAVE_FILE_NUM];		//!<
#endif

	u32 get_emerald;									//!< プレイヤーが取得しているエメラルドの情報
	u32 cur_game_mode;									//!< FOCUS中のモードがノーマルモードかタイムアタックか
	
	u32 is_save_exist;
	
	// WINDOW専用
	float win_act_pos[14-1][2];							//!< 
	float win_size_rate[2];								//!< 
	s32 win_mode;										//!< 
	s32 win_cur_slct;									//!< ウインドウでの現在の選択項目
	
	u32 efct_time;										//!< 演出時間
	u32 efct_out_flag;									//!< 

	// ACT専用
	float file_tab_pos_x[DMD_FILESLCT_SAVE_FILE_NUM];
	float file_tab_pos_y[DMD_FILESLCT_SAVE_FILE_NUM];
	float file_move_src[2];
	float file_move_dest[2];
	float file_move_pos_src[DMD_FILESLCT_SAVE_FILE_NUM];
	float file_move_pos_dst[DMD_FILESLCT_SAVE_FILE_NUM];
	
//	float file_back_pos_x[DMD_FILESLCT_SAVE_FILE_NUM];
	float file_back_pos[DMD_FILESLCT_SAVE_FILE_NUM];

	// STAGE専用
	s32 cur_file;										//!< 現在選択中のSTAGE
	s32 prev_file;										//!< 
	s32 cur_vrtcl_file;									//!< 
	s32 prev_vrtcl_file;								//!< 
	s32 crsr_idx;
	s32 crsr_prev_idx;
	float crsr_pos_y;
	float crsr_move_src;
	float crsr_move_dst;
	s32 focus_disp_no;
	s32 prev_disp_no;

	// 帯用
	float obi_pos_y;									//!< 帯の移動用座標変数
	float obi_tex_pos[2];								//!< 帯の移動用座標変数
	float obi_back_pos_y;								

	AMS_PARAM_DRAW_PRIMITIVE up_bg_vrtx;
	
	float back_bg_frame;

	u32 draw_state;
	BOOL is_jp_region;
};


//! 管理構造体
typedef struct tag_DMS_FILESLCT_MGR {
	MTS_TASK_TCB *tcb;	//!< TCB
} DMS_FILESLCT_MGR;



// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static void dmFileSlctInit(void);
static void dmFileSlctProcMain(MTS_TASK_TCB *tcb);
static void dmFileSlctDest(MTS_TASK_TCB *tcb);

// 初期化設定関連
static void dmFileSlctSetSaveData(DMS_FILESLCT_MAIN_WORK *main_work);

static void dmFileSlctProcTitleStart(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctProcCreateAct(DMS_FILESLCT_MAIN_WORK *main_work);

// メニュー用プロシージャ
static void dmFileSlctProcStopDraw(DMS_FILESLCT_MAIN_WORK *main_work);

static void dmFileSlctProcFileSelectIdle(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctProcFileTabInEfct(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctProcFileTabDecideEfct(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctProcChngFocusFile(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctProcFileTabOutEfct(DMS_FILESLCT_MAIN_WORK *main_work);

static void dmFileSlctProcSetUserName(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctProcUserNameSaveWait(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctProcCancelUserName(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctProcFadeOut(DMS_FILESLCT_MAIN_WORK *main_work);

// ウインドウ用プロシージャ
static void dmFileSlctProcWindowNodispIdle(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctProcWindowOpenEfct(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctProcWindowAnnounceIdle(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctProcWindowCloseEfct(DMS_FILESLCT_MAIN_WORK *main_work);

// 入力処理用プロシージャ
static void dmFileSlctInputProcFileSelect(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctInputProcWinNodispIdle(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctInputProcWinDispIdle(DMS_FILESLCT_MAIN_WORK *main_work);

//static void dmFileSlctInputProcFileSelectMove(DMS_FILESLCT_MAIN_WORK *main_work);

// 描画関連処理
static void dmFileSlctProcActDraw(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctCommonDraw(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctCommonFixDraw(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctWinSelectDraw(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctSaveFileDraw(DMS_FILESLCT_MAIN_WORK *main_work);

// 演出関連設定処理
static void dmFileSlctSetFileTabInEfct(DMS_FILESLCT_MAIN_WORK *main_work);
static BOOL dmFileSlctIsFileTabInEfctEnd(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctSetObiInEfct(DMS_FILESLCT_MAIN_WORK *main_work);
static BOOL dmFileSlctIsObiInEfctEnd(DMS_FILESLCT_MAIN_WORK *main_work);

static void dmFileSlctSetFileTabOutEfct(DMS_FILESLCT_MAIN_WORK *main_work);
static BOOL dmFileSlctIsFileTabOutEfctEnd(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctSetFileTabDecideEfct(DMS_FILESLCT_MAIN_WORK *main_work);
static BOOL dmFileSlctIsFileTabDecideEfctEnd(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctSetFileCurTabDecideEfct(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctSetObiOutEfct(DMS_FILESLCT_MAIN_WORK *main_work);
static BOOL dmFileSlctIsObiOutEfctEnd(DMS_FILESLCT_MAIN_WORK *main_work);

static void dmFileSlctSetFileTabBackEfct(DMS_FILESLCT_MAIN_WORK *main_work);
static BOOL dmFileSlctIsFileTabBackEfctEnd(DMS_FILESLCT_MAIN_WORK *main_work);

static void dmFileSlctSetFileVrtclChangeEfct(DMS_FILESLCT_MAIN_WORK *main_work);
static BOOL dmFileSlctIsFileVrtclChangeEfct(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctSetStageCrsrChangeEfct(DMS_FILESLCT_MAIN_WORK *main_work);
static BOOL dmFileSlctIsStageCrsrChangeEfct(DMS_FILESLCT_MAIN_WORK *main_work);

static void dmFileSlctSetWinOpenEfct(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctSetWinCloseEfct(DMS_FILESLCT_MAIN_WORK *main_work);

static void dmFileSlctSetObiEfctPos(DMS_FILESLCT_MAIN_WORK *main_work);

static s32 dmFileSlctIsTexLoad(void);//DMS_FILESLCT_MAIN_WORK *main_work);
static s32 dmFileSlctIsTexRelease(void);//DMS_FILESLCT_MAIN_WORK *main_work);

static s32 dmFileSlctGetRevisedFileVrtclNo(s32 idx, s32 diff);
static s32 dmFileSlctGetRevisedFileCrsrNo(s32 idx, s32 diff, s32 disp_act_no);

static void dmFileSlctDeleteSaveFile(DMS_FILESLCT_MAIN_WORK *main_work);
static BOOL dmFileSlctIsDeleteSaveFile(DMS_FILESLCT_MAIN_WORK *main_work);

static void dmFileSlctSetFocusChangeEfctData(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctSetOutEfctData(DMS_FILESLCT_MAIN_WORK *main_work);
static void dmFileSlctSetDecideEfctData(DMS_FILESLCT_MAIN_WORK *main_work);

static BOOL dmFileSlctSetMsgWinState(DMS_FILESLCT_MAIN_WORK *main_work);

#if _WII
static void dmFileSlctSetNumDigitFrame(u32 digit[], const int data, int digit_num);
#endif

// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ZONEごとのACTテーブル表示位置Yテーブル
const static float dm_fileslct_file_disp_y_pos_tbl[4] = {
	0.0f,
	160.f * -1.f,
	160.f * -2.f,
	160.f * -3.f,
};

// ZONEごとのカーソル表示位置Yテーブル
const static float dm_fileslct_act_crsr_disp_y_pos_tbl[3] = {
	100.0f,
	100.0f + 160.f * 1.f,
	100.0f + 160.f * 2.f,
};

// ウインドウ選択肢用表示フレームテーブル(現状２つ用)
const static float dm_fileslct_win_disp_slct_frm_tbl[2][2] = {
	{0.f, 1.f},		// 左(上)がアクティブ
	{1.f, 0.f},		// 右(下)がアクティブ
};



// ACT縦並びの表示位置テーブル
const static float dm_fileslct_vrtcl_disp_pos_y_tbl[7 - DMD_FILESLCT_ACT_VRTCL_CHNG_NUM] = {
	DMD_FILESLCT_ACT_VRTCL_CHNG_DIST * 0.f,
	DMD_FILESLCT_ACT_VRTCL_CHNG_DIST * 1.f,
	DMD_FILESLCT_ACT_VRTCL_CHNG_DIST * 2.f,
	DMD_FILESLCT_ACT_VRTCL_CHNG_DIST * 3.f,
};

#if !_WII
const static float dm_fileslct_win_act_pos_tbl[7][2] = {
	{DMD_FILESLCT_SIZE_HALF_WIDTH + 18.f, 280.0f},	// ウインドウ内のライン
	{DMD_FILESLCT_SIZE_HALF_WIDTH + 182.f, 262.0f},	// キャンセルボタン
	{DMD_FILESLCT_SIZE_HALF_WIDTH - 104.f, 274.0f},	// タイトルテキスト
	{DMD_FILESLCT_SIZE_HALF_WIDTH - 88.f, 420.0f},	// YES
	{DMD_FILESLCT_SIZE_HALF_WIDTH + 88.f, 420.0f},	// NO
	{DMD_FILESLCT_SIZE_HALF_WIDTH + 202.f, 262.0f},	// 戻る
	{DMD_FILESLCT_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
};
#else // #if _WII
const static float dm_fileslct_win_act_pos_tbl[7][2] = {
	{DMD_FILESLCT_SIZE_HALF_WIDTH + 18.f, 280.0f},	// ウインドウ内のライン
	{DMD_FILESLCT_SIZE_HALF_WIDTH + 282.f, 232.0f},	// キャンセルボタン
	{DMD_FILESLCT_SIZE_HALF_WIDTH - 104.f, 274.0f},	// タイトルテキスト
	{DMD_FILESLCT_SIZE_HALF_WIDTH - 88.f, 420.0f},	// YES
	{DMD_FILESLCT_SIZE_HALF_WIDTH + 88.f, 420.0f},	// NO
	{DMD_FILESLCT_SIZE_HALF_WIDTH + 302.f, 232.0f},	// 戻る
	{DMD_FILESLCT_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
};
#endif


const static float dm_fileslct_back_text_length_tbl[6] = {
	-39.f,
	-53.f,
	-69.f,
	-77.f,
	-70.f,
	-55.f,
};


const static s32 dm_fileslct_time_act_id_tbl[GSD_REGION_NUM][3] = {
	{ACT_FILE_YEAR1, ACT_FILE_MON1, ACT_FILE_DAY1},
	{ACT_FILE_YEAR1_US, ACT_FILE_MON1_US, ACT_FILE_DAY1_US},
	{ACT_FILE_YEAR1_EU, ACT_FILE_MON1_EU, ACT_FILE_DAY1_EU},
};


// アクションIDテーブル(初期状態)
const static u32 g_dm_act_id_tbl[ACT_NUM] = {
	IDA_D_FILESLCT_ACT_BG_BLACK,
	IDA_D_FILESLCT_ACT_ARROW_UP,
	IDA_D_FILESLCT_ACT_ARROW_DOWN,
	IDA_D_FILESLCT_ACT_GRADATION_TOP,
	IDA_D_FILESLCT_ACT_GRADATION_BOTTOM,

	// モード共通・言語別
	IDA_D_FILESLCT_JP_ACT_TEX_DEL,
	IDA_D_FILESLCT_JP_ACT_TEX_EXP,
	IDA_D_FILESLCT_JP_ACT_TEX_EXP,

	// テーブル部
	IDA_D_FILESLCT_ACT_1_TAB01,
	IDA_D_FILESLCT_ACT_SCREEN_BASE,
	IDA_D_FILESLCT_ACT_1_NUM02,
	IDA_D_FILESLCT_ACT_1_TAB05,
	IDA_D_FILESLCT_ACT_1_SCREEN,
	IDA_D_FILESLCT_ACT_1_TIME_1,
	IDA_D_FILESLCT_ACT_1_TIME_2,
	IDA_D_FILESLCT_ACT_1_TIME_COLON,
	IDA_D_FILESLCT_ACT_1_TIME_3,
	IDA_D_FILESLCT_ACT_1_TIME_4,
	IDA_D_FILESLCT_ACT_1_ICON_EMER_1,
	IDA_D_FILESLCT_ACT_1_ICON_EMER_2,
	IDA_D_FILESLCT_ACT_1_ICON_EMER_3,
	IDA_D_FILESLCT_ACT_1_ICON_EMER_4,
	IDA_D_FILESLCT_ACT_1_ICON_EMER_5,
	IDA_D_FILESLCT_ACT_1_ICON_EMER_6,
	IDA_D_FILESLCT_ACT_1_ICON_EMER_7,
	IDA_D_FILESLCT_ACT_1_TEX_ALPHA_1,
	IDA_D_FILESLCT_ACT_1_TEX_ALPHA_1,
	IDA_D_FILESLCT_ACT_1_TEX_ALPHA_1,
	IDA_D_FILESLCT_ACT_1_TEX_ALPHA_1,
	IDA_D_FILESLCT_ACT_1_TEX_ALPHA_1,
	IDA_D_FILESLCT_ACT_1_TEX_ALPHA_1,
	IDA_D_FILESLCT_ACT_1_TEX_ALPHA_1,
	IDA_D_FILESLCT_ACT_1_TEX_ALPHA_1,
	IDA_D_FILESLCT_ACT_1_TEX_ALPHA_1,
	IDA_D_FILESLCT_ACT_1_TEX_ALPHA_1,
	IDA_D_FILESLCT_ACT_1_TAB03_B,
	IDA_D_FILESLCT_ACT_1_TAB03_A,
	IDA_D_FILESLCT_ACT_1_TAB03_C,
	IDA_D_FILESLCT_ACT_1_TAB04_B,
	IDA_D_FILESLCT_ACT_1_TAB04_A,
	IDA_D_FILESLCT_ACT_1_TAB04_C,
	
	IDA_D_FILESLCT_ACT_1_YEAR_1,
	IDA_D_FILESLCT_ACT_1_YEAR_2,
	IDA_D_FILESLCT_ACT_1_YEAR_3,
	IDA_D_FILESLCT_ACT_1_YEAR_4,
	IDA_D_FILESLCT_ACT_1_SLASH_1,
	IDA_D_FILESLCT_ACT_1_MON_1,
	IDA_D_FILESLCT_ACT_1_MON_2,
	IDA_D_FILESLCT_ACT_1_SLASH_2,
	IDA_D_FILESLCT_ACT_1_DAY_1,
	IDA_D_FILESLCT_ACT_1_DAY_2,
	
	IDA_D_FILESLCT_ACT_1_YEAR_1_US,
	IDA_D_FILESLCT_ACT_1_YEAR_2_US,
	IDA_D_FILESLCT_ACT_1_YEAR_3_US,
	IDA_D_FILESLCT_ACT_1_YEAR_4_US,
	IDA_D_FILESLCT_ACT_1_SLASH_1_US,
	IDA_D_FILESLCT_ACT_1_MON_1_US,
	IDA_D_FILESLCT_ACT_1_MON_2_US,
	IDA_D_FILESLCT_ACT_1_SLASH_2_US,
	IDA_D_FILESLCT_ACT_1_DAY_1_US,
	IDA_D_FILESLCT_ACT_1_DAY_2_US,
	
	IDA_D_FILESLCT_ACT_1_YEAR_1_EU,
	IDA_D_FILESLCT_ACT_1_YEAR_2_EU,
	IDA_D_FILESLCT_ACT_1_YEAR_3_EU,
	IDA_D_FILESLCT_ACT_1_YEAR_4_EU,
	IDA_D_FILESLCT_ACT_1_SLASH_1_EU,
	IDA_D_FILESLCT_ACT_1_MON_1_EU,
	IDA_D_FILESLCT_ACT_1_MON_2_EU,
	IDA_D_FILESLCT_ACT_1_SLASH_2_EU,
	IDA_D_FILESLCT_ACT_1_DAY_1_EU,
	IDA_D_FILESLCT_ACT_1_DAY_2_EU,
	
	// セーブファイルテーブル・言語別
	IDA_D_FILESLCT_JP_ACT_1_TEX_NAME,
	IDA_D_FILESLCT_JP_ACT_1_TEX_NEWGAME,
	
#if _WII
	IDA_D_OPTION_USER_JP_ACT_TEX_OBI1,
	IDA_D_OPTION_USER_JP_ACT_TEX_OBI1,
#endif
	
	// ウインドウ関連・言語
	IDA_D_FILESLCT_JP_ACT_TEX_BACK,
	IDA_D_FILESLCT_JP_ACT_WIN_MSG,

	
	// メニュー共通データ
	IDA_D_CMN_BTN_ACT_BTN_BACK_FILESLCT,	//!<
	IDA_D_CMN_BTN_ACT_BACK_BTN,				//!<
	IDA_D_CMN_BTN_ACT_BUT_DEL,				//!<
	
	IDA_D_CMN_OBI_ACT_OBI_CENTER,			//!< 
	IDA_D_CMN_OBI_ACT_OBI_LEFT,				//!< 
	IDA_D_CMN_OBI_ACT_OBI_RIGHT2_R,			//!< 
	IDA_D_CMN_OBI_ACT_OBI_RIGHT2_L,			//!< 
	
	IDA_D_CMN_WIN_ACT_WIN_LINE,				//!< 
	
	IDA_D_CMN_MSG_JP_ACT_TEX_WINTITLE,		//!< 
	IDA_D_CMN_MSG_JP_ACT_TEX_YES,			//!< 
	IDA_D_CMN_MSG_JP_ACT_TEX_NO,			//!< 
	IDA_D_CMN_MSG_JP_ACT_TEX_BACK,			//!< 
	IDA_D_CMN_MSG_JP_ACT_TEX_BACK_FILESLCT,	//!< 
	
};


//管理情報
static DMS_FILESLCT_MGR dm_fileslct_mgr;
static DMS_FILESLCT_MGR *dm_fileslct_mgr_p = NULL;

static void *dm_fileslct_arc_amb[DME_FILESLCT_DATA_TYPE_MAX];
static void *dm_fileslct_ama[DME_FILESLCT_DATA_TYPE_MAX];
static void *dm_fileslct_amb[DME_FILESLCT_DATA_TYPE_MAX];
static void *dm_fileslct_cmn_arc_amb[4];
static void *dm_fileslct_cmn_ama[4];
static void *dm_fileslct_cmn_amb[4];
static AOS_TEXTURE dm_fileslct_tex[DME_FILESLCT_DATA_TYPE_MAX];
static AOS_TEXTURE dm_fileslct_cmn_tex[4];
static s32 dm_fileslct_is_file_decide = 0;

static u32 dm_fileslct_draw_state = 0;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ==========================================================================
// DmFileSlctBuild
/*!
 *	ファイル選択データ構築
  	(ファイルの読込みは呼び出し側で行い、引数でポインタを渡してこちらでデータ構築)
 */
// ==========================================================================
void DmFileSlctBuild(void *arc_amb[], DMS_FILESLCT_DATA_MGR *data_mgr)
{
	int i = 0;

	// 管理情報初期化
	amZeroMemory(&dm_fileslct_mgr, sizeof(DMS_FILESLCT_MGR));
	dm_fileslct_mgr_p = &dm_fileslct_mgr;
	
	for (i = 0; i < DME_FILESLCT_DATA_TYPE_MAX; i++) {
		amZeroMemory(&dm_fileslct_tex[i], sizeof(AOS_TEXTURE));
	}
	
	for (i = 0; i < 4; i++) {
		amZeroMemory(&dm_fileslct_cmn_tex[i], sizeof(AOS_TEXTURE));
	}

	for (i = 0; i < DME_FILESLCT_DATA_TYPE_MAX; i++) {
		dm_fileslct_arc_amb[i] = arc_amb[i];
		
		amBindConv((u8 *)dm_fileslct_arc_amb[i]);
		
		// AMBファイルロード
		dm_fileslct_ama[i] = amBindGet((AMS_AMB_HEADER *)dm_fileslct_arc_amb[i]
									  , 0
									  );
	
		dm_fileslct_amb[i] = amBindGet((AMS_AMB_HEADER*)dm_fileslct_arc_amb[i]
									  , 1
									  );
	}
	
	// アドレス変換
	for (i = 0; i < DME_FILESLCT_DATA_TYPE_MAX; i++) {
		amConvertAddress(dm_fileslct_ama[i]);
		amConvertAddress(dm_fileslct_amb[i]);
	}
	
	// テクスチャ構築
	for (i = 0; i < DME_FILESLCT_DATA_TYPE_MAX; i++) {
		// テクスチャ構築開始
		AoTexBuild(&dm_fileslct_tex[i], dm_fileslct_amb[i]);
		AoTexLoad(&dm_fileslct_tex[i]);
	}
	
	// メニュー共通データ
	for (int i = 0; i < 4; i++) {
		dm_fileslct_cmn_arc_amb[i] = data_mgr->arc_cmn_amb[i];
		
		amBindConv((u8 *)dm_fileslct_cmn_arc_amb[i]);
		
		// AMBファイルロード
		dm_fileslct_cmn_ama[i] = amBindGet((AMS_AMB_HEADER*)dm_fileslct_cmn_arc_amb[i]
										  , 0
										  );
		
		dm_fileslct_cmn_amb[i] = amBindGet((AMS_AMB_HEADER*)dm_fileslct_cmn_arc_amb[i]
										  , 1
										  );
		
		// アドレス変換
		amConvertAddress(dm_fileslct_cmn_ama[i]);
		amConvertAddress(dm_fileslct_cmn_amb[i]);

		// テクスチャ構築開始
		AoTexBuild(&dm_fileslct_cmn_tex[i], dm_fileslct_cmn_amb[i]);
		AoTexLoad(&dm_fileslct_cmn_tex[i]);
	}
	
}


// ==========================================================================
// DmFileSlctBuildCheck
/*!
 *	ファイル選択データ構築 終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL DmFileSlctBuildCheck(void)
{
	// テクスチャ構築チェック
	if (dmFileSlctIsTexLoad()) {
		// フラグ扱いでON
		return (TRUE);
	}

	return (FALSE);
}


// ==========================================================================
// DmFileSlctFlush
/*!
 *	ファイル選択データフラッシュ
 *
 *	@reutrn	リングシステムワークアドレス
 */
// ==========================================================================
void DmFileSlctFlush(void)
{
	// テクスチャ解放
	for (int i = 0; i < DME_FILESLCT_DATA_TYPE_MAX; i++) {
		AoTexRelease(&dm_fileslct_tex[i]);
	}
	
	for (int i = 0; i < 4; i++) {
		AoTexRelease(&dm_fileslct_cmn_tex[i]);
	}
	
}



// ==========================================================================
// DmFileSlctFlushCheck
/*!
 *	ファイル選択データフラッシュ終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL DmFileSlctFlushCheck(void)
{
	// テクスチャ解放
	if (!dmFileSlctIsTexRelease()) {
		
		return (FALSE);
	}
	
	return (TRUE);
}



// ==========================================================================
// DmFileSlctIsExit
/*!
	セーブファイル選択画面の終了確認処理
 */
// ==========================================================================
s32 DmFileSlctIsExit(void)
{
	if (!dm_fileslct_mgr_p->tcb) {
		return dm_fileslct_is_file_decide;
	}

	return 0;
}


// ==========================================================================
// DmFileSlctExit
/*!
	セーブファイル選択画面の終了処理
 */
// ==========================================================================
void DmFileSlctExit(void)
{
	mtTaskClearTcb(dm_fileslct_mgr_p->tcb);
	dm_fileslct_mgr_p->tcb = NULL;
}



// ==========================================================================
// DmFileSlctStart
/*!
	セーブファイル選択画面開始処理
 */
// ==========================================================================
void DmFileSlctStart(void *arg)
{
	UNREFERENCED_PARAMETER(arg);
	
	dmFileSlctInit();
}



// ----- Static Functions --------------------（スタティック関数の定義：局所）
// ==========================================================================
// dmFileSlctInit
/*!
	セーブファイル選択画面初期化処理
 */
// ==========================================================================
void dmFileSlctInit(void)
{
	DMS_FILESLCT_MAIN_WORK	*main_work;

	AoActSysSetDrawStateEnable(TRUE);		// どのデモを開始する際も必ず設定
	AoActSysSetDrawState(DMD_FILESLCT_DRAW_STATE_ID);
	
	// メインタスク作成
	dm_fileslct_mgr_p->tcb = MTM_TASK_MAKE_TCB(dmFileSlctProcMain
											   , dmFileSlctDest
											   , 0
											   , DMD_FILESLCT_TASK_PAUSELEVEL
											   , DMD_FILESLCT_TASK_PRIO_MAIN
											   , DMD_FILESLCT_TASK_GROUP_MAIN
											   , sizeof(DMS_FILESLCT_MAIN_WORK)
											   , "FILESLCT_MAIN"
											   );
	
	// ワーク初期化
	main_work = (DMS_FILESLCT_MAIN_WORK *)mtTaskGetTcbWork(dm_fileslct_mgr_p->tcb);
	
	main_work->draw_state = (u32)AoActSysGetDrawStateEnable();

	AoActSysSetDrawStateEnable((int)main_work->draw_state);
	
	if (main_work->draw_state) {
		dm_fileslct_draw_state = AoActSysGetDrawState();
	}
	
	// 初期化処理があればここに記述
	// リージョンデータ取得(ボタン表示切り替え用)
	if (GeEnvGetDecideKey() == GSD_DECIDE_KEY_O) {
		main_work->is_jp_region = TRUE;
	}
	else {
		main_work->is_jp_region = FALSE;
	}

	// 初期化処理があればここに記述
	dmFileSlctSetSaveData(main_work);
	dm_fileslct_is_file_decide = 0;

	// プロシージャ設定
	main_work->proc_menu_update = dmFileSlctProcTitleStart;
}



// ==========================================================================
// dmFileSlctSetSaveData
/*!
	ハイスコア設定処理
 */
// ==========================================================================
void dmFileSlctSetSaveData(DMS_FILESLCT_MAIN_WORK *main_work)
{
	u32 eme_cnt = 0;
//	u32 dummy_save_flag = 1 + 2 + 4 + 16;
	u32 last_play_file = 0;
	
//	main_work->is_save_exist = dummy_save_flag;
	
	gs::backup::SBackup &backup = gs::backup::SBackup::CreateInstance();
	
	// ファイル数分のデータロード
	for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
		
		gs::backup::SSpecial &spe_data = backup.GetSpecial(i);
		
		
//		gs::backup::SOption &opt_data = gs::backup::SOption::CreateInstance(i);
//		gs::backup::SSpecial &spe_data = gs::backup::SSpecial::CreateInstance(i);
//		gs::backup::SSystem &sys_data = gs::backup::SSystem::CreateInstance(i);
		
#if _WII
		gs::backup::SSystem &sys_data = backup.GetSystem(i);
		gs::backup::SOption &opt_data = backup.GetOption(i);
		
		main_work->save_time[i] = (OSTime)sys_data.GetLastSaveChrono();

		// プレイ時間が初期状態でなければ
		if (main_work->save_time[i] != 0) {
			main_work->is_save_exist |= 1 << i;
		}
#endif
		
		for (u32 j = 0; j < 7; j++) {
			if (spe_data[j].IsGetEmerald() == FALSE) {		// これはステージ毎のエメラルド取得してるかどうか
				break;
			}
			else {
				eme_cnt++;
			}
		}
		
		main_work->has_emerald[i] = eme_cnt;
		
		// ここでエメラルド数変数初期化
		eme_cnt = 0;
		
#if _WII
		main_work->user_name[i] = opt_data.GetName();
		main_work->last_clear_act[i] = sys_data.GetLastClearAct();
#endif
	}
	
//	backup.SetSaveIndex(3);		// 仮設定	テスト用
	
	// 最後にプレイしたファイル番号をFOCUSに設定
	last_play_file = backup.GetSaveIndex();
	
	switch (last_play_file) {
	case 0:
		main_work->cur_file = 0;
		main_work->focus_disp_no = 0;
		main_work->crsr_idx = 0;
		break;
	case 1:
		main_work->cur_file = 1;
		main_work->focus_disp_no = 1;
		main_work->crsr_idx = 0;
		break;
	case 2:
		main_work->cur_file = 2;
		main_work->focus_disp_no = 2;
		main_work->crsr_idx = 0;
		break;
	case 3:
		main_work->cur_file = 3;
		main_work->focus_disp_no = 3;
		main_work->crsr_idx = 0;
		break;
	case 4:
		main_work->cur_file = 4;
		main_work->focus_disp_no = 3;
		main_work->crsr_idx = 1;
		break;
	case 5:
		main_work->cur_file = 5;
		main_work->focus_disp_no = 3;
		main_work->crsr_idx = 2;
		break;
	default:
		break;
	}
	
	main_work->prev_file = main_work->cur_file;
	main_work->prev_disp_no = main_work->focus_disp_no;
	main_work->crsr_prev_idx = main_work->crsr_idx;
	
	// 帯テキストの初期表示位置設定
	main_work->obi_tex_pos[0] = 0.f;
	main_work->obi_tex_pos[1] = DMD_FILESLCT_OBI_MOVE_START_POS;
}



// ==========================================================================
// dmFileSlctSetClearInfo
/*!
	クリア情報設定処理
 */
// ==========================================================================
/*
void dmFileSlctSetClearInfo(DMS_FILESLCT_MAIN_WORK *main_work)
{
	UNREFERENCED_PARAMETER(main_work);
}
*/



// ==========================================================================
// dmFileSlctProcMain
/*!
	セーブファイル選択画面メインプロシージャ処理
 */
// ==========================================================================
void dmFileSlctProcMain(MTS_TASK_TCB *tcb)
{
	DMS_FILESLCT_MAIN_WORK	*main_work;

	// ワーク取得
	main_work = (DMS_FILESLCT_MAIN_WORK *)mtTaskGetTcbWork(tcb);

	// 終了処理
	if (main_work->flag & DMD_FILESLCT_FLAG_EXIT) {
		// タスククリア
		DmFileSlctExit();
		
		// イベント遷移用設定
		
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
// dmFileSlctDest
/*!
	セーブファイル選択画面終了処理
 */
// ==========================================================================
void dmFileSlctDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	
}




// ==========================================================================
// dmFileSlctProcTitleStart
/*!
	タイトルスタート時の開始プロシージャ処理
 */
// ==========================================================================
void dmFileSlctProcTitleStart(DMS_FILESLCT_MAIN_WORK *main_work)
{
	
	
	for (int i = 0; i < DME_FILESLCT_DATA_TYPE_MAX; i++) {
		main_work->ama[i] = dm_fileslct_ama[i];
		main_work->amb[i] = dm_fileslct_amb[i];
		
		main_work->tex[i] = dm_fileslct_tex[i];
	}
	
	for (int i = 0; i < 4; i++) {
		main_work->cmn_ama[i] = dm_fileslct_cmn_ama[i];
		main_work->cmn_amb[i] = dm_fileslct_cmn_amb[i];
		main_work->cmn_tex[i] = dm_fileslct_cmn_tex[i];
	}
	
	main_work->proc_menu_update = dmFileSlctProcCreateAct;
}



// ==========================================================================
// dmFileSlctProcCreateAct
/*!
	アクション生成処理

  	※cur_fileを設定する際は必ずcrsr_idxとcur_vrtcl_fileを設定して
  	それらの和を設定すること。
 */
// ==========================================================================
void dmFileSlctProcCreateAct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	// ファイル選別
	const void *ama;
	AOS_TEXTURE *tex;
	
	// アクション構築
	for (u32 i = 0; i < ACT_NUM; ++i) {
		
		if (i >= ACT_TEX_WINTITLE) {
			ama = main_work->cmn_ama[3];
			tex = &main_work->cmn_tex[3];
		}
		else if (i >= ACT_WIN_LINE) {
			ama = main_work->cmn_ama[2];
			tex = &main_work->cmn_tex[2];
		}
		else if (i >= ACT_OBI_C) {
			ama = main_work->cmn_ama[1];
			tex = &main_work->cmn_tex[1];
		}
		else if (i >= ACT_FILESLCT_BACK_BTN) {
			ama = main_work->cmn_ama[0];
			tex = &main_work->cmn_tex[0];
		}
		else if (i >= ACT_WIN_TEX_BACK) {
			ama = main_work->ama[DME_FILESLCT_DATA_TYPE_LANG_DATA];
			tex = &main_work->tex[DME_FILESLCT_DATA_TYPE_LANG_DATA];
		}
#if _WII
		else if (i == ACT_TEX_OBI_USER1
				 || i == ACT_TEX_OBI_USER2) {
			ama = DmUserNameGetUserAma();
			tex = DmUserNameGetUserLangTex();
		}
#endif
		
		else if (i >= ACT_FILE_TEX_NAME) {
			ama = main_work->ama[DME_FILESLCT_DATA_TYPE_LANG_DATA];
			tex = &main_work->tex[DME_FILESLCT_DATA_TYPE_LANG_DATA];
		}
		else if (i >= ACT_FILE_TAB1) {
			ama = main_work->ama[DME_FILESLCT_DATA_TYPE_CMN_DATA];
			tex = &main_work->tex[DME_FILESLCT_DATA_TYPE_CMN_DATA];
		}
		else if (i >= ACT_TEX_DEL) {
			ama = main_work->ama[DME_FILESLCT_DATA_TYPE_LANG_DATA];
			tex = &main_work->tex[DME_FILESLCT_DATA_TYPE_LANG_DATA];
		}
		else {
			ama = main_work->ama[DME_FILESLCT_DATA_TYPE_CMN_DATA];
			tex = &main_work->tex[DME_FILESLCT_DATA_TYPE_CMN_DATA];
		}
		
		// 構築
		AoActSetTexture(AoTexGetTexList(tex));
		main_work->act[i] = AoActCreate(ama, g_dm_act_id_tbl[i]);
	}
	
	// カーソル初期表示位置設定
	main_work->crsr_pos_y = dm_fileslct_act_crsr_disp_y_pos_tbl[main_work->crsr_idx];
	
	// ファイルテーブルノード作成
	AoActSetTexture(AoTexGetTexList(tex));
	
	// ここで初期表示位置設定
	for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
		main_work->file_tab_pos_y[i] = DMD_FILESLCT_IN_EFCT_START_POS;
		main_work->file_move_pos_src[i] = DMD_FILESLCT_IN_EFCT_START_POS;
		main_work->file_move_pos_dst[i] = dm_fileslct_file_disp_y_pos_tbl[main_work->focus_disp_no];
//		main_work->file_move_pos_dst[i] = 0.f;
	}

	// 帯初期表示位置設定
	main_work->obi_pos_y = DMD_FILESLCT_OBI_NODISP_POS_Y;
	
//	main_work->crsr_pos_y = dm_fileslct_act_disp_y_pos_tbl[0];
	main_work->flag |= DMD_FILESLCT_FLAG_IS_FADEIN;
	
	// テクスチャセットまで出来たので描画プロシージャを設定
	main_work->proc_draw = dmFileSlctProcActDraw;
	
	// イベント遷移
	main_work->proc_menu_update = dmFileSlctProcFileTabInEfct;
}



// ==========================================================================
// dmFileSlctProcFileTabInEfct
/*!
	セーブファイル選択時のセーブファイルテーブル入り演出中処理
 */
// ==========================================================================
void dmFileSlctProcFileTabInEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	// 入り演出が終了したら
	if (dmFileSlctIsFileTabInEfctEnd(main_work)
		&& dmFileSlctIsObiInEfctEnd(main_work)) {
		main_work->proc_menu_update = dmFileSlctProcFileSelectIdle;

		main_work->proc_input = dmFileSlctInputProcFileSelect;
		
		main_work->proc_win_update = dmFileSlctProcWindowNodispIdle;

		// このタイミングで上下の矢印とカーソル、帯内のFIXを表示
		main_work->disp_flag |= DMD_FILESLCT_DISP_FLAG_TB_ARROW;
		main_work->disp_flag |= DMD_FILESLCT_DISP_FLAG_ACT_CRSR;
		main_work->disp_flag |= DMD_FILESLCT_DISP_FLAG_OBI_TEX;

		main_work->timer = 0;

		for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
			main_work->file_tab_pos_y[i] = main_work->file_move_pos_dst[i];
		}

		main_work->obi_pos_y = DMD_FILESLCT_OBI_DISP_POS_Y;

		return;
	}
	
	// ファイル入り演出処理
	dmFileSlctSetFileTabInEfct(main_work);

	// 帯入り演出処理
	if (!dmFileSlctIsObiInEfctEnd(main_work)) {
		dmFileSlctSetObiInEfct(main_work);
	}

	// 座標再設定
	main_work->timer += 1.0f;
}



// ==========================================================================
// dmFileSlctProcFileSelectIdle
/*!
	セーブファイル選択時の入力待ち中処理
 */
// ==========================================================================
void dmFileSlctProcFileSelectIdle(DMS_FILESLCT_MAIN_WORK *main_work)
{
	// 入力処理
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}

	// キャンセルフラグONならば
	if (main_work->flag & DMD_FILESLCT_FLAG_CANCEL) {
		// タイトル側へ戻る(掃け演出開始)
		main_work->proc_menu_update = dmFileSlctProcFileTabOutEfct;

		// 掃け演出用設定
		dmFileSlctSetOutEfctData(main_work);
		
		// フラグOFF
		main_work->flag &= ~DMD_FILESLCT_FLAG_DECIDE;
		main_work->flag &= ~DMD_FILESLCT_FLAG_CANCEL;
		
		// メインメニューへイベント遷移先設定
		main_work->next_evt = DME_FILESLCT_NEXT_EVT_MAINMENU;
		dm_fileslct_is_file_decide = -1;
		
		DmSoundPlaySE("Cancel");
		
		return;
	}

	// 決定フラグONならば
	if (main_work->flag & DMD_FILESLCT_FLAG_DECIDE) {
		// 決定演出へ移行
		main_work->proc_menu_update = dmFileSlctProcFileTabDecideEfct;

		dmFileSlctSetDecideEfctData(main_work);

		// フラグOFF
		main_work->flag &= ~DMD_FILESLCT_FLAG_DECIDE;
		main_work->flag &= ~DMD_FILESLCT_FLAG_CANCEL;
		
		dm_fileslct_is_file_decide = 1;
		
		DmSoundPlaySE("Ok");
		
		return;
	}

	// カーソル切り替え
	if (main_work->flag & DMD_FILESLCT_FLAG_UP_CHNG_CRSR
		|| main_work->flag & DMD_FILESLCT_FLAG_DOWN_CHNG_CRSR) {
		// 決定演出へ移行
		main_work->proc_menu_update = dmFileSlctProcChngFocusFile;

		dmFileSlctSetFocusChangeEfctData(main_work);
		main_work->timer = 0;

		// フラグOFF
		main_work->flag &= ~DMD_FILESLCT_FLAG_UP_CHNG_CRSR;
		main_work->flag &= ~DMD_FILESLCT_FLAG_DOWN_CHNG_CRSR;
		
		DmSoundPlaySE("Cursol");
		
		return;
	}
	
	
}



// ==========================================================================
// dmFileSlctProcChngFocusFile
/*!
	セーブファイル選択時のFOCUS切り替え演出中処理
 */
// ==========================================================================
void dmFileSlctProcChngFocusFile(DMS_FILESLCT_MAIN_WORK *main_work)
{
	// 入力処理
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}

	if (main_work->flag & DMD_FILESLCT_FLAG_UP_CHNG_CRSR
		|| main_work->flag & DMD_FILESLCT_FLAG_DOWN_CHNG_CRSR) {
		dmFileSlctSetFocusChangeEfctData(main_work);
		main_work->timer = 0;
		// フラグOFF
		main_work->flag &= ~DMD_FILESLCT_FLAG_UP_CHNG_CRSR;
		main_work->flag &= ~DMD_FILESLCT_FLAG_DOWN_CHNG_CRSR;
		
		DmSoundPlaySE("Cursol");
	}
	
	// 掃け演出が終了したら
	if (!(main_work->flag & DMD_FILESLCT_FLAG_MOVE_VRTCL)
		&& !(main_work->flag & DMD_FILESLCT_FLAG_MOVE_CRSR)) {
		
		main_work->proc_menu_update = dmFileSlctProcFileSelectIdle;
		
		// ここの遷移はキャンセルか決定かで分岐する
		main_work->timer = 0;

		return;
	}

	// 掃け演出処理
	if (main_work->flag & DMD_FILESLCT_FLAG_MOVE_VRTCL) {
		dmFileSlctSetFileVrtclChangeEfct(main_work);
		
		if (dmFileSlctIsFileVrtclChangeEfct(main_work)) {
			main_work->flag &= ~DMD_FILESLCT_FLAG_MOVE_VRTCL;
		}
	}

	// カーソル移動
	if (main_work->flag & DMD_FILESLCT_FLAG_MOVE_CRSR) {
		dmFileSlctSetStageCrsrChangeEfct(main_work);
		
		if (dmFileSlctIsStageCrsrChangeEfct(main_work)) {
			main_work->flag &= ~DMD_FILESLCT_FLAG_MOVE_CRSR;
		}
	}
	
	// 座標再設定
	main_work->timer++;
}



// ==========================================================================
// dmFileSlctProcFileTabDecideEfct
/*!
	セーブファイル選択時のステージテーブル決定演出中処理
 */
// ==========================================================================
void dmFileSlctProcFileTabDecideEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
#if _WII
	GSS_MAIN_SYS_INFO *gs_main = GsGetMainSysInfo();
#endif
	
	// 入り演出が終了したら
	if (dmFileSlctIsFileTabDecideEfctEnd(main_work)) {
		
#if _WII
		gs::backup::SBackup &data = gs::backup::SBackup::CreateInstance();
		data.SetSaveIndex((u32)main_work->cur_file);
		
#endif
		
		// セーブデータがある場合
		if (main_work->is_save_exist & 1 << main_work->cur_file) {
			main_work->proc_menu_update = dmFileSlctProcFadeOut;
			main_work->flag |= DMD_FILESLCT_FLAG_IS_FADEOUT;
			main_work->proc_win_update = NULL;
		}
		// 新規データの場合
		else {
			main_work->proc_menu_update = dmFileSlctProcSetUserName;
			
#if _WII
			// バックアップ初期化
			gs::backup::SSystem &sys_data = data.GetSystem(data.GetSaveIndex());
			sys_data.Init();
			gs::backup::SOption &opt_data = data.GetOption(data.GetSaveIndex());
			opt_data.Init();
			gs::backup::SStage &stg_data = data.GetStage(data.GetSaveIndex());
			stg_data.Init();
			gs::backup::SSpecial &spe_data = data.GetSpecial(data.GetSaveIndex());
			spe_data.Init();
#endif
			
			main_work->flag |= DMD_FILESLCT_FLAG_SET_USERNAME;
			DmUserNameStart();
		}
		
#if _WII
		// 選んだファイル番号のバックアップをGSに設定
		amCopyMemory(&gs_main->cmp_backup
					 , &gs_main->backup
					 , sizeof(GSS_BACKUP));
#endif
		
		main_work->timer = 0.0f;
		
		return;
	}
	
	// 決定項目以外の掃け演出処理
	dmFileSlctSetFileTabDecideEfct(main_work);

	// 決定項目の掃け演出
	if (main_work->timer > DMD_FILESLCT_DECIDE_EFCT_WAIT_TIME) {
		dmFileSlctSetFileCurTabDecideEfct(main_work);
	}

	main_work->timer++;
}



// ==========================================================================
// dmFileSlctProcSetUserName
/*!
	セーブファイル選択時のユーザー名設定中処理
 */
// ==========================================================================
void dmFileSlctProcSetUserName(DMS_FILESLCT_MAIN_WORK *main_work)
{
	s32 check_ret_user = 0;
	
	check_ret_user = DmUserNameIsExit();
	
	if (check_ret_user == 1) {
		main_work->proc_menu_update = dmFileSlctProcUserNameSaveWait;
		main_work->proc_win_update = NULL;
		
		// 初の名前設定はセーブする
		DmSaveMenuStart();
	}
	else if (check_ret_user == -1) {
		for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
			main_work->file_move_pos_src[i] = main_work->file_tab_pos_y[i];
			main_work->file_move_pos_dst[i] = main_work->file_back_pos[i];
		}
		main_work->proc_menu_update = dmFileSlctProcCancelUserName;
	}
}



// ==========================================================================
// dmFileSlctProcUserNameSaveWait
/*!
	セーブファイル選択時のユーザー名セーブ中処理
 */
// ==========================================================================
void dmFileSlctProcUserNameSaveWait(DMS_FILESLCT_MAIN_WORK *main_work)
{
	if (DmSaveIsExit()) {
		main_work->proc_menu_update = dmFileSlctProcFadeOut;
		main_work->flag |= DMD_FILESLCT_FLAG_IS_FADEOUT;
		main_work->flag &= ~DMD_FILESLCT_FLAG_SET_USERNAME;
		main_work->proc_win_update = NULL;
	}
	
	// 帯掃け演出処理
	if (!dmFileSlctIsObiOutEfctEnd(main_work)) {
		dmFileSlctSetObiOutEfct(main_work);
	}
}



// ==========================================================================
// dmFileSlctProcCancelUserName
/*!
	ユーザー名設定時にキャンセルになった場合の処理
 */
// ==========================================================================
void dmFileSlctProcCancelUserName(DMS_FILESLCT_MAIN_WORK *main_work)
{
	
	dmFileSlctSetFileTabBackEfct(main_work);
	
	
	if (dmFileSlctIsFileTabBackEfctEnd(main_work)) {
		main_work->proc_menu_update = dmFileSlctProcFileSelectIdle;
		
		main_work->proc_input = dmFileSlctInputProcFileSelect;
		
		main_work->proc_win_update = dmFileSlctProcWindowNodispIdle;
		
		main_work->flag &= ~DMD_FILESLCT_FLAG_SET_USERNAME;
		
		main_work->disp_flag |= DMD_FILESLCT_DISP_FLAG_TB_ARROW;
		main_work->disp_flag |= DMD_FILESLCT_DISP_FLAG_ACT_CRSR;
		main_work->disp_flag |= DMD_FILESLCT_DISP_FLAG_OBI_TEX;
		
		return;
	}
	
	main_work->timer += 1.0f;
}



// ==========================================================================
// dmFileSlctProcFileTabOutEfct
/*!
	セーブファイル選択時のセーブファイルテーブル掃け演出中処理
 */
// ==========================================================================
void dmFileSlctProcFileTabOutEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	// 掃け演出が終了したら
	if (dmFileSlctIsFileTabOutEfctEnd(main_work)
		&& dmFileSlctIsObiOutEfctEnd(main_work)) {

		main_work->timer = 0;

		for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
			main_work->file_tab_pos_y[i] = main_work->file_move_pos_dst[i];
		}

		main_work->obi_pos_y = DMD_FILESLCT_OBI_NODISP_POS_Y;
		
		main_work->flag |= DMD_FILESLCT_FLAG_IS_FADEOUT;

		// 遷移先なし
		main_work->proc_win_update = NULL;
		main_work->proc_menu_update = dmFileSlctProcFadeOut;
		
		return;
	}
	
	// ファイル入り演出処理
	dmFileSlctSetFileTabOutEfct(main_work);

	// 帯掃け演出処理
	if (!dmFileSlctIsObiOutEfctEnd(main_work)) {
		dmFileSlctSetObiOutEfct(main_work);
	}

	// 座標再設定
	main_work->timer += 1.0f;
	
}



// ==========================================================================
// dmFileSlctProcWindowNodispIdle
/*!
	ウインドウ非表示待ち中処理
 */
// ==========================================================================
void dmFileSlctProcWindowNodispIdle(DMS_FILESLCT_MAIN_WORK *main_work)
{
	// ウインドウ入力処理
	if (main_work->proc_win_input) {
		main_work->proc_win_input(main_work);
	}

	// メニュー遷移フラグONならば
	if (main_work->flag & DMD_FILESLCT_FLAG_DEL_SAVE_FILE
		|| main_work->announce_flag) {
		main_work->proc_win_update = dmFileSlctProcWindowOpenEfct;

		// 通常処理の入力処理をなくす(二重入力を防ぐため)
		main_work->proc_input = NULL;

		// ウインドウ開閉演出時は入力処理なし
		main_work->proc_win_input = NULL;

		// ウインドウ演出用タイマー初期化
		main_work->win_timer = 0;

		// ウインドウ選択変数設定
		for (u32 i = 0; i < DME_FILESLCT_WIN_NUM; i++) {
			if (main_work->announce_flag & 1 << i) {
				main_work->win_mode = (s32)i;
				break;
			}
		}
		main_work->win_cur_slct = 1;

		// ウインドウ演出中フラグON
		main_work->flag &= ~DMD_FILESLCT_FLAG_DEL_SAVE_FILE;
//		main_work->flag |= DMD_FILESLCT_FLAG_WIN_EFCT;
		
		// ウインドウオープンSE再生
		DmSoundPlaySE("Window");
	}

	
}




// ==========================================================================
// dmFileSlctProcWindowOpenEfct
/*!
	ウインドウオープン中処理
 */
// ==========================================================================
void dmFileSlctProcWindowOpenEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	// 演出終了チェック
	if (main_work->flag & DMD_FILESLCT_FLAG_WIN_EFCT_END) {
		// ウインドウのプロシージャ設定
		main_work->proc_win_update = dmFileSlctProcWindowAnnounceIdle;
		
		// 入力処理設定
		main_work->proc_win_input = dmFileSlctInputProcWinDispIdle;
		
		// ウインドウ内アクション表示フラグON
		main_work->disp_flag |= DMD_FILESLCT_DISP_FLAG_WIN_ACT;
		
		if (main_work->win_mode == DME_FILESLCT_WIN_DELETING_FILE) {
			// データ削除中ウインドウの表示時間タイマー初期化
			main_work->del_timer = 0;
		}
		
		// ウインドウ演出中フラグOFF
		main_work->flag &= ~DMD_FILESLCT_FLAG_WIN_EFCT_END;
	}
	else {
		// ウインドウオープン演出処理
		dmFileSlctSetWinOpenEfct(main_work);
//		main_work->flag |= DMD_FILESLCT_FLAG_WIN_EFCT_END;
	}

	// ウインドウ描画
//	dmFileSlctWinSelectDraw(main_work);
}



// ==========================================================================
// dmFileSlctProcWindowAnnounceIdle
/*!
	ウインドウ入力待ち処理
 */
// ==========================================================================
void dmFileSlctProcWindowAnnounceIdle(DMS_FILESLCT_MAIN_WORK *main_work)
{
	// ウインドウ入力処理
	if (main_work->proc_win_input) {
		main_work->proc_win_input(main_work);
	}

	// ACT決定ウインドウ以外の場合
	if (main_work->win_mode == DME_FILESLCT_WIN_IS_DEL_FILE) {
	// メニュー遷移フラグONならば
		if (main_work->flag & DMD_FILESLCT_FLAG_DECIDE
			&& !main_work->win_cur_slct) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_FILESLCT_WIN_EFCT_TIME;
//			main_work->flag |= DMD_FILESLCT_FLAG_WIN_EFCT;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_FILESLCT_DISP_FLAG_WIN_ACT;
			
			main_work->proc_win_update = dmFileSlctProcWindowCloseEfct;
			
			// ここで次に表示するメッセージフラグをON
			main_work->announce_flag |= 1 << DME_FILESLCT_WIN_DELETING_FILE;
			
			// ここでセーブ削除処理開始させる
			dmFileSlctDeleteSaveFile(main_work);
			
			// フラグOFF
			main_work->flag &= ~DMD_FILESLCT_FLAG_DECIDE;
			main_work->flag &= ~DMD_FILESLCT_FLAG_CANCEL;
			
			DmSoundPlaySE("Ok");
		}
		
		else if (main_work->flag & DMD_FILESLCT_FLAG_DECIDE
			|| main_work->flag & DMD_FILESLCT_FLAG_CANCEL) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_FILESLCT_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_FILESLCT_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmFileSlctProcWindowCloseEfct;

			// フラグOFF
			main_work->flag &= ~DMD_FILESLCT_FLAG_DECIDE;
			main_work->flag &= ~DMD_FILESLCT_FLAG_CANCEL;
			
			if (main_work->flag & DMD_FILESLCT_FLAG_CANCEL) {
				DmSoundPlaySE("Cancel");
			}
			else {
				DmSoundPlaySE("Ok");
			}
		}
	}
	
	
	else if (main_work->win_mode == DME_FILESLCT_WIN_DELETING_FILE) {
		if (dmFileSlctIsDeleteSaveFile(main_work)) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_FILESLCT_WIN_EFCT_TIME;
//			main_work->flag |= DMD_FILESLCT_FLAG_WIN_EFCT;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_FILESLCT_DISP_FLAG_WIN_ACT;

			main_work->proc_win_update = dmFileSlctProcWindowCloseEfct;

			// フラグOFF
			main_work->flag &= ~DMD_FILESLCT_FLAG_DECIDE;
			main_work->flag &= ~DMD_FILESLCT_FLAG_CANCEL;
		}
	}

	// ウインドウ描画
//	dmFileSlctWinSelectDraw(main_work);
}



// ==========================================================================
// dmFileSlctProcWindowCloseEfct
/*!
	ウインドウクローズ中処理
 */
// ==========================================================================
void dmFileSlctProcWindowCloseEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	// 演出終了チェック
	if (main_work->flag & DMD_FILESLCT_FLAG_WIN_EFCT_END) {
		// ウインドウのプロシージャ設定
		main_work->proc_win_update = dmFileSlctProcWindowNodispIdle;
		
		// アナウンス分のフラグOFF
		main_work->announce_flag &= ~(1 << main_work->win_mode);

		if (!main_work->announce_flag) {
			// 入力処理設定
			main_work->proc_win_input = dmFileSlctInputProcWinNodispIdle;

			main_work->proc_input = dmFileSlctInputProcFileSelect;
		}
		
		// ウインドウ演出中フラグOFF
		main_work->flag &= ~DMD_FILESLCT_FLAG_WIN_EFCT_END;

	}
	
	// ウインドウオープン演出処理
	dmFileSlctSetWinCloseEfct(main_work);
	
	// ウインドウ描画
//	dmFileSlctWinSelectDraw(main_work);
}



// ==========================================================================
// dmFileSlctProcFadeOut
/*!
	フェードアウト処理
 */
// ==========================================================================
void dmFileSlctProcFadeOut(DMS_FILESLCT_MAIN_WORK *main_work)
{
	if (!(main_work->flag & DMD_FILESLCT_FLAG_IS_FADEOUT)) {
		main_work->proc_menu_update = dmFileSlctProcStopDraw;
		main_work->proc_draw = NULL;
	}
}



// ==========================================================================
// dmFileSlctProcStopDraw
/*!
	描画停止処理
 */
// ==========================================================================
void dmFileSlctProcStopDraw(DMS_FILESLCT_MAIN_WORK *main_work)
{
	main_work->proc_menu_update = NULL;
//	main_work->proc_menu_update = dmFileSlctProcDataRelease;
	main_work->proc_win_update = NULL;
	
	main_work->flag |= DMD_FILESLCT_FLAG_EXIT;
	
	for (int i = 0; i < ACT_NUM; i++) {
		if (main_work->act[i]) {
			AoActDelete(main_work->act[i]);
			main_work->act[i] = NULL;
		}
	}
}



// ==========================================================================
// dmFileSlctInputProcFileSelect
/*!
	STAGE選択用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmFileSlctInputProcFileSelect(DMS_FILESLCT_MAIN_WORK *main_work)
{
	// キャンセル処理
	if (AoPadStand() & GSD_KEY_CANCEL) {
		main_work->flag |= DMD_FILESLCT_FLAG_CANCEL;

		return;
	}
	
	// ロードファイル決定処理
	if (AoPadStand() & GSD_KEY_DECIDE) {
		main_work->flag |= DMD_FILESLCT_FLAG_DECIDE;

		return;
	}

	// セーブデータ削除
	if (AoPadStand() & KEY_SELECT) {
		// セーブファイルが存在するならば
		if (main_work->is_save_exist & (1 << main_work->cur_file)) {
			main_work->flag |= DMD_FILESLCT_FLAG_DEL_SAVE_FILE;
			main_work->announce_flag |= (1 << DME_FILESLCT_WIN_IS_DEL_FILE);
		}

		return;
	}

	// 十字キー操作
	if (AoPadMRepeat() & GSD_KEY_UP) {
		if (AoPadMStand() & GSD_KEY_UP
			|| main_work->cur_file != 0) {
			
			if (main_work->crsr_idx == 0) {
				// テーブル移動演出フラグON
				main_work->flag |= DMD_FILESLCT_FLAG_CHNG_VRTCL;
				
				if (main_work->focus_disp_no == 0) {
					main_work->flag |= DMD_FILESLCT_FLAG_CHNG_CRSR;
				}
			}
			
			else {
				// カーソル移動フラグON
				main_work->flag |= DMD_FILESLCT_FLAG_CHNG_CRSR;
			}
			
			main_work->flag |= DMD_FILESLCT_FLAG_UP_CHNG_CRSR;
		}

		return;
	}
	
	else if (AoPadMRepeat() & GSD_KEY_DOWN) {
		if (AoPadMStand() & GSD_KEY_DOWN
			|| main_work->cur_file != DMD_FILESLCT_SAVE_FILE_NUM - 1) {

			if (main_work->crsr_idx == DMD_FILESLCT_CRSR_POS_NUM - 1) {
				// テーブル移動演出フラグON
				main_work->flag |= DMD_FILESLCT_FLAG_CHNG_VRTCL;
				
				if (main_work->focus_disp_no == DMD_FILESLCT_FILE_POS_NUM - 1) {
					main_work->flag |= DMD_FILESLCT_FLAG_CHNG_CRSR;
				}
			}
			
			else {
				// カーソル移動フラグON
				main_work->flag |= DMD_FILESLCT_FLAG_CHNG_CRSR;
			}
			
			main_work->flag |= DMD_FILESLCT_FLAG_DOWN_CHNG_CRSR;
		}

		return;
	}
	
}



// ==========================================================================
// dmFileSlctInputProcFileSelectMove
/*!
	STAGE選択用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
/*
void dmFileSlctInputProcFileSelectMove(DMS_FILESLCT_MAIN_WORK *main_work)
{
	// キャンセル処理
	if (AoPadStand() & GSD_KEY_CANCEL) {
		main_work->flag |= DMD_FILESLCT_FLAG_CANCEL;

		return;
	}
	
	// ステージ決定処理
	if (AoPadStand() & GSD_KEY_DECIDE) {
		main_work->flag |= DMD_FILESLCT_FLAG_DECIDE;

		return;
	}
	
	// セーブデータ削除
	if (AoPadStand() & KEY_SELECT) {
		// セーブファイルが存在するならば
		if (main_work->is_save_exist & (1 << main_work->cur_file)) {
			main_work->flag |= DMD_FILESLCT_FLAG_DEL_SAVE_FILE;
			main_work->announce_flag |= (1 << DME_FILESLCT_WIN_IS_DEL_FILE);
		}

		return;
	}

	// 十字キー操作
	if (AoPadMRepeat() & GSD_KEY_UP) {
		if (AoPadMStand() & GSD_KEY_UP
			|| main_work->cur_file != 0) {
			
			if (main_work->crsr_idx == 0) {
				// テーブル移動演出フラグON
				main_work->flag |= DMD_FILESLCT_FLAG_CHNG_VRTCL;
			}
			
			else {
				// カーソル移動フラグON
				main_work->flag |= DMD_FILESLCT_FLAG_CHNG_CRSR;
			}
			
			main_work->flag |= DMD_FILESLCT_FLAG_UP_CHNG_CRSR;
		}

		return;
	}
	
	else if (AoPadMRepeat() & GSD_KEY_DOWN) {
		if (AoPadMStand() & GSD_KEY_DOWN
			|| main_work->cur_file != DMD_FILESLCT_SAVE_FILE_NUM - 1) {

			if (main_work->crsr_idx == DMD_FILESLCT_CRSR_POS_NUM - 1) {
				// テーブル移動演出フラグON
				main_work->flag |= DMD_FILESLCT_FLAG_CHNG_VRTCL;
			}
			
			else {
				// カーソル移動フラグON
				main_work->flag |= DMD_FILESLCT_FLAG_CHNG_CRSR;
			}
			
			main_work->flag |= DMD_FILESLCT_FLAG_DOWN_CHNG_CRSR;
		}

		return;
	}
	
}
*/



// ==========================================================================
// dmFileSlctInputProcWinNodispIdle
/*!
	ウインドウ非表示用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmFileSlctInputProcWinNodispIdle(DMS_FILESLCT_MAIN_WORK *main_work)
{
//	u32 tmp_cur_zone = main_work->cur_zone;
	
	// メニュー遷移フラグ処理
	if (AoPadStand() & KEY_R_UP) {
		main_work->flag |= DMD_FILESLCT_FLAG_DISP_MENU;

		// メニューウインドウ変数設定
		main_work->win_mode = DME_FILESLCT_WIN_IS_DEL_FILE;

		return;
	}
	
}



// ==========================================================================
// dmFileSlctInputProcWinDispIdle
/*!
	ウインドウ表示用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmFileSlctInputProcWinDispIdle(DMS_FILESLCT_MAIN_WORK *main_work)
{
	if (main_work->win_mode == DME_FILESLCT_WIN_IS_DEL_FILE) {
		// キャンセル処理
		if (AoPadStand() & GSD_KEY_CANCEL) {
			main_work->flag |= DMD_FILESLCT_FLAG_CANCEL;
	
			return;
		}
		
		// 決定処理
		if (AoPadStand() & GSD_KEY_DECIDE) {
			main_work->flag |= DMD_FILESLCT_FLAG_DECIDE;
			
			return;
		}
		
//	if (main_work->win_mode == DME_FILESLCT_WIN_IS_DEL_FILE) {
		if (AoPadMStand() & GSD_KEY_LEFT) {
			if (main_work->win_cur_slct != 0) {
				DmSoundPlaySE("Cursol");
			}
			
			// 選択項目切り替え
			main_work->win_cur_slct = 0;
		}
		else if (AoPadMStand() & GSD_KEY_RIGHT) {
			if (main_work->win_cur_slct != 1) {
				DmSoundPlaySE("Cursol");
			}
			
			// 選択項目切り替え
			main_work->win_cur_slct = 1;
		}
	}


	
}



// ==========================================================================
// dmFileSlctProcActDraw
/*!
	描画設定プロシージャ処理
 */
// ==========================================================================
void dmFileSlctProcActDraw(DMS_FILESLCT_MAIN_WORK *main_work)
{
	// 帯移動演出用		※常に移動しつづけるのでここに配置
	dmFileSlctSetObiEfctPos(main_work);
	
	// 共通描画処理は描画時は常に設定
	dmFileSlctCommonDraw(main_work);

	// セーブファイル描画設定
	dmFileSlctSaveFileDraw(main_work);

	// FIX関連描画設定
	dmFileSlctCommonFixDraw(main_work);
	
	// ウインドウ関連描画設定
	if (main_work->proc_win_update != NULL
		&& main_work->proc_win_update != dmFileSlctProcWindowNodispIdle) {
		dmFileSlctWinSelectDraw(main_work);
	}

	// 描画タスク生成
	if (main_work->draw_state) {
//		amDrawMakeTask(dmFileSlctTaskDraw, (u16)0x8000, (u32)0);
	}
}



// ==========================================================================
// dmFileSlctCommonDraw
/*!
	共通描画設定処理
 */
// ==========================================================================
void dmFileSlctCommonDraw(DMS_FILESLCT_MAIN_WORK *main_work)
{
	if (main_work->flag & DMD_FILESLCT_FLAG_IS_FADEIN) {
		main_work->back_bg_frame++;

		if (main_work->back_bg_frame > 30.f) {
			main_work->back_bg_frame = 30.f;
			main_work->flag &= ~DMD_FILESLCT_FLAG_IS_FADEIN;
		}
	}
	
	if (main_work->flag & DMD_FILESLCT_FLAG_IS_FADEOUT) {
		main_work->back_bg_frame++;

		if (main_work->back_bg_frame > 62.f) {
			main_work->back_bg_frame = 62.f;
			main_work->flag &= ~DMD_FILESLCT_FLAG_IS_FADEOUT;
		}
	}
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_FILESLCT_DRAW_PRIO_BG);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));

	// アクション登録(登録は全てここで行うようにし、実際の登録するかはフラグにて設定するようにする)
	AoActSortRegAction(main_work->act[ACT_BACK_BG]);
	
	AoActSetFrame(main_work->act[ACT_BACK_BG], main_work->back_bg_frame);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));

	AoActUpdate(main_work->act[ACT_BACK_BG], 0.0f);
	
	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();
}



// ==========================================================================
// dmFileSlctCommonFixDraw
/*!
	FIX描画設定処理
 */
// ==========================================================================
void dmFileSlctCommonFixDraw(DMS_FILESLCT_MAIN_WORK *main_work)
{
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_FILESLCT_DRAW_PRIO_FIX);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));

	AoActSortRegAction(main_work->act[ACT_UP_HIDE_BG]);
	AoActSortRegAction(main_work->act[ACT_DOWN_HIDE_BG]);
	
	AoActSetFrame(main_work->act[ACT_UP_HIDE_BG], main_work->back_bg_frame);
	AoActSetFrame(main_work->act[ACT_DOWN_HIDE_BG], main_work->back_bg_frame);
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	AoActUpdate(main_work->act[ACT_UP_HIDE_BG], 0.f);
	AoActUpdate(main_work->act[ACT_DOWN_HIDE_BG], 0.f);
	
	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();
	
	
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_FILESLCT_DRAW_PRIO_FIX);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));

	if (main_work->disp_flag & DMD_FILESLCT_DISP_FLAG_TB_ARROW) {
		AoActSortRegAction(main_work->act[ACT_ARROW_UP]);
		AoActSortRegAction(main_work->act[ACT_ARROW_DOWN]);
	}
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[1]));
	AoActSortRegAction(main_work->act[ACT_OBI_C]);
	
#if _WII
	if (main_work->flag & DMD_FILESLCT_FLAG_SET_USERNAME) {
		AoActSetTexture(AoTexGetTexList(DmUserNameGetUserLangTex()));
		AoActSortRegAction(main_work->act[ACT_TEX_OBI_USER1]);
		AoActSortRegAction(main_work->act[ACT_TEX_OBI_USER2]);
	}
	else {
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		AoActSortRegAction(main_work->act[ACT_TEX_EXP1]);
		AoActSortRegAction(main_work->act[ACT_TEX_EXP2]);
	}
#else
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	AoActSortRegAction(main_work->act[ACT_TEX_EXP1]);
	AoActSortRegAction(main_work->act[ACT_TEX_EXP2]);
#endif
	
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[1]));
	for (int i = ACT_OBI_L; i <= ACT_OBI_R2; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[0]));
	
	if (main_work->disp_flag & DMD_FILESLCT_DISP_FLAG_OBI_TEX) {
		if (main_work->is_save_exist & (1 << main_work->cur_file)) {
			AoActSortRegAction(main_work->act[ACT_DEL_BTN]);
		}
	}
	
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
	if (main_work->disp_flag & DMD_FILESLCT_DISP_FLAG_OBI_TEX) {
		if (main_work->is_save_exist & (1 << main_work->cur_file)) {
			AoActSortRegAction(main_work->act[ACT_TEX_DEL]);
		}
	}
	
	// 右下配置の戻るボタン
	if (!dmFileSlctSetMsgWinState(main_work)) {
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[0]));
		AoActSortRegAction(main_work->act[ACT_FILESLCT_BACK_BTN]);
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
		AoActSortRegAction(main_work->act[ACT_TEX_FIX_BACK]);
	}
	
	// 固定物の更新
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[0]));
	AoActUpdate(main_work->act[ACT_DEL_BTN], 0);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	AoActUpdate(main_work->act[ACT_ARROW_UP], 1.f);
	AoActUpdate(main_work->act[ACT_ARROW_DOWN], 1.f);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	AoActUpdate(main_work->act[ACT_TEX_DEL], 0);
	
	
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[1]));
	
	// ACCUMURATEでトランスさせる
	AoActAcmPush();

	// 帯部分(テキストは含まない)
	for (int i = ACT_OBI_C; i <= ACT_OBI_R2; i++) {
		AoActAcmInit();
		AoActAcmApplyTrans(0
						   , main_work->obi_pos_y
						   , 0
						   );
		
		// フレーム更新はSetFrameのみで行う
		AoActUpdate(main_work->act[i], 0.0f);
	}
	
	AoActAcmPop();

	// ACCUMURATEでトランスさせる
	AoActAcmPush();
	
	for (int i = 0; i < 2; i++) {
		// 帯テキスト部分
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->obi_tex_pos[i]
						   , main_work->obi_pos_y
						   , 0
						   );
		
		// フレーム更新はSetFrameのみで行う
#if _WII
		if (main_work->flag & DMD_FILESLCT_FLAG_SET_USERNAME) {
			AoActSetTexture(AoTexGetTexList(DmUserNameGetUserLangTex()));
			AoActUpdate(main_work->act[ACT_TEX_OBI_USER1 + i], 0.0f);
		}
		else {
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			AoActUpdate(main_work->act[ACT_TEX_EXP1 + i], 0.0f);
		}
#else
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		AoActUpdate(main_work->act[ACT_TEX_EXP1 + i], 0.0f);
#endif
	}
	
	AoActAcmPop();
	
	// 右下配置の戻るボタン
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[0]));
	AoActUpdate(main_work->act[ACT_FILESLCT_BACK_BTN], 0.0f);
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
	AoActUpdate(main_work->act[ACT_TEX_FIX_BACK], 0.0f);
	
	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();
}



// ==========================================================================
// dmFileSlctSaveFileDraw
/*!
	セーブファイルテーブル描画設定処理
 */
// ==========================================================================
void dmFileSlctSaveFileDraw(DMS_FILESLCT_MAIN_WORK *main_work)
{
	float tmp_pos_x = 0.f;
	float tmp_disp_clear_scr = 0.f;
	
#if _WII
	OSCalendarTime td;
	amZeroMemory(&td, sizeof(OSCalendarTime));
	s32 tmp_time_act[3];
#endif
	
	
	u32 tmp_year[4];
	u32 tmp_month[2];
	u32 tmp_day[2];
	u32 tmp_hour[2];
	u32 tmp_min[2];
	
	amZeroMemory(tmp_year, sizeof(u32) * 4);
	amZeroMemory(tmp_month, sizeof(u32) * 2);
	amZeroMemory(tmp_day, sizeof(u32) * 2);
	amZeroMemory(tmp_hour, sizeof(u32) * 2);
	amZeroMemory(tmp_min, sizeof(u32) * 2);
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_FILESLCT_DRAW_PRIO_SAVEFILE);
	
//	AoActSetFrame(main_work->act[ACT_TEX_MODE], (f32)main_work->cur_game_mode);
	
	for (int i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
		
		// アクション更新
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));

		AoActSortRegAction(main_work->act[ACT_FILE_TAB1]);
		AoActSortRegAction(main_work->act[ACT_FILE_TAB_NUM]);
		
		// テーブル部分
		if (main_work->is_save_exist & (1 << i)) {
			AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
			
			AoActSortRegAction(main_work->act[ACT_FILE_SCR_BASE]);
			
			for (u32 k = ACT_FILE_TAB_EMER; k <= ACT_FILE_TIME_M_2; k++) {
				AoActSortRegAction(main_work->act[k]);
			}
			
			if (GsEnvGetRegion() == GSD_REGION_JP) {
				for (u32 k = ACT_FILE_YEAR1; k <= ACT_FILE_DAY2; k++) {
					AoActSortRegAction(main_work->act[k]);
				}
			}
			else if (GsEnvGetRegion() == GSD_REGION_US) {
				for (u32 k = ACT_FILE_YEAR1_US; k <= ACT_FILE_DAY2_US; k++) {
					AoActSortRegAction(main_work->act[k]);
				}
			}
			else {	//if (GsEnvGetRegion() == GSD_REGION_EU) {
				for (u32 k = ACT_FILE_YEAR1_EU; k <= ACT_FILE_DAY2_EU; k++) {
					AoActSortRegAction(main_work->act[k]);
				}
			}
			
			for (u32 k = ACT_FILE_TEX_ALPHA1; k <= ACT_FILE_TEX_ALPHA10; k++) {
				AoActSortRegAction(main_work->act[k]);
			}
			
			// 所持しているカオスエメラルド設定
			// ※ここでカオスエメラルドに関してはアクション登録する
			for (u32 k = ACT_FILE_ICON_EMER1; k <= ACT_FILE_ICON_EMER7; k++) {
				// 表示させるエメラルド番号が持っている個数より小さい場合、表示設定
				if ((k - ACT_FILE_ICON_EMER1) < main_work->has_emerald[i]) {
					AoActSortRegAction(main_work->act[k]);
				}
			}
			
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			
			AoActSortRegAction(main_work->act[ACT_FILE_TEX_NAME]);
		}
		else {
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			
			AoActSortRegAction(main_work->act[ACT_FILE_TEX_NEWGAME]);
		}
		
		// FOCUS項目設定
		if (main_work->cur_file == i) {
			// 各ID別の設定
			AoActSetFrame(main_work->act[ACT_FILE_TAB1], 0.f);
		}
		// 非選択項目設定
		else {
			// 非選択項目の上に被せる半透明テーブル
			for (u32 j = ACT_FILE_TAB3_B; j <= ACT_FILE_TAB3_C; j++) {
				AoActSortRegAction(main_work->act[j]);
			}
			AoActSetFrame(main_work->act[ACT_FILE_TAB1], 1.f);
		}
		
		// ファイル番号表示設定切り替え
		AoActSetFrame(main_work->act[ACT_FILE_TAB_NUM], (f32)i);
		
		// 最後にプレイしたACTをスクリーンに表示設定
		tmp_disp_clear_scr = (f32)main_work->last_clear_act[i];
		
		AoActSetFrame(main_work->act[ACT_FILE_SCR], tmp_disp_clear_scr);
		
#if _WII
		// セーブ日時の設定
		OSTicksToCalendarTime(main_work->save_time[i], &td);
		
		// 取得した日時データを各桁のフレームデータに変換
		dmFileSlctSetNumDigitFrame(tmp_year, td.year, 4);
		dmFileSlctSetNumDigitFrame(tmp_month, td.mon + 1, 2);
		dmFileSlctSetNumDigitFrame(tmp_day, td.mday, 2);
		dmFileSlctSetNumDigitFrame(tmp_hour, td.hour, 2);
		dmFileSlctSetNumDigitFrame(tmp_min, td.min, 2);
		
		
		if (GsEnvGetRegion() == GSD_REGION_JP) {
			tmp_time_act[0] = ACT_FILE_YEAR1;
			tmp_time_act[1] = ACT_FILE_MON1;
			tmp_time_act[2] = ACT_FILE_DAY1;
		}
		else if (GsEnvGetRegion() == GSD_REGION_US) {
			tmp_time_act[0] = ACT_FILE_YEAR1_US;
			tmp_time_act[1] = ACT_FILE_MON1_US;
			tmp_time_act[2] = ACT_FILE_DAY1_US;
		}
		else {	//if (GsEnvGetRegion() == GSD_REGION_EU) {
			tmp_time_act[0] = ACT_FILE_YEAR1_EU;
			tmp_time_act[1] = ACT_FILE_MON1_EU;
			tmp_time_act[2] = ACT_FILE_DAY1_EU;
		}
		
		
		// 日付数字設定
		for (u32 j = 0; j < 4; j++) {
			AoActSetFrame(main_work->act[tmp_time_act[0] + j], (f32)tmp_year[j]);
		}
		for (u32 j = 0; j < 2; j++) {
			AoActSetFrame(main_work->act[tmp_time_act[1] + j], (f32)tmp_month[j]);
		}
		for (u32 j = 0; j < 2; j++) {
			AoActSetFrame(main_work->act[tmp_time_act[2] + j], (f32)tmp_day[j]);
		}
		for (u32 j = 0; j < 2; j++) {
			AoActSetFrame(main_work->act[ACT_FILE_TIME_H_1 + j], (f32)tmp_hour[j]);
		}
		for (u32 j = 0; j < 2; j++) {
			AoActSetFrame(main_work->act[ACT_FILE_TIME_M_1 + j], (f32)tmp_min[j]);
		}
		
		AoActSetFrame(main_work->act[ACT_FILE_SLASH1], (f32)0.f);
		AoActSetFrame(main_work->act[ACT_FILE_SLASH2], (f32)0.f);
		
		// 時間数字設定
		AoActSetFrame(main_work->act[ACT_FILE_TIME_COLON], (f32)0.f);
		
		// 名前表示設定
		for (u32 j = 0; j < 10; j++) {
			AoActSetFrame(main_work->act[ACT_FILE_TEX_ALPHA1 + j]
						  , (f32)(main_work->user_name[i][j] + 1));
		}
#endif	// #if _WII
		
		
		
		// ACCUMURATEでトランスさせる
		AoActAcmPush();
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
		
		for (u32 k = ACT_TAB_ALL_SRC; k <= ACT_TAB_ALL_DST; k++) {
			if (k >= ACT_FILE_TEX_ALPHA1 && k <= ACT_FILE_TEX_ALPHA10) {
				tmp_pos_x = DMD_FILESLCT_FILE_TABLE_TOP_POS_X
							+ (k - ACT_FILE_TEX_ALPHA1) * DMD_FILESLCT_ALPHA_ONE_DIST;
			}
			else {
				tmp_pos_x = DMD_FILESLCT_FILE_TABLE_TOP_POS_X;
			}
			
			AoActAcmInit();
			AoActAcmApplyTrans(tmp_pos_x
							   , main_work->file_tab_pos_y[i]
							   		+ i * DMD_FILESLCT_FILE_TABLE_DIST_Y
							   		+ DMD_FILESLCT_FILE_TABLE_TOP_POS_Y
							   , 0
							   );
			
			// フレーム更新はSetFrameのみで行う
			AoActUpdate(main_work->act[k], 0.0f);
		}
		
		for (u32 k = ACT_FILE_YEAR1; k <= ACT_FILE_DAY2_EU; k++) {
			AoActAcmInit();
			AoActAcmApplyTrans(DMD_FILESLCT_FILE_TABLE_TOP_POS_X
							   , main_work->file_tab_pos_y[i]
							   		+ i * DMD_FILESLCT_FILE_TABLE_DIST_Y
							   		+ DMD_FILESLCT_FILE_TABLE_TOP_POS_Y
							   , 0
							   );
			
			// フレーム更新はSetFrameのみで行う
			AoActUpdate(main_work->act[k], 0.0f);
		}
		
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		
		for (u32 k = ACT_FILE_TEX_NAME; k <= ACT_FILE_TEX_NEWGAME; k++) {
			tmp_pos_x = DMD_FILESLCT_FILE_TABLE_TOP_POS_X;
			
			AoActAcmInit();
			AoActAcmApplyTrans(tmp_pos_x
							   , main_work->file_tab_pos_y[i]
							   		+ i * DMD_FILESLCT_FILE_TABLE_DIST_Y
							   		+ DMD_FILESLCT_FILE_TABLE_TOP_POS_Y
							   , 0
							   );
			
			// フレーム更新はSetFrameのみで行う
			AoActUpdate(main_work->act[k], 0.0f);
		}
		
		AoActAcmPop();
		
		
		// ソート実行
		AoActSortExecute();
		
		// ソートバッファ描画
		AoActSortDraw();
		
		// ソートバッファ全解除
		AoActSortUnregAll();
	}

	// カーソル描画設定
	if (main_work->disp_flag & DMD_FILESLCT_DISP_FLAG_ACT_CRSR) {
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
		
		for (u32 i = ACT_FILE_TAB4_B; i <= ACT_FILE_TAB4_C; i++) {
			AoActSortRegAction(main_work->act[i]);
			
			// ACCUMURATEでトランスさせる
			AoActAcmPush();
			
			AoActAcmInit();
			AoActAcmApplyTrans(DMD_FILESLCT_FILE_TABLE_TOP_POS_X
							   , main_work->crsr_pos_y
							   , 0
							   );

			// フレーム更新はSetFrameのみで行う
			AoActUpdate(main_work->act[i], 1.0f);

			AoActAcmPop();
		}
	}
	
	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();

}




// ==========================================================================
// dmFileSlctWinSelectDraw
/*!
	ウインドウ用描画設定処理
 */
// ==========================================================================
void dmFileSlctWinSelectDraw(DMS_FILESLCT_MAIN_WORK *main_work)
{
	f32 tmp_win_size[2] = {0.f, 0.f};
	u32 i = 0;
	
	// ウインドウ用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_FILESLCT_DRAW_PRIO_WIN_FIX);
	
#if _WII
	tmp_win_size[0] = DMD_FILESLCT_WINDOW_SIZE_W * DMD_FILESLCT_DISP_SCALE_TEXT;
	tmp_win_size[1] = DMD_FILESLCT_WINDOW_SIZE_H * DMD_FILESLCT_DISP_SCALE_TEXT;
#else
	tmp_win_size[0] = DMD_FILESLCT_WINDOW_SIZE_W;
	tmp_win_size[1] = DMD_FILESLCT_WINDOW_SIZE_H;
#endif
	
	// ウインドウ描画
	AoWinSysDrawState(AOD_WIN_TYPE_A
					 , AoTexGetTexList(&main_work->cmn_tex[2])
					 , 0
					 , DMD_FILESLCT_SIZE_HALF_WIDTH			// ウインドウ中心X
					 , DMD_FILESLCT_SIZE_HALF_HEIGHT		// ウインドウ中心Y
					 , tmp_win_size[0] * main_work->win_size_rate[0]			// ウインドウ横サイズ
					 , tmp_win_size[1] * main_work->win_size_rate[1]			// ウインドウ縦サイズ
					 , dm_fileslct_draw_state				// 描画STATE
					 , 0.f
					 );
	
	// ウインドウ内の項目描画
	if (main_work->disp_flag & DMD_FILESLCT_DISP_FLAG_WIN_ACT) {		// ウインドウが表示しきっているならば
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[2]));
//		AoActSortRegAction(main_work->act[ACT_WIN_LINE]);
		
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
//		AoActSortRegAction(main_work->act[ACT_TEX_WINTITLE]);
		
		
		// ファイル削除確認メッセージの場合
		if (main_work->announce_flag & (1 << DME_FILESLCT_WIN_IS_DEL_FILE)) {
			AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[0]));
			AoActSortRegAction(main_work->act[ACT_BTN_CANCEL]);
			
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			AoActSortRegAction(main_work->act[ACT_WIN_TEX_MSG]);
			
			AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
			AoActSortRegAction(main_work->act[ACT_TEX_YES]);
			AoActSortRegAction(main_work->act[ACT_TEX_NO]);
			AoActSortRegAction(main_work->act[ACT_TEX_BACK]);
			
			AoActSetFrame(main_work->act[ACT_TEX_YES]
						  , dm_fileslct_win_disp_slct_frm_tbl[main_work->win_cur_slct][0]);
			AoActSetFrame(main_work->act[ACT_TEX_NO]
						  , dm_fileslct_win_disp_slct_frm_tbl[main_work->win_cur_slct][1]);
			
			AoActSetFrame(main_work->act[ACT_WIN_TEX_MSG], 0.f);
		}
		
		// ファイル削除中メッセージの場合
		else if (main_work->announce_flag & (1 << DME_FILESLCT_WIN_DELETING_FILE)) {
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			AoActSortRegAction(main_work->act[ACT_WIN_TEX_MSG]);
			
			AoActSetFrame(main_work->act[ACT_WIN_TEX_MSG], 1.f);
		}
		
		if (main_work->is_jp_region) {
			AoActSetFrame(main_work->act[ACT_BTN_CANCEL], (f32)0.f);
		}
		else {
			AoActSetFrame(main_work->act[ACT_BTN_CANCEL], (f32)1.f);
		}
		
		
		// ACCUMURATEでトランスさせる
		AoActAcmPush();
		
		// 以下、ウインドウメッセージ
		AoActAcmInit();
		AoActAcmApplyTrans(dm_fileslct_win_act_pos_tbl[6][0]
						   , dm_fileslct_win_act_pos_tbl[6][1]
						   , 0
						   );
		
		
#if _WII
		AoActAcmApplyScale(DMD_FILESLCT_DISP_SCALE_TEXT
						   , DMD_FILESLCT_DISP_SCALE_TEXT);
#endif
		
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		AoActUpdate(main_work->act[ACT_WIN_TEX_MSG], 0.0f);
		
		// 以下、ウインドウライン
		AoActAcmInit();
		AoActAcmApplyTrans(dm_fileslct_win_act_pos_tbl[0][0]
						   , dm_fileslct_win_act_pos_tbl[0][1]
						   , 0
						   );
		
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[2]));
		AoActUpdate(main_work->act[ACT_WIN_LINE], 0.0f);
		
		// 以下、キャンセルボタン
		AoActAcmInit();
		AoActAcmApplyTrans(dm_fileslct_win_act_pos_tbl[1][0]
						   , dm_fileslct_win_act_pos_tbl[1][1]
						   , 0
						   );
		AoActAcmApplyTrans(dm_fileslct_back_text_length_tbl[GsEnvGetLanguage()]
						   , 0
						   , 0
						   );
		
#if _WII		// Wii版のみ左へ10ピクセルさらにずらせる
		AoActAcmApplyTrans(-10.f, 0, 0);
#endif
		
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[0]));
		AoActUpdate(main_work->act[ACT_BTN_CANCEL], 0.0f);
		
		// 以下、ウインドウテキスト関連
		for (i = 0; i < 4; i++) {
			AoActAcmInit();
			AoActAcmApplyTrans(dm_fileslct_win_act_pos_tbl[i + 2][0]
							   , dm_fileslct_win_act_pos_tbl[i + 2][1]
							   , 0
							   );
			
#if _WII
			if (i != ACT_TEX_BACK - ACT_TEX_WINTITLE) {
				AoActAcmApplyScale(DMD_FILESLCT_DISP_SCALE_TEXT
								   , DMD_FILESLCT_DISP_SCALE_TEXT);
			}
#endif
			
			AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
			
			// フレーム更新はSetFrameのみで行う
			AoActUpdate(main_work->act[ACT_TEX_WINTITLE + i], 0.0f);
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
// dmFileSlctSetObiEfctPos
/*!
	帯アクションの演出用座標設定処理
 */
// ==========================================================================
void dmFileSlctSetObiEfctPos(DMS_FILESLCT_MAIN_WORK *main_work)
{
	for (u32 i = 0; i < 2; i++) {
		
		// 一定位置に来たら戻る
		if (main_work->obi_tex_pos[i] < DMD_FILESLCT_OBI_MOVE_END_POS) {
			main_work->obi_tex_pos[i] = DMD_FILESLCT_OBI_MOVE_START_POS;
		}
		
		// 移動分座標加算
		main_work->obi_tex_pos[i] += DMD_FILESLCT_OBI_MOVE_SPEED;
	}
}



// ==========================================================================
// dmFileSlctSetFileTabInEfct
/*!
	ファイルテーブル入り演出用座標設定処理

  	ここでは基準点(一つ目のファイル)となる座標のみを移動させて
  	それ以外は一定の距離で表示位置をずらすようにする
 */
// ==========================================================================
void dmFileSlctSetFileTabInEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	float move_dist = 0;
	float distance = 0;

	// ファイルテーブル移動分
	for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
		distance = main_work->file_move_pos_dst[i] - main_work->file_move_pos_src[i];
		
		move_dist = distance / DMD_FILESLCT_IN_EFCT_TIME;
		
		main_work->file_tab_pos_y[i] += move_dist;
	}

}



// ==========================================================================
// dmFileSlctSetObiInEfct
/*!
	ファイルテーブル入り演出用座標設定処理

  	ここでは基準点(一つ目のファイル)となる座標のみを移動させて
  	それ以外は一定の距離で表示位置をずらすようにする
 */
// ==========================================================================
void dmFileSlctSetObiInEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	float move_dist = 0;
	float distance = 0;

	// 帯移動分
	distance = DMD_FILESLCT_OBI_DISP_POS_Y - DMD_FILESLCT_OBI_NODISP_POS_Y;
	
	move_dist = distance / DMD_FILESLCT_OBI_EFCT_TIME;
	
	main_work->obi_pos_y += move_dist;
	
}



// ==========================================================================
// dmFileSlctIsFileTabInEfctEnd
/*!
	ファイルテーブル入り演出終了チェック処理
 */
// ==========================================================================
BOOL dmFileSlctIsFileTabInEfctEnd(DMS_FILESLCT_MAIN_WORK *main_work)
{
	if (main_work->timer >= DMD_FILESLCT_IN_EFCT_TIME) {
//	if (main_work->file_tab_pos_y > main_work->file_move_pos_dst) {

		for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
			main_work->file_tab_pos_y[i] = main_work->file_move_pos_dst[i];
		}

		return TRUE;
	}

	return FALSE;
}



// ==========================================================================
// dmFileSlctIsObiInEfctEnd
/*!
	帯入り演出終了チェック処理
 */
// ==========================================================================
BOOL dmFileSlctIsObiInEfctEnd(DMS_FILESLCT_MAIN_WORK *main_work)
{
	if (main_work->timer >= DMD_FILESLCT_OBI_EFCT_TIME) {

		main_work->obi_pos_y = DMD_FILESLCT_OBI_DISP_POS_Y;

		return TRUE;
	}

	return FALSE;
}




// ==========================================================================
// dmFileSlctSetOutEfctData
/*!
	掃け演出用のデータ設定処理
 */
// ==========================================================================
void dmFileSlctSetOutEfctData(DMS_FILESLCT_MAIN_WORK *main_work)
{
	
	// このタイミングで上下の矢印とカーソル、帯内のFIXを表示
	main_work->disp_flag &= ~DMD_FILESLCT_DISP_FLAG_TB_ARROW;
	main_work->disp_flag &= ~DMD_FILESLCT_DISP_FLAG_ACT_CRSR;
	main_work->disp_flag &= ~DMD_FILESLCT_DISP_FLAG_OBI_TEX;
	
	// ここで初期表示位置設定
	for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
//		main_work->file_tab_pos_y[i] = DMD_FILESLCT_IN_EFCT_START_POS;
		main_work->file_move_pos_src[i] = main_work->file_tab_pos_y[i];		// 仮
		main_work->file_move_pos_dst[i] = DMD_FILESLCT_OUT_EFCT_END_POS;
	}
}



// ==========================================================================
// dmFileSlctSetFileTabOutEfct
/*!
	ファイルテーブル掃け演出用座標設定処理

  	ここでは基準点(一つ目のファイル)となる座標のみを移動させて
  	それ以外は一定の距離で表示位置をずらすようにする
 */
// ==========================================================================
void dmFileSlctSetFileTabOutEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	float move_dist = 0;
	float distance = 0;

	// ファイルテーブル移動分
	for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
		distance = main_work->file_move_pos_dst[i] - main_work->file_move_pos_src[i];
		
		move_dist = distance / DMD_FILESLCT_OUT_EFCT_TIME;
		
		main_work->file_tab_pos_y[i] += move_dist;
	}

}



// ==========================================================================
// dmFileSlctSetDecideEfctData
/*!
	決定演出用のデータ設定処理
	移動先の設定などをここで行う
 */
// ==========================================================================
void dmFileSlctSetDecideEfctData(DMS_FILESLCT_MAIN_WORK *main_work)
{
	
	// このタイミングで上下の矢印とカーソル、帯内のFIXを非表示
	main_work->disp_flag &= ~DMD_FILESLCT_DISP_FLAG_TB_ARROW;
	main_work->disp_flag &= ~DMD_FILESLCT_DISP_FLAG_ACT_CRSR;
	main_work->disp_flag &= ~DMD_FILESLCT_DISP_FLAG_OBI_TEX;
	
	// このタイミングで表示位置を取得
	for (s32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
		main_work->file_back_pos[i] = main_work->file_tab_pos_y[i];
	}
	main_work->obi_back_pos_y = main_work->obi_pos_y;
	
	// ここで初期表示位置設定
	for (s32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
		if (i < main_work->cur_file) {
			main_work->file_move_pos_src[i] = main_work->file_tab_pos_y[i];		// 仮
			main_work->file_move_pos_dst[i] = DMD_FILESLCT_DECIDE_EFCT_END_UP_POS;
		}
		else if (i >= main_work->cur_file) {
			main_work->file_move_pos_src[i] = main_work->file_tab_pos_y[i];		// 仮
			main_work->file_move_pos_dst[i] = DMD_FILESLCT_DECIDE_EFCT_END_DOWN_POS;
		}
		else {
			// 決定項目は遅らせる
		}
	}
}



// ==========================================================================
// dmFileSlctSetFileTabDecideEfct
/*!
	ファイルテーブル決定演出用座標設定処理

  	ここでは基準点(一つ目のファイル)となる座標のみを移動させて
  	それ以外は一定の距離で表示位置をずらすようにする
 */
// ==========================================================================
void dmFileSlctSetFileTabDecideEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	float move_dist = 0;
	float distance = 0;

	// ファイルテーブル移動分
	for (int i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
		if (i != main_work->cur_file) {
			distance = main_work->file_move_pos_dst[i] - main_work->file_move_pos_src[i];
			
			move_dist = distance / DMD_FILESLCT_DECIDE_EFCT_TIME;
			
			main_work->file_tab_pos_y[i] += move_dist;
		}
	}
}



// ==========================================================================
// dmFileSlctSetFileTabBackEfct
/*!
	ファイルテーブル決定演出から戻る場合の座標設定処理

  	ここでは基準点(一つ目のファイル)となる座標のみを移動させて
  	それ以外は一定の距離で表示位置をずらすようにする
 */
// ==========================================================================
void dmFileSlctSetFileTabBackEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	float move_dist = 0;
	float distance = 0;

	// ファイルテーブル移動分
	for (int i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
		distance = main_work->file_move_pos_dst[i] - main_work->file_move_pos_src[i];
		
		move_dist = distance / DMD_FILESLCT_DECIDE_EFCT_TIME;
		
		main_work->file_tab_pos_y[i] += move_dist;
	}
	
	distance = main_work->obi_back_pos_y - main_work->obi_pos_y;
	
	move_dist = distance / DMD_FILESLCT_DECIDE_EFCT_TIME;
	
	main_work->obi_pos_y += move_dist;
	
}



// ==========================================================================
// dmFileSlctSetFileCurTabDecideEfct
/*!
	決定項目のファイルテーブル決定演出用座標設定処理

  	ここでは基準点(一つ目のファイル)となる座標のみを移動させて
  	それ以外は一定の距離で表示位置をずらすようにする
 */
// ==========================================================================
void dmFileSlctSetFileCurTabDecideEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	float move_dist = 0;
	float distance = 0;

	// ファイルテーブル移動分
	distance = main_work->file_move_pos_dst[main_work->cur_file]
				- main_work->file_move_pos_src[main_work->cur_file];
	
	move_dist = distance / DMD_FILESLCT_DECIDE_EFCT_TIME;
	
	main_work->file_tab_pos_y[main_work->cur_file] += move_dist;
	
	// 帯掃け演出処理
	if (!dmFileSlctIsObiOutEfctEnd(main_work)
		&& main_work->is_save_exist & (1 << main_work->cur_file)) {
		dmFileSlctSetObiOutEfct(main_work);
	}
}



// ==========================================================================
// dmFileSlctSetObiOutEfct
/*!
	ファイルテーブル掃け演出用座標設定処理

  	ここでは基準点(一つ目のファイル)となる座標のみを移動させて
  	それ以外は一定の距離で表示位置をずらすようにする
 */
// ==========================================================================
void dmFileSlctSetObiOutEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	float move_dist = 0;
	float distance = 0;

	// 帯移動分
	distance = DMD_FILESLCT_OBI_NODISP_POS_Y - DMD_FILESLCT_OBI_DISP_POS_Y;
	
	move_dist = distance / DMD_FILESLCT_OBI_EFCT_TIME;
	
	main_work->obi_pos_y += move_dist;
	
}



// ==========================================================================
// dmFileSlctIsFileTabOutEfctEnd
/*!
	ファイルテーブル掃け演出終了チェック処理
 */
// ==========================================================================
BOOL dmFileSlctIsFileTabOutEfctEnd(DMS_FILESLCT_MAIN_WORK *main_work)
{
	if (main_work->timer >= DMD_FILESLCT_OUT_EFCT_TIME) {
//	if (main_work->file_tab_pos_y > main_work->file_move_pos_dst) {

		for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
			main_work->file_tab_pos_y[i] = main_work->file_move_pos_dst[i];
		}

		return TRUE;
	}

	return FALSE;
}



// ==========================================================================
// dmFileSlctIsFileTabDecideEfctEnd
/*!
	ファイルテーブル決定演出終了チェック処理
 */
// ==========================================================================
BOOL dmFileSlctIsFileTabDecideEfctEnd(DMS_FILESLCT_MAIN_WORK *main_work)
{
	if (main_work->timer >= DMD_FILESLCT_DECIDE_EFCT_TIME * 2 + DMD_FILESLCT_DECIDE_EFCT_WAIT_TIME) {
//	if (main_work->file_tab_pos_y > main_work->file_move_pos_dst) {

		for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
			main_work->file_tab_pos_y[i] = main_work->file_move_pos_dst[i];
		}

		return TRUE;
	}

	return FALSE;
}



// ==========================================================================
// dmFileSlctIsFileTabBackEfctEnd
/*!
	ファイルテーブル決定演出後のキャンセル移動終了チェック処理
 */
// ==========================================================================
BOOL dmFileSlctIsFileTabBackEfctEnd(DMS_FILESLCT_MAIN_WORK *main_work)
{
	if (main_work->timer >= DMD_FILESLCT_DECIDE_EFCT_TIME) {
//	if (main_work->file_tab_pos_y > main_work->file_move_pos_dst) {

		for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
			main_work->file_tab_pos_y[i] = main_work->file_back_pos[i];
		}
		
		main_work->obi_pos_y = main_work->obi_back_pos_y;
		
		main_work->timer = 0;

		return TRUE;
	}

	return FALSE;
}



// ==========================================================================
// dmFileSlctIsObiOutEfctEnd
/*!
	帯掃け演出終了チェック処理
 */
// ==========================================================================
BOOL dmFileSlctIsObiOutEfctEnd(DMS_FILESLCT_MAIN_WORK *main_work)
{
	if (main_work->obi_pos_y >= DMD_FILESLCT_OBI_NODISP_POS_Y) {
//	if (main_work->timer >= DMD_FILESLCT_OBI_EFCT_TIME) {

		main_work->obi_pos_y = DMD_FILESLCT_OBI_NODISP_POS_Y;

		return TRUE;
	}

	return FALSE;
}




////////////////////// FOCUS切り替え関連 /////////////////////////////////////

// ==========================================================================
// dmFileSlctSetFocusChangeEfctData
/*!
	セーブファイル選択時の縦のACT切り替え時の設定処理
 */
// ==========================================================================
void dmFileSlctSetFocusChangeEfctData(DMS_FILESLCT_MAIN_WORK *main_work)
{
	// 上移動・下移動共に必要な変数の設定
	s32 chng_sign = 0;
	s32 disp_act_no = 0; 
	
	if (main_work->flag & DMD_FILESLCT_FLAG_UP_CHNG_CRSR) {
		chng_sign = -1;
	}
	else if (main_work->flag & DMD_FILESLCT_FLAG_DOWN_CHNG_CRSR) {
		chng_sign = 1;
	}
	else {
		MTM_ASSERT(0);
	}
	
	// 選択ファイル切り替え
	main_work->prev_file = main_work->cur_file;
	
	if (main_work->flag & DMD_FILESLCT_FLAG_CHNG_CRSR) {
		
		main_work->crsr_prev_idx = main_work->crsr_idx;
		
		disp_act_no = main_work->focus_disp_no;
		
		main_work->crsr_idx = dmFileSlctGetRevisedFileCrsrNo(main_work->crsr_idx
															 , chng_sign
															 , disp_act_no
															 );
		
		main_work->crsr_move_src = dm_fileslct_act_crsr_disp_y_pos_tbl[main_work->crsr_prev_idx];
		main_work->crsr_move_dst = dm_fileslct_act_crsr_disp_y_pos_tbl[main_work->crsr_idx];
		
		main_work->flag &= ~DMD_FILESLCT_FLAG_CHNG_CRSR;
		main_work->flag |= DMD_FILESLCT_FLAG_MOVE_CRSR;
	}
	
	
	if (main_work->flag & DMD_FILESLCT_FLAG_CHNG_VRTCL) {
		
		main_work->prev_disp_no = main_work->focus_disp_no;
		main_work->focus_disp_no = dmFileSlctGetRevisedFileVrtclNo(main_work->focus_disp_no
																   , chng_sign
																   );
		
		for (int i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
			main_work->file_move_pos_src[i] = main_work->file_tab_pos_y[i];
			main_work->file_move_pos_dst[i] = dm_fileslct_file_disp_y_pos_tbl[main_work->focus_disp_no];
		}
		
		main_work->flag &= ~DMD_FILESLCT_FLAG_CHNG_VRTCL;
		main_work->flag |= DMD_FILESLCT_FLAG_MOVE_VRTCL;
	}

	main_work->cur_file = main_work->crsr_idx + main_work->focus_disp_no;
	
}



// ==========================================================================
// dmFileSlctSetFileVrtclChangeEfct
/*!
	セーブファイル選択時の縦のACT切り替え時演出中処理
 */
// ==========================================================================
void dmFileSlctSetFileVrtclChangeEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	float move_dist[2] = {0, 0};
	float distance[2] = {0, 0};
//	float move_src = 0;
//	float move_dest = 0;

	// 縦移動する場合
	for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
		
		distance[1] = main_work->file_move_pos_dst[i] - main_work->file_move_pos_src[i];
		
		move_dist[1] = distance[1] / DMD_FILESLCT_FOCUS_FILE_CHNG_TIME;
		
		main_work->file_tab_pos_y[i] += move_dist[1];
	}
}



// ==========================================================================
// dmFileSlctIsFileVrtclChangeEfct
/*!
	セーブファイル選択時の縦のACT切り替え時演出中処理
 */
// ==========================================================================
BOOL dmFileSlctIsFileVrtclChangeEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	float move_direct = 0.f;
	
	move_direct = main_work->file_move_pos_dst[0] - main_work->file_move_pos_src[0];
	
	// 掃け演出終了チェック
	if (move_direct >= 0 && main_work->file_tab_pos_y[0] >= main_work->file_move_pos_dst[0]) {
//	if (main_work->timer >= DMD_FILESLCT_CRSR_MOVE_TIME) {
		for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
			main_work->file_tab_pos_y[i] = main_work->file_move_pos_dst[i];
		}
		
		return TRUE;
	}
	else if (move_direct <= 0 && main_work->file_tab_pos_y[0] <= main_work->file_move_pos_dst[0]) {
		for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
			main_work->file_tab_pos_y[i] = main_work->file_move_pos_dst[i];
		}
		
		return TRUE;
	}
	
#if 0
	// 掃け演出終了チェック
	if (main_work->timer >= DMD_FILESLCT_FOCUS_FILE_CHNG_TIME) {

		for (u32 i = 0; i < DMD_FILESLCT_SAVE_FILE_NUM; i++) {
			main_work->file_tab_pos_y[i] = main_work->file_move_pos_dst[i];
		}
		
		return TRUE;
	}
#endif

	return FALSE;
}



// ==========================================================================
// dmFileSlctSetStageCrsrChangeEfct
/*!
	セーブファイル選択時の縦のACT切り替え時演出中処理
 */
// ==========================================================================
void dmFileSlctSetStageCrsrChangeEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	float move_dist = 0;
	float distance = 0;

	// 縦移動する場合
	distance = main_work->crsr_move_dst - main_work->crsr_move_src;
	
	move_dist = distance / DMD_FILESLCT_CRSR_MOVE_TIME;
	
	main_work->crsr_pos_y += move_dist;
}



// ==========================================================================
// dmFileSlctIsStageCrsrChangeEfct
/*!
	セーブファイル選択時の縦のACT切り替え時演出中処理
 */
// ==========================================================================
BOOL dmFileSlctIsStageCrsrChangeEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	float move_direct = 0.f;
	
	move_direct = main_work->crsr_move_dst - main_work->crsr_move_src;
	
	// 掃け演出終了チェック
	if (move_direct >= 0 && main_work->crsr_pos_y >= main_work->crsr_move_dst) {
//	if (main_work->timer >= DMD_FILESLCT_CRSR_MOVE_TIME) {
		main_work->crsr_pos_y = main_work->crsr_move_dst;
		
		return TRUE;
	}
	else if (move_direct <= 0 && main_work->crsr_pos_y <= main_work->crsr_move_dst) {
		main_work->crsr_pos_y = main_work->crsr_move_dst;
		
		return TRUE;
	}

	return FALSE;
}





// ==========================================================================
// dmFileSlctSetWinOpenEfct
/*!
	ウインドウ入り演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmFileSlctSetWinOpenEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	if (main_work->win_timer > DMD_FILESLCT_WIN_EFCT_TIME) {
		// ウインドウ演出終了
		main_work->flag |= DMD_FILESLCT_FLAG_WIN_EFCT_END;

		main_work->win_timer = 0.0f;

		for (u32 i = 0; i < 2; i++) {
			main_work->win_size_rate[i] = DMD_FILESLCT_WIN_DEF_RATE;
		}
	}
	else {
		// タイマー更新(但しフレームレートが可変になるように修正すること)	◆
		main_work->win_timer++;
	}
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->win_size_rate[i] = main_work->win_timer / DMD_FILESLCT_WIN_EFCT_TIME;
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
// dmFileSlctSetWinCloseEfct
/*!
	ウインドウ閉め演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmFileSlctSetWinCloseEfct(DMS_FILESLCT_MAIN_WORK *main_work)
{
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->win_size_rate[i] = main_work->win_timer / DMD_FILESLCT_WIN_EFCT_TIME;
		}
		else {
			main_work->win_size_rate[i] = 0.0f;
		}
	}

	if (main_work->win_timer < 0.0f) {
		// ウインドウ演出終了
		main_work->flag |= DMD_FILESLCT_FLAG_WIN_EFCT_END;

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
// dmFileSlctDeleteSaveFile
/*!
	セーブファイル削除処理
 */
// ==========================================================================
void dmFileSlctDeleteSaveFile(DMS_FILESLCT_MAIN_WORK *main_work)
{
	UNREFERENCED_PARAMETER(main_work);
	
	// ここでセーブファイル削除処理を行う
	gs::backup::SBackup &backup = gs::backup::SBackup::CreateInstance();
	
	// 選択中ファイルの各データを取得
	gs::backup::SSystem &sys_data = backup.GetSystem((u32)main_work->cur_file);
	gs::backup::SOption &opt_data = backup.GetOption((u32)main_work->cur_file);
	gs::backup::SStage &stage_data = backup.GetStage((u32)main_work->cur_file);
	gs::backup::SSpecial &spe_data = backup.GetSpecial((u32)main_work->cur_file);
	
	// 選択中ファイルの各データを初期化(データ削除)
	sys_data.Init();
	opt_data.Init();
	stage_data.Init();
	spe_data.Init();
	
	// 初期化したことをセーブする必要があるため、セーブ
	DmCmnBackupSave(FALSE, FALSE, TRUE);
}



// ==========================================================================
// dmFileSlctIsDeleteSaveFile
/*!
	セーブファイル削除チェック処理
 */
// ==========================================================================
BOOL dmFileSlctIsDeleteSaveFile(DMS_FILESLCT_MAIN_WORK *main_work)
{
	main_work->del_timer++;
	
	// ここでセーブファイル削除チェック処理を行う
	if (DmCmnBackupIsSaveFinished()
		&& main_work->del_timer > DMD_FILESLCT_DEL_DATA_WIN_DISP_TIME) {
		main_work->is_save_exist &= ~(1 << main_work->cur_file);
		
		main_work->del_timer = 0;
		
		return TRUE;
	}
	
	
	return FALSE;
}


// ==========================================================================
// dmFileSlctSetMsgWinState
/*!
	メッセージウインドウの開閉状態設定処理
 */
// ==========================================================================
BOOL dmFileSlctSetMsgWinState(DMS_FILESLCT_MAIN_WORK *main_work)
{
	BOOL result = FALSE;
	
	if (main_work->proc_win_update == dmFileSlctProcWindowOpenEfct
		|| main_work->proc_win_update == dmFileSlctProcWindowAnnounceIdle
		|| main_work->proc_win_update == dmFileSlctProcWindowCloseEfct) {
		result = TRUE;
	}
	else if (main_work->announce_flag) {
		result = TRUE;
	}
	else if (DmUserNameIsMsgWinOpen()) {
		result = TRUE;
	}
	
	else if (main_work->proc_menu_update == dmFileSlctProcFileTabInEfct
			 || main_work->proc_menu_update == dmFileSlctProcFileTabOutEfct
			 || main_work->proc_menu_update == dmFileSlctProcFileTabDecideEfct
			 || main_work->proc_menu_update == dmFileSlctProcUserNameSaveWait
			 || main_work->proc_menu_update == dmFileSlctProcCancelUserName
			 || main_work->proc_menu_update == dmFileSlctProcFadeOut) {
		result = TRUE;
	}
	
	else {
		result = FALSE;
	}
	
	return result;
}



// ==========================================================================
// dmFileSlctIsTexLoad
/*!
	テクスチャ構築完了チェック処理
 */
// ==========================================================================
s32 dmFileSlctIsTexLoad(void)
{
	for (int i = 0; i < DME_FILESLCT_DATA_TYPE_MAX; i++) {
		if (!AoTexIsLoaded(&dm_fileslct_tex[i])) {
			// フラグ扱いでON
			return 0;
		}
	}
	
	for (int i = 0; i < 4; i++) {
		if (!AoTexIsLoaded(&dm_fileslct_cmn_tex[i])) {
			// フラグ扱いでON
			return 0;
		}
	}
	
	return 1;
}


// ==========================================================================
// dmFileSlctIsTexRelease
/*!
	テクスチャ解放完了チェック処理
 */
// ==========================================================================
s32 dmFileSlctIsTexRelease(void)
{
	for (int i = 0; i < DME_FILESLCT_DATA_TYPE_MAX; i++) {
		if (!AoTexIsReleased(&dm_fileslct_tex[i])) {
			// フラグ扱いでON
			return 0;
		}
	}
	
	for (int i = 0; i < 4; i++) {
		if (!AoTexIsReleased(&dm_fileslct_cmn_tex[i])) {
			// フラグ扱いでON
			return 0;
		}
	}
	
	return 1;
}




// ===========================================================================
//	dmFileSlctGetRevisedFileVrtclNo
/*!
	項目の場所を示す変数が最大・最小値を超えた際に補正した(回り込みさせた)値を取得する関数
	通常時のカーソル移動の選択項目に使用。

	@param id		[in] 項目番号
	@param diff		[in] 変化量(移動における)
	@return 補正された項目番号
*/
// ===========================================================================
s32 dmFileSlctGetRevisedFileVrtclNo(s32 idx, s32 diff)
{
	s32 result;
	
	result = (int)idx + diff;

	// 先頭から一つ戻ると最後に移動
	if (result < 0) {
		result = DMD_FILESLCT_FILE_POS_NUM - 1;
	}
	
	// 最後から一つ進むと先頭に移動
	if (result >= DMD_FILESLCT_FILE_POS_NUM) {
		result = 0;
	}
	
	MTM_ASSERT(result >= 0 && result < DMD_FILESLCT_FILE_POS_NUM);
	
	return result;
}



// ===========================================================================
//	dmFileSlctGetRevisedFileCrsrNo
/*!
	項目の場所を示す変数が最大・最小値を超えた際に補正した(回り込みさせた)値を取得する関数
	通常時のカーソル移動の選択項目に使用。

	@param id		[in] 項目番号
	@param diff		[in] 変化量(移動における)
	@return 補正された項目番号
*/
// ===========================================================================
s32 dmFileSlctGetRevisedFileCrsrNo(s32 idx, s32 diff, s32 disp_act_no)
{
	s32 result;
	
	result = (int)idx + diff;
	
	// 先頭から一つ戻ると最後に移動
	if (result < 0) {
		if (disp_act_no == 0) {
			result = DMD_FILESLCT_CRSR_POS_NUM - 1;
		}
		else {
			result = 0;
		}
	}
	
	// 最後から一つ進むと先頭に移動
	if (result >= DMD_FILESLCT_CRSR_POS_NUM) {
		if (disp_act_no == DMD_FILESLCT_FILE_POS_NUM - 1) {
			result = 0;
		}
		else {
			result = DMD_FILESLCT_CRSR_POS_NUM - 1;
		}
	}
	
	MTM_ASSERT(result >= 0 && result < DMD_FILESLCT_CRSR_POS_NUM);
	
	return result;
}



#if _WII
// ==========================================================================
// dmFileSlctSetNumDigitFrame
/*!
	各数字の表示描画フレーム設定処理
 */
// ==========================================================================
void dmFileSlctSetNumDigitFrame(u32 digit[], const int data, int digit_num)
{
//	int tmp_digit[digit_num];
	int tmp_digit_data = 0.f;
	int tmp_calc = 1;
	BOOL disp_zero_num = FALSE;
	
	
	tmp_digit_data = data;
	
	// スコア描画
	for (int j = digit_num - 1; j >= 0; j--) {
		for (int k = 0; k < j; k++) {
			tmp_calc = tmp_calc * 10;
		}
		
		if (tmp_digit_data >= tmp_calc) {
			digit[digit_num - 1 -j] = tmp_digit_data / tmp_calc;
			tmp_digit_data -= tmp_calc * digit[digit_num - 1 -j];
		}
		else {
			digit[digit_num - 1 -j] = 0;
		}
		
		tmp_calc = 1;
	}
}
#endif

void DmFileSlctStaticVarInit(void)
{
	memset(&dm_fileslct_mgr, 0, sizeof(dm_fileslct_mgr));
	dm_fileslct_mgr_p = NULL;
	
	memset(dm_fileslct_arc_amb,     0, sizeof(dm_fileslct_arc_amb));
	memset(dm_fileslct_ama,         0, sizeof(dm_fileslct_ama));
	memset(dm_fileslct_amb,         0, sizeof(dm_fileslct_amb));
	memset(dm_fileslct_cmn_arc_amb, 0, sizeof(dm_fileslct_cmn_arc_amb));
	memset(dm_fileslct_cmn_ama,     0, sizeof(dm_fileslct_cmn_ama));
	memset(dm_fileslct_cmn_amb,     0, sizeof(dm_fileslct_cmn_amb));
	memset(dm_fileslct_tex,         0, sizeof(dm_fileslct_tex));
	memset(dm_fileslct_cmn_tex,     0, sizeof(dm_fileslct_cmn_tex));
	dm_fileslct_is_file_decide = 0;
	
	dm_fileslct_draw_state = 0;
}


#endif // #if _WII || _PC

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
