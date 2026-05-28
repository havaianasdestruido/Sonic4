#ifndef	CRISMPSOUNDOUTPUT_IPHONE_H_INCLUDED		/* Re-definition prevention */
#define	CRISMPSOUNDOUTPUT_IPHONE_H_INCLUDED
/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2009 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Sound Output for iPhone
 * File     : CriSmpSoundOuput_iPhone.h
 * Date     : 2009-09-10
 * Version  : 1.00
 *
 ****************************************************************************/

#include <cri_xpt.h>
#include "CriSmpSoundOutput.h"


/* 出力周波数 */
//#define CRISMP_SOUNDOUTPUT_FREQ (48000)
//#define CRISMP_SOUNDOUTPUT_FREQ (44100)
#define CRISMP_SOUNDOUTPUT_FREQ (22050)

#ifdef __cplusplus
extern "C" {
#endif

/***
 *	サウンド出力処理の開始
 *  AudioSession Interruption Callback の処理から呼ばれるサウンド開始処理
 */
void CriSmpSoundOutput_ReStartSound(void);

/***
 *	サウンド出力処理の停止
 *  AudioSession Interruption Callback の処理から呼ばれるサウンド停止処理
 */
void CriSmpSoundOutput_StopSound(void);


#ifdef __cplusplus
}
#endif

#endif		//	CRISMPSOUNDOUTPUT_IPHONE_H_INCLUDED
