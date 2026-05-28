/*---------------------------------------------------------------------------

    NN material header for OpenGL

    Copyright (C) 2004-2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN material format for OpenGL
    File    : nnfmaterialgl.h
    Create  : 2004/06/02
    Modify  : 2004/08/20 NNS_MATERIAL_TEXMAP_DESC変更
    Modify  : 2004/11/22 NN標準シェーダ用マテリアル追加
    Modify  : 2004/12/21 NN標準シェーダ用マテリアル変更
    Modify  : 2005/03/22 テクスチャタイプ追加
    Modify  : 2005/04/11 NNE_TEXCOORDTYPE_DUALPARABOLOID_MAP追加
    Modify  : 2005/04/12 NND_MATTYPE_GL_USERPROFILE追加
    Modify  : 2009/02/25 for OpenGL ES 1.1
    Version : 1.01.00
    Version : 0.00.00 for OpenGL ES 1.1
    Note    : 

---------------------------------------------------------------------------*/

#ifndef	__NNFMATERIALGL_H__
#define	__NNFMATERIALGL_H__

// Material type value
#define NND_MATTYPE_GL_MATRIEALDESC					((Uint32) 1 <<  0)	/* old */
#define NND_MATTYPE_GL_STDSHADERDESC				((Uint32) 1 <<  1)	/* no support for OpenGL ES 1.1 */
#define NND_MATTYPE_GL_USERPROFILE					((Uint32) 1 <<  2)	/* no support for OpenGL ES 1.1 */
#define NND_MATTYPE_GLES11_MATRIEALDESC				((Uint32) 1 <<  3)

// Material flag
typedef	Uint32 NNF_MATFLAG;

#define	NND_MATFLAG_DOUBLESIDE						((Uint32) 1 <<  0)
#define	NND_MATFLAG_DISABLE_LIGHTING				((Uint32) 1 <<  1)
#define	NND_MATFLAG_DISABLE_FOGGING					((Uint32) 1 <<  2)
#define	NND_MATFLAG_TWOSIDED_LIGHTING				((Uint32) 1 <<  3)	/* for vertex-lighting only */
#define	NND_MATFLAG_TWOSIDED_MATERIAL				((Uint32) 1 <<  4)	/* no support */

#define NND_MATFLAG_DISABLE_DEPTH_WRITING			((Uint32) 1 <<  8)
#define NND_MATFLAG_DISABLE_COLOR_R_WRITING			((Uint32) 1 <<  9)
#define NND_MATFLAG_DISABLE_COLOR_G_WRITING			((Uint32) 1 << 10)
#define NND_MATFLAG_DISABLE_COLOR_B_WRITING			((Uint32) 1 << 11)
#define NND_MATFLAG_DISABLE_COLOR_A_WRITING			((Uint32) 1 << 12)

#define	NND_MATFLAG_CALLBACK						((Uint32) 1 << 31)


// Material Color Flag
typedef Uint32 NNF_MATCOLFLAG;
#define NND_MATCOLFLAG_VTXCOL_MATERIAL				((Uint32) 1 <<  0)	/* old */
#define NND_MATCOLFLAG_SPECULAR_INTENSITY			((Uint32) 1 <<  1)

// Vertex Color Material
#define NND_VTXCOL_GL_AMBIENT_AND_DIFFUSE			0x1602	/* old */
#define NND_VTXCOL_GL_AMBIENT						0x1200	/* old */
#define NND_VTXCOL_GL_DIFFUSE						0x1201	/* old */
#define NND_VTXCOL_GL_SPECULAR						0x1202	/* old */
#define NND_VTXCOL_GL_EMISSION						0x1600	/* old */

// Material Color
typedef struct {
	NNF_MATCOLFLAG				fFlag;
	NNS_RGBA					Ambient;
	NNS_RGBA					Diffuse;
	NNS_RGBA					Specular;
	NNS_RGBA					Emission;
	Float						Shininess;
	Uint32						VtxColMaterial;
} NNS_MATERIAL_COLOR;		/* old */


// Logic Flag
typedef Uint32 NNF_MATLOGICFLAG;
#define NND_MATLOGICFLAG_ENABLE_BLEND				((Uint32) 1 <<  0)
#define NND_MATLOGICFLAG_BLEND_FUNC_SEPARATE		((Uint32) 1 <<  1)	/* no support for OpenGL ES 1.1 */
#define NND_MATLOGICFLAG_ENABLE_LOGIC_OP			((Uint32) 1 <<  2)
#define NND_MATLOGICFLAG_ENABLE_ALPHA_TEST			((Uint32) 1 <<  3)
#define NND_MATLOGICFLAG_ENABLE_DEPTH_TEST			((Uint32) 1 <<  4)

// glBlendFunc, glBlendFuncSeparate
#define NND_BLENDFUNC_GL_ZERO						0		// (0, 0, 0, 0)
#define NND_BLENDFUNC_GL_ONE						1		// (1, 1, 1, 1)
#define NND_BLENDFUNC_GL_SRC_COLOR					0x0300	// (Rs, Gs, Bs, As)
#define NND_BLENDFUNC_GL_ONE_MINUS_SRC_COLOR		0x0301	// (1-Rs, 1-Gs, 1-Bs, 1-As)
#define NND_BLENDFUNC_GL_SRC_ALPHA					0x0302	// (As, As, As, As)
#define NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA		0x0303	// (1-As, 1-As, 1-As, 1-As)
#define NND_BLENDFUNC_GL_DST_ALPHA					0x0304	// (Ad, Ad, Ad, Ad)
#define NND_BLENDFUNC_GL_ONE_MINUS_DST_ALPHA		0x0305	// (1-Ad, 1-Ad, 1-Ad, 1-Ad)
#define NND_BLENDFUNC_GL_DST_COLOR					0x0306	// (Rd, Gd, Bd, Ad)
#define NND_BLENDFUNC_GL_ONE_MINUS_DST_COLOR		0x0307	// (1-Rd, 1-Gd, 1-Bd, 1-Ad)
#define NND_BLENDFUNC_GL_CONSTANT_COLOR				0x8001	// (Rc, Gc, Bc, Ac)				/* no support for OpenGL ES 1.1 */
#define NND_BLENDFUNC_GL_ONE_MINUS_CONSTANT_COLOR	0x8002	// (1-Rc, 1-Gc, 1-Bc, 1-Ac)		/* no support for OpenGL ES 1.1 */
#define NND_BLENDFUNC_GL_CONSTANT_ALPHA				0x8003	// (Ac, Ac, Ac, Ac)				/* no support for OpenGL ES 1.1 */
#define NND_BLENDFUNC_GL_ONE_MINUS_CONSTANT_ALPHA	0x8004	// (1-Ac, 1-Ac, 1-Ac, 1-Ac)		/* no support for OpenGL ES 1.1 */
#define NND_BLENDFUNC_GL_SRC_ALPHA_SATURATE			0x0308	// (f, f, f, 1), f = min(As, 1-Ad)

// glBlendEquation
#define NND_BLENDOP_GL_MIN							0x8007	// Rr = min( Rs, Rd )	/* no support for OpenGL ES 1.1 */
#define NND_BLENDOP_GL_MAX							0x8008	// Rr = max( Rs, Rd )	/* no support for OpenGL ES 1.1 */
#define NND_BLENDOP_GL_FUNC_ADD						0x8006	// Rr = min( 1, Rs*sR + Rd*dR )
#define NND_BLENDOP_GL_FUNC_SUBTRACT				0x800A	// Rr = max( 0, Rs*sR - Rd*dR )
#define NND_BLENDOP_GL_FUNC_REVERSE_SUBTRACT		0x800B	// Rr = max( 0, Rd*dR - Rs*sR )

// glAlphaFunc, glDepthFunc
#define NND_CMPFUNC_GL_NEVER						0x0200	// 0
#define NND_CMPFUNC_GL_LESS							0x0201	// val <  ref
#define NND_CMPFUNC_GL_EQUAL						0x0202	// val == ref
#define NND_CMPFUNC_GL_LEQUAL						0x0203	// val <= ref
#define NND_CMPFUNC_GL_GREATER						0x0204	// val >  ref
#define NND_CMPFUNC_GL_NOTEQUAL						0x0205	// val != ref
#define NND_CMPFUNC_GL_GEQUAL						0x0206	// val >= ref
#define NND_CMPFUNC_GL_ALWAYS						0x0207	// 1

// glLogicOp
#define NND_LOGICOP_GL_CLEAR						0x1500	// 0
#define NND_LOGICOP_GL_AND							0x1501	// s & d
#define NND_LOGICOP_GL_AND_REVERSE					0x1502	// s & ~d
#define NND_LOGICOP_GL_COPY							0x1503	// s
#define NND_LOGICOP_GL_AND_INVERTED					0x1504	// ~s & d
#define NND_LOGICOP_GL_NOOP							0x1505	// d
#define NND_LOGICOP_GL_XOR							0x1506	// s ^ d
#define NND_LOGICOP_GL_OR							0x1507	// s | d
#define NND_LOGICOP_GL_NOR							0x1508	// ~(s | d)
#define NND_LOGICOP_GL_EQUIV						0x1509	// ~(s ^ d)
#define NND_LOGICOP_GL_INVERT						0x150A	// ~d
#define NND_LOGICOP_GL_OR_REVERSE					0x150B	// s | ~d
#define NND_LOGICOP_GL_COPY_INVERTED				0x150C	// ~s
#define NND_LOGICOP_GL_OR_INVERTED					0x150D	// ~s | d
#define NND_LOGICOP_GL_NAND							0x150E	// ~(s & d)
#define NND_LOGICOP_GL_SET							0x150F	// s

// Logic
typedef struct{
	NNF_MATLOGICFLAG			fFlag;
	Uint16						SrcFactorRGB;   // glBlendFunc, glBlendFuncSeparete
	Uint16						DstFactorRGB;   // glBlendFunc, glBlendFuncSeparete
	Uint16						SrcFactorA;     // glBlendFuncSeparete
	Uint16						DstFactorA;     // glBlendFuncSeparete
	NNS_RGBA					BlendColor;     // glBlendColor
	Uint16						BlendOp;        // glBlendEquation
	Uint16						LogicOp;        // glLogicOp
	Uint16						AlphaFunc;      // glAlphaFunc
	Uint16						DepthFunc;      // glDepthFunc
	Float						AlphaRef;       // glAlphaFunc
} NNS_MATERIAL_LOGIC;


// Texture Map Type
typedef Uint32 NNF_TEXMAPTYPE;											/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXMAPTYPE_ENVIRONMENT					((Uint32) 1 <<  0)	/* old */

#define NND_TEXMAPTYPE_TEXCOORD0					((Uint32) 1 <<  8)	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXMAPTYPE_TEXCOORD1					((Uint32) 1 <<  9)	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXMAPTYPE_TEXCOORD2					((Uint32) 1 << 10)	/* old */
#define NND_TEXMAPTYPE_TEXCOORD3					((Uint32) 1 << 11)	/* old */
#define NND_TEXMAPTYPE_TEXCOORD_MASK				\
	( NND_TEXMAPTYPE_TEXCOORD0 | NND_TEXMAPTYPE_TEXCOORD1 | NND_TEXMAPTYPE_TEXCOORD2 | \
	NND_TEXMAPTYPE_TEXCOORD3 | NND_TEXMAPTYPE_ENVIRONMENT )

#define	NND_TEXMAPTYPE_UV_SCALE						((Uint32) 1 << 16)	/* old */ /* Revive for OpenGL ES 1.1 */

#define	NND_TEXMAPTYPE_NO_UV_TRANSFORM				((Uint32) 1 << 30)	/* old */ /* Revive for OpenGL ES 1.1 */
#define	NND_TEXMAPTYPE_CALLBACK						((Uint32) 1 << 31)	/* old */


// Texture Env Mode
#define NND_TEXENVMODE_GL_REPLACE					0x1E01	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXENVMODE_GL_MODULATE					0x2100	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXENVMODE_GL_DECAL						0x2101	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXENVMODE_GL_BLEND						0x0BE2	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXENVMODE_GL_ADD						0x0104	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXENVMODE_GL_COMBINE					0x8570	/* old */ /* Revive for OpenGL ES 1.1 */


// Texture Combine
#define NND_TEXCOMBINE_GL_REPLACE					0x1E01	/* Arg0							*/	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXCOMBINE_GL_MODULATE					0x2100	/* Arg0*Arg1					*/	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXCOMBINE_GL_ADD						0x0104	/* Arg0+Arg1					*/	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXCOMBINE_GL_ADD_SIGNED				0x8574	/* Arg0+Arg1-0.5				*/	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXCOMBINE_GL_INTERPOLATE				0x8575	/* Arg0*Arg2+Arg1*(1-Arg2)		*/	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXCOMBINE_GL_SUBTRACT					0x84E7	/* Arg0-Arg1					*/	/* old */ /* Revive for OpenGL ES 1.1 */

#define NND_TEXCOMBINE_GL_DOT3_RGB					0x86AE	/* 4*((Arg0r-0.5)*(Arg1r-0.5)+	*/	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXCOMBINE_GL_DOT3_RGBA					0x86AF	/*    (Arg0g-0.5)*(Arg1g-0.5)+	*/	/* old */ /* Revive for OpenGL ES 1.1 */
															/*    (Arg0b-0.5)*(Arg1b-0.5))	*/	/* old */
// Texture Source
#define NND_TEXENVSRC_GL_TEXTURE					0x1702	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXENVSRC_GL_TEXTURE0					0x84C0	/* old */
#define NND_TEXENVSRC_GL_TEXTURE1					0x84C1	/* old */
#define NND_TEXENVSRC_GL_TEXTURE2					0x84C2	/* old */
#define NND_TEXENVSRC_GL_TEXTURE3					0x84C3	/* old */
#define NND_TEXENVSRC_GL_TEXTURE4					0x84C4	/* old */
#define NND_TEXENVSRC_GL_TEXTURE5					0x84C5	/* old */
#define NND_TEXENVSRC_GL_TEXTURE6					0x84C6	/* old */
#define NND_TEXENVSRC_GL_TEXTURE7					0x84C7	/* old */
#define NND_TEXENVSRC_GL_CONSTANT					0x8576	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXENVSRC_GL_PRIMARY_COLOR				0x8577	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXENVSRC_GL_PREVIOUS					0x8578	/* old */ /* Revive for OpenGL ES 1.1 */

// Texture Operand
#define NND_TEXENVOP_GL_SRC_COLOR					0x0300	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXENVOP_GL_ONE_MINUS_SRC_COLOR			0x0301	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXENVOP_GL_SRC_ALPHA					0x0302	/* old */ /* Revive for OpenGL ES 1.1 */
#define NND_TEXENVOP_GL_ONE_MINUS_SRC_ALPHA			0x0303	/* old */ /* Revive for OpenGL ES 1.1 */

typedef struct {
	Uint16						CombineRGB;
	Uint16						Source0RGB;
	Uint16						Operand0RGB;
	Uint16						Source1RGB;
	Uint16						Operand1RGB;
	Uint16						Source2RGB;
	Uint16						Operand2RGB;

	Uint16						CombineAlpha;
	Uint16						Source0Alpha;
	Uint16						Operand0Alpha;
	Uint16						Source1Alpha;
	Uint16						Operand1Alpha;
	Uint16						Source2Alpha;
	Uint16						Operand2Alpha;
} NNS_TEXTURE_COMBINE;		/* old */


// Wrap Mode
#define NND_WRAPMODE_GL_CLAMP						0x2900		/* no support for OpenGL ES 1.1 */
#define NND_WRAPMODE_GL_REPEAT						0x2901
#define NND_WRAPMODE_GL_MIRRORED_REPEAT				0x8370		/* extension GL_OES_texture_mirrored_repeat for OpenGL ES 1.1 */
#define NND_WRAPMODE_GL_CLAMP_TO_BORDER				0x812D		/* extension GL_OES_texture_mirrored_repeat for OpenGL ES 1.1 */
#define NND_WRAPMODE_GL_CLAMP_TO_EDGE				0x812F


// Filter mode
#define NND_TEXFILTER_GL_NEAREST					0x2600
#define NND_TEXFILTER_GL_LINEAR						0x2601
#define NND_TEXFILTER_GL_NEAREST_MIPMAP_NEAREST		0x2700
#define NND_TEXFILTER_GL_LINEAR_MIPMAP_NEAREST		0x2701
#define NND_TEXFILTER_GL_NEAREST_MIPMAP_LINEAR		0x2702
#define NND_TEXFILTER_GL_LINEAR_MIPMAP_LINEAR		0x2703

typedef struct {
	Uint16						MagFilter;
	Uint16						MinFilter;
	Float						Anisotropy;		/* extension GL_EXT_texture_filter_anisotropic for OpenGL ES 1.1 */
} NNS_TEXTURE_FILTERMODE;


// LOD Parameter
typedef struct {
	Sint32						BaseLevel;
	Sint32						MaxLevel;
	Float						MinLOD;
	Float						MaxLOD;
	Float						LODBias;
} NNS_TEXTURE_LOD_PARAM;


// Texture Mapping Descriptor
typedef struct {
	NNF_TEXMAPTYPE				fType;
	Sint32						iTexIdx;
	Sint32						EnvMode;
	NNS_TEXTURE_COMBINE			*pCombine;
	NNS_RGBA					EnvColor;
	NNS_TEXCOORD				Offset;
	NNS_TEXCOORD				Scale;
	Sint32						WrapS;
	Sint32						WrapT;
	NNS_RGBA					*pBorderColor;
	NNS_TEXTURE_FILTERMODE		*pFilterMode;
	NNS_TEXTURE_LOD_PARAM		*pLODParam;
	void						*pTexInfo;
	Uint32						Reserved1;
	Uint32						Reserved0;
} NNS_MATERIAL_TEXMAP_DESC;	/* old */

// Material Descriptor
typedef struct {
	NNF_MATFLAG					fFlag;
	Uint32						User;
	NNS_MATERIAL_COLOR			*pColor;
	NNS_MATERIAL_COLOR			*pBackColor;
	NNS_MATERIAL_LOGIC			*pLogic;
	Sint32						nTex;
	NNS_MATERIAL_TEXMAP_DESC	*pTexDesc;
} NNS_MATERIAL_DESC;		/* old */


// --- for NN Standard Shader ---

// Material Color
typedef struct {
	NNF_MATCOLFLAG				fFlag;
	NNS_RGBA					Ambient;
	NNS_RGBA					Diffuse;
	NNS_RGBA					Specular;
	NNS_RGBA					Emission;
	Float						Shininess;
	Float						SpecularIntensity;
} NNS_MATERIAL_STDSHADER_COLOR;

// Texture Type
typedef Uint32 NNF_TEXTURETYPE;
#define NND_TEXTURETYPE_NORMAL_MAP					((Uint32) 1 <<  0)
#define NND_TEXTURETYPE_BASE_MAP					((Uint32) 1 <<  1)
#define NND_TEXTURETYPE_DECAL_MAP					((Uint32) 1 <<  2)
#define NND_TEXTURETYPE_SPECULAR_MAP				((Uint32) 1 <<  3)
#define NND_TEXTURETYPE_MODULATE_MAP				((Uint32) 1 <<  4)
#define NND_TEXTURETYPE_ADD_MAP						((Uint32) 1 <<  5)
#define NND_TEXTURETYPE_OPACITY_MAP					((Uint32) 1 <<  6)

#define NND_TEXTURETYPE_DECAL2_MAP					((Uint32) 1 <<  7)		/* new */
#define NND_TEXTURETYPE_DECAL3_MAP					((Uint32) 1 <<  8)		/* new */
#define NND_TEXTURETYPE_SHININESS_MAP				((Uint32) 1 <<  9)		/* new */
#define NND_TEXTURETYPE_DUALPARABOLOID_MAP			((Uint32) 1 << 10)		/* new */
#define NND_TEXTURETYPE_ENVMASK_MAP					((Uint32) 1 << 11)		/* new */
#define NND_TEXTURETYPE_USER1_MAP					((Uint32) 1 << 12)		/* new */
#define NND_TEXTURETYPE_USER2_MAP					((Uint32) 1 << 13)		/* new */
#define NND_TEXTURETYPE_USER3_MAP					((Uint32) 1 << 14)		/* new */
#define NND_TEXTURETYPE_USER4_MAP					((Uint32) 1 << 15)		/* new */
#define NND_TEXTURETYPE_USER5_MAP					((Uint32) 1 << 16)		/* new */
#define NND_TEXTURETYPE_USER6_MAP					((Uint32) 1 << 17)		/* new */
#define NND_TEXTURETYPE_USER7_MAP					((Uint32) 1 << 18)		/* new */
#define NND_TEXTURETYPE_USER8_MAP					((Uint32) 1 << 19)		/* new */
#define NND_TEXTURETYPE_MAP_MASK					((Uint32) 0x000fffff)

#define NND_TEXTURETYPE_NORMAL_MAP_AXLY				((Uint32) 1 << 28)		/* new */
#define	NND_TEXTURETYPE_UV_SCALE					((Uint32) 1 << 29)
#define	NND_TEXTURETYPE_NO_UV_TRANSFORM				((Uint32) 1 << 30)
#define	NND_TEXTURETYPE_CALLBACK					((Uint32) 1 << 31)

typedef Sint32 NNS_TEXCOORDTYPE;
enum {
	NNE_TEXCOORDTYPE_TEXCOORD0			=  0,
	NNE_TEXCOORDTYPE_TEXCOORD1,
	NNE_TEXCOORDTYPE_TEXCOORD2,
	NNE_TEXCOORDTYPE_TEXCOORD3,

	NNE_TEXCOORDTYPE_SPHERE_MAP			= -1,
	NNE_TEXCOORDTYPE_PROJECTION_MAP		= -2,
	NNE_TEXCOORDTYPE_DUALPARABOLOID_MAP	= -3,
};

// Texture Mapping Descriptor
typedef struct {
	NNF_TEXTURETYPE				fType;
	Sint32						iTexIdx;
	NNS_TEXCOORDTYPE			TexCoord;
	Float						Blend;
	NNS_TEXCOORD				Offset;
	NNS_TEXCOORD				Scale;
	Sint32						WrapS;
	Sint32						WrapT;
	NNS_RGBA					*pBorderColor;
	NNS_TEXTURE_FILTERMODE		*pFilterMode;
	NNS_TEXTURE_LOD_PARAM		*pLODParam;
	void						*pTexInfo;
	Uint32						Reserved1;
	Uint32						Reserved0;
} NNS_MATERIAL_STDSHADER_TEXMAP_DESC;

// Material Descriptor
typedef struct {
	NNF_MATFLAG							fFlag;
	Uint32								User;
	NNS_MATERIAL_STDSHADER_COLOR		*pColor;
	NNS_MATERIAL_LOGIC					*pLogic;
	NNF_TEXTURETYPE						fTexType;
	Sint32								nTex;
	NNS_MATERIAL_STDSHADER_TEXMAP_DESC	*pTexDesc;
} NNS_MATERIAL_STDSHADER_DESC;

typedef struct {
	NNF_MATFLAG							fFlag;
	Uint32								User;
	NNS_MATERIAL_STDSHADER_COLOR		*pColor;
	NNS_MATERIAL_LOGIC					*pLogic;
	NNF_TEXTURETYPE						fTexType;
	Sint32								nTex;
	NNS_MATERIAL_STDSHADER_TEXMAP_DESC	*pTexDesc;
	Uint32								UserProfile;
} NNS_MATERIAL_STDSHADER_DESC_USER_PROFILE;


// --- for OpenGL ES 1.1 ---

// Material Color
typedef NNS_MATERIAL_STDSHADER_COLOR NNS_MATERIAL_GLES11_COLOR;

// Logic
typedef struct {
	NNF_MATLOGICFLAG			fFlag;
	Uint16						SrcFactor;      // glBlendFunc
	Uint16						DstFactor;      // glBlendFunc
	Uint16						BlendOp;        // glBlendEquation
	Uint16						LogicOp;        // glLogicOp
	Uint16						AlphaFunc;      // glAlphaFunc
	Uint16						DepthFunc;      // glDepthFunc
	Float						AlphaRef;       // glAlphaFunc
} NNS_MATERIAL_GLES11_LOGIC;

typedef struct {
	Uint16						CombineRGB;
	Uint16						Source0RGB;
	Uint16						Operand0RGB;
	Uint16						Source1RGB;
	Uint16						Operand1RGB;
	Uint16						Source2RGB;
	Uint16						Operand2RGB;

	Uint16						CombineAlpha;
	Uint16						Source0Alpha;
	Uint16						Operand0Alpha;
	Uint16						Source1Alpha;
	Uint16						Operand1Alpha;
	Uint16						Source2Alpha;
	Uint16						Operand2Alpha;

	NNS_RGBA					EnvColor;
} NNS_TEXTURE_GLES11_COMBINE;		/* old */


// Texture Mapping Descriptor
typedef struct {
	NNF_TEXMAPTYPE				fType;
	Sint32						iTexIdx;
	Sint32						EnvMode;
	NNS_TEXTURE_GLES11_COMBINE	*pCombine;
	NNS_TEXCOORD				Offset;
	NNS_TEXCOORD				Scale;
	Sint32						WrapS;
	Sint32						WrapT;
	NNS_TEXTURE_FILTERMODE		*pFilterMode;
	Float						LODBias;
	void						*pTexInfo;
} NNS_MATERIAL_GLES11_TEXMAP_DESC;

// Material Descriptor
typedef struct {
	NNF_MATFLAG						fFlag;
	Uint32							User;
	NNS_MATERIAL_GLES11_COLOR		*pColor;
	NNS_MATERIAL_GLES11_LOGIC		*pLogic;
	Sint32							nTex;
	NNS_MATERIAL_GLES11_TEXMAP_DESC	*pTexDesc;
} NNS_MATERIAL_GLES11_DESC;

#endif	//__NNFMATERIALGL_H__

/* End of file */
