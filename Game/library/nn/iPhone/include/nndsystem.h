/*---------------------------------------------------------------------------

    NN dev System header

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN System Library
    File    : nndsystem.h
    Create  : 2002/05/10
    Modify  : 2003/06/13 クリッピング関連の関数でスケール対応、整理
    Modify  : 2003/08/15 nndsystem～.hから共通部分をnndsystem.hとして新規作成
    Modify  : 2003/09/09 NNF_CLIP追加, クリップ関数戻り値をNNF_CLIPに変更
    Modify  : 2003/09/12 CLIP関連の公開部分をnnlsystem.hに移動
    Modify  : 2003/10/08 Xbox,PC共通化向けの修正
    Modify  : 2004/06/16 DX8,DX9版統合
    Modify  : 2004/08/05 NND_PLATFORM_XENON追加
    Modify  : 2005/03/14 NND_PLATFORM_PSP追加
    Modify  : 2008/05/21 NND_PLATFORM_DXG20追加
    Modify  : 2009/05/13 NND_PLATFORM_XENON削除
    Version : 1.16.21
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNDSYSTEM_H__
#define __NNDSYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Clip status (NND_NODESTATUS_ | NND_OBJECTSTATUS_ と値の関連有り) */
/* nnlysytem.h 参照 */
#define NND_CLIP_NEAR		(1<<8)
#define NND_CLIP_FAR		(1<<9)
#define NND_CLIP_RIGHT		(1<<12)
#define NND_CLIP_LEFT		(1<<13)
#define NND_CLIP_TOP		(1<<14)
#define NND_CLIP_BOTTOM		(1<<15)
#define NND_CLIP_GS_RIGHT	(1<<16)
#define NND_CLIP_GS_LEFT	(1<<17)
#define NND_CLIP_GS_TOP		(1<<18)
#define NND_CLIP_GS_BOTTOM	(1<<19)
#define NND_CLIP_GS_MASK	(NND_CLIP_GS_RIGHT | NND_CLIP_GS_LEFT |\
							 NND_CLIP_GS_TOP | NND_CLIP_GS_BOTTOM)


typedef struct {
	Float	xad,yad;		/* aspect * dist		*/
	Float   cx,cy;			/* screen center		*/
	Float	ooxad,ooyad;	/* 1.0 / aspect * dist	*/
	Float   dist;			/* screen distance		*/
	Float	ax,ay;			/* aspect				*/
	Float	aspect;			/* aspect ay/ax			*/
	Float   w,h;			/* screen size			*/
} NNS_SCREEN;

typedef struct {
	Float		f_clip;			/* far clip			*/
	Float		n_clip;			/* near clip		*/
	Float		x1,x0,y1,y0;	/* screen clip		*/
} NNS_CLIP;


extern const NNS_MATRIX nngUnitMatrix;
extern NNS_MATRIX44 nngProjectionMatrix;
extern NNE_PROJECTION_TYPE nngProjectionType;
extern NNS_SCREEN nngScreen;
extern NNS_CLIP nngClip2d;
extern NNS_CLIP nngClip3d;

/* Clip API */
void nnSetClipPlane( void );

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* PlayStation2 System Library */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nndsystemps2.h"
#endif

/* GAMECUBE System Library */
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nndsystemgc.h"
#endif

/* Xbox System Library */
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nndsystemdx.h"
#endif

/* PC System Library */
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9 )
#include "nndsystemdx.h"
#endif

/* PSP System Library */
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#include "nndsystempsp.h"
#endif

/* DXG20 System Library */
#if ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#include "nndsystemdxg20.h"
#endif

#endif /* __NNDSYSTEM_H__ */

/* End of file */
