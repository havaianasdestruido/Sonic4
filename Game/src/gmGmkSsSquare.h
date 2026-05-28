// ================================================================
/*!
  @file gmGmkSsSquare.h
  @brief SpecialStage ŽlŠp’Œ

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSsSquare.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ================================================================
/*
 * $Log: $
 */

#ifndef GM_GMK_SSSQUARE_H_
#define GM_GMK_SSSQUARE_H_

#if	defined(__cplusplus)
extern "C" {
#endif


//----- Include Files -------------------------------------------------------

//----- Definitions ---------------------------------------------------------
// user_flag
#define GMD_GMK_SS_SQR_FLAG_HIT				(0x80000000)	//!< ˜A‘±HITƒ`ƒFƒbƒN

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------
//extern u8 NNM_ALIGN_VC(4) NNM_ALIGN_CW(4) g_gm_ss_parts_dir[];

//----- External Declarations -----------------------------------------------

// ==========================================================================
// GmGmkSsSquareBuild
/*!
 *	ƒMƒ~ƒbƒN SpecialStage ŽlŠp’Œ ƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmGmkSsSquareBuild(void);

// ==========================================================================
// GmGmkSsSquareFlush
/*!
 *	ƒMƒ~ƒbƒN SpecialStage ŽlŠp’Œ ƒf[ƒ^•Ð•t‚¯
 */
// ==========================================================================
extern void GmGmkSsSquareFlush(void);

// ==========================================================================
// GmGmkSsSquareInit
/*!
 *	ƒMƒ~ƒbƒN SpecialStage ŽlŠp’Œ ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkSsSquareInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);

// ==========================================================================
// GmGmkSsSquareBounce
/*!
 *	ƒMƒ~ƒbƒN SpecialStage ŽlŠp’Œ ƒ\ƒjƒbƒN‚ð’µ‚Ë•Ô‚·
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 */
// ==========================================================================
extern void GmGmkSsSquareBounce(OBS_OBJECT_WORK *obj_work);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_SSSQUARE_H_

