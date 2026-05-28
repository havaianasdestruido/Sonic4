// ==========================================================================
/*!
  @file gmGmkSsSquare.cpp
  @brief ÉMÉ~ÉbÉNSpecialStageéläpíå

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSsSquare.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmGmkSsSquare.cpp,v $
 */

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

// ÉfÅ[É^ÉwÉbÉ_
#include "common/model/GMK_SS_SQUARE_MDL.HMB"
#include "common/model/GMK_SS_SQUARE_MAT.HMB"

#if _IPHONE
// commonÇ∆ç\ê¨Ç™ïœÇÌÇ¡ÇƒÇ¢ÇÈèÍçáÇ…égópÇ∑ÇÈíËã`
#include "iphone/model/GMK_SS_SQUARE_TVX.HMB"
#endif // _IPHONE

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------

#define GMD_GMK_SS_SQUARE_TEST_TVX       (1 & _IPHONE)

// GMS_EVE_RECORD_EVENT : flag
#define GMD_GMK_SS_SQR_EVE_FLAG_COL_MASK	(0x03)	//!< éläpíåêFÉ^ÉCÉv
#define GMD_GMK_SS_SQR_EVE_FLAG_NUM_MASK	(0x07)	//!< éläpíåêFîÕàÕÉ}ÉXÉN

#if GMD_GMK_SS_SQUARE_TEST_TVX
#define GMD_GMK_SS_SQUARE_UV_WAIT_FRAME		( 4)	//!< UVê›íËílï\é¶ÉEÉFÉCÉg
#define GMD_GMK_SS_SQUARE_UV_NUM			(30)	//!< UVê›íËílëçêî

#define GMD_GMK_SS_SQUARE_UV_WAIT_UPDATE_SHIFT		(   3)	//!< UV_WAITê›íËäJénBitÉVÉtÉgíl
#define GMD_GMK_SS_SQUARE_UV_WAIT_UPDATE_MASK		(0x7F)	//!< UV_WAITê›íËäJénÉ}ÉXÉN(0Å`127)
#define GMD_GMK_SS_SQUARE_UV_WAIT_GET_SHIFT			(   5)	//!< UV_WAITì¸éËäJénBitÉVÉtÉgíl
#define GMD_GMK_SS_SQUARE_UV_WAIT_GET_MASK			(0x1F)	//!< UV_WAITì¸éËäJénÉ}ÉXÉN(0Å`31)
#endif // GMD_GMK_SS_SQUARE_TEST_TVX

/// É}ÉeÉäÉAÉãÉÜÅ[ÉUÅ[ÉfÅ[É^ê›íË
enum {
	GMD_GMK_SS_SQR_MAT_USER_DATA_BLUE	= 1,
	GMD_GMK_SS_SQR_MAT_USER_DATA_YELLOW,
	GMD_GMK_SS_SQR_MAT_USER_DATA_PURPLE,
	GMD_GMK_SS_SQR_MAT_USER_DATA_GREEN,

	GMD_GMK_SS_SQR_MAT_USER_DATA_END,

};

#if !GMD_GMK_SS_SQUARE_TEST_TVX
/// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNÉÜÅ[ÉUÅ[ÉpÉâÉÅÅ[É^
typedef struct tag_GMS_GMK_SS_SQR_MAT_CB_PARAM {
	u32		draw_id;		//!< ï`âÊÇ∑ÇÈID
} GMS_GMK_SS_SQR_MAT_CB_PARAM;
#endif // !GMD_GMK_SS_SQUARE_TEST_TVX

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkSsSquareMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkSsSquareDrawFunc(OBS_OBJECT_WORK *obj_work);
#if !GMD_GMK_SS_SQUARE_TEST_TVX
static NNE_BOOL gmGmkSsSquareMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param);
#endif // !GMD_GMK_SS_SQUARE_TEST_TVX
static VecFx32 gmGmkSsSquareNormalizeVectorXY( const VecFx32* vec );

//----- Global Variables ----------------------------------------------------
/*** ínå`äpìxÉfÅ[É^ ***/
// 8*8 åªèÛ 9ÉLÉÉÉâ
/// ínå`äpìxèÓïÒ
u8 NNM_ALIGN_VC(4) NNM_ALIGN_CW(4) g_gm_ss_parts_col[] = {
#if 0
	// 1íiñ⁄
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18, 0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18, 0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	// 2íiñ⁄
	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,
	// 3íiñ⁄
	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,
	// 4íiñ⁄
	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,
	// 5íiñ⁄
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x98,0x98,0x98,0x98,0x98,0x98,0x98,0x98, 0x98,0x98,0x98,0x98,0x98,0x98,0x98,0x98, 0x98,0x98,0x98,0x98,0x98,0x98,0x98,0x98, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
#else
	// 1íiñ⁄
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x74,0x74,0x74,0x74,0x74,0x74,0x74,0x74, 0x74,0x74,0x74,0x74,0x74,0x74,0x74,0x74, 0x74,0x74,0x74,0x74,0x74,0x74,0x74,0x74, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	// 2íiñ⁄
	0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x8F,0x8F,0x8F,0x8F,0x8F,0x8F,0x8F,0x8F,
	// 3íiñ⁄
	0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x8F,0x8F,0x8F,0x8F,0x8F,0x8F,0x8F,0x8F,
	// 4íiñ⁄
	0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88, 0x8F,0x8F,0x8F,0x8F,0x8F,0x8F,0x8F,0x8F,
	// 5íiñ⁄
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8, 0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8, 0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
#endif
};

/*
u8 NNM_ALIGN_VC(4) NNM_ALIGN_CW(4) g_gm_ss_parts_dir[] = {
#define	A_UP	0x00
#define	A_DW	0x80
#define	A_LF	0xc0
#define	A_RT	0x40
	A_LF, A_UP, A_RT,
	A_LF, A_UP, A_RT,
	A_LF, A_DW, A_RT,
};
*/
//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_ss_square_obj_3d_list = NULL;

#if GMD_GMK_SS_SQUARE_TEST_TVX
static AMS_AMB_HEADER* gm_gmk_ss_square_obj_tvx_list = NULL;

/// É}ÉeÉäÉAÉãÉJÉâÅ[(ÇUVÇ…ê›íËÇ∑ÇÈ)
static const NNS_TEXCOORD gm_gmk_ss_square_mat_color[GMD_GMK_SS_SQR_MAT_USER_DATA_END] = {
	{0.50f, 0.00f},		// BLUE
	{0.50f, 0.50f},		// YELLOW
	{0.00f, 0.00f},		// PURPLE
	{0.00f, 0.50f},		// GREEN
};

/// UVÉXÉNÉçÅ[ÉãílÉpÉâÉÅÅ[É^
static const u8 gm_gmk_ss_square_uv_parameter[GMD_GMK_SS_SQUARE_UV_NUM] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,
};
#endif // GMD_GMK_SS_SQUARE_TEST_TVX

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkSsSquareBuild
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éläpíå ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkSsSquareBuild(void)
{
	gm_gmk_ss_square_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_SQUARE_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_SQUARE_TEX),
								0/*draw_flag*/);
#if GMD_GMK_SS_SQUARE_TEST_TVX
	void* tvx = GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_SS_SQUARE_TVX );
	amBindConv((Uint8*)tvx);
	gm_gmk_ss_square_obj_tvx_list = (AMS_AMB_HEADER*)tvx;
#endif // GMD_GMK_SS_SQUARE_TEST_TVX
}

// ==========================================================================
// GmGmkSsSquareFlush
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éläpíå ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkSsSquareFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SS_SQUARE_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_ss_square_obj_3d_list, amb->file_num);
#if GMD_GMK_SS_SQUARE_TEST_TVX
	gm_gmk_ss_square_obj_tvx_list = NULL;
#endif // GMD_GMK_SS_SQUARE_TEST_TVX
}

// ==========================================================================
// GmGmkSsSquareInit
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éläpíå èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSsSquareInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_SS_SQUARE");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ê∂ë∂É^ÉXÉNêîÇ…ó]óTÇ™ñ≥Ç¢ÇÃÇ≈éÄñSîÕàÕÇêÿÇËãlÇﬂÇÈ
	obj_work->view_out_ofst = (s16)(obj_work->view_out_ofst - 128);
	
	// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_ss_square_obj_3d_list[IDB_GMK_SS_SQUARE_MDL_SS_SQUARE_ZNO],
					&gmk_work->obj_3d);

	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ínå`Ç†ÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;	// ÉâÉCÉgÇOñ≥å¯
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_1;	// ÉâÉCÉgÇPóLå¯

	// êFÉpÉâÉÅÅ[É^/ÉAÉNÉVÉáÉìÉpÉâÉÅÅ[É^Çuser_*Ç…ÉZÉbÉg
	obj_work->user_flag =
	obj_work->user_work = (u32)((eve_rec->flag & GMD_GMK_SS_SQR_EVE_FLAG_COL_MASK)
									 + GMD_GMK_SS_SQR_MAT_USER_DATA_BLUE);	// êFÉ^ÉCÉv
	obj_work->user_timer = MTM_MATH_CLIP(eve_rec->left, 0, 8);				// êFä∑Ç¶à ëä

	// ÉÅÉCÉìèàóù
	obj_work->ppFunc = gmGmkSsSquareMain;
#if !GMD_GMK_SS_SQUARE_TEST_TVX
	// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNê›íË
	gmk_work->obj_3d.material_cb_func = gmGmkSsSquareMaterialCallback;

	{
		// É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
		ObjAction3dNNMaterialMotionLoad( &gmk_work->obj_3d,
		                                 0,				//reg_file_id
		                                 NULL,			//data_work
		                                 NULL,			//mtn_data_path
		                                 IDB_GMK_SS_SQUARE_MAT_SS_SQUARE_ZNV,
		                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_SS_SQUARE_MAT)->pData );
		ObjDrawObjectActionSet3DNNMaterial( obj_work, 0 );

		obj_work->disp_flag |= OBD_DISP_REPEAT;
	}
#endif // !GMD_GMK_SS_SQUARE_TEST_TVX
	// ï`âÊèàóùïœçX
	obj_work->ppOut = gmGmkSsSquareDrawFunc;

	// ínå`ê›íË
	{
		OBS_COLLISION_WORK *col_work = &gmk_work->ene_com.col_work;
	
		col_work->obj_col.obj		= obj_work;
		col_work->obj_col.diff_data	= (s8*)g_gm_default_col;			// è„â∫ç∂âEê⁄êGñ äpìxèÓïÒÇìæÇÈÇΩÇﬂÇ…ÇÕ
//		col_work->obj_col.dir_data	= (u8*)g_gm_ss_parts_dir;			// diff_data/dir_dataÇégópÇ∑ÇÈïKóvÇ†ÇË
		col_work->obj_col.width		= 3*8;								// ínå`ÉTÉCÉYê›íË(8dotíPà êßå¿Ç†ÇË)
		col_work->obj_col.height	= 3*8;								// êßñÒèúäOÇ∑ÇÈÇΩÇﬂÇ…ÇÕdiff_dataÇêÍópÇ≈éùÇ¬ïKóvÇ™Ç†ÇÈ
		col_work->obj_col.ofst_x	= (s16)(0 - col_work->obj_col.width / 2);
		col_work->obj_col.ofst_y	= (s16)(0 - col_work->obj_col.height / 2);
		col_work->obj_col.attr		= OBD_COL_DATA_ATTR_CLIFF;			// äRàµÇ¢
		col_work->obj_col.flag		|= OBD_COLOBJ_NOFREE_DIFF_DATA		// diff_dataÇäJï˙ÇµÇ»Ç¢
//									 | OBD_COLOBJ_NOFREE_DIR_DATA		// dir_dataÇäJï˙ÇµÇ»Ç¢
									 | OBD_COLOBJ_NODIR_PARENT;			// êeÇÃäpìxñ≥éã
	}

	return (obj_work);
}


// ==========================================================================
// GmGmkSsSquareBounce
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éläpíå É\ÉjÉbÉNÇíµÇÀï‘Ç∑
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *
 *	@note
 *	 Å¶ user_flag ÇÃ GMD_GMK_SS_SQR_FLAG_HIT ÇégópÇ∑ÇÈÇ±Ç∆Ç…íçà”
 */
// ==========================================================================
void GmGmkSsSquareBounce(OBS_OBJECT_WORK *obj_work)
{
#if 1 // ÉqÉbÉgÇ≈íµÇÀï‘Ç∑
	OBS_COLLISION_OBJ	*obj_col = &obj_work->col_work->obj_col;
	GMS_PLAYER_WORK		*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	GMS_SPL_STG_WORK	*spl_work = GmSplStageGetWork();
	
	if (obj_col->toucher_obj == &ply_work->obj_work) {
		// é©êgÇÃÉpÅ[ÉcÇ…ÉvÉåÉCÉÑÅ[Ç™HIT
		if ( (ply_work->nudge_timer)
			&&(!(spl_work->flag & GMD_SPL_STAGE_NUDGE_HIT)) ) {	// óhÇÁÇµÇPâÒÇ…Ç¬Ç´ÅAíµÇÀÇÈÇÃÇÕÇPâÒ
			// óhÇÁÇµÇ…ÇÊÇÈíµÇÀè„Ç™ÇË
// äpìxêßå¿Ç∑ÇÈÅH
//  è„ñ ÉqÉbÉgÇó}êßÇ∑ÇÈéËíiÇ∆ÇµÇƒÅAÉvÉåÉCÉÑÅ[Ç∆é©êgÇÃëäëŒäpìxÇ©ÇÁÉqÉbÉgÇó}êßÇ∑ÇÈÇ∆Ç©
//			fx32	dist_x, dist_y;
//			Angle32	target_angle;
//			dist_x = obj_work->pos.x - ply_work->obj_work.pos.x;
//			dist_y = obj_work->pos.y - ply_work->obj_work.pos.y;
//			target_angle = nnArcTan2(FX_FX32_TO_F32(dist_y), FX_FX32_TO_F32(dist_x));
//			target_angle += (u16)(-g_gm_main_system.pseudofall_dir);
//			target_angle += 0x4000;			//égópÇ∑ÇÈèÍçáÇÕílìôóvämîF
//			if (target_angle < 0x10000) {	//
			GmPlySeqInitPinballAir(
					ply_work,
					0,										// speed_x
					(fx32)(-0x00004400), 					// speed_y
					5,
					FALSE );
			spl_work->flag |= GMD_SPL_STAGE_BOUNCE_HIT;		// íµÇÀï‘ÇµÇΩÇ±Ç∆ÇëSëÃÇ÷í ím
			spl_work->flag |= GMD_SPL_STAGE_NUDGE_HIT;		// óhÇÁÇµÇ≈íµÇÀï‘ÇµÇΩÇ±Ç∆Ç‡í ím

		} else if (!(obj_work->user_flag & GMD_GMK_SS_SQR_FLAG_HIT)) {
			// í èÌÉqÉbÉg
			if ((!(spl_work->flag & GMD_SPL_STAGE_BOUNCE_HIT))
				&&(  (MTM_MATH_ABS(ply_work->obj_work.spd.x) > FX32_ONE)
				   ||(MTM_MATH_ABS(ply_work->obj_work.spd.y) > FX32_ONE) ) ) {
				// Ç†ÇÈíˆìxÇÃë¨ìxÇ≈ÉqÉbÉgÇµÇΩéûÇÃÇ›íµÇÀï‘Ç∑
				OBS_OBJECT_WORK		*ply_obj_work = &ply_work->obj_work;
				VecFx32 offset;
				offset.x = ply_obj_work->prev_pos.x - obj_work->pos.x;
				offset.y = ply_obj_work->prev_pos.y - obj_work->pos.y;
				offset.z = 0;

				offset = gmGmkSsSquareNormalizeVectorXY( &offset );

				//åXÇ´
				ply_obj_work->dir.z = 0;

				//ÉXÉsÅ[Éhê›íË
				fx32 speed_x, speed_y;
				fx32 speed;
				speed_x = MTM_MATH_ABS(ply_obj_work->spd.x);
				speed_y = MTM_MATH_ABS(ply_obj_work->spd.y);
				speed = FX_Sqrt(FX_Mul(speed_x, speed_x) + FX_Mul(speed_y, speed_y));
				speed = speed / 2;
				speed_x = FX_Mul( offset.x, speed );
				speed_y = FX_Mul( offset.y, speed );
//				speed_x = FX_Mul( offset.x, 2*FX32_ONE );	// àÍíËÉxÉNÉgÉãÇ≈íµÇÀï‘Ç∑èÍçá
//				speed_y = FX_Mul( offset.y, 2*FX32_ONE );	// 

				GmPlySeqInitPinballAir(
						ply_work,
						speed_x, 
						speed_y, 
						5,
						FALSE );
				spl_work->flag |= GMD_SPL_STAGE_BOUNCE_HIT;		// íµÇÀï‘ÇµÇΩÇ±Ç∆ÇëSëÃÇ÷í ím
			}
		}
		obj_work->user_flag |= GMD_GMK_SS_SQR_FLAG_HIT;	// ìñÇΩÇ¡ÇΩÇ±Ç∆ÇãLâØ

	} else {
		obj_work->user_flag &=  ~GMD_GMK_SS_SQR_FLAG_HIT;	// ìñÇΩÇÁÇ»Ç©Ç¡ÇΩÇÁÉqÉbÉgèÓïÒÇè¡ãé
	}
#endif // ÉqÉbÉgÇ≈íµÇÀï‘Ç∑
}
//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkSsSquareMain
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éläpíå ÉÅÉCÉì
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsSquareMain(OBS_OBJECT_WORK *obj_work)
{
	// ÉXÉeÅ[ÉWÉNÉäÉAÇ≈é©éE(É^ÉXÉNêîÉIÅ[ÉoÅ[ëŒçÙ)
	if (GmSplStageGetWork()->flag & GMD_SPL_STAGE_RESULT) {				// ÉXÉeÅ[ÉWÉNÉäÉA
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		return;
	}

#if 1
#if GMD_GMK_SS_SQUARE_TEST_TVX
	// user timer äàóp
	u32 uv_timer = ((obj_work->user_timer >> GMD_GMK_SS_SQUARE_UV_WAIT_UPDATE_SHIFT) & GMD_GMK_SS_SQUARE_UV_WAIT_UPDATE_MASK) + 1; // (1 - 128)
	if (uv_timer >= GMD_GMK_SS_SQUARE_UV_WAIT_FRAME * GMD_GMK_SS_SQUARE_UV_NUM) {
		uv_timer = 0;
	}
	obj_work->user_timer = (obj_work->user_timer & 7) | ((uv_timer & GMD_GMK_SS_SQUARE_UV_WAIT_UPDATE_MASK) << GMD_GMK_SS_SQUARE_UV_WAIT_UPDATE_SHIFT);
#endif // GMD_GMK_SS_SQUARE_TEST_TVX
	// êFÉAÉjÉÅÅ[ÉVÉáÉìêßå‰
	if (obj_work->user_timer) {
		u32	timer1 = (u32)((obj_work->user_timer - 1) & 7);
		u32	timer2 = (u32)((timer1 + 2) & 7);
		u32 sync_timer = (g_gm_main_system.sync_time >> 3 )& 7;

		if (  (timer1 == sync_timer)
			||(timer2 == sync_timer) ) {
			obj_work->user_work = ((obj_work->user_flag - 2) & 3) + 1;		// -1 ÇµÇƒ 1Å`4Ç…é˚ÇﬂÇÈ
		} else {
			obj_work->user_work = obj_work->user_flag & GMD_GMK_SS_SQR_EVE_FLAG_NUM_MASK;
		}
	}
#else
	// ÉqÉbÉgÇµÇΩÇÁêFÇïœÇ¶ÇÈ(ÉfÉoÉbÉOóp)
	#define GMD_GMK_SQR_HIT_TEST
	OBS_COLLISION_OBJ	*obj_col = &obj_work->col_work->obj_col;
	GMS_PLAYER_WORK		*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	if (obj_col->toucher_obj == &ply_work->obj_work) {
//		if (obj_col->toucher_obj->touch_obj == obj_work) {
			// É\ÉjÉbÉNÇ™ÉqÉbÉg
			obj_work->user_work = ((obj_work->user_flag - 2) & 3) + 1;		// -1 ÇµÇƒ 1Å`4Ç…é˚ÇﬂÇÈ
		} else {
			// no hit
			obj_work->user_work = obj_work->user_flag & GMD_GMK_SS_SQR_EVE_FLAG_NUM_MASK;
//		}
	}
#endif

	// É\ÉjÉbÉNíµÇÀï‘Çµ
	GmGmkSsSquareBounce(obj_work);
}

// ==========================================================================
// gmGmkSsSquareNormalizeVectorXY
/*!
 *	ÉxÉNÉ^Çê≥ãKâª(XYç¿ïWÇÃÇ›ÅBZÇÕ0Ç…èâä˙âª)
 *
 *	@param vec	[in] É^Å[ÉQÉbÉg
 *
 *	@return ê≥ãKâªå„ÇÃÉ^Å[ÉQÉbÉg
 */
// ==========================================================================
VecFx32 gmGmkSsSquareNormalizeVectorXY( const VecFx32* vec )
{
	amAssert( vec );

	VecFx32 normal;

	fx32 length = FX_Mul(vec->x, vec->x) + FX_Mul(vec->y, vec->y);
	length = FX_Sqrt(length);
	if ( length == 0 ){
		normal.x = FX32_ONE;
		normal.y = 0;
	}
	else{
		fx32 r_length = FX_Div( FX32_ONE, length );
		normal.x = FX_Mul( vec->x, r_length );
		normal.y = FX_Mul( vec->y, r_length );
	}
	normal.z = 0;

	{
		// ÉXÉyÉXÉeéûÇÕâÊñ âÒì]Ç…çáÇÌÇπÇƒà⁄ìÆï˚å¸ï‚ê≥
		fx32 spd_x, spd_y;
		ObjUtilGetRotPosXY(normal.x, normal.y, &spd_x, &spd_y, (u16)(-g_gm_main_system.pseudofall_dir));
		normal.x = spd_x;
		normal.y = spd_y;
	}
	return normal;
}

// ==========================================================================
// gmGmkSsSquareDrawFunc
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éläpíå ï`âÊê›íËèàóù
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkSsSquareDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	OBS_ACTION3D_NN_WORK		*obj_3d;
	
#if GMD_GMK_SS_SQUARE_TEST_TVX
	if (!GmMainIsDrawEnable()) {
		return;
	}
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}
	void* tvx            = amBindGet(gm_gmk_ss_square_obj_tvx_list, IDB_MODEL_SS_SQUARE_TVX);
	NNS_TEXLIST* texlist = obj_work->obj_3d->texlist;
	VecFx32* pos         = &obj_work->pos;
	VecFx32* scale       = &obj_work->scale;
	
	// extend
	GMS_TVX_EX_WORK work;
	u32 uv_param = (u32)(obj_work->user_timer >> GMD_GMK_SS_SQUARE_UV_WAIT_GET_SHIFT) & GMD_GMK_SS_SQUARE_UV_WAIT_GET_MASK;
	
	work.u_wrap  = NNE_PRIM_TEXWRAP_CLAMP;
	work.v_wrap  = NNE_PRIM_TEXWRAP_CLAMP;
	work.coord.u = 0.125f * (float)(gm_gmk_ss_square_uv_parameter[uv_param] % 4) + gm_gmk_ss_square_mat_color[obj_work->user_work - 1].u;
	work.coord.v = 0.125f * (float)(gm_gmk_ss_square_uv_parameter[uv_param] / 4) + gm_gmk_ss_square_mat_color[obj_work->user_work - 1].v;
	work.color   = 0xffffffff;
	
	GmTvxSetModelEx(tvx, texlist, pos, scale, GMD_TVX_DISP_LIGHT_DISABLE, 0, &work);
#else
	GMS_GMK_SS_SQR_MAT_CB_PARAM	*ss_sqr_mat_cb_param;

	obj_3d		= obj_work->obj_3d;

	// É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉNê›íË
	ss_sqr_mat_cb_param = (GMS_GMK_SS_SQR_MAT_CB_PARAM *)amDrawMallocDataBuffer(sizeof(GMS_GMK_SS_SQR_MAT_CB_PARAM));
	ss_sqr_mat_cb_param->draw_id = obj_work->user_work;
	obj_3d->material_cb_param = ss_sqr_mat_cb_param;

	ObjDrawActionSummary(obj_work);
#endif  //  GMD_GMK_SS_SQUARE_TEST_TVX
}

#if !GMD_GMK_SS_SQUARE_TEST_TVX
// ==========================================================================
// gmGmkSsSquareMaterialCallback
/*!
 *	ÉMÉ~ÉbÉN SpecialStage éläpíå É}ÉeÉäÉAÉãÉRÅ[ÉãÉoÉbÉN(OBF_MATERIAL_CB)
 *
 *	@param	val		[in]	ÉhÉçÅ[ÉRÅ[ÉãÉoÉbÉNïœêî
 *	@param	param	[in]	ÉÜÅ[ÉUÅ[ÉpÉâÉÅÅ[É^
 */
// ==========================================================================
NNE_BOOL gmGmkSsSquareMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param)
{
	u32							user_data;
	GMS_GMK_SS_SQR_MAT_CB_PARAM	*user_param;

	if (param) {
		user_param = (GMS_GMK_SS_SQR_MAT_CB_PARAM*)param;

		// ÉÜÅ[ÉUÅ[ÉfÅ[É^éÊìæ
		user_data = ObjDraw3DNNGetMaterialUserData(val);

		if (!user_data ||
				user_data == user_param->draw_id) {
			return (nnPutMaterialCore(val));
		}

#if defined (MTD_DEBUG)
		if (user_data >= GMD_GMK_SS_SQR_MAT_USER_DATA_END) {
			// ÉpÉâÉÅÅ[É^É~ÉX
			MTM_ASSERT(0);
		}
#endif
	}

	return (NNE_FALSE);

	//return (nnPutMaterialCore(draw_cb_val));
}
#endif // !GMD_GMK_SS_SQUARE_TEST_TVX

//	// ==========================================================================
//	// _pt
//	/*!
//	 *	@param	tcb	[in]	TCB
//	 */
//	// ==========================================================================
