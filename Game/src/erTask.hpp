// ============================================================================
/*!
	@file	erTask.hpp
	@brief	ERフレームワーク・タスク

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2007-2009 Dimps
	$Id: erTask.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page erTaskMain ERフレームワーク・タスク

	@section erTaskSummary 概要
		ERフレームワーク上でのタスク機能を使用出来る様にします。

		クラス継承図                                     \n
			ITask                     △継承(is-a)       \n
			  △△                    ◆委譲(has-a)      \n
			  │└─────┐                           \n
			  │            │                           \n
			ITaskWork    ITaskLink    CProc<>            \n
			  △          △    △      △               \n
			  │          │    │      │               \n
			  │      CTaskCb<> │   CProcCount<>        \n
			  │                │      △               \n
			  │┌───────┼───┘               \n
			CTask<>             │                       \n
			           CTaskCbIdx<>                      \n
			            ◆                               \n
			            │                               \n
			ITaskMulti  │                               \n
			  △        │                               \n
			  │        │                               \n
			ITaskLinkMulti<>         CProcMulti<>        \n
			  △                        △               \n
			  │┌───────────┘               \n
			CTaskMulti<>                                 \n
			                                             \n
			                                             \n
			                                             \n
			IClearJudge                                  \n
			    △  ◆                                   \n
			    │  └───────┬──┬──┐       \n
			  ┌┴─┬───┬──┬┼─┬┼─┐│       \n
			  │    │      │    ││  ││  ││       \n
			CAll  CGroup  CPrio   CAnd  COr   CNot       \n


		指南書
			スタート
			  ↓
			タスクを消したい
			  │N       Y └→ IClearJudge 系
			  ↓
			基本的なタスクが使いたい
			  │N       Y └→ CTask<>
			  ↓
			タスクに繋がなくて良いからプロシージャの機能だけ欲しい
			  │N       Y └→ CProc
			  ↓
			プロシージャは要らないけどメンバ関数を毎フレーム呼んで欲しい
			  │N       Y └→ ITaskWork
			  ↓
			自作のプロシージャを毎フレーム呼んで欲しい
			  │N       Y └→ CTask<自作プロシージャ>
			  ↓
			クラスのメモリ領域をタスクワークとは別にしたい
			  │N       Y └→ ITaskLink
			  ↓
			継承(is-a)では無くて、委譲(has-a)で使いたい
			  │N       Y └→ CTaskCb
			  ↓
			プロシージャを複数持ちたい
			  │N       Y └→ CTask<CProcMulti<継承先クラス, 数>>
			  ↓
			複数のタスクプライオリティを持ちたい
			  │N       Y └→ CTaskMulti<継承先クラス, 数> か
			  │               CTask と CTaskMulti の多重継承
			  ↓
			erTask ではお役に立てないかと思われます


		erTaskの使用について
			テンプレートに依りカスタマイズ性を持たせていますが、
			基本的に CTask<> が多用される事を想定して設計しています。
			波瀾に富んだ使用は、ソースコードの肥大を招きます。
			(出来る限り肥大しない様に努めてはいますが…。)

			hpp・cpp ファイルの他に tpp ファイルがありますが、
			このファイルは普段は見る必要は有りません。
			erTask を使用する上では hpp だけ見れば十分です

			tpp は cpp 相当ですので、
			内部の詳細な挙動を把握したい時に参照して下さい。
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
#include "accelMpl.hpp"
#include "alice.h"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace er {
namespace task {











//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タスクインターフェース
		タスクに関するインターフェースを定義したクラス
 */
class ITask {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	typedef void (ITask::*FProc)();
	typedef unsigned long	TPriority;		//<優先度 (0x0000～0xFFFF)
	typedef unsigned long	TUser;			//<所有者
	typedef unsigned long	TAttribute;		//<属性
	typedef signed long		TGroup;			//<グループID
	typedef unsigned long	TStallMask;		//<実行待機グループマスク
	typedef unsigned long	TRunMask;		//<実行可能スレッドマスク

	static const TPriority	c_priority_default		= 0x1000;
	static const TUser		c_user_default			= 0;
	static const TAttribute	c_attribute_default		= ::AMD_TASK_ATTR_MAIN;
	static const TGroup		c_group_default			= 0;
	static const TStallMask	c_stall_mask_default	= 1;
	static const TRunMask	c_run_mask_default		= 0xFFFFFFFF;


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// ITask::operator()
	/*!
		フレーム毎に呼び出される関数
	 */
	// ============================================================================
	virtual void operator()() = 0;

	// ============================================================================
	// ITask::operator cast
	/*!
		::AMS_TCB *へのキャスト

		@return	キャスト後のデータ
	 */
	// ============================================================================
	operator ::AMS_TCB *() {
		return GetTaskTcb();
	}
	operator const ::AMS_TCB *() const {
		return GetTaskTcb();
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// ITask::GetTaskTcb
	/*!
		タスクTCB(aliceベース)を取得する

		@return タスクTCB(aliceベース)
	 */
	// ============================================================================
	virtual const ::AMS_TCB *GetTaskTcb() const = 0;
	::AMS_TCB *GetTaskTcb() {
		return const_cast<AMS_TCB *>(
						const_cast<const ITask *>(this)->GetTaskTcb()
					);
	}

	// ============================================================================
	// ITask::GetPriority
	/*!
		プライオリティを取得する

		@return プライオリティ
	 */
	// ============================================================================
	TPriority GetPriority() const {
		return GetTaskTcb()->priority;
	}

	// ============================================================================
	// ITask::GetUser
	/*!
		所有者を取得する

		@return 所有者
	 */
	// ============================================================================
	TUser GetUser() const {
		return GetTaskTcb()->user_id;
	}

	// ============================================================================
	// ITask::GetAttribute
	/*!
		属性を取得する

		@return フラグ
	 */
	// ============================================================================
	TAttribute GetAttribute() const {
		return GetTaskTcb()->attribute;
	}

	// ============================================================================
	// ITask::GetGroup
	/*!
		グループを取得する

		@return 状態
	 */
	// ============================================================================
	TGroup GetGroup() const {
#if 1 < AMD_TASK_THREAD_NUM
		return GetTaskTcb()->group_id;
#else //#if 1 < AMD_TASK_THREAD_NUM
		return c_group_default;
#endif //#if 1 < AMD_TASK_THREAD_NUM
	}

	// ============================================================================
	// ITask::GetStallMask
	/*!
		実行待機グループマスクを取得する

		@return 実行待機グループマスク
	 */
	// ============================================================================
	TStallMask GetStallMask() const {
#if 1 < AMD_TASK_THREAD_NUM
		return GetTaskTcb()->stall_group;
#else //#if 1 < AMD_TASK_THREAD_NUM
		return c_stall_mask_default;
#endif //#if 1 < AMD_TASK_THREAD_NUM
	}

	// ============================================================================
	// ITask::GetRunMask
	/*!
		実行待機グループマスクを取得する

		@return 実行待機グループマスク
	 */
	// ============================================================================
	TRunMask GetRunMask() const {
#if 1 < AMD_TASK_THREAD_NUM
		return GetTaskTcb()->run_thread;
#else //#if 1 < AMD_TASK_THREAD_NUM
		return c_run_mask_default;
#endif //#if 1 < AMD_TASK_THREAD_NUM
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// ITask::ITask
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
protected:
	ITask() {}

	// ============================================================================
	// ITask::~ITask
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~ITask() {}


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
};		//class ITask












//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タスクワークインターフェース
		メモリ管理と、削除等の基本機能のみを定義したクラス
		このクラスを継承したクラスは自動でタスクとして扱われます。
		毎フレーム operator() が呼び出されます。
		
		負荷的にはポインタの解決が１つ、仮想関数の解決が１つ増えます。
		それとは別にワーク領域の取得が毎フレーム必ず発生します。

		デストラクタ内で他のタスククラスを削除するのは推奨しません。
		deleteによりデストラクタが発動した場合は正常に処理されますが、
		::mtTaskClear 系による発動では ASSERT で停止する為です。
 */
class ITaskWork : public ITask {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// ITaskWork::operator new
	/*!
		タスクコア用newオペレータオーバーロード
	 
		@param	work_size	[in]	クラスサイズ
		@param	name		[in]	タスク名
		@param	priority	[in]	優先度 (0x0000 ～ 0xFFFF)
		@param	user		[in]	所有者 (0:所有者設定なし(グループ削除対象外))
		@param	attribute	[in]	属性
		@param	group		[in]	グループID
		@param	stall_mask	[in]	実行待機グループマスク
		@param	run_mask	[in]	実行可能スレッドマスク
	 
		@return	確保したメモリのポインタ
	 */
	// ============================================================================
#if defined(MTD_DEBUG)
	static void *new_(size_t work_size, const char *name
					, TPriority priority, TUser user, TAttribute attribute
					, TGroup group, TStallMask stall_mask, TRunMask run_mask);
	void *operator new(size_t work_size, const char *name
					, TPriority priority	= c_priority_default
					, TUser user			= c_user_default
					, TAttribute attribute	= c_attribute_default
					, TGroup group			= c_group_default
					, TStallMask stall_mask	= c_stall_mask_default
					, TRunMask run_mask		= c_run_mask_default) {
		return new_(work_size, name
					, priority, user, attribute, group, stall_mask, run_mask);
	}
#else	//#if defined(MTD_DEBUG)
	static void *new_(size_t work_size
					, TPriority priority, TUser user, TAttribute attribute
					, TGroup group, TStallMask stall_mask, TRunMask run_mask);
	void *operator new(size_t work_size, const char *
					, TPriority priority	= c_priority_default
					, TUser user			= c_user_default
					, TAttribute attribute	= c_attribute_default
					, TGroup group			= c_group_default
					, TStallMask stall_mask	= c_stall_mask_default
					, TRunMask run_mask		= c_run_mask_default) {
		return new_(work_size
					, priority, user, attribute, group, stall_mask, run_mask);
	}
#endif	//#if defined(MTD_DEBUG)

	// ============================================================================
	// ITaskWork::operator delete
	/*!
		タスクコア用deleteオペレータオーバーロード
	 
		@param	p	[in]	削除するポインタ
	 */
	// ============================================================================
	static void operator delete(void *p);

	// ============================================================================
	// ITaskWork::operator new
	/*!
		タスクコア用deleteオペレータオーバーロード(new時例外)
	 
		@param	p	[in]	削除するポインタ
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
	static void operator delete(void *p, const char *name
					, TPriority priority, TUser user, TAttribute attribute
					, TGroup group, TStallMask stall_mask, TRunMask run_mask);

	// ============================================================================
	// ITaskWork::operator()
	/*!
		フレーム毎に呼び出される関数
	 */
	// ============================================================================
	virtual void operator()() = 0;

	// ============================================================================
	// ITaskWork::CastFromTaskTcb
	/*!
		タスクTCB(Cベース)をITaskWorkにキャストします。

		@param	tcb	[in]	タスクTCB(Cベース)

		@return ITaskWorkへのポインタ or NULL

		@note
			ITaskWork から派生していないタスクをキャストしようとすると失敗します。
			失敗した場合は NULL を返します(アサートはありません)。
	 */
	// ============================================================================
	static const ITaskWork *CastFromTaskTcb(const ::AMS_TCB *tcb);
	static ITaskWork *CastFromTaskTcb(::AMS_TCB *tcb) {
		return const_cast<ITaskWork *>(CastFromTaskTcb(tcb));
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// ITaskWork::GetTaskTcb
	/*!
		タスクTCB(Cベース)を取得する

		@return タスクTCB(Cベース)
	 */
	// ============================================================================
	const ::AMS_TCB *GetTaskTcb() const;


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// ITaskWork::ITaskWork
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
protected:
	ITaskWork() : ITask() {}

	// ============================================================================
	// ITaskWork::ITaskWork
	/*!
		コピーコンストラクタ・コピー演算子

		@param	src	[in]	コピー元
	 */
	// ============================================================================
protected:
	ITaskWork(const ITaskWork &src);
	ITaskWork &operator=(const ITaskWork &src);

	// ============================================================================
	// ITaskWork::~ITaskWork
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~ITaskWork() {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//タスクTCB(aliceベース)のワークから所属しているタスクTCB(aliceベース)を把握する
	struct SHeader {
		::AMS_TCB	*owner;
	};

	//タスクTCB(aliceベース)のワーク
	struct SWork {
		void *data;
	};


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	// ============================================================================
	// ITaskWork::operator new[]
	/*!
		タスクコア用new[]オペレータオーバーロード
	 
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
			使用禁止
	 */
	// ============================================================================
	void *operator new[](size_t work_size, const char *name
					, TPriority priority	= c_priority_default
					, TUser user			= c_user_default
					, TAttribute attribute	= c_attribute_default
					, TGroup group			= c_group_default
					, TStallMask stall_mask	= c_stall_mask_default
					, TRunMask run_mask		= c_run_mask_default);

	// ============================================================================
	// ITaskWork::operator delete[]
	/*!
		タスクコア用delete[]オペレータオーバーロード
	 
		@param	p	[in]	削除するポインタ

		@note
			使用禁止
	 */
	// ============================================================================
	static void operator delete[](void *p);

	// ============================================================================
	// ITaskWork::procedure
	/*!
		タスクのプロシージャ関数
	 
		@param	tcb	[in]	処理するタスクTCB(aliceベース)
	 */
	// ============================================================================
	static void procedure(::AMS_TCB *tcb);

	// ============================================================================
	// ITaskWork::destructor
	/*!
		タスクのデストラクタ関数
	 
		@param	tcb	[in]	削除されるタスクTCB(aliceベース)
	 */
	// ============================================================================
	static void destructor(::AMS_TCB *tcb);


//------------------------------------------------------------------------------**********
};		//class ITaskWork












//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タスクリンクインターフェース
		タスク機能を外付けする為のクラス
		このクラスを継承したクラスはタスクリストに接続する事が出来ます。
		タスクリストに接続した場合、毎フレーム operator() が呼び出されます。

		ITaskWork と違い、実態はタスクとは別に確保しています。
		その為、タスクリストからの脱退がメモリ領域の消失に繋がりません。

		負荷的にはポインタの解決が１つ、仮想関数の解決が１つ増えます。
		それとは別にワーク領域の取得が毎フレーム必ず発生します。
*/
class ITaskLink : public ITask {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
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
	void AttachTask(const char *name
					, TPriority priority	= c_priority_default
					, TUser user			= c_user_default
					, TAttribute attribute	= c_attribute_default
					, TGroup group			= c_group_default
					, TStallMask stall_mask	= c_stall_mask_default
					, TRunMask run_mask		= c_run_mask_default);
#else	//#if defined(MTD_DEBUG)
	void AttachTask(TPriority priority	= c_priority_default
					, TUser user			= c_user_default
					, TAttribute attribute	= c_attribute_default
					, TGroup group			= c_group_default
					, TStallMask stall_mask	= c_stall_mask_default
					, TRunMask run_mask		= c_run_mask_default);
	void AttachTask(const char *
					, TPriority priority	= c_priority_default
					, TUser user			= c_user_default
					, TAttribute attribute	= c_attribute_default
					, TGroup group			= c_group_default
					, TStallMask stall_mask	= c_stall_mask_default
					, TRunMask run_mask		= c_run_mask_default) {
		AttachTask(priority, user, attribute, group, stall_mask, run_mask);
	}
#endif	//#if defined(MTD_DEBUG)

	// ============================================================================
	// ITaskLink::DetachTask
	/*!
		タスクリストから脱退
	 */
	// ============================================================================
	void DetachTask();

	// ============================================================================
	// ITaskLink::operator()
	/*!
		フレーム毎に呼び出される関数
	 */
	// ============================================================================
	virtual void operator()() = 0;

	// ============================================================================
	// ITaskLink::CastFromTaskTcb
	/*!
		タスクTCB(Cベース)をITaskLinkにキャストします。

		@param	tcb	[in]	タスクTCB(aliceベース)

		@return ITaskLinkへのポインタ or NULL

		ITaskLink から派生していないタスクをキャストしようとすると失敗します。
		失敗した場合は NULL を返します(アサートはありません)。
	 */
	// ============================================================================
	static const ITaskLink *CastFromTaskTcb(const ::AMS_TCB *tcb);
	static ITaskLink *CastFromTaskTcb(::AMS_TCB *tcb) {
		return const_cast<ITaskLink *>(CastFromTaskTcb(tcb));
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// ITaskLink::GetTaskTcb
	/*!
		タスクTCB(aliceベース)を取得する

		@return タスクTCB(aliceベース)

		@note
			IsTask が FALSE を返す時は NULL を返します。
	 */
	// ============================================================================
	const ::AMS_TCB *GetTaskTcb() const {
		return m_task_tcb;
	}

	// ============================================================================
	// ITaskLink::IsTask
	/*!
		タスク中の確認

		@retval	true:	タスク中
		@retval	false:	タスク中ではない
	 */
	// ============================================================================
	bool IsTask() {
		return ((NULL != m_task_tcb)? true: false);
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// ITaskLink::ITaskLink
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	ITaskLink() : ITask(), m_task_tcb(NULL) {}

	// ============================================================================
	// ITaskLink::ITaskLink
	/*!
		コピーコンストラクタ・コピー演算子

		@param	src	[in]	コピー元
		
		@note
			コピー可能な様に実装出来ない訳ではありませんが、
			制限の厳しいITaskWorkに合わせています
	 */
	// ============================================================================
protected:
	ITaskLink(const ITaskLink &src);
	ITaskLink &operator=(const ITaskLink &src);

	// ============================================================================
	// ITaskLink::~ITaskLink
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~ITaskLink() {
		DetachTask();
	}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
	//タスクマネージャから切断された時に呼び出し先を特定する
	struct EDestructorCbType {
		enum Type {
			DetachTask = 0,		//DetachTaskからの呼び出し
			MtTaskClear,		//mtTaskClear系からの呼び出し
			Max,
			None
		};
	};
private:
	//タスクTCB(Cベース)のワーク
	struct SWork {
		ITaskLink	*owner;
	};


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	::AMS_TCB	*m_task_tcb;


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
	// ============================================================================
	// ITaskLink::TaskDestructor
	/*!
		タスク消失時に実行される関数

		@note
			DetachTaskが呼ばれた場合に、
			タスクマネージャ側からの要請で切断された場合に発生します。

			この関数はmtTaskのデストラクタ内で実行される場合があります。
	 */
	// ============================================================================
	virtual void TaskDestructor(EDestructorCbType::Type type) {
		UNREFERENCED_PARAMETER(type);
	}

private:
	// ============================================================================
	// ITaskLink::TcbLinkDestructorCb
	/*!
		タスク消失時のコールバック

		@note
			DetachTaskが呼ばれた場合に、
			タスクマネージャ側からの要請で切断された場合に発生します。

			この関数はmtTaskのデストラクタ内で実行される場合があります。
	 */
	// ============================================================================
	void TcbLinkDestructorCb();

	// ============================================================================
	// ITaskLink::procedure
	/*!
		タスクのプロシージャ関数
	 
		@param	tcb	[in]	処理するタスクTCB(aliceベース)
	 */
	// ============================================================================
	static void procedure(::AMS_TCB *tcb);

	// ============================================================================
	// ITaskLink::destructor
	/*!
		タスクのデストラクタ関数
	 
		@param	tcb	[in]	削除されるタスクTCB(aliceベース)
	 */
	// ============================================================================
	static void destructor(::AMS_TCB *tcb);


//------------------------------------------------------------------------------**********
};		//class ITaskLink













//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タスクコールバッククラス
		継承せずにタスク機能を外付けする為のクラス
		このクラスはタスクリストに接続する事が出来ます。
		タスクリストに接続した場合、毎フレーム指定されたクラスの、
		指定されたメンバ関数が呼び出されます。

		ITaskLink は継承(is-a)して使用しますが、このクラスは委譲(has-a)して使用します。

		負荷的にはITaskLinkから追加で、
		ポインタの解決が１つ、関数ポインタの解決が１つ増えます。
*/
template<typename TType = accel::mpl::CNullType>
class CTaskCb : public ITaskLink {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	template<typename TReqType>
	struct Gene {
		typedef CTaskCb<TReqType>	Type;
	};
	typedef TType	Type;			//委譲クラス
	typedef void (TType::*FFunc)();	//呼び出すメンバ関数


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
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
	void AttachTask(const char *name, TType &owner, FFunc func, FFunc destructor = NULL
					, TPriority priority	= c_priority_default
					, TUser user			= c_user_default
					, TAttribute attribute	= c_attribute_default
					, TGroup group			= c_group_default
					, TStallMask stall_mask	= c_stall_mask_default
					, TRunMask run_mask		= c_run_mask_default);
#else	//#if defined(MTD_DEBUG)
	void AttachTask(TType &owner, FFunc func, FFunc destructor = NULL
					, TPriority priority	= c_priority_default
					, TUser user			= c_user_default
					, TAttribute attribute	= c_attribute_default
					, TGroup group			= c_group_default
					, TStallMask stall_mask	= c_stall_mask_default
					, TRunMask run_mask		= c_run_mask_default);
	void AttachTask(const char *name, TType &owner, FFunc func, FFunc destructor = NULL
					, TPriority priority	= c_priority_default
					, TUser user			= c_user_default
					, TAttribute attribute	= c_attribute_default
					, TGroup group			= c_group_default
					, TStallMask stall_mask	= c_stall_mask_default
					, TRunMask run_mask		= c_run_mask_default) {
		AttachTask(owner, func, destructor
				, priority, user, attribute, group, stall_mask, run_mask);
	}
#endif	//#if defined(MTD_DEBUG)


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CTaskCb::CTaskCb
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CTaskCb() : ITaskLink() {
		Clear();
	}

	// ============================================================================
	// CTaskCb::CTaskCb
	/*!
		タスクリストへ登録するコンストラクタ

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
public:
	CTaskCb(const char *name, TType &owner, FFunc func, FFunc destructor = NULL
					, TPriority priority	= c_priority_default
					, TUser user			= c_user_default
					, TAttribute attribute	= c_attribute_default
					, TGroup group			= c_group_default
					, TStallMask stall_mask	= c_stall_mask_default
					, TRunMask run_mask		= c_run_mask_default) : ITaskLink() {
		AttachTask(name, owner, func, destructor
					, priority, user, attribute, group, stall_mask, run_mask);
	}

	// ============================================================================
	// CTaskCb::~CTaskCb
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~CTaskCb() {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	TType	*m_target;		//呼び出すメンバ関数の所属するクラス
	FFunc	m_func;			//呼び出すメンバ関数
	FFunc	m_destructor;	//デストラクタ作動時に呼び出すメンバ関数


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	// ============================================================================
	// CTaskCb::operator()
	/*!
		フレーム毎に呼び出される関数
	 */
	// ============================================================================
	void operator()() {
		(m_target->*m_func)();
	}

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
	void TaskDestructor(EDestructorCbType::Type type);

	// ============================================================================
	// CTaskCb::Clear
	/*!
		設定クリア
	 */
	// ============================================================================
	void Clear() {
		m_target = NULL;
		m_destructor = m_func = NULL;
	}


//------------------------------------------------------------------------------**********
};		//class CTaskCb<typename TType = accel::mpl::CNullType>

//------------------------------------------------------------------------------**********

template<>
class CTaskCb<accel::mpl::CNullType> : public ITaskLink {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	template<typename TReqType>
	struct Gene {
		typedef CTaskCb<TReqType>	Type;
	};
	typedef void *Type;			//データ
	typedef void (*FFunc)(void *data);	//呼び出すメンバ関数


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
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
	void AttachTask(const char *name, void *data, FFunc func, FFunc destructor = NULL
					, TPriority priority	= c_priority_default
					, TUser user			= c_user_default
					, TAttribute attribute	= c_attribute_default
					, TGroup group			= c_group_default
					, TStallMask stall_mask	= c_stall_mask_default
					, TRunMask run_mask		= c_run_mask_default);
#else	//#if defined(MTD_DEBUG)
	void AttachTask(void *data, FFunc func, FFunc destructor = NULL
					, TPriority priority	= c_priority_default
					, TUser user			= c_user_default
					, TAttribute attribute	= c_attribute_default
					, TGroup group			= c_group_default
					, TStallMask stall_mask	= c_stall_mask_default
					, TRunMask run_mask		= c_run_mask_default);
	void AttachTask(const char *name, void *data, FFunc func, FFunc destructor = NULL
					, TPriority priority	= c_priority_default
					, TUser user			= c_user_default
					, TAttribute attribute	= c_attribute_default
					, TGroup group			= c_group_default
					, TStallMask stall_mask	= c_stall_mask_default
					, TRunMask run_mask		= c_run_mask_default) {
		AttachTask(data, func, destructor
				, priority, user, attribute, group, stall_mask, run_mask);
		UNREFERENCED_PARAMETER(name);
	}
#endif	//#if defined(MTD_DEBUG)


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CTaskCb::CTaskCb
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CTaskCb() : ITaskLink() {
		Clear();
	}

	// ============================================================================
	// CTaskCb::CTaskCb
	/*!
		タスクリストへ登録するコンストラクタ

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
public:
	CTaskCb(const char *name, void *data, FFunc func, FFunc destructor = NULL
					, TPriority priority	= c_priority_default
					, TUser user			= c_user_default
					, TAttribute attribute	= c_attribute_default
					, TGroup group			= c_group_default
					, TStallMask stall_mask	= c_stall_mask_default
					, TRunMask run_mask		= c_run_mask_default) : ITaskLink() {
		AttachTask(name, data, func, destructor
					, priority, user, attribute, group, stall_mask, run_mask);
	}

	// ============================================================================
	// CTaskCb::~CTaskCb
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~CTaskCb() {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	void	*m_target;		//呼び出すメンバ関数の所属するクラス
	FFunc	m_func;			//呼び出すメンバ関数
	FFunc	m_destructor;	//デストラクタ作動時に呼び出すメンバ関数


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	// ============================================================================
	// CTaskCb::operator()
	/*!
		フレーム毎に呼び出される関数
	 */
	// ============================================================================
	void operator()() {
		m_func(m_target);
	}

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
	void TaskDestructor(EDestructorCbType::Type type);

	// ============================================================================
	// CTaskCb::Clear
	/*!
		設定クリア
	 */
	// ============================================================================
	void Clear() {
		m_target = NULL;
		m_destructor = m_func = NULL;
	}



//------------------------------------------------------------------------------**********
};		//class CTaskCb<accel::mpl::CNullType>












//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	プロシージャクラス
		プロシージャの切り替えを行うクラス
		データは保持出来ませんので、主に継承して使用します。

		負荷的には、関数ポインタの解決が１つ発生します。
 */
template<typename TCrtpType>
class CProc {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	typedef TCrtpType			TType;	//<CRTP型
	typedef void (TCrtpType::*FProc)();	//<プロシージャ型


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CProc::operator()
	/*!
		呼び出し関数
	 */
	// ============================================================================
	void operator()() {
		if (NULL != m_it) {
			if (!IsNoneProc()) {
				(m_it->*m_proc)();
			}
		}
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CProc::IsNoneProc
	/*!
		プロシージャの設定が無いか確認する

		@retval	true	無い
		@retval	false	有る
	 */
	// ============================================================================
	bool IsNoneProc() const {
		return ((m_proc == NULL)? true: false);
	}

	// ============================================================================
	// CProc::IsProc
	/*!
		特定のプロシージャが設定されているか確認する

		@retval	true	無い
		@retval	false	有る
	 */
	// ============================================================================
	bool IsProc(FProc proc) {
		return ((m_proc == proc)? true: false);
	}
	bool IsProc() {
		return IsNoneProc();
	}

	// ============================================================================
	// CProc::GetProc
	/*!
		プロシージャを取得する

		@return 取得したプロシージャ
	 */
	// ============================================================================
	FProc GetProc() const {
		return m_proc;
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// CProc::SetTarget
	/*!
		ターゲットを設定する

		@param	it	[in]	設定するターゲット

		@note
			プロシージャは未登録まで戻ります。
	 */
	// ============================================================================
	void SetTarget(TType &it) {
		m_it = &it;
		SetProc();
	}
	void SetTarget() {
		m_it = NULL;
		SetProc();
	}

	// ============================================================================
	// CProc::SetProc
	/*!
		プロシージャを設定する

		@param	proc	[in]	設定するプロシージャ

		@note
			プロシージャの型は“void function()”型のメンバ関数です。
			メンバ関数で無ければいけません、static関数は設定出来ません。
			メンバ関数ならばprotectedでもprivateでも構いません。
	 */
	// ============================================================================
	void SetProc(FProc proc) {
		m_proc = proc;
	}
	void SetProc() {
		m_proc = NULL;
	}


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CProc::CProc
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CProc() {
		m_it = NULL;
		m_proc = NULL;
	}
	CProc(TType &it) : m_it(&it), m_proc(NULL) {}

	// ============================================================================
	// CProc::~CProc
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	~CProc() {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	FProc	m_proc;		//プロシージャ
	TType	*m_it;		//対象クラス


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
};		//class CProc












//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	プロシージャカウントクラス
		プロシージャの切り替えを行うクラス
		データは保持出来ませんので、主に継承して使用します。

		プロシージャを切り替えてから、何回プロシージャが呼ばれたかを保持します

		負荷的には、プロシージャオブジェクト分と、
		関数呼び出しが１つ、32bitの加算が１つ発生します。
 */
template<typename TCrtpType>
class CProcCount : public CProc<TCrtpType> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	typedef CProc<TCrtpType>			super_type;
	typedef typename super_type::TType	TType;		//<CRTP型
	typedef typename super_type::FProc	FProc;		//<プロシージャ型
	typedef unsigned long				TCount;		//<カウント型


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CProcCount::operator()
	/*!
		呼び出し関数
	 */
	// ============================================================================
	void operator()() {
		++m_counter;
		super_type::operator()();
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CProcCount::GetCount
	/*!
		カウントを取得する

		@return 取得したカウント数

		@note
			“カウント”とは、
			“設定中のプロシージャの実行回数(現在含まず)”です。

			SetProc() により指定した直後は (u32)-1 になります。
			u32(60fpsで約828日)を越えると (u32)-1 を経由し 0 に戻ります。

			SetProc();                     \n
			    ↑                         \n
			    │GetCount() = (u32)-1     \n
			    ↓                         \n
			プロシージャ … GetCount() = 0 \n
			    ↑                         \n
			    │GetCount() = 0           \n
			    ↓                         \n
			プロシージャ … GetCount() = 1 \n
			    ↑                         \n
			    │GetCount() = 1           \n
			    ↓                         \n
			SetProc();                     \n

	 */
	// ============================================================================
	TCount GetCount() const {
		return m_counter;
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// CProcCount::SetProc
	/*!
		プロシージャを設定する

		@param	proc	[in]	設定するプロシージャ

		@note
			プロシージャの型は“void function()”型のメンバ関数です。
			メンバ関数で無ければいけません、static関数は設定出来ません。
			メンバ関数ならばprotectedでもprivateでも構いません。
	 */
	// ============================================================================
	void SetProc(FProc proc) {
		ResetCounter();
		super_type::SetProc(proc);
	}
	void SetProc() {
		ResetCounter();
		super_type::SetProc();
	}


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CProcCount::CProcCount
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CProcCount() : super_type() {};
	CProcCount(TType &it) : super_type(it) {};

	// ============================================================================
	// CProcCount::~CProcCount
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	~CProcCount() {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	TCount	m_counter;	//カウンタ


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
	// ============================================================================
	// CProcCount::ResetCounter
	/*!
		カウンタをリセットする
	 */
	// ============================================================================
	void ResetCounter() {
		m_counter = TCount(-1);
	}


private:
//------------------------------------------------------------------------------**********
};		//class CProcCount












//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タスククラス
		タスク機能とプロシージャ機能を有したクラス
		プロシージャ切り替えはほぼ必須機能の為、主にこちらを使用する事になります。
		タスクオブジェクトとプロシージャオブジェクトを変更する事が出来ます。

		負荷的にはタスクオブジェクト分とプロシージャオブジェクト分に加え、
		関数呼び出しが１つ増えます。

		CTask はタスクに限らず、operator()() を呼ぶクラス(TTask)と、
		operator()() が呼ばれて欲しいクラス(TProc)を橋渡しする機能を有しています。

		タスク継承図とoperator()()の挙動について                       \n
			TTask::operator()()…(1)    TProc::operator()()…(3)       \n
			  △                          △                           \n
			  │    ┌──────────┘                           \n
			CTask::operator()()…(2)                                   \n
		まず、operator()() を呼ぶクラス(TTask)により (1) が呼ばれます。\n
		(1) は仮想関数ですので、実際には実態である (2) が呼ばれます。  \n
		(2) は関数内部で (3) を呼び出します。                          \n
		(3) が呼ばれます。                                             \n

		operator()() を呼ぶクラス(TTask) については、
		operator()() の仮想関数化が必須です。
		operator()() が呼ばれて欲しいクラス(TProc)については、
		operator()() の仮想関数化が必須ではありません。
		仮想関数でも構いませんし、仮想関数でなくても構いません。
 */
template <typename TCrtpType, typename TProc = CProcCount<TCrtpType>, typename TTask = ITaskWork>
class CTask : public TTask, public TProc {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	template<typename TReqCrtpType, typename TReqProc = CProcCount<TReqCrtpType>, typename TReqTask = ITaskWork>
	struct Gene {
		typedef CTask<TReqCrtpType, TReqProc, TReqTask>	Type;
	};
	typedef TCrtpType	Crtp;	//CRTPオブジェクト
	typedef TTask		Task;	//タスクオブジェクト
	typedef TProc		Proc;	//プロシージャオブジェクト


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CTask::CTask
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CTask() : Task(), Proc() {
		Proc::SetTarget(static_cast<Crtp &>(*this));
	}

	// ============================================================================
	// CTask::~CTask
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~CTask() {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
	// ============================================================================
	// CTask::operator()
	/*!
		フレーム毎に呼び出される関数
	 */
	// ============================================================================
	void operator()() {
		Proc::operator()();
	}


private:


//------------------------------------------------------------------------------**********
};		//class CTask










#if ERD_TASK_WEAK_PORTING
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	複数タスクインターフェース
		複数タスクに関するインターフェースを定義したクラス
 */
class ITaskMulti {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	typedef void (ITask::*FProc)(u32 idx);


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// ITaskMulti::operator()
	/*!
		フレーム毎に呼び出される関数

		@param	idx	[in]	タスクインデックス
	 */
	// ============================================================================
	virtual void operator()(u32 idx) = 0;


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// ITaskMulti::GetTaskTcb
	/*!
		タスクTCB(Cベース)を取得する

		@param	idx	[in]	タスクインデックス

		@return タスクTCB(Cベース)
	 */
	// ============================================================================
	virtual const ::MTS_TASK_TCB *GetTaskTcb(u32 idx) const = 0;
	::MTS_TASK_TCB *GetTaskTcb(u32 idx) {
		return const_cast<MTS_TASK_TCB *>(
						const_cast<const ITaskMulti *>(this)->GetTaskTcb(idx)
					);
	}

	// ============================================================================
	// ITaskMulti::GetPrio
	/*!
		プライオリティを取得する

		@param	idx	[in]	タスクインデックス

		@return プライオリティ
	 */
	// ============================================================================
	u16 GetPrio(u32 idx) const {
		return GetTaskTcb(idx)->priority;
	}

	// ============================================================================
	// ITaskMulti::GetGroup
	/*!
		グループを取得する

		@param	idx	[in]	タスクインデックス

		@return グループ
	 */
	// ============================================================================
	u8 GetGroup(u32 idx) const {
		return GetTaskTcb(idx)->group;
	}

	// ============================================================================
	// ITaskMulti::GetFlag
	/*!
		フラグを取得する

		@param	idx	[in]	タスクインデックス

		@return フラグ
	 */
	// ============================================================================
	u16 GetFlag(u32 idx) const {
		return GetTaskTcb(idx)->flag;
	}

	// ============================================================================
	// ITaskMulti::GetState
	/*!
		状態を取得する

		@param	idx	[in]	タスクインデックス

		@return 状態
	 */
	// ============================================================================
	u16 GetState(u32 idx) const {
		return GetTaskTcb(idx)->state;
	}

	// ============================================================================
	// ITaskMulti::GetPauseLv
	/*!
		ポーズレベルを取得する

		@param	idx	[in]	タスクインデックス

		@return ポーズレベル
	 */
	// ============================================================================
	u8 GetPauseLv(u32 idx) const {
		return GetTaskTcb(idx)->pause_level;
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// ITaskMulti::ITaskMulti
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
protected:
	ITaskMulti() {}

	// ============================================================================
	// ITaskMulti::~ITaskMulti
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~ITaskMulti() {}


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
};		//class ITaskMulti
#endif //#if ERD_TASK_WEAK_PORTING













#if ERD_TASK_WEAK_PORTING
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タスクインデックスコールバッククラス
		継承せずにタスク機能を外付けする為のクラス
		このクラスはタスクリストに接続する事が出来ます。
		タスクリストに接続した場合、毎フレーム指定されたクラスの、
		指定されたメンバ関数が呼び出されます。

		ITaskLink は継承(is-a)して使用しますが、このクラスは委譲(has-a)して使用します。

		負荷的にはITaskLinkから追加で、
		ポインタの解決が１つ、関数ポインタの解決が１つ増えます。
*/
template<typename TType = accel::mpl::CNullType>
class CTaskCbIdx : public ITaskLink {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	template<typename TReqType>
	struct Gene {
		typedef CTaskCbIdx<TReqType>	Type;
	};
	typedef TType	Type;					//委譲クラス
	typedef void (TType::*FFunc)(u32 idx);	//呼び出すメンバ関数


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
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
	void AttachTask(const char *name, TType &owner, FFunc func, FFunc destructor = NULL
					, u16 priority = 0x0000, u8 group = 0x00, u16 flag = 0x0000
					, u8 pause_level = 0x00);
#else	//#if defined(MTD_DEBUG)
	void AttachTask(TType &owner, FFunc func, FFunc destructor = NULL
					, u16 priority = 0x0000, u8 group = 0x00, u16 flag = 0x0000
					, u8 pause_level = 0x00);
	void AttachTask(const char *name, TType &owner, FFunc func, FFunc destructor = NULL
					, u16 priority = 0x0000, u8 group = 0x00, u16 flag = 0x0000
					, u8 pause_level = 0x00) {
		AttachTask(owner, func, destructor, priority, group, flag, pause_level);
	}
#endif	//#if defined(MTD_DEBUG)


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CTaskCbIdx::GetIdx
	/*!
		インデックスの取得

		@return	インデックス
	 */
	// ============================================================================
	u32 GetIdx() {
		return m_idx;
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// CTaskCbIdx::SetIdx
	/*!
		インデックスの取得

		@param	idx	[in]	インデックス
	 */
	// ============================================================================
	void SetIdx(u32 idx) {
		m_idx = idx;
	}


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CTaskCbIdx::CTaskCbIdx
	/*!
		デフォルトコンストラクタ

		@param	idx	[in]	インデックス
	 */
	// ============================================================================
public:
	explicit CTaskCbIdx(u32 idx = 0);

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
public:
	CTaskCbIdx(u32 idx, const char *name
					, TType &owner, FFunc func, FFunc destructor = NULL
					, u16 priority = 0x0000, u8 group = 0x00, u16 flag = 0x0000
					, u8 pause_level = 0x00);

	// ============================================================================
	// CTaskCbIdx::~CTaskCbIdx
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~CTaskCbIdx() {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	TType	*m_target;		//呼び出すメンバ関数の所属するクラス
	FFunc	m_func;			//呼び出すメンバ関数
	FFunc	m_destructor;	//デストラクタ作動時に呼び出すメンバ関数
	u32		m_idx;			//インデックス


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	// ============================================================================
	// CTaskCbIdx::operator()
	/*!
		フレーム毎に呼び出される関数
	 */
	// ============================================================================
	void operator()() {
		(m_target->*m_func)(m_idx);
	}

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
	void TaskDestructor(EDestructorCbType::Type type);

	// ============================================================================
	// CTaskCbIdx::Clear
	/*!
		設定クリア
	 */
	// ============================================================================
	void Clear() {
		m_target = NULL;
		m_destructor = m_func = NULL;
	}


//------------------------------------------------------------------------------**********
};		//class CTaskCbIdx<typename TType = accel::mpl::CNullType>

//------------------------------------------------------------------------------**********

template<>
class CTaskCbIdx<accel::mpl::CNullType> : public ITaskLink {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	template<typename TReqType>
	struct Gene {
		typedef CTaskCbIdx<TReqType>	Type;
	};
	typedef void *Type;			//データ
	typedef void (*FFunc)(void *data, u32 idx);	//呼び出すメンバ関数


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
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
	void AttachTask(const char *name, void *data, FFunc func, FFunc destructor = NULL
					, u16 priority = 0x0000, u8 group = 0x00, u16 flag = 0x0000
					, u8 pause_level = 0x00);
#else	//#if defined(MTD_DEBUG)
	void AttachTask(void *data, FFunc func, FFunc destructor = NULL
					, u16 priority = 0x0000, u8 group = 0x00, u16 flag = 0x0000
					, u8 pause_level = 0x00);
	void AttachTask(const char *name, void *data, FFunc func, FFunc destructor = NULL
					, u16 priority = 0x0000, u8 group = 0x00, u16 flag = 0x0000
					, u8 pause_level = 0x00) {
		AttachTask(data, func, destructor, priority, group, flag, pause_level);
	}
#endif	//#if defined(MTD_DEBUG)


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CTaskCbIdx::GetIdx
	/*!
		インデックスの取得

		@return	インデックス
	 */
	// ============================================================================
	u32 GetIdx() {
		return m_idx;
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// CTaskCbIdx::SetIdx
	/*!
		インデックスの取得

		@param	idx	[in]	インデックス
	 */
	// ============================================================================
	void SetIdx(u32 idx) {
		m_idx = idx;
	}


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CTaskCbIdx::CTaskCbIdx
	/*!
		デフォルトコンストラクタ

		@param	idx	[in]	インデックス
	 */
	// ============================================================================
public:
	explicit CTaskCbIdx(u32 idx = 0);

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
public:
	CTaskCbIdx(u32 idx, const char *name
					, void *data, FFunc func, FFunc destructor = NULL
					, u16 priority = 0x0000, u8 group = 0x00, u16 flag = 0x0000
					, u8 pause_level = 0x00);

	// ============================================================================
	// CTaskCbIdx::~CTaskCbIdx
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~CTaskCbIdx() {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	void	*m_target;		//呼び出すメンバ関数の所属するクラス
	FFunc	m_func;			//呼び出すメンバ関数
	FFunc	m_destructor;	//デストラクタ作動時に呼び出すメンバ関数
	u32		m_idx;			//インデックス


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	// ============================================================================
	// CTaskCbIdx::operator()
	/*!
		フレーム毎に呼び出される関数
	 */
	// ============================================================================
	void operator()() {
		m_func(m_target, m_idx);
	}

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
	void TaskDestructor(EDestructorCbType::Type type);

	// ============================================================================
	// CTaskCbIdx::Clear
	/*!
		設定クリア
	 */
	// ============================================================================
	void Clear() {
		m_target = NULL;
		m_destructor = m_func = NULL;
	}


//------------------------------------------------------------------------------**********
};		//class CTaskCbIdx<accel::mpl::CNullType>
#endif //#if ERD_TASK_WEAK_PORTING













#if ERD_TASK_WEAK_PORTING
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	複数タスクリンクインターフェース
		複数のタスク機能を外付けする為のクラス
		このクラスを継承したクラスはタスクリストに複数接続する事が出来ます。
		タスクリストに接続した場合、毎フレーム operator (u32 idx) が接続回数分呼び出されます。
*/
template <u32 TNum>
class ITaskLinkMulti : public ITaskMulti {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	template<u32 TReqNum>
	struct Gene {
		typedef ITaskLinkMulti<TReqNum>	Type;
	};
	enum {Num = TNum};	//個数

	
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// ITaskLinkMulti::AttachTask
	/*!
		タスクリストへ登録

		@param	idx			[in]	インデックス
		@param	name		[in]	タスク名
		@param	priority	[in]	タスク優先度
		@param	group		[in]	タクスグループ
		@param	flag		[in]	タスクフラグ
		@param	pause_level	[in]	タスクポーズレベル
	 */
	// ============================================================================
#if defined(MTD_DEBUG)
	void AttachTask(u32 idx, const char *name, u16 priority = 0x0000
					, u8 group = 0x00, u16 flag = 0x0000, u8 pause_level = 0x00) {
		getChild(idx).AttachTask(name, *this
									, &ITaskLinkMulti<TNum>::operator()
									, &ITaskLinkMulti<TNum>::TcbDestructor
									, priority, group, flag, pause_level
								);
	}
#else	//#if defined(MTD_DEBUG)
	void AttachTask(u32 idx, u16 priority = 0x0000, u8 group = 0x00
									, u16 flag = 0x0000, u8 pause_level = 0x00) {
		getChild(idx).AttachTask(*this
									, &ITaskLinkMulti<TNum>::operator()
									, &ITaskLinkMulti<TNum>::TcbDestructor
									, priority, group, flag, pause_level
								);
	}
	void AttachTask(u32 idx, const char *name, u16 priority = 0x0000, u8 group = 0x00
									, u16 flag = 0x0000, u8 pause_level = 0x00) {
		AttachTask(idx, priority, group, flag, pause_level);
	}
#endif	//#if defined(MTD_DEBUG)

	// ============================================================================
	// ITaskLinkMulti::DetachTask
	/*!
		タスクリストから脱退

		@param	idx			[in]	インデックス
	 */
	// ============================================================================
	void DetachTask(u32 idx) {
		getChild(idx).DetachTask();
	};

	// ============================================================================
	// ITaskLinkMulti::operator()
	/*!
		フレーム毎に呼び出される関数

		@param	idx			[in]	インデックス
	 */
	// ============================================================================
	virtual void operator()(u32 idx) = 0;


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// ITaskLinkMulti::GetTaskTcb
	/*!
		タスクTCB(Cベース)を取得する

		@param	idx			[in]	インデックス

		@return タスクTCB(Cベース)

		@note
			IsTask が FALSE を返す時は NULL を返します。
	 */
	// ============================================================================
	const ::MTS_TASK_TCB *GetTaskTcb(u32 idx) const {
		return getChild(idx).GetTaskTcb();
	}

	// ============================================================================
	// ITaskLinkMulti::IsTask
	/*!
		タスク中の確認

		@param	idx			[in]	インデックス

		@retval	TRUE:	タスク中
		@retval	FALSE:	タスク中ではない
	 */
	// ============================================================================
	BOOL IsTask(u32 idx) {
		return getChild(idx).IsTask();
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// ITaskLinkMulti::ITaskLinkMulti
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	ITaskLinkMulti();

	// ============================================================================
	// ITaskLinkMulti::ITaskLinkMulti
	/*!
		コピーコンストラクタ・コピー演算子

		@param	src	[in]	コピー元
		
		@note
			コピー可能な様に実装出来ない訳ではありませんが、
			制限の厳しいITaskWorkに合わせています
	 */
	// ============================================================================
protected:
	ITaskLinkMulti(const ITaskLinkMulti &src);
	ITaskLinkMulti &operator=(const ITaskLinkMulti &src);

	// ============================================================================
	// ITaskLinkMulti::~ITaskLinkMulti
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~ITaskLinkMulti();


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	typedef CTaskCbIdx<ITaskLinkMulti<TNum> >	TChild;


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	TChild	m_child[TNum];


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
	// ============================================================================
	// ITaskLinkMulti::TaskDestructor
	/*!
		タスク消失時に実行される関数

		@note
			DetachTaskが呼ばれた場合に、
			タスクマネージャ側からの要請で切断された場合に発生します。

			この関数はmtTaskのデストラクタ内で実行される場合があります。
	 */
	// ============================================================================
	virtual void TaskDestructor(u32 idx) {};

private:
	// ============================================================================
	// ITaskLinkMulti::getChild
	/*!
		子要素を取得する

		@param	idx	[in]	インデックス

		@return 取得した子要素
	 */
	// ============================================================================
	TChild &getChild(u32 idx) {
		return m_child[idx];
	}
	const TChild &getChild(u32 idx) const {
		return m_child[idx];
	}


//------------------------------------------------------------------------------**********
};		//class ITaskLinkMulti
#endif //#if ERD_TASK_WEAK_PORTING












//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	複数プロシージャクラス
		複数のプロシージャ切り替えを行うクラス
		データは保持出来ませんので、主に継承して使用します。

		負荷的には、プロシージャオブジェクト分に加えて配列アクセスが発生します。
 */
template <typename TType, unsigned long TNum>
class CProcMulti {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	typedef unsigned long	TSize;
	typedef unsigned long	TIndex;
	template<typename TReqType, TSize TReqNul>
	struct Gene {
		typedef CProcMulti<TReqType, TReqNul>	Type;
	};
	enum {Num = TNum};				//個数
	typedef TType			Type;	//プロシージャを使用するクラス型
	typedef void (TType::*FProc)();	//プロシージャ型


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CProcMulti::operator()
	/*!
		呼び出し関数

		@param	idx	[in]	インデックス
	 */
	// ============================================================================
	void operator()(TIndex idx);

	// ============================================================================
	// CProcMulti::operator()
	/*!
		全呼び出し関数
	 */
	// ============================================================================
	void operator()();


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CProcMulti::IsNoneProc
	/*!
		プロシージャの設定が無いか確認する

		@param	idx	[in]	インデックス

		@retval	true	無い
		@retval	false	有る
	 */
	// ============================================================================
	bool IsNoneProc(TIndex idx) const;

	// ============================================================================
	// CProcMulti::IsProc
	/*!
		特定のプロシージャが設定されているか確認する
		
		@param	idx	[in]	インデックス

		@retval	TRUE	無い
		@retval	FALSE	有る
	 */
	// ============================================================================
	template <typename T>
	bool IsProc(TIndex idx, T proc);
	bool IsProc(TIndex idx) {
		return IsNoneProc(idx);
	}

	// ============================================================================
	// CProcMulti::GetProc
	/*!
		プロシージャを取得する

		@param	idx	[in]	インデックス

		@return 取得したプロシージャ
	 */
	// ============================================================================
	FProc GetProc(TIndex idx) const {
		return m_proc[idx];
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
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
	template <typename T>
	void SetProc(TIndex idx, T proc);
	void SetProc(TIndex idx) {
		m_proc = reinterpret_cast<FProc>(NULL);
	}


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CProcMulti::CProcMulti
	/*!
		デフォルトコンストラクタ

		@param	target	継承先クラス
	 */
	// ============================================================================
public:
	explicit CProcMulti(TType &target);

	// ============================================================================
	// CProcMulti::~CProcMulti
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	~CProcMulti() {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	Type	&m_target;
	FProc	m_proc[TNum];


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
};		//class CProcMulti












#if ERD_TASK_WEAK_PORTING
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	複数タスククラス
		複数タスク機能と複数プロシージャ機能を有したクラス
 */
template <typename TType, u32 TNum, typename TProc = CProcMulti<TType, TNum>, typename TTask = ITaskLinkMulti<TNum> >
class CTaskMulti : public TTask, public TProc {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	template<typename TReqType, u32 TReqNum, typename TReqProc = TProc::Gene<TReqType, TReqNum>::Type, typename TReqTask = TTask::Gene<TReqNum>::Type>
	struct Gene {
		typedef CTaskMulti<TReqType, TReqNum, TReqProc, TReqTask>	Type;
	};
	typedef TType	Type;	//使用クラス
	typedef TTask	Task;	//タスクオブジェクト
	typedef TProc	Proc;	//プロシージャオブジェクト


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CTaskMulti::CTaskMulti
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	explicit CTaskMulti(TType &target) : TTask(), TProc(target) {}

	// ============================================================================
	// CTaskMulti::~CTaskMulti
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~CTaskMulti() {}


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
	// CTaskMulti::operator()
	/*!
		フレーム毎に呼び出される関数

		@param	idx			[in]	インデックス
	 */
	// ============================================================================
	void operator()(u32 idx) {
		TProc::operator()(idx);
	}


//------------------------------------------------------------------------------**********
};		//class CTaskMulti
#endif //#if ERD_TASK_WEAK_PORTING


























































#if ERD_TASK_WEAK_PORTING
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タスククリア判断基礎クラス
		クリア判断用関数オブジェクトのインターフェースです。
		タスククリア関数 Clear の引数に使用します。
 */
class IClearJudge {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// IClearJudge::operator()
	/*!
		タスク毎に呼び出される関数

		@param	tcb		[in]	判断するタスク(NULL不可)

		@return TRUE	タスク削除
		@return FALSE	タスク存続

		@note
			関数 operator()(ITaskWork *tcb) とは排他で呼び出されます。

			ITaskWork から派生していないタスクや、
			operator()(ITaskWork &tcb) をオーバーライドしていない時に、
			呼び出されます。
	 */
	// ============================================================================
	virtual BOOL operator()(const ::MTS_TASK_TCB *tcb) {
		return FALSE;
	};

	// ============================================================================
	// IClearJudge::operator()
	/*!
		タスク毎に呼び出される関数

		@param	tcb		[in]	判断するタスク

		@return TRUE	タスク削除
		@return FALSE	タスク存続

		@note
			関数 operator()(::MTS_TASK_TCB *tcb) とは排他で呼び出されます。

			この関数をオーバーライドしなければ、
			operator()(::MTS_TASK_TCB *tcb) を呼び出し処理を任せます。

			この関数をオーバーライドする必要は(処理速度を除き)通常ありません。
			operator()(::MTS_TASK_TCB *tcb) では ITaskWork の情報が削ぎ落とされる為、
			それらの情報が必要な場合はこちらを使用します。
	 */
	// ============================================================================
	virtual BOOL operator()(const ITask &tcb) {
		return (*this)(tcb.GetTaskTcb());
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
};		//class IClearJudge
#endif //#if ERD_TASK_WEAK_PORTING









#if ERD_TASK_WEAK_PORTING
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タスク全クリアクラス
		全タスククリアを行うクリア判断用関数オブジェクトです。
		タスククリア関数 Clear の引数に使用します。
 */
class CAll : public IClearJudge {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CAll::operator()
	/*!
		タスク毎に呼び出される関数

		@param	tcb		[in]	判断するタスク

		@return TRUE	タスク削除
		@return FALSE	タスク存続

		@note
			関数 operator()(ITaskWork *tcb) とは排他で呼び出されます。

			ITaskWork から派生していないタスクや、
			operator()(ITaskWork &tcb) をオーバーライドしていない時に、
			呼び出されます。
	 */
	// ============================================================================
	BOOL operator()(const ::MTS_TASK_TCB *tcb) {
		return TRUE;
	}

	// ============================================================================
	// CAll::operator()
	/*!
		タスク毎に呼び出される関数

		@param	tcb		[in]	判断するタスク

		@return TRUE	タスク削除
		@return FALSE	タスク存続

		@note
			関数 operator()(::MTS_TASK_TCB *tcb) とは排他で呼び出されます。

			この関数をオーバーライドしなければ、
			operator()(::MTS_TASK_TCB *tcb) を呼び出し処理を任せます。

			この関数をオーバーライドする必要は(処理速度を除き)通常ありません。
			operator()(::MTS_TASK_TCB *tcb) では ITaskWork の情報が削ぎ落とされる為、
			それらの情報が必要な場合はこちらを使用します。
	 */
	// ============================================================================
	BOOL operator()(const ITask &tcb) {
		return TRUE;
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CAll::CAll
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CAll() {}


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
};		//class CAll
#endif //#if ERD_TASK_WEAK_PORTING










#if ERD_TASK_WEAK_PORTING
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タスクグループクリアクラス
		グループに依ってタスククリアを行うクリア判断用関数オブジェクトです。
		同一グループがクリアされます。
		タスククリア関数 Clear の引数に使用します。
 */
class CGroup : public IClearJudge {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CGroup::operator()
	/*!
		タスク毎に呼び出される関数

		@param	tcb		[in]	判断するタスク

		@return TRUE	タスク削除
		@return FALSE	タスク存続

		@note
			関数 operator()(ITaskWork *tcb) とは排他で呼び出されます。

			ITaskWork から派生していないタスクや、
			operator()(ITaskWork &tcb) をオーバーライドしていない時に、
			呼び出されます。
	 */
	// ============================================================================
	BOOL operator()(const ::MTS_TASK_TCB *tcb) {
		return ((m_group == tcb->group)? TRUE: FALSE);
	}

	// ============================================================================
	// CGroup::operator()
	/*!
		タスク毎に呼び出される関数

		@param	tcb		[in]	判断するタスク

		@return TRUE	タスク削除
		@return FALSE	タスク存続

		@note
			関数 operator()(::MTS_TASK_TCB *tcb) とは排他で呼び出されます。

			この関数をオーバーライドしなければ、
			operator()(::MTS_TASK_TCB *tcb) を呼び出し処理を任せます。

			この関数をオーバーライドする必要は(処理速度を除き)通常ありません。
			operator()(::MTS_TASK_TCB *tcb) では ITaskWork の情報が削ぎ落とされる為、
			それらの情報が必要な場合はこちらを使用します。
	 */
	// ============================================================================
	BOOL operator()(const ITask &tcb) {
		return ((m_group == tcb.GetGroup())? TRUE: FALSE);
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CGroup::CGroup
	/*!
		デフォルトコンストラクタ

		@param	group	[in]	タスククリアを行うグループ
	 */
	// ============================================================================
public:
	explicit CGroup(u8 group) : m_group(group) {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	u8 m_group;	//削除するグループ


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
};		//class CGroup
#endif //#if ERD_TASK_WEAK_PORTING











#if ERD_TASK_WEAK_PORTING
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タスクプライオリティクリアクラス
		プライオリティに依ってタスククリアを行うクリア判断用関数オブジェクトです。
		begin <= priority <= end の範囲がクリアされます。
		タスククリア関数 Clear の引数に使用します。
 */
class CPrio : public IClearJudge {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CPrio::operator()
	/*!
		タスク毎に呼び出される関数

		@param	tcb		[in]	判断するタスク

		@return TRUE	タスク削除
		@return FALSE	タスク存続

		@note
			関数 operator()(ITaskWork *tcb) とは排他で呼び出されます。

			ITaskWork から派生していないタスクや、
			operator()(ITaskWork &tcb) をオーバーライドしていない時に、
			呼び出されます。
	 */
	// ============================================================================
	BOOL operator()(const ::MTS_TASK_TCB *tcb) {
		return (((m_begin <= tcb->priority) && (tcb->priority <= m_end))? TRUE: FALSE);
	}

	// ============================================================================
	// CPrio::operator()
	/*!
		タスク毎に呼び出される関数

		@param	tcb		[in]	判断するタスク

		@return TRUE	タスク削除
		@return FALSE	タスク存続

		@note
			関数 operator()(::MTS_TASK_TCB *tcb) とは排他で呼び出されます。

			この関数をオーバーライドしなければ、
			operator()(::MTS_TASK_TCB *tcb) を呼び出し処理を任せます。

			この関数をオーバーライドする必要は(処理速度を除き)通常ありません。
			operator()(::MTS_TASK_TCB *tcb) では ITaskWork の情報が削ぎ落とされる為、
			それらの情報が必要な場合はこちらを使用します。
	 */
	// ============================================================================
	BOOL operator()(const ITask &tcb) {
		u16 prio = tcb.GetPrio();
		return (((m_begin <= prio) && (prio <= m_end))? TRUE: FALSE);
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CPrio::CPrio
	/*!
		デフォルトコンストラクタ

		@param	begin	[in]	タスククリアを開始する優先度(自身含む)
		@param	end		[in]	タスククリアを終了する優先度(自身含む)
	 */
	// ============================================================================
public:
	CPrio(u16 begin, u16 end) : m_begin(begin), m_end(end) {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	u16 m_begin;	//削除するプライオリティの開始
	u16 m_end;		//削除するプライオリティの終了


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
};		//class CPrio
#endif //#if ERD_TASK_WEAK_PORTING












#if ERD_TASK_WEAK_PORTING
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タスククリア条件否定クラス
		タスククリアを行うクリア判断を逆にするアダプタです。
		タスククリア関数 Clear の引数に使用します。
 */
class CNot : public IClearJudge {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CNot::operator()
	/*!
		タスク毎に呼び出される関数

		@param	tcb		[in]	判断するタスク

		@return TRUE	タスク削除
		@return FALSE	タスク存続

		@note
			関数 operator()(ITaskWork *tcb) とは排他で呼び出されます。

			ITaskWork から派生していないタスクや、
			operator()(ITaskWork &tcb) をオーバーライドしていない時に、
			呼び出されます。
	 */
	// ============================================================================
	BOOL operator()(const ::MTS_TASK_TCB *tcb) {
		return !(m_judge(tcb));
	}

	// ============================================================================
	// CNot::operator()
	/*!
		タスク毎に呼び出される関数

		@param	tcb		[in]	判断するタスク

		@return TRUE	タスク削除
		@return FALSE	タスク存続

		@note
			関数 operator()(::MTS_TASK_TCB *tcb) とは排他で呼び出されます。

			この関数をオーバーライドしなければ、
			operator()(::MTS_TASK_TCB *tcb) を呼び出し処理を任せます。

			この関数をオーバーライドする必要は(処理速度を除き)通常ありません。
			operator()(::MTS_TASK_TCB *tcb) では ITaskWork の情報が削ぎ落とされる為、
			それらの情報が必要な場合はこちらを使用します。
	 */
	// ============================================================================
	BOOL operator()(const ITask &tcb) {
		return !(m_judge(tcb));
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CNot::CNot
	/*!
		デフォルトコンストラクタ

		@param	judge	[in]	タスククリア条件クラス
	 */
	// ============================================================================
public:
	explicit CNot(IClearJudge &judge) : m_judge(judge) {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	IClearJudge m_judge;	//タスククリア用関数オブジェクト


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
};		//class CNot
#endif //#if ERD_TASK_WEAK_PORTING












#if ERD_TASK_WEAK_PORTING
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タスククリア条件論理積演算クラス
		２つのクリア判断を論理積したクリア判断を作成するアダプタです。
		タスククリア関数 Clear の引数に使用します。
 */
class CAnd : public IClearJudge {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CAnd::operator()
	/*!
		タスク毎に呼び出される関数

		@param	tcb		[in]	判断するタスク

		@return TRUE	タスク削除
		@return FALSE	タスク存続

		@note
			関数 operator()(ITaskWork *tcb) とは排他で呼び出されます。

			ITaskWork から派生していないタスクや、
			operator()(ITaskWork &tcb) をオーバーライドしていない時に、
			呼び出されます。
	 */
	// ============================================================================
	BOOL operator()(const ::MTS_TASK_TCB *tcb) {
		return (m_lhs(tcb) && m_rhs(tcb));
	}

	// ============================================================================
	// CAnd::operator()
	/*!
		タスク毎に呼び出される関数

		@param	tcb		[in]	判断するタスク

		@return TRUE	タスク削除
		@return FALSE	タスク存続

		@note
			関数 operator()(::MTS_TASK_TCB *tcb) とは排他で呼び出されます。

			この関数をオーバーライドしなければ、
			operator()(::MTS_TASK_TCB *tcb) を呼び出し処理を任せます。

			この関数をオーバーライドする必要は(処理速度を除き)通常ありません。
			operator()(::MTS_TASK_TCB *tcb) では ITaskWork の情報が削ぎ落とされる為、
			それらの情報が必要な場合はこちらを使用します。
	 */
	// ============================================================================
	BOOL operator()(const ITask &tcb) {
		return (m_lhs(tcb) && m_rhs(tcb));
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CAnd::CAnd
	/*!
		デフォルトコンストラクタ

		@param	lhs	[in]	タスククリア条件クラス
		@param	rhs	[in]	タスククリア条件クラス
	 */
	// ============================================================================
public:
	CAnd(IClearJudge &lhs, IClearJudge &rhs) : m_lhs(lhs), m_rhs(rhs) {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	IClearJudge m_lhs;	//タスククリア用関数オブジェクト
	IClearJudge m_rhs;	//タスククリア用関数オブジェクト


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
};		//class CAnd
#endif //#if ERD_TASK_WEAK_PORTING












#if ERD_TASK_WEAK_PORTING
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タスククリア条件論理和演算クラス
		２つのクリア判断を論理和したクリア判断を作成するアダプタです。
		タスククリア関数 Clear の引数に使用します。
 */
class COr : public IClearJudge {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// COr::operator()
	/*!
		タスク毎に呼び出される関数

		@param	tcb		[in]	判断するタスク

		@return TRUE	タスク削除
		@return FALSE	タスク存続

		@note
			関数 operator()(ITaskWork *tcb) とは排他で呼び出されます。

			ITaskWork から派生していないタスクや、
			operator()(ITaskWork &tcb) をオーバーライドしていない時に、
			呼び出されます。
	 */
	// ============================================================================
	BOOL operator()(const ::MTS_TASK_TCB *tcb) {
		return (m_lhs(tcb) || m_rhs(tcb));
	}

	// ============================================================================
	// COr::operator()
	/*!
		タスク毎に呼び出される関数

		@param	tcb		[in]	判断するタスク

		@return TRUE	タスク削除
		@return FALSE	タスク存続

		@note
			関数 operator()(::MTS_TASK_TCB *tcb) とは排他で呼び出されます。

			この関数をオーバーライドしなければ、
			operator()(::MTS_TASK_TCB *tcb) を呼び出し処理を任せます。

			この関数をオーバーライドする必要は(処理速度を除き)通常ありません。
			operator()(::MTS_TASK_TCB *tcb) では ITaskWork の情報が削ぎ落とされる為、
			それらの情報が必要な場合はこちらを使用します。
	 */
	// ============================================================================
	BOOL operator()(const ITask &tcb) {
		return (m_lhs(tcb) || m_rhs(tcb));
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// COr::COr
	/*!
		デフォルトコンストラクタ

		@param	lhs	[in]	タスククリア条件クラス
		@param	rhs	[in]	タスククリア条件クラス
	 */
	// ============================================================================
public:
	COr(IClearJudge &lhs, IClearJudge &rhs) : m_lhs(lhs), m_rhs(rhs) {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	IClearJudge m_lhs;	//タスククリア用関数オブジェクト
	IClearJudge m_rhs;	//タスククリア用関数オブジェクト


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
};		//class COr
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
void Clear(IClearJudge judge = CAll());
#endif //#if ERD_TASK_WEAK_PORTING












} //namespace task
} //namespace er
//------ Template Include ------------- テンプレートインクルード ---------------******_IC*
#include "erTask.tpp"





#endif //#if	defined(__cplusplus)

	// ============================================================================
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
	// ============================================================================
