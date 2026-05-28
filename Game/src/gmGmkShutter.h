// ==========================================================================
/*!
  @file gmGmkShutter.h
  @brief ƒMƒ~ƒbƒNƒVƒƒƒbƒ^[

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkShutter.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_GMK_SHUTTER_H_
#define GM_GMK_SHUTTER_H_


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
// GmGmkShutterBuild
/*!
 *	ƒMƒ~ƒbƒN ƒVƒƒƒbƒ^[ ƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmGmkShutterBuild(void);

// ==========================================================================
// GmGmkShutterFlush
/*!
 *	ƒMƒ~ƒbƒN ƒVƒƒƒbƒ^[ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
extern void GmGmkShutterFlush(void);

// ==========================================================================
// GmGmkShutterInInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒVƒƒƒbƒ^[“üŒû
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkShutterInInit( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

// ==========================================================================
// GmGmkShutterOutInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒVƒƒƒbƒ^[oŒû
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkShutterOutInit( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );


// ==========================================================================
// GmGmkShutterOutChangeModeClose
/*!
 *	ƒVƒƒƒbƒ^[“üŒû•Â‚ß‚é
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
extern void GmGmkShutterInChangeModeClose( OBS_OBJECT_WORK* obj_work );

// ==========================================================================
// GmGmkShutterOutChangeModeOpen
/*!
 *	ƒVƒƒƒbƒ^[oŒûŠJ‚¯‚é
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
extern void GmGmkShutterOutChangeModeOpen( OBS_OBJECT_WORK* obj_work );


#if	defined(__cplusplus)
} /* extern "C" */
#endif

//----- Include Files -------------------------------------------------------

#endif // GM_GMK_SHUTTER_H_
