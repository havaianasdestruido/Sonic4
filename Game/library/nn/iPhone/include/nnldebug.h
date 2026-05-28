/*---------------------------------------------------------------------------

    NN Debug Library

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN Debug Library
    File    : nnldebug.h
    Create  : 2004/04/15
    Modify  : 2004/04/28
    Modify  : 2004/06/16 DX8,DX9î≈ìùçá
    Modify  : 2004/08/05 NND_PLATFORM_XENONí«â¡
    Modify  : 2004/08/12 OpenGLî≈ìùçá
    Modify  : 2005/03/10 DT06î≈ìùçá
    Modify  : 2005/03/14 PSPî≈ìùçá
    Modify  : 2006/03/15 PLAYSTATION3î≈ìùçá
    Modify  : 2008/05/21 NND_PLATFORM_DXG20í«â¡
    Modify  : 2009/05/13 NND_PLATFORM_XENONçÌèú
    Version : 1.16.31
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLDEBUG_H__
#define __NNLDEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(_DEBUG) || defined(DEBUG)
#ifndef NN_DEBUG
#define NN_DEBUG
#endif /* NN_DEBUG */
#endif /* _DEBUG / DEBUG */

#ifdef __cplusplus
}
#endif /* __cplusplus */


/* PlayStation2 Debug library */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nnldebugps2.h"
#endif

/* GAMECUBE Debug library */
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nnldebuggc.h"
#endif

/* Xbox Debug library */
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nnldebugdx.h"
#endif

/* PC Debug library */
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
#include "nnldebugdx.h"
#endif

/* OpenGL Debug library */
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11)
#include "nnldebuggl.h"
#endif

/* PSP Debug library */
#if ( NND_PLATFORM == NND_PLATFORM_PSP)
#include "nnldebugpsp.h"
#endif

/* PS3 Debug library */
#if ( NND_PLATFORM == NND_PLATFORM_PS3)
#include "nnldebugps3.h"
#endif

/* DXG20 Debug library */
#if (NND_PLATFORM == NND_PLATFORM_DXG20)
#include "nnldebugdxg20.h"
#endif

#endif /* __NNLDEBUG_H__ */

/* End of file */
