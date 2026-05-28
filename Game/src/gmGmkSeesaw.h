// =======================================================================
/*!
	@file	gmGmkSeesaw.h
	@brief	ギミック シーソー＠ゾーン４工場

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkSeesaw.h 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef GM_GMK_SEESAW_H
#define GM_GMK_SEESAW_H


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
// GmGmkSeesaw?Init
/*!
 *	ギミック シーソー＠ゾーン４工場 初期化関数
 *	GmGmkSeesaw0Init   水平
 *	GmGmkSeesaw30Init  右傾き
 *	GmGmkSeesaw330Init 左肩向き
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkSeesaw0Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSeesaw30Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSeesaw330Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


// ===========================================================================
// GmGmkSeesawBuild
/*!
	ギミック シーソー＠ゾーン４工場 データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkSeesawBuild(void);


// ===========================================================================
// GmGmkSeesawFlush
/*!
	ギミック シーソー＠ゾーン４工場 データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkSeesawFlush(void);



#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_NEEDLE_H_

//----- Include Files -------------------------------------------------------
