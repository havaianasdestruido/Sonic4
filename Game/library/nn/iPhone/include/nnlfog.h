/*---------------------------------------------------------------------------

    NN Fog Library

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN Light Library
    File    : nnlfog.h
    Create  : 2002/09/03
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
    Version : 1.16.12
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLFOG_H__
#define __NNLFOG_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void nnSetFogSwitch(NNE_BOOL on_off);
NNE_BOOL nnGetFogSwitch(void);
void nnSetFogColor(Float r, Float g, Float b);
void nnSetFogRange(Float fnear, Float ffar);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* PlayStation2 fog library */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nnlfogps2.h"
#endif

/* GAMECUBE fog library */
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nnlfoggc.h"
#endif

/* Xbox fog library */
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nnlfogdx.h"
#endif

/* PC fog library */
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
#include "nnlfogdx.h"
#endif

/* OpenGL fog library */
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#include "nnlfoggl.h"
#endif

/* PSP fog library */
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
//#include "nnlfogpsp.h"
#endif

/* PS3 fog library */
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
#include "nnlfogps3.h"
#endif

/* DXG20 fog library */
#if ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#include "nnlfogdxg20.h"
#endif

#endif /* __NNLFOG_H__ */

/* End of file */
