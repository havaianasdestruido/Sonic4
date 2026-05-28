// ==========================================================================
/*!
  @file gmSound.cpp
  @brief ゲームサウンド

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmSound.cpp 12 2011-04-20 05:46:40Z thamada $
  $Date:: 2011-04-20 14:46:40 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gs.h"
#include "gsMainSys.h"
#include "gmMain.h"
#include "gmGameDat.h"
#include "gmTask.h"

#include "gmSound.h"

#include "gmPlayer.h"

//----- Definitions ---------------------------------------------------------
#define GMD_SOUND_CHANGE_USE_VOL		(1)			//!< ジングル等呼び出し時にポーズを使わずにボリューム0を使う(早足君対策)

#if defined(AMD_DEBUG)
	//#define GMD_SOUND_NO_PLAY_BGM  (1)
	//#define GMD_SOUND_NO_PLAY_SE   (1)
#endif //defined(AMD_DEBUG)
#ifdef GMD_DEBUG_NO_CREATE_CRIAUDIO
	//GMD_DEBUG_NO_CREATE_CRIAUDIOが定義されていたら強制OFF
	#ifndef GMD_SOUND_NO_PLAY_BGM
		#define GMD_SOUND_NO_PLAY_BGM  (1)
	#endif // GMD_SOUND_NO_PLAY_BGM
	#ifndef GMD_SOUND_NO_PLAY_SE
		#define GMD_SOUND_NO_PLAY_SE   (1)
	#endif // GMD_SOUND_NO_PLAY_SE
#endif // GMD_DEBUG_NOCREATE_CRIAUDIO

#ifdef SONIC4_TRIAL
#define SOUND_PATH	"SOUND/TRIAL/"
#else
#define SOUND_PATH	"SOUND/SOUND/"
#endif// SONIC4_TRIAL

#define GMD_SOUND_SE_FILE_PATH		GSS_BASE_PATH SOUND_PATH"SND_FX.CSB"			//!< SEサウンドファイルパス
#if _WII
#define GMD_SOUND_BGM_FILE_PATH		GSS_BASE_PATH SOUND_PATH"SONICDL_SNG01.BRSAR"	//!< BGMサウンドファイルパス
#define GMD_SOUND_BGM_CPK_FILE_PATH	(NULL)											//!< BGM CPKサウンドファイルパス
#elif GSD_SND_NO_USE_STREAM_BGM
#define GMD_SOUND_BGM_FILE_PATH		(NULL)											// nouse
#define GMD_SOUND_BGM_CPK_FILE_PATH	(NULL)											// nouse
#else
#define GMD_SOUND_BGM_FILE_PATH		GSS_BASE_PATH SOUND_PATH"SONICDL_SNG01.CSB"		//!< BGMサウンドファイルパス
#define GMD_SOUND_BGM_CPK_FILE_PATH	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG01.CPK"		//!< BGM CPKサウンドファイルパス
#endif


//! サウンドデータインデックス列挙型
typedef enum
{
	GME_SOUND_DATA_IDX_SE	= 0,
	GME_SOUND_DATA_IDX_BGM,
	
	GME_SOUND_DATA_IDX_MAX
} GME_SOUND_DATA_IDX;

// ゲームサウンド設定フラグ gm_sound_flag
#define GMD_SOUND_FLAG_OBORE				(0x00000001)	//!< 溺れジングル再生中
#define GMD_SOUND_FLAG_INVINCIBLE			(0x00000004)	//!< 無敵ジングル再生中

#define GMD_SOUND_FLAG_1SHOT_BGM_PAUSE_VOL_0		(0x00000010)	//!< 1Shotジングルが発行 BGM類ポーズボリューム0
#define GMD_SOUND_FLAG_1SHOT_JNGLBGM_PAUSE_VOL_0	(0x00000020)	//!< 1Shotジングルが発行 ジングルBGM類ポーズボリューム0
#define GMD_SOUND_FLAG_JNGLBGM_BGM_PAUSE_VOL_0		(0x00000040)	//!< ジングルBGMが発行 BGM類ポーズボリューム0

#define GMD_SOUND_FLAG_BGM_PAUSE_VOL_0_MASK			(GMD_SOUND_FLAG_1SHOT_BGM_PAUSE_VOL_0 | \
													GMD_SOUND_FLAG_JNGLBGM_BGM_PAUSE_VOL_0)		//!< BGM類ポーズボリューム0マスク
#define GMD_SOUND_FLAG_JNGLBGM_PAUSE_VOL_0_MASK		(GMD_SOUND_FLAG_1SHOT_JNGLBGM_PAUSE_VOL_0)	//!< ジングルBGM類ポーズボリューム0マスク

#define GMD_SOUND_FLAG_ALL_PAUSE_BGM		(0x01000000)	//!< デモ移行用サウンドポーズ BGMポーズ
#define GMD_SOUND_FLAG_ALL_PAUSE_JINGLE		(0x01000000)	//!< デモ移行用サウンドポーズ ジングルポーズ
#define GMD_SOUND_FLAG_ALL_PAUSE_1SH_JINGLE	(0x04000000)	//!< デモ移行用サウンドポーズ 1Shotジングルポーズ
#define GMD_SOUND_FLAG_ALL_PAUSE			(0x08000000)	//!< デモ移行用サウンドポーズ
#define GMD_SOUND_FLAG_BGM_PAUSE			(0x80000000)	//!< BGM類ポーズ中

/* サウンド再生設定 */
#define GMD_SOUND_OBORE_START_BGM_FADE			(15)	//!< 溺れジングル開始時 BGMフェード時間
#define GMD_SOUND_OBORE_END_BGM_FADE			(15)	//!< 溺れジングル終了時 BGMフェード時間
#define GMD_SOUND_OBORE_END_JINGLE_FADE			(15)	//!< 溺れジングル終了時 ジングルフェード時間

#define GMD_SOUND_INVINCIBLE_START_BGM_FADE		(15)	//!< 無敵ジングル開始時 BGMフェード時間
#define GMD_SOUND_INVINCIBLE_END_BGM_FADE		(30)	//!< 無敵ジングル終了時 BGMフェード時間
#define GMD_SOUND_INVINCIBLE_END_JINGLE_FADE	(30)	//!< 無敵ジングル終了時 ジングルフェード時間

#define GMD_SOUND_1UP_END_BGM_FADE				(120)	//!< 1UPジングル終了時 BGMフェード時間

#define GMD_SOUND_GAMEOVER_START_BGM_FADE		(15)	//!< ゲームオーバージングル開始時 BGMフェード時間
#define GMD_SOUND_CLEAR_START_BGM_FADE			(15)	//!< ゲームクリアージングル開始時 BGMフェード時間
#define GMD_SOUND_CLEARFINAL_START_BGM_FADE		(15)	//!< ゲームクリアーファイナルジングル開始時 BGMフェード時間

#define GMD_SOUND_SPEEDUP_CHANGE_FADE			(15)	//!< 早足BGM変更フェード時間
#define GMD_SOUND_ANGRY_BOSS_CHANGE_FADE		(15)	//!< 怒りボスBGM変更フェード時間
#define GMD_SOUND_FINAL_BOSS_CHANGE_FADE		(15)	//!< FinalボスBGM変更フェード時間
#define GMD_SOUND_WIN_BOSS_CHANGE_FADE			(30)	//!< ボス戦勝利後BGM変更フェード時間

#define GMD_SOUND_ANGRY_BOSS_BGM_NAME			"snd_sng_boss2"		//!< 怒りボスBGM名
#define GMD_SOUND_FINAL_BOSS_BGM_NAME			"snd_sng_final"		//!< ファイナルボスBGM名


/// 1Shotタイプジングル再生ワーク
typedef struct tag_GMS_SOUND_1SHOT_JINGLE_WORK {
	Sint32	bgm_fade_in_frame;	//!< BGM復帰時フェードフレーム
} GMS_SOUND_1SHOT_JINGLE_WORK;

/// BGMフェードワーク
typedef struct tag_GMS_SOUND_BGM_FADE_WORK {
	float	start_vol;	//!< 開始ボリューム
	float	end_vol;	//!< 終了ボリューム
	float	fade_spd;	//!< フェードスピード
	float	now_vol;	//!< 現在のボリューム
	s32		frame;		//!< フレーム

	GSS_SND_SCB **snd_scb;	//!< フェードを行うSCB

	struct tag_GMS_SOUND_BGM_FADE_WORK *next;	//!< 次のフェード
	struct tag_GMS_SOUND_BGM_FADE_WORK *prev;	//!< 前のフェード

} GMS_SOUND_BGM_FADE_WORK;

/// BGMフェード管理ワーク
typedef struct tag_GMS_SOUND_BGM_FADE_MGR_WORK {
	s32						num;		//!< 登録数

	GMS_SOUND_BGM_FADE_WORK	*head;
	GMS_SOUND_BGM_FADE_WORK	*tail;

} GMS_SOUND_BGM_FADE_MGR_WORK;

/// ボス戦勝利時BGM変更管理ワーク
typedef struct tag_GMS_SOUND_BGM_WIN_BOSS_MGR_WORK {
	s32		timer;
} GMS_SOUND_BGM_WIN_BOSS_MGR_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmSoundPlay1ShotJingle(GME_SOUND_JINGLE_IDX jngl_idx, Sint32 jingle_fade_in_frame,
								Sint32 bgm_fade_out_frame, Sint32 bgm_fade_in_frame);
static void gmSound1ShotJingleFunc(MTS_TASK_TCB *tcb);
static void gmSound1ShotJingleDest(MTS_TASK_TCB *tcb);

static void gmSoundSetBGMFade(GSS_SND_SCB **snd_scb, float start_vol, float end_vol, s32 frame);
static void gmSoundSetBGMFadeEnd(GSS_SND_SCB *snd_scb);
static void gmSoundBGMFadeFunc(MTS_TASK_TCB *tcb);
static void gmSoundBGMFadeDest(MTS_TASK_TCB *tcb);
static void gmSoundBGMFadeAttachList(GMS_SOUND_BGM_FADE_MGR_WORK *mgr_work, GMS_SOUND_BGM_FADE_WORK *fade_work);
static void gmSoundBGMFadeDetachList(GMS_SOUND_BGM_FADE_MGR_WORK *mgr_work, GMS_SOUND_BGM_FADE_WORK *fade_work);

static void gmSoundBGMWinBossFunc(MTS_TASK_TCB *tcb);
static void gmSoundBGMWinBossDest(MTS_TASK_TCB *tcb);
//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
//! データワークリスト
static GSS_SND_DATA_WORK gm_sound_data_work_list[GME_SOUND_DATA_IDX_MAX]	= {{0}};

//! BGM用SCBへのポインタ
static GSS_SND_SCB *gm_sound_bgm_scb		= NULL;
static GSS_SND_SCB *gm_sound_bgm_sub_scb	= NULL;		//!< 早足BGM, 怒りボスBGM差し替え用
//! ジングル用SCBへのポインタ
static GSS_SND_SCB *gm_sound_jingle_scb	= NULL;
//! BGM差し替えタイプジングルSCBへのポインタ
static GSS_SND_SCB *gm_sound_jingle_bgm_scb	= NULL;

//! ゲームBGM管理フラグ
static u32 gm_sound_flag = 0;

//! 鳴りきりジングル管理タスク
static MTS_TASK_TCB *gm_sound_1shot_tcb = NULL;

//! BGMフェード管理タスク
static MTS_TASK_TCB *gm_sound_bgm_fade_tcb = NULL;

//! ボス戦勝利後BGM変更管理タスク
static MTS_TASK_TCB *gm_sound_bgm_win_boss_tcb = NULL;

#if GSD_SND_NO_USE_STREAM_BGM
static const char *gm_sound_bgm_csb_brsar_file_list[GSD_MAIN_STAGE_ID_MAX] = {
	// Zone1
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z1A1.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z1A2.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z1A3.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z1BOSS.CSB",
	// Zone2
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z2A1.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z2A2.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z2A3.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z2BOSS.CSB",
	// Zone3
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z3A1.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z3A2.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z3A3.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z3BOSS.CSB",
	// Zone4
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z4A1.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z4A2.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z4A3.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_Z4BOSS.CSB",
	// Final
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_ZF.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_ZF.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_ZF.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_ZF.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_ZF.CSB",
	// SS
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_SP.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_SP.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_SP.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_SP.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_SP.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_SP.CSB",
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_SP.CSB",
	// Ending
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_END.CSB",
};
#endif // GSD_SND_NO_USE_STREAM_BGM

//! BGM CueName/サウンドアーカイブラベル リスト
static const char *gm_sound_bgm_name_list[GSD_MAIN_STAGE_ID_MAX] = {
	// Zone1
	"snd_sng_z1a1",
	"snd_sng_z1a2",
	"snd_sng_z1a3",
	"snd_sng_boss1",
	// Zone2
	"snd_sng_z2a1",
	"snd_sng_z2a2",
	"snd_sng_z2a3",
	"snd_sng_boss1",
	// Zone3
	"snd_sng_z3a1",
	"snd_sng_z3a2",
	"snd_sng_z3a3",
	"snd_sng_boss1",
	// Zone4
	"snd_sng_z4a1",
	"snd_sng_z4a2",
	"snd_sng_z4a3",
	"snd_sng_boss1",
	// Final
	"snd_sng_boss2",
	"snd_sng_boss2",
	"snd_sng_boss2",
	"snd_sng_boss2",
	"snd_sng_boss2",
	// SS
	"snd_sng_special",
	"snd_sng_special",
	"snd_sng_special",
	"snd_sng_special",
	"snd_sng_special",
	"snd_sng_special",
	"snd_sng_special",
	// Ending
	"snd_jin_clear_final",
};


//! 早足BGM CueName/サウンドアーカイブラベル リスト
static const char *gm_sound_speedup_bgm_name_list[GSD_MAIN_STAGE_ID_MAX] = {
	"snd_sng_z1a1_speedup",
	"snd_sng_z1a2_speedup",
	"snd_sng_z1a3_speedup",
	"snd_sng_boss1",
	// Zone2
	"snd_sng_z2a1_speedup",
	"snd_sng_z2a2_speedup",
	"snd_sng_z2a3_speedup",
	"snd_sng_boss1",
	// Zone3
	"snd_sng_z3a1_speedup",
	"snd_sng_z3a2_speedup",
	"snd_sng_z3a3_speedup",
	"snd_sng_boss1",
	// Zone4
	"snd_sng_z4a1_speedup",
	"snd_sng_z4a2_speedup",
	"snd_sng_z4a3_speedup",
	"snd_sng_boss1",
	// Final
	"snd_sng_boss2",
	"snd_sng_boss2",
	"snd_sng_boss2",
	"snd_sng_boss2",
	"snd_sng_boss2",
	// SS
	"snd_sng_special",
	"snd_sng_special",
	"snd_sng_special",
	"snd_sng_special",
	"snd_sng_special",
	"snd_sng_special",
	"snd_sng_special",
	// Ending
	"snd_jin_clear_final",
};



//! ジングル CueName/サウンドアーカイブラベル リスト
static const char *gm_sound_jingle_name_list[] = {
	"snd_jin_1up",
	"snd_jin_clear",
	"snd_jin_clear",
	"snd_jin_emerald",
	"snd_jin_invincible",
	"snd_jin_new_record",
	"snd_jin_obore",
	"snd_jin_gameover",
};


//! ボス戦勝利BGM CueName/サウンドアーカイブラベル リスト
static const char *gm_sound_bgm_win_boss_name_list[GSD_MAIN_ZONE_TYPE_MAX] = {
	// Zone1
	"snd_sng_z1a3",
	// Zone2
	"snd_sng_z2a1",
	// Zone3
	"snd_sng_z3a3",
	// Zone4
	"snd_sng_z4a2",
	// Final
	"snd_sng_boss2",	// 仮
	// SS
	"snd_sng_special",	// 仮
};

//! ボス戦勝利BGM 待機時間リスト
static s32 gm_sound_bgm_win_boss_wait_frame_list[GSD_MAIN_ZONE_TYPE_MAX] = {
	// Zone1
	60 * 3,
	// Zone2
	60 * 3,
	// Zone3
	60 * 3,
	// Zone4
	60 * 3,
	// Final
	60 * 3,	// 仮
	// SS
	60 * 3,	// 仮
};

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmSoundBuild
/*!
 *	ゲームサウンド 構築
 */
// ==========================================================================
void GmSoundBuild(void)
{	
#if defined(GMD_SOUND_NO_PLAY_BGM) && defined(GMD_SOUND_NO_PLAY_SE)
	return;
#endif
	// データワーク初期化
	for (Sint32 i = 0; i < GME_SOUND_DATA_IDX_MAX; ++i) {
		GsSoundInitDataWork(&gm_sound_data_work_list[i]);
	}
	
	// SEビルド開始
	GsSoundBuildSeInit(&gm_sound_data_work_list[GME_SOUND_DATA_IDX_SE],
					   AME_CRIAUDIO_CSB_GAME,
					   GMD_SOUND_SE_FILE_PATH,
					   GMD_TASK_PRIO_DATALOAD_WAIT);
	// BGMビルド開始
	GsSoundBuildBgmInit(&gm_sound_data_work_list[GME_SOUND_DATA_IDX_BGM],
#if GSD_SND_NO_USE_STREAM_BGM
						gm_sound_bgm_csb_brsar_file_list[g_gs_main_sys_info.stage_id],
						NULL,
#else
#if 1
						GMD_SOUND_BGM_FILE_PATH,
						GMD_SOUND_BGM_CPK_FILE_PATH,
#else
						gm_sound_bgm_csb_brsar_file_list[g_gs_main_sys_info.stage_id],
						gm_sound_bgm_cpk_file_list[g_gs_main_sys_info.stage_id],
#endif
#endif // GSD_SND_NO_USE_STREAM_BGM
						GMD_TASK_PRIO_DATALOAD_WAIT,
#if _WII
						GSE_SND_DATA_TYPE_NW4R);
#else
						GSE_SND_DATA_TYPE_CRIAUDIO);
#endif /* _WII */
}

// ==========================================================================
// GmSoundBuildCheck
/*!
 *	サウンドデータ構築 終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL GmSoundBuildCheck(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM) && defined(GMD_SOUND_NO_PLAY_SE)
	return TRUE;
#endif
	// SEビルド更新・完了チェック
	if (FALSE == GsSoundBuildSeUpdate(&gm_sound_data_work_list[GME_SOUND_DATA_IDX_SE])) {
		return FALSE;
	}
	
	// BGMビルド更新・完了チェック
	if (FALSE == GsSoundBuildBgmUpdate(&gm_sound_data_work_list[GME_SOUND_DATA_IDX_BGM])) {
		return FALSE;
	}
	
	return TRUE;
}

// ==========================================================================
// GmSoundFlush
/*!
 *	ゲームサウンド 片付け
 */
// ==========================================================================
void GmSoundFlush(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM) && defined(GMD_SOUND_NO_PLAY_SE)
	return;
#endif
	// BGMデータフラッシュ
	GsSoundFlushBgm();
	// SEデータフラッシュ
	GsSoundFlushSe(&gm_sound_data_work_list[GME_SOUND_DATA_IDX_SE]);
}

// ==========================================================================
// GmSoundInit
/*!
 *	ゲームサウンド初期化
 */
// ==========================================================================
void GmSoundInit(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM) && defined(GMD_SOUND_NO_PLAY_SE)
	return;
#endif
	// サウンドシステムリセット
	GsSoundReset();
	
	// SCBを割り当て
#if _WII
	gm_sound_bgm_scb		= GsSoundAssignScb(GSE_SND_DATA_TYPE_NW4R);
	gm_sound_bgm_sub_scb	= GsSoundAssignScb(GSE_SND_DATA_TYPE_NW4R);
	gm_sound_jingle_scb		= GsSoundAssignScb(GSE_SND_DATA_TYPE_NW4R);
	gm_sound_jingle_bgm_scb	= GsSoundAssignScb(GSE_SND_DATA_TYPE_NW4R);
#else
	gm_sound_bgm_scb		= GsSoundAssignScb(GSE_SND_DATA_TYPE_CRIAUDIO);
	gm_sound_bgm_sub_scb	= GsSoundAssignScb(GSE_SND_DATA_TYPE_CRIAUDIO);
	gm_sound_jingle_scb		= GsSoundAssignScb(GSE_SND_DATA_TYPE_CRIAUDIO);
	gm_sound_jingle_bgm_scb	= GsSoundAssignScb(GSE_SND_DATA_TYPE_CRIAUDIO);
#endif
	
	// フレーム更新処理開始
	GsSoundBegin(GMD_TASK_NO_GAME_PAUSE,
				 GMD_TASK_PRIO_SOUND,
				 GMD_TASK_GROUP_SOUND);

	// 管理フラグクリア
	gm_sound_flag = 0;
	// 管理タスクアTCBドレスクリア
	gm_sound_1shot_tcb = NULL;
	gm_sound_bgm_win_boss_tcb = NULL;
}

// ==========================================================================
// GmSoundExit
/*!
 *	ゲームサウンド終了処理
 */
// ==========================================================================
void GmSoundExit(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM) && defined(GMD_SOUND_NO_PLAY_SE)
	return;
#endif
	if (gm_sound_1shot_tcb) {
		// タスク破棄
		mtTaskClearTcb(gm_sound_1shot_tcb);
	}

	if (gm_sound_bgm_fade_tcb) {
		// フェード処理破棄
		mtTaskClearTcb(gm_sound_bgm_fade_tcb);
	}

	if (gm_sound_bgm_win_boss_tcb) {
		// ボス戦処理BGM切り替え処理破棄
		mtTaskClearTcb(gm_sound_bgm_win_boss_tcb);
	}

	// サウンドシステム停止
	GsSoundHalt();
	
	// フレーム更新処理終了
	GsSoundEnd();
	
	// SCB破棄
	if (gm_sound_jingle_scb) {
		GsSoundStopBgm(gm_sound_jingle_scb, 0);
		GsSoundResignScb(gm_sound_jingle_scb);
		gm_sound_jingle_scb	= NULL;
	}
	if (gm_sound_bgm_scb) {
		GsSoundStopBgm(gm_sound_bgm_scb, 0);
		GsSoundResignScb(gm_sound_bgm_scb);
		gm_sound_bgm_scb	= NULL;
	}
	if (gm_sound_bgm_sub_scb) {
		GsSoundStopBgm(gm_sound_bgm_sub_scb, 0);
		GsSoundResignScb(gm_sound_bgm_sub_scb);
		gm_sound_bgm_sub_scb	= NULL;
	}
	if (gm_sound_jingle_bgm_scb) {
		GsSoundStopBgm(gm_sound_jingle_bgm_scb, 0);
		GsSoundResignScb(gm_sound_jingle_bgm_scb);
		gm_sound_jingle_bgm_scb	= NULL;
	}
	
	// GsSoundReset()はオブジェクト全クリア完了後に呼び出し
}

// ==========================================================================
// SE再生
// ==========================================================================
// ==========================================================================
// GmSoundPlaySE
/*!
 *	SE再生
 *
 *	@param	cue_name	[in]	再生するSEのキュー名
 *	@param	se_handle	[in]	SEハンドル NULL可
 *
 */
// ==========================================================================
void GmSoundPlaySE(char *cue_name, GSS_SND_SE_HANDLE *se_handle/*=NULL*/)
{
#if defined(GMD_SOUND_NO_PLAY_SE)
	return;
#endif
	GsSoundPlaySe(cue_name, se_handle);
}

#if _IPHONE
// ==========================================================================
// GmSoundPlaySEForce
/*!
 *	SE再生 強制
 *
 *	@param	cue_name	[in]	再生するSEのキュー名
 *	@param	se_handle	[in]	SEハンドル NULL可
 *
 *	@note 通常のSE再生と違い、現在の発音数による再生拒否が行われない
 *
 */
// ==========================================================================
void GmSoundPlaySEForce(char *cue_name, GSS_SND_SE_HANDLE *se_handle/*=NULL*/)
{
#if defined(GMD_SOUND_NO_PLAY_SE)
	return;
#endif
	GsSoundPlaySeForce(cue_name, se_handle);
}
#endif // _IPHONE

// ==========================================================================
// BGM
// ==========================================================================
// =======================================================================
// GmSoundPlayBGM
/*!
  BGM再生
  
  @param cue_name		[in]	再生するSEのキュー名
  @param fade_farame	[in]	フェードインにかけるフレーム数
  
  @note
  この関数は互換性のために残しています。
  代わりにGmSoundPlayStageBGM()を使用してください。
 */
// =======================================================================
void GmSoundPlayBGM(const char *cue_name, Sint32 fade_frame/*=0*/)
{
	UNREFERENCED_PARAMETER(cue_name);
	
	GmSoundPlayStageBGM(fade_frame);
}


// =======================================================================
// GmSoundPlayStageBGM
/*!
  ステージBGM再生
  
  @param fade_frame	[in]	フェードインにかけるフレーム数
  
  @note
  現在のステージに応じたサウンドを再生します。
 */
// =======================================================================
void GmSoundPlayStageBGM(Sint32 fade_frame/*=0*/)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif

	// ボリューム初期化
	GsSoundScbSetVolume(gm_sound_bgm_scb, 1.f);
	GsSoundScbSetSeqMute(gm_sound_bgm_scb, FALSE);
	
	
	if((GMD_PLF_SUPER_SONIC&g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->player_flag)!=0) { //sss - not for SSonic
		GsSoundScbSetVolume(gm_sound_bgm_scb, 0.f);
		GsSoundScbSetSeqMute(gm_sound_bgm_scb, TRUE);
	}

	// BGM再生
	GsSoundPlayBgm(gm_sound_bgm_scb,
				   gm_sound_bgm_name_list[g_gs_main_sys_info.stage_id],
				   fade_frame);

	// サウンドシステムミュート対応
	gm_sound_bgm_scb->flag |= GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM;
}

// =======================================================================
// GmSoundStopBGM
/*!
  BGM停止
  
  @param fade_frame	[in]	フェードアウトにかけるフレーム数
  
  @note
  この関数は互換性のために残しています。
  代わりにGmSoundStopStageBGM()を使用してください。
 */
// =======================================================================
void GmSoundStopBGM(Sint32 fade_frame/*=0*/)
{
	GmSoundStopStageBGM(fade_frame);
}

// =======================================================================
// GmSoundStopStageBGM
/*!
  ステージBGM停止
  
  @param fade_frame	[in]	フェードアウトにかけるフレーム数
 */
// =======================================================================
void GmSoundStopStageBGM(Sint32 fade_frame/*=0*/)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	GsSoundStopBgm(gm_sound_bgm_scb, fade_frame);
}


// =======================================================================
// GmSoundPauseStageBGM
/*!
  ステージBGM一時停止
  
  @param fade_frame	[in]	フェードアウトにかけるフレーム数
 */
// =======================================================================
void GmSoundPauseStageBGM(Sint32 fade_frame/*=0*/)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	GsSoundPauseBgm(gm_sound_bgm_scb, fade_frame);
}

// =======================================================================
// GmSoundResumeStageBGM
/*!
  ステージBGM再開
  
  @param fade_frame	[in]	フェードインにかけるフレーム数
 */
// =======================================================================
void GmSoundResumeStageBGM(Sint32 fade_frame/*=0*/)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	GsSoundResumeBgm(gm_sound_bgm_scb, fade_frame);
}

// =======================================================================
// 早足BGM
// =======================================================================
// =======================================================================
// GmSoundChangeSpeedupBGM
/*!
 *	早足BGM再生
 */
// =======================================================================
void GmSoundChangeSpeedupBGM(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	GSS_SND_SCB	*temp_scb;
	BOOL		b_pause = FALSE, b_vol_0 = FALSE;

	// 念のためサブの方を停止しておく
	GsSoundStopBgm(gm_sound_bgm_sub_scb, 0);

	// BGM停止
	if (GsSoundIsBgmPause(gm_sound_bgm_scb)) {
		b_pause = TRUE;
	}
	if (gm_sound_flag & GMD_SOUND_FLAG_BGM_PAUSE_VOL_0_MASK) {
		b_vol_0 = TRUE;
	}
	if (b_pause | b_vol_0) {
		GmSoundStopStageBGM(0);
	}
	else {
		GmSoundStopStageBGM(GMD_SOUND_SPEEDUP_CHANGE_FADE);
	}

	// サブとメインSCBを入れ替える
	temp_scb = gm_sound_bgm_scb;
	gm_sound_bgm_scb = gm_sound_bgm_sub_scb;
	gm_sound_bgm_sub_scb = temp_scb;
	
	// ボリューム初期化
	GsSoundScbSetVolume(gm_sound_bgm_scb, 1.f);
	GsSoundScbSetSeqMute(gm_sound_bgm_scb, FALSE);
	// BGM再生
	GsSoundPlayBgm(gm_sound_bgm_scb,
				   gm_sound_speedup_bgm_name_list[g_gs_main_sys_info.stage_id],
				   GMD_SOUND_SPEEDUP_CHANGE_FADE);
	// サウンドシステムミュート対応
	gm_sound_bgm_scb->flag |= GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM;

	if (b_pause) {
		// いきなりポーズしておく // 終わりがずれる...
		GmSoundPauseStageBGM(0);
	}
	if (b_vol_0) {
		// いきなりボリューム0に
		gmSoundSetBGMFadeEnd(gm_sound_bgm_scb);
		GsSoundScbSetVolume(gm_sound_bgm_scb, 0.f);
		GsSoundScbSetSeqMute(gm_sound_bgm_scb, TRUE);
	}
}

// =======================================================================
// 怒りボスBGM
// =======================================================================
// =======================================================================
// GmSoundChangeAngryBossBGM
/*!
 *	怒りボスBGM再生
 */
// =======================================================================
void GmSoundChangeAngryBossBGM(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	GSS_SND_SCB	*temp_scb;
	BOOL		b_pause = FALSE, b_vol_0 = FALSE;

	// 念のためサブの方を停止しておく
	GsSoundStopBgm(gm_sound_bgm_sub_scb, 0);

	// BGM停止
	if (GsSoundIsBgmPause(gm_sound_bgm_scb)) {
		b_pause = TRUE;
	}
	if (gm_sound_flag & GMD_SOUND_FLAG_BGM_PAUSE_VOL_0_MASK) {
		b_vol_0 = TRUE;
	}
	if (b_pause | b_vol_0) {
		GmSoundStopStageBGM(0);
	}
	else {
		GmSoundStopStageBGM(GMD_SOUND_ANGRY_BOSS_CHANGE_FADE);
	}

	// サブとメインSCBを入れ替える
	temp_scb = gm_sound_bgm_scb;
	gm_sound_bgm_scb = gm_sound_bgm_sub_scb;
	gm_sound_bgm_sub_scb = temp_scb;
	
	// ボリューム初期化
	GsSoundScbSetVolume(gm_sound_bgm_scb, 1.f);
	GsSoundScbSetSeqMute(gm_sound_bgm_scb, FALSE);
	// BGM再生
	GsSoundPlayBgm(gm_sound_bgm_scb,
				   GMD_SOUND_ANGRY_BOSS_BGM_NAME,
				   GMD_SOUND_ANGRY_BOSS_CHANGE_FADE);
	// サウンドシステムミュート対応
	gm_sound_bgm_scb->flag |= GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM;

	if (b_pause) {
		// いきなりポーズしておく
		GmSoundPauseStageBGM(0);
	}
	if (b_vol_0) {
		// いきなりボリューム0に
		gmSoundSetBGMFadeEnd(gm_sound_bgm_scb);
		GsSoundScbSetVolume(gm_sound_bgm_scb, 0.f);
		GsSoundScbSetSeqMute(gm_sound_bgm_scb, TRUE);
	}
}

// =======================================================================
// ボス戦勝利時BGM
// =======================================================================
// =======================================================================
// GmSoundChangeWinBossBGM
/*!
 *	ボス戦勝利時BGM変更再生
 */
// =======================================================================
void GmSoundChangeWinBossBGM(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	GMS_SOUND_BGM_WIN_BOSS_MGR_WORK	*mgr_work;

	if (g_gs_main_sys_info.stage_id >= GSD_MAIN_STAGE_ID_FINAL_1) {
		// Final中は切り替えない
		return;
	}

	MTM_ASSERT(gm_sound_bgm_win_boss_tcb == NULL);
	if (gm_sound_bgm_win_boss_tcb == NULL) {
		// BGM変更待機管理
		gm_sound_bgm_win_boss_tcb = MTM_TASK_MAKE_TCB(gmSoundBGMWinBossFunc, gmSoundBGMWinBossDest, 0/*flag*/,
							GMD_TASK_PAUSELEVEL_DEF, GMD_TASK_PRIO_SOUND, GMD_TASK_GROUP_SOUND,
							sizeof(GMS_SOUND_BGM_WIN_BOSS_MGR_WORK), "GM_SOUND_WB");
		mgr_work = (GMS_SOUND_BGM_WIN_BOSS_MGR_WORK*)mtTaskGetTcbWork(gm_sound_bgm_win_boss_tcb);
		MI_CpuClear8(mgr_work, sizeof(GMS_SOUND_BGM_WIN_BOSS_MGR_WORK));

		// 待機時間取得
		mgr_work->timer = gm_sound_bgm_win_boss_wait_frame_list[GMM_MAIN_GET_ZONE_TYPE()];
	}
}

// =======================================================================
// FinalボスBGM
// =======================================================================
// =======================================================================
// GmSoundChangeFinalBossBGM
/*!
 *	ファイナルボスBGM再生
 */
// =======================================================================
void GmSoundChangeFinalBossBGM(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	GSS_SND_SCB	*temp_scb;
	BOOL		b_pause = FALSE, b_vol_0 = FALSE;

	// 念のためサブの方を停止しておく
	GsSoundStopBgm(gm_sound_bgm_sub_scb, 0);

	// BGM停止
	if (GsSoundIsBgmPause(gm_sound_bgm_scb)) {
		b_pause = TRUE;
	}
	if (gm_sound_flag & GMD_SOUND_FLAG_BGM_PAUSE_VOL_0_MASK) {
		b_vol_0 = TRUE;
	}
	if (b_pause | b_vol_0) {
		GmSoundStopStageBGM(0);
	}
	else {
		GmSoundStopStageBGM(GMD_SOUND_FINAL_BOSS_CHANGE_FADE);
	}

	// サブとメインSCBを入れ替える
	temp_scb = gm_sound_bgm_scb;
	gm_sound_bgm_scb = gm_sound_bgm_sub_scb;
	gm_sound_bgm_sub_scb = temp_scb;
	
	// ボリューム初期化
	GsSoundScbSetVolume(gm_sound_bgm_scb, 1.f);
	GsSoundScbSetSeqMute(gm_sound_bgm_scb, FALSE);
	// BGM再生
	GsSoundPlayBgm(gm_sound_bgm_scb,
				   GMD_SOUND_FINAL_BOSS_BGM_NAME,
				   GMD_SOUND_FINAL_BOSS_CHANGE_FADE);
	// サウンドシステムミュート対応
	gm_sound_bgm_scb->flag |= GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM;

	if (b_pause) {
		// いきなりポーズしておく
		GmSoundPauseStageBGM(0);
	}
	if (b_vol_0) {
		// いきなりボリューム0に
		gmSoundSetBGMFadeEnd(gm_sound_bgm_scb);
		GsSoundScbSetVolume(gm_sound_bgm_scb, 0.f);
		GsSoundScbSetSeqMute(gm_sound_bgm_scb, TRUE);
	}
}

// ==========================================================================
// ジングル
// ==========================================================================
// =======================================================================
// GmSoundPlayJingle
/*!
  ジングル再生
  
  @param jngl_idx	[in]	ジングルインデックス（GME_SOUND_JINGLE_IDX_XXX）
  @param fade_frame	[in]	フェードインにかけるフレーム数
 */
// =======================================================================
void GmSoundPlayJingle(GME_SOUND_JINGLE_IDX jngl_idx, Sint32 fade_frame/*=0*/)
{
	MTM_ASSERT(jngl_idx >= 0 && jngl_idx < GME_SOUND_JINGLE_IDX_MAX);
	
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	// BGM停止
//	GmSoundPauseStageBGM(fade_frame);

	// ボリューム初期化
	GsSoundScbSetVolume(gm_sound_jingle_scb, 1.f);
	GsSoundScbSetSeqMute(gm_sound_jingle_scb, FALSE);
	// ジングル再生
	GsSoundStopBgm(gm_sound_jingle_scb);
	GsSoundPlayBgm(gm_sound_jingle_scb,
				   gm_sound_jingle_name_list[jngl_idx],
				   fade_frame);

	// 【Xbox360】
	// TCR v1.5j
	// TCR # 024 AUD : BGM
	// プレイヤーが Xbox ガイドまたは Xbox ダッシュボードで音楽プレーヤー機能を有効にしている場合、
	// ゲーム独自の BGM を再生してはならない。
	// 【PS3】
	// TRC v3.4
	// 13.0 システム設定の対応
	// [R061]　Dolby Digital Interactive Encoding、DTS Interactive、
	// システムBGM 機能を有効にした状態でも、正常に動作する（処理落ちなどが発生しない）。
	// サウンドシステムミュート対応
	gm_sound_jingle_scb->flag |= GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM;
//	// サウンドシステムミュートOFF
//	gm_sound_jingle_scb->flag &= ~GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM;
}

// =======================================================================
// GmSoundStopJingle
/*!
  ジングル停止
  
  @param fade_frame	[in]	フェードアウトにかけるフレーム数
 */
// =======================================================================
void GmSoundStopJingle(Sint32 fade_frame/*=0*/)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	// ジングル停止
	GsSoundStopBgm(gm_sound_jingle_scb, fade_frame);

//	// BGM再開
//	GmSoundResumeStageBGM(fade_frame);
}

// ==========================================================================
// BGMタイプジングル
// ==========================================================================
// =======================================================================
// GmSoundPlayBGMJingle
/*!
  BGMタイプジングル再生
  
  @param jngl_idx	[in]	ジングルインデックス（GME_SOUND_JINGLE_IDX_XXX）
  @param fade_frame	[in]	フェードインにかけるフレーム数
 */
// =======================================================================
void GmSoundPlayBGMJingle(GME_SOUND_JINGLE_IDX jngl_idx, Sint32 fade_frame/*=0*/)
{
	MTM_ASSERT(jngl_idx >= 0 && jngl_idx < GME_SOUND_JINGLE_IDX_MAX);
	
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif

	// BGM停止
//	GmSoundPauseStageBGM(fade_frame);

	// ボリューム初期化
	GsSoundScbSetVolume(gm_sound_jingle_bgm_scb, 1.f);
	GsSoundScbSetSeqMute(gm_sound_jingle_bgm_scb, FALSE);
	// ジングル再生
	GsSoundStopBgm(gm_sound_jingle_bgm_scb);
	GsSoundPlayBgm(gm_sound_jingle_bgm_scb,
				   gm_sound_jingle_name_list[jngl_idx],
				   fade_frame);

	// サウンドシステムミュート対応
	gm_sound_jingle_bgm_scb->flag |= GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM;
}

// =======================================================================
// GmSoundStopBGMJingle
/*!
  BGMタイプジングル停止
  
  @param fade_frame	[in]	フェードアウトにかけるフレーム数
 */
// =======================================================================
void GmSoundStopBGMJingle(Sint32 fade_frame/*=0*/)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	// ジングル停止
	GsSoundStopBgm(gm_sound_jingle_bgm_scb, fade_frame);
}

// =======================================================================
// GmSoundPauseBGMJingle
/*!
  BGMタイプジングル一時停止
  
  @param fade_frame	[in]	フェードアウトにかけるフレーム数
 */
// =======================================================================
void GmSoundPauseBGMJingle(Sint32 fade_frame/*=0*/)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	GsSoundPauseBgm(gm_sound_jingle_bgm_scb, fade_frame);
}

// =======================================================================
// GmSoundResumeBGMJingle
/*!
  BGMタイプジングル再開
  
  @param fade_frame	[in]	フェードインにかけるフレーム数
 */
// =======================================================================
void GmSoundResumeBGMJingle(Sint32 fade_frame/*=0*/)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	GsSoundResumeBgm(gm_sound_jingle_bgm_scb, fade_frame);
}

// ==========================================================================
// ボリューム設定
// ==========================================================================
// =======================================================================
// GmSoundSetVolumeSE
/*!
  SEボリューム設定
  
  @param volume	[in]	ボリューム（倍率）
  
  @note
  SE全体のボリュームを設定します。
 */
// =======================================================================
void GmSoundSetVolumeSE(Float volume)
{
#if defined(GMD_SOUND_NO_PLAY_SE)
	return;
#endif
	GsSoundSetVolume(GSE_SND_TYPE_SE, volume);
}

// =======================================================================
// GmSoundSetVolumeBGM
/*!
  BGMボリューム設定
  
  @param volume	[in]	ボリューム（倍率）
  
  @note
  BGM全体のボリュームを設定します（ジングルも含む）。
 */
// =======================================================================
void GmSoundSetVolumeBGM(Float volume)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	GsSoundSetVolume(GSE_SND_TYPE_BGM, volume);
}

// =======================================================================
// 特殊ジングル管理
// =======================================================================
// =======================================================================
// GmSoundPlayJingleObore
/*!
 *	ジングル再生 溺れ
 */
// =======================================================================
void GmSoundPlayJingleObore(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	if (!GsSoundIsBgmStop(gm_sound_jingle_bgm_scb)) {
		// 既にBGMタイプジングルを再生している(無敵再生中)
		MTM_ASSERT(0);	// 実際は発生しないはず
		return;
	}
	if (gm_sound_flag & GMD_SOUND_FLAG_OBORE) {
		// 既に再生中
		return;
	}
	// BGMポーズ
#if !GMD_SOUND_CHANGE_USE_VOL
	if (!GsSoundIsBgmPause(gm_sound_bgm_scb)) {
		GmSoundPauseStageBGM(GMD_SOUND_OBORE_START_BGM_FADE);
	}
#else
	if (!GsSoundIsBgmStop(gm_sound_bgm_scb) &&
			!(gm_sound_flag & GMD_SOUND_FLAG_BGM_PAUSE_VOL_0_MASK)) {
		gmSoundSetBGMFadeEnd(gm_sound_bgm_scb);
		GsSoundScbSetVolume(gm_sound_bgm_scb, 0.f);
		GsSoundScbSetSeqMute(gm_sound_bgm_scb, TRUE);
	}
	gm_sound_flag |= GMD_SOUND_FLAG_JNGLBGM_BGM_PAUSE_VOL_0;
#endif	// #if !GMD_SOUND_CHANGE_USE_VOL

	// BGMタイプジングル再生
	GmSoundPlayBGMJingle(GME_SOUND_JINGLE_IDX_OBORE, 0);
	if (gm_sound_flag & GMD_SOUND_FLAG_JNGLBGM_PAUSE_VOL_0_MASK) {
		// いきなりボリューム0
		gmSoundSetBGMFadeEnd(gm_sound_jingle_bgm_scb);
		GsSoundScbSetVolume(gm_sound_jingle_bgm_scb, 0.f);
		GsSoundScbSetSeqMute(gm_sound_jingle_bgm_scb, TRUE);
	}

	// フラグ設定
	gm_sound_flag |= GMD_SOUND_FLAG_OBORE;
}

// =======================================================================
// GmSoundStopJingleObore
/*!
 *	ジングル停止 溺れ
 */
// =======================================================================
void GmSoundStopJingleObore(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	if (!(gm_sound_flag & GMD_SOUND_FLAG_OBORE)) {
		// 既に停止中
		return;
	}
	// BGMタイプジングル停止
	if (!GsSoundIsBgmPause(gm_sound_jingle_bgm_scb) &&
			!(gm_sound_flag & GMD_SOUND_FLAG_JNGLBGM_PAUSE_VOL_0_MASK)) {
		GmSoundStopBGMJingle(GMD_SOUND_OBORE_END_JINGLE_FADE);
	}
	else {
		// ポーズ中, ボリューム0時は即時停止
		GmSoundStopBGMJingle(0);
	}

	// BGM再開
#if !GMD_SOUND_CHANGE_USE_VOL
	if (!(gm_sound_flag & GMD_SOUND_FLAG_BGM_PAUSE)) {
		GmSoundResumeStageBGM(GMD_SOUND_OBORE_END_JINGLE_FADE);
	}
#else
	if (!GsSoundIsBgmStop(gm_sound_bgm_scb) &&
			(gm_sound_flag & GMD_SOUND_FLAG_JNGLBGM_BGM_PAUSE_VOL_0) &&
			!(gm_sound_flag & (GMD_SOUND_FLAG_BGM_PAUSE_VOL_0_MASK & ~GMD_SOUND_FLAG_JNGLBGM_BGM_PAUSE_VOL_0))) {
		//GsSoundScbSetVolume(gm_sound_bgm_scb, 1.f);
		gmSoundSetBGMFade(&gm_sound_bgm_scb, 0.f, 1.f, GMD_SOUND_OBORE_END_JINGLE_FADE);
		GsSoundScbSetSeqMute(gm_sound_bgm_scb, FALSE);
	}
	gm_sound_flag &= ~GMD_SOUND_FLAG_JNGLBGM_BGM_PAUSE_VOL_0;
#endif // #if !GMD_SOUND_CHANGE_USE_VOL

	// フラグクリア
	gm_sound_flag &= ~GMD_SOUND_FLAG_OBORE;
}

// =======================================================================
// GmSoundPlayJingleInvincible
/*!
 *	ジングル再生 無敵
 */
// =======================================================================
void GmSoundPlayJingleInvincible(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	if (gm_sound_flag & GMD_SOUND_FLAG_INVINCIBLE) {
		// 既に再生中
		return;
	}
	// BGMポーズ
#if !GMD_SOUND_CHANGE_USE_VOL
	if (!GsSoundIsBgmPause(gm_sound_bgm_scb)) {
		GmSoundPauseStageBGM(GMD_SOUND_OBORE_START_BGM_FADE);
	}
#else
	if (!GsSoundIsBgmStop(gm_sound_bgm_scb) &&
			!(gm_sound_flag & GMD_SOUND_FLAG_BGM_PAUSE_VOL_0_MASK)) {
		gmSoundSetBGMFadeEnd(gm_sound_bgm_scb);
		GsSoundScbSetVolume(gm_sound_bgm_scb, 0.f);
		GsSoundScbSetSeqMute(gm_sound_bgm_scb, TRUE);
	}
	gm_sound_flag |= GMD_SOUND_FLAG_JNGLBGM_BGM_PAUSE_VOL_0;
#endif

	// BGMタイプジングル再生
	GmSoundPlayBGMJingle(GME_SOUND_JINGLE_IDX_INVINCIBLE, 0);
	if (gm_sound_flag & GMD_SOUND_FLAG_JNGLBGM_PAUSE_VOL_0_MASK) {
		// いきなりボリューム0
		gmSoundSetBGMFadeEnd(gm_sound_jingle_bgm_scb);
		GsSoundScbSetVolume(gm_sound_jingle_bgm_scb, 0.f);
		GsSoundScbSetSeqMute(gm_sound_jingle_bgm_scb, TRUE);
	}


	// フラグ設定
	gm_sound_flag |= GMD_SOUND_FLAG_INVINCIBLE;

	// 優先度の低いジングルをクリア
	gm_sound_flag &= ~GMD_SOUND_FLAG_OBORE;
}

// =======================================================================
// GmSoundStopJingleInvincible
/*!
 *	ジングル停止 無敵
 */
// =======================================================================
void GmSoundStopJingleInvincible(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	if (!(gm_sound_flag & GMD_SOUND_FLAG_INVINCIBLE)) {
		// 既に停止中
		return;
	}
	// BGMタイプジングル停止
	if (!GsSoundIsBgmPause(gm_sound_jingle_bgm_scb) &&
			!(gm_sound_flag & GMD_SOUND_FLAG_JNGLBGM_PAUSE_VOL_0_MASK)) {
		GmSoundStopBGMJingle(GMD_SOUND_INVINCIBLE_END_BGM_FADE);
	}
	else {
		// ポーズ中は即時停止
		GmSoundStopBGMJingle(0);
	}

	// BGM再開
#if !GMD_SOUND_CHANGE_USE_VOL
	if (!(gm_sound_flag & GMD_SOUND_FLAG_BGM_PAUSE)) {
		GmSoundResumeStageBGM(GMD_SOUND_INVINCIBLE_END_JINGLE_FADE);
	}
#else
	if (!GsSoundIsBgmStop(gm_sound_bgm_scb) &&
			(gm_sound_flag & GMD_SOUND_FLAG_JNGLBGM_BGM_PAUSE_VOL_0) &&
			!(gm_sound_flag & (GMD_SOUND_FLAG_BGM_PAUSE_VOL_0_MASK & ~GMD_SOUND_FLAG_JNGLBGM_BGM_PAUSE_VOL_0))) {
		//GsSoundScbSetVolume(gm_sound_bgm_scb, 1.f);
		gmSoundSetBGMFade(&gm_sound_bgm_scb, 0.f, 1.f, GMD_SOUND_INVINCIBLE_END_JINGLE_FADE);
		GsSoundScbSetSeqMute(gm_sound_bgm_scb, FALSE);
	}
	gm_sound_flag &= ~GMD_SOUND_FLAG_JNGLBGM_BGM_PAUSE_VOL_0;
#endif

	// フラグクリア
	gm_sound_flag &= ~GMD_SOUND_FLAG_INVINCIBLE;
}

// =======================================================================
// GmSoundPlayJingle1UP
/*!
 *	ジングル再生 1UP
 *
 *	@param	ret_last_sound	[in]	BGM, ジングル復帰あり
 *
 *	@note
 *		ret_last_sound : FALSEの時、BGMの終了は行いません
 */
// =======================================================================
void GmSoundPlayJingle1UP(BOOL ret_last_sound)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	if (ret_last_sound) {
		gmSoundPlay1ShotJingle(GME_SOUND_JINGLE_IDX_1UP, 0,
									0, GMD_SOUND_1UP_END_BGM_FADE);
	}
	else {
		// ジングル再生
		GmSoundPlayJingle(GME_SOUND_JINGLE_IDX_1UP, 0);
	}
}

// =======================================================================
// GmSoundPlayGameOver
/*!
 *	ジングル再生 ゲームオーバー
 *
 *	@note
 *		BGMの停止を行います
 */
// =======================================================================
void GmSoundPlayGameOver(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	// BGM停止
	GmSoundStopStageBGM(GMD_SOUND_GAMEOVER_START_BGM_FADE);
	// BGMタイプジングル停止
	GmSoundStopBGMJingle(GMD_SOUND_GAMEOVER_START_BGM_FADE);

	if (gm_sound_1shot_tcb) {
		// 1shotジングルBGM復帰待機TCBは破棄
		mtTaskClearTcb(gm_sound_1shot_tcb);
	}

	// ジングル再生
	GmSoundPlayJingle(GME_SOUND_JINGLE_IDX_GAMEOVER, 0);
}

// =======================================================================
// GmSoundPlayClear
/*!
 *	ジングル再生 ゲームクリアー
 *
 *	@note
 *		BGMの停止を行います
 */
// =======================================================================
void GmSoundPlayClear(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	// BGM停止
	GmSoundStopStageBGM(GMD_SOUND_CLEAR_START_BGM_FADE);
	// BGMタイプジングル停止
	GmSoundStopBGMJingle(GMD_SOUND_CLEAR_START_BGM_FADE);

	if (gm_sound_1shot_tcb) {
		// 1shotジングルBGM復帰待機TCBは破棄
		mtTaskClearTcb(gm_sound_1shot_tcb);
	}

	// ジングル再生
	GmSoundPlayJingle(GME_SOUND_JINGLE_IDX_CLEAR, 0);
}

// =======================================================================
// GmSoundPlayClearFinal
/*!
 *	ジングル再生 ゲームクリアー ファイナル
 *
 *	@note
 *		BGMの停止を行います
 */
// =======================================================================
void GmSoundPlayClearFinal(void)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	// BGM停止
	GmSoundStopStageBGM(GMD_SOUND_CLEARFINAL_START_BGM_FADE);
	// BGMタイプジングル停止
	GmSoundStopBGMJingle(GMD_SOUND_CLEARFINAL_START_BGM_FADE);

	if (gm_sound_1shot_tcb) {
		// 1shotジングルBGM復帰待機TCBは破棄
		mtTaskClearTcb(gm_sound_1shot_tcb);
	}

	// ジングル再生
	GmSoundPlayJingle(GME_SOUND_JINGLE_IDX_CLEAR_FINAL, 0);
}

// =======================================================================
// 全サウンドポーズ(ポーズからのデモ移行用)
// =======================================================================
// =======================================================================
// GmSoundAllPause
/*!
 *	サウンドポーズ
 *
 *	@note
 *		すべてのストリームサウンドを一時停止します。
 */
// =======================================================================
void GmSoundAllPause(void)
{
	// 1Shotジングルポーズ
	if (!GsSoundIsBgmStop(gm_sound_jingle_scb) &&
			!GsSoundIsBgmPause(gm_sound_jingle_scb)) {
		GsSoundPauseBgm(gm_sound_jingle_scb, 0);
		gm_sound_flag |= GMD_SOUND_FLAG_ALL_PAUSE_1SH_JINGLE;
	}
	// ジングルポーズ
	if (!GsSoundIsBgmStop(gm_sound_jingle_bgm_scb) &&
			!GsSoundIsBgmPause(gm_sound_jingle_bgm_scb)) {
		GsSoundPauseBgm(gm_sound_jingle_bgm_scb, 0);
		gm_sound_flag |= GMD_SOUND_FLAG_ALL_PAUSE_JINGLE;
	}
	// BGMポーズ
	if (!GsSoundIsBgmStop(gm_sound_bgm_scb)&&
			!GsSoundIsBgmPause(gm_sound_bgm_scb)) {
		GsSoundPauseBgm(gm_sound_bgm_scb, 0);
		gm_sound_flag |= GMD_SOUND_FLAG_ALL_PAUSE_BGM;
	}
	// サブBGMは停止
	GsSoundStopBgm(gm_sound_bgm_sub_scb, 0);
	// SEポーズ
	GsSoundPauseSe(GSD_SND_SE_HANDLE_PAUSE_LEVEL_GAME);

	gm_sound_flag |= GMD_SOUND_FLAG_ALL_PAUSE;
}

// =======================================================================
// GmSoundAllResume
/*!
 *	サウンド復旧
 *
 *	@note
 *		GmSoundAllPauseで一時停止したストリームサウンドを復旧します。
 */
// =======================================================================
void GmSoundAllResume(void)
{
	if (gm_sound_flag & GMD_SOUND_FLAG_ALL_PAUSE_1SH_JINGLE) {
		// 1Shotジングル復旧
		GsSoundResumeBgm(gm_sound_jingle_scb, 0);
	}
	if (gm_sound_flag & GMD_SOUND_FLAG_ALL_PAUSE_JINGLE) {
		// ジングル復旧
		GsSoundResumeBgm(gm_sound_jingle_bgm_scb, 0);
	}
	if (gm_sound_flag & GMD_SOUND_FLAG_ALL_PAUSE_BGM) {
		// BGM復旧
		GsSoundResumeBgm(gm_sound_bgm_scb, 0);
	}
	// SE復旧
	GsSoundResumeSe(GSD_SND_SE_HANDLE_PAUSE_LEVEL_GAME);

	gm_sound_flag &= ~(GMD_SOUND_FLAG_ALL_PAUSE_BGM | GMD_SOUND_FLAG_ALL_PAUSE_JINGLE |
							GMD_SOUND_FLAG_ALL_PAUSE_1SH_JINGLE | GMD_SOUND_FLAG_ALL_PAUSE);
}




//----- Local Functions -----------------------------------------------------
// =======================================================================
// gmSoundPlay1ShotJingle
/*!
 *	ジングル再生 1Shot
 *
 *	@param	jngl_idx				[in]	ジングルID
 *	@param	jingle_fade_in_frame	[in]	ジングルフェードインフレーム
 *	@param	bgm_fade_out_frame		[in]	BGMフェードアウトフレーム
 *	@param	bgm_fade_in_frame		[in]	BGMフェードインフレーム
 *
 *	@note
 *		ならし切りのジングルを再生し、BGMを再開します
 */
// =======================================================================
void gmSoundPlay1ShotJingle(GME_SOUND_JINGLE_IDX jngl_idx, Sint32 jingle_fade_in_frame,
								Sint32 bgm_fade_out_frame, Sint32 bgm_fade_in_frame)
{
	GMS_SOUND_1SHOT_JINGLE_WORK	*jingle_1shot_work;

	// BGM類ポーズ
	gm_sound_flag |= GMD_SOUND_FLAG_BGM_PAUSE;

#if !GMD_SOUND_CHANGE_USE_VOL
	// BGMポーズ
	if (!GsSoundIsBgmStop(gm_sound_bgm_scb) &&
			!GsSoundIsBgmPause(gm_sound_bgm_scb)) {
		GmSoundPauseStageBGM(bgm_fade_out_frame);
	}
	// BGMタイプジングルポーズ
	if (!GsSoundIsBgmStop(gm_sound_jingle_bgm_scb) &&
			!GsSoundIsBgmPause(gm_sound_jingle_bgm_scb)) {
		GmSoundPauseBGMJingle(bgm_fade_out_frame);
	}
#else
	// BGMポーズ
	if (!GsSoundIsBgmStop(gm_sound_bgm_scb) &&
			!(gm_sound_flag & GMD_SOUND_FLAG_BGM_PAUSE_VOL_0_MASK)) {
		if (bgm_fade_out_frame) {
			gmSoundSetBGMFade(&gm_sound_bgm_scb, 1.f, 0.f, bgm_fade_out_frame);
		}
		else {
			gmSoundSetBGMFadeEnd(gm_sound_bgm_scb);
			GsSoundScbSetVolume(gm_sound_bgm_scb, 0.f);
			GsSoundScbSetSeqMute(gm_sound_bgm_scb, TRUE);
		}
	}
	gm_sound_flag |= GMD_SOUND_FLAG_1SHOT_BGM_PAUSE_VOL_0;
	// BGMタイプジングルポーズ
	if ((!GsSoundIsBgmStop(gm_sound_jingle_bgm_scb) ||
				(gm_sound_flag & (GMD_SOUND_FLAG_INVINCIBLE | GMD_SOUND_FLAG_OBORE))) &&	// 同時に発行すると判別できない時がある為
			!(gm_sound_flag & GMD_SOUND_FLAG_JNGLBGM_PAUSE_VOL_0_MASK)) {
		if (bgm_fade_out_frame) {
			gmSoundSetBGMFade(&gm_sound_jingle_bgm_scb, 1.f, 0.f, bgm_fade_out_frame);
		}
		else {
			gmSoundSetBGMFadeEnd(gm_sound_jingle_bgm_scb);
			GsSoundScbSetVolume(gm_sound_jingle_bgm_scb, 0.f);
			GsSoundScbSetSeqMute(gm_sound_jingle_bgm_scb, TRUE);
		}
	}
	gm_sound_flag |= GMD_SOUND_FLAG_1SHOT_JNGLBGM_PAUSE_VOL_0;
#endif

	if (gm_sound_1shot_tcb) {
		// ジングル停止
		GmSoundStopJingle(0);
	}
	else {
		// ジングル再生終了管理
		gm_sound_1shot_tcb = MTM_TASK_MAKE_TCB(gmSound1ShotJingleFunc, gmSound1ShotJingleDest, 0/*flag*/,
						GMD_TASK_PAUSELEVEL_DEF, GMD_TASK_PRIO_SOUND, GMD_TASK_GROUP_SOUND,
						sizeof(GMS_SOUND_1SHOT_JINGLE_WORK), "GM_SOUND_1SH");
	}

	// ワーククリア
	jingle_1shot_work = (GMS_SOUND_1SHOT_JINGLE_WORK*)mtTaskGetTcbWork(gm_sound_1shot_tcb);
	MI_CpuClear8(jingle_1shot_work, sizeof(GMS_SOUND_1SHOT_JINGLE_WORK));

	// ジングル再生
	GmSoundPlayJingle(jngl_idx, jingle_fade_in_frame);

	// BGMフェードイン時間保存
	jingle_1shot_work->bgm_fade_in_frame = bgm_fade_in_frame;
}

// =======================================================================
// gmSound1ShotJingleFunc
/*!
 *	1Shotジングル管理
 *
 *	@param	TCB
 */
// =======================================================================
void gmSound1ShotJingleFunc(MTS_TASK_TCB *tcb)
{
	GMS_SOUND_1SHOT_JINGLE_WORK	*jingle_1shot_work;

	if (gm_sound_flag & GMD_SOUND_FLAG_ALL_PAUSE) {
		// 全体ポーズ中なので終了チェックを行わない
		return;
	}

	jingle_1shot_work = (GMS_SOUND_1SHOT_JINGLE_WORK*)mtTaskGetTcbWork(gm_sound_1shot_tcb);

	// 暫定終了チェック
	if (GsSoundIsBgmStop(gm_sound_jingle_scb)) {
		// 終了
		GmSoundStopJingle(0);

		// BGM類ポーズフラグクリア
		gm_sound_flag &= ~GMD_SOUND_FLAG_BGM_PAUSE;

#if !GMD_SOUND_CHANGE_USE_VOL
		// BGMタイプジングル復帰チェック
		if (!GsSoundIsBgmStop(gm_sound_jingle_bgm_scb) &&
				GsSoundIsBgmPause(gm_sound_jingle_bgm_scb)) {
			GmSoundResumeBGMJingle(jingle_1shot_work->bgm_fade_in_frame);
		}
		// BGM復帰チェック
		else {
			if (!GsSoundIsBgmStop(gm_sound_bgm_scb) &&
					GsSoundIsBgmPause(gm_sound_bgm_scb)) {
				GmSoundResumeStageBGM(jingle_1shot_work->bgm_fade_in_frame);
			}
		}
#else
		// BGMタイプジングル復帰チェック
		if ((!GsSoundIsBgmStop(gm_sound_jingle_bgm_scb) ||
				(gm_sound_flag & (GMD_SOUND_FLAG_INVINCIBLE | GMD_SOUND_FLAG_OBORE))) &&	// 同時に発行すると判別できない時がある為
				(gm_sound_flag & GMD_SOUND_FLAG_1SHOT_JNGLBGM_PAUSE_VOL_0) &&
				!(gm_sound_flag & (GMD_SOUND_FLAG_JNGLBGM_PAUSE_VOL_0_MASK & ~GMD_SOUND_FLAG_1SHOT_JNGLBGM_PAUSE_VOL_0))) {
			// 1shotジングルのポーズフラグしかない時 ジングルBGM復旧
			gmSoundSetBGMFade(&gm_sound_jingle_bgm_scb, 0.f, 1.f, jingle_1shot_work->bgm_fade_in_frame);
			GsSoundScbSetSeqMute(gm_sound_jingle_bgm_scb, FALSE);
		}
		// BGM復帰チェック
		else if (!GsSoundIsBgmStop(gm_sound_bgm_scb) &&
				(gm_sound_flag & GMD_SOUND_FLAG_1SHOT_BGM_PAUSE_VOL_0) &&
				!(gm_sound_flag & (GMD_SOUND_FLAG_BGM_PAUSE_VOL_0_MASK & ~GMD_SOUND_FLAG_1SHOT_BGM_PAUSE_VOL_0))) {
			// 1shotジングルのポーズフラグしかない時 BGM復旧
			gmSoundSetBGMFade(&gm_sound_bgm_scb, 0.f, 1.f, jingle_1shot_work->bgm_fade_in_frame);
			GsSoundScbSetSeqMute(gm_sound_bgm_scb, FALSE);
		}
		gm_sound_flag &= ~(GMD_SOUND_FLAG_1SHOT_JNGLBGM_PAUSE_VOL_0 | GMD_SOUND_FLAG_1SHOT_BGM_PAUSE_VOL_0);
#endif

		// タスク破棄
		mtTaskClearTcb(tcb);
	}
}

// =======================================================================
// gmSound1ShotJingleDest
/*!
 *	1Shotジングル管理
 *
 *	@param	TCB
 */
// =======================================================================
void gmSound1ShotJingleDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// TCBワーククリア
	MTM_ASSERT(gm_sound_1shot_tcb);
	gm_sound_1shot_tcb = NULL;
}


// ==========================================================================
// サウンドフェード
// ==========================================================================
// ==========================================================================
// gmSoundSetBGMFade
/*!
 *	BGMフェード設定
 *
 *	@param	snd_scb	[in]	SCB
 */
// ==========================================================================
void gmSoundSetBGMFade(GSS_SND_SCB **snd_scb, float start_vol, float end_vol, s32 frame)
{
	GMS_SOUND_BGM_FADE_MGR_WORK	*mgr_work;
	GMS_SOUND_BGM_FADE_WORK		*fade_work;

	if (GsSoundIsBgmStop(gm_sound_bgm_scb)) {
		return;
	}

	if (GsSoundIsBgmPause(gm_sound_bgm_scb)) {
		return;
	}

	// 既に登録されている場合は終了しておく
	gmSoundSetBGMFadeEnd(*snd_scb);

	if (frame <= 0) {
		MTM_ASSERT(0);
		frame = 1;
	}

	if (gm_sound_bgm_fade_tcb == NULL) {
		// フェード管理
		gm_sound_bgm_fade_tcb = MTM_TASK_MAKE_TCB(gmSoundBGMFadeFunc, gmSoundBGMFadeDest, 0/*flag*/,
							GMD_TASK_PAUSELEVEL_DEF, GMD_TASK_PRIO_SOUND, GMD_TASK_GROUP_SOUND,
							sizeof(GMS_SOUND_BGM_FADE_MGR_WORK), "GM_SOUND_BFADE");
		mgr_work = (GMS_SOUND_BGM_FADE_MGR_WORK*)mtTaskGetTcbWork(gm_sound_bgm_fade_tcb);
		MI_CpuClear8(mgr_work, sizeof(GMS_SOUND_BGM_FADE_MGR_WORK));
	}
	mgr_work = (GMS_SOUND_BGM_FADE_MGR_WORK*)mtTaskGetTcbWork(gm_sound_bgm_fade_tcb);

	// フェードワーク設定
	fade_work = (GMS_SOUND_BGM_FADE_WORK*)amMemAlloc(sizeof(GMS_SOUND_BGM_FADE_WORK));
	MI_CpuClear8(fade_work, sizeof(GMS_SOUND_BGM_FADE_WORK));

	fade_work->snd_scb		= snd_scb;
	fade_work->start_vol	= start_vol;
	fade_work->end_vol		= end_vol;
	fade_work->frame		= frame;
	fade_work->fade_spd		= (end_vol - start_vol) / (float)frame;
	fade_work->now_vol		= start_vol;

	// 連結
	gmSoundBGMFadeAttachList(mgr_work, fade_work);
}

// ==========================================================================
// gmSoundSetBGMFadeEnd
/*!
 *	指定のSCBのフェード処理を強制終了
 *
 *	@param	snd_scb	[in]	SCB
 */
// ==========================================================================
void gmSoundSetBGMFadeEnd(GSS_SND_SCB *snd_scb)
{
	GMS_SOUND_BGM_FADE_MGR_WORK	*mgr_work;
	GMS_SOUND_BGM_FADE_WORK		*fade_work, *next_fade_work;

	if (gm_sound_bgm_fade_tcb) {
		mgr_work = (GMS_SOUND_BGM_FADE_MGR_WORK*)mtTaskGetTcbWork(gm_sound_bgm_fade_tcb);
		fade_work = mgr_work->head;

		while (fade_work) {
			next_fade_work = fade_work->next;

			if (*fade_work->snd_scb == snd_scb) {
				// 終了
				gmSoundBGMFadeDetachList(mgr_work, fade_work);
				amMemFree(fade_work);
			}

			fade_work = next_fade_work;
		}

		if (mgr_work->num <= 0) {
			// 処理終了
			mtTaskClearTcb(gm_sound_bgm_fade_tcb);
		}
	}
}

// =======================================================================
// gmSoundBGMFadeFunc
/*!
 *	BGMフェード管理
 *
 *	@param	TCB
 */
// =======================================================================
void gmSoundBGMFadeFunc(MTS_TASK_TCB *tcb)
{
	GMS_SOUND_BGM_FADE_MGR_WORK	*mgr_work;
	GMS_SOUND_BGM_FADE_WORK		*fade_work, *next_fade_work;

	mgr_work = (GMS_SOUND_BGM_FADE_MGR_WORK*)mtTaskGetTcbWork(tcb);
	fade_work = mgr_work->head;

	while (fade_work) {
		next_fade_work = fade_work->next;

		fade_work->now_vol += fade_work->fade_spd;
		fade_work->frame--;

		if (fade_work->frame <= 0) {
			fade_work->now_vol = fade_work->end_vol;
		}
		GsSoundScbSetVolume(*fade_work->snd_scb, fade_work->now_vol);

		if (fade_work->frame <= 0 || GsSoundIsBgmStop(*fade_work->snd_scb)) {
			// 終了
			if (fade_work->now_vol > 0.f) {
				GsSoundScbSetSeqMute(*fade_work->snd_scb, FALSE);
			}
			else {
				GsSoundScbSetSeqMute(*fade_work->snd_scb, TRUE);
			}
			gmSoundBGMFadeDetachList(mgr_work, fade_work);
			amMemFree(fade_work);
		}

		fade_work = next_fade_work;
	}

	if (mgr_work->num <= 0) {
		// 処理終了
		mtTaskClearTcb(tcb);
	}
}

// =======================================================================
// gmSoundBGMFadeDest
/*!
 *	BGMフェードデストラクタ
 *
 *	@param	TCB
 */
// =======================================================================
void gmSoundBGMFadeDest(MTS_TASK_TCB *tcb)
{
	GMS_SOUND_BGM_FADE_MGR_WORK	*mgr_work;
	GMS_SOUND_BGM_FADE_WORK		*fade_work, *next_fade_work;

	// リストの登録が残っていたら開放
	mgr_work = (GMS_SOUND_BGM_FADE_MGR_WORK*)mtTaskGetTcbWork(tcb);
	fade_work = mgr_work->head;
	while (fade_work) {
		next_fade_work = fade_work->next;
		gmSoundBGMFadeDetachList(mgr_work, fade_work);
		amMemFree(fade_work);
		fade_work = next_fade_work;
	}

	if (gm_sound_bgm_fade_tcb == tcb) {
		gm_sound_bgm_fade_tcb = NULL;
	}
}

// ==========================================================================
// gmSoundBGMFadeAttachList
/*!
 *	BGMフェード リスト連結
 *
 *	@param	mgr_work	[in]	マネージャーワーク
 *	@param	fade_work	[in]	フェードワーク
 */
// ==========================================================================
void gmSoundBGMFadeAttachList(GMS_SOUND_BGM_FADE_MGR_WORK *mgr_work, GMS_SOUND_BGM_FADE_WORK *fade_work)
{
	if (mgr_work->tail) {
		fade_work->prev			= mgr_work->tail;
		mgr_work->tail->next	= fade_work;
		mgr_work->tail			= fade_work;
	}
	else {
		mgr_work->head = fade_work;
		mgr_work->tail = fade_work;
	}

	mgr_work->num++;
}

// ==========================================================================
// gmSoundBGMFadeDetachList
/*!
 *	BGMフェード リスト切り離し
 *
 *	@param	mgr_work	[in]	マネージャーワーク
 *	@param	fade_work	[in]	フェードワーク
 */
// ==========================================================================
void gmSoundBGMFadeDetachList(GMS_SOUND_BGM_FADE_MGR_WORK *mgr_work, GMS_SOUND_BGM_FADE_WORK *fade_work)
{
	if (fade_work->prev) {
		fade_work->prev->next = fade_work->next;
	}
	else {
		mgr_work->head = fade_work->next;
	}

	if (fade_work->next) {
		fade_work->next->prev = fade_work->prev;
	}
	else {
		mgr_work->tail = fade_work->prev;
	}

	mgr_work->num--;

	MTM_ASSERT(mgr_work->num >= 0);
}


// =======================================================================
// ボス戦勝利時BGM変更
// =======================================================================
// =======================================================================
// gmSoundBGMWinBossFunc
/*!
 *	ボス戦勝利時BGM変更管理
 *
 *	@param	TCB
 */
// =======================================================================
void gmSoundBGMWinBossFunc(MTS_TASK_TCB *tcb)
{
#if defined(GMD_SOUND_NO_PLAY_BGM)
	return;
#endif
	GMS_SOUND_BGM_WIN_BOSS_MGR_WORK	*mgr_work;

	mgr_work = (GMS_SOUND_BGM_WIN_BOSS_MGR_WORK*)mtTaskGetTcbWork(tcb);

	if (gm_sound_flag & GMD_SOUND_FLAG_ALL_PAUSE) {
		// 全体ポーズ中なので終了チェックを行わない
		return;
	}

	mgr_work->timer--;
	if (mgr_work->timer <= 0) {
		// BGM切り替え
		GSS_SND_SCB	*temp_scb;
		BOOL		b_pause = FALSE, b_vol_0 = FALSE;

		// 念のためサブの方を停止しておく
		GsSoundStopBgm(gm_sound_bgm_sub_scb, 0);

		// BGM停止
		if (GsSoundIsBgmPause(gm_sound_bgm_scb)) {
			b_pause = TRUE;
		}
		if (gm_sound_flag & GMD_SOUND_FLAG_BGM_PAUSE_VOL_0_MASK) {
			b_vol_0 = TRUE;
		}
		if (b_pause | b_vol_0) {
			GmSoundStopStageBGM(0);
		}
		else {
			GmSoundStopStageBGM(GMD_SOUND_WIN_BOSS_CHANGE_FADE);
		}

		// サブとメインSCBを入れ替える
		temp_scb = gm_sound_bgm_scb;
		gm_sound_bgm_scb = gm_sound_bgm_sub_scb;
		gm_sound_bgm_sub_scb = temp_scb;
		
		// ボリューム初期化
		GsSoundScbSetVolume(gm_sound_bgm_scb, 1.f);
		GsSoundScbSetSeqMute(gm_sound_bgm_scb, FALSE);
		// BGM再生
		GsSoundPlayBgm(gm_sound_bgm_scb,
					   gm_sound_bgm_win_boss_name_list[GMM_MAIN_GET_ZONE_TYPE()],
					   GMD_SOUND_WIN_BOSS_CHANGE_FADE);
		// サウンドシステムミュート対応
		gm_sound_bgm_scb->flag |= GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM;

		if (b_pause) {
			// いきなりポーズしておく
			GmSoundPauseStageBGM(0);
		}
		if (b_vol_0) {
			// いきなりボリューム0に
			gmSoundSetBGMFadeEnd(gm_sound_bgm_scb);
			GsSoundScbSetVolume(gm_sound_bgm_scb, 0.f);
			GsSoundScbSetSeqMute(gm_sound_bgm_scb, TRUE);
		}

		// 処理終了
		mtTaskClearTcb(tcb);
	}
}

// =======================================================================
// gmSoundBGMWinBossDest
/*!
 *	ボス戦勝利時BGM変更デストラクタ
 *
 *	@param	TCB
 */
// =======================================================================
void gmSoundBGMWinBossDest(MTS_TASK_TCB *tcb)
{
	if (tcb == gm_sound_bgm_win_boss_tcb) {
		gm_sound_bgm_win_boss_tcb = NULL;
	}
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
