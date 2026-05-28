// ============================================================================
/*!
	@file	erShape.cpp
	@brief	シェイプ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: erShape.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "erShape.hpp"



//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*


namespace er {
namespace detail {
//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********
} //namespace detail








































//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// =============================================================================
// IShape::Draw
/*!
	描画

	@param	primitive	[in]	プリミティブ情報
 */
// ==========================================================================
void IShape::Draw(const SPrimitive &primitive)
{
	if (NULL != primitive.texlist) {
		nnSetPrimitiveTexNum(primitive.texlist, primitive.texId);
		nnSetPrimitiveTexState(NNE_PRIM_TEXBLEND_MODULATE, NNE_PRIM_TEXCOORD_UV, NNE_PRIM_TEXWRAP_REPEAT, NNE_PRIM_TEXWRAP_REPEAT);
	}

	applyPrimitiveParameter(primitive);
	if ((NNE_PRIM_ALPHABLEND_ON == primitive.ablend) && (EDraw::Fill == primitive.draw_type)) {
		//半透明
#if 0
		//ソート後描画
		AMS_COMMAND_HEADER			&command = *reinterpret_cast<AMS_COMMAND_HEADER *>(amDrawMallocWorkBuffer(sizeof(AMS_COMMAND_HEADER)));
		void						*work = amDrawMallocWorkBuffer(sizeof(AMS_PARAM_DRAW_PRIMITIVE) + sizeof(NNS_MATRIX));
		AMS_PARAM_DRAW_PRIMITIVE	&prmtv = *reinterpret_cast<AMS_PARAM_DRAW_PRIMITIVE *>(work);
		NNS_MATRIX					&matrix = *reinterpret_cast<NNS_MATRIX *>(reinterpret_cast<Uint32>(work) + sizeof(AMS_PARAM_DRAW_PRIMITIVE));
		
		nnCopyMatrix(&matrix, amMatrixGetCurrent());
		prmtv = primitive;
		prmtv.mtx = &matrix;

		amZeroMemory(&command, sizeof(command));
		command.param = &prmtv;
		command.command_id = ((primitive.is_2d_draw)? AMD_COMMAND_SORT_DRAW_PRIMITIVE2D: AMD_COMMAND_SORT_DRAW_PRIMITIVE3D);
		Sint32 z_value = static_cast<Sint32>(((primitive.is_2d_draw)? primitive.zOffset * 100.0f: primitive.sortZ * 100.0f));
		amDrawAddSort(&command, z_value);
#else
		//ソート描画が上手く動かないので暫定的に即時描画
		if (primitive.is_2d_draw) {
			//2D
			nnBeginDrawPrimitive2D(primitive.format2D, primitive.ablend);
			amDrawPrimitive2D(primitive.format2D, primitive.type, primitive.vtxPCT2D, primitive.count, primitive.zOffset);
			nnEndDrawPrimitive2D();
		} else {
			//3D
			nnBeginDrawPrimitive3D(primitive.format3D, primitive.ablend, NNE_PRIM_LIGHT_DISABLE, NNE_PRIM_CULL_NONE);
			nnDrawPrimitive3D(primitive.type, primitive.vtxPCT3D, primitive.count);
			nnEndDrawPrimitive3D();
		}
#endif
	} else {
		//不透明 もしくは 透明だけどソート描画不可
		if (primitive.is_2d_draw) {
			//2D
			nnBeginDrawPrimitive2D(primitive.format2D, primitive.ablend);
			switch (primitive.draw_type) {
			case EDraw::Fill:
				amDrawPrimitive2D(primitive.format2D, primitive.type, primitive.vtxPCT2D, primitive.count, primitive.zOffset);
				break;
			case EDraw::Line:
				{
					NNE_PRIM_LINE format = static_cast<NNE_PRIM_LINE>(primitive.type);
					amDrawPrimitiveLine2D(format, primitive.vtxPCT2D, primitive.count, primitive.zOffset);
					UNREFERENCED_PARAMETER(format);
				}
				break;
			case EDraw::Point:
				{
					NNE_PRIM2D_POINT_FMT format = static_cast<NNE_PRIM2D_POINT_FMT>(primitive.type);
					amDrawPrimitivePoint2D(format, primitive.vtxPCT2D, primitive.count, primitive.zOffset);
					UNREFERENCED_PARAMETER(format);
				}
				break;
			default:
				amAssert(!"Logic error! primitive.draw_type is failed");
				break;
			}
			nnEndDrawPrimitive2D();
		} else {
			//3D
			nnBeginDrawPrimitive3D(primitive.format3D, primitive.ablend, NNE_PRIM_LIGHT_DISABLE, NNE_PRIM_CULL_NONE);
			switch (primitive.draw_type) {
			case EDraw::Fill:
				nnDrawPrimitive3D(primitive.type, primitive.vtxPCT3D, primitive.count);
				break;
			case EDraw::Line:
				{
					NNE_PRIM_LINE format = static_cast<NNE_PRIM_LINE>(primitive.type);
					nnDrawPrimitiveLine3D(format, primitive.vtxPCT2D, primitive.count);
					UNREFERENCED_PARAMETER(format);
				}
				break;
			case EDraw::Point:
				{
					nnDrawPrimitivePoint3D(primitive.vtxPCT2D, primitive.count);
				}
				break;
			default:
				amAssert(!"Logic error! primitive.draw_type is failed");
				break;
			}
			nnEndDrawPrimitive3D();
		}
	}

}

// =============================================================================
// IShape::applyPrimitiveParameter
/*!
	拡張プリミティブパラメータの反映

	@param	primitive	[in]	拡張プリミティブ情報
 */
// ==========================================================================
void IShape::applyPrimitiveParameter(const SPrimitive &primitive)
{
	const bool is_2d = primitive.is_2d_draw;

#if _WII
	GXBool ztest = GX_TRUE;
	GXBool zmask = GX_FALSE;
	GXCompare zfunc = GX_LEQUAL;
#endif

	//αテスト
	if (primitive.aTest) {
		//αテストする
#if _PC | _XBOX 
		((is_2d)? nnSetPrimitive2DAlphaTestDXG20: nnSetPrimitive3DAlphaTestDXG20)(NNE_TRUE);
		((is_2d)? nnSetPrimitive2DAlphaFuncDXG20: nnSetPrimitive3DAlphaFuncDXG20)(NNE_CMPFUNC_GREATER, 0x10);
#elif _PS3
		((is_2d)? nnSetPrimitive2DAlphaFuncPS3: nnSetPrimitive3DAlphaFuncPS3)(NND_CMPFUNC_PS3_GREATER, 0x10);
#elif _WII
		((is_2d)? nnSetPrimitive2DAlphaCompareGC: nnSetPrimitive3DAlphaCompareGC)(GX_GREATER, 0x10, GX_AOP_AND, GX_GREATER, 0x10);
#elif _IPHONE
		((is_2d)? nnSetPrimitive2DAlphaFuncGL: nnSetPrimitive3DAlphaFuncGL)(NND_CMPFUNC_GL_GREATER, 0x10);
#endif
	} else {
		//αテストしない
#if _PC | _XBOX
		((is_2d)? nnSetPrimitive2DAlphaTestDXG20: nnSetPrimitive3DAlphaTestDXG20)(NNE_FALSE);
#elif _PS3
		((is_2d)? nnSetPrimitive2DAlphaFuncPS3: nnSetPrimitive3DAlphaFuncPS3)(NND_CMPFUNC_PS3_ALWAYS, 0x10);
#elif _WII
		((is_2d)? nnSetPrimitive2DAlphaCompareGC: nnSetPrimitive3DAlphaCompareGC)(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
#elif _IPHONE
		((is_2d)? nnSetPrimitive2DAlphaFuncGL: nnSetPrimitive3DAlphaFuncGL)(NND_CMPFUNC_GL_ALWAYS, 0x10);
#endif
	}


	//Ｚマスク
	if (primitive.zMask) {
		//Ｚバッファを更新しない
#if _PC | _XBOX
		((is_2d)? nnSetPrimitive2DDepthMaskDXG20: nnSetPrimitive3DDepthMaskDXG20)(NNE_FALSE);
#elif _PS3
		((is_2d)? nnSetPrimitive2DDepthMaskPS3: nnSetPrimitive3DDepthMaskPS3)(NNE_FALSE);
#elif _WII
		zmask = GX_FALSE;
#elif _IPHONE
		((is_2d)? nnSetPrimitive2DDepthMaskGL: nnSetPrimitive3DDepthMaskGL)(NNE_FALSE);
#endif
	} else {
		//Ｚバッファを更新する
#if _PC | _XBOX
		((is_2d)? nnSetPrimitive2DDepthMaskDXG20: nnSetPrimitive3DDepthMaskDXG20)(NNE_TRUE);
#elif _PS3
		((is_2d)? nnSetPrimitive2DDepthMaskPS3: nnSetPrimitive3DDepthMaskPS3)(NNE_TRUE);
#elif _WII
		zmask = GX_TRUE;
#elif _IPHONE
		((is_2d)? nnSetPrimitive2DDepthMaskGL: nnSetPrimitive3DDepthMaskGL)(NNE_TRUE);
#endif
	}

	// Ｚテスト
	if(primitive.zTest) {
		//Ｚテストする
#if _PC | _XBOX
		((is_2d)? nnSetPrimitive2DDepthTestDXG20: nnSetPrimitive3DDepthTestDXG20)(NNE_TRUE);
		((is_2d)? nnSetPrimitive2DDepthFuncDXG20: nnSetPrimitive3DDepthFuncDXG20)(NNE_CMPFUNC_LESSEQUAL);
#elif _PS3
		((is_2d)? nnSetPrimitive2DDepthFuncPS3: nnSetPrimitive3DDepthFuncPS3)(NND_CMPFUNC_PS3_LEQUAL);
#elif _WII
		ztest = GX_TRUE;
		zfunc = GX_LEQUAL;
#elif _IPHONE
		((is_2d)? nnSetPrimitive2DDepthFuncGL: nnSetPrimitive3DDepthFuncGL)(NND_CMPFUNC_GL_LEQUAL);
#endif
	} else {
		//Ｚテストしない
#if _PC | _XBOX
		((is_2d)? nnSetPrimitive2DDepthTestDXG20: nnSetPrimitive3DDepthTestDXG20)(NNE_FALSE);
#elif _PS3
		((is_2d)? nnSetPrimitive2DDepthFuncPS3: nnSetPrimitive3DDepthFuncPS3)(NND_CMPFUNC_PS3_ALWAYS);
#elif _WII
		ztest = GX_FALSE;
		zfunc = GX_NEVER;
#elif _IPHONE
		((is_2d)? nnSetPrimitive2DDepthFuncGL: nnSetPrimitive3DDepthFuncGL)(NND_CMPFUNC_GL_ALWAYS);
#endif
	}

#if _WII
	((is_2d)? nnSetPrimitive2DZModeGC: nnSetPrimitive3DZModeGC)(ztest, zfunc, zmask);
#endif

	// αブレンド
	if (primitive.ablend) {
	// αブレンドする
#if _PC | _XBOX
	((is_2d)? nnSetPrimitive2DBlendDXG20: nnSetPrimitive3DBlendDXG20)(primitive.bldSrc, primitive.bldDst, primitive.bldMode);
#elif _PS3
	((is_2d)? nnSetPrimitive2DBlendPS3: nnSetPrimitive3DBlendPS3)(primitive.bldSrc, primitive.bldDst, primitive.bldMode);
#elif _WII
	((is_2d)? nnSetPrimitive2DBlendModeGC: nnSetPrimitive3DBlendModeGC)(primitive.bldMode, primitive.bldSrc, primitive.bldDst, GX_LO_NOOP);
#elif _IPHONE
	nnSetPrimitiveBlend(NNE_PRIM_BLEND_ADD);
#endif
	} else {
	// αブレンドしない
#if _WII
		nnSetPrimitiveBlend(NNE_PRIM_BLEND_BLEND);
#elif _IPHONE
		nnSetPrimitiveBlend(NNE_PRIM_BLEND_BLEND);
#endif	
	}

	//3D行列
	if (!is_2d) {
		//射影行列
		NNS_MATRIX44 matrix;
		nnMakeOrthoMatrix(&matrix, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f);
		amDrawSetProjection(&matrix, NNE_PROJECTION_TYPE_ORTHO);
		//ビュー行列
#if !_IPHONE
		amDrawSetWorldViewMatrix(primitive.mtx);
		nnSetPrimitive3DMatrix(primitive.mtx);
#else //!_IPHONE
		NNS_MATRIX mtx;
		nnMakeUnitMatrix(&mtx);
		nnTranslateMatrix(&mtx, &mtx, 0.5f, 0.5f, 0.0f);
		nnRotateZMatrix(&mtx, &mtx, NNM_DEGtoA32(90.0f));
		nnTranslateMatrix(&mtx, &mtx, -0.5f, -0.5f, 0.0f);
		nnMultiplyMatrix(&mtx, primitive.mtx, &mtx);
		amDrawSetWorldViewMatrix(&mtx);
		nnSetPrimitive3DMatrix(&mtx);
#endif //!_IPHONE
	}
}

//------------------------------------------------------------------------------**********



















































} //namespace er

// =============================================================================
// Function
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
// ==========================================================================
