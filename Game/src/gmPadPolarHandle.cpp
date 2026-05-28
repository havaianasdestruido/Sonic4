// ============================================================================
/*!
	@file	gmPadPolarHandle.cpp
	@brief	極座標ハンドル

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2010 Dimps
	$Id: gmPadPolarHandle.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "gmPadPolarHandle.hpp"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*


namespace gm {
//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
CPadPolarHandle *CPadPolarHandle::p_instance	= NULL;						//<共通インスタンス
const float CPadPolarHandle::c_pi				= atan2(0.0f, -1.0f) * 2;	//π


//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// =============================================================================
// CPadPolarHandle::CreateInstance
/*!
	インスタンス構築

	@return インスタンス
 */
// =============================================================================
CPadPolarHandle &CPadPolarHandle::CreateInstance()
{
	if (!p_instance) {
		static CPadPolarHandle instance;
		p_instance = &instance;
	}
	return *p_instance;
}

// =============================================================================
// CPadPolarHandle::Create
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
bool CPadPolarHandle::Create()
{
	m_area.left() = 0.0f;
	m_area.top() = 0.0f;
	m_area.right() = static_cast<float>(AMD_SCREEN_2D_WIDTH);
	m_area.bottom() = static_cast<float>(AMD_SCREEN_2D_HEIGHT);
	m_center.x() = AMD_SCREEN_2D_WIDTH * 0.5f;
	m_center.y() = AMD_SCREEN_2D_HEIGHT * 0.5f;

	return create();
}
bool CPadPolarHandle::Create(const accel::CArray<f32, 4> &area)
{
	m_area = area;
	m_center.x() = (m_area.left() + m_area.right()) * 0.5f;
	m_center.y() = (m_area.top() + m_area.bottom()) * 0.5f;

	return create();
}
bool CPadPolarHandle::Create(const float (&area)[4])
{
	m_area.left() = area[0];
	m_area.top() = area[1];
	m_area.right() = area[2];
	m_area.bottom() = area[3];
	m_center.x() = (m_area.left() + m_area.right()) * 0.5f;
	m_center.y() = (m_area.top() + m_area.bottom()) * 0.5f;

	return create();
}
bool CPadPolarHandle::Create(f32 area_left, f32 area_top, f32 area_right, f32 area_bottom)
{
	m_area.left() = area_left;
	m_area.top() = area_top;
	m_area.right() = area_right;
	m_area.bottom() = area_bottom;
	m_center.x() = (m_area.left() + m_area.right()) * 0.5f;
	m_center.y() = (m_area.top() + m_area.bottom()) * 0.5f;

	return create();
}
bool CPadPolarHandle::Create(const accel::CArray<f32, AMD_XY> &center)
{
	m_area.left() = 0.0f;
	m_area.top() = 0.0f;
	m_area.right() = static_cast<float>(AMD_SCREEN_2D_WIDTH);
	m_area.bottom() = static_cast<float>(AMD_SCREEN_2D_HEIGHT);
	m_center = center;

	return create();
}
bool CPadPolarHandle::Create(const float (&center)[AMD_XY])
{
	m_area.left() = 0.0f;
	m_area.top() = 0.0f;
	m_area.right() = static_cast<float>(AMD_SCREEN_2D_WIDTH);
	m_area.bottom() = static_cast<float>(AMD_SCREEN_2D_HEIGHT);
	m_center.x() = center[AMD_X];
	m_center.y() = center[AMD_Y];

	return create();
}
bool CPadPolarHandle::Create(f32 center_x, f32 center_y)
{
	m_area.left() = 0.0f;
	m_area.top() = 0.0f;
	m_area.right() = static_cast<float>(AMD_SCREEN_2D_WIDTH);
	m_area.bottom() = static_cast<float>(AMD_SCREEN_2D_HEIGHT);
	m_center.x() = center_x;
	m_center.y() = center_y;

	return create();
}
bool CPadPolarHandle::Create(const accel::CArray<f32, 4> &area, const accel::CArray<f32, AMD_XY> &center)
{
	m_area = area;
	m_center = center;

	return create();
}
bool CPadPolarHandle::Create(const float (&area)[4], const float (&center)[AMD_XY])
{
	m_area.left() = area[0];
	m_area.top() = area[1];
	m_area.right() = area[2];
	m_area.bottom() = area[3];
	m_center.x() = center[AMD_X];
	m_center.y() = center[AMD_Y];

	return create();
}
bool CPadPolarHandle::Create(f32 area_left, f32 area_top, f32 area_right, f32 area_bottom, f32 center_x, f32 center_y)
{
	m_area.left() = area_left;
	m_area.top() = area_top;
	m_area.right() = area_right;
	m_area.bottom() = area_bottom;
	m_center.x() = center_x;
	m_center.y() = center_y;

	return create();
}

// =============================================================================
// CPadPolarHandle::Release
/*!
	破棄
 */
// =============================================================================
void CPadPolarHandle::Release()
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
bool CPadPolarHandle::IsValid() const
{
	return m_flag[BFlag::Setup];
}

// =============================================================================
// CPadPolarHandle::Update
/*!
	描画
 */
// =============================================================================
void CPadPolarHandle::Update()
{
	if (m_flag.test(BFlag::Setup)) {
		if (-1 == m_focus) {
			s32 focus = getPushTpIndex();
			if (0 <= focus) {
				m_focus = focus;
				float value = getCurrentValue();
				m_zero_point += value;
			}
		} else if (amTpIsTouchOn(m_focus)) {
			float value = getCurrentValue();
			m_value = value - m_zero_point;
		} else if (amTpIsTouchPull(m_focus)) {
			m_zero_point = -m_value;
			m_focus = -1;
		}
	}
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
// ============================================================================
// CPadPolarHandle::GetValue
/*!
	値の取得

	@return	値

	@note
		Angle32(s32)なら16bit度、
		float(f32)fならラジアンを返します。
		360度を返す関数はありません。
 */
// ============================================================================
template <>
float CPadPolarHandle::GetValue<float>() const
{
	return m_value;
}
template <>
Angle32 CPadPolarHandle::GetValue<Angle32>() const
{
	float value = GetValue<float>();
	return NNM_RADtoA32(value);
}

// ============================================================================
// CPadPolarHandle::IsFocus
/*!
	フォーカス中確認

	@retval true	フォーカス中
	@retval false	非フォーカス
 */
// ============================================================================
bool CPadPolarHandle::IsFocus() const
{
	return (-1 != m_focus);
}

// ============================================================================
// CPadPolarHandle::GetFocusTpIndex
/*!
	フォーカス中のタップインデックスの取得

	@return	フォーカス中のタップインデックス

	@note
		フォーカスしているタップインデックスが無い場合は -1 を返します。
 */
// ============================================================================
s32 CPadPolarHandle::GetFocusTpIndex() const
{
	return m_focus;
}


//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
// ============================================================================
// CPadPolarHandle::SetValue
/*!
	値の設定

	@param	value		[in]	設定値
 */
// ============================================================================
void CPadPolarHandle::SetValue(float value)
{
	m_zero_point = getCurrentValue() - value;
	m_value = value;
}
void CPadPolarHandle::SetValue(Angle32 value)
{
	SetValue(static_cast<float>(NNM_A32toRAD(value)));
}


//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// =============================================================================
// CPadPolarHandle::create
/*!
	構築

	@retval	true	成功
	@retval	false	失敗

	@note
		失敗しません。
 */
// =============================================================================
bool CPadPolarHandle::create()
{
	Release();

	m_value = 0.0f;
	m_focus = getOnTpIndex();
	if (0 <= m_focus) {
		float value = getCurrentValue();
		m_zero_point = value;
	} else {
		m_zero_point = 0.0f;
	}

	m_flag[BFlag::Setup] = true;
	return true;
}

// =============================================================================
// CPadPolarHandle::getCurrentValue
/*!
	現在値取得

	@return 現在値
 */
// =============================================================================
float CPadPolarHandle::getCurrentValue()
{
	float result;

	if ((0 <= m_focus) && (amTpIsTouchOn(m_focus))) {
		Uint16 *on = _am_tp_touch[m_focus].on;
		//非接触中
		if (amTpIsTouchPush(m_focus)) {
			//接触直後
			m_around = 0;
		} else if ((on[AMD_X] <= m_center.x()) && (m_prev.x() <= m_center.x())) {
			//接触直後以外
			//境界越え判定(中心位置から左側の水平線を超えると周回越えが発生する)
			//X位置は境界側
			if (m_center.y() <= on[AMD_Y]) {
				if (m_prev.y() < m_center.y()) {
					//反時計回りに境界越え
					--m_around;
				}
			} else {
				if (m_center.y() <= m_prev.y()) {
					//時計回りに境界越え
					++m_around;
				}
			}
		}

		m_prev = accel::CArray<f32, AMD_XY>::initializer(on[MTD_X], on[MTD_Y]);
		accel::CArray<f32, AMD_XY> diff = m_prev - m_center;
		result = atan2(diff.y(), diff.x()) + c_pi * static_cast<float>(m_around);
	} else {
		//非接触
		result = 0.0f;
	}

	return result;
}

// =============================================================================
// CPadPolarHandle::getOnTpIndex
/*!
	Onが発生したタップインデックス取得

	@return Onが発生したタップインデックス
	
	@note
		Pushが発生していなければ-1を返します。
		複数発生した場合は若い数字を返します。
 */
// =============================================================================
s32 CPadPolarHandle::getOnTpIndex()
{
	s32 result = -1;

	for (s32 i = 0, max = arrayof(_am_tp_touch); i < max; ++i) {
		if (amTpIsTouchOn(i) && isHit(_am_tp_touch[i].on)) {
			result = i;
			break;
		}
	}

	return result;
}

// =============================================================================
// CPadPolarHandle::getPushTpIndex
/*!
	Pushが発生したタップインデックス取得

	@return Pushが発生したタップインデックス
	
	@note
		Pushが発生していなければ-1を返します。
		複数発生した場合は若い数字を返します。
 */
// =============================================================================
s32 CPadPolarHandle::getPushTpIndex()
{
	s32 result = -1;

	for (s32 i = 0, max = arrayof(_am_tp_touch); i < max; ++i) {
		if (amTpIsTouchPush(i) && isHit(_am_tp_touch[i].push)) {
			result = i;
			break;
		}
	}

	return result;
}


// =============================================================================
// CPadPolarHandle::isHit
/*!
	ヒット確認
	
	@param	point	[in]	確認位置

	@retval	true	ヒット
	@retval	false	ヒットしていない
 */
// =============================================================================
bool CPadPolarHandle::isHit(const u16 (&point)[AMD_XY])
{
	bool result = false;
	accel::CArray<f32, AMD_XY> pos = accel::CArray<f32, AMD_XY>::initializer(point[AMD_X], point[AMD_Y]);

	if (pos.x() < m_area.left()) {
		//タッチ位置が矩形左端より左なら
	} else if (m_area.right() < pos.x()) {
		//タッチ位置が矩形右端より右なら
	} else if (pos.y() < m_area.top()) {
		//タッチ位置が矩形上端より上なら
	} else if (m_area.bottom() < pos.y()) {
		//タッチ位置が矩形下端より下なら
	} else {
		//矩形内なら
		result = true;
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
