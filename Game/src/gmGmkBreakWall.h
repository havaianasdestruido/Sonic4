// =======================================================================
/*!
	@file	gmGmkBreakWall.h
	@brief	ギミック 破壊可能な壁

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkBreakWall.h 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef GM_GMK_BREAKWALL_H_
#define GM_GMK_BREAKWALL_H_


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
// GmGmkBreakWall??Init
/*!
 *	ギミック 破壊可能壁 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkBreakWall_L1Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
OBS_OBJECT_WORK* GmGmkBreakWall_L2Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
OBS_OBJECT_WORK* GmGmkBreakWall_R1Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
OBS_OBJECT_WORK* GmGmkBreakWall_R2Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
OBS_OBJECT_WORK* GmGmkBreakWall_C1Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
OBS_OBJECT_WORK* GmGmkBreakWall_C2Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
OBS_OBJECT_WORK* GmGmkBreakWall_C1_H_Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);

// ==========================================================================
// GmGmkBreakFloorInit
/*!
 *	ギミック 破壊可能床 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkBreakFloorInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


// ===========================================================================
// GmGmkBreakWallBuild
/*!
	ギミック 破壊可能壁 データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkBreakWallBuild(void);
// ===========================================================================


// ===========================================================================
// GmGmkBreakLandFlush
/*!
	ギミック 崩壊足場 データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkBreakWallFlush(void);
// ===========================================================================



#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_NEEDLE_H_

//----- Include Files -------------------------------------------------------
