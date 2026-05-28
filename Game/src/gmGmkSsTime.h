// ================================================================
/*!
  @file gmGmkSsTime.h
  @brief SpecialStage ŠÔƒpƒlƒ‹

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSsTime.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ================================================================
/*
 * $Log: $
 */

#ifndef GM_GMK_SSTIME_H_
#define GM_GMK_SSTIME_H_

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
// GmGmkSsTimeBuild
/*!
 *	ƒMƒ~ƒbƒN SpecialStage ŠÔƒpƒlƒ‹ ƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmGmkSsTimeBuild(void);

// ==========================================================================
// GmGmkSsTimeFlush
/*!
 *	ƒMƒ~ƒbƒN SpecialStage ŠÔƒpƒlƒ‹ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
extern void GmGmkSsTimeFlush(void);

// ==========================================================================
// GmGmkSsTimeInit
/*!
 *	ƒMƒ~ƒbƒN SpecialStage ŠÔƒpƒlƒ‹ ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkSsTimeInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_SSTIME_H_

