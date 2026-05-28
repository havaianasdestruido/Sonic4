/*---------------------------------------------------------------------------

    NN object data common header

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN object
    File    : nnfobject.h
    Create  : 2002/05/10
    Modify  : 2003/04/15
    Modify  : 2003/08/15 マテリアルネーム追加
    Modify  : 2003/10/08 Xbox,PC共通化向けの修正
    Modify  : 2003/10/29 NNS_MESHSETにXbox,PC用新メンバを追加
    Modify  : 2003/11/04 MorphTargetName構造体を追加
    Modify  : 2003/12/02 共通頂点フォーマット追加
    Modify  : 2003/12/18 ノードフラグ追加
                         NND_NODETYPE_RESET_SCALING_?
                         NND_NODETYPE_UNIT33_INIT_MATRIX
                         NND_NODETYPE_ORTHO33_INIT_MATRIX
    Modify  : 2004/03/18 NND_SUBOBJTYPE_PXPLUS追加
    Modify  : 2004/06/07 PC向けの修正
    Modify  : 2004/06/07 ノードフラグ追加（バウンディングボックスデータを持つ）
                         NND_NODETYPE_CLIP_BOUNDINGBOX
                         NNS_NODE構造体のメンバ変更
                         バウンディングボックス幅 BBXSize, BBYSize, BBZsize
    Modify  : 2004/06/16 DX8,DX9版統合
    Modify  : 2004/08/05 NND_PLATFORM_XENON追加
    Modify  : 2004/08/12 OpenGL版統合
    Modify  : 2005/01/28 NNS_OBJECTメンバ拡張 (PlayStation2)
                         PriNodeObj/NodeEx追加(PlayStation2)
    Modify  : 2005/03/10 DT06版統合
    Modify  : 2005/03/14 PSP版統合
    Modify  : 2006/03/22 PLAYSTATION3版統合
    Modify  : 2006/06/01 PriNodeObj/NodeEx追加(PC)
    Modify  : 2006/06/15 PC版をNEWOBJECT定義に変更
    Modify  : 2008/05/21 NND_PLATFORM_DXG20追加
    Modify  : 2008/11/12 PSP:プライオリティノード関連追加
    Modify  : 2009/03/05 GLES11追加
    Modify  : 2009/04/06 マテリアル関連処理のディスプレイ化に伴う修正
    Modify  : 2009/04/23 XSI IKに対応
    Modify  : 2009/05/13 NND_PLATFORM_XENON削除
    Version : 1.20.14
    Note    :

---------------------------------------------------------------------------*/

#ifndef	__NNFOBJECT_H__
#define	__NNFOBJECT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/************************/
/* NN Object Structure   */
/************************/

/* Node type */
typedef Uint32 NNF_NODETYPE;

/* Node type value */
#define	NND_NODETYPE_UNIT_TRANSLATION		((Uint32)1 << 0)	/* No Trans */
#define	NND_NODETYPE_UNIT_ROTATION			((Uint32)1 << 1)	/* No Rot   */
#define	NND_NODETYPE_UNIT_SCALING			((Uint32)1 << 2)	/* No Scl   */
#define	NND_NODETYPE_UNIT_INIT_MATRIX		((Uint32)1 << 3)	/* Unit init mtx */
#define	NND_NODETYPE_HIDE					((Uint32)1 << 4)	/* No draw */
#define	NND_NODETYPE_HIDE_BRANCH			((Uint32)1 << 5)	/* No draw to terminal */
#define	NND_NODETYPE_UNIT33_INIT_MATRIX		((Uint32)1 << 6)	/* Translation only init mtx */
#define	NND_NODETYPE_ORTHO33_INIT_MATRIX	((Uint32)1 << 7)	/* Orthogonal init mtx */

/* Node Euler rotation order */
#define	NND_NODETYPE_ROTATE_TYPE_XYZ		((Uint32) 0 << 8)
#define	NND_NODETYPE_ROTATE_TYPE_XZY		((Uint32) 1 << 8)
#define	NND_NODETYPE_ROTATE_TYPE_YXZ		((Uint32) 2 << 8)  /* No support */
#define	NND_NODETYPE_ROTATE_TYPE_YZX		((Uint32) 3 << 8)  /* No support */
#define	NND_NODETYPE_ROTATE_TYPE_ZXY		((Uint32) 4 << 8)
#define	NND_NODETYPE_ROTATE_TYPE_ZYX		((Uint32) 5 << 8)  /* No support */
#define	NND_NODETYPE_ROTATE_TYPE_XYX		((Uint32) 6 << 8)  /* No support */
#define	NND_NODETYPE_ROTATE_TYPE_XZX		((Uint32) 7 << 8)  /* No support */
#define	NND_NODETYPE_ROTATE_TYPE_YXY		((Uint32) 8 << 8)  /* No support */
#define	NND_NODETYPE_ROTATE_TYPE_YZY		((Uint32) 9 << 8)  /* No support */
#define	NND_NODETYPE_ROTATE_TYPE_ZXZ		((Uint32)10 << 8)  /* No support */
#define	NND_NODETYPE_ROTATE_TYPE_ZYZ		((Uint32)11 << 8)  /* No support */
#define	NND_NODETYPE_ROTATE_TYPE_MASK		((Uint32)15 << 8)

/* Reset inheritance of rotation and scaling */
#define	NND_NODETYPE_INHERIT_ONLY_TRANSLATION	((Uint32)1 << 12)

/* SIIK node */
#define	NND_NODETYPE_SIIK_EFFECTOR			((Uint32)1 << 13)
#define	NND_NODETYPE_SIIK_1BONE_IK_JOINT1	((Uint32)1 << 14)
#define	NND_NODETYPE_SIIK_2BONE_IK_JOINT1	((Uint32)1 << 15)
#define	NND_NODETYPE_SIIK_2BONE_IK_JOINT2	((Uint32)1 << 16)
#define	NND_NODETYPE_SIIK_MINUS_PREFROT_Z	((Uint32)1 << 17)

#define	NND_NODETYPE_1BONE_IK_CHAIN_ROOT	((Uint32)1 << 25)	/* new */
#define	NND_NODETYPE_2BONE_IK_CHAIN_ROOT	((Uint32)1 << 26)	/* new */
#define	NND_NODETYPE_IK_EFFECTOR			((Uint32)1 << 13)	/* rename */
#define	NND_NODETYPE_1BONE_IK_JOINT1		((Uint32)1 << 14)	/* rename */
#define	NND_NODETYPE_2BONE_IK_JOINT1		((Uint32)1 << 15)	/* rename */
#define	NND_NODETYPE_2BONE_IK_JOINT2		((Uint32)1 << 16)	/* rename */
#define	NND_NODETYPE_IK_MINUS_PREFROT_Z		((Uint32)1 << 17)	/* rename */
#define	NND_NODETYPE_XSIIK					((Uint32)1 << 27)	/* new */

#define	NND_NODETYPE_XSIIK_1BONE_IK_CHAIN_ROOT	( NND_NODETYPE_XSIIK | NND_NODETYPE_1BONE_IK_CHAIN_ROOT )
#define	NND_NODETYPE_XSIIK_2BONE_IK_CHAIN_ROOT	( NND_NODETYPE_XSIIK | NND_NODETYPE_2BONE_IK_CHAIN_ROOT )
#define	NND_NODETYPE_XSIIK_EFFECTOR				( NND_NODETYPE_XSIIK | NND_NODETYPE_IK_EFFECTOR )
#define	NND_NODETYPE_XSIIK_1BONE_IK_JOINT1		( NND_NODETYPE_XSIIK | NND_NODETYPE_1BONE_IK_JOINT1 )
#define	NND_NODETYPE_XSIIK_2BONE_IK_JOINT1		( NND_NODETYPE_XSIIK | NND_NODETYPE_2BONE_IK_JOINT1 )
#define	NND_NODETYPE_XSIIK_2BONE_IK_JOINT2		( NND_NODETYPE_XSIIK | NND_NODETYPE_2BONE_IK_JOINT2 )
#define	NND_NODETYPE_XSIIK_MINUS_PREFROT_Z		( NND_NODETYPE_XSIIK | NND_NODETYPE_IK_MINUS_PREFROT_Z )

#define NND_NODETYPE_IK_MASK				( NND_NODETYPE_XSIIK \
											| NND_NODETYPE_1BONE_IK_CHAIN_ROOT \
											| NND_NODETYPE_2BONE_IK_CHAIN_ROOT \
											| NND_NODETYPE_IK_EFFECTOR \
											| NND_NODETYPE_1BONE_IK_JOINT1 \
											| NND_NODETYPE_2BONE_IK_JOINT1 \
											| NND_NODETYPE_2BONE_IK_JOINT2 \
											| NND_NODETYPE_IK_MINUS_PREFROT_Z )

#define NNM_NODE_IS_SIIK( _nodetype )	(((_nodetype) & NND_NODETYPE_IK_MASK) != 0 && ((_nodetype) & NND_NODETYPE_XSIIK) == 0)
#define NNM_NODE_IS_XSIIK( _nodetype )	(((_nodetype) & NND_NODETYPE_XSIIK) != 0)

/* Reset inheritance of scaling on parent axis */
#define	NND_NODETYPE_RESET_SCALING_X		((Uint32)1 << 18)
#define	NND_NODETYPE_RESET_SCALING_Y		((Uint32)1 << 19)
#define	NND_NODETYPE_RESET_SCALING_Z		((Uint32)1 << 20)

/* Bounding Box Data */
#define	NND_NODETYPE_BBOX_DATA				((Uint32)1 << 21)
#define	NND_NODETYPE_BBOX_PRIOR_SPHERE		((Uint32)1 << 22)
#define	NND_NODETYPE_BBOX_DOMINATE_X		((Uint32)1 << 23)
#define	NND_NODETYPE_BBOX_DOMINATE_Y		((Uint32)2 << 23)
#define	NND_NODETYPE_BBOX_DOMINATE_Z		((Uint32)3 << 23)
#define	NND_NODETYPE_BBOX_DOMINATE_MASK		((Uint32)3 << 23)


/* Reserved (No support) */
#if 0
#define	NND_NODETYPE_BILLBOARD				((Uint32)1 << 25)	/* Billboard */
#define	NND_NODETYPE_BILLBOARD_CAMERA		((Uint32)1 << 26)	/* Camera bill board */
#define	NND_NODETYPE_BILLBOARD_FIX_X		((Uint32)1 << 27)	/* Fix X coordinate */
#define	NND_NODETYPE_BILLBOARD_FIX_Y		((Uint32)1 << 28)	/* Fix Y coordinate */
#define	NND_NODETYPE_COLLISION				((Uint32)1 << 29)	/* Collision model */
#endif

/* Index terminator */
#define	NND_NODEIDX_NIL				(-1)	/* Nil node index */
#define	NND_MTXIDX_NIL				(-1)	/* Nil matrix index */
#define	NND_VTXLISTIDX_NIL			(-1)	/* Nil vertex list index */
#define	NND_PRIMLISTIDX_NIL			(-1)	/* Nil primitive list index */

/* Node */
typedef struct {
	NNF_NODETYPE	fType;				/* Node type */
	Sint16			iMatrix;			/* Matrix index */
	Sint16			iParent;			/* Parent node index */
	Sint16			iChild;				/* Child node index */
	Sint16			iSibling;			/* Sibling node index */
	NNS_VECTOR		Translation;		/* Translation value */
	NNS_ROTATE_A32	Rotation;			/* Rotation value X */
	NNS_VECTOR		Scaling;			/* Scaling value */
	NNS_MATRIX		InvInitMtx;			/* Invert initial matrix */
	NNS_VECTOR		Center;				/* Center position */
	Float			Radius;				/* Radius */
	Uint32			User;				/* User data */
	union {
		Float		SIIKBoneLength;		/* SIIK Bone length */
		Float		BoundingBoxX;		/* Bounding box half X size */
	};
	Float			BoundingBoxY;		/* Bounding box half Y size */
	Float			BoundingBoxZ;		/* Bounding box half Z size */
} NNS_NODE;

/* Node name */
typedef struct {
	Sint32     iNode;
	const char *Name;
} NNS_NODENAME;

/* Node name sort type */
typedef enum {
	NNE_NODENAME_SORTTYPE_INDEX,
	NNE_NODENAME_SORTTYPE_NAME
} NNE_NODENAME_SORTTYPE;

/* Node name list */
typedef struct {
	NNE_NODENAME_SORTTYPE SortType;
	Sint32 nNode;
	NNS_NODENAME *pNodeNameList;
} NNS_NODENAMELIST;

/* Material name */
typedef struct {
	Sint32     iMaterial;
	const char *Name;
} NNS_MATERIALNAME;

/* Material name sort type */
typedef enum {
	NNE_MATERIALNAME_SORTTYPE_INDEX,
	NNE_MATERIALNAME_SORTTYPE_NAME
} NNE_MATERIALNAME_SORTTYPE;

/* Material name list */
typedef struct {
	NNE_MATERIALNAME_SORTTYPE SortType;
	Sint32 nMaterial;
	NNS_MATERIALNAME *pMaterialNameList;
} NNS_MATERIALNAMELIST;


/* Vertex type */
typedef Uint32	NNF_VTXTYPE;

/* Vertex type mask */
#define NND_VTXTYPE_PLATFORM_MASK			((Uint32)0x0000ffff)
#define NND_VTXTYPE_COMMON_MASK				((Uint32)0x00ff0000)
#define NND_VTXTYPE_USER_MASK				((Uint32)0xff000000)

/* Vertex list pointer */
typedef struct {
	NNF_VTXTYPE			fType;
	void				*pVtxList; 			/* Vertex list Pointer */
} NNS_VTXLISTPTR;


/* Morph target pointer */
typedef struct {
	Sint32			nVtxList;
	NNS_VTXLISTPTR	*pMorphTarget;
} NNS_MORPHTARGETPTR;

/* Morph target pointer list */
typedef struct {
	Sint32				nMorphTarget;
	NNS_MORPHTARGETPTR	*pMorphTargetPtrList;
} NNS_MORPHTARGETLIST;

/* Morph target name */
typedef struct {
	Sint32		iMorphTarget;
	const char	*Name;
} NNS_MORPHTARGETNAME;

/* Morph target name sort type */
typedef enum {
	NNE_MORPHTARGETNAME_SORTTYPE_INDEX,
	NNE_MORPHTARGETNAME_SORTTYPE_NAME
} NNE_MORPHTARGETNAME_SORTTYPE;

/* Morph target name list */
typedef struct {
	NNE_MORPHTARGETNAME_SORTTYPE	SortType;
	Sint32							nMorphTarget;
	NNS_MORPHTARGETNAME				*pMorphTargetNameList;
} NNS_MORPHTARGETNAMELIST;


/* Primitive type */
typedef Uint32	NNF_PRIMTYPE;

/* Primitive type mask */
#define NND_PRIMTYPE_PLATFORM_MASK			((Uint32)0x0000ffff)
#define NND_PRIMTYPE_COMMON_MASK			((Uint32)0x00ff0000)
#define NND_PRIMTYPE_USER_MASK				((Uint32)0xff000000)

/* Primitive pointer */
typedef struct {
	NNF_PRIMTYPE	fType;
	void			*pPrimList;		 	/* Primitive list Pointer */
} NNS_PRIMLISTPTR;


/* Meshset */
typedef struct {
	NNS_VECTOR			Center;				/* Center position */
	Float				Radius;				/* Radius */
	Sint32				iNode;				/* Node index */
	Sint32				iMatrix;			/* Matrix index */
	Sint32				iMaterial;			/* Material index */
	Sint32				iVtxList;			/* Vertex list index */
	Sint32				iPrimList;			/* Primitive list index */
#if ( NND_PLATFORM == NND_PLATFORM_XB || NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9 || NND_PLATFORM == NND_PLATFORM_DXG20)
	Sint32				iShader;			// シェーダーリストインデックス
#endif
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 || NND_PLATFORM == NND_PLATFORM_PS3 )
	Uint32				Reserved2;
	Uint32				Reserved1;
	Uint32				Reserved0;
#endif	// ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 || NND_PLATFORM == NND_PLATFORM_PS3 )
} NNS_MESHSET;


/******************************/
/* NodeEx type and structures */
/******************************/
#if ( NND_PLATFORM == NND_PLATFORM_PS2 || NND_PLATFORM == NND_PLATFORM_XB || NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9 || NND_PLATFORM == NND_PLATFORM_PSP )
#include "nnfnodeex.h"
#else
typedef struct _NNS_NODEEX NNS_NODEEX;	/* NodeEx dummy */
#endif //PlayStation2

/* NodeEx pointer */
typedef struct {
	NNS_NODEEX	*pNodeEx;
}NNS_NODEEXPTR;


/* Subobject type */
typedef Uint32	NNF_SUBOBJTYPE;

/* Subobject type value */
#define	NND_SUBOBJTYPE_OPAQUE			((Uint32)1 << 0)
#define	NND_SUBOBJTYPE_TRANSPARENT		((Uint32)1 << 1)
#define	NND_SUBOBJTYPE_PUNCHTHROUGH		((Uint32)1 << 2)
#define	NND_SUBOBJTYPE_TRANSPARENCY_MASK	( NND_SUBOBJTYPE_OPAQUE |	\
			NND_SUBOBJTYPE_TRANSPARENT | NND_SUBOBJTYPE_PUNCHTHROUGH )
#define	NND_SUBOBJTYPE_TRANSPARENCY_ALL	NND_SUBOBJTYPE_TRANSPARENCY_MASK
#define	NND_SUBOBJTYPE_RIGID			((Uint32)1 << 8)
#define	NND_SUBOBJTYPE_PLIABLE			((Uint32)1 << 9)
#define	NND_SUBOBJTYPE_PLIABILITY_MASK	\
			( NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE )
#define	NND_SUBOBJTYPE_PLIABILITY_ALL	NND_SUBOBJTYPE_PLIABILITY_MASK
#define	NND_SUBOBJTYPE_COLLISION		((Uint32)1 << 16)	/* No support */
#define	NND_SUBOBJTYPE_BILLBOARD		((Uint32)1 << 24)	/* No support */
#define	NND_SUBOBJTYPE_PXPLUS			((Uint32)1 << 28)
#define	NND_SUBOBJTYPE_ALL				((Uint32)1 << 31)	/* Do for each subobj*/

/* Subobject */
typedef struct {
	NNF_SUBOBJTYPE		fType;				/* Subobject type */
	Sint32				nMeshset;			/* Number of meshset */
	NNS_MESHSET			*pMeshsetList;		/* Meshset list */
	Sint32				nTex;				/* Number of textures */
	Sint32				*pTexNumList;		/* Texture number list */
} NNS_SUBOBJ;


#if ( NND_PLATFORM == NND_PLATFORM_GC )
#define NND_NEW_OBJECT_FORMAT		0
#else
#define NND_NEW_OBJECT_FORMAT		1
#endif

#if NND_NEW_OBJECT_FORMAT

/* Object type */
typedef Uint32	NNF_OBJECTTYPE;

/* Object type value */
#define	NND_OBJTYPE_OPAQUE					((Uint32)1 << 0)
#define	NND_OBJTYPE_TRANSPARENT				((Uint32)1 << 1)
#define	NND_OBJTYPE_PUNCHTHROUGH			((Uint32)1 << 2)
#define	NND_OBJTYPE_TRANSPARENCY_MASK		( NND_OBJTYPE_OPAQUE | NND_OBJTYPE_TRANSPARENT | NND_OBJTYPE_PUNCHTHROUGH )
#define	NND_OBJTYPE_RIGID					((Uint32)1 << 3)
#define	NND_OBJTYPE_PLIABLE					((Uint32)1 << 4)
#define	NND_OBJTYPE_PLIABILITY_MASK			( NND_OBJTYPE_RIGID | NND_OBJTYPE_PLIABLE )

#define NND_OBJTYPE_LOADED_BINARY			((Uint32)1 << 5)
#define NND_OBJTYPE_COMPILED_TEXT			((Uint32)1 << 6)
#define NND_OBJTYPE_COPYED_OBJECT			((Uint32)1 << 7)
#define NND_OBJTYPE_MORPH_OBJECT			((Uint32)1 << 8)
#define NND_OBJTYPE_MATERIALMOTION_OBJECT	((Uint32)1 << 9)

#define NND_OBJTYPE_COMMONVTXFORM			((Uint32)1 << 16)
#define NND_OBJTYPE_NODETREE_OBJECT			((Uint32)1 << 17)
#define NND_OBJTYPE_MESH_OBJECT				((Uint32)1 << 18)
#define	NND_OBJTYPE_PRIORITYNODE_OBJECT		((Uint32)1 << 19)

#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 || NND_PLATFORM == NND_PLATFORM_PS3 )
#define NND_OBJTYPE_BOUNDBUFFER				((Uint32)1 << 24)
#endif	// ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 || NND_PLATFORM == NND_PLATFORM_PS3 )
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#define	NND_OBJTYPE_PXPLUS					((Uint32)1 << 28)
#endif	// ( NND_PLATFORM == NND_PLATFORM_PS2 )
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#define	NND_OBJTYPE_MATERIAL_MOTION			((Uint32)1 << 28)
#endif	// ( NND_PLATFORM == NND_PLATFORM_PSP )

#endif	// NND_NEW_OBJECT_FORMAT

/* Object */
typedef struct {
	NNS_VECTOR			Center;					/* Center position */
	Float				Radius;					/* Radius */
	Sint32				nMaterial;				/* Number of material */
	NNS_MATERIALPTR		*pMatPtrList;			/* Material pointer list */
	Sint32				nVtxList;				/* Number of vertex list */
	NNS_VTXLISTPTR		*pVtxListPtrList;		/* Vertexlist pointer list */
	Sint32				nPrimList;				/* Number of primitive list */
	NNS_PRIMLISTPTR 	*pPrimListPtrList;		/* Primitivelist pointer list */
	Sint32				nNode;					/* Number of nodes */
	Sint32				MaxNodeDepth;			/* Maximum node depth */
	union{
		NNS_NODE		*pNodeList;				/* Node list */
		NNS_NODEEXPTR	*pNodeExPtrList;		/* NodeEx (PlayStation2 Only) */
	};
	Sint32				nMtxPal;				/* Number of matrix pallette */
	Sint32				nSubobj;				/* Number of subobjects */
	NNS_SUBOBJ			*pSubobjList;			/* Subobject list */
	Sint32				nTex;					/* Number of textures */
#if NND_NEW_OBJECT_FORMAT
	NNF_OBJECTTYPE		fType;					/* Object type */
	Sint32				Version;				/* Object format version */
	Float				BoundingBoxX;			/* Bounding box half X size */
	Float				BoundingBoxY;			/* Bounding box half Y size */
	Float				BoundingBoxZ;			/* Bounding box half Z size */
#endif	// NND_NEW_OBJECT_FORMAT
} NNS_OBJECT;

#ifdef __cplusplus
}
#endif /* __cplusplus */

/********************************************/
/* Common vertex type and structures */
/********************************************/
#include "nnfcommonvtx.h"

/********************************************/
/* PlayStation2 Default type and structures */
/********************************************/
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nnfobjectps2.h"
#endif //PlayStation2

/****************************************/
/* GAMECUBE Default type and structures */
/****************************************/
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nnfobjectgc.h"
#endif //GAMECUBE

/************************************/
/* Xbox Default type and structures */
/************************************/
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nnfobjectdx.h"
#endif //Xbox

/**********************************/
/* PC Default type and structures */
/**********************************/
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
#include "nnfobjectdx.h"
#endif

/**************************************/
/* OpenGL Default type and structures */
/**************************************/
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#include "nnfobjectgl.h"
#endif //OpenGL

/**************************************/
/* PSP Default type and structures */
/**************************************/
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#include "nnfobjectpsp.h"
#endif //PSP

/**************************************/
/* PS3 Default type and structures */
/**************************************/
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
#include "nnfobjectps3.h"
#endif //PS3

/*************************************/
/* DXG20 Default type and structures */
/*************************************/
#if ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#include "nnfobjectdxg20.h"
#endif

#endif	//__NNFOBJECT_H__

/* End of file */
