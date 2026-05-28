// ================================================================
/*!
  @file gmGmkPressPillar.h
  @brief ”—‚èo‚·’Œ

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkPressPillar.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ================================================================
#ifndef GM_GMK_PRESSPILLAR_H_
#define GM_GMK_PRESSPILLAR_H_

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
// GmGmkPressPillarBuild
/*!
 *	ƒMƒ~ƒbƒN ”—‚èo‚·’Œ ƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmGmkPressPillarBuild(void);

// ==========================================================================
// GmGmkPressPillarFlush
/*!
 *	ƒMƒ~ƒbƒN ”—‚èo‚·’Œ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
extern void GmGmkPressPillarFlush(void);

// ==========================================================================
// GmGmkPressPillarInit
/*!
 *	ƒMƒ~ƒbƒN ”—‚èo‚·’Œ ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkPressPillarInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);

// ==========================================================================
// GmGmkPressPillarStartup
/*!
 *	ƒMƒ~ƒbƒN ”—‚èo‚·’Œ ‹N“®
 *
 *	@param	id_num	[in]	‹N“®ID”Ô†
 */
// ==========================================================================
extern void GmGmkPressPillarStartup(u16 id_num);

// ==========================================================================
// GmGmkPressPillarClear
/*!
 *	ƒMƒ~ƒbƒN ”—‚èo‚·’Œ ‹N“®î•ñƒNƒŠƒA
 *
 *	@param	id_num	[in]	‹N“®ID”Ô†
 */
// ==========================================================================
extern void GmGmkPressPillarClear(void);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_PRESSPILLAR_H_

