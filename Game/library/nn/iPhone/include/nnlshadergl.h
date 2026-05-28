/*---------------------------------------------------------------------------

    NN  High level graphics library

    Copyright (C) 2004-2007 SEGA Corporation Creative Center Graphics Sect.
    All Rights Reserved.

    Module  : NN Standard Shader
    File    : nnlshadergl.h
    Create  : 2004/11/10
    Modify  : 2005/03/23 標準シェーダ変更・テクスチャ種類追加など
    Modify  : 2005/03/28 シェーダプロファイル構造体大幅変更
    Modify  : 2005/03/28 シャドウマップ2枚対応、デュアルパラボロイドマトリクス設定関数追加、
    Modify  : 2005/03/29 シェーダプロファイル関連バグ修正
    Modify  : 2005/04/12 各テクスチャマップのデュアルパラボロイド貼り対応
    Modify  : 2005/04/12 ユーザプロファイル対応
    Modify  : 2005/04/22 シェーダエンベロープ計算対応
    Modify  : 2005/06/10 描画フラグユーザープロファイル追加
    Modify  : 2005/06/17 bHalfFloat,bNoScaleEnvelope追加
    Modify  : 2006/05/16 Cgコンパイラで事前コンパイルするためのAPI追加
    Modify  : 2006/06/05 Cgコンパイラパスつきコマンド文字列作成関数追加
	Mofify  : 2007/12/05 nVertexMatrixIndexがNNE_BOOLになっていたのをSint32に修正
    Version : 1.04.04
    Note    : 

---------------------------------------------------------------------------*/
#ifndef	__NNLSHADERGL_H__
#define	__NNLSHADERGL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* フォグモデル */
typedef enum {
	NNE_FOG_NONE,
	NNE_FOG_LINEAR,
	NNE_FOG_EXP,
	NNE_FOG_EXP2,
} NNE_FOG_MODEL;

/* ライト距離減衰モデル */
typedef enum {
	NNE_ATTEN_CONSTANT,
	NNE_ATTEN_INVLINEAR,
	NNE_ATTEN_INVQUADRATIC,
} NNE_ATTEN_FUNC;

/* 標準シェーダ設定 */
typedef struct {
	NNE_BOOL		bNormalizeVertexNormal;		// 法線正規化(Vertex Shader)
	NNE_BOOL		bRescaleVertexNormal;		// 法線再スケール(Vertex Shader)
	Sint32			nMaxParallelLight;			// 最大パラレルライト数(0～4)
	Sint32			nMaxPointLight;				// 最大ポイントライト数(0～4)
	Sint32			nMaxSpotLight;				// 最大スポットライト数(0～4)
	NNE_BOOL		bLightAmbient;				// ライトアンビエント有効
	NNE_ATTEN_FUNC	PointLightDistAtten;		// ポイントライト距離減衰モデル
	NNE_ATTEN_FUNC	SpotLightDistAtten;			// スポットライト距離減衰モデル
	NNE_FOG_MODEL	FogModel;					// フォグモデル
	NNE_BOOL		bDistanceFog;				// 距離フォグ or Z値フォグ
	NNE_BOOL		bFragmentFog;				// フラグメント単位のフォグ計算

	Uint32			nUserUniform;				// ユーザユニフォーム変数の個数

	NNE_BOOL		bHalfFloat;					// halfを使う（デフォルトでTRUE）
	NNE_BOOL		bNoScaleEnvelope;			// エンベロープの法線計算にスケールを考慮しない

	NNE_BOOL		bVertexSpecular;			// 頂点スペキュラカラー使用
	NNE_BOOL		bCalcBinormal;				// 法線と接線から従法線計算
} NNS_SHADER_CONFIG;

/* テクスチャ座標 */
typedef Sint32 NNS_TEXCOORDIDX;
enum {
	NNE_TEXCOORD_NONE	= 0,	// テクスチャなしの場合に指定（シェーダプロファイルの種類を減らすため）

	NNE_TEXCOORD_0		= 0,
	NNE_TEXCOORD_1,
	NNE_TEXCOORD_2,
	NNE_TEXCOORD_3,

	NNE_TEXCOORD_NRM	= -1,
	NNE_TEXCOORD_POS	= -2,
	NNE_TEXCOORD_SPHERE_MAP			= -1,
	NNE_TEXCOORD_PROJECTION_MAP		= -2,
	NNE_TEXCOORD_DUALPARABOLOID_MAP	= -3,

	NNE_TEXCOORD_MAX	= NNE_TEXCOORD_3,
	NNE_TEXCOORD_MIN	= NNE_TEXCOORD_DUALPARABOLOID_MAP,
};

#define NND_FRAGPARALIGHT_MAX		(3)
#define NND_FRAGPOINTLIGHT_MAX		(3)

/* シェーダプロファイル */
typedef struct {
	NNE_BOOL			bLighting;					// 頂点ライティング
	NNE_BOOL			bSpecular;					// スペキュラ
	NNE_BOOL			bTwoSidedLighting;			// ２面ライティング

	Sint32				nFragParallelLight;			// フラグメント単位パラレルライト数
	Sint32				nFragPointLight;			// フラグメント単位ポイントライト数

	// テクスチャ使用
	Sint32				NormalMapType;				// 0:なし 1:RGB->XYZ 2:AXLY
	NNE_BOOL			bBaseMap;
	Sint32				nDecalMap;					// 0～3
	NNE_BOOL			bSpecularMap;
	NNE_BOOL			bShininessMap;
	NNE_BOOL			bDualParaboloidMap;
	NNE_BOOL			bEnvMaskMap;
	NNE_BOOL			bModulateMap;
	NNE_BOOL			bAddMap;
	NNE_BOOL			bOpacityMap;
	NNE_BOOL			bUser1Map;
	NNE_BOOL			bUser2Map;
	NNE_BOOL			bUser3Map;
	NNE_BOOL			bUser4Map;
	NNE_BOOL			bUser5Map;
	NNE_BOOL			bUser6Map;
	NNE_BOOL			bUser7Map;
	NNE_BOOL			bUser8Map;
	Sint32				nShadowMap;					// 0～2
	NNE_BOOL			bUserSampler2D1;
	NNE_BOOL			bUserSampler2D2;
	NNE_BOOL			bUserSampler3D1;
	NNE_BOOL			bUserSampler3D2;
	NNE_BOOL			bUserSamplerCube1;
	NNE_BOOL			bUserSamplerCube2;

	// テクスチャ座標（上記のテクスチャの順（DualParaboloid,Shadow,UserSampler除く）で使用するもののみ）
	NNS_TEXCOORDIDX		TexCoord[8];

	Uint32				UserProfile;				// マテリアルユーザープロファイル 0～15
	Uint32				UserProfileDrawobj;			// 描画フラグユーザープロファイル 0～15

	Sint32				nVertexMatrixIndex;			// 頂点マトリクス数（シェーダエンベロープ計算時）

} NNS_SHADER_PROFILE;

#define NND_MAX_USER_PROFILE			(63)		// マテリアルユーザープロファイル最大値
#define NND_MAX_USER_PROFILE_DRAWOBJ	(255)		// 描画フラグユーザープロファイル最大値
#define NND_MAX_NUM_USER_UNIFORM		(256)		// ユーザユニフォーム変数最大個数（実際の制限値はシェーダやドライバによる）

// シェーダ名（シェーダプロファイルに1対1対応する値。）
typedef struct {
	Uint64 low;
	Uint64 high;
} NNS_SHADER_NAME;

/* 標準シェーダ管理バッファサイズ取得 */
Uint32 nnCalcShaderManageBufferSizeGL( Sint32 num );

/* 標準シェーダ基本設定 */
void nnSetUpShaderConfigBasicGL( NNS_SHADER_CONFIG *config );

/* 標準シェーダ初期化関数 */
void nnConfigureShaderGL( const NNS_SHADER_CONFIG *config, void *managebuffer, Sint32 num );

/* シェーダプロファイル初期化 */
void nnInitShaderProfileGL( NNS_SHADER_PROFILE *profile );

/* シェーダネーム取得 */
NNS_SHADER_NAME nnGetShaderNameGL( const NNS_SHADER_PROFILE *profile );
/* シェーダプロファイル取得 */
void nnGetShaderProfileGL( NNS_SHADER_PROFILE *profile, NNS_SHADER_NAME Name );

/* シェーダプロファイル登録 */
Sint32 nnRegistShaderProfileGL( const NNS_SHADER_PROFILE *profile );
Sint32 nnRegistShaderNameGL( NNS_SHADER_NAME Name );

/* オブジェクト使用シェーダプロファイル登録 */
Sint32 nnRegistObjectShaderProfilesGL( const NNS_OBJECT *obj, NNF_DRAWOBJ flag );

/* 登録済みシェーダプロファイル数取得 */
Sint32 nnGetCurrentShaderProfileNumberGL( void );

/* 登録済みシェーダプロファイル取得 */
void nnGetShaderProfileOneGL( NNS_SHADER_PROFILE *profile, Sint32 idx );

/* 登録済みシェーダネームリスト取得 */
const NNS_SHADER_NAME *nnGetShaderNameListGL( void );

/* 登録済みシェーダプロファイル削除 */
void nnClearShaderProfilesGL( void );


/*** NN外部シェーダビルド用 ***/

	/* Cgコンパイラ用コマンド最大長 */
	#define NND_SHADER_MAX_CGC_COMMAND_SIZE		(4096)
	/* GLSL標準シェーダヘッダ文字列最大長 */
	#define NND_SHADER_MAX_HEADER_SIZE			(2800)

	/* Cgコンパイラ用コンパイルコマンド作成（頂点シェーダ） */
	void nnMakeCompileVertexShaderCommandForCgcGL(char *pCommandBuffer,
			const NNS_SHADER_PROFILE *profile, const char *outfname, const char *srcfname, const char *includePath);
	void nnMakeCompileVertexShaderCommandForCgcWithPathGL(char *pCommandBuffer,
			const NNS_SHADER_PROFILE *profile, const char *outfname, const char *srcfname, const char *includePath, const char *cgcPath);
	/* Cgコンパイラ用コンパイルコマンド作成（フラグメントシェーダ） */
	void nnMakeCompileFragmentShaderCommandForCgcGL(char *pCommandBuffer,
			const NNS_SHADER_PROFILE *profile, const char *outfname, const char *srcfname, const char *includePath);
	void nnMakeCompileFragmentShaderCommandForCgcWithPathGL(char *pCommandBuffer,
			const NNS_SHADER_PROFILE *profile, const char *outfname, const char *srcfname, const char *includePath, const char *cgcPath);

	/* GLSL標準シェーダヘッダ文字列作成 */
	void nnMakeStdShaderHeaderGL(char *pHeaderBuffer, const NNS_SHADER_PROFILE *profile);
	/* GLSL標準シェーダ頂点アトリビュートバインド */
	void nnBindVertexAttributeGL( GLhandleARB program );

	// シェーダタイプ
	typedef enum {
		NNE_SHADERTYPE_NONE,
		NNE_SHADERTYPE_GLSL,
		NNE_SHADERTYPE_VP_FP,
	} NNE_SHADERTYPE;

	// コンパイル済みシェーダプロファイル
	typedef struct {
		NNE_SHADERTYPE		ShaderType;

		GLhandleARB			ProgramObject;		// for GLSL

		GLuint				VertexProgram;		// for vertex_program
		GLuint				FragmentProgram;	// for fragment_program
	} NNS_COMPILED_SHADER_PROFILE;

	/* 未ビルドシェーダ数取得 */
	Sint32 nnGetUnbuildShaderProfileNumberGL( void );

	/* 未ビルドシェーダプロファイル取得 */
	Sint32 nnGetUnbuildShaderProfileOneGL( NNS_SHADER_PROFILE *profile );

	/* コンパイル済みシェーダ登録 */
	void nnRegistCompiledShaderProfileGL(const NNS_COMPILED_SHADER_PROFILE *compiledShader, const NNS_SHADER_PROFILE *profile);


/*** NN内部シェーダビルド用 ***/

	/* 標準シェーダビルド用ワークバッファサイズ取得 */
	Uint32 nnCalcBuildShaderWorkBufferSizeGL( Uint32 vtxshadersize, Uint32 fragshadersize );

	/* 標準シェーダビルド */
	Sint32 nnBuildShaderGL( void *workbuffer, const GLcharARB *vertexshader,  Uint32 vtxshadersize,
						const GLcharARB *fragmentshader, Uint32 fragshadersize );

	/* ビルドエラープログラムオブジェクト取得 */
	GLhandleARB nnGetErrorVertexShaderObjectGL( void );
	GLhandleARB nnGetErrorFragmentShaderObjectGL( void );
	GLhandleARB nnGetErrorShaderProgramObjectGL( void );
	/* ビルドエラーソース取得 */
	GLcharARB *nnGetErrorVertexShaderSourceGL( void );
	GLcharARB *nnGetErrorFragmentShaderSourceGL( void );


/* ビルド済み標準シェーダ解放 */
void nnReleaseShaderGL( void );

/* ユーザユニフォーム変数設定 */
void nnSetUserUniformGL(Sint32 idx, Float x, Float y, Float z, Float w);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNLSHADERGL_H__ */

/* End of file */
