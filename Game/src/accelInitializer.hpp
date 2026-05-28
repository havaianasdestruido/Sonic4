// ============================================================================
/*!
	@file	accelInitializer.hpp
	@brief	イニシャライザー

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: accelInitializer.hpp 2 2011-04-11 05:21:26Z thamada $
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


	
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	Initializer
		初期化済み構造体の取得(イニシャライザー)
 */
template <typename T>
inline T Initializer() {
	T result = {};
	return result;
}
template<typename T, typename T1>
inline T Initializer(const T1 &t1) {
	T result = {t1};
	return result;
}
template<typename T, typename T1, typename T2>
inline T Initializer(const T1 &t1, const T2 &t2) {
	T result = {t1, t2};
	return result;
}
template<typename T, typename T1, typename T2, typename T3>
inline T Initializer(const T1 &t1, const T2 &t2, const T3 &t3) {
	T result = {t1, t2, t3};
	return result;
}
template<typename T, typename T1, typename T2, typename T3, typename T4>
inline T Initializer(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4) {
	T result = {t1, t2, t3, t4};
	return result;
}
template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
inline T Initializer(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5) {
	T result = {t1, t2, t3, t4, t5};
	return result;
}
template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
inline T Initializer(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5, const T6 &t6) {
	T result = {t1, t2, t3, t4, t5, t6};
	return result;
}
template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
inline T Initializer(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5, const T6 &t6, const T7 &t7) {
	T result = {t1, t2, t3, t4, t5, t6, t7};
	return result;
}
template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
inline T Initializer(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5, const T6 &t6, const T7 &t7, const T8 &t8) {
	T result = {t1, t2, t3, t4, t5, t6, t7, t8};
	return result;
}
template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
inline T Initializer(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5, const T6 &t6, const T7 &t7, const T8 &t8, const T9 &t9) {
	T result = {t1, t2, t3, t4, t5, t6, t7, t8, t9};
	return result;
}
namespace detail {
	class CInitializer {
	public:
		template <typename T>
		operator T() {
			return Initializer<T>();
		}
	};
}
inline detail::CInitializer Initializer() {
	return detail::CInitializer();
}
//------------------------------------------------------------------------------**********







} //namespace accel

#endif //#if	defined(__cplusplus)


// =============================================================================
// accel::Function
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
