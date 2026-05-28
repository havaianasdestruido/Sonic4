// =======================================================================
/*!
	@file	gmSteamPipe.h
	@brief	ギミック スチームパイプ＠ゾーン４工場

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkSteamPipe.h 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef GM_GMK_STEAMPIPE_H_
#define GM_GMK_STEAMPIPE_H_


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
// GmSteamPipeInit
/*!
 *	ギミック スチームパイプ＠ゾーン４工場 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkSteamPipeGateRInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSteamPipeGateLInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSteamPipeGateEInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSteamPipeA1Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSteamPipeA2Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSteamPipeA3Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSteamPipeA4Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSteamPipeB1Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSteamPipeB2Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSteamPipeB3Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSteamPipeB4Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSteamPipeJ1Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSteamPipeJ2Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSteamPipeJ3Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);
extern OBS_OBJECT_WORK* GmGmkSteamPipeJ4Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


// ===========================================================================
// GmSteamPipeBuild
/*!
	ギミック スチームパイプ＠ゾーン４工場 データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkSteamPipeBuild(void);


// ===========================================================================
// GmSteamPipeFlush
/*!
	ギミック スチームパイプ＠ゾーン４工場 データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkSteamPipeFlush(void);



#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_STEAMPIPE_H_

//----- Include Files -------------------------------------------------------
