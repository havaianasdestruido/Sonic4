/*---------------------------------------------------------------------------

    NN object common vertex header

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN object
    File    : nnfcommonvtx.h
    Create  : 2003/12/02
    Modify  : 2003/12/16
    Modify  : 2004/03/11 NND_VTXARRAYTYPE_PS2MASK‚ÉNND_VTXARRAYTYPE_TEX1‚ð’Ç‰Á
                         NND_VTXARRAYTYPE_WGTMASK’Ç‰Á
    Version : 1.11.02
    Note    : 

---------------------------------------------------------------------------*/

#ifndef	__NNFCOMMONVTX_H__
#define	__NNFCOMMONVTX_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/************************/
/* Common vertex format */
/************************/

/* Vertex type value */
#define NND_VTXTYPE_COMMON_VTXDESC						((Uint32)1<<16)
#define NND_VTXTYPE_COMMON_VTXDESC_MORPH_TARGET			((Uint32)1<<17)
#define NND_VTXTYPE_COMMON_VTXDESC_MORPH_TARGET_NULL	((Uint32)1<<18)
#define NND_VTXTYPE_COMMON_VTXDESC_MORPH_OBJECT			((Uint32)1<<19)

/* Primitive type value */
#define NND_PRIMTYPE_COMMON_TRIANGLE_STRIP_R		((Uint32)1<<16)
#define NND_PRIMTYPE_COMMON_TRIANGLE_STRIP_L		((Uint32)1<<17)
#define NND_PRIMTYPE_COMMON_TRIANGLE_LIST			((Uint32)1<<18)
#define NND_PRIMTYPE_COMMON_QUAD_LIST				((Uint32)1<<19)
#define NND_PRIMTYPE_COMMON_TRIANGLE_QUAD_LIST		((Uint32)1<<20)


/* Weight */
typedef struct {
	Sint32		Index0;
	Sint32		Index1;
	Float		Ratio;
} NNS_COMMON_WEIGHT2;

typedef struct {
	Sint32		Index;
	Float		Ratio;
} NNS_COMMON_WEIGHT;


/* Combination array */
typedef struct {
	NNS_VECTOR			Pos;
	NNS_COMMON_WEIGHT2	Wgt;
} NNS_COMMON_PW2;

typedef struct {
	NNS_VECTOR			Pos;
	NNS_COMMON_WEIGHT	Wgt[4];
} NNS_COMMON_PW4;

typedef struct {
	NNS_VECTOR			Pos;
	NNS_VECTOR			Nrm;
} NNS_COMMON_PN;

typedef struct {
	NNS_VECTOR			Pos;
	NNS_VECTOR			Nrm;
	NNS_COMMON_WEIGHT2	Wgt;
} NNS_COMMON_PNW2;

typedef struct {
	NNS_VECTOR			Pos;
	NNS_VECTOR			Nrm;
	NNS_COMMON_WEIGHT	Wgt[4];
} NNS_COMMON_PNW4;

typedef struct {
	NNS_TEXCOORD		Tex[2];
} NNS_COMMON_TEXCOORD2;


/* Vertex array type */
typedef Uint32 NNF_VTXARRAYTYPE;

/* Vertex array type value */
#define NND_VTXARRAYTYPE_POS			((Uint32)1<<0)		/* NNS_VECTOR				*/
#define NND_VTXARRAYTYPE_NRM			((Uint32)1<<1)		/* NNS_VECTOR				*/
#define NND_VTXARRAYTYPE_NBT			((Uint32)1<<2)		/* NNS_VECTOR [3]			*/	/* no support */
#define NND_VTXARRAYTYPE_COL			((Uint32)1<<3)		/* NNS_RGBA					*/
#define NND_VTXARRAYTYPE_COL2			((Uint32)1<<4)		/* NNS_RGBA					*/	/* no support */
#define NND_VTXARRAYTYPE_TEX0			((Uint32)1<<5)		/* NNS_TEXCOORD				*/
#define NND_VTXARRAYTYPE_TEX1			((Uint32)1<<6)		/* NNS_TEXCOORD				*/
#define NND_VTXARRAYTYPE_TEX2			((Uint32)1<<7)		/* NNS_TEXCOORD				*/	/* no support */
#define NND_VTXARRAYTYPE_TEX3			((Uint32)1<<8)		/* NNS_TEXCOORD				*/	/* no support */
#define NND_VTXARRAYTYPE_WGT1			((Uint32)1<<9)		/* Sint32					*/	/* no support */
#define NND_VTXARRAYTYPE_WGT2			((Uint32)1<<10)		/* NNS_COMMON_WEIGHT2		*/
#define NND_VTXARRAYTYPE_WGT3			((Uint32)1<<11)		/* NNS_COMMON_WEIGHT [3]	*/	/* no support */
#define NND_VTXARRAYTYPE_WGT4			((Uint32)1<<12)		/* NNS_COMMON_WEIGHT [4]	*/
#define NND_VTXARRAYTYPE_WGT8			((Uint32)1<<13)		/* NNS_COMMON_WEIGHT [8]	*/	/* no support */

#define NND_VTXARRAYTYPE_WGTMASK		(NND_VTXARRAYTYPE_WGT1 | NND_VTXARRAYTYPE_WGT2 | NND_VTXARRAYTYPE_WGT3 | NND_VTXARRAYTYPE_WGT4 | NND_VTXARRAYTYPE_WGT8)
#define NND_VTXARRAYTYPE_PS2MASK		(NND_VTXARRAYTYPE_POS | NND_VTXARRAYTYPE_NRM | NND_VTXARRAYTYPE_COL | NND_VTXARRAYTYPE_TEX0 | NND_VTXARRAYTYPE_TEX1 | NND_VTXARRAYTYPE_WGT2 | NND_VTXARRAYTYPE_WGT4)


/* Vertex array */
typedef struct {
	NNF_VTXARRAYTYPE	fType;
	Sint32				Number;
	Uint32				Size;
	void				*pList;
}	NNS_VTXLIST_COMMON_ARRAY;

/* Vertex descriptor */
typedef struct {
	NNS_VTXLIST_COMMON_ARRAY	List0;		/* Positon or Combination */
	NNS_VTXLIST_COMMON_ARRAY	List1;		/* Normal or Color or Texcord or NULL */
	NNS_VTXLIST_COMMON_ARRAY	List2;		/* Color or Texcord or NULL */
	NNS_VTXLIST_COMMON_ARRAY	List3;		/* Texcord or NULL */
}	NNS_VTXLIST_COMMON_DESC,
	NNS_VTXLIST_COMMON_DESC_MORPH_TARGET,
	NNS_VTXLIST_COMMON_DESC_MORPH_OBJECT;


/* Primitive index type */
typedef Uint32 NNF_PRIMINDEXTYPE;

/* Primitive index type value */
#define NND_PRIMINDEXTYPE_LIST0			((Uint32)1<<0)
#define NND_PRIMINDEXTYPE_LIST1			((Uint32)1<<1)
#define NND_PRIMINDEXTYPE_LIST2			((Uint32)1<<2)
#define NND_PRIMINDEXTYPE_LIST3			((Uint32)1<<3)

/* Primitive list */
typedef struct {
	NNF_PRIMINDEXTYPE	fType;
	Sint32				nIndexSetSize;
	Sint32				nStrip;
	Uint16				*pLengthList;
	Uint16				*pStripList;
}	NNS_PRIMLIST_COMMON_TRIANGLE_STRIP_R,
	NNS_PRIMLIST_COMMON_TRIANGLE_STRIP_L;

typedef struct {
	NNF_PRIMINDEXTYPE	fType;
	Sint32				nIndexSetSize;
	Sint32				nTriangle;
	Uint16				*pTriangleList;
}	NNS_PRIMLIST_COMMON_TRIANGLE_LIST;

typedef struct {
	NNF_PRIMINDEXTYPE	fType;
	Sint32				nIndexSetSize;
	Sint32				nQuad;
	Uint16				*pQuadList;
}	NNS_PRIMLIST_COMMON_QUAD_LIST;

typedef struct {
	NNF_PRIMINDEXTYPE	fType;
	Sint32				nIndexSetSize;
	Sint32				nTriangle;
	Uint16				*pTriangleList;
	Sint32				nQuad;
	Uint16				*pQuadList;
}	NNS_PRIMLIST_COMMON_TRIANGLE_QUAD_LIST;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	//__NNFCOMMONVTX_H__

/* End of file */
