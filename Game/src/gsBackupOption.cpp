// ============================================================================
/*!
	@file	gsBackupOption.cpp
	@brief	バックアップ・オプション

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: gsBackupOption.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "gsBackupOption.hpp"
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
// SOption::CreateInstance
/*!
	作成

	@param	save_index	[in]	セーブインデックス

	@note
		セーブインデックスはWii以外では無視されます。
		Wiiにて省略されるとカレントセーブインデックスが使用されます。
 */
// ============================================================================
SOption &SOption::CreateInstance(Uint32 save_index)
{
	return SBackup::CreateInstance().GetOption(save_index);
}
SOption &SOption::CreateInstance()
{
	return SBackup::CreateInstance().GetOption();
}

// ============================================================================
// SOption::Init
/*!
	初期化
 */
// ============================================================================
void SOption::Init()
{
	m_is_vibration	= c_true;
	m_volume_bgm	= c_volume_bgm_max_limit / 10;
	m_volume_se		= c_volume_se_max_limit / 10;
#if _IPHONE
	m_control		= EControl::VirtualPadDown;
#endif //_IPHONE
#if _WII
	amZeroMemory(m_name, sizeof(char) * c_name_length_limit_pad);
#endif // _WII
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
// ============================================================================
// SOption::SetVibration
/*!
	振動設定
 
	@param	is_vibration	[in]	振動するか
 */
// ============================================================================
void SOption::SetVibration(bool is_vibration)
{
	m_is_vibration = ((is_vibration)? c_true: c_false);
}

// ============================================================================
// SOption::SetVolumeBgm
/*!
	BGMボリューム設定
 
	@param	volume_bgm	[in]	BGMボリューム
 */
// ============================================================================
void SOption::SetVolumeBgm(Uint32 volume_bgm)
{
	volume_bgm = (std::min<Uint32>)(volume_bgm, c_volume_bgm_max_limit);
	m_volume_bgm = volume_bgm / c_volume_bgm_unit;
}

// ============================================================================
// SOption::SetVolumeSe
/*!
	SEボリューム設定
 
	@param	volume_se	[in]	SEボリューム
 */
// ============================================================================
void SOption::SetVolumeSe(Uint32 volume_se)
{
	volume_se = (std::min<Uint32>)(volume_se, c_volume_se_max_limit);
	m_volume_se = volume_se / c_volume_se_unit;
}

#if _WII
// ============================================================================
// SOption::SetName
/*!
	ユーザ名設定
 
	@param	name		[in]	ユーザ名(ASCIIコード)
 */
// ============================================================================
void SOption::SetName(const char* name)
{
	amCopyMemory(m_name, name, c_name_length_limit_pad);
	m_name[c_name_length_limit] = '\0';
}
#endif // _WII

#if _IPHONE
// ============================================================================
// SOption::SetControl
/*!
	コントロール方法の設定

	@return コントロール方法
 */
// ============================================================================
void SOption::SetControl(EControl::Type control)
{
	control = (std::min)(control, EControl::Max);
	m_control = control;
}
#endif //_IPHONE

//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********



















































} //namespace backup
} //namespace gs

// =============================================================================
// gs::backup::SOption
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
