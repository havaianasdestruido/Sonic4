// ============================================================================
/*!
	@file	accelLerp.hpp
	@brief	線形補間ライブラリ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: accelLerp.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page AccelLerp 線形補間ライブラリ

	@section AccelLerpSummary 概要
		線形補間を提供します。
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
namespace lerp {
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 線形補間インターフェース
		線形補間のインターフェースを提供します。
 */
template<typename TType, typename TPhaseType = float>
class IAddIn {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	typedef TType								value_type;
	typedef TPhaseType							phase_type;
	typedef unsigned int						size_type;
	typedef IAddIn<value_type, phase_type>		type;
	template<typename TReqType, typename TReqPhaseType = phase_type>
	struct gene {
		typedef IAddIn<TReqType, TReqPhaseType>	type;
	};

	//データ
	struct SData {
		value_type			src;
		value_type			dst;
		phase_type			phase;
		const phase_type	original_phase;

		SData() : src(), dst(), phase(), original_phase() {}
		SData(const value_type &sr, const value_type &ds, const phase_type &ph) : src(sr), dst(ds), phase(ph), original_phase(ph) {}
		SData(const value_type &sr, const value_type &ds) : src(sr), dst(ds), phase(), original_phase() {}
		SData(const phase_type &ph) : src(), dst(), phase(ph), original_phase(ph) {}
	};
	typedef SData								data_type;


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// IAddIn::operator()
	/*!
		実行
	 */
	// ============================================================================
	virtual void operator()(data_type &data) const = 0;

	// ============================================================================
	// IAddIn::duplicate_to()
	/*!
		複製の作成
	 */
	// ============================================================================
	virtual void duplicate_to(void *dst) const = 0;


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// IAddIn::size()
	/*!
		クラスサイズの取得
	 */
	// ============================================================================
	virtual size_type size() const = 0;


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// IAddIn::~IAddIn()
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~IAddIn() {};


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class IAddIn<typename TType, typename TPhaseType>


























































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 線形補間クラス
		線形補間を提供します。
 */
template<typename TCrtpType, typename TType, typename TPhaseType = float>
class CAddIn : public IAddIn<TType, TPhaseType> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef IAddIn<TType, TPhaseType>	super_type;


public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	typedef typename super_type::data_type						data_type;
	typedef typename super_type::value_type						value_type;
	typedef typename super_type::phase_type						phase_type;
	typedef typename super_type::size_type						size_type;
	typedef TCrtpType											crtp_type;
	typedef CAddIn<crtp_type, value_type, phase_type>			type;
	template<typename TReqCrtpType, typename TReqType, typename TReqPhaseType = phase_type>
	struct gene {
		typedef CAddIn<TReqCrtpType, TReqType, TReqPhaseType>	type;
	};


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CAddIn::operator()
	/*!
		実行
	 */
	// ============================================================================
	virtual void operator()(data_type &data) const = 0;

	// ============================================================================
	// CAddIn::duplicate_to()
	/*!
		複製の作成
	 */
	// ============================================================================
	virtual void duplicate_to(void *dst) const {
		new(dst) crtp_type(*static_cast<const crtp_type *>(this));
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CAddIn::size()
	/*!
		クラスサイズの取得
	 */
	// ============================================================================
	virtual size_type size() const {
		return sizeof(crtp_type);
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class CAddIn<typename TCrtpType, typename TType, typename TPhaseType>


























































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 線形補間クラス
		線形補間を提供します。
 */
template<typename TType, unsigned int NSize = 64, typename TPhaseType = float, unsigned int TAlign = 4>
class CLerp {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef CArray<unsigned char, NSize>	data_type;

public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	enum {static_size = data_type::static_size};
	enum {align_size = TAlign};
	typedef TType													value_type;
	typedef TPhaseType												phase_type;
	typedef IAddIn<TType, TPhaseType>								add_in_type;
	typedef unsigned int											size_type;
	typedef unsigned int											align_type;
	typedef CLerp<value_type, 0, phase_type, align_size>			basic_type;
	typedef CLerp<value_type, static_size, phase_type, align_size>	type;
	template<typename TReqType, unsigned int TReqNum, typename TReqPhaseType, unsigned int TReqAlign>
	struct gene {
		typedef CLerp<TReqType, TReqNum, TReqPhaseType, TReqAlign>	type;
	};


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CLerp::operator()
	/*!
		実行

		@param	src		[in]	元値
		@param	dst		[in]	先値
		@param	phase	[in]	補間段階(0～1)

		@return 補間値
	 */
	// ============================================================================
	value_type operator()(const value_type &src, const value_type &dst, const phase_type &phase) const;
	value_type operator()(const value_type &src, const value_type &dst) const;
	value_type operator()(const phase_type &phase) const;

	// ============================================================================
	// CLerp::size
	/*!
		サイズアクセス
	
		@return サイズ
	 */
	// ============================================================================
	size_type	size() const 		{return m_size;}
	size_type	max_size() const 	{return static_size;}

	// ============================================================================
	// CLerp::clear
	/*!
		クリア
	 */
	// ============================================================================
	void clear();

	// ============================================================================
	// CLerp::count
	/*!
		登録関数数
	
		@return	登録関数数
	 */
	// ============================================================================
	size_type count() const;

	// ============================================================================
	// CLerp::empty
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
	// CLerp::operator=
	/*!
		代入演算子

		@param	src	[in]	元値

		@return 自身
	 */
	// ============================================================================
	type &operator=(const type &src);


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// CLerp::add_in
	/*!
		関数の設定
	
		@retval	true	設定成功
		@retval	false	設定失敗
	 */
	// ============================================================================
	template<typename TFuncType>
	bool add_in(const TFuncType &func);


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CLerp::CLerp
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CLerp() : m_data(), m_size() {}

	// ============================================================================
	// CLerp::CLerp
	/*!
		コピーコンストラクタ
	 */
	// ============================================================================
public:
	CLerp(const CLerp &src);

	// ============================================================================
	// CLerp::~CLerp
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	~CLerp() {
		clear();
	}

	
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	class CIterator;
	class CConstIterator;
	typedef CIterator									iterator;
	typedef CConstIterator								const_iterator;


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	data_type	m_data;	//<データ領域
	size_type	m_size;	//<使用サイズ

	
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	// ============================================================================
	// CLerp::Iterator
	/*!
		イテレータアクセス
	 */
	// ============================================================================
	const_iterator	begin() const	{return const_iterator(reinterpret_cast<const add_in_type &>((*m_data.begin())));}
	iterator		begin()			{return iterator(reinterpret_cast<add_in_type &>((*m_data.begin())));}
	const_iterator	end() const		{return const_iterator(reinterpret_cast<const add_in_type &>((*(m_data.begin() + m_size))));}
	iterator		end()			{return iterator(reinterpret_cast<add_in_type &>((*(m_data.begin() + m_size))));}


	//-- Public Class ----------------- 公開クラス -----------------------------******PCL*
public:
	//-- Local Class ------------------ ローカルクラス -------------------------******PCL*
protected:
private:
	//constイテレータ
	class CConstIterator : public std::iterator<std::forward_iterator_tag, const add_in_type> {
	private:
		const add_in_type	*m_src;

		typedef std::iterator<std::forward_iterator_tag, const add_in_type>	super_type;
	public:
		typedef typename super_type::pointer	pointer;
		typedef typename super_type::reference	reference;
		typedef typename super_type::value_type	value_type;

		CConstIterator() : m_src(NULL) {}
		CConstIterator(value_type &src) : m_src(&src) {}

		//ポインタ剥がし
		reference operator*() {
			return *m_src;
		}
		//アロー演算子
		pointer operator->() {
			return static_cast<pointer>(m_src);
		}

		//演算子
		CConstIterator &operator++() {
			typename value_type::size_type size = m_src->size() + (align_size - 1) & ~(align_size - 1);
			typename value_type::size_type &adrs = *reinterpret_cast<typename value_type::size_type *>(&m_src);
			adrs += size;
			return *this;
		}
		CConstIterator operator++(int) {
			CConstIterator result(*this);
			return ++result;
		}

		//比較
		bool operator==(const CConstIterator &rhs) {
			return (m_src == rhs.m_src);
		}
		bool operator!=(const CConstIterator &rhs) {
			return !(*this == rhs);
		}
	};


	//イテレータ
	class CIterator : public std::iterator<std::forward_iterator_tag, add_in_type> {
	private:
		CConstIterator	m_delegate;

		typedef std::iterator<std::forward_iterator_tag, add_in_type>	super_type;
		typedef CConstIterator											delegate_type;
	public:
		typedef typename super_type::pointer	pointer;
		typedef typename super_type::reference	reference;
		typedef typename super_type::value_type	value_type;

		CIterator() : m_delegate() {}
		CIterator(value_type &src) : m_delegate(src) {}

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

		//演算子
		CIterator &operator++() {
			++m_delegate;
			return *this;
		}
		CIterator operator++(int) {
			CIterator result(*this);
			return ++result;
		}

		//比較
		bool operator==(const CIterator &rhs) {
			return (m_delegate == rhs.m_delegate);
		}
		bool operator!=(const CIterator &rhs) {
			return (m_delegate != rhs.m_delegate);
		}
	};

	
//------------------------------------------------------------------------------**********
}; //class CLerp<typename TType, unsigned int NSize, typename TPhaseType, unsigned int TAlign>


// ============================================================================
// CLerp::operator()
/*!
	実行
 */
// ============================================================================
template<typename TType, unsigned int NSize, typename TPhaseType, unsigned int TAlign>
inline typename CLerp<TType, NSize, TPhaseType, TAlign>::value_type CLerp<TType, NSize, TPhaseType, TAlign>::operator()(const value_type &src, const value_type &dst, const phase_type &phase) const {
	typedef typename CLerp<TType, NSize, TPhaseType, TAlign>::value_type result_type;
	typename add_in_type::SData data(src, dst, phase);
	for (const_iterator ite = begin(), ite_end = end(); ite != ite_end; ++ite) {
		(*ite)(data);
	}
	return result_type(data.src + ((data.dst - data.src) * data.phase));
}
template<typename TType, unsigned int NSize, typename TPhaseType, unsigned int TAlign>
inline typename CLerp<TType, NSize, TPhaseType, TAlign>::value_type CLerp<TType, NSize, TPhaseType, TAlign>::operator()(const value_type &src, const value_type &dst) const {
	typedef typename CLerp<TType, NSize, TPhaseType, TAlign>::value_type result_type;
	typename add_in_type::SData data(src, dst);
	for (const_iterator ite = begin(), ite_end = end(); ite != ite_end; ++ite) {
		(*ite)(data);
	}
	return result_type(data.src + ((data.dst - data.src) * data.phase));
}
template<typename TType, unsigned int NSize, typename TPhaseType, unsigned int TAlign>
inline typename CLerp<TType, NSize, TPhaseType, TAlign>::value_type CLerp<TType, NSize, TPhaseType, TAlign>::operator()(const phase_type &phase) const {
	typedef typename CLerp<TType, NSize, TPhaseType, TAlign>::value_type result_type;
	typename add_in_type::SData data(phase);
	for (const_iterator ite = begin(), ite_end = end(); ite != ite_end; ++ite) {
		(*ite)(data);
	}
	return result_type(data.src + ((data.dst - data.src) * data.phase));
}

// ============================================================================
// CLerp::clear
/*!
	クリア
 */
// ============================================================================
template<typename TType, unsigned int NSize, typename TPhaseType, unsigned int TAlign>
inline void CLerp<TType, NSize, TPhaseType, TAlign>::clear() {
	iterator ite = begin();
	iterator ite_end = end();
	while (ite != ite_end) {
		iterator ite_crnt = ite;
		++ite;
		ite_crnt->~add_in_type();	//配置new確保なのでデストラクタを呼ぶだけ
	}
	m_size = 0;
}

// ============================================================================
// CLerp::count
/*!
	登録関数数

	@return	登録関数数
 */
// ============================================================================
template<typename TType, unsigned int NSize, typename TPhaseType, unsigned int TAlign>
inline typename CLerp<TType, NSize, TPhaseType, TAlign>::size_type CLerp<TType, NSize, TPhaseType, TAlign>::count() const {
	size_type result = 0;
	for (const_iterator ite = begin(), ite_end = end(); ite != ite_end; ++ite) {
		++result;
	}

	return result;
}

// ============================================================================
// CLerp::operator=
/*!
	代入演算子

	@param	src	[in]	元値

	@return 自身
 */
// ============================================================================
template<typename TType, unsigned int NSize, typename TPhaseType, unsigned int TAlign>
inline typename CLerp<TType, NSize, TPhaseType, TAlign>::type &CLerp<TType, NSize, TPhaseType, TAlign>::operator=(const type &src) {
	clear();
	typename data_type::iterator dst = m_data.begin();
	for (const_iterator ite = src.begin(), ite_end = src.end(); ite != ite_end; ++ite) {
		ite->duplicate_to(reinterpret_cast<void *>(&*dst));
		size_type size = ite->size() + (align_size - 1) & ~(align_size - 1);
		dst += size;
	}
	return *this;
}

// ============================================================================
// CLerp::add_in
/*!
	関数の設定

	@retval	true	設定成功
	@retval	false	設定失敗
 */
// ============================================================================
template<typename TType, unsigned int NSize, typename TPhaseType, unsigned int TAlign> template<typename TFuncType>
inline bool CLerp<TType, NSize, TPhaseType, TAlign>::add_in(const TFuncType &func) {
	bool result = false;
	size_type size = m_size + (func.size() + (align_size - 1) & ~(align_size - 1));
	if (size < static_size) {
		void *adrs = reinterpret_cast<void *>(m_data.begin() + m_size);
		new(adrs) TFuncType(func);

		m_size = size;
		result = true;
	}
	return result;
}

// ============================================================================
// CLerp::CLerp
/*!
	コピーコンストラクタ
 */
// ============================================================================
template<typename TType, unsigned int NSize, typename TPhaseType, unsigned int TAlign>
inline CLerp<TType, NSize, TPhaseType, TAlign>::CLerp(const CLerp &src) : m_data(), m_size(src.m_size) {
	typename data_type::iterator dst = m_data.begin();
	for (const_iterator ite = src.begin(), ite_end = src.end(); ite != ite_end; ++ite) {
		ite->duplicate_to(reinterpret_cast<void *>(&*dst));
		size_type size = ite->size() + (align_size - 1) & ~(align_size - 1);
		dst += size;
	}
}













template<typename TType, typename TPhaseType, unsigned int TAlign>
class CLerp<TType, 0, TPhaseType, TAlign> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	enum {static_size = 0};
	enum {align_size = TAlign};
	typedef TType													value_type;
	typedef TPhaseType												phase_type;
	typedef IAddIn<TType, TPhaseType>								add_in_type;
	typedef unsigned int											size_type;
	typedef unsigned int											align_type;
	typedef CLerp<value_type, 0, phase_type, align_size>			basic_type;
	typedef CLerp<value_type, static_size, phase_type, align_size>	type;
	template<typename TReqType, unsigned int TReqNum, typename TReqPhaseType, unsigned int TReqAlign>
	struct gene {
		typedef CLerp<TReqType, TReqNum, TReqPhaseType, TReqAlign>	type;
	};


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CLerp::operator()
	/*!
		実行

		@param	src		[in]	元値
		@param	dst		[in]	先値
		@param	phase	[in]	補間段階(0～1)

		@return 補間値
	 */
	// ============================================================================
	value_type operator()(const value_type &src, const value_type &dst, const phase_type &phase) const {
		return value_type(src + ((dst - src) * phase));
	}

	// ============================================================================
	// CLerp::size
	/*!
		サイズアクセス
	
		@return サイズ
	 */
	// ============================================================================
	size_type	size() const 		{return 0;}
	size_type	max_size() const 	{return 0;}

	// ============================================================================
	// CLerp::clear
	/*!
		クリア
	 */
	// ============================================================================
	void clear() {};

	// ============================================================================
	// CLerp::count
	/*!
		登録関数数
	
		@return	登録関数数
	 */
	// ============================================================================
	size_type count() const {return 0;};

	// ============================================================================
	// CLerp::empty
	/*!
		空か
	
		@retval	true	空
		@retval	false	空ではない
	 */
	// ============================================================================
	bool empty() {return true;}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// CLerp::add_in
	/*!
		関数の設定
	
		@retval	true	設定成功
		@retval	false	設定失敗
	 */
	// ============================================================================
	template<typename TFuncType>
	bool add_in(const TFuncType &func)	{return false;}


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	//-- Public Class ----------------- 公開クラス -----------------------------******PCL*
public:
	//-- Local Class ------------------ ローカルクラス -------------------------******PCL*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class CLerp<typename TType, 0, typename TPhaseType, unsigned int TAlign>













#if !defined(ACLD_DISABLE_OPERATOR_OVERLOAD)
// ============================================================================
// CLerp::operator
/*!
	入出力演算子
 */
// ============================================================================
template<typename TType, unsigned int NSize, typename TPhaseType, typename TFuncType>
inline CLerp<TType, NSize, TPhaseType> &operator<<(CLerp<TType, NSize, TPhaseType> &lhs, const TFuncType &rhs) {
	typedef CLerp<TType, NSize, TPhaseType>	current_type;
	lhs.add_in(rhs);
	return lhs;
}
#endif //#if !defined(ACLD_DISABLE_OPERATOR_OVERLOAD)


























































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 クリップクラス
		線形補間の両端でのクリップを提供します。
 */
template<typename TType, typename TPhaseType = float>
class CClip : public CAddIn<CClip<TType, TPhaseType>, TType, TPhaseType> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef CAddIn<CClip, TType, TPhaseType>	super_type;


public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	typedef typename super_type::data_type		data_type;
	typedef typename super_type::value_type		value_type;
	typedef typename super_type::phase_type		phase_type;
	typedef typename super_type::size_type		size_type;
	typedef typename super_type::crtp_type		crtp_type;
	typedef CClip<value_type, phase_type>		type;
	template<typename TReqType, typename TReqPhaseType = phase_type>
	struct gene {
		typedef CClip<TReqType, TReqPhaseType>	type;
	};


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CClip::operator()
	/*!
		実行
	 */
	// ============================================================================
	virtual void operator()(data_type &data) const {
		if (phase_type(1) < data.phase) {
			data.phase = phase_type(1);
		} else if (data.phase < phase_type(0)) {
			data.phase = phase_type(0);
		}
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class CClip<typename TType, typename TPhaseType>


























































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 ループクラス
		線形補間の両端での循環を提供します。
 */
template<typename TType, typename TPhaseType = float>
class CLoop : public CAddIn<CLoop<TType, TPhaseType>, TType, TPhaseType> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef CAddIn<CLoop, TType, TPhaseType>	super_type;


public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	typedef typename super_type::data_type		data_type;
	typedef typename super_type::value_type		value_type;
	typedef typename super_type::phase_type		phase_type;
	typedef typename super_type::size_type		size_type;
	typedef typename super_type::crtp_type		crtp_type;
	typedef CLoop<value_type, phase_type>		type;
	template<typename TReqType, typename TReqPhaseType = phase_type>
	struct gene {
		typedef CLoop<TReqType, TReqPhaseType>	type;
	};


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CLoop::operator()
	/*!
		実行
	 */
	// ============================================================================
	virtual void operator()(data_type &data) const {
		if (phase_type(1) < data.phase) {
			do {
				data.phase -= phase_type(1);
			} while (phase_type(1) < data.phase);
		} else if (data.phase < phase_type(0)) {
			do {
				data.phase += phase_type(1);
			} while (data.phase < phase_type(0));
		}
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class CLoop<typename TType, typename TPhaseType>


























































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 バウンドクラス
		線形補間の両端での反射を提供します。
 */
template<typename TType, typename TPhaseType = float>
class CBound : public CAddIn<CBound<TType, TPhaseType>, TType, TPhaseType> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef CAddIn<CBound, TType, TPhaseType>	super_type;


public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	typedef typename super_type::data_type		data_type;
	typedef typename super_type::value_type		value_type;
	typedef typename super_type::phase_type		phase_type;
	typedef typename super_type::size_type		size_type;
	typedef typename super_type::crtp_type		crtp_type;
	typedef CBound<value_type, phase_type>		type;
	template<typename TReqType, typename TReqPhaseType = phase_type>
	struct gene {
		typedef CBound<TReqType, TReqPhaseType>	type;
	};


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CBound::operator()
	/*!
		実行
	 */
	// ============================================================================
	virtual void operator()(data_type &data) const {
		while (true) {
			if (phase_type(1) < data.phase) {
				data.phase = phase_type(2) - data.phase;
				continue;
			} else if (data.phase < phase_type(0)) {
				data.phase = -data.phase;
				continue;
			}
			break;
		}
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class CBound<typename TType, typename TPhaseType>


























































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 リバースクラス
		線形補間の逆再生処理を提供します。
 */
template<typename TType, typename TPhaseType = float>
class CReverse : public CAddIn<CReverse<TType, TPhaseType>, TType, TPhaseType> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef CAddIn<CReverse, TType, TPhaseType>	super_type;


public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	typedef typename super_type::data_type		data_type;
	typedef typename super_type::value_type		value_type;
	typedef typename super_type::phase_type		phase_type;
	typedef typename super_type::size_type		size_type;
	typedef typename super_type::crtp_type		crtp_type;
	typedef CReverse<value_type, phase_type>		type;
	template<typename TReqType, typename TReqPhaseType = phase_type>
	struct gene {
		typedef CReverse<TReqType, TReqPhaseType>	type;
	};


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CReverse::operator()
	/*!
		実行
	 */
	// ============================================================================
	virtual void operator()(data_type &data) const {
		data = phase_type(1) - data.phase;
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class CReverse<typename TType, typename TPhaseType>


























































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 イーズクラス
		補間のイーズ処理を提供します。
 */
template<typename TType, typename TPhaseType = float, typename TWeightType = float>
class CEase : public CAddIn<CEase<TType, TPhaseType, TWeightType>, TType, TPhaseType> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef CAddIn<CEase, TType, TPhaseType>	super_type;
	typedef CLerp<TPhaseType, 0, TPhaseType>	TPhaseLinearLerpType;


public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	typedef typename super_type::data_type		data_type;
	typedef typename super_type::value_type		value_type;
	typedef typename super_type::phase_type		phase_type;
	typedef TWeightType							weight_type;
	typedef typename super_type::size_type		size_type;
	typedef typename super_type::crtp_type		crtp_type;
	typedef CEase<value_type, phase_type>		type;
	template<typename TReqType, typename TReqPhaseType = phase_type>
	struct gene {
		typedef CEase<TReqType, TReqPhaseType>	type;
	};


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CEase::operator()
	/*!
		実行
	 */
	// ============================================================================
	virtual void operator()(data_type &data) const {
		weight_type	weight = m_weight;
		phase_type	upper, lower;		//アッパー曲線・ローアー曲線

		//アッパー曲線・ローアー曲線の基点設定
		if (weight_type(0) < m_weight) {
			//減速(水平反転)
			lower = phase_type(1) - data.original_phase;
			upper = phase_type(1) - data.phase;
			upper *= upper;
		} else if (m_weight < weight_type(0)) {
			//加速
			lower = data.original_phase;
			upper = data.phase;
			upper *= upper;
			weight = -weight;
		} else {
			return;
		}

		//ウェイトが1以上なら曲線を2乗する(2なら4乗・3なら8乗)
		while (weight_type(1) < weight) {
			lower = upper;
			upper *= upper;
			weight -= weight_type(1);
		}

		//2曲線の間を補完
		if (weight_type(0) < m_weight) {
			//減速(垂直反転)
			data.phase = phase_type(1) - TPhaseLinearLerpType()(lower, upper, weight);
		} else {
			//加速
			data.phase = TPhaseLinearLerpType()(lower, upper, weight);
		};
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CEase::CEase
	/*!
		実行
	 */
	// ============================================================================
public:
	CEase(const weight_type	&weight) : m_weight(weight) {};


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	const weight_type	m_weight;


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class CEase<typename TType, typename TPhaseType>














































//------------------------------------------------------------------------------**********









} //namespace lerp
} //namespace accel
#endif //#if	defined(__cplusplus)

// =============================================================================
// accel::CLerp::Function
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
