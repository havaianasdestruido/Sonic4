// ============================================================================
/*!
	@file	erDbgTrg.hpp
	@brief	テストイベント・トリガ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: erDbgTrg.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page erTestTrgMain テストイベント・トリガ

	@section erTestTrgSummary 概要
		トリガチェック用イベントを提供します。
 */

#pragma once
#if	defined(__cplusplus)
extern "C" {
#endif

//------ C Include Files -------------- インクルード ---------------------------******CIF*
//------ C Macro ---------------------- マクロ ---------------------------------******CMC*
//------ C External Definitions ------- グローバル変数及び関数の宣言 -----------******CED*


#if	defined(__cplusplus)
} // extern "C"
#endif

#if	defined(__cplusplus)
//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "dbgEvtSelector.hpp"
#include "erTask.hpp"
#include "erTrgBasic.hpp"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace er {
namespace dbg {


#if _WII
#pragma warn_notinlined off	//インライン展開出来無い関数に対する警告メッセージの無効化
#endif //_WII




//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	テストクラス
		デバック用テストクラスです。
 */
class CTrg : public ::dbg::CEvtBase, private er::task::CProcCount<CTrg> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef ::dbg::CEvtBase				super_type;
	typedef er::task::CProcCount<CTrg>	proc_type;


	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CTrg::Enter
	/*!
		決定された時に実行される関数
	 */
	// ============================================================================
	virtual void Enter();

	// ============================================================================
	// CTrg::Focus
	/*!
		選択されている時に実行される関数
	 */
	// ============================================================================
	virtual void Focus();

	// ============================================================================
	// CTrg::MoveEvent
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


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CTrg::CTrg
	/*!
		デフォルトコンストラクタ

		@param	title	[in]	タイトル
		@param	evt_id	[in]	イベントID
		@param	flag	[in]	フラグ
	 */
	// ============================================================================
public:
	CTrg(const char *title = NULL, TEventId evt_id = TEventId(), TFlag flag = 0)
						 : super_type(title, evt_id, flag) {
		 SetTarget(*this);
		 m_is_lock = false;
	 	init();
	}
#if 0
	CTrg(const CTrg &src) : m_is_lock(src.m_is_lock), m_rect(src.m_rect) {
	}
#endif

	// ============================================================================
	// CTrg::CTrg
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~CTrg() {
		clear();
	}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:

	
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	bool				m_is_lock;
	er::CTrgRect		m_rect;


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	void clear();
	void init();

	void start();


//------------------------------------------------------------------------------**********
};		//class CTrg

























#if _WII
#pragma warn_notinlined reset	//インライン展開出来無い関数に対する警告メッセージの無効化の解除
#endif //_WII


} //namespace dbg
} //namespace er
#endif //#if	defined(__cplusplus)

	// ============================================================================
	// CTrg::Function
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
