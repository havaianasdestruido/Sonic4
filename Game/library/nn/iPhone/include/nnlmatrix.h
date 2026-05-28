/*---------------------------------------------------------------------------

    NN Matrix

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN Matrix Library
    File    : nnlmatrix.h
    Create  : 2002/05/10
    Modify  : 2003/04/24  nn***Rotate?MatrixSinCosånä÷êîí«â¡
    Modify  : 2003/04/25  nnCopyMatrixTranslationVectorånä÷êîí«â¡
    Modify  : 2003/07/15  nnScaleAddVector(Fast), nnDistanceVector(Fast),
                          nnDistanceSqVector(Fast)ä÷êîí«â¡
    Modify  : 2003/09/11  à¯êîñºÇÃsin, cosÇs, cÇ…ïœçX
    Modify  : 2004/01/08  nnInvertOrthoMatrixä÷êîí«â¡
    Modify  : 2004/12/07 nnSetCurrentMatrixí«â¡
    Modify  : 2005/02/15 nnInvertTransposeMatrix33(NotNormalized)()Çí«â¡
    Modify  : 2005/03/29 nnMultiplyProjectionMatrix,nnMultiply(Translation|Scaling)Matrix44Çí«â¡
    Modify  : 2005/09/07 nnTransformVector4D, nnTransformVectorPS2í«â¡
    Modify  : 2009/03/05 nnMakePerspectiveOffCenterMatrixÇí«â¡
    Modify  : 2005/02/15 nnInvertTransposeMatrix33(NotNormalized)()ÇçÌèú
    Version : 1.18.42
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLMATRIX_H__
#define __NNLMATRIX_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
	Uint32 nMtx;
	Uint32 StackIdx;
	NNS_MATRIX *pStackTop;
	NNS_MATRIX *pCurrent;
} NNS_MATRIXSTACK;


// Matrix
void nnCopyMatrix( NNS_MATRIX *dst, const NNS_MATRIX *src );
NNE_BOOL nnInvertMatrix( NNS_MATRIX *dst, const NNS_MATRIX *src );
void nnInvertOrthoMatrix( NNS_MATRIX *dst, const NNS_MATRIX *src );
void nnMultiplyMatrix( NNS_MATRIX *dst, const NNS_MATRIX *mtx1, const NNS_MATRIX *mtx2 );
void nnTransposeMatrix( NNS_MATRIX *dst, const NNS_MATRIX *src );
void nnCopyVectorMatrixTranslation( NNS_MATRIX *mtx, const NNS_VECTOR *vec );
void nnCopyVectorFastMatrixTranslation( NNS_MATRIX *mtx, const NNS_VECTORFAST *vec );

// Make Matrix
void nnMakeUnitMatrix( NNS_MATRIX *dst );
void nnMakePerspectiveMatrix( NNS_MATRIX44 *dst, Angle32 fovy, Float aspect, Float znear, Float zfar );
void nnMakePerspectiveOffCenterMatrix( NNS_MATRIX44 *mtx, Float left, Float right, Float bottom, Float top, Float znear, Float zfar );
void nnMakeOrthoMatrix( NNS_MATRIX44 *mtx, Float left, Float right, Float bottom, Float top, Float znear, Float zfar );
void nnMakeQuaternionMatrix( NNS_MATRIX *dst, const NNS_QUATERNION *quat );
void nnMakeRotateXMatrix( NNS_MATRIX *dst, Angle32 ax );
void nnMakeRotateYMatrix( NNS_MATRIX *dst, Angle32 ay );
void nnMakeRotateZMatrix( NNS_MATRIX *dst, Angle32 az );
void nnMakeRotateXYZMatrix( NNS_MATRIX *dst, Angle32 ax, Angle32 ay, Angle32 az );
void nnMakeRotateXZYMatrix( NNS_MATRIX *dst, Angle32 ax, Angle32 ay, Angle32 az );
void nnMakeRotateZXYMatrix( NNS_MATRIX *dst, Angle32 ax, Angle32 ay, Angle32 az );
void nnMakeRotateXMatrixSinCos( NNS_MATRIX *mtx, Float s, Float c );
void nnMakeRotateYMatrixSinCos( NNS_MATRIX *mtx, Float s, Float c );
void nnMakeRotateZMatrixSinCos( NNS_MATRIX *mtx, Float s, Float c );
void nnMakeRotateAxisMatrix( NNS_MATRIX *dst, Float vx, Float vy, Float vz, Angle32 ang );
void nnMakeScaleMatrix( NNS_MATRIX *dst, Float x, Float y, Float z );
void nnMakeTranslateMatrix( NNS_MATRIX *dst, Float x, Float y, Float z );

// Apply Matrix
void nnQuaternionMatrix( NNS_MATRIX *dst, const NNS_MATRIX *src, const NNS_QUATERNION *quat );
void nnRotateXMatrix( NNS_MATRIX *dst, const NNS_MATRIX *src, Angle32 ax );
void nnRotateYMatrix( NNS_MATRIX *dst, const NNS_MATRIX *src, Angle32 ay );
void nnRotateZMatrix( NNS_MATRIX *dst, const NNS_MATRIX *src, Angle32 az );
void nnRotateXMatrixSinCos( NNS_MATRIX *dst, const NNS_MATRIX *src, Float s, Float c );
void nnRotateYMatrixSinCos( NNS_MATRIX *dst, const NNS_MATRIX *src, Float s, Float c );
void nnRotateZMatrixSinCos( NNS_MATRIX *dst, const NNS_MATRIX *src, Float s, Float c );
void nnRotateXYZMatrix( NNS_MATRIX *dst, const NNS_MATRIX *src, Angle32 ax, Angle32 ay, Angle32 az );
void nnRotateXZYMatrix( NNS_MATRIX *dst, const NNS_MATRIX *src, Angle32 ax, Angle32 ay, Angle32 az );
void nnRotateZXYMatrix( NNS_MATRIX *dst, const NNS_MATRIX *src, Angle32 ax, Angle32 ay, Angle32 az );
void nnRotateAxisMatrix( NNS_MATRIX *dst, const NNS_MATRIX *src, Float vx, Float vy, Float vz, Angle32 ang );
void nnScaleMatrix( NNS_MATRIX *dst, const NNS_MATRIX *src, Float x, Float y, Float z );
void nnTranslateMatrix( NNS_MATRIX *dst, const NNS_MATRIX *src, Float x, Float y, Float z );

// Matrix (for NNS_MATRIX44)
void nnMultiplyProjectionMatrix( NNS_MATRIX44 *dst, const NNS_MATRIX44 *projmtx, const NNS_MATRIX *src );
void nnMultiplyTranslationMatrix44( NNS_MATRIX44 *dst, Float x, Float y, Float z, const NNS_MATRIX44 *src );
void nnMultiplyScalingMatrix44( NNS_MATRIX44 *dst, Float x, Float y, Float z, const NNS_MATRIX44 *src );

// Vector (for NNS_VECTOR)
void nnAddVector( NNS_VECTOR *dst, const NNS_VECTOR *vec1, const NNS_VECTOR *vec2 );
void nnCrossProductVector( NNS_VECTOR *dst, const NNS_VECTOR *vec1, const NNS_VECTOR *vec2 );
void nnCopyVector( NNS_VECTOR *dst, const NNS_VECTOR *src );
Float nnDotProductVector( const NNS_VECTOR *vec1, const NNS_VECTOR *vec2 );
Float nnLengthVector( const NNS_VECTOR *vec );
Float nnLengthSqVector( const NNS_VECTOR *vec );
Float nnDistanceVector( const NNS_VECTOR *vec1, const NNS_VECTOR *vec2 );
Float nnDistanceSqVector( const NNS_VECTOR *vec1, const NNS_VECTOR *vec2 );
NNE_BOOL nnNormalizeVector( NNS_VECTOR *dst, const NNS_VECTOR *src );
void nnScaleVector( NNS_VECTOR *dst, const NNS_VECTOR *src, Float scale );
void nnScaleAddVector( NNS_VECTOR *dst, const NNS_VECTOR *vec1, const NNS_VECTOR *vec2, Float scale );
void nnSubtractVector( NNS_VECTOR *dst, const NNS_VECTOR *vec1, const NNS_VECTOR *vec2 );
void nnTransformVector( NNS_VECTOR *dst, const NNS_MATRIX *mtx, const NNS_VECTOR *src );
void nnTransformNormalVector( NNS_VECTOR *dst, const NNS_MATRIX *mtx, const NNS_VECTOR *src );
void nnCopyMatrixTranslationVector( NNS_VECTOR *dst, const NNS_MATRIX *mtx );

// Vector (for NNS_VECTORFAST)
void nnSetUpVectorFast( NNS_VECTORFAST *dst, Float x, Float y, Float z );
void nnAddVectorFast( NNS_VECTORFAST *dst, const NNS_VECTORFAST *vec1, const NNS_VECTORFAST *vec2 );
void nnCrossProductVectorFast( NNS_VECTORFAST *dst, const NNS_VECTORFAST *vec1, const NNS_VECTORFAST *vec2 );
void nnCopyVectorFast( NNS_VECTORFAST *dst, const NNS_VECTORFAST *src );
Float nnDotProductVectorFast( const NNS_VECTORFAST *vec1, const NNS_VECTORFAST *vec2 );
Float nnLengthVectorFast( const NNS_VECTORFAST *vec );
Float nnLengthSqVectorFast( const NNS_VECTORFAST *vec );
Float nnDistanceVectorFast( const NNS_VECTORFAST *vec1, const NNS_VECTORFAST *vec2 );
Float nnDistanceSqVectorFast( const NNS_VECTORFAST *vec1, const NNS_VECTORFAST *vec2 );
NNE_BOOL nnNormalizeVectorFast( NNS_VECTORFAST *dst, const NNS_VECTORFAST *src );
void nnScaleVectorFast( NNS_VECTORFAST *dst, const NNS_VECTORFAST *src, Float scale );
void nnScaleAddVectorFast( NNS_VECTORFAST *dst, const NNS_VECTORFAST *vec1, const NNS_VECTORFAST *vec2, Float scale );
void nnSubtractVectorFast( NNS_VECTORFAST *dst, const NNS_VECTORFAST *vec1, const NNS_VECTORFAST *vec2 );
void nnTransformVectorFast( NNS_VECTORFAST *dst, const NNS_MATRIX  *mtx, const NNS_VECTORFAST *src );
void nnTransformNormalVectorFast( NNS_VECTORFAST *dst, const NNS_MATRIX *mtx, const NNS_VECTORFAST *src );
void nnCopyMatrixTranslationVectorFast( NNS_VECTORFAST *dst, const NNS_MATRIX *mtx );

#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
void nnTransformVectorPS2( NNS_VECTOR4D *dst, const NNS_MATRIX44 *mtx, const NNS_VECTORFAST *src );
#endif
void nnTransformVector4D( NNS_VECTOR4D *dst, const NNS_MATRIX44 *mtx, const NNS_VECTOR4D *src );

// Matrix stack
void nnSetUpMatrixStack( NNS_MATRIXSTACK *mstk, void *buf, Uint32 size );
void nnClearMatrixStack( NNS_MATRIXSTACK *mstk );
NNS_MATRIX *nnGetCurrentMatrix( NNS_MATRIXSTACK *mstk );
void nnSetCurrentMatrix( NNS_MATRIXSTACK *mstk, const NNS_MATRIX *mtx );
void nnPushMatrix( NNS_MATRIXSTACK *mstk, const NNS_MATRIX *mtx );
void nnPopMatrix( NNS_MATRIXSTACK *mstk );

// Quaternion
void nnCopyQuaternion( NNS_QUATERNION *dst, const NNS_QUATERNION *src );
void nnMultiplyQuaternion( NNS_QUATERNION *dst, const NNS_QUATERNION *quat1, const NNS_QUATERNION *quat2 );
NNE_BOOL nnNormalizeQuaternion( NNS_QUATERNION *dst, const NNS_QUATERNION *src );
NNE_BOOL nnInvertQuaternion( NNS_QUATERNION *dst, const NNS_QUATERNION *src );
void nnLogQuaternion( NNS_QUATERNION *dst, const NNS_QUATERNION *src );
void nnExpQuaternion( NNS_QUATERNION *dst, const NNS_QUATERNION *src );
void nnLerpQuaternion( NNS_QUATERNION *dst, const NNS_QUATERNION *quat1, const NNS_QUATERNION *quat2, Float t );
void nnSlerpQuaternion( NNS_QUATERNION *dst, const NNS_QUATERNION *quat1, const NNS_QUATERNION *quat2, Float t );
void nnSquadQuaternion( NNS_QUATERNION *dst, const NNS_QUATERNION *quat1, const NNS_QUATERNION *quata, const NNS_QUATERNION *quatb, const NNS_QUATERNION *quat2, Float t );
void nnMakeUnitQuaternion( NNS_QUATERNION *dst );
void nnMakeRotateAxisQuaternion( NNS_QUATERNION *dst, Float vx, Float vy, Float vz, Angle32 ang );
void nnMakeRotateMatrixQuaternion( NNS_QUATERNION *dst, const NNS_MATRIX *mtx );
void nnMakeRotateXYZQuaternion( NNS_QUATERNION *dst, Angle32 rx, Angle32 ry, Angle32 rz );
void nnMakeRotateXZYQuaternion( NNS_QUATERNION *dst, Angle32 rx, Angle32 ry, Angle32 rz );
void nnMakeRotateZXYQuaternion( NNS_QUATERNION *dst, Angle32 rx, Angle32 ry, Angle32 rz );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNLMATRIX_H__ */

/* End of file */
