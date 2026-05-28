// ================================================================
/*!
  @file gmGmkSsCircle.h
  @brief SpecialStage ä€íå

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSsCircle.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ================================================================
/*
 * $Log: $
 */

#ifndef GM_GMK_SSCIRCLE_H_
#define GM_GMK_SSCIRCLE_H_

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
// GmGmkSsCircleBuild
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ä€íå ÉfÅ[É^ç\íz
 */
// ==========================================================================
extern void GmGmkSsCircleBuild(void);

// ==========================================================================
// GmGmkSsCircleFlush
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ä€íå ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
extern void GmGmkSsCircleFlush(void);

// ==========================================================================
// GmGmkSsCircleInit
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ä€íå èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkSsCircleInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);

// ==========================================================================
// GmGmkSsOnewayThrough
/*!
 *	ÉMÉ~ÉbÉN SpecialStage àÍï˚í çsêßå¿í âﬂ
 */
// ==========================================================================
extern void GmGmkSsOnewayThrough(u32 sw_no);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_SSCIRCLE_H_

