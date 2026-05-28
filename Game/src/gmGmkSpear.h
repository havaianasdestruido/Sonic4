// =======================================================================
/*!
	@file	gmGmkSpear.h
	@brief	ギミック 槍＠ゾーン３とゾーン４も

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkSpear.h 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef GM_GMK_SPEAR_H
#define GM_GMK_SPEAR_H


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
// GmGmkSpear?Init
/*!
 *	ギミック 槍＠ゾーン３とゾーン４も 初期化関数
 *	GmGmkSpearUInit 上向き
 *	GmGmkSpearDInit 下向き
 *	GmGmkSpearLInit 左向き
 *	GmGmkSpearRInit 右向き
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkSpearUInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSpearDInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSpearLInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSpearRInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


// ===========================================================================
// GmGmkSpearBuild
/*!
	ギミック 槍＠ゾーン３とゾーン４も データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkSpearBuild(void);


// ===========================================================================
// GmGmkSpearFlush
/*!
	ギミック 槍＠ゾーン３とゾーン４も データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkSpearFlush(void);



#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_SPEAR_H

//----- Include Files -------------------------------------------------------
