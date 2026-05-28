/***************************************************************************
 *
 *	CRI Middleware SDK
 *
 *	Copyright (c) 2006-2009 CRI-MW
 *
 *	Library	: ADX Library
 *	Module	: Environmentally-dependent header for iPhone
 *	File	: adx_iPhone.h
 *	Create	: 2009-01-24
 *	Version	: Refer "ADXIPHONE_VER_NUM"
 *
 ***************************************************************************/

/* 多重定義防止					*/
/* Prevention of redefinition	*/
#ifndef _ADXIPHONE_H_INCLUDED
#define _ADXIPHONE_H_INCLUDED

/***************************************************************************
 *       バージョン情報
 *       Version
 ***************************************************************************/
#define ADXIPHONE_VER_NAME		"ADXIPHONE"
#define ADXIPHONE_VER_NUM		"1.2.0"
#define ADXIPHONE_VER_OPTION		

/***************************************************************************
 *      インクルードファイル
 *      Include files
 ***************************************************************************/
#include "cri_xpt.h"
#include "cri_adxt.h"
#include "adx_posix.h"

/***************************************************************************
 *      定数
 *      Constants
 ***************************************************************************/

/***************************************************************************
 *      処理マクロ
 *      Macro Functions
 ***************************************************************************/
/* タイムストレッチ用ワーク領域の際図 */
#define ADXIPHONE_WORKSIZE_TS	(16 * 1024)

/***************************************************************************
 *      データ型宣言
 *      Data Type Declarations
 ***************************************************************************/
/* サウンドセットアップパラメータ */
typedef struct {
	/* - ADX ミキシング用バッファの指定
	 *　・デフォルト設定とする場合、
	 *　　　svrfreq=0, mixbuf_ptr=NULL, mixbuf_size=0
	 *　　としてください。
	 *　・ADXサーバ関数の呼び出し周期をデフォルトの60Hzよりも低くした場合、
	 *　　再生音が途切れる可能性があります。
	 *　　その場合、ADXIPHONE_GetBufferSizeBySvrFreq関数で取得されるサイズ分
	 *　　のバッファを内部処理用バッファとしてADXに設定する必要があります。
	 *　・設定する場合、svrfreq, mixbuf_ptr, mixbuf_size に値を設定します。
	 */
	CriSint32	svrfreq;		/* ADX サーバ実行周期(Hz) */
	void		*mixbuf_ptr;	/* ADX ミキシング用バッファ */
	CriSint32	mixbuf_size;	/* ADX ミキシング用バッファサイズ */
	/*
	 */
} AdxiPhoneSprmSnd;

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

/* $func$ ADX サウンドのセットアップ
 * [書　式] void CRIAPI ADXIPHONE_SetupSound(AdxiPhoneSprmSnd *sprm)
 * [入　力] sprm : サウンドパラメータ構造体(AdxiPhoneSprmSnd)
 * [出　力] なし
 * [関数値] なし
 * [機　能] ・iPhone用サウンド処理の設定をします。
 *　　　　　・通常デフォルト設定で動作するよう調整していますので、基本的には引数は
 *　　　　　　NULLを指定してください。
 *　　　　　・特殊な設定を行う場合に、必要なメンバに値をセットしたAdxiPhoneSprmSnd
 *　　　　　　構造体を引数として渡します。
 *　　　　　・AdxiPhoneSprmSnd構造体を使用する場合、memset等を用いて構造体全体を
 *　　　　　　0クリアします。
 * [使用例 その１ デフォルト設定で使用する場合]
 *　　　　　// 引数 NULL でセットアップ
 *　　　　　ADXIPHONE_SetupSound(NULL);
 *　　　　　　：
 *
 * [使用例 その２ ADXサーバ関数呼び出し周期を 20Hz にする場合]
 *　　　　　// ADXサーバ関数呼び出し周期を 20Hz にする場合
 *　　　　　AdxiPhoneSprmSnd sprm;
 *　　　　　// AdxiPhoneSprmSnd構造体をクリア
 *　　　　　memset(&sprm, 0, sizeof(sprm));
 *　　　　　// ミキシング用バッファ設定に必要なメンバに値を設定
 *　　　　　sprm.svrfreq = 20;
 *　　　　　sprm.mixbuf_size = ADXIPHONE_GetBufferSizeBySvrFreq(sprm.svrfreq);
 *　　　　　sprm.mixbuf_ptr = malloc(sprm.mixbuf_size);
 *　　　　　// 引数 AdxiPhoneSprmSnd でセットアップ
 *　　　　　ADXIPHONE_SetupSound(&sprm);
 *　　　　　　：
 *　　　　　ADX_Init();
 *　　　　　ADXT_SetDefSvrFreq(20);  // デフォルト周期を設定
 */
void CRIAPI ADXIPHONE_SetupSound(AdxiPhoneSprmSnd *sprm);


/* $func$ ADX サウンドのシャットダウン
 * [書　式] void CRIAPI ADXIPHONE_ShutdownSound(void)
 * [入　力] なし
 * [出　力] なし
 * [関数値] なし
 * [機　能] ・iPhone用サウンド処理の終了処理を行います。
 */
void CRIAPI ADXIPHONE_ShutdownSound(void);


/* $func$ 設定再生周波数の取得
 * [書　式] void CRIAPI ADXIPHONE_GetSfreqReal(ADXT adxt)
 * [入　力] ADXTハンドル
 * [出　力] なし
 * [関数値] なし
 * [機　能] ADXTハンドルに設定された再生周波数を取得します。
 */
CriSint32 CRIAPI ADXIPHONE_GetSfreqReal(ADXT adxt);


/* $func$ ADXメイン処理から呼ばれるユーザ関数の登録
 * [書　式] void CRIAPI ADXIPHONE_SetUsrMainFunc(void (*func)(void *obj), void *obj)
 * [入　力] func 実行する関数
 * [入　力] obj 実行する関数(func)の引数
 * [出　力] なし
 * [関数値] なし
 * [機　能] ADXフレームワークのADXユーザーメインスレッドからコールバックされる関数を
 *　　　　　設定します。
 *　　　　　ADXのスレッドフレームワーク使用時には、MainのRunLoopに負担をかけないよう、
 *　　　　　本関数で設定できる関数内でユーザーの処理を行うことを勧めます。
 */
void CRIAPI ADXIPHONE_SetUsrMainFunc(void (*func)(void *obj), void *obj);
    
/* $func$ ADX ミキシング用バッファサイズの取得
 * [書　式] CriSint32 ADXIPHONE_GetMixBufSizeBySvrFreq(CriSint32 svrfreq)
 * [入　力] svrfreq : ADXサーバ関数呼び出し周期(Hz)
 * [出　力] なし
 * [関数値] 指定周期での動作に必要なミキシング用バッファのサイズ
 * [機　能] ADX内部で使用するミキシング用バッファのサイズを取得します。
 *　　　　　本関数は基本的には使用する必要はありません。
 *　　　　　ADXサーバ関数の呼び出し周期をデフォルトの60Hzよりも低くした場合、
 *　　　　　再生音が途切れる可能性があります。
 *　　　　　　その場合、本関数で取得されるサイズ分のメモリ領域をミキシング用
 *　　　　　バッファとして ADXIPHONE_SetupSound 関数で設定する必要があります
 *　　　　　さらに、ADXT_SetDefSvrFreq関数でサーバ関数の呼び出し周期を設定します。
 * [使用例]
 *　　　　　// ADXサーバ関数呼び出し周期を 20Hz にする場合
 *　　　　　AdxiPhoneSprmSnd sprm;
 *　　　　　memset(&sprm, 0, sizeof(sprm));
 *　　　　　sprm.svrfreq = 20;
 *　　　　　sprm.mixbuf_size = ADXIPHONE_GetBufferSizeBySvrFreq(sprm.svrfreq);
 *　　　　　sprm.mixbuf_ptr = malloc(sprm.mixbuf_size);
 *　　　　　ADXIPHONE_SetupSound(&sprm);
 *　　　　　　：
 *　　　　　ADX_Init();
 *　　　　　ADXT_SetDefSvrFreq(20);  // デフォルト周期を設定
 */
CriSint32 CRIAPI ADXIPHONE_GetMixBufSizeBySvrFreq(CriSint32 svrfreq);


/* $func$ オーディオ処理の開始
 * [書　式] void ADXIPHONE_StartSound(void)
 * [入　力] なし
 * [出　力] なし
 * [関数値] なし
 * [機　能] AudioSessionのInterruption Callbak関数から呼び出すための関数です。
 *　　　　　オーディオ処理を開始します。
 *		   本関数を呼び出す前に、AudioSessionのパメラータ設定とアクティベイトを行ってください。
 * [使用例]
 *　　　　　// AudioSession Interruption Callbak
 *         static void interruptionListenerCallback(void *inUserData, UInt32 interruptionState)
 *         {
 *	           if (interruptionState == kAudioSessionBeginInterruption) {
 *                 // オーディオ処理の停止
 *	               ADXIPHONE_StopSound();
 *	           }
 *
 *	           if (interruptionState == kAudioSessionEndInterruption) {
 *                 // AudioSessionのプロパティ設定とアクティベイト
 *		           setupAudioSession();
 *                 // オーディオ処理の開始
 *		           ADXIPHONE_StartSound();
 *	           }
 *         }
 *
 *         // AudioSessionのプロパティ設定とアクティベイト
 *         static void setupAudioSession(void)
 *         {
 *            :
 *         }
 */
void CRIAPI ADXIPHONE_StartSound(void);

/* $func$ オーディオ処理の停止
 * [書　式] void ADXIPHONE_StopSound(void)
 * [入　力] なし
 * [出　力] なし
 * [関数値] なし
 * [機　能] AudioSessionのInterruption Callbak関数から呼び出すための関数です。
 *　　　　　オーディオ処理を停止します。
 * [使用例]
 *         上記 ADXIPHONE_StartSound()を参照してください。
 */
void CRIAPI ADXIPHONE_StopSound(void);



/* -------------------------------------------------------------------------
 * タイムストレッチAPI
 * ---------------------------------------------------------------------- */
/* $func$ タイムストレッチ機能のアタッチ
 * [書　式] void CRIAPI ADXIPHONE_AttachTimeStretch(ADXT adxt, void* work, Sint32 work_size)
 * [入　力] adxt        : ADXTハンドル
 * [入　力] work        : ワーク領域
 * [入　力] work_size   : ワーク領域サイズ
 * [出　力] なし
 * [関数値] なし
 * [機　能] ADXTハンドルにタイムストレッチ機能を追加します。
 *          定数ADXIPHONE_WORKSIZE_TSで示されるサイズ分のワーク領域を確保して指定してください。
 */
void CRIAPI ADXIPHONE_AttachTimeStretch(ADXT adxt, void* work, Sint32 work_size);

/* $func$ タイムストレッチ機能のデタッチ
 * [書　式] void CRIAPI ADXIPHONE_DetachTimeStretch(ADXT adxt)
 * [入　力] adxt        : ADXTハンドル
 * [出　力] なし
 * [関数値] なし
 * [機　能] ADXTハンドルに追加されたタイムストレッチ機能を取り外します。
 *          また、ADXT_Destroy関数呼び出し時には自動的にタイムストレッチ機能が
 *          デタッチされます。
 */
void CRIAPI ADXIPHONE_DetachTimeStretch(ADXT adxt);

/* $func$ タイムストレッチの伸縮率を設定
 * [書　式] void CRIAPI ADXIPHONE_SetTimeStretchRate(ADXT adxt, Sint32 rate, Sint32 unit_ms)
 * [入　力] adxt        : ADXTハンドル
 * [入　力] rate        : タイムストレッチ伸縮率(パーセント表記)
 * [入　力] unit_ms     : 処理単位(ミリ秒)
 * [出　力] なし
 * [関数値] なし
 * [機　能] 第２引数で再生時間の伸縮比を％で指定します。100[％]を基準に、
 *          小さいほど再生時間が短く（速く）、大きいほど再生時間が長く（遅く）なります。
 *          第３引数でタイムストレッチの処理単位時間(ミリ秒)を指定します。
 *          最適な処理単位時間は声質や会話のペースで異なります。
 *          通常は60msを指定してください。会話のペースが速い場合は70～80ms、
 *          遅い場合は40～50msを指定してください。
 */
void CRIAPI ADXIPHONE_SetTimeStretchRate(ADXT adxt, Sint32 rate, Sint32 unit_ms);


/* -------------------------------------------------------------------------
 * 特殊再生 API
 * ---------------------------------------------------------------------- */
/* $func$ 再生遅延無しのメモリ再生
 * [書　式] void ADXT_StartMemNoDelay(ADXT adxt, void *adxdat)
 * [入　力] adxt        : ADXTハンドル
 * [入　力] adxdat      : ADXデータへのポインタ
 * [出　力] なし
 * [関数値] なし
 * [機　能] 再生開始を指示してから、ADXの再生ステータスがADX_STAT_PLAYINGになるまでの
 *          遅延を無くした、メモリ上のADXデータの再生を開始する完了復帰型の関数です。
 *          ADX再生ステータスがPLAYINGになるまでの処理を一括して行います。
 * [注　意] 本科数は、再生スタータスがADX_STAT_PLAYINGになるまでの遅延を解消するものです。
 *          再生音がスピーカなどの、ターゲットの音声デバイスから出力されるまでには、
 *          サウンドドライバ側の再生バッファなどの遅延要因がありますので、完全に遅延が
 *          なくなる訳ではありません。
 */
void CRIAPI ADXT_StartMemNoDelay(ADXT adxt, void *adxdat);

/* $func$ 再生遅延無しのACXデータ再生
 * [書　式] void ADXT_StartMemIdxNoDelay(ADXT adxt, void *acx, Sint32 no)
 * [入　力] adxt        : ADXTハンドル
 * [入　力] acx         : ACXデータへのポインタ
 * [入　力] no          : 再生データのインデクス番号
 * [出　力] なし
 * [関数値] なし
 * [機　能] 再生開始を指示してから、ADXの再生ステータスがADX_STAT_PLAYINGになるまでの
 *          遅延を無くした、ACXデータの再生を開始する完了復帰型の関数です。
 *          ADX再生ステータスがPLAYINGになるまでの処理を一括して行います。
 * [注　意] 本科数は、再生スタータスがADX_STAT_PLAYINGになるまでの遅延を解消するものです。
 *          再生音がスピーカなどの、ターゲットの音声デバイスから出力されるまでには、
 *          サウンドドライバ側の再生バッファなどの遅延要因がありますので、完全に遅延が
 *          なくなる訳ではありません。
 *          サウンドドライバ側の再生バッファなどの遅延要因があります。
 */
void CRIAPI ADXT_StartMemIdxNoDelay(ADXT adxt, void *acx, Sint32 no);


#ifdef __cplusplus
}
#endif

/***************************************************************************
 ***************************************************************************/
enum {
	/* フレームワーク種別 */
	ADXM_FRAMEWORK_IPHONE_SINGLE_THREAD			= ADXM_FRAMEWORK_POSIX_SINGLE_THREAD,
	ADXM_FRAMEWORK_IPHONE_MULTI_THREAD			= ADXM_FRAMEWORK_POSIX_MULTI_THREAD,
	ADXM_FRAMEWORK_IPHONE_MULTI_THREAD_NO_PRIO	= ADXM_FRAMEWORK_POSIX_MULTI_THREAD_NO_PRIO
};
enum {
	/* デフォルト同期周波数x100					*/
	ADXIPHONE_DEF_VHZ100 = ADXPOSIX_DEF_VHZ100
};

enum {
	/* スピーカ指定子(Left/Right以外は将来拡張用) */
	ADXIPHONE_SPEAKER_FRONT_LEFT	= ADXT_SPEAKER_FRONT_LEFT,
	ADXIPHONE_SPEAKER_FRONT_RIGHT	= ADXT_SPEAKER_FRONT_RIGHT,
	ADXIPHONE_SPEAKER_FRONT_CENTER	= ADXT_SPEAKER_FRONT_CENTER,
	ADXIPHONE_SPEAKER_LOW_FREQUENCY	= ADXT_SPEAKER_LOW_FREQUENCY,
	ADXIPHONE_SPEAKER_BACK_LEFT 	= ADXT_SPEAKER_BACK_LEFT,
	ADXIPHONE_SPEAKER_BACK_RIGHT	= ADXT_SPEAKER_BACK_RIGHT,
};

/*	ANSIファイルシステムのセットアップパラメータ構造体	*/
#define AdxiPhoneSprmFs	AdxPosixSprmFs

/* ファイルシステムのセットアップ */
#define ADXIPHONE_SetupFileSystem(sprm)	ADXPOSIX_SetupFileSystem((sprm))
/* ファイルシステムのシャットダウン */
#define ADXIPHONE_ShutdownFileSystem()	ADXPOSIX_ShutdownFileSystem()

/* スピーカへのセンドレベル */
#define	ADXIPHONE_GetSendSpeakerLevel	ADXT_GetSendSpeakerLevel
#define	ADXIPHONE_SetSendSpeakerLevel	ADXT_SetSendSpeakerLevel

/* 外部からデータを流し込む際の口 */
#define ADXIPHONE_SetExtTapIn(func,obj)	ADXPOSIX_SetExtTapIn((func),(obj))
#define ADXIPHONE_CleanExtTapIn()	ADXPOSIX_CleanExtTapIn()


#endif	/* #ifndef _ADXIPHONE_H_INCLUDED */

/* --- end of file --- */
