/*---------------------------------------------------------------------------

    NN  High level texture library

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN Texture Library
    File    : nnftexture.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Modify  : 2004/04/13 Xbox—p‚ÉANISOTROPIC‚ð’Ç‰Á
    Modify  : 2004/08/06 ANISOTROPIC‚ð2, 4, 8‚É•ª—£
    Modify  : 2005/04/19 NNS_TEXLIST ’è‹`‚ÌŽÀ‘Ì‚ð nnltexture.h‚ÉˆÚ“®
    Version : 1.18.25
    Note    : 

---------------------------------------------------------------------------*/
#ifndef __NNFTEXTURE_H__
#define __NNFTEXTURE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Filter mode */
typedef Uint16 NNF_TEXFILE_MAGFILTER;
#define NND_MAG_NEAREST						(0)
#define NND_MAG_LINEAR						(1)
#define NND_MAG_ANISOTROPIC					(2)		/* for Xbox */

typedef Uint16 NNF_TEXFILE_MINFILTER;
#define NND_MIN_NEAREST						(0)
#define NND_MIN_LINEAR						(1)
#define NND_MIN_NEAREST_MIPMAP_NEAREST		(2)
#define NND_MIN_NEAREST_MIPMAP_LINEAR		(3)
#define NND_MIN_LINEAR_MIPMAP_NEAREST		(4)
#define NND_MIN_LINEAR_MIPMAP_LINEAR		(5)
#define NND_MIN_ANISOTROPIC					(6)		/* for Xbox default */
#define NND_MIN_ANISOTROPIC_MIPMAP_NEAREST	(7)		/* for Xbox default */
#define NND_MIN_ANISOTROPIC_MIPMAP_LINEAR	(8)		/* for Xbox default */
#define NND_MIN_ANISOTROPIC2				(6)
#define NND_MIN_ANISOTROPIC2_MIPMAP_NEAREST	(7)
#define NND_MIN_ANISOTROPIC2_MIPMAP_LINEAR	(8)
#define NND_MIN_ANISOTROPIC4				(9)
#define NND_MIN_ANISOTROPIC4_MIPMAP_NEAREST	(10)
#define NND_MIN_ANISOTROPIC4_MIPMAP_LINEAR	(11)
#define NND_MIN_ANISOTROPIC8				(12)
#define NND_MIN_ANISOTROPIC8_MIPMAP_NEAREST	(13)
#define NND_MIN_ANISOTROPIC8_MIPMAP_LINEAR	(14)


#define NNM_MAG_FILTER(f) (f)
#define NNM_MIN_FILTER(f) (f)

/* tex type 8bit */
#define NND_TEXFTYPE_TEXTYPE_MASK		(0x000000FF)
#define NND_TEXFTYPE_GVRTEX				(0)
#define NND_TEXFTYPE_SVRTEX				(1)
#define NND_TEXFTYPE_XVRTEX				(2)

/* */
#define NND_TEXFTYPE_NO_FILENAME		(0x00000100)	/* Don't use file name */
#define NND_TEXFTYPE_NO_FILTER			(0x00000200)	/* Don't use filter */
#define NND_TEXFTYPE_LISTGLBIDX 		(0x00000400)	/* Use tex file list globalindex */
#define NND_TEXFTYPE_LISTBANK			(0x00000800)	/* Use tex file list bank */

typedef Uint32 NNF_TEXFILETYPE;

typedef struct{
	NNF_TEXFILETYPE			fType;			/* Texture load flag */
	char					*Filename;		/* Texture file name */
	NNF_TEXFILE_MINFILTER	MinFilter;		/* Texture minfilter mode */
	NNF_TEXFILE_MAGFILTER	MagFilter;		/* Texture magfilter mode */
	Uint32					GlobalIndex;	/* Globalindex number */
	Uint32					Bank;			/* Palette bank number */
}NNS_TEXFILE;

typedef struct{
	Sint32			nTex;			/* Number of textures */
	NNS_TEXFILE		*pTexFileList; 	/* Texture file name list */
}NNS_TEXFILELIST;

#ifndef __TYPEDEF_NNS_TEXINFO__
#define __TYPEDEF_NNS_TEXINFO__
typedef struct _NNS_TEXINFO NNS_TEXINFO;
#endif

#ifndef __TYPEDEF_NNS_TEXLIST__
#define __TYPEDEF_NNS_TEXLIST__
typedef struct _NNS_TEXLIST NNS_TEXLIST;
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __NNFTEXTURE_H__ */

/* End of file */
