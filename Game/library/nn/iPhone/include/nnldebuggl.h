/*---------------------------------------------------------------------------

    NN Debug for OpenGL

    Copyright (C) 2004-2006 SEGA Corporation Creative Center Graphics Sect.
    All Rights Reserved.

    Module  : NN Debug Library
    File    : nnldebuggl.h
    Create  : 2004/07/05
    Modify  : 2004/12/16 <crtdbg.h>をWIN32のみに変更
    Modify  : 2006/04/11 printf の前に (void) をつけた。
	Modify  : 2006/05/17 NNM_COMPILE_TIME_ASSERTを追加
	Modify  : 2009/08/17 #include <assert.h>を追加
    Version : 1.04.00
    Note    :

---------------------------------------------------------------------------*/
#ifndef __NNLDEBUGGL_H__
#define __NNLDEBUGGL_H__

#include <stdio.h>
#include <assert.h>
#ifdef _WIN32
#include <crtdbg.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Debug */
#ifdef NN_DEBUG
#if ( NND_PLATFORM == NND_PLATFORM_GL )
#include <GL/glu.h>
#endif	// ( NND_PLATFORM == NND_PLATFORM_GL )

// break命令で処理を停止させるマクロ
#ifdef _WIN32
#define NNM_BREAK()		_CrtDbgBreak();
#elif  _IPHONE
#define NNM_BREAK()		while(true);
#else
#define NNM_BREAK()		assert(0);
#endif
// トレース用の文字列表示
#define NNM_TRACE		__nndebug_printf
// エラー出力用の文字列表示
// ここではトレースと同じ__nndebug_printf関数
#define NNM_ERROUT		__nndebug_printf
// ASSERT処理:ファイル名とライン番号を表示して停止
#define NNM_ASSERT(f,s) 								\
	do {												\
		if (!(f)) {										\
			if((s)){									\
				NNM_TRACE(s);							\
			}											\
			NNM_TRACE("Assertion failed.\n");			\
			NNM_BREAK();								\
		}												\
	} while ( 0 )

#if ( NND_PLATFORM == NND_PLATFORM_GL )
#define NNM_CHECK_GL_ERROR()							\
	do {												\
		GLenum err = glGetError();						\
		if (err != GL_NO_ERROR) {						\
			NNM_TRACE("%s\n", gluErrorString(err));		\
			NNM_BREAK();								\
		}												\
	} while ( 0 )
#else
#define NNM_CHECK_GL_ERROR()							\
	do {												\
		GLenum err = glGetError();						\
		if (err != GL_NO_ERROR) {						\
			NNM_TRACE("GL ERROR : 0x%04x\n", err);		\
			NNM_BREAK();								\
		}												\
	} while ( 0 )
#endif

#else

#define NNM_BREAK()		((void)0)
#define NNM_TRACE		1 ? (void)0 : __nndebug_printf
#define NNM_ERROUT		1 ? (void)0 : __nndebug_printf
#define NNM_ASSERT(f,s)	((void)0)
#define NNM_CHECK_GL_ERROR()	((void)0)

#endif /* NN_DEBUG */


#define NNM_COMPILE_TIME_ASSERT(f)		typedef char NNS_COMPILE_TIME_ASSERT[ (f) ? 1 : -1 ]


//void __nndebug_printf(char *fmt, ...);
#define __nndebug_printf	(void)printf

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__NNLDEBUGGL_H__

/* End of file */
