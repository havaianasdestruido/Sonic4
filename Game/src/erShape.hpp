// ============================================================================
/*!
	@file	erShape.hpp
	@brief	シェイプ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: erShape.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page erShapeMain シェイプ

	@section erShapeSummary 概要
		シェイプを提供します。
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
#include "accelMpl.hpp"
#include "accelArray.hpp"
#include "accelBitset.hpp"
#include "erObject.hpp"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace er {









namespace detail {
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	テンプレート衝撃吸収クラス
		テンプレートの衝撃をこのクラスで吸収し、メタプログラムを容易にします。
 */
template <typename TType = accel::mpl::CNullType>
class CSuspension {
public:
	static const bool					c_is_2d = false;
	static const bool					c_is_3d = false;
	typedef AMS_PARAM_DRAW_PRIMITIVE	TPrimitive;
};
//------------------------------------------------------------------------------**********
template <>
class CSuspension<NNS_PRIM2D_P> {
public:
	static const bool					c_is_2d = true;
	static const bool					c_is_3d = false;
	static const bool					c_has_color = false;
	static const bool					c_has_texture = false;
	static const bool					c_has_normal = false;
	static const NNE_PRIM2D_FMT			c_primitive_format = NNE_PRIM2D_FMT_P;
	typedef AMS_PARAM_DRAW_PRIMITIVE	TPrimitive;
	typedef NNS_PRIM2D_P				TVertex;
	typedef void						*TMatrix;

	static void SetPrimitiveFormat(TPrimitive &primitive) {
		primitive.format2D = c_primitive_format;
	}
	static void SetPrimitiveVertexList(TPrimitive &primitive, TVertex *vertex) {
		primitive.vtxPC2D = reinterpret_cast<NNS_PRIM2D_PC *>(vertex);
	}
	template <unsigned int NSize>
	static void SetPrimitiveVertexList(TPrimitive &primitive, accel::CArray<TVertex, NSize> &vertex) {
		primitive.vtxPC2D = reinterpret_cast<NNS_PRIM2D_PC *>(&vertex);
	}
	static void InitMatrix(TMatrix &matrix) {
		UNREFERENCED_PARAMETER(matrix);
	}
	static void SetPrimitiveMatrix(TPrimitive &primitive, TMatrix &matrix) {
		primitive.mtx = NULL;
		UNREFERENCED_PARAMETER(matrix);
	}
	static void InitPrimitiveZValue(TPrimitive &primitive) {
		primitive.zOffset = -1.0f;
	}
	static NNS_RGBA8888 *GetColor(TVertex &vertex) {
		return NULL;
		UNREFERENCED_PARAMETER(vertex);
	}
};
//------------------------------------------------------------------------------**********
template <>
class CSuspension<NNS_PRIM2D_PC> {
public:
	static const bool					c_is_2d = true;
	static const bool					c_is_3d = false;
	static const bool					c_has_color = true;
	static const bool					c_has_texture = false;
	static const bool					c_has_normal = false;
	static const NNE_PRIM2D_FMT			c_primitive_format = NNE_PRIM2D_FMT_PC;
	typedef AMS_PARAM_DRAW_PRIMITIVE	TPrimitive;
	typedef NNS_PRIM2D_PC				TVertex;
	typedef void						*TMatrix;

	static void SetPrimitiveFormat(TPrimitive &primitive) {
		primitive.format2D = c_primitive_format;
	}
	static void SetPrimitiveVertexList(TPrimitive &primitive, TVertex *vertex) {
		primitive.vtxPC2D = vertex;
	}
	template <unsigned int NSize>
	static void SetPrimitiveVertexList(TPrimitive &primitive, accel::CArray<TVertex, NSize> &vertex) {
		primitive.vtxPC2D = reinterpret_cast<NNS_PRIM2D_PC *>(&vertex);
	}
	static void InitMatrix(TMatrix &matrix) {
		UNREFERENCED_PARAMETER(matrix);
	}
	static void SetPrimitiveMatrix(TPrimitive &primitive, TMatrix &matrix) {
		primitive.mtx = NULL;
		UNREFERENCED_PARAMETER(matrix);
	}
	static void InitPrimitiveZValue(TPrimitive &primitive) {
		primitive.zOffset = -1.0f;
	}
	static NNS_RGBA8888 *GetColor(TVertex &vertex) {
		return &vertex.Col;
	}
};
//------------------------------------------------------------------------------**********
template <>
class CSuspension<NNS_PRIM2D_PCT> {
public:
	static const bool					c_is_2d = true;
	static const bool					c_is_3d = false;
	static const bool					c_has_color = true;
	static const bool					c_has_texture = true;
	static const bool					c_has_normal = false;
	static const NNE_PRIM2D_FMT			c_primitive_format = NNE_PRIM2D_FMT_PCT;
	typedef AMS_PARAM_DRAW_PRIMITIVE	TPrimitive;
	typedef NNS_PRIM2D_PCT				TVertex;
	typedef void						*TMatrix;

	static void SetPrimitiveFormat(TPrimitive &primitive) {
		primitive.format2D = c_primitive_format;
	}
	static void SetPrimitiveVertexList(TPrimitive &primitive, TVertex *vertex) {
		primitive.vtxPCT2D = vertex;
	}
	template <unsigned int NSize>
	static void SetPrimitiveVertexList(TPrimitive &primitive, accel::CArray<TVertex, NSize> &vertex) {
		primitive.vtxPCT2D = reinterpret_cast<NNS_PRIM2D_PCT *>(&vertex);
	}
	static void InitMatrix(TMatrix &matrix) {
		UNREFERENCED_PARAMETER(matrix);
	}
	static void SetPrimitiveMatrix(TPrimitive &primitive, TMatrix &matrix) {
		primitive.mtx = NULL;
		UNREFERENCED_PARAMETER(matrix);
	}
	static void InitPrimitiveZValue(TPrimitive &primitive) {
		primitive.zOffset = -1.0f;
	}
	static NNS_RGBA8888 *GetColor(TVertex &vertex) {
		return &vertex.Col;
	}
};
//------------------------------------------------------------------------------**********
template <>
class CSuspension<NNS_PRIM3D_P> {
public:
	static const bool					c_is_2d = false;
	static const bool					c_is_3d = true;
	static const bool					c_has_color = false;
	static const bool					c_has_texture = false;
	static const bool					c_has_normal = false;
	static const NNE_PRIM3D_FMT			c_primitive_format = NNE_PRIM3D_FMT_P;
	typedef AMS_PARAM_DRAW_PRIMITIVE	TPrimitive;
	typedef NNS_PRIM3D_P				TVertex;
	typedef NNS_MATRIX					TMatrix;

	static void SetPrimitiveFormat(TPrimitive &primitive) {
		primitive.format3D = c_primitive_format;
	}
	static void SetPrimitiveVertexList(TPrimitive &primitive, TVertex *vertex) {
		primitive.vtxPC3D = reinterpret_cast<NNS_PRIM3D_PC *>(vertex);
	}
	template <unsigned int NSize>
	static void SetPrimitiveVertexList(TPrimitive &primitive, accel::CArray<TVertex, NSize> &vertex) {
		primitive.vtxPC3D = reinterpret_cast<NNS_PRIM3D_PC *>(&vertex);
	}
	static void InitMatrix(TMatrix &matrix) {
		nnMakeUnitMatrix(&matrix);
	}
	static void SetPrimitiveMatrix(TPrimitive &primitive, TMatrix &matrix) {
		primitive.mtx = &matrix;
	}
	static void InitPrimitiveZValue(TPrimitive &primitive) {
		primitive.sortZ = 0.0f;
	}
	static NNS_RGBA8888 *GetColor(TVertex &vertex) {
		return NULL;
		UNREFERENCED_PARAMETER(vertex);
	}
};
//------------------------------------------------------------------------------**********
template <>
class CSuspension<NNS_PRIM3D_PN> {
public:
	static const bool					c_is_2d = false;
	static const bool					c_is_3d = true;
	static const bool					c_has_color = false;
	static const bool					c_has_texture = false;
	static const bool					c_has_normal = true;
	static const NNE_PRIM3D_FMT			c_primitive_format = NNE_PRIM3D_FMT_PN;
	typedef AMS_PARAM_DRAW_PRIMITIVE	TPrimitive;
	typedef NNS_PRIM3D_PN				TVertex;
	typedef NNS_MATRIX					TMatrix;

	static void SetPrimitiveFormat(TPrimitive &primitive) {
		primitive.format3D = c_primitive_format;
	}
	static void SetPrimitiveVertexList(TPrimitive &primitive, TVertex *vertex) {
		primitive.vtxPC3D = reinterpret_cast<NNS_PRIM3D_PC *>(vertex);
	}
	template <unsigned int NSize>
	static void SetPrimitiveVertexList(TPrimitive &primitive, accel::CArray<TVertex, NSize> &vertex) {
		primitive.vtxPC3D = reinterpret_cast<NNS_PRIM3D_PC *>(&vertex);
	}
	static void InitMatrix(TMatrix &matrix) {
		nnMakeUnitMatrix(&matrix);
	}
	static void SetPrimitiveMatrix(TPrimitive &primitive, TMatrix &matrix) {
		primitive.mtx = &matrix;
	}
	static void InitPrimitiveZValue(TPrimitive &primitive) {
		primitive.sortZ = 0.0f;
	}
	static NNS_RGBA8888 *GetColor(TVertex &vertex) {
		return NULL;
		UNREFERENCED_PARAMETER(vertex);
	}
};
//------------------------------------------------------------------------------**********
template <>
class CSuspension<NNS_PRIM3D_PC> {
public:
	static const bool					c_is_2d = false;
	static const bool					c_is_3d = true;
	static const bool					c_has_color = true;
	static const bool					c_has_texture = false;
	static const bool					c_has_normal = false;
	static const NNE_PRIM3D_FMT			c_primitive_format = NNE_PRIM3D_FMT_PC;
	typedef AMS_PARAM_DRAW_PRIMITIVE	TPrimitive;
	typedef NNS_PRIM3D_PC				TVertex;
	typedef NNS_MATRIX					TMatrix;

	static void SetPrimitiveFormat(TPrimitive &primitive) {
		primitive.format3D = c_primitive_format;
	}
	static void SetPrimitiveVertexList(TPrimitive &primitive, TVertex *vertex) {
		primitive.vtxPC3D = vertex;
	}
	template <unsigned int NSize>
	static void SetPrimitiveVertexList(TPrimitive &primitive, accel::CArray<TVertex, NSize> &vertex) {
		primitive.vtxPC3D = reinterpret_cast<NNS_PRIM3D_PC *>(&vertex);
	}
	static void InitMatrix(TMatrix &matrix) {
		nnMakeUnitMatrix(&matrix);
	}
	static void SetPrimitiveMatrix(TPrimitive &primitive, TMatrix &matrix) {
		primitive.mtx = &matrix;
	}
	static void InitPrimitiveZValue(TPrimitive &primitive) {
		primitive.sortZ = 0.0f;
	}
	static NNS_RGBA8888 *GetColor(TVertex &vertex) {
		return &vertex.Col;
	}
};
//------------------------------------------------------------------------------**********
template <>
class CSuspension<NNS_PRIM3D_PNT> {
public:
	static const bool					c_is_2d = false;
	static const bool					c_is_3d = true;
	static const bool					c_has_color = false;
	static const bool					c_has_texture = true;
	static const bool					c_has_normal = true;
	static const NNE_PRIM3D_FMT			c_primitive_format = NNE_PRIM3D_FMT_PNT;
	typedef AMS_PARAM_DRAW_PRIMITIVE	TPrimitive;
	typedef NNS_PRIM3D_PNT				TVertex;
	typedef NNS_MATRIX					TMatrix;

	static void SetPrimitiveFormat(TPrimitive &primitive) {
		primitive.format3D = c_primitive_format;
	}
	static void SetPrimitiveVertexList(TPrimitive &primitive, TVertex *vertex) {
		primitive.vtxPCT3D = reinterpret_cast<NNS_PRIM3D_PCT *>(vertex);
	}
	template <unsigned int NSize>
	static void SetPrimitiveVertexList(TPrimitive &primitive, accel::CArray<TVertex, NSize> &vertex) {
		primitive.vtxPCT3D = reinterpret_cast<NNS_PRIM3D_PCT *>(&vertex);
	}
	static void InitMatrix(TMatrix &matrix) {
		nnMakeUnitMatrix(&matrix);
	}
	static void SetPrimitiveMatrix(TPrimitive &primitive, TMatrix &matrix) {
		primitive.mtx = &matrix;
	}
	static void InitPrimitiveZValue(TPrimitive &primitive) {
		primitive.sortZ = 0.0f;
	}
	static NNS_RGBA8888 *GetColor(TVertex &vertex) {
		return NULL;
		UNREFERENCED_PARAMETER(vertex);
	}
};
//------------------------------------------------------------------------------**********
template <>
class CSuspension<NNS_PRIM3D_PCT> {
public:
	static const bool					c_is_2d = false;
	static const bool					c_is_3d = true;
	static const bool					c_has_color = true;
	static const bool					c_has_texture = true;
	static const bool					c_has_normal = false;
	static const NNE_PRIM3D_FMT			c_primitive_format = NNE_PRIM3D_FMT_PCT;
	typedef AMS_PARAM_DRAW_PRIMITIVE	TPrimitive;
	typedef NNS_PRIM3D_PCT				TVertex;
	typedef NNS_MATRIX					TMatrix;

	static void SetPrimitiveFormat(TPrimitive &primitive) {
		primitive.format3D = c_primitive_format;
	}
	static void SetPrimitiveVertexList(TPrimitive &primitive, TVertex *vertex) {
		primitive.vtxPCT3D = vertex;
	}
	template <unsigned int NSize>
	static void SetPrimitiveVertexList(TPrimitive &primitive, accel::CArray<TVertex, NSize> &vertex) {
		primitive.vtxPCT3D = reinterpret_cast<NNS_PRIM3D_PCT *>(&vertex);
	}
	static void InitMatrix(TMatrix &matrix) {
		nnMakeUnitMatrix(&matrix);
	}
	static void SetPrimitiveMatrix(TPrimitive &primitive, TMatrix &matrix) {
		primitive.mtx = &matrix;
	}
	static void InitPrimitiveZValue(TPrimitive &primitive) {
		primitive.sortZ = 0.0f;
	}
	static NNS_RGBA8888 *GetColor(TVertex &vertex) {
		return &vertex.Col;
	}
};
//------------------------------------------------------------------------------**********
} //namespace detail










































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	シェイプインターフェース
		シェイプのインターフェースを提供します。
 */
class IShape : public er::object::IRelease, public er::object::IIsValid, public er::object::IDraw {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
	typedef detail::CSuspension<>	TSuspension;


private:


	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	typedef TSuspension::TPrimitive	TPrimitive;

	//描画タイプ
	struct EDraw {
		enum Type {
			Fill,	//<塗りつぶし
			Line,	//<枠
			Point,	//<点

			Max,
			None,
		};
	};

	//拡張プリミティブ情報
	struct SPrimitive : public TSuspension::TPrimitive {
		bool		is_2d_draw;	//<2D描画
		EDraw::Type	draw_type;	//<描画タイプ
	};

	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// IShape::Draw
	/*!
		描画

		@param	primitive	[in]	拡張プリミティブ情報
	 */
	// ==========================================================================
	static void Draw(const SPrimitive &primitive);
	using er::object::IDraw::Draw;


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// IShape::~IShape
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~IShape() {};


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
	// =============================================================================
	// IShape::drawTaskCb
	/*!
		描画タスクコールバック
	 */
	// ==========================================================================
	static void drawTaskCb(AMS_TCB *tcb) {
		void **work = reinterpret_cast<void **>(amTaskGetWork(tcb));
		SPrimitive &primitive = *reinterpret_cast<SPrimitive *>(work[0]);

		Draw(primitive);
	}


private:
	// =============================================================================
	// IShape::applyPrimitiveParameter
	/*!
		拡張プリミティブパラメータの反映

		@param	primitive	[in]	拡張プリミティブ情報
	 */
	// ==========================================================================
	static void applyPrimitiveParameter(const SPrimitive &primitive);


//------------------------------------------------------------------------------**********
}; //class IShape





































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	シェイプクラス
		シェイプを提供します。
 */
template <typename TType, Uint32 NVtxSize>
class CShape : public IShape {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef IShape						TSuperClass;
	typedef detail::CSuspension<TType>	TSuspension;


	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	typedef typename TSuperClass::SPrimitive	TPrimitive;
	typedef typename TSuspension::TVertex		TVertex;
	typedef typename TSuspension::TMatrix		TMatrix;
	typedef typename TSuperClass::EDraw			EDraw;
	static const Uint32	NSize = NVtxSize;


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CShape::Create
	/*!
		構築
	
		@retval	true	成功
		@retval	false	失敗

		@note
			失敗しません。
	 */
	// ==========================================================================
	bool Create() {
		Release();

		m_draw_prio = 0;
		m_draw_data.primitive.type = NNE_PRIM_TRIANGLE_STRIP;
		const bool c_is_vertex_only = !(TSuspension::c_has_color || TSuspension::c_has_texture || TSuspension::c_has_normal);
		m_draw_data.primitive.draw_type = ((c_is_vertex_only)? EDraw::Line: EDraw::Fill);
		amZeroMemory(&m_draw_data.vertex, sizeof(m_draw_data.vertex));
		TSuspension::InitPrimitiveZValue(m_draw_data.primitive);
		TSuspension::InitMatrix(m_draw_data.matrix);

		m_flag[BFlag::Create] = true;

		return true;
	}

	// =============================================================================
	// CShape::Release
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
	// CShape::IsValid
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
	// CShape::Draw
	/*!
		描画
	 */
	// ==========================================================================
	virtual void Draw() const {
		if (m_flag[BFlag::Create] && !m_flag[BFlag::NoDraw]) {
			if (amThreadCheckDraw()) {
				//描画タスクなら
				IShape::Draw(m_draw_data.primitive);
			} else {
				//描画タスクじゃないなら
				SDrawData *draw_data = reinterpret_cast<SDrawData *>(amDrawMallocDataBuffer(sizeof(SDrawData)));
				*draw_data = m_draw_data;
				amDrawMakeTask(drawTaskCb, m_draw_prio, reinterpret_cast<Uint32>(draw_data));
			}
		}
	}

	
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// =============================================================================
	// CShape::GetVertex
	/*!
		頂点の取得

		@param	idx		[in]	頂点番号

		@return 頂点情報
	 */
	// ==========================================================================
	const TVertex &GetVertex(Uint32 idx) const {
		return m_draw_data.vertex[idx];
	}

	// =============================================================================
	// CShape::GetMatrix
	/*!
		マトリックスの取得

		@return 頂点情報
	 */
	// ==========================================================================
	const TMatrix &GetMatrix() const {
		return m_draw_data.matrix;
	}

	// =============================================================================
	// CShape::GetDrawPrio
	/*!
		描画プライオリティの取得

		@return 描画プライオリティ
	 */
	// ==========================================================================
	Uint16 GetDrawPrio() const {
		return m_draw_prio;
	}

	// =============================================================================
	// CShape::GetDrawType
	/*!
		描画タイプの取得

		@return 描画タイプ
	 */
	// ==========================================================================
	EDraw::Type GetDrawType() const {
		return m_draw_data.primitive.draw_type;
	}

	// =============================================================================
	// CShape::IsAlpha
	/*!
		不透明確認

		@return 不透明か
	 */
	// ==========================================================================
	bool IsAlpha() const {
		return ((0 != m_draw_data.primitive.aTest)? true: false);
	}

	// =============================================================================
	// CShape::IsDraw
	/*!
		描画確認

		@return 描画するか
	 */
	// ==========================================================================
	bool IsDraw() const {
		return !m_flag[BFlag::NoDraw];
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// =============================================================================
	// CShape::SetVertex
	/*!
		頂点の設定

		@param	num		[in]	頂点番号(省略時は全て)
		@param	vertex	[in]	頂点情報
	 */
	// ==========================================================================
	void SetVertex(const TVertex &vertex) {
		if (m_flag[BFlag::Create]) {
			for (typename TVertexArray::iterator ite = m_draw_data.vertex.begin(), ite_end = m_draw_data.vertex.end(); ite != ite_end; ++ite) {
				*ite = vertex;
			}
		}
	}
	void SetVertex(Uint32 idx, const TVertex &vertex) {
		if (m_flag[BFlag::Create]) {
			amAssert(idx < m_draw_data.vertex.size());
			m_draw_data.vertex[idx] = vertex;
		}
	}

	// =============================================================================
	// CShape::SetMatrix
	/*!
		マトリックスの設定

		@param	matrix	[in]	頂点情報
	 */
	// ==========================================================================
	void SetMatrix(const TMatrix &matrix) {
		if (m_flag[BFlag::Create]) {
			m_draw_data.matrix = matrix;
		}
	}

	// =============================================================================
	// CShape::SetDrawPrio
	/*!
		描画プライオリティの設定

		@param	draw_prio	[in]	描画プライオリティ
	 */
	// ==========================================================================
	void SetDrawPrio(Uint16 draw_prio) {
		if (m_flag[BFlag::Create]) {
			m_draw_prio = draw_prio;
		}
	}

	// =============================================================================
	// CShape::SetDrawType
	/*!
		描画タイプの設定

		@param	draw_type	[in]	描画タイプ
	 */
	// ==========================================================================
	void SetDrawType(EDraw::Type draw_type) {
		if (m_flag[BFlag::Create]) {
			m_draw_data.primitive.draw_type = draw_type;
		}
	}

	// =============================================================================
	// CShape::SetAlpha
	/*!
		不透明度の設定

		@param	is_alpha	[in]	不透明か
	 */
	// ==========================================================================
	void SetAlpha(bool is_alpha) {
		m_draw_data.primitive.aTest = static_cast<Sint16>((is_alpha)? 1: 0);
	}

	// =============================================================================
	// CShape::SetAlpha
	/*!
		不透明度の設定

		@param	alpha	[in]	不透明度(透明:0～255:不透明)

		@note
			ブレンド設定とは排他です。

			全て頂点のα値を変更します。
	 */
	// ==========================================================================
	void SetAlpha(int alpha) {
		SetAlpha(static_cast<Uint8>(alpha));
	}
	void SetAlpha(Uint8 alpha) {
		bool is_alpha = (255 != alpha);
		SetAlpha(is_alpha);
		if (m_flag[BFlag::Create] && TSuspension::c_has_color) {
			for (typename TVertexArray::iterator ite = m_draw_data.vertex.begin(), ite_end = m_draw_data.vertex.end(); ite != ite_end; ++ite) {
				NNS_RGBA8888 &color = *TSuspension::GetColor(*ite);
				color = (color & 0xFFFFFF00) | (alpha & 0x000000FF);
			}
			m_draw_data.primitive.ablend = ((is_alpha)? NNE_PRIM_ALPHABLEND_ON: NNE_PRIM_ALPHABLEND_OFF);
			amDrawGetPrimBlendParam(AMDRAWE_BLENDTYPE_NORMAL, &m_draw_data.primitive);
		}
	}
	void SetAlpha(Float alpha) {
		SetAlpha(static_cast<Uint8>(alpha * 255.0f));
	}

	// =============================================================================
	// CShape::SetDraw
	/*!
		描画の設定

		@param	is_draw	[in]	描画する
	 */
	// ==========================================================================
	void SetDraw(bool is_draw) {
		if (m_flag[BFlag::Create]) {
			m_flag[BFlag::NoDraw] = !is_draw;
		}
	}


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// =============================================================================
	// CShape::CShape
	/*!
		デフォルトコンストラクタ
	 */
	// ==========================================================================
public:
	CShape() : m_flag() , m_draw_data() {}

	// =============================================================================
	// CShape::~CShape
	/*!
		デストラクタ
	 */
	// ==========================================================================
public:
	~CShape() {
		Release();
	}


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	struct BFlag {
		enum {
			Create,
			NoDraw,

			Max,
			None,
		};
		typedef accel::CBitset<Max> Type;
	};
	typedef accel::CArray<TVertex, NSize>	TVertexArray;
	//描画データ
	struct SDrawData {
		TPrimitive		primitive;	//<プリミティブ
		TVertexArray	vertex;		//<頂点配列
		TMatrix			matrix;		//<マトリックス(2Dなら使用しない)

		SDrawData () {
			amZeroMemory(&primitive, sizeof(primitive));
			primitive.count = static_cast<Sint32>(vertex.size());
			primitive.is_2d_draw = TSuspension::c_is_2d;
			TSuspension::SetPrimitiveFormat(primitive);
			TSuspension::SetPrimitiveVertexList(primitive, vertex);
			TSuspension::SetPrimitiveMatrix(primitive, matrix);
		}
		SDrawData(const SDrawData &rhs) {
			primitive = rhs.primitive;
			vertex = rhs.vertex;
			matrix = rhs.matrix;
			TSuspension::SetPrimitiveVertexList(primitive, vertex);
			TSuspension::SetPrimitiveMatrix(primitive, matrix);
		}
		const SDrawData &operator=(const SDrawData &rhs) {
			if (this != &rhs) {
				primitive = rhs.primitive;
				memcpy(vertex, rhs.vertex, sizeof(vertex));
				if (TSuspension::c_is_3d) {
					memcpy(matrix, rhs.matrix, sizeof(matrix));
				}
				TSuspension::SetPrimitiveVertexList(primitive, vertex);
				TSuspension::SetPrimitiveMatrix(primitive, matrix);
			}
			return *this;
		}
	};


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	typename BFlag::Type	m_flag;			//<フラグ
	SDrawData				m_draw_data;	//<描画データ
	Uint16					m_draw_prio;	//<描画プライオリティ


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class CShape




















} //namespace er
#endif //#if	defined(__cplusplus)

	// ============================================================================
	// CShape::Function
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
