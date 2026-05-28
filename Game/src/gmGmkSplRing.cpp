// ==========================================================================
/*!
  @file gmGmkSplRing.cpp
  @brief ƒXƒyƒVƒƒƒ‹ƒŠƒ“ƒOiƒXƒyƒXƒe“üŒûƒŠƒ“ƒOj

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSplRing.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmGmkSplRing.cpp,v $
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmPlySeqGmk.h"
#include "gmSound.h"
#include "gsTrial.h"
#include "gmPadVib.h"

#include "gmGmkSplRing.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "common/model/GMK_SS_RING_MDL.HMB"

#if _IPHONE
#include "iPhone/model/GMK_SS_RING_MAT.HMB"
#endif // _IPHONE


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
#define GMD_GMK_SPLRING_GET_NUM	(50)	// SPLƒŠƒ“ƒOoŒ»‚É•K—v‚ÈƒŠƒ“ƒO–‡”

// SPLƒŠƒ“ƒO”»’è‹éŒ`ƒTƒCƒY
#define GMD_GMK_SPLRING_RECT_LF	(-4)
#define GMD_GMK_SPLRING_RECT_UP	(-4)
#define GMD_GMK_SPLRING_RECT_RT	( 4)
#define GMD_GMK_SPLRING_RECT_DW	( 4)

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkSplRingWait(OBS_OBJECT_WORK *obj_work);
static void gmGmkSplRingVanishReady(OBS_OBJECT_WORK *obj_work);
static void gmGmkSplRingVanish(OBS_OBJECT_WORK *obj_work);
static void gmGmkSplRingDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

static OBS_ACTION3D_NN_WORK *gm_gmk_splring_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkSplRingBuild
/*!
 *	ƒMƒ~ƒbƒN SPLƒŠƒ“ƒO ƒf[ƒ^\’z
 */
// ==========================================================================
void GmGmkSplRingBuild(void)
{
	gm_gmk_splring_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SPL_RING_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SPL_RING_TEX),
								0/*draw_flag*/);
}

// ==========================================================================
// GmGmkSplRingFlush
/*!
 *	ƒMƒ~ƒbƒN SPLƒŠƒ“ƒO ƒf[ƒ^•Ð•t‚¯
 */
// ==========================================================================
void GmGmkSplRingFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SPL_RING_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_splring_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmGmkSplRingMake
/*!
 *	ƒMƒ~ƒbƒN SPLƒŠƒ“ƒO¶¬ŠÖ”
 *	iƒvƒƒOƒ‰ƒ€‚©‚ç‚Ì¶¬ŒÄ‚Ño‚µ—pj
 *
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *
 *	@note	Šî–{“I‚ÉƒS[ƒ‹ƒpƒlƒ‹‚©‚ç‚ÌŒÄ‚Ño‚µ‚ð‘z’èB
 *			¶¬ˆÊ’u‚ðŽw’è‚µ‚ÄSPLƒŠƒ“ƒO‚ðÝ’u‚µ‚Ü‚·B
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSplRingMake(fx32 pos_x, fx32 pos_y)
{
	OBS_OBJECT_WORK		*obj_work;

	obj_work = GmEventMgrLocalEventBirth(
				GMD_EVENT_ID_NOSET_SPL_RING,		// u16 id, 
				pos_x,								// fx32 pos_x, 
				pos_y,								// fx32 pos_y, 
				0,									// u16 flag, 
				0,									// s8 left, 
				0,									// s8 top, 
				0,									// u8 width, 
				0,									// u8 height, 
				0);									// u8 type);
	return (obj_work);
}
// ==========================================================================
// GmGmkSplRingInit
/*!
 *	ƒMƒ~ƒbƒN SPLƒŠƒ“ƒO ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSplRingInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_SPL_RING");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// Ä¶¬‚µ‚È‚¢
	gmk_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_DIE;

	// ƒIƒuƒWƒFƒNƒg“Ç‚Ýž‚Ý
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_splring_obj_3d_list[IDB_GMK_SS_RING_MDL_SS_RING_ZNO],
					&gmk_work->obj_3d);
#if _IPHONE
	ObjAction3dNNMaterialMotionLoad(&gmk_work->obj_3d,
									0,
									NULL,
									NULL,
									IDB_GMK_SS_RING_MAT_GMK_SS_RING_INV,
									(void*)ObjDataGet(GMD_DWORK_NO_GMK_SPL_RING_MAT)->pData);
	ObjDrawAction3dActionSet3DNNMaterial(&gmk_work->obj_3d, IDB_GMK_SS_RING_MAT_GMK_SS_RING_INV);
#endif // _IPHONE
	// —DæÝ’è
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// ŒÂ•ÊÝ’è
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// ˆÚ“®–³‚µ ’nŒ`‚ ‚½‚è–³‚µ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_NODISP;// Å‰‚Í”ñ•\Ž¦
	obj_work->flag |= OBD_OBJECT_NOCLIP | OBD_OBJECT_NOHIT;		// ‰æ–ÊŠO‚Å‚à¶‘¶ Å‰‚Íƒm[ƒqƒbƒg
	gmk_work->ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NOFREE_DIFF_DATA;	// diff_data‚ðŠJ•ú‚µ‚È‚¢
	
	// Body‹éŒ`ƒZƒbƒg
	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppDef = gmGmkSplRingDefFunc;
	ObjRectWorkSet(rect_work,
						GMD_GMK_SPLRING_RECT_LF, GMD_GMK_SPLRING_RECT_UP,
						GMD_GMK_SPLRING_RECT_RT, GMD_GMK_SPLRING_RECT_DW);

	// ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmGmkSplRingWait;

	return (obj_work);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkSplRingWait
/*!
 *	ƒMƒ~ƒbƒN SPLƒŠƒ“ƒO ƒƒCƒ“ŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@note
 *		ƒvƒŒƒCƒ„[‚ªÚG‚·‚é‚Ü‚Å‚Ì‘Ò‹@ó‘Ô‚Å‚·
  */
// ==========================================================================
void gmGmkSplRingWait(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK		*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	if (!ply_work) {
		// ƒvƒŒƒCƒ„[–¢¶¬‚È‚çˆ—‚µ‚È‚¢
		return;
	}
	
	if (g_gs_main_sys_info.game_mode != GSD_GAME_MODE_STORY) {
		// ƒXƒg[ƒŠ[ƒ‚[ƒhŽžˆÈŠO‚È‚çˆ—‚µ‚È‚¢
		return;
	}
	if (GsTrialIsTrial()) {
		// ‘ÌŒ±”Å‚Å‚Í‹@”\‚µ‚È‚¢
		return;
		
	}
	if (ply_work->obj_work.pos.x < (obj_work->pos.x - FXM_FLOAT_TO_FX32(AMD_SCREEN_2D_WIDTH))) {
		// ‹ß‚Ã‚©‚È‚¢‚Æˆ—‚µ‚È‚¢
		return;
	}
	
	if (GsMainSysIsSpecialStageClearedAct(g_gs_main_sys_info.stage_id)) {
		// ƒvƒŒƒC‚µ‚½ACT‚ªŠù‚ÉƒXƒyƒXƒeƒNƒŠƒAÏ‚Ý‚È‚ç‚Î‹@”\‚µ‚È‚¢
		return;
	}
	
	// ƒXƒyƒXƒe‘S‚Ä(ƒXƒyƒXƒe‚V)‚ðƒNƒŠƒA‚µ‚Ä‚¢‚½‚ç‹@”\‚µ‚È‚¢
	if (GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_SS7)) {
		return;
	}
	
	
	if (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->ring_num < GMD_GMK_SPLRING_GET_NUM) {
		// ƒŠƒ“ƒO”‚ª•K—v–‡”‚É’B‚µ‚È‚¯‚ê‚Î‹@”\OFF
		obj_work->disp_flag |= OBD_DISP_NODISP;					// •\Ž¦‚n‚e‚e
		obj_work->flag |= OBD_OBJECT_NOHIT;						// ƒqƒbƒg‚n‚e‚e
	} else {
		// ƒŠƒ“ƒO”‚ª•K—v–‡”‚É’B‚µ‚½‚ç‹@”\ON
		obj_work->disp_flag &= ~OBD_DISP_NODISP;				// •\Ž¦‚n‚m
		obj_work->flag &= ~OBD_OBJECT_NOHIT;					// ƒqƒbƒg‚n‚m
	}
	
#if !_IPHONE
	obj_work->dir.y -= 0x0300;						// YŽ²‰ñ“]
#else
	u32 motion_flag = OBD_DISP_REPEAT;
	ObjDrawAction3DNNMaterialUpdate(obj_work->obj_3d, &motion_flag);
#endif // !_IPHONE
}

// ==========================================================================
// gmGmkSplRingVanish
/*!
 *	ƒMƒ~ƒbƒN SPLƒŠƒ“ƒO ƒƒCƒ“ŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@note
 *		ÚG‚µ‚Ä‚©‚ç‚ÌÁ‹Ž‰‰o
  */
// ==========================================================================
void gmGmkSplRingVanishReady(OBS_OBJECT_WORK *obj_work)
{
#if !_IPHONE
	// ³–ÊŒü‚­‚Ü‚Å‚‘¬‰ñ“]
	obj_work->dir.y -= 0x2000;						// YŽ²‰ñ“]
#else
	// ‹­§³–ÊŒü‚«
	obj_work->obj_3d->mat_frame = 1.0f;
	u32 motion_flag = OBD_DISP_STOP;
	ObjDrawAction3DNNMaterialUpdate(obj_work->obj_3d, &motion_flag);
#endif

	if (!(obj_work->dir.y & 0x7fff)) {
		obj_work->ppFunc = gmGmkSplRingVanish;

		// ƒ\ƒjƒbƒN‚ð‰B‚·‚½‚ß‚ÌƒGƒtƒFƒNƒg
		GMS_EFFECT_3DES_WORK *gm_gmk_splring_effct = 
		GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_SPRING_00);
		GmEffect3DESSetDispOffset(gm_gmk_splring_effct, 
									0, 										// x
									0,						 				// y
									50);									// z À•WƒIƒtƒZƒbƒg
		// ‚«‚ç‚«‚ç‰‰o
		GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_SPRING_01);
	}
}
void gmGmkSplRingVanish(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK		*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	// iPhone‚Å‚à‚»‚ê‚ç‚µ‚­Œ©‚¦‚é‚Ì‚Å‚±‚Ìˆ—‚Å‚¢‚­
	obj_work->dir.y -= 0x1000;						// YŽ²‰ñ“]

	if (!(obj_work->dir.y & 0x7fff)) {
		// ‰B‚µŠ®—¹
		obj_work->ppFunc = NULL;
		obj_work->disp_flag |= OBD_DISP_NODISP;
		// ƒ\ƒjƒbƒN‚àÁ‚·
		ply_work->obj_work.disp_flag |= OBD_DISP_NODISP;
		ply_work->obj_work.move_flag |= OBD_MOVE_NOMOVE;
	}
}
// ==========================================================================
// gmGmkSplRingDefFunc
/*!
 *	ƒMƒ~ƒbƒN SPLƒŠƒ“ƒO ‹éŒ` ÚGˆ—
 *
 *	@param mine_rect	[in] Ž©•ª‚­‚ç‚¢‹éŒ`
 *	@param match_rect	[in] ‘ŠŽèUŒ‚‹éŒ`
 *
 *	@note
 *		ppDef‚É“o˜^\n
 */
// ==========================================================================
void gmGmkSplRingDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_ENEMY_COM_WORK	*com_work = (GMS_ENEMY_COM_WORK*)mine_rect->parent_obj;
	GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	if (com_work == NULL) {
		return;
	}
	if (ply_work == NULL || ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER) {
		return;
	}

	// ƒgƒƒbƒRI—¹
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		GmPlayerSetEndTruckRide(ply_work);
	}

	// ƒvƒŒƒCƒ„[SEQ•ÏX
	GmPlySeqInitSplIn(ply_work, com_work->obj_work.pos);

	// ƒoƒŠƒA, –³“GƒGƒtƒFƒNƒg•\Ž¦OFF
	ply_work->gmk_flag2 |= GMD_PLGF2_BARRIER_DISP_OFF | GMD_PLGF2_INVINCIBLE_DISP_OFF;

	// ƒƒCƒ“ˆ—•ÏX
	((OBS_OBJECT_WORK*)com_work)->ppFunc = gmGmkSplRingVanishReady;
	com_work->obj_work.dir.y = (u16)(com_work->obj_work.dir.y & 0xe000);		// ƒLƒŠ‚Ì—Ç‚¢”’l‚Ü‚Å‰ñ“]‚ði‚ß‚é
	com_work->obj_work.flag |= OBD_OBJECT_NOHIT;
	
	// ƒRƒ“ƒgƒ[ƒ‰[U“®
	GMM_PAD_VIB_SMALL();

	// SE
	GmSoundPlaySE("Special1");
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
