/*---------------------------------------------------------------------------

    NN Camera Library

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN Camera Library
    File    : nnlcamera.h
    Create  : 2002/05/09
    Modify  : 2003/05/22 カメラモーションに対応
    Modify  : 2003/08/15 __NNLCAMAPI_H__ -> __NNLCAMERA_H__
    Modify  : 2004/06/29 カメラ操作API群を追加
    Modify  : 2004/07/22 nnApproachTargetUpVectorCameraLevel, 
                         nnRotateUpVectorCameraLevelAroundTarget を追加
    Version : 1.16.00
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLCAMERA_H__
#define __NNLCAMERA_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void nnMakeCameraPointerViewMatrix( NNS_MATRIX *mtx, const NNS_CAMERAPTR *camptr );
void nnMakeCameraPointerPerspectiveMatrix( NNS_MATRIX44 *dst, const NNS_CAMERAPTR *camptr );

void nnMakeTargetRollCameraViewMatrix( NNS_MATRIX *mtx, const NNS_CAMERA_TARGET_ROLL *cam );
void nnMakeTargetUpVectorCameraViewMatrix( NNS_MATRIX *mtx, const NNS_CAMERA_TARGET_UPVECTOR *cam ); 
void nnMakeTargetUpTargetCameraViewMatrix( NNS_MATRIX *mtx, const NNS_CAMERA_TARGET_UPTARGET *cam ); 
void nnMakeRotationCameraViewMatrix( NNS_MATRIX *mtx, const NNS_CAMERA_ROTATION *cam ); 

void nnConvertCameraPointerUpVectorCamera( NNS_CAMERA_TARGET_UPVECTOR *cam, const NNS_CAMERAPTR *camptr );

void nnRollUpVectorCamera( NNS_CAMERA_TARGET_UPVECTOR *cam, Angle32 roll );
void nnPitchUpVectorCamera( NNS_CAMERA_TARGET_UPVECTOR *cam, Angle32 pitch );
void nnYawUpVectorCamera( NNS_CAMERA_TARGET_UPVECTOR *cam, Angle32 yaw );
void nnMoveUpVectorCameraLocal( NNS_CAMERA_TARGET_UPVECTOR *cam, Float x, Float y, Float z );
void nnMoveTargetUpVectorCamera( NNS_CAMERA_TARGET_UPVECTOR *cam, Float d );
void nnApproachTargetUpVectorCamera( NNS_CAMERA_TARGET_UPVECTOR *cam, Float d );
void nnApproachTargetUpVectorCameraLevel( NNS_CAMERA_TARGET_UPVECTOR *cam, Float d );
void nnRotateUpVectorCameraAroundTargetH( NNS_CAMERA_TARGET_UPVECTOR *cam, Angle32 ang );
void nnRotateUpVectorCameraAroundTargetV( NNS_CAMERA_TARGET_UPVECTOR *cam, Angle32 ang );
void nnRotateUpVectorCameraLevelAroundTarget( NNS_CAMERA_TARGET_UPVECTOR *cam, Angle32 ang );

Uint32 nnEstimateCameraBufferSize( NNF_CAMERATYPE type );

void nnCalcCameraMotion( NNS_CAMERAPTR *dstptr, const NNS_CAMERAPTR *camptr, const NNS_MOTION *mot, Float frame );

#define NND_CAMERA_MAX_BUFFER_SIZE ( sizeof( NNS_CAMERA_TARGET_UPTARGET ) )
#define NND_CAMERA_MOTION_BUFFER_SIZE ( NND_CAMERA_MAX_BUFFER_SIZE + sizeof( NNS_CAMERAPTR ) )

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNLCAMERA_H__ */


/* End of file */
