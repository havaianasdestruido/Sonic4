// ==========================================================================
/*!
  @file gmGmkTruck.h
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmGmkTruck.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_GMK_TRUCK_H_
#define GM_GMK_TRUCK_H_


//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
// 前輪
#define GMD_GMK_TRUCK_FW_CENTER_X	(0.f)
#define GMD_GMK_TRUCK_FW_CENTER_Y	(8.f)
#define GMD_GMK_TRUCK_FW_CENTER_Z	(-5.f)
// 後輪
#define GMD_GMK_TRUCK_BW_CENTER_X	(0.f)
#define GMD_GMK_TRUCK_BW_CENTER_Y	(8.f)
#define GMD_GMK_TRUCK_BW_CENTER_Z	(5.f)
//static float trans_truck_x_1 = 0.f;//-19.2f/GMD_OBJ_DRAW_SCALE;
//static float trans_truck_y_1 = 8.f;///GMD_OBJ_DRAW_SCALE;
//static float trans_truck_z_1 = -5.0f;///GMD_OBJ_DRAW_SCALE;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// トロッコ
// ==========================================================================
// ==========================================================================
// GmGmkTruckBuild
/*!
 *	ギミック トロッコ データ構築
 */
// ==========================================================================
extern void GmGmkTruckBuild(void);

// ==========================================================================
// GmGmkTruckFlush
/*!
 *	ギミック トロッコ データ片付け
 */
// ==========================================================================
extern void GmGmkTruckFlush(void);

// ==========================================================================
// GmGmkTruckInit
/*!
 *	ギミック トロッコ 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *			
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkTruckInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);

// ==========================================================================
// GmGmkTruckNoLandingInit
/*!
 *	ギミック トロッコ接地不可 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *			
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkTruckNoLandingInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);

// ==========================================================================
// トロッコ重力
// ==========================================================================
// ==========================================================================
// GmGmkTruckGravityInit
/*!
 *	ギミック トロッコ重力 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *			
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkTruckGravityInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_TRUCK_H_

//----- Include Files -------------------------------------------------------
