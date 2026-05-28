// ===========================================================================
/*!
	@file	evBuyScreen.cpp
	@brief	製品版（完全版）購入画面イベント定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#if _PC || _XBOX || _PS3 || _IPHONE

#include "dmSndBgmPlayer.h"
#include "evBuyScreen.h"
#include "dmBuyScreen.h"
#include "gs.h"
#include "gsMainSys.h"
#include "gsTrial.h"
#include "ao.h"
#include "dmSave.h"

#include "gsBackup.hpp"
using namespace gs::backup;

#include "mppUtil.h"

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）

// ===========================================================================
//! イベント遷移先列挙
// ===========================================================================
typedef enum tag_EVE_BUYSCREEN_NEXTEV {
	EVD_BUYSCREEN_NEXTEV_FULL	= 0,	//!< 製品版時遷移先
	EVD_BUYSCREEN_NEXTEV_TRIAL,			//!< 体験版時遷移先

	EVD_BUYSCREEN_NEXTEV_NUM,			//!< 遷移先数
} EVE_BUYSCREEN_NEXTEV;

// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

namespace ev {
namespace buyscreen {

// ===========================================================================
//! メインクラス
// ===========================================================================
class CMain : 
	public ao::CProc<CMain>, public ao::CTask<CMain>, public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CMain();

protected:

	//! デストラクタ
	virtual ~CMain();

	//! タスクプロシージャ
	void TaskProcMain();

	//! 体験版判定
	void ProcCheck();

	//! 購入画面ファイル読み込み
	void ProcLoad();

	//! 購入画面テクスチャ構築
	void ProcBuild();

	//! 購入画面実行
	void ProcExecute();

	//! 購入画面テクスチャ解放
	void ProcFlush();

	//! セーブ
	void ProcSave();

	//! 終了
	void ProcEnd();

#if !_PS3
	//! ゲーム終了
	void ProcGameEnd();
#endif // !_PS3

	//! 購入画面ワーク
	DMS_BUY_SCR_WORK m_work;

	//! イベント遷移先
	EVE_BUYSCREEN_NEXTEV m_next_evt;
};

} // namespace buyscreen
} // namespace ev

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! 製品版（完全版）購入画面イベント開始
// ===========================================================================
void EvBuyScreenStart(void* arg)
{
	UNREFERENCED_PARAMETER(arg);
	/*if(true) {
		mppUtil::launchUpsellScreen();//sss - new
	}
	else */
	{	
		new ev::buyscreen::CMain;
	}
	
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace ev {
namespace buyscreen {

// ***************************************************************************
// メインクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CMain::CMain()
{
	amAssert(GsTrialIsTrial());

	// アクション環境設定
	AoActSysSetDrawTaskPrio();
	AoActSysSetDrawStateEnable();

	// メンバ初期化
	DmBuyScreenInit(&m_work);
	m_next_evt = EVD_BUYSCREEN_NEXTEV_TRIAL;

	// タスク作成
	MakeTask(0, "evBuyScreen::Main");
	SetTaskProc(0, &CMain::TaskProcMain);

	// プロシージャ設定
	SetProc(0, &CMain::ProcCheck);

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CMain::~CMain()
{
	// ファイル解放(念のため)
	DmBuyScreenRelease(&m_work);

	// イベント遷移
	SyChangeNextEvt();
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
//! 体験版判定
// ===========================================================================
void CMain::ProcCheck()
{
	// チェック開始
	if (GetCount() == 0) {
		GsTrialCheckStart();
	}

	// チェック完了判定
	if (GsTrialCheckIsFinished()) {

		// 体験版判定
		if (GsTrialIsTrial()) {
			// 購入画面ファイル読み込みへ遷移
			SetOwnProc(&CMain::ProcLoad);
		}
		else {
			// 既に製品版なのでセーブして続行
			SetOwnProc(&CMain::ProcSave);
		}
	}
}

// ===========================================================================
//! 購入画面ファイル読み込み
// ===========================================================================
void CMain::ProcLoad()
{
	// 読み込み開始
	if (GetCount() == 0) {
		DmBuyScreenLoadStart(&m_work);
	}

	// 読み込み完了判定
	if (DmBuyScreenLoadIsFinished(&m_work)) {

		// 購入画面テクスチャ構築へ遷移
		SetOwnProc(&CMain::ProcBuild);
	}
}

// ===========================================================================
//! 購入画面テクスチャ構築
// ===========================================================================
void CMain::ProcBuild()
{
	// 構築開始
	if (GetCount() == 0) {
		DmBuyScreenBuildStart(&m_work);

#if _IPHONE
		//サウンド初期化
		DmSndBgmPlayerInit();
#endif //_IPHONE
	}

	// 構築完了判定
	if (DmBuyScreenBuildIsFinished(&m_work)) {

		// 購入画面実行へ遷移
		SetOwnProc(&CMain::ProcExecute);
	}
}

// ===========================================================================
//! 購入画面実行
// ===========================================================================
void CMain::ProcExecute()
{
	// 購入画面開始
	if (GetCount() == 0) {
		if(true) {
			mppUtil::launchUpsellScreen(false);//sss - new (after game)
			//SetOwnProc(&CMain::ProcEnd);
			CMain::ProcEnd();
			return;
		}
		else {		
			DmBuyScreenStart(&m_work, TRUE, TRUE);//sss (after game)
		}
	}

	// 購入画面終了判定
	if (DmBuyScreenIsFinished(&m_work)) {

		// 購入画面テクスチャ解放へ遷移
		SetOwnProc(&CMain::ProcFlush);
	}
}

// ===========================================================================
//! 購入画面テクスチャ解放
// ===========================================================================
void CMain::ProcFlush()
{
	// 解放開始
	if (GetCount() == 0) {
		DmBuyScreenFlushStart(&m_work);
	}

	// 解放完了判定
	if (DmBuyScreenFlushIsFinished(&m_work)) {

		// 結果取得
		DME_BUY_SCR_RESULT result = DmBuyScreenGetResult(&m_work);

		// ファイル解放
		DmBuyScreenRelease(&m_work);

		// 結果判定
		switch (result) {
		case DMD_BUY_SCR_RESULT_BUY:

			// ゲームクリア判定
			if (GsGetMainSysInfo()->game_flag & GSD_MAINSYS_GAME_FLAG_CLEAR) {
				// イベント遷移先設定
				m_next_evt = EVD_BUYSCREEN_NEXTEV_FULL;
			}
			else {
				// イベント遷移先設定
				m_next_evt = EVD_BUYSCREEN_NEXTEV_TRIAL;
			}

			// 終了へ遷移
			SetOwnProc(&CMain::ProcEnd);
			break;

#if !_PS3
		case DMD_BUY_SCR_RESULT_BACK:

			// ゲーム終了へ遷移
			SetOwnProc(&CMain::ProcGameEnd);
			break;
#endif // !_PS3

		case DMD_BUY_SCR_RESULT_CANCEL:
		default:

			// イベント遷移先設定
			m_next_evt = EVD_BUYSCREEN_NEXTEV_TRIAL;

			// 終了へ遷移
			SetOwnProc(&CMain::ProcEnd);
			break;
		}
	}
}

// ===========================================================================
//! セーブ
// ===========================================================================
void CMain::ProcSave()
{
	// セーブ開始
	if (GetCount() == 0) {
		DmSaveStart(1 << DME_SAVE_WIN_TRIAL_OUT_SAVE, FALSE);
	}

	// セーブ終了判定
	if (DmSaveIsExit()) {
		// ゲームクリア判定
		if (GsGetMainSysInfo()->game_flag & GSD_MAINSYS_GAME_FLAG_CLEAR) {
			// イベント遷移先設定
			m_next_evt = EVD_BUYSCREEN_NEXTEV_FULL;
		}
		else {
			// イベント遷移先設定
			m_next_evt = EVD_BUYSCREEN_NEXTEV_TRIAL;
		}

		// 終了へ遷移
		SetOwnProc(&CMain::ProcEnd);
	}

}

// ===========================================================================
//! 終了
// ===========================================================================
void CMain::ProcEnd()
{
	SyDecideEvtCase((s16)m_next_evt);

	// 体験版ならセーブデータを初期化
	if (GsTrialIsTrial()) {
		// オプションデータのインスタンス作成
		gs::backup::SOption &data = gs::backup::SOption::CreateInstance();
		
		// ボリュームのみ初期化しないため、データを退避
		u32 vol_bgm = data.GetVolumeBgm();
		u32 vol_se = data.GetVolumeSe();
#if _IPHONE
		// iPhoneでは操作方法も初期化しないため、データを退避
		gs::backup::SOption::EControl::Type ctrl = data.GetControl();
#endif //_IPHONE
		
		SBackup::CreateInstance().Init();
		
		// 退避していたボリューム値を設定
		data.SetVolumeBgm(vol_bgm);
		data.SetVolumeSe(vol_se);
#if _IPHONE
		data.SetControl(ctrl);
#endif //_IPHONE
	}

	// 終了
	SetOwnProcNone();
}

#if !_PS3
// ===========================================================================
//! ゲーム終了
// ===========================================================================
void CMain::ProcGameEnd()
{
	if (GetCount() == 0) {
#if _XBOX
		amXboxReqExit();
#elif _PC
		amWinMainLoopQuit();
#else
		amAssert(0);
		m_next_evt = EVD_BUYSCREEN_NEXTEV_TRIAL;
		SetOwnProc(&CMain::ProcEnd);
#endif
	}
}
#endif // !_PS3

} // namespace buyscreen
} // namespace ev

#endif // _PC || _XBOX || _PS3 || _IPHONE

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
