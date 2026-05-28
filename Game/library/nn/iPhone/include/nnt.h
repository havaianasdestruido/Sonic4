/*---------------------------------------------------------------------------

    NN text data macro

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN text macro
    File    : nnt.h
    Create  : 2002/05/10
    Modify  : 2003/08/22
    Modify  : 2003/09/19 フレームレート付モーション(Version2)に対応
    Modify  : 2003/10/08 Xbox,PC共通化向けの修正
    Modify  : 2003/10/29 MSSTにXbox,PC用新メンバを追加
    Modify  : 2003/11/17 モーフターゲットネームリスト、マテリアルネームリスト、
                         マテリアルモーション用定義を追加
    Modify  : 2003/12/08 共通頂点フォーマット追加
    Modify  : 2003/12/17 ウェイトつき共通頂点フォーマットに対応
    Modify  : 2004/06/07 NODE_BB?SIZE追加
    Modify  : 2004/06/16 DX8,DX9版統合
    Modify  : 2004/08/05 NND_PLATFORM_XENON追加
    Modify  : 2004/08/12 OpenGL版統合
    Modify  : 2004/08/23 ディレクショナルライト追加
    Modify  : 2004/12/22 fType,Version,BoundingBox付オブジェクト(Version2)に対応
    Modify  : 2005/01/28 nntnodeex.hのインクルードを追加(PlayStation2版)
    Modify  : 2005/03/10 DT06版統合
    Modify  : 2005/03/14 PSP版統合
    Modify  : 2005/04/05 DT06版MTX/MSSTマクロ修正
    Modify  : 2005/04/14 PSP版MSSTマクロ修正
    Modify  : 2005/04/20 PSP版MSSTマクロ修正
    Modify  : 2006/03/15 XENON版, PLAYSTATION3版統合
    Modify  : 2008/05/21 NND_PLATFORM_DXG20追加
    Modify  : 2009/05/13 NND_PLATFORM_XENON削除
    Version : 1.18.34
    Note    :

---------------------------------------------------------------------------*/

#ifndef	__NNT_H__
#define	__NNT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*********************/
/* Common definition */
/*********************/

#define	TRN( _x, _y, _z )		{ ( _x ), ( _y ), ( _z ) }
#define	ROT( _x, _y, _z )		{ NNM_DEGtoA32( _x ), NNM_DEGtoA32( _y ), \
								  NNM_DEGtoA32( _z )  }
#define	SCL( _x, _y, _z )		{ ( _x ), ( _y ), ( _z ) }

#define	NNM_DEGtoA32FLT(n)		((n) * 182.04444f)	/* Degree -> Angle32 scaling */
#define	NNM_DEGtoA16FLT(n)		((n) * 182.04444f)	/* Degree -> Angle32 scaling */


#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#define	MTX(  _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23, _a30, _a31, _a32, _a33 ) \
			{ _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23, _a30, _a31, _a32, _a33 }
#elif ( NND_PLATFORM == NND_PLATFORM_GC )
#define	MTX(  _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23 ) \
			{ _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23 }
#elif ( NND_PLATFORM == NND_PLATFORM_XB )
#define	MTX(  _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23, _a30, _a31, _a32, _a33 ) \
			{ _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23, _a30, _a31, _a32, _a33 }
#elif ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
#define	MTX(  _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23, _a30, _a31, _a32, _a33 ) \
			{ _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23, _a30, _a31, _a32, _a33 }
#elif ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#define	MTX(  _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23, _a30, _a31, _a32, _a33 ) \
			{ _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23, _a30, _a31, _a32, _a33 }
#elif ( NND_PLATFORM == NND_PLATFORM_PSP )
#define	MTX(  _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23, _a30, _a31, _a32, _a33 ) \
			{ _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23, _a30, _a31, _a32, _a33 }
#elif ( NND_PLATFORM == NND_PLATFORM_PS3 )
#define	MTX(  _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23, _a30, _a31, _a32, _a33 ) \
			{ _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23, _a30, _a31, _a32, _a33 }
#elif ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#define	MTX(  _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23, _a30, _a31, _a32, _a33 ) \
			{ _a00, _a01, _a02, _a03, _a10, _a11, _a12, _a13,  \
			  _a20, _a21, _a22, _a23, _a30, _a31, _a32, _a33 }
#endif


/**********/
/* Object */
/**********/
#define	OBJECT2( _ctr, _r, _nmat, _matptr, _nvlst, _vlstptr, _nplst, _plstptr, \
		_nnd, _mnddp, _ndptr, _nmp, _nso, _soptr, _ntex,  _type, _ver, _bbx, _bby, _bbz )  \
			{ _ctr, _r, _nmat, _matptr, _nvlst, _vlstptr, _nplst, _plstptr,   \
		_nnd, _mnddp, _ndptr, _nmp, _nso, _soptr, _ntex, _type, _ver, _bbx, _bby, _bbz }
#if NND_NEW_OBJECT_FORMAT
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_PS3 \
	|| NND_PLATFORM == NND_PLATFORM_PSP || NND_PLATFORM == NND_PLATFORM_DXG20 || NND_PLATFORM == NND_PLATFORM_GLES11 )
#define	OBJECT	OBJECT2
#else
#define	OBJECT( _ctr, _r, _nmat, _matptr, _nvlst, _vlstptr, _nplst, _plstptr, \
		_nnd, _mnddp, _ndptr, _nmp, _nso, _soptr, _ntex )  \
			{ _ctr, _r, _nmat, _matptr, _nvlst, _vlstptr, _nplst, _plstptr,   \
		_nnd, _mnddp, _ndptr, _nmp, _nso, _soptr, _ntex, 0, 0, 0.0F, 0.0F, 0.0F }
#endif
#else	// NND_NEW_OBJECT_FORMAT
#define	OBJECT( _ctr, _r, _nmat, _matptr, _nvlst, _vlstptr, _nplst, _plstptr, \
		_nnd, _mnddp, _ndptr, _nmp, _nso, _soptr, _ntex)  \
			{ _ctr, _r, _nmat, _matptr, _nvlst, _vlstptr, _nplst, _plstptr,   \
		_nnd, _mnddp, _ndptr, _nmp, _nso, _soptr, _ntex }
#endif	// NND_NEW_OBJECT_FORMAT
/* Object center */
#define	OBJ_CENTER( _x, _y, _z )		TRN( ( _x ), ( _y ), ( _z ) )
/* Object Radius */
#define	OBJ_RADIUS( _r )				( _r )
/* Number of material */
#define	OBJ_N_MAT( _num )				( _num )
/* Material pointer list pointer */
#define	OBJ_MATPTR_LIST( _ptr )			( _ptr )
/* Number of Vertex list pointer */
#define	OBJ_N_VTXLIST( _num )			( _num )
/* Vertex list pointer list */
#define	OBJ_VTXLISTPTR_LIST( _ptr )		( _ptr )
/* Number of primitive list pointer list */
#define	OBJ_N_PRIMLIST( _num )			( _num )
/* Primitive list pointer list */
#define	OBJ_PRIMLISTPTR_LIST( _ptr )	( _ptr )
/* Num of Node */
#define	OBJ_N_NODE( _num )				( _num )
/* Max node depth */
#define	OBJ_MAXNODEDEPTH( _depth )		( _depth )
/* Node list */
#define	OBJ_NODELIST( _ptr )			( _ptr )
/* Num of Matrix pallette */
#define	OBJ_N_MTXPAL( _num )			( _num )
/* Num of Subobj */
#define	OBJ_N_SUBOBJ( _num )			( _num )
/* Subobject list */
#define	OBJ_SUBOBJ_LIST( _ptr )			( _ptr )
/* Texture num */
#define OBJ_N_TEX( _num )				( _num )
/* Object Type */
#define OBJ_TYPE( _type )				( _type )
/* Object format version */
#define OBJ_VERSION( _ver )				( _ver )
/* Bounding-box size */
#define	OBJ_BBXSIZE( _x )				( _x )
#define	OBJ_BBYSIZE( _y )				( _y )
#define	OBJ_BBZSIZE( _z )				( _z )

/* Subobject */
#define	SUBOBJ( _type, _nmsst, _msstlstptr, _ntex, _texptr)	{ _type, _nmsst, _msstlstptr, _ntex, _texptr }
/* Subobject type */
#define	SOBJ_TYPE( _type )				( _type )
/* Number of meshset */
#define	SOBJ_N_MSST( _num )				( _num )
/* Meshset list */
#define	SOBJ_MSSTLIST( _ptr )			( _ptr )
/* Texture num */
#define SOBJ_N_TEX( _num )				( _num )
/* Texture number list */
#define SOBJ_TEXNUMLIST( _ptr )			( _ptr )

/* Node */
#define	NODE( _type, _nmtx, _prnt, _chld, _sibl, _trn, _rot, _scl, _imtx, _cen, _rdus, _usr, _rsv0, _rsv1, _rsv2 ) \
			{ _type, _nmtx, _prnt, _chld, _sibl, _trn, _rot, _scl, _imtx, _cen, _rdus, _usr, _rsv0, _rsv1, _rsv2 }
#define	NODE_TYPE( _type )				( _type )
#define	NODE_TRN( _x, _y, _z )			TRN( ( _x ), ( _y ), ( _z ) )
#define	NODE_ROT( _x, _y, _z )			ROT( ( _x ), ( _y ), ( _z ) )
#define	NODE_SCL( _x, _y, _z )			SCL( ( _x ), ( _y ), ( _z ) )
#define	NODE_INVINIT_MTX				MTX
#define	NODE_CENTER( _x, _y, _z )		TRN( ( _x ), ( _y ), ( _z ) )
#define	NODE_RADIUS( _r )				( _r )
#define	NODE_MATRIX( _idx )				( _idx )
#define	NODE_PARENT( _idx )				( _idx )
#define	NODE_CHILD( _idx )				( _idx )
#define	NODE_SIBLING( _idx )			( _idx )
#define	NODE_USER( _val )				( _val )
#define	NODE_BONE_LEN( _len )			( _len )
#define	NODE_BBXSIZE( _x )				( _x )
#define	NODE_BBYSIZE( _y )				( _y )
#define	NODE_BBZSIZE( _z )				( _z )
#define	NODE_RSV0( _val )				( _val )
#define	NODE_RSV1( _val )				( _val )
#define	NODE_RSV2( _val )				( _val )

/* Node name list */
#define	NODENAMELIST( _type, _num, _ptr )	{ _type, _num, _ptr }
#define	NDNL_TYPE( _type )			( _type )
#define	NDNL_N_NODE( _num )			( _num )
#define	NDNL_NAMELIST( _ptr )		( _ptr )
/* Nodename */
#define	NODENAME( _idx, _ptr )		{ _idx, _ptr }
#define	NDNM_IDX( _idx )			( _idx )
#define	NDNM_NAME( _ptr )			( _ptr )

/* Meshset */
#if ( NND_PLATFORM == NND_PLATFORM_XB ) || ( NND_PLATFORM == NND_PLATFORM_DX8 ) || ( NND_PLATFORM == NND_PLATFORM_DX9 ) || ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#define	MSST( _center, _radius, _node, _matrix, _material, _vtxlist, _primlist, _shader )	\
			{ _center, _radius, _node, _matrix, _material, _vtxlist, _primlist, _shader }
#elif ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#define	MSST( _center, _radius, _node, _matrix, _material, _vtxlist, _primlist, _rsv2, _rsv1, _rsv0 )	\
			{ _center, _radius, _node, _matrix, _material, _vtxlist, _primlist, _rsv2, _rsv1, _rsv0 }
#elif ( NND_PLATFORM == NND_PLATFORM_PSP )
#define	MSST( _center, _radius, _node, _matrix, _material, _vtxlist, _primlist )	\
			{ _center, _radius, _node, _matrix, _material, _vtxlist, _primlist }
#elif ( NND_PLATFORM == NND_PLATFORM_PS3 )
#define	MSST( _center, _radius, _node, _matrix, _material, _vtxlist, _primlist, _rsv2, _rsv1, _rsv0 )	\
			{ _center, _radius, _node, _matrix, _material, _vtxlist, _primlist, _rsv2, _rsv1, _rsv0 }
#else
#define	MSST( _center, _radius, _node, _matrix, _material, _vtxlist, _primlist )	\
			{ _center, _radius, _node, _matrix, _material, _vtxlist, _primlist }
#endif
/* Meshset center position */
#define	MSST_CENTER( _x, _y, _z )		TRN( ( _x ), ( _y ), ( _z ) )
/* Meshset Radius */
#define	MSST_RADIUS( _r )				( _r )
/* Meshset Node index */
#define	MSST_NODE( _idx )				( _idx )
/* Meshset Matrix index */
#define	MSST_MATRIX( _idx )				( _idx )
/* Material index */
#define	MSST_MATERIAL( _idx )			( _idx )
/* Vertex list index */
#define	MSST_VTXLIST( _idx )			( _idx )
/* Primitive list index */
#define	MSST_PRIMLIST( _idx )			( _idx )
/* Reserved */
#define	MSST_RESERVED0( _rsv )			( _rsv )
#define	MSST_RESERVED1( _rsv )			( _rsv )
#define	MSST_RESERVED2( _rsv )			( _rsv )

/* Material name list */
#define	MATERIALNAMELIST( _type, _num, _ptr )	{ _type, _num, _ptr }
#define	MANL_TYPE( _type )			( _type )
#define	MANL_N_MATERIAL( _num )		( _num )
#define	MANL_NAMELIST( _ptr )		( _ptr )
/* Materialname */
#define	MATERIALNAME( _idx, _ptr )	{ _idx, _ptr }
#define	MANM_IDX( _idx )			( _idx )
#define	MANM_NAME( _ptr )			( _ptr )

/*****************/
/* Common Vertex */
/*****************/
/* Vertex */
#define CMN_VTX_POS( _x, _y, _z )					{ ( _x ), ( _y ), ( _z ) }
#define CMN_VTX_NRM( _x, _y, _z )					{ ( _x ), ( _y ), ( _z ) }
#define CMN_VTX_COL( _r, _g, _b, _a )				{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define CMN_VTX_TEX( _u, _v )						{ ( _u ), ( _v ) }
#define CMN_VTX_WGT2( _i0, _i1, _w )				{ ( _i0 ), ( _i1 ), ( _w ) }
#define CMN_VTX_WGT( _i, _w )						{ ( _i ), ( _w ) }
#define CMN_VTX_WGT4( _w0, _w1, _w2, _w3 )			{ _w0, _w1, _w2, _w3 }
#define CMN_VTX_PW2( _p, _w )						{ _p, _w }
#define CMN_VTX_PW4( _p, _w )						{ _p, _w }
#define CMN_VTX_PN( _p, _n )						{ _p, _n }
#define CMN_VTX_PNW2( _p, _n, _w )					{ _p, _n, _w }
#define CMN_VTX_PNW4( _p, _n, _w )					{ _p, _n, _w }
#define CMN_VTX_TEX2( _t0, _t1 )					{ _t0, _t1 }

/* Common Descriptor */
#define CMN_VTX_ARRAY( _type, _num, _size, _ptr )	{ ( _type ), ( _num ), ( _size ), ( _ptr ) }
#define CMN_VTX_ARRAY_TYPE( _type )					( _type )
#define CMN_VTX_ARRAY_NUM( _num )					( _num )
#define CMN_VTX_ARRAY_SIZE( _size )					( _size )
#define CMN_VTX_ARRAY_PTR( _ptr )					( _ptr )
#define CMN_VTX_DESC( _a0, _a1, _a2, _a3 )			{ _a0, _a1, _a2, _a3 }

/* Primitive */
#define CMN_PRIM_STRIP_R( _type, _size, _num, _lenptr, _ptr )	\
			{ ( _type ), ( _size ), ( _num ), ( _lenptr ), ( _ptr ) }
#define CMN_PRIM_STRIP_L( _type, _size, _num, _lenptr, _ptr )	\
			{ ( _type ), ( _size ), ( _num ), ( _lenptr ), ( _ptr ) }
#define CMN_PRIM_IDXTYPE( _type )					( _type )
#define CMN_PRIM_NIDXSET( _size )					( _size )
#define CMN_PRIM_NSTRIP( _num )						( _num )
#define CMN_PRIM_LENPTR( _lenptr )					( _lenptr )
#define CMN_PRIM_PTR( _ptr )						( _ptr )

/* Morph Target */
/* Vertex */
#define CMN_MTVTX_POS( _x, _y, _z )					{ ( _x ), ( _y ), ( _z ) }
#define CMN_MTVTX_NRM( _x, _y, _z )					{ ( _x ), ( _y ), ( _z ) }
#define CMN_MTVTX_COL( _r, _g, _b, _a )				{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define CMN_MTVTX_TEX( _u, _v )						{ ( _u ), ( _v ) }
#define CMN_MTVTX_PN( _p, _n )						{ _p, _n }
#define CMN_MTVTX_TEX2( _t0, _t1 )					{ _t0, _t1 }

/* Common Descriptor */
#define CMN_MTVTX_ARRAY( _type, _num, _size, _ptr )	{ ( _type ), ( _num ), ( _size ), ( _ptr ) }
#define CMN_MTVTX_ARRAY_TYPE( _type )				( _type )
#define CMN_MTVTX_ARRAY_NUM( _num )					( _num )
#define CMN_MTVTX_ARRAY_SIZE( _size )				( _size )
#define CMN_MTVTX_ARRAY_PTR( _ptr )					( _ptr )
#define CMN_MTVTX_DESC( _a0, _a1, _a2, _a3 )		{ _a0, _a1, _a2, _a3 }

/**********/
/* Motion */
/**********/
/* Submotion */
#define	SUBMOT( _type, _iptype, _id, _sfrm, _efrm, _skfrm, _ekfrm, _nkfrm, _ksize, _klist )	\
				{ _type, _iptype, _id, _sfrm, _efrm, _skfrm, _ekfrm, _nkfrm, _ksize, _klist }
#define	SMOT_TYPE( _type )				( _type )
#define	SMOT_IPTYPE( _type )			( _type )
#define	SMOT_REPTYPE( _type )			( _type )
#define	SMOT_ID( _id )					( _id )
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#define	SMOT_SUBID0_SUBID1( _id0, _id1 )	( ((_id0) << 16 ) | (_id1) )
#else
#define	SMOT_SUBID0_SUBID1( _id0, _id1 )	( ((_id1) << 16 ) | (_id0) )
#endif
#define	SMOT_STARTFRAME( _frame )		( _frame )
#define	SMOT_ENDFRAME( _frame )			( _frame )
#define	SMOT_STARTKEY( _frame )			( _frame )
#define	SMOT_ENDKEY( _frame )			( _frame )
#define	SMOT_N_KEYFRAME( _num )			( _num )
#define	SMOT_KEYSIZE( _size )			( _size )
#define	SMOT_KEYLIST( _ptr )			( _ptr )
/* Motion */
#define	MOTION( _type, _sfrm, _efrm, _num, _ptr )	\
				{ _type, _sfrm, _efrm, _num, _ptr, 30.f, 0, 0 }
#define	MOTION2( _type, _sfrm, _efrm, _num, _ptr, _rate, _rsv0, _rsv1 )	\
				{ _type, _sfrm, _efrm, _num, _ptr, _rate, _rsv0, _rsv1 }
#define	MOT_TYPE( _type )				( _type )
#define	MOT_START_FRAME( _sfrm )		( _sfrm )
#define	MOT_END_FRAME( _efrm )			( _efrm )
#define	MOT_START_KEYFRAME( _skfrm )	( _skfrm )
#define	MOT_END_KEYFRAME( _ekfrm )		( _ekfrm )
#define	MOT_N_SUBMOT( _num )			( _num )
#define	MOT_SUBMOTLIST( _ptr )			( _ptr )
#define	MOT_FRAMERATE( _rate )			( _rate )
#define	MOT_RSV0( _val )				( _val )
#define	MOT_RSV1( _val )				( _val )
/* Motion Key */
#define	MOTKEY( _frame, _value )		{ _frame, _value }
#define	MOTKEY_SISP( _frame, _value, _shandle )	{ _frame, _value, _shandle }
#define	MOTKEY_BZR( _frame, _value, _bhandle )	{ _frame, _value, _bhandle }
#define	MFRM_FLT( _frame )				( _frame )
#define	MFRM_S16( _sframe )				( _sframe )
#define	SISP_IN( _d )					{ ( _d ) }
#define	SISP_OUT( _d )					{ ( _d ) }
#define	SISP_IN_A32( _deg )				{ NNM_DEGtoA32FLT( _deg ) }
#define	SISP_OUT_A32( _deg )			{ NNM_DEGtoA32FLT( _deg ) }
#define	SISP_IN_A16( _deg )				{ NNM_DEGtoA16FLT( _deg ) }
#define	SISP_OUT_A16( _deg )			{ NNM_DEGtoA16FLT( _deg ) }
#define	MHDL_SISP( _in, _out )			{ _in, _out }
#define	BZR_IN( _x, _y )				{ ( _x ), ( _y ) }
#define	BZR_OUT( _x, _y )				{ ( _x ), ( _y ) }
#define	BZR_IN_A32( _x, _deg )			{ ( _x ), NNM_DEGtoA32FLT( _deg ) }
#define	BZR_OUT_A32( _x, _deg )			{ ( _x ), NNM_DEGtoA32FLT( _deg ) }
#define	MHDL_BZR( _in, _out )			{ _in, _out }
#define	MVAL_FLT( _val )				( _val )
#define	MVAL_FLT_2( _v0, _v1 )			{ _v0, _v1 }
#define	MVAL_FLT_3( _v0, _v1, _v2 )		{ _v0, _v1, _v2 }
#define	MVAL_FLT_4( _v0, _v1, _v2, _v3 )	{ _v0, _v1, _v2, _v3 }
#define	MVAL_A32( _deg )				( NNM_DEGtoA32( _deg ) )
#define	MVAL_A32_3( _d0, _d1, _d2 )	\
				{ NNM_DEGtoA32( _d0 ), NNM_DEGtoA32( _d1 ), NNM_DEGtoA32( _d2 ) }
#define	MVAL_S32( _val )				( _val )
#define	MVAL_S32_3( _v0, _v1, _v2 )		{ _v0, _v1, _v2 }
#define	MVAL_A16( _deg )				( NNM_DEGtoA16( _deg ) )
#define	MVAL_A16_3( _d0, _d1, _d2 )	\
				{ NNM_DEGtoA16( _d0 ), NNM_DEGtoA16( _d1 ), NNM_DEGtoA16( _d2 ) }
#define	MVAL_U32( _val )				( _val )

/*********************/
/* Morph target list */
/*********************/
/* Morph target list */
#define	MRPHTGTLIST( _nmtgt, _mtgtptrlst )	{ _nmtgt, _mtgtptrlst }
/* Number of morph target */
#define	MRPH_N_MRPHTGT( _nmtgt )			( _nmtgt )
/* Morph target pointer */
#define	MRPH_MRPHTGTPTR_LIST( _ptr )		( _ptr )
/* Morph target pointer */
#define	MRPHTGTPTR( _nvtxlst, _mtgtptr )	{ _nvtxlst, _mtgtptr }
/* Number of Vertex list pointer for morph target */
#define	MRPH_N_VTXLIST( _num )				( _num )
/* Vertex list pointer list for morph target */
#define	MRPH_VTXLISTPTR_LIST( _ptr )		( _ptr )

/* Morph target name list */
#define	MTNAMELIST( _type, _num, _ptr )	{ _type, _num, _ptr }
#define	MTNL_TYPE( _type )			( _type )
#define	MTNL_N_MORPHTARGET( _num )		( _num )
#define	MTNL_NAMELIST( _ptr )		( _ptr )
/* Morph target name */
#define	MTNAME( _idx, _ptr )		{ _idx, _ptr }
#define	MTNM_IDX( _idx )			( _idx )
#define	MTNM_NAME( _ptr )			( _ptr )

/****************/
/* Texture list */
/****************/
/* Texture number list */
#define	STN_TEX( _id )					( _id )

/* Texture file */
#define	TEXFILE( _fname, _filter)	\
				{ 0, _fname, _filter, 0, 0}
#define	FULLTEXFILE( _ftype, _fname, _filter, _glbidx, _bank)	\
				{ _ftype, _fname, _filter, _glbidx, _bank}
#define	TF_FTYPE( _ftype )				( _ftype )
#define	TF_FILENAME( _fname )			( _fname )
#define	TF_FILTER( _min , _mag )		( _min ), ( _mag )
#define	TF_GLBIDX( _idx )				( _idx )
#define	TF_BANK( _bank )				( _bank )

/* Texture file list */
#define TEXFILELIST( _num, _ptr) \
                { _num, _ptr }
#define TFL_N_TEXFILE( _num )			( _num )
#define TFL_TEXFILE( _ptr)				( _ptr )


/**********/
/* Camera */
/**********/
/* Camera pointer */
#define	CAMERAPTR( _type, _ptr )		{ _type, _ptr }
/* Camera type */
#define	CAM_TYPE( _type )				( _type )
/* Camera data pointer */
#define	CAM_CAM( _ptr )					( _ptr )
/* Target roll camera */
#define	CAM_TGT_ROLL( _usr, _fov, _asp, _zn, _zf, _pos, _tgt, _rol )	\
				{ _usr, _fov, _asp, _zn, _zf, _pos, _tgt, _rol }
/* Target up-vector camera */
#define	CAM_TGT_UPVEC( _usr, _fov, _asp, _zn, _zf, _pos, _tgt, _upv )	\
				{ _usr, _fov, _asp, _zn, _zf, _pos, _tgt, _upv }
/* Target up-target camera */
#define	CAM_TGT_UPTGT( _usr, _fov, _asp, _zn, _zf, _pos, _tgt, _upt )	\
				{ _usr, _fov, _asp, _zn, _zf, _pos, _tgt, _upt }
/* Rotation camera */
#define	CAM_ROTATION( _usr, _fov, _asp, _zn, _zf, _pos, _rotyp, _rot )	\
				{ _usr, _fov, _asp, _zn, _zf, _pos, _rotyp, _rot }
/* Camera parameter */
#define	CAM_USER( _usr )				( _usr )
#define	CAM_FOVY( _fov )				( NNM_DEGtoA32( _fov ) )
#define	CAM_ASPECT( _asp )				( _asp )
#define	CAM_ZNEAR( _zn )				( _zn )
#define	CAM_ZFAR( _zf )					( _zf )
#define	CAM_POS( _x, _y, _z )			TRN( ( _x ), ( _y ), ( _z ) )
#define	CAM_ROTTYPE( _rotyp )			( _rotyp )
#define	CAM_ROT( _x, _y, _z )			ROT( ( _x ), ( _y ), ( _z ) )
#define	CAM_TGT( _x, _y, _z )			TRN( ( _x ), ( _y ), ( _z ) )
#define	CAM_ROLL( _deg )				( NNM_DEGtoA16( _deg ) )
#define	CAM_UPVEC( _x, _y, _z )			TRN( ( _x ), ( _y ), ( _z ) )
#define	CAM_UPTGT( _x, _y, _z )			TRN( ( _x ), ( _y ), ( _z ) )


/*********/
/* Light */
/*********/
/* Light pointer */
#define	LIGHTPTR( _type, _ptr )			{ _type, _ptr }
/* Light type */
#define	LIT_TYPE( _type )				( _type )
/* Light data pointer */
#define	LIT_LIT( _ptr )					( _ptr )
/* Parallel light */
#define	LIT_PARA( _usr, _col, _inten, _dir )	{ _usr, _col, _inten, _dir }
/* Point light */
#define	LIT_PNT( _usr, _col, _inten, _pos, _fos, _foe )	\
						{ _usr, _col, _inten, _pos, _fos, _foe }
/* Target spot light */
#define	LIT_TGT_SPOT( _usr, _col, _inten, _pos, _tgt, _iang, _oang, _fos, _foe )	\
			{ _usr, _col, _inten, _pos, _tgt, _iang, _oang, _fos, _foe }
/* Rotation spot light */
#define	LIT_ROT_SPOT( _usr, _col, _inten, _pos, _rotyp, _rot, _iang, _oang, _fos, _foe )	\
			{ _usr, _col, _inten, _pos, _rotyp, _rot, _iang, _oang, _fos, _foe }
/* Target directional light */
#define	LIT_TGT_DIR( _usr, _col, _inten, _pos, _tgt, _irng, _orng, _fos, _foe )	\
			{ _usr, _col, _inten, _pos, _tgt, _irng, _orng, _fos, _foe }
/* Rotation directional light */
#define	LIT_ROT_DIR( _usr, _col, _inten, _pos, _rotyp, _rot, _irng, _orng, _fos, _foe )	\
			{ _usr, _col, _inten, _pos, _rotyp, _rot, _irng, _orng, _fos, _foe }
/* Light parameter */
#define	LIT_USER( _usr )			( _usr )
#define	LIT_COL( _r, _g, _b, _a )	{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define	LIT_INTENSITY( _int )		( _int )
#define	LIT_DIRECTION(_x, _y, _z )	TRN( ( _x ), ( _y ), ( _z ) )
#define	LIT_POS( _x, _y, _z )		TRN( ( _x ), ( _y ), ( _z ) )
#define	LIT_FALLOFFSTART( _dist )	( _dist )
#define	LIT_FALLOFFEND( _dist )		( _dist )
#define	LIT_TGT( _x, _y, _z )		TRN( ( _x ), ( _y ), ( _z ) )
#define	LIT_INNERANG( _deg )		( NNM_DEGtoA32( _deg ) )
#define	LIT_OUTERANG( _deg )		( NNM_DEGtoA32( _deg ) )
#define	LIT_INNERRANGE( _irng )		( _irng )
#define	LIT_OUTERRANGE( _orng )		( _orng )
#define	LIT_ROTTYPE( _rotyp )		( _rotyp )
#define	LIT_ROT( _x, _y, _z )		ROT( ( _x ), ( _y ), ( _z ) )

#ifdef __cplusplus
}
#endif /* __cplusplus */

/********************************************/
/* PlayStation2 Default type and structures */
/********************************************/
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nntps2.h"
#include "nntnodeex.h"
#endif //PlayStation2

/****************************************/
/* GAMECUBE Default type and structures */
/****************************************/
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nntgc.h"
#endif //GAMECUBE

/************************************/
/* Xbox Default type and structures */
/************************************/
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nntdx.h"
#endif //Xbox

/**********************************/
/* PC Default type and structures */
/**********************************/
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
#include "nntdx.h"
#include "nntnodeex.h"
#endif

/**************************************/
/* OpenGL Default type and structures */
/**************************************/
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#include "nntgl.h"
#endif //OpenGL

/***********************************/
/* PSP Default type and structures */
/***********************************/
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#include "nntpsp.h"
#include "nntnodeex.h"
#endif //PSP

/***********************************/
/* PS3 Default type and structures */
/***********************************/
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
#include "nntps3.h"
#endif //PS3

/************************************/
/* DXG20 Default type and structures */
/************************************/
#if ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#include "nntdxg20.h"
#endif //DXG20


#endif //__NNT_H__


/* End of file */
