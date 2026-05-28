/*---------------------------------------------------------------------------

    NN Math Library for OpenGL

    Copyright (C) 2004 SEGA Corporation Creative Center Graphics Sect.
    All Rights Reserved.

    Module  : NN Math Library for DX
    File    : nnlmathgl.h
    Create  : 2004/04/20
    Modify  : 
    Version : 0.01.00
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLMATHGL_H__
#define __NNLMATHGL_H__

#include <math.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define	INVRANDMAX		(Float)(1.0 / ((double)RAND_MAX + 1.0))
#define	nnRandom()		((Float)rand() * (INVRANDMAX))
#define	nnRandomSeed(n)	(srand((Uint32)(n)))

#define	nnAbs(n)		((Float)fabs((double)(n)))
#define	nnArcCos(n)		((Angle32)NNM_RADtoA32(acos((double)(n)) ))
#define	nnArcSin(n)		((Angle32)NNM_RADtoA32(asin((double)(n)) ))
#define	nnArcTan(n)		((Angle32)NNM_RADtoA32(atan((double)(n)) ))
#define	nnArcTan2(y,x)	((Angle32)NNM_RADtoA32(atan2((double)(y), (double)(x))))
Float	nnCos(Angle32 ang);
#define	nnExp(x)		((Float)exp((x)))
#define	nnFloor(n)		((Float)floor((double)(n)))
Float	nnFraction(Float n);
#define	nnHypot(x,y)	((Float)sqrt((double)((x) * (x) + (y) * (y))))
Float	nnInvertSqrt(Float n);
#define	nnLog(n)		((Float)log((double)(n)))
#define	nnLog10(n)		((Float)log10((double)(n)))
#define	nnPow(n1,n2)	((Float)pow((double)(n1), (double)(n2)))
Float	nnRoundOff(Float n);
Float	nnRoundUp(Float n);
Float	nnSin(Angle32 ang);
void	nnSinCos(Angle32 ang, Float *s, Float *c );
Float	nnSqrt(Float n);
#define	nnTan(ang)		((Float)tan(NNM_A32toRAD(ang)))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNLMATHGL_H__ */

/* End of file */
