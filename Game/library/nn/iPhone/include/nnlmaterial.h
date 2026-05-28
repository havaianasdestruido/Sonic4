/*---------------------------------------------------------------------------

    NN Material Library

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN Material Library
    File    : nnlmaterial.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Modify  : 2003/08/07 PS2版のみnnPutMaterialCoreをインライン化
    Modify  : 2003/08/15 nnGetMaterialIndex,nnGetMaterialName関数を追加
    Modify  : 2003/10/08 Xbox,PC共通化向けの修正
    Modify  : 2003/11/14 inline 関数の定義を static inline に変更
    Modify  : 2003/12/12 nnSetDivColor()機種別ヘッダから移動・仕様変更
                         元のnnSetDivColor()をnnSetDivColorRandom()にリネーム
    Modify  : 2003/12/19 マテリアルモーション関数を追加
    Modify  : 2003/12/19 MATERIALCONTROL関係を追加
    Modify  : 2004/01/13 MATERIALCONTROL関係を全体に見直し
    Modify  : 2004/01/21 inline 関数の定義を static __inline に変更
    Modify  : 2004/03/26 nnSetMaterialControlSpecularXB関数追加
    Modify  : 2004/04/26 nnSetMaterialControlSpecularXB関数を
                         nnSetMaterialControlSpecularDXへ変更
    Modify  : 2004/06/16 DX8,DX9版統合
    Modify  : 2004/07/08 NNE_MATCTRLMODEの先頭にNNE_MATCTRLMODE_NONEを追加
                         NNS_MATCTRL_TEXOFFSET追加
                         nnSetMaterialControlTextureOffset()追加
    Modify  : 2004/08/05 NND_PLATFORM_XENON追加、NNS_DRAWCALLBACK_VAL修正
    Modify  : 2004/08/12 OpenGL版統合
    Modify  : 2004/09/01 マテリアルモーション関連関数の引数にconst付加
    Modify  : 2004/12/13 nnSetTangentLengthなど接線・従法線描画追加
    Modify  : 2004/12/24 NNS_DRAWCALLBACK_VALにメンバbReDraw追加
    Modify  : 2005/03/10 DT06版統合
    Modify  : 2005/04/21 NNS_DRAWCALLBACK_VALにメンバiVtxList,iPrevVtxList,pVtxListPtr追加
    Modify  : 2005/04/26 PSP版統合
    Modify  : 2005/05/09 nnSetMaterialControlAlphaRef(), nnSetMaterialControlAlphaWithAlphaRefSwitch()追加
    Modify  : 2005/05/30 NNF_MATSTATUS, nnCalcMaterialMotionMaterialStatusList(), nnCalcMaterialStatusListNodeStatusList()追加
    Modify  : 2005/06/21 nnCalcMaterialMotionNoReset()追加
    Modify  : 2006/03/15 PLAYSTATION3版統合
    Modify  : 2008/05/21 NND_PLATFORM_DXG20追加
    Modify  : 2008/11/12  PSP:プライオリティノード関連追加
	Modify  : 2009/03/16 nnPutMaterialCoreEx,nnPutMaterialCoreEx1st削除
    Modify  : 2009/05/13 NND_PLATFORM_XENON削除
    Version : 1.18.31
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLMATERIAL_H__
#define __NNLMATERIAL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Material status */
typedef Uint32	NNF_MATSTATUS;
#define	NND_MATSTATUS_NONE				((Uint32)0 <<  0)	/* No status */
#define	NND_MATSTATUS_HIDE				((Uint32)1 <<  0)	/* No drawing */

typedef NNS_OBJECT NNS_MATMOTOBJ;

/* マテリアル制御モード */
typedef enum {
	NNE_MATCTRLMODE_NONE,		/* マテリアル制御によって変化させない */
	NNE_MATCTRLMODE_REPLACE,	/* オブジェクトのマテリアルの値を上書き */
	NNE_MATCTRLMODE_ADD,		/* オブジェクトのマテリアルの値に加算 */
	NNE_MATCTRLMODE_MODULATE	/* オブジェクトのマテリアルの値に乗算 */
} NNE_MATCTRLMODE;

/* マテリアル制御テクスチャ座標ソース */
typedef enum {
	NNE_MATCTRL_TEXCOORDSRC_POSITION,	/* 頂点座標を変換してテクスチャ座標に使用する */
	NNE_MATCTRL_TEXCOORDSRC_NORMAL		/* 頂点法線を変換してテクスチャ座標に使用する */
} NNE_MATCTRL_TEXCOORDSRC;

/* マテリアル制御フレームバッファブレンドモード */
typedef enum {
	NNE_MATCTRL_BLEND_ALPHA,		/* アルファ合成を行なう */
	NNE_MATCTRL_BLEND_ADD,			/* 加算合成を行なう */
	NNE_MATCTRL_BLEND_SUBTRACT		/* 減算合成を行なう */
} NNE_MATCTRL_BLEND;


/* ドローコールバック変数 */
typedef struct {
	Sint32 iMaterial;						// マテリアルインデックス
	Sint32 iPrevMaterial;					// 直前のマテリアルインデックス
	Sint32 iVtxList;						// 頂点リストインデクス
	Sint32 iPrevVtxList;					// 直前の頂点リストインデクス
	Sint32 iNode;							// ノードインデックス
	Sint32 iMeshset;						// メッシュセットインデックス
	Sint32 iSubobject;						// サブオブジェクトインデックス
	NNS_MATERIALPTR *pMaterial;				// マテリアルポインタ
	NNS_VTXLISTPTR *pVtxListPtr;			// 頂点リストポインタ
	NNS_OBJECT *pObject;					// オブジェクト
	NNS_MATRIX *pMatrixPalette;				// マトリクスパレット
	NNF_NODESTATUS *pNodeStatusList;		// ノードステータスリスト
	NNF_SUBOBJTYPE DrawSubobjType;			// nnDrawObject()のサブオブジェクト指定
	NNF_DRAWOBJ DrawFlag;					// nnDrawObject()の描画フラグ
	NNE_BOOL bModified;						// マテリアルデータ修正フラグ
	NNE_BOOL bReDraw;						// 再描画フラグ
}NNS_DRAWCALLBACK_VAL;


/* マテリアル制御 */
typedef struct{
	NNE_MATCTRLMODE		mode;
	NNS_RGB				col;
}NNS_MATCTRL_RGB;

typedef struct{
	NNE_MATCTRLMODE		mode;
	Float				alpha;
}NNS_MATCTRL_ALPHA;

typedef struct{
	NNE_MATCTRLMODE		mode;
	NNS_TEXCOORD		offset;
}NNS_MATCTRL_TEXOFFSET;

typedef struct{
	NNE_MATCTRL_TEXCOORDSRC	texcoordsrc;
	NNS_MATRIX				texmtx;
}NNS_MATCTRL_ENVTEXMATRIX;

typedef struct{
	NNE_MATCTRL_BLEND	blendmode;
}NNS_MATCTRL_BLENDMODE;

typedef NNE_BOOL (*NNS_MATERIALCALLBACK_FUNC)( NNS_DRAWCALLBACK_VAL *val);
void nnSetWireColor(Float r, Float g, Float b, Float a);
void nnSetNormalLength(Float len);
void nnSetNormalColor(Float r, Float g, Float b, Float a);
void nnSetTangentLength(Float len);
void nnSetTangentColor(Float r, Float g, Float b, Float a);
void nnSetBinormalLength(Float len);
void nnSetBinormalColor(Float r, Float g, Float b, Float a);
void nnSetDivColor( Float r, Float g, Float b, Float a );
void nnSetDivColorRandom(Sint32 i);

void nnSetMaterialCallback(NNS_MATERIALCALLBACK_FUNC func);
NNS_MATERIALCALLBACK_FUNC nnGetMaterialCallback(void);

#define nnGetMaterialIndex( pMaterialNameList, MaterialName) \
	nnGetNodeIndex( (const NNS_NODENAMELIST *)pMaterialNameList, MaterialName )

#define nnGetMaterialName( pMaterialNameList, MaterialIndex) \
	nnGetNodeName( (const NNS_NODENAMELIST *)pMaterialNameList, MaterialIndex )

#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
NNE_BOOL nnPutMaterialCoreLtd( const NNS_DRAWCALLBACK_VAL *val);
NNE_BOOL nnPutMaterialCoreExt( const NNS_DRAWCALLBACK_VAL *val);
static __inline NNE_BOOL nnPutMaterialCore( const NNS_DRAWCALLBACK_VAL *val)
{
	if(val->DrawFlag & (~(NND_DRAWOBJ_INSIDE))){
		return nnPutMaterialCoreExt( val);
	}
	else{
		return nnPutMaterialCoreLtd( val);
	}
}
#else
NNE_BOOL nnPutMaterialCore( const NNS_DRAWCALLBACK_VAL *val);
#endif

Uint32 nnCalcMaterialMotionObjectBufferSize( const NNS_OBJECT *obj, const NNS_MOTION *mmot );
void nnInitMaterialMotionObject( NNS_MATMOTOBJ *mmobj, const NNS_OBJECT *obj, const NNS_MOTION *mmot );
void nnCalcMaterialMotion( NNS_MATMOTOBJ *mmobj, const NNS_OBJECT *obj, const NNS_MOTION *mmot, Float frame );
void nnDrawMaterialMotionObject( const NNS_MATMOTOBJ *mmobj, const NNS_MATRIX *mtxpal, const NNF_NODESTATUS *nodestatlist, NNF_SUBOBJTYPE subobjtype, NNF_DRAWOBJ flag );

void nnSetMaterialControlDiffuse( NNE_MATCTRLMODE mode, Float r, Float g, Float b );
void nnSetMaterialControlAmbient( NNE_MATCTRLMODE mode, Float r, Float g, Float b );
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
void nnCalcMaterialMotionNoReset( NNS_MATMOTOBJ *mmobj, const NNS_OBJECT *obj, const NNS_MOTION *mmot, Float frame );
void nnCalcMaterialMotionMaterialStatusList( NNS_MATMOTOBJ *mmobj, const NNS_OBJECT *obj, const NNS_MOTION *mmot , Float frame, const NNF_MATSTATUS *pMatStatList );
void nnCalcMaterialStatusListNodeStatusList( NNF_MATSTATUS *pMatStatList, const NNS_OBJECT *pObj, const NNF_NODESTATUS *pNodeStatList );
void nnSetMaterialControlSpecularPS2( NNE_MATCTRLMODE mode, Float spec );
#endif
#if ( NND_PLATFORM == NND_PLATFORM_GC )
void nnSetMaterialControlSpecularGC( NNE_MATCTRLMODE mode, Float r, Float g, Float b );
#endif
#if ( NND_PLATFORM == NND_PLATFORM_XB || NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
void nnSetMaterialControlSpecularXE( NNE_MATCTRLMODE mode, Float r, Float g, Float b );
#endif
#if ( NND_PLATFORM == NND_PLATFORM_DXG20)
void nnSetMaterialControlSpecularDXG20( NNE_MATCTRLMODE mode, Float r, Float g, Float b );
#endif
void nnSetMaterialControlAlpha( NNE_MATCTRLMODE mode, Float alpha );
void nnSetMaterialControlAlphaRef( NNE_MATCTRLMODE mode, Float alpharef );
void nnSetMaterialControlAlphaWithAlphaRefSwitch( NNE_BOOL on_off );
void nnSetMaterialControlEnvTexMatrix( NNE_MATCTRL_TEXCOORDSRC texcoordsrc, const NNS_MATRIX *texmtx );
void nnSetMaterialControlBlendMode( NNE_MATCTRL_BLEND blendmode );
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
void nnSetMaterialControlGsAlphaPS2( Uint32 alphaand, Uint32 alphaor );
void nnSetMaterialControlGsPrmodePS2( NNF_GSPRIMMODE prmodeand, NNF_GSPRIMMODE prmodeor );
#endif
void nnSetMaterialControlTextureOffset( NNE_TEXSLOT slot, NNE_MATCTRLMODE mode, Float u, Float v );


#ifdef __cplusplus
}
#endif /* __cplusplus */

/* PlayStation2 material library */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nnlmaterialps2.h"
#endif

/* GAMECUBE material library */
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nnlmaterialgc.h"
#endif

/* Xbox material library */
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nnlmaterialdx.h"
#endif

/* PC material library */
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
#include "nnlmaterialdx.h"
#endif

/* OpenGL material library */
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#include "nnlmaterialgl.h"
#endif

/* PSP material library */
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#include "nnlmaterialpsp.h"
#endif

/* PS3 material library */
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
#include "nnlmaterialps3.h"
#endif

/* DXG20 material library */
#if ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#include "nnlmaterialdxg20.h"
#endif

#endif /* __NNLMATERIAL_H__ */

/* End of file */
