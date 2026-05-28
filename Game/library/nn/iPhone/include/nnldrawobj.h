/*---------------------------------------------------------------------------

    NN Draw object

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN Draw object Library
    File    : nnldrawobj.h
    Create  : 2002/05/10
    Modify  : 2003/04/11
    Modify  : 2003/06/05
    Modify  : 2003/08/07 nnDrawObject関数をインライン化
    Modify  : 2003/10/08 Xbox,PC共通化向けの修正
    Modify  : 2003/10/22 Xbox,PC共通化向けのNND_DRAW_???を追加
    Modify  : 2003/10/30 Measure系定義を追加
    Modify  : 2003/11/14 inline 関数の定義を static inline に変更
    Modify  : 2003/12/10 NND_DRAWOBJ_COLORNWEIGHT追加
    Modify  : 2003/12/18 NNS_OBJECT_MEASURE, nnCalcObjectMeasure()仕様変更
    Modify  : 2003/12/19 オブジェクト描画フラグの修正
    Modify  : 2003/12/19 Xbox版統合
    Modify  : 2004/01/09 nnDrawObjectInitialPose を追加
    Modify  : 2004/01/13 nnDrawObjectInitialPose をインライン化
    Modify  : 2004/01/14 NND_DRAWOBJ_IGNOREMATAMBI を廃止、
                         NND_DRAWOBJ_IGNOREMATSPECを追加
    Modify  : 2004/01/21 inline 関数の定義を static __inline に変更
    Modify  : 2004/01/28 CopyObject系を追加
    Modify  : 2004/02/24 共通頂点コンパイル系定義を追加
    Modify  : 2004/03/16 Xbox部分修正
    Modify  : 2004/04/28 nnDrawMultiObject を追加
    Modify  : 2004/04/30 ヘッダ統合
    Modify  : 2004/06/16 DX8,DX9版統合
    Modify  : 2004/06/24 nnlshaderdx.hを追加
    Modify  : 2004/07/08 NND_DRAWOBJ_MATCTRL_TEXOFFSET追加
    Modify  : 2004/07/13 nnDrawMultiObjectInitialPose()追加
    Modify  : 2004/08/05 NND_PLATFORM_XENON追加
    Modify  : 2004/08/12 OpenGL版統合
    Modify  : 2004/08/20 nnDrawMultiObjectInitialPoseBaseMatrixList()追加,
                         上記関数対応のためnnDrawMultiObjectInitialPoseLtd/Ext()仕様変更
    Modify  : 2004/09/15 nnCountSubObjectType() -> nnCountSubObject() 変更
    Modify  : 2004/11/08 エンベロープ計算関数追加
    Modify  : 2004/11/11 nnlshadergl.h追加
    Modify  : 2004/12/13 NND_DRAWOBJ_NORMAL,NND_DRAWOBJ_TANGENTSPACE追加
    Modify  : 2005/01/24 DRAWOBJフラグ整理、NND_DRAWOBJ_COLORNTEXTURE追加
    Modify  : 2005/01/27 nnDrawPliableObjectLtd、nnDrawPliableObjectExt
                         nnExitPliableObject関数追加
                         nnCalcPliableObject関数追加
    Modify  : 2005/02/15 NND_CALCPLIABLE_PS2_SINGLEBUFFER 追加
    Modify  : 2005/03/10 DT06版統合
    Modify  : 2005/03/10 nnDrawMultiPliableObject関数追加
    Modify  : 2005/03/25 LB版 NNF_DRAWOBJを64bit化
    Modify  : 2005/04/01 PSP版統合
    Modify  : 2005/04/19 NND_DRAWOBJ_SHADER_VERTEXBLEND追加
    Modify  : 2005/06/10 NND_DRAWOBJ_SHADER_USER_PROFILE追加
    Modify  : 2005/06/10 NND_DRAWOBJ_DISABLE_***MAP追加
    Modify  : 2005/06/13 nnCalcPliableObjectNodeStatusList()追加
    Modify  : 2005/06/16 NND_DRAWOBJ_MATCTRL_TEXLODBIAS追加
    Modify  : 2005/06/20 NND_CALCPLIABLE_USE_NODESTATUS,nnCalcPliableObjectNodeStatusList追加
    Modify  : 2006/03/15 XENON版, PLAYSTATION3版統合
    Modify  : 2006/05/15 ユーザーサンプラーDRAWOBJフラグ追加
    Modify  : 2006/05/23 ユーザープロファイルフラグ範囲拡大
    Modify  : 2006/07/07 NND_CALCPLIABLE_DX_PRINODEOBJCT追加
    Modify  : 2006/07/19 NND_DRAWOBJ_DISABLE_MAPMASK追加,NND_DRAWOBJ_DISABLE_MODULATEMAPのスペルミス修正
    Modify  : 2006/07/20 NND_COPYOBJ_INSTANCE_DX対応
    Modify  : 2006/09/13 描画フラグユーザープロファイルを255まで拡張
    Modify  : 2008/05/21 NND_PLATFORM_DXG20追加
    Modify  : 2008/10/29 NND_DRAWOBJ_DISABLE_DEPTH_WRITING, NND_DRAWOBJ_DISABLE_DEPTHTEST 追加
    Modify  : 2009/05/13 NND_PLATFORM_XENON削除
    Version : 1.20.20
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLDRAWOBJ_H__
#define __NNLDRAWOBJ_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Object status */
typedef Uint32	NNF_OBJECTSTATUS;
#define NND_OBJECTSTATUS_HIDE	((NNF_OBJECTSTATUS)1 <<  0)	/* No drawing */
#define NND_OBJECTSTATUS_INSIDE	((NNF_OBJECTSTATUS)1 <<  1)	/* Completely visible */

/* Draw Object Flag */
#if (NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_PS3 || NND_PLATFORM == NND_PLATFORM_DXG20)
typedef Uint64	NNF_DRAWOBJ;
#else
typedef Uint32	NNF_DRAWOBJ;
#endif
#define NND_DRAWOBJ_HIDE	((NNF_DRAWOBJ)NND_OBJECTSTATUS_HIDE)
#define NND_DRAWOBJ_INSIDE	((NNF_DRAWOBJ)NND_OBJECTSTATUS_INSIDE)

#if (NND_PLATFORM == NND_PLATFORM_DXG20)
#define NND_DRAWOBJ_DISABLE_DEPTH_WRITING	((NNF_DRAWOBJ)1 << 2)	/* デプスバッファへの書き込みを禁止する */
#define NND_DRAWOBJ_DISABLE_DEPTHTEST		((NNF_DRAWOBJ)1 << 3)	/* デプステストをしない */
#endif

/* Meshset clip mode */
#define NND_DRAWOBJ_MESHSETCLIP			((NNF_DRAWOBJ)1 <<  4)

/* Culling mode */
#define NND_DRAWOBJ_DOUBLESIDE			((NNF_DRAWOBJ)1 <<  5)
#define NND_DRAWOBJ_BACKSIDE			((NNF_DRAWOBJ)2 <<  5)
#define NND_DRAWOBJ_FRONTSIDE			((NNF_DRAWOBJ)3 <<  5)
#define NND_DRAWOBJ_CULLING_MASK		((NNF_DRAWOBJ)3 <<  5)

/* Lighting mode */
#define NND_DRAWOBJ_DISABLE_LIGHTING	((NNF_DRAWOBJ)1 << 7)
#define NND_DRAWOBJ_LIGHTING_MASK		((NNF_DRAWOBJ)1 << 7)

/* Additional draw */
#define NND_DRAWOBJ_NORM				((NNF_DRAWOBJ)1 << 8)	/* old define */
#define NND_DRAWOBJ_NORMAL				((NNF_DRAWOBJ)1 << 8)
#define NND_DRAWOBJ_TANGENTSPACE		((NNF_DRAWOBJ)1 << 9)
#define NND_DRAWOBJ_VECTORMASK			(NND_DRAWOBJ_NORMAL | NND_DRAWOBJ_TANGENTSPACE)

/* Ignore material element */
#define NND_DRAWOBJ_IGNOREMATSPEC		((NNF_DRAWOBJ)1 << 10)
#define NND_DRAWOBJ_IGNORETEXTURE		((NNF_DRAWOBJ)1 << 11)
#define NND_DRAWOBJ_IGNOREMATMASK		(NND_DRAWOBJ_IGNOREMATSPEC | NND_DRAWOBJ_IGNORETEXTURE)

/* Modified surface */
#define NND_DRAWOBJ_WIRE				((NNF_DRAWOBJ)1 << 12)
#define NND_DRAWOBJ_COLORNTEXTURE		((NNF_DRAWOBJ)2 << 12)	/* for PlayStation2, Lindberg */
#define NND_DRAWOBJ_COLORSTRIP			((NNF_DRAWOBJ)3 << 12)
#define NND_DRAWOBJ_COLORMESHSET		((NNF_DRAWOBJ)4 << 12)
#define NND_DRAWOBJ_COLORMATERIAL		((NNF_DRAWOBJ)5 << 12)
#define NND_DRAWOBJ_COLORNWEIGHT		((NNF_DRAWOBJ)6 << 12)
#define NND_DRAWOBJ_COLORSHADER			((NNF_DRAWOBJ)7 << 12)	/* for PlayStation2, Lindberg */
#define NND_DRAWOBJ_SURFACE_MASK		((NNF_DRAWOBJ)7 << 12)

/* For specified platform */
#if (NND_PLATFORM == NND_PLATFORM_GC)
#define NND_DRAWOBJ_VTXCOLOREMU			((NNF_DRAWOBJ)1 << 16)	/* for GAMECUBE */
#endif
#if (NND_PLATFORM == NND_PLATFORM_XB) || (NND_PLATFORM == NND_PLATFORM_DX8) || (NND_PLATFORM == NND_PLATFORM_DX9)
#define NND_DRAWOBJ_FIXEDSHADER			((NNF_DRAWOBJ)1 << 16)	/* for Xbox(PC) */
#define NND_DRAWOBJ_CUSTOMSHADER		((NNF_DRAWOBJ)1 << 17)	/* for Xbox(PC) */
#define NND_DRAWOBJ_FORCE_FIXEDSHADER	((NNF_DRAWOBJ)1 << 18)	/* for Xbox(PC) */
#define	NND_DRAWOBJ_SHADER_MASK			(NND_DRAWOBJ_FIXEDSHADER | NND_DRAWOBJ_CUSTOMSHADER | NND_DRAWOBJ_FORCE_FIXEDSHADER)

#define NND_DRAWOBJ_NOINDEX				((NNF_DRAWOBJ)1 << 19)	/* for Xbox(PC) */
#endif

#if (NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_PS3 || NND_PLATFORM == NND_PLATFORM_DXG20)
#define NND_DRAWOBJ_FRAGPARALIGHT1		((NNF_DRAWOBJ)1 << 16)	/* for Lindbergh */
#define NND_DRAWOBJ_FRAGPARALIGHT2		((NNF_DRAWOBJ)2 << 16)	/* for Lindbergh */
#define NND_DRAWOBJ_FRAGPARALIGHT3		((NNF_DRAWOBJ)3 << 16)	/* for Lindbergh */
#define NND_DRAWOBJ_FRAGPARALIGHT_MASK	((NNF_DRAWOBJ)3 << 16)
#define NND_DRAWOBJ_FRAGPOINTLIGHT1		((NNF_DRAWOBJ)1 << 18)	/* for Lindbergh */
#define NND_DRAWOBJ_FRAGPOINTLIGHT2		((NNF_DRAWOBJ)2 << 18)	/* for Lindbergh */
#define NND_DRAWOBJ_FRAGPOINTLIGHT3		((NNF_DRAWOBJ)3 << 18)	/* for Lindbergh */
#define NND_DRAWOBJ_FRAGPOINTLIGHT_MASK	((NNF_DRAWOBJ)3 << 18)
#define NND_DRAWOBJ_FRAGLIGHT_MASK		(NND_DRAWOBJ_FRAGPARALIGHT_MASK | \
										 NND_DRAWOBJ_FRAGPOINTLIGHT_MASK)
#define NND_DRAWOBJ_DEPTH_ONLY			((NNF_DRAWOBJ)1 << 31)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_ALPHATEST	((NNF_DRAWOBJ)1 << 32)	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_VERTEXBLEND	((NNF_DRAWOBJ)1 << 33)	/* for Lindbergh */

#define NND_DRAWOBJ_SHADER_USER_PROFILE( _n )	((NNF_DRAWOBJ)(( _n ) & 0xff) << 34)	/* for Lindbergh */ /* _n = 0, 1, 2 ... 255 */
#define NND_DRAWOBJ_SHADER_USER_PROFILE0		NND_DRAWOBJ_SHADER_USER_PROFILE( 0 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE1		NND_DRAWOBJ_SHADER_USER_PROFILE( 1 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE2		NND_DRAWOBJ_SHADER_USER_PROFILE( 2 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE3		NND_DRAWOBJ_SHADER_USER_PROFILE( 3 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE4		NND_DRAWOBJ_SHADER_USER_PROFILE( 4 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE5		NND_DRAWOBJ_SHADER_USER_PROFILE( 5 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE6		NND_DRAWOBJ_SHADER_USER_PROFILE( 6 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE7		NND_DRAWOBJ_SHADER_USER_PROFILE( 7 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE8		NND_DRAWOBJ_SHADER_USER_PROFILE( 8 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE9		NND_DRAWOBJ_SHADER_USER_PROFILE( 9 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE10		NND_DRAWOBJ_SHADER_USER_PROFILE( 10 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE11		NND_DRAWOBJ_SHADER_USER_PROFILE( 11 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE12		NND_DRAWOBJ_SHADER_USER_PROFILE( 12 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE13		NND_DRAWOBJ_SHADER_USER_PROFILE( 13 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE14		NND_DRAWOBJ_SHADER_USER_PROFILE( 14 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE15		NND_DRAWOBJ_SHADER_USER_PROFILE( 15 )	/* for Lindbergh */
#define NND_DRAWOBJ_SHADER_USER_PROFILE_MASK	( (NNF_DRAWOBJ)0xff << 34 )				/* for Lindbergh */

#define NNM_USER_PROFILE_DRAWOBJ( _drawobjflag )	((Uint32)((( _drawobjflag ) & NND_DRAWOBJ_SHADER_USER_PROFILE_MASK) >> 34))

#define NND_DRAWOBJ_DISABLE_NORMALMAP			((NNF_DRAWOBJ)1 << 42)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_SPECULARMAP			((NNF_DRAWOBJ)1 << 43)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_SHININESSMAP		((NNF_DRAWOBJ)1 << 44)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_DUALPARABOLOIDMAP	((NNF_DRAWOBJ)1 << 45)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_ENVMASKMAP			((NNF_DRAWOBJ)1 << 46)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_MODULATEMAP			((NNF_DRAWOBJ)1 << 47)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_ADDMAP				((NNF_DRAWOBJ)1 << 48)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_OPACITYMAP			((NNF_DRAWOBJ)1 << 49)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_USER1MAP			((NNF_DRAWOBJ)1 << 50)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_USER2MAP			((NNF_DRAWOBJ)1 << 51)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_USER3MAP			((NNF_DRAWOBJ)1 << 52)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_USER4MAP			((NNF_DRAWOBJ)1 << 53)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_USER5MAP			((NNF_DRAWOBJ)1 << 54)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_USER6MAP			((NNF_DRAWOBJ)1 << 55)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_USER7MAP			((NNF_DRAWOBJ)1 << 56)	/* for Lindbergh */
#define NND_DRAWOBJ_DISABLE_USER8MAP			((NNF_DRAWOBJ)1 << 57)	/* for Lindbergh */

#define NND_DRAWOBJ_DISABLE_MAPMASK				((NNF_DRAWOBJ)0xffff << 42)	/* for Lindbergh */
#endif

#if ( NND_PLATFORM == NND_PLATFORM_GLES11 )
#define NND_DRAWOBJ_DEPTH_ONLY			((NNF_DRAWOBJ)1 << 16)
#define NND_DRAWOBJ_DISABLE_ALPHATEST	((NNF_DRAWOBJ)1 << 17)
#endif


/* Material Control */
#define NND_DRAWOBJ_MATCTRL_DIFFUSE		((NNF_DRAWOBJ)1 << 20)
#define NND_DRAWOBJ_MATCTRL_AMBIENT		((NNF_DRAWOBJ)1 << 21)
#define NND_DRAWOBJ_MATCTRL_SPECULAR	((NNF_DRAWOBJ)1 << 22)
#define NND_DRAWOBJ_MATCTRL_ALPHA		((NNF_DRAWOBJ)1 << 23)
#define NND_DRAWOBJ_MATCTRL_ENVTEXMTX	((NNF_DRAWOBJ)1 << 24)
#define NND_DRAWOBJ_MATCTRL_BLEND		((NNF_DRAWOBJ)1 << 25)
#define NND_DRAWOBJ_MATCTRL_TEXOFFSET	((NNF_DRAWOBJ)1 << 28)
#if (NND_PLATFORM == NND_PLATFORM_PS2)
#define NND_DRAWOBJ_MATCTRL_GSALPHA		((NNF_DRAWOBJ)1 << 26)	/* for PlayStation2 */
#define NND_DRAWOBJ_MATCTRL_GSPRMODE	((NNF_DRAWOBJ)1 << 27)	/* for PlayStation2 */
#endif
#if (NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_PS3 || NND_PLATFORM == NND_PLATFORM_DXG20)
#define NND_DRAWOBJ_MATCTRL_TEXLODBIAS	((NNF_DRAWOBJ)1 << 26)	/* for Lindbergh */
#define NND_DRAWOBJ_MATCTRL_SHADOWMAP		((NNF_DRAWOBJ)1 << 29)	/* for Lindbergh */
#define NND_DRAWOBJ_MATCTRL_SHADOWMAP1		((NNF_DRAWOBJ)1 << 29)	/* for Lindbergh */
#define NND_DRAWOBJ_MATCTRL_SHADOWMAP2		((NNF_DRAWOBJ)2 << 29)	/* for Lindbergh */
#define NND_DRAWOBJ_MATCTRL_SHADOWMAP_MASK	((NNF_DRAWOBJ)3 << 29)
#define NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1		((NNF_DRAWOBJ)1 << 58)
#define NND_DRAWOBJ_MATCTRL_USERSAMPLER2D2		((NNF_DRAWOBJ)1 << 59)
#define NND_DRAWOBJ_MATCTRL_USERSAMPLER2D_MASK	(NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1 | NND_DRAWOBJ_MATCTRL_USERSAMPLER2D2)
#define NND_DRAWOBJ_MATCTRL_USERSAMPLER3D1		((NNF_DRAWOBJ)1 << 60)
#define NND_DRAWOBJ_MATCTRL_USERSAMPLER3D2		((NNF_DRAWOBJ)1 << 61)
#define NND_DRAWOBJ_MATCTRL_USERSAMPLER3D_MASK	(NND_DRAWOBJ_MATCTRL_USERSAMPLER3D1 | NND_DRAWOBJ_MATCTRL_USERSAMPLER3D2)
#define NND_DRAWOBJ_MATCTRL_USERSAMPLERCUBE1		((NNF_DRAWOBJ)1 << 62)
#define NND_DRAWOBJ_MATCTRL_USERSAMPLERCUBE2		((NNF_DRAWOBJ)1 << 63)
#define NND_DRAWOBJ_MATCTRL_USERSAMPLERCUBE_MASK	(NND_DRAWOBJ_MATCTRL_USERSAMPLERCUBE1 | NND_DRAWOBJ_MATCTRL_USERSAMPLERCUBE2)
#define NND_DRAWOBJ_MATCTRL_USERSAMPLER_MASK	(NND_DRAWOBJ_MATCTRL_USERSAMPLER2D_MASK | NND_DRAWOBJ_MATCTRL_USERSAMPLER3D_MASK | NND_DRAWOBJ_MATCTRL_USERSAMPLERCUBE_MASK)
#endif
#if (NND_PLATFORM == NND_PLATFORM_PS2)
#define NND_DRAWOBJ_MATCTRL_MASK		(NND_DRAWOBJ_MATCTRL_DIFFUSE\
										| NND_DRAWOBJ_MATCTRL_AMBIENT\
										| NND_DRAWOBJ_MATCTRL_SPECULAR\
										| NND_DRAWOBJ_MATCTRL_ALPHA\
										| NND_DRAWOBJ_MATCTRL_ENVTEXMTX\
										| NND_DRAWOBJ_MATCTRL_BLEND\
										| NND_DRAWOBJ_MATCTRL_GSALPHA\
										| NND_DRAWOBJ_MATCTRL_GSPRMODE\
										| NND_DRAWOBJ_MATCTRL_TEXOFFSET\
										 )
#elif (NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_PS3 || NND_PLATFORM == NND_PLATFORM_DXG20)
#define NND_DRAWOBJ_MATCTRL_MASK		(NND_DRAWOBJ_MATCTRL_DIFFUSE\
										| NND_DRAWOBJ_MATCTRL_AMBIENT\
										| NND_DRAWOBJ_MATCTRL_SPECULAR\
										| NND_DRAWOBJ_MATCTRL_ALPHA\
										| NND_DRAWOBJ_MATCTRL_ENVTEXMTX\
										| NND_DRAWOBJ_MATCTRL_BLEND\
										| NND_DRAWOBJ_MATCTRL_TEXOFFSET\
										| NND_DRAWOBJ_MATCTRL_SHADOWMAP_MASK\
										 )
#else
#define NND_DRAWOBJ_MATCTRL_MASK		(NND_DRAWOBJ_MATCTRL_DIFFUSE\
										| NND_DRAWOBJ_MATCTRL_AMBIENT\
										| NND_DRAWOBJ_MATCTRL_SPECULAR\
										| NND_DRAWOBJ_MATCTRL_ALPHA\
										| NND_DRAWOBJ_MATCTRL_ENVTEXMTX\
										| NND_DRAWOBJ_MATCTRL_BLEND\
										| NND_DRAWOBJ_MATCTRL_TEXOFFSET\
										 )
#endif

/* Measure */
typedef struct {
	Sint32	nObj;
	Sint32	nSubobj;
	Sint32	nNode;
	Sint32	nMtx;
	Sint32	nVtx;
	Sint32	nPrim;
	Sint32	nMeshset;
	Sint32	nMaterial;
	Sint32	nTex;
} NNS_OBJECT_MEASURE;

/* object copy flag */
typedef Uint32 NNF_COPYOBJ;
/* For object copy flag */
#define	NND_COPYOBJ_CACHEFLUSH					((Uint32)1 << 0)	/* for GC */
#define	NND_COPYOBJ_COMPILE_DISPLAYLIST_GC		((Uint32)1 << 1)	/* for GC  (No support) */
#define	NND_COPYOBJ_COMPILE_VERTEXBUFFER_PS2	((Uint32)1 << 1)	/* for PS2 (No support) */
#define	NND_COPYOBJ_INSTANCE_DX					((Uint32)1 << 2)	/* for PC */

/* Clip */
NNF_OBJECTSTATUS nnCheckObjectClip( const NNS_OBJECT *obj, const NNS_MATRIX *basemtx );
NNF_OBJECTSTATUS nnCheckObjectClipMotion( const NNS_OBJECT *obj, const NNS_MOTION *mot, Float frame, const NNS_MATRIX *basemtx );

/* Draw */
void nnDrawObjectLtd( const NNS_OBJECT *obj, const NNS_MATRIX *mtxpal, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );
void nnDrawObjectExt( const NNS_OBJECT *obj, const NNS_MATRIX *mtxpal, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
static __inline void nnDrawObject( const NNS_OBJECT *obj, const NNS_MATRIX *mtxpal, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag )
{
	if(flag & (~(NND_DRAWOBJ_INSIDE))){
		nnDrawObjectExt( obj, mtxpal, nodestatlist, subobjtype, flag );
	}
	else{
		nnDrawObjectLtd( obj, mtxpal, nodestatlist, subobjtype, flag );
	}
}
#elif ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_PS3 || NND_PLATFORM == NND_PLATFORM_GLES11 )
void nnDrawObject( const NNS_OBJECT *obj, const NNS_MATRIX *mtxpal, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );

#elif ( NND_PLATFORM == NND_PLATFORM_DXG20 )
void nnDrawObject( const NNS_OBJECT *obj, const NNS_MATRIX *mtxpal, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );
#else
static __inline void nnDrawObject( const NNS_OBJECT *obj, const NNS_MATRIX *mtxpal, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag )
{
	if(flag){
		nnDrawObjectExt( obj, mtxpal, nodestatlist, subobjtype, flag );
	}
	else{
		nnDrawObjectLtd( obj, mtxpal, nodestatlist, subobjtype, flag );
	}
}
#endif


/* Draw Initial Pose */
void nnDrawObjectInitialPoseLtd( const NNS_OBJECT *obj, const NNS_MATRIX *basemtx, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );
void nnDrawObjectInitialPoseExt( const NNS_OBJECT *obj, const NNS_MATRIX *basemtx, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
static __inline void nnDrawObjectInitialPose( const NNS_OBJECT *obj, const NNS_MATRIX *basemtx, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag )
{
	if(flag & (~(NND_DRAWOBJ_INSIDE))){
		nnDrawObjectInitialPoseExt( obj, basemtx, nodestatlist, subobjtype, flag );
	}
	else{
		nnDrawObjectInitialPoseLtd( obj, basemtx, nodestatlist, subobjtype, flag );
	}
}
#elif ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_PS3 || NND_PLATFORM == NND_PLATFORM_GLES11 )
void nnDrawObjectInitialPose( const NNS_OBJECT *obj, const NNS_MATRIX *basemtx, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );
#elif ( NND_PLATFORM == NND_PLATFORM_DXG20 )
void nnDrawObjectInitialPose( const NNS_OBJECT *obj, const NNS_MATRIX *basemtx, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );
#else
static __inline void nnDrawObjectInitialPose( const NNS_OBJECT *obj, const NNS_MATRIX *basemtx, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag )
{
	if(flag){
		nnDrawObjectInitialPoseExt( obj, basemtx, nodestatlist, subobjtype, flag );
	}
	else{
		nnDrawObjectInitialPoseLtd( obj, basemtx, nodestatlist, subobjtype, flag );
	}
}
#endif


/* Multi Draw */
void nnDrawMultiObjectLtd( const NNS_OBJECT *obj, const NNS_MATRIX **mtxpalptrlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num );
void nnDrawMultiObjectExt( const NNS_OBJECT *obj, const NNS_MATRIX **mtxpalptrlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num );
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
static __inline void nnDrawMultiObject( const NNS_OBJECT *obj, const NNS_MATRIX **mtxpalptrlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num )
{
	if(flag & (~(NND_DRAWOBJ_INSIDE))){
		nnDrawMultiObjectExt( obj, mtxpalptrlist, nodestatlistptrlist, subobjtype, flag, num );
	}
	else{
		nnDrawMultiObjectLtd( obj, mtxpalptrlist, nodestatlistptrlist, subobjtype, flag, num );
	}
}
#elif ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_PS3 || NND_PLATFORM == NND_PLATFORM_GLES11 )
void nnDrawMultiObject( const NNS_OBJECT *obj, const NNS_MATRIX **mtxpalptrlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num );
#elif ( NND_PLATFORM == NND_PLATFORM_DXG20 )
void nnDrawMultiObject( const NNS_OBJECT *obj, const NNS_MATRIX **mtxpalptrlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num );
#else
static __inline void nnDrawMultiObject( const NNS_OBJECT *obj, const NNS_MATRIX **mtxpalptrlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num )
{
	if( flag ){
		nnDrawMultiObjectExt( obj, mtxpalptrlist, nodestatlistptrlist, subobjtype, flag, num );
	}
	else{
		nnDrawMultiObjectLtd( obj, mtxpalptrlist, nodestatlistptrlist, subobjtype, flag, num );
	}
}
#endif

/* Multi Draw Initial Pose */
void nnDrawMultiObjectInitialPoseLtd( const NNS_OBJECT *obj, const void *basemtxptrlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num, NNE_BOOL basemtxlistsw );
void nnDrawMultiObjectInitialPoseExt( const NNS_OBJECT *obj, const void *basemtxptrlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num, NNE_BOOL basemtxlistsw );
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
static __inline void nnDrawMultiObjectInitialPose( const NNS_OBJECT *obj, const NNS_MATRIX **basemtxptrlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num )
{
	if( flag & (~(NND_DRAWOBJ_INSIDE)) ){
		nnDrawMultiObjectInitialPoseExt( obj, basemtxptrlist, nodestatlistptrlist, subobjtype, flag, num, NNE_FALSE );
	}
	else{
		nnDrawMultiObjectInitialPoseLtd( obj, basemtxptrlist, nodestatlistptrlist, subobjtype, flag, num, NNE_FALSE );
	}
}
static __inline void nnDrawMultiObjectInitialPoseBaseMatrixList( const NNS_OBJECT *obj, const NNS_MATRIX *basemtxlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num )
{
	if( flag & (~(NND_DRAWOBJ_INSIDE)) ){
		nnDrawMultiObjectInitialPoseExt( obj, basemtxlist, nodestatlistptrlist, subobjtype, flag, num, NNE_TRUE );
	}
	else{
		nnDrawMultiObjectInitialPoseLtd( obj, basemtxlist, nodestatlistptrlist, subobjtype, flag, num, NNE_TRUE );
	}
}
#elif ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_PS3 || NND_PLATFORM == NND_PLATFORM_GLES11 )
void nnDrawMultiObjectInitialPose( const NNS_OBJECT *obj, const NNS_MATRIX **basemtxptrlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num );
void nnDrawMultiObjectInitialPoseBaseMatrixList( const NNS_OBJECT *obj, const NNS_MATRIX *basemtxlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num );
#elif ( NND_PLATFORM == NND_PLATFORM_DXG20 )
void nnDrawMultiObjectInitialPose( const NNS_OBJECT *obj, const NNS_MATRIX **basemtxptrlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num );
#else
static __inline void nnDrawMultiObjectInitialPose( const NNS_OBJECT *obj, const NNS_MATRIX **basemtxptrlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num )
{
	if( flag ){
		nnDrawMultiObjectInitialPoseExt( obj, basemtxptrlist, nodestatlistptrlist, subobjtype, flag, num, NNE_FALSE );
	}
	else{
		nnDrawMultiObjectInitialPoseLtd( obj, basemtxptrlist, nodestatlistptrlist, subobjtype, flag, num, NNE_FALSE );
	}
}
static __inline void nnDrawMultiObjectInitialPoseBaseMatrixList( const NNS_OBJECT *obj, const NNS_MATRIX *basemtxlist, const NNF_NODESTATUS **nodestatlistptrlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag, Sint32 num )
{
	if( flag ){
		nnDrawMultiObjectInitialPoseExt( obj, basemtxlist, nodestatlistptrlist, subobjtype, flag, num, NNE_TRUE );
	}
	else{
		nnDrawMultiObjectInitialPoseLtd( obj, basemtxlist, nodestatlistptrlist, subobjtype, flag, num, NNE_TRUE );
	}
}
#endif

/* Measure */
Sint32 nnCountSubObject( const NNS_OBJECT *obj, NNF_SUBOBJTYPE subobjtype );

void nnSetUpObjectMeasure( NNS_OBJECT_MEASURE *objmeasure );
void nnCalcObjectMeasure( NNS_OBJECT_MEASURE *staticobjmeasure, NNS_OBJECT_MEASURE *drawobjmeasure, const NNS_OBJECT *obj, const NNS_MATRIX *mtxpal, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );

/* Copy */
Uint32	nnCalcObjectSize( const NNS_OBJECT *obj );
Uint32	nnCopyObject( NNS_OBJECT *dstobj, const NNS_OBJECT *srcobj, NNF_COPYOBJ flag );

/* Compile Common Vertices Object */
Uint32	nnCompileCommonVerticesObject( NNS_OBJECT *dstobj, const NNS_OBJECT *srcobj, Uint32 bufsize, NNF_COPYOBJ flag );


/*** Pliable Object ***/
typedef Uint32 NNF_CALCPLIABLE;

#define	NND_CALCPLIABLE_PLIABILITY_ALL		(Uint32) ( 1 << 0 )
#define	NND_CALCPLIABLE_GL_BINDBUFFER		(Uint32) ( 1 << 1 )
#define	NND_CALCPLIABLE_PS3_BINDBUFFER		(Uint32) ( 1 << 1 )
#define	NND_CALCPLIABLE_PS2_SINGLEBUFFER	(Uint32) ( 1 << 2 )
#define	NND_CALCPLIABLE_USE_NODESTATUS		(Uint32) ( 1 << 3 )
#define	NND_CALCPLIABLE_DX_OBJECTBUFFER		(Uint32) ( 1 << 4 )
#define	NND_CALCPLIABLE_DX_PRINODEOBJCT		(Uint32) ( 1 << 5 )
#define	NND_CALCPLIABLE_DX_NOINDEX			(Uint32) ( 1 << 6 )

typedef struct {
	NNF_CALCPLIABLE flag;
	const NNS_OBJECT *pObject;
	Uint32 Size;
	void *pBuffer;
	Uint32 *pIdx;
} NNS_PLIABLEOBJ;

Uint32 nnCalcPliableObjectBufferSize(
	const NNS_OBJECT *obj,
	NNF_CALCPLIABLE flag
);

Uint32 nnInitPliableObject(
	NNS_PLIABLEOBJ *pobj,
	const NNS_OBJECT *obj,
	NNF_CALCPLIABLE flag
);

void nnExitPliableObject( NNS_PLIABLEOBJ *pobj );

void nnCalcPliableObject(
	NNS_PLIABLEOBJ *pobj,
	const NNS_MATRIX *mtxpal,
	NNF_CALCPLIABLE flag
);

void nnCalcPliableObjectNodeStatusList(
	NNS_PLIABLEOBJ *pobj,
	const NNS_MATRIX *mtxpal,
	const NNF_NODESTATUS *nodestatlist,
	NNF_CALCPLIABLE flag
);

#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_PS3 || NND_PLATFORM == NND_PLATFORM_GLES11 )
void nnDrawPliableObject(
	const NNS_PLIABLEOBJ *pobj,
	const NNS_MATRIX *drawmtx,
	const NNF_NODESTATUS *nodestatlist,
	NNF_SUBOBJTYPE subobjtype,
	NNF_DRAWOBJ flag
);
#elif ( NND_PLATFORM == NND_PLATFORM_DXG20 )
void nnDrawPliableObject( const NNS_PLIABLEOBJ *pobj, const NNS_MATRIX *basemtx, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );
#else
void nnDrawPliableObjectLtd( const NNS_PLIABLEOBJ *pobj, const NNS_MATRIX *drawmtx, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );
void nnDrawPliableObjectExt( const NNS_PLIABLEOBJ *pobj, const NNS_MATRIX *drawmtx, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
static __inline void nnDrawPliableObject( const NNS_PLIABLEOBJ *pobj, const NNS_MATRIX *drawmtx, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag )
{
	if( flag & (~(NND_DRAWOBJ_INSIDE)) ){
		nnDrawPliableObjectExt( pobj, drawmtx, nodestatlist, subobjtype, flag );
	}
	else{
		nnDrawPliableObjectLtd( pobj, drawmtx, nodestatlist, subobjtype, flag );
	}
}
#elif ( NND_PLATFORM == NND_PLATFORM_DX9 )
void nnDrawPliablePriNodeObjectLtd( const NNS_PLIABLEOBJ *pobj, const NNS_MATRIX *drawmtx, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );
void nnDrawPliablePriNodeObjectExt( const NNS_PLIABLEOBJ *pobj, const NNS_MATRIX *drawmtx, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );
void nnDrawPriNodeObjectLtd( const NNS_OBJECT *obj, const NNS_MATRIX *mtxpal, const NNF_NODESTATUS *nodestatlist, NNF_MATSETDESCTYPE matsetdesctype, NNF_DRAWOBJ flag );
void nnDrawPriNodeObjectExt( const NNS_OBJECT *obj, const NNS_MATRIX *mtxpal, const NNF_NODESTATUS *nodestatlist, NNF_MATSETDESCTYPE matsetdesctype, NNF_DRAWOBJ flag );

static __inline void nnDrawPliableObject( const NNS_PLIABLEOBJ *pobj, const NNS_MATRIX *drawmtx, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag )
{
	if( flag ){
		if( pobj->flag & NND_CALCPLIABLE_DX_PRINODEOBJCT ){
			if( pobj->flag & NND_CALCPLIABLE_DX_NOINDEX )
				nnDrawPriNodeObjectExt( pobj->pObject, drawmtx, nodestatlist, subobjtype, flag );
			else
				nnDrawPliablePriNodeObjectExt( pobj, drawmtx, nodestatlist, subobjtype, flag );
		}else{
			if( pobj->flag & NND_CALCPLIABLE_DX_NOINDEX )
				nnDrawObjectExt( pobj->pObject, drawmtx, nodestatlist, subobjtype, flag );
			else
				nnDrawPliableObjectExt( pobj, drawmtx, nodestatlist, subobjtype, flag );
		}
	}
	else{
		if( pobj->flag & NND_CALCPLIABLE_DX_PRINODEOBJCT ){
			if( pobj->flag & NND_CALCPLIABLE_DX_NOINDEX )
				nnDrawPriNodeObjectLtd( pobj->pObject, drawmtx, nodestatlist, subobjtype, flag );
			else
				nnDrawPliablePriNodeObjectLtd( pobj, drawmtx, nodestatlist, subobjtype, flag );
		}else{
			if( pobj->flag & NND_CALCPLIABLE_DX_NOINDEX )
				nnDrawObjectLtd( pobj->pObject, drawmtx, nodestatlist, subobjtype, flag );
			else
				nnDrawPliableObjectLtd( pobj, drawmtx, nodestatlist, subobjtype, flag );
		}
	}
}
#else
static __inline void nnDrawPliableObject( const NNS_PLIABLEOBJ *pobj, const NNS_MATRIX *drawmtx, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag )
{
	if( flag ){
		nnDrawPliableObjectExt( pobj, drawmtx, nodestatlist, subobjtype, flag );
	}
	else{
		nnDrawPliableObjectLtd( pobj, drawmtx, nodestatlist, subobjtype, flag );
	}
}
#endif
#endif

void nnDrawMultiPliableObject(
	const NNS_PLIABLEOBJ **pobjptrlist,
	const NNS_MATRIX **drawmtxptrlist,
	const NNF_NODESTATUS **nodestatlistptrlist,
	NNF_SUBOBJTYPE subobjtype,
	NNF_DRAWOBJ flag,
	Sint32 num
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* PlayStation2 Draw object library */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nnldrawobjps2.h"
#endif

/* GAMECUBE Draw object library */
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nnldrawobjgc.h"
#endif

/* Xbox Draw object library */
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nnldrawobjdx.h"
#include "nnlshaderdx.h"
#endif

/* PC Draw object library */
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
#include "nnldrawobjdx.h"
#include "nnlshaderdx.h"
#endif

/* OpenGL Draw object library */
#if ( NND_PLATFORM == NND_PLATFORM_GL )
#include "nnldrawobjgl.h"
#include "nnlshadergl.h"
#endif

/* PSP Draw object library */
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#include "nnldrawobjpsp.h"
#endif

/* PS3 Draw object library */
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
#include "nnldrawobjps3.h"
#include "nnlshaderps3.h"
#endif

/* DXG20 Draw object library */
#if (NND_PLATFORM == NND_PLATFORM_DXG20)
#include "nnldrawobjdxg20.h"
#include "nnlshaderdxg20.h"
#endif

/* OpenGL ES 1.1 Draw object library */
#if ( NND_PLATFORM == NND_PLATFORM_GLES11 )
#include "nnldrawobjgl.h"
#endif

#endif /* __NNLDRAWOBJ_H__ */

/* End of file */
