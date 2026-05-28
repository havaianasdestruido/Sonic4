#ifndef	CRISMPTIMER_H_INCLUDED		/* Re-definition prevention */
#define	CRISMPTIMER_H_INCLUDED
/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006-2009 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Timer
 * File     : CriSmpTimer.h
 * Date     : 2009-1-31
 * Version  : 1.0
 *
 ****************************************************************************/

#include <cri_xpt.h>

class CriSmpTimer
{
public:
	static CriSmpTimer* Create(void);
	virtual void Destroy(void);
	virtual void Start(void)=0;
	virtual void Stop(void)=0;
	virtual CriFloat32 GetElapseMsTime(void)=0;
	virtual void PrintElapseMsTime(void)=0;

protected:
	CriSmpTimer(void) {}
	virtual ~CriSmpTimer(void) {}
};

#endif		//	CRISMPTIMER_H_INCLUDED
