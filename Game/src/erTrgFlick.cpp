// ============================================================================
/*!
	@file	erTrgFlick.cpp
	@brief	タッチパネルアクション(トリガ・フリック領域)

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009-2010 Dimps
	$Id: erTrgFlick.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include  <cmath>
#include "erTrgFlick.hpp"



//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*


namespace er {
//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// =============================================================================
// CTrgFlick::Create
/*!
	作成

	@retval	true	成功
	@retval	false	失敗

	@note
		失敗しません。
 */
// ==========================================================================
bool CTrgFlick::Create(const accel::CArray<Sint32, 4> &rect)
{
	Release();

	m_rect = rect;

	return create();
}
bool CTrgFlick::Create(const accel::CArray<Sint32, 2> &xy1, const accel::CArray<Sint32, 2> &xy2)
{
	Release();

	m_rect.left() = xy1.x();
	m_rect.top() = xy1.y();
	m_rect.right() = xy2.x();
	m_rect.bottom() = xy2.y();

	return create();
}
bool CTrgFlick::Create(Sint32 x1, Sint32 y1, Sint32 x2, Sint32 y2)
{
	Release();

	m_rect.left() = x1;
	m_rect.top() = y1;
	m_rect.right() = x2;
	m_rect.bottom() = y2;

	return create();
}

// =============================================================================
// CTrgFlick::Release
/*!
	破棄
 */
// ==========================================================================
void CTrgFlick::Release()
{
	if (m_flag.test(BFlag::Setup)) {
		m_flag.set(BFlag::Setup, false);
	}
}

// =============================================================================
// CTrgFlick::IsValid
/*!
	有効確認

	@retval	true	有効
	@retval	false	無効
 */
// ==========================================================================
bool CTrgFlick::IsValid() const
{
	return m_flag.test(BFlag::Setup);
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// =============================================================================
// CTrgFlick::create
/*!
	生成
 */
// ==========================================================================
bool CTrgFlick::create()
{
	m_pos = TPos::initializer();

	//初期化
	ResetState();
	SetRepeatInterval();
	SetDoubleClickTime();
	SetMoveThreshold();

	m_flag.set(BFlag::Setup, true);

	return true;
}

// =============================================================================
// CTrgFlick::hitTest
/*!
	ヒットテスト

	@param	pos		[in]	タッチ位置
	@param	index	[in]	タッチインデックス

	@retval	true	ヒット
	@retval	false	未ヒット
 */
// ==========================================================================
bool CTrgFlick::hitTest(const TPos &pos, Uint32 index)
{
	bool result = false;
	if (m_flag.test(BFlag::Setup)) {
		if (pos.x() < m_pos.x() + m_rect.left()) {
			//タッチ位置が矩形左端より左なら
		} else if (m_pos.x() + m_rect.right() < pos.x()) {
			//タッチ位置が矩形右端より右なら
		} else if (pos.y() < m_pos.y() + m_rect.top()) {
			//タッチ位置が矩形上端より上なら
		} else if (m_pos.y() + m_rect.bottom() < pos.y()) {
			//タッチ位置が矩形下端より下なら
		} else {
			//矩形内なら
			result = true;
		}
	}
	return result;

	UNREFERENCED_PARAMETER(index);
}


//------------------------------------------------------------------------------**********


































































} //namespace er

// =============================================================================
// Function
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
