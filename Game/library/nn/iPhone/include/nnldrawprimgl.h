/*---------------------------------------------------------------------------

    NN Draw primitive

    Copyright (C) 2002 - 2005 SEGA Corporation Creative Center Graphics Sect.
    All Rights Reserved.
    Module  : NN Draw primitive Library
    File    : nnldrawprimgl.h
    Create  : 2004/09/06
    Modify  : 2005/02/15 アルファテスト、デプステスト、デプスマスク設定関数追加
    Version : 0.80.02
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLDRAWPRIMGL_H__
#define __NNLDRAWPRIMGL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void nnSetPrimitive3DMaterialGL(const NNS_RGBA *diffuse, const NNS_RGBA *ambient, const NNS_RGBA *specular,
								Float shininess, const NNS_RGBA *emission);

void nnSetPrimitive3DAlphaFuncGL( GLenum func, GLclampf ref );
void nnSetPrimitive3DDepthFuncGL( GLenum func );
void nnSetPrimitive3DDepthMaskGL( GLboolean flag );

void nnSetPrimitive2DAlphaFuncGL( GLenum func, GLclampf ref );
void nnSetPrimitive2DDepthFuncGL( GLenum func );
void nnSetPrimitive2DDepthMaskGL( GLboolean flag );


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNLDRAWPRIMGL_H__ */

/* End of file */
