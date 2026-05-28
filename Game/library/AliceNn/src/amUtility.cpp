// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       amUtility.cpp
	@brief      ユーティリティライブラリ
	@author	    Syuichi Gotou
	@date	    Date: 2009/04/01 
 */
// ================================================================

//----- Include Files --------------------------------------------------
#include "alice.h"

//----- Definitions ----------------------------------------------------
//----- Macros ---------------------------------------------------------

//----- Local Functions -----------------------------------------------
void _amUtilTask_DrawGrid(AMS_TCB *tcbp);
void _amUtilTask_DrawAxis(AMS_TCB *tcbp);

//----- Global Functions -----------------------------------------------

// ================================================================
/*!
	グリッド作成（内部で描画タスク生成）

	@param range   [input] 線と線の幅
	@param linenum [input] 縦線の数（横線もこの数値と同じとみなす）
	@param type    [input] グリッドタイプ
	@param prio    [input] 描画タスクのプライオリティ
*/
// ================================================================
void amUtilMakeGrid(float range, Sint32 linenum, Sint32 type, Uint16 prio)
{
	Sint32		work_data[2];
	NNS_PRIM3D_P *lineData;
	float width = range * (linenum-1); // グリッド全体の幅
	float width0 = -width/2;           // グリッドの一番左側の座標
	float height0 = width0;            // グリッドの一番下側の座標

	lineData = (NNS_PRIM3D_P *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_P) * linenum * 4);
	
	switch ( type )
	{
	case AMUTL_GRIDTYPE_XY: // XY平面グリッド
		// 縦の線を作る
		for (int i = 0; i < linenum*2; i = i+2)
		{
		    lineData[i].Pos.x = width0;
			lineData[i].Pos.y = height0;
			lineData[i].Pos.z = 0.0f;
			lineData[i+1].Pos.x = width0;
			lineData[i+1].Pos.y = -height0;
			lineData[i+1].Pos.z = 0.0f;
			width0 += range;
		}

		// 横の線を作る
		width0 = -width/2;
		for (int i = linenum*2; i < linenum*4; i = i+2)
		{
		    lineData[i].Pos.x = width0;
			lineData[i].Pos.y = height0;
			lineData[i].Pos.z = 0.0f;
			lineData[i+1].Pos.x = -width0;
			lineData[i+1].Pos.y = height0;
			lineData[i+1].Pos.z = 0.0f;
			height0 += range;
		}
		break;

	case AMUTL_GRIDTYPE_YZ: // YZ平面グリッド
		// 縦の線を作る
		for (int i = 0; i < linenum*2; i = i+2)
		{
		    lineData[i].Pos.x = 0.0f;
			lineData[i].Pos.y = height0;
			lineData[i].Pos.z = width0;
			lineData[i+1].Pos.x = 0.0f;
			lineData[i+1].Pos.y = -height0;
			lineData[i+1].Pos.z = width0;
			width0 += range;
		}

		// 横の線を作る
		width0 = -width/2;
		for (int i = linenum*2; i < linenum*4; i = i+2)
		{
		    lineData[i].Pos.x = 0.0f;
			lineData[i].Pos.y = height0;
			lineData[i].Pos.z = width0;
			lineData[i+1].Pos.x = 0.0f;
			lineData[i+1].Pos.y = height0;
			lineData[i+1].Pos.z = -width0;
			height0 += range;
		}
		break;

	case AMUTL_GRIDTYPE_ZX: // ZX平面グリッド
		// 縦の線を作る
		for (int i = 0; i < linenum*2; i = i+2)
		{
		    lineData[i].Pos.x = width0;
			lineData[i].Pos.y = 0.0f;
			lineData[i].Pos.z = height0;
			lineData[i+1].Pos.x = width0;
			lineData[i+1].Pos.y = 0.0f;
			lineData[i+1].Pos.z = -height0;
			width0 += range;
		}

		// 横の線を作る
		width0 = -width/2;
		for (int i = linenum*2; i < linenum*4; i = i+2)
		{
		    lineData[i].Pos.x = width0;
			lineData[i].Pos.y = 0.0f;
			lineData[i].Pos.z = height0;
			lineData[i+1].Pos.x = -width0;
			lineData[i+1].Pos.y = 0.0f;
			lineData[i+1].Pos.z = height0;
			height0 += range;
		}
		break;
	}

	work_data[0] = (Sint32)lineData;
	work_data[1] = (Sint32)linenum;
	amDrawMakeTask(_amUtilTask_DrawGrid, prio, work_data);
}

// ================================================================
/*!
	座標軸作成（内部で描画タスク生成）

	@param len [input] 座標軸の長さ
	@param prio[input] 描画タスクのプライオリティ
*/
// ================================================================
void amUtilMakeAxis(Sint32 len, Uint16 prio)
{
	Sint32		work_data[2];

	work_data[0] = len;  // 座標軸の長さ
	amDrawMakeTask(_amUtilTask_DrawAxis, prio, work_data);
}

void amZeroMemory(void *mem, size_t size)
{
		// __dcbz128(int offset,void *pBase);
		//void* address = (RegA + RegB) & ~127;
		//memset(address, 0, 128);

	memset(mem, 0, size);
	return;
}

// ================================================================
/*!
	アライメントを考慮したサイズ計算

	@param	 size  [i]	サイズ
	@param	 align [i]	アライメント
	@return			    アライメントを考慮したサイズ
	@note			    アライメントは２の累乗以外で動作は保証しない
*/
// ================================================================
size_t amCalcAlignSize(size_t size, size_t align)
{
	if (align > 1)
	{
#if AMD_DEBUG
		// ２の累乗チェック
		size_t a = align;

		for (int i = 0; i < 32; ++i, a >>= 1)
		{
			if (a & 0x1)
				break;
		}
		amAssert(a == 0x1);

#endif // AMD_DEBUG
		align--;
	}

	return ((size + align) & ~align);
}

//----- Local Functions -----------------------------------------------

// ================================================================
/*!
	グリッド描画タスク
*/
// ================================================================
void _amUtilTask_DrawGrid(AMS_TCB *tcbp)
{
	UNREFERENCED_PARAMETER(tcbp);

	Sint32		*work = (Sint32 *)amTaskGetWork(tcbp);
	NNS_MATRIX  mtx;
	Sint32 linenum = work[1];     // 縦線の数
	NNS_RGBA	LineColor = { 0.5f, 0.5f, 0.5f, 0.5f };

	// マトリクス計算
	nnMakeUnitMatrix(&mtx);
	nnMultiplyMatrix( &mtx, amDrawGetWorldViewMatrix(), &mtx );
	nnSetPrimitive3DMatrix( &mtx );

#if _PC | _XBOX
	nnSetPrimitive3DAlphaTestDXG20(NNE_FALSE);
	nnSetPrimitive3DDepthMaskDXG20(NNE_TRUE);
	nnSetPrimitive3DDepthTestDXG20(NNE_TRUE);
	nnSetPrimitive3DDepthFuncDXG20(NNE_CMPFUNC_LESS);
#elif _PS3
    nnSetPrimitive3DAlphaFuncPS3(NND_CMPFUNC_PS3_ALWAYS, 0.5f);
	nnSetPrimitive3DDepthMaskPS3(NNE_TRUE);
	nnSetPrimitive3DDepthFuncPS3(NND_CMPFUNC_PS3_LESS);
#endif

	// グリッド描画
	NNS_PRIM3D_P *lineData = (NNS_PRIM3D_P*)work[0];
	nnBeginDrawPrimitiveLine3D( &LineColor, NNE_PRIM_ALPHABLEND_OFF );
	nnDrawPrimitiveLine3D( NNE_PRIM_LINE_LIST, lineData, linenum*4 );
	nnEndDrawPrimitiveLine3D();
}

// ================================================================
/*!
	座標軸描画タスク
*/
// ================================================================
void _amUtilTask_DrawAxis(AMS_TCB *tcbp)
{
	UNREFERENCED_PARAMETER(tcbp);

    Sint32 *work = (Sint32 *)amTaskGetWork(tcbp);
	NNS_MATRIX mtx;

	NNS_PRIM3D_P	axis_line[] = {
		{{0.0F, 0.0F, 0.0F}},
		{{1.0F, 0.0F, 0.0F}},
	};
	NNS_RGBA	axisColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	float length = (float)work[0];

	// マトリクス計算
	nnMakeUnitMatrix(&mtx);
	nnMultiplyMatrix( &mtx, amDrawGetWorldViewMatrix(), &mtx );
	nnSetPrimitive3DMatrix( &mtx );

#if _PC | _XBOX
	nnSetPrimitive3DAlphaTestDXG20(NNE_FALSE);
	nnSetPrimitive3DDepthMaskDXG20(NNE_TRUE);
	nnSetPrimitive3DDepthTestDXG20(NNE_TRUE);
	nnSetPrimitive3DDepthFuncDXG20(NNE_CMPFUNC_LESS);
#elif _PS3
    nnSetPrimitive3DAlphaFuncPS3(NND_CMPFUNC_PS3_ALWAYS, 0.5f);
	nnSetPrimitive3DDepthMaskPS3(NNE_TRUE);
	nnSetPrimitive3DDepthFuncPS3(NND_CMPFUNC_PS3_LESS);
#endif

	// x 軸
	axis_line[1].Pos.x = length;
	nnBeginDrawPrimitiveLine3D( &axisColor, NNE_PRIM_ALPHABLEND_OFF );
	nnDrawPrimitiveLine3D( NNE_PRIM_LINE_STRIP, axis_line, 2 );
	nnEndDrawPrimitiveLine3D();

	// y 軸
	axisColor.r = 0.0f;
    axisColor.g = 1.0f;
	axis_line[1].Pos.x = 0.0f;
	axis_line[1].Pos.y = length;
	axis_line[1].Pos.z = 0.0f;
	nnBeginDrawPrimitiveLine3D( &axisColor, NNE_PRIM_ALPHABLEND_OFF );
	nnDrawPrimitiveLine3D( NNE_PRIM_LINE_STRIP, axis_line, 2 );
	nnEndDrawPrimitiveLine3D();

	// z 軸
	axisColor.g = 0.0f;
	axisColor.b = 1.0f;
    axis_line[1].Pos.x = 0.0f;
	axis_line[1].Pos.y = 0.0f;
	axis_line[1].Pos.z = length;
	nnBeginDrawPrimitiveLine3D( &axisColor, NNE_PRIM_ALPHABLEND_OFF );
	nnDrawPrimitiveLine3D( NNE_PRIM_LINE_STRIP, axis_line, 2 );
	nnEndDrawPrimitiveLine3D();
}
