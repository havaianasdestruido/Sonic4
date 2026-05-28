/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2009 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Sound Output for AudioQueue
 * File     : CriSmpSoundOuput_AudioQueue.cpp
 * Date     : 2009-01-07
 * Version  : 1.00
 *
 ****************************************************************************/

/****************************************************************************
 * インクルードファイル
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
//#include <CoreServices/CoreServices.h>
#include <AudioToolbox/AudioToolbox.h>

#include "CriSmpSoundOutput.h"


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
    "CriSmpSoundOutput_AudioQueue"
    "/"
    "iPhone OS"
    " Ver.1.00"
    " Build:"__DATE__" "__TIME__"\n"
    "\0"
};

/****************************************************************************
 * データ型の宣言
 ***************************************************************************/
/**
 * サウンドレンダラからの出力は
 *   44100サンプル／秒
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
	void executeAudioFrame(UInt32 inNumberFrames, Sint16 *oData);
	void fill(void);

	// AudioUnit
	Bool openAudioQueue(void);
	void closeAudioQueue(void);
	Bool startAudioQueue(void);
	Bool stopAudioQueue(void);
    // CriticalSection
    Bool createCrs(void);
    void destroyCrs(void);
    Bool enterCrs(void);
    Bool leaveCrs(void);

    // 定数
    static const Uint32 FREQ			= 44100;		// サンプリング周波数(/sec)
	static const Uint32 MAX_CH			= 2; //MAX_NCH;		// GetData で取得できるチャンネル数
    static const Uint32 PACKET_NSMPL	= NSMPL_BLK*16;	// GetData１パケット内のサンプル数 =128

	static const Uint32 BITS_PER_CHANNEL	= 16;	  									// １フレーム内の各チャンネル毎のサンプルデータのビット数
	static const Uint32 CHANNELS_PER_FRAME	= 2;										// 各フレーム内のチャンネル数
	static const Uint32 BYTES_PER_FRAME		= (BITS_PER_CHANNEL/8)*CHANNELS_PER_FRAME;	// １サンプルフレームのバイト数
	static const Uint32 FRAMES_PER_PACKET	= 1;										// 各パケット内のサンプルフレーム数
	static const Uint32 BYTES_PER_PACKET	= (BYTES_PER_FRAME*FRAMES_PER_PACKET);		// データ１パケット中のバイト数

	static const Uint32 FRAMES_PER_BUFFER = 1024*2;	// サウンドバッファ内のサンプル数
	static const Uint32 NUM_BUFFERS = 3;	// サウンドバッファの数

	static const Uint32 SIZE_BUFFER = FRAMES_PER_BUFFER*BYTES_PER_PACKET; // １バッファのサイズ(byte) */

	static const Uint32 NUM_POOLS = 2;	// サウンドプールの数
	static const Uint32 SMPLS_PER_POOL = FRAMES_PER_BUFFER*CHANNELS_PER_FRAME;

    // 変数
    Crs	crs;
    //
	Uint8	used;
    Uint32	init_count;
	// Callback Function
	Uint32 (*func)(void *obj, Uint32 nch, Float32 *sample[], Uint32 nsmpl);
	Uint32 (*func_2)(void *obj, Uint32 nch, Float32 *sample0, Float32 *sample1, Float32 *sample2, Float32 *sample3, Float32 *sample4, Float32 *sample5, Uint32 nsmpl);
	void *obj;
	//
	Sint32 remains_smpls;
	Float32	sample_buffer[MAX_CH][PACKET_NSMPL];	// Sample BUffer for recieving from App.

	// for AudioQueu
	AudioQueueRef audio_queue;
	AudioQueueBufferRef audio_buffer[NUM_BUFFERS];

	// Sound pool
	Sint32 pool_index; // 参照中のプール
	Sint32 pool_state[NUM_POOLS];		// 使用可能な pool 内のサンプル数
	Sint16 sound_pool[NUM_POOLS][SMPLS_PER_POOL];
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

/**
 *	サウンド出力オブジェクトの作成
 */
CriSmpSoundOutput* CriSmpSoundOutput::Create(void)
{
	return (CriSmpSoundOutput*)CriSmpSoundOutputLoc::Create();
}

/**
 *	サウンド出力オブジェクトは１つだけしか作成できない。
 */
CriSmpSoundOutputLoc* CriSmpSoundOutputLoc::Create(void)
{
	CriSmpSoundOutputLoc *sout  = &g_cri_smp_so_loc_obj;
   
    if (++sout->init_count > 1) return sout;

    // バージョン文字列の参照
    cri_verstr_ptr = CRI_VERSTR_ARRAY_NAME;

	// AudioQueue のオープンとプロパティのセット
	if (!sout->openAudioQueue()) {
		--sout->init_count;
		return NULL;
	}

	sout->remains_smpls = 0;

    // critical section の生成
    sout->createCrs();
 
	// sound pool
	sout->pool_index = 0;
	for (Uint32 i=0; i<CriSmpSoundOutputLoc::NUM_POOLS; i++) {
		sout->pool_state[i] = 0;
		memset(sout->sound_pool[i], 0, CriSmpSoundOutputLoc::SIZE_BUFFER);
	}

	return sout;
}

/**
 *	サウンド出力オブジェクトの破棄
 */
void CriSmpSoundOutputLoc::Destroy(void)
{
    if (this->init_count == 0) return;
    if (--this->init_count > 0) return;

	closeAudioQueue();

	this->used = FALSE;

    // critical section の破棄
    this->destroyCrs();
}

/**
 *	サウンド出力処理の開始
 */
void CriSmpSoundOutputLoc::Start(void)
{
    this->enterCrs();
    this->start();
    this->leaveCrs();
}

void CriSmpSoundOutputLoc::start(void)
{
 	this->stat = this->EXEC;
	this->startAudioQueue();
}

/**
 *	サウンド出力処理の停止
 */
void CriSmpSoundOutputLoc::Stop(void)
{
	this->enterCrs();
    this->stop();
	this->leaveCrs();
}

void CriSmpSoundOutputLoc::stop(void)
{
	stopAudioQueue();
	this->stat = this->STOP;
}

/**
 *	executeAudioFrame()から呼ばれる、PCMデータ取得関数の登録
 */
void CriSmpSoundOutputLoc::SetNotifyCallback(Uint32 (*func)(void *obj, Uint32 nch, Float32 *sample[], Uint32 nsmpl), void *obj)
{
	this->enterCrs();
	this->func = func;
	this->obj = obj;
	this->leaveCrs();
}

/**
 *	メインサーバー処理
 */
void CriSmpSoundOutputLoc::ExecuteMain(void)
{
	this->enterCrs();
    this->fill();
	this->leaveCrs();
}


void CriSmpSoundOutputLoc::SetFrequency(Uint32 frequency)
{
	// NOT SUPPORT YET.
}


/*
 * AudioUnit のオープンとプロパティのセット
 */
Bool CriSmpSoundOutputLoc::openAudioQueue(void)
{
	OSStatus err = noErr;

	// デフォルトアウトプットユニットのオープン
	// 出力フォーマットの設定
	AudioStreamBasicDescription streamFormat;
	streamFormat.mSampleRate = CriSmpSoundOutputLoc::FREQ;
	streamFormat.mFormatID = kAudioFormatLinearPCM;
	streamFormat.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
	streamFormat.mBitsPerChannel = CriSmpSoundOutputLoc::BITS_PER_CHANNEL;
	streamFormat.mChannelsPerFrame = CriSmpSoundOutputLoc::CHANNELS_PER_FRAME;
	streamFormat.mBytesPerFrame = CriSmpSoundOutputLoc::BYTES_PER_FRAME;
	streamFormat.mFramesPerPacket = CriSmpSoundOutputLoc::FRAMES_PER_PACKET;
	streamFormat.mBytesPerPacket = CriSmpSoundOutputLoc::BYTES_PER_PACKET;

	err = AudioQueueNewOutput(&streamFormat, cri_smp_so_soundRenderer, (void *)this, CFRunLoopGetCurrent(), kCFRunLoopCommonModes, 0, &this->audio_queue);

	for (Uint32 i=0; i<CriSmpSoundOutputLoc::NUM_BUFFERS; i++) {
		err = AudioQueueAllocateBuffer(this->audio_queue, streamFormat.mBytesPerFrame*CriSmpSoundOutputLoc::FRAMES_PER_BUFFER, &this->audio_buffer[i]);
		
		if (!err)
			cri_smp_so_soundRenderer(NULL, this->audio_queue, this->audio_buffer[i]);
	}

	return TRUE;
}


void CriSmpSoundOutputLoc::closeAudioQueue(void)
{
	for (Uint32 i=0; i<CriSmpSoundOutputLoc::NUM_BUFFERS; i++)
		AudioQueueFreeBuffer(this->audio_queue, this->audio_buffer[i]);

	AudioQueueDispose(this->audio_queue, true);
}

Bool CriSmpSoundOutputLoc::startAudioQueue(void)
{
	OSStatus err = noErr;

	err = AudioQueueStart(this->audio_queue, NULL);
	if (err) return FALSE;

	AudioQueueSetParameter(this->audio_queue, kAudioQueueParam_Volume, 1.0);
	
	return TRUE;
}

Bool CriSmpSoundOutputLoc::stopAudioQueue(void)
{
	OSStatus err = noErr;

	err = AudioQueuePause(this->audio_queue);

	if (err) return FALSE;

	return TRUE;
}

/**
 *	デコードデータの再生バッファへの転送
 *		48000 samples/sec
 */
void CriSmpSoundOutputLoc::executeAudioFrame(UInt32 inNumberFrames, Sint16 *odata)
{
	/* まだ補充されていない */
	if (this->pool_state[this->pool_index] == 0) {
		memset(odata, 0, inNumberFrames*BYTES_PER_FRAME);
		return;
	}

	Sint16 *pool = this->sound_pool[this->pool_index];
	memcpy(odata, pool, inNumberFrames*BYTES_PER_FRAME);
	this->pool_state[this->pool_index] = 0;

	this->pool_index = (this->pool_index+1) % NUM_POOLS;
}

/*
 * デコードデータの取得
 * データ転送処理内で行うのはからだに悪そうなので、I／O処理tは別に行うべきか？
 */
void CriSmpSoundOutputLoc::fill(void)
{
	Sint32 idx;
	Uint32 l_index = CriSmpSoundOutputLoc::PACKET_NSMPL - this->remains_smpls;
	Float32 *l_sample[MAX_CH];

	// こんど参照するバッファ
	idx = this->pool_index;

	// デコードデータの受け先 
	for (Uint32 ch=0; ch<MAX_CH; ch++)
		l_sample[ch] = sample_buffer[ch];

	for (Uint32 i=0; i<NUM_POOLS; i++) {
		Sint16 *odata = this->sound_pool[idx];

		if (this->pool_state[idx] == 0) {
			for (Uint32 frame=0; frame<FRAMES_PER_BUFFER; ++frame) {
				// パケットプールのデータを転送しつくしたら、新たにデータを取得(GetData)する。
				if (remains_smpls <= 0) {
					// 1PACKET分をデコード nsmpl has to be 128*N samples. (N=1,2,3...)
					remains_smpls = this->func(obj, MAX_CH, l_sample, PACKET_NSMPL);
					l_index = 0;
				}

				// インターリーブする
				*odata++ = PcmfToPcm16(l_sample[0][l_index]); /* L */
				*odata++ = PcmfToPcm16(l_sample[1][l_index]); /* R */

				++l_index;
				--remains_smpls;
			}

			this->pool_state[idx] = FRAMES_PER_BUFFER;
		}

		idx = (idx+1) % NUM_POOLS;
	}
}


/* ------------------------------------------------------------------
   クリティカルセクション
   ----------------------------------------------------------------- */
/**
 * クリティカルセクションの作成
 */
Bool CriSmpSoundOutputLoc::createCrs(void)
{
	memset(&this->crs, 0, sizeof(CriSmpSoundOutputLoc::Crs));

	this->crs.tid = (pthread_t)-1;

	if (pthread_mutex_init(&this->crs.mutex, NULL) != 0) {
        cri_smp_so_outputError("Crs", "pthread_mutex_init failed.", 0);
        return FALSE;
    }

	return TRUE;
}

/**
 *   クリティカルセクションの破棄
 */
void CriSmpSoundOutputLoc::destroyCrs(void)
{
	for (;;) {
        if (pthread_mutex_destroy(&this->crs.mutex) == 0) break;
		usleep(20000);
	}

	memset(&this->crs, 0, sizeof(CriSmpSoundOutputLoc::Crs));
}

/**
 * クリティカルセクションへの進入
 */
Bool CriSmpSoundOutputLoc::enterCrs(void)
{
	pthread_t tid;

	tid = pthread_self();

	if (tid != this->crs.tid) {
		if (pthread_mutex_lock(&this->crs.mutex) != 0) {
            cri_smp_so_outputError("Crs", "pthread_mutex_lock failed.", 0);
            return FALSE;
        }

        this->crs.tid = tid;
	}

	++this->crs.level;

	if (this->crs.level < 0) {
        cri_smp_so_outputError("Crs", "Lock counter overflowed.", 0);
        return FALSE;
    }

	return TRUE;
}

/**
 * クリティカルセクションからの離脱
 */
Bool CriSmpSoundOutputLoc::leaveCrs(void)
{
	--this->crs.level;

	if (this->crs.level == 0) {
		this->crs.tid = (pthread_t)-1;

		if (pthread_mutex_unlock(&this->crs.mutex) != 0) {
            cri_smp_so_outputError("Crs", "pthread_mutex_unlock failed.", 0);
            return FALSE;
        }
	}

	if (this->crs.level < 0) {
        cri_smp_so_outputError("Crs", "Leave has been executed before enter.", 0);
        return FALSE;
    }

	return TRUE;
}


/* ------------------------------------------------------------------
   エラーメッセージ出力
   ----------------------------------------------------------------- */
/**
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

    printf("SmpSoundOutput::%s error[%d]. %s\n", str, error, msg);
}


/* ------------------------------------------------------------------
   AudioUnit からコールバックされるデータ供給関数
   ----------------------------------------------------------------- */
/**
 * サウンドバッファにデータを書き込み、キューへ積む
 */
static void cri_smp_so_soundRenderer(void *inRefCon, AudioQueueRef inQ, AudioQueueBufferRef outQb)
{
	CriSmpSoundOutputLoc *sout = (CriSmpSoundOutputLoc *)inRefCon;

	// バッファ１本分のデータを埋める
	if (sout == NULL) {
		memset((Sint16 *)outQb->mAudioData, 0, CriSmpSoundOutputLoc::BYTES_PER_FRAME*CriSmpSoundOutputLoc::FRAMES_PER_BUFFER);
	}
	else {
		sout->executeAudioFrame(CriSmpSoundOutputLoc::FRAMES_PER_BUFFER, (Sint16 *)outQb->mAudioData);
	}
	// バッファをキューに積む
	outQb->mAudioDataByteSize = sizeof(Sint16)*CriSmpSoundOutputLoc::FRAMES_PER_BUFFER*2;
	AudioQueueEnqueueBuffer(inQ, outQb, 0, NULL);
}


// end of file
