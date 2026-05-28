/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2009 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Sound Output for AudioUnit
 * File     : CriSmpSoundOuput_AudioUnit.cpp
 * Date     : 2009-09-10
 * Version  : 1.02
 *
 ****************************************************************************/
/*
 * サウンドレンダラ処理の分離
 *
 * 下記のシンボル SSO_USE_SOUNDPOOL を有効にした場合、
 * AudioUnitコールバック関数内の処理から、サウンドレンダラによるサウンドデータ生成
 * 処理を分離し、代わりにサウンドレンダラ処理を CriSmpSoundOutput;;ExecuteMain()で
 * 行うようにします。
 *
 * これにより、AudioUnitコールバック関数内から負荷の高い処理を逃がすことになり、
 * AudioUnitコールバック関数では、用意されたデータの転送のみが行われるようになります。
 *
 * AudioUnitコールバック関数で転送するデータを用意するために、タイマーイベントなどで
 * CriSmpSoundOutput;;ExecuteMain()を定期的(1/60sec間隔)に呼び出すようにしてください
 * (転送するデータが用意できないと、出力音声が途切れます）。
 */
#define SSO_USE_SOUNDPOOL


/****************************************************************************
 * インクルードファイル
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <AudioToolbox/AudioToolbox.h>
//#define DEBUG
#include "CriSmpSoundOutput_iPhone.h"

/****************************************************************************
 * 処理マクロ
 ***************************************************************************/
/* バージョン文字列へのポインタの定義
 *   最適化耐性強化に使用する。
 *   staticなので固有の名前は準備せず、どのソースでも同じ変数名を使っている。
 */
static const char * volatile cri_verstr_ptr;

/* バージョン文字列の定義 [ */
static const char CRI_VERSTR_ARRAY_NAME[] = {
/* バージョン表示ツールのデフォルトの表示対象文字列 */
"\n"
"CriSmpSoundOutput_AudioUnit"
"/"
"iPhone OS"
" Ver.1.02"
" Build:"__DATE__" "__TIME__"\n"
"\0"
};


/****************************************************************************
 * データ型の宣言
 ***************************************************************************/
/***
 * サウンドレンダラからの出力は
 *   48000サンプル／秒
 */
class CriSmpSoundOutputLoc : public CriSmpSoundOutput
{
 public:
	// Critical Section
	struct Crs {
		pthread_mutex_t mutex;		// ミューテックス
		pthread_t tid;				// スレッドID
		Sint32 level;				// 多重呼び出しレベル
	};
	//
	static CriSmpSoundOutputLoc* Create(void);
	virtual void Destroy(void);
	virtual void SetNotifyCallback(Uint32 (*func)(void *obj, Uint32 uch, Float32 *sample[], Uint32 nsmpl), void *obj);
	virtual void Start(void);
	virtual void Stop(void);
	virtual void ExecuteMain(void);
	virtual void SetFrequency(Uint32 frequency);
	//
	void start(void);
	void stop(void);

	void initAudioFrame(void);
	void executeAudioFrame(UInt32 inNumberFrames, Sint16 *odata);

	// AudioUnit
	Bool openAudioUnit(void);
	void closeAudioUnit(void);
		
	// CriticalSection
	Bool createCrs(void);
	void destroyCrs(void);
	Bool enterCrs(void);
	Bool leaveCrs(void);

#if defined(SSO_USE_SOUNDPOOL)
	// SoundPool
	void initSoundPool(void);
	void fillSoundPool(void);
#endif
	
	// 定数
	static const Uint32 FREQ			= CRISMP_SOUNDOUTPUT_FREQ;	// サンプリング周波数(/sec)
	static const Uint32 MAX_CH			= 2;		// GetData で取得できるチャンネル数
	static const Uint32 PACKET_NSMPL	= 1024*2;	// GetData１パケット内のサンプル数 =128
	
	static const Uint32 BITS_PER_CHANNEL	= 16;	// １フレーム内の各チャンネル毎のサンプルデータのビット数
	static const Uint32 CHANNELS_PER_FRAME	= 2;	// 各フレーム内のチャンネル数
	static const Uint32 FRAMES_PER_PACKET	= 1;	// 各パケット内のサンプルフレーム数

	// 変数
	Crs	m_crs;
	//
	Uint8 m_used;
	Uint32 m_init_count;
	
	// GetData 関数
	Uint32 (*m_func)(void *obj, Uint32 nch, Float32 *sample[], Uint32 nsmpl);
	void *m_obj;
	//
	Uint32 m_output_index;
	Uint32 m_n_smpls;
	Float32 *m_sample[MAX_CH];
	Float32 m_sample_buffer[MAX_CH][PACKET_NSMPL];	/* Sample Buffer for recieving from App. */
	Sint16 m_last_data[MAX_CH];

	//Audio Unit用の変数宣言
	AudioUnit m_outputUnit;
	AudioComponent m_component;
	AudioComponentDescription m_desc;
	AudioStreamBasicDescription m_streamFormat;

#if defined(SSO_USE_SOUNDPOOL)
	// Sound pool
	static const Uint32 NUM_POOLS = 3;	// サウンドプールの数
	static const Uint32 SMPLS_PER_POOL = PACKET_NSMPL * CHANNELS_PER_FRAME;

	Sint32 m_pool_index; // 参照中のプール
	Sint32 m_pool_state[NUM_POOLS];		// 使用可能な pool 内のサンプル数
	Sint16 m_sound_pool[NUM_POOLS][SMPLS_PER_POOL]; // 後はサウンドバッファへ送るだけのデータ
#endif
};



/****************************************************************************
 * 変数宣言
 ***************************************************************************/

// Sound Output  Object
static CriSmpSoundOutputLoc g_cri_smp_so_loc_obj;
// Error Output
static Bool g_cri_smp_so_err_disp = FALSE;
static void cri_smp_so_outputError(const Char8 *str, const Char8 *msg, Sint32 error);
// AudioUnit Sound Renderer
static void cri_smp_so_soundRenderer(void *inRefCon, AudioQueueRef inQ, AudioQueueBufferRef outQb);
//オーディオユニット用のコールバック関数プロトタイプ宣言
static OSStatus OutputCallback(void *inRefCon,
							   AudioUnitRenderActionFlags *ioActionFlags,
							   const AudioTimeStamp *inTimeStamp,
							   UInt32 inBusNumber,
							   UInt32 inNumberFrames,
							   AudioBufferList *ioData);


/****************************************************************************
 * 関数定義
 ***************************************************************************/
CriBool CriSmpSoundOutput::Initialize(void)
{
	return TRUE;
}

void CriSmpSoundOutput::Finalize(void)
{
}

/***
 *	サウンド出力オブジェクトの作成
 */
CriSmpSoundOutput* CriSmpSoundOutput::Create(void)
{
	return (CriSmpSoundOutput*)CriSmpSoundOutputLoc::Create();
}

/***
 *	サウンド出力オブジェクトは１つだけしか作成できない。
 *  audioQueueを作成している
 */
CriSmpSoundOutputLoc* CriSmpSoundOutputLoc::Create(void)
{
	CriSmpSoundOutputLoc *sout  = &g_cri_smp_so_loc_obj;
	
    if (++sout->m_init_count > 1)
		return sout;
	
    // バージョン文字列の参照
    cri_verstr_ptr = CRI_VERSTR_ARRAY_NAME;

	sout->initAudioFrame();
	
	// AudioUnit のオープンとプロパティのセット
	if (!sout->openAudioUnit()) {
		--sout->m_init_count;
		return NULL;
	}
	
	// critical section の生成
    sout->createCrs();
	
	sout->m_used = TRUE;

	return sout;
}

/***
 *	サウンド出力オブジェクトの破棄
 */
void CriSmpSoundOutputLoc::Destroy(void)
{
    if (m_init_count == 0)
		return;
    if (--m_init_count > 0)
		return;
	
	// AudioUnitの破棄
    closeAudioUnit();

    // critical section の破棄
    destroyCrs();

	m_used = FALSE;
}

/***
 *	サウンド出力処理の開始
 */
void CriSmpSoundOutputLoc::Start(void)
{
    enterCrs();
    start();	
    leaveCrs();
}

/***
 *	サウンド出力処理の停止
 */
void CriSmpSoundOutputLoc::Stop(void)
{
	enterCrs();	
    stop();
	leaveCrs();
}

/***
 *	executeAudioFrame()から呼ばれる、PCMデータ取得関数の登録
 */
void CriSmpSoundOutputLoc::SetNotifyCallback(Uint32 (*func1)(void *obj1, Uint32 nch, Float32 *sample[], Uint32 nsmpl), void *obj1)
{
	enterCrs();
	m_func = func1;
	m_obj = obj1;
	leaveCrs();
}

/***
 *	メインサーバー処理
 */
void CriSmpSoundOutputLoc::ExecuteMain(void)
{
#if defined(SSO_USE_SOUNDPOOL)
	enterCrs();
    fillSoundPool();
	leaveCrs();
#endif
}


void CriSmpSoundOutputLoc::SetFrequency(Uint32 frequency)
{
	// NOT SUPPORT YET.
}


/*
 * AudioUnit のオープンとプロパティのセット
 */
Bool CriSmpSoundOutputLoc::openAudioUnit(void)
{
	OSStatus err = noErr;
	AURenderCallbackStruct cb;

	/* AudioUnitComponentの取得 */
	m_desc.componentType = kAudioUnitType_Output;
	m_desc.componentSubType = kAudioUnitSubType_RemoteIO;
	m_desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	m_desc.componentFlags = 0;
	m_desc.componentFlagsMask = 0;
	m_component = AudioComponentFindNext(NULL, &m_desc);
	err = AudioComponentInstanceNew(m_component, &m_outputUnit);
	if (err != noErr) {
        cri_smp_so_outputError("SmpSoundOutput", "AudioComponentInstanceNew", err);
		return FALSE;
	}

	/* ストリームバッファのフォーマット設定 */
	m_streamFormat.mSampleRate = FREQ;
	m_streamFormat.mFormatID = kAudioFormatLinearPCM;
	//	streamFormat.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked | kAudioFormatFlagIsNonInterleaved;
	m_streamFormat.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
	m_streamFormat.mBitsPerChannel = BITS_PER_CHANNEL;
	m_streamFormat.mChannelsPerFrame = CHANNELS_PER_FRAME;
	m_streamFormat.mBytesPerFrame = (BITS_PER_CHANNEL/8)*CHANNELS_PER_FRAME;
	m_streamFormat.mFramesPerPacket = FRAMES_PER_PACKET;
	m_streamFormat.mBytesPerPacket = ((BITS_PER_CHANNEL/8)*CHANNELS_PER_FRAME) * FRAMES_PER_PACKET;
	err = AudioUnitSetProperty(m_outputUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &m_streamFormat, sizeof(AudioStreamBasicDescription));
	if (err != noErr) {
		cri_smp_so_outputError("SmpSoundOutput", "AudioUnitSetProperty:StreamFormat", err);
		return FALSE;
	}

	err = AudioUnitInitialize(m_outputUnit);
	if (err != noErr) {
		cri_smp_so_outputError("SmpSoundOutput", "AudioUnitInitialize", err);
		return FALSE;
	}

	/* Render Callback 関数の登録 */
	cb.inputProc = OutputCallback;
	cb.inputProcRefCon = this;
	err = AudioUnitSetProperty(m_outputUnit,
						 kAudioUnitProperty_SetRenderCallback,
						 kAudioUnitScope_Global,
						 0,
						 &cb,
						 sizeof(AURenderCallbackStruct));

	if (err != noErr) {
		cri_smp_so_outputError("SmpSoundOutput","AudioUnitSetProperty:SetRenderCallback", err);
		return FALSE;
	}

	return TRUE;
}

void CriSmpSoundOutputLoc::closeAudioUnit(void)
{
	OSStatus err = noErr;

	if (m_outputUnit == NULL)
		return;

	err = AudioOutputUnitStop(m_outputUnit);
	if (err != noErr)
		cri_smp_so_outputError("SmpSoundOutput", "AudioOutputUnitStop", err);

	err = AudioUnitUninitialize(m_outputUnit);
	if (err != noErr)
		cri_smp_so_outputError("SmpSoundOutput", "AudioUnitUninitialize", err);

	AudioComponentInstanceDispose(m_outputUnit);
	if (err != noErr)
		cri_smp_so_outputError("SmpSoundOutput", "AudioComponentInstanceDispose", err);

	m_outputUnit = NULL;
}

/***
 *	サウンド出力処理の開始
 */
void CriSmpSoundOutputLoc::start(void)
{
	OSStatus err=noErr;

	if (this->stat == EXEC)
		return;

	err = AudioOutputUnitStart(m_outputUnit);
	if (err != noErr) {
		cri_smp_so_outputError("SmpSoundOutput","AudioOutputUnitStart", err);
		return;
	}

 	this->stat = EXEC;
}

/***
 *	サウンド出力処理の停止
 */
void CriSmpSoundOutputLoc::stop(void)
{
	OSStatus err = noErr;

	if (this->stat != EXEC)
		return;

	err = AudioOutputUnitStop(m_outputUnit);
	if (err != noErr) {
		cri_smp_so_outputError("SmpSoundOutput", "AudioOutputUnitStop", err);
		return;
	}

	this->stat = STOP;
}

/***
 *	デコードデータバッファの初期化
 */
void CriSmpSoundOutputLoc::initAudioFrame(void)
{
	Sint32 i;
	
	// GetDataの受け皿
	m_output_index = 0;
	m_n_smpls = 0;

	for (i=0; i<MAX_CH; i++) {
		m_sample[i] = m_sample_buffer[i];
		m_last_data[i] = 0;	// 最後にセットしたデータ
	}

#if defined(SSO_USE_SOUNDPOOL)
	initSoundPool();
#endif
}

/***
 * AudioUnitからコールバックされるデータ供給関数
 *
 */
static OSStatus OutputCallback(void *inRefCon,
                               AudioUnitRenderActionFlags *ioActionFlags,
                               const AudioTimeStamp *inTimeStamp,
                               UInt32 inBusNumber,
                               UInt32 inNumberFrames,
                               AudioBufferList *ioData)
{
	CriSmpSoundOutputLoc *sout = (CriSmpSoundOutputLoc *)inRefCon;
	Sint32 i;

	/* バッファ(16bit・ステレオ・インターリーブフォーマット */
	for (i=0; i<ioData->mNumberBuffers; i++)
		sout->executeAudioFrame(inNumberFrames, (Sint16 *)ioData->mBuffers[i].mData);

	return noErr;
}


#if defined(SSO_USE_SOUNDPOOL)
/***
 * サウンドプールの初期化
 */
void CriSmpSoundOutputLoc::initSoundPool(void)
{
	m_n_smpls = 0;
	m_output_index = 0;

	m_pool_index = 0;
	for (Uint32 i=0; i<NUM_POOLS; i++) {
		m_pool_state[i] = 0;
		memset(m_sound_pool[i], 0, sizeof(Sint16)*SMPLS_PER_POOL);
	}
}

/***
 * デコードデータの取得
 */
void CriSmpSoundOutputLoc::fillSoundPool(void)
{
	Sint32 idx = m_pool_index;
	Uint32 index = m_output_index;
	Uint32 n_smpls = m_n_smpls;
	Float32 *left, *right;

	left = &m_sample[0][index];
	right = &m_sample[1][index];

	for (Uint32 i=0; i<NUM_POOLS; i++) {

		if (m_pool_state[idx] == 0) {
			Sint16 *odata = m_sound_pool[idx];
			
			for (Uint32 frame=0; frame<PACKET_NSMPL; frame++) {
				// パケットプールのデータを転送しつくしたら、新たにデータを取得(GetData)する。
				if (n_smpls == 0) {
					n_smpls = m_func(m_obj, MAX_CH, m_sample, PACKET_NSMPL);
					index = 0;
					left = m_sample[0];
					right = m_sample[1];
				}

				/* Sint16に変換しながらインターリーブする */
				{
					register Sint32 tmp;
					
					tmp = (*(Sint32*)left++) << 4;	// 固定小数点数を整数に戻す
					if (tmp > 32767)	tmp = 32767;
					if (tmp < -32768)	tmp = -32768;
					odata[0] = (Sint16)tmp;
					
					tmp = (*(Sint32*)right++) << 4;	// 固定小数点数を整数に戻す
					if (tmp > 32767)	tmp = 32767;
					if (tmp < -32768)	tmp = -32768;
					odata[1] = (Sint16)tmp;
				}
				
				odata += 2;
				index++;
				n_smpls--;
			}

			m_pool_state[idx] = PACKET_NSMPL;
		}

		idx = (idx+1) % NUM_POOLS;
	}

	m_output_index = index;
	m_n_smpls = n_smpls;
}


/***
 *	デコードデータの再生バッファへの転送
 */
void CriSmpSoundOutputLoc::executeAudioFrame(UInt32 inNumberFrames, Sint16 *odata)
{
	Uint32 pool_index = m_pool_index;

	for (Uint32 i=0; i<NUM_POOLS; i++) {
		Uint32 nfrm = m_pool_state[pool_index];
		Uint32 index = 0;

		if (nfrm == 0)
			break;

		if (inNumberFrames < nfrm)
			nfrm = inNumberFrames;

		index = PACKET_NSMPL - m_pool_state[pool_index];
		memcpy(odata, &m_sound_pool[pool_index][index*m_streamFormat.mChannelsPerFrame], nfrm*m_streamFormat.mBytesPerFrame);
		odata += nfrm * m_streamFormat.mChannelsPerFrame;

		inNumberFrames -= nfrm;
		m_pool_state[pool_index] -= nfrm;

		if (m_pool_state[pool_index] ==0)
			pool_index = (pool_index+1) % NUM_POOLS;

		/* 転送おしまい */
		if (inNumberFrames == 0)
			break;
	}

	if (inNumberFrames > 0)
		memset(odata, 0, inNumberFrames*m_streamFormat.mBytesPerFrame);

	m_pool_index = pool_index;
}
#endif


/***
 *	サウンド出力処理の開始
 *  AudioSession Interruption Callback の処理から呼ばれるサウンド開始処理
 */
void CriSmpSoundOutput_ReStartSound(void)
{
	CriSmpSoundOutputLoc *sout = &g_cri_smp_so_loc_obj;

    if (sout->m_init_count == 0)
		return;

    sout->enterCrs();
	AudioSessionSetActive(true);
	sout->start();
    sout->leaveCrs();
}

/***
 *	サウンド出力処理の停止
 *  AudioSession Interruption Callback の処理から呼ばれるサウンド停止処理
 */
void CriSmpSoundOutput_StopSound(void)
{
	CriSmpSoundOutputLoc *sout = &g_cri_smp_so_loc_obj;

    if (sout->m_init_count == 0)
		return;

    sout->enterCrs();
	sout->stop();
    sout->leaveCrs();
}



/* ------------------------------------------------------------------
 クリティカルセクション
 ----------------------------------------------------------------- */
/***
 * クリティカルセクションの作成
 */
Bool CriSmpSoundOutputLoc::createCrs(void)
{
	memset(&m_crs, 0, sizeof(Crs));
	
	m_crs.tid = (pthread_t)-1;
	
	if (pthread_mutex_init(&m_crs.mutex, NULL) != 0) {
        cri_smp_so_outputError("Crs", "pthread_mutex_init failed.", 0);
        return FALSE;
    }
	
	return TRUE;
}

/***
 *   クリティカルセクションの破棄
 */
void CriSmpSoundOutputLoc::destroyCrs(void)
{
	for (;;) {
        if (pthread_mutex_destroy(&m_crs.mutex) == 0) break;
		usleep(20000);
	}
	
	memset(&m_crs, 0, sizeof(Crs));
}

/***
 * クリティカルセクションへの進入
 */
Bool CriSmpSoundOutputLoc::enterCrs(void)
{
	pthread_t tid;
	
	tid = pthread_self();
	
	if (tid != m_crs.tid) {
		if (pthread_mutex_lock(&m_crs.mutex) != 0) {
            cri_smp_so_outputError("Crs", "pthread_mutex_lock failed.", 0);
            return FALSE;
        }
		
        m_crs.tid = tid;
	}
	
	++m_crs.level;
	
	if (m_crs.level < 0) {
        cri_smp_so_outputError("Crs", "Lock counter overflowed.", 0);
        return FALSE;
    }
	
	return TRUE;
}

/***
 * クリティカルセクションからの離脱
 */
Bool CriSmpSoundOutputLoc::leaveCrs(void)
{
	--m_crs.level;
	
	if (m_crs.level == 0) {
		m_crs.tid = (pthread_t)-1;
		
		if (pthread_mutex_unlock(&m_crs.mutex) != 0) {
            cri_smp_so_outputError("Crs", "pthread_mutex_unlock failed.", 0);
            return FALSE;
        }
	}
	
	if (m_crs.level < 0) {
        cri_smp_so_outputError("Crs", "Leave has been executed before enter.", 0);
        return FALSE;
    }
	
	return TRUE;
}


/* ------------------------------------------------------------------
 エラーメッセージ出力
 ----------------------------------------------------------------- */
/***
 * SoundOutput エラー出力の有無を設定
 */
void CriSmpSoundOutput_SetErrorOutputSw(Bool sw)
{
    g_cri_smp_so_err_disp = sw;
}

/*
  * エラーの出力
 */
static void cri_smp_so_outputError(const Char8 *str, const Char8 *msg, Sint32 error)
{
    if (!g_cri_smp_so_err_disp) return;
}


// end of file
