/*****************************************************************************/
/*      amDebug.cpp                 Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* デバッグ・システムライブラリプログラム                                    */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090327-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files ---------------------------------------------------------*/

#include "alice.h"

#if _PS3
#include <cell/gcm_pm.h>
#endif


/*--- Macros ----------------------------------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/

/*--- Global Variables ------------------------------------------------------*/

float	_am_performance_main = 0.0f;
float	_am_performance_draw = 0.0f;
float	_am_performance_GPU  = 0.0f;

float	_am_framerate_main = 1.0f;
float	_am_framerate_draw = 1.0f;

Sint32	_am_exit_game = 0;


/*--- Local Variables -------------------------------------------------------*/

/*--- Global Functions ------------------------------------------------------*/

/*****************************************************************************/
/* void amSystemExitEnable(Sint32 flag)                                      */
/*---------------------------------------------------------------------------*/
/* [INPUT]  ゲームの終了を許可するかどうか                                   */
/* [FUNCTION]  ゲームの終了許可の設定                                        */
/*****************************************************************************/
void amSystemExitEnable(Sint32 flag)
{
	_am_exit_game		= flag? 1: 0;
}


/*****************************************************************************/
/* void amSystemSetFrameRateMain(float rate)                                 */
/*---------------------------------------------------------------------------*/
/* [INPUT]  メインスレッドのフレームレート(1/60秒基準での倍率)               */
/* [FUNCTION]  メインスレッドのフレームレートの設定                          */
/*****************************************************************************/
void amSystemSetFrameRateMain(float rate)
{
	_am_framerate_main		= rate;
}


/*****************************************************************************/
/* void amSystemSetFrameRateDraw(float rate)                                 */
/*---------------------------------------------------------------------------*/
/* [INPUT]  描画スレッドのフレームレート(1/60秒基準での倍率)                 */
/* [FUNCTION]  描画スレッドのフレームレートの設定                            */
/*****************************************************************************/
void amSystemSetFrameRateDraw(float rate)
{
	_am_framerate_draw		= rate;
}


/*****************************************************************************/
/* float amSystemGetFrameRateMain(void)                                      */
/*---------------------------------------------------------------------------*/
/* [RETURN]  メインスレッドのフレームレート(1/60秒基準での倍率)              */
/* [FUNCTION]  メインスレッドのフレームレートの取得                          */
/*****************************************************************************/
float amSystemGetFrameRateMain(void)
{
	return	_am_framerate_main;
}


/*****************************************************************************/
/* float amSystemGetFrameRateDraw(void)                                      */
/*---------------------------------------------------------------------------*/
/* [RETURN]  描画スレッドのフレームレート(1/60秒基準での倍率)                */
/* [FUNCTION]  描画スレッドのフレームレートの取得                            */
/*****************************************************************************/
float amSystemGetFrameRateDraw(void)
{
	return	_am_framerate_draw;
}


/*****************************************************************************/
/* float amDebugGetPerformanceMain(void)                                     */
/*---------------------------------------------------------------------------*/
/* [RETURN]  メインスレッドのCPU処理時間率(フレーム基準での%)                */
/* [FUNCTION]  メインスレッドのCPU処理時間率の取得                           */
/*****************************************************************************/
float amDebugGetPerformanceMain(void)
{
	return	_am_performance_main;
}


/*****************************************************************************/
/* float amDebugGetPerformanceDraw(void)                                     */
/*---------------------------------------------------------------------------*/
/* [RETURN]  描画スレッドのCPU処理時間率(フレーム基準での%)                  */
/* [FUNCTION]  描画スレッドのCPU処理時間率の取得                             */
/*****************************************************************************/
float amDebugGetPerformanceDraw(void)
{
	return	_am_performance_draw;
}


/*****************************************************************************/
/* float amDebugGetPerformanceGPU(void)                                      */
/*---------------------------------------------------------------------------*/
/* [RETURN]  GPU処理時間率(1/60秒基準での%)                                  */
/* [FUNCTION]  GPU処理時間率の取得                                           */
/*****************************************************************************/
float amDebugGetPerformanceGPU(void)
{
	return	_am_performance_GPU;
}


/*****************************************************************************/
/* void amDebugSetPerformanceMain(float frame)                               */
/*---------------------------------------------------------------------------*/
/* [INPUT]  frame : メインスレッドのCPU処理時間率(1/60秒基準での%)           */
/* [FUNCTION]  メインスレッドのCPU処理時間率の設定                           */
/*****************************************************************************/
void amDebugSetPerformanceMain(float frame)
{
	_am_performance_main	= frame / _am_framerate_main;
}


/*****************************************************************************/
/* void amDebugSetPerformanceDraw(float frame)                               */
/*---------------------------------------------------------------------------*/
/* [INPUT]  frame : 描画スレッドのCPU処理時間率(1/60秒基準での%)             */
/* [FUNCTION]  描画スレッドのCPU処理時間率の設定                             */
/*****************************************************************************/
void amDebugSetPerformanceDraw(float frame)
{
	_am_performance_draw	= frame / _am_framerate_draw;
}


/*****************************************************************************/
/* void amDebugSetPerformanceGPU(void)                                       */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  GPU処理時間率の設定                                           */
/*****************************************************************************/
void amDebugSetPerformanceGPU(void)
{
#if AMD_DEBUG
#if _XBOX
	LPDIRECT3DDEVICE9	d3dDev;
	float		frame;

	d3dDev		= amXboxDxGetDirect3DDevice();

	frame		= (1.0f - d3dDev->GetCounter(D3DCOUNTER_FRAME_GPU_IDLE_PERCENT) * 0.01f)
				* 60.0f
				/ (d3dDev->GetCounter(D3DCOUNTER_FRAMESPERSECOND) * _am_framerate_draw);

	_am_performance_GPU	= frame;
#elif _PS3
	Uint32		counter[4];
	Uint32		cycle;

	cellGcmSetPerfMonTrigger();
	cellGcmGetPerfMonCounter(CELL_GCM_PM_DOMAIN_GCLK, counter, &cycle);

	_am_performance_GPU	= (float)(cycle - counter[0])
			/ (500000.0f * 16.66666f * _am_framerate_draw);
#endif
#endif
}


/*****************************************************************************/
/* void amDebugDisplayPerformance(Sint32 pos_x, Sint32 pos_y)                */
/*---------------------------------------------------------------------------*/
/* [INPUT]  pos_x, pos_y : 表示位置                                          */
/* [FUNCTION]  CPU処理時間率の表示                                           */
/*****************************************************************************/
void amDebugDisplayPerformance(Sint32 pos_x, Sint32 pos_y)
{
#if AMD_DEBUG
	char	buf[256];
	Sint32	main0, main1, draw0, draw1, gpu0, gpu1;

	main1	= (Sint32)(amDebugGetPerformanceMain() * 10000.0f);
	main0	= main1 / 100;
	main1	-= main0 * 100;

	draw1	= (Sint32)(amDebugGetPerformanceDraw() * 10000.0f);
	draw0	= draw1 / 100;
	draw1	-= draw0 * 100;

	gpu1	= (Sint32)(amDebugGetPerformanceGPU() * 10000.0f);
	gpu0	= gpu1 / 100;
	gpu1	-= gpu0 * 100;

	sprintf(buf, "%3d.%02d %3d.%02d %3d.%02d", main0, main1, draw0, draw1, gpu0, gpu1);
	amPrintf(pos_x, pos_y, buf);
#else
	UNREFERENCED_PARAMETER(pos_x);
	UNREFERENCED_PARAMETER(pos_y);
#endif
}


/*--- Local Functions -------------------------------------------------------*/

