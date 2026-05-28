/*---------------------------------------------------------------------------

    NN Morph target

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN Morph target Library
    File    : nnlmorph.h
    Create  : 2002/07/24
    Modify  : 2003/03/28
    Modify  : 2003/10/08 Xbox,PCã§í âªå¸ÇØÇÃèCê≥
    Modify  : 2004/06/16 DX8,DX9î≈ìùçá
    Modify  : 2004/08/05 NND_PLATFORM_XENONí«â¡
    Modify  : 2004/08/12 OpenGLî≈ìùçá
    Modify  : 2005/04/26 CALCMORPHÉtÉâÉOÇí«â¡ÅAèCê≥
    Modify  : 2006/03/15 PLAYSTATION3î≈ìùçá
    Modify  : 2008/05/21 NND_PLATFORM_DXG20í«â¡
    Modify  : 2009/05/13 NND_PLATFORM_XENONçÌèú
    Version : 1.16.21
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLMORPH_H__
#define __NNLMORPH_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Morph object pointer */
typedef NNS_OBJECT NNS_MORPHOBJ;

/* Morph object flag */
typedef Uint32 NNF_CALCMORPH;
/* For morph object flag */
#define	NND_CALCMORPH_POSITION			((Uint32)1 << 0)
#define	NND_CALCMORPH_NORMAL			((Uint32)1 << 1)
#define	NND_CALCMORPH_TANGENT			((Uint32)1 << 2)
#define	NND_CALCMORPH_BINORMAL			((Uint32)1 << 3)
#define	NND_CALCMORPH_COLOR				((Uint32)1 << 4)
#define	NND_CALCMORPH_COLOR2			((Uint32)1 << 5)
#define	NND_CALCMORPH_TEXCOORD0			((Uint32)1 << 6)
#define	NND_CALCMORPH_TEXCOORD1			((Uint32)1 << 7)
#define	NND_CALCMORPH_TEXCOORD2			((Uint32)1 << 8)
#define	NND_CALCMORPH_TEXCOORD3			((Uint32)1 << 9)

//#define	NND_CALCMORPH_WEIGHT			((Uint32)1 << 5) /* No support */

#define	NND_CALCMORPH_TEXCOORD_ALL   \
     NND_CALCMORPH_TEXCOORD0 | NND_CALCMORPH_TEXCOORD1 | NND_CALCMORPH_TEXCOORD2 | NND_CALCMORPH_TEXCOORD3

#define	NND_CALCMORPH_ALL   \
     NND_CALCMORPH_POSITION | NND_CALCMORPH_NORMAL | NND_CALCMORPH_TANGENT | NND_CALCMORPH_BINORMAL | \
     NND_CALCMORPH_COLOR | NND_CALCMORPH_COLOR2 | NND_CALCMORPH_TEXCOORD_ALL

#define	NND_CALCMORPH_NRM_NORMALIZE		((Uint32)1 << 10)
#define	NND_CALCMORPH_TAN_NORMALIZE		((Uint32)1 << 11)
#define	NND_CALCMORPH_BNRM_NORMALIZE	((Uint32)1 << 12)

#define	NND_CALCMORPH_NORMALIZE_ALL \
     NND_CALCMORPH_NRM_NORMALIZE | NND_CALCMORPH_TAN_NORMALIZE | NND_CALCMORPH_BNRM_NORMALIZE

#define	NND_CALCMORPH_INDEPENDENT		((Uint32)1 << 13)

#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#define	NND_CALCMORPH_PS2_SINGLEBUFFER	((Uint32)1 << 16)
#define	NND_CALCMORPH_PS2_ACLAMP255		((Uint32)1 << 17)
#endif	// ( NND_PLATFORM == NND_PLATFORM_PS2 )
#if ( NND_PLATFORM == NND_PLATFORM_GL )
#define	NND_CALCMORPH_GL_BINDBUFFER		((Uint32)1 << 16)
#endif	// ( NND_PLATFORM == NND_PLATFORM_GL )
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
#define	NND_CALCMORPH_PS3_BINDBUFFER	((Uint32)1 << 16)
#endif	// ( NND_PLATFORM == NND_PLATFORM_PS3 )

/* Morph object size */
#define	NND_MORPHOBJ_SIZE_NO_MORPH		80	/* sizeof(NNS_MORPHOBJ) size aline */

/* Prototypes */
void nnDrawMorphObject(
	const NNS_MORPHOBJ *mobj, const NNS_MATRIX *mtxpal, const NNF_NODESTATUS *nodestat, 
	NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag
);

Uint32 nnInitMorphObject(
	NNS_MORPHOBJ *mobj, const NNS_OBJECT *obj, const NNS_MORPHTARGETLIST *mtgt,
	NNF_CALCMORPH flag
);

void nnCalcMorphObject(
	NNS_MORPHOBJ *mobj, const NNS_OBJECT *obj, const NNS_MORPHTARGETLIST *mtgt,
	const Float *mwpal, NNF_CALCMORPH flag
);

Uint32 nnCalcMorphObjectBufferSize( 
	const NNS_OBJECT *obj, const NNS_MORPHTARGETLIST *mtgt, NNF_CALCMORPH flag );

void nnCalcMorphMotion( 
	Float *mwpal, const NNS_MORPHTARGETLIST *mtgt, const NNS_MOTION *mot, Float frame );

void nnBlendMorphWeightPalette(
	Float *dstmwpal, const Float *srcmwpal0, Float ratio0,
	const Float *srcmwpal1, Float ratio1, const NNS_MORPHTARGETLIST *mtgt
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* PlayStation2 Morph target library */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nnlmorphps2.h"
#endif

/* GAMECUBE Morph target library */
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nnlmorphgc.h"
#endif

/* Xbox Morph target library */
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nnlmorphdx.h"
#endif

/* PC Morph target library */
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
#include "nnlmorphdx.h"
#endif

/* DXG20 Morph target library */
#if ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#include "nnlmorphdxg20.h"
#endif

#endif /* __NNLMORPH_H__ */

/* End of file */
