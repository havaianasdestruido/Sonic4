// ==========================================================================
/*!
  @file gmGmkWaterSlider.h
  @brief ƒMƒ~ƒbƒNƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[j

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkWaterSlider.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_GMK_WATER_SLIDER_H_
#define GM_GMK_WATER_SLIDER_H_


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
// GmGmkWaterSliderBuild
/*!
 *	ƒMƒ~ƒbƒN ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[ ƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmGmkWaterSliderBuild(void);

// ==========================================================================
// GmGmkWaterSliderFlush
/*!
 *	ƒMƒ~ƒbƒN ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
extern void GmGmkWaterSliderFlush(void);

// ==========================================================================
// GmGmkWaterSliderInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkWaterSliderInit( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );



// ==========================================================================
// GmGmkWaterSliderGetObj3DList
/*!
 *	OBJ3DƒŠƒXƒg‚ğæ“¾i‘•ü‚Å“¯‚¶ƒ‚ƒfƒ‹‚ğg‚¢‚Ü‚í‚µ‚Ä‚¢‚é‚½‚ßj
 *
 *	@return OBJ3DƒŠƒXƒg
 */
// ==========================================================================
extern OBS_ACTION3D_NN_WORK* GmGmkWaterSliderGetObj3DList( void );

// ==========================================================================
// GmGmkWaterSliderCheckActive
/*!
 * ƒAƒNƒeƒBƒu‚©‚Ç‚¤‚©”»’è
 *
 * @return TRUEFƒAƒNƒeƒBƒu@FALSEFƒAƒNƒeƒBƒu‚¶‚á‚È‚¢
 *
 */
// ==========================================================================
extern void GmGmkWaterSliderCheckActive( BOOL flag_draw );


// ==========================================================================
// GmGmkWaterSliderCreateEffect
/*!
 *	ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒgì¬
 *
 *	@return ƒGƒtƒFƒNƒg—pƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkWaterSliderCreateEffect( void );

// ==========================================================================
// GmGmkWaterSliderDeleteEffect
/*!
 *	ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒgíœ
 *
 */
// ==========================================================================
extern void GmGmkWaterSliderDeleteEffect( void );

#if	defined(__cplusplus)
} /* extern "C" */
#endif

//----- Include Files -------------------------------------------------------

#endif // GM_GMK_WATER_SLIDER_H_
