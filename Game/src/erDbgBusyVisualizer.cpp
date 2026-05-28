// ============================================================================
/*!
	@file	erDbgBusyVisualizer.cpp
	@brief	負荷視覚化

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: erDbgBusyVisualizer.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "erDbgBusyVisualizer.hpp"


#if _WII
#pragma warn_notinlined off	//インライン展開出来無い関数に対する警告メッセージの無効化
#endif //_WII


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*


namespace er {
namespace dbg {



//------ Class ------------------------ クラス ---------------------------------******_CL*
// =============================================================================
// CBusyVisualizerTask::CreateInstance
/*!
	インスタンス構築
 */
// ==========================================================================
CBusyVisualizerTask &CBusyVisualizerTask::CreateInstance()
{
#if 0
	//Win32の閉じる等でAliceタスクシステム破棄後にタスクデストラクタを走らせてしまう
	static CBusyVisualizerTask instance;
	return instance;
#else
	//終了時にタスクが残ってしまうが、Alice任せ
	static CBusyVisualizerTask *instance = NULL;
	if (!instance) {
		static Uint8 instance_buf[sizeof(CBusyVisualizerTask)];
		instance = new(instance_buf) CBusyVisualizerTask();
	}
	return *instance;
#endif
}

// =============================================================================
// CBusyVisualizerTask::Create
/*!
	構築

	@retval	true	成功
	@retval	false	失敗

	@note
		失敗しません。
 */
// ==========================================================================
bool CBusyVisualizerTask::Create()
{
	m_visualizer.Create();
	AttachTask("CBusyVisualizerTask");

	return true;
}

// =============================================================================
// CBusyVisualizerTask::Release
/*!
	破棄
 */
// ==========================================================================
void CBusyVisualizerTask::Release()
{
	DetachTask();
	m_visualizer.Release();
}


//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// ============================================================================
// CBusyVisualizerTask::operator()
/*!
	フレーム毎に呼び出される関数
 */
// ============================================================================
void CBusyVisualizerTask::operator()()
{
	m_visualizer.Update();
	m_visualizer.Draw();
}


//------------------------------------------------------------------------------**********


































} //namespace dbg
} //namespace er

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
