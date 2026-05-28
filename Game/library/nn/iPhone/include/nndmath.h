/*---------------------------------------------------------------------------

    NN dev Math

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN dev Math Library
    File    : nndmath.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Modify  : 2003/09/25 ノードユーザデータモーション用の補間を追加
    Modify  : 2003/12/22 マテリアルモーション用の補間を追加
    Modify  : 2004/09/13 nnInterpolateConstantF3,nnInterpolateLinearF3の引数NNS_VECTORFAST*をNNS_VECTOR*に変更
    Modify  : 2004/11/01 nnSolveBezier追加
    Modify  : 2008/04/09 nnSearchTriggerU1を追加
    Version : 1.20.12
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNDMATH_H__
#define __NNDMATH_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


void nnInterpolateConstantF1(const NNS_MOTION_KEY_TRANSLATION_X *vk, Sint32 nKey, Float frame, Float *val);
void nnInterpolateConstantF3(const NNS_MOTION_KEY_TRANSLATION_XYZ *vk, Sint32 nKey, Float frame, NNS_VECTOR *val);
void nnInterpolateConstantA32_1(const NNS_MOTION_KEY_ROTATION_X *vk, Sint32 nKey, Float frame, Angle32 *val);
void nnInterpolateConstantA32_3(const NNS_MOTION_KEY_ROTATION_XYZ *vk, Sint32 nKey, Float frame, NNS_ROTATE_A32 *val);
void nnInterpolateConstantA16_1(const NNS_MOTION_KEY_ROTATION_X_A16 *vk, Sint32 nKey, Float frame, Angle16 *val);
void nnInterpolateConstantA16_3(const NNS_MOTION_KEY_ROTATION_XYZ_A16 *vk, Sint32 nKey, Float frame, NNS_ROTATE_A16 *val);
void nnInterpolateLinearF1(const NNS_MOTION_KEY_TRANSLATION_X *vk, Sint32 nKey, Float frame, Float *val);
void nnInterpolateLinearF3(const NNS_MOTION_KEY_TRANSLATION_XYZ *vk, Sint32 nKey, Float frame, NNS_VECTOR *val);
void nnInterpolateLinearA32_1(const NNS_MOTION_KEY_ROTATION_X *vk, Sint32 nKey, Float frame, Angle32 *val);
void nnInterpolateLinearA32_3(const NNS_MOTION_KEY_ROTATION_XYZ *vk, Sint32 nKey, Float frame, NNS_ROTATE_A32 *val);
void nnInterpolateLinearA16_1(const NNS_MOTION_KEY_ROTATION_X_A16 *vk, Sint32 nKey, Float frame, Angle16 *val);
void nnInterpolateLinearA16_3(const NNS_MOTION_KEY_ROTATION_XYZ_A16 *vk, Sint32 nKey, Float frame, NNS_ROTATE_A16 *val);
void nnInterpolateBezierF1(const NNS_MOTION_KEY_TRANSLATION_X_BEZIER *vk, Sint32 nKey, Float frame, Float *val);
//void nnInterpolateBezierA16_1(const NNS_MOTION_KEY_ROTATION_X_A16_BEZIER *vk, Sint32 nKey, Float frame, Angle16 *val);
void nnInterpolateBezierA32_1(const NNS_MOTION_KEY_ROTATION_X_BEZIER *vk, Sint32 nKey, Float frame, Angle32 *val);
void nnInterpolateLerpA16_3(const NNS_MOTION_KEY_ROTATION_XYZ_A16 *vk, Sint32 nKey, Float frame, NNS_QUATERNION *val, NNF_NODETYPE rtype);
void nnInterpolateLerpA32_3(const NNS_MOTION_KEY_ROTATION_XYZ *vk, Sint32 nKey, Float frame, NNS_QUATERNION *val, NNF_NODETYPE rtype);
void nnInterpolateLerpQuat_4(const NNS_MOTION_KEY_QUATERNION *vk, Sint32 nKey, Float frame, NNS_QUATERNION *val);
void nnInterpolateSlerpA16_3(const NNS_MOTION_KEY_ROTATION_XYZ_A16 *vk, Sint32 nKey, Float frame, NNS_QUATERNION *val, NNF_NODETYPE rtype);
void nnInterpolateSlerpA32_3(const NNS_MOTION_KEY_ROTATION_XYZ *vk, Sint32 nKey, Float frame, NNS_QUATERNION *val, NNF_NODETYPE rtype);
void nnInterpolateSlerpQuat_4(const NNS_MOTION_KEY_QUATERNION *vk, Sint32 nKey, Float frame, NNS_QUATERNION *val);
void nnInterpolateSquadA16_3(const NNS_MOTION_KEY_ROTATION_XYZ_A16 *vk, Sint32 nKey, Float frame, NNS_QUATERNION *val, NNF_NODETYPE rtype);
void nnInterpolateSquadA32_3(const NNS_MOTION_KEY_ROTATION_XYZ *vk, Sint32 nKey, Float frame, NNS_QUATERNION *val, NNF_NODETYPE rtype);
void nnInterpolateSquadQuat_4(const NNS_MOTION_KEY_QUATERNION *vk, Sint32 nKey, Float frame, NNS_QUATERNION *val);
void nnInterpolateConstantQuat_4(const NNS_MOTION_KEY_QUATERNION *vk, Sint32 nKey, Float frame, NNS_QUATERNION *val);
void nnInterpolateSISplineF1(const NNS_MOTION_KEY_FLOAT_SI_SPLINE *vk, Sint32 nKey, Float frame, Float *val);
void nnInterpolateSISplineA32_1(const NNS_MOTION_KEY_ANGLE32_SI_SPLINE *vk, Sint32 nKey, Float frame, Angle32 *val);
void nnInterpolateSISplineA16_1(const NNS_MOTION_KEY_ANGLE16_SI_SPLINE *vk, Sint32 nKey, Float frame, Angle16 *val);
void nnInterpolateConstantU1(const NNS_MOTION_KEY_UINT32 *vk, Sint32 nKey, Float frame, Uint32 *val);
void nnInterpolateLinearU1(const NNS_MOTION_KEY_UINT32 *vk, Sint32 nKey, Float frame, Uint32 *val);
NNE_BOOL nnInterpolateTriggerU1(const NNS_MOTION_KEY_UINT32 *vk, Sint32 nKey, Float frame, Uint32 *val);
void nnSearchTriggerU1( const NNS_MOTION_KEY_UINT32*vk, Sint32 nKey, Float frame, Float interval, NNS_NODEUSRMOT_CALLBACK_FUNC func, NNS_NODEUSRMOT_CALLBACK_VAL *val );

void nnInterpolateConstantS32_1(const NNS_MOTION_KEY_SINT32 *vk, Sint32 nKey, Float frame, Sint32 *val);
void nnInterpolateConstantF2(const NNS_MOTION_KEY_TEXCOORD *vk, Sint32 nKey, Float frame, NNS_TEXCOORD *val);
void nnInterpolateLinearF2(const NNS_MOTION_KEY_TEXCOORD *vk, Sint32 nKey, Float frame, NNS_TEXCOORD *val);

Float nnSolveBezier(Float f0, Float h0, Float f1, Float h1, Float frame);

Sint32 nnCalcMotionKeyF( const void *pMotKey, Sint32 size, Sint32 nKey, Float frame );
Sint32 nnCalcMotionKeyN( const void *pMotKey, Sint32 size, Sint32 nKey, Float frame );

void nnSlerpNoInvQuaternion( NNS_QUATERNION *dst, const NNS_QUATERNION *q1, const NNS_QUATERNION *q2, Float t );
void nnSplineQuaternion( NNS_QUATERNION *dst, const NNS_QUATERNION *quatprev, const NNS_QUATERNION *quat, const NNS_QUATERNION *quatnext );


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNDMATH_H__ */

/* End of file */
