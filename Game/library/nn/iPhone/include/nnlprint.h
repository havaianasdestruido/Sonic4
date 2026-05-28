/*---------------------------------------------------------------------------

    NN Print Library

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN Print Library
    File    : nnlprint.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Modify  : 2003/07/17 NND_DEBUG_BUF_W,NND_DEBUG_BUF_Hを変更
    Modify  : 2003/08/15 デバッグ文字用バッファをユーザー設定に変更
                         nnInitPrint変更、nnGetPrintBufferSize追加
    Modify  : 2003/10/08 Xbox,PC共通化向けの修正
    Modify  : 2004/04/15 nnldebug.h → nnlprint.h
    Modify  : 2004/06/16 DX8,DX9版統合
    Modify  : 2004/08/05 NND_PLATFORM_XENON追加
    Modify  : 2004/08/12 OpenGL版統合
    Modify  : 2005/03/10 DT06版統合
    Modify  : 2005/03/14 PSP版統合
    Modify  : 2006/03/15 PLAYSTATION3版統合
    Modify  : 2008/05/21 NND_PLATFORM_DXG20追加
    Modify  : 2009/05/13 NND_PLATFORM_XENON削除
    Version : 1.16.22
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLPRINT_H__
#define __NNLPRINT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

Uint32 nnGetPrintBufferSize( Sint32 n );
void nnInitPrint( void *buf, Sint32 n, const void *font );
void nnExitPrint( void );
void nnSetPrintSize( Float sizex, Float sizey );
void nnSetPrintColor( NNS_RGBA8888 c );
void nnPrint( Sint32 x, Sint32 y, const char *fmt, ... );
void nnFlushPrint( void );

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* PlayStation2 Print library */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nnlprintps2.h"
#endif

/* GAMECUBE Print library */
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nnlprintgc.h"
#endif

/* Xbox Print library */
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nnlprintdx.h"
#endif

/* PC Print library */
#if ( (NND_PLATFORM == NND_PLATFORM_DX8) || (NND_PLATFORM == NND_PLATFORM_DX9) )
#include "nnlprintdx.h"
#endif

/* OpenGL Print library */
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#include "nnlprintgl.h"
#endif

/* PSP Print library */
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#include "nnlprintpsp.h"
#endif

/* PS3 Print library */
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
#include "nnlprintps3.h"
#endif

/* DXG20 Print library */
#if ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#include "nnlprintdxg20.h"
#endif

#endif /* __NNLPRINT_H__ */

/* End of file */
