// ===========================================================================
/*!
	@file	gsPresence.cpp
	@brief	プレゼンスシステム定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "gsPresence.h"
#include "gs.h"
#include "gsMainSys.h"
#include "aoPresence.h"
#include "ao.h"

#if defined(GSD_PRESENCE_WATCH_ENABLE)

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct GSS_PRESENCE_WATCH
// ---------------------------------------------------------------------------
//!	プレゼンス監視タスクワーク
// ===========================================================================
typedef struct tag_GSS_PRESENCE_WATCH {
	s32				evt_id;		//!< イベントID
} GSS_PRESENCE_WATCH;

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// 監視タスク
static void gsPresenceWatchTaskProcedure(AMS_TCB* tcb);
static void gsPresenceWatchTaskDestructor(AMS_TCB* tcb);

// 便利
static BOOL gsPresenceIsGameEvent(s32 evt_id);
static u32 gsPresenceGetPresenceIdFromStageId();

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//	AMS_TCB* g_gs_presence_watch_tcb
// ---------------------------------------------------------------------------
//!	プレゼンス監視タスクTCBポインタ
// ===========================================================================
static AMS_TCB* g_gs_presence_watch_tcb = NULL;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//	GsPresenceWatchStart
/*!
	プレゼンス監視開始
*/
// ===========================================================================
void GsPresenceWatchStart(void)
{
	// 監視タスク作成
	g_gs_presence_watch_tcb = amTaskMake(
		gsPresenceWatchTaskProcedure, gsPresenceWatchTaskDestructor,
		0, 0, 0, "gsPresence::Watch");

	// 監視タスクワーク設定
	GSS_PRESENCE_WATCH* work =
		(GSS_PRESENCE_WATCH*)amTaskGetWork(g_gs_presence_watch_tcb);
	work->evt_id = -1;

	// 監視タスク開始
	amTaskStart(g_gs_presence_watch_tcb);
}

// ===========================================================================
//	GsPresenceWatchStart
/*!
	プレゼンス監視終了
*/
// ===========================================================================
void GsPresenceWatchEnd(void)
{
	if (g_gs_presence_watch_tcb) {
		amTaskDelete(g_gs_presence_watch_tcb);
		g_gs_presence_watch_tcb = NULL;
	}
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ***************************************************************************
// 監視タスク
// ***************************************************************************
// ===========================================================================
//! 監視タスクプロシージャ
// ===========================================================================
void gsPresenceWatchTaskProcedure(AMS_TCB* tcb)
{
	// ワーク取得
	GSS_PRESENCE_WATCH* work = (GSS_PRESENCE_WATCH*)amTaskGetWork(tcb);

	// 現在のイベントID取得
	s32 evt_id = (s32)SyGetEvtInfo()->cur_evt_id;

#if _XBOX
	if (!AoAccountIsAnySigninStateChanged())
#endif // _XBOX
	{
		// イベントIDが変わっていないなら処理しない
		if (evt_id == work->evt_id) {
			return;
		}

		// スペステ分岐では処理しない
		if (evt_id == GSD_EVT_ID_SPSTAGE_BRANCH) {
			work->evt_id = evt_id;
			return;
		}

		// メニュー間遷移なら処理しない
		if (!gsPresenceIsGameEvent(evt_id) &&
			!gsPresenceIsGameEvent(work->evt_id))
		{
			work->evt_id = evt_id;
			return;
		}
	}

	// 体験版判定
	BOOL is_trial = GsTrialIsTrial();

	if (gsPresenceIsGameEvent(evt_id)) {
		// ゲーム設定
		AoPresenceSet(
			(AOE_PRESENCE)gsPresenceGetPresenceIdFromStageId(), is_trial);
	}
	else {
		// メニュー設定
		AoPresenceSet(AOD_PRESENCE_STANDBY, is_trial);
	}

	// イベントID保持
	work->evt_id = evt_id;

#if _XBOX
	// サインイン状態変更クリア
	AoAccountClearAnySigninStateChanged();
#endif // _XBOX
}

// ===========================================================================
//! 監視タスクデストラクタ
// ===========================================================================
void gsPresenceWatchTaskDestructor(AMS_TCB* tcb)
{
	amAssert(tcb == g_gs_presence_watch_tcb);
	g_gs_presence_watch_tcb = NULL;
}

// ***************************************************************************
// 便利
// ***************************************************************************
// ===========================================================================
//! ゲームイベント判定
// ===========================================================================
BOOL gsPresenceIsGameEvent(s32 evt_id)
{
	if ((evt_id == GSD_EVT_ID_MAINGAME) ||
		(evt_id == GSD_EVT_ID_SPSTAGE_BRANCH))
	{
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! ステージIDからプレゼンスID取得
// ===========================================================================
u32 gsPresenceGetPresenceIdFromStageId()
{
	const u32 id_tbl[GSD_MAIN_STAGE_ID_MAX] = {
		AOD_PRESENCE_Z11T,
		AOD_PRESENCE_Z12T,
		AOD_PRESENCE_Z13T,
		AOD_PRESENCE_Z1BT,
		AOD_PRESENCE_Z21T,
		AOD_PRESENCE_Z22T,
		AOD_PRESENCE_Z23T,
		AOD_PRESENCE_Z2BT,
		AOD_PRESENCE_Z31T,
		AOD_PRESENCE_Z32T,
		AOD_PRESENCE_Z33T,
		AOD_PRESENCE_Z3BT,
		AOD_PRESENCE_Z41T,
		AOD_PRESENCE_Z42T,
		AOD_PRESENCE_Z43T,
		AOD_PRESENCE_Z4BT,
		AOD_PRESENCE_ZFBT,
		AOD_PRESENCE_ZFBT,
		AOD_PRESENCE_ZFBT,
		AOD_PRESENCE_ZFBT,
		AOD_PRESENCE_ZFBT,
		AOD_PRESENCE_SS1T,
		AOD_PRESENCE_SS2T,
		AOD_PRESENCE_SS3T,
		AOD_PRESENCE_SS4T,
		AOD_PRESENCE_SS5T,
		AOD_PRESENCE_SS6T,
		AOD_PRESENCE_SS7T,
	};
	u32 stage_id = (u32)GsGetMainSysInfo()->stage_id;
	if (stage_id >= (u32)GSD_MAIN_STAGE_ID_MAX) {
		stage_id = GSD_MAIN_STAGE_ID_1_1;
	}
	u32 presence_id = id_tbl[stage_id];
	if (GsGetMainSysInfo()->game_mode != GSD_GAME_MODE_TIME_ATTACK) {
		presence_id += 1;
	}
	return presence_id;
}

#endif // defined(GSD_PRESENCE_WATCH_ENABLE)

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
