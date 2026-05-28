// ===========================================================================
/*!
	@file	dmSaveing.cpp
	@brief	デモ・セーブ画面

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: dmSave.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 *
 *
 */

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#include "../library/ao/include/aoTexture.h"
#include "../library/ao/include/aoAction.h"

#include "gs.h"
#include "gsMainSys.h"
#include "aoWinSys.h"
#include "akUtil.h"
#include "mtTask.h"
#include "gmTask.h"
#include "izFade.h"

#include "objObject.h"
#include "gmPlayer.h"

#include "dmCmnBackup.h"
#include "dmLoading.h"
#include "hgTrophy.h"

#include "gsSound.h"
#include "dmSound.h"

#include "dmSave.h"

// 共通データヘッダ
#if !_IPHONE
#include "common/ace/D_CMN_WIN.HMA"
#include "common/ace/D_CMN_MSG_JP.HMA"
#else
#include "ace/D_CMN_WIN.HMA"
#include "ace/D_CMN_MSG_JP.HMA"
#endif //!_IPHONE

// ----- Macros ------------------------------------------------（マクロ定義）

#define DMD_SAVE_TASK_PAUSELEVEL	(0x7fff)
#define DMD_SAVE_TASK_PRIO_MAIN		(0x2000)
#define DMD_SAVE_TASK_GROUP_MAIN	(0)

#define DMD_SAVE_FILE_PATH_NUM_MAX	(60)

#define DMD_SAVE_SIZE_WIDTH			(960.0f)
#define DMD_SAVE_SIZE_HEIGHT		(720.0f)
#define DMD_SAVE_SIZE_HALF_WIDTH	(480.0f)
#define DMD_SAVE_SIZE_HALF_HEIGHT	(360.0f)


// プライオリティ設定
#define DMD_SAVE_DRAW_PRIO_BG		(0x0100)
#define DMD_SAVE_DRAW_PRIO_WIN		(0xEFFF)//(0x7fff)
#define DMD_SAVE_DRAW_PRIO_WIN_FIX	(0xEFFF)//(0x7fff)

#define DMD_SAVE_DRAW_TASK_PRIO		(0xEFFF)//(0x7fff)

// 表示関連
#define DMD_SAVE_DRAW_STATE_ID		(100)

// ウインドウ関連
#define DMD_SAVE_WINDOW_SIZE_W		(380.f)
#define DMD_SAVE_WINDOW_SIZE_H		(180.f)
#define DMD_SAVE_WIN_DEF_RATE		(1.0f)

#if _PS3
#define DMD_SAVE_LOAD_MIN_TIME		(0)
#define DMD_SAVE_SAVE_MIN_TIME		(0)
#elif _XBOX
#define DMD_SAVE_LOAD_MIN_TIME		(60)
#define DMD_SAVE_SAVE_MIN_TIME		(60)
#else
#define DMD_SAVE_LOAD_MIN_TIME		(60)
#define DMD_SAVE_SAVE_MIN_TIME		(60)
#endif

// フェード関連
#define DMD_SAVE_FADEIN_TIME		(32.0f)
#define DMD_SAVE_FADEOUT_TIME		(32.0f)

#define DMD_SAVE_BGM_FADEIN_TIME	(32)
#define DMD_SAVE_BGM_FADEOUT_TIME	(32)

#define DMD_SAVE_SE_END_WAIT_TIME	(60)

// 演出関連
#define DMD_SAVE_WIN_EFCT_TIME		(8)

// フラグ関連
#define DMD_SAVE_FLAG_EXIT					(1 << 0)		//!< 終了フラグ
#define DMD_SAVE_FLAG_CANCEL				(1 << 1)		//!< キャンセル
#define DMD_SAVE_FLAG_DECIDE				(1 << 2)		//!< 決定フラグ
#define DMD_SAVE_FLAG_DISP_MENU				(1 << 3)
#define DMD_SAVE_FLAG_WIN_EFCT_END			(1 << 4)
#define DMD_SAVE_FLAG_WIN_MSG_END			(1 << 5)

#define DMD_SAVE_FLAG_SIGN_OUT_EXIT			(1 << 31)

// アクション表示フラグ関連
#define DMD_SAVE_DISP_FLAG_WIN_ACT			(1 << 0)
#define DMD_SAVE_DISP_FLAG_SAVE_MENU		(1 << 1)
#define DMD_SAVE_DISP_FLAG_SAVE_VIEW		(1 << 2)
#define DMD_SAVE_DISP_FLAG_MENU_TAB			(1 << 3)
#define DMD_SAVE_DISP_FLAG_SAVE_LIST		(1 << 4)
#define DMD_SAVE_DISP_FLAG_NOW_UPDATE		(1 << 5)


#if _WII
#define DMD_SAVE_DISP_SCALE_TEXT			(1.4f)
#elif _IPHONE
#define DMD_SAVE_DISP_SCALE_TEXT			(1.5f * 1.125f)
#endif



// ----- Macro Functions -----------------------------------（処理マクロ定義）


// ----- Definitions -------------------------------------------（定数の宣言）

//! アクションテーブル
typedef enum tag_DME_SAVE_ACT
{
	// メニュー共通データ
	ACT_WIN_LINE = 0,		//!< 
	
	ACT_TEX_WINTITLE,		//!< 
	ACT_TEX_MSG1,			//!< 
	ACT_TEX_MSG2,			//!< 
	ACT_TEX_MSG3,			//!<
	
	ACT_TEX_OK,				//!< 
	
	ACT_NUM,
	
	ACT_NONE
} DME_SAVE_ACT;



typedef struct tag_DMS_SAVE_MAIN_WORK	DMS_SAVE_MAIN_WORK;

//! メインタスクワーク
struct tag_DMS_SAVE_MAIN_WORK {
	
	AMS_FS			*arc_cmn_amb_fs[2];					//!< 共通アーカイブAMBファイル
	void			*arc_cmn_amb[2];					//!< 共通アーカイブAMBファイル
	void			*cmn_ama[2];						//!< AMAファイル
	void			*cmn_amb[2];						//!< AMBファイル
	AOS_TEXTURE		cmn_tex[2];							//!< メニュー共通テクスチャ
	
	// メニュー用アクション
	AOS_ACTION 		*act[ACT_NUM];
	
	// プロシージャ設定変数
	void (*proc_input)(DMS_SAVE_MAIN_WORK *);			//!< メイン入力処理関数
	void (*proc_menu_update)(DMS_SAVE_MAIN_WORK *);		//!< メインプロシージャ
	void (*proc_draw)(void);			//!< 描画用プロシージャ

	// フラグ関連
	u32	flag;											//!< 汎用フラグ
	u32 announce_flag;									//!< 0ならアナウンスなし、それ以外はフラグがあるだけ表示する
	u32 disp_flag;										//!< 
	
	// 状態変数
	s32 state;											//!< メニューかセーブ画面か
	
	// タイマー関連
	s32 timer;										//!< 汎用タイマー
	s32 win_timer;									//!< ウインドウ演出用タイマー
	
	// WINDOW専用(Wiiのみ？)
	float win_act_pos[5][2];							//!< 
	float win_size_rate[2];								//!< 
	s32 win_mode;										//!< 
	s32 win_cur_slct;									//!< ウインドウでの現在の選択項目
	
	u32 draw_state;
	
	// メインゲーム中用のサウンドSCBファイルポインタ
	GSS_SND_SCB *bgm_scb;
};


//! 管理構造体
typedef struct tag_DMS_SAVE_MGR {
	MTS_TASK_TCB *tcb;	//!< TCB
} DMS_SAVE_MGR;



// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static void dmSaveInit(void);
static void dmSaveProcMain(MTS_TASK_TCB *tcb);
static void dmSaveDest(MTS_TASK_TCB *tcb);

// 初期化設定関連

static void dmSaveLoadFontData(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveIsLoadFontData(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveLoadRequest(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveProcLoadWait(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveProcTexBuildWait(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveProcCreateAct(DMS_SAVE_MAIN_WORK *main_work);

// メニュー用プロシージャ
//static void dmSaveProcFadeIn(DMS_SAVE_MAIN_WORK *main_work);
//static void dmSaveProcFadeOut(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveProcStopDraw(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveProcDataRelease(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveProcFinish(DMS_SAVE_MAIN_WORK *main_work);

// ウインドウ用プロシージャ
static void dmSaveProcWindowNodispIdle(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveProcWindowOpenWaitIdle(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveProcWindowOpenWaitLoadIdle(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveProcWindowOpenEfct(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveProcWindowAnnounceIdle(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveProcWindowCloseEfct(DMS_SAVE_MAIN_WORK *main_work);

static void dmSaveProcWaitSeStop(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveProcWaitLoadEnd(DMS_SAVE_MAIN_WORK *main_work);

// 入力処理用プロシージャ
static void dmSaveInputProcWindow(DMS_SAVE_MAIN_WORK *main_work);

// 描画関連処理
//static void dmSaveWinSelectDraw(void);
static void dmSaveTaskDraw(AMS_TCB* tcb);

static void dmSaveSetWinOpenEfct(DMS_SAVE_MAIN_WORK *main_work);
static void dmSaveSetWinCloseEfct(DMS_SAVE_MAIN_WORK *main_work);

static s32 dmSaveIsDataLoad(DMS_SAVE_MAIN_WORK *main_work);
static s32 dmSaveIsTexLoad(DMS_SAVE_MAIN_WORK *main_work);
static s32 dmSaveIsTexRelease(DMS_SAVE_MAIN_WORK *main_work);

static void dmSaveSetSysDataForBackup(void);
static BOOL dmSaveIsSaveNecessary(void);


// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ----- Static Variables --------------------（スタティック変数の定義：局所）
// 各国別メニュー共通AMBファイルパステーブル
const static char *dm_save_menu_cmn_lng_amb_name_tbl[GSD_LANGUAGE_NUM] = {
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_JP.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_US.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_FR.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_IT.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_GE.AMB",
	GSS_BASE_PATH"DEMO/CMN/D_CMN_MSG_SP.AMB",
};


const static float dm_save_win_act_pos_tbl[ACT_NUM][2] = {
	{DMD_SAVE_SIZE_HALF_WIDTH + 42.f, 280.0f},	// ウインドウ内のライン
	{DMD_SAVE_SIZE_HALF_WIDTH - 80.f, 274.0f},	// タイトルテキスト
	{DMD_SAVE_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
	{DMD_SAVE_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
	{DMD_SAVE_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
	{DMD_SAVE_SIZE_HALF_WIDTH, 420.0f},			// OKテキスト
};


const static int dm_save_win_size_y_tbl[GSD_LANGUAGE_NUM] = {
	0,
	32,
	72,
	72,
	104,
	72,
};


// アクションIDテーブル(初期状態)
#if 1
const static u32 g_dm_act_id_tbl[ACT_NUM] = {
	// メニュー共通データ
#if !_IPHONE
	IDA_D_CMN_WIN_ACT_WIN_LINE,				//!< 
#else //!_IPHONE
	0,				//!< 
#endif //!_IPHONE
	
#if !_IPHONE
	IDA_D_CMN_MSG_JP_ACT_TEX_WINTITLE,		//!< 
#else //!_IPHONE
	0,		//!< 
#endif //!_IPHONE
	IDA_D_CMN_MSG_JP_ACT_TEX_MSG01,
	IDA_D_CMN_MSG_JP_ACT_TEX_MSG02,
	IDA_D_CMN_MSG_JP_ACT_TEX_MSG03,
	
#if !_IPHONE
	IDA_D_CMN_MSG_JP_ACT_TEX_OK,
#else //!_IPHONE
	0,		//!< 
#endif //!_IPHONE
};
#endif


//管理情報
static DMS_SAVE_MGR dm_save_mgr;
static DMS_SAVE_MGR *dm_save_mgr_p = NULL;

static u32 dm_save_draw_state = 0;
static u32 dm_save_msg_flag = 0;
static BOOL dm_save_first_save = FALSE;

static AOS_TEXTURE *dm_save_cmn_tex[2];		//!< メニュー共通テクスチャ
static AOS_ACTION *dm_save_act[ACT_NUM];	//!< 

static u32 dm_save_disp_flag;				//!< 
static u32 dm_save_is_draw_state;
static float dm_save_win_size_rate[2];		//!< 
static s32 dm_save_win_mode;				//!< 

static BOOL dm_save_draw_reserve = FALSE;
static BOOL dm_save_is_task_draw = FALSE;

static BOOL dm_save_is_snd_build = FALSE;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// DmSaveStart
/*!
	セーブ開始処理
  
  disp_flag : 表示させたいメッセージ(処理含む)を引数に設定
 */
// ==========================================================================
void DmSaveStart(u32 disp_flag, BOOL is_first_save, BOOL is_task_draw)
{
	s16 tmp_cur_evt = 0;
	
	// メインゲーム中で呼び出された場合、セーブ処理を行わない
	tmp_cur_evt = SyGetEvtInfo()->cur_evt_id;
	
	if (tmp_cur_evt == GSD_EVT_ID_MAINGAME
		|| tmp_cur_evt == GSD_EVT_ID_SPSTAGE_BRANCH
		|| tmp_cur_evt == GSD_EVT_ID_ENDING) {
		return;
	}
	
	// 体験版の場合、セーブしない
	if (disp_flag & (1 << DME_SAVE_WIN_NOW_SAVING)
		&& GsTrialIsTrial()) {
		return;
	}
	
	// 管理情報初期化設定
	amZeroMemory(&dm_save_mgr, sizeof(DMS_SAVE_MGR));
	dm_save_mgr_p = &dm_save_mgr;
	
	// メッセージフラグを取得
	dm_save_msg_flag = disp_flag;
	
	// セーブ設定を取得
	dm_save_first_save = is_first_save;
	
	// 
	dm_save_is_task_draw = is_task_draw;
	
	dm_save_is_snd_build = FALSE;
	
	dmSaveInit();
}



// ==========================================================================
// DmSaveAttenMsgStart
/*!
	注意事項メッセージ開始処理
 */
// ==========================================================================
void DmSaveAttenMsgStart()
{
	s16 tmp_cur_evt = 0;
	
	// メインゲーム中で呼び出された場合、セーブ処理を行わない
	tmp_cur_evt = SyGetEvtInfo()->cur_evt_id;
	
	// 管理情報初期化設定
	amZeroMemory(&dm_save_mgr, sizeof(DMS_SAVE_MGR));
	dm_save_mgr_p = &dm_save_mgr;
	
	// メッセージフラグを取得
	dm_save_msg_flag = 1 << DME_SAVE_WIN_ATTENTION_SAVE;
	
	// セーブ設定を取得
	dm_save_first_save = FALSE;
	
	dm_save_is_task_draw = FALSE;
	
	dm_save_is_snd_build = TRUE;
	
	dmSaveInit();
}



// ==========================================================================
// DmSaveMenuStart
/*!
	メニュ汎用セーブ開始処理
  
  disp_flag : 表示させたいメッセージ(処理含む)を引数に設定
 */
// ==========================================================================
void DmSaveMenuStart(BOOL is_task_draw, BOOL is_snd_build)
{
	s16 tmp_cur_evt = 0;
	
	// メインゲーム中で呼び出された場合、セーブ処理を行わない
	tmp_cur_evt = SyGetEvtInfo()->cur_evt_id;
	
	if (tmp_cur_evt == GSD_EVT_ID_MAINGAME
		|| tmp_cur_evt == GSD_EVT_ID_SPSTAGE_BRANCH
		|| tmp_cur_evt == GSD_EVT_ID_ENDING) {
		return;
	}
	
	// サインアウトしている場合はセーブしない
	if (!AoAccountIsCurrentEnable()) {
		return;
	}
	
#if _XBOX
	// 初めに製品版へ切り替え後の初セーブの場合、通常セーブを行う
	if (GsTrialIsTrial() && !GsTrialIsTrialDirect()) {
		GsTrialApplyDirect(); // GsTrialIsTrial() = TRUE -> FALSE
		DmSaveStart(1 << DME_SAVE_WIN_TRIAL_OUT_SAVE, FALSE, TRUE); // 初回セーブと同じ処理
		return;
	}
#endif
	
	// 現在のGSに保存されているシステムデータをバックアップに設定
	dmSaveSetSysDataForBackup();
	
	// 体験版の場合、セーブしない
	if (GsTrialIsTrial()) {
		return;
	}
	
	// セーブデータに差異がない場合、セーブしない
	if (dmSaveIsSaveNecessary()) {
		return;
	}
	
	// 管理情報初期化設定
	amZeroMemory(&dm_save_mgr, sizeof(DMS_SAVE_MGR));
	dm_save_mgr_p = &dm_save_mgr;
	
	// メッセージフラグを取得
	dm_save_msg_flag = 1 << DME_SAVE_WIN_NOW_SAVING;
	
	// セーブ設定を取得
	dm_save_first_save = FALSE;
	
	dm_save_is_task_draw = is_task_draw;
	
	dm_save_is_snd_build = is_snd_build;
	
	dmSaveInit();
}



// ==========================================================================
// DmSaveIsExit
/*!
	セーブ終了チェック処理
 */
// ==========================================================================
BOOL DmSaveIsExit(void)
{
	if (dm_save_mgr_p != NULL) {
		if (dm_save_mgr_p->tcb == NULL) {
			return TRUE;
		}
	}
	else {
		return TRUE;
	}

	return FALSE;
}



// ==========================================================================
// DmSaveIsDraw
/*!
	セーブウインドウ描画チェック処理
 */
// ==========================================================================
BOOL DmSaveIsDraw(void)
{
	return dm_save_draw_reserve;
}



// ----- Static Functions --------------------（スタティック関数の定義：局所）
// ==========================================================================
// dmSaveInit
/*!
	セーブ画面初期化処理
 */
// ==========================================================================
void dmSaveInit(void)
{
	DMS_SAVE_MAIN_WORK	*main_work;

	// メインタスク作成
	dm_save_mgr_p->tcb = MTM_TASK_MAKE_TCB(dmSaveProcMain
											, dmSaveDest
											, 0
											, DMD_SAVE_TASK_PAUSELEVEL
											, DMD_SAVE_TASK_PRIO_MAIN
											, DMD_SAVE_TASK_GROUP_MAIN
											, sizeof(DMS_SAVE_MAIN_WORK)
											, "SAVE_TASK"
											);
	
	// ワーク初期化
	main_work = (DMS_SAVE_MAIN_WORK *)mtTaskGetTcbWork(dm_save_mgr_p->tcb);
	
	// グローバル変数初期化
	dm_save_disp_flag = 0;
	dm_save_is_draw_state = 0;
	dm_save_win_mode = 0;
	dm_save_draw_reserve = FALSE;
	
	for (int i = 0; i < 2; i++) {
		dm_save_win_size_rate[i] = 0.f;
		dm_save_cmn_tex[i] = NULL;
	}
	
	for (int i = 0; i < ACT_NUM; i++) {
		dm_save_act[i] = NULL;
	}
	
	// 表示メッセージフラグ設定
	main_work->announce_flag = dm_save_msg_flag;
	
	// AO描画方法設定
	main_work->draw_state = (u32)AoActSysGetDrawStateEnable();
	
	if (main_work->draw_state) {
		dm_save_draw_state = AoActSysGetDrawState();
	}
	
	else {
		dm_save_draw_state = 0;
	}
	
	for (int i = 0; i < 2; i++) {
		dm_save_cmn_tex[i] = NULL;
	}
	
	dm_save_is_draw_state = main_work->draw_state;
	
	// プロシージャ設定
	main_work->proc_menu_update = dmSaveLoadFontData;
}



// ==========================================================================
// dmSaveSetSysDataForBackup
/*!
	セーブする直前のシステムデータをバックアップに設定する処理
 */
// ==========================================================================
void dmSaveSetSysDataForBackup(void)
{
	GSS_MAIN_SYS_INFO	*gs_main = GsGetMainSysInfo();
	
	// システムデータインスタンス作成
	gs::backup::SSystem &sys_data
		= gs::backup::SSystem::CreateInstance();
	
	
	// 累計敵討伐数設定
	sys_data.SetKilled(gs_main->ene_kill_count);
	
	// 残機数設定
	sys_data.SetPlayerStock(gs_main->rest_player_num);
}



// ==========================================================================
// dmSaveIsSaveNecessary
/*!
	セーブが必要かどうかを返す処理
 */
// ==========================================================================
BOOL dmSaveIsSaveNecessary(void)
{
	GSS_MAIN_SYS_INFO *gs_main = GsGetMainSysInfo();
	
	// セーブ有効フラグがOFFの場合、TRUE(セーブを行わない=データが同値)
	if (!gs_main->is_save_run) {
		return TRUE;
	}
	
	// システムデータインスタンス作成
	gs::backup::SSystem &sys_data
		= gs::backup::SSystem::CreateInstance();
	
	
	// 通常ステージデータインスタンス作成
	gs::backup::SStage &stg_data
		= gs::backup::SStage::CreateInstance();
	
	
	// スペシャルステージデータインスタンス作成
	gs::backup::SSpecial &spe_data
		= gs::backup::SSpecial::CreateInstance();
	
	
	// オプションデータインスタンス取得
	gs::backup::SOption &opt_data
		= gs::backup::SOption::CreateInstance();
	
	
	// システムデータ差異チェック
	
	
	// 比較用システムデータインスタンス取得
	gs::backup::SSystem &cmp_sys_data
		= gs_main->cmp_backup.GetSystem();
	
	
	// 比較用通常ステージデータインスタンス取得
	gs::backup::SStage &cmp_stg_data
		= gs_main->cmp_backup.GetStage();
	
	
	// 比較用スペシャルステージデータインスタンス取得
	gs::backup::SSpecial &cmp_spe_data
		= gs_main->cmp_backup.GetSpecial();
	
	
	// 比較用オプションデータインスタンス取得
	gs::backup::SOption &cmp_opt_data
		= gs_main->cmp_backup.GetOption();
	
	
	// システムデータ比較
	// (システムデータは一部比較しないデータがあるため、一つ一つ比較)
	
	// 残機比較
	if (sys_data.GetPlayerStock() != cmp_sys_data.GetPlayerStock()) {
		return FALSE;
	}
	
	// 累計エネミー撃退数比較
	if (sys_data.GetKilled() != cmp_sys_data.GetKilled()) {
		return FALSE;
	}
	
	// クリア回数比較
	if (sys_data.GetClearCount() != cmp_sys_data.GetClearCount()) {
		return FALSE;
	}
	
	// アナウンス比較
	for (int i = 0; i < 7; ++i) {
		gs::backup::SSystem::EAnnounce::Type msg_type
			= gs::backup::SSystem::EAnnounce::OpenZoneSelect;
		
		msg_type = (gs::backup::SSystem::EAnnounce::Type)i;
		
		if (sys_data.IsAnnounce(msg_type) != cmp_sys_data.IsAnnounce(msg_type)) {
			return FALSE;
		}
	}
	
#if _WII
	// 最後にクリアしたACT比較
	if (sys_data.GetLastClearAct() != cmp_sys_data.GetLastClearAct()) {
		return FALSE;
	}
	
	// DWCユーザーデータ比較
	if (memcmp((const void *)&sys_data.GetDwcUserData()
			   , (const void *)&cmp_sys_data.GetDwcUserData()
			   , sizeof(DWCUserData)) != 0) {
		return FALSE;
	}
#endif
	
	// ステージデータ比較
	if (memcmp((const void *)&cmp_stg_data
			   , (const void *)&stg_data
			   , sizeof(gs::backup::SStage)) != 0) {
		return FALSE;
	}
	
	// スペステデータ比較
	if (memcmp((const void *)&cmp_spe_data
			   , (const void *)&spe_data
			   , sizeof(gs::backup::SSpecial)) != 0) {
		return FALSE;
	}
	
	// オプションデータ比較
	if (memcmp((const void *)&cmp_opt_data
			   , (const void *)&opt_data
			   , sizeof(gs::backup::SOption)) != 0) {
		return FALSE;
	}
	
	
	return TRUE;
}



// ==========================================================================
// dmSaveProcMain
/*!
	セーブ画面メインプロシージャ処理
 */
// ==========================================================================
void dmSaveProcMain(MTS_TASK_TCB *tcb)
{
	DMS_SAVE_MAIN_WORK	*main_work;

	// ワーク取得
	main_work = (DMS_SAVE_MAIN_WORK *)mtTaskGetTcbWork(tcb);

	// 終了処理
	if (main_work->flag & DMD_SAVE_FLAG_EXIT) {
		// タスククリア
		mtTaskClearTcb(tcb);
		
		// グローバル変数初期化
		dm_save_disp_flag = 0;
		dm_save_is_draw_state = 0;
		dm_save_win_mode = 0;
		dm_save_is_task_draw = FALSE;
		
		for (int i = 0; i < 2; i++) {
			dm_save_win_size_rate[i] = 0.f;
		}
		
		dm_save_mgr_p = NULL;
	}
	
	// サインアウト判定
#if 1
	if (main_work->flag & DMD_SAVE_FLAG_SIGN_OUT_EXIT
		&& !AoAccountIsCurrentEnable()) {
		
		// 終了処理へ遷移
		main_work->proc_menu_update = dmSaveProcStopDraw;
		main_work->proc_input = NULL;
		main_work->proc_draw = NULL;
		dm_save_draw_reserve = FALSE;
		
		main_work->flag &= ~DMD_SAVE_FLAG_SIGN_OUT_EXIT;
		
		return;
	}
#endif

	// メニュー処理用プロシージャ
	if (main_work->proc_menu_update) {
		main_work->proc_menu_update(main_work);
	}

	// 描画設定プロシージャ
	if (main_work->proc_draw
		&& !(AoSysMsgIsShow())) {
		main_work->proc_draw();
	}

}


// ==========================================================================
// dmSaveDest
/*!
	セーブ画面終了処理
 */
// ==========================================================================
void dmSaveDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	
}




// ==========================================================================
// dmSaveLoadFontData
/*!
	フォントデータ読み込みリクエスト処理
 */
// ==========================================================================
void dmSaveLoadFontData(DMS_SAVE_MAIN_WORK *main_work)
{
	s16 tmp_cur_evt_id = SyGetEvtInfo()->cur_evt_id;
	
	// gsFont構築
	if (tmp_cur_evt_id == GSD_EVT_ID_STAFFROLL) {
		GsFontBuild(FALSE);
	}
	else {
		GsFontBuild();
	}
	
	main_work->proc_menu_update = dmSaveIsLoadFontData;
}



// ==========================================================================
// dmSaveIsLoadFontData
/*!
	フォントデータ読み込み終了チェック処理
 */
// ==========================================================================
void dmSaveIsLoadFontData(DMS_SAVE_MAIN_WORK *main_work)
{
	// gsFont構築
	if (GsFontIsBuilded()) {
		main_work->proc_menu_update = dmSaveLoadRequest;
		
		return;
	}
}



// ==========================================================================
// dmSaveLoadRequest
/*!
	ファイル読み込みリクエスト処理
 */
// ==========================================================================
void dmSaveLoadRequest(DMS_SAVE_MAIN_WORK *main_work)
{
	// メニュー共通データ読み込み
	main_work->arc_cmn_amb_fs[0] = amFsReadBackground(GSS_BASE_PATH "DEMO/CMN/D_CMN_WIN.AMB");
	
	main_work->arc_cmn_amb_fs[1] = amFsReadBackground((char *)dm_save_menu_cmn_lng_amb_name_tbl[GsEnvGetLanguage()]);
	
	// 次へ遷移
	main_work->proc_menu_update = dmSaveProcLoadWait;
}


// ==========================================================================
// dmSaveProcLoadWait
/*!
	ファイル読み込み待ち処理
 */
// ==========================================================================
void dmSaveProcLoadWait(DMS_SAVE_MAIN_WORK *main_work)
{
	// ファイル読み込み完了待ち
	if (dmSaveIsDataLoad(main_work)) {		// ファイル読込み完了チェック関数にする
		// メニュー共通データ
		for (int i = 0; i < 2; i++) {
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
		
		// サウンド構築が必要ならば
		if (dm_save_is_snd_build) {
			DmSoundBuild();
		}
		
		// 次へ遷移
		main_work->proc_menu_update = dmSaveProcTexBuildWait;
	}
}


// ==========================================================================
// dmSaveProcTexBuildWait
/*!
	テクスチャ構築待ち処理
 */
// ==========================================================================
void dmSaveProcTexBuildWait(DMS_SAVE_MAIN_WORK *main_work)
{
	// テクスチャ構築完了判定
	if (dmSaveIsTexLoad(main_work) == 1) {
		
		for (int i = 0; i < 2; i++) {
			dm_save_cmn_tex[i] = &main_work->cmn_tex[i];
		}
		
		// 次へ遷移
		main_work->proc_menu_update = dmSaveProcCreateAct;
		
	}
}



// ==========================================================================
// dmSaveProcCreateAct
/*!
	アクション生成処理
 */
// ==========================================================================
void dmSaveProcCreateAct(DMS_SAVE_MAIN_WORK *main_work)
{
	// アクション構築
	for (u32 i = 0; i < ACT_NUM; ++i) {
		// ファイル選別
		const void *ama;
		AOS_TEXTURE *tex;
		
		if (i >= ACT_TEX_WINTITLE) {
			ama = main_work->cmn_ama[1];
			tex = &main_work->cmn_tex[1];
		}
		else  {
			ama = main_work->cmn_ama[0];
			tex = &main_work->cmn_tex[0];
		}
		
		// 構築
		AoActSetTexture(AoTexGetTexList(tex));
		main_work->act[i] = AoActCreate(ama, g_dm_act_id_tbl[i]);
		
		dm_save_act[i] = main_work->act[i];
	}
	
	if (dm_save_msg_flag & (1 << DME_SAVE_WIN_NOW_SAVING)) {
		main_work->flag |= DMD_SAVE_FLAG_SIGN_OUT_EXIT;
	}
	
	main_work->proc_menu_update = dmSaveProcWindowNodispIdle;
}



// ==========================================================================
// dmSaveProcWindowNodispIdle
/*!
	ウインドウ非表示待ち中処理
 */
// ==========================================================================
void dmSaveProcWindowNodispIdle(DMS_SAVE_MAIN_WORK *main_work)
{
	// メニュー遷移フラグONならば
	if (main_work->flag & DMD_SAVE_FLAG_DISP_MENU
		|| main_work->announce_flag) {
		
		// ウインドウ演出用タイマー初期化
		main_work->win_timer = 0;
		
		// ウインドウ選択変数設定
		for (u32 i = DME_SAVE_WIN_DATA_LOADING; i < DME_SAVE_WIN_NUM; i++) {
			if (main_work->announce_flag & 1 << i) {
				dm_save_win_mode = 
				main_work->win_mode = (s32)i;
				break;
			}
		}
		
		// セーブメッセージの場合、セーブ開始
		if (main_work->win_mode == DME_SAVE_WIN_DATA_LOADING) {
			DmCmnBackupLoad();
			main_work->proc_input = NULL;
			main_work->proc_menu_update = dmSaveProcWindowOpenWaitLoadIdle;
			
		}
		
		else if (main_work->win_mode == DME_SAVE_WIN_ATTENTION_SAVE) {
			main_work->proc_input = dmSaveInputProcWindow;
			main_work->proc_menu_update = dmSaveProcWindowOpenEfct;
			main_work->proc_draw = DmSaveWinSelectDraw;
			dm_save_draw_reserve = TRUE;
			
			DmSoundPlaySE("Window");
		}
		
		else if (main_work->win_mode == DME_SAVE_WIN_NOW_SAVING) {
			DmCmnBackupSave(dm_save_first_save, FALSE);
			main_work->proc_input = NULL;
			main_work->proc_menu_update = dmSaveProcWindowOpenWaitIdle;
			main_work->proc_draw = NULL;
			dm_save_draw_reserve = FALSE;
		}
		
		else if (main_work->win_mode == DME_SAVE_WIN_TRIAL_OUT_SAVE) {
			DmCmnBackupSave(dm_save_first_save, TRUE);
			main_work->proc_input = NULL;
			main_work->proc_menu_update = dmSaveProcWindowOpenWaitIdle;
			main_work->proc_draw = NULL;
			dm_save_draw_reserve = FALSE;
		}
		
		// ウインドウ演出中フラグON
		main_work->flag &= ~DMD_SAVE_FLAG_DISP_MENU;
	}
	
	else {
		main_work->proc_menu_update = dmSaveProcStopDraw;
		dm_save_draw_reserve = FALSE;
	}
}



// ==========================================================================
// dmSaveProcWindowOpenWaitLoadIdle
/*!
	ウインドウオープン待ち中処理(データロード版)
 */
// ==========================================================================
void dmSaveProcWindowOpenWaitLoadIdle(DMS_SAVE_MAIN_WORK *main_work)
{
	// セーブ処理終了いていなければ
#if !_IPHONE
	if (!DmCmnBackupIsLoadFinished()) {
#else //!_IPHONE
	if (true) {
		//セーブメッセージを常に表示する
#endif //!_IPHONE
		// セーブ容量の空きチェック終了時
		if (AoStorageIsExecuteReal()) {
			// ここでウインドウを開くプロシージャへ
			main_work->proc_menu_update = dmSaveProcWindowOpenEfct;
			
#if _PS3
			main_work->proc_draw = NULL;
#else
			main_work->proc_draw = DmSaveWinSelectDraw;
			DmSoundPlaySE("Window");
#endif
			dm_save_draw_reserve = TRUE;
		}
		// セーブ容量の空きチェックが終っていないとき
		else {
			// 空処理
		}
	}
	
	else {
		// ここで終了処理までもってく(ウインドウは出さないまま終る)
		main_work->proc_menu_update = dmSaveProcStopDraw;
		dm_save_draw_reserve = FALSE;
	}
}



// ==========================================================================
// dmSaveProcWindowOpenWaitIdle
/*!
	ウインドウオープン待ち中処理
 */
// ==========================================================================
void dmSaveProcWindowOpenWaitIdle(DMS_SAVE_MAIN_WORK *main_work)
{
	// セーブ処理終了いていなければ
#if !_IPHONE
	if (!DmCmnBackupIsSaveFinished()) {
#else //!_IPHONE
	if (true) {
		//セーブメッセージを常に表示する
#endif //!_IPHONE
		// セーブ容量の空きチェック終了時
		if (AoStorageSaveFreeSpaceIsEnough()) {
			// ここでウインドウを開くプロシージャへ
			main_work->proc_menu_update = dmSaveProcWindowOpenEfct;
			
#if _PS3
			if (main_work->win_mode == DME_SAVE_WIN_NOW_SAVING
				|| main_work->win_mode == DME_SAVE_WIN_TRIAL_OUT_SAVE) {
				main_work->proc_draw = NULL;
			}
			
			else {
				main_work->proc_draw = DmSaveWinSelectDraw;
				DmSoundPlaySE("Window");
			}
#else
			main_work->proc_draw = DmSaveWinSelectDraw;
			DmSoundPlaySE("Window");
#endif
			dm_save_draw_reserve = TRUE;
			
		}
		// セーブ容量の空きチェックが終っていないとき
		else {
			// 空処理
		}
	}
	
	else {
		// ここで終了処理までもってく(ウインドウは出さないまま終る)
		main_work->proc_menu_update = dmSaveProcStopDraw;
		dm_save_draw_reserve = FALSE;
	}
}



// ==========================================================================
// dmSaveProcWindowOpenEfct
/*!
	ウインドウオープン中処理
 */
// ==========================================================================
void dmSaveProcWindowOpenEfct(DMS_SAVE_MAIN_WORK *main_work)
{
	// 演出終了チェック
	if (main_work->flag & DMD_SAVE_FLAG_WIN_EFCT_END) {
		// ウインドウのプロシージャ設定
		main_work->proc_menu_update = dmSaveProcWindowAnnounceIdle;
		
		// ウインドウ内アクション表示フラグON
		main_work->disp_flag |= DMD_SAVE_DISP_FLAG_WIN_ACT;
		dm_save_disp_flag = main_work->disp_flag;
		
		// ウインドウ演出中フラグOFF
		main_work->flag &= ~DMD_SAVE_FLAG_WIN_EFCT_END;
		
	}
	else {
		// ウインドウオープン演出処理
		dmSaveSetWinOpenEfct(main_work);
	}
}



// ==========================================================================
// dmSaveProcWindowAnnounceIdle
/*!
	ウインドウ入力待ち処理
 */
// ==========================================================================
void dmSaveProcWindowAnnounceIdle(DMS_SAVE_MAIN_WORK *main_work)
{
	// 入力処理
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}
	
	// ウインドウのパターン分の処理をここに記述
	// 簡易ローディング中メッセージ
	if (main_work->win_mode == DME_SAVE_WIN_DATA_LOADING) {

		if (DmCmnBackupIsLoadFinished()) {
			main_work->win_timer = DMD_SAVE_WIN_EFCT_TIME;
#if _XBOX
			if (!AoStorageLoadIsSuccessed()) {
				main_work->win_timer = 0;
				main_work->timer = DMD_SAVE_LOAD_MIN_TIME;
			}
#endif // _XBOX
			if (main_work->timer >= DMD_SAVE_LOAD_MIN_TIME) {
				
				// ウインドウ内アクション表示フラグOFF
				main_work->disp_flag &= ~DMD_SAVE_DISP_FLAG_WIN_ACT;
				dm_save_disp_flag = main_work->disp_flag;
				
				main_work->proc_menu_update = dmSaveProcWindowCloseEfct;
				
				main_work->timer = 0;
			}
		}
		
#if !_XBOX
		else if (AoSysMsgIsShow()) {
			// ウインドウ演出時間設定
			main_work->win_timer = DMD_SAVE_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_SAVE_DISP_FLAG_WIN_ACT;
			dm_save_disp_flag = main_work->disp_flag;
			
			main_work->proc_menu_update = dmSaveProcWindowCloseEfct;
			
			main_work->timer = 0;
		}
#endif
		
		// エラーメッセージが表示されたら一度タイマーをクリアさせる
		else if (!AoStorageIsExecuteReal()) {
			main_work->timer = 0;
		}
	}
	
	// セーブ時の注意事項メッセージ
	else if (main_work->win_mode == DME_SAVE_WIN_ATTENTION_SAVE) {
		// メニュー遷移フラグONならば
		if (main_work->flag & DMD_SAVE_FLAG_DECIDE) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;
			
			main_work->proc_menu_update = dmSaveProcWindowCloseEfct;
			
			// ウインドウ演出時間設定
			main_work->win_timer = DMD_SAVE_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->disp_flag &= ~DMD_SAVE_DISP_FLAG_WIN_ACT;
			
			DmSoundPlaySE("Ok");
			
			main_work->flag &= ~DMD_SAVE_FLAG_DECIDE;
			
			main_work->timer = 0;
		}
	}
	
	// セーブ中メッセージ
	else if (main_work->win_mode == DME_SAVE_WIN_NOW_SAVING) {
		if (DmCmnBackupIsSaveFinished()) {

			main_work->win_timer = DMD_SAVE_WIN_EFCT_TIME;
#if _XBOX
			if (!AoStorageSaveIsSuccessed()) {
				main_work->win_timer = 0;
				main_work->timer = DMD_SAVE_SAVE_MIN_TIME;
			}
#endif // _XBOX

			if (main_work->timer >= DMD_SAVE_SAVE_MIN_TIME) {
				// 通常処理の入力処理をなくす
				main_work->proc_input = NULL;
				
				// ウインドウ内アクション表示フラグOFF
				main_work->disp_flag &= ~DMD_SAVE_DISP_FLAG_WIN_ACT;
				
				main_work->proc_menu_update = dmSaveProcWindowCloseEfct;
				
				main_work->timer = 0;
			}
		}
		
		// エラーメッセージが表示されたら一度タイマーをクリアさせる
		else if (!AoStorageIsExecuteReal()) {
			main_work->timer = 0;
		}
	}
	
	// 体験版から製品版へ変わった際のセーブ中メッセージ
	else if (main_work->win_mode == DME_SAVE_WIN_TRIAL_OUT_SAVE) {
		if (DmCmnBackupIsSaveFinished()) {

			main_work->win_timer = DMD_SAVE_WIN_EFCT_TIME;
#if _XBOX
			if (!AoStorageSaveIsSuccessed()) {
				main_work->win_timer = 0;
				main_work->timer = DMD_SAVE_SAVE_MIN_TIME;
			}
#endif // _XBOX

			if (main_work->timer >= DMD_SAVE_SAVE_MIN_TIME) {
				// 通常処理の入力処理をなくす
				main_work->proc_input = NULL;
				
				// ウインドウ内アクション表示フラグOFF
				main_work->disp_flag &= ~DMD_SAVE_DISP_FLAG_WIN_ACT;
				
				main_work->proc_menu_update = dmSaveProcWindowCloseEfct;
				
				main_work->timer = 0;
			}
		}
		
		// エラーメッセージが表示されたら一度タイマーをクリアさせる
		else if (!AoStorageIsExecuteReal()) {
			main_work->timer = 0;
		}
	}
	
	else {
		// 無限ループ防止用
		main_work->proc_menu_update = dmSaveProcWindowCloseEfct;
		
		// ウインドウ内アクション表示フラグOFF
		main_work->disp_flag &= ~DMD_SAVE_DISP_FLAG_WIN_ACT;
	}
	
	// 表示フラグ設定
	dm_save_disp_flag = main_work->disp_flag;
	
	main_work->timer++;
}



// ==========================================================================
// dmSaveProcWindowCloseEfct
/*!
	ウインドウクローズ中処理
 */
// ==========================================================================
void dmSaveProcWindowCloseEfct(DMS_SAVE_MAIN_WORK *main_work)
{
	// 演出終了チェック
	if (main_work->flag & DMD_SAVE_FLAG_WIN_EFCT_END) {
		// ウインドウのプロシージャ設定
		if (main_work->win_mode == DME_SAVE_WIN_ATTENTION_SAVE) {
			main_work->proc_menu_update = dmSaveProcWaitSeStop;
		}
		else if (main_work->win_mode == DME_SAVE_WIN_DATA_LOADING) {
			main_work->proc_menu_update = dmSaveProcWaitLoadEnd;
		}
		else {
			main_work->proc_menu_update = dmSaveProcWindowNodispIdle;
		}
		
		// アナウンス分のフラグOFF
		main_work->announce_flag &= ~(1 << main_work->win_mode);
		
		// ウインドウ演出中フラグOFF
		main_work->flag &= ~DMD_SAVE_FLAG_WIN_EFCT_END;
		
		main_work->proc_draw = NULL;
		dm_save_draw_reserve = FALSE;
	}
	
	// ウインドウオープン演出処理
	dmSaveSetWinCloseEfct(main_work);
}


// ==========================================================================
// dmSaveProcWaitSeStop
/*!
	SE停止待ち中処理
 */
// ==========================================================================
void dmSaveProcWaitSeStop(DMS_SAVE_MAIN_WORK *main_work)
{
	if (main_work->timer > DMD_SAVE_SE_END_WAIT_TIME) {
		main_work->proc_menu_update = dmSaveProcWindowNodispIdle;
		
		main_work->timer = 0;
		
		return;
	}
	
	main_work->timer++;
}



// ==========================================================================
// dmSaveProcWaitLoadEnd
/*!
	SE停止待ち中処理
 */
// ==========================================================================
void dmSaveProcWaitLoadEnd(DMS_SAVE_MAIN_WORK *main_work)
{
	if (DmCmnBackupIsLoadFinished()) {
		main_work->proc_menu_update = dmSaveProcWindowNodispIdle;
		
		main_work->timer = 0;
		
		return;
	}
}



// ==========================================================================
// dmSaveProcStopDraw
/*!
	描画停止処理
 */
// ==========================================================================
void dmSaveProcStopDraw(DMS_SAVE_MAIN_WORK *main_work)
{
	main_work->proc_menu_update = dmSaveProcDataRelease;
}

// ==========================================================================
// dmSaveProcDataRelease
/*!
	ファイル解放リクエスト処理
 */
// ==========================================================================
void dmSaveProcDataRelease(DMS_SAVE_MAIN_WORK *main_work)
{
	// メニュー共通テクスチャ解放
	for (int i = 0; i < 2; i++) {
		AoTexRelease(&main_work->cmn_tex[i]);
	}
	
	if (dm_save_is_snd_build) {
		// デモサウンドモジュール終了
		DmSoundExit();
		
		// サウンドデータ開放処理
		DmSoundFlush();
	}
	
	// 次へ遷移
	main_work->proc_menu_update = dmSaveProcFinish;
}


// ==========================================================================
// dmSaveProcFinish
/*!
	終了処理
 */
// ==========================================================================
void dmSaveProcFinish(DMS_SAVE_MAIN_WORK *main_work)
{
	// テクスチャ解放完了判定
	if (dmSaveIsTexRelease(main_work) == 1) {
		for (int i = 0; i < 2; i++) {
			dm_save_cmn_tex[i] = NULL;
		}
		
		for (int i = 0; i < ACT_NUM; i++) {
			if (main_work->act[i]) {
				AoActDelete(main_work->act[i]);
				main_work->act[i] = NULL;
			}
			
			dm_save_act[i] = NULL;
		}
		
		// アクション解放
		for (int i = 0; i < 2; i++) {
			// ファイル解放
			if (main_work->arc_cmn_amb[i]) {
				amMemFree(main_work->arc_cmn_amb[i]);
				main_work->arc_cmn_amb[i] = NULL;
			}
		}
		
		// 終了処理へ
		main_work->flag |= DMD_SAVE_FLAG_EXIT;
		main_work->proc_menu_update = NULL;
	}
}



// ==========================================================================
// dmSaveInputProcWindow
/*!
	ウインドウ表示用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmSaveInputProcWindow(DMS_SAVE_MAIN_WORK *main_work)
{
	// 決定判定
	if (AoAccountGetCurrentId() < 0) {
		if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
			// フラグON
			main_work->flag |= DMD_SAVE_FLAG_DECIDE;
		}
	}
	
	else {
		if (AoPadStand() & GSD_KEY_DECIDE) {
			// フラグON
			main_work->flag |= DMD_SAVE_FLAG_DECIDE;
		}
	}
}



// ==========================================================================
// DmSaveWinSelectDraw
/*!
	ウインドウ用描画設定処理
 */
// ==========================================================================
void DmSaveWinSelectDraw(void)
{
//	u32 i = 0;
	s32 tmp_size_w = 0;
	s32 tmp_size_h = 0;
	s32 tmp_tex_dist = 0;
	u32 tmp_win_tex_id = 0;
	
	// ウインドウ用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_SAVE_DRAW_PRIO_WIN_FIX);
	
	if (dm_save_msg_flag & (1 << DME_SAVE_WIN_ATTENTION_SAVE)) {
#if _WII || _IPHONE
		tmp_size_w = (s32)(DMD_SAVE_WINDOW_SIZE_W + 64) * DMD_SAVE_DISP_SCALE_TEXT;
		tmp_size_h = (s32)(DMD_SAVE_WINDOW_SIZE_H + dm_save_win_size_y_tbl[GsEnvGetLanguage()]) * DMD_SAVE_DISP_SCALE_TEXT;
#else
		tmp_size_w = (s32)(DMD_SAVE_WINDOW_SIZE_W + 64);
		tmp_size_h = (s32)(DMD_SAVE_WINDOW_SIZE_H + dm_save_win_size_y_tbl[GsEnvGetLanguage()]);
#endif
	}
	else {
		// ※※※セーブとロード時について、実際にセーブ・ロードしてるとき以外は描画させないための処理
		if (!AoStorageIsExecuteReal()) {
			return;
		}
		
#if _WII || _IPHONE
		if (GsEnvGetLanguage() == GSD_LANGUAGE_GE) {
			tmp_size_w = (DMD_SAVE_WINDOW_SIZE_W + 64) * DMD_SAVE_DISP_SCALE_TEXT;
			tmp_size_h = DMD_SAVE_WINDOW_SIZE_H * DMD_SAVE_DISP_SCALE_TEXT;
		}
		else {
			tmp_size_w = DMD_SAVE_WINDOW_SIZE_W * DMD_SAVE_DISP_SCALE_TEXT;
			tmp_size_h = DMD_SAVE_WINDOW_SIZE_H * DMD_SAVE_DISP_SCALE_TEXT;
		}
		
#else
		tmp_size_w = (s32)DMD_SAVE_WINDOW_SIZE_W;
		tmp_size_h = (s32)DMD_SAVE_WINDOW_SIZE_H;
#endif
	}
	
	// 暗転以外は全て半透明ウインドウに設定
	if (dm_save_is_task_draw) {
		tmp_win_tex_id = 1;
	}
	else if (dm_save_msg_flag & 1 << DME_SAVE_WIN_ATTENTION_SAVE) {
		tmp_win_tex_id = 1;
	}
	else {
		tmp_win_tex_id = 0;
	}
	
	
	// ウインドウ描画
	if (dm_save_is_draw_state) {
		AoWinSysDrawState(AOD_WIN_TYPE_A
						 , AoTexGetTexList(dm_save_cmn_tex[0])
						 , tmp_win_tex_id
						 , DMD_SAVE_SIZE_WIDTH / 2.0f		// ウインドウ中心X
						 , DMD_SAVE_SIZE_HEIGHT / 2.0f		// ウインドウ中心Y
						 , tmp_size_w * dm_save_win_size_rate[0]			// ウインドウ横サイズ
						 , tmp_size_h * dm_save_win_size_rate[1]			// ウインドウ縦サイズ
						 , dm_save_draw_state				// 描画STATE
						 );
	}
	else {
		AoWinSysDrawTask(AOD_WIN_TYPE_A
						 , AoTexGetTexList(dm_save_cmn_tex[0])
						 , tmp_win_tex_id
						 , DMD_SAVE_SIZE_WIDTH / 2.0f		// ウインドウ中心X
						 , DMD_SAVE_SIZE_HEIGHT / 2.0f		// ウインドウ中心Y
						 , tmp_size_w * dm_save_win_size_rate[0]			// ウインドウ横サイズ
						 , tmp_size_h * dm_save_win_size_rate[1]			// ウインドウ縦サイズ
						 , DMD_SAVE_DRAW_PRIO_WIN			// タスクプライオリティ
						 );
	}
	
	// ウインドウ内の項目描画
	if (dm_save_disp_flag & DMD_SAVE_DISP_FLAG_WIN_ACT) {
		switch (dm_save_win_mode) {
		case DME_SAVE_WIN_DATA_LOADING:
			// アクション登録
			AoActSetTexture(AoTexGetTexList(dm_save_cmn_tex[1]));
			AoActSortRegAction(dm_save_act[ACT_TEX_MSG1]);
			
			break;
			
		case DME_SAVE_WIN_ATTENTION_SAVE:
			// アクション登録
			AoActSetTexture(AoTexGetTexList(dm_save_cmn_tex[1]));
			AoActSortRegAction(dm_save_act[ACT_TEX_MSG2]);
			AoActSortRegAction(dm_save_act[ACT_TEX_OK]);
			
			break;
			
		case DME_SAVE_WIN_NOW_SAVING:
			// アクション登録
			AoActSetTexture(AoTexGetTexList(dm_save_cmn_tex[1]));
			AoActSortRegAction(dm_save_act[ACT_TEX_MSG3]);
			
			break;
			
		case DME_SAVE_WIN_TRIAL_OUT_SAVE:
			// アクション登録
			AoActSetTexture(AoTexGetTexList(dm_save_cmn_tex[1]));
			AoActSortRegAction(dm_save_act[ACT_TEX_MSG3]);
			
			break;
			
		default:
			// 例外
			MTM_ASSERT(0);
			break;
		}
		
		AoActAcmPush();
		
		if (GsEnvGetLanguage() != 0) {
			tmp_tex_dist = (dm_save_win_size_y_tbl[GsEnvGetLanguage()] / 2);
		}
		else {
			tmp_tex_dist = 0;
		}
		
		for (int i = 0; i < ACT_NUM; i++) {
			// ファイル選別
			AOS_TEXTURE *tex;
			
			if (i >= ACT_TEX_WINTITLE) {
				tex = dm_save_cmn_tex[1];
			}
			else  {
				tex = dm_save_cmn_tex[0];
			}
			
			AoActAcmInit();
			AoActAcmApplyTrans(dm_save_win_act_pos_tbl[i][0]
							   , dm_save_win_act_pos_tbl[i][1]
							   , 0
							   );
			
			if (dm_save_msg_flag & (1 << DME_SAVE_WIN_ATTENTION_SAVE)) {
				if (i == ACT_NUM - 1) {
					AoActAcmApplyTrans(0, 16.f + tmp_tex_dist, 0);
				}
				else if (i == 0 || i == 1) {
					AoActAcmApplyTrans(-32.f, (f32)(tmp_tex_dist * -1), 0);
				}
			}
			
#if _WII || _IPHONE
			AoActAcmApplyScale(DMD_SAVE_DISP_SCALE_TEXT
							   , DMD_SAVE_DISP_SCALE_TEXT);
#endif
			
			// フレーム更新はSetFrameのみで行う
			AoActSetTexture(AoTexGetTexList(tex));
			AoActUpdate(dm_save_act[i], 0.0f);
		}
		
		AoActAcmPop();
		
		// ソート実行
		AoActSortExecute();

		// ソートバッファ描画
		AoActSortDraw();
		// ソートバッファ全解除
		AoActSortUnregAll();
	}
	
	if (dm_save_is_draw_state
		&& dm_save_is_task_draw) {
		amDrawMakeTask(dmSaveTaskDraw, (u16)DMD_SAVE_DRAW_TASK_PRIO, (u32)0);
	}
}



// ==========================================================================
// dmSaveTaskDraw
/*!
	セーブ画面の描画タスク
 */
// ==========================================================================
void dmSaveTaskDraw(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	AoActDrawPre();
	amDrawExecCommand(dm_save_draw_state);
	amDrawEndScene();
}



// ==========================================================================
// dmSaveIsDataLoad
/*!
	データ読み込み完了チェック処理
 */
// ==========================================================================
s32 dmSaveIsDataLoad(DMS_SAVE_MAIN_WORK *main_work)
{
	// メニュー共通データ読み込み
	for (int i = 0; i < 2; i++) {
		if (!amFsIsComplete(main_work->arc_cmn_amb_fs[i])) {
			return 0;
		}
	}
	
	return 1;
}


// ==========================================================================
// dmSaveIsTexLoad
/*!
	テクスチャ構築完了チェック処理
 */
// ==========================================================================
s32 dmSaveIsTexLoad(DMS_SAVE_MAIN_WORK *main_work)
{
	// メニュー共通データ
	for (int i = 0; i < 2; i++) {
		if (!AoTexIsLoaded(&main_work->cmn_tex[i])) {
			// フラグ扱いでON
			return 0;
		}
	}
	
	// サウンド構築が必要ならば
	if (dm_save_is_snd_build) {
		if (!DmSoundBuildCheck()) {
			return 0;
		}
	}
	
	return 1;
}


// ==========================================================================
// dmSaveIsTexRelease
/*!
	テクスチャ解放完了チェック処理
 */
// ==========================================================================
s32 dmSaveIsTexRelease(DMS_SAVE_MAIN_WORK *main_work)
{
	// メニュー共通データ
	for (int i = 0; i < 2; i++) {
		if (!AoTexIsReleased(&main_work->cmn_tex[i])) {
			return 0;
		}
	}
	
	return 1;
}





// ==========================================================================
// dmSaveSetWinOpenEfct
/*!
	ウインドウ入り演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmSaveSetWinOpenEfct(DMS_SAVE_MAIN_WORK *main_work)
{
	if (main_work->win_timer > DMD_SAVE_WIN_EFCT_TIME) {
		// ウインドウ演出終了
		main_work->flag |= DMD_SAVE_FLAG_WIN_EFCT_END;

		main_work->win_timer = 0;

		for (u32 i = 0; i < 2; i++) {
			main_work->win_size_rate[i] = DMD_SAVE_WIN_DEF_RATE;
		}
	}
	else {
		// タイマー更新(但しフレームレートが可変になるように修正すること)	◆
		main_work->win_timer++;
	}
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->win_size_rate[i] = (float)(main_work->win_timer / 8.f);
		}
		else {
			main_work->win_size_rate[i] = 1.0f;
		}

		if (main_work->win_size_rate[i] > 1.0f) {
			main_work->win_size_rate[i] = 1.0f;
		}
		
		// グローバルに設定
		dm_save_win_size_rate[i] = main_work->win_size_rate[i];
	}
}



// ==========================================================================
// dmSaveSetWinCloseEfct
/*!
	ウインドウ閉め演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmSaveSetWinCloseEfct(DMS_SAVE_MAIN_WORK *main_work)
{
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->win_size_rate[i] = (float)(main_work->win_timer / 8.f);
		}
		else {
			main_work->win_size_rate[i] = 0.0f;
		}
		
		// グローバルに設定
		dm_save_win_size_rate[i] = main_work->win_size_rate[i];
	}

	if (main_work->win_timer < 0) {
		// ウインドウ演出終了
		main_work->flag |= DMD_SAVE_FLAG_WIN_EFCT_END;

		main_work->win_timer = 0;

		for (u32 i = 0; i < 2; i++) {
			main_work->win_size_rate[i] = 0.0f;
			
			// グローバルに設定
			dm_save_win_size_rate[i] = main_work->win_size_rate[i];
		}
	}
	else {
		// タイマー更新(但しフレームレートが可変になるように修正すること)	◆
		main_work->win_timer--;
	}
}





// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
