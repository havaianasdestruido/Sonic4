// =======================================================================
/*!
	@file	gmGmkStopper.h
	@brief	ギミック ストッパー＠ゾーン２ピンボール

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkStopper.h 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef GM_GMK_STOPPER_H
#define GM_GMK_STOPPER_H


//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// GmGmkStopper?Init
/*!
 *	ギミック ストッパー＠ゾーン２ピンボール 初期化関数
 *	GmGmkStopperUInit 上向き
 *	GmGmkStopperDInit 下向き
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkStopperNormInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkStopperSlotInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


// ===========================================================================
// GmGmkStopperBuild
/*!
	ギミック ストッパー＠ゾーン２ピンボール データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkStopperBuild(void);


// ===========================================================================
// GmGmkStopperFlush
/*!
	ギミック ストッパー＠ゾーン２ピンボール データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkStopperFlush(void);



#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_NEEDLE_H_

//----- Include Files -------------------------------------------------------
