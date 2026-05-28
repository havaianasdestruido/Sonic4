/*---------------------------------------------------------------------------

    NN light header

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN light
    File    : nnflight.h
    Create  : 2003/02/06
    Modify  : 2003/03/28
    Modify  : 2003/10/08 Xbox,PC共通化向けの修正
    Modify  : 2004/06/07 DX8,DX9版統合
    Modify  : 2004/08/05 NND_PLATFORM_XENON追加
    Modify  : 2004/08/12 OpenGL版統合
    Modify  : 2004/08/19 ディレクショナルライト追加
    Modify  : 2005/03/10 DT06版統合
    Modify  : 2005/03/14 PSP版統合
    Modify  : 2006/03/15 PLAYSTATION3版統合
    Modify  : 2008/05/21 NND_PLATFORM_DXG20追加
    Modify  : 2009/03/16 OpenGL ES 1.1統合
    Modify  : 2009/05/13 NND_PLATFORM_XENON削除
    Version : 1.16.71
    Note    :

---------------------------------------------------------------------------*/

#ifndef	__NNFLIGHT_H__
#define	__NNFLIGHT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***********************/
/* NN Light Structure  */
/***********************/

/* Light type */
typedef Uint32	NNF_LIGHTTYPE;

/* Common light type */
#define NND_LIGHTTYPE_PARALLEL				((Uint32)1 << 0)	/* パラレルライト */
#define NND_LIGHTTYPE_POINT					((Uint32)1 << 1)	/* ポイントライト */
#define NND_LIGHTTYPE_TARGET_SPOT			((Uint32)1 << 2)	/* ターゲットスポットライト */
#define NND_LIGHTTYPE_ROTATION_SPOT			((Uint32)1 << 3)	/* ローテーションスポットライト */
#define NND_LIGHTTYPE_TARGET_DIRECTIONAL	((Uint32)1 << 4)	/* ターゲットディレクショナルライト */
#define NND_LIGHTTYPE_ROTATION_DIRECTIONAL	((Uint32)1 << 5)	/* ローテーションディレクショナルライト */

#define NND_LIGHTTYPE_COMMON_MASK	\
				( NND_LIGHTTYPE_PARALLEL | NND_LIGHTTYPE_POINT |	\
					NND_LIGHTTYPE_TARGET_SPOT | NND_LIGHTTYPE_ROTATION_SPOT |	\
					NND_LIGHTTYPE_TARGET_DIRECTIONAL | NND_LIGHTTYPE_ROTATION_DIRECTIONAL )


/* Parallel light */
typedef struct {
	Uint32			User;
	NNS_RGBA		Color;
	Float			Intensity;
	NNS_VECTOR		Direction;
} NNS_LIGHT_PARALLEL;

/* Point light */
typedef struct {
	Uint32			User;
	NNS_RGBA		Color;
	Float			Intensity;
	NNS_VECTOR		Position;
	Float			FallOffStart;
	Float			FallOffEnd;
} NNS_LIGHT_POINT;

/* Target spot light */
typedef struct {
	Uint32			User;
	NNS_RGBA		Color;
	Float			Intensity;
	NNS_VECTOR		Position;
	NNS_VECTOR		Target;
	Angle32			InnerAngle;
	Angle32			OuterAngle;
	Float			FallOffStart;
	Float			FallOffEnd;
} NNS_LIGHT_TARGET_SPOT;

/* Rotation spot light */
typedef struct {
	Uint32			User;
	NNS_RGBA		Color;
	Float			Intensity;
	NNS_VECTOR		Position;
	NNE_ROTATETYPE	RotType;
	NNS_ROTATE_A32	Rotation;
	Angle32			InnerAngle;
	Angle32			OuterAngle;
	Float			FallOffStart;
	Float			FallOffEnd;
} NNS_LIGHT_ROTATION_SPOT;

/* Target directional light */
typedef struct {
	Uint32			User;
	NNS_RGBA		Color;
	Float			Intensity;
	NNS_VECTOR		Position;
	NNS_VECTOR		Target;
	Float			InnerRange;
	Float			OuterRange;
	Float			FallOffStart;
	Float			FallOffEnd;
} NNS_LIGHT_TARGET_DIRECTIONAL;

/* Rotation directional light */
typedef struct {
	Uint32			User;
	NNS_RGBA		Color;
	Float			Intensity;
	NNS_VECTOR		Position;
	NNE_ROTATETYPE	RotType;
	NNS_ROTATE_A32	Rotation;
	Float			InnerRange;
	Float			OuterRange;
	Float			FallOffStart;
	Float			FallOffEnd;
} NNS_LIGHT_ROTATION_DIRECTIONAL;


/* Light pointer */
typedef struct {
	NNF_LIGHTTYPE	fType;
	void			*pLight; 		/* Light pointer */
} NNS_LIGHTPTR;

#ifdef __cplusplus
}
#endif /* __cplusplus */

/********************************************/
/* PlayStation2 Default type and structures */
/********************************************/
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nnflightps2.h"
#endif

/****************************************/
/* GAMECUBE Default type and structures */
/****************************************/
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nnflightgc.h"
#endif

/************************************/
/* Xbox Default type and structures */
/************************************/
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nnflightdx.h"
#endif

/**********************************/
/* PC Default type and structures */
/**********************************/
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
#include "nnflightdx.h"
#endif

/**************************************/
/* OpenGL Default type and structures */
/**************************************/
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#include "nnflightgl.h"
#endif //OpenGL

/************************************/
/* PSP Default type and structures */
/************************************/
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#include "nnflightpsp.h"
#endif //PSP

/*************************************/
/* DXG20 Default type and structures */
/*************************************/
#if (NND_PLATFORM == NND_PLATFORM_DXG20)
#include "nnflightdxg20.h"
#endif

/***********************************/
/* PS3 Default type and structures */
/***********************************/
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
#include "nnflightps3.h"
#endif //PS3

#endif	//__NNFLIGHT_H__

/* End of file */
