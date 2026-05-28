// ============================================================================
/*!
	@file	gmPadVirtualPad.hpp
	@brief	バーチャルパッド

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2010 Dimps
	$Id: gmPadVirtualPad.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page gmPadVirtualPadMain バーチャルパッド

	@section gmPadVirtualPadSummary 概要
		バーチャルパッドを提供します。
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
#include "accelCircularBuffer.hpp"
#include "accelBitset.hpp"
#include "accelArray.hpp"



//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*



namespace gm {


//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	バーチャルパッドクラス
		バーチャルパッドを提供します。
 */
class CPadVirtualPad {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CPadVirtualPad::CreateInstance
	/*!
		インスタンス構築

		@return インスタンス
	 */
	// ==========================================================================
	static CPadVirtualPad &CreateInstance();

	// =============================================================================
	// CPadVirtualPad::Create
	/*!
		構築

		@param	area		[in]	有効矩形
		@param	area_xy1	[in]	有効矩形(左上XY)
		@param	area_xy2	[in]	有効矩形(右下XY)
		@param	area_left	[in]	有効矩形(左上X)
		@param	area_top	[in]	有効矩形(左上Y)
		@param	area_right	[in]	有効矩形(右下X)
		@param	area_bottom	[in]	有効矩形(右下Y)

		@retval	true	成功
		@retval	false	失敗

		@note
			失敗しません。
	 */
	// =============================================================================
	bool Create();
	bool Create(const accel::CArray<f32, 4> &area);
	bool Create(const accel::CArray<f32, 2> &area_xy1, const accel::CArray<f32, 2> &area_xy2);
	bool Create(const f32 (&area)[4]);
	bool Create(const f32 (&area_xy1)[2], const f32 (&area_xy2)[2]);
	bool Create(f32 area_left, f32 area_top, f32 area_right, f32 area_bottom);

	// =============================================================================
	// CPadVirtualPad::Release
	/*!
		破棄
	 */
	// =============================================================================
	void Release();

	// =============================================================================
	// CPadVirtualPad::IsValid
	/*!
		有効確認
	
		@retval	true	有効
		@retval	false	無効
	 */
	// =============================================================================
	bool IsValid() const;

	// =============================================================================
	// CPadVirtualPad::Update
	/*!
		更新
	 */
	// =============================================================================
	void Update();

	
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CPadVirtualPad::GetValue
	/*!
		値の取得

		@return	値
	 */
	// ============================================================================
	u16 GetValue() const;

	// ============================================================================
	// CPadVirtualPad::IsFocus
	/*!
		フォーカス中確認

		@retval true	フォーカス中
		@retval false	非フォーカス
	 */
	// ============================================================================
	bool IsFocus(int tp_index) const;


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// =============================================================================
	// CPadVirtualPad::CPadVirtualPad
	/*!
		デフォルトコンストラクタ
	 */
	// =============================================================================
public:
	CPadVirtualPad() : m_flag() {}

	// =============================================================================
	// CPadVirtualPad::~CPadVirtualPad
	/*!
		デストラクタ
	 */
	// =============================================================================
public:
	~CPadVirtualPad() {
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
	
	static const int c_tp_index_max = 5;	//<タップインデックス数


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	BFlag::Type						m_flag;		//<フラグ
	accel::CArray<f32, 4>			m_area;		//<有効矩形
	accel::CBitset<c_tp_index_max>	m_focus;	//<フォーカス中のタップ番号
	accel::CCircularBuffer<u16, 2>	m_on_flag;	//<onフラグ(amPad互換)
	
	static CPadVirtualPad			*p_instance;	//<共通インスタンス


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	bool create();
	bool isHit(const u16 (&point)[AMD_XY]);
	u16 getOnFlag(const u16 (&point)[AMD_XY]);

	static bool isHit(const accel::CArray<f32, 2> &target, const accel::CArray<f32, 2> &xy1, const accel::CArray<f32, 2> &xy2);
	static bool isHit(const accel::CArray<f32, 2> &target, const accel::CArray<f32, 2> &p1, const accel::CArray<f32, 2> &p2, const accel::CArray<f32, 2> &p3);

//------------------------------------------------------------------------------**********
}; //class CPadVirtualPad





















} //namespace gm
#endif //#if	defined(__cplusplus)

	// ============================================================================
	// CPadVirtualPad::Function
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
