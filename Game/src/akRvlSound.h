// =======================================================================
/*!
  @file	akRvlSound.h
  @brief NintendoWare for Revolutionサウンド上位ライブラリ
  
  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: akRvlSound.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef AK_RVL_SOUND_H_
#define AK_RVL_SOUND_H_

#if _WII	// Wiiのみ使用

/*------ Include Files -------------------------------------------------*/
#include <nw4r/ut.h>
#include <nw4r/snd.h>

/*------ Macros --------------------------------------------------------*/
//! サウンドスレッドのデフォルトプライオリティ
#define AKD_RVL_SND_SOUND_THREAD_DEFAULT_PRIO	(10)
//! サウンドデータロードスレッドのプライオリティ
#define AKD_RVL_SND_DVD_THRED_DEFAULT_PRIO		(9)

//! サウンドハンドルの最大数
#define AKD_RVL_SND_HANDLE_MAX		(128)
//! エラーサウンドハンドル
#define AKD_RVL_SND_ERROR_HANDLE	(AkRvlSndGetErrorHandle())



/* フラグ */
#define AKD_RVL_SND_FLAG_SYSTEM_INITIALIZED		(1 << 0)
#define AKD_RVL_SND_FLAG_RESERVE_AI_RESET		(1 << 1)
#define AKD_RVL_SND_FLAG_RESERVE_AX_QUIT		(1 << 2)

// デバッグ用フラグ
#if defined(MTD_DEBUG)
//! 各サウンドアーカイブプレイヤー中の全てのサウンドプレイヤーのボリュームがakRvlSndで管理している値と一致するかチェックするアサートを有効にする
#define AKD_RVL_SND_DEBUG_ASSERT_INCOSISTENT_VOL	(1 << 30)
//! サウンドハンドルオーバーアサートを有効にする
#define AKD_RVL_SND_DEBUG_ASSERT_HANDLE_OVER		(1 << 31)

#define AKD_RVL_SND_DEBUG_FLAGS					(AKD_RVL_SND_DEBUG_ASSERT_INCOSISTENT_VOL | \
												 AKD_RVL_SND_DEBUG_ASSERT_HANDLE_OVER)
#endif /* defined(MTD_DEBUG) */

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! サウンドタイプ列挙型
typedef enum
{
	AKE_RVL_SND_TYPE_BGM	= 0,	// BGM
	AKE_RVL_SND_TYPE_SE,			// SE
	
	AKE_RVL_SND_TYPE_MAX
} AKE_RVL_SND_TYPE;

//! サウンドデータロードステート
typedef enum
{
	AKE_RVL_SND_LOAD_STATE_NONE	= 0,	//!< 無効状態
	AKE_RVL_SND_LOAD_STATE_LOADING,		//!< ロード中
	AKE_RVL_SND_LOAD_STATE_COMPLETE,	//!< ロード完了
	
	AKE_RVL_SND_LOAD_STATE_MAX
} AKE_RVL_SND_LOAD_STATE;

//! サウンドデータロードワーク
typedef struct tag_AKS_RVL_SND_LOAD_WORK
{
	AMS_FS					*am_fs;
	AKE_RVL_SND_LOAD_STATE	load_state;
	void	*buf;
} AKS_RVL_SND_LOAD_WORK;

//! サウンドハンドル
typedef nw4r::snd::SoundHandle AKS_RVL_SND_HANDLE;


//! サウンドパート構造体
typedef struct tag_AKS_RVL_SND_PART
{
	//! サウンドアーカイブ
	nw4r::snd::MemorySoundArchive	snd_arc;
	
	//! サウンドアーカイブプレイヤー
	nw4r::snd::SoundArchivePlayer	snd_arc_player;
	
	//! デフォルトハンドル
	AKS_RVL_SND_HANDLE				default_handle;
	
	//! ボリューム 0.0f～2.0f
	Float							volume;
	
	void	*player_buf;	//!< プレイヤーが使用するバッファへのポインタ
	void	*player_strm_buf;	//!< プレイヤーが使用するストリームバッファへのポインタ
} AKS_RVL_SND_PART;


/*------ External Declarations -----------------------------------------*/
// =======================================================================
// AkRvlSndInit
/*!
  サウンドシステム初期化
  
  @param snd_heap_size		[in]	サウンドヒープのサイズ（バイト単位）
  @param snd_thread_prio	[in]	サウンドスレッドのプライオリティ
  									（0:高 ～ 31:低）
  @param load_thread_prio	[in]	DVDからサウンドデータをロードするスレッドのプライオリティ
  
  @note
  サウンドシステムを使用する前に一度だけ呼び出す必要があります。
  ヒープサイズは一度設定すると変更できません。
 */
// =======================================================================
extern void AkRvlSndInit(Uint32 snd_heap_size, Sint32 snd_thread_prio, Sint32 load_thread_prio);

// =======================================================================
// AkRvlSndReset
/*!
  サウンドシステムをデフォルトの状態にリセット
  
  @note
  サウンドシステムをAkRvlSndInit()呼び出し直後の状態に戻します。
 */
// =======================================================================
extern void AkRvlSndReset(void);

// =======================================================================
// AkRvlSndExit
/*!
  サウンドシステム終了
  
  @note
  システムの終了処理やサウンドヒープ等の解放を行います。
 */
// =======================================================================
extern void AkRvlSndExit(void);

// =======================================================================
// AkRvlSndUpdate
/*!
  フレーム処理
  
  @note
  1ゲームフレームに1回呼び出してください。
 */
// =======================================================================
extern void AkRvlSndUpdate(void);

// =======================================================================
// AkRvlSndHeapAlloc
/*!
  サウンドヒープからメモリを確保
 
  @param size	[in]	確保するメモリサイズ
  
  @return 確保したメモリブロックの先頭アドレス
  
  @note
  ここで確保したメモリは、AkRvlSndReset()もしくはAkRvlSndExit()実行時にまとめて解放されます。
 */
// =======================================================================
extern void* AkRvlSndHeapAlloc(Uint32 size);

// =======================================================================
// AkRvlSndStartLoadDataMem
/*!
  データロード開始
  
  @param load_work	[io]	ロードワーク
  @param file_path	[in]	読み込むファイルのパス
  @param prio		[in]	ロード待ちタスクのプライオリティ
  
  @note
  サウンドヒープ上にデータをロードします。
  内部でロード待ちタスクを生成していますので、
  AkRvlSndCheckLoadDataMemComplete()で完了待ちを行ってください。
 */
// =======================================================================
extern void AkRvlSndStartLoadDataMem(AKS_RVL_SND_LOAD_WORK *load_work, const char* file_path, Sint32 prio);

// =======================================================================
// AkRvlSndCheckLoadDataMemComplete
/*!
  データロード完了待ち
  
  @param load_work	[io]	ロードワーク
  
  @retval TRUE	データロード完了
  @retval FALSE	データロード中
  
  @note
  サウンドヒープ上へのデータロードが完了したかチェックします。
 */
// =======================================================================
extern BOOL AkRvlSndCheckLoadDataMemComplete(AKS_RVL_SND_LOAD_WORK *load_work);

// =======================================================================
// AkRvlSndFinalizeLoadDataMem
/*!
  データロード完了処理
  
  @param load_work	[io]	ロードワーク
  
  @note
  指定ロードワークをクリアします。
  AkRvlSndCheckLoadDataMemComplete()がTRUEを返したら呼び出してください。
 */
// =======================================================================
extern void AkRvlSndFinalizeLoadDataMem(AKS_RVL_SND_LOAD_WORK *load_work);

// =======================================================================
// AkRvlSndIsLoadDataMemValid
/*!
  データロード有効チェック
  
  @param load_work	[in]	ロードワーク
  
  @retval TRUE	有効
  @retval FALSE	無効（Finalize済み）
  
  @note
  ロードワークが有効かチェックします。
  AkRvlSndStartLoadDataMem()未実行の時や、
  AkRvlSndFinalizeLoadDataMem()を実行した直後に無効状態になります。
 */
// =======================================================================
extern BOOL AkRvlSndIsLoadDataMemValid(const AKS_RVL_SND_LOAD_WORK *load_work);

// =======================================================================
// AkRvlSndInitArcMem
/*!
  サウンドアーカイブ初期化（メモリ上のサウンドアーカイブデータ使用）
  
  @param snd_type	[in]	サウンドタイプ
  @param data		[in]	サウンドアーカイブデータのアドレス
  
  @note
  AkRvlSndReleaseArcMem()で解放を行います。
  終了時であればAkRvlSndExit()を呼んだ時にも解放されます。
 */
// =======================================================================
extern void AkRvlSndInitArcMem(AKE_RVL_SND_TYPE snd_type, const void *data);

// =======================================================================
// AkRvlSndReleaseArcMem
/*!
  サウンドアーカイブ解放（メモリ上のサウンドアーカイブデータ使用）
  
  @param snd_type	[in]	サウンドタイプ
  
  @note
  解放済みの場合は何もしません。
 */
// =======================================================================
extern void AkRvlSndReleaseArcMem(AKE_RVL_SND_TYPE snd_type);

// =======================================================================
// AkRvlSndGetSoundPart
/*!
  サウンドパート取得
  
  @param snd_type	[in]	サウンドタイプ
 */
// =======================================================================
extern AKS_RVL_SND_PART* AkRvlSndGetSoundPart(AKE_RVL_SND_TYPE snd_type);


//############ 再生関連 #######################################################

// =======================================================================
// AkRvlSndPlayArc
/*!
  サウンドアーカイブのサウンドを再生する（サウンドID指定）
  
  @param snd_type		[in]	サウンドタイプ
  @param handle			[io]	サウンドハンドル（NULL可）
  @param sound_id		[in]	サウンドID
  @param is_override	[in]	デフォルトハンドル停止後再生フラグ
  
  @note
  handleにNULLを指定した場合は、BGM用のデフォルトハンドルが使用されます。
  is_overrideをTRUE指定すると、既にデフォルトハンドルで再生中のBGMがある場合は、
  それを停止してから再生します。
 */
// =======================================================================
extern void AkRvlSndPlayArc(AKE_RVL_SND_TYPE snd_type, AKS_RVL_SND_HANDLE *handle,
							Uint32 sound_id, BOOL is_override=FALSE);

// =======================================================================
// AkRvlSndPlayArc
/*! \overload
  サウンドアーカイブのサウンドを再生する（サウンドラベル指定）
  
  @param snd_type		[in]	サウンドタイプ
  @param handle			[io]	サウンドハンドル（NULL可）
  @param sound_label	[in]	サウンドラベル文字列
  @param is_override	[in]	デフォルトハンドル停止後再生フラグ
  
  @note
  再生するサウンドをサウンドIDでなくラベル文字列で指定します。
  サウンドIDファイルのインクルードが必要なくなりますが、
  文字列をサウンドIDに変換するための実行コストがかかります。
  handleにNULLを指定した場合は、BGM用のデフォルトハンドルが使用されます。
  is_overrideをTRUE指定すると、既にデフォルトハンドルで再生中のBGMがある場合は、
  それを停止してから再生します。
 */
// =======================================================================
extern void AkRvlSndPlayArc(AKE_RVL_SND_TYPE snd_type, AKS_RVL_SND_HANDLE *handle,
							const char *sound_label, BOOL is_override=FALSE);

// =======================================================================
// AkRvlSndPlayArcEx
/*!
  サウンドアーカイブのサウンドを再生する（サウンドID指定, 詳細版）
  
  @param snd_type		[in]	サウンドタイプ
  @param handle			[io]	サウンドハンドル（NULL可）
  @param sound_id		[in]	サウンドID
  @param player_id		[in]	プレイヤーID
  @param prio			[in]	プレイヤープライオリティ（0:低 ～127:高）
  @param is_override	[in]	デフォルトハンドル停止後再生フラグ
  
  @note
  handleにNULLを指定した場合は、BGM用のデフォルトハンドルが使用されます。
  is_overrideをTRUE指定すると、既にデフォルトハンドルで再生中のBGMがある場合は、
  それを停止してから再生します。
 */
// =======================================================================
extern void AkRvlSndPlayArcEx(AKE_RVL_SND_TYPE snd_type, AKS_RVL_SND_HANDLE *handle, Uint32 sound_id,
							  Uint32 player_id, Sint32 prio, BOOL is_override=FALSE);

// =======================================================================
// AkRvlSndPlayArcEx
/*! \overload
  サウンドアーカイブのサウンドを再生する（サウンドラベル指定, 詳細版）
  
  @param snd_type		[in]	サウンドタイプ
  @param handle			[io]	サウンドハンドル（NULL可）
  @param sound_label	[in]	サウンドラベル文字列
  @param player_id		[in]	プレイヤーID
  @param prio			[in]	プレイヤープライオリティ（0:低 ～127:高）
  @param is_override	[in]	デフォルトハンドル停止後再生フラグ
  
  @note
  再生するサウンドをサウンドIDでなくラベル文字列で指定します。
  サウンドIDファイルのインクルードが必要なくなりますが、
  文字列をサウンドIDに変換するための実行コストがかかります。
  handleにNULLを指定した場合は、BGM用のデフォルトハンドルが使用されます。
  is_overrideをTRUE指定すると、既にデフォルトハンドルで再生中のBGMがある場合は、
  それを停止してから再生します。
 */
// =======================================================================
extern void AkRvlSndPlayArcEx(AKE_RVL_SND_TYPE snd_type, AKS_RVL_SND_HANDLE *handle, const char *sound_label,
							  Uint32 player_id, Sint32 prio, BOOL is_override=FALSE);

// =======================================================================
// AkRvlSndPlayArcByInfo
/*!
  サウンドアーカイブのサウンドを再生する（サウンドID指定, 再生パラメータ使用）
  
  @param snd_type		[in]	サウンドタイプ
  @param handle			[io]	サウンドハンドル
  @param sound_id		[in]	サウンドID
  @param start_info		[in]	再生パラメータ(NW4Rリファレンス参照)
  								(NULL可)
  @param is_override	[in]	デフォルトハンドル停止後再生フラグ
  
  @note
  自前で作成した詳細パラメータを指定して再生します。
  start_infoにNULLを指定した場合はサウンドアーカイブで設定された値やデフォルトの値が使用されます。
  handleにNULLを指定した場合は、BGM用のデフォルトハンドルが使用されます。
  is_overrideをTRUE指定すると、既にデフォルトハンドルで再生中のBGMがある場合は、
  それを停止してから再生します。
 */
// =======================================================================
extern void AkRvlSndPlayArcByInfo(AKE_RVL_SND_TYPE snd_type, AKS_RVL_SND_HANDLE *handle, Uint32 sound_id,
								  const nw4r::snd::SoundStartable::StartInfo *start_info, BOOL is_override=FALSE);

// =======================================================================
// AkRvlSndPlayArcByInfo
/*! \overload
  サウンドアーカイブのサウンドを再生する（サウンドラベル指定, 再生パラメータ使用）
  
  @param snd_type		[in]	サウンドタイプ
  @param handle			[io]	サウンドハンドル
  @param sound_label	[in]	サウンドラベル文字列
  @param start_info		[in]	再生パラメータ(NW4Rリファレンス参照)
  								(NULL可)
  @param is_override	[in]	デフォルトハンドル停止後再生フラグ
  
  @note
  自前で作成した詳細パラメータを指定して再生します。
  start_infoにNULLを指定した場合はサウンドアーカイブで設定された値やデフォルトの値が使用されます。
  再生するサウンドをサウンドIDでなくラベル文字列で指定します。
  サウンドIDファイルのインクルードが必要なくなりますが、
  文字列をサウンドIDに変換するための実行コストがかかります。
  handleにNULLを指定した場合は、BGM用のデフォルトハンドルが使用されます。
  is_overrideをTRUE指定すると、既にデフォルトハンドルで再生中のBGMがある場合は、
  それを停止してから再生します。
 */
// =======================================================================
extern void AkRvlSndPlayArcByInfo(AKE_RVL_SND_TYPE snd_type, AKS_RVL_SND_HANDLE *handle, const char *sound_label,
								  const nw4r::snd::SoundStartable::StartInfo *start_info, BOOL is_override=FALSE);

// =======================================================================
// AkRvlSndStopArc
/*!
  サウンドアーカイブのサウンドを停止する
  
  @param snd_type	[in]	サウンドタイプ
  @param fade_frame	[in]	フェードアウトにかけるフレーム数（デフォルト:0）
  
  @note
  指定サウンドタイプのサウンドアーカイブプレイヤーで再生されているサウンドを停止します。
  fade_frameに指定したフレーム数を掛けてフェードアウトしながら停止します。
  fade_frameに0を指定した場合は直ちに停止します。
 */
// =======================================================================
extern void AkRvlSndStopArc(AKE_RVL_SND_TYPE snd_type, Sint32 fade_frame=0);

// =======================================================================
// AkRvlSndPauseArc
/*!
  サウンドアーカイブのサウンドを一時停止する
  
  @param snd_type	[in]	サウンドタイプ
  @param fade_frame	[in]	フェードアウトに掛けるフレーム数（デフォルト:0）
  
  @note
  指定サウンドタイプのサウンドアーカイブプレイヤーで再生されているサウンドを一時停止します。
  fade_frameに指定したフレーム数をかけてフェードアウトしながら一時停止します。
  fade_frameに0を指定した場合は直ちに一時停止します。
 */
// =======================================================================
extern void AkRvlSndPauseArc(AKE_RVL_SND_TYPE snd_type, Sint32 fade_frame=0);

// =======================================================================
// AkRvlSndResumeArc
/*!
  サウンドアーカイブのサウンドを再開する
  
  @param snd_type	[in]	サウンドタイプ
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
  
  @note
  指定サウンドタイプのサウンドアーカイブプレイヤーで再生されているサウンドを
  一時停止状態から再開します。
  fade_frameに指定したフレーム数をかけてフェードインしながら再開します。
  fade_frameに0を指定した場合は直ちに再開します。
 */
// =======================================================================
extern void AkRvlSndResumeArc(AKE_RVL_SND_TYPE snd_type, Sint32 fade_frame=0);

// =======================================================================
// AkRvlSndStopAll
/*!
  全てのサウンドを停止する
  
  @param fade_frame	[in]	フェードアウトにかけるフレーム数（デフォルト:0）
  
  @note
  akRvlSoundで管理している全てのサウンドを停止します。
  fade_frameに指定したフレーム数をかけてフェードアウトしながら停止します。
  fade_frameに0を指定した場合は直ちに停止します。
 */
// =======================================================================
extern void AkRvlSndStopAll(Sint32 fade_frame=0);

// =======================================================================
// AkRvlSndPauseAll
/*!
  全てのサウンドを一時停止する
  
  @param fade_frame	[in]	フェードアウトにかけるフレーム数（デフォルト:0）
  
  @note
  akRvlSoundで管理している全てのサウンドを一時停止します。
  fade_frameに指定したフレーム数をかけてフェードアウトしながら一時停止します。
  fade_frameに0を指定した場合は直ちに一時停止します。
 */
// =======================================================================
extern void AkRvlSndPauseAll(Sint32 fade_frame=0);

// =======================================================================
// AkRvlSndResumeAll
/*!
  全てのサウンドを再開する
  
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
  
  @note
  akRvlSoundで管理している全てのサウンドを一時停止状態から再開します。
  fade_frameに指定したフレーム数を掛けてフェードインしながら再開します。
  fade_frameに0を指定した場合は直ちに再開します。
 */
// =======================================================================
extern void AkRvlSndResumeAll(Sint32 fade_frame=0);

// =======================================================================
// AkRvlSndGetVolume
/*!
  BGMのボリュームを取得
  
  @param snd_type	[in]	サウンドタイプ
  
  @return	ボリューム 0.0f ～ 2.0f
 */
// =======================================================================
extern Float AkRvlSndGetVolume(AKE_RVL_SND_TYPE snd_type);

// =======================================================================
// AkRvlSndSetVolume
/*!
  BGMのボリュームを設定
  
  @param vol	[in]	ボリューム 0.0f ～ 2.0f
 */
// =======================================================================
extern void AkRvlSndSetVolume(AKE_RVL_SND_TYPE snd_type, Float vol);


//############ ハンドル操作 ###################################################

// =======================================================================
// AkRvlSndHandleSetFadeIn
/*!
  フェードイン設定
  
  @param handle		[io]	サウンドハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数
  
  @note
  指定ハンドルに関連付けられているサウンドをフェードインさせます。
  この関数によって再生の開始は行われません。
  AkRvlSndPlay***()を呼んだ直後に呼び出してください。
  どのサウンドタイプでも使用可能です。
 */
// =======================================================================
extern void AkRvlSndHandleSetFadeIn(AKS_RVL_SND_HANDLE *handle, Sint32 fade_frame);

// =======================================================================
// AkRvlSndHandleStop
/*!
  サウンド停止
  
  @param handle		[io]	サウンドハンドル
  @param fade_frame	[in]	フェードアウトにかけるフレーム数（デフォルト:0）
  
  @note
  指定ハンドルに関連付けられているサウンドを停止します。
  fade_frameに指定したフレーム数をかけてフェードアウトしながら停止します。
  fade_frameに0を指定した場合は直ちに停止します。
  どのサウンドタイプでも使用可能です。
 */
// =======================================================================
extern void AkRvlSndHandleStop(AKS_RVL_SND_HANDLE *handle, Sint32 fade_frame=0);

// =======================================================================
// AkRvlSndHandlePause
/*!
  サウンド一時停止
  
  @param handle		[io]	サウンドハンドル
  @param fade_frame	[in]	フェードアウトにかけるフレーム数（デフォルト:0）
  
  @note
  指定ハンドルに関連付けれられているサウンドを一時停止します。
  fade_frameに指定したフレーム数をかけてフェードアウトしながら一時停止します。
  fade_frameに0を指定した場合は直ちに停止します。
  どのサウンドタイプでも使用可能です。
 */
// =======================================================================
extern void AkRvlSndHandlePause(AKS_RVL_SND_HANDLE *handle, Sint32 fade_frame=0);

// =======================================================================
// AkRvlSndHandleResume
/*!
  サウンド再開
  
  @param handle		[io]	サウンドハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
  
  @note
  指定ハンドルに関連付けられているスアンドを一時停止状態から再開します。
  fade_frameに指定したフレーム数をかけてフェードインしながら再開します。
  fade_frameに0を指定した場合は直ちに再開します。
  どのサウンドタイプでも使用可能です。
 */
// =======================================================================
extern void AkRvlSndHandleResume(AKS_RVL_SND_HANDLE *handle, Sint32 fade_frame=0);

// =======================================================================
// AkRvlSndHandleIsPause
/*!
  サウンドが一時停止中かチェック
  
  @param handle	[in]	サウンドハンドル
  
  @retval TRUE	一時停止中
  @retval FALSE	一時停止中ではない
  
  @note
  指定ハンドルに関連付けられているサウンドが一時停止中かどうかチェックします。
  一時停止移行時のフェードアウト中は一時停止中とみなされます。
  再開時のフェードイン中は一時停止中とみなされません。
  どのサウンドタイプでも使用可能です。
 */
// =======================================================================
extern BOOL AkRvlSndHandleIsPause(const AKS_RVL_SND_HANDLE *handle);

// =======================================================================
// AkRvlSndHandleIsCompleteStop
/*!
  サウンドが完全に停止したかチェック
  
  @param handle	[in]	サウンドハンドル
  
  @retval TRUE	完全停止
  @retval FALSE	完全停止ではない
  
  @note
  指定ハンドルに関連付けられているサウンドが完全に停止したかチェックします。
  フェードイン・アウト中は停止したとはみなされません。
  どのサウンドタイプでも利用可能です。
 */
// =======================================================================
extern BOOL AkRvlSndHandleIsCompleteStop(const AKS_RVL_SND_HANDLE *handle);

// =======================================================================
// AkRvlSndHandleSetVolume
/*!
  ハンドルボリューム設定
  
  @param handle	[io]	サウンドハンドル
  
  @note
  指定ハンドルに関連付けられているサウンドのボリュームを設定します。
  どのサウンドタイプでも利用可能です。
  （※ハンドルに設定されているボリューム値の取得については、NW4Rの使用上不可能です。）
 */
// =======================================================================
extern void AkRvlSndHandleSetVolume(AKS_RVL_SND_HANDLE *handle, Float vol);

// =======================================================================
// AkRvlSndHandleSetSeqMute
/*!
  ハンドルに関連付けれらているシーケンスのミュート設定
  
  @param handle		[io]	サウンドハンドル
  @param mute_state	[in]	ミュート状態
  
  @note
  指定ハンドルに関連付けられているシーケンスサウンドをミュート状態に設定、
  またはミュート状態を解除します。シーケンスサウンドでのみ利用可能です。
  ノートオンを抑制することでミュートしています。
  指定したmute_stateに対応する動作はnw4r::snd::SeqSoundHandle::SetTrackMute()に準じます。
  ミュート中はボイスを消費したくない場合などに利用してください。
  MEMO:
   nw4r::snd::SeqMute
  	MUTE_OFF		-> ミュート解除
  	MUTE_NO_STOP	-> 発音中の音は変化なし
  	MUTE_RELEASE	-> 発音中の音をリリース発音後、緩やかに停止
  	MUTE_STOP		-> 発音中の音を直ちに停止
 */
// =======================================================================
extern void AkRvlSndHandleSetSeqMute(AKS_RVL_SND_HANDLE *handle, nw4r::snd::SeqMute mute_state);


//############ サウンドハンドル関連 ###########################################

// =======================================================================
// AkRvlSndInitHandleHeap
/*!
  サウンドハンドルのヒープを初期化
 */
// =======================================================================
extern void AkRvlSndInitHandleHeap(void);

// =======================================================================
// AkRvlSndResetHandleHeap
/*!
  サウンドハンドルのヒープをクリア
  
  @note
  全てのサウンドハンドルを解放し、確保情報をクリアします。
 */
// =======================================================================
extern void AkRvlSndResetHandleHeap(void);

// =======================================================================
// AkRvlSndAllocHandle
/*!
  サウンドハンドルを確保
  
  @return サウンドハンドル
  
  @note
  サウンドハンドルの確保に失敗した場合AKD_RVL_SND_ERROR_HANDLEを返します。
 */
// =======================================================================
extern AKS_RVL_SND_HANDLE* AkRvlSndAllocHandle(void);

// =======================================================================
// AkRvlSndFreeHandle
/*!
  サウンドハンドルを解放
  
  @param handle	[io]	サウンドハンドル
  
  @note
  サウンドの停止処理は行いませんので、
  停止したい場合は別途AkRvlSndHandleStop()を呼んでください。
 */
// =======================================================================
extern void AkRvlSndFreeHandle(AKS_RVL_SND_HANDLE *handle);

// =======================================================================
// AkRvlSndGetErrorHandle
/*!
  エラーハンドル取得
  
  @return エラーハンドル（AKD_RVL_SND_ERROR_HANDLE）
  
  @note
  原則この関数は使用せず、AKD_RVL_SND_ERROR_HANDLEを使用してください。
 */
// =======================================================================
extern AKS_RVL_SND_HANDLE* AkRvlSndGetErrorHandle(void);

// =======================================================================
// AkRvlSndGetFreeHandleNum
/*!
  空きハンドル数を取得する
  
  @return 空きハンドル数
 */
// =======================================================================
extern Uint32 AkRvlSndGetFreeHandleNum(void);

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

#endif /* _WII */

#endif /* AK_RVL_SOUND_H_ */
