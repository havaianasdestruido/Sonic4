// ================================================================
/*!
  @file gmGmkSsOblong.h
  @brief ÉMÉ~ÉbÉNRingGateèIí[íå

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSsOblong.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ================================================================

#ifndef GM_GMK_SSOBLONG_H_
#define GM_GMK_SSOBLONG_H_

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
// GmGmkSsOblongBuild
/*!
 *	ÉMÉ~ÉbÉN SpecialStage RingGateèIí[íå ÉfÅ[É^ç\íz
 */
// ==========================================================================
extern void GmGmkSsOblongBuild(void);
// ==========================================================================
// GmGmkSsOblongFlush
/*!
 *	ÉMÉ~ÉbÉN SpecialStage RingGateèIí[íå ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
extern void GmGmkSsOblongFlush(void);
// ==========================================================================
// GmGmkSsOblongInit
/*!
 *	ÉMÉ~ÉbÉN SpecialStage RingGateèIí[íå èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkSsOblongInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_SSOBLONG_H_

