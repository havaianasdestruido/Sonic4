/*---------------------------------------------------------------------------

    NN System library for OpenGL

    Copyright (C) 2004 SEGA Corporation Creative Center Graphics Sect.
    All Rights Reserved.

    Module  : NN System Library for DX
    File    : nnlsystempc.h
    Create  : 2004/05/06
    Modify  : 
    Version : 0.00.00
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLSYSTEMGL_H__
#define __NNLSYSTEMGL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
	Sint32	WindowWidth;
	Sint32	WindowHeight;
} NNS_CONFIG_GL;

void nnConfigureSystemGL( NNS_CONFIG_GL *config );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNLSYSTEMGL_H__ */

/* End of file */
