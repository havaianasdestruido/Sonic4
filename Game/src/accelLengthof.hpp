// ============================================================================
/*!
	@file	accelLengthof.hpp
	@brief	lengthof演算子

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: accelLengthof.hpp 2 2011-04-11 05:21:26Z thamada $
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
	@brief	lengthof演算子
		配列の長さ取得
	@note
		constexpr関数では無いので注意
 */
template<typename T, unsigned int N>
inline unsigned int lengthof(const T (&)[N]) {
	return N;
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
