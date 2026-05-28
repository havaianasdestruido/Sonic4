// ============================================================================
/*!
	@file	accelMpl.hpp
	@brief	メタプログラミングライブラリ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2008-2009 Dimps
	$Id: accelMpl.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

#pragma once
#if	defined(__cplusplus)

//------ Include ---------------------- インクルード ---------------------------******_IC*
//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace accel {
namespace mpl {


	
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	NULLタイプ
		NULL扱いされるクラス
 */
class CNullType {};
//------------------------------------------------------------------------------**********


//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	空タイプ
		空扱いされるクラス
 */
struct CEmptyType {};
//------------------------------------------------------------------------------**********


//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	数値タイプ
		数値を型として扱うクラス
		同じ数値からは常に同じ型が作成され、違う数値からは常に違う型が作成される
		主にオーバーロードで使用する
 */
template <int val>
struct CInt2Type {
	static const int Value = val;
	static const int value = val;
};
//------------------------------------------------------------------------------**********


//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	クラスタイプ
		クラスを型として扱うクラス
		このクラスを通す事により、メモリ確保やコンストラクタを除去する事が出来る
		データは必要無いが、型情報だけ欲しい場合に使用する
		主にオーバーロードで使用する
 */
template <typename T>
struct CType2Type {
	typedef T OriginalType;
	typedef T original_type;
	typedef T value_type;
};
//------------------------------------------------------------------------------**********


//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	スタティックアサートクラス
		コンパイル時に決定するアサート検出を行う
 */
namespace detail {
	template <bool>
	struct CComplieTimeChecker {
		CComplieTimeChecker(...);
	};
	template <>
	struct CComplieTimeChecker<false> {
	};

	template <bool>
	struct CCompliteTimeError;
	template <>
	struct CCompliteTimeError<true> {
	};
} //namespace detail
#define ACLM_MPL_STATIC_ASSERT_MSG(expr, msg)												\
{																							\
	class ERROR_##msg {};																	\
	(void)sizeof(accel::mpl::detail::CComplieTimeChecker<(expr) != 0>((ERROR_##msg())));	\
}																							//
#define ACLM_MPL_STATIC_ASSERT(expr) (accel::mpl::detail::CCompliteTimeError<(expr) != 0>())
//------------------------------------------------------------------------------**********







} //namespace mpl
} //namespace accel

#endif //#if	defined(__cplusplus)


// =============================================================================
// accel::mpl::Function
/*!
	関数の説明

	@param	org1	[io]	引数１の説明
	@param	org2	[in]	引数２の説明
	@param	org3	[out]	引数３の説明

	@return	戻り値の説明
		or
	@retval	0	正常
	@retval	!0	異常

	@exception 例外
 
	@note
		補足説明
 */
// ==========================================================================
