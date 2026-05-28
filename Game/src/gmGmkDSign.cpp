// ==========================================================================
/*!
  @file gmGmkDSign.cpp
  @brief 危険告知看板

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmGmkDSign.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"

#include "gmGmkDSign.h"

// データヘッダ
#include "common/model/GMK_DSIGN_MDL.HMB"


//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_dsign_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkDSignBuild
/*!
 *	ギミック 危険告知看板 データ構築
 */
// ==========================================================================
void GmGmkDSignBuild(void)
{
	gm_gmk_dsign_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_DSIGN_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_DSIGN_TEX),
								0/*draw_flag*/);
}

// ==========================================================================
// GmGmkDSignFlush
/*!
 *	ギミック 危険告知看板 データ片付け
 */
// ==========================================================================
void GmGmkDSignFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_DSIGN_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_dsign_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmGmkDSignInit
/*!
 *	ギミック 危険告知看板 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkDSignInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;
	s32					sign_type;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_DSIGN");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_dsign_obj_3d_list[IDB_GMK_DSIGN_MDL_GMK_D_SIGN_ZNO],
					&gmk_work->obj_3d);

	// 優先設定
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_M_FRONT;
	// フラグ
	obj_work->flag		|= OBD_OBJECT_NOHIT;					// あたり無し
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// 方向設定
	sign_type = eve_rec->id - GMD_EVENT_ID_GMK_DANGER_SIGN_D;
	sign_type = MTM_MATH_CLIP(sign_type, 0, 3);
	obj_work->dir.z = (u16)(sign_type * 0x4000);

	return (obj_work);
}


//----- Local Functions -----------------------------------------------------



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
