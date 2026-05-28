/*****************************************************************************/
/*      amThread.cpp                Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* スレッドライブラリプログラム                                              */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090323-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files ---------------------------------------------------------*/

#include "alice.h"

#if _IPHONE
#include <sys/time.h>
#include <errno.h>
#endif


/*--- Macros ----------------------------------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

#if _PS3
typedef void (*AMF_THREAD_PROC)(uint64_t);
#elif _WII
typedef void *(*AMF_THREAD_PROC)(void *);
#elif _IPHONE
typedef void *(*AMF_THREAD_PROC)(void *);
#endif


/*--- External Valiables ----------------------------------------------------*/

/*--- Global Variables ------------------------------------------------------*/

/*--- Local Variables -------------------------------------------------------*/

/*--- Global Functions ------------------------------------------------------*/

/*****************************************************************************/
/* AMS_THREAD_ID amThreadCreate(AMS_THREAD *thread, void *proc, void *arg,   */
/*         AMD_CORE core, Sint32 prio, size_t stack_size, const char *name)  */
/*---------------------------------------------------------------------------*/
/* [INPUT]  proc : エントリ関数のポインタ                                    */
/*          arg  : スレッド構造体に格納される引数                            */
/*          core : 実行コア（XBOXのみ有効）                                  */
/*          prio : スレッドプライオリティ（機種によって指定方法が異なる）    */
/*          stack_size : スタックサイズ                                      */
/*          name : スレッド名（PS3/PSPのみ有効、デバッガで使用する）         */
/* [OUTPUT] thread : スレッド構造体                                          */
/* [RETURN] スレッドID                                                       */
/* [FUNCTION]  スレッドの作成                                                */
/*****************************************************************************/
AMS_THREAD_ID amThreadCreate(AMS_THREAD *thread, void *proc, void *arg, AMD_CORE core, Sint32 prio, size_t stack_size, const char *name)
{
	AMS_THREAD_ID	ret = 0;

	amAssert(thread);
	amAssert(proc);

#if _PC
	UNREFERENCED_PARAMETER(core);
	UNREFERENCED_PARAMETER(name);

	thread->handle	= CreateThread(NULL, stack_size, (LPTHREAD_START_ROUTINE)proc,
			(LPVOID)thread, CREATE_SUSPENDED, &thread->thread_id);
	if (thread->handle != NULL) {
		ret		= thread->thread_id;
		amAlarmCreate(&thread->alarm_exit);
		amMutexCreate(&thread->mutex);
		thread->arg		= arg;
		SetThreadPriority(thread->handle, prio);
		ResumeThread(thread->handle);
	}
#elif _XBOX
	UNREFERENCED_PARAMETER(name);

	thread->handle	= CreateThread(NULL, stack_size, (LPTHREAD_START_ROUTINE)proc,
			(LPVOID)thread, CREATE_SUSPENDED, &thread->thread_id);
	if (thread->handle != NULL) {
		ret		= thread->thread_id;
		amAlarmCreate(&thread->alarm_exit);
		thread->event_exit	= NULL;
		thread->arg		= arg;
		XSetThreadProcessor(thread->handle, core);
		SetThreadPriority(thread->handle, prio);
		ResumeThread(thread->handle);
	}
#elif _PS3
	UNREFERENCED_PARAMETER(core);

//	AMF_THREAD_PROC		entry = reinterpret_cast<AMF_THREAD_PROC>proc;
	AMF_THREAD_PROC		entry = (AMF_THREAD_PROC)proc;

	thread->arg		= arg;
	amAlarmCreate(&thread->alarm_exit);
	amAlarmCreate(&thread->alarm_quit);
	if (sys_ppu_thread_create(&thread->thread_id, entry,
			(uint64_t)thread, prio, stack_size,
			SYS_PPU_THREAD_CREATE_JOINABLE, name) == CELL_OK) {
		ret		= thread->thread_id;
	} else {
		amAlarmDelete(&thread->alarm_exit);
		amAlarmDelete(&thread->alarm_quit);
	}
#elif _WII
	UNREFERENCED_PARAMETER(core);
	UNREFERENCED_PARAMETER(name);

	AMF_THREAD_PROC		entry = (AMF_THREAD_PROC)proc;

	thread->stack	= amMemAllocSystem(stack_size);

	if (OSCreateThread(&thread->thread, entry, (void *)thread,
			(void *)((Uint8 *)thread->stack + stack_size),
			stack_size, prio, /* OS_THREAD_ATTR_DETACH */ 0)) {
		thread->thread_id	= &thread->thread;
		ret		= thread->thread_id;
		amAlarmCreate(&thread->alarm_exit);
		thread->arg		= arg;
		OSResumeThread(&thread->thread);
	} else
		amMemFreeSystem(thread->stack);
#elif _IPHONE
	UNREFERENCED_PARAMETER(core);
	UNREFERENCED_PARAMETER(name);

	AMF_THREAD_PROC		entry = (AMF_THREAD_PROC)proc;

	thread->stack	= amMemAllocSystem(stack_size);

	pthread_t	thread_id;
	int		policy;
	struct sched_param	param;

	thread_id	= pthread_self();
	pthread_getschedparam(thread_id, &policy, &param);

	pthread_attr_t		attr;

	pthread_attr_init(&attr);
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr, policy);

	struct sched_param	priority;
	priority.sched_priority		= prio;
	pthread_attr_setschedparam(&attr, &priority);

	pthread_attr_setstack(&attr, thread->stack, stack_size);

	thread->arg		= arg;
	amAlarmCreate(&thread->alarm_exit);
	amAlarmCreate(&thread->alarm_quit);
	if (!pthread_create(&thread->thread_id, &attr, entry, (void *)thread)) {
		ret		= thread->thread_id;
	} else {
		amAlarmDelete(&thread->alarm_exit);
		amAlarmDelete(&thread->alarm_quit);
	}
#endif

	return	ret;
}


/*****************************************************************************/
/* void amThreadOpen(AMS_THREAD *thread)                                     */
/*---------------------------------------------------------------------------*/
/* [INPUT]  thread : スレッド構造体                                          */
/* [FUNCTION]  スレッドの開始(子スレッド側)                                  */
/*****************************************************************************/
void amThreadOpen(AMS_THREAD *thread)
{
	UNREFERENCED_PARAMETER(thread);

#if _PC	
#elif _XBOX
	amAssert(thread);
	if (thread->event_exit == NULL) {
		char	tname[MAX_PATH];
		sprintf_s(tname, MAX_PATH, "%d", thread->alarm_exit.alarm_id);
		thread->event_exit	= OpenEvent(0, FALSE, tname);
	}
#elif _PS3
#elif _WII
#elif _IPHONE
#endif
}


/*****************************************************************************/
/* Sint32 amThreadExit(AMS_THREAD *thread)                                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]  thread : スレッド構造体                                          */
/* [RETURN] スレッドを終了できなかったら0                                    */
/* [FUNCTION]  スレッドの終了(親スレッド側)                                  */
/*****************************************************************************/
Sint32 amThreadExit(AMS_THREAD *thread)
{
	amAssert(thread);

	amAlarmSet(&thread->alarm_exit);

	return	1;
}


/*****************************************************************************/
/* Sint32 amThreadCheckExit(AMS_THREAD *thread)                              */
/*---------------------------------------------------------------------------*/
/* [INPUT]  thread : スレッド構造体                                          */
/* [RETURN] スレッド終了要求がでていなかったら0                              */
/* [FUNCTION]  スレッドの終了確認(子スレッド側)                              */
/*****************************************************************************/
Sint32 amThreadCheckExit(AMS_THREAD *thread)
{
	Sint32		ret;

	amAssert(thread);

#if _PC
	ret		= amAlarmCheck(&thread->alarm_exit);
#elif _XBOX
	if (thread->event_exit == NULL)
		return	0;

	ret		= WaitForSingleObject(thread->event_exit, 0);
	ret		= (ret == WAIT_OBJECT_0)? 1: 0;
#elif _PS3
	ret		= amAlarmCheck(&thread->alarm_exit);
#elif _WII
	ret		= amAlarmCheck(&thread->alarm_exit);
#elif _IPHONE
	ret		= amAlarmCheck(&thread->alarm_exit);
#endif

	return	ret;
}


/*****************************************************************************/
/* void amThreadQuit(AMS_THREAD *thread)                                     */
/*---------------------------------------------------------------------------*/
/* [INPUT]  thread : スレッド構造体                                          */
/* [FUNCTION]  スレッドの終了通知(子スレッド側)                              */
/*****************************************************************************/
void amThreadQuit(AMS_THREAD *thread)
{
	amAssert(thread);

#if _PC | _XBOX
	UNREFERENCED_PARAMETER(thread);
	ExitThread(0);
#elif _PS3
	amAlarmSet(&thread->alarm_quit);
	sys_ppu_thread_exit(0);
#elif _WII
	UNREFERENCED_PARAMETER(thread);
	OSExitThread(NULL);
#elif _IPHONE
	amAlarmSet(&thread->alarm_quit);
	pthread_exit(NULL);
#endif
}


/*****************************************************************************/
/* Sint32 amThreadCheckQuit(AMS_THREAD *thread)                              */
/*---------------------------------------------------------------------------*/
/* [INPUT]  thread : スレッド構造体                                          */
/* [RETURN] スレッドが終了していなかったら0                                  */
/* [FUNCTION]  スレッドの終了確認(親スレッド側)                              */
/*****************************************************************************/
Sint32 amThreadCheckQuit(AMS_THREAD *thread)
{
	amAssert(thread);

#if _PC | _XBOX
	if (WaitForSingleObject(thread->handle, 0))
		return	0;
#elif _PS3
	if (!amAlarmCheck(&thread->alarm_quit))
		return	0;
#elif _WII
	if (!OSIsThreadTerminated(thread->thread_id))
		return	0;
	OSJoinThread(thread->thread_id, NULL);
#elif _IPHONE
	if (!amAlarmCheck(&thread->alarm_quit))
		return	0;
#endif

	return	1;
}


/*****************************************************************************/
/* void amThreadWaitQuit(AMS_THREAD *thread)                                 */
/*---------------------------------------------------------------------------*/
/* [INPUT]  thread : スレッド構造体                                          */
/* [FUNCTION]  スレッドの終了待ち(親スレッド側)                              */
/*****************************************************************************/
void amThreadWaitQuit(AMS_THREAD *thread)
{
	amAssert(thread);

#if _PC | _XBOX
	WaitForSingleObject(thread->handle, INFINITE);
#elif _PS3
	uint64_t	vptr;
	sys_ppu_thread_join(thread->thread_id, &vptr);
#elif _WII
	OSJoinThread(thread->thread_id, NULL);
#elif _IHPNE
	pthread_join(thread->thread_id);
#endif
}


/*****************************************************************************/
/* void amThreadDelete(AMS_THREAD *thread)                                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]  thread : スレッド構造体                                          */
/* [FUNCTION]  スレッドの削除(親スレッド側)                                  */
/*****************************************************************************/
void amThreadDelete(AMS_THREAD *thread)
{
	amAssert(thread);

#if _PC
	amMutexDelete(&thread->mutex);
#elif _XBOX
	CloseHandle(thread->event_exit);
	thread->event_exit	= NULL;
#elif _PS3
	sys_ppu_thread_detach(thread->thread_id);
	amAlarmDelete(&thread->alarm_quit);
#elif _WII
//	OSDetachThread(thread->thread_id);
	if (thread->stack != NULL) {
		amMemFreeSystem(thread->stack);
		thread->stack	= NULL;
	}
#elif _IPHONE
	pthread_detach(thread->thread_id);
	if (thread->stack != NULL) {
		amMemFreeSystem(thread->stack);
		thread->stack	= NULL;
	}
	amAlarmDelete(&thread->alarm_quit);
#endif
	amAlarmDelete(&thread->alarm_exit);
}


/*****************************************************************************/
/* BOOL amThreadCheckDraw(BOOL default_thread)                               */
/*---------------------------------------------------------------------------*/
/* [INPUT]  default_thread : 描画スレッドがない場合に返される値              */
/*                           シングルスレッド動作時は必ずTRUEが返る          */
/* [RETURN]  描画スレッドかどうか                                            */
/* [FUNCTION]  描画スレッドかどうかを調べる                                  */
/*             NNの関数を呼び出していいかどうかの判定に使う                  */
/*****************************************************************************/
BOOL amThreadCheckDraw(BOOL default_thread)
{
#if !AMD_USE_DRAW_THREAD
	return	1;
#else
#if !_IPHONE
	if ((Uint32)_am_draw_thread_id == 0)
#else
	if ((Uint32)_am_game_thread_id == 0)
#endif
		return	default_thread;
#if _PC | _XBOX
	if (GetCurrentThreadId() == _am_draw_thread_id)
		return	1;
#elif _PS3
	sys_ppu_thread_t	thread_id;
	if (sys_ppu_thread_get_id(&thread_id) == CELL_OK) {
		if (thread_id == _am_draw_thread_id)
			return	1;
	}
#elif _WII
	if (OSGetCurrentThread() == _am_draw_thread.thread_id)
		return	1;
#elif _IPHONE
	pthread_t	thread_id;
	thread_id	= pthread_self();
	if (thread_id != _am_game_thread.thread_id)
		return	1;
#endif
	return	0;
#endif
}


#if AMD_DEBUG && AMD_USE_DRAW_THREAD
/*****************************************************************************/
/* void amThreadCheckSafe(Sint32 mode, char *fname)                          */
/*---------------------------------------------------------------------------*/
/* [INPUT] mode : スレッドセーフなスレッド                                   */
/*                0 : メインスレッド                                         */
/*                1 : 描画スレッド                                           */
/* [FUNCTION]  任意のスレッドかどうかを調べる                                */
/*             違った場合はログを出力してアサート                            */
/*****************************************************************************/
void amThreadCheckSafe(Sint32 mode, char *fname)
{
	Sint32	flag = 0;

	flag	= amThreadCheckDraw((mode >> 1) & 1);

	if ((mode & 1) ^ flag) {
		if (!mode)
			amSystemLog("[ERR] %s is called from DRAW_THREAD!!\n", fname);
		else
			amSystemLog("[ERR] %s is called from MAIN_THREAD!!\n", fname);
		amAssert(0);
	}
}
#endif


/*****************************************************************************/
/* void amThreadSleep(Sint32 msec)                                           */
/*---------------------------------------------------------------------------*/
/* [INPUT]  msec : スリープする時間(ミリ秒)                                  */
/* [FUNCTION]  カレントスレッドのスリープ                                    */
/*****************************************************************************/
void amThreadSleep(Sint32 msec)
{
#if _PC | _XBOX
	Sleep(msec);
#elif _PS3
	sys_timer_usleep(msec * 1000);
#elif _WII
	OSSleepMilliseconds(msec);
#elif _IPHONE
	struct timespec		req, rem;
	req.tv_sec		= msec / 1000;
	req.tv_nsec		= (msec % 1000) * 1000000;
	rem.tv_sec		= 0;
	rem.tv_nsec		= 0;
	while (nanosleep(&req, &rem)) {
		if (errno == EINTR) {
			req.tv_sec		= rem.tv_sec;
			req.tv_nsec		= rem.tv_nsec;
		} else {
			break;		// error
		}
	}
#endif
}


/*****************************************************************************/
/* void amMutexCreate(AMS_MUTEX *mutex)                                      */
/*---------------------------------------------------------------------------*/
/* [OUTPUT]  ミューテックス構造体                                            */
/* [FUNCTION]  ミューテックスの作成                                          */
/*****************************************************************************/
void amMutexCreate(AMS_MUTEX *mutex)
{
	amAssert(mutex);

#if _PC
	InitializeCriticalSection(mutex);
#elif _XBOX
	InitializeCriticalSection(mutex);
#elif _PS3
	sys_lwmutex_attribute_t		attr;
	sys_lwmutex_attribute_initialize(attr);
	sys_lwmutex_create(mutex, &attr);
#elif _WII
	OSInitMutex(mutex);
#elif _IPHONE
	pthread_mutexattr_t		attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(mutex, &attr);
#endif
}


/*****************************************************************************/
/* Sint32 amMutexDelete(AMS_MUTEX *mutex)                                    */
/*---------------------------------------------------------------------------*/
/* [INPUT]  mutex : ミューテックス構造体                                     */
/* [RETURN] 削除できなかったら0                                              */
/* [FUNCTION]  ミューテックスの削除                                          */
/*****************************************************************************/
Sint32 amMutexDelete(AMS_MUTEX *mutex)
{
	Sint32		ret = 1;

	amAssert(mutex);

#if _PC
	DeleteCriticalSection(mutex);
#elif _XBOX
	DeleteCriticalSection(mutex);
#elif _PS3
	ret		= !sys_lwmutex_destroy(mutex);
#elif _WII
	OSInitMutex(mutex);		// 使用中ならエラーでとまる
#elif _IPHONE
	pthread_mutex_destroy(mutex);
#endif

	return	ret;
}


/*****************************************************************************/
/* void amMutexLock(AMS_MUTEX *mutex)                                        */
/*---------------------------------------------------------------------------*/
/* [INPUT]  mutex : ミューテックス構造体                                     */
/* [FUNCTION]  ミューテックスロック                                          */
/*****************************************************************************/
void amMutexLock(AMS_MUTEX *mutex)
{
	amAssert(mutex);

#if _PC
	EnterCriticalSection(mutex);
#elif _XBOX
	EnterCriticalSection(mutex);
#elif _PS3
	sys_lwmutex_lock(mutex, 0);
#elif _WII
	OSLockMutex(mutex);
#elif _IPHONE
	pthread_mutex_lock(mutex);
#endif
}


/*****************************************************************************/
/* Sint32 amMutexTrylock(AMS_MUTEX *mutex)                                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]  mutex : ミューテックス構造体                                     */
/* [RETURN] ロックできなかったら0                                            */
/* [FUNCTION]  ミューテックスロック試行                                      */
/*****************************************************************************/
Sint32 amMutexTrylock(AMS_MUTEX *mutex)
{
	Sint32		ret;

	amAssert(mutex);

#if _PC
	ret		= TryEnterCriticalSection(mutex);
#elif _XBOX
	ret		= TryEnterCriticalSection(mutex);
#elif _PS3
	ret		= !sys_lwmutex_trylock(mutex);
#elif _WII
	ret		= OSTryLockMutex(mutex);
#elif _IPHONE
	ret		= !pthread_mutex_trylock(mutex);
#endif

	return	ret;
}


/*****************************************************************************/
/* void amMutexUnlock(AMS_MUTEX *mutex)                                      */
/*---------------------------------------------------------------------------*/
/* [INPUT]  mutex : ミューテックス構造体                                     */
/* [FUNCTION]  ミューテックスアンロック                                      */
/*****************************************************************************/
void amMutexUnlock(AMS_MUTEX *mutex)
{
	amAssert(mutex);

#if _PC
	LeaveCriticalSection(mutex);
#elif _XBOX
	LeaveCriticalSection(mutex);
#elif _PS3
	sys_lwmutex_unlock(mutex);
#elif _WII
	OSUnlockMutex(mutex);
#elif _IPHONE
	pthread_mutex_unlock(mutex);
#endif
}


/*--- Local Functions -------------------------------------------------------*/

