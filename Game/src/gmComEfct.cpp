// ==========================================================================
/*!
  @file gmComEfct.cpp
  @brief エフェクト 共通使用タイプ

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmComEfct.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"

#include "objObject.h"
#include "gmMain.h"
#include "gmGameDat.h"
#include "gmObj.h"
#include "gmPlayer.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectZone.h"

#include "gmComEfct.h"

//----- Definitions ---------------------------------------------------------
#define GMD_COM_EFCT_OFST_FRONT_OBJ		(16.f)
#define GMD_COM_EFCT_OFST_FRONT_OBJ_FX	(16*FX32_ONE)
#define GMD_COM_EFCT_OFST_FRONT_A		((float)(GMD_OBJ_DEFAULT_POS_Z_A_FRONT/FX32_ONE))
#define GMD_COM_EFCT_OFST_FRONT_A_FX	(GMD_OBJ_DEFAULT_POS_Z_A_FRONT)

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// リング
// ==========================================================================
// ==========================================================================
// GmComEfctCreateRing
/*!
 *	リング
 */
// ==========================================================================
void GmComEfctCreateRing(fx32 pos_x, fx32 pos_y)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	efct_work = GmEfctCmnEsCreate(NULL, GME_EFCT_CMN_IDX_RING);

	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	//efct_work->efct_com.obj_work.ppFunc = gmPlyEfctSweatMain;

	// 位置調整 USE FLIP
	//GmEffect3DESSetDispOffset(efct_work, -8.f, 10.f, -5.f);
	// 位置調整
	efct_work->efct_com.obj_work.pos.x = pos_x;
	efct_work->efct_com.obj_work.pos.y = pos_y;
	efct_work->efct_com.obj_work.pos.z = GMD_OBJ_DEFAULT_POS_Z_A_FRONT;
}

// ==========================================================================
// 敵倒し煙
// ==========================================================================
// ==========================================================================
// GmComEfctCreateEneDeadSmoke
/*!
 *	敵倒し煙
 */
// ==========================================================================
void GmComEfctCreateEneDeadSmoke(OBS_OBJECT_WORK *obj_work, fx32 ofst_x, fx32 ofst_y)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	efct_work = GmEfctCmnEsCreate(NULL, GME_EFCT_CMN_IDX_BOMB_SMOKE);

	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_3) {
		if (GmMainIsWaterLevel() &&
				((obj_work->pos.y + ofst_y - 48*FX32_ONE) >> FX32_SHIFT) > g_gm_main_system.water_level) {
			efct_work = GmEfctZoneEsCreate(obj_work, GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_BOMB_SMOKE_Z3);
		}
		else {
			efct_work = GmEfctCmnEsCreate(NULL, GME_EFCT_CMN_IDX_BOMB_SMOKE);
		}
	}
	else {
		efct_work = GmEfctCmnEsCreate(NULL, GME_EFCT_CMN_IDX_BOMB_SMOKE);
	}

	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	//efct_work->efct_com.obj_work.ppFunc = gmPlyEfctBubbleMain;

	// 親クリア
	efct_work->efct_com.obj_work.parent_obj = NULL;

	// 位置調整 USE FLIP
	efct_work->efct_com.obj_work.pos = obj_work->pos;
	//GmEffect3DESSetDispOffset(efct_work,
	//			-16.f,
	//			FXM_FX32_TO_FLOAT(-ofst_y),
	//			FXM_FX32_TO_FLOAT(ofst_x));
	GmComEfctSetDispOffset(efct_work, ofst_x, ofst_y, GMD_COM_EFCT_OFST_FRONT_A_FX);
}

// ==========================================================================
// ヒット
// ==========================================================================
// ==========================================================================
// GmComEfctCreateHitPlayer
/*!
 *	プレイヤーがヒット
 */
// ==========================================================================
void GmComEfctCreateHitPlayer(OBS_OBJECT_WORK *obj_work, fx32 ofst_x, fx32 ofst_y)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	//efct_work = GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_HIT_S);
	efct_work = GmEfctCmnEsCreate(NULL, GME_EFCT_CMN_IDX_HIT_S);

	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	//efct_work->efct_com.obj_work.ppFunc = gmPlyEfctBubbleMain;

	// 位置調整 USE FLIP
	efct_work->efct_com.obj_work.pos = obj_work->pos;
	//GmEffect3DESSetDispOffset(efct_work,
	//			-16.f,
	//			FXM_FX32_TO_FLOAT(-ofst_y),
	//			FXM_FX32_TO_FLOAT(ofst_x));
	GmComEfctSetDispOffset(efct_work, ofst_x, ofst_y, GMD_COM_EFCT_OFST_FRONT_A_FX);
}

// ==========================================================================
// GmComEfctCreateHitEnemy
/*!
 *	敵がヒット
 *
 *	@note
 *		user_timer	: エフェクト生成間隔
 */
// ==========================================================================
void GmComEfctCreateHitEnemy(OBS_OBJECT_WORK *obj_work, fx32 ofst_x, fx32 ofst_y)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	efct_work = GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_HIT_E);

	// GmEffectDefaultMainFuncDeleteAtEnd
	// メイン処理差し替え
	//efct_work->efct_com.obj_work.ppFunc = gmPlyEfctBubbleMain;

	// 位置調整 USE FLIP
	//efct_work->efct_com.obj_work.pos = obj_work->pos;
	//GmEffect3DESSetDispOffset(efct_work,
	//			-16.f,
	//			FXM_FX32_TO_FLOAT(-ofst_y),
	//			FXM_FX32_TO_FLOAT(ofst_x));
	GmComEfctSetDispOffset(efct_work, ofst_x, ofst_y, GMD_COM_EFCT_OFST_FRONT_A_FX);
}

// ==========================================================================
// スプリング
// ==========================================================================
// ==========================================================================
// GmComEfctCreateSpring
/*!
 *	スプリング
 */
// ==========================================================================
void GmComEfctCreateSpring(OBS_OBJECT_WORK *obj_work, fx32 ofst_x, fx32 ofst_y, fx32 ofst_z)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	efct_work = GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_SPRING);

	// 位置調整
	//GmEffect3DESSetDispOffset(efct_work, FXM_FX32_TO_FLOAT(ofst_x), FXM_FX32_TO_FLOAT(-ofst_y), 16.f);
	GmComEfctSetDispOffset(efct_work, ofst_x, ofst_y, GMD_COM_EFCT_OFST_FRONT_OBJ_FX + ofst_z);
	//GmEffect3DESSetDispRotation(efct_work, 0, 0, 0);
	//GmEffect3DESAddDispOffset();
	//GmEffect3DESAddDispRotation();
#if _IPHONE
	efct_work->obj_3des.ecb->drawObjState = OBD_DRAW_CMD_STATE_3DNN; // 描画コマンドを通常へ
#endif // _IPHONE


	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	// メイン処理差し替え
	//efct_work->efct_com.obj_work.ppFunc = gmPlyEfctRollDashMain;
}


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
void GmComEfctSetDispOffset(GMS_EFFECT_3DES_WORK *efct_work, fx32 ofst_x, fx32 ofst_y, fx32 ofst_z)
{
	if (efct_work->efct_com.obj_work.disp_flag & OBD_DISP_NODIRFLIP) {
		GmEffect3DESSetDispOffset(efct_work,
								FXM_FX32_TO_FLOAT(ofst_x),
								FXM_FX32_TO_FLOAT(-ofst_y),
								FXM_FX32_TO_FLOAT(ofst_z));
	}
	else {
		GmEffect3DESSetDispOffset(efct_work,
								FXM_FX32_TO_FLOAT(-ofst_z),
								FXM_FX32_TO_FLOAT(-ofst_y),
								FXM_FX32_TO_FLOAT(ofst_x));
	}
}

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
void GmComEfctSetDispOffsetF(GMS_EFFECT_3DES_WORK *efct_work, float ofst_x, float ofst_y, float ofst_z)
{
	if (efct_work->efct_com.obj_work.disp_flag & OBD_DISP_NODIRFLIP) {
		GmEffect3DESSetDispOffset(efct_work, ofst_x, -ofst_y, ofst_z);
	}
	else {
		GmEffect3DESSetDispOffset(efct_work, -ofst_z, -ofst_y, ofst_x);
	}
}

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
void GmComEfctAddDispOffset(GMS_EFFECT_3DES_WORK *efct_work, fx32 ofst_x, fx32 ofst_y, fx32 ofst_z)
{
	if (efct_work->efct_com.obj_work.disp_flag & OBD_DISP_NODIRFLIP) {
		GmEffect3DESAddDispOffset(efct_work,
						FXM_FX32_TO_FLOAT(ofst_x),
						FXM_FX32_TO_FLOAT(-ofst_y),
						FXM_FX32_TO_FLOAT(ofst_z));
	}
	else {
		GmEffect3DESAddDispOffset(efct_work,
						FXM_FX32_TO_FLOAT(-ofst_z),
						FXM_FX32_TO_FLOAT(-ofst_y),
						FXM_FX32_TO_FLOAT(ofst_x));
	}
}

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
void GmComEfctAddDispOffsetF(GMS_EFFECT_3DES_WORK *efct_work, float ofst_x, float ofst_y, float ofst_z)
{
	if (efct_work->efct_com.obj_work.disp_flag & OBD_DISP_NODIRFLIP) {
		GmEffect3DESAddDispOffset(efct_work, ofst_x, -ofst_y, ofst_z);
	}
	else {
		GmEffect3DESAddDispOffset(efct_work, -ofst_z, -ofst_y, ofst_x);
	}
}

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
void GmComEfctSetDispRotation(GMS_EFFECT_3DES_WORK *efct_work, u16 dir_x, u16 dir_y, u16 dir_z)
{
	if (efct_work->efct_com.obj_work.disp_flag & OBD_DISP_NODIRFLIP) {
		GmEffect3DESSetDispRotation(efct_work, (Angle16)dir_x, (Angle16)dir_y, (Angle16)dir_z);
	}
	else {
		GmEffect3DESSetDispRotation(efct_work, (Angle16)-dir_z, (Angle16)-dir_y, (Angle16)dir_x);
	}
}

// ==========================================================================
// GmComEfctSetDispRotationS
/*!
 *	エフェクト回転設定
 *
 *	@param	efct_work	[in]	生成したエフェクトv
 */
// ==========================================================================
void GmComEfctSetDispRotationS(GMS_EFFECT_3DES_WORK *efct_work, Angle16 dir_x, Angle16 dir_y, Angle16 dir_z)
{
	if (efct_work->efct_com.obj_work.disp_flag & OBD_DISP_NODIRFLIP) {
		GmEffect3DESSetDispRotation(efct_work, dir_x, dir_y, dir_z);
	}
	else {
		GmEffect3DESSetDispRotation(efct_work, (Angle16)-dir_z, (Angle16)-dir_y, dir_x);
	}
}

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
void GmComEfctAddDispRotation(GMS_EFFECT_3DES_WORK *efct_work, u16 dir_x, u16 dir_y, u16 dir_z)
{
	if (efct_work->efct_com.obj_work.disp_flag & OBD_DISP_NODIRFLIP) {
		GmEffect3DESAddDispRotation(efct_work, (Angle16)dir_x, (Angle16)dir_y, (Angle16)dir_z);
	}
	else {
		GmEffect3DESAddDispRotation(efct_work, (Angle16)-dir_z, (Angle16)-dir_y, (Angle16)dir_x);
	}
}

// ==========================================================================
// GmComEfctAddDispRotationS
/*!
 *	エフェクト回転設定
 *
 *	@param	efct_work	[in]	生成したエフェクトv
 */
// ==========================================================================
void GmComEfctAddDispRotationS(GMS_EFFECT_3DES_WORK *efct_work, Angle16 dir_x, Angle16 dir_y, Angle16 dir_z)
{
	if (efct_work->efct_com.obj_work.disp_flag & OBD_DISP_NODIRFLIP) {
		GmEffect3DESAddDispRotation(efct_work, dir_x, dir_y, dir_z);
	}
	else {
		GmEffect3DESAddDispRotation(efct_work, (Angle16)-dir_z, (Angle16)-dir_y, dir_x);
	}
}


//----- Local Functions -----------------------------------------------------



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
