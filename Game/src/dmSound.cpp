// ===========================================================================
/*!
	@file	dmSound.cpp
	@brief	デモ・サウンドモジュール

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: dmSound.cpp 20 2011-04-22 12:46:46Z thamada $
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

#include "gs.h"
#include "gsSound.h"
#include "dmSound.h"

//----- Definitions ---------------------------------------------------------
#ifdef SONIC4_TRIAL
#define SOUND_PATH	"SOUND/TRIAL/"
#else
#define SOUND_PATH	"SOUND/SOUND/"
#endif// SONIC4_TRIAL

#define DMD_SOUND_SE_FILE_PATH	GSS_BASE_PATH SOUND_PATH"SND_FX.CSB"	//!< SEサウンドファイルパス

//! サウンドデータインデックス列挙型
typedef enum tag_DME_SOUND_DATA_IDX {
	DME_SOUND_DATA_IDX_SE	= 0,
	DME_SOUND_DATA_IDX_BGM,
	
	DME_SOUND_DATA_IDX_MAX
} DME_SOUND_DATA_IDX;


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
//! データワークリスト
static GSS_SND_DATA_WORK dm_sound_data_work_list[DME_SOUND_DATA_IDX_MAX]	= {{0}};

//! BGM用SCBへのポインタ
static GSS_SND_SCB *dm_sound_bgm_scb	= NULL;
//! ジングル用SCBへのポインタ
static GSS_SND_SCB *dm_sound_jingle_scb	= NULL;

//! CSB/BRSARファイル名リスト
static const char *dm_sound_bgm_csb_brsar_file_list[] = {
#if _WII
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG01.BRSAR",
#elif _IPHONE
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG_TITLE.CSB",
#else
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG01.CSB",
#endif /* _WII */
};

//! CPKファイル名リスト		※最終的にCPKはBGMとジングルは一つにまとまる
static const char *dm_sound_bgm_cpk_file_list[] = {
#if _WII
	NULL,
#elif _IPHONE
	NULL,
#else
	GSS_BASE_PATH SOUND_PATH"SONICDL_SNG01.CPK",
#endif /* _WII */
};

//! BGM CueName/サウンドアーカイブラベル リスト
static const char *dm_sound_bgm_name_list[] = {
#if _WII
	"snd_sng_menu",
#else
	"snd_sng_menu",
//	"SNG_MENU",
#endif /* _WII */
};

//! ジングル CueName/サウンドアーカイブラベル リスト
static const char *dm_sound_jingle_name_list[DME_SOUND_JINGLE_IDX_MAX]	= {
#if _WII
	"snd_sng_title",
#else
	"snd_sng_title",
#endif /* _WII */
};

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// DmSoundBuild
/*!
 *	デモサウンド 構築
 */
// ==========================================================================
void DmSoundBuild(void)
{
	
	// データワーク初期化
	for (Sint32 i = 0; i < DME_SOUND_DATA_IDX_MAX; ++i) {
		GsSoundInitDataWork(&dm_sound_data_work_list[i]);
	}
	
	// SEビルド開始
	GsSoundBuildSeInit(&dm_sound_data_work_list[DME_SOUND_DATA_IDX_SE],
					   AME_CRIAUDIO_CSB_GAME,
					   DMD_SOUND_SE_FILE_PATH,
					   0x1000);
	// BGMビルド開始
	GsSoundBuildBgmInit(&dm_sound_data_work_list[DME_SOUND_DATA_IDX_BGM],
						dm_sound_bgm_csb_brsar_file_list[0],//仮
						dm_sound_bgm_cpk_file_list[0],//仮
						0x1000,
#if _WII
						GSE_SND_DATA_TYPE_NW4R);
#else
						GSE_SND_DATA_TYPE_CRIAUDIO);
#endif /* _WII */
}

// ==========================================================================
// DmSoundBuildCheck
/*!
 *	サウンドデータ構築 終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL DmSoundBuildCheck(void)
{
	// SEビルド更新・完了チェック
	if (FALSE == GsSoundBuildSeUpdate(&dm_sound_data_work_list[DME_SOUND_DATA_IDX_SE])) {
		return FALSE;
	}
	
	// BGMビルド更新・完了チェック
	if (FALSE == GsSoundBuildBgmUpdate(&dm_sound_data_work_list[DME_SOUND_DATA_IDX_BGM])) {
		return FALSE;
	}
	
	return TRUE;
}

// ==========================================================================
// DmSoundFlush
/*!
 *	ゲームサウンド 片付け
 */
// ==========================================================================
void DmSoundFlush(void)
{
	// BGMデータフラッシュ
	GsSoundFlushBgm();
	// SEデータフラッシュ
	GsSoundFlushSe(&dm_sound_data_work_list[DME_SOUND_DATA_IDX_SE]);
}

// ==========================================================================
// DmSoundInit
/*!
 *	ゲームサウンド初期化
 */
// ==========================================================================
void DmSoundInit(void)
{
	// サウンドシステムリセット
	GsSoundReset();
	
	// SCBを割り当て
#if _WII
	dm_sound_bgm_scb	= GsSoundAssignScb(GSE_SND_DATA_TYPE_NW4R);
	dm_sound_jingle_scb	= GsSoundAssignScb(GSE_SND_DATA_TYPE_NW4R);
#else
	dm_sound_bgm_scb	= GsSoundAssignScb(GSE_SND_DATA_TYPE_CRIAUDIO);
	dm_sound_jingle_scb	= GsSoundAssignScb(GSE_SND_DATA_TYPE_CRIAUDIO);
#endif
	
	// フレーム更新処理開始
	GsSoundBegin(0x1000,
				 1,
				 3);
}

// ==========================================================================
// DmSoundExit
/*!
 *	ゲームサウンド終了処理
 */
// ==========================================================================
void DmSoundExit(void)
{
	// サウンド停止処理
	GsSoundHalt();
	
	// フレーム更新処理終了
	GsSoundEnd();
	
	// SCB破棄
	if (dm_sound_jingle_scb) {
		GsSoundStopBgm(dm_sound_jingle_scb, 0);
		GsSoundResignScb(dm_sound_jingle_scb);
		dm_sound_jingle_scb	= NULL;
	}
	if (dm_sound_bgm_scb) {
		GsSoundStopBgm(dm_sound_bgm_scb, 0);
		GsSoundResignScb(dm_sound_bgm_scb);
		dm_sound_bgm_scb	= NULL;
	}
	
	// サウンドシステムリセット
	GsSoundReset();
}

// ==========================================================================
// SE再生
// ==========================================================================
// ==========================================================================
// DmSoundPlaySE
/*!
 *	SE再生
 *
 *	@param	cue_name	[in]	再生するSEのキュー名
 *
 */
// ==========================================================================
void DmSoundPlaySE(char *cue_name)
{
	GsSoundPlaySe(cue_name);
}

// ==========================================================================
// BGM
// ==========================================================================
// =======================================================================
// DmSoundPlayBGM
/*!
  BGM再生
  
  @param cue_name		[in]	再生するSEのキュー名
  @param fade_farame	[in]	フェードインにかけるフレーム数
  
  @note
  この関数は互換性のために残しています。
  代わりにDmSoundPlayStageBGM()を使用してください。
 */
// =======================================================================
void DmSoundPlayBGM(const char *cue_name, Sint32 fade_frame/*=0*/)
{
	UNREFERENCED_PARAMETER(cue_name);
	
	if (dm_sound_bgm_scb) {
		GsSoundPlayBgm(dm_sound_bgm_scb
						, cue_name
						, fade_frame);
	}
	else {
//		MTM_ASSERT(0);
	}
	
	// ユーザーBGM再生時のミュート対応
	dm_sound_bgm_scb->flag |= GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM;
}


// =======================================================================
// DmSoundStopBGM
/*!
  BGM停止
  
  @param fade_frame	[in]	フェードアウトにかけるフレーム数
  
  @note
  この関数は互換性のために残しています。
  代わりにDmSoundStopStageBGM()を使用してください。
 */
// =======================================================================
void DmSoundStopBGM(Sint32 fade_frame/*=0*/)
{
	DmSoundStopStageBGM(fade_frame);
}

// =======================================================================
// DmSoundPlayStageBGM
/*!
  メニューBGM再生
  
  @param fade_frame	[in]	フェードインにかけるフレーム数
  
  @note
  メニューBGMを再生します。
 */
// =======================================================================
void DmSoundPlayMenuBGM(DME_SOUND_BGM_IDX idx, Sint32 fade_frame/*=0*/)
{
	if (dm_sound_bgm_scb) {
		// BGM再生(テーブル参照)
		GsSoundPlayBgm(dm_sound_bgm_scb
						, dm_sound_bgm_name_list[idx]
						, fade_frame);
	}
	else {
//		MTM_ASSERT(0);
	}
	
	// ユーザーBGM再生時のミュート対応
	dm_sound_bgm_scb->flag |= GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM;
}


// =======================================================================
// DmSoundStopStageBGM
/*!
  ステージBGM停止
  
  @param fade_frame	[in]	フェードアウトにかけるフレーム数
 */
// =======================================================================
void DmSoundStopStageBGM(Sint32 fade_frame/*=0*/)
{
	if (dm_sound_bgm_scb) {
		GsSoundStopBgm(dm_sound_bgm_scb, fade_frame);
	}
}


// =======================================================================
// DmSoundPauseStageBGM
/*!
  ステージBGM一時停止
  
  @param fade_frame	[in]	フェードアウトにかけるフレーム数
 */
// =======================================================================
void DmSoundPauseStageBGM(Sint32 fade_frame/*=0*/)
{
	if (dm_sound_bgm_scb) {
		GsSoundPauseBgm(dm_sound_bgm_scb, fade_frame);
	}
}

// =======================================================================
// DmSoundResumeStageBGM
/*!
  ステージBGM再開
  
  @param fade_frame	[in]	フェードインにかけるフレーム数
 */
// =======================================================================
void DmSoundResumeStageBGM(Sint32 fade_frame/*=0*/)
{
	if (dm_sound_bgm_scb) {
		GsSoundResumeBgm(dm_sound_bgm_scb, fade_frame);
	}
}

// ==========================================================================
// ジングル
// ==========================================================================
// =======================================================================
// DmSoundPlayJingle
/*!
  ジングル再生
  
  @param jngl_idx	[in]	ジングルインデックス（DME_SOUND_JINGLE_IDX_XXX）
  @param fade_frame	[in]	フェードインにかけるフレーム数
 */
// =======================================================================
void DmSoundPlayJingle(DME_SOUND_JINGLE_IDX jngl_idx, Sint32 fade_frame/*=0*/)
{
	MTM_ASSERT(jngl_idx >= 0 && jngl_idx < DME_SOUND_JINGLE_IDX_MAX);
	
	GsSoundStopBgm(dm_sound_jingle_scb);
	GsSoundPlayBgm(dm_sound_jingle_scb,
				   dm_sound_jingle_name_list[jngl_idx],
				   fade_frame);
	
	// ユーザーBGM再生時のミュート対応
	dm_sound_jingle_scb->flag |= GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM;
}

// =======================================================================
// DmSoundStopJingle
/*!
  ジングル停止
  
  @param fade_frame	[in]	フェードアウトにかけるフレーム数
 */
// =======================================================================
void DmSoundStopJingle(Sint32 fade_frame/*=0*/)
{
	GsSoundStopBgm(dm_sound_jingle_scb, fade_frame);
}

// ==========================================================================
// ボリューム設定
// ==========================================================================
// =======================================================================
// DmSoundSetVolumeSE
/*!
  SEボリューム設定
  
  @param volume	[in]	ボリューム（倍率）
  
  @note
  SE全体のボリュームを設定します。
  引数のボリュームは0～10の範囲で、
  実際の設定値の範囲は0.0～1.0になるため、
  ここで変換します。
 */
// =======================================================================
void DmSoundSetVolumeSE(Float volume)
{
	GSS_MAIN_SYS_INFO	*gs_main = GsGetMainSysInfo();
	float set_vol = 0.f;
	
	MTM_ASSERT(volume >= 0.f && volume <= 10.0f);
	
	if (volume != 0) {
		set_vol = volume / 10.f;
	}
	else {
		set_vol = 0.f;
	}
	
	MTM_ASSERT(set_vol >= 0.f && set_vol <= 1.0f);
	
	// gsに保存
	gs_main->se_volume = set_vol;
	
	GsSoundSetVolume(GSE_SND_TYPE_SE, set_vol);
}

// =======================================================================
// DmSoundSetVolumeBGM
/*!
  BGMボリューム設定
  
  @param volume	[in]	ボリューム（倍率）
  
  @note
  BGM全体のボリュームを設定します（ジングルも含む）。
 */
// =======================================================================
void DmSoundSetVolumeBGM(Float volume)
{
	GSS_MAIN_SYS_INFO	*gs_main = GsGetMainSysInfo();
	float set_vol = 0.f;
	
	MTM_ASSERT(volume >= 0.f && volume <= 10.0f);
	
	if (volume != 0) {
		set_vol = volume / 10.f;
	}
	else {
		set_vol = 0.f;
	}
	
	MTM_ASSERT(set_vol >= 0.f && set_vol <= 1.0f);
	
	// gsに保存
	gs_main->bgm_volume = set_vol;
	
	GsSoundSetVolume(GSE_SND_TYPE_BGM, set_vol);
}


// =======================================================================
// DmSoundIsStopStageBGM
/*!
  ステージBGM停止チェック
  
  @param fade_frame	[in]	フェードアウトにかけるフレーム数
 */
// =======================================================================
BOOL DmSoundIsStopStageBGM(void)
{
	if (GsSoundIsBgmStop(dm_sound_bgm_scb)) {
		return TRUE;
	}
	
	return FALSE;
}


// =======================================================================
// DmSoundIsStopJingle
/*!
  ジングル停止チェック
  
  @param fade_frame	[in]	フェードアウトにかけるフレーム数
 */
// =======================================================================
BOOL DmSoundIsStopJingle(void)
{
	if (GsSoundIsBgmStop(dm_sound_jingle_scb)) {
		return TRUE;
	}
	
	return FALSE;
}


// ==========================================================================
// DmSoundStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void DmSoundStaticVarInit(void)
{
	//! データワークリスト
	memset(dm_sound_data_work_list, 0, sizeof(dm_sound_data_work_list));
	
	//! BGM用SCBへのポインタ
	dm_sound_bgm_scb = NULL;
	//! ジングル用SCBへのポインタ
	dm_sound_jingle_scb	= NULL;
}


//----- Local Functions -----------------------------------------------------



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
