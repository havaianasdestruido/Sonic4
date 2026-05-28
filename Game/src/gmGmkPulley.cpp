// ==========================================================================
/*!
  @file gmGmkPulley.cpp
  @brief ääé‘(Pulley)

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkPulley.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmGmkPulley.cpp,v $
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
#include "gmPlySpec.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectZone.h"
#include "gmComEfct.h"
#include "gmSound.h"

#include "gmGmkPulley.h"

// ÉfÅ[É^ÉwÉbÉ_
#include "common/model/GMK_PULLEY_MDL.HMB"
#include "common/model/GMK_PULLEY_MTN.HMB"


//----- Macros --------------------------------------------------------------
#define GMD_GMK_PULLEY_USE_DRAW_SERVER	(1 & (_IPHONE | _PC))

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------

#define GDM_GMK_PULLEY_POLE_YOFS (6)	// ääé‘É|Å[ÉãÇ™ècÇ…ëÂÇ´Ç≠Ç»Ç¡ÇΩï™ÉIÉtÉZÉbÉgó 

// ääé‘îªíËãÈå`ÉTÉCÉY
#define GMD_GMK_PULLEY_RECT_LF	(-4)
#define GMD_GMK_PULLEY_RECT_UP	( 3 + GDM_GMK_PULLEY_POLE_YOFS)
#define GMD_GMK_PULLEY_RECT_RT	( 4)
#define GMD_GMK_PULLEY_RECT_DW	(18 + GDM_GMK_PULLEY_POLE_YOFS)

#define GMD_GMK_PULLEY_ANGLE	(0x2800)
#define GMD_GMK_PULLEY_BRAKE	(fx32)((1.0f * FX32_ONE) /64)	// é©ëRå∏ë¨
#define GMD_GMK_PULLEY_GRAVITY	(fx32)((1.0f * FX32_ONE) /32)	// èdóÕâ¡ë¨
#define GMD_GMK_PULLEY_ACCEL	(fx32)((1.25f * FX32_ONE) /32)	// ëÄçÏâ¡ë¨

//#define GMD_GMK_PULLEY_PUT_XSPD	(8 << FX32_SHIFT)		// ÉvÅ[ÉäÅ[ó£ÇÍÇÈéûÇÃÉWÉÉÉìÉvÇwç≈í·ë¨ìx
#define GMD_GMK_PULLEY_PUT_XSPD_MIN	(4 << FX32_SHIFT)		// ÉvÅ[ÉäÅ[ó£ÇÍÇÈéûÇÃÉWÉÉÉìÉvÇwç≈í·ë¨ìx
#define GMD_GMK_PULLEY_PUT_XSPD_MAX	(6 << FX32_SHIFT)		// ÉvÅ[ÉäÅ[ó£ÇÍÇÈéûÇÃÉWÉÉÉìÉvÇwç≈çÇë¨ìx
#define GMD_GMK_PULLEY_PUT_YSPD	(-(3 << FX32_SHIFT))	// ÉvÅ[ÉäÅ[ó£ÇÍÇÈéûÇÃÉWÉÉÉìÉvÇxë¨ìx

#define GMD_GMK_PULLEY_REHIT_TIME	(36)			// ÉvÅ[ÉäÅ[Ç©ÇÁó£ÇÍÇΩéûÇÃHITîªíËçƒäJÇ‹Ç≈ÇÃéûä‘

// GMS_EVE_RECORD_EVENT : flag
#define GMD_GMK_PULLEY_EVE_FLAG_LEFT	(0x0001)	//!< ç∂ï˚å¸Ç÷à⁄ìÆ
#define GMD_GMK_PULLEY_EVE_FLAG_TILT    (0x0002)	//!< éŒÇﬂï˚å¸Ç÷à⁄ìÆ

// user_flag
#define GMD_GMK_PULLEY_MOVE_STOP		(1 << 15)			//!< à⁄ìÆë¨ìxí‚é~èÛë‘(ÉäÉXÉ^Å[ÉgÉAÉNÉVÉáÉìêßå‰Ç…égóp


/// ääé‘ÉvÅ[ÉäÅ[ÉèÅ[ÉN
typedef struct tag_GMS_GMK_PULLEY_WORK {

	GMS_ENEMY_3D_WORK	gmk_work;
	GSS_SND_SE_HANDLE*	se_handle;
	GMS_EFFECT_3DES_WORK* efct_work;

} GMS_GMK_PULLEY_WORK;

#if GMD_GMK_PULLEY_USE_DRAW_SERVER
// ä«óù
#define GMD_GMK_PULLEY_REGISTER_NUM	(32)	//	ìoò^êî

typedef enum tag_GME_GMK_PULLEY_TYPE {
	GMD_GMK_PULLEY_TYPE_ROPE_N = 0,		//!<	í èÌÉçÅ[Év
	GMD_GMK_PULLEY_TYPE_ROPE_TL,		//!<	éŒÇﬂÉçÅ[Évç∂å¸Ç´
	GMD_GMK_PULLEY_TYPE_ROPE_TR,		//!<	éŒÇﬂÉçÅ[ÉvâEå¸Ç´
	GMD_GMK_PULLEY_TYPE_POLE_L,			//!<	É|Å[Éãç∂
	GMD_GMK_PULLEY_TYPE_POLE_R,			//!<	É|Å[ÉãâE
	
	GMD_GMK_PULLEY_TYPE_MAX
} GME_GMK_PULLEY_TYPE;

typedef struct tag_GMS_GMK_PULLEY_REGISTER {
	u16				type;		//!<	ï`âÊÉ^ÉCÉv
	u16				flip;		//!<	îΩì]É^ÉCÉv(Yé≤)
	VecFx32			vec;		//!<	ìoò^à íu
} GMS_GMK_PULLEY_REGISTER;

typedef struct tag_GMS_GMK_PULLEY_MANAGER {
	u32				tex_id;		//!<	ÉeÉNÉXÉ`ÉÉID
	NNS_TEXLIST*	texlist;	//!<	ÉeÉNÉXÉ`ÉÉÉäÉXÉg
	u32				num;		//!<	ìoò^êî
	u32				rsv;		//!<	ó\ñÒ
	
	GMS_GMK_PULLEY_REGISTER	reg[GMD_GMK_PULLEY_REGISTER_NUM];	//!<	ìoò^ääé‘èÓïÒ
} GMS_GMK_PULLEY_MANAGER;
#endif // GMD_GMK_PULLEY_USE_DRAW_SERVER


//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkPulleyBaseExit(MTS_TASK_TCB *tcb);
static void gmGmkPulleyDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
static void gmGmkPulleyMove(OBS_OBJECT_WORK *obj_work);
static void gmGmkPulleySonicTakeOffSet(GMS_PLAYER_WORK *ply_work, fx32 spd_x);
static void gmGmkPulleySecedeSet(OBS_OBJECT_WORK *obj_work, fx32 pos_x);
static void gmGmkPulleySecede(OBS_OBJECT_WORK *obj_work);
static void gmGmkPulleyRotMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkPulleySparkInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkPulleySparkKill(OBS_OBJECT_WORK *obj_work);

#if GMD_GMK_PULLEY_USE_DRAW_SERVER
static void gmGmkPulleyDrawSetRopeN(OBS_OBJECT_WORK *obj_work);
static void gmGmkPulleyDrawSetRopeTL(OBS_OBJECT_WORK *obj_work);
static void gmGmkPulleyDrawSetRopeTR(OBS_OBJECT_WORK *obj_work);
static void gmGmkPulleyDrawSetPoleL(OBS_OBJECT_WORK *obj_work);
static void gmGmkPulleyDrawSetPoleR(OBS_OBJECT_WORK *obj_work);

static void gmGmkPulleyDrawSetObject(OBS_OBJECT_WORK *obj_work, GME_GMK_PULLEY_TYPE type);
#endif // GMD_GMK_PULLEY_USE_DRAW_SERVER

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_pulley_obj_3d_list = NULL;

#if GMD_GMK_PULLEY_USE_DRAW_SERVER
static GMS_GMK_PULLEY_MANAGER gm_gmk_pulley_manager = {0};

// äeÉIÉuÉWÉFÉNÉgí∏ì_ç¿ïW
const static NNS_VECTOR gm_gmk_pulley_pos[][4] = {
	// GMD_GMK_PULLEY_TYPE_ROPE_N
	{
		{ 0.000000F * 3.2f,   0.984266F * 3.2f,  0.000000F * 3.2f},
		{ 0.000000F * 3.2f, -17.034674F * 3.2f,  0.000000F * 3.2f},
		{20.000000F * 3.2f,   0.984266F * 3.2f, -0.000000F * 3.2f},
		{20.000000F * 3.2f, -17.034674F * 3.2f, -0.000000F * 3.2f},
	},
	// GMD_GMK_PULLEY_TYPE_ROPE_TL
	{
		{ 0.000000F * 3.2f,  -1.696280F * 3.2f,  0.000000F * 3.2f},
		{20.000000F * 3.2f, -11.696259F * 3.2f,  0.000000F * 3.2f},
		{ 0.000000F * 3.2f,   0.262015F * 3.2f, -0.000000F * 3.2f},
		{20.000000F * 3.2f,  -9.737963F * 3.2f,  0.000000F * 3.2f},
	},
	// GMD_GMK_PULLEY_TYPE_ROPE_TR
	{
		{ 0.000000F * 3.2f,   0.262015F * 3.2f, -0.000000F * 3.2f},
		{20.000000F * 3.2f,  -9.737963F * 3.2f,  0.000000F * 3.2f},
		{ 0.000000F * 3.2f,  -1.696280F * 3.2f,  0.000000F * 3.2f},
		{20.000000F * 3.2f, -11.696259F * 3.2f,  0.000000F * 3.2f},
	},
	// GMD_GMK_PULLEY_TYPE_POLE_L
	{
		{-6.449916F * 3.2f, -17.000000F * 3.2f,  0.000000F * 3.2f},
		{ 0.000000F * 3.2f, -17.000000F * 3.2f,  0.000000F * 3.2f},
		{-6.449916F * 3.2f,   0.912988F * 3.2f, -0.000000F * 3.2f},
		{ 0.000000F * 3.2f,   0.912988F * 3.2f, -0.000000F * 3.2f},
	},
	// GMD_GMK_PULLEY_TYPE_POLE_R
	{
		{ 0.000000F * 3.2f, -17.000000F * 3.2f,  0.000000F * 3.2f},
		{ 6.449916F * 3.2f, -17.000000F * 3.2f,  0.000000F * 3.2f},
		{ 0.000000F * 3.2f,   0.912988F * 3.2f, -0.000000F * 3.2f},
		{ 6.449916F * 3.2f,   0.912988F * 3.2f, -0.000000F * 3.2f},
	},
};

// äeÉIÉuÉWÉFÉNÉgUVç¿ïW
const static NNS_TEXCOORD gm_gmk_pulley_tex[][4] = {
	// GMD_GMK_PULLEY_TYPE_ROPE_N
	{
		{0.215957F,  0.047000F},
		{0.215957F,  0.610000F},
		{0.742860F,  0.047000F},
		{0.742860F,  0.610000F},
	},
	// GMD_GMK_PULLEY_TYPE_ROPE_TL
	{
		{0.303349F,  0.125000F},
		{0.733542F,  0.125000F},
		{0.303083F,  0.071000F},
		{0.733542F,  0.071000F},
	},
	// GMD_GMK_PULLEY_TYPE_ROPE_TR
	{
		{0.303349F,  0.125000F},
		{0.733542F,  0.125000F},
		{0.303083F,  0.071000F},
		{0.733542F,  0.071000F},
	},
	// GMD_GMK_PULLEY_TYPE_POLE_L
	{
		{0.020000F,  0.790000F},
		{0.020000F,  0.950000F},
		{0.491000F,  0.790000F},
		{0.491000F,  0.950000F},
	},
	// GMD_GMK_PULLEY_TYPE_POLE_R
	{
		{0.506436F,  0.797227F},
		{0.506436F,  0.965386F},
		{0.975000F,  0.797227F},
		{0.975000F,  0.965386F},
	},
};

// 


#endif // GMD_GMK_PULLEY_USE_DRAW_SERVER

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkPulleyBuild
/*!
 *	ÉMÉ~ÉbÉN ääé‘ ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkPulleyBuild(void)
{
	gm_gmk_pulley_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_PULLEY_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_PULLEY_TEX),
								0/*draw_flag*/);
}

// ==========================================================================
// GmGmkPulleyFlush
/*!
 *	ÉMÉ~ÉbÉN ääé‘ ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkPulleyFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_PULLEY_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_pulley_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmGmkPulleyBaseInit
/*!
 *	ÉMÉ~ÉbÉN ääé‘ñ{ëÃ èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkPulleyBaseInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);

	pos_y -= GDM_GMK_PULLEY_POLE_YOFS << FX32_SHIFT;	// ääé‘É|Å[ÉãÉTÉCÉYïœçXÇ…î∫Ç§ÉIÉtÉZÉbÉgí≤êÆ

//	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_PULLEY_BASE");
	obj_work = GMM_ENEMY_CREATE_RIDE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_PULLEY_WORK), "GMK_PULLEY_BASE");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// SEÉnÉìÉhÉãèâä˙âª
	((GMS_GMK_PULLEY_WORK*)obj_work)->se_handle = NULL;

	// SE handle äJï˙ÇÃÇΩÇﬂÇ…ÅAdestructorÇÕêÍópèàóùÇ…íuÇ´ä∑Ç¶ÇÈ
	mtTaskChangeTcbDestructor(obj_work->tcb, gmGmkPulleyBaseExit);

	// ääé‘ñ{ëÃ
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_pulley_obj_3d_list[IDB_GMK_PULLEY_MDL_GMK_PULLEY_ZNO],
					&gmk_work->obj_3d);

	// ÉÇÅ[ÉVÉáÉìèâä˙âª
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, FALSE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_GMK_PULLEY_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);

	// ÉAÉNÉVÉáÉìê›íË
	ObjDrawObjectActionSet(obj_work, IDB_GMK_PULLEY_MTN_GMK_PULLEY_N_ZNM);

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ìñÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT;
	gmk_work->ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NOFREE_DIFF_DATA;	// diff_dataÇäJï˙ÇµÇ»Ç¢

	// ãÈå`ê›íË
	gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppHit = NULL;
	rect_work->ppDef = gmGmkPulleyDefFunc;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	ObjRectWorkSet(rect_work,
						GMD_GMK_PULLEY_RECT_LF, GMD_GMK_PULLEY_RECT_UP,
						GMD_GMK_PULLEY_RECT_RT, GMD_GMK_PULLEY_RECT_DW);
	rect_work->flag |= OBD_RECT_OUT;

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z;

	// ÉÅÉCÉìèàóù
	obj_work->ppFunc = NULL;

	// ääé‘ÉvÅ[ÉäÅ[(éq)ê∂ê¨
	{
		OBS_OBJECT_WORK	*rot_work;
		GMS_EFFECT_3DNN_WORK	*efct_work;
//		rot_work = GMM_ENEMY_CREATE_WORK(NULL, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_PULLEY_ROT");
//		rot_work->parent_obj = obj_work;
//		rot_work->view_out_ofst = obj_work->view_out_ofst;
//		gmk_work = (GMS_ENEMY_3D_WORK*)rot_work;
		rot_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_3DNN_WORK), obj_work/*parent_obj*/, 0/*sort_prio*/, "GMK_PULLEY_ROT");
		efct_work = (GMS_EFFECT_3DNN_WORK*)rot_work;
		// ääé‘ÉvÅ[ÉäÅ[
		ObjObjectCopyAction3dNNModel(rot_work,
						&gm_gmk_pulley_obj_3d_list[IDB_GMK_PULLEY_MDL_GMK_PULLEY_ROT_ZNO],
						&efct_work->obj_3d);
		rot_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ìñÇΩÇËñ≥Çµ
		rot_work->disp_flag |= OBD_DISP_NODIRFLIP;
		rot_work->disp_flag &= ~OBD_DISP_NODIR;
		rot_work->flag |= OBD_OBJECT_NOCLIP;						// âÊñ ÉNÉäÉbÉvÇÕÇµÇ»Ç¢ÅiêeÇ™éÄÇÒÇæÇÁéqÇ‡éÄñSÅj
//		efct_work->efct_com.col_work.obj_col.flag |= OBD_COLOBJ_NOFREE_DIFF_DATA;	// diff_dataÇäJï˙ÇµÇ»Ç¢
		rot_work->ppFunc = gmGmkPulleyRotMain;
	}

	// âŒâ‘ÉGÉtÉFÉNÉgÉ|ÉCÉìÉ^èâä˙âª
	((GMS_GMK_PULLEY_WORK*)obj_work)->efct_work = NULL;

	return (obj_work);
}

// ==========================================================================
// GmGmkPulleyPoleLInit
/*!
 *	ÉMÉ~ÉbÉN ääé‘É|Å[Éãç∂ èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkPulleyPoleLInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	pos_y -= GDM_GMK_PULLEY_POLE_YOFS << FX32_SHIFT;	// ääé‘É|Å[ÉãÉTÉCÉYïœçXÇ…î∫Ç§ÉIÉtÉZÉbÉgí≤êÆ

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_PULLEY_POLE_L");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ääé‘É|Å[Éãç∂
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_pulley_obj_3d_list[IDB_GMK_PULLEY_MDL_GMK_PULLEY_R_ZNO],
					&gmk_work->obj_3d);

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ìñÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->flag 		|= OBD_OBJECT_NOHIT;					// ãÈå`Ç†ÇΩÇËñ≥ÇµÅü
	gmk_work->ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NOFREE_DIFF_DATA;	// diff_dataÇäJï˙ÇµÇ»Ç¢

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	obj_work->ppFunc = NULL;

#if GMD_GMK_PULLEY_USE_DRAW_SERVER
	// ï`âÊÇÇ‹Ç∆ÇﬂÇÈÇΩÇﬂÇ…èoóÕÇïœçXÇ∑ÇÈÅB
	obj_work->ppOut = gmGmkPulleyDrawSetPoleL;
#endif // GMD_GMK_PULLEY_USE_DRAW_SERVER

	return (obj_work);
}

// ==========================================================================
// GmGmkPulleyPoleRInit
/*!
 *	ÉMÉ~ÉbÉN ääé‘É|Å[ÉãâE èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkPulleyPoleRInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	pos_y -= GDM_GMK_PULLEY_POLE_YOFS << FX32_SHIFT;	// ääé‘É|Å[ÉãÉTÉCÉYïœçXÇ…î∫Ç§ÉIÉtÉZÉbÉgí≤êÆ

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_PULLEY_POLE_R");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ääé‘É|Å[ÉãâE
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_pulley_obj_3d_list[IDB_GMK_PULLEY_MDL_GMK_PULLEY_L_ZNO],
					&gmk_work->obj_3d);

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ìñÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->flag 		|= OBD_OBJECT_NOHIT;					// ãÈå`Ç†ÇΩÇËñ≥ÇµÅü
	gmk_work->ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NOFREE_DIFF_DATA;	// diff_dataÇäJï˙ÇµÇ»Ç¢

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	obj_work->ppFunc = NULL;

#if GMD_GMK_PULLEY_USE_DRAW_SERVER
	// ï`âÊÇÇ‹Ç∆ÇﬂÇÈÇΩÇﬂÇ…èoóÕÇïœçXÇ∑ÇÈÅB
	obj_work->ppOut = gmGmkPulleyDrawSetPoleR;
#endif // GMD_GMK_PULLEY_USE_DRAW_SERVER

	return (obj_work);
}

// ==========================================================================
// GmGmkPulleyRopeFInit
/*!
 *	ÉMÉ~ÉbÉN ääé‘ÉçÅ[ÉvêÖïΩ(Flat) èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkPulleyRopeFInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	pos_y -= GDM_GMK_PULLEY_POLE_YOFS << FX32_SHIFT;	// ääé‘É|Å[ÉãÉTÉCÉYïœçXÇ…î∫Ç§ÉIÉtÉZÉbÉgí≤êÆ

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_PULLEY_ROPE_F");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ääé‘ÉçÅ[ÉvêÖïΩ
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_pulley_obj_3d_list[IDB_GMK_PULLEY_MDL_GMK_PULLEY_ROPE_ZNO],
					&gmk_work->obj_3d);

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ìñÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->flag 		|= OBD_OBJECT_NOHIT;					// ãÈå`Ç†ÇΩÇËñ≥ÇµÅü
	gmk_work->ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NOFREE_DIFF_DATA;	// diff_dataÇäJï˙ÇµÇ»Ç¢

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	obj_work->ppFunc = NULL;

#if GMD_GMK_PULLEY_USE_DRAW_SERVER
	// ï`âÊÇÇ‹Ç∆ÇﬂÇÈÇΩÇﬂÇ…èoóÕÇïœçXÇ∑ÇÈÅB
	obj_work->ppOut = gmGmkPulleyDrawSetRopeN;
#endif // GMD_GMK_PULLEY_USE_DRAW_SERVER

	return (obj_work);
}

// ==========================================================================
// GmGmkPulleyRopeTInit
/*!
 *	ÉMÉ~ÉbÉN ääé‘ÉçÅ[ÉvéŒÇﬂ(Tilt) èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkPulleyRopeTInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	pos_y -= GDM_GMK_PULLEY_POLE_YOFS << FX32_SHIFT;	// ääé‘É|Å[ÉãÉTÉCÉYïœçXÇ…î∫Ç§ÉIÉtÉZÉbÉgí≤êÆ

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_PULLEY_ROPE_T");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ääé‘ÉçÅ[ÉvéŒÇﬂ
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_pulley_obj_3d_list[IDB_GMK_PULLEY_MDL_GMK_PULLEY_ROPE_SL_ZNO],
					&gmk_work->obj_3d);

	if (eve_rec->id == GMD_EVENT_ID_PULLEY_TROPE_TL) {
		// ç∂å¸Ç´ÇÃèÍçáï\é¶îΩì]
		obj_work->dir.y = 0x8000;
	}

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ìñÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->flag 		|= OBD_OBJECT_NOHIT;					// ãÈå`Ç†ÇΩÇËñ≥ÇµÅü
	gmk_work->ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NOFREE_DIFF_DATA;	// diff_dataÇäJï˙ÇµÇ»Ç¢

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	obj_work->ppFunc = NULL;

#if GMD_GMK_PULLEY_USE_DRAW_SERVER
	// ï`âÊÇÇ‹Ç∆ÇﬂÇÈÇΩÇﬂÇ…èoóÕÇïœçXÇ∑ÇÈÅB
	// ç∂å¸Ç´
	if (eve_rec->id == GMD_EVENT_ID_PULLEY_TROPE_TL) {
		obj_work->ppOut = gmGmkPulleyDrawSetRopeTL;
	}
	// âEå¸Ç´
	else {
		obj_work->ppOut = gmGmkPulleyDrawSetRopeTR;
	}
#endif // GMD_GMK_PULLEY_USE_DRAW_SERVER

	return (obj_work);
}

// ==========================================================================
// GmGmkPulleyDrawServerMain
/*!
 *	ÉMÉ~ÉbÉN ääé‘ï`âÊÉTÅ[Éo
 *
 *	@note åªç›ÇÕObjObject.cppë§Ç©ÇÁÉRÅ[ÉãÇ≥ÇÍÇƒÇ¢Ç‹Ç∑ÅB
 */
// ==========================================================================
void GmGmkPulleyDrawServerMain(void)
{
#if GMD_GMK_PULLEY_USE_DRAW_SERVER
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	GMS_GMK_PULLEY_MANAGER* mgr = &gm_gmk_pulley_manager;
	
	// ìoò^Ç™Ç»ÇØÇÍÇŒèIóπ
	if (mgr->num <= 0) {
		return;
	}
	
	// ÉvÉäÉ~ÉeÉBÉuÇ∆ÇµÇƒìoò^
	AMS_PARAM_DRAW_PRIMITIVE dat;
	
	// ÉxÅ[ÉXÉ}ÉgÉäÉbÉNÉXñ≥Çµ
	
	// ÉvÉäÉ~ÉeÉBÉuê›íË
	dat.type = NNE_PRIM_TRIANGLE_STRIP;
	
	dat.ablend = NNE_PRIM_ALPHABLEND_OFF;
	// ÉAÉãÉtÉ@ÉuÉåÉìÉhê›íË
#if defined(_PC) | defined(_XBOX)
	dat.bldSrc = NNE_BLENDMODE_SRCCOL;
	dat.bldDst = NNE_BLENDMODE_DSTCOL;
	dat.bldMode = NNE_BLENDOP_ADD;
#else
	dat.bldSrc = NND_BLENDFUNC_GL_SRC_COLOR;
	dat.bldDst = NND_BLENDFUNC_GL_DST_COLOR;
	dat.bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif
	// ÉeÉXÉgê›íË
	dat.aTest = 1;
	dat.zMask = 0;
	dat.zTest = 1;
	
	// É\Å[ÉgÇµÇ»Ç¢
	dat.noSort = 1;
	
	// ÉeÉNÉXÉ`ÉÉê›íË
	dat.texlist = mgr->texlist;
	dat.texId   = mgr->tex_id;
	
	// ÉJÉâÅ[ê›íË
	u32 color   = GmMainGetLightColor();
	
	// ÉeÉNÉXÉ`ÉÉÉNÉâÉìÉvê›íË
	dat.uwrap = NNE_PRIM_TEXWRAP_CLAMP;
	dat.vwrap = NNE_PRIM_TEXWRAP_CLAMP;
	
	// í∏ì_ÉfÅ[É^çÏê¨
	dat.count = (s32)(mgr->num * 6 - 2);
	NNS_PRIM3D_PCT* v_pos = (NNS_PRIM3D_PCT*)amDrawMallocDataBuffer((s32)(sizeof(NNS_PRIM3D_PCT) * dat.count));
	dat.vtxPCT3D = v_pos;
	dat.format3D = NNE_PRIM3D_FMT_PCT;
	
	for (u32 i = 0; i < mgr->num; i++) {
		NNS_PRIM3D_PCT* v = v_pos + i * 6; // ê›íËÇ∑ÇÈí∏ì_(î¬É|Éä4ì_ï™)
		NNS_PRIM3D_PCT* v_back = v_pos + (i * 6 - 1); // ê›íËÇ∑ÇÈí∏ì_(èkëﬁÉ|ÉäÉSÉìçÏê¨óp)
		NNS_PRIM3D_PCT* v_next = v_pos + (i * 6 + 4); // ê›íËÇ∑ÇÈí∏ì_(èkëﬁÉ|ÉäÉSÉìçÏê¨óp)
		GMS_GMK_PULLEY_REGISTER* reg = &mgr->reg[i];
		
		NNS_VECTOR    vec;
		
		// êÊÇ…åvéZ
		vec.x = FX_FX32_TO_F32(reg->vec.x);
		vec.y = FX_FX32_TO_F32(reg->vec.y);
		vec.z = FX_FX32_TO_F32(reg->vec.z);
		
		NNS_VECTOR*    pos = (NNS_VECTOR*)gm_gmk_pulley_pos[reg->type];
		NNS_TEXCOORD* tex = (NNS_TEXCOORD*)gm_gmk_pulley_tex[reg->type];

		// îΩì]ê›íË
		NNS_MATRIX mtx;
		if (reg->flip) {
			nnMakeRotateYMatrix(&mtx, reg->flip);
		}
		
		//float tmp_y = 1.0f;
		//if (reg->type == GMD_GMK_PULLEY_TYPE_POLE_L || reg->type == GMD_GMK_PULLEY_TYPE_POLE_R) {
		//	tmp_y = 0.0f;
		//}
		float tmp_y = 0.0f;
		if (reg->type == GMD_GMK_PULLEY_TYPE_ROPE_TR) {
			tmp_y = 2.0f;
		}
		
		// î¬É|Éäê›íË
		NNS_VECTOR temp_vector;
		for (int j = 0; j < 4; j++) {
			if (reg->flip) {
				nnTransformVector(&temp_vector, &mtx, &pos[j]);
			}
			else {
				nnCopyVector(&temp_vector, &pos[j]);
			}
			v[j].Pos.x = temp_vector.x + vec.x;
			v[j].Pos.y = temp_vector.y - vec.y + tmp_y;
			v[j].Pos.z = temp_vector.z + vec.z;
			v[j].Tex.u = tex[j].u;
			v[j].Tex.v = tex[j].v;
			v[j].Col   = color;
		}
		
		// èkëﬁÉ|ÉäÉSÉìê›íË
		if (i != 0) {
			*v_back = v[0];
		}
		if (i != mgr->num - 1) {
			*v_next = v[3];
		}
	}

	// ìoò^
	NNS_MATRIX unit_mtx;
	nnMakeUnitMatrix(&unit_mtx);

	amMatrixPush(&unit_mtx);
	ObjDraw3DNNDrawPrimitive(&dat);
	amMatrixPop();

	mgr->num = 0;
#endif // GMD_GMK_PULLEY_USE_DRAW_SERVER
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkPulleyBaseExit
/*!
 *	ÉMÉ~ÉbÉN ääé‘ èIóπèàóù
 *
 *	@param tcb		[in] tcbÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkPulleyBaseExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK*	obj_work  = (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_GMK_PULLEY_WORK* pul_work = (GMS_GMK_PULLEY_WORK*)obj_work;
	
	if (pul_work->se_handle) {
		GsSoundStopSeHandle(pul_work->se_handle);
		GsSoundFreeSeHandle(pul_work->se_handle);
		pul_work->se_handle = NULL;
	}
	
	// ÉGÉlÉ~Å[ïWèÄèIóπèàóù
	GmEnemyDefaultExit(tcb);
}

// ==========================================================================
// gmGmkPulleyDefFunc
/*!
 *	ÉMÉ~ÉbÉN ääé‘ ãÈå` Ç≠ÇÁÇ¢èàóù
 *
 *	@param mine_rect	[in] é©ï™Ç≠ÇÁÇ¢ãÈå`
 *	@param match_rect	[in] ëäéËçUåÇãÈå`
 *
 *	@note
 *		ppDefÇ…ìoò^\n
 */
// ==========================================================================
void gmGmkPulleyDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
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

	GmPlySeqInitPulley(ply_work, com_work);

	// ÉAÉNÉVÉáÉìïœçX
	ObjDrawObjectActionSet3DNN(&com_work->obj_work, IDB_GMK_PULLEY_MTN_GMK_PULLEY_ST_ZNM, 0);
	if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
		com_work->obj_work.dir.y = 0x8000;
	} else {
		com_work->obj_work.dir.y = 0;
	}
	com_work->obj_work.user_flag &= ~GMD_GMK_PULLEY_MOVE_STOP;
	((OBS_OBJECT_WORK*)com_work)->ppFunc = gmGmkPulleyMove;

	// É\ÉjÉbÉNÇ™åXéŒÇµÇƒÇ‡ãÈå`îªíËÇ©ÇÁäOÇÍÇ»Ç¢ÇÊÇ§Ç…ãÈå`ÉTÉCÉYägëÂ
	ObjRectWorkSet(&com_work->rect_work[GMD_ENEMY_RECT_BODY],
						GMD_GMK_PULLEY_RECT_LF * 8, GMD_GMK_PULLEY_RECT_UP,
						GMD_GMK_PULLEY_RECT_RT * 8, GMD_GMK_PULLEY_RECT_DW);

}


// ==========================================================================
// gmGmkPulleyMove
/*!
 *	ÉMÉ~ÉbÉN ääé‘à⁄ìÆ ÉÅÉCÉìä÷êî
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *
 *	@note
 *		eve_rec->left ÇÕà⁄ìÆÉuÉçÉbÉNêî
 */
// ==========================================================================
void gmGmkPulleyMove(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	GMS_PLAYER_WORK		*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	fx32	limit_l, limit_r, offset;

#if 1
	// ÉMÉ~ÉbÉNÉçÉbÉNíÜÅH
	if (ply_work->gmk_obj != obj_work) {
		// ÉçÉbÉNÇÕÇ∏ÇÍÇΩÅiÉWÉÉÉìÉvìôÇ≈ìríÜâ∫é‘Åj
		gmGmkPulleySecedeSet(obj_work, NULL);
		obj_work->flag |= OBD_OBJECT_NOHIT;								// ÇµÇŒÇÁÇ≠HITîªíËñ≥å¯
		obj_work->user_timer = GMD_GMK_PULLEY_REHIT_TIME;				// HITîªíËçƒäJÇ‹Ç≈ÇÃéûä‘
		return;
	}
#else
	if (!GmPlayerCheckGimmickEnable(ply_work)) {
		// ÉçÉbÉNÇÕÇ∏ÇÍÇΩ
		obj_work->dir.z = 0;
		gmk_work->ene_com.target_dp_dir.z = obj_work->dir.z;
		obj_work->ppFunc = NULL;
		return;
	}
#endif
	// é©ëRå∏ë¨
	if (obj_work->spd.x > 0) {
		obj_work->spd.x -= GMD_GMK_PULLEY_BRAKE;
		if (obj_work->spd.x < 0) {
			obj_work->spd.x = 0;
		}
	} else {
		obj_work->spd.x += GMD_GMK_PULLEY_BRAKE;
		if (obj_work->spd.x > 0) {
			obj_work->spd.x = 0;
		}
	}
	// èdóÕâ¡ë¨
	if (gmk_work->ene_com.eve_rec->flag & GMD_GMK_PULLEY_EVE_FLAG_TILT) {
		if (gmk_work->ene_com.eve_rec->flag & GMD_GMK_PULLEY_EVE_FLAG_LEFT) {
			obj_work->spd.x -= GMD_GMK_PULLEY_GRAVITY;
		} else {
			obj_work->spd.x += GMD_GMK_PULLEY_GRAVITY;
		}
	}

	// à⁄ìÆëÄçÏ
	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC) {
		// ÉXÉeÉBÉbÉNëÄçÏ
		Angle32	roll = MTM_MATH_CLIP(GmPlayerKeyGetGimmickRotZ(ply_work), -GMD_PL_DEF_ROLL_MAX, GMD_PL_DEF_ROLL_MAX);
		roll = roll * GMD_GMK_PULLEY_ACCEL / GMD_PL_DEF_ROLL_MAX;
		obj_work->spd.x += roll;
	} else {
		// åXéŒëÄçÏ
		Angle32	roll = MTM_MATH_CLIP(ply_work->key_rot_z, -GMD_PL_DEF_ROLL_MAX, GMD_PL_DEF_ROLL_MAX);
		roll = roll * GMD_GMK_PULLEY_ACCEL / GMD_PL_DEF_ROLL_MAX;
		obj_work->spd.x += roll;
	}

	// ë¨ìxèÛë‘ÇÃÉtÉâÉOÉZÉbÉg
	if (  (ply_work->act_state != GME_PLY_ACT_STATE_GMK_HANG_ACT)
		&&(obj_work->spd.x > (-FX32_ONE >> 4))
		&&(obj_work->spd.x < (FX32_ONE >> 4)) ) {
		obj_work->user_flag |= GMD_GMK_PULLEY_MOVE_STOP;
	}
	
	// ë¨ìxÇ…âûÇ∂ÇΩÉvÉåÉCÉÑÅ[ÉAÉNÉVÉáÉìïœçX
	{
		GME_PLY_ACT_STATE	act_id;
		s32					act_id_p;
		if (!(  (ply_work->act_state == GME_PLY_ACT_STATE_GMK_HANG_ACT)
			  ||(ply_work->act_state == GME_PLY_ACT_STATE_GMK_HANG_ST) )
			||(ply_work->obj_work.disp_flag & OBD_DISP_END) ) {
			
			if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
				// ç∂å¸Ç´
//				if (obj_work->spd.x > FX32_ONE) {
//					act_id = GME_PLY_ACT_STATE_GMK_HANG_B;
//					act_id_p = IDB_GMK_PULLEY_MTN_GMK_PULLEY_B_ZNM;
//				} else if (obj_work->spd.x < -FX32_ONE) {
//					act_id = GME_PLY_ACT_STATE_GMK_HANG_F;
//					act_id_p = IDB_GMK_PULLEY_MTN_GMK_PULLEY_F_ZNM;
//				} else {
					act_id = GME_PLY_ACT_STATE_GMK_HANG;
					act_id_p = IDB_GMK_PULLEY_MTN_GMK_PULLEY_N_ZNM;
//				}
			} else {
				// âEå¸Ç´
//				if (obj_work->spd.x > FX32_ONE) {
//					act_id = GME_PLY_ACT_STATE_GMK_HANG_F;
//					act_id_p = IDB_GMK_PULLEY_MTN_GMK_PULLEY_F_ZNM;
//				} else if (obj_work->spd.x < -FX32_ONE) {
//					act_id = GME_PLY_ACT_STATE_GMK_HANG_B;
//					act_id_p = IDB_GMK_PULLEY_MTN_GMK_PULLEY_B_ZNM;
//				} else {
					act_id = GME_PLY_ACT_STATE_GMK_HANG;
					act_id_p = IDB_GMK_PULLEY_MTN_GMK_PULLEY_N_ZNM;
//				}
			}
//			if (  (obj_work->spd.x != 0)
			if ( (  (obj_work->spd.x < (-FX32_ONE >> 4))
				  ||(obj_work->spd.x > (FX32_ONE >> 4)) )
				&&(obj_work->user_flag & GMD_GMK_PULLEY_MOVE_STOP) ) {
				// ÉäÉXÉ^Å[Ég
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_HANG_ACT);
				ObjDrawObjectActionSet3DNN(obj_work, IDB_GMK_PULLEY_MTN_GMK_PULLEY_ACT_ZNM, 0);
				obj_work->user_flag &= ~GMD_GMK_PULLEY_MOVE_STOP;
			} else 
			if (ply_work->act_state != act_id) {
				GmPlayerActionChange(ply_work, act_id);
				ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

				ObjDrawObjectActionSet3DNN(obj_work, act_id_p, 0);
				obj_work->disp_flag |= OBD_DISP_REPEAT;
			}
		}
	}
	
	// ë¨ìxÇ…âûÇ∂ÇΩâÒì]
	obj_work->dir.z = (u16)(MTM_MATH_CLIP((s16)(obj_work->spd.x / 4), -GMD_GMK_PULLEY_ANGLE, GMD_GMK_PULLEY_ANGLE));
	gmk_work->ene_com.target_dp_dir.z = obj_work->dir.z;
	
	if (gmk_work->ene_com.eve_rec->flag & GMD_GMK_PULLEY_EVE_FLAG_LEFT) {
		// ç∂å¸Ç´à⁄ìÆâ¬ìÆîÕàÕ
		limit_l = gmk_work->ene_com.born_pos_x - (gmk_work->ene_com.eve_rec->left * 64 * FX32_ONE);
		limit_r = gmk_work->ene_com.born_pos_x;
	} else {
		// âEå¸Ç´à⁄ìÆâ¬ìÆîÕàÕ
		limit_l = gmk_work->ene_com.born_pos_x;
		limit_r = gmk_work->ene_com.born_pos_x + (gmk_work->ene_com.eve_rec->left * 64 * FX32_ONE);
	}

	// ç∂âEèIíÖÉ`ÉFÉbÉN
	if (obj_work->pos.x < limit_l) {
		// ç∂í[ìûíÖÅió£íEÉZÉbÉgÅj
		if (ply_work->obj_work.pos.x > (limit_l + (8 << FX32_SHIFT))) {
			ply_work->obj_work.pos.x = limit_l + (8 << FX32_SHIFT);		// çÇë¨à⁄ìÆéûÇÃç¿ïWÇ∏ÇÍÇï‚ê≥
		}
		gmGmkPulleySonicTakeOffSet(ply_work, obj_work->spd.x);			// É\ÉjÉbÉNèÛë‘ÉZÉbÉg
		gmGmkPulleySecedeSet(obj_work, limit_l);						// ääé‘èÛë‘ÉZÉbÉg
		obj_work->user_timer = 0;										// HITîªíËçƒäJÇ‹Ç≈ÇÃéûä‘

	} else if (obj_work->pos.x > limit_r) {
		// âEí[ìûíÖÅió£íEÉZÉbÉgÅj
		if (ply_work->obj_work.pos.x < (limit_r - (8 << FX32_SHIFT))) {
			ply_work->obj_work.pos.x = limit_r - (8 << FX32_SHIFT);		// çÇë¨à⁄ìÆéûÇÃç¿ïWÇ∏ÇÍÇï‚ê≥
		}
		gmGmkPulleySonicTakeOffSet(ply_work, obj_work->spd.x);			// É\ÉjÉbÉNèÛë‘ÉZÉbÉg
		gmGmkPulleySecedeSet(obj_work, limit_r);						// ääé‘èÛë‘ÉZÉbÉg
		obj_work->user_timer = 0;										// HITîªíËçƒäJÇ‹Ç≈ÇÃéûä‘
	}

	// ÉGÉtÉFÉNÉgî≠ê∂êßå‰
	{
		if (  (obj_work->ppFunc == gmGmkPulleyMove)
			&&(MTM_MATH_ABS(obj_work->spd.x) > FX32_ONE) ) {
			gmGmkPulleySparkInit(obj_work);
		} else {
			gmGmkPulleySparkKill(obj_work);
		}
	}

	ObjObjectMove( obj_work );

	if (gmk_work->ene_com.eve_rec->flag & GMD_GMK_PULLEY_EVE_FLAG_TILT) {
		// éŒÇﬂà⁄ìÆéûÇÃÇxà íuéZèo
		offset = MTM_MATH_ABS(gmk_work->ene_com.born_pos_x - obj_work->pos.x);
		offset = offset / 2;
		obj_work->pos.y = gmk_work->ene_com.born_pos_y + offset;
	}
}

// ==========================================================================
// gmGmkPulleySonicTakeOffSet
/*!
 *	ÉMÉ~ÉbÉN ääé‘ É\ÉjÉbÉNÇÃó£íEà⁄çs
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *	@param spd_x	[in] éÀèoéûÇÃääé‘ë¨ìx
 *
 */
// ==========================================================================
void gmGmkPulleySonicTakeOffSet(GMS_PLAYER_WORK *ply_work, fx32 spd_x)
{
	ply_work->obj_work.spd.y = 0;
	ply_work->obj_work.spd.z = 0;
	ply_work->obj_work.dir.z = 0;
	ply_work->obj_work.spd_m = spd_x;
	ply_work->obj_work.spd.y = GMD_GMK_PULLEY_PUT_YSPD;

	GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_JUMP);
	// éÀèoéûç≈í·ë¨ìxÅEç≈çÇë¨ìxçƒê›íË
	if (spd_x > 0) {
		// âEà⁄ìÆ
		if (ply_work->obj_work.spd.x < GMD_GMK_PULLEY_PUT_XSPD_MIN) {
			ply_work->obj_work.spd.x = GMD_GMK_PULLEY_PUT_XSPD_MIN;
		} else if (ply_work->obj_work.spd.x > GMD_GMK_PULLEY_PUT_XSPD_MAX) {
			ply_work->obj_work.spd.x = GMD_GMK_PULLEY_PUT_XSPD_MAX;
		}
	} else {
		// ç∂à⁄ìÆ
		if (ply_work->obj_work.spd.x > -GMD_GMK_PULLEY_PUT_XSPD_MIN) {
			ply_work->obj_work.spd.x = -GMD_GMK_PULLEY_PUT_XSPD_MIN;
		} else if (ply_work->obj_work.spd.x < -GMD_GMK_PULLEY_PUT_XSPD_MAX) {
			ply_work->obj_work.spd.x = -GMD_GMK_PULLEY_PUT_XSPD_MAX;
		}
	}
	ply_work->obj_work.spd.x = ply_work->obj_work.spd.x / 2;
	ply_work->obj_work.spd.y = GMD_GMK_PULLEY_PUT_YSPD;

	GmPlySeqSetJumpState(ply_work,
						0,											// FALLåvéZÇçsÇÌÇ»Ç¢éûä‘
						GMD_PLY_SEQ_SETJUMPSTATE_IGNORE_JUMPBTN		// ÉWÉÉÉìÉvÉ{É^Éìñ≥éã
						| GMD_PLY_SEQ_SETJUMPSTATE_GMK_JUMP			// ÉMÉ~ÉbÉNÉWÉÉÉìÉv
//						| GMD_PLY_SEQ_SETJUMPSTATE_NOHOMING			// ÉzÅ[É~ÉìÉOïsâ¬
//						| GMD_PLY_SEQ_SETJUMPSTATE_NOJUMPMOVE);		// ÉWÉÉÉìÉvíÜà⁄ìÆïsâ¬Ç…
						| GMD_PLY_SEQ_SETJUMPSTATE_NOHOMING);		// ÉzÅ[É~ÉìÉOïsâ¬
}

// ==========================================================================
// gmGmkPulleySecedeSet
/*!
 *	ÉMÉ~ÉbÉN ääé‘É\ÉjÉbÉNó£íEå„ÇÃóhÇÍ Ç÷ÇÃà⁄çs
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *	@param pos_x	[in] ääé‘éwíËXç¿ïW(éwíËà íu(0à»äO)Ç»ÇÁç¿ïWÇÉZÉbÉg)
 *
 */
// ==========================================================================
void gmGmkPulleySecedeSet(OBS_OBJECT_WORK *obj_work, fx32 pos_x)
{
	GMS_ENEMY_3D_WORK	*gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	if (pos_x) {
		// ç¿ïWéwíËÇ™Ç†ÇÍÇŒÉZÉbÉg
		obj_work->pos.x = pos_x;
	}
	obj_work->spd.x = 0;
	obj_work->spd.y = 0;
	obj_work->spd_m = 0;
	obj_work->dir.z = 0;
	gmk_work->ene_com.target_dp_dir.z = obj_work->dir.z;
	ObjDrawObjectActionSet3DNN(obj_work, IDB_GMK_PULLEY_MTN_GMK_PULLEY_END_ZNM, 0);
	obj_work->ppFunc = gmGmkPulleySecede;

	// ãÈå`ÉTÉCÉYÇïWèÄâª
	ObjRectWorkSet(&gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY],
						GMD_GMK_PULLEY_RECT_LF, GMD_GMK_PULLEY_RECT_UP,
						GMD_GMK_PULLEY_RECT_RT, GMD_GMK_PULLEY_RECT_DW);

	// ÉGÉtÉFÉNÉgè¡ãé
	gmGmkPulleySparkKill(obj_work);

	// SE stop
	{
		GMS_GMK_PULLEY_WORK* pul_work = (GMS_GMK_PULLEY_WORK*)obj_work;
		if (pul_work->se_handle) {
			GsSoundStopSeHandle(pul_work->se_handle);
			GsSoundFreeSeHandle(pul_work->se_handle);
			pul_work->se_handle = NULL;
		}
	}
}
// ==========================================================================
// gmGmkPulleySecede
/*!
 *	ÉMÉ~ÉbÉN ääé‘É\ÉjÉbÉNó£íEå„ÇÃóhÇÍ ÉÅÉCÉìä÷êî
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *
 */
// ==========================================================================
void gmGmkPulleySecede(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->user_timer) {
		obj_work->user_timer--;
		if (!obj_work->user_timer) {
			// éwíËéûä‘Ç≈HITîªíËçƒäJ
			obj_work->flag &= ~OBD_OBJECT_NOHIT;
		}
	}

	if (obj_work->disp_flag & OBD_DISP_END) {
		ObjDrawObjectActionSet3DNN(obj_work, IDB_GMK_PULLEY_MTN_GMK_PULLEY_N_ZNM, 0);
		obj_work->disp_flag |= OBD_DISP_REPEAT;
		obj_work->ppFunc = NULL;
	}
}

// ==========================================================================
// gmGmkPulleyRotMain
/*!
 *	ääé‘Ç…ïtêèÇ∑ÇÈÉvÅ[ÉäÅ[ïî(éqÉ^ÉXÉN)êßå‰
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkPulleyRotMain(OBS_OBJECT_WORK *obj_work)
{
	OBS_OBJECT_WORK 	*parent_obj = obj_work->parent_obj;
	u16	dir_z;
	
	obj_work->pos = parent_obj->pos;
	dir_z = (u16)(parent_obj->spd.x >> 1);
	obj_work->dir.z += dir_z;
}

// ==========================================================================
// gmGmkPulleySparkInit
/*!
 *	ääé‘Ç…ïtêèÇ∑ÇÈâŒâ‘ÉGÉtÉFÉNÉgê∂ê¨
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkPulleySparkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	GMS_GMK_PULLEY_WORK *pul_work = (GMS_GMK_PULLEY_WORK*)obj_work;

	if (!pul_work->efct_work) {
		// ÉGÉtÉFÉNÉgê∂ê¨
		Angle16	rot_y = 0;
		Angle16	rot_z = 0;
		pul_work->efct_work = GmEfctZoneEsCreate(obj_work, GSD_MAIN_ZONE_TYPE_1, GME_EFCT_Z01_IDX_SPARK_K);
		if (obj_work->spd.x < 0) {
			rot_z = (u16)0xc000;
			GmComEfctAddDispOffsetF(pul_work->efct_work, 3.0f, 0, 0);
		}
		if (gmk_work->ene_com.eve_rec->flag & GMD_GMK_PULLEY_EVE_FLAG_TILT) {
			if (gmk_work->ene_com.eve_rec->flag & GMD_GMK_PULLEY_EVE_FLAG_LEFT) {
				rot_z += (Angle16)(0x10000 * 26.565 / 360);
			} else {
				rot_z += (Angle16)(-0x10000 * 26.565 / 360);
			}
		}
		GmComEfctAddDispRotationS(pul_work->efct_work, 0, rot_y, rot_z);


		// SEçƒê∂
		{
			GMS_GMK_PULLEY_WORK* pul_work = (GMS_GMK_PULLEY_WORK*)obj_work;

			if (!pul_work->se_handle) {
				// handleñ¢éÊìæÇ»ÇÁêVãKÇ≈çƒê∂
				pul_work->se_handle = GsSoundAllocSeHandle();
				GmSoundPlaySE("Pulley", pul_work->se_handle);
			} else {
// É|Å[ÉYÅEÉäÉWÉÖÅ[ÉÄÇ≈ÇÃêßå‰Çîpé~
//				// handleéÊìæçœÇ›Ç»ÇÁÉäÉWÉÖÅ[ÉÄ
//				GsSoundResumeSeHandle(pul_work->se_handle);
				// handleéÊìæçœÇ›Ç»ÇÁÉ{ÉäÉÖÅ[ÉÄÉZÉbÉg
				pul_work->se_handle->snd_ctrl_param.volume = 1.0f;
			}
		}
	}
}
// ==========================================================================
// gmGmkPulleySparkKill
/*!
 *	ääé‘Ç…ïtêèÇ∑ÇÈâŒâ‘ÉGÉtÉFÉNÉgè¡ãé
 *
 */
// ==========================================================================
void gmGmkPulleySparkKill(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PULLEY_WORK *pul_work = (GMS_GMK_PULLEY_WORK*)obj_work;

	if (pul_work->efct_work) {
		// ÉGÉtÉFÉNÉgçÌèú
		ObjDrawKillAction3DES((OBS_OBJECT_WORK*)pul_work->efct_work);
		pul_work->efct_work = NULL;

		// SEí‚é~
		{
			GMS_GMK_PULLEY_WORK* pul_work = (GMS_GMK_PULLEY_WORK*)obj_work;
// É|Å[ÉYÅEÉäÉWÉÖÅ[ÉÄÇ≈ÇÃêßå‰Çîpé~
//			GsSoundPauseSeHandle(pul_work->se_handle);
			// É{ÉäÉÖÅ[ÉÄÉ[Éç
			pul_work->se_handle->snd_ctrl_param.volume = 0.0f;
		}
	}
}


#if GMD_GMK_PULLEY_USE_DRAW_SERVER
// ==========================================================================
// gmGmkPulleyDrawSetRopeN
/*!
 *	í èÌÉçÅ[ÉvÇÃï`âÊìoò^
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
static void gmGmkPulleyDrawSetRopeN(OBS_OBJECT_WORK *obj_work)
{
	gmGmkPulleyDrawSetObject(obj_work, GMD_GMK_PULLEY_TYPE_ROPE_N);
}

// ==========================================================================
// gmGmkPulleyDrawSetRopeTL
/*!
 *	éŒÇﬂÉçÅ[Év(ç∂å¸Ç´)ÇÃï`âÊìoò^
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
static void gmGmkPulleyDrawSetRopeTL(OBS_OBJECT_WORK *obj_work)
{
	gmGmkPulleyDrawSetObject(obj_work, GMD_GMK_PULLEY_TYPE_ROPE_TL);
}

// ==========================================================================
// gmGmkPulleyDrawSetRopeTR
/*!
 *	éŒÇﬂÉçÅ[Év(âEå¸Ç´)ÇÃï`âÊìoò^
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
static void gmGmkPulleyDrawSetRopeTR(OBS_OBJECT_WORK *obj_work)
{
	gmGmkPulleyDrawSetObject(obj_work, GMD_GMK_PULLEY_TYPE_ROPE_TR);
}

// ==========================================================================
// gmGmkPulleyDrawSetPoleL
/*!
 *	É|Å[Éãç∂å¸Ç´ÇÃï`âÊìoò^
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
static void gmGmkPulleyDrawSetPoleL(OBS_OBJECT_WORK *obj_work)
{
	gmGmkPulleyDrawSetObject(obj_work, GMD_GMK_PULLEY_TYPE_POLE_L);
}

// ==========================================================================
// gmGmkPulleyDrawSetPoleR
/*!
 *	É|Å[ÉãâEå¸Ç´ÇÃï`âÊìoò^
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
static void gmGmkPulleyDrawSetPoleR(OBS_OBJECT_WORK *obj_work)
{
	gmGmkPulleyDrawSetObject(obj_work, GMD_GMK_PULLEY_TYPE_POLE_R);
}

// ==========================================================================
// gmGmkPulleyDrawSetObject
/*!
 *	ääé‘ÉIÉuÉWÉFÉNÉgÇÃï`âÊìoò^
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *	@param type	[in] ääé‘ÉIÉuÉWÉFÉNÉgÉ^ÉCÉv
 */
// ==========================================================================
static void gmGmkPulleyDrawSetObject(OBS_OBJECT_WORK *obj_work, GME_GMK_PULLEY_TYPE type)
{
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}
	
	GMS_GMK_PULLEY_MANAGER* mgr = &gm_gmk_pulley_manager;
	
	MTM_ASSERT(mgr->num < GMD_GMK_PULLEY_REGISTER_NUM);
	
	// ìoò^
	mgr->texlist = obj_work->obj_3d->texlist;
	mgr->tex_id  = 0;
	GMS_GMK_PULLEY_REGISTER* reg = &mgr->reg[mgr->num];
	reg->type = (u16)type;
	reg->flip = (u16)obj_work->dir.y;
	reg->vec  = obj_work->pos;
	
	// ëùâ¡
	++mgr->num;
}
#endif // GMD_GMK_PULLEY_USE_DRAW_SERVER




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
