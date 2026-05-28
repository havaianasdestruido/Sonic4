// ============================================================================
/*!
	@file	dbgPadEmu.hpp
	@brief	パッドエミュレータ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: dbgPadEmu.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page dbgPadEmuMain パッドエミュレータ

	@section dbgPadEmuSummary 概要
		パッドエミュレータを提供します。

		ドラッグモード(デフォルト)
		        1/8   2/8                     6/8   7/8   8/8
		  0┌──┬──┬───────────┬──┬──┐
		   │ L1 │ L2 │      上のボタン      │ R2 │ R1 │
		1/8├──┼──┴───────────┴──┼──┤
		   │    │                                  │    │
		   │ 左 │                                  │  右│      ＿
		   │ の │             方向キー             │  の│    ／  ＼
		   │ ボ │            [ドラック]            │  ボ│   │ □ │
		   │ タ │                                  │取タ│    ＼  ／
		   │ ン │                                  │消ン│      ￣
		   │    │                                  │    │
		7/8├──┼──┬───────────┬──┼──┤
		   │Fre2│SELE│   下のボタン  決定   │STAR│Fre1│
		8/8└──┴──┴───────────┴──┴──┘
		
		タップモード
		        1/8   2/8   3/8         5/8   6/8   7/8   8/8
		  0┌──┬──┬───────────┬──┬──┐
		   │ L1 │ L2 │      上のボタン      │ R2 │ R1 │
		1/8├──┼──┴──┬─────┬──┴──┼──┤
		   │    │          │          │          │    │
		   │ 左 │          │    上    │          │  右│      ＿
		   │ の │          │          │          │  の│    ／  ＼
		4/8│ ボ │    左    ├─────┤    右    │  ボ│   │ □ │
		   │ タ │          │          │          │取タ│    ＼  ／
		   │ ン │          │    下    │          │消ン│      ￣
		   │    │          │          │          │    │
		7/8├──┼──┬──┴─────┴──┬──┼──┤
		   │Fre2│SELE│   下のボタン  決定   │STAR│Fre1│
		8/8└──┴──┴───────────┴──┴──┘

		ゲームモード
		        1/8    2/8         4/8        6/8   7/8   8/8
		  0┌─────┬───────────┬─────┐
		   │  SELECT  │          上          │ 上ボタン │
		   ├─────┼───────────┼─────┤
		2/8│          │                      │          │
		   │          │                      │          │      ＿
		   │          │                      │          │    ／  ＼
		4/8│    左    │                      │    右    │   │ □ │
		   │          │                      │          │    ＼  ／
		   │          │                      │          │      ￣
		   │          │                      │          │
		7/8├─────┬───────────┼─────┤
		   │   Fre1   │          下          │Fre2 / R2 │ 注：Fre1 + Fre2でソフトリセットを設定しています。
		8/8└─────┴───────────┴─────┘

		ダミーモード
		        1/8                                 7/8   8/8
		  0┌──┬─────────────────┬──┐
		   │Fre4│                                  │Fre3│
		   ├──┘                                  └──┘
		   │                                              │
		   │                                              │      ＿
		   │                                              │    ／  ＼
		   │                                              │   │ □ │
		   │                                              │    ＼  ／
		   │                                              │      ￣
		   │                                              │
		7/8├──┐                                  ┌──┤
		   │Fre2│                                  │Fre1│
		8/8└──┴─────────────────┴──┘
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
#include "erTask.hpp"
#include "erTrgBasic.hpp"
#include "accelBitset.hpp"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*

//パッドハック開始
#if _IPHONE
	#undef PAD_DIRECT
	#undef PAD_STAND
	#undef PAD_REPEAT
	#undef PAD_RELEASE
	#define PAD_DIRECT(_port)		(dbg::CPadEmu::CreateInstance().GetPadDirect())
	#define PAD_STAND(_port)		(dbg::CPadEmu::CreateInstance().GetPadStand())
	#define PAD_REPEAT(_port)		(dbg::CPadEmu::CreateInstance().GetPadRepeat())
	#define PAD_RELEASE(_port)		(dbg::CPadEmu::CreateInstance().GetPadRelease())
	#undef PAD_ADIRECT
	#undef PAD_ASTAND
	#undef PAD_AREPEAT
	#undef PAD_ARELEASE
	#define PAD_ADIRECT(_port)		(dbg::CPadEmu::CreateInstance().GetPadDirect())
	#define PAD_ASTAND(_port)		(dbg::CPadEmu::CreateInstance().GetPadStand())
	#define PAD_AREPEAT(_port)		(dbg::CPadEmu::CreateInstance().GetPadRepeat())
	#define PAD_ARELEASE(_port)		(dbg::CPadEmu::CreateInstance().GetPadRelease())
	#undef PAD_MDIRECT
	#undef PAD_MSTAND
	#undef PAD_MREPEAT
	#undef PAD_MRELEASE
	#define PAD_MDIRECT(_port)		(dbg::CPadEmu::CreateInstance().GetPadDirect())
	#define PAD_MSTAND(_port)		(dbg::CPadEmu::CreateInstance().GetPadStand())
	#define PAD_MREPEAT(_port)		(dbg::CPadEmu::CreateInstance().GetPadRepeat())
	#define PAD_MRELEASE(_port)		(dbg::CPadEmu::CreateInstance().GetPadRelease())

	#include "aoPad.h"
	#define AoPadDirect(...)		(dbg::CPadEmu::CreateInstance().GetPadDirect())
	#define AoPadStand(...)			(dbg::CPadEmu::CreateInstance().GetPadStand())
	#define AoPadRepeat(...)		(dbg::CPadEmu::CreateInstance().GetPadRepeat())
	#define AoPadRelease(...)		(dbg::CPadEmu::CreateInstance().GetPadRelease())
	#define AoPadADirect(...)		(dbg::CPadEmu::CreateInstance().GetPadDirect())
	#define AoPadAStand(...)		(dbg::CPadEmu::CreateInstance().GetPadStand())
	#define AoPadARepeat(...)		(dbg::CPadEmu::CreateInstance().GetPadRepeat())
	#define AoPadARelease(...)		(dbg::CPadEmu::CreateInstance().GetPadRelease())
	#define AoPadMDirect(...)		(dbg::CPadEmu::CreateInstance().GetPadDirect())
	#define AoPadMStand(...)		(dbg::CPadEmu::CreateInstance().GetPadStand())
	#define AoPadMRepeat(...)		(dbg::CPadEmu::CreateInstance().GetPadRepeat())
	#define AoPadMRelease(...)		(dbg::CPadEmu::CreateInstance().GetPadRelease())
	
	#define AoPadSomeoneDirect(u16_key, ...)	((dbg::CPadEmu::CreateInstance().IsPadDirect(u16_key))? s32(0): s32(-1))
	#define AoPadSomeoneStand(u16_key, ...)		((dbg::CPadEmu::CreateInstance().IsPadStand(u16_key))? s32(0): s32(-1))
	#define AoPadSomeoneRepeat(u16_key, ...)	((dbg::CPadEmu::CreateInstance().IsPadRepeat(u16_key))? s32(0): s32(-1))
	#define AoPadSomeoneRelease(u16_key, ...)	((dbg::CPadEmu::CreateInstance().IsPadRelease(u16_key))? s32(0): s32(-1))
	#define AoPadSomeoneADirect(u16_key, ...)	((dbg::CPadEmu::CreateInstance().IsPadDirect(u16_key))? s32(0): s32(-1))
	#define AoPadSomeoneAStand(u16_key, ...)	((dbg::CPadEmu::CreateInstance().IsPadStand(u16_key))? s32(0): s32(-1))
	#define AoPadSomeoneARepeat(u16_key, ...)	((dbg::CPadEmu::CreateInstance().IsPadRepeat(u16_key))? s32(0): s32(-1))
	#define AoPadSomeoneARelease(u16_key, ...)	((dbg::CPadEmu::CreateInstance().IsPadRelease(u16_key))? s32(0): s32(-1))
	#define AoPadSomeoneMDirect(u16_key, ...)	((dbg::CPadEmu::CreateInstance().IsPadDirect(u16_key))? s32(0): s32(-1))
	#define AoPadSomeoneMStand(u16_key, ...)	((dbg::CPadEmu::CreateInstance().IsPadStand(u16_key))? s32(0): s32(-1))
	#define AoPadSomeoneMRepeat(u16_key, ...)	((dbg::CPadEmu::CreateInstance().IsPadRepeat(u16_key))? s32(0): s32(-1))
	#define AoPadSomeoneMRelease(u16_key, ...)	((dbg::CPadEmu::CreateInstance().IsPadRelease(u16_key))? s32(0): s32(-1))
#endif //_IPHONE




































































namespace dbg {


#if _WII
#pragma warn_notinlined off	//インライン展開出来無い関数に対する警告メッセージの無効化
#endif //_WII


//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	パッドエミュレータクラス
		パッドエミュレータを提供します。
 */
class CPadEmu : public er::task::CTask<CPadEmu, er::task::CProcCount<CPadEmu>, er::task::ITaskLink> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef er::task::CTask<CPadEmu, er::task::CProcCount<CPadEmu>, er::task::ITaskLink>	TSuperType;
	typedef TSuperType		TTask;
	typedef CPadEmu			TThisType;


	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//モード列挙
	struct EMode {
		enum Type {
			Tap,	//<タップモード(タップ操作を主したパッドエミュレーション)
			Drag,	//<ドラッグモード(十字キー操作にドラッグ操作を利用したパッドエミュレーション)
			Game,	//<ゲームモード(アクションパート時に最適なパッドエミュレーション)
			Dummy,	//<ダミーモード(何もしない)

			Max,
			None,
		};
	};

	//パッド列挙
	struct ETrgPad {
		enum Type {
			LeftDrag,		//<方向キー・ドラッグ
			LeftLeft,		//<方向キー・左
			LeftUp,			//<方向キー・上
			LeftRight,		//<方向キー・右
			LeftDown,		//<方向キー・下
			RightLeft,		//<左ボタン
			RightUp,		//<上ボタン
			RightRight,		//<右ボタン
			RightDown,		//<下ボタン
			TriggerLeft1,	//<L1ボタン
			TriggerRight1,	//<R1ボタン
			TriggerLeft2,	//<L2ボタン
			TriggerRight2,	//<R2ボタン
			Select,			//<Selectボタン
			Start,			//<Startボタン
			Free1,			//<フリー1
			Free2,			//<フリー2
			Free3,			//<フリー3
			Free4,			//<フリー4

			Max,
			None,
		};
	};

	typedef Uint16	TPadInfo;	//<パッド情報
	typedef Uint16	TPadKey;	//<パッドキー


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CPadEmu::CreateInstance
	/*!
		インスタンス構築

		@return インスタンス
	 */
	// ==========================================================================
	static CPadEmu &CreateInstance();

	// =============================================================================
	// CPadEmu::Create
	/*!
		構築

		@param	config	[in]	コンフィグデータ
		@param	size	[in]	コンフィグデータサイズ
		@param	mode	[in]	設定モード

		@retval	true	成功
		@retval	false	失敗

		@note
			失敗しません。
	 */
	// ==========================================================================
	bool Create(const accel::CArray<Float32, 4> *config, Uint32 size);
	template <Uint32 N>
	bool Create(const accel::CArray<Float32, 4> (&config)[N]) {
		return Create(config, N);
	}
	bool Create(EMode::Type mode);

	// =============================================================================
	// CPadEmu::Release
	/*!
		破棄
	 */
	// ==========================================================================
	virtual void Release();

	// =============================================================================
	// CPadEmu::IsValid
	/*!
		有効確認
	
		@retval	true	有効
		@retval	false	無効
	 */
	// ==========================================================================
	virtual bool IsValid() const;

	// =============================================================================
	// CPadEmu::Update
	/*!
		描画
	 */
	// ==========================================================================
	virtual void Update();

	// =============================================================================
	// CPadEmu::operator[]
	/*!
		インデクサ
	 */
	// ==========================================================================
	const er::CTrgState &operator[](ETrgPad::Type pad) const;

	
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CPadEmu::GetPadDirect
	/*!
		パッドの直値の取得

		@return	パッド情報
	 */
	// ============================================================================
	TPadInfo GetPadDirect() const;
	bool IsPadDirect(TPadKey key) const {
		return ((GetPadDirect() & key)? true: false);
	}

	// ============================================================================
	// CPadEmu::GetPadStand
	/*!
		パッドのONエッジの取得

		@return	パッド情報
	 */
	// ============================================================================
	TPadInfo GetPadStand() const;
	bool IsPadStand(TPadKey key) const {
		return ((GetPadStand() & key)? true: false);
	}

	// ============================================================================
	// CPadEmu::GetPadRelease
	/*!
		パッドのOFFエッジの取得

		@return	パッド情報
	 */
	// ============================================================================
	TPadInfo GetPadRelease() const;
	bool IsPadRelease(TPadKey key) const {
		return ((GetPadRelease() & key)? true: false);
	}

	// ============================================================================
	// CPadEmu::GetPadRepeat
	/*!
		パッドのリピートの取得

		@return	パッド情報
	 */
	// ============================================================================
	TPadInfo GetPadRepeat() const;
	bool IsPadRepeat(TPadKey key) const {
		return ((GetPadRepeat() & key)? true: false);
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// =============================================================================
	// CPadEmu::CPadEmu
	/*!
		デフォルトコンストラクタ
	 */
	// ==========================================================================
public:
	CPadEmu() : m_flag() {}

	// =============================================================================
	// CPadEmu::~CPadEmu
	/*!
		デストラクタ
	 */
	// ==========================================================================
public:
	~CPadEmu() {
		Release();
	}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	struct BFlag {
		enum {
			Setup,

			Max,
			None,
		};
		typedef accel::CBitset<Max> Type;
	};

	//パッドトリガエリアテーブル
	typedef accel::CArray<Float32, 4>	TTrgArea;
	static const TTrgArea c_trg_area_table[EMode::Max][ETrgPad::Max];

	//パッドトリガ→トリガ変換テーブル
	typedef accel::CArray<er::CTrgRect, ETrgPad::Max>	TTrg;
	static const TPadInfo c_trg2pad_table[ETrgPad::Max];


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	BFlag::Type		m_flag;		//<フラグ
	TTrg			m_trg;		//<パッドトリガ
	
	static TThisType		*p_instance;		//<共通インスタンス
	static TThisType		p_instance_data;	//<共通インスタンスデータ


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class CPadEmu



















#if _WII
#pragma warn_notinlined reset	//インライン展開出来無い関数に対する警告メッセージの無効化の解除
#endif //_WII


} //namespace dbg
#endif //#if	defined(__cplusplus)

	// ============================================================================
	// CPadEmu::Function
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
