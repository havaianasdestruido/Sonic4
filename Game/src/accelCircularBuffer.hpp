// ============================================================================
/*!
	@file	accelCircularBuffer.hpp
	@brief	循環バッファクラス

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: accelCircularBuffer.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page AccelCircularBuffer 循環バッファクラス

	@section AccelCircularBufferSummary 概要
		C++言語にてスタックヒープベースのboost::circular_bufferクラスを扱う処理を提供します。
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
//------ define ----------------------- デファイン -----------------------------******_DE*
//オペレーターオーバーロードを無効化するならこのヘッダの前に、下記を定義して下さい
//	ACLD_DISABLE_OPERATOR_OVERLOAD
//


//------ Include ---------------------- インクルード ---------------------------******_IC*
#include <iterator>
#include "accelArray.hpp"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*


namespace accel {
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 循環バッファクラス
		スタックヒープベースのboost::circular_bufferクラスです。
 */
template<typename TType, std::size_t NSize>
class CCircularBuffer {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef CArray<TType, NSize>	data_type;

public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	enum {static_size = data_type::static_size};
	typedef typename data_type::value_type				value_type;
	typedef typename data_type::size_type				size_type;
	typedef CCircularBuffer<value_type, static_size>	type;
	template<typename TReqType, std::size_t TReqNum>
	struct gene {
		typedef CCircularBuffer<TReqType, TReqNum>		type;
	};

	typedef typename data_type::pointer					pointer;
	typedef typename data_type::const_pointer			const_pointer;
	typedef typename data_type::reference				reference;
	typedef typename data_type::const_reference			const_reference;

	class CIterator;
	class CConstIterator;
	typedef CIterator									iterator;
	typedef CConstIterator								const_iterator;
	typedef std::reverse_iterator<iterator>				reverse_iterator;
	typedef std::reverse_iterator<const_iterator>		const_reverse_iterator;
	

	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CCircularBuffer::Indexer
	/*!
		インデクサアクセス
	 */
	// ============================================================================
	const_reference	operator[](size_type i) const		{return at(i);}
	reference		operator[](size_type i)				{return at(i);}
	template <typename TIndex>
	const_reference	operator[](TIndex i) const			{return operator[](static_cast<size_type>(i));}
	template <typename TIndex>
	reference		operator[](TIndex i)				{return operator[](static_cast<size_type>(i));}

	// ============================================================================
	// CCircularBuffer::Terminal
	/*!
		ターミネータアクセス
	 */
	// ============================================================================
	const_reference			front() const	{return at(0);}
	reference				front()			{return at(0);}
	const_reference			back() const	{return at(m_size - 1);}
	reference				back()			{return at(m_size - 1);}

	// ============================================================================
	// CCircularBuffer::Iterator
	/*!
		イテレータアクセス
	 */
	// ============================================================================
	const_iterator			begin() const	{return const_iterator(*this, 0);}
	iterator				begin()			{return iterator(*this, 0);}
	const_iterator			end() const		{return const_iterator(*this, m_size);}
	iterator				end()			{return iterator(*this, m_size);}
	const_reverse_iterator	rbegin() const	{return const_reverse_iterator(end());}
	reverse_iterator		rbegin()		{return reverse_iterator(end());}
	const_reverse_iterator	rend() const	{return const_reverse_iterator(begin());}
	reverse_iterator		rend()			{return reverse_iterator(begin());}

	// ============================================================================
	// CCircularBuffer::PushPop
	/*!
		プッシュ・ポップ
	 */
	// ============================================================================
	void push_back(const_reference value = value_type()) {
		if (m_size < static_size) {
			++m_size;
		} else {
			++m_begin;
			if (static_size <= m_begin) {
				m_begin = 0;
			}
		}
		back() = value;
	}
	void push_front(const_reference value = value_type()) {
		if (m_size < static_size) {
			++m_size;
		}
		if (0 < m_begin) {
			--m_begin;
		} else {
			m_begin = static_size - 1;
		}
		front() = value;
	}
	void pop_back() {
		if (0 < m_size) {
			--m_size;
		}
	}
	void pop_front() {
		if (0 < m_size) {
			--m_size;
			++m_begin;
			if (static_size <= m_begin) {
				m_begin = 0;
			}
		}
	}

	// ============================================================================
	// CCircularBuffer::size
	/*!
		サイズアクセス
	
		@return サイズ
	 */
	// ============================================================================
	size_type	size() const 		{return m_size;}
	size_type	max_size() const 	{return static_size;}

	// ============================================================================
	// CCircularBuffer::clear
	/*!
		クリア
	 */
	// ============================================================================
	void clear() {
		m_size = 0;
	}

	// ============================================================================
	// CCircularBuffer::empty
	/*!
		空か
	
		@retval	true	空
		@retval	false	空ではない
	 */
	// ============================================================================
	bool empty() {
		return ((0 == m_size)? true: false);
	}

	// ============================================================================
	// CCircularBuffer::full
	/*!
		満杯か
	
		@retval	true	満杯
		@retval	false	満杯ではない
	 */
	// ============================================================================
	bool full() {
		return ((static_size == m_size)? true: false);
	}

#if 0
	// ============================================================================
	// CCircularBuffer::insert
	/*!
		挿入
	
		@param	pos	[in]	挿入位置の手前のイテレータ
	
		@return	成功すればその要素へのイテレータ、成功しなければ末端イテレータ
	 */
	// ============================================================================
	iterator insert(iterator pos, const_reference item = value_type()) {
	}

	// ============================================================================
	// CCircularBuffer::rinsert
	/*!
		逆挿入
	
		@param	pos	[in]	挿入位置の直後のイテレータ
	
		@return	成功すればその要素へのイテレータ、成功しなければ末端イテレータ
	 */
	// ============================================================================
	iterator rinsert(iterator pos, const_reference item = value_type()) {
	}

	// ============================================================================
	// CCircularBuffer::erase
	/*!
		削除
	
		@param	pos	[in]	削除位置
	
		@return	削除した次の要素
	 */
	// ============================================================================
	iterator erase(iterator pos) {
	}
	iterator erase(const_iterator pos) {
		erase(const_cast<iterator>(pos));
	}
	
	// ============================================================================
	// CCircularBuffer::rerase
	/*!
		逆削除
	
		@param	pos	[in]	削除位置
	
		@return	削除した次の要素
	 */
	// ============================================================================
	iterator rerase(iterator pos) {
	}
	iterator rerase(const_iterator pos) {
		return rerase(const_cast<iterator>(pos));
	}
#endif

	// ============================================================================
	// CCircularBuffer::at
	/*!
		検索
	
		@param	key	[in]	検索キー
	
		@return	一致したらその要素へのイテレータ、一致が無ければ末端イテレータ
	 */
	// ============================================================================
	const_reference at(size_type index) const {
		if (static_size <= index) {
			index %= static_size;
		}
		index += m_begin;
		if (static_size <= index) {
			index -= static_size;
		}
		return m_data[index];
	}
	reference at(size_type index) {
		return const_cast<reference>(const_cast<const CCircularBuffer *>(this)->at(index));
	}

	
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- CircularBufferer Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CCircularBuffer::CCircularBuffer
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CCircularBuffer() {
		m_begin = 0;
		m_size = 0;
	}

	
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	data_type	m_data;		//<データ領域
	size_type	m_begin;	//<開始位置
	size_type	m_size;		//<格納サイズ

	
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	//-- Public Class ----------------- 公開クラス -----------------------------******PCL*
public:
	//constイテレータ
	class CConstIterator : public std::iterator<std::random_access_iterator_tag, const value_type> {
	private:
		const CCircularBuffer	*m_src;
		size_type				m_index;

		typedef std::iterator<std::random_access_iterator_tag, const value_type>	super_type;
	public:
		typedef typename super_type::difference_type	difference_type;
		typedef typename super_type::pointer			pointer;
		typedef typename super_type::reference			reference;

		CConstIterator() : m_src(NULL) {
			m_index = 0;
		}
		CConstIterator(const CCircularBuffer &src, size_type index) : m_src(&src) {
			m_index = index;
		}

		//ポインタ剥がし
		reference operator*() {
			return (*m_src)[m_index];
		}
		//アロー演算子
		pointer operator->() {
			return &(*m_src)[m_index];
		}

		//インデクサ
		reference operator[](size_type index) {
			if (index <= CCircularBuffer::static_size) {
				index %= CCircularBuffer::static_size;
			}
			index += m_index;
			if (index <= CCircularBuffer::static_size) {
				index -= CCircularBuffer::static_size;
			}
			return (*m_src)[index];
		}
		template <typename TIndex>
		reference operator[](TIndex index) {
			return operator[](static_cast<size_type>(index));
		}

		//演算子
		CConstIterator &operator++() {
			if (m_index < CCircularBuffer::static_size) {
				++m_index;
			} else {
				m_index = 0;
			}
			return *this;
		}
		CConstIterator operator++(int) {
			CConstIterator result(*this);
			return ++result;
		}
		CConstIterator &operator+=(difference_type distance) {
			if (distance <= CCircularBuffer::static_size) {
				distance %= CCircularBuffer::static_size;
			}
			m_index += distance;
			if (m_index <= CCircularBuffer::static_size) {
				m_index -= CCircularBuffer::static_size;
			}
			return *this;
		}
		CConstIterator &operator--() {
			if (0 < m_index) {
				--m_index;
			} else {
				m_index = CCircularBuffer::static_size;
			}
			return *this;
		}
		CConstIterator operator--(int) {
			CConstIterator result(*this);
			--result;
			return result;
		}
		CConstIterator &operator-=(difference_type distance) {
			if (distance <= CCircularBuffer::static_size) {
				distance %= CCircularBuffer::static_size;
			}
			m_index += distance;
			if (m_index <= CCircularBuffer::static_size) {
				m_index -= CCircularBuffer::static_size;
			}
			return *this;
		}
		CConstIterator operator-(difference_type distance) {
			CConstIterator result(*this);
			result -= distance;
			return result;
		}
		difference_type operator-(const CConstIterator &rhs) {
			return &(*this)[m_index] - &rhs[m_index];
		}

		//比較
		bool operator==(const CConstIterator &rhs) {
			return ((m_src == rhs.m_src) && (m_index == rhs.m_index));
		}
		bool operator!=(const CConstIterator &rhs) {
			return !(*this == rhs);
		}
		bool operator<(const CConstIterator &rhs) {
			return &(*this)[m_index] < &rhs[m_index];
		}
		bool operator<=(const CConstIterator &rhs) {
			return &(*this)[m_index] <= &rhs[m_index];
		}
		bool operator>(const CConstIterator &rhs) {
			return &(*this)[m_index] > &rhs[m_index];
		}
		bool operator>=(const CConstIterator &rhs) {
			return &(*this)[m_index] >= &rhs[m_index];
		}
	};


	//イテレータ
	class CIterator : public std::iterator<std::random_access_iterator_tag, value_type> {
	private:
		CConstIterator	m_delegate;

		typedef std::iterator<std::random_access_iterator_tag, value_type>	super_type;
		typedef CConstIterator												delegate_type;
	public:
		typedef typename super_type::difference_type	difference_type;
		typedef typename super_type::pointer			pointer;
		typedef typename super_type::reference			reference;

		CIterator() : m_delegate() {}
		CIterator(CCircularBuffer &src, size_type index) : m_delegate(src, index) {}

		//キャスト
		operator delegate_type() {
			return m_delegate;
		}

		//ポインタ剥がし
		reference operator*() {
			return const_cast<reference>(*m_delegate);
		}
		//アロー演算子
		pointer operator->() {
			return &operator*();
		}

		//インデクサ
		reference operator[](size_type index) {
			return const_cast<reference>(m_delegate[index]);
		}
		template <typename TIndex>
		reference operator[](TIndex index) {
			return operator[](static_cast<size_type>(index));
		}

		//演算子
		CIterator &operator++() {
			++m_delegate;
			return *this;
		}
		CIterator operator++(int) {
			CIterator result(*this);
			return ++result;
		}
		CIterator &operator+=(difference_type distance) {
			m_delegate += distance;
			return *this;
		}
		CIterator &operator--() {
			--m_delegate;
			return *this;
		}
		CIterator operator--(int) {
			CIterator result(*this);
			--result;
			return result;
		}
		CIterator &operator-=(difference_type distance) {
			m_delegate -= distance;
			return *this;
		}
		CIterator operator-(difference_type distance) {
			CIterator result(*this);
			result -= distance;
			return result;
		}
		difference_type operator-(const CIterator &rhs) {
			return (m_delegate - rhs.m_delegate);
		}

		//比較
		bool operator==(const CIterator &rhs) {
			return (m_delegate == rhs.m_delegate);
		}
		bool operator!=(const CIterator &rhs) {
			return (m_delegate != rhs.m_delegate);
		}
		bool operator<(const CIterator &rhs) {
			return (m_delegate < rhs.m_delegate);
		}
		bool operator<=(const CIterator &rhs) {
			return (m_delegate <= rhs.m_delegate);
		}
		bool operator>(const CIterator &rhs) {
			return (m_delegate > rhs.m_delegate);
		}
		bool operator>=(const CIterator &rhs) {
			return (m_delegate >= rhs.m_delegate);
		}
	};


//------------------------------------------------------------------------------**********
}; //class CCircularBuffer<typename TType, std::size_t NSize>





















































#if !defined(ACLD_DISABLE_OPERATOR_OVERLOAD)
// ============================================================================
// CCircularBuffer::operator
/*!
	単項演算子
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> operator+(const CCircularBuffer<TType, NSize> &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	return rhs;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> operator-(const CCircularBuffer<TType, NSize> &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	current_type result;
	for (typename current_type::const_iterator ite = rhs.begin(), ite_end = rhs.end(); ite != ite_end; ++ite) {
		result.push_back(typename current_type::value_type(-*ite));
	}
	return result;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> operator~(const CCircularBuffer<TType, NSize> &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	current_type result;
	for (typename current_type::const_iterator ite = rhs.begin(), ite_end = rhs.end(); ite != ite_end; ++ite) {
		result.push_back(typename current_type::value_type(~*ite));
	}
	return result;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> operator!(const CCircularBuffer<TType, NSize> &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	current_type result;
	for (typename current_type::const_iterator ite = rhs.begin(), ite_end = rhs.end(); ite != ite_end; ++ite) {
		result.push_back(typename current_type::value_type(!*ite));
	}
	return result;
}

// ============================================================================
// CCircularBuffer::operator
/*!
	前置インクリメント演算子・前置デクリメント演算子
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> &operator++(CCircularBuffer<TType, NSize> &lhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	for (typename current_type::iterator ite = lhs.begin(), ite_end = lhs.end(); ite != ite_end; ++ite) {
		++(*ite);
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> &operator--(CCircularBuffer<TType, NSize> &lhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	for (typename current_type::iterator ite = lhs.begin(), ite_end = lhs.end(); ite != ite_end; ++ite) {
		--(*ite);
	}
	return lhs;
}

// ============================================================================
// CCircularBuffer::operator
/*!
	後置インクリメント演算子・後置デクリメント演算子
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> operator++(CCircularBuffer<TType, NSize> &lhs, int) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	current_type result(lhs);
	++lhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> operator--(CCircularBuffer<TType, NSize> &lhs, int) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	current_type result(lhs);
	--lhs;
	return result;
}

// ============================================================================
// CCircularBuffer::operator
/*!
	代入演算子(スカラ)

	@note
		代入演算子(ベクトル)はコンパイラ定義を使用
 */
// ============================================================================

// ============================================================================
// CCircularBuffer::operator
/*!
	算術代入演算子(ベクトル)
 */
// ============================================================================

// ============================================================================
// CCircularBuffer::operator
/*!
	算術代入演算子(スカラ)
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> &operator+=(CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	for (typename current_type::iterator ite = lhs.begin(), ite_end = lhs.end(); ite != ite_end; ++ite) {
		*ite += rhs;
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> &operator-=(CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	for (typename current_type::iterator ite = lhs.begin(), ite_end = lhs.end(); ite != ite_end; ++ite) {
		*ite -= rhs;
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> &operator*=(CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	for (typename current_type::iterator ite = lhs.begin(), ite_end = lhs.end(); ite != ite_end; ++ite) {
		*ite *= rhs;
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> &operator/=(CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	for (typename current_type::iterator ite = lhs.begin(), ite_end = lhs.end(); ite != ite_end; ++ite) {
		*ite /= rhs;
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> &operator%=(CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	for (typename current_type::iterator ite = lhs.begin(), ite_end = lhs.end(); ite != ite_end; ++ite) {
		*ite %= rhs;
	}
	return lhs;
}


// ============================================================================
// CCircularBuffer::operator
/*!
	論述代入演算子(ベクトル)
 */
// ============================================================================


// ============================================================================
// CCircularBuffer::operator
/*!
	論述代入演算子(スカラ)
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> &operator&=(CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	for (typename current_type::iterator ite = lhs.begin(), ite_end = lhs.end(); ite != ite_end; ++ite) {
		*ite &= rhs;
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> &operator|=(CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	for (typename current_type::iterator ite = lhs.begin(), ite_end = lhs.end(); ite != ite_end; ++ite) {
		*ite |= rhs;
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> &operator^=(CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	for (typename current_type::iterator ite = lhs.begin(), ite_end = lhs.end(); ite != ite_end; ++ite) {
		*ite ^= rhs;
	}
	return lhs;
}


// ============================================================================
// CCircularBuffer::operator
/*!
	算術演算子(ベクトル)
 */
// ============================================================================


// ============================================================================
// CCircularBuffer::operator
/*!
	算術演算子(スカラ)
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> operator+(const CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	current_type result(lhs);
	result += rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> operator-(const CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	current_type result(lhs);
	result -= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> operator*(const CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	current_type result(lhs);
	result *= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> operator/(const CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	current_type result(lhs);
	result /= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> operator%(const CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	current_type result(lhs);
	result %= rhs;
	return result;
}


// ============================================================================
// CCircularBuffer::operator
/*!
	論述演算子(ベクトル)
 */
// ============================================================================


// ============================================================================
// CCircularBuffer::operator
/*!
	論述演算子(スカラ)
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> operator&(const CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	current_type result(lhs);
	result &= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> operator|(const CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	current_type result(lhs);
	result |= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CCircularBuffer<TType, NSize> operator^(const CCircularBuffer<TType, NSize> &lhs, const TType &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	current_type result(lhs);
	result ^= rhs;
	return result;
}

// ============================================================================
// CCircularBuffer::operator
/*!
	比較演算子(ベクトル)

	@note
		“<,<=,>,>=”については未定義です。
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline bool operator==(const CCircularBuffer<TType, NSize> &lhs, const CCircularBuffer<TType, NSize> &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	bool result = true;
	typename current_type::size_type max = lhs.size();
	if (max != rhs.size()) {
		result = false;
	} else for (typename current_type::size_type i = 0; i < max; ++i) {
		if (lhs[i] != rhs[i]) {
			result = false;
			break;
		}
	}
	return result;
}

template<typename TType, std::size_t NSize>
inline bool operator!=(const CCircularBuffer<TType, NSize> &lhs, const CCircularBuffer<TType, NSize> &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	bool result = false;
	typename current_type::size_type max = lhs.size();
	if (max != rhs.size()) {
		result = true;
	} else for (typename current_type::size_type i = 0; i < max; ++i) {
		if (lhs[i] != rhs[i]) {
			result = true;
			break;
		}
	}
	return result;
}

// ============================================================================
// CCircularBuffer::operator
/*!
	入出力演算子
 */
// ============================================================================
template<typename TType, std::size_t NSize, typename TStreamType>
inline TStreamType &operator>>(TStreamType &lhs, CCircularBuffer<TType, NSize> &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	for (typename current_type::iterator ite = rhs.begin(), ite_end = rhs.end(); ite != ite_end; ++ite) {
		lhs >> *ite;
	}
	return lhs;
}
template<typename TType, std::size_t NSize, typename TStreamType>
inline TStreamType &operator>>(TStreamType &lhs, const CCircularBuffer<TType, NSize> &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	for (typename current_type::const_iterator ite = rhs.begin(), ite_end = rhs.end(); ite != ite_end; ++ite) {
		lhs >> *ite;
	}
	return lhs;
}

template<typename TType, std::size_t NSize, typename TStreamType>
inline TStreamType &operator<<(TStreamType &lhs, CCircularBuffer<TType, NSize> &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	for (typename current_type::const_iterator ite = rhs.begin(), ite_end = rhs.end(); ite != ite_end; ++ite) {
		lhs << *ite;
	}
	return lhs;
}
template<typename TType, std::size_t NSize, typename TStreamType>
inline TStreamType &operator<<(TStreamType &lhs, const CCircularBuffer<TType, NSize> &rhs) {
	typedef CCircularBuffer<TType, NSize>	current_type;
	for (typename current_type::const_iterator ite = rhs.begin(), ite_end = rhs.end(); ite != ite_end; ++ite) {
		lhs << *ite;
	}
	return lhs;
}



#endif //#if !defined(ACLD_DISABLE_OPERATOR_OVERLOAD)

//------------------------------------------------------------------------------**********




















//------------------------------------------------------------------------------**********









} //namespace accel
#endif //#if	defined(__cplusplus)

// =============================================================================
// accel::CCircularBuffer::Function
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
