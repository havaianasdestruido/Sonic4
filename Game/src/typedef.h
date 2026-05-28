// ==========================================================================
/*!
  @file typedef.h
  @brief Œ^’è‹`

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: typedef.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef TYPEDEF_H_
#define TYPEDEF_H_


//----- Include Files -------------------------------------------------------
#include <alice.h>

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------

// ==========================================================================
//	Šî–{Œ^
// ==========================================================================
typedef Uint8	u8;
typedef Uint16	u16;
typedef Uint32	u32;
typedef Uint64	u64;

typedef Sint8	s8;
typedef Sint16	s16;
typedef Sint32	s32;
typedef Sint64	s64;

typedef Float	f32;


// ==========================================================================
//	‰ž—pŒ^
// ==========================================================================
typedef volatile u8		vu8;
typedef volatile u16	vu16;
typedef volatile u32	vu32;
typedef volatile u64	vu64;

typedef volatile s8		vs8;
typedef volatile s16	vs16;
typedef volatile s32	vs32;
typedef volatile s64	vs64;

typedef volatile f32	vf32;



//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------


#if	defined(__cplusplus)
} /* extern "C" */
#endif


#endif // GX_H_

//----- Include Files -------------------------------------------------------
