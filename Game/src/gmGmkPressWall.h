// =======================================================================
/*!
	@file	gmPressWall.h
	@brief	ギミック 迫る壁＠ゾーン３と４

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkPressWall.h 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef GM_GMK_PRESS_WALL_
#define GM_GMK_PRESS_WALL_


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
// GmPressWallInit
/*!
 *	ギミック 迫る壁＠ゾーン３と４ 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkPressWallInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);

// ===========================================================================
// GmGmkPressWallStopInit
/*!
	ギミック 迫る壁＠ゾーン３と４ 初期化関数
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note

 */
// ===========================================================================
extern OBS_OBJECT_WORK* GmGmkPressWallStopInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


// ===========================================================================
// GmGmkPressWallControlInit
/*!
	ギミック 迫る壁＠ゾーン３と４ 初期化関数
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note

 */
// ===========================================================================
OBS_OBJECT_WORK* GmGmkPressWallControlerInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


// ===========================================================================
// GmPressWallBuild
/*!
	ギミック 迫る壁＠ゾーン３と４ データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkPressWallBuild(void);


// ===========================================================================
// GmPressWallFlush
/*!
	ギミック 迫る壁＠ゾーン３と４ データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkPressWallFlush(void);



#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_PRESS_WALL_

//----- Include Files -------------------------------------------------------
