// ==========================================================================
/*!
  @file gmGmkDrainTank.h
  @brief ƒMƒ~ƒbƒN”r‰t‘•’u

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkDrainTank.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_GMK_DRAIN_TANK_H_
#define GM_GMK_DRAIN_TANK_H_


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
// GmGmkDrainTankBuild
/*!
 *	ƒMƒ~ƒbƒN ”r‰t‘•’u ƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmGmkDrainTankBuild(void);

// ==========================================================================
// GmGmkDrainTankFlush
/*!
 *	ƒMƒ~ƒbƒN ”r‰t‘•’u ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
extern void GmGmkDrainTankFlush(void);

// ==========================================================================
// GmGmkDrainTankInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@”r‰t‘•’u“üŒû
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkDrainTankInitIn( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );


// ==========================================================================
// GmGmkDrainTankInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@”r‰t‘•’uoŒû
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkDrainTankInitOut( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

// ==========================================================================
// GmGmkDrainTankSplashInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@”r‰t‘•’u•¬o‚·…
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkDrainTankSplashInit( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

#if	defined(__cplusplus)
} /* extern "C" */
#endif

//----- Include Files -------------------------------------------------------

#endif // GM_GMK_DRAIN_TANK_H_
