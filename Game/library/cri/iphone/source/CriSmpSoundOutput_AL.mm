/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006-2009 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Sound Output for OpenAL
 * File     : CriSmpSoundOuput_AL.cpp
 * Date     : 2009-01-29
 * Version  : 3.00
 *
 ****************************************************************************/
/* ---------------------------------------------------
 *	OpenAL 初期化・終了をADXTにまかせる
 * --------------------------------------------------- */
// #define	USE_ADXT

/* ---------------------------------------------------
 *	ExecuteMain処理時間の計測
 * --------------------------------------------------- */
//#define	MEASURE_LOAD


/****************************************************************************
 * インクルードファイル
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <memory.h>
#include <sys/time.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#if defined(USE_ADXT)
#include "cri_adxt.h"
#endif
#if defined(MEASURE_LOAD)
#include "CriSmpTimer.h"
#endif

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
    "CriSmpSoundOutput_AL"
    "/"
    "iPhone"
    " Ver.3.00"
    " Build:"__DATE__" "__TIME__"\n"
    "\0"
};

/****************************************************************************
 * データ型の宣言
 ***************************************************************************/
/**
 * サウンドレンダラからの出力は
 *   44100サンプル／秒
 *
 *  ExecuteMain関数が1/60sec間隔で呼ばれるとして
 *    735サンプル／(1/60sec)
 *
 *  1 フレームは 256 サンプル(2パケット)
 *  1 回の呼び出しでは 最大 2～3 フレーム転送できればよい
 */
class CriSmpSoundOutputLoc : public CriSmpSoundOutput
{
public:
    // 出力チャンネル
    enum CH {
        CH_TOP = 0,
        CH_L = CH_TOP,
        CH_R,
 	    CH_NUM
    };
    // Critical Section
    struct Crs {
        pthread_mutex_t mutex;		// ミューテックス
        pthread_t tid;				// スレッドID
        CriSint32 level;			// 多重呼び出しレベル
    };

    static CriSmpSoundOutputLoc* Create(void);
	virtual void Destroy(void);
	virtual void SetNotifyCallback(CriUint32 (*func)(void *obj, CriUint32 uch, CriFloat32 *sample[], CriUint32 nsmpl), void *obj);
	virtual void Start(void);
	virtual void Stop(void);
	virtual void ExecuteMain(void);
	virtual void SetFrequency(CriUint32 frequency);
    //
	CriBool initializeAL(void);
	void finalizeAL(void);
	void start(void);
	void stop(void);
	void executeAuduioFrame(void);
	CriBool check(const CriChar8 *msg);
    // パケット溜め込み処理
	void FillPool(void);
    // CriticalSection
    CriBool createCrs(void);
    void destroyCrs(void);
    CriBool enterCrs(void);
    CriBool leaveCrs(void);

    // 定数
	static const CriUint32 MAX_CH		= 2;							// GetData で取得するチャンネル数
    static const CriUint32 DATA_CH		= 1;							// 出力チャンネル数
    static const CriUint32 BUFFER_CH	= 2;							// １バッファ中に入れ込むチャンネル数

    static const CriUint32 FREQ			= 44100;						// サンプリング数(/sec)
    static const CriUint32 PACKET_NSMPL	= NSMPL_BLK;					// 1パケット内のサンプル数 =128

	static const CriUint32 FRAME_NPACKET= 2;							// 1フレーム内のパケット数
	static const CriUint32 FRAME_NSMPL	= PACKET_NSMPL*FRAME_NPACKET;	// 1フレーム内のサンプル数 = 256
    static const CriUint32 N_FRAME		= 24;							// 用意するフレーム数
    static const CriUint32 SET_NFRAME	= 16;							// 1回でセットするフレーム最大数
	//
    Crs	crs;
    //		
	CriUint8	used;
    CriUint32	init_count;
	CriUint32 (*func)(void *obj, CriUint32 nch, CriFloat32 *sample[], CriUint32 nsmpl);
	void *obj;
	//
	CriFloat32	sample_buffer[MAX_CH][FRAME_NSMPL*SET_NFRAME];	// Sample BUffer for recieving from App.
	CriSint32 wsmpl;
	CriSint32 rdidx;
    //
	CriSint16	*data[DATA_CH];			// データセット用バッファ
	ALenum	format;						// フォーマット
	CriSint32	nch;					// Output チャンネル数
	CriSint32	freq;					// サンプリング周波数
	CriSint32	unit;					// 単位読み込みサンプル数
	CriSint32	size;					// 単位読み込みサイズ
	CriSint32	nsmpl;					// 再生済みサンプル数
	//
	ALCdevice *alc_device;
	ALCcontext *alc_context;
	ALuint	buffer[DATA_CH][N_FRAME];	// バッファ
	ALuint	source[DATA_CH];			// ソース

#if defined(MEASURE_LOAD)
    CriSmpTimer *timer;
    CriUint32 cpu_load;					// cpu負荷 μsec
    CriUint32 exec_interval;			// ExecuteMain 呼出間隔 μsec
#endif
};


/****************************************************************************
 * 変数宣言
 ***************************************************************************/
// Sound Output  Object
static CriSmpSoundOutputLoc g_cri_smp_so_loc_obj;
// Error Output
static CriBool g_cri_smp_so_err_disp = FALSE;
static void cri_smp_so_outputError(const CriChar8 *str, const CriChar8 *msg, CriSint32 error);


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
	CriBool l_err = FALSE;
	CriSmpSoundOutputLoc *sout = &g_cri_smp_so_loc_obj;
   
    if (++sout->init_count > 1) return sout;

    // バージョン文字列の参照
    cri_verstr_ptr = CRI_VERSTR_ARRAY_NAME;

	sout->alc_device = NULL;
	sout->alc_context = NULL;
	sout->wsmpl = 0;
	sout->rdidx = 0;
	
    // OpenAL の初期化
	sout->initializeAL();

    // エラーステータスのクリア
    alGetError();

	// パラメータ設定
    sout->format= AL_FORMAT_STEREO16;
	sout->nch	= CriSmpSoundOutputLoc::BUFFER_CH;
	sout->freq	= CriSmpSoundOutputLoc::FREQ;
	sout->unit	= CriSmpSoundOutputLoc::FRAME_NSMPL;
	sout->size	= sout->unit * sout->nch * sizeof(Sint16);

	// データセット用バッファの確保
    for (CriUint32 ch=0; ch<CriSmpSoundOutputLoc::DATA_CH; ch++) {
        sout->data[ch] = (Sint16 *)malloc(sout->size);
        if (sout->data[ch] == NULL) {
            cri_smp_so_outputError("Create", "Can not allocate memory.\n", 0);

            for (CriUint32 i=0; i<ch; i++)
                free(sout->data[i]);

            --sout->init_count;

            return NULL;
        }
        memset(sout->data[ch], 0, sout->size);
    }

    // ソースの作成
    if (!l_err) {
        alGenSources(CriSmpSoundOutputLoc::DATA_CH, sout->source);
        if (sout->check("Can not create source.")) l_err = TRUE;
    }

    for (CriUint32 ch=0; ch<CriSmpSoundOutputLoc::DATA_CH; ch++) {
        if (!l_err) {
            // バッファの作成
            alGenBuffers(CriSmpSoundOutputLoc::N_FRAME, sout->buffer[ch]);
            if (sout->check("Can not create buffer.")) l_err = TRUE;

            // バッファのクリア
            if (!l_err) {
                for (CriUint32 j=0; j<CriSmpSoundOutputLoc::N_FRAME; j++) {
                    alBufferData(sout->buffer[ch][j], sout->format, sout->data[ch], sout->size, sout->freq);
                    if (sout->check("Failed to copy data.")) l_err = TRUE;
                }
            }

            // バッファをソースに割り当てる
            if (!l_err) {
                alSourceQueueBuffers(sout->source[ch], CriSmpSoundOutputLoc::N_FRAME, &sout->buffer[ch][0]);
                if (sout->check("Failed to queue buffer.")) {
					printf("\r\n %d %d %d\r\n", sout->source[ch], CriSmpSoundOutputLoc::N_FRAME, &sout->buffer[ch][0]);
					l_err = TRUE;
				}
            }
        }
    }

    // エラー発生時の後始末
	if (l_err) {
        for (CriUint32 ch=0; ch<CriSmpSoundOutputLoc::DATA_CH; ch++) {
            if (alIsBuffer(sout->buffer[ch][0]) == AL_TRUE)
                alDeleteBuffers(CriSmpSoundOutputLoc::N_FRAME, sout->buffer[ch]);

            if (alIsSource(sout->source[ch]) == AL_TRUE)
                alDeleteSources(1, &sout->source[ch]);

            if (sout->data[ch])
                free(sout->data[ch]);
        }

        --sout->init_count;

		return NULL;
	}

    // critical section の生成
    sout->createCrs();

    sout->used = TRUE;

#if defined(MEASURE_LOAD)
   	sout->timer = CriSmpTimer::Create();
    sout->cpu_load = 0;
    sout->exec_interval = 0;
    sout->timer->Start();
#endif

	return sout;
}

/**
 *	サウンド出力オブジェクトの破棄
 */
void CriSmpSoundOutputLoc::Destroy(void)
{
    if (init_count == 0) return;
    if (--init_count > 0) return;

    ALenum l_status;
	ALuint	l_buffers[N_FRAME];

	used = FALSE;

    for (CriUint32 ch=0; ch<DATA_CH; ch++) {
        // 停止状態でないなら、停止させる
        alGetSourcei(source[ch], AL_SOURCE_STATE, &l_status);
        check("Failed to get source status.");

        if (l_status != AL_STOPPED)
            alSourceStop(source[ch]);

        alSourceUnqueueBuffers(source[ch], N_FRAME, l_buffers);

        // バッファの開放
        if (alIsBuffer(buffer[ch][0]) == AL_TRUE)
            alDeleteBuffers(N_FRAME, buffer[ch]);
    }

    // ソースの開放	
    if (alIsSource(source[0]) == AL_TRUE)
        alDeleteSources(DATA_CH, source);

	// データセットバッファの開放
    for (CriUint32 ch=0; ch<DATA_CH; ch++) {
        if (data[ch]) {
            free(data[ch]);
            data[ch] = NULL;
        }
    }

	// OpenAL の終了
	finalizeAL();
	check("SmpSoundOutput::Destroy");

    // critical section の破棄
    destroyCrs();
}

/**
 * OpenAL デバイス、コンテキスト初期化
 */
CriBool CriSmpSoundOutputLoc::initializeAL(void)
{
	ALCenum err;
	ALCint attrlist[] = {
		ALC_FREQUENCY, 44100,	// 出力周波数 : 44100Hz
		ALC_REFRESH, 60,		// サーバ呼び出し頻度 : 60Hz
		ALC_SYNC, AL_TRUE,		// OpenAL側でスレッドを使用しない　
		0
	};

	// open device
	alc_device = alcOpenDevice(NULL);
	if (alc_device == NULL)
		return FALSE;
	
	// create context
	alc_context = alcCreateContext(alc_device, attrlist);
	if (alc_context == NULL) {
		err = alcGetError(alc_device);
        cri_smp_so_outputError("alc", "CreateContext\n", err);
		finalizeAL();
		return FALSE;
	}
	
	// change current context
	alcGetError(alc_device);
	if (alcMakeContextCurrent(alc_context) != ALC_TRUE) {
		err = alcGetError(alc_device);
        cri_smp_so_outputError("alc", "MakeContextCurrent\n", err);
		finalizeAL();
		return FALSE;
	}

	if (alc_context) {
		alcProcessContext(alc_context);
		err = alcGetError(alc_device);
		if (err != AL_NO_ERROR)
            cri_smp_so_outputError("alc", "ProcessContext\n", err);
	}
	return TRUE;
}

/**
 * OpenAL デバイス、コンテキスト破棄
 */
void CriSmpSoundOutputLoc::finalizeAL(void)
{
	if (alc_context != NULL) {
		alcMakeContextCurrent(NULL);
		alcDestroyContext(alc_context);
		alc_context=NULL;
	}

	if (alc_device != NULL) {
		alcCloseDevice(alc_device);
		alc_device=NULL;
	}
}


/**
 *	サウンド出力処理の開始
 */
void CriSmpSoundOutputLoc::Start(void)
{
    enterCrs();
    start();
    leaveCrs();
}

void CriSmpSoundOutputLoc::start(void)
{
    for (CriUint32 ch=0; ch<DATA_CH; ch++) {
        alSourcePlay(source[ch]);
        check("SmpSoundOutput::Start");
    }

 	stat = EXEC;
}

/**
 *	サウンド出力処理の停止
 */
void CriSmpSoundOutputLoc::Stop(void)
{
    enterCrs();
    stop();
    leaveCrs();
}

void CriSmpSoundOutputLoc::stop(void)
{
	stat = STOP;
}

/**
 *	executeAudioFrame()から呼ばれる、PCMデータ取得関数の登録
 */
void CriSmpSoundOutputLoc::SetNotifyCallback(CriUint32 (*func)(void *obj, CriUint32 nch, CriFloat32 *sample[], CriUint32 nsmpl), void *obj)
{
    enterCrs();
	this->func = func;
	this->obj = obj;
    leaveCrs();
}

/**
 *	メインサーバー処理
 */
void CriSmpSoundOutputLoc::ExecuteMain(void)
{
    enterCrs();

#if defined(MEASURE_LOAD)
    exec_interval = (Uint32)(timer->GetElapseMsTime() * 1000.0f);
    timer->Start();
    cpu_load = 0;
#endif
	
	executeAuduioFrame();

#if defined(MEASURE_LOAD)
    // msec to μsec
    cpu_load = (Uint32)(timer->GetElapseMsTime() * 1000.0f);
#endif

    leaveCrs();
}

/**
 *	デコードデータの再生バッファへの転送
 *		44100 samples/sec
 */
void CriSmpSoundOutputLoc::executeAuduioFrame(void)
{
	ALint l_nproc = -1;
	ALuint l_buffer[DATA_CH];
	CriFloat32 *l_sample[MAX_CH];

	if (stat == STOP) {
        for (CriUint32 ch=0; ch<DATA_CH; ch++)
            alSourceStop(source[ch]);
		return;
	}
    
	if (!func) return;

	// ステータスの監視
    {
        ALenum l_status;
        CriUint32 ln_stopped = 0;

        for (CriUint32 ch=0; ch<DATA_CH; ch++) {
            alGetSourcei(source[ch], AL_SOURCE_STATE, &l_status);
            check("Failed to get source status.");

            if (l_status == AL_STOPPED)
                ++ln_stopped;
        }

        if (stat == STOP) return;

        // すべてのCHでサウンドバッファを使い切ってしまっている場合、再スタート
        if (ln_stopped >= DATA_CH) start();
        // すべてのCHがバッファを使い切るまで待つ
        else if (ln_stopped > 0) return;
    }

    // 再生済みバッファ数の取得
    for (CriUint32 ch=0; ch<DATA_CH; ch++) {
        ALint nproc = 0;

        alGetSourcei(source[ch], AL_BUFFERS_PROCESSED, &nproc);
        check("Failed to processed buffers.");

        // 消費バランスが崩れないようにする
        if (nproc >= 0) {
            if (l_nproc < 0) l_nproc = nproc;
            else if (l_nproc != nproc) l_nproc = 0;
        }
    }

    // セットするフレーム数を制限する
    if (l_nproc > (ALint)SET_NFRAME)
        l_nproc = (ALint)SET_NFRAME;

	// デコードデータの受け先
	for (CriUint32 ch=0; ch<MAX_CH; ch++)
		l_sample[ch] = sample_buffer[ch];

	// 1FRAME分をデコード nsmpl has to be 128*N samples. (N=1,2,3...)
	if (wsmpl <= 0) {
		CriSint32 req_smpl;

		if (l_nproc == 0)
			req_smpl = FRAME_NSMPL*(SET_NFRAME/2);
		else
			req_smpl = FRAME_NSMPL*l_nproc;

		wsmpl = this->func(obj, MAX_CH, l_sample, req_smpl);
		rdidx=0;
	}

 	// 再生済みバッファの更新
	while (l_nproc > 0) {
		// 1FRAME分をデコード nsmpl has to be 128*N samples. (N=1,2,3...)
		if (wsmpl <= 0) {
			wsmpl = this->func(obj, MAX_CH, l_sample, FRAME_NSMPL*l_nproc);
			rdidx=0;
		}

		{
			CriFloat32 *lp_sample_l = &l_sample[0][rdidx];
			CriFloat32 *lp_sample_r = &l_sample[1][rdidx];
			CriSint16 *lp_data = data[0];

			// 再生済みバッファの取得
            alSourceUnqueueBuffers(source[0], 1, &l_buffer[0]);
            check("Failed to unqueue buffers.");

			// 再生済みバッファにデータを入れなおす
			for (CriUint32 j=0; j<FRAME_NSMPL; j++) {
				*lp_data = PcmfToPcm16(*lp_sample_l);
				*(lp_data+1) = PcmfToPcm16(*lp_sample_r);
				lp_sample_l++;
				lp_sample_r++;
				lp_data+=2;
			}

            // バッファへデータをセットする
            alBufferData(l_buffer[0], format, data[0], size, freq);
            // 新たにキューに積み直す
            alSourceQueueBuffers(source[0], 1, &l_buffer[0]);
        }

		rdidx += FRAME_NSMPL;
		wsmpl -= FRAME_NSMPL;
	   --l_nproc;
    }
}


void CriSmpSoundOutputLoc::SetFrequency(CriUint32 frequency)
{
	/*
    enterCrs();
	NOT SUPPORT YET.
    leaveCrs();
	*/
}

/**
 *	エラーチェック
 *
 */
CriBool CriSmpSoundOutputLoc::check(const CriChar8 *msg)
{
	ALenum error = alGetError();

	if (error != AL_NO_ERROR) {
        cri_smp_so_outputError("OpenAL", msg, error);
		return TRUE;
	}

	return FALSE;
}


/* ------------------------------------------------------------------
   クリティカルセクション
   ----------------------------------------------------------------- */
/**
 * クリティカルセクションの作成
 */
CriBool CriSmpSoundOutputLoc::createCrs(void)
{
	memset(&crs, 0, sizeof(CriSmpSoundOutputLoc::Crs));

	crs.tid = (pthread_t)-1;

	if (pthread_mutex_init(&crs.mutex, NULL) != 0) {
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
        if (pthread_mutex_destroy(&crs.mutex) == 0) break;
		usleep(20000);
	}

	memset(&crs, 0, sizeof(CriSmpSoundOutputLoc::Crs));
}

/**
 * クリティカルセクションへの進入
 */
CriBool CriSmpSoundOutputLoc::enterCrs(void)
{
	pthread_t tid;

	tid = pthread_self();

	if (tid != crs.tid) {
		if (pthread_mutex_lock(&crs.mutex) != 0) {
            cri_smp_so_outputError("Crs", "pthread_mutex_lock failed.", 0);
            return FALSE;
        }

        crs.tid = tid;
	}

	++crs.level;

	if (crs.level < 0) {
        cri_smp_so_outputError("Crs", "Lock counter overflowed.", 0);
        return FALSE;
    }

	return TRUE;
}

/**
 * クリティカルセクションからの離脱
 */
CriBool CriSmpSoundOutputLoc::leaveCrs(void)
{
	--crs.level;

	if (crs.level == 0) {
		crs.tid = (pthread_t)-1;

		if (pthread_mutex_unlock(&crs.mutex) != 0) {
            cri_smp_so_outputError("Crs", "pthread_mutex_unlock failed.", 0);
            return FALSE;
        }
	}

	if (crs.level < 0) {
        cri_smp_so_outputError("Crs", "Leave has been executed before enter.", 0);
        return FALSE;
    }

	return TRUE;
}


/* ------------------------------------------------------------------
   スレッド処理
   ----------------------------------------------------------------- */
enum CriSmpThrdStat {
	CRISMP_THRD_STAT_NOT_AVAILABLE = 0,
	CRISMP_THRD_STAT_AVAILABLE = 1,
};

struct CriSmpThrdInfo {
	pthread_t			tid;		// Thread ID
	CriSint32			policy;		// Scheduling policy
	pthread_attr_t		attr;		// Attribute
	struct sched_param	sched_prm;	// Scheduling parameter

	CriSmpThrdStat		stat;		// スレッド状態
    CriBool				act;		// スレッド実処理実施フラグ

    void				*arg;		// スレッド処理引数

    CriSint32			init_count;
};

static CriSmpThrdInfo cri_smp_sound_output_thrd_info;

struct CriSmpThrdParam {
    CriSint32 policy;
    CriSint32 priority;
    void *(*proc)(void *);
    void *arg;
};

/**
 *	待ち処理
 *	1/60sec 間隔で ExecuteMain が呼ばれるようにする。
 */
static void cri_smp_so_wait(CriUint32 exec_usec)
{
	if (1000000/60 < exec_usec) usleep(1000000/60);
	else usleep((1000000/60) - exec_usec);
}

/**
 * 時間の取得
 */
inline static CriUint32 cri_smp_so_getTime(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec & 2047) * 1000000 + tv.tv_usec;
}

/**
 *	SoundOutput スレッド処理
 */
static void *cri_smp_sound_output_proc(void *obj)
{
    CriUint32 exec_usec = 0;
	CriSmpThrdInfo *ti = (CriSmpThrdInfo *)obj;
    CriSmpSoundOutput *sout = (CriSmpSoundOutput *)ti->arg;

    // 親スレッドよりも若干優先度を上げる
    nice(-2);

	ti->stat = CRISMP_THRD_STAT_AVAILABLE;

    while (ti->act) {
        CriUint32 count, count2;

        cri_smp_so_wait(exec_usec);

        count = cri_smp_so_getTime();
        sout->ExecuteMain();
        count2 = cri_smp_so_getTime();

        if (count2 >= count)
            exec_usec = count2 - count;
        else
            exec_usec = (2048 * 1000000) - count + count2;
    }

	ti->stat = CRISMP_THRD_STAT_NOT_AVAILABLE;

	return NULL;
}

/**
 * スレッドの作成
 */
static CriBool cri_smp_so_createThread(CriSmpThrdInfo *ti, CriSmpThrdParam *prm)
{
	pthread_attr_init(&ti->attr);

    ti->act = FALSE;
	ti->stat = CRISMP_THRD_STAT_NOT_AVAILABLE;
    ti->act = TRUE;
    ti->arg = prm->arg;
	ti->policy = prm->policy;
    ti->sched_prm.sched_priority = prm->priority;

    pthread_attr_setschedpolicy(&ti->attr, ti->policy);

	//pthread_attr_getschedparam(&ti->attr, &ti->sched_prm);
    //ti->sched_prm.__sched_priority = prm->priority;
    //pthread_attr_setschedparam(&ti->attr,  &ti->sched_prm);

	if (pthread_create(&ti->tid, &ti->attr, prm->proc, ti)) {
        cri_smp_so_outputError("Trd", "Failed to create thread. (Super-user level is required.)", 0);
		return FALSE;
    }

	return TRUE;
}

/**
 * スレッドの開始待ち
 */
static void cri_smp_so_waitForThreadStart(CriSmpThrdInfo *ti)
{
	/* スレッドの動作開始まで待つ */
	if (ti->tid != 0) {
		for (;;) {
			if (ti->stat == CRISMP_THRD_STAT_AVAILABLE) break;

            cri_smp_so_wait(0);
		}
	}
}

/**
 * スレッドの終了待ち
 */
static void cri_smp_so_waitForThreadExit(CriSmpThrdInfo *ti)
{
	if (ti->tid != 0) {
		for (;;) {
            if (ti->stat == CRISMP_THRD_STAT_NOT_AVAILABLE) break;

            cri_smp_so_wait(0);
        }

        pthread_join(ti->tid, NULL);
		pthread_detach(ti->tid);
        ti->arg = NULL;
	}
}


/* ------------------------------------------------------------------
   エラーメッセージ出力
   ----------------------------------------------------------------- */
/*
 * エラーの出力
 */
static void cri_smp_so_outputError(const CriChar8 *str, const CriChar8 *msg, CriSint32 error)
{
    if (!g_cri_smp_so_err_disp) return;

    printf("SmpSoundOutput::%s error[%d]. %s\n", str, error, msg);
}


/* ------------------------------------------------------------------
   CriSmpSoundOutput_AL.hで宣言される関数
   ----------------------------------------------------------------- */
/**
 *	SoundOutputスレッドの生成
 */
CriSmpSoundOutput *CriSmpSoundOutput_CreateThread(void)
{
    CriSmpThrdInfo *ti = &cri_smp_sound_output_thrd_info;
    CriSmpSoundOutput *sout = CriSmpSoundOutput::Create();

    if (sout == NULL) return NULL;

    if (++ti->init_count > 1) return sout;

    // スレッド生成
    {
        CriSmpThrdParam param;

        param.policy = SCHED_FIFO;
		param.priority = 8;
        param.proc = cri_smp_sound_output_proc;
        param.arg = sout;

        if (!cri_smp_so_createThread(ti, &param)) {
            cri_smp_so_outputError("TRD", "Failed to create thread.", 0);
            sout->Destroy();
            --ti->init_count;
            return NULL;
        }
    }

    // スレッド処理開始を待つ
    cri_smp_so_waitForThreadStart(ti);

    return sout;
}

/**
 * SoundOutputスレッドの破棄
 */
void CriSmpSoundOutput_DestroyThread(void)
{
    CriSmpThrdInfo *ti = &cri_smp_sound_output_thrd_info;
    CriSmpSoundOutput *sout = (CriSmpSoundOutput *)ti->arg;

    if (!sout) return;

    if (ti->init_count == 0) return;
    if (--ti->init_count > 0) return;

    sout->Stop();

    ti->act = FALSE;
    cri_smp_so_waitForThreadExit(ti);

    sout->Destroy();
}

/**
 * SoundOutput エラー出力の有無を設定
 */
void CriSmpSoundOutput_SetErrorOutputSw(CriBool sw)
{
    g_cri_smp_so_err_disp = sw;
}

#if defined(MEASURE_LOAD)
/**
 * 処理負荷の取得
 *		直前に処理した ExecuteMain の計測値を返します。
 *
 *	load		: CPU負荷 μsec
 *	ngetdata	: GetDataした回数
 *	interval	: ExecuteMain 呼び出し間隔
 */
CriBool getSoundOouputCpuLoad(CriUint32 *load, CriUint32 *ngetdata, CriUint32 *interval)
{
	CriSmpSoundOutputLoc *sout  = &g_cri_smp_so_loc_obj;

    if (!sout) return FALSE;

    sout->enterCrs();

    if (load) *load = sout->cpu_load;
    if (ngetdata) *ngetdata = sout->n_getdata;
	if (interval) *interval = sout->exec_interval;

    sout->leaveCrs();

    return TRUE;
}
#endif

// end of file
