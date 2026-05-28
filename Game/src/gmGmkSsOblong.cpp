// ==========================================================================
/*!
  @file gmGmkSsOblong.cpp
  @brief ÉMÉ~ÉbÉNRingGateèIí[íå

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSsOblong.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmPlySeqGmk.h"

#if _IPHONE
#include "gmTvx.h"
#endif // _IPHONE

#include "gmSplStage.h"
#include "gmGmkSsSquare.h"
#include "gmGmkSsOblong.h"

// ÉfÅ[É^ÉwÉbÉ_
#include "common/model/GMK_SS_OBLONG_MDL.HMB"
#include "common/model/GMK_SS_OBLONG_MAT.HMB"

#if _IPHONE
// commonÇ∆ç\ê¨Ç™ïœÇÌÇ¡ÇƒÇ¢ÇÈèÍçáÇ…égópÇ∑ÇÈíËã`
#include "iphone/model/GMK_SS_OBLONG_TVX.HMB"
#endif // _IPHONE

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------

#define GMD_GMK_SS_OBLONG_TEST_TVX       (1 & _IPHONE)

// GMS_EVE_RECORD_EVENT : flag
#define GMD_GMK_SS_OBL_EVE_FLAG_ROT	(0x01)	//!< ècâÒì]

#if GMD_GMK_SS_OBLONG_TEST_TVX
#define GMD_GMK_SS_OBLONG_UV_WAIT_FRAME		( 3)	//!< UVê›íËílï\é¶ÉEÉFÉCÉg
#define GMD_GMK_SS_OBLONG_UV_NUM			(16)	//!< UVê›íËílëçêî

/// É}ÉeÉäÉAÉãÉÜÅ[ÉUÅ[ÉfÅ[É^ê›íË
enum {
	GMD_GMK_SS_OBL_MAT_USER_DATA_YELLOW	= 0,	//â©
	GMD_GMK_SS_OBL_MAT_USER_DATA_OCHER,			//â©ìy
	GMD_GMK_SS_OBL_MAT_USER_DATA_ORANGE,		//ÉIÉåÉìÉW
	GMD_GMK_SS_OBL_MAT_USER_DATA_CINNABAR,		//éÈ

	GMD_GMK_SS_OBL_MAT_USER_DATA_END,

};
#else
/// É}ÉeÉäÉAÉãÉÜÅ[ÉUÅ[ÉfÅ[É^ê›íË
enum {
	GMD_GMK_SS_OBL_MAT_USER_DATA_YELLOW	= 65,	//â©
	GMD_GMK_SS_OBL_MAT_USER_DATA_OCHER,			//â©ìy
	GMD_GMK_SS_OBL_MAT_USER_DATA_ORANGE,		//ÉIÉåÉìÉW
	GMD_GMK_SS_OBL_MAT_USER_DATA_CINNABAR,		//éÈ

	GMD_GMK_SS_OBL_MAT_USER_DATA_END,

};

/// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNÉÜÅ[ÉUÅ[ÉpÉâÉÅÅ[É^
typedef struct tag_GMS_GMK_SS_OBL_MAT_CB_PARAM {
	u32		draw_id;		//!< ï`âÊÇ∑ÇÈID
} GMS_GMK_SS_OBL_MAT_CB_PARAM;
#endif // GMD_GMK_SS_OBLONG_TEST_TVX

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkSsOblongMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkSsOblongDrawFunc(OBS_OBJECT_WORK *obj_work);
#if !GMD_GMK_SS_OBLONG_TEST_TVX
static NNE_BOOL gmGmkSsOblongMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param);
#endif // !GMD_GMK_SS_OBLONG_TEST_TVX

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_ss_oblong_obj_3d_list = NULL;

#if GMD_GMK_SS_OBLONG_TEST_TVX
static AMS_AMB_HEADER* gm_gmk_ss_oblong_obj_tvx_list = NULL;

/// É}ÉeÉäÉAÉãÉJÉâÅ[(ÇUVÇ…Çƒê›íËÇ∑ÇÈ)
static const NNS_TEXCOORD gm_gmk_ss_oblong_mat_color[GMD_GMK_SS_OBL_MAT_USER_DATA_END] = {
	{0.00f, 0.00f},		// â©
	{0.00f, 0.25f},		// â©ê‘
	{0.00f, 0.50f},		// ÉIÉåÉìÉW
	{0.00f, 0.75f},		// éÈ
};
#else
static const u32 gm_gmk_ss_oblong_mat_tbl[4] = {
	GMD_GMK_SS_OBL_MAT_USER_DATA_CINNABAR,		//éÈ
	GMD_GMK_SS_OBL_MAT_USER_DATA_ORANGE,		//ÉIÉåÉìÉW
	GMD_GMK_SS_OBL_MAT_USER_DATA_OCHER,			//â©ìy
	GMD_GMK_SS_OBL_MAT_USER_DATA_YELLOW,		//â©
};
#endif // GMD_GMK_SS_OBLONG_TEST_TVX
	
//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkSsOblongBuild
/*!
 *	ÉMÉ~ÉbÉN SpecialStage RingGateèIí[íå ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkSsOblongBuild(void)
{
	gm_gmk_ss_oblong_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_OBLONG_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_OBLONG_TEX),
								0/*draw_flag*/);
#if GMD_GMK_SS_OBLONG_TEST_TVX
	void* tvx = GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_SS_OBLONG_TVX );
	amBindConv((Uint8*)tvx);
	gm_gmk_ss_oblong_obj_tvx_list = (AMS_AMB_HEADER*)tvx;
#endif // GMD_GMK_SS_OBLONG_TEST_TVX
}

// ==========================================================================
// GmGmkSsOblongFlush
/*!
 *	ÉMÉ~ÉbÉN SpecialStage RingGateèIí[íå ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkSsOblongFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_OBLONG_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_ss_oblong_obj_3d_list, amb->file_num);
#if GMD_GMK_SS_OBLONG_TEST_TVX
	gm_gmk_ss_oblong_obj_tvx_list = NULL;
#endif // GMD_GMK_SS_OBLONG_TEST_TVX
}

// ==========================================================================
// GmGmkSsOblongInit
/*!
 *	ÉMÉ~ÉbÉN SpecialStage RingGateèIí[íå èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSsOblongInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_SS_OBLONG");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ê∂ë∂É^ÉXÉNêîÇ…ó]óTÇ™ñ≥Ç¢ÇÃÇ≈éÄñSîÕàÕÇêÿÇËãlÇﬂÇÈ
	obj_work->view_out_ofst = (s16)(obj_work->view_out_ofst - 128);
	
	// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_ss_oblong_obj_3d_list[IDB_GMK_SS_OBLONG_MDL_SS_OBLONG_ZNO],
					&gmk_work->obj_3d);

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ínå`Ç†ÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;	// ÉâÉCÉgÇOñ≥å¯
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_1;	// ÉâÉCÉgÇPóLå¯

	// êFÉpÉâÉÅÅ[É^/ÉAÉNÉVÉáÉìÉpÉâÉÅÅ[É^Çuser_*Ç…ÉZÉbÉg
	obj_work->user_flag = 0;
	obj_work->user_work = GMD_GMK_SS_OBL_MAT_USER_DATA_YELLOW;		//â©
	
	// ÉÅÉCÉìèàóù
	obj_work->ppFunc = gmGmkSsOblongMain;

#if GMD_GMK_SS_OBLONG_TEST_TVX
	obj_work->user_timer = 0;
	
#else
	// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNê›íË
	gmk_work->obj_3d.material_cb_func = gmGmkSsOblongMaterialCallback;

	{
		// É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
		ObjAction3dNNMaterialMotionLoad( &gmk_work->obj_3d,
		                                 0,				//reg_file_id
		                                 NULL,			//data_work
		                                 NULL,			//mtn_data_path
		                                 IDB_GMK_SS_OBLONG_MAT_SS_OBLONG_ZNV,
		                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_SS_OBLONG_MAT)->pData );
		ObjDrawObjectActionSet3DNNMaterial( obj_work, 0 );

		obj_work->disp_flag |= OBD_DISP_REPEAT;
	}
#endif // !GMD_GMK_SS_OBLONG_TEST_TVX

	if (eve_rec->flag & GMD_GMK_SS_OBL_EVE_FLAG_ROT) {
		// ècÇÕ90ìxâÒì]
		obj_work->dir.z = 0x4000;
	}

	// ï`âÊèàóùïœçX
	obj_work->ppOut = gmGmkSsOblongDrawFunc;

	// ínå`ê›íË
	{
		OBS_COLLISION_WORK *col_work = &gmk_work->ene_com.col_work;
	
		col_work->obj_col.obj		= obj_work;
		col_work->obj_col.diff_data	= (s8*)g_gm_default_col;			// è„â∫ç∂âEê⁄êGñ äpìxèÓïÒÇìæÇÈÇΩÇﬂÇ…ÇÕ
		if (eve_rec->flag & GMD_GMK_SS_OBL_EVE_FLAG_ROT) {
			// èc
			col_work->obj_col.width		= 3*8;							// ínå`ÉTÉCÉYê›íË(8dotíPà êßå¿Ç†ÇË)
			col_work->obj_col.height	= 6*8;							// êßñÒèúäOÇ∑ÇÈÇΩÇﬂÇ…ÇÕdiff_dataÇêÍópÇ≈éùÇ¬ïKóvÇ™Ç†ÇÈ
		} else {
			// â°
			col_work->obj_col.width		= 6*8;							// ínå`ÉTÉCÉYê›íË(8dotíPà êßå¿Ç†ÇË)
			col_work->obj_col.height	= 3*8;							// êßñÒèúäOÇ∑ÇÈÇΩÇﬂÇ…ÇÕdiff_dataÇêÍópÇ≈éùÇ¬ïKóvÇ™Ç†ÇÈ
		}
		col_work->obj_col.ofst_x	= (s16)(0 - col_work->obj_col.width / 2);
		col_work->obj_col.ofst_y	= (s16)(0 - col_work->obj_col.height / 2);
		col_work->obj_col.attr		= OBD_COL_DATA_ATTR_CLIFF;			// äRàµÇ¢
		col_work->obj_col.flag		|= OBD_COLOBJ_NOFREE_DIFF_DATA		// diff_dataÇäJï˙ÇµÇ»Ç¢
									 | OBD_COLOBJ_NODIR_PARENT;			// êeÇÃäpìxñ≥éã
	}

	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkSsOblongMain
/*!
 *	ÉMÉ~ÉbÉN SpecialStage RingGateèIí[íå ÉÅÉCÉì
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsOblongMain(OBS_OBJECT_WORK *obj_work)
{
	// ÉXÉeÅ[ÉWÉNÉäÉAÇ≈é©éE(É^ÉXÉNêîÉIÅ[ÉoÅ[ëŒçÙ)
	if (GmSplStageGetWork()->flag & GMD_SPL_STAGE_RESULT) {				// ÉXÉeÅ[ÉWÉNÉäÉA
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		return;
	}

#if GMD_GMK_SS_OBLONG_TEST_TVX
	// êFÉAÉjÉÅÅ[ÉVÉáÉìêßå‰
	// 32frameñàÇ…ÇPÉpÉ^Å[ÉìêiÇﬂÇÈ
	obj_work->user_work = (((g_gm_main_system.sync_time + 1) >> 5) & 3);
	
	// ÉeÉNÉXÉ`ÉÉêßå‰
	obj_work->user_timer++;
	if (obj_work->user_timer >= GMD_GMK_SS_OBLONG_UV_WAIT_FRAME * GMD_GMK_SS_OBLONG_UV_NUM) {
		obj_work->user_timer = 0;
	}
#else
	u32 mat_type;
	// êFÉAÉjÉÅÅ[ÉVÉáÉìêßå‰
	// 32frameñàÇ…ÇPÉpÉ^Å[ÉìêiÇﬂÇÈ
	mat_type = (((g_gm_main_system.sync_time + 1) >> 5) & 3);
	obj_work->user_work = gm_gmk_ss_oblong_mat_tbl[mat_type];
#endif // GMD_GMK_SS_OBLONG_TEST_TVX

	// É\ÉjÉbÉNíµÇÀï‘Çµ
	GmGmkSsSquareBounce(obj_work);
}

// ==========================================================================
// gmGmkSsOblongDrawFunc
/*!
 *	ÉMÉ~ÉbÉN SpecialStage RingGateèIí[íå ï`âÊê›íËèàóù
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsOblongDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	OBS_ACTION3D_NN_WORK		*obj_3d;

	obj_3d		= obj_work->obj_3d;

#if GMD_GMK_SS_OBLONG_TEST_TVX
	// ï`âÊÇµÇ»Ç¢ÉtÉåÅ[ÉÄÇÕèIóπ
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}
	
	void* tvx            = amBindGet(gm_gmk_ss_oblong_obj_tvx_list, IDB_MODEL_SS_OBLONG_TVX);
	NNS_TEXLIST* texlist = obj_work->obj_3d->texlist;
	VecFx32* pos         = &obj_work->pos;
	VecFx32* scale       = &obj_work->scale;
	u32 disp_flag        = GMD_TVX_DISP_LIGHT_DISABLE;
	u32 rotate           = 0;
	if (obj_work->dir.z) {
		disp_flag |= GMD_TVX_DISP_ROTATE;
		rotate = obj_work->dir.z;
	}
	
	// extend
	GMS_TVX_EX_WORK work;
	u32 uv_param = (u32)(obj_work->user_timer / GMD_GMK_SS_OBLONG_UV_WAIT_FRAME);
	
	work.u_wrap  = NNE_PRIM_TEXWRAP_CLAMP;
	work.v_wrap  = NNE_PRIM_TEXWRAP_CLAMP;
	work.coord.u = 0.125f * (float)(uv_param % 8) + gm_gmk_ss_oblong_mat_color[obj_work->user_work].u;
	work.coord.v = 0.125f * (float)(uv_param / 8) + gm_gmk_ss_oblong_mat_color[obj_work->user_work].v;
	work.color   = 0xffffffff;
	
	GmTvxSetModelEx(tvx, texlist, pos, scale, disp_flag, rotate, &work);
#else
	GMS_GMK_SS_OBL_MAT_CB_PARAM	*ss_obl_mat_cb_param;
	
	// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNê›íË
	ss_obl_mat_cb_param = (GMS_GMK_SS_OBL_MAT_CB_PARAM *)amDrawMallocDataBuffer(sizeof(GMS_GMK_SS_OBL_MAT_CB_PARAM));
	ss_obl_mat_cb_param->draw_id = obj_work->user_work;
	obj_3d->material_cb_param = ss_obl_mat_cb_param;

	ObjDrawActionSummary(obj_work);
#endif  //  GMD_GMK_SS_OBLONG_TEST_TVX
}

#if !GMD_GMK_SS_OBLONG_TEST_TVX
// ==========================================================================
// gmGmkSsOblongMaterialCallback
/*!
 *	ÉMÉ~ÉbÉN SpecialStage RingGateèIí[íå É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉN(OBF_MATERIAL_CB)
 *
 *	@param	val		[in]	ÉhÉçÅ[ÉRÅ[ÉãÉoÉbÉNïœêî
 *	@param	param	[in]	ÉÜÅ[ÉUÅ[ÉpÉâÉÅÅ[É^
 */
// ==========================================================================
NNE_BOOL gmGmkSsOblongMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param)
{
	u32							user_data;
	GMS_GMK_SS_OBL_MAT_CB_PARAM	*user_param;

	if (param) {
		user_param = (GMS_GMK_SS_OBL_MAT_CB_PARAM*)param;

		// ÉÜÅ[ÉUÅ[ÉfÅ[É^éÊìæ
		user_data = ObjDraw3DNNGetMaterialUserData(val);

		if (!user_data ||
				user_data == user_param->draw_id) {
			return (nnPutMaterialCore(val));
		}

#if defined (MTD_DEBUG)
		if (user_data >= GMD_GMK_SS_OBL_MAT_USER_DATA_END) {
			// ÉpÉâÉÅÅ[É^É~ÉX
			MTM_ASSERT(0);
		}
#endif
	}

	return (NNE_FALSE);

	//return (nnPutMaterialCore(draw_cb_val));
}
#endif // !GMD_GMK_SS_OBLONG_TEST_TVX

//	// ==========================================================================
//	// _pt
//	/*!
//	 *	@param	tcb	[in]	TCB
//	 */
//	// ==========================================================================
