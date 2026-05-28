// =======================================================================
/*!
  @file	gmDebugPause.cpp
  @brief メインゲームデバッグポーズ

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmDebugPause.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akUtil.h"
#include "gmMain.h"
#include "gmPlayer.h"
#include "gmGameDat.h"
#include "gmDebugPause.h"
#include "gmEventMgr.h"
#include "gmFix.h"
#include "gmSound.h"
#include "gmRing.h"

#if defined(MTD_DEBUG)
// 全文デバッグコード

#if _IPHONE
#include "dbgPadEmu.hpp"
#endif //_IPHONE

/*------ Macros --------------------------------------------------------*/
/* フラグ */
#define GMD_DBG_PAUSE_FLAG_END			(1 << 0)		//!< 終了フラグ
#define GMD_DBG_PAUSE_FLAG_DECIDED		(1 << 1)		//!< 決定押下フラグ

/* 定義値 */
#define GMD_DBG_PAUSE_DRAW_POS_X		(32)			//!< デバッグポーズメニュー表示位置X
#define GMD_DBG_PAUSE_DRAW_POS_Y		(14)			//!< デバッグポーズメニュー表示位置Y

#define GMD_DBG_PAUSE_SCORE_DIGIT_NUM	(9)				//!< スコアの桁数

#define GMD_DBG_PAUSE_CHALLENGE_DIGIT_NUM	(3)			//!< チャレンジ数の桁数

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! デバッグ表示項目
typedef enum
{
	GME_DBG_PAUSE_ITEM_ZONE	= 0,		//!< ZONE
	GME_DBG_PAUSE_ITEM_ACT,				//!< ACT
	GME_DBG_PAUSE_ITEM_INVINCIBLE,		//!< 無敵
	GME_DBG_PAUSE_ITEM_RECT,			//!< 矩形表示
	GME_DBG_PAUSE_ITEM_PLY_POS_TYPE,	//!< プレイヤー座標表示タイプ
	GME_DBG_PAUSE_ITEM_RING,			//!< リング数
	GME_DBG_PAUSE_ITEM_TIMER,			//!< ゲームタイマ
	GME_DBG_PAUSE_ITEM_SCORE,			//!< スコア
	GME_DBG_PAUSE_ITEM_CHALLENGE,		//!< チャレンジ数
	GME_DBG_PAUSE_ITEM_DEBUG_DISP,		//!< デバッグ表示
	GME_DBG_PAUSE_ITEM_FIX_DISP,		//!< FIX表示
	GME_DBG_PAUSE_ITEM_EVENT_RESET,		//!< イベント復旧
	GME_DBG_PAUSE_ITEM_PLAYER_DISP,		//!< プレイヤー表示設定
	GME_DBG_PAUSE_ITEM_DEBUG_CHECK,		//!< デバッグチェック設定
	GME_DBG_PAUSE_ITEM_MAP_DISP,		//!< マップ表示設定
	GME_DBG_PAUSE_ITEM_OBJ_DISP,		//!< オブジェクト表示設定
#if _IPHONE
	GME_DBG_PAUSE_ITEM_SCREEN_UP,		//!< 画面強制アップ設定
#endif // _IPHONE
	
	GME_DBG_PAUSE_ITEM_MAX
} GME_DBG_PAUSE_ITEM;

//! 矩形表示タイプ
typedef enum
{
	GME_DBG_PAUSE_RECT_DISP_TYPE_OFF	= 0,	//!< オフ
	GME_DBG_PAUSE_RECT_DISP_TYPE_HIT,			//!< 攻撃・喰らい矩形
	GME_DBG_PAUSE_RECT_DISP_TYPE_FIELD,			//!< 地形当たり矩形
	GME_DBG_PAUSE_RECT_DISP_TYPE_BOTH,			//!< 両方
	
	GME_DBG_PAUSE_RECT_DISP_TYPE_MAX
} GME_DBG_PAUSE_RECT_DISP_TYPE;


//! プレイヤー座標表示タイプ
typedef enum
{
	GME_DBG_PAUSE_PLY_POS_DISP_TYPE_10I	= 0,	//!< 10進整数
	GME_DBG_PAUSE_PLY_POS_DISP_TYPE_16I,		//!< 16進整数
	GME_DBG_PAUSE_PLY_POS_DISP_TYPE_10F,		//!< 10進小数点有り
		
	GME_DBG_PAUSE_PLY_POS_DISP_TYPE_MAX
} GME_DBG_PAUSE_PLY_POS_TYPE;

//! タイマ項目のフォーカス位置
typedef enum
{
	GME_DBG_PAUSE_TIMER_FOCUS_MIN	= 0,
	GME_DBG_PAUSE_TIMER_FOCUS_SEC,
	
	GME_DBG_PAUSE_TIMER_FOCUS_MAX
} GME_DBG_PAUSE_TIMER_FOCUS;

//! デバッグポーズワーク
typedef struct tag_GMS_DBG_PAUSE_WORK
{
	Uint32		flag;
	Sint32		select_item;	//!< 現在選択中の項目
	Sint32		time_focus_h;	//!< タイマ項目フォーカス位置
	Sint32		score_inc_size;	//!< スコア変更増加サイズ（10をこの値でべき乗したサイズで増減）
	Sint32		challenge_inc_size;	//!< チャレンジ数変更増加サイズ（10をこの値でべき乗したサイズで増減）
	//! g_gm_main_system.game_flag タイムカウント系フラグのポーズ直前の状態を保存
	Uint32		time_count_flag_save;
	
	/* デバッグ情報 */
	// ステージ
	Sint32		zone;
	Sint32		act;
	
	// 無敵
	BOOL		is_invincible;
	
	// 矩形表示
	Sint32		rect_disp_type;
	
	// プレイヤー座標表示タイプ
	Sint32		ply_pos_disp_type;
	
	// リング
	Sint16		ring_num;
	Sint16		reserved[1];
	
	// タイマ
	Sint32		cur_time;
	
	// スコア
	Sint32		score;
	
	// チャレンジ数
	Uint32		challenge_num;
	
	// デバッグ表示
	BOOL		is_debug_disp_on;
	
	// FIX表示
	BOOL		is_fix_disp_on;
	
	// イベント復旧
	BOOL		is_eve_reset_on;

	// プレイヤー表示
	BOOL		is_player_disp_on;
	
	// デバッグチェック
	Sint32		debug_check_mode;

	// マップ表示
	BOOL		is_map_disp_on;

	// オブジェクト表示
	BOOL		is_obj_disp_on;

#if _IPHONE
	// 画面強制アップ設定
	BOOL		is_screen_up_on;
#endif _IPHONE

} GMS_DBG_PAUSE_WORK;

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/

static void gmDbgPauseDest(MTS_TASK_TCB *tcb);
static void gmDbgPauseMain(MTS_TASK_TCB *tcb);
static void gmDbgPauseDraw(GMS_DBG_PAUSE_WORK *dbg_pause_work);
static void gmDbgPauseGetDebugStatus(GMS_DBG_PAUSE_WORK *dbg_pause_work);
static void gmDbgPauseSetDebugStatus(GMS_DBG_PAUSE_WORK *dbg_pause_work);
static Sint32 gmDbgPauseGetChange(void);
static Sint32 gmDbgPauseGetFocusMoveV(void);
static Sint32 gmDbgPauseGetFocusMoveH(void);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
//! デバッグポーズタスクTCB
static MTS_TASK_TCB	*gm_dbg_pause_tcb	= NULL;

//! ON/OFF表示
const static char* gm_dbg_pause_string_onoff[]	= {
	"OFF",
	"ON",
};

//! ゾーン名
const char *gm_dbg_pause_zone_name[GSD_MAIN_STAGE_ID_MAX] = {
	"1", "1", "1", "1",
	"2", "2", "2", "2",
	"3", "3", "3", "3",
	"4", "4", "4", "4",
	"FINAL", "FINAL", "FINAL", "FINAL", "FINAL",
	"SS", "SS", "SS", "SS", "SS", "SS", "SS",
};

//! アクト名
const char *gm_dbg_pause_act_name[GSD_MAIN_STAGE_ID_MAX] = {
	"1", "2", "3", "BOSS",
	"1", "2", "3", "BOSS",
	"1", "2", "3", "BOSS",
	"1", "2", "3", "BOSS",
	"1", "2", "3", "4", "5",
	"1", "2", "3", "4", "5", "6", "7",
};

//! ゾーン先頭アクト
const s32 gm_dbg_pause_zone_top[GSD_MAIN_ZONE_TYPE_MAX] = {
	GSD_MAIN_STAGE_ID_1_1,
	GSD_MAIN_STAGE_ID_2_1,
	GSD_MAIN_STAGE_ID_3_1,
	GSD_MAIN_STAGE_ID_4_1,
	GSD_MAIN_STAGE_ID_FINAL_1,
	GSD_MAIN_STAGE_ID_SS1,
};

//! 矩形表示タイプ
const char* gm_dbg_pause_string_rect[GME_DBG_PAUSE_RECT_DISP_TYPE_MAX]	= {
	"OFF",
	"HIT",
	"FIELD",
	"HIT+FIELD",
};

//! プレイヤー座標表示タイプ
const char* gm_dbg_pause_string_ply_pos_disp[GME_DBG_PAUSE_PLY_POS_DISP_TYPE_MAX]	= {
	"10 Int",
	"16 Int",
	"10 Float",
};

//!< デバッグチェックモード
const char* gm_dbg_pause_string_debug_check_mode[GMD_MAIN_DEBUG_CHECK_MODE_MAX]	= {
	"None",
	"Check Final Boss Finish",
};

/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GmDbgPauseInit
/*!
  デバッグポーズ初期化
 */
// =======================================================================
void GmDbgPauseInit(void)
{
	GMS_DBG_PAUSE_WORK	*dbg_pause_work;
	Uint32	time_count_flag_save	= 0;
	
	MTM_ASSERT(gm_dbg_pause_tcb == NULL && "gmDebugPause.cpp::GmDbgPauseInit() already executed\n");
	
	// サウンド一時停止
	GmSoundAllPause();
	
	// オブジェクトポーズ開始
	ObjObjectPause(GMD_DEBUG_DEBUGPAUSE_LEVEL);
	
	// ポーズ演出開始
	g_gm_main_system.game_flag	|= GMD_GAME_FLAG_PAUSE_DEMO;
	g_gm_main_system.game_flag	&= ~GMD_GAME_FLAG_PAUSE_IS_DECIDED;
	
	// タイムカウント系フラグを退避
	time_count_flag_save	= g_gm_main_system.game_flag & (GMD_GAME_FLAG_COUNT_GAME_TIME |
															GMD_GAME_FLAG_COUNT_SYNC_TIME);
	
	// ゲームタイマ、同期タイマ停止
	g_gm_main_system.game_flag	&= ~(GMD_GAME_FLAG_COUNT_GAME_TIME |
									 GMD_GAME_FLAG_COUNT_SYNC_TIME);
	
	// タスク生成
	gm_dbg_pause_tcb	= MTM_TASK_MAKE_TCB(gmDbgPauseMain,
											gmDbgPauseDest,
											0,//flag
											GMD_TASK_NO_GAME_PAUSE,
											GMD_TASK_PRIO_PAUSE,
											GMD_TASK_GROUP_PAUSE,
											sizeof(GMS_DBG_PAUSE_WORK),
											"GM_DBG_PAUSE");
	
	// ワーククリア
	dbg_pause_work	= (GMS_DBG_PAUSE_WORK*)mtTaskGetTcbWork(gm_dbg_pause_tcb);
	amZeroMemory(dbg_pause_work, sizeof(GMS_DBG_PAUSE_WORK));
	
	// 退避したタイムカウント系フラグをワークに保存
	dbg_pause_work->time_count_flag_save	= time_count_flag_save;
	
	// デバッグステータス取得
	gmDbgPauseGetDebugStatus(dbg_pause_work);

#if _IPHONE
	//パッドトリガをドラッグに設定
	dbg::CPadEmu &pad_emu = dbg::CPadEmu::CreateInstance();
	pad_emu.Create(dbg::CPadEmu::EMode::Drag);
#endif //_IPHONE
}



/*------ Static Functions ----------------------------------------------*/
// =======================================================================
// gmDbgPauseDest
/*!
  デバッグポーズ デストラクタ
 */
// =======================================================================
void gmDbgPauseDest(MTS_TASK_TCB *tcb)
{
	GMS_DBG_PAUSE_WORK	*dbg_pause_work	= (GMS_DBG_PAUSE_WORK*)mtTaskGetTcbWork(tcb);
	
	// ゲームタイマ、同期タイマ再開
#if 1	// 単純にタイマを再開するのではなく、ポーズ直前の状態に戻す
	g_gm_main_system.game_flag	|= (dbg_pause_work->time_count_flag_save &
									(GMD_GAME_FLAG_COUNT_GAME_TIME |
									 GMD_GAME_FLAG_COUNT_SYNC_TIME));
#else
	g_gm_main_system.game_flag	|= (GMD_GAME_FLAG_COUNT_GAME_TIME |
									GMD_GAME_FLAG_COUNT_SYNC_TIME);
#endif	/* 1 */
	
	// ポーズ演出終了
	g_gm_main_system.game_flag	&= ~GMD_GAME_FLAG_PAUSE_DEMO;

#if _IPHONE
	//パッドトリガをゲームに設定
	dbg::CPadEmu &pad_emu = dbg::CPadEmu::CreateInstance();
	pad_emu.Create(dbg::CPadEmu::EMode::Game);
#endif //_IPHONE
	
	gm_dbg_pause_tcb	= NULL;
}

// =======================================================================
// gmDbgPauseMain
/*!
  デバッグポーズ メイン処理
 */
// =======================================================================
void gmDbgPauseMain(MTS_TASK_TCB *tcb)
{
	GMS_DBG_PAUSE_WORK	*dbg_pause_work	= (GMS_DBG_PAUSE_WORK*)mtTaskGetTcbWork(tcb);
	Sint32 change_val;
	Sint32 focus_v_move;
	Sint32 focus_h_move;
	
	MTM_ASSERT(dbg_pause_work);
	
	if (dbg_pause_work->flag & GMD_DBG_PAUSE_FLAG_END) {
		// 終了
		
		// オブジェクトポーズ終了
		ObjObjectPauseOut();
		
		// タスククリア
		mtTaskClearTcb(tcb);
		
		// サウンド再開
		GmSoundAllResume();
		return;
	}
	
	
	// 入力値取得
	change_val	= gmDbgPauseGetChange();
	
	// 上下移動取得
	focus_v_move	= gmDbgPauseGetFocusMoveV();
	
	// 左右移動取得
	focus_h_move	= gmDbgPauseGetFocusMoveH();
	
	
	if (AoPadStand() & (KEY_START | KEY_R_RIGHT) ||
		AoPadStand() & KEY_R_DOWN) {
		// 決定・キャンセル操作
		
		if (AoPadStand() & KEY_R_DOWN) {
			dbg_pause_work->flag	|= GMD_DBG_PAUSE_FLAG_DECIDED;
		}
		
		// デバッグステータス設定
		gmDbgPauseSetDebugStatus(dbg_pause_work);
		
		// 終了通知
		dbg_pause_work->flag	|= GMD_DBG_PAUSE_FLAG_END;
		
		if (AoPadStand() & KEY_R_DOWN) {
			// 決定時
			
			switch (dbg_pause_work->select_item) {
			case GME_DBG_PAUSE_ITEM_ZONE:
			case GME_DBG_PAUSE_ITEM_ACT:
				// ステージID設定
				// TODO : ここでステージID設定すると解放処理が正常に行われないので、
				//        ステージ間のイベント遷移処理がある程度固まってきてから実装する
				//GsGetMainSysInfo()->stage_id	= gm_dbg_pause_zone_top[dbg_pause_work->zone] + dbg_pause_work->act;
				
				// イベント移行先設定
				//SyDecideEvt(GSD_EVT_ID_MAINGAME);
				
				break;
			default:
				// 何もしない
				break;
			}
		}
		
		return;
	}
	else {
		// アイテム選択
		if (focus_v_move < 0) {
			dbg_pause_work->select_item--;
			if (dbg_pause_work->select_item < 0) {
				dbg_pause_work->select_item	= GME_DBG_PAUSE_ITEM_MAX - 1;
			}
		}
		else if (focus_v_move > 0) {
			dbg_pause_work->select_item++;
			if (dbg_pause_work->select_item >= GME_DBG_PAUSE_ITEM_MAX) {
				dbg_pause_work->select_item	= 0;
			}
		}
	}
	
	
	// 項目毎処理
	switch (dbg_pause_work->select_item) {
	case GME_DBG_PAUSE_ITEM_ZONE:
#if 0	// TODO : ステージ間遷移未実装のため無効化
		if (change_val > 0) {
			dbg_pause_work->zone++;
		}
		else if (change_val < 0) {
			dbg_pause_work->zone--;
		}
#endif
		dbg_pause_work->zone	= MTM_MATH_CLIP(dbg_pause_work->zone, 0, GSD_MAIN_ZONE_TYPE_MAX-1);
		break;
		
	case GME_DBG_PAUSE_ITEM_ACT:
#if 0	// TODO : ステージ間遷移未実装のため無効化
		if (change_val > 0) {
			dbg_pause_work->act++;
		}
		else if (change_val < 0) {
			dbg_pause_work->act--;
		}
#endif
		break;
		
	case GME_DBG_PAUSE_ITEM_INVINCIBLE:
#if 0	// TODO : デバッグ無敵が未実装なので無効化
		if (change_val) {
			dbg_pause_work->is_invincible	=
				((dbg_pause_work->is_invincible) ? FALSE : TRUE);
		}
#endif
		break;
		
	case GME_DBG_PAUSE_ITEM_RECT:
		if (change_val > 0) {
			dbg_pause_work->rect_disp_type++;
		}
		else if (change_val < 0) {
			dbg_pause_work->rect_disp_type--;
		}
		
		if (dbg_pause_work->rect_disp_type >= GME_DBG_PAUSE_RECT_DISP_TYPE_MAX) {
			dbg_pause_work->rect_disp_type	= 0;
		}
		else if (dbg_pause_work->rect_disp_type < 0) {
			dbg_pause_work->rect_disp_type	= GME_DBG_PAUSE_RECT_DISP_TYPE_MAX - 1;
		}
		break;
		
	case GME_DBG_PAUSE_ITEM_PLY_POS_TYPE:
		if (change_val > 0) {
			dbg_pause_work->ply_pos_disp_type++;
		}
		else if (change_val < 0) {
			dbg_pause_work->ply_pos_disp_type--;
		}
		
		if (dbg_pause_work->ply_pos_disp_type >= GME_DBG_PAUSE_PLY_POS_DISP_TYPE_MAX) {
			dbg_pause_work->ply_pos_disp_type	= 0;
		}
		else if (dbg_pause_work->ply_pos_disp_type < 0) {
			dbg_pause_work->ply_pos_disp_type	= GME_DBG_PAUSE_PLY_POS_DISP_TYPE_MAX - 1;
		}
		break;
		
	case GME_DBG_PAUSE_ITEM_RING:
		if (change_val > 0) {
			dbg_pause_work->ring_num++;
		}
		else if (change_val < 0) {
			dbg_pause_work->ring_num--;
		}
		
		if (dbg_pause_work->ring_num < 0) {
			dbg_pause_work->ring_num	= 0;
		}
		break;
		
	case GME_DBG_PAUSE_ITEM_TIMER:
		{
			Sint32	time_step;
			
			// 左右移動取得
			if (focus_h_move > 0) {
				dbg_pause_work->time_focus_h++;
			}
			else if (focus_h_move < 0) {
				dbg_pause_work->time_focus_h--;
			}
			
			// 回り込み
			if (dbg_pause_work->time_focus_h >= GME_DBG_PAUSE_TIMER_FOCUS_MAX) {
				dbg_pause_work->time_focus_h	= 0;
			}
			else if (dbg_pause_work->time_focus_h < 0) {
				dbg_pause_work->time_focus_h	= GME_DBG_PAUSE_TIMER_FOCUS_MAX-1;
			}
			
			// フォーカス位置によって変更値を変更
			switch (dbg_pause_work->time_focus_h) {
			case GME_DBG_PAUSE_TIMER_FOCUS_MIN:
				// 1分単位
				time_step	= 60*60;
				break;
			case GME_DBG_PAUSE_TIMER_FOCUS_SEC:
				// 1秒単位
				time_step	= 60;
				break;
			default:
				time_step	= 60*60;
			}
			
			// 値変更
			if (change_val > 0) {
				dbg_pause_work->cur_time	+= time_step;
			}
			else if (change_val < 0) {
				dbg_pause_work->cur_time	-= time_step;
			}
		}
		
		if (dbg_pause_work->cur_time < 0) {
			dbg_pause_work->cur_time	= 0;
		}
		break;
		
	case GME_DBG_PAUSE_ITEM_SCORE:
		{
			Sint32	inc_step;
			
			// 左右移動取得
			if (focus_h_move > 0) {
				dbg_pause_work->score_inc_size++;
			}
			else if (focus_h_move < 0) {
				dbg_pause_work->score_inc_size--;
			}
			
			// 増減サイズクリップ
			dbg_pause_work->score_inc_size	= (Sint32)MTM_MATH_CLIP(dbg_pause_work->score_inc_size,
																	0, GMD_DBG_PAUSE_SCORE_DIGIT_NUM-1);
			
			inc_step	= (Sint32)nnPow(10, dbg_pause_work->score_inc_size);
			
			if (change_val > 0) {
				dbg_pause_work->score	+= inc_step;
			}
			else if (change_val < 0) {
				dbg_pause_work->score	-= inc_step;
			}
			
			// スコアサイズクリップ
			dbg_pause_work->score	= MTM_MATH_CLIP(dbg_pause_work->score,
													0, 999999999);
		}
		break;
		
	case GME_DBG_PAUSE_ITEM_CHALLENGE:
		{
			Sint32	inc_step;
			
			// 左右移動取得
			if (focus_h_move > 0) {
				dbg_pause_work->challenge_inc_size++;
			}
			else if (focus_h_move < 0) {
				dbg_pause_work->challenge_inc_size--;
			}
			
			// 増減サイズクリップ
			dbg_pause_work->challenge_inc_size	= (Sint32)MTM_MATH_CLIP(dbg_pause_work->challenge_inc_size,
																		0, GMD_DBG_PAUSE_CHALLENGE_DIGIT_NUM-1);
			
			inc_step	= (Sint32)nnPow(10, dbg_pause_work->challenge_inc_size);
			
			if (change_val > 0) {
				dbg_pause_work->challenge_num	+= inc_step;
			}
			else if (change_val < 0) {
				dbg_pause_work->challenge_num	-= inc_step;
			}
			
			// スコアサイズクリップ
			dbg_pause_work->challenge_num	= MTM_MATH_CLIP(dbg_pause_work->challenge_num,
															1, GSD_MAINSYS_PLAYER_REST_MAX);
		}
		break;
		
	case GME_DBG_PAUSE_ITEM_DEBUG_DISP:
		if (change_val) {
			dbg_pause_work->is_debug_disp_on	=
				((dbg_pause_work->is_debug_disp_on) ? FALSE : TRUE);
		}
		break;
		
	case GME_DBG_PAUSE_ITEM_FIX_DISP:
		if (change_val) {
			dbg_pause_work->is_fix_disp_on	=
				((dbg_pause_work->is_fix_disp_on) ? FALSE : TRUE);
		}
		break;
		
	case GME_DBG_PAUSE_ITEM_EVENT_RESET:
		if (change_val) {
			dbg_pause_work->is_eve_reset_on	=
				((dbg_pause_work->is_eve_reset_on) ? FALSE : TRUE);
		}
		break;

	case GME_DBG_PAUSE_ITEM_PLAYER_DISP:
		if (change_val) {
			dbg_pause_work->is_player_disp_on	=
				((dbg_pause_work->is_player_disp_on) ? FALSE : TRUE);
		}
		break;
		
	case GME_DBG_PAUSE_ITEM_DEBUG_CHECK:
		if (change_val > 0) {
			dbg_pause_work->debug_check_mode++;
		}
		else if (change_val < 0) {
			dbg_pause_work->debug_check_mode--;
		}
		
		if (dbg_pause_work->debug_check_mode >= GMD_MAIN_DEBUG_CHECK_MODE_MAX) {
			dbg_pause_work->debug_check_mode	= 0;
		}
		else if (dbg_pause_work->debug_check_mode < 0) {
			dbg_pause_work->debug_check_mode	= GMD_MAIN_DEBUG_CHECK_MODE_MAX - 1;
		}
		break;

	case GME_DBG_PAUSE_ITEM_MAP_DISP:
		if (change_val) {
			dbg_pause_work->is_map_disp_on	=
				((dbg_pause_work->is_map_disp_on) ? FALSE : TRUE);
		}
		break;

	case GME_DBG_PAUSE_ITEM_OBJ_DISP:
		if (change_val) {
			dbg_pause_work->is_obj_disp_on	=
				((dbg_pause_work->is_obj_disp_on) ? FALSE : TRUE);
		}
		break;

	case GME_DBG_PAUSE_ITEM_SCREEN_UP:
		if (change_val) {
			dbg_pause_work->is_screen_up_on	=
				((dbg_pause_work->is_screen_up_on) ? FALSE : TRUE);
		}
		break;
	}
	
	// アクトIDクリップ
	switch (dbg_pause_work->zone) {
		case GSD_MAIN_ZONE_TYPE_FINAL:	// FINALゾーン
			dbg_pause_work->act	= MTM_MATH_CLIP(dbg_pause_work->act,
												0, 4);
			break;
		case GSD_MAIN_ZONE_TYPE_SS:		// スペステ
			dbg_pause_work->act	= MTM_MATH_CLIP(dbg_pause_work->act,
												0, 6);
			break;
		default:	// 通常ステージ
			dbg_pause_work->act	= MTM_MATH_CLIP(dbg_pause_work->act,
												0, 3);
		}
	
	// 描画
	gmDbgPauseDraw(dbg_pause_work);
}


// =======================================================================
// gmDbgPauseDraw
/*!
  デバッグポーズ 描画
  
  @param dbg_pause_work	[in]	デバッグポーズワーク
 */
// =======================================================================
void gmDbgPauseDraw(GMS_DBG_PAUSE_WORK *dbg_pause_work)
{
	Sint32	pos_x	= GMD_DBG_PAUSE_DRAW_POS_X;
	Sint32	pos_y	= GMD_DBG_PAUSE_DRAW_POS_Y;
	
	Sint32	zone_id	= dbg_pause_work->zone;
	Sint32	act_id	= dbg_pause_work->act;
	
	
	// タイトル
	amPrintf(pos_x - 1, pos_y -1,
			 "|------- Debug Pause Menu -------|");
	
	// ゾーン番号
	amPrintf(pos_x, pos_y, "ZONE        : %s", gm_dbg_pause_zone_name[gm_dbg_pause_zone_top[zone_id] + act_id]);
	
	pos_y++;
	
	// アクト番号
	amPrintf(pos_x, pos_y, "ACT         : %s", gm_dbg_pause_act_name[gm_dbg_pause_zone_top[zone_id] + act_id]);
	
	pos_y++;
	
	// 無敵
	amPrintf(pos_x, pos_y, "INVINCIBLE  : %s", gm_dbg_pause_string_onoff[dbg_pause_work->is_invincible]);
	
	pos_y++;
	
	// 矩形表示
	amPrintf(pos_x, pos_y, "RECT        : %s", gm_dbg_pause_string_rect[dbg_pause_work->rect_disp_type]);
	
	pos_y++;
	
	// プレイヤー座標表示タイプ
	amPrintf(pos_x, pos_y, "PLY_POS_DSP : %s", gm_dbg_pause_string_ply_pos_disp[dbg_pause_work->ply_pos_disp_type]);
	
	pos_y++;
	
	// リング数
	amPrintf(pos_x, pos_y, "RING        : %d", dbg_pause_work->ring_num);
	
	pos_y++;
	
	// タイム
	{
		Uint16	min;
		Uint16	sec;
		Uint16	msec;
		const static char* focus_string[GME_DBG_PAUSE_TIMER_FOCUS_MAX]	= {
			"min",
			"sec",
		};
		
		AkUtilFrame60ToTime((Uint32)dbg_pause_work->cur_time, &min, &sec, &msec);
		amPrintf(pos_x, pos_y, "TIME[%s]   : %01d'%02d''%02d",
				 focus_string[dbg_pause_work->time_focus_h],
				 min, sec, msec);
	}
	
	pos_y++;
	
	// スコア
	amPrintf(pos_x, pos_y, "SCORE       : %09d  [step=%d]",
			 dbg_pause_work->score,
			 (Sint32)nnPow(10, dbg_pause_work->score_inc_size));
	
	pos_y++;
	
	// チャレンジ数
	amPrintf(pos_x, pos_y, "CHALLENGE   : %04d  [step=%d]",
			 dbg_pause_work->challenge_num,
			 (Sint32)nnPow(10, dbg_pause_work->challenge_inc_size));
	
	pos_y++;
	
	// デバッグ表示
	amPrintf(pos_x, pos_y, "DEBUG DISP  : %s", gm_dbg_pause_string_onoff[dbg_pause_work->is_debug_disp_on]);
	
	pos_y++;
	
	// FIX表示
	amPrintf(pos_x, pos_y, "FIX DISP    : %s", gm_dbg_pause_string_onoff[dbg_pause_work->is_fix_disp_on]);
	
	pos_y++;
	
	// イベント復旧
	amPrintf(pos_x, pos_y, "EVENT RESET : %s", gm_dbg_pause_string_onoff[dbg_pause_work->is_eve_reset_on]);

	pos_y++;

	// プレイヤー表示
	amPrintf(pos_x, pos_y, "PLAYER DISP : %s", gm_dbg_pause_string_onoff[dbg_pause_work->is_player_disp_on]);
	
	pos_y++;
	
	// デバッグチェック設定
	amPrintf(pos_x, pos_y, "DBG CHECK   : %s", gm_dbg_pause_string_debug_check_mode[dbg_pause_work->debug_check_mode]);

	pos_y++;

	// マップ表示
	amPrintf(pos_x, pos_y, "MAP DISP    : %s", gm_dbg_pause_string_onoff[dbg_pause_work->is_map_disp_on]);
	
	pos_y++;

	// オブジェクト表示
	amPrintf(pos_x, pos_y, "OBJ DISP    : %s", gm_dbg_pause_string_onoff[dbg_pause_work->is_obj_disp_on]);
	
	pos_y++;

#if _IPHONE
	// オブジェクト表示
	amPrintf(pos_x, pos_y, "SCREEN UP   : %s", gm_dbg_pause_string_onoff[dbg_pause_work->is_screen_up_on]);
	
	pos_y++;
#endif // _IPHONE

	// 説明
	pos_y	+= 5;
	amPrintf(pos_x, pos_y, "UP/DOWN    : Move Cursor");
	pos_y++;
	amPrintf(pos_x, pos_y, "LEFT/RIGHT : Toggle Mode,etc");
	pos_y++;
	amPrintf(pos_x, pos_y, "X/Y        : Change Value");
	pos_y++;
	amPrintf(pos_x, pos_y, "A          : Decide");
	pos_y++;
	amPrintf(pos_x, pos_y, "Start/B    : Exit");
	
	
	// カーソル
	amPrintf(GMD_DBG_PAUSE_DRAW_POS_X - 1,
			 GMD_DBG_PAUSE_DRAW_POS_Y + dbg_pause_work->select_item,
			 ">");
}

// =======================================================================
// gmDbgPauseGetDebugStatus
/*!
  デバッグポーズ ステータス取得
  
  @param dbg_pause_work	[io]	デバッグポーズワーク
 */
// =======================================================================
void gmDbgPauseGetDebugStatus(GMS_DBG_PAUSE_WORK *dbg_pause_work)
{
	Sint32	act_id	= 0;
	
	// ゾーンID取得
	dbg_pause_work->zone	= g_gm_gamedat_zone_type_tbl[GsGetMainSysInfo()->stage_id];
	
	// アクトID取得
	for (Sint32 i = GSD_MAIN_ZONE_TYPE_MAX-1; i >= 0; --i) {
		Sint32	stage_id	= GsGetMainSysInfo()->stage_id;
		
		act_id	= stage_id - gm_dbg_pause_zone_top[i];
		
		if (act_id >= 0) {
			break;
		}
	}
	dbg_pause_work->act	= act_id;
	
	// 無敵状態取得
#if 0	// TODO : デバッグ無敵が未実装なので無効化
	if (GsGetMainSysInfo()->debug_flag & GSD_DEBUG_NODAMAGE) {
		dbg_pause_work->is_invincible	= TRUE;
	}
	else {
		dbg_pause_work->is_invincible	= FALSE;
	}
#endif
	
	// 矩形表示タイプ取得
	{
		Uint32	dbg_rect_flag	= g_obj.flag & (OBD_OBJ_RECT_D | OBD_OBJ_RECTF_D);
		
		switch (dbg_rect_flag) {
		case 0:
			dbg_pause_work->rect_disp_type	= GME_DBG_PAUSE_RECT_DISP_TYPE_OFF;
			break;
		case OBD_OBJ_RECT_D:
			dbg_pause_work->rect_disp_type	= GME_DBG_PAUSE_RECT_DISP_TYPE_HIT;
			break;
		case OBD_OBJ_RECTF_D:
			dbg_pause_work->rect_disp_type	= GME_DBG_PAUSE_RECT_DISP_TYPE_FIELD;
			break;
		case (OBD_OBJ_RECT_D | OBD_OBJ_RECTF_D):
			dbg_pause_work->rect_disp_type	= GME_DBG_PAUSE_RECT_DISP_TYPE_BOTH;
			break;
		}
	}
	
	// プレイヤー座標表示タイプ
	{
		Uint32	dbg_ply_pos_disp_flag	= g_gm_main_system.debug_flag & (GMD_GAME_DEBUG_FLAG_PLY_POS_16 | GMD_GAME_DEBUG_FLAG_PLY_POS_10F);
		
		switch (dbg_ply_pos_disp_flag) {
		case 0:
			dbg_pause_work->ply_pos_disp_type	= GME_DBG_PAUSE_PLY_POS_DISP_TYPE_10I;
			break;
		case GMD_GAME_DEBUG_FLAG_PLY_POS_16:
			dbg_pause_work->ply_pos_disp_type	= GME_DBG_PAUSE_PLY_POS_DISP_TYPE_16I;
			break;
		case GMD_GAME_DEBUG_FLAG_PLY_POS_10F:
			dbg_pause_work->ply_pos_disp_type	= GME_DBG_PAUSE_PLY_POS_DISP_TYPE_10F;
			break;
		}
	}
	
	// リング数取得
	if (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]) {
		dbg_pause_work->ring_num	= g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->ring_num;
	}
	
	// タイム取得
	dbg_pause_work->cur_time	= (Sint32)g_gm_main_system.game_time;
	
	// スコア取得
	if (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]) {
		dbg_pause_work->score	= (Sint32)g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->score;
	}
	else {
		// プレイヤー生成されてないときは0を設定
		dbg_pause_work->score	= 0;
	}
	
	// チャレンジ数取得
	dbg_pause_work->challenge_num	= (Sint32)g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P];
	
	// デバッグ表示状態取得
	if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_DEBUG_DISP) {
		dbg_pause_work->is_debug_disp_on	= TRUE;
	}
	else {
		dbg_pause_work->is_debug_disp_on	= FALSE;
	}
	
	// FIX表示状態取得
	if (GmFixIsDisp()) {
		dbg_pause_work->is_fix_disp_on	= TRUE;
	}
	else {
		dbg_pause_work->is_fix_disp_on	= FALSE;
	}

	// プレイヤー表示状態取得
	if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_PLY_NO_DISP) {
		dbg_pause_work->is_player_disp_on	= FALSE;
	}
	else {
		dbg_pause_work->is_player_disp_on	= TRUE;
	}
	
	// デバッグチェックモード取得
	dbg_pause_work->debug_check_mode	= g_gm_main_system.debug_check_mode;

	// マップ表示状態取得
	if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_MAP_NO_DISP) {
		dbg_pause_work->is_map_disp_on	= FALSE;
	}
	else {
		dbg_pause_work->is_map_disp_on	= TRUE;
	}

	// オブジェクト表示状態取得
	if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_OBJ_NO_DISP) {
		dbg_pause_work->is_obj_disp_on	= FALSE;
	}
	else {
		dbg_pause_work->is_obj_disp_on	= TRUE;
	}
#if _IPHONE
	// 画面強制アップ状態
	if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_UP_SCREEN) {
		dbg_pause_work->is_screen_up_on= TRUE;
	}
	else {
		dbg_pause_work->is_screen_up_on	= FALSE;
	}
#endif // _IPHONE
}

// =======================================================================
// gmDbgPauseSetDebugStatus
/*!
  デバッグポーズ ステータス設定
 
  @param dbg_pause_work	[io]	デバッグポーズワーク
 */
// =======================================================================
void gmDbgPauseSetDebugStatus(GMS_DBG_PAUSE_WORK *dbg_pause_work)
{
	// 無敵状態設定
#if 0	// TODO : デバッグ無敵が未実装なので無効化
	if (dbg_pause_work->is_invincible) {
		GsGetMainSysInfo()->debug_flag	|= GSD_DEBUG_NODAMAGE;
	}
	else {
		GsGetMainSysInfo()->debug_flag	&= ~GSD_DEBUG_NODAMAGE;
	}
#endif
	
	// 矩形表示タイプ設定
	{
		g_obj.flag	&= ~(OBD_OBJ_RECT_D | OBD_OBJ_RECTF_D);
		
		switch (dbg_pause_work->rect_disp_type) {
		case GME_DBG_PAUSE_RECT_DISP_TYPE_OFF:
			// 何もしない
			break;
		case GME_DBG_PAUSE_RECT_DISP_TYPE_HIT:
			g_obj.flag	|= OBD_OBJ_RECT_D;
			break;
		case GME_DBG_PAUSE_RECT_DISP_TYPE_FIELD:
			g_obj.flag	|= OBD_OBJ_RECTF_D;
			break;
		case GME_DBG_PAUSE_RECT_DISP_TYPE_BOTH:
			g_obj.flag	|= (OBD_OBJ_RECT_D | OBD_OBJ_RECTF_D);
			break;
		}
	}
	
	// プレイヤー座標表示タイプ
	{
		g_gm_main_system.debug_flag	&= ~(GMD_GAME_DEBUG_FLAG_PLY_POS_16 | GMD_GAME_DEBUG_FLAG_PLY_POS_10F);
		
		switch (dbg_pause_work->ply_pos_disp_type) {
		case GME_DBG_PAUSE_PLY_POS_DISP_TYPE_10I:
			// 何もしない
			break;
		case GME_DBG_PAUSE_PLY_POS_DISP_TYPE_16I:
			g_gm_main_system.debug_flag	|= GMD_GAME_DEBUG_FLAG_PLY_POS_16;
			break;
		case GME_DBG_PAUSE_PLY_POS_DISP_TYPE_10F:
			g_gm_main_system.debug_flag	|= GMD_GAME_DEBUG_FLAG_PLY_POS_10F;
			break;
		}
	}
	
	// リング数設定
	if (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]) {
		g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->ring_num	= dbg_pause_work->ring_num;
	}
	
	// タイム設定
	g_gm_main_system.game_time	= (Uint32)dbg_pause_work->cur_time;
	
	// スコア設定
	if (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]) {
		g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->score	= (Uint32)dbg_pause_work->score;
	}
	
	// チャレンジ数設定
	g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P]	= (Uint32)dbg_pause_work->challenge_num;
	
	// デバッグ表示設定
	if (dbg_pause_work->is_debug_disp_on) {
		g_gs_main_sys_info.debug_flag	|= GSD_DEBUG_DEBUG_DISP;
		_am_fs_display_mode = 1;
	}
	else {
		g_gs_main_sys_info.debug_flag	&= ~GSD_DEBUG_DEBUG_DISP;
		_am_fs_display_mode = 0;
	}
	
	// FIX表示設定
	if (dbg_pause_work->is_fix_disp_on) {
		GmFixSetDisp(TRUE);
	}
	else {
		GmFixSetDisp(FALSE);
	}

	// イベント復旧
	if (dbg_pause_work->is_eve_reset_on) {
		GmEventDataFlush();
		GmEventDataBuild();
	}

	// プレイヤー表示
	if (dbg_pause_work->is_player_disp_on) {
		g_gm_main_system.debug_flag &= ~GMD_GAME_DEBUG_FLAG_PLY_NO_DISP;
	}
	else {
		g_gm_main_system.debug_flag |= GMD_GAME_DEBUG_FLAG_PLY_NO_DISP;
	}
	
	// デバッグチェックモード
	g_gm_main_system.debug_check_mode	= dbg_pause_work->debug_check_mode;

	// 以下、決定ボタンが押されたときのみ反映
	if (dbg_pause_work->flag & GMD_DBG_PAUSE_FLAG_DECIDED) {
		
		if (dbg_pause_work->select_item == GME_DBG_PAUSE_ITEM_MAP_DISP) {
			
			// マップ表示
			if (dbg_pause_work->is_map_disp_on) {
				g_gm_main_system.debug_flag &= ~GMD_GAME_DEBUG_FLAG_MAP_NO_DISP;
			}
			else {
				g_gm_main_system.debug_flag |= GMD_GAME_DEBUG_FLAG_MAP_NO_DISP;
			}
		}
		
		// オブジェクト表示
		if (dbg_pause_work->select_item == GME_DBG_PAUSE_ITEM_OBJ_DISP) {
			OBS_OBJECT_WORK	*obj_work;
			if (dbg_pause_work->is_obj_disp_on) {
				g_gm_main_system.debug_flag &= ~GMD_GAME_DEBUG_FLAG_OBJ_NO_DISP;
				
				obj_work = ObjObjectSearchRegistObject(NULL, 0xFFFF);
				while (obj_work) {
					if (obj_work->obj_type != GMD_OBJTYPE_PLAYER &&
						obj_work->obj_type != GMD_OBJTYPE_MAPFAR &&
						obj_work->obj_type != GMD_OBJTYPE_COCKPIT) {
						obj_work->disp_flag &= ~OBD_DISP_NODISP;
					}
					obj_work = ObjObjectSearchRegistObject(obj_work, 0xFFFF);
				}
				
				// リング表示
				GmRingGetWork()->flag &= ~GMD_RING_SYS_FLAG_NODISP;
			}
			else {
				g_gm_main_system.debug_flag |= GMD_GAME_DEBUG_FLAG_OBJ_NO_DISP;
				
				obj_work = ObjObjectSearchRegistObject(NULL, 0xFFFF);
				while (obj_work) {
					if (obj_work->obj_type != GMD_OBJTYPE_PLAYER &&
						obj_work->obj_type != GMD_OBJTYPE_MAPFAR &&
						obj_work->obj_type != GMD_OBJTYPE_COCKPIT) {
						obj_work->disp_flag |= OBD_DISP_NODISP;
					}
					obj_work = ObjObjectSearchRegistObject(obj_work, 0xFFFF);
				}
				
				// リング非表示
				GmRingGetWork()->flag |= GMD_RING_SYS_FLAG_NODISP;
			}
		}

#if _IPHONE
		// 画面強制アップ状態
		if (dbg_pause_work->is_screen_up_on) {
			g_gm_main_system.debug_flag |= GMD_GAME_DEBUG_FLAG_UP_SCREEN;
		}
		else {
			g_gm_main_system.debug_flag &= ~GMD_GAME_DEBUG_FLAG_UP_SCREEN;
		}
#endif // _IPHONE
	}
}

// =======================================================================
// gmDbgPauseGetChange
/*!
  変更入力結果取得
  
  @retval -1	減少・前
  @retval 0		変更なし
  @retval 1		増加・次
 */
// =======================================================================
Sint32 gmDbgPauseGetChange(void)
{
	if (AoPadRepeat() & KEY_R_UP) {
		return -1;
	}
	else if (AoPadRepeat() & KEY_R_LEFT) {
		return 1;
	}
	else {
		return 0;
	}
}

// =======================================================================
// gmDbgPauseGetFocusMoveV
/*!
  上下フォーカス移動入力結果取得
  
  @retval -1	上
  @retval 0		変更無し
  @retval 1		下
 */
// =======================================================================
Sint32 gmDbgPauseGetFocusMoveV(void)
{
	if (AoPadRepeat() & KEY_L_UP) {
		return -1;
	}
	else if (AoPadRepeat() & KEY_L_DOWN) {
		return 1;
	}
	else {
		return 0;
	}
}

// =======================================================================
// gmDbgPauseGetFocusMoveH
/*!
  左右フォーカス移動入力結果取得
  
  @retval -1	左
  @retval 0		変更無し
  @retval 1		右
 */
// =======================================================================
Sint32 gmDbgPauseGetFocusMoveH(void)
{
	if (AoPadRepeat() & KEY_L_LEFT) {
		return -1;
	}
	else if (AoPadRepeat() & KEY_L_RIGHT) {
		return 1;
	}
	else {
		return 0;
	}
}

#endif /* defined(MTD_DEBUG) */

// =======================================================================
// test_func
/*!
  関数機能
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return 返値説明
 
  @note
  補足説明
 */
// =======================================================================
