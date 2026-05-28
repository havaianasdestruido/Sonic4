/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 1998-2008 CRI Middleware Co., Ltd.
 *
 * Library  : CRI Middleware Library
 * Module   : CRI Common Header
 * File     : cri_xpts.h
 * Date     : 2008-02-01
 * Version  : 2.00
 *
 ****************************************************************************/
#ifndef CRI_XPTS_H
#define CRI_XPTS_H

/*****************************************************************************
 * 基本データ型宣言
 *****************************************************************************/

/*****************************************************************************
 * CriUint8 CriSint8 CriUint16 CriSint16 に関しては共通
 *****************************************************************************/

#ifndef _TYPEDEF_CriUint8
#define _TYPEDEF_CriUint8
typedef unsigned char			CriUint8;		/* 符号なし１バイト整数 */
#endif

#ifndef _TYPEDEF_CriSint8
#define _TYPEDEF_CriSint8
typedef signed char				CriSint8;		/* 符号つき１バイト整数 */
#endif

#ifndef _TYPEDEF_CriUint16
#define _TYPEDEF_CriUint16
typedef unsigned short			CriUint16;		/* 符号なし２バイト整数 */
#endif

#ifndef _TYPEDEF_CriSint16
#define _TYPEDEF_CriSint16
typedef signed short			CriSint16;		/* 符号つき２バイト整数 */
#endif

/*****************************************************************************
 * CriUint32 CriSint32 CriUint64 CriSint64 CriUint128 CriSint128 (その1 固有の定義が必要な場合)
 *****************************************************************************/

#ifdef XPT_TGT_PSP
	
#ifndef _TYPEDEF_CriUint64
#define _TYPEDEF_CriUint64
typedef long long				CriUint64;		/* 符号なし８バイト整数 */
#endif

#ifndef _TYPEDEF_CriSint64
#define _TYPEDEF_CriSint64
typedef signed long long		CriSint64;		/* 符号つき８バイト整数 */
#endif

#endif	/* endif XPT_TGT_PSP */

#ifdef XPT_TGT_EE

#ifndef _TYPEDEF_CriUint32
#define _TYPEDEF_CriUint32
typedef unsigned int			CriUint32;		/* 符号なし４バイト整数 */
#endif

#ifndef _TYPEDEF_CriSint32
#define _TYPEDEF_CriSint32
typedef signed int				CriSint32;		/* 符号つき４バイト整数 */
#endif

#ifndef _TYPEDEF_CriUint64
#define _TYPEDEF_CriUint64
typedef unsigned long			CriUint64;		/* 符号なし８バイト整数 */
#endif

#ifndef _TYPEDEF_CriSint64
#define _TYPEDEF_CriSint64
typedef signed long				CriSint64;		/* 符号つき８バイト整数 */
#endif

#ifndef _TYPEDEF_CriUint128
#define _TYPEDEF_CriUint128						/* 符号なし16バイト整数 */
typedef unsigned int			CriUint128 __attribute__ ((mode (TI)));
#endif

#ifndef _TYPEDEF_CriSint128
#define _TYPEDEF_CriSint128						/* 符号つき16バイト整数 */
typedef int						CriSint128 __attribute__ ((mode (TI)));
#endif

#endif	/* endif XPT_TGT_EE */

#if defined(XPT_TGT_GC) || defined(XPT_TGT_WII)

#ifndef _TYPEDEF_CriUint64
#define _TYPEDEF_CriUint64
typedef unsigned long long		CriUint64;		/* 符号なし８バイト整数 */
#endif

#ifndef _TYPEDEF_CriSint64
#define _TYPEDEF_CriSint64
typedef signed long long		CriSint64;		/* 符号つき８バイト整数 */
#endif

#endif	/* endif XPT_TGT_GC */

#if	defined(XPT_TGT_MAC)

#ifndef _TYPEDEF_CriUint64
#define _TYPEDEF_CriUint64
typedef unsigned long long		CriUint64;		/* 符号なし８バイト整数 */
#endif

#ifndef _TYPEDEF_CriSint64
#define _TYPEDEF_CriSint64
typedef signed long long		CriSint64;		/* 符号つき８バイト整数 */
#endif

#endif	/* endif XPT_TGT_MAC */


#ifdef XPT_TGT_IOP

#ifndef _TYPEDEF_CriUint32
#define _TYPEDEF_CriUint32
typedef unsigned int			CriUint32;		/* 符号なし４バイト整数 */
#endif

#ifndef _TYPEDEF_CriSint32
#define _TYPEDEF_CriSint32
typedef signed int				CriSint32;		/* 符号つき４バイト整数 */
#endif

#endif	/* endif XPT_TGT_IOP */

/*****************************************************************************
 * CriUint32 CriSint32 CriUint64 CriSint64 CriUint128 CriSint128 (その2 デフォルトの定義)
 *****************************************************************************/

#ifndef _TYPEDEF_CriUint32
#define _TYPEDEF_CriUint32
typedef unsigned long			CriUint32;		/* 符号なし４バイト整数 */
#endif

#ifndef _TYPEDEF_CriSint32
#define _TYPEDEF_CriSint32
typedef signed long				CriSint32;		/* 符号つき４バイト整数 */
#endif

#ifndef _TYPEDEF_CriUint64
#define _TYPEDEF_CriUint64
typedef struct {								/* 符号なし8バイト整数 */
    CriUint32			h;						/* 上位32ビット 			*/
    CriUint32			l;						/* 下位32ビット 			*/
} CriUint64;
#endif

#ifndef _TYPEDEF_CriSint64
#define _TYPEDEF_CriSint64
typedef struct {								/* 符号つき8バイト整数 */
    CriSint32			h;						/* 上位32ビット 			*/
    CriUint32			l;						/* 下位32ビット 			*/
} CriSint64;
#endif

#ifndef _TYPEDEF_CriUint128
#define _TYPEDEF_CriUint128
typedef struct {								/* 符号なし16バイト整数 */
	CriUint64			h;						/* 上位64ビット */
	CriUint64			l;						/* 下位64ビット */
} CriUint128;
#endif

#ifndef _TYPEDEF_CriSint128
#define _TYPEDEF_CriSint128
typedef struct {								/* 符号つき16バイト整数 */
	CriSint64	h;								/* 上位64ビット */
	CriUint64	l;								/* 下位64ビット */
} CriSint128;
#endif


/*****************************************************************************
 * CriFloat16 CriFloat32 に関してはほぼ共通（PowerPlant使用時のみ例外）
 *****************************************************************************/

#ifndef _TYPEDEF_CriFloat16
#define _TYPEDEF_CriFloat16
typedef signed short			CriFloat16;		/* ２バイト実数 */
#endif

/* Mac環境でPowerPlant使用時はCriFloat32とCriFloat64がMacTypes.hで定義済み */
#ifndef __MACTYPES__

#ifndef _TYPEDEF_CriFloat32
#define _TYPEDEF_CriFloat32
typedef float					CriFloat32;		/* ４バイト実数 */
#endif

#ifndef _TYPEDEF_CriFloat64
#define _TYPEDEF_CriFloat64
typedef double					CriFloat64;		/* ８バイト実数 */
#endif

#endif	/* endif __MACTYPES__ */

/*****************************************************************************
 * CriFixed32 CriBool CriChar8 に関しては共通
 *****************************************************************************/

#ifndef _TYPEDEF_CriFixed32
#define _TYPEDEF_CriFixed32
typedef signed long				CriFixed32;		/* 固定小数点32ビット */
#endif

#ifndef _TYPEDEF_CriBool
#define _TYPEDEF_CriBool
typedef CriSint32				CriBool;		/* 論理型（論理定数を値にとる） */
#endif

#ifndef _TYPEDEF_CriChar8
#define _TYPEDEF_CriChar8
typedef char					CriChar8;		/* 文字型 */
#endif

/*****************************************************************************
 * ポインタを格納可能な整数型
 *****************************************************************************/

#ifndef _TYPEDEF_CriSintPtr
#define _TYPEDEF_CriSintPtr
typedef CriSint32				CriSintPtr;
#endif

#ifndef _TYPEDEF_CriUintPtr
#define _TYPEDEF_CriUintPtr
typedef CriUint32				CriUintPtr;
#endif


/*****************************************************************************
 * 呼び出し規約
 *****************************************************************************/

#ifndef CRIAPI
#define CRIAPI
#endif	/* endif CRIAPI */

#endif	/* CRI_XPTS_H */

/* end of file */
