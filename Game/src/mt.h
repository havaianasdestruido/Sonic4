// ==========================================================================
/*!
  @file mt.h
  @brief MT関連

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: mt.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef MT_H_
#define MT_H_



//----- Include Files -------------------------------------------------------
#include "typedef.h"

#include "mtMath.h"
#include "mtTask.h"
#include "mtMemory.h"
#include "mtPad.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
#if AMD_DEBUG
	#define	MTD_DEBUG	(1)
#else
	#define	MTD_RELEASE	(1)
#endif


#define	OS_TPrintf	NNM_TRACE
#define	OS_Printf	NNM_TRACE

/*!
  @defgroup MTD_XYZ
  @brief 座標系の定義
  
  座標などを配列で定義、アクセスする場合に使用します。
 */
//@{
#define MTD_X       (0) ///< Ｘ座標のインデックス
#define MTD_Y       (1) ///< Ｙ座標のインデックス
#define MTD_Z       (2) ///< Ｚ座標のインデックス
#define MTD_W       (3) ///< Ｗ座標のインデックス

#define MTD_XY      (2) ///< ２次元座標の定義
#define MTD_XYZ     (3) ///< ３次元座標の定義
#define MTD_XYZW    (4) ///< ４次元座標の定義
//@}

/*!
  @defgroup MTD_RECT
  @brief 矩形定義
  
  矩形を配列で定義、アクセスする場合に使用します。
 */
//@{
#define MTD_LEFT    (0) ///< 矩形左のインデックス
#define MTD_TOP     (1) ///< 矩形上のインデックス
#define MTD_RIGHT   (2) ///< 矩形右のインデックス
#define MTD_BOTTOM  (3) ///< 矩形下のインデックス

#define MTD_RECT    (4) ///< 矩形の定義
//@}

/*!
  @defgroup MTD_WH
  @brief サイズ定義
  
  サイズを配列で定義、アクセスする場合に使用します。
 */
//@{
#define MTD_WIDTH   (0) ///< 幅のインデックス
#define MTD_HEIGHT  (1) ///< 高さのインデックス
#define MTD_DEPTH   (2) ///< 奥行きのインデックス

#define MTD_WH      (2) ///< 幅と高さの定義
#define MTD_WHD     (3) ///< 幅と高さと奥行きの定義
//@}


//----- Macros --------------------------------------------------------------
// ==========================================================================
// MTM_ASSERT
/*!
	アサート
 */
// ==========================================================================
#define MTM_ASSERT(_e)	amAssert(_e)

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // MT_H_

//----- Include Files -------------------------------------------------------
