// ==========================================================================
/*!
  @file gmGmkSsEmerald.cpp
  @brief ÉMÉ~ÉbÉNSpecialStageÉJÉIÉXÉGÉÅÉâÉãÉh

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSsEmerald.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmGmkSsEmerald.cpp,v $
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
#include "gmEffectZone.h"
#include "gmSound.h"

#include "gmGmkSsEmerald.h"

// ÉfÅ[É^ÉwÉbÉ_
#include "common/model/GMK_SS_EMERALD_MDL.HMB"
#include "common/model/GMK_SS_EMERALD_MTN.HMB"
#include "common/model/GMK_SS_1UP_MDL.HMB"

//mpp
#include "mppAchievementSupport.h"


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------

// GMS_EVE_RECORD_EVENT : flag
#define GMD_GMK_SS_EME_EVE_FLAG_EME_MASK	(0x07)	//!< ÉGÉÅÉâÉãÉhêFÉ^ÉCÉv


//É}ÉeÉäÉAÉãÉÜÅ[ÉUÅ[ÉfÅ[É^ÇÃégóp/ñ¢égóp
//#define GMD_GMK_SS_EME_USE_MATUSER					// égópÇ∑ÇÈèÍçáíËã`ON

#ifdef GMD_GMK_SS_EME_USE_MATUSER
/// É}ÉeÉäÉAÉãÉÜÅ[ÉUÅ[ÉfÅ[É^ê›íË
enum {
	GMD_GMK_SS_EME_MAT_USER_DATA_COLOR1	= 1,
	GMD_GMK_SS_EME_MAT_USER_DATA_COLOR2,
	GMD_GMK_SS_EME_MAT_USER_DATA_COLOR3,
	GMD_GMK_SS_EME_MAT_USER_DATA_COLOR4,
	GMD_GMK_SS_EME_MAT_USER_DATA_COLOR5,
	GMD_GMK_SS_EME_MAT_USER_DATA_COLOR6,
	GMD_GMK_SS_EME_MAT_USER_DATA_COLOR7,

	GMD_GMK_SS_EME_MAT_USER_DATA_END,

};

/// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNÉÜÅ[ÉUÅ[ÉpÉâÉÅÅ[É^
typedef struct tag_GMS_GMK_SS_EME_MAT_CB_PARAM {
	u32		draw_id;		//!< ï`âÊÇ∑ÇÈID
} GMS_GMK_SS_EME_MAT_CB_PARAM;

#endif//GMD_GMK_SS_EME_USE_MATUSER
//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkSsEmeraldMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkSsEmeraldDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
#ifdef GMD_GMK_SS_EME_USE_MATUSER
static void gmGmkSsEmeraldDrawFunc(OBS_OBJECT_WORK *obj_work);
static NNE_BOOL gmGmkSsEmeraldMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param);
#endif//GMD_GMK_SS_EME_USE_MATUSER
static void gmGmkSsEmeraldEfctKill(void);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_ss_emerald_obj_3d_list = NULL;
static OBS_ACTION3D_NN_WORK *gm_gmk_ss_1up_obj_3d_list = NULL;
static GMS_EFFECT_3DES_WORK *gm_gmk_ss_emerald_effct = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkSsEmeraldBuild
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ÉJÉIÉXÉGÉÅÉâÉãÉh + 1up ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkSsEmeraldBuild(void)
{
	gm_gmk_ss_emerald_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_EMERALD_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_EMERALD_TEX),
								0/*draw_flag*/);
	gm_gmk_ss_1up_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_1UP_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_1UP_TEX),
								0/*draw_flag*/);
}

// ==========================================================================
// GmGmkSsEmeraldFlush
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ÉJÉIÉXÉGÉÅÉâÉãÉh + 1up ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkSsEmeraldFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_EMERALD_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_ss_emerald_obj_3d_list, amb->file_num);

	amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_1UP_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_ss_1up_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmGmkSsEmeraldInit
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ÉJÉIÉXÉGÉÅÉâÉãÉh èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSsEmeraldInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;
	u16					stage_id;
	BOOL				flag_1up = FALSE;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_SS_EMERALD");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ê∂ë∂É^ÉXÉNêîÇ…ó]óTÇ™ñ≥Ç¢ÇÃÇ≈éÄñSîÕàÕÇêÿÇËãlÇﬂÇÈ
	obj_work->view_out_ofst = (s16)(obj_work->view_out_ofst - 128);
	
	stage_id = (u16)(g_gs_main_sys_info.stage_id - GSD_MAIN_STAGE_ID_SS1);		// SplStg 1Å`7
	MTM_ASSERT(stage_id <= GSD_MAIN_STAGE_ID_SS7);

	// ÉXÉyÉVÉÉÉãÉXÉeÅ[ÉWÉfÅ[É^ÉCÉìÉXÉ^ÉìÉXçÏê¨
#if 0	// ÉZÅ[ÉuÉfÅ[É^Ç…ä÷ÇÌÇÁÇ∏ÇPupÇã≠êßï\é¶
	if (1) {
#else
	gs::backup::SSpecial &spe_data = gs::backup::SSpecial::CreateInstance();

	// ÉIÉuÉWÉFÉNÉgèâä˙âª
	if (spe_data[stage_id].IsGetEmerald()) {
#endif
		// 1upÇï\é¶ÅiÉJÉIÉXÉGÉÅÉâÉãÉhälìæçœÇ›Åj
		flag_1up = TRUE;
		// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_gmk_ss_1up_obj_3d_list[IDB_GMK_SS_1UP_MDL_SS_1UP_ZNO],
						&gmk_work->obj_3d);
	} else {
		// ChaosEmeraldÇï\é¶ÅiÉJÉIÉXÉGÉÅÉâÉãÉhñ¢älìæÅj

		// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_gmk_ss_emerald_obj_3d_list[IDB_GMK_SS_EMERALD_MDL_SS_CAOS_RED_ZNO + stage_id],
						&gmk_work->obj_3d);
		// ÉÇÅ[ÉVÉáÉìèâä˙âª
		ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, FALSE/*marge*/,
										ObjDataGet(GMD_DWORK_NO_GMK_SS_EMERALD_MTN), NULL/*mtn_data_path*/,
										0/*index*/, NULL/*archive*/);

		// ÉAÉNÉVÉáÉìê›íË
		ObjDrawObjectActionSet(obj_work, IDB_GMK_SS_EMERALD_MTN_SS_CAOS_RED_ZNM + stage_id);
	}

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ínå`Ç†ÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT;
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;	// ÉâÉCÉgÇOñ≥å¯
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_1;	// ÉâÉCÉgÇPóLå¯

	// ÉÅÉCÉìèàóù
	obj_work->ppFunc = gmGmkSsEmeraldMain;

#ifdef GMD_GMK_SS_EME_USE_MATUSER
	obj_work->user_work = (eve_rec->flag & GMD_GMK_SS_EME_EVE_FLAG_EME_MASK)
									 + GMD_GMK_SS_EME_MAT_USER_DATA_COLOR1;	// êFÉ^ÉCÉv

	// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNê›íË
	gmk_work->obj_3d.material_cb_func = gmGmkSsEmeraldMaterialCallback;

	// ï`âÊèàóùïœçX
	obj_work->ppOut = gmGmkSsEmeraldDrawFunc;
#endif//GMD_GMK_SS_EME_USE_MATUSER

	// ãÈå`ê›íË
	{
		OBS_RECT_WORK	*rect_work;
		gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
		rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
		rect_work->ppDef = gmGmkSsEmeraldDefFunc;
		ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
		ObjRectWorkSet(rect_work, -4, -4, 4, 4);
	}

#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// ÉJÉIÉXÉGÉÅÉâÉãÉh Åï 1up Ç≈ÉGÉtÉFÉNÉgê∂ê¨
	{
		Sint32	efct_no;
		
		if (flag_1up) {
			efct_no = GME_EFCT_ZSS_IDX_1UP;
		} else {
			efct_no = GME_EFCT_ZSS_IDX_CHAOS_E_1 + stage_id;
		}
		gm_gmk_ss_emerald_effct =
			GmEfctZoneEsCreate(obj_work,
								GSD_MAIN_ZONE_TYPE_SS,
								efct_no);									// ÉGÉtÉFÉNÉgî≠ê∂
	}
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkSsEmeraldMain
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ÉJÉIÉXÉGÉÅÉâÉãÉh í èÌë“ã@
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsEmeraldMain(OBS_OBJECT_WORK *obj_work)
{
#if _IPHONE
	obj_work->dir.z = GmMainGetObjectRotation();
#else
	OBS_CAMERA	*obj_camera = ObjCameraGet(g_obj.glb_camera_id);

	// âÒì]ÇÉJÉÅÉâÇ∆ìØÇ∂Ç≠
	if (obj_camera) {
		obj_work->dir.z  = (u16)-obj_camera->roll;
	}
#endif // _IPHONE
}

// ==========================================================================
// gmGmkSsEmeraldDefFunc
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ÉJÉIÉXÉGÉÅÉâÉãÉh ãÈå` Ç≠ÇÁÇ¢èàóù
 *
 *	@param mine_rect	[in] é©ï™Ç≠ÇÁÇ¢ãÈå`
 *	@param match_rect	[in] ëäéËçUåÇãÈå`
 *
 *	@note
 *		ppDefÇ…ìoò^\n
 */
// ==========================================================================
void gmGmkSsEmeraldDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
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

	// ÉGÉÅÉâÉãÉhéÊìæâπÉWÉìÉOÉã
	GmSoundPlayJingle(GME_SOUND_JINGLE_IDX_EMERALD, 0);
	// SE
	GmSoundPlaySE("Special5");
	
	////mppAchievementSupport::get()->event_ChaosEmeraldCollected(1);  //kolya  //sss

	// ÉGÉtÉFÉNÉg
	GmComEfctCreateRing(com_work->obj_work.pos.x, com_work->obj_work.pos.y);
	gmGmkSsEmeraldEfctKill();
	
	// è¡ñ≈
	com_work->obj_work.flag |= OBD_OBJECT_TASKCLEAR;

	g_gm_main_system.game_flag |= GMD_GAME_FLAG_SPL_CHAOSGET;				// èIóπÉtÉâÉOSET
}


#ifdef GMD_GMK_SS_EME_USE_MATUSER
// ==========================================================================
// gmGmkSsEmeraldDrawFunc
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éûä‘ÉpÉlÉã ï`âÊê›íËèàóù
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsEmeraldDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	OBS_ACTION3D_NN_WORK		*obj_3d;
	GMS_GMK_SS_EME_MAT_CB_PARAM	*ss_emerald_mat_cb_param;

	obj_3d		= obj_work->obj_3d;

	// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNê›íË
	ss_emerald_mat_cb_param = (GMS_GMK_SS_EME_MAT_CB_PARAM *)amDrawMallocDataBuffer(sizeof(GMS_GMK_SS_EME_MAT_CB_PARAM));
	ss_emerald_mat_cb_param->draw_id = obj_work->user_work;
	obj_3d->material_cb_param = ss_emerald_mat_cb_param;

	ObjDrawActionSummary(obj_work);
}

// ==========================================================================
// gmGmkSsEmeraldMaterialCallback
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éûä‘ÉpÉlÉã É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉN(OBF_MATERIAL_CB)
 *
 *	@param	val		[in]	ÉhÉçÅ[ÉRÅ[ÉãÉoÉbÉNïœêî
 *	@param	param	[in]	ÉÜÅ[ÉUÅ[ÉpÉâÉÅÅ[É^
 */
// ==========================================================================
NNE_BOOL gmGmkSsEmeraldMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param)
{
	u32							user_data;
	GMS_GMK_SS_EME_MAT_CB_PARAM	*user_param;

	if (param) {
		user_param = (GMS_GMK_SS_EME_MAT_CB_PARAM*)param;

		// ÉÜÅ[ÉUÅ[ÉfÅ[É^éÊìæ
		user_data = ObjDraw3DNNGetMaterialUserData(val);

		if (!user_data ||
				user_data == user_param->draw_id) {
			return (nnPutMaterialCore(val));
		}

#if defined (MTD_DEBUG)
		if (user_data >= GMD_GMK_SS_EME_MAT_USER_DATA_END) {
			// ÉpÉâÉÅÅ[É^É~ÉX
			MTM_ASSERT(0);
		}
#endif
	}

	return (NNE_FALSE);

	//return (nnPutMaterialCore(draw_cb_val));
}
#endif//GMD_GMK_SS_EME_USE_MATUSER

// ==========================================================================
// gmGmkGoalPanelEfctKill
/*!
 *	ÉMÉ~ÉbÉN ÉSÅ[ÉãÉpÉlÉã ÉGÉtÉFÉNÉgè¡ãé
 *
 *	@note
 */
// ==========================================================================
void gmGmkSsEmeraldEfctKill(void)
{
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	if (gm_gmk_ss_emerald_effct) {
		ObjDrawKillAction3DES((OBS_OBJECT_WORK*)gm_gmk_ss_emerald_effct);
		gm_gmk_ss_emerald_effct = NULL;
	}
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
}
//	// ==========================================================================
//	// _pt
//	/*!
//	 *	@param	tcb	[in]	TCB
//	 */
//	// ==========================================================================
