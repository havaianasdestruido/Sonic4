// ==========================================================================
/*!
  @file fx.h
  @brief fxŒ^‘€ìŠÖ˜A

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: fx.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef FX_H_
#define FX_H_


//----- Include Files -------------------------------------------------------
#include <alice.h>
#include "typedef.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
#define FX_INLINE	(0)

// ==========================================================================
//	fx32: 1:19:12
// ==========================================================================
typedef s32 fx32;
#define FX32_SHIFT          12
#define FX32_INT_SIZE       19
#define FX32_DEC_SIZE       12

#define FX32_INT_MASK       0x7ffff000
#define FX32_DEC_MASK       0x00000fff
#define FX32_SIGN_MASK      0x80000000

#define FX32_MAX            ((fx32)0x7fffffff)
#define FX32_MIN            ((fx32)0x80000000)

#define FX_MUL(v1, v2)       FX32_CAST(((fx64)(v1) * (v2) + 0x800LL) >> FX32_SHIFT)
#define FX_MUL32x64C(v1, v2) FX32_CAST(((v2) * (v1) + 0x80000000LL) >> 32)

#define FX_FX32_TO_F32(x)    ((f32)((x) / (f32)(1 << FX32_SHIFT)))
#define FX_F32_TO_FX32(x)    ((fx32)(((x) > 0) ? \
                                     ((x) * (1 << FX32_SHIFT) + 0.5f ) : \
                                     ((x) * (1 << FX32_SHIFT) - 0.5f )))

#define FX32_CONST(x)        FX_F32_TO_FX32(x)


// ==========================================================================
//	fx64: 1:51:12
// ==========================================================================
typedef s64 fx64;
#define FX64_SHIFT          12
#define FX64_INT_SIZE       51
#define FX64_DEC_SIZE       12

#define FX64_INT_MASK       ((fx64)0x7ffffffffffff000)
#define FX64_DEC_MASK       ((fx64)0x0000000000000fff)
#define FX64_SIGN_MASK      ((fx64)0x8000000000000000)

#define FX64_MAX            ((fx64)0x7fffffffffffffff)
#define FX64_MIN            ((fx64)0x8000000000000000)

#define FX_FX64_TO_F32(x)   ((f32)((x) / (f32)(1 << FX64_SHIFT)))
#define FX_F32_TO_FX64(x)   ((fx64)(((x) > 0) ? \
                                    ((x) * (1 << FX32_SHIFT) + 0.5f ) : \
                                    ((x) * (1 << FX32_SHIFT) - 0.5f )))

#define FX64_CONST(x)       FX_F32_TO_FX64(x)


// ==========================================================================
//	fx64c: 1:31:32
// ==========================================================================
typedef s64 fx64c;
#define FX64C_SHIFT          32
#define FX64C_INT_SIZE       31
#define FX64C_DEC_SIZE       32

#define FX64C_INT_MASK       ((fx64c)0x7fffffff00000000)
#define FX64C_DEC_MASK       ((fx64c)0x00000000ffffffff)
#define FX64C_SIGN_MASK      ((fx64c)0x8000000000000000)

#define FX64C_MAX            ((fx64c)0x7fffffffffffffff)
#define FX64C_MIN            ((fx64c)0x8000000000000000)

#define FX_FX64C_TO_F32(x)   ((f32)((x) / (f32)((fx64c)1 << FX64C_SHIFT)))
#define FX_F32_TO_FX64C(x)   ((fx64c)(((x) > 0) ? \
                                      ((x) * ((fx64c)1 << FX64C_SHIFT) + 0.5f ) : \
                                      ((x) * ((fx64c)1 << FX64C_SHIFT) - 0.5f )))

#define FX64C_CONST(x)      FX_F32_TO_FX64C(x)

// 4294967296 = 2^32

// ==========================================================================
//	fx16: 1:3:12
// ==========================================================================
typedef s16 fx16;
#define FX16_SHIFT          12
#define FX16_INT_SIZE       3
#define FX16_DEC_SIZE       12

#define FX16_INT_MASK       0x7000
#define FX16_DEC_MASK       0x0fff
#define FX16_SIGN_MASK      0x8000

/* 7.999759  */
#define FX16_MAX            ((fx16)0x7fff)
/* -8        */
#define FX16_MIN            ((fx16)0x8000)

#define FX_FX16_TO_F32(x)    ((f32)((x) / (f32)(1 << FX16_SHIFT)))
#define FX_F32_TO_FX16(x)    ((fx16)(((x) > 0) ? \
                                     (fx16)((x) * (1 << FX16_SHIFT) + 0.5f ) : \
                                     (fx16)((x) * (1 << FX16_SHIFT) - 0.5f )))

#define FX16_CONST(x)       FX_F32_TO_FX16(x)



// ==========================================================================
// fx const
// ==========================================================================
#define FX64C_ONE				((fx64c) 0x0000000100000000LL)	// 1.000000000
#define FX64C_HALF				((fx64c) 0x0000000080000000LL)	// 0.500000000
#define FX32_ONE				((fx32) 0x00001000L)			// 1.000
#define FX32_HALF				((fx32) 0x00000800L)			// 0.500
#define FX32_SQRT2				((fx32) 0x000016a1L)			// 1.414
#define FX32_SQRT1_2			((fx32) 0x00000b50L)			// 0.707
#define FX32_SQRT3				((fx32) 0x00001bb6L)			// 1.732
#define FX32_SQRT1_3			((fx32) 0x0000093dL)			// 0.577
#define FX16_ONE				((fx16) 0x1000)					// 1.000
#define FX16_HALF				((fx16) 0x0800)					// 0.500
#define FX16_SQRT2				((fx16) 0x16a1)					// 1.414
#define FX16_SQRT1_2			((fx16) 0x0b50)					// 0.707
#define FX16_SQRT3				((fx16) 0x1bb6)					// 1.732
#define FX16_SQRT1_3			((fx16) 0x093d)					// 0.577

// ==========================================================================
// fx Zp—p
// ==========================================================================
#define	FX_DIV_SHIFT	(32 - FX32_SHIFT)						// 20
#define	FX_DIV_1_2		(1 << (FX_DIV_SHIFT - 1))

#define	FX_SQRT_SHIFT	((32 + FX32_SHIFT) / 2 - FX32_SHIFT)	// 10
#define	FX_SQRT_1_2		(1 << (FX_SQRT_SHIFT - 1))


// ==========================================================================
//	VecFx32
// ==========================================================================
typedef struct
{
    fx32    x;
    fx32    y;
    fx32    z;
}
VecFx32;

// ==========================================================================
//	VecFx16
// ==========================================================================
typedef struct
{
    fx16    x;
    fx16    y;
    fx16    z;
}
VecFx16;



//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------
#define FXM_FX32_TO_FLOAT(a)	((float)(a) / FX32_ONE)
#define FXM_FLOAT_TO_FX32(a)	((fx32)((a) * FX32_ONE))

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// FX32_CAST
/*!
 */
// ==========================================================================
#if FX_INLINE
inline fx32 FX32_CAST(s64 res)
{
#if defined _DEBUG
	if (!(res >= FX32_MIN && res <= FX32_MAX)) {
		amAssert(!"fx32: Overflow/Underflow");
	}
#endif	// #if defined _DEBUG

	return ((fx32)res);
}
#else
extern fx32 FX32_CAST(s64 res);
#endif

// ==========================================================================
// FX_Mul
/*!
	æZ

	@param	v1	[in]	fx32
	@param	v2	[in]	fx32

	@return	fx32 * fx32 ‚ÌæZŒ‹‰Ê
 */
// ==========================================================================
#if FX_INLINE
inline fx32 FX_Mul(fx32 v1, fx32 v2)
{
	return FX32_CAST(((s64)(v1) * v2 + 0x800LL) >> FX32_SHIFT);
}
#else
extern fx32 FX_Mul(fx32 v1, fx32 v2);
#endif

// ==========================================================================
// FX_Mul32x64c
/*!
	æZ

	@param	v1	[in]	fx32
	@param	v2	[in]	fx64c

	@return	fx32 * fx64c ‚ÌæZŒ‹‰Ê
 */
// ==========================================================================
#if FX_INLINE
inline fx32 FX_Mul32x64c(fx32 v32, fx64c v64c)
{
	fx64c	tmp = v64c * v32 + 0x80000000LL;
	return (FX32_CAST(tmp >> FX64C_SHIFT));
}
#else
extern fx32 FX_Mul32x64c(fx32 v32, fx64c v64c);
#endif

// ==========================================================================
// FX_Div
/*!
	œZ

	@param	numer	[in]	fx32 Š„‚ç‚ê‚é”
	@param	denom	[in]	fx32 Š„‚é”

	@return	œZŒ‹‰Ê
 */
// ==========================================================================
#if FX_INLINE
inline fx32 FX_Div(fx32 numer, fx32 denom)
{
	return ((fx32)((((fx64)numer << 32) / denom + FX_DIV_1_2) >> FX_DIV_SHIFT));

}
#else
extern fx32 FX_Div(fx32 numer, fx32 denom);
#endif

// ==========================================================================
// FX_DivS32
/*!
	œZ

	@param	numer	[in]	s32 Š„‚ç‚ê‚é”
	@param	denom	[in]	s32 Š„‚é”

	@return	œZŒ‹‰Ê
 */
// ==========================================================================
#if FX_INLINE
inline s32 FX_DivS32(s32 numer, s32 denom)
{
	return (numer/denom);
}
#else
extern s32 FX_DivS32(s32 numer, s32 denom);
#endif

// ==========================================================================
// FX_DivFx64c
/*!
	œZ

	@param	numer	[in]	fx32 Š„‚ç‚ê‚é”
	@param	denom	[in]	fx32 Š„‚é”

	@return	œZŒ‹‰Ê fx64c
 */
// ==========================================================================
#if FX_INLINE
inline fx64c FX_DivFx64c(fx32 numer, fx32 denom)
{
	return ((fx32)(((fx64)numer << 32) / denom));
}
#else
extern fx64c FX_DivFx64c(fx32 numer, fx32 denom);
#endif

// ==========================================================================
// FX_Mod
/*!
	è—]

	@param	numer	[in]	fx32 Š„‚ç‚ê‚é”
	@param	denom	[in]	fx32 Š„‚é”

	@return	è—]Œ‹‰Ê
 */
// ==========================================================================
#if FX_INLINE
inline fx32 FX_Mod(fx32 numer, fx32 denom)
{
	return ((fx32)(((fx64)numer << 32) % denom + FX_DIV_1_2) >> FX_DIV_SHIFT);
}
#else
extern fx32 FX_Mod(fx32 numer, fx32 denom);
#endif

// ==========================================================================
// FX_ModS32
/*!
	è—]

	@param	numer	[in]	s32 Š„‚ç‚ê‚é”
	@param	denom	[in]	s32 Š„‚é”

	@return	è—]Œ‹‰Ê
 */
// ==========================================================================
#if FX_INLINE
inline s32 FX_ModS32(s32 numer, s32 denom)
{
	return (numer%denom);
}
#else
extern s32 FX_ModS32(s32 numer, s32 denom);
#endif

// ==========================================================================
// FX_Inv
/*!
	‹t”æ“¾

	@param	denom	[in]	fx32

	@return	‹t”
 */
// ==========================================================================
#if FX_INLINE
inline fx32 FX_Inv(fx32 denom)
{
	return (FX_Div(FX32_ONE, denom));
}
#else
extern fx32 FX_Inv(fx32 denom);
#endif

// ==========================================================================
// FX_InvFx64c
/*!
	‹t”æ“¾

	@param	denom	[in]	fx32

	@return	‹t” fx64c
 */
// ==========================================================================
#if FX_INLINE
inline fx64c FX_InvFx64c(fx32 denom)
{
	return (FX_DivFx64c(FX32_ONE, denom));
}
#else
extern fx64c FX_InvFx64c(fx32 denom);
#endif

// ==========================================================================
// FX_Sqrt
/*!
	•½•ûªæ“¾

	@param	denom	[in]	fx32

	@return	•½•ûª
 */
// ==========================================================================
#if FX_INLINE
inline fx32 FX_Sqrt(fx32 x)
{
	float	f;

	f = nnSqrt(FXM_FX32_TO_FLOAT(x));
	return ((fx32)nnRoundOff(f * FX32_ONE + 0.5f));

}
#else
extern fx32 FX_Sqrt(fx32 x);
#endif

// ==========================================================================
// FX_Sin
/*!
	Sin

	@param	angle	[in]	Šp“x

	@return	³Œ·‚Ì‹ß—’l
 */
// ==========================================================================
#if FX_INLINE
inline fx32 FX_Sin(s32 angle)
{
	float	f = nnSin(angle);

	//return ((fx32)nnRoundOff(f * FX32_ONE + 0.5f));
	return (FXM_FLOAT_TO_FX32(f));
}
#else
extern fx32 FX_Sin(s32 angle);
#endif

// ==========================================================================
// FX_Cos
/*!
	Cos

	@param	angle	[in]	Šp“x

	@return	—]Œ·‚Ì‹ß—’l
 */
// ==========================================================================
#if FX_INLINE
inline fx32 FX_Cos(s32 angle)
{
	float	f = nnCos(angle);

	//return ((fx32)nnRoundOff(f * FX32_ONE + 0.5f));
	return (FXM_FLOAT_TO_FX32(f));
}
#else
extern fx32 FX_Cos(s32 angle);
#endif


// ==========================================================================
// VEC_Set
/*!
	VectorƒZƒbƒg

	@param	a	[in]	VecFx32
	@param	x	[in]	X
	@param	y	[in]	Y
	@param	z	[in]	Z

	@return	—]Œ·‚Ì‹ß—’l
 */
// ==========================================================================
inline void VEC_Set(VecFx32 *a, fx32 x, fx32 y, fx32 z)
{
    a->x = x;
    a->y = y;
    a->z = z;
}

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // FX_H_

//----- Include Files -------------------------------------------------------
