// ==========================================================================
/*!
  @file gmGmkSsEndurance.cpp
  @brief ÉMÉ~ÉbÉNSpecialStageëœãvíå

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSsEndurance.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmGmkSsEndurance.cpp,v $
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"
#include "gmSound.h"
#include "gmEffect.h"
#include "gmEffectZone.h"

#if _IPHONE
#include "gmTvx.h"
#endif // _IPHONE

#include "gmSplStage.h"
#include "gmGmkSsSquare.h"
#include "gmGmkSsEndurance.h"

// ÉfÅ[É^ÉwÉbÉ_
#include "common/model/GMK_SS_ENDURANCE_MDL.HMB"
#include "common/model/GMK_SS_ENDURANCE_MAT.HMB"

#if _IPHONE
// commonÇ∆ç\ê¨Ç™ïœÇÌÇ¡ÇƒÇ¢ÇÈèÍçáÇ…égópÇ∑ÇÈíËã`
#include "iphone/model/GMK_SS_ENDURANCE_TVX.HMB"
#endif // _IPHONE

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------

#define GMD_GMK_SS_ENDURANCE_TEST_TVX       (1 & _IPHONE)

#define GMD_GMK_SSEND_FLUSH_TIME	(30)

// GMS_EVE_RECORD_EVENT : flag
#define GMD_GMK_SSEND_EVE_FLAG_LIFE_MASK	(0x0003)	//!< ëœãvíl

#if GMD_GMK_SS_ENDURANCE_TEST_TVX
#define GMD_GMK_SS_ENDURANCE_UV_WAIT_FRAME	( 4)	//!< UVê›íËílï\é¶ÉEÉFÉCÉg
#define GMD_GMK_SS_ENDURANCE_UV_NUM			(30)	//!< UVê›íËílëçêî

#define GMD_GMK_SS_ENDURANCE_UV_WAIT_UPDATE_SHIFT		(   8)	//!< UV_WAITê›íËäJénBitÉVÉtÉgíl
#define GMD_GMK_SS_ENDURANCE_UV_WAIT_UPDATE_MASK		(0x7F)	//!< UV_WAITê›íËäJénÉ}ÉXÉN(0Å`127)
#define GMD_GMK_SS_ENDURANCE_UV_WAIT_GET_SHIFT			(  10)	//!< UV_WAITì¸éËäJénBitÉVÉtÉgíl
#define GMD_GMK_SS_ENDURANCE_UV_WAIT_GET_MASK			(0x1F)	//!< UV_WAITì¸éËäJénÉ}ÉXÉN(0Å`31)
#endif // GMD_GMK_SS_ENDURANCE_TEST_TVX

/// É}ÉeÉäÉAÉãÉÜÅ[ÉUÅ[ÉfÅ[É^ê›íË
enum {
	GMD_GMK_SS_END_MAT_USER_DATA_BLUE	= 1,
	GMD_GMK_SS_END_MAT_USER_DATA_YELLOW,
	GMD_GMK_SS_END_MAT_USER_DATA_PURPLE,
	GMD_GMK_SS_END_MAT_USER_DATA_GREEN,

	GMD_GMK_SS_END_MAT_USER_DATA_END,

};

#if !GMD_GMK_SS_ENDURANCE_TEST_TVX
/// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNÉÜÅ[ÉUÅ[ÉpÉâÉÅÅ[É^
typedef struct tag_GMS_GMK_SS_END_MAT_CB_PARAM {
	u32		draw_id;		//!< ï`âÊÇ∑ÇÈID
} GMS_GMK_SS_END_MAT_CB_PARAM;
#endif // !GMD_GMK_SS_ENDURANCE_TEST_TVX

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkSsEnduranceWait(OBS_OBJECT_WORK *obj_work);
static void gmGmkSsEnduranceDamage(OBS_OBJECT_WORK *obj_work);
static void gmGmkSsEnduranceDrawFunc(OBS_OBJECT_WORK *obj_work);
#if GMD_GMK_SS_ENDURANCE_TEST_TVX
static void gmGmkSsEnduranceUpdateUVTimer(OBS_OBJECT_WORK *obj_work);
#else
static NNE_BOOL gmGmkSsEnduranceMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param);
#endif // !GMD_GMK_SS_ENDURANCE_TEST_TVX
static u32 gmGmkSsEnduranceColorSet(u32 color_num);
static void gmGmkSsEnduranceScaleSet(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_ss_endurance_obj_3d_list = NULL;

static const u16 gm_gmk_ss_endurance_color[GMD_GMK_SS_END_MAT_USER_DATA_END] = {
	NULL,									// ëœãvíl0
	GMD_GMK_SS_END_MAT_USER_DATA_PURPLE,	// ëœãvíl1
	GMD_GMK_SS_END_MAT_USER_DATA_YELLOW,	// ëœãvíl2
	GMD_GMK_SS_END_MAT_USER_DATA_GREEN,		// ëœãvíl3
	GMD_GMK_SS_END_MAT_USER_DATA_BLUE,		// ëœãvíl4
};

#if GMD_GMK_SS_ENDURANCE_TEST_TVX
static AMS_AMB_HEADER* gm_gmk_ss_endurance_obj_tvx_list = NULL;

/// É}ÉeÉäÉAÉãÉJÉâÅ[(Çí∏ì_Ç…ê›íËÇ∑ÇÈ)
static const NNS_TEXCOORD gm_gmk_ss_endurance_mat_color[GMD_GMK_SS_END_MAT_USER_DATA_END] = {
	{0.00f, 0.00f},		// ñ≥(àÍâûÅAí èÌèÛë‘Çê›íËÇµÇƒÇ®Ç≠)
	{0.50f, 0.00f},		// ê¬
	{0.50f, 0.50f},		// â©
	{0.00f, 0.00f},		// éá
	{0.00f, 0.50f},		// óŒ
};

/// UVÉXÉNÉçÅ[ÉãílÉpÉâÉÅÅ[É^
static const u8 gm_gmk_ss_endurance_uv_parameter[GMD_GMK_SS_ENDURANCE_UV_NUM] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,
};
#endif // GMD_GMK_SS_ENDURANCE_TEST_TVX

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkSsEnduranceBuild
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ëœãvíå ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkSsEnduranceBuild(void)
{
	gm_gmk_ss_endurance_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_ENDURANCE_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_ENDURANCE_TEX),
								0/*draw_flag*/);
#if GMD_GMK_SS_ENDURANCE_TEST_TVX
	void* tvx = GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_SS_ENDURANCE_TVX );
	amBindConv((Uint8*)tvx);
	gm_gmk_ss_endurance_obj_tvx_list = (AMS_AMB_HEADER*)tvx;
#endif // GMD_GMK_SS_ENDURANCE_TEST_TVX
}

// ==========================================================================
// GmGmkSsEnduranceFlush
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ëœãvíå ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkSsEnduranceFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_ENDURANCE_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_ss_endurance_obj_3d_list, amb->file_num);
#if GMD_GMK_SS_ENDURANCE_TEST_TVX
	gm_gmk_ss_endurance_obj_tvx_list = NULL;
#endif // GMD_GMK_SS_ENDURANCE_TEST_TVX
}

// ==========================================================================
// GmGmkSsEnduranceInit
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ëœãvíå èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSsEnduranceInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_SS_ENDURANCE");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ê∂ë∂É^ÉXÉNêîÇ…ó]óTÇ™ñ≥Ç¢ÇÃÇ≈éÄñSîÕàÕÇêÿÇËãlÇﬂÇÈ
	obj_work->view_out_ofst = (s16)(obj_work->view_out_ofst - 128);
	
	// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_ss_endurance_obj_3d_list[IDB_GMK_SS_ENDURANCE_MDL_SS_LASTING_ZNO],
					&gmk_work->obj_3d);

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;				// à⁄ìÆñ≥Çµ ínå`Ç†ÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
//	obj_work->flag |= OBD_OBJECT_NOCLIP;									// âÊñ äOÇ≈Ç‡ê∂ë∂
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;	// ÉâÉCÉgÇOñ≥å¯
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_1;	// ÉâÉCÉgÇPóLå¯

	obj_work->user_flag = (u32)(((~eve_rec->flag) & GMD_GMK_SSEND_EVE_FLAG_LIFE_MASK)
											+ 1 - eve_rec->byte_param[1]);	// ëœãvíl(1Å`4)
	MTM_ASSERT(obj_work->user_flag > 0);
	obj_work->user_work = gmGmkSsEnduranceColorSet(obj_work->user_flag);
	gmGmkSsEnduranceScaleSet(obj_work);

	// ÉÅÉCÉìèàóù
	obj_work->ppFunc = gmGmkSsEnduranceWait;

#if GMD_GMK_SS_ENDURANCE_TEST_TVX
	obj_work->user_timer = 0;
#else
	// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNê›íË
	gmk_work->obj_3d.material_cb_func = gmGmkSsEnduranceMaterialCallback;

	// É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
	{
		ObjAction3dNNMaterialMotionLoad( &gmk_work->obj_3d,
		                                 0,				//reg_file_id
		                                 NULL,			//data_work
		                                 NULL,			//mtn_data_path
		                                 IDB_GMK_SS_ENDURANCE_MAT_SS_LASTING_ZNV,
		                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_SS_ENDURANCE_MAT)->pData );
		ObjDrawObjectActionSet3DNNMaterial( obj_work, 0 );

		obj_work->disp_flag |= OBD_DISP_REPEAT;
	}
#endif // !GMD_GMK_SS_ENDURANCE_TEST_TVX

	// ï`âÊèàóùïœçX
	obj_work->ppOut = gmGmkSsEnduranceDrawFunc;

	// ínå`ê›íË
	{
		OBS_COLLISION_WORK *col_work = &gmk_work->ene_com.col_work;
	
		col_work->obj_col.obj		= obj_work;
		col_work->obj_col.diff_data	= (s8*)g_gm_default_col;			// è„â∫ç∂âEê⁄êGñ äpìxèÓïÒÇìæÇÈÇΩÇﬂÇ…ÇÕ
//		col_work->obj_col.dir_data	= (u8*)g_gm_ss_parts_dir;			// diff_data/dir_dataÇégópÇ∑ÇÈïKóvÇ†ÇË
		col_work->obj_col.width		= 3*8;								// ínå`ÉTÉCÉYê›íË(8dotíPà êßå¿Ç†ÇË)
		col_work->obj_col.height	= 3*8;								// êßñÒèúäOÇ∑ÇÈÇΩÇﬂÇ…ÇÕdiff_dataÇêÍópÇ≈éùÇ¬ïKóvÇ™Ç†ÇÈ
		col_work->obj_col.ofst_x	= (s16)(0 - col_work->obj_col.width / 2);
		col_work->obj_col.ofst_y	= (s16)(0 - col_work->obj_col.height / 2);
		col_work->obj_col.attr		= OBD_COL_DATA_ATTR_CLIFF;			// äRàµÇ¢
		col_work->obj_col.flag		|= OBD_COLOBJ_NOFREE_DIFF_DATA		// diff_dataÇäJï˙ÇµÇ»Ç¢
//										| OBD_COLOBJ_NOFREE_DIR_DATA	// dir_dataÇäJï˙ÇµÇ»Ç¢
										| OBD_COLOBJ_NODIR_PARENT;		// êeÇÃäpìxñ≥éã
	}

	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkSsEnduranceWait
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ëœãvíå í èÌë“ã@
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsEnduranceWait(OBS_OBJECT_WORK *obj_work)
{
	OBS_COLLISION_OBJ	*obj_col = &obj_work->col_work->obj_col;
	GMS_PLAYER_WORK		*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

//	OBS_CAMERA	*camera = ObjCameraGet(g_obj.glb_camera_id);
//	obj_work->dir.z  = (u16)-camera->roll;

	// ÉXÉeÅ[ÉWÉNÉäÉAÇ≈é©éE(É^ÉXÉNêîÉIÅ[ÉoÅ[ëŒçÙ)
	if (GmSplStageGetWork()->flag & GMD_SPL_STAGE_RESULT) {				// ÉXÉeÅ[ÉWÉNÉäÉA
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		return;
	}

	if (obj_col->toucher_obj == &ply_work->obj_work) {
//		if (obj_col->toucher_obj->touch_obj == obj_work) {
			// É\ÉjÉbÉNÇ™ÉqÉbÉg
			obj_work->ppFunc = gmGmkSsEnduranceDamage;
#if GMD_GMK_SS_ENDURANCE_TEST_TVX
			obj_work->user_timer |= GMD_GMK_SSEND_FLUSH_TIME;				// É_ÉÅÅ[ÉWåpë±éûä‘
#else
			obj_work->user_timer = GMD_GMK_SSEND_FLUSH_TIME;				// É_ÉÅÅ[ÉWåpë±éûä‘
#endif // GMD_GMK_SS_ENDURANCE_TEST_TVX

			// SE
			GmSoundPlaySE("Special3");
//		}
	}

#if GMD_GMK_SS_ENDURANCE_TEST_TVX
	gmGmkSsEnduranceUpdateUVTimer(obj_work);
#endif // GMD_GMK_SS_ENDURANCE_TEST_TVX

	// É\ÉjÉbÉNíµÇÀï‘ÇµÉeÉXÉg
	GmGmkSsSquareBounce(obj_work);
}

// ==========================================================================
// gmGmkSsEnduranceDamage
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ëœãvíå É_ÉÅÅ[ÉW
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsEnduranceDamage(OBS_OBJECT_WORK *obj_work)
{
	// ÉXÉeÅ[ÉWÉNÉäÉAÇ≈é©éE(É^ÉXÉNêîÉIÅ[ÉoÅ[ëŒçÙ)
	if (GmSplStageGetWork()->flag & GMD_SPL_STAGE_RESULT) {				// ÉXÉeÅ[ÉWÉNÉäÉA
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		return;
	}

#if GMD_GMK_SS_ENDURANCE_TEST_TVX
	u32 timer = (u32)(obj_work->user_timer & 0xff);
	timer--;
	obj_work->user_timer = (obj_work->user_timer & 0xffffff00) | (timer & 0xff);
	if (!timer) {
#else
	obj_work->user_timer--;
	if (!obj_work->user_timer) {
#endif // GMD_GMK_SS_ENDURANCE_TEST_TVX
		// É_ÉÅÅ[ÉWä˙ä‘èIóπ
		obj_work->user_flag--;												// ÉâÉCÉtå∏éZ
		((GMS_ENEMY_3D_WORK*)obj_work)->ene_com.eve_rec->byte_param[1]++;	// ãLâØÉ_ÉÅÅ[ÉWÇâ¡éZ
		if (obj_work->user_flag & ~GMD_GMK_SS_SQR_FLAG_HIT) {
			// ë“ã@èÛë‘Ç÷à⁄çs
			gmGmkSsEnduranceScaleSet(obj_work);
			obj_work->user_work = gmGmkSsEnduranceColorSet(obj_work->user_flag & ~GMD_GMK_SS_SQR_FLAG_HIT);
			obj_work->ppFunc = gmGmkSsEnduranceWait;
		} else {
			// è¡ñ≈
			obj_work->flag |= OBD_OBJECT_TASKCLEAR;

			// çƒê∂ê¨ÇµÇ»Ç¢
			((GMS_ENEMY_3D_WORK*)obj_work)->ene_com.enemy_flag |= GMD_ENEMY_FLAG_DIE;

#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
			GMS_EFFECT_3DES_WORK* effect_work = 
				GmEfctZoneEsCreate(obj_work,
								GSD_MAIN_ZONE_TYPE_SS,
								GME_EFCT_ZSS_IDX_LOST);						// ÉGÉtÉFÉNÉgî≠ê∂
			effect_work->efct_com.obj_work.flag |= OBD_OBJECT_PARENT_NODIE;	// êeÇ™éÄÇÒÇ≈Ç‡ì∆ÇËóßÇø
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
		}
	} else {
		// É_ÉÅÅ[ÉWåpë±(êFä∑Ç¶)
		u32	color_num;
		color_num = (u32)(((obj_work->user_timer & 0x0c) >> 2) + 1);
		obj_work->user_work = gmGmkSsEnduranceColorSet(color_num);
	}

#if GMD_GMK_SS_ENDURANCE_TEST_TVX
	gmGmkSsEnduranceUpdateUVTimer(obj_work);
#endif // GMD_GMK_SS_ENDURANCE_TEST_TVX

	// É\ÉjÉbÉNíµÇÀï‘ÇµÉeÉXÉg
	GmGmkSsSquareBounce(obj_work);
}

// ==========================================================================
// gmGmkSsEnduranceDrawFunc
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ëœãvíå ï`âÊê›íËèàóù
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsEnduranceDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	OBS_ACTION3D_NN_WORK		*obj_3d;
	obj_3d		= obj_work->obj_3d;

#if GMD_GMK_SS_ENDURANCE_TEST_TVX
	// ï`âÊÇµÇ»Ç¢ÉtÉåÅ[ÉÄÇÕèIóπ
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	// ï`âÊÇµÇ»Ç¢Ç»ÇÁèIóπ
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}
	
	void* tvx            = amBindGet(gm_gmk_ss_endurance_obj_tvx_list, IDB_MODEL_SS_LASTING_TVX);
	NNS_TEXLIST* texlist = obj_work->obj_3d->texlist;
	VecFx32* pos         = &obj_work->pos;
	VecFx32* scale       = &obj_work->scale;
	
	// extend
	GMS_TVX_EX_WORK work;
	u32 uv_param = (((u32)obj_work->user_timer) >> GMD_GMK_SS_ENDURANCE_UV_WAIT_GET_SHIFT) & GMD_GMK_SS_ENDURANCE_UV_WAIT_GET_MASK;
	
	work.u_wrap  = NNE_PRIM_TEXWRAP_CLAMP;
	work.v_wrap  = NNE_PRIM_TEXWRAP_CLAMP;
	work.coord.u = 0.125f * (float)(gm_gmk_ss_endurance_uv_parameter[uv_param] % 4) + gm_gmk_ss_endurance_mat_color[obj_work->user_work].u;
	work.coord.v = 0.125f * (float)(gm_gmk_ss_endurance_uv_parameter[uv_param] / 4) + gm_gmk_ss_endurance_mat_color[obj_work->user_work].v;
	work.color   = 0xffffffff;
	
	GmTvxSetModelEx(tvx, texlist, pos, scale, GMD_TVX_DISP_SCALE | GMD_TVX_DISP_LIGHT_DISABLE, 0, &work);
#else
	GMS_GMK_SS_END_MAT_CB_PARAM	*ss_sqr_mat_cb_param;

	// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNê›íË
	ss_sqr_mat_cb_param = (GMS_GMK_SS_END_MAT_CB_PARAM *)amDrawMallocDataBuffer(sizeof(GMS_GMK_SS_END_MAT_CB_PARAM));
	ss_sqr_mat_cb_param->draw_id = obj_work->user_work;
	obj_3d->material_cb_param = ss_sqr_mat_cb_param;

	ObjDrawActionSummary(obj_work);
#endif  //  GMD_GMK_SS_ENDURANCE_TEST_TVX
}

#if GMD_GMK_SS_ENDURANCE_TEST_TVX
// ==========================================================================
// gmGmkSSEnduranceUpdateUVTimer
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ëœãvíå UVÉXÉNÉçÅ[ÉãÉ^ÉCÉ}Å[èàóù
 *
 *	@param	obj_work	[in]	É^ÉCÉ}Å[ÇçXêVÇ∑ÇÈÉIÉuÉWÉFÉNÉg
 *
 *	@note	obj_work->user_timer ÇÃ â∫à 9Å`16bitÇópÇ¢ÇƒèàóùÇçsÇ¢Ç‹Ç∑ÅB
 *			timerÇÃégópï˚ñ@Ç™ïœçXÇ≥ÇÍÇÈèÍçáÇÕÇ±Ç±Ç‡ïœçXÇ™ïKóv
 */
// ==========================================================================
static void gmGmkSsEnduranceUpdateUVTimer(OBS_OBJECT_WORK *obj_work)
{
	u32 timer = ((u32)obj_work->user_timer >> GMD_GMK_SS_ENDURANCE_UV_WAIT_UPDATE_SHIFT) & GMD_GMK_SS_ENDURANCE_UV_WAIT_UPDATE_MASK;
	timer++;
	if (timer >= GMD_GMK_SS_ENDURANCE_UV_WAIT_FRAME * GMD_GMK_SS_ENDURANCE_UV_NUM) {
		timer = 0;
	}
	obj_work->user_timer = (obj_work->user_timer & 0xff) | (timer << GMD_GMK_SS_ENDURANCE_UV_WAIT_UPDATE_SHIFT);
}
#else
// ==========================================================================
// gmGmkSsEnduranceMaterialCallback
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ëœãvíå É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉN(OBF_MATERIAL_CB)
 *
 *	@param	val		[in]	ÉhÉçÅ[ÉRÅ[ÉãÉoÉbÉNïœêî
 *	@param	param	[in]	ÉÜÅ[ÉUÅ[ÉpÉâÉÅÅ[É^
 */
// ==========================================================================
NNE_BOOL gmGmkSsEnduranceMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param)
{
	u32							user_data;
	GMS_GMK_SS_END_MAT_CB_PARAM	*user_param;

	if (param) {
		user_param = (GMS_GMK_SS_END_MAT_CB_PARAM*)param;

		// ÉÜÅ[ÉUÅ[ÉfÅ[É^éÊìæ
		user_data = ObjDraw3DNNGetMaterialUserData(val);

		if (!user_data ||
				user_data == user_param->draw_id) {
			return (nnPutMaterialCore(val));
		}

#if defined (MTD_DEBUG)
		if (user_data >= GMD_GMK_SS_END_MAT_USER_DATA_END) {
			// ÉpÉâÉÅÅ[É^É~ÉX
			MTM_ASSERT(0);
		}
#endif
	}

	return (NNE_FALSE);

	//return (nnPutMaterialCore(draw_cb_val));
}
#endif // !GMD_GMK_SS_ENDURANCE_TEST_TVX

// ==========================================================================
// gmGmkSsEnduranceColorSet
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ëœãvíå ÉJÉâÅ[ÉZÉbÉg
 *
 *	@param	obj_work[in]	ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
u32 gmGmkSsEnduranceColorSet(u32 color_num)
{
	MTM_ASSERT(color_num);
	return (gm_gmk_ss_endurance_color[color_num]);
}

// ==========================================================================
// gmGmkSsEnduranceScaleSet
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ëœãvíå ÉJÉâÅ[ÉZÉbÉg
 *
 *	@param	obj_work[in]	ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsEnduranceScaleSet(OBS_OBJECT_WORK *obj_work)
{
	fx32 scale = FX32_ONE;
	switch(obj_work->user_flag & ~GMD_GMK_SS_SQR_FLAG_HIT) {
		case 1:
			scale = FX32_ONE * 7/10;										// 70%ï\é¶
			break;
		case 2:
			scale = FX32_ONE * 8/10;										// 80%ï\é¶
			break;
		case 3:
			scale = FX32_ONE * 9/10;										// 90%ï\é¶
			break;
		case 4:
		default:
			break;
	}
	obj_work->scale.x =
	obj_work->scale.y =
	obj_work->scale.z = scale;
}

//	// ==========================================================================
//	// _pt
//	/*!
//	 *	@param	tcb	[in]	TCB
//	 */
//	// ==========================================================================
