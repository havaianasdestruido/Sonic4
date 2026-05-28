// ============================================================================
/*!
	@file	gsBackupSystem.hpp
	@brief	バックアップ・システム

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: gsBackupSystem.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page gsBackupSystemMain バックアップ・システム

	@section gsBackupSystemSummary 概要
		バックアップのシステムを管理します。
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
#include "gsBackupDefine.hpp"
#if _WII
#include "dwc.h"
#endif // _WII


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace gs {
namespace backup {









//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	バックアップ・システムクラス
		バックアップのシステムを管理します。
 */
class SSystem {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	enum {
		c_false					= 0,	//<bool in Sint32
		c_true					= 1,	//<bool in Sint32
		c_player_stock_limit	= 1000,	//<残機上限(999 表示値を-1にする為 1,000に設定)
		c_killed_limit			= 1000,	//<累計エネミー撃退数上限(1,000)
		c_clear_count_limit		= 2,	//<ゲームクリア回数上限(2)
	};

	//アナウンスインデックス
	struct EAnnounce {
		enum Type {
			OpenZoneSelect,		//<ゾーンセレクト開放
			OpenZone1Boss,		//<ゾーン1ボス開放
			OpenZone2Boss,		//<ゾーン2ボス開放
			OpenZone3Boss,		//<ゾーン3ボス開放
			OpenZone4Boss,		//<ゾーン4ボス開放
			OpenFinalZone,		//<ファイナルゾーン開放
			OpenSuperSonic,		//<スーパーソニック開放
			OpenSpecialStage,	//<スペシャルステージゾーン開放
#if _IPHONE
			TruckTilt,			//<トロッコステージ・傾斜操作メッセージ
			TruckFlick,			//<トロッコステージ・フリック操作メッセージ
			SpecialStageTilt,	//<スペシャルステージ・傾斜操作メッセージ
			SpecialStageFlick,	//<スペシャルステージ・フリック操作メッセージ
#endif //_IPHONE

			Max,
			None,
		};
	};


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
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
	static SSystem &CreateInstance(Uint32 save_index);
	static SSystem &CreateInstance();

	// ============================================================================
	// SSystem::Init
	/*!
		初期化
	 */
	// ============================================================================
	void Init();


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// SSystem::GetPlayerStock
	/*!
		残機の取得
	 
		@return 残機
	 */
	// ============================================================================
	Uint32 GetPlayerStock() const {
		return m_player_stock;
	}

	// ============================================================================
	// SSystem::GetKilled
	/*!
		累計エネミー撃退数の取得
	 
		@return 累計エネミー撃退数
	 */
	// ============================================================================
	Uint32 GetKilled() const {
		return m_killed;
	}

	// ============================================================================
	// SSystem::GetClearCount
	/*!
		ゲームクリア回数の取得
	 
		@return ゲームクリア回数
	 */
	// ============================================================================
	Uint32 GetClearCount() const {
		return m_clear_count;
	}

	// ============================================================================
	// SSystem::IsAnnounce
	/*!
		アナウンス確認

		@param	index	[in]	アナウンスインデックス
	 
		@retval	true	アナウンス済み
		@retval	false	未アナウンス
	 */
	// ============================================================================
	bool IsAnnounce(EAnnounce::Type index) const;

#if _WII
	// ============================================================================
	// SSystem::GetLastClearAct
	/*!
		最終クリアアクトの取得
	 
		@return 最終クリアアクト
	 */
	// ============================================================================
	EStage::Type GetLastClearAct() const {
		return static_cast<EStage::Type>(m_last_clear_act);
	}
#endif //_WII

#if _WII
	// ============================================================================
	// SSystem::GetLastSaveChrono
	/*!
		最終セーブ日時の取得
	 
		@return 最終セーブ日時
	 */
	// ============================================================================
	Sint64 GetLastSaveChrono() const {
		return m_last_save_chrono;
	}
#endif //_WII

#if _WII
	// ============================================================================
	// SSystem::GetDwcUserData
	/*!
		DWCユーザデータの取得
	 
		@return 最終セーブ日時
	 */
	// ============================================================================
	DWCUserData& GetDwcUserData() {
		return m_dwc_userdata;
	};
#endif // _WII

	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// SSystem::SetPlayerStock
	/*!
		残機設定
	 
		@param	player_stock	[in]	残機
	 */
	// ============================================================================
	void SetPlayerStock(Uint32 player_stock);

	// ============================================================================
	// SSystem::SetKilled
	/*!
		累計エネミー撃退数設定
	 
		@return 累計エネミー撃退数
	 */
	// ============================================================================
	void SetKilled(Uint32 killed);

	// ============================================================================
	// SSystem::SetClearCount
	/*!
		ゲームクリア回数設定
	 
		@return ゲームクリア回数
	 */
	// ============================================================================
	void SetClearCount(Uint32 count);

	// ============================================================================
	// SSystem::SetAnnounce
	/*!
		アナウンス設定

		@param	index		[in]	アナウンスインデックス
		@param	is_announce	[in]	アナウンスしたか
	 */
	// ============================================================================
	void SetAnnounce(EAnnounce::Type index, bool is_announce);

#if _WII
	// ============================================================================
	// SSystem::SetLastClearAct
	/*!
		最終クリアアクト設定
	 
		@param	last_clear_act	[in]	最終クリアアクト
	 */
	// ============================================================================
	void SetLastClearAct(EStage::Type last_clear_act);
#endif // _WII

#if _WII
	// ============================================================================
	// SSystem::SetLastSaveChrono
	/*!
		最終セーブ日時設定
	 
		@param	last_save_chrono	[in]	最終セーブ日時
	 */
	// ============================================================================
	void SetLastSaveChrono(Sint64 last_save_chrono);
#endif // _WII


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	struct {
		Uint32	m_player_stock					:10;	//<残機
		Uint32	m_killed						:10;	//<累計エネミー撃退数
		Uint32	m_clear_count					:2;		//<ゲームクリア回数(2回まで保存)
		Uint32	m_announce_open_zone_select		:1;		//<アナウンス・ゾーンセレクト開放
		Uint32	m_announce_open_zone1_boss		:1;		//<アナウンス・ゾーン1ボス開放
		Uint32	m_announce_open_zone2_boss		:1;		//<アナウンス・ゾーン2ボス開放
		Uint32	m_announce_open_zone3_boss		:1;		//<アナウンス・ゾーン3ボス開放
		Uint32	m_announce_open_zone4_boss		:1;		//<アナウンス・ゾーン4ボス開放
		Uint32	m_announce_open_final_zone		:1;		//<アナウンス・ファイナルゾーン開放
		Uint32	m_announce_open_supersonic		:1;		//<アナウンス・スーパーソニック開放
		Uint32	m_announce_open_specialstage	:1;		//<アナウンス・スペステゾーン開放
		Uint32	m_reserve1						:2;
	};

#if _WII
	struct {
		Sint64	m_last_save_chrono				;		//<最終セーブ日時
	};
	struct {
		Uint32	m_last_clear_act				:5;		//<最終クリアアクト
		Uint32	m_reserve2						:27;
	};
	DWCUserData	m_dwc_userdata;						//<DWCユーザデータ
#endif //_WII
#if _IPHONE
	struct {
		Uint32	m_announcetruck_tilt			:1;	//<アナウンス・トロッコステージ・傾斜操作メッセージ
		Uint32	m_announcetruck_flick			:1;	//<アナウンス・トロッコステージ・フリック操作メッセージ
		Uint32	m_announcespecial_stage_tilt	:1;	//<アナウンス・スペシャルステージ・傾斜操作メッセージ
		Uint32	m_announcespecial_stage_flick	:1;	//<アナウンス・スペシャルステージ・フリック操作メッセージ
		Uint32	m_reserve2						:28;
	};
#endif //_IPHONE


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class SSystem






















} //namespace backup
} //namespace gs
#endif //#if	defined(__cplusplus)

	// ============================================================================
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
	// ============================================================================
