/*---------------------------------------------------------------------------

    NN camera header

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN camera
    File    : nnfcamera.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Version : 1.00.00
    Note    : 

---------------------------------------------------------------------------*/

#ifndef	__NNFCAMERA_H__
#define	__NNFCAMERA_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/************************/
/* NN Camera Structure  */
/************************/
/* Camera type */
typedef Uint32	NNF_CAMERATYPE;

/* Camera type member */
#define NND_CAMERATYPE_MEMBER_USER		((Uint32)1 << 0)
#define NND_CAMERATYPE_MEMBER_FOVY		((Uint32)1 << 1)
#define NND_CAMERATYPE_MEMBER_ASPECT	((Uint32)1 << 2)
#define NND_CAMERATYPE_MEMBER_ZNEAR		((Uint32)1 << 3)
#define NND_CAMERATYPE_MEMBER_ZFAR		((Uint32)1 << 4)
#define NND_CAMERATYPE_MEMBER_POSITION	((Uint32)1 << 5)
#define NND_CAMERATYPE_MEMBER_TARGET	((Uint32)1 << 6)
#define NND_CAMERATYPE_MEMBER_ROLL		((Uint32)1 << 7)
#define NND_CAMERATYPE_MEMBER_UPVECTOR	((Uint32)1 << 8)
#define NND_CAMERATYPE_MEMBER_UPTARGET	((Uint32)1 << 9)
#define NND_CAMERATYPE_MEMBER_ROTTYPE	((Uint32)1 << 10)
#define NND_CAMERATYPE_MEMBER_ROTATION	((Uint32)1 << 11)

#define NND_CAMERATYPE_MEMBER_COMMON	\
	( NND_CAMERATYPE_MEMBER_USER | NND_CAMERATYPE_MEMBER_FOVY |	\
		NND_CAMERATYPE_MEMBER_ASPECT | NND_CAMERATYPE_MEMBER_ZNEAR |	\
		NND_CAMERATYPE_MEMBER_ZFAR | NND_CAMERATYPE_MEMBER_POSITION )


/* Camera type */

/* ローテーションカメラ */
#define	NND_CAMERATYPE_ROTATION	\
	( NND_CAMERATYPE_MEMBER_COMMON |	\
		NND_CAMERATYPE_MEMBER_ROTTYPE | NND_CAMERATYPE_MEMBER_ROTATION )

/* ターゲットロールカメラ */
#define NND_CAMERATYPE_TARGET_ROLL	\
	( NND_CAMERATYPE_MEMBER_COMMON |	\
		NND_CAMERATYPE_MEMBER_TARGET | NND_CAMERATYPE_MEMBER_ROLL )

/* ターゲットアップベクトルカメラ */
#define NND_CAMERATYPE_TARGET_UPVECTOR	\
	( NND_CAMERATYPE_MEMBER_COMMON |	\
		NND_CAMERATYPE_MEMBER_TARGET | NND_CAMERATYPE_MEMBER_UPVECTOR )

/* ターゲットアップターゲットカメラ */
#define NND_CAMERATYPE_TARGET_UPTARGET	\
	( NND_CAMERATYPE_MEMBER_COMMON |	\
		NND_CAMERATYPE_MEMBER_TARGET | NND_CAMERATYPE_MEMBER_UPTARGET )


/* Camera structure */

/* ターゲットロールカメラ */
typedef struct {
	Uint32			User;
	Angle32			Fovy;
	Float			Aspect;
	Float			ZNear;
	Float			ZFar;
	NNS_VECTOR		Position;
	NNS_VECTOR		Target;
	Angle32			Roll;
} NNS_CAMERA_TARGET_ROLL;

/* ターゲットアップベクトルカメラ */
typedef struct {
	Uint32			User;
	Angle32			Fovy;
	Float			Aspect;
	Float			ZNear;
	Float			ZFar;
	NNS_VECTOR		Position;
	NNS_VECTOR		Target;
	NNS_VECTOR		UpVector;
} NNS_CAMERA_TARGET_UPVECTOR;

/* ターゲットアップターゲットカメラ */
typedef struct {
	Uint32			User;
	Angle32			Fovy;
	Float			Aspect;
	Float			ZNear;
	Float			ZFar;
	NNS_VECTOR		Position;
	NNS_VECTOR		Target;
	NNS_VECTOR		UpTarget;
} NNS_CAMERA_TARGET_UPTARGET;

/* ローテーションカメラ */
typedef struct {
	Uint32			User;
	Angle32			Fovy;
	Float			Aspect;
	Float			ZNear;
	Float			ZFar;
	NNS_VECTOR		Position;
	NNE_ROTATETYPE	RotType;
	NNS_ROTATE_A32	Rotation;
} NNS_CAMERA_ROTATION;


/* Camera pointer */
typedef struct {
	NNF_CAMERATYPE		fType;
	void				*pCamera; 		/* Camera Pointer */
} NNS_CAMERAPTR;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	//__NNFCAMERA_H__

/* End of file */
