// ============================================================================
/*!
	@file	erDbgBusyVisualizer.hpp
	@brief	負荷視覚化

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: erDbgBusyVisualizer.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page erDbgBusyVisualizerMain 負荷視覚化

	@section erDbgBusyVisualizerSummary 概要
		負荷視覚化を提供します。
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
#include "erShape.hpp"
#include "erObject.hpp"
#include "accelInitializer.hpp"
#include "accelCircularBuffer.hpp"
#include "accelBitset.hpp"
#include "accelArray.hpp"
#include "accelMpl.hpp"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace er {
namespace dbg {


#if _WII
#pragma warn_notinlined off	//インライン展開出来無い関数に対する警告メッセージの無効化
#endif //_WII







//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	変動レポートクラス
		変動に関するレポートを提供します。
 */
template <Uint32 NAveSize, typename TTargetType = float>
class CFluctuateReport : public er::object::IRelease, public er::object::IIsValid, public er::object::IUpdate {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	typedef TTargetType	TType;
	static const Uint32	NSize = NAveSize;

	typedef Uint32		TPeakHoldTime;
	typedef TType		TPeakMoveSpeed;


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CFluctuateReport::Create
	/*!
		構築
	
		@param	peak_move_speed	[in]	ピーク減衰速度
		@param	peak_hold_time	[in]	ピークホールド時間
	
		@retval	true	成功
		@retval	false	失敗

		@note
			失敗しません。
	 */
	// ==========================================================================
	bool Create(TPeakMoveSpeed peak_move_speed = TPeakMoveSpeed(0), TPeakHoldTime peak_hold_time = TPeakHoldTime(-1)) {
		Release();
		
		m_peak_move_speed = peak_move_speed;
		m_peak_hold_time = peak_hold_time;
		reset();

		m_flag[BFlag::Create] = true;
		return true;
	}

	// =============================================================================
	// CFluctuateReport::Release
	/*!
		破棄
	 */
	// ==========================================================================
	virtual void Release() {
		if (m_flag[BFlag::Create]) {
			m_flag.reset();
		}
	}

	// =============================================================================
	// CFluctuateReport::IsValid
	/*!
		有効確認
	
		@retval	true	有効
		@retval	false	無効
	 */
	// ==========================================================================
	virtual bool IsValid() const {
		return m_flag[BFlag::Create];
	}

	// =============================================================================
	// CFluctuateReport::Update
	/*!
		更新
	 */
	// ==========================================================================
	virtual void Update() {
		if (m_flag[BFlag::Create]) {
			insert(m_insert);
		}
	}

	// =============================================================================
	// CFluctuateReport::Reset
	/*!
		リセット
	 */
	// ==========================================================================
	void Reset() {
		if (m_flag[BFlag::Create]) {
			reset();
		}
	}



	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// =============================================================================
	// CFluctuateReport::GetValue
	/*!
		値の取得
	 */
	// ==========================================================================
	const TType &GetValue() const {
		return m_insert;
	}

	// =============================================================================
	// CFluctuateReport::operator[]
	/*!
		値の取得
	
		@note
			0が現在値、1が前回値、2が2つ前の値
	 */
	// ==========================================================================
	const TType &operator[](int index) const {
		return m_data[index];
	}
	const TType &operator[](unsigned int index) const {
		return m_data[index];
	}

	// =============================================================================
	// CFluctuateReport::GetAve
	/*!
		平均取得
	 */
	// ==========================================================================
	TType GetAve() const {
		if (m_flag[BFlag::Create]) {
			createAveCache();
		}
		return m_ave_cache;
	}

	// =============================================================================
	// CFluctuateReport::GetMin
	/*!
		最小取得
	 */
	// ==========================================================================
	TType GetMin() const {
		if (m_flag[BFlag::Create]) {
			createRangeCache();
		}
		return m_range_cache[ERangeKind::Small];
	}

	// =============================================================================
	// CFluctuateReport::GetMax
	/*!
		最大取得
	 */
	// ==========================================================================
	TType GetMax() const {
		if (m_flag[BFlag::Create]) {
			createRangeCache();
		}
		return m_range_cache[ERangeKind::Large];
	}

	// =============================================================================
	// CFluctuateReport::GetHoldMin
	/*!
		最小ホールド取得
	 */
	// ==========================================================================
	TType GetHoldMin() const {
		return m_peak[ERangeKind::Small].value;
	}

	// =============================================================================
	// CFluctuateReport::GetHoldMax
	/*!
		最大ホールド取得
	 */
	// ==========================================================================
	TType GetHoldMax() const {
		return m_peak[ERangeKind::Large].value;
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// =============================================================================
	// CFluctuateReport::SetValue
	/*!
		設定
	 */
	// ==========================================================================
	void SetValue(const TType &data) {
		if (m_flag[BFlag::Create]) {
			m_insert = data;
		}
	}

	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// =============================================================================
	// CFluctuateReport::CBusyVisualizer
	/*!
		デフォルトコンストラクタ
	 */
	// ==========================================================================
public:
	CFluctuateReport() {}

	// =============================================================================
	// CFluctuateReport::~CBusyVisualizer
	/*!
		デストラクタ
	 */
	// ==========================================================================
public:
	~CFluctuateReport() {
		Release();
	}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	///フラグ
	struct BFlag {
		enum {
			Create,

			Max,
			None,
		};
		typedef accel::CBitset<Max> Type;
	};

	///mutableフラグ
	struct BMutableFlag {
		enum {
			AveCache,
			RangeCache,

			Max,
			None,
		};
		typedef accel::CBitset<Max> Type;
	};

	///データ
	typedef accel::CCircularBuffer<float, NSize>	TData;

	///末端
	struct ERangeKind {
		enum Type {
			Small,
			Large,

			Max,
			None,
		};
	};
	
	//レンジデータ
	typedef accel::CArray<TType, ERangeKind::Max>	TRangeData;
	
	//ピークデータ
	struct SPeak {
		TType			value;
		TPeakHoldTime	time;
	};
	typedef accel::CArray<SPeak, ERangeKind::Max>	TPeakData;



	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	typename BFlag::Type				m_flag;				//<フラグ
	mutable typename BMutableFlag::Type	m_mutable_flag;		//<mutableフラグ
	TType								m_insert;			//<挿入データ
	TData								m_data;				//<データ
	mutable TType						m_ave_cache;		//<平均キャッシュ
	mutable TRangeData					m_range_cache;		//<レンジキャッシュ
	TPeakData							m_peak;				//<ピークデータ
	TPeakHoldTime						m_peak_hold_time;	//<ピークホールド時間
	TPeakMoveSpeed						m_peak_move_speed;	//<ピーク移動速度


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	void reset();
	void insert(const TType &data);
	void createAveCache() const;
	void createRangeCache() const;
	
	
//------------------------------------------------------------------------------**********
}; //class CFluctuateReport



//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// =============================================================================
// CFluctuateReport::reset
/*!
	リセット
 */
// ==========================================================================
template <Uint32 NAveSize, typename TTargetType>
void CFluctuateReport<NAveSize, TTargetType>::reset()
{
	m_data.clear();
	m_mutable_flag[BMutableFlag::RangeCache] = false;
	m_mutable_flag[BMutableFlag::AveCache] = false;
}

// =============================================================================
// CFluctuateReport::insert
/*!
	挿入
 */
// ==========================================================================
template <Uint32 NAveSize, typename TTargetType>
void CFluctuateReport<NAveSize, TTargetType>::insert(const TType &data)
{
	//ピーク
	if (m_data.empty()) {
		//初回
		for (typename TPeakData::iterator peak = m_peak.begin(), peak_end = m_peak.end(); peak != peak_end; ++peak) {
			peak->value = data;
			peak->time = m_peak_hold_time;
		}
	} else {
		//初回以外
		{ //ERangeKind::Small
			typename TPeakData::reference peak = m_peak[ERangeKind::Small];
			if (data < peak.value) {
				peak.value = data;
				peak.time = m_peak_hold_time;
			} else if (0 < peak.time) {
				--peak.time;
			} else {
				peak.value += m_peak_move_speed;
			}
		}
		{ //ERangeKind::Large
			typename TPeakData::reference peak = m_peak[ERangeKind::Large];
			if (peak.value < data) {
				peak.value = data;
				peak.time = m_peak_hold_time;
			} else if (0 < peak.time) {
				--peak.time;
			} else {
				peak.value -= m_peak_move_speed;
			}
		}
	}

	//挿入
	m_data.push_back(data);

	//キャッシュリセット
	m_mutable_flag[BMutableFlag::RangeCache] = false;
	m_mutable_flag[BMutableFlag::AveCache] = false;
}

// =============================================================================
// CFluctuateReport::createAveCache
/*!
	平均キャッシュの作成
 */
// ==========================================================================
template <Uint32 NAveSize, typename TTargetType>
void CFluctuateReport<NAveSize, TTargetType>::createAveCache() const
{
	if (!m_mutable_flag[BMutableFlag::AveCache]) {
		TType ave;
		typename TData::const_iterator data = m_data.begin(), data_end = m_data.end();
		if (data != data_end) {
			ave = *data;
			while (++data != data_end) {
				ave += *data;
			}
		}
		m_ave_cache = ave / TType(m_data.size());
		
		m_mutable_flag[BMutableFlag::AveCache] = true;
	}
}

// =============================================================================
// CFluctuateReport::createRangeCache
/*!
	レンジキャッシュの作成
 */
// ==========================================================================
template <Uint32 NAveSize, typename TTargetType>
void CFluctuateReport<NAveSize, TTargetType>::createRangeCache() const
{
	if (!m_mutable_flag[BMutableFlag::RangeCache]) {
		TRangeData range;
		typename TData::const_iterator data = m_data.begin(), data_end = m_data.end();
		if (data != data_end) {
			range[ERangeKind::Large] = range[ERangeKind::Small] = *data;
			while (++data != data_end) {
				if (*data < range[ERangeKind::Small]) {
					range[ERangeKind::Small] = *data;
				} else if (range[ERangeKind::Large] < *data) {
					range[ERangeKind::Large] = *data;
				}
			}
		}
		m_range_cache = range;

		m_mutable_flag[BMutableFlag::RangeCache] = true;
	}
}

//------------------------------------------------------------------------------**********


































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	負荷視覚化インターフェース
		負荷視覚化のインターフェースを提供します。
 */
class IBusyVisualizer : public er::object::IRelease, public er::object::IIsValid
						, public er::object::IUpdate, public er::object::IDraw {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
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
}; //class IBusyVisualizer








































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	負荷視覚化クラス
		負荷視覚化を提供します。
 */
template <Uint32 NAveSize>
class CBusyVisualizer : public IBusyVisualizer {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef IBusyVisualizer	super_type;


	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	static const Uint32	NSize = NAveSize;
	typedef float				TScale;		//<スケール


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CBusyVisualizer::Create
	/*!
		構築
	
		@retval	true	成功
		@retval	false	失敗

		@note
			失敗しません。
	 */
	// ==========================================================================
	bool Create();

	// =============================================================================
	// CBusyVisualizer::Release
	/*!
		破棄
	 */
	// ==========================================================================
	virtual void Release() {
		if (m_flag[BFlag::Create]) {
			m_flag[BFlag::Create] = false;
		}
	}

	// =============================================================================
	// CBusyVisualizer::IsValid
	/*!
		有効確認
	
		@retval	true	有効
		@retval	false	無効
	 */
	// ==========================================================================
	virtual bool IsValid() const {
		return m_flag[BFlag::Create];
	}

	// =============================================================================
	// CBusyVisualizer::Update
	/*!
		更新
	 */
	// ==========================================================================
	virtual void Update();

	// =============================================================================
	// CBusyVisualizer::Draw
	/*!
		描画
	 */
	// ==========================================================================
	virtual void Draw() const;

	
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// =============================================================================
	// CBusyVisualizer::GetScale
	/*!
		スケールの取得
	 */
	// ==========================================================================
	TScale GetScale() const {
		return m_scale;
	}

	
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// =============================================================================
	// CBusyVisualizer::SetScale
	/*!
		スケールの設定
	 */
	// ==========================================================================
	void SetScale(TScale scale) {
		m_scale = scale;
	}

	
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// =============================================================================
	// CBusyVisualizer::CBusyVisualizer
	/*!
		デフォルトコンストラクタ
	 */
	// ==========================================================================
public:
	CBusyVisualizer() : super_type() , m_flag() {}

	// =============================================================================
	// CBusyVisualizer::~CBusyVisualizer
	/*!
		デストラクタ
	 */
	// ==========================================================================
public:
	~CBusyVisualizer() {
		Release();
	}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	struct BFlag {
		enum {
			Create,
			NoDraw,
			NoUpdate,

			Max,
			None,
		};
		typedef accel::CBitset<Max> Type;
	};

	//負荷レポート
	struct EReport {
		enum Type {
			Calc,
			Draw,
			Gpu,

			Max,
			None,
		};
	};
	typedef accel::CArray<CFluctuateReport<NSize>, EReport::Max>	TReport;
	typedef CShape<NNS_PRIM3D_PC, (3 * 4 + 1) * 6>		TShape;


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	typename BFlag::Type	m_flag;		//<フラグ
	TReport					m_report;	//<レポート
	TShape					m_shape;	//<シェイプ
	TScale					m_scale;	//<スケール

	


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class CBusyVisualizer



//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// =============================================================================
// CBusyVisualizer::Create
/*!
	構築

	@retval	true	成功
	@retval	false	失敗

	@note
		失敗しません。
 */
// ==========================================================================
template <Uint32 NAveSize>
bool CBusyVisualizer<NAveSize>::Create()
{
	Release();

	static const NNS_RGBA8888	c_color[] = {0x44CC00FF, 0xCCCC00FF, 0x0044CCFF};

	m_shape.Create();
	m_shape.SetDrawPrio(0xEFFF);
	for (typename TReport::size_type i = 0, max = m_report.size(); i < max; ++i) {
		typename TReport::reference	report = m_report[i];
		report.Create(0.005f, 30);

		{	//グラフ
			TShape &shape = m_shape;
			Uint32 index = (i * 4 + 0) * 6;
			TShape::TVertex vertex;
			vertex.Col = c_color[i];
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.00f + 0.97f + (0.01f * i), 0.0f, 0.0f);
			shape.SetVertex(index + 0, vertex);
			shape.SetVertex(index + 1, vertex);
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.01f + 0.97f + (0.01f * i), 0.0f, 0.0f);
			shape.SetVertex(index + 2, vertex);
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.00f + 0.97f + (0.01f * i), 0.0f, 0.0f);
			shape.SetVertex(index + 3, vertex);
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.01f + 0.97f + (0.01f * i), 0.0f, 0.0f);
			shape.SetVertex(index + 4, vertex);
			shape.SetVertex(index + 5, vertex);
		}
		{	//最小ピーク
			TShape &shape = m_shape;
			Uint32 index = (i * 4 + 1) * 6;
			TShape::TVertex vertex;
			vertex.Col = 0xFF4400FF;
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.00f + 0.97f + (0.01f * i), 0.000f, 0.0f);
			shape.SetVertex(index + 0, vertex);
			shape.SetVertex(index + 1, vertex);
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.01f + 0.97f + (0.01f * i), 0.000f, 0.0f);
			shape.SetVertex(index + 2, vertex);
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.00f + 0.97f + (0.01f * i), 0.005f, 0.0f);
			shape.SetVertex(index + 3, vertex);
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.01f + 0.97f + (0.01f * i), 0.005f, 0.0f);
			shape.SetVertex(index + 4, vertex);
			shape.SetVertex(index + 5, vertex);
		}
		{	//最大ピーク
			TShape &shape = m_shape;
			Uint32 index = (i * 4 + 2) * 6;
			TShape::TVertex vertex;
			vertex.Col = 0xFF4400FF;
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.00f + 0.97f + (0.01f * i), 0.000f, 0.0f);
			shape.SetVertex(index + 0, vertex);
			shape.SetVertex(index + 1, vertex);
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.01f + 0.97f + (0.01f * i), 0.000f, 0.0f);
			shape.SetVertex(index + 2, vertex);
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.00f + 0.97f + (0.01f * i), 0.005f, 0.0f);
			shape.SetVertex(index + 3, vertex);
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.01f + 0.97f + (0.01f * i), 0.005f, 0.0f);
			shape.SetVertex(index + 4, vertex);
			shape.SetVertex(index + 5, vertex);
		}
		{	//平均ピーク
			TShape &shape = m_shape;
			Uint32 index = (i * 4 + 3) * 6;
			TShape::TVertex vertex;
			vertex.Col = 0xFFFFFFFF;
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.00f + 0.97f + (0.01f * i), 0.000f, 0.0f);
			shape.SetVertex(index + 0, vertex);
			shape.SetVertex(index + 1, vertex);
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.01f + 0.97f + (0.01f * i), 0.000f, 0.0f);
			shape.SetVertex(index + 2, vertex);
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.00f + 0.97f + (0.01f * i), 0.005f, 0.0f);
			shape.SetVertex(index + 3, vertex);
			vertex.Pos = accel::Initializer<NNS_VECTOR>(0.01f + 0.97f + (0.01f * i), 0.005f, 0.0f);
			shape.SetVertex(index + 4, vertex);
			shape.SetVertex(index + 5, vertex);
		}
	}
	{	//マーク
		TShape &shape = m_shape;
		Uint32 index = (12) * 6;
		TShape::TVertex vertex;
		vertex.Col = 0x000000FF;
		vertex.Pos = accel::Initializer<NNS_VECTOR>(0.00f + 0.97f, 1.000f, 0.0f);
		shape.SetVertex(index + 0, vertex);
		shape.SetVertex(index + 1, vertex);
		vertex.Pos = accel::Initializer<NNS_VECTOR>(0.03f + 0.97f, 1.000f, 0.0f);
		shape.SetVertex(index + 2, vertex);
		vertex.Pos = accel::Initializer<NNS_VECTOR>(0.00f + 0.97f, 1.005f, 0.0f);
		shape.SetVertex(index + 3, vertex);
		vertex.Pos = accel::Initializer<NNS_VECTOR>(0.03f + 0.97f, 1.005f, 0.0f);
		shape.SetVertex(index + 4, vertex);
		shape.SetVertex(index + 5, vertex);
	}
	
	//スケール
	m_scale = TScale(1);

	m_flag[BFlag::Create] = true;
	return true;
}

// =============================================================================
// CBusyVisualizer::Update
/*!
	更新
 */
// ==========================================================================
template <Uint32 NAveSize>
void CBusyVisualizer<NAveSize>::Update()
{
	if (m_flag[BFlag::Create] && !m_flag[BFlag::NoUpdate]) {
		typedef float(*FGetPerformance)(void);
		static const FGetPerformance c_get_performance[] = {amDebugGetPerformanceMain, amDebugGetPerformanceDraw, amDebugGetPerformanceGPU};
		
		//調整値
		TScale adjust = TScale(1) / m_scale;
		
		for (typename TReport::size_type i = 0, max = m_report.size(); i < max; ++i) {
			typename TReport::reference	report = m_report[i];
			FGetPerformance get_performance = c_get_performance[i];

			report.SetValue(get_performance());
			report.Update();

			//グラフ
			for (Uint32 k = 3, k_max = 6; k < k_max; ++k) {
				TShape &shape = m_shape;
				Uint32 index = (i * 4 + 0) * 6;
				TShape::TVertex vertex = shape.GetVertex(index + k);
				vertex.Pos.y = report.GetValue() * adjust;
				shape.SetVertex(index + k, vertex);
			}

			{	//最小ピーク
				TShape &shape = m_shape;
				Uint32 index = (i * 4 + 1) * 6;
				for (Uint32 k = 1, k_max = 3; k < k_max; ++k) {
					TShape::TVertex vertex = shape.GetVertex(index + k);
					vertex.Pos.y = report.GetHoldMin() * adjust;
					shape.SetVertex(index + k, vertex);
					vertex.Pos.y += 0.005f;
					shape.SetVertex(index + k + 2, vertex);
				}
				shape.SetVertex(index + 0, shape.GetVertex(index + 1));
				shape.SetVertex(index + 5, shape.GetVertex(index + 4));
			}
			{	//最大ピーク
				TShape &shape = m_shape;
				Uint32 index = (i * 4 + 2) * 6;
				for (Uint32 k = 1, k_max = 3; k < k_max; ++k) {
					TShape::TVertex vertex = shape.GetVertex(index + k);
					vertex.Pos.y = report.GetHoldMax() * adjust;
					shape.SetVertex(index + k, vertex);
					vertex.Pos.y += 0.005f;
					shape.SetVertex(index + k + 2, vertex);
				}
				shape.SetVertex(index + 0, shape.GetVertex(index + 1));
				shape.SetVertex(index + 5, shape.GetVertex(index + 4));
			}
			{	//平均
				TShape &shape = m_shape;
				Uint32 index = (i * 4 + 3) * 6;
				for (Uint32 k = 1, k_max = 3; k < k_max; ++k) {
					TShape::TVertex vertex = shape.GetVertex(index + k);
					vertex.Pos.y = report.GetAve() * adjust;
					shape.SetVertex(index + k, vertex);
					vertex.Pos.y += 0.005f;
					shape.SetVertex(index + k + 2, vertex);
				}
				shape.SetVertex(index + 0, shape.GetVertex(index + 1));
				shape.SetVertex(index + 5, shape.GetVertex(index + 4));
			}
		}		
		{	//マーク
			TShape &shape = m_shape;
			Uint32 index = (12) * 6;
			for (Uint32 k = 1, k_max = 3; k < k_max; ++k) {
				TShape::TVertex vertex = shape.GetVertex(index + k);
				vertex.Pos.y = TScale(1) * adjust;
				shape.SetVertex(index + k, vertex);
				vertex.Pos.y += 0.005f;
				shape.SetVertex(index + k + 2, vertex);
			}
			shape.SetVertex(index + 0, shape.GetVertex(index + 1));
			shape.SetVertex(index + 5, shape.GetVertex(index + 4));
		}
	}
}

// =============================================================================
// CBusyVisualizer::Draw
/*!
	描画
 */
// ==========================================================================
template <Uint32 NAveSize>
void CBusyVisualizer<NAveSize>::Draw() const
{
	if (m_flag[BFlag::Create] && !m_flag[BFlag::NoDraw]) {
		const TShape &shape = m_shape;
		shape.Draw();
	}
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********








































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	負荷視覚化タスククラス
		負荷視覚化タスクを提供します。
 */
class CBusyVisualizerTask : public er::task::ITaskLink, public er::object::IRelease, public er::object::IIsValid {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CBusyVisualizerTask::CreateInstance
	/*!
		インスタンス構築
	 */
	// ==========================================================================
	static CBusyVisualizerTask &CreateInstance();

	// =============================================================================
	// CBusyVisualizerTask::Create
	/*!
		構築
	
		@retval	true	成功
		@retval	false	失敗

		@note
			失敗しません。
	 */
	// ==========================================================================
	bool Create();

	// =============================================================================
	// CBusyVisualizerTask::Release
	/*!
		破棄
	 */
	// ==========================================================================
	virtual void Release();

	// =============================================================================
	// CFluctuateReport::IsValid
	/*!
		有効確認
	
		@retval	true	有効
		@retval	false	無効
	 */
	// ==========================================================================
	virtual bool IsValid() const {
		return m_visualizer.IsValid();
	}

	
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// =============================================================================
	// CBusyVisualizer::GetScale
	/*!
		スケールの取得
	 */
	// ==========================================================================
	float GetScale() const {
		return m_visualizer.GetScale();
	}

	
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// =============================================================================
	// CBusyVisualizer::SetScale
	/*!
		スケールの設定
	 */
	// ==========================================================================
	void SetScale(float scale) {
		m_visualizer.SetScale(scale);
	}

	
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// =============================================================================
	// CBusyVisualizerTask::CBusyVisualizer
	/*!
		デフォルトコンストラクタ
	 */
	// ==========================================================================
private:
	CBusyVisualizerTask() {}

	// =============================================================================
	// CBusyVisualizerTask::~CBusyVisualizerTask
	/*!
		デストラクタ
	 */
	// ==========================================================================
private:
	~CBusyVisualizerTask() {}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	typedef CBusyVisualizer<30>	TBusyVisualizer;


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	TBusyVisualizer	m_visualizer;


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
	// ============================================================================
	// CBusyVisualizerTask::operator()
	/*!
		フレーム毎に呼び出される関数
	 */
	// ============================================================================
	virtual void operator()();
private:
//------------------------------------------------------------------------------**********
}; //class CBusyVisualizerTask



//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********

















#if _WII
#pragma warn_notinlined reset	//インライン展開出来無い関数に対する警告メッセージの無効化の解除
#endif //_WII


} //namespace dbg
} //namespace er
#endif //#if	defined(__cplusplus)

	// ============================================================================
	// CBusyVisualizer::Function
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
