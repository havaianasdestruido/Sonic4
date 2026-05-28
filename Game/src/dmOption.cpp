// ===========================================================================
/*!
	@file	dmOption.cpp
	@brief	デモ・オプション画面

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: dmOption.cpp 20 2011-04-22 12:46:46Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 *
 *
 */

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#include "aoTexture.h"
#include "aoAction.h"

#include "gs.h"
#include "gsMainSys.h"
#include "izFade.h"
#include "aoWinSys.h"
#include "mtTask.h"
#include "gmTask.h"
#include "gsEnvironment.h"

#include "gsBackup.hpp"
#include "gsBackupOption.hpp"

#include "dmOption.h"

#if _PC || _WII
#include "dmUserName.h"
#endif // #if _PC || _WII

#include "dmLoading.h"
#include "dmManual.h"
#include "dmStaffRoll.h"

#include "dmCmnBackup.h"
#include "dmSave.h"

#include "gsSound.h"
#include "dmSound.h"
#include "dmSndBgmPlayer.h"

#if _IPHONE
#include "erTrgBasic.hpp"
#include "erTrgAoAction.hpp"
#endif //_IPHONE

// データヘッダ
#if !_IPHONE
#include "common/ace/D_OPTION.HMA"
#include "common/ace/D_OPTION_JP.HMA"
#include "common/ace/D_OPTION_USER_JP.HMA"
#else //!_IPHONE
#include "ace/D_OPTION.HMA"
#include "ace/D_OPTION_JP.HMA"
#endif //!_IPHONE

// 共通データヘッダ
#if !_IPHONE
#include "common/ace/D_CMN_BG.HMA"
#include "common/ace/D_CMN_BTN.HMA"
#include "common/ace/D_CMN_OBI.HMA"
#include "common/ace/D_CMN_WIN.HMA"
#include "common/ace/D_CMN_MSG_JP.HMA"
#else //!_IPHONE
#include "ace/D_CMN_BG.HMA"
#include "ace/D_CMN_BTN.HMA"
#include "ace/D_CMN_WIN.HMA"
#include "ace/D_CMN_MSG_JP.HMA"
#endif //!_IPHONE

// ----- Macros ------------------------------------------------（マクロ定義）

#define DMD_OPT_TASK_PAUSELEVEL			(0x7fff)
#define DMD_OPT_TASK_PRIO_MAIN			(0x2000)
#define DMD_OPT_TASK_GROUP_MAIN			(10)

#define DMD_OPT_FILE_PATH_NUM_MAX		(60)

#define DMD_OPT_CMN_DATA_FILENAME		(GSS_BASE_PATH"DEMO/CMN/D_CMN_WIN.AMB")
#define DMD_OPT_DATA_FILENAME			(GSS_BASE_PATH"DEMO/OPT/D_OPTION.AMB")

#define DMD_OPT_SIZE_WIDTH				(960.0f)
#define DMD_OPT_SIZE_HEIGHT				(720.0f)
#define DMD_OPT_SIZE_HALF_WIDTH			(480.0f)
#define DMD_OPT_SIZE_HALF_HEIGHT		(360.0f)


// プライオリティ設定
#define DMD_OPT_DRAW_PRIO_TOP_MENU		(0x2000)
#define DMD_OPT_DRAW_PRIO_SET_MENU		(0x2000)
#define DMD_OPT_DRAW_PRIO_CTRL_MENU		(0x2000)
#define DMD_OPT_DRAW_PRIO_BG			(0x1000)
#define DMD_OPT_DRAW_PRIO_FIX			(0x3000)
#define DMD_OPT_DRAW_PRIO_WIN			(0x3000)
#define DMD_OPT_DRAW_PRIO_WIN_FIX		(0x4000)

// 表示位置
#if !_IPHONE
//#define DMD_OPT_ACT_TABLE_		(60)
#define DMD_OPT_FILE_TABLE_TOP_POS_X	(170.0f)
#define DMD_OPT_TOP_MENU_TOP_POS_Y		(200.0f)
#define DMD_OPT_TOP_MENU_DIST_Y			(100.0f)
#define DMD_OPT_ALPHA_ONE_DIST			(20.f)
#else //!_IPHONE
#define DMD_OPT_TOP_MENU_ROW_MAX		(2)
#define DMD_OPT_TOP_MENU_DIST_X			(480.0f)
#define DMD_OPT_TOP_MENU_DIST_Y			(-20.0f + 240.0f)
#define DMD_OPT_TOP_MENU_TOP_POS_X		(DMD_OPT_SIZE_HALF_WIDTH - (DMD_OPT_TOP_MENU_DIST_X / 2))
#define DMD_OPT_TOP_MENU_TOP_POS_Y		(DMD_OPT_SIZE_HALF_HEIGHT - (DMD_OPT_TOP_MENU_DIST_Y / 2))
#endif //!_IPHONE

//#define DMD_OPT_STAGE_TAB_DISP_POS_X	()
#define DMD_OPT_STAGE_TAB_NODISP_POS_X	(1120.f)

#if !_WII
#define DMD_OPT_CTRL_MENU_CTRLR_POS_X	(720.f)
#else
#define DMD_OPT_CTRL_MENU_CTRLR_POS_X	(640.f)
#endif

#define DMD_OPT_CTRL_MENU_CTRLR_POS_Y	(384.f)

#define DMD_OPT_SAVE_FILE_NUM			(6)
#define DMD_OPT_FILE_POS_NUM			(4)
#define DMD_OPT_CRSR_POS_NUM			(3)

#define DMD_OPT_SET_TEX_NUM				(3)		// iphoneでは２に変更する

#define DMD_OPT_VOL_DATA_MAX			(10)

#define DMD_OPT_DRAW_STATE_ID			(0)

// ウインドウ関連
#if !_IPHONE
#define DMD_OPT_WINDOW_SIZE_W			(608.f)
#define DMD_OPT_WINDOW_SIZE_H			(240.f)
#else //!_IPHONE
#define DMD_OPT_WINDOW_SIZE_W			(840.f)
#define DMD_OPT_WINDOW_SIZE_H			(400.f)
#endif //!_IPHONE
#define DMD_OPT_WIN_DEF_RATE			(1.0f)

// フェード関連
#define DMD_OPT_FADEIN_TIME				(32.0f)
#define DMD_OPT_FADEOUT_TIME			(32.0f)

#define DMD_OPT_BGM_FADEIN_TIME			(32)
#define DMD_OPT_BGM_FADEOUT_TIME		(32)

// 演出関連
#define DMD_OPT_FOCUS_FILE_CHNG_TIME	(16.0f)
#define DMD_OPT_IN_EFCT_START_POS		(720.0f)
#define DMD_OPT_IN_EFCT_TIME			(32.0f)
#define DMD_OPT_OUT_EFCT_END_POS		(720.0f)
#define DMD_OPT_OUT_EFCT_TIME			(32.0f)
#define DMD_OPT_WIN_EFCT_TIME			(8.0f)
#define DMD_OPT_OBI_MOVE_START_POS		(1120.f)
#define DMD_OPT_OBI_MOVE_END_POS		(-1120.f)
#define DMD_OPT_OBI_MOVE_SPEED			(-3.f)
#define DMD_OPT_OBI_EFCT_TIME			(16.f)
#define DMD_OPT_OBI_NODISP_POS_Y		(192.f)
#define DMD_OPT_OBI_DISP_POS_Y			(0.f)
#define DMD_OPT_ACT_VRTCL_CHNG_DIST		(128.f)
#define DMD_OPT_ACT_VRTCL_CHNG_NUM		(3)
#define DMD_OPT_CRSR_MOVE_TIME			(8.0f)
#define DMD_OPT_DECIDE_EFCT_UP_TIME		(8.0f)
#define DMD_OPT_DECIDE_EFCT_DOWN_TIME	(8.0f)
#define DMD_OPT_DECIDE_EFCT_WAIT_TIME	(28.0f)
#define DMD_OPT_DECIDE_EFCT_END_UP_POS		(-960.0f)
#define DMD_OPT_DECIDE_EFCT_END_DOWN_POS	(720.0f)
#define DMD_OPT_DECIDE_FADE_COL_MAX		(255.f)
#define DMD_OPT_DECIDE_FADE_COL_MIN		(0.f)
#define DMD_OPT_NRML_CTRL_WAIT_EFCT_TIME	(120.f)
#define DMD_OPT_PUSH_DFLT_BTN_EFCT_DIST	(4.f)

#if _PS3 || _XBOX || _PC
#define DMD_OPT_OBI_MSG_SCALE_SIZE		(1.2f)
#endif

#if _IPHONE //操作方法ウインドウ
#define DMD_OPT_SETTING_CONTROL_WINDOW_SIZE_W			(800.f)
#define DMD_OPT_SETTING_CONTROL_WINDOW_SIZE_H			(600.f)
#endif //_IPHONE //操作方法ウインドウ


// フラグ関連
#define DMD_OPT_FLAG_EXIT				(1 << 0)		//!< 終了フラグ
#define DMD_OPT_FLAG_CANCEL				(1 << 1)		//!< キャンセル
#define DMD_OPT_FLAG_DECIDE				(1 << 2)		//!< 決定フラグ
#define DMD_OPT_FLAG_DISP_MENU			(1 << 3)
#define DMD_OPT_FLAG_DECIDE_EFCT		(1 << 4)
#define DMD_OPT_FLAG_CHNG_CTRL			(1 << 5)
#define DMD_OPT_FLAG_UP_CHNG_CRSR		(1 << 6)
#define DMD_OPT_FLAG_DOWN_CHNG_CRSR		(1 << 7)
#define DMD_OPT_FLAG_OPEN_SSONIC		(1 << 8)
#define DMD_OPT_FLAG_NO_CHNG_CTRL		(1 << 9)
#define DMD_OPT_FLAG_PUSH_DFLT_BTN		(1 << 10)
#define DMD_OPT_FLAG_WIN_EFCT_END		(1 << 11)
#define DMD_OPT_FLAG_BGM_UP_EFCT		(1 << 12)
#define DMD_OPT_FLAG_BGM_DOWN_EFCT		(1 << 13)
#define DMD_OPT_FLAG_SE_UP_EFCT			(1 << 14)
#define DMD_OPT_FLAG_SE_DOWN_EFCT		(1 << 15)
#define DMD_OPT_FLAG_MENU_CRSR_EFCT		(1 << 18)
#define DMD_OPT_FLAG_CHNG_STAFFROLL		(1 << 19)

#define DMD_OPT_FLAG_DEMO_SND_END		(1 << 20)

#define DMD_OPT_FLAG_SIGN_OUT_EXIT		(1 << 31)	// サインアウト時の終了フラグ

#if _IPHONE //操作方法ウインドウ
#define DMD_OPT_FLAG_SETTING_CONTROL_WINDOW		(1 << 24)	//設定・操作方法ウインドウを開く
#endif //_IPHONE //操作方法ウインドウ

// アクション表示フラグ関連
#define DMD_OPT_DISP_FLAG_WIN_ACT	(1 << 0)
#define DMD_OPT_DISP_FLAG_TB_ARROW	(1 << 1)
#define DMD_OPT_DISP_FLAG_ACT_CRSR	(1 << 2)
#define DMD_OPT_DISP_FLAG_OBI_TEX	(1 << 3)



// ----- Macro Functions -----------------------------------（処理マクロ定義）


// ----- Definitions -------------------------------------------（定数の宣言）
//! 次のイベント
typedef enum tag_DME_OPT_NEXT_EVT
{
	DME_OPT_NEXT_EVT_STAFFROLL = 0,	//!< スタッフロール
	DME_OPT_NEXT_EVT_TITLE,			//!< タイトル

	DME_OPT_NEXT_EVT_MAX
} DME_OPT_NEXT_EVT;


typedef enum tag_DME_OPT_DATA_TYPE
{
	DME_OPT_DATA_TYPE_CMN_DATA = 0,		//!< 共通データ
	DME_OPT_DATA_TYPE_LANG_DATA,		//!< 言語別データ
	
	DME_OPT_DATA_TYPE_MAX,
	DME_OPT_DATA_TYPE_NONE
} DME_OPT_DATA_TYPE;


//! オプショントップメニューでの選択項目タイプ
typedef enum tag_DME_OPT_MENU_STATE
{
	DME_OPT_MENU_STATE_TOP = 0,		//!< オプションTOP
	DME_OPT_MENU_STATE_CTRL,		//!< 操作方法画面
	DME_OPT_MENU_STATE_SET,			//!< 設定画面
//	DME_OPT_MENU_STATE_USER,		//!< 
	
	DME_OPT_MENU_STATE_NUM,
	DME_OPT_MENU_STATE_NONE
} DME_OPT_MENU_STATE;


//! オプショントップメニューでの選択項目タイプ
typedef enum tag_DME_OPT_TOP_MENU
{
	DME_OPT_TOP_MENU_1 = 0,		//!< 
	DME_OPT_TOP_MENU_2,			//!< 
	DME_OPT_TOP_MENU_3,			//!< 
	DME_OPT_TOP_MENU_4,			//!< 
	
	DME_OPT_TOP_MENU_NUM,
	DME_OPT_TOP_MENU_NONE
} DME_OPT_TOP_MENU;


//! オプション設定画面での選択項目タイプ
typedef enum tag_DME_OPT_SET_MENU
{
	DME_OPT_SET_MENU_BGM = 0,	//!< 
	DME_OPT_SET_MENU_SE,		//!< 
//	DME_OPT_SET_MENU_VBRT,		//!< 
	DME_OPT_SET_MENU_DEFAULT,	//!< 
	
	DME_OPT_SET_MENU_NUM,
	DME_OPT_SET_MENU_NONE
} DME_OPT_SET_MENU;


//! オプション設定画面での振動設定項目タイプ
typedef enum tag_DME_OPT_CTRL_VBRT
{
	DME_OPT_CTRL_VBRT_ON = 0,	//!< 
	DME_OPT_CTRL_VBRT_OFF,		//!< 
	
	DME_OPT_CTRL_VBRT_NUM,
	DME_OPT_CTRL_VBRT_NONE
} DME_OPT_CTRL_VBRT;


//! オプション操作方法画面での選択項目タイプ
typedef enum tag_DME_OPT_CTRL_MENU
{
	DME_OPT_CTRL_MENU_NORMAL = 0,	//!< 
	DME_OPT_CTRL_MENU_CLASSIC,		//!< 
	
	DME_OPT_CTRL_MENU_NUM,
	DME_OPT_CTRL_MENU_NONE
} DME_OPT_CTRL_MENU;



//! アクションテーブル(ノード含む)
typedef enum tag_DME_OPT_ACT
{
	// オプション共通・言語共通
#if !_IPHONE
	ACT_TAB_TITLE1 = 0,		//!< 
	ACT_TAB_TITLE2,		//!< 
//	ACT_TAB_BACK,		//!< 
#else //!_IPHONE
	ACT_TAB_TITLE1 = 0,		//!< 
	ACT_BACK_BTN01_L,
	ACT_BACK_BTN01_R,
#endif //!_IPHONE

	// オプショントップ・言語共通
	ACT_TAB_SELECT,		//!< 
#if !_IPHONE
	ACT_CRSR_TOP_SLCT1,	//!< 
	ACT_CRSR_TOP_SLCT2,	//!< 
	ACT_CRSR_TOP_SLCT3,	//!< 
#else //!_IPHONE
	ACT_TAB_SELECT_L,	//!< 
	ACT_TAB_SELECT_R,	//!< 
#endif //!_IPHONE

	// オプション・設定・言語共通
	ACT_NUM_VOL_1,
	ACT_NUM_VOL_2,
	ACT_NUM_VOL_3,
	ACT_NUM_VOL_4,
	ACT_ICON_VOL_UP,
	ACT_ICON_VOL_DOWN,
	ACT_GAUGE_VOL_1,
	ACT_GAUGE_VOL_2,
	ACT_GAUGE_VOL_3,
	ACT_GAUGE_VOL_4,
	ACT_GAUGE_VOL_5,
	ACT_GAUGE_VOL_6,
	ACT_GAUGE_VOL_7,
	ACT_GAUGE_VOL_8,
	ACT_GAUGE_VOL_9,
	ACT_GAUGE_VOL_10,
#if !_IPHONE
	ACT_CUR_SLCT_TAB1,
	ACT_CUR_SLCT_TAB2,
	ACT_TAB_DEFAULT1,
	ACT_TAB_DEFAULT2,
	ACT_TAB_DEFAULT3,
//	ACT_TAB_CRSR_BAR1,
//	ACT_TAB_CRSR_BAR2,
//	ACT_TAB_CRSR_BAR3,
	ACT_LINE_SEPARATE1,
	ACT_LINE_SEPARATE2,
	ACT_LINE_SEPARATE3,
	ACT_LINE_SEPARATE4,
//	ACT_TAB_SET_TITLE_L,
//	ACT_TAB_SET_TITLE_C,
//	ACT_TAB_SET_TITLE_R,
//	ACT_TAB_SET_TITLE_L2,
//	ACT_TAB_SET_TITLE_R2,
#else //!_IPHONE
	ACT_CNT01_L,
	ACT_CNT01_C,
	ACT_CNT01_R,
	ACT_CNT02_L,
	ACT_CNT02_C,
	ACT_CNT02_R,
#endif //!_IPHONE

#if _IPHONE //操作方法ウインドウ
	// オプション・設定・操作方法ウインドウ・言語共通
	ACT_TYPE_SCREEN,
	ACT_TYPE_SCREEN_A,
	ACT_TYPE_SCREEN_A2,
	ACT_TYPE_SCREEN_B,
	ACT_TYPE_YUBI_B1,
	ACT_TYPE_YUBI_B2,
	ACT_TYPE_LINE_B1,
	ACT_TYPE_LINE_B2,
	ACT_A_TYPE_BTN_L,
	ACT_A_TYPE_BTN_C,
	ACT_A_TYPE_BTN_R,
	ACT_B_TYPE_BTN_L,
	ACT_B_TYPE_BTN_C,
	ACT_B_TYPE_BTN_R,
#endif //_IPHONE //操作方法ウインドウ

	// オプション・操作方法・言語共通
	ACT_TAB_SOUSA_01L,
	ACT_TAB_SOUSA_01C,
	ACT_TAB_SOUSA_01R,
	ACT_TAB_SOUSA_01L2,
	ACT_TAB_SOUSA_01C2,
	ACT_TAB_SOUSA_01R2,
	ACT_LINE_SOUSA01,
	ACT_LINE_SOUSA02,
	ACT_SOUSA_CONT_A,
#if !_IPHONE
	ACT_SOUSA_CONT_B,
	ACT_SOUSA_BUT_A_A,
	ACT_SOUSA_BUT_A_B,
#else //!_IPHONE
	ACT_SOUSA_SCREEN,
	ACT_SOUSA_LINE_SONIC1,
	ACT_SOUSA_LINE_SONIC2,
	ACT_SOUSA_LINE_SONIC3,
	ACT_SOUSA_SSICON,
	ACT_SOUSA_FUKIDASHI,
	ACT_SOUSA_LINE_PA1,
	ACT_SOUSA_LINE_PA2,
	ACT_SOUSA_LINE_PA3,
	ACT_SOUSA_LINE_JUMP1,
	ACT_SOUSA_LINE_JUMP2,
	ACT_SOUSA_LINE_JUMP3,
	ACT_SOUSA_LINE_MOVE1,
	ACT_SOUSA_LINE_MOVE2,
	ACT_SOUSA_LINE_MOVE3,
	ACT_SOUSA_YUBI,
#endif //!_IPHONE
	ACT_S_SCREEN_01A,
	ACT_S_SCREEN_01B,

	// オプション共通・言語別
	ACT_TEX_TITLE,		//!< 
#if !_IPHONE
	ACT_TEX_OBI1,		//!< 
	ACT_TEX_OBI2,		//!< 
#endif //!_IPHONE

	// オプショントップ・言語別
	ACT_TEX_PLAY,		//!<
	ACT_TEX_CONTROL,	//!< 
	ACT_TEX_SETTING,	//!< 
	ACT_TEX_CREDIT,		//!< 
#if !_IPHONE
	ACT_TEX_USER,		//!< 
#endif //!_IPHONE

	// オプション・設定・言語別
	ACT_TEX_BGM,
	ACT_TEX_SE,
#if !_IPHONE
	ACT_TEX_VIVE,
#else //!_IPHONE
	ACT_TEX_CTRL,
#endif //!_IPHONE
	ACT_TEX_DEFAULT,
	ACT_TEX_ON,
	ACT_TEX_OFF,
#if _IPHONE
	ACT_TEX_TILT,
	ACT_TEX_TOUCH,
#endif //_IPHONE

#if _IPHONE //操作方法ウインドウ
	// オプション・設定・操作方法ウインドウ・言語別
	ACT_TEX_MSG01,
	ACT_TEX_TEX_DPAD,
	ACT_TEX_TYPE_A,
	ACT_TEX_TYPE_B,
	ACT_TEX_A_TILT,
	ACT_TEX_B_FLICK,
#endif //_IPHONE //操作方法ウインドウ
	
	// オプション・操作方法・言語別
	ACT_TEX_EX_A,
	ACT_TEX_EX_B,
	ACT_TEX_HOM01,
	ACT_TEX_SPIN01,
	ACT_TEX_SUPER,
#if _IPHONE
	ACT_TEX_TAP,
#endif //_IPHONE
	ACT_TEX_PAUSE,
	ACT_TEX_JUMP,
	ACT_TEX_MOVE,
	
#if _WII
	ACT_TEX_OBI_USER1,
	ACT_TEX_OBI_USER2,
#endif
	
	// メニュー共通データ
	ACT_WAVE_BG,			//!< 
	ACT_DOWN_BG,			//!< 
	ACT_BLUE_BG,			//!<
	
	ACT_BTN_CANCEL1,		//!< 
	
#if !_IPHONE
	ACT_OBI_C,				//!< 
	ACT_OBI_L,				//!< 
	ACT_OBI_R,				//!< 
	
	ACT_WIN_LINE,			//!< 
	
	ACT_TEX_WINTITLE,		//!< 
#endif //!_IPHONE
	ACT_TEX_BACK,			//!< 
	
	
	ACT_NUM,

	ACT_NONE
} DME_OPT_ACT;


typedef struct tag_DMS_OPT_MAIN_WORK	DMS_OPT_MAIN_WORK;

//! メインタスクワーク
struct tag_DMS_OPT_MAIN_WORK {
	
	AMS_FS			*arc_cmn_amb_fs[5];					//!< 共通アーカイブAMBファイル
	void			*arc_cmn_amb[5];					//!< 共通アーカイブAMBファイル
	void			*cmn_ama[5];						//!< AMAファイル
	void			*cmn_amb[5];						//!< AMBファイル
	AOS_TEXTURE		cmn_tex[5];							//!< メニュー共通テクスチャ
	
	AMS_FS			*arc_amb_fs[DME_OPT_DATA_TYPE_MAX];//!< アーカイブAMBファイル
	AMS_FS			*user_arc_amb_fs[DME_OPT_DATA_TYPE_MAX];//!< ユーザー名設定画面AMBファイル
	AMS_FS			*manual_arc_amb_fs[DME_OPT_DATA_TYPE_MAX];//!< マニュアル画面AMBファイル
	
	void			*arc_amb[DME_OPT_DATA_TYPE_MAX];//!< アーカイブAMBファイル
	void			*user_arc_amb[DME_OPT_DATA_TYPE_MAX];//!< ユーザー名設定画面AMBファイル
	void			*manual_arc_amb[DME_OPT_DATA_TYPE_MAX];//!< マニュアル画面AMBファイル
	
	AMS_FS			*win_amb_fs;					//!< ウインドウAMBファイル
	
	void			*ama[DME_OPT_DATA_TYPE_MAX];	//!< AMAファイル
	void			*amb[DME_OPT_DATA_TYPE_MAX];	//!< AMBファイル
	void			*win_amb;						//!< ウインドウ用AMBファイル
	
	AOS_TEXTURE		tex[DME_OPT_DATA_TYPE_MAX];		//!< テクスチャ
	AOS_TEXTURE		win_tex;						//!< ウインドウテクスチャ

	// ウインドウ用アクション

	// メニュー用アクション
	AOS_ACTION 		*act[ACT_NUM];
	AOS_ACTION 		*bg_icon_node;					//!< 背景に表示するアイコン(仮としてソニック)

//	void (*proc_win_input)(DMS_OPT_MAIN_WORK *);	//!< 入力処理関数
	void (*proc_input)(DMS_OPT_MAIN_WORK *);		//!< 入力処理関数
//	void (*proc_win_update)(DMS_OPT_MAIN_WORK *);	//!< ウインドウ用プロシージャ
	void (*proc_update)(DMS_OPT_MAIN_WORK *);		//!< メニュー用プロシージャ
	void (*proc_draw)(DMS_OPT_MAIN_WORK *);			//!< 描画用プロシージャ
	void (*proc_menu_draw)(DMS_OPT_MAIN_WORK *);		//!< メニュー描画用プロシージャ

	float frm_update_time;							//!< フレーム更新
	float timer;									//!< 汎用タイマー
	float efct_timer;								//!< 汎用タイマー
	float win_timer;								//!< 汎用タイマー
	float push_efct_timer[4];						//!< ボリューム押した際の演出用タイマー
	float vib_timer;								//!< 振動演出タイマー
	u32	flag;										//!< 汎用フラグ
	s32 state;										//!< TOPか操作方法か設定かなど
	u32 disp_flag;									//!< 表示切替用フラグ

	s32 next_evt;									//!< 次のイベント
	s32 prev_evt;									//!< 前のイベント
#if _IPHONE
	er::CTrgAoAction	trg_slct[DME_OPT_TOP_MENU_NUM];	//<トップメニュー選択肢トリガ
	er::CTrgAoAction	trg_return;					//<戻るトリガ
#endif

	// 設定情報
	s32 ctrl_mode;
	s32 prev_ctrl_mode;
	s32 volume_data[2];
	s32 set_vbrt;
#if _IPHONE
	er::CTrgAoAction	trg_bgm_btn[2];		//<BGM音量ボタントリガ
	er::CTrgRect		trg_bgm_slider;		//<BGM音量スライダートリガ
	er::CTrgAoAction	trg_se_btn[2];		//<SE音量ボタントリガ
	er::CTrgRect		trg_se_slider;		//<SE音量スライダートリガ
	er::CTrgAoAction	trg_ctrl_btn[2];	//<操作方法ボタン(タッチ・バーチャルパッド)
#endif
#if _IPHONE //操作方法ウインドウ
	er::CTrgAoAction	ctrl_win_trg_btn[2];	//<操作方法ウインドウ・トリガ
	float				ctrl_win_window_prgrs;	//<操作方法ウインドウ・ウインドウ開閉進行度
#endif //_IPHONE //操作方法ウインドウ


	s32 cur_slct_top;								//!< オプションTOPでのFOCUS項目
	s32 cur_slct_set;								//!< 設定画面でのFOCUS項目
	
	float top_crsr_pos_y;							//!<
	float src_crsr_pos_y;							//!< 
	float dst_crsr_pos_y;							//!< 
	
	// 操作画面の項目表示位置用
	float ctrl_tab_pos_x[2];
	float ctrl_tab_pos_y[2];
	float ctrl_move_src[2][2];
	float ctrl_move_dest[2][2];

	// 帯用
	float obi_pos_y;								//!< 帯の移動用座標変数
	float obi_tex_pos[2];							//!< 帯の移動用座標変数

	float set_icon_dir;
	float set_icon_shdw_dir;

	float win_size_rate[2];								//!<
	
	u32 draw_state;

	AOS_ACT_COL decide_menu_col;
	AOS_ACT_COL vol_icon_col;
	AOS_ACT_COL win_col;

	s32 nrml_disp_type;
	s32 prev_nrml_disp_type;
	
	BOOL is_jp_region;
	
	// メインゲーム中用のサウンドSCBファイルポインタ
	GSS_SND_SCB *bgm_scb;
	
	GSS_SND_SE_HANDLE *se_handle;
};


//! 管理構造体
typedef struct tag_DMS_OPT_MGR {
	MTS_TASK_TCB *tcb;	//!< TCB
} DMS_OPT_MGR;


// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static void dmOptInit(void);
static void dmOptProcMain(MTS_TASK_TCB *tcb);
static void dmOptDest(MTS_TASK_TCB *tcb);

// 初期化設定関連
static void dmOptSetSaveOptionData(DMS_OPT_MAIN_WORK *main_work);
static void dmOptSetInitDispOptionData(DMS_OPT_MAIN_WORK *main_work);
static void dmOptSetNextEvt(DMS_OPT_MAIN_WORK *main_work);

static void dmOptLoadFontData(DMS_OPT_MAIN_WORK *main_work);
static void dmOptIsLoadFontData(DMS_OPT_MAIN_WORK *main_work);
static void dmOptLoadRequest(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcLoadWait(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcLoadWait2(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcTexBuildWait(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcCheckLoadingEnd(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcCreateAct(DMS_OPT_MAIN_WORK *main_work);

// メニュー用プロシージャ
static void dmOptProcFadeIn(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcFadeOut(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcStopDraw(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcDataRelease(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcFinish(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcFinishWaitSave(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcWaitFinished(DMS_OPT_MAIN_WORK *main_work);

static void dmOptProcTopMenuIdle(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcTopMenuDecideEfct(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcSetMenuInEfct(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcSetMenuIdle(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcSetMenuOutEfct(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcCtrlMenuIdle(DMS_OPT_MAIN_WORK *main_work);
#if _PC || _WII
static void dmOptProcUserMenuIdle(DMS_OPT_MAIN_WORK *main_work);
#endif

#if _PS3 || _XBOX || _IPHONE
static void dmOptProcManualStartFadeOut(DMS_OPT_MAIN_WORK *main_work);
#endif

#if _PC || _PS3 || _XBOX || _IPHONE
static void dmOptProcManualIdle(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcManualEndFadeIn(DMS_OPT_MAIN_WORK *main_work);
#endif // #if _PC || _PS3 || _XBOX || _IPHONE

static void dmOptProcStfrlStartFadeOut(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcStfrlIdle(DMS_OPT_MAIN_WORK *main_work);
static void dmOptProcStfrlEndFadeIn(DMS_OPT_MAIN_WORK *main_work);

// ウインドウ用プロシージャ
//static void dmOptProcWindowNodispIdle(DMS_OPT_MAIN_WORK *main_work);
//static void dmOptProcWindowOpenEfct(DMS_OPT_MAIN_WORK *main_work);
//static void dmOptProcWindowAnnounceIdle(DMS_OPT_MAIN_WORK *main_work);
//static void dmOptProcWindowCloseEfct(DMS_OPT_MAIN_WORK *main_work);

// 入力処理用プロシージャ
static void dmOptInputProcTopMenu(DMS_OPT_MAIN_WORK *main_work);
static void dmOptInputProcSettingMenu(DMS_OPT_MAIN_WORK *main_work);
static void dmOptInputProcControlMenu(DMS_OPT_MAIN_WORK *main_work);

//static void dmOptInputProcFileSelectMove(DMS_OPT_MAIN_WORK *main_work);

// 描画関連処理
static void dmOptProcActDraw(DMS_OPT_MAIN_WORK *main_work);
static void dmOptCommonDraw(DMS_OPT_MAIN_WORK *main_work);
static void dmOptCommonFixDraw(DMS_OPT_MAIN_WORK *main_work);
static void dmOptTopMenuDraw(DMS_OPT_MAIN_WORK *main_work);
static void dmOptSettingMenuDraw(DMS_OPT_MAIN_WORK *main_work);
static void dmOptControlMenuDraw(DMS_OPT_MAIN_WORK *main_work);
static void dmOptTaskDraw(AMS_TCB* tcb);

// 演出関連設定処理
static void dmOptSetTopMenuTabDecideEfct(DMS_OPT_MAIN_WORK *main_work);
static BOOL dmOptIsTopMenuTabDecideEfctEnd(DMS_OPT_MAIN_WORK *main_work);

static void dmOptSetCtrlFocusChangeEfct(DMS_OPT_MAIN_WORK *main_work);
static BOOL dmOptIsCtrlFocusChangeEfctEnd(DMS_OPT_MAIN_WORK *main_work);

static void dmOptSetWinOpenEfct(DMS_OPT_MAIN_WORK *main_work);
static void dmOptSetWinCloseEfct(DMS_OPT_MAIN_WORK *main_work);

static void dmOptSetChngFocusCrsrData(DMS_OPT_MAIN_WORK *main_work);

//static void dmOptSetActChngZonePosInit(DMS_OPT_MAIN_WORK *main_work, s32 diff);
static void dmOptSetObiEfctPos(DMS_OPT_MAIN_WORK *main_work);
static void dmOptSetNextProcFunc(DMS_OPT_MAIN_WORK *main_work);

static s32 dmOptIsDataLoad(DMS_OPT_MAIN_WORK *main_work);
static s32 dmOptIsTexLoad(DMS_OPT_MAIN_WORK *main_work);
static s32 dmOptIsTexLoad2(DMS_OPT_MAIN_WORK *main_work);
static s32 dmOptIsTexRelease(DMS_OPT_MAIN_WORK *main_work);

static s32 dmOptGetRevisedTopMenuNo(s32 idx, s32 diff);
static s32 dmOptGetRevisedSettingMenuNo(s32 idx, s32 diff);
static s32 dmOptGetRevisedVolume(s32 idx, s32 diff);

static void dmOptSetTopMenuDecideEfctData(DMS_OPT_MAIN_WORK *main_work);
static void dmOptSetDefaultDataSetMenu(DMS_OPT_MAIN_WORK *main_work);
static void dmOptSetDfltPushEfct(DMS_OPT_MAIN_WORK *main_work);
static void dmOptSetVolPushEfct(DMS_OPT_MAIN_WORK *main_work);

#if _IPHONE
static void dmOptControlResetAct(DMS_OPT_MAIN_WORK *main_work);
#endif //_IPHONE


// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ----- Static Variables --------------------（スタティック変数の定義：局所）

// メニュー共通AMBファイルパステーブル
const static char *dm_opt_menu_cmn_amb_name_tbl[4] = {
	GSS_BASE_PATH"DEMO/CMN/D_CMN_BG.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_BTN.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_OBI.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_WIN.AMB",
};

// 各国別メニュー共通AMBファイルパステーブル
const static char *dm_opt_menu_cmn_lng_amb_name_tbl[GSD_LANGUAGE_NUM] = {
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_JP.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_US.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_FR.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_IT.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_GE.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_SP.AMB",
};


// 各国別AMBファイルパステーブル
const static char *dm_opt_main_lng_amb_name_tbl[GSD_LANGUAGE_NUM] = {
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_JP.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_US.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_FR.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_IT.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_GE.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_SP.AMB",
};


#if _WII || _PC
// 各国別AMBファイルパステーブル
const static char *dm_opt_user_lng_amb_name_tbl[GSD_LANGUAGE_NUM] = {
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER_JP.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER_US.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER_FR.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER_IT.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER_GE.AMB",
	GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER_SP.AMB",
};
#endif // #if _WII || _PC

#if _PC || _PS3 || _XBOX || _IPHONE
// 各国別AMBファイルパステーブル
const static char *dm_opt_manual_file_lng_amb_name_tbl[GSD_LANGUAGE_NUM] = {
	GSS_BASE_PATH"DEMO/MANUAL/D_MANUAL_JP.AMB",
	GSS_BASE_PATH"DEMO/MANUAL/D_MANUAL_US.AMB",
	GSS_BASE_PATH"DEMO/MANUAL/D_MANUAL_FR.AMB",
	GSS_BASE_PATH"DEMO/MANUAL/D_MANUAL_IT.AMB",
	GSS_BASE_PATH"DEMO/MANUAL/D_MANUAL_GE.AMB",
	GSS_BASE_PATH"DEMO/MANUAL/D_MANUAL_SP.AMB",
};
#endif // #if _PC || _PS3 || _XBOX || _IPHONE



// 操作方法画面の上側のコントロールTABの座標位置テーブル
const static float dm_opt_ctrl_up_tab_disp_pos_tbl[2][2] = {
	{80.f, 0.f},		// 上がアクティブ
	{0.f, 0.f},			// 下がアクティブ
};


// 操作方法画面の下側のコントロールTABの座標位置テーブル
const static float dm_opt_ctrl_down_tab_disp_pos_tbl[2][2] = {
	{0.f, 0.f},			// 上がアクティブ
	{80.f, 0.f},		// 下がアクティブ
};


// 選択項目表示切替フレームテーブル
const static float dm_opt_ctrl_menu_disp_chng_frm_tbl[2][2] = {
	{1.f, 0.f},		// 左(上)がアクティブ
	{0.f, 1.f},		// 右(下)がアクティブ
};


// 操作方法画面の項目アイコン初期表示位置テーブル
const static float dm_opt_ctrl_icon_disp_pos_tbl[2][2] = {
	{148.f, 276.f},		// 上
	{148.f, 438.f},		// 下
};


// 操作方法画面のノーマル項目表示位置テーブル
const static float dm_opt_ctrl_nrml_disp_pos_tbl[2][2] = {
	{120.f, 0.f},		// 上がアクティブ
	{0.f, 0.f},				// 下がアクティブ
};


// 操作方法画面のクラシック項目表示位置テーブル
const static float dm_opt_ctrl_clsc_disp_pos_tbl[2][2] = {
	{0.f, 0.f},				// 上がアクティブ
	{120.f, 0.f},		// 下がアクティブ
};



// 設定画面項目表示座標テーブル
const static float dm_opt_set_tab_pos_y_tbl[DME_OPT_SET_MENU_NUM] = {
#if !_IPHONE
	230.f + 48.f,		// BGM
	308.f + 48.f,		// SE
	408.f + 48.f,//492.f,		// デフォルト
#else //!_IPHONE
	178.f + 48.f,		// BGM
	308.f + 48.f,		// SE
	460.f + 48.f,		// デフォルト
#endif //!_IPHONE
};


// 設定画面分岐線表示座標テーブル
const static float dm_opt_set_line_pos_y_tbl[3] = {
	190.f + 48.f,		// BGM
	268.f + 48.f,		// BGM
	346.f + 48.f,		// SE
//	474.f + 48.f,		// デフォルト
};



const static s32 dm_opt_top_menu_tex_tbl[DME_OPT_TOP_MENU_NUM] = {
#if _WII
	ACT_TEX_CONTROL,	// 操作方法
	ACT_TEX_SETTING,	// 設定
	ACT_TEX_USER,		// ユーザー名入力
	ACT_TEX_CREDIT,		// クレジット
#else
	ACT_TEX_PLAY,		// 遊び方
	ACT_TEX_CONTROL,	// 操作方法
	ACT_TEX_SETTING,	// 設定
	ACT_TEX_CREDIT,		// クレジット
#endif
};


const static s32 dm_opt_set_menu_tex_tbl[DME_OPT_SET_MENU_NUM] = {

	ACT_TEX_BGM,		// BGM
	ACT_TEX_SE,			// SE
	ACT_TEX_DEFAULT,	// デフォルト

};


// アクションIDテーブル(初期状態)
const static u32 g_dm_act_id_tbl[ACT_NUM] = {
	// オプション共通・言語共通
#if !_IPHONE
	IDA_D_OPTION_ACT_TAB_TITLE1,
	IDA_D_OPTION_ACT_TAB_TITLE2,
//	IDA_D_OPTION_ACT_TAB_BACK,
#else //!_IPHONE
	IDA_D_OPTION_ACT_TAB_TITLE2,
	IDA_D_CMN_WIN_ACT_STGSLCT_BACK_L,
	IDA_D_CMN_WIN_ACT_STGSLCT_BACK_C,
#endif //!_IPHONE

	// オプショントップ・言語共通
#if !_IPHONE
	IDA_D_OPTION_ACT_TAB_A_02_ON,
	IDA_D_OPTION_ACT_TAB_A_01_L,
	IDA_D_OPTION_ACT_TAB_A_01_C,
	IDA_D_OPTION_ACT_TAB_A_01_R,
#else //!_IPHONE
	IDA_D_OPTION_ACT_TAB_MANU_C,
	IDA_D_OPTION_ACT_TAB_MANU_L,
	IDA_D_OPTION_ACT_TAB_MANU_R,
#endif //!_IPHONE

	// オプション・設定・言語共通
	IDA_D_OPTION_ACT_NUM_VOL_100,
	IDA_D_OPTION_ACT_NUM_VOL_10,
	IDA_D_OPTION_ACT_NUM_VOL_1,
	IDA_D_OPTION_ACT_NUM_VOL_PERCENT,
	IDA_D_OPTION_ACT_ICON_VOL_UP,
	IDA_D_OPTION_ACT_ICON_VOL_DOWN,
	IDA_D_OPTION_ACT_ICON_VOL_GAUGE1,
	IDA_D_OPTION_ACT_ICON_VOL_GAUGE2,
	IDA_D_OPTION_ACT_ICON_VOL_GAUGE3,
	IDA_D_OPTION_ACT_ICON_VOL_GAUGE4,
	IDA_D_OPTION_ACT_ICON_VOL_GAUGE5,
	IDA_D_OPTION_ACT_ICON_VOL_GAUGE6,
	IDA_D_OPTION_ACT_ICON_VOL_GAUGE7,
	IDA_D_OPTION_ACT_ICON_VOL_GAUGE8,
	IDA_D_OPTION_ACT_ICON_VOL_GAUGE9,
	IDA_D_OPTION_ACT_ICON_VOL_GAUGE10,
#if !_IPHONE
	IDA_D_OPTION_ACT_TAB_SE_02L,
	IDA_D_OPTION_ACT_TAB_SE_02R,
	IDA_D_OPTION_ACT_TAB_DEFAULT_1L,
	IDA_D_OPTION_ACT_TAB_DEFAULT_1C,
	IDA_D_OPTION_ACT_TAB_DEFAULT_1R,
//	IDA_D_OPTION_ACT_TAB_A_01_L,
//	IDA_D_OPTION_ACT_TAB_A_01_C,
//	IDA_D_OPTION_ACT_TAB_A_01_R,
	IDA_D_OPTION_ACT_LINE_SE_01,
	IDA_D_OPTION_ACT_LINE_SE_01,
	IDA_D_OPTION_ACT_LINE_SE_01,
	IDA_D_OPTION_ACT_LINE_SE_01,
//	IDA_D_OPTION_ACT_TAB_SE_01L,
//	IDA_D_OPTION_ACT_TAB_SE_01C,
//	IDA_D_OPTION_ACT_TAB_SE_01R,
//	IDA_D_OPTION_ACT_TAB_SE_01L_2,
//	IDA_D_OPTION_ACT_TAB_SE_01R_2,
#else //!_IPHONE
	IDA_D_OPTION_ACT_CNT01_L,
	IDA_D_OPTION_ACT_CNT01_C,
	IDA_D_OPTION_ACT_CNT01_R,
	IDA_D_OPTION_ACT_CNT02_L,
	IDA_D_OPTION_ACT_CNT02_C,
	IDA_D_OPTION_ACT_CNT02_R,
#endif //!_IPHONE

#if _IPHONE //操作方法ウインドウ
	// オプション・設定・操作方法ウインドウ・言語共通
	IDA_D_OPTION_ACT_TYPE_SCREEN_DPAD,
	IDA_D_OPTION_ACT_TYPE_SCREEN_A,
	IDA_D_OPTION_ACT_TYPE_SCREEN_A2,
	IDA_D_OPTION_ACT_TYPE_SCREEN_B,
	IDA_D_OPTION_ACT_TYPE_YUBI_B1,
	IDA_D_OPTION_ACT_TYPE_YUBI_B2,
	IDA_D_OPTION_ACT_TYPE_LINE_B1,
	IDA_D_OPTION_ACT_TYPE_LINE_B2,
	IDA_D_OPTION_ACT_A_TYPE_BTN_L,
	IDA_D_OPTION_ACT_A_TYPE_BTN_C,
	IDA_D_OPTION_ACT_A_TYPE_BTN_R,
	IDA_D_OPTION_ACT_B_TYPE_BTN_L,
	IDA_D_OPTION_ACT_B_TYPE_BTN_C,
	IDA_D_OPTION_ACT_B_TYPE_BTN_R,
#endif //_IPHONE //操作方法ウインドウ

	// オプション・操作方法・言語共通
	IDA_D_OPTION_ACT_TAB_SOUSA_01L,
	IDA_D_OPTION_ACT_TAB_SOUSA_01C,
	IDA_D_OPTION_ACT_TAB_SOUSA_01R,
	IDA_D_OPTION_ACT_TAB_SOUSA_01L2,
	IDA_D_OPTION_ACT_TAB_SOUSA_01C2,
	IDA_D_OPTION_ACT_TAB_SOUSA_01R2,
	IDA_D_OPTION_ACT_LINE_SOUSA01,
	IDA_D_OPTION_ACT_LINE_SOUSA02,
	IDA_D_OPTION_ACT_SOUSA_CONT_A,
#if !_IPHONE
	IDA_D_OPTION_ACT_SOUSA_CONT_B,
	IDA_D_OPTION_ACT_SOUSA_BUT_A_A,
	IDA_D_OPTION_ACT_SOUSA_BUT_A_B,
#else //!_IPHONE
	IDA_D_OPTION_ACT_SCREEN_NORMAL,
	IDA_D_OPTION_ACT_SOUSA_LINE_SONIC1,
	IDA_D_OPTION_ACT_SOUSA_LINE_SONIC2,
	IDA_D_OPTION_ACT_SOUSA_LINE_SONIC3,
	IDA_D_OPTION_ACT_SOUSA_SSICON,
	IDA_D_OPTION_ACT_SOUSA_HUKIDASHI,
	IDA_D_OPTION_ACT_SOUSA_LINE_PA1,
	IDA_D_OPTION_ACT_SOUSA_LINE_PA2,
	IDA_D_OPTION_ACT_SOUSA_LINE_PA3,
	IDA_D_OPTION_ACT_SOUSA_LINE_JUMP1,
	IDA_D_OPTION_ACT_SOUSA_LINE_JUMP2,
	IDA_D_OPTION_ACT_SOUSA_LINE_JUMP3,
	IDA_D_OPTION_ACT_SOUSA_LINE_MOVE1,
	IDA_D_OPTION_ACT_SOUSA_LINE_MOVE2,
	IDA_D_OPTION_ACT_A_SOUSA_LINE_MOVE3,
	IDA_D_OPTION_ACT_SOUSA_YUBI,
#endif //!_IPHONE
	
	IDA_D_OPTION_ACT_S_SCREEN_01A,
	IDA_D_OPTION_ACT_S_SCREEN_01B,

	// オプション共通・言語別
	IDA_D_OPTION_JP_ACT_TEX_TITLE,
#if !_IPHONE
	IDA_D_OPTION_JP_ACT_TEX_OBI1,
	IDA_D_OPTION_JP_ACT_TEX_OBI1,
#endif //!_IPHONE

	// オプショントップ・言語別
#if !_WII
	IDA_D_OPTION_JP_ACT_TEX_TOP_PLAY,
#else
	IDA_D_OPTION_JP_ACT_TEX_TOP_CONTROL,	// WIIはここは使用しないのでダミー
#endif
	IDA_D_OPTION_JP_ACT_TEX_TOP_CONTROL,
	IDA_D_OPTION_JP_ACT_TEX_TOP_SETTING,
	IDA_D_OPTION_JP_ACT_TEX_TOP_CREDIT,
#if !_IPHONE
	IDA_D_OPTION_JP_ACT_TEX_TOP_USER,
#endif //!_IPHONE

	// オプション・設定・言語別
	IDA_D_OPTION_JP_ACT_TEX_ITEM_BGM,
	IDA_D_OPTION_JP_ACT_TEX_ITEM_SE,
#if !_IPHONE
	IDA_D_OPTION_JP_ACT_TEX_ITEM_VBRT,
#else //!_IPHONE
	IDA_D_OPTION_JP_ACT_TEX_ITEM_CONT,
#endif //!_IPHONE
	IDA_D_OPTION_JP_ACT_TEX_DEFAULT,
	IDA_D_OPTION_JP_ACT_TEX_ON,
	IDA_D_OPTION_JP_ACT_TEX_OFF,
#if _IPHONE
	IDA_D_OPTION_JP_ACT_TEX_CONT_TILT,
	IDA_D_OPTION_JP_ACT_TEX_CONT_TOUCH,
#endif //_IPHONE

#if _IPHONE //操作方法ウインドウ
	// オプション・設定・操作方法ウインドウ・言語別
	IDA_D_OPTION_JP_ACT_TEX_MSG01,
	IDA_D_OPTION_JP_ACT_TEX_DPAD,
	IDA_D_OPTION_JP_ACT_TEX_TYPE_A,
	IDA_D_OPTION_JP_ACT_TEX_TYPE_B,
	IDA_D_OPTION_JP_ACT_TEX_A_TILT,
	IDA_D_OPTION_JP_ACT_TEX_B_FLICK,
#endif //_IPHONE //操作方法ウインドウ

	// オプション・操作方法・言語別
	IDA_D_OPTION_JP_ACT_SOUSA_TEX_EX_A,
	IDA_D_OPTION_JP_ACT_SOUSA_TEX_EX_B,
	IDA_D_OPTION_JP_ACT_SOUSA_TEX_HOM01,
	IDA_D_OPTION_JP_ACT_SOUSA_TEX_SPIN01,
	IDA_D_OPTION_JP_ACT_SOUSA_TEX_SUPER,
#if _IPHONE
	IDA_D_OPTION_JP_ACT_SOUSA_TEX_TAP,
#endif //_IPHONE
	IDA_D_OPTION_JP_ACT_SOUSA_TEX_PAUSE,
	IDA_D_OPTION_JP_ACT_SOUSA_TEX_JUMP,
	IDA_D_OPTION_JP_ACT_SOUSA_TEX_MOVE,
	
#if _WII
	IDA_D_OPTION_USER_JP_ACT_TEX_OBI1,
	IDA_D_OPTION_USER_JP_ACT_TEX_OBI1,
#endif
	
	// メニュー共通データ
	IDA_D_CMN_BG_ACT_BG_WAVE,				//!< 
	IDA_D_CMN_BG_ACT_BG_DOWN_WHITE,			//!< 
	IDA_D_CMN_BG_ACT_BG_BLUE,				//!<
	
	IDA_D_CMN_BTN_ACT_BTN_BACK_OPT,			//!< 
	
#if !_IPHONE
	IDA_D_CMN_OBI_ACT_OBI_CENTER,			//!< 
	IDA_D_CMN_OBI_ACT_OBI_LEFT,				//!< 
	IDA_D_CMN_OBI_ACT_OBI_RIGHT,			//!< 
	
	IDA_D_CMN_WIN_ACT_WIN_LINE,				//!< 
	
	IDA_D_CMN_MSG_JP_ACT_TEX_WINTITLE,		//!< 
#endif //!_IPHONE
	IDA_D_CMN_MSG_JP_ACT_TEX_BACK_OPT,		//!< 
	
};


//管理情報
static DMS_OPT_MGR dm_opt_mgr;
static DMS_OPT_MGR *dm_opt_mgr_p = NULL;

static AOS_TEXTURE dm_opt_win_tex;

static u32 dm_opt_draw_state = 0;
static BOOL dm_opt_is_pause_maingame = FALSE;

static s16 dm_opt_prev_evt = 0;


// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// DmOptionStart
/*!
	オプション画面開始処理
 */
// ==========================================================================
void DmOptionStart(void *arg)
{
	s16 tmp_cur_evt = 0;
	
	UNREFERENCED_PARAMETER(arg);
	
	// 管理情報初期化設定
	amZeroMemory(&dm_opt_mgr, sizeof(DMS_OPT_MGR));
	dm_opt_mgr_p = &dm_opt_mgr;
	
	amZeroMemory(&dm_opt_win_tex, sizeof(AOS_TEXTURE));
	
	// 現在ロードしているイベントがメインゲームかどうかの設定
	tmp_cur_evt = SyGetEvtInfo()->cur_evt_id;
	
	if (tmp_cur_evt == GSD_EVT_ID_MAINGAME
		|| tmp_cur_evt == GSD_EVT_ID_SPSTAGE_BRANCH) {
		// ポーズメニューからの遷移フラグON
		dm_opt_is_pause_maingame = TRUE;
		
		mtTaskStartPause(GMD_TASK_GAME_PAUSE_LEVEL);
	}
	else {
		// フラグクリア
		dm_opt_is_pause_maingame = FALSE;
	}
	
	dmOptInit();
}


// ==========================================================================
// DmOptionIsExit
/*!
	オプション画面終了チェック処理
 */
// ==========================================================================
BOOL DmOptionIsExit(void)
{
	if (dm_opt_mgr_p != NULL) {
		if (dm_opt_mgr_p->tcb == NULL) {
		
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
// dmOptInit
/*!
	オプション画面初期化処理
 */
// ==========================================================================
void dmOptInit(void)
{
	DMS_OPT_MAIN_WORK	*main_work;


	// メインタスク作成
	dm_opt_mgr_p->tcb = MTM_TASK_MAKE_TCB(dmOptProcMain
										, dmOptDest
										, 0
										, DMD_OPT_TASK_PAUSELEVEL
										, DMD_OPT_TASK_PRIO_MAIN
										, DMD_OPT_TASK_GROUP_MAIN
										, sizeof(DMS_OPT_MAIN_WORK)
										, "OPT_MAIN"
										);
	
	// ワーク初期化
	main_work = (DMS_OPT_MAIN_WORK *)mtTaskGetTcbWork(dm_opt_mgr_p->tcb);
	
	// AO描画方法設定
	if (dm_opt_is_pause_maingame) {
		main_work->draw_state = (u32)AoActSysGetDrawStateEnable();
		
		if (main_work->draw_state) {
			dm_opt_draw_state = AoActSysGetDrawState();
		}
	}
	else {
		main_work->draw_state = TRUE;
		dm_opt_draw_state = DMD_OPT_DRAW_STATE_ID;
	}
	
	AoActSysSetDrawStateEnable((int)main_work->draw_state);
	
	if (main_work->draw_state) {
		dm_opt_draw_state = AoActSysGetDrawState();
	}
	
	// 初期化処理があればここに記述
	dmOptSetInitDispOptionData(main_work);

	// プロシージャ設定
	main_work->proc_update = dmOptLoadFontData;
	
//	DmLoadingStart();
	
	// タスク開始
//	amTaskStart(tcb);
}



// ==========================================================================
// dmOptSetSaveOptionData
/*!
	セーブされているオプション設定データのロード設定処理
 */
// ==========================================================================
void dmOptSetSaveOptionData(DMS_OPT_MAIN_WORK *main_work)
{
	int vol_bgm = 0;
	int vol_se = 0;
//	bool is_vib = false;
	
	// バックアップデータ取得
	gs::backup::SOption &data = gs::backup::SOption::CreateInstance();
	
	vol_bgm = (int)data.GetVolumeBgm();
	vol_se = (int)data.GetVolumeSe();
	
	// 各設定データ取得(0除算防止)
	if (vol_bgm != 0) {
		main_work->volume_data[0] = (int)(vol_bgm / 10);
	}
	else {
		main_work->volume_data[0] = 0;
	}
	
	if (vol_se != 0) {
		main_work->volume_data[1] = (int)(vol_se / 10);
	}
	else {
		main_work->volume_data[1] = 0;
	}
	
#if 0
	is_vib = data.IsVibration();
	
	if (is_vib) {		// ※※※※※仕様変更により削除予定
		main_work->set_vbrt = DME_OPT_CTRL_VBRT_ON;
	}
	else {
		main_work->set_vbrt = DME_OPT_CTRL_VBRT_OFF;
	}
#endif
	
	// 初期ボリューム設定
	data.SetVolumeBgm((u32)main_work->volume_data[0] * 10);
	data.SetVolumeSe((u32)main_work->volume_data[1] * 10);
}



// ==========================================================================
// dmOptSetInitDispOptionData
/*!
	オプション画面における初期表示位置設定処理
 */
// ==========================================================================
void dmOptSetInitDispOptionData(DMS_OPT_MAIN_WORK *main_work)
{
	// リージョンデータ取得(ボタン表示切り替え用)
	if (GeEnvGetDecideKey() == GSD_DECIDE_KEY_O) {
		main_work->is_jp_region = TRUE;
	}
	else {
		main_work->is_jp_region = FALSE;
	}
	
	main_work->vol_icon_col.r = 255;
	main_work->vol_icon_col.g = 255;
	main_work->vol_icon_col.b = 0;
	main_work->vol_icon_col.a = 255;
	
	main_work->win_col.r = 0;
	main_work->win_col.g = 0;
	main_work->win_col.b = 0;
	main_work->win_col.a = 255;
	
	main_work->win_size_rate[0] = 0.f;
	main_work->win_size_rate[1] = 0.f;
	
	
	// 操作方法画面の左側の項目初期表示位置設定
	main_work->ctrl_tab_pos_x[0] = dm_opt_ctrl_nrml_disp_pos_tbl[main_work->ctrl_mode][0];
	main_work->ctrl_tab_pos_y[0] = dm_opt_ctrl_nrml_disp_pos_tbl[main_work->ctrl_mode][1];
	main_work->ctrl_tab_pos_x[1] = dm_opt_ctrl_clsc_disp_pos_tbl[main_work->ctrl_mode][0];
	main_work->ctrl_tab_pos_y[1] = dm_opt_ctrl_clsc_disp_pos_tbl[main_work->ctrl_mode][1];
	
	main_work->decide_menu_col.r = 255;
	main_work->decide_menu_col.g = 255;
	main_work->decide_menu_col.b = 255;
	main_work->decide_menu_col.a = 0;
	
	main_work->prev_nrml_disp_type = 2;
	
	main_work->top_crsr_pos_y = DMD_OPT_TOP_MENU_TOP_POS_Y;
	
	// フレーム更新率設定		// 60F用の仮設定
	main_work->frm_update_time = 1.0f;
	
	
	// 帯テキストの初期表示位置設定
	main_work->obi_tex_pos[0] = 0.f;
	main_work->obi_tex_pos[1] = DMD_OPT_OBI_MOVE_START_POS;
}



// ==========================================================================
// dmOptProcMain
/*!
	オプション画面メインプロシージャ処理
 */
// ==========================================================================
void dmOptProcMain(MTS_TASK_TCB *tcb)
{
	DMS_OPT_MAIN_WORK	*main_work;

	// ワーク取得
	main_work = (DMS_OPT_MAIN_WORK *)mtTaskGetTcbWork(tcb);

	// 終了処理
	if (main_work->flag & DMD_OPT_FLAG_EXIT) {
		// タスククリア
		mtTaskClearTcb(tcb);
		
		dm_opt_mgr_p = NULL;
		
		// メインゲーム中の場合、タスクポーズ解除
		if (dm_opt_is_pause_maingame) {
			mtTaskEndPause();
		}
		
		// 保険
#if _PC || _WII
		DmUserNameExit();
#endif
		
		// イベント遷移用設定
		dmOptSetNextEvt(main_work);
	}
	
	// システム関連処理(サインアウト時はタイトルへ戻す)
	if (main_work->flag & DMD_OPT_FLAG_SIGN_OUT_EXIT
		&& !AoAccountIsCurrentEnable()) {
		
		main_work->proc_update = dmOptProcFadeOut;
		
		// サインアウト終了フラグOFF
		main_work->flag &= ~DMD_OPT_FLAG_SIGN_OUT_EXIT;
		
		main_work->next_evt = DME_OPT_NEXT_EVT_TITLE;
		
		// フェード終了		※問題あれば有効にする
//		IzFadeExit();
		
		// フェード処理開始
		if (dm_opt_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_TAKEOEVER
								, IZE_FADE_TYPE_BLACK_FADEOUT
								, DMD_OPT_FADEOUT_TIME
								, TRUE
								);
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_TAKEOEVER
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_OPT_FADEOUT_TIME
						   );
		}
		
		// BGMフェードアウト開始
		DmSndBgmPlayerExit();
		main_work->flag |= DMD_OPT_FLAG_DEMO_SND_END;
		
		// ウインドウ遷移関連設定
		main_work->flag &= ~DMD_OPT_FLAG_DECIDE;
		main_work->flag &= ~DMD_OPT_FLAG_CANCEL;
		main_work->proc_input = NULL;
		main_work->win_timer = 0;
	}
	
	
	// メニュー処理用プロシージャ
	if (main_work->proc_update) {
		main_work->proc_update(main_work);
	}

	// 描画設定プロシージャ
	if (main_work->proc_draw) {
		main_work->proc_draw(main_work);
	}

}


// ==========================================================================
// dmOptDest
/*!
	オプション画面終了処理
 */
// ==========================================================================
void dmOptDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	
}



// ==========================================================================
// dmOptSetNextEvt
/*!
	次のイベント遷移設定処理
 */
// ==========================================================================
void dmOptSetNextEvt(DMS_OPT_MAIN_WORK *main_work)
{
	s16 tmp_prev_evt = 0;
	
	UNREFERENCED_PARAMETER(main_work);
	
	if (main_work->flag & DMD_OPT_FLAG_CHNG_STAFFROLL) {
		tmp_prev_evt = GSD_EVT_ID_STAFFROLL;
		dm_opt_prev_evt = SyGetEvtInfo()->old_evt_id;
		
		main_work->flag &= ~DMD_OPT_FLAG_CHNG_STAFFROLL;
	}
	
	else {
		tmp_prev_evt = SyGetEvtInfo()->old_evt_id;
		
		if (tmp_prev_evt == GSD_EVT_ID_STAFFROLL) {
			tmp_prev_evt = dm_opt_prev_evt;
		}
	}
	
	// ここで一つ前のイベントがタイトルの場合、メインメニューへ切り替える(暫定)
	if (tmp_prev_evt == GSD_EVT_ID_TITLE) {
		tmp_prev_evt = GSD_EVT_ID_MAINMENU;
	}
	
	if (main_work->next_evt == DME_OPT_NEXT_EVT_TITLE) {
		tmp_prev_evt = GSD_EVT_ID_TITLE;
	}
	
	// メインゲーム中以外の場合、イベント遷移
	if (!dm_opt_is_pause_maingame) {
		SyDecideEvt(tmp_prev_evt);
		SyChangeNextEvt();
	}
}



// ==========================================================================
// dmOptLoadFontData
/*!
	フォントデータ読み込みリクエスト処理
 */
// ==========================================================================
void dmOptLoadFontData(DMS_OPT_MAIN_WORK *main_work)
{
	// gsFont構築
	GsFontBuild();
	
	main_work->proc_update = dmOptIsLoadFontData;
}



// ==========================================================================
// dmTitleIsLoadFontData
/*!
	フォントデータ読み込み終了チェック処理
 */
// ==========================================================================
void dmOptIsLoadFontData(DMS_OPT_MAIN_WORK *main_work)
{
	// gsFont構築
	if (GsFontIsBuilded()) {
		main_work->proc_update = dmOptLoadRequest;
		
		return;
	}
}



// ==========================================================================
// dmOptLoadRequest
/*!
	ファイル読み込みリクエスト処理
 */
// ==========================================================================
void dmOptLoadRequest(DMS_OPT_MAIN_WORK *main_work)
{
	main_work->arc_amb_fs[0] = amFsReadBackground(GSS_BASE_PATH "DEMO/OPTION/D_OPTION.AMB");
	main_work->arc_amb_fs[1] = amFsReadBackground((char *)dm_opt_main_lng_amb_name_tbl[GsEnvGetLanguage()]);

	// メニュー共通データ読み込み
	for (int i = 0; i < 4; i++) {
		main_work->arc_cmn_amb_fs[i] = amFsReadBackground((char *)dm_opt_menu_cmn_amb_name_tbl[i]);
	}

	main_work->arc_cmn_amb_fs[4] = amFsReadBackground((char *)dm_opt_menu_cmn_lng_amb_name_tbl[GsEnvGetLanguage()]);

#if _PC || _WII
	main_work->user_arc_amb_fs[0] = amFsReadBackground(GSS_BASE_PATH "DEMO/OPTION/D_OPTION_USER.AMB");
	main_work->user_arc_amb_fs[1] = amFsReadBackground((char *)dm_opt_user_lng_amb_name_tbl[GsEnvGetLanguage()]);
#endif
	
#if _PC || _PS3 || _XBOX || _IPHONE
	main_work->manual_arc_amb_fs[0] = amFsReadBackground(GSS_BASE_PATH "DEMO/MANUAL/D_MANUAL.AMB");
	main_work->manual_arc_amb_fs[1] = amFsReadBackground((char *)dm_opt_manual_file_lng_amb_name_tbl[GsEnvGetLanguage()]);
#endif
	
	// 次へ遷移
	main_work->proc_update = dmOptProcLoadWait;
}


// ==========================================================================
// dmOptProcLoadWait
/*!
	ファイル読み込み待ち処理
 */
// ==========================================================================
void dmOptProcLoadWait(DMS_OPT_MAIN_WORK *main_work)
{
	// ファイル読み込み完了待ち
	if (dmOptIsDataLoad(main_work)) {		// ファイル読込み完了チェック関数にする

		// ファイル取得
		for (int i = 0; i < DME_OPT_DATA_TYPE_MAX; i++) {
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

			// i構築開始
			AoTexBuild(&main_work->tex[i], main_work->amb[i]);
			AoTexLoad(&main_work->tex[i]);
		}

#if _PC || _WII
		for (int i = 0; i < 2; i++) {
			main_work->user_arc_amb[i] = main_work->user_arc_amb_fs[i]->buf;
			main_work->user_arc_amb_fs[i]->buf = NULL;
			
			// リクエストクリア
			amFsClearRequest(main_work->user_arc_amb_fs[i]);
			main_work->user_arc_amb_fs[i] = NULL;
		}
		
		// ユーザ名設定画面のデータ構築
		DmUserNameBuild(main_work->user_arc_amb);
#endif

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
		
		
		// GsFont構築
		if (!dm_opt_is_pause_maingame) {
			GsFontBuild();
			
			// サウンド構築
			DmSndBgmPlayerInit();
//			DmSoundBuild();
		}
		
		
		// 次へ遷移
		main_work->proc_update = dmOptProcLoadWait2;
	}
}



// ==========================================================================
// dmOptProcLoadWait2
/*!
	ファイル読み込み待ち処理
 */
// ==========================================================================
void dmOptProcLoadWait2(DMS_OPT_MAIN_WORK *main_work)
{
	// ファイル読み込み完了待ち
	if (dmOptIsTexLoad(main_work) == 1) {

#if _PC || _PS3 || _XBOX || _IPHONE
		for (int i = 0; i < 2; i++) {
			main_work->manual_arc_amb[i] = main_work->manual_arc_amb_fs[i]->buf;
			main_work->manual_arc_amb_fs[i]->buf = NULL;
			
			// リクエストクリア
			amFsClearRequest(main_work->manual_arc_amb_fs[i]);
			main_work->manual_arc_amb_fs[i] = NULL;
		}
		
		// オンラインマニュアル画面のデータ構築
		DmManualBuild(main_work->manual_arc_amb);
#endif
		
		// 次へ遷移
		main_work->proc_update = dmOptProcTexBuildWait;
	}
}




// ==========================================================================
// dmOptProcTexBuildWait
/*!
	テクスチャ構築待ち処理
 */
// ==========================================================================
void dmOptProcTexBuildWait(DMS_OPT_MAIN_WORK *main_work)
{
	// テクスチャ構築完了判定
	if (dmOptIsTexLoad2(main_work) == 1) {
		// 次へ遷移
		main_work->proc_update = dmOptProcCheckLoadingEnd;
		
		// ローディング終了設定
//		DmLoadingSetLoadComplete();
		
		if (!dm_opt_is_pause_maingame) {
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
// dmOptProcCheckLoadingEnd
/*!
	ローディング終了待ち処理
 */
// ==========================================================================
void dmOptProcCheckLoadingEnd(DMS_OPT_MAIN_WORK *main_work)
{
	// テクスチャ構築完了判定
//	if (DmLoadingIsExit()) {
		// 次へ遷移
		main_work->proc_update = dmOptProcCreateAct;
		

		// フェード処理開始
		if (dm_opt_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_BLACK_FADEIN
								, DMD_OPT_FADEIN_TIME
								, TRUE
								);
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEIN
						   , DMD_OPT_FADEIN_TIME
						   );
		}
//	}
}


// ==========================================================================
// dmOptProcCreateAct
/*!
	アクション生成処理

  	※cur_fileを設定する際は必ずcrsr_idxとcur_vrtcl_fileを設定して
  	それらの和を設定すること。
 */
// ==========================================================================
void dmOptProcCreateAct(DMS_OPT_MAIN_WORK *main_work)
{
	// ファイル選別
	const void *ama;
	AOS_TEXTURE *tex;
	
	// アクション構築
	for (u32 i = 0; i < ACT_NUM; ++i) {
		
#if !_IPHONE
		if (i >= ACT_TEX_WINTITLE) {
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
#else //!_IPHONE
		if (i >= ACT_TEX_BACK) {
			ama = main_work->cmn_ama[4];
			tex = &main_work->cmn_tex[4];
		}
#endif //!_IPHONE
		else if (i >= ACT_BTN_CANCEL1) {
			ama = main_work->cmn_ama[1];
			tex = &main_work->cmn_tex[1];
		}
		else if (i >= ACT_WAVE_BG) {
			ama = main_work->cmn_ama[0];
			tex = &main_work->cmn_tex[0];
		}
		
#if _WII
		else if (i == ACT_TEX_OBI_USER1
				 || i == ACT_TEX_OBI_USER2) {
			ama = DmUserNameGetUserAma();
			tex = DmUserNameGetUserLangTex();
		}
#endif
		
#if _IPHONE
		else if (ACT_BACK_BTN01_L <= i && i <= ACT_BACK_BTN01_R) {
			ama = main_work->cmn_ama[3];
			tex = &main_work->cmn_tex[3];
		}
#endif //_IPHONE
		else if (i >= ACT_TEX_TITLE) {
			ama = main_work->ama[DME_OPT_DATA_TYPE_LANG_DATA];
			tex = &main_work->tex[DME_OPT_DATA_TYPE_LANG_DATA];
		}
		else {
			ama = main_work->ama[DME_OPT_DATA_TYPE_CMN_DATA];
			tex = &main_work->tex[DME_OPT_DATA_TYPE_CMN_DATA];
		}
		
		// 構築
		AoActSetTexture(AoTexGetTexList(tex));
		main_work->act[i] = AoActCreate(ama, g_dm_act_id_tbl[i]);
	}
#if _IPHONE	//当たり判定構築
	{
		for (er::CTrgAoAction *slct = main_work->trg_slct, *slct_end = main_work->trg_slct + arrayof(main_work->trg_slct); slct != slct_end; ++slct) {
			new(slct) er::CTrgAoAction();
			slct->Create(main_work->act[ACT_TAB_SELECT]);
		}
		for (int i = 0, max = arrayof(main_work->trg_bgm_btn); i < max; ++i) {
			int c_act_id_table[] = {ACT_ICON_VOL_DOWN, ACT_ICON_VOL_UP};

			er::CTrgAoAction &bgm_btn = main_work->trg_bgm_btn[i];
			new(&bgm_btn) er::CTrgAoAction();
			bgm_btn.Create(main_work->act[c_act_id_table[i]]);

			er::CTrgAoAction &se_btn = main_work->trg_se_btn[i];
			new(&se_btn) er::CTrgAoAction();
			se_btn.Create(main_work->act[c_act_id_table[i]]);
		}
		{
			er::CTrgRect &slider = main_work->trg_bgm_slider;
			new(&slider) er::CTrgRect();
			slider.Create(42,70,426,124);
			slider.SetMoveThreshold(30);
		}
		{
			er::CTrgRect &slider = main_work->trg_se_slider;
			new(&slider) er::CTrgRect();
			slider.Create(42,136,436,180);
			slider.SetMoveThreshold(30);
		}
		{
			er::CTrgAoAction &ret = main_work->trg_return;
			new(&ret) er::CTrgAoAction();
			ret.Create(main_work->act[ACT_BACK_BTN01_R]);
		}
		for (int i = 0, max = arrayof(main_work->trg_ctrl_btn); i < max; ++i) {
			int c_act_id_table[] = {ACT_CNT01_C, ACT_CNT02_C};

			er::CTrgAoAction &btn = main_work->trg_ctrl_btn[i];
			new(&btn) er::CTrgAoAction();
			btn.Create(main_work->act[c_act_id_table[i]]);
		}
		//操作方法ウインドウ
		for (int i = 0, max = arrayof(main_work->ctrl_win_trg_btn); i < max; ++i) {
			int c_act_id_table[] = {ACT_A_TYPE_BTN_C, ACT_B_TYPE_BTN_C};

			er::CTrgAoAction &btn = main_work->ctrl_win_trg_btn[i];
			new(&btn) er::CTrgAoAction();
			btn.Create(main_work->act[c_act_id_table[i]]);
		}
	}
#endif //_IPHONE	//当たり判定構築
	
	// ここで背景の大アイコンアクション作成
	ama = main_work->ama[DME_OPT_DATA_TYPE_CMN_DATA];
	tex = &main_work->tex[DME_OPT_DATA_TYPE_CMN_DATA];
	
	// 帯初期表示位置設定
	main_work->obi_pos_y = DMD_OPT_OBI_NODISP_POS_Y;
	
	// テクスチャセットまで出来たので描画プロシージャを設定
	main_work->proc_draw = dmOptProcActDraw;
	main_work->proc_menu_draw = dmOptTopMenuDraw;
	
	// イベント遷移
	main_work->proc_update = dmOptProcFadeIn;
	
	main_work->flag |= DMD_OPT_FLAG_SIGN_OUT_EXIT;
	
#if _WII
	// ユーザーネーム側にメニュー共通データを渡す(ボタン、ウインドウ、テキストの順)
	DmUserNameSetMenuCmnAmaData(main_work->cmn_ama[1]
								, main_work->cmn_ama[3]
								, main_work->cmn_ama[4]
								);
	
	DmUserNameSetMenuCmnAmbData(main_work->cmn_amb[1]
								, main_work->cmn_amb[3]
								, main_work->cmn_amb[4]
								);
	
	DmUserNameSetMenuCmnTexData(&main_work->cmn_tex[1]
								, &main_work->cmn_tex[3]
								, &main_work->cmn_tex[4]
								);
#endif
	
	// メインゲーム中でないとき
	if (!dm_opt_is_pause_maingame) {
		DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_MENU);
//		DmSoundPlayMenuBGM(DME_SOUND_BGM_IDX_MENU
//						   , DMD_OPT_BGM_FADEIN_TIME);
	}
	// メインゲーム中のとき
	else {
		// ゲーム中のBGM再生
		GsSoundPlayBgm(main_work->bgm_scb
						, "snd_sng_menu"
						, DMD_OPT_BGM_FADEIN_TIME
					   );
	}
}



// ==========================================================================
// dmOptProcFadeIn
/*!
	フェードイン中処理
 */
// ==========================================================================
void dmOptProcFadeIn(DMS_OPT_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
		IzFadeExit();
		
		// 入力処理設定
		main_work->proc_input = dmOptInputProcTopMenu;
		
		main_work->proc_update = dmOptProcTopMenuIdle;
		
	}
}



// ==========================================================================
// dmOptProcTopMenuIdle
/*!
	オプション・トップメニュー時の入力待ち中処理
 */
// ==========================================================================
void dmOptProcTopMenuIdle(DMS_OPT_MAIN_WORK *main_work)
{
	// 入力処理
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}

#if _IPHONE
	{
		er::CTrgAoAction &ret = main_work->trg_return;
		float frame = main_work->act[ACT_BACK_BTN01_L]->frame;
		if (ret.GetState(0)[er::CTrgState::EState::Up] && ret.GetState(0)[er::CTrgState::EState::Prev]) {
			frame = 2.0f;
		} else if (ret.GetState(0)[er::CTrgState::EState::On]) {
			frame = 1.0f;
		} else if (2.0f <= main_work->act[ACT_BACK_BTN01_L]->frame) {
			//決定演出中なら続ける
			//何もしない
		} else {
			frame = 0.0f;
		}

		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
		for (int i = ACT_BACK_BTN01_L; i <= ACT_BACK_BTN01_R; i++) {
			AoActSetFrame(main_work->act[i], frame);
			AoActUpdate(main_work->act[i], 0.0f);
		}
	}
#endif //_IPHONE

	// キャンセルフラグONならば
	if (main_work->flag & DMD_OPT_FLAG_CANCEL) {
		// タイトル側へ戻る(掃け演出開始)
		main_work->proc_update = dmOptProcFadeOut;
		
		// フェード処理開始
		if (dm_opt_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_BLACK_FADEOUT
								, DMD_OPT_FADEOUT_TIME
								, TRUE
								);
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_OPT_FADEOUT_TIME
						   );
		}
		
		// フラグOFF
		main_work->flag &= ~DMD_OPT_FLAG_DECIDE;
		main_work->flag &= ~DMD_OPT_FLAG_CANCEL;
		
		if (!dm_opt_is_pause_maingame) {
#if !defined(_IPHONE)
#if defined (GSD_DEBUG_DEMO_SELECT) || defined (GSD_DEBUG_DEMO_SELECT_TOP)
			DmSoundPlaySE("Cancel");
			
			DmSndBgmPlayerExit();
			main_work->flag |= DMD_OPT_FLAG_DEMO_SND_END;
#endif
#else //!defined(_IPHONE)
			DmSoundPlaySE("Cancel");
#endif //!defined(_IPHONE)
		}
		else {
			GsSoundPlaySe("Cancel", main_work->se_handle);
			
			GsSoundStopBgm(main_work->bgm_scb
						   , DMD_OPT_BGM_FADEOUT_TIME
						   );
		}
		
		return;
	}

	// 決定フラグONならば
	if (main_work->flag & DMD_OPT_FLAG_DECIDE) {
		// 決定演出へ移行
		main_work->proc_update = dmOptProcTopMenuDecideEfct;

		dmOptSetTopMenuDecideEfctData(main_work);

		// 決定SE再生
		if (!dm_opt_is_pause_maingame) {
			DmSoundPlaySE("Ok");
		}
		else {
			GsSoundPlaySe("Ok", main_work->se_handle);
		}

		// フラグOFF
		main_work->flag &= ~DMD_OPT_FLAG_DECIDE;
		main_work->flag &= ~DMD_OPT_FLAG_CANCEL;
		
		return;
	}

	// カーソル切り替え
	if (main_work->flag & DMD_OPT_FLAG_UP_CHNG_CRSR) {
		//
		
		main_work->cur_slct_top = dmOptGetRevisedTopMenuNo(main_work->cur_slct_top, -1);
		
		if (!dm_opt_is_pause_maingame) {
			DmSoundPlaySE("Cursol");
		}
		else {
			GsSoundPlaySe("Cursol", main_work->se_handle);
		}
		
		// カーソル移動設定
		main_work->flag |= DMD_OPT_FLAG_MENU_CRSR_EFCT;
		dmOptSetChngFocusCrsrData(main_work);
		
		// フラグOFF
		main_work->flag &= ~DMD_OPT_FLAG_UP_CHNG_CRSR;
		main_work->flag &= ~DMD_OPT_FLAG_DOWN_CHNG_CRSR;
	}
	
	if (main_work->flag & DMD_OPT_FLAG_DOWN_CHNG_CRSR) {
		//
		
		main_work->cur_slct_top = dmOptGetRevisedTopMenuNo(main_work->cur_slct_top, 1);
		
		if (!dm_opt_is_pause_maingame) {
			DmSoundPlaySE("Cursol");
		}
		else {
			GsSoundPlaySe("Cursol", main_work->se_handle);
		}
		
		// カーソル移動設定
		main_work->flag |= DMD_OPT_FLAG_MENU_CRSR_EFCT;
		dmOptSetChngFocusCrsrData(main_work);
		
		// フラグOFF
		main_work->flag &= ~DMD_OPT_FLAG_UP_CHNG_CRSR;
		main_work->flag &= ~DMD_OPT_FLAG_DOWN_CHNG_CRSR;
	}
	
	// カーソル移動演出
	if (main_work->flag & DMD_OPT_FLAG_MENU_CRSR_EFCT) {
		dmOptSetCtrlFocusChangeEfct(main_work);
		
		if (dmOptIsCtrlFocusChangeEfctEnd(main_work)) {
			main_work->flag &= ~DMD_OPT_FLAG_MENU_CRSR_EFCT;
		}
	}
}



// ==========================================================================
// dmOptProcTopMenuDecideEfct
/*!
	オプション・トップメニュー時の決定演出中処理
 */
// ==========================================================================
void dmOptProcTopMenuDecideEfct(DMS_OPT_MAIN_WORK *main_work)
{
#if !_IPHONE
	// 決定演出処理
	dmOptSetTopMenuTabDecideEfct(main_work);
#endif //!_IPHONE

	// 決定演出が終了したら
	if (dmOptIsTopMenuTabDecideEfctEnd(main_work)) {
		// ここは決定した項目別に切り分ける。
		dmOptSetNextProcFunc(main_work);

		// カーソル初期設定
//		main_work->crsr_idx = 0;

//		main_work->focus_disp_no = 0;

		main_work->timer = 0.0f;

		return;
	}
	
	// カーソル移動演出
	if (main_work->flag & DMD_OPT_FLAG_MENU_CRSR_EFCT) {
		dmOptSetCtrlFocusChangeEfct(main_work);
		
		if (dmOptIsCtrlFocusChangeEfctEnd(main_work)) {
			main_work->flag &= ~DMD_OPT_FLAG_MENU_CRSR_EFCT;
		}
	}
	
	main_work->timer++;
}



#if _PC || _PS3 || _XBOX || _IPHONE
// ==========================================================================
// dmOptProcManualStartFadeOut
/*!
	オンラインマニュアルへの処理
 */
// ==========================================================================
void dmOptProcManualStartFadeOut(DMS_OPT_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		main_work->proc_update = dmOptProcManualIdle;
		
		// サインアウト終了フラグOFF
		main_work->flag &= ~DMD_OPT_FLAG_SIGN_OUT_EXIT;
		
		// 完全版購入画面起動
		DmManualStart();

#if _IPHONE
		//オプションの描画を止める
		main_work->proc_menu_draw = NULL;
#endif //_IPHONE
	}
}



// ==========================================================================
// dmOptProcManualIdle
/*!
	オンラインマニュアル中処理
 */
// ==========================================================================
void dmOptProcManualIdle(DMS_OPT_MAIN_WORK *main_work)
{
	if (DmManualIsExit()) {
		main_work->proc_update = dmOptProcManualEndFadeIn;
#if _IPHONE
		main_work->proc_menu_draw = dmOptTopMenuDraw;
#endif //_IPHONE
		
		// サインアウト終了フラグOFF
		main_work->flag |= DMD_OPT_FLAG_SIGN_OUT_EXIT;
		
		// フェード処理開始
		if (dm_opt_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_BLACK_FADEIN
								, DMD_OPT_FADEIN_TIME
								, TRUE
								);
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEIN
						   , DMD_OPT_FADEIN_TIME
						   );
		}
	}
}



// ==========================================================================
// dmOptProcManualEndFadeIn
/*!
	オンラインマニュアルからオプションへ戻るときの処理
 */
// ==========================================================================
void dmOptProcManualEndFadeIn(DMS_OPT_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		main_work->proc_update = dmOptProcTopMenuIdle;
		main_work->proc_input = dmOptInputProcTopMenu;
	}
}

#endif	// #if _PC || _PS3 || _XBOX || _IPHONE



// ==========================================================================
// dmOptProcSetMenuInEfct
/*!
	オプション・設定画面時の入り演出中処理
 */
// ==========================================================================
void dmOptProcSetMenuInEfct(DMS_OPT_MAIN_WORK *main_work)
{
	// ウインドウ演出
	if (main_work->flag & DMD_OPT_FLAG_WIN_EFCT_END) {
		// イベント遷移
		main_work->proc_update = dmOptProcSetMenuIdle;
		main_work->proc_input = dmOptInputProcSettingMenu;
		
		main_work->disp_flag |= DMD_OPT_DISP_FLAG_WIN_ACT;
		
		main_work->flag &= ~DMD_OPT_FLAG_WIN_EFCT_END;
	}
	else {
		dmOptSetWinOpenEfct(main_work);
	}
}



// ==========================================================================
// dmOptProcSetMenuIdle
/*!
	オプション・設定画面時の入力待ち中処理
 */
// ==========================================================================
void dmOptProcSetMenuIdle(DMS_OPT_MAIN_WORK *main_work)
{
//	bool tmp_set_vbrt = false;
	
	// 入力処理
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}

#if _IPHONE
	{
		er::CTrgAoAction &ret = main_work->trg_return;
		float frame = main_work->act[ACT_BACK_BTN01_L]->frame;
		if (ret.GetState(0)[er::CTrgState::EState::Up] && ret.GetState(0)[er::CTrgState::EState::Prev]) {
			frame = 2.0f;
		} else if (ret.GetState(0)[er::CTrgState::EState::On]) {
			frame = 1.0f;
		} else if (2.0f <= main_work->act[ACT_BACK_BTN01_L]->frame) {
			//決定演出中なら続ける
			//何もしない
		} else {
			frame = 0.0f;
		}

		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
		for (int i = ACT_BACK_BTN01_L; i <= ACT_BACK_BTN01_R; i++) {
			AoActSetFrame(main_work->act[i], frame);
			AoActUpdate(main_work->act[i], 0.0f);
		}
	}
#endif //_IPHONE

	// キャンセルフラグONならば
#if _IPHONE //操作方法ウインドウ
	if ((main_work->flag & DMD_OPT_FLAG_CANCEL) && (DMD_OPT_FLAG_SETTING_CONTROL_WINDOW & main_work->flag)) {
		//ウインドウが開いていたら閉じる
		main_work->flag &= ~(DMD_OPT_FLAG_CANCEL | DMD_OPT_FLAG_SETTING_CONTROL_WINDOW);

		if (!dm_opt_is_pause_maingame) {
			DmSoundPlaySE("Cancel");
		}
		else {
			GsSoundPlaySe("Cancel", main_work->se_handle);
		}
		
		return;
	}
#endif //_IPHONE //操作方法ウインドウ
	if (main_work->flag & DMD_OPT_FLAG_CANCEL) {
		
		// ここで設定画面で設定したデータをセット
		gs::backup::SOption &data = gs::backup::SOption::CreateInstance();
		
		// 各設定データ設定
		data.SetVolumeBgm((u32)main_work->volume_data[0] * 10);
		data.SetVolumeSe((u32)main_work->volume_data[1] * 10);
		
#if 0
		if (main_work->set_vbrt) {
			tmp_set_vbrt = false;
		}
		else {
			tmp_set_vbrt = true;
		}
		data.SetVibration(tmp_set_vbrt);
#endif
#if _IPHONE
		{
			GSS_MAIN_SYS_INFO *main_sys_info = GsGetMainSysInfo();
			using namespace gs::backup;
			switch (data.GetControl()) {
			case SOption::EControl::VirtualPadDown:	//<バーチャルパッド(下)
				main_sys_info->game_flag &= ~GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK;
				main_sys_info->game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
				break;
			case SOption::EControl::VirtualPadUp:	//<バーチャルパッド(上)
				main_sys_info->game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK;
				main_sys_info->game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
				break;
			case SOption::EControl::Tilt:	//<傾斜
			default:
				main_sys_info->game_flag &= ~GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK;
				main_sys_info->game_flag &= ~GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
				break;
			}
		}
#endif //_IPHONE
		
		// オプショントップへ戻る(掃け演出開始)
		main_work->proc_update = dmOptProcSetMenuOutEfct;
		
		// ウインドウ演出時間設定
		main_work->win_timer = DMD_OPT_WIN_EFCT_TIME;
		
		main_work->disp_flag &= ~DMD_OPT_DISP_FLAG_WIN_ACT;
		
		if (!dm_opt_is_pause_maingame) {
			DmSoundPlaySE("Cancel");
		}
		else {
			GsSoundPlaySe("Cancel", main_work->se_handle);
		}
		
#if 0
		// 振動設定OFF
		AoPadEnableVibration(FALSE);
		AoPadSetVibration(0, 0);
#endif
		
		// フラグOFF
		main_work->flag &= ~DMD_OPT_FLAG_DECIDE;
		main_work->flag &= ~DMD_OPT_FLAG_CANCEL;
		
		return;
	}

	// デフォルト項目で決定フラグONならば
	if (main_work->flag & DMD_OPT_FLAG_DECIDE) {
		// デフォルト設定
		if (main_work->cur_slct_set == DME_OPT_SET_MENU_DEFAULT) {
			dmOptSetDefaultDataSetMenu(main_work);
		}
		
		if (!dm_opt_is_pause_maingame) {
			DmSoundPlaySE("Ok");
		}
		else {
			GsSoundPlaySe("Ok", main_work->se_handle);
		}
		
		// フラグOFF
		main_work->flag &= ~DMD_OPT_FLAG_DECIDE;
		main_work->flag &= ~DMD_OPT_FLAG_CANCEL;
		
		return;
	}

	
	// カーソル切り替え
	if (main_work->flag & DMD_OPT_FLAG_UP_CHNG_CRSR) {
		// 
		main_work->cur_slct_set = dmOptGetRevisedSettingMenuNo(main_work->cur_slct_set, -1);
		
		if (!dm_opt_is_pause_maingame) {
			DmSoundPlaySE("Cursol");
		}
		else {
			GsSoundPlaySe("Cursol", main_work->se_handle);
		}
		
		// フラグOFF
		main_work->flag &= ~DMD_OPT_FLAG_UP_CHNG_CRSR;
		main_work->flag &= ~DMD_OPT_FLAG_DOWN_CHNG_CRSR;
		
		return;
	}
	
	if (main_work->flag & DMD_OPT_FLAG_DOWN_CHNG_CRSR) {
		//
		
		main_work->cur_slct_set = dmOptGetRevisedSettingMenuNo(main_work->cur_slct_set, 1);
		
		if (!dm_opt_is_pause_maingame) {
			DmSoundPlaySE("Cursol");
		}
		else {
			GsSoundPlaySe("Cursol", main_work->se_handle);
		}
		
		// フラグOFF
		main_work->flag &= ~DMD_OPT_FLAG_UP_CHNG_CRSR;
		main_work->flag &= ~DMD_OPT_FLAG_DOWN_CHNG_CRSR;
		
		return;
	}
	
	dmOptSetVolPushEfct(main_work);
	
	if (main_work->flag & DMD_OPT_FLAG_PUSH_DFLT_BTN) {
		dmOptSetDfltPushEfct(main_work);
	}

#if _IPHONE //操作方法ウインドウ
	if (DMD_OPT_FLAG_SETTING_CONTROL_WINDOW & main_work->flag) {
		//開ける
		if (main_work->ctrl_win_window_prgrs < 1.0f) {
			main_work->ctrl_win_window_prgrs += 1.0f / DMD_OPT_WIN_EFCT_TIME;
			if (1.0 < main_work->ctrl_win_window_prgrs) {
				main_work->ctrl_win_window_prgrs = 1.0f;
			}
		}
	} else {
		//閉める
		if (0.0f < main_work->ctrl_win_window_prgrs) {
			main_work->ctrl_win_window_prgrs -= 1.0f / DMD_OPT_WIN_EFCT_TIME;
			if (main_work->ctrl_win_window_prgrs < 0.0f) {
				main_work->ctrl_win_window_prgrs = 0.0f;
			}
		}
	}
#endif //_IPHONE //操作方法ウインドウ
}



// ==========================================================================
// dmOptProcSetMenuOutEfct
/*!
	オプション・設定画面時の入り演出中処理
 */
// ==========================================================================
void dmOptProcSetMenuOutEfct(DMS_OPT_MAIN_WORK *main_work)
{
	// ウインドウ演出
	if (main_work->flag & DMD_OPT_FLAG_WIN_EFCT_END) {
		// オプショントップ側へ戻る
		main_work->proc_update = dmOptProcTopMenuIdle;
		
		main_work->proc_input = dmOptInputProcTopMenu;
		
		main_work->proc_menu_draw = dmOptTopMenuDraw;
		
		main_work->state = DME_OPT_MENU_STATE_TOP;
		
		main_work->flag &= ~DMD_OPT_FLAG_WIN_EFCT_END;
	}
	else {
		dmOptSetWinCloseEfct(main_work);
	}
}



// ==========================================================================
// dmOptProcCtrlMenuIdle
/*!
	オプション・操作方法画面時の入力待ち中処理
 */
// ==========================================================================
void dmOptProcCtrlMenuIdle(DMS_OPT_MAIN_WORK *main_work)
{
	// 入力処理
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}

#if _IPHONE
	{
		er::CTrgAoAction &ret = main_work->trg_return;
		float frame = main_work->act[ACT_BACK_BTN01_L]->frame;
		if (ret.GetState(0)[er::CTrgState::EState::Up] && ret.GetState(0)[er::CTrgState::EState::Prev]) {
			frame = 2.0f;
		} else if (ret.GetState(0)[er::CTrgState::EState::On]) {
			frame = 1.0f;
		} else if (2.0f <= main_work->act[ACT_BACK_BTN01_L]->frame) {
			//決定演出中なら続ける
			//何もしない
		} else {
			frame = 0.0f;
		}

		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
		for (int i = ACT_BACK_BTN01_L; i <= ACT_BACK_BTN01_R; i++) {
			AoActSetFrame(main_work->act[i], frame);
			AoActUpdate(main_work->act[i], 0.0f);
		}
	}
#endif //_IPHONE

	// キャンセルフラグONならば
	if (main_work->flag & DMD_OPT_FLAG_CANCEL) {
		// タイトル側へ戻る
		main_work->proc_input = dmOptInputProcTopMenu;
		main_work->proc_update = dmOptProcTopMenuIdle;
		main_work->proc_menu_draw = dmOptTopMenuDraw;

		main_work->state = DME_OPT_MENU_STATE_TOP;
		
		main_work->top_crsr_pos_y = DMD_OPT_TOP_MENU_TOP_POS_Y
									+ main_work->cur_slct_top * DMD_OPT_TOP_MENU_DIST_Y;
		
		if (!dm_opt_is_pause_maingame) {
			DmSoundPlaySE("Cancel");
		}
		else {
			GsSoundPlaySe("Cancel", main_work->se_handle);
		}
		
		// フラグOFF
		main_work->flag &= ~DMD_OPT_FLAG_DECIDE;
		main_work->flag &= ~DMD_OPT_FLAG_CANCEL;
		
		return;
	}

}



// ==========================================================================
// dmOptProcUserMenuIdle
/*!
	オプション・ユーザー名設定画面時の入力待ち中処理
 */
// ==========================================================================
#if _PC || _WII
void dmOptProcUserMenuIdle(DMS_OPT_MAIN_WORK *main_work)
{
	// キャンセルフラグONならば
	if (DmUserNameIsExit()) {
		// タイトル側へ戻る
		main_work->proc_input = dmOptInputProcTopMenu;
		main_work->proc_update = dmOptProcTopMenuIdle;
		main_work->proc_menu_draw = dmOptTopMenuDraw;

		main_work->state = DME_OPT_MENU_STATE_TOP;

		return;
	}
}
#endif



// ==========================================================================
// dmOptProcStfrlStartFadeOut
/*!
	スタッフロールフェードイン中処理
 */
// ==========================================================================
void dmOptProcStfrlStartFadeOut(DMS_OPT_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		main_work->proc_update = dmOptProcStfrlIdle;
		
		main_work->proc_draw = NULL;
		
		// サインアウト終了フラグOFF
		main_work->flag &= ~DMD_OPT_FLAG_SIGN_OUT_EXIT;
		
		// スタッフロール画面起動
		DmStaffRollStart(NULL);
	}
}



// ==========================================================================
// dmOptProcStfrlIdle
/*!
	スタッフロール中処理
 */
// ==========================================================================
void dmOptProcStfrlIdle(DMS_OPT_MAIN_WORK *main_work)
{
	if (DmStaffRollIsExit()) {
		main_work->proc_update = dmOptProcStfrlEndFadeIn;
		
		main_work->proc_draw = dmOptProcActDraw;
		
		AoActSysSetDrawStateEnable(TRUE);
		AoActSysSetDrawState(dm_opt_draw_state);
		
		// サインアウト終了フラグOFF
		main_work->flag |= DMD_OPT_FLAG_SIGN_OUT_EXIT;
		
		// フェード処理開始
		if (dm_opt_is_pause_maingame) {
			// ゲーム中のBGM再生
			GsSoundPlayBgm(main_work->bgm_scb
							, "snd_sng_menu"
							, DMD_OPT_BGM_FADEIN_TIME
						   );
			
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_BLACK_FADEIN
								, DMD_OPT_FADEIN_TIME
								, TRUE
								);
		}
		else {
			DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_MENU);
			// オプションでのサウンドを再度再生開始
//			DmSoundPlayMenuBGM(DME_SOUND_BGM_IDX_MENU
//							   , DMD_OPT_BGM_FADEIN_TIME);
			
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEIN
						   , DMD_OPT_FADEIN_TIME
						   );
		}
	}
}



// ==========================================================================
// dmOptProcStfrlEndFadeIn
/*!
	スタッフロールからオプションへ戻るときの処理
 */
// ==========================================================================
void dmOptProcStfrlEndFadeIn(DMS_OPT_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
		IzFadeExit();
		
		main_work->flag |= DMD_OPT_FLAG_SIGN_OUT_EXIT;
		
		main_work->proc_update = dmOptProcTopMenuIdle;
		main_work->proc_input = dmOptInputProcTopMenu;
	}
}



// ==========================================================================
// dmOptProcFadeOut
/*!
	フェードアウト中処理
 */
// ==========================================================================
void dmOptProcFadeOut(DMS_OPT_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
//		IzFadeExit();

		// 遷移先なし
//		main_work->proc_win_update = NULL;
		main_work->proc_update = dmOptProcStopDraw;
		main_work->proc_draw = NULL;

		main_work->timer = 0;

		
		if (!dm_opt_is_pause_maingame) {
//			DmSoundExit();
		}
		else {
			GsSoundStopBgm(main_work->bgm_scb, 0);
			GsSoundResignScb(main_work->bgm_scb);
			main_work->bgm_scb	= NULL;
			
			GsSoundFreeSeHandle(main_work->se_handle);
			main_work->se_handle = NULL;
		}
		
		return;
	}
}



// ==========================================================================
// dmOptProcStopDraw
/*!
	描画停止処理
 */
// ==========================================================================
void dmOptProcStopDraw(DMS_OPT_MAIN_WORK *main_work)
{
	main_work->proc_update = dmOptProcDataRelease;
}

// ==========================================================================
// dmOptProcDataRelease
/*!
	ファイル解放リクエスト処理
 */
// ==========================================================================
void dmOptProcDataRelease(DMS_OPT_MAIN_WORK *main_work)
{
	// テクスチャ解放
	for (int i = 0; i < DME_OPT_DATA_TYPE_MAX; i++) {
		AoTexRelease(&main_work->tex[i]);
	}

	// メニュー共通テクスチャ解放
	for (int i = 0; i < 5; i++) {
		AoTexRelease(&main_work->cmn_tex[i]);
	}

	// ウインドウテクスチャ解放
//	AoTexRelease(&dm_opt_win_tex);

#if _PC || _WII
	// ユーザー名関連のテクスチャ解放
	DmUserNameFlush();
#endif

#if _PC || _PS3 || _XBOX || _IPHONE
	DmManualFlush();
#endif
	
	if (!dm_opt_is_pause_maingame) {
//		DmSoundFlush();
	}

	// 次へ遷移
	main_work->proc_update = dmOptProcFinish;
}


// ==========================================================================
// dmOptProcFinish
/*!
	終了処理
 */
// ==========================================================================
void dmOptProcFinish(DMS_OPT_MAIN_WORK *main_work)
{
	// テクスチャ解放完了判定
	if (dmOptIsTexRelease(main_work) == 1) {

#if _IPHONE	//当たり判定解放
		{
			for (er::CTrgAoAction *slct = main_work->trg_slct, *slct_end = main_work->trg_slct + arrayof(main_work->trg_slct); slct != slct_end; ++slct) {
				slct->Release();
				slct->~CTrgAoAction();
			}
			for (er::CTrgAoAction *bgm_btn = main_work->trg_bgm_btn, *bgm_btn_end = main_work->trg_bgm_btn + arrayof(main_work->trg_bgm_btn); bgm_btn != bgm_btn_end; ++bgm_btn) {
				bgm_btn->Release();
				bgm_btn->~CTrgAoAction();
			}
			for (er::CTrgAoAction *se_btn = main_work->trg_se_btn, *se_btn_end = main_work->trg_se_btn + arrayof(main_work->trg_se_btn); se_btn != se_btn_end; ++se_btn) {
				se_btn->Release();
				se_btn->~CTrgAoAction();
			}
			{
				er::CTrgRect &slider = main_work->trg_bgm_slider;
				slider.Release();
				slider.~CTrgRect();
			}
			{
				er::CTrgRect &slider = main_work->trg_se_slider;
				slider.Release();
				slider.~CTrgRect();
			}
			{
				er::CTrgAoAction &ret = main_work->trg_return;
				ret.Release();
				ret.~CTrgAoAction();
			}
			for (er::CTrgAoAction *btn = main_work->trg_ctrl_btn, *btn_end = main_work->trg_ctrl_btn + arrayof(main_work->trg_ctrl_btn); btn != btn_end; ++btn) {
				btn->Release();
				btn->~CTrgAoAction();
			}
			//操作方法ウインドウ
			for (er::CTrgAoAction *btn = main_work->ctrl_win_trg_btn, *btn_end = main_work->ctrl_win_trg_btn + arrayof(main_work->ctrl_win_trg_btn); btn != btn_end; ++btn) {
				btn->Release();
				btn->~CTrgAoAction();
			}
		}
#endif //_IPHONE	//当たり判定解放

		for (int i = 0; i < ACT_NUM; i++) {
			if (main_work->act[i]) {
				AoActDelete(main_work->act[i]);
				main_work->act[i] = NULL;
			}
		}
		
		// ファイル解放
		for (int i = 0; i < DME_OPT_DATA_TYPE_MAX; i++) {
			if (main_work->arc_amb[i]) {
				amMemFree(main_work->arc_amb[i]);
				main_work->arc_amb[i] = NULL;
			}
		}

		// 共通アクションファイル解放
		for (int i = 0; i < 5; i++) {
			// ファイル解放
			if (main_work->arc_cmn_amb[i]) {
				amMemFree(main_work->arc_cmn_amb[i]);
				main_work->arc_cmn_amb[i] = NULL;
			}
		}

#if _PC || _WII
		// ユーザー名データ
		for (int i = 0; i < DME_OPT_DATA_TYPE_MAX; i++) {
			if (main_work->user_arc_amb[i]) {
				amMemFree(main_work->user_arc_amb[i]);
				main_work->user_arc_amb[i] = NULL;
			}
		}
#endif

#if _PC || _PS3 || _XBOX || _IPHONE
		for (int i = 0; i < DME_OPT_DATA_TYPE_MAX; i++) {
			if (main_work->manual_arc_amb[i]) {
				amMemFree(main_work->manual_arc_amb[i]);
				main_work->manual_arc_amb[i] = NULL;
			}
		}
#endif
//		if (dm_opt_win_amb) {
//			amMemFree(dm_opt_win_amb);
//			dm_opt_win_amb = NULL;
//		}

//		main_work->proc_win_update = NULL;
		
		// セーブ開始
		DmSaveMenuStart(TRUE);
		
		main_work->proc_update = dmOptProcFinishWaitSave;
	}
}



// ==========================================================================
// dmOptProcFinishWaitSave
/*!
	終了処理
 */
// ==========================================================================
void dmOptProcFinishWaitSave(DMS_OPT_MAIN_WORK *main_work)
{
	if (DmSaveIsExit()) {
		// 終了処理へ
		main_work->proc_update = dmOptProcWaitFinished;
	}
}



// ==========================================================================
// dmOptProcWaitFinished
/*!
	終了処理
 */
// ==========================================================================
void dmOptProcWaitFinished(DMS_OPT_MAIN_WORK *main_work)
{
	// サウンド終了フラグONならば
	if (main_work->flag & DMD_OPT_FLAG_DEMO_SND_END) {
		// サウンド終了待ち
		if (DmSndBgmPlayerIsTaskExit()) {
			// 終了処理へ
			main_work->flag |= DMD_OPT_FLAG_EXIT;
			main_work->proc_update = NULL;
			
			main_work->flag &= ~DMD_OPT_FLAG_DEMO_SND_END;
		}
	}
	else {
		// 終了処理へ
		main_work->flag |= DMD_OPT_FLAG_EXIT;
		main_work->proc_update = NULL;
	}
}



// ==========================================================================
// dmOptInputProcTopMenu
/*!
	オプション・トップメニュー用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmOptInputProcTopMenu(DMS_OPT_MAIN_WORK *main_work)
{
	// キャンセル処理
#if !_IPHONE
	if (AoPadStand() & GSD_KEY_CANCEL) {
#else //!_IPHONE
	if (main_work->trg_return.GetState(0)[er::CTrgState::EState::Up] && main_work->trg_return.GetState(0)[er::CTrgState::EState::Prev]) {
#endif //!_IPHONE
		main_work->flag |= DMD_OPT_FLAG_CANCEL;

		return;
	}
	
#if !_IPHONE
	// 決定処理
	if (AoPadStand() & GSD_KEY_DECIDE) {
		main_work->flag |= DMD_OPT_FLAG_DECIDE;

		return;
	}

	// 十字キー操作
	if (AoPadMRepeat() & GSD_KEY_UP) {
		if (AoPadMStand() & GSD_KEY_UP
			|| main_work->cur_slct_top != 0) {
			
			main_work->flag |= DMD_OPT_FLAG_UP_CHNG_CRSR;
		}

		return;
	}
	
	else if (AoPadMRepeat() & GSD_KEY_DOWN) {
		if (AoPadMStand() & GSD_KEY_DOWN
			|| main_work->cur_slct_top != DME_OPT_TOP_MENU_NUM - 1) {
			
			main_work->flag |= DMD_OPT_FLAG_DOWN_CHNG_CRSR;
		}

		return;
	}
#else //!_IPHONE
	for (int i = 0, max = arrayof(main_work->trg_slct); i < max; ++i) {
		er::CTrgAoAction &slct = main_work->trg_slct[i];
		if (slct.GetState(0)[er::CTrgState::EState::Up] && slct.GetState(0)[er::CTrgState::EState::Prev]) {
			//選択
			main_work->cur_slct_top = i;
			main_work->flag |= DMD_OPT_FLAG_DECIDE;
			break;
		}
	}
#endif //!_IPHONE	
}



// ==========================================================================
// dmOptInputProcSettingMenu
/*!
	オプション・設定画面用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmOptInputProcSettingMenu(DMS_OPT_MAIN_WORK *main_work)
{
	// キャンセル処理
#if !_IPHONE
	if (AoPadStand() & GSD_KEY_CANCEL) {
#else //!_IPHONE
	if (main_work->trg_return.GetState(0)[er::CTrgState::EState::Up] && main_work->trg_return.GetState(0)[er::CTrgState::EState::Prev]) {
#endif //!_IPHONE
		main_work->flag |= DMD_OPT_FLAG_CANCEL;

		return;
	}
	
#if _IPHONE //操作方法ウインドウ
	if (!(DMD_OPT_FLAG_SETTING_CONTROL_WINDOW & main_work->flag)) {
		//操作方法ウインドウが開いていないなら
		using namespace gs::backup;
		{ //傾斜操作
			er::CTrgAoAction &btn = main_work->trg_ctrl_btn[0];			
			if (btn.GetState(0)[er::CTrgState::EState::Stand]) {
				SOption &option = SOption::CreateInstance();
				option.SetControl(SOption::EControl::Tilt);
#if _IPHONE
				//SE再生
				((!dm_opt_is_pause_maingame)? DmSoundPlaySE("Cursol"): GsSoundPlaySe("Cursol", main_work->se_handle));
#endif //_IPHONE
			}
		}
		{ //バーチャルパッド操作
			er::CTrgAoAction &btn = main_work->trg_ctrl_btn[1];			
			if (btn.GetState(0)[er::CTrgState::EState::Stand]) {
				SOption &option = SOption::CreateInstance();
				if (SOption::EControl::Tilt == option.GetControl()) {
					option.SetControl(SOption::EControl::VirtualPadDown);
				}
#if _IPHONE
				//SE再生
				((!dm_opt_is_pause_maingame)? DmSoundPlaySE("Cursol"): GsSoundPlaySe("Cursol", main_work->se_handle));
#endif //_IPHONE
			} else if (btn.GetState(0)[er::CTrgState::EState::Up] && btn.GetState(0)[er::CTrgState::EState::Prev]) {
				main_work->flag |= DMD_OPT_FLAG_SETTING_CONTROL_WINDOW; //操作方法ウインドウを開く
#if _IPHONE
				//SE再生
				((!dm_opt_is_pause_maingame)? DmSoundPlaySE("Window"): GsSoundPlaySe("Window", main_work->se_handle));
#endif //_IPHONE
				return;
			}
		}
	}
	if (0.0f < main_work->ctrl_win_window_prgrs) {
		if (1.0f == main_work->ctrl_win_window_prgrs) {
			using namespace gs::backup;
			SOption &option = SOption::CreateInstance();
			bool is_click = true;
			if (main_work->ctrl_win_trg_btn[0].GetState(0)[er::CTrgState::EState::Stand]) {
				option.SetControl(SOption::EControl::VirtualPadDown);
			} else if (main_work->ctrl_win_trg_btn[1].GetState(0)[er::CTrgState::EState::Stand]) {
				option.SetControl(SOption::EControl::VirtualPadUp);
			} else {
				is_click = false;
			}
#if _IPHONE
			//SE再生
			if (is_click) {
				((!dm_opt_is_pause_maingame)? DmSoundPlaySE("Cursol"): GsSoundPlaySe("Cursol", main_work->se_handle));
			}
#endif //_IPHONE
		}
		return;
	}
#endif //_IPHONE //操作方法ウインドウ
#if _IPHONE
	//音量調整
	bool is_click_default = false;
	bool is_click_volume[arrayof(main_work->trg_bgm_btn)] = {false, false};
	
	//ボリューム
	for (int i = 0, max = arrayof(main_work->trg_bgm_btn); i < max; ++i) {
		//ボタン
		for (int k = 0; k < 2; ++k) {
			er::CTrgAoAction *btn = ((0 == k)? main_work->trg_bgm_btn: main_work->trg_se_btn);
			if (btn[i].GetState(0)[er::CTrgState::EState::Lock]) {
				if (btn[i].GetState(0)[er::CTrgState::EState::Repeat]) {
					main_work->cur_slct_set = ((0 == k)? DME_OPT_SET_MENU_BGM: DME_OPT_SET_MENU_SE);
					is_click_volume[i] = true;
					break;
				}
			} else if (btn[i].GetState(0)[er::CTrgState::EState::Out]) {
				btn[i].DelLock();
			}
		}
		//スライダー
		for (int k = 0; k < 2; ++k) {
			er::CTrgRect &slider = ((0 == k)? main_work->trg_bgm_slider: main_work->trg_se_slider);
			const er::CTrgState &state = slider.GetState(0);
			if (state[er::CTrgState::EState::Move]) {
				er::CTrgState::TMove move = state.GetLastMove();
				er::CTrgState::TMoveThreshold thrsld = state.GetMoveThreshold();

				if ((move.x() < -thrsld) || (thrsld < move.x())) {
					main_work->cur_slct_set = ((0 == k)? DME_OPT_SET_MENU_BGM: DME_OPT_SET_MENU_SE);
					is_click_volume[((move.x()<0)? 0: 1)] = true;
					break;
				}
				if ((move.y() < -thrsld) || (thrsld < move.y())) {
					slider.DelLock();
				}
			}
		}
	}
#endif //_IPHONE

	// 決定処理
#if !_IPHONE
	if (AoPadStand() & GSD_KEY_DECIDE) {
#else //!_IPHONE
	if (is_click_default) {
#endif //!_IPHONE
		if (main_work->cur_slct_set == DME_OPT_SET_MENU_DEFAULT) {
			main_work->flag |= DMD_OPT_FLAG_PUSH_DFLT_BTN;
			main_work->efct_timer = 0;
		}
		main_work->flag |= DMD_OPT_FLAG_DECIDE;

		return;
	}

	// 十字キー操作
#if !_IPHONE
	if (AoPadMRepeat() & GSD_KEY_UP) {
		if (AoPadMStand() & GSD_KEY_UP
			|| main_work->cur_slct_set != 0) {
#else //!_IPHONE
	if (false) {{
#endif //!_IPHONE
			
			main_work->flag |= DMD_OPT_FLAG_UP_CHNG_CRSR;
		}

		return;
	}
	
#if !_IPHONE
	else if (AoPadMRepeat() & GSD_KEY_DOWN) {
		if (AoPadMStand() & GSD_KEY_DOWN
			|| main_work->cur_slct_set != DME_OPT_TOP_MENU_NUM - 1) {
#else //!_IPHONE
	else if (false) {{
#endif //!_IPHONE
			
			main_work->flag |= DMD_OPT_FLAG_DOWN_CHNG_CRSR;
		}

		return;
	}
	
	
#if !_IPHONE
	else if (AoPadMRepeat() & GSD_KEY_LEFT
			 && main_work->cur_slct_set != DME_OPT_SET_MENU_DEFAULT) {
#else //!_IPHONE
	else if (is_click_volume[0]) {
#endif //!_IPHONE
		switch (main_work->cur_slct_set) {
		case DME_OPT_SET_MENU_BGM:
#if !_IPHONE
			if (AoPadMStand() & GSD_KEY_LEFT
#else //!_IPHONE
			if (true
#endif //!_IPHONE
				|| main_work->volume_data[0] != 0) {
				main_work->volume_data[0] = dmOptGetRevisedVolume(main_work->volume_data[0], -1);
				main_work->push_efct_timer[1] = 12.f;
				
				// ボリューム変更処理
				DmSoundSetVolumeBGM((Float)main_work->volume_data[0]);
			}
			
			break;
			
		case DME_OPT_SET_MENU_SE:
#if !_IPHONE
			if (AoPadMStand() & GSD_KEY_LEFT
#else //!_IPHONE
			if (true
#endif //!_IPHONE
				|| main_work->volume_data[1] != 0) {
				main_work->volume_data[1] = dmOptGetRevisedVolume(main_work->volume_data[1], -1);
				main_work->push_efct_timer[3] = 12.f;
				
				// 反映後のボリュームでSE再生
				DmSoundSetVolumeSE((Float)main_work->volume_data[1]);
				
				if (!dm_opt_is_pause_maingame) {
					DmSoundPlaySE("Cursol");
				}
				else {
					GsSoundPlaySe("Cursol", main_work->se_handle);
				}
			}
			 
			break;
			
		default:
			break;
		}

		return;
	}
	
#if !_IPHONE
	else if (AoPadMRepeat() & GSD_KEY_RIGHT
			 && main_work->cur_slct_set != DME_OPT_SET_MENU_DEFAULT) {
#else //!_IPHONE
	else if (is_click_volume[1]) {
#endif //!_IPHONE
		switch (main_work->cur_slct_set) {
		case DME_OPT_SET_MENU_BGM:
#if !_IPHONE
			if (AoPadMStand() & GSD_KEY_RIGHT
#else //!_IPHONE
			if (true
#endif //!_IPHONE
				|| main_work->volume_data[0] != DMD_OPT_VOL_DATA_MAX) {
				main_work->volume_data[0] = dmOptGetRevisedVolume(main_work->volume_data[0], 1);
				main_work->push_efct_timer[0] = 12.f;
				
				// ボリューム変更処理
				DmSoundSetVolumeBGM((Float)main_work->volume_data[0]);
			}
				
			break;
			
		case DME_OPT_SET_MENU_SE:
#if !_IPHONE
			if (AoPadMStand() & GSD_KEY_RIGHT
#else //!_IPHONE
			if (true
#endif //!_IPHONE
				|| main_work->volume_data[1] != DMD_OPT_VOL_DATA_MAX) {
				main_work->volume_data[1] = dmOptGetRevisedVolume(main_work->volume_data[1], 1);
				main_work->push_efct_timer[2] = 12.f;
				
				// 反映後のボリュームでSE再生
				DmSoundSetVolumeSE((Float)main_work->volume_data[1]);
				
				if (!dm_opt_is_pause_maingame) {
					DmSoundPlaySE("Cursol");
				}
				else {
					GsSoundPlaySe("Cursol", main_work->se_handle);
				}
			}
				
			break;
			
		default:
			break;
		}
		
		return;
	}
	
}



// ==========================================================================
// dmOptInputProcControlMenu
/*!
	オプション・操作方法設定画面用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmOptInputProcControlMenu(DMS_OPT_MAIN_WORK *main_work)
{
	// キャンセル処理
#if !_IPHONE
	if (AoPadStand() & GSD_KEY_CANCEL) {
#else //!_IPHONE
	if (main_work->trg_return.GetState(0)[er::CTrgState::EState::Up] && main_work->trg_return.GetState(0)[er::CTrgState::EState::Prev]) {
#endif //!_IPHONE
		main_work->flag |= DMD_OPT_FLAG_CANCEL;

		return;
	}
	
	// 決定処理
	if (AoPadStand() & GSD_KEY_DECIDE) {
		main_work->flag |= DMD_OPT_FLAG_DECIDE;

		return;
	}
}



// ==========================================================================
// dmOptProcActDraw
/*!
	描画設定プロシージャ処理
 */
// ==========================================================================
void dmOptProcActDraw(DMS_OPT_MAIN_WORK *main_work)
{
	// 帯移動演出用		※常に移動しつづけるのでここに配置
	dmOptSetObiEfctPos(main_work);
	
	// 共通描画処理は描画時は常に設定
	dmOptCommonDraw(main_work);

	// FIX関連描画設定
	dmOptCommonFixDraw(main_work);

	// セーブファイル描画設定
	if (main_work->proc_menu_draw) {
		main_work->proc_menu_draw(main_work);
	}
	
	// 描画タスク生成
	if (dm_opt_is_pause_maingame) {
		if (main_work->draw_state) {
			amDrawMakeTask(dmOptTaskDraw, (u16)0x8000, (u32)0);
		}
	}
	else {
		amDrawMakeTask(dmOptTaskDraw, (u16)0x8000, (u32)0);
	}

}



// ==========================================================================
// dmOptTaskDraw
/*!
	オプション画面の描画タスク
 */
// ==========================================================================
void dmOptTaskDraw(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	AoActDrawPre();
	amDrawExecCommand(dm_opt_draw_state);
	amDrawEndScene();
}



// ==========================================================================
// dmOptCommonDraw
/*!
	共通描画設定処理
 */
// ==========================================================================
void dmOptCommonDraw(DMS_OPT_MAIN_WORK *main_work)
{
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_OPT_DRAW_PRIO_BG);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[0]));
	AoActSortRegAction(main_work->act[ACT_WAVE_BG]);
	AoActSortRegAction(main_work->act[ACT_BLUE_BG]);
	AoActSortRegAction(main_work->act[ACT_DOWN_BG]);
	

	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[0]));
	AoActUpdate(main_work->act[ACT_WAVE_BG], 1.0f);
	AoActUpdate(main_work->act[ACT_BLUE_BG], 0.0f);
	AoActUpdate(main_work->act[ACT_DOWN_BG], 0.0f);
	
	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();
}



// ==========================================================================
// dmOptCommonFixDraw
/*!
	FIX描画設定処理
 */
// ==========================================================================
void dmOptCommonFixDraw(DMS_OPT_MAIN_WORK *main_work)
{
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_OPT_DRAW_PRIO_FIX);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));

	// アクション登録
#if !_IPHONE
	for (int i = ACT_TAB_TITLE1; i <= ACT_TAB_TITLE2; i++) {
#else //!_IPHONE
	for (int i = ACT_TAB_TITLE1; i <= ACT_TAB_TITLE1; i++) {
#endif //!_IPHONE
		AoActSortRegAction(main_work->act[i]);
	}
	
	// 戻るボタン
#if !_IPHONE
#if _WII || _PC
	if (!DmUserNameIsMsgWinOpen()) {
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[1]));
		AoActSortRegAction(main_work->act[ACT_BTN_CANCEL1]);
	}
#else
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[1]));
	AoActSortRegAction(main_work->act[ACT_BTN_CANCEL1]);
#endif
	
	// アクション登録
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[2]));
	AoActSortRegAction(main_work->act[ACT_OBI_C]);
#endif //!_IPHONE
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));

	// アクション登録
#if !_IPHONE
	for (int i = ACT_TEX_TITLE; i < ACT_TEX_OBI1; i++) {
#else //!_IPHONE
	for (int i = ACT_TEX_TITLE; i <= ACT_TEX_TITLE; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
#endif //!_IPHONE
	
	//
#if _WII
	if (main_work->state == 3) {
		AoActSetTexture(AoTexGetTexList(DmUserNameGetUserLangTex()));
		AoActSortRegAction(main_work->act[ACT_TEX_OBI_USER1]);
		AoActSortRegAction(main_work->act[ACT_TEX_OBI_USER2]);
	}
	else {
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		AoActSortRegAction(main_work->act[ACT_TEX_OBI1]);
		AoActSortRegAction(main_work->act[ACT_TEX_OBI2]);
	}
#elif _IPHONE
	//無し
#else
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	AoActSortRegAction(main_work->act[ACT_TEX_OBI1]);
	AoActSortRegAction(main_work->act[ACT_TEX_OBI2]);
#endif
	
#if !_IPHONE
	// アクション登録
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[2]));
	for (int i = ACT_OBI_L; i <= ACT_OBI_R; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
#endif //!_IPHONE
	
	// 戻るテキスト
#if _WII || _PC
	if (!DmUserNameIsMsgWinOpen()) {
		AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[4]));
		AoActSortRegAction(main_work->act[ACT_TEX_BACK]);
	}
#elif _IPHONE
	for (int i = ACT_BACK_BTN01_L; i <= ACT_BACK_BTN01_R; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	AoActSortRegAction(main_work->act[ACT_TEX_BACK]);
#else
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[4]));
	AoActSortRegAction(main_work->act[ACT_TEX_BACK]);
#endif
	
	AoActSetFrame(main_work->act[ACT_TEX_TITLE], (f32)main_work->state);

#if !_IPHONE
	if (main_work->state == DME_OPT_MENU_STATE_TOP) {
		if (main_work->cur_slct_top == DME_OPT_TOP_MENU_4) {
			AoActSetFrame(main_work->act[ACT_TEX_OBI1], 4.f);
			AoActSetFrame(main_work->act[ACT_TEX_OBI2], 4.f);
		}
		
		else if (main_work->cur_slct_top == DME_OPT_TOP_MENU_3) {
#if !_WII
			AoActSetFrame(main_work->act[ACT_TEX_OBI1], 2.f);
			AoActSetFrame(main_work->act[ACT_TEX_OBI2], 2.f);
#else
			AoActSetFrame(main_work->act[ACT_TEX_OBI1], 3.f);
			AoActSetFrame(main_work->act[ACT_TEX_OBI2], 3.f);
#endif
		}
		
		else if (main_work->cur_slct_top == DME_OPT_TOP_MENU_2) {
#if !_WII
			AoActSetFrame(main_work->act[ACT_TEX_OBI1], 1.f);
			AoActSetFrame(main_work->act[ACT_TEX_OBI2], 1.f);
#else
			AoActSetFrame(main_work->act[ACT_TEX_OBI1], 2.f);
			AoActSetFrame(main_work->act[ACT_TEX_OBI2], 2.f);
#endif
		}
		
		else {
#if !_WII
			AoActSetFrame(main_work->act[ACT_TEX_OBI1], 0.f);
			AoActSetFrame(main_work->act[ACT_TEX_OBI2], 0.f);
#else
			AoActSetFrame(main_work->act[ACT_TEX_OBI1], 1.f);
			AoActSetFrame(main_work->act[ACT_TEX_OBI2], 1.f);
#endif
		}
	}
	else if (main_work->state == DME_OPT_MENU_STATE_CTRL) {
		AoActSetFrame(main_work->act[ACT_TEX_OBI1], 1.f);
		AoActSetFrame(main_work->act[ACT_TEX_OBI2], 1.f);
	}
	else {
		AoActSetFrame(main_work->act[ACT_TEX_OBI1], 5.f);
		AoActSetFrame(main_work->act[ACT_TEX_OBI2], 5.f);
	}
	
	
	if (main_work->state == DME_OPT_MENU_STATE_SET) {
		AoActSetFrame(main_work->act[ACT_TEX_OBI1], 5.f);
		AoActSetFrame(main_work->act[ACT_TEX_OBI2], 5.f);
	}
#endif //!_IPHONE
	
#if _WII
	if (main_work->cur_slct_top == DME_OPT_TOP_MENU_3) {
		AoActSetFrame(main_work->act[ACT_TEX_OBI1], 3.f);
		AoActSetFrame(main_work->act[ACT_TEX_OBI2], 3.f);
	}
#endif
	
	
	if (main_work->is_jp_region) {
		AoActSetFrame(main_work->act[ACT_BTN_CANCEL1], (f32)0.f);
	}
	else {
		AoActSetFrame(main_work->act[ACT_BTN_CANCEL1], (f32)1.f);
	}
	

	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
#if !_IPHONE
	for (int i = ACT_TAB_TITLE1; i <= ACT_TAB_TITLE2; i++) {
#else //!_IPHONE
	for (int i = ACT_TAB_TITLE1; i <= ACT_TAB_TITLE1; i++) {
#endif //!_IPHONE
		AoActUpdate(main_work->act[i], 0.0f);
	}
	
#if !_IPHONE
	// 戻るボタン
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[1]));
	AoActUpdate(main_work->act[ACT_BTN_CANCEL1], 0.0f);
	
	// 帯
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[2]));
	for (int i = ACT_OBI_C; i <= ACT_OBI_R; i++) {
		AoActUpdate(main_work->act[i], 0.0f);
	}
#endif //!_IPHONE
	
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	AoActUpdate(main_work->act[ACT_TEX_TITLE], 0.0f);
	
	// 戻るテキスト
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[4]));
	AoActUpdate(main_work->act[ACT_TEX_BACK], 0.0f);
#if _IPHONE
	AoActSetTexture(AoTexGetTexList(&main_work->cmn_tex[3]));
	for (int i = ACT_BACK_BTN01_L; i <= ACT_BACK_BTN01_R; i++) {
		float update_frame = ((2.0f <= main_work->act[i]->frame)? 1.0f: 0.0f);
		AoActUpdate(main_work->act[i], update_frame);
	}
#endif //_IPHONE
#if _IPHONE	//当たり判定処理
	{
		er::CTrgAoAction &ret = main_work->trg_return;
		ret.Update();
	}
#endif //_IPHONE	//当たり判定処理
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
#if !_IPHONE
	// ACCUMURATEでトランスさせる
	AoActAcmPush();
	
	for (int i = 0; i < 2; i++) {
		// 帯テキスト部分
		AoActAcmInit();
		AoActAcmApplyTrans(main_work->obi_tex_pos[i]
						   , 0
						   , 0
						   );
		
#if _PS3 || _XBOX || _PC
		AoActAcmApplyScale(DMD_OPT_OBI_MSG_SCALE_SIZE
						   , DMD_OPT_OBI_MSG_SCALE_SIZE);
#endif
		
		// フレーム更新はSetFrameのみで行う
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		AoActUpdate(main_work->act[ACT_TEX_OBI1 + i], 0.0f);
#if _WII
		AoActSetTexture(AoTexGetTexList(DmUserNameGetUserLangTex()));
		AoActUpdate(main_work->act[ACT_TEX_OBI_USER1 + i], 0.0f);
#endif
	}
	
	AoActAcmPop();
#endif //!_IPHONE

	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();
}



// ==========================================================================
// dmOptTopMenuDraw
/*!
	オプション・トップメニュー描画設定処理
 */
// ==========================================================================
void dmOptTopMenuDraw(DMS_OPT_MAIN_WORK *main_work)
{
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_OPT_DRAW_PRIO_TOP_MENU);
	
	for (int i = 0; i < DME_OPT_TOP_MENU_NUM; i++) {
		
		// アクション更新
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
		
		AoActSortRegAction(main_work->act[ACT_TAB_SELECT]);
#if _IPHONE
		AoActSortRegAction(main_work->act[ACT_TAB_SELECT_L]);
		AoActSortRegAction(main_work->act[ACT_TAB_SELECT_R]);
#endif //_IPHONE
		
		// アクション更新
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		
		AoActSortRegAction(main_work->act[dm_opt_top_menu_tex_tbl[i]]);
		
		// FOCUS項目設定
#if !_IPHONE
		if (main_work->cur_slct_top == i) {
			// 各ID別の設定
			AoActSetFrame(main_work->act[ACT_TAB_SELECT], 0.f);
			AoActSetFrame(main_work->act[dm_opt_top_menu_tex_tbl[i]], 0.f);
		}
		// 非選択項目設定
		else {
			AoActSetFrame(main_work->act[ACT_TAB_SELECT], 1.f);
			AoActSetFrame(main_work->act[dm_opt_top_menu_tex_tbl[i]], 1.f);
		}
#else //!_IPHONE
		if (DMD_OPT_FLAG_DECIDE_EFCT & main_work->flag) {
			if (i == main_work->cur_slct_top) {
				//選択項目なら
				//決定演出
				float base_frame = 2.0f + main_work->timer;
				AoActSetFrame(main_work->act[ACT_TAB_SELECT], base_frame);
				AoActSetFrame(main_work->act[ACT_TAB_SELECT_L], base_frame);
				AoActSetFrame(main_work->act[ACT_TAB_SELECT_R], base_frame);
				AoActSetFrame(main_work->act[dm_opt_top_menu_tex_tbl[i]], 1.f);
			} else {
				//選択項目以外なら
				AoActSetFrame(main_work->act[ACT_TAB_SELECT], 0.f);
				AoActSetFrame(main_work->act[ACT_TAB_SELECT_L], 0.f);
				AoActSetFrame(main_work->act[ACT_TAB_SELECT_R], 0.f);
				AoActSetFrame(main_work->act[dm_opt_top_menu_tex_tbl[i]], 0.f);
			}
		} else if (IzFadeIsExe() && !IzFadeIsEnd()) {
			//フェードアウト中なら
			AoActSetFrame(main_work->act[ACT_TAB_SELECT], 0.f);
			AoActSetFrame(main_work->act[ACT_TAB_SELECT_L], 0.f);
			AoActSetFrame(main_work->act[ACT_TAB_SELECT_R], 0.f);
			AoActSetFrame(main_work->act[dm_opt_top_menu_tex_tbl[i]], 0.f);
		} else if (main_work->trg_slct[i].GetState(0)[er::CTrgState::EState::On]) {
			//選択中
			AoActSetFrame(main_work->act[ACT_TAB_SELECT], 1.f);
			AoActSetFrame(main_work->act[ACT_TAB_SELECT_L], 1.f);
			AoActSetFrame(main_work->act[ACT_TAB_SELECT_R], 1.f);
			AoActSetFrame(main_work->act[dm_opt_top_menu_tex_tbl[i]], 1.f);
		} else {
			//非選択
			AoActSetFrame(main_work->act[ACT_TAB_SELECT], 0.f);
			AoActSetFrame(main_work->act[ACT_TAB_SELECT_L], 0.f);
			AoActSetFrame(main_work->act[ACT_TAB_SELECT_R], 0.f);
			AoActSetFrame(main_work->act[dm_opt_top_menu_tex_tbl[i]], 0.f);
		}
#endif //_IPHONE
		
		// ACCUMURATEでトランスさせる
		AoActAcmPush();
		
		AoActAcmInit();
#if !_IPHONE
		AoActAcmApplyTrans(DMD_OPT_SIZE_HALF_WIDTH
						   , DMD_OPT_TOP_MENU_TOP_POS_Y
						   		+ i * DMD_OPT_TOP_MENU_DIST_Y
						   , 0
						   );
#else //!_IPHONE
		AoActAcmApplyTrans(DMD_OPT_TOP_MENU_TOP_POS_X + (i % DMD_OPT_TOP_MENU_ROW_MAX) * DMD_OPT_TOP_MENU_DIST_X
						   , DMD_OPT_TOP_MENU_TOP_POS_Y + (i / DMD_OPT_TOP_MENU_ROW_MAX) * DMD_OPT_TOP_MENU_DIST_Y
						   , 0
						   );
#endif //!_IPHONE
		
		// 選択項目が決定したときのみ
		if (main_work->cur_slct_top == i
			&& main_work->flag & DMD_OPT_FLAG_DECIDE_EFCT) {
			AoActAcmApplyFade(main_work->decide_menu_col);
		}
		
		// フレーム更新はSetFrameのみで行う
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
		AoActUpdate(main_work->act[ACT_TAB_SELECT], 0.0f);
#if _IPHONE
		AoActUpdate(main_work->act[ACT_TAB_SELECT_L], 0.0f);
		AoActUpdate(main_work->act[ACT_TAB_SELECT_R], 0.0f);
#endif //_IPHONE
#if _IPHONE	//当たり判定処理
		{
			er::CTrgAoAction &slct = main_work->trg_slct[i];
			slct.Update();
		}
#endif //_IPHONE	//当たり判定処理
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		AoActUpdate(main_work->act[dm_opt_top_menu_tex_tbl[i]], 0.0f);
		
		AoActAcmPop();
		
		
		// ソート実行
		AoActSortExecute();
		
		// ソートバッファ描画
		AoActSortDraw();
		
		// ソートバッファ全解除
		AoActSortUnregAll();
	}
	
#if !_IPHONE
	// テクスチャセット
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	for (int j = ACT_CRSR_TOP_SLCT1; j <= ACT_CRSR_TOP_SLCT3; j++) {
//		AoActSortRegAction(main_work->act[j]);
	}
	
	
	AoActAcmPush();
	
	for (int j = ACT_CRSR_TOP_SLCT1; j <= ACT_CRSR_TOP_SLCT3; j++) {
		
		AoActAcmInit();
		AoActAcmApplyTrans(DMD_OPT_SIZE_HALF_WIDTH
						   , main_work->top_crsr_pos_y
						   , 0
						   );
		
		AoActUpdate(main_work->act[j], 0.f);
	}
	
	AoActAcmPop();
#endif //!_IPHONE
	
	// ソート実行
	AoActSortExecute();
	
	// ソートバッファ描画
	AoActSortDraw();
	
	// ソートバッファ全解除
	AoActSortUnregAll();

/*
	// カーソル描画設定
	if (main_work->disp_flag & DMD_OPT_DISP_FLAG_ACT_CRSR) {
		for (u32 i = ACT_FILE_TAB4_B; i <= ACT_FILE_TAB4_C; i++) {
			AoActSortRegAction(main_work->act[i]);
			
			// ACCUMURATEでトランスさせる
			AoActAcmPush();
			
			AoActAcmInit();
			AoActAcmApplyTrans(DMD_OPT_FILE_TABLE_TOP_POS_X
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
*/

}



// ==========================================================================
// dmOptSettingMenuDraw
/*!
	オプション・設定画面描画設定処理
 */
// ==========================================================================
void dmOptSettingMenuDraw(DMS_OPT_MAIN_WORK *main_work)
{
	AOS_ACT_COL tmp_col;
	f32 move_dist = 0.f;
	f32 disp_dist = 0.f;
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_OPT_DRAW_PRIO_SET_MENU);
	
	// ウインドウ描画(設定ウインドウ)
	AoWinSysDrawState(AOD_WIN_TYPE_A
					 , AoTexGetTexList(&main_work->tex[0])
//					 , AoTexGetTexList(&dm_opt_win_tex)
					 , 3
//					 , 0
					 , DMD_OPT_SIZE_HALF_WIDTH			// ウインドウ中心X
					 , DMD_OPT_SIZE_HALF_HEIGHT			// ウインドウ中心Y
					 , DMD_OPT_WINDOW_SIZE_W * main_work->win_size_rate[0]			// ウインドウ横サイズ
					 , DMD_OPT_WINDOW_SIZE_H * main_work->win_size_rate[1]			// ウインドウ縦サイズ
					 , dm_opt_draw_state				// 描画STATE
					 );
	
	if (main_work->disp_flag & DMD_OPT_DISP_FLAG_WIN_ACT) {
		
		// デフォルト以外の項目描画設定
		for (int i = 0; i < DME_OPT_SET_MENU_NUM - 1; i++) {
			
			// アクション更新
			AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
			
#if !_IPHONE
			if (main_work->cur_slct_set == i) {
				AoActSortRegAction(main_work->act[ACT_CUR_SLCT_TAB1]);
				AoActSortRegAction(main_work->act[ACT_CUR_SLCT_TAB2]);
			}
#endif //!_IPHONE
			
			
			// BGMとSEの場合
			if (i < 2) {
				for (u32 k = ACT_ICON_VOL_UP; k <= ACT_GAUGE_VOL_10; k++) {
					AoActSortRegAction(main_work->act[k]);
				}
			
				// ここはVOLの値ごとに登録数を変更		※仮で全て表示
				if (main_work->volume_data[i] == 10) {
					AoActSortRegAction(main_work->act[ACT_NUM_VOL_1]);
				}
				if (main_work->volume_data[i] > 0) {
					AoActSortRegAction(main_work->act[ACT_NUM_VOL_2]);
				}
				AoActSortRegAction(main_work->act[ACT_NUM_VOL_3]);
				AoActSortRegAction(main_work->act[ACT_NUM_VOL_4]);
			}
			
			else {
//				AoActSortRegAction(main_work->act[ACT_ARROW_SETTING1]);
//				AoActSortRegAction(main_work->act[ACT_ARROW_SETTING2]);
			}
			
			// アクション更新
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
			
			// 項目別にテキストの表示を切り替える
			AoActSortRegAction(main_work->act[dm_opt_set_menu_tex_tbl[i]]);
			
			if (i > DME_OPT_SET_MENU_SE) {
				// ON/OFFテキスト
				AoActSortRegAction(main_work->act[ACT_TEX_ON]);
				AoActSortRegAction(main_work->act[ACT_TEX_OFF]);
			}
			
			// FOCUS項目設定
			if (main_work->cur_slct_set == i) {
				// 各ID別の設定
//				for (u32 k = ACT_TAB_CRSR_BAR1; k <= ACT_TAB_CRSR_BAR3; k++) {
//					AoActSetFrame(main_work->act[k], 0.f);
//				}
				AoActSetFrame(main_work->act[dm_opt_set_menu_tex_tbl[i]], 0.f);

				tmp_col.r = tmp_col.g = tmp_col.b = tmp_col.a = 255;
			}
			// 非選択項目設定
			else {
//				for (u32 k = ACT_TAB_CRSR_BAR1; k <= ACT_TAB_CRSR_BAR3; k++) {
//					AoActSetFrame(main_work->act[k], 1.f);
//				}
				tmp_col.r = tmp_col.g = tmp_col.b = 255;
				tmp_col.a = 60;
			}
			
			// ボリューム％設定
#if !_IPHONE
			if (main_work->volume_data[i] == 10) {
				AoActSetFrame(main_work->act[ACT_NUM_VOL_1], 0.f);
				AoActSetFrame(main_work->act[ACT_NUM_VOL_2], 9.f);
			}
			else if (main_work->volume_data[i] < 10) {
				AoActSetFrame(main_work->act[ACT_NUM_VOL_1], 0.f);
				AoActSetFrame(main_work->act[ACT_NUM_VOL_2], (f32)(main_work->volume_data[i] - 1));
			}
			AoActSetFrame(main_work->act[ACT_NUM_VOL_3], 9.f);
#else //!_IPHONE
			AoActSetFrame(main_work->act[ACT_NUM_VOL_1], 1.f);
			AoActSetFrame(main_work->act[ACT_NUM_VOL_2], main_work->volume_data[i] % 10);
			AoActSetFrame(main_work->act[ACT_NUM_VOL_3], 0.f);
#endif //!_IPHONE
			
			// ボリューム色設定
			if (i <= DME_OPT_SET_MENU_SE) {
				for (s32 k = 0; k < 10; k++) {
					if (k < main_work->volume_data[i]) {
						AoActSetFrame(main_work->act[ACT_GAUGE_VOL_1 + k], 0.f);
					}
					else {
						AoActSetFrame(main_work->act[ACT_GAUGE_VOL_1 + k], 1.f);
					}
				}
			}
			
			// 振動のON/OFF切り替え設定
			if (main_work->set_vbrt == DME_OPT_CTRL_VBRT_ON) {
				AoActSetFrame(main_work->act[ACT_TEX_ON], 0.f);
				AoActSetFrame(main_work->act[ACT_TEX_OFF], 1.f);
			}
			else {
				AoActSetFrame(main_work->act[ACT_TEX_ON], 1.f);
				AoActSetFrame(main_work->act[ACT_TEX_OFF], 0.f);
			}
			
			AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
			
#if !_IPHONE
			for (u32 k = ACT_NUM_VOL_1; k <= ACT_TAB_DEFAULT3; k++) {
#else //!_IPHONE
			for (u32 k = ACT_NUM_VOL_1; k <= ACT_GAUGE_VOL_10; k++) {
#endif //!_IPHONE
				// ACCUMURATEでトランスさせる
				AoActAcmPush();
				
				AoActAcmInit();
				
				AoActAcmApplyTrans(DMD_OPT_SIZE_HALF_WIDTH + move_dist
								   , dm_opt_set_tab_pos_y_tbl[i]
								   , 0
								   );
				
				
				if (main_work->push_efct_timer[0 + 2 * i] > 0
					&& k == ACT_ICON_VOL_UP) {
					AoActAcmApplyColor(main_work->vol_icon_col);
				}

				if (main_work->push_efct_timer[1 + 2 * i] > 0
					&& k == ACT_ICON_VOL_DOWN) {
					AoActAcmApplyColor(main_work->vol_icon_col);
				}
				
				// フレーム更新はSetFrameのみで行う
				AoActUpdate(main_work->act[k], 0.0f);
				
#if _IPHONE	//当たり判定処理
				{
					int trg_index;
					switch (k) {
					case ACT_ICON_VOL_DOWN:
						trg_index = 0;
						break;
					case ACT_ICON_VOL_UP:
						trg_index = 1;
						break;
					default:
						trg_index = -1;
						break;
					}
					if (0 <= trg_index) {
						er::CTrgAoAction *trg;
						switch (i) {
						case DME_OPT_SET_MENU_BGM:
							trg = main_work->trg_bgm_btn;
							break;
						case DME_OPT_SET_MENU_SE:
							trg = main_work->trg_se_btn;
							break;
						default:
							trg = NULL;
							break;
						}
						if (trg) {
							trg[trg_index].Update();
						} else {
							amAssert(trg);
						}
					}
				}
#endif //_IPHONE	//当たり判定処理

				AoActAcmPop();
			}
			
			
			AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));

			for (u32 k = ACT_TEX_BGM; k <= ACT_TEX_OFF; k++) {
				AoActAcmPush();
				
				AoActAcmInit();
				AoActAcmApplyTrans(DMD_OPT_SIZE_HALF_WIDTH
								   , dm_opt_set_tab_pos_y_tbl[i]
								   , 0
								   );
				
				AoActUpdate(main_work->act[k], 0.0f);
				
				AoActAcmPop();
			}

			// ソート実行
			AoActSortExecute();
			
			// ソートバッファ描画
			AoActSortDraw();
			
			// ソートバッファ全解除
			AoActSortUnregAll();
		}

#if _IPHONE	//当たり判定処理
	{
		{
			er::CTrgRect &slider = main_work->trg_bgm_slider;
			slider.Update();
		}
		{
			er::CTrgRect &slider = main_work->trg_se_slider;
			slider.Update();
		}
	}
#endif //_IPHONE	//当たり判定処理
#if _IPHONE
		//操作方法
		{
			using namespace gs::backup;
			float frame_tilt, frame_pad;
			if (SOption::EControl::Tilt == SOption::CreateInstance().GetControl()) {
				//傾斜
				frame_tilt = 1.0f;
				frame_pad = 0.0f;
			} else {
				//バーチャルパッド
				frame_tilt = 0.0f;
				frame_pad = 1.0f;
			}
			//傾斜関係アクション
			for (AOS_ACTION **act = &main_work->act[ACT_CNT01_L], **act_end = &main_work->act[ACT_CNT01_R+1]; act != act_end; ++act) {
				AoActSetFrame(*act, frame_tilt);
			}
			for (AOS_ACTION **act = &main_work->act[ACT_TEX_TILT], **act_end = &main_work->act[ACT_TEX_TILT+1]; act != act_end; ++act) {
				AoActSetFrame(*act, frame_tilt);
			}
			//バーチャルパッド関係アクション
			for (AOS_ACTION **act = &main_work->act[ACT_CNT02_L], **act_end = &main_work->act[ACT_CNT02_R+1]; act != act_end; ++act) {
				AoActSetFrame(*act, frame_pad);
			}
			for (AOS_ACTION **act = &main_work->act[ACT_TEX_TOUCH], **act_end = &main_work->act[ACT_TEX_TOUCH+1]; act != act_end; ++act) {
				AoActSetFrame(*act, frame_pad);
			}

			AoActAcmPush();
			AoActAcmInit();
			AoActAcmApplyTrans(DMD_OPT_SIZE_HALF_WIDTH + move_dist
								, dm_opt_set_tab_pos_y_tbl[DME_OPT_SET_MENU_DEFAULT]
								, 0
								);
			//言語共通
			AoActSetTexture(AoTexGetTexList(&main_work->tex[DME_OPT_DATA_TYPE_CMN_DATA]));
			for (AOS_ACTION **act = &main_work->act[ACT_CNT01_L], **act_end = &main_work->act[ACT_CNT02_R+1]; act != act_end; ++act) {
				AoActUpdate(*act, 0.0f);
			}
			//各国対応
			AoActSetTexture(AoTexGetTexList(&main_work->tex[DME_OPT_DATA_TYPE_LANG_DATA]));
			AoActUpdate(main_work->act[ACT_TEX_CTRL], 0.0f);
			for (AOS_ACTION **act = &main_work->act[ACT_TEX_TILT], **act_end = &main_work->act[ACT_TEX_TOUCH+1]; act != act_end; ++act) {
				AoActUpdate(*act, 0.0f);
			}
			//トリガ更新
			for (er::CTrgAoAction *btn = main_work->trg_ctrl_btn, *btn_end = main_work->trg_ctrl_btn + arrayof(main_work->trg_ctrl_btn); btn != btn_end; ++btn) {
				btn->Update();
			}
			AoActAcmPop();

			//描画
			AoActSortRegAction(main_work->act[ACT_TEX_CTRL]);
			for (AOS_ACTION **act = &main_work->act[ACT_CNT01_L], **act_end = &main_work->act[ACT_CNT02_R+1]; act != act_end; ++act) {
				AoActSortRegAction(*act);
			}
			for (AOS_ACTION **act = &main_work->act[ACT_TEX_TILT], **act_end = &main_work->act[ACT_TEX_TOUCH+1]; act != act_end; ++act) {
				AoActSortRegAction(*act);
			}
			
			//ソート描画
			AoActSortExecute();
			AoActSortDraw();
			AoActSortUnregAll();
		}
#endif //_IPHONE
#if _IPHONE //操作方法ウインドウ
		if (0.0f < main_work->ctrl_win_window_prgrs) {
			AoWinSysDrawState(AOD_WIN_TYPE_A
							, AoTexGetTexList(&main_work->cmn_tex[3])
							, 0
							, DMD_OPT_SIZE_HALF_WIDTH									// ウインドウ中心X
							, DMD_OPT_SIZE_HALF_HEIGHT									// ウインドウ中心Y
							, 1280 * main_work->ctrl_win_window_prgrs					// ウインドウ横サイズ
							, DMD_OPT_SIZE_HEIGHT * main_work->ctrl_win_window_prgrs	// ウインドウ縦サイズ
							, dm_opt_draw_state											// 描画STATE
					 );
			if (1.0f == main_work->ctrl_win_window_prgrs) {
				AoActSetTexture(AoTexGetTexList(&main_work->tex[DME_OPT_DATA_TYPE_CMN_DATA]));
				for (AOS_ACTION **act = &main_work->act[ACT_TYPE_SCREEN], **act_end = &main_work->act[ACT_B_TYPE_BTN_R+1]; act != act_end; ++act) {
					AoActUpdate(*act);
				}
				AoActSetTexture(AoTexGetTexList(&main_work->tex[DME_OPT_DATA_TYPE_LANG_DATA]));
				for (AOS_ACTION **act = &main_work->act[ACT_TEX_MSG01], **act_end = &main_work->act[ACT_TEX_B_FLICK+1]; act != act_end; ++act) {
					AoActUpdate(*act);
				}
				//トリガ更新
				for (er::CTrgAoAction *btn = main_work->ctrl_win_trg_btn, *btn_end = main_work->ctrl_win_trg_btn + arrayof(main_work->ctrl_win_trg_btn); btn != btn_end; ++btn) {
					btn->Update();
				}
				//特殊フレームの設定
				using namespace gs::backup;
				SOption::EControl::Type ctrl = SOption::CreateInstance().GetControl();
				if (SOption::EControl::VirtualPadUp == ctrl) {
					//バーチャルパッド(上)
					AoActSetTexture(AoTexGetTexList(&main_work->tex[DME_OPT_DATA_TYPE_CMN_DATA]));
					for (AOS_ACTION **act = &main_work->act[ACT_A_TYPE_BTN_L], **act_end = &main_work->act[ACT_A_TYPE_BTN_R+1]; act != act_end; ++act) {
						AoActSetFrame(*act, 1.0f);
						AoActUpdate(*act, 0.0f);
					}
					for (AOS_ACTION **act = &main_work->act[ACT_B_TYPE_BTN_L], **act_end = &main_work->act[ACT_B_TYPE_BTN_R+1]; act != act_end; ++act) {
						AoActSetFrame(*act, 0.0f);
						AoActUpdate(*act, 0.0f);
					}
					AoActSetTexture(AoTexGetTexList(&main_work->tex[DME_OPT_DATA_TYPE_LANG_DATA]));
					AoActSetFrame(main_work->act[ACT_TEX_TYPE_A], 1.0f);
					AoActSetFrame(main_work->act[ACT_TEX_TYPE_B], 0.0f);
					for (AOS_ACTION **act = &main_work->act[ACT_TEX_TYPE_A], **act_end = &main_work->act[ACT_TEX_TYPE_B+1]; act != act_end; ++act) {
						AoActUpdate(*act, 0.0f);
					}
				} else {
					//バーチャルパッド(下)
					AoActSetTexture(AoTexGetTexList(&main_work->tex[DME_OPT_DATA_TYPE_CMN_DATA]));
					for (AOS_ACTION **act = &main_work->act[ACT_A_TYPE_BTN_L], **act_end = &main_work->act[ACT_A_TYPE_BTN_R+1]; act != act_end; ++act) {
						AoActSetFrame(*act, 0.0f);
						AoActUpdate(*act, 0.0f);
					}
					for (AOS_ACTION **act = &main_work->act[ACT_B_TYPE_BTN_L], **act_end = &main_work->act[ACT_B_TYPE_BTN_R+1]; act != act_end; ++act) {
						AoActSetFrame(*act, 1.0f);
						AoActUpdate(*act, 0.0f);
					}
					AoActSetTexture(AoTexGetTexList(&main_work->tex[DME_OPT_DATA_TYPE_LANG_DATA]));
					AoActSetFrame(main_work->act[ACT_TEX_TYPE_A], 0.0f);
					AoActSetFrame(main_work->act[ACT_TEX_TYPE_B], 1.0f);
					for (AOS_ACTION **act = &main_work->act[ACT_TEX_TYPE_A], **act_end = &main_work->act[ACT_TEX_TYPE_B+1]; act != act_end; ++act) {
						AoActUpdate(*act, 0.0f);
					}
				}
				

				//描画
				for (AOS_ACTION **act = &main_work->act[ACT_TYPE_SCREEN], **act_end = &main_work->act[ACT_TYPE_SCREEN+1]; act != act_end; ++act) {
					AoActSortRegAction(*act);
				}
				for (AOS_ACTION **act = &main_work->act[ACT_A_TYPE_BTN_L], **act_end = &main_work->act[ACT_B_TYPE_BTN_R+1]; act != act_end; ++act) {
					AoActSortRegAction(*act);
				}
				for (AOS_ACTION **act = &main_work->act[ACT_TEX_MSG01], **act_end = &main_work->act[ACT_TEX_TYPE_B+1]; act != act_end; ++act) {
					AoActSortRegAction(*act);
				}
				if (SOption::EControl::VirtualPadUp == ctrl) {
					//バーチャルパッド(上)	
					AoActSortRegAction(main_work->act[ACT_TYPE_SCREEN_B]);
					for (AOS_ACTION **act = &main_work->act[ACT_TYPE_YUBI_B1], **act_end = &main_work->act[ACT_TYPE_LINE_B2+1]; act != act_end; ++act) {
						AoActSortRegAction(*act);
					}
					AoActSortRegAction(main_work->act[ACT_TEX_B_FLICK]);
				} else {
					//バーチャルパッド(下)
					for (AOS_ACTION **act = &main_work->act[ACT_TYPE_SCREEN_A], **act_end = &main_work->act[ACT_TYPE_SCREEN_A2+1]; act != act_end; ++act) {
						AoActSortRegAction(*act);
					}
					AoActSortRegAction(main_work->act[ACT_TEX_A_TILT]);
				}
			}
			
			//戻るアイコン
			AoActSortRegAction(main_work->act[ACT_TEX_BACK]);
			for (AOS_ACTION **act = &main_work->act[ACT_BACK_BTN01_L], **act_end = &main_work->act[ACT_BACK_BTN01_R+1]; act != act_end; ++act) {
				AoActSortRegAction(*act);
			}
			//ソート描画
			AoActSortExecute();
			AoActSortDraw();
			AoActSortUnregAll();
		}
#endif //_IPHONE //操作方法ウインドウ
	}


#if !_IPHONE
	if (main_work->disp_flag & DMD_OPT_DISP_FLAG_WIN_ACT) {
		
		// デフォルト項目の描画設定
		AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
		
		for (u32 j = ACT_LINE_SEPARATE1; j < ACT_LINE_SEPARATE4; j++) {
			AoActSortRegAction(main_work->act[j]);
		}
		
//		for (u32 j = ACT_TAB_SET_TITLE_L; j <= ACT_TAB_SET_TITLE_R2; j++) {
//			AoActSortRegAction(main_work->act[j]);
//		}
		
		for (u32 j = ACT_TAB_DEFAULT1; j <= ACT_TAB_DEFAULT3; j++) {
			AoActSortRegAction(main_work->act[j]);
		}
		
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		
		AoActSortRegAction(main_work->act[ACT_TEX_DEFAULT]);
		
		
		// FOCUS項目設定
		if (main_work->cur_slct_set == DME_OPT_SET_MENU_DEFAULT) {
			// 各ID別の設定
			if (main_work->flag & DMD_OPT_FLAG_PUSH_DFLT_BTN) {
				for (u32 k = ACT_TAB_DEFAULT1; k <= ACT_TAB_DEFAULT3; k++) {
					AoActSetFrame(main_work->act[k], 2.f);
				}
				disp_dist = DMD_OPT_PUSH_DFLT_BTN_EFCT_DIST;
			}
			else {
				for (u32 k = ACT_TAB_DEFAULT1; k <= ACT_TAB_DEFAULT3; k++) {
					AoActSetFrame(main_work->act[k], 0.f);
				}
				disp_dist = 0.f;
			}
			AoActSetFrame(main_work->act[ACT_TEX_DEFAULT], 0.f);
		}
		// 非選択項目設定
		else {
			for (u32 k = ACT_TAB_DEFAULT1; k <= ACT_TAB_DEFAULT3; k++) {
				AoActSetFrame(main_work->act[k], 1.f);
			}
			AoActSetFrame(main_work->act[ACT_TEX_DEFAULT], 1.f);
		}
		
		
		AoActAcmPush();
		
		for (u32 j = ACT_LINE_SEPARATE1; j < ACT_LINE_SEPARATE4; j++) {
			AoActAcmInit();
			AoActAcmApplyTrans(DMD_OPT_SIZE_HALF_WIDTH
							   , dm_opt_set_line_pos_y_tbl[j - ACT_LINE_SEPARATE1]
							   , 0
							   );
			
			AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
			AoActUpdate(main_work->act[j], 0.0f);
		}
		
		AoActAcmPop();
		
		
		
		AoActAcmPush();
		
		AoActAcmInit();
		AoActAcmApplyTrans(DMD_OPT_SIZE_HALF_WIDTH
						   , 160.f
						   , 0
						   );
		
//		for (u32 j = ACT_TAB_SET_TITLE_L; j <= ACT_TAB_SET_TITLE_R2; j++) {
//			AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
//			AoActUpdate(main_work->act[j], 0.0f);
//		}
		
		AoActAcmPop();
		
		
		AoActAcmPush();
		
		AoActAcmInit();
		AoActAcmApplyTrans(DMD_OPT_SIZE_HALF_WIDTH
						   , dm_opt_set_tab_pos_y_tbl[2] + disp_dist
						   , 0
						   );
		
		for (u32 j = ACT_TAB_DEFAULT1; j <= ACT_TAB_DEFAULT3; j++) {
			AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
			AoActUpdate(main_work->act[j], 0.0f);
		}
		
		AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
		AoActUpdate(main_work->act[ACT_TEX_DEFAULT], 0.0f);
		
		AoActAcmPop();
	}
	
	// ソート実行
	AoActSortExecute();
	
	// ソートバッファ描画
	AoActSortDraw();
	
	// ソートバッファ全解除
	AoActSortUnregAll();
#endif //!_IPHONE
}



// ==========================================================================
// dmOptControlMenuDraw
/*!
	オプション・操作方法画面描画設定処理
 */
// ==========================================================================
void dmOptControlMenuDraw(DMS_OPT_MAIN_WORK *main_work)
{
	// AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_OPT_DRAW_PRIO_CTRL_MENU);
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	for (int i = ACT_TAB_SOUSA_01L; i <= ACT_S_SCREEN_01B; i++) {
#if !_IPHONE
		if (i >= ACT_SOUSA_CONT_B && i <= ACT_SOUSA_BUT_A_B) {
			if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD) {
				AoActSortRegAction(main_work->act[i]);
			}
		}
		else {
			AoActSortRegAction(main_work->act[i]);
		}
	}
#else //!_IPHONE
		if (i >= ACT_SOUSA_LINE_SONIC1 && i <= ACT_SOUSA_FUKIDASHI) {
			if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD) {
				if (main_work->act[i]) {
					AoActSortRegAction(main_work->act[i]);
				}
			}
		}
		else {
			if (main_work->act[i]) {
				AoActSortRegAction(main_work->act[i]);
			}
		}
	}
	
	//傾斜操作以外の時はipod画像の傾斜演出を止める
	{
		using namespace gs::backup;
		if (SOption::EControl::Tilt != SOption::CreateInstance().GetControl()) {
			//ipod画像
			if (main_work->act[ACT_SOUSA_CONT_A]) {
				AoActSetFrame(main_work->act[ACT_SOUSA_CONT_A], 0.0f);
			}
			//SSアイコン
			if (main_work->act[ACT_SOUSA_SSICON]) {
				AoActSetFrame(main_work->act[ACT_SOUSA_SSICON], 0.0f);
			}
		}
	}
#endif //!_IPHONE
	
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
	for (int i = ACT_TEX_EX_A; i <= ACT_TEX_MOVE; i++) {
		if (i == ACT_TEX_SUPER) {
			if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD) {
#if _IPHONE
				if (main_work->act[i]) //↓に繋げる
#endif //_IPHONE
				AoActSortRegAction(main_work->act[i]);
			}
		}
		else {
#if _IPHONE
			if (main_work->act[i]) //↓に繋げる
#endif //_IPHONE
			AoActSortRegAction(main_work->act[i]);
		}
	}
	
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[0]));
	
	for (int i = ACT_TAB_SOUSA_01L; i <= ACT_S_SCREEN_01B; i++) {
#if !_IPHONE
		AoActUpdate(main_work->act[i], 0.f);
#else //!_IPHONE
		if (main_work->act[i]) {
			AoActUpdate(main_work->act[i]);
		}
#endif //!_IPHONE
	}
	
	
	AoActSetTexture(AoTexGetTexList(&main_work->tex[1]));
	
	for (int i = ACT_TEX_EX_A; i <= ACT_TEX_MOVE; i++) {
#if !_IPHONE
		AoActUpdate(main_work->act[i], 0.f);
#else //!_IPHONE
		if (main_work->act[i]) {
			AoActUpdate(main_work->act[i]);
		}
#endif //!_IPHONE
	}
	
	
	// ソート実行
#if !_IPHONE
	AoActSortExecute();
#else //!_IPHONE
	AoActSortExecuteFix();
#endif //!_IPHONE
	
	// ソートバッファ描画
	AoActSortDraw();
	
	// ソートバッファ全解除
	AoActSortUnregAll();
	
}



// ==========================================================================
// dmOptSetObiEfctPos
/*!
	帯アクションの演出用座標設定処理
 */
// ==========================================================================
void dmOptSetObiEfctPos(DMS_OPT_MAIN_WORK *main_work)
{
	for (u32 i = 0; i < 2; i++) {
		
		// 一定位置に来たら戻る
		if (main_work->obi_tex_pos[i] < DMD_OPT_OBI_MOVE_END_POS) {
			main_work->obi_tex_pos[i] = DMD_OPT_OBI_MOVE_START_POS;
		}
		
		// 移動分座標加算
		main_work->obi_tex_pos[i] += DMD_OPT_OBI_MOVE_SPEED;
	}
}



// ==========================================================================
// dmOptSetNextProcFunc
/*!
	トップメニューにて選択した項目のプロシージャ設定処理
 */
// ==========================================================================
void dmOptSetNextProcFunc(DMS_OPT_MAIN_WORK *main_work)
{
	
	switch (main_work->cur_slct_top) {
#if _PS3 || _XBOX || _IPHONE
	case DME_OPT_TOP_MENU_1:
		// クレジットは完全に別モジュールに切り替わるため、フェードを挟む
		main_work->proc_update = dmOptProcManualStartFadeOut;
		main_work->proc_input = NULL;
//		main_work->proc_menu_draw = dmOptTopMenuDraw;
		
		main_work->state = DME_OPT_MENU_STATE_TOP;
		
		// フェード処理開始
		if (dm_opt_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_BLACK_FADEOUT
								, DMD_OPT_FADEOUT_TIME
								, TRUE
								);
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_OPT_FADEOUT_TIME
						   );
		}
		
		break;
		
	case DME_OPT_TOP_MENU_2:
		main_work->proc_update = dmOptProcCtrlMenuIdle;
		main_work->proc_input = dmOptInputProcControlMenu;
		main_work->proc_menu_draw = dmOptControlMenuDraw;
#if _IPHONE
		dmOptControlResetAct(main_work);
#endif //_IPHONE
		
		main_work->state = DME_OPT_MENU_STATE_CTRL;
		
		break;
		
	case DME_OPT_TOP_MENU_3:
		main_work->proc_update = dmOptProcSetMenuInEfct;
		main_work->proc_input = NULL;//dmOptInputProcSettingMenu;
		main_work->proc_menu_draw = dmOptSettingMenuDraw;
		
		main_work->state = DME_OPT_MENU_STATE_SET;
		main_work->cur_slct_set = DME_OPT_SET_MENU_BGM;
#if _IPHONE
		// 決定SE再生
		((!dm_opt_is_pause_maingame)? DmSoundPlaySE("Window"): GsSoundPlaySe("Window", main_work->se_handle));
#endif //_IPHONE
		
		dmOptSetSaveOptionData(main_work);
		
		break;
		
#else
	case DME_OPT_TOP_MENU_1:
		main_work->proc_update = dmOptProcCtrlMenuIdle;
		main_work->proc_input = dmOptInputProcControlMenu;
		main_work->proc_menu_draw = dmOptControlMenuDraw;
		
		main_work->state = DME_OPT_MENU_STATE_CTRL;
		
		break;
		
	case DME_OPT_TOP_MENU_2:
		main_work->proc_update = dmOptProcSetMenuInEfct;
		main_work->proc_input = NULL;//dmOptInputProcSettingMenu;
		main_work->proc_menu_draw = dmOptSettingMenuDraw;
		
		main_work->state = DME_OPT_MENU_STATE_SET;
		main_work->cur_slct_set = DME_OPT_SET_MENU_BGM;	
		
		dmOptSetSaveOptionData(main_work);
		
		break;
		
	case DME_OPT_TOP_MENU_3:
		DmUserNameStart();
		
		main_work->proc_update = dmOptProcUserMenuIdle;
		main_work->proc_input = NULL;
		main_work->proc_menu_draw = NULL;
		
		main_work->state = 3;
		main_work->cur_slct_set = DME_OPT_SET_MENU_BGM;	
		
		break;
#endif
		
		
	case DME_OPT_TOP_MENU_4:
		// クレジットは完全に別モジュールに切り替わるため、フェードを挟む
		main_work->proc_update = dmOptProcStfrlStartFadeOut;
		main_work->proc_input = NULL;
//		main_work->proc_menu_draw = dmOptTopMenuDraw;
		
		main_work->state = DME_OPT_MENU_STATE_TOP;
		
//		main_work->flag |= DMD_OPT_FLAG_CHNG_STAFFROLL;
		
		// フェード処理開始
		if (dm_opt_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_BLACK_FADEOUT
								, DMD_OPT_FADEOUT_TIME
								, TRUE
								);
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_OPT_FADEOUT_TIME
						   );
		}
		
		// BGMもフェードアウトさせる
		if (!dm_opt_is_pause_maingame) {
			DmSndBgmPlayerBgmStop();
//			DmSoundStopStageBGM(DMD_OPT_BGM_FADEOUT_TIME);
		}
		else {
			GsSoundStopBgm(main_work->bgm_scb
						   , DMD_OPT_BGM_FADEOUT_TIME
						   );
		}
		
		break;
		
	default:
		break;
	}
	
}



// ==========================================================================
// dmOptSetDefaultDataSetMenu
/*!
	オプション設定画面のデフォルトデータ設定処理
 */
// ==========================================================================
void dmOptSetDefaultDataSetMenu(DMS_OPT_MAIN_WORK *main_work)
{
	// ボリュームデータの初期データ設定
	for (u32 i = 0; i < 2; i++) {
		main_work->volume_data[i] = DMD_OPT_VOL_DATA_MAX;
	}
	
	// ボリューム変更処理
	DmSoundSetVolumeBGM((Float)main_work->volume_data[0]);
	DmSoundSetVolumeSE((Float)main_work->volume_data[1]);
	
	// 振動設定の初期データ設定
	main_work->set_vbrt = DME_OPT_CTRL_VBRT_ON;
	
	// 振動設定処理はなし
	
}



////////////////////// FOCUS切り替え関連 /////////////////////////////////////

// ==========================================================================
// dmOptSetTopMenuDecideEfctData
/*!
	オプション時の縦のACT切り替え時の設定処理
 */
// ==========================================================================
void dmOptSetTopMenuDecideEfctData(DMS_OPT_MAIN_WORK *main_work)
{
	// 決定演出フラグON
	main_work->flag |= DMD_OPT_FLAG_DECIDE_EFCT;
}



// ==========================================================================
// dmOptSetTopMenuTabDecideEfct
/*!
	オプション時の縦のACT切り替え時演出中処理
 */
// ==========================================================================
void dmOptSetTopMenuTabDecideEfct(DMS_OPT_MAIN_WORK *main_work)
{
	float tmp_add_alpha = 0;
	float tmp_alpha = 0;
	
	tmp_alpha = (f32)main_work->decide_menu_col.a;
	
	// フェードカラー上昇時
	if (main_work->timer <= DMD_OPT_DECIDE_EFCT_UP_TIME) {
		tmp_add_alpha = DMD_OPT_DECIDE_FADE_COL_MAX / DMD_OPT_DECIDE_EFCT_UP_TIME;
		
		tmp_alpha += tmp_add_alpha;
		
		if (tmp_alpha >= DMD_OPT_DECIDE_FADE_COL_MAX) {
			tmp_alpha = DMD_OPT_DECIDE_FADE_COL_MAX;
		}
	}
	
	// フェードカラー下降時
	else if (main_work->timer <= DMD_OPT_DECIDE_EFCT_UP_TIME + DMD_OPT_DECIDE_EFCT_DOWN_TIME) {
		tmp_add_alpha = DMD_OPT_DECIDE_FADE_COL_MAX / DMD_OPT_DECIDE_EFCT_DOWN_TIME;
		
		tmp_alpha -= tmp_add_alpha;
		
		if (tmp_alpha < DMD_OPT_DECIDE_FADE_COL_MIN) {
			tmp_alpha = DMD_OPT_DECIDE_FADE_COL_MIN;
		}
	}
	
	main_work->decide_menu_col.a = (u8)tmp_alpha;
	
}



// ==========================================================================
// dmOptIsTopMenuTabDecideEfctEnd
/*!
	オプション時の縦のACT切り替え時演出中処理
 */
// ==========================================================================
BOOL dmOptIsTopMenuTabDecideEfctEnd(DMS_OPT_MAIN_WORK *main_work)
{
	// 掃け演出終了チェック
	if (main_work->timer > DMD_OPT_DECIDE_EFCT_WAIT_TIME) {
		// フラグOFF
		main_work->flag &= ~DMD_OPT_FLAG_DECIDE_EFCT;
		
		main_work->decide_menu_col.a = 0;
		main_work->timer = 0;
		
		return TRUE;
	}

	return FALSE;
}



// ==========================================================================
// dmOptSetCtrlFocusChangeEfct
/*!
	オプション時の縦のACT切り替え時演出中処理
 */
// ==========================================================================
void dmOptSetCtrlFocusChangeEfct(DMS_OPT_MAIN_WORK *main_work)
{
	float move_dist = 0.f;
	float distance = 0.f;

	// 縦移動する場合
	distance = main_work->dst_crsr_pos_y - main_work->src_crsr_pos_y;
	
	move_dist = distance / DMD_OPT_CRSR_MOVE_TIME;
	
	main_work->top_crsr_pos_y += move_dist;
}



// ==========================================================================
// dmOptIsCtrlFocusChangeEfctEnd
/*!
	オプション時の縦のACT切り替え時演出中処理
 */
// ==========================================================================
BOOL dmOptIsCtrlFocusChangeEfctEnd(DMS_OPT_MAIN_WORK *main_work)
{
	float move_direct = 0.f;
	
	move_direct = main_work->dst_crsr_pos_y
					- main_work->src_crsr_pos_y;
	
	// 掃け演出終了チェック
	if (main_work->top_crsr_pos_y >= main_work->dst_crsr_pos_y
		 && move_direct >= 0) {
		main_work->top_crsr_pos_y = main_work->dst_crsr_pos_y;
		
		return TRUE;
	}
	
	else if (main_work->top_crsr_pos_y <= main_work->dst_crsr_pos_y
		 && move_direct <= 0) {
		main_work->top_crsr_pos_y = main_work->dst_crsr_pos_y;
		
		return TRUE;
	}
	
//	main_work->timer++;
	
	return FALSE;
}



// ==========================================================================
// dmOptSetVolPushEfct
/*!
	設定画面時のボタン押し演出中処理
 */
// ==========================================================================
void dmOptSetVolPushEfct(DMS_OPT_MAIN_WORK *main_work)
{
	
	for (int i = 0; i < 4; i++) {
		if (main_work->push_efct_timer[i] > 0.f) {
			main_work->push_efct_timer[i] -= 1.0f;		// ◆仮→PAL対応時は要修正
		}
		else {
			main_work->push_efct_timer[i] = 0.f;
		}
	}
	
}



// ==========================================================================
// dmOptSetDfltPushEfct
/*!
	設定画面時のボタン押し演出中処理
 */
// ==========================================================================
void dmOptSetDfltPushEfct(DMS_OPT_MAIN_WORK *main_work)
{
	if (main_work->efct_timer > 10.f) {
		main_work->flag &= ~DMD_OPT_FLAG_PUSH_DFLT_BTN;

		main_work->efct_timer = 0.f;
	}
	
	main_work->efct_timer++;
}



// ==========================================================================
// dmOptSetWinOpenEfct
/*!
	ウインドウ入り演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmOptSetWinOpenEfct(DMS_OPT_MAIN_WORK *main_work)
{
	if (main_work->win_timer > DMD_OPT_WIN_EFCT_TIME) {
		// ウインドウ演出終了
		main_work->flag |= DMD_OPT_FLAG_WIN_EFCT_END;

		main_work->win_timer = 0.0f;

		for (u32 i = 0; i < 2; i++) {
			main_work->win_size_rate[i] = DMD_OPT_WIN_DEF_RATE;
		}
	}
	else {
		// タイマー更新(但しフレームレートが可変になるように修正すること)	◆
		main_work->win_timer++;
	}
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->win_size_rate[i] = main_work->win_timer / DMD_OPT_WIN_EFCT_TIME;
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
// dmOptSetWinCloseEfct
/*!
	ウインドウ閉め演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmOptSetWinCloseEfct(DMS_OPT_MAIN_WORK *main_work)
{
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->win_size_rate[i] = main_work->win_timer / DMD_OPT_WIN_EFCT_TIME;
		}
		else {
			main_work->win_size_rate[i] = 0.0f;
		}
		
		if (main_work->win_size_rate[i] < 0.0f) {
			main_work->win_size_rate[i] = 0.0f;
		}
	}

	if (main_work->win_timer < 0.0f) {
		// ウインドウ演出終了
		main_work->flag |= DMD_OPT_FLAG_WIN_EFCT_END;

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
// dmOptSetChngFocusCrsrData
/*!
	表示設定処理
 */
// ==========================================================================
void dmOptSetChngFocusCrsrData(DMS_OPT_MAIN_WORK *main_work)
{
//	if (main_work->flag & DMD_OPT_FLAG_UP_CHNG_CRSR) {
		// カーソル移動開始位置・移動先設定
		main_work->src_crsr_pos_y = main_work->top_crsr_pos_y;
		main_work->dst_crsr_pos_y = DMD_OPT_TOP_MENU_TOP_POS_Y
									+ main_work->cur_slct_top * DMD_OPT_TOP_MENU_DIST_Y;
		
		main_work->flag |= DMD_OPT_FLAG_MENU_CRSR_EFCT;
//	}
/*
	if (main_work->flag & DMD_OPT_FLAG_DOWN_CHNG_CRSR) {
		// カーソル移動開始位置・移動先設定
		main_work->src_crsr_pos_y = main_work->top_crsr_pos_y;
		main_work->dst_crsr_pos_y = DMD_OPT_TOP_MENU_TOP_POS_Y
									+ main_work->cur_slct_top * DMD_OPT_TOP_MENU_DIST_Y;
		
		main_work->flag |= DMD_OPT_FLAG_MENU_CRSR_EFCT;
	}
*/
}



// ==========================================================================
// dmOptIsDataLoad
/*!
	データ読み込み完了チェック処理
 */
// ==========================================================================
s32 dmOptIsDataLoad(DMS_OPT_MAIN_WORK *main_work)
{
	
	for (int i = 0; i < DME_OPT_DATA_TYPE_MAX; i++) {
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
	
#if _PC || _WII
	for (int i = 0; i < DME_OPT_DATA_TYPE_MAX; i++) {
		if (!amFsIsComplete(main_work->user_arc_amb_fs[i])) {
			return 0;
		}
	}
#endif
	
#if _PC || _PS3 || _XBOX || _IPHONE
	for (int i = 0; i < DME_OPT_DATA_TYPE_MAX; i++) {
		if (!amFsIsComplete(main_work->manual_arc_amb_fs[i])) {
			return 0;
		}
	}
#endif
	
//	if (!amFsIsComplete(main_work->win_amb_fs)) {
//		return 0;
//	}
	
	return 1;
}


// ==========================================================================
// dmOptIsTexLoad
/*!
	テクスチャ構築完了チェック処理
 */
// ==========================================================================
s32 dmOptIsTexLoad(DMS_OPT_MAIN_WORK *main_work)
{
	
	for (int i = 0; i < DME_OPT_DATA_TYPE_MAX; i++) {
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
	
	// サウンドビルドチェックも仮でここに配置
	if (!dm_opt_is_pause_maingame) {
		if (!DmSndBgmPlayerIsSndSysBuild()) {
			return 0;
		}
	}
	
#if _PC || _WII
	if (!DmUserNameBuildCheck()) {
		return 0;
	}
#endif

	return 1;
}



// ==========================================================================
// dmOptIsTexLoad2
/*!
	テクスチャ構築完了チェック処理
 */
// ==========================================================================
s32 dmOptIsTexLoad2(DMS_OPT_MAIN_WORK *main_work)
{
	UNREFERENCED_PARAMETER(main_work);
	
#if _PC || _PS3 || _XBOX || _IPHONE
	if (!DmManualBuildCheck()) {
		return 0;
	}
#endif
	
	return 1;
}



// ==========================================================================
// dmOptIsTexRelease
/*!
	テクスチャ解放完了チェック処理
 */
// ==========================================================================
s32 dmOptIsTexRelease(DMS_OPT_MAIN_WORK *main_work)
{

	for (int i = 0; i < DME_OPT_DATA_TYPE_MAX; i++) {
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
	
//	if (!AoTexIsReleased(&main_work->win_tex)) {
//		// フラグ扱いでON
//		return 0;
//	}

#if _PC || _WII
	if (!DmUserNameFlushCheck()) {
		return 0;
	}
#endif

#if _PC || _PS3 || _XBOX || _IPHONE
	if (!DmManualFlushCheck()) {
		return 0;
	}
#endif
	
//	if (!AoTexIsReleased(&dm_opt_win_tex)) {
//		return 0;
//	}
	

	return 1;
}




// ===========================================================================
//	dmOptGetRevisedTopMenuNo
/*!
	項目の場所を示す変数が最大・最小値を超えた際に補正した(回り込みさせた)値を取得する関数
	通常時のカーソル移動の選択項目に使用。

	@param id		[in] 項目番号
	@param diff		[in] 変化量(移動における)
	@return 補正された項目番号
*/
// ===========================================================================
s32 dmOptGetRevisedTopMenuNo(s32 idx, s32 diff)
{
	s32 result;
	
	result = (int)idx + diff;

	// 先頭から一つ戻ると最後に移動
	if (result < 0) {
		result = DME_OPT_TOP_MENU_NUM - 1;
	}
	
	// 最後から一つ進むと先頭に移動
	if (result >= DME_OPT_TOP_MENU_NUM) {
		result = 0;
	}
	
	MTM_ASSERT(result >= 0 && result < DME_OPT_TOP_MENU_NUM);
	
	return result;
}



// ===========================================================================
//	dmOptGetRevisedSettingMenuNo
/*!
	項目の場所を示す変数が最大・最小値を超えた際に補正した(回り込みさせた)値を取得する関数
	通常時のカーソル移動の選択項目に使用。

	@param id		[in] 項目番号
	@param diff		[in] 変化量(移動における)
	@return 補正された項目番号
*/
// ===========================================================================
s32 dmOptGetRevisedSettingMenuNo(s32 idx, s32 diff)
{
	s32 result;
	
	result = (int)idx + diff;

	// 先頭から一つ戻ると最後に移動
	if (result < 0) {
		result = DME_OPT_SET_MENU_NUM - 1;
	}
	
	// 最後から一つ進むと先頭に移動
	if (result >= DME_OPT_SET_MENU_NUM) {
		result = 0;
	}
	
	MTM_ASSERT(result >= 0 && result < DME_OPT_SET_MENU_NUM);
	
	return result;
}



// ===========================================================================
//	dmOptGetRevisedVolume
/*!
	項目の場所を示す変数が最大・最小値を超えた際に補正した(回り込みさせた)値を取得する関数
	通常時のカーソル移動の選択項目に使用。

	@param id		[in] 項目番号
	@param diff		[in] 変化量(移動における)
	@return 補正された項目番号
*/
// ===========================================================================
s32 dmOptGetRevisedVolume(s32 idx, s32 diff)
{
	s32 result;
	
	result = (int)idx + diff;

	// 先頭から一つ戻ると最後に移動
	if (result < 0) {
		result = 0;
	}
	
	// 最後から一つ進むと先頭に移動
	if (result > DMD_OPT_VOL_DATA_MAX) {
		result = DMD_OPT_VOL_DATA_MAX;
	}
	
	MTM_ASSERT(result >= 0 && result <= DMD_OPT_VOL_DATA_MAX);
	
	return result;
}


#if _IPHONE
// ===========================================================================
//	dmOptControlResetAct
/*!
	操作方法用にアクションを再設定する
*/
// ===========================================================================
void dmOptControlResetAct(DMS_OPT_MAIN_WORK *main_work)
{
	using namespace gs::backup;
	
	struct SResetLocalTable {
		DME_OPT_ACT	act_idx;	//<作用させるAOS_ACTION*構造体のインデックス
		int			act_id;		//<作用後のアクションインデックス
		int			ama_idx;	//<作用後のAMAファイル
	};
	const SResetLocalTable c_reset_table[SOption::EControl::Max][23] = {
		//傾斜
		{	{ACT_SOUSA_SCREEN,		IDA_D_OPTION_ACT_SCREEN_NORMAL,			DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_SONIC1,	IDA_D_OPTION_ACT_SOUSA_LINE_SONIC1,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_SONIC2,	IDA_D_OPTION_ACT_SOUSA_LINE_SONIC2,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_SONIC3,	IDA_D_OPTION_ACT_SOUSA_LINE_SONIC3,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_SSICON,		IDA_D_OPTION_ACT_SOUSA_SSICON,			DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_FUKIDASHI,	IDA_D_OPTION_ACT_SOUSA_HUKIDASHI,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_PA1,	IDA_D_OPTION_ACT_SOUSA_LINE_PA1,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_PA2,	IDA_D_OPTION_ACT_SOUSA_LINE_PA2,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_PA3,	IDA_D_OPTION_ACT_SOUSA_LINE_PA3,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_JUMP1,	IDA_D_OPTION_ACT_SOUSA_LINE_JUMP1,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_JUMP2,	IDA_D_OPTION_ACT_SOUSA_LINE_JUMP2,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_JUMP3,	IDA_D_OPTION_ACT_SOUSA_LINE_JUMP3,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_MOVE1,	IDA_D_OPTION_ACT_SOUSA_LINE_MOVE1,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_MOVE2,	IDA_D_OPTION_ACT_SOUSA_LINE_MOVE2,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_MOVE3,	-1,										-1},
			{ACT_SOUSA_YUBI,		IDA_D_OPTION_ACT_SOUSA_YUBI,			DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_TEX_EX_A,			IDA_D_OPTION_JP_ACT_SOUSA_TEX_EX_A,		DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_EX_B,			IDA_D_OPTION_JP_ACT_SOUSA_TEX_EX_B,		DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_SUPER,			IDA_D_OPTION_JP_ACT_SOUSA_TEX_SUPER,	DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_TAP,			IDA_D_OPTION_JP_ACT_SOUSA_TEX_TAP,		DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_PAUSE,			IDA_D_OPTION_JP_ACT_SOUSA_TEX_PAUSE,	DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_JUMP,			IDA_D_OPTION_JP_ACT_SOUSA_TEX_JUMP,		DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_MOVE,			IDA_D_OPTION_JP_ACT_SOUSA_TEX_MOVE,		DME_OPT_DATA_TYPE_LANG_DATA},
		},
		//バーチャルパッド(下)
		{	{ACT_SOUSA_SCREEN,		IDA_D_OPTION_ACT_SCREEN_A,				DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_SONIC1,	IDA_D_OPTION_ACT_A_SOUSA_LINE_SONIC1,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_SONIC2,	IDA_D_OPTION_ACT_A_SOUSA_LINE_SONIC2,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_SONIC3,	IDA_D_OPTION_ACT_A_SOUSA_LINE_SONIC3,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_SSICON,		IDA_D_OPTION_ACT_A_SOUSA_SSICON,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_FUKIDASHI,	-1,										-1},
			{ACT_SOUSA_LINE_PA1,	IDA_D_OPTION_ACT_A_SOUSA_LINE_PA1,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_PA2,	IDA_D_OPTION_ACT_A_SOUSA_LINE_PA2,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_PA3,	IDA_D_OPTION_ACT_A_SOUSA_LINE_PA3,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_JUMP1,	IDA_D_OPTION_ACT_A_SOUSA_LINE_JUMP1,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_JUMP2,	IDA_D_OPTION_ACT_A_SOUSA_LINE_JUMP2,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_JUMP3,	IDA_D_OPTION_ACT_A_SOUSA_LINE_JUMP3,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_MOVE1,	IDA_D_OPTION_ACT_A_SOUSA_LINE_MOVE1,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_MOVE2,	IDA_D_OPTION_ACT_A_SOUSA_LINE_MOVE2,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_MOVE3,	IDA_D_OPTION_ACT_A_SOUSA_LINE_MOVE3,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_YUBI,		-1,										-1},
			{ACT_TEX_EX_A,			IDA_D_OPTION_JP_ACT_SOUSA_TEX_EX_A2,	DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_EX_B,			IDA_D_OPTION_JP_ACT_SOUSA_TEX_EX_B2,	DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_SUPER,			IDA_D_OPTION_JP_ACT_A_SOUSA_TEX_SUPER,	DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_TAP,			-1,										-1},
			{ACT_TEX_PAUSE,			IDA_D_OPTION_JP_ACT_A_SOUSA_TEX_PAUSE,	DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_JUMP,			IDA_D_OPTION_JP_ACT_A_SOUSA_TEX_JUMP,	DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_MOVE,			IDA_D_OPTION_JP_ACT_A_SOUSA_TEX_MOVE,	DME_OPT_DATA_TYPE_LANG_DATA},
		},
		//バーチャルパッド(上)
		{	{ACT_SOUSA_SCREEN,		IDA_D_OPTION_ACT_SCREEN_A,				DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_SONIC1,	IDA_D_OPTION_ACT_A_SOUSA_LINE_SONIC1,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_SONIC2,	IDA_D_OPTION_ACT_A_SOUSA_LINE_SONIC2,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_SONIC3,	IDA_D_OPTION_ACT_A_SOUSA_LINE_SONIC3,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_SSICON,		IDA_D_OPTION_ACT_A_SOUSA_SSICON,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_FUKIDASHI,	-1,										-1},
			{ACT_SOUSA_LINE_PA1,	IDA_D_OPTION_ACT_A_SOUSA_LINE_PA1,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_PA2,	IDA_D_OPTION_ACT_A_SOUSA_LINE_PA2,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_PA3,	IDA_D_OPTION_ACT_A_SOUSA_LINE_PA3,		DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_JUMP1,	IDA_D_OPTION_ACT_A_SOUSA_LINE_JUMP1,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_JUMP2,	IDA_D_OPTION_ACT_A_SOUSA_LINE_JUMP2,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_JUMP3,	IDA_D_OPTION_ACT_A_SOUSA_LINE_JUMP3,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_MOVE1,	IDA_D_OPTION_ACT_A_SOUSA_LINE_MOVE1,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_MOVE2,	IDA_D_OPTION_ACT_A_SOUSA_LINE_MOVE2,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_LINE_MOVE3,	IDA_D_OPTION_ACT_A_SOUSA_LINE_MOVE3,	DME_OPT_DATA_TYPE_CMN_DATA},
			{ACT_SOUSA_YUBI,		-1,										-1},
			{ACT_TEX_EX_A,			IDA_D_OPTION_JP_ACT_SOUSA_TEX_EX_A2,	DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_EX_B,			IDA_D_OPTION_JP_ACT_SOUSA_TEX_EX_B2,	DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_SUPER,			IDA_D_OPTION_JP_ACT_A_SOUSA_TEX_SUPER,	DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_TAP,			-1,										-1},
			{ACT_TEX_PAUSE,			IDA_D_OPTION_JP_ACT_A_SOUSA_TEX_PAUSE,	DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_JUMP,			IDA_D_OPTION_JP_ACT_A_SOUSA_TEX_JUMP,	DME_OPT_DATA_TYPE_LANG_DATA},
			{ACT_TEX_MOVE,			IDA_D_OPTION_JP_ACT_A_SOUSA_TEX_MOVE,	DME_OPT_DATA_TYPE_LANG_DATA},
		},
	};
	//既存のアクションは破棄
	for (const SResetLocalTable *reset = c_reset_table[0], *reset_end = c_reset_table[0] + arrayof(c_reset_table[0]); reset != reset_end; ++reset) {
		if (main_work->act[reset->act_idx]) {
			AoActDelete(main_work->act[reset->act_idx]);
			main_work->act[reset->act_idx] = NULL;
		}
	}
	//新規アクション構築
	SOption::EControl::Type ctrl = SOption::CreateInstance().GetControl();
	for (const SResetLocalTable *reset = c_reset_table[ctrl], *reset_end = c_reset_table[ctrl] + arrayof(c_reset_table[ctrl]); reset != reset_end; ++reset) {
		if (0 <= reset->act_id) {
			main_work->act[reset->act_idx] = AoActCreate(main_work->ama[reset->ama_idx], reset->act_id);
		}
	}
	//傾斜操作時はフレームを全リセット
	if (SOption::EControl::Tilt == ctrl) {
		for (AOS_ACTION **act = &main_work->act[ACT_TAB_SOUSA_01L], **act_end = &main_work->act[ACT_S_SCREEN_01B+1]; act != act_end; ++act) {
			if (*act) {
				AoActSetFrame(*act, 0.0f);
			}
		}
		for (AOS_ACTION **act = &main_work->act[ACT_TEX_EX_A], **act_end = &main_work->act[ACT_TEX_MOVE+1]; act != act_end; ++act) {
			if (*act) {
				AoActSetFrame(*act, 0.0f);
			}
		}
	}
}
#endif //_IPHONE


// ==========================================================================
// DmOptionStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void DmOptionStaticVarInit(void)
{
	memset(&dm_opt_mgr, 0, sizeof(dm_opt_mgr));
	dm_opt_mgr_p = NULL;
	
	memset(&dm_opt_win_tex, 0, sizeof(dm_opt_win_tex));
	
	dm_opt_draw_state = 0;
	dm_opt_is_pause_maingame = FALSE;
	
	dm_opt_prev_evt = 0;
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
