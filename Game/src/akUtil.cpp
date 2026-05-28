// =======================================================================
/*!
  @file	akUtil.h
  @brief ユーティリティ

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: akUtil.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akUtil.h"

/*------ Macros --------------------------------------------------------*/
#define AKD_UTIL_FRAME60_TO_TIME_MAX_FRAME		(59 * 3600 + 59 * 60 + 59)	//!< 60フレーム >> 時間 変換時最大フレーム数

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/

/*------ Global Functions ----------------------------------------------*/
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
void AkUtilFrame60ToTime(Uint32 frame, Uint16 *min, Uint16 *sec, Uint16 *msec)
{
	u16		l_min, l_sec, l_msec;

	if (frame > AKD_UTIL_FRAME60_TO_TIME_MAX_FRAME) {
		frame = AKD_UTIL_FRAME60_TO_TIME_MAX_FRAME;
	}

	/* 分 */
	l_min = (Uint16)((Sint32)frame / (60 * 60));
	frame -= l_min * 60 * 60;

	/* 秒 */
	l_sec = (Uint16)((Sint32)frame / 60);
	frame -= l_sec * 60;

	/* ミリ秒 */
	// 60分割を100分割にする 0x01aa =/= 1.66...
	// カンスト時にセンチ秒を99にするためにの
	// 0x01b1 = 100 / 59 で計算しています
	l_msec = (u16)((frame * 0x01b1) >> 8);
	if (l_msec >= 100) {
		l_msec = 99;
	}

	if (min) {
		*min = l_min;
	}
	if (sec) {
		*sec = l_sec;
	}
	if (msec) {
		*msec = l_msec;
	}
}


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
Sint32 AkUtilNumValueToDigits(Sint32 val, Sint32 *digit_list, Sint32 digit_num,
							  const Sint32 radix/*=10*/)
{
	Sint32	work_val	= val;
	Sint32	remainder;
	Sint32	place	= 1;	// 基数^(桁-1)
	
	MTM_ASSERT(digit_list);
	MTM_ASSERT(digit_num > 0);
	
	for (Sint32 n = 0; n < digit_num; ++n) {
		remainder	= work_val % (place * radix);
		digit_list[n]	= (Sint32)(remainder / place);
		work_val	= work_val - remainder;
		place	*= radix;
	}
	
	MTM_ASSERT(work_val >= 0);
	
	return work_val;
}


/*------ Static Functions ----------------------------------------------*/
// =======================================================================
// test_func
/*!
  関数機能
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return 返値説明
 
  @note
  補足説明
 */
// =======================================================================
