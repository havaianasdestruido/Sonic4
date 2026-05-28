/*---------------------------------------------------------------------------

    NN Interleaved Vertex Format for OpenGL

    Copyright(C)SEGA All Rights Reserved.
    CS R&D suport Dept.

    Module  : NN Interleaved Vertex Format
    File    : nntvertexgl.h
    Create  : 2009/09/29
    Modify  : 
    Version : 1.04.06 for LINDBERGH
    Version : 1.00.09 for OpenGL ES 1.1
    Note    : 

---------------------------------------------------------------------------*/

#ifndef	__NNTVERTEXGL_H__
#define	__NNTVERTEXGL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*****************************/
/* Interleaved Vertex Format */
/*****************************/
#if 0
// macro for VisualStudio

#define NNM_VTXFORM_POS_S4		Sint16 x, y, z, w;
#define NNM_VTXFORM_POS_F3		Float x, y, z;
#define NNM_VTXFORM_WGT_O
#define NNM_VTXFORM_WGT_UB1		Uint8 w0;
#define NNM_VTXFORM_WGT_UB2		Uint8 w0, w1;
#define NNM_VTXFORM_WGT_UB3		Uint8 w0, w1, w2;
#define NNM_VTXFORM_WGT_UB4		Uint8 w0, w1, w2, w4;
#define NNM_VTXFORM_WGT_F1		Float w0;
#define NNM_VTXFORM_WGT_F2		Float w0, w1;
#define NNM_VTXFORM_WGT_F3		Float w0, w1, w2;
#define NNM_VTXFORM_IDX_O
#define NNM_VTXFORM_IDX_UB1		Uint8 i0;
#define NNM_VTXFORM_IDX_UB2		Uint8 i0, i1;
#define NNM_VTXFORM_IDX_UB3		Uint8 i0, i1, i2;
#define NNM_VTXFORM_IDX_UB4		Uint8 i0, i1, i2, i3;
#define NNM_VTXFORM_NRM_O
#define NNM_VTXFORM_NRM_B3		Sint8 nx, ny, nz;
#define NNM_VTXFORM_NRM_S3		Sint16 nx, ny, nz;
#define NNM_VTXFORM_NRM_F3		Float nx, ny, nz;
#define NNM_VTXFORM_TAN_O
#define NNM_VTXFORM_TAN_B3		Sint8 tx, ty, tz;
#define NNM_VTXFORM_TAN_S3		Sint16 tx, ty, tz;
#define NNM_VTXFORM_TAN_F3		Float tx, ty, tz;
#define NNM_VTXFORM_BNRM_O
#define NNM_VTXFORM_BNRM_B3		Sint8 bx, by, bz;
#define NNM_VTXFORM_BNRM_S3		Sint16 bx, by, bz;
#define NNM_VTXFORM_BNRM_F3		Float bx, by, bz;
#define NNM_VTXFORM_COL_O
#define NNM_VTXFORM_COL_UB3		Uint8 r, g, b;
#define NNM_VTXFORM_COL_UB4		Uint8 r, g, b, a;
#define NNM_VTXFORM_COL2_O
#define NNM_VTXFORM_COL2_UB3	Uint8 r2, g2, b2;
#define NNM_VTXFORM_TEX0_O
#define NNM_VTXFORM_TEX0_S2		Sint16 u0, v0;
#define NNM_VTXFORM_TEX0_F2		Float u0, v0;
#define NNM_VTXFORM_TEX1_O
#define NNM_VTXFORM_TEX1_S2		Sint16 u1, v1;
#define NNM_VTXFORM_TEX1_F2		Float u1, v1;
#define NNM_VTXFORM_TEX2_O
#define NNM_VTXFORM_TEX2_S2		Sint16 u2, v2;
#define NNM_VTXFORM_TEX2_F2		Float u2, v2;
#define NNM_VTXFORM_TEX3_O
#define NNM_VTXFORM_TEX3_S2		Sint16 u3, v3;
#define NNM_VTXFORM_TEX3_F2		Float u3, v3;

#define NNM_VTXNAME_POS_S4		_PS4
#define NNM_VTXNAME_POS_F3		_PF3
#define NNM_VTXNAME_WGT_O
#define NNM_VTXNAME_WGT_UB1		_WUB1
#define NNM_VTXNAME_WGT_UB2		_WUB2
#define NNM_VTXNAME_WGT_UB3		_WUB3
#define NNM_VTXNAME_WGT_UB4		_WUB4
#define NNM_VTXNAME_WGT_F1		_WF1
#define NNM_VTXNAME_WGT_F2		_WF2
#define NNM_VTXNAME_WGT_F3		_WF3
#define NNM_VTXNAME_IDX_O
#define NNM_VTXNAME_IDX_UB1		_IUB1
#define NNM_VTXNAME_IDX_UB2		_IUB2
#define NNM_VTXNAME_IDX_UB3		_IUB3
#define NNM_VTXNAME_IDX_UB4		_IUB4
#define NNM_VTXNAME_NRM_O
#define NNM_VTXNAME_NRM_B3		_NB3
#define NNM_VTXNAME_NRM_S3		_NS3
#define NNM_VTXNAME_NRM_F3		_NF3
#define NNM_VTXNAME_TAN_O
#define NNM_VTXNAME_TAN_B3		_TB3
#define NNM_VTXNAME_TAN_S3		_TS3
#define NNM_VTXNAME_TAN_F3		_TF3
#define NNM_VTXNAME_BNRM_O
#define NNM_VTXNAME_BNRM_B3		_BB3
#define NNM_VTXNAME_BNRM_S3		_BS3
#define NNM_VTXNAME_BNRM_F3		_BF3
#define NNM_VTXNAME_COL_O
#define NNM_VTXNAME_COL_UB3		_CUB3
#define NNM_VTXNAME_COL_UB4		_CUB4
#define NNM_VTXNAME_COL2_O
#define NNM_VTXNAME_COL2_UB3	_C2UB3
#define NNM_VTXNAME_TEX0_O
#define NNM_VTXNAME_TEX0_S2		_T0S2
#define NNM_VTXNAME_TEX0_F2		_T0F2
#define NNM_VTXNAME_TEX1_O
#define NNM_VTXNAME_TEX1_S2		_T1S2
#define NNM_VTXNAME_TEX1_F2		_T1F2
#define NNM_VTXNAME_TEX2_O
#define NNM_VTXNAME_TEX2_S2		_T2S2
#define NNM_VTXNAME_TEX2_F2		_T2F2
#define NNM_VTXNAME_TEX3_O
#define NNM_VTXNAME_TEX3_S2		_T3S2
#define NNM_VTXNAME_TEX3_F2		_T3F2


#define NNM_VTXNAME_HEAD()			NNS_VERTEX_GL
#define NNM_VTXNAME_POS( form )		NNM_VTXNAME_POS_##form
#define NNM_VTXNAME_WGT( form )		NNM_VTXNAME_WGT_##form
#define NNM_VTXNAME_IDX( form )		NNM_VTXNAME_IDX_##form
#define NNM_VTXNAME_NRM( form )		NNM_VTXNAME_NRM_##form
#define NNM_VTXNAME_TAN( form )		NNM_VTXNAME_TAN_##form
#define NNM_VTXNAME_BNRM( form )	NNM_VTXNAME_BNRM_##form
#define NNM_VTXNAME_COL( form )		NNM_VTXNAME_COL_##form
#define NNM_VTXNAME_COL2( form )	NNM_VTXNAME_COL2_##form
#define NNM_VTXNAME_TEX0( form )	NNM_VTXNAME_TEX0_##form
#define NNM_VTXNAME_TEX1( form )	NNM_VTXNAME_TEX1_##form
#define NNM_VTXNAME_TEX2( form )	NNM_VTXNAME_TEX2_##form
#define NNM_VTXNAME_TEX3( form )	NNM_VTXNAME_TEX3_##form


#define NNM_VTXNAME( pform, wform, iform, nform, tform, bform, cform, c2form, t0form, t1form, t2form, t3form ) \
	NNM_VTXNAME_HEAD() ## \
	NNM_VTXNAME_POS(pform) ## NNM_VTXNAME_WGT(wform) ## NNM_VTXNAME_IDX(iform) ## \
	NNM_VTXNAME_NRM(nform) ## NNM_VTXNAME_TAN(tform) ## NNM_VTXNAME_BNRM(bform) ## \
	NNM_VTXNAME_COL(cform) ## NNM_VTXNAME_COL2(c2form) ## \
	NNM_VTXNAME_TEX0(t0form) ## NNM_VTXNAME_TEX1(t1form) ## NNM_VTXNAME_TEX2(t2form) ## NNM_VTXNAME_TEX3(t3form)


#define NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, t0form, t1form, t2form, t3form )	\
typedef struct { \
	NNM_VTXFORM_POS_##pform \
	NNM_VTXFORM_WGT_##wform \
	NNM_VTXFORM_IDX_##iform \
	NNM_VTXFORM_NRM_##nform \
	NNM_VTXFORM_TAN_##tform \
	NNM_VTXFORM_BNRM_##bform \
	NNM_VTXFORM_COL_##cform \
	NNM_VTXFORM_COL2_##c2form \
	NNM_VTXFORM_TEX0_##t0form \
	NNM_VTXFORM_TEX1_##t1form \
	NNM_VTXFORM_TEX2_##t2form \
	NNM_VTXFORM_TEX3_##t3form \
} NNM_VTXNAME( pform, wform, iform, nform, tform, bform, cform, c2form, t0form, t1form, t2form, t3form );


#if ( NND_PLATFORM == NND_PLATFORM_GLES11 )
#define NNM_TYPEDEF_VERTEX_ALL_TEX( pform, wform, iform, nform, tform, bform, cform, c2form ) \
	NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, O, O, O, O ) \
	NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, S2, O, O, O ) \
	NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, S2, S2, O, O ) \
	NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, F2, O, O, O ) \
	NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, F2, F2, O, O )
#else
#define NNM_TYPEDEF_VERTEX_ALL_TEX( pform, wform, iform, nform, tform, bform, cform, c2form ) \
	NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, O, O, O, O ) \
	NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, S2, O, O, O ) \
	NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, S2, S2, O, O ) \
	NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, S2, S2, S2, O ) \
	NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, S2, S2, S2, S2 ) \
	NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, F2, O, O, O ) \
	NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, F2, F2, O, O ) \
	NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, F2, F2, F2, O ) \
	NNM_TYPEDEF_VERTEX( pform, wform, iform, nform, tform, bform, cform, c2form, F2, F2, F2, F2 )
#endif

#if ( NND_PLATFORM == NND_PLATFORM_GLES11 )
#define NNM_TYPEDEF_VERTEX_ALL_COL_TEX( pform, wform, iform, nform, tform, bform ) \
	NNM_TYPEDEF_VERTEX_ALL_TEX( pform, wform, iform, nform, tform, bform, O, O ) \
	NNM_TYPEDEF_VERTEX_ALL_TEX( pform, wform, iform, nform, tform, bform, UB4, O )
#else
#define NNM_TYPEDEF_VERTEX_ALL_COL_TEX( pform, wform, iform, nform, tform, bform ) \
	NNM_TYPEDEF_VERTEX_ALL_TEX( pform, wform, iform, nform, tform, bform, O, O ) \
	NNM_TYPEDEF_VERTEX_ALL_TEX( pform, wform, iform, nform, tform, bform, UB3, O ) \
	NNM_TYPEDEF_VERTEX_ALL_TEX( pform, wform, iform, nform, tform, bform, UB3, UB3 ) \
	NNM_TYPEDEF_VERTEX_ALL_TEX( pform, wform, iform, nform, tform, bform, UB4, O ) \
	NNM_TYPEDEF_VERTEX_ALL_TEX( pform, wform, iform, nform, tform, bform, UB4, UB3 )
#endif

#if ( NND_PLATFORM == NND_PLATFORM_GLES11 )
#define NNM_TYPEDEF_VERTEX_ALL_NRM_COL_TEX( pform, wform, iform ) \
	NNM_TYPEDEF_VERTEX_ALL_COL_TEX( pform, wform, iform, O, O, O ) \
	NNM_TYPEDEF_VERTEX_ALL_COL_TEX( pform, wform, iform, B3, O, O ) \
	NNM_TYPEDEF_VERTEX_ALL_COL_TEX( pform, wform, iform, S3, O, O ) \
	NNM_TYPEDEF_VERTEX_ALL_COL_TEX( pform, wform, iform, F3, O, O )
#else
#define NNM_TYPEDEF_VERTEX_ALL_NRM_COL_TEX( pform, wform, iform ) \
	NNM_TYPEDEF_VERTEX_ALL_COL_TEX( pform, wform, iform, O, O, O ) \
	NNM_TYPEDEF_VERTEX_ALL_COL_TEX( pform, wform, iform, B3, O, O ) \
	NNM_TYPEDEF_VERTEX_ALL_COL_TEX( pform, wform, iform, B3, B3, B3 ) \
	NNM_TYPEDEF_VERTEX_ALL_COL_TEX( pform, wform, iform, S3, O, O ) \
	NNM_TYPEDEF_VERTEX_ALL_COL_TEX( pform, wform, iform, S3, S3, S3 ) \
	NNM_TYPEDEF_VERTEX_ALL_COL_TEX( pform, wform, iform, F3, O, O ) \
	NNM_TYPEDEF_VERTEX_ALL_COL_TEX( pform, wform, iform, F3, F3, F3 )
#endif

#if ( NND_PLATFORM == NND_PLATFORM_GLES11 )
#define NNM_TYPEDEF_VERTEX_ALL_WGT_IDX_NRM_COL_TEX( pform ) \
	NNM_TYPEDEF_VERTEX_ALL_NRM_COL_TEX( pform, O, O ) \
	NNM_TYPEDEF_VERTEX_ALL_NRM_COL_TEX( pform, F1, UB1 ) \
	NNM_TYPEDEF_VERTEX_ALL_NRM_COL_TEX( pform, F2, UB2 ) \
	NNM_TYPEDEF_VERTEX_ALL_NRM_COL_TEX( pform, F3, UB3 )
#else
#define NNM_TYPEDEF_VERTEX_ALL_WGT_IDX_NRM_COL_TEX( pform ) \
	NNM_TYPEDEF_VERTEX_ALL_NRM_COL_TEX( pform, O, O ) \
	NNM_TYPEDEF_VERTEX_ALL_NRM_COL_TEX( pform, O, UB1 ) \
	NNM_TYPEDEF_VERTEX_ALL_NRM_COL_TEX( pform, UB2, UB2 ) \
	NNM_TYPEDEF_VERTEX_ALL_NRM_COL_TEX( pform, UB3, UB3 ) \
	NNM_TYPEDEF_VERTEX_ALL_NRM_COL_TEX( pform, UB4, UB4 )
#endif

#define NNM_TYPEDEF_VERTEX_ALL_POS_WGT_IDX_NRM_COL_TEX() \
	NNM_TYPEDEF_VERTEX_ALL_WGT_IDX_NRM_COL_TEX( S4 ) \
	NNM_TYPEDEF_VERTEX_ALL_WGT_IDX_NRM_COL_TEX( F3 )


NNM_TYPEDEF_VERTEX_ALL_POS_WGT_IDX_NRM_COL_TEX()

#else

// define all

#if ( NND_PLATFORM == NND_PLATFORM_GLES11 )

typedef struct {
	Sint16 x, y, z, w;
} NNS_VERTEX_GL_PS4;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PS4_NB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_NB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PS4_NS3;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NS3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NS3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NS3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NS3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_NS3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NS3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NS3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PS4_NF3;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NF3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NF3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NF3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NF3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_NF3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NF3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NF3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
} NNS_VERTEX_GL_PS4_WF1_IUB1;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WF1_IUB1_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NB3;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NS3;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NS3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NS3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NS3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NS3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NS3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NS3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NS3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NF3;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NF3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NF3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NF3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NF3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NF3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NF3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NF3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF1_IUB1_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
} NNS_VERTEX_GL_PS4_WF2_IUB2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WF2_IUB2_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NB3;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NS3;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NS3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NS3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NS3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NS3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NS3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NS3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NS3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NF3;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NF3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NF3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NF3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NF3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NF3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NF3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NF3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF2_IUB2_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
} NNS_VERTEX_GL_PS4_WF3_IUB3;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WF3_IUB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NB3;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NS3;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NS3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NS3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NS3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NS3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NS3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NS3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NS3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NF3;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NF3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NF3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NF3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NF3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NF3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NF3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NF3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WF3_IUB3_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
} NNS_VERTEX_GL_PF3;

typedef struct {
	Float x, y, z;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_T0S2;

typedef struct {
	Float x, y, z;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_T0F2;

typedef struct {
	Float x, y, z;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PF3_NB3;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_NB3_CUB4;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PF3_NS3;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NS3_T0S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NS3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NS3_T0F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NS3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_NS3_CUB4;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NS3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NS3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PF3_NF3;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NF3_T0S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NF3_T0F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_NF3_CUB4;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
} NNS_VERTEX_GL_PF3_WF1_IUB1;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_T0S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_T0F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WF1_IUB1_CUB4;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NB3;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NB3_T0S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NB3_T0F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NB3_CUB4;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NS3;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NS3_T0S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NS3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NS3_T0F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NS3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NS3_CUB4;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NS3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NS3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NF3;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NF3_T0S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NF3_T0F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NF3_CUB4;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float w0;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF1_IUB1_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
} NNS_VERTEX_GL_PF3_WF2_IUB2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WF2_IUB2_CUB4;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NB3;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NB3_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NB3_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NB3_CUB4;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NS3;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NS3_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NS3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NS3_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NS3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NS3_CUB4;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NS3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NS3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NF3;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NF3_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NF3_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NF3_CUB4;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF2_IUB2_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
} NNS_VERTEX_GL_PF3_WF3_IUB3;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WF3_IUB3_CUB4;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NB3;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NB3_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NB3_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NB3_CUB4;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NS3;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NS3_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NS3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NS3_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NS3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NS3_CUB4;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NS3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NS3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NF3;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NF3_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NF3_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NF3_CUB4;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WF3_IUB3_NF3_CUB4_T0F2_T1F2;

#endif	// ( NND_PLATFORM == NND_PLATFORM_GLES11 )

#if ( NND_PLATFORM == NND_PLATFORM_GL )

typedef struct {
	Sint16 x, y, z, w;
} NNS_VERTEX_GL_PS4;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PS4_NB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_NB3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NB3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NB3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NB3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NB3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_NB3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NB3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NB3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_NB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_NB3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NB3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NB3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PS4_NS3;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NS3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NS3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NS3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NS3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NS3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NS3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_NS3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NS3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NS3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NS3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NS3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_NS3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NS3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NS3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_NS3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NS3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NS3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_NS3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NS3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NS3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PS4_NF3;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NF3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NF3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NF3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NF3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NF3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NF3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_NF3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NF3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NF3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NF3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NF3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_NF3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NF3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NF3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_NF3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NF3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NF3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_NF3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NF3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NF3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
} NNS_VERTEX_GL_PS4_IUB1;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_IUB1_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_IUB1_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PS4_IUB1_NB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PS4_IUB1_NS3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PS4_IUB1_NF3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Sint16 x, y, z, w;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PS4_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
} NNS_VERTEX_GL_PF3;

typedef struct {
	Float x, y, z;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_T0S2;

typedef struct {
	Float x, y, z;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_T0F2;

typedef struct {
	Float x, y, z;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PF3_NB3;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_NB3_CUB3;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NB3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NB3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NB3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NB3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_NB3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NB3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NB3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_NB3_CUB4;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_NB3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NB3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NB3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PF3_NS3;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NS3_T0S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NS3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NS3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NS3_T0F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NS3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NS3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_NS3_CUB3;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NS3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NS3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NS3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NS3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_NS3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NS3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NS3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_NS3_CUB4;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NS3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NS3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_NS3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NS3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NS3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_T0S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_T0F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PF3_NF3;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NF3_T0S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NF3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NF3_T0F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NF3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_NF3_CUB3;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NF3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NF3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NF3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NF3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_NF3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NF3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NF3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_NF3_CUB4;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_NF3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NF3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NF3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_T0S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_T0F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
} NNS_VERTEX_GL_PF3_IUB1;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_IUB1_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_IUB1_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PF3_IUB1_NB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PF3_IUB1_NS3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PF3_IUB1_NF3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 i0;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_IUB1_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1;
	Uint8 i0, i1;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB2_IUB2_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2;
	Uint8 i0, i1, i2;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB3_IUB3_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint8 nx, ny, nz;
	Sint8 tx, ty, tz;
	Sint8 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NB3_TB3_BB3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Sint16 nx, ny, nz;
	Sint16 tx, ty, tz;
	Sint16 bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NS3_TS3_BS3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB3_C2UB3_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_T0F2_T1F2_T2F2_T3F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Sint16 u0, v0;
	Sint16 u1, v1;
	Sint16 u2, v2;
	Sint16 u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0S2_T1S2_T2S2_T3S2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2;

typedef struct {
	Float x, y, z;
	Uint8 w0, w1, w2, w4;
	Uint8 i0, i1, i2, i3;
	Float nx, ny, nz;
	Float tx, ty, tz;
	Float bx, by, bz;
	Uint8 r, g, b, a;
	Uint8 r2, g2, b2;
	Float u0, v0;
	Float u1, v1;
	Float u2, v2;
	Float u3, v3;
} NNS_VERTEX_GL_PF3_WUB4_IUB4_NF3_TF3_BF3_CUB4_C2UB3_T0F2_T1F2_T2F2_T3F2;

#endif	// ( NND_PLATFORM == NND_PLATFORM_GL )

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	//__NNTVERTEXGL_H__

/* End of file */

