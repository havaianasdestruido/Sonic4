// =======================================================================
/*!
	@file	gmGmkUpBumper.h
	@brief	ギミック 登るバンパー＠ゾーン４工場

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkUpBumper.h 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef GM_GMK_UPBUMPER_H
#define GM_GMK_UPBUMPER_H


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
// GmGmkUpBumper?Init
/*!
 *	ギミック 登るバンパー＠ゾーン４工場 初期化関数
 *	GmGmkUpBumperLInit 左くっつき
 *	GmGmkUpBumperRInit 右くっつき
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkUpBumperLInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkUpBumperRInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


// ===========================================================================
// GmGmkUpBumperBuild
/*!
	ギミック 登るバンパー＠ゾーン４工場 データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkUpBumperBuild(void);


// ===========================================================================
// GmGmkUpBumperFlush
/*!
	ギミック 登るバンパー＠ゾーン４工場 データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkUpBumperFlush(void);



#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_NEEDLE_H_

//----- Include Files -------------------------------------------------------
