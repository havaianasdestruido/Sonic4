/*---------------------------------------------------------------------------

    NN dev api header for OpenGL

    Copyright (C) 2004 SEGA Corporation Creative Center Graphics Sect.
    All Rights Reserved.

    Module  : NN dev api header for OpenGL
    File    : nndgl.h
    Create  : 2004/04/20
    Modify  : 2004/11/18 標準シェーダ関連いろいろ
    Modify  : 2004/11/25 フォグパラメータ
    Modify  : 2004/11/29 シェーダ関連
    Modify  : 2005/01/25 nnPutFogSwitchGL関数追加
    Modify  : 2005/02/01 use_matrix_palette削除
	Modify  : 2006/04/13 NNM_ALIGN_POINTERの廃止（C言語の仕様上、NULLポインタとの減算が未定義なので）
	Modify  : 2006/05/23 Cgコンパイラによるシェーダ事前コンパイル対応
    Version : 1.04.00
    Note    : 

---------------------------------------------------------------------------*/

#ifndef	__NNDGL_H__
#define	__NNDGL_H__

#include <memory.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



// Supported GL Extensions
typedef struct {
	// Supported Extensions
	GLint		max_texture_units;
	NNE_BOOL	light_max_exponent;
	GLfloat		max_shininess;
#if ( NND_PLATFORM == NND_PLATFORM_GL )
	NNE_BOOL	half_float;
	NNE_BOOL	NV_vertex_program3;
	NNE_BOOL	NV_fragment_program2;
#endif	// ( NND_PLATFORM == NND_PLATFORM_GL )
} NNS_SUPPORTED_GL_EXTENSIONS;

extern NNS_SUPPORTED_GL_EXTENSIONS nngGLExtensions;


// 定数
extern const NNS_RGBA nngColorWhite;
extern const NNS_RGBA nngColorBlack;
extern const NNS_MATRIX nngEnvMtx;
extern const GLenum nngGLModelView[];

// OpenGL定数マクロ
#define NNM_GL_TEXTURE( _slot )			( GL_TEXTURE0 + ( _slot ) )
#define NNM_GL_LIGHT( _idx )			( GL_LIGHT0 + ( _idx ) )

// メモリー操作
#define	nnCopyMemory( dst, src, size )	memcpy( (dst), (src), (size) )
#define	nnFillMemory( dst, fill, size )	memset( (dst), (fill), (size) )

// align
//#define NNM_ALIGN_POINTER( _ptr, _align )		((((char *)(_ptr) + ((_align) - 1) - (char*)NULL) / (_align)) * (_align) + (char *)NULL)
#define NNM_ALIGN_SIZE( _size, _align )			((((_size) + ((_align) - 1)) / (_align)) * (_align))

// プロジェクションマトリクス
void nnLoadProjectionMatrixGL(const NNS_MATRIX44 *mtx);

// オブジェクト描画
extern NNS_DRAWCALLBACK_VAL nngDrawCallBackVal;

void nnDrawObjectVertexList(const NNS_VTXLISTPTR *vlistptr, NNF_DRAWOBJ flag);
void nnDrawObjectPrimitiveList(const NNS_PRIMLISTPTR *plistptr, NNF_DRAWOBJ flag);
void nnDrawObjectVertexListInitialPose(const NNS_VTXLISTPTR *vlistptr, NNF_DRAWOBJ flag);
void nnDrawObjectNormal(const NNS_VTXLISTPTR *vlistptr, const NNS_PRIMLISTPTR *plistptr, const NNS_MATRIX *mtxpal, NNF_DRAWOBJ flag);
void nnDrawObjectCommonVertex(const NNS_VTXLISTPTR *vlistptr, const NNS_PRIMLISTPTR *plistptr, const NNS_MATRIX *mtxpal, NNF_DRAWOBJ flag);
void nnDrawObjectCommonVertexNormal(const NNS_VTXLISTPTR *vlistptr, const NNS_PRIMLISTPTR *plistptr, const NNS_MATRIX *mtxpal);
void nnDrawObjectVertexBlend(const NNS_VTXLISTPTR *vlistptr, const NNS_PRIMLISTPTR *plistptr, const NNS_MATRIX *mtxpal, NNF_DRAWOBJ flag);

void nnInvertTransposeMatrix33( NNS_MATRIX *dst, const NNS_MATRIX *src );
void nnInvertTransposeMatrix33NotNormalized( NNS_MATRIX *dst, const NNS_MATRIX *src );

// オブジェクトコピー
Uint32 nnCopyMaterialList(NNS_MATERIALPTR *dstmatptr, const NNS_MATERIALPTR *srcmatptr, Sint32 nMaterial, NNF_COPYOBJ flag);
Uint32 nnCopyVertexList(NNS_VTXLISTPTR *dstvlist, const NNS_VTXLISTPTR *srcvlist, Sint32 nVtxList, NNF_COPYOBJ flag);
Uint32 nnCopyPrimitiveList(NNS_PRIMLISTPTR *dstplist, const NNS_PRIMLISTPTR *srcplist, Sint32 nPrimList, NNF_COPYOBJ flag);
Uint32 nnCopySubobjList(NNS_SUBOBJ *pSubobjListDst, const NNS_SUBOBJ *pSubobjListSrc, Sint32 nSubobj, NNF_COPYOBJ flag);

Uint32 nnBindBufferVertexListGL(NNS_VTXLISTPTR *dstvlist, const NNS_VTXLISTPTR *srcvlist, Sint32 nVtxList, NNF_BINDOBJ flag);
Uint32 nnBindBufferPrimitiveListGL(NNS_PRIMLISTPTR *dstplist, const NNS_PRIMLISTPTR *srcplist, Sint32 nPrimList, NNF_BINDOBJ flag);

void nnConvertPosition4sTo3f(Float *dst, const Sint16 *src, Sint32 nVertex);
void nnConvertNormal3bTo3f(Float *dst, const Sint8 *src, Sint32 nVertex);
void nnConvertNormal3sTo3f(Float *dst, const Sint16 *src, Sint32 nVertex);


// マテリアル
NNE_BOOL nnPutMaterial(NNS_DRAWCALLBACK_VAL *val);

/* ランダム色分け */
void nnSetDivColorRandomA(int nSeed, Uint32* seeds);

// テクスチャ
Sint32 nnSetTexInfo( NNE_TEXSLOT slot, NNS_TEXINFO *pTexInfo );

// プリミティブ描画
extern NNE_PRIM_BLEND		nngDrawPrimBlend;
extern NNE_BOOL				nngDrawPrimTexture;
extern NNE_PRIM_TEXCOORD	nngDrawPrimTexCoord;

void nnPutPrimitiveTexParameter(void);
void nnPutPrimitiveNoTexture(void);


// フォグ
extern NNE_BOOL nngFogSwitch;
extern Float nngFogStart;
extern Float nngFogEnd;
extern Float nngFogDensity;

void nnPutFogSwitchGL( NNE_BOOL on_off );

#if ( NND_PLATFORM == NND_PLATFORM_GL )

// ライト数
extern Sint32 nngNumParallelLight;
extern Sint32 nngNumPointLight;
extern Sint32 nngNumSpotLight;
extern Float nngPointLightFallOffEnd[4];
extern Float nngPointLightFallOffScale[4];
extern Float nngSpotLightFallOffEnd[4];
extern Float nngSpotLightFallOffScale[4];
extern Float nngSpotLightAngleScale[4];



// 頂点アトリビュートロケーション
#define NND_WEIGHT_LOCATION			(1)
#define NND_MTXIDX_LOCATION			(5)
#define NND_TANGENT_LOCATION		(6)
#define NND_BINORMAL_LOCATION		(7)

// Vertex Program Local Parameter Index
#define NND_VTXPROGPARAM_NUMPARALLELLIGHT			(0)
#define NND_VTXPROGPARAM_NUMPOINTLIGHT				(1)
#define NND_VTXPROGPARAM_NUMSPOTLIGHT				(2)
#define NND_VTXPROGPARAM_POINTLIGHTFALLOFFEND		(3)		/* 0~4 */
#define NND_VTXPROGPARAM_POINTLIGHTFALLOFFSCALE		(7)		/* 0~4 */
#define NND_VTXPROGPARAM_SPOTLIGHTFALLOFFEND		(11)	/* 0~4 */
#define NND_VTXPROGPARAM_SPOTLIGHTFALLOFFSCALE		(15)	/* 0~4 */
#define NND_VTXPROGPARAM_SPOTLIGHTANGLESCALE		(19)	/* 0~4 */
#define NND_VTXPROGPARAM_TEXUSER1PARAM				(23)
#define NND_VTXPROGPARAM_TEXUSER2PARAM				(24)
#define NND_VTXPROGPARAM_TEXUSER3PARAM				(25)
#define NND_VTXPROGPARAM_TEXUSER4PARAM				(26)
#define NND_VTXPROGPARAM_TEXUSER5PARAM				(27)
#define NND_VTXPROGPARAM_TEXUSER6PARAM				(28)
#define NND_VTXPROGPARAM_TEXUSER7PARAM				(29)
#define NND_VTXPROGPARAM_TEXUSER8PARAM				(30)
#define NND_VTXPROGPARAM_POSITIONMATRIX				(31)	/* 0 or 4*16 */
#define NND_VTXPROGPARAM_NORMALMATRIX				(95)	/* 0 or 3*16 */
#define NND_VTXPROGPARAM_USERUNIFORM				(143)	/* 0~256 */

// Fragment Program Local Parameter Index
#define NND_FRAGPROGPARAM_POINTLIGHTFALLOFFEND		(0)		/* 0~8 */
#define NND_FRAGPROGPARAM_POINTLIGHTFALLOFFSCALE	(4)		/* 0~8 */
#define NND_FRAGPROGPARAM_TEXBASEALPHA				(8)
#define NND_FRAGPROGPARAM_TEXDECALALPHA				(9)
#define NND_FRAGPROGPARAM_TEXDECAL2ALPHA			(10)
#define NND_FRAGPROGPARAM_TEXDECAL3ALPHA			(11)
#define NND_FRAGPROGPARAM_TEXSHININESSLEVEL			(12)
#define NND_FRAGPROGPARAM_TEXDUALPARABOLOIDLEVEL	(13)
#define NND_FRAGPROGPARAM_TEXADDLEVEL				(14)
#define NND_FRAGPROGPARAM_TEXSHADOWCOLOR			(15)
#define NND_FRAGPROGPARAM_TEXSHADOW2COLOR			(16)
#define NND_FRAGPROGPARAM_TEXUSER1PARAM				(17)
#define NND_FRAGPROGPARAM_TEXUSER2PARAM				(18)
#define NND_FRAGPROGPARAM_TEXUSER3PARAM				(19)
#define NND_FRAGPROGPARAM_TEXUSER4PARAM				(20)
#define NND_FRAGPROGPARAM_TEXUSER5PARAM				(21)
#define NND_FRAGPROGPARAM_TEXUSER6PARAM				(22)
#define NND_FRAGPROGPARAM_TEXUSER7PARAM				(23)
#define NND_FRAGPROGPARAM_TEXUSER8PARAM				(24)
#define NND_FRAGPROGPARAM_DUALPARABOLOIDMATRIX		(25)	/* 3 */
#define NND_FRAGPROGPARAM_USERUNIFORM				(28)	/* 0~256 */


#define nnInitShaderName(_name)		((_name).high = (_name).low = (Uint64)(-1))

// シェーダマネージャ
typedef struct {
	NNS_SHADER_NAME		Name;

	NNE_SHADERTYPE		ShaderType;

	GLhandleARB			ProgramObject;		// for GLSL

	GLuint				VertexProgram;		// for vertex_program
	GLuint				FragmentProgram;	// for fragment_program

	GLint			NumParallelLightLocation;
	GLint			NumPointLightLocation;
	GLint			NumSpotLightLocation;
	GLint			PointLightFallOffEndLocation;
	GLint			PointLightFallOffScaleLocation;
	GLint			SpotLightFallOffEndLocation;
	GLint			SpotLightFallOffScaleLocation;
	GLint			SpotLightAngleScaleLocation;

	GLint			TexBaseAlphaLocation;
	GLint			TexDecalAlphaLocation;
	GLint			TexDecal2AlphaLocation;
	GLint			TexDecal3AlphaLocation;
	GLint			TexShininessLevelLocation;
	GLint			TexDualParaboloidLevelLocation;
	GLint			TexAddLevelLocation;
	GLint			TexShadowColorLocation;
	GLint			TexShadow2ColorLocation;
	GLint			TexUser1ParamLocation;
	GLint			TexUser2ParamLocation;
	GLint			TexUser3ParamLocation;
	GLint			TexUser4ParamLocation;
	GLint			TexUser5ParamLocation;
	GLint			TexUser6ParamLocation;
	GLint			TexUser7ParamLocation;
	GLint			TexUser8ParamLocation;

	GLint			TexNormalSamplerLocation;
	GLint			TexBaseSamplerLocation;
	GLint			TexDecalSamplerLocation;
	GLint			TexDecal2SamplerLocation;
	GLint			TexDecal3SamplerLocation;
	GLint			TexSpecularSamplerLocation;
	GLint			TexShininessSamplerLocation;
	GLint			TexDualParaboloidSamplerLocation;
	GLint			TexEnvMaskSamplerLocation;
	GLint			TexModulateSamplerLocation;
	GLint			TexAddSamplerLocation;
	GLint			TexOpacitySamplerLocation;
	GLint			TexUser1SamplerLocation;
	GLint			TexUser2SamplerLocation;
	GLint			TexUser3SamplerLocation;
	GLint			TexUser4SamplerLocation;
	GLint			TexUser5SamplerLocation;
	GLint			TexUser6SamplerLocation;
	GLint			TexUser7SamplerLocation;
	GLint			TexUser8SamplerLocation;

	GLint			TexShadowSamplerLocation;
	GLint			TexShadow2SamplerLocation;

	GLint			TexUserSampler2D1Location;
	GLint			TexUserSampler2D2Location;
	GLint			TexUserSampler3D1Location;
	GLint			TexUserSampler3D2Location;
	GLint			TexUserSamplerCube1Location;
	GLint			TexUserSamplerCube2Location;

	GLint			DualParaboloidMatrixLocation;
	GLint			UserUniformLocation;
	GLint			PositionMatrixLocation;
	GLint			NormalMatrixLocation;

} NNS_SHADER_MANAGER;

/* ユニフォーム変数変更 */
void nnPutShaderUniformMatrices(const NNS_MATRIX *pMtxPal, const Uint16 *pMtxIdx, Sint32 nMtxIdx);

/* テクスチャタイプマスク取得 */
NNF_TEXTURETYPE nnGetTextureMask( NNF_DRAWOBJ flag );

#endif	// ( NND_PLATFORM == NND_PLATFORM_GL )


#if ( NND_PLATFORM == NND_PLATFORM_GLES11 )

// 環境マップ対応
typedef enum {
	NNE_TEXCOORDSRC_DISABLE,
	NNE_TEXCOORDSRC_TEXCOORD0,
	NNE_TEXCOORDSRC_TEXCOORD1,
	NNE_TEXCOORDSRC_NORMAL,
	NNE_TEXCOORDSRC_POSITION,
} NNE_TEXCOORDSRC;

void nnSetTexCoordSrc( NNE_TEXSLOT slot, NNE_TEXCOORDSRC src );
NNE_TEXCOORDSRC nnGetTexCoordSrc( NNE_TEXSLOT slot );
void nnSetNormalFormatType( Uint32 ftype );
Uint32 nnGetNormalFormatType();
void nnPutEnvironmentTextureMatrix( const NNS_MATRIX *pEnvMtx );


// OpenGL ⇔ OpenGL ES 互換

#define glColor3f( r, g, b )			glColor4f( (r), (g), (b), 1.f )
#define glColor3fv( p )					glColor4f( ((float*)(p))[0], ((float*)(p))[1], ((float*)(p))[2], 1.f )
#define glColor4fv( p )					glColor4f( ((float*)(p))[0], ((float*)(p))[1], ((float*)(p))[2], ((float*)(p))[3] )
#define glOrtho( l, r, b, t, n, f )		glOrthof( (GLfloat)(l), (GLfloat)(r), (GLfloat)(b), (GLfloat)(t), (GLfloat)(n), (GLfloat)(f) )

#if defined(GL_OES_blend_subtract)

#ifndef glBlendEquation
#define glBlendEquation( mode )			glBlendEquationOES( mode )
#endif
#ifndef GL_FUNC_ADD
#define GL_FUNC_ADD						GL_FUNC_ADD_OES
#endif
#ifndef GL_FUNC_SUBTRACT
#define GL_FUNC_SUBTRACT				GL_FUNC_SUBTRACT_OES
#endif
#ifndef GL_FUNC_REVERSE_SUBTRACT
#define GL_FUNC_REVERSE_SUBTRACT		GL_FUNC_REVERSE_SUBTRACT_OES
#endif

#else	// defined(GL_OES_blend_subtract)

#ifndef glBlendEquation
#define glBlendEquation( mode )			((void)0)
#endif
#ifndef GL_FUNC_ADD
#define GL_FUNC_ADD						0x8006
#endif
#ifndef GL_FUNC_SUBTRACT
#define GL_FUNC_SUBTRACT				0x800A
#endif
#ifndef GL_FUNC_REVERSE_SUBTRACT
#define GL_FUNC_REVERSE_SUBTRACT		0x800B
#endif

#endif	// defined(GL_OES_blend_subtract)

#endif	// ( NND_PLATFORM == NND_PLATFORM_GLES11 )

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__NNDGL_H__

/* End of file */
