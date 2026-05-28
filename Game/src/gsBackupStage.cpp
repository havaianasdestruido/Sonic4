// ============================================================================
/*!
	@file	gsBackupStage.cpp
	@brief	バックアップ・ステージ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: gsBackupStage.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "gsBackupStage.hpp"
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
// SStageSolo::Init
/*!
	初期化
 */
// ============================================================================
void SStageSolo::Init()
{
	for (SRecord *ite = m_record, *ite_end = m_record + ERecordKind::Max; ite != ite_end; ++ite) {
		ite->high_score				= c_high_score_max_limit / c_high_score_unit;
		ite->fast_time				= c_fast_time_max_limit;
		ite->is_high_score_enable	= c_false;
		ite->is_fast_time_enable	= c_false;
		ite->is_high_score_uploaded	= c_false;
		ite->is_fast_time_uploaded	= c_false;
	}
	m_is_new						= c_true;
	m_is_high_score_use_supersonic	= c_false;
	m_is_fast_time_use_supersonic	= c_false;
	m_is_score_uploaded_once		= c_false;
	m_is_time_uploaded_once			= c_false;
	m_is_use_supersonic_once		= c_false;
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
// ============================================================================
// SStageSolo::IsHighScoreEnable
/*!
	ハイスコアは有効か
 
	@param	is_supersonic	[in]	スーパーソニックか
	@retval	true	有効
	@retval	false	無効
 */
// ============================================================================
bool SStageSolo::IsHighScoreEnable(bool is_supersonic) const
{
	return ((c_false != getRecord(is_supersonic).is_high_score_enable)? true: false);
}

// ============================================================================
// SStageSolo::IsFastTimeEnable
/*!
	最速タイムは有効か
 
	@param	is_supersonic	[in]	スーパーソニックか
	@retval	true	有効
	@retval	false	無効
 */
// ============================================================================
bool SStageSolo::IsFastTimeEnable(bool is_supersonic) const
{
	return ((c_false != getRecord(is_supersonic).is_fast_time_enable)? true: false);
}

// ============================================================================
// SStageSolo::GetHighScore
/*!
	ハイスコアの取得
 
	@param	is_supersonic	[in]	スーパーソニックか
	@return	ハイスコア
 */
// ============================================================================
Uint32 SStageSolo::GetHighScore(bool is_supersonic) const
{
	return getRecord(is_supersonic).high_score * c_high_score_unit;
}

// ============================================================================
// SStageSolo::GetFastTime
/*!
	最速タイムの取得
 
	@param	is_supersonic	[in]	スーパーソニックか
	@return	最速タイム
 */
// ============================================================================
Uint32 SStageSolo::GetFastTime(bool is_supersonic) const
{
	return getRecord(is_supersonic).fast_time;
}

// ============================================================================
// SStageSolo::IsHighScoreUploaded
/*!
	ハイスコアはアップロード済みか
 
	@param	is_supersonic	[in]	スーパーソニックか
	@retval	true	アップロード済み
	@retval	false	未アップロード
 */
// ============================================================================
bool SStageSolo::IsHighScoreUploaded(bool is_supersonic) const
{
	return ((c_false != getRecord(is_supersonic).is_high_score_uploaded)? true: false);
}

// ============================================================================
// SStageSolo::IsFastTimeUploaded
/*!
	最速タイムはアップロード済みか
 
	@param	is_supersonic	[in]	スーパーソニックか
	@retval	true	アップロード済み
	@retval	false	未アップロード
 */
// ============================================================================
bool SStageSolo::IsFastTimeUploaded(bool is_supersonic) const
{
	return ((c_false != getRecord(is_supersonic).is_fast_time_uploaded)? true: false);
}


//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
// ============================================================================
// SStageSolo::SetNew
/*!
	NEW状態設定
 
	@param	is_new	[in]	NEW状態か
 */
// ============================================================================
void SStageSolo::SetNew(bool is_new)
{
	m_is_new = ((is_new)? c_true: c_false);
}

// ============================================================================
// SStageSolo::SetHighScore
/*!
	ハイスコア設定
 
	@param	high_score			[in]	ハイスコア
	@param is_use_supersonic	[in]	スーパーソニック使用か
 */
// ============================================================================
void SStageSolo::SetHighScore(Uint32 high_score, bool is_use_supersonic)
{
	SRecord &record = getRecord(is_use_supersonic);
	high_score = (std::min<Uint32>)(high_score, c_high_score_max_limit);
	record.high_score = high_score / c_high_score_unit;
	record.is_high_score_enable = c_true;

	if (getRecord(!is_use_supersonic).high_score < record.high_score) {
		m_is_high_score_use_supersonic = ((is_use_supersonic)? c_true: c_false);
	}
	if (is_use_supersonic) {
		m_is_use_supersonic_once = c_true;
	}
}

// ============================================================================
// SStageSolo::SetFastTime
/*!
	最速タイム設定
 
	@param	fast_time			[in]	最速タイム
	@param is_use_supersonic	[in]	スーパーソニック使用か
 */
// ============================================================================
void SStageSolo::SetFastTime(Uint32 fast_time, bool is_use_supersonic)
{
	SRecord &record = getRecord(is_use_supersonic);
	fast_time = (std::min<Uint32>)(fast_time, c_fast_time_max_limit);
	record.fast_time = fast_time;
	record.is_fast_time_enable = c_true;

	if (record.fast_time < getRecord(!is_use_supersonic).fast_time) {
		m_is_fast_time_use_supersonic = ((is_use_supersonic)? c_true: c_false);
	}
	if (is_use_supersonic) {
		m_is_use_supersonic_once = c_true;
	}
}

// ============================================================================
// SStageSolo::SetHighScoreUploaded
/*!
	ハイスコアのアップロード設定
 
	@param	is_supersonic	[in]	スーパーソニックか
	@param	is_uploaded		[in]	アップロード済みか
 */
// ============================================================================
void SStageSolo::SetHighScoreUploaded(bool is_supersonic, bool is_uploaded)
{
	getRecord(is_supersonic).is_high_score_uploaded = ((is_uploaded)? c_true: c_false);
//	m_is_score_uploaded_once = c_true;
}

// ============================================================================
// SStageSolo::SetFastTimeUploaded
/*!
	最速タイムのアップロード設定
 
	@param	is_supersonic	[in]	スーパーソニックか
	@param	is_uploaded		[in]	アップロード済みか
 */
// ============================================================================
void SStageSolo::SetFastTimeUploaded(bool is_supersonic, bool is_uploaded)
{
	getRecord(is_supersonic).is_fast_time_uploaded = ((is_uploaded)? c_true: c_false);
//	m_is_time_uploaded_once = c_true;
}

// ============================================================================
// SStageSolo::SetScoreUploadedOnce
/*!
	過去1回はスコアをアップロード済み設定
 
	@param is_uploaded_once	[in]	過去1回はアップロード済みか
 */
// ============================================================================
void SStageSolo::SetScoreUploadedOnce(bool is_uploaded_once)
{
	m_is_score_uploaded_once = ((is_uploaded_once)? c_true: c_false);
}

// ============================================================================
// SStageSolo::SetTimeUploadedOnce
/*!
	過去1回はタイムレコードをアップロード済み設定
 
	@param is_uploaded_once	[in]	過去1回はアップロード済みか
 */
// ============================================================================
void SStageSolo::SetTimeUploadedOnce(bool is_uploaded_once)
{
	m_is_time_uploaded_once = ((is_uploaded_once)? c_true: c_false);
}

// ============================================================================
// SStageSolo::SetUseSuperSonicOnce
/*!
	過去1回はスーパーソニック使用設定
 
	@param is_use_supersonic_once	[in]	スーパーソニック使用か
 */
// ============================================================================
void SStageSolo::SetUseSuperSonicOnce(bool is_use_supersonic_once)
{
	m_is_use_supersonic_once = ((is_use_supersonic_once)? c_true: c_false);
}

#if defined(MTD_DEBUG)
// ============================================================================
// SStageSolo::SetHighScoreEnable
/*!
	ハイスコア有効設定
 
	@param	is_supersonic	[in]	スーパーソニックか
	@param	is_enable		[in]	ハイスコアは有効か
 */
// ============================================================================
void SStageSolo::SetHighScoreEnable(bool is_supersonic, bool is_enable)
{
	getRecord(is_supersonic).is_high_score_enable = ((is_enable)? c_true: c_false);
}
#endif //defined(MTD_DEBUG)

#if defined(MTD_DEBUG)
// ============================================================================
// SStageSolo::SetFastTimeEnable
/*!
	最速タイム有効設定
 
	@param	is_supersonic	[in]	スーパーソニックか
	@param	is_enable		[in]	最速タイムは有効か
 */
// ============================================================================
void SStageSolo::SetFastTimeEnable(bool is_supersonic, bool is_enable)
{
	getRecord(is_supersonic).is_fast_time_enable = ((is_enable)? c_true: c_false);
}
#endif //defined(MTD_DEBUG)

#if defined(MTD_DEBUG)
// ============================================================================
// SStageSolo::SetUseSuperSonicOnce
/*!
	ハイスコアはスーパーソニック使用設定
 
	@param is_use_supersonic	[in]	スーパーソニック使用か
 */
// ============================================================================
void SStageSolo::SetHighScoreUseSuperSonic(bool is_use_supersonic)
{
	m_is_high_score_use_supersonic = ((is_use_supersonic)? c_true: c_false);
}
#endif //defined(MTD_DEBUG)

#if defined(MTD_DEBUG)
// ============================================================================
// SStageSolo::SetFastTimeUseSuperSonic
/*!
	最速タイムはスーパーソニック使用設定
 
	@param is_use_supersonic	[in]	スーパーソニック使用か
 */
// ============================================================================
void SStageSolo::SetFastTimeUseSuperSonic(bool is_use_supersonic)
{
	m_is_fast_time_use_supersonic = ((is_use_supersonic)? c_true: c_false);
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
// SStage::CreateInstance
/*!
	作成

	@param	save_index	[in]	セーブインデックス

	@note
		セーブインデックスはWii以外では無視されます。
		Wiiにて省略されるとカレントセーブインデックスが使用されます。
 */
// ============================================================================
SStage &SStage::CreateInstance(Uint32 save_index)
{
	return SBackup::CreateInstance().GetStage(save_index);
}
SStage &SStage::CreateInstance()
{
	return SBackup::CreateInstance().GetStage();
}

// ============================================================================
// SStage::Init
/*!
	初期化
 */
// ============================================================================
void SStage::Init()
{
	for (SStageSolo *ite = m_stage, *ite_end = m_stage + c_size; ite != ite_end; ++ite) {
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
// gs::backup::SStage
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
