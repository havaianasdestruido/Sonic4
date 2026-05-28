// =======================================================================
/*!
  @file	akUtil.h
  @brief ユーティリティ

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: akUtil.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef AK_UTIL_H_
#define AK_UTIL_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/
// =======================================================================
// AkUtilFrame60ToTime
/*!
  1秒60フレーム計算でフレームを時間に変換する
 
  @param frame	[in]	変換するフレーム数
  @param min	[out]	分 格納領域 (NULL可)
  @param sec	[out]	秒 格納領域 (NULL可)
  @param msec	[out]	ミリ秒 格納領域 (NULL可)
  
  @note
      59分 59秒 99 を超えていた場合は丸めます。
  ※nlUtilからの移植です。
 */
// =======================================================================
extern void AkUtilFrame60ToTime(Uint32 frame, Uint16 *min, Uint16 *sec, Uint16 *msec);

// =======================================================================
// AkUtilNumValueToDigits
/*!
  整数値を数字列に変換
 
  @param val		[in]	数値
  @param digit_list	[out]	数字 格納領域
  @param digit_num	[in]	数字桁数
  @param radix		[in]	基数（デフォルト=10）
  
  @return 溢れた数（溢れなかった場合は0）
  
  @note
  valを各桁の数字に変換して、要素数digit_numの配列に格納します。
  （インデックス0 が一の位になります。）
  指定の桁数で表現できない数値だった場合は溢れた分を戻り値として返します。
  (e.g. digit_num=2でval=1234を指定した場合、1200を返します。)
  溢れなかった場合は0を返します。
  radixを指定するとその基数で桁分解が行われます。
 */
// =======================================================================
extern Sint32 AkUtilNumValueToDigits(Sint32 val, Sint32 *digit_list, Sint32 digit_num,
									 const Sint32 radix=10);


#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* AK_UTIL_H_ */
