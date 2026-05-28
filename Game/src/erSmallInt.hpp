// ============================================================================
/*!
	@file	erSmallInt.hpp
	@brief	狭域整数

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: erSmallInt.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page erSmallIntMain 狭域整数

	@section erSmallIntSummary 概要
		狭域整数を提供します。
 */

#pragma once
#if	defined(__cplusplus)
extern "C" {
#endif

//------ C Include Files -------------- インクルード ---------------------------******CIF*
//------ C Macro ---------------------- マクロ ---------------------------------******CMC*
//------ C External Definitions ------- グローバル変数及び関数の宣言 -----------******CED*


#if	defined(__cplusplus)
} // extern "C"
#endif

#if	defined(__cplusplus)
//------ Include ---------------------- インクルード ---------------------------******_IC*
//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace er {









//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	狭域整数クラス
		狭域整数を提供します。
 */
template <typename TType = int>
class CSmallInt {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	typedef TType	TValue;

	//自動static_castクラス
	class CAutoStaticCast {
	private:
		int m_val;
	public:
		explicit CAutoStaticCast(int val) : m_val(val) {}
		template <typename T>
		operator T() {
			return T(m_val);
		}
		template <typename T>
		operator T*() {
			return static_cast<T *>(&m_val);
		}
	};


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CSmallInt::operator TValue
	/*!
		キャスト
	 */
	// ============================================================================
	operator TValue() const {
		return m_val;
	}

	// ============================================================================
	// CSmallInt::operator+
	/*!
		正符号演算子
	 */
	// ============================================================================
	CSmallInt operator+() {
		CSmallInt result = *this;
		return result;
	}

	// ============================================================================
	// CSmallInt::operator-
	/*!
		負符号演算子
	 */
	// ============================================================================
	CSmallInt operator-() {
		CSmallInt result = *this;
		result = -result.m_val;
		return result;
	}

	// ============================================================================
	// CSmallInt::++operator
	/*!
		前置インクリメント
	 */
	// ============================================================================
	CSmallInt &operator++() {
		if (m_val < m_max - TValue(1)) {
			++m_val;
		} else {
			m_val -= m_max - TValue(1);
		}
		return *this;
	}

	// ============================================================================
	// CSmallInt::operator++
	/*!
		後置インクリメント
	 */
	// ============================================================================
	CSmallInt operator++(int) {
		CSmallInt result = *this;
		++*this;
		return result;
	}

	// ============================================================================
	// CSmallInt::--operator
	/*!
		前置デクリメント
	 */
	// ============================================================================
	CSmallInt &operator--() {
		if (m_min + TValue(1) <= m_val) {
			--m_val;
		} else {
			m_val = m_max - (TValue(1) - m_val);
		}
		return *this;
	}

	// ============================================================================
	// CSmallInt::operator--
	/*!
		後置デクリメント
	 */
	// ============================================================================
	CSmallInt operator--(int) {
		CSmallInt result = *this;
		--*this;
		return result;
	}

	// ============================================================================
	// CSmallInt::operator=
	/*!
		代入演算子
	 */
	// ============================================================================
	CSmallInt &operator=(TValue rhs) {
		m_val = clip(rhs, m_min, m_max);
		return *this;
	}

	// ============================================================================
	// CSmallInt::operator+=
	/*!
		代入加算演算子
	 */
	// ============================================================================
	CSmallInt &operator+=(TValue rhs) {
		rhs = clip(rhs, TValue(0), GetRange());
		if (m_val < m_max - rhs) {
			m_val += rhs;
		} else {
			m_val -= m_max - rhs;
		}
		return *this;
	}

	// ============================================================================
	// CSmallInt::operator-=
	/*!
		代入減算演算子
	 */
	// ============================================================================
	CSmallInt &operator-=(TValue rhs) {
		rhs = clip(rhs, TValue(0), GetRange());
		if (m_min + rhs <= m_val) {
			m_val -= rhs;
		} else {
			m_val = m_max - (rhs - m_val);
		}
		return *this;
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CSmallInt::GetVal
	/*!
		値の取得

		@return 値
	 */
	// ============================================================================
	CAutoStaticCast GetVal() {
		return CAutoStaticCast(m_val);
	}
	template <typename T>
	T GetVal() {
		return T(m_val);
	}
	

	// ============================================================================
	// CSmallInt::GetMin
	/*!
		最小の取得

		@return 値
	 */
	// ============================================================================
	TValue GetMin() {
		return m_min;
	}

	// ============================================================================
	// CSmallInt::IsMin
	/*!
		最小確認

		@retval	TRUE	最小値
		@retval	TRUE	最小値ではない
	 */
	// ============================================================================
	BOOL IsMin() {
		return ((m_val == m_min)? TRUE: FALSE);
	}

	// ============================================================================
	// CSmallInt::GetMax
	/*!
		最大の取得

		@return 値
	 */
	// ============================================================================
	TValue GetMax() {
		return m_max;
	}

	// ============================================================================
	// CSmallInt::IsMax
	/*!
		最大確認

		@retval	TRUE	最大値
		@retval	TRUE	最大値ではない
	 */
	// ============================================================================
	BOOL IsMax() {
		return ((m_val == (m_max - TValue(1)))? TRUE: FALSE);
	}

	// ============================================================================
	// CSmallInt::GetRange
	/*!
		最小から最大までの距離の取得

		@return 値
	 */
	// ============================================================================
	TValue GetRange() {
		return m_max - m_min;
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// CSmallInt::SetRange
	/*!
		範囲制限の再設定

		@param	min	[in]	最小値(含む)
		@param	max	[in]	最大値(含まず)
		@param	val	[in]	初期値
	 */
	// ============================================================================
	void SetRange(TValue min, TValue max) {
		m_min = min;
		m_max = max;
		m_val = min;
	}
	void SetRange(TValue min, TValue max, TValue val) {
		m_min = min;
		m_max = max;
		m_val = clip(val, m_min, m_max);
	}


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CSmallInt::CSmallInt
	/*!
		デフォルトコンストラクタ

		@param	min	[in]	最小値(含む)
		@param	max	[in]	最大値(含まず)
		@param	val	[in]	初期値
	 */
	// ============================================================================
public:
	CSmallInt(TValue min = TValue(0), TValue max = TValue(0)) {
		SetRange(min, max);
	}
	CSmallInt(TValue min, TValue max, TValue val) {
		SetRange(min, max, val);
	}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	TValue	m_min;	//<最小値(含む)
	TValue	m_max;	//<最大値(含まず)
	TValue	m_val;	//<値



	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	// ============================================================================
	// CSmallInt::clip
	/*!
		クリップ

		@param	val	[in]	値
		@param	min	[in]	最小値(含む)
		@param	max	[in]	最大値(含まず)
	 */
	// ============================================================================
	static TValue clip(TValue val, TValue min, TValue max){
		TValue result = val;
		if ((result < min) || (max <= result)) {
			TValue range = max - min;
			while (max <= result) {
				result -= range;
			}
			while (result < min) {
				result += range;
			}
		}
		return result;
	}


//------------------------------------------------------------------------------**********
}; //class CSmallInt




















} //namespace er
#endif //#if	defined(__cplusplus)

	// ============================================================================
	// CSmallInt::Function
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
	// ============================================================================
