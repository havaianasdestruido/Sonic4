// ==========================================================================
/*!
  @file gmGmkPressPillar.cpp
  @brief ”—‚èo‚·’Œ

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkPressPillar.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
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
#include "gmEffect.h"
#include "gmEffectZone.h"
#include "gmSound.h"

#include "gmGmkPressPillar.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "common/model/GMK_P_PILLAR_MDL.HMB"
#include "common/model/GMK_P_PILLAR_MAT.HMB"


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
// ƒRƒŠƒWƒ‡ƒ“ƒTƒCƒY
#define GMD_GMK_PPIL_COL_WIDTH	(12 * 8)					// •
#define GMD_GMK_PPIL_COL_HEIGHT	(34 * 8)					// ‚‚³

// Šeƒp[ƒcƒIƒtƒZƒbƒg—Ê
#define GMD_GMK_PPIL_PIL_OFS_MAX	(0x00020000)			// “V”Â¨Žx’ŒƒIƒtƒZƒbƒgÅ‘å’l
#define GMD_GMK_PPIL_PIL_OFS_MIN	(0x00018000)			// “V”Â¨Žx’ŒƒIƒtƒZƒbƒgÅ¬’l
#define GMD_GMK_PPIL_SPR_OFS_MAX	(0x0001c000)			// “V”Â¨Žx’ŒƒIƒtƒZƒbƒgÅ‘å’l
#define GMD_GMK_PPIL_SPR_OFS_MIN	(0x00018000)			// “V”Â¨Žx’ŒƒIƒtƒZƒbƒgÅ¬’l(SPR_OFS_MAX - ((PIL_OFS_MAX - PIL_OFS_MIN) / 2))



// GMS_EVE_RECORD_EVENT::flag
#define GMD_GMK_PPIL_ID_NUM_MASK	(0x0f)					// ID_No.(ƒZƒbƒg‚Å‚«‚é”‚Í16‚Ü‚Å)
#define GMD_GMK_PPIL_ID_NUM_MAX		(0x0f)					// ID_No.(ƒZƒbƒg‚Å‚«‚é”‚Í16‚Ü‚Å)
#define GMD_GMK_PPIL_FLAG_SHOCK_ABS	(0x10)					// ƒVƒ‡ƒbƒNƒAƒuƒ][ƒo[‹@\ON/OFF(default on)
#define GMD_GMK_PPIL_FLAG_EFFECT	(0x20)					// ƒGƒtƒFƒNƒgON/OFF				(default on)

// OBS_OBJECT_WORK::user_flag
#define GMD_GMK_PPIL_COLHIT		(0x00000001)
//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkPPillarTopWait(OBS_OBJECT_WORK *obj_work);
static void gmGmkPPillarTopMove(OBS_OBJECT_WORK *obj_work);
static void gmGmkPPillarBodyFollow(OBS_OBJECT_WORK *obj_work);
static void gmGmkPPillarBodyMove(OBS_OBJECT_WORK *obj_work);
static void gmGmkPPillarBodyMoveEx(OBS_OBJECT_WORK *obj_work);
static void gmGmkPPillarSpringFollow(OBS_OBJECT_WORK *obj_work);
static void gmGmkPPillarSpringMove(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

static OBS_ACTION3D_NN_WORK *gm_gmk_press_pillar_obj_3d_list = NULL;
static u8 gm_gmk_press_pillar_sw[GMD_GMK_PPIL_ID_NUM_MAX];
//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkPressPillarBuild
/*!
 *	ƒMƒ~ƒbƒN ”—‚èo‚·’Œ ƒf[ƒ^\’z
 */
// ==========================================================================
void GmGmkPressPillarBuild(void)
{
	gm_gmk_press_pillar_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_PRESS_PILLAR_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_PRESS_PILLAR_TEX),
								0/*draw_flag*/);

	GmGmkPressPillarClear();
}

// ==========================================================================
// GmGmkPressPillarFlush
/*!
 *	ƒMƒ~ƒbƒN ”—‚èo‚·’Œ ƒf[ƒ^•Ð•t‚¯
 */
// ==========================================================================
void GmGmkPressPillarFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_PRESS_PILLAR_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_press_pillar_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmGmkPressPillarInit
/*!
 *	ƒMƒ~ƒbƒN ”—‚èo‚·’Œ ‰Šú‰»ŠÖ”
 *
 *	@param	eve_rec	[inout]	ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param	pos_x	[in]	oŒ»À•W
 *	@param	pos_y	[in]
 *	@param	type	[in]	ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkPressPillarInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;
	u32	pil_type;
	
	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_P_PIL_TOP");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	pil_type = 0;
	if (eve_rec->id == GMD_EVENT_ID_GMK_P_PILLAR_R) {
		pil_type = 1;
	}
	// ƒIƒuƒWƒFƒNƒg“Ç‚Ýž‚Ý¡¡¡¡“V”Â‚ª–{‘Ì¡¡¡¡
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_press_pillar_obj_3d_list[IDB_GMK_P_PILLAR_MDL_GMK_P_PILLAR_TOP_ZNO + pil_type],
					&gmk_work->obj_3d);

	// —DæÝ’è
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK + 1*FX32_ONE;	// ”—‚èo‚·’Œƒ{ƒfƒB‚æ‚è—Dæ‚ð‘O‚É

	// ŒÂ•ÊÝ’è
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->move_flag |= OBD_MOVE_NOCOLOBJ;					// OBJ’nŒ`‚Æ‚ÍHIT‚µ‚È‚¢
	obj_work->move_flag |= OBD_MOVE_NOCOL_W | OBD_MOVE_JUMP;
	obj_work->flag |= OBD_OBJECT_B;
	obj_work->user_flag = 0;
	
	// ’nŒ`Ý’è
	{
		OBS_COLLISION_WORK *col_work = &gmk_work->ene_com.col_work;
	
		col_work->obj_col.obj		= obj_work;
		col_work->obj_col.width		= GMD_GMK_PPIL_COL_WIDTH;
		col_work->obj_col.height	= GMD_GMK_PPIL_COL_HEIGHT;
		col_work->obj_col.ofst_x	= (s16)(-col_work->obj_col.width  /2);
		col_work->obj_col.ofst_y	= 0;
		if (eve_rec->id == GMD_EVENT_ID_GMK_P_PILLAR_R) {
			col_work->obj_col.ofst_y = (s16)(-col_work->obj_col.height);
		}
	}
	// ’nŒ`‚ ‚½‚è
	if (eve_rec->id == GMD_EVENT_ID_GMK_P_PILLAR_N) {
		ObjObjectFieldRectSet(obj_work,
								-GMD_GMK_PPIL_COL_WIDTH/2 +2,	// left
								 -1,							// top
								 GMD_GMK_PPIL_COL_WIDTH/2 -2,	// right
								 GMD_GMK_PPIL_COL_HEIGHT);		// bottom
	} else {
		ObjObjectFieldRectSet(obj_work,
								-GMD_GMK_PPIL_COL_WIDTH/2 +2,	// left
								-GMD_GMK_PPIL_COL_HEIGHT,		// top
								 GMD_GMK_PPIL_COL_WIDTH/2 -2,	// right
								 -1);							// bottom
	}

	// ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmGmkPPillarTopWait;

	// Žqƒ^ƒXƒN¶¬
	{
		OBS_OBJECT_WORK			*sub_work;
		GMS_EFFECT_3DNN_WORK	*efct_work;

		// ¡¡¡¡Žx’Œ¡¡¡¡
		sub_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_3DNN_WORK), obj_work/*parent_obj*/, 0/*sort_prio*/, "GMK_P_PIL_BODY");
		efct_work = (GMS_EFFECT_3DNN_WORK*)sub_work;
		// ƒIƒuƒWƒFƒNƒg“Ç‚Ýž‚Ý
		ObjObjectCopyAction3dNNModel(sub_work,
						&gm_gmk_press_pillar_obj_3d_list[IDB_GMK_P_PILLAR_MDL_GMK_P_PILLAR_BODY_ZNO + pil_type],
						&efct_work->obj_3d);
		// ƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“
		ObjAction3dNNMaterialMotionLoad( &efct_work->obj_3d,
		                                 0,				//reg_file_id
		                                 NULL,			//data_work
		                                 NULL,			//mtn_data_path
		                                 (s32)(IDB_GMK_P_PILLAR_MAT_GMK_P_PILLAR_BODY_ZNV + pil_type),
		                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_PRESS_PILLAR_MAT)->pData );
		ObjDrawObjectActionSet3DNNMaterial( sub_work, 0);
		// —DæÝ’è
		sub_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK + 0*FX32_ONE;
		// ŒÂ•ÊÝ’è
		sub_work->move_flag |= OBD_MOVE_NOCOL;
		sub_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT;
		sub_work->ppFunc = gmGmkPPillarBodyFollow;


		// ¡¡¡¡‚Î‚Ë¡¡¡¡
		sub_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_3DNN_WORK), obj_work/*parent_obj*/, 0/*sort_prio*/, "GMK_P_PIL_SPRING");
		efct_work = (GMS_EFFECT_3DNN_WORK*)sub_work;
		// ƒIƒuƒWƒFƒNƒg“Ç‚Ýž‚Ý
		ObjObjectCopyAction3dNNModel(sub_work,
						&gm_gmk_press_pillar_obj_3d_list[IDB_GMK_P_PILLAR_MDL_GMK_P_PILLAR_SPRING_ZNO],
						&efct_work->obj_3d);
		// —DæÝ’è
		sub_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK + 0*FX32_ONE;
		// ŒÂ•ÊÝ’è
		sub_work->move_flag |= OBD_MOVE_NOCOL;
		sub_work->disp_flag |= OBD_DISP_NODIRFLIP;
		sub_work->ppFunc = gmGmkPPillarSpringFollow;
	}

	return (obj_work);
}


// ==========================================================================
// GmGmkPressPillarStartup
/*!
 *	ƒMƒ~ƒbƒN ”—‚èo‚·’Œ ‹N“®
 *
 *	@param	id_num	[in]	‹N“®ID”Ô†
 */
// ==========================================================================
void GmGmkPressPillarStartup(u16 id_num)
{
	id_num &= GMD_GMK_PPIL_ID_NUM_MASK;
	gm_gmk_press_pillar_sw[id_num] = 1;
}

// ==========================================================================
// GmGmkPressPillarClear
/*!
 *	ƒMƒ~ƒbƒN ”—‚èo‚·’Œ ‹N“®î•ñƒNƒŠƒA
 *
 *	@param	id_num	[in]	‹N“®ID”Ô†
 */
// ==========================================================================
void GmGmkPressPillarClear(void)
{
	MI_CpuClear8(gm_gmk_press_pillar_sw, sizeof(gm_gmk_press_pillar_sw));
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkPPillarTopWait
/*!
 *	ƒMƒ~ƒbƒN ”—‚èo‚·’Œ“V”Â ƒƒCƒ“ŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 * @note
 *	GMS_EVE_RECORD_EVENT::top	: ˆÚ“®‘¬“x(0‚È‚çŠî€‘¬“x)
 *	GMS_EVE_RECORD_EVENT::height: ˆÚ“®‹——£(0‚È‚ç•Çƒqƒbƒg‚·‚é‚Ü‚Å)
 */
// ==========================================================================
void gmGmkPPillarTopWait(OBS_OBJECT_WORK *obj_work)
{
	GMS_EVE_RECORD_EVENT *eve_rec = ((GMS_ENEMY_COM_WORK*)obj_work)->eve_rec;
	u8	id_num = (u8)(eve_rec->flag & GMD_GMK_PPIL_ID_NUM_MASK);

	if (gm_gmk_press_pillar_sw[id_num]) {
		// ƒXƒCƒbƒ`ONyˆÚ“®ŠJŽnz
		fx32 spd_y;
		spd_y = (fx32)(eve_rec->top << (FX32_SHIFT-2));
		spd_y = MTM_MATH_ABS(spd_y);
		if (!spd_y) {
			spd_y = FX32_ONE;
		}
		if (eve_rec->id == GMD_EVENT_ID_GMK_P_PILLAR_N) {
			obj_work->spd.y = -spd_y;
		} else {
			obj_work->spd.y = spd_y;
		}
		obj_work->ppFunc = gmGmkPPillarTopMove;
	}
}

void gmGmkPPillarTopMove(OBS_OBJECT_WORK *obj_work)
{
	GMS_EVE_RECORD_EVENT *eve_rec = ((GMS_ENEMY_COM_WORK*)obj_work)->eve_rec;
	GMS_ENEMY_COM_WORK	 *gmk_work = (GMS_ENEMY_COM_WORK*)obj_work;
	fx32	ofst_y = (fx32)(eve_rec->height << FX32_SHIFT);
	BOOL	stop_flag = FALSE;

	// ‹K’èˆÚ“®—Êƒ`ƒFƒbƒN
	if (ofst_y) {
		if (eve_rec->id == GMD_EVENT_ID_GMK_P_PILLAR_N) {
			ofst_y = gmk_work->born_pos_y - ofst_y;
			if (obj_work->pos.y <= ofst_y) {
				obj_work->pos.y = ofst_y;
				stop_flag = TRUE;
			}
		} else {
			ofst_y = gmk_work->born_pos_y + ofst_y;
			if (obj_work->pos.y >= ofst_y) {
				obj_work->pos.y = ofst_y;
				stop_flag = TRUE;
			}
		}
	}

	// ƒqƒbƒgƒ`ƒFƒbƒN
	if (obj_work->move_flag & OBD_MOVE_COL_MASK) {
		stop_flag = TRUE;
	}

	if (stop_flag) {
		// •Çƒqƒbƒg or —\’èˆÚ“®—ÊI—¹
		obj_work->spd.y = 0;
		obj_work->ppFunc = NULL;
		obj_work->user_flag |= GMD_GMK_PPIL_COLHIT;

		if (!(eve_rec->flag & GMD_GMK_PPIL_FLAG_EFFECT)) {
			// ƒGƒtƒFƒNƒg¶¬‹‘”Ûƒtƒ‰ƒO‚ªON‚Å‚È‚¯‚ê‚ÎƒGƒtƒFƒNƒg‚ð¶¬

			OBS_OBJECT_WORK *effobj_work = 
				(OBS_OBJECT_WORK*)GmEfctZoneEsCreate(obj_work, GSD_MAIN_ZONE_TYPE_4, GME_EFCT_Z04_IDX_PILLAR_Z4);
			if (eve_rec->id == GMD_EVENT_ID_GMK_P_PILLAR_N) {
				// ãŒü‚«‚Í”½“]
				effobj_work->dir.z = 0x8000;
			}
		}
	}
}

// ==========================================================================
// gmGmkPPillarBodyFollow
/*!
 *	ƒMƒ~ƒbƒN ”—‚èo‚·’ŒŽx’Œ ƒƒCƒ“ŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 */
// ==========================================================================
void gmGmkPPillarBodyFollow(OBS_OBJECT_WORK *obj_work)
{
	OBS_OBJECT_WORK 		*parent_work = obj_work->parent_obj;

	fx32 pos_y = parent_work->pos.y;
	if (((GMS_ENEMY_COM_WORK*)parent_work)->eve_rec->id == GMD_EVENT_ID_GMK_P_PILLAR_N) {
		pos_y += GMD_GMK_PPIL_PIL_OFS_MAX;
	} else {
		pos_y -= GMD_GMK_PPIL_PIL_OFS_MAX;
	}
	obj_work->pos.y = pos_y;

	if (parent_work->user_flag & GMD_GMK_PPIL_COLHIT) {
		// e‚ª•Çƒqƒbƒg‚É‚æ‚è’âŽ~
		if (((GMS_ENEMY_COM_WORK*)parent_work)->eve_rec->flag & GMD_GMK_PPIL_FLAG_SHOCK_ABS) {
			// ƒVƒ‡ƒbƒNOFF
			obj_work->spd.y = 0;
			obj_work->ppFunc = NULL;
		} else {
			// ƒVƒ‡ƒbƒNON
			obj_work->spd.y = obj_work->user_timer;				// Ž©—¥ˆÚ“®ŠJŽn
			obj_work->ppFunc = gmGmkPPillarBodyMove;
			obj_work->user_flag = 0;						// L‚ÑLkó‹µƒtƒ‰ƒO
		}
	} else {
		// e‚ÍˆÚ“®’†
		if (parent_work->spd.y) {
			obj_work->user_timer = parent_work->spd.y;		// ‘¬“x‹L˜^
		}
	}
}

#if 0	// ‹@ŠB“I‚Éˆ³—ÍŠ|‚©‚è‚Á‚Ï‚È‚µver
void gmGmkPPillarBodyMove(OBS_OBJECT_WORK *obj_work)
{
	OBS_OBJECT_WORK 		*parent_work = obj_work->parent_obj;
	GMS_EVE_RECORD_EVENT	*eve_rec = ((GMS_ENEMY_COM_WORK*)parent_work)->eve_rec;

	fx32	spd_y;
	spd_y = obj_work->spd.y * 3 / 4;
	if (0x0000200 < MTM_MATH_ABS(spd_y)) {
		obj_work->spd.y = spd_y;
	}

	if (eve_rec->id == GMD_EVENT_ID_GMK_P_PILLAR_N) {
		// ã¸’†
		if (obj_work->pos.y <= (parent_work->pos.y + GMD_GMK_PPIL_PIL_OFS_MIN)) {
			// ’âŽ~
			obj_work->pos.y = (parent_work->pos.y + GMD_GMK_PPIL_PIL_OFS_MIN);
			obj_work->spd.y = 0;
			obj_work->ppFunc = NULL;
		}
	} else {
		// ‰º~’†
		if (obj_work->pos.y >= (parent_work->pos.y - GMD_GMK_PPIL_PIL_OFS_MIN)) {
			// ’âŽ~
			obj_work->pos.y = (parent_work->pos.y - GMD_GMK_PPIL_PIL_OFS_MIN);
			obj_work->spd.y = 0;
			obj_work->ppFunc = NULL;
		}
	}
}
#else	// Œ¸Š¨L‚Ñver

void gmGmkPPillarBodyMove(OBS_OBJECT_WORK *obj_work)
{
	// Œ¸Š’†
	OBS_OBJECT_WORK 		*parent_work = obj_work->parent_obj;
	GMS_EVE_RECORD_EVENT	*eve_rec = ((GMS_ENEMY_COM_WORK*)parent_work)->eve_rec;

	fx32	spd_y;
	spd_y = obj_work->spd.y * 3 / 4;
	if (0x000010 < MTM_MATH_ABS(spd_y)) {
		obj_work->spd.y = spd_y;
	} else {
		// ‘¬“x“I‚ÉƒTƒXk‚Ý‚«‚Á‚½
		if (eve_rec->id == GMD_EVENT_ID_GMK_P_PILLAR_N) {
//			obj_work->spd.y = 0x20;
			obj_work->spd.y = -obj_work->user_timer / 32;
		} else {
//			obj_work->spd.y = -0x20;
			obj_work->spd.y = -obj_work->user_timer / 32;
		}
		obj_work->ppFunc = gmGmkPPillarBodyMoveEx;
	}

	if (eve_rec->id == GMD_EVENT_ID_GMK_P_PILLAR_N) {
		// ã¸’†
		if (obj_work->pos.y <= (parent_work->pos.y + GMD_GMK_PPIL_PIL_OFS_MIN)) {
			// ƒTƒX’ê•t‚«’âŽ~
			obj_work->pos.y = (parent_work->pos.y + GMD_GMK_PPIL_PIL_OFS_MIN);
			obj_work->spd.y = 0x20;
			obj_work->ppFunc = gmGmkPPillarBodyMoveEx;
		}
	} else {
		// ‰º~’†
		if (obj_work->pos.y >= (parent_work->pos.y - GMD_GMK_PPIL_PIL_OFS_MIN)) {
			// ƒTƒX’ê•t‚«’âŽ~
			obj_work->pos.y = (parent_work->pos.y - GMD_GMK_PPIL_PIL_OFS_MIN);
			obj_work->spd.y = -0x20;
			obj_work->ppFunc = gmGmkPPillarBodyMoveEx;
		}
	}
	parent_work->user_work = (u32)obj_work->pos.y;	// ’ŒYpos‚ðeƒ[ƒN‚É•ÛŽ
}

void gmGmkPPillarBodyMoveEx(OBS_OBJECT_WORK *obj_work)
{
	OBS_OBJECT_WORK 		*parent_work = obj_work->parent_obj;
	GMS_EVE_RECORD_EVENT	*eve_rec = ((GMS_ENEMY_COM_WORK*)parent_work)->eve_rec;

	if (!obj_work->user_flag) {
		// L‚Ñ‰Á‘¬’†
		obj_work->spd.y = obj_work->spd.y * 8 / 7;
		if (0x0000800 <= MTM_MATH_ABS(obj_work->spd.y)) {
			obj_work->user_flag = 1;
		}
	} else {
		// L‚ÑŒ¸‘¬’†
		fx32 spd_y = obj_work->spd.y * 7 / 8;
		if (0x0000080 < MTM_MATH_ABS(obj_work->spd.y)) {
			obj_work->spd.y = spd_y;
		}
	}

	if (eve_rec->id == GMD_EVENT_ID_GMK_P_PILLAR_N) {
		// ã¸’†
		if (obj_work->pos.y >= (parent_work->pos.y + GMD_GMK_PPIL_PIL_OFS_MAX)) {
			// Å‘åL’·‚É’B‚µ‚½
			obj_work->pos.y = (parent_work->pos.y + GMD_GMK_PPIL_PIL_OFS_MAX);
			obj_work->spd.y = 0;
			obj_work->ppFunc = NULL;
		}
	} else {
		// ‰º~’†
		if (obj_work->pos.y <= (parent_work->pos.y - GMD_GMK_PPIL_PIL_OFS_MAX)) {
			// Å‘åL’·‚É’B‚µ‚½
			obj_work->pos.y = (parent_work->pos.y - GMD_GMK_PPIL_PIL_OFS_MAX);
			obj_work->spd.y = 0;
			obj_work->ppFunc = NULL;
		}
	}
	parent_work->user_work = (u32)obj_work->pos.y;	// ’ŒYpos‚ðeƒ[ƒN‚É•ÛŽ
}
#endif
// ==========================================================================
// gmGmkPPillarSpringFollow
/*!
 *	ƒMƒ~ƒbƒN ”—‚èo‚·’Œ‚Î‚Ë ƒƒCƒ“ŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 */
// ==========================================================================
void gmGmkPPillarSpringFollow(OBS_OBJECT_WORK *obj_work)
{
	OBS_OBJECT_WORK 		*parent_work = obj_work->parent_obj;

	fx32 pos_y = parent_work->pos.y;
	if (((GMS_ENEMY_COM_WORK*)parent_work)->eve_rec->id == GMD_EVENT_ID_GMK_P_PILLAR_N) {
		pos_y += GMD_GMK_PPIL_SPR_OFS_MAX;
	} else {
		pos_y -= GMD_GMK_PPIL_SPR_OFS_MAX;
	}
	obj_work->pos.y = pos_y;

	if (parent_work->user_flag & GMD_GMK_PPIL_COLHIT) {
		// e‚ª•Çƒqƒbƒg‚É‚æ‚è’âŽ~
		if (((GMS_ENEMY_COM_WORK*)parent_work)->eve_rec->flag & GMD_GMK_PPIL_FLAG_SHOCK_ABS) {
			// ƒVƒ‡ƒbƒNOFF
			obj_work->ppFunc = NULL;
		} else {
			// ƒVƒ‡ƒbƒNON
			obj_work->ppFunc = gmGmkPPillarSpringMove;
		}
		obj_work->spd.y = 0;
	}
}

void gmGmkPPillarSpringMove(OBS_OBJECT_WORK *obj_work)
{
	OBS_OBJECT_WORK 		*parent_work = obj_work->parent_obj;
	GMS_EVE_RECORD_EVENT	*eve_rec = ((GMS_ENEMY_COM_WORK*)parent_work)->eve_rec;

	fx32	ofs_y;
	fx32 	pos_y;
	float	rate;

	if (eve_rec->id == GMD_EVENT_ID_GMK_P_PILLAR_N) {
		// À•W
		ofs_y = parent_work->pos.y + GMD_GMK_PPIL_PIL_OFS_MIN - (fx32)parent_work->user_work;
		ofs_y /= 2;
		obj_work->pos.y = parent_work->pos.y + GMD_GMK_PPIL_SPR_OFS_MIN - ofs_y;
		// ‚Î‚Ëk¬
		pos_y = obj_work->pos.y - (parent_work->pos.y + GMD_GMK_PPIL_SPR_OFS_MIN);
		pos_y = MTM_MATH_ABS(pos_y);
		rate  = (float)(pos_y) / (GMD_GMK_PPIL_SPR_OFS_MAX - GMD_GMK_PPIL_SPR_OFS_MIN);
		obj_work->scale.y = FXM_FLOAT_TO_FX32(rate);
	} else {
		// À•W
		ofs_y = parent_work->pos.y - GMD_GMK_PPIL_PIL_OFS_MIN - (fx32)parent_work->user_work;
		ofs_y /= 2;
		obj_work->pos.y = parent_work->pos.y - GMD_GMK_PPIL_SPR_OFS_MIN - ofs_y;
		// ‚Î‚Ëk¬
		pos_y = obj_work->pos.y - (parent_work->pos.y - GMD_GMK_PPIL_SPR_OFS_MIN);
		pos_y = MTM_MATH_ABS(pos_y);
		rate  = (float)(pos_y) / (GMD_GMK_PPIL_SPR_OFS_MAX - GMD_GMK_PPIL_SPR_OFS_MIN);
		obj_work->scale.y = FXM_FLOAT_TO_FX32(rate);
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
