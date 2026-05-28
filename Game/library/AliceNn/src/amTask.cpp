/************************************************************************/
/*      amTask.cpp                                                      */
/*            Copyright(c) 2000-2009 Dimps CORP. All Rights Reserved.   */
/*----------------------------------------------------------------------*/
/* タスクライブラリ                                                     */
/*----------------------------------------------------------------------*/
/* Date      Ver.   Comment                                             */
/* 20000918  0.10   簡易バージョン                                      */
/* 20001006  1.00   正式版                                              */
/* 20001011  1.10   タスクリスト表示機能                                */
/* 20001012  1.11   ワークの先頭位置を 16byte アライン                  */
/* 20010622  1.12   CPUカウント消費量表示機能を付加 (いやーん)          */
/************************************************************************/

/*----- Update Logs ----------------------------------------------------*/

/*----- Include files --------------------------------------------------*/

#include "alice.h"

/*----- definitions ----------------------------------------------------*/

#if AMD_DEBUG && (AMD_TASK_THREAD_NUM > 1)
#define AMD_TASK_DEBUG			(1)
#else
#define AMD_TASK_DEBUG			(0)
#endif


/*----- Macros ---------------------------------------------------------*/

// タスクリストの表示数
#define AMD_TASK_LIST_LENGTH  (14)


/*----- External valiables ---------------------------------------------*/

/*----- Global valiables -----------------------------------------------*/

AMS_TASK	*_am_default_taskp = NULL;

const AMS_TASKLIST_OWNER *_am_owner_list = NULL;
int _am_szOwnerList = 0;

Sint32 _am_display_tasklist = 1;	// Task List 0:off !0:on
Sint32 _am_tlist_cline = 0;			// 選択中のタスクのインデックス
Sint32 _am_tlist_dline = 0;			// 表示タスクリストの先頭インデックス


/*----- Local declarations ---------------------------------------------*/

void _amTaskExecuteInOrder(AMS_TASK *taskp);

#if AMD_TASK_THREAD_NUM > 1
#if _PC
DWORD WINAPI _amTaskExec(DWORD arg);
#elif _XBOX
DWORD WINAPI _amTaskExec(DWORD arg);
#elif _PS3
void _amTaskExec(uint64_t arg);
#endif
AMS_TCB *_amTaskGetEnableTcb(AMS_TASK *taskp, Sint32 thread_id);
void _amTaskWakeupThread(AMS_TASK *taskp);
static void amTaskDisplayThread(AMS_TASK *taskp);
#endif

static void _amTaskDeleteReal(AMS_TCB *tcb);
static void _amTaskCheckWork(AMS_TASK *taskp);

static void amTaskDisplayList(AMS_TASK *taskp, int locx, int locy);


/*----- Inline functions -----------------------------------------------*/

inline AMS_TCB *amTaskNextTcb(AMS_TCB *tcbp)
{
	return	(AMS_TCB *)((Sint32)tcbp
			+ sizeof(AMS_TCB) + sizeof(AMS_TCB_FOOTER) + tcbp->taskp->tcb_work_size);
}


inline AMS_TCB *amPrevNextTcb(AMS_TCB *tcbp)
{
	return	(AMS_TCB *)((Sint32)tcbp
			- sizeof(AMS_TCB) - sizeof(AMS_TCB_FOOTER) - tcbp->taskp->tcb_work_size);
}


inline AMS_TCB_FOOTER *amTaskGetTcbFooter(AMS_TCB *tcbp)
{
	return	(AMS_TCB_FOOTER *)((Sint32)tcbp + sizeof(AMS_TCB)
			+ tcbp->taskp->tcb_work_size);
}


/*----- Global functions -----------------------------------------------*/

/************************************************************************/
/* AMS_TASK *amTaskInitSystem(Sint32 max_tcb, Sint32 work_size,         */
/*                                                   Sint32 thread_num) */
/*----------------------------------------------------------------------*/
/* [INPUT]  max_tcb    : ＴＣＢの最大数                                 */
/*          work_size  : ＴＣＢユーザーワークのサイズ                   */
/*          thread_num : ＴＣＢスレッド数                               */
/* [RETURN] タスクシステム構造体のポインタ                              */
/* [FUNCTION] タスクシステム初期化                                      */
/************************************************************************/
AMS_TASK *amTaskInitSystem(Sint32 max_tcb, Sint32 work_size, Sint32 thread_num)
{
	AMS_TASK	*taskp;

	taskp		= (AMS_TASK *)amMemAllocSystem(sizeof(AMS_TASK));
	if (_am_default_taskp == NULL)
		_am_default_taskp	= taskp;

	taskp->tcb_max		= max_tcb;
	taskp->tcb_num		= 0;
	taskp->tcb_alloc	= 0;
	taskp->tcb_free		= 0;

	// TCBワークサイズの定義
	taskp->tcb_work_size	= (work_size + 63) & ~63;
	taskp->tcb_buffer_size	= max_tcb;

	// TCBバッファの確保
	taskp->tcb_buffer	= (AMS_TCB *)amMemAllocSystem(taskp->tcb_buffer_size *
			(taskp->tcb_work_size + sizeof(AMS_TCB) + sizeof(AMS_TCB_FOOTER)));
	taskp->tcb_bufp		= (AMS_TCB **)amMemAllocSystem(taskp->tcb_buffer_size * 4);

#if AMD_TASK_DEBUG
	// TCBリポートバッファの確保
	taskp->tcb_report_num		= 0;
	taskp->tcb_report	= (AMS_TCB_REPORT *)amMemAllocSystem(
			taskp->tcb_buffer_size * sizeof(AMS_TCB_REPORT));
#endif

	// 先頭TCB の初期化
	strncpy(taskp->tcb_head.name, "TCB Head", AMD_TASK_NAME_LEN);
	taskp->tcb_head.priority	= 0x00000000;
	taskp->tcb_head.prev		= NULL;
	taskp->tcb_head.next		= &taskp->tcb_tail;
	taskp->tcb_head.user_id		= AMD_TASK_UID_NONE;
	taskp->tcb_head.attribute	= AMD_TASK_ATTR_SYSTEM;
	taskp->tcb_head.wkbegin		= AMD_TASK_WORK_CODE;
//	taskp->tcb_head.wkend		= AMD_TASK_WORK_CODE;

	amTaskSetProcedure(&taskp->tcb_head, (TaskProc)AMD_TASK_STATE_NULL);
	amTaskSetDestructor(&taskp->tcb_head, (TaskProc)AMD_TASK_STATE_NULL);

	// 末尾TCB の初期化
	strncpy(taskp->tcb_tail.name, "TCB Tail", AMD_TASK_NAME_LEN);
	taskp->tcb_tail.priority	= 0xFFFFFFFF;
	taskp->tcb_tail.prev		= &taskp->tcb_head;
	taskp->tcb_tail.next		= NULL;
	taskp->tcb_tail.user_id		= AMD_TASK_UID_NONE;
	taskp->tcb_tail.attribute	= AMD_TASK_ATTR_SYSTEM;
	taskp->tcb_tail.wkbegin		= AMD_TASK_WORK_CODE;
//	taskp->tcb_tail.wkend		= AMD_TASK_WORK_CODE;
#if AMD_TASK_THREAD_NUM > 1
	taskp->tcb_tail.thread		= (AMS_TCB_THREAD *)AMD_TASK_TCB_STAT_WAIT;
#endif

	amTaskSetProcedure(&taskp->tcb_tail, (TaskProc)AMD_TASK_STATE_NULL);
	amTaskSetDestructor(&taskp->tcb_tail, (TaskProc)AMD_TASK_STATE_NULL);
    
	// バッファポインタの初期化とワーク破壊検査コードの書き込み
{
	AMS_TCB		*tcbp = taskp->tcb_buffer;
	AMS_TCB_FOOTER	*footer;
	for (int i = 0; i < taskp->tcb_buffer_size; i++) {
		taskp->tcb_bufp[i]	= tcbp;
		tcbp->taskp			= taskp;
		footer		= amTaskGetTcbFooter(tcbp);
		tcbp->footer		= footer;
		tcbp->wkbegin		= AMD_TASK_WORK_CODE;
		footer->wkend		= AMD_TASK_WORK_CODE;
		tcbp		= amTaskNextTcb(tcbp);
	}
}

#if AMD_TASK_THREAD_NUM > 1
{
	AMS_TCB_THREAD	*thread;
	Sint32		i, prio;
	amMutexCreate(&taskp->mutex);
	amAlarmCreate(&taskp->alarm_end);
	taskp->tcb_thread_num	= thread_num;
	taskp->tcb_top			= NULL;
	taskp->tcb_stall		= (Uint32)AMD_TASK_GROUP_ALL;
	for (i = 0; i < 32; i++)
		taskp->tcb_stall_count[i]	= 0;
	thread		= &taskp->tcb_thread[0];
	for (i = 0; i < thread_num; i++, thread++) {
		thread->id		= i;
		thread->taskp	= taskp;
#if AMD_TASK_DEBUG
		thread->report_head.next	= NULL;
		thread->report_tail			= &thread->report_head;
#endif

		// マトリクススタックの生成
		thread->matrix_stack_buf	= amMemAllocSystem(
				AMD_MATRIX_STACK_SIZE * sizeof(NNS_MATRIX));
		nnSetUpMatrixStack(&thread->matrix_stack,
				thread->matrix_stack_buf, AMD_MATRIX_STACK_SIZE);

		// 起床アラームの生成
		amAlarmCreate(&thread->alarm_wakeup);

		// ＴＣＢ実行スレッドの生成
#if _PC
		prio	= THREAD_PRIORITY_NORMAL;
#elif _XBOX
		prio	= THREAD_PRIORITY_NORMAL;
#elif _PS3
		sys_ppu_thread_t	id;
		sys_ppu_thread_get_id(&id);
		sys_ppu_thread_get_priority(id, &prio);
		prio	-= 1;
#endif
		thread->thread_id	= amThreadCreate(&thread->thread,
				_amTaskExec, thread, (AMD_CORE)i, prio, 0x4000, "TCB");
	}
}
#else
	UNREFERENCED_PARAMETER(thread_num);
#endif

	return	taskp;
}


/************************************************************************/
/* void amTaskExitSystem(AMS_TASK *taskp)                               */
/*----------------------------------------------------------------------*/
/* [INPUT]  taskp : タスクシステム構造体のポインタ                      */
/* [FUNCTION] タスクシステム終了                                        */
/************************************************************************/
void amTaskExitSystem(AMS_TASK *taskp)
{
	amAssert(taskp);

#if AMD_TASK_THREAD_NUM > 1
{
	AMS_TCB_THREAD	*thread;
	Sint32		i;
	for (i = 0; i < taskp->tcb_thread_num; i++) {
		thread		= &taskp->tcb_thread[i];
		amThreadExit(&thread->thread);
		amAlarmSet(&thread->alarm_wakeup);
		amThreadWaitQuit(&thread->thread);
		amMemFreeSystem(thread->matrix_stack_buf);
		amAlarmDelete(&thread->alarm_wakeup);
		amThreadDelete(&thread->thread);
	}
	amAlarmDelete(&taskp->alarm_end);
	amMutexDelete(&taskp->mutex);
}
#endif

	amMemFreeSystem(taskp->tcb_buffer);
	amMemFreeSystem(taskp->tcb_bufp);
	amMemFreeSystem(taskp);
}


/************************************************************************/
/* void amTaskReset(AMS_TASK *taskp)                                    */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    taskp      : タスクシステム構造体                                 */
/* [FUNCTION]                                                           */
/*    タスクの全消去                                                    */
/************************************************************************/
void amTaskReset(AMS_TASK *taskp)
{
	amAssert(taskp);

	taskp->tcb_num		= 0;
	taskp->tcb_alloc	= 0;
	taskp->tcb_free		= 0;
	taskp->tcb_head.next		= &taskp->tcb_tail;
	taskp->tcb_tail.prev		= &taskp->tcb_head;

	AMS_TCB		*tcbp = taskp->tcb_buffer;
	AMS_TCB_FOOTER	*footer;
	for (int i = 0; i < taskp->tcb_buffer_size; i++) {
		taskp->tcb_bufp[i]	= tcbp;
		tcbp->taskp			= taskp;
		footer		= amTaskGetTcbFooter(tcbp);
		tcbp->footer		= footer;
		tcbp->wkbegin		= AMD_TASK_WORK_CODE;
		footer->wkend		= AMD_TASK_WORK_CODE;
		tcbp		= amTaskNextTcb(tcbp);
	}
}


/************************************************************************/
/* void amTaskExecute(AMS_TASK *taskp)                                  */
/*----------------------------------------------------------------------*/
/* [INPUT]  taskp : 実行するタスクシステム                              */
/* [FUNCTION] タスク実行                                                */
/************************************************************************/
void amTaskExecute(AMS_TASK *taskp)
{
#if AMD_TASK_THREAD_NUM == 1
	_amTaskExecuteInOrder(taskp);
#else
	if (taskp->tcb_thread_num == 1) {
		_amTaskExecuteInOrder(taskp);
		return;
	}

	AMS_TCB			*tcb;
	Sint32			i;

	// タスクスレッド実行
	amMutexLock(&taskp->mutex);
	taskp->tcb_top		= taskp->tcb_head.next;
	taskp->tcb_stall	= 0;
#if AMD_TASK_DEBUG
{
	AMS_TIMER	timer;
	amTimerCreate(&timer);
	amTimerStart(&timer);
	taskp->tcb_start_count	= timer.count_start;
	amTimerDelete(&timer);
	taskp->tcb_report_num	= 0;
}
#endif
	amAlarmClear(&taskp->alarm_end);
	for (i = 0; i < taskp->tcb_thread_num; i++) {
#if AMD_TASK_DEBUG
		taskp->tcb_thread[i].report_head.next	= NULL;
		taskp->tcb_thread[i].report_tail		=
				&taskp->tcb_thread[i].report_head;
#endif
		amAlarmClear(&taskp->tcb_thread[i].alarm_wakeup);
		amAlarmSet(&taskp->tcb_thread[i].alarm_wakeup);
	}
	amMutexUnlock(&taskp->mutex);

	// タスク実行完了待ち
	amAlarmWait(&taskp->alarm_end);

	amMutexLock(&taskp->mutex);

	// タスク削除ループ
	tcb		= taskp->tcb_head.next;
	while (tcb != &taskp->tcb_tail) {
		tcb->thread		= (AMS_TCB_THREAD *)AMD_TASK_TCB_STAT_WAIT;
		if (tcb->procedure == (TaskProc)AMD_TASK_STATE_KILL)
			_amTaskDeleteReal(tcb);
		tcb		= tcb->next;
	}

#if AMD_DEBUG
	if (taskp == _am_default_taskp) {
#if AMD_USE_KEYBD
		if (amGetKeybdModStat(KBD_MOD_LCTRL | KBD_MOD_RCTRL) &&
			amGetKeybdStand(KBD_T))
			_am_display_tasklist	= (_am_display_tasklist) ? 0 : 1;
#endif // AMD_USE_KEYBD
		_amTaskCheckWork(taskp);
		if (_am_display_tasklist) {
//			amTaskDisplayList(taskp, 1, 2);
			amTaskDisplayThread(taskp);
		}
	}
#endif // AMD_DEBUG
	amMutexUnlock(&taskp->mutex);
#endif
}


/************************************************************************/
/* AMS_TCB *amTaskMake(AMS_TASK *taskp,                                 */
/*                      TaskProc proc, TaskProc dest, Uint16 prio,      */
/*                           Uint32 user, Uint32 attr, char *name,      */
/*                           Uint32 stall, Sint32 group, Uint32 run)    */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    taskp : タスクシステム構造体                                      */
/*    proc  : 処理関数のポインタ (無い場合は NULL)                      */
/*    dest  : 消去時の処理関数へのポインタ (無い場合は NULL)            */
/*    prio  : 優先度 (0x0000 ～ 0xFFFF)                                 */
/*    user  : 所有者 (0:所有者設定なし(グループ削除対象外))             */
/*    attr  : 属性                                                      */
/*    name  : タスクの名前                                              */
/*    stall : 実行待機TCBグループマスク                                 */
/*    group : TCBグループID                                             */
/*    run   : 実行可能スレッドマスク                                    */
/* [RETURN]                                                             */
/*    TCB へのポインタ                                                  */
/* [FUNCTION]                                                           */
/*    タスク生成                                                        */
/************************************************************************/
AMS_TCB *amTaskMake(AMS_TASK *taskp, TaskProc proc, TaskProc dest, Uint32 prio, Uint32 user, Uint32 attr, char *name, Uint32 stall, Sint32 group, Uint32 run)
{
	amAssert(taskp);

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&taskp->mutex);
#else
	UNREFERENCED_PARAMETER(stall);
	UNREFERENCED_PARAMETER(group);
	UNREFERENCED_PARAMETER(run);
#endif

    // tcb num +1
    taskp->tcb_num++;

    // タスクオーバー
    amAssert(taskp->tcb_num <= taskp->tcb_max);

    // プールから TCB を取得
	AMS_TCB	*tcb;
	tcb		= taskp->tcb_bufp[taskp->tcb_alloc];
    taskp->tcb_alloc	= (taskp->tcb_alloc + 1) & (taskp->tcb_max - 1);

	// 変な定義値をチェック
	amAssert(prio <= 0xFFFF);

	// 初期化
	AMS_TCB_FOOTER	*footer;
	memset(tcb->name, 0, AMD_TASK_NAME_LEN);
	strncpy(tcb->name, name, AMD_TASK_NAME_LEN - 1);
	footer	= amTaskGetTcbFooter(tcb);
	footer->cpu_cnt		= 0;
	footer->cpu_cnt_max	= 0;
	tcb->priority	= prio;
	tcb->priority	= prio;
	tcb->user_id	= user;
	tcb->attribute	= attr;
#if AMD_TASK_THREAD_NUM > 1
	tcb->stall_group	= stall;
	tcb->group_id	= group;
	tcb->thread		= (AMS_TCB_THREAD *)AMD_TASK_TCB_STAT_PENDING;
	tcb->run_thread	= run;
#endif
	amTaskSetProcedure(tcb, proc);
	amTaskSetDestructor(tcb, dest);

	// リストに接続
	AMS_TCB *tmp;
	for (tmp = taskp->tcb_head.next;
		(tmp != &taskp->tcb_tail) && (tmp->priority <= prio);
			tmp = tmp->next) { ;}
	tmp->prev->next	= tcb;
	tcb->prev	= tmp->prev;
	tmp->prev	= tcb;
	tcb->next	= tmp;

#if AMD_TASK_THREAD_NUM > 1
	_amTaskWakeupThread(taskp);

	amMutexUnlock(&taskp->mutex);
#endif

	return	tcb;
}


/************************************************************************/
/* Sint32 amTaskPending(AMS_TCB *tcbp)                                  */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    tcbp : 実行を保留するTCBへのポインタ                              */
/* [RETURN]                                                             */
/*    TCB の状態                                                        */
/* [FUNCTION]                                                           */
/*    タスクの実行保留                                                  */
/************************************************************************/
Sint32 amTaskPending(AMS_TCB *tcbp)
{
#if AMD_TASK_THREAD_NUM > 1
	AMS_TASK	*taskp;
	Sint32		ret;

	taskp	= tcbp->taskp;

	amMutexLock(&taskp->mutex);

	ret		= (Sint32)tcbp->thread;
	if (ret == AMD_TASK_TCB_STAT_WAIT) {
		tcbp->thread	= (AMS_TCB_THREAD *)AMD_TASK_TCB_STAT_PENDING;
//		_amTaskWakeupThread(taskp);
	} else if (ret > 0)
		ret		= AMD_TASK_TCB_STAT_RUNNING;

	amMutexUnlock(&taskp->mutex);

	return	ret;
#else
	UNREFERENCED_PARAMETER(tcbp);
	return	0;
#endif
}


/************************************************************************/
/* Sint32 amTaskStart(AMS_TCB *tcbp)                                    */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    tcbp : 実行許可するTCBへのポインタ                                */
/* [RETURN]                                                             */
/*    TCB の状態                                                        */
/* [FUNCTION]                                                           */
/*    タスクの実行許可                                                  */
/************************************************************************/
Sint32 amTaskStart(AMS_TCB *tcbp)
{
#if AMD_TASK_THREAD_NUM > 1
	AMS_TASK	*taskp;
	Sint32		ret;

	taskp	= tcbp->taskp;

	amMutexLock(&taskp->mutex);

	ret		= (Sint32)tcbp->thread;
	if (ret == AMD_TASK_TCB_STAT_PENDING) {
		tcbp->thread	= (AMS_TCB_THREAD *)AMD_TASK_TCB_STAT_WAIT;
		_amTaskWakeupThread(taskp);
	} else if (ret > 0)
		ret		= AMD_TASK_TCB_STAT_RUNNING;

	amMutexUnlock(&taskp->mutex);

	return	ret;
#else
	UNREFERENCED_PARAMETER(tcbp);
	return	0;
#endif
}


/************************************************************************/
/* Sint32 amTaskDelete(AMS_TCB *tcb)                                    */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    tcb : 削除するタスクの TCB へのポインタ                           */
/* [FUNCTION]                                                           */
/*    タスク消去                                                        */
/************************************************************************/
void amTaskDelete(AMS_TCB *tcb)
{
	// なんじゃそら
	amAssert(tcb /* amDeleteTask */);

	// 二度消し対策
	if (tcb->procedure == (TaskProc)AMD_TASK_STATE_KILL)
		return;

	// デストラクタ
	if (((Sint32)tcb->destructor) != NULL)
		tcb->destructor(tcb);

	// KILL フラグ
	tcb->procedure	= (TaskProc)AMD_TASK_STATE_KILL;
}


/************************************************************************/
/* void amTaskDeleteGroup(Uint32 user, Uint32 attr, Uint32 flag)        */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    taskp : タスクシステム構造体                                      */
/*    user	: 削除対象ユーザ (AMD_TASK_UID_ALL または 0 : 全ユーザ)     */
/*    attr  : 削除対象属性フラグ                                        */
/*    flag  : attr の評価方法                                           */
/*           AMD_TASK_DELGRP_INCLUSIVE    : 一部条件                    */
/*           AMD_TASK_DELGRP_EXCLUSIVE    : 全条件                      */
/*           AMD_TASK_DELGRP_NOTINCLUSIVE : 一部条件否定                */
/*           AMD_TASK_DELGRP_NOTEXCLUSIVE : 全条件否定                  */
/* [FUNCTION]                                                           */
/*    タスクのグループ消去                                              */
/************************************************************************/
void amTaskDeleteGroup(AMS_TASK *taskp, Uint32 user, Uint32 attr, Uint32 flag)
{
	AMS_TCB	*tcb;

	amAssert(taskp);

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&taskp->mutex);
#endif

	if (user == 0)
		user	= (Uint32)AMD_TASK_UID_ALL;
	tcb		= taskp->tcb_head.next;

	switch (flag) {
		case	AMD_TASK_DELGRP_INCLUSIVE:		// ひとつでも満たす
			while (tcb != &taskp->tcb_tail) {
				if ((tcb->user_id & user) && ((tcb->attribute & attr) != 0))
					amTaskDelete(tcb);
			tcb		= tcb->next;
		}
		break;

		case	AMD_TASK_DELGRP_EXCLUSIVE:		// 全部満たす
			while (tcb != &taskp->tcb_tail) {
				if ((tcb->user_id & user) && ((tcb->attribute & attr) == attr))
					amTaskDelete(tcb);
			tcb		= tcb->next;
		}
		break;

		case	AMD_TASK_DELGRP_NOTINCLUSIVE:	// ひとつも満たさない
			while (tcb != &taskp->tcb_tail) {
				if ((tcb->user_id & user) && ((tcb->attribute & attr) == 0))
					amTaskDelete(tcb);
			tcb		= tcb->next;
		}
		break;

		case	AMD_TASK_DELGRP_NOTEXCLUSIVE:	// 全部満たさない
			while (tcb != &taskp->tcb_tail) {
				if ((tcb->user_id & user) && ((tcb->attribute & attr) != attr))
					amTaskDelete(tcb);
			tcb		= tcb->next;
		}
		break;

		default:
			// そんなフラグはないよ
			amAssert(0);
			break;
	}

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&taskp->mutex);
#endif
}


/************************************************************************/
/* void amTaskDeletePriority(AMS_TASK *taskp,                           */
/*                   Uint32 prio_begin, Uint32 prio_end, Uint32 user)   */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    taskp      : タスクシステム構造体                                 */
/*    prio_begin : 削除開始優先(以上)                                   */
/*    prio_end   : 削除開始優先(以下)                                   */
/*    user       : 削除対象ユーザ                                       */
/*                 AMD_TASK_UID_ALL - 全ユーザ                          */
/*                 0 (default)      - 全タスク(ユーザ指定なしも含む)    */
/* [FUNCTION]                                                           */
/*    タスクの優先範囲消去                                              */
/************************************************************************/
void amTaskDeletePriority(AMS_TASK *taskp, Uint32 prio_begin, Uint32 prio_end, Uint32 user)
{
	amAssert(taskp);

	// 変な値をチェック
	amAssert(prio_begin <= 0xFFFF);
	amAssert(prio_end <= 0xFFFF);
	amAssert(prio_begin <= prio_end);

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&taskp->mutex);
#endif

	AMS_TCB		*tcb;
	tcb		= taskp->tcb_head.next;

	// serach
	while ((tcb != &taskp->tcb_tail) && (tcb->priority < prio_begin))
		tcb		= tcb->next;

	// delete
	while ((tcb != &taskp->tcb_tail) && (tcb->priority <= prio_end)) {
		if ((user == 0) || (tcb->user_id & user))
			amTaskDelete(tcb);
		tcb		= tcb->next;
	}

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&taskp->mutex);
#endif
}


/************************************************************************/
/* void amTaskSleep(AMS_TCB *tcb)                                       */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    tcb : 停止するタスクの TCB へのポインタ                           */
/* [FUNCTION]                                                           */
/*    タスク停止                                                        */
/************************************************************************/
void amTaskSleep(AMS_TCB *tcb)
{
	// なんじゃそら
	amAssert(tcb /* amSleepTask */);

#if _PC
	if ((Sint32)tcb->procedure > 0)
		*(Sint32 *)&tcb->procedure	= -(Sint32)tcb->procedure;
#else
	*(Sint32 *)&tcb->procedure	|= AMD_TASK_STATE_SLEEP_MASK;
#endif
}

/************************************************************************/
/* void amTaskSleepGroup(AMS_TASK *taskp,                               */
/*                              Uint32 user, Uint32 attr, Uint32 flag)  */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    taskp : タスクシステム構造体                                      */
/*    user  : 停止対象ユーザ (AMD_TASK_UID_ALL または 0 : 全ユーザ)     */
/*    attr  : 停止対象属性フラグ                                        */
/*    flag  : attr の評価方法                                           */
/*           AMD_TASK_SLPGRP_INCLUSIVE    : 一部条件                    */
/*           AMD_TASK_SLPGRP_EXCLUSIVE    : 全条件                      */
/*           AMD_TASK_SLPGRP_NOTINCLUSIVE : 一部条件否定                */
/*           AMD_TASK_SLPGRP_NOTEXCLUSIVE : 全条件否定                  */
/* [FUNCTION]                                                           */
/*    タスクのグループ停止                                              */
/************************************************************************/
void amTaskSleepGroup(AMS_TASK *taskp, Uint32 user, Uint32 attr, Uint32 flag)
{
	AMS_TCB	*tcb;

	amAssert(taskp);

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&taskp->mutex);
#endif

	if (user == 0)
		user	= (Uint32)AMD_TASK_UID_ALL;
	tcb		= taskp->tcb_head.next;

	switch (flag) {
		case	AMD_TASK_SLPGRP_INCLUSIVE:		// ひとつでも満たす
			while (tcb != &taskp->tcb_tail) {
				if ((tcb->user_id & user) && ((tcb->attribute & attr) != 0))
					amTaskSleep(tcb);
				tcb		= tcb->next;
			}
			break;

		case	AMD_TASK_SLPGRP_EXCLUSIVE:		// 全部満たす
			while (tcb != &taskp->tcb_tail) {
				if ((tcb->user_id & user) && ((tcb->attribute & attr) == attr))
					amTaskSleep(tcb);
				tcb		= tcb->next;
			}
			break;

		case	AMD_TASK_SLPGRP_NOTINCLUSIVE:	// ひとつも満たさない
			while (tcb != &taskp->tcb_tail) {
				if ((tcb->user_id & user) && ((tcb->attribute & attr) == 0))
					amTaskSleep(tcb);
				tcb		= tcb->next;
			}
			break;

		case	AMD_TASK_SLPGRP_NOTEXCLUSIVE:	// 全部満たさない
			while (tcb != &taskp->tcb_tail) {
				if ((tcb->user_id & user) && ((tcb->attribute & attr) != attr))
					amTaskSleep(tcb);
				tcb		= tcb->next;
			}
			break;

		default:
			// そんなフラグはないよ
			amAssert(0);
			break;
	}

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&taskp->mutex);
#endif
}

/************************************************************************/
/* void amTaskSleepPriority(AMS_TASK *taskp,                            */
/*                   Uint32 prio_begin, Uint32 prio_end, Uint32 user)   */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    taskp      : タスクシステム構造体                                 */
/*    prio_begin : 停止開始優先(以上)                                   */
/*    prio_end   : 停止開始優先(以下)                                   */
/*    user       : 停止対象ユーザ                                       */
/*                 AMD_TASK_UID_ALL - 全ユーザ                          */
/*                 0 (default)      - 全タスク(ユーザ指定なしも含む)    */
/* [FUNCTION]                                                           */
/*    タスクの優先範囲停止                                              */
/************************************************************************/
void amTaskSleepPriority(AMS_TASK *taskp, Uint32 prio_begin, Uint32 prio_end, Uint32 user)
{
	amAssert(taskp);

	// 変な値をチェック
	amAssert(prio_begin <= 0xFFFF);
	amAssert(prio_end <= 0xFFFF);
	amAssert(prio_begin <= prio_end);

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&taskp->mutex);
#endif

	AMS_TCB		*tcb;
	tcb		= taskp->tcb_head.next;

	// serach
	while ((tcb != &taskp->tcb_tail) && (tcb->priority < prio_begin))
		tcb		= tcb->next;

	// sleep
	while ((tcb != &taskp->tcb_tail) && (tcb->priority <= prio_end)) {
		if ((user == 0) || (tcb->user_id & user))
			amTaskSleep(tcb);
		tcb		= tcb->next;
	}

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&taskp->mutex);
#endif
}

/************************************************************************/
/* void amTaskWakeup(AMS_TCB *tcb)                                      */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    tcb : 再開するタスクの TCB へのポインタ                           */
/* [FUNCTION]                                                           */
/*    タスク再開                                                        */
/************************************************************************/
void amTaskWakeup(AMS_TCB *tcb)
{
	// なんじゃそら
	amAssert(tcb /* amWakeupTask */);

#if _PC
	if ((Sint32)tcb->procedure < AMD_TASK_STATE_BARRIER)
		*(Sint32 *)&tcb->procedure	= -(Sint32)tcb->procedure;
#else
	*(Sint32 *)&tcb->procedure	&= ~AMD_TASK_STATE_SLEEP_MASK;
#endif
}

/************************************************************************/
/* void amTaskWakeupGroup(AMS_TASK *taskp,                              */
/*                              Uint32 user, Uint32 attr, Uint32 flag)  */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    taskp : タスクシステム構造体                                      */
/*    user  : 再開対象ユーザ (AMD_TASK_UID_ALL または 0 : 全ユーザ)     */
/*    attr  : 再開対象属性フラグ                                        */
/*    flag  : attr の評価方法                                           */
/*           AMD_TASK_SLPGRP_INCLUSIVE    : 一部条件                    */
/*           AMD_TASK_SLPGRP_EXCLUSIVE    : 全条件                      */
/*           AMD_TASK_SLPGRP_NOTINCLUSIVE : 一部条件否定                */
/*           AMD_TASK_SLPGRP_NOTEXCLUSIVE : 全条件否定                  */
/* [FUNCTION]                                                           */
/*    タスクのグループ再開                                              */
/************************************************************************/
void amTaskWakeupGroup(AMS_TASK *taskp, Uint32 user, Uint32 attr, Uint32 flag)
{
	AMS_TCB	*tcb;

	amAssert(taskp);

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&taskp->mutex);
#endif

	if (user == 0)
		user	= (Uint32)AMD_TASK_UID_ALL;
	tcb		= taskp->tcb_head.next;

	switch (flag) {
		case	AMD_TASK_SLPGRP_INCLUSIVE:		// ひとつでも満たす
			while (tcb != &taskp->tcb_tail) {
				if ((tcb->user_id & user) && ((tcb->attribute & attr) != 0))
					amTaskWakeup(tcb);
				tcb		= tcb->next;
			}
			break;

		case	AMD_TASK_SLPGRP_EXCLUSIVE:		// 全部満たす
			while (tcb != &taskp->tcb_tail) {
				if ((tcb->user_id & user) && ((tcb->attribute & attr) == attr))
					amTaskWakeup(tcb);
				tcb		= tcb->next;
			}
			break;

		case	AMD_TASK_SLPGRP_NOTINCLUSIVE:	// ひとつも満たさない
			while (tcb != &taskp->tcb_tail) {
				if ((tcb->user_id & user) && ((tcb->attribute & attr) == 0))
					amTaskWakeup(tcb);
				tcb		= tcb->next;
			}
			break;

		case	AMD_TASK_SLPGRP_NOTEXCLUSIVE:	// 全部満たさない
			while (tcb != &taskp->tcb_tail) {
			if ((tcb->user_id & user) && ((tcb->attribute & attr) != attr))
					amTaskWakeup(tcb);
				tcb		= tcb->next;
			}
			break;

		default:
			// そんなフラグはないよ
			amAssert(0);
			break;
	}

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&taskp->mutex);
#endif
}

/************************************************************************/
/* void amTaskWakeupPriority(AMS_TASK *taskp,                           */
/*                   Uint32 prio_begin, Uint32 prio_end, Uint32 user)   */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    taskp      : タスクシステム構造体                                 */
/*    prio_begin : 再開開始優先(以上)                                   */
/*    prio_end   : 再開開始優先(以下)                                   */
/*    user       : 再開対象ユーザ                                       */
/*                 AMD_TASK_UID_ALL - 全ユーザ                          */
/*                 0 (default)      - 全タスク(ユーザ指定なしも含む)    */
/* [FUNCTION]                                                           */
/*    タスクの優先範囲再開                                              */
/************************************************************************/
void amTaskWakeupPriority(AMS_TASK *taskp, Uint32 prio_begin, Uint32 prio_end, Uint32 user)
{
	amAssert(taskp);

	// 変な値をチェック
	amAssert(prio_begin <= 0xFFFF);
	amAssert(prio_end <= 0xFFFF);
	amAssert(prio_begin <= prio_end);

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&taskp->mutex);
#endif

	AMS_TCB	*tcb;
	tcb		= taskp->tcb_head.next;

	// serach
	while ((tcb != &taskp->tcb_tail) && (tcb->priority < prio_begin))
		tcb		= tcb->next;

	// sleep
	while ((tcb != &taskp->tcb_tail) && (tcb->priority <= prio_end)) {
		if ((user == 0) || (tcb->user_id & user))
			amTaskWakeup(tcb);
		tcb		= tcb->next;
	}

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&taskp->mutex);
#endif
}

/************************************************************************/
/* void amTaskSetOwnerName(AMS_TASKLIST_OWNER *pList, Sint32 listSize)  */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*     pList : オーナー名リストへのポインタ                             */
/*  listSize : リストの要素数                                           */
/* [FUNCTION] タスクリストのオーナー名の設定                            */
/************************************************************************/
void amTaskSetOwnerName(const AMS_TASKLIST_OWNER *pList, Uint32 listSize)
{
	_am_owner_list = pList;
	_am_szOwnerList = (_am_owner_list) ? listSize : 0;
}

/************************************************************************/
/* void amTaskDisplayList(AMS_TASK *taskp, int locx, int locy)          */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    locx, locy : 表示位置                                             */
/* [FUNCTION] タスクリスト表示                                          */
/************************************************************************/
void amTaskDisplayList(AMS_TASK *taskp, int locx, int locy)
{
	UNREFERENCED_PARAMETER(locx);
	UNREFERENCED_PARAMETER(locy);
	UNREFERENCED_PARAMETER(taskp);

#if AMD_DEBUG & AMD_USE_KEYBD & 0
#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&taskp->mutex);
#endif

    if (locx < 0) locx = 0;
    if (locy < 0) locy = 0;

    int tline, dline;
    int base_locy = locy;
    AMS_TCB *target = NULL;
    AMS_TCB *tcb    = _am_tcb_head.next;
    AMS_TCB_FOOTER *footer;
    
    // header
    amPrintf(locx, locy, "[TASK LIST (%3d/%d)]",
             _am_tcb_num, AMD_TASK_BUFFER_SIZE);

    // no task
    if (_am_tcb_num <= 0) return;
    
    // 選択カーソル処理

    if (amGetKeybdRepeat(KBD_UP)) {
        _am_tlist_cline--;
        if (_am_tlist_cline < 0) _am_tlist_cline = 0;

        if (_am_tlist_cline < _am_tlist_dline)
            _am_tlist_dline = _am_tlist_cline;
    }

    if (amGetKeybdRepeat(KBD_DOWN)) {
        _am_tlist_cline++;

        if ((_am_tlist_cline - _am_tlist_dline) >
            (AMD_TASK_LIST_LENGTH - 1))
            _am_tlist_dline =
                _am_tlist_cline - (AMD_TASK_LIST_LENGTH - 1);
    }
    if ((_am_tcb_num - _am_tlist_dline) < AMD_TASK_LIST_LENGTH)
        _am_tlist_dline = _am_tcb_num - AMD_TASK_LIST_LENGTH;
    
    // リスト表示
    tline = 0;
    dline = 0;
    while ((tcb != &_am_tcb_tail) && dline < AMD_TASK_LIST_LENGTH) {
        if (tline >= _am_tlist_dline) {
            locy++;

#if _PC
			if ((Sint32)tcb->procedure < AMD_TASK_STATE_KILL)
				amPrintColor(0x80804040);
#else
			if ((Sint32)tcb->procedure & AMD_TASK_STATE_SLEEP_MASK)
				amPrintColor(0x80804040);
#endif

            // put line
            amPrintf(locx + 1, locy, "%04X %s",
                     tcb->priority, tcb->name);
            footer = amTaskGetTcbFooter(tcb);
            amPrintf(locx + 24, locy, "(%5d/%5d)",
                     footer->cpu_cnt, footer->cpu_cnt_max);

            // cursor
            if (tline == _am_tlist_cline) {
                target = tcb;
                amPrint(locx, locy, "\25");
            }
            dline++;

			amPrintColor(0x80808080);
        }
        
        tcb = tcb->next;
        tline++;
    }

    if (target == NULL) {
        target = tcb->prev;
        amPrint(locx, locy, "\25");
        _am_tlist_cline = tline - 1;
    }
    
    //--- description
    
    char namebuf[32], sysattr[5], usrattr[9];

    // user display
    sprintf(namebuf, "%08X", target->user_id);
    for (int i = 0; i < _am_szOwnerList; i++) {
        if (target->user_id == _am_owner_list[i].uflag)
            strcpy(namebuf, _am_owner_list[i].name);
    }

    // attribute display
    register Uint32 attr = target->attribute;
    sysattr[0] = (attr & AMD_TASK_ATTR_SYSTEM) ? 'S' : '-';
    sysattr[1] = (attr & AMD_TASK_ATTR_MAIN)   ? 'M' : '-';
    sysattr[2] = (attr & AMD_TASK_ATTR_DRAW3D) ? '3' : '-';
    sysattr[3] = (attr & AMD_TASK_ATTR_DRAW2D) ? '2' : '-';
    sysattr[4] = '\0';

    usrattr[0] = (attr & AMD_TASK_ATTR_USER_0) ? '0' : '-';
    usrattr[1] = (attr & AMD_TASK_ATTR_USER_1) ? '1' : '-';
    usrattr[2] = (attr & AMD_TASK_ATTR_USER_2) ? '2' : '-';
    usrattr[3] = (attr & AMD_TASK_ATTR_USER_3) ? '3' : '-';
    usrattr[4] = (attr & AMD_TASK_ATTR_USER_4) ? '4' : '-';
    usrattr[5] = (attr & AMD_TASK_ATTR_USER_5) ? '5' : '-';
    usrattr[6] = (attr & AMD_TASK_ATTR_USER_6) ? '6' : '-';
    usrattr[7] = (attr & AMD_TASK_ATTR_USER_7) ? '7' : '-';
    usrattr[8] = '\0';

    locy = base_locy + AMD_TASK_LIST_LENGTH + 2;
    amPrint (locx, locy++, "[DESCRIPTION]");
    amPrintf(locx + 1, locy++, "ADDR    :%08X", (Uint32)target);
    amPrintf(locx + 1, locy++, "OWNER   :%s", namebuf);
    amPrintf(locx + 1, locy++, "SYS_ATTR:%s", sysattr);
    amPrintf(locx + 1, locy++, "USR_ATTR:%s", usrattr);

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&taskp->mutex);
#endif
#endif // AMD_DEBUG
}


/************************************************************************/
/* void amTaskDisplayThread(AMS_TASK *taskp)                            */
/*----------------------------------------------------------------------*/
/* [FUNCTION] タスクスレッド表示                                        */
/************************************************************************/
void amTaskDisplayThread(AMS_TASK *taskp)
{
#if AMD_TASK_DEBUG
	AMS_TCB_THREAD	*thread;
	AMS_TCB_REPORT	*report;

	if (taskp->tcb_thread_num == 1)
		return;

#if 0
	// 単純な文字による表示
	Sint32		i, pos_x, pos_y = 10;

	thread	= &taskp->tcb_thread[0];
	for (i = 0; i < taskp->tcb_thread_num; i++, thread++, pos_y++) {
		report	= thread->report_head.next;
		amPrintf(2, pos_y, "%d :", i);
		pos_x	= 6;
		for (; report != NULL; report = report->next) {
			amPrintf(pos_x, pos_y, report->name);
			pos_x++;
		}
	}
#else
	// メーター表示
	AMS_TASK_THREAD_MAP_HEADER	*mheader;
	AMS_TASK_THREAD_MAP_THREAD	*mthread;
	AMS_TASK_THREAD_MAP_TCB		*mtcb;
	Sint32			i, n;
	static float	x0 = 0.0f;
	static float	x1 = 1000.0f;
	static float	y0 = 200.0f;
	static float	dy = 4.0f;
	static float	ny = 8.0f;

	mheader	= (AMS_TASK_THREAD_MAP_HEADER *)amDrawGetDataBuffer();
	mheader->thread_num		= taskp->tcb_thread_num;
	mheader->x0		= x0;
	mheader->x1		= (x1 - x0) / amSystemGetFrameRateMain() + x0;
	mheader->y0		= y0;
	mheader->dy		= dy;
	mheader->ny		= ny;

	thread	= &taskp->tcb_thread[0];
	mthread	= (AMS_TASK_THREAD_MAP_THREAD *)(mheader + 1);
	for (i = 0; i < taskp->tcb_thread_num; i++, thread++) {
		mtcb	= (AMS_TASK_THREAD_MAP_TCB *)(mthread + 1);
		report	= thread->report_head.next;
		for (n = 0; report != NULL; report = report->next, mtcb++, n++) {
			mtcb->frame_start	= report->frame_start;
			mtcb->frame_end		= report->frame_end;
		}
		mthread->thread_tcb_num		= n;
		mthread	= (AMS_TASK_THREAD_MAP_THREAD *)mtcb;
	}
	amDrawSetDataBuffer((char *)mthread);

	amDrawRegistCommand(
			AMD_COMMAND_STATE_DEBUG,
			AMD_COMMAND_DEBUG_THREAD,
			mheader);
#endif

#else
	UNREFERENCED_PARAMETER(taskp);
#endif
}


/*----- Local functions ------------------------------------------------*/

/************************************************************************/
/* void _amTaskExecuteInOrder(AMS_TASK *taskp)                          */
/*----------------------------------------------------------------------*/
/* [INPUT]  taskp : 実行するタスクシステム                              */
/* [FUNCTION] タスク実行                                                */
/************************************************************************/
void _amTaskExecuteInOrder(AMS_TASK *taskp)
{
	AMS_TCB			*tcb;
#if AMD_DEBUG
	AMS_TIMER		timer;
	AMS_TCB_FOOTER	*footer;
	amTimerCreate(&timer);
#endif

	// タスク実行ループ
	tcb		= taskp->tcb_head.next;
	while (tcb != &taskp->tcb_tail) {
#if AMD_DEBUG
		// 計測開始
		amTimerStart(&timer);
#endif

#if _PC | _IPHONE
		if ((Sint32)tcb->procedure > 0) { // ちと怖い
#else
		if (!((Sint32)tcb->procedure & AMD_TASK_STATE_SLEEP_MASK)) { // ちと怖い
#endif
			tcb->procedure(tcb);
		}

#if AMD_DEBUG
		// 計測
		amTimerEnd(&timer);
		footer	= amTaskGetTcbFooter(tcb);
		footer->cpu_cnt		= (Uint32)amTimerGetuSec(&timer);
		if (footer->cpu_cnt_max < footer->cpu_cnt)
			footer->cpu_cnt_max	= footer->cpu_cnt;
#endif

		// next
		tcb		= tcb->next;
	}

	// タスク削除ループ
	tcb		= taskp->tcb_head.next;
	while (tcb != &taskp->tcb_tail) {
		if (tcb->procedure == (TaskProc)AMD_TASK_STATE_KILL)
			_amTaskDeleteReal(tcb);
		tcb		= tcb->next;
	}

#if AMD_DEBUG
	amTimerDelete(&timer);
	if (taskp == _am_default_taskp) {
#if AMD_USE_KEYBD
		if (amGetKeybdModStat(KBD_MOD_LCTRL | KBD_MOD_RCTRL) &&
			amGetKeybdStand(KBD_T))
			_am_display_tasklist	= (_am_display_tasklist) ? 0 : 1;
#endif // AMD_USE_KEYBD
		_amTaskCheckWork(taskp);
		if (_am_display_tasklist)
			amTaskDisplayList(taskp, 1, 2);
	}
#endif // AMD_DEBUG
}


#if AMD_TASK_THREAD_NUM > 1
/************************************************************************/
/* DWORD WINAPI _amTaskExec(DWORD arg)                                  */
/* void _amTaskExec(uint64_t arg)                                       */
/*----------------------------------------------------------------------*/
/* [FUNCTION]  実行スレッド                                             */
/************************************************************************/
#if _PC
DWORD WINAPI _amTaskExec(DWORD arg)
#elif _XBOX
DWORD WINAPI _amTaskExec(DWORD arg)
#elif _PS3
void _amTaskExec(uint64_t arg)
#endif
{
	AMS_TCB_THREAD	*thread = (AMS_TCB_THREAD *)(((AMS_THREAD *)arg)->arg);
	AMS_TASK	*taskp = thread->taskp;
	AMS_TCB		*tcbp;
#if AMD_DEBUG
	AMS_TIMER	timer;
	AMS_TCB_FOOTER	*footer;
	amTimerCreate(&timer);
#endif

	amThreadOpen(&thread->thread);

	for (;;) {
		// 終了要求チェック
		if (amThreadCheckExit(&thread->thread))
			break;

		amMutexLock(&taskp->mutex);

		// 実行可能なTCBの検索
		tcbp	= _amTaskGetEnableTcb(thread->taskp, thread->id);
		if (tcbp == NULL) {
			amMutexUnlock(&taskp->mutex);
			amAlarmWait(&thread->alarm_wakeup);
			continue;
		}

#if _PC
		if ((Sint32)tcb->procedure > 0) { // ちと怖い
#else
		if (!((Sint32)tcbp->procedure & AMD_TASK_STATE_SLEEP_MASK)) {	// ちと怖い
#endif
			void (*procedure)(AMS_TCB *);

			// TCBの実行準備
			procedure	= tcbp->procedure;
			tcbp->thread		= thread;
			thread->tcbp		= tcbp;
			taskp->tcb_stall_count[tcbp->group_id]++;
			taskp->tcb_stall	|= 1 << tcbp->group_id;
#if AMD_DEBUG
			// 計測開始
			amTimerStart(&timer);
#endif
			amMutexUnlock(&taskp->mutex);

			// TCBの実行
			procedure(tcbp);

			// TCBの実行完了
			amMutexLock(&taskp->mutex);
#if AMD_DEBUG
			// 計測
			amTimerEnd(&timer, 0);
			footer	= amTaskGetTcbFooter(tcbp);
			footer->cpu_cnt		= (Uint32)amTimerGetuSec(&timer);
			if (footer->cpu_cnt_max < footer->cpu_cnt)
				footer->cpu_cnt_max	= footer->cpu_cnt;

#if AMD_TASK_DEBUG
{
			AMS_TCB_REPORT	*report;
			report		= &taskp->tcb_report[taskp->tcb_report_num++];
			memcpy(report->name, tcbp->name, AMD_TASK_NAME_LEN);
			report->frame_start	= amTimerCalcFrame(
					taskp->tcb_start_count, timer.count_start, &timer);
			report->frame_end	= amTimerCalcFrame(
					taskp->tcb_start_count, timer.count_end, &timer);
			report->next		= NULL;
			thread->report_tail->next	= report;
			thread->report_tail			= report;
}
#endif
#endif
			thread->tcbp		= NULL;
			if ((--taskp->tcb_stall_count[tcbp->group_id]) == 0)
				taskp->tcb_stall	&= ~(1 << tcbp->group_id);
		}
		tcbp->thread		= (AMS_TCB_THREAD *)AMD_TASK_TCB_STAT_END;
		if (tcbp == taskp->tcb_top) {
			do {
				tcbp		= tcbp->next;
			} while (tcbp->thread == (AMS_TCB_THREAD *)AMD_TASK_TCB_STAT_END);
			taskp->tcb_top	= tcbp;
		}

		_amTaskWakeupThread(taskp);

		// 完了チェック
		if ((taskp->tcb_top == &taskp->tcb_tail) && (taskp->tcb_stall == 0)) {
			taskp->tcb_top		= NULL;
			taskp->tcb_stall	= (Uint32)AMD_TASK_GROUP_ALL;
			amAlarmSet(&taskp->alarm_end);
		}

		amMutexUnlock(&taskp->mutex);
	}

#if AMD_DEBUG
	amTimerDelete(&timer);
#endif

	amThreadQuit(&thread->thread);

#if _PC
	return	0;
#elif _XBOX
	return	0;
#elif _PS3
	return;
#endif
}


/************************************************************************/
/* AMS_TCB *_amTaskGetEnableTcb(AMS_TASK *taskp, Sint32 thread_id)      */
/*----------------------------------------------------------------------*/
/* [FUNCTION]  実行可能なTCBの検索                                      */
/************************************************************************/
AMS_TCB *_amTaskGetEnableTcb(AMS_TASK *taskp, Sint32 thread_id)
{
	AMS_TCB		*tcbp;
	Uint32		mask;

	tcbp	= taskp->tcb_top;

	if ((tcbp == NULL) || (taskp->tcb_stall == AMD_TASK_GROUP_ALL))
		return	NULL;

	mask	= 1 << thread_id;

	for (; tcbp != &taskp->tcb_tail; tcbp = tcbp->next) {
		// 割り当て済み
		if (tcbp->thread != (AMS_TCB_THREAD *)AMD_TASK_TCB_STAT_WAIT)
			continue;

		// 実行許可待ち
		if (tcbp->thread == (AMS_TCB_THREAD *)AMD_TASK_TCB_STAT_PENDING)
			continue;

		// ストール
		if (tcbp->stall_group & taskp->tcb_stall) {
			// バリア
			if ((Sint32)tcbp->procedure == AMD_TASK_STATE_BARRIER)
				return	NULL;
			continue;
		}

		// 実行可能スレッド
		if (!(tcbp->run_thread & mask))
			continue;

		return	tcbp;
	}

	return	NULL;
}


/************************************************************************/
/* void _amTaskWakeupThread(AMS_TASK *taskp)                            */
/*----------------------------------------------------------------------*/
/* [FUNCTION]  TCB実行スレッドの起床                                    */
/************************************************************************/
void _amTaskWakeupThread(AMS_TASK *taskp)
{
	Sint32		i;

	for (i = 0; i < taskp->tcb_thread_num; i++) {
		amAlarmClear(&taskp->tcb_thread[i].alarm_wakeup);
		amAlarmSet(&taskp->tcb_thread[i].alarm_wakeup);
	}
}
#endif


/************************************************************************/
/* void _amTaskDeleteReal(AMS_TCB *tcb)                                 */
/*----------------------------------------------------------------------*/
/* [FUNCTION] タスク消去（実際に実行）                                  */
/************************************************************************/
void _amTaskDeleteReal(AMS_TCB *tcb)
{
	AMS_TASK	*taskp;

	taskp	= tcb->taskp;

	tcb->prev->next	= tcb->next;
	tcb->next->prev	= tcb->prev;

	// return to the pool
	taskp->tcb_bufp[taskp->tcb_free] = tcb;
	taskp->tcb_free	= (taskp->tcb_free + 1) & (taskp->tcb_buffer_size - 1);

	taskp->tcb_num--;

	// 消しすぎ
	amAssert(taskp->tcb_num >= 0);
}


/************************************************************************/
/* void _amTaskCheckWork(AMS_TASK *taskp)                               */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ワーク破壊検査                                            */
/************************************************************************/
void _amTaskCheckWork(AMS_TASK *taskp)
{
	AMS_TCB *tcb;

	tcb		= taskp->tcb_buffer;
	for (int i = 0; i < taskp->tcb_buffer_size; i++) {
		// ワークが壊されてるよ
		amAssert(tcb->wkbegin == AMD_TASK_WORK_CODE);
		amAssert(amTaskGetTcbFooter(tcb)->wkend == AMD_TASK_WORK_CODE);
		tcb		= amTaskNextTcb(tcb);
	}
}
