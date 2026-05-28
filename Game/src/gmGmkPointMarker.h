// =======================================================================
/*!
	@file	gmGmkPointMarker.h
	@brief	ギミック ポイントマーカー

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkPointMarker.h 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef GM_GMK_POINTMARKER_H_
#define GM_GMK_POINTMARKER_H_


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
// GmGmkPointMarkerInit
/*!
 *	ギミック ポイントマーカー 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkPointMarkerInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


// ===========================================================================
// GmGmkPointMarkerBuild
/*!
	ギミック ポイントマーカー データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkPointMarkerBuild(void);


// ===========================================================================
// GmGmkPointMarkerFlush
/*!
	ギミック ポイントマーカー データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkPointMarkerFlush(void);



#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_NEEDLE_H_

//----- Include Files -------------------------------------------------------
