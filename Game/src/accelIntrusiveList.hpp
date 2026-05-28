// ============================================================================
/*!
	@file	accelIntrusiveList.hpp
	@brief	侵食型リスト

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: accelIntrusiveList.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page AccelIntrusiveList 侵食型リスト

	@section AccelIntrusiveListSummary 概要
		侵食型の双方向循環リストを提供します。
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
//------ Include ---------------------- インクルード ---------------------------******_IC*
#include <iterator>


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*


namespace accel {
namespace intrusive {


template<typename TCrtpType>
class CListConnectPolicyDefault;





































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 侵食型リストノードクラス
		侵食型リストのノードを提供します。
 */
template<typename TCrtpType, typename TConnectPolicy = CListConnectPolicyDefault<TCrtpType> >
class CListNode : public TConnectPolicy {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef TCrtpType		crtp_type;
	typedef TConnectPolicy	connect_policy_type;


public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	typedef crtp_type										value_type;
	typedef CListNode<crtp_type, connect_policy_type>		type;
	template<typename TReqTCrtpType, typename TReqConnectPolicy = connect_policy_type>
	struct gene {
		typedef CListNode<TReqTCrtpType, TReqConnectPolicy>	type;
	};

	typedef value_type			*pointer;
	typedef const value_type	*const_pointer;
	typedef value_type			&reference;
	typedef const value_type	&const_reference;


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CListNode::operator=
	/*!
		代入演算子

		@param	src	[in]	ノード
	 */
	// ============================================================================
	reference operator=(const type &rhs) {
		if (this != &rhs) {
			independent();
			set_prev(const_cast<type &>(rhs));
		}
		return *this;
	}

	// ============================================================================
	// CListNode::independence
	/*!
		独立確認
	
		@retval	true	独立
		@retval	false	他と関係有り
	 */
	// ============================================================================
	bool independence() const {
		return (m_prev == m_next);
	}

	// ============================================================================
	// CListNode::independent
	/*!
		独立する
	 */
	// ============================================================================
	void independent() {
		m_prev->m_next = m_next;
		m_next->m_prev = m_prev;
		m_prev = m_next = this;
	}

	
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CListNode::get
	/*!
		自ノードの取得
	
		@return ノード
	 */
	// ============================================================================
	const_reference get() const {
		return static_cast<const_reference>(*this);
	}
	reference get() {
		return static_cast<reference>(*this);
	}

	// ============================================================================
	// CListNode::get_next
	/*!
		次ノードの取得
	
		@return ノード
	 */
	// ============================================================================
	const_reference get_next() const {
		return static_cast<const_reference>(*m_next);
	}
	reference get_next() {
		return static_cast<reference>(*m_next);
	}

	// ============================================================================
	// CListNode::get_prev
	/*!
		前ノードの取得
	
		@return ノード
	 */
	// ============================================================================
	const_reference get_prev() const {
		return static_cast<const_reference>(*m_prev);
	}
	reference get_prev() {
		return static_cast<reference>(*m_prev);
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// CListNode::set_next
	/*!
		次ノードの設定
	
		@param	node	[in]	ノード
	 */
	// ============================================================================
	void set_next(type &node) {
		if (this != &node) {
			node.m_next = m_next;
			node.m_prev = this;
			node.m_next->m_prev = node.m_prev->m_next = &node;
		}
	}

	// ============================================================================
	// CListNode::set_prev
	/*!
		前ノードの設定
	
		@param	node	[in]	ノード
	 */
	// ============================================================================
	void set_prev(type &node) {
		if (this != &node) {
			node.m_next = this;
			node.m_prev = m_prev;
			node.m_next->m_prev = node.m_prev->m_next = &node;
		}
	}


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CListNode::CListNode
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CListNode() {
		m_prev = m_next = this;
	}

	// ============================================================================
	// CListNode::CListNode
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CListNode(type &src) {
		m_prev = m_next = this;
		set_prev(const_cast<type &>(src));
	}

	// ============================================================================
	// CListNode::~CListNode
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	virtual ~CListNode() {
		independent();
	}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	type *m_next;
	type *m_prev;

	
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class CListNode<typename TType, typename TConnectPolicy>









































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 侵食型リストヘッドクラス
		侵食型リストのヘッドを提供します。
 */
template<typename TCrtpType, typename TConnectPolicy = CListConnectPolicyDefault<TCrtpType> >
class CList {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef TCrtpType		crtp_type;
	typedef TConnectPolicy	connect_policy_type;


public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	typedef CListNode<crtp_type, connect_policy_type>	node_type;
	typedef typename node_type::value_type				value_type;
	typedef          unsigned int						size_type;
	typedef CList<crtp_type, connect_policy_type>		type;
	template<typename TReqType, typename TReqConnectPolicy = connect_policy_type>
	struct gene {
		typedef CList<TReqType, TReqConnectPolicy>		type;
	};

	typedef typename node_type::pointer				pointer;
	typedef typename node_type::const_pointer		const_pointer;
	typedef typename node_type::reference			reference;
	typedef typename node_type::const_reference		const_reference;

	class CIterator;
	class CConstIterator;
	typedef CIterator								iterator;
	typedef CConstIterator							const_iterator;
	typedef std::reverse_iterator<iterator>			reverse_iterator;
	typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CList::Terminal
	/*!
		ターミネータアクセス
	 */
	// ============================================================================
	const_reference			front() const	{return m_data.get_next();}
	reference				front()			{return m_data.get_next();}
	const_reference			back() const	{return m_data.get_prev();}
	reference				back()			{return m_data.get_prev();}

	// ============================================================================
	// CList::Iterator
	/*!
		イテレータアクセス
	 */
	// ============================================================================
	const_iterator			begin() const	{return const_iterator(m_data.get_next());}
	iterator				begin()			{return iterator(m_data.get_next());}
	const_iterator			end() const		{return const_iterator(m_data.get());}
	iterator				end()			{return iterator(m_data.get());}
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
	void push_back(node_type &value) {
		value.independent();
		m_data.set_prev(value);
	}
	void push_front(node_type &value) {
		value.independent();
		m_data.set_next(value);
	}
	void pop_back() {
		m_data.get_prev().independent();
	}
	void pop_front() {
		m_data.get_next().independent();
	}

	// ============================================================================
	// CList::size
	/*!
		サイズアクセス
	
		@return サイズ
	 */
	// ============================================================================
	size_type size() const {
		size_type size = 0;
		for (const_iterator ite = begin(), ite_end = end(); ite != ite_end; ++ite) {
			size++;
		}
		return size;
	}

	// ============================================================================
	// CList::clear
	/*!
		クリア
	 */
	// ============================================================================
	void clear() {
		m_data.independent();
	}

	// ============================================================================
	// CList::empty
	/*!
		空か
	
		@retval	true	空
		@retval	false	空ではない
	 */
	// ============================================================================
	bool empty() {
		return m_data.independence();
	}

	// ============================================================================
	// CList::erase
	/*!
		削除
	
		@param	pos	[in]	削除位置
	 */
	// ============================================================================
	void erase(value_type &value) {
		value.independent();
	}
	void erase(const value_type &value) {
		erase(const_cast<value_type &>(value));
	}
	void erase(iterator pos) {
		pos->independent();
	}
	void erase(const_iterator pos) {
		erase(const_cast<iterator>(pos));
	}

	// ============================================================================
	// CList::insert
	/*!
		挿入
	
		@param	value	[in]	挿入データ
	
		@return	成功すればその要素へのイテレータ、成功しなければ末端イテレータ
	 */
	// ============================================================================
	iterator insert(iterator pos, node_type &value) {
		value.independent();
		pos->set_prev(value);
		return iterator(value.get());
	}
	iterator insert(const_iterator pos, node_type &value) {
		return insert(const_cast<iterator>(pos), value);
	}
	iterator insert(node_type &value) {
		value.independent();
		TConnectPolicy::insert(begin(), end(), value);
		return iterator(value.get());
	}
	
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CList::CList
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CList() : m_data() {}

	// ============================================================================
	// CList::~CList
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~CList() {
		clear();
	}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	node_type	m_data;	//<データ領域

	
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	//-- Public Class ----------------- 公開クラス -----------------------------******PCL*
public:
	//constイテレータ
	class CConstIterator : public std::iterator<std::bidirectional_iterator_tag, const value_type> {
	private:
		const node_type *m_value;

		typedef std::iterator<std::bidirectional_iterator_tag, const value_type>	super_type;
	public:
		typedef typename super_type::pointer	pointer;
		typedef typename super_type::reference	reference;

		CConstIterator() : m_value() {}
		CConstIterator(const node_type &value) : m_value(&value) {}

		//ポインタ剥がし
		reference operator*() {
			return static_cast<reference>(*m_value);
		}
		//アロー演算子
		pointer operator->() {
			return static_cast<pointer>(m_value);
		}

		//演算子
		CConstIterator &operator++() {
			m_value = &m_value->get_next();
			return *this;
		}
		CConstIterator operator++(int) {
			CConstIterator result(*this);
			return ++result;
		}
		CConstIterator &operator--() {
			m_value = m_value.get_prev();
			return *this;
		}
		CConstIterator operator--(int) {
			CConstIterator result(*this);
			--result;
			return result;
		}

		//比較
		bool operator==(const CConstIterator &rhs) {
			return (m_value == rhs.m_value);
		}
		bool operator!=(const CConstIterator &rhs) {
			return (m_value != rhs.m_value);
		}
	};


	//イテレータ
	class CIterator : public std::iterator<std::random_access_iterator_tag, value_type> {
	private:
		CConstIterator	m_delegate;

		typedef std::iterator<std::random_access_iterator_tag, value_type>	super_type;
		typedef CConstIterator												delegate_type;
	public:
		typedef typename super_type::pointer			pointer;
		typedef typename super_type::reference			reference;

		CIterator() : m_delegate() {}
		CIterator(node_type &value) : m_delegate(value) {}

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
		CIterator &operator--() {
			--m_delegate;
			return *this;
		}
		CIterator operator--(int) {
			CIterator result(*this);
			--result;
			return result;
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
}; //class CList<typename TType, typename TConnectPolicy>


































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 侵食型リスト標準接続ポリシー
		侵食型リストの標準的な接続を提供します。
 */
template<typename TCrtpType>
class CListConnectPolicyDefault {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef TCrtpType										crtp_type;
	typedef CListConnectPolicyDefault<crtp_type>			type;
//	typedef typename CList<crtp_type, type>::iterator		iterator;	//定義出来ず(循環した?)
//	typedef typename CList<crtp_type, type>::node_type		value_type;	//定義出来ず(循環した?)


public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
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
	// ============================================================================
	// CListConnectPolicyDefault::insert
	/*!
		挿入
	
		@param	pos		[in]	挿入基準位置
		@param	value	[in]	挿入データ
	 */
	// ============================================================================
	template<typename iterator, typename value_type>
	static void insert(iterator begin, iterator end, value_type value) {
		begin.set_prev(value);
	}


//------------------------------------------------------------------------------**********
}; //class CListConnectPolicyDefault


































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	 侵食型リスト優先順接続ポリシー
		侵食型リストに優先順の接続を提供します。
 */
template<typename TCrtpType>
class CListConnectPolicyPriority {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef TCrtpType										crtp_type;
	typedef CListConnectPolicyPriority<crtp_type>			type;
//	typedef typename CList<crtp_type, type>::iterator		iterator;	//定義出来ず(循環した?)
//	typedef typename CList<crtp_type, type>::node_type		value_type;	//定義出来ず(循環した?)


public:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
	typedef unsigned int	priority_type;


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CListConnectPolicyPriority::GetPriority
	/*!
		優先度の取得

		@return 優先度
	 */
	// ============================================================================
	priority_type GetPriority() {
		return m_priority;
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// CListConnectPolicyPriority::SetPriority
	/*!
		優先度の設定

		@param	priority	[in]	優先度
	 */
	// ============================================================================
	void SetPriority(priority_type priority) {
		m_priority = priority;
	}


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CListConnectPolicyPriority::CListConnectPolicyPriority
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CListConnectPolicyPriority() : m_priority() {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	priority_type	m_priority;


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	// ============================================================================
	// CListConnectPolicyPriority::insert
	/*!
		挿入
	
		@param	pos		[in]	開始イテレータ
		@param	end		[in]	終端イテレータ
		@param	value	[in]	挿入データ
	 */
	// ============================================================================
	template<typename iterator, typename value_type>
	static void insert(iterator begin, iterator end, value_type value) {
		priority_type prio = value.GetPriority();
		for (; begin != end; ++begin) {
			if (prio < begin->GetPriority()) {
				break;
			}
		}
		begin->set_prev(value);
	}


//------------------------------------------------------------------------------**********
}; //class CListConnectPolicyPriority


































































} //namespace intrusive
} //namespace accel
#endif //#if	defined(__cplusplus)

// =============================================================================
// accel::intrusive::CList::Function
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
