// =======================================================================
/*!
  @file	gsSound.cpp
  @brief プラットフォーム共通サウンドモジュール

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gsSound.cpp 186 2011-05-26 19:49:18Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akMath.h"
#include "gsMainSys.h"
#include "gsSystemBgm.h"
#include "gsSound.h"

#include "Sonic4_Utility.h"

/*------ Macros --------------------------------------------------------*/
/* デバッグ */
#if defined(MTD_DEBUG)
#define GSD_SND_DEBUG_ASSERT_SE_HANDLE_OVER		//! SEハンドルオーバーアサートを有効にする
#endif /* defined(MTD_DEBUG) */

#define GSD_SND_DEBUG_PRINT_POS_X		(50)	//! デバッグ表示位置X
#define GSD_SND_DEBUG_PRINT_POS_Y		(43)	//! デバッグ表示位置Y

//############ 共通 ###########################################################
/* 定義値 */

//! 同時にデータロード待ちできる数
#define GSD_SND_LOAD_NUM_MAX		(16)

#if _IPHONE
#define GSD_SND_SE_WAIT_CTRL  (0 & _IPHONE)
#define GSD_SND_SE_PLAY_LIMIT_CTRL  (1 & _IPHONE)
#endif // _IPHONE

#if GSD_SND_SE_WAIT_CTRL
#define GSD_SND_SE_WAIT_CUENAME_MAX_LENGTH  (14) //! 保存キュー名

#define GSD_SND_SE_WAIT_FRAME_ALL		(5) //!< 全SEを再生しないフレーム数
#define GSD_SND_SE_WAIT_FRAME_SAME		(8) //!< 同一SEを再生しないフレーム数
#endif // GSD_SND_SE_WAIT_CTRL

#if GSD_SND_SE_PLAY_LIMIT_CTRL
#define GSD_SND_SE_PLAY_LIMIT_NUM		(8)	//!< 最大サウンド再生数(BGM分を考慮、強制再生はこの影響を受けない)
#endif // GSD_SND_SE_PLAY_LIMIT_CTRL

#if _IPHONE
#define GSD_SND_SUSPEND_WAIT_COUNT						(8)		//!< 8フレ待機
#define GSD_SND_NOPLAY_ERROR_STATE_RESTART_LIMIT		(90)	//!< CRIAuido再起動カウント
#define GSD_SND_NOPLAY_ERROR_CLEAR_SE_CRI_RESTART_LIMIT (1000)	//!< CRIAudio再起動カウント・SE開放用
#endif // _IPHONE


//############ SEハンドル関連 #################################################

//! エラーSEハンドル
#define GSD_SND_ERROR_SE_HANDLE				(&gs_sound_se_handle_error)

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

#if GSD_SND_SE_WAIT_CTRL
//! SE連続再生抑制
struct tag_GSS_SND_SE_WAIT_CTRL {
Uint8 wait_name; //!< GsPlaySoundSe時ウェイト 
Uint8 wait_id; //!< GsPlaySoundSeById時ウェイト 
char set_name[GSD_SND_SE_WAIT_CUENAME_MAX_LENGTH]; //!< 再生キュー名(暫定14文字) 
Uint32 set_id; //!< 再生ID 
};
typedef struct tag_GSS_SND_SE_WAIT_CTRL GSS_SND_SE_WAIT_CTRL;
#endif // GSD_SND_SE_WAIT_CTRL

#if GSD_SND_SE_PLAY_LIMIT_CTRL
//!< SE最大再生数抑制
typedef struct tag_GSS_SND_SE_PLAY_LIMIT_CTRL {
	s32 count; //!< 現在再生個数
} GSS_SND_SE_PLAY_LIMIT_CTRL;
#endif // GSD_SND_SE_PLAY_LIMIT_CTRL

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
static void gsSoundProcMain(MTS_TASK_TCB *tcb);
static void gsSoundInitSystemMainInfo(void);
static void gsSoundResetSystemMainInfo(void);
static void gsSoundClearSystemMainInfo(void);
#if _WII
static void gsSoundWiiSaveAuxReturnVol(void);
static void gsSoundWiiRestoreAuxReturnVol(void);
static void gsSoundWiiSaveMainOutVolume(void);
static void gsSoundWiiRestoreMainOutVolume(void);
static Float gsSoundWiiGetSavedMainOutVolume(void);
#endif /* _WII */
static inline void gsSoundSetEnableSystemControlVolume(BOOL enable);
static inline BOOL gsSoundIsSystemControlVolumeEnabled(void);
static void gsSoundUpdateSystemControlVolume(void);
static Float gsSoundGetGlobalVolume(void);
static Float gsSoundGetSndScbMuteVolume(const GSS_SND_SCB *scb);
static void gsSoundInitSndScbHeap(void);
static void gsSoundResetSndScbHeap(void);
static Uint32 gsSndGetFreeScbNum(void);
static void gsSoundInitSndScb(GSS_SND_SCB *scb, Sint32 scb_no, GSE_SND_DATA_TYPE snd_data_type);
static void gsSoundClearSndScb(GSS_SND_SCB *scb, Sint32 auply_no);
static void gsSoundUpdateSndScb(GSS_SND_SCB *scb);
static void gsSoundUpdateSndScbStatus(GSS_SND_SCB *scb);
static BOOL gsSoundCheckSndScbPause(const GSS_SND_SCB *scb);
static BOOL gsSoundCheckSndScbStop(const GSS_SND_SCB *scb);
static Sint32 gsSoundGetAuplyNo(Sint32 scb_no);
#if _WII	// Warning対策（Wii版でしか参照されないので）
static void gsSoundPauseAllScb(GSE_SND_SCB_PAUSE_LEVEL pause_level);
static void gsSoundResumeAllScb(GSE_SND_SCB_PAUSE_LEVEL pause_level);
#endif /* _WII (Warning対策) */
static void gsSoundCriStrmSetFadeIn(GSS_SND_SCB *scb, Sint32 fade_frame);
static void gsSoundCriStrmStop(GSS_SND_SCB *scb, Sint32 fade_frame, BOOL is_takeover=FALSE);
static void gsSoundCriStrmPause(GSS_SND_SCB *scb, Sint32 fade_frame);
static void gsSoundCriStrmResume(GSS_SND_SCB *scb, Sint32 fade_frame);
static void gsSoundUpdateSndCtrl(GSS_SND_CTRL_PARAM *snd_ctrl_param, CriAuPlayer *au_player);
static void gsSoundSeHandleUpdateVolume(GSS_SND_SE_HANDLE *se_handle);
static void gsSoundUpdateVolume(void);
static void gsSoundInitSeHandleHeap(void);
static void gsSoundResetSeHandleHeap(void);
static void gsSoundClearSeHandleHeap(void);
static Uint32 gsSoundGetFreeSeHandleNum(void);
static void gsSoundInitSeHandle(GSS_SND_SE_HANDLE *se_handle, BOOL b_reset=FALSE);
static void gsSoundClearSeHandle(GSS_SND_SE_HANDLE *se_handle, BOOL b_takeover_cue=FALSE);
static void gsSoundUpdateSndSeHandle(GSS_SND_SE_HANDLE *se_handle);
static void gsSoundUpdateSeHandleStatus(GSS_SND_SE_HANDLE *se_handle);
static BOOL gsSoundCheckSeHandlePause(const GSS_SND_SE_HANDLE *se_handle);
static BOOL gsSoundCheckSeHandleStop(const GSS_SND_SE_HANDLE *se_handle);
static inline BOOL gsSoundIsSeHandleCueSet(const GSS_SND_SE_HANDLE *se_handle);
static inline GSS_SND_SE_HANDLE* gsSoundGetDefaultSeHandle(void);
static void gsSoundCriSeSetFadeIn(GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame);
static void gsSoundCriSeStop(GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame, BOOL is_immediate,
							 BOOL is_takeover=FALSE);
static void gsSoundCriSePause(GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame);
static void gsSoundCriSeResume(GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame);

#if _IPHONE
static void gsSoundPlaySe(const char *se_name, Uint32 se_id, GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame);
static BOOL gsSoundIsSystemSuspendWait(void);
static void gsSoundUpdateSystemSuspendWait(void);
#endif // _IPHONE

#if _WII
//############ HBM関連 ########################################################
static void gsSoundWiiStopDesignatedSoundForHbm(void);
static void gsSoundWiiUpdateHBMSeq(void);
// HBMへ移行時のシーケンス
static void gsSoundWiiEnterHBMProcInit(void);
static void gsSoundWiiEnterHBMProcFadeOut(void);
static void gsSoundWiiEnterHBMProcFinalize(void);
// HBMから復帰時のシーケンス
static void gsSoundWiiLeaveHBMProcInit(void);
static void gsSoundWiiLeaveHBMProcStartFadeIn(void);
static void gsSoundWiiLeaveHBMProcFadeIn(void);
#endif /* _WII */

//mpp -----------------------------

#include "mppUtil.h"

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
//! GSサウンドシステムフレーム処理タスクTCB
MTS_TASK_TCB	*gs_sound_tcb	= NULL;

//! サウンドシステムメイン情報
GSS_SND_SYS_MAIN_INFO gs_sound_sys_main_info	= {0};

//! SCBヒープ
GSS_SND_SCB gs_sound_scb_heap[GSD_SND_SCB_MAX];
//! SCBの使用済みフラグ
Uint8 gs_sound_scb_heap_usage_flag[(GSD_SND_SCB_MAX+7)/8]	= {0};

//! SEハンドルヒープ
GSS_SND_SE_HANDLE gs_sound_se_handle_heap[GSD_SND_SE_HANDLE_MAX];
//! SEハンドルの使用済みフラグ
Uint8 gs_sound_se_handle_heap_usage_flag[(GSD_SND_SE_HANDLE_MAX+7)/8]	= {0};
//! エラーSEハンドル（SEハンドルの取得に失敗したときのハンドル）
GSS_SND_SE_HANDLE gs_sound_se_handle_error;
//! デフォルトSEハンドル（実体はSEハンドルヒープから確保したハンドル）
GSS_SND_SE_HANDLE *gs_sound_se_handle_default	= NULL;

//! ボリューム
Float gs_sound_volume[GSE_SND_TYPE_MAX]	= {0};

#if GSD_SND_SE_WAIT_CTRL
//!< SE連続再生抑制 
GSS_SND_SE_WAIT_CTRL gs_sound_se_wait_ctrl;
#endif // GSD_SND_SE_WAIT_CTRL


#if GSD_SND_SE_PLAY_LIMIT_CTRL
GSS_SND_SE_PLAY_LIMIT_CTRL gs_sound_se_play_limit_ctrl;
#endif // GSD_SND_SE_PLAY_LIMIT_CTRL


/*------ Global Functions ----------------------------------------------*/

// =======================================================================
// GsSoundBuildSeInit
/*!
  SEサウンドデータ構築 開始
  
  @param snd_data_work	[in]	GSサウンドデータワーク
  @param csb_type		[in]	CSBタイプ
  @param csb_file_path	[in]	CSBファイルパス
  @param prio			[in]	ロード待ちタスクのプライオリティ
  
  @note
  SEサウンドデータを構築します。
  GsSoundBuildSeUpdate()で完了待ちを行ってください。
 */
// =======================================================================
void GsSoundBuildSeInit(GSS_SND_DATA_WORK *snd_data_work, AMD_CRIAUDIO_CSBTYPE csb_type,
						const char *csb_file_path, Sint32 prio)
{
	MTM_ASSERT(snd_data_work);
	MTM_ASSERT(amCriAudioGetGlobal()->CueSheet[csb_type] == NULL);
	
	snd_data_work->is_active	= TRUE;
	
	snd_data_work->data_type	= GSE_SND_DATA_TYPE_CRIAUDIO;
	snd_data_work->csb_type	= csb_type;
	
	amCriAudioCreateCueSheet(const_cast<char*>(csb_file_path), AME_CRIAUDIO_CSB_GAME, prio);
}

// =======================================================================
// GsSoundBuildSeUpdate
/*!
  SEサウンドデータ構築 更新
  
  @param snd_data_work	[io]	GSサウンドデータワーク
  
  @retval TRUE	終了
  @retval FALSE	構築中
 */
// =======================================================================
BOOL GsSoundBuildSeUpdate(GSS_SND_DATA_WORK *snd_data_work)
{
	MTM_ASSERT(snd_data_work);
	
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if	= amCriAudioGetGlobal();
	
	if (cri_audio_if->loadState[snd_data_work->csb_type] == CriAuCueSheet::LOAD_STATUS_COMPLETE) {
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// GsSoundBuildBgmInit
/*!
  BGMサウンドデータ構築 開始
  
  @param snd_data_work	[in]	GSサウンドデータワーク
  @param file_path		[in]	ファイルパス（CSBもしくはBRSAR）
  @param cpk_file_path	[in]	CPKファイルパス(WIIの場合はNULLを指定)
  @param prio			[in]	ロード待ちタスクのプライオリティ
  @param data_type		[in]	データタイプ(GSE_SND_DATA_TYPE_XXX)
  
  @note
  BGMデータに応じたdata_typeを指定してください。
  Wii以外のプラットフォームの場合にdata_type==GSE_SND_DATA_TYPE_NW4Rを指定すると
  アサートに失敗します。
  GsSoundBuildBgmUpdate()で完了待ちを行ってください。
 */
// =======================================================================
void GsSoundBuildBgmInit(GSS_SND_DATA_WORK *snd_data_work,
						 const char *file_path, const char *cpk_file_path,
						 Sint32 prio, GSE_SND_DATA_TYPE data_type)
{
	MTM_ASSERT(snd_data_work);
	
	snd_data_work->is_active	= TRUE;
	
	snd_data_work->data_type	= data_type;
	
	if (data_type == GSE_SND_DATA_TYPE_CRIAUDIO) {
#if GSD_SND_NO_USE_STREAM_BGM
		snd_data_work->csb_type	= AME_CRIAUDIO_CSB_SYSTEM;
		amCriAudioCreateCueSheet(const_cast<char*>(file_path), snd_data_work->csb_type, prio);
#else
		snd_data_work->csb_type	= AME_CRIAUDIO_CSB_STREAM;
		amCriAudioCreateCueSheet(const_cast<char*>(file_path), snd_data_work->csb_type, prio);
		amCriAudioBindCPK(const_cast<char*>(cpk_file_path), prio);
#endif // GSD_SND_NO_USE_STREAM_BGM
	}
	else {
#if _WII
		MTM_ASSERT(data_type == GSE_SND_DATA_TYPE_NW4R);
		MTM_ASSERT(cpk_file_path == NULL);
		AkRvlSndStartLoadDataMem(&snd_data_work->load_work, file_path, prio);
#else
		MTM_ASSERT(FALSE);
#endif /* _WII */
	}
}

// =======================================================================
// GsSoundBuildBgmUpdate
/*!
  BGMサウンドデータ構築 更新
  
  @param snd_data_work	[io]	GSサウンドデータワーク
  
  @retval TRUE	終了
  @retval FALSE	構築中
  
  @note
  snd_data_workは完了時に自動的に初期化されます。
 */
// =======================================================================
BOOL GsSoundBuildBgmUpdate(GSS_SND_DATA_WORK *snd_data_work)
{
	MTM_ASSERT(snd_data_work);
	
	if (!snd_data_work->is_active) {
		return TRUE;
	}
	
	if (snd_data_work->data_type == GSE_SND_DATA_TYPE_CRIAUDIO) {
		AMS_CRIAUDIO_INTERFACE	*cri_audio_if	= amCriAudioGetGlobal();
		
#if GSD_SND_NO_USE_STREAM_BGM
		if (cri_audio_if->loadState[AME_CRIAUDIO_CSB_SYSTEM] != CriAuCueSheet::LOAD_STATUS_COMPLETE) {
			return FALSE;
		}
#if 0
		// プレイヤーを作成(CPK使用の場合はAliceの方で作られている)
		for (Uint32 i = 0; i < AME_CRIAUDIO_STRM_MAX; i++) {
			cri_audio_if->auply[i] = CriAuPlayer::Create(cri_audio_if->auobj, cri_audio_if->err);
		}
#endif // 0
#else
		if (cri_audio_if->loadState[AME_CRIAUDIO_CSB_STREAM] != CriAuCueSheet::LOAD_STATUS_COMPLETE) {
			return FALSE;
		}
		if (cri_audio_if->binder_status != CRIFSBINDER_STATUS_COMPLETE) {
			return FALSE;
		}
#endif // GSD_SND_NO_USE_STREAM_BGM
		// 完了時にここに到達
#if _IPHONE
		// volume init
		gsSoundUpdateVolume();
#endif // _IPHONE
		
		// データワーククリア
		// （フラッシュ時にはsnd_data_workの情報は必要ないのでここでクリアする）
		GsSoundInitDataWork(snd_data_work);
		
		return TRUE;
	}
	else {
#if _WII
		if (AkRvlSndIsLoadDataMemValid(&snd_data_work->load_work)) {
			if (!AkRvlSndCheckLoadDataMemComplete(&snd_data_work->load_work)) {
				return FALSE;
			}
		}
		
		// 完了時にここに到達
		
		// メモリサウンドアーカイブ初期化
		AkRvlSndInitArcMem(AKE_RVL_SND_TYPE_BGM, snd_data_work->load_work.buf);
		
		// データロード完了
		AkRvlSndFinalizeLoadDataMem(&snd_data_work->load_work);
		
		// データワーククリア
		// （フラッシュ時にはsnd_data_workの情報は必要ないのでここでクリアする）
		GsSoundInitDataWork(snd_data_work);
		
		return TRUE;
#else
		MTM_ASSERT(FALSE);
		return TRUE;
#endif /* _WII */
	}
}


// =======================================================================
// GsSoundFlushSe
/*!
  サウンドデータフラッシュ
  
  @param snd_data_work	[io]	GSサウンドデータワーク
  
  @note
  完了待ちを行う必要はありません。
  データワーク単位で呼び出す必要があります。
 */
// =======================================================================
void GsSoundFlushSe(GSS_SND_DATA_WORK *snd_data_work)
{
	amCriAudioDestroyCueSheet(snd_data_work->csb_type);
	
	// データワーククリア
	GsSoundInitDataWork(snd_data_work);
}

// =======================================================================
// GsSoundFlushBgm
/*!
  サウンドデータフラッシュ
  
  @note
  完了待ちを行う必要はありません。
  全てのBGMデータをフラッシュします。
 */
// =======================================================================
void GsSoundFlushBgm(void)
{
#if GSD_SND_NO_USE_STREAM_BGM
	amCriAudioDestroyCueSheet(AME_CRIAUDIO_CSB_SYSTEM);
#else
	amCriAudioDestroyCueSheet(AME_CRIAUDIO_CSB_STREAM);
#endif // GSD_SND_NO_USE_STREAM_BGM
#if _WII
	
	// サウンドアーカイブ解放
	AkRvlSndReleaseArcMem(AKE_RVL_SND_TYPE_BGM);
	
	// サウンドヒープ全体をまとめてクリア
	AkRvlSndReset();
#endif /* _WII */
}


// =======================================================================
// GsSoundInitDataWork
/*!
  サウンドデータワーククリア
  
  @param snd_data_work	[io]	GSサウンドデータワーク
  
  @note
  パラメータ等をクリアしてニュートラルな状態にします。
  SEデータの解放時にGsSoundBuildSe***()後の状態が参照されますので、
  未解放のデータと関連付けれられているワークに対しては使用しないでください。
  （BGMの場合は問題ありません。）
 */
// =======================================================================
void GsSoundInitDataWork(GSS_SND_DATA_WORK *snd_data_work)
{
	MTM_ASSERT(snd_data_work);
	amZeroMemory(snd_data_work, sizeof(GSS_SND_DATA_WORK));
}

// =======================================================================
// GsSoundInit
/*!
  サウンドシステム初期化処理
  
  @note
  各種システムの初期化を行います。
  amCriAudioInit()は呼び出されません。
  amCriAudioInit()よりも後に呼び出してください。
 */
// =======================================================================
void GsSoundInit(void)
{
	GSS_MAIN_SYS_INFO	*main_sys_info	= GsGetMainSysInfo();
	
#if _WII
	// Wiiサウンドシステム初期化
	AkRvlSndInit(GSD_SND_NW4R_SOUND_HEAP_SIZE,
				 AKD_RVL_SND_SOUND_THREAD_DEFAULT_PRIO,
				 AKD_RVL_SND_DVD_THRED_DEFAULT_PRIO);
	
	// サウンドモードの設定
#if !GSD_SND_WII_SUPPORT_SRROUND_OUTPUT
	// サラウンド出力に対応しない場合は、
	// 本体がサラウンドに設定されているときはステレオ出力に切り替える
	nw4r::snd::OutputMode mode	= nw4r::snd::SoundSystem::GetOutputMode();
	if (mode == nw4r::snd::OUTPUT_MODE_DPL2) {
		nw4r::snd::SoundSystem::SetOutputMode(nw4r::snd::OUTPUT_MODE_STEREO);
	}
#endif /* GSD_SND_WII_SUPPORT_SRROUND_OUTPUT */
	
	// CRI Audio側に反映（内部情報を持っている場合を想定して、念のため）
	CriSoundRendererWii	*snd_renderer	=
		(CriSoundRendererWii*)amCriAudioGetGlobal()->sndrndr;
	switch (nw4r::snd::SoundSystem::GetOutputMode()) {
	case nw4r::snd::OUTPUT_MODE_MONO:
		snd_renderer->SetOutputMode_WII(CriSoundRendererWii::MONO);
		break;
		
	case nw4r::snd::OUTPUT_MODE_STEREO:
		snd_renderer->SetOutputMode_WII(CriSoundRendererWii::STEREO);
		break;
		
	case nw4r::snd::OUTPUT_MODE_DPL2:
		snd_renderer->SetOutputMode_WII(CriSoundRendererWii::DPL2);
		break;
		
	default:
		MTM_ASSERT(FALSE);
	}
#endif /* _WII */
	
	// メイン情報初期化
	gsSoundInitSystemMainInfo();
	
	// SCBヒープを初期化
	gsSoundInitSndScbHeap();
	
	// SEハンドルヒープを初期化
	gsSoundInitSeHandleHeap();
	
	// メインシステムのボリュームに初期値を設定
	main_sys_info->bgm_volume	= 1.0f;
	main_sys_info->se_volume	= 1.0f;
	
	// ボリューム初期化
	for (Sint32 i = 0; i < GSE_SND_TYPE_MAX; ++i) {
		GsSoundSetVolume((GSE_SND_TYPE)i, 1.f);
	}
	
#if GSD_SND_SE_WAIT_CTRL
	amZeroMemory(&gs_sound_se_wait_ctrl, sizeof(GSS_SND_SE_WAIT_CTRL));
#endif // GSD_SND_SE_WAIT_CTRL
#if GSD_SND_SE_PLAY_LIMIT_CTRL
	amZeroMemory(&gs_sound_se_play_limit_ctrl, sizeof(GSS_SND_SE_PLAY_LIMIT_CTRL));
#endif // GSD_SND_SE_PLAY_LIMIT_CTRL
}

// =======================================================================
// GsSoundHalt
/*!
  サウンド停止
  
  @note
  サウンドの再生などを全て停止します。
  SCB,SEハンドルヒープのリセットは行われません。
  システム自体は終了しないことに注意してください。
 */
// =======================================================================
void GsSoundHalt(void)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
	// BGM再生停止
	for (Sint32 i = 0; i < GSD_SND_SCB_MAX; ++i) {
		if (gs_sound_scb_heap[i].flag & GSD_SND_SCB_FLAG_INITIALIZED) {
			GsSoundStopBgm(&gs_sound_scb_heap[i]);
		}
	}
	
	// SE再生停止（CRI Audio で再生しているものをまとめて停止）
	cri_audio_if->auobj->Stop(CriAuObj::STOP_MODE_IMMEDIATE, cri_audio_if->err);
}

// =======================================================================
// GsSoundReset
/*!
  サウンドシステムリセット
  
  @note
  各種システムのリセット等を行います。
  全てのユーザ側で確保した全てのSCB,SEハンドルの解放が終えた後に呼び出してください。
 */
// =======================================================================
void GsSoundReset(void)
{
	// ボリュームリセット
	for (Sint32 i = 0; i < GSE_SND_TYPE_MAX; ++i) {
		GsSoundSetVolume((GSE_SND_TYPE)i, 1.f);
	}
	
	// SEハンドルヒープをリセット
	gsSoundResetSeHandleHeap();
	
	// SCBヒープをリセット
	gsSoundResetSndScbHeap();
	
#if _WII
	AkRvlSndReset();
#endif
	
	// メイン情報リセット
	gsSoundResetSystemMainInfo();
}

// =======================================================================
// GsSoundExit
/*!
  サウンドシステム終了処理
  
  @note
  各種システムの終了などを行います。
  amCriAudioExit()は呼び出されません。
  amCriAudioExit()よりも前に呼び出してください。
 */
// =======================================================================
void GsSoundExit(void)
{
	// ボリュームクリア
	for (Sint32 i = 0; i < GSE_SND_TYPE_MAX; ++i) {
		GsSoundSetVolume((GSE_SND_TYPE)i, 0.f);
	}
	
	// サウンドシステム停止
	GsSoundHalt();
	
	// フレーム処理がまだ実行されていたら終了する
	GsSoundEnd();
	
	// SEハンドルヒープをクリア
	gsSoundClearSeHandleHeap();
	
	// SCBヒープをクリア
	gsSoundResetSndScbHeap();
	
	// メイン情報クリア
	gsSoundClearSystemMainInfo();
	
#if _WII
	// Wii用サウンドシステム停止
	AkRvlSndExit();
#endif /* _WII */
}

// =======================================================================
// GsSoundBegin
/*!
  サウンドメイン処理開始
  
  @param task_pause_level	[in]	サウンドタスク ポーズレベル
  @param task_prio			[in]	サウンドタスク プライオリティ
  @param task_group			[in]	サウンドタスク グループ
  
  @note
  サウンドのフレーム更新処理を開始します。
 */
// =======================================================================
void GsSoundBegin(Uint16 task_pause_level, Uint32 task_prio, Sint32 task_group)
{
	MTM_ASSERT(gs_sound_tcb == NULL);
	
	GsSoundSetVolumeFromMainSysInfo();
	
	gs_sound_tcb	= MTM_TASK_MAKE_TCB(gsSoundProcMain,
										NULL,
										0,//flag
										task_pause_level,
										task_prio,
										task_group,
										0,//work_size
										"GS_SND_MAIN");
}

// =======================================================================
// GsSoundEnd
/*!
  サウンドメイン処理終了
  
  @note
  サウンドのフレーム更新処理を終了します。
 */
// =======================================================================
void GsSoundEnd(void)
{
	if (gs_sound_tcb) {
		mtTaskClearTcb(gs_sound_tcb);
		gs_sound_tcb	= NULL;
	}
}

// =======================================================================
// GsSoundIsRunning
/*!
  サウンドメイン処理 実行中判定
  
  @retval TRUE	実行中
  @retval FALSE	実行中でない
 
  @note
  サウンドのフレーム更新処理が実行中か判定します。
 */
// =======================================================================
BOOL GsSoundIsRunning(void)
{
	if (gs_sound_tcb) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// GsSoundGetSysMainInfo
/*!
  サウンドシステムメイン情報取得
  
  @return サウンドシステムメイン情報
 */
// =======================================================================
GSS_SND_SYS_MAIN_INFO* GsSoundGetSysMainInfo(void)
{
	return &gs_sound_sys_main_info;
}

#if _IPHONE
// =======================================================================
// GsSoundPlaySe
/*!
  SE再生
  
  @param se_name	[in]	再生するSEのキュー名
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
 */
// =======================================================================
void GsSoundPlaySe(const char *se_name, GSS_SND_SE_HANDLE *se_handle/*=NULL*/,
				   Sint32 fade_frame/*=0*/)
{
#if GSD_SND_SE_WAIT_CTRL
	GSS_SND_SE_WAIT_CTRL* gsswc_if = &gs_sound_se_wait_ctrl;
	
	if (strncmp(se_name, "result", 6) != 0) {
		//	全SE再生しない期間
		if (gsswc_if->wait_name > GSD_SND_SE_WAIT_FRAME_ALL || gsswc_if->wait_id > GSD_SND_SE_WAIT_FRAME_ALL) {
			return;
		}
		//	同一SE再生しない期間
		if (gsswc_if->wait_name > 0) {
			if (strcmp(se_name, gsswc_if->set_name) == 0) {
				return;
			}
		}
	
		// 抑制設定
		strncpy(gsswc_if->set_name, se_name, GSD_SND_SE_WAIT_CUENAME_MAX_LENGTH);
		gsswc_if->wait_name = GSD_SND_SE_WAIT_FRAME_SAME;
	}
#endif // GSD_SND_SE_WAIT_CTRL
	
#if GSD_SND_SE_PLAY_LIMIT_CTRL
	GSS_SND_SE_PLAY_LIMIT_CTRL* gssplc_if = &gs_sound_se_play_limit_ctrl;
	
	if (gssplc_if->count >= GSD_SND_SE_PLAY_LIMIT_NUM) {
		// これ以上は再生を行わない(他曲再生防止)
		return;
	}
	
	gssplc_if->count++;
#endif // GSD_SND_SE_PLAY_LIMIT_CTRL
	
	gsSoundPlaySe(se_name, 0, se_handle, fade_frame);
}

// =======================================================================
// GsSoundPlaySeById
/*!
  SE再生(ID指定)
  
  @param se_id		[in]	再生するSEのキューID
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
 */
// =======================================================================
void GsSoundPlaySeById(Uint32 se_id, GSS_SND_SE_HANDLE *se_handle/*=NULL*/,
					   Sint32 fade_frame/*=0*/)
{
#if GSD_SND_SE_WAIT_CTRL
	GSS_SND_SE_WAIT_CTRL* gsswc_if = &gs_sound_se_wait_ctrl;
	
	//	全SE再生しない期間
	if (gsswc_if->wait_name > GSD_SND_SE_WAIT_FRAME_ALL || gsswc_if->wait_id > GSD_SND_SE_WAIT_FRAME_ALL) {
		return;
	}
	//	同一SE再生しない期間
	if (gsswc_if->wait_id > 0) {
		if (se_id == gsswc_if->set_id) {
			return;
		}
	}
	
	// 抑制設定
	gsswc_if->set_id = se_id;
	gsswc_if->wait_id = GSD_SND_SE_WAIT_FRAME_SAME;
#endif // GSD_SND_SE_WAIT_CTRL
	
#if GSD_SND_SE_PLAY_LIMIT_CTRL
	GSS_SND_SE_PLAY_LIMIT_CTRL* gssplc_if = &gs_sound_se_play_limit_ctrl;
	
	if (gssplc_if->count >= GSD_SND_SE_PLAY_LIMIT_NUM) {
		// これ以上は再生を行わない(他曲再生防止)
		return;
	}
	
	gssplc_if->count++;
#endif // GSD_SND_SE_PLAY_LIMIT_CTRL
	
	gsSoundPlaySe(NULL, se_id, se_handle, fade_frame);
}

// =======================================================================
// GsSoundPlaySeForce
/*!
  SE再生
  
  @param se_name	[in]	再生するSEのキュー名
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
 */
// =======================================================================
void GsSoundPlaySeForce(const char *se_name, GSS_SND_SE_HANDLE *se_handle/*=NULL*/,
				   Sint32 fade_frame/*=0*/)
{
	// 強制再生
	gsSoundPlaySe(se_name, 0, se_handle, fade_frame);
}

// =======================================================================
// GsSoundPlaySeByIdForce
/*!
  SE再生(ID指定)
  
  @param se_id		[in]	再生するSEのキューID
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
 */
// =======================================================================
void GsSoundPlaySeByIdForce(Uint32 se_id, GSS_SND_SE_HANDLE *se_handle/*=NULL*/,
					   Sint32 fade_frame/*=0*/)
{
	// 強制再生
	gsSoundPlaySe(NULL, se_id, se_handle, fade_frame);
}
#else
// =======================================================================
// GsSoundPlaySe
/*!
  SE再生
  
  @param se_name	[in]	再生するSEのキュー名
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
 */
// =======================================================================
void GsSoundPlaySe(const char *se_name, GSS_SND_SE_HANDLE *se_handle/*=NULL*/,
				   Sint32 fade_frame/*=0*/)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();

	if (NULL == se_handle) {
		// デフォルトハンドル取得
		se_handle	= gsSoundGetDefaultSeHandle();
		MTM_ASSERT(se_handle->au_player);
		// デフォルトハンドルに FLAG_NO_AUTOCLEAR の使用は禁止
		MTM_ASSERT(!(se_handle->flag & GSD_SND_SE_HANDLE_FLAG_NO_AUTOCLEAR));
	}
	
	// ポーズ中のプレーヤに対して新たにCueをセットする場合は
	// Stop()することで、既に関連付けられているボイスを解放しておく
	// （さもなくば、ボイスが確保されたまま残ってしまうため）
	if (se_handle->au_player->IsPaused(cri_audio_if->err)) {
		se_handle->au_player->Stop(CriAuPlayer::STOP_MODE_IMMEDIATE, cri_audio_if->err);
	}
	
	// ハンドル初期化
	if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_NO_AUTOCLEAR) {
		// 自動解放しない場合はリセットだけする
		
		// ハンドルは初期化済みであることが前提
		MTM_ASSERT(se_handle->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED);
		
		// ハンドルリセット（CUE解放しない）
		gsSoundInitSeHandle(se_handle, TRUE);
		
		// 一旦セットされたNO_AUTOCLEARフラグは、この関数を使用する限りオフにしない
		se_handle->flag	|= GSD_SND_SE_HANDLE_FLAG_NO_AUTOCLEAR;
	}
	else {
		gsSoundInitSeHandle(se_handle, FALSE);
	}
	
	// 再生開始済みフラグ設定
	se_handle->flag	|= GSD_SND_SE_HANDLE_FLAG_PLAY_STARTED;
	
	// 再生
	se_handle->au_player->SetCue(se_name, cri_audio_if->err);
	gsSoundCriSeSetFadeIn(se_handle, fade_frame);	// フェードイン設定
	gsSoundSeHandleUpdateVolume(se_handle);

	se_handle->au_player->Play(cri_audio_if->err);	// MEMO : Play()直後はSTATUS_PREPになる
	
}

// =======================================================================
// GsSoundPlaySeById
/*!
  SE再生(ID指定)
  
  @param se_id		[in]	再生するSEのキューID
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
 */
// =======================================================================
void GsSoundPlaySeById(Uint32 se_id, GSS_SND_SE_HANDLE *se_handle/*=NULL*/,
					   Sint32 fade_frame/*=0*/)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();

	if (NULL == se_handle) {
		// デフォルトハンドル取得
		se_handle	= gsSoundGetDefaultSeHandle();
		MTM_ASSERT(se_handle->au_player);
		// デフォルトハンドルに FLAG_NO_AUTOCLEAR の使用は禁止
		MTM_ASSERT(!(se_handle->flag & GSD_SND_SE_HANDLE_FLAG_NO_AUTOCLEAR));
	}
	
	// ポーズ中のプレーヤに対して新たにCueをセットする場合は
	// Stop()することで、既に関連付けられているボイスを解放しておく
	// （さもなくば、ボイスが確保されたまま残ってしまうため）
	if (se_handle->au_player->IsPaused(cri_audio_if->err)) {
		se_handle->au_player->Stop(CriAuPlayer::STOP_MODE_IMMEDIATE, cri_audio_if->err);
	}
	
	// ハンドル初期化
	if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_NO_AUTOCLEAR) {
		// 自動解放しない場合はリセットだけする
		
		// ハンドルは初期化済みであることが前提
		MTM_ASSERT(se_handle->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED);
		
		// ハンドルリセット（CUE解放しない）
		gsSoundInitSeHandle(se_handle, TRUE);
		
		// 一旦セットされたNO_AUTOCLEARフラグは、この関数を使用する限りオフにしない
		se_handle->flag	|= GSD_SND_SE_HANDLE_FLAG_NO_AUTOCLEAR;
	}
	else {
		gsSoundInitSeHandle(se_handle, FALSE);
	}
	
	// 再生開始済みフラグ設定
	se_handle->flag	|= GSD_SND_SE_HANDLE_FLAG_PLAY_STARTED;
	
	// 再生
	se_handle->au_player->SetCueById(se_id, cri_audio_if->err);
	gsSoundCriSeSetFadeIn(se_handle, fade_frame);	// フェードイン設定
	gsSoundSeHandleUpdateVolume(se_handle);
	se_handle->au_player->Play(cri_audio_if->err);	// MEMO : Play()直後はSTATUS_PREPになる
}
#endif // _IPHONE

// =======================================================================
// GsSoundStopSe
/*!
  SE停止
  
  @param fade_frame		[in]	フェードアウトに掛けるフレーム数（デフォルト:0）
  @param is_immediate	[in]	即時停止フラグ
  
  @note
  再生中の全てのSEを停止します。
  is_immediateをTRUEに指定すると、fade_frame==0だった場合、
  データにリリースタイムが設定されていてもそれを無視して即時に停止します。
 */
// =======================================================================
void GsSoundStopSe(Sint32 fade_frame/*=0*/, BOOL is_immediate/*=FALSE*/)
{
	for (Sint32 i = 0; i < GSD_SND_SE_HANDLE_MAX; ++i) {
		// 全てのアクティブなハンドルに関連付けられたCriAudioプレーヤをStop
		GSS_SND_SE_HANDLE	*se_handle	= &gs_sound_se_handle_heap[i];
		
		if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED) {
			gsSoundCriSeStop(se_handle, fade_frame, is_immediate);
		}
	}
}

// =======================================================================
// GsSoundPauseSe
/*!
  SE一時停止
  
  @param pause_level	[in]	ポーズレベル
  @param fade_frame		[in]	フェードアウトに掛けるフレーム数（デフォルト:0）
  
  @note
  再生中の全てのSEを一時停止します。
 */
// =======================================================================
void GsSoundPauseSe(GSE_SND_SE_HANDLE_PAUSE_LEVEL pause_level,
					Sint32 fade_frame/*=0*/)
{
	for (Sint32 i = 0; i < GSD_SND_SE_HANDLE_MAX; ++i) {
		// 全てのアクティブなハンドルに関連付けられたCriAudioプレーヤをPause
		GSS_SND_SE_HANDLE	*se_handle	= &gs_sound_se_handle_heap[i];
		
		if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED) {
			
			// ポーズレベル付与
			if (se_handle->cur_pause_level < pause_level) {
				se_handle->cur_pause_level	= pause_level;
			}
			
			gsSoundCriSePause(se_handle, fade_frame);
		}
	}
}

// =======================================================================
// GsSoundResumeSe
/*!
  SE再開
  
  @param pause_level	[in]	ポーズレベル
  @param fade_frame		[in]	フェードインに掛けるフレーム数（デフォルト:0）
  
  @note
  一時停止中の全てのSEを再開します。
 */
// =======================================================================
void GsSoundResumeSe(GSE_SND_SE_HANDLE_PAUSE_LEVEL pause_level,
					 Sint32 fade_frame/*=0*/)
{
	for (Sint32 i = 0; i < GSD_SND_SE_HANDLE_MAX; ++i) {
		// 全てのアクティブなハンドルに関連付けられたCriAudioプレーヤをResume
		GSS_SND_SE_HANDLE	*se_handle	= &gs_sound_se_handle_heap[i];
		
		if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED) {
			
			if (se_handle->cur_pause_level <= pause_level) {
				
				// 指定ポーズレベル値以下のハンドルのみ再開
				// （個別にポーズしているハンドルは最高レベルが付与されているので、
				//   最高レベル値が指定されない限りは再開されない）
				
				se_handle->cur_pause_level	= GSD_SND_SE_HANDLE_PAUSE_LEVEL_NONE;	// ポーズレベルクリア
				
				gsSoundCriSeResume(se_handle, fade_frame);
			}
		}
	}
	
}

// =======================================================================
// GsSoundStopSeHandle
/*!
  指定SEハンドルに関連付けられたSEを停止
  
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードアウトにかけるフレーム数（デフォルト:0）
 */
// =======================================================================
void GsSoundStopSeHandle(GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame/*=0*/)
{
	MTM_ASSERT(se_handle);
	
	gsSoundCriSeStop(se_handle, fade_frame, FALSE);
}

#if 0
/*
  REMINDER :
  複数のCUEが関連付けられているハンドルでは、最後にSetCueされたサウンド以外は
  ボリュームをコントロールすることができません。
  HBM移行時のサウンドミュート＆ポーズが行われた状態のときに上記のようなハンドルを
  GsSoundResumeSeHandle()で再開させると、最後にSetCueされたサウンド以外は
  ボリューム0が反映されずに鳴ってしまう恐れがあります。
  そのため、CriAuPlayerで再生されている全てのサウンドボリュームをコントロールする
  機能が実装されない限りは、下記の２つの関数を使用禁止とします。
 */
// =======================================================================
// GsSoundPauseSeHandle
/*!
  指定SEハンドルに関連付けられたSEを一時停止
  
  @param se_handle		[io]	SEハンドル
  @param fade_frame		[in]	フェードアウトにかけるフレーム数（デフォルト:0）
  
  @note
  GSD_SND_SE_HANDLE_PAUSE_LEVEL_EACH のポーズレベルでポーズをかけます。
  GsSoundResumeSeHandle()で再開します。
 */
// =======================================================================
void GsSoundPauseSeHandle(GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame/*=0*/)
{
	MTM_ASSERT(se_handle);
	
	if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED &&
		se_handle->au_player != NULL) {
		
		// ポーズレベル付与
		if (se_handle->cur_pause_level < GSD_SND_SE_HANDLE_PAUSE_LEVEL_EACH) {
			se_handle->cur_pause_level	= GSD_SND_SE_HANDLE_PAUSE_LEVEL_EACH;
		}
		
		gsSoundCriSePause(se_handle, fade_frame);
	}
}

// =======================================================================
// GsSoundResumeSeHandle
/*!
  指定SEハンドルに関連付けられたSEを再開
  
  @param se_handle		[io]	SEハンドル
  @param fade_frame		[in]	フェードアウトにかけるフレーム数（デフォルト:0）
  
  @note
  既定ポーズレベル値以下の値のレベルでポーズされている場合のみ再開されます。
 */
// =======================================================================
void GsSoundResumeSeHandle(GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame/*=0*/)
{
	MTM_ASSERT(se_handle);
	
	if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED &&
		se_handle->au_player != NULL &&
		se_handle->cur_pause_level <= GSD_SND_SE_HANDLE_PAUSE_LEVEL_EACH) {
		
		// 既定ポーズ値以下の値のポーズレベルでポーズされているハンドルのみ再開
		
		se_handle->cur_pause_level	= GSD_SND_SE_HANDLE_PAUSE_LEVEL_NONE;	// ポーズレベルクリア
		
		gsSoundCriSeResume(se_handle, fade_frame);
	}
}
#endif /* 0 */

// =======================================================================
// GsSoundPlayBgm
/*!
  BGM再生
  
  @param scb		[io]	SCB（NULL可）
  @param bgm_name	[in]	再生するBGMの名前
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
  
  @note
  scbにNULLを指定した場合、フェードインは行われません。
 */
// =======================================================================
void GsSoundPlayBgm(GSS_SND_SCB *scb, const char *bgm_name, Sint32 fade_frame/*=0*/)
{
	MTM_ASSERT(scb);

	// ポーズレベルクリア
	scb->cur_pause_level	= GSD_SND_SCB_PAUSE_LEVEL_NONE;
	
	if (scb->snd_data_type == GSE_SND_DATA_TYPE_NW4R) {
#if _WII
		MTM_ASSERT(scb->snd_handle);
		AkRvlSndHandleStop(scb->snd_handle, 0);
		AkRvlSndPlayArc(AKE_RVL_SND_TYPE_BGM, scb->snd_handle, bgm_name);
		AkRvlSndHandleSetFadeIn(scb->snd_handle, fade_frame);
#else
		MTM_ASSERT(FALSE);
#endif /* _WII */
	}
	else {
		MTM_ASSERT(scb->snd_data_type == GSE_SND_DATA_TYPE_CRIAUDIO);
		
		gsSoundCriStrmStop(scb, 0);
		
		// CRIプレイヤーのリセット（ポーズ状態などを残さないようにする）
		AMS_CRIAUDIO_INTERFACE	*cri_audio_if	= amCriAudioGetGlobal();
		CriAuPlayer	*au_player	= cri_audio_if->auply[scb->auply_no];
		if (au_player) {
			au_player->ReleaseCue(cri_audio_if->err);
			au_player->ResetParameters(cri_audio_if->err);
		}
		
		amCriAudioStrmPlay((Uint32)scb->auply_no, const_cast<char*>(bgm_name));
		gsSoundCriStrmSetFadeIn(scb, fade_frame);
#if _IPHONE
		// ボリュームを強制的に適応
		gsSoundUpdateVolume();
		au_player->Update(cri_audio_if->err);
#endif // _IPHONE
				
	}
}

// =======================================================================
// GsSoundStopBgm
/*!
  BGM停止
  
  @param scb		[io]	SCB（NULL不可）
  @param fade_frame	[in]	フェードアウトにかけるフレーム数（デフォルト:0）
 */
// =======================================================================
void GsSoundStopBgm(GSS_SND_SCB *scb, Sint32 fade_frame/*=0*/)
{
	MTM_ASSERT(scb);
	
	if (scb->snd_data_type == GSE_SND_DATA_TYPE_NW4R) {
#if _WII
		AkRvlSndHandleStop(scb->snd_handle, fade_frame);
#else
		MTM_ASSERT(FALSE);
#endif /* _WII */
	}
	else {
		MTM_ASSERT(scb->snd_data_type == GSE_SND_DATA_TYPE_CRIAUDIO);
		gsSoundCriStrmStop(scb, fade_frame);
	}
}

// =======================================================================
// GsSoundPauseBgm
/*!
  BGM一時停止
  
  @param scb		[io]	SCB（NULL不可）
  @param fade_frame	[in]	フェードアウトにかけるフレーム数（デフォルト:0）
  
  @note
  GSD_SND_SCB_PAUSE_LEVEL_EACH のポーズレベルでポーズをかけます。
 */
// =======================================================================
void GsSoundPauseBgm(GSS_SND_SCB *scb, Sint32 fade_frame/*=0*/)
{
	MTM_ASSERT(scb);
	
	if (scb->flag & GSD_SND_SCB_FLAG_INITIALIZED) {
		
		// ポーズレベル付与
		if (scb->cur_pause_level < GSD_SND_SCB_PAUSE_LEVEL_EACH) {
			scb->cur_pause_level	= GSD_SND_SCB_PAUSE_LEVEL_EACH;
		}
		
		if (scb->snd_data_type == GSE_SND_DATA_TYPE_NW4R) {
#if _WII
			AkRvlSndHandlePause(scb->snd_handle, fade_frame);
#else
			MTM_ASSERT(FALSE);
#endif /* _WII */
		}
		else {
			gsSoundCriStrmPause(scb, fade_frame);
		}
	}
}

// =======================================================================
// GsSoundResumeBgm
/*!
  BGM再開
  
  @param scb		[io]	SCB（NULL不可）
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
  
  @note
  既定ポーズレベル値以下のポーズレベルでポーズされている場合のみ再開されます。
 */
// =======================================================================
void GsSoundResumeBgm(GSS_SND_SCB *scb, Sint32 fade_frame/*=0*/)
{
	MTM_ASSERT(scb);
	
	if ((scb->flag & GSD_SND_SCB_FLAG_INITIALIZED) &&
		scb->cur_pause_level <= GSD_SND_SCB_PAUSE_LEVEL_EACH) {
		
		// 既定ポーズ値以下のレベルでポーズされているSCBのみ再開
		
		scb->cur_pause_level	= GSD_SND_SCB_PAUSE_LEVEL_NONE;	// ポーズレベルクリア
		
		if (scb->snd_data_type == GSE_SND_DATA_TYPE_NW4R) {
#if _WII
			AkRvlSndHandleResume(scb->snd_handle, fade_frame);
#else
			MTM_ASSERT(FALSE);
#endif /* _WII */
		}
		else {
			gsSoundCriStrmResume(scb, fade_frame);
			
		}
	}
}


// =======================================================================
// GsSoundSetVolumeFromMainSysInfo
/*!
  メインシステム情報からボリューム設定
  
  @note
  メインシステムで管理している情報(GSS_MAIN_SYS_INFO)に設定されている
  ボリューム値をgsSoundで管理しているボリュームに反映します。
 */
// =======================================================================
void GsSoundSetVolumeFromMainSysInfo(void)
{
	GSS_MAIN_SYS_INFO	*main_sys_info	= GsGetMainSysInfo();
	
	GsSoundSetVolume(GSE_SND_TYPE_BGM, main_sys_info->bgm_volume);
	GsSoundSetVolume(GSE_SND_TYPE_SE, main_sys_info->se_volume);
}

// =======================================================================
// GsSoundGetVolume
/*!
  ボリューム取得
  
  @param snd_type	[in]	サウンドタイプ
  
  @return ボリューム値
  
  @note
  サウンドタイプ毎のボリュームを取得します。
 */
// =======================================================================
Float GsSoundGetVolume(GSE_SND_TYPE snd_type)
{
	MTM_ASSERT(snd_type >= 0 && snd_type < GSE_SND_TYPE_MAX);
	
	return gs_sound_volume[snd_type];
}

// =======================================================================
// GsSoundSetVolume
/*!
  ボリューム設定
  
  @param snd_type	[in]	サウンドタイプ
  @param vol		[in]	ボリューム値
  
  @note
  サウンドタイプ毎のボリュームを設定します。
 */
// =======================================================================
void GsSoundSetVolume(GSE_SND_TYPE snd_type, Float vol)
{
	MTM_ASSERT(snd_type >= 0 && snd_type < GSE_SND_TYPE_MAX);
	//  ポータルメニュー側のサウンド設定が OFFのときは音を鳴らさない
	if (!Sonic4_isSoundFlag())	vol = 0.0f;
	gs_sound_volume[snd_type]	= vol;
}

// =======================================================================
// GsSoundScbSetVolume
/*!
  SCBボリューム設定
  
  @param vol	[in]	ボリューム値
  
  @note
  SCB単位でのボリュームを設定します。
  （※SCB単位のボリューム値の「取得」についてはakRvlSndで不可能なため、用意していません。）
 */
// =======================================================================
void GsSoundScbSetVolume(GSS_SND_SCB *scb, Float vol)
{
	MTM_ASSERT(scb);
	
	if (scb->snd_data_type == GSE_SND_DATA_TYPE_NW4R) {
#if _WII
		AkRvlSndHandleSetVolume(scb->snd_handle, vol);
#else
		MTM_ASSERT(FALSE);
#endif /* _WII */
	}
	else {
		scb->snd_ctrl_param.volume	= vol;
	}
}

// =======================================================================
// GsSoundScbSetSeqMute
/*!
  SCBのシーケンスミュート設定（ノートオン抑制）
  
  @param scb		[io]	SCB
  @param mute_on	[in]	ミュートフラグ
  
  @note
  シーケンスのノートオンを抑制することでミュートを行います。
  SCBのデータタイプが GSE_SND_DATA_TYPE_CRIAUDIOの場合は何もしません。
  ボリュームによるミュートだけではボイスを消費しますが、
  再生を継続しつつシーケンスを鳴らさない場合は、この関数を利用することで
  消費ボイスと負荷を軽減できます。
 */
// =======================================================================
void GsSoundScbSetSeqMute(GSS_SND_SCB *scb, BOOL mute_on)
{
	MTM_ASSERT(scb);
	
	// NW4Rのシーケンスサウンドのみ対応
	if (scb->snd_data_type != GSE_SND_DATA_TYPE_NW4R) {
		return;
	}
	
#if _WII
	if (mute_on) {
		AkRvlSndHandleSetSeqMute(scb->snd_handle, nw4r::snd::MUTE_STOP);
	}
	else {
		AkRvlSndHandleSetSeqMute(scb->snd_handle, nw4r::snd::MUTE_OFF);
	}
#else
	UNREFERENCED_PARAMETER(mute_on);
	MTM_ASSERT(FALSE);
#endif /* _WII */
}

// =======================================================================
// GsSoundAssignScb
/*!
  SCB割り当て
  
  @param snd_data_type	[in]	サウンドデータタイプ
  								GSE_SND_DATA_TYPE_CRIAUDIO → CRIAudioストリームサウンド
  								GSE_SND_DATA_TYPE_NW4R     → NW4Rシーケンスサウンド
  
  @return SCB
  
  @note
  GsSound管理下のSCBが割り当てられます。
  この関数で得られたSCBは毎フレーム更新処理が適用されます。
  使用後はGsSoundResignScb()で破棄してください。
 */
// =======================================================================
GSS_SND_SCB* GsSoundAssignScb(GSE_SND_DATA_TYPE snd_data_type)
{
	// REMINDER :
	// SCBのインデックスはamCriAudioで管理するオーディオプレーヤーのインデックスに
	// 利用されるが、AMD_CRIAUDIO_STREAM_TYPEが何かに関係なく割り当てていることに注意。
	// （各AME_CRIAUDIO_STRM_***を区別して処理が行われるようになった場合に問題となる。）
	
	// 空いているSCBを探す
	for (Sint32 i = 0; i < GSD_SND_SCB_MAX; ++i) {
		if (!(gs_sound_scb_heap_usage_flag[i>>3] & (1 << (i&0x07)))) {
			gs_sound_scb_heap_usage_flag[i>>3]	|= 1 << (i&0x07);
			
#if defined(MTD_DEBUG)
			OS_TPrintf("SCB remain : %d\n", gsSndGetFreeScbNum());
#endif /* defined(MTD_DEBUG) */
			
			MTM_ASSERT(!(gs_sound_scb_heap[i].flag & GSD_SND_SCB_FLAG_INITIALIZED));
			gsSoundInitSndScb(&gs_sound_scb_heap[i], i, snd_data_type);
			return &gs_sound_scb_heap[i];
		}
	}
	
	MTM_ASSERT(!"gsSound.cpp::GsSoundAssignScb() Error! Exceed scb limit\n");
	
	// 空いているSCBがなかった
	return NULL;
}

// =======================================================================
// GsSoundResignScb
/*!
  SCB破棄
  
  @param scb	[io]	SCB（NULL不可）
  
  @note
  GsSound管理下のSCBを破棄します。
  この関数を呼ぶことで、指定のSCBへのフレーム毎更新処理が適用されなくなります。
 */
// =======================================================================
void GsSoundResignScb(GSS_SND_SCB *scb)
{
	MTM_ASSERT(scb);
	
	// アドレスの一致する場所を探す
	for (Sint32 i = 0; i < GSD_SND_SCB_MAX; ++i) {
		if (&gs_sound_scb_heap[i] == scb) {
			// 二重解放チェック
			MTM_ASSERT(gs_sound_scb_heap_usage_flag[i>>3] & (1<<(i&0x07)));
			
			gsSoundClearSndScb(&gs_sound_scb_heap[i], gs_sound_scb_heap[i].auply_no);	// auply_noは設定されているはず
			gs_sound_scb_heap_usage_flag[i>>3]	&= ~(1 << (i&0x07));
#if defined(MTD_DEBUG)
			OS_TPrintf("SCB remain %d\n", gsSndGetFreeScbNum());
#endif /* defined(MTD_DEBUG) */
			return;
		}
	}
	
	// GsSoundAssignScb()で取得したSCBではなかった
	MTM_ASSERT(!"gsSound.cpp::GsSoundResignScb() Error! Invaid SCB specified\n");
}

// =======================================================================
// GsSoundAllocSeHandle
/*!
  SEハンドルを確保
  
  @return SEハンドル
  
  @note
  SEハンドルの確保に失敗した場合GSD_SND_ERROR_SE_HANDLEを返します。
  使用後はGsSoundFreeSeHandle()で破棄してください。
 */
// =======================================================================
GSS_SND_SE_HANDLE* GsSoundAllocSeHandle(void)
{
	// 空いているハンドルを探す
	for (Sint32 i = 0; i < GSD_SND_SE_HANDLE_MAX; ++i) {
		if (!(gs_sound_se_handle_heap_usage_flag[i>>3] & (1 << (i&0x07)))) {
			gs_sound_se_handle_heap_usage_flag[i>>3]	|= 1 << (i&0x07);
#if defined(MTD_DEBUG)
			//OS_TPrintf("Gs SE handle remain : %d\n", gsSoundGetFreeSeHandleNum());
#endif /* defined(MTD_DEBUG) */
			
			MTM_ASSERT(!(gs_sound_se_handle_heap[i].flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED));
			gsSoundInitSeHandle(&gs_sound_se_handle_heap[i]);
			return &gs_sound_se_handle_heap[i];
		}
	}
	
#if defined(GSD_SND_DEBUG_ASSERT_SE_HANDLE_OVER)
	MTM_ASSERT(!"gsSound.cpp::GsSoundAllocSeHandle() Error! Exceed SE handle limit\n");
#endif /* defined(MTD_DEBUG) */
	
	// 空いているSEハンドルがなかった
	gsSoundInitSeHandle(GSD_SND_ERROR_SE_HANDLE);
	return GSD_SND_ERROR_SE_HANDLE;
}

// =======================================================================
// GsSoundFreeSeHandle
/*!
  SEハンドルを解放
  
  @param se_handle	[io]	SEハンドル
 */
// =======================================================================
void GsSoundFreeSeHandle(GSS_SND_SE_HANDLE *se_handle)
{
	if (GSD_SND_ERROR_SE_HANDLE == se_handle) {
		gsSoundClearSeHandle(GSD_SND_ERROR_SE_HANDLE);
		return;
	}
	
	// アドレスの一致する場所を探す
	for (Sint32 i = 0; i < GSD_SND_SE_HANDLE_MAX; ++i) {
		if (&gs_sound_se_handle_heap[i] == se_handle) {
			// 二重解放チェック
			MTM_ASSERT(gs_sound_se_handle_heap_usage_flag[i>>3] & (1<<(i&0x07)));
			
			gsSoundClearSeHandle(se_handle);
			gs_sound_se_handle_heap_usage_flag[i>>3]	&= ~(1 << (i&0x07));
#if defined(MTD_DEBUG)
			//OS_TPrintf("GS SE handle remain : %d\n", gsSoundGetFreeSeHandleNum());
#endif /* defined(MTD_DEBUG) */
			return;
		}
	}
	
	// GsSoundAllocSeHandle()で取得したハンドルではなかった
	MTM_ASSERT(!"gsSound.cpp::GsSoundFreeSeHandle() Error! Invalid se handle specified\n");
}

// =======================================================================
// GsSoundRequestFreeSeHandle
/*!
  SEハンドルを解放リクエスト（再生停止時に解放）
  
  @param se_handle	[io]	SEハンドル
  
  @note
  SEハンドルに関連付けられているサウンドの再生が停止した時点でハンドルを解放します。
  再生開始していないハンドルに対しても有効です。
  解放リクエスト後はハンドルに対して操作は行わないでください。
  ループサウンドの場合、事前に停止操作を行なわないと
  いつまでも解放されなくなりますので注意してください。
  メイン処理が動いていないと解放されません。
  リクエスト後、再生が停止する前にメイン処理が終了した場合や、
  メイン処理終了後にリクエストした分については
  SEハンドルヒープリセット処理が行われるGsSoundReset()で解放する必要があります。
 */
// =======================================================================
void GsSoundRequestFreeSeHandle(GSS_SND_SE_HANDLE *se_handle)
{
	MTM_ASSERT(se_handle);
	
	se_handle->flag	|= GSD_SND_SE_HANDLE_FLAG_FREE_ON_STOP;
}

// =======================================================================
// GsSoundEnterHBM
/*!
  ホームボタンメニューへ移行時 サウンド操作開始処理
  
  @param arg	[io]	引数
  
  @note
  Wii以外では何もしません。コールバック関数ではありません。
 */
// =======================================================================
void GsSoundEnterHBM(void *arg)
{
#if _WII
	UNREFERENCED_PARAMETER(arg);
	
	// HBMへ移行時のフェード処理中
	gs_sound_sys_main_info.flag	|= GSD_SND_SYS_MAIN_FLAG_ENTER_HBM_FADING;
	
	// シーケンス設定
	MTM_ASSERT(gs_sound_sys_main_info.proc_hbm == NULL);
	gs_sound_sys_main_info.proc_hbm	= gsSoundWiiEnterHBMProcInit;
#else
	UNREFERENCED_PARAMETER(arg);
#endif /* _WII */
}

// =======================================================================
// GsSoundLeaveHBM
/*!
  ホームボタンメニューから復帰時 サウンド操作開始処理
  
  @param arg	[io]	引数
  
  @note
  Wii以外では何もしません。コールバック関数ではありません。
 */
// =======================================================================
void GsSoundLeaveHBM(void *arg)
{
#if _WII
	UNREFERENCED_PARAMETER(arg);
	
	// HBMから復帰時のフェード処理中
	gs_sound_sys_main_info.flag	|= GSD_SND_SYS_MAIN_FLAG_LEAVE_HBM_FADING;
	
	MTM_ASSERT(gs_sound_sys_main_info.proc_hbm == NULL);
	gs_sound_sys_main_info.proc_hbm	= gsSoundWiiLeaveHBMProcInit;
#else
	UNREFERENCED_PARAMETER(arg);
#endif /* _WII */
}

// =======================================================================
// GsSoundFadeHBM
/*!
  ホームボタンメニューフェード中 サウンド操作更新処理
  
  @param arg	[io]	引数
  
  @retval 1		フェード中
  @retval -1	フェード中ではない
  
  @note
  Wii以外では何もしません。コールバック関数ではありません。
 */
// =======================================================================
Sint32 GsSoundFadeHBM(void *arg)
{
#if _WII
	UNREFERENCED_PARAMETER(arg);
	
	// タスクがポーズ中の時、もしくはメイン処理タスクが起動していないときは
	// 自前でメイン処理を呼び出す
	if (amWiiIsPauseHBM() || !GsSoundIsRunning()) {
		gsSoundProcMain(NULL);
	}
	
	// フェード終了チェック
	if (gs_sound_sys_main_info.flag & GSD_SND_SYS_MAIN_FLAG_HBM_FADING_MASK) {
		// フェード中
		return 1;	// -1,0以外を返す（0だとHBMの初期化が始まってしまう）
	}
	else {
		// フェード中でない
		return -1;	// 
	}
#else
	UNREFERENCED_PARAMETER(arg);
	return -1;
#endif /* _WII */
}


/*------ Static Functions ----------------------------------------------*/

// =======================================================================
// gsSoundProcMain
/*!
  GSサウンドシステム フレーム処理関数
  
  @param tcb	[io]	TCB
 */
// =======================================================================
void gsSoundProcMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	
	/*
	  REMINDER :
	  tcb=NULLで直接呼び出す場合があるので、tcbにはアクセスしないこと！
	 */
	
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
#if _WII
	// HBM関連シーケンス更新（Wiiのみ）
	gsSoundWiiUpdateHBMSeq();
#endif /* _WII */
#if _IPHONE
	// サスペンド周辺処理更新 (_IPHONEのみ)
	gsSoundUpdateSystemSuspendWait();
#endif // _IPHONE
	
	// システム制御ボリューム処理更新
	gsSoundUpdateSystemControlVolume();
	
	// SCBリストを走査してアクティブなものを更新
	for (Sint32 i = 0; i < GSD_SND_SCB_MAX; ++i) {
		if (gs_sound_scb_heap[i].flag & GSD_SND_SCB_FLAG_INITIALIZED) {
			gsSoundUpdateSndScb(&gs_sound_scb_heap[i]);
		}
	}
	
	// 全てのアクティブなSEハンドルを更新（主にフェード更新）
	for (Sint32 i = 0; i < GSD_SND_SE_HANDLE_MAX; ++i) {
		// 全てのアクティブなハンドルに関連付けられたCriAudioプレーヤをUpdate
		GSS_SND_SE_HANDLE	*se_handle	= &gs_sound_se_handle_heap[i];
		
		if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED) {
			gsSoundUpdateSndSeHandle(se_handle);
		}
	}
	
	
	// ボリューム更新
	gsSoundUpdateVolume();
	
	
	// SE更新処理（パラメータ反映、終了判定など）
	for (Sint32 i = 0; i < GSD_SND_SE_HANDLE_MAX; ++i) {
		// 全てのアクティブなハンドルに関連付けられたCriAudioプレーヤをUpdate
		GSS_SND_SE_HANDLE	*se_handle	= &gs_sound_se_handle_heap[i];
		
		if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED) {
			
			// 更新
			// 再生終了したらハンドルをクリアする
			switch (se_handle->au_player->GetStatus(cri_audio_if->err)) {
			case CriAuPlayer::STATUS_ERROR:
				se_handle->au_player->Stop(cri_audio_if->err);
				// no break
			case CriAuPlayer::STATUS_STOP:	// no break
			case CriAuPlayer::STATUS_PLAYEND:
				if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_FREE_ON_STOP) {
					// 停止時解放フラグが立っている場合はハンドル解放（再生開始の有無は問わない）
					GsSoundFreeSeHandle(se_handle);
				}
				else {
					// 停止時通常処理
					if ((se_handle->flag & GSD_SND_SE_HANDLE_FLAG_PLAY_STARTED) &&
						!(se_handle->flag & GSD_SND_SE_HANDLE_FLAG_NO_AUTOCLEAR)) {
						// ↑ - 再生開始していないハンドルはクリアしない
						//    - 自動クリア禁止の場合はクリアしない
						gsSoundClearSeHandle(se_handle);
					}
				}
				break;
			default:
				// 更新（SetCueされていない場合はUpdateは呼ばない）
				se_handle->au_player->Update(cri_audio_if->err);
				break;
			}
		}
		else {
			if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_FREE_ON_STOP) {
				// 停止時解放フラグが立っている場合はハンドル解放
				// （既にクリアされている状態のハンドルも対象にするためこのタイミングでも行う）
				GsSoundFreeSeHandle(se_handle);
			}
		}
	}
	
	if (GSD_SND_ERROR_SE_HANDLE->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED) {
		switch (GSD_SND_ERROR_SE_HANDLE->au_player->GetStatus(cri_audio_if->err)) {
		case CriAuPlayer::STATUS_ERROR:
			GSD_SND_ERROR_SE_HANDLE->au_player->Stop(cri_audio_if->err);
			// no break
		case CriAuPlayer::STATUS_STOP:	// no break
		case CriAuPlayer::STATUS_PLAYEND:
			if (GSD_SND_ERROR_SE_HANDLE->flag & GSD_SND_SE_HANDLE_FLAG_FREE_ON_STOP) {
					// 停止時解放フラグが立っている場合はハンドル解放（再生開始の有無は問わない）
					GsSoundFreeSeHandle(GSD_SND_ERROR_SE_HANDLE);
			}
			else {
				// 停止時通常処理
				if (GSD_SND_ERROR_SE_HANDLE->flag & GSD_SND_SE_HANDLE_FLAG_PLAY_STARTED) {
					// ↑再生開始していないハンドルはクリアしない
					gsSoundClearSeHandle(GSD_SND_ERROR_SE_HANDLE);
				}
			}
			break;
		default:
			GSD_SND_ERROR_SE_HANDLE->au_player->Update(cri_audio_if->err);
			break;
		}
	}
	else {
		if (GSD_SND_ERROR_SE_HANDLE->flag & GSD_SND_SE_HANDLE_FLAG_FREE_ON_STOP) {
			// 停止時解放フラグが立っている場合はハンドル解放
			// （既にクリアされている状態のハンドルも対象にするためこのタイミングでも行う）
			GsSoundFreeSeHandle(GSD_SND_ERROR_SE_HANDLE);
		}
	}
	
#if _WII
	// AkRvlSoundフレーム更新処理
	AkRvlSndUpdate();
#else
#if 0	// ↓↓↓↓Alice側で更新しているので無効化↓↓↓↓
	// CRI BGM フレーム更新処理
	for (Sint32 i = 0; i < GSD_SND_SCB_MAX; ++i) {
		// 全てのアクティブなSCBに関連付けられたストリーム用CriAudioプレーヤをUpdate
		if (gs_sound_scb_heap[i].flag & GSD_SND_SCB_FLAG_INITIALIZED) {
			cri_audio_if->auply[gs_sound_scb_heap[i].auply_no]->Update(cri_audio_if->err);
		}
	}
#endif /* 0 */
#endif

#if GSD_SND_SE_WAIT_CTRL
	{//  SE連続再生抑制更新
		GSS_SND_SE_WAIT_CTRL* gsswc_if = &gs_sound_se_wait_ctrl;
		
		if (gsswc_if->wait_name > 0) {
		--gsswc_if->wait_name;
		}
		if (gsswc_if->wait_id > 0) {
			--gsswc_if->wait_id;
		}
	}
#endif // GSD_SND_SE_WAIT_CTRL
	
	
	// SCBリストを走査してアクティブなもののステータス更新
	for (Sint32 i = 0; i < GSD_SND_SCB_MAX; ++i) {
		if (gs_sound_scb_heap[i].flag & GSD_SND_SCB_FLAG_INITIALIZED) {
			gsSoundUpdateSndScbStatus(&gs_sound_scb_heap[i]);
		}
	}
	
	// 全てのアクティブなハンドルのステータス更新
	for (Sint32 i = 0; i < GSD_SND_SE_HANDLE_MAX; ++i) {
		if (gs_sound_se_handle_heap[i].flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED) {
			gsSoundUpdateSeHandleStatus(&gs_sound_se_handle_heap[i]);
		}
	}
	if (GSD_SND_ERROR_SE_HANDLE->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED) {
		gsSoundUpdateSeHandleStatus(GSD_SND_ERROR_SE_HANDLE);
	}
	
#if defined(MTD_DEBUG)
	if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_DEBUG_DISP) {
		// デバッグ情報
#if _WII
		amPrintf(GSD_SND_DEBUG_PRINT_POS_X, GSD_SND_DEBUG_PRINT_POS_Y - 2,
				 "GS number of voices NW4R : %d", nw4r::snd::SoundSystem::GetVoiceCount());
#endif /* _WII */
		amPrintf(GSD_SND_DEBUG_PRINT_POS_X, GSD_SND_DEBUG_PRINT_POS_Y - 1,
				 "GS number of voices : %d", cri_audio_if->auobj->GetNumberOfVoices());
		amPrintf(GSD_SND_DEBUG_PRINT_POS_X, GSD_SND_DEBUG_PRINT_POS_Y,
				 "GS SE handle remain : %d", gsSoundGetFreeSeHandleNum());
	}
#endif /* defined(MTD_DEBUG) */

#if GSD_SND_SE_PLAY_LIMIT_CTRL
	GSS_SND_SE_PLAY_LIMIT_CTRL* gssplc_if = &gs_sound_se_play_limit_ctrl;
	
	gssplc_if->count = cri_audio_if->auobj->GetNumberOfVoices(); // 現在の最大値を確保
#endif // GSD_SND_SE_PLAY_LIMIT_CTRL
}

// =======================================================================
// gsSoundInitSystemMainInfo
/*!
  サウンドシステムメイン情報 初期化
  
  @note
  メイン情報全体を初期化します。
  システム初期化後に一度だけ呼び出します。
 */
// =======================================================================
void gsSoundInitSystemMainInfo(void)
{
	// 一旦クリア
	gsSoundClearSystemMainInfo();
	
#if _WII
	// 現在のAUXバスリターンボリュームを退避して初期値としておく（Wiiのみ）
	gsSoundWiiSaveAuxReturnVol();
#endif /* _WII */
	
	// その他、必要ならば初期値の設定など
}

// =======================================================================
// gsSoundResetSystemMainInfo
/*!
  サウンドシステムメイン情報 リセット
  
  @note
  メイン情報を、必要な項目だけ初期化します。
 */
// =======================================================================
void gsSoundResetSystemMainInfo(void)
{
	// フラグリセット
	gs_sound_sys_main_info.flag	&= ~GSD_SND_SYS_MAIN_FLAG_RESET_CLEAR_MASK;
	
	// MEMO: 退避済AUXリターンボリュームがリセット処理をまたいで持ち越される状況を想定して
	//       ここでは退避処理は行わない。
	
	// MEMO: HOMEボタンメニュー中はリセット処理をまたいでもサウンドが鳴らないようにするため、
	//       GSD_SND_SYS_MAIN_FLAG_USE_SYSTEM_CNT_VOLフラグのクリアや、
	//       gs_sound_sys_main_info.system_cnt_volのクリアは行わない。
}

// =======================================================================
// gsSoundClearSystemMainInfo
/*!
  サウンドシステムメイン情報 クリア
  
  @note
  メイン情報全体をクリアします。
  システム終了時に呼び出します。
 */
// =======================================================================
void gsSoundClearSystemMainInfo(void)
{
	// ゼロクリア
	amZeroMemory(&gs_sound_sys_main_info, sizeof(GSS_SND_SYS_MAIN_INFO));
}

#if _WII
// =======================================================================
// gsSoundWiiSaveAuxReturnVol
/*!
  Wii AUXバスリターンボリューム値を退避
  
  @note
  Wii以外で呼び出した場合は何もしません。
 */
// =======================================================================
void gsSoundWiiSaveAuxReturnVol(void)
{
	gs_sound_sys_main_info.aux_a_ret_vol_save	= AXGetAuxAReturnVolume();
	gs_sound_sys_main_info.aux_b_ret_vol_save	= AXGetAuxBReturnVolume();
	gs_sound_sys_main_info.aux_c_ret_vol_save	= AXGetAuxCReturnVolume();
}

// =======================================================================
// gsSoundWiiRestoreAuxReturnVol
/*!
  Wii 退避されたAUXバスリターンボリューム値をAXに反映
  
  @note
  Wii以外で呼び出した場合は何もしません。
 */
// =======================================================================
void gsSoundWiiRestoreAuxReturnVol(void)
{
	AXSetAuxAReturnVolume(gs_sound_sys_main_info.aux_a_ret_vol_save);
	AXSetAuxBReturnVolume(gs_sound_sys_main_info.aux_b_ret_vol_save);
	AXSetAuxCReturnVolume(gs_sound_sys_main_info.aux_c_ret_vol_save);
}

// =======================================================================
// gsSoundWiiSaveMainOutVolume
/*!
  Wii メイン出力ボリュームを退避
 */
// =======================================================================
void gsSoundWiiSaveMainOutVolume(void)
{
	gs_sound_sys_main_info.hbm_mainout_vol_save	= nw4r::snd::SoundSystem::GetMainOutVolume();
}

// =======================================================================
// gsSoundWiiRestoreMainOutVolume
/*!
  Wii 退避されたメイン出力ボリュームを反映
 */
// =======================================================================
void gsSoundWiiRestoreMainOutVolume(void)
{
	nw4r::snd::SoundSystem::SetMainOutVolume(gs_sound_sys_main_info.hbm_mainout_vol_save, 0);
}

// =======================================================================
// gsSoundWiiGetSavedMainOutVolume
/*!
  Wii 退避されたメイン出力ボリュームを取得
  
  @return 退避されたメイン出力ボリューム値
 
  @note
  Wii以外では必ず1.fを返します。
 */
// =======================================================================
Float gsSoundWiiGetSavedMainOutVolume(void)
{
	return gs_sound_sys_main_info.hbm_mainout_vol_save;
}
#endif /* _WII */

// =======================================================================
// gsSoundSetSystemControlVolume
/*!
  システム制御ボリューム反映 有効・無効設定
  
  @param enable	[in]	有効フラグ（TRUE：有効化, FALSE：無効化）
  
  @note
  システム制御ボリューム反映の有効・無効設定を行います。
  システム制御ボリュームの反映が必要なくなった時点で、
  必ずgsSoundSetEnableSystemControlVolume(FALSE)を呼んでください。
 */
// =======================================================================
inline void gsSoundSetEnableSystemControlVolume(BOOL enable)
{
	if (enable) {
		gs_sound_sys_main_info.flag	|= GSD_SND_SYS_MAIN_FLAG_USE_SYSTEM_CNT_VOL;
	}
	else {
		gs_sound_sys_main_info.flag	&= ~GSD_SND_SYS_MAIN_FLAG_USE_SYSTEM_CNT_VOL;
	}
}

// =======================================================================
// gsSoundIsSystemControlVolumeEnabled
/*!
  システム制御ボリューム反映 有効判定
  
  @retval TRUE	ボリューム反映有効
  @retval FALSE	ボリューム反映無効
 */
// =======================================================================
inline BOOL gsSoundIsSystemControlVolumeEnabled(void)
{
	if (gs_sound_sys_main_info.flag & GSD_SND_SYS_MAIN_FLAG_USE_SYSTEM_CNT_VOL) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gsSoundUpdateSystemControlVolume
/*!
  システム制御ボリューム処理 更新
 */
// =======================================================================
void gsSoundUpdateSystemControlVolume(void)
{
	if (!gsSoundIsSystemControlVolumeEnabled()) {
		return;
	}
	
	// 現状は何もしない。将来的に必要になったらフェード処理の更新などを実装。
}

// =======================================================================
// gsSoundGetGlobalVolume
/*!
  全体反映ボリュームを取得
  
  @return 全体に反映するボリューム値を取得
  
  @note
  システム制御ボリュームなどを考慮した全体に反映するボリューム値を取得します。
 */
// =======================================================================
Float gsSoundGetGlobalVolume(void)
{
	Float	global_volume	= 1.f;
	
	// システム制御ボリューム取得
	if (gsSoundIsSystemControlVolumeEnabled()) {
		global_volume	*= gs_sound_sys_main_info.system_cnt_vol;
	}
	
	// 今後、必要になったらさらにボリューム値を掛ける
#if _IPHONE
	// サスペンド復帰待ちだったら音量強制ゼロ設定
	if (gsSoundIsSystemSuspendWait()) {
		global_volume = 0.0f;
	}
#endif // _IPHONE
	
	return global_volume;
}

// =======================================================================
// gsSoundGetSndScbMuteVolume
/*!
  SCBのミュートボリュームを取得する
  
  @param scb	[in]	SCB
  
  @return ミュートボリューム
  
  @note
  指定SCBに対してミュートが必要かどうかを判定し、
  ミュート成分のボリューム値を返します。
 */
// =======================================================================
Float gsSoundGetSndScbMuteVolume(const GSS_SND_SCB *scb)
{
	MTM_ASSERT(scb);
	
	/* 現状は単純にミュートの有無だけなので、返す値は1.0fもしくは0.f */
	
	if (GsSystemBgmIsPlay()) {
		if (scb->flag & GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM) {
			return 0.f;
		}
	}
	
	return 1.f;
}

// =======================================================================
// gsSoundInitSndScbHeap
/*!
  SCBヒープを初期化
 */
// =======================================================================
void gsSoundInitSndScbHeap(void)
{
	gsSoundResetSndScbHeap();
}

// =======================================================================
// gsSoundResetSndScbHeap
/*!
  SCBヒープをクリア
  
  @note
  全てのSCBを解放し、確保情報をクリアします。
 */
// =======================================================================
void gsSoundResetSndScbHeap(void)
{
	for (Sint32 i = 0; i < GSD_SND_SCB_MAX; ++i) {
		gsSoundClearSndScb(&gs_sound_scb_heap[i], gsSoundGetAuplyNo(i));
		
		// ↑念のため、SCBに関連付けられていないプレーヤも全てクリーンな状態にする
	}
	amZeroMemory(gs_sound_scb_heap_usage_flag, sizeof(gs_sound_scb_heap_usage_flag));
}

// =======================================================================
// gsSndGetFreeScbNum
/*!
  空きSCB数を取得する
  
  @return 空きSCB数
 */
// =======================================================================
Uint32 gsSndGetFreeScbNum(void)
{
	Uint32	num = GSD_SND_SCB_MAX;
	const static Sint32 elem_num	= (GSD_SND_SCB_MAX+7)/8;
	
	// 割り当て済みのSCB数分減算していく
	for (Sint32 i = 0; i < elem_num; ++i) {
		num	-= AkMathCountBitPopulation(gs_sound_scb_heap_usage_flag[i]);
		MTM_ASSERT(num <= GSD_SND_SCB_MAX);
	}
	
	return num;
}

// =======================================================================
// gsSoundInitSndScb
/*!
  SCBを初期化
  
  @param scb			[io]	SCB
  @param snd_data_type	[in]	サウンドデータタイプ
  
  @note
  初期化状態となるようにパラメータを設定します。
 */
// =======================================================================
void gsSoundInitSndScb(GSS_SND_SCB *scb, Sint32 scb_no, GSE_SND_DATA_TYPE snd_data_type)
{
	MTM_ASSERT(scb);
	MTM_ASSERT(scb_no >= 0 && scb_no < GSD_SND_SCB_MAX);
	
	// 一旦ニュートラルな状態にする
	gsSoundClearSndScb(scb, gsSoundGetAuplyNo(scb_no));	// これから関連付けられる予定のauply_noを指定
	
	// サウンドデータタイプ設定
	scb->snd_data_type	= snd_data_type;
	
	if (snd_data_type == GSE_SND_DATA_TYPE_NW4R) {
#if _WII
		// akRvlSndサウンドハンドル取得
		scb->snd_handle	= AkRvlSndAllocHandle();
#else
		MTM_ASSERT(FALSE);
#endif /* _WII */
	}
	else {
		MTM_ASSERT(snd_data_type == GSE_SND_DATA_TYPE_CRIAUDIO);
		
		// オーディオプレイヤーのインデックス取得
		// （SE再生ではauplyを使用しないのでBGMでの使用前提）
		scb->auply_no	= gsSoundGetAuplyNo(scb_no);
		MTM_ASSERT(amCriAudioGetGlobal()->auply[scb->auply_no]);
		
		// ボリューム初期化
		scb->snd_ctrl_param.fade_vol	= 1.f;
		scb->snd_ctrl_param.fade_sub_vol	= 1.f;
		scb->snd_ctrl_param.volume	= 1.f;
	}

#if _IPHONE
	scb->noplay_error_state.sample = Uint32(-1);
	scb->noplay_error_state.counter = 0;
#endif //_IPHONE

	// 初期化済み
	scb->flag	|= GSD_SND_SCB_FLAG_INITIALIZED;
}

// =======================================================================
// gsSoundClearSndScb
/*!
  SCBをクリア
  
  @param scb		[io]	SCB
  @param auply_no	[in]	SCBに関連付けられているor関連付けられる予定の
  							AMS_CRIAUDIO_INTERFACE::auply[]のインデックス
  @note
  パラメータをクリアします。
 */
// =======================================================================
void gsSoundClearSndScb(GSS_SND_SCB *scb, Sint32 auply_no)
{
	MTM_ASSERT(scb);
	
#if _WII
	// akRvlSndサウンドハンドル解放
	if (scb->snd_handle) {
		AkRvlSndFreeHandle(scb->snd_handle);
	}
#endif /* _WII */
	
	// CRIプレイヤーのリセット
	// （フラグやsnd_data_typeは信用できないのでNW4Rだったとしても行う）
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if	= amCriAudioGetGlobal();
	CriAuPlayer	*au_player	= cri_audio_if->auply[auply_no];
	
	if (au_player) {
		au_player->ReleaseCue(cri_audio_if->err);
		au_player->ResetParameters(cri_audio_if->err);
	}
	
	amZeroMemory(scb, sizeof(GSS_SND_SCB));
}

// =======================================================================
// gsSoundUpdateSndScb
/*!
  SCB更新
  
  @param scb	[io]	SCB
  
  @note
  SCBに対するフレーム更新処理を行います。
 */
// =======================================================================
void gsSoundUpdateSndScb(GSS_SND_SCB *scb)
{
	MTM_ASSERT(scb);
	
	if (scb->snd_data_type == GSE_SND_DATA_TYPE_NW4R) {
		// NW4Rの場合は更新はAkRvlSndUpdate()に任せるので、これ以降何もしない
		return;
	}
	
	/* 以下、CRI Audio ストリームサウンド専用処理 */
	
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if	= amCriAudioGetGlobal();
	CriAuPlayer	*au_player	= cri_audio_if->auply[scb->auply_no];
	
	MTM_ASSERT(au_player);
	
	// サウンドコントロール更新
	gsSoundUpdateSndCtrl(&scb->snd_ctrl_param, au_player);

#if _IPHONE
	//異常停止状態確認
	if (gsSoundCheckSndScbStop(scb) || ((GSD_SND_SCB_FLAG_IS_STOP | GSD_SND_SCB_FLAG_IS_PAUSE) & scb->flag)) {
		//意図した停止中なら
		scb->noplay_error_state.sample = Uint32(-1);
		scb->noplay_error_state.counter = 0;
	} else {
		//再生中なら
		Uint32 sample = au_player->GetNumPlayedSamples(cri_audio_if->err);
		if (scb->noplay_error_state.sample != sample) {
			//再生中
			scb->noplay_error_state.sample = sample;
			scb->noplay_error_state.counter = 0;
		} else if (GSD_SND_NOPLAY_ERROR_STATE_RESTART_LIMIT < scb->noplay_error_state.counter++) {
			//異常停止なら
			CriSmpSoundOutput_StopSound();
			CriSmpSoundOutput_ReStartSound();

			scb->noplay_error_state.sample = Uint32(-1);
			scb->noplay_error_state.counter = 0;
		}
	}
#endif //_IPHONE
}

// =======================================================================
// gsSoundUpdateSndScbStatus
/*!
  SCBステータス更新
  
  @param scb	[io]	SCB（NULL不可）
  
  @note
  停止中・一時停止中などの状態をチェック・更新します。
 */
// =======================================================================
void gsSoundUpdateSndScbStatus(GSS_SND_SCB *scb)
{
	MTM_ASSERT(scb);
	
	// 再生終了チェック
	if (gsSoundCheckSndScbStop(scb)) {
		scb->flag	|= GSD_SND_SCB_FLAG_IS_STOP;
	}
	else {
		scb->flag	&= ~GSD_SND_SCB_FLAG_IS_STOP;
	}
	
	// 一時停止中チェック
	if (gsSoundCheckSndScbPause(scb)) {
		scb->flag	|= GSD_SND_SCB_FLAG_IS_PAUSE;
	}
	else {
		scb->flag	&= ~GSD_SND_SCB_FLAG_IS_PAUSE;
	}
}

// =======================================================================
// gsSoundCheckSndScbPause
/*!
  SCB一時停止中判定
  
  @param scb	[in]	SCB（NULL不可）
  
  @note
  サウンドデータタイプに応じた一時停止中判定を行います。
 */
// =======================================================================
BOOL gsSoundCheckSndScbPause(const GSS_SND_SCB *scb)
{
	MTM_ASSERT(scb);
	
	if (scb->snd_data_type == GSE_SND_DATA_TYPE_NW4R) {
#if _WII
		return AkRvlSndHandleIsPause(scb->snd_handle);
#else
		MTM_ASSERT(FALSE);
		return FALSE;
#endif /* _WII */
	}
	else {
		AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
		
		if (scb->snd_ctrl_param.fade_state == GSE_SND_FADE_STATE_FADING_OUT_TO_PAUSE ||
			cri_audio_if->auply[scb->auply_no]->IsPaused(cri_audio_if->err)) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
}

// =======================================================================
// gsSoundCheckSndScbStop
/*!
  SCB再生停止中判定
  
  @param scb	[in]	SCB（NULL不可）
  
  @note
  サウンドデータタイプに応じた終了判定を行います。
 */
// =======================================================================
BOOL gsSoundCheckSndScbStop(const GSS_SND_SCB *scb)
{
	MTM_ASSERT(scb);
	
	if (scb->snd_data_type == GSE_SND_DATA_TYPE_NW4R) {
#if _WII
		MTM_ASSERT(scb->snd_handle);
		if (scb->snd_handle->IsAttachedSound()) {
			return FALSE;
		}
		else {
			return TRUE;
		}
#else
		MTM_ASSERT(FALSE);
		return TRUE;
#endif /* _WII */
	}
	else {
		AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
		
		switch (cri_audio_if->auply[scb->auply_no]->GetStatus(cri_audio_if->err)) {
		case CriAuPlayer::STATUS_ERROR:		// no break
		case CriAuPlayer::STATUS_STOP:		// no break
		case CriAuPlayer::STATUS_PLAYEND:
			return TRUE;
			
		default:
			return FALSE;
		}
	}
}

// =======================================================================
// gsSoundGetAuplyNo
/*!
  SCBヒープ上のインデックスに対応したCRIAudioPlayerのインデックスを取得
  
  @param scb_no	[in]	SCBヒープ上のインデックス
 
  @return AMS_CRIAUDIO_INTERFACE::auply[]のインデックス
 */
// =======================================================================
Sint32 gsSoundGetAuplyNo(Sint32 scb_no)
{
	MTM_ASSERT(scb_no >= 0 && scb_no < GSD_SND_SCB_MAX);
	
	// 現状、両者のインデックスは1対1で対応
	return scb_no;
}

#if _WII	// Warning対策（Wii版でしか参照されないので）
// =======================================================================
// gsSoundPauseAllScb
/*!
  全SCB一時停止（システム用）
  
  @param pause_level	[in]	ポーズレベル
  
  @note
  SCBのポーズなどはゲーム側・デモ側で管理する方針なので、この関数は公開しない。
  HBM遷移時などのシステムによる制御用に使用。
 */
// =======================================================================
void gsSoundPauseAllScb(GSE_SND_SCB_PAUSE_LEVEL pause_level)
{
	for (Sint32 i = 0; i < GSD_SND_SCB_MAX; ++i) {
		GSS_SND_SCB *scb	= &gs_sound_scb_heap[i];
		
		if (scb->flag & GSD_SND_SCB_FLAG_INITIALIZED) {
			
			// ポーズレベル付与
			if (scb->cur_pause_level < pause_level) {
				scb->cur_pause_level	= pause_level;
			}
			
			if (scb->snd_data_type == GSE_SND_DATA_TYPE_NW4R) {
#if _WII
				AkRvlSndHandlePause(scb->snd_handle, 0);
#else
				MTM_ASSERT(FALSE);
#endif /* _WII */
			}
			else {
				gsSoundCriStrmPause(scb, 0);
			}
		}
	}
}

// =======================================================================
// gsSoundResumeAllScb
/*!
  全SCB再開（システム用）
  
  @param pause_level	[in]	ポーズレベル
  
  @note
  指定ポーズレベル値以下のレベルでポーズされているSCBを再開します。
  SCBのポーズなどはゲーム側・デモ側で管理する方針なので、この関数は公開しない。
  HBM遷移時などのシステムによる制御用に使用。
 */
// =======================================================================
void gsSoundResumeAllScb(GSE_SND_SCB_PAUSE_LEVEL pause_level)
{
	for (Sint32 i = 0; i < GSD_SND_SCB_MAX; ++i) {
		GSS_SND_SCB *scb	= &gs_sound_scb_heap[i];
		
		if ((scb->flag & GSD_SND_SCB_FLAG_INITIALIZED) &&
			scb->cur_pause_level <= pause_level) {
			
			// 既定ポーズレベル値以下のレベルでポーズされているSCBのみ再開
			
			scb->cur_pause_level	= GSD_SND_SCB_PAUSE_LEVEL_NONE;	// ポーズレベルクリア
			
			if (scb->snd_data_type == GSE_SND_DATA_TYPE_NW4R) {
#if _WII
				AkRvlSndHandleResume(scb->snd_handle, 0);
#else
				MTM_ASSERT(FALSE);
#endif /* _WII */
			}
			else {
				gsSoundCriStrmResume(scb, 0);
			}
		}
	}
}
#endif /* _WII (Warning対策) */

void mpp_gsOnOffBGM(int on_off) {///qqq
	Sint32 i = 0; //BGM
	{
		GSS_SND_SCB *scb	= &gs_sound_scb_heap[i];
		
		if ((scb->flag & GSD_SND_SCB_FLAG_INITIALIZED)) {
			if(on_off) {
				gsSoundCriStrmResume(scb, 0);
			}
			else {
				gsSoundCriStrmPause(scb, 0);
			}
		}
		
	}
	
}

// =======================================================================
// gsSoundCriStrmSetFadeIn
/*!
  CRI用フェードイン設定
  
  @param scb		[io]	SCB
  @param fade_frame	[in]	フェードインにかけるフレーム数
  
  @note
  CRI用にフェードインパラメータを設定します。
  fade_frameに0を指定した場合は通常の再生開始のパラメータ設定になります。
 */
// =======================================================================
void gsSoundCriStrmSetFadeIn(GSS_SND_SCB *scb, Sint32 fade_frame)
{
	MTM_ASSERT(scb);
	MTM_ASSERT(fade_frame >= 0);
	
	if (scb->snd_data_type != GSE_SND_DATA_TYPE_CRIAUDIO) {
		MTM_ASSERT(FALSE);
		return;
	}
	
	if (fade_frame == 0) {
		scb->snd_ctrl_param.fade_state	= GSE_SND_FADE_STATE_NORMAL;
		scb->snd_ctrl_param.fade_frame_max	= 0;
		scb->snd_ctrl_param.fade_frame_cnt	= 0;
		scb->snd_ctrl_param.fade_vol		= 1.f;
	}
	else {
		scb->snd_ctrl_param.fade_state	= GSE_SND_FADE_STATE_FADING_IN;
		scb->snd_ctrl_param.fade_frame_max	= fade_frame;
		scb->snd_ctrl_param.fade_frame_cnt	= 0;
		scb->snd_ctrl_param.fade_vol		= 0.f;	//! 最初は無音
	}
	
	scb->snd_ctrl_param.fade_sub_vol	= 1.f;
}

// =======================================================================
// gsSoundCriStrmStop
/*!
  CRIストリームサウンド停止
  
  @param scb			[io]	SCB
  @param fade_frame		[in]	フェードアウトにかけるフレーム数
  @param is_takeover	[in]	フェードボリューム引き継ぎフラグ
  								（デフォルト：FALSE）
  
  @note
  CRIのストリームサウンドをフェードアウトしながら停止させます。
  fade_frameに0を指定すると直ちに停止します。
 */
// =======================================================================
void gsSoundCriStrmStop(GSS_SND_SCB *scb, Sint32 fade_frame, BOOL is_takeover/*=FALSE*/)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
	MTM_ASSERT(scb);
	MTM_ASSERT(fade_frame >= 0);
	
	if (scb->snd_data_type != GSE_SND_DATA_TYPE_CRIAUDIO) {
		MTM_ASSERT(FALSE);
		return;
	}
	
	if (is_takeover) {
		scb->snd_ctrl_param.fade_sub_vol
			= scb->snd_ctrl_param.fade_sub_vol * scb->snd_ctrl_param.fade_vol;
	}
	else {
		scb->snd_ctrl_param.fade_sub_vol	= 1.f;
	}
	
	if (fade_frame == 0) {
		scb->snd_ctrl_param.fade_state	= GSE_SND_FADE_STATE_NORMAL;
		scb->snd_ctrl_param.fade_frame_max	=
			scb->snd_ctrl_param.fade_frame_cnt	= 0;
		scb->snd_ctrl_param.fade_vol	= 0.f;
		cri_audio_if->auply[scb->auply_no]->Stop(cri_audio_if->err);
	}
	else {
		scb->snd_ctrl_param.fade_state	= GSE_SND_FADE_STATE_FADING_OUT_TO_STOP;
		scb->snd_ctrl_param.fade_frame_max	= fade_frame;
		scb->snd_ctrl_param.fade_frame_cnt	= 0;
		scb->snd_ctrl_param.fade_vol		= 1.f;	//! 現在の音量から減衰させる
	}
}

// =======================================================================
// gsSoundCriStrmPause
/*!
  CRIストリームサウンド一時停止
  
  @param scb		[io]	SCB
  @param fade_frame	[in]	フェードアウトにかけるフレーム数
  
  @note
  CRIのストリームサウンドをフェードアウトしながら一時停止させます。
  fade_frameに0を指定すると直ちに一時停止します。
 */
// =======================================================================
void gsSoundCriStrmPause(GSS_SND_SCB *scb, Sint32 fade_frame)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
	MTM_ASSERT(scb);
	MTM_ASSERT(fade_frame >= 0);
	
	if (scb->snd_data_type != GSE_SND_DATA_TYPE_CRIAUDIO) {
		MTM_ASSERT(FALSE);
		return;
	}
	
	// 停止へのフェードアウト中にポーズを実行した場合は指定フレームでそのまま停止させる
	// TODO : 次回実装では廃止予定
	if (scb->snd_ctrl_param.fade_state == GSE_SND_FADE_STATE_FADING_OUT_TO_STOP) {
		gsSoundCriStrmStop(scb, fade_frame, TRUE);
		return;
	}
	
	if (fade_frame == 0) {
		scb->snd_ctrl_param.fade_state	= GSE_SND_FADE_STATE_NORMAL;
		scb->snd_ctrl_param.fade_frame_max	=
			scb->snd_ctrl_param.fade_frame_cnt	= 0;
		scb->snd_ctrl_param.fade_vol	= 0.f;
		cri_audio_if->auply[scb->auply_no]->Pause(TRUE, cri_audio_if->err);
	}
	else {
		scb->snd_ctrl_param.fade_state	= GSE_SND_FADE_STATE_FADING_OUT_TO_PAUSE;
		scb->snd_ctrl_param.fade_frame_max	= fade_frame;
		scb->snd_ctrl_param.fade_frame_cnt	= 0;
		scb->snd_ctrl_param.fade_vol	= 1.f;	//! 現在の音量から減衰させる
	}
}

// =======================================================================
// gsSoundCriStrmResume
/*!
  CRIストリームサウンド再開
  
  @param scb		[io]	SCB
  @param fade_frame	[in]	フェードインにかけるフレーム数
  
  @note
  CRIのストリームサウンドをフェードインしながら一時停止から再開させます。
  fade_frameに0を指定すると直ちに再開します。
 */
// =======================================================================
void gsSoundCriStrmResume(GSS_SND_SCB *scb, Sint32 fade_frame)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	MTM_ASSERT(scb);
	
	if (scb->snd_data_type != GSE_SND_DATA_TYPE_CRIAUDIO) {
		MTM_ASSERT(FALSE);
		return;
	}
	
	if (!gsSoundCheckSndScbPause(scb)) {
		// ポーズ中以外の場合は無視
		return;
	}
	
	// ポーズ解除
	cri_audio_if->auply[scb->auply_no]->Pause(FALSE, cri_audio_if->err);
	
	if (fade_frame == 0) {
		scb->snd_ctrl_param.fade_state	= GSE_SND_FADE_STATE_NORMAL;
		scb->snd_ctrl_param.fade_frame_max	=
			scb->snd_ctrl_param.fade_frame_cnt	= 0;
		scb->snd_ctrl_param.fade_vol	= 1.f;
	}
	else {
		// フェードイン設定
		gsSoundCriStrmSetFadeIn(scb, fade_frame);
	}
}


// =======================================================================
// gsSoundUpdateSndCtrl
/*!
  サウンドコントロール 更新
  
  @param snd_ctrl_param	[io]	サウンドコントロールパラメータ
  @param au_player		[io]	CRI Audio プレーヤ
  
  @note
  CRI Audioプレーヤに対するパラメータのフレーム更新処理を行います。
  この更新ではCriAuPlayer::Update()によるパラメータの反映（ボリュームなど）は行われません。
 */
// =======================================================================
void gsSoundUpdateSndCtrl(GSS_SND_CTRL_PARAM *snd_ctrl_param, CriAuPlayer *au_player)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if	= amCriAudioGetGlobal();
	
	// フェード更新
	if (snd_ctrl_param->fade_state == GSE_SND_FADE_STATE_FADING_IN ||
		snd_ctrl_param->fade_state == GSE_SND_FADE_STATE_FADING_OUT_TO_STOP ||
		snd_ctrl_param->fade_state == GSE_SND_FADE_STATE_FADING_OUT_TO_PAUSE) {
		
		// 指定フレーム経過チェック
		if (snd_ctrl_param->fade_frame_max <= snd_ctrl_param->fade_frame_cnt) {
			
			if (snd_ctrl_param->fade_state == GSE_SND_FADE_STATE_FADING_IN) {
				// 音量を1.0fにfix
				snd_ctrl_param->fade_vol	= 1.0f;
			}
			else {
				if (snd_ctrl_param->fade_state == GSE_SND_FADE_STATE_FADING_OUT_TO_PAUSE) {
					// ポーズ
					au_player->Pause(TRUE, cri_audio_if->err);
				}
				else {
					MTM_ASSERT(snd_ctrl_param->fade_state == GSE_SND_FADE_STATE_FADING_OUT_TO_STOP);
					
					// 停止
					// （自前でフェードした末の停止なので、データに設定されているリリースタイムは無視したいので、
					//   強制的に「即時停止モード」で停止させる。
					//   ここで停止しておかないとリリースタイム中はfade_state==GSE_SND_FADE_STATE_NORMALとなり、
					//   完全に停止するまでの間fade_vol=1.fが反映されてしまうので。）
					au_player->Stop(CriAuPlayer::STOP_MODE_IMMEDIATE, cri_audio_if->err);
				}
				
				// 音量を0にfix
				snd_ctrl_param->fade_vol	= 0.f;
			}
			
			// ステート,パラメータ設定
			snd_ctrl_param->fade_state	= GSE_SND_FADE_STATE_NORMAL;
			snd_ctrl_param->fade_frame_max	=
				snd_ctrl_param->fade_frame_cnt	= 0;
		}
		else {
			// 更新中
			
			switch (au_player->GetStatus(cri_audio_if->err)) {
				Float	rate;
			case CriAuPlayer::STATUS_PLAYING:	// no break
			case CriAuPlayer::STATUS_PLAYEND:
				// 再生開始後は（停止後も）ポーズ中でない限りフェード処理し続ける
				if (!au_player->IsPaused(cri_audio_if->err)) {
					
					if (snd_ctrl_param->fade_frame_max == 0) {	// ゼロ除算回避
						rate	= 1.0f;
					}
					else {
						rate	= 1.0f * ((Float)snd_ctrl_param->fade_frame_cnt / (Float)snd_ctrl_param->fade_frame_max);
					}
					
					// フェードタイプに応じてボリューム設定
					if (snd_ctrl_param->fade_state == GSE_SND_FADE_STATE_FADING_IN) {
						snd_ctrl_param->fade_vol	= rate;	// 徐々に大きく
					}
					else {
						MTM_ASSERT(snd_ctrl_param->fade_state == GSE_SND_FADE_STATE_FADING_OUT_TO_STOP ||
								   snd_ctrl_param->fade_state == GSE_SND_FADE_STATE_FADING_OUT_TO_PAUSE);
						snd_ctrl_param->fade_vol	= 1.f - rate;	// 徐々に小さく
					}
					
					// フレーム更新
					snd_ctrl_param->fade_frame_cnt++;
				}
				break;
				
			default:
				// 再生がまだ開始していない時やエラー時は音を出さない
				snd_ctrl_param->fade_vol	= 0.f;
				break;
			}
		}
	}
	else {
		if (au_player->IsPaused(cri_audio_if->err)) {
			// ポーズ中はボリュームを絞っておく（Resume時に一瞬音が大きくなるのを防ぐため）
			snd_ctrl_param->fade_vol	= 0.f;
		}
		else {
			// 非フェード中かつ再生中はフェードボリュームによる影響を与えない
			snd_ctrl_param->fade_vol	= 1.f;
		}
	}
}


// =======================================================================
// gsSoundSeHandleUpdateVolume
/*!
  SEハンドルに関連付けられているサウンドへのボリューム反映
  
  @param se_handle	[io]	SEハンドル
 */
// =======================================================================
void gsSoundSeHandleUpdateVolume(GSS_SND_SE_HANDLE *se_handle)
{
	MTM_ASSERT(se_handle);
	
	if (se_handle->au_player) {
		// ボリューム設定箇所が1つしかないので、フェードボリューム・SEハンドル単位ボリュームも掛け合わせる
		se_handle->au_player->SetVolume(gs_sound_volume[GSE_SND_TYPE_SE] *
										se_handle->snd_ctrl_param.volume *
										se_handle->snd_ctrl_param.fade_vol *
										se_handle->snd_ctrl_param.fade_sub_vol *
										gsSoundGetGlobalVolume());
	}
}

// =======================================================================
// gsSoundUpdateVolume
/*!
  ボリューム設定反映
  
  @note
  gsSoundで管理しているボリューム値をCriAudio/NW4Rに反映する。
 */
// =======================================================================
void gsSoundUpdateVolume(void)
{
	// BGMボリューム反映
#if _WII
	// フェードボリュームやハンドル単位のボリュームの合成はNW4Rでやるので、ここではBGMボリュームと全体に反映するボリュームだけ設定
	AkRvlSndSetVolume(AKE_RVL_SND_TYPE_BGM, gs_sound_volume[GSE_SND_TYPE_BGM] * gsSoundGetGlobalVolume());
#endif /* _WII */
	
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
	for (Sint32 i = 0; i < GSD_SND_SCB_MAX; ++i) {
		if (gs_sound_scb_heap[i].flag & GSD_SND_SCB_FLAG_INITIALIZED &&
			gs_sound_scb_heap[i].snd_data_type == GSE_SND_DATA_TYPE_CRIAUDIO) {
			
			CriAuPlayer::Status	status	= cri_audio_if->auply[gs_sound_scb_heap[i].auply_no]->GetStatus(cri_audio_if->err);
			
			if (CriAuPlayer::STATUS_STOP != status &&
				CriAuPlayer::STATUS_PLAYEND != status) {
				// ボリューム設定箇所が1つしかないので、フェードボリューム・SCB単位ボリュームも掛け合わせる
				cri_audio_if->auply[gs_sound_scb_heap[i].auply_no]->SetVolume(gs_sound_volume[GSE_SND_TYPE_BGM] *
																			  gs_sound_scb_heap[i].snd_ctrl_param.volume *
																			  gs_sound_scb_heap[i].snd_ctrl_param.fade_vol *
																			  gs_sound_scb_heap[i].snd_ctrl_param.fade_sub_vol *
																			  gsSoundGetGlobalVolume() *
																			  gsSoundGetSndScbMuteVolume(&gs_sound_scb_heap[i]));
			}
		}
	}
	
	// SEボリューム反映
	for (Sint32 i = 0; i < GSD_SND_SE_HANDLE_MAX; ++i) {
		if (gs_sound_se_handle_heap[i].flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED) {
			if (gsSoundIsSeHandleCueSet(&gs_sound_se_handle_heap[i])) {
				gsSoundSeHandleUpdateVolume(&gs_sound_se_handle_heap[i]);
			}
		}
	}
	if (GSD_SND_ERROR_SE_HANDLE->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED) {
		if (gsSoundIsSeHandleCueSet(GSD_SND_ERROR_SE_HANDLE)) {
			gsSoundSeHandleUpdateVolume(GSD_SND_ERROR_SE_HANDLE);
		}
	}
}

// =======================================================================
// gsSoundInitSeHandleHeap
/*!
  SEハンドルヒープを初期化
 */
// =======================================================================
void gsSoundInitSeHandleHeap(void)
{
	gsSoundResetSeHandleHeap();
}

// =======================================================================
// gsSoundResetSeHandleHeap
/*!
  SEハンドルヒープをリセット
  
  @note
  全てのSEハンドルを解放、確保情報をクリアし、初期化後の状態にします。
 */
// =======================================================================
void gsSoundResetSeHandleHeap(void)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
	gsSoundClearSeHandleHeap();
	
	for (Sint32 i = 0; i < GSD_SND_SE_HANDLE_MAX; ++i) {
#if _WII
		gs_sound_se_handle_heap[i].au_player	=
			(CriAuPlayer*)CriAuPlayerWii::Create(cri_audio_if->auobj, cri_audio_if->err);
#else
		gs_sound_se_handle_heap[i].au_player	=
			CriAuPlayer::Create(cri_audio_if->auobj, cri_audio_if->err);
#endif /* _WII */
	}
#if _WII
	GSD_SND_ERROR_SE_HANDLE->au_player	=
		(CriAuPlayer*)CriAuPlayerWii::Create(cri_audio_if->auobj, cri_audio_if->err);
#else
	GSD_SND_ERROR_SE_HANDLE->au_player	=
		CriAuPlayer::Create(cri_audio_if->auobj, cri_audio_if->err);
#endif	/* _WII */
	
	
	// デフォルトハンドル確保
	gs_sound_se_handle_default	= GsSoundAllocSeHandle();
}

// =======================================================================
// gsSoundClearSeHandleHeap
/*!
  SEハンドルヒープをクリア
  
  @note
  SEハンドルヒープを使用しなくなった時点で必ず呼び出してください。
 */
// =======================================================================
void gsSoundClearSeHandleHeap(void)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
	// デフォルトハンドル解放
	if (gs_sound_se_handle_default) {
		GsSoundFreeSeHandle(gs_sound_se_handle_default);
		gs_sound_se_handle_default	= NULL;
	}
	
#if _IPHONE
	// amCriAudio.cppより、この処理が無いと下のau_player->Destroyが無限ループ
	// SEで問題が出そうならまた対応する
	int cri_init_count = 0;
	cri_audio_if->auobj->Stop(CriAuObj::STOP_MODE_IMMEDIATE, cri_audio_if->err);
	while (cri_audio_if->auobj->GetPlaybackStatus(cri_audio_if->err) == CriAuObj::PLAYBACK_STATUS_PLAYING) {
		CriAuObj::ExecuteMain(cri_audio_if->err);
		cri_audio_if->sndout->ExecuteMain();
		// あまりに長い間復帰出来ない場合は一回初期化
		if (GSD_SND_NOPLAY_ERROR_CLEAR_SE_CRI_RESTART_LIMIT < cri_init_count++) {
			cri_init_count = 0;
			CriSmpSoundOutput_StopSound();
			CriSmpSoundOutput_ReStartSound();
		}
	}
#endif // _IPHONE
	
	for (Sint32 i = 0; i < GSD_SND_SE_HANDLE_MAX; ++i) {
		gsSoundClearSeHandle(&gs_sound_se_handle_heap[i]);
		if (gs_sound_se_handle_heap[i].au_player) {
			// CriAudioプレーヤを破棄する
			gs_sound_se_handle_heap[i].au_player->Destroy(cri_audio_if->err);
			gs_sound_se_handle_heap[i].au_player	= NULL;
		}
	}
	gsSoundClearSeHandle(GSD_SND_ERROR_SE_HANDLE);
	if (GSD_SND_ERROR_SE_HANDLE->au_player) {
		// CriAudioプレーヤを破棄する
		GSD_SND_ERROR_SE_HANDLE->au_player->Destroy(cri_audio_if->err);
		GSD_SND_ERROR_SE_HANDLE->au_player	= NULL;
	}
	
	// 確保情報をクリア
	amZeroMemory(gs_sound_se_handle_heap_usage_flag, sizeof(gs_sound_se_handle_heap_usage_flag));
}


// =======================================================================
// gsSoundGetFreeSeHandleNum
/*!
  空きSEハンドル数を取得する
  
  @return 空きSEハンドル数
 */
// =======================================================================
Uint32 gsSoundGetFreeSeHandleNum(void)
{
	Uint32	num	= GSD_SND_SE_HANDLE_MAX;
	const static Sint32 elem_num	= (GSD_SND_SE_HANDLE_MAX+7)/8;
	
	// 使用済みのハンドル数分減算していく
	for (Sint32 i = 0; i < elem_num; ++i) {
		num	-= AkMathCountBitPopulation(gs_sound_se_handle_heap_usage_flag[i]);
		MTM_ASSERT(num <= GSD_SND_SE_HANDLE_MAX);
	}
	
	return num;
}

// =======================================================================
// gsSoundInitSeHandle
/*!
  SEハンドルを初期化
  
  @param se_handle	[io]	SEハンドル
  @param b_reset	[in]	リセットフラグ
  							(CUEの解放などを行わずに初期化状態に戻します)
  
  @note
  初期化状態となるようにパラメータを設定します。
 */
// =======================================================================
void gsSoundInitSeHandle(GSS_SND_SE_HANDLE *se_handle, BOOL b_reset/*=FALSE*/)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
	MTM_ASSERT(se_handle);
	MTM_ASSERT(se_handle->au_player);
	
	if (se_handle->au_player) {
		if (CriAuPlayer::STATUS_PREP == se_handle->au_player->GetStatus(cri_audio_if->err)) {
			// 同じフレームで、このハンドルを対象にして再生された場合に
			// CriAuPlayer::Play()以降に行ったパラメータ設定が上書きされてしまうので、
			// 上書きされる前にここでパラメータを反映しておく
			// （ただし反映されるのはCriAuPlayer::Set***で設定を行ったもののみ）
			se_handle->au_player->Update(cri_audio_if->err);
		}
	}
	
	// 一旦ニュートラルな状態にする
	gsSoundClearSeHandle(se_handle, b_reset);
	
	// 初期化済み
	se_handle->flag	|= GSD_SND_SE_HANDLE_FLAG_INITIALIZED;
	
	// ボリューム初期化
	se_handle->snd_ctrl_param.fade_vol	= 1.f;
	se_handle->snd_ctrl_param.fade_sub_vol	= 1.f;
	se_handle->snd_ctrl_param.volume	= 1.f;
}

// =======================================================================
// gsSoundClearSeHandle
/*!
  SEハンドルクリア
  
  @param se_handle		[io]	SEハンドル
  @param b_takeover_cue	[in]	CUE設定引継ぎ(default:FALSE)
  								（CUEを解放しない）
  
  @note
  パラメータのクリア、CriAudioプレーヤの初期化等を行います。
 */
// =======================================================================
void gsSoundClearSeHandle(GSS_SND_SE_HANDLE *se_handle, BOOL b_takeover_cue/*=FALSE*/)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
	MTM_ASSERT(se_handle);
	
	// フラグクリア
	se_handle->flag	= 0;
	
	// ボリュームクリア
	se_handle->snd_ctrl_param.fade_vol	= 0;
	se_handle->snd_ctrl_param.fade_sub_vol	= 0;
	se_handle->snd_ctrl_param.volume	= 0;
	
	// ポーズレベルクリア
	se_handle->cur_pause_level	= GSD_SND_SE_HANDLE_PAUSE_LEVEL_NONE;
	
	// プレイヤーのリセット
	if (se_handle->au_player) {
		if (!b_takeover_cue) {
			se_handle->au_player->ReleaseCue(cri_audio_if->err);
		}
		se_handle->au_player->ResetParameters(cri_audio_if->err);
	}
}

// =======================================================================
// gsSoundUpdateSndSeHandle
/*!
  SEハンドル更新（ボリューム更新前処理）
  
  @param se_handle	[io]	SEハンドル
  
  @note
  SEハンドルに対するフレーム更新処理を行います。
  ボリューム更新処理（gsSoundUpdateVolume）の前に行う処理です。
  プレーヤへのパラメータの反映や終了判定処理などは行いません。
 */
// =======================================================================
void gsSoundUpdateSndSeHandle(GSS_SND_SE_HANDLE *se_handle)
{
	MTM_ASSERT(se_handle);
	
	// サウンドコントロール更新
	gsSoundUpdateSndCtrl(&se_handle->snd_ctrl_param, se_handle->au_player);
}

// =======================================================================
// gsSoundUpdateSeHandleStatus
/*!
  SEハンドルステータス更新
  
  @param se_handle	[io]	SEハンドル（NULL不可）
  
  @note
  停止中・一時停止中などの状態をチェック・更新します。
 */
// =======================================================================
void gsSoundUpdateSeHandleStatus(GSS_SND_SE_HANDLE *se_handle)
{
	MTM_ASSERT(se_handle);
	
	// 再生終了チェック
	if (gsSoundCheckSeHandleStop(se_handle)) {
		se_handle->flag	|= GSD_SND_SE_HANDLE_FLAG_IS_STOP;
	}
	else {
		se_handle->flag	&= ~GSD_SND_SE_HANDLE_FLAG_IS_STOP;
	}
	
	// 一時停止中チェック
	if (gsSoundCheckSeHandlePause(se_handle)) {
		se_handle->flag	|= GSD_SND_SE_HANDLE_FLAG_IS_PAUSE;
	}
	else {
		se_handle->flag	&= ~GSD_SND_SE_HANDLE_FLAG_IS_PAUSE;
	}
}

// =======================================================================
// gsSoundCheckSeHandlePause
/*!
  SEハンドル一時停止中判定
  
  @param se_handle	[in]	SEハンドル（NULL不可）
  
  @note
  CRI Audioの情報から一時停止中判定を行います。
 */
// =======================================================================
BOOL gsSoundCheckSeHandlePause(const GSS_SND_SE_HANDLE *se_handle)
{
	MTM_ASSERT(se_handle);
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
	if (se_handle->au_player) {
		if (se_handle->snd_ctrl_param.fade_state == GSE_SND_FADE_STATE_FADING_OUT_TO_PAUSE ||
			se_handle->au_player->IsPaused(cri_audio_if->err)) {
			return TRUE;
		}
	}
	
	return FALSE;
}

// =======================================================================
// gsSoundCheckSeHandleStop
/*!
  SEハンドル再生停止中判定
  
  @param se_handle	[in]	SEハンドル（NULL不可）
  
  @note
  CRI Audioの情報から停止中判定を行います。
 */
// =======================================================================
BOOL gsSoundCheckSeHandleStop(const GSS_SND_SE_HANDLE *se_handle)
{
	MTM_ASSERT(se_handle);
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
	if (se_handle->au_player) {
		switch (se_handle->au_player->GetStatus(cri_audio_if->err)) {
		case CriAuPlayer::STATUS_ERROR:		// no break
		case CriAuPlayer::STATUS_STOP:		// no break
		case CriAuPlayer::STATUS_PLAYEND:
			return TRUE;
			
		default:
			return FALSE;
		}
	}
	
	return TRUE;
}

// =======================================================================
// gsSoundIsSeHandleCueSet
/*!
  Cue設定済み判定
  
  @param se_handle	[in]	SEハンドル
  
  @retval TRUE	Cue設定済み
  @retval FALSE	Cue未設定
 
  @note
  CriAuPlayer::SetCue()を行った直後にCriAuPlayer::Play()を呼んでいることが前提となっています。
  SetCue()を行っただけの場合、この関数では「設定済み」と判定されません。
 */
// =======================================================================
inline BOOL gsSoundIsSeHandleCueSet(const GSS_SND_SE_HANDLE *se_handle)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
	MTM_ASSERT(se_handle);
	
	if (se_handle->au_player) {
		CriAuPlayer::Status status	= se_handle->au_player->GetStatus(cri_audio_if->err);
		if (status == CriAuPlayer::STATUS_STOP ||
			status == CriAuPlayer::STATUS_PLAYEND) {
			return FALSE;
		}
	}
	else {
		return FALSE;
	}
	
	return TRUE;
}

// =======================================================================
// gsSoundGetDefaultSeHandle
/*!
  デフォルトハンドル取得
  
  @return デフォルトハンドル
 */
// =======================================================================
inline GSS_SND_SE_HANDLE* gsSoundGetDefaultSeHandle(void)
{
	MTM_ASSERT(gs_sound_se_handle_default);
	
	return gs_sound_se_handle_default;
}

// =======================================================================
// gsSoundCriSeSetFadeIn
/*!
  SE用フェードイン設定
  
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数
  
  @note
  SE(CRI)用にフェードインパラメータを設定します。
  fade_frameに0を指定した場合は通常の再生開始のパラメータ設定になります。
 */
// =======================================================================
void gsSoundCriSeSetFadeIn(GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame)
{
	MTM_ASSERT(se_handle);
	MTM_ASSERT(fade_frame >= 0);
	
	if (fade_frame == 0) {
		se_handle->snd_ctrl_param.fade_state	= GSE_SND_FADE_STATE_NORMAL;
		se_handle->snd_ctrl_param.fade_frame_max	= 0;
		se_handle->snd_ctrl_param.fade_frame_cnt	= 0;
		se_handle->snd_ctrl_param.fade_vol			= 1.f;
	}
	else {
		se_handle->snd_ctrl_param.fade_state	= GSE_SND_FADE_STATE_FADING_IN;
		se_handle->snd_ctrl_param.fade_frame_max	= fade_frame;
		se_handle->snd_ctrl_param.fade_frame_cnt	= 0;
		se_handle->snd_ctrl_param.fade_vol			= 0.f;	//! 最初は無音
	}
	
	se_handle->snd_ctrl_param.fade_sub_vol	= 1.f;
}

// =======================================================================
// gsSoundCriSeStop
/*!
  CRI SE 停止
 
  @param se_handle		[io]	SEハンドル
  @param fade_frame		[in]	フェードアウトにかけるフレーム数
  @param is_immediate	[in]	即時停止フラグ
  @param is_takeover	[in]	フェードボリューム引き継ぎフラグ
  								（デフォルト：FALSE）
  @note
  CRIのSEをフェードアウトしながら停止させます。
  fade_frameに0を指定すると直ちに停止します。
  is_immediateをTRUEに指定するとfade_frame==0の場合に即時停止します
  （データのリリースタイムも無視されます）。
 */
// =======================================================================
void gsSoundCriSeStop(GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame, BOOL is_immediate,
					  BOOL is_takeover/*=FALSE*/)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
	MTM_ASSERT(se_handle);
	
	if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED &&
		se_handle->au_player != NULL) {
		
		if (is_takeover) {
			se_handle->snd_ctrl_param.fade_sub_vol
				= se_handle->snd_ctrl_param.fade_sub_vol * se_handle->snd_ctrl_param.fade_vol;
		}
		else {
			se_handle->snd_ctrl_param.fade_sub_vol	= 1.f;
		}
		
		
		if (fade_frame == 0) {
			se_handle->snd_ctrl_param.fade_state	= GSE_SND_FADE_STATE_NORMAL;
			se_handle->snd_ctrl_param.fade_frame_max	=
				se_handle->snd_ctrl_param.fade_frame_cnt	= 0;
			se_handle->snd_ctrl_param.fade_vol	= 0.f;
			if (is_immediate) {
				se_handle->au_player->Stop(CriAuPlayer::STOP_MODE_IMMEDIATE, cri_audio_if->err);
			}
			else {
				se_handle->au_player->Stop(cri_audio_if->err);
			}
		}
		else {
			se_handle->snd_ctrl_param.fade_state	= GSE_SND_FADE_STATE_FADING_OUT_TO_STOP;
			se_handle->snd_ctrl_param.fade_frame_max	= fade_frame;
			se_handle->snd_ctrl_param.fade_frame_cnt	= 0;
			se_handle->snd_ctrl_param.fade_vol			= 1.f;	//! 現在の音量から減衰させる
		}
	}
}

// =======================================================================
// gsSoundCriSePause
/*!
  CRI SE 一時停止
  
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードアウトにかけるフレーム数
  
  @note
  CRIのSEをフェードアウトしながら一時停止させます。
  fade_frameに0を指定すると直ちに一時停止します。
 */
// =======================================================================
void gsSoundCriSePause(GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
	MTM_ASSERT(se_handle);
	MTM_ASSERT(fade_frame >= 0);
	
	// 停止へのフェードアウト中にポーズを実行した場合は指定フレームでそのまま停止させる
	// TODO : 次回実装では廃止予定
	if (se_handle->snd_ctrl_param.fade_state == GSE_SND_FADE_STATE_FADING_OUT_TO_STOP) {
		gsSoundCriSeStop(se_handle, fade_frame, TRUE, TRUE);
		return;
	}
	
	if (fade_frame == 0) {
		se_handle->snd_ctrl_param.fade_state	= GSE_SND_FADE_STATE_NORMAL;
		se_handle->snd_ctrl_param.fade_frame_max	=
			se_handle->snd_ctrl_param.fade_frame_cnt	= 0;
		se_handle->snd_ctrl_param.fade_vol	= 0.f;
		se_handle->au_player->Pause(TRUE, cri_audio_if->err);
	}
	else {
		se_handle->snd_ctrl_param.fade_state	= GSE_SND_FADE_STATE_FADING_OUT_TO_PAUSE;
		se_handle->snd_ctrl_param.fade_frame_max	= fade_frame;
		se_handle->snd_ctrl_param.fade_frame_cnt	= 0;
		se_handle->snd_ctrl_param.fade_vol	= 1.f;	//! 現在の音量から減衰させる
	}
}

// =======================================================================
// gsSoundCriSeResume
/*!
  CRI SE 再開
  
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数
  
  @note
  CRIのSEをフェードインしながら一時停止から再開させます。
  fade_frameに0を指定すると直ちに再開します。
 */
// =======================================================================
void gsSoundCriSeResume(GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	MTM_ASSERT(se_handle);
	
	if (!gsSoundCheckSeHandlePause(se_handle)) {
		// ポーズ中以外の場合は無視
		return;
	}
	
	// ポーズ解除
	se_handle->au_player->Pause(FALSE, cri_audio_if->err);
	
	if (fade_frame == 0) {
		se_handle->snd_ctrl_param.fade_state	= GSE_SND_FADE_STATE_NORMAL;
		se_handle->snd_ctrl_param.fade_frame_max	=
			se_handle->snd_ctrl_param.fade_frame_cnt	= 0;
		se_handle->snd_ctrl_param.fade_vol	= 1.f;
	}
	else {
		// フェードイン設定
		gsSoundCriSeSetFadeIn(se_handle, fade_frame);
	}
}

#if _IPHONE
// =======================================================================
// GsSoundPlaySe
/*!
  SE再生
  
  @param se_name	[in]	再生するSEのキュー名
  @param se_id		[in]	再生するSEのキューID
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
  
  @note se_nameが非NULLなら se_name、NULLなら se_idを使った再生を行う
  
 */
// =======================================================================
void gsSoundPlaySe(const char *se_name, Uint32 se_id, GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame)
{
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();

	if (NULL == se_handle) {
		// デフォルトハンドル取得
		se_handle	= gsSoundGetDefaultSeHandle();
		MTM_ASSERT(se_handle->au_player);
		// デフォルトハンドルに FLAG_NO_AUTOCLEAR の使用は禁止
		MTM_ASSERT(!(se_handle->flag & GSD_SND_SE_HANDLE_FLAG_NO_AUTOCLEAR));
	}
	
	// ポーズ中のプレーヤに対して新たにCueをセットする場合は
	// Stop()することで、既に関連付けられているボイスを解放しておく
	// （さもなくば、ボイスが確保されたまま残ってしまうため）
	if (se_handle->au_player->IsPaused(cri_audio_if->err)) {
		se_handle->au_player->Stop(CriAuPlayer::STOP_MODE_IMMEDIATE, cri_audio_if->err);
	}
	
	// ハンドル初期化
	if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_NO_AUTOCLEAR) {
		// 自動解放しない場合はリセットだけする
		
		// ハンドルは初期化済みであることが前提
		MTM_ASSERT(se_handle->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED);
		
		// ハンドルリセット（CUE解放しない）
		gsSoundInitSeHandle(se_handle, TRUE);
		
		// 一旦セットされたNO_AUTOCLEARフラグは、この関数を使用する限りオフにしない
		se_handle->flag	|= GSD_SND_SE_HANDLE_FLAG_NO_AUTOCLEAR;
	}
	else {
		gsSoundInitSeHandle(se_handle, FALSE);
	}
	
	// 再生開始済みフラグ設定
	se_handle->flag	|= GSD_SND_SE_HANDLE_FLAG_PLAY_STARTED;
	
	// 再生
	if (se_name) {
		// キュー名での再生
		se_handle->au_player->SetCue(se_name, cri_audio_if->err);
	}
	else {
		// キューIDでの再生
		se_handle->au_player->SetCueById(se_id, cri_audio_if->err);
	}
	gsSoundCriSeSetFadeIn(se_handle, fade_frame);	// フェードイン設定
	gsSoundSeHandleUpdateVolume(se_handle);
	se_handle->au_player->Play(cri_audio_if->err);	// MEMO : Play()直後はSTATUS_PREPになる
}

BOOL gsSoundIsSystemSuspendWait(void)
{
	BOOL flag = FALSE;
	if (GsSoundGetSysMainInfo()->suspend_wait_count > 0) {
		flag = TRUE;
	}
	return flag;
}

void gsSoundUpdateSystemSuspendWait(void)
{
	GSS_SND_SYS_MAIN_INFO* info = GsSoundGetSysMainInfo();
	// カウント更新
	if (info->suspend_wait_count > 0) {
		info->suspend_wait_count--;
	}
	// サスペンドチェック
	if (GsMainSysGetSuspendedFlag()) {
		info->suspend_wait_count = GSD_SND_SUSPEND_WAIT_COUNT;
	}
}

#endif // _IPHONE


#if _WII
// ############################################################################
// HBM関連
// ############################################################################

// =======================================================================
// gsSoundWiiStopDesignatedSoundForHbm
/*!
  HBM移行用に指定されたサウンドを停止
  
  @note
  GSD_SND_SCB_FLAG_STOP_ON_HBMフラグが設定されたSCB、
  および、GSD_SND_SE_HANDLE_FLAG_STOP_ON_HBMが設定されたSEハンドルを再生停止します。
 */
// =======================================================================
void gsSoundWiiStopDesignatedSoundForHbm(void)
{
	// 指定SCBを停止
	for (Sint32 i = 0; i < GSD_SND_SCB_MAX; ++i) {
		GSS_SND_SCB	*scb	= &gs_sound_scb_heap[i];
		
		if ((scb->flag & GSD_SND_SCB_FLAG_INITIALIZED) &&
			(scb->flag & GSD_SND_SCB_FLAG_STOP_ON_HBM)) {
			
			GsSoundStopBgm(scb);
		}
	}
	
	// 指定SEを停止
	for (Sint32 i = 0; i < GSD_SND_SE_HANDLE_MAX; ++i) {
		GSS_SND_SE_HANDLE	*se_handle	= &gs_sound_se_handle_heap[i];
		
		if ((se_handle->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED) &&
			(se_handle->flag & GSD_SND_SE_HANDLE_FLAG_STOP_ON_HBM)) {
			gsSoundCriSeStop(se_handle, 0, TRUE);
		}
	}
}

// =======================================================================
// gsSoundWiiUpdateHBMSeq
/*!
  HBMサウンド処理シーケンス更新
  
  @note
  Wii以外では何もしません。
 */
// =======================================================================
void gsSoundWiiUpdateHBMSeq(void)
{
	// 更新処理
	if (gs_sound_sys_main_info.proc_hbm) {
		gs_sound_sys_main_info.proc_hbm();
	}
}

// ============================================================================
// HBMへ移行時のシーケンス
// ============================================================================
// =======================================================================
// gsSoundWiiEnterHBMProc***
/*!
  HBMへ移行時シーケンス 処理関数
 */
// =======================================================================
// HBM移行シーケンス 初期化処理
// （※直接呼び出さず、プロシージャとして設定してください。）
void gsSoundWiiEnterHBMProcInit(void)
{
	MTM_ASSERT(gs_sound_sys_main_info.flag & GSD_SND_SYS_MAIN_FLAG_ENTER_HBM_FADING);
	
	// AUXバスリターンボリュームを退避
	gsSoundWiiSaveAuxReturnVol();
	
	// システム制御ボリューム反映を有効化
	gsSoundSetEnableSystemControlVolume(TRUE);
	// システム制御ボリューム初期化
	gs_sound_sys_main_info.system_cnt_vol	= 1.0;
	
	// エフェクトクリア処理開始
	nw4r::snd::SoundSystem::ClearEffect(nw4r::snd::AUX_A, GSD_SND_WII_HBM_CLEAR_EFFECT_FADEOUT_TIME_MSEC);
	nw4r::snd::SoundSystem::ClearEffect(nw4r::snd::AUX_B, GSD_SND_WII_HBM_CLEAR_EFFECT_FADEOUT_TIME_MSEC);
	nw4r::snd::SoundSystem::ClearEffect(nw4r::snd::AUX_C, GSD_SND_WII_HBM_CLEAR_EFFECT_FADEOUT_TIME_MSEC);
	
	// メイン出力ボリューム退避
	gsSoundWiiSaveMainOutVolume();
	
	// メイン出力ボリュームフェード初期化
	nw4r::snd::SoundSystem::SetMainOutVolume(0.f, GSD_SND_WII_HBM_MAINOUT_VOL_FADEOUT_TIME_MSEC);
	
	// シーケンス設定
	gs_sound_sys_main_info.proc_hbm	= gsSoundWiiEnterHBMProcFadeOut;
}

// HBM移行シーケンス フェードアウト処理
void gsSoundWiiEnterHBMProcFadeOut(void)
{
	BOOL	result	= TRUE;
	
	// エフェクトクリア終了チェック
	if (!(nw4r::snd::SoundSystem::IsFinishedClearEffect(nw4r::snd::AUX_A) &&
		  nw4r::snd::SoundSystem::IsFinishedClearEffect(nw4r::snd::AUX_B) &&
		  nw4r::snd::SoundSystem::IsFinishedClearEffect(nw4r::snd::AUX_C))) {
		result	= FALSE;
	}
	
	// メイン出力ボリュームフェード終了チェック
	if (nw4r::snd::SoundSystem::GetMainOutVolume() > 0.f) {
		result	= FALSE;
	}
	
	if (result) {
		
		// システム制御ボリュームをミュート状態に
		gs_sound_sys_main_info.system_cnt_vol	= 0.f;
		
		gs_sound_sys_main_info.proc_hbm	= gsSoundWiiEnterHBMProcFinalize;
	}
}

// HBM移行シーケンス 終了処理
void gsSoundWiiEnterHBMProcFinalize(void)
{
	// HBM移行時に停止する設定となっているサウンドを停止する
	gsSoundWiiStopDesignatedSoundForHbm();
	
	// MEMO:
	//   １つのプレイヤーに対して複数のサウンドが関連付けられている場合、
	//   最後にSetCueされたサウンド以外はミュートできないので、ポーズしてしまう。）
	
	// BGMポーズ
	gsSoundPauseAllScb(GSD_SND_SCB_PAUSE_LEVEL_HBM);
	
	// SEポーズ
	GsSoundPauseSe(GSD_SND_SE_HANDLE_PAUSE_LEVEL_HBM, 0);
	
	// メイン出力ボリュームを復帰（HBMでサウンドが鳴るように）
	gsSoundWiiRestoreMainOutVolume();
	
	// HBM移行時のサウンド操作処理完了
	gs_sound_sys_main_info.flag	&= ~GSD_SND_SYS_MAIN_FLAG_ENTER_HBM_FADING;
	
	// シーケンス終了
	gs_sound_sys_main_info.proc_hbm	= NULL;
}

// ============================================================================
// HBMから復帰時のシーケンス
// ============================================================================
// =======================================================================
// gsSoundWiiLeaveHBMProc***
/*!
  HBMから復帰時シーケンス 処理関数
 */
// =======================================================================
// HBMから復帰時シーケンス 初期化処理
// （※直接呼び出さず、プロシージャとして設定してください。）
void gsSoundWiiLeaveHBMProcInit(void)
{
	MTM_ASSERT(gs_sound_sys_main_info.flag & GSD_SND_SYS_MAIN_FLAG_LEAVE_HBM_FADING);
	
	// メイン出力ボリュームフェードイン開始音量設定
	nw4r::snd::SoundSystem::SetMainOutVolume(0.f, 0);
	
	gs_sound_sys_main_info.proc_hbm	= gsSoundWiiLeaveHBMProcStartFadeIn;
}

// HBMから復帰時シーケンス フェードイン開始処理
void gsSoundWiiLeaveHBMProcStartFadeIn(void)
{
	// MEMO: ↓この方法でHBMのサウンドが途中で鳴らなくなってしまう場合は
	//       システム制御ボリュームでフェードインするように変更する。
	//       （同一プレーヤで複数のサウンドを鳴らすと最後にSetCueしたサウンド以外は
	//       システム制御ボリュームが反映されなくなる問題があるが、
	//       この時点でSEは止まっているので問題ないはず）
	
	// メイン出力ボリュームフェードイン開始
	nw4r::snd::SoundSystem::SetMainOutVolume(gsSoundWiiGetSavedMainOutVolume(),
											 GSD_SND_WII_HBM_MAINOUT_VOL_FADEIN_TIME_MSEC);
	
	// システム制御ボリュームを無効化（＝ミュート状態を解除）
	gsSoundSetEnableSystemControlVolume(FALSE);
	gs_sound_sys_main_info.system_cnt_vol	= 1.f;
	
	// SE再開
	GsSoundResumeSe(GSD_SND_SE_HANDLE_PAUSE_LEVEL_HBM, 0);
	
	// BGM再開
	gsSoundResumeAllScb(GSD_SND_SCB_PAUSE_LEVEL_HBM);
	
	// AUXバスリターンボリュームを復帰
	gsSoundWiiRestoreAuxReturnVol();
	
	gs_sound_sys_main_info.proc_hbm	= gsSoundWiiLeaveHBMProcFadeIn;
}

// HBMから復帰時シーケンス フェードイン処理
void gsSoundWiiLeaveHBMProcFadeIn(void)
{
	// メイン出力ボリュームフェード終了チェック
	if (nw4r::snd::SoundSystem::GetMainOutVolume() >= gsSoundWiiGetSavedMainOutVolume()) {
		
		// HBM退出時のサウンド操作処理完了
		gs_sound_sys_main_info.flag	&= ~GSD_SND_SYS_MAIN_FLAG_LEAVE_HBM_FADING;
		
		// シーケンス終了
		gs_sound_sys_main_info.proc_hbm	= NULL;
	}
}

#endif /* _WII */


// ==========================================================================
// GsSoundStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void GsSoundStaticVarInit(void)
{
	//! GSサウンドシステムフレーム処理タスクTCB
	gs_sound_tcb = NULL;
	//! サウンドシステムメイン情報
	memset(&gs_sound_sys_main_info, 0, sizeof(gs_sound_sys_main_info));
	//! SCBヒープ
	memset(gs_sound_scb_heap, 0, sizeof(gs_sound_scb_heap));
	//! SCBの使用済みフラグ
	memset(gs_sound_scb_heap_usage_flag, 0, sizeof(gs_sound_scb_heap_usage_flag));
	
	//! SEハンドルヒープ
	memset(gs_sound_se_handle_heap, 0, sizeof(gs_sound_se_handle_heap));
	//! SEハンドルの使用済みフラグ
	memset(gs_sound_se_handle_heap_usage_flag, 0, sizeof(gs_sound_se_handle_heap_usage_flag));
	//! エラーSEハンドル（SEハンドルの取得に失敗したときのハンドル）
	memset(&gs_sound_se_handle_error, 0, sizeof(gs_sound_se_handle_error));
	//! デフォルトSEハンドル（実体はSEハンドルヒープから確保したハンドル）
	gs_sound_se_handle_default	= NULL;
	//! ボリューム
	memset(gs_sound_volume, 0, sizeof(gs_sound_volume));
	
#if GSD_SND_SE_WAIT_CTRL
	//!< SE連続再生抑制 
	memset(&gs_sound_se_wait_ctrl, 0, sizeof(gs_sound_se_wait_ctrl));
#endif // GSD_SND_SE_WAIT_CTRL
	
#if GSD_SND_SE_PLAY_LIMIT_CTRL
	memset(&gs_sound_se_play_limit_ctrl, 0, sizeof(gs_sound_se_play_limit_ctrl));
#endif // GSD_SND_SE_PLAY_LIMIT_CTRL
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
