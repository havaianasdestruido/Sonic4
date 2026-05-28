// ==========================================================================
/*!
  @file syEvtSys.c
  @brief イベントシステム

  @author Ishizaki
                Copyright(c) 2009 Dimps

  $Id: syEvtSys.cpp 20 2011-04-22 12:46:46Z thamada $
  $Date:: 2011-04-22 21:46:46 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 *
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#ifndef _DS
#include "mt.h"
#include "mi.h"
#endif

#include "syEvtSys.h"

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------
/* イベントデータが分岐先を2つ以上持っているか否かを判定 */
#define	SYM_CHECK_EVT_DATA_BRUNCH(_sy_evt_data)	(0 < (_sy_evt_data)->next_evt_id[1] ? TRUE : FALSE)

//----- Definitions ---------------------------------------------------------
/*** デバック ***/
#if defined (MTD_DEBUG)
#define	SYD_OVL_DEBUG_CONFLICT_CHECK		(1)			//!< オーバーレイ領域競合チェックを行う(MTD_DEBUG時のみ) 定義コメントアウトでチェックを行わない
#else
#endif

/*** システム設定 ***/
//#define SYD_EVT_SYS_TCB_PRI		(0x0100)			//!< イベントシステム TCB優先
//#define SYD_EVT_SYS_TCB_GROUP	(0xF0)				//!< イベントシステム TCBグループ

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
#if defined _DS
static void syEvtSys(void);
#else
static void syEvtSys(MTS_TASK_TCB *tcb);
#endif

static void syEvtSysOvlCallBack(void);

static void syDecideNextEvt(void);
#if (SYD_OVL_CONTROL_NUM > 1)
static void syChangeOverlay(void);
#endif

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static MTS_TASK_TCB	*sy_evt_tcb = NULL;			/* イベントシステムタスク */
static SYS_EVT_INFO	sy_evt_info;					/* イベント管理情報 */

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// SyInitEvtSys
/*!
 *	イベント管理システム初期化
 *
 *	@param	evt_data		[in]	イベントデータ
 *	@param	evt_data_num	[in]	イベントデータ数
 *	@param	start_evt_id	[in]	開始時イベントID
 *	@param	tcb_use			[in]	タスクシステムの使用設定
 *	@param	pri				[in]	タスクシステム使用時のタスク優先
 *	@param	group			[in]	タスクシステム使用時のタスクグループ
 *
 *	@note
 *		tcb_use の設定により、イベントシステムを自動実行させるか、\n
 *		任意に実行させるかを決めます。\n
 *		FALSEに設定した場合は、初期化後にメインループ等で syEvtSys を呼び出して下さい。
 */
// ==========================================================================
void SyInitEvtSys(const SYS_EVT_DATA *evt_data, s32 evt_data_num, s16 start_evt_id, BOOL tcb_use, u16 pri, u8 group)
{
	if (tcb_use) {
		/*** イベントシステムタスク作成 ***/
		sy_evt_tcb = MTM_TASK_MAKE_TCB(syEvtSys, NULL,
						MTD_TASK_TCB_FLAG_IMMORTAL | MTD_TASK_TCB_FLAG_NO_PAUSE, 0, pri, group,
						0, "SY_EVT_SYS");
	}

	/*** ワーク初期化 ***/
	memset(&sy_evt_info, 0, sizeof(SYS_EVT_INFO));
	/* メンバ設定 */
	sy_evt_info.evt_data		= evt_data;
	sy_evt_info.evt_data_num	= evt_data_num;
	sy_evt_info.cur_evt_data	= &evt_data[0];
	sy_evt_info.next_evt_data	= NULL;
	sy_evt_info.cur_evt_id		= 0;
	sy_evt_info.req_evt_id		= 0;
	sy_evt_info.flag			= SYD_EVT_FLAG_NOP;
#if defined _DS
	{
		s32		i;
		for (i = 0; i < SYD_OVL_CONTROL_NUM; i++) {
			sy_evt_info.ovl_context[i].ovl_id = (FSOverlayID)-1;
		}
	}
#endif // #if defined _DS

	/* 初回イベントリクエスト */
	SyDecideEvt(start_evt_id);
	SyChangeNextEvt();
}

// ==========================================================================
// SyExitEvtSys
/*!
 *	イベント管理システムの終了
 */
// ==========================================================================
void SyExitEvtSys(void)
{
	if (sy_evt_tcb) {
#if defined _DS
		sy_evt_tcb->flag &= ~MTD_TASK_TCB_FLAG_IMMORTAL;
#endif
		mtTaskClearTcb(sy_evt_tcb);
		sy_evt_tcb = NULL;
	}
}

/****************************************************************************/
// イベント情報インターフェース
/****************************************************************************/
// ==========================================================================
// SyGetEvtInfo
/*!
 *	イベント情報のポインタを取得する
 *
 *	@return	イベント情報ポインタ
 */
// ==========================================================================
SYS_EVT_INFO* SyGetEvtInfo(void)
{
	return (&sy_evt_info);
}

/****************************************************************************/
// 次イベント確定
/****************************************************************************/
// ==========================================================================
// SyDecideEvtCase
/*!
 *	現在のイベントデータのevt_case番目に設定されているイベントに移行確定時呼び出し
 *
 *	@param	evt_case	[in]	次イベントのケース番号
 */
// ==========================================================================
void SyDecideEvtCase(s16 evt_case)
{
	if (!sy_evt_info.cur_evt_data->next_evt_id[evt_case]) {
		MTM_ASSERT(!"syEvtSys.c::SyDecideEvtCase() evt_case error!");
#if defined (MTD_DEBUG)
		if (evt_case == 0) {
			MTM_ASSERT(!"syEvtSys.c::SyDecideEvtCase() next_evt_id list error!");
		}
#endif
		evt_case = 0;
	}

	sy_evt_info.req_evt_case	= evt_case;	// ケース保存
	SyDecideEvt(sy_evt_info.cur_evt_data->next_evt_id[evt_case]);
}

// ==========================================================================
// SyDecideEvtIdCase
/*!
 *	リクエストIDをイベント遷移リストに登録されているかチェックして遷移
 *
 *	@param	req_id	[in]	次イベントのケース番号
 */
// ==========================================================================
void SyDecideEvtIdCase(s16 req_id)
{
	s32	i;

	// リクエストイベントID 遷移リストチェック
	for (i = 0; i < SYD_NEXT_EVT_MAX && sy_evt_info.cur_evt_data->next_evt_id[i]; i++) {
		if (sy_evt_info.cur_evt_data->next_evt_id[i] == req_id) {
			break;
		}
	}
	if (i >= SYD_NEXT_EVT_MAX || sy_evt_info.cur_evt_data->next_evt_id[i] == 0) {
		MTM_ASSERT(!"syEvtSys.c::SyDecideEvtIdCase() req_id error !");
		req_id = sy_evt_info.cur_evt_data->next_evt_id[0];
	}

	SyDecideEvt(req_id);
}

// ==========================================================================
// SyDecideEvt
/*!
 *	req_id で示される イベントIDを持つイベントに移行確定時呼び出し
 *
 *	@param	req_id	[in]	次イベントのID
 *
 *	@note
 *	こちらはシステム等による強制イベント移行処理時に使用する\n
 *	通常のイベント移行はSyDecideEvtCaseで行う
 */
// ==========================================================================
void SyDecideEvt(s16 req_id)
{
#if defined (MTD_DEBUG)
	OS_Printf("SyDecideEvt: current event id:%d\n", sy_evt_info.cur_evt_id);
	OS_Printf("SyDecideEvt: decide evt id:%d\n", req_id);
#endif

	/* 引数チェック */
	if ((req_id <= 0) || (sy_evt_info.evt_data_num <= req_id)) {
#if defined (MTD_DEBUG)
		/* 不正な引数が渡された */
		MTM_ASSERT(0);
#endif
		return;
	} 
	
//qqq	
#define ALWAYS_START_WITH_GAME_MENU____NEVER_OPEN_DEVELOPER_MENU 	
#ifdef ALWAYS_START_WITH_GAME_MENU____NEVER_OPEN_DEVELOPER_MENU	
	if(req_id == 15) {
		req_id = 2;
		OS_Printf("SyDecideEvt: MPP change 15 to 2 for debug purpose only\n", req_id);		//qqq
	}
#endif

	/*** イベントをリクエストする ***/
	/* イベント番号保存 */
	sy_evt_info.req_evt_id	= req_id;
	/* 次のイベント確定 */
	syDecideNextEvt();
}

/****************************************************************************/
// イベント切り換え
/****************************************************************************/
// ==========================================================================
// SyChangeNextEvt
/*!
 *	次のイベントに切り替える
 *
 *	@note
 *	イベント終了時に呼び出す
 */
// ==========================================================================
void SyChangeNextEvt(void)
{
	const SYS_EVT_DATA	*evt_datap;

	/* 現在のイベントデータ取得 */
	evt_datap	= sy_evt_info.cur_evt_data;

	/*** 分岐先イベント判定 ***/
	if (sy_evt_info.req_evt_id < 0) {
	/* 分岐先が指定されていない */
#if defined (MTD_DEBUG)
		MTM_ASSERT(0);
#endif
		sy_evt_info.req_evt_id	= evt_datap->next_evt_id[0];	// 保険
	}

	/*** イベントチェンジ判定 ***/
	sy_evt_info.flag	= SYD_EVT_FLAG_REQ;

	/*** 引数設定クリア ***/
	sy_evt_info.arg_size = 0;
	MI_CpuClear8(sy_evt_info.arg, SYD_EVT_ARG_SIZE);
}

// ==========================================================================
// SyChangeNextEvtArg
/*!
 *	次のイベントに切り替える(引数付き)
 *
 *	@param	arg_size	[in]	引数サイズ
 *	@param	arg_buf		[in]	引数バッファ
 *
 *	@note
 *	イベント終了時に呼び出す \n
 *	SYD_EVT_ARG_SIZE バイトまでのデータを次の初期化に引き渡します \n
 *	引き渡された値は、次のイベントの初期化関数内のみで有効になります
 */
// ==========================================================================
void SyChangeNextEvtArg(u32 arg_size, void *arg_buf)
{
	// イベント切り替え
	SyChangeNextEvt();

	if (arg_size > SYD_EVT_ARG_SIZE) {
		MTM_ASSERT(!"syEvtSys.c::SyChangeNextEvtArg() arg_size error !");
		arg_size = SYD_EVT_ARG_SIZE;
	}

	if (arg_size) {
		MI_CpuCopy8(arg_buf, sy_evt_info.arg, arg_size);
		sy_evt_info.arg_size = arg_size;
	}
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// syEvtSys
/*!
 *	メインシステム
 */
// ==========================================================================
#if defined _DS
void syEvtSys(void)
#else
void syEvtSys(MTS_TASK_TCB *tcb)
#endif
{
	const SYS_EVT_DATA	*evt_datap;

#ifndef _DS
	UNREFERENCED_PARAMETER(tcb);
#endif

	/* 現在のイベントデータ取得 */
//	evt_datap	= sy_evt_info.cur_evt_data;

	/*** イベントチェンジ判定 ***/
	if (sy_evt_info.flag == SYD_EVT_FLAG_REQ) {
		/*** イベントの切り替え ***/
#if defined (MTD_DEBUG)
		OS_Printf("syEvtSys:change event id:%d\n", sy_evt_info.cur_evt_id);
#endif

		/* 現在のイベントデータ取得 */
		evt_datap	= sy_evt_info.cur_evt_data;

		/* ユーザー側終了処理 */
		if (evt_datap->exit_func != NULL) {
			evt_datap->exit_func();
		}
		/* システム側終了処理 */
		if (evt_datap->exit_sys_func != NULL) {
			evt_datap->exit_sys_func();
		}

		/* 古いイベントID保存 */
		sy_evt_info.old_evt_id	= sy_evt_info.cur_evt_id;

		/* イベントの切り替え */
		sy_evt_info.cur_evt_id		= sy_evt_info.req_evt_id;
		sy_evt_info.cur_evt_data	= 
					&sy_evt_info.evt_data[sy_evt_info.cur_evt_id];

#if defined (MTD_DEBUG)
#if defined _DS
		/*** ヒープ使用状況 ***/
		/* メインヒープ */
		OS_TPrintf("[MainHeap] free size        : %d\n", mtMemGetTotalFreeSizeMain());
		OS_TPrintf("[MainHeap] Allocatable size : %d\n", mtMemGetAllocatableSizeMain());
		/* システムヒープ */
		OS_TPrintf("[SystemHeap] free size        : %d\n", mtMemGetTotalFreeSizeSys());
		OS_TPrintf("[SystemHeap] Allocatable size : %d\n", mtMemGetAllocatableSizeSys());
		/* DTCMヒープ */
	//	OS_TPrintf("DTCMHeap free size        : %d\n", mtMemGetTotalFreeSizeDTCM());
	//	OS_TPrintf("DTCMHeap Allocatable size : %d\n", mtMemGetAllocatableSizeDTCM());

		/*** 実行TCB状況 ***/
		mtTaskPrintTcbList();

#endif // #if defined _DS
#endif	// MTD_DEBUG

		/* 切り替え後のイベントデータ取得 */
		evt_datap = sy_evt_info.cur_evt_data;


#if defined _DS
#if (SYD_OVL_CONTROL_NUM == 1)
		// matsuri使用

		/* オーバーレイ切り換え */
		if (evt_datap->ovl_id[0] != (FSOverlayID)-1) {
		// オーバーレイ切り替え有り
			/* フラグの設定 */
			sy_evt_info.flag = SYD_EVT_FLAG_OVL_WAIT;

			mtUtilChangeOverlayArm9(evt_datap->ovl_id[0], NULL);

			/* リクエストイベントIDクリア */
			sy_evt_info.req_evt_id = -1;

			/* 分岐先が一つのみであるか判定 */
			if (!(SYM_CHECK_EVT_DATA_BRUNCH(evt_datap))) {
			/* 分岐先は一つのみ */
				/* 分岐先確定 */
				sy_evt_info.req_evt_id	= evt_datap->next_evt_id[0];
				/* 次のイベント確定 */
				syDecideNextEvt();
			}
			syEvtSysOvlCallBack();
		}
		else {
		// オーバーレイ切り替え無し
			/* リクエストイベントIDクリア */
			sy_evt_info.req_evt_id = -1;

			/* 分岐先が一つのみであるか判定 */
			if (!(SYM_CHECK_EVT_DATA_BRUNCH(evt_datap))) {
			/* 分岐先は一つのみ */
				/* 分岐先確定 */
				sy_evt_info.req_evt_id	= evt_datap->next_evt_id[0];
				/* 次のイベント確定 */
				syDecideNextEvt();
			}
			syEvtSysOvlCallBack();
		}
#else // #if (SYD_OVL_CONTROL_NUM == 1)
		// sy使用

		/* オーバーレイ切り換え */
		if (evt_datap->ovl_id[0] != (FSOverlayID)-1) {
		// オーバーレイ切り替え有り
			/* フラグの設定 */
			sy_evt_info.flag = SYD_EVT_FLAG_OVL_WAIT;

			/* オーバーレイ切り替え */
			syChangeOverlay();
		}

		/* リクエストイベントIDクリア */
		sy_evt_info.req_evt_id = -1;

		/* 分岐先が一つのみであるか判定 */
		if (!(SYM_CHECK_EVT_DATA_BRUNCH(evt_datap))) {
		/* 分岐先は一つのみ */
			/* 分岐先確定 */
			sy_evt_info.req_evt_id	= evt_datap->next_evt_id[0];
			/* 次のイベント確定 */
			syDecideNextEvt();
		}

		syEvtSysOvlCallBack();
#endif // #if (SYD_OVL_CONTROL_NUM == 1)

#else // #if defined _DS
		
		/* リクエストイベントIDクリア */
		sy_evt_info.req_evt_id = -1;
		/* 分岐先が一つのみであるか判定 */
		if (!(SYM_CHECK_EVT_DATA_BRUNCH(evt_datap))) {
		/* 分岐先は一つのみ */
			/* 分岐先確定 */
			sy_evt_info.req_evt_id	= evt_datap->next_evt_id[0];
			/* 次のイベント確定 */
			syDecideNextEvt();
		}
		syEvtSysOvlCallBack();

#endif // #if defined _DS
	}
}

// ==========================================================================
// syEvtSysOvlCallBack
/*!
 *	メインシステム オーバーレイ切り換え時コールバック
 */
// ==========================================================================
void syEvtSysOvlCallBack(void)
{
	const SYS_EVT_DATA	*evt_datap;

	/* 現在のイベントデータ取得 */
	evt_datap	= sy_evt_info.cur_evt_data;

	/* フラグの初期化 */
	sy_evt_info.flag	= SYD_EVT_FLAG_NOP;
	/* システム側初期化処理 */
	if (evt_datap->init_sys_func != NULL) {
		evt_datap->init_sys_func();
	}
	/* ユーザー側初期化処理 */
	if (evt_datap->init_func != NULL) {	// 念のため
		void *arg = NULL;

		if (sy_evt_info.arg_size) {
			arg = sy_evt_info.arg;
		}
		evt_datap->init_func(arg);
	}
}

// ==========================================================================
// syDecideNextEvt
/*!
 *	次イベント確定
 */
// ==========================================================================
void syDecideNextEvt(void)
{
//	const SYS_EVT_DATA	*evt_datap;

	if (sy_evt_info.req_evt_id < 0) {
	/* 分岐先が指定されていない */
#if defined (MTD_DEBUG)
		MTM_ASSERT(0);
#endif
		sy_evt_info.req_evt_id	= sy_evt_info.cur_evt_data->next_evt_id[0];	// 保険
	}

	/* 分岐先確定 */
	sy_evt_info.next_evt_data	= &sy_evt_info.evt_data[sy_evt_info.req_evt_id];

#if 0
	/* 次のイベントデータ取得 */
	evt_datap	= sy_evt_info.next_evt_data;

#if AMD_READ_FILE_BACKGROUND
	/* データロード */
	if (evt_datap->load_func) {
		evt_datap->load_func();
	}
#endif
#endif
}


#if (SYD_OVL_CONTROL_NUM > 1)
// ==========================================================================
// オーバーレイ
// ==========================================================================
// ==========================================================================
// syChangeOverlay
/*!
 *	
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void syChangeOverlay(void)
{
	s32					i, j;
	const SYS_EVT_DATA	*evt_datap = sy_evt_info.cur_evt_data;
	SYS_OVL_CONTEXT		ovl_context;
	BOOL				new_ovl[SYD_OVL_CONTROL_NUM] = {FALSE};
	BOOL				old_ovl[SYD_OVL_CONTROL_NUM] = {FALSE};
	FSOverlayInfo		ovl_info;

#if 0
	// 要転送オーバーレイチェック
	for (j = 0; j < SYD_OVL_CONTROL_NUM; j++) {
		if (sy_evt_info.ovl_context[j].ovl_id != (FSOverlayID)-1) {
			for (i = 0; i < SYD_OVL_CONTROL_NUM && evt_datap->ovl_id[i] != (FSOverlayID)-1; i++) {
				if (evt_datap->ovl_id[i] == sy_evt_info.ovl_context[j].ovl_id) {
					// 既に転送済み
					new_ovl[i] = TRUE;
					old_ovl[j] = TRUE;
					break;
				}
			}
		}
	}
#else
	// 要転送オーバーレイチェック
	for (i = 0; i < SYD_OVL_CONTROL_NUM && evt_datap->ovl_id[i] != (FSOverlayID)-1; i++) {
		for (j = 0; j < SYD_OVL_CONTROL_NUM; j++) {
			if (evt_datap->ovl_id[i] == sy_evt_info.ovl_context[j].ovl_id) {
				// 既に転送済み
				new_ovl[i] = TRUE;
				old_ovl[j] = TRUE;
				break;
			}
		}
	}
#endif

	// 不要オーバーレイ開放
	for (j = 0; j < SYD_OVL_CONTROL_NUM; j++) {
		if (!old_ovl[j] && sy_evt_info.ovl_context[j].ovl_id != (FSOverlayID)-1) {
			if (!FS_UnloadOverlay(MI_PROCESSOR_ARM9, sy_evt_info.ovl_context[j].ovl_id)) {
				OS_TPrintf("syEvtSys::syChangeOverlay() error! FS_UnloadOverlay Failed! ID=%d\n", sy_evt_info.ovl_context[j].ovl_id);
				MTM_ASSERT(0);
			}

			// コンテキストクリア
			sy_evt_info.ovl_context[j].ovl_id = (FSOverlayID)-1;
		}
	}

	// 未転送オーバーレイ転送
	for (i = 0, j = 0; i < SYD_OVL_CONTROL_NUM && evt_datap->ovl_id[i] != (FSOverlayID)-1; i++) {
		if (new_ovl[i]) {
			// 転送済み
			continue;
		}

		// オーバーレイ情報取得
		if (!FS_LoadOverlayInfo(&ovl_info, MI_PROCESSOR_ARM9, evt_datap->ovl_id[i])) {
			MTM_ASSERT(!"syEvtSys::syChangeOverlay() fatal error! Can't get overlay info");
		}
		ovl_context.ovl_id = evt_datap->ovl_id[i];
		ovl_context.addr = FS_GetOverlayAddress(&ovl_info);
		ovl_context.size = FS_GetOverlayTotalSize(&ovl_info);

#if defined (MTD_DEBUG) && defined (SYD_OVL_DEBUG_CONFLICT_CHECK)
		// 領域競合チェック
		{
			s32	conflict_cnt;
			u32	end_addr = (u32)ovl_context.addr + ovl_context.size;

			for (conflict_cnt = 0; conflict_cnt < SYD_OVL_CONTROL_NUM; conflict_cnt++) {
				if (sy_evt_info.ovl_context[conflict_cnt].ovl_id != (FSOverlayID)-1) {

					if (((u32)ovl_context.addr >= (u32)sy_evt_info.ovl_context[conflict_cnt].addr &&
								(u32)ovl_context.addr < (u32)sy_evt_info.ovl_context[conflict_cnt].addr + (u32)sy_evt_info.ovl_context[conflict_cnt].size) ||
							((u32)ovl_context.addr < (u32)sy_evt_info.ovl_context[conflict_cnt].addr &&
								end_addr > (u32)sy_evt_info.ovl_context[conflict_cnt].addr)) {
						OS_TPrintf("syEvtSys::syChangeOverlay() error! overlay conflict EVENT = %d, ovlID=%d, ovlID=%d\n",
														sy_evt_info.cur_evt_id, sy_evt_info.ovl_context[conflict_cnt].ovl_id, evt_datap->ovl_id[i]);
						MTM_ASSERT(0);
					}
				}
			}
		}
#endif	// #if defined (MTD_DEBUG)

		for (; j < SYD_OVL_CONTROL_NUM; j++) {
			if (sy_evt_info.ovl_context[j].ovl_id != (FSOverlayID)-1) {
				continue;
			}

			// 転送
			if (!FS_LoadOverlay(MI_PROCESSOR_ARM9, evt_datap->ovl_id[i])) {
				OS_TPrintf("syEvtSys::syChangeOverlay() error! FS_LoadOverlay Failed! ID=%d\n", evt_datap->ovl_id[i]);
				MTM_ASSERT(0);
			}

			// コンテキスト設定
			sy_evt_info.ovl_context[j] = ovl_context;
			break;
		}
	}
}

#endif


// ==========================================================================
// syEvtStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void syEvtStaticVarInit(void)
{
	sy_evt_tcb = NULL;			/* イベントシステムタスク */
	memset(&sy_evt_info, 0, sizeof(sy_evt_info));
	sy_evt_info;				/* イベント管理情報 */
}

// ==========================================================================
// _sy
/*!
 *	
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
