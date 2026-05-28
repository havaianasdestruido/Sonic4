// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       amTypes.h
	@brief      データ型定義 ヘッダ
	@author	    Syuichi Gotou
	@date	    Date: 2009/04/01 
 */
// ================================================================
#ifndef _AM_TYPES_H
#define _AM_TYPES_H

//----- Include Files --------------------------------------------------
#include "NN.h"

//----- Definitions ----------------------------------------------------
#define BIT_0  (0x00000001) //  0bit
#define BIT_1  (0x00000002) //  1bit
#define BIT_2  (0x00000004) //  2bit
#define BIT_3  (0x00000008) //  3bit
#define BIT_4  (0x00000010) //  4bit
#define BIT_5  (0x00000020) //  5bit
#define BIT_6  (0x00000040) //  6bit
#define BIT_7  (0x00000080) //  7bit
#define BIT_8  (0x00000100) //  8bit
#define BIT_9  (0x00000200) //  9bit
#define BIT_10 (0x00000400) // 10bit
#define BIT_11 (0x00000800) // 11bit
#define BIT_12 (0x00001000) // 12bit
#define BIT_13 (0x00002000) // 13bit
#define BIT_14 (0x00004000) // 14bit
#define BIT_15 (0x00008000) // 15bit
#define BIT_16 (0x00010000) // 16bit
#define BIT_17 (0x00020000) // 17bit
#define BIT_18 (0x00040000) // 18bit
#define BIT_19 (0x00080000) // 19bit
#define BIT_20 (0x00100000) // 20bit
#define BIT_21 (0x00200000) // 21bit
#define BIT_22 (0x00400000) // 22bit
#define BIT_23 (0x00800000) // 23bit
#define BIT_24 (0x01000000) // 24bit
#define BIT_25 (0x02000000) // 25bit
#define BIT_26 (0x04000000) // 26bit
#define BIT_27 (0x08000000) // 27bit
#define BIT_28 (0x10000000) // 28bit
#define BIT_29 (0x20000000) // 29bit
#define BIT_30 (0x40000000) // 30bit
#define BIT_31 (0x80000000) // 31bit

//----- Macros ---------------------------------------------------------

//----- Enum Definitions -----------------------------------------------

//----- Type Definitions -----------------------------------------------

#if _PS3 | _IPHONE
typedef Sint32		BOOL;
typedef char		CHAR;
#endif

// ベクトル
typedef struct {
	Sint32		x, y, z, w;
} AMS_VECTOR4I;

// ベクトル
typedef struct {
	Sint32		x, y, z;
} AMS_VECTOR3I;

typedef NNS_VECTOR		AMS_VECTOR3;
typedef NNS_VECTOR4D	AMS_VECTOR;

// クォータニオン
typedef NNS_QUATERNION	AMS_QUAT;

// マトリクス
typedef NNS_MATRIX		AMS_MATRIX;

//! 32ビット整数カラー RGBA8888
typedef union {
    struct {
        Uint8      r;     //!< R (赤)
        Uint8      g;     //!< G (緑)
        Uint8      b;     //!< B (青)
        Uint8      a;     //!< A (アルファチャンネル)
    };
    Uint32         color; //!< カラー
} AMS_RGBA8888;
typedef AMS_RGBA8888 AMS_RGBA32; //!< 32ビット整数カラー RGBA32(8:8:8:8)

//----- Include Files --------------------------------------------------
#include "format/AME.h"


#endif	// _AM_TYPES_H
