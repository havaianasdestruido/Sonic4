// ============================================================================
/*!
	@file	erTask.cpp
	@brief	matsuri C++拡張タスク

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2007-2009 Dimps
	$Id: erTask.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */


//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "erTask.hpp"

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
// ITaskWork::new_
/*!
	タスクコア用newオペレータオーバーロードのコア

	@param	work_size	[in]	クラスサイズ
	@param	name		[in]	タスク名
	@param	priority	[in]	優先度 (0x0000 ～ 0xFFFF)
	@param	user		[in]	所有者 (0:所有者設定なし(グループ削除対象外))
	@param	attribute	[in]	属性
	@param	group		[in]	グループID
	@param	stall_mask	[in]	実行待機グループマスク
	@param	run_mask	[in]	実行可能スレッドマスク

	@return	確保したメモリのポインタ

	@note
		SHeader １つとクラスサイズが保存出来る量のメモリを確保し、
		先頭に SHeader 、その後ろにクラスを配置する。
		先頭の SHeader にはワークの管理者 ::MTS_TASK_TCB* を保存し、
		new の値として クラス配置用のメモリアドレスを返す。

		この先頭の SHeader は delete で使用する。

		この方式はアラインがずれると言う欠点がある。
		今回はクラスに返す値は４バイトアラインされている。
 */
// ============================================================================
#if defined(MTD_DEBUG)
void *ITaskWork::new_(size_t work_size, const char *name
					, TPriority priority, TUser user, TAttribute attribute
					, TGroup group, TStallMask stall_mask, TRunMask run_mask)
{
	::AMS_TCB *tcb = amTaskMake(procedure, destructor
					, priority, user, attribute
					, const_cast<char *>(name)	//要修正
					, stall_mask, group, run_mask);
	SWork *work = reinterpret_cast<SWork *>(::amTaskGetWork(tcb));
	work->data = amMemAlloc(sizeof(SHeader) + work_size);
	SHeader *header = reinterpret_cast<SHeader *>(work->data);
	header->owner = tcb;
	return reinterpret_cast<void *>(&header[1]);
}
#else	//#if defined(MTD_DEBUG)
void *ITaskWork::new_(size_t work_size
					, TPriority priority, TUser user, TAttribute attribute
					, TGroup group, TStallMask stall_mask, TRunMask run_mask)
{
	::AMS_TCB *tcb = amTaskMake(procedure, destructor
					, priority, user, attribute
					, ""
					, stall_mask, group, run_mask);
	SWork *work = reinterpret_cast<SWork *>(amTaskGetWork(tcb));
	work->data = amMemAlloc(sizeof(SHeader) + work_size);
	SHeader *header = reinterpret_cast<SHeader *>(work->data);
	header->owner = tcb;
	return reinterpret_cast<void *>(&header[1]);
}
#endif	//#if defined(MTD_DEBUG)

// ============================================================================
// ITaskWork::operator delete
/*!
	タスクベース用deleteオペレータオーバーロード
 
	@param	p	[in]	削除するポインタ

	@note
		クラスのアドレスから sizeof(SHeader) 遡ると、
		ワークの管理者 ::AMS_TCB* が分かる。
		これを使用しタスクの削除を行う。

#if ERD_TASK_WEAK_PORTING
		::mtTask の destructor に delete を呼ぶ命令が含まれているので、
		destructor を呼ばない様にしてから ::mtTaskClearTcb を実行
#endif //#if ERD_TASK_WEAK_PORTING
 */
// ============================================================================
void ITaskWork::operator delete(void *p)
{
#if 0
	//コンパイラ解決(コンパイラに依っては機能しない)
	::AMS_TCB *tcb = reinterpret_cast<ITask *>(p)->GetTaskTcb();
#else
	//決め打ち
	const ::AMS_TCB *tcb_const = reinterpret_cast<ITaskWork *>(p)->ITaskWork::GetTaskTcb();
	::AMS_TCB *tcb = const_cast< ::AMS_TCB *>(tcb_const);
#endif
	if (NULL != tcb) {
		::amTaskSetDestructor(tcb, NULL);
		SWork *work = reinterpret_cast<SWork *>(::amTaskGetWork(tcb));
		::amMemFree(work->data);

		::amTaskDelete(tcb);
	}
}

// ============================================================================
// ITaskWork::operator new
/*!
	タスクコア用deleteオペレータオーバーロード(new時例外)
 
	@param	p			[in]	削除するポインタ
	@param	name		[in]	タスク名
	@param	priority	[in]	優先度 (0x0000 ～ 0xFFFF)
	@param	user		[in]	所有者 (0:所有者設定なし(グループ削除対象外))
	@param	attribute	[in]	属性
	@param	group		[in]	グループID
	@param	stall_mask	[in]	実行待機グループマスク
	@param	run_mask	[in]	実行可能スレッドマスク
 
	@return	確保したメモリのポインタ

	@note
		new時例外は発生しない為、通常は使用しない。
 */
// ============================================================================
void ITaskWork::operator delete(void *p, const char *name
				, TPriority priority, TUser user, TAttribute attribute
				, TGroup group, TStallMask stall_mask, TRunMask run_mask)
{
	amAssert(!"Logic error! Exception is not supported when 'new' operation is used.");

	UNREFERENCED_PARAMETER(p);
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(priority);
	UNREFERENCED_PARAMETER(user);
	UNREFERENCED_PARAMETER(attribute);
	UNREFERENCED_PARAMETER(group);
	UNREFERENCED_PARAMETER(stall_mask);
	UNREFERENCED_PARAMETER(run_mask);
}

// ============================================================================
// ITaskWork::CastFromTaskTcb
/*!
	タスクTCB(aliceベース)をITaskWorkにキャストします。

	@param	tcb	[in]	タスクTCB(Cベース)

	@return ITaskWorkへのポインタ or NULL

	@note
		ITaskWork から派生していないタスクをキャストしようとすると失敗します。
		失敗した場合は NULL を返します(アサートはありません)。
 */
// ============================================================================
const ITaskWork *ITaskWork::CastFromTaskTcb(const ::AMS_TCB *tcb)
{
	//キャスト可能確認
	if ((ITaskWork::procedure == tcb->procedure)
			||
		(ITaskWork::destructor == tcb->destructor)
		) {
		//プロシージャかデストラクタがITaskWork準拠なら
		const SWork *work = reinterpret_cast<const SWork *>(
						::amTaskGetWork(const_cast<AMS_TCB *>(tcb))
				);
		const SHeader *header = reinterpret_cast<const SHeader *>(work->data);

		//オーナー情報確認
		if (header->owner == tcb) {
			//オーナー情報が一致していたら
			return reinterpret_cast<const ITaskWork *>(&header[1]);
		}
	}
	return NULL;
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
// ============================================================================
// ITaskWork::GetTaskTcb
/*!
	タスクTCB(aliceベース)を取得する

	@return タスクTCB(aliceベース)
 */
// ============================================================================
const ::AMS_TCB *ITaskWork::GetTaskTcb() const
{
	SHeader *header = reinterpret_cast<SHeader *>(
							reinterpret_cast<unsigned long>(this) - sizeof(SHeader)
											);
	return header->owner;
}

//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// ============================================================================
// ITaskWork::procedure
/*!
	タスクのプロシージャ関数
	 
	@param	tcb	[in]	処理するタスクTCB(aliceベース)
 */
// ============================================================================
void ITaskWork::procedure(::AMS_TCB *tcb)
{
	SWork *work = reinterpret_cast<SWork *>(::amTaskGetWork(tcb));
	SHeader *header = reinterpret_cast<SHeader *>(work->data);
	ITaskWork &it = *reinterpret_cast<ITaskWork *>(&header[1]);

	it();
}

// ============================================================================
// ITaskWork::destructor
/*!
	タスクのデストラクタ関数
	 
	@param	tcb	[in]	削除されるタスクTCB(aliceベース)

	@note
		::mtTaskClear 系から削除された時に delete を呼ぶ
		
		この関数は終了時に確実に呼ばれる分けではありません。
		::mtTaskClear 系に因る終了では呼ばれますが、
		delete に因る終了では呼ばれません。
		終了時に確実に実行したい命令は delete で行って下さい。
 */
// ============================================================================
void ITaskWork::destructor(::AMS_TCB *tcb)
{
	SWork *work = reinterpret_cast<SWork *>(::amTaskGetWork(tcb));
	SHeader *header = reinterpret_cast<SHeader *>(work->data);
	ITaskWork *it = reinterpret_cast<ITaskWork *>(&header[1]);

	header->owner = NULL;
	delete it;
}



//------------------------------------------------------------------------------**********














//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// ITaskLink::AttachTask
/*!
	タスクリストへ登録

	@param	name		[in]	タスク名
	@param	priority	[in]	優先度 (0x0000 ～ 0xFFFF)
	@param	user		[in]	所有者 (0:所有者設定なし(グループ削除対象外))
	@param	attribute	[in]	属性
	@param	group		[in]	グループID
	@param	stall_mask	[in]	実行待機グループマスク
	@param	run_mask	[in]	実行可能スレッドマスク
 */
// ============================================================================
#if defined(MTD_DEBUG)
void ITaskLink::AttachTask(const char *name
					, TPriority priority, TUser user, TAttribute attribute
					, TGroup group, TStallMask stall_mask, TRunMask run_mask)
{
	DetachTask();
	//登録
	m_task_tcb = amTaskMake(procedure, destructor
					, priority, user, attribute
					, const_cast<char *>(name)	//要修正
					, stall_mask, group, run_mask);
	SWork *header = reinterpret_cast<SWork *>(::amTaskGetWork(m_task_tcb));
	header->owner = this;
}
#else	//#if defined(MTD_DEBUG)
void ITaskLink::AttachTask(TPriority priority, TUser user, TAttribute attribute
					, TGroup group, TStallMask stall_mask, TRunMask run_mask)
{
	DetachTask();
	//登録
	m_task_tcb = amTaskMake(procedure, destructor
					, priority, user, attribute
					, ""
					, stall_mask, group, run_mask);
	SWork *header = reinterpret_cast<SWork *>(::amTaskGetWork(m_task_tcb));
	header->owner = this;
}
#endif	//#if defined(MTD_DEBUG)

// ============================================================================
// ITaskLink::DetachTask
/*!
	タスクリストから脱退
 */
// ============================================================================
void ITaskLink::DetachTask() {
	if (NULL != m_task_tcb) {
		//起動中
		::amTaskSetDestructor(m_task_tcb, NULL);
		::amTaskDelete(m_task_tcb);
		TaskDestructor(EDestructorCbType::DetachTask);
		m_task_tcb = NULL;
	}
}

// ============================================================================
// ITaskLink::CastFromTaskTcb
/*!
	タスクTCB(Cベース)をITaskLinkにキャストします。

	@param	tcb	[in]	タスクTCB(Cベース)

	@return ITaskLinkへのポインタ or NULL

	ITaskLink から派生していないタスクをキャストしようとすると失敗します。
	失敗した場合は NULL を返します(アサートはありません)。
 */
// ============================================================================
const ITaskLink *ITaskLink::CastFromTaskTcb(const ::AMS_TCB *tcb)
{
	//キャスト可能確認
	if ((ITaskLink::procedure == tcb->procedure)
			||
		(ITaskLink::destructor == tcb->destructor)
		) {
		//プロシージャかデストラクタがITaskLink準拠なら
		const SWork *work = reinterpret_cast<const SWork *>(
						::amTaskGetWork(const_cast<AMS_TCB *>(tcb))
				);

		//オーナー情報確認
		if ((NULL != work->owner) && (work->owner->m_task_tcb == tcb)) {
			//オーナー情報が一致していたら
			return reinterpret_cast<const ITaskLink *>(work->owner);
		}
	}
	return NULL;
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// ============================================================================
// ITaskLink::TcbLinkDestructorCb
/*!
	タスク消失時のコールバック

	@note
		タスクマネージャ側からの要請で切断された場合に発生します。

		この関数はmtTaskのデストラクタ内で実行される場合があります。
 */
// ============================================================================
void ITaskLink::TcbLinkDestructorCb()
{
	TaskDestructor(EDestructorCbType::MtTaskClear);
	m_task_tcb = NULL;
}

// ============================================================================
// ITaskLink::procedure
/*!
	タスクのプロシージャ関数
	 
	@param	tcb	[in]	処理するタスクTCB(aliceベース)
 */
// ============================================================================
void ITaskLink::procedure(::AMS_TCB *tcb)
{
	SWork *work = reinterpret_cast<SWork *>(::amTaskGetWork(tcb));
	ITaskLink &it = *work->owner;

	it();
}

// ============================================================================
// ITaskLink::destructor
/*!
	タスクのデストラクタ関数
	 
	@param	tcb	[in]	削除されるタスクTCB(Cベース)

	@note
		::mtTaskClear 系から削除された時に TcbLinkDestructorCb を呼ぶ
		
		この関数は終了時に確実に呼ばれる分けではありません。
		::mtTaskClear 系に因る終了では呼ばれますが、
		DetachTask に因る終了では呼ばれません。
 */
// ============================================================================
void ITaskLink::destructor(::AMS_TCB *tcb)
{
	SWork *work = reinterpret_cast<SWork *>(::amTaskGetWork(tcb));
	ITaskLink &it = *work->owner;

	it.TcbLinkDestructorCb();
}


//------------------------------------------------------------------------------**********












//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// CTaskCb::AttachTask
/*!
	タスクリストへ登録

	@param	name		[in]	タスク名
	@param	data		[in]	データ
	@param	func		[in]	コールバック関数
	@param	destructor	[in]	タスク切断時のコールバック関数
	@param	priority	[in]	優先度 (0x0000 ～ 0xFFFF)
	@param	user		[in]	所有者 (0:所有者設定なし(グループ削除対象外))
	@param	attribute	[in]	属性
	@param	group		[in]	グループID
	@param	stall_mask	[in]	実行待機グループマスク
	@param	run_mask	[in]	実行可能スレッドマスク
 */
// ============================================================================
#if defined(MTD_DEBUG)
void CTaskCb<>::AttachTask(const char *name, void *data, FFunc func, FFunc destructor
					, TPriority priority, TUser user, TAttribute attribute
					, TGroup group, TStallMask stall_mask, TRunMask run_mask) {
	ITaskLink::AttachTask(name
					, priority, user, attribute, group, stall_mask, run_mask);
	m_target = data;
	m_func = func;
	m_destructor = destructor;
}
#else	//#if defined(MTD_DEBUG)
void CTaskCb<>::AttachTask(void *data, FFunc func, FFunc destructor
					, TPriority priority, TUser user, TAttribute attribute
					, TGroup group, TStallMask stall_mask, TRunMask run_mask) {
	ITaskLink::AttachTask(priority, user, attribute, group, stall_mask, run_mask);
	m_target = data;
	m_func = func;
	m_destructor = destructor;
}
#endif	//#if defined(MTD_DEBUG)


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// ============================================================================
// CTaskCb<>::TaskDestructor
/*!
	タスク消失時に実行される関数

	@note
		DetachTaskが呼ばれた場合に、
		タスクマネージャ側からの要請で切断された場合に発生します。

		この関数はmtTaskのデストラクタ内で実行される場合があります。
 */
// ============================================================================
void CTaskCb<>::TaskDestructor(EDestructorCbType::Type type) {
	if (NULL != m_destructor) {
		m_destructor(m_target);
	}
	Clear();
	UNREFERENCED_PARAMETER(type);
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
	@param	data		[in]	データ
	@param	func		[in]	コールバック関数
	@param	destructor	[in]	タスク切断時のコールバック関数
	@param	priority	[in]	タスク優先度
	@param	group		[in]	タクスグループ
	@param	flag		[in]	タスクフラグ
	@param	pause_level	[in]	タスクポーズレベル
 */
// ============================================================================
#if defined(MTD_DEBUG)
void CTaskCbIdx<>::AttachTask(const char *name, void *data, FFunc func, FFunc destructor
				, u16 priority, u8 group, u16 flag, u8 pause_level) {
	ITaskLink::AttachTask(name, priority, group, flag, pause_level);
	m_target = data;
	m_func = func;
	m_destructor = destructor;
}
#else	//#if defined(MTD_DEBUG)
void CTaskCbIdx<>::AttachTask(void *data, FFunc func, FFunc destructor
				, u16 priority, u8 group, u16 flag, u8 pause_level) {
	ITaskLink::AttachTask(priority, group, flag, pause_level);
	m_target = data;
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
CTaskCbIdx<>::CTaskCbIdx(u32 idx) : ITaskLink()
{
	Clear();
	m_idx =idx;
}

// ============================================================================
// CTaskCbIdx::CTaskCbIdx
/*!
	タスクリストへ登録するコンストラクタ

	@param	idx			[in]	インデックス
	@param	name		[in]	タスク名
	@param	data		[in]	データ
	@param	func		[in]	コールバック関数
	@param	destructor	[in]	タスク切断時のコールバック関数
	@param	priority	[in]	タスク優先度
	@param	group		[in]	タクスグループ
	@param	flag		[in]	タスクフラグ
	@param	pause_level	[in]	タスクポーズレベル
 */
// ============================================================================
CTaskCbIdx<>::CTaskCbIdx(u32 idx, const char *name
				, void *data, FFunc func, FFunc destructor
				, u16 priority, u8 group, u16 flag, u8 pause_level) : ITaskLink()
{
	Clear();
	m_idx =idx;
	AttachTask(name, data, func, destructor, priority, group, flag, pause_level);
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// ============================================================================
// CTaskCbIdx<>::TaskDestructor
/*!
	タスク消失時に実行される関数

	@note
		DetachTaskが呼ばれた場合に、
		タスクマネージャ側からの要請で切断された場合に発生します。

		この関数はmtTaskのデストラクタ内で実行される場合があります。
 */
// ============================================================================
void CTaskCbIdx<>::TaskDestructor(EDestructorCbType::Type type) {
	if (NULL != m_destructor) {
		m_destructor(m_target, m_idx);
	}
	Clear();
}


//------------------------------------------------------------------------------**********
#endif //#if ERD_TASK_WEAK_PORTING












#if ERD_TASK_WEAK_PORTING
// ============================================================================
// Clear
/*!
	タスクの一括クリア
 
	@param	judge	[in]	クリア判断用関数オブジェクト

	@note
		//一般的なタスククリア
		Clear()                  == ::mtTaskClearTcbAll()
		Clear(CGroup(group))     == ::mtTaskClearTcbGroup(group)
		Clear(CPrio(begin, end)) == ::mtTaskClearTcbPriority(begin, end)

		//応用的なタスククリア
		Clear(CAnd(CPrio(begin, end), CGroup(group)))
		                               == begin～end 内の group のみ削除
		Clear(CNot(CGroup(group)))     == group のみ存続
		Clear(CNot(CPrio(begin, end))) == begin～end のみ存続
 */
// ============================================================================
void Clear(IClearJudge judge)
{
	::MTS_TASK_TCB	*tcb;
	::MTS_TASK_TCB	*tcb_next;

	//自身のTCBを取得する
	tcb = ::mtTaskGetOwnTcb();
	//先頭(MainTcb)まで遡る
	while (NULL != tcb->tcb_prev) {
		tcb = tcb->tcb_prev;
	}

	//処理開始
	do {
		//システムTCB・茉理ライブラリTCB確認
		if (MTD_TASK_TCB_GROPU_SYSTEM == tcb->group) {
			//システムTCBなら
			//省略
			tcb = tcb->tcb_next;
			continue;
		} else if (MTD_TASK_TCB_GROPU_MATSURI == tcb->group) {
			//茉理ライブラリTCBなら
			//省略
			tcb = tcb->tcb_next;
			continue;
		}

		//通常タスクなら
		tcb_next = tcb->tcb_next;
		ITask *i_tcb;
		if (NULL != (i_tcb = ITaskWork::CastFromTaskTcb(tcb))) {
			//ITaskWorkへのキャスト成功
			if (judge(*i_tcb)) {
				delete i_tcb;
			}
		} else if (NULL != (i_tcb = ITaskLink::CastFromTaskTcb(tcb))) {
			//ITaskLinkへのキャスト成功
			if (judge(*i_tcb)) {
				delete i_tcb;
			}
		} else {
			//ITaskWorkへのキャスト失敗
			if (judge(tcb)) {
				::mtTaskClearTcb(tcb);
			}
		}

		tcb = tcb_next;
	} while (NULL != tcb);
}
#endif //#if ERD_TASK_WEAK_PORTING










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
