// =======================================================================
/*!
	@file	gmGmkSeesaw.h
	@brief	ギミック スプリングカタパ＠ゾーン２

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkSpCtplt.h 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef GM_GMK_SPCTPLT_H
#define GM_GMK_SPCTPLT_H


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
// GmGmkSpCtplt?Init
/*!
 *	ギミック スプリングカタパ＠ゾーン２ 初期化関数
 *	GmGmkSpCtplt0Init   水平
 *	GmGmkSpCtplt30Init  右傾き
 *	GmGmkSpCtplt330Init 左肩向き
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkSpCtplt0Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSpCtplt45Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSpCtplt315Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


// ===========================================================================
// GmGmkSpCtpltBuild
/*!
	ギミック スプリングカタパ＠ゾーン２ データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkSpCtpltBuild(void);


// ===========================================================================
// GmGmkSpCtpltFlush
/*!
	ギミック スプリングカタパ＠ゾーン２ データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkSpCtpltFlush(void);



#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_SPCTPLT_H

//----- Include Files -------------------------------------------------------
