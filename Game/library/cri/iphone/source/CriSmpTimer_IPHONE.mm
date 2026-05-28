/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006-2009 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Timer
 * File     : CriSmpTimer_LINDBERGH.cpp
 * Date     : 2009-01-31
 * Version  : 1.00
 *
 ****************************************************************************/
#include <stdio.h>
#include <sys/time.h>
#include <CriSmpTimer.h>

/***
*		Class Functions
***/

class CriSmpTimerLoc : public CriSmpTimer
{
public:
	virtual void Start(void);
	virtual void Stop(void);
	virtual CriFloat32 GetElapseMsTime(void);
	virtual void PrintElapseMsTime(void);

	CriUint32 get_count(void);
	CriUint32 diff_count(CriUint32 count);

	//
	CriBool cnt_flag;
	CriUint32 start_count;
	CriUint32 stop_count;
};


/*
 *	カウンタ値の取得
 */
CriUint32 CriSmpTimerLoc::get_count(void)
{
	struct timeval tv;
	CriUint32 count;

	/* 時刻の取得 */
	// 時刻取得のオーバーヘッド注意
	gettimeofday(&tv, NULL);

	/* カウンタ値の作成 */
	/* tv_secに関しては2048以上の値を使用しない（32bit値の範囲内に収めるため） */
	count = (tv.tv_sec & 2047) * 1000000 + tv.tv_usec;

	return count;
}

/*
 *	カウンタ値の差分
 */
CriUint32 CriSmpTimerLoc::diff_count(CriUint32 count)
{
	CriUint32 diff;

	if (count >= start_count)
		diff = count - start_count;
	else
		diff = (2048 * 1000000) - start_count + count;
	
	return	diff;
}


CriSmpTimer* CriSmpTimer::Create(void)
{
	CriSmpTimerLoc* tmrl;

	tmrl = new CriSmpTimerLoc;

	tmrl->cnt_flag = FALSE;

	tmrl->start_count = 0;
	tmrl->stop_count = 0;

	tmrl->Start();

	return tmrl;
}

void CriSmpTimer::Destroy(void)
{
	delete this;
}

void CriSmpTimerLoc::Start(void) {
	cnt_flag = TRUE;
	start_count = get_count();
}

void CriSmpTimerLoc::Stop(void) {
	cnt_flag = FALSE;
	stop_count = get_count();
}

Float32 CriSmpTimerLoc::GetElapseMsTime(void)
{
	CriUint32 count;

	if (cnt_flag == TRUE)
		count = get_count();
	else
		count = stop_count;

	return (CriFloat32)diff_count(count)/1000.0f;
}

void CriSmpTimerLoc::PrintElapseMsTime(void)
{
	CriFloat32 etime;

	etime = GetElapseMsTime();
	printf("Elapse Time = %f\n", etime);
}

/***
*			Timer for Debugging
***/
static const CriUint32 MAX_DBG_TIMER=8;

CriSmpTimer* g_cri_smp_dbg_timer[MAX_DBG_TIMER];

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void cri_smp_dbg_initialize(void)
{
	for (CriUint32 i=0; i<MAX_DBG_TIMER; i++) {
		g_cri_smp_dbg_timer[i] = CriSmpTimer::Create();
		g_cri_smp_dbg_timer[i]->Start();
		g_cri_smp_dbg_timer[i]->Stop();
	}
}

void cri_smp_dbg_finalize(void)
{
	for (CriUint32 i=0; i<MAX_DBG_TIMER; i++)
		g_cri_smp_dbg_timer[i]->Destroy();
}

void cri_smp_dbg_timer_start(CriUint32 no)
{
	g_cri_smp_dbg_timer[no]->Start();
}

void cri_smp_dbg_timer_stop(CriUint32 no)
{
	g_cri_smp_dbg_timer[no]->Stop();
}

CriFloat32 cri_smp_dbg_timer_get_elapse_time(CriUint32 no)
{
	return g_cri_smp_dbg_timer[no]->GetElapseMsTime();
}

#ifdef __cplusplus
}
#endif /* __cplusplus */


/* end of file */
