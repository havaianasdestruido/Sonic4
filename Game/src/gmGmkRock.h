// ==========================================================================
/*!
  @file gmGmkRock.h
  @brief ƒMƒ~ƒbƒN‘åŠâ

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkRock.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_GMK_ROCK_H_
#define GM_GMK_ROCK_H_


#if	defined(__cplusplus)
extern "C" {
#endif

//----- Include Files -------------------------------------------------------

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// GmGmkRockBuild
/*!
 *	ƒMƒ~ƒbƒN ‘åŠâ ƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmGmkRockBuild(void);

// ==========================================================================
// GmGmkRockFlush
/*!
 *	ƒMƒ~ƒbƒN ‘åŠâ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
extern void GmGmkRockFlush(void);

// ==========================================================================
// GmGmkRockChaseManagerInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”i’ÇÕ‘åŠâŠÇ—j
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkRockChaseManagerInit( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

// ==========================================================================
// GmGmkRockChaseInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”i‘åŠâj
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkRockChaseInit( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

// ==========================================================================
// GmGmkRockFallManagerInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”i—‰º‘åŠâŠÇ—j
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkRockFallManagerInit( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

// ==========================================================================
// GmGmkRockFallInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”i—‰º‘åŠâj
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkRockFallInit( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

// ==========================================================================
// GmGmkRockHookInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”i‘åŠâ‚ğx‚¦‚é‘•’uj
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkRockHookInit( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );


#if	defined(__cplusplus)
} /* extern "C" */
#endif

//----- Include Files -------------------------------------------------------

#endif // GM_GMK_ROCK_H_
