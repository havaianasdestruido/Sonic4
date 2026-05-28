/*****************************************************************************/
/*      amThread.h                  Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* スレッドライブラリヘッダ                                                  */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090323-       0.01   first version                                        */
/*****************************************************************************/

#ifndef _AM_THREAD_H
#define _AM_THREAD_H

/*--- Include Files (Pre Definitions) ---------------------------------------*/

#if _PS3
#include <sys/ppu_thread.h>
#include <sys/synchronization.h>
#elif _WII
#include <revolution/os.h>
#elif _IPHONE
#include <pthread.h>
#endif


/*--- Definitions -----------------------------------------------------------*/

#if !AMD_DEBUG
#define amThreadCheckSafe(...)	(void)0
#endif

typedef enum {
	AMD_CORE_1A = 0,
	AMD_CORE_1B,
	AMD_CORE_2A,
	AMD_CORE_2B,
	AMD_CORE_3A,
	AMD_CORE_3B,
} AMD_CORE;

// ミューテックス
#if _PC
typedef CRITICAL_SECTION		AMS_MUTEX;
#elif _XBOX
typedef CRITICAL_SECTION		AMS_MUTEX;
#elif _PS3
typedef sys_lwmutex_t			AMS_MUTEX;
#elif _WII
typedef OSMutex					AMS_MUTEX;
#elif _IPHONE
typedef pthread_mutex_t			AMS_MUTEX;
#endif

// スレッドID
#if _PC
typedef DWORD					AMS_THREAD_ID;
#elif _XBOX
typedef DWORD					AMS_THREAD_ID;
#elif _PS3
typedef sys_ppu_thread_t		AMS_THREAD_ID;
#elif _WII
typedef OSThread*				AMS_THREAD_ID;
#elif _IPHONE
typedef pthread_t				AMS_THREAD_ID;
#endif

#include "amTimer.h"

// スレッド構造体
typedef struct {
#if _PC
	AMS_THREAD_ID	thread_id;	// スレッドID
	HANDLE		handle;			// スレッドハンドル
	AMS_ALARM	alarm_exit;		// 終了要求アラーム
	AMS_MUTEX	mutex;			// 描画ミューテックス
#elif _XBOX
	AMS_THREAD_ID	thread_id;	// スレッドID
	HANDLE		handle;			// スレッドハンドル
	AMS_ALARM	alarm_exit;		// 終了要求アラーム
	HANDLE		event_exit;		// 終了要求イベントハンドル
#elif _PS3
	AMS_THREAD_ID	thread_id;	// スレッドID
	AMS_ALARM	alarm_exit;		// 終了要求アラーム
	AMS_ALARM	alarm_quit;		// 終了通知アラーム
#elif _WII
	AMS_THREAD_ID	thread_id;	// スレッドID
	OSThread	thread;			// スレッド
	AMS_ALARM	alarm_exit;		// 終了要求アラーム
	void		*stack;			// スタックバッファ
#elif _IPHONE
	AMS_THREAD_ID	thread_id;	// スレッドID
	AMS_ALARM	alarm_exit;		// 終了要求アラーム
	AMS_ALARM	alarm_quit;		// 終了通知アラーム
	void		*stack;			// スタックバッファ
#endif
	void		*arg;			// 引数
} AMS_THREAD;


/*--- Macros ----------------------------------------------------------------*/

/*--- Include Files (Post Definitions) --------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/

/*--- External Functions ----------------------------------------------------*/

/*****************************************************************************/
/* AMS_THREAD_ID amThreadCreate(AMS_THREAD *thread, void *proc, void *arg,   */
/*         AMD_CORE core, Sint32 prio, size_t stack_size, const char *name)  */
/*---------------------------------------------------------------------------*/
/* [INPUT]  proc : エントリ関数のポインタ                                    */
/*          arg  : スレッド構造体に格納される引数                            */
/*          core : 実行コア（_XBOXのみ有効）                                 */
/*          prio : スレッドプライオリティ（機種によって指定方法が異なる）    */
/*          stack_size : スタックサイズ                                      */
/*          name : スレッド名（PS3のみ有効、デバッガで使用する）             */
/* [OUTPUT] thread : スレッド構造体                                          */
/* [RETURN] スレッドID                                                       */
/* [FUNCTION]  スレッドの作成                                                */
/*****************************************************************************/
AMS_THREAD_ID amThreadCreate(AMS_THREAD *thread, void *proc, void *arg,
		AMD_CORE core, Sint32 prio, size_t stack_size, const char *name = NULL);

/*****************************************************************************/
/* void amThreadOpen(AMS_THREAD *thread)                                     */
/*---------------------------------------------------------------------------*/
/* [INPUT]  thread : スレッド構造体                                          */
/* [FUNCTION]  スレッドの開始(子スレッド側)                                  */
/*****************************************************************************/
void amThreadOpen(AMS_THREAD *thread);

/*****************************************************************************/
/* Sint32 amThreadExit(AMS_THREAD *thread)                                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]  thread : スレッド構造体                                          */
/* [RETURN] スレッドを終了できなかったら0                                    */
/* [FUNCTION]  スレッドの終了(親スレッド側)                                  */
/*****************************************************************************/
Sint32 amThreadExit(AMS_THREAD *thread);

/*****************************************************************************/
/* Sint32 amThreadCheckExit(AMS_THREAD *thread)                              */
/*---------------------------------------------------------------------------*/
/* [INPUT]  thread : スレッド構造体                                          */
/* [RETURN] スレッド終了要求がでていなかったら0                              */
/* [FUNCTION]  スレッドの終了確認(子スレッド側)                              */
/*****************************************************************************/
Sint32 amThreadCheckExit(AMS_THREAD *thread);

/*****************************************************************************/
/* void amThreadQuit(AMS_THREAD *thread)                                     */
/*---------------------------------------------------------------------------*/
/* [INPUT]  thread : スレッド構造体                                          */
/* [FUNCTION]  スレッドの終了通知(子スレッド側)                              */
/*****************************************************************************/
void amThreadQuit(AMS_THREAD *thread);

/*****************************************************************************/
/* Sint32 amThreadCheckQuit(AMS_THREAD *thread)                              */
/*---------------------------------------------------------------------------*/
/* [INPUT]  thread : スレッド構造体                                          */
/* [RETURN] スレッドが終了していなかったら0                                  */
/* [FUNCTION]  スレッドの終了確認(親スレッド側)                              */
/*****************************************************************************/
Sint32 amThreadCheckQuit(AMS_THREAD *thread);

/*****************************************************************************/
/* void amThreadWaitQuit(AMS_THREAD *thread)                                 */
/*---------------------------------------------------------------------------*/
/* [INPUT]  thread : スレッド構造体                                          */
/* [FUNCTION]  スレッドの終了待ち(親スレッド側)                              */
/*****************************************************************************/
void amThreadWaitQuit(AMS_THREAD *thread);

/*****************************************************************************/
/* void amThreadDelete(AMS_THREAD *thread)                                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]  thread : スレッド構造体                                          */
/* [FUNCTION]  スレッドの削除(親スレッド側)                                  */
/*****************************************************************************/
void amThreadDelete(AMS_THREAD *thread);

/*****************************************************************************/
/* BOOL amThreadCheckDraw(BOOL default_thread)                               */
/*---------------------------------------------------------------------------*/
/* [INPUT]  default_thread : 描画スレッドがない場合に返される値              */
/*                           シングルスレッド動作時は必ずTRUEが返る          */
/* [RETURN]  描画スレッドかどうか                                            */
/* [FUNCTION]  描画スレッドかどうかを調べる                                  */
/*             NNの関数を呼び出していいかどうかの判定に使う                  */
/*****************************************************************************/
BOOL amThreadCheckDraw(BOOL default_thread = 0);

/*****************************************************************************/
/* AMS_THREAD_ID amThreadGetCurrentID(void)                                  */
/*---------------------------------------------------------------------------*/
/* [RETURN]  現在のスレッドID                                                */
/* [FUNCTION]  現在のスレッドIDを取得する                                    */
/*****************************************************************************/
inline AMS_THREAD_ID amThreadGetCurrentID(void)
{
#if _PC
	return	GetCurrentThreadId();
#elif _XBOX
	return	GetCurrentThreadId();
#elif _PS3
	sys_ppu_thread_t	thread_id;
	sys_ppu_thread_get_id(&thread_id);
	return	thread_id;
#elif _WII
	return	OSGetCurrentThread();
#elif _IPHONE
	return	pthread_self();
#endif
}

/*****************************************************************************/
/* void amThreadCheckSafe(Sint32 mode, char *fname)                          */
/*---------------------------------------------------------------------------*/
/* [INPUT] mode : スレッドセーフなスレッド                                   */
/*                0 : メインスレッド                                         */
/*                1 : 描画スレッド                                           */
/* [FUNCTION]  任意のスレッドかどうかを調べる                                */
/*             違った場合はログを出力してアサート                            */
/*****************************************************************************/
#if AMD_DEBUG && AMD_USE_DRAW_THREAD
void amThreadCheckSafe(Sint32 mode, char *fname);
#else
#define amThreadCheckSafe(...)		(void)0
#endif

// 描画スレッドが無かった場合の判定
#define AMD_THREAD_SAFE_NO_THREAD_MAIN		(0 << 1)
#define AMD_THREAD_SAFE_NO_THREAD_DRAW		(1 << 1)


/*****************************************************************************/
/* void amThreadSleep(Sint32 msec)                                           */
/*---------------------------------------------------------------------------*/
/* [INPUT]  msec : スリープする時間(ミリ秒)                                  */
/* [FUNCTION]  カレントスレッドのスリープ                                    */
/*****************************************************************************/
void amThreadSleep(Sint32 msec);

/*****************************************************************************/
/* void amMutexCreate(AMS_MUTEX *mutex)                                      */
/*---------------------------------------------------------------------------*/
/* [OUTPUT]  ミューテックス構造体                                            */
/* [FUNCTION]  ミューテックスの作成                                          */
/*****************************************************************************/
void amMutexCreate(AMS_MUTEX *mutex);

/*****************************************************************************/
/* Sint32 amMutexDelete(AMS_MUTEX *mutex)                                    */
/*---------------------------------------------------------------------------*/
/* [INPUT]  mutex : ミューテックス構造体                                     */
/* [RETURN] 削除できなかったら0                                              */
/* [FUNCTION]  ミューテックスの削除                                          */
/*****************************************************************************/
Sint32 amMutexDelete(AMS_MUTEX *mutex);

/*****************************************************************************/
/* void amMutexLock(AMS_MUTEX *mutex)                                        */
/*---------------------------------------------------------------------------*/
/* [INPUT]  mutex : ミューテックス構造体                                     */
/* [FUNCTION]  ミューテックスロック                                          */
/*****************************************************************************/
void amMutexLock(AMS_MUTEX *mutex);

/*****************************************************************************/
/* Sint32 amMutexTrylock(AMS_MUTEX *mutex)                                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]  mutex : ミューテックス構造体                                     */
/* [RETURN] ロックできなかったら0                                            */
/* [FUNCTION]  ミューテックスロック試行                                      */
/*****************************************************************************/
Sint32 amMutexTrylock(AMS_MUTEX *mutex);

/*****************************************************************************/
/* void amMutexUnlock(AMS_MUTEX *mutex)                                      */
/*---------------------------------------------------------------------------*/
/* [INPUT]  mutex : ミューテックス構造体                                     */
/* [FUNCTION]  ミューテックスアンロック                                      */
/*****************************************************************************/
void amMutexUnlock(AMS_MUTEX *mutex);


#endif
