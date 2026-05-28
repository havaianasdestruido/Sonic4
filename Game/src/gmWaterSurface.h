// ==========================================================================
/*!
  @file gmWaterSurface.h
  @brief 

  @author hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmWaterSurface.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_WATER_SURFACE_
#define GM_WATER_SURFACE_

//----- Include Files -------------------------------------------------------


#define GMD_WATER_SURFACE_USE_RENDER	(1 & (_PC | _PS3 | _XBOX | _WII | _IPHONE))	//ƒŒƒ“ƒ_…–Ê

#if	defined(__cplusplus)
extern "C" {
#endif



#include "gmEventMgr.h"

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------

// ==========================================================================
//ƒf[ƒ^
// ==========================================================================

// ==========================================================================
// GmWaterSurfaceInitData
/*!
 * …–Êƒf[ƒ^‚ğ‰Šú‰»
 *
 * @param amb ambƒf[ƒ^ƒwƒbƒ_
 */
// ==========================================================================
extern void GmWaterSurfaceInitData( AMS_AMB_HEADER* amb );

// ==========================================================================
// GmWaterSurfaceBuildData
/*!
 * …–Êƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmWaterSurfaceBuildData( void );

// ==========================================================================
// GmWaterSurfaceCheckLoading
/*!
 * “Ç‚İ‚İ‘Ò‚¿ƒ`ƒFƒbƒN
 *
 * @return TRUEF“Ç‚İ‚İI—¹ FALSEF“Ç‚İ‚İ‘Ò‚¿
 *
 */
// ==========================================================================
extern BOOL GmWaterSurfaceCheckLoading( void );

// ==========================================================================
// GmWaterSurfaceInit
/*!
 * …–Ê‰Šú‰»
 */
// ==========================================================================
extern void GmWaterSurfaceInit( void );

// ==========================================================================
// GmWaterSurfaceExit
/*!
 * …–ÊI—¹
 */
// ==========================================================================
extern void GmWaterSurfaceExit( void );

// ==========================================================================
// GmWaterSurfaceRelease
/*!
 * …–Ê\’z‚µ‚½ƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
extern void GmWaterSurfaceRelease( void );

// ==========================================================================
// GmWaterSurfaceFlushData
/*!
 * …–Ê“Ç‚İ‚ñ‚¾ƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
extern void GmWaterSurfaceFlushData( void );


// ==========================================================================
// GmWaterSurfaceCheckFlush
/*!
 * …–Ê“Ç‚İ‚ñ‚¾ƒf[ƒ^‰ğ•ú‘Ò‚¿
 */
// ==========================================================================
extern BOOL GmWaterSurfaceCheckFlush( void );




// ==========================================================================
//…–ÊŠÇ—
// ==========================================================================

// ==========================================================================
// GmWaterSurfaceRequestChangeWaterLevel
/*!
 * …–ÊƒŒƒxƒ‹•ÏX‚ğw’è
 *
 * @param water_level …–ÊƒŒƒxƒ‹
 * @param time •ÏX‚ÉŠ|‚¯‚éƒtƒŒ[ƒ€”
 * @param flag_add_time ŠÔ‚ğ‰ÁZ‚·‚éƒtƒ‰ƒOiTRUEF‰ÁZ@FALSEFã‘‚«j
 *
 */
// ==========================================================================
extern void GmWaterSurfaceRequestChangeWaterLevel( u16 water_level, u16 time, BOOL flag_add_time );

// ==========================================================================
// GmWaterSurfaceRequestAddWatarLevel
/*!
 * …–ÊƒŒƒxƒ‹•ÏX‚ğw’è
 *
 * @param water_level ’Ç‰Á‚·‚é…–ÊƒŒƒxƒ‹
 * @param time •ÏX‚ÉŠ|‚¯‚éƒtƒŒ[ƒ€”
 * @param flag_add_time ŠÔ‚ğ‰ÁZ‚·‚éƒtƒ‰ƒOiTRUEF‰ÁZ@FALSEFã‘‚«j
 *
 */
// ==========================================================================
extern void GmWaterSurfaceRequestAddWatarLevel( float water_level, u16 time, BOOL flag_add_time );

// ==========================================================================
// GmWaterSurfaceSetFlagDraw
/*!
 * …–Ê•`‰æƒtƒ‰ƒO‚ğİ’è
 *
 * @param flag_draw …–Ê•`‰æƒtƒ‰ƒO
 *
 */
// ==========================================================================
extern void GmWaterSurfaceSetFlagDraw( BOOL flag_draw );

// ==========================================================================
// GmWaterSurfaceSetFlagEnableRef
/*!
 * ”½Ë—LŒøƒtƒ‰ƒO‚ğİ’è
 *
 * @param BOOL flag_enable_ref ”½Ë—LŒøƒtƒ‰ƒO
 *
 */
// ==========================================================================
extern void GmWaterSurfaceSetFlagEnableRef( BOOL flag_enable_ref );

// ==========================================================================
// GmWaterSurfaceDrawNoWaterField
/*!
 * …–Ê‚ğ•`‰æ‚³‚¹‚È‚¢‚½‚ß‚Ì”Â‚ğ•`‰æ
 *
 * @param left ‹éŒ`
 * @param top ‹éŒ`
 * @param right ‹éŒ`
 * @param bottom ‹éŒ`
 *
 */
// ==========================================================================
extern void GmWaterSurfaceDrawNoWaterField( 
									float left,
									float top,
									float right,
									float bottom);


// ==========================================================================
// GmWaterSurfaceGetRenderTarget
/*!
 * ƒŒƒ“ƒ_[ƒ^[ƒQƒbƒg‚ğæ“¾
 *
 * @return  ƒŒƒ“ƒ_[ƒ^[ƒQƒbƒg
 */
// ==========================================================================
extern AMS_RENDER_TARGET* GmWaterSurfaceGetRenderTarget( void );

#if	defined(__cplusplus)
} /* extern "C" */
#endif


#endif // GM_WATER_SURFACE_

//----- Include Files -------------------------------------------------------
