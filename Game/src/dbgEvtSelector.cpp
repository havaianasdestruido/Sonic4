// ============================================================================
/*!
	@file	dbgEvtSelector.cpp
	@brief	イベントセレクタ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2007-2008 Dimps
	$Id: dbgEvtSelector.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */
/*
 *
 *	◆イベントセレクタ使用方法
 *		dbgEvtSelector.inc を参照して下さい。
 */
#if defined(AMD_DEBUG)
//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "syEvtSys.h"
#include "accelInitializer.hpp"
#include "dbgPadEmu.hpp"
#include "dbgEvtSelector.hpp"
#include "izFade.h"
#include <aoPad.h>

//インクルード設定
#define _INC_DBG_EVT_SELECTOR_INCLUDE
#include "dbgEvtSelector.inc"
#undef _INC_DBG_EVT_SELECTOR_INCLUDE


#if _WII
#pragma warn_notinlined off	//インライン展開出来無い関数に対する警告メッセージの無効化
#endif //_WII


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
// ============================================================================
// DbgEvtSelector
/*!
	イベントセレクタ・初期化

	@param	arg	[in]	引数
 */
// ============================================================================
void DbgEvtSelector(void *arg)
{
	// フェード終了
	IzFadeExit();

	new("EVT SLCT", 0x1000) dbg::CEvtSelector();
	UNREFERENCED_PARAMETER(arg);
}



//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*


namespace dbg {
//------ Static Definitions ----------- ローカル関数宣言 -------------------------******_SD*
//ローカル関数設定
#define _INC_DBG_EVT_SELECTOR_STATIC_DEFINITIONS
#include "dbgEvtSelector.inc"
#undef _INC_DBG_EVT_SELECTOR_STATIC_DEFINITIONS
//ローカルクラス設定
#define _INC_DBG_EVT_SELECTOR_STATIC_CLASSES
#include "dbgEvtSelector.inc"
#undef _INC_DBG_EVT_SELECTOR_STATIC_CLASSES


//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
const CEvtSelector::TColor CEvtSelector::c_color[EColor::Max] = {
				0xFFFFFFFF,	///<白
				0xFF0000FF,	///<赤
				0xFF00FF00,	///<緑
				0xFFFF0000,	///<青
				0xFF808080,	///<灰
				0xFF00FFFF,	///<黄
				0xFFFF00FF,	///<紫
				0xFFFFFF00,	///<水
				0xFF000000,	///<黒
				0xFF000080,	///<暗赤
				0xFF008000,	///<暗緑
				0xFF800000,	///<暗青
				0xFFCCCCCC,	///<銀
				0xFF008080,	///<暗黄
				0xFF800080,	///<暗紫
				0xFF808000,	///<暗水
			};


//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// =============================================================================
// CEvtSelector::Printv
/*!
	文字の描画

	@param	x		[in]	文字表示Ｘ座標
	@param	y		[in]	文字表示Ｙ座標
	@param	clr		[in]	文字表示色
	@param	fmt		[in]	printf書式
	@param	vlist	[in]	可変引数開始位置
 */
// ==========================================================================
void CEvtSelector::Printv(TPos x, TPos y, TColor clr, const char *fmt, va_list vlist)
{
	char buf[256];
	vsprintf(buf, fmt, vlist);

	::amPrintColor(clr);
	::amPrint(x, y, buf);
	::amPrintColor(0xFFFFFFFF);
}

// ============================================================================
// CEvtSelector::SetNextEvtArg
/*!
	次のイベントの引数を設定します

	@param	arg_size	[in]	引数サイズ
	@param	arg_buf		[in]	引数バッファ
 */
// ============================================================================
void CEvtSelector::SetNextEvtArg(TNextEvtArgSize arg_size, TNextEvtArgBuffer arg_buf)
{
	//既存のイベント引数バッファ開放
	if (NULL != m_evt_arg.data) {
		amMemFree(m_evt_arg.data);
	}

	m_evt_arg.data = amMemAlloc(arg_size);
	m_evt_arg.size = arg_size;
	memcpy(m_evt_arg.data, arg_buf, m_evt_arg.size);
}


// ============================================================================
// CEvtSelector::GetPadDirect
/*!
	パッドの直値の取得
 */
// ============================================================================
CEvtSelector::TPadInfo CEvtSelector::GetPadDirect() const
{
#if !_IPHONE
	return AoPadMDirect();
#else //!_IPHONE
	return dbg::CPadEmu::CreateInstance().GetPadDirect();
#endif  //!_IPHONE
}

CEvtSelector::TPadInfo CEvtSelector::GetPadDirect(TPadIndex index) const
{
#if !_IPHONE
	return AoPadPortMDirect(index);
#else //!_IPHONE
	return dbg::CPadEmu::CreateInstance().GetPadDirect();
	UNREFERENCED_PARAMETER(index);
#endif  //!_IPHONE
}

CEvtSelector::TPadIndex CEvtSelector::GetPadDirectIndex(TPadKey key) const
{
#if !_IPHONE
	return AoPadSomeoneMDirect(key);
#else //!_IPHONE
	return ((dbg::CPadEmu::CreateInstance().IsPadDirect(key))? TPadIndex(0): TPadIndex(-1));
#endif  //!_IPHONE
}

// ============================================================================
// CEvtSelector::GetPadStand
/*!
	パッドのONエッジの取得
 */
// ============================================================================
CEvtSelector::TPadInfo CEvtSelector::GetPadStand() const
{
#if !_IPHONE
	return AoPadMStand();
#else //!_IPHONE
	return dbg::CPadEmu::CreateInstance().GetPadStand();
#endif  //!_IPHONE
}

CEvtSelector::TPadInfo CEvtSelector::GetPadStand(TPadIndex index) const
{
#if !_IPHONE
	return AoPadPortMStand(index);
#else //!_IPHONE
	return dbg::CPadEmu::CreateInstance().GetPadStand();
	UNREFERENCED_PARAMETER(index);
#endif  //!_IPHONE
}

CEvtSelector::TPadIndex CEvtSelector::GetPadStandIndex(TPadKey key) const
{
#if !_IPHONE
	return AoPadSomeoneMStand(key);
#else //!_IPHONE
	return ((dbg::CPadEmu::CreateInstance().IsPadStand(key))? TPadIndex(0): TPadIndex(-1));
#endif  //!_IPHONE
}

// ============================================================================
// CEvtSelector::GetPadRelease
/*!
	パッドのOFFエッジの取得
 */
// ============================================================================
CEvtSelector::TPadInfo CEvtSelector::GetPadRelease() const
{
#if !_IPHONE
	return AoPadMRelease();
#else //!_IPHONE
	return dbg::CPadEmu::CreateInstance().GetPadRelease();
#endif  //!_IPHONE
}

CEvtSelector::TPadInfo CEvtSelector::GetPadRelease(TPadIndex index) const
{
#if !_IPHONE
	return AoPadPortMRelease(index);
#else //!_IPHONE
	return dbg::CPadEmu::CreateInstance().GetPadRelease();
	UNREFERENCED_PARAMETER(index);
#endif  //!_IPHONE
}

CEvtSelector::TPadIndex CEvtSelector::GetPadReleaseIndex(TPadKey key) const
{
#if !_IPHONE
	return AoPadSomeoneMRelease(key);
#else //!_IPHONE
	return ((dbg::CPadEmu::CreateInstance().IsPadRelease(key))? TPadIndex(0): TPadIndex(-1));
#endif  //!_IPHONE
}

// ============================================================================
// CEvtSelector::GetPadRepeat
/*!
	パッドのリピートの取得
 */
// ============================================================================
CEvtSelector::TPadInfo CEvtSelector::GetPadRepeat() const
{
#if !_IPHONE
	return AoPadMRepeat();
#else //!_IPHONE
	return dbg::CPadEmu::CreateInstance().GetPadRepeat();
#endif  //!_IPHONE
}

CEvtSelector::TPadInfo CEvtSelector::GetPadRepeat(TPadIndex index) const
{
#if !_IPHONE
	return AoPadPortMRepeat(index);
#else //!_IPHONE
	return dbg::CPadEmu::CreateInstance().GetPadRepeat();
	UNREFERENCED_PARAMETER(index);
#endif  //!_IPHONE
}

CEvtSelector::TPadIndex CEvtSelector::GetPadRepeatIndex(TPadKey key) const
{
#if !_IPHONE
	return AoPadSomeoneMRepeat(key);
#else //!_IPHONE
	return ((dbg::CPadEmu::CreateInstance().IsPadRepeat(key))? TPadIndex(0): TPadIndex(-1));
#endif  //!_IPHONE
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// =============================================================================
// CEvtSelector::CEvtSelector
/*!
	デフォルトコンストラクタ
 */
// ==========================================================================
CEvtSelector::CEvtSelector()
{
	//状態表示
	m_use_fade = true;
	m_slct = 0;
	m_direct_start = TMapEvtIdx::size_type(-1);

	//オプションの設定
	#define _INC_DBG_EVT_SELECTOR_OPTION
	#include "dbgEvtSelector.inc"
	#undef _INC_DBG_EVT_SELECTOR_OPTION

	//メニューの設定
	#define _INC_DBG_EVT_SELECTOR_ADD_MENU
	#include "dbgEvtSelector.inc"
	#undef _INC_DBG_EVT_SELECTOR_ADD_MENU

	//実初期化
	protectInit();
}

// =============================================================================
// CEvtSelector::CEvtSelector
/*!
	デストラクタ
 */
// ==========================================================================
CEvtSelector::~CEvtSelector()
{
	//負荷表示終了
	er::dbg::CBusyVisualizerTask &visualizer = er::dbg::CBusyVisualizerTask::CreateInstance();
	visualizer.Release();

	//コールバック呼び出し(後処理)
	for (TMapEvtIdx::iterator ite = m_evt_idx.begin(), ite_end = m_evt_idx.end(); ite != ite_end; ++ite) {
		delete *ite;
	}
	m_evt_idx.clear();

	//遷移
	if (NULL != m_evt_arg.data) {
		::SyChangeNextEvtArg(m_evt_arg.size, m_evt_arg.data);
		//イベント引数バッファ開放
		amMemFree(m_evt_arg.data);
	} else {
		::SyChangeNextEvt();
	}

}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// =============================================================================
// CEvtSelector::protectInit
/*!
	初期化
 */
// ==========================================================================
void CEvtSelector::protectInit()
{
	//経過時間の計測開始
	getElapsedTime();

	//VRAMクリア
	clearVram();
	amDrawMakeTask(drawTaskClearVram, 0, this);

	//引数の初期化
	m_evt_arg.size = 0;
	m_evt_arg.data = NULL;

	//マスター登録
	for (TMapEvtIdx::iterator ite = m_evt_idx.begin(), ite_end = m_evt_idx.end(); ite != ite_end; ++ite) {
		(*ite)->SetMaster(*this);
	}

	//即時起動設定
	if (m_direct_start < m_evt_idx.size()) {
		m_slct = m_direct_start;

		//コールバック呼び出し(オンフォーカス・決定・オンブラー)
		m_evt_idx[m_slct]->OnFocus();
		m_evt_idx[m_slct]->Enter();
		::SyDecideEvt(static_cast<s16>(m_evt_idx[m_slct]->GetEvtId()));
		m_evt_idx[m_slct]->OnBlur();

		delete this;
		return;
	}

	//初期位置確認
	if (m_evt_idx.size() <= m_slct) {
		m_slct = 0;
	}

	//フェード用シェイプ
	m_fade_shape.Create();
	for (Uint32 i = 0; i < TFadeShape::NSize; ++i) {
		TFadeShape::TVertex vertex;
		vertex.Col = 0x000000FF;
		switch (i) {
		case 1:
			vertex.Pos.x = 0;
			vertex.Pos.y = AMD_SCREEN_HEIGHT;
			break;
		case 2:
			vertex.Pos.x = AMD_SCREEN_WIDTH;
			vertex.Pos.y = 0;
			break;
		case 3:
			vertex.Pos.x = AMD_SCREEN_WIDTH;
			vertex.Pos.y = AMD_SCREEN_HEIGHT;
			break;
		case 0:
		default:
			vertex.Pos.x = 0;
			vertex.Pos.y = 0;
			break;
		}
		m_fade_shape.SetVertex(i, vertex);
	}
	m_fade_shape.SetAlpha(1.0f);
	m_fade_shape.Draw();

	//負荷表示開始
	er::dbg::CBusyVisualizerTask &visualizer = er::dbg::CBusyVisualizerTask::CreateInstance();
#if !_WII
	visualizer.Create();
#if _IPHONE
	visualizer.SetScale(3.0f);
#endif //_IPHONE
#endif //!_WII



#if _IPHONE
	//パッドトリガ設定
	dbg::CPadEmu &pad_emu = dbg::CPadEmu::CreateInstance();
	pad_emu.AttachTask("PadEmulator", 0);
	pad_emu.Create(dbg::CPadEmu::EMode::Drag);
	m_pad_emu_drag_mode = true;
#endif //_IPHONE

	//プロシージャ設定
	SetProc(&CEvtSelector::fadeIn);
}

// =============================================================================
// CEvtSelector::clearVram
/*!
	VRAM初期化
 */
// ==========================================================================
void CEvtSelector::clearVram()
{
	if (!amThreadCheckDraw()) {
		//更新タスク
	} else {
		//描画タスク
		//背面設定
		NNS_RGBA_U8 color = accel::Initializer<NNS_RGBA_U8, Uint8, Uint8, Uint8, Uint8>(40, 96, 128, 0xFF);
		amDrawSetBGColor(&color);
	}
}

// =============================================================================
// CEvtSelector::fadeIn
/*!
	フェードイン
 */
// ==========================================================================
void CEvtSelector::fadeIn()
{
	TCount count = GetCount() + 1;
	dispTitle();
	dispList(EColor::White, EColor::White);

	if (m_use_fade && (count <= 16)) {
		Float32 alpha = 1.0f - (Float32(count) * (1.0f / 16.0f));
		m_fade_shape.SetAlpha(alpha);
		m_fade_shape.Draw();
	} else {
		//コールバック呼び出し(オンフォーカス)
		m_evt_idx[m_slct]->OnFocus();

		//遷移
		SetProc(&CEvtSelector::slctEvt);
	}
}

// =============================================================================
// CEvtSelector::slctEvt
/*!
	選択
 */
// ==========================================================================
void CEvtSelector::slctEvt()
{
	dispTitle();

	//カーソル移動
	{
		TMoveDirect				move = m_evt_idx[m_slct]->MoveEvent();
		TMapEvtIdx::size_type	old_slct = m_slct;
		TMapEvtIdx::size_type	num = m_evt_idx.size();

		if (0 != move) {
			if (move < 0) {
				//上
				if (0 < m_slct) {
					--m_slct;
				} else {
					m_slct = num - 1;
				}
			} else if (0 < move) {
				//下
				if (m_slct < num - 1) {
					++m_slct;
				} else {
					m_slct = 0;
				}
			}

			//カーソル移動確認
			if (old_slct != m_slct) {
				//コールバック呼び出し(オンブラー)
				m_evt_idx[old_slct]->OnBlur();
				//コールバック呼び出し(オンフォーカス)
				m_evt_idx[m_slct]->OnFocus();
			}
		}
	}

	//リスト表示
	dispList(EColor::Red, EColor::White);

	//コールバック呼び出し(フォーカス・ブラー)
	for (TMapEvtIdx::iterator ite = m_evt_idx.begin(), ite_end = m_evt_idx.begin() + s32(m_slct); ite != ite_end; ++ite) {
		(*ite)->Blur();
	}
	m_evt_idx[m_slct]->Focus();
	for (TMapEvtIdx::iterator ite = m_evt_idx.begin() + s32(m_slct) + 1, ite_end = m_evt_idx.end(); ite != ite_end; ++ite) {
		(*ite)->Blur();
	}

	//決定
	if (m_evt_idx[m_slct]->IsEnter()) {
		//イベントID確認
		if (GSD_EVT_ID_NOP != m_evt_idx[m_slct]->GetEvtId()) {
			//有効イベントIDなら決定
			::SyDecideEvt(static_cast<s16>(m_evt_idx[m_slct]->GetEvtId()));
			//コールバック呼び出し(決定・オンブラー)
			m_evt_idx[m_slct]->Enter();
			m_evt_idx[m_slct]->OnBlur();

			//プロシージャ設定
			SetProc(&CEvtSelector::fadeOut);
		}
	}

#if _IPHONE
	//パッドトリガのモード切替設定
	dbg::CPadEmu &pad_emu = dbg::CPadEmu::CreateInstance();
	if (pad_emu[dbg::CPadEmu::ETrgPad::Free1][er::CTrgState::EState::Down]) {
		pad_emu.Create(((m_pad_emu_drag_mode)? dbg::CPadEmu::EMode::Tap: dbg::CPadEmu::EMode::Drag));
		m_pad_emu_drag_mode = !m_pad_emu_drag_mode;
	}
#endif //_IPHONE
}

// =============================================================================
// CEvtSelector::fadeOut
/*!
	フェードアウト
 */
// ==========================================================================
void CEvtSelector::fadeOut()
{
	TCount count = GetCount() + 1;
	dispTitle();
	dispList(EColor::Red, EColor::Gray);

	if (m_evt_idx[m_slct]->IsWhiteFade()) {
		for (Uint32 i = 0; i < TFadeShape::NSize; ++i) {
			TFadeShape::TVertex vertex = m_fade_shape.GetVertex(i);
			vertex.Col = 0xFFFFFFFF;
			m_fade_shape.SetVertex(i, vertex);
		}
	}

	if (m_use_fade && (count <= 16)) {
		Float32 alpha = Float32(count) * (1.0f / 16.0f);
		m_fade_shape.SetAlpha(alpha);
		m_fade_shape.Draw();
	} else {
		//プロシージャ設定
		delete this;
	}
}

// =============================================================================
// CEvtSelector::dispTitle
/*!
	タイトル描画
 */
// ==========================================================================
void CEvtSelector::dispTitle()
{
	{
		CEvtSelector &cb = *this;
#define _INC_DBG_EVT_SELECTOR_ROM_STATUS
#include "dbgEvtSelector.inc"
#undef _INC_DBG_EVT_SELECTOR_ROM_STATUS
	}
}

// =============================================================================
// CEvtSelector::dispList
/*!
	リストの描画
 */
// ==========================================================================
void CEvtSelector::dispList(TColor crnt, TColor back)
{
	u32 num = m_evt_idx.size();

	//表示
	Print(8, 10, "%03d: %s", m_slct, m_evt_idx[m_slct]->GetTitle());
	Print(2, 11, "|------------------------------|");
	if (1 < num) {
		Print(int(3 + (m_slct * (30-1) / (num-1))), 11, "*");
	} else {
		Print(3, 11, "*");
	}

	UNREFERENCED_PARAMETER(crnt);
	UNREFERENCED_PARAMETER(back);
}

// ============================================================================
// CEvtSelector::drawTaskClearVram
/*!
	VRAMの初期化(描画タスク側)

	@param	evt	[in]	イベントセレクタクラス
 */
// ============================================================================
void CEvtSelector::drawTaskClearVram(AMS_TCB *tcb)
{
	CEvtSelector &it = *reinterpret_cast<CEvtSelector *>(amTaskGetWork(tcb));
	it.clearVram();
}

// ============================================================================
// CEvtSelector::getElapsedTime
/*!
	経過時間を返します

	@return 経過時間

	@note
		開始は初めて CEvtSelector が起動した時に成ります。
 */
// ============================================================================
const ::AMS_TIMER &CEvtSelector::getElapsedTime()
{
	static ::AMS_TIMER *initialized = NULL;
	static ::AMS_TIMER timer;

	if (!initialized) {
		::amTimerCreate(&timer);
		::amTimerStart(&timer);
		initialized = &timer;
	}

	return timer;
}

//------------------------------------------------------------------------------**********





































































//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// CEvtBase::IsEnter
/*!
	決定を判定する関数

	@retval	TRUE	決定	
	@retval	FALSE	未決定

	@note
		デフォルトでは Start ボタンと A ボタンが決定操作になります
 */
// ============================================================================
bool CEvtBase::IsEnter()
{
	if (IsPadStand(TPadKey(GSD_KEY_DECIDE | KEY_START))) {
		return true;
	}
	return false;
}

// ============================================================================
// CEvtBase::MoveEvent
/*!
	イベント間の移動を判定する関数

	@retval	0		移動しない
	@retval	1～		次のイベントへ
	@retval	～-1	前のイベントへ

	@note
		デフォルトでは ←・→ が移動操作になります
 */
// ============================================================================
CEvtBase::TMoveDirect CEvtBase::MoveEvent()
{
	if (IsPadRepeat(KEY_L_RIGHT)) {
		return TMoveDirect(+1);
	} else if (IsPadRepeat(KEY_L_LEFT)) {
		return TMoveDirect(-1);
	}
	return TMoveDirect(0);
}

// ============================================================================
// CEvtBase::Printv
/*!
	文字を表示します

	@param	x		[in]	文字表示Ｘ座標
	@param	y		[in]	文字表示Ｙ座標
	@param	clr		[in]	文字表示色
	@param	fmt		[in]	printf書式
	@param	vlist	[in]	可変引数開始位置

	@note
		座標は上下一体で扱います
		(0,0) は上画面の左上、 (0,24) は下画面の左上として扱われます
		ただし、上画面にて下方向に溢れた時に下画面には回りこみません
 */
// ============================================================================
void CEvtBase::Printv(TPos x, TPos y, TColor clr, const char *fmt, va_list vlist)
{
	if (NULL != m_master) {
		m_master->Printv(x, y, clr, fmt, vlist);
	}
}

// ============================================================================
// CEvtBase::SetNextEvtArg
/*!
	次のイベントの引数を設定します

	@param	arg_size	[in]	引数サイズ
	@param	arg_buf		[in]	引数バッファ
 */
// ============================================================================
void CEvtBase::SetNextEvtArg(TNextEvtArgSize arg_size, TNextEvtArgBuffer arg_buf)
{
	if (NULL != m_master) {
		m_master->SetNextEvtArg(arg_size, arg_buf);
	}
}

// ============================================================================
// CEvtBase::GetPadDirect
/*!
	パッドの直値の取得
 */
// ============================================================================
CEvtBase::TPadInfo CEvtBase::GetPadDirect() const
{
	return ((NULL != m_master)? m_master->GetPadDirect(): AoPadMDirect());
}

CEvtBase::TPadInfo CEvtBase::GetPadDirect(TPadIndex index) const
{
	return ((NULL != m_master)? m_master->GetPadDirect(index): AoPadPortMDirect(index));
}

CEvtBase::TPadIndex CEvtBase::GetPadDirectIndex(TPadKey key) const
{
	return ((NULL != m_master)? m_master->GetPadDirectIndex(key): AoPadSomeoneMDirect(key));
}

bool CEvtBase::IsPadDirect(TPadKey key) const
{
	bool result;
	if (NULL != m_master) {
		result = m_master->IsPadDirect(key);
	} else {
		result = (TPadIndex(0) <= GetPadDirectIndex(key)? true: false);
	}
	return result;
}

// ============================================================================
// CEvtBase::GetPadStand
/*!
	パッドのONエッジの取得
 */
// ============================================================================
CEvtBase::TPadInfo CEvtBase::GetPadStand() const
{
	return ((NULL != m_master)? m_master->GetPadStand(): AoPadMStand());
}

CEvtBase::TPadInfo CEvtBase::GetPadStand(TPadIndex index) const
{
	return ((NULL != m_master)? m_master->GetPadStand(index): AoPadPortMStand(index));
}

CEvtBase::TPadIndex CEvtBase::GetPadStandIndex(TPadKey key) const
{
	return ((NULL != m_master)? m_master->GetPadStandIndex(key): AoPadSomeoneMStand(key));
}

bool CEvtBase::IsPadStand(TPadKey key) const
{
	bool result;
	if (NULL != m_master) {
		result = m_master->IsPadStand(key);
	} else {
		result = (TPadIndex(0) <= GetPadStandIndex(key)? true: false);
	}
	return result;
}

// ============================================================================
// CEvtBase::GetPadRelease
/*!
	パッドのOFFエッジの取得
 */
// ============================================================================
CEvtBase::TPadInfo CEvtBase::GetPadRelease() const
{
	return ((NULL != m_master)? m_master->GetPadRelease(): AoPadMRelease());
}

CEvtBase::TPadInfo CEvtBase::GetPadRelease(TPadIndex index) const
{
	return ((NULL != m_master)? m_master->GetPadRelease(index): AoPadPortMRelease(index));
}

CEvtBase::TPadIndex CEvtBase::GetPadReleaseIndex(TPadKey key) const
{
	return ((NULL != m_master)? m_master->GetPadReleaseIndex(key): AoPadSomeoneMRelease(key));
}

bool CEvtBase::IsPadRelease(TPadKey key) const
{
	bool result;
	if (NULL != m_master) {
		result = m_master->IsPadRelease(key);
	} else {
		result = (TPadIndex(0) <= GetPadReleaseIndex(key)? true: false);
	}
	return result;
}

// ============================================================================
// CEvtBase::GetPadRepeat
/*!
	パッドのリピートの取得
 */
// ============================================================================
CEvtBase::TPadInfo CEvtBase::GetPadRepeat() const
{
	return ((NULL != m_master)? m_master->GetPadRepeat(): AoPadMRepeat());
}

CEvtBase::TPadInfo CEvtBase::GetPadRepeat(TPadIndex index) const
{
	return ((NULL != m_master)? m_master->GetPadRepeat(index): AoPadPortMRepeat(index));
}

CEvtBase::TPadIndex CEvtBase::GetPadRepeatIndex(TPadKey key) const
{
	return ((NULL != m_master)? m_master->GetPadRepeatIndex(key): AoPadSomeoneMRepeat(key));
}

bool CEvtBase::IsPadRepeat(TPadKey key) const
{
	bool result;
	if (NULL != m_master) {
		result = m_master->IsPadRepeat(key);
	} else {
		result = (TPadIndex(0) <= GetPadRepeatIndex(key)? true: false);
	}
	return result;
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********





































































//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********





































































//------ Static Functions ------------- ローカル関数定義 -------------------------******_SF*
#define _INC_DBG_EVT_SELECTOR_STATIC_FUNCTIONS
#include "dbgEvtSelector.inc"
#undef _INC_DBG_EVT_SELECTOR_STATIC_FUNCTIONS


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
