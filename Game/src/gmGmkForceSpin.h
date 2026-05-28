// =======================================================================
/*!
	@file	gmGmkForceSpin.h
	@brief	ギミック 強制スピン＠主にゾーン２ピンボール

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkForceSpin.h 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef GM_GMK_FORCESPIN_H
#define GM_GMK_FORCESPIN_H


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
// GmGmkForceSpinSetInit
/*!
 *	ギミック 強制スピンセット＠主にゾーン２ 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkForceSpinSetInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
// ==========================================================================
// GmGmkForceSpinResetInit
/*!
 *	ギミック 強制スピン解除＠主にゾーン２ 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkForceSpinResetInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_FORCESPIN_H

//----- Include Files -------------------------------------------------------
