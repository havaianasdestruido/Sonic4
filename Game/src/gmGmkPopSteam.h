// =======================================================================
/*!
	@file	gmGmkPopSteam.h
	@brief	ギミック ポップスチーム＠ゾーン４

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkPopSteam.h 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef GM_GMK_POPSTEAM_H
#define GM_GMK_POPSTEAM_H


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
// GmGmkPopSteam?Init
/*!
 *	ギミック ポップスチーム＠ゾーン４ 初期化関数
 *	GmGmkPopSteamUInit 上向き
 *	GmGmkPopSteamRInit 右向き
 *	GmGmkPopSteamDInit 下向き
 *	GmGmkPopSteamLInit 左向き
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkPopSteamUInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkPopSteamRInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkPopSteamDInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkPopSteamLInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


// ===========================================================================
// GmGmkPopSteamBuild
/*!
	ギミック ポップスチーム＠ゾーン４ データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkPopSteamBuild(void);


// ===========================================================================
// GmGmkPopSteamFlush
/*!
	ギミック ポップスチーム＠ゾーン３とゾーン４も データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkPopSteamFlush(void);



#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_POPSTEAM_H

//----- Include Files -------------------------------------------------------
