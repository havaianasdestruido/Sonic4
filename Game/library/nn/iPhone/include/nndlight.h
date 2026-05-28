/*---------------------------------------------------------------------------

    NN dev Light

    Copyright (C) 2004 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN dev Light Library
    File    : nndlight.h
    Create  : 2004/08/26
    Modify  : 2004/08/26
    Modify  : 2004/09/13 ÉÇÅ[ÉVÉáÉìï‚ä‘ÇÃNNS_VECTORFAST*ÇNNS_VECTOR*Ç…ïœçX
    Version : 1.17.00
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNDLIGHT_H__
#define __NNDLIGHT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void nnCalcLightMotionCore( NNS_LIGHTPTR *dstptr, const NNS_LIGHTPTR *camptr, const NNS_MOTION *mot, Float frame );
void nnCalcMotionLightScalar( const NNS_SUBMOTION *submot, Float frame, Float *val );
void nnCalcMotionLightAngle( const NNS_SUBMOTION *submot, Float frame, Angle32 *ang );
void nnCalcMotionLightXYZ(const NNS_SUBMOTION *submot, Float frame, NNS_VECTOR *xyz);
void nnCalcMotionLightRGB(const NNS_SUBMOTION *submot, Float frame, NNS_RGB* col);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNDLIGHT_H__ */

/* End of file */
