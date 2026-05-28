/*****************************************************************************/
/*      amTimer.cpp                 Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* タイマーライブラリプログラム                                              */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090318-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files ---------------------------------------------------------*/

#include "alice.h"

#if _IPHONE
#include <sys/time.h>
#include <errno.h>
#endif

/*--- Macros ----------------------------------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

#if _WII
void amAlarmHandler(OSAlarm *alarm0, OSContext *context);
#endif
#if _IPHONE
//void amAlarmHandler(int signal_id, siginfo_t *info, void *context);
void amAlarmHandler(int signal_id);
#endif


/*--- External Valiables ----------------------------------------------------*/

/*--- Global Variables ------------------------------------------------------*/

/*--- Local Variables -------------------------------------------------------*/

#if !_IPHONE
static Uint32		_am_timer_id = 0x11234567ULL;
#else
static Uint32		_am_timer_id = 0x11234567ULL;
//static Uint32		_am_timer_id = SIGRTMIN + 1;
AMS_ALARM			*_am_alarm_timer = NULL;
#endif


/*--- Global Functions ------------------------------------------------------*/

/*****************************************************************************/
/* AMS_TIMER *amTimerCreate(AMS_TIMER *timer)                                */
/*---------------------------------------------------------------------------*/
/* [INPUT]  timer : 確保済みのタイマー構造体                                 */
/* [RETURN]  タイマー構造体                                                  */
/* [FUNCTION]  タイマーの作成                                                */
/*****************************************************************************/
AMS_TIMER *amTimerCreate(AMS_TIMER *timer)
{
	if (timer == NULL) {
		timer	= (AMS_TIMER *)amMemAllocSystem(sizeof(AMS_TIMER));
		amAssert(timer);
		memset(timer, 0, sizeof(AMS_TIMER));
		timer->delete_flag	= 1;
	} else
		memset(timer, 0, sizeof(AMS_TIMER));

#if _PC
	LARGE_INTEGER	li;
	QueryPerformanceFrequency(&li);
	timer->count_freq	= (float)li.QuadPart;
	timer->count_end	= 0;
#elif _XBOX
	LARGE_INTEGER	li;
	QueryPerformanceFrequency(&li);
	timer->count_freq	= (float)li.QuadPart;
#elif _PS3
	timer->count_freq	= (float)1000000;
#elif _WII
	timer->count_freq	= (float)OS_TIMER_CLOCK;
#elif _IPHONE
	timer->count_freq	= (float)1000000;
#endif

	return	timer;
}


/*****************************************************************************/
/* void amTimerDelete(AMS_TIMER *timer)                                      */
/*---------------------------------------------------------------------------*/
/* [RETURN]  タイマー構造体                                                  */
/* [FUNCTION]  タイマーの削除                                                */
/*****************************************************************************/
void amTimerDelete(AMS_TIMER *timer)
{
	amAssert(timer);

	if (timer->delete_flag)
		amMemFreeSystem(timer);
	else
		memset(timer, 0, sizeof(AMS_TIMER));
}


/*****************************************************************************/
/* void amTimerStart(AMS_TIMER *timer)                                       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  timer : タイマー構造体                                           */
/* [FUNCTION]  タイマー計測の開始                                            */
/*****************************************************************************/
void amTimerStart(AMS_TIMER *timer)
{
#if _PC | _XBOX
	QueryPerformanceCounter((LARGE_INTEGER *)&timer->count_start);
#elif _PS3
	timer->count_start		= (Uint64)sys_time_get_system_time();
#elif _WII
	timer->count_start		= (Uint64)OSGetTime();
#elif _IPHONE
	struct timeval		tv;
	timerclear(&tv);
	gettimeofday(&tv, NULL);
	timer->count_start		= (Uint64)tv.tv_sec * 1000000 + (Uint64)tv.tv_usec;
#endif
}


/*****************************************************************************/
/* void amTimerEnd(AMS_TIMER *timer, Sint32 reset)                           */
/*---------------------------------------------------------------------------*/
/* [INPUT]  timer : タイマー構造体                                           */
/*          reset : タイマーをリセットするかどうか                           */
/* [FUNCTION]  タイマー計測の終了                                            */
/*****************************************************************************/
void amTimerEnd(AMS_TIMER *timer, Sint32 reset)
{
	float	count;

#if _PC | _XBOX
	QueryPerformanceCounter((LARGE_INTEGER *)&timer->count_end);
#elif _PS3
	timer->count_end		= (Uint64)sys_time_get_system_time();
#elif _WII
	timer->count_end		= (Uint64)OSGetTime();
#elif _IPHONE
	struct timeval		tv;
	timerclear(&tv);
	gettimeofday(&tv, NULL);
	timer->count_end		= (Uint64)tv.tv_sec * 1000000 + (Uint64)tv.tv_usec;
#endif

	count		= (float)(timer->count_end - timer->count_start);
	timer->usec		= count * 1000000.0f / timer->count_freq;
	timer->msec		= count * 1000.0f / timer->count_freq;
	timer->frame	= count * 60.0f / timer->count_freq;

	if (reset)
		timer->count_start		= timer->count_end;
}


float amTimerCalcFrame(Uint64 count_start, Uint64 count_end, AMS_TIMER *timer)
{
	float	count;

	count	= (float)(count_end - count_start) * 60.0f / timer->count_freq;

	return	count;
}


/*****************************************************************************/
/* AMS_ALARM *amAlarmCreateTimer(AMS_ALARM *alarm)                           */
/*---------------------------------------------------------------------------*/
/* [INPUT]  alarm : 確保済みの周期タイマー構造体                             */
/* [RETURN]  周期タイマー構造体                                              */
/* [FUNCTION]  周期タイマーの作成                                            */
/*****************************************************************************/
AMS_ALARM *amAlarmCreateTimer(AMS_ALARM *alarm)
{
	Sint32	delete_flag = (alarm == NULL)? 1: 0;

	if (alarm == NULL)
		alarm	= (AMS_ALARM *)amMemAllocSystem(sizeof(AMS_ALARM));

	amAssert(alarm);

	memset(alarm, 0, sizeof(AMS_ALARM));
	alarm->alarm_id			= _am_timer_id;
	alarm->delete_flag		= delete_flag;

#if _PC | _XBOX
#if AMD_USE_PERFORMANCE_COUNTER
	LARGE_INTEGER	li;
	QueryPerformanceFrequency(&li);
	alarm->count_freq	= (float)li.QuadPart;
	alarm->handle	= CreateEvent(NULL, FALSE, FALSE, NULL);
	amMutexCreate(&alarm->mutex);
#else
	alarm->handle	= CreateWaitableTimer(NULL, FALSE, NULL);
	alarm->handle_vsync	= CreateEvent(NULL, FALSE, FALSE, NULL);
#endif
#elif _PS3
	sys_event_queue_attribute_t	attr;
//	sys_ppu_thread_t	id;
//	int		pri;

	sys_event_queue_attribute_initialize(attr);
	sys_event_queue_create(&alarm->event, &attr, SYS_EVENT_QUEUE_LOCAL, 4);

	sys_timer_create(&alarm->timer);
	sys_timer_connect_event_queue(alarm->timer, alarm->event, _am_timer_id, 0, 0);
	_am_timer_id++;

	sys_event_port_create(&alarm->port, SYS_EVENT_PORT_LOCAL, SYS_EVENT_PORT_NO_NAME);
	sys_event_port_connect_local(alarm->port, alarm->event);
#elif _WII
	OSCreateAlarm(&alarm->alarm);
	OSInitSemaphore(&alarm->sema, 0);
	OSSetAlarmUserData(&alarm->alarm, alarm);
#elif _IPHONE
	sem_init(&alarm->sema, 0, 0);
#if AMD_USE_PERFORMANCE_COUNTER
	amTimerCreate(&alarm->timer);
#else
	alarm->action.sa_sigaction	= amAlarmHandler;
	alarm->action.sa_flags		= SA_SIGINFO | SA_RESTART;
	sigemptyset(&alarm->action.sa_mask);
	sigaction(_am_timer_id, &alarm->action, NULL);
//	signal(SIGALRM, amAlarmHandler);
	alarm->event.sigev_notify	= SIGEV_SIGNAL;
	alarm->event.sigev_signo	= _am_timer_id;
	alarm->event.sigev_value.sival_ptr	= &alarm;
	timer_create(ITIMER_REAL, &alarm->event, &alarm->timer_id);
#endif
	_am_timer_id++;
#endif

	return	alarm;
}


/*****************************************************************************/
/* AMS_ALARM *amAlarmCreate(AMS_ALARM *alarm)                                */
/*---------------------------------------------------------------------------*/
/* [INPUT]  alarm : 確保済みの周期タイマー構造体                             */
/* [RETURN]  周期タイマー構造体                                              */
/* [FUNCTION]  周期タイマーの作成                                            */
/*****************************************************************************/
AMS_ALARM *amAlarmCreate(AMS_ALARM *alarm)
{
	Sint32	delete_flag = (alarm == NULL)? 1: 0;

	if (alarm == NULL)
		alarm	= (AMS_ALARM *)amMemAllocSystem(sizeof(AMS_ALARM));

	amAssert(alarm);

	memset(alarm, 0, sizeof(AMS_ALARM));
	alarm->alarm_id			= _am_timer_id;
	alarm->delete_flag		= delete_flag;

#if _PC | _XBOX
	char	tname[MAX_PATH];
	sprintf_s(tname, MAX_PATH, "%d", _am_timer_id);
	alarm->handle		= CreateEvent(NULL, FALSE, FALSE, tname);
	_am_timer_id++;
#elif _PS3
	sys_event_queue_attribute_t	attr;

	sys_event_queue_attribute_initialize(attr);
	sys_event_queue_create(&alarm->event, &attr, SYS_EVENT_QUEUE_LOCAL, 4);

	sys_event_port_create(&alarm->port, SYS_EVENT_PORT_LOCAL, SYS_EVENT_PORT_NO_NAME);
	sys_event_port_connect_local(alarm->port, alarm->event);

	alarm->timer		= NULL;
#elif _WII
	OSCreateAlarm(&alarm->alarm);
	OSInitSemaphore(&alarm->sema, 0);
	OSSetAlarmUserData(&alarm->alarm, alarm);
#elif _IPHONE
	sem_init(&alarm->sema, 0, 0);
#if !AMD_USE_PERFORMANCE_COUNTER
	alarm->action.sa_sigaction	= amAlarmHandler;
	alarm->action.sa_flags		= SA_SIGINFO | SA_RESTART;
	sigemptyset(&alarm->action.sa_mask);
	sigaction(_am_timer_id, &alarm->action, NULL);
	alarm->event.sigev_notify	= SIGEV_SIGNAL;
	alarm->event.sigev_signo	= _am_timer_id;
	alarm->event.sigev_value.sival_ptr	= &alarm;
	timer_create(ITIMER_REAL, &alarm->event, &alarm->timer_id);
#endif
	_am_timer_id++;
#endif

	return	alarm;
}


/*****************************************************************************/
/* void amAlarmDelete(AMS_ALARM *alarm)                                      */
/*---------------------------------------------------------------------------*/
/* [INPUT]  alarm    : 周期タイマー構造体                                    */
/* [FUNCTION]  周期タイマーの削除                                            */
/*****************************************************************************/
void amAlarmDelete(AMS_ALARM *alarm)
{
	amAssert(alarm);

#if _PC | _XBOX
#if AMD_USE_PERFORMANCE_COUNTER
	if (alarm->handle != NULL) {
		CloseHandle(alarm->handle);
		amMutexDelete(&alarm->mutex);
	}
#else
	if (alarm->handle != NULL)
		CloseHandle(alarm->handle);
	if (alarm->handle_vsync != NULL)
		CloseHandle(alarm->handle_vsync);
#endif
#elif _PS3
	if (alarm->timer != 0) {
		sys_timer_disconnect_event_queue(alarm->timer);
		sys_timer_destroy(alarm->timer);
	}
	if (alarm->port != 0) {
		sys_event_port_disconnect(alarm->port);
		sys_event_port_destroy(alarm->port);
	}
	if (alarm->event != 0)
		sys_event_queue_destroy(alarm->event, 0);
#elif _WII
	OSCancelAlarm(&alarm->alarm);
#elif _IPHONE
#if AMD_USE_PERFORMANCE_COUNTER
	amTimerDelete(&alarm->timer);
#else
	timer_delete(alarm->timer_id);
	if (_am_alarm_timer == alarm)
		_am_alarm_timer		= NULL;
#endif
	sem_destroy(&alarm->sema);
#endif

	if (alarm->delete_flag)
		amMemFreeSystem(alarm);
	else
		memset(alarm, 0, sizeof(AMS_ALARM));
}


/*****************************************************************************/
/* void amAlarmSetTimerVSync(AMS_ALARM *alarm)                               */
/*---------------------------------------------------------------------------*/
/* [INPUT]  alarm    : 周期タイマー構造体                                    */
/* [FUNCTION]  周期タイマーの設定(VSync周期)                                 */
/*****************************************************************************/
void amAlarmSetTimerVSync(AMS_ALARM *alarm)
{
	Uint32		interval;

	interval	= (Uint32)(1000000.0f / _am_draw_video.refresh_rate);

	amAlarmSetTimer(alarm, interval);
}


/*****************************************************************************/
/* void amAlarmSetTimer(AMS_ALARM *alarm, Uint32 interval)                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]  alarm    : 周期タイマー構造体                                    */
/*          interval : 周期(マイクロ秒単位)                                  */
/* [FUNCTION]  周期タイマーの設定                                            */
/*****************************************************************************/
void amAlarmSetTimer(AMS_ALARM *alarm, Uint32 interval)
{
	amAssert(alarm);

#if _PC | _XBOX
#if AMD_USE_PERFORMANCE_COUNTER
	amMutexLock(&alarm->mutex);
	QueryPerformanceCounter((LARGE_INTEGER *)&alarm->count_start);
	alarm->count_interval	= (Uint64)((float)interval * alarm->count_freq / 1000000.0f + 0.5f);
	alarm->count_end	= alarm->count_start + alarm->count_interval;
	amMutexUnlock(&alarm->mutex);
#else
	LARGE_INTEGER	DueTime;
	DueTime.HighPart	= (DWORD)0xffffffff;
	DueTime.LowPart		= (DWORD)-((Sint32)interval * 10);		// 100ns単位
	SetWaitableTimer(alarm->handle, &DueTime, 16, NULL, NULL, FALSE);
#endif
#elif _PS3
	system_time_t	DueTime;
	DueTime		= interval;
	sys_timer_stop(alarm->timer);
	sys_timer_start_periodic(alarm->timer, DueTime);
#elif _WII
	OSCancelAlarm(&alarm->alarm);
//	while (OSTryWaitSemaphore(&alarm->sema) > 0)
//		;
	OSSetPeriodicAlarm(&alarm->alarm,
			OSGetTime() + OSMicrosecondsToTicks(interval),
			OSMicrosecondsToTicks(interval),
			amAlarmHandler);
#elif _IPHONE
#if AMD_USE_PERFORMANCE_COUNTER
	amTimerStart(&alarm->timer);
	alarm->count_end	= alarm->timer.count_start + interval;
	alarm->count_interval	= interval;
#else
	alarm->tspec.it_interval.tv_sec		= interval / 1000000;
	alarm->tspec.it_interval.tv_nsec	= (interval % 1000000) * 1000;
//	alarm->tspec.it_interval.tv_usec	= interval % 1000000;
	alarm->tspec.it_value		= alarm->tspec.it_interval;
	timer_settime(alarm->timer_id, 0, &alarm->tspec, NULL);
	_am_alarm_timer		= alarm;
//	setitimer(ITIMER_REAL, &alarm->tspec, NULL);
#endif
#endif
}


/*****************************************************************************/
/* void amAlarmSet(AMS_ALARM *alarm)                                         */
/*---------------------------------------------------------------------------*/
/* [INPUT]  alarm    : 周期タイマー構造体                                    */
/* [FUNCTION]  周期タイマーのシグナル設定                                    */
/*****************************************************************************/
void amAlarmSet(AMS_ALARM *alarm)
{
	amAssert(alarm);

#if _PC | _XBOX
#if AMD_USE_PERFORMANCE_COUNTER
	amAssert(alarm->handle);
	SetEvent(alarm->handle);
#else
	if (alarm->handle_vsync != NULL)
		SetEvent(alarm->handle_vsync);
	else {
		amAssert(alarm->handle);
		SetEvent(alarm->handle);
	}
#endif
#elif _PS3
	amAssert(alarm->event);
	amAssert(alarm->port);
	sys_event_port_send(alarm->port, 0, 0, 0);
#elif _WII
	OSSignalSemaphore(&alarm->sema);
#elif _IPHONE
	sem_post(&alarm->sema);
#endif
}


/*****************************************************************************/
/* void amAlarmClear(AMS_ALARM *alarm)                                       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  alarm    : 周期タイマー構造体                                    */
/* [FUNCTION]  周期タイマーのシグナル解除                                    */
/*****************************************************************************/
void amAlarmClear(AMS_ALARM *alarm)
{
	amAssert(alarm);

#if _PC | _XBOX
#if AMD_USE_PERFORMANCE_COUNTER
	amAssert(alarm->handle);
	ResetEvent(alarm->handle);
#else
	if (alarm->handle != NULL);
		ResetEvent(alarm->handle);
	if (alarm->handle_vsync != NULL)
		ResetEvent(alarm->handle_vsync);
#endif
#elif _PS3
	amAssert(alarm->event);
	sys_event_queue_drain(alarm->event);
#elif _WII
	while (OSTryWaitSemaphore(&alarm->sema) > 0)
		;
#elif _IPHONE
	while (sem_trywait(&alarm->sema) == 0)
		;
#endif
}


/*****************************************************************************/
/* void amAlarmWaitTimer(AMS_ALARM *alarm)                                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]  alarm    : 周期タイマー構造体                                    */
/* [FUNCTION]  周期タイマーの待機                                            */
/*****************************************************************************/
void amAlarmWaitTimer(AMS_ALARM *alarm)
{
	amAssert(alarm);

#if _PC | _XBOX
#if AMD_USE_PERFORMANCE_COUNTER
	if (alarm->count_end) {
		Uint64	count;
		amMutexLock(&alarm->mutex);
		QueryPerformanceCounter((LARGE_INTEGER *)&count);
		while (count >= alarm->count_end)
			alarm->count_end	+= alarm->count_interval;
		amMutexUnlock(&alarm->mutex);
		for (;;) {
			Sleep(0);
			QueryPerformanceCounter((LARGE_INTEGER *)&count);
			if (count >= alarm->count_end)
				break;
		}
	}
#else
	WaitForSingleObject(alarm->handle, 0);
	WaitForSingleObject(alarm->handle, INFINITE);
#endif
#elif _PS3
	sys_event_t		event;
	sys_event_queue_drain(alarm->event);
	sys_event_queue_receive(alarm->event, &event, SYS_NO_TIMEOUT);
#elif _WII
	while (OSTryWaitSemaphore(&alarm->sema) > 0)
		;
	OSWaitSemaphore(&alarm->sema);
#elif _IPHONE
#if AMD_USE_PERFORMANCE_COUNTER
	if (alarm->count_end) {
		struct timespec		req, rem;
		Uint32		usec;
		amTimerEnd(&alarm->timer);
		while (alarm->count_end < alarm->timer.count_end)
			alarm->count_end	+= alarm->count_interval;
		usec			= alarm->count_end - alarm->timer.count_end;
		req.tv_sec		= usec / 1000000;
		req.tv_nsec		= (usec % 1000000) * 1000;
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
	}
#else
	while (sem_trywait(&alarm->sema) == 0)
		;
	sem_wait(&alarm->sema);
#endif
#endif
}


/*****************************************************************************/
/* void amAlarmWaitVSync(AMS_ALARM *alarm)                                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]  alarm    : 周期タイマー構造体                                    */
/* [FUNCTION]  周期タイマーの待機(VSync同期)                                 */
/*****************************************************************************/
void amAlarmWaitVSync(AMS_ALARM *alarm)
{
	amAssert(alarm);

#if _IPHONE
	amAlarmWaitTimer(alarm);
#else
	// VSyncの整数倍チェック
	float	draw_fps = floor(_am_draw_video.refresh_rate + 0.5f);
	float	main_fps = floor(60.0f / amSystemGetFrameRateMain() + 0.5f);
	float	rate;
	if (main_fps >= draw_fps)
		rate	= main_fps / draw_fps;
	else
		rate	= draw_fps / main_fps;
	rate	= rate - floor(rate);
	if ((rate > 0.01f) && (rate < 0.99f))
		return	amAlarmWaitTimer(alarm);

#if _PC | _XBOX
#if AMD_USE_PERFORMANCE_COUNTER
	WaitForSingleObject(alarm->handle, 0);
	amDrawSetVSyncAlarm(alarm);
	if (alarm->count_end) {
		Uint64	count, count_end;
		QueryPerformanceCounter((LARGE_INTEGER *)&count);
		amMutexLock(&alarm->mutex);
		count_end	= alarm->count_end;
		while (count >= count_end)
			count_end	+= alarm->count_interval;
		alarm->count_end	= count_end;

#if _PC
		// メインが軽すぎるとコマ飛びする？
		count_end	-= alarm->count_interval >> 2;
		while (count < count_end)
			QueryPerformanceCounter((LARGE_INTEGER *)&count);
#endif

		amMutexUnlock(&alarm->mutex);
		WaitForSingleObject(alarm->handle, 0);
		for (;;) {
			QueryPerformanceCounter((LARGE_INTEGER *)&count);
			if (WaitForSingleObject(alarm->handle, 0) == WAIT_OBJECT_0) {
//				amSystemLog("V");
				break;
			}
			if (count >= alarm->count_end) {
//				amSystemLog("T");
				break;
			}
		}
	} else {
		WaitForSingleObject(alarm->handle, INFINITE);
//		amSystemLog("V");
	}
#else
	HANDLE	handles[] = {
		alarm->handle, alarm->handle_vsync,
	};
	WaitForSingleObject(alarm->handle, 0);
	WaitForSingleObject(alarm->handle_vsync, 0);
	amDrawSetVSyncAlarm(alarm);
	WaitForMultipleObjects(2, handles, FALSE, INFINITE);
#endif
#elif _PS3
	sys_event_t		event;
	sys_event_queue_drain(alarm->event);
	amDrawSetVSyncAlarm(alarm);
	sys_event_queue_receive(alarm->event, &event, SYS_NO_TIMEOUT);
#elif _WII
	while (OSTryWaitSemaphore(&alarm->sema) > 0)
		;
	amDrawSetVSyncAlarm(alarm);
	OSWaitSemaphore(&alarm->sema);
#endif
#endif
}


/*****************************************************************************/
/* void amAlarmWait(AMS_ALARM *alarm)                                        */
/*---------------------------------------------------------------------------*/
/* [INPUT]  alarm    : 周期タイマー構造体                                    */
/* [FUNCTION]  周期タイマーのシグナル待機                                    */
/*****************************************************************************/
void amAlarmWait(AMS_ALARM *alarm)
{
	amAssert(alarm);

#if _PC | _XBOX
	amAssert(alarm->handle);
	WaitForSingleObject(alarm->handle, INFINITE);
#elif _PS3
	sys_event_t		event;
	sys_event_queue_receive(alarm->event, &event, SYS_NO_TIMEOUT);
#elif _WII
	OSWaitSemaphore(&alarm->sema);
#elif _IPHONE
	sem_wait(&alarm->sema);
#endif
}


/*****************************************************************************/
/* void amAlarmUpdateTimer(AMS_ALARM *alarm)                                 */
/*---------------------------------------------------------------------------*/
/* [INPUT]  alarm    : 周期タイマー構造体                                    */
/* [FUNCTION]  周期タイマーのアップデート                                    */
/*****************************************************************************/
void amAlarmUpdateTimer(AMS_ALARM *alarm)
{
	amAssert(alarm);

#if _PC | _XBOX
#if AMD_USE_PERFORMANCE_COUNTER
	Uint64	count;
	if (alarm->count_end) {
		QueryPerformanceCounter((LARGE_INTEGER *)&count);
		amMutexLock(&alarm->mutex);
		while (count >= alarm->count_end)
			alarm->count_end	+= alarm->count_interval;
		amMutexUnlock(&alarm->mutex);
	}
#endif
#elif _IPHONE
#if AMD_USE_PERFORMANCE_COUNTER
	Uint64	count;
	if (alarm->count_end) {
		amTimerEnd(&alarm->timer);
		while (alarm->count_end < alarm->timer.count_end)
			alarm->count_end	+= alarm->count_interval;
	}
#endif
#endif
}


/*****************************************************************************/
/* Sint32 amAlarmCheckTimer(AMS_ALARM *alarm)                                */
/*---------------------------------------------------------------------------*/
/* [INPUT]  alarm    : 周期タイマー構造体                                    */
/* [FUNCTION]  周期タイマーのチェック                                        */
/*****************************************************************************/
Sint32 amAlarmCheckTimer(AMS_ALARM *alarm)
{
	Sint32		ret = 0;

	amAssert(alarm);

#if _PC | _XBOX
#if AMD_USE_PERFORMANCE_COUNTER
	if (alarm->handle &&
			(WaitForSingleObject(alarm->handle, 0) == WAIT_OBJECT_0))
		ret		= 1;
	if (alarm->count_end) {
		Uint64	count;
		QueryPerformanceCounter((LARGE_INTEGER *)&count);
		if (count >= alarm->count_end)
			ret		= 1;
	}
#else
	amAssert(alarm->handle);
	if (WaitForSingleObject(alarm->handle, 0) == WAIT_OBJECT_0)
		ret		= 1;
#endif
#elif _PS3
	sys_event_t		event;
	int				num;
	if (sys_event_queue_tryreceive(alarm->event, &event, 1, &num) == CELL_OK)
		ret		= num;
#elif _WII
	if (OSTryWaitSemaphore(&alarm->sema) > 0)
		ret		= 1;
#elif _IPHONE
	if (sem_trywait(&alarm->sema) == 0)
		ret		= 1;
#endif

	return	ret;
}


/*****************************************************************************/
/* Sint32 amAlarmCheck(AMS_ALARM *alarm)                                     */
/*---------------------------------------------------------------------------*/
/* [INPUT]  alarm    : 周期タイマー構造体                                    */
/* [RETURN]  指定時間経過しているかどうか                                    */
/* [FUNCTION]  周期タイマーのチェック                                        */
/*****************************************************************************/
Sint32 amAlarmCheck(AMS_ALARM *alarm)
{
	Sint32		ret = 0;

	amAssert(alarm);

#if _PC | _XBOX
	amAssert(alarm->handle);
	if (WaitForSingleObject(alarm->handle, 0) == WAIT_OBJECT_0)
		ret		= 1;
#elif _PS3
	sys_event_t		event;
	int				num;
	if (sys_event_queue_tryreceive(alarm->event, &event, 1, &num) == CELL_OK)
		ret		= num;
#elif _WII
	if (OSTryWaitSemaphore(&alarm->sema) > 0)
		ret		= 1;
#elif _IPHONE
	if (sem_trywait(&alarm->sema) == 0)
		ret		= 1;
#endif

	return	ret;
}


#if _WII
/*****************************************************************************/
/* void amAlarmHandler(OSAlarm *alarm0, OSContext *context)                  */
/*---------------------------------------------------------------------------*/
/* [INPUT]  alarm0   : OSタイマー構造体                                      */
/*          context  : コンテキスト構造体                                    */
/* [FUNCTION]  アラームハンドラ                                              */
/*****************************************************************************/
void amAlarmHandler(OSAlarm *alarm0, OSContext *context)
{
	AMS_ALARM	*alarm = (AMS_ALARM *)OSGetAlarmUserData(alarm0);

	if (alarm == NULL)
		return;

	OSSignalSemaphore(&alarm->sema);
}
#endif

#if _IPHONE
/*****************************************************************************/
/* void amAlarmHandler(int signal_id, siginfo_t *info, void *context)        */
/*---------------------------------------------------------------------------*/
/* [INPUT]  signal_id : シグナルID                                           */
/*          info      : シグナル引数                                         */
/*          context   : コンテキスト構造体                                   */
/* [FUNCTION]  アラームハンドラ                                              */
/*****************************************************************************/
//void amAlarmHandler(int signal_id, siginfo_t *info, void *context)
void amAlarmHandler(int signal_id)
{
//	if (info == NULL)
//		return;

//	if (info->si_value.sival_ptr == NULL)
//		return;

//	AMS_ALARM	*alarm = (AMS_ALARM *)(info->si_value.sival_ptr);
//	sem_post(&alarm->sema);
	AMS_ALARM	*alarm = _am_alarm_timer;
	if (alarm != NULL)
		sem_post(&alarm->sema);
}
#endif


/*--- Local Functions -------------------------------------------------------*/

