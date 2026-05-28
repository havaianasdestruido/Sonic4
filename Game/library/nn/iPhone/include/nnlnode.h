/*---------------------------------------------------------------------------

    NN Node

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN Node Library
    File    : nnlnode.h
    Create  : 2002/05/10
    Modify  : 2003/05/14
    Modify  : 2003/06/09  nnDrawCircumsphereTRSList()追加
    Modify  : 2003/06/13  ノードステータス設定フラグを追加
    Modify  : 2003/06/25  nnCalcNodeMatrixTRSList()を追加
    Modify  : 2003/09/02  nnCalcMatrixPaletteLinkMotion()を追加
    Modify  : 2003/09/03  NNS_TRS メンバの型をVECTORからVECTORFASTに変更
    Modify  : 2003/10/02  ノードユーザデータモーションコールバック用関数追加
    Modify  : 2004/01/09  nnCalcNodeStatusListInitialPose関数追加
    Modify  : 2004/01/29  ノードツリー他関数追加
    Modify  : 2004/02/10  nnCalcNodeHideMotion関数追加
    Modify  : 2004/06/10  nnDrawClipBox, nnClipSphere関数追加
                          nnDrawCircumsphere***をnnDrawClipBound***に改名
    Modify  : 2004/06/11  NND_SETNODESTATUS_CLIP_SPHERE, NND_DRAWCS_SPHERE を追加
    Modify  : 2004/06/14  nnSetCircumsphereColor を nnSetClipBoundColor に改名
    Modify  : 2004/09/30  nnBlendMotionNode を追加
    Modify  : 2005/03/17  nnDrawGridPlane()追加
    Modify  : 2008/11/12  PSP:プライオリティノード関連追加
    Version : 1.18.30
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLNODE_H__
#define __NNLNODE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Node status */
typedef Uint32	NNF_NODESTATUS;
#define	NND_NODESTATUS_NONE				((Uint32)0 <<  0)	/* No status */
#define	NND_NODESTATUS_HIDE				((Uint32)1 <<  0)	/* No drawing */
#define	NND_NODESTATUS_INSIDE			((Uint32)1 <<  1)	/* Completely visible */
#define	NND_NODESTATUS_CROSSNEAR		((Uint32)1 <<  2)	/* Straddled near clipping plane */
#define	NND_NODESTATUS_CROSSFAR			((Uint32)1 <<  3)	/* Straddled far clipping plane */
#define	NND_NODESTATUS_OUTSIDE			((Uint32)1 <<  4)	/* Completely unvisible */
#define NND_NODESTATUS_PS2_GSINSIDE		((Uint32)1 <<  5)	/* Completely insides of gs window coordinates */
#define NND_NODESTATUS_CLIP_MASK		(NND_NODESTATUS_INSIDE    |\
										 NND_NODESTATUS_CROSSNEAR | NND_NODESTATUS_CROSSFAR |\
										 NND_NODESTATUS_OUTSIDE   | NND_NODESTATUS_PS2_GSINSIDE)
#define	NND_NODESTATUS_WIRE				((Uint32)1 << 10)	/* No support */

/* Set node status flag */
typedef Uint32	NNF_SETNODESTATUS;
#define NND_SETNODESTATUS_CLIP_HIDE				((Uint32)1 << 0)
#define NND_SETNODESTATUS_CLIP_WIRE				((Uint32)1 << 1)
#define NND_SETNODESTATUS_CLIP_BRANCH			((Uint32)1 << 3)
#define NND_SETNODESTATUS_CLIP_SCALE			((Uint32)1 << 4)
#define NND_SETNODESTATUS_CLIP_SPHERE			((Uint32)1 << 5)
#define NND_SETNODESTATUS_CLIP_MASK	( \
	NND_SETNODESTATUS_CLIP_HIDE | NND_SETNODESTATUS_CLIP_WIRE | \
	NND_SETNODESTATUS_CLIP_BRANCH | NND_SETNODESTATUS_CLIP_SCALE | \
	NND_SETNODESTATUS_CLIP_SPHERE )
/* Inherit hide status flag */
#define NND_SETNODESTATUS_HIDE					((Uint32)1 << 2)

/* DrawCircumsphere flag */
typedef Uint32	NNF_DRAWCS;
#define NND_DRAWCS_OBJECT		((Uint32)1 <<  8)
#define NND_DRAWCS_NODE			((Uint32)1 <<  9)
#define NND_DRAWCS_MESHSET		((Uint32)1 << 10)
#define NND_DRAWCS_SPHERE		((Uint32)1 << 15)
#define NND_DRAWCS_HIDE			((Uint32)1 << 16)
/* DrawCircumsphere color number */
typedef enum {
	NNE_CIRCUM_COL_NONE = 0,
	NNE_CIRCUM_COL_HIDE,
	NNE_CIRCUM_COL_CLIPHIDE,
	NNE_CIRCUM_COL_INSIDE,
	NNE_CIRCUM_COL_GSINSIDE,
	NNE_CIRCUM_COL_CROSSNEAR,
	NNE_CIRCUM_COL_ERR
} NNE_CIRCUM_COL;

/* DrawSIIKBone flag */
typedef Uint32	NNF_DRAWSIIKBONE;
#define NND_DRAWSIIKBONE_WIRE	((Uint32)1 <<  0)
#define NND_DRAWSIIKBONE_POLY	((Uint32)1 <<  1)

/* DrawNodeTree flag */
typedef Uint32	NNF_DRAWNODETREE;
#define NND_DRAWNODETREE_WIRE		((Uint32)1 <<  0)
#define NND_DRAWNODETREE_POLY		((Uint32)1 <<  1)
#define NND_DRAWNODETREE_ALLNODE	((Uint32)1 <<  2)
#define NND_DRAWNODETREE_DRAWNODE	((Uint32)1 <<  3)
#define NND_DRAWNODETREE_STRATPOINT	((Uint32)1 <<  4)
#define NND_DRAWNODETREE_DRAWTYPEMASK	((Uint32)0x00000001)

/* Node TRS Motion */
typedef struct {
	NNS_VECTORFAST		Translation;
	NNS_QUATERNION		Rotation;
	NNS_VECTORFAST		Scaling;
} NNS_TRS;

/* Motion blend mode */
typedef enum {
	NNE_MOTIONBLEND_REPLACE_ALL,
	NNE_MOTIONBLEND_ADD_TRANSLATION,
	NNE_MOTIONBLEND_ADD_ALL
} NNE_MOTIONBLEND;

/* Node user data motion callback */
typedef struct {
	Sint32				iNode;
	Float				Frame;
	union {
		Uint32			IValue;
		Float			FValue;
	};
	const NNS_MOTION	*pMotion;
	Sint32				iSubmot;
	NNF_SUBMOTION_TYPE	fSubmotType;
	NNF_SMOTIPTYPE		fSubmotIPType;
	const NNS_OBJECT	*pObject;
} NNS_NODEUSRMOT_CALLBACK_VAL;

typedef void ( *NNS_NODEUSRMOT_CALLBACK_FUNC )
							( NNS_NODEUSRMOT_CALLBACK_VAL *val );

/* Function Prototypes */
void nnCalcMatrixPaletteMultiplyMatrix(NNS_MATRIX* dstpal, const NNS_MATRIX* src, const NNS_MATRIX* srcpal, Sint32 num);
void nnCalcMatrixPalette(NNS_MATRIX *mtxpal, NNF_NODESTATUS *nodestatlist, const NNS_OBJECT *obj, const NNS_MATRIX *basemtx, NNS_MATRIXSTACK *mstk, NNF_SETNODESTATUS flag);
void nnCalcMatrixPaletteMotion(NNS_MATRIX *mtxpal, NNF_NODESTATUS *nodestatlist, const NNS_OBJECT *obj, const NNS_MOTION *mot, Float frame, const NNS_MATRIX *basemtx, NNS_MATRIXSTACK *mstk, NNF_SETNODESTATUS flag);

void nnSetUpNodeStatusList(NNF_NODESTATUS *nodestatlist, Sint32 num, NNF_NODESTATUS flag);
void nnCalcNodeStatusListMatrixPalette(NNF_NODESTATUS *nodestatlist, const NNS_MATRIX *mtxpal, const NNS_OBJECT *obj, NNF_SETNODESTATUS flag);
void nnCalcNodeStatusListMatrixList( NNF_NODESTATUS *nodestatlist, const NNS_OBJECT *obj, const NNS_MATRIX *mtxlist, const NNS_MATRIX *basemtx, NNF_SETNODESTATUS flag );
void nnCalcNodeStatusListInitialPose( NNF_NODESTATUS *nodestatlist, const NNS_OBJECT *obj, const NNS_MATRIX *basemtx, NNF_SETNODESTATUS flag );

void nnCalcTRS( NNS_TRS *trs, const NNS_OBJECT *obj, Sint32 nodeidx );
void nnCalcTRSList( NNS_TRS *trslist, const NNS_OBJECT *obj );
void nnCalcTRSListMotion( NNS_TRS *trslist, const NNS_OBJECT *obj, const NNS_MOTION *mot, Float frame );
void nnCalcTRSMotion( NNS_TRS *trs, const NNS_OBJECT *obj, Sint32 nodeidx, const NNS_MOTION *mot, Float frame );
void nnLinkMotion( NNS_TRS *dstpose, const NNS_TRS *pose0, const NNS_TRS *pose1, Sint32 nnode, Float ratio );
void nnCalcMatrixPaletteLinkMotion( NNS_MATRIX *mtxpal, NNF_NODESTATUS *nodestatlist, const NNS_OBJECT *obj, const NNS_MOTION *mot0, Float frame0, const NNS_MOTION *mot1, Float frame1, Float ratio, const NNS_MATRIX *basemtx, NNS_MATRIXSTACK *mstk, NNF_SETNODESTATUS flag );
void nnCalcMatrixPaletteTRSList( NNS_MATRIX *mtxpal, NNF_NODESTATUS *nodestatlist, const NNS_OBJECT *obj,
								const NNS_TRS *trslist, const NNS_MATRIX *basemtx, NNS_MATRIXSTACK *mstk, NNF_SETNODESTATUS flag);
void nnBlendMotion( NNS_TRS *dstpose, const NNS_TRS *srcpose, const NNS_OBJECT *obj,
					const NNS_MOTION *mot, Float frame, NNE_MOTIONBLEND blendmode );
void nnBlendMotionNode( NNS_TRS *dsttrs, const NNS_TRS *srctrs, const NNS_OBJECT *obj,
					Sint32 inode, const NNS_MOTION *mot, Float frame, NNE_MOTIONBLEND blendmode );
void nnCalcNodeMatrixTRSList( NNS_MATRIX *mtx, const NNS_OBJECT *obj, Sint32 nodeidx, const NNS_TRS *trslist, const NNS_MATRIX *basemtx );

void nnDrawCircumsphere(const NNS_OBJECT *obj, const NNS_MATRIX *basemtx, NNS_MATRIXSTACK *mstk, NNF_DRAWCS flag);
void nnDrawCircumsphereMotion(const NNS_OBJECT *obj, const NNS_MOTION *mot, Float frame, const NNS_MATRIX *basemtx, NNS_MATRIXSTACK *mstk, NNF_DRAWCS flag);
void nnDrawCircumsphereTRSList(const NNS_OBJECT *obj, const NNS_TRS *trslist, const NNS_MATRIX *basemtx, NNS_MATRIXSTACK *mstk, NNF_DRAWCS flag);
void nnSetCircumsphereColor( NNF_DRAWCS dstflag, NNE_CIRCUM_COL colnum, const NNS_RGBA *col );

Sint32 nnGetNodeIndex( const NNS_NODENAMELIST *pNodeNameList, const char *NodeName);
const char *nnGetNodeName( const NNS_NODENAMELIST *pNodeNameList, Sint32 NodeIndex);

void nnCalcMatrixList( NNS_MATRIX *mtxlist, const NNS_OBJECT *obj, const NNS_MATRIX *basemtx );
void nnCalcMatrixPaletteMatrixList( NNS_MATRIX *mtxpal, const NNS_OBJECT *obj, const NNS_MATRIX *mtxlist, const NNS_MATRIX *basemtx );
void nnCalcMatrixListMultiplyMatrix( NNS_MATRIX *dstlist, const NNS_MATRIX *src, const NNS_MATRIX *srclist, Sint32 num );
void nnCalcMatrixListMotion( NNS_MATRIX *mtxlist, const NNS_OBJECT *obj, const NNS_MOTION *mot, Float frame, const NNS_MATRIX *basemtx );
void nnCalcMatrixListTRSList( NNS_MATRIX *mtxlist, const NNS_OBJECT *obj, const NNS_TRS *trslist, const NNS_MATRIX *basemtx );
void nnCalcNodeMatrix( NNS_MATRIX *mtx, const NNS_OBJECT *obj, Sint32 nodeidx, const NNS_MATRIX *basemtx );
void nnCalcNodeMatrixMotion( NNS_MATRIX *mtx, const NNS_OBJECT *obj, Sint32 nodeidx, const NNS_MOTION *mot, Float frame, const NNS_MATRIX *basemtx );

void nnSetBoneColor(NNS_RGBA *pDiff, NNS_RGB *pAmb, NNS_RGBA *pWire);
void nnSetEffectorColor(NNS_RGBA *pXcol, NNS_RGBA *pYcol, NNS_RGBA *pZcol);
void nnDrawSIIKBone( const NNS_OBJECT *obj, const NNS_MATRIX *basemtx, const NNS_MATRIX *mtxlist, NNF_DRAWSIIKBONE flag );
void nnDrawNodeTree( const NNS_OBJECT *obj, const NNS_MATRIX *basemtx, const NNS_MATRIX *mtxlist, NNF_DRAWNODETREE flag );
Uint32 nnCalcGridBufferSize(Sint32 Xnum, Sint32 Znum);
void nnInitGrid(NNS_VECTOR *pBuf, Sint32 Xnum, Sint32 Znum);
void nnDrawGrid(NNS_VECTOR *p, Float length, const NNS_MATRIX *mtx);
void nnDrawGridPlane(Sint32 Xnum, Sint32 Znum, Float length, const NNS_MATRIX *mtx, NNS_RGBA *pcolor);
void nnDrawAxis(NNS_VECTOR *p, Float length, const NNS_MATRIX *mtx);

void nnDrawClipBox( const NNS_VECTOR *center, Float sx, Float sy, Float sz, const NNS_MATRIX *mtx );
void nnDrawClipSphere( const NNS_VECTOR *center, Float radius, const NNS_MATRIX *mtx );
void nnDrawClipBound( const NNS_OBJECT *obj, const NNS_MATRIX *basemtx, NNS_MATRIXSTACK *mstk, NNF_DRAWCS flag);
void nnDrawClipBoundMotion( const NNS_OBJECT *obj, const NNS_MOTION *mot, Float frame, const NNS_MATRIX *basemtx, NNS_MATRIXSTACK *mstk, NNF_DRAWCS flag);
void nnDrawClipBoundTRSList( const NNS_OBJECT *obj, const NNS_TRS *trslist, const NNS_MATRIX *basemtx, NNS_MATRIXSTACK *mstk, NNF_DRAWCS flag);
void nnSetClipBoundColor( NNF_DRAWCS dstflag, NNE_CIRCUM_COL colnum, const NNS_RGBA *col );

void nnSetNodeUserMotionCallback( NNS_NODEUSRMOT_CALLBACK_FUNC func );
NNS_NODEUSRMOT_CALLBACK_FUNC nnGetNodeUserMotionCallback( void );
void nnSetNodeUserMotionTriggerTime( Float t );

void nnCalcNodeHideMotion( NNF_NODESTATUS *nodestatlist, const NNS_MOTION *mot, Float frame );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nnlnodeps2.h"
#endif

#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
#include "nnlnodeex.h"
#endif

#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#include "nnlnodepsp.h"
#endif

#endif /* __NNLNODE_H__ */

/* End of file */
