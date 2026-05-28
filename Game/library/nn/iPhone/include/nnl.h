/*---------------------------------------------------------------------------

    NN API header

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support Dept.
    All Rights Reserved.

    Module  : NN API header
    File    : nnl.h
    Create  : 2002/05/10
    Modify  : 2003/03/28
    Modify  : 2004/04/15 nnldebug.h → nnlprint.h
                         新規nnldebug.h追加
    Modify  : 2004/07/06 nnltexture.hをnnlmaterial.hの前にincludeするよう変更
    Modify  : 2008/05/23 nnltexture.hをnnldrawobj.hの前にincludeするよう変更
    Version : 1.15.21
    Note    : 

---------------------------------------------------------------------------*/

#ifndef	__NNL_H__
#define	__NNL_H__

/* NN System  */
#include <nnlsystem.h>

/* NN Debug  */
#include <nnldebug.h>

/* NN Math  */
#include <nnlmath.h>

/* NN Collision  */
#include <nnlcollision.h>

/* NN Matrix */
#include <nnlmatrix.h>

/* NN Node */
#include <nnlnode.h>

/* NN Texture */
#include <nnltexture.h>

/* NN Draw object */
#include <nnldrawobj.h>

/* NN Morph Target */
#include <nnlmorph.h>

/* NN Draw primitive */
#include <nnldrawprim.h>

/* NV */
//#include <nv.h>

/* NN Material */
#include <nnlmaterial.h>

/* NN Fog */
#include <nnlfog.h>

/* NN Light */
#include <nnllight.h>

/* NN Camera */
#include <nnlcamera.h>

/* NN Scene */
#include <nnlscene.h>

/* NN Print  */
#include <nnlprint.h>

#endif //__NNL_H__

/* End of file */
