// ============================================================================
/*!
	@file	dbgSaveDataEditor.hpp
	@brief	セーブデータエディタ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: dbgSaveDataEditor.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page dbgSaveDataEditorMain セーブデータエディタ

	@section dbgSaveDataEditorSummary 概要
		セーブデータエディタを提供します。
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
#include "dbgEvtSelector.hpp"
#include "erSmallInt.hpp"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace dbg {


#if _WII
#pragma warn_notinlined off	//インライン展開出来無い関数に対する警告メッセージの無効化
#endif //_WII









//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	エディタインターフェース
		エディタ機能のインターフェースを提供します。
 */
class IEditor {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// IEditor::Focus
	/*!
		選択中の処理

		@param	cb	[io]	コールバックデータ

		@retval	true	終了
		@retval	false	継続
	 */
	// ==========================================================================
	virtual bool Focus(CEvtBase &cb) = 0;

	// ============================================================================
	// IEditor::Blur
	/*!
		非選択中の処理

		@param	cb	[io]	コールバックデータ
	 */
	// ============================================================================
	virtual void Blur(CEvtBase &cb) = 0;

	// =============================================================================
	// IEditor::Full
	/*!
		最強化

		@param	cb	[io]	コールバックデータ
	 */
	// ==========================================================================
	virtual void Full(CEvtBase &cb) = 0;

	// ============================================================================
	// IEditor::Reset
	/*!
		白紙化

		@param	cb	[io]	コールバックデータ
	 */
	// ============================================================================
	virtual void Reset(CEvtBase &cb) = 0;


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// IEditor::GetTitle
	/*!
		タイトルの取得

		@return	タイトル
	 */
	// ============================================================================
	virtual const char *GetTitle() = 0;


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// IEditor::IEditor
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	IEditor() {};

	// ============================================================================
	// IEditor::~IEditor
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~IEditor() {};


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class IEditor




































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	セーブIOエディタ
		セーブIOを提供するクラスです。
 */
class CIoEditor : public IEditor {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef IEditor	super_type;

	
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CIoEditor::Focus
	/*!
		選択中の処理

		@param	cb	[io]	コールバックデータ

		@retval	true	終了
		@retval	false	継続
	 */
	// ==========================================================================
	virtual bool Focus(CEvtBase &cb);

	// ============================================================================
	// CIoEditor::Blur
	/*!
		非選択中の処理

		@param	cb	[io]	コールバックデータ
	 */
	// ============================================================================
	virtual void Blur(CEvtBase &cb);

	// =============================================================================
	// CIoEditor::Full
	/*!
		最強化

		@param	cb	[io]	コールバックデータ
	 */
	// ==========================================================================
	virtual void Full(CEvtBase &cb);

	// ============================================================================
	// CIoEditor::Reset
	/*!
		白紙化

		@param	cb	[io]	コールバックデータ
	 */
	// ============================================================================
	virtual void Reset(CEvtBase &cb);


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CIoEditor::GetTitle
	/*!
		タイトルの取得

		@return	タイトル
	 */
	// ============================================================================
	virtual const char *GetTitle() {
		return "Save IO Test";
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CIoEditor::CIoEditor
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CIoEditor();


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//項目
	struct EElement {
		enum Type {
			Load,			//<読み込み
			Save,			//<書き込み
			FirstSave,		//<書き込み(初回)
			ErrorClear,		//<エラークリア
#if _WII || _IPHONE
			Delete,			//<削除
#endif //_WII || _IPHONE

			Max,
			None,
		};
	};


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	er::CSmallInt<>	m_crsr;


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	bool update(CEvtBase &cb);
	void print(CEvtBase &cb, bool is_enable = true);


//------------------------------------------------------------------------------**********
}; //class CIoEditor




































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	システムエディタ
		システムデータを改変するクラスです。
 */
class CSystemEditor : public IEditor {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef IEditor	super_type;

	
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CSystemEditor::Focus
	/*!
		選択中の処理

		@param	cb	[io]	コールバックデータ

		@retval	true	終了
		@retval	false	継続
	 */
	// ==========================================================================
	virtual bool Focus(CEvtBase &cb);

	// ============================================================================
	// CSystemEditor::Blur
	/*!
		非選択中の処理

		@param	cb	[io]	コールバックデータ
	 */
	// ============================================================================
	virtual void Blur(CEvtBase &cb);

	// =============================================================================
	// CSystemEditor::Full
	/*!
		最強化

		@param	cb	[io]	コールバックデータ
	 */
	// ==========================================================================
	virtual void Full(CEvtBase &cb);

	// ============================================================================
	// CSystemEditor::Reset
	/*!
		白紙化

		@param	cb	[io]	コールバックデータ
	 */
	// ============================================================================
	virtual void Reset(CEvtBase &cb);


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CSystemEditor::GetTitle
	/*!
		タイトルの取得

		@return	タイトル
	 */
	// ============================================================================
	virtual const char *GetTitle() {
		return "System";
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CSystemEditor::CSystemEditor
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CSystemEditor();


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//項目
	struct EElement {
		enum Type {
			PlayerStock,				//<残機
#if _WII
			LastClearAct,				//<最終クリアアクト
			LastSaveChrono,				//<最終セーブ時刻
#endif // _WII
			AnnounceOpenZoneSelect,		//<アナウンス・ゾーンセレクト開放
			AnnounceOpenZone1Boss,		//<アナウンス・ゾーン1ボス開放
			AnnounceOpenZone2Boss,		//<アナウンス・ゾーン2ボス開放
			AnnounceOpenZone3Boss,		//<アナウンス・ゾーン3ボス開放
			AnnounceOpenZone4Boss,		//<アナウンス・ゾーン4ボス開放
			AnnounceOpenFinalZone,		//<アナウンス・ファイナルゾーン開放
			AnnounceOpenSuperSonic,		//<アナウンス・スーパーソニック
			AnnounceOpenSpecialStage,	//<アナウンス・スペステゾーン開放
#if _IPHONE
			AnnounceTruckTilt,			//<アナウンス・トロッコステージ・傾斜操作メッセージ
			AnnounceTruckFlick,			//<アナウンス・トロッコステージ・フリック操作メッセージ
			AnnounceSpecialStageTilt,	//<アナウンス・スペシャルステージ・傾斜操作メッセージ
			AnnounceSpecialStageFlick,	//<アナウンス・スペシャルステージ・フリック操作メッセージ
#endif //_IPHONE
			Killed,						//<累計エネミー撃退数
			ClearCount,					//<ゲームクリア回数

			Max,
			None,
		};
	};


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	er::CSmallInt<>	m_crsr;


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	bool update(CEvtBase &cb);
	void print(CEvtBase &cb, bool is_enable = true);


//------------------------------------------------------------------------------**********
}; //class CSystemEditor














































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	スペシャルステージエディタ
		スペシャルステージデータを改変するクラスです。
 */
class CStageEditor : public IEditor {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef IEditor	super_type;

	
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CStageEditor::Focus
	/*!
		選択中の処理

		@param	cb	[io]	コールバックデータ

		@retval	true	終了
		@retval	false	継続
	 */
	// ==========================================================================
	virtual bool Focus(CEvtBase &cb);

	// ============================================================================
	// CStageEditor::Blur
	/*!
		非選択中の処理

		@param	cb	[io]	コールバックデータ
	 */
	// ============================================================================
	virtual void Blur(CEvtBase &cb);

	// =============================================================================
	// CStageEditor::Full
	/*!
		最強化

		@param	cb	[io]	コールバックデータ
	 */
	// ==========================================================================
	virtual void Full(CEvtBase &cb);

	// ============================================================================
	// CStageEditor::Reset
	/*!
		白紙化

		@param	cb	[io]	コールバックデータ
	 */
	// ============================================================================
	virtual void Reset(CEvtBase &cb);


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CStageEditor::GetTitle
	/*!
		タイトルの取得

		@return	タイトル
	 */
	// ============================================================================
	virtual const char *GetTitle() {
		return "Stage";
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CStageEditor::CStageEditor
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CStageEditor();


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//項目
	struct EElement {
		enum Type {
			New,					//<NEW状態
			HighScore,				//<ハイスコア
			FastTime,				//<最速タイム
			HighScoreS,				//<ハイスコア(スーパーソニック使用)
			FastTimeS,				//<最速タイム(スーパーソニック使用)
			HighScoreUploaded,		//<ハイスコアはアップロード済み
			FastTimeUploaded,		//<最速タイムはアップロード済み
			HighScoreUploadedS,		//<ハイスコアはアップロード済み(スーパーソニック使用)
			FastTimeUploadedS,		//<最速タイムはアップロード済み(スーパーソニック使用)
			HighScoreUseSuperSonic,	//<ハイスコアはスーパーソニック使用
			FastTimeUseSuperSonic,	//<最速タイムはスーパーソニック使用
			ScoreUploadedOnce,		//<過去1回はスコアをアップロード済みか
			TimeUploadedOnce,		//<過去1回はタイムをアップロード済みか
			UseSuperSonicOnce,		//<過去1回はスーパーソニック使用か

			Max,
			None,
		};
	};


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	er::CSmallInt<>	m_crsr;


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	bool update(CEvtBase &cb);
	void print(CEvtBase &cb, bool is_enable = true);


//------------------------------------------------------------------------------**********
}; //class CStageEditor














































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	スペシャルステージエディタ
		スペシャルステージデータを改変するクラスです。
 */
class CSpecialEditor : public IEditor {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef IEditor	super_type;

	
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CSpecialEditor::Focus
	/*!
		選択中の処理

		@param	cb	[io]	コールバックデータ

		@retval	true	終了
		@retval	false	継続
	 */
	// ==========================================================================
	virtual bool Focus(CEvtBase &cb);

	// ============================================================================
	// CSpecialEditor::Blur
	/*!
		非選択中の処理

		@param	cb	[io]	コールバックデータ
	 */
	// ============================================================================
	virtual void Blur(CEvtBase &cb);

	// =============================================================================
	// CSpecialEditor::Full
	/*!
		最強化

		@param	cb	[io]	コールバックデータ
	 */
	// ==========================================================================
	virtual void Full(CEvtBase &cb);

	// ============================================================================
	// CSpecialEditor::Reset
	/*!
		白紙化

		@param	cb	[io]	コールバックデータ
	 */
	// ============================================================================
	virtual void Reset(CEvtBase &cb);


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CSpecialEditor::GetTitle
	/*!
		タイトルの取得

		@return	タイトル
	 */
	// ============================================================================
	virtual const char *GetTitle() {
		return "Special stage";
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CSpecialEditor::CSpecialEditor
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CSpecialEditor();


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//項目
	struct EElement {
		enum Type {
			HighScore,			//<ハイスコア
			GetEmeraldStage,	//<エメラルド獲得ステージ

			Max,
			None,
		};
	};


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	er::CSmallInt<>	m_crsr;


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	bool update(CEvtBase &cb);
	void print(CEvtBase &cb, bool is_enable = true);


//------------------------------------------------------------------------------**********
}; //class CSpecialEditor














































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	オプションエディタ
		オプションデータを改変するクラスです。
 */
class COptionEditor : public IEditor {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef IEditor	super_type;

	
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// COptionEditor::Focus
	/*!
		選択中の処理

		@param	cb	[io]	コールバックデータ

		@retval	true	終了
		@retval	false	継続
	 */
	// ==========================================================================
	virtual bool Focus(CEvtBase &cb);

	// ============================================================================
	// COptionEditor::Blur
	/*!
		非選択中の処理

		@param	cb	[io]	コールバックデータ
	 */
	// ============================================================================
	virtual void Blur(CEvtBase &cb);

	// =============================================================================
	// COptionEditor::Full
	/*!
		最強化

		@param	cb	[io]	コールバックデータ
	 */
	// ==========================================================================
	virtual void Full(CEvtBase &cb);

	// ============================================================================
	// COptionEditor::Reset
	/*!
		白紙化

		@param	cb	[io]	コールバックデータ
	 */
	// ============================================================================
	virtual void Reset(CEvtBase &cb);


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// COptionEditor::GetTitle
	/*!
		タイトルの取得

		@return	タイトル
	 */
	// ============================================================================
	virtual const char *GetTitle() {
		return "Option";
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// COptionEditor::COptionEditor
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	COptionEditor();


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//項目
	struct EElement {
		enum Type {
			Vibration,		//<振動
			VolumeBgm,		//<ボリューム・BGM
			VolumeSe,		//<ボリューム・SE
#if _IPHONE
			Control,		//<コントロール方法
#endif //_IPHONE
#if _WII
			UserName,		//<ユーザー名
#endif // _WII

			Max,
			None,
		};
	};


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	er::CSmallInt<>	m_crsr;


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	bool update(CEvtBase &cb);
	void print(CEvtBase &cb, bool is_enable = true);


//------------------------------------------------------------------------------**********
}; //class COptionEditor




































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	インデックスセレクトエディタ
		セーブインデックスを改変するクラスです。
 */
class CIndexSelectEditor : public IEditor {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef IEditor	super_type;

	
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CIndexSelectEditor::Focus
	/*!
		選択中の処理

		@param	cb	[io]	コールバックデータ

		@retval	true	終了
		@retval	false	継続
	 */
	// ==========================================================================
	virtual bool Focus(CEvtBase &cb);

	// ============================================================================
	// CIndexSelectEditor::Blur
	/*!
		非選択中の処理

		@param	cb	[io]	コールバックデータ
	 */
	// ============================================================================
	virtual void Blur(CEvtBase &cb);

	// =============================================================================
	// CIndexSelectEditor::Full
	/*!
		最強化

		@param	cb	[io]	コールバックデータ
	 */
	// ==========================================================================
	virtual void Full(CEvtBase &cb);

	// ============================================================================
	// CIndexSelectEditor::Reset
	/*!
		白紙化

		@param	cb	[io]	コールバックデータ
	 */
	// ============================================================================
	virtual void Reset(CEvtBase &cb);


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CIndexSelectEditor::GetTitle
	/*!
		タイトルの取得

		@return	タイトル
	 */
	// ============================================================================
	virtual const char *GetTitle() {
		return "Index select";
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CIndexSelectEditor::CIndexSelectEditor
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CIndexSelectEditor();


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//項目
	struct EElement {
		enum Type {
			SaveIndex,	//<セーブインデックス

			Max,
			None,
		};
	};


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	er::CSmallInt<>	m_crsr;


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	bool update(CEvtBase &cb);
	void print(CEvtBase &cb, bool is_enable = true);


//------------------------------------------------------------------------------**********
}; //class CIndexSelectEditor































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	セーブデータエディタ
		セーブデータを改変するクラスです。
 */
class CSaveDataEditor : public CEvtBase {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef CEvtBase				super_type;
	typedef std::deque<IEditor *>	TEditerList;

	
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CSaveDataEditor::Focus
	/*!
		選択されている時に実行される関数
	 */
	// ============================================================================
	virtual void Focus();

	// ============================================================================
	// CSaveDataEditor::MoveEvent
	/*!
		イベント間の移動を判定する関数

		@retval	0		移動しない
		@retval	1～		次のイベントへ
		@retval	～-1	前のイベントへ

		@note
			デフォルトでは ←・→ が移動操作になります
	 */
	// ============================================================================
	virtual TMoveDirect MoveEvent();


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CSaveDataEditor::CSaveDataEditor
	/*!
		デフォルトコンストラクタ

		@param	ti	[in]	タイトル
		@param	ev	[in]	イベントID
		@param	fl	[in]	フラグ
	 */
	// ============================================================================
public:
	CSaveDataEditor(const char *title = NULL, GSE_EVT_ID evt_id = GSD_EVT_ID_NOP, int flag = 0);


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	TEditerList			m_list;
	er::CSmallInt<>		m_crsr;			//<カーソル位置
	bool				m_is_exe;		//<エディタを実行しているか

	CSystemEditor		m_system;		//<システムエディタ
	CStageEditor		m_stage;		//<ステージステージエディタ
	CSpecialEditor		m_special;		//<スペシャルステージエディタ
	COptionEditor		m_option;		//<オプションエディタ
#if _WII
	CIndexSelectEditor	m_save_index;	//<インデックスセレクトエディタ
#endif //_WII
	CIoEditor			m_io;			//<セーブIOエディタ


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	void printTitle(bool is_enable = true);

	class CTitlePrint {
		CEvtBase *m_cb;
		int						m_x;
		int						m_y;
		int						m_w;
		int						m_h;
		CEvtBase::EColor::Type	m_clr;
		int						m_i;
	public:
		CTitlePrint(CEvtBase &cb, int x = 0, int y = 0, int w = 1, int h = 1, CEvtBase::EColor::Type clr = CEvtBase::EColor::White) : m_cb(&cb) {
			m_x = x;
			m_y = y;
			m_w = w;
			m_h = h;
			m_clr = clr;
			m_i = 0;
		}
		void operator()(IEditor *ite) {
			int x = m_x + (m_i / m_h) * m_w;
			int y = m_y + m_i % m_h;
			m_cb->Printc(x, y, m_clr, "%02d:%s", m_i, ite->GetTitle());
			++m_i;
		}
	};


//------------------------------------------------------------------------------**********
}; //class CSaveDataEditor













#if _WII
#pragma warn_notinlined reset	//インライン展開出来無い関数に対する警告メッセージの無効化の解除
#endif //_WII


} //namespace dbg
#endif //#if	defined(__cplusplus)

	// ============================================================================
	// CSaveDataEditor::Function
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
