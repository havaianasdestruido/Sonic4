// ================================================================
/*!
  @file gmGmkSplRing.h
  @brief ƒXƒyƒVƒƒƒ‹ƒŠƒ“ƒOiƒXƒyƒXƒe“üŒûƒŠƒ“ƒOj

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSplRing.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ================================================================
/*
 * $Log: $
 */

#ifndef GM_GMK_SPLRING_H_
#define GM_GMK_SPLRING_H_

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
// GmGmkSplRingBuild
/*!
 *	ƒMƒ~ƒbƒN SPLƒŠƒ“ƒO ƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmGmkSplRingBuild(void);

// ==========================================================================
// GmGmkSplRingFlush
/*!
 *	ƒMƒ~ƒbƒN SPLƒŠƒ“ƒO ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
extern void GmGmkSplRingFlush(void);

// ==========================================================================
// GmGmkSplRingMake
/*!
 *	ƒMƒ~ƒbƒN SPLƒŠƒ“ƒO¶¬ŠÖ”
 *	iƒvƒƒOƒ‰ƒ€‚©‚ç‚Ì¶¬ŒÄ‚Ño‚µ—pj
 *
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *
 *	@note	Šî–{“I‚ÉƒS[ƒ‹ƒpƒlƒ‹‚©‚ç‚ÌŒÄ‚Ño‚µ‚ğ‘z’èB
 *			¶¬ˆÊ’u‚ğw’è‚µ‚ÄSPLƒŠƒ“ƒO‚ğİ’u‚µ‚Ü‚·B
 *
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkSplRingMake(fx32 pos_x, fx32 pos_y);

// ==========================================================================
// GmGmkSplRingInit
/*!
 *	ƒMƒ~ƒbƒN SPLƒŠƒ“ƒO ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkSplRingInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_SPLRING_H_

