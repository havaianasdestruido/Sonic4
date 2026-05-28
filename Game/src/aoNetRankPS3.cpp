// ===========================================================================
/*!
	@file	aoNetRankPS3.cpp
	@brief	AoLibrary ネットワークランキングモジュール定義(PS3)

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"
#include "aoNetRank.h"
#include "gs.h"

#if defined(AOD_PLATFORM_PS3)

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct AOS_NET_RANK_USER_DATA
// ---------------------------------------------------------------------------
//!	ランキングユーザデータ
// ===========================================================================
typedef struct tag_AOS_NET_RANK_USER_DATA {
	u32				region;		//!< リージョン
	BOOL			is_ss;		//!< 真：スーパーソニック使用
} AOS_NET_RANK_USER_DATA;

// ----- Class Definitions -------------------------------------（クラス宣言）

namespace ao {
namespace net {

// ===========================================================================
//	class CRank
// ---------------------------------------------------------------------------
//!	ランキングクラス
// ===========================================================================
class CRank :
	public CProc<CRank>, public CTask<CRank>, public CThread<CRank>,
	public CAllocAmNormal
{
public:

	//! コンストラクタ
	CRank();

	//! デストラクタ
	virtual ~CRank();

	//! 接続完了判定
	BOOL IsConnected();

	//! 操作無効判定
	BOOL IsControlDisable() const;

	//! 終了開始
	void End();

	//! エラー判定
	BOOL IsError() const;


	//! 送信データ設定
	void SendSetData(
		u32 board, AOE_NET_RANK_TYPE type, const AOS_NET_RANK_SEND* data);

	//! 送信開始
	void SendStart();

	//! 送信データクリア
	void SendClear();

	//! 送信キャンセル通知
	void SendCancel();

	//! 送信終了判定
	BOOL SendIsFinished() const;

	//! 送信成功判定
	BOOL SendIsSuccessed() const;


	//! 受信開始
	void RecvStart(u32 board, AOE_NET_RANK_TYPE type, u32 rank);

	//! 受信キャンセル通知
	void RecvCancel();

	//! 受信終了判定
	BOOL RecvIsFinished() const;

	//! 受信成功判定
	BOOL RecvIsSuccessed() const;

	//! 受信データ数取得
	u32 RecvGetNum() const;

	//! 受信データ取得
	const AOS_NET_RANK_DATA* RecvGetData(u32 no);


	//! ネットワーク接続開始ダイアログ終了通知
	void NoticeNetStartDialogEnd();

protected:

	// タスクプロシージャ
	void TaskProcMain();

	// プロシージャ
	void ProcReady();
	void ProcConnecting();
	void ProcConnected();
	void ProcSending();
	void ProcRecving();
	void ProcRecvingNear();
	void ProcRecvingFriend();
	void ProcDisconnecting();
	void ProcError();
	void ProcErrorMessageWait();
	void ProcErrorMessage();

	// スレッドプロシージャ
	void ThreadProcConnect();
	int ThreadProcSubSend();
	int ThreadProcSubRecv();
	int ThreadProcSubRecvFriends();
	int ThreadProcSubRecvOwnRank();

	// ユーティリティ
	void UtilMakeRecvData(
		AOS_NET_RANK_DATA& dest,
		const SceNpScoreRankData& recv,
		const SceNpScoreGameInfo& recvu,
		AOE_NET_RANK_TYPE type);

	// エラー
	void SetError(u32 error, int sce_result);

	// 定数
	enum {
		// 状態
		STATE_CONNECTING	= 0,	//!< 接続中
		STATE_CONNECTED,			//!< 接続済み
		STATE_SENDING,				//!< 送信中
		STATE_RECVING,				//!< 受信中

		// エラー
		ERROR_NONE		= 0,	//!< エラー無し
		ERROR_OFFLINE,			//!< サインインできていない
		ERROR_PARENTAL,			//!< パレンタルロックされた
		ERROR_OPERATION,		//!< 不正な関数呼び出し
		ERROR_OTHER,			//!< その他のエラー
		ERROR_NUM,				//!< エラー数

		SEND_DATA_MAX	= 16,	//!< 送信データ最大数
		RECV_DATA_MAX	= 10,	//!< 受信データ最大数
		RECVF_DATA_MAX	= 101,	//!< フレンド受信データ最大数

		// 初期化済みフラグ
		INIT_LIBNET_MOD	= 0,	//!< libnetctlモジュールロード済み
		INIT_LIBNETCTL,			//!< libnetctl初期化済み
		INIT_NP_MOD,			//!< NPモジュールロード済み
		INIT_NP,				//!< NP初期化済み
		INIT_NP_SCORE,			//!< NPランキング初期化済み
		INIT_NP_TCTX,			//!< コンテキスト作成済み

		INIT_LIBNET_MOD_BIT	= (1 << INIT_LIBNET_MOD),//!< libnetctl初期化済み
		INIT_LIBNETCTL_BIT	= (1 << INIT_LIBNETCTL),//!< libnetctl初期化済み
		INIT_NP_MOD_BIT		= (1 << INIT_NP_MOD),	//!< NPモジュールロード済み
		INIT_NP_BIT			= (1 << INIT_NP),		//!< NP初期化済み
		INIT_NP_SCORE_BIT	= (1 << INIT_NP_SCORE),	//!< NPランキング初期化済み
		INIT_NP_TCTX_BIT	= (1 << INIT_NP_TCTX),	//!< コンテキスト作成済み
	};

	//! 送信情報
	struct SEND_INFO {
		u32					board_no;		//!< ボード番号
		AOE_NET_RANK_TYPE	type;			//!< 送信タイプ
		AOS_NET_RANK_SEND	data;			//!< 送信データ
	};

	u32 m_state; //!< 状態
	BOOL m_end_flag; //!< 終了フラグ
	u32 m_init_flag; //!< 初期化済みフラグ
	s32 m_title_ctx; //!< タイトルコンテキスト
	AMS_MUTEX m_mutex; //!< ミューテックス
	u32 m_error; //!< エラー
	u32 m_sce_error_code; //!< エラーコード

	// 送信関連
	BOOL m_send_execute; //!< 送信実行フラグ
	SEND_INFO m_send_tbl[SEND_DATA_MAX]; //!< 送信情報配列
	u32 m_send_num; //!< 送信情報数
	BOOL m_send_success; //!< 送信成功フラグ
	BOOL m_send_cancel;	//!< 送信キャンセルフラグ

	// 受信関連
	BOOL m_recv_execute; //!< 受信実行フラグ
	AOS_NET_RANK_DATA m_recv_data[RECVF_DATA_MAX]; //!< 受信データ
	AOE_NET_RANK_TYPE m_recv_type; //!< 受信ランキングタイプ
	BOOL m_recv_success; //!< 受信成功フラグ
	u32 m_recv_board_no; //!< 受信ランキングボード番号
	u32 m_recv_rank; //!< 受信ランク
	u32 m_recv_data_num; //!< 受信データ数
	BOOL m_recv_cancel;	//!< 受信キャンセルフラグ

	// 自身の順位受信関連
	BOOL m_recv_own_rank_execute; //!< 自身の順位受信実行フラグ
	s32 m_recv_own_rank; //!< 自身の順位
	u32 m_recv_all_rank_num; //!< ランキング登録数

	void* m_np_pool; //!< NP用バッファ
	u32 m_np_pool_size; //!< NP用バッファサイズ
	SceNpId m_np_id; //!< 自身のNPID

	BOOL m_is_net_start_dlg_end; //!<ネットワーク接続開始ダイアログ終了フラグ

	AMS_MUTEX m_cancel_mutex; //!< キャンセルフラグ用ミューテックス
};

} // namespace net
} // namespace ao

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

static BOOL aoNetRankIsUtilMessage(u32 error);
static BOOL aoNetRankIsOfflineMessage(u32 error);
static u32 aoNetRankConvBoardNo(u32 board);
static void aoNetRankSysCallbackNotice(
	uint64_t status, uint64_t param, void* arg);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//	CRank* g_ao_net_rank
// ---------------------------------------------------------------------------
//!	ランキングクラスポインタ
// ===========================================================================
static ao::net::CRank* g_ao_net_rank = NULL;

// ===========================================================================
//	BOOL g_ao_net_rank_error
// ---------------------------------------------------------------------------
//!	エラー
// ===========================================================================
static BOOL g_ao_net_rank_error = FALSE;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ***************************************************************************
// 開始処理 & 終了処理
// ***************************************************************************
// ===========================================================================
//	AoNetRankStart
/*!
	開始処理

	@note
	ネットワークランキングへアクセスするための準備処理を開始します。\n
	すでに開始済みの場合はアサートし何も行いません。\n
	準備処理の完了はAoNetRankIsConnected関数で判定して下さい。\n
*/
// ===========================================================================
void AoNetRankStart(void)
{
	if (g_ao_net_rank) {
		amAssert(0);
		return;
	}
	g_ao_net_rank = new ao::net::CRank();
}

// ===========================================================================
//	AoNetRankIsConnected
/*!
	接続済み判定

	@return 真：接続済み　偽：接続中
	@note
	AoNetRankStart関数で開始した準備処理の完了を判定します。\n
	AoNetRankStart関数を呼び出指定無い場合は、アサートしFALSEを返します。\n
*/
// ===========================================================================
BOOL AoNetRankIsConnected(void)
{
	if (g_ao_net_rank == NULL) {
		return FALSE;
	}
	return g_ao_net_rank->IsConnected();
}

// ===========================================================================
//	AoNetRankIsControlDisable
/*!
	ユーザ操作無効判定

	@return 真：無効　偽：それ以外
	@note
	モジュール内部でUI表示をしているなど、
	呼び出し側のユーザ操作を無効にしなければならない状態の場合に
	TRUEを返します。\n
*/
// ===========================================================================
BOOL AoNetRankIsControlDisable(void)
{
	if (g_ao_net_rank == NULL) {
		return FALSE;
	}
	return g_ao_net_rank->IsControlDisable();
}

// ===========================================================================
//	AoNetRankEnd
/*!
	終了処理

	@note
	ネットワークランキングへのアクセスを終了します。\n
	AoNetRankStart関数呼び出し以降であれば、
	どのタイミングでも呼び出し可能です。\n
	終了処理の完了はAoNetRankIsFinished関数で判定して下さい。\n
	どのような状態であっても、このモジュールを終了する際には、
	この関数を呼び出すようにして下さい。\n
	既に終了済みの場合はアサートし何も行いません。\n
*/
// ===========================================================================
void AoNetRankEnd(void)
{
	if (g_ao_net_rank == NULL) {
		return;
	}
	return g_ao_net_rank->End();
}

// ===========================================================================
//	AoNetRankIsFinished
/*!
	終了済み判定

	@return 真：終了済み　偽：終了中
	@note
	このモジュールが終了している（初期状態）か判定します。\n
	AoNetRankStart関数の呼び出しによりFALSEを返すようになり、
	AoNetRankEnd関数の呼出し後、
	一定時間が経過するとTRUEを返すようになります。\n
*/
// ===========================================================================
BOOL AoNetRankIsFinished(void)
{
	if (g_ao_net_rank) {
		return FALSE;
	}
	return TRUE;
}

// ===========================================================================
//	AoNetRankForcedEnding
/*!
	強制終了
*/
// ===========================================================================
void AoNetRankForcedEnding(void)
{
	if (g_ao_net_rank) {
		delete g_ao_net_rank;
		g_ao_net_rank = NULL;
	}
}


// ***************************************************************************
// エラー
// ***************************************************************************
// ===========================================================================
//	AoNetRankIsError
/*!
	エラー判定

	@return 真：エラーあり　偽：エラー無し
	@note
	通信エラーなど、このモジュール内でエラーが発生したかどうかを判定します。\n
	AoNetRankStart関数呼び出し直後にエラー内容はクリア(エラーなし)され、
	以後、エラーが発生してからAoNetRankClearError関数が呼び出されるまで
	TRUEを返します。\n
	AoNetRankEnd関数を呼び出してもエラー内容はクリアされません。\n
*/
// ===========================================================================
BOOL AoNetRankIsError(void)
{
	if (g_ao_net_rank_error) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//	AoNetRankClearError
/*!
	エラークリア

	@note
	AoNetRankIsError関数がTRUEを返す状態で呼び出すと、
	保持しているエラー情報をクリアし、
	AoNetRankIsError関数がFALSEを返すようになります。\n
	AoNetRankIsError関数がFALSEを返す状態で呼び出した場合は何も行いません。\n
*/
// ===========================================================================
void AoNetRankClearError(void)
{
	g_ao_net_rank_error = FALSE;
}


// ***************************************************************************
// 送信
// ***************************************************************************
// ===========================================================================
//	AoNetRankSendSetData
/*!
	ランキング送信データ設定

	@param board	[in] 送信先ランキングボード番号
	@param type		[in] ランキングタイプ
	@param data		[in] 送信データ
	@param is_asc	[in] 真：昇順　偽：降順
	@note
	指定のランキングボードへ送信する自身のデータを設定します。\n
	実際の送信はAoNetRankSend関数呼び出しにより行なわれます。\n
	複数回この関数を呼び出すと、最大16個までデータを積みます。\n
	内部でdataのコピーを保持するので、
	この関数呼出し後、dataは破棄して問題ありません。\n
	AoNetRankSendIsFinished関数とAoNetRankRecvIsFinished関数が
	全てTRUEを返す状態以外でこの関数を呼び出さないで下さい。\n
*/
// ===========================================================================
void AoNetRankSendSetData(
	u32 board, AOE_NET_RANK_TYPE type, const AOS_NET_RANK_SEND* data,
	BOOL is_asc)
{
	UNREFERENCED_PARAMETER(is_asc);

	if (g_ao_net_rank == NULL) {
		amAssert(0);
		return;
	}
	g_ao_net_rank->SendSetData(aoNetRankConvBoardNo(board), type, data);
}

// ===========================================================================
//	AoNetRankSendStart
/*!
	ランキング送信

	@note
	AoNetRankSendSetData関数で設定した送信データを送信します。\n
	AoNetRankSendIsFinished関数とAoNetRankRecvIsFinished関数が
	全てTRUEを返す状態以外でこの関数を呼び出さないで下さい。\n
*/
// ===========================================================================
void AoNetRankSendStart(void)
{
	if (g_ao_net_rank == NULL) {
		amAssert(0);
		return;
	}
	g_ao_net_rank->SendStart();
}

// ===========================================================================
//	AoNetRankSendClear
/*!
	ランキング送信データクリア

	@note
	AoNetRankSendSetData関数で設定した送信データをクリアします。\n
	送信処理が終了すると自動でクリアされるので呼び出す必要はありませんが、
	念のためAoNetRankSendSetData関数呼び出し前に呼び出すことをお勧めします。\n
*/
// ===========================================================================
void AoNetRankSendClear(void)
{
	if (g_ao_net_rank == NULL) {
		amAssert(0);
		return;
	}
	g_ao_net_rank->SendClear();
}

// ===========================================================================
//	AoNetRankSendCancel
/*!
	ランキングデータ送信キャンセル

	@note
	AoNetRankSendStart関数で開始した送信処理を中断します。\n
	この関数を呼び出しても、すぐに送信が終了するわけでは無いので、
	AoNetRankSendIsFinished関数で終了判定を行うようにして下さい。\n
	AoNetRankSendIsFinished関数がFALSEを返す間のみ呼び出し可能です。\n
	この関数を呼び出した場合は、
	AoNetRankSendIsComplate関数が必ずFALSEを返すようになります。\n
*/
// ===========================================================================
void AoNetRankSendCancel(void)
{
	if (g_ao_net_rank == NULL) {
		amAssert(0);
		return;
	}
	g_ao_net_rank->SendCancel();
}

// ===========================================================================
//	AoNetRankSendIsFinished
/*!
	ランキングデータ送信終了判定

	@return 真：終了済み　偽：送信中
	@note
	AoNetRankSendStart関数で開始した送信処理が終了したか判定します。\n
*/
// ===========================================================================
BOOL AoNetRankSendIsFinished(void)
{
	if (g_ao_net_rank == NULL) {
		amAssert(0);
		return TRUE;
	}
	return g_ao_net_rank->SendIsFinished();
}

// ===========================================================================
//	AoNetRankSendIsComplate
/*!
	ランキングデータ送信完了判定

	@return 真：送信完了できた　偽：できなかった
	@note
	AoNetRankSendStart関数で開始した送信処理により、
	データが正常にランキングに反映されたか判定します。\n
	この関数がFALSEを返す場合は、通信エラーにより送信ができなかったか、
	AoNetRankSendCancel関数で処理が中断されたかのどちらかとなります。\n
	この関数はAoNetRankSendIsFinished関数がTRUEを返す間のみ呼び出し可能です。\n
*/
// ===========================================================================
BOOL AoNetRankSendIsComplate(void)
{
	if (g_ao_net_rank == NULL) {
		amAssert(0);
		return TRUE;
	}
	return g_ao_net_rank->SendIsSuccessed();
}


// ***************************************************************************
// 受信
// ***************************************************************************
// ===========================================================================
//	AoNetRankRecvStart
/*!
	ランキングデータ受信開始

	@param board	[in] 受信元ランキングボード番号
	@param type		[in] ランキングタイプ
	@param rank		[in] 取得するランキングの先頭順位
	@param is_asc	[in] 真：昇順　偽：降順
	@note
	指定のランキングボードから指定のランキングデータを受信します。\n
	rankにAOD_NET_RANK_OWN_NEARを指定することで、
	自分の周辺のランキングを取得することができます。\n
	AoNetRankSendIsFinished関数とAoNetRankRecvIsFinished関数が
	全てTRUEを返す状態以外でこの関数を呼び出さないで下さい。\n
*/
// ===========================================================================
void AoNetRankRecvStart(
	u32 board, AOE_NET_RANK_TYPE type, u32 rank, BOOL is_asc)
{
	UNREFERENCED_PARAMETER(is_asc);

	if (g_ao_net_rank == NULL) {
		amAssert(0);
		return;
	}
	g_ao_net_rank->RecvStart(aoNetRankConvBoardNo(board), type, rank);
}

// ===========================================================================
//	AoNetRankRecvCancel
/*!
	ランキングデータ受信キャンセル

	@note
	AoNetRankRecvStart関数で開始した受信処理を中断します。\n
	この関数を呼び出しても、すぐに受信が終了するわけでは無いので、
	AoNetRankRecvIsFinished関数で終了判定を行うようにして下さい。\n
	AoNetRankRecvIsFinished関数がFALSEを返す間のみ呼び出し可能です。\n
	この関数を呼び出した場合は、
	AoNetRankRecvIsComplate関数が必ずFALSEを返すようになります。\n
*/
// ===========================================================================
void AoNetRankRecvCancel(void)
{
	if (g_ao_net_rank == NULL) {
		amAssert(0);
		return;
	}
	g_ao_net_rank->RecvCancel();
}

// ===========================================================================
//	AoNetRankRecvIsFinished
/*!
	ランキングデータ受信終了判定

	@return 真：終了済み　偽：送信中
	@note
	AoNetRankRecvStart関数で開始した受信処理が終了したか判定します。\n
*/
// ===========================================================================
BOOL AoNetRankRecvIsFinished(void)
{
	if (g_ao_net_rank == NULL) {
		amAssert(0);
		return TRUE;
	}
	return g_ao_net_rank->RecvIsFinished();
}

// ===========================================================================
//	AoNetRankRecvIsComplate
/*!
	ランキングデータ受信完了判定

	@return 真：受信完了できた　偽：できなかった
	@note
	AoNetRankRecvStart関数で開始した受信処理により、
	正常にランキングデータが取得できたか判定します。\n
	この関数がFALSEを返す場合は、通信エラーにより受信ができなかったか、
	AoNetRankRecvCancel関数で処理が中断されたかのどちらかとなります。\n
	この関数はAoNetRankRecvIsFinished関数がTRUEを返す間のみ呼び出し可能です。\n
*/
// ===========================================================================
BOOL AoNetRankRecvIsComplate(void)
{
	if (g_ao_net_rank == NULL) {
		amAssert(0);
		return TRUE;
	}
	return g_ao_net_rank->RecvIsSuccessed();
}

// ===========================================================================
//	AoNetRankRecvGetRecvNum
/*!
	受信データ数取得

	@return 受信データ数(最大AOD_NET_RANK_RECV_MAX)
	@note
	受信したデータ数を返します。\n
	通常はAOD_NET_RANK_RECV_MAXを返しますが、ランキングに登録されている人数が
	AOD_NET_RANK_RECV_MAXに満たない場合は
	AOD_NET_RANK_RECV_MAXより小さい数を返します。\n
	この関数はAoNetRankRecvIsComplate関数が
	TRUEを返す状態でのみ呼び出し可能です。\n
*/
// ===========================================================================
u32 AoNetRankRecvGetRecvNum(void)
{
	if (g_ao_net_rank == NULL) {
		amAssert(0);
		return 0;
	}
	return g_ao_net_rank->RecvGetNum();
}

// ===========================================================================
//	AoNetRankRecvGetData
/*!
	受信データ取得

	@param no		[in] 受信データ番号
	@return 受信データ
	@note
	受信データ番号には、0からAoNetRankRecvGetRecvNum関数の戻り値から
	1を引いた数値までを指定できます。\n
	受信データは次回AoNetRankRecvStart関数を呼び出すまで有効です。\n
*/
// ===========================================================================
const AOS_NET_RANK_DATA* AoNetRankRecvGetData(u32 no)
{
	if (g_ao_net_rank == NULL) {
		amAssert(0);
		return NULL;
	}
	return g_ao_net_rank->RecvGetData(no);
}


// ***************************************************************************
// ユーティリティ
// ***************************************************************************
// ===========================================================================
//	AoNetRankUtilSendDataMake
/*!
	受信データから名前取得

	@param data		[in] 受信データ
	@return 名前(終端文字含む)
*/
// ===========================================================================
const char* AoNetRankUtilRecvDataGetName(const AOS_NET_RANK_DATA* data)
{
	amAssert(data);
	return data->name;
}

// ===========================================================================
//	AoNetRankUtilRecvDataIsSs
/*!
	受信データからスーパーソニック使用の有無を判定

	@param data		[in] 受信データ
	@return 真：スーパーソニック使用　偽：未使用
*/
// ===========================================================================
BOOL AoNetRankUtilRecvDataIsSs(const AOS_NET_RANK_DATA* data)
{
	amAssert(data);
	if (data->flag & AOD_NET_RANK_FLAG_SS) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//	AoNetRankUtilRecvDataIsOwn
/*!
	受信データが自身のデータか判定

	@param data		[in] 受信データ
	@return 真：自身のデータである　偽：自身のデータではない
*/
// ===========================================================================
BOOL AoNetRankUtilRecvDataIsOwn(const AOS_NET_RANK_DATA* data)
{
	amAssert(data);
	if (data->flag & AOD_NET_RANK_FLAG_OWN) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//	AoNetRankUtilRecvDataGetTime
/*!
	受信データからタイム取得

	@param data		[in] 受信データ
	@return タイム
*/
// ===========================================================================
u32 AoNetRankUtilRecvDataGetTime(const AOS_NET_RANK_DATA* data)
{
	amAssert(data);
	return data->time;
}

// ===========================================================================
//	AoNetRankUtilRecvDataGetScore
/*!
	受信データからスコア取得

	@param data		[in] 受信データ
	@return スコア
*/
// ===========================================================================
u32 AoNetRankUtilRecvDataGetScore(const AOS_NET_RANK_DATA* data)
{
	amAssert(data);
	return data->score;
}

// ===========================================================================
//	AoNetRankUtilRecvDataGetRegion
/*!
	受信データからリージョン取得

	@param data		[in] 受信データ
	@return リージョン
*/
// ===========================================================================
AOE_NET_RANK_REGION AoNetRankUtilRecvDataGetRegion(
	const AOS_NET_RANK_DATA* data)
{
	if (data->region < (u32)AOD_NET_RANK_REGION_NUM) {
		return (AOE_NET_RANK_REGION)data->region;
	}
	return AOD_NET_RANK_REGION_OTHER;
}

// ===========================================================================
//	AoNetRankUtilRecvDataGetRank
/*!
	受信データから順位取得

	@param data		[in] 受信データ
	@return 順位
*/
// ===========================================================================
u32 AoNetRankUtilRecvDataGetRank(const AOS_NET_RANK_DATA* data)
{
	amAssert(data);
	return data->rank;
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace ao {

#if 1
// ===========================================================================
//! いろいろと問題があるのでスレッド関数を特殊化
// ===========================================================================
template <>
void CThread<net::CRank>::threadFunc(uint64_t arg)
{
	while (g_ao_net_rank == NULL) {
		// 10ミリ秒程度スリープ
		sys_timer_usleep(10 * 1000);
	}

	g_ao_net_rank->CallThreadProcedure(0);
}
#endif

namespace net {

// ***************************************************************************
// ランキングクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CRank::CRank()
{
	// メンバ初期化
	m_state = CRank::STATE_CONNECTING;
	m_end_flag = FALSE;
	m_init_flag = 0;
	m_title_ctx = -1;
	m_error = CRank::ERROR_NONE;
	m_sce_error_code = 0;

	m_send_execute = FALSE;
	amZeroMemory(m_send_tbl, sizeof(CRank::SEND_INFO) * CRank::SEND_DATA_MAX);
	m_send_num = 0;
	m_send_success = FALSE;
	m_send_cancel = FALSE;

	m_recv_execute = FALSE;
	amZeroMemory(m_recv_data, sizeof(AOS_NET_RANK_DATA) * RECVF_DATA_MAX);
	m_recv_type = AOD_NET_RANK_TYPE_NONE;
	m_recv_success = FALSE;
	m_recv_board_no = 0;
	m_recv_cancel = FALSE;

	m_recv_own_rank_execute = FALSE;
	m_recv_own_rank = -1;
	m_recv_all_rank_num = 0;

	// NP用バッファ作成
	m_np_pool_size = 128 * 1024;
	m_np_pool = amMemAlloc(m_np_pool_size);

	// ミューテックス作成
	amMutexCreate(&m_mutex);
	amMutexCreate(&m_cancel_mutex);

	// 準備プロシージャ設定
	SetProc(0, &CRank::ProcReady);

	// メインタスク作成
	MakeTask(0, "aoNetRank::Main");

	// メインタスクプロシージャ設定
	SetTaskProc(0, &CRank::TaskProcMain);

	// メインタスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CRank::~CRank()
{
	// スレッド終了待ち
	RequestEndThread(0);
	WaitEndThread(0);

	// ミューテックス削除
	amMutexDelete(&m_mutex);
	amMutexDelete(&m_cancel_mutex);

	// タイトルコンテキスト削除
	if (m_init_flag & INIT_NP_TCTX_BIT) {
		amAssert(0); // 既に終了済みのはず
		sceNpScoreDestroyTitleCtx(m_title_ctx);
		m_title_ctx = -1;
		m_init_flag &= ~INIT_NP_TCTX_BIT;
	}

	// NPスコアランキング終了
	if (m_init_flag & INIT_NP_SCORE_BIT) {
		amAssert(0); // 既に終了済みのはず
		sceNpScoreTerm();
		m_init_flag &= ~INIT_NP_SCORE_BIT;
	}

	// NP終了
	if (m_init_flag & INIT_NP_BIT) {
		amAssert(0); // 既に終了済みのはず
		sceNpTerm();
		m_init_flag &= ~INIT_NP_BIT;
	}

	// NPモジュールアンロード
	if (m_init_flag & INIT_NP_MOD_BIT) {
		amAssert(0); // 既に終了済みのはず
		cellSysmoduleUnloadModule(CELL_SYSMODULE_SYSUTIL_NP);
		m_init_flag &= ~INIT_NP_MOD_BIT;
	}

	// NP用バッファ解放
	if (m_np_pool) {
		amMemFree(m_np_pool);
		m_np_pool = NULL;
	}

	// libnetctl終了
	if (m_init_flag & INIT_LIBNETCTL_BIT) {
		amAssert(0); // 既に終了済みのはず
		cellNetCtlTerm();
		m_init_flag &= ~INIT_LIBNETCTL_BIT;
	}

	// libnetctlモジュールアンロード
	if (m_init_flag & INIT_LIBNET_MOD_BIT) {
		amAssert(0); // 既に終了済みのはず
		cellSysmoduleUnloadModule(CELL_SYSMODULE_NET);
		m_init_flag &= ~INIT_LIBNET_MOD_BIT;
	}

	g_ao_net_rank = NULL;
}

// ===========================================================================
//! 接続完了判定
// ===========================================================================
BOOL CRank::IsConnected()
{
	BOOL ret;
	amMutexLock(&m_mutex);
	if (m_state != CRank::STATE_CONNECTING) {
		ret = TRUE;
	}
	else {
		ret = FALSE;
	}
	amMutexUnlock(&m_mutex);
	return ret;
}

// ===========================================================================
//! 操作無効判定
// ===========================================================================
BOOL CRank::IsControlDisable() const
{
	// エラーが発生して以降は操作不能にする
	if (IsError()) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! 終了開始
// ===========================================================================
void CRank::End()
{
	m_end_flag = TRUE;
}

// ===========================================================================
//! エラー判定
// ===========================================================================
BOOL CRank::IsError() const
{
	if (m_error != CRank::ERROR_NONE) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! 送信データ設定
// ===========================================================================
void CRank::SendSetData(
	u32 board, AOE_NET_RANK_TYPE type, const AOS_NET_RANK_SEND* data)
{
	// 状態チェック
	if (!SendIsFinished()) {
		amAssert(0);
		return;
	}
	if (m_send_num >= CRank::SEND_DATA_MAX) {
		amAssert(0);
		return;
	}

	m_send_tbl[m_send_num].board_no = board;
	m_send_tbl[m_send_num].type = type;
	amCopyMemory(
		&m_send_tbl[m_send_num].data, data, sizeof(AOS_NET_RANK_SEND));
	m_send_num += 1;
}

// ===========================================================================
//! 送信開始
// ===========================================================================
void CRank::SendStart()
{
	// 状態チェック
	if (m_state != CRank::STATE_CONNECTED) {
		amAssert(0);
		return;
	}

	// 送信数が0なら何もしない
	if (m_send_num == 0) {
		SendClear();
		m_send_success = TRUE;
		return;
	}

	// 送信中設定
	m_state = CRank::STATE_SENDING;
	m_send_success = FALSE;
	m_send_cancel = FALSE;

	// 送信中へ遷移
	SetOwnProc(&CRank::ProcSending);
}

// ===========================================================================
//! 送信データクリア
// ===========================================================================
void CRank::SendClear()
{
	if (SendIsFinished()) {
		m_send_num = 0;
	}
}

// ===========================================================================
//! 送信キャンセル通知
// ===========================================================================
void CRank::SendCancel()
{
	amMutexLock(&m_cancel_mutex);
	m_send_cancel = TRUE;
	amMutexUnlock(&m_cancel_mutex);
}

// ===========================================================================
//! 送信終了判定
// ===========================================================================
BOOL CRank::SendIsFinished() const
{
	if (m_state != CRank::STATE_SENDING) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! 送信成功判定
// ===========================================================================
BOOL CRank::SendIsSuccessed() const
{
	return m_send_success;
}

// ===========================================================================
//! 受信開始
// ===========================================================================
void CRank::RecvStart(u32 board, AOE_NET_RANK_TYPE type, u32 rank)
{
	// 状態チェック
	if (m_state != CRank::STATE_CONNECTED) {
		amAssert(0);
		return;
	}

	// 受信情報取得
	m_recv_type = type;
	m_recv_success = FALSE;
	m_recv_board_no = board;
	m_recv_rank = rank;
	m_recv_cancel = FALSE;

	// 受信中設定
	m_state = CRank::STATE_RECVING;

	// 受信中へ遷移
	if (rank == AOD_NET_RANK_OWN_NEAR) {
		SetOwnProc(&CRank::ProcRecvingNear);
	}
	else if (rank == AOD_NET_RANK_FRIENDS) {
		SetOwnProc(&CRank::ProcRecvingFriend);
	}
	else {
		SetOwnProc(&CRank::ProcRecving);
	}
}

// ===========================================================================
//! 受信キャンセル通知
// ===========================================================================
void CRank::RecvCancel()
{
	amMutexLock(&m_cancel_mutex);
	m_recv_cancel = TRUE;
	amMutexUnlock(&m_cancel_mutex);
}

// ===========================================================================
//! 受信終了判定
// ===========================================================================
BOOL CRank::RecvIsFinished() const
{
	if (m_state != CRank::STATE_RECVING) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! 受信成功判定
// ===========================================================================
BOOL CRank::RecvIsSuccessed() const
{
	return m_recv_success;
}

// ===========================================================================
//! 受信データ数取得
// ===========================================================================
u32 CRank::RecvGetNum() const
{
	return m_recv_data_num;
}

// ===========================================================================
//! 受信データ取得
// ===========================================================================
const AOS_NET_RANK_DATA* CRank::RecvGetData(u32 no)
{
	amAssert(no < RecvGetNum());
	return &m_recv_data[no];
}

// ===========================================================================
//! ネットワーク接続開始ダイアログ終了通知
// ===========================================================================
void CRank::NoticeNetStartDialogEnd()
{
	m_is_net_start_dlg_end = TRUE;
}


// ***************************************************************************
// タスクプロシージャ
// ***************************************************************************
// ===========================================================================
//! メインタスク
// ===========================================================================
void CRank::TaskProcMain()
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
//! 準備
// ===========================================================================
void CRank::ProcReady()
{
	// スレッド作成
	SetThreadProc(0, &CRank::ThreadProcConnect);
	s32 prio;
	sys_ppu_thread_t id;
	sys_ppu_thread_get_id(&id);
	sys_ppu_thread_get_priority(id, &prio);
	prio += 1;
	StartThread(0, (AMD_CORE)0, (u32)prio);

	// 接続中へ遷移
	SetOwnProc(&CRank::ProcConnecting);
}

// ===========================================================================
//! 接続中
// ===========================================================================
void CRank::ProcConnecting()
{
	amMutexLock(&m_mutex);
	// エラー判定
	if (IsError()) {
		SetOwnProc(&CRank::ProcError);
		amMutexUnlock(&m_mutex);
		return;
	}
	amMutexUnlock(&m_mutex);

	// キャンセル判定
	if (m_end_flag) {
		SetOwnProc(&CRank::ProcDisconnecting);
		return;
	}

	// 接続済み判定
	amMutexLock(&m_mutex);
	if (m_state == CRank::STATE_CONNECTED) {
		// 接続済みへ遷移
		SetOwnProc(&CRank::ProcConnected);
	}
	amMutexUnlock(&m_mutex);
}

// ===========================================================================
//! 接続済み
// ===========================================================================
void CRank::ProcConnected()
{
	amMutexLock(&m_mutex);
	// エラー判定
	if (IsError()) {
		SetOwnProc(&CRank::ProcError);
		amMutexUnlock(&m_mutex);
		return;
	}
	amMutexUnlock(&m_mutex);

	// キャンセル判定
	if (m_end_flag) {
		SetOwnProc(&CRank::ProcDisconnecting);
		return;
	}

	// empty
}

// ===========================================================================
//! 送信中
// ===========================================================================
void CRank::ProcSending()
{
	amMutexLock(&m_mutex);
	// エラー判定
	if (IsError()) {
		m_state = CRank::STATE_CONNECTED;
		SetOwnProc(&CRank::ProcError);
		amMutexUnlock(&m_mutex);
		return;
	}
	amMutexUnlock(&m_mutex);

	if (GetCount() == 0) {

		// 送信実行フラグ設定
		amMutexLock(&m_mutex);
		m_send_execute = TRUE;
		amMutexUnlock(&m_mutex);
	}

	amMutexLock(&m_mutex);
	// 送信完了判定
	if (!m_send_execute) {
		// 送信終了
		if (IsError()) {
			// エラー発生
			m_state = CRank::STATE_CONNECTED;
			SetOwnProc(&CRank::ProcError);
		}
		else {
			// 正常終了
			m_send_success = TRUE;
			m_state = CRank::STATE_CONNECTED;
			SetOwnProc(&CRank::ProcConnected);
		}
	}
	amMutexUnlock(&m_mutex);
}

// ===========================================================================
//! 受信中
// ===========================================================================
void CRank::ProcRecving()
{
	amMutexLock(&m_mutex);
	// エラー判定
	if (IsError()) {
		m_state = CRank::STATE_CONNECTED;
		SetOwnProc(&CRank::ProcError);
		amMutexUnlock(&m_mutex);
		return;
	}
	amMutexUnlock(&m_mutex);

	if (GetCount() == 0) {

		// 送信実行フラグ設定
		amMutexLock(&m_mutex);
		m_recv_execute = TRUE;
		amMutexUnlock(&m_mutex);
	}

	amMutexLock(&m_mutex);
	// 送信完了判定
	if (!m_recv_execute) {
		// 送信終了
		if (IsError()) {
			// エラー発生
			m_state = CRank::STATE_CONNECTED;
			SetOwnProc(&CRank::ProcError);
		}
		else {
			// 正常終了
			m_recv_success = TRUE;
			m_state = CRank::STATE_CONNECTED;
			SetOwnProc(&CRank::ProcConnected);
		}
	}
	amMutexUnlock(&m_mutex);
}

// ===========================================================================
//! 受信中(周辺ランキング)
// ===========================================================================
void CRank::ProcRecvingNear()
{
	amMutexLock(&m_mutex);
	// エラー判定
	if (IsError()) {
		m_state = CRank::STATE_CONNECTED;
		SetOwnProc(&CRank::ProcError);
		amMutexUnlock(&m_mutex);
		return;
	}
	amMutexUnlock(&m_mutex);

	if (GetCount() == 0) {

		// 自身の順位取得実行フラグ設定
		amMutexLock(&m_mutex);
		m_recv_own_rank_execute = TRUE;
		amMutexUnlock(&m_mutex);
	}

	amMutexLock(&m_mutex);
	// 送信完了判定
	if (!m_recv_own_rank_execute) {
		// 送信終了
		if (IsError()) {
			// エラー発生
			m_state = CRank::STATE_CONNECTED;
			SetOwnProc(&CRank::ProcError);
		}
		else if (m_recv_own_rank < 0) {
			// 自身はランキングされていない
			m_recv_data_num = 0;
			m_recv_success = TRUE;
			m_state = CRank::STATE_CONNECTED;
			SetOwnProc(&CRank::ProcConnected);
		}
		else {
			// 周辺受信開始
			if (m_recv_own_rank <= 4) {
				m_recv_rank = 0;
			}
			else if ((m_recv_all_rank_num - (u32)m_recv_own_rank) < 6) {
				if (m_recv_all_rank_num < CRank::RECV_DATA_MAX) {
					m_recv_rank = 0;
				}
				else {
					m_recv_rank = m_recv_all_rank_num - CRank::RECV_DATA_MAX;
				}
			}
			else {
				m_recv_rank = m_recv_own_rank - 4;
			}
			SetOwnProc(&CRank::ProcRecving);
		}
	}
	amMutexUnlock(&m_mutex);
}

// ===========================================================================
//! 受信中(フレンドランキング)
// ===========================================================================
void CRank::ProcRecvingFriend()
{
	ProcRecving();
}

// ===========================================================================
//! 切断中
// ===========================================================================
void CRank::ProcDisconnecting()
{
	if (GetCount() == 0) {
		// スレッド終了要求
		RequestEndThread(0);
	}

	// スレッド終了待ち
	if (IsEndThread(0)) {

		// エラー判定
		if (IsError()) {
			// エラーメッセージ表示へ遷移
			SetOwnProc(&CRank::ProcErrorMessageWait);
			return;
		}

		// 終了
		SetOwnProcNone();
	}
}

// ===========================================================================
//! エラー
// ===========================================================================
void CRank::ProcError()
{
	if (GetCount() == 0) {
		// スレッド終了要求
		RequestEndThread(0);
	}

	// スレッド終了待ち
	if (IsEndThread(0)) {
		// エラーメッセージ表示へ遷移
		SetOwnProc(&CRank::ProcErrorMessageWait);
	}
}

// ===========================================================================
//! エラーメッセージ表示待ち
// ===========================================================================
void CRank::ProcErrorMessageWait()
{
	// 既存メッセージ表示終了待ち
	if (AoSysMsgIsFinished()) {
		// エラー表示開始
		if (m_error == CRank::ERROR_OFFLINE) {
			m_sce_error_code = SCE_NP_ERROR_OFFLINE;
		}

		if (m_error == CRank::ERROR_PARENTAL) {
			// 年齢制限
			AoSysMsgStart(
				AOD_SYS_MSG_NET_ERROR_PARENTALLOCK, AOD_SYS_MSG_SELECT_OK);
		}
		else if (aoNetRankIsUtilMessage(m_sce_error_code)) {
			// 専用文言あり
			AoSysMsgStartSceError(m_sce_error_code);
		}
		else if (aoNetRankIsOfflineMessage(m_sce_error_code)) {
			// 未サインイン
			AoSysMsgStart(
				AOD_SYS_MSG_NET_ERROR_OFFLINE, AOD_SYS_MSG_SELECT_OK);
		}
		else {
			// 汎用エラーメッセージ
			AoSysMsgStart(AOD_SYS_MSG_NET_ERROR_COMMON, AOD_SYS_MSG_SELECT_OK);
		}

		// エラーメッセージ表示中へ遷移
		SetOwnProc(&CRank::ProcErrorMessage);
	}
}

// ===========================================================================
//! エラーメッセージ
// ===========================================================================
void CRank::ProcErrorMessage()
{
	// エラー表示終了待ち
	if (AoSysMsgIsFinished()) {

		// 専用文言だった場合は
		// ランキングメニューから抜ける旨のメッセージを表示する
		if (aoNetRankIsUtilMessage(m_sce_error_code)) {
			// 汎用エラーメッセージ
			AoSysMsgStart(AOD_SYS_MSG_NET_ERROR_COMMON, AOD_SYS_MSG_SELECT_OK);
			m_sce_error_code = 0;
		}
		else {
			// 終了
			SetOwnProcNone();
		}
	}
}


// ***************************************************************************
// スレッドプロシージャ
// ***************************************************************************
// ===========================================================================
//! 接続スレッド
// ===========================================================================
void CRank::ThreadProcConnect()
{
	int result;
	u32 flag;
	CellNetCtlNetStartDialogParam net_dlg_param;
	CellNetCtlNetStartDialogResult net_dlg_result;
	u32 sys_cb_id = (u32)-1;
	int isRestricted, age;

	// libnetctlモジュールロード
	if (cellSysmoduleIsLoaded(CELL_SYSMODULE_NET) != 0) {
		result = cellSysmoduleLoadModule(CELL_SYSMODULE_NET);
		if (result < 0) {
			// エラー
			amMutexLock(&m_mutex);
			SetError(CRank::ERROR_OTHER, result);
			amMutexUnlock(&m_mutex);
			goto wait;
		}
		amMutexLock(&m_mutex);
		m_init_flag |= INIT_LIBNET_MOD_BIT;
		amMutexUnlock(&m_mutex);
	}

	// libnetctl初期化
	result = cellNetCtlInit();
	if ((u32)result != CELL_NET_CTL_ERROR_NOT_TERMINATED) {
		if (result != 0) {
			// エラー
			amMutexLock(&m_mutex);
			SetError(CRank::ERROR_OTHER, result);
			amMutexUnlock(&m_mutex);
			goto wait;
		}
		amMutexLock(&m_mutex);
		m_init_flag |= INIT_LIBNETCTL_BIT;
		amMutexUnlock(&m_mutex);
	}

	// ネットワーク開始ダイアログ用システムコールバック登録
	m_is_net_start_dlg_end = FALSE;
	sys_cb_id = AoSysPS3AddSystemCallbackNotice(
		aoNetRankSysCallbackNotice, this);

	// ネットワーク開始ダイアログ表示
	amZeroMemory(&net_dlg_param, sizeof(CellNetCtlNetStartDialogParam));
	net_dlg_param.size = sizeof(CellNetCtlNetStartDialogParam);
	net_dlg_param.type = CELL_NET_CTL_NETSTART_TYPE_NP;
	result = cellNetCtlNetStartDialogLoadAsync(&net_dlg_param);
	if (result != 0) {
		// エラー
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_OTHER, result);
		amMutexUnlock(&m_mutex);
		goto wait;
	}

	// ネットワーク開始ダイアログ終了待ち
	while (!m_is_net_start_dlg_end) {

		// 終了判定
		if (IsRequestEndThread()) {
			// ネットワーク開始ダイアログ強制終了
			cellNetCtlNetStartDialogAbortAsync();
			break;
		}

		// 100ミリ秒程度スリープ
		sys_timer_usleep(100 * 1000);
	}

	// ネットワーク開始ダイアログ終了
	amZeroMemory(&net_dlg_result, sizeof(CellNetCtlNetStartDialogResult));
	net_dlg_result.size = sizeof(CellNetCtlNetStartDialogResult);
	result = cellNetCtlNetStartDialogUnloadAsync(&net_dlg_result);
	if (result != 0) {
		// エラー
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_OTHER, result);
		amMutexUnlock(&m_mutex);
		goto wait;
	}
	if (net_dlg_result.result != 0) {
		// 未サインイン
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_OFFLINE, net_dlg_result.result);
		amMutexUnlock(&m_mutex);
		goto wait;
	}

	// ネットワーク開始ダイアログ用システムコールバック登録解除
	AoSysPS3DelSystemCallbackNotice(sys_cb_id);
	sys_cb_id = (u32)-1;

	// NPモジュールロード
	if (cellSysmoduleIsLoaded(CELL_SYSMODULE_SYSUTIL_NP) != 0) {
		result = cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP);
		if (result < 0) {
			// エラー
			amMutexLock(&m_mutex);
			SetError(CRank::ERROR_OTHER, result);
			amMutexUnlock(&m_mutex);
			goto wait;
		}
		amMutexLock(&m_mutex);
		m_init_flag |= INIT_NP_MOD_BIT;
		amMutexUnlock(&m_mutex);
	}

	// NP初期化
	result = sceNpInit(m_np_pool_size, m_np_pool);
	if (result < 0) {
		// エラー
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_OTHER, result);
		amMutexUnlock(&m_mutex);
		goto wait;
	}
	amMutexLock(&m_mutex);
	m_init_flag |= INIT_NP_BIT;
	amMutexUnlock(&m_mutex);

	// パレンタルロック
	result = sceNpManagerGetContentRatingFlag(&isRestricted, &age);
	if (result < 0) {
		// エラー
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_OTHER, result);
		amMutexUnlock(&m_mutex);
		goto wait;
	}
	if (isRestricted && (age < GsEnvGetPs3ParentalLockAge())) {
		// 年齢制限
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_PARENTAL, 0);
		amMutexUnlock(&m_mutex);
		goto wait;
	}

	// NPスコアランキング初期化
	result = sceNpScoreInit();
	if (result < 0) {
		// エラー
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_OTHER, result);
		amMutexUnlock(&m_mutex);
		goto wait;
	}
	amMutexLock(&m_mutex);
	m_init_flag |= INIT_NP_SCORE_BIT;
	amMutexUnlock(&m_mutex);

	// NPID取得
	result = sceNpManagerGetNpId(&m_np_id);
	if (result < 0) {
		// エラー
		amMutexLock(&m_mutex);
		switch (result) {
		case SCE_NP_ERROR_INVALID_STATE:
		case SCE_NP_ERROR_OFFLINE:
			// 未サインイン
			SetError(CRank::ERROR_OFFLINE, result);
			break;
		default:
			SetError(CRank::ERROR_OTHER, result);
			break;
		}
		amMutexUnlock(&m_mutex);
		goto wait;
	}

	// タイトルコンテキスト作成
	result = sceNpScoreCreateTitleCtx(
		GsEnvGetPs3ComId(),
		GsEnvGetPs3ComPassphrase(),
		&m_np_id);
	if (result < 0) {
		// エラー
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_OTHER, result);
		amMutexUnlock(&m_mutex);
		goto wait;
	}
	m_title_ctx = result;
	amMutexLock(&m_mutex);
	m_init_flag |= INIT_NP_TCTX_BIT;
	amMutexUnlock(&m_mutex);

	// 接続済み設定
	amMutexLock(&m_mutex);
	m_send_execute = FALSE;
	m_recv_execute = FALSE;
	m_recv_own_rank_execute = FALSE;
	m_state = CRank::STATE_CONNECTED;
	amMutexUnlock(&m_mutex);

wait:

	// 接続済み
	while (1) {

		BOOL is_execute;

		// 終了判定
		if (IsRequestEndThread()) {
			goto end;
		}

		// 送信処理判定
		amMutexLock(&m_mutex);
		is_execute = m_send_execute;
		amMutexUnlock(&m_mutex);
		if (is_execute) {

			// 送信処理
			result = ThreadProcSubSend();
			if (result < 0) {
				// エラー
				amMutexLock(&m_mutex);
				SetError(CRank::ERROR_OTHER, result);
				amMutexUnlock(&m_mutex);
				// 切断開始処理はメインスレッドから行なうのでエラー設定のみ
			}

			// 終了
			amMutexLock(&m_mutex);
			m_send_execute = FALSE;
			amMutexUnlock(&m_mutex);
		}

		// 受信処理判定
		amMutexLock(&m_mutex);
		is_execute = m_recv_execute;
		amMutexUnlock(&m_mutex);
		if (is_execute) {

			// 受信処理
			if (m_recv_rank == AOD_NET_RANK_FRIENDS) {
				result = ThreadProcSubRecvFriends();
			}
			else {
				result = ThreadProcSubRecv();
			}
			if (result < 0) {
				// エラー
				amMutexLock(&m_mutex);
				SetError(CRank::ERROR_OTHER, result);
				amMutexUnlock(&m_mutex);
				// 切断開始処理はメインスレッドから行なうのでエラー設定のみ
			}

			// 終了
			amMutexLock(&m_mutex);
			m_recv_execute = FALSE;
			amMutexUnlock(&m_mutex);
		}

		// 自身の順位受信処理判定
		amMutexLock(&m_mutex);
		is_execute = m_recv_own_rank_execute;
		amMutexUnlock(&m_mutex);
		if (is_execute) {

			// 受信処理
			result = ThreadProcSubRecvOwnRank();
			if (result < 0) {
				// エラー
				amMutexLock(&m_mutex);
				SetError(CRank::ERROR_OTHER, result);
				amMutexUnlock(&m_mutex);
				// 切断開始処理はメインスレッドから行なうのでエラー設定のみ
			}

			// 終了
			amMutexLock(&m_mutex);
			m_recv_own_rank_execute = FALSE;
			amMutexUnlock(&m_mutex);
		}

		// 100ミリ秒程度スリープ
		sys_timer_usleep(100 * 1000);
	}

end:

	// タイトルコンテキスト削除
	amMutexLock(&m_mutex);
	flag = m_init_flag;
	amMutexUnlock(&m_mutex);
	if (flag & INIT_NP_TCTX_BIT) {
		result = sceNpScoreDestroyTitleCtx(m_title_ctx);
		m_title_ctx = -1;
		amMutexLock(&m_mutex);
		if (result < 0) {
			SetError(CRank::ERROR_OTHER, result);
		}
		m_init_flag &= ~INIT_NP_TCTX_BIT;
		amMutexUnlock(&m_mutex);
	}

	// NPスコアランキング終了
	amMutexLock(&m_mutex);
	flag = m_init_flag;
	amMutexUnlock(&m_mutex);
	if (flag & INIT_NP_SCORE_BIT) {
		result = sceNpScoreTerm();
		amMutexLock(&m_mutex);
		if (result < 0) {
			SetError(CRank::ERROR_OTHER, result);
		}
		m_init_flag &= ~INIT_NP_SCORE_BIT;
		amMutexUnlock(&m_mutex);
	}

	// NP終了
	amMutexLock(&m_mutex);
	flag = m_init_flag;
	amMutexUnlock(&m_mutex);
	if (flag & INIT_NP_BIT) {
		result = sceNpTerm();
		amMutexLock(&m_mutex);
		if (result < 0) {
			SetError(CRank::ERROR_OTHER, result);
		}
		m_init_flag &= ~INIT_NP_BIT;
		amMutexUnlock(&m_mutex);
	}

	// NPモジュールアンロード
	amMutexLock(&m_mutex);
	flag = m_init_flag;
	amMutexUnlock(&m_mutex);
	if (flag & INIT_NP_MOD_BIT) {
		result = cellSysmoduleUnloadModule(CELL_SYSMODULE_SYSUTIL_NP);
		amMutexLock(&m_mutex);
		if (result < 0) {
			SetError(CRank::ERROR_OTHER, result);
		}
		m_init_flag &= ~INIT_NP_MOD_BIT;
		amMutexUnlock(&m_mutex);
	}

	// ネットワーク開始ダイアログ用システムコールバック登録解除
	if (sys_cb_id != (u32)-1) {
		AoSysPS3DelSystemCallbackNotice(sys_cb_id);
		sys_cb_id = (u32)-1;
	}

	// libnetctl終了処理
	amMutexLock(&m_mutex);
	flag = m_init_flag;
	amMutexUnlock(&m_mutex);
	if (flag & (INIT_LIBNETCTL_BIT | INIT_LIBNET_MOD_BIT)) {
		cellNetCtlTerm();
		amMutexLock(&m_mutex);
		m_init_flag &= ~INIT_LIBNETCTL_BIT;
		amMutexUnlock(&m_mutex);
	}

	// libnetctlモジュールアンロード
	amMutexLock(&m_mutex);
	flag = m_init_flag;
	amMutexUnlock(&m_mutex);
	if (flag & INIT_LIBNET_MOD_BIT) {
		result = cellSysmoduleUnloadModule(CELL_SYSMODULE_NET);
		amMutexLock(&m_mutex);
		if (result < 0) {
			SetError(CRank::ERROR_OTHER, result);
		}
		m_init_flag &= ~INIT_LIBNET_MOD_BIT;
		amMutexUnlock(&m_mutex);
	}
}

// ===========================================================================
//! 接続スレッド - 送信処理
// ===========================================================================
int CRank::ThreadProcSubSend()
{
	int sce_result = 0;
	int trans_ctx_id = -1;
	int result;
	SceNpScoreComment np_comment;
	SceNpScoreGameInfo np_gameinfo;
	SceNpScoreRankNumber np_tmp_rank;
	BOOL is_cancel;

	int thprio;
	sys_ppu_thread_t thid;
	sys_ppu_thread_get_id(&thid);
	sys_ppu_thread_get_priority(thid, &thprio);

	while (m_send_num > 0) {

		amMutexLock(&m_cancel_mutex);
		is_cancel = m_send_cancel;
		amMutexUnlock(&m_cancel_mutex);
		if (is_cancel || IsRequestEndThread()) {
			goto end;
		}

		const CRank::SEND_INFO& send_info = m_send_tbl[m_send_num - 1];
		m_send_num -= 1;

		amZeroMemory(&np_comment, sizeof(SceNpScoreComment));
		amZeroMemory(&np_gameinfo, sizeof(SceNpScoreGameInfo));
		np_tmp_rank = 0;

		// ユーザデータ作成
		AOS_NET_RANK_USER_DATA* userdata =
			(AOS_NET_RANK_USER_DATA*)(&np_gameinfo.nativeData[0]);
		switch (GsEnvGetRegion()) {
		case GSD_REGION_JP:
			userdata->region = AOD_NET_RANK_REGION_JP;
			break;
		case GSD_REGION_US:
			if (GsEnvIsRegionAsia()) {
				userdata->region = AOD_NET_RANK_REGION_OTHER;
			}
			else {
				userdata->region = AOD_NET_RANK_REGION_US;
			}
			break;
		case GSD_REGION_EU:
			userdata->region = AOD_NET_RANK_REGION_EU;
			break;
		default:
			userdata->region = AOD_NET_RANK_REGION_OTHER;
			break;
		}
		if (send_info.data.ss) {
			userdata->is_ss = TRUE;
		}
		else {
			userdata->is_ss = FALSE;
		}

		// トランジションコンテキスト作成
		result = sceNpScoreCreateTransactionCtx(m_title_ctx);
		if (result < 0) {
			sce_result = result;
			goto end;
		}
		trans_ctx_id = result;

		// 送信
		result = sceNpScoreRecordScoreAsync(
			trans_ctx_id,
			send_info.board_no,
			send_info.data.time,
			&np_comment,
			&np_gameinfo,
			&np_tmp_rank,
			thprio,
			NULL);
		if (result < 0) {
			sce_result = result;
			goto end;
		}
		while (1) {
			int32_t async_result;
			result = sceNpScorePollAsync(trans_ctx_id, &async_result);
			if (result < 0) {
				sce_result = result;
				sceNpScoreAbortTransaction(trans_ctx_id);
				goto end;
			}
			if (result == 0) {
				// 終了
				result = async_result;
				break;
			}
			amMutexLock(&m_cancel_mutex);
			is_cancel = m_send_cancel;
			amMutexUnlock(&m_cancel_mutex);
			if (is_cancel || IsRequestEndThread()) {
				result = 0;
				sceNpScoreAbortTransaction(trans_ctx_id);
				goto end;
			}

			// 10ミリ秒程度スリープ
			sys_timer_usleep(10 * 1000);
		}
		if ((u32)result == SCE_NP_COMMUNITY_SERVER_ERROR_NOT_BEST_SCORE) {
			// 記録更新なし(正常)
		}
		else if (result < 0) {
			sce_result = result;
			goto end;
		}

		// トランジションコンテキスト解放
		if (trans_ctx_id >= 0) {
			result = sceNpScoreDestroyTransactionCtx(trans_ctx_id);
			trans_ctx_id = -1;
			if (result < 0) {
				sce_result = result;
				goto end;
			}
		}
	}

end:

	if (trans_ctx_id >= 0) {
		sceNpScoreDestroyTransactionCtx(trans_ctx_id);
		trans_ctx_id = -1;
	}

	return sce_result;
}

// ===========================================================================
//! 接続スレッド - 受信処理
// ===========================================================================
int CRank::ThreadProcSubRecv()
{
	int sce_result = 0;
	int trans_ctx_id = -1;
	SceNpScoreRankData* recv_data_sce = NULL;
	SceNpScoreGameInfo* recv_udata_sce = NULL;
	CellRtcTick recv_lastSortDate;
	SceNpScoreRankNumber recv_totalRecord;
	int result;
	BOOL is_cancel;

	int thprio;
	sys_ppu_thread_t thid;
	sys_ppu_thread_get_id(&thid);
	sys_ppu_thread_get_priority(thid, &thprio);

	m_recv_data_num = 0;

	amMutexLock(&m_cancel_mutex);
	is_cancel = m_recv_cancel;
	amMutexUnlock(&m_cancel_mutex);
	if (is_cancel) {
		goto end;
	}

	// 受信バッファ確保
	recv_data_sce = (SceNpScoreRankData*)amMemAlloc(
		sizeof(SceNpScoreRankData) * CRank::RECV_DATA_MAX);
	recv_udata_sce = (SceNpScoreGameInfo*)amMemAlloc(
		sizeof(SceNpScoreGameInfo) * CRank::RECV_DATA_MAX);

	// トランジションコンテキスト作成
	result = sceNpScoreCreateTransactionCtx(m_title_ctx);
	if (result < 0) {
		sce_result = result;
		goto end;
	}
	trans_ctx_id = result;

	// 受信
	result = sceNpScoreGetRankingByRangeAsync(
		trans_ctx_id,
		m_recv_board_no,
		m_recv_rank + 1,
		recv_data_sce,
		sizeof(SceNpScoreRankData) * CRank::RECV_DATA_MAX,
		NULL,
		0,
		recv_udata_sce,
		sizeof(SceNpScoreGameInfo) * CRank::RECV_DATA_MAX,
		CRank::RECV_DATA_MAX,
		&recv_lastSortDate,
		&recv_totalRecord,
		thprio,
		NULL);
	if (result < 0) {
		sce_result = result;
		goto end;
	}
	while (1) {
		int32_t async_result;
		result = sceNpScorePollAsync(trans_ctx_id, &async_result);
		if (result < 0) {
			sce_result = result;
			sceNpScoreAbortTransaction(trans_ctx_id);
			goto end;
		}
		if (result == 0) {
			// 終了
			result = async_result;
			break;
		}
		amMutexLock(&m_cancel_mutex);
		is_cancel = m_recv_cancel;
		amMutexUnlock(&m_cancel_mutex);
		if (is_cancel || IsRequestEndThread()) {
			result = 0;
			sceNpScoreAbortTransaction(trans_ctx_id);
			goto end;
		}

		// 10ミリ秒程度スリープ
		sys_timer_usleep(10 * 1000);
	}
	if ((u32)result == SCE_NP_COMMUNITY_SERVER_ERROR_GAME_RANKING_NOT_FOUND) {
		// ランキングデータなし(正常)
		m_recv_data_num = 0;
		goto end;
	}
	else if (result < 0) {
		sce_result = result;
		goto end;
	}

	// 受信データ数取得
	m_recv_data_num = (u32)result;
	if (m_recv_data_num > (u32)RECV_DATA_MAX) {
		m_recv_data_num = (u32)RECV_DATA_MAX; // 念のため
	}

	// トランジションコンテキスト解放
	if (trans_ctx_id >= 0) {
		result = sceNpScoreDestroyTransactionCtx(trans_ctx_id);
		trans_ctx_id = -1;
		if (result < 0) {
			sce_result = result;
			goto end;
		}
	}

	// 受信データ取得
	for (u32 i = 0; i < m_recv_data_num; ++i) {
		const SceNpScoreRankData& recv = recv_data_sce[i];
		const SceNpScoreGameInfo& recvu = recv_udata_sce[i];
		AOS_NET_RANK_DATA& dest = m_recv_data[i];
		UtilMakeRecvData(dest, recv, recvu, m_recv_type);
	}

end:

	if (trans_ctx_id >= 0) {
		sceNpScoreDestroyTransactionCtx(trans_ctx_id);
		trans_ctx_id = -1;
	}

	if (recv_udata_sce) {
		amMemFree(recv_udata_sce);
		recv_udata_sce = NULL;
	}
	if (recv_data_sce) {
		amMemFree(recv_data_sce);
		recv_data_sce = NULL;
	}

	return sce_result;
}

// ===========================================================================
//! 接続スレッド - フレンド順位受信処理
// ===========================================================================
int CRank::ThreadProcSubRecvFriends()
{
	int sce_result = 0;
	int trans_ctx_id = -1;
	u32 friend_num;
	SceNpId* recv_id_sce = NULL;
	SceNpScorePlayerRankData* recv_data_sce = NULL;
	SceNpScoreGameInfo* recv_udata_sce = NULL;
	CellRtcTick recv_lastSortDate;
	SceNpScoreRankNumber recv_totalRecord;
	int result;
	BOOL is_cancel;

	int thprio;
	sys_ppu_thread_t thid;
	sys_ppu_thread_get_id(&thid);
	sys_ppu_thread_get_priority(thid, &thprio);

	m_recv_data_num = 0;

	amMutexLock(&m_cancel_mutex);
	is_cancel = m_recv_cancel;
	amMutexUnlock(&m_cancel_mutex);
	if (is_cancel) {
		goto end;
	}

	// フレンド数取得
	result = sceNpBasicGetFriendListEntryCount(&friend_num);
	if (result < 0) {
		sce_result = result;
		goto end;
	}
	if (friend_num > (CRank::RECVF_DATA_MAX - 1)) {
		friend_num = (u32)(CRank::RECVF_DATA_MAX - 1);
	}

	// フレンド情報格納要メモリ確保
	recv_id_sce = (SceNpId*)amMemAlloc(
		sizeof(SceNpId) * CRank::RECVF_DATA_MAX);
	recv_data_sce = (SceNpScorePlayerRankData*)amMemAlloc(
		sizeof(SceNpScorePlayerRankData) * CRank::RECVF_DATA_MAX);
	recv_udata_sce = (SceNpScoreGameInfo*)amMemAlloc(
		sizeof(SceNpScoreGameInfo) * CRank::RECVF_DATA_MAX);

	// フレンド情報取得
	for (u32 i = 0; i < friend_num; ++i) {
		result = sceNpBasicGetFriendListEntry(i, &recv_id_sce[i]);
		if (result < 0) {
			sce_result = result;
			goto end;
		}
	}
	// 自身の情報設定
	recv_id_sce[friend_num] = m_np_id;

	// トランジションコンテキスト作成
	result = sceNpScoreCreateTransactionCtx(m_title_ctx);
	if (result < 0) {
		sce_result = result;
		goto end;
	}
	trans_ctx_id = result;

	// 受信
	result = sceNpScoreGetRankingByNpIdAsync(
		trans_ctx_id,
		m_recv_board_no,
		recv_id_sce,
		sizeof(SceNpId) * (friend_num + 1),
		recv_data_sce,
		sizeof(SceNpScorePlayerRankData) * (friend_num + 1),
		NULL,
		0,
		recv_udata_sce,
		sizeof(SceNpScoreGameInfo) * (friend_num + 1),
		friend_num + 1,
		&recv_lastSortDate,
		&recv_totalRecord,
		thprio,
		NULL);
	if (result < 0) {
		sce_result = result;
		goto end;
	}
	while (1) {
		int32_t async_result;
		result = sceNpScorePollAsync(trans_ctx_id, &async_result);
		if (result < 0) {
			sce_result = result;
			sceNpScoreAbortTransaction(trans_ctx_id);
			goto end;
		}
		if (result == 0) {
			// 終了
			result = async_result;
			break;
		}
		amMutexLock(&m_cancel_mutex);
		is_cancel = m_recv_cancel;
		amMutexUnlock(&m_cancel_mutex);
		if (is_cancel || IsRequestEndThread()) {
			result = 0;
			sceNpScoreAbortTransaction(trans_ctx_id);
			goto end;
		}

		// 10ミリ秒程度スリープ
		sys_timer_usleep(10 * 1000);
	}
	if ((u32)result == SCE_NP_COMMUNITY_SERVER_ERROR_GAME_RANKING_NOT_FOUND) {
		// ランキングデータなし(正常)
		m_recv_data_num = 0;
		goto end;
	}
	else if (result < 0) {
		sce_result = result;
		goto end;
	}

	// トランジションコンテキスト解放
	if (trans_ctx_id >= 0) {
		result = sceNpScoreDestroyTransactionCtx(trans_ctx_id);
		trans_ctx_id = -1;
		if (result < 0) {
			sce_result = result;
			goto end;
		}
	}

	// ランキングデータ取得
	m_recv_data_num = 0;
	for (u32 i = 0; i < (friend_num + 1); ++i) {
		const SceNpScorePlayerRankData& recv_base = recv_data_sce[i];
		const SceNpScoreGameInfo& recvu = recv_udata_sce[i];
		if (recv_base.hasData == 0) {
			// データなし
			continue;
		}
		const SceNpScoreRankData& recv = recv_base.rankData;
		u32 rrank = (u32)recv.serialRank;
		if (rrank > 0) {
			rrank -= 1;
		}

		// 挿入位置を検索
		s32 insert_pos;
		for (insert_pos = 0; insert_pos < (s32)m_recv_data_num; ++insert_pos) {
			if (rrank < m_recv_data[insert_pos].rrank) {
				break;
			}
		}
		if (insert_pos < (s32)m_recv_data_num) {
			// 挿入位置以降をずらす
			for (s32 j = (s32)(m_recv_data_num - 1); j >= insert_pos; --j) {
				m_recv_data[j + 1] = m_recv_data[j];
			}
		}
		// 挿入
		UtilMakeRecvData(m_recv_data[insert_pos], recv, recvu, m_recv_type);

		m_recv_data_num += 1;
	}

	// 実順位をフレンド内のものに変換
	for (u32 i = 0; i < m_recv_data_num; ++i) {
		m_recv_data[i].rrank = i;
	}

end:

	if (trans_ctx_id >= 0) {
		sceNpScoreDestroyTransactionCtx(trans_ctx_id);
		trans_ctx_id = -1;
	}
	if (recv_id_sce) {
		amMemFree(recv_id_sce);
		recv_id_sce = NULL;
	}
	if (recv_data_sce) {
		amMemFree(recv_data_sce);
		recv_data_sce = NULL;
	}
	if (recv_udata_sce) {
		amMemFree(recv_udata_sce);
		recv_udata_sce = NULL;
	}

	return sce_result;
}

// ===========================================================================
//! 接続スレッド - 自身の順位受信処理
// ===========================================================================
int CRank::ThreadProcSubRecvOwnRank()
{
	int sce_result = 0;
	int trans_ctx_id = -1;
	SceNpScorePlayerRankData* recv_data_sce = NULL;
	CellRtcTick recv_lastSortDate;
	SceNpScoreRankNumber recv_totalRecord;
	int result;
	BOOL is_cancel;

	int thprio;
	sys_ppu_thread_t thid;
	sys_ppu_thread_get_id(&thid);
	sys_ppu_thread_get_priority(thid, &thprio);

	m_recv_own_rank = -1;
	m_recv_all_rank_num = 0;
	m_recv_data_num = 0;

	amMutexLock(&m_cancel_mutex);
	is_cancel = m_recv_cancel;
	amMutexUnlock(&m_cancel_mutex);
	if (is_cancel) {
		goto end;
	}

	// 受信バッファ確保
	recv_data_sce = (SceNpScorePlayerRankData*)amMemAlloc(
		sizeof(SceNpScorePlayerRankData));

	// トランジションコンテキスト作成
	result = sceNpScoreCreateTransactionCtx(m_title_ctx);
	if (result < 0) {
		sce_result = result;
		goto end;
	}
	trans_ctx_id = result;

	// 受信
	result = sceNpScoreGetRankingByNpIdAsync(
		trans_ctx_id,
		m_recv_board_no,
		&m_np_id,
		sizeof(SceNpId),
		recv_data_sce,
		sizeof(SceNpScorePlayerRankData),
		NULL,
		0,
		NULL,
		0,
		1,
		&recv_lastSortDate,
		&recv_totalRecord,
		thprio,
		NULL);
	if (result < 0) {
		sce_result = result;
		goto end;
	}
	while (1) {
		int32_t async_result;
		result = sceNpScorePollAsync(trans_ctx_id, &async_result);
		if (result < 0) {
			sce_result = result;
			sceNpScoreAbortTransaction(trans_ctx_id);
			goto end;
		}
		if (result == 0) {
			// 終了
			result = async_result;
			break;
		}
		amMutexLock(&m_cancel_mutex);
		is_cancel = m_recv_cancel;
		amMutexUnlock(&m_cancel_mutex);
		if (is_cancel || IsRequestEndThread()) {
			result = 0;
			sceNpScoreAbortTransaction(trans_ctx_id);
			goto end;
		}

		// 10ミリ秒程度スリープ
		sys_timer_usleep(10 * 1000);
	}
	if ((u32)result == SCE_NP_COMMUNITY_SERVER_ERROR_GAME_RANKING_NOT_FOUND) {
		// ランキングデータなし(正常)
		m_recv_data_num = 0;
		goto end;
	}
	else if (result < 0) {
		sce_result = result;
		goto end;
	}

	// トランジションコンテキスト解放
	if (trans_ctx_id >= 0) {
		result = sceNpScoreDestroyTransactionCtx(trans_ctx_id);
		trans_ctx_id = -1;
		if (result < 0) {
			sce_result = result;
			goto end;
		}
	}

	// 受信データ取得
	if (recv_data_sce->hasData && (recv_data_sce->rankData.serialRank >= 1)) {
		m_recv_own_rank = (s32)(recv_data_sce->rankData.serialRank - 1);
	}
	m_recv_all_rank_num = (u32)recv_totalRecord;

	// 受信データ破棄
	if (recv_data_sce) {
		amMemFree(recv_data_sce);
		recv_data_sce = NULL;
	}

end:

	if (trans_ctx_id >= 0) {
		sceNpScoreDestroyTransactionCtx(trans_ctx_id);
		trans_ctx_id = -1;
	}

	if (recv_data_sce) {
		amMemFree(recv_data_sce);
		recv_data_sce = NULL;
	}

	return sce_result;
}


// ***************************************************************************
// ユーティリティ
// ***************************************************************************
// ===========================================================================
//! 受信データから情報取得
// ===========================================================================
void CRank::UtilMakeRecvData(
	AOS_NET_RANK_DATA& dest,
	const SceNpScoreRankData& recv,
	const SceNpScoreGameInfo& recvu,
	AOE_NET_RANK_TYPE type)
{
	const AOS_NET_RANK_USER_DATA* userdata =
		(const AOS_NET_RANK_USER_DATA*)(&recvu.nativeData[0]);

	// フラグ初期化
	dest.flag = 0;

	// 名前取得
	amCopyMemory(dest.name, recv.npId.handle.data, AOD_NET_RANK_NAME_LEN - 1);
	dest.name[AOD_NET_RANK_NAME_LEN - 1] = '\0';

	// 実順位
	if (recv.serialRank == 0) {
		amAssert(0); // ありえない？
		dest.rrank = recv.serialRank;
	}
	else {
		dest.rrank = (u32)(recv.serialRank - 1);
	}

	// 表示順位
	if (recv.rank == 0) {
		amAssert(0); // ありえない？
		dest.rank = recv.rank;
	}
	else {
		dest.rank = (u32)(recv.rank - 1);
	}

	// スコア取得
	dest.score = (u32)recv.scoreValue;

	// 自身判定
	if (sceNpUtilCmpNpId(&m_np_id, &recv.npId) == 0) {
		dest.flag |= AOD_NET_RANK_FLAG_OWN;
	}

	// スーパーソニック使用判定
	if (userdata->is_ss) {
		dest.flag |= AOD_NET_RANK_FLAG_SS;
	}

	// リージョン
	dest.region = userdata->region;
	if (dest.region >= AOD_NET_RANK_REGION_NUM) {
		dest.region = AOD_NET_RANK_REGION_OTHER;
	}
}


// ***************************************************************************
// エラー
// ***************************************************************************
// ===========================================================================
//! エラー設定
// ===========================================================================
void CRank::SetError(u32 error, int sce_result)
{
	// 既にエラー設定済みなら上書きしない
	if (!IsError()) {
		m_error = error;
	}
	if (m_sce_error_code == 0) {
		m_sce_error_code = (u32)sce_result;
	}
	g_ao_net_rank_error = TRUE;
}

} // namespace net
} // namespace ao

// ===========================================================================
//! メッセージユーティリティに専用メッセージがあるか判定
// ===========================================================================
BOOL aoNetRankIsUtilMessage(u32 error)
{
	switch (error) {
	case SCE_NP_COMMUNITY_SERVER_ERROR_BLACKLISTED_USER_ID:
	case SCE_NP_COMMUNITY_SERVER_ERROR_NOT_RECORDABLE_VERSION:
	case SCE_NP_COMMUNITY_SERVER_ERROR_RANKING_BEFORE_SERVICE:
	case SCE_NP_COMMUNITY_SERVER_ERROR_RANKING_END_OF_SERVICE:
	case SCE_NP_COMMUNITY_SERVER_ERROR_RANKING_MAINTENANCE:
		return TRUE;
		break;

	default:
		break;
	}
	return FALSE;
}

// ===========================================================================
//! オフラインエラーか判定
// ===========================================================================
BOOL aoNetRankIsOfflineMessage(u32 error)
{
	switch (error) {
	case SCE_NP_ERROR_INVALID_STATE:
	case SCE_NP_ERROR_OFFLINE:
	case SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID:
		return TRUE;
		break;

	default:
		break;
	}
	return FALSE;
}

// ===========================================================================
//! ボード番号変換
// ===========================================================================
u32 aoNetRankConvBoardNo(u32 board)
{
	amAssert(board < 144);

	// スペシャルステージ
	if (board >= 102) {
		u32 ss = (board - 102) / 6;
		u32 mode;
		if (((board - 102) % 6) < 3) {
			mode = 0;
		}
		else {
			mode = 1;
		}
		board = 102 + ((ss * 2) + mode);
	}

	amAssert(board < 116);
	return board;
}

// ===========================================================================
//! システムコールバック
// ===========================================================================
void aoNetRankSysCallbackNotice(uint64_t status, uint64_t param, void* arg)
{
	if (status == CELL_SYSUTIL_NET_CTL_NETSTART_FINISHED) {
		((ao::net::CRank*)arg)->NoticeNetStartDialogEnd();
	}
}

#endif // defined(AOD_PLATFORM_PS3)

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
