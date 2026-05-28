/*---------------------------------------------------------------------------

    NN common definition

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN common definition
    File    : nns.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Modify  : 2003/10/08 PC向け修正 d3d*.hを追加
    Modify  : 2004/03/29 Xboxマトリクス定義修正
    Modify  : 2004/04/21 NND_COMPILER, NND_VECTORFASTTYPE, NND_MATRIXTYPE 定義追加
    Modify  : 2004/06/17 DX8,DX9版統合
    Modify  : 2004/06/29 onWin版統合
    Modify  : 2004/07/01 PS2onWin版修正(d3d8.h, d3dx8.h のインクルード追加)
    Modify  : 2004/07/20 _XENON追加
    Modify  : 2004/08/05 NND_PLATFORM_XENON追加
    Modify  : 2004/08/12 OpenGL版統合
    Modify  : 2004/08/23 NND_FLOAT_LARGE_VALUE 追加
    Modify  : 2004/09/10 OpenGL版でwindows.hのincludeを追加
    Modify  : 2004/12/16 glut.hをgl.hに変更/<gl/ .h>を<GL/ .h>に変更
    Modify  : 2005/03/01 PS2onWin版修正(d3d9.h, d3dx9.h のインクルードに変更)
    Modify  : 2005/03/01 LINDBERG用定義追加、変更
    Modify  : 2005/03/10 DT06版統合
    Modify  : 2005/03/30 segaglprocs.h のインクルード追加
    Modify  : 2005/05/30 PSP版統合 PSP用ALIGNマクロ追加
                         NNS_MATRIX,NNS_VECTORFAST,NNS_QUATERNIONを16Byteアラインへ変更
                         NNS_VECTORFASTのメンバーに”w”を追加
    Modify  : 2005/05/31 PSP版,NNS_QUATERNIONを16Byteアラインは問題があるのでアラインし
                         ないように修正
    Modify  : 2006/01/12 PS3版追加
    Modify  : 2006/03/15 XENON版の一部追記
    Modify  : 2007/12/02 LINDBERGH版でglprocsの替わりにgleeを使うように変更
    Modify  : 2008/05/19 _DXG20追加
    Modify  : 2008/05/21 NND_PLATFORM_DXG20追加
    Modify  : 2009/01/27 NNS_TEXCOORD_U16,NNS_COLOR16追加
    Modify  : 2009/02/24 NND_PLATFORM_GLES11追加
    Modify  : 2009/03/16 WIN32の条件追加
    Modify  : 2009/05/13 NND_PLATFORM_XENON削除
    Version : 1.18.29
    Note    :

---------------------------------------------------------------------------*/

#ifndef __NNS_H__
#define __NNS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/********************/
/* Platform         */
/********************/
#define     NND_PLATFORM_PS2    (1)
#define     NND_PLATFORM_GC     (2)
#define     NND_PLATFORM_XB     (3)
#define     NND_PLATFORM_PC     (4)
#define     NND_PLATFORM_GL     (5)
#define     NND_PLATFORM_DX8    (6)
#define     NND_PLATFORM_DX9    (7)
#define     NND_PLATFORM_XENON  (8)
#define     NND_PLATFORM_PSP    (9)
#define     NND_PLATFORM_PS3    (10)
#define     NND_PLATFORM_DT06   (11)
#define     NND_PLATFORM_DXG20  (12)
#define     NND_PLATFORM_GLES11 (13)

#ifndef NND_PLATFORM
#if     defined( __ee__ )
#define     NND_PLATFORM    NND_PLATFORM_PS2
#elif   defined( GEKKO )
#define     NND_PLATFORM    NND_PLATFORM_GC
#elif   defined( _XBOX )
#if _XBOX_VER >= 200
#define     NND_PLATFORM    NND_PLATFORM_DXG20
#else
#define     NND_PLATFORM    NND_PLATFORM_XB
#endif
#elif   defined( _DX8 )
#define     NND_PLATFORM    NND_PLATFORM_DX8
#elif   defined( _DX9 )
#define     NND_PLATFORM    NND_PLATFORM_DX9
#elif   defined( LINDBERG )
#define     NND_PLATFORM    NND_PLATFORM_GL
#elif   defined( SN_TARGET_PS3 )
#define     NND_PLATFORM    NND_PLATFORM_PS3
#elif   defined( PS3_ON_WIN )
#define     NND_PLATFORM    NND_PLATFORM_PS3
#elif   defined( _PSP )
#define     NND_PLATFORM    NND_PLATFORM_PSP
#elif   defined( _DXG20 )
#define     NND_PLATFORM    NND_PLATFORM_DXG20
#elif   defined( _GLES11 )
#define     NND_PLATFORM    NND_PLATFORM_GLES11
#endif
#endif

/* Compiler */
#define     NND_COMPILER_CW         (1)
#define     NND_COMPILER_VC         (2)
#define     NND_COMPILER_GCC        (3)
#define     NND_COMPILER_SNC        (4)
#define     NND_COMPILER_OTHER      (99)

#if     ( NND_PLATFORM == NND_PLATFORM_PS2 || NND_PLATFORM == NND_PLATFORM_GC )
#if     defined( _MSC_VER )
#define     NND_COMPILER    NND_COMPILER_VC
#elif   defined( __MWERKS__ )
#define     NND_COMPILER    NND_COMPILER_CW
#elif   defined(__GNUC__) || defined(__GCC__)
#define     NND_COMPILER    NND_COMPILER_GCC
#else
#define     NND_COMPILER    NND_COMPILER_OTHER
#endif
#elif   ( NND_PLATFORM == NND_PLATFORM_PS3 )
#if defined(__GNUC__) || defined(__GCC__)
#define     NND_COMPILER    NND_COMPILER_GCC
#elif       defined( __SNC__ )
#define     NND_COMPILER    NND_COMPILER_SNC
#elif       defined( _MSC_VER )
#define     NND_COMPILER    NND_COMPILER_VC
#else
#define     NND_COMPILER    NND_COMPILER_OTHER
#endif
#elif   defined( _MSC_VER )
#define     NND_COMPILER    NND_COMPILER_VC
#elif   defined( LINDBERG )
#define     NND_COMPILER    NND_COMPILER_GCC
#else
#define     NND_COMPILER    NND_COMPILER_OTHER
#endif

/* Alignment macro */
#if ( NND_COMPILER == NND_COMPILER_CW || NND_COMPILER == NND_COMPILER_GCC || NND_COMPILER == NND_COMPILER_SNC )
#define NNM_ALIGN_CW( n )   __attribute__((aligned( n )))
#define NNM_ALIGN_VC( n )
#endif
#if ( NND_COMPILER == NND_COMPILER_VC )
#define NNM_ALIGN_VC( n )   _declspec(align( n ))
#define NNM_ALIGN_CW( n )
#pragma warning(push,4)
#pragma warning(disable:4127)   // conditional expression is constant
#pragma warning(disable:4201)   // nonstandard extension used : nameless struct/union
#pragma warning(disable:4213)   // nonstandard extension used : cast on l-value
#pragma warning(disable:4324)   // '<unnamed-tag>' : structure was padded due to __declspec(align())
#endif
#if ( NND_COMPILER == NND_COMPILER_OTHER )
#define NNM_ALIGN_VC( n )
#define NNM_ALIGN_CW( n )
#endif

#ifdef PS3_ON_WIN
#pragma warning (disable: 4201)
#pragma warning (disable: 4127)
#endif

#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#define NNM_ALIGN_PSP( n )  __attribute__((aligned( n )))
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

/***************/
/* Basic types */
/***************/

#if (( NND_PLATFORM == NND_PLATFORM_PS2 ) && defined( WIN32 ) )
#include    <d3d9.h>
#include    <d3dx9.h>

#elif ( NND_PLATFORM == NND_PLATFORM_GC )
#include    <dolphin.h>
#include    <stdio.h>

#elif ( NND_PLATFORM == NND_PLATFORM_XB )
#include    <xtl.h>
#include    <xgraphics.h>

#elif ( NND_PLATFORM == NND_PLATFORM_DX8 )
#include    <d3d8.h>
#include    <d3dx8.h>

#elif ( NND_PLATFORM == NND_PLATFORM_DX9 )
#include    <d3d9.h>
#include    <d3dx9.h>

#elif ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#if defined( _XBOX )
// DXG20 On xbox360
#include    <xtl.h>
#include    <xgraphics.h>
#endif
#include    <d3d9.h>
#include    <d3dx9.h>

#elif ( NND_PLATFORM == NND_PLATFORM_GL )
#ifdef      _SEGAGL
#include    <GL/gl.h>
#include    <SEGAGL/segaglprocs.h>
#else
#include    <GL/GLee.h>
#endif

#elif (( NND_PLATFORM == NND_PLATFORM_PSP ) && defined( WIN32 ) )
#include    <GLee.h>

#elif ( NND_PLATFORM == NND_PLATFORM_GLES11 )
#ifndef _IPHONE
#include    <GLES/gl.h>
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#if (( NND_PLATFORM == NND_PLATFORM_PS2 ) && ( NND_COMPILER != NND_COMPILER_VC ))
#define PXD_NOINCLUDE_SG_XPT
#if ( NND_COMPILER == NND_COMPILER_CW )
#pragma fast_fptosi on
#endif /* ( NND_COMPILER == NND_COMPILER_CW ) */
#ifndef NULL
#define NULL    0
#endif
#ifndef _TYPEDEF_Uint128
#define _TYPEDEF_Uint128
typedef unsigned int        Uint128 __attribute__ ((mode (TI)));
#endif
#ifndef _TYPEDEF_Sint128
#define _TYPEDEF_Sint128
typedef int                 Sint128 __attribute__ ((mode (TI)));
#endif
#ifndef _TYPEDEF_Uint64
#define _TYPEDEF_Uint64
typedef unsigned long       Uint64;
#endif
#ifndef _TYPEDEF_Sint64
#define _TYPEDEF_Sint64
typedef long                Sint64;
#endif
#ifndef _TYPEDEF_Uint32
#define _TYPEDEF_Uint32
typedef unsigned int        Uint32;
#endif
#ifndef _TYPEDEF_Sint32
#define _TYPEDEF_Sint32
typedef int                 Sint32;
#endif
#ifndef _TYPEDEF_Uint16
#define _TYPEDEF_Uint16
typedef unsigned short      Uint16;
#endif
#ifndef _TYPEDEF_Sint16
#define _TYPEDEF_Sint16
typedef short               Sint16;
#endif
#ifndef _TYPEDEF_Uint8
#define _TYPEDEF_Uint8
typedef unsigned char       Uint8;
#endif
#ifndef _TYPEDEF_Sint8
#define _TYPEDEF_Sint8
typedef signed char         Sint8;
#endif
#ifndef _TYPEDEF_Float
#define _TYPEDEF_Float
typedef float               Float;
#endif

#elif ( NND_PLATFORM == NND_PLATFORM_GC )
#ifndef _TYPEDEF_Uint64
#define _TYPEDEF_Uint64
typedef u64                 Uint64;
#endif
#ifndef _TYPEDEF_Sint64
#define _TYPEDEF_Sint64
typedef s64                 Sint64;
#endif
#ifndef _TYPEDEF_Uint32
#define _TYPEDEF_Uint32
typedef u32                 Uint32;
#endif
#ifndef _TYPEDEF_Sint32
#define _TYPEDEF_Sint32
typedef s32                 Sint32;
#endif
#ifndef _TYPEDEF_Uint16
#define _TYPEDEF_Uint16
typedef u16                 Uint16;
#endif
#ifndef _TYPEDEF_Sint16
#define _TYPEDEF_Sint16
typedef s16                 Sint16;
#endif
#ifndef _TYPEDEF_Uint8
#define _TYPEDEF_Uint8
typedef u8                  Uint8;
#endif
#ifndef _TYPEDEF_Sint8
#define _TYPEDEF_Sint8
typedef s8                  Sint8;
#endif
#ifndef _TYPEDEF_Float
#define _TYPEDEF_Float
typedef f32                 Float;
#endif

#elif ( NND_PLATFORM == NND_PLATFORM_PSP )
#ifndef WIN32

#ifndef _TYPEDEF_Uint128
#define _TYPEDEF_Uint128
typedef unsigned int        Uint128 __attribute__((mode(TI)));
#endif
#ifndef _TYPEDEF_Sint128
#define _TYPEDEF_Sint128
typedef int                 Sint128 __attribute__((mode(TI)));
#endif
#ifndef _TYPEDEF_Uint64
#define _TYPEDEF_Uint64
typedef unsigned int        Uint64  __attribute__((mode(DI)));
#endif
#ifndef _TYPEDEF_Sint64
#define _TYPEDEF_Sint64
typedef int                 Sint64  __attribute__((mode(DI)));
#endif
#ifndef _TYPEDEF_Uint32
#define _TYPEDEF_Uint32
typedef unsigned int        Uint32;
#endif
#ifndef _TYPEDEF_Sint32
#define _TYPEDEF_Sint32
typedef int                 Sint32;
#endif
#ifndef _TYPEDEF_Uint16
#define _TYPEDEF_Uint16
typedef unsigned short      Uint16;
#endif
#ifndef _TYPEDEF_Sint16
#define _TYPEDEF_Sint16
typedef short               Sint16;
#endif
#ifndef _TYPEDEF_Uint8
#define _TYPEDEF_Uint8
typedef unsigned char       Uint8;
#endif
#ifndef _TYPEDEF_Sint8
#define _TYPEDEF_Sint8
typedef char                Sint8;
#endif
#ifndef _TYPEDEF_Float
#define _TYPEDEF_Float
typedef float               Float;
#endif
#endif
#elif ( NND_PLATFORM == NND_PLATFORM_PS3 )
#ifdef _WIN32
#define CELL_SDK_VERSION    0x080000
#else
#include <../../common/include/sdk_version.h>
#endif
#ifndef NULL
#define NULL    0
#endif
#ifndef _TYPEDEF_Uint64
#define _TYPEDEF_Uint64
#if CELL_SDK_VERSION>=0x080000
typedef unsigned long long  Uint64;
#else
typedef unsigned long       Uint64;
#endif
#endif
#ifndef _TYPEDEF_Sint64
#define _TYPEDEF_Sint64
#if CELL_SDK_VERSION>=0x080000
typedef signed long long    Sint64;
#else
typedef long                Sint64;
#endif
#endif
#ifndef _TYPEDEF_Uint32
#define _TYPEDEF_Uint32
typedef unsigned int        Uint32;
#endif
#ifndef _TYPEDEF_Sint32
#define _TYPEDEF_Sint32
typedef int                 Sint32;
#endif
#ifndef _TYPEDEF_Uint16
#define _TYPEDEF_Uint16
typedef unsigned short      Uint16;
#endif
#ifndef _TYPEDEF_Sint16
#define _TYPEDEF_Sint16
typedef short               Sint16;
#endif
#ifndef _TYPEDEF_Uint8
#define _TYPEDEF_Uint8
typedef unsigned char       Uint8;
#endif
#ifndef _TYPEDEF_Sint8
#define _TYPEDEF_Sint8
typedef	signed char			Sint8;
#endif
#ifndef _TYPEDEF_Float
#define _TYPEDEF_Float
typedef float               Float;
#endif
#elif ( NND_PLATFORM == NND_PLATFORM_GLES11 )
#ifndef _IPHONE
#include <GLES/gl.h>
#else
#include <OpenGLES/ES1/gl.h>
#endif
#endif

#if ( NND_COMPILER == NND_COMPILER_VC )
#ifndef NULL
#define NULL    (0)
#endif
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#ifndef _TYPEDEF_Uint128
#define _TYPEDEF_Uint128
typedef struct {                            /* signed 16 byte integer   */
    unsigned long   h;                      /* upper 64bit */
    unsigned long   l;                      /* lower 64bit */
} Uint128;
#endif
#ifndef _TYPEDEF_Sint128
#define _TYPEDEF_Sint128
typedef struct {                            /* unsigned 16 byte integer */
    unsigned long   h;                      /* upper 64bit */
    unsigned long   l;                      /* lower 64bit */
} Sint128;
#endif
#endif  /* ( NND_PLATFORM == NND_PLATFORM_PS2 ) */
#ifndef _TYPEDEF_Uint64
#define _TYPEDEF_Uint64
typedef unsigned _int64     Uint64;
#endif
#ifndef _TYPEDEF_Sint64
#define _TYPEDEF_Sint64
typedef _int64              Sint64;
#endif
#ifndef _TYPEDEF_Uint32
#define _TYPEDEF_Uint32
typedef unsigned long       Uint32;
#endif
#ifndef _TYPEDEF_Sint32
#define _TYPEDEF_Sint32
typedef long                Sint32;
#endif
#ifndef _TYPEDEF_Uint16
#define _TYPEDEF_Uint16
typedef unsigned short      Uint16;
#endif
#ifndef _TYPEDEF_Sint16
#define _TYPEDEF_Sint16
typedef short               Sint16;
#endif
#ifndef _TYPEDEF_Uint8
#define _TYPEDEF_Uint8
typedef unsigned char       Uint8;
#endif
#ifndef _TYPEDEF_Sint8
#define _TYPEDEF_Sint8
typedef signed char         Sint8;
#endif
#ifndef _TYPEDEF_Float
#define _TYPEDEF_Float
typedef float               Float;
#endif

#elif ( NND_COMPILER == NND_COMPILER_OTHER )
#ifndef NULL
#define NULL    (0)
#endif
#ifndef _TYPEDEF_Uint64
#define _TYPEDEF_Uint64
typedef unsigned long long  Uint64;
#endif
#ifndef _TYPEDEF_Sint64
#define _TYPEDEF_Sint64
typedef long long           Sint64;
#endif
#ifndef _TYPEDEF_Uint32
#define _TYPEDEF_Uint32
typedef unsigned long       Uint32;
#endif
#ifndef _TYPEDEF_Sint32
#define _TYPEDEF_Sint32
typedef long                Sint32;
#endif
#ifndef _TYPEDEF_Uint16
#define _TYPEDEF_Uint16
typedef unsigned short      Uint16;
#endif
#ifndef _TYPEDEF_Sint16
#define _TYPEDEF_Sint16
typedef short               Sint16;
#endif
#ifndef _TYPEDEF_Uint8
#define _TYPEDEF_Uint8
typedef unsigned char       Uint8;
#endif
#ifndef _TYPEDEF_Sint8
#define _TYPEDEF_Sint8
typedef signed char         Sint8;
#endif
#ifndef _TYPEDEF_Float
#define _TYPEDEF_Float
typedef float               Float;
#endif
#endif  // NND_COMPILER

/* Angle */
typedef Float           Radian;
typedef Sint32          Angle32;
typedef Sint16          Angle16;

/* Bool */
typedef enum {
    NNE_FALSE = 0,
    NNE_OFF = 0,
    NNE_TRUE = 1,
    NNE_ON = 1
} NNE_BOOL;

/* Vector */
typedef struct {
    Float   x;
    Float   y;
    Float   z;
} NNS_VECTOR;


#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
typedef NNM_ALIGN_VC( 16 ) struct {
    Float   x;
    Float   y;
    Float   z;
    Float   w;
} NNS_VECTORFAST NNM_ALIGN_CW( 16 );
#elif ( NND_PLATFORM == NND_PLATFORM_GC )
typedef struct {
    Float   x;
    Float   y;
    Float   z;
} NNS_VECTORFAST;
#elif ( NND_PLATFORM == NND_PLATFORM_XB || NND_PLATFORM == NND_PLATFORM_DX9 || NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DXG20 )
typedef NNM_ALIGN_VC( 16 ) struct {
    Float   x;
    Float   y;
    Float   z;
    Float   w;
} NNS_VECTORFAST;
#elif ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_PS3 || NND_PLATFORM == NND_PLATFORM_GLES11 )
typedef NNM_ALIGN_VC( 16 ) struct {
    Float   x;
    Float   y;
    Float   z;
    Float   w;  /* 不定値 */
} NNS_VECTORFAST NNM_ALIGN_CW( 16 );
#elif ( NND_PLATFORM == NND_PLATFORM_PSP )
#ifndef WIN32
typedef struct {
    Float   x;
    Float   y;
    Float   z;
    Float   w;
} NNS_VECTORFAST NNM_ALIGN_PSP(16);
#else
typedef NNM_ALIGN_VC( 16 ) struct {
    Float   x;
    Float   y;
    Float   z;
    Float   w;
} NNS_VECTORFAST;
#endif
#endif

#define     NND_VECTORFASTTYPE_XYZ          (1)
#define     NND_VECTORFASTTYPE_XYZW         (2)

#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#define     NND_VECTORFASTTYPE      NND_VECTORFASTTYPE_XYZW
#elif ( NND_PLATFORM == NND_PLATFORM_GC )
#define     NND_VECTORFASTTYPE      NND_VECTORFASTTYPE_XYZ
#elif ( NND_PLATFORM == NND_PLATFORM_XB || NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9 || NND_PLATFORM == NND_PLATFORM_DXG20 )
#define     NND_VECTORFASTTYPE      NND_VECTORFASTTYPE_XYZW
#elif ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#define     NND_VECTORFASTTYPE      NND_VECTORFASTTYPE_XYZ
#elif ( NND_PLATFORM == NND_PLATFORM_PSP )
#define     NND_VECTORFASTTYPE      NND_VECTORFASTTYPE_XYZW
#elif ( NND_PLATFORM == NND_PLATFORM_PS3 )
#define     NND_VECTORFASTTYPE      NND_VECTORFASTTYPE_XYZW
#endif


typedef struct {
    Sint16  x;
    Sint16  y;
    Sint16  z;
} NNS_VECTOR_S16;

typedef struct {
    Sint8   x;
    Sint8   y;
    Sint8   z;
} NNS_VECTOR_S8;

typedef struct {
    Float   x;
    Float   y;
} NNS_VECTOR2D;

typedef struct {
    Float   x;
    Float   y;
    Float   z;
    Float   w;
} NNS_VECTOR4D;

/* Rotation */
typedef struct {
    Float   x;
    Float   y;
    Float   z;
} NNS_ROTATE;

typedef struct {
    Angle32 x;
    Angle32 y;
    Angle32 z;
} NNS_ROTATE_A32;

typedef struct {
    Angle16 x;
    Angle16 y;
    Angle16 z;
} NNS_ROTATE_A16;

/* Texture coordinate */
typedef struct {
    Float   u;
    Float   v;
} NNS_TEXCOORD;

typedef struct {
    Sint16  u;
    Sint16  v;
} NNS_TEXCOORD_S16;

typedef struct {
    Uint16  u;
    Uint16  v;
} NNS_TEXCOORD_U16;

/* Quatanion */
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
typedef NNM_ALIGN_VC( 16 ) struct {
	Float	x;
	Float	y;
	Float	z;
	Float	w;
} NNS_QUATERNION NNM_ALIGN_CW( 16 );
#else
typedef struct {
	Float	x;
	Float	y;
	Float	z;
	Float	w;
} NNS_QUATERNION;
#endif

#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
typedef NNM_ALIGN_VC( 16 ) struct {
    Float   x;
    Float   y;
    Float   z;
    Float   w;
} NNS_QUATERNIONFAST NNM_ALIGN_CW( 16 );
#endif

/* Color */
typedef struct {
    Float   r;
    Float   g;
    Float   b;
} NNS_RGB;

typedef struct {
    Float   r;
    Float   g;
    Float   b;
    Float   a;
} NNS_RGBA;

typedef struct {
    Uint8   r;
    Uint8   g;
    Uint8   b;
    Uint8   a;
} NNS_RGB_U8;

typedef struct {
    Uint8   r;
    Uint8   g;
    Uint8   b;
    Uint8   a;
} NNS_RGBA_U8;

typedef struct {
    Uint32  r;
    Uint32  g;
    Uint32  b;
    Uint32  a;
} NNS_RGBA_U32;

typedef Uint32  NNS_RGBA8888;
typedef Uint16  NNS_COLOR16;

/* Matrix */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
typedef NNM_ALIGN_VC( 16 )  Float   NNS_MATRIX[4][4]    NNM_ALIGN_CW( 16 ); /* 4x4 Matrix */
typedef NNM_ALIGN_VC( 16 )  Float   NNS_MATRIX44[4][4]  NNM_ALIGN_CW( 16 ); /* 4x4 Matrix */
#elif ( NND_PLATFORM == NND_PLATFORM_GC )
typedef Mtx     NNS_MATRIX;         /* 3x4 Matrix */
typedef Mtx44   NNS_MATRIX44;       /* 4x4 Matrix */
#elif ( NND_PLATFORM == NND_PLATFORM_XB )
typedef NNM_ALIGN_VC( 16 )  Float   NNS_MATRIX[4][4];   /* 4x4 Matrix */
typedef NNM_ALIGN_VC( 16 )  Float   NNS_MATRIX44[4][4]; /* 4x4 Matrix */
#elif ( NND_PLATFORM == NND_PLATFORM_PC || NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9 || NND_PLATFORM == NND_PLATFORM_DXG20 )
typedef NNM_ALIGN_VC( 16 )  Float   NNS_MATRIX[4][4];   /* 4x4 Matrix */
typedef NNM_ALIGN_VC( 16 )  Float   NNS_MATRIX44[4][4]; /* 4x4 Matrix */
#elif ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
typedef Float   NNS_MATRIX[16];
typedef Float   NNS_MATRIX44[16];
#elif ( NND_PLATFORM == NND_PLATFORM_PSP )
#ifndef WIN32
typedef Float   NNS_MATRIX[16]      NNM_ALIGN_PSP(16);
typedef Float   NNS_MATRIX44[16]    NNM_ALIGN_PSP(16);
#else
typedef Float   NNS_MATRIX[16]; /* 4x4 Matrix */
typedef Float   NNS_MATRIX44[16];   /* 4x4 Matrix */
#endif
#elif ( NND_PLATFORM == NND_PLATFORM_PS3 )
typedef NNM_ALIGN_VC( 16 )  Float   NNS_MATRIX[16]      NNM_ALIGN_CW( 16 );
typedef NNM_ALIGN_VC( 16 )  Float   NNS_MATRIX44[16]    NNM_ALIGN_CW( 16 );
#else
typedef Float   NNS_MATRIX[3][4];
typedef Float   NNS_MATRIX44[4][4];
#endif

#define     NND_MATRIXTYPE_34       (1)
#define     NND_MATRIXTYPE_44       (2)

#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#define     NND_MATRIXTYPE      NND_MATRIXTYPE_44
#elif ( NND_PLATFORM == NND_PLATFORM_GC )
#define     NND_MATRIXTYPE      NND_MATRIXTYPE_34
#elif ( NND_PLATFORM == NND_PLATFORM_XB )
#define     NND_MATRIXTYPE      NND_MATRIXTYPE_44
#elif ( NND_PLATFORM == NND_PLATFORM_DX8 )
#define     NND_MATRIXTYPE      NND_MATRIXTYPE_44
#elif ( NND_PLATFORM == NND_PLATFORM_DX9 )
#define     NND_MATRIXTYPE      NND_MATRIXTYPE_44
#elif ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#define     NND_MATRIXTYPE      NND_MATRIXTYPE_44
#elif ( NND_PLATFORM == NND_PLATFORM_PSP )
#define     NND_MATRIXTYPE      NND_MATRIXTYPE_44
#elif ( NND_PLATFORM == NND_PLATFORM_PS3 )
#define     NND_MATRIXTYPE      NND_MATRIXTYPE_44
#elif ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#define     NND_MATRIXTYPE      NND_MATRIXTYPE_44
#else
#define     NND_MATRIXTYPE      NND_MATRIXTYPE_34
#endif


/***************************/
/* Basic conversion macros */
/***************************/
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#define NNM_MTX( mtx, row, column ) ( (mtx)[(column)][(row)] )
#elif ( NND_PLATFORM == NND_PLATFORM_XB )
#define NNM_MTX( mtx, row, column ) ( (mtx)[(column)][(row)] )
#elif ( NND_PLATFORM == NND_PLATFORM_DX8 )
#define NNM_MTX( mtx, row, column ) ( (mtx)[(column)][(row)] )
#elif ( NND_PLATFORM == NND_PLATFORM_DX9 )
#define NNM_MTX( mtx, row, column ) ( (mtx)[(column)][(row)] )
#elif ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#define NNM_MTX( mtx, row, column ) ( (mtx)[(column) * 4 + (row)] )
#elif ( NND_PLATFORM == NND_PLATFORM_PSP )
#define NNM_MTX( mtx, row, column ) ( (mtx)[(column) * 4 + (row)] )
#elif ( NND_PLATFORM == NND_PLATFORM_PS3 )
#define NNM_MTX( mtx, row, column ) ( (mtx)[(column) * 4 + (row)] )
#elif ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#define NNM_MTX( mtx, row, column ) ( (mtx)[(column)][(row)] )
#else
#define NNM_MTX( mtx, row, column ) ( (mtx)[(row)][(column)] )
#endif

#define NNM_DEGtoRAD(n)     ((n) * 0.017453293f)            /* Degree -> Radian  */
#define NNM_DEGtoA32(n)     ((Angle32)((n) * 182.04444f))   /* Degree -> Angle32 */
#define NNM_RADtoA32(n)     ((Angle32)((n) * 10430.378f))   /* Radian -> Angle32 */
#define NNM_RADtoDEG(n)     ((n) * 57.295780f)              /* Radian -> Degree  */
#define NNM_A32toDEG(n)     ((n) * 0.0054931641f)           /* Angle32 -> Degree */
#define NNM_A32toRAD(n)     ((n) * 0.000095873799f)         /* Angle32 -> Radian */


#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#define NNM_DEGtoA16(n)     ((Angle16)((Angle32)((n) * 182.04444f)))    /* Degree -> Angle16 */
#else
#define NNM_DEGtoA16(n)     ((Angle16)((n) * 182.04444f))   /* Degree -> Angle16 */
#endif
#define NNM_RADtoA16(n)     ((Angle16)((n) * 10430.378f))   /* Radian -> Angle16 */
#define NNM_A16toDEG(n)     ((n) * 0.0054931641f)           /* Angle16 -> Degree */
#define NNM_A16toRAD(n)     ((n) * 0.000095873799f)         /* Angle16 -> Radian */

#define NNM_MAX(a,b) ((a)>(b)?(a):(b))
#define NNM_MIN(a,b) ((a)<(b)?(a):(b))


/***************************/
/* Basic definition macros */
/***************************/
#define NND_PI                  (3.1415926536f)
#define NND_FLOAT_LARGE_VALUE   (1.0e+12f)

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif //__NNS_H__

/* End of file */
