// ================================================================
/*!
  @file gmGmkCamScrLim.h
  @brief ƒMƒ~ƒbƒN ƒJƒƒ‰ ƒXƒNƒ[ƒ‹”ÍˆÍ§ŒÀ

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkCamScrLim.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ================================================================
/*
 * $Log: $
 */

#ifndef GM_GMK_CAMSCRLIM_H_
#define GM_GMK_CAMSCRLIM_H_

#if	defined(__cplusplus)
extern "C" {
#endif


//----- Include Files -------------------------------------------------------

//----- Definitions ---------------------------------------------------------
// ƒXƒNƒ[ƒ‹§ŒÀ‰ğœ•ûŒü
#define GMD_GMK_SCR_LMT_RELEASE_LEFT		(0x0001)		//!< ¶ §ŒÀ‰ğœ
#define GMD_GMK_SCR_LMT_RELEASE_TOP			(0x0002)        //!< ã §ŒÀ‰ğœ
#define GMD_GMK_SCR_LMT_RELEASE_RIGHT		(0x0004)        //!< ‰E §ŒÀ‰ğœ
#define GMD_GMK_SCR_LMT_RELEASE_BOTTOM		(0x0008)        //!< ‰º §ŒÀ‰ğœ
#define GMD_GMK_SCR_LMT_RELEASE_ALL			(0x000f)        //!< ‘S•ûŒü

// GMS_EVE_RECORD_EVENT : flag
#define GMD_GMK_SCR_LMT_EVE_FLAG_LEFT		(0x0001)		//!< ¶ §ŒÀ’†
#define GMD_GMK_SCR_LMT_EVE_FLAG_TOP		(0x0002)        //!< ã §ŒÀ’†
#define GMD_GMK_SCR_LMT_EVE_FLAG_RIGHT		(0x0004)        //!< ‰E §ŒÀ’†
#define GMD_GMK_SCR_LMT_EVE_FLAG_BOTTOM		(0x0008)        //!< ‰º §ŒÀ’†
#define GMD_GMK_SCR_LMT_EVE_FLAG_ALL		(0x000f)        //!< ‰º §ŒÀ’†

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// GmGmkCamScrLimitRelease
/*!
 *	ƒMƒ~ƒbƒN ƒJƒƒ‰ ƒXƒNƒ[ƒ‹”ÍˆÍ§ŒÀ ‰ğœ ‰Šú‰»ŠÖ”
 *	iƒ}ƒbƒvƒCƒxƒ“ƒgˆÈŠO‚©‚ç‚ÌŒÄ‚Ño‚µ—pj
 *
 *	@param	flag	[in]	ƒXƒNƒ[ƒ‹§ŒÀ‰ğœ•ûŒü
 *
 *	@note
 *		flag	: ƒXƒNƒ[ƒ‹§ŒÀ‚ğ‰ğœ‚µ‚½‚¢•ûŒü‚Ì‚ğflag‚ÉƒZƒbƒg \n
 *			¶	:	GMD_GMK_SCR_LMT_RELEASE_LEFT	 \n
 *			ã	:	GMD_GMK_SCR_LMT_RELEASE_TOP		 \n
 *			‰E	:	GMD_GMK_SCR_LMT_RELEASE_RIGHT	 \n
 *			‰º	:	GMD_GMK_SCR_LMT_RELEASE_BOTTOM	 \n
 *			‘S‚Ä:	GMD_GMK_SCR_LMT_RELEASE_ALL
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkCamScrLimitRelease(u8 flag);
// ==========================================================================
// GmGmkCamScrLimitSet
/*!
 *	ƒMƒ~ƒbƒN ƒJƒƒ‰ ƒXƒNƒ[ƒ‹”ÍˆÍ§ŒÀ ‰Šú‰»ŠÖ”
 *	iƒ}ƒbƒvƒCƒxƒ“ƒgˆÈŠO‚©‚ç‚ÌŒÄ‚Ño‚µ—pj
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *
 *	@note
 *		eve_rec ‚É flag, left, top, width, heigh
 *		pos_x , pos_y ‚»‚ê‚¼‚ê‚É•K—p‚Èî•ñ‚ğƒZƒbƒg‚µ‚ÄƒR[ƒ‹‚µ‚Ä‚­‚¾‚³‚¢B
 *		‹éŒ`î•ñ(left, top, width, height)‚Í’l‚ª‚Q”{‚³‚êg—p‚³‚ê‚é‚±‚Æ‚É’ˆÓB
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkCamScrLimitSet(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y);
// ==========================================================================
// GmGmkCamScrLimitInit
/*!
 *	ƒMƒ~ƒbƒN ƒJƒƒ‰ ƒXƒNƒ[ƒ‹”ÍˆÍ§ŒÀİ’u ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkCamScrLimitInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);
// ==========================================================================
// GmGmkCamScrLimitReleaseInit
/*!
 *	ƒMƒ~ƒbƒN ƒJƒƒ‰ ƒXƒNƒ[ƒ‹”ÍˆÍ§ŒÀ ‰ğœ ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkCamScrLimitReleaseInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type);

// ==========================================================================
// GmCamScrLimitSetDirect
/*!
 * ƒXƒNƒ[ƒ‹§ŒÀ‘¦ƒZƒbƒg
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *
 *	@note
 *		eve_rec ‚É flag, left, top, width, heigh
 *		pos_x , pos_y ‚»‚ê‚¼‚ê‚É•K—p‚Èî•ñ‚ğƒZƒbƒg‚µ‚ÄƒR[ƒ‹‚µ‚Ä‚­‚¾‚³‚¢B
 *
 *		pos_x/y(‰æ–Ê’†S) ‚©‚ç left, top, width, height ‚Å
 *		ƒZƒbƒg‚µ‚½‹——£‚ÅƒXƒNƒ[ƒ‹‚ğ§ŒÀ‚µ‚Ü‚·B
 *
 *		¦ƒc[ƒ‹‚Ì”’l“ü—Í§–ñ“s‡‚©‚ç§ŒÀ‹——£(left, top, width, height)
 *		@‚Í’l‚ª‚Q”{‚³‚ê‚Äg—p‚³‚ê‚é‚±‚Æ‚É’ˆÓB
 *
 *		flag	: ƒXƒNƒ[ƒ‹§ŒÀ‚ğİ’u‚µ‚½‚¢•ûŒü‚ğflag‚ÉƒZƒbƒg \n
 *			¶	:	GMD_GMK_SCR_LMT_RELEASE_LEFT	 \n
 *			ã	:	GMD_GMK_SCR_LMT_RELEASE_TOP		 \n
 *			‰E	:	GMD_GMK_SCR_LMT_RELEASE_RIGHT	 \n
 *			‰º	:	GMD_GMK_SCR_LMT_RELEASE_BOTTOM	 \n
 *			‘S‚Ä:	GMD_GMK_SCR_LMT_RELEASE_ALL
 */
// ==========================================================================
extern void GmCamScrLimitSetDirect(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_CAMSCRLIM_H_

//----- Include Files -------------------------------------------------------
