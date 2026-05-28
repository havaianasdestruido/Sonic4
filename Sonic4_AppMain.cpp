/*****************************************************************************/
/*      Sonic4_AppMain.cpp          Author : Syuichi Gotou                   */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* iPhone アプリケーションメイン                                             */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090630-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files (Pre Definitions) ---------------------------------------*/

#include <alice.h>
#include "Sonic4_AppMain.h"
#include "Sonic4_AppVar.h"
#include "Sonic4_Utility.h"
#include "gsMainSys.h"
#include "../../src/accelCircularBuffer.hpp"
#include <cmath>
#include <numeric>
#define CHECK_GL_ERROR() ({ GLenum __error = glGetError(); if(__error) printf("OpenGL error 0x%04X in %s\n", __error, __FUNCTION__); (__error ? false : true); })

#if AMD_SAMPLE_SAVE_LOG
#include <sys/time.h>
#endif // AMD_SAMPLE_SAVE_LOG

#include <AudioToolbox/AudioServices.h>

/*--- Definitions -----------------------------------------------------------*/

#define AMD_DEBUG_CHAR_MAX		(1200)		// デバッグ文字数
#define AMD_STANDARD_SHADER_NUM	(256)		// 標準シェーダー数

#define AMD_HOG_HEAP_SIZE		(1024*1024*27)	// 27MB

/*--- Macros ----------------------------------------------------------------*/

/*--- Include Files (Post Definitions) --------------------------------------*/
#define WINDOWS_DATA	(0)  // WindowsPCのデータを読み込む


/*--- Local Declarations ----------------------------------------------------*/

static int  init(const char* pDocPath);
static void game_proc(int arg);
static int  draw_proc(void);
static void cri_proc(int arg);
static void finish(void);

static void initNN(void);
static void exitNN(void);

/*--- External Variables ----------------------------------------------------*/
#if APPD_MAIN_DRAW_FRAME
extern Sint32 _am_dbg_display_mode;
#endif // APPD_MAIN_DRAW_FRAME

/*--- Global Variables ------------------------------------------------------*/

Uint32	_am_system_flag[4];				// システムフラグ
Uint32	_am_debug_flag[4];				// デバッグフラグ

Uint32	_am_draw_counter = 0;

void	*_am_debug_print_buf = NULL;

NNS_MATRIXSTACK	_am_default_stack;
void	*_am_default_stack_buf;

#if AMD_USE_DRAW_THREAD
AMS_ALARM		_am_main_timer;
AMS_THREAD		_am_game_thread;
AMS_THREAD_ID	_am_game_thread_id = (AMS_THREAD_ID)0;
NNS_MATRIXSTACK	_am_game_stack;
AMS_THREAD		_am_cri_thread;
AMS_THREAD_ID	_am_cri_thread_id = (AMS_THREAD_ID)0;
void	*_am_game_stack_buf;
#else
AMS_THREAD		_am_cri_thread;
AMS_THREAD_ID	_am_cri_thread_id = (AMS_THREAD_ID)0;
#endif

int _am_sample_count = AMD_SAMPLE_COUNT_DEFAULT;
BOOL _am_sample_draw_enable = FALSE;
BOOL _am_sample_is_suspended = FALSE;
int _am_sample_suspended_count; // ハード固有のサスペンド命令が発生している待ちカウント
BOOL _am_sample_is_sleep; //!< スリープに移行するか否か
BOOL _am_sample_is_accel; // 加速度センサーを使うか否か
BOOL _am_sample_is_ignore_audio_interruption = FALSE; //オーディオ割り込み処理の無視

#if !AMD_DEBUG_FRAME_RATE
extern float _am_performance_GPU;
#endif // !AMD_DEBUG_FRAME_RATE

BOOL _am_sample_end_suspended = FALSE;
#if APPD_MAIN_CRIAUDIO_INIT_DELAY
BOOL _am_sample_init = FALSE;
#endif // APPD_MAIN_CRIAUDIO_INIT_DELAY

/*--- Local Variables -------------------------------------------------------*/
namespace {
	//フレームレートの算出
	namespace vfr {
		const int c_fps_count_buf = 60;		//<フレームレートを決定する為の更新レートの履歴
		typedef accel::CCircularBuffer<int, c_fps_count_buf> TFpsCountBuf;
		TFpsCountBuf l_fps_count_buf;
	}
}

/*--- Inline Functions ------------------------------------------------------*/
//## Inline Functions

/*--- Global Functions ------------------------------------------------------*/
//## Global Functions

/*****************************************************************************/
/* int Sonic4_AppInit(int Width, int Height, const char* pDocPath)                  */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  アプリケーション初期化                                        */
/*****************************************************************************/
int Sonic4_AppInit(int Width, int Height, const char* pDocPath)
{
	// 変数の初期化
	Sonic4_AppVar::Init();
	
	// /var/mobile/Applications/.../*.app の末尾に / をつける
	char path[sizeof(_am_fs_device_name)/sizeof(_am_fs_device_name[0])];
	memset(path, 0, sizeof(_am_fs_device_name)/sizeof(_am_fs_device_name[0]));
	AMD_STRCPY_S(path, sizeof(_am_fs_device_name)/sizeof(_am_fs_device_name[0]), (char*)pDocPath);
	int len = strlen(path);
	if (path[len-1] != '/')
	{
		path[len] = '/';
	}
	AMD_STRCAT_S(_am_fs_device_name, sizeof(_am_fs_device_name)/sizeof(_am_fs_device_name[0]), "Data/");
	
	amIPhoneInitNN(Width, Height);
	init(path);

	AppLog("test!");
	
	return	0;
}

/*****************************************************************************/
/* int Sonic4_AppMainLoop(void)                                                     */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  アプリケーションメインループ                                  */
/*****************************************************************************/
int Sonic4_AppMainLoop(void)
{
	int		result = 0;

#if AMD_USE_DRAW_THREAD
	result = draw_proc();
#else
	int loop = _am_sample_count;
//	result = amIPhoneMainLoop();
	// amIPhoneMainLoop()を自力実装
	
	// シングルスレッド
#if AMD_DEBUG
	AMS_TIMER	timer;
	amTimerCreate(&timer);
#if AMD_DEBUG_FRAME_RATE
	amTimerStart(&timer);
#endif
#endif
	
	// 終了チェック
	if (_amIPhoneCheckExit()) {
		return 1;
	}
#if APPD_MAIN_CRIAUDIO_INIT_DELAY
	// 初期化チェック
	if (!_am_sample_init) {
		AppLog("_am_sample_init init");
		
		// 一度AudioSessionを初期化
		UInt32 sessionCategory = kAudioSessionCategory_AmbientSound; //qqq - for iPod enable// kAudioSessionCategory_SoloAmbientSound; //kAudioSessionCategory_MediaPlayback;
		AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
		AudioSessionSetActive(true);
		
		// Set IO Buffer size
		//Float32 buffersize = 512.0/CRISMP_SOUNDOUTPUT_FREQ;
		Float32 buffersize = 512.0/22050.0;
		UInt32 size = sizeof(buffersize);
		AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, size, &buffersize);
		AudioSessionSetActive(true);
		
		// CriOutput Init
		if (amCriAudioCreateSmpOutput()) {
			amCriAudioInit();
			GsInitUser();
			_am_sample_init = TRUE;
			AppLog("_am_sample_init done");
		}
		else {
			AppLog("_am_sample_init miss");
			return 0;
		}
#if 0
/*		
		CriSmpSoundOutput* sndout = NULL;
		sndout = CriSmpSoundOutput::Create();
		if (sndout) {
			amCriAudioInit(sndout);
			GsInitUser();
			_am_sample_init = TRUE;
			AppLog("_am_sample_init done");
		}
		else {
			// 一度AudioSessionを初期化
			UInt32 sessionCategory = kAudioSessionCategory_SoloAmbientSound; //kAudioSessionCategory_MediaPlayback;
			AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
			AudioSessionSetActive(true);
			
			// Set IO Buffer size
			//Float32 buffersize = 512.0/CRISMP_SOUNDOUTPUT_FREQ;
			Float32 buffersize = 512.0/22050.0;
			UInt32 size = sizeof(buffersize);
			AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, size, &buffersize);
			AudioSessionSetActive(true);
			
			return 0;
		}
 */
#endif // 0
	}
#endif // APPD_MAIN_CRIAUDIO_INIT_DELAY
	//フレームレートの算出
	AMS_TIMER fps_count_timer;
	amTimerCreate(&fps_count_timer);
	amTimerStart(&fps_count_timer);
	
	{{//qqq - bgm vs ipod
		/*
		extern void mpp_gsOnOffBGM(int on_off);
		
		static int bgmCount = 0;
		bgmCount = (bgmCount+1) & 0xF;
		static Uint32 iPodMusicIsPlaying_last = 1111;
		
		if(bgmCount == 0) {
			UInt32 iPodMusicIsPlaying = 0;
			UInt32 ioDataSize = sizeof( iPodMusicIsPlaying );
			AudioSessionGetProperty( kAudioSessionProperty_OtherAudioIsPlaying, &ioDataSize, &iPodMusicIsPlaying );	
			/+
			static int mCount = 0;
			mCount = (mCount+1) & 0x1F;
			iPodMusicIsPlaying = mCount>>4;+/
			
			
			
			if(iPodMusicIsPlaying!=iPodMusicIsPlaying_last) 
			{
				iPodMusicIsPlaying_last = iPodMusicIsPlaying;
				mpp_gsOnOffBGM(!iPodMusicIsPlaying);			
			}
			
		}*/
		
		UInt32 iPodMusicIsPlaying = 0;
		UInt32 ioDataSize = sizeof( iPodMusicIsPlaying );
		AudioSessionGetProperty( kAudioSessionProperty_OtherAudioIsPlaying, &ioDataSize, &iPodMusicIsPlaying );
		
		static bool last_iPodMusicIsPlaying = false;
		
		if(iPodMusicIsPlaying) {
			CriSmpSoundOutput_StopSound();
		}
		else {
			if(last_iPodMusicIsPlaying) {
				if(_am_sample_suspended_count<=0) {
					CriSmpSoundOutput_ReStartSound();
				}
			}
		}
		last_iPodMusicIsPlaying = iPodMusicIsPlaying;
		
	}}

	amDrawBeginScene();
	if (amDrawBegin()) {
		// サスペンド復帰待ち設定
		if (!_am_sample_is_suspended) {
			if (_am_sample_suspended_count > 0) {
				_am_sample_suspended_count--;
				//printf("Suspend Count %d \n", _am_sample_suspended_count);
			}
		}
		
		// カウント完了待ち
		if (_am_sample_suspended_count <= 0) {
			// サスペンド命令来たら更新しない!!
			// 前がサスペンドだったか設定
			GsMainSysSetSuspendedFlag(_am_sample_end_suspended);
			if (_am_sample_end_suspended) {
				//printf("Suspend Count %d \n", _am_sample_suspended_count);
				_am_sample_end_suspended = FALSE;
#if 0
				/*
				// Initialize for audio session
				Uint32 sessionCategory = kAudioSessionCategory_SoloAmbientSound; //kAudioSessionCategory_MediaPlayback;
				AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
				AudioSessionSetActive(true);

				// Set IO Buffer size
				//Float32 buffersize = 512.0/CRISMP_SOUNDOUTPUT_FREQ;
				Float32 buffersize = 512.0/22050.0;
				Uint32 size = sizeof(buffersize);
				AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, size, &buffersize);
				AudioSessionSetActive(true);
				 */
#endif // 0
				CriSmpSoundOutput_ReStartSound();
				
				CHECK_GL_ERROR();
			}
			// メイン
#if AMD_DEBUG && !AMD_DEBUG_FRAME_RATE
			amTimerStart(&timer);
#endif
			int check = loop - 2;
			if (check < 0) {
				check = 0;
			}
			for (int i = 0; i < loop; i++) {
				if (i == check) {
					_am_sample_draw_enable = TRUE;
				}
				else {
					_am_sample_draw_enable = FALSE;
				}
				amPadGetData();
				amTpExecute();
				amTaskExecute();
				amDrawCloseDisplayList();
			}
			amDrawGetDisplayList();
			
			amFsServer();
			
#if AMD_DEBUG && !AMD_DEBUG_FRAME_RATE
			//amTimerEnd(&timer);
			//_am_performance_GPU = timer.frame;
			//amDebugSetPerformanceGPU(timer.frame);
#endif
		}
		else {
			// サスペンド履歴を残す
			_am_sample_end_suspended = TRUE;
		}
#if APPD_MAIN_DRAW_FRAME
		int debug_def = _am_dbg_display_mode;
		_am_dbg_display_mode = 2;
#endif // APPD_MAIN_DRAW_FRAME
		amMemDisplayInfo(AMD_TEXT_WIDTH - 22, 2);
#if APPD_MAIN_DRAW_FRAME
		_am_dbg_display_mode = debug_def;
#endif // APPD_MAIN_DRAW_FRAME
#if AMD_DEBUG && !AMD_DEBUG_FRAME_RATE
		amTimerEnd(&timer);
		amDebugSetPerformanceMain(timer.frame);
#endif
		// 描画
		amDrawExecRegist();
		amDrawExecCommand(AMD_COMMAND_STATE_MAKE_TASK);
		amDrawExecute();
		amDrawDisplay();
		amDrawExecCommand(AMD_COMMAND_STATE_DEBUG);
#if AMD_DEBUG && !AMD_DEBUG_FRAME_RATE
		amTimerEnd(&timer);
		amDebugSetPerformanceDraw(timer.frame);
#endif
#if defined(MTD_DEBUG)
		if (GsGetMainSysInfo()->debug_flag & GSD_DEBUG_DEBUG_DISP) {
			amDebugDisplayPerformance(AMD_TEXT_WIDTH - 22, 1);
			//amEffectDebugDisplayInfo();
		} else {
			amPrint(0, 0, " ");
		}
		nnFlushPrint();
#else
		// 画面が崩れないようにするためにこの処理が必要
		//nnBeginDrawPrimitive2D(NNE_PRIM2D_FMT_PC, NNE_PRIM_ALPHABLEND_ON);
		//nnEndDrawPrimitive2D();
		glDepthMask(GL_TRUE);
#endif //defined(MTD_DEBUG)
		amDrawEnd();
#if AMD_DEBUG && AMD_DEBUG_FRAME_RATE
		amTimerEnd(&timer);
		amDebugSetPerformanceMain(timer.frame);
		amDebugSetPerformanceDraw(timer.frame);
#endif
	}
	
	//フレームレートの算出
	{
		//過去60fのfpsを調査し、処理落ちしないfpsを調査する
		amTimerEnd(&fps_count_timer);
		float frame = amTimerGetFrame(&fps_count_timer);
		amTimerDelete(&fps_count_timer);
		vfr::l_fps_count_buf.push_back(std::ceil(frame));
	
		int sum = std::accumulate(vfr::l_fps_count_buf.begin(), vfr::l_fps_count_buf.end(), 0);
#if defined(MTD_DEBUG)
		float fps = 60.0f * vfr::l_fps_count_buf.size() / sum;
#endif //defined(MTD_DEBUG)
		int fps_count = 1;
		sum -= vfr::c_fps_count_buf;
		do {
			++fps_count;
			sum -= vfr::c_fps_count_buf;
			if (3 <= fps_count) {
				//20fps以下に鳴る場合は速度90%を保てれば良しとする
				sum -= vfr::c_fps_count_buf / 9;
			}
		} while (0 < sum);
		if (6 < fps_count) {
			//10fps以下には設定しない
			fps_count = 6;
		}
		_am_sample_count = fps_count;
#if defined(MTD_DEBUG)
		if (GsGetMainSysInfo()->debug_flag & GSD_DEBUG_FPS_DISP) {
			amPrintf(0, 1, "%5.1f/%2d", fps, (60/_am_sample_count));
		}
#endif //defined(MTD_DEBUG)
	}

#if AMD_DEBUG
	amTimerDelete(&timer);
#endif
	
	amDrawWaitVSync();
#endif
	// セガロゴのデモ終了タイミング
	if (Sonic4_GetLogoDemoEnd())
	{
		Sonic4_LogoDemoEnd();
		return 1;
	}
	
	return result;
}

/*****************************************************************************/
/* int Sonic4_AppFinish(void)                                                       */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  アプリケーション終了                                          */
/*****************************************************************************/
int Sonic4_AppFinish(void)
{
	finish();
	
	// 変数のクリア
	Sonic4_AppVar::Exit();
	
	return	0;
}

/*****************************************************************************/
/* void Sonic4_AppSuspend(void)                                                      */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  アプリケーション休止                                          */
/*****************************************************************************/
void Sonic4_AppSuspend(void)
{
	// touch cancel(OS3.Xだと着信時にタッチキャンセルイベントが発生しないのでここで対応)
	amIPhoneTouchCanceled(NULL, NULL, NULL);
	
#if APPD_MAIN_CRIAUDIO_INIT_DELAY
	// まだサウンドシステムが生成されていないので終了
	if (!_am_sample_init) {
		return;
	}
#endif // APPD_MAIN_CRIAUDIO_INIT_DELAY
	// サウンドを強制的に音量0にする
	AMS_CRIAUDIO_INTERFACE	*cri_audio_if = amCriAudioGetGlobal();
	
	for (int i = 0; i < AME_CRIAUDIO_CSB_MAX; i++) {
		CriAuPlayer *auply = cri_audio_if->auply[i];
		if (auply != NULL) {
			CriAuPlayer::Status	status	= auply->GetStatus(cri_audio_if->err);
		
			if (CriAuPlayer::STATUS_STOP != status &&
				CriAuPlayer::STATUS_PLAYEND != status) {
				auply->SetVolume(0.0f);
				auply->Update();
			}
		}
	}
	// サウンドを適応させる為に一度だけ更新
	// CRIだけでも大丈夫？
	for (int j = 0; j < 50; j++) {
		amCriAudioExcuteMain();
	}
}

#if AMD_SAMPLE_SAVE_LOG
void AppLog(const char *log_text)
{
	char buffer[256];
	memset(buffer, 0 , sizeof(char) * 256);
	
	time_t now;
	now = time(NULL);
	
	sprintf(buffer, "%s: %s", log_text, ctime(&now));
#if !SONIC4_TRIAL
	FILE *pfile = fopen("/private/var/mobile/Media/DCIM/Log.txt", "ab");
#else //!SONIC4_TRIAL
	FILE *pfile = fopen("/private/var/mobile/Media/DCIM/LogTrial.txt", "ab");
#endif //!SONIC4_TRIAL
	if (pfile != NULL) {
		fwrite(buffer, sizeof(char) * strlen(buffer), 1, pfile);
		fclose(pfile);
	}
}
void AppLogv(const char *log_text, va_list vlist)
{
	char buffer[256] = {};
	vsprintf(buffer, log_text, vlist);
	AppLog(buffer);
}
void AppLogf(const char *log_text, ...)
{
	va_list vlist;
	va_start(vlist, log_text);
	AppLogv(log_text, vlist);
	va_end(vlist);
}
#endif // AMD_SAMPLE_SAVE_LOG

/*--- TCB Functions ---------------------------------------------------------*/
//## TCB Functions

/*--- Local Functions -------------------------------------------------------*/
//## Local Functions

/*****************************************************************************/
/* int init(void)                                                            */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  初期化                                                        */
/*****************************************************************************/
int init(const char* pDocPath)
{
#if AMD_FS_DEVICE_NAME

#if WINDOWS_DATA
	AMD_STRCPY_S(_am_fs_device_name, sizeof(_am_fs_device_name)/sizeof(_am_fs_device_name[0]), "/private/var/mobile/Media/DCIM/");
#else
	AMD_STRCPY_S(_am_fs_device_name, sizeof(_am_fs_device_name)/sizeof(_am_fs_device_name[0]), pDocPath);
#endif

#endif

	initNN();

	// マトリクススタック領域の確保、設定
	_am_default_stack_buf	= amMemAllocSystem(AMD_MATRIX_STACK_SIZE * sizeof(NNS_MATRIX));
	nnSetUpMatrixStack(&_am_default_stack, _am_default_stack_buf, AMD_MATRIX_STACK_SIZE);
#if AMD_USE_DRAW_THREAD
	_am_game_stack_buf		= amMemAllocSystem(AMD_MATRIX_STACK_SIZE * sizeof(NNS_MATRIX));
	nnSetUpMatrixStack(&_am_game_stack, _am_game_stack_buf, AMD_MATRIX_STACK_SIZE);
#endif

	// タスクシステムの初期化
	amTaskInitSystem();

	// 描画システムの初期化
//	amDrawCreateBuffer();		// 引数でバッファサイズを指定
	amDrawCreateBuffer(128 * 1024, 1024 * 1024, 3 * 1024 * 1024);
	amDrawInitDisplayList();

	// パッドの初期化
	amPadInit();
	amTpInit();

	amFsInit(0, 0, AMD_DEVICE_ADDNAME, NULL, 128, 128, 4, NULL);

	amMemInit(AMD_HOG_HEAP_SIZE);
	
#ifndef GMD_DEBUG_NO_CREATE_CRIAUDIO
#if AMD_USE_CRIAUDIO
#if !APPD_MAIN_CRIAUDIO_INIT_DELAY
	amCriAudioInit();
#endif // !APPD_MAIN_CRIAUDIO_INIT_DELAY
#elif AMD_USE_CRIADX

	AMS_ADX_PARAM Param;
	
	memset(&Param, 0, sizeof(AMS_ADX_PARAM));
	Param.handleNum = 2;
	Param.streamNum = 2;
	Param.channelMax = 2;
	Param.frequencyMax = 22050;
	Param.vsync_prio = -1;
	Param.fs_prio = -1;
	Param.idle_prio = -1;
	
	amAdxInitSystem(&Param);
#endif
#endif // GMD_DEBUG_NO_CREATE_CRIAUDIO

#if AMD_USE_DRAW_THREAD
	// ゲームスレッドの作成
	Sint32	prio;
	Uint32	stack_size = 0x10000;
	pthread_t	thread_id;
	int		policy;
	struct sched_param	param;

	thread_id	= pthread_self();
	pthread_getschedparam(thread_id, &policy, &param);
	prio		= param.sched_priority + 20;

	_am_game_thread_id	= amThreadCreate(&_am_game_thread, (void *)game_proc,
			NULL, AMD_CORE_2A, prio, stack_size, "GAME_THREAD");

	// メインタイマーの作成
	amAlarmCreateTimer(&_am_main_timer);

	// 固定
	amAlarmSetTimer(&_am_main_timer, 1000000 / 60);
	amSystemSetFrameRateMain(1.0f);
	
	// CRI Audioスレッドの作成
	prio		= param.sched_priority + 1;
	_am_cri_thread_id	= amThreadCreate(&_am_cri_thread, (void *)cri_proc,
			NULL, AMD_CORE_2A, prio, stack_size, "CRI_THREAD");
#else
#ifndef GMD_DEBUG_NO_CREATE_CRIAUDIO
#if (0) // 有効にすると描画スレッドの負荷が上がる
	Sint32	prio;
	Uint32	stack_size = 0x10000;
	pthread_t	thread_id;
	int		policy;
	struct sched_param	param;
	
	thread_id	= pthread_self();
	pthread_getschedparam(thread_id, &policy, &param);
	prio		= param.sched_priority + 1;
	_am_cri_thread_id	= amThreadCreate(&_am_cri_thread, (void *)cri_proc,
										 NULL, AMD_CORE_2A, prio, stack_size, "CRI_THREAD");
#endif //(0) // 有効にすると描画スレッドの負荷が上がる
#endif // GMD_DEBUG_NO_CREATE_CRIAUDIO
#endif // AMD_USE_DRAW_THREAD

#if !AMD_USE_DRAW_THREAD
#if !APPD_MAIN_CRIAUDIO_INIT_DELAY
	// ユーザー初期化
	GsInitUser();
#endif // !APPD_MAIN_CRIAUDIO_INIT_DELAY
#endif

	return	0;
}


#if AMD_USE_DRAW_THREAD
/*****************************************************************************/
/* void game_proc(int arg)                                                   */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  ゲームスレッド                                                */
/*****************************************************************************/
void game_proc(int arg)
{
	UNREFERENCED_PARAMETER(arg);

	// ユーザー初期化
	GsInitUser();

//#define HOG_CFR_GAME_PROC
#if defined(HOG_CFR_GAME_PROC)
	AMS_ALARM	alarm_cfr;
	amAlarmCreateTimer(&alarm_cfr);
	amAlarmSetTimer(&alarm_cfr, 1000000.0f / 60.0f * 2.0f);
#endif //defined(HOG_CFR_GAME_PROC)

	for (;;) {
		// 終了要求チェック
		if (amThreadCheckExit(&_am_game_thread))
			break;

		amIPhoneMainLoop();

#if defined(HOG_CFR_GAME_PROC)
		amAlarmWaitTimer(&alarm_cfr);
#endif //defined(HOG_CFR_GAME_PROC)
	}

#if defined(HOG_CFR_GAME_PROC)
	amAlarmDelete(&alarm_cfr);
#endif //defined(HOG_CFR_GAME_PROC)

	amThreadQuit(&_am_game_thread);
}
#endif

#if AMD_USE_DRAW_THREAD
/*****************************************************************************/
/* int draw_proc(void)                                                       */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  描画スレッド                                                  */
/*****************************************************************************/
int draw_proc(void)
{
	static Sint32	init_flag = 0;
#if AMD_DEBUG
	static AMS_TIMER	timer;
#endif

	// 終了チェック
	if (_amIPhoneCheckExit()) {
#if AMD_DEBUG
		if (init_flag)
			amTimerDelete(&timer);
#endif
#if AMD_USE_DRAW_THREAD
		amThreadExit(&_am_game_thread);
		amThreadWaitQuit(&_am_game_thread);
#endif
		return 1;
	}

	if (!init_flag) {
		nnInitLight();
#if AMD_DEBUG
		amTimerCreate(&timer);
#endif
		init_flag		= 1;
	}

#if AMD_DEBUG
	amTimerStart(&timer);
#endif

//	amFsServer();

	// 読み出しディスプレイリストの設定
#if !_LIB_DEBUG
	if (amDrawGetDisplayList() != -1) {
#else
	static Sint32 id_old = -2;
	Sint32	id = amDrawGetDisplayList();
	if (id == id_old)
		amSystemLog("Drop frame %d %d\n", id, id_old);
	else if (id != ((id_old + 1) % AMD_DISPLAYLIST_NUM))
		amSystemLog("Skip frame %d %d\n", id, id_old);
	id_old	= id;
	if (id != -1) {
#endif
		amDrawInitState();
		amDrawBeginScene();

		if (amDrawBegin()) {
			// 登録リスト実行
			amDrawExecRegist();

			// タスクの生成
			amDrawExecCommand(AMD_COMMAND_STATE_MAKE_TASK);

			// タスク実行
			amDrawExecute();

			amDrawDisplay();

			// デバッグ表示
			amDrawExecCommand(AMD_COMMAND_STATE_DEBUG);
			amDebugDisplayPerformance(AMD_TEXT_WIDTH - 22, 1);

			// デバッグ文字表示
			amDrawExecCommand(AMD_COMMAND_STATE_DEBUG_PRINT);
#if AMD_DEBUG
			amDebugSetPerformanceDraw(timer.frame);
#if AMD_DEBUG_FRAME_RATE
			nnPrint(7, 1, "%4d", _am_draw_counter++);
#endif
#endif
			nnFlushPrint();

			amDrawEnd();
		}
	}

#if AMD_DEBUG
	amTimerEnd(&timer);
#endif

	return	0;
}
#endif

#if AMD_USE_DRAW_THREAD || _IPHONE
/*****************************************************************************/
/* void cri_proc(int arg)                                                    */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  CRI Audioスレッド                                             */
/*****************************************************************************/
void cri_proc(int arg)
{
	AMS_ALARM	alarm;

	UNREFERENCED_PARAMETER(arg);

	amAlarmCreateTimer(&alarm);
	amAlarmSetTimer(&alarm, 1000000 / 60);

	for (;;) {
		// 終了要求チェック
		if (amThreadCheckExit(&_am_cri_thread))
			break;

		amAlarmWaitTimer(&alarm);

#if AMD_USE_CRIAUDIO
		amCriAudioExcuteMain();
#elif AMD_USE_CRIADX
		amAdxExcuteMain();
#endif // AMD_USE_CRIAUDIO
	}

	amAlarmDelete(&alarm);

	amThreadQuit(&_am_cri_thread);
}
#endif


/*****************************************************************************/
/* void finish(void)                                                         */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  終了                                                          */
/*****************************************************************************/
void finish(void)
{
	// ユーザー終了処理
	GsExitUser();

#if AMD_USE_DRAW_THREAD
	amThreadDelete(&_am_game_thread);
	_am_game_thread_id		= (AMS_THREAD_ID)0;
	amThreadDelete(&_am_cri_thread);
#endif
	amDrawExitDisplayList();
	amDrawDeleteBuffer();
	amPadExit();
	amTaskExitSystem();
#ifndef GMD_DEBUG_NO_CREATE_CRIAUDIO
#if AMD_USE_CRIAUDIO
	amCriAudioExit();
#elif AMD_USE_CRIADX
	amAdxExitSystem();
#endif // AMD_USE_CRIAUDIO
#endif // GMD_DEBUG_NO_CREATE_CRIAUDIO
	amFsExit();
	amMemExit();
	
	exitNN();

#if AMD_USE_DRAW_THREAD
	amAlarmDelete(&_am_main_timer);
#endif

	// デバッグプリント領域の解放
	if (_am_debug_print_buf != NULL)
		amMemFreeSystem(_am_debug_print_buf);

	// マトリクススタック領域の解放
#if AMD_USE_DRAW_THREAD
	amMemFreeSystem(_am_game_stack_buf);
#endif
	amMemFreeSystem(_am_default_stack_buf);
}


/*****************************************************************************/
/* void initNN(void)                                                         */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  NNライブラリの初期化                                          */
/*             Windows版はディスプレイモードが切り替わる毎に呼び出される     */
/*****************************************************************************/
void initNN(void)
{
	// デバッグプリント初期化
	Uint32		size;
	if (_am_debug_print_buf == NULL) {
		size		= nnGetPrintBufferSize(AMD_DEBUG_CHAR_MAX);
		_am_debug_print_buf	= amMemAllocSystem(size);
	}
	nnInitPrint(_am_debug_print_buf, AMD_DEBUG_CHAR_MAX, NULL);
	
}


/*****************************************************************************/
/* void exitNN(void)                                                         */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  NNライブラリの終了                                            */
/*****************************************************************************/
void exitNN(void)
{
	nnExitPrint();
	
	amIPhoneExitNN();
}
