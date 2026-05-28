// ============================================================================
/*!
	@file	gsBackupSpecial.hpp
	@brief	バックアップ・スペステ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: gsBackupSpecial.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page gsBackupSpecialMain バックアップ・スペステ

	@section gsBackupSpecialSummary 概要
		バックアップのスペステを管理します。
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
	@brief	バックアップ・スペステ単体クラス
		バックアップのスペステ単体を管理します。
 */
class SSpecialSolo {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	enum {
		c_false					= 0,		//<bool in Sint32
		c_true					= 1,		//<bool in Sint32
		c_high_score_max_limit	= 1000000000,	//<ハイスコア上限(1,000,000,000)
		c_high_score_unit		= 10,		//<ハイスコア単位
		c_fast_time_max_limit	= 36000,	//<最速タイム上限(10'00"00)
	};

	///エメラルド獲得ステージ
	struct EEmeraldStage {
		enum Type {
			Null,		//<未所持
			Zone1Act1,	//<ゾーン1アクト1
			Zone1Act2,	//<ゾーン1アクト2
			Zone1Act3,	//<ゾーン1アクト3
			Zone2Act1,	//<ゾーン2アクト1
			Zone2Act2,	//<ゾーン2アクト2
			Zone2Act3,	//<ゾーン2アクト3
			Zone3Act1,	//<ゾーン3アクト1
			Zone3Act2,	//<ゾーン3アクト2
			Zone3Act3,	//<ゾーン3アクト3
			Zone4Act1,	//<ゾーン4アクト1
			Zone4Act2,	//<ゾーン4アクト2
			Zone4Act3,	//<ゾーン4アクト3
			
			Max,
			None,
		};
	};


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// SSpecialSolo::Init
	/*!
		初期化
	 */
	// ============================================================================
	void Init();


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// SSpecialSolo::IsNew
	/*!
		NEW状態確認
	 
		@retval	true	NEW状態
		@retval	false	NEW状態ではない
	 */
	// ============================================================================
	bool IsNew() const {
		return false;
	}

	// ============================================================================
	// SStageSolo::IsHighScoreEnable
	/*!
		ハイスコアは有効か
	 
		@retval	true	有効
		@retval	false	無効
	 */
	// ============================================================================
	bool IsHighScoreEnable() const {
		return ((c_false != m_is_high_score_enable)? true: false);
	}

	// ============================================================================
	// SStageSolo::IsFastTimeEnable
	/*!
		最速タイムは有効か
	 
		@retval	true	有効
		@retval	false	無効
	 */
	// ============================================================================
	bool IsFastTimeEnable() const {
		return ((c_false != m_is_fast_time_enable)? true: false);
	}

	// ============================================================================
	// SSpecialSolo::GetHighScore
	/*!
		ハイスコアの取得
	 
		@return	ハイスコア
	 */
	// ============================================================================
	Uint32 GetHighScore() const {
		return m_high_score * c_high_score_unit;
	}

	// ============================================================================
	// SStageSolo::GetFastTime
	/*!
		最速タイムの取得
	 
		@return	最速タイム
	 */
	// ============================================================================
	Uint32 GetFastTime() const {
		return m_fast_time;
	}

	// ============================================================================
	// SStageSolo::IsHighScoreUploaded
	/*!
		ハイスコアはアップロード済みか
	 
		@retval	true	アップロード済み
		@retval	false	未アップロード
	 */
	// ============================================================================
	bool IsHighScoreUploaded() const {
		return ((c_false != m_is_high_score_uploaded)? true: false);
	}

	// ============================================================================
	// SStageSolo::IsFastTimeUploaded
	/*!
		最速タイムはアップロード済みか
	 
		@retval	true	アップロード済み
		@retval	false	未アップロード
	 */
	// ============================================================================
	bool IsFastTimeUploaded() const {
		return ((c_false != m_is_fast_time_uploaded)? true: false);
	}

	// ============================================================================
	// SSpecialSolo::IsGetEmerald
	/*!
		エメラルド獲得ステージ確認
	 
		@retval	true	獲得済み
		@retval	false	未獲得
	 */
	// ============================================================================
	bool IsGetEmerald() const {
		return ((0 != m_emerald_stage)? true: false);
	}

	// ============================================================================
	// SSpecialSolo::GetEmeraldStage
	/*!
		エメラルド獲得ステージの取得
	 
		@return	エメラルド獲得ステージ
	 */
	// ============================================================================
	EEmeraldStage::Type GetEmeraldStage() const {
		return static_cast<EEmeraldStage::Type>(m_emerald_stage);
	}

	// ============================================================================
	// SSpecialSolo::IsScoreUploadedOnce
	/*!
		過去1回はスコアをアップロード済みか
	 
		@retval	true	アップロード済み
		@retval	false	未アップロード
	 */
	// ============================================================================
	bool IsScoreUploadedOnce() const {
		return ((c_false != m_is_score_uploaded_once)? true: false);
	}

	// ============================================================================
	// SSpecialSolo::IsTimeUploadedOnce
	/*!
		過去1回はタイムレコードをアップロード済みか
	 
		@retval	true	アップロード済み
		@retval	false	未アップロード
	 */
	// ============================================================================
	bool IsTimeUploadedOnce() const {
		return ((c_false != m_is_time_uploaded_once)? true: false);
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// SSpecialSolo::SetNew
	/*!
		NEW状態設定
	 
		@param	is_new	[in]	NEW状態か
	 */
	// ============================================================================
	void SetNew(bool = false);

	// ============================================================================
	// SSpecialSolo::SetHighScore
	/*!
		ハイスコア設定
	 
		@param	high_score	[in]	ハイスコア
	 */
	// ============================================================================
	void SetHighScore(Uint32 high_score);

	// ============================================================================
	// SStageSolo::SetFastTime
	/*!
		最速タイム設定
	 
		@param	fast_time			[in]	最速タイム
	 */
	// ============================================================================
	void SetFastTime(Uint32 fast_time);

	// ============================================================================
	// SStageSolo::SetHighScoreUploaded
	/*!
		ハイスコアのアップロード設定
	 
		@param	is_uploaded	[in]	アップロード済みか
	 */
	// ============================================================================
	void SetHighScoreUploaded(bool is_uploaded = true);

	// ============================================================================
	// SStageSolo::SetFastTimeUploaded
	/*!
		最速タイムのアップロード設定
	 
		@param	is_uploaded	[in]	アップロード済みか
	 */
	// ============================================================================
	void SetFastTimeUploaded(bool is_uploaded = true);

	// ============================================================================
	// SSpecialSolo::SetEmeraldStage
	/*!
		エメラルド獲得ステージ設定
	 
		@param	stage	[in]	ステージ
	 */
	// ============================================================================
	void SetEmeraldStage(EEmeraldStage::Type emerald_stage);

	// ============================================================================
	// SSpecialSolo::SetScoreUploadedOnce
	/*!
		過去1回はスコアをアップロード済み設定
	 
		@param is_uploaded_once	[in]	過去1回はアップロード済みか
	 */
	// ============================================================================
	void SetScoreUploadedOnce(bool is_uploaded_once);

	// ============================================================================
	// SSpecialSolo::SetTimeUploadedOnce
	/*!
		過去1回はタイムレコードをアップロード済み設定
	 
		@param is_uploaded_once	[in]	過去1回はアップロード済みか
	 */
	// ============================================================================
	void SetTimeUploadedOnce(bool is_uploaded_once);

#if defined(MTD_DEBUG)
	// ============================================================================
	// SSpecialSolo::SetHighScoreEnable
	/*!
		ハイスコア有効設定
	 
		@param is_enable	[in]	ハイスコアは有効か
	 */
	// ============================================================================
	void SetHighScoreEnable(bool is_enable);
#endif //defined(MTD_DEBUG)

#if defined(MTD_DEBUG)
	// ============================================================================
	// SSpecialSolo::SetFastTimeEnable
	/*!
		最速タイム有効設定
	 
		@param is_enable	[in]	最速タイムは有効か
	 */
	// ============================================================================
	void SetFastTimeEnable(bool is_enable);
#endif //defined(MTD_DEBUG)


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	struct {
		Uint32	m_high_score					:32;	//<ハイスコア(0～100,000)
		Uint32	m_fast_time						:16;	//<最速タイム(0'00"00～10'00"00)
		Uint32	m_is_high_score_enable			:1;		//<ハイスコアは有効か
		Uint32	m_is_fast_time_enable			:1;		//<最速タイムは有効か
	};
	struct {
		Uint32	m_is_high_score_uploaded		:1;		//<ハイスコアはアップロード済みか
		Uint32	m_is_fast_time_uploaded			:1;		//<最速タイムはアップロード済みか
		Uint32	m_emerald_stage					:4;		//<エメラルド獲得ステージ
		Uint32	m_is_score_uploaded_once		:1;		//<過去1回はスコアをアップロード済みか
		Uint32	m_is_time_uploaded_once			:1;		//<過去1回はタイムレコードをアップロード済みか
		Uint32	m_reserve1						:24;
	};


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class SSpecialSolo




















//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	バックアップ・スペステクラス
		バックアップのスペステを管理します。
 */
class SSpecial {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	static const Uint32 c_size = ESpecialStage::Max;	//<スペステ数


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
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
	static SSpecial &CreateInstance(Uint32 save_index);
	static SSpecial &CreateInstance();

	// ============================================================================
	// SSpecial::Init
	/*!
		初期化
	 */
	// ============================================================================
	void Init();

	// ============================================================================
	// SSpecial::operator[]
	/*!
		インデクサ
	 */
	// ============================================================================
	SSpecialSolo &operator[](Uint32 index) {
		return m_stage[index];
	}
	const SSpecialSolo &operator[](Uint32 index) const {
		return m_stage[index];
	}
	SSpecialSolo &operator[](EStage::Type index) {
		return operator[](static_cast<Uint32>(index));
	}
	const SSpecialSolo &operator[](EStage::Type index) const {
		return operator[](static_cast<Uint32>(index));
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// SSpecial::GetSize
	/*!
		スペステ数の取得
	
		@return スペステ数
	 */
	// ============================================================================
	static Uint32 GetSize() {
		return c_size;
	}

	
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	SSpecialSolo	m_stage[c_size];	//<スペステデータ


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class SSpecial






















} //namespace backup
} //namespace gs
#endif //#if	defined(__cplusplus)

	// ============================================================================
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
	// ============================================================================
