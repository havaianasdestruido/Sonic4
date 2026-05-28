/*---------------------------------------------------------------------------

    NN Math

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN Math Library
    File    : nnlmath.h
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
    Modify  : 2009/05/13 NND_PLATFORM_XENONçÌèú
    Version : 1.16.22
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLMATH_H__
#define __NNLMATH_H__

/* PlayStation2 math */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nnlmathps2.h"
#endif

/* GAMECUBE math */
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nnlmathgc.h"
#endif

/* Xbox math */
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nnlmathdx.h"
#endif

/* PC math */
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9 )
#include "nnlmathdx.h"
#endif

/* OpenGL math */
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#include "nnlmathgl.h"
#endif

/* PSP math */
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#include "nnlmathpsp.h"
#endif

/* PS3 math */
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
#include "nnlmathps3.h"
#endif

/* DXG20 math */
#if ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#include "nnlmathdxg20.h"
#endif

#endif /* __NNLMATH_H__ */

/* End of file */
