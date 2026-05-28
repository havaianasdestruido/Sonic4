// ===========================================================================
/*!
	@file	dmRankSysWait.cpp
	@brief	ランキングシステム アクセス待ち制御モジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "dmRankSysWait.h"

#if defined(MTD_DEBUG)
#include "gs.h"
#include "gsMainSys.h"
#include "ao.h"
#endif // defined(MTD_DEBUG)

// ----- Macros ------------------------------------------------（マクロ定義）

#if _WII && !defined(DMD_RANK_SYS_WAIT_ACCESS_CTRL_DISABLE)

//! モジュール有効
#define DMD_RANK_SYS_WAIT_ENABLE	(1)

#endif // _WII && !defined(DMD_RANK_SYS_WAIT_ACCESS_CTRL_DISABLE)

//! アクセス数最大値
#define DMD_RANK_SYS_WAIT_ACCESS_MAX	(10)

//! アクセス数最大値分のアクセスにかける時間
#define DMD_RANK_SYS_WAIT_ACCESS_MAX_TIME	(300)

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

#if defined(DMD_RANK_SYS_WAIT_ENABLE)

// ===========================================================================
//	struct DMS_RANK_SYS_WAIT
// ---------------------------------------------------------------------------
//!	アクセス待ち制御構造体
// ===========================================================================
typedef struct DMS_RANK_SYS_WAIT {
	u32 base_time;									//!< 基準時間
	u32	access_time_num;							//!< アクセス数
	u32	access_time[DMD_RANK_SYS_WAIT_ACCESS_MAX];	//!< アクセス時間
} DMS_RANK_SYS_WAIT;

#endif // defined(DMD_RANK_SYS_WAIT_ENABLE)

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

#if defined(DMD_RANK_SYS_WAIT_ENABLE)

static u64 dmRankSysWaitGetTick(void);
static void dmRankSysWaitOptimize(DMS_RANK_SYS_WAIT& wait, u32 now);

#endif // defined(DMD_RANK_SYS_WAIT_ENABLE)

#if defined(MTD_DEBUG)
// テストイベントタスク
static void dmRankSysWaitTest0000(AMS_TCB* tcb);
#endif // defined(MTD_DEBUG)

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

#if defined(DMD_RANK_SYS_WAIT_ENABLE)

// ===========================================================================
//	DMS_RANK_SYS_WAIT g_dm_rank_sys_wait
// ---------------------------------------------------------------------------
//!	アクセス待ち制御構造体
// ===========================================================================
static DMS_RANK_SYS_WAIT g_dm_rank_sys_wait = { 0, 0, };

// ===========================================================================
//	u32 g_dm_rank_sys_wait_time_tbl[]
// ---------------------------------------------------------------------------
//!	過去5分以内のアクセス回数ごとの待ち時間
// ===========================================================================
static const u32
	g_dm_rank_sys_wait_time_tbl[DMD_RANK_SYS_WAIT_ACCESS_MAX + 1] =
{
	0,
	12,
	24,
	30,
	30,
	30,
	30,
	30,
	30,
	30,
	54,
};

#endif // defined(DMD_RANK_SYS_WAIT_ENABLE)

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//	DmRankSysWaitInit
/*!
	初期化

	@note
	制御開始時に呼び出してください。\n
*/
// ===========================================================================
void DmRankSysWaitInit(void)
{
#if defined(DMD_RANK_SYS_WAIT_ENABLE)

	g_dm_rank_sys_wait.base_time =
		(u32)OSTicksToSeconds(dmRankSysWaitGetTick());
	g_dm_rank_sys_wait.access_time_num = 0;

#endif // defined(DMD_RANK_SYS_WAIT_ENABLE)
}

// ===========================================================================
//	DmRankSysWaitGetWaitTime
/*!
	アクセス待ち時間取得

	@return アクセス待ち時間(ミリ秒単位)
*/
// ===========================================================================
u32 DmRankSysWaitGetWaitTime(void)
{
#if defined(DMD_RANK_SYS_WAIT_ENABLE)

	u32& base_time = g_dm_rank_sys_wait.base_time;
	u32& time_num = g_dm_rank_sys_wait.access_time_num;
	u32* times = g_dm_rank_sys_wait.access_time;

	amAssert(time_num <= DMD_RANK_SYS_WAIT_ACCESS_MAX);

	// 時間取得
	u32 time_now = (u32)(OSTicksToSeconds(dmRankSysWaitGetTick()) - base_time);

	// 5分以上前のアクセス時間を削除
	dmRankSysWaitOptimize(g_dm_rank_sys_wait, time_now);

	// 前回アクセス時間取得
	u32 prev_time = 0;
	if (time_num > 0) {
		prev_time = times[time_num - 1];
	}

	// 前回アクセスからの経過時間取得
	u32 diff = time_now - prev_time;

	// 規定の待ち時間取得
	u32 wait = g_dm_rank_sys_wait_time_tbl[time_num];

	// 待ち時間が経過したか判定
	if (diff >= wait) {
		return 0;
	}

	// 待ち時間を返す
	return (u32)(wait - diff);

#else

	return 0;

#endif // defined(DMD_RANK_SYS_WAIT_ENABLE)
}

// ===========================================================================
//	DmRankSysWaitNoticeTrans
/*!
	アクセス通知
*/
// ===========================================================================
void DmRankSysWaitNoticeTrans(void)
{
#if defined(DMD_RANK_SYS_WAIT_ENABLE)

	u32& base_time = g_dm_rank_sys_wait.base_time;
	u32& time_num = g_dm_rank_sys_wait.access_time_num;
	u32* times = g_dm_rank_sys_wait.access_time;

	// 時間取得
	u32 time_now = (u32)(OSTicksToSeconds(dmRankSysWaitGetTick()) - base_time);

	// 5分以上前のアクセス時間を削除
	dmRankSysWaitOptimize(g_dm_rank_sys_wait, time_now);
	if (time_num >= DMD_RANK_SYS_WAIT_ACCESS_MAX) {
		// エラー
		amAssert(0);
		DmRankSysWaitInit();
	}

	// アクセス時間設定
	times[time_num] = time_now;
	time_num += 1;

#endif // defined(DMD_RANK_SYS_WAIT_ENABLE)
}

#if defined(MTD_DEBUG)
// ===========================================================================
//! テストイベント
// ===========================================================================
void DmRankSysWaitTestStart(void *arg)
{
	UNREFERENCED_PARAMETER(arg);
	AMS_TCB* tcb = amTaskMake(
		dmRankSysWaitTest0000, NULL, 0, 0, 0, "dmRankSysWait::Test");
	u32* work = (u32*)amTaskGetWork(tcb);
	work[0] = 0;
	work[1] = 0;
	amTaskStart(tcb);
}
#endif // defined(MTD_DEBUG)

// ----- Static Functions --------------------（スタティック関数の定義：局所）

#if defined(DMD_RANK_SYS_WAIT_ENABLE)

// ===========================================================================
//! tick取得
// ===========================================================================
u64 dmRankSysWaitGetTick(void)
{
	return (u64)OSGetTime();
}

// ===========================================================================
//! 調整
// ===========================================================================
void dmRankSysWaitOptimize(DMS_RANK_SYS_WAIT& wait, u32 now)
{
	u32& time_num = wait.access_time_num;
	u32* times = wait.access_time;

	u32 i, j;
	for (i = 0; i < time_num; ++i) {
		if (now < (times[i] + DMD_RANK_SYS_WAIT_ACCESS_MAX_TIME)) {
			break;
		}
	}
	for (j = 0; j < time_num; ++j) {
		if ((i + j) >= DMD_RANK_SYS_WAIT_ACCESS_MAX) {
			break;
		}
		times[j] = times[i + j];
	}
	time_num -= i;
}

#endif // defined(DMD_RANK_SYS_WAIT_ENABLE)

#if defined(MTD_DEBUG)
// ***************************************************************************
// テストイベントタスク
// ***************************************************************************
// ===========================================================================
//! テストイベントタスクプロシージャ0000
// ===========================================================================
void dmRankSysWaitTest0000(AMS_TCB* tcb)
{
#if defined(DMD_RANK_SYS_WAIT_ENABLE)

	u32& base_time = g_dm_rank_sys_wait.base_time;
	u32& time_num = g_dm_rank_sys_wait.access_time_num;
	u32* times = g_dm_rank_sys_wait.access_time;
	amPrintf(4, 4, "WAIT:%d", DmRankSysWaitGetWaitTime());

	// 時間取得
	u32 time_now = (u32)(OSTicksToSeconds(dmRankSysWaitGetTick()) - base_time);

	for (u32 i = 0; i < time_num; ++i) {
		s32 d = 0;
		if (i > 0) {
			d = (s32)(times[i] - times[i - 1]);
		}
		amPrintf(
			4, (s32)(6 + i), "%2d : %8d : %8d : %3d",
			i, times[i], (u32)(time_now - times[i]), d);
	}

	u32* work = (u32*)amTaskGetWork(tcb);
	u32 ttime = (u32)(OSTicksToSeconds(dmRankSysWaitGetTick()) - work[0]);
	if (ttime >= DMD_RANK_SYS_WAIT_ACCESS_MAX_TIME) {
		if (work[1] > 10) {
			amAssert(0);
		}
		work[0] = (u32)OSTicksToSeconds(dmRankSysWaitGetTick());
		work[1] = 0;
	}

	amPrintf(4, 20, "%d:%d", ttime, work[1]);

	if (DmRankSysWaitGetWaitTime() == 0) {
	//	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
			DmRankSysWaitNoticeTrans();
			work[1] += 1;
	//	}
	}

	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		amTaskDelete(tcb);
		SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
		SyChangeNextEvt();
	}

#else

	amTaskDelete(tcb);
	SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
	SyChangeNextEvt();

#endif // defined(DMD_RANK_SYS_WAIT_ENABLE)
}
#endif // defined(MTD_DEBUG)

// ===========================================================================
//	function
/*!
	説明

	@param param0	[in] 入力引数0説明
	@param param1	[out] 出力ポインタ引数1説明
	@param param2	[io] 入出力ポインタ引数2説明
	@return 返値説明
	@note 補足説明
*/
// ===========================================================================

// ===========================================================================
//	int variable
// ---------------------------------------------------------------------------
//!	変数説明
// ===========================================================================

	// =======================================================================
	//	function
	/*!
		説明

		@param param0	[in] 入力引数0説明
		@param param1	[out] 出力ポインタ引数1説明
		@param param2	[io] 入出力ポインタ引数2説明
		@return 返値説明
		@note 補足説明
	*/
	// =======================================================================

// ***************************************************************************
// ラベル
// ***************************************************************************
