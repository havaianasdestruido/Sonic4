/*---------------------------------------------------------------------------

    NN Light Library

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN Light Library
    File    : nnllight.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Modify  : 2003/08/21 nnPutLightSettings()‚ð’Ç‰Á
    Modify  : 2003/10/08 Xbox,PC‹¤’Ê‰»Œü‚¯‚ÌC³
    Modify  : 2004/06/16 DX8,DX9”Å“‡
    Modify  : 2004/08/05 NND_PLATFORM_XENON’Ç‰Á
    Modify  : 2004/08/12 OpenGL”Å“‡
    Modify  : 2004/08/23 nnCalcLightMotion()’Ç‰Á
    Modify  : 2004/08/26 nnSetUpTargetDirectionalLight(), nnSetUpRotationDirectionalLight()’Ç‰Á
                         nnSetLightAngle(), nnSetLightRange(), nnSetLightFallOff()’Ç‰Á
    Modify  : 2005/03/10 DT06”Å“‡
    Modify  : 2005/03/14 PSP”Å“‡
    Modify  : 2006/03/15 PLAYSTATION3”Å“‡
    Modify  : 2008/05/21 NND_PLATFORM_DXG20’Ç‰Á
    Modify  : 2009/05/13 NND_PLATFORM_XENONíœ
    Version : 1.17.12
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLLIGHT_H__
#define __NNLLIGHT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* PlayStation2 */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
typedef enum {
	NNE_LIGHT_0 = 0,
	NNE_LIGHT_1,
	NNE_LIGHT_2,
	NNE_LIGHT_3,
	NNE_LIGHT_4,
	NNE_LIGHT_5,
	NNE_LIGHT_6,
	NNE_LIGHT_7,
	NNE_LIGHT_MAX,
	NNE_LIGHT_ALL
} NNE_LIGHT;

#endif

/* GAMECUBE */
#if ( NND_PLATFORM == NND_PLATFORM_GC )
typedef enum {
	NNE_LIGHT_0 = 0,
	NNE_LIGHT_1,
	NNE_LIGHT_2,
	NNE_LIGHT_3,
	NNE_LIGHT_4,
	NNE_LIGHT_5,
	NNE_LIGHT_6,
	NNE_LIGHT_7,
	NNE_LIGHT_MAX,
	NNE_LIGHT_ALL
} NNE_LIGHT;
#endif

/* Xbox */
#if ( NND_PLATFORM == NND_PLATFORM_XB || NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9 || NND_PLATFORM == NND_PLATFORM_DXG20 )
typedef enum {
	NNE_LIGHT_0 = 0,
	NNE_LIGHT_1,
	NNE_LIGHT_2,
	NNE_LIGHT_3,
	NNE_LIGHT_4,
	NNE_LIGHT_5,
	NNE_LIGHT_6,
	NNE_LIGHT_7,
	NNE_LIGHT_MAX,
	NNE_LIGHT_ALL
} NNE_LIGHT;
#endif

/* OpenGL */
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_PS3 || NND_PLATFORM == NND_PLATFORM_GLES11 )
typedef enum {
	NNE_LIGHT_0 = 0,
	NNE_LIGHT_1,
	NNE_LIGHT_2,
	NNE_LIGHT_3,
	NNE_LIGHT_4,
	NNE_LIGHT_5,
	NNE_LIGHT_6,
	NNE_LIGHT_7,
	NNE_LIGHT_MAX,
	NNE_LIGHT_ALL
} NNE_LIGHT;
#endif

/* PSP */
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
typedef enum {
	NNE_LIGHT_0 = 0,
	NNE_LIGHT_1,
	NNE_LIGHT_2,
	NNE_LIGHT_3,
	NNE_LIGHT_MAX,
	NNE_LIGHT_ALL
} NNE_LIGHT;
#endif

void nnInitLight( void );
void nnSetLight( NNE_LIGHT no, const void *light, NNF_LIGHTTYPE type );
void nnSetUpParallelLight( NNS_LIGHT_PARALLEL *light,
			const NNS_RGBA *color, Float inten, const NNS_VECTOR *dir );
void nnSetUpPointLight( NNS_LIGHT_POINT *light, const NNS_RGBA *color,
					Float inten, const NNS_VECTOR *pos, Float falloffstart, Float falloffend );
void nnSetUpTargetSpotLight( NNS_LIGHT_TARGET_SPOT *light, const NNS_RGBA *color, Float inten, const NNS_VECTOR *pos,
					const NNS_VECTOR *target, Angle32 innerangle, Angle32 outerangle, Float falloffstart, Float falloffend );
void nnSetUpRotationSpotLight( NNS_LIGHT_ROTATION_SPOT *light, const NNS_RGBA *color, Float inten, const NNS_VECTOR *pos,
					NNE_ROTATETYPE rottype, NNS_ROTATE_A32 *rotation, Angle32 innerangle, Angle32 outerangle, Float falloffstart, Float falloffend );
void nnSetUpTargetDirectionalLight( NNS_LIGHT_TARGET_DIRECTIONAL *light, const NNS_RGBA *color, Float inten, const NNS_VECTOR *pos,
					const NNS_VECTOR *target, Float innerrange, Float outerrange, Float falloffstart, Float falloffend );
void nnSetUpRotationDirectionalLight( NNS_LIGHT_ROTATION_DIRECTIONAL *light, const NNS_RGBA *color, Float inten, const NNS_VECTOR *pos,
					NNE_ROTATETYPE rottype, NNS_ROTATE_A32 *rotation, Float innerrange, Float outerrange, Float falloffstart, Float falloffend );
void nnSetLightSwitch( NNE_LIGHT no, NNE_BOOL on_off );
void nnSetLightType( NNE_LIGHT no, NNF_LIGHTTYPE type );
void nnSetLightColor( NNE_LIGHT no, Float r, Float g, Float b );
void nnSetLightAlpha( NNE_LIGHT no, Float a);
void nnSetLightDirection( NNE_LIGHT no, Float x, Float y, Float z );
void nnSetLightPosition( NNE_LIGHT no, Float x, Float y, Float z );
void nnSetLightTarget( NNE_LIGHT no, Float x, Float y, Float z);
void nnSetLightRotation( NNE_LIGHT no, NNE_ROTATETYPE rottype, Angle32 rotx, Angle32 roty, Angle32 rotz);
void nnSetLightAngle( NNE_LIGHT no, Angle32 innerangle, Angle32 outerangle );
void nnSetLightRange( NNE_LIGHT no, Float innerrange, Float outerrange );
void nnSetLightFallOff( NNE_LIGHT no, Float falloffstart, Float falloffend );
void nnSetAmbientColor( Float r, Float g, Float b );
void nnSetLightIntensity( NNE_LIGHT no, Float intensity );
void nnSetLightMatrix( const NNS_MATRIX *mtx );
#define nnSetLightPointer( no, litptr )	\
		nnSetLight( (no), (litptr)->pLight, (litptr)->fType )
Uint32 nnEstimateLightBufferSize( NNF_LIGHTTYPE type);
void nnPutLightSettings(void);

void nnCalcLightMotion( NNS_LIGHTPTR *dstptr, const NNS_LIGHTPTR *litptr, const NNS_MOTION *mot, Float frame );
#define	NND_LIGHT_MOTION_BUFFER_SIZE ( sizeof( NNS_LIGHT_ROTATION_DIRECTIONAL ) + NND_LIGHT_MAX_BUFFER_SIZE + sizeof( NNS_LIGHTPTR ) )

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* PlayStation2 light library */
#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include "nnllightps2.h"
#endif

/* GAMECUBE light library */
#if ( NND_PLATFORM == NND_PLATFORM_GC )
#include "nnllightgc.h"
#endif

/* Xbox light library */
#if ( NND_PLATFORM == NND_PLATFORM_XB )
#include "nnllightdx.h"
#endif

/* PC light library */
#if ( NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9)
#include "nnllightdx.h"
#endif

/* OpenGL light library */
#if ( NND_PLATFORM == NND_PLATFORM_GL || NND_PLATFORM == NND_PLATFORM_GLES11 )
#include "nnllightgl.h"
#endif

/* PSP light library */
#if ( NND_PLATFORM == NND_PLATFORM_PSP )
#include "nnllightpsp.h"
#endif

/* PS3 light library */
#if ( NND_PLATFORM == NND_PLATFORM_PS3 )
#include "nnllightps3.h"
#endif

/* DXG20 light library */
#if ( NND_PLATFORM == NND_PLATFORM_DXG20 )
#include "nnllightdxg20.h"
#endif

#endif /* __NNLLIGHT_H__ */

/* End of file */
