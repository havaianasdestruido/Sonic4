// ==========================================================================
/*!
  @file gmComEfct.h
  @brief エフェクト 共通使用タイプ

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmComEfct.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_COM_EFCT_H_
#define GM_COM_EFCT_H_


//----- Include Files -------------------------------------------------------
#include "gmEffect.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// リング
// ==========================================================================
// ==========================================================================
// GmComEfctCreateRing
/*!
 *	リング
 */
// ==========================================================================
extern void GmComEfctCreateRing(fx32 pos_x, fx32 pos_y);

// ==========================================================================
// 敵倒し煙
// ==========================================================================
// ==========================================================================
// GmComEfctCreateEneDeadSmoke
/*!
 *	敵倒し煙
 */
// ==========================================================================
extern void GmComEfctCreateEneDeadSmoke(OBS_OBJECT_WORK *obj_work, fx32 ofst_x, fx32 ofst_y);

// ==========================================================================
// ヒット
// ==========================================================================
// ==========================================================================
// GmComEfctCreateHitPlayer
/*!
 *	プレイヤーがヒット
 */
// ==========================================================================
extern void GmComEfctCreateHitPlayer(OBS_OBJECT_WORK *obj_work, fx32 ofst_x, fx32 ofst_y);

// ==========================================================================
// GmComEfctCreateHitEnemy
/*!
 *	敵がヒット
 *
 *	@note
 *		user_timer	: エフェクト生成間隔
 */
// ==========================================================================
extern void GmComEfctCreateHitEnemy(OBS_OBJECT_WORK *obj_work, fx32 ofst_x, fx32 ofst_y);

// ==========================================================================
// スプリング
// ==========================================================================
// ==========================================================================
// GmComEfctCreateSpring
/*!
 *	スプリング
 */
// ==========================================================================
extern void GmComEfctCreateSpring(OBS_OBJECT_WORK *obj_work, fx32 ofst_x, fx32 ofst_y, fx32 ofst_z = 0);


// ==========================================================================
// Utility
// ==========================================================================
// ==========================================================================
// GmComEfctSetDispOffset
/*!
 *	エフェクトオフセット設定
 *
 *	@param	efct_work	[in]	生成したエフェクト
 *	@param	ofst_x		[in]	オフセットX
 *	@param	ofst_y		[in]	オフセットY
 *	@param	ofst_z		[in]	オフセットZ
 */
// ==========================================================================
extern void GmComEfctSetDispOffset(GMS_EFFECT_3DES_WORK *efct_work, fx32 ofst_x, fx32 ofst_y, fx32 ofst_z);

// ==========================================================================
// GmComEfctSetDispOffsetF
/*!
 *	エフェクトオフセット設定
 *
 *	@param	efct_work	[in]	生成したエフェクト
 *	@param	ofst_x		[in]	オフセットX
 *	@param	ofst_y		[in]	オフセットY
 *	@param	ofst_z		[in]	オフセットZ
 */
// ==========================================================================
extern void GmComEfctSetDispOffsetF(GMS_EFFECT_3DES_WORK *efct_work, float ofst_x, float ofst_y, float ofst_z);

// ==========================================================================
// GmComEfctAddDispOffset
/*!
 *	エフェクトオフセット加算設定
 *
 *	@param	efct_work	[in]	生成したエフェクト
 *	@param	ofst_x		[in]	オフセットX
 *	@param	ofst_y		[in]	オフセットY
 *	@param	ofst_z		[in]	オフセットZ
 */
// ==========================================================================
extern void GmComEfctAddDispOffset(GMS_EFFECT_3DES_WORK *efct_work, fx32 ofst_x, fx32 ofst_y, fx32 ofst_z);

// ==========================================================================
// GmComEfctAddDispOffsetF
/*!
 *	エフェクトオフセット加算設定
 *
 *	@param	efct_work	[in]	生成したエフェクト
 *	@param	ofst_x		[in]	オフセットX
 *	@param	ofst_y		[in]	オフセットY
 *	@param	ofst_z		[in]	オフセットZ
 */
// ==========================================================================
extern void GmComEfctAddDispOffsetF(GMS_EFFECT_3DES_WORK *efct_work, float ofst_x, float ofst_y, float ofst_z);

// ==========================================================================
// GmComEfctSetDispRotation
/*!
 *	エフェクト回転設定
 *
 *	@param	efct_work	[in]	生成したエフェクト
 *	@param	dir_x		[in]	回転X
 *	@param	dir_y		[in]	回転Y
 *	@param	dir_z		[in]	回転Z
 */
// ==========================================================================
extern void GmComEfctSetDispRotation(GMS_EFFECT_3DES_WORK *efct_work, u16 dir_x, u16 dir_y, u16 dir_z);

// ==========================================================================
// GmComEfctSetDispRotationS
/*!
 *	エフェクト回転設定
 *
 *	@param	efct_work	[in]	生成したエフェクトv
 */
// ==========================================================================
extern void GmComEfctSetDispRotationS(GMS_EFFECT_3DES_WORK *efct_work, Angle16 dir_x, Angle16 dir_y, Angle16 dir_z);

// ==========================================================================
// GmComEfctAddDispRotation
/*!
 *	エフェクト回転設定
 *
 *	@param	efct_work	[in]	生成したエフェクト
 *	@param	dir_x		[in]	回転X
 *	@param	dir_y		[in]	回転Y
 *	@param	dir_z		[in]	回転Z
 */
// ==========================================================================
extern void GmComEfctAddDispRotation(GMS_EFFECT_3DES_WORK *efct_work, u16 dir_x, u16 dir_y, u16 dir_z);

// ==========================================================================
// GmComEfctAddDispRotationS
/*!
 *	エフェクト回転設定
 *
 *	@param	efct_work	[in]	生成したエフェクトv
 */
// ==========================================================================
extern void GmComEfctAddDispRotationS(GMS_EFFECT_3DES_WORK *efct_work, Angle16 dir_x, Angle16 dir_y, Angle16 dir_z);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _PT_H_

//----- Include Files -------------------------------------------------------
