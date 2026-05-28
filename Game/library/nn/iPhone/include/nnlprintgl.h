/*---------------------------------------------------------------------------

    NN Print for DX

    Copyright (C) 2002-2004 SEGA Corporation Creative Center Graphics Sect.
    All Rights Reserved.

    Module  : NN Print Library
    File    : nnlprintgl.h
    Create  : 2004/05/06
    Modify  : 
    Version : 0.00.00
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLPRINTGL_H__
#define __NNLPRINTGL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#if ( NND_PLATFORM == NND_PLATFORM_GL )
typedef struct {
	struct {
		GLfloat t[2];
		GLubyte c[4];
		GLfloat v[3];
	} vtx[4];
} NNS_PRINT_BUF;
#endif	// ( NND_PLATFORM == NND_PLATFORM_GL )

#if ( NND_PLATFORM == NND_PLATFORM_GLES11 )
typedef struct {
	struct {
		GLfloat t[2];
		GLubyte c[4];
		GLfloat v[3];
	} vtx[6];
} NNS_PRINT_BUF;
#endif	// ( NND_PLATFORM == NND_PLATFORM_GLES11 )

#if defined(_IPHONE)
/* âÊñ ÉÇÅ[Éh */
typedef enum {
	NNE_POM_VERTICAL,			// ècâÊñ 
	NNE_POM_HORIZON_LEFT,		// ç∂âÒì]â°âÊñ 
	NNE_POM_HORIZON_RIGHT,		// âEâÒì]â°âÊñ 
} NNE_PRINT_ORIENTATION_MODE;
void nnSetPrintOrientationMode( NNE_PRINT_ORIENTATION_MODE mode  );
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__NNLPRINTGL_H__

/* End of file */
