// ==========================================================================
/*!
  @file gmBoss2.h
  @brief ƒ{ƒX2

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmBoss2.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_BOSS2_H_
#define GM_BOSS2_H_


#if	defined(__cplusplus)
extern "C" {
#endif

//----- Include Files -------------------------------------------------------

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------

// =======================================================================
// GmBoss2Build
/*!
 *	ƒ{ƒX2\’z
 */
// =======================================================================
extern void GmBoss2Build( void );

// =======================================================================
// GmBoss2Flush
/*!
 *	ƒ{ƒX2‰ğ•ú
 */
// =======================================================================
extern void GmBoss2Flush( void );

// ==========================================================================
// GmBoss2Init
/*!
 *	ƒ{ƒX2ŠÇ—‰Šú‰»ŠÖ”
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmBoss2Init( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

// ==========================================================================
// GmBoss2BodyInit
/*!
 *	ƒ{ƒX2‘Ì‰Šú‰»ŠÖ”
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmBoss2BodyInit( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

// ==========================================================================
// GmBoss2EggInit
/*!
 *	ƒ{ƒX2ƒGƒbƒOƒ}ƒ“‰Šú‰»ŠÖ”
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmBoss2EggInit( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

// ==========================================================================
// GmBoss2BallInit
/*!
 *	ƒ{ƒX2ƒgƒQƒ{[ƒ‹‰Šú‰»ŠÖ”
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmBoss2BallInit( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );


#if	defined(__cplusplus)
} /* extern "C" */
#endif

//----- Include Files -------------------------------------------------------

#endif // GM_BOSS2_H_






