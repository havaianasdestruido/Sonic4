// =======================================================================
/*!
	@file	gmSplStage.h
	@brief	ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒWŠÇ—

	@author K.Kuramoto
				Copyright(c) 2009 Dimps
	$Id: gmSplStage.h 2 2011-04-11 05:21:26Z thamada $
	$Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef GM_SPL_STAGE_H_
#define GM_SPL_STAGE_H_

//----- Include Files -------------------------------------------------------

//----- Definitions ---------------------------------------------------------
//ƒXƒyƒXƒeƒƒCƒ“ƒ[ƒN
typedef struct tag_GMS_SPL_STG_WORK {	
	u32 	counter;
	u32 	flag;
	Angle32	roll;				///< ƒJƒƒ‰‰ñ“]—Ê
	Angle32	roll_spd;			///< ƒJƒƒ‰‰ñ“]‘¬“x
	NNS_VECTOR	light_vec;		///< ƒ‰ƒCƒg
	u16		get_ring;			// æ“¾ƒŠƒ“ƒOƒtƒ‰ƒO
#if defined(MTD_DEBUG)
	s16		dbg_cursor;			// ƒfƒoƒbƒO—pƒJ[ƒ\ƒ‹ˆÊ’u
#endif//defined(MTD_DEBUG)
} GMS_SPL_STG_WORK;

//GMS_SPL_STG_WORK::flag
#define GMD_SPL_STAGE_BOUNCE_HIT	(0x00000001)	// ƒoƒEƒ“ƒhƒqƒbƒg(1ƒtƒŒ[ƒ€–ˆ‰Šú‰»)
#define GMD_SPL_STAGE_NUDGE_HIT		(0x00000002)	// —h‚ç‚µƒqƒbƒg(—h‚ê‚P‰ñ–ˆ‰Šú‰»)
#define GMD_SPL_STAGE_RESULT		(0x00000004)	// ƒŠƒUƒ‹ƒgˆÚs’Ê’m


// ƒŠƒ“ƒOƒQ[ƒgŠÇ—
#define GMD_SS_RINGGATE_MAX			(9)			// ‚PƒXƒe[ƒW‚ ‚½‚è‚ÌƒŠƒ“ƒOƒQ[ƒgÅ‘åİ’u”

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------

// ==========================================================================
// GmSplStageStart
/*!
	ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒWŠJn
 */
// ==========================================================================
extern void GmSplStageStart(void);

// ==========================================================================
// GmSplStageExit
/*!
	ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒW‹­§I—¹ˆ—
 */
// ==========================================================================
extern void GmSplStageExit(void);

// ==========================================================================
// GmSplStageSetLight
/*!
 * ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒW—pƒ‰ƒCƒgƒZƒbƒg
 *
 */
// ==========================================================================
extern void GmSplStageSetLight(void);

// ==========================================================================
// GmSplStageGetWork
/*!
 * ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒW—pƒ[ƒNæ“¾
 *
 *	@return	ƒXƒyƒXƒe—p(GMS_SPL_STG_WORK)ƒ[ƒNƒAƒhƒŒƒX
 */
// ==========================================================================
extern GMS_SPL_STG_WORK* GmSplStageGetWork(void);

// ==========================================================================
// GmSplStageSwSet
/*!
 * ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒW—pƒXƒCƒbƒ`ƒtƒ‰ƒOƒZƒbƒg
 *
 *	@param sw_no	[in] ƒXƒCƒbƒ`No.
 */
// ==========================================================================
extern void GmSplStageSwSet(u32 sw_no);

// ==========================================================================
// GmSplStageSwCheck
/*!
 * ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒW—pƒXƒCƒbƒ`ƒtƒ‰ƒOƒ`ƒFƒbƒN
 *
 *	@param sw_no	[in] ƒXƒCƒbƒ`No.
 */
// ==========================================================================
extern BOOL GmSplStageSwCheck(u32 sw_no);

// ==========================================================================
// GmSplStageRingGateNumGet
/*!
 *	ƒŠƒ“ƒOƒQ[ƒgæ“¾–‡”æ“¾
 *
 *	@param gate_id	[in] ƒQ[ƒgID”Ô†( 0`GMD_SS_RINGGATE_MAX )
 *
 *	@return ƒŠƒ“ƒO•K—v–‡”
 */
// ==========================================================================
extern u16 GmSplStageRingGateNumGet(u16 gate_id);

//----- Include Files -------------------------------------------------------

#endif // GM_SPL_STAGE_H_