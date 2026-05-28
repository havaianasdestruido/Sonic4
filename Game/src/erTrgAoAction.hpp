// ============================================================================
/*!
	@file	erTrgAoAction.hpp
	@brief	タッチパネルアクション(トリガ・AoAction拡張)

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009-2010 Dimps
	$Id: erTrgAoAction.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page erTrgExtAoActionMain タッチパネルアクション(トリガ・AoAction拡張)

	@section erTrgExtAoActionSummary 概要
		タッチパネルアクション(トリガ)のAoAction拡張です。
		AoActionの当り領域別のアクションを提供します。
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
#include "erTrg.hpp"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace er {
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タッチパネルアクション(トリガ)AoActionクラス
		タッチパネルアクション(トリガ)のAoAction当たり機能を提供します。
 */
class CTrgAoAction : public CTrgBase<> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef CTrgBase<>	TSuperClass;


	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CTrgAoAction::Create
	/*!
		作成
	
		@param	act	[in]	使用するアクション
	
		@retval	true	成功
		@retval	false	失敗

		@note
			NULLが入力されると失敗します。
	 */
	// ==========================================================================
	bool Create(const AOS_ACTION *act);

	// =============================================================================
	// CTrgAoAction::Release
	/*!
		破棄
	 */
	// ==========================================================================
	virtual void Release();

	// =============================================================================
	// CTrgAoAction::IsValid
	/*!
		有効確認
	
		@retval	true	有効
		@retval	false	無効
	 */
	// ==========================================================================
	virtual bool IsValid() const;


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//フラグ型
	struct BFlag {
		enum {
			Setup,

			Max,
			None,
		};
		typedef accel::CBitset<Max> Type;
	};


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	BFlag::Type			m_flag;	//<フラグ
	const AOS_ACTION	*m_act;	//<アクション


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
	bool create();

	// =============================================================================
	// CTrgAoAction::hitTest
	/*!
		ヒットテスト

		@param	pos		[in]	タッチ位置
		@param	index	[in]	タッチインデックス

		@retval	true	ヒット
		@retval	false	未ヒット
	 */
	// ==========================================================================
	virtual bool hitTest(const TPos &pos, Uint32 index);


private:
//------------------------------------------------------------------------------**********
}; //class CTrgBase




















} //namespace er
#endif //#if	defined(__cplusplus)

	// ============================================================================
	// CTrg::Function
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
