/*---------------------------------------------------------------------------

    NN dev Node

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN dev Node Library
    File    : nndnode.h
    Create  : 2002/08/20
    Modify  : 2003/04/22  SIIKに対応
    Modify  : 2003/04/25  エフェクタの位置の計算方法修正対応
    Modify  : 2003/05/09  ボーン描画用関数追加
    Modify  : 2003/06/06  ライブラリ最適化用関数追加
    Modify  : 2003/06/09  nnDrawCircumsphere～Node()引数削減、nnDrawCircumsphereTRSListNode()追加
    Modify  : 2003/06/13  NODESTATUS処理内部関数を整理
    Modify  : 2003/06/25  nnCalcNodeMatrixTRSListNodeを追加
    Modify  : 2003/07/23  nnCalc2BoneSIIK, nnCalc1BoneSIIK の引数を変更
    Modify  : 2003/09/02  nnCalcMatrixPaletteLinkMotionNodeを追加
    Modify  : 2003/09/10  nnCalcRotateMatrixFast若干修正
    Modify  : 2003/09/25  nnCalcMotionUserDataを追加
    Modify  : 2003/12/18  nnNormalizeColumnを追加
    Modify  : 2003/01/19  nnMakeNodeTreeMatrixを追加
    Modify  : 2004/02/09  nnCalcMotionNodeHideを追加
    Modify  : 2004/02/10  nnCalcNodeMotionCoreの引数変更
    Modify  : 2004/04/21 NND_VECTORFASTTYPEマクロ対応、nnmSetUpVectorFastマクロをインライン関数化
    Modify  : 2004/06/11  nnDrawClipBoxCore(), nnCalcClipBoxNode()追加
    Modify  : 2004/06/16 DX8,DX9版統合
    Modify  : 2004/09/13 モーション補間のNNS_VECTORFAST*をNNS_VECTOR*に変更
    Modify  : 2005/01/26 nnCalcMotionUserData()関数仕様変更
    Modify  : 2008/04/08 nnCallbackMotionUserData()を追加
    Modify  : 2008/04/28 nnCallbackMotionUserData()がハングするのを修正
    Modify  : 2009/04/23 XSIIK対応用関数追加
    Version : 1.20.14
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNDNODE_H__
#define __NNDNODE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* モーション適用状態を表すフラグ */
#define	NND_MOTIONFLAG_NONE	(0)
#define NND_MOTIONFLAG_XYZ	(1)
#define NND_MOTIONFLAG_QUAT	(2)

#if 1
static __inline void nnmSetUpVectorFast( NNS_VECTORFAST *dst, Float x, Float y, Float z)
{
	dst->x = x;
	dst->y = y;
	dst->z = z;
#if ( NND_VECTORFASTTYPE == NND_VECTORFASTTYPE_XYZW )
	dst->w = 1.f;
#endif
}
#else
//高速化のためのマクロ
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#define nnmSetUpVectorFast( dst, v0, v1, v2) \
{\
	(dst)->x = v0;\
	(dst)->y = v1;\
	(dst)->z = v2;\
	(dst)->w = 1.0f;\
}
#else
#define nnmSetUpVectorFast( dst, v0, v1, v2) \
{\
	(dst)->x = v0;\
	(dst)->y = v1;\
	(dst)->z = v2;\
}
#endif
#endif


#if ( NND_PLATFORM == NND_PLATFORM_GC )
#define nnmRotateMatrixFast( ang, ma, mb )\
{\
	if(ang != 0){\
		nnSinCos( ang, &s, &c );\
		save0 = NNM_MTX( *mtx, 0, ma );\
		save1 = NNM_MTX( *mtx, 0, mb );\
		NNM_MTX( *mtx, 0, ma ) = save0 *  c + save1 * s;\
		NNM_MTX( *mtx, 0, mb ) = save0 * -s + save1 * c;\
		save2 = NNM_MTX( *mtx, 1, ma );\
		save3 = NNM_MTX( *mtx, 1, mb );\
		NNM_MTX( *mtx, 1, ma ) = save2 *  c + save3 * s;\
		NNM_MTX( *mtx, 1, mb ) = save2 * -s + save3 * c;\
		save0 = NNM_MTX( *mtx, 2, ma );\
		save1 = NNM_MTX( *mtx, 2, mb );\
		NNM_MTX( *mtx, 2, ma ) = save0 *  c + save1 * s;\
		NNM_MTX( *mtx, 2, mb ) = save0 * -s + save1 * c;\
	}\
}
#else
#define nnmRotateMatrixFast( ang, ma, mb )\
{\
	if(ang != 0){\
		nnSinCos( ang, &sw, &cw );\
		s = sw;\
		c = cw;\
		save0 = NNM_MTX( *mtx, 0, ma );\
		save1 = NNM_MTX( *mtx, 0, mb );\
		NNM_MTX( *mtx, 0, ma ) = save0 *  c + save1 * s;\
		NNM_MTX( *mtx, 0, mb ) = save0 * -s + save1 * c;\
		save2 = NNM_MTX( *mtx, 1, ma );\
		save3 = NNM_MTX( *mtx, 1, mb );\
		NNM_MTX( *mtx, 1, ma ) = save2 *  c + save3 * s;\
		NNM_MTX( *mtx, 1, mb ) = save2 * -s + save3 * c;\
		save0 = NNM_MTX( *mtx, 2, ma );\
		save1 = NNM_MTX( *mtx, 2, mb );\
		NNM_MTX( *mtx, 2, ma ) = save0 *  c + save1 * s;\
		NNM_MTX( *mtx, 2, mb ) = save0 * -s + save1 * c;\
	}\
}
#endif


#define nnmRotateMatrixSinCosFast( ma, mb)\
{\
	for(i = 0;i < 3;i++){\
		save0 = NNM_MTX( *mtx, i, ma );\
		save1 = NNM_MTX( *mtx, i, mb );\
		NNM_MTX( *mtx, i, ma ) = save0 *  c + save1 * s;\
		NNM_MTX( *mtx, i, mb ) = save0 * -s + save1 * c;\
	}\
}

Sint32 nnCalcMotionTranslate(const NNS_SUBMOTION *submot, Float frame, NNS_VECTOR *tv);
Sint32 nnCalcMotionRotate( const NNS_SUBMOTION *submot, Float frame, NNS_ROTATE_A32 *rv, NNS_QUATERNION *rq, NNF_NODETYPE rtype );
Sint32 nnCalcMotionScale(const NNS_SUBMOTION *submot, Float frame, NNS_VECTOR *sv);
void nnCalcMatrixPaletteNode( Sint32 nodeIdx );
void nnCalcMatrixPaletteMotionNode(Sint32 nodeIdx );
void nnCalcMatrixPaletteTRSListNode( Sint32 nodeIdx );
void nnCalcMatrixPaletteLinkMotionNode( Sint32 nodeIdx );
Sint32 nnCalcMotionFrame(Float *dstframe, NNF_MOTIONTYPE fType, Float startframe, Float endframe, Float frame);

void nnCalcNodeStatusListMatrixPaletteNode( Sint32 nodeIdx );

void nnInitCircumsphere(void);
void nnDrawCircumsphereNode( Sint32 nodeIdx, NNF_SETNODESTATUS hideflag);
void nnDrawCircumsphereMotionNode( Sint32 nodeIdx, NNF_SETNODESTATUS hideflag);
void nnDrawCircumsphereTRSListNode( Sint32 nodeIdx, NNF_SETNODESTATUS hideflag);
void nnDrawCircumsphereCore( const NNS_VECTOR *center, Float radius, const NNS_MATRIX *mtx, const NNS_RGBA *col, NNE_BOOL trans );

void nnCalcMultiplyMatrices( NNS_MATRIX *dstlist, const NNS_MATRIX *src, const NNS_MATRIX *srclist, Sint32 num );
void nnCalcMatrixListMotionNode( NNS_MATRIX *mtxlist, const NNS_OBJECT *obj, const NNS_MATRIX *basemtx, const NNS_MOTION *mot, Float frame );
void nnCalcNodeMatrixNode( NNS_MATRIX *mtx, const NNS_OBJECT *obj, Sint32 nodeidx );
void nnCalcNodeMatrixMotionNode( NNS_MATRIX *mtx, Sint32 nodeidx );
void nnCalcNodeMatrixTRSListNode( NNS_MATRIX *mtx, const NNS_OBJECT *obj, Sint32 nodeidx, const NNS_TRS *trslist );

Sint32 nnCalcNodeMotionCore( NNS_MATRIX *pNodeMtx, Sint32 *pHideFlag, const NNS_MATRIX *pBaseMtx,
							 const NNS_NODE *pNode, Sint32 NodeIdx,
							 const NNS_OBJECT *pObj, const NNS_MOTION *pMot, Sint32 SubMotIdx, Float frame );
Sint32 nnCalcNodeMotionTRSCore( Sint32 *tflag, Sint32 *rflag, Sint32 *sflag,
								NNS_VECTOR *tv, NNS_VECTOR *sv,
								NNS_QUATERNION *rq, NNS_QUATERNION *invrq,
								const NNS_NODE *pNode, Sint32 NodeIdx,
								const NNS_MOTION *pMot, Sint32 SubMotIdx, Float frame );

void nnCopyMatrix33( NNS_MATRIX *dst, const NNS_MATRIX *src );

void nnCalc2BoneSIIK( NNS_MATRIX *jnt1mtx, NNS_MATRIX *jnt1motmtx, NNS_MATRIX *jnt2mtx, NNS_MATRIX *jnt2motmtx, NNS_MATRIX *effmtx, Float lbone1, Float lbone2, NNE_BOOL mprefz );
void nnCalc1BoneSIIK( NNS_MATRIX *jnt1mtx, NNS_MATRIX *jnt1motmtx, NNS_MATRIX *effmtx, Float lbone1 );
void nnCalcMatrixPaletteMotionNode2BoneSIIK( Sint32 jnt1nodeIdx );
void nnCalcMatrixPaletteMotionNode2BoneXSIIK( Sint32 rootidx );
void nnCalcMatrixPaletteMotionNode1BoneSIIK( Sint32 jnt1nodeIdx );
void nnCalcMatrixPaletteMotionNode1BoneXSIIK( Sint32 rootidx );
void nnCalcMatrixListMotionNode1BoneSIIK( NNS_MATRIX *mtxlist, const NNS_OBJECT *obj,
							const NNS_MATRIX *basemtx, Sint32 jnt1idx, Sint32 submotidx, const NNS_MOTION *mot, Float frame );
void nnCalcMatrixListMotionNode1BoneXSIIK( NNS_MATRIX *mtxlist, const NNS_OBJECT *obj,
							const NNS_MATRIX *basemtx, Sint32 rootidx, Sint32 submotidx, const NNS_MOTION *mot, Float frame );
void nnCalcMatrixListMotionNode2BoneSIIK( NNS_MATRIX *mtxlist, const NNS_OBJECT *obj,
							const NNS_MATRIX *basemtx, Sint32 jnt1idx, Sint32 submotidx, const NNS_MOTION *mot, Float frame );
void nnCalcMatrixListMotionNode2BoneXSIIK( NNS_MATRIX *mtxlist, const NNS_OBJECT *obj,
							const NNS_MATRIX *basemtx, Sint32 rootidx, Sint32 submotidx, const NNS_MOTION *mot, Float frame );
void nnCalcMatrixTRSList1BoneSIIK( NNS_MATRIX *jnt1mtx, NNS_MATRIX *effmtx, const NNS_OBJECT *obj, const NNS_TRS *trslist, const NNS_MATRIX *basemtx, Sint32 jnt1idx );
void nnCalcMatrixTRSList1BoneXSIIK( NNS_MATRIX *mtxlist, const NNS_OBJECT *obj, const NNS_TRS *trslist, const NNS_MATRIX *basemtx, Sint32 rootidx );
void nnCalcMatrixTRSList2BoneSIIK( NNS_MATRIX *jnt1mtx, NNS_MATRIX *jnt2mtx, NNS_MATRIX *effmtx, const NNS_OBJECT *obj, const NNS_TRS *trslist, const NNS_MATRIX *basemtx, Sint32 jnt1idx );
void nnCalcMatrixTRSList2BoneXSIIK( NNS_MATRIX *mtxlist, const NNS_OBJECT *obj, const NNS_TRS *trslist, const NNS_MATRIX *basemtx, Sint32 rootidx );
void nnCalcMatrixPaletteTRSListNode2BoneSIIK( Sint32 jnt1nodeIdx );
void nnCalcMatrixPaletteTRSListNode2BoneXSIIK( Sint32 rootidx );
void nnCalcMatrixPaletteTRSListNode1BoneSIIK( Sint32 jnt1nodeIdx );
void nnCalcMatrixPaletteTRSListNode1BoneXSIIK( Sint32 rootidx );
void nnAdjustMatrixXaxis( NNS_MATRIX *srcmtx, NNS_VECTORFAST *pos );
void nnCalcCosineTheorem( Float *sin, Float *cos, Float a, Float b, Float c );
void nnCalcCosineTheorem2( Float *sin0, Float *cos0, Float *sin1, Float *cos1, Float a, Float b, Float c );
void nnDrawOneBoneData(Float bonelength, const NNS_MATRIX *mtx, NNF_DRAWSIIKBONE flag);
void nnDrawEffector(NNS_VECTOR *p, const NNS_MATRIX *mtx);
void nnMakeNodeTreeMatrix(NNS_MATRIX *mtx, NNS_VECTOR *vec, NNS_VECTOR *trans);

void nnRotateXYZMatrixFast( NNS_MATRIX *mtx, Angle32 ax, Angle32 ay, Angle32 az );
void nnRotateXZYMatrixFast( NNS_MATRIX *mtx, Angle32 ax, Angle32 ay, Angle32 az );
void nnRotateZXYMatrixFast( NNS_MATRIX *mtx, Angle32 ax, Angle32 ay, Angle32 az );
void nnTranslateMatrixFast( NNS_MATRIX *mtx, Float x, Float y, Float z );
void nnScaleMatrixFast( NNS_MATRIX *mtx, Float x, Float y, Float z );

void nnRotateXMatrixSinCosFast( NNS_MATRIX *mtx, Float s, Float c );
void nnRotateYMatrixSinCosFast( NNS_MATRIX *mtx, Float s, Float c );
void nnRotateZMatrixSinCosFast( NNS_MATRIX *mtx, Float s, Float c );

Float nnEstimateMatrixScaling( const NNS_MATRIX *mtx );
void nnCalcClipSetNodeStatus( NNF_NODESTATUS *pNodeStatList, const NNS_NODE *pNodeList, Sint32 nodeIdx, const NNS_MATRIX *pNodeMtx, Float rootscale, NNF_SETNODESTATUS flag );
void nnSetUpNodeStatusListFlag( Sint32 nodeidx, NNF_SETNODESTATUS flag );
NNF_CLIP nnCalcClipBoxNode( const NNS_NODE *node, const NNS_MATRIX *mtx );
void nnDrawClipBoxCore( const NNS_VECTOR *center, Float sx, Float sy, Float sz, const NNS_MATRIX *mtx, const NNS_RGBA *col, NNE_BOOL trans );

void nnCallbackMotionUserData( const NNS_OBJECT *obj, const NNS_MOTION *mot, Sint32 SubMotIdx, Sint32 NodeIdx, Float nframe, Float orgframe );
void nnCalcMotionUserData( NNS_NODEUSRMOT_CALLBACK_VAL *val, const NNS_SUBMOTION *submot, Float frame );
void nnNormalizeColumn( NNS_MATRIX *mtx, Sint32 clm );

Sint32 nnCalcMotionNodeHide( const NNS_SUBMOTION *submot, Float frame );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNDNODE_H__ */

/* End of file */
