/*---------------------------------------------------------------------------

    NN Draw primitive

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN Draw primitive Library
    File    : nnldrawprim.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Modify  : 2003/07/29 nnChangePrimitive3DMatrix()追加
    Modify  : 2003/10/08 Xbox,PC共通化向けの修正
    Modify  : 2003/10/20 NNE_PRIM_BLEND_PS2_CUSTOM 追加
    Modify  : 2004/06/07 PC向けの修正
    Modify  : 2004/06/10 NNE_PRIM_TEXWRAPにPS2用REGION追加
    Modify  : 2004/06/15 NNE_PRIM_ALPHABLENDのONとOFFの定義順を反転
    Modify  : 2004/06/16 DX8,DX9版統合
    Modify  : 2004/08/05 NND_PLATFORM_XENON追加
    Modify  : 2005/03/10 DT06版統合
    Modify  : 2005/03/14 PSP版統合
    Modify  : 2006/03/15 PLAYSTATION3版統合
    Modify  : 2008/05/21 NND_PLATFORM_DXG20追加
    Modify  : 2009/01/27 PSP専用宣言をnndrawprimpsp.hへ移動
    Modify  : 2009/05/13 NND_PLATFORM_XENON削除
    Modify  : 2009/05/13 NND_PLATFORM_GC追加
    Version : 1.16.23
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLDRAWPRIM_H__
#define __NNLDRAWPRIM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* common draw primitive library */

/* プリミティブ 2D 用 */
typedef struct {
	NNS_VECTOR2D	Pos;	/* 頂点座標			*/
} NNS_PRIM2D_P;

typedef struct {
	NNS_VECTOR2D	Pos;	/* 頂点座標			*/
	NNS_RGBA8888	Col;	/* 頂点カラー		*/
} NNS_PRIM2D_PC;

typedef struct {
	NNS_VECTOR2D	Pos;	/* 頂点座標			*/
	NNS_RGBA8888	Col;	/* 頂点カラー		*/
	NNS_TEXCOORD	Tex;	/* テクスチャ座標	*/
} NNS_PRIM2D_PCT;

/* プリミティブ 3D 用 */
typedef struct {
	NNS_VECTOR		Pos;	/* 頂点座標			*/
} NNS_PRIM3D_P;

typedef struct {
	NNS_VECTOR		Pos;	/* 頂点座標			*/
	NNS_VECTOR		Nrm;	/* 頂点法線			*/
} NNS_PRIM3D_PN;

typedef struct {
	NNS_VECTOR		Pos;	/* 頂点座標			*/
	NNS_RGBA8888	Col;	/* 頂点カラー		*/
} NNS_PRIM3D_PC;

typedef struct {
	NNS_VECTOR		Pos;	/* 頂点座標			*/
	NNS_VECTOR		Nrm;	/* 頂点法線			*/
	NNS_TEXCOORD	Tex;	/* テクスチャ座標	*/
} NNS_PRIM3D_PNT;

typedef struct {
	NNS_VECTOR		Pos;	/* 頂点座標			*/
	NNS_RGBA8888	Col;	/* 頂点カラー		*/
	NNS_TEXCOORD	Tex;	/* テクスチャ座標	*/
} NNS_PRIM3D_PCT;

/* 不透明、半透明タイプ */
typedef enum {
	NNE_PRIM_ALPHABLEND_OFF,
	NNE_PRIM_ALPHABLEND_ON
} NNE_PRIM_ALPHABLEND;

/* テクスチャブレンドタイプ */
typedef enum {
	NNE_PRIM_TEXBLEND_MODULATE,
	NNE_PRIM_TEXBLEND_REPLACE
} NNE_PRIM_TEXBLEND;

/* テクスチャラッピングタイプ */
typedef enum {
	NNE_PRIM_TEXWRAP_REPEAT,
	NNE_PRIM_TEXWRAP_CLAMP,
	NNE_PRIM_TEXWRAP_REGION_REPEAT,	/* PrimEx & PS2 Only */
	NNE_PRIM_TEXWRAP_REGION_CLAMP	/* PrimEx & PS2 Only */
} NNE_PRIM_TEXWRAP;

/* テクスチャマッピングタイプ */
typedef enum {
	NNE_PRIM_TEXCOORD_UV,
	NNE_PRIM_TEXCOORD_ENVIRONMENT
} NNE_PRIM_TEXCOORD;

/* プリミティブ2Dフォーマット */
typedef enum {
	NNE_PRIM2D_FMT_P,
	NNE_PRIM2D_FMT_PC,
	NNE_PRIM2D_FMT_PCT
} NNE_PRIM2D_FMT;

/* プリミティブ2D POINT フォーマット */
typedef enum {
	NNE_PRIM2D_POINT_FMT_P,
	NNE_PRIM2D_POINT_FMT_PC
} NNE_PRIM2D_POINT_FMT;

/* プリミティブ3Dフォーマット */
typedef enum {
	NNE_PRIM3D_FMT_P,
	NNE_PRIM3D_FMT_PN,
	NNE_PRIM3D_FMT_PC,
	NNE_PRIM3D_FMT_PNT,
	NNE_PRIM3D_FMT_PCT
} NNE_PRIM3D_FMT;

/* プリミティブ3D POINT フォーマット */
typedef enum {
	NNE_PRIM3D_POINT_FMT_P,
	NNE_PRIM3D_POINT_FMT_PC
} NNE_PRIM3D_POINT_FMT;

/* ライティングタイプ */
typedef enum {
	NNE_PRIM_LIGHT_DISABLE,				/* ライティング無し */
	NNE_PRIM_LIGHT_ENABLE,				/* ライティング有り */
	NNE_PRIM_LIGHT_SPECULAR				/* ライティング有り(スペキュラ付き) */
} NNE_PRIM_LIGHT;

/* ブレンディングタイプ */
typedef enum {
	NNE_PRIM_BLEND_ADD,					/* SA, ONE : C = As * Cs + Cd */
	NNE_PRIM_BLEND_BLEND,				/* SA, ISA : C = As * Cs + (1 - As) * Cd */
	NNE_PRIM_BLEND_PS2_CUSTOM,			/* PlayStation2 */
	NNE_PRIM_BLEND_PSP_CUSTOM = 3		/* PSP */
} NNE_PRIM_BLEND;

/* カリングタイプ */
typedef enum {
	NNE_PRIM_CULL_NONE,					/* カリング無し */
	NNE_PRIM_CULL_R,					/* 右回りのポリゴンをカリング */
	NNE_PRIM_CULL_L						/* 左回りのポリゴンをカリング */
} NNE_PRIM_CULL;

/* トライアングルタイプ */
typedef enum {
	NNE_PRIM_TRIANGLE_LIST,
	NNE_PRIM_TRIANGLE_STRIP
} NNE_PRIM_TRIANGLE;

/* ラインタイプ */
typedef enum {
	NNE_PRIM_LINE_LIST,
	NNE_PRIM_LINE_STRIP
} NNE_PRIM_LINE;

/* 2D、3D */
void nnSetPrimitiveBlend( NNE_PRIM_BLEND blend );
void nnSetPrimitiveTexNum( const NNS_TEXLIST *texlist, Sint32 num );
void nnSetPrimitiveTexState( NNE_PRIM_TEXBLEND blend, NNE_PRIM_TEXCOORD coord,
								NNE_PRIM_TEXWRAP uwrap, NNE_PRIM_TEXWRAP vwrap );

/* 2D */
void nnBeginDrawPrimitive2D( NNE_PRIM2D_FMT fmt, NNE_PRIM_ALPHABLEND blend );
void nnDrawPrimitive2D( NNE_PRIM_TRIANGLE type, const void *vtx, Sint32 count, Float pri );
void nnEndDrawPrimitive2D( void );

/* 3D */
void nnSetPrimitive3DMatrix( const NNS_MATRIX *mtx );
void nnSetPrimitive3DMaterial( const NNS_RGBA *diffuse, const NNS_RGB *ambient, Float specular );
void nnBeginDrawPrimitive3D( NNE_PRIM3D_FMT fmt, NNE_PRIM_ALPHABLEND blend, NNE_PRIM_LIGHT light, NNE_PRIM_CULL cull );
void nnDrawPrimitive3D( NNE_PRIM_TRIANGLE type, const void *vtx, Sint32 count );
void nnEndDrawPrimitive3D( void );
void nnChangePrimitive3DMatrix( const NNS_MATRIX *mtx );

/* 2D Line */
void nnBeginDrawPrimitiveLine2D( const NNS_RGBA *col, NNE_PRIM_ALPHABLEND blend );
void nnDrawPrimitiveLine2D( NNE_PRIM_LINE type, const void *vtx, Sint32 count, Float pri);
void nnEndDrawPrimitiveLine2D( void );

/* 3D Line */
void nnBeginDrawPrimitiveLine3D( const NNS_RGBA *col, NNE_PRIM_ALPHABLEND blend );
void nnDrawPrimitiveLine3D( NNE_PRIM_LINE type, const void *vtx, Sint32 count);
void nnEndDrawPrimitiveLine3D( void );

/* 2D Point */
void nnBeginDrawPrimitivePoint2D( NNE_PRIM2D_POINT_FMT fmt, const NNS_RGBA *col, NNE_PRIM_ALPHABLEND blend );
void nnDrawPrimitivePoint2D( const void *vtx, Sint32 count, Float pri );
void nnEndDrawPrimitivePoint2D( void );

/* 3D Point */
void nnBeginDrawPrimitivePoint3D( NNE_PRIM3D_POINT_FMT fmt, const NNS_RGBA *col, NNE_PRIM_ALPHABLEND blend );
void nnDrawPrimitivePoint3D( const void *vtx, Sint32 count );
void nnEndDrawPrimitivePoint3D( void );

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* PlayStation2 Draw primitive library */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nnldrawprimps2.h"
#endif

/* GAMECUBE Draw primitve library */
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nnldrawprimgc.h"
#endif

/* Xbox Draw primitive library */
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nnldrawprimdx.h"
#endif

/* PC Draw primitive library */
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
#include "nnldrawprimdx.h"
#endif

/* Open GL Draw primitive library */
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#include "nnldrawprimgl.h"
#endif

/* PSP Draw primitive library */
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#include "nnldrawprimpsp.h"
#endif

/* PS3 Draw primitive library */
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
#include "nnldrawprimps3.h"
#endif

/* DXG20 Draw primitive library */
#if ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#include "nnldrawprimdxg20.h"
#endif

#endif /* __NNLDRAWPRIM_H__ */

/* End of file */
