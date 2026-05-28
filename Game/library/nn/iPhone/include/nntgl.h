/*---------------------------------------------------------------------------

    NN text format macro for OpenGL

    Copyright (C) 2004-2005 SEGA Corporation Creative Center Graphics Sect.
    All Rights Reserved.

    Module  : NN text format
    File    : nntgl.h
    Create  : 2004/06/15
    Modify  : 2004/08/05
    Modify  : 2004/08/20 NNS_MATERIAL_TEXMAP_DESC変更
    Modify  : 2004/12/07 Tangent,Binormal追加
    Modify  : 2004/12/24 NN標準シェーダ用マテリアル追加
    Modify  : 2005/02/17 1-bone weight対応
    Modify  : 2005/04/12 マテリアルにUserProfile追加
    Modify  : 2009/09/29 nntvertexgl.hをinclude
    Version : 1.01.00
    Note    : 

---------------------------------------------------------------------------*/

#ifndef	__NNTGL_H__
#define	__NNTGL_H__

// Vertex data
#define POS_4S( _x, _y, _z, _w )			((Sint16)( _x )), ((Sint16)( _y )), ((Sint16)( _z )), ((Sint16)( _w ))
#define POS_4I( _x, _y, _z, _w )			( _x ), ( _y ), ( _z ), ( _w )
#define	POS_3F( _x, _y, _z )				( _x ), ( _y ), ( _z )
#define	POS_3D( _x, _y, _z )				( _x ), ( _y ), ( _z )

#define WGT_1UB( _w0 )						((Uint8 )( _w0 ))
#define WGT_1US( _w0 )						((Uint16)( _w0 ))
#define WGT_1UI( _w0 )						((Uint32)( _w0 ))
#define WGT_1F( _w0 )						( _w0 )
#define WGT_1D( _w0 )						( _w0 )

#define WGT_2UB( _w0, _w1 )					((Uint8 )( _w0 )), ((Uint8 )( _w1 ))
#define WGT_2US( _w0, _w1 )					((Uint16)( _w0 )), ((Uint16)( _w1 ))
#define WGT_2UI( _w0, _w1 )					((Uint32)( _w0 )), ((Uint32)( _w1 ))
#define WGT_2F( _w0, _w1 )					( _w0 ), ( _w1 )
#define WGT_2D( _w0, _w1 )					( _w0 ), ( _w1 )

#define WGT_3UB( _w0, _w1, _w2 )			((Uint8 )( _w0 )), ((Uint8 )( _w1 )), ((Uint8 )( _w2 ))
#define WGT_3US( _w0, _w1, _w2 )			((Uint16)( _w0 )), ((Uint16)( _w1 )), ((Uint16)( _w2 ))
#define WGT_3UI( _w0, _w1, _w2 )			((Uint32)( _w0 )), ((Uint32)( _w1 )), ((Uint32)( _w2 ))
#define WGT_3F( _w0, _w1, _w2 )				( _w0 ), ( _w1 ), ( _w2 )
#define WGT_3D( _w0, _w1, _w2 )				( _w0 ), ( _w1 ), ( _w2 )

#define MTXIDX_1UB( _i0 )					((Uint8 )( _i0 ))
#define MTXIDX_1US( _i0 )					((Uint16)( _i0 ))
#define MTXIDX_1UI( _i0 )					((Uint32)( _i0 ))

#define MTXIDX_2UB( _i0, _i1 )				((Uint8 )( _i0 )), ((Uint8 )( _i1 ))
#define MTXIDX_2US( _i0, _i1 )				((Uint16)( _i0 )), ((Uint16)( _i1 ))
#define MTXIDX_2UI( _i0, _i1 )				((Uint32)( _i0 )), ((Uint32)( _i1 ))

#define MTXIDX_3UB( _i0, _i1, _i2 )			((Uint8 )( _i0 )), ((Uint8 )( _i1 )), ((Uint8 )( _i2 ))
#define MTXIDX_3US( _i0, _i1, _i2 )			((Uint16)( _i0 )), ((Uint16)( _i1 )), ((Uint16)( _i2 ))
#define MTXIDX_3UI( _i0, _i1, _i2 )			((Uint32)( _i0 )), ((Uint32)( _i1 )), ((Uint32)( _i2 ))

#define MTXIDX_4UB( _i0, _i1, _i2, _i3 )	((Uint8 )( _i0 )), ((Uint8 )( _i1 )), ((Uint8 )( _i2 )), ((Uint8 )( _i3 ))
#define MTXIDX_4US( _i0, _i1, _i2, _i3 )	((Uint16)( _i0 )), ((Uint16)( _i1 )), ((Uint16)( _i2 )), ((Uint16)( _i3 ))
#define MTXIDX_4UI( _i0, _i1, _i2, _i3 )	((Uint32)( _i0 )), ((Uint32)( _i1 )), ((Uint32)( _i2 )), ((Uint32)( _i3 ))

#define NRM_3B( _x, _y, _z )				((Sint8 )( _x )), ((Sint8 )( _y )), ((Sint8 )( _z ))
#define NRM_3S( _x, _y, _z )				((Sint16)( _x )), ((Sint16)( _y )), ((Sint16)( _z ))
#define NRM_3I( _x, _y, _z )				( _x ), ( _y ), ( _z )
#define NRM_3F( _x, _y, _z )				( _x ), ( _y ), ( _z )
#define NRM_3D( _x, _y, _z )				( _x ), ( _y ), ( _z )

#define TAN_3B( _x, _y, _z )				((Sint8 )( _x )), ((Sint8 )( _y )), ((Sint8 )( _z ))
#define TAN_3S( _x, _y, _z )				((Sint16)( _x )), ((Sint16)( _y )), ((Sint16)( _z ))
#define TAN_3I( _x, _y, _z )				( _x ), ( _y ), ( _z )
#define TAN_3F( _x, _y, _z )				( _x ), ( _y ), ( _z )
#define TAN_3D( _x, _y, _z )				( _x ), ( _y ), ( _z )

#define BNRM_3B( _x, _y, _z )				((Sint8 )( _x )), ((Sint8 )( _y )), ((Sint8 )( _z ))
#define BNRM_3S( _x, _y, _z )				((Sint16)( _x )), ((Sint16)( _y )), ((Sint16)( _z ))
#define BNRM_3I( _x, _y, _z )				( _x ), ( _y ), ( _z )
#define BNRM_3F( _x, _y, _z )				( _x ), ( _y ), ( _z )
#define BNRM_3D( _x, _y, _z )				( _x ), ( _y ), ( _z )

#define COL_4UB( _r, _g, _b, _a )			((Uint8 )( _r )), ((Uint8 )( _g )), ((Uint8 )( _b )), ((Uint8 )( _a ))
#define COL_4US( _r, _g, _b, _a )			((Uint16)( _r )), ((Uint16)( _g )), ((Uint16)( _b )), ((Uint16)( _a ))
#define COL_4UI( _r, _g, _b, _a )			((Uint32)( _r )), ((Uint32)( _g )), ((Uint32)( _b )), ((Uint32)( _a ))
#define COL_4F( _r, _g, _b, _a )			( _r ), ( _g ), ( _b ), ( _a )
#define COL_4D( _r, _g, _b, _a )			( _r ), ( _g ), ( _b ), ( _a )

#define COL_3UB( _r, _g, _b )				((Uint8 )( _r )), ((Uint8 )( _g )), ((Uint8 )( _b ))
#define COL_3US( _r, _g, _b )				((Uint16)( _r )), ((Uint16)( _g )), ((Uint16)( _b ))
#define COL_3UI( _r, _g, _b )				((Uint32)( _r )), ((Uint32)( _g )), ((Uint32)( _b ))
#define COL_3F( _r, _g, _b )				( _r ), ( _g ), ( _b )
#define COL_3D( _r, _g, _b )				( _r ), ( _g ), ( _b )

#define COL2_3UB( _r, _g, _b )				((Uint8 )( _r )), ((Uint8 )( _g )), ((Uint8 )( _b ))
#define COL2_3US( _r, _g, _b )				((Uint16)( _r )), ((Uint16)( _g )), ((Uint16)( _b ))
#define COL2_3UI( _r, _g, _b )				((Uint32)( _r )), ((Uint32)( _g )), ((Uint32)( _b ))
#define COL2_3F( _r, _g, _b )				( _r ), ( _g ), ( _b )
#define COL2_3D( _r, _g, _b )				( _r ), ( _g ), ( _b )

#define TEX_2S( _s, _t )					((Sint16)( _s )), ((Sint16)( _t ))
#define TEX_2F( _s, _t )					( _s ), ( _t )
#define TEX_2D( _s, _t )					( _s ), ( _t )

#define	POS_3F_DIFF( _x, _y, _z )			( _x ), ( _y ), ( _z )
#define NRM_3F_DIFF( _x, _y, _z )			( _x ), ( _y ), ( _z )

// Vertex Array
#define VTXARRAY( _type, _size, _dtype, _stride, _ptr )				\
				{ _type, _size, _dtype, _stride, _ptr }

#define VA_TYPE( _type )					( _type )
#define VA_SIZE( _size )					( _size )
#define VA_DATATYPE( _dtype )				( _dtype )
#define VA_STRIDE( _stride )				( _stride )
#define VA_PTR( _ptr )						( _ptr )

// Vertex descriptor
#define	VTXDESC( _type, _nvtx, _narray, _aptr, _bsize, _pbuf, _nmtx, _mtxptr, _bufname )		\
				{ _type, _nvtx, _narray, _aptr, _bsize, _pbuf, _nmtx, _mtxptr, _bufname }

#define	VDESC_TYPE( _type )					( _type )
#define	VDESC_N_VTX( _nvtx )				( _nvtx )
#define	VDESC_N_ARRAY( _narray )			( _narray )
#define	VDESC_P_ARRAY( _aptr )				( _aptr )
#define	VDESC_BUFSIZE( _bsize )				( _bsize )
#define	VDESC_P_BUF( _pbuf )				( _pbuf )
#define	VDESC_N_MTX( _nmtx )				( _nmtx )
#define	VDESC_P_MTX( _mtxptr )				( _mtxptr )
#define	VDESC_BUFNAME( _bufname )			( _bufname )

// Primitive descriptor
#define PRIMDESC( _mode, _cptr, _dtype, _iptr, _nprim, _bsize, _pbuf, _bufname )			\
				{ _mode, _cptr, _dtype, _iptr, _nprim, _bsize, _pbuf, _bufname }

#define PDESC_MODE( _mode )					( _mode )
#define PDESC_P_COUNT( _pcount )			( _pcount )
#define PDESC_DATATYPE( _dtype )			( _dtype )
#define PDESC_P_INDEX( _pindex )			( _pindex )
#define PDESC_N_PRIM( _nprim )				( _nprim )
#define	PDESC_BUFSIZE( _bsize )				( _bsize )
#define	PDESC_P_BUF( _pbuf )				( _pbuf )
#define	PDESC_BUFNAME( _bufname )			( _bufname )

// Pointer list
#define	MATLISTPTR( _type, _ptr )			{ (_type), (_ptr) }
#define	MAT_TYPE( _type )					(_type)
#define	MAT_PTR( _ptr )						(_ptr)

#define	PRIMLISTPTR( _type, _ptr )			{ (_type), (_ptr) }
#define	PRIM_TYPE( _type )					(_type)
#define	PRIM_LIST( _ptr )					(_ptr)

#define	VTXLISTPTR( _type, _ptr )			{ (_type), (_ptr) }
#define	VTX_TYPE( _type )					(_type)
#define	VTX_LIST( _ptr )					(_ptr)

// Material color
#define	MAT_COLOR( _flag, _ambi, _diff, _spec, _emis, _shin, _vcol )\
				{ _flag,  _ambi, _diff, _spec, _emis, _shin, _vcol }

#define	MAT_COLFLAG( _f )					( _f )
#define	MAT_AMBIENT( _r, _g, _b, _a )		{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define	MAT_DIFFUSE( _r, _g, _b, _a )		{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define	MAT_SPECULAR( _r, _g, _b, _a )		{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define	MAT_EMISSION( _r, _g, _b, _a )		{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define	MAT_SHININESS( _shin )				( _shin )
#define	MAT_VTXCOLMAT( _vcol )				( _vcol )

// Material logic
#define MAT_LOGIC( _flag, _srgb, _drgb, _sa, _da, _bcol, _bop, _lop, _afunc, _dfunc, _aref )	\
				{ _flag, _srgb, _drgb, _sa, _da, _bcol, _bop, _lop, _afunc, _dfunc, _aref }

#define MAT_LOGFLAG( _flag )				( _flag )
#define MAT_SRC_FACTOR_RGB( _sfactor )		( _sfactor )
#define MAT_DST_FACTOR_RGB( _dfactor )		( _dfactor )
#define MAT_SRC_FACTOR_ALPHA( _sfactor )	( _sfactor )
#define MAT_DST_FACTOR_ALPHA( _dfactor )	( _dfactor )
#define	MAT_BLENDCOL( _r, _g, _b, _a )		{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define MAT_BLENDOP( _op )					( _op )
#define MAT_LOGICOP( _op )					( _op )
#define MAT_ALPHAFUNC( _func )				( _func )
#define MAT_DEPTHFUNC( _func )				( _func )
#define MAT_ALPHAREF( _ref )				( _ref )

// Material descriptor
#define	MAT_DESC( _flag, _user, _pcol, _pbcol, _plog, _ntex, _ptex )	\
			{ _flag, _user, _pcol, _pbcol, _plog, _ntex, _ptex }

#define	MAT_FLAG( _flag )					( _flag )
#define MAT_USER( _user )					( _user )
#define	MAT_P_COLOR( _ptr )					( _ptr )
#define	MAT_P_BACKCOL( _ptr )				( _ptr )
#define	MAT_P_LOGIC( _ptr )					( _ptr )
#define MAT_N_TEX( _num )					( _num )
#define MAT_P_TEXDESC( _ptr )				( _ptr )

// Texture combine
#define TEX_COMBINE( _crgb, _s0rgb, _op0rgb, _s1rgb, _op1rgb, _s2rgb, _op2rgb,	\
						_ca, _s0a, _op0a, _s1a, _op1a, _s2a, _op2a )		\
					{ _crgb, _s0rgb, _op0rgb, _s1rgb, _op1rgb, _s2rgb, _op2rgb,	\
						_ca, _s0a, _op0a, _s1a, _op1a, _s2a, _op2a }

#define TEX_COMBINE_RGB( _val )				((Uint16)( _val ))
#define TEX_SOURCE0_RGB( _val )				((Uint16)( _val ))
#define TEX_OPERAND0_RGB( _val )			((Uint16)( _val ))
#define TEX_SOURCE1_RGB( _val )				((Uint16)( _val ))
#define TEX_OPERAND1_RGB( _val )			((Uint16)( _val ))
#define TEX_SOURCE2_RGB( _val )				((Uint16)( _val ))
#define TEX_OPERAND2_RGB( _val )			((Uint16)( _val ))
#define TEX_COMBINE_ALPHA( _val )			((Uint16)( _val ))
#define TEX_SOURCE0_ALPHA( _val )			((Uint16)( _val ))
#define TEX_OPERAND0_ALPHA( _val )			((Uint16)( _val ))
#define TEX_SOURCE1_ALPHA( _val )			((Uint16)( _val ))
#define TEX_OPERAND1_ALPHA( _val )			((Uint16)( _val ))
#define TEX_SOURCE2_ALPHA( _val )			((Uint16)( _val ))
#define TEX_OPERAND2_ALPHA( _val )			((Uint16)( _val ))

// Texture border color
#define TEX_BORDER_COLOR( _r, _g, _b, _a )	{ ( _r ), ( _g ), ( _b ), ( _a ) }

// Texture filter mode
#define TEX_FILTER_MODE( _mag, _min, _aniso )	{ ( _mag ), ( _min ), ( _aniso ) }
#define TEX_MAG_FILTER( _filter )			( _filter )
#define TEX_MIN_FILTER( _filter )			( _filter )
#define TEX_ANISOTROPY( _aniso )			( _aniso )

// Texture LOD parameter
#define TEX_LOD_PARAM( _blv, _mlv, _min, _max, _bias )	\
					{ ( _blv ), ( _mlv ), ( _min ), ( _max ), ( _bias ) }
#define TEX_BASE_LEVEL( _lv )				( _lv )
#define TEX_MAX_LEVEL( _lv )				( _lv )
#define TEX_MIN_LOD( _param )				( _param )
#define TEX_MAX_LOD( _param )				( _param )
#define TEX_LOD_BIAS( _bias )				( _bias )

// Texture mapping descriptor
#define	TEX_DESC( _type, _idx, _emode, _cptr, _ecol, _offset, _scl,				\
				_wraps, _wrapt, _bcol,	_fmode, _lod, _tinfo, _rsv1, _rsv0 )	\
			{ _type, _idx, _emode, _cptr, _ecol, _offset, _scl,					\
				_wraps, _wrapt, _bcol, _fmode, _lod, _tinfo, _rsv1, _rsv0 }

#define TEX_MAPTYPE( _type )				( _type )
#define TEX_INDEX( _idx )					( _idx )
#define TEX_ENVMODE( _mode )				( _mode )
#define TEX_P_COMBINE( _ptr )				( _ptr )
#define TEX_ENV_COLOR( _r, _g, _b, _a )		{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define TEX_OFFSET( _s, _t )				{ ( _s ), ( _t ) }
#define TEX_SCALE( _s, _t )					{ ( _s ), ( _t ) }
#define TEX_WRAP_S( _wmode )				( _wmode )
#define TEX_WRAP_T( _wmode )				( _wmode )
#define TEX_P_BORDER_COLOR( _ptr )			( _ptr )
#define TEX_P_FILTER_MODE( _ptr )			( _ptr )
#define TEX_P_LOD_PARAM( _ptr )				( _ptr )
#define TEX_P_TEX_INFO( _ptr )				( _ptr )
#define TEX_RESERVED1( _rsv )				( _rsv )
#define TEX_RESERVED0( _rsv )				( _rsv )

// Material color for standard shader
#define	MAT_STD_COLOR( _flag, _ambi, _diff, _spec, _emis, _shin, _sinten )\
					 { _flag, _ambi, _diff, _spec, _emis, _shin, _sinten }

#define	MAT_STD_COLFLAG( _f )					( _f )
#define	MAT_STD_AMBIENT( _r, _g, _b, _a )		{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define	MAT_STD_DIFFUSE( _r, _g, _b, _a )		{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define	MAT_STD_SPECULAR( _r, _g, _b, _a )		{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define	MAT_STD_EMISSION( _r, _g, _b, _a )		{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define	MAT_STD_SHININESS( _shin )				( _shin )
#define	MAT_STD_SPECINTEN( _inten )				( _inten )

// Texture mapping descriptor for standard shader
#define	TEX_STD_DESC( _type, _idx, _coord, _blend, _offset, _scl,				\
				_wraps, _wrapt, _bcol,	_fmode, _lod, _tinfo, _rsv1, _rsv0 )	\
					{ _type, _idx, _coord, _blend, _offset, _scl,				\
				_wraps, _wrapt, _bcol, _fmode, _lod, _tinfo, _rsv1, _rsv0 }

#define TEX_STD_MAPTYPE( _type )			( _type )
#define TEX_STD_INDEX( _idx )				( _idx )
#define TEX_STD_TEXCOORD( _coord )			( _coord )
#define TEX_STD_BLEND( _blend )				( _blend )
#define TEX_STD_OFFSET( _s, _t )			{ ( _s ), ( _t ) }
#define TEX_STD_SCALE( _s, _t )				{ ( _s ), ( _t ) }
#define TEX_STD_WRAP_S( _wmode )			( _wmode )
#define TEX_STD_WRAP_T( _wmode )			( _wmode )
#define TEX_STD_P_BORDER_COLOR( _ptr )		( _ptr )
#define TEX_STD_P_FILTER_MODE( _ptr )		( _ptr )
#define TEX_STD_P_LOD_PARAM( _ptr )			( _ptr )
#define TEX_STD_P_TEX_INFO( _ptr )			( _ptr )
#define TEX_STD_RESERVED1( _rsv )			( _rsv )
#define TEX_STD_RESERVED0( _rsv )			( _rsv )

// Material descriptor for standard shader
#define	MAT_STD_DESC( _flag, _user, _pcol, _plog, _ttype, _ntex, _ptex )	\
					{ _flag, _user, _pcol, _plog, _ttype, _ntex, _ptex }
#define	MAT_STD_DESC_USERPROF( _flag, _user, _pcol, _plog, _ttype, _ntex, _ptex, _prof )	\
					{ _flag, _user, _pcol, _plog, _ttype, _ntex, _ptex, _prof }

#define	MAT_STD_FLAG( _flag )				( _flag )
#define MAT_STD_USER( _user )				( _user )
#define	MAT_STD_P_COLOR( _ptr )				( _ptr )
#define	MAT_STD_P_LOGIC( _ptr )				( _ptr )
#define	MAT_STD_TEXTYPE( _ttype )			( _ttype )
#define MAT_STD_N_TEX( _num )				( _num )
#define MAT_STD_P_TEXDESC( _ptr )			( _ptr )
#define MAT_STD_USERPROF( _prof )			( _prof )


// Material color for OpenGL ES 1.1
#define	MAT_ES11_COLOR( _flag, _ambi, _diff, _spec, _emis, _shin, _sinten )\
					  { _flag, _ambi, _diff, _spec, _emis, _shin, _sinten }

#define	MAT_ES11_COLFLAG( _f )					( _f )
#define	MAT_ES11_AMBIENT( _r, _g, _b, _a )		{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define	MAT_ES11_DIFFUSE( _r, _g, _b, _a )		{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define	MAT_ES11_SPECULAR( _r, _g, _b, _a )		{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define	MAT_ES11_EMISSION( _r, _g, _b, _a )		{ ( _r ), ( _g ), ( _b ), ( _a ) }
#define	MAT_ES11_SHININESS( _shin )				( _shin )
#define	MAT_ES11_SPECINTEN( _inten )			( _inten )

// Material logic for OpenGL ES 1.1
#define MAT_ES11_LOGIC( _flag, _src, _dst, _bop, _lop, _afunc, _dfunc, _aref )	\
					  { _flag, _src, _dst, _bop, _lop, _afunc, _dfunc, _aref }

#define MAT_ES11_LOGFLAG( _flag )				( _flag )
#define MAT_ES11_SRC_FACTOR( _sfactor )			( _sfactor )
#define MAT_ES11_DST_FACTOR( _dfactor )			( _dfactor )
#define MAT_ES11_BLENDOP( _op )					( _op )
#define MAT_ES11_LOGICOP( _op )					( _op )
#define MAT_ES11_ALPHAFUNC( _func )				( _func )
#define MAT_ES11_DEPTHFUNC( _func )				( _func )
#define MAT_ES11_ALPHAREF( _ref )				( _ref )


// Texture combine for OpenGL ES 1.1
#define TEX_ES11_COMBINE( _crgb, _s0rgb, _op0rgb, _s1rgb, _op1rgb, _s2rgb, _op2rgb,	\
							_ca, _s0a, _op0a, _s1a, _op1a, _s2a, _op2a, _col )		\
						{ _crgb, _s0rgb, _op0rgb, _s1rgb, _op1rgb, _s2rgb, _op2rgb,	\
							_ca, _s0a, _op0a, _s1a, _op1a, _s2a, _op2a, _col }

#define TEX_ES11_COMBINE_RGB( _val )			((Uint16)( _val ))
#define TEX_ES11_SOURCE0_RGB( _val )			((Uint16)( _val ))
#define TEX_ES11_OPERAND0_RGB( _val )			((Uint16)( _val ))
#define TEX_ES11_SOURCE1_RGB( _val )			((Uint16)( _val ))
#define TEX_ES11_OPERAND1_RGB( _val )			((Uint16)( _val ))
#define TEX_ES11_SOURCE2_RGB( _val )			((Uint16)( _val ))
#define TEX_ES11_OPERAND2_RGB( _val )			((Uint16)( _val ))
#define TEX_ES11_COMBINE_ALPHA( _val )			((Uint16)( _val ))
#define TEX_ES11_SOURCE0_ALPHA( _val )			((Uint16)( _val ))
#define TEX_ES11_OPERAND0_ALPHA( _val )			((Uint16)( _val ))
#define TEX_ES11_SOURCE1_ALPHA( _val )			((Uint16)( _val ))
#define TEX_ES11_OPERAND1_ALPHA( _val )			((Uint16)( _val ))
#define TEX_ES11_SOURCE2_ALPHA( _val )			((Uint16)( _val ))
#define TEX_ES11_OPERAND2_ALPHA( _val )			((Uint16)( _val ))
#define TEX_ES11_ENV_COLOR( _r, _g, _b, _a )	{ ( _r ), ( _g ), ( _b ), ( _a ) }

// Texture mapping descriptor for OpenGL ES 1.1
#define	TEX_ES11_DESC( _type, _idx, _emode, _cptr, _offset, _scl,	\
							_wraps, _wrapt, _fmode, _lod, _tinfo )	\
					 { _type, _idx, _emode, _cptr, _offset, _scl,	\
							_wraps, _wrapt, _fmode, _lod, _tinfo }

#define TEX_ES11_MAPTYPE( _type )			( _type )
#define TEX_ES11_INDEX( _idx )				( _idx )
#define TEX_ES11_ENVMODE( _mode )			( _mode )
#define TEX_ES11_P_COMBINE( _ptr )			( _ptr )
#define TEX_ES11_OFFSET( _s, _t )			{ ( _s ), ( _t ) }
#define TEX_ES11_SCALE( _s, _t )			{ ( _s ), ( _t ) }
#define TEX_ES11_WRAP_S( _wmode )			( _wmode )
#define TEX_ES11_WRAP_T( _wmode )			( _wmode )
#define TEX_ES11_P_FILTER_MODE( _ptr )		( _ptr )
#define TEX_ES11_LOD_BIAS( _bias )			( _bias )
#define TEX_ES11_P_TEX_INFO( _ptr )			( _ptr )

// Material descriptor for OpenGL ES 1.1
#define	MAT_ES11_DESC( _flag, _user, _pcol, _plog, _ntex, _ptex )	\
					 { _flag, _user, _pcol, _plog, _ntex, _ptex }

#define	MAT_ES11_FLAG( _flag )				( _flag )
#define MAT_ES11_USER( _user )				( _user )
#define	MAT_ES11_P_COLOR( _ptr )			( _ptr )
#define	MAT_ES11_P_LOGIC( _ptr )			( _ptr )
#define MAT_ES11_N_TEX( _num )				( _num )
#define MAT_ES11_P_TEXDESC( _ptr )			( _ptr )


#include "nntvertexgl.h"

#endif	//__NNTGL_H__

/* End of file */
