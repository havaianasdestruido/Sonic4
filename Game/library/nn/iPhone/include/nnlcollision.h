/*---------------------------------------------------------------------------

    NN Collision

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN Collision Library
    File    : nnlcollision.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Version : 1.00.00
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLCOLLISION_H__
#define __NNLCOLLISION_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* 球 */
typedef struct {
	NNS_VECTOR	c;		/* 中心座標 */
	Float		r;		/* 半径 */
} NNS_SPHERE;

/* カプセル */
typedef struct {
	NNS_VECTOR	c1;		/* カプセルの端1の中心座標 */
	NNS_VECTOR	c2;		/* カプセルの端2の中心座標 */
	Float		r;		/* 半径 */
} NNS_CAPSULE;

/* 平行六面体 */
typedef struct {
	NNS_VECTOR	p;		/* 六面体のある１頂点 */
	NNS_VECTOR	v[3];	/* 頂点 p を始点とする辺のベクトル */
} NNS_BOX;


/* 球とカプセルの衝突判定 */
NNE_BOOL nnCheckCollisionSS( const NNS_SPHERE *sphere1, const NNS_SPHERE *sphere2 );
NNE_BOOL nnCheckCollisionSC( const NNS_SPHERE *sphere, const NNS_CAPSULE *capsule );
NNE_BOOL nnCheckCollisionCC( const NNS_CAPSULE *capsule1, const NNS_CAPSULE *capsule2 );

/* BOX との衝突判定 */
NNE_BOOL nnCheckCollisionBB( const NNS_BOX *box1, const NNS_BOX *box2 );
NNE_BOOL nnCheckCollisionBS( const NNS_BOX *box, const NNS_SPHERE *sphere );
NNE_BOOL nnCheckCollisionBC( const NNS_BOX *box, const NNS_CAPSULE *capsule );


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNLCOLLISION_H__ */

/* End of file */
