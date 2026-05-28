// ============================================================================
/*!
	@file	erTaskMulit.tpp
	@brief	matsuri C++拡張タスク

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2008-2009 Dimps
	$Id$
 */
// ============================================================================
/*
 * $Log$
 */


//------ Include ---------------------- インクルード ---------------------------******_IC*
//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace er {
namespace task {



//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// CTaskCb::AttachTask
/*!
	タスクリストへ登録

	@param	name		[in]	タスク名
	@param	owner		[in]	呼び出すメンバ関数の所属するクラス
	@param	func		[in]	呼び出すメンバ関数
	@param	destructor	[in]	タスク切断時に呼び出すメンバ関数
	@param	priority	[in]	優先度 (0x0000 ～ 0xFFFF)
	@param	user		[in]	所有者 (0:所有者設定なし(グループ削除対象外))
	@param	attribute	[in]	属性
	@param	group		[in]	グループID
	@param	stall_mask	[in]	実行待機グループマスク
	@param	run_mask	[in]	実行可能スレッドマスク
 */
// ============================================================================
#if defined(MTD_DEBUG)
template<typename TType>
void CTaskCb<TType>::AttachTask(const char *name, TType &owner, FFunc func, FFunc destructor
					, TPriority priority, TUser user, TAttribute attribute
					, TGroup group, TStallMask stall_mask, TRunMask run_mask) {
	ITaskLink::AttachTask(name, priority, user, attribute, group, stall_mask, run_mask);
	m_target = &owner;
	m_func = func;
	m_destructor = destructor;
}
#else	//#if defined(MTD_DEBUG)
template<typename TType>
void CTaskCb<TType>::AttachTask(TType &owner, FFunc func, FFunc destructor
				, TPriority priority, TUser user, TAttribute attribute
					, TGroup group, TStallMask stall_mask, TRunMask run_mask) {
	ITaskLink::AttachTask(priority, user, attribute, group, stall_mask, run_mask);
	m_target = &owner;
	m_func = func;
	m_destructor = destructor;
}
#endif	//#if defined(MTD_DEBUG)


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// ============================================================================
// CTaskCb::TaskDestructor
/*!
	タスク消失時に実行される関数

	@note
		DetachTaskが呼ばれた場合に、
		タスクマネージャ側からの要請で切断された場合に発生します。

		この関数はmtTaskのデストラクタ内で実行される場合があります。
 */
// ============================================================================
template<typename TType>
void CTaskCb<TType>::TaskDestructor(EDestructorCbType::Type type) {
	if (NULL != m_destructor) {
		(m_target->*m_destructor)();
	}
	Clear();
}


//------------------------------------------------------------------------------**********






































#if ERD_TASK_WEAK_PORTING
//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// CTaskCbIdx::AttachTask
/*!
	タスクリストへ登録

	@param	name		[in]	タスク名
	@param	owner		[in]	呼び出すメンバ関数の所属するクラス
	@param	func		[in]	呼び出すメンバ関数
	@param	destructor	[in]	タスク切断時に呼び出すメンバ関数
	@param	priority	[in]	タスク優先度
	@param	group		[in]	タクスグループ
	@param	flag		[in]	タスクフラグ
	@param	pause_level	[in]	タスクポーズレベル
 */
// ============================================================================
#if defined(MTD_DEBUG)
template<typename TType>
void CTaskCbIdx<TType>::AttachTask(const char *name, TType &owner, FFunc func, FFunc destructor
				, u16 priority, u8 group, u16 flag, u8 pause_level) {
	ITaskLink::AttachTask(name, priority, group, flag, pause_level);
	m_target = &owner;
	m_func = func;
	m_destructor = destructor;
}
#else	//#if defined(MTD_DEBUG)
template<typename TType>
void CTaskCbIdx<TType>::AttachTask(TType &owner, FFunc func, FFunc destructor
				, u16 priority, u8 group, u16 flag, u8 pause_level) {
	ITaskLink::AttachTask(priority, group, flag, pause_level);
	m_target = &owner;
	m_func = func;
	m_destructor = destructor;
}
#endif	//#if defined(MTD_DEBUG)


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// ============================================================================
// CTaskCbIdx::CTaskCbIdx
/*!
	デフォルトコンストラクタ

	@param	idx	[in]	インデックス
 */
// ============================================================================
template<typename TType>
CTaskCbIdx<TType>::CTaskCbIdx(u32 idx) : ITaskLink() {
	Clear();
	m_idx =idx;
}

// ============================================================================
// CTaskCbIdx::CTaskCbIdx
/*!
	タスクリストへ登録するコンストラクタ

	@param	idx			[in]	インデックス
	@param	name		[in]	タスク名
	@param	owner		[in]	呼び出すメンバ関数の所属するクラス
	@param	func		[in]	呼び出すメンバ関数
	@param	destructor	[in]	タスク切断時に呼び出すメンバ関数
	@param	priority	[in]	タスク優先度
	@param	group		[in]	タクスグループ
	@param	flag		[in]	タスクフラグ
	@param	pause_level	[in]	タスクポーズレベル
 */
// ============================================================================
template<typename TType>
CTaskCbIdx<TType>::CTaskCbIdx(u32 idx, const char *name
				, TType &owner, FFunc func, FFunc destructor
				, u16 priority, u8 group, u16 flag, u8 pause_level) : CTaskCbIdx(idx) {
	Clear();
	m_idx =idx;
	AttachTask(name, owner, func, destructor, priority, group, flag, pause_level);
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// ============================================================================
// CTaskCbIdx::TaskDestructor
/*!
	タスク消失時に実行される関数

	@note
		DetachTaskが呼ばれた場合に、
		タスクマネージャ側からの要請で切断された場合に発生します。

		この関数はmtTaskのデストラクタ内で実行される場合があります。
 */
// ============================================================================
template<typename TType>
void CTaskCbIdx<TType>::TaskDestructor(EDestructorCbType::Type type) {
	if (NULL != m_destructor) {
		(m_target->*m_destructor)(m_idx);
	}
	Clear();
}


//------------------------------------------------------------------------------**********
#endif //#if ERD_TASK_WEAK_PORTING













#if ERD_TASK_WEAK_PORTING
//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// ============================================================================
// ITaskLinkMulti::ITaskLinkMulti
/*!
	デフォルトコンストラクタ
 */
// ============================================================================
template <u32 TNum>
ITaskLinkMulti<TNum>::ITaskLinkMulti() : ITaskMulti()
{
	for (u32 i = 0; i < TNum; ++i) {
		getChild(i).SetIdx(i);
	}
}

// ============================================================================
// ITaskLinkMulti::~ITaskLinkMulti
/*!
	デストラクタ
 */
// ============================================================================
template <u32 TNum>
ITaskLinkMulti<TNum>::~ITaskLinkMulti()
{
	for (u32 i = 0; i < TNum; ++i) {
		getChild(i).DetachTask();
	}
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********
#endif //#if ERD_TASK_WEAK_PORTING













//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// CProcMulti::operator()
/*!
	呼び出し関数

	@param	idx	[in]	インデックス
 */
// ============================================================================
template <typename TType, unsigned long TNum>
void CProcMulti<TType, TNum>::operator()(TIndex idx) {
	if (NULL != m_proc[idx]) {
		(m_target.*m_proc[idx])();
	}
}

// ============================================================================
// CProcMulti::operator()
/*!
	全呼び出し関数
 */
// ============================================================================
template <typename TType, unsigned long TNum>
void CProcMulti<TType, TNum>::operator()() {
	for (u32 i = 0; i < TNum; ++i) {
		operator()(i);
	}
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
// ============================================================================
// CProcMulti::IsNoneProc
/*!
	プロシージャの設定が無いか確認する

	@param	idx	[in]	インデックス

	@retval	true	無い
	@retval	false	有る
 */
// ============================================================================
template <typename TType, unsigned long TNum>
bool CProcMulti<TType, TNum>::IsNoneProc(TIndex idx) const {
	return ((NULL == m_proc[idx])? true: false);
}

// ============================================================================
// CProcMulti::IsProc
/*!
	特定のプロシージャが設定されているか確認する
	
	@param	idx	[in]	インデックス

	@retval	true	無い
	@retval	false	有る
 */
// ============================================================================
template <typename TType, unsigned long TNum> template <typename T>
bool CProcMulti<TType, TNum>::IsProc(TIndex idx, T proc) {
	return ((static_cast<FProc>(proc) == m_proc[idx])? true: false);
}

//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
// ============================================================================
// CProcMulti::SetProc
/*!
	プロシージャを設定する

	@param	proc	[in]	設定するプロシージャ

	@note
		プロシージャの型は“void function()”型のメンバ関数です。
		メンバ関数で無ければいけません、static関数は設定出来ません。
		メンバ関数ならばprotectedでもprivateでも構いません。
 */
// ============================================================================
template <typename TType, unsigned long TNum> template <typename T>
void CProcMulti<TType, TNum>::SetProc(TIndex idx, T proc) {
#if 0
	m_proc[idx] = *er::mpl::static_reinterpret_cast<FProc *>(&proc);
#else
	m_proc[idx] = *reinterpret_cast<FProc *>(&proc);
#endif
}


//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// ============================================================================
// CProcMulti::CProcMulti
/*!
	デフォルトコンストラクタ

	@param	target	継承先クラス
 */
// ============================================================================
template <typename TType, unsigned long TNum>
CProcMulti<TType, TNum>::CProcMulti(TType &target) : m_target(target) {
	memset(m_proc, 0, sizeof(m_proc));
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********

















































} //namespace task
} //namespace er

// =============================================================================
// erTask::Function
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
