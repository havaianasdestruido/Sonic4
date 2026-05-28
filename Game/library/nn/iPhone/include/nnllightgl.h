/*---------------------------------------------------------------------------

    NN Light library for OpenGL

    Copyright (C) 2004 SEGA Corporation Creative Center Graphics Sect.
    All Rights Reserved.

    Module  : NN Light Library for OpenGL
    File    : nnlightgl.h
    Create  : 2004/04/23
    Modify  : 
    Version : 0.00.00
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLIGHTGL_H__
#define __NNLIGHTGL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define	NND_LIGHT_MAX_BUFFER_SIZE	(sizeof( NNS_LIGHT_STANDARD_GL ))

typedef struct {
	NNE_BOOL		bEnable;
	NNF_LIGHTTYPE	fType;

	Float			Intensity;

	NNS_RGBA		Ambient;
	NNS_RGBA		Diffuse;
	NNS_RGBA		Specular;

	NNS_VECTOR		Direction;
	NNS_VECTOR4D	Position;

	NNS_VECTOR		Target;

	NNE_ROTATETYPE	RotType;
	NNS_ROTATE_A32	Rotation;

	Angle32			InnerAngle;
	Angle32			OuterAngle;

	Float			InnerRange;
	Float			OuterRange;

	Float			FallOffStart;
	Float			FallOffEnd;

	Float			SpotExponent;
	Float			SpotCutoff;
	Float			ConstantAttenuation;
	Float			LinearAttenuation;
	Float			QuadraticAttenuation;

} NNS_GL_LIGHT_DATA;

typedef struct{
	NNS_RGBA			AmbientColor;
	NNS_GL_LIGHT_DATA	LightData[NNE_LIGHT_MAX];
} NNS_GL_LIGHT;

extern NNS_GL_LIGHT	nngLight;


void nnSetLightAmbientGL( NNE_LIGHT no, Float r, Float g, Float b );
void nnSetLightDiffuseGL( NNE_LIGHT no, Float r, Float g, Float b );
void nnSetLightSpecularGL( NNE_LIGHT no, Float r, Float g, Float b );
void nnSetLightSpotEffectGL( NNE_LIGHT no, Float exp, Float cutoff );
void nnSetLightAttenuationGL( NNE_LIGHT no, Float cnst, Float lin, Float quad );

void nnSetUpStandardLightGL( NNS_LIGHT_STANDARD_GL *light,
							const NNS_RGBA *ambient, const NNS_RGBA *diffuse, const NNS_RGBA *specular,
							const NNS_VECTOR4D *position, const NNS_VECTOR *direction,
							Float expornent, Float cutoff, Float cnstattn, Float linattn, Float quadattn);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNLIGHTGL_H__ */

/* End of file */
