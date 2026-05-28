// ============================================================================
/*!
	@file	dbgEvtSelector.hpp
	@brief	イベントセレクタ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2007-2008 Dimps
	$Id: dbgEvtSelector.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page dbgEvtSelectorMain イベントセレクタ

	@section dbgEvtSelectorSummary 概要
		イベントの選択機構を提供します。
 */

#pragma once
#if	defined(__cplusplus)
extern "C" {
#endif

//------ C Include Files -------------- インクルード ---------------------------******CIF*
//------ C Macro ---------------------- マクロ ---------------------------------******CMC*
//------ C External Definitions ------- グローバル変数及び関数の宣言 -----------******CED*
// ============================================================================
// DbgEvtSelector
/*!
	イベントセレクタ・初期化

	@param	arg	[in]	引数
 */
// ============================================================================
extern void DbgEvtSelector(void *arg);


#if	defined(__cplusplus)
} // extern "C"
#endif

#if	defined(__cplusplus)
//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "gsMainSys.h"

#include "accelArray.hpp"
#include "erTask.hpp"
#include "erTrg.hpp"
#include "erShape.hpp"
#include "erDbgBusyVisualizer.hpp"
#include <deque>


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace dbg {


#if _WII
#pragma warn_notinlined off	//インライン展開出来無い関数に対する警告メッセージの無効化
#endif //_WII









//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	イベントセレクタクラス
		イベントセレクタを提供します。
 */
class CEvtBase;
class CEvtSelector : public er::task::CTask<CEvtSelector> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef	er::task::CTask<CEvtSelector>	TSuperType;


	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	typedef int				TPos;				//<位置
	typedef unsigned long	TColor;				//<色
	typedef int				TFlag;				//<フラグ
	typedef unsigned long	TNextEvtArgSize;	//<次のイベントの引数サイズ
	typedef void *			TNextEvtArgBuffer;	//<次のイベントの引数
	typedef int				TMoveDirect;		//<移動方向
	typedef Uint16			TPadInfo;			//<パッド情報
	typedef Uint16			TPadKey;			//<パッドキー
	typedef int				TPadIndex;			//<パッドインデックス

	//色列挙
	struct EColor {
		enum Type {
			White,			///<白
			Red,			///<赤
			Green,			///<緑
			Blue,			///<青
			Gray,			///<灰
			Yellow,			///<黄
			Magenta,		///<紫
			Cyan,			///<水
			Black,			///<黒
			DarkRed,		///<暗赤
			DarkGreen,		///<暗緑
			DarkBlue,		///<暗青
			Silver,			///<銀
			DarkYellow,		///<暗黄
			DarkMagenta,	///<暗紫
			DarkCyan,		///<暗水

			Max,
			None,
		};
	};

	//色
	static const TColor c_color[EColor::Max];


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CEvtSelector::Printv
	/*!
		文字の描画

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
	// ==========================================================================
	void Printv(TPos x, TPos y, TColor clr, const char *fmt, va_list vlist);

	// ============================================================================
	// CEvtSelector::Printc
	/*!
		文字の描画

		@param	x		[in]	文字表示Ｘ座標
		@param	y		[in]	文字表示Ｙ座標
		@param	clr		[in]	文字表示色
		@param	fmt		[in]	printf書式
		@param	vlist	[in]	printf引数

		@note
			Printv参照
	 */
	// ============================================================================
	void Printc(TPos x, TPos y, TColor clr, const char *fmt, ...) {
		va_list	vlist;
		va_start(vlist, fmt);
		Printv(x, y, clr, fmt, vlist);
		va_end(vlist);
	}
	void Printc(TPos x, TPos y, EColor::Type clr, const char *fmt, ...) {
		va_list	vlist;
		va_start(vlist, fmt);
		Printv(x, y, c_color[clr], fmt, vlist);
		va_end(vlist);
	}

	// ============================================================================
	// CEvtSelector::Print
	/*!
		文字の描画

		@param	x		[in]	文字表示Ｘ座標
		@param	y		[in]	文字表示Ｙ座標
		@param	clr		[in]	文字表示色
		@param	fmt		[in]	printf書式
		@param	vlist	[in]	printf引数

		@note
			Printv参照
	 */
	// ============================================================================
	void Print(TPos x, TPos y, const char *fmt, ...) {
		va_list	vlist;
		va_start(vlist, fmt);
		Printv(x, y, c_color[EColor::White], fmt, vlist);
		va_end(vlist);
	}

	// ============================================================================
	// CEvtSelector::SetNextEvtArg
	/*!
		次のイベントの引数を設定します

		@param	arg_size	[in]	引数サイズ
		@param	arg_buf		[in]	引数バッファ
	 */
	// ============================================================================
	void SetNextEvtArg(TNextEvtArgSize arg_size, TNextEvtArgBuffer arg_buf);
	void SetNextEvtArg() {
		SetNextEvtArg(0, NULL);
	}

	// ============================================================================
	// CEvtSelector::SetNextEvtArg
	/*!
		次のイベントの引数を設定します

		@param	arg			[in]	引数
	 */
	// ============================================================================
	template <typename T>
	void SetNextEvtArg(const T &arg) {
		SetNextEvtArg(sizeof(arg), &arg);
	}

	// ============================================================================
	// CEvtSelector::GetPadDirect
	/*!
		パッドの直値の取得

		@param	index	パッドインデックス

		@return	パッド情報
	 */
	// ============================================================================
	TPadInfo GetPadDirect() const;
	TPadInfo GetPadDirect(TPadIndex index) const;
	TPadIndex GetPadDirectIndex(TPadKey key) const;
	bool IsPadDirect(TPadKey key) const {
		return ((TPadIndex(0) <= GetPadDirectIndex(key))? true: false);
	}

	// ============================================================================
	// CEvtSelector::GetPadStand
	/*!
		パッドのONエッジの取得

		@param	index	パッドインデックス

		@return	パッド情報
	 */
	// ============================================================================
	TPadInfo GetPadStand() const;
	TPadInfo GetPadStand(TPadIndex index) const;
	TPadIndex GetPadStandIndex(TPadKey key) const;
	bool IsPadStand(TPadKey key) const {
		return ((TPadIndex(0) <= GetPadStandIndex(key))? true: false);
	}

	// ============================================================================
	// CEvtSelector::GetPadRelease
	/*!
		パッドのOFFエッジの取得

		@param	index	パッドインデックス

		@return	パッド情報
	 */
	// ============================================================================
	TPadInfo GetPadRelease() const;
	TPadInfo GetPadRelease(TPadIndex index) const;
	TPadIndex GetPadReleaseIndex(TPadKey key) const;
	bool IsPadRelease(TPadKey key) const {
		return ((TPadIndex(0) <= GetPadReleaseIndex(key))? true: false);
	}

	// ============================================================================
	// CEvtSelector::GetPadRepeat
	/*!
		パッドのリピートの取得

		@param	index	パッドインデックス

		@return	パッド情報
	 */
	// ============================================================================
	TPadInfo GetPadRepeat() const;
	TPadInfo GetPadRepeat(TPadIndex index) const;
	TPadIndex GetPadRepeatIndex(TPadKey key) const;
	bool IsPadRepeat(TPadKey key) const {
		return ((TPadIndex(0) <= GetPadRepeatIndex(key))? true: false);
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CEvtSelector::CEvtSelector
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CEvtSelector();

	// ============================================================================
	// CEvtSelector::~CEvtSelector
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~CEvtSelector();


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//イベントインデックスリスト
	typedef std::deque<CEvtBase *>			TMapEvtIdx;

	//フェード用シェイプ型
	typedef er::CShape<NNS_PRIM2D_PC, 4>	TFadeShape;


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	TMapEvtIdx::size_type			m_slct;				//<選択
	TMapEvtIdx::size_type			m_direct_start;		//<即時起動
	bool							m_use_fade;			//<フェード
	struct {
		TNextEvtArgSize		size;
		TNextEvtArgBuffer	data;
	}								m_evt_arg;			//<イベントの引数
	TMapEvtIdx						m_evt_idx;			//<イベントインデックス

	TFadeShape						m_fade_shape;		//<フェード用シェイプ
	::AMS_TIMER						m_timer;			//<タイマー

#if _IPHONE
	bool						m_pad_emu_drag_mode;	//<パッドエミュレーションがタップモード
#endif //_IPHONE


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	void protectInit();
	void clearVram();
	void fadeIn();
	void slctEvt();
	void fadeOut();
	void dispTitle();
	void dispList(TColor crnt, TColor back);
	void dispList(EColor::Type crnt, EColor::Type back) {
		dispList(c_color[crnt], c_color[back]);
	}

	static void drawTaskClearVram(AMS_TCB *tcb);
	static const ::AMS_TIMER &getElapsedTime();


//------------------------------------------------------------------------------**********
};		//class CEvtSele/ctor










//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	イベントセレクタ用基礎クラス
		イベントセレクタの基礎となるクラスです。
 */
class CEvtBase {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	typedef CEvtSelector::TPos				TPos;				//<位置
	typedef CEvtSelector::TColor			TColor;				//<色
	typedef int								TFlag;				//<フラグ
	typedef CEvtSelector::TNextEvtArgSize	TNextEvtArgSize;	//<次のイベントの引数サイズ
	typedef CEvtSelector::TNextEvtArgBuffer	TNextEvtArgBuffer;	//<次のイベントの引数
	typedef int								TMoveDirect;		//<移動方向
	typedef ::GSE_EVT_ID					TEventId;			//<イベントID
	typedef CEvtSelector::EColor			EColor;				//<色列挙
	typedef CEvtSelector::TPadInfo			TPadInfo;			//<パッド情報
	typedef CEvtSelector::TPadKey			TPadKey;			//<パッドキー
	typedef CEvtSelector::TPadIndex			TPadIndex;			//<パッドインデックス


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CEvtBase::Enter
	/*!
		決定された時に実行される関数
	 */
	// ============================================================================
	virtual void Enter() {}

	// ============================================================================
	// CEvtBase::Focus
	/*!
		選択されている時に実行される関数
	 */
	// ============================================================================
	virtual void Focus() {}

	// ============================================================================
	// CEvtBase::Blur
	/*!
		選択されていない時に実行される関数
	 */
	// ============================================================================
	virtual void Blur() {}

	// ============================================================================
	// CEvtBase::OnFocus
	/*!
		選択された直後に実行される関数
	 */
	// ============================================================================
	virtual void OnFocus() {}

	// ============================================================================
	// CEvtBase::OnBlur
	/*!
		選択が解除された直後に実行される関数
	 */
	// ============================================================================
	virtual void OnBlur() {}

	// ============================================================================
	// CEvtBase::IsEnter
	/*!
		決定を判定する関数

		@retval	true	決定	
		@retval	false	未決定

		@note
			デフォルトでは Start ボタンと A ボタンが決定操作になります
	 */
	// ============================================================================
	virtual bool IsEnter();

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
	virtual TMoveDirect MoveEvent();

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
			ただし、 上画面にて下方向に溢れた時に下画面には回りこみません
	 */
	// ============================================================================
	void Printv(TPos x, TPos y, TColor clr, const char *fmt, va_list vlist);

	// ============================================================================
	// CEvtBase::Printc
	/*!
		文字を表示します

		@param	x		[in]	文字表示Ｘ座標
		@param	y		[in]	文字表示Ｙ座標
		@param	clr		[in]	文字表示色
		@param	fmt		[in]	printf書式
		@param	vlist	[in]	printf引数

		@note
			Printv参照
	 */
	// ============================================================================
	void Printc(TPos x, TPos y, TColor clr, const char *fmt, ...) {
		if (NULL != m_master) {
			va_list	vlist;
			va_start(vlist, fmt);
			m_master->Printv(x, y, clr, fmt, vlist);
			va_end(vlist);
		}
	}
	void Printc(TPos x, TPos y, EColor::Type clr, const char *fmt, ...) {
		if (NULL != m_master) {
			va_list	vlist;
			va_start(vlist, fmt);
			m_master->Printv(x, y, CEvtSelector::c_color[clr], fmt, vlist);
			va_end(vlist);
		}
	}

	// ============================================================================
	// CEvtBase::Print
	/*!
		文字を表示します

		@param	x		[in]	文字表示Ｘ座標
		@param	y		[in]	文字表示Ｙ座標
		@param	clr		[in]	文字表示色
		@param	fmt		[in]	printf書式
		@param	vlist	[in]	printf引数

		@note
			Printv参照
	 */
	// ============================================================================
	void Print(TPos x, TPos y, const char *fmt, ...) {
		if (NULL != m_master) {
			va_list	vlist;
			va_start(vlist, fmt);
			m_master->Printv(x, y, CEvtSelector::c_color[EColor::White], fmt, vlist);
			va_end(vlist);
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
	void SetNextEvtArg(TNextEvtArgSize arg_size, TNextEvtArgBuffer arg_buf);
	void SetNextEvtArg() {
		SetNextEvtArg(0, NULL);
	}

	// ============================================================================
	// CEvtBase::SetNextEvtArg
	/*!
		次のイベントの引数を設定します

		@param	arg			[in]	引数
	 */
	// ============================================================================
	template <typename T>
	void SetNextEvtArg(const T &arg) {
		SetNextEvtArg(sizeof(arg), &arg);
	}

	// ============================================================================
	// CEvtBase::GetPadDirect
	/*!
		パッドの直値の取得

		@param	index	パッドインデックス

		@return	パッド情報
	 */
	// ============================================================================
	TPadInfo GetPadDirect() const;
	TPadInfo GetPadDirect(TPadIndex index) const;
	TPadIndex GetPadDirectIndex(TPadKey key) const;
	bool IsPadDirect(TPadKey key) const;

	// ============================================================================
	// CEvtBase::GetPadStand
	/*!
		パッドのONエッジの取得

		@param	index	パッドインデックス

		@return	パッド情報
	 */
	// ============================================================================
	TPadInfo GetPadStand() const;
	TPadInfo GetPadStand(TPadIndex index) const;
	TPadIndex GetPadStandIndex(TPadKey key) const;
	bool IsPadStand(TPadKey key) const;

	// ============================================================================
	// CEvtBase::GetPadRelease
	/*!
		パッドのOFFエッジの取得

		@param	index	パッドインデックス

		@return	パッド情報
	 */
	// ============================================================================
	TPadInfo GetPadRelease() const;
	TPadInfo GetPadRelease(TPadIndex index) const;
	TPadIndex GetPadReleaseIndex(TPadKey key) const;
	bool IsPadRelease(TPadKey key) const;

	// ============================================================================
	// CEvtBase::GetPadRepeat
	/*!
		パッドのリピートの取得

		@param	index	パッドインデックス

		@return	パッド情報
	 */
	// ============================================================================
	TPadInfo GetPadRepeat() const;
	TPadInfo GetPadRepeat(TPadIndex index) const;
	TPadIndex GetPadRepeatIndex(TPadKey key) const;
	bool IsPadRepeat(TPadKey key) const;


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CEvtBase::GetTitle
	/*!
		タイトルの取得

		@return タイトル
	 */
	// ============================================================================
	const char *GetTitle() {
		return m_title;
	};

	// ============================================================================
	// CEvtBase::GetEvtId
	/*!
		イベントIDの取得

		@return イベントID
	 */
	// ============================================================================
	TEventId GetEvtId() {
		return m_evt_id;
	};

	// ============================================================================
	// CEvtBase::IsWhiteFade
	/*!
		白フェードの取得

		@retval	true	白フェードする
		@retval	false	白フェードしない
	 */
	// ============================================================================
	bool IsWhiteFade() {
		return ((m_flag & DFlag::WhiteFadeOut)? true: false);
	};


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// CEvtBase::SetWhiteFade
	/*!
		白フェードの設定

		@param	is_on	[in]	タイトル
	 */
	// ============================================================================
	void SetWhiteFade(bool effective) {
		if (effective) {
			m_flag |= DFlag::WhiteFadeOut;
		} else {
			m_flag &= ~DFlag::WhiteFadeOut;
		}
	};


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CEvtBase::CEvtBase
	/*!
		デフォルトコンストラクタ

		@param	title	[in]	タイトル
		@param	evt_id	[in]	イベントID
		@param	flag	[in]	フラグ
	 */
	// ============================================================================
public:
	CEvtBase(const char *title = NULL, TEventId evt_id = TEventId(), TFlag flag = 0)
						 : m_master(NULL), m_title(title), m_evt_id(evt_id), m_flag(flag) {};

	// ============================================================================
	// CEvtBase::CEvtBase
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~CEvtBase() {};


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//フレンド関数
	friend class CEvtSelector;
	//フラグ列挙
	struct DFlag {
		typedef int Type;
		enum {
			WhiteFadeOut	= 1 << 0,	//<白フェードアウト

			Dummy
		};
	};

	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	CEvtSelector	*m_master;	//<ノード管理者
	const char		*m_title;	//<タイトル
	TEventId		m_evt_id;	//<イベントID
	DFlag::Type		m_flag;		//<フラグ


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
	// ============================================================================
	// CEvtBase::SetTitle
	/*!
		タイトルの設定

		@param	title	[in]	タイトル
	 */
	// ============================================================================
	void SetTitle(const char *title) {
		m_title = title;
	};

	// ============================================================================
	// CEvtBase::SetEvtId
	/*!
		イベントIDの取得

		@return イベントID
	 */
	// ============================================================================
	void SetEvtId(TEventId evt_id) {
		m_evt_id = evt_id;
	};

	// ============================================================================
	// CEvtBase::GetMaster
	/*!
		ノード管理者の設定
	 */
	// ============================================================================
	CEvtSelector *GetMaster() {
		return m_master;
	};


private:
	// ============================================================================
	// CEvtBase::SetMaster
	/*!
		ノード管理者の設定
	 */
	// ============================================================================
	void SetMaster(CEvtSelector &master) {
		m_master = &master;
	};


//------------------------------------------------------------------------------**********
};		//class CEvtBase













//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	イベントセレクタ用Ｃベースクラス
		コールバックベースのクラスです。
		Ｃ言語的に使用する事が出来ます。
 */
class CEvtCb : public CEvtBase {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//コールバックタイプ列挙
	struct ECbKind {
		enum Type {
			Init		= 0,	//<初期化
			Blur,				//<選択されていない時
			OnFocus,			//<選択された時
			Focus,				//<選択され続けている
			OnBlur,				//<選択から抜けた
			Enter,				//<決定された
			Exit,				//<後処理

			Max,
			None
		};
	};
	//イベントコールバック関数
	typedef void(*FCbFunc)(ECbKind::Type kind, CEvtCb &cb, void *cb_data);


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CEvtCb::Enter
	/*!
		決定された時に実行される関数
	 */
	// ============================================================================
	virtual void Enter() {
		CallCb(ECbKind::Enter);
	}

	// ============================================================================
	// CEvtCb::Focus
	/*!
		選択されている時に実行される関数
	 */
	// ============================================================================
	virtual void Focus() {
		CallCb(ECbKind::Focus);
	}

	// ============================================================================
	// CEvtCb::Blur
	/*!
		選択されていない時に実行される関数
	 */
	// ============================================================================
	virtual void Blur() {
		CallCb(ECbKind::Blur);
	}

	// ============================================================================
	// CEvtCb::OnFocus
	/*!
		選択された直後に実行される関数
	 */
	// ============================================================================
	virtual void OnFocus() {
		CallCb(ECbKind::OnFocus);
	}

	// ============================================================================
	// CEvtCb::OnBlur
	/*!
		選択が解除された直後に実行される関数
	 */
	// ============================================================================
	virtual void OnBlur() {
		CallCb(ECbKind::OnBlur);
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CEvtCb::CEvtCb
	/*!
		デフォルトコンストラクタ

		@param	ti	[in]	タイトル
		@param	ev	[in]	イベントID
		@param	fl	[in]	フラグ
		@param	cb	[in]	コールバック
	 */
	// ============================================================================
public:
	CEvtCb(const char *title = NULL, TEventId evt_id = TEventId(), TFlag flag = 0
			, FCbFunc cb = NULL, void *cb_data = NULL)
			: CEvtBase(title, evt_id, flag), m_cb(cb), m_cb_data(cb_data) {
		CallCb(ECbKind::Init);
	};

	// ============================================================================
	// CEvtCb::~CEvtCb
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~CEvtCb() {
		CallCb(ECbKind::Exit);
	};


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	FCbFunc m_cb;		//<コールバック
	void	*m_cb_data;	//<コールバックデータ


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	// ============================================================================
	// CEvtCb::CallCb
	/*!
		コールバック呼び出し

		@param	kind	[in]	コールバックの種類
	 */
	// ============================================================================
	void CallCb(ECbKind::Type kind) {
		if (NULL != m_cb) {
			m_cb(kind, *this, m_cb_data);
		}
	}


//------------------------------------------------------------------------------**********
};		//class CEvtCb












#if _WII
#pragma warn_notinlined reset	//インライン展開出来無い関数に対する警告メッセージの無効化の解除
#endif //_WII


} //namespace dbg
#endif //#if	defined(__cplusplus)

	// ============================================================================
	// CEvtSelector::Function
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
	// ============================================================================
