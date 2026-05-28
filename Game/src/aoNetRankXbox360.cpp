// ===========================================================================
/*!
	@file	aoNetRankXbox360.cpp
	@brief	AoLibrary ネットワークランキングモジュール定義(Xbox360)

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"
#include "aoNetRank.h"
#include "gs.h"

#if defined(AOD_PLATFORM_XBOX360)

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
		u32 board, AOE_NET_RANK_TYPE type, const AOS_NET_RANK_SEND* data,
		BOOL is_asc);

	//! 送信開始
	void SendStart();

	//! 送信データクリア
	void SendClear();

	//! 送信キャンセル可能判定
	BOOL IsCancelable();

	//! 送信キャンセル通知
	void SendCancel();

	//! 送信終了判定
	BOOL SendIsFinished() const;

	//! 送信成功判定
	BOOL SendIsSuccessed() const;


	//! 受信開始
	void RecvStart(u32 board, AOE_NET_RANK_TYPE type, u32 rank, BOOL is_asc);

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
	BOOL ThreadProcSubSendCheckProfile();
	int ThreadProcSubRecv();
	int ThreadProcSubRecvFriends();
	int ThreadProcSubRecvNear();

	// ユーティリティ
	BOOL UtilMakeRecvData(
		AOS_NET_RANK_DATA& dest,
		const XUSER_STATS_ROW& row,
		AOE_NET_RANK_TYPE type,
		const AOS_NET_RANK_BOARD_INFO& binfo,
		BOOL is_asc);

	// エラー
	void SetError(u32 error);

	// 定数
	enum {
		// 状態
		STATE_CONNECTING	= 0,	//!< 接続中
		STATE_CONNECTED,			//!< 接続済み
		STATE_SENDING,				//!< 送信中
		STATE_RECVING,				//!< 受信中

		// エラー
		ERROR_NONE		= 0,	//!< エラー無し
		ERROR_ACCOUNT,			//!< 有効なアカウントではない
		ERROR_OFFLINE,			//!< サインインできていない

		ERROR_OPERATION,		//!< 不正な関数呼び出し
		ERROR_OTHER,			//!< その他のエラー
		ERROR_NUM,				//!< エラー数

		SEND_DATA_MAX	= 16,	//!< 送信データ数
		RECV_DATA_MAX	= 10,	//!< 受信データ最大数
		RECVF_DATA_MAX	= 101,	//!< フレンド受信データ最大数
	};

	//! 送信情報
	struct SEND_INFO {
		u32					board_no;		//!< ボード番号
		AOE_NET_RANK_TYPE	type;			//!< 送信タイプ
		AOS_NET_RANK_SEND	data;			//!< 送信データ
		BOOL				is_asc;			//!< 昇順フラグ
	};

	u32 m_state; //!< 状態
	BOOL m_end_flag; //!< 終了フラグ
	AMS_MUTEX m_mutex; //!< ミューテックス
	u32 m_error; //!< エラー

	// 送信関連
	BOOL m_send_execute; //!< 送信実行フラグ
	SEND_INFO m_send_tbl[SEND_DATA_MAX]; //!< 送信情報配列
	u32 m_send_num; //!< 送信情報数
	BOOL m_send_success; //!< 送信成功フラグ

	// 受信関連
	BOOL m_recv_execute; //!< 受信実行フラグ
	AOS_NET_RANK_DATA m_recv_data[RECVF_DATA_MAX]; //!< 受信データ
	AOE_NET_RANK_TYPE m_recv_type; //!< 受信ランキングタイプ
	BOOL m_recv_success; //!< 受信成功フラグ
	u32 m_recv_board_no; //!< 受信ランキングボード番号
	u32 m_recv_rank; //!< 受信ランク
	BOOL m_recv_asc; //!< 昇順フラグ
	u32 m_recv_data_num; //!< 受信データ数

	u32 m_own_id;		//!< 自身のアカウントID
	XUID m_own_xuid;	//!< 自身のXUID

	BOOL m_session_cancelable; //!< セッションキャンセル可能フラグ
	BOOL m_session_cancel; //!< セッションキャンセルフラグ
};

} // namespace net
} // namespace ao

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

static s32 aoNetRankConvTimeToMsec(u32 time);

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

// ===========================================================================
//! 前回送信時間
// ===========================================================================
static u32 g_ao_net_rank_send_time = 0;

#include "aoNetRankXbox360Info.inc"

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
	if (g_ao_net_rank == NULL) {
		amAssert(0);
		return;
	}
	g_ao_net_rank->SendSetData(board, type, data, is_asc);
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

#if defined(AOD_PLATFORM_XBOX360)
// ===========================================================================
//! 送信キャンセル可能判定
// ===========================================================================
BOOL AoNetRankSendIsCancelable(void)
{
	if (g_ao_net_rank == NULL) {
		return FALSE;
	}
	return g_ao_net_rank->IsCancelable();
}
#endif // defined(AOD_PLATFORM_XBOX360)

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
	if (g_ao_net_rank == NULL) {
		amAssert(0);
		return;
	}
	g_ao_net_rank->RecvStart(board, type, rank, is_asc);
}

// ===========================================================================
//	AoNetRankSendCancel
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

// ===========================================================================
//	AoNetRankUtilRecvDataGetXUID
/*!
	受信データからXUID取得

	@param data		[in] 受信データ
	@return XUID
*/
// ===========================================================================
XUID AoNetRankUtilRecvDataGetXUID(const AOS_NET_RANK_DATA* data)
{
	amAssert(data);
	return data->xuid;
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace ao {
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
	m_error = CRank::ERROR_NONE;

	m_send_execute = FALSE;
	amZeroMemory(m_send_tbl, sizeof(CRank::SEND_INFO) * CRank::SEND_DATA_MAX);
	m_send_num = 0;
	m_send_success = FALSE;

	m_recv_execute = FALSE;
	amZeroMemory(m_recv_data, sizeof(AOS_NET_RANK_DATA) * RECVF_DATA_MAX);
	m_recv_type = AOD_NET_RANK_TYPE_NONE;
	m_recv_success = FALSE;
	m_recv_board_no = 0;

	m_session_cancelable = FALSE;
	m_session_cancel = FALSE;

	// ミューテックス作成
	amMutexCreate(&m_mutex);

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
	u32 board, AOE_NET_RANK_TYPE type, const AOS_NET_RANK_SEND* data,
	BOOL is_asc)
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
	m_send_tbl[m_send_num].is_asc = is_asc;
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
//! 送信キャンセル可能判定
// ===========================================================================
BOOL CRank::IsCancelable()
{
	if (!SendIsFinished()) {
		BOOL ret;
		amMutexLock(&m_mutex);
		ret = m_session_cancelable;
		amMutexUnlock(&m_mutex);
		return ret;
	}
	return FALSE;
}

// ===========================================================================
//! 送信キャンセル通知
// ===========================================================================
void CRank::SendCancel()
{
	amMutexLock(&m_mutex);
	if (m_session_cancelable) {
		m_session_cancel = TRUE;
	}
	amMutexUnlock(&m_mutex);
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
void CRank::RecvStart(u32 board, AOE_NET_RANK_TYPE type, u32 rank, BOOL is_asc)
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
	m_recv_asc = is_asc;

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
	// キャンセルは無し
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
	DWORD result;

	// ユーザのサインイン情報取得
	m_own_id = (u32)AoAccountGetCurrentId();
	if (m_own_id >= 4) {
		SetError(CRank::ERROR_ACCOUNT);
		SetOwnProc(&CRank::ProcError);
		return;
	}
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		SetError(CRank::ERROR_OFFLINE);
		SetOwnProc(&CRank::ProcError);
		return;
	}
	if (XUserGetSigninState(m_own_id) != eXUserSigninState_SignedInToLive) {
		SetError(CRank::ERROR_OFFLINE);
		SetOwnProc(&CRank::ProcError);
		return;
	}
	XUSER_SIGNIN_INFO info;
	result = XUserGetSigninInfo(
		m_own_id, XUSER_GET_SIGNIN_INFO_ONLINE_XUID_ONLY, &info);
	if (result != ERROR_SUCCESS) {
		SetError(CRank::ERROR_ACCOUNT);
		SetOwnProc(&CRank::ProcError);
		return;
	}
	if (info.dwInfoFlags != XUSER_INFO_FLAG_LIVE_ENABLED) {
		SetError(CRank::ERROR_ACCOUNT);
		SetOwnProc(&CRank::ProcError);
		return;
	}
	if (info.UserSigninState != eXUserSigninState_SignedInToLive) {
		SetError(CRank::ERROR_OFFLINE);
		SetOwnProc(&CRank::ProcError);
		return;
	}
	m_own_xuid = info.xuid;

	// スレッド作成
	SetThreadProc(0, &CRank::ThreadProcConnect);
	StartThread(0, (AMD_CORE)0, (u32)THREAD_PRIORITY_BELOW_NORMAL);

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
		m_session_cancelable = FALSE;
		m_session_cancel = FALSE;
		amMutexUnlock(&m_mutex);
	}

	amMutexLock(&m_mutex);
	// 送信完了判定
	if (!m_send_execute) {
		// 送信データクリア
		SendClear();

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
	ProcRecving();
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
//! エラーメッセージ表示開始待ち
// ===========================================================================
void CRank::ProcErrorMessageWait()
{
	// サインイン状態が無効になったのならメッセージ表示なしで終了
	if (!AoAccountIsCurrentEnable()) {
		// 終了
		SetOwnProcNone();
		return;
	}

	// 既存メッセージ表示終了待ち
	if (AoSysMsgIsFinished()) {

		// エラー表示開始
		switch (m_error) {
		case CRank::ERROR_ACCOUNT:
			AoSysMsgStart(
				AOD_SYS_MSG_NET_ERROR_OFFLINE, AOD_SYS_MSG_SELECT_OK);
			break;
		case CRank::ERROR_OFFLINE:
			AoSysMsgStart(
				AOD_SYS_MSG_NET_ERROR_OFFLINE, AOD_SYS_MSG_SELECT_OK);
			break;
		default:
			AoSysMsgStart(AOD_SYS_MSG_NET_ERROR_COMMON, AOD_SYS_MSG_SELECT_OK);
			break;
		}

		// メッセージ表示中へ遷移
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
		// 終了
		SetOwnProcNone();
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
	// 接続済み設定
	amMutexLock(&m_mutex);
	m_send_execute = FALSE;
	m_recv_execute = FALSE;
	m_state = CRank::STATE_CONNECTED;
	amMutexUnlock(&m_mutex);

	// 終了要求があるまでループ
	while (!IsRequestEndThread()) {

		int result;
		BOOL is_execute;

		// 送信処理判定
		amMutexLock(&m_mutex);
		is_execute = m_send_execute;
		amMutexUnlock(&m_mutex);
		if (is_execute) {

			// 送信数が0になるまで送信する
			while (m_send_num > 0) {
				// 送信処理
				result = ThreadProcSubSend();
				if (result != 0) {
					// エラー
					amMutexLock(&m_mutex);
					SetError(CRank::ERROR_OTHER);
					amMutexUnlock(&m_mutex);
					// 切断開始処理はメインスレッドから行なうのでエラー設定のみ
					break;
				}
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
			else if (m_recv_rank == AOD_NET_RANK_OWN_NEAR) {
				result = ThreadProcSubRecvNear();
			}
			else {
				result = ThreadProcSubRecv();
			}
			if (result != 0) {
				// エラー
				amMutexLock(&m_mutex);
				SetError(CRank::ERROR_OTHER);
				amMutexUnlock(&m_mutex);
				// 切断開始処理はメインスレッドから行なうのでエラー設定のみ
			}

			// 終了
			amMutexLock(&m_mutex);
			m_recv_execute = FALSE;
			amMutexUnlock(&m_mutex);
		}

		// 100ミリ秒程度スリープ
		Sleep(100);
	}
}

// ===========================================================================
//! 接続スレッド - 送信処理
// ===========================================================================
int CRank::ThreadProcSubSend()
{
	HANDLE hSession = INVALID_HANDLE_VALUE;
	ULONGLONG nonce;
	XSESSION_INFO session_info;
	BOOL is_private;
	s32 write_count;
	u32 board_num;
	XSESSION_VIEW_PROPERTIES vprop[1];
	XUSER_PROPERTY prop[4];
	BOOL is_start = FALSE;
	BOOL is_join = FALSE;
	DWORD result;
	int ret = 0;

	// 送信データが無いなら終了
	if (m_send_num == 0) {
		return 0;
	}

	while (1) {
		BOOL is_cancel;
		DWORD tc = GetTickCount();
		DWORD time = (DWORD)(tc - g_ao_net_rank_send_time);
		if ((u32)time >= (u32)(20 * 1000)) {
			g_ao_net_rank_send_time = tc;
			break;
		}
		if (!ThreadProcSubSendCheckProfile()) {
			ret = 1;
			goto end;
		}

		amMutexLock(&m_mutex);
		m_session_cancelable = TRUE;
		is_cancel = m_session_cancel;
		amMutexUnlock(&m_mutex);
		if (is_cancel) {
			m_send_num = 0;
			goto end;
		}

		// 100ミリ秒程度スリープ
		Sleep(100);
	}
	amMutexLock(&m_mutex);
	m_session_cancelable = FALSE;
	amMutexUnlock(&m_mutex);
	{
		DWORD one_min_time = GetTickCount();
		while (1) {
			if ((GetTickCount() - one_min_time) >= 500) {
				break;
			}
			amMutexLock(&m_mutex);
			BOOL is_cancel = m_session_cancel;
			amMutexUnlock(&m_mutex);
			if (is_cancel) {
				m_send_num = 0;
				goto end;
			}

			Sleep(100);
		}
	}

	if (!ThreadProcSubSendCheckProfile()) {
		ret = 1;
		goto end;
	}

	// コンテキスト設定
	XUserSetContext(
		m_own_id, X_CONTEXT_GAME_TYPE, X_CONTEXT_GAME_TYPE_STANDARD);

	if (!ThreadProcSubSendCheckProfile()) {
		ret = 1;
		goto end;
	}

	// セッション作成
	result = XSessionCreate(
		XSESSION_CREATE_USES_STATS,
		m_own_id,
		8,
		8,
		&nonce,
		&session_info,
		NULL,
		&hSession);
	if ((result != ERROR_SUCCESS) || (hSession == INVALID_HANDLE_VALUE)) {
		hSession = INVALID_HANDLE_VALUE;
		ret = 1;
		goto end;
	}

	if (!ThreadProcSubSendCheckProfile()) {
		ret = 1;
		goto end;
	}

	// セッション参加
	is_private = FALSE;
	result = XSessionJoinRemote(
		hSession,
		1,
		&m_own_xuid,
		&is_private,
		NULL);
	if (result != ERROR_SUCCESS) {
		ret = 1;
		goto end;
	}
	is_join = TRUE;

	if (!ThreadProcSubSendCheckProfile()) {
		ret = 1;
		goto end;
	}

	// セッション開始
	result = XSessionStart(hSession, 0, NULL);
	if (result != ERROR_SUCCESS) {
		ret = 1;
		goto end;
	}
	is_start = TRUE;

	if (!ThreadProcSubSendCheckProfile()) {
		ret = 1;
		goto end;
	}

	// 書き込み数を算出
	// (1度のセッションで更新できるボードは5つまで)
	board_num = 0;
	write_count = 0;
	for (s32 i = (s32)(m_send_num - 1); i >= 0; --i) {
		u32 board_no = m_send_tbl[i].board_no;
		s32 j;
		for (j = (s32)(m_send_num - 1); j > i; --j) {
			if (board_no == m_send_tbl[j].board_no) {
				break;
			}
		}
		if (j <= i) {
			board_num += 1;
			if (board_num > 5) {
				break;
			}
		}
		write_count += 1;
	}

	// 途中キャンセル時のランキングボード整合性確保のため
	// 送信総数が6の場合は、送信数を3にする
	if ((m_send_num >= 6) && (write_count > 3)) {
		write_count = 3;
	}

	// 書き込み
	while (write_count > 0) {
		write_count -= 1;
		amAssert(m_send_num > 0);

		const CRank::SEND_INFO& send_info = m_send_tbl[m_send_num - 1];
		m_send_num -= 1;

		amAssert(send_info.board_no < AOD_NET_RANK_BOARD_NUM);
		const AOS_NET_RANK_BOARD_INFO& binfo =
			g_ao_net_rank_board_info[send_info.board_no];

		amZeroMemory(&vprop[0], sizeof(XSESSION_VIEW_PROPERTIES) * 1);
		amZeroMemory(&prop[0], sizeof(XUSER_PROPERTY) * 4);
		if (send_info.type == AOD_NET_RANK_TYPE_TIME) {
			prop[0].dwPropertyId = PROPERTY_TIME;
			prop[0].value.type = XUSER_DATA_TYPE_INT64;
			prop[0].value.i64Data = (s32)send_info.data.time;
		}
		else {
			prop[0].dwPropertyId = PROPERTY_SCORE;
			prop[0].value.type = XUSER_DATA_TYPE_INT64;
			prop[0].value.i64Data = (s32)send_info.data.time;
		}
		if (send_info.is_asc) {
			prop[0].value.i64Data = -prop[0].value.i64Data;
		}

		prop[1].dwPropertyId = PROPERTY_REGION;
		prop[1].value.type = XUSER_DATA_TYPE_INT32;
		switch (GsEnvGetRegion()) {
		case GSD_REGION_JP:
			prop[1].value.nData = (LONG)AOD_NET_RANK_REGION_JP;
			break;
		case GSD_REGION_US:
			if (GsEnvIsRegionAsia()) {
				prop[1].value.nData = (LONG)AOD_NET_RANK_REGION_OTHER;
			}
			else {
				prop[1].value.nData = (LONG)AOD_NET_RANK_REGION_US;
			}
			break;
		case GSD_REGION_EU:
			prop[1].value.nData = (LONG)AOD_NET_RANK_REGION_EU;
			break;
		default:
			prop[1].value.nData = (LONG)AOD_NET_RANK_REGION_OTHER;
			break;
		}
		if (send_info.type == AOD_NET_RANK_TYPE_TIME) {
			if (binfo.ss_col >= 0) {
				prop[2].dwPropertyId = PROPERTY_SONIC_TYPE;
				prop[2].value.type = XUSER_DATA_TYPE_INT32;
				if (send_info.data.ss) {
					prop[2].value.nData = 1;
				}
				else {
					prop[2].value.nData = 0;
				}
				prop[3].dwPropertyId = PROPERTY_STIME;
				prop[3].value.type = XUSER_DATA_TYPE_INT32;
				prop[3].value.nData = aoNetRankConvTimeToMsec(send_info.data.time);
				vprop[0].dwNumProperties = 4;
			}
			else {
				prop[2].dwPropertyId = PROPERTY_STIME;
				prop[2].value.type = XUSER_DATA_TYPE_INT32;
				prop[2].value.nData = aoNetRankConvTimeToMsec(send_info.data.time);
				vprop[0].dwNumProperties = 3;
			}
		}
		else {
			if (binfo.ss_col >= 0) {
				prop[2].dwPropertyId = PROPERTY_SONIC_TYPE;
				prop[2].value.type = XUSER_DATA_TYPE_INT32;
				if (send_info.data.ss) {
					prop[2].value.nData = 1;
				}
				else {
					prop[2].value.nData = 0;
				}
				vprop[0].dwNumProperties = 3;
			}
			else {
				vprop[0].dwNumProperties = 2;
			}
		}
		vprop[0].pProperties = &prop[0];
		vprop[0].dwViewId = binfo.board;

		if (!ThreadProcSubSendCheckProfile()) {
			ret = 1;
			goto end;
		}

		result = XSessionWriteStats(hSession, m_own_xuid, 1, vprop, NULL);
		if (result != ERROR_SUCCESS) {
			ret = 1;
			goto end;
		}
	}

end:

	// セッション終了
	if (is_start) {
		if (ThreadProcSubSendCheckProfile()) {
			result = XSessionEnd(hSession, NULL);
			if (result != ERROR_SUCCESS) {
				ret = 1;
			}
		}
		is_start = FALSE;
	}

	// セッション離脱
	if (is_join) {
		if (ThreadProcSubSendCheckProfile()) {
			result = XSessionLeaveRemote(hSession, 1, &m_own_xuid, NULL);
			if (result != ERROR_SUCCESS) {
				ret = 1;
			}
		}
		is_join = FALSE;
	}

	// セッション削除
	if (hSession != INVALID_HANDLE_VALUE) {
		result = XSessionDelete(hSession, NULL);
		if (!XCloseHandle(hSession)) {
			ret = 1;
		}
		if (result != ERROR_SUCCESS) {
			ret = 1;
		}
		hSession = INVALID_HANDLE_VALUE;
	}

	return ret;
}

// ===========================================================================
//! 接続スレッド - 送信処理プロフィール有効判定
// ===========================================================================
BOOL CRank::ThreadProcSubSendCheckProfile()
{
	if (XUserGetSigninState(m_own_id) == eXUserSigninState_SignedInToLive) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! 接続スレッド - 受信処理
// ===========================================================================
int CRank::ThreadProcSubRecv()
{
	HANDLE hEnum = INVALID_HANDLE_VALUE;
	XUSER_STATS_SPEC stats_spec;
	DWORD enum_buf_size;
	XUSER_STATS_READ_RESULTS* enum_buf = NULL;
	DWORD result;
	DWORD enum_result;
	u32 recv_num;
	int ret = 0;

	m_recv_data_num = 0;

	amAssert(m_recv_board_no < AOD_NET_RANK_BOARD_NUM);
	const AOS_NET_RANK_BOARD_INFO& binfo =
		g_ao_net_rank_board_info[m_recv_board_no];

	// 列挙準備
	stats_spec.dwViewId = binfo.board;
	stats_spec.rgwColumnIds[0] = binfo.region_col;
	if (binfo.ss_col >= 0) {
		stats_spec.dwNumColumnIds = 2;
		stats_spec.rgwColumnIds[1] = binfo.ss_col;
	}
	else {
		stats_spec.dwNumColumnIds = 1;
	}
	result = XUserCreateStatsEnumeratorByRank(
		0,
		m_recv_rank + 1,
		CRank::RECV_DATA_MAX,
		1,
		&stats_spec,
		&enum_buf_size,
		&hEnum);
	if ((result != ERROR_SUCCESS) || (enum_buf_size == 0)) {
		hEnum = INVALID_HANDLE_VALUE;
		ret = 1;
		goto end;
	}

	// バッファ確保
	enum_buf = (XUSER_STATS_READ_RESULTS*)amMemAlloc(enum_buf_size);

	// 列挙
	result = XEnumerate(hEnum, enum_buf, enum_buf_size, &enum_result, NULL);
	if (result != ERROR_SUCCESS) {
		ret = 1;
		goto end;
	}

	// 受信データ取得
	if (enum_buf->dwNumViews >= 1) {
		recv_num = (u32)enum_buf->pViews[0].dwNumRows;
	}
	else {
		recv_num = 0;
	}
	if (recv_num > (u32)RECV_DATA_MAX) {
		recv_num = (u32)RECV_DATA_MAX; // 念のため
	}
	m_recv_data_num = 0;
	for (u32 i = 0; i < recv_num; ++i) {
		const XUSER_STATS_ROW& row = enum_buf->pViews[0].pRows[i];
		AOS_NET_RANK_DATA& dest = m_recv_data[m_recv_data_num];
		if (UtilMakeRecvData(dest, row, m_recv_type, binfo, m_recv_asc)) {
			m_recv_data_num += 1;
		}
	}

end:

	// 列挙ハンドル解放
	if (hEnum != INVALID_HANDLE_VALUE) {
		CloseHandle(hEnum);
		hEnum = INVALID_HANDLE_VALUE;
	}

	// バッファ解放
	if (enum_buf) {
		amMemFree(enum_buf);
		enum_buf = NULL;
	}

	return ret;
}

// ===========================================================================
//! 接続スレッド - フレンド順位受信処理
// ===========================================================================
int CRank::ThreadProcSubRecvFriends()
{
	HANDLE hEnum = INVALID_HANDLE_VALUE;
	DWORD friend_buf_size;
	XONLINE_FRIEND* friend_buf = NULL;
	XUID* xuid_buf = NULL;
	DWORD result;
	DWORD friend_num;
	DWORD rfriend_num;
	DWORD rfriend_no;
	XUSER_STATS_SPEC stats_spec;
	DWORD recv_buf_size;
	XUSER_STATS_READ_RESULTS* recv_buf = NULL;
	u32 recv_num;
	int ret = 0;

	m_recv_data_num = 0;

	amAssert(m_recv_board_no < AOD_NET_RANK_BOARD_NUM);
	const AOS_NET_RANK_BOARD_INFO& binfo =
		g_ao_net_rank_board_info[m_recv_board_no];

	// フレンド列挙準備
	result = XFriendsCreateEnumerator(
		m_own_id,
		0,
		100,
		&friend_buf_size,
		&hEnum);
	if ((result != ERROR_SUCCESS) || (friend_buf_size == 0)) {
		hEnum = INVALID_HANDLE_VALUE;
		ret = 1;
		goto end;
	}

	// フレンドバッファ確保
	friend_buf = (XONLINE_FRIEND*)amMemAlloc(friend_buf_size);

	// フレンド列挙
	result = XEnumerate(
		hEnum, friend_buf, friend_buf_size, &friend_num, NULL);
	if (result != ERROR_SUCCESS) {
		friend_num = 0;
	}
	if (friend_num > 100) {
		friend_num = 100;
	}

	// 有効なフレンド数算出
	rfriend_num = 0;
	for (u32 i = 0; i < friend_num; ++i) {
		if (!(friend_buf[i].dwFriendState &
			 (XONLINE_FRIENDSTATE_FLAG_SENTREQUEST |
			  XONLINE_FRIENDSTATE_FLAG_RECEIVEDREQUEST)))
		{
			rfriend_num += 1;
		}
	}

	// XUIDのバッファ確保
	xuid_buf = (XUID*)amMemAlloc(sizeof(XUID) * (rfriend_num + 1));

	// 有効なフレンドのみの情報取得
	rfriend_no = 0;
	for (u32 i = 0; i < friend_num; ++i) {
		if (!(friend_buf[i].dwFriendState &
			 (XONLINE_FRIENDSTATE_FLAG_SENTREQUEST |
			  XONLINE_FRIENDSTATE_FLAG_RECEIVEDREQUEST)))
		{
			amAssert(rfriend_no < rfriend_num);
			xuid_buf[rfriend_no] = friend_buf[i].xuid;
			rfriend_no += 1;
		}
	}
	amAssert(rfriend_no == rfriend_num);
	xuid_buf[rfriend_num] = m_own_xuid;

	// フレンドランキング取得準備
	stats_spec.dwViewId = binfo.board;
	stats_spec.rgwColumnIds[0] = binfo.region_col;
	if (binfo.ss_col >= 0) {
		stats_spec.dwNumColumnIds = 2;
		stats_spec.rgwColumnIds[1] = binfo.ss_col;
	}
	else {
		stats_spec.dwNumColumnIds = 1;
	}
	recv_buf_size = 0;
	result = XUserReadStats(
		0,
		rfriend_num + 1,
		xuid_buf,
		1,
		&stats_spec,
		&recv_buf_size,
		NULL,
		NULL);
	if ((result != ERROR_INSUFFICIENT_BUFFER) || (recv_buf_size == 0)) {
		ret = 1;
		goto end;
	}

	// 受信バッファ確保
	recv_buf = (XUSER_STATS_READ_RESULTS*)amMemAlloc(recv_buf_size);

	// フレンドランキング取得
	result = XUserReadStats(
		0,
		rfriend_num + 1,
		xuid_buf,
		1,
		&stats_spec,
		&recv_buf_size,
		recv_buf,
		NULL);
	if (result != ERROR_SUCCESS) {
		ret = 1;
		goto end;
	}

	// 受信データ取得
	if (recv_buf->dwNumViews >= 1) {
		recv_num = (u32)recv_buf->pViews[0].dwNumRows;
	}
	else {
		recv_num = 0;
	}
	if (recv_num > (u32)RECVF_DATA_MAX) {
		recv_num = (u32)RECVF_DATA_MAX; // 念のため
	}
	m_recv_data_num = 0;
	for (u32 i = 0; i < recv_num; ++i) {
		const XUSER_STATS_ROW& row = recv_buf->pViews[0].pRows[i];
		AOS_NET_RANK_DATA& dest = m_recv_data[m_recv_data_num];
		if (UtilMakeRecvData(dest, row, m_recv_type, binfo, m_recv_asc)) {
			m_recv_data_num += 1;
		}
	}

	// 受信データ並びかえ
	for (u32 i = 0; i < m_recv_data_num; ++i) {
		u32 no1 = i;
		for (u32 j = i + 1; j < m_recv_data_num; ++j) {
			if (m_recv_data[j].rrank < m_recv_data[no1].rrank) {
				no1 = j;
			}
		}
		if (no1 != i) {
			AOS_NET_RANK_DATA temp = m_recv_data[i];
			m_recv_data[i] = m_recv_data[no1];
			m_recv_data[no1] = temp;
		}
	}

end:

	// 列挙ハンドル解放
	if (hEnum != INVALID_HANDLE_VALUE) {
		CloseHandle(hEnum);
		hEnum = INVALID_HANDLE_VALUE;
	}

	// バッファ解放
	if (recv_buf) {
		amMemFree(recv_buf);
		recv_buf = NULL;
	}
	if (xuid_buf) {
		amMemFree(xuid_buf);
		xuid_buf = NULL;
	}
	if (friend_buf) {
		amMemFree(friend_buf);
		friend_buf = NULL;
	}

	return ret;
}

// ===========================================================================
//! 接続スレッド - 周辺ランキング受信処理
// ===========================================================================
int CRank::ThreadProcSubRecvNear()
{
	HANDLE hEnum = INVALID_HANDLE_VALUE;
	XUSER_STATS_SPEC stats_spec;
	DWORD enum_buf_size;
	XUSER_STATS_READ_RESULTS* enum_buf = NULL;
	DWORD result;
	DWORD enum_result;
	u32 recv_num;
	int ret = 0;

	m_recv_data_num = 0;

	amAssert(m_recv_board_no < AOD_NET_RANK_BOARD_NUM);
	const AOS_NET_RANK_BOARD_INFO& binfo =
		g_ao_net_rank_board_info[m_recv_board_no];

	// 列挙準備
	stats_spec.dwViewId = binfo.board;
	stats_spec.rgwColumnIds[0] = binfo.region_col;
	if (binfo.ss_col >= 0) {
		stats_spec.dwNumColumnIds = 2;
		stats_spec.rgwColumnIds[1] = binfo.ss_col;
	}
	else {
		stats_spec.dwNumColumnIds = 1;
	}
	result = XUserCreateStatsEnumeratorByXuid(
		0,
		m_own_xuid,
		CRank::RECV_DATA_MAX,
		1,
		&stats_spec,
		&enum_buf_size,
		&hEnum);
	if ((result != ERROR_SUCCESS) || (enum_buf_size == 0)) {
		hEnum = INVALID_HANDLE_VALUE;
		ret = 1;
		goto end;
	}

	// バッファ確保
	enum_buf = (XUSER_STATS_READ_RESULTS*)amMemAlloc(enum_buf_size);

	// 列挙
	result = XEnumerate(hEnum, enum_buf, enum_buf_size, &enum_result, NULL);
	if (result != ERROR_SUCCESS) {
		ret = 1;
		goto end;
	}

	// 受信データ取得
	if (enum_buf->dwNumViews >= 1) {
		recv_num = (u32)enum_buf->pViews[0].dwNumRows;
	}
	else {
		recv_num = 0;
	}
	if (recv_num > (u32)RECV_DATA_MAX) {
		recv_num = (u32)RECV_DATA_MAX; // 念のため
	}
	m_recv_data_num = 0;
	for (u32 i = 0; i < recv_num; ++i) {
		const XUSER_STATS_ROW& row = enum_buf->pViews[0].pRows[i];
		AOS_NET_RANK_DATA& dest = m_recv_data[m_recv_data_num];
		if (UtilMakeRecvData(dest, row, m_recv_type, binfo, m_recv_asc)) {
			m_recv_data_num += 1;
		}
	}

end:

	// 列挙ハンドル解放
	if (hEnum != INVALID_HANDLE_VALUE) {
		CloseHandle(hEnum);
		hEnum = INVALID_HANDLE_VALUE;
	}

	// バッファ解放
	if (enum_buf) {
		amMemFree(enum_buf);
		enum_buf = NULL;
	}

	return ret;
}


// ***************************************************************************
// ユーティリティ
// ***************************************************************************
// ===========================================================================
//! 受信データから情報取得
// ===========================================================================
BOOL CRank::UtilMakeRecvData(
	AOS_NET_RANK_DATA& dest,
	const XUSER_STATS_ROW& row, AOE_NET_RANK_TYPE type,
	const AOS_NET_RANK_BOARD_INFO& binfo,
	BOOL is_asc)
{
	// 無効判定
	if (row.dwRank == 0) {
		return FALSE;
	}

	// フラグ初期化
	dest.flag = 0;

	// 名前取得
	amCopyMemory(dest.name, row.szGamertag, AOD_NET_RANK_NAME_LEN - 1);
	dest.name[AOD_NET_RANK_NAME_LEN - 1] = '\0';

	// 順位
	dest.rrank = dest.rank = (u32)(row.dwRank - 1);

	// スコア取得
	if (is_asc) {
		dest.score = (u32)(-row.i64Rating);
	}
	else {
		dest.score = (u32)row.i64Rating;
	}

	// 自身判定
	if (IsEqualXUID(row.xuid, m_own_xuid)) {
		dest.flag |= AOD_NET_RANK_FLAG_OWN;
	}

	// その他
	BOOL is_rg = FALSE;
	for (u32 i = 0; i < row.dwNumColumns; ++i) {

		const XUSER_STATS_COLUMN& col = row.pColumns[i];

		if (col.wColumnId == binfo.region_col) {
			if (col.Value.type == XUSER_DATA_TYPE_INT32) {
				dest.region = (u8)col.Value.nData;
				if (dest.region >= AOD_NET_RANK_REGION_NUM) {
					dest.region = AOD_NET_RANK_REGION_OTHER;
				}
				is_rg = TRUE;
			}
		}

		else if ((binfo.ss_col >= 0) && (col.wColumnId == binfo.ss_col)) {
			if (col.Value.type == XUSER_DATA_TYPE_INT32) {
				if (col.Value.nData) {
					dest.flag |= AOD_NET_RANK_FLAG_SS;
				}
				else {
					dest.flag &= ~AOD_NET_RANK_FLAG_SS;
				}
			}
		}

		else {
			return FALSE;
		}
	}
	if (!is_rg) {
		return FALSE;
	}

	// XUID
	dest.xuid = row.xuid;

	return TRUE;
}


// ***************************************************************************
// エラー
// ***************************************************************************
// ===========================================================================
//! エラー設定
// ===========================================================================
void CRank::SetError(u32 error)
{
	// 既にエラー設定済みなら上書きしない
	if (!IsError()) {
		m_error = error;
	}
	g_ao_net_rank_error = TRUE;
}

} // namespace net
} // namespace ao

#include "akUtil.h"

// ===========================================================================
//! タイムをミリ秒に変換
// ===========================================================================
s32 aoNetRankConvTimeToMsec(u32 time)
{
	u16 min, sec, msec;
	AkUtilFrame60ToTime(time, &min, &sec, &msec);
	return (s32)((((min * 60) + sec) * 1000) + msec);
}

#endif // defined(AOD_PLATFORM_XBOX360)

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
