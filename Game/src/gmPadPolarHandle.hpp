// ============================================================================
/*!
	@file	gmPadPolarHandle.hpp
	@brief	極座標ハンドル

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2010 Dimps
	$Id: gmPadPolarHandle.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page gmPadPolarHandleMain 極座標ハンドル

	@section gmPadPolarHandleSummary 概要
		極座標ハンドルを提供します。
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
#include "accelBitset.hpp"
#include "accelArray.hpp"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*



namespace gm {


//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	極座標ハンドルクラス
		極座標ハンドルを提供します。
 */
class CPadPolarHandle {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CPadPolarHandle::CreateInstance
	/*!
		インスタンス構築

		@return インスタンス
	 */
	// ==========================================================================
	static CPadPolarHandle &CreateInstance();

	// =============================================================================
	// CPadPolarHandle::Create
	/*!
		構築

		@param	area		[in]	有効範囲
		@param	area_left	[in]	有効範囲(左上X)
		@param	area_top	[in]	有効範囲(左上Y)
		@param	area_right	[in]	有効範囲(右下X)
		@param	area_bottom	[in]	有効範囲(右下Y)
		@param	center		[in]	中心位置
		@param	center_x	[in]	中心位置X
		@param	center_y	[in]	中心位置Y

		@retval	true	成功
		@retval	false	失敗

		@note
			失敗しません。
	 */
	// =============================================================================
	bool Create();
	bool Create(const accel::CArray<f32, 4> &area);
	bool Create(const float (&area)[4]);
	bool Create(f32 area_left, f32 area_top, f32 area_right, f32 area_bottom);
	bool Create(const accel::CArray<f32, AMD_XY> &center);
	bool Create(const float (&center)[AMD_XY]);
	bool Create(f32 center_x, f32 center_y);
	bool Create(const accel::CArray<f32, 4> &area
				, const accel::CArray<f32, AMD_XY> &center);
	bool Create(const float (&area)[4], const float (&center)[AMD_XY]);
	bool Create(f32 area_left, f32 area_top, f32 area_right, f32 area_bottom
				, f32 center_x, f32 center_y);

	// =============================================================================
	// CPadPolarHandle::Release
	/*!
		破棄
	 */
	// =============================================================================
	void Release();

	// =============================================================================
	// CPadPolarHandle::IsValid
	/*!
		有効確認
	
		@retval	true	有効
		@retval	false	無効
	 */
	// =============================================================================
	bool IsValid() const;

	// =============================================================================
	// CPadPolarHandle::Update
	/*!
		更新
	 */
	// =============================================================================
	void Update();

	
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CPadPolarHandle::GetValue
	/*!
		値の取得

		@return	値
	
		@note
			Angle32(s32)なら16bit度、
			float(f32)fならラジアンを返します。
			360度を返す関数はありません。
	 */
	// ============================================================================
	template <typename TType>
	TType GetValue() const; //未定義
	//template <>
	//float GetValue<float>() const;
	//template <>
	//Angle32 GetValue<Angle32>() const;

	// ============================================================================
	// CPadPolarHandle::IsFocus
	/*!
		フォーカス中確認

		@retval true	フォーカス中
		@retval false	非フォーカス
	 */
	// ============================================================================
	bool IsFocus() const;

	// ============================================================================
	// CPadPolarHandle::GetFocusTpIndex
	/*!
		フォーカス中のタップインデックスの取得

		@return	フォーカス中のタップインデックス
	
		@note
			フォーカスしているタップインデックスが無い場合は -1 を返します。
	 */
	// ============================================================================
	s32 GetFocusTpIndex() const;


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// CPadPolarHandle::SetValue
	/*!
		値の設定

		@param	value		[in]	設定値
	 */
	// ============================================================================
	void SetValue(float value);
	void SetValue(Angle32 value);


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// =============================================================================
	// CPadPolarHandle::CPadPolarHandle
	/*!
		デフォルトコンストラクタ
	 */
	// =============================================================================
public:
	CPadPolarHandle() : m_flag() {}

	// =============================================================================
	// CPadPolarHandle::~CPadPolarHandle
	/*!
		デストラクタ
	 */
	// =============================================================================
public:
	~CPadPolarHandle() {
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

	static const float c_pi; //π
	

	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	BFlag::Type					m_flag;			//<フラグ
	accel::CArray<f32, 4>		m_area;			//<有効範囲
	accel::CArray<f32, AMD_XY>	m_center;		//<中心点
	s32							m_focus;		//<フォーカス中のタップインデックス
	accel::CArray<f32, AMD_XY>	m_prev;			//<前回点
	s32							m_around;		//<周回
	float						m_value;		//<現在値(ラジアン)
	float						m_zero_point;	//<零ポイント
	
	static CPadPolarHandle		*p_instance;	//<共通インスタンス


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	bool create();
	float getCurrentValue();
	s32 getOnTpIndex();
	s32 getPushTpIndex();
	bool isHit(const u16 (&point)[AMD_XY]);


//------------------------------------------------------------------------------**********
}; //class CPadPolarHandle





















} //namespace gm
#endif //#if	defined(__cplusplus)

	// ============================================================================
	// CPadPolarHandle::Function
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
