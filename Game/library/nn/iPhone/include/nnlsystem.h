/*---------------------------------------------------------------------------

    NN System

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN System Library
    File    : nnlsystem.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Modify  : 2003/09/12 CLIP関連を追加
    Modify  : 2003/10/08 Xbox,PC共通化向けの修正
    Modify  : 2004/06/07 PC向けの修正
    Modify  : 2004/06/11 nnCalcClipBox()追加
    Modify  : 2004/06/16 DX8,DX9版統合
    Modify  : 2004/08/05 NND_PLATFORM_XENON追加
    Modify  : 2004/08/12 OpenGL版統合
    Modify  : 2005/03/10 DT06版統合
    Modify  : 2005/03/14 PSP版統合
    Modify  : 2006/03/15 PLAYSTATION3版統合
    Modify  : 2006/07/20 extern C追加
    Modify  : 2008/05/21 NND_PLATFORM_DXG20追加
    Modify  : 2009/05/13 NND_PLATFORM_XENON削除
    Version : 1.20.20
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLSYSTEM_H__
#define __NNLSYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Clip status (NND_NODESTATUS_ | NND_OBJECTSTATUS_ と値の関連有り) */
typedef Uint32	NNF_CLIP;
#define	NND_CLIP_NONE			((Uint32)0 <<  0)
#define	NND_CLIP_NORMAL			NND_CLIP_NONE
#define	NND_CLIP_INSIDE			((Uint32)1 <<  1)
#define	NND_CLIP_CROSSNEAR		((Uint32)1 <<  2)	/* Straddled near clipping plane */
#define	NND_CLIP_CROSSFAR		((Uint32)1 <<  3)	/* Straddled far clipping plane */
#define	NND_CLIP_OUTSIDE		((Uint32)1 <<  4)
#define	NND_CLIP_PS2_GSINSIDE	((Uint32)1 <<  5)	/* Completely insides of gs window coordinate system */
#define NND_CLIPOBJECT_MASK	(NND_OBJECTSTATUS_HIDE | NND_OBJECTSTATUS_INSIDE)
#define NND_CLIP_STAT_MASK	(NND_CLIP_NONE | NND_CLIP_INSIDE |\
							 NND_CLIP_CROSSNEAR | NND_CLIP_CROSSFAR |\
							 NND_CLIP_OUTSIDE | NND_CLIP_PS2_GSINSIDE)

/* Projection type */
typedef enum{
	NNE_PROJECTION_TYPE_PERSPECTIVE,
	NNE_PROJECTION_TYPE_ORTHO
} NNE_PROJECTION_TYPE;


/* Projection */
void nnSetProjection( const NNS_MATRIX44 *mtx, NNE_PROJECTION_TYPE type );

/* Clip */
void nnSetClipScreenCoordinates(const NNS_VECTOR2D *pos);
void nnSetClipZ(Float znear, Float zfar);

NNF_CLIP nnCalcClip( const NNS_VECTOR *center, Float radius, const NNS_MATRIX *mtx );
NNF_CLIP nnCalcClipCore( const NNS_VECTOR *center, Float radius, const NNS_MATRIX *mtx );
NNF_CLIP nnCalcClipCoreOutsideOnly( const NNS_VECTOR *center, Float radius, const NNS_MATRIX *mtx );
NNF_CLIP nnCalcClipUniformScale( const NNS_VECTOR *center, Float radius, const NNS_MATRIX *mtx, Float factor );
NNF_CLIP nnCalcClipBox( const NNS_VECTOR *center, Float sx, Float sy, Float sz, const NNS_MATRIX *mtx );
NNF_CLIP nnCalcClipBoxOutsideOnly( const NNS_VECTOR *center, Float sx, Float sy, Float sz, const NNS_MATRIX *mtx );

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* PlayStation2 system */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nnlsystemps2.h"
#endif

/* GAMECUBE system */
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nnlsystemgc.h"
#endif

/* Xbox system */
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nnlsystemdx.h"
#endif

/* PC system */
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
#include "nnlsystemdx.h"
#endif

/* OpenGL system */
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#include "nnlsystemgl.h"
#endif

/* PSP system */
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#include "nnlsystempsp.h"
#endif

/* PS3 system */
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
#include "nnlsystemps3.h"
#endif

/* DXG20 system */
#if ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#include "nnlsystemdxg20.h"
#endif

#endif /* __NNLSYSTEM_H__ */

/* End of file */
