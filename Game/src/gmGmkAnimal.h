// ================================================================
/*!
  @file gmGmkAnimal.h
  @brief “®•¨

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkAnimal.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ================================================================
/*
 * $Log: $
 */

#ifndef GM_GMK_ANIMAL_H_
#define GM_GMK_ANIMAL_H_

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
// GmGmkAnimalBuild
/*!
 *	ƒMƒ~ƒbƒN “®•¨ ƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmGmkAnimalBuild(void);

// ==========================================================================
// GmGmkAnimalFlush
/*!
 *	ƒMƒ~ƒbƒN “®•¨ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
extern void GmGmkAnimalFlush(void);

// ==========================================================================
// GmGmkAnimalInit
/*!
 *	ƒMƒ~ƒbƒN “®•¨ ‰Šú‰»ŠÖ”
 *
 *	@param	obj_work[in]	eƒ[ƒN
 *	@param	ofs_x	[in]	À•WƒIƒtƒZƒbƒg‚wieÀ•W‚©‚ç‚ÌƒIƒtƒZƒbƒgj
 *	@param	ofs_y	[in]	À•WƒIƒtƒZƒbƒg‚xieÀ•W‚©‚ç‚ÌƒIƒtƒZƒbƒgj
 *	@param	ofs_z	[in]	À•WƒIƒtƒZƒbƒg‚yieÀ•W‚©‚ç‚ÌƒIƒtƒZƒbƒgj
 *	@param	type	[in]	“®•¨í—Ş
 *	@param	vec		[in]	ˆÚ“®•ûŒü
 *	@param	timer	[in]	ˆÚ“®ŠJn‚Ü‚Å‚ÌŠÔ
 *
 *	@note
 *		type ‚Í“®•¨í—Ş
 *			0		: ƒ‰ƒ“ƒ_ƒ€
 *			1`2	: ƒXƒe[ƒWŒÅ—L‚Ì‚Qí—Ş‚Ç‚¿‚ç‚ğ•\¦‚·‚é‚©w’èiƒJƒvƒZƒ‹—pj
 *		vec ‚Í“®•¨‚ÌˆÚ“®•ûŒü
 *			0		: ¶
 *			1		: ‰EiƒJƒvƒZƒ‹—pj
 *		timer ‚Í“®•¨‚ÌˆÚ“®ŠJn‚Ü‚Å‚ÌŠÔ
 *			0		: ‘¦s“®
 *			1`		: ƒEƒFƒCƒgŠÔiƒJƒvƒZƒ‹—pj
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkAnimalInit(OBS_OBJECT_WORK *parent_work, fx32 ofs_x, fx32 ofs_y, fx32 ofs_z, u8 type, u8 vec, u16 timer);

// ==========================================================================
// GmGmkEndingAnimalInit
/*!
 *	ƒMƒ~ƒbƒN ƒGƒ“ƒfƒBƒ“ƒO—p“®•¨ ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkEndingAnimalInit(GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_ANIMAL_H_

//----- Include Files -------------------------------------------------------
