// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       amUtility.h
	@brief      ユーティリティライブラリ　ヘッダ
	@author	    Syuichi Gotou
	@date	    Date: 2009/04/01 
 */
// ================================================================
#ifndef _AM_UTILITY_H
#define _AM_UTILITY_H

//----- Include Files --------------------------------------------------
//----- Definitions ----------------------------------------------------
//----- Macros ---------------------------------------------------------

//! 数値
#define AMD_MATH_E			(2.7182818284590452354f)	//!< e
#define AMD_MATH_LOG2E		(1.4426950408889634074f)	//!< log2(e)
#define AMD_MATH_LOG10E		(0.4342944819032518276f)	//!< log10(e)
#define AMD_MATH_LN2		(0.6931471805599453094f)	//!< ln(2)
#define AMD_MATH_LN10		(2.3025850929940456840f)	//!< ln(10)
#define AMD_MATH_PI			(NND_PI)					//!< pi		(3.14159265358979323846f)
#define AMD_MATH_2PI		(NND_PI*2)					//!< 2 * pi	(3.14159265358979323846f * 2.0f)
#define AMD_MATH_PI_2		(1.5707963267948966192f)	//!< pi / 2
#define AMD_MATH_PI_4		(0.7853981633974483096f)	//!< pi / 4
#define AMD_MATH_3PI_4		(2.3561944901923448370f)	//!< 3 * pi / 2
#define AMD_MATH_SQRTPI		(1.7724538509055160279f)	//!< sqrt(pi)
#define AMD_MATH_1_PI		(0.3183098861837906715f)	//!< 1 / pi
#define AMD_MATH_2_PI		(0.6366197723675813430f)	//!< 2 / pi
#define AMD_MATH_2_SQRTPI	(1.1283791670955125739f)	//!< 2 / sqrt(pi)
#define AMD_MATH_SQRT2		(1.4142135623730950488f)	//!< sqrt(2)
#define AMD_MATH_1_SQRT2	(0.7071067811865475244f)	//!< 1 / sqrt(2)
#define AMD_MATH_SQRT3		(1.7320508075688772935f)	//!< sqrt(3)
#define AMD_MATH_1_LN2		(1.4426950408889633870f)	//!< 1 / log(2)
#define AMD_MATH_1_LN10		(0.4342944819032518276f)	//!< 1 / log(10)
#define AMD_MATH_LOG10TWO	(0.3010299956639811952f)	//!< log10(2)
#define AMD_MATH_LOG2TEN	(3.3219280948873623478f)	//!< log2(10)
#define AMD_MATH_SQRT3_2	(0.8660254037844386467f)	//!< sqrt(3) / 2
#define AMD_MATH_S8TOFLOAT	(0.0039215686274509803f)	//!< 1.0f/255.0f

//! ON・OFF
#define AMD_ON							(1)
#define AMD_OFF							(0)

//! 配列の要素数
#define arrayof(array_)					(sizeof(array_) / sizeof((array_)[0]))

//! メモリコピー
#define amCopyMemory(dest_,src_,size_)	(memcpy(dest_, src_, size_))

//! 最大値
#define amMax(a_, b_)					(((a_) > (b_))	? (a_): (b_))

//! 最小値
#define amMin(a_, b_)					(((a_) < (b_))	? (a_): (b_))

//! 絶対値
#define amAbs(a_)						(((a_) >= 0)	? (a_): (-a_))

//! クランプ
#define amClamp(n_, min_, max_)			((n_) < (min_) ? (min_) : ((n_) > (max_) ? (max_): (n_)))
#define amClamp_(n_, min_, max_, tmp_)	(((tmp_) = (n_)) < (min_)? (min_): ((tmp_) > (max_)? (max_): (tmp_)))

//! 平方根
#define amSqrt(fs_)		nnSqrt(fs_)

//! 累乗
#define amPow2(n_)						((n_) * (n_))
#define amPow3(n_)						((n_) * (n_) * (n_))

//! 補間
#define amLerp(zero_, one_, rate_)		((zero_) + ((one_) - (zero_)) * (float)(rate_))

//! フラグ操作
#define amFlagOn(dst_, on_)   			((dst_) |=  (on_))
#define amFlagOff(dst_, off_)   		((dst_) &= ~(off_))
#define amFlagFlip(dst_, flip_)			((dst_) ^=  (flip_))
#define amFlagOnOff(dst_, on_, off_)	((dst_) = (((dst_) & ~(off_)) | (on_)))

//! メンバのオフセット
#if !defined(offsetof)
#define offsetof(type_, member_)		((size_t)(&((type_*)0)->member_))
#undef offsetof
#endif // !defined(offsetof)

//! ループ
#define amLoop(n_, min_, max_) \
	((n_) < (min_) \
	? ((n_) + ((max_) - (min_))) \
	: ((n_) >= (max_) ? ((n_) - ((max_) - (min_))) : (n_)))
#define amLoopHigher(n_, min_, max_) \
	((n_) >= (max_) ? ((n_) - ((max_) - (min_))): (n_))
#define amLoopLower(n_, min_, max_) \
	((n_) < (min_) ? (n_ + ((max_) - (min_))): (n_))

//! アライメントサイズ (静的定義用)
#define AMM_ALIGN_SIZE(size_, align_)			(((size_) + ((align_) - 1)) & ~((align_) - 1))

//! 色の格納
// floatColor → NNS_RGBA8888
#define	AMD_FCOLTORGBA8888( r, g, b, a )	(((((Uint32)(r*255))&0xFF) << 24)	\
								   | ((((Uint32)(g*255))&0xFF) << 16)	\
								   | ((((Uint32)(b*255))&0xFF) <<  8)	\
								   | ((((Uint32)(a*255))&0xFF) <<  0))

// intColor → NNS_RGBA8888
#define	AMD_RGBA8888( r, g, b, a )	(((((Uint32)(r))&0xFF) << 24)	\
								   | ((((Uint32)(g))&0xFF) << 16)	\
								   | ((((Uint32)(b))&0xFF) <<  8)	\
								   | ((((Uint32)(a))&0xFF) <<  0))

//! 文字列処理
#if _PC | _XBOX
#define AMD_WCSCAT_S(_a,_b)				wcscat_s(_a,_b)
#define AMD_WCSCPY_S(_a,_b)				wcscpy_s(_a,_b)
#define AMD_WCSCAT2_S(_a,_b,_c)			wcscat_s(_a,_b,_c)
#define AMD_WCSCPY2_S(_a,_b,_c)			wcscpy_s(_a,_b,_c)
#define AMD_STRNCPY_S(_a,_b,_c,_d)		strncpy_s(_a,_b,_c,_d)
#define AMD_STRCPY_S(_a,_b,_c)			strcpy_s(_a,_b,_c)
#define AMD_SPRINTF_S(_a,_b,...)		sprintf_s(_a,_b, __VA_ARGS__)
#define AMD_MEMCPY_S(_a,_b,_c,_d)		memcpy_s(_a,_b,_c,_d)
#define AMD_STRCAT_S(_a,_b,_c)			strcat_s(_a,_b,_c)
#else
#define AMD_WCSCAT_S(_a,_b)				wcscat(_a,_b)
#define AMD_WCSCPY_S(_a,_b)				wcscpy(_a,_b)
#define AMD_WCSCAT2_S(_a,_b,_c)			wcscat(_a,_c)
#define AMD_WCSCPY2_S(_a,_b,_c)			wcscpy(_a,_c)
#define AMD_STRNCPY_S(_a,_b,_c,_d)		strncpy(_a,_c,_d)
#define AMD_STRCPY_S(_a,_b,_c)			strcpy(_a,_c)
#define AMD_SPRINTF_S(_a,_b,...)		snprintf(_a,_b, __VA_ARGS__)
#define AMD_MEMCPY_S(_a,_b,_c,_d)		memcpy(_a,_c,_d)
#define AMD_STRCAT_S(_a,_b,_c)			strcat(_a,_c)
#endif

// 固定小数
#define AMD_FX32_ONE			((Sint32) 0x00001000L)			// 1.000
#define AMD_FX32_TO_FLOAT(a)	((float)(a) / AMD_FX32_ONE)
#define AMD_FLOAT_TO_FX32(a)	((Sint32)((a) * AMD_FX32_ONE))

//----- Enum Definitions -----------------------------------------------

//! グリッド表示の描画タイプ
typedef enum _AMUTL_GRIDTYPE {
    AMUTL_GRIDTYPE_XY      = 0,  //!< XY平面
	AMUTL_GRIDTYPE_YZ      = 1,  //!< YZ平面
	AMUTL_GRIDTYPE_ZX      = 2,  //!< ZX平面
} AMUTIL_GRIDTYPE;

//----- Inline Functions ---------------------------------------------
//## Inline Functions

// ================================================================
/*!
	ゼロ判定 (+0.0 or -0.0)

	@param	fs [i]		数値
	
	@retval		0 :  0.0 以外
	@retval		1 :  0.0
*/
// ================================================================
inline Sint32 amIsZerof(float fs)
{
	if ( 0.000f < fs || fs < 0.000f )
		return 0;
	return 1;
}

// ================================================================
/*!
	剰余

	@param	fs [i]		数値1
	@param	fd [i]		数値2
	
	@return			剰余
*/
// ================================================================
inline float amModf(float fs, float fd)
{
	return fmod( fs, fd );
}

//! 正弦、余弦
inline void amSinCos(Angle32 angle, float* pSn, float *pCs)
{
    return nnSinCos(angle, pSn, pCs);
}

//! 正弦、余弦
inline void amSinCos(float radian, float* pSn, float *pCs)
{
	amSinCos( NNM_RADtoA32(radian), pSn, pCs );
}

//----- External Functions ---------------------------------------------

// ================================================================
/*!
	グリッド作成（内部で描画タスク生成）

	@param range   [input] 線と線の幅
	@param linenum [input] 縦線の数（横線もこの数値と同じとみなす）
	@param type    [input] グリッドタイプ
	@param prio    [input] 描画タスクのプライオリティ
*/
// ================================================================
void amUtilMakeGrid(float range=5.0f, Sint32 linenum=50, Sint32 type=AMUTL_GRIDTYPE_ZX, Uint16 prio=0x1000);

// ================================================================
/*!
	座標軸作成（内部で描画タスク生成）

	@param len [input] 座標軸の長さ
	@param prio    [input] 描画タスクのプライオリティ
*/
// ================================================================
void amUtilMakeAxis(Sint32 len, Uint16 prio=0x1000);

//! メモリゼロクリア
// Xbox360はmemsetよりも高速な方法がある
extern void amZeroMemory(void *mem, size_t size);

//! アライメントを考慮したサイズ計算
extern size_t amCalcAlignSize(size_t size, size_t align);


#endif	// _AM_UTILITY_H