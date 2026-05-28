// ===========================================================================
/*!
	@file	aoNetRankWii.cpp
	@brief	AoLibrary ネットワークランキングモジュール定義(Wii)

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"
#include "aoNetRank.h"
#include "gs.h"

#if defined(AOD_PLATFORM_WII)

// ----- Macros ------------------------------------------------（マクロ定義）

#if defined(MTD_DEBUG)
//! デバッグ用のサーバを使用する(無効にする場合はコメントアウト)
#define HOG_DWC_SERVER_DEBUG		(1)
#endif // defined(MTD_DEBUG)

#if defined(HOG_DWC_SERVER_DEBUG)
// デバッグ用サーバ使用

#define HOG_DWC_AUTHSERVER		DWC_AUTHSERVER_DEBUG	//!< 認証サーバ

#else
// 製品用サーバ使用

#define HOG_DWC_AUTHSERVER		DWC_AUTHSERVER_RELEASE	//!< 認証サーバ

#endif // defined(AOD_NET_RANK_SERVER_DEBUG)

#define HOG_DWC_GAMENAME		"sonicdlwii"	//!< ゲーム名
#define HOG_DWC_GAMECODE		'WSNJ'			//!< イニシャルコード
#define HOG_DWC_GSPRODUCTID		(12069)			//!< プロダクトID
#define HOG_DWC_GSSECRETKEY		"DkJwkG"		//!< シークレットキー

//! ランキング初期化データ
#define HOG_DWC_RANK_INITDATA "AjvjvnZGBuBaehGRejFP0003a8b50000627d000200003db8537dsonicdlwii"

// ユーザ定義データ
#define AOD_NET_RANK_USER_DATA_FLAG_SS		(1 << 0)	//!< Sソニック使用
#define AOD_NET_RANK_USER_DATA_NAME_LEN		(10)		//!< 名前長

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct AOS_NET_RANK_USER_DATA
// ---------------------------------------------------------------------------
//!	ランキングユーザ定義データ
// ===========================================================================
typedef struct tag_AOS_NET_RANK_USER_DATA {
	char	name[AOD_NET_RANK_USER_DATA_NAME_LEN];		//!< 名前
	u8		is_ss;						//!< 真：スーパーソニック使用
	u8		pad[5];						//!< パディング
} AOS_NET_RANK_USER_DATA; // 16 byte

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
	CRank(DWCUserData* userdata);

	//! デストラクタ
	virtual ~CRank();

	//! ユーザデータ保存必要判定
	BOOL IsNeedUserdataSave() const;

	//! ユーザデータ保存完了通知
	void NoticeUserdataSaved();

	//! 接続完了判定
	BOOL IsConnected();


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


	//! 操作無効判定
	BOOL IsControlDisable() const;

	//! エラー判定
	BOOL IsError() const;

	//! 終了開始
	void End();

	//! ログインコールバック
	void LoginCallback(DWCError error, int profileID);

protected:

	// タスクプロシージャ
	void TaskProcMain0000();

	// プロシージャ
	void ProcInit();
	void ProcConnecting();
	void ProcWaitUserdataSaved();
	void ProcConnected();
	void ProcSending();
	void ProcRecving();
	void ProcDisconnecting();
	void ProcError();
	void ProcFatalError();
	void ProcErrorMessageWait();
	void ProcErrorMessage();

	// スレッドプロシージャ
	void ThreadProcConnect();

	//! エラー
	void SetError(u32 error);

	enum {
		// 状態
		STATE_CONNECTING	= 0,	//!< 接続中
		STATE_CONNECTED,			//!< 接続済み
		STATE_SENDING,				//!< 送信中
		STATE_RECVING,				//!< 受信中

		// エラー
		ERROR_NONE		= 0,	//!< エラー無し
		ERROR_NAME,				//!< 不正な名前
		ERROR_LOCK,				//!< WiFi制限
		ERROR_OPERATION,		//!< 不正な関数呼び出し
		ERROR_OTHER,			//!< その他のエラー
		ERROR_FATAL,			//!< 致命的なエラー
		ERROR_NUM,				//!< エラー数

		//! 受信データ最大数
		SEND_DATA_MAX	= 1,	//!< 送信データ数
		RECV_DATA_MAX	= 10,

		//! 初期化済みフラグ
		INIT_FLAG_SO	= 0,	//!< SO初期化済み
		INIT_FLAG_SO_S,			//!< SO開始済み
		INIT_FLAG_DWC,			//!< DWC初期化済み
		INIT_FLAG_FRD,			//!< friendmatch初期化済み
		INIT_FLAG_RNK,			//!< ランキング初期化済み

		INIT_FLAG_SO_BIT	= (1 << INIT_FLAG_SO),	//!< SO初期化済み
		INIT_FLAG_SO_S_BIT	= (1 << INIT_FLAG_SO_S),//!< SO開始済み
		INIT_FLAG_DWC_BIT	= (1 << INIT_FLAG_DWC),	//!< DWC初期化済み
		INIT_FLAG_FRD_BIT	= (1 << INIT_FLAG_FRD),	//!< friendmatch初期化済み
		INIT_FLAG_RNK_BIT	= (1 << INIT_FLAG_RNK),	//!< ランキング初期化済み

		// 定数
		NAME_LEN = 16,	//!< ユーザ名長
		WAIT_TIME = 30,	//!< 通信開始待ち時間
	};

	//! 送信情報
	struct SEND_INFO {
		u32					board_no;		//!< ボード番号
		AOE_NET_RANK_TYPE	type;			//!< 送信タイプ
		AOS_NET_RANK_SEND	data;			//!< 送信データ
	};

	DWCUserData* m_userdata; //!< ユーザデータ
	int m_gs_profile_id; //!< GSプロファイルID
	BOOL m_end_flag; //!< 終了フラグ

	u32 m_init_flag; //!< 初期化済みフラグ

	u32 m_error; //!< エラー
	DWCError m_dwc_error; //!< DWCエラー
	int m_dwc_error_code; //!< DWCエラーコード
	DWCErrorType m_dwc_error_type; //!< DWCエラータイプ

	AMS_MUTEX m_mutex; //!< ミューテックス

	u32 m_state; //!< 状態
	BOOL m_is_login; //!< ログイン済みフラグ
	BOOL m_is_need_userdata_save; //!< ユーザデータセーブ必要フラグ

	//! 送信データ
	SEND_INFO m_send_tbl[SEND_DATA_MAX]; //!< 送信情報配列
	u32 m_send_num; //!< 送信情報数
	BOOL m_send_success; //!< 送信成功フラグ

	//! 受信データ
	AOS_NET_RANK_DATA m_recv_data[RECV_DATA_MAX];
	AOE_NET_RANK_TYPE m_recv_type; //!< 受信ランキングタイプ
	BOOL m_recv_success; //!< 受信成功フラグ
	u32 m_recv_board_no; //!< 受信ランキングボード番号
	u32 m_recv_rank; //!< 受信ランク
	BOOL m_recv_asc; //!< 受信昇順ソート
	u32 m_recv_data_num; //!< 受信データ数

	//!< ランキングデータ(送受信用)
	AOS_NET_RANK_USER_DATA m_rank_data ATTRIBUTE_ALIGN(32);

	u16 m_username[NAME_LEN]; //!< ユーザ名(UTF16)

	u32 m_init_wait; //!< 通信待ち時間
};

} // namespace net
} // namespace ao

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// SO用
static void* aoNetRankSoMemAlloc(u32 name, s32 size);
static void aoNetRankSoMemFree(u32 name, void* ptr, s32 size);

// DWC用
static void aoNetRankDwcLoginCallback(
	DWCError error, int profileID, void* param);
static void* aoNetRankDwcMemAlloc(DWCAllocType name, u32 size, int align);
static void aoNetRankDwcMemFree(DWCAllocType name, void* ptr, u32 size);

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
//	DWCUserData* g_ao_net_rank_userdata
// ---------------------------------------------------------------------------
//!	ユーザデータ
// ===========================================================================
static DWCUserData* g_ao_net_rank_userdata = NULL;

// ===========================================================================
//	char* g_ao_net_rank_username
// ---------------------------------------------------------------------------
//!	ユーザ名(ASCII)
// ===========================================================================
static const char* g_ao_net_rank_username = NULL;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ***************************************************************************
// Wii
// ***************************************************************************
// ===========================================================================
//	AoNetRankWiiSetUserData
/*!
	WiFiユーザデータ設定

	@param data		[io] ユーザデータ
	@note
	WiFi通信で使用するユーザデータ領域のポインタを設定してください。\n
	WiFi通信中は、
	この関数によるユーザデータの切り替えを行なわないようにして下さい。\n
	ここで設定したユーザデータ領域は、通信中動的に変更されます。\n
	AoNetRankStart関数呼び出し前に設定するようにして下さい。\n
*/
// ===========================================================================
void AoNetRankWiiSetUserData(DWCUserData* data)
{
	g_ao_net_rank_userdata = data;
}

// ===========================================================================
//	AoNetRankWiiSetUserName
/*!
	ユーザ名設定

	@param name		[in] ユーザ名(ASCIIコード)
	@note
	ランキングにアップする名前を指定して下さい。\n
	文字コードはASCIIです。\n
	WiFi接続時に名前のチェックを行い、不適切な名前の場合はエラーとなります。\n
	AoNetRankStart関数呼び出し前に設定するようにして下さい。\n
*/
// ===========================================================================
void AoNetRankWiiSetUserName(const char* name)
{
	g_ao_net_rank_username = name;
}


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
		// 既に開始済み
		amAssert(0);
		return;
	}

	// 接続開始
	amAssert(g_ao_net_rank_userdata);
	g_ao_net_rank = new ao::net::CRank(g_ao_net_rank_userdata);
}

// ===========================================================================
//	AoNetRankWiiIsNeedSaveUserdata
/*!
	ユーザデータの保存が必要か判定

	@return 真：必要　偽：それ以外
	@note
	AoNetRankStart関数で開始した接続処理中に、
	ユーザデータの保存が必要になったかどうかを判定します。\n
	この関数がTRUEを返す場合は、ユーザデータの保存を行い、
	AoNetRankWiiNoticeUserdataSaved関数で
	保存した旨を通知するようにして下さい。\n
	AoNetRankWiiNoticeUserdataSaved関数を呼び出した段階で
	この関数はFALSEを返すようになります。\n
	この関数は、AoNetRankStart関数呼び出しから、
	AoNetRankIsConnected関数がTRUEを返すまでの間に使用するようにして下さい。\n
*/
// ===========================================================================
BOOL AoNetRankWiiIsNeedSaveUserdata(void)
{
	if (!g_ao_net_rank) {
		return FALSE;
	}
	return g_ao_net_rank->IsNeedUserdataSave();
}

// ===========================================================================
//	AoNetRankWiiPreSaveUserdata
/*!
	ユーザデータ保存前処理

	@note
	ユーザデータを保存するための前処理として、
	ダーティーフラグの削除などを行ないます。\n
	この関数を呼び出した場合は、必ずユーザデータを保存するようにして下さい。\n
*/
// ===========================================================================
void AoNetRankWiiPreSaveUserdata(void)
{
	if (!AoNetRankWiiIsNeedSaveUserdata()) {
		amAssert(0);
		return;
	}
	// ユーザデータのダーティーフラグを消去
	DWC_ClearDirtyFlag(g_ao_net_rank_userdata);
}

// ===========================================================================
//	AoNetRankWiiNoticeUserdataSaved
/*!
	ユーザデータの保存完了通知

	@note
	AoNetRankWiiIsNeedSaveUserdata関数がTRUEを返した場合に行なう
	ユーザデータの保存処理が完了した際に呼び出して下さい。\n
	この関数を呼び出さない場合は、通信処理が進行しなくなります。\n
*/
// ===========================================================================
void AoNetRankWiiNoticeUserdataSaved(void)
{
	if (!g_ao_net_rank) {
		amAssert(0);
		return;
	}
	return g_ao_net_rank->NoticeUserdataSaved();
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
	if (!g_ao_net_rank) {
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
	if (!g_ao_net_rank) {
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
	if (!g_ao_net_rank) {
		return;
	}

	// 終了開始
	g_ao_net_rank->End();
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
	g_ao_net_rank->SendSetData(board, type, data);
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
	if (!g_ao_net_rank) {
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
	if (!g_ao_net_rank) {
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
	if (!g_ao_net_rank) {
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
	if (!g_ao_net_rank) {
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
	if (!g_ao_net_rank) {
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
	if (!g_ao_net_rank) {
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
	if (!g_ao_net_rank) {
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
	if (!g_ao_net_rank) {
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
	if (!g_ao_net_rank) {
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
	return data->score;
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
namespace net {

// ***************************************************************************
// ランキングクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CRank::CRank(DWCUserData* userdata)
{
	// HBM無効化
	amWiiSetEnableHBM(0);

	// メンバ初期化
	m_userdata = NULL;
	m_gs_profile_id = 0;
	m_end_flag = FALSE;
	m_init_flag = 0;
	m_error = ERROR_NONE;
	m_dwc_error = DWC_ERROR_NONE;
	m_dwc_error_code = 0;
	m_dwc_error_type = DWC_ETYPE_NO_ERROR;
	m_is_login = FALSE;
	m_is_need_userdata_save = FALSE;

	amZeroMemory(&m_send_tbl, sizeof(CRank::SEND_INFO) * CRank::SEND_DATA_MAX);
	m_send_num = 0;
	m_send_success = FALSE;

	amZeroMemory(m_recv_data, sizeof(AOS_NET_RANK_DATA) * RECV_DATA_MAX);
	m_recv_type = AOD_NET_RANK_TYPE_NONE;
	m_recv_success = FALSE;
	m_recv_board_no = 0;
	m_recv_rank = 0;
	m_recv_data_num = 0;
	m_init_wait = 0;

	// 状態設定
	m_state = CRank::STATE_CONNECTING;

	// ユーザデータ設定
	m_userdata = userdata;

	// 初期化処理プロシージャ設定
	SetProc(0, &CRank::ProcInit);

	// メインタスク作成
	MakeTask(0, "aoNetRank::Main");

	// メインタスクプロシージャ設定
	SetTaskProc(0, &CRank::TaskProcMain0000);

	// ミューテックス作成
	amMutexCreate(&m_mutex);

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

	// ランキング終了
	if (m_init_flag & INIT_FLAG_RNK_BIT) {
		amAssert(0); // 既に終了済みのはず
		DWC_RnkShutdown();
		m_init_flag &= ~INIT_FLAG_RNK_BIT;
	}

	// フレンドマッチ終了
	if (m_init_flag & INIT_FLAG_FRD_BIT) {
		amAssert(0); // 既に終了済みのはず
		DWC_ShutdownFriendsMatch();
		m_init_flag &= ~INIT_FLAG_FRD_BIT;
	}

	// DWCライブラリ終了
	if (m_init_flag & INIT_FLAG_DWC_BIT) {
		amAssert(0); // 既に終了済みのはず
		DWC_Shutdown();
		m_init_flag &= ~INIT_FLAG_DWC_BIT;
	}

	// ソケットライブラリ終了
	if (m_init_flag & INIT_FLAG_SO_S_BIT) {
		amAssert(0); // 既に終了済みのはず
		SOCleanup();
		m_init_flag &= ~INIT_FLAG_SO_S_BIT;
	}

	// ソケットライブラリ終了
	if (m_init_flag & INIT_FLAG_SO_BIT) {
		amAssert(0); // 既に終了済みのはず
		SOFinish();
		m_init_flag &= ~INIT_FLAG_SO_BIT;
	}

	g_ao_net_rank = NULL;

	// HBM有効化
	amWiiSetEnableHBM(1);
}

// ===========================================================================
//! ユーザデータ保存必要判定
// ===========================================================================
BOOL CRank::IsNeedUserdataSave() const
{
	if (IsProc(0, &CRank::ProcWaitUserdataSaved)) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! ユーザデータ保存完了通知
// ===========================================================================
void CRank::NoticeUserdataSaved()
{
	if (IsNeedUserdataSave()) {
		amAssert(!DWC_CheckDirtyFlag(m_userdata));
		amMutexLock(&m_mutex);
		m_is_need_userdata_save = FALSE;
		amMutexUnlock(&m_mutex);
		SetProc(0, &CRank::ProcConnecting);
	}
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
	// キャンセルは無し
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
	SetOwnProc(&CRank::ProcRecving);
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
//! 終了開始
// ===========================================================================
void CRank::End()
{
	// 終了フラグ設定
	m_end_flag = TRUE;
}

// ===========================================================================
//! ログインコールバック
// ===========================================================================
void CRank::LoginCallback(DWCError error, int profileID)
{
	// エラー判定
	if (error != DWC_ERROR_NONE) {
		SetError(CRank::ERROR_OTHER);
		SetProc(0, &CRank::ProcError);
		// エラーでもログイン設定は必要なのでreturnしない
	}

	// GSプロファイルID保持
	m_gs_profile_id = profileID;

	// ログイン設定
	amMutexLock(&m_mutex);
	m_is_login = TRUE;
	amMutexUnlock(&m_mutex);
}


// ***************************************************************************
// タスクプロシージャ
// ***************************************************************************
// ===========================================================================
//! メインタスク0000
// ===========================================================================
void CRank::TaskProcMain0000()
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

		amMutexLock(&m_mutex);
		// FriendsMatchライブラリ更新
		if (m_init_flag & INIT_FLAG_FRD_BIT) {
			DWC_ProcessFriendsMatch();
		};

		// 名前チェック
		amMutexUnlock(&m_mutex);
	}
}


// ***************************************************************************
// プロシージャ
// ***************************************************************************
// ===========================================================================
//! 初期化処理
// ===========================================================================
void CRank::ProcInit()
{
	// HBMが表示されなくなって一定時間経過したら通信開始
	if (GetCount() == 0) {
		m_init_wait = 0;
	}
	if (!amWiiIsDrawHBM()) {
		m_init_wait += 1;
	}
	else {
		m_init_wait = 0;
	}
	if (m_init_wait >= WAIT_TIME) {

		// スレッド作成
		SetThreadProc(0, &CRank::ThreadProcConnect);
		OSPriority prio = OSGetThreadPriority(OSGetCurrentThread()) + 1;
		StartThread(0, (AMD_CORE)0, (u32)prio);

		// 接続中へ遷移
		SetOwnProc(&CRank::ProcConnecting);
	}
}

// ===========================================================================
//! 接続中
// ===========================================================================
void CRank::ProcConnecting()
{
	amMutexLock(&m_mutex);
	// エラー判定
	if (m_error != CRank::ERROR_NONE) {
		SetOwnProc(&CRank::ProcError);
		amMutexUnlock(&m_mutex);
		return;
	}
	if (m_init_flag & INIT_FLAG_DWC_BIT) {
		if (DWC_GetLastError(NULL) != DWC_ERROR_NONE) {
			SetError(CRank::ERROR_OTHER);
			SetOwnProc(&CRank::ProcError);
			amMutexUnlock(&m_mutex);
			return;
		}
	}
	amMutexUnlock(&m_mutex);

	// キャンセル判定
	if (m_end_flag) {
		SetOwnProc(&CRank::ProcDisconnecting);
		return;
	}

	// ユーザデータセーブ必要判定
	amMutexLock(&m_mutex);
	if (m_is_need_userdata_save) {
		// ユーザデータセーブ待ちへ遷移
		SetOwnProc(&CRank::ProcWaitUserdataSaved);
	}
	amMutexUnlock(&m_mutex);

	// 接続済み判定
	amMutexLock(&m_mutex);
	if (m_state == CRank::STATE_CONNECTED) {
		// 接続済みへ遷移
		SetOwnProc(&CRank::ProcConnected);
	}
	amMutexUnlock(&m_mutex);
}

// ===========================================================================
//! ユーザデータ保存待ち
// ===========================================================================
void CRank::ProcWaitUserdataSaved()
{
	amMutexLock(&m_mutex);
	// エラー判定
	if (m_error != CRank::ERROR_NONE) {
		SetOwnProc(&CRank::ProcError);
		amMutexUnlock(&m_mutex);
		return;
	}
	if (m_init_flag & INIT_FLAG_DWC_BIT) {
		if (DWC_GetLastError(NULL) != DWC_ERROR_NONE) {
			SetError(CRank::ERROR_OTHER);
			SetOwnProc(&CRank::ProcError);
			amMutexUnlock(&m_mutex);
			return;
		}
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
//! 接続済み
// ===========================================================================
void CRank::ProcConnected()
{
	// エラー判定
	if (DWC_GetLastError(NULL) != DWC_ERROR_NONE) {
		SetError(CRank::ERROR_OTHER);
		SetOwnProc(&CRank::ProcError);
		return;
	}

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
	DWCRnkError error;

	// エラー判定
	if (DWC_GetLastError(NULL) != DWC_ERROR_NONE) {
		goto error;
	}

	if (GetCount() == 0) {

		// 送信データが無いなら終了
		if (m_send_num == 0) {
			m_send_success = TRUE;
			m_state = CRank::STATE_CONNECTED;
			SetOwnProc(&CRank::ProcConnected);
			return;
		}

		CRank::SEND_INFO& send = m_send_tbl[m_send_num - 1];
		m_send_num -= 1;

		// 送信開始
		amZeroMemory(&m_rank_data, sizeof(AOS_NET_RANK_USER_DATA));
		strncpy(
			m_rank_data.name,
			g_ao_net_rank_username,
			AOD_NET_RANK_USER_DATA_NAME_LEN);
		if (send.data.ss) {
			m_rank_data.is_ss = (u8)1;
		};
		DWCRnkRegion region;
		switch (GsEnvGetRegion()) {
		case GSD_REGION_JP:
			region = DWC_RNK_REGION_JP;
			break;
		case GSD_REGION_US:
			if (GsEnvIsRegionAsia()) {
				region = DWC_RNK_REGION_ALL;
			}
			else {
				region = DWC_RNK_REGION_US;
			}
			break;
		case GSD_REGION_EU:
			region = DWC_RNK_REGION_EU;
			break;
		default:
			region = DWC_RNK_REGION_ALL;
			break;
		}
		error = DWC_RnkPutScoreAsync(
			send.board_no,
			region,
			(s32)send.data.time,
			&m_rank_data,
			sizeof(AOS_NET_RANK_USER_DATA));
		if (error != DWC_RNK_SUCCESS) {
			// エラー
			goto error;
		}
	}

	// 送信処理
	error = DWC_RnkProcess();
	if (error != DWC_RNK_SUCCESS) {
		// エラー
		DWC_RnkCancelProcess();
		goto error;
	}

	// 完了判定
	switch (DWC_RnkGetState()) {
	case DWC_RNK_STATE_PUT_ASYNC:
		// 処理中
		break;

	case DWC_RNK_STATE_COMPLETED:
		// 完了
		SetOwnProc(&CRank::ProcSending);
		break;

	case DWC_RNK_STATE_NOTREADY:
	case DWC_RNK_STATE_INITIALIZED:
	case DWC_RNK_STATE_GET_ASYNC:
	case DWC_RNK_STATE_ERROR:
	default:
		// エラー
		DWC_RnkCancelProcess();
		goto error;
		break;
	}

	return;

error:

	m_state = CRank::STATE_CONNECTED;
	SetError(CRank::ERROR_OTHER);
	SetOwnProc(&CRank::ProcError);
}

// ===========================================================================
//! 受信中
// ===========================================================================
void CRank::ProcRecving()
{
	DWCRnkError error;

	// エラー判定
	if (DWC_GetLastError(NULL) != DWC_ERROR_NONE) {
		goto error;
	}

	if (GetCount() == 0) {
		// 受信開始
		DWCRnkGetMode mode;
		DWCRnkGetParam param;
		if (m_recv_rank == AOD_NET_RANK_OWN_NEAR) {
			mode = DWC_RNK_GET_MODE_NEAR;
			param.size = sizeof(DWCRnkGetParam_nearby);
			param.nearby.limit = RECV_DATA_MAX;
			param.nearby.since = 0;
			if (m_recv_asc) {
				param.nearby.sort = DWC_RNK_ORDER_ASC;
			}
			else {
				param.nearby.sort = DWC_RNK_ORDER_DES;
			}
		}
		else {
			mode = DWC_RNK_GET_MODE_TOPLIST;
			param.size = sizeof(DWCRnkGetParam_toplist);
			param.toplist.limit = RECV_DATA_MAX;
			param.toplist.since = 0;
			if (m_recv_asc) {
				param.toplist.sort = DWC_RNK_ORDER_ASC;
			}
			else {
				param.toplist.sort = DWC_RNK_ORDER_DES;
			}
		}
		error = DWC_RnkGetScoreAsync(
			mode, m_recv_board_no, DWC_RNK_REGION_ALL, &param);
		if (error != DWC_RNK_SUCCESS) {
			// エラー
			goto error;
		}
	}

	// 受信処理
	error = DWC_RnkProcess();
	if (error != DWC_RNK_SUCCESS) {
		// エラー
		DWC_RnkCancelProcess();
		goto error;
	}

	// 完了判定
	switch (DWC_RnkGetState()) {
	case DWC_RNK_STATE_GET_ASYNC:
		// 処理中
		break;

	case DWC_RNK_STATE_COMPLETED:
		// 完了
		{
			// 受信したリスト数取得
			error = DWC_RnkResGetRowCount(&m_recv_data_num);
			if (error != DWC_RNK_SUCCESS) {
				goto error;
			}
			if (m_recv_data_num > (u32)RECV_DATA_MAX) {
				m_recv_data_num = (u32)RECV_DATA_MAX; // 念のため
			}

			// 自身の記録が正常に格納されているかチェック
			for (u32 i = 0; i < m_recv_data_num; ++i) {
				DWCRnkData recv_data;
				error = DWC_RnkResGetRow(&recv_data, i);
				if (error != DWC_RNK_SUCCESS) {
					goto error;
				}
				if (recv_data.pid != m_gs_profile_id) {
					continue;
				}
				if ((int)recv_data.region == -1) {
					m_recv_data_num = 0;
					break;
				}
			}

			// 受信データ取得
			u32 own_rank = 0;
			BOOL own_enable = FALSE;
			for (u32 i = 0; i < m_recv_data_num; ++i) {
				DWCRnkData recv_data;
				error = DWC_RnkResGetRow(&recv_data, i);
				if (error != DWC_RNK_SUCCESS) {
					goto error;
				}
				AOS_NET_RANK_USER_DATA* recvudata =
					(AOS_NET_RANK_USER_DATA*)recv_data.userdata;
				AOS_NET_RANK_DATA& dest = m_recv_data[i];
				amCopyMemory(
					dest.name, recvudata->name,
					sizeof(char) * AOD_NET_RANK_USER_DATA_NAME_LEN);
				dest.name[AOD_NET_RANK_USER_DATA_NAME_LEN] = '\0';
				dest.flag = 0;
				dest.rank = (u32)recv_data.order;
				switch (recv_data.region) {
				case DWC_RNK_REGION_JP:
					dest.region = AOD_NET_RANK_REGION_JP;
					break;
				case DWC_RNK_REGION_US:
					dest.region = AOD_NET_RANK_REGION_US;
					break;
				case DWC_RNK_REGION_EU:
					dest.region = AOD_NET_RANK_REGION_EU;
					break;
				default:
					dest.region = AOD_NET_RANK_REGION_OTHER;
					break;
				}
				dest.time = (u32)recv_data.score;
				if (recvudata->is_ss) {
					dest.flag |= AOD_NET_RANK_FLAG_SS;
				}

				// 自身のスコア判定
				if ((recv_data.pid == m_gs_profile_id) ||
					(recv_data.order > 0))
				{
					dest.flag |= AOD_NET_RANK_FLAG_OWN;
					own_enable = TRUE;
					own_rank = (u32)recv_data.order;
				}
				else {
					dest.flag &= ~AOD_NET_RANK_FLAG_OWN;
				}
			}

			// 受信データ整理
			if (m_recv_rank == AOD_NET_RANK_OWN_NEAR) {
				if (own_enable) {
					// スコア順にソートする
					u32 own_index = 0;
					for (u32 i = 1; i < m_recv_data_num; ++i) {
						AOS_NET_RANK_DATA& own = m_recv_data[i - 1];
						AOS_NET_RANK_DATA& tgt = m_recv_data[i];
						if (m_recv_asc) {
							if (own.time <= tgt.time) {
								break;
							}
						}
						else {
							if (own.time >= tgt.time) {
								break;
							}
						}
						AOS_NET_RANK_DATA temp = own;
						own = tgt;
						tgt = temp;
						own_index = i;
					}
					// 順位設定
					u32 top_rank = 0;
					if (own_rank >= (own_index + 1)) {
						top_rank = (u32)(own_rank - 1 - own_index);
					}
					for (u32 i = 0; i < m_recv_data_num; ++i) {
						AOS_NET_RANK_DATA& dest = m_recv_data[i];
						if (i == 0) {
							dest.rank = top_rank;
						}
						else {
							AOS_NET_RANK_DATA& prev = m_recv_data[i - 1];
							if (dest.time == prev.time) {
								dest.rank = prev.rank;
							}
							else {
								dest.rank = (u32)(top_rank + i);
							}
						}
						if (dest.flag & AOD_NET_RANK_FLAG_OWN) {
							amAssert(dest.rank == (own_rank - 1));
						}
						dest.rrank = (u32)(top_rank + i);
					}
				}
				else {
					// 無効にする
					m_recv_data_num = 0;
				}
			}
			else {
				// 順位順になっているので順位を設定
				for (u32 i = 0; i < m_recv_data_num; ++i) {
					AOS_NET_RANK_DATA& dest = m_recv_data[i];
					if (i == 0) {
						dest.rank = 0;
					}
					else {
						AOS_NET_RANK_DATA& prev = m_recv_data[i - 1];
						if (dest.time == prev.time) {
							dest.rank = prev.rank;
						}
						else {
							dest.rank = i;
						}
					}
					dest.rrank = i;
				}
			}
		}
		m_recv_success = TRUE;
		m_state = CRank::STATE_CONNECTED;
		SetOwnProc(&CRank::ProcConnected);
		break;

	case DWC_RNK_STATE_NOTREADY:
	case DWC_RNK_STATE_INITIALIZED:
	case DWC_RNK_STATE_PUT_ASYNC:
	case DWC_RNK_STATE_ERROR:
	default:
		// エラー
		DWC_RnkCancelProcess();
		goto error;
		break;
	}

	return;

error:

	m_state = CRank::STATE_CONNECTED;
	SetError(CRank::ERROR_OTHER);
	SetOwnProc(&CRank::ProcError);
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
		if (m_error != CRank::ERROR_NONE) {
			// エラーメッセージ表示へ遷移
			SetOwnProc(&CRank::ProcErrorMessageWait);
			return;
		}

		// 終了
		SetOwnProcNone();
	}
}

// ===========================================================================
//! エラー発生
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
//! 致命的なエラー発生
// ===========================================================================
void CRank::ProcFatalError()
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
	// 既存メッセージ表示終了待ち
	if (AoSysMsgIsFinished()) {

		AOE_SYS_MSG_SELECT select;
		AOE_SYS_MSG_ID msg;
		BOOL is_show_code;

		// 不正な名前判定
		if (m_error == CRank::ERROR_NAME) {
			select = AOD_SYS_MSG_SELECT_OK;
			is_show_code = FALSE;
			msg = AOD_SYS_MSG_NET_ERROR_DWC0001;
		}

		// WiFi制限
		else if (m_error == CRank::ERROR_LOCK) {
			select = AOD_SYS_MSG_SELECT_OK;
			is_show_code = FALSE;
			msg = AOD_SYS_MSG_NET_ERROR_DWC0003;
		}

		else {

			// FATALエラー判定
			if (m_dwc_error_type == DWC_ETYPE_FATAL) {
				select = AOD_SYS_MSG_SELECT_DISABLE;
				is_show_code = TRUE;
			}
			else {
				// 通常エラー
				select = AOD_SYS_MSG_SELECT_OK;

				// エラーコードの表示の有無を判定
				switch (m_dwc_error_type) {
				case DWC_ETYPE_SHOW_ERROR:
				case DWC_ETYPE_SHUTDOWN_FM:
				case DWC_ETYPE_SHUTDOWN_GHTTP:
				case DWC_ETYPE_SHUTDOWN_ND:
				case DWC_ETYPE_DISCONNECT:
				case DWC_ETYPE_FATAL:
				case DWC_ETYPE_SHUTDOWN_TAC:
					is_show_code = TRUE;
					break;
				case DWC_ETYPE_NO_ERROR:
				case DWC_ETYPE_LIGHT:
				default:
					is_show_code = FALSE;
					break;
				}
			}

			// 表示メッセージ判定
			int code = -m_dwc_error_code;
			if (((code >= 20102) && (code <= 20109)) ||
				((code >= 20111) && (code <= 20999)))
			{
				msg = AOD_SYS_MSG_NET_ERROR_E001;
			}
			else if ((code == 20101) || ((code >= 23000) && (code <= 23999))) {
				msg = AOD_SYS_MSG_NET_ERROR_E002;
			}
			else if (code == 20110) {
				msg = AOD_SYS_MSG_NET_ERROR_E003;
			}
			else if (code == 29000) {
				msg = AOD_SYS_MSG_NET_ERROR_E004;
			}
			else if (code == 29001) {
				msg = AOD_SYS_MSG_NET_ERROR_E005;
			}
			else if ((code == 20100) || ((code >= 50000) && (code <= 59999)))
			{
				msg = AOD_SYS_MSG_NET_ERROR_E006;
			}
			else {
				if (m_dwc_error_type != DWC_ETYPE_FATAL) {
					msg = AOD_SYS_MSG_NET_ERROR_E007;
				}
				else {
					msg = AOD_SYS_MSG_NET_ERROR_E008;
				}
			}
		}

		// エラー表示開始
		if (m_dwc_error_code >= 0) {
			is_show_code = FALSE;
		}
		if (is_show_code) {
			AoSysMsgStart(msg, -m_dwc_error_code, select);
		}
		else {
			AoSysMsgStart(msg, select);
		}

		// エラーメッセージ表示へ遷移
		SetOwnProc(&CRank::ProcErrorMessage);
	}
}

// ===========================================================================
//! エラーメッセージ表示
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
	int ret;
	DWCRnkError rnk_error;
	u32 flag;
	s32 dstlen;
	u16* name_tbl[1];
	char cn_result;
	int badwordsnum;
	BOOL is_name_fail = FALSE;

	// Wi-Fi接続制限判定
	if (SCCheckPCMessageRestriction()) {
		// エラー
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_LOCK);
		amMutexUnlock(&m_mutex);
		goto wait;
	}

	// ソケットライブラリ初期化
	SOLibraryConfig so_config;
	amZeroMemory(&so_config, sizeof(SOLibraryConfig));
	so_config.alloc = aoNetRankSoMemAlloc;
	so_config.free = aoNetRankSoMemFree;
	ret = SOInit(&so_config);
	if (ret < 0) {
		amMutexLock(&m_mutex);
		switch (ret) {
		case SO_EALREADY:
		case SO_EINVAL:
		case SO_ENOMEM:
			// プログラムのエラー
			SetError(CRank::ERROR_OPERATION);
			m_dwc_error = DWC_ERROR_NETWORK;
			m_dwc_error_code = 0;
			m_dwc_error_type = DWC_ETYPE_SHOW_ERROR;
			break;

		default:
			// 致命的なエラーとして扱い、ゲームの進行を停止させる
			SetError(CRank::ERROR_FATAL);
			break;
		}
		amMutexUnlock(&m_mutex);
		goto wait;
	}
	amMutexLock(&m_mutex);
	m_init_flag |= INIT_FLAG_SO_BIT;
	amMutexUnlock(&m_mutex);

	// 終了判定
	if (IsRequestEndThread()) {
		goto end;
	}

	// ソケットライブラリ開始
	ret = SOStartup();
	if (ret < 0) {
		amMutexLock(&m_mutex);
		switch (ret) {
		case SO_EALREADY:
		case SO_EBUSY:
		case SO_EINVAL:
		case SO_ENETRESET:
		case SO_ENOENT:
		case SO_ENOLINK:
		case SO_ENXIO:
		case SO_ETIMEDOUT:
		case SO_ERR_DHCP_TIMEOUT:
		case SO_ERR_DHCP_EXPIRED:
		case SO_ERR_DHCP_NAK:
		case SO_ERR_ADDR_COLLISION:
		case SO_ERR_LINK_DOWN:
		case SO_ERR_LINK_UP_TIMEOUT:
			SetError(CRank::ERROR_OTHER);
			m_dwc_error = DWC_ERROR_NETWORK;
			m_dwc_error_code = NETGetStartupErrorCode(ret);
			m_dwc_error_type = DWC_ETYPE_SHOW_ERROR;
			break;

		case SO_EFATAL:
		default:
			// 致命的なエラーとして扱い、ゲームの進行を停止させる
			SetError(CRank::ERROR_FATAL);
			break;
		}
		amMutexUnlock(&m_mutex);
		goto wait;
	}
	amMutexLock(&m_mutex);
	m_init_flag |= INIT_FLAG_SO_S_BIT;
	amMutexUnlock(&m_mutex);

	// 終了判定
	if (IsRequestEndThread()) {
		goto end;
	}

	// DWCライブラリ初期化
	ret = DWC_Init(
		HOG_DWC_AUTHSERVER,
		HOG_DWC_GAMENAME,
		HOG_DWC_GAMECODE,
		aoNetRankDwcMemAlloc,
		aoNetRankDwcMemFree);
	if (ret != 0) {
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_OTHER);
		m_dwc_error = DWC_ERROR_NETWORK;
		m_dwc_error_code = 0;
		m_dwc_error_type = DWC_ETYPE_SHOW_ERROR;
		amMutexUnlock(&m_mutex);
		goto wait;
	}
	amMutexLock(&m_mutex);
	m_init_flag |= INIT_FLAG_DWC_BIT;
	amMutexUnlock(&m_mutex);

	// DWCエラークリア
	DWC_ClearError();

	// 終了判定
	if (IsRequestEndThread()) {
		goto end;
	}

	// ユーザ名UTF16変換
	if (strlen(g_ao_net_rank_username) == 0) {
		// 名前が不適切
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_NAME);
		amMutexUnlock(&m_mutex);
		goto wait;
	}
	amZeroMemory(m_username, sizeof(u16) * CRank::NAME_LEN);
	dstlen = CRank::NAME_LEN;
	if (ENCConvertStringAsciiToUnicode(
		m_username, &dstlen,
		(u8*)g_ao_net_rank_username, (s32*)NULL) != ENC_OK)
	{
		// 名前が不適切
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_NAME);
		amMutexUnlock(&m_mutex);
		goto wait;
	}
	if (dstlen < CRank::NAME_LEN) {
		m_username[dstlen] = L'\0';
	}

	// ユーザデータ作成
	amMutexLock(&m_mutex);
	if (!DWC_CheckUserData(m_userdata)) {
		DWC_CreateUserData(m_userdata);
	}
	amMutexUnlock(&m_mutex);

	// フレンドマッチ初期化
	DWC_InitFriendsMatch(
		NULL,
		m_userdata,
		HOG_DWC_GSPRODUCTID,
		HOG_DWC_GAMENAME,
		HOG_DWC_GSSECRETKEY,
		0, 0, NULL, 0);

	// ここではキャンセル不可

	// 認証開始
	if (!DWC_LoginAsync(m_username, NULL, aoNetRankDwcLoginCallback, this)) {
		// エラー
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_OTHER);
		amMutexUnlock(&m_mutex);
		goto wait;
	}
	amMutexLock(&m_mutex);
	m_init_flag |= INIT_FLAG_FRD_BIT;
	amMutexUnlock(&m_mutex);

	// 認証待ち
	while (1) {

		// 認証済み判定
		amMutexLock(&m_mutex);
		BOOL is_next = m_is_login;
		amMutexUnlock(&m_mutex);
		if (is_next) {
			break;
		}

		// 強制終了判定
		if (IsRequestEndThread()) {
			goto end;
		}

		// 20ミリ秒程度スリープ
		OSSleepMilliseconds(20);
	}

	// 不正な名前チェック
	if (DWC_GetIngamesnCheckResult() == DWC_INGAMESN_INVALID) {
		// 名前が不適切
		is_name_fail = TRUE;
	}
	else if (DWC_GetIngamesnCheckResult() != DWC_INGAMESN_VALID) {
		// エラー
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_OTHER);
		amMutexUnlock(&m_mutex);
		goto wait;
	}

	// 名前チェック開始
	name_tbl[0] = m_username;
	if (!DWC_CheckProfanityExAsync(
		(const u16**)&name_tbl[0], 1, (const char*)NULL, 0,
		&cn_result, &badwordsnum, DWC_PROF_REGION_ALL))
	{
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_OTHER);
		amMutexUnlock(&m_mutex);
		goto wait;
	}

	// 名前チェック完了待ち
	while (1) {

		// 20ミリ秒程度スリープ
		OSSleepMilliseconds(20);

		// 終了判定
		if (IsRequestEndThread()) {
			DWC_CheckProfanityAbort();
			goto end;
		}

		amMutexLock(&m_mutex);
		DWCProfState prof = DWC_CheckProfanityProcess();
		amMutexUnlock(&m_mutex);

		if (prof == DWC_PROF_STATE_SUCCESS) {
			DWC_CheckProfanityAbort();
			if (cn_result == 1) {
				// 名前が不適切
				is_name_fail = TRUE;
			}
			break;
		}
		else if (prof != DWC_PROF_STATE_OPERATING) {
			DWC_CheckProfanityAbort();

			amMutexLock(&m_mutex);
			SetError(CRank::ERROR_OTHER);
			amMutexUnlock(&m_mutex);
			goto wait;
		}
	}

	// 終了判定
	if (IsRequestEndThread()) {
		goto end;
	}

	// ユーザデータセーブ必要判定
	amMutexLock(&m_mutex);
	if (DWC_CheckDirtyFlag(m_userdata)) {
		// セーブが必要
		m_is_need_userdata_save = TRUE;
	}
	amMutexUnlock(&m_mutex);

	// ユーザデータセーブ待ち
	while (1) {

		// ユーザデータセーブ済み判定
		amMutexLock(&m_mutex);
		BOOL is_next = !m_is_need_userdata_save;
		amMutexUnlock(&m_mutex);
		if (is_next) {
			break;
		}

		// 終了判定
		if (IsRequestEndThread()) {
			goto end;
		}

		// 20ミリ秒程度スリープ
		OSSleepMilliseconds(20);
	}

	// 不正な名前チェック
	if (is_name_fail) {
		// 名前が不適切
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_NAME);
		amMutexUnlock(&m_mutex);
		goto wait;
	}

	// ランキング初期化
	// 接続
	rnk_error = DWC_RnkInitialize(HOG_DWC_RANK_INITDATA, m_userdata);
	if (rnk_error != DWC_RNK_SUCCESS) {
		// エラー
		amMutexLock(&m_mutex);
		SetError(CRank::ERROR_OTHER);
		amMutexUnlock(&m_mutex);
		goto wait;
	}
	amMutexLock(&m_mutex);
	m_init_flag |= INIT_FLAG_RNK_BIT;
	amMutexUnlock(&m_mutex);

	// 接続済み設定
	amMutexLock(&m_mutex);
	m_state = CRank::STATE_CONNECTED;
	amMutexUnlock(&m_mutex);

wait:

	// 接続済み
	while (1) {

		// 終了判定
		if (IsRequestEndThread()) {
			goto end;
		}

		// 100ミリ秒程度スリープ
		OSSleepMilliseconds(100);
	}

end:

	// ランキング終了
	amMutexLock(&m_mutex);
	flag = m_init_flag;
	amMutexUnlock(&m_mutex);
	if (flag & INIT_FLAG_RNK_BIT) {
		DWC_RnkShutdown();
		amMutexLock(&m_mutex);
		m_init_flag &= ~INIT_FLAG_RNK_BIT;
		amMutexUnlock(&m_mutex);
	}

	// フレンドマッチ終了
	amMutexLock(&m_mutex);
	if (m_init_flag & INIT_FLAG_FRD_BIT) {
		DWC_ShutdownFriendsMatch();
		m_init_flag &= ~INIT_FLAG_FRD_BIT;
	}
	amMutexUnlock(&m_mutex);

	// DWCライブラリ終了
	amMutexLock(&m_mutex);
	flag = m_init_flag;
	amMutexUnlock(&m_mutex);
	if (flag & INIT_FLAG_DWC_BIT) {
		// ユーザデータのダーティーフラグクリア
		if (m_userdata) {
			if (DWC_CheckDirtyFlag(m_userdata)) {
				DWC_ClearDirtyFlag(m_userdata);
			}
		}

		DWC_Shutdown();
		amMutexLock(&m_mutex);
		m_init_flag &= ~INIT_FLAG_DWC_BIT;
		amMutexUnlock(&m_mutex);
	}

	// ソケットライブラリ終了
	amMutexLock(&m_mutex);
	flag = m_init_flag;
	amMutexUnlock(&m_mutex);
	if (flag & INIT_FLAG_SO_S_BIT) {
		ret = SOCleanup();
		if (ret < 0) {
			switch (ret) {
			case SO_EALREADY:
			case SO_EBUSY:
			case SO_ENETRESET:
				if (m_error != CRank::ERROR_NONE) {
					SetError(CRank::ERROR_OTHER);
					m_dwc_error = DWC_ERROR_NETWORK;
					m_dwc_error_code = 0;
					m_dwc_error_type = DWC_ETYPE_SHOW_ERROR;
				}
				break;
			case SO_EFATAL:
			default:
				// 致命的なエラーとして扱い、ゲームの進行を停止させる
				SetError(CRank::ERROR_FATAL);
				break;
			}
		}
		amMutexLock(&m_mutex);
		m_init_flag &= ~INIT_FLAG_SO_S_BIT;
		amMutexUnlock(&m_mutex);
	}

	// ソケットライブラリ終了
	amMutexLock(&m_mutex);
	flag = m_init_flag;
	amMutexUnlock(&m_mutex);
	if (flag & INIT_FLAG_SO_BIT) {
		ret = SOFinish();
		if (ret < 0) {
			switch (ret) {
			case SO_EAGAIN:
			case SO_EALREADY:
			case SO_EBUSY:
			case SO_EINPROGRESS:
				if (m_error != CRank::ERROR_NONE) {
					SetError(CRank::ERROR_OTHER);
					m_dwc_error = DWC_ERROR_NETWORK;
					m_dwc_error_code = 0;
					m_dwc_error_type = DWC_ETYPE_SHOW_ERROR;
				}
				break;

			default:
				// 致命的なエラーとして扱い、ゲームの進行を停止させる
				SetError(CRank::ERROR_FATAL);
				break;
			}
		}
		amMutexLock(&m_mutex);
		m_init_flag &= ~INIT_FLAG_SO_BIT;
		amMutexUnlock(&m_mutex);
	}
}


// ***************************************************************************
// エラー
// ***************************************************************************
// ===========================================================================
//! エラー設定
// ===========================================================================
void CRank::SetError(u32 error)
{
	// 操作エラーならアサート
	if (error == CRank::ERROR_OPERATION) {
		amAssert(0);
	}
	m_error = error;

	// DWCエラー取得
	if (m_init_flag & INIT_FLAG_DWC_BIT) {
		m_dwc_error = DWC_GetLastErrorEx(&m_dwc_error_code, &m_dwc_error_type);
	}

	g_ao_net_rank_error = TRUE;
}

} // namespace net
} // namespace ao


// ***************************************************************************
// SO用
// ***************************************************************************
// ===========================================================================
//	aoNetRankSoMemAlloc
/*!
	SOライブラリ用のヒープ確保関数

	@param name		[in] 無視
	@param size		[in] 確保するヒープサイズ
	@param align	[in] 確保するヒープのバイトアライン
	@return 確保したヒープ先頭のポインタ
*/
// ===========================================================================
void* aoNetRankSoMemAlloc(u32 name, s32 size)
{
	UNREFERENCED_PARAMETER(name);
	return amMemAllocHeap((u32)size, 0);
}

// ===========================================================================
//	aoNetRankSoMemFree
/*!
	SOライブラリ用のヒープ解放関数

	@param name		[in] 無視
	@param ptr		[io] 解放するヒープ領域先頭のポインタ
	@param size		[in] 無視
*/
// ===========================================================================
void aoNetRankSoMemFree(u32 name, void* ptr, s32 size)
{
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(size);
	amMemFree(ptr);
}


// ***************************************************************************
// DWC用
// ***************************************************************************
// ===========================================================================
//	aoNetRankDwcLoginCallback
/*!
	DWCログインコールバック

	@param error		[in] DWCエラー種別
	@param profileID	[in] 取得できた自分のGSプロファイルID
	@param param		[io] コールバック用パラメータ(CRank*)
*/
// ===========================================================================
void aoNetRankDwcLoginCallback(DWCError error, int profileID, void* param)
{
	((ao::net::CRank*)param)->LoginCallback(error, profileID);
}

// ===========================================================================
//	aoNetRankDwcMemAlloc
/*!
	DWCライブラリ用のヒープ確保関数

	@param name		[in] 無視
	@param size		[in] 確保するヒープサイズ
	@param align	[in] 確保するヒープのバイトアライン
	@return 確保したヒープ先頭のポインタ
*/
// ===========================================================================
void* aoNetRankDwcMemAlloc(DWCAllocType name, u32 size, int align)
{
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(align);
	return amMemAlloc(size);
}

// ===========================================================================
//	aoNetRankDwcMemAlloc
/*!
	DWCライブラリ用のヒープ解放関数

	@param name		[in] 無視
	@param ptr		[io] 解放するヒープ領域先頭のポインタ
	@param size		[in] 無視
*/
// ===========================================================================
void aoNetRankDwcMemFree(DWCAllocType name, void* ptr, u32 size)
{
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(size);
	amMemFree(ptr);
}

#endif // defined(AOD_PLATFORM_WII)

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
