// ================================================================
/*!
  @file gmGmkPulley.h
  @brief ŠŠÔ(Pulley)

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkPulley.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ================================================================
/*
 * $Log: $
 */

#ifndef GM_GMK_PULLEY_H_
#define GM_GMK_PULLEY_H_

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
// GmGmkPulleyBuild
/*!
 *	ƒMƒ~ƒbƒN ŠŠÔ ƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmGmkPulleyBuild(void);

// ==========================================================================
// GmGmkPulleyFlush
/*!
 *	ƒMƒ~ƒbƒN ŠŠÔ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
extern void GmGmkPulleyFlush(void);

// ==========================================================================
// GmGmkPulleyBaseInit
/*!
 *	ƒMƒ~ƒbƒN ŠŠÔ–{‘Ì ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkPulleyBaseInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);
// ==========================================================================
// GmGmkPulleyPoleLInit
/*!
 *	ƒMƒ~ƒbƒN ŠŠÔƒ|[ƒ‹¶ ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkPulleyPoleLInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);
// ==========================================================================
// GmGmkPulleyPoleRInit
/*!
 *	ƒMƒ~ƒbƒN ŠŠÔƒ|[ƒ‹‰E ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkPulleyPoleRInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);
// ==========================================================================
// GmGmkPulleyRopeFInit
/*!
 *	ƒMƒ~ƒbƒN ŠŠÔƒ[ƒv…•½ ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkPulleyRopeFInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);
// ==========================================================================
// GmGmkPulleyRopeTInit
/*!
 *	ƒMƒ~ƒbƒN ŠŠÔƒ[ƒvÎ‚ß ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkPulleyRopeTInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_PULLEY_H_

//----- Include Files -------------------------------------------------------
