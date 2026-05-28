/*---------------------------------------------------------------------------

    NN texture api header

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN texture api header
    File    : nnltexture.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Modify  : 2003/07/30 NND_TEXFLAG_PS2_LOADHIGHを追加
                         NND_TEXFLAG_PS2_FIXLODPARAMの定義を変更
    Modify  : 2003/10/08 Xbox,PC共通化向けの修正
    Modify  : 2003/12/24 NND_TEXFLAG_PS2_FIXCSAを追加
    Modify  : 2004/03/29 nnltexturexb.h追加
    Modify  : 2004/04/27 NND_TEXFLAG_PS2_LINEARを追加
    Modify  : 2004/06/16 DX8,DX9版統合
    Modify  : 2004/06/30 onWin版統合
    Modify  : 2004/07/30 nnSetTexturePaletteBankOne/Num()追加
    Modify  : 2004/08/05 NND_PLATFORM_XENON追加
    Modify  : 2004/08/12 OpenGL版統合
    Modify  : 2004/08/24 XboxのNNS_TEXINFO構造体にMaxAnisotropyを追加
    Modify  : 2005/03/10 DT06版統合
    Modify  : 2005/03/14 PSP版統合
    Modify  : 2005/04/07 PSP版テクスチャタイプ追加
    Modify  : 2005/04/19 NNS_TEXLIST を nnftexture.hから移動
                         NNS_TEXLIST のメンバ pTexInfoList の型をNNS_TEXINFO*に変更
    Modify  : 2005/04/26 PSP版 NNS_TEXINFO構造体にgimピクチャ用メンバ追加
    Modify  : 2005/05/23 DT06版 NNS_TEXINFO構造体にWidth,Heightおよびヘッダを追加
    Modify  : 2005/06/16 PS2版 NND_TEXFLAG_PS2_ONCACHE追加(FIXLODPARAM,FIXCSA,LINEAR定義変更)
    Modify  : 2006/03/15 PLAYSTAION3版統合
    Modify  : 2008/05/21 NND_PLATFORM_DXG20追加
    Modify  : 2009/05/13 NND_PLATFORM_XENON削除
    Version : 1.18.30
    Note    :

---------------------------------------------------------------------------*/

#ifndef __NNLTEXTURE_H__
#define __NNLTEXTURE_H__

#include <nv.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* TexFlag */
typedef Uint32 NNF_TEXFLAG;
#define NND_TEXFLAG_ALLOCATE		(1 << 0)
#define NND_TEXFLAG_PS2_PREPARE		(1 << 8)
#define NND_TEXFLAG_PS2_LOAD		(1 << 9)
#define NND_TEXFLAG_PS2_LOADHIGH	(1 <<10)
#define NND_TEXFLAG_PS2_ONCACHE		(1 <<11)
#define NND_TEXFLAG_PS2_FIXLODPARAM	(1 <<12)
#define NND_TEXFLAG_PS2_FIXCSA		(1 <<13)
#define NND_TEXFLAG_PS2_LINEAR		(1 <<14)


/* PlayStation2 */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
struct _NNS_TEXINFO {
	NVS_SVROBJ	SvrObj;
	void		*pMainMemory;		/* Texture hostmemory address */
	void		*pLocalMemory;		/* Texture localmemory address */
	Uint32		nLocalBytes;		/* Texture localmemory size */
	Uint32		nDmaTagBytes;		/* DMA tag size */
	Uint32		GlobalIndex;		/* GlobalIndex number */
	Uint32		Bank;				/* Bank number */
	Uint16		MinFilter;			/* Texture minfilter mode */
	Uint16		MagFilter;			/* Texture magfilter mode */
	NNF_TEXFLAG	Flag;				/* Flag */
};

typedef enum {
	NNE_TEXSLOT_0 = 0,
	NNE_TEXSLOT_1,
	NNE_TEXSLOT_MAX
} NNE_TEXSLOT;

/* API */
void nnInitTexturePS2( Uint32 localptr, Uint32 size );
void nnExitTexturePS2( void );
void nnInitTexturePalettePS2(void *pal16, Uint32 num16, void *pal256, Uint32 num256, void *mainbuf);

#include <nnltextureps2.h>
#endif


/* GAMECUBE */
#if ( NND_PLATFORM == NND_PLATFORM_GC )
struct _NNS_TEXINFO {
	NVS_GVROBJ	GvrObj;			/* GVR Object */
	void		*pMainMemory;	/* Texture address */
#ifdef WIN32
	D3DTEXTUREFILTERTYPE MinFilter;/* MinFilter */
	D3DTEXTUREFILTERTYPE MagFilter;/* MagFilter */
	D3DTEXTUREFILTERTYPE MipFilter;/* MipFilter */
#endif /* WIN32 */
	Uint32		GlobalIndex; 	/* GlobalIndex number */
	Uint32		Bank;			/* bank number */
	NNF_TEXFLAG	Flag;			/* Load Flag */
};

typedef enum {
	NNE_TEXSLOT_0 = 0,
	NNE_TEXSLOT_1,
	NNE_TEXSLOT_2,
	NNE_TEXSLOT_3,
	NNE_TEXSLOT_4,
	NNE_TEXSLOT_5,
	NNE_TEXSLOT_6,
	NNE_TEXSLOT_7,
	NNE_TEXSLOT_MAX
} NNE_TEXSLOT;

#ifndef WIN32
void nnInitTextureGC( OSHeapHandle heap, Uint32 size );
void *nnTexAllocGC(Uint32 size);
void nnTexFreeGC(void *tex);
#endif /* WIN32 */
void nnInitTexturePaletteGC(Uint16 *pal16, Sint32 num16, Uint16 *pal256, Sint32 num256);

#endif


/* Xbox */
#if ( NND_PLATFORM == NND_PLATFORM_XB || NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
struct _NNS_TEXINFO {
	NVS_XVROBJ	xvrobj;
	Uint32		GlobalIndex;		/* GlobalIndex number */
	Uint32		Bank;				/* Bank number */
	NNF_TEXFLAG	Flag;				/* Load Flag */
	NNF_TEXFILE_MINFILTER	nnMinFilter;
	NNF_TEXFILE_MAGFILTER	nnMagFilter;
	D3DTEXTUREFILTERTYPE	MinFilter;
	D3DTEXTUREFILTERTYPE	MagFilter;
	D3DTEXTUREFILTERTYPE	MipFilter;
	Uint32		MaxAnisotropy;
};

typedef enum {
	NNE_TEXSLOT_0 = 0,
	NNE_TEXSLOT_1,
	NNE_TEXSLOT_2,
	NNE_TEXSLOT_3,
	NNE_TEXSLOT_MAX
} NNE_TEXSLOT;

void nnInitTextureXB( void *ptr, Uint32 size );

#include <nnltexturexb.h>
#endif

/* OpenGL */
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
struct _NNS_TEXINFO {
	GLuint		TexName;
	Uint32		GlobalIndex;		/* GlobalIndex number */
	Uint32		Bank;				/* Bank number */
	NNF_TEXFLAG	Flag;				/* Load Flag */
};

typedef enum {
	NNE_TEXSLOT_0 = 0,
	NNE_TEXSLOT_1,
	NNE_TEXSLOT_2,
	NNE_TEXSLOT_3,
	NNE_TEXSLOT_4,
	NNE_TEXSLOT_5,
	NNE_TEXSLOT_6,
	NNE_TEXSLOT_7,
	NNE_TEXSLOT_MAX,
} NNE_TEXSLOT;

#endif

/* PSP */
#if ( NND_PLATFORM == NND_PLATFORM_PSP )


struct _NNS_TEXINFO {
	NVS_UVROBJ	UvrObj;
	void		*pMainMemory;		/* Texture hostmemory address */
	void		*pTexBuffer;		/* texture buffer */
	Uint32		nLocalBytes;		/* Texture localmemory size */
	Uint32		GlobalIndex;		/* GlobalIndex number */
	Uint32		Bank;				/* Bank number */
	NNF_TEXFLAG	Flag;				/* Flag */
	Uint32		MinFilter;
	Uint32		MagFilter;
};

typedef enum {
	NNE_TEXSLOT_0 = 0,
	NNE_TEXSLOT_1,
	NNE_TEXSLOT_MAX
} NNE_TEXSLOT;

void nnInitTexturePalettePSP(void *pal16, Uint32 num16, void *pal256, Uint32 num256, void *mainbuf);

#include <nnltexturepsp.h>
#endif

/* PS3 */
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
struct _NNS_TEXINFO {
	union{
		void		*pSbglTexture;
		void		*pTexture;
	};
	Uint32		GlobalIndex;		/* GlobalIndex number */
	Uint32		Bank;				/* Bank number */
	NNF_TEXFLAG	Flag;				/* Load Flag */
};

typedef enum {
	NNE_TEXSLOT_0 = 0,
	NNE_TEXSLOT_1,
	NNE_TEXSLOT_2,
	NNE_TEXSLOT_3,
	NNE_TEXSLOT_4,
	NNE_TEXSLOT_5,
	NNE_TEXSLOT_6,
	NNE_TEXSLOT_7,
	NNE_TEXSLOT_MAX,
} NNE_TEXSLOT;

#endif

/* DXG20 */
#if ( NND_PLATFORM == NND_PLATFORM_DXG20 )
struct _NNS_TEXINFO {
	NVS_XVROBJ	xvrobj;
	Uint32		GlobalIndex;		/* GlobalIndex number */
	Uint32		Bank;				/* Bank number */
	NNF_TEXFLAG	Flag;				/* Load Flag */
	NNF_TEXFILE_MINFILTER	nnMinFilter;
	NNF_TEXFILE_MAGFILTER	nnMagFilter;
	D3DTEXTUREFILTERTYPE	MinFilter;
	D3DTEXTUREFILTERTYPE	MagFilter;
	D3DTEXTUREFILTERTYPE	MipFilter;
	Uint32		MaxAnisotropy;
};

typedef enum {
	NNE_TEXSLOT_0 = 0,
	NNE_TEXSLOT_1,
	NNE_TEXSLOT_2,
	NNE_TEXSLOT_3,
	NNE_TEXSLOT_4,
	NNE_TEXSLOT_5,
	NNE_TEXSLOT_6,
	NNE_TEXSLOT_7,
	NNE_TEXSLOT_8,
	NNE_TEXSLOT_9,
	NNE_TEXSLOT_10,
	NNE_TEXSLOT_11,
	NNE_TEXSLOT_12,
	NNE_TEXSLOT_13,
	NNE_TEXSLOT_14,
	NNE_TEXSLOT_15,
	NNE_TEXSLOT_MAX
} NNE_TEXSLOT;

#if 0 /* 必要ない(2008.10.31)*/
void nnInitTextureDXG20( void *ptr, Uint32 size );
#endif

#include <nnltexturedxg20.h>
#endif

/* TexInfo */
#ifndef __TYPEDEF_NNS_TEXINFO__
#define __TYPEDEF_NNS_TEXINFO__
typedef struct _NNS_TEXINFO NNS_TEXINFO;
#endif


/* TexList */
struct _NNS_TEXLIST{
	Sint32		nTex;			/* Number of textures */
	NNS_TEXINFO	*pTexInfoList;	/* Texture information list*/
};

#ifndef __TYPEDEF_NNS_TEXLIST__
#define __TYPEDEF_NNS_TEXLIST__
typedef struct _NNS_TEXLIST NNS_TEXLIST;
#endif


extern const NNS_TEXLIST *nngCurrentTextureList;

Uint32 nnEstimateTexlistSize( Sint32 num );
void nnSetUpTexlist( NNS_TEXLIST **texlist, Sint32 num, void *buf );
Sint32 nnSetTexture( NNE_TEXSLOT slot, const NNS_TEXLIST *pTexList, Sint32 num );
Sint32 nnSetTextureList( const NNS_TEXLIST *pTexList );
Sint32 nnGetTextureList(const NNS_TEXLIST **pTexList);
Sint32 nnSetTextureNum( NNE_TEXSLOT slot, Sint32 num );
Sint32 nnLoadTextureMemoryOne( NNS_TEXINFO *pTexInfo, const void *tex,
		NNF_TEXFILE_MINFILTER minfilter, NNF_TEXFILE_MAGFILTER magfilter,
		Uint32 globalIndex, Uint32 bank, NNF_TEXFLAG flag );
Sint32 nnReleaseTextureOne( NNS_TEXINFO *pTexInfo );
Sint32 nnSetTexturePaletteBankOne( NNS_TEXINFO *pTexInfo, Uint32 bank );
Sint32 nnSetTexturePaletteBankNum( NNS_TEXLIST *pTexList, Sint32 num, Uint32 bank );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNLTEXTURE_H__ */

/* End of file */
