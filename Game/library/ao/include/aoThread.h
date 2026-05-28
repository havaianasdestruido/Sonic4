// ===========================================================================
/*!
	@file	aoThread.h
	@brief	AoLibrary スレッドクラス宣言

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================
#pragma once

namespace ao {

// ----- Include Files ---------------------------------------（インクルード）
// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

// ***************************************************************************
// スレッドクラス
// ***************************************************************************
// ===========================================================================
//	class CThread
// ---------------------------------------------------------------------------
//!	スレッドクラス
// ===========================================================================
template <class T>
class CThread
{
public:

	enum {
		// デフォルトスレッドスタックサイズ
#if defined(AOD_PLATFORM_WII)
		DEF_STACK_SIZE = 0x1000,
#else
		DEF_STACK_SIZE = 0x4000,
#endif // defined(AOD_PLATFORM_WII)
	};

protected:

	// =======================================================================
	//	TypeThreadProcedure
	// -----------------------------------------------------------------------
	//!	スレッドプロシージャ型
	// =======================================================================
	typedef void (T::*TypeThreadProcedure)();

	// =======================================================================
	//	CThread
	/*!
		コンストラクタ
	*/
	// =======================================================================
	CThread();

	// =======================================================================
	//	~CThread
	/*!
		デストラクタ
	*/
	// =======================================================================
	virtual ~CThread();

	// =======================================================================
	//	SetThreadProcNone
	/*!
		無効スレッドプロシージャ設定

		@param no			[in] スレッド番号(常に0)
	*/
	// =======================================================================
	void SetThreadProcNone(u32 no);

	// =======================================================================
	//	SetThreadProc
	/*!
		スレッドプロシージャ設定

		@param no			[in] スレッド番号(常に0)
		@param proc			[in] スレッドプロシージャ
	*/
	// =======================================================================
	void SetThreadProc(u32 no, TypeThreadProcedure proc);

	// =======================================================================
	//	GetThreadProc
	/*!
		スレッド関数取得

		@param no			[in] スレッド番号(常に0)
	*/
	// =======================================================================
	TypeThreadProcedure GetThreadProc(u32 no);

	// =======================================================================
	//	StartThread
	/*!
		スレッド開始

		@param no			[in] スレッド番号(常に0)
		@param core			[in] コア
		@param prio			[in] スレッド優先度
		@param stack_size	[in] スレッドスタックサイズ
	*/
	// =======================================================================
	void StartThread(
		u32 no, AMD_CORE core, u32 prio, u32 stack_size = DEF_STACK_SIZE);

	// =======================================================================
	//	NoticeEndThread
	/*!
		スレッド終了要求発行(親スレッド呼び出し)

		@param no			[in] スレッド番号(常に0)
		@note
		amThreadExitを呼び出します。\n
	*/
	// =======================================================================
	void RequestEndThread(u32 no);

	// =======================================================================
	//	IsEndThread
	/*!
		スレッド終了判定(親スレッド呼び出し)

		@param no			[in] スレッド番号(常に0)
		@return 真：終了済み　偽：それ以外
		@note
		amThreadCheckQuitを呼び出します。\n
	*/
	// =======================================================================
	BOOL IsEndThread(u32 no);

	// =======================================================================
	//	WaitEndThread
	/*!
		スレッド終了待ち(親スレッド呼び出し)

		@param no			[in] スレッド番号(常に0)
		@note
		amThreadWaitQuitを呼び出します。\n
	*/
	// =======================================================================
	void WaitEndThread(u32 no);

	// =======================================================================
	//	IsRequestEndThread
	/*!
		スレッド終了要求判定(子スレッド呼び出し)

		@note
		amThreadCheckExitを呼び出します。\n
	*/
	// =======================================================================
	BOOL IsRequestEndThread();

private:

	// =======================================================================
	//	CallThreadProcedure
	/*!
		スレッド関数呼び出し

		@param no	[in] スレッド番号(常に0)
	*/
	// =======================================================================
	void CallThreadProcedure(u32 no);

	// =======================================================================
	//	threadFunc
	/*!
		スレッド関数

		@param arg	[io] スレッド引数
	*/
	// =======================================================================
#if _PC | _XBOX
	static DWORD WINAPI threadFunc(DWORD arg);
#elif _PS3
	static void threadFunc(uint64_t arg);
#elif _WII
	static void* threadFunc(void* arg);
#endif

	// =======================================================================
	//	TypeThreadProcedure m_proc
	// -----------------------------------------------------------------------
	//!	スレッドプロシージャ
	// =======================================================================
	TypeThreadProcedure m_proc;

	// =======================================================================
	//	AMS_THREAD m_thread
	// -----------------------------------------------------------------------
	//!	スレッド
	// =======================================================================
	AMS_THREAD m_thread;

	// =======================================================================
	//	AMS_THREAD_ID m_thread_id
	// -----------------------------------------------------------------------
	//!	スレッドID
	// =======================================================================
	AMS_THREAD_ID m_thread_id;

	// =======================================================================
	//	BOOL m_is_execute
	// -----------------------------------------------------------------------
	//!	真：スレッド動作中　偽：スレッド無効
	// =======================================================================
	BOOL m_is_execute;

	// =======================================================================
	//	BOOL m_is_check_exit
	// -----------------------------------------------------------------------
	//!	真：スレッド終了要求あり　偽：スレッド終了要求なし
	// =======================================================================
	BOOL m_is_check_exit;
};

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ***************************************************************************
// スレッドクラス
// ***************************************************************************
// ===========================================================================
//	CThread
/*!
	コンストラクタ
*/
// ===========================================================================
template <class T>
CThread<T>::CThread()
{
	m_proc = NULL;
	m_is_execute = FALSE;
	m_is_check_exit = FALSE;
}

// ===========================================================================
//	~CThread
/*!
	デストラクタ
*/
// ===========================================================================
template <class T>
CThread<T>::~CThread()
{
	if (m_is_execute) {
		amThreadExit(&m_thread);
		amThreadWaitQuit(&m_thread);
		amThreadDelete(&m_thread);
		m_is_execute = FALSE;
		m_is_check_exit = FALSE;
	}
}

// ===========================================================================
//	SetThreadProcNone
/*!
	無効スレッドプロシージャ設定

	@param no			[in] スレッド番号(常に0)
*/
// ===========================================================================
template <class T>
void CThread<T>::SetThreadProcNone(u32 no)
{
	UNREFERENCED_PARAMETER(no);
	amAssert(no == 0);
	m_proc = NULL;
}

// ===========================================================================
//	SetThreadProc
/*!
	スレッドプロシージャ設定

	@param no			[in] スレッド番号(常に0)
	@param proc			[in] スレッドプロシージャ
*/
// ===========================================================================
template <class T>
void CThread<T>::SetThreadProc(u32 no, TypeThreadProcedure proc)
{
	UNREFERENCED_PARAMETER(no);
	amAssert(no == 0);
	m_proc = proc;
}

// ===========================================================================
//	GetThreadProc
/*!
	スレッド関数取得

	@param no			[in] スレッド番号(常に0)
*/
// ===========================================================================
template <class T>
typename CThread<T>::TypeThreadProcedure CThread<T>::GetThreadProc(u32 no)
{
	UNREFERENCED_PARAMETER(no);
	amAssert(no == 0);
	return m_proc;
}

// ===========================================================================
//	StartThread
/*!
	スレッド開始

	@param no			[in] スレッド番号(常に0)
	@param core			[in] コア
	@param prio			[in] スレッド優先度
	@param stack_size	[in] スレッドスタックサイズ
*/
// ===========================================================================
template <class T>
void CThread<T>::StartThread(u32 no, AMD_CORE core, u32 prio, u32 stack_size)
{
	UNREFERENCED_PARAMETER(no);
	amAssert(no == 0);

	if (m_is_execute) {
		amThreadExit(&m_thread);
		amThreadWaitQuit(&m_thread);
		amThreadDelete(&m_thread);
		m_is_execute = FALSE;
		m_is_check_exit = FALSE;
	}

	m_is_check_exit = FALSE;
	m_thread_id = amThreadCreate(
		&m_thread, (void*)(&CThread<T>::threadFunc), this,
		core, (s32)prio, stack_size);
	m_is_execute = TRUE;
}

// ===========================================================================
//	NoticeEndThread
/*!
	スレッド終了要求発行(親スレッド呼び出し)

	@param no			[in] スレッド番号(常に0)
	@note
	amThreadExitを呼び出します。\n
*/
// ===========================================================================
template <class T>
void CThread<T>::RequestEndThread(u32 no)
{
	UNREFERENCED_PARAMETER(no);
	amAssert(no == 0);
	if (m_is_execute) {
		amThreadExit(&m_thread);
	}
}

// ===========================================================================
//	IsEndThread
/*!
	スレッド終了判定(親スレッド呼び出し)

	@param no			[in] スレッド番号(常に0)
	@return 真：終了済み　偽：それ以外
	@note
	amThreadCheckQuitを呼び出します。\n
*/
// ===========================================================================
template <class T>
BOOL CThread<T>::IsEndThread(u32 no)
{
	UNREFERENCED_PARAMETER(no);
	amAssert(no == 0);
	if (m_is_execute) {
		if (amThreadCheckQuit(&m_thread)) {
			amThreadDelete(&m_thread);
			m_is_execute = FALSE;
			m_is_check_exit = FALSE;
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

// ===========================================================================
//	WaitEndThread
/*!
	スレッド終了待ち(親スレッド呼び出し)

	@param no			[in] スレッド番号(常に0)
	@note
	amThreadWaitQuitを呼び出します。\n
*/
// ===========================================================================
template <class T>
void CThread<T>::WaitEndThread(u32 no)
{
	UNREFERENCED_PARAMETER(no);
	amAssert(no == 0);
	if (m_is_execute) {
		amThreadWaitQuit(&m_thread);
		amThreadDelete(&m_thread);
		m_is_execute = FALSE;
		m_is_check_exit = FALSE;
	}
}

// ===========================================================================
//	IsRequestEndThread
/*!
	スレッド終了要求判定(子スレッド呼び出し)

	@note
	amThreadCheckExitを呼び出します。\n
*/
// ===========================================================================
template <class T>
BOOL CThread<T>::IsRequestEndThread()
{
	amAssert(m_is_execute);
	if (m_is_check_exit || amThreadCheckExit(&m_thread)) {
		m_is_check_exit = TRUE;
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//	CallThreadProcedure
/*!
	スレッド関数呼び出し

	@param no	[in] スレッド番号(常に0)
*/
// ===========================================================================
template <class T>
void CThread<T>::CallThreadProcedure(u32 no)
{
	UNREFERENCED_PARAMETER(no);
	amAssert(no == 0);

	// 開始
	amThreadOpen(&m_thread);

	// スレッド関数呼び出し
	if (m_proc) {
		(((T*)this)->*m_proc)();
	}

	// 終了
	amThreadQuit(&m_thread);
}

// ===========================================================================
//	threadFunc
/*!
	スレッド関数

	@param arg	[io] スレッド引数
*/
// ===========================================================================
#if _PC | _XBOX
template <class T>
DWORD WINAPI CThread<T>::threadFunc(DWORD arg)
{
	// 引数取得
	AMS_THREAD* th = (AMS_THREAD*)arg;

	// スレッドクラス取得
	CThread<T>* base = (CThread<T>*)(th->arg);

	// 呼び出し
	base->CallThreadProcedure(0);

	return 0;
}
#elif _PS3
template <class T>
void CThread<T>::threadFunc(uint64_t arg)
{
	// 引数取得
	AMS_THREAD* th = (AMS_THREAD*)arg;

	// スレッドクラス取得
	CThread<T>* base = (CThread<T>*)(th->arg);

	// 呼び出し
	base->CallThreadProcedure(0);
}
#elif _WII
template <class T>
void* CThread<T>::threadFunc(void* arg)
{
	// 引数取得
	AMS_THREAD* th = (AMS_THREAD*)arg;

	// スレッドクラス取得
	CThread<T>* base = (CThread<T>*)(th->arg);

	// 呼び出し
	base->CallThreadProcedure(0);

	return 0;
}
#endif

// ----- Static Functions --------------------（スタティック関数の定義：局所）

} // namespace ao

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
