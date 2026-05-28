// ==========================================================================
/*!
  @file gmGmkDashPanel.cpp
  @brief ƒ_ƒbƒVƒ…ƒpƒlƒ‹

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkDashPanel.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmGmkDashPanel.cpp,v $
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmPlySeqGmk.h"
#include "gmPlySeq.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"
#include "gmSound.h"

#include "gmGmkDashPanel.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "common/model/GMK_DASH_P_MDL.HMB"
#include "common/model/GMK_DASH_P_MTN.HMB"
#include "common/model/GMK_DASH_P_MAT.HMB"


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
// ƒ_ƒbƒVƒ…ƒpƒlƒ‹”»’è‹éŒ`ƒTƒCƒY
#define GMD_GMK_DASH_P_RECT_LF	(-8)
#define GMD_GMK_DASH_P_RECT_UP	(-8)
#define GMD_GMK_DASH_P_RECT_RT	( 8)
#define GMD_GMK_DASH_P_RECT_DW	( 8)
// ƒ_ƒbƒVƒ…ƒpƒlƒ‹”»’è‹éŒ`ƒTƒCƒY ƒgƒƒbƒRƒXƒe[ƒW c—p
#define GMD_GMK_DASH_P_RECT_TV_LF	(-16)
#define GMD_GMK_DASH_P_RECT_TV_RT	( 16)
//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkDashPanelDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_dash_panel_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkDashPanelBuild
/*!
 *	ƒMƒ~ƒbƒN ƒ_ƒbƒVƒ…ƒpƒlƒ‹ ƒf[ƒ^\’z
 */
// ==========================================================================
void GmGmkDashPanelBuild(void)
{
	gm_gmk_dash_panel_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_DASH_P_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_DASH_P_TEX),
								0/*draw_flag*/);
}

// ==========================================================================
// GmGmkDashPanelFlush
/*!
 *	ƒMƒ~ƒbƒN ƒ_ƒbƒVƒ…ƒpƒlƒ‹ ƒf[ƒ^•Ð•t‚¯
 */
// ==========================================================================
void GmGmkDashPanelFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_DASH_P_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_dash_panel_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmGmkDashPanelInit
/*!
 *	ƒMƒ~ƒbƒN ƒ_ƒbƒVƒ…ƒpƒlƒ‹ ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkDashPanelInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_DASH_PANEL");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ƒIƒuƒWƒFƒNƒg“Ç‚Ýž‚Ý
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_dash_panel_obj_3d_list[IDB_GMK_DASH_P_MDL_GMK_DASH_P_ZNO],
					&gmk_work->obj_3d);

	// ƒ‚[ƒVƒ‡ƒ“‰Šú‰»
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, FALSE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_GMK_DASH_P_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);

	// ƒAƒNƒVƒ‡ƒ“Ý’è
	ObjDrawObjectActionSet(obj_work, IDB_GMK_DASH_P_MTN_GMK_DASH_P_ZNM);

	// ƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“
	ObjAction3dNNMaterialMotionLoad( &gmk_work->obj_3d,
	                                 0,				//reg_file_id
	                                 NULL,			//data_work
	                                 NULL,			//mtn_data_path
	                                 IDB_GMK_DASH_P_MAT_GMK_DASH_P_ZNV,
	                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_DASH_P_MAT)->pData );
	ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_DASH_P_MAT_GMK_DASH_P_ZNV );

	// —DæÝ’è
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// ŒÂ•ÊÝ’è
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// ˆÚ“®–³‚µ ’nŒ`‚ ‚½‚è–³‚µ
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// ‹éŒ`Ý’è
	gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppHit = NULL;
	rect_work->ppDef = gmGmkDashPanelDefFunc;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);

	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_2 &&
			(eve_rec->id == GMD_EVENT_ID_GMK_DASH_PANEL_VR ||
				eve_rec->id == GMD_EVENT_ID_GMK_DASH_PANEL_VL)) {
		// ƒgƒƒbƒRƒXƒe[ƒW c—p
		ObjRectWorkSet(rect_work,
							GMD_GMK_DASH_P_RECT_TV_LF, GMD_GMK_DASH_P_RECT_UP,
							GMD_GMK_DASH_P_RECT_TV_RT, GMD_GMK_DASH_P_RECT_DW);
	}
	else {
		ObjRectWorkSet(rect_work,
							GMD_GMK_DASH_P_RECT_LF, GMD_GMK_DASH_P_RECT_UP,
							GMD_GMK_DASH_P_RECT_RT, GMD_GMK_DASH_P_RECT_DW);
	}
	rect_work->flag |= OBD_RECT_OUT;

	// ƒ^ƒCƒv•ÊÝ’è
	if (eve_rec->id == GMD_EVENT_ID_GMK_DASH_PANEL_L) {
		// ¶
		obj_work->dir.y = 0x8000;
	}
	else if (eve_rec->id == GMD_EVENT_ID_GMK_DASH_PANEL_VR) {
		// c‰E•Ç
		obj_work->dir.z = 0xc000;
	}
	else if (eve_rec->id == GMD_EVENT_ID_GMK_DASH_PANEL_VL) {
		// c¶•Ç
		obj_work->dir.z = 0x4000;
		obj_work->dir.y = 0x8000;
	}
	else {
		// ‰E
		obj_work->dir.z = 0;
		obj_work->dir.y = 0;
	}

	// ƒƒCƒ“ˆ—
	obj_work->ppFunc = NULL;

#if OBD_OBJECT_USE_NOEXIST
	obj_work->flag |= OBD_OBJECT_NOEXIST_ENABLE;
#endif // OBD_OBJECT_USE_NOEXIST
	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkDashPanelDefFunc
/*!
 *	ƒMƒ~ƒbƒN ƒ_ƒbƒVƒ…ƒpƒlƒ‹ ‹éŒ` ‚­‚ç‚¢ˆ—
 *
 *	@param mine_rect	[in] Ž©•ª‚­‚ç‚¢‹éŒ`
 *	@param match_rect	[in] ‘ŠŽèUŒ‚‹éŒ`
 *
 *	@note
 *		ppDef‚É“o˜^\n
 */
// ==========================================================================
void gmGmkDashPanelDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_ENEMY_COM_WORK	*com_work = (GMS_ENEMY_COM_WORK*)mine_rect->parent_obj;
	GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;
	u16				eve_id;
//	fx32 spd_x = 0, spd_y = 0;

	if (com_work == NULL) {
		return;
	}
	if (ply_work == NULL || ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER) {
		return;
	}

	eve_id = com_work->eve_rec->id;

	// ƒvƒŒƒCƒ„[‚ªÚ’n‚µ‚Ä‚È‚¯‚ê‚Î‰½‚à‚µ‚È‚¢
	if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER)) {
		// ƒqƒbƒg‚µ‚È‚©‚Á‚½Ž–‚É‚·‚é
		com_work->rect_work[GMD_ENEMY_RECT_BODY].flag &= ~(OBD_RECT_DAMAGE | OBD_RECT_FRAMEHIT | OBD_RECT_FRAMEOUT);
		return;
	}

	// ƒvƒŒƒCƒ„[ƒV[ƒPƒ“ƒXØ‚è‘Ö‚¦
	GmPlySeqInitDashPanel(ply_work,
			(GME_PLYGMK_DASHPANEL_TYPE)(com_work->eve_rec->id - GMD_EVENT_ID_GMK_DASH_PANEL_R));

	// SE
	GmSoundPlaySE("DashPanel");

//	ObjDrawObjectActionSet3DNNMaterial( &com_work->obj_work, IDB_GMK_DASH_P_MAT_GMK_DASH_P_ZNV );
#if 0
	// ‘¬“xƒZƒbƒg
	if (eve_id == GMD_EVENT_ID_GMK_DASH_PANEL_R) {
		// ‰EŒü‚«
		spd_x =  ply_work->spd_max;
	} else if (eve_id == GMD_EVENT_ID_GMK_DASH_PANEL_L) {
		// ¶Œü‚«
		spd_x = -ply_work->spd_max;
	} else if ( eve_id == GMD_EVENT_ID_GMK_DASH_PANEL_VR
			  ||eve_id == GMD_EVENT_ID_GMK_DASH_PANEL_VL ) {
		// ãŒü‚«
		spd_y = -ply_work->spd_max;
	}
	// ƒvƒŒƒCƒ„[‘¬“x‚ðÝ’è
	GmPlayerSpdSet(ply_work, spd_x, spd_y);
#else
#if 0
	// ‘¬“xƒZƒbƒg
	if (eve_id == GMD_EVENT_ID_GMK_DASH_PANEL_R) {
		// ‰EŒü‚«
		ply_work->obj_work.spd_m = ply_work->spd_max;
	} else if (eve_id == GMD_EVENT_ID_GMK_DASH_PANEL_L) {
		// ¶Œü‚«
		ply_work->obj_work.spd_m = -ply_work->spd_max;
	} else if (eve_id == GMD_EVENT_ID_GMK_DASH_PANEL_VR) {
		// ãŒü‚«
		ply_work->obj_work.spd_m = ply_work->spd_max;
	} else if (eve_id == GMD_EVENT_ID_GMK_DASH_PANEL_VL) {
		// ãŒü‚«
		ply_work->obj_work.spd_m = -ply_work->spd_max;
	}
#endif
#endif
}


//	#if 0
//	
//	// ==========================================================================
//	// gmGmkMain
//	/*!
//	 *	ƒMƒ~ƒbƒN  ƒƒCƒ“ˆ—
//	 *
//	 *	@param	gmk_work	[in]	ƒGƒlƒ~[ƒ[ƒN
//	 */
//	// ==========================================================================
//	void gmGmkMain(GMS_ENEMY_WORK *gmk_work)
//	{
//	}
//	
//	
//	// ==========================================================================
//	// gmGmkDefFunc
//	/*!
//	 *	ƒMƒ~ƒbƒN  HITˆ—
//	 *
//	 *	@param	match_rect	[in]	‘ŠŽè‹éŒ`ƒ[ƒN
//	 *	@param	mine_rect	[in]	Ž©•ª‹éŒ`ƒ[ƒN
//	 */
//	// ==========================================================================
//	void gmGmkDefFunc(OBS_RECT_WORK *match_rect, OBS_RECT_WORK *mine_rect)
//	{
//		GMS_ENEMY_WORK	*gmk_work = (GMS_ENEMY_WORK*)mine_rect->parent_obj;
//		GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;
//	
//		if (gmk_work == NULL || ply_work == NULL) {
//			return;
//		}
//		if (ply_work->obj.obj_type != GMD_OBJTYPE_PLAYER) {
//			return;
//		}
//	}
//	#endif
//	// ==========================================================================
//	// _pt
//	/*!
//	 *	@param	tcb	[in]	TCB
//	 */
//	// ==========================================================================
