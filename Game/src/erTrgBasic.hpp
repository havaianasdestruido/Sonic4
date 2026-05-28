// ============================================================================
/*!
	@file	erTrgBasic.hpp
	@brief	タッチパネルアクション(トリガ・単純領域)

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009-2010 Dimps
	$Id: erTrgBasic.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page erTrgBasicMain タッチパネルアクション(トリガ・単純領域)

	@section erTrgBasicSummary 概要
		タッチパネルアクション(トリガ)の単純領域クラス群です。
		単純領域別のアクションを提供します。

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
	@brief	タッチパネルアクション(トリガ)矩形クラス
		タッチパネルアクション(トリガ)の矩形当たり機能を提供します。
 */
class CTrgRect : public CTrgBase<> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef CTrgBase<>	TSuperClass;


	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CTrgRect::Create
	/*!
		作成
	
		@retval	true	成功
		@retval	false	失敗

		@note
			失敗しません。
	 */
	// ==========================================================================
	bool Create(const accel::CArray<Sint32, 4> &rect);
	bool Create(const accel::CArray<Sint32, 2> &xy1, const accel::CArray<Sint32, 2> &xy2);
	bool Create(Sint32 x1, Sint32 y1, Sint32 x2, Sint32 y2);

	// =============================================================================
	// CTrgRect::Release
	/*!
		破棄
	 */
	// ==========================================================================
	virtual void Release();

	// =============================================================================
	// CTrgRect::IsValid
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

	//状態型
	typedef accel::CArray<Sint32, 2> TPos;
	typedef accel::CArray<Sint32, 4> TRect;


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	BFlag::Type	m_flag;	//<フラグ
	TPos		m_pos;	//<位置
	TRect		m_rect;	//<矩形


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
	bool create();

	// =============================================================================
	// CTrgRect::hitTest
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
}; //class CTrgRect




















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
