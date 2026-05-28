// ============================================================================
/*!
	@file	gsBackupStage.hpp
	@brief	バックアップ・ステージ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: gsBackupStage.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page gsBackupStageMain バックアップ・ステージ

	@section gsBackupStageSummary 概要
		バックアップのステージを管理します。
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
	@brief	バックアップ・ステージ単体クラス
		バックアップのステージ単体を管理します。
 */
class SStageSolo {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	enum {
		c_false					= 0,		//<bool in Sint32
		c_true					= 1,		//<bool in Sint32
		c_high_score_max_limit	= 1000000000,	//<ハイスコア上限(10,000,000)
		c_high_score_unit		= 10,		//<ハイスコア単位
		c_fast_time_max_limit	= 36000,	//<最速タイム上限(10'00"00)
	};

	//レコードの種類
	struct ERecordKind {
		enum Type {
			Sonic,		//<ソニック単体
			SuperSonic,	//<スーパーソニック併用

			Max,
			None,
		};
	};


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// SStageSolo::Init
	/*!
		初期化
	 */
	// ============================================================================
	void Init();


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// SStageSolo::IsNew
	/*!
		NEW状態確認
	 
		@retval	true	NEW状態
		@retval	false	NEW状態ではない
	 */
	// ============================================================================
	bool IsNew() const {
		return ((c_false != m_is_new)? true: false);
	}

	// ============================================================================
	// SStageSolo::IsHighScoreEnable
	/*!
		ハイスコアは有効か
	 
		@param	is_supersonic	[in]	スーパーソニックか
		@retval	true	有効
		@retval	false	無効
	 */
	// ============================================================================
	bool IsHighScoreEnable(bool is_supersonic) const;

	// ============================================================================
	// SStageSolo::IsFastTimeEnable
	/*!
		最速タイムは有効か
	 
		@param	is_supersonic	[in]	スーパーソニックか
		@retval	true	有効
		@retval	false	無効
	 */
	// ============================================================================
	bool IsFastTimeEnable(bool is_supersonic) const;

	// ============================================================================
	// SStageSolo::GetHighScore
	/*!
		ハイスコアの取得
	 
		@param	is_supersonic	[in]	スーパーソニックか
		@return	ハイスコア
	 */
	// ============================================================================
	Uint32 GetHighScore(bool is_supersonic) const;

	// ============================================================================
	// SStageSolo::GetFastTime
	/*!
		最速タイムの取得
	 
		@param	is_supersonic	[in]	スーパーソニックか
		@return	最速タイム
	 */
	// ============================================================================
	Uint32 GetFastTime(bool is_supersonic) const;

	// ============================================================================
	// SStageSolo::IsHighScoreUploaded
	/*!
		ハイスコアはアップロード済みか
	 
		@param	is_supersonic	[in]	スーパーソニックか
		@retval	true	アップロード済み
		@retval	false	未アップロード
	 */
	// ============================================================================
	bool IsHighScoreUploaded(bool is_supersonic) const;

	// ============================================================================
	// SStageSolo::IsFastTimeUploaded
	/*!
		最速タイムはアップロード済みか
	 
		@param	is_supersonic	[in]	スーパーソニックか
		@retval	true	アップロード済み
		@retval	false	未アップロード
	 */
	// ============================================================================
	bool IsFastTimeUploaded(bool is_supersonic) const;

	// ============================================================================
	// SStageSolo::IsHighScoreUseSuperSonic
	/*!
		ハイスコアはスーパーソニック使用か
	 
		@retval	true	スーパーソニック使用
		@retval	false	ソニックのみ
	 */
	// ============================================================================
	bool IsHighScoreUseSuperSonic() const {
		return ((c_false != m_is_high_score_use_supersonic)? true: false);
	}

	// ============================================================================
	// SStageSolo::IsFastTimeUseSuperSonic
	/*!
		最速タイムはスーパーソニック使用か
	 
		@retval	true	スーパーソニック使用
		@retval	false	ソニックのみ
	 */
	// ============================================================================
	bool IsFastTimeUseSuperSonic() const {
		return ((c_false != m_is_fast_time_use_supersonic)? true: false);
	}

	// ============================================================================
	// SStageSolo::IsScoreUploadedOnce
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
	// SStageSolo::IsTimeUploadedOnce
	/*!
		過去1回はタイムレコードをアップロード済みか
	 
		@retval	true	アップロード済み
		@retval	false	未アップロード
	 */
	// ============================================================================
	bool IsTimeUploadedOnce() const {
		return ((c_false != m_is_time_uploaded_once)? true: false);
	}

	// ============================================================================
	// SStageSolo::IsUseSuperSonicOnce
	/*!
		過去1回はスーパーソニックでゴールパネルを通過したか
	 
		@retval	true	スーパーソニック使用
		@retval	false	ソニックのみ
	 */
	// ============================================================================
	bool IsUseSuperSonicOnce() const {
		return ((c_false != m_is_use_supersonic_once)? true: false);
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// SStageSolo::SetNew
	/*!
		NEW状態設定
	 
		@param	is_new	[in]	NEW状態か
	 */
	// ============================================================================
	void SetNew(bool is_new = false);

	// ============================================================================
	// SStageSolo::SetHighScore
	/*!
		ハイスコア設定
	 
		@param	high_score			[in]	ハイスコア
		@param is_use_supersonic	[in]	スーパーソニック使用か
	 */
	// ============================================================================
	void SetHighScore(Uint32 high_score, bool is_use_supersonic);

	// ============================================================================
	// SStageSolo::SetFastTime
	/*!
		最速タイム設定
	 
		@param	fast_time			[in]	最速タイム
		@param is_use_supersonic	[in]	スーパーソニック使用か
	 */
	// ============================================================================
	void SetFastTime(Uint32 fast_time, bool is_use_supersonic);

	// ============================================================================
	// SStageSolo::SetHighScoreUploaded
	/*!
		ハイスコアのアップロード設定
	 
		@param	is_supersonic	[in]	スーパーソニックか
		@param	is_uploaded		[in]	アップロード済みか
	 */
	// ============================================================================
	void SetHighScoreUploaded(bool is_supersonic, bool is_uploaded = true);

	// ============================================================================
	// SStageSolo::SetFastTimeUploaded
	/*!
		最速タイムのアップロード設定
	 
		@param	is_supersonic	[in]	スーパーソニックか
		@param	is_uploaded		[in]	アップロード済みか
	 */
	// ============================================================================
	void SetFastTimeUploaded(bool is_supersonic, bool is_uploaded = true);

	// ============================================================================
	// SStageSolo::SetScoreUploadedOnce
	/*!
		過去1回はスコアをアップロード済み設定
	 
		@param is_uploaded_once	[in]	過去1回はアップロード済みか
	 */
	// ============================================================================
	void SetScoreUploadedOnce(bool is_uploaded_once);

	// ============================================================================
	// SStageSolo::SetTimeUploadedOnce
	/*!
		過去1回はタイムレコードをアップロード済み設定
	 
		@param is_uploaded_once	[in]	過去1回はアップロード済みか
	 */
	// ============================================================================
	void SetTimeUploadedOnce(bool is_uploaded_once);

	// ============================================================================
	// SStageSolo::SetUseSuperSonicOnce
	/*!
		過去1回はスーパーソニックでゴールパネルを通過した設定
	 
		@param is_use_supersonic_once	[in]	スーパーソニック使用か
	 */
	// ============================================================================
	void SetUseSuperSonicOnce(bool is_use_supersonic_once);

#if defined(MTD_DEBUG)
	// ============================================================================
	// SStageSolo::SetHighScoreEnable
	/*!
		ハイスコア有効設定
	 
		@param	is_supersonic	[in]	スーパーソニックか
		@param	is_enable		[in]	ハイスコアは有効か
	 */
	// ============================================================================
	void SetHighScoreEnable(bool is_supersonic, bool is_enable);
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
	void SetFastTimeEnable(bool is_supersonic, bool is_enable);
#endif //defined(MTD_DEBUG)

#if defined(MTD_DEBUG)
	// ============================================================================
	// SStageSolo::SetUseSuperSonicOnce
	/*!
		ハイスコアはスーパーソニック使用設定
	 
		@param is_use_supersonic	[in]	スーパーソニック使用か
	 */
	// ============================================================================
	void SetHighScoreUseSuperSonic(bool is_use_supersonic);
#endif //defined(MTD_DEBUG)

#if defined(MTD_DEBUG)
	// ============================================================================
	// SStageSolo::SetFastTimeUseSuperSonic
	/*!
		最速タイムはスーパーソニック使用設定
	 
		@param is_use_supersonic	[in]	スーパーソニック使用か
	 */
	// ============================================================================
	void SetFastTimeUseSuperSonic(bool is_use_supersonic);
#endif //defined(MTD_DEBUG)


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//レコード
	struct SRecord {
		struct {
			Uint32	high_score				:32;	//<ハイスコア(0～100,000)
			Uint32	fast_time				:16;	//<最速タイム(0'00"00～10'00"00)
			Uint32	is_high_score_enable	:1;		//<ハイスコアは有効か
			Uint32	is_fast_time_enable		:1;		//<最速タイムは有効か
		};
		struct {
			Uint32	is_high_score_uploaded	:1;		//<ハイスコアはアップロード済みか
			Uint32	is_fast_time_uploaded	:1;		//<最速タイムはアップロード済みか
			Uint32	reserve1				:12;
		};
	};


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	SRecord		m_record[ERecordKind::Max];				//<レコード
	struct {
		Uint32	m_is_new						:1;		//<NEW状態か
		Uint32	m_is_high_score_use_supersonic	:1;		//<ハイスコアはスーパーソニック使用か
		Uint32	m_is_fast_time_use_supersonic	:1;		//<最速タイムはスーパーソニック使用か
		Uint32	m_is_score_uploaded_once		:1;		//<過去1回はスコアをアップロード済みか
		Uint32	m_is_time_uploaded_once			:1;		//<過去1回はタイムレコードをアップロード済みか
		Uint32	m_is_use_supersonic_once		:1;		//<過去1回はスーパーソニックでゴールパネルを通過したか
		Uint32	m_reserve1						:26;
	};


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	// ============================================================================
	// SStageSolo::getRecord
	/*!
		レコード取得
	 
		@param	record_kind		[in]	レコードの種類
		@param	is_supersonic	[in]	スーパーソニックか

		@return 記録
	 */
	// ============================================================================
	SRecord &getRecord(ERecordKind::Type record_kind) {
		return m_record[record_kind];
	};
	SRecord &getRecord(bool is_supersonic) {
		return getRecord((is_supersonic)? ERecordKind::SuperSonic: ERecordKind::Sonic);
	};
	const SRecord &getRecord(ERecordKind::Type record_kind) const {
		return m_record[record_kind];
	};
	const SRecord &getRecord(bool is_supersonic) const {
		return getRecord((is_supersonic)? ERecordKind::SuperSonic: ERecordKind::Sonic);
	};


//------------------------------------------------------------------------------**********
}; //class SStageSolo




















//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	バックアップ・ステージクラス
		バックアップのステージを管理します。
 */
class SStage {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	static const Uint32 c_size = EStage::Max;	//<ステージ数


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
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
	static SStage &CreateInstance(Uint32 save_index);
	static SStage &CreateInstance();

	// ============================================================================
	// SStage::Init
	/*!
		初期化
	 */
	// ============================================================================
	void Init();

	// ============================================================================
	// SStage::operator[]
	/*!
		インデクサ
	 */
	// ============================================================================
	SStageSolo &operator[](Uint32 index) {
		return m_stage[index];
	}
	const SStageSolo &operator[](Uint32 index) const {
		return m_stage[index];
	}
	SStageSolo &operator[](EStage::Type index) {
		return operator[](static_cast<Uint32>(index));
	}
	const SStageSolo &operator[](EStage::Type index) const {
		return operator[](static_cast<Uint32>(index));
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// SStage::GetSize
	/*!
		ステージ数の取得
	
		@return ステージ数
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
	SStageSolo	m_stage[c_size];	//<ステージデータ


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class SStage






















} //namespace backup
} //namespace gs
#endif //#if	defined(__cplusplus)

	// ============================================================================
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
	// ============================================================================
