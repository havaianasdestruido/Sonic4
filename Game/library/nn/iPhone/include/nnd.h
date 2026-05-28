/*---------------------------------------------------------------------------

    NN dev api header

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN dev api header
    File    : nnd.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Modify  : 2003/10/08 Xbox,PCã§í âªå¸ÇØÇÃèCê≥
    Modify  : 2004/06/16 DX8,DX9î≈ìùçá
    Modify  : 2004/08/05 NND_PLATFORM_XENONí«â¡
    Modify  : 2004/08/12 OpenGLî≈ìùçá
    Modify  : 2004/08/26 nndlight.hÇí«â¡
    Modify  : 2005/03/10 DT06î≈ìùçá
    Modify  : 2005/03/14 PSPî≈ìùçá
    Modify  : 2006/03/31 PS3î≈ìùçá
    Modify  : 2008/05/21 NND_PLATFORM_DXG20í«â¡
    Modify  : 2009/05/13 NND_PLATFORM_XENONçÌèú
    Version : 1.16.62
    Note    :

---------------------------------------------------------------------------*/

#ifndef	__NND_H__
#define	__NND_H__

/* Math api */
#include <nndmath.h>

/* NN Node */
#include <nndnode.h>

/* NN Camera */
#include <nndcamera.h>

/* NN Light */
#include <nndlight.h>

/* System api */
#include <nndsystem.h>


/* PlayStation2 dev api */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nndps2.h"
#endif

/* GAMECUBE dev api */
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nndgc.h"
#endif

/* Xbox dev api */
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nnddx.h"
#endif

/* PC dev api */
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9 )
#include "nnddx.h"
#endif

/* OpenGL dev api */
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#include "nndgl.h"
#endif

/* PSP dev api */
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#include "nndpsp.h"
#endif

/* PS3 dev api */
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
#include "nndps3.h"
#endif

/* DXG20 dev api */
#if ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#include "nnddxg20.h"
#endif

#endif //__NND_H__

/* End of file */
