// ===========================================================================
/*!
	@file	dmBuyScreen.cpp
	@brief	製品版（完全版）購入画面モジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#if _PC || _XBOX || _PS3

#include "dmBuyScreen.h"
#include "gsEnvironment.h"
#include "gsTrial.h"
#include "ao.h"
#include "izFade.h"
#include "dmSave.h"

#include "../common/ace/d_buy_screen.hma"
#if _PS3
#include "../common/ace/D_CMN_BTN.HMA"
#endif // _PS3

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

namespace dm {
namespace buyscreen {

// ===========================================================================
//! メインクラス
// ===========================================================================
class CMain :
	public ao::CProc<CMain>, public ao::CTask<CMain>, public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CMain(DMS_BUY_SCR_WORK& work, BOOL is_ui_show, BOOL is_save);

protected:

	//! デストラクタ
	virtual ~CMain();

	//! タスクプロシージャ
	void TaskProcMain();

	//! 準備
	void ProcReady();

	//! フェードイン
	void ProcFadeIn();

	//! ユーザ選択待ち
	void ProcWaitInput();

	//! 購入
	void ProcStore();

	//! 購入済みチェック
	void ProcBuyCheck();

	//! フェードアウト
	void ProcFadeOut();

#if !_PS3
	//! セーブ
	void ProcSave();
#endif // !_PS3

	//! アクション構築
	void ActionBuild();

	//! アクション解放
	void ActionRelease();

	//! 画面表示
	void ShowUI();

	//! 代入演算子(警告回避)
	CMain operator = (CMain&) {
		amAssert(0);
		return *this;
	}

	//! ワーク
	DMS_BUY_SCR_WORK& m_work;

	//! UI表示フラグ
	BOOL m_is_ui_show;

#if !_PS3
	//! 購入後セーブ実行フラグ
	BOOL m_is_save;
#endif // !_PS3

	//! 背景アクション
	AOS_ACTION* m_act_bg;

#if _PS3

	//! 決定ボタンアクション
	AOS_ACTION* m_act_o;

	//! キャンセルボタンアクション
	AOS_ACTION* m_act_x;
#endif // _PS3
};

// ===========================================================================
//! ファイル読み込みクラス
// ===========================================================================
class CLoad : public ao::CTask<CLoad>, public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CLoad(DMS_BUY_SCR_WORK& work);

protected:

	//! デストラクタ
	virtual ~CLoad();

	//! 読み込み待ち
	void TaskProcWait();

#if _PS3
	//! ボタン読み込み待ち
	void TaskProcWaitBtn();
#endif // _PS3

	//! 代入演算子(警告回避)
	CLoad operator = (CLoad&) {
		amAssert(0);
		return *this;
	}

	//! ワーク
	DMS_BUY_SCR_WORK& m_work;

	//! ファイル読み込み用
	AMS_FS* m_fs;
};

// ===========================================================================
//! 構築クラス
// ===========================================================================
class CBuild : public ao::CTask<CBuild>, public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CBuild(DMS_BUY_SCR_WORK& work);

protected:

	//! デストラクタ
	virtual ~CBuild();

	//! 構築待ち
	void TaskProcWait();

	//! 代入演算子(警告回避)
	CBuild operator = (CBuild&) {
		amAssert(0);
		return *this;
	}

	//! ワーク
	DMS_BUY_SCR_WORK& m_work;
};

// ===========================================================================
//! 解放クラス
// ===========================================================================
class CFlush : public ao::CTask<CFlush>, public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CFlush(DMS_BUY_SCR_WORK& work);

protected:

	//! デストラクタ
	virtual ~CFlush();

	//! 解放待ち
	void TaskProcWait();

	//! 代入演算子(警告回避)
	CFlush operator = (CFlush&) {
		amAssert(0);
		return *this;
	}

	//! ワーク
	DMS_BUY_SCR_WORK& m_work;
};

} // namespace buyscreen
} // namespace dm

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//! AMBファイルパス
// ===========================================================================
static const char* g_dm_buy_screen_amb_path =
	GSS_BASE_PATH"DEMO/BUY_SCREEN/D_BUY_SCREEN_";

// ===========================================================================
//! AMB言語別ファイル名配列
// ===========================================================================
static const char* g_dm_buy_screen_amb_lng_tbl[GSD_LANGUAGE_NUM] = {
	"JP", "US", "FR", "IT", "GE", "SP"
};

// ===========================================================================
//! AMB拡張子
// ===========================================================================
static const char* g_dm_buy_screen_amb_ext = ".AMB";

// ===========================================================================
//! ボタンAMBファイルパス
// ===========================================================================
static const char* g_dm_buy_screen_btn_amb_path =
	GSS_BASE_PATH"DEMO/CMN/D_CMN_BTN.AMB";

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! 製品版（完全版）購入画面 ワーク初期化
// ===========================================================================
void DmBuyScreenInit(DMS_BUY_SCR_WORK* work)
{
	amZeroMemory(work, sizeof(DMS_BUY_SCR_WORK));
}

// ===========================================================================
//! 製品版（完全版）購入画面 ファイル読み込み開始
// ===========================================================================
void DmBuyScreenLoadStart(DMS_BUY_SCR_WORK* work)
{
	if (!DmBuyScreenLoadIsFinished(work)) {
		amAssert(work->tcb == NULL);

		// ファイル読み込みクラス作成
		new dm::buyscreen::CLoad(*work);
	}
}

// ===========================================================================
//! 製品版（完全版）購入画面 ファイル読み込み完了判定
// ===========================================================================
BOOL DmBuyScreenLoadIsFinished(const DMS_BUY_SCR_WORK* work)
{
	amAssert(work);
#if _PS3
	if (work->file && work->btn_file) {
		return TRUE;
	}
#else
	if (work->file) {
		return TRUE;
	}
#endif // _PS3
	return FALSE;
}

// ===========================================================================
//! 製品版（完全版）購入画面 テクスチャ構築開始
// ===========================================================================
void DmBuyScreenBuildStart(DMS_BUY_SCR_WORK* work)
{
	amAssert(DmBuyScreenLoadIsFinished(work));

	if (!DmBuyScreenBuildIsFinished(work)) {
		amAssert(work->tcb == NULL);

		// テクスチャ構築クラス作成
		new dm::buyscreen::CBuild(*work);
	}
}

// ===========================================================================
//! 製品版（完全版）購入画面 テクスチャ構築完了判定
// ===========================================================================
BOOL DmBuyScreenBuildIsFinished(const DMS_BUY_SCR_WORK* work)
{
	amAssert(work);
#if _PS3
	if (work->builded && work->btn_builded) {
		return TRUE;
	}
#else
	if (work->builded) {
		return TRUE;
	}
#endif // _PS3
	return FALSE;
}

// ===========================================================================
//! 製品版（完全版）購入画面 開始
// ===========================================================================
void DmBuyScreenStart(DMS_BUY_SCR_WORK* work, BOOL is_ui_show, BOOL is_save)
{
	amAssert(!is_ui_show || DmBuyScreenBuildIsFinished(work));

	if (DmBuyScreenIsFinished(work)) {
		amAssert(work->tcb == NULL);

		// 結果初期化
		work->result = DMD_BUY_SCR_RESULT_NONE;

		// メインクラス作成
		new dm::buyscreen::CMain(*work, is_ui_show, is_save);
	}
}

// ===========================================================================
//! 製品版（完全版）購入画面 完了判定
// ===========================================================================
BOOL DmBuyScreenIsFinished(const DMS_BUY_SCR_WORK* work)
{
	amAssert(work);

	if (!work->tcb) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! 製品版（完全版）購入画面 結果取得
// ===========================================================================
DME_BUY_SCR_RESULT DmBuyScreenGetResult(const DMS_BUY_SCR_WORK* work)
{
	return work->result;
}

// ===========================================================================
//! 製品版（完全版）購入画面 テクスチャ解放開始
// ===========================================================================
void DmBuyScreenFlushStart(DMS_BUY_SCR_WORK* work)
{
	amAssert(DmBuyScreenIsFinished(work));

	if (!DmBuyScreenFlushIsFinished(work)) {
		amAssert(work->tcb == NULL);

		// テクスチャ解放クラス作成
		new dm::buyscreen::CFlush(*work);
	}
}

// ===========================================================================
//! 製品版（完全版）購入画面 テクスチャ解放完了判定
// ===========================================================================
BOOL DmBuyScreenFlushIsFinished(const DMS_BUY_SCR_WORK* work)
{
	amAssert(work);
#if _PS3
	if (!work->builded && !work->btn_builded) {
		return TRUE;
	}
#else
	if (!work->builded) {
		return TRUE;
	}
#endif // _PS3
	return FALSE;
}

// ===========================================================================
//! 製品版（完全版）購入画面 ファイル解放
// ===========================================================================
void DmBuyScreenRelease(DMS_BUY_SCR_WORK* work)
{
	amAssert(DmBuyScreenFlushIsFinished(work));

	if (work->file) {
		amMemFree(work->file);
		work->file = NULL;
	}
#if _PS3
	if (work->btn_file) {
		amMemFree(work->btn_file);
		work->btn_file = NULL;
	}
#endif // _PS3
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace dm {
namespace buyscreen {

// ***************************************************************************
// メインクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CMain::CMain(DMS_BUY_SCR_WORK& work, BOOL is_ui_show, BOOL is_save)
	 : m_work(work), m_is_ui_show(is_ui_show)
{
	// メンバ初期化
	m_act_bg = NULL;
#if _PS3
	m_act_o = NULL;
	m_act_x = NULL;
#endif // _PS3
#if !_PS3
	m_is_save = is_save;
#endif // !_PS3

	// タスク作成
	MakeTask(0, "dmBuyScreen::Execute");
	m_work.tcb = GetTcb(0);
	SetTaskProc(0, &CMain::TaskProcMain);

	// プロシージャ設定
	SetProc(0, &CMain::ProcReady);

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CMain::~CMain()
{
	ActionRelease();
	m_work.tcb = NULL;
}

// ===========================================================================
//! タスクプロシージャ
// ===========================================================================
void CMain::TaskProcMain()
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

// ===========================================================================
//! 準備
// ===========================================================================
void CMain::ProcReady()
{
	// UIの有無で分岐
	if (m_is_ui_show) {
		// アクション構築
		ActionBuild();

		// フェードインへ遷移
		SetOwnProc(&CMain::ProcFadeIn);
	}
	else {
		// 購入へ遷移
		SetOwnProc(&CMain::ProcStore);
	}
}

// ===========================================================================
//! フェードイン
// ===========================================================================
void CMain::ProcFadeIn()
{
	// 表示
	ShowUI();

	// フェード開始
	if (GetCount() == 0) {
		IzFadeInitEasy(
			IZE_FADE_SET_TYPE_TAKEOEVER, IZE_FADE_TYPE_BLACK_FADEIN, 16.0f);
	}

	// フェード完了判定
	if (IzFadeIsEnd()) {

		// ユーザ選択待ちへ遷移
		SetOwnProc(&CMain::ProcWaitInput);
	}
}

// ===========================================================================
//! ユーザ選択待ち
// ===========================================================================
void CMain::ProcWaitInput()
{
	// 表示
	ShowUI();

	// 入力判定
	BOOL is_buy = FALSE;
	BOOL is_cancel = FALSE;

#if _PS3
	if (AoPadStand() & GSD_KEY_DECIDE) {
		// 購入開始
		is_buy = TRUE;
	}
	else if (AoPadStand() & GSD_KEY_CANCEL) {
		// ゲームに戻る
		is_cancel = TRUE;
	}
#else
	BOOL is_exit = FALSE;
	if (AoPadStand() & GSD_KEY_DECIDE) {
		// ゲームを終了
		is_exit = TRUE;
	}
	else if (AoPadStand() & GSD_KEY_CANCEL) {
		// ゲームに戻る
		is_cancel = TRUE;
	}
	else if (AoPadStand() & KEY_R_LEFT) {
		// 購入開始
		is_buy = TRUE;
	}
#endif

	// キャンセル判定
	if (is_cancel || !AoAccountIsCurrentEnable()) {

		// 結果設定
		m_work.result = DMD_BUY_SCR_RESULT_CANCEL;

		// フェードアウトへ遷移
		SetOwnProc(&CMain::ProcFadeOut);
	}

	// 購入開始判定
	else if (is_buy) {

		// 購入へ遷移
		SetOwnProc(&CMain::ProcStore);
	}

#if !_PS3
	// ゲーム終了判定
	else if (is_exit) {

		// 結果設定
		m_work.result = DMD_BUY_SCR_RESULT_BACK;

		// フェードアウトへ遷移
		SetOwnProc(&CMain::ProcFadeOut);
	}
#endif // !_PS3
}

// ===========================================================================
//! 購入
// ===========================================================================
void CMain::ProcStore()
{
	// 表示
	ShowUI();

	// 購入開始
	if (GetCount() == 0) {
		GsTrialStoreStart();
	}

	// 購入終了待ち
	if (GsTrialStoreIsFinished()) {

#if !defined(_DLC)
		GsTrialDebugSetTrial(FALSE);
#endif // !defined(_DLC)

		// 購入済みチェックへ遷移
		SetOwnProc(&CMain::ProcBuyCheck);
	}
}

// ===========================================================================
//! 購入済みチェック
// ===========================================================================
void CMain::ProcBuyCheck()
{
	// 表示
	ShowUI();

	// 購入済チェック開始
	if (GetCount() == 0) {
		GsTrialCheckStart();
	}

	// 購入済みチェック完了判定
	if (GsTrialCheckIsFinished()) {

		// 結果設定
		if (!GsTrialIsTrial()) {
			// 購入した
			m_work.result = DMD_BUY_SCR_RESULT_BUY;
		}
		else {
			// キャンセル
			m_work.result = DMD_BUY_SCR_RESULT_CANCEL;
		}

		// UIの有無で分岐
		if (m_is_ui_show) {

			if (GsTrialIsTrial()) {
				// 未購入なら入力待ちへ戻る
				SetOwnProc(&CMain::ProcWaitInput);
			}
			else {
				// フェードアウトへ遷移
				SetOwnProc(&CMain::ProcFadeOut);
			}
		}
		else {

#if !_PS3
			// 購入したならセーブへ遷移
			if (m_work.result == DMD_BUY_SCR_RESULT_BUY) {
				SetOwnProc(&CMain::ProcSave);
			}
			else
#endif // !_PS3
			{
				// 終了
				SetOwnProcNone();
			}
		}
	}
}

// ===========================================================================
//! フェードアウト
// ===========================================================================
void CMain::ProcFadeOut()
{
	// 表示
	ShowUI();

	// フェード開始
	if (GetCount() == 0) {
		IzFadeInitEasy(
			IZE_FADE_SET_TYPE_TAKEOEVER, IZE_FADE_TYPE_BLACK_FADEOUT, 16.0f);
	}

	// フェード完了判定
	if (IzFadeIsEnd()) {

#if !_PS3
		// 購入したならセーブへ遷移
		if (m_work.result == DMD_BUY_SCR_RESULT_BUY) {
			SetOwnProc(&CMain::ProcSave);
		}
		else
#endif // !_PS3
		{
			// 終了
			SetOwnProcNone();
		}
	}
}

#if !_PS3
// ===========================================================================
//! セーブ
// ===========================================================================
void CMain::ProcSave()
{
	if (m_is_save) {
		// セーブ開始
		if (GetCount() == 0) {
			DmSaveStart(1 << DME_SAVE_WIN_TRIAL_OUT_SAVE, FALSE, TRUE);
		}

		// セーブ終了判定
		if (DmSaveIsExit()) {
			// 終了
			SetOwnProcNone();
		}
	}
	else {
		// 終了
		SetOwnProcNone();
	}
}
#endif // !_PS3

// ===========================================================================
//! アクション構築
// ===========================================================================
void CMain::ActionBuild()
{
	if (m_is_ui_show) {

		// AMA取得
		const void* ama = amBindGet((AMS_AMB_HEADER*)m_work.file, 0);

		// アクション構築
		AoActSetTexture(AoTexGetTexList(&m_work.tex));
		m_act_bg = AoActCreateNode(ama, IDA_D_BUY_SCREEN_ROOT_NODE_BG);

#if _PS3
		// ボタンAMA取得
		const void* btn_ama = amBindGet((AMS_AMB_HEADER*)m_work.btn_file, 0);

		// ボタンアクション構築
		AoActSetTexture(AoTexGetTexList(&m_work.btn_tex));
		m_act_o = AoActCreate(btn_ama, IDA_D_CMN_BTN_ACT_BACK_BTN);
		m_act_x = AoActCreate(btn_ama, IDA_D_CMN_BTN_ACT_BACK_BTN);
		if (GeEnvGetDecideKey() == GSD_DECIDE_KEY_O) {
			AoActSetFrame(m_act_o, 1.0f);
			AoActSetFrame(m_act_x, 0.0f);
		}
		else {
			AoActSetFrame(m_act_o, 0.0f);
			AoActSetFrame(m_act_x, 1.0f);
		}
#endif // _PS3
	}
}

// ===========================================================================
//! アクション解放
// ===========================================================================
void CMain::ActionRelease()
{
	if (m_act_bg) {
		AoActDelete(m_act_bg);
		m_act_bg = NULL;
	}
#if _PS3
	if (m_act_o) {
		AoActDelete(m_act_o);
		m_act_o = NULL;
	}
	if (m_act_x) {
		AoActDelete(m_act_x);
		m_act_x = NULL;
	}
#endif // _PS3
}

// ===========================================================================
//! 画面表示
// ===========================================================================
void CMain::ShowUI()
{
	if (m_is_ui_show && m_act_bg) {

		// テクスチャ設定
		AoActSetTexture(AoTexGetTexList(&m_work.tex));

		// アクション更新
		AoActUpdate(m_act_bg);

		// アクション描画
		AoActDraw(m_act_bg);

#if _PS3

		// ボタンX位置
		const s32 btn_pos_x = 620;

		// 決定ボタンY位置
		const s32 btn_pos_y_o = 580;

		// キャンセルボタンY位置
		const s32 btn_pos_y_x = 636;

		// アキュムレート退避
		AoActAcmPush();

		// ボタンテクスチャ設定
		AoActSetTexture(AoTexGetTexList(&m_work.btn_tex));

		// アキュムレート初期化
		AoActAcmInit();

		// 決定ボタン位置設定
		AoActAcmApplyTrans((f32)btn_pos_x, (f32)btn_pos_y_o, 0.0f);

		// 決定ボタン更新
		AoActUpdate(m_act_o, 0.0f);

		// 決定ボタン描画
		AoActDraw(m_act_o);

		// アキュムレート初期化
		AoActAcmInit();

		// キャンセルボタン位置設定
		AoActAcmApplyTrans((f32)btn_pos_x, (f32)btn_pos_y_x, 0.0f);

		// キャンセルボタン更新
		AoActUpdate(m_act_x, 0.0f);

		// キャンセルボタン描画
		AoActDraw(m_act_x);

		// アキュムレート復帰
		AoActAcmPop();

#endif // _PS3
	}
}


// ***************************************************************************
// ファイル読み込みクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CLoad::CLoad(DMS_BUY_SCR_WORK& work) : m_work(work)
{
	// AMBファイルパス作成
	char path[64];
	sprintf(
		path, "%s%s%s",
		g_dm_buy_screen_amb_path,
		g_dm_buy_screen_amb_lng_tbl[GsEnvGetLanguage()],
		g_dm_buy_screen_amb_ext);

	// AMBファイル読み込みリクエスト発行
	m_fs = amFsReadBackground(path);

	// タスク作成
	MakeTask(0, "dmBuyScreen::Load");
	m_work.tcb = GetTcb(0);
	SetTaskProc(0, &CLoad::TaskProcWait);

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CLoad::~CLoad()
{
	if (m_fs) {
		amFsClearRequest(m_fs);
		m_fs = NULL;
	}
	m_work.tcb = NULL;
}

// ===========================================================================
//! 読み込み待ち
// ===========================================================================
void CLoad::TaskProcWait()
{
	// ファイル読み込み完了待ち
	if (amFsIsComplete(m_fs)) {

		// ファイル取得
		m_work.file = m_fs->buf;
		m_fs->buf = NULL;
		amFsClearRequest(m_fs);
		m_fs = NULL;

		// アドレス変換
		amConvertAddress(m_work.file);

#if _PS3
		// ボタンAMBファイルパス作成
		char path[64];
		strcpy(path, g_dm_buy_screen_btn_amb_path);

		// ボタンAMBファイル読み込みリクエスト発行
		m_fs = amFsReadBackground(path);

		// ボタン読み込み待ちへ遷移
		SetOwnTaskProc(&CLoad::TaskProcWaitBtn);
#else
		// 終了
		delete this;
#endif // _PS3
	}
}

#if _PS3
// ===========================================================================
//! ボタン読み込み待ち
// ===========================================================================
void CLoad::TaskProcWaitBtn()
{
	// ファイル読み込み完了待ち
	if (amFsIsComplete(m_fs)) {

		// ファイル取得
		m_work.btn_file = m_fs->buf;
		m_fs->buf = NULL;
		amFsClearRequest(m_fs);
		m_fs = NULL;

		// アドレス変換
		amConvertAddress(m_work.btn_file);

		// 終了
		delete this;
	}
}
#endif // _PS3


// ***************************************************************************
// 構築クラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CBuild::CBuild(DMS_BUY_SCR_WORK& work) : m_work(work)
{
	// テクスチャ構築開始
	AoTexBuild(&m_work.tex, amBindGet((AMS_AMB_HEADER*)m_work.file, 1));
	AoTexLoad(&m_work.tex);
#if _PS3
	AoTexBuild(
		&m_work.btn_tex, amBindGet((AMS_AMB_HEADER*)m_work.btn_file, 1));
	AoTexLoad(&m_work.btn_tex);
#endif // _PS3

	// タスク作成
	MakeTask(0, "dmBuyScreen::Build");
	m_work.tcb = GetTcb(0);
	SetTaskProc(0, &CBuild::TaskProcWait);

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CBuild::~CBuild()
{
	m_work.tcb = NULL;
}

// ===========================================================================
//! 構築待ち
// ===========================================================================
void CBuild::TaskProcWait()
{
	// テクスチャ構築待ち
	if (!AoTexIsLoaded(&m_work.tex)) {
		return;
	}
#if _PS3
	if (!AoTexIsLoaded(&m_work.btn_tex)) {
		return;
	}
#endif // _PS3

	// 構築済み設定
	m_work.builded = TRUE;
#if _PS3
	m_work.btn_builded = TRUE;
#endif // _PS3

	// 終了
	delete this;
}


// ***************************************************************************
// 解放クラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CFlush::CFlush(DMS_BUY_SCR_WORK& work) : m_work(work)
{
	// テクスチャ解放開始
	if (m_work.builded) {
		AoTexRelease(&m_work.tex);
	}
#if _PS3
	if (m_work.btn_builded) {
		AoTexRelease(&m_work.btn_tex);
	}
#endif // _PS3

	// タスク作成
	MakeTask(0, "dmBuyScreen::Flush");
	m_work.tcb = GetTcb(0);
	SetTaskProc(0, &CFlush::TaskProcWait);

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CFlush::~CFlush()
{
	m_work.tcb = NULL;
}

// ===========================================================================
//! 解放待ち
// ===========================================================================
void CFlush::TaskProcWait()
{
	// テクスチャ解放待ち
	if (!AoTexIsReleased(&m_work.tex)) {
		return;
	}
#if _PS3
	if (!AoTexIsReleased(&m_work.btn_tex)) {
		return;
	}
#endif // _PS3

	// 解放済み設定
	m_work.builded = FALSE;
#if _PS3
	m_work.btn_builded = FALSE;
#endif // _PS3

	// 終了
	delete this;
}

} // namespace buyscreen
} // namespace dm

#endif // _PC || _XBOX || _PS3

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
