/*---------------------------------------------------------------------------

    NN light header for OpenGL

    Copyright (C) 2004 SEGA Corporation Creative Center Graphics Sect.
    All Rights Reserved.

    Module  : NN light for OpenGL
    File    : nnflightgl.h
    Create  : 2004/04/23
    Modify  : 
    Version : 0.00.00
    Note    : 

---------------------------------------------------------------------------*/

#ifndef	__NNFLIGHTGL_H__
#define	__NNFLIGHTGL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define NND_LIGHTTYPE_STANDARD_GL	((Uint32)1 << 16)

#define NND_LIGHTTYPE_MASK	( NND_LIGHTTYPE_COMMON_MASK | NND_LIGHTTYPE_STANDARD_GL )

/* OpenGL standard light */
typedef struct {
	Uint32			User;
	NNS_RGBA		Ambient;
	NNS_RGBA		Diffuse;
	NNS_RGBA		Specular;
	NNS_VECTOR4D	Position;
	NNS_VECTOR		SpotDirection;
	Float			SpotExponent;
	Float			SpotCutoff;
	Float			ConstantAttenuation;
	Float			LinearAttenuation;
	Float			QuadraticAttenuation;
} NNS_LIGHT_STANDARD_GL;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	//__NNFLIGHTGL_H__

/* End of file */
