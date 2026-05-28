// ============================================================================
/*!
	@file	accelBitset.hpp
	@brief	ビット配列クラス

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: accelBitset.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page AccelBitset ビット配列クラス

	@section AccelBitsetSummary 概要
		C++言語にてスタックヒープベースのstd::bitsetクラスを扱う処理を提供します。
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
#include <cstddef>
#include <bitset>
#include <iterator>


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*


namespace accel {
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 BITSETクラス
		独自拡張を施したstd::bitsetクラスです。
 */
template<std::size_t NSize>
class CBitset {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef std::bitset<NSize>	delegate_type;

public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	enum {static_size = NSize};
	typedef bool									value_type;
	typedef unsigned long							block_type;
	typedef std::size_t								size_type;
	typedef CBitset<static_size>					type;
	template<std::size_t TReqNum>
	struct gene {
		typedef CBitset<TReqNum>					type;
	};

	typedef value_type								*pointer;
	typedef const value_type						*const_pointer;
	typedef typename delegate_type::reference		reference;
	typedef value_type								const_reference;

	class CIterator;
	class CConstIterator;
	typedef CIterator								iterator;
	typedef CConstIterator							const_iterator;
	typedef std::reverse_iterator<iterator>			reverse_iterator;
	typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	static const size_type npos = size_type(-1);


	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CBitset::Indexer
	/*!
		インデクサアクセス
	 */
	// ============================================================================
	const_reference	operator[](size_type i) const		{return at(i);}
	reference		operator[](size_type i)				{return at(i);}
#if !defined(_IPHONE)
	template <typename TIndex>
	const_reference	operator[](TIndex i) const			{return operator[](static_cast<size_type>(i));}
	template <typename TIndex>
	reference		operator[](TIndex i)				{return operator[](static_cast<size_type>(i));}
#endif

	// ============================================================================
	// CBitset::Iterator
	/*!
		イテレータアクセス
	 */
	// ============================================================================
	const_iterator			begin() const	{return const_iterator(*this, 0);}
	iterator				begin()			{return iterator(*this, 0);}
	const_iterator			end() const		{return const_iterator(*this, static_size);}
	iterator				end()			{return iterator(*this, static_size);}
	const_reverse_iterator	rbegin() const	{return const_reverse_iterator(end());}
	reverse_iterator		rbegin()		{return reverse_iterator(end());}
	const_reverse_iterator	rend() const	{return const_reverse_iterator(begin());}
	reverse_iterator		rend()			{return reverse_iterator(begin());}

	// ============================================================================
	// CBitset::operator
	/*!
		単項演算子
	 */
	// ============================================================================
	CBitset operator~() const {
		return CBitset(~m_delegate);
	}

	// ============================================================================
	// CBitset::operator
	/*!
		代入算術演算子
	 */
	// ============================================================================
	CBitset &operator-=(const CBitset &rhs) {
		m_delegate &= ~rhs.m_delegate;
		return *this;
	}

	// ============================================================================
	// CBitset::operator
	/*!
		代入論述演算子
	 */
	// ============================================================================
	CBitset &operator&=(const CBitset &rhs) {
		m_delegate &= static_cast<const delegate_type &>(rhs);
		return *this;
	}
	CBitset &operator|=(const CBitset &rhs) {
		m_delegate |= static_cast<const delegate_type &>(rhs);
		return *this;
	}
	CBitset &operator^=(const CBitset &rhs) {
		m_delegate ^= static_cast<const delegate_type &>(rhs);
		return *this;
	}

	// ============================================================================
	// CBitset::operator
	/*!
		代入シフト演算子
	 */
	// ============================================================================
	CBitset &operator<<=(size_type pos) {
		m_delegate <<= pos;
		return *this;
	}
	CBitset &operator>>=(size_type pos) {
		m_delegate >>= pos;
		return *this;
	}

	// ============================================================================
	// CBitset::operator
	/*!
		比較演算子
	 */
	// ============================================================================
	bool operator==(const CBitset &rhs) {
		return (m_delegate == rhs.m_delegate);
	}
	bool operator!=(const CBitset &rhs) {
		return (m_delegate != rhs.m_delegate);
	}

	// ============================================================================
	// CBitset::at
	/*!
		検索
	 */
	// ============================================================================
	const_reference at(size_type pos) const {
		return m_delegate.test(pos);
	}
	reference at(size_type pos) {
		return m_delegate[pos];
	}

	// ============================================================================
	// CBitset::find_low_on_bit
	/*!
		検索(下位ON)
	 */
	// ============================================================================
	size_type find_low_on_bit() const {
		for (size_type i = 0; i < static_size; ++i) {
			if (m_delegate[i]) {
				return i;
				break;
			}
		}
		return npos;
	}

	// ============================================================================
	// CBitset::find_low_off_bit
	/*!
		検索(下位OFF)
	 */
	// ============================================================================
	size_type find_low_off_bit() const {
		for (size_type i = 0; i < static_size; ++i) {
			if (!m_delegate[i]) {
				return i;
				break;
			}
		}
		return npos;
	}

	// ============================================================================
	// CBitset::find_high_on_bit
	/*!
		検索(上位ON)
	 */
	// ============================================================================
	size_type find_high_on_bit() const {
		for (size_type i = static_size; 0 < i; --i) {
			size_type crnt = i - 1;
			if (m_delegate[crnt]) {
				return crnt;
				break;
			}
		}
		return npos;
	}

	// ============================================================================
	// CBitset::find_high_off_bit
	/*!
		検索(上位OFF)
	 */
	// ============================================================================
	size_type find_high_off_bit() const {
		for (size_type i = static_size; 0 < i; --i) {
			size_type crnt = i - 1;
			if (!m_delegate[crnt]) {
				return crnt;
				break;
			}
		}
		return npos;
	}

	// ============================================================================
	// CBitset::flip
	/*!
		反転
	 */
	// ============================================================================
	CBitset &flip() {
		m_delegate.flip();
		return *this;
	}
	CBitset &flip(size_type pos) {
		m_delegate.flip(pos);
		return *this;
	}

	// ============================================================================
	// CBitset::size
	/*!
		サイズアクセス
	
		@return サイズ
	 */
	// ============================================================================
	size_type size() const {
		return m_delegate.size();
	}

	// ============================================================================
	// CBitset::count
	/*!
		カウント
	
		@return カウント
	 */
	// ============================================================================
	size_type count() const {
		return m_delegate.count();
	}

	// ============================================================================
	// CBitset::to_ulong
	/*!
		ulong化

		@return ulong
	*/
	// ============================================================================
	unsigned long to_ulong() const {
		return m_delegate.to_ulong();
	}

	// ============================================================================
	// CBitset::to_string
	/*!
		文字列化
	
		@return 文字列
	 */
	// ============================================================================
	template<typename TElem, typename TTr, typename TAlloc>
	std::basic_string<TElem, TTr, TAlloc> to_string() const {
		return m_delegate.to_string<std::basic_string<TElem, TTr, TAlloc> >();
	}
	template<typename TElem, typename TTr>
	std::basic_string<TElem, TTr> to_string() const {
		return m_delegate.to_string<std::basic_string<TElem, TTr> >();
	}
	template<typename TElem>
	std::basic_string<TElem> to_string() const {
		return m_delegate.to_string<std::basic_string<TElem> >();
	}
	std::string to_string() const {
		return m_delegate.to_string();
	}

	// ============================================================================
	// CBitset::test
	/*!
		テスト
	 */
	// ============================================================================
	bool test(size_type pos) const {
		return m_delegate.test(pos);
	}

	// ============================================================================
	// CBitset::any
	/*!
		どれか
	 */
	// ============================================================================
	bool any() const {
		return m_delegate.any();
	}

	// ============================================================================
	// CBitset::none
	/*!
		どれも
	 */
	// ============================================================================
	bool none() const {
		return m_delegate.none();
	}

	// ============================================================================
	// CBitset::full
	/*!
		すべて
	 */
	// ============================================================================
	bool full() const {
		return (~m_delegate).none();
	}

	// ============================================================================
	// CBitset::set
	/*!
		セット
	 */
	// ============================================================================
	CBitset &set() {
		m_delegate.set();
		return *this;
	}
	CBitset &set(size_type pos, value_type value = true) {
		m_delegate.set(pos, value);
		return *this;
	}

	// ============================================================================
	// CBitset::reset
	/*!
		リセット
	 */
	// ============================================================================
	CBitset &reset() {
		m_delegate.reset();
		return *this;
	}
	CBitset &reset(size_type pos) {
		m_delegate.reset(pos);
		return *this;
	}
	template<typename TElem, typename TTr, typename TAlloc>
	CBitset &reset(const std::basic_string<TElem, TTr, TAlloc> &str, typename std::basic_string<TElem, TTr, TAlloc>::size_type pos, typename std::basic_string<TElem, TTr, TAlloc>::size_type count = 0, TElem e0 = TElem('0')) {
		m_delegate._Construct(str, pos, count, e0);
		return *this;
	}

	
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CBitset::CBitset
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CBitset() : m_delegate() {}
	CBitset(block_type val) : m_delegate(val) {}
	CBitset(const delegate_type &val) : m_delegate(val) {}
	template<typename TElem, typename TTr, typename TAlloc>
	CBitset(const std::basic_string<TElem, TTr, TAlloc> &str, typename std::basic_string<TElem, TTr, TAlloc>::size_type pos = 0) : m_delegate(str, pos) {}
	template<typename TElem, typename TTr, typename TAlloc>
	CBitset(const std::basic_string<TElem, TTr, TAlloc> &str, typename std::basic_string<TElem, TTr, TAlloc>::size_type pos, typename std::basic_string<TElem, TTr, TAlloc>::size_type count = 0, TElem e0 = TElem('0')) : m_delegate(str, pos, count, e0) {}

	
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	delegate_type m_delegate;	//<委譲


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	//-- Public Class ----------------- 公開クラス -----------------------------******PCL*
public:
	//constイテレータ
	class CConstIterator : public std::iterator<std::random_access_iterator_tag, const value_type> {
	private:
		const CBitset	*m_src;
		size_type		m_index;

		typedef std::iterator<std::random_access_iterator_tag, const value_type>	super_type;
	public:
		typedef typename super_type::difference_type	difference_type;
		typedef typename super_type::pointer			pointer;
		typedef typename super_type::reference			reference;
		typedef typename super_type::value_type			value_type;

		CConstIterator() : m_src(NULL) {
			m_index = 0;
		}
		CConstIterator(const CBitset &src, size_type index) : m_src(&src) {
			m_index = index;
		}

		//ポインタ剥がし
		reference operator*() {
			return (*m_src)[m_index];
		}
		//アロー演算子
		pointer operator->() {
			return &operator*();
		}

		//インデクサ
		reference operator[](size_type index) {
			return (*m_src)[m_index + index];
		}
		template <typename TIndex>
		reference operator[](TIndex index) {
			return operator[](static_cast<size_type>(index));
		}

		//演算子
		CConstIterator &operator++() {
			++m_index;
			return *this;
		}
		CConstIterator operator++(int) {
			CConstIterator result(*this);
			return ++result;
		}
		CConstIterator &operator+=(difference_type distance) {
			m_index += distance;
			return *this;
		}
		CConstIterator &operator--() {
			--m_index;
			return *this;
		}
		CConstIterator operator--(int) {
			CConstIterator result(*this);
			--result;
			return result;
		}
		CConstIterator &operator-=(difference_type distance) {
			m_index -= distance;
			return *this;
		}
		CConstIterator operator-(difference_type distance) {
			CConstIterator result(*this);
			result -= distance;
			return result;
		}
		difference_type operator-(const CConstIterator &rhs) {
			return difference_type(m_index) - difference_type(rhs.m_index);
		}

		//比較
		bool operator==(const CConstIterator &rhs) {
			return &**this == &*rhs;
		}
		bool operator!=(const CConstIterator &rhs) {
			return &**this != &*rhs;
		}
		bool operator<(const CConstIterator &rhs) {
			return &**this < &*rhs;
		}
		bool operator<=(const CConstIterator &rhs) {
			return &**this <= &*rhs;
		}
		bool operator>(const CConstIterator &rhs) {
			return &**this > &*rhs;
		}
		bool operator>=(const CConstIterator &rhs) {
			return &**this >= &*rhs;
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
		typedef typename super_type::value_type			value_type;

		CIterator() : m_delegate() {}
		CIterator(CBitset &src, size_type index) : m_delegate(src, index) {}

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
}; //class CBitset<std::size_t NSize>





















































// ============================================================================
// CBitset::operator
/*!
	算術演算子
 */
// ============================================================================
template<std::size_t NSize>
inline CBitset<NSize> operator-(const CBitset<NSize> &lhs, const CBitset<NSize> &rhs) {
	typedef CBitset<NSize>	current_type;
	current_type result(lhs);
	result -= rhs;
	return result;
}

// ============================================================================
// CBitset::operator
/*!
	論述演算子
 */
// ============================================================================
template<std::size_t NSize>
inline CBitset<NSize> operator&(const CBitset<NSize> &lhs, const CBitset<NSize> &rhs) {
	typedef CBitset<NSize>	current_type;
	current_type result(lhs);
	result &= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CBitset<NSize> operator|(const CBitset<NSize> &lhs, const CBitset<NSize> &rhs) {
	typedef CBitset<NSize>	current_type;
	current_type result(lhs);
	result |= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CBitset<NSize> operator^(const CBitset<NSize> &lhs, const CBitset<NSize> &rhs) {
	typedef CBitset<NSize>	current_type;
	current_type result(lhs);
	result ^= rhs;
	return result;
}

// ============================================================================
// CBitset::operator
/*!
	シフト演算子
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CBitset<NSize> operator<<(const CBitset<NSize> &lhs, typename CBitset<NSize>::size_type rhs) {
	typedef CBitset<NSize>	current_type;
	current_type result(lhs);
	result <<= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CBitset<NSize> operator>>(const CBitset<NSize> &lhs, typename CBitset<NSize>::size_type rhs) {
	typedef CBitset<NSize>	current_type;
	current_type result(lhs);
	result >>= rhs;
	return result;
}

// ============================================================================
// CBitset::operator
/*!
	入出力演算子
 */
// ============================================================================
template<std::size_t NSize, typename TElem, typename TTr>
inline std::basic_ostream<TElem, TTr> &operator<<(std::basic_ostream<TElem, TTr> &lhs, const CBitset<NSize> &rhs) {
	return lhs << rhs.m_delegate;
}

template<std::size_t NSize, typename TElem, typename TTr>
inline std::basic_istream<TElem, TTr> &operator>>(std::basic_istream<TElem, TTr> &lhs, CBitset<NSize> &rhs) {
	return lhs >> rhs.m_delegate;
}











































} //namespace accel
#endif //#if	defined(__cplusplus)

// =============================================================================
// accel::CBitset::Function
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
