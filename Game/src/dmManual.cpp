// ===========================================================================
/*!
	@file	dmManual.cpp
	@brief	デモ・オンラインマニュアル画面

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: dmManual.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 *
 *
 */

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#include "dmManual.h"
#include "../library/ao/include/aoTexture.h"
#include "../library/ao/include/aoAction.h"

#include "gs.h"
#include "gsMainSys.h"
#include "izFade.h"
#include "aoWinSys.h"
#include "gsSound.h"
#include "dmSound.h"

#if _IPHONE
#include "erTrgAoAction.hpp"
#endif //_IPHONE

// データヘッダ
#if !_IPHONE
#include "common/ace/D_MANUAL.HMA"
#include "common/ace/D_MANUAL_JP.HMA"
#else //!_IPHONE
#include "ace/D_MANUAL.HMA"
#include "ace/D_MANUAL_JP.HMA"
#endif //!_IPHONE


// ----- Macros ------------------------------------------------（マクロ定義）

#define DMD_MANUAL_TASK_PAUSELEVEL		(0x7fff)
#define DMD_MANUAL_TASK_PRIO_MAIN		(0x3000)
#define DMD_MANUAL_TASK_GROUP_MAIN		(10)

#define DMD_MANUAL_FILE_PATH_NUM_MAX	(60)

#define DMD_MANUAL_CMN_DATA_FILENAME	(GSS_BASE_PATH"DEMO/CMN/D_CMN_WIN.AMB")
#define DMD_MANUAL_DATA_FILENAME		(GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER.AMB")

#define DMD_MANUAL_SIZE_WIDTH			(960.0f)
#define DMD_MANUAL_SIZE_HEIGHT			(720.0f)
#define DMD_MANUAL_SIZE_HALF_WIDTH		(480.0f)
#define DMD_MANUAL_SIZE_HALF_HEIGHT		(360.0f)


// プライオリティ設定
#define DMD_MANUAL_DRAW_PRIO_CHAR		(0x3000)
#define DMD_MANUAL_DRAW_PRIO_BG			(0x2000)
#define DMD_MANUAL_DRAW_PRIO_FIX		(0x2800)

// 表示位置
#define DMD_MANUAL_DRAW_STATE_ID		(0)

#if _PS3
#if defined(HOG_RGN_JP)
#define DMD_MANUAL_DISP_PAGE_NUM		(18)
#else
#define DMD_MANUAL_DISP_PAGE_NUM		(15)
#endif
#elif _IPHONE
#define DMD_MANUAL_DISP_PAGE_NUM		(15)
#else
#define DMD_MANUAL_DISP_PAGE_NUM		(14)
#endif

// フェード関連
#define DMD_MANUAL_FADEIN_TIME			(32.0f)
#define DMD_MANUAL_FADEOUT_TIME			(32.0f)

// 演出関連
#if _PS3
#define DMD_MANUAL_PAGE_NO_SRC			(0)
#if defined(HOG_RGN_JP)
#define DMD_MANUAL_PAGE_NO_DST			(18)
#define DMD_MANUAL_CAUTION_PAGE_NUM		(4)
#else
#define DMD_MANUAL_PAGE_NO_DST			(15)
#define DMD_MANUAL_CAUTION_PAGE_NUM		(1)
#endif
#elif _IPHONE
#define DMD_MANUAL_PAGE_NO_SRC			(0)
#define DMD_MANUAL_PAGE_NO_DST			(DMD_MANUAL_DISP_PAGE_NUM)
#define DMD_MANUAL_CAUTION_PAGE_NUM		(1)
#else
#define DMD_MANUAL_PAGE_NO_SRC			(0)
#define DMD_MANUAL_PAGE_NO_DST			(14)
#define DMD_MANUAL_CAUTION_PAGE_NUM		(1)
#endif

#define DMD_MANUAL_MAX_PAGE_DISP_10		(0.f)

#if _PS3
#if defined(HOG_RGN_JP)
#define DMD_MANUAL_MAX_PAGE_DISP_1		(0.f)
#else
#define DMD_MANUAL_MAX_PAGE_DISP_1		(1.f)
#endif
#else
#define DMD_MANUAL_MAX_PAGE_DISP_1		(0.f)
#endif

// フラグ関連
#define DMD_MANUAL_FLAG_EXIT			(1 << 0)		//!< 終了フラグ
#define DMD_MANUAL_FLAG_CANCEL			(1 << 1)		//!< キャンセル
#define DMD_MANUAL_FLAG_DECIDE			(1 << 2)		//!< 決定フラグ
#define DMD_MANUAL_FLAG_NEXT_PAGE		(1 << 3)		//!< 次のページへ
#define DMD_MANUAL_FLAG_PREV_PAGE		(1 << 4)		//!< 前のページへ

#define DMD_MANUAL_FLAG_SIGN_OUT_EXIT	(1 << 31)	// サインアウト時の終了フラグ


// ----- Macro Functions -----------------------------------（処理マクロ定義）


// ----- Definitions -------------------------------------------（定数の宣言）
//!< ファイル種別
typedef enum tag_DME_MANUAL_DATA_TYPE
{
	DME_MANUAL_DATA_TYPE_CMN_DATA = 0,	//!< 共通データ
	DME_MANUAL_DATA_TYPE_LANG_DATA,		//!< 言語別データ
	
	DME_MANUAL_DATA_TYPE_MAX,
	DME_MANUAL_DATA_TYPE_NONE
} DME_MANUAL_DATA_TYPE;


//! アクションテーブル(ノード含む)
typedef enum tag_DME_MANUAL_ACT
{
	// 言語共通
	// FIX類
	ACT_BG04A,
	ACT_BG04B,
	ACT_BG01A,
	ACT_BG01B,
	ACT_BG02,
//	ACT_BG03A,
//	ACT_BG03B,
#if !_IPHONE
	ACT_OBI,
	ACT_BUT01,
	ACT_ICON_PAGE01,
	ACT_ICON_PAGE02,
	ACT_ICON_PAGE03,
#endif //!_IPHONE
	ACT_NUM1,
	ACT_NUM2,
	ACT_NUM3,
	ACT_NUM4,
	ACT_NUM5,
	ACT_ARROW_L,
	ACT_ARROW_R,
#if _IPHONE
	ACT_BACK_L,
	ACT_BACK_C,
#endif //_IPHONE
	
	
	ACT_TAB01_L,
	ACT_TAB01_C,
	ACT_TAB01_R,
	ACT_SCR01,
	
	ACT_TAB02_L,
	ACT_TAB02_C,
	ACT_TAB02_R,
	ACT_TAB02_L2,
	ACT_TAB02_C2,
	ACT_TAB02_R2,
	ACT_SONIC,
	ACT_EGGMAN,
	
	ACT_TAB03_L,
	ACT_TAB03_C,
	ACT_TAB03_R,
	ACT_TAB03_L2,
	ACT_TAB03_C2,
	ACT_TAB03_R2,
	ACT_CNT01A,
	ACT_CNT01B,
#if _IPHONE
	ACT_CNT01C,
	ACT_YUBI,
	ACT_TAB03_L3,
	ACT_TAB03_C3,
	ACT_TAB03_R3,
	ACT_TAB03_L4,
	ACT_TAB03_C4,
	ACT_TAB03_R4,
#endif //_IPHONE
	ACT_CNT02A,
	ACT_CNT02B,
	
	ACT_TAB04_L,
	ACT_TAB04_C,
	ACT_TAB04_R,
	ACT_SCR04,
	
	ACT_TAB05_L,
	ACT_TAB05_C,
	ACT_TAB05_R,
	ACT_TAB05_L2,
	ACT_TAB05_C2,
	ACT_TAB05_R2,
	ACT_SCR05A,
	ACT_SCR05B,
	
	ACT_TAB06A,
	ACT_TAB06B,
	ACT_TAB06C,
	ACT_TAB06D_L,
	ACT_TAB06D_C,
	ACT_TAB06D_R,
	ACT_BOU_A,
	ACT_BOU_B,
	ACT_BOU_C,
	ACT_BOU_D,
	ACT_SCR06,
	
	ACT_TAB07_L,
	ACT_TAB07_C,
	ACT_TAB07_R,
	ACT_TAB07_L2,
	ACT_TAB07_C2,
	ACT_TAB07_R2,
	ACT_SCR07,
	
	ACT_TAB08_L,
	ACT_TAB08_C,
	ACT_TAB08_R,
	ACT_TAB08_L2,
	ACT_TAB08_C2,
	ACT_TAB08_R2,
	ACT_SCR08A,
	ACT_SCR08B,
	
	ACT_TAB09_L,
	ACT_TAB09_C,
	ACT_TAB09_R,
	ACT_TAB09_L2,
	ACT_TAB09_C2,
	ACT_TAB09_R2,
	ACT_TAB09_L3,
	ACT_TAB09_C3,
	ACT_TAB09_R3,
	ACT_SCR09A,
	ACT_SCR09B,
	ACT_SCR09C,
	
	ACT_TAB10_L,
	ACT_TAB10_C,
	ACT_TAB10_R,
	ACT_TAB10_L2,
	ACT_TAB10_C2,
	ACT_TAB10_R2,
	ACT_TAB10_L3,
	ACT_TAB10_C3,
	ACT_TAB10_R3,
	ACT_SCR10A,
	ACT_SCR10B,
	ACT_SCR10C,
	
	ACT_TAB11_L,
	ACT_TAB11_C,
	ACT_TAB11_R,
	ACT_SCR11,
	
#if !_IPHONE
	ACT_TAB12_L,
	ACT_TAB12_C,
	ACT_TAB12_R,
#endif //!_IPHONE
	
	ACT_TAB13_L,
	ACT_TAB13_C,
	ACT_TAB13_R,
	ACT_SCR13,
	
#if !_IPHONE
	ACT_TAB_CAUTION_C,
	ACT_TAB_CAUTION_L,
	ACT_TAB_CAUTION_R,
#else //!_IPHONE
	ACT_TAB_CRI_L,
	ACT_TAB_CRI_C,
	ACT_TAB_CRI_R,
	ACT_DYNA,
#endif //!_IPHONE
	ACT_CRILOGO,
#if !_IPHONE
	ACT_TAB_CAUTION_C2,
	ACT_TAB_CAUTION_L2,
	ACT_TAB_CAUTION_R2,
	
	ACT_TAB00_L,
	ACT_TAB00_C,
	ACT_TAB00_R,
	ACT_TAB00_L2,
	ACT_TAB00_C2,
	ACT_TAB00_R2,
	ACT_TAB00_L3,
	ACT_TAB00_C3,
	ACT_TAB00_R3,
	ACT_CNT03A,
	ACT_CNT03B,
#endif //!_IPHONE
	
	
	// 言語別
	// FIX類
	ACT_TEX_MODORU,
#if !_IPHONE
	ACT_TEX_PAGE,
#endif //!_IPHONE
	ACT_TEX_ASOBI,
	
	// ページ別
	ACT_TEX_TIT01,
	ACT_TEX_EX01,
	
	ACT_TEX_TIT02,
	ACT_TEX_EX02A,
	ACT_TEX_EX02B,
	
	ACT_TEX_TIT03A,
	ACT_TEX_TIT03B,
	ACT_TEX_EX03A,
	ACT_TEX_EX03B,
#if _IPHONE
	ACT_TEX_EX03TAP,
	ACT_TEX_TIT03A_CNT,
	ACT_TEX_TIT03B_CNT,
	ACT_TEX_TIT03A2,
	ACT_TEX_TIT03B2,
	ACT_TEX_EX03A2,
	ACT_TEX_EX03B2,
	ACT_TEX_TIT03A_CNT2,
	ACT_TEX_TIT03B_CNT2,
#endif //_IPHONE
	
	ACT_TEX_TIT04,
	ACT_TEX_EX04,
	
	ACT_TEX_TIT05A,
	ACT_TEX_TIT05B,
	ACT_TEX_EX05A,
	ACT_TEX_EX05B,
#if _IPHONE
	ACT_TEX_TIT05A_CNT,
	ACT_TEX_TIT05B_CNT,
	ACT_TEX_TIT05A2,
	ACT_TEX_TIT05B2,
	ACT_TEX_EX05A2,
	ACT_TEX_EX05B2,
	ACT_TEX_TIT05A_CNT2,
	ACT_TEX_TIT05B_CNT2,
#endif //_IPHONE
	
	ACT_TEX_TIT06,
	ACT_TEX_EX06A,
	ACT_TEX_EX06B,
	ACT_TEX_EX06C,
	ACT_TEX_EX06D,
	
	ACT_TEX_TIT07,
	ACT_TEX_EX07A,
	ACT_TEX_EX07B,
	
	ACT_TEX_TIT08A,
	ACT_TEX_TIT08B,
	ACT_TEX_EX08A,
	ACT_TEX_EX08B,
	
	ACT_TEX_TIT09A,
	ACT_TEX_TIT09B,
	ACT_TEX_EX09A,
	ACT_TEX_EX09B,
	ACT_TEX_EX09C,
	
	ACT_TEX_TIT10,
	ACT_TEX_EX10A,
	ACT_TEX_EX10B,
	ACT_TEX_EX10C,
	ACT_TEX_EX10D,
	
	ACT_TEX_TIT11,
	ACT_TEX_EX11,
	
#if !_IPHONE
	ACT_TEX_TIT12,
	ACT_TEX_EX12,
	ACT_SCR12,
#endif //!_IPHONE
	
	ACT_TEX_TIT13,
	ACT_TEX_EX13,
	
#if !_IPHONE
	ACT_TEX_CAUTION1,
	ACT_TEX_CAUTION2,
	ACT_TEX_CAUTION3,
	ACT_TEX_CAUTION4,
	
	ACT_TEX_CAUTION_LOCAL,
	
	ACT_SCR14A,
	ACT_SCR14B,
	ACT_TEX_EX14A,
	ACT_TEX_EX14B,
	ACT_TEX_EX14C,
#endif //!_IPHONE
	
	
	ACT_NUM,
	
	ACT_NONE
} DME_MANUAL_ACT;


typedef struct tag_DMS_MANUAL_MAIN_WORK	DMS_MANUAL_MAIN_WORK;

//! メインタスクワーク
struct tag_DMS_MANUAL_MAIN_WORK {

	AMS_FS			*arc_amb;							//!< アーカイブAMBファイル
	void			*ama[DME_MANUAL_DATA_TYPE_MAX];	//!< AMAファイル
	void			*amb[DME_MANUAL_DATA_TYPE_MAX];	//!< AMBファイル
	
	AOS_TEXTURE		tex[DME_MANUAL_DATA_TYPE_MAX];		//!< テクスチャ

	// メニュー用アクション
	AOS_ACTION 		*act[ACT_NUM];

	void (*proc_input)(DMS_MANUAL_MAIN_WORK *);		//!< 入力用プロシージャ
	void (*proc_update)(DMS_MANUAL_MAIN_WORK *);		//!< メニュー用プロシージャ
	void (*proc_draw)(DMS_MANUAL_MAIN_WORK *);			//!< 描画用プロシージャ

	float timer;											//!< 汎用タイマー
	u32	flag;											//!< 汎用フラグ
	float efct_timer;

	s32 cur_disp_page;
#if _IPHONE
	s32 cur_disp_page_prev;								//<1f前の表示ページ数
#endif //_IPHONE
	
	BOOL is_jp_region;									//!< 
	BOOL is_maingame_load;								//!< メインゲームのロード時
	u32 draw_state;
	
	GSS_SND_SE_HANDLE *se_handle;

#if _IPHONE
	er::CTrgAoAction	trg_btn[2];						//ボタントリガ(次へ,前へ)
	er::CTrgAoAction	trg_return;						//戻るトリガ
#endif //_IPHONE
};


//! 管理構造体
typedef struct tag_DMS_MANUAL_MGR {
	MTS_TASK_TCB *tcb;	//!< TCB
} DMS_MANUAL_MGR;



// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static void dmManualInit(void);
static void dmManualProcMain(MTS_TASK_TCB *tcb);
static void dmManualDest(MTS_TASK_TCB *tcb);

// 初期化設定関連
static void dmManualProcInit(DMS_MANUAL_MAIN_WORK *main_work);
static void dmManualProcCreateAct(DMS_MANUAL_MAIN_WORK *main_work);

// メニュー用プロシージャ
static void dmManualProcStopDraw(DMS_MANUAL_MAIN_WORK *main_work);

static void dmManualProcFadeIn(DMS_MANUAL_MAIN_WORK *main_work);
static void dmManualProcWaitInput(DMS_MANUAL_MAIN_WORK *main_work);
static void dmManualProcFadeOut(DMS_MANUAL_MAIN_WORK *main_work);

// 描画関連処理
static void dmManualCommonBgDraw(DMS_MANUAL_MAIN_WORK *main_work);
static void dmManualProcActDraw(DMS_MANUAL_MAIN_WORK *main_work);
static void dmManualCommonDraw(DMS_MANUAL_MAIN_WORK *main_work);
static void dmManualPageDraw(DMS_MANUAL_MAIN_WORK *main_work);
static void dmManualTaskDraw(AMS_TCB* tcb);

// 演出関連設定処理
static void dmManualInputProcMain(DMS_MANUAL_MAIN_WORK *main_work);

static void dmManualSetInitData(DMS_MANUAL_MAIN_WORK *main_work);

static s32 dmManualIsTexLoad(void);
static s32 dmManualIsTexRelease(void);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ----- Static Variables --------------------（スタティック変数の定義：局所）


// ZONEごとのACTテーブル表示位置Yテーブル(言語共通)
const static int dm_manual_disp_act_cmn_tbl[DMD_MANUAL_DISP_PAGE_NUM][2] = {
#if _PS3	// PS3のみ初めに4ページ分追加
#if defined(HOG_RGN_JP)
	{ACT_TAB_CAUTION_C, ACT_TAB_CAUTION_R},		// ctn_1
	{ACT_TAB_CAUTION_C, ACT_CRILOGO},		// ctn_2
	{ACT_TAB_CAUTION_C, ACT_TAB_CAUTION_R},		// ctn_3
	{ACT_TAB_CAUTION_C, ACT_TAB_CAUTION_R},		// ctn_4
#else
	{ACT_CRILOGO, ACT_TAB_CAUTION_R2},		// ctn_l
#endif
#elif _IPHONE
	{ACT_TAB_CRI_L, ACT_CRILOGO},			// ctn_1
#else
	{ACT_CRILOGO, ACT_TAB_CAUTION_R2},		// ctn_l
#endif
	
	{ACT_TAB01_L, ACT_SCR01},		// 1
	{ACT_TAB02_L, ACT_EGGMAN},		// 2
#if !_IPHONE
	{ACT_TAB03_L, ACT_CNT02B},		// 3
#else //!_IPHONE
	{ACT_TAB03_L, ACT_YUBI},		// 3
	{ACT_TAB03_L3, ACT_CNT02B},		// 3append
#endif //!_IPHONE
	{ACT_TAB04_L, ACT_SCR04},		// 4
	{ACT_TAB05_L, ACT_SCR05B},		// 5
#if _IPHONE
	{ACT_TAB05_L, ACT_SCR05B},		// 5append
#endif //_IPHONE
	
#if _PS3	// PS3のみ初めに4ページ分追加
	{ACT_TAB00_L, ACT_CNT03B},		// 5
#endif
	
	{ACT_TAB06A, ACT_SCR06},		// 6
	{ACT_TAB07_L, ACT_SCR07},		// 7
	{ACT_TAB08_L, ACT_SCR08B},		// 8
	{ACT_TAB09_L, ACT_SCR09C},		// 9
	{ACT_TAB10_L, ACT_SCR10C},		// 10
	{ACT_TAB11_L, ACT_SCR11},		// 11
#if !_IPHONE
	{ACT_TAB12_L, ACT_TAB12_R},		// 12
#else //!_IPHONE
	//無し
#endif //!_IPHONE
	{ACT_TAB13_L, ACT_SCR13},		// 13
};



// ZONEごとのACTテーブル表示位置Yテーブル(言語別)
const static int dm_manual_disp_act_lang_tbl[DMD_MANUAL_DISP_PAGE_NUM][2] = {
#if _PS3	// PS3のみ初めに4ページ分追加
#if defined(HOG_RGN_JP)
	{ACT_TEX_CAUTION1, ACT_TEX_CAUTION1},		// ctn_1
	{ACT_TEX_CAUTION2, ACT_TEX_CAUTION2},		// ctn_2
	{ACT_TEX_CAUTION3, ACT_TEX_CAUTION3},		// ctn_3
	{ACT_TEX_CAUTION4, ACT_TEX_CAUTION4},		// ctn_4
#else
	{ACT_TEX_CAUTION_LOCAL, ACT_TEX_CAUTION_LOCAL},	// ctn_2
#endif
#elif _IPHONE
	{1, 0},										// ctn_1(範囲を矛盾させて表示させない)
#else
	{ACT_TEX_CAUTION_LOCAL, ACT_TEX_CAUTION_LOCAL},	// ctn_2
#endif
	
	{ACT_TEX_TIT01, ACT_TEX_EX01},		// 1
	{ACT_TEX_TIT02, ACT_TEX_EX02B},		// 2
#if !_IPHONE
	{ACT_TEX_TIT03A, ACT_TEX_EX03B},	// 3
#else //!_IPHONE
	{ACT_TEX_TIT03A, ACT_TEX_TIT03B_CNT},	// 3
	{ACT_TEX_TIT03A2, ACT_TEX_TIT03B_CNT2},	// 3append
#endif //!_IPHONE
	{ACT_TEX_TIT04, ACT_TEX_EX04},		// 4
#if !_IPHONE
	{ACT_TEX_TIT05A, ACT_TEX_EX05B},	// 5
#else //!_IPHONE
	{ACT_TEX_TIT05A, ACT_TEX_TIT05B_CNT},	// 5
	{ACT_TEX_TIT05A2, ACT_TEX_TIT05B_CNT2},	// 5append
#endif //!_IPHONE
	
#if _PS3	// PS3のみ初めに4ページ分追加
	{ACT_SCR14A, ACT_TEX_EX14C},	// 5
#endif
	
	{ACT_TEX_TIT06, ACT_TEX_EX06D},		// 6
	{ACT_TEX_TIT07, ACT_TEX_EX07B},		// 7
	{ACT_TEX_TIT08A, ACT_TEX_EX08B},	// 8
	{ACT_TEX_TIT09A, ACT_TEX_EX09C},	// 9
	{ACT_TEX_TIT10, ACT_TEX_EX10D},		// 10
	{ACT_TEX_TIT11, ACT_TEX_EX11},		// 11
#if !_IPHONE
	{ACT_TEX_TIT12, ACT_SCR12},			// 12
#else //!_IPHONE
	//無し
#endif //!_IPHONE
	{ACT_TEX_TIT13, ACT_TEX_EX13},		// 13
};



// アクションIDテーブル(初期状態)
const static u32 g_dm_act_id_tbl[ACT_NUM] = {
	
	IDA_D_MANUAL_ACT_BG04A,
	IDA_D_MANUAL_ACT_BG04B,
	IDA_D_MANUAL_ACT_BG01A,
	IDA_D_MANUAL_ACT_BG01B,
	IDA_D_MANUAL_ACT_BG02,
//	IDA_D_MANUAL_ACT_BG03A,
//	IDA_D_MANUAL_ACT_BG03B,
#if !_IPHONE
	IDA_D_MANUAL_ACT_OBI,
	IDA_D_MANUAL_ACT_BUT01,
	IDA_D_MANUAL_ACT_ICON_PAGE01,
	IDA_D_MANUAL_ACT_ICON_PAGE02,
	IDA_D_MANUAL_ACT_ICON_PAGE03,
#endif //!_IPHONE
	IDA_D_MANUAL_ACT_NUM1,
	IDA_D_MANUAL_ACT_NUM2,
	IDA_D_MANUAL_ACT_NUM3,
	IDA_D_MANUAL_ACT_NUM4,
	IDA_D_MANUAL_ACT_NUM5,
	IDA_D_MANUAL_ACT_ARROW_L,
	IDA_D_MANUAL_ACT_ARROW_R,
#if _IPHONE
	IDA_D_MANUAL_ACT_BACK_L,
	IDA_D_MANUAL_ACT_BACK_C,
#endif //_IPHONE
	IDA_D_MANUAL_ACT_TAB01_L,
	IDA_D_MANUAL_ACT_TAB01_C,
	IDA_D_MANUAL_ACT_TAB01_R,
	IDA_D_MANUAL_ACT_SCR01,
	IDA_D_MANUAL_ACT_TAB02_L,
	IDA_D_MANUAL_ACT_TAB02_C,
	IDA_D_MANUAL_ACT_TAB02_R,
	IDA_D_MANUAL_ACT_TAB02_L2,
	IDA_D_MANUAL_ACT_TAB02_C2,
	IDA_D_MANUAL_ACT_TAB02_R2,
	IDA_D_MANUAL_ACT_SONIC,
	IDA_D_MANUAL_ACT_EGGMAN,
	IDA_D_MANUAL_ACT_TAB03_L,
	IDA_D_MANUAL_ACT_TAB03_C,
	IDA_D_MANUAL_ACT_TAB03_R,
	IDA_D_MANUAL_ACT_TAB03_L2,
	IDA_D_MANUAL_ACT_TAB03_C2,
	IDA_D_MANUAL_ACT_TAB03_R2,
	IDA_D_MANUAL_ACT_CNT01A,
	IDA_D_MANUAL_ACT_CNT01B,
#if _IPHONE
	IDA_D_MANUAL_ACT_CNT01C,
	IDA_D_MANUAL_ACT_YUBI,
	IDA_D_MANUAL_ACT_TAB03_L,
	IDA_D_MANUAL_ACT_TAB03_C,
	IDA_D_MANUAL_ACT_TAB03_R,
	IDA_D_MANUAL_ACT_TAB03_L2,
	IDA_D_MANUAL_ACT_TAB03_C2,
	IDA_D_MANUAL_ACT_TAB03_R2,
#endif //_IPHONE
	IDA_D_MANUAL_ACT_CNT02A,
	IDA_D_MANUAL_ACT_CNT02B,
	IDA_D_MANUAL_ACT_TAB04_L,
	IDA_D_MANUAL_ACT_TAB04_C,
	IDA_D_MANUAL_ACT_TAB04_R,
	IDA_D_MANUAL_ACT_SCR04,
	IDA_D_MANUAL_ACT_TAB05_L,
	IDA_D_MANUAL_ACT_TAB05_C,
	IDA_D_MANUAL_ACT_TAB05_R,
	IDA_D_MANUAL_ACT_TAB05_L2,
	IDA_D_MANUAL_ACT_TAB05_C2,
	IDA_D_MANUAL_ACT_TAB05_R2,
	IDA_D_MANUAL_ACT_SCR05A,
	IDA_D_MANUAL_ACT_SCR05B,
	IDA_D_MANUAL_ACT_TAB06A,
	IDA_D_MANUAL_ACT_TAB06B,
	IDA_D_MANUAL_ACT_TAB06C,
	IDA_D_MANUAL_ACT_TAB06D_L,
	IDA_D_MANUAL_ACT_TAB06D_C,
	IDA_D_MANUAL_ACT_TAB06D_R,
	IDA_D_MANUAL_ACT_BOU_A,
	IDA_D_MANUAL_ACT_BOU_B,
	IDA_D_MANUAL_ACT_BOU_C,
	IDA_D_MANUAL_ACT_BOU_D,
	IDA_D_MANUAL_ACT_SCR06,
	IDA_D_MANUAL_ACT_TAB07_L,
	IDA_D_MANUAL_ACT_TAB07_C,
	IDA_D_MANUAL_ACT_TAB07_R,
	IDA_D_MANUAL_ACT_TAB07_L2,
	IDA_D_MANUAL_ACT_TAB07_C2,
	IDA_D_MANUAL_ACT_TAB07_R2,
	IDA_D_MANUAL_ACT_SCR07,
	IDA_D_MANUAL_ACT_TAB08_L,
	IDA_D_MANUAL_ACT_TAB08_C,
	IDA_D_MANUAL_ACT_TAB08_R,
	IDA_D_MANUAL_ACT_TAB08_L2,
	IDA_D_MANUAL_ACT_TAB08_C2,
	IDA_D_MANUAL_ACT_TAB08_R2,
	IDA_D_MANUAL_ACT_SCR08A,
	IDA_D_MANUAL_ACT_SCR08B,
	IDA_D_MANUAL_ACT_TAB09_L,
	IDA_D_MANUAL_ACT_TAB09_C,
	IDA_D_MANUAL_ACT_TAB09_R,
	IDA_D_MANUAL_ACT_TAB09_L2,
	IDA_D_MANUAL_ACT_TAB09_C2,
	IDA_D_MANUAL_ACT_TAB09_R2,
	IDA_D_MANUAL_ACT_TAB09_L3,
	IDA_D_MANUAL_ACT_TAB09_C3,
	IDA_D_MANUAL_ACT_TAB09_R3,
	IDA_D_MANUAL_ACT_SCR09A,
	IDA_D_MANUAL_ACT_SCR09B,
	IDA_D_MANUAL_ACT_SCR09C,
	IDA_D_MANUAL_ACT_TAB10_L,
	IDA_D_MANUAL_ACT_TAB10_C,
	IDA_D_MANUAL_ACT_TAB10_R,
	IDA_D_MANUAL_ACT_TAB10_L2,
	IDA_D_MANUAL_ACT_TAB10_C2,
	IDA_D_MANUAL_ACT_TAB10_R2,
	IDA_D_MANUAL_ACT_TAB10_L3,
	IDA_D_MANUAL_ACT_TAB10_C3,
	IDA_D_MANUAL_ACT_TAB10_R3,
	IDA_D_MANUAL_ACT_SCR10A,
	IDA_D_MANUAL_ACT_SCR10B,
	IDA_D_MANUAL_ACT_SCR10C,
	IDA_D_MANUAL_ACT_TAB11_L,
	IDA_D_MANUAL_ACT_TAB11_C,
	IDA_D_MANUAL_ACT_TAB11_R,
	IDA_D_MANUAL_ACT_SCR11,
#if !_IPHONE
	IDA_D_MANUAL_ACT_TAB12_L,
	IDA_D_MANUAL_ACT_TAB12_C,
	IDA_D_MANUAL_ACT_TAB12_R,
#endif //!_IPHONE
	IDA_D_MANUAL_ACT_TAB13_L,
	IDA_D_MANUAL_ACT_TAB13_C,
	IDA_D_MANUAL_ACT_TAB13_R,
	IDA_D_MANUAL_ACT_SCR13,
	
#if !_IPHONE
	IDA_D_MANUAL_ACT_CAU01_TAB_C,
	IDA_D_MANUAL_ACT_CAU01_TAB_L,
	IDA_D_MANUAL_ACT_CAU01_TAB_R,
#else //!_IPHONE
	IDA_D_MANUAL_ACT_TAB_CRI_L,
	IDA_D_MANUAL_ACT_TAB_CRI_C,
	IDA_D_MANUAL_ACT_TAB_CRI_R,
	IDA_D_MANUAL_ACT_DYNA,
#endif //!_IPHONE
	IDA_D_MANUAL_ACT_CRILOGO,
#if !_IPHONE
	IDA_D_MANUAL_ACT_TAB_CAU2_2_L,
	IDA_D_MANUAL_ACT_TAB_CAU2_2_C,
	IDA_D_MANUAL_ACT_TAB_CAU2_2_R,
	
	IDA_D_MANUAL_ACT_TAB00_L,
	IDA_D_MANUAL_ACT_TAB00_C,
	IDA_D_MANUAL_ACT_TAB00_R,
	IDA_D_MANUAL_ACT_TAB00_L2,
	IDA_D_MANUAL_ACT_TAB00_C2,
	IDA_D_MANUAL_ACT_TAB00_R2,
	IDA_D_MANUAL_ACT_TAB00_L3,
	IDA_D_MANUAL_ACT_TAB00_C3,
	IDA_D_MANUAL_ACT_TAB00_R3,
	IDA_D_MANUAL_ACT_CNT03A,
	IDA_D_MANUAL_ACT_CNT03B,
#endif //!_IPHONE
	
	
	IDA_D_MANUAL_JP_ACT_TEX_MODORU,
#if !_IPHONE
	IDA_D_MANUAL_JP_ACT_TEX_PAGE,
#endif //!_IPHONE
	IDA_D_MANUAL_JP_ACT_TEX_ASOBI,
	IDA_D_MANUAL_JP_ACT_TEX_TIT01,
	IDA_D_MANUAL_JP_ACT_TEX_EX01,
	IDA_D_MANUAL_JP_ACT_TEX_TIT02,
	IDA_D_MANUAL_JP_ACT_TEX_EX02A,
	IDA_D_MANUAL_JP_ACT_TEX_EX02B,
	IDA_D_MANUAL_JP_ACT_TEX_TIT03A,
	IDA_D_MANUAL_JP_ACT_TEX_TIT03B,
	IDA_D_MANUAL_JP_ACT_TEX_EX03A,
	IDA_D_MANUAL_JP_ACT_TEX_EX03B,
#if _IPHONE
	IDA_D_MANUAL_JP_ACT_TEX_EX03TAP,
	IDA_D_MANUAL_JP_ACT_TEX_TIT03A_CNT,
	IDA_D_MANUAL_JP_ACT_TEX_TIT03B_CNT,
	IDA_D_MANUAL_JP_ACT_TEX_TIT03A,
	IDA_D_MANUAL_JP_ACT_TEX_TIT03B,
	IDA_D_MANUAL_JP_ACT_TEX_EX03A2,
	IDA_D_MANUAL_JP_ACT_TEX_EX03B2,
	IDA_D_MANUAL_JP_ACT_TEX_TIT03A_CNT2,
	IDA_D_MANUAL_JP_ACT_TEX_TIT03B_CNT2,
#endif //_IPHONE
	IDA_D_MANUAL_JP_ACT_TEX_TIT04,
	IDA_D_MANUAL_JP_ACT_TEX_EX04,
	IDA_D_MANUAL_JP_ACT_TEX_TIT05A,
	IDA_D_MANUAL_JP_ACT_TEX_TIT05B,
	IDA_D_MANUAL_JP_ACT_TEX_EX05A,
	IDA_D_MANUAL_JP_ACT_TEX_EX05B,
#if _IPHONE
	IDA_D_MANUAL_JP_ACT_TEX_TIT05A_CNT,
	IDA_D_MANUAL_JP_ACT_TEX_TIT05B_CNT,
	IDA_D_MANUAL_JP_ACT_TEX_TIT05A,
	IDA_D_MANUAL_JP_ACT_TEX_TIT05B,
	IDA_D_MANUAL_JP_ACT_TEX_EX05A2,
	IDA_D_MANUAL_JP_ACT_TEX_EX05B2,
	IDA_D_MANUAL_JP_ACT_TEX_TIT05A_CNT2,
	IDA_D_MANUAL_JP_ACT_TEX_TIT05B_CNT2,
#endif //_IPHONE
	IDA_D_MANUAL_JP_ACT_TEX_TIT06,
	IDA_D_MANUAL_JP_ACT_TEX_EX06A,
	IDA_D_MANUAL_JP_ACT_TEX_EX06B,
	IDA_D_MANUAL_JP_ACT_TEX_EX06C,
	IDA_D_MANUAL_JP_ACT_TEX_EX06D,
	IDA_D_MANUAL_JP_ACT_TEX_TIT07,
	IDA_D_MANUAL_JP_ACT_TEX_EX07A,
	IDA_D_MANUAL_JP_ACT_TEX_EX07B,
	IDA_D_MANUAL_JP_ACT_TEX_TIT08A,
	IDA_D_MANUAL_JP_ACT_TEX_TIT08B,
	IDA_D_MANUAL_JP_ACT_TEX_EX08A,
	IDA_D_MANUAL_JP_ACT_TEX_EX08B,
	IDA_D_MANUAL_JP_ACT_TEX_TIT09A,
	IDA_D_MANUAL_JP_ACT_TEX_TIT09B,
	IDA_D_MANUAL_JP_ACT_TEX_EX09A,
	IDA_D_MANUAL_JP_ACT_TEX_EX09B,
	IDA_D_MANUAL_JP_ACT_TEX_EX09C,
	IDA_D_MANUAL_JP_ACT_TEX_TIT10,
	IDA_D_MANUAL_JP_ACT_TEX_EX10A,
	IDA_D_MANUAL_JP_ACT_TEX_EX10B,
	IDA_D_MANUAL_JP_ACT_TEX_EX10C,
	IDA_D_MANUAL_JP_ACT_TEX_EX10D,
	IDA_D_MANUAL_JP_ACT_TEX_TIT11,
	IDA_D_MANUAL_JP_ACT_TEX_EX11,
#if !_IPHONE
	IDA_D_MANUAL_JP_ACT_TEX_TIT12,
	IDA_D_MANUAL_JP_ACT_TEX_EX12,
	IDA_D_MANUAL_JP_ACT_SCR12,
#endif //!_IPHONE
	IDA_D_MANUAL_JP_ACT_TEX_TIT13,
	IDA_D_MANUAL_JP_ACT_TEX_EX13,
	
#if !_IPHONE
	IDA_D_MANUAL_JP_ACT_TEX_CAU01,
	IDA_D_MANUAL_JP_ACT_TEX_CAU02,
	IDA_D_MANUAL_JP_ACT_TEX_CAU03,
	IDA_D_MANUAL_JP_ACT_TEX_CAU04,
	
	IDA_D_MANUAL_JP_ACT_CAU2_2,
	
	IDA_D_MANUAL_JP_ACT_SCR14A,
	IDA_D_MANUAL_JP_ACT_SCR14B,
	IDA_D_MANUAL_JP_ACT_TEX_EX14A,
	IDA_D_MANUAL_JP_ACT_TEX_EX14B,
	IDA_D_MANUAL_JP_ACT_TEX_EX14C,
#endif //!_IPHONE
	
};


//管理情報
static DMS_MANUAL_MGR dm_manual_mgr;
static DMS_MANUAL_MGR *dm_manual_mgr_p = NULL;

static void *dm_manual_ama[DME_MANUAL_DATA_TYPE_MAX];
static void *dm_manual_amb[DME_MANUAL_DATA_TYPE_MAX];
static AOS_TEXTURE dm_manual_tex[DME_MANUAL_DATA_TYPE_MAX];

static u32 dm_manual_draw_state = 0;
static BOOL dm_manual_is_pause_maingame = FALSE;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ==========================================================================
// DmManualBuild
/*!
 *	ファイル選択データ構築
  	(ファイルの読込みは呼び出し側で行い、引数でポインタを渡してこちらでデータ構築)
 */
// ==========================================================================
void DmManualBuild(void *arc_amb[])
{
	int i = 0;

	// 管理情報初期化
	amZeroMemory(&dm_manual_mgr, sizeof(DMS_MANUAL_MGR));
	dm_manual_mgr_p = &dm_manual_mgr;
	
	for (i = 0; i < DME_MANUAL_DATA_TYPE_MAX; i++) {
		amZeroMemory(&dm_manual_tex[i], sizeof(AOS_TEXTURE));
	}

	// AMBファイルロード
	for (i = 0; i < DME_MANUAL_DATA_TYPE_MAX; i++) {
		amBindConv((u8 *)arc_amb[i]);
		
		dm_manual_ama[i] = amBindGet((AMS_AMB_HEADER*)arc_amb[i]
									  , 0
									  );
		
		dm_manual_amb[i] = amBindGet((AMS_AMB_HEADER*)arc_amb[i]
									  , 1
									  );
	}
	
	// アドレス変換
	for (i = 0; i < DME_MANUAL_DATA_TYPE_MAX; i++) {
		amConvertAddress(dm_manual_ama[i]);
		amConvertAddress(dm_manual_amb[i]);
	}
	
	// テクスチャ構築
	for (i = 0; i < DME_MANUAL_DATA_TYPE_MAX; i++) {
		// テクスチャ構築開始
		AoTexBuild(&dm_manual_tex[i], dm_manual_amb[i]);
		AoTexLoad(&dm_manual_tex[i]);
	}
}

// ==========================================================================
// DmManualBuildCheck
/*!
 *	ファイル選択データ構築 終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL DmManualBuildCheck(void)
{
	// テクスチャ構築チェック
	if (dmManualIsTexLoad()) {
		// フラグ扱いでON
		return (TRUE);
	}

	return (FALSE);
}


// ==========================================================================
// DmManualFlush
/*!
 *	ファイル選択データフラッシュ
 *
 *	@reutrn	リングシステムワークアドレス
 */
// ==========================================================================
void DmManualFlush(void)
{
	// テクスチャ解放
	for (int i = 0; i < DME_MANUAL_DATA_TYPE_MAX; i++) {
		AoTexRelease(&dm_manual_tex[i]);
	}
}


// ==========================================================================
// DmManualFlushCheck
/*!
 *	ファイル選択データフラッシュ終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL DmManualFlushCheck(void)
{
	// テクスチャ解放
	if (dmManualIsTexRelease()) {
		
		return (TRUE);
	}
	
	return (FALSE);
}


// ==========================================================================
// DmManualStart
/*!
	オンラインマニュアル画面開始処理
 */
// ==========================================================================
void DmManualStart(void)
{
	dmManualInit();
}



// ==========================================================================
// DmManualIsExit
/*!
	オンラインマニュアル画面の終了確認処理
 */
// ==========================================================================
BOOL DmManualIsExit(void)
{
	if (dm_manual_mgr_p->tcb == NULL) {
		return TRUE;
	}

	return FALSE;
}


// ==========================================================================
// DmManualExit
/*!
	オンラインマニュアル画面の終了処理
 */
// ==========================================================================
void DmManualExit(void)
{
	// タスククリア
	if (dm_manual_mgr_p->tcb) {
		mtTaskClearTcb(dm_manual_mgr_p->tcb);

		dm_manual_mgr_p->tcb = NULL;
	}
}




// ----- Static Functions --------------------（スタティック関数の定義：局所）
// ==========================================================================
// dmManualInit
/*!
	オンラインマニュアル画面初期化処理
 */
// ==========================================================================
void dmManualInit(void)
{
	DMS_MANUAL_MAIN_WORK	*main_work;
	s16 tmp_cur_evt = 0;
	
	// メインタスク作成
	dm_manual_mgr_p->tcb = MTM_TASK_MAKE_TCB(dmManualProcMain
											   , dmManualDest
											   , 0
											   , DMD_MANUAL_TASK_PAUSELEVEL
											   , DMD_MANUAL_TASK_PRIO_MAIN
											   , DMD_MANUAL_TASK_GROUP_MAIN
											   , sizeof(DMS_MANUAL_MAIN_WORK)
											   , "MANUAL_MAIN"
											   );
	
	// ワーク初期化
	main_work = (DMS_MANUAL_MAIN_WORK *)mtTaskGetTcbWork(dm_manual_mgr_p->tcb);
	
	main_work->draw_state = AoActSysGetDrawStateEnable();

	AoActSysSetDrawStateEnable(main_work->draw_state);
	
	if (main_work->draw_state) {
		dm_manual_draw_state = AoActSysGetDrawState();
		AoActSysSetDrawState(dm_manual_draw_state);
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
	dmManualSetInitData(main_work);

	// 現在ロードしているイベントがメインゲームかどうかの設定
	tmp_cur_evt = SyGetEvtInfo()->cur_evt_id;
	
	if (tmp_cur_evt == GSD_EVT_ID_MAINGAME
		|| tmp_cur_evt == GSD_EVT_ID_SPSTAGE_BRANCH) {
		// ポーズメニューからの遷移フラグON
		dm_manual_is_pause_maingame = TRUE;
		
		// SEハンドル確保
		main_work->se_handle = GsSoundAllocSeHandle();
	}
	
	// プロシージャ設定
	main_work->proc_update = dmManualProcInit;
}



// ==========================================================================
// dmManualSetInitData
/*!
	セーブデータにある名前データの設定処理
 */
// ==========================================================================
void dmManualSetInitData(DMS_MANUAL_MAIN_WORK *main_work)
{
	// 初期表示させるページ数を設定
	main_work->cur_disp_page = DMD_MANUAL_PAGE_NO_SRC;
#if _IPHONE
	main_work->cur_disp_page_prev = DMD_MANUAL_PAGE_NO_SRC - 1;
#endif //_IPHONE
	
}



// ==========================================================================
// dmManualProcMain
/*!
	オンラインマニュアル画面メインプロシージャ処理
 */
// ==========================================================================
void dmManualProcMain(MTS_TASK_TCB *tcb)
{
	DMS_MANUAL_MAIN_WORK	*main_work;
	
	// ワーク取得
	main_work = (DMS_MANUAL_MAIN_WORK *)mtTaskGetTcbWork(tcb);
	
	// 終了処理
	if (main_work->flag & DMD_MANUAL_FLAG_EXIT) {
		// タスククリア
		DmManualExit();
		
		// イベント遷移用設定
		
	}
	
	// システム関連処理(サインアウト時はタイトルへ戻す)
	if (main_work->flag & DMD_MANUAL_FLAG_SIGN_OUT_EXIT
		&& !AoAccountIsCurrentEnable()) {
		
		main_work->proc_update = dmManualProcFadeOut;
		
		// サインアウト終了フラグOFF
		main_work->flag &= ~DMD_MANUAL_FLAG_SIGN_OUT_EXIT;
		
		// フェード終了		※問題あれば有効にする
//		IzFadeExit();
		
		if (dm_manual_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_TAKEOEVER
								, IZE_FADE_TYPE_BLACK_FADEOUT
								, DMD_MANUAL_FADEOUT_TIME
								, TRUE
								);
		}
		else {
			// フェードアウト開始
			IzFadeInitEasy(IZE_FADE_SET_TYPE_TAKEOEVER
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_MANUAL_FADEOUT_TIME
						   );
		}
		
		// ウインドウ遷移関連設定
		main_work->flag &= ~DMD_MANUAL_FLAG_DECIDE;
		main_work->flag &= ~DMD_MANUAL_FLAG_CANCEL;
		main_work->proc_input = NULL;
	}
	
	// 更新処理用プロシージャ
	if (main_work->proc_update) {
		main_work->proc_update(main_work);
	}
	
	// 描画設定プロシージャ
	if (main_work->proc_draw) {
		main_work->proc_draw(main_work);
	}

}


// ==========================================================================
// dmManualDest
/*!
	オンラインマニュアル画面終了処理
 */
// ==========================================================================
void dmManualDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	
}



// ==========================================================================
// dmManualProcInit
/*!
	テクスチャ構築待ち処理
 */
// ==========================================================================
void dmManualProcInit(DMS_MANUAL_MAIN_WORK *main_work)
{
	// 次へ遷移
	main_work->proc_update = dmManualProcCreateAct;
	
	// サインアウト終了フラグON
	main_work->flag |= DMD_MANUAL_FLAG_SIGN_OUT_EXIT;
}


// ==========================================================================
// dmManualProcCreateAct
/*!
	アクション生成処理

  	※cur_fileを設定する際は必ずcrsr_idxとcur_vrtcl_fileを設定して
  	それらの和を設定すること。
 */
// ==========================================================================
void dmManualProcCreateAct(DMS_MANUAL_MAIN_WORK *main_work)
{
	// ファイル選別
	const void *ama;
	AOS_TEXTURE *tex;
	
	// アクション構築
#if !_IPHONE	//当たり判定構築
	for (u32 i = 0; i <= ACT_ARROW_R; ++i) {
#else //!_IPHONE	//当たり判定構築
	for (u32 i = 0; i <= ACT_BACK_C; ++i) {
#endif //!_IPHONE	//当たり判定構築
		ama = dm_manual_ama[DME_MANUAL_DATA_TYPE_CMN_DATA];
		tex = &dm_manual_tex[DME_MANUAL_DATA_TYPE_CMN_DATA];
		
		// 構築
		AoActSetTexture(AoTexGetTexList(tex));
		main_work->act[i] = AoActCreate(ama, g_dm_act_id_tbl[i]);
	}
#if _IPHONE	//当たり判定構築
	for (u32 i = ACT_ARROW_L; i <= ACT_ARROW_R; ++i) {
		er::CTrgAoAction &trg = main_work->trg_btn[i - ACT_ARROW_L];
		new(&trg) er::CTrgAoAction();
		trg.Create(main_work->act[i]);
	}
	{
		er::CTrgAoAction &trg = main_work->trg_return;
		new(&trg) er::CTrgAoAction();
		trg.Create(main_work->act[ACT_BACK_C]);
	}
#endif //_IPHONE	//当たり判定構築
	
	for (u32 i = ACT_TEX_MODORU; i <= ACT_TEX_ASOBI; ++i) {
		ama = dm_manual_ama[DME_MANUAL_DATA_TYPE_LANG_DATA];
		tex = &dm_manual_tex[DME_MANUAL_DATA_TYPE_LANG_DATA];
		
		// 構築
		AoActSetTexture(AoTexGetTexList(tex));
		main_work->act[i] = AoActCreate(ama, g_dm_act_id_tbl[i]);
	}
	
	// イベント遷移
	main_work->proc_update = dmManualProcFadeIn;
	
	// テクスチャセットまで出来たので描画プロシージャを設定
	main_work->proc_draw = dmManualProcActDraw;
	
	// フェードイン開始
	if (dm_manual_is_pause_maingame) {
		IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
							, 0x7fff
							, IZD_FADE_DT_PRIO_DEF
							, IZD_FADE_DRAW_STATE_DEF
							, IZE_FADE_SET_TYPE_NORMAL
							, IZE_FADE_TYPE_BLACK_FADEIN
							, DMD_MANUAL_FADEIN_TIME
							, TRUE
							);
	}
	else {
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEIN
					   , DMD_MANUAL_FADEIN_TIME
					   );
	}
}



// ==========================================================================
// dmManualProcFadeIn
/*!
	オンラインマニュアル時のフェードイン中処理
 */
// ==========================================================================
void dmManualProcFadeIn(DMS_MANUAL_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
		IzFadeExit();
		
		main_work->proc_update = dmManualProcWaitInput;
		main_work->proc_input = dmManualInputProcMain;
	}
}



// ==========================================================================
// dmManualProcWaitInput
/*!
	オンラインマニュアル時の入力待ち中処理
 */
// ==========================================================================
void dmManualProcWaitInput(DMS_MANUAL_MAIN_WORK *main_work)
{
	// 入力処理
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}

#if _IPHONE
	const int c_act_id_table[] = {ACT_BACK_L, ACT_BACK_C, ACT_TEX_MODORU};
	for (int i = 0, max = arrayof(c_act_id_table); i < max; ++i) {
		AOS_ACTION *act = main_work->act[c_act_id_table[i]];
		float frame;
		if (main_work->trg_return.GetState(0)[er::CTrgState::EState::Up] && main_work->trg_return.GetState(0)[er::CTrgState::EState::Prev]) {
			frame = 2.0f;
		} else if (main_work->trg_return.GetState(0)[er::CTrgState::EState::On]) {
			frame = 1.0f;
		} else if (2.0f <= act->frame) {
			frame = act->frame;
		} else {
			frame = 0.0f;
		}
		AoActSetFrame(act, frame);
	}
#endif //_IPHONE

	// キャンセルフラグONならば
	if (main_work->flag & DMD_MANUAL_FLAG_CANCEL) {
		// タイトル側へ戻る(掃け演出開始)
		main_work->proc_update = dmManualProcFadeOut;

		if (dm_manual_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_BLACK_FADEOUT
								, DMD_MANUAL_FADEOUT_TIME
								, TRUE
								);
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_MANUAL_FADEOUT_TIME
						   );
		}
		
		
		if (!dm_manual_is_pause_maingame) {
			DmSoundPlaySE("Cancel");
		}
		else {
			GsSoundPlaySe("Cancel", main_work->se_handle);
		}
		
		// フラグOFF
		main_work->flag &= ~DMD_MANUAL_FLAG_DECIDE;
		main_work->flag &= ~DMD_MANUAL_FLAG_CANCEL;
		
		return;
	}
	
	// 次ページフラグONならば
	if (main_work->flag & DMD_MANUAL_FLAG_NEXT_PAGE) {
		// 次のページへ
		main_work->cur_disp_page++;
		
		if (main_work->cur_disp_page > DMD_MANUAL_PAGE_NO_DST - 1) {
			main_work->cur_disp_page = DMD_MANUAL_PAGE_NO_DST - 1;
		}
		else {
			if (!dm_manual_is_pause_maingame) {
				DmSoundPlaySE("Cursol");
			}
			else {
				GsSoundPlaySe("Cursol", main_work->se_handle);
			}
		}
		
		main_work->flag &= ~DMD_MANUAL_FLAG_NEXT_PAGE;
		
		return;
	}
	
	// 前ページフラグONならば
	if (main_work->flag & DMD_MANUAL_FLAG_PREV_PAGE) {
		// 前のページへ
		main_work->cur_disp_page--;
		
		if (main_work->cur_disp_page < DMD_MANUAL_PAGE_NO_SRC) {
			main_work->cur_disp_page = DMD_MANUAL_PAGE_NO_SRC;
		}
		else {
			if (!dm_manual_is_pause_maingame) {
				DmSoundPlaySE("Cursol");
			}
			else {
				GsSoundPlaySe("Cursol", main_work->se_handle);
			}
		}
		
		main_work->flag &= ~DMD_MANUAL_FLAG_PREV_PAGE;
		
		return;
	}
}




// ==========================================================================
// dmManualProcFadeOut
/*!
	フェードアウト中処理
 */
// ==========================================================================
void dmManualProcFadeOut(DMS_MANUAL_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
//		IzFadeExit();
		
		main_work->proc_update = dmManualProcStopDraw;
		main_work->proc_draw = NULL;
		
		if (main_work->se_handle) {
			GsSoundFreeSeHandle(main_work->se_handle);
			main_work->se_handle = NULL;
		}
	}
}



// ==========================================================================
// dmManualInputProcMain
/*!
	入力プロシージャ処理
 */
// ==========================================================================
void dmManualInputProcMain(DMS_MANUAL_MAIN_WORK *main_work)
{
	// キャンセル処理
#if !_IPHONE
	if (AoPadStand() & GSD_KEY_CANCEL) {
#else //!_IPHONE
	if (main_work->trg_return.GetState(0)[er::CTrgState::EState::Up] && main_work->trg_return.GetState(0)[er::CTrgState::EState::Prev]) {
#endif //!_IPHONE
		main_work->flag |= DMD_MANUAL_FLAG_CANCEL;

		return;
	}
	
#if !_IPHONE
	// 十字キー操作
	if (AoPadMRepeat() & GSD_KEY_RIGHT) {
		if (AoPadMStand() & GSD_KEY_RIGHT
#else //!_IPHONE
	if (main_work->trg_btn[1].GetState(0)[er::CTrgState::EState::Repeat]) {
		if (main_work->trg_btn[1].GetState(0)[er::CTrgState::EState::Down]
#endif //!_IPHONE
			|| main_work->cur_disp_page != DMD_MANUAL_PAGE_NO_DST - 1) {
			
			main_work->flag |= DMD_MANUAL_FLAG_NEXT_PAGE;
		}

		return;
	}
	
#if !_IPHONE
	else if (AoPadMRepeat() & GSD_KEY_LEFT) {
		if (AoPadMStand() & GSD_KEY_LEFT
#else //!_IPHONE
	else if (main_work->trg_btn[0].GetState(0)[er::CTrgState::EState::Repeat]) {
		if (main_work->trg_btn[0].GetState(0)[er::CTrgState::EState::Down]
#endif //!_IPHONE
			|| main_work->cur_disp_page != DMD_MANUAL_PAGE_NO_SRC) {
			
			main_work->flag |= DMD_MANUAL_FLAG_PREV_PAGE;
		}

		return;
	}
}




// ==========================================================================
// dmManualProcStopDraw
/*!
	描画停止処理
 */
// ==========================================================================
void dmManualProcStopDraw(DMS_MANUAL_MAIN_WORK *main_work)
{
	for (int i = 0; i < ACT_NUM; i++) {
		if (main_work->act[i]) {
			AoActDelete(main_work->act[i]);
			main_work->act[i] = NULL;
		}
	}
#if _IPHONE	//当たり判定解放
	//ボタントリガ
	for (int i = 0; i < arrayof(main_work->trg_btn); i++) {
		er::CTrgAoAction &trg = main_work->trg_btn[i];
		trg.Release();
		trg.~CTrgAoAction();
	}
	//戻るトリガ
	{
		er::CTrgAoAction &trg = main_work->trg_return;
		trg.Release();
		trg.~CTrgAoAction();
	}
#endif //_IPHONE	//当たり判定解放
	
	main_work->proc_update = NULL;
	main_work->flag |= DMD_MANUAL_FLAG_EXIT;
}




// ==========================================================================
// dmManualProcActDraw
/*!
	描画設定プロシージャ処理
 */
// ==========================================================================
void dmManualProcActDraw(DMS_MANUAL_MAIN_WORK *main_work)
{
	// 背景描画設定
	dmManualCommonBgDraw(main_work);
	
	// ページ切り替え部の表示設定
	dmManualPageDraw(main_work);
	
	// 共通描画処理は描画時は常に設定
	dmManualCommonDraw(main_work);
	
	// 描画タスク生成
	if (main_work->draw_state) {
		amDrawMakeTask(dmManualTaskDraw, (u16)0x8000, (u32)0);
	}
}



// ==========================================================================
// dmManualTaskDraw
/*!
	オンラインマニュアル画面の描画タスク
 */
// ==========================================================================
void dmManualTaskDraw(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	AoActDrawPre();
	amDrawExecCommand(dm_manual_draw_state);
	amDrawEndScene();
}



// ==========================================================================
// dmManualCommonBgDraw
/*!
	共通背景描画設定処理
 */
// ==========================================================================
void dmManualCommonBgDraw(DMS_MANUAL_MAIN_WORK *main_work)
{
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(0x1800);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&dm_manual_tex[0]));

	// 言語共通
	for (int i = ACT_BG04A; i <= ACT_BG02; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	
	for (int i = ACT_BG04A; i <= ACT_BG02; i++) {
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
// dmManualCommonDraw
/*!
	共通描画設定処理
 */
// ==========================================================================
void dmManualCommonDraw(DMS_MANUAL_MAIN_WORK *main_work)
{
	float digit_10 = 0.f;
	float digit_1 = 0.f;
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(0x3000);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&dm_manual_tex[0]));

	// 言語共通
#if !_IPHONE
	for (int i = ACT_OBI; i <= ACT_ARROW_R; i++) {
#else //!_IPHONE
	for (int i = ACT_NUM1; i <= ACT_ARROW_R; i++) {
#endif //!_IPHONE
		if (i == ACT_NUM1) {
			if (main_work->cur_disp_page >= 10 - 1) {
				AoActSortRegAction(main_work->act[i]);
			}
		}
		
		else if (i == ACT_ARROW_L
#if !_IPHONE
				 || i == ACT_ICON_PAGE03) {
#else //!_IPHONE
				) {
#endif //!_IPHONE
			if (main_work->cur_disp_page != DMD_MANUAL_PAGE_NO_SRC) {
				AoActSortRegAction(main_work->act[i]);
			}
		}
		
		else if (i == ACT_ARROW_R
#if !_IPHONE
				 || i == ACT_ICON_PAGE02) {
#else //!_IPHONE
				) {
#endif //!_IPHONE
			if (main_work->cur_disp_page != DMD_MANUAL_PAGE_NO_DST - 1) {
				AoActSortRegAction(main_work->act[i]);
			}
		}
		
		else {
			AoActSortRegAction(main_work->act[i]);
		}
	}
	
	// 言語別
	AoActSetTexture(AoTexGetTexList(&dm_manual_tex[1]));
	
	for (int i = ACT_TEX_MODORU; i < ACT_TEX_ASOBI; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	
#if _PS3
	if (main_work->cur_disp_page >= DMD_MANUAL_CAUTION_PAGE_NUM) {
		AoActSortRegAction(main_work->act[ACT_TEX_ASOBI]);
	}
#else
	if (main_work->cur_disp_page >= DMD_MANUAL_CAUTION_PAGE_NUM) {
		AoActSortRegAction(main_work->act[ACT_TEX_ASOBI]);
	}
#endif
	
#if _IPHONE
	//戻るボタン台紙S
	for (int i = ACT_BACK_L; i <= ACT_BACK_C; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
#endif //_IPHONE
	
#if !_IPHONE
	// ボタンの表示フレーム設定
	if (main_work->is_jp_region) {
		AoActSetFrame(main_work->act[ACT_BUT01], 0.f);
	}
	else {
		AoActSetFrame(main_work->act[ACT_BUT01], 1.f);
	}
#endif //!_IPHONE
	
	
	if (main_work->cur_disp_page != 0) {
		digit_10 = (float)(main_work->cur_disp_page / 10);
	}
	else {
		digit_10 = 0;
	}
	
	digit_1 = (float)(main_work->cur_disp_page % 10);
	
	if (main_work->cur_disp_page == 10 - 1) {
		AoActSetFrame(main_work->act[ACT_NUM1]
					  , 0.f);
		
		AoActSetFrame(main_work->act[ACT_NUM2]
					  , 0.f);
	}
	
	else if (main_work->cur_disp_page < 10) {
		AoActSetFrame(main_work->act[ACT_NUM1]
					  , 0.f);
		
		AoActSetFrame(main_work->act[ACT_NUM2]
					  , (float)main_work->cur_disp_page + 1);
	}
	else {
		AoActSetFrame(main_work->act[ACT_NUM1]
					  , digit_10);
		
		AoActSetFrame(main_work->act[ACT_NUM2]
					  , digit_1 + 1);
	}
	
	AoActSetFrame(main_work->act[ACT_NUM4]
				  , DMD_MANUAL_MAX_PAGE_DISP_10);
	AoActSetFrame(main_work->act[ACT_NUM5]
				  , DMD_MANUAL_MAX_PAGE_DISP_1);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&dm_manual_tex[0]));
	
#if !_IPHONE
	for (int i = ACT_OBI; i <= ACT_ARROW_R; i++) {
		AoActUpdate(main_work->act[i], 0.0f);
	}
#else //!_IPHONE
	for (int i = ACT_NUM1; i < ACT_ARROW_L; i++) {
		AoActUpdate(main_work->act[i], 0.0f);
	}
	for (int i = ACT_ARROW_L; i <= ACT_ARROW_R; i++) {
		AoActUpdate(main_work->act[i], 1.0f);
	}
#endif //!_IPHONE
#if !_IPHONE
	for (int i = ACT_BACK_L; i <= ACT_BACK_C; i++) {
		AoActUpdate(main_work->act[i], 0.0f);
	}
#else //!_IPHONE
	for (int i = ACT_BACK_L; i <= ACT_BACK_C; i++) {
		float update_frame = ((2.0f <= main_work->act[i]->frame)? 1.0f: 0.0f);
		AoActUpdate(main_work->act[i], update_frame);
	}
#endif //!_IPHONE
#if _IPHONE	//当たり判定処理
	for (int i = 0; i < arrayof(main_work->trg_btn); i++) {
		er::CTrgAoAction &trg = main_work->trg_btn[i];
		trg.Update();
	}
	{
		er::CTrgAoAction &trg = main_work->trg_return;
		trg.Update();
	}
#endif //_IPHONE	//当たり判定処理
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&dm_manual_tex[1]));
	
#if !_IPHONE
	for (int i = ACT_TEX_MODORU; i <= ACT_TEX_ASOBI; i++) {
		AoActUpdate(main_work->act[i], 0.0f);
	}
#else //!_IPHONE
	for (int i = ACT_TEX_MODORU; i < ACT_TEX_MODORU+1; i++) {
		float update_frame = ((2.0f <= main_work->act[i]->frame)? 1.0f: 0.0f);
		AoActUpdate(main_work->act[i], update_frame);
	}
	for (int i = ACT_TEX_MODORU+1; i <= ACT_TEX_ASOBI; i++) {
		AoActUpdate(main_work->act[i], 0.0f);
	}
#endif //!_IPHONE

	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();
}



// ==========================================================================
// dmManualPageDraw
/*!
	ページ別描画設定処理
 */
// ==========================================================================
void dmManualPageDraw(DMS_MANUAL_MAIN_WORK *main_work)
{
	int disp_cmn_act_start_page = 0;
	int disp_cmn_act_end_page = 0;
	
	int disp_lang_act_start_page = 0;
	int disp_lang_act_end_page = 0;
	
	int tmp_disp_page_no = 0;
	
	const void *ama = NULL;
	
#if _PS3
	float tmp_cmp_height = 0.f;
#endif
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(0x2000);
	
#if _PS3
	tmp_disp_page_no = main_work->cur_disp_page;
	
	// 共通表示アクションのID取得
	disp_cmn_act_start_page = dm_manual_disp_act_cmn_tbl[tmp_disp_page_no][0];
	disp_cmn_act_end_page = dm_manual_disp_act_cmn_tbl[tmp_disp_page_no][1];
	
	// 言語別表示アクションのID取得
	disp_lang_act_start_page = dm_manual_disp_act_lang_tbl[tmp_disp_page_no][0];
	disp_lang_act_end_page = dm_manual_disp_act_lang_tbl[tmp_disp_page_no][1];
#else
	tmp_disp_page_no = main_work->cur_disp_page;
	
	// 共通表示アクションのID取得
	disp_cmn_act_start_page = dm_manual_disp_act_cmn_tbl[tmp_disp_page_no][0];
	disp_cmn_act_end_page = dm_manual_disp_act_cmn_tbl[tmp_disp_page_no][1];
	
	// 言語別表示アクションのID取得
	disp_lang_act_start_page = dm_manual_disp_act_lang_tbl[tmp_disp_page_no][0];
	disp_lang_act_end_page = dm_manual_disp_act_lang_tbl[tmp_disp_page_no][1];
#endif
	
#if _IPHONE
	//頁切り替え判定
	bool is_change_page = (main_work->cur_disp_page_prev != main_work->cur_disp_page);
	if (is_change_page && (DMD_MANUAL_PAGE_NO_SRC <= main_work->cur_disp_page_prev)) {
		//頁切り替えが発生したら(かつ初回ではないなら)
		//1f前に表示していたアクションの破棄
		// 言語共通
		for (int i = dm_manual_disp_act_cmn_tbl[main_work->cur_disp_page_prev][0]; i <= dm_manual_disp_act_cmn_tbl[main_work->cur_disp_page_prev][1]; i++) {
			AoActDelete(main_work->act[i]);
			main_work->act[i] = NULL;
		}
		for (int i = dm_manual_disp_act_lang_tbl[main_work->cur_disp_page_prev][0]; i <= dm_manual_disp_act_lang_tbl[main_work->cur_disp_page_prev][1]; i++) {
			AoActDelete(main_work->act[i]);
			main_work->act[i] = NULL;
		}
	}
	//1f前のページ数を更新
	main_work->cur_disp_page_prev = main_work->cur_disp_page;
#endif //_IPHONE
	
	// アクション構築
	// 言語共通
	for (int i = disp_cmn_act_start_page; i <= disp_cmn_act_end_page; i++) {
#if _IPHONE
		if (is_change_page) {
#endif //_IPHONE
		ama = dm_manual_ama[DME_MANUAL_DATA_TYPE_CMN_DATA];
		AoActSetTexture(AoTexGetTexList(&dm_manual_tex[0]));
		
		main_work->act[i] = AoActCreate(ama, g_dm_act_id_tbl[i]);
#if _IPHONE
		}
#endif //_IPHONE
		
		AoActSortRegAction(main_work->act[i]);
	}
	
	
	// 言語別
	AoActSetTexture(AoTexGetTexList(&dm_manual_tex[1]));
	
	for (int i = disp_lang_act_start_page; i <= disp_lang_act_end_page; i++) {
#if _IPHONE
		if (is_change_page) {
#endif //_IPHONE
		ama = dm_manual_ama[DME_MANUAL_DATA_TYPE_LANG_DATA];
		
		main_work->act[i] = AoActCreate(ama, g_dm_act_id_tbl[i]);
#if _IPHONE
		}
#endif //_IPHONE
		
		AoActSortRegAction(main_work->act[i]);
	}
	
#if _PS3
#if defined(HOG_RGN_JP)
	
#if AMD_PS3_USE_LIBRESC
	tmp_cmp_height = _am_draw_video.resc_height;
#else
	tmp_cmp_height = _am_draw_video.disp_height;
#endif
	
	if (tmp_disp_page_no >= 0 && tmp_disp_page_no < 4) {
		if (_am_draw_video.wide_screen) {
			if (tmp_cmp_height <= 600) {
				for (int i = disp_cmn_act_start_page; i <= disp_cmn_act_end_page; i++) {
					if (i >= ACT_TAB_CAUTION_C && i <= ACT_TAB_CAUTION_R) {
						AoActSetFrame(main_work->act[i], 1.f);
					}
					else {
						AoActSetFrame(main_work->act[i], 0.f);
					}
				}
				
				for (int i = disp_lang_act_start_page; i <= disp_lang_act_end_page; i++) {
					AoActSetFrame(main_work->act[i], 0.f);
				}
			}
			else if (tmp_cmp_height <= 800) {
				for (int i = disp_cmn_act_start_page; i <= disp_cmn_act_end_page; i++) {
					AoActSetFrame(main_work->act[i], 1.f);
				}
				
				for (int i = disp_lang_act_start_page; i <= disp_lang_act_end_page; i++) {
					AoActSetFrame(main_work->act[i], 1.f);
				}
			}
			else {
				for (int i = disp_cmn_act_start_page; i <= disp_cmn_act_end_page; i++) {
					AoActSetFrame(main_work->act[i], 1.f);
				}
				
				for (int i = disp_lang_act_start_page; i <= disp_lang_act_end_page; i++) {
					AoActSetFrame(main_work->act[i], 2.f);
				}
			}
		}
		else {
			for (int i = disp_cmn_act_start_page; i <= disp_cmn_act_end_page; i++) {
				AoActSetFrame(main_work->act[i], 0.f);
			}
			
			for (int i = disp_lang_act_start_page; i <= disp_lang_act_end_page; i++) {
				AoActSetFrame(main_work->act[i], 0.f);
			}
		}
	}
#else
	if (tmp_disp_page_no == 0) {
		AoActSetFrame(main_work->act[ACT_CRILOGO], 2.f);
	}
#endif
#endif
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&dm_manual_tex[0]));
	
	for (int i = disp_cmn_act_start_page; i <= disp_cmn_act_end_page; i++) {
#if !_IPHONE
		AoActUpdate(main_work->act[i], 0.0f);
#else //!_IPHONE
		AoActUpdate(main_work->act[i], 1.0f);
#endif //!_IPHONE
	}
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&dm_manual_tex[1]));
	
	for (int i = disp_lang_act_start_page; i <= disp_lang_act_end_page; i++) {
#if !_IPHONE
		AoActUpdate(main_work->act[i], 0.0f);
#else //!_IPHONE
		AoActUpdate(main_work->act[i], 1.0f);
#endif //!_IPHONE
	}
	
	// ソート実行
	AoActSortExecute();
	
	// ソートバッファ描画
	AoActSortDraw();
	
	// ソートバッファ全解除
	AoActSortUnregAll();
	
	
	
#if !_IPHONE
	// 言語共通
	for (int i = disp_cmn_act_start_page; i <= disp_cmn_act_end_page; i++) {
		AoActDelete(main_work->act[i]);
		main_work->act[i] = NULL;
	}
	
	for (int i = disp_lang_act_start_page; i <= disp_lang_act_end_page; i++) {
		AoActDelete(main_work->act[i]);
		main_work->act[i] = NULL;
	}
#endif //!_IPHONE
}



// ==========================================================================
// dmManualIsTexLoad
/*!
	テクスチャ構築完了チェック処理
 */
// ==========================================================================
s32 dmManualIsTexLoad(void)
{
	
	for (int i = 0; i < DME_MANUAL_DATA_TYPE_MAX; i++) {
		if (!AoTexIsLoaded(&dm_manual_tex[i])) {
			// フラグ扱いでON
			return 0;
		}
	}

	return 1;
}


// ==========================================================================
// dmManualIsTexRelease
/*!
	テクスチャ解放完了チェック処理
 */
// ==========================================================================
s32 dmManualIsTexRelease(void)
{
	
	for (int i = 0; i < DME_MANUAL_DATA_TYPE_MAX; i++) {
		if (!AoTexIsReleased(&dm_manual_tex[i])) {
			// フラグ扱いでON
			return 0;
		}
	}

	return 1;
}




// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
