/*****************************************************************************/
/*      alice.h                                                              */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* 共通ヘッダ                                                                */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090316-       0.01   first version                                        */
/*****************************************************************************/

#ifndef _ALICE_H
#define _ALICE_H

/*--- Include Files (Pre Definitions) ---------------------------------------*/

#include <nn.h>

#if _PS3
#include <float.h>     // FLT_MAX
#include <sys/paths.h> // SYS_HOST_ROOT
#define UNREFERENCED_PARAMETER(P)          (P = P)
#define MAX_PATH          260
#elif _WII
#include <float.h>     // FLT_MAX
#include <nnd.h>
#include <revolution.h>
#define UNREFERENCED_PARAMETER(P)          (P = P)
#define MAX_PATH          260

#if AMD_DEBUG
#include "screenshot.h" //スクリーンキャプチャ
#endif

#elif _IPHONE
#include <float.h>     // FLT_MAX
#define UNREFERENCED_PARAMETER(P)          (P = P)
#define MAX_PATH          260
#include <stdarg.h>    // va_start
#include <dirent.h>    // opendir
#endif

/*--- Definitions -----------------------------------------------------------*/

/*
// AMD_DEBUG は 要望により_DEBUG と関連を持たせないようにしました
// AMD_DEBUG は プリプロセッサに定義してください
#ifdef _DEBUG
#define AMD_DEBUG				(1)
#else
#define AMD_DEBUG				(0)
#endif
*/

#define	AMD_USE_KEYBD			(0)			// キーボード(未実装)

#if _IPHONE
#define AMD_USE_DRAW_THREAD		(0)			// 描画スレッドの使用
#else
#define AMD_USE_DRAW_THREAD		(1)			// 描画スレッドの使用
#endif

#define AMD_TASK_THREAD_NUM		(1)			// メインタスクのスレッド数

#define AMD_MATRIX_STACK_SIZE	(32)		// マトリクススタックサイズ

#define AMD_SHADER_PRECOMPILED	(0)			// プリコンパイルシェーダー使用

#if _IPHONE
// AppDelegate.m の方も変更すること
#define	AMD_USE_CRIAUDIO		(1)			// CRIAudio
#define	AMD_USE_CRIADX			(0)			// CRIADX
#else
#define	AMD_USE_CRIAUDIO		(1)			// CRIAudio
#define	AMD_USE_CRIADX			(0)			// CRIADX
#endif


/*--- Macros ----------------------------------------------------------------*/

#define amPrint			amDrawPrint
#define amPrintf		amDrawPrintf
#define amPrintColor	amDrawPrintColor

#if AMD_DEBUG
#define amAssert(arg_)   NNM_ASSERT((Uint32)(arg_), #arg_)
#if _PC
#define amSystemLog      amWinSystemLog
#else
#define amSystemLog      printf
#endif
#else
#define amAssert(_e)        (void)0
#define amSystemLog(...)    (void)0
#endif


/*--- Include Files (Post Definitions) --------------------------------------*/

#include "amTypes.h"
#include "amDebug.h"
#include "amTask.h"
#include "amTimer.h"
#include "amThread.h"
#include "amMalloc.h"
#include "amFs.h"
#include "amDraw.h"
#include "amRender.h"
#include "amMatrix.h"
#include "amPad.h"
#include "amConvert.h"
#include "amBind.h"
#include "amShader.h"
#include "amTexture.h"
#include "amObject.h"
#include "amMotion.h"
#include "amUtility.h"
#include "amVector.h"
#include "amQuat.h"
#include "amEffect.h"
#include "amCriAudio.h"
#include "amTxb.h"
#include "amTrail.h"

#if _PC
#include "Win32/amWin.h"
#include "Win32/amWinDx.h"
#elif _XBOX
#include "Xbox360/amXbox.h"
#include "Xbox360/amXboxDx.h"
#elif _PS3
#include "Ps3/amPs3.h"
#elif _WII
#include "Wii/amWii.h"
#elif _IPHONE
#include "iPhone/amIPhone.h"
#include "iPhone/amIPhoneBase.h"
#include "amTp.h"
#include "iPhone/amIPhoneAdx.h"
#endif


/*--- Local Declarations ----------------------------------------------------*/

/*--- External Variables ----------------------------------------------------*/

extern Uint32	_am_system_flag[4];				// システムフラグ
extern Uint32	_am_debug_flag[4];				// デバッグフラグ
extern NNS_MATRIXSTACK	_am_default_stack;

#if AMD_USE_DRAW_THREAD
#if !_IPHONE
extern AMS_ALARM		_am_main_timer;
extern AMS_THREAD		_am_draw_thread;
extern AMS_THREAD_ID	_am_draw_thread_id;
extern NNS_MATRIXSTACK	_am_draw_stack;
#else
extern AMS_ALARM		_am_main_timer;
extern AMS_THREAD		_am_game_thread;
extern AMS_THREAD_ID	_am_game_thread_id;
extern NNS_MATRIXSTACK	_am_game_stack;
#endif
#endif


/*--- Global Variables ------------------------------------------------------*/

/*--- Local Variables -------------------------------------------------------*/

/*--- Inline Functions ------------------------------------------------------*/
//## Inline Functions

/*--- Global Functions ------------------------------------------------------*/
//## Global Functions

/*--- TCB Functions ---------------------------------------------------------*/
//## TCB Functions

/*--- Local Functions -------------------------------------------------------*/
//## Local Functions

#endif
