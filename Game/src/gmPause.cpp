// =======================================================================
/*!
  @file	gmPause.cpp
  @brief ポーズ

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmPause.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "gmMain.h"
#include "gmPauseMenu.h"
#include "gsSound.h"
#include "gmSound.h"
#include "dmOption.h"
#include "dmRanking.h"
#include "aoTrophy.h"
#include "izFade.h"
#include "gmPlayer.h"

#include "gmPause.h"


#if defined(MTD_DEBUG)
#include "gmDebugPause.h"
#if _IPHONE
#include "dbgPadEmu.hpp"
#endif //_IPHONE
#endif /* defined(MTD_DEBUG) */

// mpp ---------------------------------
#include "mppCheckPointStorage.h"
#include "mppAchievementSupport.h"

/*------ Macros --------------------------------------------------------*/
/* フラグ */
#define GMD_PAUSE_FLAG_END			(1 << 0)		//!< 終了フラグ

/* 定義値 */
#define GMD_PAUSE_FADEOUT_TIME		(20)			//!< フェードアウト時間
#define GMD_PAUSE_FADEIN_TIME		(20)			//!< フェードイン時間

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! ポーズワーク
typedef struct tag_GMS_PAUSE_WORK
{
	Uint32	flag;
	
	// 更新処理
	void (*proc_update)(struct tag_GMS_PAUSE_WORK*);
	
	//! g_gm_main_system.game_flag タイムカウント系フラグのポーズ直前の状態を保存
	Uint32	time_count_flag_save;
} GMS_PAUSE_WORK;

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
static void gmPauseDest(MTS_TASK_TCB *tcb);
static void gmPauseMain(MTS_TASK_TCB *tcb);
static void gmPauseExecRecoverRoutine(GMS_PAUSE_WORK *pause_work, BOOL b_rec_snd);
// 更新シーケンス
static void gmPauseProcUpdateInit(GMS_PAUSE_WORK *pause_work);
static void gmPauseProcUpdateReinit(GMS_PAUSE_WORK *pause_work);
static void gmPauseProcUpdatePauseMenuStart(GMS_PAUSE_WORK *pause_work);
static void gmPauseProcUpdateWaitDecision(GMS_PAUSE_WORK *pause_work);
#if !_WII
static void gmPauseProcUpdateFadeOutToOption(GMS_PAUSE_WORK *pause_work);
#endif /* !_WII */
#if _PC || _XBOX
static void gmPauseProcUpdateFadeOutToRanking(GMS_PAUSE_WORK *pause_work);
#endif /* _PC || _XBOX */
static void gmPauseProcUpdateWaitRecover(GMS_PAUSE_WORK *pause_work);
static void gmPauseProcUpdateFadeInFromDemo(GMS_PAUSE_WORK *pause_work);
static void gmPauseProcUpdateFadeOutToExitGame(GMS_PAUSE_WORK *pause_work);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
//! ポーズタスクTCB
static MTS_TASK_TCB *gm_pause_tcb	= NULL;

/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GmPauseInit
/*!
  ポーズ初期化
 */
// =======================================================================
void GmPauseInit(void)
{
	GMS_PAUSE_WORK	*pause_work;
	Uint32	time_count_flag_save	= 0;
	
	MTM_ASSERT(gm_pause_tcb == NULL && "gmPause.cpp::GmPauseInit() already executed\n");
	
#if defined(MTD_DEBUG)
	// パッドが抜けていたり、システムUI表示中ならデバッグポーズは起動しない
	if (AoPadIsConnected() && !AoSysIsShowPlatformUI()) {
		// L1を同時に押してたらデバッグポーズを起動
#if _WII
		if (AoPadDirect() & KEY_R1) {	// Wii版はCを押していたら起動
#elif _IPHONE
	}{ //直前のif文を無効化
		if (AoPadDirect() & KEY_R2) {	// iPhone版は右下を押していたら起動
#else
		if (AoPadDirect() & KEY_L1) {
#endif /* _WII */
			// デバッグポーズ初期化
			GmDbgPauseInit();
			return;
		}
	}
#endif /* defined(MTD_DEBUG) */
	
#if defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	UNREFERENCED_PARAMETER(pause_work);
	// コックピット無効の時は何もしない
	return;
#endif /* defined(GMD_DEBUG_NO_CREATE_COCKPIT) */
	
	// サウンドは１フレーム後に一時停止
	;
	
	// オブジェクトポーズ開始
	ObjObjectPause(GMD_TASK_GAME_PAUSE_LEVEL);
	
	// ポーズ演出開始
	g_gm_main_system.game_flag	|= GMD_GAME_FLAG_PAUSE_DEMO;
	g_gm_main_system.game_flag	&= ~GMD_GAME_FLAG_PAUSE_IS_DECIDED;
	
	// タイムカウント系フラグを退避
	time_count_flag_save	= g_gm_main_system.game_flag & (GMD_GAME_FLAG_COUNT_GAME_TIME |
															GMD_GAME_FLAG_COUNT_SYNC_TIME);
	
	// ゲームタイマ、同期タイマ停止
	g_gm_main_system.game_flag	&= ~(GMD_GAME_FLAG_COUNT_GAME_TIME |
									 GMD_GAME_FLAG_COUNT_SYNC_TIME);
	
	// ポーズタスク生成
	gm_pause_tcb	= MTM_TASK_MAKE_TCB(gmPauseMain,
										gmPauseDest,
										0,//flag
										GMD_TASK_NO_GAME_PAUSE,
										GMD_TASK_PRIO_PAUSE,
										GMD_TASK_GROUP_PAUSE,
										sizeof(GMS_PAUSE_WORK),
										"GM_PAUSE");
	
	// ワーク初期化
	pause_work	= (GMS_PAUSE_WORK*)mtTaskGetTcbWork(gm_pause_tcb);
	amZeroMemory(pause_work, sizeof(GMS_PAUSE_WORK));
	
	// 退避したタイムカウント系フラグをワークに保存
	pause_work->time_count_flag_save	= time_count_flag_save;
	
	// シーケンス初期化
	gmPauseProcUpdateInit(pause_work);
}

// =======================================================================
// GmPauseExit
/*!
  ポーズ終了
 */
// =======================================================================
void GmPauseExit(void)
{
	if (!GmPauseMenuIsFinished()) {
		GmPauseMenuCancel();
	}
	
	if (gm_pause_tcb) {
		mtTaskClearTcb(gm_pause_tcb);
		gm_pause_tcb	= NULL;
	}
}


// =======================================================================
// GmPauseCheckExecutable
/*!
  ポーズ実行可能チェック
  
  @retval TRUE	ポーズ開始可
  @retval FALSE ポーズ開始不可
 */
// =======================================================================
BOOL GmPauseCheckExecutable(void)
{
	if (gm_pause_tcb == NULL) {
		if (!(g_gm_main_system.game_flag & GMD_GAME_FLAG_PAUSE_WAIT_MASK) &&
				(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P] &&
					!(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->player_flag & GMD_PLF_DIE))) {
			return TRUE;
		}
	}
	
	return FALSE;
}

/*------ Static Functions ----------------------------------------------*/

// =======================================================================
// gmPauseDest
/*!
  ポーズタスク終了処理
 */
// =======================================================================
void gmPauseDest(MTS_TASK_TCB *tcb)
{
	GMS_PAUSE_WORK	*pause_work	= (GMS_PAUSE_WORK*)mtTaskGetTcbWork(tcb);
	
	// ゲームタイマ、同期タイマ再開
#if 1	// 単純にタイマを再開するのではなく、ポーズ直前の状態に戻す
	g_gm_main_system.game_flag	|= (pause_work->time_count_flag_save &
									(GMD_GAME_FLAG_COUNT_GAME_TIME |
									 GMD_GAME_FLAG_COUNT_SYNC_TIME));
#else
	g_gm_main_system.game_flag	|= (GMD_GAME_FLAG_COUNT_GAME_TIME |
									GMD_GAME_FLAG_COUNT_SYNC_TIME);
#endif /* 1 */
	
	// ポーズ演出終了
	g_gm_main_system.game_flag	&= ~GMD_GAME_FLAG_PAUSE_DEMO;
	
	gm_pause_tcb	= NULL;
}

// =======================================================================
// gmPauseMain
/*!
  ポーズタスク メイン処理
 */
// =======================================================================
void gmPauseMain(MTS_TASK_TCB *tcb)
{
	GMS_PAUSE_WORK	*pause_work	= (GMS_PAUSE_WORK*)mtTaskGetTcbWork(tcb);
	
	MTM_ASSERT(pause_work);
	
	// 更新処理
	if (pause_work->proc_update) {
		pause_work->proc_update(pause_work);
	}
	
	// 終了チェック
	if (pause_work->flag & GMD_PAUSE_FLAG_END) {
		mtTaskClearTcb(tcb);
	}
}

// =======================================================================
// gmPauseExecRecoverRoutine
/*!
  復帰時共通処理
  
  @param pause_work	[io]	ポーズワーク
  @param b_rec_snd	[in]	サウンド復帰フラグ
 */
// =======================================================================
void gmPauseExecRecoverRoutine(GMS_PAUSE_WORK *pause_work, BOOL b_rec_snd)
{
	// オブジェクトポーズ終了
	ObjObjectPauseOut();
	
	// 終了通知
	pause_work->flag	|= GMD_PAUSE_FLAG_END;
	
	if (b_rec_snd) {
		// サウンド再開
		GmSoundAllResume();
	}
}

// ============================================================================
// 更新シーケンス
// ============================================================================
// =======================================================================
// gmPauseProcUpdate****
/*!
  更新シーケンス
 */
// =======================================================================
// シーケンス初期化
void gmPauseProcUpdateInit(GMS_PAUSE_WORK *pause_work)
{
	// 処理関数設定
	pause_work->proc_update	= gmPauseProcUpdatePauseMenuStart;
}

// シーケンス再初期化（各種メニューから戻ってきたときの初期化）
void gmPauseProcUpdateReinit(GMS_PAUSE_WORK *pause_work)
{
	// サウンドは一時停止済み
	
	// ポーズメニュー生成
	GmPauseMenuStart(GMD_TASK_PRIO_PAUSE);
	
	// 処理関数設定
	pause_work->proc_update	= gmPauseProcUpdateWaitDecision;
}

// ポーズメニュー開始
void gmPauseProcUpdatePauseMenuStart(GMS_PAUSE_WORK *pause_work)
{
	// サウンドを一時停止
	// ObjObjectPause()を呼び出した後、
	// その同じフレームでobjMain()が非ポーズ状態で実行されるため、
	// 当たり処理などでサウンドが鳴らされた場合に備えて、
	// このタイミング（ObjObjectPause()呼び出しの次のフレーム）でサウンドポーズをかける
	GmSoundAllPause();
	
	// ポーズメニュー生成
	GmPauseMenuStart(GMD_TASK_PRIO_PAUSE);
	
	// 処理関数設定
	pause_work->proc_update	= gmPauseProcUpdateWaitDecision;
}


// シーケンス更新 結果待ち
void gmPauseProcUpdateWaitDecision(GMS_PAUSE_WORK *pause_work)
{
	
	
	if (GmPauseMenuIsFinished()) {
		GME_PMENU_RESULT	result	= GmPauseMenuGetResult();
		BOOL				is_fade_out	= FALSE;
		
		switch (result) {
#if !_WII
		case GME_PMENU_RESULT_OPTION:
			is_fade_out	= TRUE;
			pause_work->proc_update	= gmPauseProcUpdateFadeOutToOption;
			break;
#endif /* !_WII */
			
#if _PC || _XBOX
		case GME_PMENU_RESULT_RANKING:
			is_fade_out	= TRUE;
			pause_work->proc_update	= gmPauseProcUpdateFadeOutToRanking;
			break;
			
		
#endif /* _PC || _XBOX */
			
#if _XBOX
		case GME_PMENU_RESULT_TROPHY:
			AoTrophyShowAchievementUI();
			pause_work->proc_update	= gmPauseProcUpdateWaitRecover;
			break;
#endif /* _XBOX */
			
		case GME_PMENU_RESULT_RETRY:	// no break
		case GME_PMENU_RESULT_BACK:		// no break
		case GME_PMENU_RESULT_MAINMENU:
			{{//qqq
				if(result == GME_PMENU_RESULT_RETRY /*|| result == GME_PMENU_RESULT_MAINMENU*/) {
					OS_TPrintf("try to clear saved state from PAUSE for RETRY...\n");
					mppCheckPointStorage::removeState();
				}
				if(result == GME_PMENU_RESULT_MAINMENU) {
					mpp_flushAchievementsToGlobalNet(true); //on exit from pause to main menu
				}
			}}
			is_fade_out	= TRUE;
			pause_work->proc_update	= gmPauseProcUpdateFadeOutToExitGame;
			break;
			
		case GME_PMENU_RESULT_CANCEL:	// no break
		case GME_PMENU_RESULT_NONE:		// no break
		default:
			// 復帰時処理
			gmPauseExecRecoverRoutine(pause_work, TRUE);
			
			pause_work->proc_update	= NULL;
			
			break;
		}
		
		// フェードアウト初期化
		if (is_fade_out) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF,
								0x7fff,
								IZD_FADE_DT_PRIO_DEF,
								IZD_FADE_DRAW_STATE_DEF,
								IZE_FADE_SET_TYPE_NORMAL,
								IZE_FADE_TYPE_BLACK_FADEOUT,
								GMD_PAUSE_FADEOUT_TIME);
		}
	}
}

#if !_WII
// シーケンス更新 オプションへフェードアウト
void gmPauseProcUpdateFadeOutToOption(GMS_PAUSE_WORK *pause_work)
{
	if (IzFadeIsEnd()) {
		// フェード完了後、オプション画面初期化＆復帰待ちへ
		DmOptionStart(NULL);
		pause_work->proc_update	= gmPauseProcUpdateWaitRecover;
	}
}
#endif /* !_WII */

#if _PC || _XBOX
// シーケンス更新 ランキングへフェードアウト
void gmPauseProcUpdateFadeOutToRanking(GMS_PAUSE_WORK *pause_work)
{
	if (IzFadeIsEnd()) {
		// フェード完了後、ランキング画面初期化＆復帰待ちへ
		DmRankingStart(NULL);
		pause_work->proc_update	= gmPauseProcUpdateWaitRecover;
	}
}
#endif /* _PC || _XBOX */

// シーケンス更新 ポーズメニュー復帰待ち
void gmPauseProcUpdateWaitRecover(GMS_PAUSE_WORK *pause_work)
{
	BOOL	result	= TRUE;
	
#if !_WII
	// オプション終了チェック
	if (!DmOptionIsExit()) {
		result	= FALSE;
	}
#endif /* !_WII */
	
#if _PC || _XBOX
	// ランキング終了チェック
	if (!DmRankingIsExit()) {
		result	= FALSE;
	}
#endif /* _PC || _XBOX */
	
#if _XBOX
	// 実績終了チェック
	if (!AoTrophyIsHideAchievementUI()) {
		result	= FALSE;
	}
#endif /* _XBOX */
	
	if (result) {
		// フェードイン初期化
		IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF,
							0x7fff,
							IZD_FADE_DT_PRIO_DEF,
							IZD_FADE_DRAW_STATE_DEF,
							IZE_FADE_SET_TYPE_TAKEOEVER,
							IZE_FADE_TYPE_BLACK_FADEIN,
							GMD_PAUSE_FADEIN_TIME);
		
		// ポーズメニューへ復帰
		pause_work->proc_update	= gmPauseProcUpdateFadeInFromDemo;
	}
}

// シーケンス更新 各デモからのフェードイン
void gmPauseProcUpdateFadeInFromDemo(GMS_PAUSE_WORK *pause_work)
{
	if (IzFadeIsEnd()) {
		
		// フェード終了
		IzFadeExit();
		
		// 再度ポーズメニューを開く
		gmPauseProcUpdateReinit(pause_work);
	}
}

// メインゲーム退出時（ステセレ移行、リトライ）のフェードアウト
void gmPauseProcUpdateFadeOutToExitGame(GMS_PAUSE_WORK *pause_work)
{
	if (IzFadeIsEnd()) {
		// フェード完了後、決定後処理へ
		
		// 決定が行われた
		g_gm_main_system.game_flag	|= GMD_GAME_FLAG_PAUSE_IS_DECIDED;
		
		// 復帰時処理
		gmPauseExecRecoverRoutine(pause_work, FALSE);	// サウンド復帰しない
	}
}




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
