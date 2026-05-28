// ==========================================================================
/*!
  @file gmGmkDecoFrameMgr.h
  @brief ƒMƒ~ƒbƒN‘•ü‹¤’ÊƒtƒŒ[ƒ€ŠÇ—

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkDecoFrameMgr.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_GMK_DECO_FRAME_MGR_H_
#define GM_GMK_DECO_FRAME_MGR_H_


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
// GmGmkDecoFrameMgrInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@‘•ü‹¤’ÊƒtƒŒ[ƒ€ŠÇ—
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkDecoFrameMgrInit( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

#if	defined(__cplusplus)
} /* extern "C" */
#endif

//----- Include Files -------------------------------------------------------

#endif // GM_GMK_DECO_FRAME_MGR_H_
