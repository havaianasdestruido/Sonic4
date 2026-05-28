// ============================================================================
/*!
	@file	erTrg.cpp
	@brief	タッチパネルアクション(トリガ)

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009-2010 Dimps
	$Id$
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include  <cmath>
#include "erTrg.hpp"



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
// CTrgBase::Update
/*!
	更新
 */
// ==========================================================================
template<typename TState>
void CTrgBase<TState>::Update()
{
	if (IsValid() && !m_flag[BFlag::Frieze]) {
		//セットアップ済み かつ 凍結で無いなら
		//状態更新
		for (typename TStateArray::size_type i = 0; i < TStateArray::static_size; ++i) {
			bool									is_on = false;
			bool									is_edge;
			typename TStateArray::value_type::TMove	move;
			int index = static_cast<int>(i);
			AMS_TP_TOUCH_STATUS &touch = _am_tp_touch[index];
			if (amTpIsTouchOn(index)) {
				//ONなら
				is_edge	= amTpIsTouchPush(index);
				if (is_edge) {
					//ONエッジなら
					m_pos[index] = TPos::initializer(touch.push[MTD_X], touch.push[MTD_Y]);
				}
				TPos pos = TPos::initializer(touch.on[MTD_X], touch.on[MTD_Y]);
				if (!m_flag[BFlag::NoHit]) {
					is_on = hitTest(pos, static_cast<Uint32>(i));
				}
				move = pos - m_pos[index];
				m_pos[index] = pos;
			} else {
				//OFFなら
				//is_on = false;
				is_edge = amTpIsTouchPull(index);
				move = TPos::initializer(touch.pull[MTD_X], touch.pull[MTD_Y]) - m_pos[index];
				if (is_edge) {
					//OFFエッジなら
					m_pos[index] = TPos::initializer();
				}
			}
			typename TStateArray::value_type &state = m_state[i];
			state.Push(is_on, is_edge, move);
		}
	}
}

// ============================================================================
// CTrgBase::ResetState
/*!
	状態の初期化

	@param	index	[in]	インデックス

	@note
		インデックスが省略された場合は、全て対応します。
 */
// ============================================================================
template<typename TState>
void CTrgBase<TState>::ResetState()
{
	for (typename TStateArray::iterator ite = m_state.begin(), ite_end = m_state.end(); ite != ite_end; ++ite) {
		ite->ResetState();
	}
}
template<typename TState>
void CTrgBase<TState>::ResetState(Uint32 index)
{
	amAssert(index < m_state.size());
	m_state[index].ResetState();
}

// ============================================================================
// CTrgBase::AddLock
/*!
	ロックの付加

	@param	index	[in]	インデックス
 */
// ============================================================================
template<typename TState>
void CTrgBase<TState>::AddLock()
{
	getState().AddLock();
}
template<typename TState>
void CTrgBase<TState>::AddLock(Uint32 index)
{
	getState(index).AddLock();
}

// ============================================================================
// CTrgBase::DelLock
/*!
	ロックの除去

	@param	index	[in]	インデックス
 */
// ============================================================================
template<typename TState>
void CTrgBase<TState>::DelLock()
{
	getState().DelLock();
}
template<typename TState>
void CTrgBase<TState>::DelLock(Uint32 index)
{
	getState(index).DelLock();
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
// ============================================================================
// CTrgBase::IsFrieze
/*!
	状態の凍結確認
 */
// ============================================================================
template<typename TState>
bool CTrgBase<TState>::IsFrieze() const
{
	return m_flag[BFlag::Frieze];
}

// ============================================================================
// CTrgBase::IsNoHit
/*!
	無接触確認
 */
// ============================================================================
template<typename TState>
bool CTrgBase<TState>::IsNoHit() const
{
	return m_flag[BFlag::NoHit];
}

// ============================================================================
// CTrgBase::GetState
/*!
	状態取得

	@param	index	[in]	インデックス

	@note
		インデックスが省略された場合は、
		インデックスの若いロック中の値を返します。
		ロック中の値が無ければインデックスの若いオン中の値を返します。
		ロック中の値が無ければ0番目が返ります。
 */
// ============================================================================
template<typename TState>
const typename CTrgBase<TState>::TState &CTrgBase<TState>::GetState() const
{
	return getState();
}
template<typename TState>
const typename CTrgBase<TState>::TState &CTrgBase<TState>::GetState(Uint32 index) const
{
	return getState(index);
}

// ============================================================================
// CTrgBase::GetRepeatInterval
/*!
	リピート間隔の取得
 */
// ============================================================================
template<typename TState>
const typename CTrgBase<TState>::TRepeatInterval &CTrgBase<TState>::GetRepeatInterval() const
{
	return GetState(0).GetRepeatInterval();
}

// ============================================================================
// CTrgBase::GetDoubleClickTime
/*!
	ダブルクリック感知時間の取得
 */
// ============================================================================
template<typename TState>
typename CTrgBase<TState>::TDoubleClickTime CTrgBase<TState>::GetDoubleClickTime() const
{
	return GetState(0).GetDoubleClickTime();
}

// ============================================================================
// CTrgBase::GetMoveThreshold
/*!
	移動感知閾値の取得
 */
// ============================================================================
template<typename TState>
typename CTrgBase<TState>::TMoveThreshold CTrgBase<TState>::GetMoveThreshold() const
{
	return GetState(0).GetMoveThreshold();
}


//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
// ============================================================================
// CTrgBase::SetFrieze
/*!
	状態の凍結設定
 */
// ============================================================================
template<typename TState>
void CTrgBase<TState>::SetFrieze(bool frieze)
{
	m_flag.set(BFlag::Frieze, frieze);
}

// ============================================================================
// CTrgBase::SetNoHit
/*!
	無接触設定
 */
// ============================================================================
template<typename TState>
void CTrgBase<TState>::SetNoHit(bool nohit)
{
	m_flag.set(BFlag::NoHit, nohit);
}

// ============================================================================
// CTrgBase::SetRepeatInterval
/*!
	リピート間隔の設定
 */
// ============================================================================
template<typename TState>
void CTrgBase<TState>::SetRepeatInterval(const TRepeatInterval &repeat_interval)
{
	for (typename TStateArray::iterator ite = m_state.begin(), ite_end = m_state.end(); ite != ite_end; ++ite) {
		ite->SetRepeatInterval(repeat_interval);
	}
}
template<typename TState>
void CTrgBase<TState>::SetRepeatInterval(typename TRepeatInterval::value_type first, typename TRepeatInterval::value_type second)
{
	for (typename TStateArray::iterator ite = m_state.begin(), ite_end = m_state.end(); ite != ite_end; ++ite) {
		ite->SetRepeatInterval(first, second);
	}
}
template<typename TState>
void CTrgBase<TState>::SetRepeatInterval()
{
	for (typename TStateArray::iterator ite = m_state.begin(), ite_end = m_state.end(); ite != ite_end; ++ite) {
		ite->SetRepeatInterval();
	}
}

// ============================================================================
// CTrgBase::SetDoubleClickTime
/*!
	ダブルクリック感知時間の設定
 */
// ============================================================================
template<typename TState>
void CTrgBase<TState>::SetDoubleClickTime(TDoubleClickTime wc_time)
{
	for (typename TStateArray::iterator ite = m_state.begin(), ite_end = m_state.end(); ite != ite_end; ++ite) {
		ite->SetDoubleClickTime(wc_time);
	}
}
template<typename TState>
void CTrgBase<TState>::SetDoubleClickTime()
{
	for (typename TStateArray::iterator ite = m_state.begin(), ite_end = m_state.end(); ite != ite_end; ++ite) {
		ite->SetDoubleClickTime();
	}
}

// ============================================================================
// CTrgBase::SetMoveThreshold
/*!
	移動感知閾値の設定
 */
// ============================================================================
template<typename TState>
void CTrgBase<TState>::SetMoveThreshold(TMoveThreshold move_threshold)
{
	for (typename TStateArray::iterator ite = m_state.begin(), ite_end = m_state.end(); ite != ite_end; ++ite) {
		ite->SetMoveThreshold(move_threshold);
	}
}
template<typename TState>
void CTrgBase<TState>::SetMoveThreshold()
{
	for (typename TStateArray::iterator ite = m_state.begin(), ite_end = m_state.end(); ite != ite_end; ++ite) {
		ite->SetMoveThreshold();
	}
}


//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// ============================================================================
// CTrgBase::getState
/*!
	状態取得

	@param	index	[in]	インデックス

	@note
		インデックスが省略された場合は、
		インデックスの若いロック中の値を返します。
		ロック中の値が無ければインデックスの若いオン中の値を返します。
		ロック中の値が無ければ0番目が返ります。
 */
// ============================================================================
template<typename TState>
typename CTrgBase<TState>::TState &CTrgBase<TState>::getState()
{
	const TState &result = static_cast<const CTrgBase *>(this)->getState();
	return const_cast<TState &>(result);
}
template<typename TState>
const typename CTrgBase<TState>::TState &CTrgBase<TState>::getState() const
{
	for (typename TStateArray::const_iterator ite = m_state.begin(), ite_end = m_state.end(); ite != ite_end; ++ite) {
		if ((*ite)[TStateArray::value_type::EState::Lock]) {
			return *ite;
		}
	}
	for (typename TStateArray::const_iterator ite = m_state.begin(), ite_end = m_state.end(); ite != ite_end; ++ite) {
		if ((*ite)[TStateArray::value_type::EState::On]) {
			return *ite;
		}
	}
	return m_state[0];
}
template<typename TState>
typename CTrgBase<TState>::TState &CTrgBase<TState>::getState(Uint32 index)
{
	const TState &result = static_cast<const CTrgBase *>(this)->getState(index);
	return const_cast<TState &>(result);
}
template<typename TState>
const typename CTrgBase<TState>::TState &CTrgBase<TState>::getState(Uint32 index) const
{
	amAssert(index < m_state.size());
	return m_state[index];
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
