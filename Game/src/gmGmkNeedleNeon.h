// ==========================================================================
/*!
  @file gmGmkNeedleNeon.h
  @brief ƒMƒ~ƒbƒNjiƒlƒIƒ“j

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkNeedleNeon.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_GMK_NEEDLE_NEON_H_
#define GM_GMK_NEEDLE_NEON_H_


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
// GmGmkNeedleNeonBuild
/*!
 *	ƒMƒ~ƒbƒN jiƒlƒIƒ“j ƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmGmkNeedleNeonBuild(void);

// ==========================================================================
// GmGmkNeedleNeonFlush
/*!
 *	ƒMƒ~ƒbƒN jiƒlƒIƒ“j ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
extern void GmGmkNeedleNeonFlush(void);

// ==========================================================================
// GmGmkNeedleNeonInitStand
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒlƒIƒ“jiƒXƒ^ƒ“ƒh•”j
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkNeedleNeonInitStand( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

// ==========================================================================
// GmGmkNeedleNeonInitNeedle
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒlƒIƒ“jij•”j
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkNeedleNeonInitNeedle( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

// ==========================================================================
// GmGmkNeedleNeonInitGlaer
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒlƒIƒ“jiƒOƒŒƒA•”j
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkNeedleNeonInitGlaer( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );




// ==========================================================================
// GmGmkNeedleNeonChangeModeActive
/*!
 *	ƒAƒNƒeƒBƒu‚É•ÏX@ƒlƒIƒ“j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
extern void GmGmkNeedleNeonChangeModeActive( OBS_OBJECT_WORK* obj_work );

// ==========================================================================
// GmGmkNeedleNeonChangeModeWait
/*!
 *	‘Ò‹@‚É•ÏX@ƒlƒIƒ“j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
extern void GmGmkNeedleNeonChangeModeWait( OBS_OBJECT_WORK* obj_work );

// ==========================================================================
// GmGmkNeedleNeonChangeModeTimer
/*!
 *	ŒÀ®‚É•ÏX@ƒlƒIƒ“j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
extern void GmGmkNeedleNeonChangeModeTimer( OBS_OBJECT_WORK* obj_work );

#if	defined(__cplusplus)
} /* extern "C" */
#endif

//----- Include Files -------------------------------------------------------

#endif // GM_GMK_NEEDLE_NEON_H_
