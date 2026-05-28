// ==========================================================================
/*!
  @file gmGmkSsArrow.cpp
  @brief ÉMÉ~ÉbÉNSpecialStageñÓàÛ

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSsArrow.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"

#include "gmSplStage.h"
#include "gmGmkSsArrow.h"

// ÉfÅ[É^ÉwÉbÉ_
#include "common/model/GMK_SS_ARROW_MDL.HMB"
#include "common/model/GMK_SS_ARROW_MAT.HMB"


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------

// GMS_EVE_RECORD_EVENT : flag
#define GMD_GMK_SS_ARW_EVE_FLAG_VEC_MASK	(0x03)	//!< ñÓàÛï˚å¸É^ÉCÉv

// ÉAÉjÉÅÅ[ÉVÉáÉìsync
#define GMD_GMK_SS_ARW_ANIM_LOOP_TIME		(24)	//!< ÉAÉjÉÅÅ[ÉVÉáÉìÉãÅ[ÉvÉtÉåÅ[ÉÄêî

/// É}ÉeÉäÉAÉãÉÜÅ[ÉUÅ[ÉfÅ[É^ê›íË
enum {
	GMD_GMK_SS_SQR_MAT_USER_DATA_BLUE	= 1,
	GMD_GMK_SS_SQR_MAT_USER_DATA_YELLOW,
	GMD_GMK_SS_SQR_MAT_USER_DATA_PURPLE,
	GMD_GMK_SS_SQR_MAT_USER_DATA_GREEN,

	GMD_GMK_SS_SQR_MAT_USER_DATA_END,

};

/// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNÉÜÅ[ÉUÅ[ÉpÉâÉÅÅ[É^
typedef struct tag_GMS_GMK_SS_SQR_MAT_CB_PARAM {
	u32		draw_id;		//!< ï`âÊÇ∑ÇÈID
} GMS_GMK_SS_SQR_MAT_CB_PARAM;

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkSsArrowMain(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_ss_arrow_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkSsArrowBuild
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ñÓàÛ ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkSsArrowBuild(void)
{
	gm_gmk_ss_arrow_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_ARROW_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_ARROW_TEX),
								0/*draw_flag*/);
}

// ==========================================================================
// GmGmkSsArrowFlush
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ñÓàÛ ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkSsArrowFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_ARROW_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_ss_arrow_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmGmkSsArrowInit
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ñÓàÛ èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSsArrowInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_SS_ARROW");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ê∂ë∂É^ÉXÉNêîÇ…ó]óTÇ™ñ≥Ç¢ÇÃÇ≈éÄñSîÕàÕÇêÿÇËãlÇﬂÇÈ
	obj_work->view_out_ofst = (s16)(obj_work->view_out_ofst - 128);
	
	// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_ss_arrow_obj_3d_list[IDB_GMK_SS_ARROW_MDL_SS_ARROW_ZNO],
					&gmk_work->obj_3d);

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ínå`Ç†ÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;	// ÉâÉCÉgÇOñ≥å¯
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_1;	// ÉâÉCÉgÇPóLå¯

	// êFÉpÉâÉÅÅ[É^/ÉAÉNÉVÉáÉìÉpÉâÉÅÅ[É^Çuser_*Ç…ÉZÉbÉg
//	obj_work->user_flag =
//	obj_work->user_work = (eve_rec->flag & GMD_GMK_SS_SQR_EVE_FLAG_COL_MASK)
//									 + GMD_GMK_SS_SQR_MAT_USER_DATA_BLUE;	// êFÉ^ÉCÉv
//
	// ÉÅÉCÉìèàóù
	obj_work->ppFunc = gmGmkSsArrowMain;
//
//	// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNê›íË
//	gmk_work->obj_3d.material_cb_func = gmGmkSsSquareMaterialCallback;
//
	{
		// É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
		ObjAction3dNNMaterialMotionLoad( &gmk_work->obj_3d,
		                                 0,				//reg_file_id
		                                 NULL,			//data_work
		                                 NULL,			//mtn_data_path
		                                 IDB_GMK_SS_ARROW_MAT_SS_ARROW_ZNV,
		                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_SS_ARROW_MAT)->pData );
		ObjDrawObjectActionSet3DNNMaterial( obj_work, 0 );

//		obj_work->disp_flag |= OBD_DISP_REPEAT;
	}

	// ï˚å¸ÉZÉbÉg
//	obj_work->dir.z = (u16)((eve_rec->flag & GMD_GMK_SS_ARW_EVE_FLAG_VEC_MASK) << 14);
	obj_work->dir.z = (u16)(eve_rec->width << 8);
	
	// ÉAÉNÉVÉáÉìÉ^ÉCÉ~ÉìÉOà ëä
	s32	base_time, adj_time;
	base_time = (s32)(g_gm_main_system.sync_time % GMD_GMK_SS_ARW_ANIM_LOOP_TIME);
	adj_time  = (s32)(MTM_MATH_CLIP(eve_rec->left, 0, GMD_GMK_SS_ARW_ANIM_LOOP_TIME / 8) << 3);
	base_time -= adj_time;
	if (base_time < 0) {
		base_time += GMD_GMK_SS_ARW_ANIM_LOOP_TIME;
	}
	obj_work->user_timer = base_time;
	gmk_work->obj_3d.mat_frame = (float)(base_time);

	// ï`âÊèàóùïœçX
//	obj_work->ppOut = gmGmkSsSquareDrawFunc;

	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkSsArrowMain
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ñÓàÛ ÉÅÉCÉì
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsArrowMain(OBS_OBJECT_WORK *obj_work)
{
	if (GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// ÉXÉeÅ[ÉWÉNÉäÉAÇ≈é©éE(É^ÉXÉNêîÉIÅ[ÉoÅ[ëŒçÙ)
		if (GmSplStageGetWork()->flag & GMD_SPL_STAGE_RESULT) {				// ÉXÉeÅ[ÉWÉNÉäÉA
			obj_work->flag |= OBD_OBJECT_TASKCLEAR;
			return;
		}
	}

	obj_work->user_timer++;
	// ÉAÉjÉÅÅ[ÉVÉáÉìÉãÅ[Évêßå‰
	if (obj_work->user_timer >= GMD_GMK_SS_ARW_ANIM_LOOP_TIME) {
		obj_work->user_timer = 0;
		ObjDrawObjectActionSet3DNNMaterial( obj_work, 0 );
	}
}


//	// ==========================================================================
//	// _pt
//	/*!
//	 *	@param	tcb	[in]	TCB
//	 */
//	// ==========================================================================
