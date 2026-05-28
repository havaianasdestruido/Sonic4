// ============================================================================
/*!
	@file	gsBackupOption.hpp
	@brief	バックアップ・オプション

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: gsBackupOption.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page gsBackupOptionMain バックアップ・オプション

	@section gsBackupOptionSummary 概要
		バックアップのオプションを管理します。
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


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace gs {
namespace backup {









//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	バックアップ・オプションクラス
		バックアップのオプションを管理します。
 */
class SOption {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	enum {
		c_false					= 0,	//<bool in Sint32
		c_true					= 1,	//<bool in Sint32
		c_volume_bgm_max_limit	= 100,	//<BGMボリューム上限(100)
		c_volume_bgm_unit		= 10,	//<BGMボリューム単位
		c_volume_se_max_limit	= 100,	//<SEボリューム上限(100)
		c_volume_se_unit		= 10,	//<SEボリューム単位
		c_name_length_limit		= 10,	//<ユーザー名文字数上限
		c_name_length_limit_pad	= 16,	//<ユーザー名文字数上限(パディング付)
	};
	
#if _IPHONE
	//コントロール方法
	struct EControl {
		enum Type {
			Tilt,			//<傾斜
			VirtualPadDown,	//<バーチャルパッド(下)
			VirtualPadUp,	//<バーチャルパッド(上)
			
			Max,
			None
		};
	};
#endif //_IPHONE


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
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
	static SOption &CreateInstance(Uint32 save_index);
	static SOption &CreateInstance();

	// ============================================================================
	// SOption::Init
	/*!
		初期化
	 */
	// ============================================================================
	void Init();


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// SOption::IsVibration
	/*!
		振動の取得
	 
		@retval	true	振動する
		@retval	true	振動しない
	 */
	// ============================================================================
	bool IsVibration() const {
		return ((c_false != m_is_vibration)? true: false);
	}

	// ============================================================================
	// SOption::GetVolumeBgm
	/*!
		BGMボリュームの取得
	 
		@return BGMボリューム
	 */
	// ============================================================================
	Uint32 GetVolumeBgm() const {
		return m_volume_bgm * c_volume_bgm_unit;
	}

	// ============================================================================
	// SOption::GetVolumeSe
	/*!
		SEボリュームの取得
	 
		@return SEボリューム
	 */
	// ============================================================================
	Uint32 GetVolumeSe() const {
		return m_volume_se * c_volume_se_unit;
	}

#if _WII
	// ============================================================================
	// SOption::GetName
	/*!
		ユーザー名の取得
	 
		@return ユーザー名(ASCIIコード)
	 */
	// ============================================================================
	const char* GetName() const {
		return m_name;
	};
#endif // _WII

#if _IPHONE
	// ============================================================================
	// SOption::GetControl
	/*!
		コントロール方法の取得
	 
		@return コントロール方法
	 */
	// ============================================================================
	EControl::Type GetControl() const {
		return static_cast<EControl::Type>(m_control);
	};
#endif //_IPHONE


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// SOption::SetVibration
	/*!
		振動設定
	 
		@param	is_vibration	[in]	振動するか
	 */
	// ============================================================================
	void SetVibration(bool is_vibration);

	// ============================================================================
	// SOption::SetVolumeBgm
	/*!
		BGMボリューム設定
	 
		@param	volume_bgm	[in]	BGMボリューム
	 */
	// ============================================================================
	void SetVolumeBgm(Uint32 volume_bgm);

	// ============================================================================
	// SOption::SetVolumeSe
	/*!
		SEボリューム設定
	 
		@param	volume_se	[in]	SEボリューム
	 */
	// ============================================================================
	void SetVolumeSe(Uint32 volume_se);

#if _WII
	// ============================================================================
	// SOption::SetName
	/*!
		ユーザ名設定
	 
		@param	name		[in]	ユーザ名(ASCIIコード)
	 */
	// ============================================================================
	void SetName(const char* name);
#endif // _WII

#if _IPHONE
	// ============================================================================
	// SOption::SetControl
	/*!
		コントロール方法の設定
	 
		@return コントロール方法
	 */
	// ============================================================================
	void SetControl(EControl::Type control);
#endif //_IPHONE


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	struct {
		Uint32	m_is_vibration	:1;	//<振動
		Uint32	m_volume_bgm	:4;	//<BGMボリューム
		Uint32	m_volume_se		:4;	//<SEボリューム
#if !_IPHONE
		Uint32	m_reserve1		:23;
#else //!_IPHONE
		Uint32	m_control		:2;	//<コントロール方法
		Uint32	m_reserve1		:21;
#endif //!_IPHONE
	};

#if _WII
	char	m_name[c_name_length_limit_pad];	//<ユーザー名
#endif //_WII


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class SOption






















} //namespace backup
} //namespace gs
#endif //#if	defined(__cplusplus)

	// ============================================================================
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
	// ============================================================================
