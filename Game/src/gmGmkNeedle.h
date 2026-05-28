// =======================================================================
/*!
	@file	gmGmkNeedle.h
	@brief	ギミック 針

	@author Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: gmGmkNeedle.h 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef GM_GMK_NEEDLE_H_
#define GM_GMK_NEEDLE_H_


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
// GmGmkNeedleBuild
/*!
	ギミック トゲ データ構築
 */
// ==========================================================================
extern void GmGmkNeedleBuild(void);

// ==========================================================================
// GmGmkNeedleFlush
/*!
	ギミック トゲ データ片付け
 */
// ==========================================================================
extern void GmGmkNeedleFlush(void);

// ==========================================================================
// GmGmkNeedleInit
/*!
 *	ギミック 針 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkNeedleInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);

// ==========================================================================
// GmGmkActNeedleInit
/*!
 *	ギミック 出入り針 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkActNeedleInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


extern OBS_OBJECT_WORK* GmGmkBackNeedleInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkStandNeedleInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);



// ==========================================================================
// GmGmkNeedleSetLight
/*!
 *	ギミック 針 ライト設定
 */
// ==========================================================================
extern void GmGmkNeedleSetLight(void);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_NEEDLE_H_

//----- Include Files -------------------------------------------------------
