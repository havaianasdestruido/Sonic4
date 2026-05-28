// ==========================================================================
/*!
  @file gmGmkLand.cpp
  @brief ïÇìá

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkLand.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmGmkLand.cpp,v $
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmPlySeqGmk.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"

#include "gmGmkLand.h"

#if _IPHONE
#include "gmTvx.h"
#endif // _IPHONE

// ÉfÅ[É^ÉwÉbÉ_
#include "common/model/GMK_LAND_1_MDL.HMB"
#include "common/model/GMK_LAND_2_MDL.HMB"
#include "common/model/GMK_LAND_2_MTN.HMB"
#include "common/model/GMK_LAND_2_MAT.HMB"
#include "common/model/GMK_LAND_3_MDL.HMB"
#include "common/model/GMK_LAND_3_MAT.HMB"
#include "common/model/GMK_LAND_4_MDL.HMB"
#include "common/model/GMK_LAND_F_MDL.HMB"
#include "common/model/GMK_LAND_F_MAT.HMB"


//----- Macros --------------------------------------------------------------
/*!
  @defgroup GMD_MATH_ANGLE
  @brief äpìxÇ…ä÷ÇµÇƒÇÃíËã`
  
  äpìxÇÕÇQÉŒ=0x00010000Ç≈àµÇ¡ÇƒÇ¢Ç‹Ç∑ÅB
 */
//@{
/// ÇQÉŒÉâÉWÉAÉìÇÃäpìx
#define GMD_MATH_MAX_ANGLE      (0x00010000)
/// ÇQÉŒÉâÉWÉAÉìÇÃäpìxÇÃÉ}ÉXÉN
#define DMG_MATH_ANGLE_MASK     (MTD_MATH_MAX_ANGLE-1)

#define GMD_GMK_LAND_3_TEST_TVX       (1 & _IPHONE)

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
/* ââèoê›íË */
// óéâ∫É^ÉCÉv
#define GMD_GMK_LAND_FALL_TIME		(30)	//!< ïWèÄóéâ∫äJénéûä‘
// îjâÛÉ^ÉCÉv
#define GMD_GMK_LAND_BREAK_SPD		(8)		//!< ïWèÄîjâÛë¨ìx
#define GMD_GMK_LAND_BREAK_DELAY	(10)	//!< ïWèÄîjâÛÇ∏ÇÍéûä‘

#define GMD_GMK_LAND_COL_OFST_Y		(-18+1)	//(-12+1)	//!< í èÌÉ^ÉCÉvìáínå`ÉIÉtÉZÉbÉgY
#define GMD_GMK_LAND_COL_COL_OFST_Y	(-32+1)	//!< âüÇµÉ^ÉCÉvìáínå`ÉIÉtÉZÉbÉgY

#define GMD_GMK_LAND_BREAK_OAM_NUM_MAX			(9)	//!< îjâÛÉ^ÉCÉvïÇìáÅEè∞OAMégópç≈ëÂêî

#define GMD_GMK_LAND_BREAK_CHIP_NUM_X		(3)					//!< îjâÛéûâ°É`ÉbÉvêî
#define GMD_GMK_LAND_BREAK_CHIP_NUM_Y		(3)					//!< îjâÛéûècÉ`ÉbÉvêî
#define GMD_GMK_LAND_BREAK_CHIP_SIZE_X		(16)				//!< îjâÛéûÉ`ÉbÉvâ°ÉTÉCÉY
#define GMD_GMK_LAND_BREAK_CHIP_SIZE_Y		(8)					//!< îjâÛéûÉ`ÉbÉvècÉTÉCÉY

#define GMD_GMK_LAND_BREAK_ACT_OFST_X		(-24)			//!< îjâÛÉ^ÉCÉv ÉAÉNÉVÉáÉìê›íËéûÉIÉtÉZÉbÉg
#define GMD_GMK_LAND_BREAK_ACT_OFST_Y		(-12)			//!< îjâÛÉ^ÉCÉv ÉAÉNÉVÉáÉìê›íËéûÉIÉtÉZÉbÉg

// èÊÇ¡ÇΩÇÁìÆÇ≠É^ÉCÉv
// obj_work : user_flag
#define GMD_GMK_LAND_RIDE_FLAG		0x10000		//!< ãNìÆÉtÉâÉO
#define GMD_GMK_LAND_RIDE_TIME_MASK	0x003ff		//!< ì∆é©é¸ä˙É^ÉCÉ}Å[É}ÉXÉNíl

// GMS_EVE_RECORD_EVENT : flag
//#define GMD_GMK__EVE_FLAG_		(0x0001)	//!< 
#define GMD_GMK_LAND_EVE_FLAG_SPEED_MASK		(0x0003)	//!< ë¨ìxê›íËéÊìæópÉ}ÉXÉN
#define GMD_GMK_LAND_EVE_FLAG_RIDE_START		(0x0004)	//!< èÊÇ¡ÇΩÇÁìÆÇ´èoÇ∑ÅiÉ^ÉCÉ}Å[îÒìØä˙à⁄ìÆÅj
#define GMD_GMK_LAND_EVE_FLAG_LEFT_UP			(0x0008)	//!< ç∂è„Ç©ÇÁâEâ∫Ç÷à⁄ìÆÅiSQUARE_MOVE ON ÇÃèÍçáîΩéûåvâÒÇËÅj
#define GMD_GMK_LAND_EVE_FLAG_TIMING_MASK		(0x0030)	//!< à⁄ìÆÉ^ÉCÉ~ÉìÉOéÊìæópÉ}ÉXÉN
#define GMD_GMK_LAND_EVE_FLAG_TIMING_SHIFT		(4)			//!< à⁄ìÆÉ^ÉCÉ~ÉìÉOópÉVÉtÉgíl
#define GMD_GMK_LAND_EVE_FLAG_FALL				(0x0040)	//!< èÊÇÈÇ∆óéâ∫
#define GMD_GMK_LAND_EVE_FLAG_NO_THROUGH		(0x0080)	//!< Ç∑ÇËî≤ÇØïsâ¬


// enemy_flag
#define GMD_GMK_LAND_ENEMY_FLAG_ON_OBJ			(0x0001)	//!< è„Ç…ÉIÉuÉWÉFÉNÉgÇ™èÊÇ¡ÇΩÉtÉâÉO

/// ìáÉ^ÉCÉv
enum {
	GMD_GMK_LAND_TYPE_NORMAL	= 0,		// í èÌÉ^ÉCÉv
	GMD_GMK_LAND_TYPE_BIG,					// ëÂÉ^ÉCÉv
	GMD_GMK_LAND_TYPE_COL,					// âüÇµÉ^ÉCÉv

	GMD_GMK_LAND_TYPE_MAX
};

// forÅuZone3 RopeÅv

// GMS_EVE_RECORD_EVENT : flag
#define GMD_GMK_Z3LAND_ROPE_EVE_FLAG_REV	(0x0001)	//!< ï˚å¸ÉäÉoÅ[ÉXÉtÉâÉO

#if GMD_GMK_LAND_3_TEST_TVX
#define GMD_GMK_LAND_3_ROPE_MOTION (120) // ÉÇÅ[ÉVÉáÉìêî
#endif // GMD_GMK_LAND_3_TEST_TVX

//#define GMD_GMK_Z3LAND_ROPE_DEBUG_POS_DISP			//!< óLå¯Ç≈ÅAäeÉçÅ[ÉvÇÃÉAÉjÉÅÅ[ÉVÉáÉìêiçsìxÉfÉoÉbÉOï\é¶
//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkLandMoveInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkLandMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkLandMove(OBS_OBJECT_WORK *obj_work);
static void gmGmkLandColFall(OBS_OBJECT_WORK *obj_work);
static void gmGmkZ3LandPulleyMain(OBS_OBJECT_WORK *obj_work);
#if GMD_GMK_LAND_3_TEST_TVX
static void gmGmkLand3TvxRopeMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkLand3TvxDrawFunc(OBS_OBJECT_WORK *obj_work);
static void gmGmkLand3TvxRDrawFunc(OBS_OBJECT_WORK *obj_work);
static void gmGmkLand3TvxPulleyDrawFunc(OBS_OBJECT_WORK *obj_work);
static void gmGmkLand3TvxRopeDrawFunc(OBS_OBJECT_WORK *obj_work);
static void gmGmkLand3TvxDrawFuncEx(u32 tvx_index, NNS_TEXLIST *texlist, VecFx32 *pos, VecFx32 *scale, u32 disp_flag, Angle16 dir_z, NNS_TEXCOORD* uv);
#endif // GMD_GMK_SS_CIRCLE_TEST_TVX

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_land_obj_3d_list = NULL;
//static s32 gm_land_synchro_time = 0;		//!< ìØä˙É^ÉCÉvìáìØä˙ópÉ^ÉCÉ}
//static s16 gm_land_synchro_num = 0;			//!< ìØä˙É^ÉCÉvìáë∂ç›êî

static const u8	gm_gmk_land_spd_tbl[4] = {
	4,		// ïÅí 
	2,		// íxÇ¢
	3,		// è≠ÇµíxÇ¢
	5,		// ë¨Ç¢
};

// ZoneñàÇÃobjÉfÅ[É^íËã`
static const s32 gm_gmk_land_obj_data[GSD_MAIN_ZONE_TYPE_MAX][2] = {
	{ GMD_DWORK_NO_GMK_LAND_1_MODEL, GMD_DWORK_NO_GMK_LAND_1_TEX, },	// zone1
	{ GMD_DWORK_NO_GMK_LAND_2_MODEL, GMD_DWORK_NO_GMK_LAND_2_TEX, },	// zone2
	{ GMD_DWORK_NO_GMK_LAND_3_MODEL, GMD_DWORK_NO_GMK_LAND_3_TEX, },	// zone3
	{ GMD_DWORK_NO_GMK_LAND_4_MODEL, GMD_DWORK_NO_GMK_LAND_4_TEX, },	// zone4
	{ GMD_DWORK_NO_GMK_LAND_F_MODEL, GMD_DWORK_NO_GMK_LAND_F_TEX, },	// zoneFinal
	{ 0, 							 0,							  },	// SpecialStage
};

// ZoneñàÇÃMODELÉfÅ[É^íËã`
static const s32 gm_gmk_land_mdl_data[GSD_MAIN_ZONE_TYPE_MAX][3] = {
	{ IDB_GMK_LAND_1_MDL_GMK_LAND_1_ZNO, IDB_GMK_LAND_1_MDL_GMK_LAND_B_1_ZNO, IDB_GMK_LAND_1_MDL_GMK_LAND_1_ZNO,   }, // zone1
	{ IDB_GMK_LAND_2_MDL_GMK_LAND_2_ZNO, IDB_GMK_LAND_2_MDL_GMK_LAND_B_2_ZNO, IDB_GMK_LAND_2_MDL_GMK_LAND_R_2_ZNO, }, // zone2
	{ IDB_GMK_LAND_3_MDL_GMK_LAND_3_ZNO, IDB_GMK_LAND_3_MDL_GMK_LAND_3_ZNO,   IDB_GMK_LAND_3_MDL_GMK_LAND_R_3_ZNO, }, // zone3
	{ IDB_GMK_LAND_4_MDL_GMK_LAND_4_ZNO, IDB_GMK_LAND_4_MDL_GMK_LAND_B_4_ZNO, IDB_GMK_LAND_4_MDL_GMK_LAND_4_ZNO,   }, // zone4
	{ IDB_GMK_LAND_F_MDL_GMK_LAND_F_ZNO, IDB_GMK_LAND_F_MDL_GMK_LAND_F_ZNO,   IDB_GMK_LAND_F_MDL_GMK_LAND_F_ZNO,   }, // zoneFinal
	{ 0,								 0,									  0,								   }, // SpecialStage
};

///ínå`É^ÉCÉv
u8 gm_gmk_land_col_type_tbl[GMD_GMK_LAND_TYPE_MAX] = {
	0,  						  	// í èÌÉ^ÉCÉv
	1,  						  	// ëÂÉ^ÉCÉv
	2,  						  	// âüÇµÉ^ÉCÉv
};

#ifdef GMD_GMK_Z3LAND_ROPE_DEBUG_POS_DISP
static u32 gm_gmk_z3land_rope_debug_count = 0;
#endif//GMD_GMK_Z3LAND_ROPE_DEBUG_POS_DISP

#if GMD_GMK_LAND_3_TEST_TVX
static AMS_AMB_HEADER* gm_gmk_land_3_obj_tvx_list = NULL;
#endif // GMD_GMK_LAND_3_TEST_TVX

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkLandBuild
/*!
 *	ÉMÉ~ÉbÉN ïÇìá ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkLandBuild(void)
{
	GSE_MAIN_ZONE_TYPE	zone_id = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];

	gm_gmk_land_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(gm_gmk_land_obj_data[zone_id][0]),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(gm_gmk_land_obj_data[zone_id][1]),
								0/*draw_flag*/);
#if GMD_GMK_LAND_3_TEST_TVX
	if (zone_id == GSD_MAIN_ZONE_TYPE_3) {
		void* tvx = GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_LAND_3_TVX );
		amBindConv((Uint8*)tvx);
		gm_gmk_land_3_obj_tvx_list = (AMS_AMB_HEADER*)tvx;
	}
#endif // GMD_GMK_LAND_3_TEST_TVX
}

// ==========================================================================
// GmGmkLandFlush
/*!
 *	ÉMÉ~ÉbÉN ïÇìá ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkLandFlush(void)
{
	GSE_MAIN_ZONE_TYPE	zone_id = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(gm_gmk_land_obj_data[zone_id][0]);

	GmGameDBuildRegFlushModel(gm_gmk_land_obj_3d_list, amb->file_num);
#if GMD_GMK_SS_CIRCLE_TEST_TVX
	gm_gmk_land_3_obj_tvx_list = NULL;
#endif // GMD_GMK_SS_CIRCLE_TEST_TVX
}

// ==========================================================================
// GmGmkLandInit
/*!
 *	ÉMÉ~ÉbÉN ïÇìá èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkLandInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;
	s32					land_type;
	u16					obj_type;
	s32					mdl_type;
	GSE_MAIN_ZONE_TYPE	zone_id = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_RIDE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_LAND");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ìáÉ^ÉCÉvéÊìæ
	if (eve_rec->id == GMD_EVENT_ID_LAND_BIG) {
		// ïÇìá(ëÂ)
		land_type = GMD_GMK_LAND_TYPE_BIG;
		obj_type = IDB_GMK_LAND_2_MDL_GMK_LAND_B_2_ZNO;
	} else if (eve_rec->id == GMD_EVENT_ID_LAND_COL) {
		// ïÇìá(ìñÇΩÇË)
		land_type = GMD_GMK_LAND_TYPE_COL;
		obj_type = IDB_GMK_LAND_2_MDL_GMK_LAND_R_2_ZNO;
	} else {
		// ïÇìá(í èÌ)
		land_type = GMD_GMK_LAND_TYPE_NORMAL;
		obj_type = IDB_GMK_LAND_2_MDL_GMK_LAND_2_ZNO;
	}

	mdl_type = gm_gmk_land_mdl_data[zone_id][obj_type];

	// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_land_obj_3d_list[mdl_type],
					&gmk_work->obj_3d);

	if (zone_id == GSD_MAIN_ZONE_TYPE_2) {
		// Zone2ÇÃÇ›ÉÇÅ[ÉVÉáÉìÅ{É}ÉeÉäÉAÉãÉAÉjÉÅê›íË
		s32 mtn_type = mdl_type + IDB_GMK_LAND_2_MTN_GMK_LAND_2_ZNM;
		s32	mat_type = mdl_type + IDB_GMK_LAND_2_MAT_GMK_LAND_2_ZNV;
		// ÉÇÅ[ÉVÉáÉìèâä˙âª
		ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, FALSE/*marge*/,
										ObjDataGet(GMD_DWORK_NO_GMK_LAND_2_MTN), NULL/*mtn_data_path*/,
										0/*index*/, NULL/*archive*/);

		// ÉAÉNÉVÉáÉìê›íË
		ObjDrawObjectActionSet(obj_work, mtn_type);

		// É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
		ObjAction3dNNMaterialMotionLoad( &gmk_work->obj_3d,
		                                 0,				//reg_file_id
		                                 NULL,			//data_work
		                                 NULL,			//mtn_data_path
		                                 mat_type,
		                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_LAND_2_MAT)->pData );
		ObjDrawObjectActionSet3DNNMaterial( obj_work, 0 );

		obj_work->disp_flag |= OBD_DISP_REPEAT;
	} else if (zone_id == GSD_MAIN_ZONE_TYPE_FINAL) {
		// ZoneFinalÇÕÉ}ÉeÉäÉAÉãÉAÉjÉÅê›íË
		s32	mat_type = mdl_type + IDB_GMK_LAND_F_MAT_GMK_LAND_F_ZNV;
		// É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
		ObjAction3dNNMaterialMotionLoad( &gmk_work->obj_3d,
		                                 0,				//reg_file_id
		                                 NULL,			//data_work
		                                 NULL,			//mtn_data_path
		                                 mat_type,
		                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_LAND_F_MAT)->pData );
		ObjDrawObjectActionSet3DNNMaterial( obj_work, 0 );

		obj_work->disp_flag |= OBD_DISP_REPEAT;
	}
#if _WII
	else if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_1 ||
			g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_2) {
		// ÉâÉCÉgê›íË
		gmk_work->obj_3d.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
		gmk_work->obj_3d.use_light_flag |= OBD_LIGHT_USE_FLAG_5;
	}
#endif // _WII
#if _IPHONE
	else if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3 ||
			 g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_BOSS) {
		// ÉâÉCÉgê›íË
		gmk_work->obj_3d.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
		gmk_work->obj_3d.use_light_flag |= OBD_LIGHT_USE_FLAG_1;
	}
#if GMD_GMK_LAND_3_TEST_TVX
	if (zone_id == GSD_MAIN_ZONE_TYPE_3) {
		// TVXèoóÕ
		if (mdl_type == IDB_GMK_LAND_3_MDL_GMK_LAND_3_ZNO) {
			obj_work->ppOut = gmGmkLand3TvxDrawFunc;
		}
		else {
			obj_work->ppOut = gmGmkLand3TvxRDrawFunc;
		}
	}
#endif // GMD_GMK_LAND_3_TEST_TVX
#endif // _IPHONE


	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;


	// ínå`ê›íË
	gmk_work->ene_com.col_work.obj_col.obj			= obj_work;

// diff_dataÇÉZÉbÉgÇ∑ÇÈÇ±Ç∆Ç≈ pixelíPà Ç≈ÇÃÉ`ÉFÉbÉNÇ∆Ç»ÇÈÅBÇ∑ÇËî≤ÇØÇ…Ç‡ëŒâûÅB
// g_gm_default_col ÇégópÇ∑ÇÈèÍçáÇÕãÈå`ÉTÉCÉYÇ…8dotíPà ÇÃêßå¿Ç™ïtÇ´Ç‹Ç∑ÅB
	gmk_work->ene_com.col_work.obj_col.diff_data	= (s8*)g_gm_default_col;	// äÓñ{ínå`èÓïÒ
	gmk_work->ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NOFREE_DIFF_DATA;		// diff_dataÇäJï˙ÇµÇ»Ç¢

	if (!(eve_rec->flag & GMD_GMK_LAND_EVE_FLAG_NO_THROUGH) &&
			eve_rec->id != GMD_EVENT_ID_LAND_COL) {
		// Ç∑ÇËî≤ÇØëÆê´ê›íË
		gmk_work->ene_com.col_work.obj_col.attr = OBD_COL_DATA_ATTR_THROUGH;
	}

	switch (gm_gmk_land_col_type_tbl[land_type]) {
		default:
		case 0:
			// í èÌÉ^ÉCÉv
			gmk_work->ene_com.col_work.obj_col.width		= 6*8;								// ínå`ÉTÉCÉYê›íË(ÉhÉbÉg)ÇWÉhÉbÉgíPà å¿íË
			gmk_work->ene_com.col_work.obj_col.height		= 3*8;
			gmk_work->ene_com.col_work.obj_col.ofst_x		= (s16)(-gmk_work->ene_com.col_work.obj_col.width /2);
			gmk_work->ene_com.col_work.obj_col.ofst_y		= GMD_GMK_LAND_COL_OFST_Y;
			if (gmk_work->ene_com.col_work.obj_col.attr & OBD_COL_DATA_ATTR_THROUGH) {
				// Ç∑ÇËî≤ÇØëŒâû
				gmk_work->ene_com.col_work.obj_col.height	= 1*8;
//				gmk_work->ene_com.col_work.obj_col.ofst_y	+= 4;
				gmk_work->ene_com.col_work.obj_col.ofst_y	+= 1;
			}
			break;
		case 1:
			// ëÂÉ^ÉCÉv
			gmk_work->ene_com.col_work.obj_col.width		= 10*8;								// ínå`ÉTÉCÉYê›íË(ÉhÉbÉg)ÇWÉhÉbÉgíPà å¿íË
			gmk_work->ene_com.col_work.obj_col.height		= 3*8;
			gmk_work->ene_com.col_work.obj_col.ofst_x		= (s16)(-gmk_work->ene_com.col_work.obj_col.width /2);
			gmk_work->ene_com.col_work.obj_col.ofst_y		= GMD_GMK_LAND_COL_OFST_Y;
			if (gmk_work->ene_com.col_work.obj_col.attr & OBD_COL_DATA_ATTR_THROUGH) {
				// Ç∑ÇËî≤ÇØëŒâû
				gmk_work->ene_com.col_work.obj_col.height	= 1*8;
//				gmk_work->ene_com.col_work.obj_col.ofst_y	+= 4;
				gmk_work->ene_com.col_work.obj_col.ofst_y	+= 1;
			}
			break;
		case 2:
			// ìñÇΩÇËÉ^ÉCÉv
			if (zone_id != GSD_MAIN_ZONE_TYPE_3) {
				gmk_work->ene_com.col_work.obj_col.width		= 8*8;							// ínå`ÉTÉCÉYê›íË(ÉhÉbÉg)ÇWÉhÉbÉgíPà å¿íË
				gmk_work->ene_com.col_work.obj_col.height		= 8*8;
				gmk_work->ene_com.col_work.obj_col.ofst_x		= (s16)(-gmk_work->ene_com.col_work.obj_col.width /2);
				gmk_work->ene_com.col_work.obj_col.ofst_y		= GMD_GMK_LAND_COL_COL_OFST_Y;
			} else {
				// Zone3ÇÃÇ›îºï™ÇÃÉTÉCÉY
				gmk_work->ene_com.col_work.obj_col.width		= 3*8;							// ínå`ÉTÉCÉYê›íË(ÉhÉbÉg)ÇWÉhÉbÉgíPà å¿íË	Å¶4*8ÇæÇ∆ínñ Ç…à¯Ç¡Ç©Ç©ÇÈ@HOGzone3
				gmk_work->ene_com.col_work.obj_col.height		= 4*8;
				gmk_work->ene_com.col_work.obj_col.ofst_x		= (s16)(-gmk_work->ene_com.col_work.obj_col.width /2);
				gmk_work->ene_com.col_work.obj_col.ofst_y		= GMD_GMK_LAND_COL_COL_OFST_Y /2;
			}
			obj_work->field_rect[MTD_LEFT]	 = (s16)(gmk_work->ene_com.col_work.obj_col.ofst_x);
			obj_work->field_rect[MTD_TOP]	 = (s16)(gmk_work->ene_com.col_work.obj_col.ofst_y);
			obj_work->field_rect[MTD_RIGHT]	 = (s16)(gmk_work->ene_com.col_work.obj_col.ofst_x + gmk_work->ene_com.col_work.obj_col.width);
			obj_work->field_rect[MTD_BOTTOM] = (s16)(gmk_work->ene_com.col_work.obj_col.ofst_y + gmk_work->ene_com.col_work.obj_col.height);

			break;
	}

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ínå`Ç†ÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->flag |= OBD_OBJECT_NOHIT;							// ãÈå`Ç†ÇΩÇËñ≥ÇµÅü

	gmGmkLandMoveInit(obj_work);

	return (obj_work);
}


// ==========================================================================
// GmGmkZ3LandPulleyInit
/*!
 *	ÉMÉ~ÉbÉN ïÇìá ïtêèääé‘ èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkZ3LandPulleyInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;
	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_RIDE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_LAND_PULLEY");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_land_obj_3d_list[IDB_GMK_LAND_3_MDL_LAND_3_GEAR_ZNO],
					&gmk_work->obj_3d);

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK - (8 * FX32_ONE);

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ínå`Ç†ÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->flag |= OBD_OBJECT_NOHIT;							// ãÈå`Ç†ÇΩÇËñ≥ÇµÅü

	obj_work->user_work = (u32)((s16)(eve_rec->left << 8) / 10);	// âÒì]ë¨ìx( /10 ÇÕë¨ìxî˜í≤êÆópÇ…Åj
#if 0
	obj_work->ppFunc = gmGmkZ3LandPulleyMain;
#endif // 0
#if GMD_GMK_LAND_3_TEST_TVX
	// TVXèoóÕ
	obj_work->ppOut = gmGmkLand3TvxPulleyDrawFunc;
#endif // GMD_GMK_LAND_3_TEST_TVX

	return (obj_work);
}

// ==========================================================================
// GmGmkZ3LandRopeInit
/*!
 *	ÉMÉ~ÉbÉN ïÇìá ïtêèÉçÅ[Év èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
#ifdef GMD_GMK_Z3LAND_ROPE_DEBUG_POS_DISP
void gmGmkZ3LandRopeMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	
	amPrintf(40, obj_work->user_work + 1, "mat %f", gmk_work->obj_3d.mat_frame);
}
#endif//GMD_GMK_Z3LAND_ROPE_DEBUG_POS_DISP

OBS_OBJECT_WORK* GmGmkZ3LandRopeInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;
	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_RIDE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_LAND_ROPE");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_land_obj_3d_list[IDB_GMK_LAND_3_MDL_LAND_3_ROPE_ZNO],
					&gmk_work->obj_3d);
#if !GMD_GMK_LAND_3_TEST_TVX
	// É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
	ObjAction3dNNMaterialMotionLoad( &gmk_work->obj_3d,
	                                 0,				//reg_file_id
	                                 NULL,			//data_work
	                                 NULL,			//mtn_data_path
	                                 IDB_GMK_LAND_3_MAT_LAND_3_ROPE_ZNV,
	                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_LAND_3_ROPE_MAT)->pData );
	ObjDrawObjectActionSet3DNNMaterial( obj_work, 0 );
	obj_work->disp_flag |= OBD_DISP_REPEAT;
#endif // !GMD_GMK_LAND_3_TEST_TVX
	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK - (16 * FX32_ONE);

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ínå`Ç†ÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->flag |= OBD_OBJECT_NOHIT;							// ãÈå`Ç†ÇΩÇËñ≥ÇµÅü

	// âÒì]ï˚å¸ê›íË
	if (eve_rec->id == GMD_EVENT_ID_GMK_Z3LAND_ROPE_H) {
		obj_work->dir.z = 0xC000;
	}
	if (eve_rec->flag & GMD_GMK_Z3LAND_ROPE_EVE_FLAG_REV) {
		obj_work->dir.z += 0x8000;
	}
	// É}ÉeÉäÉAÉãÉAÉjÉÅÅ[ÉVÉáÉìë¨ìxê›íË
	if (eve_rec->left) {
		gmk_work->obj_3d.mat_speed = (float)(eve_rec->left) / 10.0f;	// ÉAÉjÉÅë¨ìxÅi / 10.0fÇÕë¨ìxî˜í≤êÆópÇ…Åj
	}
	
#if !GMD_GMK_LAND_3_TEST_TVX
	// É}ÉeÉäÉAÉãÉAÉjÉÅÅ[ÉVÉáÉìÇÃìØä˙Ç†ÇÌÇπ
	{
		float	frame_max, calc_temp;
		frame_max = amMotionMaterialGetEndFrame(gmk_work->obj_3d.motion, gmk_work->obj_3d.mat_act_id) -
							amMotionMaterialGetStartFrame(gmk_work->obj_3d.motion, gmk_work->obj_3d.mat_act_id);
		calc_temp = (float)(g_gm_main_system.sync_time) / (frame_max / gmk_work->obj_3d.mat_speed);
		gmk_work->obj_3d.mat_frame = (calc_temp - (int)(calc_temp)) * frame_max;
	}
#endif // !GMD_GMK_LAND_3_TEST_TVX
#ifdef GMD_GMK_Z3LAND_ROPE_DEBUG_POS_DISP
	// debug_disp
	obj_work->ppFunc = gmGmkZ3LandRopeMain;
	obj_work->user_work = gm_gmk_z3land_rope_debug_count++;
#endif//GMD_GMK_Z3LAND_ROPE_DEBUG_POS_DISP

#if GMD_GMK_LAND_3_TEST_TVX
	// ÉÇÅ[ÉVÉáÉìçXêV
	obj_work->ppFunc = gmGmkLand3TvxRopeMain;
	float frame_max = (float)GMD_GMK_LAND_3_ROPE_MOTION;
	float calc_temp = (float)(g_gm_main_system.sync_time) / (frame_max / gmk_work->obj_3d.mat_speed);
	gmk_work->obj_3d.mat_frame = (calc_temp - (int)(calc_temp)) * frame_max;
	// TVXèoóÕ
	obj_work->ppOut = gmGmkLand3TvxRopeDrawFunc;
#endif // GMD_GMK_LAND_3_TEST_TVX

	return (obj_work);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkLandMoveInit
/*!
 *	ÉMÉ~ÉbÉN ïÇìá à⁄ìÆèâä˙âªèàóù
 *
 *	@param	gmk_work	[in]	ÉGÉlÉ~Å[ÉèÅ[ÉN
 *
 *	@note
 *		prev_pos	: íÜêSà íuï€ë∂ \n
 *		user_timer	: à⁄ìÆÉ^ÉCÉ~ÉìÉO
 */
// ==========================================================================
void gmGmkLandMoveInit(OBS_OBJECT_WORK *obj_work)
{
	fx32				width;	// ç≈ëÂà⁄ìÆîÕàÕ
	fx32				pos;	// èâä˙à íu
	fx32				center;	// à⁄ìÆîÕàÕíÜêSà íu
	fx32				pos_temp;
	u16					cnt;

	GMS_ENEMY_3D_WORK	*gmk_work;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// íÜêSéÊìæ
	obj_work->prev_pos.x = ((obj_work->pos.x >> FX32_SHIFT) + (gmk_work->ene_com.eve_rec->left)) + (gmk_work->ene_com.eve_rec->width >> 1);
	obj_work->prev_pos.y = ((obj_work->pos.y >> FX32_SHIFT) + (gmk_work->ene_com.eve_rec->top)) + (gmk_work->ene_com.eve_rec->height >> 1);

	// ÉÅÉCÉìèàóù
	obj_work->ppFunc = gmGmkLandMain;

	// à⁄ìÆñ≥Çµ
	if (!(gmk_work->ene_com.eve_rec->width | gmk_work->ene_com.eve_rec->height)) {
		return;
	}

	if (!(gmk_work->ene_com.eve_rec->id == GMD_EVENT_ID_LAND_AROUND)) {
		// í èÌà⁄ìÆÉ^ÉCÉv
		// ècorâ° ÉTÉCÉYÇÃëÂÇ´Ç¢ï˚Ç…çáÇÌÇπÇƒê›íË
		if (gmk_work->ene_com.eve_rec->height < gmk_work->ene_com.eve_rec->width) {
			// â°
			width	= gmk_work->ene_com.eve_rec->width >> 1;
			pos		= obj_work->pos.x >> FX32_SHIFT;
			center	= obj_work->prev_pos.x;
		}
		else {
			// èc
			width	= gmk_work->ene_com.eve_rec->height >> 1;
			pos		= obj_work->pos.y >> FX32_SHIFT;
			center	= obj_work->prev_pos.y;
		}

		// èâä˙ÉJÉEÉìÉgåvéZ
		if (!(gmk_work->ene_com.eve_rec->flag & GMD_GMK_LAND_EVE_FLAG_RIDE_START)) {
			// í èÌ
			for (cnt = 0x300; cnt > 0x100; cnt -= 4) {
				pos_temp = center + ((width * mtMathSin((u16)(cnt << 6))) >> 12);
				if (pos_temp > pos) {
					break;
				}
			}
			obj_work->user_timer = cnt;
		} else {
			// èÊÇ¡ÇΩÇÁìÆÇ≠
			obj_work->user_timer = 0;
			obj_work->user_flag  = 0;								// ÉâÉCÉhãNìÆÉtÉâÉOåìÉ^ÉCÉ}Å[
		}
		// à ëä
		{
			s16	phase =
				(s16)(((gmk_work->ene_com.eve_rec->flag & GMD_GMK_LAND_EVE_FLAG_TIMING_MASK) >> GMD_GMK_LAND_EVE_FLAG_TIMING_SHIFT) << 8);
			obj_work->user_timer -= phase;
			obj_work->user_timer &= 0x00003FFF;
		}
	} else {
		// ãÈå`ÇÇ»ÇºÇÈà⁄ìÆÉ^ÉCÉv
		s16	left = (s16)(gmk_work->ene_com.eve_rec->left * 2);		// ãÈå`ç∂
		s16	top  = (s16)(gmk_work->ene_com.eve_rec->top * 2);		// ãÈå`è„
		s16	width = (s16)(gmk_work->ene_com.eve_rec->width * 2);	// ãÈå`ïù
		s16	height = (s16)(gmk_work->ene_com.eve_rec->height * 2);	// ãÈå`çÇÇ≥
		s32	periphery = (s32)(width * 2 + height * 2);				// äOé¸ãóó£

		if (top == 0) {
			// è„ï”
			obj_work->user_timer = MTM_MATH_ABS(left) * 0x1000 / periphery;
		} else if (left == 0) {
			// ç∂ï”
			obj_work->user_timer = (periphery - MTM_MATH_ABS(top)) * 0x1000 / periphery;
		} else if ((left + width) == 0) {
			// âEï”
			obj_work->user_timer = (width + MTM_MATH_ABS(top)) * 0x1000 / periphery;
		} else {
			// â∫ï”
			obj_work->user_timer = (periphery - height - MTM_MATH_ABS(left)) * 0x1000 / periphery;
		}

		obj_work->view_out_ofst += 256;								// éÄñSîÕàÕägëÂ
	}

}


// ==========================================================================
// gmGmkLandMain
/*!
 *	ÉMÉ~ÉbÉN ïÇìá í èÌÉÅÉCÉìèàóù
 *
 *	@param	gmk_work	[in]	ÉGÉlÉ~Å[ÉèÅ[ÉN
 *
 *	@note
 *		prev_pos	: íÜêSà íuï€ë∂ \n
 *		user_timer	: à⁄ìÆÉ^ÉCÉ~ÉìÉO \n
 *		user_work	: óéâ∫É^ÉCÉ}
 */
// ==========================================================================
void gmGmkLandMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	OBS_COLLISION_OBJ	*obj_col = &obj_work->col_work->obj_col;

	if (obj_work->user_work < GMD_GMK_LAND_FALL_TIME) {
		// à⁄ìÆèàóù
		gmGmkLandMove(obj_work);
	}

//	obj_work->dir.x = (u16)(-0x0200);							// èÊÇ¡ÇƒÇ»Ç¢Ç∆Ç´ÅAÇŸÇÒÇÃÇËëOåXÇ´
	// è„Ç…ÉIÉuÉWÉFÉNÉgÇ™èÊÇ¡ÇΩéûÇÃíæÇ›çûÇ›
	if (obj_col->rider_obj) {
		if (obj_col->rider_obj->ride_obj == obj_work) {
			gmk_work->ene_com.enemy_flag |= GMD_GMK_LAND_ENEMY_FLAG_ON_OBJ;

			if (obj_work->disp_flag & OBD_DISP_VFLIP) {
				// ãtèdóÕèÛë‘
				obj_work->ofst.y = -1*FX32_ONE;
			}
			else {
				obj_work->ofst.y = 1*FX32_ONE;
			}
//			obj_work->dir.x = (u16)(-0x0400);					// èÊÇ¡ÇƒÇÈÇ∆Ç´ÅAÇ‡Ç§è≠ÇµëOåXÇ´
		}
	}

	// ìãèÊÉ`ÉFÉbÉN
	if (gmk_work->ene_com.enemy_flag & GMD_GMK_LAND_ENEMY_FLAG_ON_OBJ) {
		// óéâ∫É^ÉCÉvÇÃÇ›óéâ∫É`ÉFÉbÉN
		if (gmk_work->ene_com.eve_rec->flag & GMD_GMK_LAND_EVE_FLAG_FALL) {
			obj_work->user_work++;
			if (obj_work->user_work == GMD_GMK_LAND_FALL_TIME) {
				// óéâ∫äJén
				obj_work->move_flag &= ~OBD_MOVE_NOMOVE;
				obj_work->move_flag |= OBD_MOVE_FALL;
				obj_work->prev_pos.x = obj_work->pos.x;
				obj_work->prev_pos.y = obj_work->pos.y;
				obj_work->spd_fall_max	= (GMD_OBJ_DEF_FALL_SPDMA / 2);	// óéâ∫ç≈ëÂë¨ìx

				if (gmk_work->ene_com.eve_rec->id == GMD_EVENT_ID_LAND_COL) {
					// ìñÇΩÇËÇ†ÇËïÇìáÇ»ÇÁínå`Ç≈é~Ç‹ÇÈ
					obj_work->move_flag &= ~OBD_MOVE_NOCOL;		// ínå`Ç†ÇΩÇËÇ†ÇË
					obj_work->move_flag |= OBD_MOVE_NOCOL_W;	// â°ÇÕîªíËÇµÇ»Ç¢
					obj_work->ppFunc = gmGmkLandColFall;
//					obj_work->ppFunc = NULL;
				} else {
					// í èÌÅFÉÅÉCÉìèàóùÉNÉäÉA
					obj_work->ppFunc = NULL;
				}
			}
		}
		// èÊÇ¡ÇΩÇÁìÆÇ´èoÇ∑É^ÉCÉv
		if (gmk_work->ene_com.eve_rec->flag & GMD_GMK_LAND_EVE_FLAG_RIDE_START) {
			// èÊÇ¡ÇΩÉtÉâÉOÉZÉbÉg
			obj_work->user_flag |= GMD_GMK_LAND_RIDE_FLAG;
		}
	}
}
// ==========================================================================
// gmGmkLandMove
/*!
 *	ÉMÉ~ÉbÉN ïÇìá à⁄ìÆèàóù
 *
 *	@param	gmk_work	[in]	ÉGÉlÉ~Å[ÉèÅ[ÉN
 *
 *	@note
 *		prev_pos	: íÜêSà íuï€ë∂ \n
 *		user_timer	: à⁄ìÆÉ^ÉCÉ~ÉìÉO
 */
// ==========================================================================
void gmGmkLandMove(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	fx32				pos_x, pos_y;
	// äeê›íË
	u8					spd = gm_gmk_land_spd_tbl[(u8)(gmk_work->ene_com.eve_rec->flag & GMD_GMK_LAND_EVE_FLAG_SPEED_MASK)];
	u16					cnt = (u16)obj_work->user_timer;


	if (!(gmk_work->ene_com.eve_rec->id == GMD_EVENT_ID_LAND_AROUND)) {
		// í èÌà⁄ìÆÉ^ÉCÉv
		// à íuÉJÉEÉìÉgÇåvéZ
		s32					center_x = obj_work->prev_pos.x;
		s32					center_y = obj_work->prev_pos.y;
		s16					width = (s16)(gmk_work->ene_com.eve_rec->width >> 1);
		s16					height = (s16)(gmk_work->ene_com.eve_rec->height >> 1);

		if (!(gmk_work->ene_com.eve_rec->flag & GMD_GMK_LAND_EVE_FLAG_RIDE_START)) {
			// í èÌ
			cnt = (u16)(((g_gm_main_system.sync_time * spd) + cnt) & 0x03FF);
		} else {
			// èÊÇ¡ÇΩÇÁìÆÇ´èoÇ∑
			if (!obj_work->user_flag) {
				// ñ¢ìãèÊ
				cnt = (u16)(obj_work->user_timer & 0x03FF);
			} else {
				// ìãèÊçœÇ›
				cnt = (u16)(((spd * (obj_work->user_flag & GMD_GMK_LAND_RIDE_TIME_MASK)) + cnt) & 0x03FF);
				obj_work->user_flag = (u32)(  (obj_work->user_flag & GMD_GMK_LAND_RIDE_FLAG)
											|((obj_work->user_flag + 1) & GMD_GMK_LAND_RIDE_TIME_MASK) );
			}
		}
		if (gmk_work->ene_com.eve_rec->flag & GMD_GMK_LAND_EVE_FLAG_LEFT_UP) {
			pos_x = (center_x << FX32_SHIFT) + (width  * mtMathSin((u16)((cnt << 6) + GMD_MATH_MAX_ANGLE/2)));
			pos_y = (center_y << FX32_SHIFT) + (height * mtMathSin((u16)(cnt << 6)));
		}
		else {
			pos_x = (center_x << FX32_SHIFT) + (width  * mtMathSin((u16)(cnt << 6)));
			pos_y = (center_y << FX32_SHIFT) + (height * mtMathSin((u16)(cnt << 6)));
		}
	} else {
		// ãÈå`ÇÇ»ÇºÇÈà⁄ìÆÉ^ÉCÉv
		s16	left = (s16)(gmk_work->ene_com.eve_rec->left * 2);		// ãÈå`ç∂
		s16	top  = (s16)(gmk_work->ene_com.eve_rec->top * 2);		// ãÈå`è„
		s16	width = (s16)(gmk_work->ene_com.eve_rec->width * 2);	// ãÈå`ïù
		s16	height = (s16)(gmk_work->ene_com.eve_rec->height * 2);	// ãÈå`çÇÇ≥
		s32	periphery = (s32)(width * 2 + height * 2);				// äOé¸ãóó£
		s32	progress;												// êiçsìx

		if (spd) {
			cnt = (u16)(((g_gm_main_system.sync_time * spd) + cnt) & 0x0FFF);
		}
		else {
			cnt = (u16)(((g_gm_main_system.sync_time + (cnt >> 2)) & 0x03FF) << 2);
		}
		progress  = periphery * cnt / 0x1000;;				// êiçsìx

		if (!(gmk_work->ene_com.eve_rec->flag & GMD_GMK_LAND_EVE_FLAG_LEFT_UP)) {
			// âEâÒÇË
			if (progress <= (s32)(width)) {
				// è„ï”

				pos_x = gmk_work->ene_com.born_pos_x
						+ ((left + (s16)progress) << FX32_SHIFT);
				pos_y = gmk_work->ene_com.born_pos_y
						+ (top << FX32_SHIFT);
			} else if (progress <= (s32)(width + height)) {
				// âEï”
				pos_x = gmk_work->ene_com.born_pos_x
						+ ((left + width) << FX32_SHIFT);
				pos_y = gmk_work->ene_com.born_pos_y
						+ ((top + (s16)progress - width) << FX32_SHIFT);
			} else if (progress <= (s32)(width * 2 + height)) {
				// â∫ï”
				pos_x = gmk_work->ene_com.born_pos_x
						+ ((left + width - ((s16)progress - width - height)) << FX32_SHIFT);
				pos_y = gmk_work->ene_com.born_pos_y
						+ ((top + height) << FX32_SHIFT);
			} else {
				// ç∂ï”
				pos_x = gmk_work->ene_com.born_pos_x
						+ (left << FX32_SHIFT);
				pos_y = gmk_work->ene_com.born_pos_y
						+ ((top + (height - ((s16)progress - width * 2 - height))) << FX32_SHIFT);
			}
		} else {
			// ç∂âÒÇË
			if (progress <= (s32)(width)) {
				// è„ï”
				pos_x = gmk_work->ene_com.born_pos_x
						+ ((left + width - (s16)progress) << FX32_SHIFT);
				pos_y = gmk_work->ene_com.born_pos_y
						+ (top << FX32_SHIFT);
			} else if (progress <= (s32)(width + height)) {
				// ç∂ï”
				pos_x = gmk_work->ene_com.born_pos_x
						+ (left << FX32_SHIFT);
				pos_y = gmk_work->ene_com.born_pos_y
						+ ((top + (s16)progress - width) << FX32_SHIFT);
			} else if (progress <= (s32)(width * 2 + height)) {
				// â∫ï”
				pos_x = gmk_work->ene_com.born_pos_x
						+ ((left + ((s16)progress - width - height)) << FX32_SHIFT);
				pos_y = gmk_work->ene_com.born_pos_y
						+ ((top + height) << FX32_SHIFT);
			} else {
				// âEï”
				pos_x = gmk_work->ene_com.born_pos_x
						+ ((left + width) << FX32_SHIFT);
				pos_y = gmk_work->ene_com.born_pos_y
						+ ((top + (height - ((s16)progress - width * 2 - height))) << FX32_SHIFT);
			}
		}
	}
	
	// ç¿ïWê›íË
	obj_work->move.x = pos_x - obj_work->pos.x;
	obj_work->move.y = pos_y - obj_work->pos.y;
	obj_work->pos.x = pos_x;
	obj_work->pos.y = pos_y;

}

// ==========================================================================
// gmGmkLandColFall
/*!
 *	ÉMÉ~ÉbÉN ìñÇΩÇËÇ†ÇËïÇìá óéâ∫èàóù
 *
 *	@param	gmk_work	[in]	ÉGÉlÉ~Å[ÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkLandColFall(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->move_flag & OBD_MOVE_UNDER) {
		// ê⁄ínÇµÇΩÇÁà»å„ïsìÆ
		obj_work->move_flag |= OBD_MOVE_NOCOL;		// à»å„ínñ Ç∆ÇÃê⁄êGîªíËÇµÇ»Ç¢
		obj_work->ppFunc = NULL;
	}
}

// ==========================================================================
// gmGmkZ3LandPulleyMain
/*!
 *	ÉMÉ~ÉbÉN Zone3ääé‘
 *
 *	@param	gmk_work	[in]	ÉGÉlÉ~Å[ÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkZ3LandPulleyMain(OBS_OBJECT_WORK *obj_work)
{
	obj_work->dir.z += (s16)(obj_work->user_work);
}

#if GMD_GMK_LAND_3_TEST_TVX
void gmGmkLand3TvxRopeMain(OBS_OBJECT_WORK *obj_work)
{
	OBS_ACTION3D_NN_WORK *obj_3d = obj_work->obj_3d;
	obj_3d->mat_frame += obj_3d->mat_speed;
	if (obj_3d->mat_frame >= (float)GMD_GMK_LAND_3_ROPE_MOTION) {
		obj_3d->mat_frame = obj_3d->mat_frame - (float)GMD_GMK_LAND_3_ROPE_MOTION;
	}
}

void gmGmkLand3TvxDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	if (!GmMainIsDrawEnable()) {
		return;
	}
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}

	NNS_TEXCOORD uv = {0.0f, 0.0f};
	
	// ñ{ï`âÊ
	gmGmkLand3TvxDrawFuncEx(IDB_GMK_LAND_3_MDL_GMK_LAND_3_ZNO, obj_work->obj_3d->texlist,
		&obj_work->pos, &obj_work->scale, GMD_TVX_DISP_LIGHT_DISABLE, 0, &uv);
}

void gmGmkLand3TvxRDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	if (!GmMainIsDrawEnable()) {
		return;
	}
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}
	
	NNS_TEXCOORD uv = {0.0f, 0.0f};
	
	// ñ{ï`âÊ
	gmGmkLand3TvxDrawFuncEx(IDB_GMK_LAND_3_MDL_GMK_LAND_R_3_ZNO, obj_work->obj_3d->texlist,
							&obj_work->pos, &obj_work->scale, GMD_TVX_DISP_LIGHT_DISABLE, 0, &uv);
}

void gmGmkLand3TvxPulleyDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	if (!GmMainIsDrawEnable()) {
		return;
	}
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}

	NNS_TEXCOORD uv = {0.0f, 0.0f};
	
	// ñ{ï`âÊ
	gmGmkLand3TvxDrawFuncEx(IDB_GMK_LAND_3_MDL_LAND_3_GEAR_ZNO, obj_work->obj_3d->texlist,
		&obj_work->pos, &obj_work->scale, GMD_TVX_DISP_LIGHT_DISABLE | GMD_TVX_DISP_ROTATE,
		(Angle16)-obj_work->dir.z, &uv);
}

void gmGmkLand3TvxRopeDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	if (!GmMainIsDrawEnable()) {
		return;
	}
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}

	NNS_TEXCOORD uv = {0.0f, 0.0f};
	uv.v = -0.25f * obj_work->obj_3d->mat_frame / (float)GMD_GMK_LAND_3_ROPE_MOTION;
	
	// ñ{ï`âÊ
	gmGmkLand3TvxDrawFuncEx(IDB_GMK_LAND_3_MDL_LAND_3_ROPE_ZNO, obj_work->obj_3d->texlist,
		&obj_work->pos, &obj_work->scale, GMD_TVX_DISP_LIGHT_DISABLE | GMD_TVX_DISP_ROTATE,
		(Angle16)-obj_work->dir.z, &uv);
}

void gmGmkLand3TvxDrawFuncEx(u32 tvx_index, NNS_TEXLIST *texlist, VecFx32 *pos, VecFx32 *scale, u32 disp_flag, Angle16 dir_z, NNS_TEXCOORD* uv)
{
	void* tvx            = amBindGet(gm_gmk_land_3_obj_tvx_list, tvx_index);
	
	// extend
	GMS_TVX_EX_WORK work;
	
	work.u_wrap  = NNE_PRIM_TEXWRAP_CLAMP;
	work.v_wrap  = NNE_PRIM_TEXWRAP_CLAMP;
	work.coord.u = uv->u;
	work.coord.v = uv->v;
	work.color   = 0xffffffff;
	
	GmTvxSetModelEx(tvx, texlist, pos, scale, disp_flag, dir_z, &work);
}
#endif // GMD_GMK_LAND_3_TEST_TVX

//	#if 0
//	
//	// ==========================================================================
//	// gmGmkMain
//	/*!
//	 *	ÉMÉ~ÉbÉN  ÉÅÉCÉìèàóù
//	 *
//	 *	@param	gmk_work	[in]	ÉGÉlÉ~Å[ÉèÅ[ÉN
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
//	 *	ÉMÉ~ÉbÉN  HITèàóù
//	 *
//	 *	@param	match_rect	[in]	ëäéËãÈå`ÉèÅ[ÉN
//	 *	@param	mine_rect	[in]	é©ï™ãÈå`ÉèÅ[ÉN
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
