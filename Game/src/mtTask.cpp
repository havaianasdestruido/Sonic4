// ==========================================================================
/*!
  @file mtTask.cpp
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: mtTask.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "mtTask.h"

//----- Definitions ---------------------------------------------------------
#define MTD_TASK_SYS_GROUP	(15)					//!< システム用グループ
#define MTD_TASK_SYS_UID	(AMD_TASK_UID_0F)		//!< システム用ユーザーID
#define MTD_TASK_SYS_ATTR	(AMD_TASK_ATTR_SYSTEM)	//!< システム用属性


#define MTD_TASK_TCB_FLAG_SYSTEM		((u32)(1 << 31))     ///< システム用

typedef struct tag_GSS_TASK_SYS {
	s32		pause_level;		//!< 発動中のポーズレベル
	s32		pause_level_set;	//!< 設定待機ポーズレベル
} GSS_TASK_SYS;



//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void mtTaskSystemMain(MTS_TASK_TCB *tcb);
static void mtTaskSystemDest(MTS_TASK_TCB *tcb);
static void mtTaskProcedure(AMS_TCB *am_tcb);
static void mtTaskDestructor(AMS_TCB *am_tcb);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static MTS_TASK_TCB	*gs_task_mt_system_tcb = NULL;		//!< GSタスクシステムTCB
static GSS_TASK_SYS gs_task_mtsys = {0};				//!< GSタスクシステムワーク

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// システム
// ==========================================================================
// ==========================================================================
// mtTaskInitSystem
/*!
 *	MT タスクシステム 初期化
 */
// ==========================================================================
void mtTaskInitSystem(void)
{
	if (gs_task_mt_system_tcb) {
		return;
	}

	gs_task_mt_system_tcb
		= MTM_TASK_MAKE_TCB(mtTaskSystemMain, mtTaskSystemDest, MTD_TASK_TCB_FLAG_SYSTEM/*flag*/,
					0xFFFF/*pause_level*/, 0/*prio*/, MTD_TASK_SYS_GROUP, 0/*work_size*/, "GS_TASKMT_SYS");


	memset(&gs_task_mtsys, 0, sizeof(gs_task_mtsys));

	gs_task_mtsys.pause_level = -1;
	gs_task_mtsys.pause_level_set = -1;
}

// ==========================================================================
// mtTaskExitSystem
/*!
 *	MT タスクシステム 終了処理
 */
// ==========================================================================
void mtTaskExitSystem(void)
{
	mtTaskClearTcb(gs_task_mt_system_tcb);
}


// ==========================================================================
// mtTaskMake
/*!
 *	MT タスク生成
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
MTS_TASK_TCB* mtTaskMake(GSF_TASK_PROCEDURE proc, GSF_TASK_DESTRUCTOR dest,
			u32 flag, u16 pause_level, u32 prio, s32 group, u32 work_size, const char *name)
{
#if 1

	AMS_TCB			*am_tcb;
	MTS_TASK_TCB	*tcb;

#if defined (MTD_DEBUG)
	// 一旦カット◆
	// OS_Printf("Warning! MTD_TASK_TCB_FLAG_IMMORTAL invalidity \n");
#endif // #if defined (MTD_DEBUG)
	//OS_Printf(".... NAME: %s \n", name);//qqq

	if (!(flag & MTD_TASK_TCB_FLAG_SYSTEM)) {
		// システム以外は MTD_TASK_GROUP_MAX 設定不可
		if ((u32)group >= MTD_TASK_GROUP_MAX) {
			MTM_ASSERT((u32)group < MTD_TASK_GROUP_MAX);
			group = MTD_TASK_GROUP_MAX - 1;
		}
	}

	// user : 所有者 (0:所有者設定なし(グループ削除対象外)) (MTのgroupのようなもの)
	am_tcb = amTaskMake(mtTaskProcedure, mtTaskDestructor,
					prio, AMD_TASK_UID_00 << group/*user*/, MTD_TASK_DEF_ATTR/*attr*/, (char*)name/*,
					stall, group, run*/);

	tcb = (MTS_TASK_TCB*)amTaskGetWork(am_tcb);

	tcb->am_tcb		= am_tcb;
	tcb->proc		= proc;
	tcb->dest		= dest;
	tcb->pause_level= pause_level;
	if (flag & MTD_TASK_TCB_FLAG_NO_PAUSE) {
		tcb->pause_level = 0xFFFF;
	}
	tcb->work		= NULL;
	if (work_size) {
		tcb->work = mtMemAllocSysTail(work_size);
		amZeroMemory(tcb->work, work_size);
	}

	amTaskStart(am_tcb);

	return (tcb);
#else
	AMS_TCB			*am_tcb;
	MTS_TASK_TCB	*tcb;

	am_tcb = amTaskMake(mtTaskProcedure, mtTaskDestructor,
					prio, user, attr, name,
					stall, group);

	tcb = (MTS_TASK_TCB*)amTaskGetWork(am_tcb);

	tcb->am_tcb		= am_tcb;
	tcb->proc		= proc;
	tcb->dest		= dest;
	tcb->pause_level= pause_level;
	tcb->work		= NULL;
	if (work_size) {
		tcb->work = amMemAllocLow(work_size);
		amZeroMemory(tcb->work, work_size);
	}

	return (tcb);
#endif
}

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
#if 0
MTS_TASK_TCB* mtTaskMakeMT(GSF_TASK_PROCEDURE proc, GSF_TASK_DESTRUCTOR dest, u32 flag, u16 pause_level, u32 prio, u32 group, u32 work_size, char *name)
{
	MTM_ASSERT(group < MTD_TASK_GROUP_MAX);	// 15はシステムで使用
	MTM_ASSERT(prio > 0);		// 優先0はシステムで使用

	return (mtTaskMake(proc, dest,
				prio, (AMD_TASK_UID_00 << group)/*user*/, MTD_TASK_DEF_ATTR/*attr*/, name,
				1/*stall*/, 0/*group*/, pause_level, work_size));
}
#endif

// ==========================================================================
// mtTaskGetAmTcb
/*!
 *	MT タスク  AM TCB取得
 *
 *	@param	tcb [in]	GS TCB
 */
// ==========================================================================
AMS_TCB* mtTaskGetAmTcb(MTS_TASK_TCB *tcb)
{
	MTM_ASSERT(tcb);

	return (tcb->am_tcb);
}

// ==========================================================================
// mtTaskGetTcbWork
/*!
 *	MT タスク  TCBワーク取得
 *
 *	@param	tcb [in]	GS TCB
 */
// ==========================================================================
void* mtTaskGetTcbWork(MTS_TASK_TCB *tcb)
{
	MTM_ASSERT(tcb);

	return (tcb->work);
}

// ==========================================================================
// mtTaskChangeTcbProcedure
/*!
 *	処理関数変更
 *
 *	@param	tcb		[in]	GS TCB
 *	@param	proc	[in]	処理関数のアドレス
 */
// ==========================================================================
void mtTaskChangeTcbProcedure(MTS_TASK_TCB *tcb, GSF_TASK_PROCEDURE proc)
{
	amAssert(tcb);

	tcb->proc = proc;
}

// ==========================================================================
// mtTaskChangeTcbDestructor
/*!
 *	終了関数変更
 *
 *	@param	tcb		[in]	GS TCB
 *	@param	dest	[in]	終了関数のアドレス
 */
// ==========================================================================
void mtTaskChangeTcbDestructor(MTS_TASK_TCB *tcb, GSF_TASK_DESTRUCTOR dest)
{
	amAssert(tcb);

	tcb->dest = dest;
}

// ==========================================================================
// mtTaskClearTcb
/*!
 *	タスクを削除する
 *
 *	@param	tcb		[in]	GS TCB
 */
// ==========================================================================
void mtTaskClearTcb(MTS_TASK_TCB *tcb)
{
	amAssert(tcb);

	amTaskDelete(tcb->am_tcb);
}

// ==========================================================================
// mtTaskClearTcbAll
/*!
 *	タスクを全て削除する
 *
 *	@param	tcb		[in]	GS TCB
 */
// ==========================================================================
void mtTaskClearTcbAll(void)
{
	amTaskDeleteGroup((u32)AMD_TASK_UID_ALL, (u32)AMD_TASK_ATTR_MAIN/*attr*/, (u32)AMD_TASK_SLPGRP_EXCLUSIVE);
}

// ==========================================================================
// mtTaskClearPriority
/*!
 *	タスクを優先範囲削除する
 *
 *	@param	prio_begin	[in]	削除開始優先(以上)
 *	@param	prio_end	[in]	削除開始優先(以下)
 */
// ==========================================================================
void mtTaskClearPriority(u32 prio_begin, u32 prio_end)
{
	amAssert(prio_begin != 0);	// 優先0はシステムが使用

	amTaskDeletePriority(prio_begin, prio_end, (u32)AMD_TASK_UID_ALL);
}

// ==========================================================================
// mtTaskClearGroup
/*!
 *	タスクをグループ削除する
 *
 *	@param	group		[in]	削除対象グループ
 */
// ==========================================================================
void mtTaskClearGroup(u32 group)
{
	MTM_ASSERT(group < MTD_TASK_GROUP_MAX);

	amTaskDeleteGroup((AMD_TASK_UID_00 << group), AMD_TASK_ATTR_MAIN/*attr*/, AMD_TASK_SLPGRP_EXCLUSIVE);
}


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
void mtTaskStartPause(u16 pause_level)
{
	if (pause_level >= 0xFFFF) {
		amAssert(!"mtTask:mtTaskStartPauseMT() Error pause_level\n");
		pause_level = 0xFFFE;
	}

	gs_task_mtsys.pause_level_set = pause_level;
}

// ==========================================================================
// mtTaskEndPause
/*!
 *	タスクポーズを終了する
 */
// ==========================================================================
void mtTaskEndPause(void)
{
	gs_task_mtsys.pause_level_set = -1;
}

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
BOOL mtTaskIsPaused(u16 *pause_level)
{
	if (pause_level) {
		if (gs_task_mtsys.pause_level_set >= 0) {
			*pause_level = (u16)gs_task_mtsys.pause_level_set;
		}
		else {
			*pause_level = 0;
		}
	}

	return (gs_task_mtsys.pause_level_set >= 0 ? TRUE : FALSE);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// システム
// ==========================================================================
// ==========================================================================
// mtTaskSystemMain
/*!
 *	GS タスク MT仕様 メイン処理
 *
 *	@param	tcb		[in]	GS TCB
 */
// ==========================================================================
void mtTaskSystemMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	// ポーズレベル設定
	gs_task_mtsys.pause_level = gs_task_mtsys.pause_level_set;
}

// ==========================================================================
// mtTaskMTSystemDest
/*!
 *	GS タスク MT仕様 終了処理
 *
 *	@param	tcb		[in]	GS TCB
 */
// ==========================================================================
void mtTaskSystemDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	gs_task_mt_system_tcb = NULL;
	amZeroMemory(&gs_task_mtsys, sizeof(gs_task_mtsys));
}


// ==========================================================================
// ユーザー処理
// ==========================================================================
// ==========================================================================
// mtTaskProcedure
/*!
 *	GS タスク プロシージャー
 *
 *	@param	am_tcb [in]	
 */
// ==========================================================================
void mtTaskProcedure(AMS_TCB *am_tcb)
{
	MTS_TASK_TCB	*tcb;

	tcb = (MTS_TASK_TCB*)amTaskGetWork(am_tcb);

	if (tcb->pause_level <= gs_task_mtsys.pause_level) {
		// ポーズ中
		return;
	}

	// ユーザープロシージャー
	if (tcb->proc) {
		tcb->proc(tcb);
	}
#if 0
	AMS_TCB_FOOTER* footer = am_tcb->footer;
#if 0
	if (footer->cpu_cnt > 10000) {
		printf("%s:%5d/%5d\n", am_tcb->name, footer->cpu_cnt, footer->cpu_cnt_max);
	}
#endif
	if (amTpIsTouchPush(0)) {
		if (strcmp(am_tcb->name, "GM_MAP_MAIN") == 0) {
			printf("%s:%5d\n", am_tcb->name, footer->cpu_cnt);
		}
	}
#endif
}

// ==========================================================================
// mtTaskDestructor
/*!
 *	GS タスク デストラクタ
 *
 *	@param	am_tcb [in]	
 */
// ==========================================================================
void mtTaskDestructor(AMS_TCB *am_tcb)
{
	MTS_TASK_TCB	*tcb;
	
	//printf("::::REMOVE: %s\n", am_tcb->name);//qqq

	tcb = (MTS_TASK_TCB*)amTaskGetWork(am_tcb);

	// ユーザーデストラクタ
	if (tcb->dest) {
		tcb->dest(tcb);
	}

	// ワーク解放
	if (tcb->work) {
		amMemFree(tcb->work);
	}
}



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
