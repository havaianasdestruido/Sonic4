// ============================================================================
/*!
	@file	gsBackup.cpp
	@brief	バックアップ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: gsBackup.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "gsBackup.hpp"
#include "gsMainSys.h"



//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*


namespace gs {
namespace backup {
//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// SBackup::CreateInstance
/*!
	作成
 */
// ============================================================================
SBackup &SBackup::CreateInstance()
{
	GSS_MAIN_SYS_INFO *main_info = GsGetMainSysInfo();
	return main_info->backup;
}

// ============================================================================
// SBackup::Init
/*!
	初期化
 */
// ============================================================================
void SBackup::Init()
{
	for (SSystem *ite = m_system, *ite_end = m_system + c_save_size; ite != ite_end; ++ite) {
		ite->Init();
	}
	for (SOption *ite = m_option, *ite_end = m_option + c_save_size; ite != ite_end; ++ite) {
		ite->Init();
	}
	for (SStage *ite = m_stage, *ite_end = m_stage + c_save_size; ite != ite_end; ++ite) {
		ite->Init();
	}
	for (SSpecial *ite = m_special, *ite_end = m_special + c_save_size; ite != ite_end; ++ite) {
		ite->Init();
	}

#if _WII
	m_last_save_index = 0;
#endif //_WII
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********



















































} //namespace backup
} //namespace gs

// =============================================================================
// gs::backup::SBackup
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
