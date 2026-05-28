// ============================================================================
/*!
	@file	erTrg.cpp
	@brief	タッチパネルアクション(トリガ)

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009-2010 Dimps
	$Id: erTrg.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include  <cmath>
#include  <numeric>
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
const CTrgState::TRepeatInterval CTrgState::c_repeat_interval_default = {{15, 6}};	//リピート標準値
const CTrgState::TDoubleClickTime CTrgState::c_wc_time_default = 6;					//ダブルクリック感知時間標準値
const CTrgState::TMoveThreshold CTrgState::c_move_threshold_default = 2;			//移動感知閾値標準値


//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// CTrgState::Push
/*!
	状態のプッシュ

	@param	is_on	[in]	ONか
	@param	is_edge	[in]	ONエッジ・もしくはOFFエッジか
	@param	move	[in]	移動量
 */
// ============================================================================
void CTrgState::Push(bool is_on, bool is_edge, const TMove &move)
{
	updateTime();
	updateOnPrev(is_on);
	updateEdge(is_edge);
	updateRepeat();
	updateLock();
	updateMoveOver(move);
	updateClick();
}

// ============================================================================
// CTrgState::operator[]
/*!
	インデクサ

	@param	kind	[in]	値の種類

	@return 値
 */
// ============================================================================
bool CTrgState::operator[](EState::Type kind) const
{
	return m_state[ETime::Direct][kind];
}

// ============================================================================
// CTrgState::AddLock
/*!
	ロックの付加
 */
// ============================================================================
void CTrgState::AddLock()
{
	m_state[ETime::Direct].set(EState::Lock, true);
}

// ============================================================================
// CTrgState::DelLock
/*!
	ロックの除去
 */
// ============================================================================
void CTrgState::DelLock()
{
	m_state[ETime::Direct].set(EState::Lock, false);
}

// ============================================================================
// CTrgState::ResetState
/*!
	状態の初期化
 */
// ============================================================================
void CTrgState::ResetState()
{
	for (TState::iterator state = m_state.begin(), state_end= m_state.end(); state != state_end; ++state) {
		state->reset();
	}
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
// ============================================================================
// CTrgState::GetMove
/*!
	ムーブ量の取得

	@return	前回のムーブ量
 */
// ============================================================================
CTrgState::TMove CTrgState::GetMove() const
{
	if (m_state[ETime::Direct][EState::Move]) {
		return GetLastMove();
	} else {
		return TMove::initializer();
	}
}

// ============================================================================
// CTrgState::GetOver
/*!
	オーバー量の取得

	@return	前回のオーバー量
 */
// ============================================================================
CTrgState::TMove CTrgState::GetOver() const
{
	if (m_state[ETime::Direct][EState::Over]) {
		return GetLastOver();
	} else {
		return TMove::initializer();
	}
}


//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
// ============================================================================
// CTrgState::SetRepeatInterval
/*!
	リピート間隔の設定
 */
// ============================================================================
void CTrgState::SetRepeatInterval(const TRepeatInterval &repeat_interval)
{
	m_repeat_interval = repeat_interval;
}
void CTrgState::SetRepeatInterval(TCounter first, TCounter second)
{
	m_repeat_interval = TRepeatInterval::initializer(first, second);
}

// ============================================================================
// CTrgState::SetDoubleClickTime
/*!
	ダブルクリック感知時間の設定
 */
// ============================================================================
void CTrgState::SetDoubleClickTime(TDoubleClickTime wc_time)
{
	m_wc_time = wc_time;
}

// ============================================================================
// CTrgState::SetMoveThreshold
/*!
	移動感知閾値の設定
 */
// ============================================================================
void CTrgState::SetMoveThreshold(TMoveThreshold move_threshold)
{
	m_move_threshold = move_threshold;
}


//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// ============================================================================
// CTrgState::CTrgState
/*!
	デフォルトコンストラクタ
 */
// ============================================================================
CTrgState::CTrgState() : m_state()
						, m_counter(c_counter_none)
						, m_repeat_interval(c_repeat_interval_default)
						, m_wc_time(c_wc_time_default)
						, m_move_threshold(c_move_threshold_default)
						, m_move_accumulate(TMove::initializer())
						, m_move_report(TMove::initializer())
{
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// ============================================================================
// CTrgState::updateTime
/*!
	時間の更新
 */
// ============================================================================
void CTrgState::updateTime()
{
	if (0 == m_counter) {
		m_counter = c_counter_none;
	} else if (0 < m_counter) {
		--m_counter;
	}
}

// ============================================================================
// CTrgState::updateOnPrev
/*!
	On・Prevの更新

	@param	is_on	[in]	ONか
 */
// ============================================================================
void CTrgState::updateOnPrev(bool is_on)
{
	//前回値保存
	m_state.push_front();

	//初期設定
	if (is_on) {
		m_state[ETime::Direct].set(BState::On, true);
	}
	
	//1f前設定
	if (m_state[ETime::Prev].test(BState::On)) {
		m_state[ETime::Direct].set(BState::Prev, true);
	}
	
	//状態の引継ぎ
	if (m_state[ETime::Prev].test(BState::Up)) {
		//前回ノーヒット直後なら
		typedef accel::CArray<BState::Type::size_type, 1>	TTakeover;
		static const TTakeover takeover = {{BState::DoubleClickWait}};
		for (TTakeover::size_type i = 0; i < TTakeover::static_size; ++i) {
			m_state[ETime::Direct].set(takeover[i], m_state[ETime::Prev].test(takeover[i]));
		}
	} else {
		//それ以外なら
		typedef accel::CArray<BState::Type::size_type, 4>	TTakeover;
		static const TTakeover takeover = {{BState::Lock, BState::DragAndDrop, BState::DoubleClickWait, BState::DoubleClickSecond}};
		for (TTakeover::size_type i = 0; i < TTakeover::static_size; ++i) {
			m_state[ETime::Direct].set(takeover[i], m_state[ETime::Prev].test(takeover[i]));
		}
	}
}

// ============================================================================
// CTrgState::updateEdge
/*!
	ONエッジ・OFFエッジの更新

	@param	is_edge	[in]	ONエッジ・もしくはOFFエッジか
 */
// ============================================================================
void CTrgState::updateEdge(bool is_edge)
{
	if (m_state[ETime::Direct].test(BState::On)) {
		//現在接触中
		if (m_state[ETime::Prev].test(BState::On)) {
			//接触継続
		} else {
			//接触した瞬間
			//スタンドアクション
			m_state[ETime::Direct].set(BState::Stand, true);
			//ダウン・インアクション
			BState::Type::size_type kind = ((is_edge)? BState::Down: BState::In);
			m_state[ETime::Direct].set(kind, true);
		}
	} else {
		//現在非接触
		if (m_state[ETime::Prev].test(BState::On)) {
			//非接触になった瞬間
			//リリースアクション
			m_state[ETime::Direct].set(BState::Release, true);
		//アップ・アウトアクション
			BState::Type::size_type kind = ((is_edge)? BState::Up: BState::Out);
			m_state[ETime::Direct].set(kind, true);
		} else {
			//非接触継続
			if (m_state[ETime::Direct].test(BState::Lock)) {
				//ロック中なら
				if (is_edge) {
					//プルなら
					m_state[ETime::Direct].set(BState::Up, true);
				}
			}
		}
	}
}

// ============================================================================
// CTrgState::updateRepeat
/*!
	リピートの更新
 */
// ============================================================================
void CTrgState::updateRepeat()
{
	if (m_state[ETime::Direct].test(BState::Stand)) {
		//プッシュなら
		//リピート(初回)
		m_state[ETime::Direct].set(BState::Repeat, true);
		m_counter = m_repeat_interval[ERepeatInterval::First];
	} else if (m_state[ETime::Direct].test(BState::Release)) {
		//プルなら
		//リピート終了
		m_counter = c_counter_none;
	} else if (m_state[ETime::Direct].test(BState::On)) {
		//オン継続なら
		//リピート(２回目以降)
		if (0 == m_counter) {
			//カウンタが 0 なら
			//リピート
			m_state[ETime::Direct].set(BState::Repeat, true);
			m_counter = m_repeat_interval[ERepeatInterval::Second];
		}
	}
}

// ============================================================================
// CTrgState::updateLock
/*!
	ロックの更新
 */
// ============================================================================
void CTrgState::updateLock()
{
	if (m_state[ETime::Direct].test(BState::Down)) {
		m_state[ETime::Direct].set(BState::Lock, true);
	}

	if (m_state[ETime::Prev].test(BState::Up)) {
		m_state[ETime::Direct].set(BState::Lock, false);
	}
}

// ============================================================================
// CTrgState::updateMoveOver
/*!
	ムーブ・オーバーの更新

	@param	move	[in]	移動量
 */
// ============================================================================
void CTrgState::updateMoveOver(const TMove &move)
{
	if (m_state[ETime::Direct].test(BState::Stand)) {
		//プッシュなら
		m_state[ETime::Direct].set(BState::Over, true);
		if (m_state[ETime::Direct].test(BState::Lock)) {
			//ロック中なら
			m_state[ETime::Direct].set(BState::Move, true);
		}
		if (!m_state[ETime::Prev].test(BState::Lock)) {
			resetMove();
		}
	} else if (m_state[ETime::Direct].test(BState::On)) {
		//オン(プッシュ除く)なら
		if (addMove(move)) {
			//移動したなら
			m_state[ETime::Direct].set(BState::Over, true);
			if (m_state[ETime::Direct].test(BState::Lock)) {
				//ダウンなら
				m_state[ETime::Direct].set(BState::Move, true);
				m_state[ETime::Direct].set(BState::DragAndDrop, true);
			}
		}
	} else if (m_state[ETime::Direct].test(BState::Lock)) {
		//オフ かつ ロック中なら
		if (addMove(move)) {
			//移動したなら
			m_state[ETime::Direct].set(BState::Move, true);
			m_state[ETime::Direct].set(BState::DragAndDrop, true);
		}
	}
	if (m_state[ETime::Prev].test(BState::Up)) {
		//1f前がアウトなら
		m_state[ETime::Direct].set(BState::DragAndDrop, false);
	}
}

// ============================================================================
// CTrgState::updateClick
/*!
	クリック・シングルクリック・ダブルクリックの更新
 */
// ============================================================================
void CTrgState::updateClick()
{
	if (m_state[ETime::Direct].test(BState::Down)) {
		//ダウンなら
		if (m_state[ETime::Direct].test(BState::DoubleClickWait)) {
			//ダブルクリック待ち中なら
			m_state[ETime::Direct].set(BState::DoubleClick, true);
			m_state[ETime::Direct].set(BState::DoubleClickSecond, true);
			m_state[ETime::Direct].set(BState::DoubleClickWait, false);
			m_counter = c_counter_none;
		}
	} else if (m_state[ETime::Direct].test(BState::Up)) {
		//アップなら
		if (m_state[ETime::Direct].test(BState::Lock)) {
			//ロック中なら
			if (m_state[ETime::Direct].test(BState::Prev)) {
				//領域内なら
				if (!m_state[ETime::Direct].test(BState::DoubleClickSecond)) {
					//ダブルクリック２回目接触中じゃないなら
					if (0 < m_wc_time) {
						//ダブルクリック有りなら
						m_state[ETime::Direct].set(BState::Click, true);
						m_state[ETime::Direct].set(BState::DoubleClickWait, true);
						m_counter = m_wc_time;
					} else {
						//ダブルクリック無しなら
						m_state[ETime::Direct].set(BState::Click, true);
						m_state[ETime::Direct].set(BState::SingleClick, true);
					}
				}
			}
		}
	} else if (!m_state[ETime::Direct].test(BState::On)) {
		//オフ継続なら
		if (m_state[ETime::Direct].test(BState::DoubleClickWait)) {
			//ダブルクリック待ち中なら
			if (0 == m_counter) {
				//カウンタが 0 なら
				m_state[ETime::Direct].set(BState::SingleClick, true);
				m_state[ETime::Direct].set(BState::DoubleClickWait, false);
			}
		}
		//1f前がアップなら
		if (m_state[ETime::Prev].test(BState::Up)) {
			m_state[ETime::Direct].set(BState::DoubleClickSecond, false);
		}
	}
}

// ============================================================================
// CTrgState::resetMove
/*!
	移動量の初期化
 */
// ============================================================================
void CTrgState::resetMove()
{
	m_move_report = m_move_accumulate = TMove::initializer();
}

// ============================================================================
// CTrgState::addMove
/*!
	移動量の加算

	@param	move	[in]	移動量

	@retval	true	移動した
	@retval	false	移動しなかった
 */
// ============================================================================
bool CTrgState::addMove(const TMove &move)
{
	bool is_move = false;
	m_move_accumulate += move;
	for (TMove::size_type i = 0; i < TMove::static_size; ++i) {
		if (m_move_threshold < std::abs(m_move_accumulate[i])) {
			is_move = true;
			break;
		}
	}
	if (is_move) {
		m_move_report = m_move_accumulate;
		m_move_accumulate = TMove::initializer();
	}
	return is_move;
}

//------------------------------------------------------------------------------**********


































































//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// CTrgStateEx::Push
/*!
	状態のプッシュ

	@param	is_on	[in]	ONか
	@param	is_edge	[in]	ONエッジ・もしくはOFFエッジか
	@param	move	[in]	移動量
 */
// ============================================================================
void CTrgStateEx::Push(bool is_on, bool is_edge, const TMove &move)
{
	TSuperClass::Push(is_on, is_edge, move);
	if (is_on || is_edge) {
		//ONもしくはON前後
		m_pos_history.push_front(move);
	} else {
		//完全にOFF(前回値を継続)
		//完全にOFFの時にmoveの値がおかしい?(20100420-1600)
		m_pos_history.push_front(TMove::initializer());
	}
}

// ============================================================================
// CTrgStateEx::ResetState
/*!
	状態の初期化
 */
// ============================================================================
void CTrgStateEx::ResetState()
{
	TSuperClass::ResetState();
	m_pos_history.clear();
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
// ============================================================================
// CTrgStateEx::GetDragSpeed
/*!
	ドラッグ速度の取得
 */
// ============================================================================
CTrgStateEx::TDragSpeed CTrgStateEx::GetDragSpeed() const
{
	TMove sum = std::accumulate(m_pos_history.begin(), m_pos_history.end(), TMove::initializer());
	const TDragSpeed::value_type c_coefficient = TDragSpeed::value_type(1) / TDragSpeed::value_type(m_pos_history.static_size);
	TDragSpeed result;
	result.x() = static_cast<TDragSpeed::value_type>(sum.x()) * c_coefficient;
	result.y() = static_cast<TDragSpeed::value_type>(sum.y()) * c_coefficient;

	return result;
}


//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// ============================================================================
// CTrgState::CTrgState
/*!
	デフォルトコンストラクタ
 */
// ============================================================================
CTrgStateEx::CTrgStateEx() : TSuperClass(), m_pos_history()
{
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
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
