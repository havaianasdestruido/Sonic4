// ============================================================================
/*!
	@file	gsBackupSystem.cpp
	@brief	バックアップ・システム

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: gsBackupSystem.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "gsBackupSystem.hpp"
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
// SSystem::CreateInstance
/*!
	作成

	@param	save_index	[in]	セーブインデックス

	@note
		セーブインデックスはWii以外では無視されます。
		Wiiにて省略されるとカレントセーブインデックスが使用されます。
 */
// ============================================================================
SSystem &SSystem::CreateInstance(Uint32 save_index)
{
	return SBackup::CreateInstance().GetSystem(save_index);
}
SSystem &SSystem::CreateInstance()
{
	return SBackup::CreateInstance().GetSystem();
}

// ============================================================================
// SSystem::Init
/*!
	初期化
 */
// ============================================================================
void SSystem::Init()
{
	m_player_stock					= 3;
	m_killed						= 0;
	m_clear_count					= 0;
	m_announce_open_zone_select		= c_false;
	m_announce_open_zone1_boss		= c_false;
	m_announce_open_zone2_boss		= c_false;
	m_announce_open_zone3_boss		= c_false;
	m_announce_open_zone4_boss		= c_false;
	m_announce_open_final_zone		= c_false;
	m_announce_open_supersonic		= c_false;
	m_announce_open_specialstage	= c_false;

#if _WII
	m_last_clear_act			= 0;
	m_last_save_chrono			= (Sint64)0;
	amZeroMemory(&m_dwc_userdata, sizeof(DWCUserData));
#endif //_WII
#if _IPHONE
	m_announcetruck_tilt			= c_false;
	m_announcetruck_flick			= c_false;
	m_announcespecial_stage_tilt	= c_false;
	m_announcespecial_stage_flick	= c_false;
#endif //_IPHONE
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
// ============================================================================
// SSystem::IsAnnounce
/*!
	アナウンス確認

	@param	index	[in]	アナウンスインデックス
 
	@retval	true	アナウンス済み
	@retval	false	未アナウンス
 */
// ============================================================================
bool SSystem::IsAnnounce(EAnnounce::Type index) const
{
	Uint32 result;
	switch (index) {
	case EAnnounce::OpenZoneSelect:		result = m_announce_open_zone_select;	break;
	case EAnnounce::OpenZone1Boss:		result = m_announce_open_zone1_boss;	break;
	case EAnnounce::OpenZone2Boss:		result = m_announce_open_zone2_boss;	break;
	case EAnnounce::OpenZone3Boss:		result = m_announce_open_zone3_boss;	break;
	case EAnnounce::OpenZone4Boss:		result = m_announce_open_zone4_boss;	break;
	case EAnnounce::OpenFinalZone:		result = m_announce_open_final_zone;	break;
	case EAnnounce::OpenSuperSonic:		result = m_announce_open_supersonic;	break;
	case EAnnounce::OpenSpecialStage:	result = m_announce_open_specialstage;	break;
#if _IPHONE
	case EAnnounce::TruckTilt:			result = m_announcetruck_tilt;			break;
	case EAnnounce::TruckFlick:			result = m_announcetruck_flick;			break;
	case EAnnounce::SpecialStageTilt:	result = m_announcespecial_stage_tilt;	break;
	case EAnnounce::SpecialStageFlick:	result = m_announcespecial_stage_flick;	break;
#endif //_IPHONE
	default:							result = c_false;						break;
	}
	return ((c_false != result)? true: false);
}


//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
// ============================================================================
// SSystem::SetPlayerStock
/*!
	残機設定
 
	@param	player_stock	[in]	残機
 */
// ============================================================================
void SSystem::SetPlayerStock(Uint32 player_stock)
{
	player_stock = (std::min<Uint32>)(player_stock, c_player_stock_limit);
	m_player_stock = player_stock;
}

// ============================================================================
// SSystem::SetKilled
/*!
	累計エネミー撃退数設定
 
	@return 累計エネミー撃退数
 */
// ============================================================================
void SSystem::SetKilled(Uint32 killed)
{
	killed = (std::min<Uint32>)(killed, c_killed_limit);
	m_killed = killed;
}

// ============================================================================
// SSystem::SetClearCount
/*!
	ゲームクリア回数設定
 
	@return ゲームクリア回数
 */
// ============================================================================
void SSystem::SetClearCount(Uint32 count)
{
	count = (std::min<Uint32>)(count, c_clear_count_limit);
	m_clear_count = count;
}

// ============================================================================
// SSystem::SetAnnounce
/*!
	アナウンス設定

	@param	index		[in]	アナウンスインデックス
	@param	is_announce	[in]	アナウンスしたか
 */
// ============================================================================
void SSystem::SetAnnounce(EAnnounce::Type index, bool is_announce)
{
	Uint32 set = ((is_announce)? c_true: c_false);
	switch (index) {
	case EAnnounce::OpenZoneSelect:		m_announce_open_zone_select = set;	break;
	case EAnnounce::OpenZone1Boss:		m_announce_open_zone1_boss = set;	break;
	case EAnnounce::OpenZone2Boss:		m_announce_open_zone2_boss = set;	break;
	case EAnnounce::OpenZone3Boss:		m_announce_open_zone3_boss = set;	break;
	case EAnnounce::OpenZone4Boss:		m_announce_open_zone4_boss = set;	break;
	case EAnnounce::OpenFinalZone:		m_announce_open_final_zone = set;	break;
	case EAnnounce::OpenSuperSonic:		m_announce_open_supersonic = set;	break;
	case EAnnounce::OpenSpecialStage:	m_announce_open_specialstage = set;	break;
#if _IPHONE
	case EAnnounce::TruckTilt:			m_announcetruck_tilt = set;				break;
	case EAnnounce::TruckFlick:			m_announcetruck_flick = set;			break;
	case EAnnounce::SpecialStageTilt:	m_announcespecial_stage_tilt = set;		break;
	case EAnnounce::SpecialStageFlick:	m_announcespecial_stage_flick = set;	break;
#endif //_IPHONE
	default:																break;
	}
}

#if _WII
// ============================================================================
// SSystem::SetLastClearAct
/*!
	最終クリアアクト設定
 
	@param	last_clear_act	[in]	最終クリアアクト
 */
// ============================================================================
void SSystem::SetLastClearAct(EStage::Type last_clear_act)
{
	last_clear_act = (std::min)(last_clear_act, EStage::Max);
	m_last_clear_act = static_cast<Uint32>(last_clear_act);
}
#endif //_WII

#if _WII
// ============================================================================
// SSystem::SetLastSaveChrono
/*!
	最終セーブ日時設定
 
	@param	last_save_chrono	[in]	最終セーブ日時
 */
// ============================================================================
void SSystem::SetLastSaveChrono(Sint64 last_save_chrono)
{
	m_last_save_chrono = last_save_chrono;
}
#endif //_WII


//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********



















































} //namespace backup
} //namespace gs

// =============================================================================
// gs::backup::SSystem
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
