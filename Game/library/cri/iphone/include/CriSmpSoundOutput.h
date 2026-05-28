#ifndef	CRISMPSOUNDOUTPUT_H_INCLUDED		/* Re-definition prevention */
#define	CRISMPSOUNDOUTPUT_H_INCLUDED
/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006-2008 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Sound Output
 * File     : CriSmpSoundOuput.h
 * Date     : 2008-10-20
 * Version  : 1.00
 *
 ****************************************************************************/

#include <cri_xpt.h>

class CriSmpSoundOutput
{
public:
	static CriSmpSoundOutput* Create(void);
	static CriBool Initialize(void);
	static void Finalize(void);
	virtual void Destroy(void) = 0;
	virtual void SetNotifyCallback(CriUint32 (*func)(void *obj, CriUint32 nch, CriFloat32 *sample[], CriUint32 nsmpl), void *obj) = 0;
	virtual void Start(void) = 0;
	virtual void Stop(void) = 0;
	virtual void ExecuteMain(void) = 0;
	virtual void SetFrequency(CriUint32 frequency) = 0;
	static const CriUint32 MAX_NCH=8;
	static const CriUint32 NSMPL_BLK=128;
	enum Status {
		STOP,
		EXEC
	} stat;
	inline CriSint16 PcmfToPcm16(CriFloat32 pcmf) {
		if ( pcmf > -1.0f && pcmf <=1.0f ) {
			return (Sint16)(pcmf*32767.0f);
		} else if ( pcmf > 1.0f ) {
			return  32767;
		} else {
			return -32768;
		}
	}
	
protected:
	CriSmpSoundOutput(void) {}
	virtual ~CriSmpSoundOutput(void) {}
};

#ifdef __cplusplus
extern "C" {
#endif
CriSmpSoundOutput *CriSmpSoundOutput_CreateThread(void);
void CriSmpSoundOutput_DestroyThread(void);
#ifdef __cplusplus
}
#endif

#endif		//	CRISMPSOUNDOUTPUT_H_INCLUDED
