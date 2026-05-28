// ===========================================================================
/*!
	@file	dmUserName.cpp
	@brief	デモ・ユーザー名設定画面

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: dmUserName.cpp 2 2011-04-11 05:21:26Z thamada $
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

#include "dmUserName.h"
#include "../library/ao/include/aoTexture.h"
#include "../library/ao/include/aoAction.h"

#include "gs.h"
#include "gsMainSys.h"
#include "izFade.h"
#include "aoWinSys.h"

#include "gsBackup.hpp"
#include "gsBackupOption.hpp"

#include "dmSound.h"
#include "dmCmnBackup.h"

#include "gsCharCode.h"

// データヘッダ
#include "common/ace/D_OPTION_USER.HMA"
#include "common/ace/D_OPTION_USER_JP.HMA"

#include "common/ace/D_CMN_BTN.HMA"
#include "common/ace/D_CMN_WIN.HMA"
#include "common/ace/D_CMN_MSG_JP.HMA"

// ----- Macros ------------------------------------------------（マクロ定義）

#define DMD_USERNAME_TASK_PAUSELEVEL		(0x7fff)
#define DMD_USERNAME_TASK_PRIO_MAIN			(0x2000)
#define DMD_USERNAME_TASK_GROUP_MAIN		(10)

#define DMD_USERNAME_FILE_PATH_NUM_MAX		(60)

#define DMD_USERNAME_CMN_DATA_FILENAME		(GSS_BASE_PATH"DEMO/CMN/D_CMN_WIN.AMB")
#define DMD_USERNAME_DATA_FILENAME			(GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER.AMB")

#define DMD_USERNAME_SIZE_WIDTH				(960.0f)
#define DMD_USERNAME_SIZE_HEIGHT			(720.0f)
#define DMD_USERNAME_SIZE_HALF_WIDTH		(480.0f)
#define DMD_USERNAME_SIZE_HALF_HEIGHT		(360.0f)


// プライオリティ設定
#define DMD_USERNAME_DRAW_PRIO_CHAR			(0x3000)
#define DMD_USERNAME_DRAW_PRIO_BG			(0x2800)
#define DMD_USERNAME_DRAW_PRIO_FIX			(0x2c00)
#define DMD_USERNAME_DRAW_PRIO_WIN			(0x3000)
#define DMD_USERNAME_DRAW_PRIO_NAME_CHAR	(0x4000)
#define DMD_USERNAME_DRAW_PRIO_MSG_WIN		(0x4800)
#define DMD_USERNAME_DRAW_PRIO_MSG_WIN_FIX	(0x4c00)

// 表示位置
//#define DMD_USERNAME_ACT_TABLE_		(60)
#define DMD_USERNAME_FILE_TABLE_TOP_POS_X	(170.0f)
#define DMD_USERNAME_FILE_TABLE_TOP_POS_Y	(100.0f)
#define DMD_USERNAME_FILE_TABLE_DIST_Y		(160.0f)//(160.0f)
#define DMD_USERNAME_ALPHA_ONE_DIST			(20.f)

//#define DMD_USERNAME_STAGE_TAB_DISP_POS_X	()
#define DMD_USERNAME_STAGE_TAB_NODISP_POS_X	(1120.f)

#define DMD_USERNAME_SAVE_FILE_NUM			(6)
#define DMD_USERNAME_FILE_POS_NUM			(4)
#define DMD_USERNAME_CRSR_POS_NUM			(3)

#define DMD_USERNAME_CHAR_NO_MAX			(49)
#define DMD_USERNAME_CHAR_NUM_MAX			(10)

#define DMD_USERNAME_DRAW_STATE_ID			(10)

// ウインドウ関連
#define DMD_USERNAME_WINDOW_SIZE_W			(732.f)
#define DMD_USERNAME_WINDOW_SIZE_H			(340.f)
#define DMD_USERNAME_MSG_WIN_SIZE_W			(512.f)
#define DMD_USERNAME_MSG_WIN_SIZE_H			(180.f)
#define DMD_USERNAME_WIN_DEF_RATE			(1.0f)

// フェード関連
#define DMD_USERNAME_FADEIN_TIME			(32.0f)
#define DMD_USERNAME_FADEOUT_TIME			(32.0f)

// 演出関連
#define DMD_USERNAME_FOCUS_FILE_CHNG_TIME	(16.0f)
#define DMD_USERNAME_IN_EFCT_START_POS		(720.0f)
#define DMD_USERNAME_IN_EFCT_TIME			(32.0f)
#define DMD_USERNAME_OUT_EFCT_END_POS		(720.0f)
#define DMD_USERNAME_OUT_EFCT_TIME			(32.0f)
#define DMD_USERNAME_WIN_EFCT_TIME			(12.0f)
#define DMD_USERNAME_OBI_MOVE_START_POS		(1216.f)
#define DMD_USERNAME_OBI_MOVE_END_POS		(-512.f)
#define DMD_USERNAME_OBI_MOVE_SPEED			(-3.f)
#define DMD_USERNAME_OBI_EFCT_TIME			(16.f)
#define DMD_USERNAME_OBI_NODISP_POS_Y		(192.f)
#define DMD_USERNAME_OBI_DISP_POS_Y			(0.f)
#define DMD_USERNAME_ACT_VRTCL_CHNG_DIST	(128.f)
#define DMD_USERNAME_ACT_VRTCL_CHNG_NUM		(3)
#define DMD_USERNAME_CRSR_MOVE_TIME			(8.0f)
#define DMD_USERNAME_DECIDE_EFCT_TIME		(32.0f)
#define DMD_USERNAME_DECIDE_EFCT_WAIT_TIME	(48.0f)
#define DMD_USERNAME_DECIDE_EFCT_END_UP_POS		(-960.0f)
#define DMD_USERNAME_DECIDE_EFCT_END_DOWN_POS	(720.0f)

#define DMD_USERNAME_CRSR_DFLT_POS_X		(195.f)
#define DMD_USERNAME_CRSR_DFLT_POS_Y		(235.f)
#define DMD_USERNAME_CRSR_DIST_ONE_X		(64.f)
#define DMD_USERNAME_CRSR_DIST_ONE_Y		(51.f)
#define DMD_USERNAME_CRSR_DOWN_POS_Y		(500.f)

#define DMD_USERNAME_FOCUS_NAME_CHAR_DIST	(40.f)

#if _WII
#define DMD_USERNAME_DISP_SCALE_TEXT		(1.4f)
#endif


// フラグ関連
#define DMD_USERNAME_FLAG_EXIT				(1 << 0)		//!< 終了フラグ
#define DMD_USERNAME_FLAG_CANCEL			(1 << 1)		//!< キャンセル
#define DMD_USERNAME_FLAG_DECIDE			(1 << 2)		//!< 決定フラグ
#define DMD_USERNAME_FLAG_DECIDE_NAME		(1 << 3)
#define DMD_USERNAME_FLAG_WIN_EFCT_END		(1 << 4)
#define DMD_USERNAME_FLAG_PUSH_BS_BTN		(1 << 5)
#define DMD_USERNAME_FLAG_PUSH_RESIZE_BTN	(1 << 6)
#define DMD_USERNAME_FLAG_PUSH_SPACE_BTN	(1 << 7)
#define DMD_USERNAME_FLAG_PUSH_DECIDE_BTN	(1 << 8)
#define DMD_USERNAME_FLAG_CHNG_VRTCL		(1 << 9)
#define DMD_USERNAME_FLAG_CHNG_CRSR			(1 << 10)
#define DMD_USERNAME_FLAG_RE_CHNG_ZONE		(1 << 11)
#define DMD_USERNAME_FLAG_RE_CHNG_VRTCL		(1 << 12)
#define DMD_USERNAME_FLAG_RE_CHNG_CRSR		(1 << 13)
#define DMD_USERNAME_FLAG_UP_CHNG_CRSR		(1 << 14)
#define DMD_USERNAME_FLAG_DOWN_CHNG_CRSR	(1 << 15)
#define DMD_USERNAME_FLAG_LEFT_CHNG_CRSR	(1 << 16)
#define DMD_USERNAME_FLAG_RIGHT_CHNG_CRSR	(1 << 17)
#define DMD_USERNAME_FLAG_DEL_SAVE_FILE		(1 << 18)
#define DMD_USERNAME_FLAG_CHNG_NAME_POS		(1 << 19)
#define DMD_USERNAME_FLAG_MSG_WIN_EFCT_END	(1 << 20)
#define DMD_USERNAME_FLAG_MSG_BACK_INPUT	(1 << 21)

// アクション表示フラグ関連
#define DMD_USERNAME_DISP_FLAG_WIN_ACT		(1 << 0)
#define DMD_USERNAME_DISP_FLAG_TB_ARROW		(1 << 1)
#define DMD_USERNAME_DISP_FLAG_ACT_CRSR		(1 << 2)
#define DMD_USERNAME_DISP_FLAG_OBI_TEX		(1 << 3)
#define DMD_USERNAME_DISP_FLAG_MSG_WIN_ACT	(1 << 4)
#define DMD_USERNAME_DISP_FLAG_WIN_DRAW		(1 << 5)



// ----- Macro Functions -----------------------------------（処理マクロ定義）


// ----- Definitions -------------------------------------------（定数の宣言）
//! 次のイベント
typedef enum tag_DME_USERNAME_NEXT_EVT
{
	DME_USERNAME_NEXT_EVT_MAINGAME = 0,	//!< メインゲーム
	DME_USERNAME_NEXT_EVT_OPTION,		//!< オプション
	DME_USERNAME_NEXT_EVT_MAINMENU,		//!< メインメニュー
	DME_USERNAME_NEXT_EVT_TITLE,			//!< タイトル

	DME_USERNAME_NEXT_EVT_MAX
} DME_USERNAME_NEXT_EVT;


typedef enum tag_DME_USERNAME_DATA_TYPE
{
	DME_USERNAME_DATA_TYPE_CMN_DATA = 0,		//!< 共通データ
	DME_USERNAME_DATA_TYPE_LANG_DATA,			//!< 言語別データ
//	DME_USERNAME_DATA_TYPE_WIN_CMN_DATA,		//!< ウインドウ用データ
//	DME_USERNAME_DATA_TYPE_WIN_LANG_DATA,		//!< ウインドウ用言語別データ
	
	DME_USERNAME_DATA_TYPE_MAX,
	DME_USERNAME_DATA_TYPE_NONE
} DME_USERNAME_DATA_TYPE;


typedef enum tag_DME_USERNAME_INPUT_STATE
{
	DME_USERNAME_INPUT_STATE_NAME_INPUT,
	DME_USERNAME_INPUT_STATE_CRSR_MOVE,
	DME_USERNAME_INPUT_STATE_NUM,
	DME_USERNAME_INPUT_STATE_NONE
} DME_USERNAME_INPUT_STATE;

//! ウインドウ表示パターンタイプ
typedef enum tag_DME_USERNAME_WIN
{
	DME_USERNAME_WIN_PLEASE_SET_NAME = 0,	//!< 
	DME_USERNAME_WIN_USER_CANCEL,			//!< 
	DME_USERNAME_WIN_DECIDE_NAME,			//!< 
	
	DME_USERNAME_WIN_NUM,
	DME_USERNAME_WIN_NONE
} DME_USERNAME_WIN;


//! セーブファイルNO
typedef enum tag_DME_USERNAME_SAVE_FILE
{
	DME_USERNAME_SAVE_FILE_1 = 0,		//!< 
	DME_USERNAME_SAVE_FILE_2,			//!< 
	DME_USERNAME_SAVE_FILE_3,			//!< 
	DME_USERNAME_SAVE_FILE_4,			//!< 
	DME_USERNAME_SAVE_FILE_5,		//!< 
	DME_USERNAME_SAVE_FILE_6,		//!< 
	
	DME_USERNAME_SAVE_FILE_NUM,
	DME_USERNAME_SAVE_FILE_NONE
} DME_USERNAME_SAVE_FILE;




//! アクションテーブル(ノード含む)
typedef enum tag_DME_USERNAME_ACT
{
	// モード共通・言語共通
	ACT_WIN_TITLE_TAB_1 = 0,//!< 
	ACT_WIN_TITLE_TAB_2,	//!< 
	ACT_WIN_TITLE_TAB_3,	//!< 
	ACT_WIN_BG_TAB_1,		//!< 
	ACT_WIN_BG_TAB_2,		//!< 
	ACT_WIN_BG_TAB_3,		//!< 
//	ACT_FRAME_TAB_1,		//!< 
//	ACT_FRAME_TAB_2,		//!< 
//	ACT_FRAME_TAB_3,		//!< 
//	ACT_FRAME_TAB_4,		//!< 
//	ACT_FRAME_TAB_5,		//!<
	
	ACT_BTN_BS,
	ACT_BTN_CAP,
	ACT_BTN_SPACE,
	ACT_BTN_KETTEI_LEFT,
	ACT_BTN_KETTEI_RIGHT,
	ACT_CUR_CRSR1,
	ACT_CUR_CRSR2,
	ACT_CUR_CRSR3,
	
	ACT_CHAR_1,
	ACT_CHAR_2,
	ACT_CHAR_3,
	ACT_CHAR_4,
	ACT_CHAR_5,
	ACT_CHAR_6,
	ACT_CHAR_7,
	ACT_CHAR_8,
	ACT_CHAR_9,
	ACT_CHAR_0,
	ACT_CHAR_A,
	ACT_CHAR_B,
	ACT_CHAR_C,
	ACT_CHAR_D,
	ACT_CHAR_E,
	ACT_CHAR_F,
	ACT_CHAR_G,
	ACT_CHAR_H,
	ACT_CHAR_I,
	ACT_CHAR_J,
	ACT_CHAR_K,
	ACT_CHAR_L,
	ACT_CHAR_M,
	ACT_CHAR_N,
	ACT_CHAR_O,
	ACT_CHAR_P,
	ACT_CHAR_Q,
	ACT_CHAR_R,
	ACT_CHAR_S,
	ACT_CHAR_T,
	ACT_CHAR_U,
	ACT_CHAR_V,
	ACT_CHAR_W,
	ACT_CHAR_X,
	ACT_CHAR_Y,
	ACT_CHAR_Z,
	ACT_CHAR_KIGO01,
	ACT_CHAR_KIGO02,
	ACT_CHAR_KIGO03,
	ACT_CHAR_KIGO04,
	ACT_CHAR_KIGO05,
	ACT_CHAR_KIGO06,
	ACT_CHAR_KIGO07,
	ACT_CHAR_KIGO08,
	ACT_CHAR_KIGO09,
	ACT_CHAR_KIGO10,
	ACT_CHAR_KIGO11,
	ACT_CHAR_KIGO12,
	ACT_CHAR_KIGO13,
	ACT_CHAR_ALP_1,
	ACT_CHAR_ALP_2,
	ACT_CHAR_ALP_3,
	ACT_CHAR_ALP_4,
	ACT_CHAR_ALP_5,
	ACT_CHAR_ALP_6,
	ACT_CHAR_ALP_7,
	ACT_CHAR_ALP_8,
	ACT_CHAR_ALP_9,
	ACT_CHAR_ALP_10,
//	ACT_FRAME_TAB_6,		//!< 
//	ACT_FRAME_TAB_7,		//!< 
//	ACT_FRAME_TAB_8,		//!< 
//	ACT_FRAME_TAB_9,		//!< 
//	ACT_FRAME_TAB_10,		//!< 
	ACT_FRAME_NAME_CRSR,	//!< 

	// モード共通・言語別
	ACT_TEX_BS,				//!< 
	ACT_TEX_CAP,			//!< 
	ACT_TEX_SPACE,			//!< 
	ACT_TEX_KETTEI,			//!<
	
	ACT_TEX_WIN_MSG2,		//!< 
	
	// メニュー共通データ
	ACT_BTN_CANCEL_WIN,		//!< 
	
	ACT_TEX_BACK_WIN,		//!< 
	ACT_TEX_YES,			//!< 
	ACT_TEX_NO,				//!< 
	ACT_TEX_OK,				//!< 
	
	ACT_NUM,

//	ACT_MODE_CMN = ACT_OBI_DOWN,
//	ACT_MODE_CMN = ACT_TEX_OBI2,
//	ACT_LANG_DIF = ACT_TEX_MODE,

	ACT_FIX_START = ACT_WIN_TITLE_TAB_1,
	ACT_FIX_END = ACT_BTN_KETTEI_RIGHT,

	ACT_SLCT_CHAR_START = ACT_CHAR_1,
	ACT_SLCT_CHAR_END = ACT_CHAR_KIGO13,

	ACT_SAVE_CHAR_START = ACT_CHAR_ALP_1,
	ACT_SAVE_CHAR_END = ACT_CHAR_ALP_10,
	
	ACT_CHNG_CHAR_START = ACT_CHAR_A,
	ACT_CHNG_CHAR_END = ACT_CHAR_Z,
	
	ACT_NONE
} DME_USERNAME_ACT;


typedef struct tag_DMS_USERNAME_MAIN_WORK	DMS_USERNAME_MAIN_WORK;

//! メインタスクワーク
struct tag_DMS_USERNAME_MAIN_WORK {

	AMS_FS			*arc_amb;							//!< アーカイブAMBファイル
	AMS_FS			*win_amb_fs;						//!< ウインドウAMBファイル
//	AMS_FS *		ama_fs[DME_USERNAME_DATA_TYPE_MAX];	//!< AMAファイル管理
//	AMS_FS *		amb_fs[DME_USERNAME_DATA_TYPE_MAX];	//!< AMBファイル管理
	void			*ama[DME_USERNAME_DATA_TYPE_MAX];	//!< AMAファイル
	void			*amb[DME_USERNAME_DATA_TYPE_MAX];	//!< AMBファイル
	void			*win_amb;							//!< ウインドウ用AMBファイル
	
	AOS_TEXTURE		tex[DME_USERNAME_DATA_TYPE_MAX];		//!< テクスチャ
	AOS_TEXTURE		win_tex;							//!< ウインドウテクスチャ

	// ウインドウ用アクション

	// メニュー用アクション
	AOS_ACTION 		*act[ACT_NUM];
	AOS_ACTION		*node_act;			// テーブルノード用
	
	AOS_ACTION 		*file_act[DMD_USERNAME_SAVE_FILE_NUM];

	void (*proc_win_input)(DMS_USERNAME_MAIN_WORK *);	//!< 入力処理関数
	void (*proc_input)(DMS_USERNAME_MAIN_WORK *);		//!< 入力処理関数
	void (*proc_win_update)(DMS_USERNAME_MAIN_WORK *);	//!< ウインドウ用プロシージャ
	void (*proc_menu_update)(DMS_USERNAME_MAIN_WORK *);	//!< メニュー用プロシージャ
	void (*proc_draw)(DMS_USERNAME_MAIN_WORK *);			//!< 描画用プロシージャ

	float timer;											//!< 汎用タイマー
	u32	flag;											//!< 汎用フラグ
	s32 state;											//!< ZONE選択中かSTAGE選択中か
	float win_timer;									//!< ウインドウ演出用タイマー
	u32 disp_flag;										//!< 表示切替用フラグ

	s32 next_evt;										//!< 次のイベント
	s32 prev_evt;										//!< 前のイベント

	// WINDOW専用
	float win_act_pos[14-1][2];							//!< 
	float win_size_rate[2];								//!< 
	float msg_win_size_rate[2];								//!< 
	s32 win_mode;										//!< 
	s32 win_cur_slct;									//!< ウインドウでの現在の選択項目
	s32 wait_timer;
	
	float efct_timer[4];									//!< 演出時間
	u32 efct_out_flag;									//!< 
	u32 announce_flag;									//!< 0ならアナウンスなし、それ以外はフラグがあるだけ表示する
	
	// STAGE専用
	float crsr_move_src;
	float crsr_move_dst;
	float crsr_pos[2];
	float prev_crsr_pos[2];
	float crsr_scale;
	
	float crsr_focus[2];
	float prev_crsr_focus[2];
	s32 cur_char;
	s32 focus_name_no;
	s32 is_char_small;

	float cur_slct_disp_name_pos_x;
	
	u32 draw_state;

	// セーブする名前文字列
	u8 name_string[DMD_USERNAME_CHAR_NUM_MAX + 1];
	
	// 変更前の名前文字列
	u8 prev_name_string[DMD_USERNAME_CHAR_NUM_MAX + 1];
};


//! 管理構造体
typedef struct tag_DMS_USERNAME_MGR {
	MTS_TASK_TCB *tcb;	//!< TCB
} DMS_USERNAME_MGR;



// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static void dmUserNameInit(void);
static void dmUserNameProcMain(MTS_TASK_TCB *tcb);
static void dmUserNameDest(MTS_TASK_TCB *tcb);

// 初期化設定関連
static void dmUserNameSetSaveData(DMS_USERNAME_MAIN_WORK *main_work);

static void dmUserNameProcInit(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameProcCreateAct(DMS_USERNAME_MAIN_WORK *main_work);

// メニュー用プロシージャ
static void dmUserNameProcStopDraw(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameProcFinish(DMS_USERNAME_MAIN_WORK *main_work);

static void dmUserNameProcWaitInput(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameProcChngFocusNamePos(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameProcSlctCharDecideEfct(DMS_USERNAME_MAIN_WORK *main_work);

// ウインドウ用プロシージャ
static void dmUserNameProcWindowInEfct(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameProcWindowOutEfct(DMS_USERNAME_MAIN_WORK *main_work);

static void dmUserNameProcWindowNodispIdle(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameProcWindowOpenEfct(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameProcWindowAnnounceIdle(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameProcWindowCloseEfct(DMS_USERNAME_MAIN_WORK *main_work);

// 入力処理用プロシージャ
static void dmUserNameInputProcCharSlct(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameInputProcChngFocus(DMS_USERNAME_MAIN_WORK *main_work);

static void dmUserNameInputProcWinYesNo(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameInputProcWinOk(DMS_USERNAME_MAIN_WORK *main_work);

// 描画関連処理
static void dmUserNameProcActDraw(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameCommonDraw(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameCommonFixDraw(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameCharDraw(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameWinMsgDraw(DMS_USERNAME_MAIN_WORK *main_work);

// 演出関連設定処理
static void dmUserNameSetFileTabDecideEfct(DMS_USERNAME_MAIN_WORK *main_work);
static BOOL dmUserNameIsFileTabDecideEfctEnd(DMS_USERNAME_MAIN_WORK *main_work);

static void dmUserNameSetWinOpenEfct(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameSetWinCloseEfct(DMS_USERNAME_MAIN_WORK *main_work);

static void dmUserNameSetMsgWinOpenEfct(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameSetMsgWinCloseEfct(DMS_USERNAME_MAIN_WORK *main_work);

static s32 dmUserNameIsTexLoad(void);
static s32 dmUserNameIsTexRelease(void);

static s32 dmUserNameGetRevisedCharCrsrNo(s32 idx, s32 diff, s32 dir, s32 focus_row, s32 focus_clmn);
static s32 dmUserNameGetRevisedSetNameNo(s32 idx, s32 diff);
static s32 dmUserNameGetRevisedSetFocusNameCrsr(s32 idx, s32 diff, s32 max);

static void dmUserNameSetDecideChar(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameSetFocusChangeEfctData(DMS_USERNAME_MAIN_WORK *main_work);
//static void dmUserNameSetDecideEfctData(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameSetCrsrChangeData(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameSetChngFocusNamePos(DMS_USERNAME_MAIN_WORK *main_work);

static void dmUserNameSetBtnPushEfct(DMS_USERNAME_MAIN_WORK *main_work, u32 i);
static void dmUserNameSetNameBackSpace(DMS_USERNAME_MAIN_WORK *main_work);
static void dmUserNameSetMsgWinState(DMS_USERNAME_MAIN_WORK *main_work);

static BOOL dmUserNameIsNameStringAllSpace(DMS_USERNAME_MAIN_WORK *main_work);
static BOOL dmUserNameIsChangeNameString(DMS_USERNAME_MAIN_WORK *main_work);

static void dmUserNameProcDataSave(DMS_USERNAME_MAIN_WORK *main_work);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ZONEごとのカーソル表示位置Yテーブル
const static float dm_username_act_crsr_disp_y_pos_tbl[3] = {
	100.0f,
	100.0f + 160.f * 1.f,
	100.0f + 160.f * 2.f,
};


// ボタン行のFOCUS値ごとの表示位置テーブル
const static float dm_username_char_crsr_down_disp_y_pos_tbl[10] = {
	228.f,
	228.f,
	355.f,
	355.f,
	483.f,
	483.f,
	483.f,
	708.f,
	708.f,
	708.f,
};

// ボタン行のFOCUS値ごとのSCALE値テーブル
const static float dm_username_char_crsr_down_scale_x_tbl[10] = {
	4.6f,
	4.6f,
	4.6f,
	4.6f,
	4.6f,
	4.6f,
	4.6f,
	7.4f,
	7.4f,
	7.4f,
};


/*
// 表示ウインドウアクションIDテーブル
const static float dm_username_win_act_frm_tbl[DME_USERNAME_WIN_NUM][3] = {
	// タイトル			メッセージ			OK
	{2.f, 0.f, 1.f},		// メニュー
	{3.f, 0.f, 0.f},		// アクト決定
	{1.f, 1.f, 0.f},		// ACT選択可能メッセージ
	{1.f, 2.f, 0.f},		// BOSSACT選択可能メッセージ
	{1.f, 3.f, 0.f},		// FINALZONE選択可能メッセージ
	{1.f, 4.f, 0.f},		// スーパーソニック変身可能メッセージ
//	フレーム、フレーム、パターン番号
};
*/

#if !_WII
const static float dm_username_win_act_pos_tbl[11][2] = {
	{DMD_USERNAME_SIZE_HALF_WIDTH, 360.0f},			// メッセージ1
	{DMD_USERNAME_SIZE_HALF_WIDTH, 360.0f - 32.f},			// メッセージ2
	{DMD_USERNAME_SIZE_HALF_WIDTH, 360.0f},			// メッセージ3
	{DMD_USERNAME_SIZE_HALF_WIDTH - 88.f, 420.0f + 24.f},	// YES
	{DMD_USERNAME_SIZE_HALF_WIDTH + 88.f, 420.0f + 24.f},	// NO
	{DMD_USERNAME_SIZE_HALF_WIDTH, 420.0f + 32.f},			// OK
	{DMD_USERNAME_SIZE_HALF_WIDTH + 182.f + 66.f, 264.0f},	// キャンセルボタン
	{DMD_USERNAME_SIZE_HALF_WIDTH + 202.f + 66.f, 264.0f},	// 戻る
};
#else
const static float dm_username_win_act_pos_tbl[11][2] = {
	{DMD_USERNAME_SIZE_HALF_WIDTH, 360.0f},			// メッセージ1
	{DMD_USERNAME_SIZE_HALF_WIDTH, 360.0f - 32.f},			// メッセージ2
	{DMD_USERNAME_SIZE_HALF_WIDTH, 360.0f},			// メッセージ3
	{DMD_USERNAME_SIZE_HALF_WIDTH - 88.f, 420.0f + 24.f},	// YES
	{DMD_USERNAME_SIZE_HALF_WIDTH + 88.f, 420.0f + 24.f},	// NO
	{DMD_USERNAME_SIZE_HALF_WIDTH, 420.0f + 32.f},			// OK
	{DMD_USERNAME_SIZE_HALF_WIDTH + 262.f + 96.f, 232.0f},	// キャンセルボタン
	{DMD_USERNAME_SIZE_HALF_WIDTH + 282.f + 96.f, 232.0f},	// 戻る
};
#endif


const static float dm_username_back_text_length_tbl[6] = {
	-39.f,
	-53.f,
	-69.f,
	-77.f,
	-70.f,
	-55.f,
};


// ウインドウ選択肢用表示フレームテーブル(現状２つ用)
const static float dm_username_win_disp_slct_frm_tbl[2][2] = {
	{0.f, 1.f},		// 左(上)がアクティブ
	{1.f, 0.f},		// 右(下)がアクティブ
};



// ACT縦並びの表示位置テーブル
const static float dm_username_vrtcl_disp_pos_y_tbl[7 - DMD_USERNAME_ACT_VRTCL_CHNG_NUM] = {
	DMD_USERNAME_ACT_VRTCL_CHNG_DIST * 0.f,
	DMD_USERNAME_ACT_VRTCL_CHNG_DIST * 1.f,
	DMD_USERNAME_ACT_VRTCL_CHNG_DIST * 2.f,
	DMD_USERNAME_ACT_VRTCL_CHNG_DIST * 3.f,
};


// カーソル表示座標テーブル(文字数分)
/*
const static float dm_username_win_disp_slct_frm_tbl[10][6] = {
	{0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
	{0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
	{0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
	{0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
	{0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
	{0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
};
*/


// アクションIDテーブル(初期状態)
const static u32 g_dm_act_id_tbl[ACT_NUM] = {
	// 言語共通
	IDA_D_OPTION_USER_ACT_TAB04_LEFT,
	IDA_D_OPTION_USER_ACT_TAB04_CENTER,
	IDA_D_OPTION_USER_ACT_TAB04_RIGHT,
	IDA_D_OPTION_USER_ACT_TAB06_UE,
	IDA_D_OPTION_USER_ACT_TAB06_SHITA01,
	IDA_D_OPTION_USER_ACT_TAB06_SHITA02,
//	IDA_D_OPTION_USER_ACT_TAB01_A,
//	IDA_D_OPTION_USER_ACT_TAB01_B,
//	IDA_D_OPTION_USER_ACT_TAB01_C,
//	IDA_D_OPTION_USER_ACT_TAB01_D,
//	IDA_D_OPTION_USER_ACT_TAB01_E,

	// テーブル部
	IDA_D_OPTION_USER_ACT_BUT_BS,
	IDA_D_OPTION_USER_ACT_BUT_CAP,
	IDA_D_OPTION_USER_ACT_BUT_SPACE,
	IDA_D_OPTION_USER_ACT_BUT_KETTEI_LEFT,
	IDA_D_OPTION_USER_ACT_BUT_KETTEI_RIGHT,
	IDA_D_OPTION_USER_ACT_CUR_ALPHA1,
	IDA_D_OPTION_USER_ACT_CUR_ALPHA2,
	IDA_D_OPTION_USER_ACT_CUR_ALPHA3,

	// 文字
	IDA_D_OPTION_USER_ACT_1,
	IDA_D_OPTION_USER_ACT_2,
	IDA_D_OPTION_USER_ACT_3,
	IDA_D_OPTION_USER_ACT_4,
	IDA_D_OPTION_USER_ACT_5,
	IDA_D_OPTION_USER_ACT_6,
	IDA_D_OPTION_USER_ACT_7,
	IDA_D_OPTION_USER_ACT_8,
	IDA_D_OPTION_USER_ACT_9,
	IDA_D_OPTION_USER_ACT_0,
	IDA_D_OPTION_USER_ACT_A,
	IDA_D_OPTION_USER_ACT_B,
	IDA_D_OPTION_USER_ACT_C,
	IDA_D_OPTION_USER_ACT_D,
	IDA_D_OPTION_USER_ACT_E,
	IDA_D_OPTION_USER_ACT_F,
	IDA_D_OPTION_USER_ACT_G,
	IDA_D_OPTION_USER_ACT_H,
	IDA_D_OPTION_USER_ACT_I,
	IDA_D_OPTION_USER_ACT_J,
	IDA_D_OPTION_USER_ACT_K,
	IDA_D_OPTION_USER_ACT_L,
	IDA_D_OPTION_USER_ACT_M,
	IDA_D_OPTION_USER_ACT_N,
	IDA_D_OPTION_USER_ACT_O,
	IDA_D_OPTION_USER_ACT_P,
	IDA_D_OPTION_USER_ACT_Q,
	IDA_D_OPTION_USER_ACT_R,
	IDA_D_OPTION_USER_ACT_S,
	IDA_D_OPTION_USER_ACT_T,
	IDA_D_OPTION_USER_ACT_U,
	IDA_D_OPTION_USER_ACT_V,
	IDA_D_OPTION_USER_ACT_W,
	IDA_D_OPTION_USER_ACT_X,
	IDA_D_OPTION_USER_ACT_Y,
	IDA_D_OPTION_USER_ACT_Z,
	IDA_D_OPTION_USER_ACT_KIGO01,
	IDA_D_OPTION_USER_ACT_KIGO02,
	IDA_D_OPTION_USER_ACT_KIGO03,
	IDA_D_OPTION_USER_ACT_KIGO04,
	IDA_D_OPTION_USER_ACT_KIGO05,
	IDA_D_OPTION_USER_ACT_KIGO06,
	IDA_D_OPTION_USER_ACT_KIGO07,
	IDA_D_OPTION_USER_ACT_KIGO08,
	IDA_D_OPTION_USER_ACT_KIGO09,
	IDA_D_OPTION_USER_ACT_KIGO10,
	IDA_D_OPTION_USER_ACT_KIGO11,
	IDA_D_OPTION_USER_ACT_KIGO12,
	IDA_D_OPTION_USER_ACT_KIGO13,
	IDA_D_OPTION_USER_ACT_ALP_1,
	IDA_D_OPTION_USER_ACT_ALP_2,
	IDA_D_OPTION_USER_ACT_ALP_3,
	IDA_D_OPTION_USER_ACT_ALP_4,
	IDA_D_OPTION_USER_ACT_ALP_5,
	IDA_D_OPTION_USER_ACT_ALP_6,
	IDA_D_OPTION_USER_ACT_ALP_7,
	IDA_D_OPTION_USER_ACT_ALP_8,
	IDA_D_OPTION_USER_ACT_ALP_9,
	IDA_D_OPTION_USER_ACT_ALP_10,
//	IDA_D_OPTION_USER_ACT_TAB01_F,
//	IDA_D_OPTION_USER_ACT_TAB01_G,
//	IDA_D_OPTION_USER_ACT_TAB01_H,
//	IDA_D_OPTION_USER_ACT_TAB01_I,
//	IDA_D_OPTION_USER_ACT_TAB01_J,
	IDA_D_OPTION_USER_ACT_CUR02,
	
	// 言語別
	IDA_D_OPTION_USER_JP_ACT_TEX_BS,
	IDA_D_OPTION_USER_JP_ACT_TEX_CAP,
	IDA_D_OPTION_USER_JP_ACT_TEX_SPACE,
	IDA_D_OPTION_USER_JP_ACT_TEX_KETTEI,
	
	IDA_D_OPTION_USER_JP_ACT_TEX_WIN_MSG2,	//!< 
	
	// メニュー共通データ
	IDA_D_CMN_BTN_ACT_BACK_BTN,		//!< 
	
	IDA_D_CMN_MSG_JP_ACT_TEX_BACK,	//!< 
	IDA_D_CMN_MSG_JP_ACT_TEX_YES,	//!< 
	IDA_D_CMN_MSG_JP_ACT_TEX_NO,	//!< 
	IDA_D_CMN_MSG_JP_ACT_TEX_OK,	//!< 
	
};



//管理情報
static DMS_USERNAME_MGR dm_username_mgr;
static DMS_USERNAME_MGR *dm_username_mgr_p = NULL;

static void *dm_username_ama[DME_USERNAME_DATA_TYPE_MAX];
static void *dm_username_amb[DME_USERNAME_DATA_TYPE_MAX];
static AOS_TEXTURE dm_username_tex[DME_USERNAME_DATA_TYPE_MAX];
static s32 dm_username_is_name_decide = 0;

static void *dm_username_cmn_ama[3];
static void *dm_username_cmn_amb[3];
static AOS_TEXTURE *dm_username_cmn_tex[3];

static u32 dm_username_draw_state = 0;
static BOOL dm_username_is_win_open = FALSE;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ==========================================================================
// DmUserNameBuild
/*!
 *	ユーザー名設定画面データ構築
  	(ファイルの読込みは呼び出し側で行い、引数でポインタを渡してこちらでデータ構築)
 */
// ==========================================================================
void DmUserNameBuild(void *arc_amb[])
{
	int i = 0;

	// 管理情報初期化
	amZeroMemory(&dm_username_mgr, sizeof(DMS_USERNAME_MGR));
	dm_username_mgr_p = &dm_username_mgr;
	
	for (i = 0; i < DME_USERNAME_DATA_TYPE_MAX; i++) {
		amZeroMemory(&dm_username_tex[i], sizeof(AOS_TEXTURE));
	}

	// AMBファイルロード
	for (i = 0; i < DME_USERNAME_DATA_TYPE_MAX; i++) {
		amBindConv((u8 *)arc_amb[i]);
		
		dm_username_ama[i] = amBindGet((AMS_AMB_HEADER*)arc_amb[i]
									   , 0
									   );
		
		dm_username_amb[i] = amBindGet((AMS_AMB_HEADER*)arc_amb[i]
									   , 1
									   );
	}
	
	// アドレス変換
	for (i = 0; i < DME_USERNAME_DATA_TYPE_MAX; i++) {
		amConvertAddress(dm_username_ama[i]);
		amConvertAddress(dm_username_amb[i]);
	}
	
	// テクスチャ構築
	for (i = 0; i < DME_USERNAME_DATA_TYPE_MAX; i++) {
		// テクスチャ構築開始
		AoTexBuild(&dm_username_tex[i], dm_username_amb[i]);
		AoTexLoad(&dm_username_tex[i]);
	}
}

// ==========================================================================
// DmUserNameBuildCheck
/*!
 *	ユーザー名設定画面データ構築 終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL DmUserNameBuildCheck(void)
{
	// テクスチャ構築チェック
	if (dmUserNameIsTexLoad()) {
		// フラグ扱いでON
		return (TRUE);
	}

	return (FALSE);
}


// ==========================================================================
// DmUserNameSetMenuCmnAmaData
/*!
 *	ユーザー名設定画面にメニュー共通AMAデータ設定
  	(構築済みデータをポインタのみ取得)
 */
// ==========================================================================
void DmUserNameSetMenuCmnAmaData(void *btn_ama, void *win_ama, void *tex_ama)
{
	dm_username_cmn_ama[0] = btn_ama;
	dm_username_cmn_ama[1] = win_ama;
	dm_username_cmn_ama[2] = tex_ama;
}


// ==========================================================================
// DmUserNameSetMenuCmnAmbData
/*!
 *	ユーザー名設定画面にメニュー共通AMBデータ設定
  	(構築済みデータをポインタのみ取得)
 */
// ==========================================================================
void DmUserNameSetMenuCmnAmbData(void *btn_amb, void *win_amb, void *tex_amb)
{
	dm_username_cmn_amb[0] = btn_amb;
	dm_username_cmn_amb[1] = win_amb;
	dm_username_cmn_amb[2] = tex_amb;
}


// ==========================================================================
// DmUserNameSetMenuCmnTexData
/*!
 *	ユーザー名設定画面にメニュー共通TEXデータ設定
  	(構築済みデータをポインタのみ取得)
 */
// ==========================================================================
void DmUserNameSetMenuCmnTexData(AOS_TEXTURE *btn_tex, AOS_TEXTURE *win_tex, AOS_TEXTURE *tex_tex)
{
	dm_username_cmn_tex[0] = btn_tex;
	dm_username_cmn_tex[1] = win_tex;
	dm_username_cmn_tex[2] = tex_tex;
}


// ==========================================================================
// DmUserNameFlush
/*!
 *	ユーザー名設定画面データフラッシュ
 *
 *	@reutrn	リングシステムワークアドレス
 */
// ==========================================================================
void DmUserNameFlush(void)
{
	// テクスチャ解放
	for (int i = 0; i < DME_USERNAME_DATA_TYPE_MAX; i++) {
		AoTexRelease(&dm_username_tex[i]);
		
	}
}


// ==========================================================================
// DmUserNameFlushCheck
/*!
 *	ユーザー名設定画面データフラッシュ終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL DmUserNameFlushCheck(void)
{
	// テクスチャ解放
	if (dmUserNameIsTexRelease()) {
		
		return (TRUE);
	}
	
	return (FALSE);
}


// ==========================================================================
// DmUserNameStart
/*!
	ユーザー名設定画面開始処理
 */
// ==========================================================================
void DmUserNameStart(void)
{
	dmUserNameInit();
}



// ==========================================================================
// DmUserNameIsExit
/*!
	ユーザー名設定画面の終了確認処理
 */
// ==========================================================================
s32 DmUserNameIsExit(void)
{
	if (dm_username_mgr_p->tcb == NULL) {
		return dm_username_is_name_decide;
	}

	return 0;
}


// ==========================================================================
// DmUserNameExit
/*!
	ユーザー名設定画面の終了処理
 */
// ==========================================================================
void DmUserNameExit(void)
{
	// タスククリア
	if (dm_username_mgr_p->tcb) {
		mtTaskClearTcb(dm_username_mgr_p->tcb);

		dm_username_mgr_p->tcb = NULL;
	}
}



// ==========================================================================
// DmUserNameGetUserLangAma
/*!
	ユーザー名設定画面の言語別AMAデータ取得
  	(帯文を外部で使用するため)
 */
// ==========================================================================
void *DmUserNameGetUserAma(void)
{
	if (dm_username_ama[DME_USERNAME_DATA_TYPE_LANG_DATA]) {
		return dm_username_ama[DME_USERNAME_DATA_TYPE_LANG_DATA];
	}
	
	else {
		MTM_ASSERT(0);
		return NULL;
	}
}



// ==========================================================================
// DmUserNameGetUserLangTex
/*!
	ユーザー名設定画面の言語別TEXデータ取得
  	(帯文を外部で使用するため)
 */
// ==========================================================================
AOS_TEXTURE *DmUserNameGetUserLangTex(void)
{
	return &dm_username_tex[DME_USERNAME_DATA_TYPE_LANG_DATA];
}



// ==========================================================================
// DmUserNameIsMsgWinOpen
/*!
	ユーザー名設定画面のメッセージウインドウが開いているかどうか
 */
// ==========================================================================
BOOL DmUserNameIsMsgWinOpen(void)
{
	return dm_username_is_win_open;
}


// ----- Static Functions --------------------（スタティック関数の定義：局所）
// ==========================================================================
// dmUserNameInit
/*!
	ユーザー名設定画面初期化処理
 */
// ==========================================================================
void dmUserNameInit(void)
{
	DMS_USERNAME_MAIN_WORK	*main_work;

	// メインタスク作成
	dm_username_mgr_p->tcb = MTM_TASK_MAKE_TCB(dmUserNameProcMain
											   , dmUserNameDest
											   , 0
											   , DMD_USERNAME_TASK_PAUSELEVEL
											   , DMD_USERNAME_TASK_PRIO_MAIN
											   , DMD_USERNAME_TASK_GROUP_MAIN
											   , sizeof(DMS_USERNAME_MAIN_WORK)
											   , "USERNAME_MAIN"
											   );
	
	// ワーク初期化
	main_work = (DMS_USERNAME_MAIN_WORK *)mtTaskGetTcbWork(dm_username_mgr_p->tcb);
	
	main_work->draw_state = (u32)AoActSysGetDrawStateEnable();

	AoActSysSetDrawStateEnable((int)main_work->draw_state);
	
	if (main_work->draw_state) {
		dm_username_draw_state = AoActSysGetDrawState();
	}

	// 初期化処理があればここに記述
	dmUserNameSetSaveData(main_work);

	// プロシージャ設定
	main_work->proc_menu_update = dmUserNameProcInit;
}



// ==========================================================================
// dmUserNameSetSaveData
/*!
	セーブデータにある名前データの設定処理
 */
// ==========================================================================
void dmUserNameSetSaveData(DMS_USERNAME_MAIN_WORK *main_work)
{
	// 初期化関連
	main_work->crsr_scale = 1.f;
	
#if _WII
	gs::backup::SOption &data
		= gs::backup::SOption::CreateInstance();
	GsCharCodeConvStringAsciiToGame(
		(u8 *)main_work->name_string, 10,
		data.GetName(), 10);
#else
	
	// セーブされている名前をロード
	// ※PC版は常にSONICの仮設定
	main_work->name_string[0] = 29;
	main_work->name_string[1] = 25;
	main_work->name_string[2] = 24;
	main_work->name_string[3] = 19;
	main_work->name_string[4] = 13;
#endif
	
	for (int i = 0; i < 10; i++) {
		if (main_work->name_string[i] != 0) {
			break;
		}
	}
	
	
	// 文字を入力するFOCUS位置を設定
	for (int i = 0; i < 10; i++) {
		main_work->focus_name_no = i;
		
		if (main_work->name_string[i] == 0) {
			break;
		}
	}
	
	// ここで初めの状態の名前文字列をコピー
	for (int i = 0; i < DMD_USERNAME_CHAR_NUM_MAX; i++) {
		main_work->prev_name_string[i] = main_work->name_string[i];
	}
	
	// 初期状態の入力モードを設定
	main_work->state = DME_USERNAME_INPUT_STATE_NAME_INPUT;
}



// ==========================================================================
// dmUserNameProcMain
/*!
	ユーザー名設定画面メインプロシージャ処理
 */
// ==========================================================================
void dmUserNameProcMain(MTS_TASK_TCB *tcb)
{
	DMS_USERNAME_MAIN_WORK	*main_work;

	// ワーク取得
	main_work = (DMS_USERNAME_MAIN_WORK *)mtTaskGetTcbWork(tcb);

	// 終了処理
	if (main_work->flag & DMD_USERNAME_FLAG_EXIT) {
		// タスククリア
		DmUserNameExit();
	}

	// システム関連処理

	// ウインドウ処理用プロシージャ
	if (main_work->proc_win_update) {
		main_work->proc_win_update(main_work);
	}

	// メニュー処理用プロシージャ
	if (main_work->proc_menu_update
		&& !main_work->announce_flag) {
		main_work->proc_menu_update(main_work);
	}

	// 描画設定プロシージャ
	if (main_work->proc_draw) {
		main_work->proc_draw(main_work);
	}

	// ウインドウ開閉状態設定
	dmUserNameSetMsgWinState(main_work);
}


// ==========================================================================
// dmUserNameDest
/*!
	ユーザー名設定画面終了処理
 */
// ==========================================================================
void dmUserNameDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	
}



// ==========================================================================
// dmUserNameProcInit
/*!
	テクスチャ構築待ち処理
 */
// ==========================================================================
void dmUserNameProcInit(DMS_USERNAME_MAIN_WORK *main_work)
{
	// 次へ遷移
	main_work->proc_menu_update = dmUserNameProcCreateAct;
	
	
}


// ==========================================================================
// dmUserNameProcCreateAct
/*!
	アクション生成処理
 */
// ==========================================================================
void dmUserNameProcCreateAct(DMS_USERNAME_MAIN_WORK *main_work)
{
	// ファイル選別
	const void *ama;
	AOS_TEXTURE *tex;
	
	// アクション構築
	for (u32 i = 0; i < ACT_NUM; ++i) {
		
		if (i >= ACT_TEX_BACK_WIN) {
			ama = dm_username_cmn_ama[2];
			tex = dm_username_cmn_tex[2];
		}
		else if (i >= ACT_BTN_CANCEL_WIN) {
			ama = dm_username_cmn_ama[0];
			tex = dm_username_cmn_tex[0];
		}
		
		else if (i >= ACT_TEX_BS) {
			ama = dm_username_ama[DME_USERNAME_DATA_TYPE_LANG_DATA];
			tex = &dm_username_tex[DME_USERNAME_DATA_TYPE_LANG_DATA];
		}
		else {
			ama = dm_username_ama[DME_USERNAME_DATA_TYPE_CMN_DATA];
			tex = &dm_username_tex[DME_USERNAME_DATA_TYPE_CMN_DATA];
		}
		
		// 構築
		AoActSetTexture(AoTexGetTexList(tex));
		main_work->act[i] = AoActCreate(ama, g_dm_act_id_tbl[i]);
	}
	
	ama = dm_username_ama[DME_USERNAME_DATA_TYPE_CMN_DATA];
	tex = &dm_username_tex[DME_USERNAME_DATA_TYPE_CMN_DATA];
	AoActSetTexture(AoTexGetTexList(tex));
	main_work->node_act = AoActCreateNode(ama, 0);
	
	
	// テクスチャセットまで出来たので描画プロシージャを設定
	main_work->proc_draw = dmUserNameProcActDraw;
	
	// イベント遷移
	main_work->proc_menu_update = dmUserNameProcWindowInEfct;
}



// ==========================================================================
// dmUserNameProcWindowInEfct
/*!
	ウインドウオープン処理
 */
// ==========================================================================
void dmUserNameProcWindowInEfct(DMS_USERNAME_MAIN_WORK *main_work)
{
	// ウインドウ演出
	if (main_work->flag & DMD_USERNAME_FLAG_WIN_EFCT_END) {
		// イベント遷移
		main_work->proc_menu_update = dmUserNameProcWaitInput;
		main_work->proc_win_update = dmUserNameProcWindowNodispIdle;
		
		main_work->proc_input = dmUserNameInputProcCharSlct;
		
//		main_work->announce_flag |= 1 << DME_USERNAME_WIN_PLEASE_SET_NAME;
		
		main_work->disp_flag |= DMD_USERNAME_DISP_FLAG_ACT_CRSR;
		main_work->disp_flag |= DMD_USERNAME_DISP_FLAG_WIN_ACT;
		
		main_work->flag &= ~DMD_USERNAME_FLAG_WIN_EFCT_END;
	}
	else {
		dmUserNameSetWinOpenEfct(main_work);
	}
}



// ==========================================================================
// dmUserNameProcWaitInput
/*!
	ユーザー名設定時の入力待ち中処理
 */
// ==========================================================================
void dmUserNameProcWaitInput(DMS_USERNAME_MAIN_WORK *main_work)
{
	// 入力処理
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}

	// キャンセルフラグONならば
	if (main_work->flag & DMD_USERNAME_FLAG_CANCEL) {
		// ウインドウ演出時間設定
		main_work->win_timer = DMD_USERNAME_WIN_EFCT_TIME;
		
		dm_username_is_name_decide = -1;
		
		if (dmUserNameIsChangeNameString(main_work)) {
			main_work->announce_flag |= 1 << DME_USERNAME_WIN_USER_CANCEL;
		}
		else {
			main_work->proc_input = NULL;
			main_work->proc_win_input = NULL;
			
			// タイトル側へ戻る(掃け演出開始)
			main_work->proc_menu_update = dmUserNameProcWindowOutEfct;
			
			// ウインドウ演出時間設定
			main_work->win_timer = DMD_USERNAME_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_USERNAME_DISP_FLAG_MSG_WIN_ACT;
			
			// 文字入力ウインドウのアクションを非表示
			main_work->disp_flag &= ~DMD_USERNAME_DISP_FLAG_WIN_ACT;
		}
		
		// フラグOFF
		main_work->flag &= ~DMD_USERNAME_FLAG_DECIDE;
		main_work->flag &= ~DMD_USERNAME_FLAG_CANCEL;
		
		// メインメニューへイベント遷移先設定
		main_work->next_evt = DME_USERNAME_NEXT_EVT_MAINMENU;
		
		DmSoundPlaySE("Cancel");
		
		return;
	}

	// 決定フラグONならば
	if (main_work->flag & DMD_USERNAME_FLAG_DECIDE) {
		// 0文字だったら元に戻す
//		if (main_work->name_string[0] == 0) {
//			return;
//		}
		
		// 決定時の設定処理
		dmUserNameSetDecideChar(main_work);
		
		
		if (main_work->flag & DMD_USERNAME_FLAG_DECIDE_NAME) {
//			dmUserNameSetDecideEfctData(main_work);
			
			// 決定演出へ移行
			main_work->proc_menu_update = dmUserNameProcSlctCharDecideEfct;		// ここは終了処理へ切り替え(フェードなし)
			
			main_work->flag &= ~DMD_USERNAME_FLAG_DECIDE_NAME;
			
			dm_username_is_name_decide = 1;
			
			dmUserNameProcDataSave(main_work);
		}
		
		// フラグOFF
		main_work->flag &= ~DMD_USERNAME_FLAG_DECIDE;
		main_work->flag &= ~DMD_USERNAME_FLAG_CANCEL;
		
		DmSoundPlaySE("Ok");
		
		return;
	}
	
	// カーソル切り替え
	if (main_work->flag & DMD_USERNAME_FLAG_UP_CHNG_CRSR
		|| main_work->flag & DMD_USERNAME_FLAG_DOWN_CHNG_CRSR
		|| main_work->flag & DMD_USERNAME_FLAG_LEFT_CHNG_CRSR
		|| main_work->flag & DMD_USERNAME_FLAG_RIGHT_CHNG_CRSR) {
		
		dmUserNameSetFocusChangeEfctData(main_work);
		main_work->timer = 0;
		
		if (main_work->flag & DMD_USERNAME_FLAG_CHNG_NAME_POS) {
			main_work->proc_menu_update = dmUserNameProcChngFocusNamePos;
			main_work->proc_input = dmUserNameInputProcChngFocus;
			
			main_work->state = DME_USERNAME_INPUT_STATE_CRSR_MOVE;
			
			main_work->flag &= ~DMD_USERNAME_FLAG_CHNG_NAME_POS;
		}
		
		// フラグOFF
		main_work->flag &= ~DMD_USERNAME_FLAG_UP_CHNG_CRSR;
		main_work->flag &= ~DMD_USERNAME_FLAG_DOWN_CHNG_CRSR;
		main_work->flag &= ~DMD_USERNAME_FLAG_LEFT_CHNG_CRSR;
		main_work->flag &= ~DMD_USERNAME_FLAG_RIGHT_CHNG_CRSR;
		
		DmSoundPlaySE("Cursol");
		
		return;
	}
	
	// ボタン押した際の演出更新
	for (u32 i = 0; i < 5; i++) {
		if (main_work->flag & (1 << (5 + i))) {
			dmUserNameSetBtnPushEfct(main_work, i);
		}
	}
}



// ==========================================================================
// dmUserNameProcChngFocusNamePos
/*!
	ユーザー名設定時の入力待ち中処理
 */
// ==========================================================================
void dmUserNameProcChngFocusNamePos(DMS_USERNAME_MAIN_WORK *main_work)
{
	// 入力処理
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}

	// キャンセルフラグONならば
	if (main_work->flag & DMD_USERNAME_FLAG_CANCEL) {
		// ウインドウ演出時間設定
		main_work->win_timer = DMD_USERNAME_WIN_EFCT_TIME;
		
		dm_username_is_name_decide = -1;
		
		if (dmUserNameIsChangeNameString(main_work)) {
			main_work->announce_flag |= 1 << DME_USERNAME_WIN_USER_CANCEL;
		}
		else {
			main_work->proc_input = NULL;
			main_work->proc_win_input = NULL;
			
			// タイトル側へ戻る(掃け演出開始)
			main_work->proc_menu_update = dmUserNameProcWindowOutEfct;
			
			// ウインドウ演出時間設定
			main_work->win_timer = DMD_USERNAME_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_USERNAME_DISP_FLAG_MSG_WIN_ACT;
			
			// 文字入力ウインドウのアクションを非表示
			main_work->disp_flag &= ~DMD_USERNAME_DISP_FLAG_WIN_ACT;
		}
		
		// フラグOFF
		main_work->flag &= ~DMD_USERNAME_FLAG_DECIDE;
		main_work->flag &= ~DMD_USERNAME_FLAG_CANCEL;
		
		// メインメニューへイベント遷移先設定
		main_work->next_evt = DME_USERNAME_NEXT_EVT_MAINMENU;
		
		DmSoundPlaySE("Cancel");
		
		return;
	}
	
	// 名前入力モードへ戻る
	if (main_work->flag & DMD_USERNAME_FLAG_UP_CHNG_CRSR
		|| main_work->flag & DMD_USERNAME_FLAG_DOWN_CHNG_CRSR) {
		
		// 上方向を押した場合のみ、下側へ回りこみ
		if (main_work->flag & DMD_USERNAME_FLAG_UP_CHNG_CRSR) {
			main_work->crsr_focus[1] = (f32)dmUserNameGetRevisedCharCrsrNo((s32)main_work->crsr_focus[1]
																		  , -1, 1
																		  , (s32)main_work->crsr_focus[1]
																		  , (s32)main_work->crsr_focus[0]);
			
			main_work->cur_char = (s32)(main_work->crsr_focus[0] + main_work->crsr_focus[1] * 10);
		}
		
		main_work->proc_menu_update = dmUserNameProcWaitInput;
		
		main_work->proc_input = dmUserNameInputProcCharSlct;
		
		main_work->state = DME_USERNAME_INPUT_STATE_NAME_INPUT;
		
		// 名前入力側のカーソルを表示ON
		main_work->disp_flag |= DMD_USERNAME_DISP_FLAG_ACT_CRSR;
		
		// フラグOFF
		main_work->flag &= ~DMD_USERNAME_FLAG_UP_CHNG_CRSR;
		main_work->flag &= ~DMD_USERNAME_FLAG_DOWN_CHNG_CRSR;
		main_work->flag &= ~DMD_USERNAME_FLAG_LEFT_CHNG_CRSR;
		main_work->flag &= ~DMD_USERNAME_FLAG_RIGHT_CHNG_CRSR;
		
		DmSoundPlaySE("Cursol");
		
		return;
	}
	
	// カーソル位置切り替え
	if (main_work->flag & DMD_USERNAME_FLAG_LEFT_CHNG_CRSR
		|| main_work->flag & DMD_USERNAME_FLAG_RIGHT_CHNG_CRSR) {
		
		dmUserNameSetChngFocusNamePos(main_work);
		main_work->timer = 0;
		
		// フラグOFF
		main_work->flag &= ~DMD_USERNAME_FLAG_UP_CHNG_CRSR;
		main_work->flag &= ~DMD_USERNAME_FLAG_DOWN_CHNG_CRSR;
		main_work->flag &= ~DMD_USERNAME_FLAG_LEFT_CHNG_CRSR;
		main_work->flag &= ~DMD_USERNAME_FLAG_RIGHT_CHNG_CRSR;
		
		DmSoundPlaySE("Cursol");
		
		return;
	}
}



// ==========================================================================
// dmUserNameProcSlctCharDecideEfct
/*!
	ユーザー名設定時の選択文字決定演出中処理
 */
// ==========================================================================
void dmUserNameProcSlctCharDecideEfct(DMS_USERNAME_MAIN_WORK *main_work)
{
	// 入り演出が終了したら
	if (dmUserNameIsFileTabDecideEfctEnd(main_work)) {
		// ※ここでウインドウ閉じる演出を入れるか入れないかを分岐する
		main_work->proc_menu_update = dmUserNameProcWindowOutEfct;
		
//		main_work->announce_flag |= 1 << DME_USERNAME_WIN_DECIDE_NAME;
		// 文字入力ウインドウのアクションを非表示
		main_work->disp_flag &= ~DMD_USERNAME_DISP_FLAG_WIN_ACT;
		
		// ウインドウ演出時間設定
		main_work->win_timer = DMD_USERNAME_WIN_EFCT_TIME;
		
		main_work->proc_input = NULL;
		
		main_work->timer = 0.0f;
		
		return;
	}
	
	// 決定項目以外の掃け演出処理
	dmUserNameSetFileTabDecideEfct(main_work);

	main_work->timer++;
	
	// ボタン押した際の演出更新
	for (u32 i = 0; i < 5; i++) {
		if (main_work->flag & (1 << (5 + i))) {
			dmUserNameSetBtnPushEfct(main_work, i);
		}
	}
}


// ==========================================================================
// dmUserNameProcDataSave
/*!
	データセーブ処理
 */
// ==========================================================================
void dmUserNameProcDataSave(DMS_USERNAME_MAIN_WORK *main_work)
{
#if _WII
	// ここで名前をASCIIコードに変換
	char temp[16];
	GsCharCodeConvStringGameToAscii(
		temp, 16,
		(u8 *)&main_work->name_string[0], 10);
	gs::backup::SOption::CreateInstance().SetName(temp);
#else
	UNREFERENCED_PARAMETER(main_work);
#endif
}




// ==========================================================================
// dmUserNameProcWindowOutEfct
/*!
	ウインドウクローズ処理
 */
// ==========================================================================
void dmUserNameProcWindowOutEfct(DMS_USERNAME_MAIN_WORK *main_work)
{
	// ウインドウ演出
	if (main_work->flag & DMD_USERNAME_FLAG_WIN_EFCT_END) {
		// イベント遷移
		main_work->proc_menu_update = dmUserNameProcStopDraw;
		main_work->proc_draw = NULL;
		
		main_work->proc_input = NULL;
		
		main_work->flag &= ~DMD_USERNAME_FLAG_WIN_EFCT_END;
	}
	else {
		dmUserNameSetWinCloseEfct(main_work);
	}
}




// ==========================================================================
// dmUserNameProcStopDraw
/*!
	描画停止処理
 */
// ==========================================================================
void dmUserNameProcStopDraw(DMS_USERNAME_MAIN_WORK *main_work)
{
	main_work->proc_menu_update = dmUserNameProcFinish;
}



// ==========================================================================
// dmUserNameProcFinish
/*!
	終了処理
 */
// ==========================================================================
void dmUserNameProcFinish(DMS_USERNAME_MAIN_WORK *main_work)
{
	// アクション解放
	for (int i = 0; i < ACT_NUM; i++) {
		if (main_work->act[i]) {
			AoActDelete(main_work->act[i]);
			main_work->act[i] = NULL;
		}
	}
	if (main_work->node_act) {
		AoActDelete(main_work->node_act);
		main_work->node_act = NULL;
	}
	
	// 終了処理へ
	main_work->flag |= DMD_USERNAME_FLAG_EXIT;
	main_work->proc_win_update = NULL;
	main_work->proc_menu_update = NULL;
}



// ==========================================================================
// dmUserNameProcWindowNodispIdle
/*!
	ウインドウ非表示待ち中処理
 */
// ==========================================================================
void dmUserNameProcWindowNodispIdle(DMS_USERNAME_MAIN_WORK *main_work)
{
	// メニュー遷移フラグONならば
	if (main_work->announce_flag) {
		main_work->proc_win_update = dmUserNameProcWindowOpenEfct;

		// 通常処理の入力処理をなくす(二重入力を防ぐため)
//		main_work->proc_input = NULL;

		// ウインドウ開閉演出時は入力処理なし
		main_work->proc_win_input = NULL;

		// ウインドウ演出用タイマー初期化
		main_work->win_timer = 0;

		// ウインドウ選択変数設定
		for (u32 i = DME_USERNAME_WIN_PLEASE_SET_NAME; i < DME_USERNAME_WIN_NUM; i++) {
			if (main_work->announce_flag & 1 << i) {
				main_work->win_mode = (s32)i;
				break;
			}
		}
		
		DmSoundPlaySE("Window");

		// ウインドウ演出中フラグON
		main_work->disp_flag |= DMD_USERNAME_DISP_FLAG_WIN_DRAW;
	}
	
	
	if (main_work->disp_flag & DMD_USERNAME_DISP_FLAG_WIN_DRAW) {
		main_work->disp_flag &= ~DMD_USERNAME_DISP_FLAG_WIN_DRAW;
	}
}




// ==========================================================================
// dmUserNameProcWindowOpenEfct
/*!
	ウインドウオープン中処理
 */
// ==========================================================================
void dmUserNameProcWindowOpenEfct(DMS_USERNAME_MAIN_WORK *main_work)
{
	// 演出終了チェック
	if (main_work->flag & DMD_USERNAME_FLAG_MSG_WIN_EFCT_END) {
		// ウインドウのプロシージャ設定
		main_work->proc_win_update = dmUserNameProcWindowAnnounceIdle;
		
		if (main_work->win_mode == DME_USERNAME_WIN_USER_CANCEL) {
			main_work->proc_win_input = dmUserNameInputProcWinYesNo;
			
			// デフォルトいいえ設定
			main_work->win_cur_slct = 1;
		}
		else {
			main_work->proc_win_input = dmUserNameInputProcWinOk;
		}
		
		// ウインドウ内アクション表示フラグON
		main_work->disp_flag |= DMD_USERNAME_DISP_FLAG_MSG_WIN_ACT;
		
		// ウインドウ演出中フラグOFF
		main_work->flag &= ~DMD_USERNAME_FLAG_MSG_WIN_EFCT_END;
	}
	else {
		// ウインドウオープン演出処理
		dmUserNameSetMsgWinOpenEfct(main_work);
	}
	
	
	if (!(main_work->disp_flag & DMD_USERNAME_DISP_FLAG_WIN_DRAW)) {
		main_work->disp_flag |= DMD_USERNAME_DISP_FLAG_WIN_DRAW;
	}
}



// ==========================================================================
// dmUserNameProcWindowAnnounceIdle
/*!
	ウインドウ入力待ち処理
 */
// ==========================================================================
void dmUserNameProcWindowAnnounceIdle(DMS_USERNAME_MAIN_WORK *main_work)
{
	// タイマー更新
	main_work->wait_timer++;
	
	// ウインドウ入力処理
	if (main_work->proc_win_input) {
		main_work->proc_win_input(main_work);
	}

	// ウインドウのパターン分の処理をここに記述
	if (main_work->win_mode == DME_USERNAME_WIN_PLEASE_SET_NAME) {
		if (main_work->flag & DMD_USERNAME_FLAG_DECIDE) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
//			main_work->proc_input = NULL;
			
			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;
			
			// ウインドウ演出時間設定
			main_work->win_timer = DMD_USERNAME_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_USERNAME_DISP_FLAG_MSG_WIN_ACT;
			
			main_work->proc_win_update = dmUserNameProcWindowCloseEfct;
			
			DmSoundPlaySE("Ok");
			
			main_work->wait_timer = 0;
			
			// フラグOFF
			main_work->flag &= ~DMD_USERNAME_FLAG_DECIDE;
			main_work->flag &= ~DMD_USERNAME_FLAG_CANCEL;
		}
	}
	
	
	else if (main_work->win_mode == DME_USERNAME_WIN_USER_CANCEL) {
		// キャンセルならば
		if (main_work->flag & DMD_USERNAME_FLAG_CANCEL
			|| (main_work->flag & DMD_USERNAME_FLAG_DECIDE
				&& main_work->win_cur_slct == 1)) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
//			main_work->proc_input = NULL;
			
			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;
			
			// シーケンスを入力へ戻す
			if (main_work->state == DME_USERNAME_INPUT_STATE_NAME_INPUT) {
				main_work->proc_menu_update = dmUserNameProcWaitInput;
			}
			else {
				main_work->proc_menu_update = dmUserNameProcChngFocusNamePos;
			}
			
			// ウインドウ演出時間設定
			main_work->win_timer = DMD_USERNAME_WIN_EFCT_TIME;
			
			main_work->flag |= DMD_USERNAME_FLAG_MSG_BACK_INPUT;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_USERNAME_DISP_FLAG_MSG_WIN_ACT;
			
			main_work->proc_win_update = dmUserNameProcWindowCloseEfct;
			
			if (main_work->flag & DMD_USERNAME_FLAG_CANCEL) {
				DmSoundPlaySE("Cancel");
			}
			else {
				DmSoundPlaySE("Ok");
			}
			
			main_work->wait_timer = 0;
			
			// フラグOFF
			main_work->flag &= ~DMD_USERNAME_FLAG_DECIDE;
			main_work->flag &= ~DMD_USERNAME_FLAG_CANCEL;
		}
		
		// 決定ならば
		if (main_work->flag & DMD_USERNAME_FLAG_DECIDE
			&& main_work->win_cur_slct == 0) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
//			main_work->proc_input = NULL;
			
			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;
			
			// タイトル側へ戻る(掃け演出開始)
			main_work->proc_menu_update = dmUserNameProcWindowOutEfct;
			
			// ウインドウ演出時間設定
			main_work->win_timer = DMD_USERNAME_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_USERNAME_DISP_FLAG_MSG_WIN_ACT;
			
			main_work->proc_win_update = dmUserNameProcWindowCloseEfct;
			
			DmSoundPlaySE("Ok");
			
			main_work->flag &= ~DMD_USERNAME_FLAG_DECIDE;
			
			main_work->wait_timer = 0;
			
			// フラグOFF
			main_work->flag &= ~DMD_USERNAME_FLAG_DECIDE;
			main_work->flag &= ~DMD_USERNAME_FLAG_CANCEL;
		}
		
	}
	
	else if (main_work->win_mode == DME_USERNAME_WIN_DECIDE_NAME) {
		if (main_work->flag & DMD_USERNAME_FLAG_DECIDE) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
//			main_work->proc_input = NULL;
			
			// ウインドウ開閉演出時は入力処理なし
			main_work->proc_win_input = NULL;
			
			// ウインドウ演出時間設定
			main_work->win_timer = DMD_USERNAME_WIN_EFCT_TIME;
			main_work->flag &= ~DMD_USERNAME_FLAG_DECIDE;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_USERNAME_DISP_FLAG_MSG_WIN_ACT;
			
			DmSoundPlaySE("Ok");
			
			main_work->wait_timer = 0;
			
			main_work->proc_win_update = dmUserNameProcWindowCloseEfct;
		}
	}
	
	if (!(main_work->disp_flag & DMD_USERNAME_DISP_FLAG_WIN_DRAW)) {
		main_work->disp_flag |= DMD_USERNAME_DISP_FLAG_WIN_DRAW;
	}
}



// ==========================================================================
// dmUserNameProcWindowCloseEfct
/*!
	ウインドウクローズ中処理
 */
// ==========================================================================
void dmUserNameProcWindowCloseEfct(DMS_USERNAME_MAIN_WORK *main_work)
{
	if (!(main_work->disp_flag & DMD_USERNAME_DISP_FLAG_WIN_DRAW)) {
		main_work->disp_flag |= DMD_USERNAME_DISP_FLAG_WIN_DRAW;
	}
	
	// 演出終了チェック
	if (main_work->flag & DMD_USERNAME_FLAG_MSG_WIN_EFCT_END) {
		// ウインドウのプロシージャ設定
		main_work->proc_win_update = dmUserNameProcWindowNodispIdle;
		
		if (main_work->win_mode == DME_USERNAME_WIN_USER_CANCEL
			|| main_work->win_mode == DME_USERNAME_WIN_DECIDE_NAME) {
			// ウインドウ演出時間設定
			main_work->win_timer = DMD_USERNAME_WIN_EFCT_TIME;
			
			if (!(main_work->flag & DMD_USERNAME_FLAG_MSG_BACK_INPUT)) {
				// 文字入力ウインドウのアクションを非表示
				main_work->disp_flag &= ~DMD_USERNAME_DISP_FLAG_WIN_ACT;
			}
			else {
				main_work->flag &= ~DMD_USERNAME_FLAG_MSG_BACK_INPUT;
			}
		}
		
		// アナウンス分のフラグOFF
		main_work->announce_flag &= ~(1 << main_work->win_mode);
		
		if (main_work->disp_flag & DMD_USERNAME_DISP_FLAG_WIN_DRAW) {
			main_work->disp_flag &= ~DMD_USERNAME_DISP_FLAG_WIN_DRAW;
		}
		
		// ウインドウ演出中フラグOFF
		main_work->flag &= ~DMD_USERNAME_FLAG_MSG_WIN_EFCT_END;
	}
	
	// ウインドウクローズ演出処理
	else {
		dmUserNameSetMsgWinCloseEfct(main_work);
	}
}



// ==========================================================================
// dmUserNameInputProcCharSlct
/*!
	名前選択時入力プロシージャ処理
 */
// ==========================================================================
void dmUserNameInputProcCharSlct(DMS_USERNAME_MAIN_WORK *main_work)
{
	s32 rpt_width_max = 0;
	s32 rpt_height_max = 0;
	
	
	if (main_work->crsr_focus[1] == 5) {
		rpt_width_max = 8;
	}
	
	else if (main_work->crsr_focus[1] == 4) {
		rpt_width_max = 9;
	}
	
	else {
		rpt_width_max = 10;
	}
	
	if (main_work->crsr_focus[0] == 6) {
		rpt_height_max = 5;
	}
	else {
		rpt_height_max = 6;
	}
	
	// キャンセル処理
	if (AoPadRepeat() & GSD_KEY_CANCEL) {
		main_work->flag |= DMD_USERNAME_FLAG_CANCEL;

		return;
	}
	
	// 決定処理
	if (AoPadRepeat() & GSD_KEY_DECIDE) {
		main_work->flag |= DMD_USERNAME_FLAG_DECIDE;

		return;
	}

	// 十字キー操作
	if (AoPadMRepeat() & GSD_KEY_UP) {
		if (AoPadMStand() & GSD_KEY_UP
			|| main_work->crsr_focus[1] != 0) {
			
			main_work->flag |= DMD_USERNAME_FLAG_UP_CHNG_CRSR;
		}

		return;
	}
	
	else if (AoPadMRepeat() & GSD_KEY_DOWN) {
		if (AoPadMStand() & GSD_KEY_DOWN
			|| main_work->crsr_focus[1] != rpt_height_max - 1) {

			main_work->flag |= DMD_USERNAME_FLAG_DOWN_CHNG_CRSR;
		}

		return;
	}
	
	else if (AoPadMRepeat() & GSD_KEY_LEFT) {
		if (AoPadMStand() & GSD_KEY_LEFT
			|| main_work->crsr_focus[0] != 0) {

			main_work->flag |= DMD_USERNAME_FLAG_LEFT_CHNG_CRSR;
		}

		return;
	}
	
	else if (AoPadMRepeat() & GSD_KEY_RIGHT) {
		if (AoPadMStand() & GSD_KEY_RIGHT
			|| main_work->crsr_focus[0] != rpt_width_max - 1) {

			main_work->flag |= DMD_USERNAME_FLAG_RIGHT_CHNG_CRSR;
		}

		return;
	}
	
}



// ==========================================================================
// dmUserNameInputProcChngFocus
/*!
	名前選択時上段FOCUS文字切り替え入力プロシージャ処理
 */
// ==========================================================================
void dmUserNameInputProcChngFocus(DMS_USERNAME_MAIN_WORK *main_work)
{
	s32 rpt_width_max = 0;
	
	// ここで文字列の空文字までの配列番号を取得
	for (int i = 0; i < DMD_USERNAME_CHAR_NUM_MAX; i++) {
		rpt_width_max = i;
		
		if (main_work->name_string[i] == 0) {
			break;
		}
	}
	
	// キャンセル処理
	if (AoPadRepeat() & GSD_KEY_CANCEL) {
		main_work->flag |= DMD_USERNAME_FLAG_CANCEL;

		return;
	}
	
	// 十字キー操作
	if (AoPadMStand() & GSD_KEY_UP) {
		main_work->flag |= DMD_USERNAME_FLAG_UP_CHNG_CRSR;
		return;
	}
	
	else if (AoPadMStand() & GSD_KEY_DOWN) {
		main_work->flag |= DMD_USERNAME_FLAG_DOWN_CHNG_CRSR;
		return;
	}
	
	else if (AoPadMRepeat() & GSD_KEY_LEFT) {
		if (AoPadMStand() & GSD_KEY_LEFT
			|| main_work->focus_name_no != 0) {

			main_work->flag |= DMD_USERNAME_FLAG_LEFT_CHNG_CRSR;
		}

		return;
	}
	
	else if (AoPadMRepeat() & GSD_KEY_RIGHT) {
		if (AoPadMStand() & GSD_KEY_RIGHT
			|| main_work->focus_name_no != rpt_width_max) {

			main_work->flag |= DMD_USERNAME_FLAG_RIGHT_CHNG_CRSR;
		}

		return;
	}
}



// ==========================================================================
// dmUserNameInputProcWinYesNo
/*!
	ウインドウ非表示用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmUserNameInputProcWinYesNo(DMS_USERNAME_MAIN_WORK *main_work)
{
	// キャンセル判定
	if (AoPadStand() & GSD_KEY_CANCEL) {
		// フラグON
		main_work->flag |= DMD_USERNAME_FLAG_CANCEL;
	}
	
	// 決定判定
	if (AoPadStand() & GSD_KEY_DECIDE) {
		// フラグON
		main_work->flag |= DMD_USERNAME_FLAG_DECIDE;
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
}



// ==========================================================================
// dmUserNameInputProcWinOk
/*!
	ウインドウ非表示用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmUserNameInputProcWinOk(DMS_USERNAME_MAIN_WORK *main_work)
{
	// 決定・キャンセル判定
	if ((AoPadStand() & GSD_KEY_DECIDE)
		|| (AoPadStand() & GSD_KEY_CANCEL)) {
		// フラグOFF
		main_work->flag |= DMD_USERNAME_FLAG_DECIDE;
	}
}



// ==========================================================================
// dmUserNameProcActDraw
/*!
	描画設定プロシージャ処理
 */
// ==========================================================================
void dmUserNameProcActDraw(DMS_USERNAME_MAIN_WORK *main_work)
{
	// 共通描画処理は描画時は常に設定
	dmUserNameCommonDraw(main_work);

	if (main_work->disp_flag & DMD_USERNAME_DISP_FLAG_WIN_ACT) {
		// FIX関連描画設定
		dmUserNameCommonFixDraw(main_work);

		// セーブファイル描画設定
		dmUserNameCharDraw(main_work);
	}
	
	if (main_work->disp_flag & DMD_USERNAME_DISP_FLAG_WIN_DRAW) {
		dmUserNameWinMsgDraw(main_work);
	}
	
	// 描画タスク生成
	if (main_work->draw_state) {
//		amDrawMakeTask(dmUserNameTaskDraw, (u16)0x8000, (u32)0);
	}
}


// ==========================================================================
// dmUserNameCommonDraw
/*!
	共通描画設定処理
 */
// ==========================================================================
void dmUserNameCommonDraw(DMS_USERNAME_MAIN_WORK *main_work)
{
	// ウインドウ台紙描画
	AoWinSysDrawState(AOD_WIN_TYPE_A
					 , AoTexGetTexList(&dm_username_tex[0])
					 , 0
					 , DMD_USERNAME_SIZE_HALF_WIDTH			// ウインドウ中心X
					 , DMD_USERNAME_SIZE_HALF_HEIGHT - 2.f		// ウインドウ中心Y
					 , DMD_USERNAME_WINDOW_SIZE_W * main_work->win_size_rate[0]			// ウインドウ横サイズ
					 , DMD_USERNAME_WINDOW_SIZE_H * main_work->win_size_rate[1]			// ウインドウ縦サイズ
					 , dm_username_draw_state				// 描画STATE
					 );
	
//	AoWinSysDrawTask(AOD_WIN_TYPE_A
//					 , AoTexGetTexList(&dm_username_tex[0])
//					 , 0
//					 , DMD_USERNAME_SIZE_HALF_WIDTH			// ウインドウ中心X
//					 , DMD_USERNAME_SIZE_HALF_HEIGHT + 24.f		// ウインドウ中心Y
//					 , DMD_USERNAME_WINDOW_SIZE_W * main_work->win_size_rate[0]			// ウインドウ横サイズ
//					 , DMD_USERNAME_WINDOW_SIZE_H * main_work->win_size_rate[1]			// ウインドウ縦サイズ
//					 , DMD_USERNAME_DRAW_PRIO_BG			// 描画タスク優先度
//					 );
}



// ==========================================================================
// dmUserNameCommonFixDraw
/*!
	FIX描画設定処理
 */
// ==========================================================================
void dmUserNameCommonFixDraw(DMS_USERNAME_MAIN_WORK *main_work)
{
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_USERNAME_DRAW_PRIO_FIX);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&dm_username_tex[0]));

	// アクション登録(登録は全てここで行うようにし、実際の登録するかはフラグにて設定するようにする)
	for (int i = ACT_FIX_START; i <= ACT_FIX_END; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	AoActSortRegAction(main_work->node_act);
	
	if (main_work->cur_char == 50
		|| main_work->cur_char == 51) {
		if (main_work->flag & DMD_USERNAME_FLAG_PUSH_BS_BTN) {
			AoActSetFrame(main_work->act[ACT_BTN_BS], 2.f);
		}
		else {
			AoActSetFrame(main_work->act[ACT_BTN_BS], 1.f);
		}
	}
	else {
		AoActSetFrame(main_work->act[ACT_BTN_BS], 0.f);
	}

	
	if (main_work->cur_char == 52
		|| main_work->cur_char == 53) {
		if (main_work->flag & DMD_USERNAME_FLAG_PUSH_RESIZE_BTN) {
			AoActSetFrame(main_work->act[ACT_BTN_CAP], 2.f);
		}
		else {
			AoActSetFrame(main_work->act[ACT_BTN_CAP], 1.f);
		}
	}
	else {
		AoActSetFrame(main_work->act[ACT_BTN_CAP], 0.f);
	}

	
	if (main_work->cur_char == 54
		|| main_work->cur_char == 55) {
		if (main_work->flag & DMD_USERNAME_FLAG_PUSH_SPACE_BTN) {
			AoActSetFrame(main_work->act[ACT_BTN_SPACE], 2.f);
		}
		else {
			AoActSetFrame(main_work->act[ACT_BTN_SPACE], 1.f);
		}
	}
	else {
		AoActSetFrame(main_work->act[ACT_BTN_SPACE], 0.f);
	}

	
	if (main_work->cur_char >= 57
		&& main_work->cur_char <= 59) {
		if (main_work->flag & DMD_USERNAME_FLAG_PUSH_DECIDE_BTN) {
			AoActSetFrame(main_work->act[ACT_BTN_KETTEI_LEFT], 2.f);
			AoActSetFrame(main_work->act[ACT_BTN_KETTEI_RIGHT], 2.f);
		}
		else {
			AoActSetFrame(main_work->act[ACT_BTN_KETTEI_LEFT], 1.f);
			AoActSetFrame(main_work->act[ACT_BTN_KETTEI_RIGHT], 1.f);
		}
	}
	else {
		AoActSetFrame(main_work->act[ACT_BTN_KETTEI_LEFT], 0.f);
		AoActSetFrame(main_work->act[ACT_BTN_KETTEI_RIGHT], 0.f);
	}

	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&dm_username_tex[0]));

	// 固定物の更新
	// アクション登録(登録は全てここで行うようにし、実際の登録するかはフラグにて設定するようにする)
	for (int i = ACT_FIX_START; i <= ACT_FIX_END; i++) {
		AoActUpdate(main_work->act[i], 0.f);
	}

	AoActUpdate(main_work->node_act, 0.f);
	
	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();
}



// ==========================================================================
// dmUserNameCharDraw
/*!
	文字描画設定処理
 */
// ==========================================================================
void dmUserNameCharDraw(DMS_USERNAME_MAIN_WORK *main_work)
{
	float set_frame = 0.f;
	float scale_crsr_pos = 0;
	
	dmUserNameSetCrsrChangeData(main_work);
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_USERNAME_DRAW_PRIO_CHAR);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&dm_username_tex[0]));
	
	for (int i = ACT_SLCT_CHAR_START; i <= ACT_SLCT_CHAR_END; i++) {
		
		AoActSortRegAction(main_work->act[i]);
	}
	
	
	for (int i = ACT_CHAR_1; i <= ACT_CHAR_0; i++) {
		if (main_work->cur_char == i - ACT_CHAR_1
			&& main_work->disp_flag & DMD_USERNAME_DISP_FLAG_ACT_CRSR) {
			set_frame = 1.f;
		}
		else {
			set_frame = 0.f;
		}
		AoActSetFrame(main_work->act[i], set_frame);
	}
	
	for (int i = ACT_CHNG_CHAR_START; i <= ACT_CHNG_CHAR_END; i++) {
		
		if (main_work->cur_char == i - ACT_CHAR_1) {
			if (main_work->is_char_small) {
				set_frame = 3.f;
			}
			else {
				set_frame = 1.f;
			}
		}
		else {
			if (main_work->is_char_small) {
				set_frame = 2.f;
			}
			else {
				set_frame = 0.f;
			}
		}
		
		AoActSetFrame(main_work->act[i], set_frame);
	}
	
	for (int i = ACT_CHAR_KIGO01; i <= ACT_CHAR_KIGO13; i++) {
		if (main_work->cur_char == i - ACT_CHAR_1) {
			set_frame = 1.f;
		}
		else {
			set_frame = 0.f;
		}
		
		AoActSetFrame(main_work->act[i], set_frame);
	}
	
	
	AoActSetTexture(AoTexGetTexList(&dm_username_tex[0]));
	
	for (int i = ACT_SLCT_CHAR_START; i <= ACT_SLCT_CHAR_END; i++) {
		AoActUpdate(main_work->act[i], 0.f);
	}
	
	// ソート実行
	AoActSortExecute();
	
	// ソートバッファ描画
	AoActSortDraw();
	
	// ソートバッファ全解除
	AoActSortUnregAll();

	
	
	
//	AoActSysSetDrawTaskPrio(DMD_USERNAME_DRAW_PRIO_NAME_CHAR);
	
	AoActSetTexture(AoTexGetTexList(&dm_username_tex[0]));
	
	for (int i = ACT_CHAR_ALP_1; i <= ACT_CHAR_ALP_10; i++) {
//		AoActSetFrame(main_work->act[i], (f32)(main_work->name_string[i - ACT_CHAR_ALP_1]));
	}
	
	for (int i = ACT_SAVE_CHAR_START; i <= ACT_SAVE_CHAR_END; i++) {
		
		AoActSortRegAction(main_work->act[i]);
		AoActSetFrame(main_work->act[i], (f32)(main_work->name_string[i - ACT_CHAR_ALP_1]));
		AoActUpdate(main_work->act[i], 0.0f);
	}
	
	// ソート実行
	AoActSortExecute();
	
	// ソートバッファ描画
	AoActSortDraw();
	
	// ソートバッファ全解除
	AoActSortUnregAll();

	
	AoActSetTexture(AoTexGetTexList(&dm_username_tex[1]));
	
	// バックスペース
	if (main_work->cur_char == 50 || main_work->cur_char == 51) {
		AoActSetFrame(main_work->act[ACT_TEX_BS], 1.f);
	}
	else {
		AoActSetFrame(main_work->act[ACT_TEX_BS], 0.f);
	}
	
	// 大文字小文字切り替え
	if (main_work->cur_char == 52 || main_work->cur_char == 53) {
		AoActSetFrame(main_work->act[ACT_TEX_CAP], 1.f);
	}
	else {
		AoActSetFrame(main_work->act[ACT_TEX_CAP], 0.f);
	}
	
	// スペース
	if (main_work->cur_char == 54 || main_work->cur_char == 55) {
		AoActSetFrame(main_work->act[ACT_TEX_SPACE], 1.f);
	}
	else {
		AoActSetFrame(main_work->act[ACT_TEX_SPACE], 0.f);
	}
	
	// 決定
	if (main_work->cur_char == 57 || main_work->cur_char == 58 || main_work->cur_char == 59) {
		AoActSetFrame(main_work->act[ACT_TEX_KETTEI], 1.f);
	}
	else {
		AoActSetFrame(main_work->act[ACT_TEX_KETTEI], 0.f);
	}
	for (int i = ACT_TEX_BS; i <= ACT_TEX_KETTEI; i++) {
		
		AoActSetTexture(AoTexGetTexList(&dm_username_tex[1]));
		
		AoActSortRegAction(main_work->act[i]);
		AoActUpdate(main_work->act[i], 0.0f);
	}
	
	// ソート実行
	AoActSortExecute();
	
	// ソートバッファ描画
	AoActSortDraw();
	
	// ソートバッファ全解除
	AoActSortUnregAll();
	
	
	
	AoActSetTexture(AoTexGetTexList(&dm_username_tex[0]));
	// カーソル描画設定
	for (int i = 0; i < 3; i++) {
		if (main_work->disp_flag & DMD_USERNAME_DISP_FLAG_ACT_CRSR) {
			AoActSortRegAction(main_work->act[ACT_CUR_CRSR1 + i]);
			
			// ACCUMURATEでトランスさせる
			AoActAcmPush();
			
			AoActAcmInit();
			
			if (i == 1) {
				AoActAcmApplyScale(main_work->crsr_scale
								   , 1.0f
								   );
			}
			
			if (main_work->crsr_scale > 1.f) {
				if (i == 0) {
					scale_crsr_pos = (main_work->crsr_scale * 10.f - 11.f) * -1.f;
				}
				else if (i == 2) {
					scale_crsr_pos = main_work->crsr_scale * 10.f - 11.f;
				}
				else {
					scale_crsr_pos = 0.f;
				}
			}
			else {
				scale_crsr_pos = 0.f;
			}
			
			AoActAcmApplyTrans(main_work->crsr_pos[0] + scale_crsr_pos
							   , main_work->crsr_pos[1]
							   , 0
							   );

			// フレーム更新はSetFrameのみで行う
			AoActUpdate(main_work->act[ACT_CUR_CRSR1 + i], 0.0f);

			AoActAcmPop();
		}
	}
	
	
	AoActSetTexture(AoTexGetTexList(&dm_username_tex[0]));
	
	AoActSortRegAction(main_work->act[ACT_FRAME_NAME_CRSR]);
	
	// ACCUMURATEでトランスさせる
	AoActAcmPush();
	
	AoActAcmInit();
	
	AoActAcmApplyTrans(main_work->focus_name_no * DMD_USERNAME_FOCUS_NAME_CHAR_DIST
					   , 0
					   , 0
					   );

	// フレーム更新はSetFrameのみで行う
	AoActUpdate(main_work->act[ACT_FRAME_NAME_CRSR], 0.0f);

	AoActAcmPop();
	
	
	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();

	
	
}



// ==========================================================================
// dmUserNameWinMsgDraw
/*!
	文字描画設定処理
 */
// ==========================================================================
void dmUserNameWinMsgDraw(DMS_USERNAME_MAIN_WORK *main_work)
{
	f32 tmp_win_size[2] = {0.f, 0.f};
	
	// ウインドウ用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_USERNAME_DRAW_PRIO_MSG_WIN_FIX);
	
#if _WII
	tmp_win_size[0] = DMD_USERNAME_MSG_WIN_SIZE_W * DMD_USERNAME_DISP_SCALE_TEXT;
	tmp_win_size[1] = DMD_USERNAME_MSG_WIN_SIZE_H * DMD_USERNAME_DISP_SCALE_TEXT;
#else
	tmp_win_size[0] = DMD_USERNAME_MSG_WIN_SIZE_W;
	tmp_win_size[1] = DMD_USERNAME_MSG_WIN_SIZE_H;
#endif
	
	// ウインドウ描画
	if (main_work->draw_state) {
		AoWinSysDrawState(AOD_WIN_TYPE_A
						 , AoTexGetTexList(dm_username_cmn_tex[1])
						 , 0
						 , DMD_USERNAME_SIZE_WIDTH / 2.0f		// ウインドウ中心X
						 , DMD_USERNAME_SIZE_HEIGHT / 2.0f		// ウインドウ中心Y
						 , tmp_win_size[0] * main_work->msg_win_size_rate[0]
						 , tmp_win_size[1] * main_work->msg_win_size_rate[1]
						 , dm_username_draw_state				// 描画STATE
						 );
	}
	else {
		AoWinSysDrawTask(AOD_WIN_TYPE_A
						 , AoTexGetTexList(dm_username_cmn_tex[1])
						 , 0
						 , DMD_USERNAME_SIZE_WIDTH / 2.0f		// ウインドウ中心X
						 , DMD_USERNAME_SIZE_HEIGHT / 2.0f		// ウインドウ中心Y
						 , tmp_win_size[0] * main_work->msg_win_size_rate[0]			// ウインドウ横サイズ
						 , tmp_win_size[1] * main_work->msg_win_size_rate[1]			// ウインドウ縦サイズ
						 , DMD_USERNAME_DRAW_PRIO_MSG_WIN			// 描画タスク優先度
						 );
	}
	
	// ウインドウ内の項目描画
	if (main_work->disp_flag & DMD_USERNAME_DISP_FLAG_MSG_WIN_ACT) {		// ウインドウが表示しきっているならば
		switch (main_work->win_mode) {
		case DME_USERNAME_WIN_PLEASE_SET_NAME:
			// アクション登録
			AoActSetTexture(AoTexGetTexList(&dm_username_tex[DME_USERNAME_DATA_TYPE_LANG_DATA]));
			AoActSortRegAction(main_work->act[ACT_TEX_WIN_MSG2]);
			
			AoActSetTexture(AoTexGetTexList(dm_username_cmn_tex[2]));
			AoActSortRegAction(main_work->act[ACT_TEX_OK]);
			
			// 初期表示フレーム設定
			AoActSetFrame(main_work->act[ACT_TEX_OK], 0.f);
			
			break;
			
		case DME_USERNAME_WIN_USER_CANCEL:
			AoActSetTexture(AoTexGetTexList(&dm_username_tex[DME_USERNAME_DATA_TYPE_LANG_DATA]));
			AoActSortRegAction(main_work->act[ACT_TEX_WIN_MSG2]);
			
			AoActSetTexture(AoTexGetTexList(dm_username_cmn_tex[2]));
			AoActSortRegAction(main_work->act[ACT_TEX_YES]);
			AoActSortRegAction(main_work->act[ACT_TEX_NO]);
			AoActSortRegAction(main_work->act[ACT_TEX_BACK_WIN]);
			
			AoActSetTexture(AoTexGetTexList(dm_username_cmn_tex[0]));
			AoActSortRegAction(main_work->act[ACT_BTN_CANCEL_WIN]);
			
			// 初期表示フレーム設定
			if (main_work->win_cur_slct) {
				AoActSetFrame(main_work->act[ACT_TEX_YES], 1.f);
				AoActSetFrame(main_work->act[ACT_TEX_NO], 0.f);
			}
			else {
				AoActSetFrame(main_work->act[ACT_TEX_YES], 0.f);
				AoActSetFrame(main_work->act[ACT_TEX_NO], 1.f);
			}
			
			break;
			
		case DME_USERNAME_WIN_DECIDE_NAME:
			AoActSetTexture(AoTexGetTexList(&dm_username_tex[DME_USERNAME_DATA_TYPE_LANG_DATA]));
			AoActSortRegAction(main_work->act[ACT_TEX_WIN_MSG2]);
			
			AoActSetTexture(AoTexGetTexList(dm_username_cmn_tex[2]));
			AoActSortRegAction(main_work->act[ACT_TEX_OK]);
			
			// 初期表示フレーム設定
			AoActSetFrame(main_work->act[ACT_TEX_OK], 0.f);
			
			break;
			
		default:
			// 例外
			MTM_ASSERT(0);
			break;
		}
		
		
		AoActAcmPush();
		
		AoActAcmInit();
#if _WII
		AoActAcmApplyScale(DMD_USERNAME_DISP_SCALE_TEXT
						   , DMD_USERNAME_DISP_SCALE_TEXT);
#endif
		
		AoActAcmApplyTrans(dm_username_win_act_pos_tbl[1][0]
						   , dm_username_win_act_pos_tbl[1][1]
						   , 0
						   );
		
		// フレーム更新はSetFrameのみで行う
		AoActSetTexture(AoTexGetTexList(&dm_username_tex[DME_USERNAME_DATA_TYPE_LANG_DATA]));
		AoActUpdate(main_work->act[ACT_TEX_WIN_MSG2], 0.0f);
		
		for (int i = 0; i < 3; i++) {
			AoActAcmInit();
			
#if _WII
			AoActAcmApplyScale(DMD_USERNAME_DISP_SCALE_TEXT
							   , DMD_USERNAME_DISP_SCALE_TEXT);
#endif
			
			AoActAcmApplyTrans(dm_username_win_act_pos_tbl[i + 3][0]
							   , dm_username_win_act_pos_tbl[i + 3][1]
							   , 0
							   );
			
			// フレーム更新はSetFrameのみで行う
			AoActSetTexture(AoTexGetTexList(dm_username_cmn_tex[2]));
			AoActUpdate(main_work->act[ACT_TEX_YES + i], 0.0f);
		}
		
		// 戻るボタン・戻るテキスト
		AoActAcmInit();
		AoActAcmApplyTrans(dm_username_win_act_pos_tbl[7][0]
						   , dm_username_win_act_pos_tbl[7][1]
						   , 0
						   );
		
		// フレーム更新はSetFrameのみで行う
		AoActSetTexture(AoTexGetTexList(dm_username_cmn_tex[2]));
		AoActUpdate(main_work->act[ACT_TEX_BACK_WIN], 0.0f);
		
		AoActAcmInit();
		AoActAcmApplyTrans(dm_username_win_act_pos_tbl[6][0]
						   , dm_username_win_act_pos_tbl[6][1]
						   , 0
						   );
		
		AoActAcmApplyTrans(dm_username_back_text_length_tbl[GsEnvGetLanguage()]
						   , 0
						   , 0
						   );
		
#if _WII		// Wii版のみ左へ10ピクセルさらにずらせる
		AoActAcmApplyTrans(-10.f, 0, 0);
#endif
		
		// フレーム更新はSetFrameのみで行う
		AoActSetTexture(AoTexGetTexList(dm_username_cmn_tex[0]));
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
// dmUserNameSetDecideEfctData
/*!
	決定演出用のデータ設定処理
	移動先の設定などをここで行う
 */
// ==========================================================================
/*
void dmUserNameSetDecideEfctData(DMS_USERNAME_MAIN_WORK *main_work)
{
	
}
*/


// ==========================================================================
// dmUserNameSetFileTabDecideEfct
/*!
	ファイルテーブル決定演出用座標設定処理

  	ここでは基準点(一つ目のファイル)となる座標のみを移動させて
  	それ以外は一定の距離で表示位置をずらすようにする
 */
// ==========================================================================
void dmUserNameSetFileTabDecideEfct(DMS_USERNAME_MAIN_WORK *main_work)
{
	// 演出更新分
	UNREFERENCED_PARAMETER(main_work);
}


// ==========================================================================
// dmUserNameIsFileTabDecideEfctEnd
/*!
	ファイルテーブル決定演出終了チェック処理
 */
// ==========================================================================
BOOL dmUserNameIsFileTabDecideEfctEnd(DMS_USERNAME_MAIN_WORK *main_work)
{
	if (main_work->timer >= DMD_USERNAME_DECIDE_EFCT_TIME) {
		
		return TRUE;
	}

	return FALSE;
}



// ==========================================================================
// dmUserNameSetDecideChar
/*!
	決定文字設定処理
 */
// ==========================================================================
void dmUserNameSetDecideChar(DMS_USERNAME_MAIN_WORK *main_work)
{
	s32 set_char = 0;
	s8 tmp_name_string[DMD_USERNAME_CHAR_NUM_MAX + 1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	// 文字を設定名前に登録
	if (main_work->cur_char >= 0 && main_work->cur_char < DMD_USERNAME_CHAR_NO_MAX) {
		if (main_work->cur_char >= 10 && main_work->cur_char <= 35
			&& main_work->is_char_small == 1) {
			set_char = main_work->cur_char + 26;
		}
		
		else if (main_work->cur_char >= 36) {
			set_char = main_work->cur_char + 26;
		}
		
		else {
			set_char = main_work->cur_char;
		}
		
		// 入力するカーソル位置に文字がない場合
		if (!main_work->name_string[main_work->focus_name_no]) {
			main_work->name_string[main_work->focus_name_no] = (u8)(set_char + 1);
			main_work->focus_name_no = dmUserNameGetRevisedSetNameNo(main_work->focus_name_no
																	 , 1
																	 );
		}
		// カーソル位置に文字がある場合
		else {
			// カーソル位置の文字から終端までを一つずらす
			for (int i = main_work->focus_name_no; i < DMD_USERNAME_CHAR_NUM_MAX; i++) {
				if (i < DMD_USERNAME_CHAR_NUM_MAX) {
					tmp_name_string[i + 1] = main_work->name_string[i];
				}
				else {
					tmp_name_string[i] = main_work->name_string[i];
				}
			}
			
			if (main_work->focus_name_no != DMD_USERNAME_CHAR_NUM_MAX - 1) {
				// カーソル位置の文字から終端までを一つずらす
				for (int i = main_work->focus_name_no + 1; i < DMD_USERNAME_CHAR_NUM_MAX; i++) {
					main_work->name_string[i] = tmp_name_string[i];
				}
			}
			
			main_work->name_string[main_work->focus_name_no] = (u8)(set_char + 1);
			main_work->focus_name_no = dmUserNameGetRevisedSetNameNo(main_work->focus_name_no
																	 , 1
																	 );
		}
	}
	
	// バックスペース
	if (main_work->cur_char == 50 || main_work->cur_char == 51) {

		dmUserNameSetNameBackSpace(main_work);
		
//		if (main_work->name_string[0] == 0) {
//			main_work->announce_flag |= 1 << DME_USERNAME_WIN_PLEASE_SET_NAME;
//		}
		
		main_work->flag |= DMD_USERNAME_FLAG_PUSH_BS_BTN;
		main_work->efct_timer[0] = 0;
	}
	
	// 大文字小文字切り替え
	if (main_work->cur_char == 52 || main_work->cur_char == 53) {
		main_work->is_char_small ^= 1;
		
		main_work->flag |= DMD_USERNAME_FLAG_PUSH_RESIZE_BTN;
		main_work->efct_timer[1] = 0;
	}
	
	// スペース
	if (main_work->cur_char == 54 || main_work->cur_char == 55) {
		
		// 入力するカーソル位置に文字がない場合
		if (!main_work->name_string[main_work->focus_name_no]) {
			main_work->name_string[main_work->focus_name_no] = 76;
			main_work->focus_name_no = dmUserNameGetRevisedSetNameNo(main_work->focus_name_no
																	 , 1
																	 );
		}
		// カーソル位置に文字がある場合
		else {
			// カーソル位置の文字から終端までを一つずらす
			for (int i = main_work->focus_name_no; i < DMD_USERNAME_CHAR_NUM_MAX; i++) {
				if (i < DMD_USERNAME_CHAR_NUM_MAX) {
					tmp_name_string[i + 1] = main_work->name_string[i];
				}
				else {
					tmp_name_string[i] = main_work->name_string[i];
				}
			}
			
			if (main_work->focus_name_no != DMD_USERNAME_CHAR_NUM_MAX - 1) {
				// カーソル位置の文字から終端までを一つずらす
				for (int i = main_work->focus_name_no + 1; i < DMD_USERNAME_CHAR_NUM_MAX; i++) {
					main_work->name_string[i] = tmp_name_string[i];
				}
			}
			
			main_work->name_string[main_work->focus_name_no] = 76;
			main_work->focus_name_no = dmUserNameGetRevisedSetNameNo(main_work->focus_name_no
																	 , 1
																	 );
		}
		
		main_work->flag |= DMD_USERNAME_FLAG_PUSH_SPACE_BTN;
		main_work->efct_timer[2] = 0;
	}
	
	// 決定
	if (main_work->cur_char == 57 || main_work->cur_char == 58 || main_work->cur_char == 59) {
		// 文字列が空でないか
		if (main_work->name_string[0] != 0) {
			// 入力されている文字列がスペース以外があれば
			if (!dmUserNameIsNameStringAllSpace(main_work)) {
				// 決定演出遷移フラグON
				main_work->flag |= DMD_USERNAME_FLAG_DECIDE_NAME;
				
				main_work->flag |= DMD_USERNAME_FLAG_PUSH_DECIDE_BTN;
				main_work->efct_timer[3] = 0;
			}
		}
	}
}




////////////////////// FOCUS切り替え関連 /////////////////////////////////////

// ==========================================================================
// dmUserNameSetFocusChangeEfctData
/*!
	ユーザー名設定時の縦のACT切り替え時の設定処理
 */
// ==========================================================================
void dmUserNameSetFocusChangeEfctData(DMS_USERNAME_MAIN_WORK *main_work)
{
	// 上移動・下移動共に必要な変数の設定
	s32 chng_sign = 0;
	s32 diff_set_dir = 0;
	
	// 
	if (main_work->flag & DMD_USERNAME_FLAG_UP_CHNG_CRSR
		&& main_work->crsr_focus[1] == 0) {
		main_work->flag |= DMD_USERNAME_FLAG_CHNG_NAME_POS;
		
		// ここで名前入力のカーソルを非表示
		main_work->disp_flag &= ~DMD_USERNAME_DISP_FLAG_ACT_CRSR;
		
		
		return;
	}
	
	if (main_work->flag & DMD_USERNAME_FLAG_UP_CHNG_CRSR
		 || main_work->flag & DMD_USERNAME_FLAG_LEFT_CHNG_CRSR) {
		chng_sign = -1;
	}
	else if (main_work->flag & DMD_USERNAME_FLAG_DOWN_CHNG_CRSR
			  || main_work->flag & DMD_USERNAME_FLAG_RIGHT_CHNG_CRSR) {
		chng_sign = 1;
	}
	else {
		MTM_ASSERT(0);
	}
	
	if (main_work->flag & DMD_USERNAME_FLAG_UP_CHNG_CRSR
		 || main_work->flag & DMD_USERNAME_FLAG_DOWN_CHNG_CRSR) {
		diff_set_dir = 1;
	}
	else if (main_work->flag & DMD_USERNAME_FLAG_LEFT_CHNG_CRSR
			  || main_work->flag & DMD_USERNAME_FLAG_RIGHT_CHNG_CRSR) {
		diff_set_dir = 0;
	}
	else {
		MTM_ASSERT(0);
	}
	
	// 選択ファイル切り替え
	for (u32 i = 0; i < 2; i++) {
		main_work->prev_crsr_focus[i] = main_work->crsr_focus[i];
	}
	
	if (main_work->flag & DMD_USERNAME_FLAG_UP_CHNG_CRSR
		 || main_work->flag & DMD_USERNAME_FLAG_DOWN_CHNG_CRSR) {
		main_work->crsr_focus[1] = (f32)dmUserNameGetRevisedCharCrsrNo((s32)main_work->crsr_focus[1]
																	  , chng_sign, diff_set_dir
																	  , (s32)main_work->crsr_focus[1]
																	  , (s32)main_work->crsr_focus[0]);
	}
	else if (main_work->flag & DMD_USERNAME_FLAG_LEFT_CHNG_CRSR
			  || main_work->flag & DMD_USERNAME_FLAG_RIGHT_CHNG_CRSR) {
		main_work->crsr_focus[0] = (f32)dmUserNameGetRevisedCharCrsrNo((s32)main_work->crsr_focus[0]
																	  , chng_sign, diff_set_dir
																	  , (s32)main_work->crsr_focus[1]
																	  , (s32)main_work->crsr_focus[0]);
	}
	else {
		MTM_ASSERT(0);
	}
	
	main_work->cur_char = (s32)(main_work->crsr_focus[0] + main_work->crsr_focus[1] * 10);
}



// ==========================================================================
// dmUserNameSetChngFocusNamePos
/*!
	ユーザー名設定時の縦のACT切り替え時の設定処理
 */
// ==========================================================================
void dmUserNameSetChngFocusNamePos(DMS_USERNAME_MAIN_WORK *main_work)
{
	s32 rpt_width_max = 0;
	s32 chng_sign = 0;
	
	// ここで文字列の空文字までの配列番号を取得
	for (int i = 0; i < DMD_USERNAME_CHAR_NUM_MAX; i++) {
		rpt_width_max = i;
		
		if (main_work->name_string[i] == 0) {
			break;
		}
	}
	
	
	if (main_work->flag & DMD_USERNAME_FLAG_LEFT_CHNG_CRSR) {
		chng_sign = -1;
	}
	else if (main_work->flag & DMD_USERNAME_FLAG_RIGHT_CHNG_CRSR) {
		chng_sign = 1;
	}
	else {
		MTM_ASSERT(0);
	}
	
	// FOCUSカーソル移動設定
	main_work->focus_name_no = dmUserNameGetRevisedSetFocusNameCrsr(main_work->focus_name_no
																	, chng_sign
																	, rpt_width_max
																	);
}



// ==========================================================================
// dmUserNameSetCrsrChangeData
/*!
	カーソル切り替え後のデータ設定処理
 */
// ==========================================================================
void dmUserNameSetCrsrChangeData(DMS_USERNAME_MAIN_WORK *main_work)
{
	// カーソルの座標設定
	if (main_work->crsr_focus[1] != 5) {
		main_work->crsr_pos[0] = DMD_USERNAME_CRSR_DFLT_POS_X + DMD_USERNAME_CRSR_DIST_ONE_X * main_work->crsr_focus[0];
		main_work->crsr_pos[1] = DMD_USERNAME_CRSR_DFLT_POS_Y + DMD_USERNAME_CRSR_DIST_ONE_Y * main_work->crsr_focus[1];
		
		main_work->crsr_scale = 1.0f;
	}
	
	else {
		main_work->crsr_pos[0] = dm_username_char_crsr_down_disp_y_pos_tbl[(s32)main_work->crsr_focus[0]];
		main_work->crsr_pos[1] = DMD_USERNAME_CRSR_DOWN_POS_Y;
		
		main_work->crsr_scale = dm_username_char_crsr_down_scale_x_tbl[(s32)main_work->crsr_focus[0]];
	}
}



// ==========================================================================
// dmUserNameSetNameBackSpace
/*!
	バックスペース押した際の設定処理
 */
// ==========================================================================
void dmUserNameSetNameBackSpace(DMS_USERNAME_MAIN_WORK *main_work)
{
	s8 tmp_name_string[DMD_USERNAME_CHAR_NUM_MAX] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
#if _WII
	BOOL is_last_char = FALSE;
#endif
	
	// 文字入力した箇所が空文字の場合
	if (main_work->name_string[main_work->focus_name_no] == 0) {
		main_work->focus_name_no = dmUserNameGetRevisedSetNameNo(main_work->focus_name_no
																 , -1
																 );
		
		main_work->name_string[main_work->focus_name_no] = 0;
	}
	// 文字入力した箇所に文字がある場合
	else {
		if (main_work->name_string[main_work->focus_name_no + 1] != 0) {
			// カーソル位置の文字から終端までを一つずらす
			for (int i = main_work->focus_name_no; i < DMD_USERNAME_CHAR_NUM_MAX; i++) {
				tmp_name_string[i] = main_work->name_string[i + 1];
			}
		}
		
		main_work->name_string[main_work->focus_name_no] = 0;
//		main_work->focus_name_no = dmUserNameGetRevisedSetNameNo(main_work->focus_name_no
//																 , 1
//																 );
		
		// カーソル位置の文字から終端までを一つずらす
		for (int i = main_work->focus_name_no; i < DMD_USERNAME_CHAR_NUM_MAX; i++) {
			main_work->name_string[i] = tmp_name_string[i];
		}
	}
}



// ==========================================================================
// dmUserNameSetBtnPushEfct
/*!
	ボタン押し演出中処理
 */
// ==========================================================================
void dmUserNameSetBtnPushEfct(DMS_USERNAME_MAIN_WORK *main_work, u32 i)
{
	if (main_work->efct_timer[i] > 10.f) {
		main_work->flag &= ~(1 << (5 + i));

		main_work->efct_timer[i] = 0.f;
	}
	
	main_work->efct_timer[i]++;
}




// ==========================================================================
// dmUserNameSetWinOpenEfct
/*!
	ウインドウ入り演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmUserNameSetWinOpenEfct(DMS_USERNAME_MAIN_WORK *main_work)
{
	if (main_work->win_timer > DMD_USERNAME_WIN_EFCT_TIME) {
		// ウインドウ演出終了
		main_work->flag |= DMD_USERNAME_FLAG_WIN_EFCT_END;

		main_work->win_timer = 0.0f;

		for (u32 i = 0; i < 2; i++) {
			main_work->win_size_rate[i] = DMD_USERNAME_WIN_DEF_RATE;
		}
	}
	else {
		// タイマー更新(但しフレームレートが可変になるように修正すること)	◆
		main_work->win_timer++;
	}
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->win_size_rate[i] = main_work->win_timer / DMD_USERNAME_WIN_EFCT_TIME;
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
// dmUserNameSetWinCloseEfct
/*!
	ウインドウ閉め演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmUserNameSetWinCloseEfct(DMS_USERNAME_MAIN_WORK *main_work)
{
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->win_size_rate[i] = main_work->win_timer / DMD_USERNAME_WIN_EFCT_TIME;
		}
		else {
			main_work->win_size_rate[i] = 0.0f;
		}
	}

	if (main_work->win_timer < 0.0f) {
		// ウインドウ演出終了
		main_work->flag |= DMD_USERNAME_FLAG_WIN_EFCT_END;

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
// dmUserNameSetMsgWinOpenEfct
/*!
	ウインドウ入り演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmUserNameSetMsgWinOpenEfct(DMS_USERNAME_MAIN_WORK *main_work)
{
	if (main_work->win_timer > DMD_USERNAME_WIN_EFCT_TIME) {
		// ウインドウ演出終了
		main_work->flag |= DMD_USERNAME_FLAG_MSG_WIN_EFCT_END;

		main_work->win_timer = 0.0f;

		for (u32 i = 0; i < 2; i++) {
			main_work->msg_win_size_rate[i] = DMD_USERNAME_WIN_DEF_RATE;
		}
	}
	else {
		// タイマー更新(但しフレームレートが可変になるように修正すること)	◆
		main_work->win_timer++;
	}
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->msg_win_size_rate[i] = main_work->win_timer / DMD_USERNAME_WIN_EFCT_TIME;
		}
		else {
			main_work->msg_win_size_rate[i] = 1.0f;
		}

		if (main_work->msg_win_size_rate[i] > 1.0f) {
			main_work->msg_win_size_rate[i] = 1.0f;
		}
	}
}


// ==========================================================================
// dmUserNameSetMsgWinCloseEfct
/*!
	ウインドウ閉め演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmUserNameSetMsgWinCloseEfct(DMS_USERNAME_MAIN_WORK *main_work)
{
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->msg_win_size_rate[i] = main_work->win_timer / DMD_USERNAME_WIN_EFCT_TIME;
		}
		else {
			main_work->msg_win_size_rate[i] = 0.0f;
		}
	}

	if (main_work->win_timer < 0.0f) {
		// ウインドウ演出終了
		main_work->flag |= DMD_USERNAME_FLAG_MSG_WIN_EFCT_END;

		main_work->win_timer = 0.0f;

		for (u32 i = 0; i < 2; i++) {
			main_work->msg_win_size_rate[i] = 0.0f;
		}
	}
	else {
		// タイマー更新(但しフレームレートが可変になるように修正すること)	◆
		main_work->win_timer--;
	}
}



// ==========================================================================
// dmUserNameSetMsgWinState
/*!
	メッセージウインドウの開閉状態設定処理
 */
// ==========================================================================
void dmUserNameSetMsgWinState(DMS_USERNAME_MAIN_WORK *main_work)
{
	if (main_work->proc_win_update == dmUserNameProcWindowOpenEfct
		|| main_work->proc_win_update == dmUserNameProcWindowAnnounceIdle
		|| main_work->proc_win_update == dmUserNameProcWindowCloseEfct) {
		dm_username_is_win_open = TRUE;
	}
	else if (main_work->announce_flag) {
		dm_username_is_win_open = TRUE;
	}
	else {
		dm_username_is_win_open = FALSE;
	}
}



// ==========================================================================
// dmUserNameIsNameStringAllSpace
/*!
	名前文字列が全てスペースかどうかの判別処理
 */
// ==========================================================================
BOOL dmUserNameIsNameStringAllSpace(DMS_USERNAME_MAIN_WORK *main_work)
{
	BOOL result = TRUE;	// TRUEをデフォルト設定
	s32 char_cnt = 0;	// 0を初期値
	
	// 文字数カウント
	for (int i = 0; i < DMD_USERNAME_CHAR_NUM_MAX; i++) {
		if (main_work->name_string[i] == 0) {
			break;
		}
		
		char_cnt++;
	}
	
	// 入力文字数分、スペースばかりかどうかのチェック
	for (int i = 0; i < char_cnt; i++) {
		// スペース以外が一つでもあればTRUE
		if (main_work->name_string[i] != 76) {
			result = FALSE;
			
			break;
		}
		
		if (i == char_cnt - 1) {
			result = TRUE;
		}
	}
	
	return result;
}



// ==========================================================================
// dmUserNameIsChangeNameString
/*!
	名前文字列が変更前と変更があったかどうかの判別処理
 */
// ==========================================================================
BOOL dmUserNameIsChangeNameString(DMS_USERNAME_MAIN_WORK *main_work)
{
	BOOL result = FALSE;	// TRUEをデフォルト設定
	
	// 文字数カウント
	for (int i = 0; i < DMD_USERNAME_CHAR_NUM_MAX; i++) {
		if (main_work->name_string[i] != main_work->prev_name_string[i]) {
			result = TRUE;
			
			break;
		}
	}
	
	return result;
}



// ==========================================================================
// dmUserNameIsTexLoad
/*!
	テクスチャ構築完了チェック処理
 */
// ==========================================================================
s32 dmUserNameIsTexLoad(void)
{
	for (int i = 0; i < DME_USERNAME_DATA_TYPE_MAX; i++) {
		if (!AoTexIsLoaded(&dm_username_tex[i])) {
			// フラグ扱いでON
			return 0;
		}
	}

	return 1;
}


// ==========================================================================
// dmUserNameIsTexRelease
/*!
	テクスチャ解放完了チェック処理
 */
// ==========================================================================
s32 dmUserNameIsTexRelease(void)
{
	for (int i = 0; i < DME_USERNAME_DATA_TYPE_MAX; i++) {
		if (!AoTexIsReleased(&dm_username_tex[i])) {
			// フラグ扱いでON
			return 0;
		}
	}
	
	return 1;
}



// ===========================================================================
//	dmUserNameGetRevisedCharCrsrNo
/*!
	項目の場所を示す変数が最大・最小値を超えた際に補正した(回り込みさせた)値を取得する関数
	通常時のカーソル移動の選択項目に使用。

	@param id		[in] 項目番号
	@param diff		[in] 変化量(移動における)
	@return 補正された項目番号
*/
// ===========================================================================
s32 dmUserNameGetRevisedCharCrsrNo(s32 idx, s32 diff, s32 dir, s32 focus_row, s32 focus_clmn)
{
	s32 result = 0;
	s32 rpt_max = 0;
	
	if (focus_row != 5) {
		result = (int)idx + diff;
		
		// 方向ごとに切り替え
		if (dir == 0) {			// X方向
			if (focus_row != 4) {
				rpt_max = 10;
			}
			else {
				rpt_max = 9;
			}
			
			// 先頭から一つ戻ると最後に移動
			if (result < 0) {
				result = rpt_max - 1;
			}
			
			// 最後から一つ進むと先頭に移動
			if (result >= rpt_max) {
				result = 0;
			}
			
			MTM_ASSERT(result >= 0 && result < rpt_max);
		}		
	
		else {					// Y方向
			// 先頭から一つ戻ると最後に移動
			if (focus_clmn == 9
				&& focus_row == 3) {
				if (diff > 0) {
					result = 5;
				}
			}
			
			// 最後から一つ進むと先頭に移動
			else if (focus_clmn != 6) {
				if (result < 0) {
					result = 6 - 1;
				}
				if (result >= 6) {
					result = 0;
				}
			}
			else {
				if (result < 0) {
					result = 5 - 1;
				}
				if (result >= 5) {
					result = 0;
				}
			}
			
			MTM_ASSERT(result >= 0 && result < 6);
		}
	}
	
	else {
		result = (int)idx + diff;
		
		// 方向ごとに切り替え
		if (dir == 0) {			// X方向
			if (diff > 0) {
				if (idx == 0 || idx == 1) {
					result = 2;
				}
				else if (idx == 2 || idx == 3) {
					result = 4;
				}
				else if (idx == 4 || idx == 5) {
					result = 7;
				}
				else if (idx >= 7 && idx <= 9) {
					result = 0;
				}
			}
			else if (diff < 0) {
				if (idx == 0 || idx == 1) {
					result = 7;
				}
				else if (idx == 2 || idx == 3) {
					result = 0;
				}
				else if (idx == 4 || idx == 5) {
					result = 2;
				}
				else if (idx >= 7 && idx <= 9) {
					result = 4;
				}
			}
			
			// 先頭から一つ戻ると最後に移動
			if (result < 0) {
				result = 10 - 1;
			}
			
			// 最後から一つ進むと先頭に移動
			if (result >= 10) {
				result = 0;
			}
			
			MTM_ASSERT(result >= 0 && result < 10);
		}		
	
		else {					// Y方向
			// 先頭から一つ戻ると最後に移動
			if (focus_clmn == 9
				&& focus_row == 5) {
				if (diff < 0) {
					result = 3;
				}
			}
			
			if (result < 0) {
				result = 6 - 1;
			}
			
			// 最後から一つ進むと先頭に移動
			if (result >= 6) {
				result = 0;
			}
			
			MTM_ASSERT(result >= 0 && result < 6);
		}
	}
	
	return result;
}



// ===========================================================================
//	dmUserNameGetRevisedSetFocusNameCrsr
/*!
	項目の場所を示す変数が最大・最小値を超えた際に補正した(回り込みさせた)値を取得する関数
	通常時のカーソル移動の選択項目に使用。

	@param id		[in] 項目番号
	@param diff		[in] 変化量(移動における)
	@return 補正された項目番号
*/
// ===========================================================================
s32 dmUserNameGetRevisedSetFocusNameCrsr(s32 idx, s32 diff, s32 max)
{
	s32 result;
	
	result = (int)idx + diff;

	// 先頭から一つ戻ると最後に移動
	if (result < 0) {
		result = 0;
	}
	
	// 最後から一つ進むと先頭に移動
	if (result > max) {
		result = max;
	}
	
	MTM_ASSERT(result >= 0 && result <= max);
	
	return result;
}



// ===========================================================================
//	dmUserNameGetRevisedSetNameNo
/*!
	項目の場所を示す変数が最大・最小値を超えた際に補正した(回り込みさせた)値を取得する関数
	通常時のカーソル移動の選択項目に使用。

	@param id		[in] 項目番号
	@param diff		[in] 変化量(移動における)
	@return 補正された項目番号
*/
// ===========================================================================
s32 dmUserNameGetRevisedSetNameNo(s32 idx, s32 diff)
{
	s32 result;
	
	result = (int)idx + diff;

	// 先頭から一つ戻ると最後に移動
	if (result < 0) {
		result = 0;
	}
	
	// 最後から一つ進むと先頭に移動
	if (result >= 10) {
		result = 10 - 1;
	}
	
	MTM_ASSERT(result >= 0 && result < 10);
	
	return result;
}



#endif // #if _WII || _PC


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
