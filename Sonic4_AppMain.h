/*****************************************************************************/
/*      Sonic4_AppMain.h           Author : Syuichi Gotou                    */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* iPhone アプリケーションメインヘッダ                                       */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090630-       0.01   first version                                        */
/*****************************************************************************/

#ifndef _APPMAIN_H
#define _APPMAIN_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
	
/*--- Include Files (Pre Definitions) ---------------------------------------*/


/*--- Definitions -----------------------------------------------------------*/
#define AMD_SAMPLE_COUNT_DEFAULT		(3)

#define AMD_SAMPLE_SAVE_LOG				(0) // AppLogの使用
	
#define APPD_MAIN_DRAW_FRAME			(0)
	
#define APPD_MAIN_CRIAUDIO_INIT_DELAY	(1) // 初期化を遅らせる
	
/*--- Macros ----------------------------------------------------------------*/



/*--- Include Files (Post Definitions) --------------------------------------*/



/*--- Local Declarations ----------------------------------------------------*/

/*--- External Variables ----------------------------------------------------*/


/*--- Global Variables ------------------------------------------------------*/
extern int _am_sample_count;
extern BOOL _am_sample_draw_enable;
extern BOOL _am_sample_is_suspended;
extern int _am_sample_suspended_count; // ハード固有のサスペンド命令が発生している待ちカウント
extern BOOL _am_sample_is_sleep;		// スリープに移行するか否か
extern BOOL _am_sample_is_accel; // 加速度センサーを使うか否か
extern BOOL _am_sample_is_ignore_audio_interruption; //オーディオ割り込み処理の無視


/*--- Local Variables -------------------------------------------------------*/

/*--- Inline Functions ------------------------------------------------------*/
//## Inline Functions

/*--- Global Functions ------------------------------------------------------*/
//## Global Functions

// アプリケーション初期化
int Sonic4_AppInit(int Width, int Height, const char* pDocPath);

// アプリケーションメインループ
int Sonic4_AppMainLoop(void);

// アプリケーション終了
int Sonic4_AppFinish(void);

// アプリケーション休止
void Sonic4_AppSuspend(void);

#if AMD_SAMPLE_SAVE_LOG
// ログ保存
void AppLog(const char *log_text);
void AppLogv(const char *log_text, va_list vlist);
void AppLogf(const char *log_text, ...);
#else
#define AppLog(log_text)
#define AppLogv(log_text, vlist);
#define AppLogf(log_text, ...);
#endif // AMD_SAMPLE_SAVE_LOG
/*--- TCB Functions ---------------------------------------------------------*/
//## TCB Functions

/*--- Local Functions -------------------------------------------------------*/
//## Local Functions
	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
