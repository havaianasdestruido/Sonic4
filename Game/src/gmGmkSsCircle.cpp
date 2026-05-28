// ==========================================================================
/*!
  @file gmGmkSsCircle.cpp
  @brief ƒMƒ~ƒbƒNSpecialStageŠÛ’Œ

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSsCircle.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmGmkSsCircle.cpp,v $
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"

#if _IPHONE
#include "gmTvx.h"
#endif // _IPHONE

#include "gmSplStage.h"
#include "gmGmkSsSquare.h"
#include "gmGmkSsCircle.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "common/model/GMK_SS_CIRCLE_MDL.HMB"

#if _IPHONE
// common‚Æ\¬‚ª•Ï‚í‚Á‚Ä‚¢‚éê‡‚ÉŽg—p‚·‚é’è‹`
#include "iphone/model/GMK_SS_CIRCLE_TVX.HMB"
#endif // _IPHONE


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
#define GMD_GMK_SS_CIRCLE_TEST_TVX       (1 & _IPHONE)

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkSsOnewayMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkSsCircleMain(OBS_OBJECT_WORK *obj_work);
#if GMD_GMK_SS_CIRCLE_TEST_TVX
static void gmGmkSsCircleDrawFunc(OBS_OBJECT_WORK *obj_work);
#endif // GMD_GMK_SS_CIRCLE_TEST_TVX

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_ss_circle_obj_3d_list = NULL;

#if GMD_GMK_SS_CIRCLE_TEST_TVX
static AMS_AMB_HEADER* gm_gmk_ss_circle_obj_tvx_list = NULL;
#endif // GMD_GMK_SS_CIRCLE_TEST_TVX

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkSsCircleBuild
/*!
 *	ƒMƒ~ƒbƒN SpecialStage ŠÛ’Œ ƒf[ƒ^\’z
 */
// ==========================================================================
void GmGmkSsCircleBuild(void)
{
	gm_gmk_ss_circle_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_CIRCLE_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_CIRCLE_TEX),
								0/*draw_flag*/);
#if GMD_GMK_SS_CIRCLE_TEST_TVX
	void* tvx = GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_SS_CIRCLE_TVX );
	amBindConv((Uint8*)tvx);
	gm_gmk_ss_circle_obj_tvx_list = (AMS_AMB_HEADER*)tvx;
#endif // GMD_GMK_SS_CIRCLE_TEST_TVX
}

// ==========================================================================
// GmGmkSsCircleFlush
/*!
 *	ƒMƒ~ƒbƒN SpecialStage ŠÛ’Œ ƒf[ƒ^•Ð•t‚¯
 */
// ==========================================================================
void GmGmkSsCircleFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_CIRCLE_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_ss_circle_obj_3d_list, amb->file_num);
#if GMD_GMK_SS_CIRCLE_TEST_TVX
	gm_gmk_ss_circle_obj_tvx_list = NULL;
#endif // GMD_GMK_SS_CIRCLE_TEST_TVX
}

// ==========================================================================
// GmGmkSsCircleInit
/*!
 *	ƒMƒ~ƒbƒN SpecialStage ŠÛ’Œ ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSsCircleInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_SS_CIRCLE");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ¶‘¶ƒ^ƒXƒN”‚É—]—T‚ª–³‚¢‚Ì‚ÅŽ€–S”ÍˆÍ‚ðØ‚è‹l‚ß‚é
	obj_work->view_out_ofst = (s16)(obj_work->view_out_ofst - 128);
	
	// ƒIƒuƒWƒFƒNƒg“Ç‚Ýž‚Ý
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_ss_circle_obj_3d_list[IDB_GMK_SS_CIRCLE_MDL_SS_CIRCLE_ZNO],
					&gmk_work->obj_3d);

	// —DæÝ’è
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// ŒÂ•ÊÝ’è
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// ˆÚ“®–³‚µ ’nŒ`‚ ‚½‚è–³‚µ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;	// ƒ‰ƒCƒg‚O–³Œø
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_1;	// ƒ‰ƒCƒg‚P—LŒø
	obj_work->user_flag = 0;
		
	// ƒƒCƒ“ˆ—
	if (eve_rec->id == GMD_EVENT_ID_SS_ONEWAY) {
		// ˆê•û’ÊsŠÛ’Œ
		obj_work->ppFunc = gmGmkSsOnewayMain;

		// ƒAƒ‹ƒtƒ@
		obj_work->disp_flag |= OBD_DISP_DRAWSTATE;			// ƒ†[ƒU[•`‰æƒXƒe[ƒg”½‰f
		obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_ALPHA;	// ƒAƒ‹ƒtƒ@§Œä‚ ‚è
		obj_work->obj_3d->draw_state.alpha.alpha = 0.5f;	// ”¼“§–¾
	} else {
		// •’Ê‚ÌŠÛ’Œ
		obj_work->ppFunc = gmGmkSsCircleMain;

		// ’nŒ`Ý’è
		{
			OBS_COLLISION_WORK *col_work = &gmk_work->ene_com.col_work;
		
			col_work->obj_col.obj		= obj_work;
			col_work->obj_col.diff_data	= (s8*)g_gm_default_col;			// ã‰º¶‰EÚG–ÊŠp“xî•ñ‚ð“¾‚é‚½‚ß‚É‚Í
//			col_work->obj_col.dir_data	= (u8*)g_gm_ss_parts_dir;			// diff_data/dir_data‚ðŽg—p‚·‚é•K—v‚ ‚è
			col_work->obj_col.width		= 3*8;								// ’nŒ`ƒTƒCƒYÝ’è(8dot’PˆÊ§ŒÀ‚ ‚è)
			col_work->obj_col.height	= 3*8;								// §–ñœŠO‚·‚é‚½‚ß‚É‚Ídiff_data‚ðê—p‚ÅŽ‚Â•K—v‚ª‚ ‚é
			col_work->obj_col.ofst_x	= (s16)(0 - col_work->obj_col.width / 2);
			col_work->obj_col.ofst_y	= (s16)(0 - col_work->obj_col.height / 2);
			col_work->obj_col.attr		= OBD_COL_DATA_ATTR_CLIFF;			// ŠRˆµ‚¢
			col_work->obj_col.flag		|= OBD_COLOBJ_NOFREE_DIFF_DATA		// diff_data‚ðŠJ•ú‚µ‚È‚¢
//										 | OBD_COLOBJ_NOFREE_DIR_DATA		// dir_data‚ðŠJ•ú‚µ‚È‚¢
										 | OBD_COLOBJ_NODIR_PARENT;			// e‚ÌŠp“x–³Ž‹
		}
	}
#if GMD_GMK_SS_CIRCLE_TEST_TVX
	obj_work->ppOut  = gmGmkSsCircleDrawFunc; // •`‰æ‚ðæ‚ÁŽæ‚é
#endif // GMD_GMK_SS_CIRCLE_TEST_TVX
	return (obj_work);
}


// ==========================================================================
// GmGmkSsOnewayThrough
/*!
 *	ƒMƒ~ƒbƒN SpecialStage ˆê•û’Ês§ŒÀ’Ê‰ß
 */
// ==========================================================================
void GmGmkSsOnewayThrough(u32 sw_no)
{
	GmSplStageSwSet(sw_no);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkSsCircleMain
/*!
 *	ƒMƒ~ƒbƒN SpecialStage ŠÛ’Œ ƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkSsCircleMain(OBS_OBJECT_WORK *obj_work)
{
	// ƒXƒe[ƒWƒNƒŠƒA‚ÅŽ©ŽE(ƒ^ƒXƒN”ƒI[ƒo[‘Îô)
	if (GmSplStageGetWork()->flag & GMD_SPL_STAGE_RESULT) {				// ƒXƒe[ƒWƒNƒŠƒA
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		return;
	}

	if (!(g_gm_main_system.sync_time & 0x0f)) {
		obj_work->dir.z += 0x10000 / 8;		// 45“x
	}

	// ƒ\ƒjƒbƒN’µ‚Ë•Ô‚µƒeƒXƒg
	GmGmkSsSquareBounce(obj_work);
}

// ==========================================================================
// gmGmkSsOnewayMain
/*!
 *	ƒMƒ~ƒbƒN SpecialStage ŠÛ’Œ ƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkSsOnewayMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ƒXƒe[ƒWƒNƒŠƒA‚ÅŽ©ŽE(ƒ^ƒXƒN”ƒI[ƒo[‘Îô)
	if (GmSplStageGetWork()->flag & GMD_SPL_STAGE_RESULT) {				// ƒXƒe[ƒWƒNƒŠƒA
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		return;
	}

	// ’Ê‰ßÏ‚Ý‚È‚ç•’Ê‚ÌŠÛ’Œ‚É•Ï‚í‚è‚Ü‚·B
	if (GmSplStageSwCheck(gmk_work->ene_com.eve_rec->flag)) {
		// •’Ê‚ÌŠÛ’Œ‚É•ÏX
		obj_work->ppFunc = gmGmkSsCircleMain;

		// ’nŒ`Ý’è
		{
			OBS_COLLISION_WORK *col_work = &gmk_work->ene_com.col_work;
		
			col_work->obj_col.obj		= obj_work;
			col_work->obj_col.diff_data	= (s8*)g_gm_default_col;			// ã‰º¶‰EÚG–ÊŠp“xî•ñ‚ð“¾‚é‚½‚ß‚É‚Í
//			col_work->obj_col.dir_data	= (u8*)g_gm_ss_parts_dir;			// diff_data/dir_data‚ðŽg—p‚·‚é•K—v‚ ‚è
			col_work->obj_col.width		= 3*8;								// ’nŒ`ƒTƒCƒYÝ’è(8dot’PˆÊ§ŒÀ‚ ‚è)
			col_work->obj_col.height	= 3*8;								// §–ñœŠO‚·‚é‚½‚ß‚É‚Ídiff_data‚ðê—p‚ÅŽ‚Â•K—v‚ª‚ ‚é
			col_work->obj_col.ofst_x	= (s16)(0 - col_work->obj_col.width / 2);
			col_work->obj_col.ofst_y	= (s16)(0 - col_work->obj_col.height / 2);
			col_work->obj_col.attr		= OBD_COL_DATA_ATTR_CLIFF;			// ŠRˆµ‚¢
			col_work->obj_col.flag		|= OBD_COLOBJ_NOFREE_DIFF_DATA		// diff_data‚ðŠJ•ú‚µ‚È‚¢
//										 | OBD_COLOBJ_NOFREE_DIR_DATA		// dir_data‚ðŠJ•ú‚µ‚È‚¢
										 | OBD_COLOBJ_NODIR_PARENT;			// e‚ÌŠp“x–³Ž‹
		}
		// ƒAƒ‹ƒtƒ@
		obj_work->disp_flag &= ~OBD_DISP_DRAWSTATE;			// ƒ†[ƒU[•`‰æƒXƒe[ƒg”½‰f
		obj_work->obj_3d->drawflag &= ~NND_DRAWOBJ_MATCTRL_ALPHA;	// ƒAƒ‹ƒtƒ@§Œä‚È‚µ
		obj_work->obj_3d->draw_state.alpha.alpha = 1.0f;	// ”¼“§–¾
	}

	// ƒ\ƒjƒbƒN’µ‚Ë•Ô‚µƒeƒXƒg
	GmGmkSsSquareBounce(obj_work);
}

#if GMD_GMK_SS_CIRCLE_TEST_TVX
// ==========================================================================
// gmGmkSsCircleDrawFunc
/*!
 *	ƒMƒ~ƒbƒN SpecialStage ƒS[ƒ‹ •`‰æÝ’èˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkSsCircleDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	if (!GmMainIsDrawEnable()) {
		return;
	}
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}

	void* tvx            = amBindGet(gm_gmk_ss_circle_obj_tvx_list, IDB_GMK_SS_CIRCLE_TVX_SS_CIRCLE_TVX);
	NNS_TEXLIST* texlist = obj_work->obj_3d->texlist;
	VecFx32* pos         = &obj_work->pos;
	VecFx32* scale       = &obj_work->scale;
	u32 disp_flag        = GMD_TVX_DISP_LIGHT_DISABLE | GMD_TVX_DISP_ROTATE;
	
	// extend
	GMS_TVX_EX_WORK work;
	
	work.u_wrap  = NNE_PRIM_TEXWRAP_CLAMP;
	work.v_wrap  = NNE_PRIM_TEXWRAP_CLAMP;
	work.coord.u = 0.0f;
	work.coord.v = 0.0f;
	work.color   = 0xffffffff;
	// ƒuƒŒƒ“ƒhÝ’è‚ ‚è
	if (obj_work->obj_3d->draw_state.alpha.alpha == 0.5f) {
		work.color = 0xffffff88;
		disp_flag |= GMD_TVX_DISP_BLEND;
	}
	
	GmTvxSetModelEx(tvx, texlist, pos, scale, disp_flag, (Angle16)-obj_work->dir.z, &work);
}
#endif // GMD_GMK_SS_CIRCLE_TEST_TVX

//	// ==========================================================================
//	// _pt
//	/*!
//	 *	@param	tcb	[in]	TCB
//	 */
//	// ==========================================================================
