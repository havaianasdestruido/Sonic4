// ============================================================================
/*!
	@file	dbgSaveDataEditor.cpp
	@brief	セーブデータエディタ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: dbgSaveDataEditor.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */
#if defined(AMD_DEBUG)
//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "dbgSaveDataEditor.hpp"
#include "gsEnvironment.h"
#include "gsBackup.hpp"
#include "gsCastBackup.hpp"
#include "accelLerp.hpp"
#include <algorithm>
#include <aoStorage.h>


#if _WII
#pragma warn_notinlined off	//インライン展開出来無い関数に対する警告メッセージの無効化
#endif //_WII


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*


namespace dbg {





































































//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// =============================================================================
// CIoEditor::Focus
/*!
	選択中の処理

	@param	cb	[io]	コールバックデータ

	@retval	true	終了
	@retval	false	継続
 */
// ==========================================================================
bool CIoEditor::Focus(CEvtBase &cb)
{
	bool result = update(cb);
	print(cb, true);
	return result;
}

// ============================================================================
// CIoEditor::Blur
/*!
	非選択中の処理

	@param	cb	[io]	コールバックデータ
 */
// ============================================================================
void CIoEditor::Blur(CEvtBase &cb)
{
	print(cb, false);
}

// =============================================================================
// CIoEditor::Full
/*!
	最強化

	@param	cb	[io]	コールバックデータ
 */
// ==========================================================================
void CIoEditor::Full(CEvtBase &)
{
}

// ============================================================================
// CIoEditor::Reset
/*!
	白紙化

	@param	cb	[io]	コールバックデータ
 */
// ============================================================================
void CIoEditor::Reset(CEvtBase &)
{
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// ============================================================================
// CIoEditor::CIoEditor
/*!
	デフォルトコンストラクタ
 */
// ============================================================================
CIoEditor::CIoEditor() : super_type() {
	m_crsr.SetRange(0, EElement::Max);
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// =============================================================================
// CIoEditor::update
/*!
	選択中の処理

	@param	cb	[io]	コールバックデータ

	@retval	true	終了
	@retval	false	継続
 */
// ==========================================================================
bool CIoEditor::update(CEvtBase &cb)
{
	bool result = false;

	//カーソル移動
	if (cb.IsPadDirect(GSD_KEY_UP)) {
		if (cb.IsPadRepeat(GSD_KEY_UP) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
			--m_crsr;
		}
	} else if (cb.IsPadDirect(GSD_KEY_DOWN)) {
		if (cb.IsPadRepeat(GSD_KEY_DOWN) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
			++m_crsr;
		}
	}

	using namespace gs::backup;
	SBackup &backup = SBackup::CreateInstance();
	Uint32	size = sizeof(backup);

	switch (m_crsr) {
	case EElement::Load:	//読み込み
		if (cb.IsPadStand(GSD_KEY_DECIDE)) {
			AoStorageLoadStart(&backup, size);
		}
		break;
	case EElement::Save:	//書き込み
		if (cb.IsPadStand(GSD_KEY_DECIDE)) {
			AoStorageSaveStart(&backup, size, FALSE);
		}
		break;
	case EElement::FirstSave:	//書き込み(初回)
		if (cb.IsPadStand(GSD_KEY_DECIDE)) {
			AoStorageSaveStart(&backup, size, TRUE);
		}
		break;
	case EElement::ErrorClear:	//エラークリア
		if (cb.IsPadStand(GSD_KEY_DECIDE)) {
			AoStorageClearError();
		}
		break;
#if _WII || _IPHONE
	case EElement::Delete:	//削除
		if (cb.IsPadStand(GSD_KEY_DECIDE)) {
			AoStorageDeleteStart();
		}
		break;
#endif //_WII || _IPHONE
	default:
		break;
	}

	if (cb.IsPadStand(GSD_KEY_CANCEL)) {
		result = true;
	}

	return result;
}

// ============================================================================
// CIoEditor::print
/*!
	表示

	@param	cb			[io]	コールバックデータ
	@param	is_enable	[in]	有効判定
 */
// ============================================================================
void CIoEditor::print(CEvtBase &cb, bool is_enable)
{
	int x, y;
	CEvtBase::EColor::Type clr = ((is_enable)? CEvtBase::EColor::White: CEvtBase::EColor::Gray);
	CEvtBase::EColor::Type crnt = ((is_enable)? CEvtBase::EColor::Yellow: CEvtBase::EColor::DarkYellow);
	CEvtBase::EColor::Type	font_clr;

	//操作説明
	x = 38;
	y = 5;
	cb.Printc(x, y++, clr, "%c         : Return", GsEnvDebugGetCancelKeyChar());
	cb.Printc(x, y++, clr, "Up/Down   : Select  (L/R:SpeedUp)");
	cb.Printc(x, y++, clr, "%c         : Enter", GsEnvDebugGetDecideKeyChar());


	//using namespace gs::backup;
	//SBackup &backup = SBackup::CreateInstance();

	//項目
	y += 2;
	int w = x + 20;

	//カーソル
	cb.Printc(x-1, y+m_crsr.GetVal<int>(), crnt, ">");

	//読み込み
	font_clr = (((m_crsr==EElement::Load))? crnt: clr);
	cb.Printc(x, y, font_clr, "Load");
	y++;
	//書き込み
	font_clr = (((m_crsr==EElement::Save))? crnt: clr);
	cb.Printc(x, y, font_clr, "Save");
	y++;
	//書き込み(初回)
	font_clr = (((m_crsr==EElement::FirstSave))? crnt: clr);
	cb.Printc(x, y, font_clr, "Save (First)");
	y++;
	//エラークリア
	font_clr = (((m_crsr==EElement::ErrorClear))? crnt: clr);
	cb.Printc(x, y, font_clr, "ErrorClear");
	y++;
#if _WII || _IPHONE
	//削除
	font_clr = (((m_crsr==EElement::Delete))? crnt: clr);
	cb.Printc(x, y, font_clr, "Delete");
	y++;
#endif //_WII || _IPHONE

	//インフォメーション
	y+=3;
	//エラー判定
	font_clr = clr;
	cb.Printc(x, y, font_clr, "IsError()");
	cb.Printc(w, y, font_clr, "%s", ((AoStorageIsError())? "true": "false"));
	y++;
	//エラー取得
	font_clr = clr;
	cb.Printc(x, y, font_clr, "GetError()");
	cb.Printc(w, y, font_clr, "%d", AoStorageGetError());
	y++;
	//ロード完了判定
	font_clr = clr;
	cb.Printc(x, y, font_clr, "LoadIsFinished()");
	cb.Printc(w, y, font_clr, "%s", ((AoStorageLoadIsFinished())? "true": "false"));
	y++;
	//ロード成功判定
	font_clr = clr;
	cb.Printc(x, y, font_clr, "LoadIsSuccessed()");
	if (AoStorageLoadIsFinished()) {
		cb.Printc(w, y, font_clr, "%s", ((AoStorageLoadIsSuccessed())? "true": "false"));
	}
	y++;
	//セーブ完了判定
	font_clr = clr;
	cb.Printc(x, y, font_clr, "SaveIsFinished()");
	cb.Printc(w, y, font_clr, "%s", ((AoStorageSaveIsFinished())? "true": "false"));
	y++;
	//セーブ成功判定
	font_clr = clr;
	cb.Printc(x, y, font_clr, "SaveIsSuccessed()");
	if (AoStorageSaveIsFinished()) {
		cb.Printc(w, y, font_clr, "%s", ((AoStorageSaveIsSuccessed())? "true": "false"));
	}
	y++;
#if _WII || _IPHONE
	//削除完了判定
	font_clr = clr;
	cb.Printc(x, y, font_clr, "DeleteIsFinished()");
	cb.Printc(w, y, font_clr, "%s", ((AoStorageDeleteIsFinished())? "true": "false"));
	y++;
	//削除成功判定
	font_clr = clr;
	cb.Printc(x, y, font_clr, "DeleteIsSuccessed()");
	if (AoStorageDeleteIsFinished()) {
		cb.Printc(w, y, font_clr, "%s", ((AoStorageDeleteIsSuccessed())? "true": "false"));
	}
	y++;
#endif //_WII || _IPHONE
}


//------------------------------------------------------------------------------**********





































































//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// =============================================================================
// CSystemEditor::Focus
/*!
	選択中の処理

	@param	cb	[io]	コールバックデータ

	@retval	true	終了
	@retval	false	継続
 */
// ==========================================================================
bool CSystemEditor::Focus(CEvtBase &cb)
{
	bool result = update(cb);
	print(cb, true);
	return result;
}

// ============================================================================
// CSystemEditor::Blur
/*!
	非選択中の処理

	@param	cb	[io]	コールバックデータ
 */
// ============================================================================
void CSystemEditor::Blur(CEvtBase &cb)
{
	print(cb, false);
}

// =============================================================================
// CSystemEditor::Full
/*!
	最強化

	@param	cb	[io]	コールバックデータ
 */
// ==========================================================================
void CSystemEditor::Full(CEvtBase &)
{
}

// ============================================================================
// CSystemEditor::Reset
/*!
	白紙化

	@param	cb	[io]	コールバックデータ
 */
// ============================================================================
void CSystemEditor::Reset(CEvtBase &)
{
	using namespace gs::backup;
	SSystem &system = SSystem::CreateInstance();
	system.Init();
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// ============================================================================
// CSystemEditor::CSystemEditor
/*!
	デフォルトコンストラクタ
 */
// ============================================================================
CSystemEditor::CSystemEditor() : super_type() {
	m_crsr.SetRange(0, EElement::Max);
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// =============================================================================
// CSystemEditor::update
/*!
	選択中の処理

	@param	cb	[io]	コールバックデータ

	@retval	true	終了
	@retval	false	継続
 */
// ==========================================================================
bool CSystemEditor::update(CEvtBase &cb)
{
	bool result = false;

	//カーソル移動
	if (cb.IsPadDirect(GSD_KEY_UP)) {
		if (cb.IsPadRepeat(GSD_KEY_UP) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
			--m_crsr;
		}
	} else if (cb.IsPadDirect(GSD_KEY_DOWN)) {
		if (cb.IsPadRepeat(GSD_KEY_DOWN) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
			++m_crsr;
		}
	}

	using namespace gs::backup;
	SSystem &system = SSystem::CreateInstance();

	switch (m_crsr) {
	case EElement::PlayerStock:	//残機
		{
			er::CSmallInt<Uint32> val(0, SSystem::c_player_stock_limit+1);
			val = system.GetPlayerStock();
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val -= ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val += ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				system.SetPlayerStock(val);
			}
		}
		break;
#if _WII
	case EElement::LastClearAct:	//最終クリアアクト
		{
			er::CSmallInt<Uint32> val(0, EStage::Max);
			val = system.GetLastClearAct();
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					--val;
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					++val;
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				system.SetLastClearAct(val.GetVal<EStage::Type>());
			}
		}
		break;
	case EElement::LastSaveChrono:	//最終セーブ時刻
		{
			Sint64 val = system.GetLastSaveChrono();
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					--val;
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					++val;
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				system.SetLastSaveChrono(val);
			}
		}
		break;
#endif // _WII
	case EElement::AnnounceOpenZoneSelect:	//アナウンス・ゾーンセレクト開放
		{
			bool val = system.IsAnnounce(SSystem::EAnnounce::OpenZoneSelect);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				system.SetAnnounce(SSystem::EAnnounce::OpenZoneSelect, val);
			}
		}
		break;
	case EElement::AnnounceOpenZone1Boss:	//アナウンス・ゾーン1ボス開放
		{
			bool val = system.IsAnnounce(SSystem::EAnnounce::OpenZone1Boss);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				system.SetAnnounce(SSystem::EAnnounce::OpenZone1Boss, val);
			}
		}
		break;
	case EElement::AnnounceOpenZone2Boss:	//アナウンス・ゾーン2ボス開放
		{
			bool val = system.IsAnnounce(SSystem::EAnnounce::OpenZone2Boss);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				system.SetAnnounce(SSystem::EAnnounce::OpenZone2Boss, val);
			}
		}
		break;
	case EElement::AnnounceOpenZone3Boss:	//アナウンス・ゾーン3ボス開放
		{
			bool val = system.IsAnnounce(SSystem::EAnnounce::OpenZone3Boss);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				system.SetAnnounce(SSystem::EAnnounce::OpenZone3Boss, val);
			}
		}
		break;
	case EElement::AnnounceOpenZone4Boss:	//アナウンス・ゾーン4ボス開放
		{
			bool val = system.IsAnnounce(SSystem::EAnnounce::OpenZone4Boss);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				system.SetAnnounce(SSystem::EAnnounce::OpenZone4Boss, val);
			}
		}
		break;
	case EElement::AnnounceOpenFinalZone:	//アナウンス・ファイナルゾーン開放
		{
			bool val = system.IsAnnounce(SSystem::EAnnounce::OpenFinalZone);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				system.SetAnnounce(SSystem::EAnnounce::OpenFinalZone, val);
			}
		}
		break;
	case EElement::AnnounceOpenSuperSonic:	//アナウンス・スーパーソニック
		{
			bool val = system.IsAnnounce(SSystem::EAnnounce::OpenSuperSonic);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				system.SetAnnounce(SSystem::EAnnounce::OpenSuperSonic, val);
			}
		}
		break;
	case EElement::AnnounceOpenSpecialStage:	//アナウンス・スペステゾーン開放
		{
			bool val = system.IsAnnounce(SSystem::EAnnounce::OpenSpecialStage);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				system.SetAnnounce(SSystem::EAnnounce::OpenSpecialStage, val);
			}
		}
		break;
#if _IPHONE
	case EElement::AnnounceTruckTilt:	//アナウンス・トロッコステージ・傾斜操作メッセージ
		{
			bool val = system.IsAnnounce(SSystem::EAnnounce::TruckTilt);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				system.SetAnnounce(SSystem::EAnnounce::TruckTilt, val);
			}
		}
		break;
	case EElement::AnnounceTruckFlick:	//アナウンス・トロッコステージ・フリック操作メッセージ
		{
			bool val = system.IsAnnounce(SSystem::EAnnounce::TruckFlick);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				system.SetAnnounce(SSystem::EAnnounce::TruckFlick, val);
			}
		}
		break;
	case EElement::AnnounceSpecialStageTilt:	//アナウンス・スペシャルステージ・傾斜操作メッセージ
		{
			bool val = system.IsAnnounce(SSystem::EAnnounce::SpecialStageTilt);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				system.SetAnnounce(SSystem::EAnnounce::SpecialStageTilt, val);
			}
		}
		break;
	case EElement::AnnounceSpecialStageFlick:	//アナウンス・スペシャルステージ・フリック操作メッセージ
		{
			bool val = system.IsAnnounce(SSystem::EAnnounce::SpecialStageFlick);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				system.SetAnnounce(SSystem::EAnnounce::SpecialStageFlick, val);
			}
		}
		break;
#endif //_IPHONE
	case EElement::Killed:	//累計エネミー撃退数
		{
			er::CSmallInt<Uint32> val(0, SSystem::c_killed_limit+1);
			val = system.GetKilled();
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val -= ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val += ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				system.SetKilled(val);
			}
		}
		break;
	case EElement::ClearCount:	//クリア回数
		{
			er::CSmallInt<Uint32> val(0, SSystem::c_clear_count_limit+1);
			val = system.GetClearCount();
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val -= ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val += ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				system.SetClearCount(val);
			}
		}
		break;
	default:
		break;
	}

	if (cb.IsPadStand(GSD_KEY_CANCEL)) {
		result = true;
	}

	return result;
}

// ============================================================================
// CSystemEditor::print
/*!
	表示

	@param	cb			[io]	コールバックデータ
	@param	is_enable	[in]	有効判定
 */
// ============================================================================
void CSystemEditor::print(CEvtBase &cb, bool is_enable)
{
	int x, y;
	CEvtBase::EColor::Type clr = ((is_enable)? CEvtBase::EColor::White: CEvtBase::EColor::Gray);
	CEvtBase::EColor::Type crnt = ((is_enable)? CEvtBase::EColor::Yellow: CEvtBase::EColor::DarkYellow);
	CEvtBase::EColor::Type	font_clr;

	//操作説明
	x = 38;
	y = 5;
	cb.Printc(x, y++, clr, "%c         : Return", GsEnvDebugGetCancelKeyChar());
	cb.Printc(x, y++, clr, "Up/Down   : Select  (L/R:SpeedUp)");
	cb.Printc(x, y++, clr, "Left/Right: Change  (L/R:SpeedUp)");

	using namespace gs::backup;
	SSystem &system = SSystem::CreateInstance();

	//項目
	y += 2;
	int w = x + 32;

	static const char *announce_str[] = {"already", "yet"};

	//カーソル
	cb.Printc(x-1, y+m_crsr.GetVal<int>(), crnt, ">");

	//残機
	font_clr = (((m_crsr==EElement::PlayerStock))? crnt: clr);
	cb.Printc(x, y, font_clr, "Player stock");
	cb.Printc(w, y, font_clr, "%d", system.GetPlayerStock());
	y++;
#if _WII
	//最終クリアアクト
	font_clr = (((m_crsr==EElement::LastClearAct))? crnt: clr);
	cb.Printc(x, y, font_clr, "Last clear act");
	cb.Printc(w, y, font_clr, "%d", system.GetLastClearAct());
	y++;
	//最終セーブ時刻
	{
		Sint64 chrono64 = system.GetLastSaveChrono();
		Uint32 *chrono = reinterpret_cast<Uint32 *>(&chrono64);
		font_clr = (((m_crsr==EElement::LastSaveChrono))? crnt: clr);
		cb.Printc(x, y, font_clr, "Last save chrono");
		cb.Printc(w, y, font_clr, "%08X,%08X", chrono[0], chrono[1]);
		y++;
	}
#endif // _WII
	//アナウンス・ゾーンセレクト開放
	font_clr = (((m_crsr==EElement::AnnounceOpenZoneSelect))? crnt: clr);
	cb.Printc(x, y, font_clr, "Announce/ Open zone Select");
	cb.Printc(w, y, font_clr, "%s", announce_str[((system.IsAnnounce(SSystem::EAnnounce::OpenZoneSelect))? 0: 1)]);
	y++;
	//アナウンス・ゾーン1ボス開放
	font_clr = (((m_crsr==EElement::AnnounceOpenZone1Boss))? crnt: clr);
	cb.Printc(x, y, font_clr, "Announce/ Open zone1 boss");
	cb.Printc(w, y, font_clr, "%s", announce_str[((system.IsAnnounce(SSystem::EAnnounce::OpenZone1Boss))? 0: 1)]);
	y++;
	//アナウンス・ゾーン2ボス開放
	font_clr = (((m_crsr==EElement::AnnounceOpenZone2Boss))? crnt: clr);
	cb.Printc(x, y, font_clr, "Announce/ Open zone2 boss");
	cb.Printc(w, y, font_clr, "%s", announce_str[((system.IsAnnounce(SSystem::EAnnounce::OpenZone2Boss))? 0: 1)]);
	y++;
	//アナウンス・ゾーン3ボス開放
	font_clr = (((m_crsr==EElement::AnnounceOpenZone3Boss))? crnt: clr);
	cb.Printc(x, y, font_clr, "Announce/ Open zone3 boss");
	cb.Printc(w, y, font_clr, "%s", announce_str[((system.IsAnnounce(SSystem::EAnnounce::OpenZone3Boss))? 0: 1)]);
	y++;
	//アナウンス・ゾーン4ボス開放
	font_clr = (((m_crsr==EElement::AnnounceOpenZone4Boss))? crnt: clr);
	cb.Printc(x, y, font_clr, "Announce/ Open zone4 boss");
	cb.Printc(w, y, font_clr, "%s", announce_str[((system.IsAnnounce(SSystem::EAnnounce::OpenZone4Boss))? 0: 1)]);
	y++;
	//アナウンス・ファイナルゾーン開放
	font_clr = (((m_crsr==EElement::AnnounceOpenFinalZone))? crnt: clr);
	cb.Printc(x, y, font_clr, "Announce/ Open final zone");
	cb.Printc(w, y, font_clr, "%s", announce_str[((system.IsAnnounce(SSystem::EAnnounce::OpenFinalZone))? 0: 1)]);
	y++;
	//アナウンス・スーパーソニック
	font_clr = (((m_crsr==EElement::AnnounceOpenSuperSonic))? crnt: clr);
	cb.Printc(x, y, font_clr, "Announce/ Open S-sonic");
	cb.Printc(w, y, font_clr, "%s", announce_str[((system.IsAnnounce(SSystem::EAnnounce::OpenSuperSonic))? 0: 1)]);
	y++;
	//アナウンス・スペステゾーン開放
	font_clr = (((m_crsr==EElement::AnnounceOpenSpecialStage))? crnt: clr);
	cb.Printc(x, y, font_clr, "Announce/ Open special zone");
	cb.Printc(w, y, font_clr, "%s", announce_str[((system.IsAnnounce(SSystem::EAnnounce::OpenSpecialStage))? 0: 1)]);
	y++;
#if _IPHONE
	//アナウンス・トロッコステージ・傾斜操作メッセージ
	font_clr = (((m_crsr==EElement::AnnounceTruckTilt))? crnt: clr);
	cb.Printc(x, y, font_clr, "Announce/ Truck tilt");
	cb.Printc(w, y, font_clr, "%s", announce_str[((system.IsAnnounce(SSystem::EAnnounce::TruckTilt))? 0: 1)]);
	y++;
	//アナウンス・トロッコステージ・フリック操作メッセージ
	font_clr = (((m_crsr==EElement::AnnounceTruckFlick))? crnt: clr);
	cb.Printc(x, y, font_clr, "Announce/ Truck flick");
	cb.Printc(w, y, font_clr, "%s", announce_str[((system.IsAnnounce(SSystem::EAnnounce::TruckFlick))? 0: 1)]);
	y++;
	//アナウンス・スペシャルステージ・傾斜操作メッセージ
	font_clr = (((m_crsr==EElement::AnnounceSpecialStageTilt))? crnt: clr);
	cb.Printc(x, y, font_clr, "Announce/ Special stage tilt");
	cb.Printc(w, y, font_clr, "%s", announce_str[((system.IsAnnounce(SSystem::EAnnounce::SpecialStageTilt))? 0: 1)]);
	y++;
	//アナウンス・スペシャルステージ・フリック操作メッセージ
	font_clr = (((m_crsr==EElement::AnnounceSpecialStageFlick))? crnt: clr);
	cb.Printc(x, y, font_clr, "Announce/ Special stage flick");
	cb.Printc(w, y, font_clr, "%s", announce_str[((system.IsAnnounce(SSystem::EAnnounce::SpecialStageFlick))? 0: 1)]);
	y++;
#endif //_IPHONE
	//累計エネミー撃退数
	font_clr = (((m_crsr==EElement::Killed))? crnt: clr);
	cb.Printc(x, y, font_clr, "Killed");
	cb.Printc(w, y, font_clr, "%d", system.GetKilled());
	y++;
	//ゲームクリア回数
	font_clr = (((m_crsr==EElement::ClearCount))? crnt: clr);
	cb.Printc(x, y, font_clr, "ClearCount");
	cb.Printc(w, y, font_clr, "%d", system.GetClearCount());
	y++;
}


//------------------------------------------------------------------------------**********



































































//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// =============================================================================
// CStageEditor::Focus
/*!
	選択中の処理

	@param	cb	[io]	コールバックデータ

	@retval	true	終了
	@retval	false	継続
 */
// ==========================================================================
bool CStageEditor::Focus(CEvtBase &cb)
{
	bool result = update(cb);
	print(cb, true);
	return result;
}

// ============================================================================
// CStageEditor::Blur
/*!
	非選択中の処理

	@param	cb	[io]	コールバックデータ
 */
// ============================================================================
void CStageEditor::Blur(CEvtBase &cb)
{
	print(cb, false);
}

// =============================================================================
// CStageEditor::Full
/*!
	最強化

	@param	cb	[io]	コールバックデータ
 */
// ==========================================================================
void CStageEditor::Full(CEvtBase &)
{
}

// ============================================================================
// CStageEditor::Reset
/*!
	白紙化

	@param	cb	[io]	コールバックデータ
 */
// ============================================================================
void CStageEditor::Reset(CEvtBase &)
{
	using namespace gs::backup;
	SStage &stage = SStage::CreateInstance();
	stage.Init();
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// ============================================================================
// CStageEditor::CStageEditor
/*!
	デフォルトコンストラクタ
 */
// ============================================================================
CStageEditor::CStageEditor() : super_type() {
	using namespace gs::backup;
#if 0
	SStage &stage = SStage::CreateInstance();
	m_crsr.SetRange(0, stage.GetSize() * EElement::Max);
#else
	m_crsr.SetRange(0, SStage::c_size * EElement::Max);
#endif
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// =============================================================================
// CStageEditor::update
/*!
	選択中の処理

	@param	cb	[io]	コールバックデータ

	@retval	true	終了
	@retval	false	継続
 */
// ==========================================================================
bool CStageEditor::update(CEvtBase &cb)
{
	bool result = false;

	//カーソル移動
	if (cb.IsPadDirect(GSD_KEY_UP)) {
		if (cb.IsPadRepeat(GSD_KEY_UP) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
			--m_crsr;
		}
	} else if (cb.IsPadDirect(GSD_KEY_DOWN)) {
		if (cb.IsPadRepeat(GSD_KEY_DOWN) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
			++m_crsr;
		}
	}

	using namespace gs::backup;
	SStage &stage = SStage::CreateInstance();

	EStage::Type	idx = EStage::Type(m_crsr.GetVal<Uint32>() / EElement::Max);
	EElement::Type	element = EElement::Type(m_crsr.GetVal<Uint32>() % EElement::Max);

	switch (element) {
	case EElement::New:	//NEW状態
		{
			bool val = stage[idx].IsNew();
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				stage[idx].SetNew(val);
			}
		}
		break;
	case EElement::HighScore:	//ハイスコア
		{
			er::CSmallInt<Uint32> val(0, SStageSolo::c_high_score_max_limit+1);
			val = stage[idx].GetHighScore(false);
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val -= SStageSolo::c_high_score_unit * ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val += SStageSolo::c_high_score_unit * ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				stage[idx].SetHighScore(val, false);
			}
		}
		break;
	case EElement::FastTime:	//最速タイム
		{
			er::CSmallInt<Uint32> val(0, SStageSolo::c_fast_time_max_limit+1);
			val = stage[idx].GetFastTime(false);
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val -= ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val += ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				stage[idx].SetFastTime(val, false);
			}
		}
		break;
	case EElement::HighScoreS:	//ハイスコア(スーパーソニック使用)
		{
			er::CSmallInt<Uint32> val(0, SStageSolo::c_high_score_max_limit+1);
			val = stage[idx].GetHighScore(true);
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val -= SStageSolo::c_high_score_unit * ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val += SStageSolo::c_high_score_unit * ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				stage[idx].SetHighScore(val, true);
			}
		}
		break;
	case EElement::FastTimeS:	//最速タイム(スーパーソニック使用)
		{
			er::CSmallInt<Uint32> val(0, SStageSolo::c_fast_time_max_limit+1);
			val = stage[idx].GetFastTime(true);
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val -= ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val += ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				stage[idx].SetFastTime(val, true);
			}
		}
		break;
	case EElement::HighScoreUploaded:	//ハイスコアはアップロード済み
		{
			bool val = stage[idx].IsHighScoreUploaded(false);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				stage[idx].SetHighScoreUploaded(false, val);
			}
		}
		break;
	case EElement::FastTimeUploaded:	//最速タイムはアップロード済み
		{
			bool val = stage[idx].IsFastTimeUploaded(false);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				stage[idx].SetFastTimeUploaded(false, val);
			}
		}
		break;
	case EElement::HighScoreUploadedS:	//ハイスコアはアップロード済み(スーパーソニック使用)
		{
			bool val = stage[idx].IsHighScoreUploaded(true);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				stage[idx].SetHighScoreUploaded(true, val);
			}
		}
		break;
	case EElement::FastTimeUploadedS:	//最速タイムはアップロード済み(スーパーソニック使用)
		{
			bool val = stage[idx].IsFastTimeUploaded(true);
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				stage[idx].SetFastTimeUploaded(true, val);
			}
		}
		break;
	case EElement::HighScoreUseSuperSonic:	//ハイスコアはスーパーソニック使用
		{
			bool val = stage[idx].IsHighScoreUseSuperSonic();
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				stage[idx].SetHighScoreUseSuperSonic(val);
			}
		}
		break;
	case EElement::FastTimeUseSuperSonic:	//最速タイムはスーパーソニック使用
		{
			bool val = stage[idx].IsFastTimeUseSuperSonic();
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				stage[idx].SetFastTimeUseSuperSonic(val);
			}
		}
		break;
	case EElement::ScoreUploadedOnce:	//過去1回はスコアをアップロード済みか
		{
			bool val = stage[idx].IsScoreUploadedOnce();
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				stage[idx].SetScoreUploadedOnce(val);
			}
		}
		break;
	case EElement::TimeUploadedOnce:	//過去1回はタイムをアップロード済みか
		{
			bool val = stage[idx].IsTimeUploadedOnce();
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				stage[idx].SetTimeUploadedOnce(val);
			}
		}
		break;
	case EElement::UseSuperSonicOnce:	//過去1回はスーパーソニック使用か
		{
			bool val = stage[idx].IsUseSuperSonicOnce();
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				stage[idx].SetUseSuperSonicOnce(val);
			}
		}
		break;
	default:
		break;
	}

	if (cb.IsPadStand(GSD_KEY_CANCEL)) {
		result = true;
	}

	return result;
}

// ============================================================================
// CStageEditor::print
/*!
	表示

	@param	cb			[io]	コールバックデータ
	@param	is_enable	[in]	有効判定
 */
// ============================================================================
void CStageEditor::print(CEvtBase &cb, bool is_enable)
{
	int x, y;
	CEvtBase::EColor::Type clr = ((is_enable)? CEvtBase::EColor::White: CEvtBase::EColor::Gray);
	CEvtBase::EColor::Type crnt = ((is_enable)? CEvtBase::EColor::Yellow: CEvtBase::EColor::DarkYellow);

	//操作説明
	x = 38;
	y = 5;
	cb.Printc(x, y++, clr, "%c         : Return", GsEnvDebugGetCancelKeyChar());
	cb.Printc(x, y++, clr, "Up/Down   : Select  (L/R:SpeedUp)");
	cb.Printc(x, y++, clr, "Left/Right: Change  (L/R:SpeedUp)");

	using namespace gs::backup;
	SStage &stage = SStage::CreateInstance();

	//項目
	y += 2;
	int w = x + 30;

#if 1
	//循環
	int h = 31;
	for (int i = 0; i < h; ++i) {
		if ((0 < i) && (i < (h-1))) {
			cb.Printc(x-2, y+i, clr, "|");
		} else {
			cb.Printc(x-2, y+i, clr, "-");
		}
	}
	//スクロール
	int scroll = accel::lerp::CLerp<int, 0>()(1, h - 2, m_crsr.GetVal<float>() / (m_crsr.GetRange() - 1));
	cb.Printc(x-2, y+scroll, clr, "*");
	//カーソル
	cb.Printc(x-1, y+h/2, crnt, ">");
	//項目
	for (int i = 0; i < h; ++i) {
		er::CSmallInt<>	crsr = m_crsr;
		crsr += i - h / 2;
		EStage::Type			idx = EStage::Type(crsr.GetVal<Uint32>() / EElement::Max);
		EElement::Type			element = EElement::Type(crsr.GetVal<Uint32>() % EElement::Max);
		CEvtBase::EColor::Type	font_clr = ((0 == (i - h / 2))? crnt: clr);
#else
	//リスト
	//カーソル
	cb.Printc(x-1, y+m_crsr.GetVal<int>(), crnt, ">");
	//項目
	for (int i = static_cast<int>(m_crsr.GetMin()), max = static_cast<int>(m_crsr.GetMax()); i < max; ++i) {
		EStage::Type			idx = EStage::Type(i / EElement::Max);
		EElement::Type			element = EElement::Type(i % EElement::Max);
		CEvtBase::EColor::Type	font_clr = ((i == m_crsr)? crnt: clr);
#endif

		switch (element) {
		case EElement::New:	//NEW状態
			cb.Printc(x, y, font_clr, "%-3s/ New", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%s", ((stage[idx].IsNew())? "new": "already"));
			break;
		case EElement::HighScore:	//ハイスコア
			cb.Printc(x, y, font_clr, "%-3s/ HighScore", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%d", stage[idx].GetHighScore(false));
			break;
		case EElement::FastTime:	//最速タイム
			cb.Printc(x, y, font_clr, "%-3s/ FastTime", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%d", stage[idx].GetFastTime(false));
			break;
		case EElement::HighScoreS:	//ハイスコア(スーパーソニック使用)
			cb.Printc(x, y, font_clr, "%-3s/ HighScore(S)", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%d", stage[idx].GetHighScore(true));
			break;
		case EElement::FastTimeS:	//最速タイム(スーパーソニック使用)
			cb.Printc(x, y, font_clr, "%-3s/ FastTime(S)", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%d", stage[idx].GetFastTime(true));
			break;
		case EElement::HighScoreUploaded:	//ハイスコアはアップロード済み
			cb.Printc(x, y, font_clr, "%-3s/ HighScore uploaded", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%s", ((stage[idx].IsHighScoreUploaded(false))? "already": "yet"));
			break;
		case EElement::FastTimeUploaded:	//最速タイムはアップロード済み
			cb.Printc(x, y, font_clr, "%-3s/ FastTime uploaded", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%s", ((stage[idx].IsFastTimeUploaded(false))? "already": "yet"));
			break;
		case EElement::HighScoreUploadedS:	//ハイスコアはアップロード済み(スーパーソニック使用)
			cb.Printc(x, y, font_clr, "%-3s/ HighScore uploaded(S)", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%s", ((stage[idx].IsHighScoreUploaded(true))? "already": "yet"));
			break;
		case EElement::FastTimeUploadedS:	//最速タイムはアップロード済み(スーパーソニック使用)
			cb.Printc(x, y, font_clr, "%-3s/ FastTime uploaded(S)", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%s", ((stage[idx].IsFastTimeUploaded(true))? "already": "yet"));
			break;
		case EElement::HighScoreUseSuperSonic:	//ハイスコアはスーパーソニック使用
			cb.Printc(x, y, font_clr, "%-3s/ HighScore use S-sonic", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%c", ((stage[idx].IsHighScoreUseSuperSonic())? 'O': 'X'));
			break;
		case EElement::FastTimeUseSuperSonic:	//最速タイムはスーパーソニック使用
			cb.Printc(x, y, font_clr, "%-3s/ FastTime use S-sonic", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%c", ((stage[idx].IsFastTimeUseSuperSonic())? 'O': 'X'));
			break;
		case EElement::ScoreUploadedOnce:	//過去1回はスコアをアップロード済みか
			cb.Printc(x, y, font_clr, "%-3s/ Score Uploaded once", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%s", ((stage[idx].IsScoreUploadedOnce())? "already": "yet"));
			break;
		case EElement::TimeUploadedOnce:	//過去1回はタイムをアップロード済みか
			cb.Printc(x, y, font_clr, "%-3s/ Time Uploaded once", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%s", ((stage[idx].IsTimeUploadedOnce())? "already": "yet"));
			break;
		case EElement::UseSuperSonicOnce:	//過去1回はスーパーソニック使用か
			cb.Printc(x, y, font_clr, "%-3s/ Use S-sonic once", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%c", ((stage[idx].IsUseSuperSonicOnce())? 'O': 'X'));
			break;
		default:
			break;
		}
		y++;
	}
}


//------------------------------------------------------------------------------**********



































































//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// =============================================================================
// CSpecialEditor::Focus
/*!
	選択中の処理

	@param	cb	[io]	コールバックデータ

	@retval	true	終了
	@retval	false	継続
 */
// ==========================================================================
bool CSpecialEditor::Focus(CEvtBase &cb)
{
	bool result = update(cb);
	print(cb, true);
	return result;
}

// ============================================================================
// CSpecialEditor::Blur
/*!
	非選択中の処理

	@param	cb	[io]	コールバックデータ
 */
// ============================================================================
void CSpecialEditor::Blur(CEvtBase &cb)
{
	print(cb, false);
}

// =============================================================================
// CSpecialEditor::Full
/*!
	最強化

	@param	cb	[io]	コールバックデータ
 */
// ==========================================================================
void CSpecialEditor::Full(CEvtBase &)
{
}

// ============================================================================
// CSpecialEditor::Reset
/*!
	白紙化

	@param	cb	[io]	コールバックデータ
 */
// ============================================================================
void CSpecialEditor::Reset(CEvtBase &)
{
	using namespace gs::backup;
	SSpecial &special = SSpecial::CreateInstance();
	special.Init();
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// ============================================================================
// CSpecialEditor::CSpecialEditor
/*!
	デフォルトコンストラクタ
 */
// ============================================================================
CSpecialEditor::CSpecialEditor() : super_type() {
	using namespace gs::backup;
#if 0
	SSpecial &special = SSpecial::CreateInstance();
	m_crsr.SetRange(0, special.GetSize() * EElement::Max);
#else
	m_crsr.SetRange(0, SSpecial::c_size * EElement::Max);
#endif
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// =============================================================================
// CSpecialEditor::update
/*!
	選択中の処理

	@param	cb	[io]	コールバックデータ

	@retval	true	終了
	@retval	false	継続
 */
// ==========================================================================
bool CSpecialEditor::update(CEvtBase &cb)
{
	bool result = false;

	//カーソル移動
	if (cb.IsPadDirect(GSD_KEY_UP)) {
		if (cb.IsPadRepeat(GSD_KEY_UP) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
			--m_crsr;
		}
	} else if (cb.IsPadDirect(GSD_KEY_DOWN)) {
		if (cb.IsPadRepeat(GSD_KEY_DOWN) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
			++m_crsr;
		}
	}

	using namespace gs::backup;
	SSpecial &special = SSpecial::CreateInstance();

	SSpecialSolo::EEmeraldStage::Type	idx = SSpecialSolo::EEmeraldStage::Type(m_crsr.GetVal<Uint32>() / EElement::Max);
	EElement::Type						element = EElement::Type(m_crsr.GetVal<Uint32>() % EElement::Max);

	switch (element) {
	case EElement::HighScore:	//ハイスコア
		{
			er::CSmallInt<Uint32> val(0, SSpecialSolo::c_high_score_max_limit+1);
			val = special[idx].GetHighScore();
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val -= SSpecialSolo::c_high_score_unit * ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val += SSpecialSolo::c_high_score_unit * ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				special[idx].SetHighScore(val);
			}
		}
		break;
	case EElement::GetEmeraldStage:	//エメラルド獲得ステージ
		{
			er::CSmallInt<Uint32> val(0, SSpecialSolo::EEmeraldStage::Max);
			val = special[idx].GetEmeraldStage();
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					--val;
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					++val;
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				special[idx].SetEmeraldStage(val.GetVal<SSpecialSolo::EEmeraldStage::Type>());
			}
		}
		break;
	default:
		break;
	}

	if (cb.IsPadStand(GSD_KEY_CANCEL)) {
		result = true;
	}

	return result;
}

// ============================================================================
// CSpecialEditor::print
/*!
	表示

	@param	cb			[io]	コールバックデータ
	@param	is_enable	[in]	有効判定
 */
// ============================================================================
void CSpecialEditor::print(CEvtBase &cb, bool is_enable)
{
	int x, y;
	CEvtBase::EColor::Type clr = ((is_enable)? CEvtBase::EColor::White: CEvtBase::EColor::Gray);
	CEvtBase::EColor::Type crnt = ((is_enable)? CEvtBase::EColor::Yellow: CEvtBase::EColor::DarkYellow);

	//操作説明
	x = 38;
	y = 5;
	cb.Printc(x, y++, clr, "%c         : Return", GsEnvDebugGetCancelKeyChar());
	cb.Printc(x, y++, clr, "Up/Down   : Select  (L/R:SpeedUp)");
	cb.Printc(x, y++, clr, "Left/Right: Change  (L/R:SpeedUp)");

	using namespace gs::backup;
	SSpecial &special = SSpecial::CreateInstance();

	//項目
	y += 2;
	int w = x + 24;

//	static const char *emerald_str[] = {"Red", "Blue", "Yellow", "Green", "White", "Cyan", "Purple"};

#if 0
	//循環
	int h = 31;
	for (int i = 0; i < h; ++i) {
		if ((0 < i) && (i < (h-1))) {
			cb.Printc(x-2, y+i, clr, "|");
		} else {
			cb.Printc(x-2, y+i, clr, "-");
		}
	}
	//スクロール
	int scroll = accel::lerp::CLerp<int, 0>()(1, h - 2, m_crsr.GetVal<float>() / (m_crsr.GetRange() - 1));
	cb.Printc(x-2, y+scroll, clr, "*");
	//カーソル
	cb.Printc(x-1, y+h/2, crnt, ">");
	//項目
	for (int i = 0; i < h; ++i) {
		er::CSmallInt<>	crsr = m_crsr;
		crsr += i - h / 2;
		SSpecialSolo::EEmeraldStage::Type	idx = SSpecialSolo::EEmeraldStage::Type(crsr.GetVal<Uint32>() / EElement::Max);
		EElement::Type						element = EElement::Type(crsr.GetVal<Uint32>() % EElement::Max);
		CEvtBase::EColor::Type				font_clr = ((0 == (i - h / 2))? crnt: clr);
#else
	//リスト
	//カーソル
	cb.Printc(x-1, y+m_crsr.GetVal<int>(), crnt, ">");
	//項目
	for (int i = static_cast<int>(m_crsr.GetMin()), max = static_cast<int>(m_crsr.GetMax()); i < max; ++i) {
		ESpecialStage::Type		idx = ESpecialStage::Type(i / EElement::Max);
		EElement::Type			element = EElement::Type(i % EElement::Max);
		CEvtBase::EColor::Type	font_clr = ((i == m_crsr)? crnt: clr);
#endif

		switch (element) {
		case EElement::HighScore:	//ハイスコア
			cb.Printc(x, y, font_clr, "%-3s/ HighScore", gs::public_cast<const char*>(idx));
			cb.Printc(w, y, font_clr, "%d", special[idx].GetHighScore());
			break;
		case EElement::GetEmeraldStage:	//エメラルド獲得ステージ
			cb.Printc(x, y, font_clr, "%-3s/ Acquisition stage", gs::public_cast<const char*>(idx));
			if (special[idx].IsGetEmerald()) {
				cb.Printc(w, y, font_clr, "%-3s", gs::public_cast<const char*>(special[idx].GetEmeraldStage()));
			} else {
				cb.Printc(w, y, font_clr, "X");
			}
			break;
		default:
			break;
		}
		y++;
	}
}


//------------------------------------------------------------------------------**********



































































//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// =============================================================================
// COptionEditor::Focus
/*!
	選択中の処理

	@param	cb	[io]	コールバックデータ

	@retval	true	終了
	@retval	false	継続
 */
// ==========================================================================
bool COptionEditor::Focus(CEvtBase &cb)
{
	bool result = update(cb);
	print(cb, true);
	return result;
}

// ============================================================================
// COptionEditor::Blur
/*!
	非選択中の処理

	@param	cb	[io]	コールバックデータ
 */
// ============================================================================
void COptionEditor::Blur(CEvtBase &cb)
{
	print(cb, false);
}

// =============================================================================
// COptionEditor::Full
/*!
	最強化

	@param	cb	[io]	コールバックデータ
 */
// ==========================================================================
void COptionEditor::Full(CEvtBase &)
{
}

// ============================================================================
// COptionEditor::Reset
/*!
	白紙化

	@param	cb	[io]	コールバックデータ
 */
// ============================================================================
void COptionEditor::Reset(CEvtBase &)
{
	using namespace gs::backup;
	SOption &option = SOption::CreateInstance();
	option.Init();
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// ============================================================================
// COptionEditor::COptionEditor
/*!
	デフォルトコンストラクタ
 */
// ============================================================================
COptionEditor::COptionEditor() : super_type() {
	m_crsr.SetRange(0, EElement::Max);
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// =============================================================================
// COptionEditor::update
/*!
	選択中の処理

	@param	cb	[io]	コールバックデータ

	@retval	true	終了
	@retval	false	継続
 */
// ==========================================================================
bool COptionEditor::update(CEvtBase &cb)
{
	bool result = false;

	//カーソル移動
	if (cb.IsPadDirect(GSD_KEY_UP)) {
		if (cb.IsPadRepeat(GSD_KEY_UP) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
			--m_crsr;
		}
	} else if (cb.IsPadDirect(GSD_KEY_DOWN)) {
		if (cb.IsPadRepeat(GSD_KEY_DOWN) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
			++m_crsr;
		}
	}

	using namespace gs::backup;
	SOption &option = SOption::CreateInstance();

	switch (m_crsr) {
	case EElement::Vibration:	//振動
		{
			bool val = option.IsVibration();
			if (cb.IsPadRepeat(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				val = !val;
				option.SetVibration(val);
			}
		}
		break;
	case EElement::VolumeBgm:	//ボリューム・BGM
		{
			er::CSmallInt<Uint32> val(0, SOption::c_volume_bgm_max_limit+1);
			val = option.GetVolumeBgm();
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val -= SOption::c_volume_bgm_unit;
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val += SOption::c_volume_bgm_unit;
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				option.SetVolumeBgm(val);
			}
		}
		break;
	case EElement::VolumeSe:	//ボリューム・SE
		{
			er::CSmallInt<Uint32> val(0, SOption::c_volume_se_max_limit+1);
			val = option.GetVolumeSe();
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val -= SOption::c_volume_se_unit;
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val += SOption::c_volume_se_unit;
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				option.SetVolumeSe(val);
			}
		}
		break;
#if _IPHONE
	case EElement::Control:	//コントロール方法
		{
			er::CSmallInt<> val(0, SOption::EControl::Max);
			val = option.GetControl();
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					--val;
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					++val;
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				option.SetControl(val.GetVal());
			}
		}
		break;
#endif //_IPHONE
#if _WII
	case EElement::UserName:	///ユーザー名
		{
		}
		break;
#endif // _WII
	default:
		break;
	}

	if (cb.IsPadStand(GSD_KEY_CANCEL)) {
		result = true;
	}

	return result;
}

// ============================================================================
// COptionEditor::print
/*!
	表示

	@param	cb			[io]	コールバックデータ
	@param	is_enable	[in]	有効判定
 */
// ============================================================================
void COptionEditor::print(CEvtBase &cb, bool is_enable)
{
	int x, y;
	CEvtBase::EColor::Type clr = ((is_enable)? CEvtBase::EColor::White: CEvtBase::EColor::Gray);
	CEvtBase::EColor::Type crnt = ((is_enable)? CEvtBase::EColor::Yellow: CEvtBase::EColor::DarkYellow);
	CEvtBase::EColor::Type	font_clr;

	//操作説明
	x = 38;
	y = 5;
	cb.Printc(x, y++, clr, "%c         : Return", GsEnvDebugGetCancelKeyChar());
	cb.Printc(x, y++, clr, "Up/Down   : Select  (L/R:SpeedUp)");
	cb.Printc(x, y++, clr, "Left/Right: Change  (L/R:SpeedUp)");

	using namespace gs::backup;
	SOption &option = SOption::CreateInstance();

	//項目
	y += 2;
	int w = x + 16;

	static const char *control_str[] = {"Tilt", "Virtual Pad (Down)", "Virtual Pad (Up)"};

	//カーソル
	cb.Printc(x-1, y+m_crsr.GetVal<int>(), crnt, ">");

	//振動
	font_clr = (((m_crsr==EElement::Vibration))? crnt: clr);
	cb.Printc(x, y, font_clr, "Vibration");
	cb.Printc(w, y, font_clr, "%c", ((option.IsVibration())? 'O': 'X'));
	y++;
	//ボリューム・BGM
	font_clr = (((m_crsr==EElement::VolumeBgm))? crnt: clr);
	cb.Printc(x, y, font_clr, "Volume/ Bgm");
	cb.Printc(w, y, font_clr, "%d", option.GetVolumeBgm());
	y++;
	//ボリューム・SE
	font_clr = (((m_crsr==EElement::VolumeSe))? crnt: clr);
	cb.Printc(x, y, font_clr, "Volume/ Se");
	cb.Printc(w, y, font_clr, "%d", option.GetVolumeSe());
	y++;
#if _IPHONE
	//コントロール方法
	font_clr = (((m_crsr==EElement::Control))? crnt: clr);
	cb.Printc(x, y, font_clr, "Control");
	cb.Printc(w, y, font_clr, "%s", control_str[option.GetControl()]);
	y++;
#endif //_IPHONE
#if _WII
	//ユーザー名
	font_clr = (((m_crsr==EElement::UserName))? crnt: clr);
	cb.Printc(x, y, font_clr, "User name");
	cb.Printc(w, y, font_clr, "%s", option.GetName());
	y++;
#endif // _WII
}


//------------------------------------------------------------------------------**********





































































//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// =============================================================================
// CIndexSelectEditor::Focus
/*!
	選択中の処理

	@param	cb	[io]	コールバックデータ

	@retval	true	終了
	@retval	false	継続
 */
// ==========================================================================
bool CIndexSelectEditor::Focus(CEvtBase &cb)
{
	bool result = update(cb);
	print(cb, true);
	return result;
}

// ============================================================================
// CIndexSelectEditor::Blur
/*!
	非選択中の処理

	@param	cb	[io]	コールバックデータ
 */
// ============================================================================
void CIndexSelectEditor::Blur(CEvtBase &cb)
{
	print(cb, false);
}

// =============================================================================
// CIndexSelectEditor::Full
/*!
	最強化

	@param	cb	[io]	コールバックデータ
 */
// ==========================================================================
void CIndexSelectEditor::Full(CEvtBase &)
{
}

// ============================================================================
// CIndexSelectEditor::Reset
/*!
	白紙化

	@param	cb	[io]	コールバックデータ
 */
// ============================================================================
void CIndexSelectEditor::Reset(CEvtBase &)
{
	using namespace gs::backup;
	SBackup &backup = SBackup::CreateInstance();
	backup.SetSaveIndex(0);
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// ============================================================================
// CIndexSelectEditor::CIndexSelectEditor
/*!
	デフォルトコンストラクタ
 */
// ============================================================================
CIndexSelectEditor::CIndexSelectEditor() : super_type() {
	m_crsr.SetRange(0, EElement::Max);
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// =============================================================================
// CIndexSelectEditor::update
/*!
	選択中の処理

	@param	cb	[io]	コールバックデータ

	@retval	true	終了
	@retval	false	継続
 */
// ==========================================================================
bool CIndexSelectEditor::update(CEvtBase &cb)
{
	bool result = false;

	//カーソル移動
	if (cb.IsPadDirect(GSD_KEY_UP)) {
		if (cb.IsPadRepeat(GSD_KEY_UP) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
			--m_crsr;
		}
	} else if (cb.IsPadDirect(GSD_KEY_DOWN)) {
		if (cb.IsPadRepeat(GSD_KEY_DOWN) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
			++m_crsr;
		}
	}

	using namespace gs::backup;
	SBackup &backup = SBackup::CreateInstance();

	switch (m_crsr) {
	case EElement::SaveIndex:	//セーブインデックス
		{
			er::CSmallInt<Uint32> val(0, SBackup::c_save_size);
			val = backup.GetSaveIndex();
			if (cb.IsPadDirect(GSD_KEY_LEFT)) {
				if (cb.IsPadRepeat(GSD_KEY_LEFT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val -= ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			} else if (cb.IsPadDirect(GSD_KEY_RIGHT)) {
				if (cb.IsPadRepeat(GSD_KEY_RIGHT) || cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					val += ((cb.IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1)))? 10u: 1u);
				}
			}
			if (cb.IsPadDirect(CEvtBase::TPadKey(GSD_KEY_LEFT | GSD_KEY_RIGHT))) {
				backup.SetSaveIndex(val);
			}
		}
		break;
	}

	if (cb.IsPadStand(GSD_KEY_CANCEL)) {
		result = true;
	}

	return result;
}

// ============================================================================
// CIndexSelectEditor::print
/*!
	表示

	@param	cb			[io]	コールバックデータ
	@param	is_enable	[in]	有効判定
 */
// ============================================================================
void CIndexSelectEditor::print(CEvtBase &cb, bool is_enable)
{
	int x, y;
	CEvtBase::EColor::Type clr = ((is_enable)? CEvtBase::EColor::White: CEvtBase::EColor::Gray);
	CEvtBase::EColor::Type crnt = ((is_enable)? CEvtBase::EColor::Yellow: CEvtBase::EColor::DarkYellow);
	CEvtBase::EColor::Type	font_clr;

	//操作説明
	x = 38;
	y = 5;
	cb.Printc(x, y++, clr, "%c         : Return", GsEnvDebugGetCancelKeyChar());
	cb.Printc(x, y++, clr, "Up/Down   : Select  (L/R:SpeedUp)");
	cb.Printc(x, y++, clr, "Left/Right: Change  (L/R:SpeedUp)");

	using namespace gs::backup;
	SBackup &backup = SBackup::CreateInstance();

	//項目
	y += 2;
	int w = x + 16;

	//カーソル
	cb.Printc(x-1, y+m_crsr.GetVal<int>(), crnt, ">");

	//セーブインデックス
	font_clr = (((m_crsr==EElement::SaveIndex))? crnt: clr);
	cb.Printc(x, y, font_clr, "Save Index");
	cb.Printc(w, y, font_clr, "%d", backup.GetSaveIndex());
	y++;
}


//------------------------------------------------------------------------------**********
































































//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// CSaveDataEditor::Focus
/*!
	選択されている時に実行される関数
 */
// ============================================================================
void CSaveDataEditor::Focus() {
	if (0 < m_list.size()) {
		if (m_is_exe) {
			//エディタの実行中
			printTitle(false);

			bool result = m_list[m_crsr.GetVal()]->Focus(*this);
			if (result) {
				m_is_exe = false;
			}
		} else {
			//エディタ未実行
			if (IsPadRepeat(GSD_KEY_UP)) {
				--m_crsr;
			} else if (IsPadRepeat(GSD_KEY_DOWN)) {
				++m_crsr;
			}
			if (IsPadStand(GSD_KEY_DECIDE)) {
				m_is_exe = true;
			} else if (IsPadDirect(KEY_START)) {
				if (!IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					//単体
					m_list[m_crsr.GetVal()]->Full(*this);
				} else {
					//全項目
					for (TEditerList::iterator ite = m_list.begin(), ite_end = m_list.end(); ite != ite_end; ++ite) {
						(*ite)->Full(*this);
					}
				}
			} else if (IsPadDirect(KEY_SELECT)) {
				if (!IsPadDirect(CEvtBase::TPadKey(KEY_L1 | KEY_R1))) {
					//単体
					m_list[m_crsr.GetVal()]->Reset(*this);
				} else {
					//全項目
					for (TEditerList::iterator ite = m_list.begin(), ite_end = m_list.end(); ite != ite_end; ++ite) {
						(*ite)->Reset(*this);
					}
				}
			}

			printTitle(true);
			m_list[m_crsr.GetVal()]->Blur(*this);
		}
	}
}

// ============================================================================
// CSaveDataEditor::MoveEvent
/*!
	イベント間の移動を判定する関数

	@retval	0		移動しない
	@retval	1～		次のイベントへ
	@retval	～-1	前のイベントへ

	@note
		デフォルトでは ←・→ が移動操作になります
 */
// ============================================================================
CSaveDataEditor::TMoveDirect CSaveDataEditor::MoveEvent()
{
	TMoveDirect direct = TMoveDirect(0);
	if (!m_is_exe) {
		direct = super_type::MoveEvent();
	}
	return direct;
}



//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// ============================================================================
// CSaveDataEditor::CSaveDataEditor
/*!
	デフォルトコンストラクタ

	@param	ti	[in]	タイトル
	@param	ev	[in]	イベントID
	@param	fl	[in]	フラグ
 */
// ============================================================================
CSaveDataEditor::CSaveDataEditor(const char *title, GSE_EVT_ID evt_id, int flag)
		: super_type(title, evt_id, flag)
{
	//エディタの登録
	m_list.push_back(&m_system);
	m_list.push_back(&m_stage);
	m_list.push_back(&m_special);
	m_list.push_back(&m_option);
#if _WII
	m_list.push_back(&m_save_index);
#endif //_WII
	m_list.push_back(&m_io);

	//その他の初期化
	int max = int((0 < m_list.size())? m_list.size(): 0);
	m_crsr.SetRange(0, max);
	m_is_exe = false;
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// ============================================================================
// CSaveDataEditor::printTitle
/*!
	タイトルの表示

	@param	is_enable	[in]	有効判定
 */
// ============================================================================
void CSaveDataEditor::printTitle(bool is_enable)
{
	if (0 < m_list.size()) {
		int x, y, w, h;
		CEvtBase::EColor::Type clr = ((is_enable)? CEvtBase::EColor::White: CEvtBase::EColor::Gray);
		//操作説明
		x = 3;
		y = 12;
		Printc(x, y++, clr, "%c       : Enter", GsEnvDebugGetDecideKeyChar());
		Printc(x, y++, clr, "Up/Down : Select");
		Printc(x, y++, clr, "Start   : FullPower (L/R:All)");
		Printc(x, y++, clr, "Select  : Reset     (L/R:All)");

		//選択項目
		x = 4;
		y = 17;
		w = 14;
		h = 30;
		Printc(x-1+(m_crsr/h)*w, y+(m_crsr%h), clr, ">");

		std::for_each(m_list.begin(), m_list.end(), CTitlePrint(*this, x, y, w, h, clr));
	}
}


//------------------------------------------------------------------------------**********












} //namespace dbg
#endif //defined(AMD_DEBUG)
// =============================================================================
// Function
/*!
	関数の説明

	@param	org1	[io]	引数１の説明
	@param	org2	[in]	引数２の説明
	@param	org3	[out]	引数３の説明

	@return	戻り値の説明
		or
	@retval	0	正常
	@retval	!0	異常

	@exception 例外
 
	@note
		補足説明
 */
// ==========================================================================
