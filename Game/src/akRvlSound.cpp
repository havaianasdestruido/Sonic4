// =======================================================================
/*!
  @file	akRvlSound.cpp
  @brief NintendoWare for Revolutionサウンド上位ライブラリ

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: akRvlSound.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"


#if _WII	// WIIのみ使用

#include "akMath.h"

#include "akRvlSound.h"

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

//! サウンドシステム管理構造体
typedef struct tag_AKS_RVL_SND_MGR
{
	// 管理用フラグ
	Uint32	flag;
	
	//!< サウンドパート
	AKS_RVL_SND_PART		snd_parts[AKE_RVL_SND_TYPE_MAX];
	
	//! サウンドヒープ
	nw4r::snd::SoundHeap	snd_heap;
	
	void	*heap_mem;		//!< サウンドヒープに使用するメモリ領域へのポインタ
	Uint32	heap_size;		//!< サウンドヒープに使用するメモリ領域のサイズ
} AKS_RVL_SND_MGR;


/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
static inline AKS_RVL_SND_PART* akRvlSndGetSndPart(AKE_RVL_SND_TYPE snd_type);
static inline nw4r::snd::MemorySoundArchive* akRvlSndGetMemSndArchive(AKE_RVL_SND_TYPE snd_type);
static inline nw4r::snd::SoundArchivePlayer* akRvlSndGetSndArcPlayer(AKE_RVL_SND_TYPE snd_type);
static inline nw4r::snd::SoundHeap* akRvlSndGetSndHeap(void);
static inline AKS_RVL_SND_HANDLE* akRvlSndGetDefaultHandle(AKE_RVL_SND_TYPE snd_type);
static void akRvlSndLoadDataProc(AMS_TCB *tcb);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
//! サウンドシステム管理
static AKS_RVL_SND_MGR ak_rvl_snd_mgr;

//! 動的サウンドハンドルの取得に失敗したときのハンドル
static AKS_RVL_SND_HANDLE	ak_rvl_snd_handle_error;

//! 動的サウンドハンドル
static AKS_RVL_SND_HANDLE	ak_rvl_snd_handle_heap[AKD_RVL_SND_HANDLE_MAX];
//! 動的サウンドハンドルの使用済みフラグ（1ハンドルにつき1bit）
static Uint8 ak_rvl_snd_handle_heap_usage_flag[(AKD_RVL_SND_HANDLE_MAX+7)/8]	= {0};

/*------ Global Functions ----------------------------------------------*/

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
void AkRvlSndInit(Uint32 snd_heap_size, Sint32 snd_thread_prio, Sint32 load_thread_prio)
{
	bool	result;
	
	// 初期化済みなら何もしない
	if (ak_rvl_snd_mgr.flag & AKD_RVL_SND_FLAG_SYSTEM_INITIALIZED) {
		return;
	}
	
	MTM_ASSERT((ak_rvl_snd_mgr.flag & ~AKD_RVL_SND_DEBUG_FLAGS) == 0);
	MTM_ASSERT(ak_rvl_snd_mgr.heap_mem == NULL);
	
	// 初期化済み設定
	ak_rvl_snd_mgr.flag	|= AKD_RVL_SND_FLAG_SYSTEM_INITIALIZED;
	
#if defined(MTD_DEBUG)
	// デバッグ設定
	ak_rvl_snd_mgr.flag	|= (AKD_RVL_SND_DEBUG_ASSERT_INCOSISTENT_VOL |
							AKD_RVL_SND_DEBUG_ASSERT_HANDLE_OVER);
#endif /* defined(MTD_DEBUG) */
	
	// オーディオインターフェース初期化
	if (!AICheckInit()) {
		AIInit(NULL);
		ak_rvl_snd_mgr.flag	|= AKD_RVL_SND_FLAG_RESERVE_AI_RESET;
	}
	
	// オーディオライブラリ初期化
	if (!AXIsInit()) {
		AXInit();
		ak_rvl_snd_mgr.flag	|= AKD_RVL_SND_FLAG_RESERVE_AX_QUIT;
	}
	
	// サウンドシステム初期化
	nw4r::snd::SoundSystem::InitSoundSystem(snd_thread_prio,
											load_thread_prio);
	
	// サウンドヒープ構築
	ak_rvl_snd_mgr.heap_size	= snd_heap_size;
	if (snd_heap_size != 0) {
		
		// サウンドヒープ用メモリ確保
		ak_rvl_snd_mgr.heap_mem	= amMemAlloc(snd_heap_size);
		
		result	= akRvlSndGetSndHeap()->Create(ak_rvl_snd_mgr.heap_mem,
										 snd_heap_size);
		MTM_ASSERT(result && "akRvlSound.cpp::AkRvlSndInit() Error! sound heap creation failed\n");
	}
	else {
		akRvlSndGetSndHeap()->Destroy();
	}
	
	// デフォルトのハンドルを初期化
	for (Sint32 i = 0; i < AKE_RVL_SND_TYPE_MAX; ++i) {
		akRvlSndGetDefaultHandle((AKE_RVL_SND_TYPE)i)->DetachSound();
	}
	
	// サウンドハンドルヒープを初期化する
	AkRvlSndInitHandleHeap();
	
	// ボリューム初期化
	for (Sint32 snd_type = 0; snd_type < AKE_RVL_SND_TYPE_MAX; ++snd_type) {
		AkRvlSndSetVolume((AKE_RVL_SND_TYPE)snd_type, 1.f);
	}
	
	// TODO : デバッグ情報表示 (ヒープ確保情報とか)
}

// =======================================================================
// AkRvlSndReset
/*!
  サウンドシステムをデフォルトの状態にリセット
  
  @note
  サウンドシステムをAkRvlSndInit()呼び出し直後の状態に戻します。
 */
// =======================================================================
void AkRvlSndReset(void)
{
	bool	result;
	nw4r::snd::SoundHeap&	snd_heap	= *akRvlSndGetSndHeap();
		
	// ハンドル初期化
	for (Sint32 snd_type = 0; snd_type < AKE_RVL_SND_TYPE_MAX; ++snd_type) {
		akRvlSndGetDefaultHandle((AKE_RVL_SND_TYPE)snd_type)->DetachSound();
	}
	AkRvlSndResetHandleHeap();
	
	// サウンドヒープを構築しなおす
	snd_heap.Clear();
	snd_heap.Destroy();
	result	= snd_heap.Create(ak_rvl_snd_mgr.heap_mem, ak_rvl_snd_mgr.heap_size);
	MTM_ASSERT(result && "akRvlSound.cpp::AkRvlSndReset() Error! sound heap creation failed\n");
	
	// ボリュームリセット
	for (Sint32 snd_type = 0; snd_type < AKE_RVL_SND_TYPE_MAX; ++snd_type) {
		AkRvlSndSetVolume((AKE_RVL_SND_TYPE)snd_type, 1.f);
	}
}

// =======================================================================
// AkRvlSndExit
/*!
  サウンドシステム終了
  
  @note
  システムの終了処理やサウンドヒープ等の解放を行います。
 */
// =======================================================================
void AkRvlSndExit(void)
{
	// サウンドアーカイブ解放（未解放の時のため）
	for (Sint32 i = 0; i < AKE_RVL_SND_TYPE_MAX; ++i) {
		AkRvlSndReleaseArcMem((AKE_RVL_SND_TYPE)i);
	}
	
	// ハンドルヒープをクリア
	AkRvlSndResetHandleHeap();
	
	// デフォルトハンドルを初期化
	for (Sint32 i = 0; i < AKE_RVL_SND_TYPE_MAX; ++i) {
		akRvlSndGetDefaultHandle((AKE_RVL_SND_TYPE)i)->DetachSound();
	}
	
	/* ヒープの破棄 */
	if (akRvlSndGetSndHeap()->IsValid()) {
		akRvlSndGetSndHeap()->Clear();
		akRvlSndGetSndHeap()->Destroy();
	}
	
	/* ヒープ用バッファの解放 */
	if (ak_rvl_snd_mgr.heap_mem) {
		amMemFree(ak_rvl_snd_mgr.heap_mem);
		ak_rvl_snd_mgr.heap_mem	= NULL;
		ak_rvl_snd_mgr.heap_size	= 0;
	}
	
	// サウンドシステム終了
	if (nw4r::snd::SoundSystem::IsInitializedSoundSystem()) {
		nw4r::snd::SoundSystem::ShutdownSoundSystem();
	}
	
	// オーディオライブラリ終了
	if (ak_rvl_snd_mgr.flag & AKD_RVL_SND_FLAG_RESERVE_AX_QUIT) {
		if (AXIsInit()) {
			AXQuit();
		}
		ak_rvl_snd_mgr.flag	&= ~AKD_RVL_SND_FLAG_RESERVE_AX_QUIT;
	}
	
	// オーディオインターフェース終了
	if (ak_rvl_snd_mgr.flag & AKD_RVL_SND_FLAG_RESERVE_AI_RESET) {
		if (AICheckInit()) {
			AIReset();
		}
		ak_rvl_snd_mgr.flag	&= ~AKD_RVL_SND_FLAG_RESERVE_AI_RESET;
	}
	
	// 初期化済みフラグ解除
	ak_rvl_snd_mgr.flag	&= ~AKD_RVL_SND_FLAG_SYSTEM_INITIALIZED;
	
	MTM_ASSERT((ak_rvl_snd_mgr.flag & ~AKD_RVL_SND_DEBUG_FLAGS) == 0);
}

// =======================================================================
// AkRvlSndUpdate
/*!
  フレーム処理
  
  @note
  1ゲームフレームに1回呼び出してください。
 */
// =======================================================================
void AkRvlSndUpdate(void)
{
	for (Sint32 snd_type = 0; snd_type < AKE_RVL_SND_TYPE_MAX; ++snd_type) {
		if (akRvlSndGetSndArcPlayer((AKE_RVL_SND_TYPE)snd_type)->IsAvailable()) {
			akRvlSndGetSndArcPlayer((AKE_RVL_SND_TYPE)snd_type)->Update();
		}
	}
}

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
void* AkRvlSndHeapAlloc(Uint32 size)
{
	nw4r::snd::SoundHeap&	snd_heap	= *akRvlSndGetSndHeap();
	MTM_ASSERT(snd_heap.IsValid());
	return snd_heap.Alloc(size);
}

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
void AkRvlSndStartLoadDataMem(AKS_RVL_SND_LOAD_WORK *load_work, const char* file_path, Sint32 prio)
{
	MTM_ASSERT(load_work);
	MTM_ASSERT(load_work->am_fs == NULL);
	MTM_ASSERT(load_work->load_state == AKE_RVL_SND_LOAD_STATE_NONE);
	MTM_ASSERT(load_work->buf == NULL);
	MTM_ASSERT(file_path);
	
	// ロード開始（通常ヒープ上にメモリ確保・ロードしてから、後でサウンドヒープ上にコピーする）
	amFsSetMallocMode((AMD_FS_MALLOC_TEMP | AMD_FS_MALLOC_MEM2), 1);
	load_work->am_fs	= amFsReadBackground(const_cast<char*>(file_path), NULL, 0);
	amFsSetMallocMode(AMD_FS_MALLOC_NORMAL, 0);
	
	// ステート設定
	load_work->load_state	= AKE_RVL_SND_LOAD_STATE_LOADING;
	
	// バッファポインタクリア
	load_work->buf	= NULL;
	
	// ロード待ちタスク生成
	AMS_TCB *tcb	= amTaskMake(akRvlSndLoadDataProc, NULL, (Uint32)prio, 0, 0,
								 "# LOAD RVL SND DATA #");
	AKS_RVL_SND_LOAD_WORK	**work	= (AKS_RVL_SND_LOAD_WORK**)amTaskGetWork(tcb);
	work[0]	= load_work;
	amTaskStart(tcb);
}

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
BOOL AkRvlSndCheckLoadDataMemComplete(AKS_RVL_SND_LOAD_WORK *load_work)
{
	if (load_work->load_state == AKE_RVL_SND_LOAD_STATE_COMPLETE) {
		MTM_ASSERT(load_work->am_fs == NULL);
		return TRUE;
	}
	
	return FALSE;
}

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
void AkRvlSndFinalizeLoadDataMem(AKS_RVL_SND_LOAD_WORK *load_work)
{
	MTM_ASSERT(load_work);
	amZeroMemory(load_work, sizeof(AKS_RVL_SND_LOAD_WORK));
}

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
BOOL AkRvlSndIsLoadDataMemValid(const AKS_RVL_SND_LOAD_WORK *load_work)
{
	MTM_ASSERT(load_work);
	
	if (load_work->load_state == AKE_RVL_SND_LOAD_STATE_NONE) {
		return FALSE;
	}
	
	return TRUE;
}

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
void AkRvlSndInitArcMem(AKE_RVL_SND_TYPE snd_type, const void *data)
{
	bool	result	= false;
	
	AKS_RVL_SND_PART&	snd_part	= *akRvlSndGetSndPart(snd_type);
	
	MTM_ASSERT(snd_part.player_buf == NULL);
	MTM_ASSERT(snd_part.player_strm_buf == NULL);
	
	// サウンドアーカイブ初期化
	result	= akRvlSndGetMemSndArchive(snd_type)->Setup(data);
	
	MTM_ASSERT(result && "akRvlSound.cpp::AkRvlSndInitArcMem() Error! sound archive setup failed\n");
	
	// サウンドアーカイブプレイヤー初期化
	{
		Uint32	setup_size;
		Uint32	setup_strm_buf_size;
		
		// バッファ確保
		setup_size	= akRvlSndGetSndArcPlayer(snd_type)->GetRequiredMemSize(akRvlSndGetMemSndArchive(snd_type));
		if (setup_size > 0) {
			snd_part.player_buf	= amMemAlloc(setup_size);
		}
		else {
			snd_part.player_buf	= NULL;
		}
		
		// ストリームバッファ確保
		setup_strm_buf_size	= akRvlSndGetSndArcPlayer(snd_type)->GetRequiredStrmBufferSize(akRvlSndGetMemSndArchive(snd_type));
		if (setup_strm_buf_size > 0) {
			snd_part.player_strm_buf	= amMemAlloc(setup_strm_buf_size);
		}
		else {
			snd_part.player_strm_buf	= NULL;
		}
		
		result	= akRvlSndGetSndArcPlayer(snd_type)->Setup(akRvlSndGetMemSndArchive(snd_type),
														   snd_part.player_buf,
														   setup_size,
														   snd_part.player_strm_buf,
														   setup_strm_buf_size);
	}
	
	MTM_ASSERT(result && "akRvlSound.cpp::AkRvlSndInitArcMem() Error! sound arc player setup failed\n");
	
#if defined(MTD_DEBUG)
	// ボリューム初期化時にボリューム値整合性チェックを行わないようにする
	Uint32	saved_debug_flag	= AKD_RVL_SND_DEBUG_ASSERT_INCOSISTENT_VOL & ak_rvl_snd_mgr.flag;
	ak_rvl_snd_mgr.flag	&= ~AKD_RVL_SND_DEBUG_ASSERT_INCOSISTENT_VOL;
#endif /* defined(MTD_DEBUG) */
	
	// ボリューム初期化
	// （SoundArchivePlayer内の各SoundPlayerのボリューム値を
	//   ak_rvl_snd_mgrで管理してるボリューム値と合わせておくために、念のため。
	AkRvlSndSetVolume(snd_type, AkRvlSndGetVolume(snd_type));
	
#if defined(MTD_DEBUG)
	// ボリューム値整合性チェックの設定を復帰
	ak_rvl_snd_mgr.flag	|= saved_debug_flag;
#endif /* defined(MTD_DEBUG) */
	
//	akRvlSndGetSndArcPlayer(snd_type)->LoadGroup((Uint32)0, akRvlSndGetSndHeap());
	
//	MTM_ASSERT(akRvlSndGetSndArcPlayer(snd_type)->IsLoadedGroup(0));
//	MTM_ASSERT(akRvlSndGetSndArcPlayer(snd_type)->IsLoadedGroup(1));
}


// =======================================================================
// AkRvlSndReleaseArcMem
/*!
  サウンドアーカイブ解放（メモリ上のサウンドアーカイブデータ使用）
  
  @param snd_type	[in]	サウンドタイプ
  
  @note
  解放済みの場合は何もしません。
 */
// =======================================================================
void AkRvlSndReleaseArcMem(AKE_RVL_SND_TYPE snd_type)
{
	AKS_RVL_SND_PART&	snd_part	= *akRvlSndGetSndPart(snd_type);
	
	// プレイヤー終了
	if (akRvlSndGetSndArcPlayer(snd_type)->IsAvailable()) {
		akRvlSndGetSndArcPlayer(snd_type)->Shutdown();
	}
	
	/* プレイヤーで使用したバッファの解放 */
	if (snd_part.player_strm_buf) {
		amMemFree(snd_part.player_strm_buf);
		snd_part.player_strm_buf	= NULL;
	}
	
	if (snd_part.player_buf) {
		amMemFree(snd_part.player_buf);
		snd_part.player_buf	= NULL;
	}
	
	if (akRvlSndGetMemSndArchive(snd_type)->IsAvailable()) {
		// サウンドアーカイブ終了
		akRvlSndGetMemSndArchive(snd_type)->Shutdown();
	}
}

// =======================================================================
// AkRvlSndGetSoundPart
/*!
  サウンドパート取得
  
  @param snd_type	[in]	サウンドタイプ
 */
// =======================================================================
AKS_RVL_SND_PART* AkRvlSndGetSoundPart(AKE_RVL_SND_TYPE snd_type)
{
	return akRvlSndGetSndPart(snd_type);
}


// ############################################################################
// 再生関連
// ############################################################################

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
void AkRvlSndPlayArc(AKE_RVL_SND_TYPE snd_type, AKS_RVL_SND_HANDLE *handle,
					 Uint32 sound_id, BOOL is_override/*=FALSE*/)
{
	AkRvlSndPlayArcByInfo(snd_type, handle, sound_id, NULL, is_override);
}

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
void AkRvlSndPlayArc(AKE_RVL_SND_TYPE snd_type, AKS_RVL_SND_HANDLE *handle,
					 const char *sound_label, BOOL is_override/*=FALSE*/)
{
	AkRvlSndPlayArcByInfo(snd_type, handle, sound_label, NULL, is_override);
}

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
void AkRvlSndPlayArcEx(AKE_RVL_SND_TYPE snd_type, AKS_RVL_SND_HANDLE *handle, Uint32 sound_id,
					   Uint32 player_id, Sint32 prio, BOOL is_override/*=FALSE*/)
{
	nw4r::snd::SoundStartable::StartInfo	start_info;
	
	MTM_ASSERT(start_info.enableFlag == 0);
	
	// プレイヤーID設定
	start_info.playerId	= player_id;
	start_info.enableFlag	|= nw4r::snd::SoundStartable::StartInfo::ENABLE_PLAYER_ID;
	
	// プレイヤープライオリティ設定
	start_info.playerPriority	= prio;
	start_info.enableFlag	|= nw4r::snd::SoundStartable::StartInfo::ENABLE_PLAYER_PRIORITY;
	
	// 再生パラメータを指定して再生処理呼び出し
	AkRvlSndPlayArcByInfo(snd_type, handle, sound_id, &start_info, is_override);
}

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
void AkRvlSndPlayArcEx(AKE_RVL_SND_TYPE snd_type, AKS_RVL_SND_HANDLE *handle, const char *sound_label,
					   Uint32 player_id, Sint32 prio, BOOL is_override/*=FALSE*/)
{
	const nw4r::snd::SoundArchive&	snd_arc	= akRvlSndGetSndArcPlayer(snd_type)->GetSoundArchive();
	
	// サウンドラベルをサウンドIDに変換して再生処理呼び出し
	AkRvlSndPlayArcEx(snd_type,
					  handle,
					  snd_arc.ConvertLabelStringToSoundId(sound_label),
					  player_id, prio,
					  is_override);
}

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
void AkRvlSndPlayArcByInfo(AKE_RVL_SND_TYPE snd_type, AKS_RVL_SND_HANDLE *handle, Uint32 sound_id,
						   const nw4r::snd::SoundStartable::StartInfo *start_info, BOOL is_override/*=FALSE*/)
{
	AKS_RVL_SND_HANDLE	*default_handle	= akRvlSndGetDefaultHandle(snd_type);
	
	MTM_ASSERT(nw4r::snd::SoundArchive::INVALID_ID != sound_id
			   && "akRvlSound.cpp::AkRvlSndPlayArcByInfo() Error! invalid sound id\n");
	
	if (NULL == handle) {
		handle	= default_handle;
	}
	
	// 上書きONの場合はデフォルトハンドルでの再生を停止してから再生する
	if (is_override) {
		// デフォルトハンドルで再生されているサウンドを停止する
		if (default_handle->IsAttachedSound()) {
			default_handle->Stop(0);
			default_handle->DetachSound();
		}
	}
	
	// ユーザが指定したハンドルを解放する
	if (handle != default_handle) {
		handle->DetachSound();
	}
	
	/* 再生 */
	if (start_info != NULL) {
		// 再生パラメータ使用
		akRvlSndGetSndArcPlayer(snd_type)->PrepareSound(handle, sound_id, *start_info);
	}
	else {
		// 再生パラメータ不使用
		akRvlSndGetSndArcPlayer(snd_type)->PrepareSound(handle, sound_id);
	}
	
	handle->StartPrepared();	// 再生開始
}

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
void AkRvlSndPlayArcByInfo(AKE_RVL_SND_TYPE snd_type, AKS_RVL_SND_HANDLE *handle, const char *sound_label,
						   const nw4r::snd::SoundStartable::StartInfo *start_info, BOOL is_override/*=FALSE*/)
{
	const nw4r::snd::SoundArchive&	snd_arc	= akRvlSndGetSndArcPlayer(snd_type)->GetSoundArchive();
	
	// サウンドラベルをサウンドIDに変換して再生処理呼び出し
	AkRvlSndPlayArcByInfo(snd_type,
						  handle,
						  snd_arc.ConvertLabelStringToSoundId(sound_label),
						  start_info,
						  is_override);
}


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
void AkRvlSndStopArc(AKE_RVL_SND_TYPE snd_type, Sint32 fade_frame/*=0*/)
{
	nw4r::snd::SoundArchivePlayer&	snd_arc_player	= *akRvlSndGetSndArcPlayer(snd_type);
	
	if (!snd_arc_player.IsAvailable()) {
		return;
	}
	
	Uint32	ply_cnt	= snd_arc_player.GetSoundPlayerCount();
	for (Uint32 i = 0; i < ply_cnt; ++i) {
		snd_arc_player.GetSoundPlayer(i).StopAllSound(fade_frame);
	}
}

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
void AkRvlSndPauseArc(AKE_RVL_SND_TYPE snd_type, Sint32 fade_frame/*=0*/)
{
	nw4r::snd::SoundArchivePlayer&	snd_arc_player	= *akRvlSndGetSndArcPlayer(snd_type);
	
	if (!snd_arc_player.IsAvailable()) {
		return;
	}
	
	Uint32	ply_cnt	= snd_arc_player.GetSoundPlayerCount();
	for (Uint32 i = 0; i < ply_cnt; ++i) {
		snd_arc_player.GetSoundPlayer(i).PauseAllSound(true, fade_frame);
	}
}

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
void AkRvlSndResumeArc(AKE_RVL_SND_TYPE snd_type, Sint32 fade_frame/*=0*/)
{
	nw4r::snd::SoundArchivePlayer&	snd_arc_player	= *akRvlSndGetSndArcPlayer(snd_type);
	
	if (!snd_arc_player.IsAvailable()) {
		return;
	}
	
	Uint32	ply_cnt	= snd_arc_player.GetSoundPlayerCount();
	for (Uint32 i = 0; i < ply_cnt; ++i) {
		snd_arc_player.GetSoundPlayer(i).PauseAllSound(false, fade_frame);
	}
}

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
void AkRvlSndStopAll(Sint32 fade_frame/*=0*/)
{
	for (Sint32 snd_type = 0; snd_type < AKE_RVL_SND_TYPE_MAX; ++snd_type) {
		AkRvlSndStopArc((AKE_RVL_SND_TYPE)snd_type, fade_frame);
	}
}

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
void AkRvlSndPauseAll(Sint32 fade_frame/*=0*/)
{
	for (Sint32 snd_type = 0; snd_type < AKE_RVL_SND_TYPE_MAX; ++snd_type) {
		AkRvlSndPauseArc((AKE_RVL_SND_TYPE)snd_type, fade_frame);
	}
}

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
void AkRvlSndResumeAll(Sint32 fade_frame/*=0*/)
{
	for (Sint32 snd_type = 0; snd_type < AKE_RVL_SND_TYPE_MAX; ++snd_type) {
		AkRvlSndResumeArc((AKE_RVL_SND_TYPE)snd_type, fade_frame);
	}
}

// =======================================================================
// AkRvlSndGetVolume
/*!
  BGMのボリュームを取得
  
  @param snd_type	[in]	サウンドタイプ
  
  @return	ボリューム 0.0f ～ 2.0f
 */
// =======================================================================
Float AkRvlSndGetVolume(AKE_RVL_SND_TYPE snd_type)
{
#if defined(MTD_DEBUG)
	// 全てのプレイヤーのボリューム値がak_rvl_snd_mgrで管理しているボリュームと一致しているかチェック
	if (AKD_RVL_SND_DEBUG_ASSERT_INCOSISTENT_VOL & ak_rvl_snd_mgr.flag) {
		nw4r::snd::SoundArchivePlayer& snd_arc_player	= *akRvlSndGetSndArcPlayer(snd_type);
		
		Uint32	ply_cnt	= snd_arc_player.GetSoundPlayerCount();
		for (Uint32 i = 0; i < ply_cnt; ++i) {
			if (akRvlSndGetSndPart(snd_type)->volume != snd_arc_player.GetSoundPlayer(i).GetVolume()) {
				
				MTM_ASSERT(!"akRvlSound.cpp::AkRvlSndGetVolume() Error! Inconsistent volume value detected\n");
			}
		}
	}
#endif /* defined(MTD_DEBUG) */
	
	return akRvlSndGetSndPart(snd_type)->volume;
}

// =======================================================================
// AkRvlSndSetVolume
/*!
  BGMのボリュームを設定
  
  @param vol	[in]	ボリューム 0.0f ～ 2.0f
 */
// =======================================================================
void AkRvlSndSetVolume(AKE_RVL_SND_TYPE snd_type, Float vol)
{
	nw4r::snd::SoundArchivePlayer& snd_arc_player	= *akRvlSndGetSndArcPlayer(snd_type);
	
	MTM_ASSERT(vol >= 0.f && vol <= 2.f);
	
	// パートのボリュームを保存しておく
	akRvlSndGetSndPart(snd_type)->volume	= vol;
	
	// サウンドアーカイブプレイヤー中の全てのプレイヤーのボリュームを設定する。
	Uint32	ply_cnt	= snd_arc_player.GetSoundPlayerCount();
	for (Uint32 i = 0; i < ply_cnt; ++i) {
		snd_arc_player.GetSoundPlayer(i).SetVolume(vol);
	}
}


// ############################################################################
// ハンドル操作
// ############################################################################

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
void AkRvlSndHandleSetFadeIn(AKS_RVL_SND_HANDLE *handle, Sint32 fade_frame)
{
	MTM_ASSERT(handle);
	
	// フェードインを設定
	handle->FadeIn(fade_frame);
}

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
void AkRvlSndHandleStop(AKS_RVL_SND_HANDLE *handle, Sint32 fade_frame/*=0*/)
{
	MTM_ASSERT(handle);
	
	// 停止
	handle->Stop(fade_frame);
}

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
void AkRvlSndHandlePause(AKS_RVL_SND_HANDLE *handle, Sint32 fade_frame/*=0*/)
{
	MTM_ASSERT(handle);
	
	// 一時停止
	handle->Pause(true, fade_frame);
}

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
void AkRvlSndHandleResume(AKS_RVL_SND_HANDLE *handle, Sint32 fade_frame/*=0*/)
{
	MTM_ASSERT(handle);
	
	// 再開
	handle->Pause(false, fade_frame);
}

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
BOOL AkRvlSndHandleIsPause(const AKS_RVL_SND_HANDLE *handle)
{
	MTM_ASSERT(handle);
	
	// ポーズチェック
	if (handle->IsPause()) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

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
BOOL AkRvlSndHandleIsCompleteStop(const AKS_RVL_SND_HANDLE *handle)
{
	MTM_ASSERT(handle);
	
	// 停止チェック
	if (!handle->IsAttachedSound()) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}


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
void AkRvlSndHandleSetVolume(AKS_RVL_SND_HANDLE *handle, Float vol)
{
	MTM_ASSERT(handle);
	
	handle->SetVolume(vol);
}

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
void AkRvlSndHandleSetSeqMute(AKS_RVL_SND_HANDLE *handle, nw4r::snd::SeqMute mute_state)
{
	nw4r::snd::SeqSoundHandle	seq_snd_handle(handle);
	// ↑引数がシーケンスサウンド以外の場合はデフォルトコンストラクタと同じ挙動になる
	
	if (!seq_snd_handle.IsAttachedSound()) {
		// シーケンスサウンド以外が指定された場合は何もしない
		return;
	}
	
	// 全トラックに対してミュート状態設定
	seq_snd_handle.SetTrackMute((Uint32)0xffffffff, mute_state);
}



// ############################################################################
// サウンドハンドル関連
// ############################################################################

// =======================================================================
// AkRvlSndInitHandleHeap
/*!
  サウンドハンドルのヒープを初期化
 */
// =======================================================================
void AkRvlSndInitHandleHeap(void)
{
	AkRvlSndResetHandleHeap();
}

// =======================================================================
// AkRvlSndResetHandleHeap
/*!
  サウンドハンドルのヒープをクリア
  
  @note
  全てのサウンドハンドルを解放し、確保情報をクリアします。
 */
// =======================================================================
void AkRvlSndResetHandleHeap(void)
{
	for (Sint32 i = 0; i < AKD_RVL_SND_HANDLE_MAX; ++i) {
		ak_rvl_snd_handle_heap[i].DetachSound();
	}
	AKD_RVL_SND_ERROR_HANDLE->DetachSound();
	amZeroMemory(ak_rvl_snd_handle_heap_usage_flag, sizeof(ak_rvl_snd_handle_heap_usage_flag));
}

// =======================================================================
// AkRvlSndAllocHandle
/*!
  サウンドハンドルを確保
  
  @return サウンドハンドル
  
  @note
  サウンドハンドルの確保に失敗した場合AKD_RVL_SND_ERROR_HANDLEを返します。
 */
// =======================================================================
AKS_RVL_SND_HANDLE* AkRvlSndAllocHandle(void)
{
	// 空いているハンドルを探す
	for (Sint32 i = 0; i < AKD_RVL_SND_HANDLE_MAX; ++i) {
		if (!(ak_rvl_snd_handle_heap_usage_flag[i>>3] & (1 << (i&0x07)))) {
			ak_rvl_snd_handle_heap_usage_flag[i>>3]	|= 1 << (i&0x07);
			
#if defined(MTD_DEBUG)
			OS_TPrintf("Sound handle remain : %d\n", AkRvlSndGetFreeHandleNum());
#endif /* defined(MTD_DEBUG) */
			
			MTM_ASSERT(!ak_rvl_snd_handle_heap[i].IsAttachedSound());
			ak_rvl_snd_handle_heap[i].DetachSound();
			return &ak_rvl_snd_handle_heap[i];
		}
	}
	
#if defined(MTD_DEBUG)
	if (AKD_RVL_SND_DEBUG_ASSERT_HANDLE_OVER & ak_rvl_snd_mgr.flag) {
		MTM_ASSERT(!"akRvlSound.cpp::AkRvlSndAllocHandle() Error! Exceed sound handle limit\n");
	}
#endif /* defined(MTD_DEBUG) */
	
	// 空いているハンドルがなかった
	AKD_RVL_SND_ERROR_HANDLE->DetachSound();
	return AKD_RVL_SND_ERROR_HANDLE;
}

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
void AkRvlSndFreeHandle(AKS_RVL_SND_HANDLE *handle)
{
	if (AKD_RVL_SND_ERROR_HANDLE == handle) {
		handle->DetachSound();
		return;
	}
	
	// アドレスの一致する場所を探す
	for (Sint32 i = 0; i < AKD_RVL_SND_HANDLE_MAX; ++i) {
		if (&ak_rvl_snd_handle_heap[i] == handle) {
			// 二重解放チェック
			MTM_ASSERT(ak_rvl_snd_handle_heap_usage_flag[i>>3] & (1<<(i&0x07)));
			
			handle->DetachSound();
			ak_rvl_snd_handle_heap_usage_flag[i>>3]	&= ~(1 << (i&0x07));
#if defined(MTD_DEBUG)
			OS_TPrintf("Sound handle remain : %d\n", AkRvlSndGetFreeHandleNum());
#endif /* defined(MTD_DEBUG) */
			return;
		}
	}
	
	// AkRvlSndAllocHandle()で取得したハンドルではなかった
	MTM_ASSERT(!"akRvlSound.ccp::AkRvlSndFreeHandle() Error! Invalid handle specified");
}

// =======================================================================
// AkRvlSndGetErrorHandle
/*!
  エラーハンドル取得
  
  @return エラーハンドル（AKD_RVL_SND_ERROR_HANDLE）
  
  @note
  原則この関数は使用せず、AKD_RVL_SND_ERROR_HANDLEを使用してください。
 */
// =======================================================================
AKS_RVL_SND_HANDLE* AkRvlSndGetErrorHandle(void)
{
	return &ak_rvl_snd_handle_error;
}

// =======================================================================
// AkRvlSndGetFreeHandleNum
/*!
  空きハンドル数を取得する
  
  @return 空きハンドル数
 */
// =======================================================================
Uint32 AkRvlSndGetFreeHandleNum(void)
{
	Uint32	num	= AKD_RVL_SND_HANDLE_MAX;
	const static Sint32	elem_num	= (AKD_RVL_SND_HANDLE_MAX+7)/8;
	
	// 使用済みのハンドル数分減算していく
	for (Sint32 i = 0; i < elem_num; ++i) {
		num	-= AkMathCountBitPopulation(ak_rvl_snd_handle_heap_usage_flag[i]);
		MTM_ASSERT(num <= AKD_RVL_SND_HANDLE_MAX);
	}
	
	return num;
}


/*------ Static Functions ----------------------------------------------*/
// =======================================================================
// akRvlSndGetSndPart
/*!
  サウンドパート取得
  
  @param snd_type	[in]	サウンドタイプ
  
  @return サウンドパート
 */
// =======================================================================
inline AKS_RVL_SND_PART* akRvlSndGetSndPart(AKE_RVL_SND_TYPE snd_type)
{
	return &ak_rvl_snd_mgr.snd_parts[snd_type];
}

// =======================================================================
// akRvlSndGetMemSndArchive
/*!
  メモリサウンドアーカイブ取得
  
  @param snd_type	[in]	サウンドタイプ
  
  @return メモリサウンドアーカイブへのポインタ
 */
// =======================================================================
inline nw4r::snd::MemorySoundArchive* akRvlSndGetMemSndArchive(AKE_RVL_SND_TYPE snd_type)
{
	return &akRvlSndGetSndPart(snd_type)->snd_arc;
}

// =======================================================================
// akRvlSndGetSndArcPlayer
/*!
  サウンドアーカイブプレイヤー取得
  
  @param snd_type	[in]	サウンドタイプ
  
  @return サウンドアーカイブプレイヤー
 */
// =======================================================================
inline nw4r::snd::SoundArchivePlayer* akRvlSndGetSndArcPlayer(AKE_RVL_SND_TYPE snd_type)
{
	return &akRvlSndGetSndPart(snd_type)->snd_arc_player;
}

// =======================================================================
// akRvlSndGetSndHeap
/*!
  サウンドヒープ取得
  
  @return サウンドヒープ
 */
// =======================================================================
inline nw4r::snd::SoundHeap* akRvlSndGetSndHeap(void)
{
	return &ak_rvl_snd_mgr.snd_heap;
}

// =======================================================================
// akRvlSndGetDefaultHandle
/*!
  デフォルトハンドル取得
  
  @param snd_type	[in]	サウンドタイプ
  
  @return デフォルトサウンドハンドル
 */
// =======================================================================
inline AKS_RVL_SND_HANDLE* akRvlSndGetDefaultHandle(AKE_RVL_SND_TYPE snd_type)
{
	return &akRvlSndGetSndPart(snd_type)->default_handle;
}


// =======================================================================
// akRvlSndLoadDataProc
/*!
  サウンドデータロードプロシージャ
  
  @param tcb	[io]	TCB
 */
// =======================================================================
void akRvlSndLoadDataProc(AMS_TCB *tcb)
{
	AKS_RVL_SND_LOAD_WORK	*load_work;
	
	load_work	= *((AKS_RVL_SND_LOAD_WORK**)amTaskGetWork(tcb));
	
	// 完了チェック
	if (amFsIsComplete(load_work->am_fs)) {
		Uint32	data_size	= (Uint32)load_work->am_fs->length;
		
		// サウンドヒープからメモリを確保
		load_work->buf	= AkRvlSndHeapAlloc(data_size);
		
		// サウンドヒープ上のメモリブロックにコピー
		amCopyMemory(load_work->buf, load_work->am_fs->buf, data_size);
		
		// 読み込みリクエストクリア（バッファ自動的に解放）
		amFsClearRequest(load_work->am_fs);
		load_work->am_fs	= NULL;
		
		// ステート設定
		load_work->load_state	= AKE_RVL_SND_LOAD_STATE_COMPLETE;
		
		amTaskDelete(tcb);
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


#endif /* _WII */
