// ===========================================================================
/*!
	@file	dmRankSys.cpp
	@brief	ランキングシステム定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "dmRankSys.h"
#include "dmRankSysWait.h"
#include "ao.h"
#include "aoNetRank.h"
#include "gs.h"
#include "gsMainSys.h"
#include "gsBackup.hpp"
#include "dmSave.h"

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

namespace dm {

// ===========================================================================
//	class CRankSys
// ---------------------------------------------------------------------------
//!	ランキングシステムクラス
// ===========================================================================
class CRankSys :
	public ao::CProc<CRankSys>, public ao::CTask<CRankSys>,
	public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CRankSys();

	//! デストラクタ
	virtual ~CRankSys();

#if _WII
	//! セーブ後判定
	BOOL IsSaveAfter() const;
#endif // _WII

	//! 接続済み判定
	BOOL IsConnected() const;

	//! エラー判定
	BOOL IsError() const;

	//! 終了
	void End();

	//! ボード設定
	void SetBoard(
		DME_RANK_SYS_BOARD board, DME_RANK_SYS_RANK rank, DME_RANK_SYS_SS ss,
		BOOL is_friend, BOOL is_upload);

	//! アップロード中判定
	BOOL IsUploading() const;

	//! ボード内容取得開始
	void StartRankDownload(u32 no);

	//! ボードの自分周辺取得開始
	void StartRankDownloadNear();

	//! ランキングダウンロード中判定
	BOOL IsDownloading() const;

	//! ランキング表示数取得
	u32 GetShowNum() const;

	//! 実順位取得
	u32 GetRealNo(u32 no) const;

	//! 表示順位取得
	u32 GetShowNo(u32 no) const;

	//! 表示名取得
	const char* GetShowName(u32 no) const;

	//! 表示スコア取得
	u32 GetShowScore(u32 no) const;

	//! 表示リージョン取得
	DME_RANK_SYS_REGION GetShowRegion(u32 no) const;

	//! 表示スーパーソニック使用有無取得
	BOOL IsShowSs(u32 no) const;

	//! 自身の記録判定
	BOOL IsShowOwn(u32 no) const;

#if _XBOX
	//! XUID取得
	XUID GetXUID(u32 no) const;
#endif // _XBOX

	//! 通信処理のキャンセル可能判定
	BOOL IsCancelable() const;

	//! 通信処理のキャンセル
	void Cancel();

protected:

	// タスクプロシージャ
	void TaskProcMain();

	// プロシージャ
	void ProcConnecting();
#if _WII
	void ProcConnectingSave();
#endif // _WII
	void ProcConnected();
	void ProcUploadReady();
	void ProcUploading();
	void ProcUploadSaving();
	void ProcDownloadNormalReady();
	void ProcDownloadNormalDownloading();
	void ProcDownloadFriendReady();
	void ProcDownloadFriendDownloading();
	void ProcDownloadNearReady();
	void ProcDownloadNearDownloading();
	void ProcEnd();
#if _WII
	void ProcEndSave();
	void ProcErrorWait();
	void ProcError();
#endif // _WII

	// 定数
	enum {
		// 状態
		STATE_CONNECTING	= 0,	//!< 接続中
		STATE_UPLOADING,			//!< アップロード中
		STATE_UPLOADING_SAVE,		//!< アップロード後セーブ中
		STATE_IDLE,					//!< アイドリング
		STATE_DOWNLOADING,			//!< ランキングダウンロード中
		STATE_END,					//!< 終了中
		STATE_ERROR,				//!< エラー中

		DATA_NAME_LEN		= 20,	//!< 名前長
	};

	// 以下、データに設定する順位は1～とし、0なら無効とする

	//! ランキングデータ構造体
	struct DATA {
		u32					real_no;		//!< 実ランキング順位
		u32					rank_no;		//!< 表示ランキング順位
		char				name[DATA_NAME_LEN];	//!< 名前
		u32					score;			//!< スコア
		BOOL				ss;				//!< スーパーソニック使用フラグ
		BOOL				own;			//!< 自記録フラグ
		DME_RANK_SYS_REGION	region;			//!< リージョンタイプ
#if _XBOX
		XUID				xuid;			//!< XUID
#endif // _XBOX
	};

	//! ボード情報構造体
	struct BOARD_INFO {
		DME_RANK_SYS_BOARD	board;			//!< ボード
		DME_RANK_SYS_RANK	rank;			//!< ランキングタイプ
		DME_RANK_SYS_SS		ss;				//!< スーパーソニックタイプ
	};

	//! 通常ランキングデータ構造体
	struct NORMAL_RANK {
		BOARD_INFO	board;							//!< ボード情報
		BOOL		is_down;						//!< ダウンロード済みフラグ
		BOOL		is_near;						//!< 自分周辺フラグ
		u32			num;							//!< 表示数
		DATA		data[DMD_RANK_SYS_SHOW_MAX];	//!< ランキング
	};

	//! フレンドランキングデータ構造体
	struct FRIEND_RANK {
		BOARD_INFO	board;							//!< ボード情報
		BOOL		is_down;						//!< ダウンロード済みフラグ
		u32			req_top_no;						//!< 先頭順位(1-)
		u32			num;							//!< 表示数
		DATA		data[DMD_RANK_SYS_FRIEND_MAX];	//!< ランキング
	};

	//! リクエスト構造体
	struct REQUEST {
		BOARD_INFO		board;			//!< ボード情報
		BOOL			is_friend;		//!< 真：フレンドみの　偽：全員
		BOOL			is_upload;		//!< 真：要アップロード
		u32				top_no;			//!< 先頭順位(1-)
	};

	// ユーティリティ
	void UtilRecvDataToData(DATA& dest, const AOS_NET_RANK_DATA* src) const;

	// パラメータ初期化
	void InitParamData(DATA* data, u32 num);
	void InitParamBoardInfo(BOARD_INFO& info);
	void InitParamNormalRank(NORMAL_RANK& rank);
	void InitParamFriendRank(FRIEND_RANK& rank);
	void InitParamRequest(REQUEST& req);

	u32					m_state;		//!< 状態
	BOOL				m_end;			//!< 終了フラグ
	REQUEST				m_request;		//!< リクエスト
	NORMAL_RANK			m_nrank;		//!< 通常ランキングデータ
	FRIEND_RANK			m_frank;		//!< フレンドランキングデータ

#if _PS3 || _XBOX
	BOOL				m_cancel;		//!< キャンセルフラグ
#endif // _PS3 || _XBOX

#if _WII
	char				m_name[AOD_NET_RANK_NAME_LEN];	//!< 名前
	BOOL				m_is_save_after;	//!< 接続中のセーブ処理後フラグ
#endif // _WII
};

} // namespace dm

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// 送受信制御
static BOOL dmRankSysWaitIsTransable(void);
static void dmRankSysWaitSetTrans(void);

// ユーティリティ
static u32 dmRankSysUtilMakeBoardNo(
	DME_RANK_SYS_BOARD board, DME_RANK_SYS_RANK rank, DME_RANK_SYS_SS ss);
static BOOL dmRankSysUtilIsBoardAsc(
	DME_RANK_SYS_BOARD board, DME_RANK_SYS_RANK rank, DME_RANK_SYS_SS ss);

// グローバル
static BOOL dmRankSysGlbIsRecordUpdate(DME_RANK_SYS_BOARD board);
static BOOL dmRankSysGlbIsRecordUpdate(
	DME_RANK_SYS_BOARD board, DME_RANK_SYS_RANK rank, DME_RANK_SYS_SS ss);
static BOOL dmRankSysGlbIsRecordSs(
	DME_RANK_SYS_BOARD board, DME_RANK_SYS_RANK rank);
static u32 dmRankSysGlbGetRecord(
	DME_RANK_SYS_BOARD board, DME_RANK_SYS_RANK rank, BOOL ss);
static void dmRankSysGlbSetRecordUpload(DME_RANK_SYS_BOARD board);
static u32 dmRankSysGlbConvBoardIdToBackup(DME_RANK_SYS_BOARD board);
static BOOL dmRankSysGlbIsSaveEnable(void);

#if _WII
// Wii
static void dmRankSysWiiSetUserInfo(char* name, u32 len);
static const char* dmRankSysWiiGetUserName(void);
static DWCUserData* dmRankSysWiiGetDwcUserData(void);
#endif // _WII

#if _XBOX
// Xbox360
static void dmRankSysXbox360TaskShowGamerCardUI00(AMS_TCB* tcb);
static void dmRankSysXbox360TaskShowGamerCardUI01(AMS_TCB* tcb);
static void dmRankSysXbox360TaskShowGamerCardDisableUI00(AMS_TCB* tcb);
#endif // _XBOX

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//	dm::CRankSys* g_dm_rank_sys
// ---------------------------------------------------------------------------
//!	ランキングシステムクラスポインタ
// ===========================================================================
static dm::CRankSys* g_dm_rank_sys = NULL;

#if _XBOX
// ===========================================================================
//	AMS_TCB* g_dm_rank_sys_gcui_tcb
// ---------------------------------------------------------------------------
//!	ゲーマーカード表示タスクTCBポインタ
// ===========================================================================
static AMS_TCB* g_dm_rank_sys_gcui_tcb = NULL;
#endif // _XBOX

#if _WII && 0
// デバッグ用ユーザ名
static const char g_dm_rank_sys_dbg_username[
	gs::backup::SOption::c_name_length_limit] = "test";

// デバッグ用ユーザデータ
static DWCUserData g_dm_rank_sys_dbg_dwc_userdata;
#endif // _WII

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ***************************************************************************
// 初期化&終了処理関連
// ***************************************************************************
// ===========================================================================
//	DmRankSysInit
/*!
	ランキングシステム初期化処理

	@note
	アプリケーション起動時に一度だけ呼び出して下さい。\n
*/
// ===========================================================================
void DmRankSysInit(void)
{
	g_dm_rank_sys = NULL;
	DmRankSysWaitInit();

#if _XBOX
	g_dm_rank_sys_gcui_tcb = NULL;
#endif // _XBOX
}

// ===========================================================================
//	DmRankSysExit
/*!
	ランキングシステム終了処理

	@note
	アプリケーション終了時に一度だけ呼び出して下さい。\n
*/
// ===========================================================================
void DmRankSysExit(void)
{
	if (g_dm_rank_sys) {
		delete g_dm_rank_sys;
		g_dm_rank_sys = NULL;
	}

#if _XBOX
	if (g_dm_rank_sys_gcui_tcb) {
		amTaskDelete(g_dm_rank_sys_gcui_tcb);
		g_dm_rank_sys_gcui_tcb = NULL;
	}
#endif // _XBOX
}

#if !_IPHONE
// ***************************************************************************
// サーバ接続関連
// ***************************************************************************
// ===========================================================================
//	DmRankSysStart
/*!
	サーバ接続開始

	@note
	ランキングへアクセスするためのサーバ接続を開始します。\n
	接続完了判定はDmRankSysIsConnected関数で行なって下さい。\n
	また、この関数呼び出し以降は、
	定期的にDmRankSysIsError関数でエラー判定を行うようにして下さい。\n
	既にサーバ接続が開始されている場合は、アサートし何も行ないません。\n
*/
// ===========================================================================
void DmRankSysStart(void)
{
	// 既に通信中ならアサートし何も行なわない
	if (!DmRankSysIsFinished()) {
		amAssert(0);
		return;
	}

	// 開始
	g_dm_rank_sys = new dm::CRankSys;
}

// ===========================================================================
//	DmRankSysIsSaveAfter
/*!
	サーバ接続処理中行われたセーブ処理の後か判定

	@param 真：セーブ処理の後　偽：それ以外
	@note
	サーバ接続中にセーブ処理が必要になった場合、
	セーブ処理が必要になって以降、TRUEを返すようになります。\n
*/
// ===========================================================================
BOOL DmRankSysIsSaveAfter(void)
{
#if _WII
	if (g_dm_rank_sys) {
		return g_dm_rank_sys->IsSaveAfter();
	}
#endif // _WII

	return FALSE;
}

// ===========================================================================
//	DmRankSysStart
/*!
	サーバ接続完了判定

	@return 真：完了済み　偽：それ以外
	@note
	DmRankSysStart関数で開始したサーバ接続処理が
	完了したかどうかを判定します。\n
	この関数がTRUEを返す状態でのみボード関連の関数を呼び出すことができます。\n
	DmRankSysStart関数が呼ばれていない状態で呼び出した場合は、
	アサートしFALSEを返します。\n
*/
// ===========================================================================
BOOL DmRankSysIsConnected(void)
{
	if (DmRankSysIsFinished()) {
		amAssert(0);
		return FALSE;
	}
	return g_dm_rank_sys->IsConnected();
}

// ===========================================================================
//	DmRankSysIsError
/*!
	エラー判定

	@return 真：エラー発生　偽：それ以外
	@note
	通信処理においてエラーが発生したかどうかを判定します。\n
	この関数がTRUEを返す場合は、ただちにDmRankSysEnd関数を呼び出し、
	通信を終了して下さい。\n
	なお、エラー関連のユーザ通知は、内部で全て行ないますので、
	呼び出し側が何らかのユーザ通知を行なう必要はありません。\n
	DmRankSysStart関数が呼び出された段階でエラーはクリアされます。\n
*/
// ===========================================================================
BOOL DmRankSysIsError(void)
{
	if (DmRankSysIsFinished()) {
		return FALSE;
	}
	return g_dm_rank_sys->IsError();
}

// ===========================================================================
//	DmRankSysEnd
/*!
	サーバ接続終了開始

	@note
	DmRankSysStart関数で開始した通信処理の終了を開始します。\n
	通信処理は即時に終了しないので、
	DmRankSysIsFinished関数で終了を待つようにして下さい。\n
	既に終了済みの場合は何も行ないません。\n
*/
// ===========================================================================
void DmRankSysEnd(void)
{
	if (g_dm_rank_sys) {
		g_dm_rank_sys->End();
	}
}

// ===========================================================================
//	DmRankSysStart
/*!
	サーバ接続終了判定

	@return 真：終了済み　偽：それ以外
	@note
	全ての通信処理が終了したかどうかを判定します。\n
	DmRankSysEnd関数呼び出しから時間経過によりTRUEを返すようになります。\n
*/
// ===========================================================================
BOOL DmRankSysIsFinished(void)
{
	if (g_dm_rank_sys) {
		return FALSE;
	}
	return TRUE;
}


// ***************************************************************************
// ボード関連
// ***************************************************************************
// ===========================================================================
//	DmRankSysNoticeBoard
/*!
	ボード設定

	@param board		[in] ボード
	@param rank			[in] ランキングタイプ
	@param ss			[in] スーパーソニックタイプ
	@param is_friend	[in] 真：フレンドランキング　偽：全体ランキング
	@param is_upload	[in] 真：自身の記録アップロードする　偽：しない
	@note
	表示したいボード情報を設定し、必要な通信処理を行ないます。\n
	この関数は、以下の関数がFALSEを返す状態でのみ呼び出し可能です。\n
	DmRankSysIsUpLoading\n
	DmRankSysIsDownloading\n
*/
// ===========================================================================
void DmRankSysNoticeBoard(
	DME_RANK_SYS_BOARD board, DME_RANK_SYS_RANK rank, DME_RANK_SYS_SS ss,
	BOOL is_friend, BOOL is_upload)
{
	if (!g_dm_rank_sys) {
		amAssert(0);
		return;
	}
	if (DmRankSysOwnRecodeIsUpdate(board) == FALSE) {
		is_upload = FALSE;
	}
	g_dm_rank_sys->SetBoard(board, rank, ss, is_friend, is_upload);
}

// ===========================================================================
//	DmRankSysIsUpLoading
/*!
	自身の記録のアップロード中判定

	@return 真：アップロード中　偽：それ以外
	@note
	DmRankSysNoticeBoard関数呼出し後、必要ならアップロードを開始し、
	アップロード中は、この関数がTRUEを返します。\n
*/
// ===========================================================================
BOOL DmRankSysIsUpLoading(void)
{
	if (!g_dm_rank_sys) {
		amAssert(0);
		return FALSE;
	}
	return g_dm_rank_sys->IsUploading();
}

// ===========================================================================
//	DmRankSysNoticeShowRankNo
/*!
	表示先頭順位通知

	@param no			[in] 表示先頭順位(0 - )
	@note
	DmRankSysNoticeBoard関数で設定したボードから
	指定の順位を先頭とする
	最大DMD_RANK_SYS_SHOW_MAX個のランキングを取得します。\n
	通信処理の完了は、
	DmRankSysIsDownloading関数で行なうようにして下さい。\n
	この関数は、以下の関数がFALSEを返す状態でのみ呼び出し可能です。\n
	DmRankSysIsUpLoading\n
	DmRankSysIsDownloading\n
	表示先頭順位にDMD_RANK_SYS_OWN_NEARを指定することで、
	自身の周辺ランキングを取得することができます。\n
*/
// ===========================================================================
void DmRankSysNoticeShowRankNo(u32 no)
{
	if (!g_dm_rank_sys) {
		amAssert(0);
		return;
	}
	if (no == DMD_RANK_SYS_OWN_NEAR) {
		g_dm_rank_sys->StartRankDownloadNear();
	}
	else {
		g_dm_rank_sys->StartRankDownload(no);
	}
}

// ===========================================================================
//	DmRankSysIsDownloading
/*!
	ランキングのダウンロード中判定

	@return 真：ダウンロード中　偽：それ以外
	@note
	DmRankSysNoticeShowRankNo関数で開始した通信処理が
	継続中かどうかを判定します。\n
*/
// ===========================================================================
BOOL DmRankSysIsDownloading(void)
{
	if (!g_dm_rank_sys) {
		amAssert(0);
		return FALSE;
	}
	return g_dm_rank_sys->IsDownloading();
}

// ===========================================================================
//	DmRankSysGetShowNum
/*!
	ランキング表示数取得

	@return ランキング表示数
	@note
	DmRankSysNoticeShowRankNo関数で取得したランキング情報が
	何人分あるかを返します。\n
	この関数はいつでも呼び出し可能です。\n
*/
// ===========================================================================
u32 DmRankSysGetShowNum(void)
{
	if (!g_dm_rank_sys) {
		amAssert(0);
		return 0;
	}
	return g_dm_rank_sys->GetShowNum();
}

// ===========================================================================
//	DmRankSysGetRealRankNo
/*!
	実順位取得

	@param no			[in] 表示番号(0 - )
	@return 実ランキング順位
	@note
	引数に指定する表示番号がDmRankSysGetShowNum関数の戻り値未満の値であれば
	いつでも呼び出し可能です。\n
*/
// ===========================================================================
u32 DmRankSysGetRealRankNo(u32 no)
{
	if (!g_dm_rank_sys || (no >= DmRankSysGetShowNum())) {
		amAssert(0);
		return no;
	}
	return g_dm_rank_sys->GetRealNo(no);
}

// ===========================================================================
//	DmRankSysGetShowRankNo
/*!
	表示順位取得

	@param no			[in] 表示番号(0 - )
	@return 表示ランキング順位
	@note
	引数に指定する表示番号がDmRankSysGetShowNum関数の戻り値未満の値であれば
	いつでも呼び出し可能です。\n
*/
// ===========================================================================
u32 DmRankSysGetShowRankNo(u32 no)
{
	if (!g_dm_rank_sys || (no >= DmRankSysGetShowNum())) {
		amAssert(0);
		return no;
	}
	return g_dm_rank_sys->GetShowNo(no);
}

// ===========================================================================
//	DmRankSysGetShowName
/*!
	表示名取得

	@param no			[in] 表示番号(0 - )
	@return 表示名
	@note
	引数に指定する表示番号がDmRankSysGetShowNum関数の戻り値未満の値であれば
	いつでも呼び出し可能です。\n
*/
// ===========================================================================
const char* DmRankSysGetShowName(u32 no)
{
	if (!g_dm_rank_sys || (no >= DmRankSysGetShowNum())) {
		amAssert(0);
		return NULL;
	}
	return g_dm_rank_sys->GetShowName(no);
}

// ===========================================================================
//	DmRankSysGetShowScore
/*!
	表示スコア取得

	@param no			[in] 表示番号(0 - )
	@return 表示スコア
	@note
	引数に指定する表示番号がDmRankSysGetShowNum関数の戻り値未満の値であれば
	いつでも呼び出し可能です。\n
*/
// ===========================================================================
u32 DmRankSysGetShowScore(u32 no)
{
	if (!g_dm_rank_sys || (no >= DmRankSysGetShowNum())) {
		amAssert(0);
		return 0;
	}
	return g_dm_rank_sys->GetShowScore(no);
}

// ===========================================================================
//	DmRankSysGetShowRegion
/*!
	表示リージョン取得

	@param no			[in] 表示番号(0 - )
	@return 表示リージョン
	@note
	引数に指定する表示番号がDmRankSysGetShowNum関数の戻り値未満の値であれば
	いつでも呼び出し可能です。\n
*/
// ===========================================================================
DME_RANK_SYS_REGION DmRankSysGetShowRegion(u32 no)
{
	if (!g_dm_rank_sys || (no >= DmRankSysGetShowNum())) {
		amAssert(0);
		return DMD_RANK_SYS_REGION_OTHER;
	}
	return g_dm_rank_sys->GetShowRegion(no);
}

// ===========================================================================
//	DmRankSysGetShowRegion
/*!
	表示スーパーソニック使用有無取得

	@param no			[in] 表示番号(0 - )
	@return 真：スーパーソニック使用　偽：未使用
	@note
	引数に指定する表示番号がDmRankSysGetShowNum関数の戻り値未満の値であれば
	いつでも呼び出し可能です。\n
*/
// ===========================================================================
BOOL DmRankSysGetShowSs(u32 no)
{
	if (!g_dm_rank_sys || (no >= DmRankSysGetShowNum())) {
		amAssert(0);
		return FALSE;
	}
	return g_dm_rank_sys->IsShowSs(no);
}

// ===========================================================================
//	DmRankSysIsShowOwn
/*!
	自身の記録判定

	@param no			[in] 表示番号(0 - )
	@return 真：自身の記録　偽：それ以外
	@note
	引数に指定する表示番号がDmRankSysGetShowNum関数の戻り値未満の値であれば
	いつでも呼び出し可能です。\n
*/
// ===========================================================================
BOOL DmRankSysIsShowOwn(u32 no)
{
	if (!g_dm_rank_sys || (no >= DmRankSysGetShowNum())) {
		amAssert(0);
		return FALSE;
	}
	return g_dm_rank_sys->IsShowOwn(no);
}

// ===========================================================================
//	DmRankSysIsCancelable
/*!
	通信処理のキャンセル可能判定

	@return 真：キャンセル可能　偽：不可能
	@note
	以下の関数がTRUEを返す状態で、
	通信処理のキャンセルが可能かどうかを判定します。\n
	DmRankSysIsUpLoading\n
	DmRankSysIsDownloading\n
	注意：Wii版以外では、常にFALSEを返します。\n
*/
// ===========================================================================
BOOL DmRankSysIsCancelable(void)
{
	if (!g_dm_rank_sys) {
		return FALSE;
	}
	return g_dm_rank_sys->IsCancelable();
}

// ===========================================================================
//	DmRankSysNoticeCancel
/*!
	通信処理のキャンセル通知

	@note
	以下の関数がTRUEを返す状態で、通信処理のキャンセルを行ないます。\n
	DmRankSysIsUpLoading\n
	DmRankSysIsDownloading\n
	キャンセルを行なうと、上記関数はFALSEを返すようになります。\n
	この関数はDmRankSysIsCancelable関数がTRUEを返す状態でのみ
	呼び出し可能です。\n
*/
// ===========================================================================
void DmRankSysNoticeCancel(void)
{
	if (DmRankSysIsCancelable()) {
		g_dm_rank_sys->Cancel();
	}
}

#if _XBOX

// ===========================================================================
//	DmRankSysGamerCardShow
/*!
	ゲーマーカード表示開始

	@param no			[in] 表示番号(0 - )
	@note
	Xbox360版にて、指定番号のユーザのゲーマーカードUIを表示します。\n
	表示の終了はDmRankSysCamerCardShowIsFinished関数で判定して下さい。\n
*/
// ===========================================================================
void DmRankSysGamerCardShow(u32 no)
{
	if (!g_dm_rank_sys || (no >= DmRankSysGetShowNum())) {
		amAssert(0);
		return;
	}

	// XUID取得
	XUID xuid = g_dm_rank_sys->GetXUID(no);

	// アカウント有効判定
	if (!AoAccountIsCurrentEnable()) {
		return;
	}

	// アカウントID取得
	u32 id = (u32)AoAccountGetCurrentId();
	if (id >= 4) {
		return;
	}

	// アカウントが有効か判定
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		return;
	}

	BOOL is_privilege = TRUE;

	// 自身のプロフィールか判定
	XUID own_xuid;
	if (XUserGetXUID(id, &own_xuid) != ERROR_SUCCESS) {
		return;
	}
	if (!IsEqualXUID(own_xuid, xuid)) {

		// 他人のプロフィールを閲覧する権限があるか判定
		BOOL fResult;
		if (XUserCheckPrivilege(id, XPRIVILEGE_PROFILE_VIEWING, &fResult) !=
			ERROR_SUCCESS)
		{
			return;
		}
		if (fResult) {
			// 他人のプロフィールを閲覧する権限がある
			// empty
		}
		else {
			// 他人のプロフィールを閲覧する権限がない

			// 友達のプロフィールを閲覧する権限があるか判定
			if (XUserCheckPrivilege(
				id, XPRIVILEGE_PROFILE_VIEWING_FRIENDS_ONLY, &fResult) !=
				ERROR_SUCCESS)
			{
				return;
			}
			if (fResult) {
				// 友達のプロフィールを閲覧する権限がある

				// 相手がフレンドか判定
				DWORD ret = XUserAreUsersFriends(id, &xuid, 1, &fResult, NULL);
				if (ret != ERROR_SUCCESS) {
					return;
				}
				if (fResult) {
					// 相手はフレンド
					// empty
				}
				else {
					// 相手はフレンドではない
					is_privilege = FALSE;
				}
			}
			else {
				// 友達のプロフィールを閲覧する権限がない
				is_privilege = FALSE;
			}
		}
	}

	if (is_privilege) {

		// ゲーマーカードUI表示タスク作成
		g_dm_rank_sys_gcui_tcb = amTaskMake(
			dmRankSysXbox360TaskShowGamerCardUI00, NULL,
			0, 0, 0, "dmRankSys::GamerCardUI");

		// ワーク設定
		XUID* work = (XUID*)amTaskGetWork(g_dm_rank_sys_gcui_tcb);
		*work = g_dm_rank_sys->GetXUID(no);
	}
	else {

		// 閲覧権限無し通知タスク作成
		g_dm_rank_sys_gcui_tcb = amTaskMake(
			dmRankSysXbox360TaskShowGamerCardDisableUI00, NULL,
			0, 0, 0, "dmRankSys::GamerCardUI");
	}

	// タスク開始
	amTaskStart(g_dm_rank_sys_gcui_tcb);
}

// ===========================================================================
//	DmRankSysCamerCardShowIsFinished
/*!
	ゲーマーカード表示完了判定

	@return 真：終了済み　偽：それ以外
	@note
	Xbox360版にて、DmRankSysGamerCardShow関数で開始したゲーマーカードUI表示が
	終了したか判定します。\n
	DmRankSysGamerCardShow関数を呼び出していない場合は、TRUEを返します。\n
*/
// ===========================================================================
BOOL DmRankSysCamerCardShowIsFinished(void)
{
	if (g_dm_rank_sys_gcui_tcb) {
		return FALSE;
	}
	return TRUE;
}

#endif


// ***************************************************************************
// ユーティリティ
// ***************************************************************************
// ===========================================================================
//	DmRankSysOwnRecodeIsUpdate
/*!
	自身の記録の更新判定

	@param board		[in] ボード
	@return 真：更新されている　偽：更新されていない
*/
// ===========================================================================
BOOL DmRankSysOwnRecodeIsUpdate(DME_RANK_SYS_BOARD board)
{
	return dmRankSysGlbIsRecordUpdate(board);
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace dm {

// ***************************************************************************
// ランキングシステムクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CRankSys::CRankSys()
{
	// メンバ初期化
#if _PS3 || _XBOX
	m_cancel = FALSE;
#endif // _PS3 || _XBOX
	m_state = CRankSys::STATE_CONNECTING;
	m_end = FALSE;
#if _WII
	m_is_save_after = FALSE;
#endif // _WII
	InitParamRequest(m_request);
	InitParamNormalRank(m_nrank);
	InitParamFriendRank(m_frank);

#if _WII
	// ユーザ情報設定
	dmRankSysWiiSetUserInfo(m_name, AOD_NET_RANK_NAME_LEN);
#endif // _WII

	// エラークリア
	AoNetRankClearError();

#if _WII
	// Wiiはセーブ無効なら通信も無効
	if (!dmRankSysGlbIsSaveEnable()) {

		// エラー表示プロシージャ設定
		m_state = CRankSys::STATE_ERROR;
		SetProc(0, &CRankSys::ProcErrorWait);
	}
	else
#endif // _WII
	{
		// 通信開始
		AoNetRankStart();

		// 接続中プロシージャ設定
		SetProc(0, &CRankSys::ProcConnecting);
	}

	// メインタスク作成
	MakeTask(0, "dmRankSys::Main");

	// メインタスクプロシージャ設定
	SetTaskProc(0, &CRankSys::TaskProcMain);

	// メインタスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CRankSys::~CRankSys()
{
	AoNetRankForcedEnding();
	g_dm_rank_sys = NULL;
}

#if _WII
// ===========================================================================
//! セーブ後判定
// ===========================================================================
BOOL CRankSys::IsSaveAfter() const
{
	return m_is_save_after;
}
#endif // _WII

// ===========================================================================
//! 接続済み判定
// ===========================================================================
BOOL CRankSys::IsConnected() const
{
	if (m_state != CRankSys::STATE_CONNECTING) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! エラー判定
// ===========================================================================
BOOL CRankSys::IsError() const
{
	if (m_state == CRankSys::STATE_ERROR) {
		return TRUE;
	}
	return AoNetRankIsError();
}

// ===========================================================================
//! 終了
// ===========================================================================
void CRankSys::End()
{
	m_end = TRUE;
}

// ===========================================================================
//! ボード設定
// ===========================================================================
void CRankSys::SetBoard(
	DME_RANK_SYS_BOARD board, DME_RANK_SYS_RANK rank, DME_RANK_SYS_SS ss,
	BOOL is_friend, BOOL is_upload)
{
	if (m_state != CRankSys::STATE_IDLE) {
		amAssert(0);
		return;
	}
	amAssert((u32)board < DMD_RANK_SYS_BOARD_NUM);
	amAssert((u32)rank < DMD_RANK_SYS_RANK_NUM);
	amAssert((u32)ss < DMD_RANK_SYS_SS_NUM);

	// セーブが無効ならアップロードしない
	if (!dmRankSysGlbIsSaveEnable()) {
		is_upload = FALSE;
	}

#if _PS3 || _XBOX
	m_cancel = FALSE;
#endif // _PS3 || _XBOX

	// 既存の受信データクリア
	InitParamRequest(m_request);
	InitParamNormalRank(m_nrank);
	InitParamFriendRank(m_frank);

	// リクエスト作成
	m_request.board.board = board;
	m_request.board.rank = rank;
	m_request.board.ss = ss;
	m_request.is_friend = is_friend;
	m_request.is_upload = is_upload;

	// 記録更新判定
	if (m_request.is_upload) {
		m_request.is_upload = dmRankSysGlbIsRecordUpdate(board);
	}
	if (m_request.is_upload) {
		// アップロードへ遷移
		m_state = CRankSys::STATE_UPLOADING;
		SetProc(0, &CRankSys::ProcUploadReady);
	}
	else {
		m_state = CRankSys::STATE_IDLE;
		SetProc(0, &CRankSys::ProcConnected); // 念のため
	}
}

// ===========================================================================
//! アップロード中判定
// ===========================================================================
BOOL CRankSys::IsUploading() const
{
	if ((m_state == CRankSys::STATE_UPLOADING) ||
		(m_state == CRankSys::STATE_UPLOADING_SAVE))
	{
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! ボード内容取得開始
// ===========================================================================
void CRankSys::StartRankDownload(u32 no)
{
	if (m_state != CRankSys::STATE_IDLE) {
		amAssert(0);
		return;
	}

#if _PS3 || _XBOX
	m_cancel = FALSE;
#endif // _PS3 || _XBOX

	// 既存判定
	if (m_request.is_friend) {
		if ((m_request.board.board == m_frank.board.board) &&
			(m_request.board.rank == m_frank.board.rank) &&
			(m_request.board.ss == m_frank.board.ss))
		{
			if (m_frank.is_down) {
				// ダウンロード必要なし
				if (no < m_frank.num) {
					m_frank.req_top_no = no;
				}
				return;
			}
		}

		// ダウンロードへ遷移
		m_state = CRankSys::STATE_DOWNLOADING;
		m_frank.is_down = FALSE;
		m_request.top_no = (u32)(no + 1);
		SetProc(0, &CRankSys::ProcDownloadFriendReady);
	}
	else {
		if ((m_request.board.board == m_nrank.board.board) &&
			(m_request.board.rank == m_nrank.board.rank) &&
			(m_request.board.ss == m_nrank.board.ss))
		{
			if (m_nrank.is_down && !m_nrank.is_near) {
				if ((m_nrank.num > 0) &&
					(m_nrank.data[0].real_no == (no + 1)))
				{
					// ダウンロード必要なし
					return;
				}
			}
		}

		// ダウンロードへ遷移
		m_state = CRankSys::STATE_DOWNLOADING;
		m_nrank.is_down = FALSE;
		m_request.top_no = (u32)(no + 1);
		SetProc(0, &CRankSys::ProcDownloadNormalReady);
	}
}

// ===========================================================================
//! ボードの自分周辺取得開始
// ===========================================================================
void CRankSys::StartRankDownloadNear()
{
	if (m_state != CRankSys::STATE_IDLE) {
		amAssert(0);
		return;
	}

#if _PS3 || _XBOX
	m_cancel = FALSE;
#endif // _PS3 || _XBOX

	if ((m_request.board.board == m_nrank.board.board) &&
		(m_request.board.rank == m_nrank.board.rank) &&
		(m_request.board.ss == m_nrank.board.ss))
	{
		if (m_nrank.is_down && m_nrank.is_near) {
			// ダウンロード必要なし
			return;
		}
	}

	// ダウンロードへ遷移
	m_state = CRankSys::STATE_DOWNLOADING;
	m_nrank.is_down = FALSE;
	m_request.top_no = 0;
	SetProc(0, &CRankSys::ProcDownloadNearReady);
}

// ===========================================================================
//! ランキングダウンロード中判定
// ===========================================================================
BOOL CRankSys::IsDownloading() const
{
	if (m_state == CRankSys::STATE_DOWNLOADING) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! ランキング表示数取得
// ===========================================================================
u32 CRankSys::GetShowNum() const
{
	if (m_state != CRankSys::STATE_IDLE) {
		return 0;
	}
	if (m_request.is_friend) {
		if (m_frank.is_down == FALSE) {
			return 0;
		}
		if (m_frank.req_top_no >= m_frank.num) {
			return 0;
		}
		else if ((m_frank.num - m_frank.req_top_no) < DMD_RANK_SYS_SHOW_MAX) {
			return (u32)(m_frank.num - m_frank.req_top_no);
		}
		return DMD_RANK_SYS_SHOW_MAX;
	}
	if (m_nrank.is_down == FALSE) {
		return 0;
	}
	return m_nrank.num;
}

// ===========================================================================
//! 実順位取得
// ===========================================================================
u32 CRankSys::GetRealNo(u32 no) const
{
	amAssert(no < GetShowNum());
	if (m_request.is_friend) {
		return (u32)(m_frank.req_top_no + no);
	//	return (u32)(m_frank.data[m_frank.req_top_no + no].real_no - 1);
	}
	return (u32)(m_nrank.data[no].real_no - 1);
}

// ===========================================================================
//! 表示順位取得
// ===========================================================================
u32 CRankSys::GetShowNo(u32 no) const
{
	amAssert(no < GetShowNum());
	if (m_request.is_friend) {
		return (u32)(m_frank.data[m_frank.req_top_no + no].rank_no - 1);
	}
	return (u32)(m_nrank.data[no].rank_no - 1);
}

// ===========================================================================
//! 表示名取得
// ===========================================================================
const char* CRankSys::GetShowName(u32 no) const
{
	amAssert(no < GetShowNum());
	if (m_request.is_friend) {
		return m_frank.data[m_frank.req_top_no + no].name;
	}
	return m_nrank.data[no].name;
}

// ===========================================================================
//! 表示スコア取得
// ===========================================================================
u32 CRankSys::GetShowScore(u32 no) const
{
	amAssert(no < GetShowNum());
	if (m_request.is_friend) {
		return m_frank.data[m_frank.req_top_no + no].score;
	}
	return m_nrank.data[no].score;
}

// ===========================================================================
//! 表示リージョン取得
// ===========================================================================
DME_RANK_SYS_REGION CRankSys::GetShowRegion(u32 no) const
{
	amAssert(no < GetShowNum());
	if (m_request.is_friend) {
		return m_frank.data[m_frank.req_top_no + no].region;
	}
	return m_nrank.data[no].region;
}

// ===========================================================================
//! 表示スーパーソニック使用有無取得
// ===========================================================================
BOOL CRankSys::IsShowSs(u32 no) const
{
	amAssert(no < GetShowNum());
	if (m_request.is_friend) {
		return m_frank.data[m_frank.req_top_no + no].ss;
	}
	return m_nrank.data[no].ss;
}

// ===========================================================================
//! 自身の記録判定
// ===========================================================================
BOOL CRankSys::IsShowOwn(u32 no) const
{
	amAssert(no < GetShowNum());
	if (m_request.is_friend) {
		return m_frank.data[m_frank.req_top_no + no].own;
	}
	return m_nrank.data[no].own;
}

#if _XBOX
// ===========================================================================
//! XUID取得
// ===========================================================================
XUID CRankSys::GetXUID(u32 no) const
{
	amAssert(no < GetShowNum());
	if (m_request.is_friend) {
		return m_frank.data[m_frank.req_top_no + no].xuid;
	}
	return m_nrank.data[no].xuid;
}
#endif // _XBOX

// ===========================================================================
//! 通信処理のキャンセル可能判定
// ===========================================================================
BOOL CRankSys::IsCancelable() const
{
#if _WII

	if ((dmRankSysWaitIsTransable() == FALSE) &&
		(IsProc(0, &CRankSys::ProcUploadReady) ||
		 IsProc(0, &CRankSys::ProcDownloadNormalReady) ||
		 IsProc(0, &CRankSys::ProcDownloadFriendReady) ||
		 IsProc(0, &CRankSys::ProcDownloadNearReady)))
	{
		return TRUE;
	}
	return FALSE;

#elif _PS3

	if (m_state == CRankSys::STATE_UPLOADING_SAVE) {
		return FALSE;
	}
	return TRUE;

#elif _XBOX

	return AoNetRankSendIsCancelable();

#else

	return FALSE;

#endif // _WII
}

// ===========================================================================
//! 通信処理のキャンセル
// ===========================================================================
void CRankSys::Cancel()
{
#if _WII

	if (dmRankSysWaitIsTransable() == FALSE) {
		if (IsProc(0, &CRankSys::ProcUploadReady)) {
			m_state = CRankSys::STATE_IDLE;
			SetOwnProc(&CRankSys::ProcConnected);
		}
		else if (
			IsProc(0, &CRankSys::ProcDownloadNormalReady) ||
			IsProc(0, &CRankSys::ProcDownloadFriendReady) ||
			IsProc(0, &CRankSys::ProcDownloadNearReady))
		{
			m_state = CRankSys::STATE_IDLE;
			SetOwnProc(&CRankSys::ProcConnected);
		}
		else {
			amAssert(0);
		}
	}
	else {
		amAssert(0);
	}

#elif _PS3

	m_cancel = TRUE;

#elif _XBOX

	if (IsCancelable()) {
		m_cancel = TRUE;
	}

#endif // _WII
}


// ***************************************************************************
// タスクプロシージャ
// ***************************************************************************
// ===========================================================================
//! メインタスク
// ===========================================================================
void CRankSys::TaskProcMain()
{
	// プロシージャ確認
	if (IsProcNone(0)) {
		// 終了
		if (AoNetRankIsFinished()) {
			DeleteOwnTask();
			delete this;
		}
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
//! 接続中
// ===========================================================================
void CRankSys::ProcConnecting()
{
	// 終了判定
	if (m_end) {
		AoNetRankEnd();
		m_state = CRankSys::STATE_END;
		SetOwnProc(&CRankSys::ProcEnd);
		return;
	}

	// 接続済み判定
	if (AoNetRankIsConnected()) {
		// 接続済みへ遷移
		m_state = CRankSys::STATE_IDLE;
		SetOwnProc(&CRankSys::ProcConnected);
	}

#if _WII
	// 要セーブ判定
	else if (AoNetRankWiiIsNeedSaveUserdata()) {
		// セーブへ遷移
		SetOwnProc(&CRankSys::ProcConnectingSave);
	}
#endif // _WII
}

#if _WII
// ===========================================================================
//! 接続中(セーブ)
// ===========================================================================
void CRankSys::ProcConnectingSave()
{
	if (GetCount() == 0) {
		// セーブ前処理
		AoNetRankWiiPreSaveUserdata();

		// セーブ開始
		DmSaveMenuStart();

		// セーブ後フラグON
		m_is_save_after = TRUE;
	}

	// セーブ終了判定
	if (DmSaveIsExit()) {

		// セーブ完了通知
		AoNetRankWiiNoticeUserdataSaved();

		// 接続中へ遷移
		SetOwnProc(&CRankSys::ProcConnecting);
	}
}
#endif // _WII

// ===========================================================================
//! 接続済み
// ===========================================================================
void CRankSys::ProcConnected()
{
	// 終了判定
	if (m_end) {
		AoNetRankEnd();
		m_state = CRankSys::STATE_END;
		SetOwnProc(&CRankSys::ProcEnd);
		return;
	}

	// empty
}

// ===========================================================================
//! アップロード準備
// ===========================================================================
void CRankSys::ProcUploadReady()
{
	// 終了判定
	if (m_end) {
		AoNetRankEnd();
		m_state = CRankSys::STATE_END;
		SetOwnProc(&CRankSys::ProcEnd);
		return;
	}

	// サブステート
	// 0 : タイムアタック + 通常ソニック
	// 1 : タイムアタック + スーパーソニック
	// 2 : タイムアタック + 両方ソニック
	// 3 : スコアアタック + 通常ソニック
	// 4 : スコアアタック + スーパーソニック
	// 5 : スコアアタック + 両方ソニック

	// 送信データ作成
	AoNetRankSendClear();
	BOOL upload = FALSE;
	for (u32 i = 0; i < 6; ++i) {
		u32 board;
		AOE_NET_RANK_TYPE rank_type;
		AOS_NET_RANK_SEND data;
		BOOL is_acs;
		switch (i) {
		case 0: // タイムアタック + 通常ソニック
			// 記録更新判定
			if (dmRankSysGlbIsRecordUpdate(
				m_request.board.board,
				DMD_RANK_SYS_RANK_TIME, DMD_RANK_SYS_SS_DISABLE))
			{
				// ボード番号算出
				board = dmRankSysUtilMakeBoardNo(
					m_request.board.board,
					DMD_RANK_SYS_RANK_TIME,
					DMD_RANK_SYS_SS_DISABLE);

				// 送信データ作成
				rank_type = AOD_NET_RANK_TYPE_TIME;
				data.time = dmRankSysGlbGetRecord(
					m_request.board.board, DMD_RANK_SYS_RANK_TIME, FALSE);
				data.ss = FALSE;
				upload = TRUE;
				is_acs = dmRankSysUtilIsBoardAsc(
					m_request.board.board,
					DMD_RANK_SYS_RANK_TIME,
					DMD_RANK_SYS_SS_DISABLE);
			}
			else {
				continue;
			}
			break;

		case 1: // タイムアタック + スーパーソニック
			// 記録更新判定
			if (dmRankSysGlbIsRecordUpdate(
				m_request.board.board,
				DMD_RANK_SYS_RANK_TIME, DMD_RANK_SYS_SS_ENABLE))
			{
				// ボード番号算出
				board = dmRankSysUtilMakeBoardNo(
					m_request.board.board,
					DMD_RANK_SYS_RANK_TIME,
					DMD_RANK_SYS_SS_ENABLE);

				// 送信データ作成
				rank_type = AOD_NET_RANK_TYPE_TIME;
				data.time = dmRankSysGlbGetRecord(
					m_request.board.board, DMD_RANK_SYS_RANK_TIME, TRUE);
				data.ss = TRUE;
				upload = TRUE;
				is_acs = dmRankSysUtilIsBoardAsc(
					m_request.board.board,
					DMD_RANK_SYS_RANK_TIME,
					DMD_RANK_SYS_SS_ENABLE);
			}
			else {
				continue;
			}
			break;

		case 2: // タイムアタック + 両方ソニック
			// 記録更新判定
			if (dmRankSysGlbIsRecordUpdate(
				m_request.board.board,
				DMD_RANK_SYS_RANK_TIME, DMD_RANK_SYS_SS_BOTH))
			{
				// ボード番号算出
				board = dmRankSysUtilMakeBoardNo(
					m_request.board.board,
					DMD_RANK_SYS_RANK_TIME,
					DMD_RANK_SYS_SS_BOTH);

				// 送信データ作成
				rank_type = AOD_NET_RANK_TYPE_TIME;
				data.ss = dmRankSysGlbIsRecordSs(
					m_request.board.board, DMD_RANK_SYS_RANK_TIME);
				data.time = dmRankSysGlbGetRecord(
					m_request.board.board, DMD_RANK_SYS_RANK_TIME, data.ss);
				upload = TRUE;
				is_acs = dmRankSysUtilIsBoardAsc(
					m_request.board.board,
					DMD_RANK_SYS_RANK_TIME,
					DMD_RANK_SYS_SS_BOTH);
			}
			else {
				continue;
			}
			break;

		case 3: // スコアアタック + 通常ソニック
			// 記録更新判定
			if (dmRankSysGlbIsRecordUpdate(
				m_request.board.board,
				DMD_RANK_SYS_RANK_SCORE, DMD_RANK_SYS_SS_DISABLE))
			{
				// ボード番号算出
				board = dmRankSysUtilMakeBoardNo(
					m_request.board.board,
					DMD_RANK_SYS_RANK_SCORE,
					DMD_RANK_SYS_SS_DISABLE);

				// 送信データ作成
				rank_type = AOD_NET_RANK_TYPE_SCORE;
				data.score = dmRankSysGlbGetRecord(
					m_request.board.board, DMD_RANK_SYS_RANK_SCORE, FALSE);
				data.ss = FALSE;
				upload = TRUE;
				is_acs = dmRankSysUtilIsBoardAsc(
					m_request.board.board,
					DMD_RANK_SYS_RANK_SCORE,
					DMD_RANK_SYS_SS_DISABLE);
			}
			else {
				continue;
			}
			break;

		case 4: // スコアアタック + スーパーソニック
			// 記録更新判定
			if (dmRankSysGlbIsRecordUpdate(
				m_request.board.board,
				DMD_RANK_SYS_RANK_SCORE, DMD_RANK_SYS_SS_ENABLE))
			{
				// ボード番号算出
				board = dmRankSysUtilMakeBoardNo(
					m_request.board.board,
					DMD_RANK_SYS_RANK_SCORE,
					DMD_RANK_SYS_SS_ENABLE);

				// 送信データ作成
				rank_type = AOD_NET_RANK_TYPE_SCORE;
				data.score = dmRankSysGlbGetRecord(
					m_request.board.board, DMD_RANK_SYS_RANK_SCORE, TRUE);
				data.ss = TRUE;
				upload = TRUE;
				is_acs = dmRankSysUtilIsBoardAsc(
					m_request.board.board,
					DMD_RANK_SYS_RANK_SCORE,
					DMD_RANK_SYS_SS_ENABLE);
			}
			else {
				continue;
			}
			break;

		case 5: // スコアアタック + 両方ソニック
			// 記録更新判定
			if (dmRankSysGlbIsRecordUpdate(
				m_request.board.board,
				DMD_RANK_SYS_RANK_SCORE, DMD_RANK_SYS_SS_BOTH))
			{
				// ボード番号算出
				board = dmRankSysUtilMakeBoardNo(
					m_request.board.board,
					DMD_RANK_SYS_RANK_SCORE,
					DMD_RANK_SYS_SS_BOTH);

				// 送信データ作成
				rank_type = AOD_NET_RANK_TYPE_SCORE;
				data.ss = dmRankSysGlbIsRecordSs(
					m_request.board.board, DMD_RANK_SYS_RANK_SCORE);
				data.time = dmRankSysGlbGetRecord(
					m_request.board.board, DMD_RANK_SYS_RANK_SCORE, data.ss);
				upload = TRUE;
				is_acs = dmRankSysUtilIsBoardAsc(
					m_request.board.board,
					DMD_RANK_SYS_RANK_SCORE,
					DMD_RANK_SYS_SS_BOTH);
			}
			else {
				continue;
			}
			break;

		default:
			amAssert(0);
			continue;
			break;
		}

		// 送信データ設定
		AoNetRankSendSetData(board, rank_type, &data, is_acs);
	}

	if (upload) {

		// 待ち
		if (!dmRankSysWaitIsTransable()) {
			return;
		}
		dmRankSysWaitSetTrans();

		// 送信開始
		AoNetRankSendStart();

		// 送信中へ遷移
		SetOwnProc(&CRankSys::ProcUploading);
	}
	else {
		// アップロード完了
		dmRankSysGlbSetRecordUpload(m_request.board.board);

		// アイドルへ遷移
		m_state = CRankSys::STATE_IDLE;
		SetOwnProc(&CRankSys::ProcConnected);
	}
}

// ===========================================================================
//! アップロード中
// ===========================================================================
void CRankSys::ProcUploading()
{
	// 送信終了判定
	if (AoNetRankSendIsFinished()) {

		// 結果判定
		if (!AoNetRankSendIsComplate()) {
			// 中断
			AoNetRankEnd();
			m_state = CRankSys::STATE_END;
			SetOwnProc(&CRankSys::ProcEnd);
			return;
		}

		// アップロード完了
#if _PS3 || _XBOX
		if (!m_cancel)
#endif // _PS3 || _XBOX
		{
			dmRankSysGlbSetRecordUpload(m_request.board.board);
		}

		// セーブへ遷移
		m_state = CRankSys::STATE_UPLOADING_SAVE;
		SetOwnProc(&CRankSys::ProcUploadSaving);
	}

#if _PS3 || _XBOX
	else if (m_cancel) {
		AoNetRankSendCancel();
	}
#endif // _PS3 || _XBOX
}

// ===========================================================================
//! アップロード後セーブ中
// ===========================================================================
void CRankSys::ProcUploadSaving()
{
#if !_WII

	if (GetCount() == 0) {
		// セーブ開始
		DmSaveMenuStart();
	}

	// セーブ終了判定
	if (DmSaveIsExit()) {

		// アイドルへ遷移
		m_state = CRankSys::STATE_IDLE;
		SetOwnProc(&CRankSys::ProcConnected);
	}

#else

	// Wiiはセーブ頻度を抑えるためにアップロード後セーブは行わない

	// アイドルへ遷移
	m_state = CRankSys::STATE_IDLE;
	SetOwnProc(&CRankSys::ProcConnected);

#endif // !_WII
}

// ===========================================================================
//! 通常ランキングダウンロード準備
// ===========================================================================
void CRankSys::ProcDownloadNormalReady()
{
	// 終了判定
	if (m_end) {
		AoNetRankEnd();
		m_state = CRankSys::STATE_END;
		SetOwnProc(&CRankSys::ProcEnd);
		return;
	}

	// 待ち
	if (!dmRankSysWaitIsTransable()) {
		return;
	}
	dmRankSysWaitSetTrans();

	// ボード番号算出
	u32 board = dmRankSysUtilMakeBoardNo(
		m_request.board.board, m_request.board.rank, m_request.board.ss);

	// 昇順判定
	BOOL is_acs = dmRankSysUtilIsBoardAsc(
		m_request.board.board, m_request.board.rank, m_request.board.ss);

	// 取得先頭順位取得
	u32 rank = m_request.top_no;
	if (rank == 0) {
		amAssert(0);
	}
	else {
		rank -= 1;
	}

	// 受信開始
	AOE_NET_RANK_TYPE rank_type;
	if (m_request.board.rank == DMD_RANK_SYS_RANK_TIME) {
		rank_type = AOD_NET_RANK_TYPE_TIME;
	}
	else {
		rank_type = AOD_NET_RANK_TYPE_SCORE;
	}
	AoNetRankRecvStart(board, rank_type, rank, is_acs);

	// 受信中へ遷移
	SetOwnProc(&CRankSys::ProcDownloadNormalDownloading);
}

// ===========================================================================
//! 通常ランキングダウンロード中
// ===========================================================================
void CRankSys::ProcDownloadNormalDownloading()
{
	// 受信完了判定
	if (AoNetRankRecvIsFinished()) {

		// 結果判定
		if (!AoNetRankRecvIsComplate()) {
			// 中断
			AoNetRankEnd();
			m_state = CRankSys::STATE_END;
			SetOwnProc(&CRankSys::ProcEnd);
			return;
		}

		// ダウンロード済み設定
		m_nrank.is_down = TRUE;

		// 受信データがあるなら内部データ更新
#if _PS3
		if ((AoNetRankRecvGetRecvNum() > 0) && !m_cancel) {
#else
		if (AoNetRankRecvGetRecvNum() > 0) {
#endif // _PS3

			m_nrank.board = m_request.board;
			m_nrank.is_near = FALSE;
			m_nrank.num = AoNetRankRecvGetRecvNum();
			if (m_nrank.num > DMD_RANK_SYS_SHOW_MAX) {
				m_nrank.num = DMD_RANK_SYS_SHOW_MAX;
			}

			u32 i;
			for (i = 0; i < m_nrank.num; ++i) {
				CRankSys::DATA& data = m_nrank.data[i];
				const AOS_NET_RANK_DATA* src = AoNetRankRecvGetData(i);
				UtilRecvDataToData(data, src);
				if (m_nrank.board.ss == DMD_RANK_SYS_SS_DISABLE) {
					data.ss = FALSE;
				}
				else if (m_nrank.board.ss == DMD_RANK_SYS_SS_ENABLE) {
					data.ss = TRUE;
				}
			}
			if (i < DMD_RANK_SYS_SHOW_MAX) {
				InitParamData(
					&m_nrank.data[i], (u32)(DMD_RANK_SYS_SHOW_MAX - i));
			}
		}

		// アイドルへ遷移
		m_state = CRankSys::STATE_IDLE;
		SetOwnProc(&CRankSys::ProcConnected);
	}

#if _PS3
	else if (m_cancel) {
		AoNetRankRecvCancel();
	}
#endif // _PS3
}

// ===========================================================================
//! フレンドランキングダウンロード準備
// ===========================================================================
void CRankSys::ProcDownloadFriendReady()
{
	// 終了判定
	if (m_end) {
		AoNetRankEnd();
		m_state = CRankSys::STATE_END;
		SetOwnProc(&CRankSys::ProcEnd);
		return;
	}

	// 待ち
	if (!dmRankSysWaitIsTransable()) {
		return;
	}
	dmRankSysWaitSetTrans();

	// ボード番号算出
	u32 board = dmRankSysUtilMakeBoardNo(
		m_request.board.board, m_request.board.rank, m_request.board.ss);

	// 昇順判定
	BOOL is_acs = dmRankSysUtilIsBoardAsc(
		m_request.board.board, m_request.board.rank, m_request.board.ss);

	// 受信開始
	AOE_NET_RANK_TYPE rank_type;
	if (m_request.board.rank == DMD_RANK_SYS_RANK_TIME) {
		rank_type = AOD_NET_RANK_TYPE_TIME;
	}
	else {
		rank_type = AOD_NET_RANK_TYPE_SCORE;
	}
	AoNetRankRecvStart(board, rank_type, AOD_NET_RANK_FRIENDS, is_acs);

	// 受信中へ遷移
	SetOwnProc(&CRankSys::ProcDownloadFriendDownloading);
}

// ===========================================================================
//! フレンドランキングダウンロード中
// ===========================================================================
void CRankSys::ProcDownloadFriendDownloading()
{
	// 受信完了判定
	if (AoNetRankRecvIsFinished()) {

		// 結果判定
		if (!AoNetRankRecvIsComplate()) {
			// 中断
			AoNetRankEnd();
			m_state = CRankSys::STATE_END;
			SetOwnProc(&CRankSys::ProcEnd);
			return;
		}

		// ダウンロード済み設定
		m_frank.is_down = TRUE;
		m_frank.req_top_no = 0;

		// 受信データがあるなら内部データ更新
#if _PS3
		if ((AoNetRankRecvGetRecvNum() > 0) && !m_cancel) {
#else
		if (AoNetRankRecvGetRecvNum() > 0) {
#endif // _PS3

			u32 top_no = m_request.top_no;
			if (top_no > 0) {
				top_no -= 1;
			}
			if (top_no < AoNetRankRecvGetRecvNum()) {
				m_frank.req_top_no = top_no;
			}
			else {
				m_frank.req_top_no = (u32)(AoNetRankRecvGetRecvNum() - 1);
			}

			m_frank.board = m_request.board;
			m_frank.num = AoNetRankRecvGetRecvNum();
			if (m_frank.num > DMD_RANK_SYS_FRIEND_MAX) {
				m_frank.num = DMD_RANK_SYS_FRIEND_MAX;
			}

			u32 i;
			for (i = 0; i < m_frank.num; ++i) {
				CRankSys::DATA& data = m_frank.data[i];
				const AOS_NET_RANK_DATA* src = AoNetRankRecvGetData(i);
				UtilRecvDataToData(data, src);
				if (m_frank.board.ss == DMD_RANK_SYS_SS_DISABLE) {
					data.ss = FALSE;
				}
				else if (m_nrank.board.ss == DMD_RANK_SYS_SS_ENABLE) {
					data.ss = TRUE;
				}
			}
			if (i < DMD_RANK_SYS_FRIEND_MAX) {
				InitParamData(
					&m_frank.data[i], (u32)(DMD_RANK_SYS_FRIEND_MAX - i));
			}
		}

		// アイドルへ遷移
		m_state = CRankSys::STATE_IDLE;
		SetOwnProc(&CRankSys::ProcConnected);
	}

#if _PS3
	else if (m_cancel) {
		AoNetRankRecvCancel();
	}
#endif // _PS3
}

// ===========================================================================
//! 周辺ランキングダウンロード準備
// ===========================================================================
void CRankSys::ProcDownloadNearReady()
{
	// 終了判定
	if (m_end) {
		AoNetRankEnd();
		m_state = CRankSys::STATE_END;
		SetOwnProc(&CRankSys::ProcEnd);
		return;
	}

	// 待ち
	if (!dmRankSysWaitIsTransable()) {
		return;
	}
	dmRankSysWaitSetTrans();

	// ボード番号算出
	u32 board = dmRankSysUtilMakeBoardNo(
		m_request.board.board, m_request.board.rank, m_request.board.ss);

	// 昇順判定
	BOOL is_acs = dmRankSysUtilIsBoardAsc(
		m_request.board.board, m_request.board.rank, m_request.board.ss);

	// 受信開始
	AOE_NET_RANK_TYPE rank_type;
	if (m_request.board.rank == DMD_RANK_SYS_RANK_TIME) {
		rank_type = AOD_NET_RANK_TYPE_TIME;
	}
	else {
		rank_type = AOD_NET_RANK_TYPE_SCORE;
	}
	AoNetRankRecvStart(board, rank_type, AOD_NET_RANK_OWN_NEAR, is_acs);

	// 受信中へ遷移
	SetOwnProc(&CRankSys::ProcDownloadNearDownloading);
}

// ===========================================================================
//! 周辺ランキングダウンロード中
// ===========================================================================
void CRankSys::ProcDownloadNearDownloading()
{
	// 受信完了判定
	if (AoNetRankRecvIsFinished()) {

		// 結果判定
		if (!AoNetRankRecvIsComplate()) {
			// 中断
			AoNetRankEnd();
			m_state = CRankSys::STATE_END;
			SetOwnProc(&CRankSys::ProcEnd);
			return;
		}

		// ダウンロード済み設定
		m_nrank.is_down = TRUE;

		// 受信データがあるなら内部データ更新
#if _PS3
		if ((AoNetRankRecvGetRecvNum() > 0) && !m_cancel) {
#else
		if (AoNetRankRecvGetRecvNum() > 0) {
#endif // _PS3

			m_nrank.board = m_request.board;
			m_nrank.is_near = TRUE;
			m_nrank.num = AoNetRankRecvGetRecvNum();
			if (m_nrank.num > DMD_RANK_SYS_SHOW_MAX) {
				m_nrank.num = DMD_RANK_SYS_SHOW_MAX;
			}

			u32 i;
			for (i = 0; i < m_nrank.num; ++i) {
				CRankSys::DATA& data = m_nrank.data[i];
				const AOS_NET_RANK_DATA* src = AoNetRankRecvGetData(i);
				UtilRecvDataToData(data, src);
				if (m_nrank.board.ss == DMD_RANK_SYS_SS_DISABLE) {
					data.ss = FALSE;
				}
				else if (m_nrank.board.ss == DMD_RANK_SYS_SS_ENABLE) {
					data.ss = TRUE;
				}
			}
			if (i < DMD_RANK_SYS_SHOW_MAX) {
				InitParamData(
					&m_nrank.data[i], (u32)(DMD_RANK_SYS_SHOW_MAX - i));
			}
		}
		else {
			m_nrank.board = m_request.board;
			m_nrank.is_near = TRUE;
			m_nrank.num = 0;
			InitParamData(&m_nrank.data[0], DMD_RANK_SYS_SHOW_MAX);
		}

		// アイドルへ遷移
		m_state = CRankSys::STATE_IDLE;
		SetOwnProc(&CRankSys::ProcConnected);
	}

#if _PS3
	else if (m_cancel) {
		AoNetRankRecvCancel();
	}
#endif // _PS3
}

// ===========================================================================
//! 終了中
// ===========================================================================
void CRankSys::ProcEnd()
{
	// 終了判定
	if (AoNetRankIsFinished()) {
#if _WII
		// セーブへ遷移
		SetOwnProc(&CRankSys::ProcEndSave);
#else
		// 終了
		SetOwnProcNone();
#endif
	}
}

#if _WII
// ===========================================================================
//! 終了時セーブ
// ===========================================================================
void CRankSys::ProcEndSave()
{
	if (GetCount() == 0) {

		// セーブ開始
		DmSaveMenuStart();
	}

	// セーブ終了判定
	if (DmSaveIsExit()) {

		// 終了
		SetOwnProcNone();
	}
}

// ===========================================================================
//! エラー表示待ち
// ===========================================================================
void CRankSys::ProcErrorWait()
{
	// 既存メッセージ終了判定
	if (AoSysMsgIsFinished()) {

		// エラーメッセージ表示開始
		AoSysMsgStart(AOD_SYS_MSG_NET_ERROR_NOSAVE, AOD_SYS_MSG_SELECT_OK);

		// 表示中へ遷移
		SetOwnProc(&CRankSys::ProcError);
	}
}

// ===========================================================================
//! エラー表示中
// ===========================================================================
void CRankSys::ProcError()
{
	// 終了判定
	if (AoSysMsgIsFinished() && m_end) {
		SetOwnProc(&CRankSys::ProcEnd);
	}
}
#endif // _WII


// ***************************************************************************
// ユーティリティ
// ***************************************************************************
// ===========================================================================
//! 受信データを内部データにコピー
// ===========================================================================
void CRankSys::UtilRecvDataToData(
	DATA& dest, const AOS_NET_RANK_DATA* src) const
{
	dest.real_no = (u32)(src->rrank + 1);
	dest.rank_no = (u32)(src->rank + 1);
	strncpy(
		dest.name, AoNetRankUtilRecvDataGetName(src), CRankSys::DATA_NAME_LEN);
	dest.score = src->score;
	dest.ss = AoNetRankUtilRecvDataIsSs(src);
	dest.own = AoNetRankUtilRecvDataIsOwn(src);
	switch (src->region) {
	case AOD_NET_RANK_REGION_JP:
		dest.region = DMD_RANK_SYS_REGION_JP;
		break;
	case AOD_NET_RANK_REGION_US:
		dest.region = DMD_RANK_SYS_REGION_US;
		break;
	case AOD_NET_RANK_REGION_EU:
		dest.region = DMD_RANK_SYS_REGION_EU;
		break;
	default:
		dest.region = DMD_RANK_SYS_REGION_OTHER;
		break;
	}
#if _XBOX
	dest.xuid = src->xuid;
#endif // _XBOX
}


// ***************************************************************************
// パラメータ初期化
// ***************************************************************************
// ===========================================================================
//! ランキングデータ構造体初期化
// ===========================================================================
void CRankSys::InitParamData(CRankSys::DATA* data, u32 num)
{
	amZeroMemory(data, sizeof(CRankSys::DATA) * num);
}

// ===========================================================================
//! ボード情報構造体初期化
// ===========================================================================
void CRankSys::InitParamBoardInfo(CRankSys::BOARD_INFO& info)
{
	info.board = DMD_RANK_SYS_BOARD_NONE;
	info.rank = DMD_RANK_SYS_RANK_NONE;
	info.ss = DMD_RANK_SYS_SS_NONE;
}

// ===========================================================================
//! 通常ランキングデータ構造体初期化
// ===========================================================================
void CRankSys::InitParamNormalRank(CRankSys::NORMAL_RANK& rank)
{
	InitParamBoardInfo(rank.board);
	rank.is_down = FALSE;
	rank.is_near = FALSE;
	rank.num = 0;
	InitParamData(rank.data, DMD_RANK_SYS_SHOW_MAX);
}

// ===========================================================================
//! フレンドランキングデータ構造体初期化
// ===========================================================================
void CRankSys::InitParamFriendRank(CRankSys::FRIEND_RANK& rank)
{
	InitParamBoardInfo(rank.board);
	rank.is_down = FALSE;
	rank.num = 0;
	InitParamData(rank.data, DMD_RANK_SYS_FRIEND_MAX);
}

// ===========================================================================
//! リクエスト構造体初期化
// ===========================================================================
void CRankSys::InitParamRequest(CRankSys::REQUEST& req)
{
	InitParamBoardInfo(req.board);
	req.is_friend = FALSE;
	req.is_upload = FALSE;
	req.top_no = 0;
}

} // namespace dm


// ***************************************************************************
// 送受信制御
// ***************************************************************************
// ===========================================================================
//! 送受信可能判定
// ===========================================================================
BOOL dmRankSysWaitIsTransable(void)
{
	if (DmRankSysWaitGetWaitTime() == 0) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! 送受信通知
// ===========================================================================
void dmRankSysWaitSetTrans(void)
{
	DmRankSysWaitNoticeTrans();
}


// ***************************************************************************
// ユーティリティ
// ***************************************************************************
// ===========================================================================
//! ボート番号算出
// ===========================================================================
u32 dmRankSysUtilMakeBoardNo(
	DME_RANK_SYS_BOARD board, DME_RANK_SYS_RANK rank, DME_RANK_SYS_SS ss)
{
#if _WII

	UNREFERENCED_PARAMETER(rank);
	UNREFERENCED_PARAMETER(ss);
	return (u32)board;

#else

	return (u32)(
		(board * (DMD_RANK_SYS_RANK_NUM * DMD_RANK_SYS_SS_NUM)) +
		(rank * DMD_RANK_SYS_SS_NUM) +
		(ss));

#endif // _WII
}

// ===========================================================================
//! ボート昇順判定
// ===========================================================================
BOOL dmRankSysUtilIsBoardAsc(
	DME_RANK_SYS_BOARD board, DME_RANK_SYS_RANK rank, DME_RANK_SYS_SS ss)
{
	UNREFERENCED_PARAMETER(ss);
	if (rank == DMD_RANK_SYS_RANK_SCORE) {
		return FALSE;
	}
	if ((u32)board >= (u32)DMD_RANK_SYS_BOARD_SS_S) {
		return FALSE;
	}
	return TRUE;
}


// ***************************************************************************
// グローバル
// ***************************************************************************
// ===========================================================================
//! 指定のボードで記録が更新されているか判定
// ===========================================================================
BOOL dmRankSysGlbIsRecordUpdate(DME_RANK_SYS_BOARD board)
{
	if (dmRankSysGlbIsRecordUpdate(
			board, DMD_RANK_SYS_RANK_TIME, DMD_RANK_SYS_SS_DISABLE) ||
		dmRankSysGlbIsRecordUpdate(
			board, DMD_RANK_SYS_RANK_TIME, DMD_RANK_SYS_SS_ENABLE) ||
		dmRankSysGlbIsRecordUpdate(
			board, DMD_RANK_SYS_RANK_TIME, DMD_RANK_SYS_SS_BOTH) ||
		dmRankSysGlbIsRecordUpdate(
			board, DMD_RANK_SYS_RANK_SCORE, DMD_RANK_SYS_SS_DISABLE) ||
		dmRankSysGlbIsRecordUpdate(
			board, DMD_RANK_SYS_RANK_SCORE, DMD_RANK_SYS_SS_ENABLE) ||
		dmRankSysGlbIsRecordUpdate(
			board, DMD_RANK_SYS_RANK_SCORE, DMD_RANK_SYS_SS_BOTH))
	{
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! 指定のボードで記録が更新されているか判定(詳細)
// ===========================================================================
BOOL dmRankSysGlbIsRecordUpdate(
	DME_RANK_SYS_BOARD board, DME_RANK_SYS_RANK rank, DME_RANK_SYS_SS ss)
{
#if _WII

	if (rank != DMD_RANK_SYS_RANK_TIME) {
		return FALSE;
	}

	if ((u32)board < (u32)DMD_RANK_SYS_BOARD_SS_S) {
		// 通常ステージ
		if (ss != DMD_RANK_SYS_SS_BOTH) {
			return FALSE;
		}
		u32 index = dmRankSysGlbConvBoardIdToBackup(board);
		const gs::backup::SStageSolo& stage =
			gs::backup::SStage::CreateInstance()[index];
		if (stage.IsFastTimeEnable(stage.IsFastTimeUseSuperSonic())) {
			if (!stage.IsFastTimeUploaded(stage.IsFastTimeUseSuperSonic())) {
				return TRUE;
			}
		}
	}
	else {
		// スペシャルステージ
		if (ss != DMD_RANK_SYS_SS_DISABLE) {
			return FALSE;
		}
		u32 index = dmRankSysGlbConvBoardIdToBackup(board);
		const gs::backup::SSpecialSolo& stage =
			gs::backup::SSpecial::CreateInstance()[index];
		if (stage.IsFastTimeEnable()) {
			if (!stage.IsFastTimeUploaded()) {
				return TRUE;
			}
		}
	}

	return FALSE;

#else

	if ((u32)board < (u32)DMD_RANK_SYS_BOARD_SS_S) {
		// 通常ステージ
		u32 index = dmRankSysGlbConvBoardIdToBackup(board);
		const gs::backup::SStageSolo& stage =
			gs::backup::SStage::CreateInstance()[index];

		bool is_ss;
		switch (ss) {
		case DMD_RANK_SYS_SS_BOTH:
			if (rank == DMD_RANK_SYS_RANK_TIME) {
				if (stage.IsFastTimeUseSuperSonic()) {
					is_ss = true;
				}
				else {
					is_ss = false;
				}
			}
			else {
				if (stage.IsHighScoreUseSuperSonic()) {
					is_ss = true;
				}
				else {
					is_ss = false;
				}
			}
			break;

		case DMD_RANK_SYS_SS_DISABLE:
			is_ss = false;
			break;

		case DMD_RANK_SYS_SS_ENABLE:
			is_ss = true;
			break;

		default:
			amAssert(0);
			is_ss = false;
			break;
		}
		if (rank == DMD_RANK_SYS_RANK_TIME) {
			if (stage.IsFastTimeEnable(is_ss)) {
				if (!stage.IsFastTimeUploaded(is_ss)) {
					return TRUE;
				}
			}
		}
		else {
			if (stage.IsHighScoreEnable(is_ss)) {
				if (!stage.IsHighScoreUploaded(is_ss)) {
					return TRUE;
				}
			}
		}
	}
	else {
		// スペシャルステージ
		if (ss != DMD_RANK_SYS_SS_DISABLE) {
			return FALSE;
		}
		u32 index = dmRankSysGlbConvBoardIdToBackup(board);
		const gs::backup::SSpecialSolo& stage =
			gs::backup::SSpecial::CreateInstance()[index];
		if (rank == DMD_RANK_SYS_RANK_TIME) {
			if (stage.IsFastTimeEnable()) {
				if (!stage.IsFastTimeUploaded()) {
					return TRUE;
				}
			}
		}
		else {
			if (stage.IsHighScoreEnable()) {
				if (!stage.IsHighScoreUploaded()) {
					return TRUE;
				}
			}
		}
	}

	return FALSE;

#endif // _WII
}

// ===========================================================================
//! スーパーソニックが記録か判定
// ===========================================================================
static BOOL dmRankSysGlbIsRecordSs(
	DME_RANK_SYS_BOARD board, DME_RANK_SYS_RANK rank)
{
	if ((u32)board < (u32)DMD_RANK_SYS_BOARD_SS_S) {
		// 通常ステージ
		u32 index = dmRankSysGlbConvBoardIdToBackup(board);
		const gs::backup::SStageSolo& stage =
			gs::backup::SStage::CreateInstance()[index];
		if (rank == DMD_RANK_SYS_RANK_TIME) {
			if (stage.IsFastTimeUseSuperSonic()) {
				return TRUE;
			}
		}
		else {
			if (stage.IsHighScoreUseSuperSonic()) {
				return TRUE;
			}
		}
	}
	else {
		// スペシャルステージは通常ソニックのみ
		return FALSE;
	}

	return FALSE;
}

// ===========================================================================
//! 記録取得(スーパーソニック使用の有無別)
// ===========================================================================
u32 dmRankSysGlbGetRecord(
	DME_RANK_SYS_BOARD board, DME_RANK_SYS_RANK rank, BOOL ss)
{
	bool is_ss = ((ss) ? true : false);
	if ((u32)board < (u32)DMD_RANK_SYS_BOARD_SS_S) {
		// 通常ステージ
		u32 index = dmRankSysGlbConvBoardIdToBackup(board);
		const gs::backup::SStageSolo& stage =
			gs::backup::SStage::CreateInstance()[index];
		if (rank == DMD_RANK_SYS_RANK_TIME) {
			return stage.GetFastTime(is_ss);
		}
		else {
			return stage.GetHighScore(is_ss);
		}
	}
	else {
		// スペシャルステージ
		u32 index = dmRankSysGlbConvBoardIdToBackup(board);
		const gs::backup::SSpecialSolo& stage =
			gs::backup::SSpecial::CreateInstance()[index];
		if (rank == DMD_RANK_SYS_RANK_TIME) {
			return stage.GetFastTime();
		}
		else {
			return stage.GetHighScore();
		}
	}

	amAssert(0);
	return 0;
}

// ===========================================================================
//! アップロード済み通知
// ===========================================================================
void dmRankSysGlbSetRecordUpload(DME_RANK_SYS_BOARD board)
{
	if ((u32)board < (u32)DMD_RANK_SYS_BOARD_SS_S) {
		// 通常ステージ
		u32 index = dmRankSysGlbConvBoardIdToBackup(board);
		gs::backup::SStageSolo& stage =
			gs::backup::SStage::CreateInstance()[index];
		if (stage.IsFastTimeEnable(false)) {
			stage.SetFastTimeUploaded(false);
			stage.SetTimeUploadedOnce(true);
		}
		if (stage.IsFastTimeEnable(true)) {
			stage.SetFastTimeUploaded(true);
			stage.SetTimeUploadedOnce(true);
		}
		if (stage.IsHighScoreEnable(false)) {
			stage.SetHighScoreUploaded(false);
			stage.SetScoreUploadedOnce(true);
		}
		if (stage.IsHighScoreEnable(true)) {
			stage.SetHighScoreUploaded(true);
			stage.SetScoreUploadedOnce(true);
		}
	}
	else {
		// スペシャルステージ
		u32 index = dmRankSysGlbConvBoardIdToBackup(board);
		gs::backup::SSpecialSolo& stage =
			gs::backup::SSpecial::CreateInstance()[index];
		if (stage.IsFastTimeEnable()) {
			stage.SetFastTimeUploaded();
			stage.SetTimeUploadedOnce(true);
		}
		if (stage.IsHighScoreEnable()) {
			stage.SetHighScoreUploaded();
			stage.SetScoreUploadedOnce(true);
		}
	}
}

// ===========================================================================
//! ボードIDをgsBackupのインデックスに変換
// ===========================================================================
u32 dmRankSysGlbConvBoardIdToBackup(DME_RANK_SYS_BOARD board)
{
	u32 ret;
	if ((u32)board < (u32)DMD_RANK_SYS_BOARD_SS_S) {
		const u32 bu_id_tbl[] = {
			gs::backup::EStage::Zone1Act1,
			gs::backup::EStage::Zone1Act2,
			gs::backup::EStage::Zone1Act3,
			gs::backup::EStage::Zone1Boss,
			gs::backup::EStage::Zone2Act1,
			gs::backup::EStage::Zone2Act2,
			gs::backup::EStage::Zone2Act3,
			gs::backup::EStage::Zone2Boss,
			gs::backup::EStage::Zone3Act1,
			gs::backup::EStage::Zone3Act2,
			gs::backup::EStage::Zone3Act3,
			gs::backup::EStage::Zone3Boss,
			gs::backup::EStage::Zone4Act1,
			gs::backup::EStage::Zone4Act2,
			gs::backup::EStage::Zone4Act3,
			gs::backup::EStage::Zone4Boss,
			gs::backup::EStage::Final,
		};
		ret = bu_id_tbl[board];
	}
	else {
		const u32 bu_id_tbl[] = {
			gs::backup::ESpecialStage::Stage1,
			gs::backup::ESpecialStage::Stage2,
			gs::backup::ESpecialStage::Stage3,
			gs::backup::ESpecialStage::Stage4,
			gs::backup::ESpecialStage::Stage5,
			gs::backup::ESpecialStage::Stage6,
			gs::backup::ESpecialStage::Stage7,
		};
		ret = bu_id_tbl[board - DMD_RANK_SYS_BOARD_SS_S];
	}
	return ret;
}

// ===========================================================================
//! セーブ有効判定
// ===========================================================================
BOOL dmRankSysGlbIsSaveEnable(void)
{
	if (GsGetMainSysInfo()->is_save_run) {
		return TRUE;
	}
	return FALSE;
}


#if _WII
// ***************************************************************************
// Wii
// ***************************************************************************
// ===========================================================================
//! aoNetRankへのユーザ情報登録
// ===========================================================================
void dmRankSysWiiSetUserInfo(char* name, u32 len)
{
	UNREFERENCED_PARAMETER(len);

	// セーブデータから名前をコピー
	const u32 blen = gs::backup::SOption::c_name_length_limit;
	amAssert(blen < len);
	amCopyMemory(name, dmRankSysWiiGetUserName(), blen);
	name[blen] = '\0';

	// ユーザ名設定
	AoNetRankWiiSetUserName(name);

	// DWCユーザデータ設定
	AoNetRankWiiSetUserData(dmRankSysWiiGetDwcUserData());
}

// ===========================================================================
//! ユーザー名取得
// ===========================================================================
const char* dmRankSysWiiGetUserName(void)
{
	return gs::backup::SOption::CreateInstance().GetName();
}

// ===========================================================================
//! DWCユーザーデータ取得
// ===========================================================================
DWCUserData* dmRankSysWiiGetDwcUserData(void)
{
	return &gs::backup::SSystem::CreateInstance().GetDwcUserData();
}
#endif // _WII


#if _XBOX

// ***************************************************************************
// Xbox360
// ***************************************************************************
// ===========================================================================
//! ゲーマーカードUI表示00
// ===========================================================================
void dmRankSysXbox360TaskShowGamerCardUI00(AMS_TCB* tcb)
{
	// アカウント有効判定
	if (!AoAccountIsCurrentEnable()) {
		// 終了
		amTaskDelete(tcb);
		g_dm_rank_sys_gcui_tcb = NULL;
		return;
	}

	// XUID取得
	XUID xuid = *((XUID*)amTaskGetWork(tcb));

	// UI表示判定
	if (!AoSysIsShowPlatformUI()) {

		// アカウントID取得
		u32 id = (u32)AoAccountGetCurrentId();
		if (id >= 4) {
			// 終了
			amTaskDelete(tcb);
			g_dm_rank_sys_gcui_tcb = NULL;
			return;
		}

		// アカウントが有効か判定
		if (!AoAccountIsCurrentEnableRealXbox360()) {
			// 終了
			amTaskDelete(tcb);
			g_dm_rank_sys_gcui_tcb = NULL;
			return;
		}

		// UI表示
		DWORD result = XShowGamerCardUI(id, xuid);

		// エラー判定
		if (result != ERROR_SUCCESS) {
			// 終了
			amTaskDelete(tcb);
			g_dm_rank_sys_gcui_tcb = NULL;
		}
		else {
			// 表示終了待ちへ遷移
			amTaskSetProcedure(tcb, dmRankSysXbox360TaskShowGamerCardUI01);
		}
	}
}

// ===========================================================================
//! ゲーマーカードUI表示01
// ===========================================================================
void dmRankSysXbox360TaskShowGamerCardUI01(AMS_TCB* tcb)
{
	// 表示完了判定
	if (!AoSysIsShowPlatformUI()) {
		// 終了
		amTaskDelete(tcb);
		g_dm_rank_sys_gcui_tcb = NULL;
	}
}

// ===========================================================================
//! ゲーマーカードUI表示権限無し通知00
// ===========================================================================
void dmRankSysXbox360TaskShowGamerCardDisableUI00(AMS_TCB* tcb)
{
	// アカウント有効判定
	if (!AoAccountIsCurrentEnable()) {
		// 終了
		amTaskDelete(tcb);
		g_dm_rank_sys_gcui_tcb = NULL;
		return;
	}

	// UI表示判定
	if (!AoSysIsShowPlatformUI()) {

		// アカウントID取得
		u32 id = (u32)AoAccountGetCurrentId();
		if (id >= 4) {
			// 終了
			amTaskDelete(tcb);
			g_dm_rank_sys_gcui_tcb = NULL;
			return;
		}

		// アカウントが有効か判定
		if (!AoAccountIsCurrentEnableRealXbox360()) {
			// 終了
			amTaskDelete(tcb);
			g_dm_rank_sys_gcui_tcb = NULL;
			return;
		}

		// メッセージ表示開始
		AoSysMsgStart(AOD_SYS_MSG_ERROR_PRIVILEGE, AOD_SYS_MSG_SELECT_OK);

		// 表示終了待ちへ遷移
		amTaskSetProcedure(tcb, dmRankSysXbox360TaskShowGamerCardUI01);
	}
}

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
#endif //!_IPHONE