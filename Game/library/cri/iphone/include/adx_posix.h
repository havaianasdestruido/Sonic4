/***************************************************************************
 *
 *	CRI Middleware SDK
 *
 *	Copyright (c) 2004-2009 CRI-MW
 *
 *	Library	: ADX Library
 *	Module	: Environmentally-dependent header for Posix
 *	File	: adx_posix.h
 *	Create	: 2004-10-27
 *	Version	: Refer "ADXPOSIX_VER_NUM"
 *  Comment : 2009-01-24 adx_linux.h をposix用に改編
 *
 ***************************************************************************/

/* 多重定義防止					*/
/* Prevention of redefinition	*/
#ifndef _ADXPOSIX_H_INCLUDED
#define _ADXPOSIX_H_INCLUDED

/***************************************************************************
 *       バージョン情報
 *       Version
 ***************************************************************************/
#define ADXPOSIX_VER_NAME		"ADXPOSIX"
#define ADXPOSIX_VER_NUM		"1.20"
#define ADXPOSIX_VER_OPTION		

/***************************************************************************
 *      インクルードファイル
 *      Include files
 ***************************************************************************/
#include <pthread.h>
#include "cri_xpt.h"

/***************************************************************************
 *      定数
 *      Constants
 ***************************************************************************/
/* フレームワーク種別（Posix） */
/* Framework type for posix */
typedef enum {
	ADXM_FRAMEWORK_DEFAULT						= 0,
	ADXM_FRAMEWORK_POSIX_SINGLE_THREAD			= 1,
	ADXM_FRAMEWORK_POSIX_MULTI_THREAD			= 2,
	ADXM_FRAMEWORK_POSIX_MULTI_THREAD_NO_PRIO	= 3
} AdxmFramework;

/* デフォルトスケジューリングポリシー */
#if defined(XPT_TGT_IPHONE)
#define ADXM_POLICY_VV			(SCHED_OTHER)	/* 仮想VSync生成スレッド:未使用 */
#define ADXM_POLICY_VSYNC		(SCHED_FIFO)	/* Vsyncスレッド */
#define ADXM_POLICY_FS			(SCHED_FIFO)	/* ファイル処理スレッド */
#define ADXM_POLICY_MAIN		(SCHED_RR)		/* メインスレッド */
#define ADXM_POLICY_MWIDLE		(SCHED_RR)		/* アイドルスレッド */
#else
#define ADXM_POLICY_VV			(SCHED_FIFO)	/* 仮想VSync生成スレッド */
#define ADXM_POLICY_VSYNC		(SCHED_FIFO)	/* Vsyncスレッド */
#define ADXM_POLICY_FS			(SCHED_FIFO)	/* ファイル処理スレッド */
#define ADXM_POLICY_MAIN		(SCHED_FIFO)	/* メインスレッド */
#define ADXM_POLICY_MWIDLE		(SCHED_OTHER)	/* アイドルスレッド */
#endif

/* デフォルトプライオリティ */
#if defined(XPT_TGT_IPHONE)
#define ADXM_PRIO_VV			(0)				/* 仮想Vsync生成スレッド:未使用 */
#define ADXM_PRIO_VSYNC			(16)		 	/* Vsyncスレッド */
#define ADXM_PRIO_FS			(-8)			/* ファイル処理スレッド */
#define ADXM_PRIO_MAIN			(-12)		 	/* メインスレッド */
#define ADXM_PRIO_MWIDLE		(-16)		   	/* アイドルスレッド */
#else
#define ADXM_PRIO_VV			(16)			/* 仮想Vsync生成スレッド */
#define ADXM_PRIO_VSYNC			(8)				/* Vsyncスレッド */
#define ADXM_PRIO_FS			(4)				/* ファイル処理スレッド */
#define ADXM_PRIO_MAIN			(2)				/* メインスレッド */
#define ADXM_PRIO_MWIDLE		(0)				/* アイドルスレッド */
#endif

/* デフォルト同期周波数x100					*/
/* Default of virtual vsync frequency x100	*/
#define ADXPOSIX_DEF_VHZ100		(6000)			/* 60Hz */

/***************************************************************************
 *      処理マクロ
 *      Macro Functions
 ***************************************************************************/

/***************************************************************************
 *      データ型宣言
 *      Data Type Declarations
 ***************************************************************************/
/*	ANSIファイルシステムのセットアップパラメータ構造体	*/
/*	Parameter structure for ANSI setup function			*/
typedef struct {
	CriChar8	*rtdir;		   	/* ルートディレクトリ	*/
								/* Root directory		*/
} AdxPosixSprmFs;

/* スレッドセットアップパラメータ（Posix） */
/* Thread setup parameter for posix */
typedef struct {
	/* スケジューリングポリシー */
	CriSint32 policy_vv;	   	/* 仮想Vsync生成スレッド */
	CriSint32 policy_vsync;		/* Vsyncスレッド */
	CriSint32 policy_fs;		/* ファイル処理スレッド */
	CriSint32 policy_main;	   	/* メインスレッド */
	CriSint32 policy_mwidle;   	/* アイドルスレッド */
	/* プライオリティ */
	CriSint32 priority_vv;	   	/* 仮想Vsync生成スレッド */
	CriSint32 priority_vsync;  	/* Vsyncスレッド */
	CriSint32 priority_fs;	   	/* ファイル処理スレッド */
	CriSint32 priority_main;   	/* メインスレッド */
	CriSint32 priority_mwidle; 	/* アイドルスレッド */
} AdxmThreadSprm;

/* スレッドのセットアップを行う際の注意点							*/
/* (a) セットアップパラメータは、フレームワークとして				*/
/* 　　ADXM_FRAMEWORK_POSIX_MULTI_THREADを選んだ場合のみ有効です。	*/
/* (b) セットアップパラメータは、ADXM_SetupFramework関数に引数prmと	*/
/* 　　して渡します。NULLを指定するとデフォルト設定が使用されます。	*/
/* (c) プライオリティを設定するためには、スケジューリングポリシーと	*/
/* 　　してADXPOSIX_SCHED_FIFOまたはADXPOSIX_SCHED_RRを指定する必要	*/
/* 　　があります。													*/
/* (d) スケジューリングポリシーとしてADXPOSIX_SCHED_OTHERを設定した	*/
/* 　　場合は、プライオリティとして0を設定してください。			*/
/* (e) プライオリティは数値が高いほど優先度が高いです。				*/

/***************************************************************************
 *      変数宣言
 *      Prototype Functions
 ***************************************************************************/

/***************************************************************************
 *      関数宣言
 *      Prototype Functions
 ***************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/***
*			フレームワーク設定関数
*			Framework setting function
***/
/* CRIミドルウェアのフレームワークセットアップ */
/* Setup the framework */
CriBool CRIAPI ADXM_SetupFramework(AdxmFramework framework, void *prm);

/* CRIミドルウェアのフレームワークの終了 */
/* Shutdown the framework */
CriBool CRIAPI ADXM_ShutdownFramework(void);

/* ファイルシステムのセットアップ */
void CRIAPI ADXPOSIX_SetupFileSystem(AdxPosixSprmFs *sprmd);

/* ファイルシステムのシャットダウン */
void CRIAPI ADXPOSIX_ShutdownFileSystem(void);

#if !defined(XPT_TGT_IPHONE)
/*	同期周波数の設定（1/100Hz単位、60Hzなら6000）*/
void CRIAPI ADXM_SetVhz(CriUint32 vhz100);

/*	同期周波数の取得 */
CriUint32 CRIAPI ADXM_GetVhz(void);

/* 仮想Vsync周期の測定値の取得 */
CriUint32 CRIAPI ADXPOSIX_GetVirtualVsyncIntervalTime(void);

/* 仮想Vsync周期の補正 */
void CRIAPI ADXPOSIX_ResetVirtualVsyncInterval(void);
#endif

/* 外部からデータを流し込む際の口 */
CriSint32 CRIAPI ADXPOSIX_SetExtTapIn(CriUint32 (*func)(void *obj, CriUint32 nch, CriFloat32 *sample[], CriUint32 nsmpl), void *obj);
void CRIAPI ADXPOSIX_CleanExtTapIn(void);


#ifdef __cplusplus
}
#endif

/***************************************************************************
 *      旧バージョンとの互換用
 *      For compatibility with old version
 ***************************************************************************/
#define ADXPOSIX_SPRM_ANSI				AdxPosixSprmFs
#define AdxPosixSprmAnsi				AdxPosixSprmFs
#define ADXPOSIX_SetupAnsiFs(sprm)		ADXPOSIX_SetupFileSystem(sprm)
#define ADXPOSIX_ShutdownAnsiFs()		ADXPOSIX_ShutdownFileSystem()

#endif	/* #ifndef _ADXPOSIX_H_INCLUDED */

/* end of file */
