/*---------------------------------------------------------------------------

    NN Draw object

    Copyright (C) 2004 SEGA Corporation Creative Center Graphics Sect.
    All Rights Reserved.

    Module  : NN Draw object Library
    File    : nnldrawobjgl.h
    Create  : 2004/05/21
    Modify  : 2004/11/05 NND_BINDOBJ_EXCLUDE_PLIABLE_VERTICESフラグ追加
    Modify  : 2004/11/08 nnDeleteBufferPliableObjectGL関数追加
    Modify  : 2005/06/21 NND_BINDOBJ_DETACHED_ARRAY追加
    Version : 1.03.01
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLDRAWOBJGL_H__
#define __NNLDRAWOBJGL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef Uint32 NNF_BINDOBJ;

#define NND_BINDOBJ_EXCLUDE_PLIABLE_VERTICES	((Uint32)(1 << 0))

#define NND_BINDOBJ_DETACHED_ARRAY				((Uint32)(1 << 1))
	/* Position,Normal等の各配列がメモリ上で離れている場合を考慮する。*/
	/* テキスト形式のオブジェクトファイルではnnBindBufferObjectGL()内で自動的にこのフラグが立つ */


/* 頂点・インデクスデータをOpenGL serverに送る */
Uint32 nnBindBufferObjectGL(NNS_OBJECT *dstobj, const NNS_OBJECT *srcobj, NNF_BINDOBJ flag);
void nnBindBufferObjectDirectGL(NNS_OBJECT *obj, NNF_BINDOBJ flag);

Uint32 nnCalcBindBufferObjectSizeGL(const NNS_OBJECT *obj, NNF_BINDOBJ flag);

/* 頂点・インデクスデータをOpenGL serverから削除 */
void nnDeleteBufferObjectGL(NNS_OBJECT *obj);

/* エンベロープオブジェクトの頂点データをOpenGL serverから削除 */
void nnDeleteBufferPliableObjectGL( NNS_PLIABLEOBJ *pobj );


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNLDRAWOBJGL_H__ */

/* End of file */
