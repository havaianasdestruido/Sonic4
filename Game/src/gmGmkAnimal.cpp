// ==========================================================================
/*!
  @file gmGmkAnimal.cpp
  @brief ìÆï®

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkAnimal.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmGmkAnimal.cpp,v $
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gs.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDBuild.h"
#include "gmEffect.h"
#include "gmGameDat.h"
#include "gmEnding.h"

#include "gmGmkAnimal.h"

// ÉfÅ[É^ÉwÉbÉ_
#if (_IPHONE)
#include "iPhone/model/GMK_ANIMAL_MDL.HMB"
#include "iPhone/model/GMK_ANIMAL_MTN.HMB"
#else
#include "common/model/GMK_ANIMAL_MDL.HMB"
#include "common/model/GMK_ANIMAL_MTN.HMB"
#endif

//----- Macros --------------------------------------------------------------
#define GMD_GMK_ANIMAL_USE_BOARD  (1 & (_IPHONE))

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
typedef enum tag_GSE_GMK_ANM_TYPE {
	GSD_GMK_ANM_TYPE_PIG		= 0,	// Ç‘ÇΩ
	GSD_GMK_ANM_TYPE_BIRD,				// Ç∆ÇË
	GSD_GMK_ANM_TYPE_PENGUIN,			// ÉyÉìÉMÉì
	GSD_GMK_ANM_TYPE_RABBIT,			// Ç§Ç≥Ç¨
	GSD_GMK_ANM_TYPE_CHICKEN,			// Ç…ÇÌÇ∆ÇË

	GSD_GMK_ANM_TYPE_MAX
} GSE_GMK_ANM_TYPE;

#if GMD_GMK_ANIMAL_USE_BOARD
typedef enum tag_GSE_GMK_ANM_MODEL_TYPE {
	GSD_GMK_ANM_MODEL_TYPE_ST = 0,	// stand
	GSD_GMK_ANM_MODEL_TYPE_UP,		// 1st jump
	GSD_GMK_ANM_MODEL_TYPE_DOWN,	// jump right
	GSD_GMK_ANM_MODEL_TYPE_DOWN_L,	// jump left
	
	GSD_GMK_ANM_MODE_TYPE_MAX
} GSE_GMK_ANM_MODEL_TYPE;
#endif // GMD_GMK_ANIMAL_USE_BOARD

// ìÆï®à⁄ìÆÉpÉâÉÅÅ[É^
typedef struct tag_GMS_GMK_ANIMAL_PARAM {
	fx32	spd_x;			//!< Xë¨ìx
	fx32	jump;			//!< ÉWÉÉÉìÉvèâë¨
	fx32	gravity;		//!< èdóÕâ¡ë¨ìx
} GMS_GMK_ANIMAL_PARAM;

#define GMD_GMK_ANIMAL_ANGLE	0x1800	// ìÆï®â°å¸Ç´äpìx


//ÉGÉìÉfÉBÉìÉOópìÆï®
//GMS_EVE_RECORD_EVENT::flag
#define GMD_GMK_ANIMAL_ANML_TYPE_MASK	(0x07)
#define GMD_GMK_ANIMAL_MOVE_TYPE_MASK	(0x18)
#define GMD_GMK_ANIMAL_MOVE_TYPE_LEFT	(0x08)
#define GMD_GMK_ANIMAL_MOVE_TYPE_RIGHT	(0x10)
//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkAnimalWait(OBS_OBJECT_WORK *obj_work);
static void gmGmkAnimalJump(OBS_OBJECT_WORK *obj_work);
static void gmGmkAnimalObjSet(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_NN_WORK *dest_obj_3d);
static void gmGmkEndingAnimalMove(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_animal_obj_3d_list = NULL;

// äeÉ]Å[ÉìÇ…ìoèÍÇ∑ÇÈìÆï®íËã`
static const u32 g_gm_gmk_animal_type_id[GSD_MAIN_ZONE_TYPE_MAX][2] = {
	{ GSD_GMK_ANM_TYPE_RABBIT,	GSD_GMK_ANM_TYPE_BIRD	},	// zone 1
	{ GSD_GMK_ANM_TYPE_CHICKEN,	GSD_GMK_ANM_TYPE_PIG	},	// zone 2
	{ GSD_GMK_ANM_TYPE_PENGUIN,	GSD_GMK_ANM_TYPE_BIRD	},	// zone 3
	{ GSD_GMK_ANM_TYPE_CHICKEN,	GSD_GMK_ANM_TYPE_RABBIT	},	// zone 4
	{ NULL,						NULL					},	// zone Final
	{ NULL,						NULL					},	// SpecialStage
};

#if GMD_GMK_ANIMAL_USE_BOARD
// ìÆï®OBJÉÇÉfÉã
static const u32 g_gm_gmk_animal_obj_id[GSD_GMK_ANM_TYPE_MAX][GSD_GMK_ANM_MODE_TYPE_MAX] = {
	{
		IDB_GMK_ANIMAL_MDL_GMK_ANML_PIG_ST_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_PIG_UP_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_PIG_DOWN_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_PIG_DOWN_L_INO,
	}, // Ç‘ÇΩ
	{
		IDB_GMK_ANIMAL_MDL_GMK_ANML_BIRD_ST_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_BIRD_UP_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_BIRD_DOWN_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_BIRD_DOWN_L_INO,
	}, // Ç∆ÇË
	{
		IDB_GMK_ANIMAL_MDL_GMK_ANML_PEN_ST_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_PEN_UP_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_PEN_DOWN_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_PEN_DOWN_L_INO,
	}, // ÉyÉìÉMÉì
	{
		IDB_GMK_ANIMAL_MDL_GMK_ANML_RAB_ST_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_RAB_UP_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_RAB_DOWN_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_RAB_DOWN_L_INO,
	}, // Ç§Ç≥Ç¨
	{
		IDB_GMK_ANIMAL_MDL_GMK_ANML_COCK_ST_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_COCK_UP_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_COCK_DOWN_INO,
		IDB_GMK_ANIMAL_MDL_GMK_ANML_COCK_DOWN_L_INO,
	}, // Ç…ÇÌÇ∆ÇË
};
#else
// ìÆï®OBJÉÇÉfÉã
static const u32 g_gm_gmk_animal_obj_id[GSD_GMK_ANM_TYPE_MAX] = {
	IDB_GMK_ANIMAL_MDL_GMK_ANML_PIG_ZNO,	// Ç‘ÇΩ
	IDB_GMK_ANIMAL_MDL_GMK_ANML_BIRD_ZNO,	// Ç∆ÇË
	IDB_GMK_ANIMAL_MDL_GMK_ANML_PEN_ZNO,	// ÉyÉìÉMÉì
	IDB_GMK_ANIMAL_MDL_GMK_ANML_RABBIT_ZNO,	// Ç§Ç≥Ç¨
	IDB_GMK_ANIMAL_MDL_GMK_ANML_COCK_ZNO,	// Ç…ÇÌÇ∆ÇË
};

// ìÆï®ÉÇÅ[ÉVÉáÉìID
static const s32 g_gm_gmk_animal_mtn_id[GSD_GMK_ANM_TYPE_MAX][3] = {
	// ÉtÉbÉgÉèÅ[ÉN								è„è∏									â∫ç~
	{ IDB_GMK_ANIMAL_MTN_GMK_ANML_PIG_ST_ZNM,	IDB_GMK_ANIMAL_MTN_GMK_ANML_PIG_UP_ZNM,		IDB_GMK_ANIMAL_MTN_GMK_ANML_PIG_DOWN_ZNM,	},	// Ç‘ÇΩ
	{ IDB_GMK_ANIMAL_MTN_GMK_ANML_BIRD_ST_ZNM,	IDB_GMK_ANIMAL_MTN_GMK_ANML_BIRD_UP_ZNM,	IDB_GMK_ANIMAL_MTN_GMK_ANML_BIRD_DOWN_ZNM,	},  // Ç∆ÇË
	{ IDB_GMK_ANIMAL_MTN_GMK_ANML_PEN_ST_ZNM,	IDB_GMK_ANIMAL_MTN_GMK_ANML_PEN_UP_ZNM,		IDB_GMK_ANIMAL_MTN_GMK_ANML_PEN_DOWN_ZNM,	},  // ÉyÉìÉMÉì
	{ IDB_GMK_ANIMAL_MTN_GMK_ANML_RABBIT_ST_ZNM,IDB_GMK_ANIMAL_MTN_GMK_ANML_RABBIT_UP_ZNM,	IDB_GMK_ANIMAL_MTN_GMK_ANML_RABBIT_DOWN_ZNM,},  // Ç§Ç≥Ç¨
	{ IDB_GMK_ANIMAL_MTN_GMK_ANML_COCK_ST_ZNM,	IDB_GMK_ANIMAL_MTN_GMK_ANML_COCK_UP_ZNM,	IDB_GMK_ANIMAL_MTN_GMK_ANML_COCK_DOWN_ZNM,	},  // Ç…ÇÌÇ∆ÇË
};
#endif // GMD_GMK_ANIMAL_USE_BOARD

// ìÆï®à⁄ìÆÉpÉâÉÅÅ[É^
static const GMS_GMK_ANIMAL_PARAM g_gm_gmk_animal_speed_param[GSD_GMK_ANM_TYPE_MAX] = {
	 // x_speed				// jump_speed		// gravity
	{(0x14<<(FX32_SHIFT-4)),(-4 << FX32_SHIFT),	GMD_OBJ_DEF_FALL_SPD+0xc0,	},	// Ç‘ÇΩ
	{(0x14<<(FX32_SHIFT-4)),(-4 << FX32_SHIFT),	GMD_OBJ_DEF_FALL_SPD-0xc0,	},	// Ç∆ÇË
	{(0x14<<(FX32_SHIFT-4)),(-4 << FX32_SHIFT),	GMD_OBJ_DEF_FALL_SPD+0xc0,	},	// ÉyÉìÉMÉì
	{(0x14<<(FX32_SHIFT-4)),(-4 << FX32_SHIFT),	GMD_OBJ_DEF_FALL_SPD+0xc0,	},	// Ç§Ç≥Ç¨
	{(0x14<<(FX32_SHIFT-4)),(-4 << FX32_SHIFT),	GMD_OBJ_DEF_FALL_SPD-0xc0,	},	// Ç…ÇÌÇ∆ÇË
};
//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkAnimalBuild
/*!
 *	ÉMÉ~ÉbÉN ìÆï® ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkAnimalBuild(void)
{
	gm_gmk_animal_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_ANIMAL_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_ANIMAL_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
}

// ==========================================================================
// GmGmkAnimalFlush
/*!
 *	ÉMÉ~ÉbÉN ìÆï® ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkAnimalFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_ANIMAL_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_animal_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmGmkAnimalInit
/*!
 *	ÉMÉ~ÉbÉN ìÆï® èâä˙âªä÷êî
 *
 *	@param	obj_work[in]	êeÉèÅ[ÉN
 *	@param	ofs_x	[in]	ç¿ïWÉIÉtÉZÉbÉgÇwÅiêeç¿ïWÇ©ÇÁÇÃÉIÉtÉZÉbÉgÅj
 *	@param	ofs_y	[in]	ç¿ïWÉIÉtÉZÉbÉgÇxÅiêeç¿ïWÇ©ÇÁÇÃÉIÉtÉZÉbÉgÅj
 *	@param	ofs_z	[in]	ç¿ïWÉIÉtÉZÉbÉgÇyÅiêeç¿ïWÇ©ÇÁÇÃÉIÉtÉZÉbÉgÅj
 *	@param	type	[in]	ìÆï®éÌóﬁ
 *	@param	vec		[in]	à⁄ìÆï˚å¸
 *	@param	timer	[in]	à⁄ìÆäJénÇ‹Ç≈ÇÃéûä‘
 *
 *	@note
 *		type ÇÕìÆï®éÌóﬁ
 *			0		: ÉâÉìÉ_ÉÄ
 *			1Å`2	: ÉXÉeÅ[ÉWå≈óLÇÃÇQéÌóﬁÇ«ÇøÇÁÇï\é¶Ç∑ÇÈÇ©éwíËÅiÉJÉvÉZÉãópÅj
 *		vec ÇÕìÆï®ÇÃà⁄ìÆï˚å¸
 *			0		: ç∂
 *			1		: âEÅiÉJÉvÉZÉãópÅj
 *		timer ÇÕìÆï®ÇÃà⁄ìÆäJénÇ‹Ç≈ÇÃéûä‘
 *			0		: ë¶éûçsìÆ
 *			1Å`		: ÉEÉFÉCÉgéûä‘ÅiÉJÉvÉZÉãópÅj
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkAnimalInit(OBS_OBJECT_WORK *parent_work, fx32 ofs_x, fx32 ofs_y, fx32 ofs_z, u8 type, u8 vec, u16 timer)
{
	OBS_OBJECT_WORK			*obj_work;
	GMS_EFFECT_3DNN_WORK	*efct_work;
	GSE_MAIN_ZONE_TYPE		zone_id;

	obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_3DNN_WORK), parent_work/*parent_obj*/, 0/*sort_prio*/, "GMK_ANIMAL");
	efct_work = (GMS_EFFECT_3DNN_WORK*)obj_work;

	// âÊñ äOê∂ë∂îÕàÕê›íË
	obj_work->view_out_ofst = 64;

	// èâä˙ÉIÉtÉZÉbÉg
	obj_work->pos.x += ofs_x;
	obj_work->pos.y += ofs_y;
	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK + ofs_z;

	
	// ìÆï®É^ÉCÉvÉZÉbÉg
	if (type) {
		type = (u8)((type - 1) & 1);
	} else {
		type = (u8)(mtMathRand() & 1);
	}
	zone_id = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	obj_work->user_work = g_gm_gmk_animal_type_id[zone_id][type];

	// å¸Ç´
	obj_work->user_flag = vec;

	// ÉEÉFÉCÉgéûä‘
	obj_work->user_timer = timer;


	gmGmkAnimalObjSet(obj_work, &efct_work->obj_3d);


#if 0
	// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
#if GMD_GMK_ANIMAL_USE_BOARD
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_animal_obj_3d_list[g_gm_gmk_animal_obj_id[obj_work->user_work][GSD_GMK_ANM_MODEL_TYPE_ST]],
					&efct_work->obj_3d);
#else
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_animal_obj_3d_list[g_gm_gmk_animal_obj_id[obj_work->user_work]],
					&efct_work->obj_3d);
	// ÉÇÅ[ÉVÉáÉìèâä˙âª
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_GMK_ANIMAL_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);
	// ÉgÉDÅ[Éìê›íË
	ObjDrawObjectSetToon(obj_work);

	// ÉAÉNÉVÉáÉìê›íË
	ObjDrawObjectActionSet(obj_work, g_gm_gmk_animal_mtn_id[obj_work->user_work][0]);
#endif // GMD_GMK_ANIMAL_USE_BOARD
	// ínå`Ç†ÇΩÇË
	ObjObjectFieldRectSet(obj_work, -2, -8, 2, 0);

	// å¬ï ê›íË
#if _PS3 | _XBOX | _PC
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_NOSCALE | OBD_DISP_REPEAT;
#else	// _WII | _IPHONE
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_NOSCALE;
#endif
#endif
//	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ínå`Ç†ÇΩÇËñ≥Çµ
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL_MASK;
	obj_work->move_flag &= ~OBD_MOVE_FALL;
	obj_work->flag		|= OBD_OBJECT_PARENT_NODIE;				// êeÇ™éÄÇÒÇ≈Ç‡ê∂Ç´î≤Ç≠
	obj_work->flag		|= OBD_OBJECT_NOHIT;					// ÉRÉäÉWÉáÉìîªíËÇµÇ»Ç¢
	obj_work->flag		&= ~OBD_OBJECT_NOCLIP;					// âÊñ äOÇ≈éÄñS
	// ÉÅÉCÉìèàóù
	obj_work->ppFunc = gmGmkAnimalWait;

	return (obj_work);
}

// ==========================================================================
// GmGmkEndingAnimalInit
/*!
 *	ÉMÉ~ÉbÉN ÉGÉìÉfÉBÉìÉOópìÆï® èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkEndingAnimalInit(GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;
	u32					anml_type;
	
	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_END_ANIMAL");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	anml_type = (u32)(eve_rec->flag & GMD_GMK_ANIMAL_ANML_TYPE_MASK);
	MTM_ASSERT(anml_type < GSD_GMK_ANM_TYPE_MAX);
	obj_work->user_work = anml_type;
	

	// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
	gmGmkAnimalObjSet(obj_work, &gmk_work->obj_3d);

	// å¸Ç´
	if (eve_rec->flag & GMD_GMK_ANIMAL_MOVE_TYPE_RIGHT) {
		obj_work->user_flag = 2;
	} else {
		obj_work->user_flag = 0;
	}


	// å¬ï ê›íË
//	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL_MASK;
//	obj_work->move_flag &= ~OBD_MOVE_FALL;
#if _PS3 | _XBOX | _PC
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_NOSCALE | OBD_DISP_REPEAT;
#else	// _WII | _IPHONE
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_NOSCALE;
#endif
	obj_work->move_flag &= ~(OBD_MOVE_NOMOVE
							| OBD_MOVE_NOCOLFIELD	// BGínå`Ç…ÇÕÉqÉbÉg
							| OBD_MOVE_NOCOL		// BGínå`Ç…ÇÕÉqÉbÉg
							| OBD_MOVE_NOCOL_H);	// BGínå`Ç…ÇÕÉqÉbÉg
	obj_work->move_flag |=  (OBD_MOVE_JUMP
							| OBD_MOVE_FALL
							| OBD_MOVE_NOCOL_W
							| OBD_MOVE_NOCOLOBJ);	// OBJínå`Ç…ÇÕÉmÅ[ÉqÉbÉgÅiÉJÉvÉZÉãÇ…ìñÇΩÇ¡ÇƒÇµÇ‹Ç§ÇΩÇﬂÅj

	obj_work->spd.y	   = -g_gm_gmk_animal_speed_param[obj_work->user_work].jump;	// ç≈èâÇÕÇ∑ÇÆÇ…íÖín(MINUSï˚å¸)
	obj_work->spd_fall = g_gm_gmk_animal_speed_param[obj_work->user_work].gravity;

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;


	obj_work->flag		|= OBD_OBJECT_PARENT_NODIE;				// êeÇ™éÄÇÒÇ≈Ç‡ê∂Ç´î≤Ç≠
	obj_work->flag		|= OBD_OBJECT_NOHIT;					// ÉRÉäÉWÉáÉìîªíËÇµÇ»Ç¢
	obj_work->flag		&= ~OBD_OBJECT_NOCLIP;					// âÊñ äOÇ≈éÄñS
	// ÉÅÉCÉìèàóù
	obj_work->ppFunc = gmGmkEndingAnimalMove;

	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkAnimalObjSet
/*!
 *	ÉMÉ~ÉbÉN ìÆï®ÉIÉuÉWÉFÉNÉgÉZÉbÉgÉAÉbÉv
 *
 *	@param obj_work		[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *	@param dest_obj_3d	[in] ÉRÉsÅ[êÊ3DÉIÉuÉWÉFÉNÉgÉèÅ[ÉNÉ|ÉCÉìÉ^
 *
 *	@note
 *		éñëOÇ… obj_work->user_work Ç÷ìÆï®É^ÉCÉv(GSE_GMK_ANM_TYPE)Ç
 *		ÉZÉbÉgÇµÇƒÇ©ÇÁÉRÅ[ÉãÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB
 */
// ==========================================================================
void gmGmkAnimalObjSet(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_NN_WORK *dest_obj_3d)
{
	// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
#if GMD_GMK_ANIMAL_USE_BOARD
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_animal_obj_3d_list[g_gm_gmk_animal_obj_id[obj_work->user_work][GSD_GMK_ANM_MODEL_TYPE_ST]],
					dest_obj_3d);
#else //GMD_GMK_ANIMAL_USE_BOARD
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_animal_obj_3d_list[g_gm_gmk_animal_obj_id[obj_work->user_work]],
					dest_obj_3d);

	// ÉÇÅ[ÉVÉáÉìèâä˙âª
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_GMK_ANIMAL_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);
	// ÉgÉDÅ[Éìê›íË
	ObjDrawObjectSetToon(obj_work);

	// ÉAÉNÉVÉáÉìê›íË
	ObjDrawObjectActionSet(obj_work, g_gm_gmk_animal_mtn_id[obj_work->user_work][0]);
#endif // GMD_GMK_ANIMAL_USE_BOARD

	// ínå`Ç†ÇΩÇË
	ObjObjectFieldRectSet(obj_work, -2, -8, 2, 0);

	// å¬ï ê›íË
#if _PS3 | _XBOX | _PC
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_NOSCALE | OBD_DISP_REPEAT;
#else	// _WII | _IPHONE
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_NOSCALE;
#endif
}
// ==========================================================================
// gmGmkAnimalWait
/*!
 *	ÉMÉ~ÉbÉN ìÆï®à⁄ìÆë“Çø
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *
 */
// ==========================================================================
void gmGmkAnimalWait(OBS_OBJECT_WORK *obj_work)
{
//	GMS_EFFECT_3DNN_WORK	*efct_work = (GMS_EFFECT_3DNN_WORK*)obj_work;

	if (obj_work->user_timer) {
		obj_work->user_timer--;
	} else {
#if GMD_GMK_ANIMAL_USE_BOARD
		ObjObjectAction3dNNModelReleaseCopy(obj_work);
		ObjObjectCopyAction3dNNModel(obj_work,
									 &gm_gmk_animal_obj_3d_list[g_gm_gmk_animal_obj_id[obj_work->user_work][GSD_GMK_ANM_MODEL_TYPE_UP]],
									 obj_work->obj_3d);
#endif // GMD_GMK_ANIMAL_USE_BOARD
		// ÉÅÉCÉìèàóù
		obj_work->move_flag &= ~(OBD_MOVE_NOMOVE
								| OBD_MOVE_NOCOLFIELD	// BGínå`Ç…ÇÕÉqÉbÉg
								| OBD_MOVE_NOCOL		// BGínå`Ç…ÇÕÉqÉbÉg
								| OBD_MOVE_NOCOL_H);	// BGínå`Ç…ÇÕÉqÉbÉg
		obj_work->move_flag |=  (OBD_MOVE_JUMP
								| OBD_MOVE_FALL
								| OBD_MOVE_NOCOL_W
								| OBD_MOVE_NOCOLOBJ);	// OBJínå`Ç…ÇÕÉmÅ[ÉqÉbÉgÅiÉJÉvÉZÉãÇ…ìñÇΩÇ¡ÇƒÇµÇ‹Ç§ÇΩÇﬂÅj

		obj_work->spd.y	   = g_gm_gmk_animal_speed_param[obj_work->user_work].jump;
		obj_work->spd_fall = g_gm_gmk_animal_speed_param[obj_work->user_work].gravity;

		// óDêÊê›íË
		obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_FRONT;

		obj_work->ppFunc = gmGmkAnimalJump;
	}
}

// ==========================================================================
// gmGmkAnimalJump
/*!
 *	ÉMÉ~ÉbÉN ìÆï®ÉWÉÉÉìÉv
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *
 */
// ==========================================================================
void gmGmkAnimalJump(OBS_OBJECT_WORK *obj_work)
{
#if !GMD_GMK_ANIMAL_USE_BOARD
	// ÉÇÉfÉãÇ≈ÉAÉjÉÅÅ[ÉVÉáÉìÇçsÇ§ç€Ç…ïKóvÇ»èàóù
	// ÉAÉNÉVÉáÉìïœçXÉ`ÉFÉbÉN
	if (obj_work->spd.y < 0) {
		// è„è∏íÜ
		if (obj_work->obj_3d->act_id[0] != g_gm_gmk_animal_mtn_id[obj_work->user_work][1]) {
			ObjDrawObjectActionSet3DNNBlend(obj_work, g_gm_gmk_animal_mtn_id[obj_work->user_work][1]);
			obj_work->disp_flag |= OBD_DISP_REPEAT;
		}
	} else {
		// â∫ç~íÜ
		if (obj_work->obj_3d->act_id[0] != g_gm_gmk_animal_mtn_id[obj_work->user_work][2]) {
			ObjDrawObjectActionSet3DNNBlend(obj_work, g_gm_gmk_animal_mtn_id[obj_work->user_work][2]);
			obj_work->disp_flag |= OBD_DISP_REPEAT;
		}
	}
#endif // !GMD_GMK_ANIMAL_USE_BOARD

	// íÖínÅïà⁄ìÆë¨ìxïœçXÉ`ÉFÉbÉN
	if (obj_work->move_flag & OBD_MOVE_UNDER) {
		if (obj_work->user_flag) {
			// âE
			obj_work->spd.x	= g_gm_gmk_animal_speed_param[obj_work->user_work].spd_x;
#if GMD_GMK_ANIMAL_USE_BOARD
			ObjObjectAction3dNNModelReleaseCopy(obj_work);
			ObjObjectCopyAction3dNNModel(obj_work,
										 &gm_gmk_animal_obj_3d_list[g_gm_gmk_animal_obj_id[obj_work->user_work][GSD_GMK_ANM_MODEL_TYPE_DOWN]],
										 obj_work->obj_3d);
			obj_work->dir.y = 0xB000;
#else
			obj_work->dir.y = (u16)(-GMD_GMK_ANIMAL_ANGLE);
#endif // GMD_GMK_ANIMAL_USE_BOARD
		} else {
			// ç∂
			obj_work->spd.x	= -g_gm_gmk_animal_speed_param[obj_work->user_work].spd_x;
#if GMD_GMK_ANIMAL_USE_BOARD
			ObjObjectAction3dNNModelReleaseCopy(obj_work);
			ObjObjectCopyAction3dNNModel(obj_work,
										 &gm_gmk_animal_obj_3d_list[g_gm_gmk_animal_obj_id[obj_work->user_work][GSD_GMK_ANM_MODEL_TYPE_DOWN_L]],
										 obj_work->obj_3d);
			obj_work->dir.y = 0xB000;
#else
			obj_work->dir.y = 0x8000+GMD_GMK_ANIMAL_ANGLE;
#endif // GMD_GMK_ANIMAL_USE_BOARD
		}
		obj_work->spd.y		= g_gm_gmk_animal_speed_param[obj_work->user_work].jump;
		obj_work->move_flag |= OBD_MOVE_JUMP;
		obj_work->disp_flag &= ~(OBD_DISP_NODIRFLIP | OBD_DISP_NODIR);
	}
}

// ==========================================================================
// gmGmkEndingAnimalMove
/*!
 *	ÉMÉ~ÉbÉN ÉGÉìÉfÉBÉìÉOìÆï®à⁄ìÆ
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *
 */
// ==========================================================================
void gmGmkEndingAnimalMove(OBS_OBJECT_WORK *obj_work)
{
#if !GMD_GMK_ANIMAL_USE_BOARD
	// ÉÇÉfÉãÇ≈ÉAÉjÉÅÅ[ÉVÉáÉìÇçsÇ§ç€Ç…ïKóvÇ»èàóù
	// ÉAÉNÉVÉáÉìïœçXÉ`ÉFÉbÉN
	if (obj_work->spd.y < 0) {
		if (obj_work->obj_3d->act_id[0] != g_gm_gmk_animal_mtn_id[obj_work->user_work][1]) {
			ObjDrawObjectActionSet3DNNBlend(obj_work, g_gm_gmk_animal_mtn_id[obj_work->user_work][1]);
			obj_work->disp_flag |= OBD_DISP_REPEAT;
		}
	} else {
		if (obj_work->obj_3d->act_id[0] != g_gm_gmk_animal_mtn_id[obj_work->user_work][2]) {
			ObjDrawObjectActionSet3DNNBlend(obj_work, g_gm_gmk_animal_mtn_id[obj_work->user_work][2]);
			obj_work->disp_flag |= OBD_DISP_REPEAT;
		}
	}
#endif // !GMD_GMK_ANIMAL_USE_BOARD

	// íÖínÅïà⁄ìÆë¨ìxïœçXÉ`ÉFÉbÉN
	if (obj_work->move_flag & OBD_MOVE_UNDER) {
		if (  (!(((GMS_ENEMY_COM_WORK*)obj_work)->eve_rec->flag & GMD_GMK_ANIMAL_MOVE_TYPE_MASK))
			||(GmEndingAnimalForwardChk()) ) {
			// ëOå¸Ç´
				obj_work->spd.x	= 0;
#if GMD_GMK_ANIMAL_USE_BOARD
				// iPhoneêÍópî¬É|ÉäÉÇÉfÉãêÿÇËë÷Ç¶
				ObjObjectAction3dNNModelReleaseCopy(obj_work);
				ObjObjectCopyAction3dNNModel(obj_work,
											 &gm_gmk_animal_obj_3d_list[g_gm_gmk_animal_obj_id[obj_work->user_work][GSD_GMK_ANM_MODEL_TYPE_UP]],
											 obj_work->obj_3d);
				obj_work->dir.y = 0xB000;
#else
				obj_work->dir.y = 0xc000;
#endif // GMD_GMK_ANIMAL_USE_BOARD
		} else {
			// â°å¸Ç´
			obj_work->user_flag = (++obj_work->user_flag) & 3;
			if (obj_work->user_flag & 2) {
				// âE
				obj_work->spd.x	= g_gm_gmk_animal_speed_param[obj_work->user_work].spd_x;
#if GMD_GMK_ANIMAL_USE_BOARD
				// iPhoneêÍópî¬É|ÉäÉÇÉfÉãêÿÇËë÷Ç¶
				ObjObjectAction3dNNModelReleaseCopy(obj_work);
				ObjObjectCopyAction3dNNModel(obj_work,
											 &gm_gmk_animal_obj_3d_list[g_gm_gmk_animal_obj_id[obj_work->user_work][GSD_GMK_ANM_MODEL_TYPE_DOWN]],
											 obj_work->obj_3d);
				obj_work->dir.y = 0xB000;
#else
				obj_work->dir.y = (u16)(-GMD_GMK_ANIMAL_ANGLE);
#endif // GMD_GMK_ANIMAL_USE_BOARD
			} else {
				// ç∂
				obj_work->spd.x	= -g_gm_gmk_animal_speed_param[obj_work->user_work].spd_x;
#if GMD_GMK_ANIMAL_USE_BOARD
				// iPhoneêÍópî¬É|ÉäÉÇÉfÉãêÿÇËë÷Ç¶
				ObjObjectAction3dNNModelReleaseCopy(obj_work);
				ObjObjectCopyAction3dNNModel(obj_work,
											 &gm_gmk_animal_obj_3d_list[g_gm_gmk_animal_obj_id[obj_work->user_work][GSD_GMK_ANM_MODEL_TYPE_DOWN_L]],
											 obj_work->obj_3d);
				obj_work->dir.y = 0xB000;
#else
				obj_work->dir.y = 0x8000+GMD_GMK_ANIMAL_ANGLE;
#endif // GMD_GMK_ANIMAL_USE_BOARD
			}
		}
		obj_work->spd.y		= g_gm_gmk_animal_speed_param[obj_work->user_work].jump;
		obj_work->move_flag |= OBD_MOVE_JUMP;
		obj_work->disp_flag &= ~(OBD_DISP_NODIRFLIP | OBD_DISP_NODIR);
	}
}

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
