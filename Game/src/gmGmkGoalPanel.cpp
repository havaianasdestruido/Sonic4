// ==========================================================================
/*!
  @file gmGmkGoalPanel.cpp
  @brief ƒS[ƒ‹ƒpƒlƒ‹

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkGoalPanel.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmGmkGoalPanel.cpp,v $
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "objDraw.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmPlySeqGmk.h"
#include "gmPlySeq.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmSound.h"
#include "hgTrophy.h"
#include "gmPadVib.h"

#include "gmGmkGoalPanel.h"
#include "gmGmkCamScrLim.h"
#include "gmGmkSplRing.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "common/model/gmk_goal_pnl_mdl.hmb"

//mpp -------------------------------
#include "mppCheckPointStorage.h"
#include "mppAchievementSupport.h"


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
#define GMD_GMK_GOAL_EFCT_POS_Y				(30.0f)			//YÀ•W
#define GMD_GMK_GOAL_EFCT_POS_Z				(15.0f)			//ZÀ•W
#define GMD_GMK_GOAL_EFCT_ROT_Z				0//(-0x4000)		//Z‰ñ“]

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkGoalPanelMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkGoalPanelPass(OBS_OBJECT_WORK *obj_work);
static void gmGmkGoalPanelWait(OBS_OBJECT_WORK *obj_work);
static void gmGmkGoalPanelEfctKill(void);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_goal_panel_obj_3d_list = NULL;
static GMS_EFFECT_3DES_WORK *gm_gmk_goal_panel_effct = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkGoalPanelBuild
/*!
 *	ƒMƒ~ƒbƒN ƒS[ƒ‹ƒpƒlƒ‹ ƒf[ƒ^\’z
 */
// ==========================================================================
void GmGmkGoalPanelBuild(void)
{
	gm_gmk_goal_panel_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GOAL_PNL_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GOAL_PNL_TEX),
								0/*draw_flag*/);
}

// ==========================================================================
// GmGmkGoalPanelFlush
/*!
 *	ƒMƒ~ƒbƒN ƒS[ƒ‹ƒpƒlƒ‹ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
void GmGmkGoalPanelFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GOAL_PNL_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_goal_panel_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmGmkGoalPanelInit
/*!
 *	ƒMƒ~ƒbƒN ƒS[ƒ‹ƒpƒlƒ‹ ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkGoalPanelInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_GOAL_PANEL");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// Ä¶¬‚µ‚È‚¢
	gmk_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_DIE;

	// ƒIƒuƒWƒFƒNƒg“Ç‚İ‚İ
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_goal_panel_obj_3d_list[IDB_GMK_GOAL_PNL_MDL_GMK_GOAL_PNL_ZNO],
					&gmk_work->obj_3d);

	// —Dæİ’è
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// ŒÂ•Êİ’è
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// ˆÚ“®–³‚µ ’nŒ`‚ ‚½‚è–³‚µ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->flag |= OBD_OBJECT_NOCLIP;						// ‰æ–ÊŠO‚Å‚à¶‘¶
	obj_work->dir.y = 0x8000;									// ƒGƒbƒOƒ}ƒ“‚ª•\.
	gmk_work->ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NOFREE_DIFF_DATA;	// diff_data‚ğŠJ•ú‚µ‚È‚¢

	// ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmGmkGoalPanelMain;

#ifndef HOG_INLINE3_ROM
	// SPLƒŠƒ“ƒO¶¬
	GmGmkSplRingMake(pos_x + 96 * FX32_ONE, pos_y - 96 * FX32_ONE);
#endif
	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkGoalPanelMain
/*!
 *	ƒMƒ~ƒbƒN ƒS[ƒ‹ƒpƒlƒ‹ ’ÊíƒƒCƒ“ˆ—
 *
 *	@param	gmk_work	[in]	ƒGƒlƒ~[ƒ[ƒN
 *
 *	@note
 */
// ==========================================================================
void gmGmkGoalPanelMain(OBS_OBJECT_WORK *obj_work)
{
//	GMS_ENEMY_3D_WORK	*gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	GMS_PLAYER_WORK		*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
#if 0
	// 3D•\¦A”ñ•\¦(ƒNƒŠƒbƒsƒ“ƒO)
	if ( ((   g_obj.camera[0][MTD_X] -0x040*FX32_ONE) < obj_work->pos.x &&
			 (g_obj.camera[0][MTD_X] +0x140*FX32_ONE) > obj_work->pos.x ) &&
			((g_obj.camera[0][MTD_Y] -0x040*FX32_ONE) < obj_work->pos.y &&
			 (g_obj.camera[0][MTD_Y] +0x100*FX32_ONE) > obj_work->pos.y) ) {
		// •\¦
		obj_work->disp_flag &= ~OBD_DISP_NODISP;
	}
	else {
		// ”ñ•\¦
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
#endif
	if (obj_work->pos.x < ply_work->obj_work.pos.x) {
		// ƒ\ƒjƒbƒN‚ªƒS[ƒ‹ƒ‰ƒCƒ“‚ğ’Ê‰ß
		
		{{//qqq
			OS_TPrintf("try to clear saved state after reaching of the Goal Panel...\n");			
			mpp_checkAchievementsForSuccessLevelEnd(); //only for super sonic mode detect - do not flush achievements here
			mppCheckPointStorage::removeState();
		}}

		// ƒgƒƒtƒB[/ÀÑæ“¾ó‹µƒZƒbƒg(GmPlayerSetGoalState‚æ‚èæ‚Éˆ—)
		if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
			// ƒX[ƒp[ƒ\ƒjƒbƒN‚Ìp‚ÅƒS[ƒ‹
			g_gm_main_system.game_flag |= GMD_GAME_FLAG_GOAL_AS_S_SONIC;
		} else {
			// •’Êƒ\ƒjƒbƒN‚ÅƒS[ƒ‹
			g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_GOAL_AS_S_SONIC;
		}
		HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING_GOAL_IN);

		// ƒvƒŒƒCƒ„[İ’è
		GmPlayerSetGoalState(ply_work);
 
		g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_COUNT_GAME_TIME;	// ƒQ[ƒ€ƒ^ƒCƒ}’â~ 
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_GOAL_IN;			// ƒS[ƒ‹’Ê’m

		obj_work->user_work = 0x1000;			// ‰ñ“]‘¬“x
		obj_work->user_timer = 120;				// ‰ñ“]‘±ŠÔ
		obj_work->ppFunc = gmGmkGoalPanelPass;

		{	// ƒXƒNƒ[ƒ‹§ŒÀƒZƒbƒg
			GMS_EVE_RECORD_EVENT eve_rec;
			eve_rec.flag	= GMD_GMK_SCR_LMT_EVE_FLAG_LEFT | GMD_GMK_SCR_LMT_EVE_FLAG_RIGHT;
//			eve_rec.flag	= GMD_GMK_SCR_LMT_EVE_FLAG_ALL;
			eve_rec.left	= -192/2;
			eve_rec.top		= -208/2;
			eve_rec.width	= 384/2;
			eve_rec.height	= 224/2;
			GmGmkCamScrLimitSet(&eve_rec, obj_work->pos.x, obj_work->pos.y);
		}

		// ƒGƒtƒFƒNƒg¶¬
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
		gm_gmk_goal_panel_effct =
		GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_GOAL);									// ƒGƒtƒFƒNƒg”­¶
		GmEffect3DESSetDispOffset(gm_gmk_goal_panel_effct, 
									0, 										// x
									GMD_GMK_GOAL_EFCT_POS_Y, 				// y
									GMD_GMK_GOAL_EFCT_POS_Z);				// z À•WƒIƒtƒZƒbƒg
		GmEffect3DESSetDispRotation(gm_gmk_goal_panel_effct, 0, 0, GMD_GMK_GOAL_EFCT_ROT_Z);// ‰ñ“]
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */

		// ƒRƒ“ƒgƒ[ƒ‰[U“®
		GMM_PAD_VIB_SMALL();

		// SE
		GmSoundPlaySE("GoalPanel");
	}
}
// ==========================================================================
// gmGmkGoalPanelPass
/*!
 *	ƒMƒ~ƒbƒN ƒS[ƒ‹ƒpƒlƒ‹ ’Ê‰ß
 *
 *	@param	gmk_work	[in]	ƒGƒlƒ~[ƒ[ƒN
 *
 *	@note
 */
// ==========================================================================
void gmGmkGoalPanelPass(OBS_OBJECT_WORK *obj_work)
{
	--obj_work->user_timer;
	if (obj_work->user_timer <= 0) {
		obj_work->user_timer = 0;
		obj_work->user_work = 0;
		obj_work->dir.y = 0;									// ƒ\ƒjƒbƒN‚ª•\.

		obj_work->user_timer = 60 * 2;
		obj_work->ppFunc = gmGmkGoalPanelWait;

		//ƒGƒtƒFƒNƒgíœ
		gmGmkGoalPanelEfctKill();

		// ƒvƒŒƒCƒ„[’ÊíACTƒS[ƒ‹
		GmPlySeqChangeActGoal(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]);
	}

	obj_work->dir.y += (u16)(obj_work->user_work);
}

// ==========================================================================
// gmGmkGoalPanelWait
/*!
 *	ƒMƒ~ƒbƒN ƒS[ƒ‹ƒpƒlƒ‹ I—¹‘Ò‚¿
 *
 *	@param	gmk_work	[in]	ƒGƒlƒ~[ƒ[ƒN
 *
 *	@note
 */
// ==========================================================================
void gmGmkGoalPanelWait(OBS_OBJECT_WORK *obj_work)
{
	if (--obj_work->user_timer <= 0) {
#if 1
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_CLEAR;		// ƒXƒe[ƒWƒNƒŠƒA
		obj_work->ppFunc = NULL;

		//ƒGƒtƒFƒNƒgíœ
		gmGmkGoalPanelEfctKill();

#else	// ƒfƒoƒbƒO—p˜A‘±ƒGƒtƒFƒNƒg¶¬
		//ƒGƒtƒFƒNƒgíœ
		if (gm_gmk_goal_panel_effct){
			ObjDrawKillAction3DES((OBS_OBJECT_WORK*)gm_gmk_goal_panel_effct);
			gm_gmk_goal_panel_effct = NULL;
			obj_work->user_timer = 60*1;
		} else {
			float xp = 0;
			float yp = GMD_GMK_GOAL_EFCT_POS_Y;
			float zp = GMD_GMK_GOAL_EFCT_POS_Z;
			Angle16 xr = 0;
			Angle16 yr = 0;
			Angle16 zr = GMD_GMK_GOAL_EFCT_ROT_Z;
			gm_gmk_goal_panel_effct =
			GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_GOAL);									// ƒGƒtƒFƒNƒg”­¶
			GmEffect3DESSetDispOffset(gm_gmk_goal_panel_effct, xp, yp, zp);	// À•WƒIƒtƒZƒbƒg
			GmEffect3DESSetDispRotation(gm_gmk_goal_panel_effct, xr, yr, zr);// ‰ñ“]
			obj_work->user_timer = 60*2;
		}
#endif
	}
}

// ==========================================================================
// gmGmkGoalPanelEfctKill
/*!
 *	ƒMƒ~ƒbƒN ƒS[ƒ‹ƒpƒlƒ‹ ƒGƒtƒFƒNƒgÁ‹
 *
 *	@note
 */
// ==========================================================================
void gmGmkGoalPanelEfctKill(void)
{
	if (gm_gmk_goal_panel_effct) {
		ObjDrawKillAction3DES((OBS_OBJECT_WORK*)gm_gmk_goal_panel_effct);
		gm_gmk_goal_panel_effct = NULL;
	}
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
//	 *	@param	match_rect	[in]	‘Šè‹éŒ`ƒ[ƒN
//	 *	@param	mine_rect	[in]	©•ª‹éŒ`ƒ[ƒN
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
