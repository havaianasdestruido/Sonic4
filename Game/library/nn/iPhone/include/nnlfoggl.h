/*---------------------------------------------------------------------------

    NN Fog library for OpenGL

    Copyright (C) 2004 SEGA Corporation Creative Center Graphics Sect.
    All Rights Reserved.

    Module  : NN Fog Library for DX
    File    : nnlfoggl.h
    Create  : 2004/06/28
    Modify  : 2004/11/25 nnSetFogDensity’Ç‰Á
    Version : 0.00.00
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLFOGGL_H__
#define __NNLFOGGL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void nnSetFogLinearGL( Float fnear, Float ffar );
void nnSetFogExpGL( Float density );
void nnSetFogExp2GL( Float density );
void nnSetFogDensityGL( Float density );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNLFOGGL_H__ */

/* End of file */
