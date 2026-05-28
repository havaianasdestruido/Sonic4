// ============================================================================
/*!
	@file	gmPadVirtualPad.cpp
	@brief	バーチャルパッド

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2010 Dimps
	$Id: gmPadVirtualPad.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "gmPadVirtualPad.hpp"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*


namespace gm {
//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
CPadVirtualPad *CPadVirtualPad::p_instance = NULL;		//<共通インスタンス


//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// =============================================================================
// CPadVirtualPad::CreateInstance
/*!
	インスタンス構築

	@return インスタンス
 */
// =============================================================================
CPadVirtualPad &CPadVirtualPad::CreateInstance()
{
	if (!p_instance) {
		static CPadVirtualPad instance;
		p_instance = &instance;
	}
	return *p_instance;
}

// =============================================================================
// CPadVirtualPad::Create
/*!
	構築

	@param	center		[in]	中心位置
	@param	center_y	[in]	中心位置Y
	@param	center_x	[in]	中心位置X

	@retval	true	成功
	@retval	false	失敗

	@note
		失敗しません。
 */
// =============================================================================
bool CPadVirtualPad::Create()
{
	m_area.left() = 0.0f;
	m_area.top() = 0.0f;
	m_area.right() = static_cast<float>(AMD_SCREEN_2D_WIDTH);
	m_area.bottom() = static_cast<float>(AMD_SCREEN_2D_HEIGHT);
	
	return create();
}
bool CPadVirtualPad::Create(const accel::CArray<f32, 4> &area)
{
	m_area = area;

	return create();
}
bool CPadVirtualPad::Create(const accel::CArray<f32, 2> &area_xy1, const accel::CArray<f32, 2> &area_xy2)
{
	m_area.left() = area_xy1.x();
	m_area.top() = area_xy1.y();
	m_area.right() = area_xy2.x();
	m_area.bottom() = area_xy2.y();

	return create();
}
bool CPadVirtualPad::Create(const f32 (&area)[4])
{
	for (int i = 0; i < 4; ++i) {
		m_area[i] = area[i];
	}

	return create();
}
bool CPadVirtualPad::Create(const f32 (&area_xy1)[2], const f32 (&area_xy2)[2])
{
	m_area.left() = area_xy1[0];
	m_area.top() = area_xy1[1];
	m_area.right() = area_xy2[0];
	m_area.bottom() = area_xy2[1];

	return create();
}
bool CPadVirtualPad::Create(f32 area_left, f32 area_top, f32 area_right, f32 area_bottom)
{
	m_area.left() = area_left;
	m_area.top() = area_top;
	m_area.right() = area_right;
	m_area.bottom() = area_bottom;

	return create();
}

// =============================================================================
// CPadVirtualPad::Release
/*!
	破棄
 */
// =============================================================================
void CPadVirtualPad::Release()
{
	if (m_flag[BFlag::Setup]) {

		m_flag[BFlag::Setup] = false;
	}
}

// =============================================================================
// IIsValid::IsValid
/*!
	有効確認

	@retval	true	有効
	@retval	false	無効
 */
// =============================================================================
bool CPadVirtualPad::IsValid() const
{
	return m_flag[BFlag::Setup];
}

// =============================================================================
// CPadVirtualPad::Update
/*!
	更新
 */
// =============================================================================
void CPadVirtualPad::Update()
{
	if (m_flag.test(BFlag::Setup)) {
		//フォーカスの更新
		for (int i = 0, max = m_focus.size(); i < max; ++i) {
			if (m_focus.test(i)) {
				//フォーカス中
				if (!amTpIsTouchOn(i) || !isHit(_am_tp_touch[i].on)) {
					m_focus.set(i, false);
				}
			} else {
				//非フォーカス
				if (amTpIsTouchOn(i) && isHit(_am_tp_touch[i].on)) {
					m_focus.set(i, true);
				}
			}
		}
		//onフラグの更新
		u16 on_flag = 0;
		for (int i = 0, max = m_focus.size(); i < max; ++i) {
			if (m_focus.test(i)) {
				on_flag |= getOnFlag(_am_tp_touch[i].on);
			}
		}
		//フラグの整理(右・下優先)
		if ((KEY_L_LEFT | KEY_L_RIGHT) == ((KEY_L_LEFT | KEY_L_RIGHT) & on_flag)) {
			on_flag &= ~KEY_L_LEFT;
		}
		if ((KEY_L_UP | KEY_L_DOWN) == ((KEY_L_UP | KEY_L_DOWN) & on_flag)) {
			on_flag &= ~KEY_L_UP;
		}
		//設定
		m_on_flag.push_front(on_flag);
	}
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
// ============================================================================
// CPadVirtualPad::GetValue
/*!
	値の取得

	@return	値
 */
// ============================================================================
u16 CPadVirtualPad::GetValue() const
{
	return m_on_flag[0];
}

// ============================================================================
// CPadVirtualPad::IsFocus
/*!
	フォーカス中確認

	@retval true	フォーカス中
	@retval false	非フォーカス
 */
// ============================================================================
bool CPadVirtualPad::IsFocus(int tp_index) const
{
	return m_focus[tp_index];
}

//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// =============================================================================
// CPadVirtualPad::create
/*!
	構築

	@retval	true	成功
	@retval	false	失敗

	@note
		失敗しません。
 */
// =============================================================================
bool CPadVirtualPad::create()
{
	Release();

	m_flag[BFlag::Setup] = true;
	return true;
}

// =============================================================================
// CPadVirtualPad::isHit
/*!
	ヒット確認
	
	@param	point	[in]	確認位置

	@retval	true	ヒット
	@retval	false	ヒットしていない
 */
// =============================================================================
bool CPadVirtualPad::isHit(const u16 (&point)[AMD_XY])
{
	bool result = false;
	accel::CArray<f32, AMD_XY> pos = accel::CArray<f32, AMD_XY>::initializer(point[AMD_X], point[AMD_Y]);

	if (pos.x() < m_area.left()) {
		//確認位置が矩形左端より左なら
	} else if (m_area.right() < pos.x()) {
		//確認位置が矩形右端より右なら
	} else if (pos.y() < m_area.top()) {
		//確認位置が矩形上端より上なら
	} else if (m_area.bottom() < pos.y()) {
		//確認位置が矩形下端より下なら
	} else {
		//矩形内なら
		result = true;
	}

	return result;
}

// =============================================================================
// CPadVirtualPad::getOnFlag
/*!
	onフラグの取得
	
	@param	point	[in]	確認位置

	@return onフラグ
 */
// =============================================================================
u16 CPadVirtualPad::getOnFlag(const u16 (&point)[AMD_XY])
{
	accel::CArray<f32, AMD_XY> pos = accel::CArray<f32, AMD_XY>::initializer(point[AMD_X], point[AMD_Y]);
	accel::CArray<f32, AMD_XY> center = accel::CArray<f32, AMD_XY>::initializer(	(m_area.left() + m_area.right()) * 0.5f
																				,	(m_area.top() + m_area.bottom()) * 0.5f
																				);
	u16 result = 0;
	//矩形判定
	{
		const float c_zone_range = 0.4f;	//中心から c_zone_range/2 の範囲を取る
		const float c_height_range_half = (m_area.bottom() - m_area.top()) * c_zone_range * 0.5f;
		accel::CArray<f32, AMD_XY> left_xy1 = accel::CArray<f32, AMD_XY>::initializer(m_area.left(), center.y() - c_height_range_half);
		accel::CArray<f32, AMD_XY> left_xy2 = accel::CArray<f32, AMD_XY>::initializer(center.x(), center.y() + c_height_range_half);
		accel::CArray<f32, AMD_XY> right_xy1 = accel::CArray<f32, AMD_XY>::initializer(center.x(), center.y() - c_height_range_half);
		accel::CArray<f32, AMD_XY> right_xy2 = accel::CArray<f32, AMD_XY>::initializer(m_area.right(), center.y() + c_height_range_half);
		if (isHit(pos, right_xy1, right_xy2)) {
			result = KEY_L_RIGHT;
		} else if (isHit(pos, left_xy1, left_xy2)) {
			result = KEY_L_LEFT;
		}
	}

	//X判定
	if (0 == result) {
		//まだヒットしていないなら
		accel::CArray<f32, AMD_XY> up_left = accel::CArray<f32, AMD_XY>::initializer(m_area.left(), m_area.top());
		accel::CArray<f32, AMD_XY> up_right = accel::CArray<f32, AMD_XY>::initializer(m_area.right(), m_area.top());
		accel::CArray<f32, AMD_XY> down_left = accel::CArray<f32, AMD_XY>::initializer(m_area.left(), m_area.bottom());
		accel::CArray<f32, AMD_XY> down_right = accel::CArray<f32, AMD_XY>::initializer(m_area.right(), m_area.bottom());
		if (isHit(pos, center, up_right, down_right)) {
			result = KEY_L_RIGHT;
		} else if (isHit(pos, center, down_left, up_left)) {
			result = KEY_L_LEFT;
		} else if (isHit(pos, center, up_left, up_right)) {
			result = KEY_L_UP;
		} else if (isHit(pos, center, down_right, down_left)) {
			result = KEY_L_DOWN;
		}
	}

	return result;
}

// =============================================================================
// CPadVirtualPad::isHit
/*!
	ヒット確認(矩形)
	
	@param	point	[in]	確認位置
	@param	xy1		[in]	矩形(左上)
	@param	xy2		[in]	矩形(右下)

	@retval	true	ヒット
	@retval	false	ヒットしていない
 */
// =============================================================================
bool CPadVirtualPad::isHit(const accel::CArray<f32, AMD_XY> &target, const accel::CArray<f32, AMD_XY> &xy1, const accel::CArray<f32, AMD_XY> &xy2)
{
	bool result = false;
	if (target.x() < xy1.x()) {
		//確認位置が矩形左端より左なら
	} else if (xy2.x() < target.x()) {
		//確認位置が矩形右端より右なら
	} else if (target.y() < xy1.y()) {
		//確認位置が矩形上端より上なら
	} else if (xy2.y() < target.y()) {
		//確認位置が矩形下端より下なら
	} else {
		//矩形内なら
		result = true;
	}
	return result;
}

// =============================================================================
// CPadVirtualPad::isHit
/*!
	ヒット確認(三角形)
	
	@param	point	[in]	確認位置
	@param	p1		[in]	三角形点1
	@param	p2		[in]	三角形点2
	@param	p3		[in]	三角形点3

	@retval	true	ヒット
	@retval	false	ヒットしていない
 */
// =============================================================================
bool CPadVirtualPad::isHit(const accel::CArray<f32, AMD_XY> &target, const accel::CArray<f32, AMD_XY> &p1, const accel::CArray<f32, AMD_XY> &p2, const accel::CArray<f32, AMD_XY> &p3)
{
	class CLocalLogic {
	public:
		//外積
		static f32 Cross (const accel::CArray<f32, AMD_XY> &p1, const accel::CArray<f32, AMD_XY> &p2) {
			return p1.x() * p2.y() - p1.y() * p2.x();
		}
	};
	
	f32 p12 = CLocalLogic::Cross(p1 - target, p2 - target);
	f32 p23 = CLocalLogic::Cross(p2 - target, p3 - target);
	f32 p31 = CLocalLogic::Cross(p3 - target, p1 - target);
	
	bool result;
	if ((0.0f < p12 * p23) && (0.0f < p12 * p31)) {
		result = true;
	} else {
		result = false;
	}
	return result;
}


//------------------------------------------------------------------------------**********



























































































} //namespace gm

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
