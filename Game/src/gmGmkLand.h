// ================================================================
/*!
  @file gmGmkLand.h
  @brief •‚“‡

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkLand.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ================================================================
/*
 * $Log: $
 */

#ifndef GM_GMK_LAND_H_
#define GM_GMK_LAND_H_

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
// GmGmkLandBuild
/*!
 *	ƒMƒ~ƒbƒN •‚“‡ ƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmGmkLandBuild(void);

// ==========================================================================
// GmGmkLandFlush
/*!
 *	ƒMƒ~ƒbƒN •‚“‡ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
extern void GmGmkLandFlush(void);

// ==========================================================================
// GmGmkLandInit
/*!
 *	ƒMƒ~ƒbƒN •‚“‡ ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkLandInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);

// ==========================================================================
// GmGmkZ3LandPulleyInit
/*!
 *	ƒMƒ~ƒbƒN •‚“‡ •tŠŠÔ ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkZ3LandPulleyInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);

// ==========================================================================
// GmGmkZ3LandRopeInit
/*!
 *	ƒMƒ~ƒbƒN •‚“‡ •tƒ[ƒv ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkZ3LandRopeInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_LAND_H_

//----- Include Files -------------------------------------------------------
