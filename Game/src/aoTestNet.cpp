// ===========================================================================
/*!
	@file	aoTest.cpp
	@brief	テスト定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "aoTest.h"
#include "ao.h"
#include "dmRankSys.h"
#include "akUtil.h"

#include "gs.h"
#include "gsMainSys.h"

#if defined (MTD_DEBUG)

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

namespace ao {
namespace test {
namespace net {

//! 通信テストクラス
class CMain : public CProc<CMain>, public CTask<CMain>, public CAllocAmNormal
{
public:

	//! コンストラクタ
	CMain();

	//! デストラクタ
	virtual ~CMain();

protected:

	// タスクプロシージャ
	void TaskProcMain();

	// プロシージャ
	void ProcReady();
	void ProcLoading();
	void ProcSaving();
	void ProcConnecting();
	void ProcSelectBoard();
	void ProcEditRecord();
	void ProcSending();
	void ProcSelectRank();
	void ProcRecving();
	void ProcRecvSuccess();
	void ProcDisconnecting();
	void ProcDisconnected();
	void ProcErrorDisconnecting();
	void ProcErrorDisconnected();

	u32 m_count; //! カウンタ
	BOOL m_is_need_font_release; //!< フォント解放必要フラグ

	u32 m_select;		//!< 選択
	u32 m_board;		//!< ボード
	u32 m_rank;			//!< ランキングタイプ
	u32 m_ss;			//!< スーパーソニックタイプ
	BOOL m_is_friend;	//!< フレンドランキングフラグ
	BOOL m_is_upload;	//!< アップロードフラグ
	u32 m_top_rank;		//!< 表示トップランク
	BOOL m_is_near;		//!< 周辺ランキング
};

} // namespace net
} // namespace test
} // namespace ao

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! テスト開始
// ===========================================================================
void AoTestNetStart(void* arg)
{
	UNREFERENCED_PARAMETER(arg);
	new ao::test::net::CMain;
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace ao {
namespace test {
namespace net {

// ***************************************************************************
// 通信テストクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CMain::CMain()
{
	m_count = 0;

	// タスク作成
	MakeTask(0, "aoTest::Net");

	// プロシージャ設定
	SetProc(0, &CMain::ProcLoading);

	// タスクプロシージャ設定
	SetTaskProc(0, &CMain::TaskProcMain);

	// タスク開始
	StartTask(0);

	// フォント
	m_is_need_font_release = FALSE;
	if (!GsFontIsBuilding() && !GsFontIsBuilded()) {
		GsFontBuild();
		m_is_need_font_release = TRUE;
	}
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CMain::~CMain()
{
	// フォント解放
	if (m_is_need_font_release) {
		GsFontRelease();
		m_is_need_font_release = FALSE;
	}

	// デバッグランチャーに戻る
	SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
	SyChangeNextEvt();
}


// ***************************************************************************
// タスクプロシージャ
// ***************************************************************************
// ===========================================================================
//! タスクプロシージャ
// ===========================================================================
void CMain::TaskProcMain()
{
	// プロシージャ確認
	if (IsProcNone(0)) {
		// 終了
		DeleteOwnTask();
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
//! ロード
// ===========================================================================
void CMain::ProcLoading()
{
	// 画面表示
	amPrintf(4, 4, "NOW LOADING... %d", m_count++);

	if (GetCount() == 0) {
		// ロード開始
		gs::backup::SBackup& backup =
			gs::backup::SBackup::CreateInstance();
		AoStorageLoadStart(&backup, sizeof(backup));
	}

	// ロード完了判定
	if (AoStorageLoadIsFinished()) {
		// セーブへ遷移
		SetOwnProc(&CMain::ProcSaving);
	}
}

// ===========================================================================
//! セーブ
// ===========================================================================
void CMain::ProcSaving()
{
	// 画面表示
	amPrintf(4, 4, "NOW SAVING... %d", m_count++);

	if (GetCount() == 0) {
		// ロード開始
		gs::backup::SBackup& backup =
			gs::backup::SBackup::CreateInstance();
		AoStorageSaveStart(&backup, sizeof(backup), TRUE);
	}

	// ロード完了判定
	if (AoStorageSaveIsFinished()) {
		// 準備へ遷移
		SetOwnProc(&CMain::ProcReady);
	}
}

// ===========================================================================
//! 準備
// ===========================================================================
void CMain::ProcReady()
{
	// 画面表示
	amPrintf(4, 4, "PLEASE PUSH ANY KEY. %d", m_count++);
	amPrintf(4, 5, "%c:CONNECT", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	// 開始判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {

		// 通信開始
		DmRankSysStart();

		// 接続中に遷移
		SetOwnProc(&CMain::ProcConnecting);
	}

	// 終了判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		delete this;
	}
}

// ===========================================================================
//! 接続中
// ===========================================================================
void CMain::ProcConnecting()
{
	// 画面表示
	amPrintf(4, 4, "NOW CONNECTING... %d", m_count++);
	amPrintf(4, 5, "%c:CANCEL", GsEnvDebugGetCancelKeyChar());

	// エラー判定
	if (DmRankSysIsError()) {
		// 切断開始
		DmRankSysEnd();

		// 切断中(エラー)へ遷移
		SetOwnProc(&CMain::ProcErrorDisconnecting);
		return;
	}

	// 終了判定
	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		// 切断開始
		DmRankSysEnd();

		// 切断中へ遷移
		SetOwnProc(&CMain::ProcDisconnecting);
		return;
	}

	// 接続完了判定
	if (DmRankSysIsConnected()) {

		m_board = 0;
		m_rank = 0;
		m_ss = 0;
		m_is_friend = FALSE;
		m_is_upload = TRUE;

		// ボード選択へ遷移
		SetOwnProc(&CMain::ProcSelectBoard);
	}
}

//! ボード名配列
static const char* g_ao_test_net_board_name[DMD_RANK_SYS_BOARD_NUM] = {
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

// ===========================================================================
//! ボード選択
// ===========================================================================
void CMain::ProcSelectBoard()
{
	// 画面表示
	amPrintf(4, 4, "SELECT BOARD. %d", m_count++);
	amPrintf(4, 5, "%c:NEXT", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:DISCONNECT", GsEnvDebugGetCancelKeyChar());

	// エラー判定
	if (DmRankSysIsError()) {
		// 切断開始
		DmRankSysEnd();

		// 切断中(エラー)へ遷移
		SetOwnProc(&CMain::ProcErrorDisconnecting);
		return;
	}

	if (GetCount() == 0) {
		m_select = 0;
	}

	const char* bool_name[2] = {
		"ON",
		"OFF",
	};

#if _WII

	if (AoPadSomeoneMRepeat(GSD_KEY_LEFT) >= 0) {
		m_select = (u32)((m_select + 1) % 2);
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_RIGHT) >= 0) {
		m_select = (u32)((m_select + 1) % 2);
	}

	if (m_select == 0) {
		if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
			m_board = (u32)(
				(m_board + (DMD_RANK_SYS_BOARD_NUM - 1)) %
				DMD_RANK_SYS_BOARD_NUM);
		}
		if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
			m_board = (u32)((m_board + 1) % DMD_RANK_SYS_BOARD_NUM);
		}
	}
	else if (m_select == 1) {
		if ((AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) ||
			(AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0))
		{
			m_is_upload = !m_is_upload;
		}
	}

	m_rank = (u32)DMD_RANK_SYS_RANK_TIME;
	m_ss = (u32)DMD_RANK_SYS_SS_BOTH;
	m_is_friend = FALSE;

	amPrintColor(0xffffff00);
	amPrint(4, 8, "BOARD");
	for (u32 i = 0; i < DMD_RANK_SYS_BOARD_NUM; ++i) {
		if (i == (u32)m_board) {
			if (m_select == 0) {
				amPrintColor(0xff0000ff);
			}
			else {
				amPrintColor(0xff00ffff);
			}
		}
		else {
			amPrintColor(0xffffffff);
		}
		amPrintf(4, (s32)(9 + i), "%s", g_ao_test_net_board_name[i]);
	}

	amPrintColor(0xffffff00);
	amPrint(20, 8, "UPLOAD");
	for (u32 i = 0; i < 2; ++i) {
		if (i != (u32)m_is_upload) {
			if (m_select == 1) {
				amPrintColor(0xff0000ff);
			}
			else {
				amPrintColor(0xff00ffff);
			}
		}
		else {
			amPrintColor(0xffffffff);
		}
		amPrintf(20, (s32)(9 + i), "%s", bool_name[i]);
	}

#else

	if (AoPadSomeoneMRepeat(GSD_KEY_LEFT) >= 0) {
		m_select = (u32)((m_select + 3) % 4);
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_RIGHT) >= 0) {
		m_select = (u32)((m_select + 1) % 4);
	}

	if (m_select == 0) {
		if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
			m_board = (u32)(
				(m_board + (DMD_RANK_SYS_BOARD_NUM - 1)) %
				DMD_RANK_SYS_BOARD_NUM);
		}
		if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
			m_board = (u32)((m_board + 1) % DMD_RANK_SYS_BOARD_NUM);
		}
	}
	else if (m_select == 1) {
		if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
			m_rank = (u32)(
				(m_rank + (DMD_RANK_SYS_RANK_NUM - 1)) %
				DMD_RANK_SYS_RANK_NUM);
		}
		if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
			m_rank = (u32)((m_rank + 1) % DMD_RANK_SYS_RANK_NUM);
		}
	}
	else if (m_select == 2) {
		if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
			m_ss = (u32)(
				(m_ss + (DMD_RANK_SYS_SS_NUM - 1)) %
				DMD_RANK_SYS_SS_NUM);
		}
		if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
			m_ss = (u32)((m_ss + 1) % DMD_RANK_SYS_SS_NUM);
		}
	}
	else if (m_select == 3) {
		if ((AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) ||
			(AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0))
		{
			m_is_friend = !m_is_friend;
		}
	}

	const char* rank_name[DMD_RANK_SYS_RANK_NUM] = {
		"TimeAttack", "ScoreAttack",
	};

	const char* ss_name[DMD_RANK_SYS_SS_NUM] = {
		"Both", "Disable", "Enable",
	};

	m_is_upload = TRUE;

	amPrintColor(0xffffff00);
	amPrint(4, 8, "BOARD");
	for (u32 i = 0; i < DMD_RANK_SYS_BOARD_NUM; ++i) {
		if (i == (u32)m_board) {
			if (m_select == 0) {
				amPrintColor(0xff0000ff);
			}
			else {
				amPrintColor(0xff00ffff);
			}
		}
		else {
			amPrintColor(0xffffffff);
		}
		amPrintf(4, (s32)(9 + i), "%s", g_ao_test_net_board_name[i]);
	}

	amPrintColor(0xffffff00);
	amPrint(20, 8, "RANKING");
	for (u32 i = 0; i < DMD_RANK_SYS_RANK_NUM; ++i) {
		if (i == (u32)m_rank) {
			if (m_select == 1) {
				amPrintColor(0xff0000ff);
			}
			else {
				amPrintColor(0xff00ffff);
			}
		}
		else {
			amPrintColor(0xffffffff);
		}
		amPrintf(20, (s32)(9 + i), "%s", rank_name[i]);
	}

	amPrintColor(0xffffff00);
	amPrint(32, 8, "SUPER");
	for (u32 i = 0; i < DMD_RANK_SYS_SS_NUM; ++i) {
		if (i == (u32)m_ss) {
			if (m_select == 2) {
				amPrintColor(0xff0000ff);
			}
			else {
				amPrintColor(0xff00ffff);
			}
		}
		else {
			amPrintColor(0xffffffff);
		}
		amPrintf(32, (s32)(9 + i), "%s", ss_name[i]);
	}

	amPrintColor(0xffffff00);
	amPrint(42, 8, "FRIEND");
	for (u32 i = 0; i < 2; ++i) {
		if (i != (u32)m_is_friend) {
			if (m_select == 3) {
				amPrintColor(0xff0000ff);
			}
			else {
				amPrintColor(0xff00ffff);
			}
		}
		else {
			amPrintColor(0xffffffff);
		}
		amPrintf(42, (s32)(9 + i), "%s", bool_name[i]);
	}

#endif // _WII

	amPrintColor(0xffffffff);

	// 開始判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
		// 編集中へ遷移
		SetOwnProc(&CMain::ProcEditRecord);
	}

	// 終了判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		// 切断開始
		DmRankSysEnd();

		// 切断中へ遷移
		SetOwnProc(&CMain::ProcDisconnecting);
	}
}

static void aoTestNetUtilPrintColor(s32 y, BOOL select, BOOL enable);
static BOOL aoTestNetUtilEditValue(u32& value, u32 min, u32 max, u32 one);
static BOOL aoTestNetUitlEditFlag(bool& flag);

//! printカラー設定
static void aoTestNetUtilPrintColor(s32 y, BOOL select, BOOL enable)
{
	if (select) {
		amPrintColor(0xff0000ff);
		amPrint(4, y, ">");
	}
	else {
		if (enable) {
			amPrintColor(0xffffffff);
		}
		else {
			amPrintColor(0xff7f7f7f);
		}
	}
}

//! 数値編集
BOOL aoTestNetUtilEditValue(u32& value, u32 min, u32 max, u32 one)
{
	u32 v = value;
	if ((AoPadSomeoneRepeat(GSD_KEY_LEFT) >= 0) ||
		(AoPadSomeoneADirect(GSD_KEY_LEFT) >= 0))
	{
		if ((s32)(value - one) < (s32)min) {
			value = 0;
		}
		else {
			value -= one;
		}
	}
	if ((AoPadSomeoneRepeat(GSD_KEY_RIGHT) >= 0) ||
		(AoPadSomeoneADirect(GSD_KEY_RIGHT) >= 0))
	{
		if ((s32)(value + one) > (s32)max) {
			value = max;
		}
		else {
			value += one;
		}
	}
	if (value != v) {
		return TRUE;
	}
	return FALSE;
}

//! フラグ編集
BOOL aoTestNetUitlEditFlag(bool& flag)
{
	if ((AoPadSomeoneMStand(GSD_KEY_LEFT) >= 0) ||
		(AoPadSomeoneMStand(GSD_KEY_RIGHT) >= 0))
	{
		if (flag) {
			flag = false;
		}
		else {
			flag = true;
		}
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! 記録編集
// ===========================================================================
void CMain::ProcEditRecord()
{
	// 画面表示
	amPrintf(4, 4, "EDIT RECORD. %d", m_count++);
	amPrintf(4, 5, "%c:SEND", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:BACK", GsEnvDebugGetCancelKeyChar());

	// エラー判定
	if (DmRankSysIsError()) {
		// 切断開始
		DmRankSysEnd();

		// 切断中(エラー)へ遷移
		SetOwnProc(&CMain::ProcErrorDisconnecting);
		return;
	}

	if (GetCount() == 0) {
		m_select = 0;
	}

	// ボード
	amPrintColor(0xffffff00);
	amPrint(4, 8, "BOARD");
	amPrintColor(0xffffffff);
	amPrintf(4, 9, "%s", g_ao_test_net_board_name[m_board]);

	// 記録
	amPrintColor(0xffffff00);
	amPrint(4, 11, "RECORD");

	u16 min, sec, msec;

	s32 y = 12;
	if (m_board < (u32)DMD_RANK_SYS_BOARD_SS_S) {
		// 通常ステージ

		const u32 max_time = gs::backup::SStageSolo::c_fast_time_max_limit;
		const u32 max_score = gs::backup::SStageSolo::c_high_score_max_limit;

		gs::backup::SStageSolo& stage =
			gs::backup::SStage::CreateInstance()[m_board];

		if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
			m_select = (u32)((m_select + 7) % 8);
		}
		if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
			m_select = (u32)((m_select + 1) % 8);
		}

		u32 record;
		bool flag;
		switch (m_select) {
		case 0:
			record = stage.GetFastTime(false);
			if (aoTestNetUtilEditValue(record, 0, max_time, 60)) {
				stage.SetFastTime(record, false);
			}
			break;
		case 1:
			record = stage.GetHighScore(false);
			if (aoTestNetUtilEditValue(record, 0, max_score, 100)) {
				stage.SetHighScore(record, false);
			}
			break;
		case 2:
			record = stage.GetFastTime(true);
			if (aoTestNetUtilEditValue(record, 0, max_time, 60)) {
				stage.SetFastTime(record, true);
			}
			break;
		case 3:
			record = stage.GetHighScore(true);
			if (aoTestNetUtilEditValue(record, 0, max_score, 100)) {
				stage.SetHighScore(record, true);
			}
			break;
		case 4:
			flag = stage.IsFastTimeUploaded(false);
			if (aoTestNetUitlEditFlag(flag)) {
				stage.SetFastTimeUploaded(false, flag);
				if (flag) {
					stage.SetTimeUploadedOnce(true);
				}
			}
			break;
		case 5:
			flag = stage.IsHighScoreUploaded(false);
			if (aoTestNetUitlEditFlag(flag)) {
				stage.SetHighScoreUploaded(false, flag);
				if (flag) {
					stage.SetScoreUploadedOnce(true);
				}
			}
			break;
		case 6:
			flag = stage.IsFastTimeUploaded(true);
			if (aoTestNetUitlEditFlag(flag)) {
				stage.SetFastTimeUploaded(true, flag);
				if (flag) {
					stage.SetTimeUploadedOnce(true);
				}
			}
			break;
		case 7:
			flag = stage.IsHighScoreUploaded(true);
			if (aoTestNetUitlEditFlag(flag)) {
				stage.SetHighScoreUploaded(true, flag);
				if (flag) {
					stage.SetScoreUploadedOnce(true);
				}
			}
			break;
		default:
			m_select = 0;
			break;
		}

		aoTestNetUtilPrintColor(y, m_select == 0, stage.IsFastTimeEnable(false));
		AkUtilFrame60ToTime(stage.GetFastTime(false), &min, &sec, &msec);
		amPrintf(5, y++, "TIME              : %d\'%02d\"%02d", min, sec, msec);
		aoTestNetUtilPrintColor(y, m_select == 1, stage.IsHighScoreEnable(false));
		amPrintf(5, y++, "SCORE             : %d", stage.GetHighScore(false));
		aoTestNetUtilPrintColor(y, m_select == 2, stage.IsFastTimeEnable(true));
		AkUtilFrame60ToTime(stage.GetFastTime(true), &min, &sec, &msec);
		amPrintf(5, y++, "TIME(S)           : %d\'%02d\"%02d", min, sec, msec);
		aoTestNetUtilPrintColor(y, m_select == 3, stage.IsHighScoreEnable(true));
		amPrintf(5, y++, "SCORE(S)          : %d", stage.GetHighScore(true));
		aoTestNetUtilPrintColor(y, m_select == 4, TRUE);
		amPrintf(5, y++, "TIME UPLOADED     : %s", stage.IsFastTimeUploaded(false) ? "O" : "X");
		aoTestNetUtilPrintColor(y, m_select == 5, TRUE);
		amPrintf(5, y++, "SCORE UPLOADED    : %s", stage.IsHighScoreUploaded(false) ? "O" : "X");
		aoTestNetUtilPrintColor(y, m_select == 6, TRUE);
		amPrintf(5, y++, "TIME UPLOADED(S)  : %s", stage.IsFastTimeUploaded(true) ? "O" : "X");
		aoTestNetUtilPrintColor(y, m_select == 7, TRUE);
		amPrintf(5, y++, "SCORE UPLOADED(S) : %s", stage.IsHighScoreUploaded(true) ? "O" : "X");
	}
	else {

		const u32 max_time = gs::backup::SSpecialSolo::c_fast_time_max_limit;
		const u32 max_score = gs::backup::SSpecialSolo::c_high_score_max_limit;

		// スペシャルステージ
		gs::backup::SSpecialSolo& stage =
			gs::backup::SSpecial::CreateInstance()
			[m_board - DMD_RANK_SYS_BOARD_SS_S];

		if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
			m_select = (u32)((m_select + 3) % 4);
		}
		if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
			m_select = (u32)((m_select + 1) % 4);
		}

		u32 record;
		bool flag;
		switch (m_select) {
		case 0:
			record = stage.GetFastTime();
			if (aoTestNetUtilEditValue(record, 0, max_time, 60)) {
				stage.SetFastTime(record);
			}
			break;
		case 1:
			record = stage.GetHighScore();
			if (aoTestNetUtilEditValue(record, 0, max_score, 100)) {
				stage.SetHighScore(record);
			}
			break;
		case 2:
			flag = stage.IsFastTimeUploaded();
			if (aoTestNetUitlEditFlag(flag)) {
				stage.SetFastTimeUploaded(flag);
				if (flag) {
					stage.SetTimeUploadedOnce(true);
				}
			}
			break;
		case 3:
			flag = stage.IsHighScoreUploaded();
			if (aoTestNetUitlEditFlag(flag)) {
				stage.SetHighScoreUploaded(flag);
				if (flag) {
					stage.SetScoreUploadedOnce(true);
				}
			}
			break;
		default:
			m_select = 0;
			break;
		}

		aoTestNetUtilPrintColor(y, m_select == 0, stage.IsFastTimeEnable());
		AkUtilFrame60ToTime(stage.GetFastTime(), &min, &sec, &msec);
		amPrintf(5, y++, "TIME              : %d\'%02d\"%02d", min, sec, msec);
		aoTestNetUtilPrintColor(y, m_select == 1, stage.IsHighScoreEnable());
		amPrintf(5, y++, "SCORE             : %d", stage.GetHighScore());
		aoTestNetUtilPrintColor(y, m_select == 2, TRUE);
		amPrintf(5, y++, "TIME UPLOADED     : %s", stage.IsFastTimeUploaded() ? "O" : "X");
		aoTestNetUtilPrintColor(y, m_select == 3, TRUE);
		amPrintf(5, y++, "SCORE UPLOADED    : %s", stage.IsHighScoreUploaded() ? "O" : "X");
	}
	amPrintColor(0xffffffff);

	// 開始判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
		// ボード設定
		DmRankSysNoticeBoard(
			(DME_RANK_SYS_BOARD)m_board,
			(DME_RANK_SYS_RANK)m_rank,
			(DME_RANK_SYS_SS)m_ss,
			m_is_friend, m_is_upload);

		// 送信中へ遷移
		SetOwnProc(&CMain::ProcSending);
	}

	// 終了判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// ボード選択へ遷移
		SetOwnProc(&CMain::ProcSelectBoard);
	}
}

// ===========================================================================
//! 送信中
// ===========================================================================
void CMain::ProcSending()
{
	// 画面表示
	amPrintf(4, 4, "NOW UPLOADING... %d", m_count++);
	if (DmRankSysIsCancelable()) {
		amPrintf(4, 5, "%c:CANCEL", GsEnvDebugGetCancelKeyChar());
	}

	// エラー判定
	if (DmRankSysIsError()) {
		// 切断開始
		DmRankSysEnd();

		// 切断中(エラー)へ遷移
		SetOwnProc(&CMain::ProcErrorDisconnecting);
		return;
	}

	// アップロード終了判定
	if (!DmRankSysIsUpLoading()) {

		// ランク選択へ遷移
		SetOwnProc(&CMain::ProcSelectRank);
	}

	// キャンセル判定
	else if (DmRankSysIsCancelable() &&
		(AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0))
	{
		// キャンセル
		DmRankSysNoticeCancel();

		// ボード選択へ戻る
		SetOwnProc(&CMain::ProcSelectBoard);
	}
}

// ===========================================================================
//! ランク選択
// ===========================================================================
void CMain::ProcSelectRank()
{
	// 画面表示
	amPrintf(4, 4, "SELECT RANK. %d", m_count++);
	amPrintf(4, 5, "%c:RECV", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:BACK", GsEnvDebugGetCancelKeyChar());

	// エラー判定
	if (DmRankSysIsError()) {
		// 切断開始
		DmRankSysEnd();

		// 切断中(エラー)へ遷移
		SetOwnProc(&CMain::ProcErrorDisconnecting);
		return;
	}

	// 表示ランク設定
	if (GetCount() == 0) {
		m_top_rank = 0;
		m_is_near = FALSE;
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
		m_top_rank += 1;
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
		if (m_top_rank > 0) {
			m_top_rank -= 1;
		}
	}
	if ((AoPadSomeoneMRepeat(GSD_KEY_LEFT) >= 0) ||
		(AoPadSomeoneMRepeat(GSD_KEY_RIGHT) >= 0))
	{
		m_is_near = !m_is_near;
	}
	if (m_is_near) {
		amPrint(4, 8, "RANK:NEAR");
	}
	else {
		amPrintf(4, 8, "RANK:%d", m_top_rank + 1);
	}

	// 決定判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {

		// ダウンロード開始
		if (m_is_near) {
			DmRankSysNoticeShowRankNo(DMD_RANK_SYS_OWN_NEAR);
		}
		else {
			DmRankSysNoticeShowRankNo(m_top_rank);
		}

		// 受信中へ遷移
		SetOwnProc(&CMain::ProcRecving);
	}

	// キャンセル判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// ボード選択へ戻る
		SetOwnProc(&CMain::ProcSelectBoard);
	}
}

// ===========================================================================
//! 受信中
// ===========================================================================
void CMain::ProcRecving()
{
	// 画面表示
	amPrintf(4, 4, "NOW DOWNLOADING... %d", m_count++);
	if (DmRankSysIsCancelable()) {
		amPrintf(4, 5, "%c:CANCEL", GsEnvDebugGetCancelKeyChar());
	}

	// エラー判定
	if (DmRankSysIsError()) {
		// 切断開始
		DmRankSysEnd();

		// 切断中(エラー)へ遷移
		SetOwnProc(&CMain::ProcErrorDisconnecting);
		return;
	}

	// ダウンロード終了判定
	if (!DmRankSysIsDownloading()) {

		// 受信成功へ遷移
		SetOwnProc(&CMain::ProcRecvSuccess);
	}

	// キャンセル判定
	else if (DmRankSysIsCancelable() &&
		(AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0))
	{
		// キャンセル
		DmRankSysNoticeCancel();

		// ボード選択へ戻る
		SetOwnProc(&CMain::ProcSelectBoard);
	}
}

// ===========================================================================
//! 受信成功
// ===========================================================================
void CMain::ProcRecvSuccess()
{
	// 画面表示
	amPrintf(4, 4, "DOWNLOAD SUCCESS. %d", m_count++);
#if _XBOX
	if (DmRankSysGetShowNum() > 0) {
		amPrintf(4, 5, "%c:GAMER CARD UI", GsEnvDebugGetDecideKeyChar());
		amPrintf(4, 6, "%c:BACK", GsEnvDebugGetCancelKeyChar());
	}
	else {
		amPrintf(4, 5, "%c:BACK", GsEnvDebugGetCancelKeyChar());
	}
#else
	amPrintf(4, 5, "%c:BACK", GsEnvDebugGetCancelKeyChar());
#endif // _XBOX

	// エラー判定
	if (DmRankSysIsError()) {
		// 切断開始
		DmRankSysEnd();

		// 切断中(エラー)へ遷移
		SetOwnProc(&CMain::ProcErrorDisconnecting);
		return;
	}

	if (GetCount() == 0) {
		m_select = 0;
	}

	// 選択
	if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
		if (m_select > 0) {
			m_select -= 1;
		}
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
		if ((s32)m_select < ((s32)DmRankSysGetShowNum() - 1)) {
			m_select += 1;
		}
	}

	// ランキング表示
	amPrintColor(0xffffff00);
	if (m_rank == (u32)DMD_RANK_SYS_RANK_TIME) {
		amPrint(4, 7, "SRANK    RANK               NAME      TIME   RGN   SS");
		for (u32 i = 0; i < DmRankSysGetShowNum(); ++i) {
			if (DmRankSysIsShowOwn(i)) {
				amPrintColor(0xff0000ff);
			}
			else {
				amPrintColor(0xffffffff);
			}
			if (m_select == i) {
				amPrint(4, (s32)(8 + i), ">");
			}
			u16 min, sec, msec;
			AkUtilFrame60ToTime(DmRankSysGetShowScore(i), &min, &sec, &msec);
			amPrintf(
				5, (s32)(8 + i),
				"%5d : %5d : %16s : %d\'%02d\"%02d :   %d :  %d",
				DmRankSysGetRealRankNo(i) + 1,
				DmRankSysGetShowRankNo(i) + 1,
				DmRankSysGetShowName(i),
				min, sec, msec,
				DmRankSysGetShowRegion(i),
				DmRankSysGetShowSs(i));
		}
	}
	else {
		amPrint(4, 7, "SRANK    RANK               NAME     SCORE   RGN   SS");
		for (u32 i = 0; i < DmRankSysGetShowNum(); ++i) {
			if (DmRankSysIsShowOwn(i)) {
				amPrintColor(0xff0000ff);
			}
			else {
				amPrintColor(0xffffffff);
			}
			if (m_select == i) {
				amPrint(4, (s32)(8 + i), ">");
			}
			amPrintf(
				5, (s32)(8 + i),
				"%5d : %5d : %16s : %07d :   %d :  %d",
				DmRankSysGetRealRankNo(i) + 1,
				DmRankSysGetShowRankNo(i) + 1,
				DmRankSysGetShowName(i),
				DmRankSysGetShowScore(i),
				DmRankSysGetShowRegion(i),
				DmRankSysGetShowSs(i));
		}
	}
	amPrintColor(0xffffffff);

#if _XBOX

	if (DmRankSysCamerCardShowIsFinished()) {
		// ゲーマーカードUI表示
		if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
			if (DmRankSysGetShowNum() > 0) {
				DmRankSysGamerCardShow(m_select);
			}
		}
		// キャンセル判定
		else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
			// ランク選択へ戻る
			SetOwnProc(&CMain::ProcSelectRank);
		}
	}

#else

	// キャンセル判定
	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {

		// ランク選択へ戻る
		SetOwnProc(&CMain::ProcSelectRank);
	}
#endif // _XBOX
}

// ===========================================================================
//! 切断中
// ===========================================================================
void CMain::ProcDisconnecting()
{
	// 画面表示
	amPrintf(4, 4, "NOW DISCONNECTING... %d", m_count++);

	// エラー判定
	if (DmRankSysIsError()) {
		// 切断中(エラー)へ遷移
		SetOwnProc(&CMain::ProcErrorDisconnecting);
		return;
	}

	// 切断完了判定
	if (DmRankSysIsFinished()) {
		// 切断済みへ遷移
		SetOwnProc(&CMain::ProcDisconnected);
	}
}

// ===========================================================================
//! 切断済み
// ===========================================================================
void CMain::ProcDisconnected()
{
	// 画面表示
	amPrintf(4, 4, "DISCONNECTED. %d", m_count++);
	amPrintf(4, 5, "%c:BACK", GsEnvDebugGetCancelKeyChar());

	// 終了判定
	if (AoPadSomeoneMStand(GSD_KEY_CANCEL) >= 0) {
		// 最初へ遷移
		SetOwnProc(&CMain::ProcReady);
	}
}

// ===========================================================================
//! 切断中(エラー)
// ===========================================================================
void CMain::ProcErrorDisconnecting()
{
	// 画面表示
	amPrintf(
		4, 4, "NOW DISCONNECTING... (ERROR:%d) %d",
		DmRankSysIsError(), m_count++);

	// 切断完了判定
	if (DmRankSysIsFinished()) {
		// 切断済みへ遷移
		SetOwnProc(&CMain::ProcErrorDisconnected);
	}
}

// ===========================================================================
//! 切断済み(エラー)
// ===========================================================================
void CMain::ProcErrorDisconnected()
{
	// 画面表示
	amPrintf(
		4, 4, "NOW DISCONNECTED... (ERROR:%d) %d",
		DmRankSysIsError(), m_count++);
	amPrintf(4, 5, "%c:BACK", GsEnvDebugGetCancelKeyChar());

	// 終了判定
	if (AoPadSomeoneMStand(GSD_KEY_CANCEL) >= 0) {
		// 最初へ遷移
		SetOwnProc(&CMain::ProcReady);
	}
}

} // namespace net
} // namespace test
} // namespace ao

#endif // defined (MTD_DEBUG)

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
