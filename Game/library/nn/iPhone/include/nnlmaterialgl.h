/*---------------------------------------------------------------------------

    NN Material library for OpenGL

    Copyright (C) 2002, 2003 SEGA Corporation Library R&D Dept.
    All Rights Reserved.

    Module  : NN dev Material Library for OpenGL
    File    : nndmaterialgc.h
    Create  : 2004/04/28
    Modify  : 2004/11/29 標準シェーダ
    Modify  : 2004/12/22 標準シェーダ用マテリアル設定関数
    Modify  : 2005/01/24 nnPrint等シェーダ化
    Modify  : 2005/01/24 nnPutColorNTexture追加
    Modify  : 2005/04/26 デュアルパラボロイドマトリクスを3x3にした。
    Modify  : 2005/06/16 テクスチャLODバイアスコントロール追加
    Modify  : 2006/05/11 ユーザーサンプラー機能追加
    Version : 1.04.00
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNDMATERIALGC_H__
#define __NNDMATERIALGC_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*** マテリアルコントロール ***/

#if ( NND_PLATFORM == NND_PLATFORM_GL )

typedef enum {
	NNE_TEXTURETYPE_NORMAL,
	NNE_TEXTURETYPE_BASE,
	NNE_TEXTURETYPE_DECAL,
	NNE_TEXTURETYPE_DECAL2,
	NNE_TEXTURETYPE_DECAL3,
	NNE_TEXTURETYPE_SPECULAR,
	NNE_TEXTURETYPE_SHININESS,
	NNE_TEXTURETYPE_ENVMASK,
	NNE_TEXTURETYPE_MODULATE,
	NNE_TEXTURETYPE_ADD,
	NNE_TEXTURETYPE_OPACITY,
	NNE_TEXTURETYPE_USER1,
	NNE_TEXTURETYPE_USER2,
	NNE_TEXTURETYPE_USER3,
	NNE_TEXTURETYPE_USER4,
	NNE_TEXTURETYPE_USER5,
	NNE_TEXTURETYPE_USER6,
	NNE_TEXTURETYPE_USER7,
	NNE_TEXTURETYPE_USER8,
	NNE_TEXTURETYPE_DUALPARABOLOID,

	NNE_TEXTURETYPE_MAX,
} NNE_TEXTURETYPE_GL;

typedef enum {
	NNE_SHADOWMAP_1 = 0,
	NNE_SHADOWMAP_2,

	NNE_SHADOWMAP_MAX,
} NNE_SHADOWMAP;

typedef enum {
	NNE_USER_SAMPLER_2D_1,
	NNE_USER_SAMPLER_2D_2,
	NNE_USER_SAMPLER_3D_1,
	NNE_USER_SAMPLER_3D_2,
	NNE_USER_SAMPLER_CUBE_1,
	NNE_USER_SAMPLER_CUBE_2,

	NNE_USER_SAMPLER_MAX,
} NNE_USER_SAMPLER;

typedef struct{
	NNE_MATCTRLMODE		mode;
	Float				bias;
} NNS_MATCTRL_TEXLODBIAS;

#endif	// ( NND_PLATFORM == NND_PLATFORM_GL )

void nnSetMaterialControlSpecularGL( NNE_MATCTRLMODE mode, Float r, Float g, Float b );

#if ( NND_PLATFORM == NND_PLATFORM_GL )
void nnSetMaterialControlTextureOffsetGL( NNE_TEXTURETYPE_GL textype, NNE_MATCTRLMODE mode, Float u, Float v );
void nnSetMaterialControlTextureLodBiasGL( NNE_TEXTURETYPE_GL textype, NNE_MATCTRLMODE mode, Float bias );
void nnSetMaterialControlShadowMapGL( NNE_SHADOWMAP idx, GLuint texname, const NNS_MATRIX44 *mtx, Float r, Float g, Float b );
void nnSetMaterialControlUserSamplerGL( NNE_USER_SAMPLER idx, GLuint texname );

// デュアルパラボロイド環境マップマトリクス設定
void nnSetDualParaboloidMatrixGL( NNS_MATRIX *mtx );
#endif	// ( NND_PLATFORM == NND_PLATFORM_GL )


/*** マテリアル設定(nnPutMaterialで使用される変数/関数) ***/
// マテリアルコールバックで必要なとき以外はなるべく参照しないでください。

// NN内部管理グローバル変数
#if ( NND_PLATFORM == NND_PLATFORM_GL )
typedef struct {
	GLuint			texname;
	NNS_MATRIX44	mtx;
	NNS_RGB			col;
} NNS_MATCTRL_SHADOWMAP;

typedef struct {
	GLuint			texname;
} NNS_MATCTRL_USERSAMPLER;
#endif	// ( NND_PLATFORM == NND_PLATFORM_GL )

extern NNF_MATFLAG nngPreMatFlag;
extern const void *nngpPreMatColor;
extern const void *nngpPreMatLogic;

extern NNS_MATCTRL_RGB nngMatCtrlDiffuse;
extern NNS_MATCTRL_RGB nngMatCtrlAmbient;
extern NNS_MATCTRL_RGB nngMatCtrlSpecular;
extern NNS_MATCTRL_ALPHA nngMatCtrlAlpha;
extern NNS_MATCTRL_ENVTEXMATRIX nngMatCtrlEnvTexMtx;
extern NNS_MATCTRL_BLENDMODE nngMatCtrlBlendMode;
extern NNS_MATCTRL_TEXOFFSET nngMatCtrlTexOffset[NNE_TEXSLOT_MAX];

#if ( NND_PLATFORM == NND_PLATFORM_GL )
extern NNS_MATCTRL_TEXOFFSET nngMatCtrlTexOffsetGL[NNE_TEXTURETYPE_MAX];
extern NNS_MATCTRL_TEXLODBIAS nngMatCtrlTexLodBias[NNE_TEXTURETYPE_MAX];
extern NNS_MATCTRL_SHADOWMAP nngMatCtrlShadowMap[NNE_SHADOWMAP_MAX];
//extern NNS_MATRIX nngDualParaboloidMatrix;
extern Float nngDualParaboloidMatrix33[];
extern NNS_MATCTRL_USERSAMPLER nngMatCtrlUserSamplerTexName[NNE_USER_SAMPLER_MAX];
#endif	// ( NND_PLATFORM == NND_PLATFORM_GL )

// PreMat変数初期化
void nnInitPreviousMaterialValueGL(void);
// マテリアルフラグ設定
void nnPutMaterialFlagGL(const NNS_DRAWCALLBACK_VAL *val, NNF_MATFLAG fMatFlag);
// マテリアルカラー設定
void nnPutMaterialColorGL(GLenum face, const NNS_DRAWCALLBACK_VAL *val, const NNS_MATERIAL_STDSHADER_COLOR *pColor);
// マテリアルLogic設定
void nnPutMaterialLogicGL(const NNS_DRAWCALLBACK_VAL *val, const NNS_MATERIAL_LOGIC *pLogic);
// テクスチャマッピング設定
void nnPutMaterialTextureOneGL(NNE_TEXSLOT slot, const NNS_DRAWCALLBACK_VAL *val, const NNS_MATERIAL_TEXMAP_DESC *pTex);
void nnPutMaterialTexturesGL(const NNS_DRAWCALLBACK_VAL *val, const NNS_MATERIAL_TEXMAP_DESC *texdesc, Sint32 num);

#if ( NND_PLATFORM == NND_PLATFORM_GL )
void nnPutMaterialStdShaderTextureOneGL(NNE_TEXSLOT slot, const NNS_DRAWCALLBACK_VAL *val,
										const NNS_MATERIAL_STDSHADER_TEXMAP_DESC *pTex);
void nnPutMaterialStdShaderTexturesGL(const NNS_DRAWCALLBACK_VAL *val,
									  const NNS_MATERIAL_STDSHADER_TEXMAP_DESC *texdesc, Sint32 num);
#endif	// ( NND_PLATFORM == NND_PLATFORM_GL )

// 固定マテリアル設定(特殊描画用)
void nnPutFixedMaterialGL(void);
// テクスチャ無し設定(特殊描画用)
void nnPutDisableTexturesGL(void);

// 色分け描画カラー設定関数
void nnPutWireColor(void);
void nnPutColorStrip(Sint32 iStrip, Sint32 iMeshset, Sint32 iSubobj);
void nnPutColorMeshset(Sint32 iMeshset, Sint32 iSubobj);
void nnPutColorMaterial(Sint32 iMaterial);
void nnPutColorNWeight(const NNS_VTXLISTPTR *vlistptr);
void nnPutColorShader( const NNS_DRAWCALLBACK_VAL *val );
void nnPutColorNTexture(Sint32 nTexture);

#if ( NND_PLATFORM == NND_PLATFORM_GL )
// 標準シェーダ
void nnBindShaderGL( const NNS_DRAWCALLBACK_VAL *val );
void nnBindFixedShaderGL( void );
void nnBindPrintShaderGL( void );
void nnBindPrimitive2DShaderGL( NNE_BOOL bTexture );
void nnBindPrimitive3DShaderGL( NNE_BOOL bLighting, NNE_BOOL bTexture, NNE_PRIM_TEXCOORD texcoord );


//*** マテリアルコールバック専用関数
// ユーザユニフォーム変数設定
void nnPutUserUniformGL( const NNS_DRAWCALLBACK_VAL *val );

#endif	// ( NND_PLATFORM == NND_PLATFORM_GL )

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNDMATERIALGC_H__ */

/* End of file */
