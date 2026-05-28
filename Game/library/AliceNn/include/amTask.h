/************************************************************************/
/*      amTask.h                                                        */
/*            Copyright(c) 2000-2009 Dimps CORP. All Rights Reserved.   */
/*----------------------------------------------------------------------*/
/* タスクライブラリヘッダ                                               */
/*----------------------------------------------------------------------*/
/************************************************************************/

#ifndef _AM_TASK_H_
#define _AM_TASK_H_

/*----- Include files --------------------------------------------------*/

#include "amThread.h"


/*----- Const values ---------------------------------------------------*/

// 定数関連
//typedef enum _ame_task_
enum
{
    // 仕様定義
    AMD_TASK_NAME_LEN    = 16,         // TCB 名の最大長
    AMD_TASK_DEFAULT_WORK_SIZE  = (128 - 64), // TCB ワークサイズ
    AMD_TASK_DEFAULT_BUFFER_SIZE = 0xFF + 1,   // 最大TCB 数 (bit-fill + 1)

    // システム変数
    AMD_TASK_STATE_NULL  =  0,			// Null state
    AMD_TASK_STATE_KILL  = -1,			// Kill state
    AMD_TASK_STATE_BARRIER	= -3,		// バリア
	AMD_TASK_STATE_SLEEP_MASK	= 1,	// Sleep state mask

    // グループ消去属性条件評価フラグ
    AMD_TASK_DELGRP_INCLUSIVE    = 0, // ひとつでも満たせば対象
    AMD_TASK_DELGRP_EXCLUSIVE    = 1, // すべて満たせば対象
    AMD_TASK_DELGRP_NOTINCLUSIVE = 2, // ひとつでも満たせば対象外
    AMD_TASK_DELGRP_NOTEXCLUSIVE = 3, // すべて満たせば対象外

    // グループ停止属性条件評価フラグ
    AMD_TASK_SLPGRP_INCLUSIVE    = 0, // ひとつでも満たせば対象
    AMD_TASK_SLPGRP_EXCLUSIVE    = 1, // すべて満たせば対象
    AMD_TASK_SLPGRP_NOTINCLUSIVE = 2, // ひとつでも満たせば対象外
    AMD_TASK_SLPGRP_NOTEXCLUSIVE = 3, // すべて満たせば対象外

    // User ID
    AMD_TASK_UID_NONE = (0),          // グループ消去対象外
    AMD_TASK_UID_00   = (0x1 <<  0),  // ユーザ 1-16
    AMD_TASK_UID_01   = (0x1 <<  1),
    AMD_TASK_UID_02   = (0x1 <<  2),
    AMD_TASK_UID_03   = (0x1 <<  3),
    AMD_TASK_UID_04   = (0x1 <<  4),
    AMD_TASK_UID_05   = (0x1 <<  5),
    AMD_TASK_UID_06   = (0x1 <<  6),
    AMD_TASK_UID_07   = (0x1 <<  7),
    AMD_TASK_UID_08   = (0x1 <<  8),
    AMD_TASK_UID_09   = (0x1 <<  9),
    AMD_TASK_UID_0A   = (0x1 << 10),
    AMD_TASK_UID_0B   = (0x1 << 11),
    AMD_TASK_UID_0C   = (0x1 << 12),
    AMD_TASK_UID_0D   = (0x1 << 13),
    AMD_TASK_UID_0E   = (0x1 << 14),
    AMD_TASK_UID_0F   = (0x1 << 15),
    AMD_TASK_UID_ALL  = (0xFFFFFFFF),  // 全ユーザ
    
    // Attribute
    AMD_TASK_ATTR_SYSTEM = (0x1 << 0), // システム
    AMD_TASK_ATTR_MAIN   = (0x1 << 1), // 通常の処理
    AMD_TASK_ATTR_DRAW3D = (0x1 << 2), // 3D 描画命令発行
    AMD_TASK_ATTR_DRAW2D = (0x1 << 3), // 2D 描画命令発行

    // User attribute
    AMD_TASK_ATTR_USER_0 = (0x1 << 16),
    AMD_TASK_ATTR_USER_1 = (0x1 << 17),
    AMD_TASK_ATTR_USER_2 = (0x1 << 18),
    AMD_TASK_ATTR_USER_3 = (0x1 << 19),
    AMD_TASK_ATTR_USER_4 = (0x1 << 20),
    AMD_TASK_ATTR_USER_5 = (0x1 << 21),
    AMD_TASK_ATTR_USER_6 = (0x1 << 22),
    AMD_TASK_ATTR_USER_7 = (0x1 << 23),

	// グループID
	AMD_TASK_GID_00		=  0,
	AMD_TASK_GID_01		=  1,
	AMD_TASK_GID_02		=  2,
	AMD_TASK_GID_03		=  3,
	AMD_TASK_GID_04		=  4,
	AMD_TASK_GID_05		=  5,
	AMD_TASK_GID_06		=  6,
	AMD_TASK_GID_07		=  7,
	AMD_TASK_GID_08		=  8,
	AMD_TASK_GID_09		=  9,
	AMD_TASK_GID_10		= 10,
	AMD_TASK_GID_11		= 11,
	AMD_TASK_GID_12		= 12,
	AMD_TASK_GID_13		= 13,
	AMD_TASK_GID_14		= 14,
	AMD_TASK_GID_15		= 15,
	AMD_TASK_GID_16		= 16,
	AMD_TASK_GID_17		= 17,
	AMD_TASK_GID_18		= 18,
	AMD_TASK_GID_19		= 19,
	AMD_TASK_GID_20		= 20,
	AMD_TASK_GID_21		= 21,
	AMD_TASK_GID_22		= 22,
	AMD_TASK_GID_23		= 23,
	AMD_TASK_GID_24		= 24,
	AMD_TASK_GID_25		= 25,
	AMD_TASK_GID_26		= 26,
	AMD_TASK_GID_27		= 27,
	AMD_TASK_GID_28		= 28,
	AMD_TASK_GID_29		= 29,
	AMD_TASK_GID_30		= 30,
	AMD_TASK_GID_31		= 31,

	// グループマスク
	AMD_TASK_GROUP_00	= (0x1 <<  0),
	AMD_TASK_GROUP_01	= (0x1 <<  1),
	AMD_TASK_GROUP_02	= (0x1 <<  2),
	AMD_TASK_GROUP_03	= (0x1 <<  3),
	AMD_TASK_GROUP_04	= (0x1 <<  4),
	AMD_TASK_GROUP_05	= (0x1 <<  5),
	AMD_TASK_GROUP_06	= (0x1 <<  6),
	AMD_TASK_GROUP_07	= (0x1 <<  7),
	AMD_TASK_GROUP_08	= (0x1 <<  8),
	AMD_TASK_GROUP_09	= (0x1 <<  9),
	AMD_TASK_GROUP_10	= (0x1 << 10),
	AMD_TASK_GROUP_11	= (0x1 << 11),
	AMD_TASK_GROUP_12	= (0x1 << 12),
	AMD_TASK_GROUP_13	= (0x1 << 13),
	AMD_TASK_GROUP_14	= (0x1 << 14),
	AMD_TASK_GROUP_15	= (0x1 << 15),
	AMD_TASK_GROUP_16	= (0x1 << 16),
	AMD_TASK_GROUP_17	= (0x1 << 17),
	AMD_TASK_GROUP_18	= (0x1 << 18),
	AMD_TASK_GROUP_19	= (0x1 << 19),
	AMD_TASK_GROUP_20	= (0x1 << 20),
	AMD_TASK_GROUP_21	= (0x1 << 21),
	AMD_TASK_GROUP_22	= (0x1 << 22),
	AMD_TASK_GROUP_23	= (0x1 << 23),
	AMD_TASK_GROUP_24	= (0x1 << 24),
	AMD_TASK_GROUP_25	= (0x1 << 25),
	AMD_TASK_GROUP_26	= (0x1 << 26),
	AMD_TASK_GROUP_27	= (0x1 << 27),
	AMD_TASK_GROUP_28	= (0x1 << 28),
	AMD_TASK_GROUP_29	= (0x1 << 29),
	AMD_TASK_GROUP_30	= (0x1 << 30),
	AMD_TASK_GROUP_31	= (0x1 << 31),
	AMD_TASK_GROUP_ALL	= (0xffffffff),

	AMD_TASK_TCB_STAT_WAIT		= 0,	// 実行待機中
	AMD_TASK_TCB_STAT_PENDING	= -1,	// 実行許可待ち
	AMD_TASK_TCB_STAT_END		= -2,	// 実行済み

	AMD_TASK_TCB_STAT_RUNNING	= -3,	// 実行中

    // work checker
    AMD_TASK_WORK_CODE   = 0x0D020A0C,
//} AME_TASK;
};

// TCBの実行状態

#define AMD_TASK_WORK_SIZE		(_am_tcb_work_size)
#define AMD_TASK_BUFFER_SIZE	(_am_tcb_buffer_size)


/*----- Type definitions -----------------------------------------------*/

// task
typedef struct _ams_tcb_footer_	AMS_TCB_FOOTER;
typedef struct _ams_tcb_thread_	AMS_TCB_THREAD;
typedef struct _ams_tcb_report_	AMS_TCB_REPORT;
typedef struct _ams_tcb_		AMS_TCB;
typedef struct _ams_task_		AMS_TASK;

// Task procedure
typedef void (*TaskProc)(AMS_TCB *);

// tcb (64/80bytes)
struct _ams_tcb_ {
    char    name[AMD_TASK_NAME_LEN];
    // --- 16byte
    
	Uint32	user_id;
	Uint32	attribute;
    
	Uint32	priority;

	TaskProc	procedure;
	TaskProc	destructor;

	struct _ams_tcb_	*prev;
	struct _ams_tcb_	*next;

	AMS_TASK	*taskp;
	AMS_TCB_FOOTER	*footer;

#if AMD_TASK_THREAD_NUM > 1
	Uint32	stall_group;		// 実行待ちグループマスク
	Uint32	group_id;			// グループID
	Sint32	run_thread;			// 実行可能スレッドマスク
	AMS_TCB_THREAD	*thread;	// 実行スレッド
#endif

    Sint32  wkbegin;
    //  --- 56/72byte

    Uint8   work[8];
};

struct _ams_tcb_footer_ {
    Sint32  wkend;

	Sint32	reserved[1];

    Uint32  cpu_cnt;			// マイクロ秒単位
    Uint32  cpu_cnt_max;
};

typedef struct _ams_tasklist_owner_ {
    Uint32  uflag;
    char    name[12];
} AMS_TASKLIST_OWNER;


// TCB Report
struct _ams_tcb_report_ {
	char	name[AMD_TASK_NAME_LEN];	// 実行TCB名
	float	frame_start;		// 開始時間(1/60秒)
	float	frame_end;			// 終了時間(1/60秒)
	AMS_TCB_REPORT	*next;		// 次の実行TCB
	Sint32	reserved[1];
};

// TCB Thread
struct _ams_tcb_thread_ {
	Sint32			id;				// TCBスレッドID
	AMS_TASK		*taskp;			// 所属タスク
	AMS_TCB			*tcbp;			// 実行TCB
	AMS_THREAD		thread;			// スレッド
	AMS_THREAD_ID	thread_id;		// スレッドID
	AMS_ALARM		alarm_wakeup;	// 起床アラーム
	NNS_MATRIXSTACK	matrix_stack;	// マトリクススタック
	void			*matrix_stack_buf;

#if AMD_DEBUG && (AMD_TASK_THREAD_NUM > 1)
	AMS_TCB_REPORT	report_head;
	AMS_TCB_REPORT	*report_tail;
#endif
};

// Task
struct _ams_task_ {
	Sint32	tcb_max;	// TCB 最大数
	Sint32	tcb_num;	// TCB 数
	Uint32	tcb_alloc;	// TCB 確保位置
	Uint32	tcb_free;	// TCB 開放位置
	AMS_TCB	tcb_head;	// TCB 先頭
	AMS_TCB	tcb_tail;	// TCB 末尾

	Sint32	tcb_work_size;
	Sint32	tcb_buffer_size;
	AMS_TCB	*tcb_buffer;
	AMS_TCB	**tcb_bufp;

#if AMD_TASK_THREAD_NUM > 1
	AMS_MUTEX	mutex;
	Sint32	tcb_thread_num;	// TCB スレッド数
	AMS_TCB	*tcb_top;	// TCB 検索先頭
	Uint32	tcb_stall;	// TCB ストールグループ
	Uint8	tcb_stall_count[32];	// TCB ストールカウンタ
	AMS_ALARM	alarm_end;	// 完了アラーム
	AMS_TCB_THREAD	tcb_thread[AMD_TASK_THREAD_NUM];
#if AMD_DEBUG
	Uint64	tcb_start_count;
	Sint32	tcb_report_num;
	AMS_TCB_REPORT	*tcb_report;
#endif
#endif
};


// スレッドメーター表示パラメータ
typedef struct {
	Sint32	thread_num;		// スレッド数
	float	x0, x1;			// 横位置
	float	y0, dy, ny;		// 縦位置、縦幅、縦間隔
} AMS_TASK_THREAD_MAP_HEADER;

typedef struct {
	Sint32	thread_tcb_num;	// 実行TCB数
} AMS_TASK_THREAD_MAP_THREAD;

typedef struct {
	float	frame_start;	// 開始時間(1/60秒)
	float	frame_end;		// 終了時間(1/60秒)
} AMS_TASK_THREAD_MAP_TCB;


/*----- External variables ---------------------------------------------*/

extern AMS_TASK	*_am_default_taskp;


/*----- External functions ---------------------------------------------*/

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
AMS_TASK *amTaskInitSystem(
		Sint32 max_tcb = AMD_TASK_DEFAULT_BUFFER_SIZE,
		Sint32 work_size = AMD_TASK_DEFAULT_WORK_SIZE,
		Sint32 thread_num = AMD_TASK_THREAD_NUM);

/************************************************************************/
/* void amTaskExitSystem(AMS_TASK *taskp)                               */
/*----------------------------------------------------------------------*/
/* [INPUT]  taskp : タスクシステム構造体のポインタ                      */
/* [FUNCTION] タスクシステム終了                                        */
/************************************************************************/
void amTaskExitSystem(AMS_TASK *taskp = _am_default_taskp);

/************************************************************************/
/* void amTaskExecute(AMS_TASK *taskp)                                  */
/*----------------------------------------------------------------------*/
/* [INPUT]  taskp : 実行するタスクシステム                              */
/* [FUNCTION] タスク実行                                                */
/************************************************************************/
void amTaskExecute(AMS_TASK *taskp = _am_default_taskp);

/************************************************************************/
/* void amTaskReset(AMS_TASK *taskp)                                    */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    taskp      : タスクシステム構造体                                 */
/* [FUNCTION]                                                           */
/*    タスクの全消去                                                    */
/************************************************************************/
void amTaskReset(AMS_TASK *taskp = _am_default_taskp);

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
AMS_TCB *amTaskMake(AMS_TASK *taskp, TaskProc proc, TaskProc dest,
		Uint32 prio, Uint32 user, Uint32 attr, char *name,
		Uint32 stall = 1, Sint32 group = 0, Uint32 run = 0xffffffff);
inline AMS_TCB *amTaskMake(TaskProc proc, TaskProc dest,
		Uint32 prio, Uint32 user, Uint32 attr, char *name,
		Uint32 stall = 1, Sint32 group = 0, Uint32 run = 0xffffffff)
{
	return	amTaskMake(_am_default_taskp, proc, dest, prio, user,
			attr, name, stall, group, run);
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
Sint32 amTaskPending(AMS_TCB *tcbp);


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
Sint32 amTaskStart(AMS_TCB *tcbp);


/************************************************************************/
/* void amTaskSetProcedure(AMS_TCB *tcb, TaskProc proc)                 */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    tcb  : タスクの TCB へのポインタ                                  */
/*    proc : プロシージャへのポインタ                                   */
/* [FUNCTION]                                                           */
/*    プロシージャの設定                                                */
/************************************************************************/
inline void amTaskSetProcedure(AMS_TCB *tcb, TaskProc proc) {
	tcb->procedure = (proc == NULL) ?
		(TaskProc)AMD_TASK_STATE_NULL : proc;
}

/************************************************************************/
/* void amTaskSetDestructor(AMS_TCB *tcb, TaskProc dest)                */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    tcb  : タスクの TCB へのポインタ                                  */
/*    dest : デストラクタへのポインタ                                   */
/* [FUNCTION]                                                           */
/*    デストラクタの設定                                                */
/************************************************************************/
inline void amTaskSetDestructor(AMS_TCB *tcb, TaskProc dest) {
	tcb->destructor = (dest == NULL) ?
		(TaskProc)AMD_TASK_STATE_NULL : dest;
}

/************************************************************************/
/* void amTaskDelete(AMS_TCB *tcb)                                      */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    tcb : 削除するタスクの TCB へのポインタ                           */
/* [FUNCTION]                                                           */
/*    タスク消去                                                        */
/************************************************************************/
void amTaskDelete(AMS_TCB *tcb);

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
void amTaskDeleteGroup(AMS_TASK *taskp,
		Uint32 user, Uint32 attr, Uint32 flag);
inline void amTaskDeleteGroup(Uint32 user, Uint32 attr, Uint32 flag)
{
	amTaskDeleteGroup(_am_default_taskp, user, attr, flag);
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
void amTaskDeletePriority(AMS_TASK *taskp,
		Uint32 prio_begin, Uint32 prio_end, Uint32 user = 0);
inline void amTaskDeletePriority(
		Uint32 prio_begin, Uint32 prio_end, Uint32 user = 0)
{
	amTaskDeletePriority(_am_default_taskp, prio_begin, prio_end, user);
}

/************************************************************************/
/* void amTaskSleep(AMS_TCB *tcb)                                       */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    tcb : 停止するタスクの TCB へのポインタ                           */
/* [FUNCTION]                                                           */
/*    タスク停止                                                        */
/************************************************************************/
void amTaskSleep(AMS_TCB *tcb);

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
void amTaskSleepGroup(AMS_TASK *taskp,
		Uint32 user, Uint32 attr, Uint32 flag);
inline void amTaskSleepGroup(Uint32 user, Uint32 attr, Uint32 flag)
{
	amTaskSleepGroup(_am_default_taskp, user, attr, flag);
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
void amTaskSleepPriority(AMS_TASK *taskp,
		Uint32 prio_begin, Uint32 prio_end, Uint32 user = 0);
inline void amTaskSleepPriority(
		Uint32 prio_begin, Uint32 prio_end, Uint32 user = 0)
{
	amTaskSleepPriority(_am_default_taskp, prio_begin, prio_end, user);
}

/************************************************************************/
/* void amTaskWakeup(AMS_TCB *tcb)                                      */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    tcb : 再開するタスクの TCB へのポインタ                           */
/* [FUNCTION]                                                           */
/*    タスク再開                                                        */
/************************************************************************/
void amTaskWakeup(AMS_TCB *tcb);

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
void amTaskWakeupGroup(AMS_TASK *taskp,
		Uint32 user, Uint32 attr, Uint32 flag);

inline void amTaskWakeupGroup(Uint32 user, Uint32 attr, Uint32 flag)
{
	amTaskWakeupGroup(_am_default_taskp, user, attr, flag);
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
void amTaskWakeupPriority(AMS_TASK *taskp,
		Uint32 prio_begin, Uint32 prio_end, Uint32 user = 0);
inline void amTaskWakeupPriority(
		Uint32 prio_begin, Uint32 prio_end, Uint32 user = 0)
{
	amTaskWakeupPriority(_am_default_taskp, prio_begin, prio_end, user);
}

/************************************************************************/
/* void * amTaskGetWork(AMS_TCB *tcb)                                   */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*    tcb : ワークを取得するタスクの TCB へのポインタ                   */
/* [RETURN]                                                             */
/*    ワークへのポインタ                                                */
/* [FUNCTION]                                                           */
/*    タスクのワーク領域へのポインタを取得                              */
/************************************************************************/
inline void *amTaskGetWork(AMS_TCB *tcb) {
	return tcb->work;
}

/************************************************************************/
/* void amTaskSetOwnerName(AMS_TASKLIST_OWNER *pList, Sint32 listSize)  */
/*----------------------------------------------------------------------*/
/* [INPUT]                                                              */
/*     pList : オーナー名リストへのポインタ                             */
/*  listSize : リストの要素数                                           */
/* [FUNCTION] タスクリストのオーナー名の設定                            */
/************************************************************************/
void amTaskSetOwnerName(const AMS_TASKLIST_OWNER *pList, Uint32 listSize);

#endif // _AM_TASK_H_
