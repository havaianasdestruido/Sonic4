/*---------------------------------------------------------------------------

    NN material header

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN material
    File    : nnfmaterial.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Modify  : 2003/10/08 Xbox,PCã§í âªå¸ÇØÇÃèCê≥
    Modify  : 2004/06/16 DX8,DX9î≈ìùçá
    Modify  : 2004/08/05 NND_PLATFORM_XENONí«â¡
    Modify  : 2004/08/12 OpenGLî≈ìùçá
    Modify  : 2005/03/10 DT06î≈ìùçá
    Modify  : 2005/03/14 PSPî≈ìùçá
    Modify  : 2006/03/15 PLAYSTATION3î≈ìùçá
    Modify  : 2008/05/21 NND_PLATFORM_DXG20í«â¡
    Modify  : 2009/03/05 GLES11í«â¡
    Modify  : 2009/05/13 NND_PLATFORM_XENONçÌèú
    Version : 1.16.32
    Note    :

---------------------------------------------------------------------------*/

#ifndef	__NNFMATERIAL_H__
#define	__NNFMATERIAL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Material type */
typedef Uint32	NNF_MATTYPE;

/* Material pointer */
typedef struct {
	NNF_MATTYPE			fType;
	void				*pMaterial; 	/* Material Pointer */
} NNS_MATERIALPTR;

#ifdef __cplusplus
}
#endif /* __cplusplus */

/********************************************/
/* PlayStation2 Default type and structures */
/********************************************/
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nnfmaterialps2.h"
#endif //PlayStation2

/****************************************/
/* GAMECUBE Default type and structures */
/****************************************/
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nnfmaterialgc.h"
#endif //GAMECUBE

/************************************/
/* Xbox Default type and structures */
/************************************/
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nnfmaterialdx.h"
#endif //Xbox

/************************************/
/* PC Default type and structures */
/************************************/
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9 )
#include "nnfmaterialdx.h"
#endif //PC

/**************************************/
/* OpenGL Default type and structures */
/**************************************/
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#include "nnfmaterialgl.h"
#endif //OpenGL

/************************************/
/* PSP Default type and structures */
/************************************/
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#include "nnfmaterialpsp.h"
#endif //PSP

/*************************************/
/* DXG20 Default type and structures */
/*************************************/
#if ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#include "nnfmaterialdxg20.h"
#endif


/***********************************/
/* PS3 Default type and structures */
/***********************************/
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
#include "nnfmaterialps3.h"
#endif //PS3

#endif	//__NNFMATERIAL_H__

/* End of file */
