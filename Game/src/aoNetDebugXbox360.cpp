// ===========================================================================
/*!
	@file	aoNetDebugXbox360.cpp
	@brief	Xbox360デバッグ用通信モジュール宣言

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "aoNetDebugXbox360.h"
#include "ao.h"
#include "gs.h"
#include "gsMainSys.h"

#if _XBOX
#if defined(MTD_DEBUG)

// ----- Macros ------------------------------------------------（マクロ定義）

#define AOD_NET_RANK_BOARD_NUM	(144)	//!< ボード数

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct AOS_NET_RANK_BOARD_INFO
// ---------------------------------------------------------------------------
//!	ボード情報
// ===========================================================================
typedef struct tag_AOS_NET_RANK_BOARD_INFO {
	u8		board;					//!< ボードID
	s8		region_col;				//!< リージョン列ID
	s8		ss_col;					//!< スーパーソニック列ID
	s8		stime_col;				//!< 表示タイム列
} AOS_NET_RANK_BOARD_INFO; // 4 byte

// ----- Class Definitions -------------------------------------（クラス宣言）

namespace ao {
namespace dbg {

// ===========================================================================
//! ランキングボード削除クラス
// ===========================================================================
class CRankDelete :
	public ao::CProc<CRankDelete>, public ao::CTask<CRankDelete>,
	public CThread<CRankDelete>, public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CRankDelete();

protected:

	//! デストラクタ
	virtual ~CRankDelete();

	// タスク
	void TaskProcMain();

	// プロシージャ
	void ProcReady();
	void ProcSelect();
	void ProcMessage();
	void ProcDeleting();
	void ProcSuccess();
	void ProcError();

	// スレッド
	void ThreadProcDelete();

	u32 m_select;	//!< 選択番号
	u32 m_count;	//!< カウンタ
	BOOL m_error;	//!< エラーフラグ
};

} // namespace dbg
} // namespace ao

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//! ボード数
// ===========================================================================
static const u32 g_ao_net_dbg_delete_board_num = 24;

// ===========================================================================
//! ボード名配列
// ===========================================================================
static const char*
	g_ao_net_dbg_delete_board_name_tbl[g_ao_net_dbg_delete_board_num] =
{
	"Zone1-1",
	"Zone1-2",
	"Zone1-3",
	"Zone1-Boss",
	"Zone2-1",
	"Zone2-2",
	"Zone2-3",
	"Zone2-Boss",
	"Zone2-1",
	"Zone2-2",
	"Zone2-3",
	"Zone2-Boss",
	"Zone2-1",
	"Zone2-2",
	"Zone2-3",
	"Zone2-Boss",
	"FinalZone",
	"SpesialStage1",
	"SpesialStage2",
	"SpesialStage3",
	"SpesialStage4",
	"SpesialStage5",
	"SpesialStage6",
	"SpesialStage7",
};

#include "aoNetRankXbox360Info.inc"

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! ランキングボード削除イベント開始
// ===========================================================================
void AoNetDebugXbox360DeleteRankingBoard(void* arg)
{
	new ao::dbg::CRankDelete();
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace ao {
namespace dbg {

// ***************************************************************************
// ランキングボード削除クラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CRankDelete::CRankDelete()
{
	m_select = 0;
	m_count = 0;

	// 準備プロシージャ設定
	SetProc(0, &CRankDelete::ProcReady);

	// メインタスク作成
	MakeTask(0, "CRankDelete::Main");

	// メインタスクプロシージャ設定
	SetTaskProc(0, &CRankDelete::TaskProcMain);

	// メインタスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CRankDelete::~CRankDelete()
{
	// デバッグランチャーに戻る
	SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
	SyChangeNextEvt();
}


// ***************************************************************************
// タスク
// ***************************************************************************
// ===========================================================================
//! メインタスクプロシージャ
// ===========================================================================
void CRankDelete::TaskProcMain()
{
	// プロシージャ確認
	if (IsProcNone(0)) {
		// 終了
		delete this;
	}
	else {
		// 実行
		Call(0);
	}
}


// ***************************************************************************
// プロシージャ
// ***************************************************************************
// ===========================================================================
//! 準備
// ===========================================================================
void CRankDelete::ProcReady()
{
	SetOwnProc(&CRankDelete::ProcSelect);
}

// ===========================================================================
//! 選択
// ===========================================================================
void CRankDelete::ProcSelect()
{
	amPrintf(4, 4, "PLEASE SELECT DELETE RANKING BOARD. %d", m_count++);
	amPrintf(4, 5, "%c:DECIDE", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
		if (m_select > 0) {
			m_select -= 1;
		}
		else {
			m_select = g_ao_net_dbg_delete_board_num;
		}
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
		if (m_select < g_ao_net_dbg_delete_board_num) {
			m_select += 1;
		}
		else {
			m_select = 0;
		}
	}

	for (u32 i = 0; i <= g_ao_net_dbg_delete_board_num; ++i) {
		if (i == m_select) {
			amPrint(4, (s32)(8 + i), ">");
		}
		if (i != g_ao_net_dbg_delete_board_num) {
			amPrintf(
				6, (s32)(8 + i), "%s", g_ao_net_dbg_delete_board_name_tbl[i]);
		}
		else {
			amPrint(6, (s32)(9 + i), "ALL");
		}
	}

	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
		SetOwnProc(&CRankDelete::ProcMessage);
	}
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		SetOwnProcNone();
	}
}

// ===========================================================================
//! 確認メッセージ表示中
// ===========================================================================
void CRankDelete::ProcMessage()
{
	amPrintf(4, 4, "MESSAGE SHOWING... %d", m_count++);

	if (GetCount() == 0) {
		AoSysMsgStart(
			L"指定のランキングボードを削除してもよろしいですか？\n"
			L"この操作は元の戻すことができません！！",
			AOD_SYS_MSG_SELECT_OKCANCEL);
	}

	if (AoSysMsgIsFinished()) {
		if (AoSysMsgGetResult() == AOD_SYS_MSG_SELECT_OK) {
			SetOwnProc(&CRankDelete::ProcDeleting);
		}
		else {
			SetOwnProc(&CRankDelete::ProcSelect);
		}
	}
}

// ===========================================================================
//! 削除中
// ===========================================================================
void CRankDelete::ProcDeleting()
{
	amPrintf(4, 4, "NOW DELETING... %d", m_count++);

	if (GetCount() == 0) {
		m_error = FALSE;
		SetThreadProc(0, &CRankDelete::ThreadProcDelete);
		StartThread(0, (AMD_CORE)0, (u32)THREAD_PRIORITY_BELOW_NORMAL);
	}

	if (IsEndThread(0)) {
		if (m_error) {
			SetOwnProc(&CRankDelete::ProcError);
		}
		else {
			SetOwnProc(&CRankDelete::ProcSuccess);
		}
	}
}

// ===========================================================================
//! 成功
// ===========================================================================
void CRankDelete::ProcSuccess()
{
	amPrintf(4, 4, "SUCCESS. %d", m_count++);
	amPrintf(4, 5, "%c:BACK", GsEnvDebugGetCancelKeyChar());

	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		SetOwnProc(&CRankDelete::ProcSelect);
	}
}

// ===========================================================================
//! エラー
// ===========================================================================
void CRankDelete::ProcError()
{
	amPrintf(4, 4, "ERROR. %d", m_count++);
	amPrintf(4, 5, "%c:BACK", GsEnvDebugGetCancelKeyChar());

	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		SetOwnProc(&CRankDelete::ProcSelect);
	}
}


// ***************************************************************************
// スレッド
// ***************************************************************************
// ===========================================================================
//! 削除スレッド
// ===========================================================================
void CRankDelete::ThreadProcDelete()
{
	DWORD result;
	u32 dboard_s;
	u32 dboard_num;

	if (m_select < g_ao_net_dbg_delete_board_num) {
		dboard_s = m_select;
		dboard_num = 1;
	}
	else {
		dboard_s = 0;
		dboard_num = g_ao_net_dbg_delete_board_num;
	}

	for (u32 b = 0; b < dboard_num; ++b) {
		u32 s_id = (u32)((dboard_s + b) * 6);

		for (u32 i = 0; i < 6; ++i) {
			result = XUserResetStatsViewAllUsers(
				g_ao_net_rank_board_info[s_id + i].board, NULL);
			if (result != ERROR_SUCCESS) {
				m_error = TRUE;
				return;
			}
		}
	}
}

} // namespace dbg
} // namespace ao

#endif // defined(MTD_DEBUG)
#endif // _XBOX

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
