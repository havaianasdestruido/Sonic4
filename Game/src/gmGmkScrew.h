// ================================================================
/*!
  @file gmGmkScrew.h
  @brief ÉMÉ~ÉbÉN ÉRÅ[ÉNÉXÉNÉäÉÖÅ[

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkScrew.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ================================================================
/*
 * $Log: $
 */

#ifndef GM_GMK_SCREW_H_
#define GM_GMK_SCREW_H_

#if	defined(__cplusplus)
extern "C" {
#endif


//----- Include Files -------------------------------------------------------

//----- Definitions ---------------------------------------------------------

// GMS_EVE_RECORD_EVENT : flag
#define GMD_GMK_SCREW_EVE_FLAG_LEFT	(0x01)

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------

// ==========================================================================
// GmGmkScrewInit
/*!
 *	ÉMÉ~ÉbÉN ÉRÅ[ÉNÉXÉNÉäÉÖÅ[ èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkScrewInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_SCREW_H_

//----- Include Files -------------------------------------------------------
