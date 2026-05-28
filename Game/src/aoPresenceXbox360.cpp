// ===========================================================================
/*!
	@file	aoPresence.cpp
	@brief	プレゼンスシステム定義(Xbox360)

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "aoPresence.h"
#include "ao.h"

#if _XBOX

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

namespace ao {

// ===========================================================================
//	class CPresence
// ---------------------------------------------------------------------------
//!	プレゼンスクラス
// ===========================================================================
class CPresence :
	public CTask<CPresence>, public CThread<CPresence>, public CAllocAmNormal
{
public:

	//! コンストラクタ
	CPresence(AOE_PRESENCE presence, BOOL is_trial);

	//! デストラクタ
	virtual ~CPresence();

	//! プレゼンス変更通知
	void Change(AOE_PRESENCE presence, BOOL is_trial);

protected:

	// タスクプロシージャ
	void TaskProcReady();
	void TaskProcWait();

	// スレッドプロシージャ
	void ThreadProcSetPresence();

	AOE_PRESENCE	m_set_presence;	//!< 設定するプレゼンス
	BOOL			m_set_is_trial;	//!< 設定するプレゼンス体験版フラグ
	AOE_PRESENCE	m_req_presence;	//!< リクエスト中のプレゼンス
	BOOL			m_req_is_trial;	//!< リクエスト中のプレゼンス体験版フラグ
};

} // namespace ao

// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct AOS_PRESENCE_PARAM
// ---------------------------------------------------------------------------
//!	プレゼンスパラメータ
// ===========================================================================
typedef struct tag_AOS_PRESENCE_PARAM {
	s8	presence;		//!< X_CONTEXT_PRESENCE
	s8	mode;			//!< X_CONTEXT_GAME_MODE
	s8	stage;			//!< CONTEXT_GAME_STAGE
	s8	act_deco;		//!< CONTEXT_PRESENCE_ACT_DECO
	s8	boss_deco;		//!< CONTEXT_PRESENCE_BOSS_DECO
	s8	ss_deco;		//!< CONTEXT_PRESENCE_SS_DECO
	s8	stage_no;		//!< PROPERTY_PRESENCE_STAGE_NO
} AOS_PRESENCE_PARAM;

// ===========================================================================
//	struct AOS_PRESENCE_GLOBAL
// ---------------------------------------------------------------------------
//!	グローバルパラメータ
// ===========================================================================
typedef struct tag_AOS_PRESENCE_GLOBAL {
	AOE_PRESENCE		now_presence;	//!< 現在のプレゼンス
	AOS_PRESENCE_PARAM	now_param;		//!< 現在の設定
	ao::CPresence*		system;			//!< プレゼンスクラス
	BOOL				is_idle_trial;	//!< 真：体験版
	XUID				idle_xuid[4];	//!< アイドル状態設定オンラインXUID
} AOS_PRESENCE_GLOBAL;

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//	AOS_PRESENCE_GLOBAL g_ao_presence_glb
// ---------------------------------------------------------------------------
//!	グローバルパラメータ
// ===========================================================================
static AOS_PRESENCE_GLOBAL g_ao_presence_glb = {
	AOD_PRESENCE_DEFAULT,
	{ -1, -1, -1, -1, -1, -1, -1},
	NULL,
	FALSE,
	{ INVALID_XUID, INVALID_XUID, INVALID_XUID, INVALID_XUID },
};

// ===========================================================================
//	AOS_PRESENCE_PARAM g_ao_presence_param_tbl[]
// ---------------------------------------------------------------------------
//!	プレゼンスパラメータ配列
// ===========================================================================
static const AOS_PRESENCE_PARAM g_ao_presence_param_tbl[AOD_PRESENCE_NUM] = {
	// 待機中(メニューなど)
	{ CONTEXT_PRESENCE_PRESENCE_STANDBY, CONTEXT_GAME_MODE_STANDBY, -1, -1, -1, -1, -1 },

	// Zone1-1タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE1, CONTEXT_PRESENCE_ACT_DECO_Z1, -1, -1, 1 },
	// Zone1-1スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE1, CONTEXT_PRESENCE_ACT_DECO_Z1, -1, -1, 1 },
	// Zone1-2タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE1, CONTEXT_PRESENCE_ACT_DECO_Z1, -1, -1, 2 },
	// Zone1-2スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE1, CONTEXT_PRESENCE_ACT_DECO_Z1, -1, -1, 2 },
	// Zone1-3タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE1, CONTEXT_PRESENCE_ACT_DECO_Z1, -1, -1, 3 },
	// Zone1-3スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE1, CONTEXT_PRESENCE_ACT_DECO_Z1, -1, -1, 3 },
	// Zone1-Bタイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_BOSS, CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE1, -1, CONTEXT_PRESENCE_BOSS_DECO_Z1, -1, -1 },
	// Zone1-Bスコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_BOSS, CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE1, -1, CONTEXT_PRESENCE_BOSS_DECO_Z1, -1, -1 },

	// Zone2-1タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE2, CONTEXT_PRESENCE_ACT_DECO_Z2, -1, -1, 1 },
	// Zone2-1スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE2, CONTEXT_PRESENCE_ACT_DECO_Z2, -1, -1, 1 },
	// Zone2-2タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE2, CONTEXT_PRESENCE_ACT_DECO_Z2, -1, -1, 2 },
	// Zone2-2スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE2, CONTEXT_PRESENCE_ACT_DECO_Z2, -1, -1, 2 },
	// Zone2-3タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE2, CONTEXT_PRESENCE_ACT_DECO_Z2, -1, -1, 3 },
	// Zone2-3スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE2, CONTEXT_PRESENCE_ACT_DECO_Z2, -1, -1, 3 },
	// Zone2-Bタイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_BOSS, CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE2, -1, CONTEXT_PRESENCE_BOSS_DECO_Z2, -1, -1 },
	// Zone2-Bスコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_BOSS, CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE2, -1, CONTEXT_PRESENCE_BOSS_DECO_Z2, -1, -1 },

	// Zone3-1タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE3, CONTEXT_PRESENCE_ACT_DECO_Z3, -1, -1, 1 },
	// Zone3-1スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE3, CONTEXT_PRESENCE_ACT_DECO_Z3, -1, -1, 1 },
	// Zone3-2タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE3, CONTEXT_PRESENCE_ACT_DECO_Z3, -1, -1, 2 },
	// Zone3-2スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE3, CONTEXT_PRESENCE_ACT_DECO_Z3, -1, -1, 2 },
	// Zone3-3タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE3, CONTEXT_PRESENCE_ACT_DECO_Z3, -1, -1, 3 },
	// Zone3-3スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE3, CONTEXT_PRESENCE_ACT_DECO_Z3, -1, -1, 3 },
	// Zone3-Bタイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_BOSS, CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE3, -1, CONTEXT_PRESENCE_BOSS_DECO_Z3, -1, -1 },
	// Zone3-Bスコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_BOSS, CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE3, -1, CONTEXT_PRESENCE_BOSS_DECO_Z3, -1, -1 },

	// Zone4-1タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE4, CONTEXT_PRESENCE_ACT_DECO_Z4, -1, -1, 1 },
	// Zone4-1スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE4, CONTEXT_PRESENCE_ACT_DECO_Z4, -1, -1, 1 },
	// Zone4-2タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE4, CONTEXT_PRESENCE_ACT_DECO_Z4, -1, -1, 2 },
	// Zone4-2スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE4, CONTEXT_PRESENCE_ACT_DECO_Z4, -1, -1, 2 },
	// Zone4-3タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE4, CONTEXT_PRESENCE_ACT_DECO_Z4, -1, -1, 3 },
	// Zone4-3スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_ACT,  CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE4, CONTEXT_PRESENCE_ACT_DECO_Z4, -1, -1, 3 },
	// Zone4-Bタイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_BOSS, CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONE4, -1, CONTEXT_PRESENCE_BOSS_DECO_Z4, -1, -1 },
	// Zone4-Bスコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_BOSS, CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONE4, -1, CONTEXT_PRESENCE_BOSS_DECO_Z4, -1, -1 },

	// FinalZoneタイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_BOSS, CONTEXT_GAME_MODE_TIME_ATTACK,  CONTEXT_GAME_STAGE_ZONEF, -1, CONTEXT_PRESENCE_BOSS_DECO_ZF, -1, -1 },
	// FinalZoneスコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_BOSS, CONTEXT_GAME_MODE_SCORE_ATTACK, CONTEXT_GAME_STAGE_ZONEF, -1, CONTEXT_PRESENCE_BOSS_DECO_ZF, -1, -1 },

	// SpecialStage1タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_SS,  CONTEXT_GAME_MODE_TIME_ATTACK,  -1, -1, -1, CONTEXT_PRESENCE_SS_DECO_SS1, 1 },
	// SpecialStage1スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_SS,  CONTEXT_GAME_MODE_SCORE_ATTACK, -1, -1, -1, CONTEXT_PRESENCE_SS_DECO_SS1, 1 },
	// SpecialStage2タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_SS,  CONTEXT_GAME_MODE_TIME_ATTACK,  -1, -1, -1, CONTEXT_PRESENCE_SS_DECO_SS2, 2 },
	// SpecialStage2スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_SS,  CONTEXT_GAME_MODE_SCORE_ATTACK, -1, -1, -1, CONTEXT_PRESENCE_SS_DECO_SS2, 2 },
	// SpecialStage3タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_SS,  CONTEXT_GAME_MODE_TIME_ATTACK,  -1, -1, -1, CONTEXT_PRESENCE_SS_DECO_SS3, 3 },
	// SpecialStage3スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_SS,  CONTEXT_GAME_MODE_SCORE_ATTACK, -1, -1, -1, CONTEXT_PRESENCE_SS_DECO_SS3, 3 },
	// SpecialStage4タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_SS,  CONTEXT_GAME_MODE_TIME_ATTACK,  -1, -1, -1, CONTEXT_PRESENCE_SS_DECO_SS4, 4 },
	// SpecialStage4スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_SS,  CONTEXT_GAME_MODE_SCORE_ATTACK, -1, -1, -1, CONTEXT_PRESENCE_SS_DECO_SS4, 4 },
	// SpecialStage5タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_SS,  CONTEXT_GAME_MODE_TIME_ATTACK,  -1, -1, -1, CONTEXT_PRESENCE_SS_DECO_SS5, 5 },
	// SpecialStage5スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_SS,  CONTEXT_GAME_MODE_SCORE_ATTACK, -1, -1, -1, CONTEXT_PRESENCE_SS_DECO_SS5, 5 },
	// SpecialStage6タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_SS,  CONTEXT_GAME_MODE_TIME_ATTACK,  -1, -1, -1, CONTEXT_PRESENCE_SS_DECO_SS6, 6 },
	// SpecialStage6スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_SS,  CONTEXT_GAME_MODE_SCORE_ATTACK, -1, -1, -1, CONTEXT_PRESENCE_SS_DECO_SS6, 6 },
	// SpecialStage7タイムアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_SS,  CONTEXT_GAME_MODE_TIME_ATTACK,  -1, -1, -1, CONTEXT_PRESENCE_SS_DECO_SS7, 7 },
	// SpecialStage7スコアアタックプレイ中
	{ CONTEXT_PRESENCE_PRESENCE_SS,  CONTEXT_GAME_MODE_SCORE_ATTACK, -1, -1, -1, CONTEXT_PRESENCE_SS_DECO_SS7, 7 },
};

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//	AoPresenceInit
/*!
	プレゼンスシステム初期化開始

	@note
	アプリケーション起動時に一度だけ呼び出して下さい。\n
*/
// ===========================================================================
void AoPresenceInit(void)
{
	// グローバルパラメータ初期化
	g_ao_presence_glb.now_presence = AOD_PRESENCE_DEFAULT;
	g_ao_presence_glb.system = NULL;
}

// ===========================================================================
//	AoPresenceInit
/*!
	プレゼンスシステム初期化終了判定

	@return 真：初期化終了済み　偽：それ以外
	@note
	AoPresenceInit関数で開始した終了処理が完了したかどうかを判定します。\n
*/
// ===========================================================================
BOOL AoPresenceInitialized(void)
{
	// empty
	return TRUE;
}

// ===========================================================================
//	AoPresenceInit
/*!
	プレゼンスシステム終了処理

	@note
	アプリケーション終了時に一度だけ呼び出して下さい。\n
*/
// ===========================================================================
void AoPresenceExit(void)
{
	if (g_ao_presence_glb.system) {
		delete g_ao_presence_glb.system;
		g_ao_presence_glb.system = NULL;
	}
}

// ===========================================================================
//	AoPresenceSet
/*!
	プレゼンス設定

	@param presence	[in] 設定するプレゼンス
	@param is_trial	[in] 真：体験版　偽：製品版
	@note
	フレンドなどに公開する現在プレイ中のユーザのプレゼンスを設定します。\n
	この関数はゲーム中何度でも呼び出し可能です。\n
	既に同じプレゼンスが設定されている場合は何も行ないません。\n
	無効な引数を指定した場合は、アサートしAOD_PRESENCE_STANDBYを設定します。\n
*/
// ===========================================================================
void AoPresenceSet(AOE_PRESENCE presence, BOOL is_trial)
{
	if ((u32)presence >= (u32)AOD_PRESENCE_NUM) {
		amAssert(0);
		presence = AOD_PRESENCE_STANDBY;
	}
	// 設定
	g_ao_presence_glb.now_presence = presence;
	if (g_ao_presence_glb.system) {
		g_ao_presence_glb.system->Change(presence, is_trial);
	}
	else {
		g_ao_presence_glb.system = new ao::CPresence(presence, is_trial);
	}
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace ao {

// ***************************************************************************
// プレゼンスクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CPresence::CPresence(AOE_PRESENCE presence, BOOL is_trial)
{
	// パラメータ設定
	m_set_presence = AOD_PRESENCE_NONE;
	m_set_is_trial = FALSE;
	m_req_presence = presence;
	m_req_is_trial = is_trial;

	// タスク作成
	MakeTask(0, "aoPresence");

	// タスクプロシージャ設定
	SetTaskProc(0, &CPresence::TaskProcReady);

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CPresence::~CPresence()
{
	g_ao_presence_glb.system = NULL;
}


// ===========================================================================
//! プレゼンス変更通知
// ===========================================================================
void CPresence::Change(AOE_PRESENCE presence, BOOL is_trial)
{
	if ((presence != m_set_presence) || (is_trial != m_set_is_trial)) {
		m_req_presence = presence;
		m_req_is_trial = is_trial;
	}
}


// ***************************************************************************
// タスクプロシージャ
// ***************************************************************************
// ===========================================================================
//! 準備
// ===========================================================================
void CPresence::TaskProcReady()
{
	// リクエストがないなら終了
	if ((u32)m_req_presence >= AOD_PRESENCE_NUM) {
		delete this;
		return;
	}

	// リクエストを設定
	m_set_presence = m_req_presence;
	m_set_is_trial = m_req_is_trial;
	m_req_presence = AOD_PRESENCE_NONE;
	m_req_is_trial = FALSE;

	// スレッド作成
	SetThreadProc(0, &CPresence::ThreadProcSetPresence);
	StartThread(0, (AMD_CORE)0, (u32)THREAD_PRIORITY_BELOW_NORMAL);

	// 設定完了待ちへ遷移
	SetOwnTaskProc(&CPresence::TaskProcWait);
}

// ===========================================================================
//! 設定完了待ち
// ===========================================================================
void CPresence::TaskProcWait()
{
	// スレッド終了判定
	if (IsEndThread(0)) {
		// 準備へ遷移
		SetOwnTaskProc(&CPresence::TaskProcReady);
	}
}


// ***************************************************************************
// スレッドプロシージャ
// ***************************************************************************
// ===========================================================================
//! 設定
// ===========================================================================
void CPresence::ThreadProcSetPresence()
{
	// 現在の設定取得
	AOS_PRESENCE_PARAM& now_param = g_ao_presence_glb.now_param;

	// パラメータ取得
	AOS_PRESENCE_PARAM param = g_ao_presence_param_tbl[m_set_presence];
	if (m_set_is_trial) {
		param.presence = CONTEXT_PRESENCE_PRESENCE_TRIAL;
		param.mode = -1;
		param.stage = -1;
		param.act_deco = -1;
		param.boss_deco = -1;
		param.ss_deco = -1;
		param.stage_no = -1;
	}

	u32 uid = (u32)AoAccountGetCurrentId();
	if (uid >= 4) {
		goto idle;
	}

	// 終了判定
	if (IsRequestEndThread()) {
		return;
	}
	// アカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		goto idle;
	}

	// X_CONTEXT_GAME_MODE設定
	if ((param.mode >= 0) && (param.mode != now_param.mode)) {
		XUserSetContext(
			(DWORD)uid,
			(DWORD)X_CONTEXT_GAME_MODE,
			(DWORD)param.mode);
		now_param.mode = param.mode;
	}

	// 終了判定
	if (IsRequestEndThread()) {
		return;
	}
	// アカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		goto idle;
	}

	// CONTEXT_GAME_STAGE設定
	if ((param.stage >= 0) && (param.stage != now_param.stage)) {
		XUserSetContext(
			(DWORD)uid,
			(DWORD)CONTEXT_GAME_STAGE,
			(DWORD)param.stage); 
		now_param.stage = param.stage;
		amSystemLog("aoPresenceNotice : %d stage%d\n", uid, param.stage);
	}

	// 終了判定
	if (IsRequestEndThread()) {
		return;
	}
	// アカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		goto idle;
	}

	// CONTEXT_PRESENCE_ACT_DECO設定
	if ((param.act_deco >= 0) && (param.act_deco != now_param.act_deco)) {
		XUserSetContext(
			(DWORD)uid,
			(DWORD)CONTEXT_PRESENCE_ACT_DECO,
			(DWORD)param.act_deco);
		now_param.act_deco = param.act_deco;
		amSystemLog("aoPresenceNotice : %d act_deco%d\n", uid, param.act_deco);
	}

	// 終了判定
	if (IsRequestEndThread()) {
		return;
	}
	// アカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		goto idle;
	}

	// CONTEXT_PRESENCE_BOSS_DECO設定
	if ((param.boss_deco >= 0) && (param.boss_deco != now_param.boss_deco)) {
		XUserSetContext(
			(DWORD)uid,
			(DWORD)CONTEXT_PRESENCE_BOSS_DECO,
			(DWORD)param.boss_deco);
		now_param.boss_deco = param.boss_deco;
		amSystemLog(
			"aoPresenceNotice : %d boss_deco%d\n", uid, param.boss_deco);
	}

	// 終了判定
	if (IsRequestEndThread()) {
		return;
	}
	// アカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		goto idle;
	}

	// CONTEXT_PRESENCE_SS_DECO設定
	if ((param.ss_deco >= 0) && (param.ss_deco != now_param.ss_deco)) {
		XUserSetContext(
			(DWORD)uid,
			(DWORD)CONTEXT_PRESENCE_SS_DECO,
			(DWORD)param.ss_deco);
		now_param.ss_deco = param.ss_deco;
		amSystemLog("aoPresenceNotice : %d ss_deco%d\n", uid, param.ss_deco);
	}

	// 終了判定
	if (IsRequestEndThread()) {
		return;
	}
	// アカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		goto idle;
	}

	// PROPERTY_PRESENCE_STAGE_NO設定
	if ((param.stage_no >= 0) && (param.stage_no != now_param.stage_no)) {
		DWORD value = (DWORD)param.stage_no;
		XUserSetProperty(
			(DWORD)uid,
			(DWORD)PROPERTY_PRESENCE_STAGE_NO,
			sizeof(value),
			&value);
		now_param.stage_no = param.stage_no;
		amSystemLog("aoPresenceNotice : %d stage_no%d\n", uid, param.stage_no);
	}

	// 終了判定
	if (IsRequestEndThread()) {
		return;
	}
	// アカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		goto idle;
	}

	// X_CONTEXT_PRESENCE設定
	if ((param.presence >= 0) && (param.presence != now_param.presence)) {
		XUserSetContext(
			(DWORD)uid,
			(DWORD)X_CONTEXT_PRESENCE,
			(DWORD)param.presence);
		now_param.presence = param.presence;
		amSystemLog("aoPresenceNotice : %d presence%d\n", uid, param.presence);
	}

idle:

	// カレントアカウントが無効なら現在の設定をクリア
	if (((u32)AoAccountGetCurrentId() >= 4) ||
		!AoAccountIsCurrentEnableRealXbox360())
	{
		now_param.presence = -1;
		now_param.mode = -1;
		now_param.stage = -1;
		now_param.act_deco = -1;
		now_param.boss_deco = -1;
		now_param.ss_deco = -1;
		now_param.stage_no = -1;
	}

	if (( m_set_is_trial && !g_ao_presence_glb.is_idle_trial) ||
		(!m_set_is_trial &&  g_ao_presence_glb.is_idle_trial))
	{
		// お試し版と完全版が切り替わったので
		// 待機中プレゼンス再設定
		g_ao_presence_glb.is_idle_trial = m_set_is_trial;
		for (u32 i = 0; i < 4; ++i) {
			g_ao_presence_glb.idle_xuid[i] = INVALID_XUID;
		}
	}

	// 待機状態プレゼンス設定
	for (u32 i = 0; i < 4; ++i) {

		DWORD idle_presence = CONTEXT_PRESENCE_PRESENCE_IDLE;
		if (m_set_is_trial) {
			idle_presence = CONTEXT_PRESENCE_PRESENCE_TRIALIDLE;
		}

		// 終了判定
		if (IsRequestEndThread()) {
			return;
		}

		// 有効なカレントアカウントは除外
		if (i == (u32)AoAccountGetCurrentId()) {
			if (AoAccountIsCurrentEnableRealXbox360()) {
				g_ao_presence_glb.idle_xuid[i] = INVALID_XUID;
				continue;
			}
		}

		// オンライン状態でのみ処理
		if (XUserGetSigninState(i) != eXUserSigninState_SignedInToLive) {
			g_ao_presence_glb.idle_xuid[i] = INVALID_XUID;
			continue;
		}

		// サインイン情報取得
		XUSER_SIGNIN_INFO info;
		DWORD result = XUserGetSigninInfo(
			i, XUSER_GET_SIGNIN_INFO_ONLINE_XUID_ONLY, &info);
		if (result != ERROR_SUCCESS) {
			g_ao_presence_glb.idle_xuid[i] = INVALID_XUID;
			continue;
		}

		// XUID比較
		if (IsEqualXUID(info.xuid, g_ao_presence_glb.idle_xuid[i])) {
			continue;
		}
		g_ao_presence_glb.idle_xuid[i] = info.xuid;

		// 無効なXUIDなら除外
		if (g_ao_presence_glb.idle_xuid[i] == INVALID_XUID) {
			continue;
		}

		// プレゼンス設定
		XUserSetContext(
			(DWORD)i,
			(DWORD)X_CONTEXT_PRESENCE,
			(DWORD)idle_presence);
		amSystemLog("aoPresenceNotice : %d idle\n", i);
	}
}

} // namespace ao

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
