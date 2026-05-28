// ==========================================================================
/*!
  @file gmGmkTarzanRope.h
  @brief ÉMÉ~ÉbÉNÅ@É^Å[ÉUÉìÉçÅ[Év

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkTarzanRope.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_GMK_TARZAN_ROPE_H_
#define GM_GMK_TARZAN_ROPE_H_


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
// GmGmkTarzanRopeBuild
/*!
 *	ÉMÉ~ÉbÉN É^Å[ÉUÉìÉçÅ[Év ÉfÅ[É^ç\íz
 */
// ==========================================================================
extern void GmGmkTarzanRopeBuild(void);

// ==========================================================================
// GmGmkTarzanRopeFlush
/*!
 *	ÉMÉ~ÉbÉN É^Å[ÉUÉìÉçÅ[Év ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
extern void GmGmkTarzanRopeFlush(void);

// ==========================================================================
// GmGmkWaterSliderInit
/*!
 *	ÉMÉ~ÉbÉNèâä˙âªä÷êîÅ@É^Å[ÉUÉìÉçÅ[Év
 *
 *	@param eve_rec	[io] ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param pos_x	[in] èoåªç¿ïW
 *	@param pos_y	[in] 
 *	@param type		[in] èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkTarzanRopeInit( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

#if	defined(__cplusplus)
} /* extern "C" */
#endif

//----- Include Files -------------------------------------------------------

#endif // GM_GMK_TARZAN_ROPE_H_
