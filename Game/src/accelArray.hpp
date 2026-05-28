// ============================================================================
/*!
	@file	accelArray.hpp
	@brief	配列クラス

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2007-2009 Dimps
	$Id: accelArray.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page AccelArray 配列クラス

	@section AccelArraySummary 概要
		C++言語にて配列を扱う処理を提供します。
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
#include <iterator>


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*


namespace accel {
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 配列クラス
		クラスの配列化を行います。
 */
template<typename TType, std::size_t NSize>
class CArray {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	enum {static_size = NSize};
	typedef TType													value_type;
	typedef unsigned long											size_type;
	typedef CArray<value_type, static_size>							type;
	template<typename TReqType, std::size_t TReqNum>
	struct gene {
		typedef CArray<TReqType, TReqNum>							type;
	};
	template<std::size_t TReqNum>
	struct array {
		typedef CArray<CArray<value_type, TReqNum>, static_size>	type;
	};

	typedef TType													*pointer;
	typedef const TType												*const_pointer;
	typedef TType													&reference;
	typedef const TType												&const_reference;

	typedef TType													*iterator;
	typedef const TType												*const_iterator;
	typedef std::reverse_iterator<iterator>							reverse_iterator;
	typedef const std::reverse_iterator<iterator>					const_reverse_iterator;


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	value_type		_[static_size];


	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CArray::cast
	/*!
		キャストアクセス
	 */
	// ============================================================================
	operator iterator()				{return &_[0];}
	operator const_iterator() const	{return &_[0];}

	// ============================================================================
	// CArray::Indexer
	/*!
		インデクサアクセス
	 */
	// ============================================================================
	const_reference	operator[](size_type i) const		{return _[i];}
	reference		operator[](size_type i)				{return _[i];}
	template <typename TIndex>
	const_reference	operator[](TIndex i) const			{return operator[](static_cast<size_type>(i));}
	template <typename TIndex>
	reference		operator[](TIndex i)				{return operator[](static_cast<size_type>(i));}

	// ============================================================================
	// CArray::Terminal
	/*!
		ターミネータアクセス
	 */
	// ============================================================================
	const_reference			front() const	{return _[0];}
	reference				front()			{return _[0];}
	const_reference			back() const	{return _[static_size-1];}
	reference				back()			{return _[static_size-1];}

	// ============================================================================
	// CArray::Iterator
	/*!
		イテレータアクセス
	 */
	// ============================================================================
	const_iterator			begin() const	{return _;}
	iterator				begin()			{return _;}
	const_iterator			end() const		{return _ + static_size;}
	iterator				end()			{return _ + static_size;}
	const_reverse_iterator	rbegin() const	{return const_reverse_iterator(_ + static_size);}
	reverse_iterator		rbegin()		{return reverse_iterator(_ + static_size);}
	const_reverse_iterator	rend() const	{return const_reverse_iterator(_);}
	reverse_iterator		rend()			{return reverse_iterator(_);}

	// ============================================================================
	// CArray::size
	/*!
		サイズアクセス
	 */
	// ============================================================================
	size_type	size() const 		{return static_size;}
	size_type	bytesize() const 	{return static_size * sizeof(value_type);}

	// ============================================================================
	// CArray::initializer
	/*!
		イニシャライザ

		@note
			C言語配列との親和性を考えて、コンストラクタは定義しません。
			初期化データを作成する場合は下記の様にすると良いでしょう。
			CArray data = CArray::initializer(value_type);
	 */
	// ============================================================================
	static CArray initializer(const value_type &rhs = value_type()) {
		typedef CArray	current_type;
		current_type result;
		for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
			result._[i] = rhs;
		}
		return result;
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
}; //class CArray<typename TType, std::size_t NSize>

template<typename TType>
class CArray<TType, 1> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	enum {static_size = 1};
	typedef TType													value_type;
	typedef unsigned long											size_type;
	typedef CArray<value_type, static_size>							type;
	template<typename TReqType, std::size_t TReqNum>
	struct gene {
		typedef CArray<TReqType, TReqNum>							type;
	};
	template<std::size_t TReqNum>
	struct array {
		typedef CArray<CArray<value_type, TReqNum>, static_size>	type;
	};

	typedef TType													*pointer;
	typedef const TType												*const_pointer;
	typedef TType													&reference;
	typedef const TType												&const_reference;

	typedef TType													*iterator;
	typedef const TType												*const_iterator;
	typedef std::reverse_iterator<iterator>							reverse_iterator;
	typedef const std::reverse_iterator<iterator>					const_reverse_iterator;


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	value_type		_[static_size];


	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CArray::cast
	/*!
		キャストアクセス
	 */
	// ============================================================================
	operator iterator()					{return &_[0];}
	operator const_iterator() const		{return &_[0];}
	operator reference()				{return _[0];}
	operator const_reference() const	{return _[0];}

	// ============================================================================
	// CArray::Indexer
	/*!
		インデクサアクセス
	 */
	// ============================================================================
	const_reference	operator[](size_type i) const		{return _[i];}
	reference		operator[](size_type i)				{return _[i];}
	template <typename TIndex>
	const_reference	operator[](TIndex i) const			{return operator[](static_cast<size_type>(i));}
	template <typename TIndex>
	reference		operator[](TIndex i)				{return operator[](static_cast<size_type>(i));}

	// ============================================================================
	// CArray::Terminal
	/*!
		ターミネータアクセス
	 */
	// ============================================================================
	const_reference			front() const	{return _[0];}
	reference				front()			{return _[0];}
	const_reference			back() const	{return _[static_size-1];}
	reference				back()			{return _[static_size-1];}

	// ============================================================================
	// CArray::Iterator
	/*!
		イテレータアクセス
	 */
	// ============================================================================
	const_iterator			begin() const	{return _;}
	iterator				begin()			{return _;}
	const_iterator			end() const		{return _ + static_size;}
	iterator				end()			{return _ + static_size;}
	const_reverse_iterator	rbegin() const	{return const_reverse_iterator(_ + static_size);}
	reverse_iterator		rbegin()		{return reverse_iterator(_ + static_size);}
	const_reverse_iterator	rend() const	{return const_reverse_iterator(_);}
	reverse_iterator		rend()			{return reverse_iterator(_);}

	// ============================================================================
	// CArray::size
	/*!
		サイズアクセス
	 */
	// ============================================================================
	size_type	size() const 		{return static_size;}
	size_type	bytesize() const 	{return static_size * sizeof(value_type);}

	// ============================================================================
	// CArray::initializer
	/*!
		イニシャライザ

		@note
			C言語配列との親和性を考えて、コンストラクタは定義しません。
			初期化データを作成する場合は下記の様にすると良いでしょう。
			CArray data = CArray::initializer(value_type);
	 */
	// ============================================================================
	static CArray initializer(const value_type &rhs = value_type()) {
		typedef CArray	current_type;
		current_type result;
		for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
			result._[i] = rhs;
		}
		return result;
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	reference operator=(const_reference in) {return _[0] = in;}


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
}; //class CArray<typename TType, 1>

template<typename TType>
class CArray<TType, 2> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	enum {static_size = 2};
	typedef TType													value_type;
	typedef unsigned long											size_type;
	typedef CArray<value_type, static_size>							type;
	template<typename TReqType, std::size_t TReqNum>
	struct gene {
		typedef CArray<TReqType, TReqNum>							type;
	};
	template<std::size_t TReqNum>
	struct array {
		typedef CArray<CArray<value_type, TReqNum>, static_size>	type;
	};

	typedef TType													*pointer;
	typedef const TType												*const_pointer;
	typedef TType													&reference;
	typedef const TType												&const_reference;

	typedef TType													*iterator;
	typedef const TType												*const_iterator;
	typedef std::reverse_iterator<iterator>							reverse_iterator;
	typedef const std::reverse_iterator<iterator>					const_reverse_iterator;


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	value_type		_[static_size];


	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CArray::property_emulate
	/*!
		擬似プロパティ
	 */
	// ============================================================================
	const_reference	x() const		{return _[0];}
	reference		x()				{return _[0];}
	const_reference	y() const		{return _[1];}
	reference		y()				{return _[1];}
	const_reference	u() const		{return _[0];}
	reference		u()				{return _[0];}
	const_reference	v() const		{return _[1];}
	reference		v()				{return _[1];}
	const_reference	s() const		{return _[0];}
	reference		s()				{return _[0];}
	const_reference	t() const		{return _[1];}
	reference		t()				{return _[1];}
	const_reference	width() const	{return _[0];}
	reference		width()			{return _[0];}
	const_reference	height() const	{return _[1];}
	reference		height()		{return _[1];}

	// ============================================================================
	// CArray::cast
	/*!
		キャストアクセス
	 */
	// ============================================================================
	operator iterator()				{return &_[0];}
	operator const_iterator() const	{return &_[0];}

	// ============================================================================
	// CArray::Indexer
	/*!
		インデクサアクセス
	 */
	// ============================================================================
	const_reference	operator[](size_type i) const		{return _[i];}
	reference		operator[](size_type i)				{return _[i];}
	template <typename TIndex>
	const_reference	operator[](TIndex i) const			{return operator[](static_cast<size_type>(i));}
	template <typename TIndex>
	reference		operator[](TIndex i)				{return operator[](static_cast<size_type>(i));}

	// ============================================================================
	// CArray::Terminal
	/*!
		ターミネータアクセス
	 */
	// ============================================================================
	const_reference			front() const	{return _[0];}
	reference				front()			{return _[0];}
	const_reference			back() const	{return _[static_size-1];}
	reference				back()			{return _[static_size-1];}

	// ============================================================================
	// CArray::Iterator
	/*!
		イテレータアクセス
	 */
	// ============================================================================
	const_iterator			begin() const	{return _;}
	iterator				begin()			{return _;}
	const_iterator			end() const		{return _ + static_size;}
	iterator				end()			{return _ + static_size;}
	const_reverse_iterator	rbegin() const	{return const_reverse_iterator(_ + static_size);}
	reverse_iterator		rbegin()		{return reverse_iterator(_ + static_size);}
	const_reverse_iterator	rend() const	{return const_reverse_iterator(_);}
	reverse_iterator		rend()			{return reverse_iterator(_);}

	// ============================================================================
	// CArray::size
	/*!
		サイズアクセス
	 */
	// ============================================================================
	size_type	size() const 		{return static_size;}
	size_type	bytesize() const 	{return static_size * sizeof(value_type);}

	// ============================================================================
	// CArray::initializer
	/*!
		イニシャライザ

		@note
			C言語配列との親和性を考えて、コンストラクタは定義しません。
			初期化データを作成する場合は下記の様にすると良いでしょう。
			CArray data = CArray::initializer(value_type);
	 */
	// ============================================================================
	static CArray initializer(const value_type &rhs = value_type()) {
		typedef CArray	current_type;
		current_type result;
		for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
			result._[i] = rhs;
		}
		return result;
	}
	static CArray initializer(const value_type &rhs0, const value_type &rhs1) {
		typedef CArray	current_type;
		current_type result = {{rhs0, rhs1}};
		return result;
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
}; //class CArray<typename TType, 2>

template<typename TType>
class CArray<TType, 3> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	enum {static_size = 3};
	typedef TType													value_type;
	typedef unsigned long											size_type;
	typedef CArray<value_type, static_size>							type;
	template<typename TReqType, std::size_t TReqNum>
	struct gene {
		typedef CArray<TReqType, TReqNum>							type;
	};
	template<std::size_t TReqNum>
	struct array {
		typedef CArray<CArray<value_type, TReqNum>, static_size>	type;
	};

	typedef TType													*pointer;
	typedef const TType												*const_pointer;
	typedef TType													&reference;
	typedef const TType												&const_reference;

	typedef TType													*iterator;
	typedef const TType												*const_iterator;
	typedef std::reverse_iterator<iterator>							reverse_iterator;
	typedef const std::reverse_iterator<iterator>					const_reverse_iterator;


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	value_type		_[static_size];


	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CArray::property_emulate
	/*!
		擬似プロパティ
	 */
	// ============================================================================
	const_reference	x() const		{return _[0];}
	reference		x()				{return _[0];}
	const_reference	y() const		{return _[1];}
	reference		y()				{return _[1];}
	const_reference	z() const		{return _[2];}
	reference		z()				{return _[2];}
	const_reference	r() const		{return _[0];}
	reference		r()				{return _[0];}
	const_reference	g() const		{return _[1];}
	reference		g()				{return _[1];}
	const_reference	b() const		{return _[2];}
	reference		b()				{return _[2];}
	const_reference	width() const	{return _[0];}
	reference		width()			{return _[0];}
	const_reference	height() const	{return _[1];}
	reference		height()		{return _[1];}
	const_reference	depth() const	{return _[2];}
	reference		depth()			{return _[2];}
	const_reference	yaw() const		{return _[0];}
	reference		yaw()			{return _[0];}
	const_reference	pitch() const	{return _[1];}
	reference		pitch()			{return _[1];}
	const_reference	roll() const	{return _[2];}
	reference		roll()			{return _[2];}

	// ============================================================================
	// CArray::cast
	/*!
		キャストアクセス
	 */
	// ============================================================================
	operator iterator()				{return &_[0];}
	operator const_iterator() const	{return &_[0];}

	// ============================================================================
	// CArray::Indexer
	/*!
		インデクサアクセス
	 */
	// ============================================================================
	const_reference	operator[](size_type i) const		{return _[i];}
	reference		operator[](size_type i)				{return _[i];}
	template <typename TIndex>
	const_reference	operator[](TIndex i) const			{return operator[](static_cast<size_type>(i));}
	template <typename TIndex>
	reference		operator[](TIndex i)				{return operator[](static_cast<size_type>(i));}

	// ============================================================================
	// CArray::Terminal
	/*!
		ターミネータアクセス
	 */
	// ============================================================================
	const_reference			front() const	{return _[0];}
	reference				front()			{return _[0];}
	const_reference			back() const	{return _[static_size-1];}
	reference				back()			{return _[static_size-1];}

	// ============================================================================
	// CArray::Iterator
	/*!
		イテレータアクセス
	 */
	// ============================================================================
	const_iterator			begin() const	{return _;}
	iterator				begin()			{return _;}
	const_iterator			end() const		{return _ + static_size;}
	iterator				end()			{return _ + static_size;}
	const_reverse_iterator	rbegin() const	{return const_reverse_iterator(_ + static_size);}
	reverse_iterator		rbegin()		{return reverse_iterator(_ + static_size);}
	const_reverse_iterator	rend() const	{return const_reverse_iterator(_);}
	reverse_iterator		rend()			{return reverse_iterator(_);}

	// ============================================================================
	// CArray::size
	/*!
		サイズアクセス
	 */
	// ============================================================================
	size_type	size() const 		{return static_size;}
	size_type	bytesize() const 	{return static_size * sizeof(value_type);}

	// ============================================================================
	// CArray::initializer
	/*!
		イニシャライザ

		@note
			C言語配列との親和性を考えて、コンストラクタは定義しません。
			初期化データを作成する場合は下記の様にすると良いでしょう。
			CArray data = CArray::initializer(value_type);
	 */
	// ============================================================================
	static CArray initializer(const value_type &rhs = value_type()) {
		typedef CArray	current_type;
		current_type result;
		for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
			result._[i] = rhs;
		}
		return result;
	}
	static CArray initializer(const value_type &rhs0, const value_type &rhs1, const value_type &rhs2 = value_type()) {
		typedef CArray	current_type;
		current_type result = {{rhs0, rhs1, rhs2}};
		return result;
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
}; //class CArray<typename TType, 3>

template<typename TType>
class CArray<TType, 4> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	enum {static_size = 4};
	typedef TType													value_type;
	typedef unsigned long											size_type;
	typedef CArray<value_type, static_size>							type;
	template<typename TReqType, std::size_t TReqNum>
	struct gene {
		typedef CArray<TReqType, TReqNum>							type;
	};
	template<std::size_t TReqNum>
	struct array {
		typedef CArray<CArray<value_type, TReqNum>, static_size>	type;
	};

	typedef TType													*pointer;
	typedef const TType												*const_pointer;
	typedef TType													&reference;
	typedef const TType												&const_reference;

	typedef TType													*iterator;
	typedef const TType												*const_iterator;
	typedef std::reverse_iterator<iterator>							reverse_iterator;
	typedef const std::reverse_iterator<iterator>					const_reverse_iterator;


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	value_type		_[static_size];


	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CArray::property_emulate
	/*!
		擬似プロパティ
	 */
	// ============================================================================
	const_reference	x() const		{return _[0];}
	reference		x()				{return _[0];}
	const_reference	y() const		{return _[1];}
	reference		y()				{return _[1];}
	const_reference	z() const		{return _[2];}
	reference		z()				{return _[2];}
	const_reference	w() const		{return _[3];}
	reference		w()				{return _[3];}
	const_reference	r() const		{return _[0];}
	reference		r()				{return _[0];}
	const_reference	g() const		{return _[1];}
	reference		g()				{return _[1];}
	const_reference	b() const		{return _[2];}
	reference		b()				{return _[2];}
	const_reference	a() const		{return _[3];}
	reference		a()				{return _[3];}
	const_reference	left() const	{return _[0];}
	reference		left()			{return _[0];}
	const_reference	top() const		{return _[1];}
	reference		top()			{return _[1];}
	const_reference	right() const	{return _[2];}	const_reference	width() const	{return _[2];}
	reference		right()			{return _[2];}	reference		width()			{return _[2];}
	const_reference	bottom() const	{return _[3];}	const_reference	height() const	{return _[3];}
	reference		bottom()		{return _[3];}	reference		height()		{return _[3];}

	// ============================================================================
	// CArray::cast
	/*!
		キャストアクセス
	 */
	// ============================================================================
	operator iterator()				{return &_[0];}
	operator const_iterator() const	{return &_[0];}

	// ============================================================================
	// CArray::Indexer
	/*!
		インデクサアクセス
	 */
	// ============================================================================
	const_reference	operator[](size_type i) const		{return _[i];}
	reference		operator[](size_type i)				{return _[i];}
	template <typename TIndex>
	const_reference	operator[](TIndex i) const			{return operator[](static_cast<size_type>(i));}
	template <typename TIndex>
	reference		operator[](TIndex i)				{return operator[](static_cast<size_type>(i));}

	// ============================================================================
	// CArray::Terminal
	/*!
		ターミネータアクセス
	 */
	// ============================================================================
	const_reference			front() const	{return _[0];}
	reference				front()			{return _[0];}
	const_reference			back() const	{return _[static_size-1];}
	reference				back()			{return _[static_size-1];}

	// ============================================================================
	// CArray::Iterator
	/*!
		イテレータアクセス
	 */
	// ============================================================================
	const_iterator			begin() const	{return _;}
	iterator				begin()			{return _;}
	const_iterator			end() const		{return _ + static_size;}
	iterator				end()			{return _ + static_size;}
	const_reverse_iterator	rbegin() const	{return const_reverse_iterator(_ + static_size);}
	reverse_iterator		rbegin()		{return reverse_iterator(_ + static_size);}
	const_reverse_iterator	rend() const	{return const_reverse_iterator(_);}
	reverse_iterator		rend()			{return reverse_iterator(_);}

	// ============================================================================
	// CArray::size
	/*!
		サイズアクセス
	 */
	// ============================================================================
	size_type	size() const 		{return static_size;}
	size_type	bytesize() const 	{return static_size * sizeof(value_type);}

	// ============================================================================
	// CArray::initializer
	/*!
		イニシャライザ

		@note
			C言語配列との親和性を考えて、コンストラクタは定義しません。
			初期化データを作成する場合は下記の様にすると良いでしょう。
			CArray data = CArray::initializer(value_type);
	 */
	// ============================================================================
	static CArray initializer(const value_type &rhs = value_type()) {
		typedef CArray	current_type;
		current_type result;
		for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
			result._[i] = rhs;
		}
		return result;
	}
	static CArray initializer(const value_type &rhs0, const value_type &rhs1, const value_type &rhs2 = value_type(), const value_type &rhs3 = value_type()) {
		typedef CArray	current_type;
		current_type result = {{rhs0, rhs1, rhs2, rhs3}};
		return result;
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
}; //class CArray<typename TType, 4>





















































#if !defined(ACLD_DISABLE_OPERATOR_OVERLOAD)
// ============================================================================
// CArray::operator
/*!
	単項演算子
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator+(const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	return rhs;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator-(const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		result._[i] = current_type::value_type(-rhs._[i]);
	}
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator~(const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		result._[i] = current_type::value_type(~rhs._[i]);
	}
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator!(const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		result._[i] = current_type::value_type(!rhs._[i]);
	}
	return result;
}

// ============================================================================
// CArray::operator
/*!
	前置インクリメント演算子・前置デクリメント演算子
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator++(CArray<TType, NSize> &lhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		++lhs._[i];
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator--(CArray<TType, NSize> &lhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		--lhs._[i];
	}
	return lhs;
}

// ============================================================================
// CArray::operator
/*!
	後置インクリメント演算子・後置デクリメント演算子
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator++(CArray<TType, NSize> &lhs, int) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	++lhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator--(CArray<TType, NSize> &lhs, int) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	--lhs;
	return result;
}

// ============================================================================
// CArray::operator
/*!
	代入演算子(スカラ)

	@note
		代入演算子(ベクトル)はコンパイラ定義を使用
 */
// ============================================================================

// ============================================================================
// CArray::operator
/*!
	算術代入演算子(ベクトル)
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator+=(CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] += rhs._[i];
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator-=(CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] -= rhs._[i];
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator*=(CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] *= rhs._[i];
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator/=(CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] /= rhs._[i];
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator%=(CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] %= rhs._[i];
	}
	return lhs;
}

// ============================================================================
// CArray::operator
/*!
	算術代入演算子(スカラ)
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator+=(CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] += rhs;
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator-=(CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] -= rhs;
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator*=(CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] *= rhs;
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator/=(CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] /= rhs;
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator%=(CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] %= rhs;
	}
	return lhs;
}

// ============================================================================
// CArray::operator
/*!
	論述代入演算子(ベクトル)
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator&=(CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] &= rhs._[i];
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator|=(CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] |= rhs._[i];
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator^=(CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] ^= rhs._[i];
	}
	return lhs;
}

// ============================================================================
// CArray::operator
/*!
	論述代入演算子(スカラ)
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator&=(CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] &= rhs;
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator|=(CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] |= rhs;
	}
	return lhs;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> &operator^=(CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs._[i] ^= rhs;
	}
	return lhs;
}

// ============================================================================
// CArray::operator
/*!
	算術演算子(ベクトル)
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator+(const CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result += rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator-(const CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result -= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator*(const CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result *= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator/(const CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result /= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator%(const CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result %= rhs;
	return result;
}

// ============================================================================
// CArray::operator
/*!
	算術演算子(スカラ)
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator+(const CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result += rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator-(const CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result -= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator*(const CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result *= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator/(const CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result /= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator%(const CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result %= rhs;
	return result;
}

// ============================================================================
// CArray::operator
/*!
	論述演算子(ベクトル)
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator&(const CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result &= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator|(const CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result |= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator^(const CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result ^= rhs;
	return result;
}

// ============================================================================
// CArray::operator
/*!
	論述演算子(スカラ)
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator&(const CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result &= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator|(const CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result |= rhs;
	return result;
}

template<typename TType, std::size_t NSize>
inline CArray<TType, NSize> operator^(const CArray<TType, NSize> &lhs, const TType &rhs) {
	typedef CArray<TType, NSize>	current_type;
	current_type result(lhs);
	result ^= rhs;
	return result;
}

// ============================================================================
// CArray::operator
/*!
	比較演算子(ベクトル)

	@note
		“<,<=,>,>=”については未定義です。
		それらの比較演算子については、
		boostとの互換性を残すか捨てるか思案中です。
		boostとの互換性を残した場合は“辞書順”比較になり、
		捨てた場合は“全ての要素がtrueならtrueを返す”比較になります。
 */
// ============================================================================
template<typename TType, std::size_t NSize>
inline bool operator==(const CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	bool result = true;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		if (lhs._[i] != rhs._[i]) {
			result = false;
			break;
		}
	}
	return result;
}

template<typename TType, std::size_t NSize>
inline bool operator!=(const CArray<TType, NSize> &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	bool result = false;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		if (lhs._[i] != rhs._[i]) {
			result = true;
			break;
		}
	}
	return result;
}

// ============================================================================
// CArray::operator
/*!
	入出力演算子
 */
// ============================================================================
template<typename TType, std::size_t NSize, typename TStreamType>
inline TStreamType &operator>>(TStreamType &lhs, CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs >> rhs._[i];
	}
	return lhs;
}
template<typename TType, std::size_t NSize, typename TStreamType>
inline TStreamType &operator>>(TStreamType &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs >> rhs._[i];
	}
	return lhs;
}

template<typename TType, std::size_t NSize, typename TStreamType>
inline TStreamType &operator<<(TStreamType &lhs, CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs << rhs._[i];
	}
	return lhs;
}
template<typename TType, std::size_t NSize, typename TStreamType>
inline TStreamType &operator<<(TStreamType &lhs, const CArray<TType, NSize> &rhs) {
	typedef CArray<TType, NSize>	current_type;
	for (typename current_type::size_type i = 0; i < current_type::static_size; ++i) {
		lhs << rhs._[i];
	}
	return lhs;
}



#endif //#if !defined(ACLD_DISABLE_OPERATOR_OVERLOAD)

//------------------------------------------------------------------------------**********









} //namespace accel
#endif //#if	defined(__cplusplus)

// =============================================================================
// accel::CArray::Function
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
