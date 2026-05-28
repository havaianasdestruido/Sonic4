// =======================================================================
/*!
	@file	gmGmkBreakLand.h
	@brief	ギミック 崩壊足場

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkBreakLand.h 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef GM_GMK_BREAKLAND_H_
#define GM_GMK_BREAKLAND_H_


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
// GmGmkBreakLandInit
/*!
 *	ギミック 崩壊足場 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkBreakLandRInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkBreakLandLInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


// ===========================================================================
// GmGmkBreakLandBuild
/*!
	ギミック 崩壊足場 データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkBreakLandBuild(void);


// ===========================================================================
// GmGmkBreakLandFlush
/*!
	ギミック 崩壊足場 データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkBreakLandFlush(void);

// ==========================================================================
// GmGmkBreakLandSetLight
/*!
 *	ギミック 崩壊足場 ライト設定
 */
// ==========================================================================
extern void GmGmkBreakLandSetLight(void);


#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_NEEDLE_H_

//----- Include Files -------------------------------------------------------
