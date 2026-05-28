/*---------------------------------------------------------------------------

    NN object format for OpenGL

    Copyright (C) 2004-2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN object format for OpenGL
    File    : nnfobjectgl.h
    Create  : 2004/05/21
    Modify  : 2004/12/07 Tangent,Binormal
    Modify  : 2004/12/10 OBJECT_SPACE_NORMAL
    Modify  : 2005/02/16 REVERSE_NORMAL,REVERSE_TANGENT,REVERSE_BINORMAL
    Modify  : 2009/02/25 for OpenGL ES 1.1
    Modify  : 2009/09/25 Interleaved
    Version : 0.81.00
    Version : 0.00.00 for OpenGL ES 1.1
    Note    : 

---------------------------------------------------------------------------*/

#ifndef	__NNFOBJECTGL_H__
#define	__NNFOBJECTGL_H__

/*********************/
/* Vertex Descriptor */
/*********************/

#define	NND_VTXTYPE_GL_VERTEXDESC					((Uint32) 1 <<  0)
#define	NND_VTXTYPE_GL_VERTEXDESC_MORPH_TARGET		((Uint32) 1 <<  1)
#define	NND_VTXTYPE_GL_VERTEXDESC_MORPH_TARGET_NULL	((Uint32) 1 <<  2)
#define	NND_VTXTYPE_GL_BOUNDBUFFER					((Uint32) 1 <<  4)

// Vertex Array Type
#define NND_VTXARRAYTYPE_GL_POS						((Uint32) 1 <<  0)
#define NND_VTXARRAYTYPE_GL_WGT						((Uint32) 1 <<  1)
#define NND_VTXARRAYTYPE_GL_MTXIDX					((Uint32) 1 <<  2)
#define NND_VTXARRAYTYPE_GL_NRM						((Uint32) 1 <<  3)
#define NND_VTXARRAYTYPE_GL_COL						((Uint32) 1 <<  4)
#define NND_VTXARRAYTYPE_GL_COL2					((Uint32) 1 <<  5)						/* no support for OpenGL ES 1.1 */
#define NND_VTXARRAYTYPE_GL_TAN						((Uint32) 1 <<  6)	/* Tangent */		/* no support for OpenGL ES 1.1 */
#define NND_VTXARRAYTYPE_GL_BNRM					((Uint32) 1 <<  7)	/* Binormal */		/* no support for OpenGL ES 1.1 */
#define NND_VTXARRAYTYPE_GL_TEX0					((Uint32) 1 <<  8)
#define NND_VTXARRAYTYPE_GL_TEX1					((Uint32) 1 <<  9)
#define NND_VTXARRAYTYPE_GL_TEX2					((Uint32) 1 << 10)						/* no support for OpenGL ES 1.1 */
#define NND_VTXARRAYTYPE_GL_TEX3					((Uint32) 1 << 11)						/* no support for OpenGL ES 1.1 */
//#define NND_VTXARRAYTYPE_GL_INDEX					((Uint32) 1 << 16)	/* no support */
//#define NND_VTXARRAYTYPE_GL_FOGCOORD				((Uint32) 1 << 17)	/* no support */
//#define NND_VTXARRAYTYPE_GL_EDGEFLAG				((Uint32) 1 << 18)	/* no support */

#define NND_VTXARRAYTYPE_INTERLEAVED				((Uint32) 1 << 16)

#define NND_VTXARRAYTYPE_REVERSE_NORMAL				((Uint32) 1 << 27)	/* no support */				/* no support for OpenGL ES 1.1 */
#define NND_VTXARRAYTYPE_REVERSE_TANGENT			((Uint32) 1 << 28)	/* for only object-space */		/* no support for OpenGL ES 1.1 */
#define NND_VTXARRAYTYPE_REVERSE_BINORMAL			((Uint32) 1 << 29)	/* no support */				/* no support for OpenGL ES 1.1 */
#define NND_VTXARRAYTYPE_OBJECT_SPACE_NORMAL		((Uint32) 1 << 30)									/* no support for OpenGL ES 1.1 */
#define NND_VTXARRAYTYPE_MORPH_TARGET_DIFF			((Uint32) 1 << 31)

// Data Type
#define NND_DATATYPE_GL_BYTE						0x1400
#define NND_DATATYPE_GL_UNSIGNED_BYTE				0x1401
#define NND_DATATYPE_GL_SHORT						0x1402
#define NND_DATATYPE_GL_UNSIGNED_SHORT				0x1403
#define NND_DATATYPE_GL_INT							0x1404				/* no support for OpenGL ES 1.1 */
#define NND_DATATYPE_GL_UNSIGNED_INT				0x1405				/* no support for OpenGL ES 1.1 */
#define NND_DATATYPE_GL_FLOAT						0x1406
#define NND_DATATYPE_GL_DOUBLE						0x140A				/* no support for OpenGL ES 1.1 */
#define NND_DATATYPE_GL_FIXED_OES					0x140C				/* for OpenGL ES 1.1 */

// Vertex Array
typedef struct {
	Uint32				Type;				/* NND_VTXARRAYTYPE_...	*/
	Sint32				Size;				/* 2, 3, 4				*/
	Uint32				DataType;			/* NND_DATATYPE_...		*/
	Sint32				Stride;

	void				*Pointer;
} NNS_VTXARRAY_GL;

// Vertex List Descriptor
typedef struct {
	Uint32				Type;				/* NND_VTXARRAYTYPE_...	*/
	Sint32				nVertex;
	Sint32				nArray;
	NNS_VTXARRAY_GL		*pArray;
	Sint32				VertexBufferSize;
	void				*pVertexBuffer;
	Sint32				nMatrix;
	Uint16				*pMatrixIndices;
	Uint32				BufferName;
} NNS_VTXLIST_GL_DESC;


/************************/
/* Primitive Descriptor */
/************************/

#define	NND_PRIMTYPE_GL_PRIMITIVEDESC				((Uint32) 1 <<  0)
#define	NND_PRIMTYPE_GL_BOUNDBUFFER					((Uint32) 1 <<  1)


// Primitive Mode
#define NND_PRIMMODE_GL_POINTS							0x0000
#define NND_PRIMMODE_GL_LINES							0x0001
#define NND_PRIMMODE_GL_LINE_LOOP						0x0002
#define NND_PRIMMODE_GL_LINE_STRIP						0x0003
#define NND_PRIMMODE_GL_TRIANGLES						0x0004
#define NND_PRIMMODE_GL_TRIANGLE_STRIP					0x0005
#define NND_PRIMMODE_GL_TRIANGLE_FAN					0x0006
#define NND_PRIMMODE_GL_QUADS							0x0007		/* no support for OpenGL ES 1.1 */
#define NND_PRIMMODE_GL_QUAD_STRIP						0x0008		/* no support for OpenGL ES 1.1 */
#define NND_PRIMMODE_GL_POLYGON							0x0009		/* no support for OpenGL ES 1.1 */

// Primitive List Descriptor
typedef struct {
	Uint32		Mode;			/* NND_PRIMMODE_...	*/
	Sint32		*pCounts;
	Uint32		DataType;		/* NND_DATATYPE_...	*/
	void		**pIndices;
	Sint32		nPrim;
	Sint32		IndexBufferSize;
	void		*pIndexBuffer;
	Uint32		BufferName;
} NNS_PRIMLIST_GL_DESC;

#endif	//__NNFOBJECTGL_H__

/* End of file */
