// ===========================================================================
/*!
	@file	aoAvatarAward.cpp
	@brief	アバターアワードモジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "aoAvatarAward.h"
#include "ao.h"

#if _XBOX

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

namespace ao {
namespace avataraward {

// ===========================================================================
//	class CGet
// ---------------------------------------------------------------------------
//!	アバターアワード取得クラス
// ===========================================================================
class CGet :
	public CProc<CGet>, public CTask<CGet>, public CThread<CGet>,
	public CAllocAmNormal
{
public:

	//! コンストラクタ
	CGet(AOE_AVATARITEM item);

	//! デストラクタ
	virtual ~CGet();

	//! 取得アワード追加
	void Add(AOE_AVATARITEM item);

protected:

	// タスクプロシージャ
	void TaskProcMain();

	// プロシージャ
	void ProcReady();
	void ProcWait();

	// スレッドプロシージャ
	void ThreadProcGet();

	// 定数
	enum {
		QUE_NUM = 4,	//!< キュー数
	};

	AOE_AVATARITEM m_item_que[QUE_NUM]; //!< リクエストキュー
	AOE_AVATARAWARD_RESULT m_result; //!< 結果
};

} // namespace avataraward
} // namespace ao

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//	ao::avataraward::CGet* g_ao_avataraward_get
// ---------------------------------------------------------------------------
//!	アバターアワード取得クラス
// ===========================================================================
static ao::avataraward::CGet* g_ao_avataraward_get = NULL;

// ===========================================================================
//	AOE_AVATARAWARD_RESULT g_ao_avataraward_result
// ---------------------------------------------------------------------------
//!	アバターアワード処理結果
// ===========================================================================
static AOE_AVATARAWARD_RESULT g_ao_avataraward_result =
	AOD_AVATARAWARD_RESULT_SUCCESS;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//	AoAvatarAwardGetStart
/*!
	アバターアイテム取得開始

	@param item		[in] 取得するアバターアイテム
	@note
	アバターアイテムの取得処理を非同期で開始します。\n
	終了判定はAoAvatarAwardIsGetEnd関数で行なって下さい。\n
	成功判定はAoAvatarAwardIsGetSuccess関数で行なって下さい。\n
	同じアバターアイテムの取得処理が行なわれている状態で呼び出した場合は
	何も行ないません。\n
	別のアバターアイテムの取得処理が行なわれている状態で呼び出した場合は
	別のアバターアイテムの取得処理が終了してから
	指定のアバターアイテムの取得処理を行ないます。\n
*/
// ===========================================================================
void AoAvatarAwardGetStart(AOE_AVATARITEM item)
{
	if (g_ao_avataraward_get) {
		g_ao_avataraward_get->Add(item);
	}
	else {
		g_ao_avataraward_get = new ao::avataraward::CGet(item);
	}
}

// ===========================================================================
//	AoAvatarAwardGetIsEnd
/*!
	アバターアイテム取得完了判定

	@return 真：完了済み　偽：それ以外
	@note
	最後に呼び出したAoAvatarAwardGetStart関数で開始した
	アバターアイテム取得処理が完了したかどうかを判定します。\n
*/
// ===========================================================================
BOOL AoAvatarAwardGetIsEnd(void)
{
	if (g_ao_avataraward_get) {
		return FALSE;
	}
	return TRUE;
}

// ===========================================================================
//	AoAvatarAwardGetResult
/*!
	アバターアワード処理結果取得

	@return アバターアワード処理結果
	@note
	直前に行なわれたアバターアワード処理の結果を返します。\n
*/
// ===========================================================================
AOE_AVATARAWARD_RESULT AoAvatarAwardGetResult(void)
{
	return g_ao_avataraward_result;
}

// ===========================================================================
//	AoAvatarAwardGetResult
/*!
	アバターアワード処理強制終了

	@note
	アバターアワード関連の処理が行なわれている場合は、
	処理の強制終了を行ないます。\n
*/
// ===========================================================================
void AoAvatarAwardTerminate(void)
{
	if (g_ao_avataraward_get) {
		delete g_ao_avataraward_get;
		g_ao_avataraward_get = NULL;
	}
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace ao {
namespace avataraward {

// ***************************************************************************
// アバターアワード取得クラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CGet::CGet(AOE_AVATARITEM item)
{
	// メンバ初期化
	m_item_que[0] = item;
	for (u32 i = 1; i < (u32)QUE_NUM; ++i) {
		m_item_que[i] = AOD_AVATARITEM_NONE;
	}

	// タスク作成
	MakeTask(0, "aoAvatarAward::Get");

	// タスクプロシージャ設定
	SetTaskProc(0, &CGet::TaskProcMain);

	// プロシージャ設定
	SetProc(0, &CGet::ProcReady);

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CGet::~CGet()
{
	g_ao_avataraward_get = NULL;
}

// ===========================================================================
//! 取得アワード追加
// ===========================================================================
void CGet::Add(AOE_AVATARITEM item)
{
	// キューに積む
	for (u32 i = 0; i < QUE_NUM; ++i) {
		if ((u32)m_item_que[i] >= AOD_AVATARITEM_NUM) {
			m_item_que[i] = item;
			break;
		}
		if (m_item_que[i] == item) {
			break;
		}
	}
}


// ***************************************************************************
// タスクプロシージャ
// ***************************************************************************
// ===========================================================================
//! メインタスク
// ===========================================================================
void CGet::TaskProcMain()
{
	// 実行
	Call(0);

	// プロシージャ確認
	if (IsProcNone(0)) {
		// 終了
		DeleteOwnTask();
		delete this;
	}

	// ↑ここでは、かならずプロシージャ実行直後にタスク削除判定を行うこと
	// そうしないと、Addで積まれたキューが実行されずに終了してしまう場合がある
}


// ***************************************************************************
// プロシージャ
// ***************************************************************************
// ===========================================================================
//! 準備
// ===========================================================================
void CGet::ProcReady()
{
	// 終了判定
	if ((u32)m_item_que[0] >= AOD_AVATARITEM_NUM) {
		SetOwnProcNone();
		return;
	}

	// 結果クリア
	m_result = AOD_AVATARAWARD_RESULT_SUCCESS;

	// スレッド作成
	SetThreadProc(0, &CGet::ThreadProcGet);
	StartThread(0, (AMD_CORE)0, (u32)THREAD_PRIORITY_BELOW_NORMAL);

	// 取得待ちへ遷移
	SetOwnProc(&CGet::ProcWait);
}

// ===========================================================================
//! 取得待ち
// ===========================================================================
void CGet::ProcWait()
{
	// スレッド終了待ち
	if (IsEndThread(0)) {

		// 結果設定
		g_ao_avataraward_result = m_result;

		// キュー更新
		for (u32 i = 0; i < (u32)(QUE_NUM - 1); ++i) {
			m_item_que[i] = m_item_que[i + 1];
		}
		m_item_que[QUE_NUM - 1] = AOD_AVATARITEM_NONE;

		// 準備へ遷移
		SetOwnProc(&CGet::ProcReady);
	}
}


// ***************************************************************************
// スレッドプロシージャ
// ***************************************************************************
// ===========================================================================
//! 取得スレッド
// ===========================================================================
void CGet::ThreadProcGet()
{
	// ID配列
	const u32 avatarasset_id_tbl[AOD_AVATARITEM_NUM] = {
		AVATARASSETAWARD_ITEM1,
		AVATARASSETAWARD_ITEM2,
	};

	// ID取得
	if ((u32)m_item_que[0] >= (u32)AOD_AVATARITEM_NUM) {
		m_result = AOD_AVATARAWARD_RESULT_ERROR;
		return;
	}
	u32 id = avatarasset_id_tbl[m_item_que[0]];

	// カレントユーザID取得
	s32 userid = AoAccountGetCurrentId();
	if ((u32)userid >= 4) {
		m_result = AOD_AVATARAWARD_RESULT_ERROR;
		return;
	}

	// アカウント有効判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		m_result = AOD_AVATARAWARD_RESULT_ERROR;
		return;
	}

	// 取得
	XUSER_AVATARASSET param;
	param.dwUserIndex = (u32)userid;
	param.dwAwardId = (u32)id;
	DWORD result = XUserAwardAvatarAssets(1, &param, NULL);
	if (result != ERROR_SUCCESS) {
		m_result = AOD_AVATARAWARD_RESULT_ERROR;
	}
	else {
		m_result = AOD_AVATARAWARD_RESULT_SUCCESS;
	}
}

} // namespace avataraward
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
