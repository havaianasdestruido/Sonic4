// ============================================================================
/*!
	@file	gsBackupSpecial.cpp
	@brief	バックアップ・スペステ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: gsBackupSpecial.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "gsBackupSpecial.hpp"
#include "gsBackup.hpp"
#include <algorithm>



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
// SSpecialSolo::Init
/*!
	初期化
 */
// ============================================================================
void SSpecialSolo::Init()
{
	m_high_score = c_high_score_max_limit / c_high_score_unit;
	m_fast_time = c_fast_time_max_limit;
	
	m_is_high_score_enable		= c_false;
	m_is_fast_time_enable		= c_false;
	m_is_high_score_uploaded	= c_false;
	m_is_fast_time_uploaded		= c_false;
	m_emerald_stage				= 0;
	m_is_score_uploaded_once		= c_false;
	m_is_time_uploaded_once			= c_false;
	
#ifdef MPPDEBUG_OPEN_ALL		
	if(!false) {{//qqq//test -- open all
		m_high_score = c_high_score_max_limit / c_high_score_unit;
		m_fast_time = c_fast_time_max_limit;
		
		m_is_high_score_enable		= !c_false;
		m_is_fast_time_enable		= !c_false;
		m_is_high_score_uploaded	= !c_false;
		m_is_fast_time_uploaded		= !c_false;
		m_emerald_stage				= 01;
		m_is_score_uploaded_once		= !c_false;
		m_is_time_uploaded_once			= !c_false;
	}}
#endif	
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
// ============================================================================
// SSpecialSolo::SetNew
/*!
	NEW状態設定
 
	@param	is_new	[in]	NEW状態か
 */
// ============================================================================
void SSpecialSolo::SetNew(bool)
{
}

// ============================================================================
// SSpecialSolo::SetHighScore
/*!
	ハイスコア設定
 
	@param	high_score	[in]	ハイスコア
 */
// ============================================================================
void SSpecialSolo::SetHighScore(Uint32 high_score)
{
	high_score = (std::min<Uint32>)(high_score, c_high_score_max_limit);
	m_high_score = high_score / c_high_score_unit;
	m_is_high_score_enable = c_true;
}

// ============================================================================
// SStageSolo::SetFastTime
/*!
	最速タイム設定
 
	@param	fast_time			[in]	最速タイム
 */
// ============================================================================
void SSpecialSolo::SetFastTime(Uint32 fast_time)
{
	fast_time = (std::min<Uint32>)(fast_time, c_fast_time_max_limit);
	m_fast_time = fast_time;
	m_is_fast_time_enable = c_true;
}

// ============================================================================
// SStageSolo::SetHighScoreUploaded
/*!
	ハイスコアのアップロード設定
 
	@param	is_uploaded	[in]	アップロード済みか
 */
// ============================================================================
void SSpecialSolo::SetHighScoreUploaded(bool is_uploaded)
{
	m_is_high_score_uploaded = ((is_uploaded)? c_true: c_false);
}

// ============================================================================
// SStageSolo::SetFastTimeUploaded
/*!
	最速タイムのアップロード設定
 
	@param	is_uploaded	[in]	アップロード済みか
 */
// ============================================================================
void SSpecialSolo::SetFastTimeUploaded(bool is_uploaded)
{
	m_is_fast_time_uploaded = ((is_uploaded)? c_true: c_false);
}

// ============================================================================
// SSpecialSolo::SetEmeraldStage
/*!
	エメラルド獲得ステージ設定
 
	@param	stage	[in]	ステージ
 */
// ============================================================================
void SSpecialSolo::SetEmeraldStage(EEmeraldStage::Type emerald_stage)
{
	emerald_stage = (std::min)(emerald_stage, EEmeraldStage::Max);
	m_emerald_stage = static_cast<Uint32>(emerald_stage);
}

// ============================================================================
// SSpecialSolo::SetScoreUploadedOnce
/*!
	過去1回はスコアをアップロード済み設定
 
	@param is_uploaded_once	[in]	過去1回はアップロード済みか
 */
// ============================================================================
void SSpecialSolo::SetScoreUploadedOnce(bool is_uploaded_once)
{
	m_is_score_uploaded_once = ((is_uploaded_once)? c_true: c_false);
}

// ============================================================================
// SSpecialSolo::SetTimeUploadedOnce
/*!
	過去1回はタイムレコードをアップロード済み設定
 
	@param is_uploaded_once	[in]	過去1回はアップロード済みか
 */
// ============================================================================
void SSpecialSolo::SetTimeUploadedOnce(bool is_uploaded_once)
{
	m_is_time_uploaded_once = ((is_uploaded_once)? c_true: c_false);
}

#if defined(MTD_DEBUG)
// ============================================================================
// SSpecialSolo::SetHighScoreEnable
/*!
	ハイスコア有効設定
 
	@param is_enable	[in]	ハイスコアは有効か
 */
// ============================================================================
void SSpecialSolo::SetHighScoreEnable(bool is_enable)
{
	m_is_high_score_enable = ((is_enable)? c_true: c_false);
}
#endif //defined(MTD_DEBUG)

#if defined(MTD_DEBUG)
// ============================================================================
// SSpecialSolo::SetFastTimeEnable
/*!
	最速タイム有効設定
 
	@param is_enable	[in]	最速タイムは有効か
 */
// ============================================================================
void SSpecialSolo::SetFastTimeEnable(bool is_enable)
{
	m_is_fast_time_enable = ((is_enable)? c_true: c_false);
}
#endif //defined(MTD_DEBUG)


//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********



















































//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// SSpecial::CreateInstance
/*!
	作成

	@param	save_index	[in]	セーブインデックス

	@note
		セーブインデックスはWii以外では無視されます。
		Wiiにて省略されるとカレントセーブインデックスが使用されます。
 */
// ============================================================================
SSpecial &SSpecial::CreateInstance(Uint32 save_index)
{
	return SBackup::CreateInstance().GetSpecial(save_index);
}
SSpecial &SSpecial::CreateInstance()
{
	return SBackup::CreateInstance().GetSpecial();
}

// ============================================================================
// SSpecial::Init
/*!
	初期化
 */
// ============================================================================
void SSpecial::Init()
{
	for (SSpecialSolo *ite = m_stage, *ite_end = m_stage + c_size; ite != ite_end; ++ite) {
		ite->Init();
	}
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********



















































} //namespace backup
} //namespace gs

// =============================================================================
// gs::backup::SSpecial
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
