/*---------------------------------------------------------------------------

    NN format structs header

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN format header
    File    : nnf.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Modify  : 2004/09/13 nnftexture.hを先にインクルードするように変更
    Version : 1.17.00
    Note    : 

---------------------------------------------------------------------------*/

#ifndef	__NNF_H__
#define	__NNF_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Rotation type for camera and light */
typedef enum {
	NNE_ROTATETYPE_XYZ,
	NNE_ROTATETYPE_XZY,
	NNE_ROTATETYPE_YXZ,	/* No support */
	NNE_ROTATETYPE_YZX,	/* No support */
	NNE_ROTATETYPE_ZXY,
	NNE_ROTATETYPE_ZYX	/* No support */
} NNE_ROTATETYPE;

#ifdef __cplusplus
}
#endif /* __cplusplus */


/* NN Texture format */
#include <nnftexture.h>

/* NN Material format */
#include <nnfmaterial.h>

/* NN Object format */
#include <nnfobject.h>

/* NN Motion format */
#include <nnfmotion.h>

/* NN Camera format */
#include <nnfcamera.h>

/* NN Light format */
#include <nnflight.h>

/* NN Scene format */
#include <nnfscene.h>

#endif //__NNF_H__

/* End of file */
