// ===========================================================================
/*!
	@file	dmSndBgmPlayer.cpp
	@brief	デモ・BGMプレイヤーモジュール

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: dmSndBgmPlayer.cpp 20 2011-04-22 12:46:46Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gsMainSys.h"
#include "gmTask.h"

#include "gsSound.h"
#include "dmSound.h"
#include "dmSndBgmPlayer.h"

//----- Definitions ---------------------------------------------------------
#define DMD_SND_BGM_PLAYER_TASK_PAUSELEVEL		(0)
#define DMD_SND_BGM_PLAYER_TASK_PRIO_MAIN		(0x1000)
#define DMD_SND_BGM_PLAYER_TASK_GROUP_MAIN		(0)

#define DMD_SND_BGM_PLAYER_BGM_FADEIN_TIME		(32)
#define DMD_SND_BGM_PLAYER_BGM_FADEOUT_TIME		(24)


// フラグ関連
#define DMD_SND_BGM_PLAYER_FLAG_EXIT					(1 << 0)		//!< 終了フラグ
#define DMD_SND_BGM_PLAYER_FLAG_CANCEL					(1 << 1)		//!< 
#define DMD_SND_BGM_PLAYER_FLAG_FINISH_BGM				(1 << 2)		//!< 
#define DMD_SND_BGM_PLAYER_FLAG_STOP_BGM				(1 << 3)
#define DMD_SND_BGM_PLAYER_FLAG_SET_TITLE_BGM			(1 << 4)
#define DMD_SND_BGM_PLAYER_FLAG_SET_MENU_BGM			(1 << 5)
#define DMD_SND_BGM_PLAYER_FLAG_CHANGE_TITLE_BGM		(1 << 6)
#define DMD_SND_BGM_PLAYER_FLAG_CHANGE_MENU_BGM			(1 << 7)







typedef struct tag_DMS_SND_BGM_PLAYER_MAIN_WORK	DMS_SND_BGM_PLAYER_MAIN_WORK;

//! メインタスクワーク
struct tag_DMS_SND_BGM_PLAYER_MAIN_WORK {
	void (*proc_update)(DMS_SND_BGM_PLAYER_MAIN_WORK *);	//!< フロー処理プロシージャ
	
	u32	flag;											//!< 汎用フラグ
	s32 end_timer;
};


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

static void dmSndBgmPlayerInit(void);
static void dmSndBgmPlayerProcMain(MTS_TASK_TCB *tcb);
static void dmSndBgmPlayerDest(MTS_TASK_TCB *tcb);

static void dmSndBgmPlayerProcInit(DMS_SND_BGM_PLAYER_MAIN_WORK *main_work);
static void dmSndBgmPlayerProcBuildIdle(DMS_SND_BGM_PLAYER_MAIN_WORK *main_work);
static void dmSndBgmPlayerProcWaitSetBgm(DMS_SND_BGM_PLAYER_MAIN_WORK *main_work);
static void dmSndBgmPlayerProcPlayIdle(DMS_SND_BGM_PLAYER_MAIN_WORK *main_work);
static void dmSndBgmPlayerProcStopIdle(DMS_SND_BGM_PLAYER_MAIN_WORK *main_work);
static void dmSndBgmPlayerProcSndRelease(DMS_SND_BGM_PLAYER_MAIN_WORK *main_work);
static void dmSndBgmPlayerProcSndFinish(DMS_SND_BGM_PLAYER_MAIN_WORK *main_work);


//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
// デモBGMプレイヤーTCBポインタ
static MTS_TASK_TCB	*dm_snd_bgm_player_tcb = NULL;
static u32 dm_snd_bgm_player_flag = 0;



//----- Global Functions ----------------------------------------------------
// ==========================================================================
// DmSndBgmPlayerInit
/*!
 *	ゲームサウンド初期化
 */
// ==========================================================================
void DmSndBgmPlayerInit(void)
{
	// 初期化処理
	dmSndBgmPlayerInit();
}


// ==========================================================================
// DmSndBgmPlayerExit
/*!
 *	ゲームサウンド終了処理
 */
// ==========================================================================
void DmSndBgmPlayerExit(void)
{
	dm_snd_bgm_player_flag |= DMD_SND_BGM_PLAYER_FLAG_FINISH_BGM;
}


// ==========================================================================
// DmSndBgmPlayerBgmStop
/*!
 *	ゲームサウンド停止処理
 */
// ==========================================================================
void DmSndBgmPlayerBgmStop(void)
{
	dm_snd_bgm_player_flag |= DMD_SND_BGM_PLAYER_FLAG_STOP_BGM;
}


// ==========================================================================
// DmSndBgmPlayerIsTaskExit
/*!
 *	ゲームサウンド終了チェック
 */
// ==========================================================================
BOOL DmSndBgmPlayerIsTaskExit(void)
{
	if (dm_snd_bgm_player_tcb) {
		return FALSE;
	}
	
	return TRUE;
}



// ==========================================================================
// DmSndBgmPlayerIsSndSysBuild
/*!
 *	ゲームサウンド構築終了チェック
 */
// ==========================================================================
BOOL DmSndBgmPlayerIsSndSysBuild(void)
{
	if (DmSoundBuildCheck()) {
		return TRUE;
	}
	
	return FALSE;
}



// ==========================================================================
// DmSndBgmPlayerPlayBgm
/*!
 *	ゲームサウンド再生
 */
// ==========================================================================
void DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX idx)
{
	// 再生時にモジュールが削除されていたら生成
	if (!dm_snd_bgm_player_tcb) {
		dmSndBgmPlayerInit();
	}
	
	if (idx == DME_SND_BGM_PLAYER_IDX_MENU) {
		if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_SET_TITLE_BGM) {
			dm_snd_bgm_player_flag |= DMD_SND_BGM_PLAYER_FLAG_CHANGE_MENU_BGM;
		}
		else if (!(dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_SET_MENU_BGM)) {
			dm_snd_bgm_player_flag |= DMD_SND_BGM_PLAYER_FLAG_SET_MENU_BGM;
		}
	}
	
	else if (idx == DME_SND_BGM_PLAYER_IDX_TITLE) {
		if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_SET_MENU_BGM) {
			dm_snd_bgm_player_flag |= DMD_SND_BGM_PLAYER_FLAG_CHANGE_TITLE_BGM;
		}
		else if (!(dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_SET_TITLE_BGM)) {
			dm_snd_bgm_player_flag |= DMD_SND_BGM_PLAYER_FLAG_SET_TITLE_BGM;
		}
	}
	
	
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// dmSndBgmPlayerInit
/*!
 *	ゲームサウンド初期化
 */
// ==========================================================================
void dmSndBgmPlayerInit(void)
{
	DMS_SND_BGM_PLAYER_MAIN_WORK	*main_work;
	
	// タスクが生きているかどうか
	if (dm_snd_bgm_player_tcb) {
		// タスクが生きていたら初期化処理はさせない
		return;
	}
	
	// メインタスク作成
	dm_snd_bgm_player_tcb = MTM_TASK_MAKE_TCB(dmSndBgmPlayerProcMain
											, dmSndBgmPlayerDest
											, 0
											, DMD_SND_BGM_PLAYER_TASK_PAUSELEVEL
											, DMD_SND_BGM_PLAYER_TASK_PRIO_MAIN
											, DMD_SND_BGM_PLAYER_TASK_GROUP_MAIN
											, sizeof(DMS_SND_BGM_PLAYER_MAIN_WORK)
											, "DM_SND_BGM_PLAYER_MAIN"
											);
	
	// ワーク初期化
	main_work = (DMS_SND_BGM_PLAYER_MAIN_WORK *)mtTaskGetTcbWork(dm_snd_bgm_player_tcb);
	
	// フラグクリア
	dm_snd_bgm_player_flag = 0;
	
	// プロシージャ設定
	main_work->proc_update = dmSndBgmPlayerProcInit;
}



// ==========================================================================
// dmSndBgmPlayerProcMain
/*!
 *	デモ・BGMプレイヤーメイン処理
 */
// ==========================================================================
void dmSndBgmPlayerProcMain(MTS_TASK_TCB *tcb)
{
	DMS_SND_BGM_PLAYER_MAIN_WORK	*main_work;

	// ワーク取得
	main_work = (DMS_SND_BGM_PLAYER_MAIN_WORK *)mtTaskGetTcbWork(tcb);

	// 終了処理
	if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_EXIT) {
		// タスククリア
		mtTaskClearTcb(tcb);
		
		// フラグクリア
		dm_snd_bgm_player_flag = 0;
		
		// イベント遷移用設定
		dm_snd_bgm_player_tcb = NULL;
	}
	
	// フロー処理用プロシージャ
	if (main_work->proc_update) {
		main_work->proc_update(main_work);
	}
}



// ==========================================================================
// dmSndBgmPlayerDest
/*!
 *	デストラクタ
 */
// ==========================================================================
void dmSndBgmPlayerDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
}



// ==========================================================================
// dmSndBgmPlayerProcInit
/*!
	初期化設定プロシージャ
 */
// ==========================================================================
void dmSndBgmPlayerProcInit(DMS_SND_BGM_PLAYER_MAIN_WORK *main_work)
{
	// サウンド構築
	DmSoundBuild();
	
	main_work->proc_update = dmSndBgmPlayerProcBuildIdle;
}



// ==========================================================================
// dmSndBgmPlayerProcBuildIdle
/*!
	システムビルド待ちプロシージャ
 */
// ==========================================================================
void dmSndBgmPlayerProcBuildIdle(DMS_SND_BGM_PLAYER_MAIN_WORK *main_work)
{
	// サウンド構築待ちチェック
	if (DmSoundBuildCheck()) {
		// サウンド初期化処理
		DmSoundInit();
		
		// プロシージャ切り替え
		main_work->proc_update = dmSndBgmPlayerProcWaitSetBgm;
		
		main_work->end_timer = 0;
	}
}



// ==========================================================================
// dmSndBgmPlayerProcWaitSetBgm
/*!
	BGM設定待ちプロシージャ
 */
// ==========================================================================
void dmSndBgmPlayerProcWaitSetBgm(DMS_SND_BGM_PLAYER_MAIN_WORK *main_work)
{
	GSS_MAIN_SYS_INFO	*gs_main = GsGetMainSysInfo();
	
	// BGM停止フラグチェック
	if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_STOP_BGM
		|| dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_FINISH_BGM) {
		// ジングル停止
		DmSoundStopJingle(DMD_SND_BGM_PLAYER_BGM_FADEOUT_TIME);
		
		// BGMフェードアウト開始
		DmSoundStopBGM(DMD_SND_BGM_PLAYER_BGM_FADEOUT_TIME);
		
		// プロシージャ切り替え(ボリュームフェードアウトへ)
		main_work->proc_update = dmSndBgmPlayerProcStopIdle;
		
		return;
	}
	
	
	// BGM設定待ちチェック
	if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_SET_TITLE_BGM
		|| dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_SET_MENU_BGM) {
		// サウンド初期化処理
//		DmSoundInit();
		
		if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_SET_TITLE_BGM) {
			// メインシステムのボリュームに初期値を設定
			gs_main->bgm_volume	= 1.0f;
			gs_main->se_volume	= 1.0f;
			
			// ボリューム初期化
			for (Sint32 i = 0; i < GSE_SND_TYPE_MAX; ++i) {
				GsSoundSetVolume((GSE_SND_TYPE)i, 1.f);
			}
			
			DmSoundPlayJingle(DME_SOUND_JINGLE_IDX_TITLE, 0);
			
			dm_snd_bgm_player_flag &= ~DMD_SND_BGM_PLAYER_FLAG_SET_MENU_BGM;
		}
		else {
			DmSoundPlayMenuBGM(DME_SOUND_BGM_IDX_MENU
							   , DMD_SND_BGM_PLAYER_BGM_FADEIN_TIME);
			
			dm_snd_bgm_player_flag &= ~DMD_SND_BGM_PLAYER_FLAG_SET_TITLE_BGM;
		}
		
		// プロシージャ切り替え
		main_work->proc_update = dmSndBgmPlayerProcPlayIdle;
		
		main_work->end_timer = 0;
		
		return;
	}
	
	// タイトルジングルへ切り替えフラグチェック
	else if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_CHANGE_TITLE_BGM) {
		// BGMフェードアウト開始
		DmSoundStopBGM(0);//DMD_SND_BGM_PLAYER_BGM_FADEOUT_TIME);
		
		// プロシージャ切り替え(ボリュームフェードアウトへ)
		main_work->proc_update = dmSndBgmPlayerProcStopIdle;
	}
	
	// メニューBGMへ切り替えフラグチェック
	else if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_CHANGE_MENU_BGM) {
		// ジングル停止
		DmSoundStopJingle(DMD_SND_BGM_PLAYER_BGM_FADEOUT_TIME);
		
		// プロシージャ切り替え(ボリュームフェードアウトへ)
		main_work->proc_update = dmSndBgmPlayerProcStopIdle;
	}
	
//	main_work->end_timer++;
}



// ==========================================================================
// dmSndBgmPlayerProcPlayIdle
/*!
	BGM再生中プロシージャ
 */
// ==========================================================================
void dmSndBgmPlayerProcPlayIdle(DMS_SND_BGM_PLAYER_MAIN_WORK *main_work)
{
	// BGM停止フラグチェック
	if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_STOP_BGM
		|| dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_FINISH_BGM) {
		// ジングル停止
		DmSoundStopJingle(DMD_SND_BGM_PLAYER_BGM_FADEOUT_TIME);
		
		// BGMフェードアウト開始
		DmSoundStopBGM(DMD_SND_BGM_PLAYER_BGM_FADEOUT_TIME);
		
		// プロシージャ切り替え(ボリュームフェードアウトへ)
		main_work->proc_update = dmSndBgmPlayerProcStopIdle;
	}
	
	// タイトルジングルへ切り替えフラグチェック
	else if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_CHANGE_TITLE_BGM) {
		// BGMフェードアウト開始
		DmSoundStopBGM(0);//DMD_SND_BGM_PLAYER_BGM_FADEOUT_TIME);
		
		// プロシージャ切り替え(ボリュームフェードアウトへ)
		main_work->proc_update = dmSndBgmPlayerProcStopIdle;
	}
	
	// メニューBGMへ切り替えフラグチェック
	else if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_CHANGE_MENU_BGM) {
		// ジングル停止
		DmSoundStopJingle(DMD_SND_BGM_PLAYER_BGM_FADEOUT_TIME);
		
		// プロシージャ切り替え(ボリュームフェードアウトへ)
		main_work->proc_update = dmSndBgmPlayerProcStopIdle;
	}
}



// ==========================================================================
// dmSndBgmPlayerProcStopIdle
/*!
	BGM停止中プロシージャ
 */
// ==========================================================================
void dmSndBgmPlayerProcStopIdle(DMS_SND_BGM_PLAYER_MAIN_WORK *main_work)
{
	// デモ周りのBGMが全て停止状態となったとき
	if (DmSoundIsStopStageBGM()
		&& DmSoundIsStopJingle()) {
		if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_FINISH_BGM) {
			// フラグOFF
			dm_snd_bgm_player_flag &= ~DMD_SND_BGM_PLAYER_FLAG_FINISH_BGM;
			
			// プロシージャ切り替え(終了プロシージャへ)
			main_work->proc_update = dmSndBgmPlayerProcSndRelease;
		}
		else {
			if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_CHANGE_TITLE_BGM) {
				dm_snd_bgm_player_flag |= DMD_SND_BGM_PLAYER_FLAG_SET_TITLE_BGM;
				
				dm_snd_bgm_player_flag &= ~DMD_SND_BGM_PLAYER_FLAG_CHANGE_TITLE_BGM;
			}
			
			else if (!(dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_CHANGE_TITLE_BGM)
					 && !(dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_CHANGE_MENU_BGM)
					 && !(dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_STOP_BGM)) {
				MTM_ASSERT(0);
				
				dm_snd_bgm_player_flag |= DMD_SND_BGM_PLAYER_FLAG_SET_TITLE_BGM;
			}
			
			else if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_STOP_BGM) {
				dm_snd_bgm_player_flag &= ~DMD_SND_BGM_PLAYER_FLAG_SET_TITLE_BGM;
				dm_snd_bgm_player_flag &= ~DMD_SND_BGM_PLAYER_FLAG_SET_MENU_BGM;
				
				dm_snd_bgm_player_flag &= ~DMD_SND_BGM_PLAYER_FLAG_STOP_BGM;
			}
			
			else {	// if (dm_snd_bgm_player_flag & DMD_SND_BGM_PLAYER_FLAG_CHANGE_MENU_BGM) {
				dm_snd_bgm_player_flag |= DMD_SND_BGM_PLAYER_FLAG_SET_MENU_BGM;
				dm_snd_bgm_player_flag &= ~DMD_SND_BGM_PLAYER_FLAG_CHANGE_MENU_BGM;
			}
			
			// プロシージャ切り替え(再生プロシージャへ)
			main_work->proc_update = dmSndBgmPlayerProcWaitSetBgm;
		}
	}
}



// ==========================================================================
// dmSndBgmPlayerProcSndRelease
/*!
	サウンドプレイヤー開放プロシージャ
 */
// ==========================================================================
void dmSndBgmPlayerProcSndRelease(DMS_SND_BGM_PLAYER_MAIN_WORK *main_work)
{
	// デモサウンドモジュール終了
	DmSoundExit();
	
	// サウンドデータ開放処理
	DmSoundFlush();
	
	// プロシージャ切り替え(終了プロシージャへ)
	main_work->proc_update = dmSndBgmPlayerProcSndFinish;
}



// ==========================================================================
// dmSndBgmPlayerProcSndFinish
/*!
	サウンドプレイヤー終了プロシージャ
 */
// ==========================================================================
void dmSndBgmPlayerProcSndFinish(DMS_SND_BGM_PLAYER_MAIN_WORK *main_work)
{
	main_work->proc_update = NULL;
	
	// 終了フラグON
	dm_snd_bgm_player_flag |= DMD_SND_BGM_PLAYER_FLAG_EXIT;
}


// ==========================================================================
// DmSndBgmPlayerStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void DmSndBgmPlayerStaticVarInit(void)
{
	// デモBGMプレイヤーTCBポインタ
	dm_snd_bgm_player_tcb = NULL;
	dm_snd_bgm_player_flag = 0;
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
