/*---------------------------------------------------------------------------

    NN dev Camera

    Copyright (C) 2003 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN dev Camera Library
    File    : nndcamera.h
    Create  : 2003/02/13
    Modify  : 2003/03/28
    Modify  : 2003/05/23 カメラモーションに対応
    Modify  : 2004/06/29 nnTransformUpVectorCameraLocalを追加
    Modify  : 2004/09/13 nnCalcMotionCameraXYZの引数NNS_VECTORFAST*をNNS_VECTOR*に変更
    Version : 1.17.00
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNDCAMERA_H__
#define __NNDCAMERA_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void nnMakeVectorCameraViewMatrix( NNS_MATRIX *mtx, const NNS_VECTOR *pos, const NNS_VECTOR *right, const NNS_VECTOR *up, const NNS_VECTOR *ilook );
void nnCalcCameraMotionCore( NNS_CAMERAPTR *dstptr, const NNS_CAMERAPTR *camptr, const NNS_MOTION *mot, Float frame );
void nnCalcMotionCameraScalar( const NNS_SUBMOTION *submot, Float frame, Float *val );
void nnCalcMotionCameraAngle( const NNS_SUBMOTION *submot, Float frame, Angle32 *ang );
void nnCalcMotionCameraXYZ(const NNS_SUBMOTION *submot, Float frame, NNS_VECTOR *xyz);

void nnTransformUpVectorCameraLocal( NNS_VECTOR *vec, const NNS_CAMERA_TARGET_UPVECTOR *cam, Float x, Float y, Float z );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNDCAMERA_H__ */

/* End of file */
