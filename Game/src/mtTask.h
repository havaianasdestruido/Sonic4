// ==========================================================================
/*!
  @file mtTask.h
  @brief MT仕様タスクシステム

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: mtTask.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *	タスクグループ
 *		所有者(AMD_TASK_UID_00 ～ AMD_TASK_UID_0F)を使用
 *		AMD_TASK_UID_0F はシステムで使用するため、AMD_TASK_UID_00 ～ AMD_TASK_UID_0E をユーザーで使用
 *		タスク生成時の指定は、0～14のグループNOで指定する
 *
 *	ポーズレベル
 *		mtTaskで吸収
 *		ポーズ開始時のポーズレベル以下のタスクが停止
 *
 *	ワークサイズ
 *		タスク生成時に自動的にワークを取得
 *		タスク破棄時に自動解放
 *
 *	プライオリティ
 *		システムが使用するため、0指定は不可
 */


#ifndef GS_TASK_H_
#define GS_TASK_H_




//----- Include Files -------------------------------------------------------
#include <alice.h>
#include "mt.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
#define MTD_TASK_DEF_ATTR		(AMD_TASK_ATTR_MAIN)	//!< ユーザー用標準属性
#define MTD_TASK_GROUP_MAX		(15)
#define MTD_TASK_PRIORITY_TAIL	(0xEFFF)
#define MTD_TASK_ERROR_ADDR		(NULL)


#define MTD_TASK_TCB_FLAG_NO_PAUSE		(1<< 0)     ///< ポーズ不可 (ポーズレベル0xFFFFで設定)
#define MTD_TASK_TCB_FLAG_IMMORTAL		(1<< 1)     ///< 不死 無効

typedef struct tag_MTS_TASK_TCB {
	AMS_TCB		*am_tcb;		//!< TCBワーク

	void		(*proc)(struct tag_MTS_TASK_TCB *);	//!< ユーザープロシージャ
	void		(*dest)(struct tag_MTS_TASK_TCB *);	//!< ユーザーデストラクタ

	u16			pause_level;	//!< ポーズレベル

	void		*work;			//!< ワーク

} MTS_TASK_TCB;


/// タスクプロシージャ型
typedef void (*GSF_TASK_PROCEDURE)(MTS_TASK_TCB *);
/// タスクデストラクタ型
typedef void (*GSF_TASK_DESTRUCTOR)(MTS_TASK_TCB *);


/// 無効設定
#define mtSetTaskBarColor(color)	

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------
// ==========================================================================
// MTM_TASK_MAKE_TCB
/*!
 *	タスク生成 MT
 *
 *	@param	proc		[in]	処理関数
 *	@param	dest		[in]	終了処理関数
 *	@param	flag		[in]	mt用フラグ 参照無し
 *	@param	pause_level	[in]	mt用ポーズレベル
 *	@param	prio		[in]	タスク優先
 *	@param	group		[in]	タスクグループ(0～14)
 *	@param	work_size	[in]	ワークサイズ
 *	@param	name		[in]	デバッグ用TCB名(NULL可)
 *
 *	@return   生成タスクのTCB
 *
 *	@note
 *		MTと同じ形式で呼び出します。\n
 *		flagは無効になります。\n
 *		pause_levelは、UserIDに置き換わります。\n
 */
// ==========================================================================
#define MTM_TASK_MAKE_TCB(proc, dest, flag, pause_level, prio, group, work_size, name)	\
		(mtTaskMake((proc), (dest), (flag), (pause_level), (prio), (group), (work_size), (name)))

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// システム
// ==========================================================================
// ==========================================================================
// mtTaskInitSystem
/*!
 *	MT タスクシステム 初期化
 */
// ==========================================================================
extern void mtTaskInitSystem(void);

// ==========================================================================
// mtTaskExitSystem
/*!
 *	MT タスクシステム 終了処理
 */
// ==========================================================================
extern void mtTaskExitSystem(void);

// ==========================================================================
// mtTaskMake
/*!
 *	タスク生成
 *
 *	@param	proc		[in]	処理関数のポインタ (無い場合は NULL)
 *	@param	dest		[in]	消去時の処理関数へのポインタ (無い場合は NULL)
 *	@param	prio		[in]	優先度 (0x0000 ～ 0xFFFF)
 *	@param	name		[in]	タスクの名前
 *	@param	group		[in]	0 ～ 14
 *	@param	pause_level	[in]	ポーズレベル
 *	@param	work_size	[in]	取得ワークサイズ
 */
// ==========================================================================
extern MTS_TASK_TCB* mtTaskMake(GSF_TASK_PROCEDURE proc, GSF_TASK_DESTRUCTOR dest,
			u32 flag, u16 pause_level, u32 prio, s32 group, u32 work_size, const char *name);

// ==========================================================================
// mtTaskMakeMT
/*!
 *	タスク生成 MT
 *
 *	@param	proc		[in]	処理関数
 *	@param	dest		[in]	終了処理関数
 *	@param	flag		[in]	mt用フラグ 参照無し
 *	@param	pause_level	[in]	mt用ポーズレベル
 *	@param	prio		[in]	タスク優先
 *	@param	group		[in]	タスクグループ(0～14)
 *	@param	work_size	[in]	ワークサイズ
 *	@param	name		[in]	デバッグ用TCB名(NULL可)
 *
 *	@return   生成タスクのTCB
 *
 *	@note
 *		MTと同じ形式で呼び出します。\n
 *		flagは無効になります。\n
 *		pause_levelは、UserIDに置き換わります。\n
 */
// ==========================================================================
//extern MTS_TASK_TCB* mtTaskMakeMT(GSF_TASK_PROCEDURE proc, GSF_TASK_DESTRUCTOR dest, u32 flag, u16 pause_level, u32 prio, u32 group, u32 work_size, char *name);

// ==========================================================================
// mtTaskGetAmTcb
/*!
 *	MT タスクシステム  AM TCB取得
 *
 *	@param	tcb [in]	GS TCB
 */
// ==========================================================================
extern AMS_TCB* mtTaskGetAmTcb(MTS_TASK_TCB *tcb);

// ==========================================================================
// mtTaskGetTcbWork
/*!
 *	MT タスクシステム  TCBワーク取得
 *
 *	@param	tcb [in]	GS TCB
 */
// ==========================================================================
extern void* mtTaskGetTcbWork(MTS_TASK_TCB *tcb);

// ==========================================================================
// mtTaskChangeTcbProcedure
/*!
 *	処理関数変更
 *
 *	@param	tcb		[in]	GS TCB
 *	@param	proc	[in]	処理関数のアドレス
 */
// ==========================================================================
extern void mtTaskChangeTcbProcedure(MTS_TASK_TCB *tcb, GSF_TASK_PROCEDURE proc);

// ==========================================================================
// mtTaskChangeTcbDestructor
/*!
 *	終了関数変更
 *
 *	@param	tcb		[in]	GS TCB
 *	@param	dest	[in]	終了関数のアドレス
 */
// ==========================================================================
extern void mtTaskChangeTcbDestructor(MTS_TASK_TCB *tcb, GSF_TASK_DESTRUCTOR dest);

// ==========================================================================
// mtTaskClearTcb
/*!
 *	タスクを削除する
 *
 *	@param	tcb		[in]	GS TCB
 */
// ==========================================================================
extern void mtTaskClearTcb(MTS_TASK_TCB *tcb);

// ==========================================================================
// mtTaskClearTcbAll
/*!
 *	タスクを全て削除する
 *
 *	@param	tcb		[in]	GS TCB
 */
// ==========================================================================
extern void mtTaskClearTcbAll(void);

// ==========================================================================
// mtTaskClearPriority
/*!
 *	タスクを優先範囲削除する
 *
 *	@param	prio_begin	[in]	削除開始優先(以上)
 *	@param	prio_end	[in]	削除開始優先(以下)
 */
// ==========================================================================
extern void mtTaskClearPriority(u32 prio_begin, u32 prio_end);

// ==========================================================================
// mtTaskClearGroup
/*!
 *	タスクをグループ削除する
 *
 *	@param	group		[in]	削除対象グループ
 */
// ==========================================================================
extern void mtTaskClearGroup(u32 group);

// ==========================================================================
// mtTaskStartPause
/*!
 *	タスクをポーズ状態にする
 *
 *	@param	pause_level		[in]	ポーズレベル
 *
 *	@note
 *		TCBポーズレベル <= pause_levelのタスクが停止します
 */
// ==========================================================================
extern void mtTaskStartPause(u16 pause_level);

// ==========================================================================
// mtTaskEndPause
/*!
 *	タスクポーズを終了する
 */
// ==========================================================================
extern void mtTaskEndPause(void);

// ==========================================================================
// mtTaskIsPaused
/*!
 *	タスクのポーズ状態をチェックする
 *
 *	@param	pause_level	[out]	ポーズレベル NULL可
 *
 *	@return	TRUE : ポーズ中		FALSE : 処理実行中
 *
 *	@note
 *		return : FALSE の場合は、pause_levelの設定値は0になります。
 */
// ==========================================================================
extern BOOL mtTaskIsPaused(u16 *pause_level);





////////////////////////////////////////////////////////////////////////////////
#if 0

// ==========================================================================
// GsTaskPending
/*!
 *	GS タスク  実行を保留する
 *
 *	@param	tcb		[in]	GS TCB
 */
// ==========================================================================
inline void GsTaskPending(MTS_TASK_TCB *tcb)
{
	amAssert(tcb);

	amTaskPending(tcb->am_tcb);
}

// ==========================================================================
// GsTaskStart
/*!
 *	GS タスク  実行を許可する
 *
 *	@param	tcb		[in]	GS TCB
 */
// ==========================================================================
inline void GsTaskStart(MTS_TASK_TCB *tcb)
{
	amAssert(tcb);

	amTaskStart(tcb->am_tcb);
}

// ==========================================================================
// GsTaskDelete
/*!
 *	GS タスク  タスクを削除する
 *
 *	@param	tcb		[in]	GS TCB
 */
// ==========================================================================
inline void GsTaskDelete(MTS_TASK_TCB *tcb)
{
	amAssert(tcb);

	amTaskDelete(tcb->am_tcb);
}

// ==========================================================================
// GsTaskDeleteGroup
/*!
 *	GS タスク  タスクをグループ削除する
 *
 *	@param	user		[in]	削除対象ユーザ (AMD_TASK_UID_ALL または 0 : 全ユーザ)
 *	@param	attr		[in]	削除対象属性フラグ
 *	@param	flag		[in]	attr の評価方法
 *								AMD_TASK_DELGRP_INCLUSIVE    : 一部条件
 *								AMD_TASK_DELGRP_EXCLUSIVE    : 全条件
 *								AMD_TASK_DELGRP_NOTINCLUSIVE : 一部条件否定
 *								AMD_TASK_DELGRP_NOTEXCLUSIVE : 全条件否定
 */
// ==========================================================================
#define GsTaskDeleteGroup(user, attr, flag)	amTaskDeleteGroup(user, attr, flag)

// ==========================================================================
// GsTaskDeletePriority
/*!
 *	GS タスク  タスクを優先範囲削除する
 *
 *	@param	prio_begin	[in]	削除開始優先(以上)
 *	@param	prio_end	[in]	削除開始優先(以下)
 *	@param	user		[in]	削除対象ユーザ (AMD_TASK_UID_ALL または 0 : 全ユーザ)
 */
// ==========================================================================
#define GsTaskDeletePriority(prio_begin, prio_end, user)	amTaskDeletePriority(prio_begin, prio_end, user)

// ==========================================================================
// GsTaskSleep
/*!
 *	GS タスク  タスク停止
 *
 *	@param	tcb		[in]	GS TCB
 */
// ==========================================================================
inline void GsTaskSleep(MTS_TASK_TCB *tcb)
{
	amAssert(tcb);

	amTaskSleep(tcb->am_tcb);
}

// ==========================================================================
// GsTaskSleepGroup
/*!
 *	GS タスク  タスクをグループ停止する
 *
 *	@param	user		[in]	停止対象ユーザ (AMD_TASK_UID_ALL または 0 : 全ユーザ)
 *	@param	attr		[in]	停止対象属性フラグ
 *	@param	flag		[in]	attr の評価方法
 *								AMD_TASK_DELGRP_INCLUSIVE    : 一部条件
 *								AMD_TASK_DELGRP_EXCLUSIVE    : 全条件
 *								AMD_TASK_DELGRP_NOTINCLUSIVE : 一部条件否定
 *								AMD_TASK_DELGRP_NOTEXCLUSIVE : 全条件否定
 */
// ==========================================================================
#define GsTaskSleepGroup(user, attr, flag)	amTaskSleepGroup(user, attr, flag)

// ==========================================================================
// GsTaskSleepPriority
/*!
 *	GS タスク  タスクを優先範囲停止する
 *
 *	@param	prio_begin	[in]	停止開始優先(以上)
 *	@param	prio_end	[in]	停止開始優先(以下)
 *	@param	user		[in]	停止対象ユーザ (AMD_TASK_UID_ALL または 0 : 全ユーザ)
 */
// ==========================================================================
#define GsTaskSleepPriority(prio_begin, prio_end, user)	amTaskSleepPriority(prio_begin, prio_end, user)

// ==========================================================================
// GsTaskWakeup
/*!
 *	GS タスク  タスク再開
 *
 *	@param	tcb		[in]	GS TCB
 */
// ==========================================================================
inline void GsTaskWakeup(MTS_TASK_TCB *tcb)
{
	amAssert(tcb);

	amTaskWakeup(tcb->am_tcb);
}

// ==========================================================================
// GsTaskWakeupGroup
/*!
 *	GS タスク  タスクをグループ再開する
 *
 *	@param	user		[in]	再開対象ユーザ (AMD_TASK_UID_ALL または 0 : 全ユーザ)
 *	@param	attr		[in]	再開対象属性フラグ
 *	@param	flag		[in]	attr の評価方法
 *								AMD_TASK_DELGRP_INCLUSIVE    : 一部条件
 *								AMD_TASK_DELGRP_EXCLUSIVE    : 全条件
 *								AMD_TASK_DELGRP_NOTINCLUSIVE : 一部条件否定
 *								AMD_TASK_DELGRP_NOTEXCLUSIVE : 全条件否定
 */
// ==========================================================================
#define GsTaskWakeupGroup(user, attr, flag)	amTaskWakeupGroup(user, attr, flag)

// ==========================================================================
// GsTaskWakeupPriority
/*!
 *	GS タスク  タスクを優先範囲再開する
 *
 *	@param	prio_begin	[in]	再開開始優先(以上)
 *	@param	prio_end	[in]	再開開始優先(以下)
 *	@param	user		[in]	再開対象ユーザ (AMD_TASK_UID_ALL または 0 : 全ユーザ)
 */
// ==========================================================================
#define GsTaskWakeupPriority(prio_begin, prio_end, user)	amTaskWakeupPriority(prio_begin, prio_end, user)



#endif


#if	defined(__cplusplus)
} /* extern "C" */
#endif


#endif // _PT_H_

//----- Include Files -------------------------------------------------------
