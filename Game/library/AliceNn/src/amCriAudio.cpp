// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       amCriAudio.cpp
	@brief      CRI Audio ライブラリ
	@author	    Syuichi Gotou
	@date	    Date: 2009/05/02 
 */
// ================================================================


//----- Include Files --------------------------------------------------
#include "alice.h"

#if AMD_USE_CRIAUDIO

//----- Definitions ----------------------------------------------------

//----- Macros ---------------------------------------------------------

//----- Local Variables -----------------------------------------------

// 固定ヒープ
//static Uint8 _amCriAudio_csbHeap[AMD_CRIAUDIO_CSB_SIZE];
//static Uint8 _amCriAudio_strmHeap[AMD_CRIAUDIO_STREAM_SIZE];
Uint8* _amCriAudio_csbHeap;
Uint8* _amCriAudio_strmHeap;

static AMS_CRIAUDIO_INTERFACE  amCriAudio_global;
AMS_CRIAUDIO_INTERFACE* pAu = &amCriAudio_global;

#if _IPHONE
static CriSmpSoundOutput* amCriAudio_sndout = NULL;
#endif // _IPHONE

//----- Local Functions ------------------------------------------------

CriUint32 _amCriAudio_notify_callback_func(void *obj, CriUint32 nch, CriFloat32 *sample[], CriUint32 nsmpl);

// タスク
void _amCriAudio_taskWaitLoadCueSheet(AMS_TCB *tcbp);
void _amCriAudio_taskWaitBindCPK(AMS_TCB *tcbp);


//----- External Functions ---------------------------------------------
//## External Functions


#if _IPHONE
// ================================================================
/*!
 CRI SoundOutput 作成
 外部からoutput用クラスを作りたい場合に呼ぶこと
 
 @return 実体があるか否か TRUEなら実体あり
 */
// ================================================================
BOOL amCriAudioCreateSmpOutput(void) {
	BOOL flag = FALSE;
	
	// 実体が無ければ作成
	if (!amCriAudio_sndout) {
		CriSmpSoundOutput::Initialize(); 
		amCriAudio_sndout = CriSmpSoundOutput::Create();
	}
	
	// 実体がある場合はフラグ成立
	if (amCriAudio_sndout) {
		flag = TRUE;
	}
	
	return flag;
}
#endif // _IPHONE

// ================================================================
/*!
	CRI Audio システム初期化(amFsInitの後で呼ぶこと)
*/
// ================================================================
#if _WII
void amCriAudioInit(const CriSoundRendererWii::ConfigParameter *config/*=NULL*/)
#else
void amCriAudioInit(const CriSoundRendererBasic::ConfigParameter *config/*=NULL*/)
#endif
{
	CriSint32			fs_wksize;

	memset(pAu, 0, sizeof(AMS_CRIAUDIO_INTERFACE));
	pAu->err = CRIERR_OK;
	
	_amCriAudio_csbHeap = (Uint8*)amMemAllocSystem(AMD_CRIAUDIO_CSB_SIZE);
	_amCriAudio_strmHeap = (Uint8*)amMemAllocSystem(AMD_CRIAUDIO_STREAM_SIZE);

	amAssert(_amCriAudio_csbHeap);
	amAssert(_amCriAudio_strmHeap);

    // CRI File Systemの初期化は amFsInit で行っている
	criFs_InitializeConfiguration(pAu->fs_config);
	criFs_CalculateWorkSize(pAu->fs_config, &fs_wksize);

	// CSB
	pAu->heap[AME_CRIAUDIO_HEAP_CSB] = criHeap_Create(_amCriAudio_csbHeap, AMD_CRIAUDIO_CSB_SIZE);
	pAu->fs_work[AME_CRIAUDIO_HEAP_CSB] =
		criHeap_AllocFix(pAu->heap[AME_CRIAUDIO_HEAP_CSB], fs_wksize, "amCriWork_csb", CRIHEAP_DEFAULT_MEM_ALIGN);

	//	Create CRI Sound Renderer
#if _IPHONE
	CriSoundRendererBasic::ConfigParameter default_config;
	default_config.srate = CRISMP_SOUNDOUTPUT_FREQ;
	default_config.max_voices = 16;
	pAu->sndrndr = CriSoundRendererBasic::Create(pAu->heap[AME_CRIAUDIO_HEAP_CSB],
		(config ? (*config) : default_config), pAu->err);
	pAu->sndrndr->SetWetSendMasterSwitch(OFF, pAu->err);
#elif _WII
	const static CriSoundRendererWii::ConfigParameter	default_config;
	pAu->sndrndr = (CriSoundRenderer*)CriSoundRendererWii::Create(pAu->heap[AME_CRIAUDIO_HEAP_CSB],
																  (config ? (*config) : default_config),
																  pAu->err);
#else
	const static CriSoundRendererBasic::ConfigParameter	default_config;
	pAu->sndrndr = (CriSoundRenderer*)CriSoundRendererBasic::Create(pAu->heap[AME_CRIAUDIO_HEAP_CSB],
																	(config ? (*config) : default_config),
																	pAu->err);
#endif

	//  Initialize & Create Sample Sound Output
#if _IPHONE
	if (amCriAudio_sndout) {
		pAu->sndout = amCriAudio_sndout;
	}
	else {
		CriSmpSoundOutput::Initialize(); 
		pAu->sndout = CriSmpSoundOutput::Create();
	}
#else
	CriSmpSoundOutput::Initialize(); 
	pAu->sndout = CriSmpSoundOutput::Create();
#endif // _IPHONE
	//	Set Callback function when PCM data are required from Sound Library(DirectSound, XAudio,...)
	pAu->sndout->SetNotifyCallback(_amCriAudio_notify_callback_func, (void *)pAu->sndrndr);
	pAu->sndout->Start();

	//	Create CRI Audio Object
	pAu->auobj = CriAuObj::Create(pAu->heap[AME_CRIAUDIO_HEAP_CSB], pAu->sndrndr, "AliceNN", pAu->err);

	// ストリーム
	pAu->heap[AME_CRIAUDIO_HEAP_STREAM] = criHeap_Create(_amCriAudio_strmHeap, AMD_CRIAUDIO_STREAM_SIZE);
	pAu->fs_work[AME_CRIAUDIO_HEAP_STREAM] =
		criHeap_AllocFix(pAu->heap[AME_CRIAUDIO_HEAP_STREAM], fs_wksize, "amCriWork_strm", CRIHEAP_DEFAULT_MEM_ALIGN);

	CriAu::StreamSpecSound stmspec[AME_CRIAUDIO_STRM_MAX];

#if _IPHONE
	stmspec[0].nch = 2;
	stmspec[0].sampling_rate = 22050;
	stmspec[1].nch = 2;
	stmspec[1].sampling_rate = 22050;
	stmspec[2].nch = 1;
	stmspec[2].sampling_rate = 22050;
	stmspec[3].nch = 1;
	stmspec[3].sampling_rate = 22050;
#else
	stmspec[0].nch = 2;
	stmspec[0].sampling_rate = 48000;
	stmspec[1].nch = 2;
	stmspec[1].sampling_rate = 48000;
	stmspec[2].nch = 1;
	stmspec[2].sampling_rate = 48000;
	stmspec[3].nch = 1;
	stmspec[3].sampling_rate = 24000;
#endif

	//	Following 0.0f means no loading the data. The recommendation value is 10.0f.
	Uint32 bufsize = CriAuUtility::CalcStreamingMinimumBufferSize(AME_CRIAUDIO_STRM_MAX, stmspec, 0.0f, pAu->err);

	// 確保サイズが十分でないと、アロケートしない
	if ( AMD_CRIAUDIO_STREAM_SIZE > bufsize)
	{
		CriAu::AllocateStreamingBuffer(pAu->heap[AME_CRIAUDIO_HEAP_STREAM], bufsize);
	}
}

// ================================================================
/*!
	CRI Audio 管理構造体の取得

	@param output [output] CRI Audio 管理構造体へのポインタ
	@note これがあればなんでもできますが、ライブラリとの競合に注意すること
*/
// ================================================================
AMS_CRIAUDIO_INTERFACE* amCriAudioGetGlobal()
{
    amAssert(pAu);
	return pAu;
}

// ================================================================
/*!
	CRI Audio システム終了（amFsExitの前で呼ぶこと）
*/
// ================================================================
void amCriAudioExit()
{
	// ストリーム対応
	pAu->auobj->Stop(CriAuObj::STOP_MODE_IMMEDIATE, pAu->err);
	while ( (pAu->auobj->GetPlaybackStatus(pAu->err) == CriAuObj::PLAYBACK_STATUS_PLAYING) ) {
		CriAuObj::ExecuteMain(pAu->err);
		pAu->sndout->ExecuteMain();
	}
	for (Uint32 i=0; i < AME_CRIAUDIO_STRM_MAX; i++) {
		if ( pAu->auply[i] )
		{
		    pAu->auply[i]->Destroy(pAu->err);
			pAu->auply[i] = NULL;
		}
	}

	// Bind CPK file
	if ( pAu->binder && pAu->bndr_work )
	{
	    criFsBinder_Destroy(pAu->binder);
	    criHeap_Free(pAu->heap[AME_CRIAUDIO_HEAP_STREAM], pAu->bndr_work);
		pAu->binder = NULL;
		pAu->bndr_work = NULL;
	}

	// CueSheet 
	for (int i = 0; i < AME_CRIAUDIO_CSB_MAX; i++)
	{
		if ( pAu->CueSheet[i] )
		{
	        pAu->auobj->DetachCueSheet(pAu->CueSheet[i], pAu->err);
	        pAu->CueSheet[i]->UnloadCueSheetBinaryFile(pAu->err);
	        pAu->CueSheet[i]->Destroy(pAu->err);
			pAu->CueSheet[i] = NULL;
		}
	}

	// CRI Audio Object
	if ( pAu->auobj )
	{
	    pAu->auobj->Destroy(pAu->err);
		pAu->auobj = NULL;
	}

	// サウンド出力
	if ( pAu->sndout )
	{
	    pAu->sndout->Stop();
	    pAu->sndout->SetNotifyCallback(NULL, NULL);
	    pAu->sndout->Destroy();   
	    CriSmpSoundOutput::Finalize(); 
		pAu->sndout = NULL;
		amCriAudio_sndout = NULL;
	}

	CriAu::ReleaseStreamingBuffer(pAu->err);

	// サウンドレンダラ
	if ( pAu->sndrndr )
	{
	    pAu->sndrndr->Destroy(pAu->err);
		pAu->sndrndr = NULL;
	}

	// CRIヒープ(CSB)
	if ( pAu->heap[AME_CRIAUDIO_HEAP_CSB] )
	{
	    criHeap_Free(pAu->heap[AME_CRIAUDIO_HEAP_CSB], pAu->fs_work[AME_CRIAUDIO_HEAP_CSB]);
	    criHeap_Destroy(pAu->heap[AME_CRIAUDIO_HEAP_CSB]);
		pAu->heap[AME_CRIAUDIO_HEAP_CSB] = NULL;
	}

	// CRIヒープ(ストリーム)
	if ( pAu->heap[AME_CRIAUDIO_HEAP_STREAM] )
	{
	    criHeap_Free(pAu->heap[AME_CRIAUDIO_HEAP_STREAM], pAu->fs_work[AME_CRIAUDIO_HEAP_STREAM]);
	    criHeap_Destroy(pAu->heap[AME_CRIAUDIO_HEAP_STREAM]);
		pAu->heap[AME_CRIAUDIO_HEAP_STREAM] = NULL;
	}
	
	amMemFreeSystem(_amCriAudio_csbHeap);
	amMemFreeSystem(_amCriAudio_strmHeap);
}

// ================================================================
/*!
	CueSheet を生成する（内部で読み込み待ちタスクを生成）

	@param	filePath [input]		CueSheetへのファイルパス名
	@param  csbType  [input]		CueSheetの種類（AMD_CRIAUDIO_CSBTYPE）
	@param  prio     [input]		読み込み待ちタスクの優先度
*/
// ================================================================
void amCriAudioCreateCueSheet(char* filePath, Sint32 csbType, Sint32 prio)
{
	amAssert(pAu->heap[AME_CRIAUDIO_HEAP_CSB]);
	amAssert(pAu->auobj);
	amAssert(pAu->CueSheet[csbType] == NULL);
	char		fname[256];
	CriFsBinderHn	binder = *_am_fs_binder_default;

#if AMD_FS_DEVICE_NAME
	if ((_am_fs_device & AMD_DEVICE_ADDNAME) && (binder == NULL)) {
		AMD_STRCPY_S(fname, 256, _am_fs_device_name);
		AMD_STRCAT_S(fname, 256, filePath);
	} else
#endif
	AMD_STRCPY_S(fname, 256, filePath);

    //	Create Cue Sheet
	pAu->CueSheet[csbType] = CriAuCueSheet::Create(pAu->heap[AME_CRIAUDIO_HEAP_CSB], pAu->err);

	//	Load Cue Sheet Binary File to Cue Sheet Object
	pAu->CueSheet[csbType]->StartLoadingCueSheetBinaryFile(binder, fname, pAu->err);

	// CueSheet読み込み待ちタスク生成
	AMS_TCB *tcbp = amTaskMake(_amCriAudio_taskWaitLoadCueSheet, NULL, prio, 0, 0,
		"# LOAD CUESHEET #");
	Sint32		*work = (Sint32 *)amTaskGetWork(tcbp);
	work[0] = csbType;
	amTaskStart(tcbp);
}

// ================================================================
/*!
	CueSheet の登録解除および削除

	@param  csbType  [input]		CueSheetの種類（AMD_CRIAUDIO_CSBTYPE）
*/
// ================================================================
void amCriAudioDestroyCueSheet(Sint32 csbType)
{
	amAssert(pAu->auobj);
	
#if _IPHONE
	pAu->auobj->Stop(CriAuObj::STOP_MODE_IMMEDIATE, pAu->err);
	while (pAu->auobj->GetPlaybackStatus(pAu->err) == CriAuObj::PLAYBACK_STATUS_PLAYING) {
		CriAuObj::ExecuteMain(pAu->err);
		pAu->sndout->ExecuteMain();
	}
	if (csbType == AME_CRIAUDIO_CSB_SYSTEM)
	{
		for (Uint32 i=0; i < AME_CRIAUDIO_STRM_MAX; i++) 
		{
			if ( pAu->auply[i] )
			{
				pAu->auply[i]->Destroy(pAu->err);
				pAu->auply[i] = NULL;
			}
		}
	}
#endif // _IPHONE
	// ストリーム対応
	if ( csbType == AME_CRIAUDIO_CSB_STREAM )
	{
		for (Uint32 i=0; i < AME_CRIAUDIO_STRM_MAX; i++) 
		{
			if ( pAu->auply[i] )
			{
				pAu->auply[i]->Destroy(pAu->err);
				pAu->auply[i] = NULL;
			}
		}

		// Bind CPK file
		if ( pAu->binder && pAu->bndr_work )
		{
			criFsBinder_Destroy(pAu->binder);
			criHeap_Free(pAu->heap[AME_CRIAUDIO_HEAP_STREAM], pAu->bndr_work);
			pAu->binder = NULL;
			pAu->bndr_work = NULL;
		}
	}

    if ( pAu->CueSheet[csbType] )
	{
	    pAu->auobj->DetachCueSheet(pAu->CueSheet[csbType], pAu->err);
	    pAu->CueSheet[csbType]->UnloadCueSheetBinaryFile(pAu->err);
	    pAu->CueSheet[csbType]->Destroy(pAu->err);
		pAu->CueSheet[csbType] = NULL;
	}
}

// ================================================================
/*!
	CPKファイルのバインド

	@param	filePath [input]		CueSheetへのファイルパス名
	@param  prio     [input]		読み込み待ちタスクの優先度
*/
// ================================================================
void amCriAudioBindCPK(char* filePath, Sint32 prio)
{
	// Bind CPK file
	CriSint32	bndr_wksize;
	char		fname[256];
	CriFsBinderHn	binder = *_am_fs_binder_default;

#if AMD_FS_DEVICE_NAME
	if ((_am_fs_device & AMD_DEVICE_ADDNAME) && (binder == NULL)) {
		AMD_STRCPY_S(fname, 256, _am_fs_device_name);
		AMD_STRCAT_S(fname, 256, filePath);
	} else
#endif
	AMD_STRCPY_S(fname, 256, filePath);

	// Create CriFsBinder handle
	criFsBinder_Create(&pAu->binder);

	// Allocate work area for CPK binding
	criFsBinder_GetWorkSizeForBindCpk(binder, fname, &bndr_wksize);
	pAu->bndr_work = criHeap_AllocFix(pAu->heap[AME_CRIAUDIO_HEAP_STREAM], bndr_wksize, "bndr_work", CRIHEAP_DEFAULT_MEM_ALIGN);

	// Bind CPK file
	criFsBinder_BindCpk(pAu->binder, binder, fname, pAu->bndr_work, bndr_wksize, &pAu->binder_id);

	// CPKバインド待ちタスク生成
	AMS_TCB *tcbp = amTaskMake(_amCriAudio_taskWaitBindCPK, NULL, prio, 0, 0,
		"# LOAD CUESHEET #");
	Sint32		*work = (Sint32 *)amTaskGetWork(tcbp);
	work[0] = AME_CRIAUDIO_CSB_STREAM;
	amTaskStart(tcbp);
}

// ================================================================
/*!
	メイン実行（amFsServerで常に呼ばれる）
*/
// ================================================================
void amCriAudioExcuteMain()
{
    //	Update Status and Delete voices which are end of playback
	CriAuObj::ExecuteMain(pAu->err);
	pAu->sndout->ExecuteMain();

	//	Check the status of CRI Audio Object
	if ( pAu->auobj->GetPlaybackStatus(pAu->err) == pAu->auobj->PLAYBACK_STATUS_STOP )
	{

	}

	for (Uint32 i=0; i < AME_CRIAUDIO_STRM_MAX; i++) {
		if ( pAu->auply[i] )
		{
			// SetCue されていない場合は Updata 呼ばない（W06060804:Cue is not set対策）
			CriAuPlayer::Status status = pAu->auply[i]->GetStatus(pAu->err);
			if ( status != CriAuPlayer::STATUS_STOP)
			{
		        pAu->auply[i]->Update(pAu->err);
			}
		}
	}
}

// ================================================================
/*!
	Cueの単純再生
*/
// ================================================================
void amCriAudioPlay(char* CueName)
{
	pAu->auobj->Play(CueName, pAu->err);
}

// ================================================================
/*!
	CueIDによるCueの単純再生
*/
// ================================================================
void amCriAudioPlayById(Uint32 cueId)
{
	pAu->auobj->PlayById(cueId, pAu->err);
}

// ================================================================
/*!
	ストリームCueの単純再生

	@param	Id		[input]		オーディオプレイヤーのＩＤ
	@param  CueName [input]		Cueの名前
*/
// ================================================================
void amCriAudioStrmPlay(Uint32 Id, char* CueName)
{
	amAssert(Id < AME_CRIAUDIO_STRM_MAX);

	pAu->auply[Id]->Stop(pAu->err);
	pAu->auply[Id]->SetCue(CueName, pAu->err);
	pAu->auply[Id]->Play(pAu->err);
}

//----- Local Functions ------------------------------------------------
//## Local Functions

//	Notify Callback Function (from CriSmpSoundOutput)
CriUint32 _amCriAudio_notify_callback_func(void *obj, CriUint32 nch, CriFloat32 *sample[], CriUint32 nsmpl)
{
	CriSoundRenderer* sndrndr=(CriSoundRenderer*)obj;

	CriError err;

#ifdef XPT_TGT_WII
	CRI_NOP(nch);
	CRI_NOP(sample);
	// Execute Audio Frame
	((CriSoundRendererWii*)sndrndr)->ExecuteAudioFrame_WII(err);
#else
	//	Getting PCM Data from CRI Sound Renderer
	//	nsmpl has to be 128*N samples. (N=1,2,3...)
	((CriSoundRendererBasic*)sndrndr)->GetData(nch, nsmpl, sample, err);
#endif

	return nsmpl;
}

/*--- TCB Functions ---------------------------------------------------------*/
//## TCB Functions

// CueSheet 読み込み待ち
void _amCriAudio_taskWaitLoadCueSheet(AMS_TCB *tcbp)
{
	Sint32		*work = (Sint32 *)amTaskGetWork(tcbp);
	Sint32 csbType = work[0];

    // Wait for loading completed
	pAu->loadState[csbType] = pAu->CueSheet[csbType]->GetLoadStatus(pAu->err);
	if ( pAu->loadState[csbType] != CriAuCueSheet::LOAD_STATUS_COMPLETE ) return;

	// ストリーム（BGMなど）の場合はCPKバインド後にAttachCueSheetを呼ぶので、
	// 重複呼び出しを避ける
	if ( csbType != AME_CRIAUDIO_CSB_STREAM )
	{
		//	Attach Cue Sheet to CRI Audio Object
	    pAu->auobj->AttachCueSheet(pAu->CueSheet[csbType], pAu->err);
#if _IPHONE
		// System を BGM として使用
		if (csbType == AME_CRIAUDIO_CSB_SYSTEM) {
			//	Create CRI Audio Player
			for (Uint32 i=0; i < AME_CRIAUDIO_STRM_MAX; i++) {
				pAu->auply[i] = CriAuPlayer::Create(pAu->auobj, pAu->err);
			}
		}
#endif // _IPHONE
	}

	amTaskDelete(tcbp);
}

// CPK バインド待ち
void _amCriAudio_taskWaitBindCPK(AMS_TCB *tcbp)
{
    Sint32		*work = (Sint32 *)amTaskGetWork(tcbp);
	Sint32 csbType = work[0];

	criFsBinder_GetStatus(pAu->binder_id, &pAu->binder_status);
	if ( pAu->loadState[csbType] != CriAuCueSheet::LOAD_STATUS_COMPLETE ) return;
	if (pAu->binder_status != CRIFSBINDER_STATUS_COMPLETE) return;

	//	Set Binder and the folder of streaming files
	pAu->CueSheet[csbType]->SetStreamingBinder(pAu->binder, NULL, pAu->err);

	//	Attach Cue Sheet to CRI Audio Object
	pAu->auobj->AttachCueSheet( pAu->CueSheet[csbType], pAu->err);

	//	Create CRI Audio Player
	for (Uint32 i=0; i < AME_CRIAUDIO_STRM_MAX; i++) {
		pAu->auply[i] = CriAuPlayer::Create(pAu->auobj, pAu->err);
	}

	amTaskDelete(tcbp);
}

#endif // AMD_USE_CRIAUDIO