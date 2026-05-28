/*****************************************************************************/
/*      amIPhoneAdx.h                  Author : Syuichi Gotou                */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* iPhone用ADXライブラリ ヘッダ                                              */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 091216-       0.01   first version                                        */
/*****************************************************************************/

#ifndef _H_AMIPHONE_ADX
#define _H_AMIPHONE_ADX

//----- Include Files --------------------------------------------------

#include <cri_mw.h>

//----- Macros ---------------------------------------------------------

//! 最大ハンドル数
#define AMD_ADX_HANDLE_MAX		(_am_adx_handle_max)

//! 最大ストリーム数
#define AMD_ADX_STREAM_MAX		(_am_adx_stream_max)

//----- Definitions ----------------------------------------------------

#define AMD_ADX_STAT_STOP		(ADXT_STAT_STOP)		//!< 0 : 停止中
#define AMD_ADX_STAT_DECINFO	(ADXT_STAT_DECINFO)		//!< 1 : 情報取得中
#define AMD_ADX_STAT_PREP		(ADXT_STAT_PREP)		//!< 2 : 再生準備中
#define AMD_ADX_STAT_PLAYING	(ADXT_STAT_PLAYING)		//!< 3 : デコード & 再生中
#define AMD_ADX_STAT_DECEND		(ADXT_STAT_DECEND)		//!< 4 : デコード終了
#define AMD_ADX_STAT_PLAYEND	(ADXT_STAT_PLAYEND)		//!< 5 : 再生終了

//----- Enum Definitions -----------------------------------------------

#define AMD_ADX_HANDLE_BLANK	(-1)		//!< 空きハンドルを使用

#define AMD_ADX_FLAG_OVERWRITE	(0)			//!< 上書き
#define AMD_ADX_FLAG_FIRST		(-3)		//!< 最初に使用したハンドルを上書き
#define AMD_ADX_FLAG_NOPLAY		(-1)		//!< 再生しない or 再生しなかった
#define AMD_ADX_FLAG_READING	(-2)		//!< パーティション情報のロード中

// 旧バージョンとの互換用(使用しないこと)
#define AMD_ADX_BLANK			AMD_ADX_HANDLE_BLANK
#define AMD_ADX_OVERWRITE		AMD_ADX_FLAG_OVERWRITE
#define AMD_ADX_FIRST			AMD_ADX_FLAG_FIRST
#define AMD_ADX_NOPLAY			AMD_ADX_FLAG_NOPLAY
#define AMD_ADX_READING			AMD_ADX_FLAG_READING

//----- Type Definitions -----------------------------------------------

//! ADX管理ワーク
typedef struct _AMS_ADXT {
	ADXT		adxt;				//! ADXハンドル
	Sint32		status;				//! ハンドルの状態
	float		vol;				//! ボリューム (ローカル)
	Sint32		volDb;				//! ボリューム (dB)
	Sint8*		pWork;
	Uint32		reserved[3];
} AMS_ADXT; // 32 byte

//! 初期化パラメータ (amInitAdxSystemの引数で使用)
typedef struct _AMS_ADX_PARAM {
	Sint32			handleNum;		//!< 最大ハンドル数
	Sint32			streamNum;		//!< 最大ストリーム数
	Sint32			channelMax;		//!< 
	Sint32			frequencyMax;	//!< 
	char*			fcacheName;		//!< ファイルキャッシュリスト名 (NULL指定可能)
	Sint32			vsync_prio;		//!< Vsyncスレッド優先度(負数:デフォルト22)
	Sint32			fs_prio;		//!< ファイルスレッド優先度(負数:デフォルト24)
	Sint32			idle_prio;		//!< アイドルスレッド優先度(負数:デフォルト50)
} AMS_ADX_PARAM;

//----- External Variables ---------------------------------------------

extern Sint32		_am_adx_handle_max;
extern Sint32		_am_adx_stream_max;

//----- External Definitions -------------------------------------------

//! amAdxInitSystem
/*!
	ADXシステムの初期化

	@param	pParam	[in]	初期化パラメータへのポインタ

	@note
	起動時に一度だけ呼び出して下さい。
 */
extern void amAdxInitSystem(AMS_ADX_PARAM* pParam);

//! amAdxExitSystem
/*!
	ADXシステムの終了

	@note
	終了時に一度だけ呼び出して下さい。
 */
extern void amAdxExitSystem(void);

//! amAdxExcuteMain
extern void amAdxExcuteMain(void);

extern Sint32 amAdxGetStatus(Sint32 handle);

//! ADX再生 (ストリーム) ファイルパス指定
/*!
	@param	handle		[in]	ADXハンドル (0～)
	@param	fileName	[in]	ADXファイル名
	@param	flag		[in]	制御フラグ (省略可)

	@retval	AMD_ADX_FLAG_NOPLAY	再生しなかった
	@retval	0～					再生するADXハンドル
 */
extern Sint32 amAdxPlay(
	Sint32 handle, const char* fileName, Sint32 flag=AMD_ADX_FLAG_OVERWRITE);

//! ADX再生 (ストリーム) パーティション&パス指定
/*!
	@param	handle		[in]	ADXハンドル (0～)
	@param	partId		[in]	パーティションID(CPKファイルID)
	@param	fileName	[in]	ADXファイル名(CPKコンテンツファイル名)
	@param	flag		[in]	制御フラグ (省略可)

	@retval	AMD_ADX_FLAG_NOPLAY	再生しなかった
	@retval	0～					再生するADXハンドル
 */
extern Sint32 amAdxPlayCpk(
	Sint32 handle, Sint32 partId, const char* fileName,
	Sint32 flag=AMD_ADX_FLAG_OVERWRITE);

//! ADX再生 (オンメモリ)
/*!
	@param	handle		[in]	ADXハンドル (0～)
	@param	pBuf		[in]	ADXデータへのポインタ
	@param	flag		[in]	制御フラグ (省略可)

	@retval	AMD_ADX_FLAG_NOPLAY	再生しなかった
	@retval	0～					再生するADXハンドル
 */
extern Sint32 amAdxPlayMem(
	Sint32 handle, void* pBuf, Sint32 flag=AMD_ADX_FLAG_OVERWRITE);
	
//! ACX再生 (オンメモリ)
/*!
	@param	handle		[in]	ACXハンドル (0～)
	@param	pBuf		[in]	ACXデータへのポインタ
	@param	id			[in]	再生ID（0～）
	@param	flag		[in]	制御フラグ (省略可)

	@retval	AMD_ADX_FLAG_NOPLAY	再生しなかった
	@retval	0～					再生するACXハンドル
 */
extern Sint32 amAdxPlayMemIdx(
	Sint32 handle, void* pBuf, CriSint32 Id, Sint32 flag=AMD_ADX_FLAG_OVERWRITE);

//! ADXの停止
/*!
	指定ハンドルのADXを停止します。

	@param	handle	[in]	ADXハンドル (0～)
 */
extern void amAdxStop(Sint32 handle);

//! ADXの一時停止
/*!
	指定ハンドルのADXを一時停止/再開します。

	@param	handle	[in]	ADXハンドル (0～)
	@param	flag	[in]	制御フラグ (1:一時停止 0:再開)
 */
extern void amAdxPause(Sint32 handle, Sint32 flag);

//! ADXの停止判定
/*!
	指定ハンドルのADXが停止中か判定します。

	@param	handle	[in]	ADXハンドル (0～)

	@retval	1	停止中
	@retval	0	停止中ではない
 */
extern Sint32 amAdxIsStop(Sint32 handle);

//! ADXの全停止判定
/*!
	全ハンドルのADXが停止中か判定します。

	@retval	1	停止中
	@retval	0	停止中ではない
 */
extern Sint32 amAdxIsStopAll(void);

//! ADXのマスターボリューム設定
/*!
	ADXのマスターボリュームを取得します。

	@return マスターボリューム
 */
extern float amAdxGetMasterVol(void);

//! ADXのマスターボリューム設定
/*!
	ADXのマスターボリュームを設定します。

	@param	vol		[in]	ボリューム倍率 (0.0f～)

	@note
	初期状態では1.0fに設定されています。
 */
extern void amAdxSetMasterVol(float vol);

//! ADXのボリューム取得
/*!
	指定ハンドルのADXボリュームを取得します。

	@param	handle	[in]	ADXハンドル (0～)

	@return	ボリューム倍率 (0.0f ～)
 */
extern float amAdxGetOutVol(Sint32 handle);

//! ADXのボリューム設定
/*!
	指定ハンドルのADXボリュームを設定します。

	@param	handle	[in]	ADXハンドル (0～)
	@param	vol		[in]	ボリューム倍率 (0.0f～)
 */
extern void amAdxSetOutVol(Sint32 handle, float vol);

#endif // _H_AMIPHONE_ADX
