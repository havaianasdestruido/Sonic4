// ============================================================================
/*!
	@file	gsBackup.hpp
	@brief	バックアップ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: gsBackup.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page gsBackupMain バックアップ

	@section gsBackupSummary 概要
		バックアップを提供します。
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
#include "gsBackupSystem.hpp"
#include "gsBackupOption.hpp"
#include "gsBackupStage.hpp"
#include "gsBackupSpecial.hpp"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace gs {
namespace backup {









//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	バックアップメインクラス
		バックアップのメインを提供します。
 */
class SBackup {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
#if _WII
	static const Uint32 c_save_size	= 6;								//<セーブデータ数
	static const Uint32 c_last_save_index_max_limit	= c_save_size - 1;	//<最終セーブデータインデックス上限(5)
#else //_WII
	static const Uint32 c_save_size	= 1;								//<セーブデータ数
#endif //_WII


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// SBackup::CreateInstance
	/*!
		作成
	 */
	// ============================================================================
	static SBackup &CreateInstance();

	// ============================================================================
	// SBackup::Init
	/*!
		初期化
	 */
	// ============================================================================
	void Init();


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// SBackup::GetSaveIndex
	/*!
		カレントセーブインデックスの取得

		@return 選択中のセーブインデックス
	 */
	// ============================================================================
	Uint32 GetSaveIndex() const {
#if _WII
		return m_last_save_index;
#else //_WII
		return 0;
#endif //_WII
	}

	// ============================================================================
	// SBackup::GetSystem
	/*!
		システムデータの取得

		@param	save_index	[in]	セーブインデックス
	 
		@return システムデータ
	 */
	// ============================================================================
	SSystem &GetSystem(Uint32 save_index) {
		return m_system[save_index];
	}
	const SSystem &GetSystem(Uint32 save_index) const {
		return m_system[save_index];
	}
	SSystem &GetSystem() {
		return GetSystem(GetSaveIndex());
	}
	const SSystem &GetSystem() const {
		return GetSystem(GetSaveIndex());
	}

	// ============================================================================
	// SBackup::GetOption
	/*!
		オプションデータの取得

		@param	save_index	[in]	セーブインデックス
	 
		@return オプションデータ
	 */
	// ============================================================================
	SOption &GetOption(Uint32 save_index) {
		return m_option[save_index];
	}
	const SOption &GetOption(Uint32 save_index) const {
		return m_option[save_index];
	}
	SOption &GetOption() {
		return GetOption(GetSaveIndex());
	}
	const SOption &GetOption() const {
		return GetOption(GetSaveIndex());
	}

	// ============================================================================
	// SBackup::GetStage
	/*!
		ステージデータの取得

		@param	save_index	[in]	セーブインデックス
	 
		@return ステージデータ
	 */
	// ============================================================================
	SStage &GetStage(Uint32 save_index) {
		return m_stage[save_index];
	}
	const SStage &GetStage(Uint32 save_index) const {
		return m_stage[save_index];
	}
	SStage &GetStage() {
		return GetStage(GetSaveIndex());
	}
	const SStage &GetStage() const {
		return GetStage(GetSaveIndex());
	}

	// ============================================================================
	// SBackup::GetSpecial
	/*!
		スペステデータの取得

		@param	save_index	[in]	セーブインデックス
	 
		@return スペステデータ
	 */
	// ============================================================================
	SSpecial &GetSpecial(Uint32 save_index) {
		return m_special[save_index];
	}
	const SSpecial &GetSpecial(Uint32 save_index) const {
		return m_special[save_index];
	}
	SSpecial &GetSpecial() {
		return GetSpecial(GetSaveIndex());
	}
	const SSpecial &GetSpecial() const {
		return GetSpecial(GetSaveIndex());
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// SBackup::SetSaveIndex
	/*!
		カレントセーブインデックス設定

		@param	選択中のセーブインデックス
	 */
	// ============================================================================
	void SetSaveIndex(Uint32 save_index) {
#if _WII
		m_last_save_index = save_index;
#else //_WII
		UNREFERENCED_PARAMETER(save_index);
#endif //_WII
	}


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	SSystem		m_system[c_save_size];	//<システムデータ
	SOption		m_option[c_save_size];	//<オプションデータ
	SStage		m_stage[c_save_size];	//<ステージデータ
	SSpecial	m_special[c_save_size];	//<スペステデータ

#if _WII
	struct {
		Uint32	m_last_save_index	:3;	//<最終セーブデータインデックス
		Uint32	m_reserve1			:29;
	};
#endif //_WII


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class SBackup




















} //namespace backup
} //namespace gs












//------------------------------------------------------------------------------**********
//ベターC化
typedef gs::backup::SBackup	GSS_BACKUP;
//------------------------------------------------------------------------------**********














#endif //#if	defined(__cplusplus)

	// ============================================================================
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
	// ============================================================================
