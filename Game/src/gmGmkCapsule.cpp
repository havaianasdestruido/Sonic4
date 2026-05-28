// ==========================================================================
/*!
  @file gmGmkCapsule.cpp
  @brief ÉJÉvÉZÉã

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkCapsule.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmGmkCapsule.cpp,v $
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmGmkAnimal.h"
#include "gmGmkCamScrLim.h"
#include "gmSound.h"
#include "gmPlySeq.h"
#include "gmPadVib.h"

#include "gmGmkCapsule.h"

// ÉfÅ[É^ÉwÉbÉ_
#include "common/model/GMK_CAPSULE_MDL.HMB"
#include "common/model/GMK_CAPSULE_MTN.HMB"


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
// ÉJÉvÉZÉãîªíËãÈå`ÉTÉCÉY
#define GMD_GMK_CAPSULE_RECT_LF	(-8)
#define GMD_GMK_CAPSULE_RECT_UP	(-8)
#define GMD_GMK_CAPSULE_RECT_RT	( 8)
#define GMD_GMK_CAPSULE_RECT_DW	( 8)

//user_flag
#define GMD_GMK_CAPSULE_RIDE_FLAG ( 1 << 0)	// àÍìxÇ≈Ç‡èÊÇ¡ÇΩÇÁON

#define GMD_GMK_CAPSULE_END_TIME	(60*7)

// ìÆï®ä÷òA
#if _PS3 | _XBOX | _PC
#define GMD_GMK_CAPSULE_ANIMAL_MAX		20		// ï\é¶Ç∑ÇÈìÆï®ÇÃêî
#else	// _WII | _IPHONE
#define GMD_GMK_CAPSULE_ANIMAL_MAX		20		// ï\é¶Ç∑ÇÈìÆï®ÇÃêî
#endif
#define GMD_GMK_CAPSULE_ANIMAL_BASEWAIT	60		// ã§í ë“Çøéûä‘

#if (GMD_GMK_CAPSULE_ANIMAL_MAX == 6)
 #define GMD_GMK_CAPSULE_ANIMAL_WAITGAP	(120/GMD_GMK_CAPSULE_ANIMAL_MAX)		// ë“Çøéûä‘ä‘äu
#else
 #define GMD_GMK_CAPSULE_ANIMAL_WAITGAP	(240/GMD_GMK_CAPSULE_ANIMAL_MAX)		// ë“Çøéûä‘ä‘äu
#endif

// ìÆï®îzíuÉfÅ[É^ç\ê¨
typedef struct tag_GMS_GMK_CAPSULE_ANIMAL_SET_PARAM {
	s16		ofs_x;			//!< XÉIÉtÉZÉbÉg
	s16		ofs_y;			//!< YÉIÉtÉZÉbÉg
	s16		ofs_z;			//!< ZÉIÉtÉZÉbÉg
	u8		type;			//!< éÌóﬁÉ^ÉCÉv
	u8		vec;			//!< à⁄ìÆï˚å¸
	u16		time;			//!< ìÆçÏäJénwait
} GMS_GMK_CAPSULE_ANIMAL_SET_PARAM;
//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkCapsuleSwitchMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkCapsuleBodyMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkCapsuleKeyMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkCapsuleAnimalMake(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

static OBS_ACTION3D_NN_WORK *gm_gmk_capsule_obj_3d_list = NULL;

// ìÆï®îzíuÉfÅ[É^
#if (GMD_GMK_CAPSULE_ANIMAL_MAX == 30)	// ê∂ê¨êî30
static const GMS_GMK_CAPSULE_ANIMAL_SET_PARAM g_gm_gmk_capsule_animal_set[GMD_GMK_CAPSULE_ANIMAL_MAX] = {
	//	ofs_x	ofs_y	ofs_z	typ	vec	time
	{	-28,	-20,	-96,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*15,	},	//  1
	{	-25,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*23,	},	//  2
	{	-22,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 5,	},	//  3
	{	-19,	-20,	  0,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*20,	},	//  4
	{	-16,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*11,	},	//  5
	{	-13,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*14,	},	//  6
	{	-10,	-20,	-96,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*26,	},	//  7
	{	 -7,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 4,	},	//  8
	{	 -4,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*24,	},	//  9
	{	 -1,	-20,	  0,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 9,	},	// 10
	{	-13,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*19,	},	// 11
	{	-10,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 1,	},	// 12
	{	 -7,	-20,	-96,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 8,	},	// 13
	{	 -4,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*28,	},	// 14
	{	 -1,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 0,	},	// 15
	{	 +2,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*21,	},	// 16
	{	 +5,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*13,	},	// 17
	{	 +8,	-20,	  0,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 2,	},	// 18
	{	+11,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*27,	},	// 19
	{	+14,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*10,	},	// 20
	{	 +2,	-20,	-96,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 6,	},	// 21
	{	 +5,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*18,	},	// 22
	{	 +8,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 3,	},	// 23
	{	+11,	-20,	  0,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*25,	},	// 24
	{	+14,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*16,	},	// 25
	{	+17,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*22,	},	// 26
	{	+20,	-20,	-96,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 7,	},	// 27
	{	+23,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*29,	},	// 28
	{	+26,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*17,	},	// 29
	{	+29,	-20,	  0,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*12,	},	// 30
};
#elif (GMD_GMK_CAPSULE_ANIMAL_MAX == 25)	// ê∂ê¨êî25
static const GMS_GMK_CAPSULE_ANIMAL_SET_PARAM g_gm_gmk_capsule_animal_set[GMD_GMK_CAPSULE_ANIMAL_MAX] = {
	//	ofs_x	ofs_y	ofs_z	typ	vec	time
	{	-27,	-20,	-96,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*16,	},	//  1
	{	-24,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*12,	},	//  2
	{	-21,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*19,	},	//  3
	{	-18,	-20,	  0,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 3,	},	//  4
	{	-15,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*23,	},	//  5
	{	-12,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 9,	},	//  6
	{	 -9,	-20,	-96,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*21,	},	//  7
	{	 -6,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 1,	},	//  8
	{	 -6,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*17,	},	//  9
	{	 -3,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 6,	},	// 10
	{	 -3,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*15,	},	// 11
	{	  0,	-20,	  0,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*11,	},	// 12
	{	  0,	-20,	  0,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 0,	},	// 13
	{	  0,	-20,	  0,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 5,	},	// 14
	{	 +3,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*20,	},	// 15
	{	 +3,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 8,	},	// 16
	{	 +6,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*24,	},	// 17
	{	 +6,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 2,	},	// 18
	{	 +9,	-20,	-96,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*13,	},	// 19
	{	+12,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*10,	},	// 20
	{	+15,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*18,	},	// 21
	{	+18,	-20,	  0,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 4,	},	// 22
	{	+21,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*22,	},	// 23
	{	+24,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 7,	},	// 24
	{	+27,	-20,	-96,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*14,	},	// 25
};
#elif (GMD_GMK_CAPSULE_ANIMAL_MAX == 20)	// ê∂ê¨êî20
static const GMS_GMK_CAPSULE_ANIMAL_SET_PARAM g_gm_gmk_capsule_animal_set[GMD_GMK_CAPSULE_ANIMAL_MAX] = {
	//	ofs_x	ofs_y	ofs_z	typ	vec	time
	{	-28,	-20,	-96,	0,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 9,	},	//  1
	{	-25,	-20,	-64,	0,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 2,	},	//  2
	{	-22,	-20,	-32,	0,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*14,	},	//  3
	{	-19,	-20,	  0,	0,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 7,	},	//  4
	{	-16,	-20,	-32,	0,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*16,	},	//  5
	{	-13,	-20,	-64,	0,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 3,	},	//  6
	{	-10,	-20,	-96,	0,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*10,	},	//  7
	{	 -7,	-20,	-64,	0,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*18,	},	//  8
	{	 -4,	-20,	-32,	0,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 6,	},	//  9
	{	 -1,	-20,	  0,	0,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*15,	},	// 10
	{	 +2,	-20,	-32,	0,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 0,	},	// 11
	{	 +5,	-20,	-64,	0,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*19,	},	// 12
	{	 +8,	-20,	-96,	0,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 5,	},	// 13
	{	+11,	-20,	-64,	0,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*12,	},	// 14
	{	+14,	-20,	-32,	0,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 1,	},	// 15
	{	+17,	-20,	  0,	0,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*17,	},	// 16
	{	+20,	-20,	-32,	0,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*11,	},	// 17
	{	+23,	-20,	-64,	0,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 8,	},	// 18
	{	+26,	-20,	-96,	0,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*13,	},	// 19
	{	+29,	-20,	-64,	0,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 4,	},	// 20
};
#elif (GMD_GMK_CAPSULE_ANIMAL_MAX == 15)	// ê∂ê¨êî15
static const GMS_GMK_CAPSULE_ANIMAL_SET_PARAM g_gm_gmk_capsule_animal_set[GMD_GMK_CAPSULE_ANIMAL_MAX] = {
	//	ofs_x	ofs_y	ofs_z	typ	vec	time
	{	-28,	-20,	-96,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 3,	},	//  1
	{	-24,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 9,	},	//  2
	{	-20,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*11,	},	//  3
	{	-16,	-20,	  0,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 1,	},	//  4
	{	-12,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*14,	},	//  5
	{	 -8,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 6,	},	//  6
	{	 -4,	-20,	-96,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*12,	},	//  7
	{	  0,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 0,	},	//  8
	{	 +4,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 8,	},	//  9
	{	 +8,	-20,	  0,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 4,	},	// 10
	{	+12,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*13,	},	// 11
	{	+16,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 2,	},	// 12
	{	+20,	-20,	-96,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*10,	},	// 13
	{	+24,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 5,	},	// 14
	{	+28,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 7,	},	// 15
};
#elif (GMD_GMK_CAPSULE_ANIMAL_MAX == 12)	// ê∂ê¨êî6
static const GMS_GMK_CAPSULE_ANIMAL_SET_PARAM g_gm_gmk_capsule_animal_set[GMD_GMK_CAPSULE_ANIMAL_MAX] = {
	//	ofs_x	ofs_y	ofs_z	typ	vec	time
	{	-28,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 6,	},	//  1
	{	-23,	-20,	-96,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 2,	},	//  2
	{	-18,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 9,	},	//  3
	{	-13,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 5,	},	//  4
	{	 -8,	-20,	  0,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*10,	},	//  5
	{	  3,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 0,	},	//  6
	{	 +2,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 7,	},	//  7
	{	 +7,	-20,	-96,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 4,	},	//  8
	{	+12,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP*11,	},	//  9
	{	+17,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 1,	},	// 10
	{	+22,	-20,	  0,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 8,	},	// 11
	{	+27,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 3,	},	// 12

};
#elif (GMD_GMK_CAPSULE_ANIMAL_MAX == 6)	// ê∂ê¨êî6
static const GMS_GMK_CAPSULE_ANIMAL_SET_PARAM g_gm_gmk_capsule_animal_set[GMD_GMK_CAPSULE_ANIMAL_MAX] = {
	{	-23,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 2,	},	//  4
	{	-14,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 4,	},	//  5
	{	 -5,	-20,	  0,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 0,	},	//  7
	{	 +5,	-20,	-32,	2,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 5,	},	//  9
	{	+14,	-20,	-64,	2,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 1,	},	// 10
	{	+23,	-20,	-96,	2,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 3,	},	// 12
};
#else	// ê∂ê¨êî10
static const GMS_GMK_CAPSULE_ANIMAL_SET_PARAM g_gm_gmk_capsule_animal_set[GMD_GMK_CAPSULE_ANIMAL_MAX] = {
	//	ofs_x	ofs_y	ofs_z	typ	vec	time
	{	-23,	-20,	-96,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 6,	},	//  1
	{	-18,	-20,	-64,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 2,	},	//  2
	{	-13,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 8,	},	//  3
	{	 -8,	-20,	  0,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 4,	},	//  4
	{	  3,	-20,	-32,	1,	1,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 0,	},	//  5
	{	 +2,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 7,	},	//  6
	{	 +7,	-20,	-96,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 5,	},	//  7
	{	+12,	-20,	-64,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 1,	},	//  8
	{	+17,	-20,	-32,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 9,	},	//  9
	{	+22,	-20,	  0,	1,	0,	GMD_GMK_CAPSULE_ANIMAL_BASEWAIT+GMD_GMK_CAPSULE_ANIMAL_WAITGAP* 3,	},	// 10

};
#endif	// ê∂ê¨êîí≤êÆ
//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkCapsuleBuild
/*!
 *	ÉMÉ~ÉbÉN ÉJÉvÉZÉã ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkCapsuleBuild(void)
{
	gm_gmk_capsule_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_CAPSULE_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_CAPSULE_TEX),
								0/*draw_flag*/);
}

// ==========================================================================
// GmGmkCapsuleFlush
/*!
 *	ÉMÉ~ÉbÉN ÉJÉvÉZÉã ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkCapsuleFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_CAPSULE_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_capsule_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmGmkCapsuleInit
/*!
 *	ÉMÉ~ÉbÉN ÉJÉvÉZÉã èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkCapsuleInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_CAPSULE");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_capsule_obj_3d_list[IDB_GMK_CAPSULE_MDL_GMK_CAPSULE_SWITCH_ZNO],
					&gmk_work->obj_3d);

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK - 64*FX32_ONE;	// ÉJÉvÉZÉãÉ{ÉfÉBÇÊÇËóDêÊÇå„ÇÎÇ…

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ínå`Ç†ÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// ínå`ê›íË
	{
		OBS_COLLISION_WORK *col_work = &gmk_work->ene_com.col_work;
	
		col_work->obj_col.obj		= obj_work;
		col_work->obj_col.width		= 4*8;								// ínå`ÉTÉCÉYê›íË(ÉhÉbÉg)
		col_work->obj_col.height	= 5*8;
		col_work->obj_col.ofst_x	= (s16)(-col_work->obj_col.width  /2);
		col_work->obj_col.ofst_y	= (s16)(-76);	// âüÇ≥ÇÍÇΩå„ÇÃçÇÇ≥
	}

	// É{ÉfÉBãÈå`ÉZÉbÉg for É\ÉjÉbÉNÉzÅ[É~ÉìÉO
	{
		OBS_RECT_WORK		*rect_work;
		gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
		gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
		rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
		rect_work->ppHit = NULL;
		rect_work->ppDef = NULL;
		ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
		ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
		ObjRectWorkSet(rect_work, -4, -80, 4, -72);
	}

	// ÉMÉ~ÉbÉNêßå‰ä÷òA
	obj_work->user_flag &= ~GMD_GMK_CAPSULE_RIDE_FLAG;			// RideÉtÉâÉOèâä˙âª

	// ÉÅÉCÉìèàóù
	obj_work->ppFunc = gmGmkCapsuleSwitchMain;

	// éqÉ^ÉXÉN)ê∂ê¨
	{
		OBS_OBJECT_WORK			*sub_work;
		GMS_EFFECT_3DNN_WORK	*efct_work;
		// ÉJÉvÉZÉãñ{ëÃ
		sub_work = GmEventMgrLocalEventBirth(
					GMD_EVENT_ID_NOSET_CAPSULE_BODY,	// u16 id, 
					pos_x,								// fx32 pos_x, 
					pos_y,								// fx32 pos_y, 
					gmk_work->ene_com.eve_rec->flag,	// u16 flag, 
					gmk_work->ene_com.eve_rec->left,	// s8 left, 
					gmk_work->ene_com.eve_rec->top,		// s8 top, 
					gmk_work->ene_com.eve_rec->width,	// u8 width, 
					gmk_work->ene_com.eve_rec->height,	// u8 height, 
					0);									// u8 type);
		sub_work->parent_obj = obj_work;
		sub_work->view_out_ofst = obj_work->view_out_ofst;
		gmk_work = (GMS_ENEMY_3D_WORK*)sub_work;

		// ÉJÉvÉZÉãåÆ
		sub_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_3DNN_WORK), obj_work/*parent_obj*/, 0/*sort_prio*/, "GMK_CAPSULE_BODY");
		efct_work = (GMS_EFFECT_3DNN_WORK*)sub_work;
		// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
		ObjObjectCopyAction3dNNModel(sub_work,
						&gm_gmk_capsule_obj_3d_list[IDB_GMK_CAPSULE_MDL_GMK_CAPSULE_KEY_ZNO],
						&efct_work->obj_3d);
		// óDêÊê›íË
		sub_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;
		// å¬ï ê›íË
		sub_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ìñÇΩÇËñ≥Çµ
		sub_work->disp_flag |= OBD_DISP_NODIRFLIP;
		sub_work->ppFunc = gmGmkCapsuleKeyMain;
	}

	return (obj_work);
}


// ==========================================================================
// GmGmkCapsuleBodyInit
/*!
 *	ÉMÉ~ÉbÉN ÉJÉvÉZÉãñ{ëÃ èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkCapsuleBodyInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_CAPSULE_BODY");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_capsule_obj_3d_list[IDB_GMK_CAPSULE_MDL_GMK_CAPSULE_ZNO],
					&gmk_work->obj_3d);

	// ÉÇÅ[ÉVÉáÉìèâä˙âª
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, FALSE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_GMK_CAPSULE_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);
	// ÉAÉNÉVÉáÉìê›íË
#if	_WII | _IPHONE
	ObjDrawObjectActionSet(obj_work, IDB_GMK_CAPSULE_MTN_GMK_CAPSULE_ZNM);
#endif
	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ínå`Ç†ÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// ínå`ê›íË
	{
		OBS_COLLISION_WORK *col_work = &gmk_work->ene_com.col_work;
	
		col_work->obj_col.obj		= obj_work;
		col_work->obj_col.width		= 8*8;								// ínå`ÉTÉCÉYê›íË(ÉhÉbÉg)
		col_work->obj_col.height	= 60;//8*8;
		col_work->obj_col.ofst_x	= (s16)(-col_work->obj_col.width /2);
		col_work->obj_col.ofst_y	= (s16)(-col_work->obj_col.height);
	}

	// ÉÅÉCÉìèàóù
	obj_work->ppFunc = gmGmkCapsuleBodyMain;

	return (obj_work);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkCapsuleSwitchMain
/*!
 *	ÉMÉ~ÉbÉN ÉJÉvÉZÉãÉXÉCÉbÉ`ïî ÉÅÉCÉìä÷êî
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *
 */
// ==========================================================================
void gmGmkCapsuleSwitchMain(OBS_OBJECT_WORK *obj_work)
{
	OBS_COLLISION_OBJ	*obj_col = &obj_work->col_work->obj_col;

	if (obj_col->rider_obj) {
		obj_work->ofst.y = 6 << FX32_SHIFT;
		if (!(obj_work->user_flag & GMD_GMK_CAPSULE_RIDE_FLAG)) {
			// èââÒÉXÉCÉbÉ`ãNìÆ
			g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_COUNT_GAME_TIME;	// ÉQÅ[ÉÄÉ^ÉCÉ}í‚é~ 
			g_gm_main_system.game_flag |= GMD_GAME_FLAG_GOAL_IN;			// ÉSÅ[Éãí ím

			GMS_EFFECT_3DES_WORK *gm_gmk_capsule_effct;

			gm_gmk_capsule_effct = GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_CAP_OPEN);
			GmEffect3DESSetDispOffset(gm_gmk_capsule_effct, 0, 24.0f, 40.0f);	// ç¿ïWÉIÉtÉZÉbÉg

			gmGmkCapsuleAnimalMake(obj_work);						// ìÆï®ê∂ê¨
			obj_work->user_timer = 1;								// ÉJÉEÉìÉgÉAÉbÉväJén

			{	// ÉXÉNÉçÅ[Éãêßå¿ÉZÉbÉg
				GMS_EVE_RECORD_EVENT eve_rec;
				eve_rec.flag	= GMD_GMK_SCR_LMT_EVE_FLAG_LEFT | GMD_GMK_SCR_LMT_EVE_FLAG_RIGHT | GMD_GMK_SCR_LMT_EVE_FLAG_TOP;
//				eve_rec.flag	= GMD_GMK_SCR_LMT_EVE_FLAG_ALL;
				eve_rec.left	= -192/2;
				eve_rec.top		= -208/2;
				eve_rec.width	= 384/2;
				eve_rec.height	= 224/2;
				GmGmkCamScrLimitSet(&eve_rec, obj_work->pos.x, obj_work->pos.y);
			}

			// ÉRÉìÉgÉçÅ[ÉâÅ[êUìÆ
			GMM_PAD_VIB_SMALL();

			// SE
			GmSoundPlaySE("Capsule");

			// ÉvÉåÉCÉÑÅ[ÇÉ{ÉXÉXÉeÅ[ÉWÉSÅ[ÉãÉVÅ[ÉPÉìÉXÇ÷
			GmPlySeqChangeBossGoal(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P],
					obj_work->pos.x, obj_work->pos.y);
		}
		obj_work->user_flag |= GMD_GMK_CAPSULE_RIDE_FLAG;
	} else {
		obj_work->ofst.y = 0;
	}

	if (obj_work->user_timer) {
		obj_work->user_timer++;
		if (obj_work->user_timer == GMD_GMK_CAPSULE_END_TIME) {
			g_gm_main_system.game_flag |= GMD_GAME_FLAG_CLEAR;		// ÉXÉeÅ[ÉWÉNÉäÉA
		}
	}
}

// ==========================================================================
// gmGmkCapsuleSwitchMain
/*!
 *	ÉMÉ~ÉbÉN ÉJÉvÉZÉãñ{ëÃïî ÉÅÉCÉìä÷êî
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *
 */
// ==========================================================================
void gmGmkCapsuleBodyMain(OBS_OBJECT_WORK *obj_work)
{
	OBS_OBJECT_WORK 	*parent_obj	= obj_work->parent_obj;

	if (parent_obj->user_flag & GMD_GMK_CAPSULE_RIDE_FLAG) {
		ObjDrawObjectActionSet3DNN(obj_work, IDB_GMK_CAPSULE_MTN_GMK_CAPSULE_RUN_ZNM, 0);
		obj_work->ppFunc = NULL;
	}
}

// ==========================================================================
// gmGmkCapsuleSwitchMain
/*!
 *	ÉMÉ~ÉbÉN ÉJÉvÉZÉãåÆïî ÉÅÉCÉìä÷êî
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *
 */
// ==========================================================================
void gmGmkCapsuleKeyMain(OBS_OBJECT_WORK *obj_work)
{
	OBS_OBJECT_WORK 	*parent_obj	= obj_work->parent_obj;

	if (parent_obj->user_flag & GMD_GMK_CAPSULE_RIDE_FLAG) {
		GMS_EFFECT_3DES_WORK *gm_gmk_capsule_effct;

		obj_work->spd.x = 6 << FX32_SHIFT;
		obj_work->spd.y = -4 << FX32_SHIFT;
		obj_work->move_flag &= ~OBD_MOVE_NOMOVE;
		obj_work->move_flag |= OBD_MOVE_FALL;
		obj_work->ppFunc = NULL;

		gm_gmk_capsule_effct = GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_CAP_KEY1);
		GmEffect3DESSetDispOffset(gm_gmk_capsule_effct, 0, 46.0f, 40.0f);	// ç¿ïWÉIÉtÉZÉbÉg

		gm_gmk_capsule_effct = GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_CAP_KEY2);
		GmEffect3DESSetDispOffset(gm_gmk_capsule_effct, 0, 46.0f, 40.0f);	// ç¿ïWÉIÉtÉZÉbÉg

	}

}

// ==========================================================================
// gmGmkCapsuleSwitchMain
/*!
 *	ÉMÉ~ÉbÉN ìÆï®ê∂ê¨
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *
 */
// ==========================================================================
void gmGmkCapsuleAnimalMake(OBS_OBJECT_WORK *obj_work)
{
	u16	i;
	for (i = 0; i < GMD_GMK_CAPSULE_ANIMAL_MAX; i++) {
		GmGmkAnimalInit(obj_work,
			g_gm_gmk_capsule_animal_set[i].ofs_x << FX32_SHIFT,
			g_gm_gmk_capsule_animal_set[i].ofs_y << FX32_SHIFT,
			g_gm_gmk_capsule_animal_set[i].ofs_z << FX32_SHIFT,
			g_gm_gmk_capsule_animal_set[i].type,
			g_gm_gmk_capsule_animal_set[i].vec,
			g_gm_gmk_capsule_animal_set[i].time);
	}
}

//	#if 0
//	
//	// ==========================================================================
//	// gmGmkMain
//	/*!
//	 *	ÉMÉ~ÉbÉN  ÉÅÉCÉìèàóù
//	 *
//	 *	@param	gmk_work	[in]	ÉGÉlÉ~Å[ÉèÅ[ÉN
//	 */
//	// ==========================================================================
//	void gmGmkMain(GMS_ENEMY_WORK *gmk_work)
//	{
//	}
//	
//	
//	// ==========================================================================
//	// gmGmkDefFunc
//	/*!
//	 *	ÉMÉ~ÉbÉN  HITèàóù
//	 *
//	 *	@param	match_rect	[in]	ëäéËãÈå`ÉèÅ[ÉN
//	 *	@param	mine_rect	[in]	é©ï™ãÈå`ÉèÅ[ÉN
//	 */
//	// ==========================================================================
//	void gmGmkDefFunc(OBS_RECT_WORK *match_rect, OBS_RECT_WORK *mine_rect)
//	{
//		GMS_ENEMY_WORK	*gmk_work = (GMS_ENEMY_WORK*)mine_rect->parent_obj;
//		GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;
//	
//		if (gmk_work == NULL || ply_work == NULL) {
//			return;
//		}
//		if (ply_work->obj.obj_type != GMD_OBJTYPE_PLAYER) {
//			return;
//		}
//	}
//	#endif
//	// ==========================================================================
//	// _pt
//	/*!
//	 *	@param	tcb	[in]	TCB
//	 */
//	// ==========================================================================
