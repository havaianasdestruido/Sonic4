// ==========================================================================
/*!
  @file gmGmkSsTime.cpp
  @brief ÉMÉ~ÉbÉNSpecialStageéûä‘ÉpÉlÉã

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSsTime.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmGmkSsTime.cpp,v $
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"
#include "gmComEfct.h"
#include "gmSound.h"
#include "gmEffect.h"
#include "gmEffectZone.h"
#include "gmFix.h"
#include "gsEnvironment.h"

#include "gmSplStage.h"
#include "gmGmkSsTime.h"

// ÉfÅ[É^ÉwÉbÉ_
#include "common/model/GMK_SS_TIME_MDL.HMB"


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------

// GMS_EVE_RECORD_EVENT : flag
#define GMD_GMK_SS_TIME_EVE_FLAG_TIME_MASK	(0x03)	//!< éûä‘É^ÉCÉv

/// éûä‘ÉpÉlÉãÉ^ÉCÉv
enum {
	GME_GMK_SS_TIME_5SEC	= 0,
	GME_GMK_SS_TIME_10SEC,
	GME_GMK_SS_TIME_15SEC,

	GME_GMK_SS_TIME_END,

};

#if 0
/// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNÉÜÅ[ÉUÅ[ÉpÉâÉÅÅ[É^
typedef struct tag_GMS_GMK_SS_TIME_MAT_CB_PARAM {
	u32		draw_id;		//!< ï`âÊÇ∑ÇÈID
} GMS_GMK_SS_TIME_MAT_CB_PARAM;

#endif

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkSsTimeMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkSsTimeDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
static void gmGmkSsTimeEfctMain(OBS_OBJECT_WORK *obj_work);
#if 0
static void gmGmkSsTimeDrawFunc(OBS_OBJECT_WORK *obj_work);
static NNE_BOOL gmGmkSsTimeMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param);
#endif
//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_ss_time_obj_3d_list = NULL;

static const u32 gm_gmk_ss_time_add_subtract[] = {
	 5 * 60,		// plus_5SEC
	10 * 60,		// plus_10SEC
	15 * 60,		// plus_15SEC
};

static const Sint32 gm_gmk_ss_time_add_msg[GSD_LANGUAGE_NUM] = {
	GME_EFCT_ZSS_IDX_TIME_JP,	// ì˙ñ{åÍ
	GME_EFCT_ZSS_IDX_TIME_EN,	// âpåÍ 
	GME_EFCT_ZSS_IDX_TIME_FR,	// ÉtÉâÉìÉXåÍ 
	GME_EFCT_ZSS_IDX_TIME_IT,	// ÉCÉ^ÉäÉAåÍ 
	GME_EFCT_ZSS_IDX_TIME_DE,	// ÉhÉCÉcåÍ 
	GME_EFCT_ZSS_IDX_TIME_ES,	// ÉXÉyÉCÉìåÍ 
};
//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkSsTimeBuild
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éûä‘ÉpÉlÉã ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkSsTimeBuild(void)
{
	gm_gmk_ss_time_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_TIME_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_TIME_TEX),
								0/*draw_flag*/);
}

// ==========================================================================
// GmGmkSsTimeFlush
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éûä‘ÉpÉlÉã ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkSsTimeFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_TIME_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_ss_time_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmGmkSsTimeInit
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éûä‘ÉpÉlÉã èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSsTimeInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;
	u32	time_type;
	
	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_SS_TIME");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ê∂ë∂É^ÉXÉNêîÇ…ó]óTÇ™ñ≥Ç¢ÇÃÇ≈éÄñSîÕàÕÇêÿÇËãlÇﬂÇÈ
	obj_work->view_out_ofst = (s16)(obj_work->view_out_ofst - 128);
	
	time_type = (u32)(eve_rec->flag & GMD_GMK_SS_TIME_EVE_FLAG_TIME_MASK);
	time_type = MTM_MATH_CLIP(time_type, GME_GMK_SS_TIME_5SEC, GME_GMK_SS_TIME_15SEC);
	obj_work->user_work = time_type;

	// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_ss_time_obj_3d_list[IDB_GMK_SS_TIME_MDL_SS_TIME_5S_ZNO + time_type],
					&gmk_work->obj_3d);

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;				// à⁄ìÆñ≥Çµ ínå`Ç†ÇΩÇËñ≥Çµ
	gmk_work->ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NOFREE_DIFF_DATA;	// diff_dataÇäJï˙ÇµÇ»Ç¢
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;	// ÉâÉCÉgÇOñ≥å¯
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_1;	// ÉâÉCÉgÇPóLå¯
	
	obj_work->scale.x =
	obj_work->scale.y =
	obj_work->scale.z = FX32_ONE * 3/2;										// ÉXÉPÅ[Éã1.5î{ÅóÉZÉKóvñ]

	// ÉÅÉCÉìèàóù
	obj_work->ppFunc = gmGmkSsTimeMain;

#if 0
	// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNê›íË
	gmk_work->obj_3d.material_cb_func = gmGmkSsTimeMaterialCallback;

	// ï`âÊèàóùïœçX
	obj_work->ppOut = gmGmkSsTimeDrawFunc;
#endif

	// ãÈå`ê›íË
	{
		OBS_RECT_WORK	*rect_work;
		gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
		rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
		rect_work->ppDef = gmGmkSsTimeDefFunc;
		ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
		ObjRectWorkSet(rect_work, -6, -6, 6, 6);
	}
	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkSsTimeMain
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éûä‘ÉpÉlÉã í èÌë“ã@
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsTimeMain(OBS_OBJECT_WORK *obj_work)
{
#if !_IPHONE
	OBS_CAMERA	*obj_camera = ObjCameraGet(g_obj.glb_camera_id);
#endif // !_IPHONE

	// ÉXÉeÅ[ÉWÉNÉäÉAÇ≈é©éE(É^ÉXÉNêîÉIÅ[ÉoÅ[ëŒçÙ)
	if (GmSplStageGetWork()->flag & GMD_SPL_STAGE_RESULT) {				// ÉXÉeÅ[ÉWÉNÉäÉA
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		return;
	}

#if _IPHONE
	obj_work->dir.z = GmMainGetObjectRotation();
#else
	// âÒì]ÇÉJÉÅÉâÇ∆ìØÇ∂Ç≠
	if (obj_camera) {
		obj_work->dir.z  = (u16)-obj_camera->roll;
	}
#endif // _IPHONE
}

// ==========================================================================
// gmGmkSsTimeDefFunc
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éûä‘ÉpÉlÉã ãÈå` Ç≠ÇÁÇ¢èàóù
 *
 *	@param mine_rect	[in] é©ï™Ç≠ÇÁÇ¢ãÈå`
 *	@param match_rect	[in] ëäéËçUåÇãÈå`
 *
 *	@note
 *		ppDefÇ…ìoò^\n
 */
// ==========================================================================
void gmGmkSsTimeDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_ENEMY_COM_WORK	*com_work = (GMS_ENEMY_COM_WORK*)mine_rect->parent_obj;
	GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	if (com_work == NULL) {
		return;
	}
	if (ply_work == NULL || ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER) {
		return;
	}

	if (ply_work->gmk_obj == (OBS_OBJECT_WORK*)com_work) {
		return;
	}

#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// éÊìæÉGÉtÉFÉNÉg
	GMS_EFFECT_3DES_WORK* effect_work = 
		GmEfctZoneEsCreate(&com_work->obj_work,
						GSD_MAIN_ZONE_TYPE_SS,
						GME_EFCT_ZSS_IDX_TIME_UP);						// ÉGÉtÉFÉNÉgî≠ê∂
	effect_work->efct_com.obj_work.flag |= OBD_OBJECT_PARENT_NODIE;		// êeÇ™éÄÇÒÇ≈Ç‡ì∆ÇËóßÇø
	effect_work->efct_com.obj_work.ppFunc = gmGmkSsTimeEfctMain;

	// TIME_PLUSï\é¶ÉGÉtÉFÉNÉg
	effect_work = 
		GmEfctZoneEsCreate(&com_work->obj_work,
						GSD_MAIN_ZONE_TYPE_SS,
						gm_gmk_ss_time_add_msg[GsEnvGetLanguage()]);	// ÉGÉtÉFÉNÉgî≠ê∂
	effect_work->efct_com.obj_work.flag |= OBD_OBJECT_PARENT_NODIE;		// êeÇ™éÄÇÒÇ≈Ç‡ì∆ÇËóßÇø
	effect_work->efct_com.obj_work.ppFunc = gmGmkSsTimeEfctMain;
	effect_work->obj_3des.command_state =  OBD_DRAW_CMD_STATE_3DFIX;	// ç≈ëOñ Ç…ï\é¶
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */

	// çƒê∂ê¨ÇµÇ»Ç¢
	com_work->enemy_flag |= GMD_ENEMY_FLAG_DIE;

	// SE
	GmSoundPlaySE("Special6");

	// FIX å¯â 
	GmFixRequestTimerFlash();

	// ÉAÉCÉeÉÄå¯â (éûä‘â¡å∏éZ)
	g_gm_main_system.game_time += gm_gmk_ss_time_add_subtract[com_work->obj_work.user_work];

	// è¡ñ≈
	com_work->obj_work.flag |= OBD_OBJECT_TASKCLEAR;

}

// ==========================================================================
// gmGmkSsTimeEfctMain
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éûä‘ÉpÉlÉã è¡ñ≈ÉGÉtÉFÉNÉgÉÅÉCÉì
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsTimeEfctMain(OBS_OBJECT_WORK *obj_work)
{
	OBS_CAMERA	*obj_camera = ObjCameraGet(g_obj.glb_camera_id);

	// âÒì]ÇÉJÉÅÉâÇ∆ìØÇ∂Ç≠
	if (obj_camera) {
		obj_work->dir.z  = (u16)-obj_camera->roll;
	}
}
#if 0
// ==========================================================================
// gmGmkSsTimeDrawFunc
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éûä‘ÉpÉlÉã ï`âÊê›íËèàóù
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsTimeDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	OBS_ACTION3D_NN_WORK		*obj_3d;
	GMS_GMK_SS_TIME_MAT_CB_PARAM	*ss_time_mat_cb_param;

	obj_3d		= obj_work->obj_3d;

	// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNê›íË
	ss_time_mat_cb_param = (GMS_GMK_SS_TIME_MAT_CB_PARAM *)amDrawMallocDataBuffer(sizeof(GMS_GMK_SS_TIME_MAT_CB_PARAM));
	ss_time_mat_cb_param->draw_id = obj_work->user_work;
	obj_3d->material_cb_param = ss_time_mat_cb_param;

	ObjDrawActionSummary(obj_work);
}
// ==========================================================================
// gmGmkSsTimeMaterialCallback
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éûä‘ÉpÉlÉã É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉN(OBF_MATERIAL_CB)
 *
 *	@param	val		[in]	ÉhÉçÅ[ÉRÅ[ÉãÉoÉbÉNïœêî
 *	@param	param	[in]	ÉÜÅ[ÉUÅ[ÉpÉâÉÅÅ[É^
 */
// ==========================================================================
NNE_BOOL gmGmkSsTimeMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param)
{
	u32							user_data;
	GMS_GMK_SS_TIME_MAT_CB_PARAM	*user_param;

	if (param) {
		user_param = (GMS_GMK_SS_TIME_MAT_CB_PARAM*)param;

		// ÉÜÅ[ÉUÅ[ÉfÅ[É^éÊìæ
		user_data = ObjDraw3DNNGetMaterialUserData(val);

		if (!user_data ||
				user_data == user_param->draw_id) {
			return (nnPutMaterialCore(val));
		}

#if defined (MTD_DEBUG)
		if (user_data >= GMD_GMK_SS_TIME_MAT_USER_DATA_END) {
			// ÉpÉâÉÅÅ[É^É~ÉX
			MTM_ASSERT(0);
		}
#endif
	}

	return (NNE_FALSE);

	//return (nnPutMaterialCore(draw_cb_val));
}
#endif

//	// ==========================================================================
//	// _pt
//	/*!
//	 *	@param	tcb	[in]	TCB
//	 */
//	// ==========================================================================
