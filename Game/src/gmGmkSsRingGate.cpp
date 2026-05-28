// ==========================================================================
/*!
  @file gmGmkSsRingGate.cpp
  @brief ÉMÉ~ÉbÉNSpecialStageÉäÉìÉOÉQÅ[Ég

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSsRingGate.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmGmkSsRingGate.cpp,v $
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
#include "gmPadVib.h"
#include "gmSplStage.h"

#if _IPHONE
#include "gmTvx.h"
#endif // _IPHONE

#include "gmGmkSsRingGate.h"

// ÉfÅ[É^ÉwÉbÉ_
#include "common/model/GMK_SS_RINGGATE_MDL.HMB"
#include "common/model/GMK_SS_RINGGATE_MAT.HMB"

#if _IPHONE
// commonÇ∆ç\ê¨Ç™ïœÇÌÇ¡ÇƒÇ¢ÇÈèÍçáÇ…égópÇ∑ÇÈíËã`
#include "iphone/model/GMK_SS_RINGGATE_TVX.HMB"
#endif // _IPHONE

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------

#define GMD_GMK_SS_RINGGATE_TEST_TVX       (1 & _IPHONE)

#define GMD_GMK_SS_RINGGATE_EVE_FLAG_V	(0x01)	//!< ècíuÇ´ÉtÉâÉO
#if GMD_GMK_SS_RINGGATE_TEST_TVX
#define GMD_GMK_SS_RINGGATE_FLAG_UV_WAIT_UPDATE_SHIFT	(   8)	//!< UV_WAITê›íËBitÉVÉtÉgíl
#define GMD_GMK_SS_RINGGATE_FLAG_UV_WAIT_UPDATE_MASK	(0x7f)	//!< UV_WAITê›íËÉ}ÉXÉN(0Å`63)
#define GMD_GMK_SS_RINGGATE_FLAG_UV_WAIT_GET_SHIFT		(  13)	//!< UV_WAITì¸éËäJénBitÉVÉtÉgíl
#define GMD_GMK_SS_RINGGATE_FLAG_UV_WAIT_GET_MASK		(0x03)	//!< UV_WAITì¸éËäJénÉ}ÉXÉN(0Å`3)
#endif // GMD_GMK_SS_RINGGATE_TEST_TVX

#define	GMD_GMK_SS_GATE_ANIM_LOOP_TIME	(128)	//!< ÉAÉjÉÅÅ[ÉVÉáÉìëçéûä‘

// íµÇÀï‘ÇµHITÉRÉäÉWÉáÉìÉTÉCÉY
#define GMD_GMK_SS_RG_HIT_RECT_X	(52)	// 48
#define GMD_GMK_SS_RG_HIT_RECT_Y	(20)	// 16

#define GMD_GMK_SS_RINGGATE_VANISH_TIME	(20)	//!< è¡ãééûä‘
#define GMD_GMK_SS_RINGGATE_MONO_TIME	(GMD_GMK_SS_RINGGATE_VANISH_TIME-12)	//!< ÉÇÉmÉâÉãÇ÷ÇÃêÿÇËë÷Ç¶éûä‘
#define GMD_GMK_SS_RINGGATE_VANISH_ALPHA_RATE (0.2f)	//!< ãKíËÉNÉäÉAå„ÇÃìßâﬂó¶
#define GMD_GMK_SS_RINGGATE_VANISH_ALPHA (GMD_GMK_SS_RINGGATE_VANISH_TIME * GMD_GMK_SS_RINGGATE_VANISH_ALPHA_RATE)	//!< ãKíËÉNÉäÉAå„ÇÃìßâﬂó¶

#define GMD_GMK_SS_NUM_ID_RING	0				//!<éqÉ^ÉXÉNID
#define GMD_GMK_SS_NUM_ID_10	1
#define GMD_GMK_SS_NUM_ID_1		2

#define GMD_GMK_SS_NUM_OFFSET	(9 << FX32_SHIFT)	//!< êîéöóﬁï\é¶ä‘äu
//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkSsRingGateMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkSsRingGateVanish(OBS_OBJECT_WORK *obj_work);
#if 1
static void gmGmkSsRingGateDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
#endif
static void gmGmkSsRingGateNumMain(OBS_OBJECT_WORK *obj_work);

#if GMD_GMK_SS_RINGGATE_TEST_TVX
static void gmGmkSsRingGateDrawFunc(OBS_OBJECT_WORK *obj_work);
#endif // GMD_GMK_SS_RINGGATE_TEST_TVX

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_ss_ringgate_obj_3d_list = NULL;

#if GMD_GMK_SS_RINGGATE_TEST_TVX
static AMS_AMB_HEADER* gm_gmk_ss_ringgate_obj_tvx_list = NULL;
#endif // GMD_GMK_SS_RINGGATE_TEST_TVX

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkSsRingGateBuild
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ÉäÉìÉOÉQÅ[Ég ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkSsRingGateBuild(void)
{
	gm_gmk_ss_ringgate_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_RINGGATE_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_RINGGATE_TEX),
								0/*draw_flag*/);
#if GMD_GMK_SS_RINGGATE_TEST_TVX
	void* tvx = GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_SS_RINGGATE_TVX );
	amBindConv((Uint8*)tvx);
	gm_gmk_ss_ringgate_obj_tvx_list = (AMS_AMB_HEADER*)tvx;
#endif // GMD_GMK_SS_RINGGATE_TEST_TVX
}

// ==========================================================================
// GmGmkSsRingGateFlush
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ÉäÉìÉOÉQÅ[Ég ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkSsRingGateFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_RINGGATE_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_ss_ringgate_obj_3d_list, amb->file_num);
#if GMD_GMK_SS_RINGGATE_TEST_TVX
	gm_gmk_ss_ringgate_obj_tvx_list = NULL;
#endif // GMD_GMK_SS_RINGGATE_TEST_TVX
}

// ==========================================================================
// GmGmkSsRingGateInit
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ÉäÉìÉOÉQÅ[Ég èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSsRingGateInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_SS_RINGGATE");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	MTM_ASSERT(eve_rec->left < GMD_SS_RINGGATE_MAX);
	obj_work->user_work = GmSplStageRingGateNumGet(eve_rec->left);				// í âﬂâ¬î\ÉäÉìÉOñáêî
	obj_work->user_flag = (u32)(eve_rec->flag & GMD_GMK_SS_RINGGATE_EVE_FLAG_V);// ècâ°ÉtÉâÉOï€éù
	obj_work->user_timer = GMD_GMK_SS_RINGGATE_VANISH_TIME;						// è¡ãééûä‘

	if ((u16)(obj_work->user_work) > g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->ring_num) {
		// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›ÅiÉJÉâÅ[Åj
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_gmk_ss_ringgate_obj_3d_list[IDB_GMK_SS_RINGGATE_MDL_SS_GATE_ZNO],
						&gmk_work->obj_3d);

#if !GMD_GMK_SS_RINGGATE_TEST_TVX
		// É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
		ObjAction3dNNMaterialMotionLoad( &gmk_work->obj_3d,
		                                 0,				//reg_file_id
		                                 NULL,			//data_work
		                                 NULL,			//mtn_data_path
		                                 IDB_GMK_SS_RINGGATE_MAT_SS_GATE_ZNV,
		                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_SS_RINGGATE_MAT)->pData );
		ObjDrawObjectActionSet3DNNMaterial( obj_work, 0 );

		obj_work->disp_flag |= OBD_DISP_REPEAT;
#endif // GMD_GMK_SS_RINGGATE_TEST_TVX

		// ÉAÉNÉVÉáÉìÉ^ÉCÉ~ÉìÉOà ëä
		u32 sync_timer = g_gm_main_system.sync_time % GMD_GMK_SS_GATE_ANIM_LOOP_TIME;
		gmk_work->obj_3d.mat_frame = (float)(sync_timer);
#if GMD_GMK_SS_RINGGATE_TEST_TVX
		obj_work->ppOut = gmGmkSsRingGateDrawFunc;
		obj_work->user_flag = (obj_work->user_flag & GMD_GMK_SS_RINGGATE_EVE_FLAG_V) | ((sync_timer & GMD_GMK_SS_RINGGATE_FLAG_UV_WAIT_UPDATE_MASK) << GMD_GMK_SS_RINGGATE_FLAG_UV_WAIT_UPDATE_SHIFT);
#else
		gmk_work->obj_3d.mat_frame = (float)(sync_timer);
#endif // GMD_GMK_SS_RINGGATE_TEST_TVX
	} else {
		// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›ÅiÉÇÉmÉNÉçÅj
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_gmk_ss_ringgate_obj_3d_list[IDB_GMK_SS_RINGGATE_MDL_SS_GATE_MONO_ZNO],
						&gmk_work->obj_3d);
	}

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;				// à⁄ìÆñ≥Çµ ínå`Ç†ÇΩÇËñ≥Çµ
	gmk_work->ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NOFREE_DIFF_DATA;	// diff_dataÇäJï˙ÇµÇ»Ç¢
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;	// ÉâÉCÉgÇOñ≥å¯
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_1;	// ÉâÉCÉgÇPóLå¯

	if ((u16)(obj_work->user_work) > g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->ring_num) {
		// ãKíËÉäÉìÉOêîÇ…íBÇµÇƒÇ¢Ç»Ç¢éûÇÃÇ›
		// ïtëÆï\é¶ï®çÏê¨
		OBS_OBJECT_WORK			*sub_work;
		GMS_EFFECT_3DNN_WORK	*efct_work;

		// Å°Å°Å°ÉäÉìÉOÅ°Å°Å°
		sub_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_3DNN_WORK), obj_work/*parent_obj*/, 0/*sort_prio*/, "GATERING");
		efct_work = (GMS_EFFECT_3DNN_WORK*)sub_work;
		// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
		ObjObjectCopyAction3dNNModel(sub_work,
						&gm_gmk_ss_ringgate_obj_3d_list[IDB_GMK_SS_RINGGATE_MDL_SS_GATE_R_ZNO],
						&efct_work->obj_3d);
		// óDêÊê›íË
		sub_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK + FX32_ONE * 16;
		// å¬ï ê›íË
		sub_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ìñÇΩÇËñ≥Çµ
//		sub_work->disp_flag |= OBD_DISP_NODIRFLIP;
		sub_work->disp_flag &= ~OBD_DISP_NODIR;
		sub_work->user_work = GMD_GMK_SS_NUM_ID_RING;
		sub_work->ppFunc = gmGmkSsRingGateNumMain;
		sub_work->dir.y = 0xc000;			// å¸Ç´ï‚ê≥

		// Å°Å°Å°êîéö(è\ÇÃåÖ)Å°Å°Å°
		sub_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_3DNN_WORK), obj_work/*parent_obj*/, 0/*sort_prio*/, "GATENUM10");
		efct_work = (GMS_EFFECT_3DNN_WORK*)sub_work;
		// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
		sub_work->user_timer = (s32)(obj_work->user_work / 10);
		ObjObjectCopyAction3dNNModel(sub_work,
						&gm_gmk_ss_ringgate_obj_3d_list[IDB_GMK_SS_RINGGATE_MDL_SS_GATE_0F_ZNO + sub_work->user_timer],
						&efct_work->obj_3d);
		// óDêÊê›íË
		sub_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK + FX32_ONE * 16;
		// å¬ï ê›íË
		sub_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ìñÇΩÇËñ≥Çµ
//		sub_work->disp_flag |= OBD_DISP_NODIRFLIP;
		sub_work->disp_flag &= ~OBD_DISP_NODIR;
		sub_work->user_work = GMD_GMK_SS_NUM_ID_10;
		sub_work->ppFunc = gmGmkSsRingGateNumMain;
		sub_work->dir.y = 0xc000;			// å¸Ç´ï‚ê≥

		// Å°Å°Å°êîéö(àÍÇÃåÖ)Å°Å°Å°
		sub_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_3DNN_WORK), obj_work/*parent_obj*/, 0/*sort_prio*/, "GATENUM1");
		efct_work = (GMS_EFFECT_3DNN_WORK*)sub_work;
		// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
		sub_work->user_timer = (s32)(obj_work->user_work % 10);
		ObjObjectCopyAction3dNNModel(sub_work,
						&gm_gmk_ss_ringgate_obj_3d_list[IDB_GMK_SS_RINGGATE_MDL_SS_GATE_0F_ZNO + sub_work->user_timer],
						&efct_work->obj_3d);
		// óDêÊê›íË
		sub_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK + FX32_ONE * 16;
		// å¬ï ê›íË
		sub_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ìñÇΩÇËñ≥Çµ
//		sub_work->disp_flag |= OBD_DISP_NODIRFLIP;
		sub_work->disp_flag &= ~OBD_DISP_NODIR;
		sub_work->user_work = GMD_GMK_SS_NUM_ID_1;
		sub_work->ppFunc = gmGmkSsRingGateNumMain;
		sub_work->dir.y = 0xc000;			// å¸Ç´ï‚ê≥
	}
	// ÉÅÉCÉìèàóù
	obj_work->ppFunc = gmGmkSsRingGateMain;

	// ãÈå`ê›íË
	{
		OBS_RECT_WORK	*rect_work;
		gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
		rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
		rect_work->ppDef = gmGmkSsRingGateDefFunc;
		ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
		if (eve_rec->flag & GMD_GMK_SS_RINGGATE_EVE_FLAG_V) {
			// ècíuÇ´
			ObjRectWorkSet(rect_work, -GMD_GMK_SS_RG_HIT_RECT_Y,
									  -GMD_GMK_SS_RG_HIT_RECT_X,
									   GMD_GMK_SS_RG_HIT_RECT_Y,
									   GMD_GMK_SS_RG_HIT_RECT_X);					// work, L, U, R, D
			obj_work->dir.z = 0x4000;										// 90ìxâÒì]
		} else {
			// â°íuÇ´
			ObjRectWorkSet(rect_work, -GMD_GMK_SS_RG_HIT_RECT_X,
									  -GMD_GMK_SS_RG_HIT_RECT_Y,
									   GMD_GMK_SS_RG_HIT_RECT_X,
									   GMD_GMK_SS_RG_HIT_RECT_Y);					// work, L, U, R, D
		}
		rect_work->flag |= OBD_RECT_OUT;
	}

	// ínå`ê›íË
	{
		OBS_COLLISION_WORK *col_work = &gmk_work->ene_com.col_work;
	
		col_work->obj_col.obj		= obj_work;
		col_work->obj_col.diff_data	= (s8*)g_gm_default_col;			// è„â∫ç∂âEê⁄êGñ äpìxèÓïÒÇìæÇÈÇΩÇﬂÇ…ÇÕ
//		col_work->obj_col.dir_data	= (u8*)g_gm_ss_parts_dir;			// diff_data/dir_dataÇégópÇ∑ÇÈïKóvÇ†ÇË
		if (eve_rec->flag & GMD_GMK_SS_RINGGATE_EVE_FLAG_V) {
			// ècíuÇ´
			col_work->obj_col.width		= 3*8;							// ínå`ÉTÉCÉYê›íË(8dotíPà êßå¿Ç†ÇË)
			col_work->obj_col.height	= 12*8;							// êßñÒèúäOÇ∑ÇÈÇΩÇﬂÇ…ÇÕdiff_dataÇêÍópÇ≈éùÇ¬ïKóvÇ™Ç†ÇÈ
		} else {
			// â°íuÇ´
			col_work->obj_col.width		= 12*8;							// ínå`ÉTÉCÉYê›íË(8dotíPà êßå¿Ç†ÇË)
			col_work->obj_col.height	= 3*8;							// êßñÒèúäOÇ∑ÇÈÇΩÇﬂÇ…ÇÕdiff_dataÇêÍópÇ≈éùÇ¬ïKóvÇ™Ç†ÇÈ
		}
		col_work->obj_col.ofst_x	= (s16)(0 - col_work->obj_col.width / 2);
		col_work->obj_col.ofst_y	= (s16)(0 - col_work->obj_col.height / 2);
		col_work->obj_col.attr		= OBD_COL_DATA_ATTR_CLIFF;			// äRàµÇ¢
		col_work->obj_col.flag		|= OBD_COLOBJ_NOFREE_DIFF_DATA		// diff_dataÇäJï˙ÇµÇ»Ç¢
//									 | OBD_COLOBJ_NOFREE_DIR_DATA		// dir_dataÇäJï˙ÇµÇ»Ç¢
									 | OBD_COLOBJ_NODIR_PARENT;			// êeÇÃäpìxñ≥éã
	}

	// ê∂ê¨éûÇ…ãKíËÉäÉìÉOêîÇ…íBÇµÇƒÇ¢ÇΩÇÁç≈èâÇ©ÇÁìßâﬂ
	if ((u16)(obj_work->user_work) <= g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->ring_num) {
		// ãKíËñáêîà»è„ÉäÉìÉOÇéÊìæÇµÇΩ
		gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;	// ìñÇΩÇËîªíËè¡ãé
		gmk_work->ene_com.col_work.obj_col.obj = NULL;						// ínå`ÉRÉäÉWÉáÉìè¡ãé
		// ÉAÉãÉtÉ@ê›íË
//		obj_work->disp_flag |= OBD_DISP_DRAWSTATE;							// ÉÜÅ[ÉUÅ[ï`âÊÉXÉeÅ[ÉgîΩâf
//		obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_ALPHA;			// ÉAÉãÉtÉ@êßå‰Ç†ÇË
//		obj_work->obj_3d->draw_state.alpha.alpha = GMD_GMK_SS_RINGGATE_VANISH_ALPHA_RATE;
		// Å™îºìßñæñ≥ÇµÇ…Ç»ÇËÇ‹ÇµÇΩ
		obj_work->ppFunc = NULL;
#if GMD_GMK_SS_RINGGATE_TEST_TVX
		obj_work->ppOut = ObjDrawActionSummary; // í èÌï`âÊ
#endif // GMD_GMK_SS_RINGGATE_TEST_TVX
	}
	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkSsRingGateMain
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ÉäÉìÉOÉQÅ[Ég ÉÅÉCÉì
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsRingGateMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK		*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
/*
	OBS_COLLISION_OBJ	*obj_col = &obj_work->col_work->obj_col;

	if (obj_col->toucher_obj == &ply_work->obj_work) {
		// É\ÉjÉbÉNÇ™ÉqÉbÉg
		ply_work->obj_work.spd.x = -ply_work->obj_work.spd.x;					// X/Yë¨ìxîΩì]ÇµÇƒ
		ply_work->obj_work.spd.y = -ply_work->obj_work.spd.y;					//  ÉQÅ[ÉgÇ…ÇÕÇ∂Ç©ÇÍÇΩólÇ…å©ÇπÇÈ
		ply_work->obj_work.spd_m = -ply_work->obj_work.spd_m;					//  
	}
*/
#if GMD_GMK_SS_RINGGATE_TEST_TVX
	//	 ÉÇÅ[ÉVÉáÉìçXêV
	u32 timer = (obj_work->user_flag >> GMD_GMK_SS_RINGGATE_FLAG_UV_WAIT_UPDATE_SHIFT) & GMD_GMK_SS_RINGGATE_FLAG_UV_WAIT_UPDATE_MASK;
	timer++;
	obj_work->user_flag = (obj_work->user_flag & GMD_GMK_SS_RINGGATE_EVE_FLAG_V) | ((timer & GMD_GMK_SS_RINGGATE_FLAG_UV_WAIT_UPDATE_MASK) << GMD_GMK_SS_RINGGATE_FLAG_UV_WAIT_UPDATE_SHIFT);
#endif // GMD_GMK_SS_RINGGATE_TEST_TVX

	if ((u16)(obj_work->user_work) <= ply_work->ring_num) {
		// ãKíËñáêîà»è„ÉäÉìÉOÇéÊìæÇµÇΩ
		GMS_ENEMY_3D_WORK	*gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
		gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;	// ìñÇΩÇËîªíËè¡ãé
		gmk_work->ene_com.col_work.obj_col.obj = NULL;						// ínå`ÉRÉäÉWÉáÉìè¡ãé
		obj_work->ppFunc = gmGmkSsRingGateVanish;							// è¡ñ≈èàóùÇ÷


		// ÉAÉãÉtÉ@ê›íË
		obj_work->disp_flag |= OBD_DISP_DRAWSTATE;							// ÉÜÅ[ÉUÅ[ï`âÊÉXÉeÅ[ÉgîΩâf
		obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_ALPHA;			// ÉAÉãÉtÉ@êßå‰Ç†ÇË
		obj_work->obj_3d->draw_state.alpha.alpha = 1.0f;					// îºìßñæ

// SEÉRÅ[ÉãÇÕ gmSplStage.cpp Ç≈àÍäáä«óù
//		// SE
//		GmSoundPlaySE("Special7");

		obj_work->user_timer = GMD_GMK_SS_RINGGATE_VANISH_TIME;				// è¡ãééûä‘

#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
		// ÉGÉtÉFÉNÉgê∂ê¨
		GMS_EFFECT_3DES_WORK* effect_work = 
			GmEfctZoneEsCreate(obj_work,
							GSD_MAIN_ZONE_TYPE_SS,
							GME_EFCT_ZSS_IDX_RING_GATE);					// ÉGÉtÉFÉNÉgî≠ê∂
		GmEffect3DESSetDispOffset(effect_work, 0.0f, 0.0f, 8.0f);			// ç¿ïWÉIÉtÉZÉbÉg
		effect_work->efct_com.obj_work.dir.z = obj_work->dir.z;				// âÒì]ï˚å¸ÉRÉsÅ[
		effect_work->efct_com.obj_work.flag |= OBD_OBJECT_PARENT_NODIE;		// êeÇ™éÄÇÒÇ≈Ç‡ì∆ÇËóßÇø
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
	}
}
// ==========================================================================
// gmGmkSsRingGateVanish
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ÉäÉìÉOÉQÅ[Ég è¡é∏
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsRingGateVanish(OBS_OBJECT_WORK *obj_work)
{
	obj_work->user_timer--;
	if (obj_work->user_timer == GMD_GMK_SS_RINGGATE_MONO_TIME) {
#if GMD_GMK_SS_RINGGATE_TEST_TVX
		obj_work->ppOut = ObjDrawActionSummary; // í èÌï`âÊ
#else
		// ÉÇÉmÉâÉãÉÇÉfÉãÇ…êÿÇËë÷Ç¶
		// ÉÇÅ[ÉVÉáÉìàÍíUäJï˙
		ObjAction3dNNMotionRelease(obj_work->obj_3d);
#endif // GMD_GMK_SS_RINGGATE_TEST_TVX
		// ÉJÉâÅ[ÉÇÉfÉãâï˙
		ObjObjectAction3dNNModelReleaseCopy(obj_work);
		// ÉÇÉmÉNÉçÉÇÉfÉãÇ…êÿÇËë÷Ç¶
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_gmk_ss_ringgate_obj_3d_list[IDB_GMK_SS_RINGGATE_MDL_SS_GATE_MONO_ZNO],
						&((GMS_ENEMY_3D_WORK*)obj_work)->obj_3d);
		// à»å„ÉÇÅ[ÉVÉáÉìçƒê∂ñ≥Çµ
//		obj_work->disp_flag |= OBD_DISP_DRAWSTATE;							// ÉÜÅ[ÉUÅ[ï`âÊÉXÉeÅ[ÉgîΩâf
//		obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_ALPHA;			// ÉAÉãÉtÉ@êßå‰Ç†ÇË
		// îºìßñæÇ‡ñ≥Çµ
	}
	if (obj_work->user_timer > GMD_GMK_SS_RINGGATE_VANISH_ALPHA) {
		// ìßÇØÇƒÇ¢Ç≠
		obj_work->obj_3d->draw_state.alpha.alpha = (float)(obj_work->user_timer) / (float)(GMD_GMK_SS_RINGGATE_VANISH_TIME);
	} else {
#if 0
		// è¡ñ≈
		GMS_ENEMY_3D_WORK	*gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
		gmk_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_DIE;					// çƒê∂ê¨ÇµÇ»Ç¢
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;								// è¡ñ≈
#else
		// ãKíËìßâﬂó¶Ç≈écÇ∑
		obj_work->obj_3d->draw_state.alpha.alpha = GMD_GMK_SS_RINGGATE_VANISH_ALPHA_RATE;
		obj_work->ppFunc = NULL;
#endif
	}
}
#if 1
// ==========================================================================
// gmGmkSsRingGateDefFunc
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ÉäÉìÉOÉQÅ[Ég ãÈå` Ç≠ÇÁÇ¢èàóù
 *
 *	@param mine_rect	[in] é©ï™Ç≠ÇÁÇ¢ãÈå`
 *	@param match_rect	[in] ëäéËçUåÇãÈå`
 *
 *	@note
 *		ppDefÇ…ìoò^\n
 */
// ==========================================================================
void gmGmkSsRingGateDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
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

#if 1
#if !_IPHONE
	OBS_CAMERA	*obj_camera = ObjCameraGet(g_obj.glb_camera_id);
	if (obj_camera)
#endif // _IPHONE
	{
		fx32	spd_x, spd_y;
		fx32	src_spd_x = FX_Mul(ply_work->obj_work.spd.x, 0x1400);	// x1.25
		fx32	src_spd_y = FX_Mul(ply_work->obj_work.spd.y, 0x1400);	// x1.25
		s16 roll;

#if _IPHONE
		roll = (s16)GmMainGetObjectRotation();
#else
		roll = (s16)-obj_camera->roll;
#endif // _IPHONE
		if (com_work->obj_work.user_flag & GMD_GMK_SS_RINGGATE_EVE_FLAG_V) {
			// ècíuÇ´
			if (roll > 0) {
				roll -= 0x4000;
			} else {
				roll += 0x4000;
			}
		} else {
			// â°íuÇ´
		}
		roll *= 2;
		src_spd_y = 0 - src_spd_y;
		spd_x = FX_Mul(src_spd_x, mtMathCos(roll));
		spd_x += FX_Mul(src_spd_y, mtMathSin(roll));
		spd_y = FX_Mul(src_spd_y, mtMathCos(roll));
		spd_y -= FX_Mul(src_spd_x, mtMathSin(roll));
		ply_work->obj_work.spd.x = spd_x;
		ply_work->obj_work.spd.y = spd_y;
		ply_work->obj_work.spd_m = FX_Mul(-ply_work->obj_work.spd_m, 0x1400);	// x1.25

		// ÉRÉìÉgÉçÅ[ÉâÅ[êUìÆ
		GMM_PAD_VIB_MID_TIME(60);											// 60 Frame
	}
#else
	ply_work->obj_work.spd.x = -ply_work->obj_work.spd.x;					// X/Yë¨ìxîΩì]ÇµÇƒ
	ply_work->obj_work.spd.y = -ply_work->obj_work.spd.y;					//  ÉQÅ[ÉgÇ…ÇÕÇ∂Ç©ÇÍÇΩólÇ…å©ÇπÇÈ
	ply_work->obj_work.spd_m = -ply_work->obj_work.spd_m;					//  
#endif
	ply_work->player_flag &= ~GMD_PLF_USER_MASK;
	ply_work->player_flag |= GMD_PLF_USER1;		// ÉWÉÉÉìÉvÉ{É^Éìñ≥éã
}
#endif
// ==========================================================================
// gmGmkSsRingGateNumMain
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ÉäÉìÉOÉQÅ[Ég êîéöìôï\é¶ï®ÉÅÉCÉì
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsRingGateNumMain(OBS_OBJECT_WORK *obj_work)
{
	OBS_OBJECT_WORK 		*parent_work = obj_work->parent_obj;
	GMS_EFFECT_3DNN_WORK	*efct_work = (GMS_EFFECT_3DNN_WORK*)obj_work;
#if !_IPHONE
	OBS_CAMERA				*obj_camera = ObjCameraGet(g_obj.glb_camera_id);
#endif // !_IPHONE
	s32	ring_num;

	if (parent_work->user_timer < GMD_GMK_SS_RINGGATE_MONO_TIME) {
		// êeÇ™ÉÇÉmÉNÉçÇ…Ç»Ç¡ÇΩÇÁè¡ñ≈
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;								// è¡ñ≈
		return;
	}

	ring_num = (s32)(parent_work->user_work - g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->ring_num);
	ring_num = MTM_MATH_CLIP(ring_num, 0, 99);								// ï\é¶ÇÕ0Å`99ñá
	if (obj_work->user_work == GMD_GMK_SS_NUM_ID_10) {
		// 10ÇÃåÖÇ™
		if (ring_num < 10) {
			// í âﬂâ¬î\ÉäÉìÉOñáêîÇ™10à»â∫Ç»ÇÁîÒï\é¶
			obj_work->disp_flag |= OBD_DISP_NODISP;
			return;
		}
	}

	// êîéöïœçXçƒê›íËÉ`ÉFÉbÉN
	{
		BOOL num_chg = FALSE;
		switch (obj_work->user_work) {
			case GMD_GMK_SS_NUM_ID_1:
				if (obj_work->user_timer != ring_num % 10) {
					// êîéöçXêV
					obj_work->user_timer = ring_num % 10;
					num_chg = TRUE;
				}
				break;
				
			case GMD_GMK_SS_NUM_ID_10:
				if (obj_work->user_timer != ring_num / 10) {
					// êîéöçXêV
					obj_work->user_timer = ring_num / 10;
					num_chg = TRUE;
				}
				break;
				
			case GMD_GMK_SS_NUM_ID_RING:
			default:
				break;
		}
		if (num_chg) {
			// ÉÇÅ[ÉVÉáÉìàÍíUäJï˙
			ObjAction3dNNMotionRelease(obj_work->obj_3d);
			// í èÌÉÇÉfÉãâï˙
			ObjObjectAction3dNNModelReleaseCopy(obj_work);
			// êÿÇËë÷Ç¶
			ObjObjectCopyAction3dNNModel(obj_work,
							&gm_gmk_ss_ringgate_obj_3d_list[IDB_GMK_SS_RINGGATE_MDL_SS_GATE_0F_ZNO + obj_work->user_timer],
							&efct_work->obj_3d);
		}
	}

#if _IPHONE
	obj_work->dir.z = GmMainGetObjectRotation();
#else
	// âÒì]ÇÉJÉÅÉâÇ∆ìØÇ∂Ç≠
	if (obj_camera) {
		obj_work->dir.z  = (u16)-obj_camera->roll;
	}
#endif // _IPHONE

	// êeÇÃìßâﬂÇÉRÉsÅ[
	obj_work->disp_flag |= (parent_work->disp_flag & OBD_DISP_DRAWSTATE);						// ÉÜÅ[ÉUÅ[ï`âÊÉXÉeÅ[ÉgîΩâf
	obj_work->obj_3d->drawflag |= (parent_work->obj_3d->drawflag & NND_DRAWOBJ_MATCTRL_ALPHA);	// ÉAÉãÉtÉ@êßå‰Ç†ÇË
	obj_work->obj_3d->draw_state.alpha.alpha = parent_work->obj_3d->draw_state.alpha.alpha;		// îºìßñæ

	fx32 pos_x = parent_work->pos.x;
	fx32 pos_y = parent_work->pos.y;

	// ç¿ïWçXêV
	switch (obj_work->user_work) {
		case GMD_GMK_SS_NUM_ID_1:
			pos_x += FX_Mul(GMD_GMK_SS_NUM_OFFSET, mtMathCos( obj_work->dir.z ));
			pos_y += FX_Mul(GMD_GMK_SS_NUM_OFFSET, mtMathSin( obj_work->dir.z ));
			break;
			
		case GMD_GMK_SS_NUM_ID_RING:
			pos_x += FX_Mul(-GMD_GMK_SS_NUM_OFFSET, mtMathCos( obj_work->dir.z ));
			pos_y += FX_Mul(-GMD_GMK_SS_NUM_OFFSET, mtMathSin( obj_work->dir.z ));
			break;

		case GMD_GMK_SS_NUM_ID_10:
		default:
			break;
	}
	obj_work->pos.x = pos_x;
	obj_work->pos.y = pos_y;
}

#if GMD_GMK_SS_RINGGATE_TEST_TVX
// ==========================================================================
// gmGmkSsRingGateDrawFunc
/*!
 *	ÉMÉ~ÉbÉN SpecialStage ÉäÉìÉOÉQÅ[Ég ï`âÊ
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsRingGateDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	// ï`âÊÇµÇ»Ç¢ÉtÉåÅ[ÉÄÇÕèIóπ
	if (!GmMainIsDrawEnable()) {
		return;
	}
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}
	
	void* tvx            = amBindGet(gm_gmk_ss_ringgate_obj_tvx_list, 0);
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
	u32 uv_param = (u32)(obj_work->user_flag >> GMD_GMK_SS_RINGGATE_FLAG_UV_WAIT_GET_SHIFT) & GMD_GMK_SS_RINGGATE_FLAG_UV_WAIT_GET_MASK;
	
	work.u_wrap  = NNE_PRIM_TEXWRAP_REPEAT;
	work.v_wrap  = NNE_PRIM_TEXWRAP_REPEAT;
	work.coord.u = -0.25f * (float)(uv_param);
	work.coord.v = 0.00f;
	work.color   = 0xffffffff;
	
	GmTvxSetModelEx(tvx, texlist, pos, scale, disp_flag, rotate, &work);
}
#endif // GMD_GMK_SS_RINGGATE_TEST_TVX


//	// ==========================================================================
//	// _pt
//	/*!
//	 *	@param	tcb	[in]	TCB
//	 */
//	// ==========================================================================
