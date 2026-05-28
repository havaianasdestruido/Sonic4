// ==========================================================================
/*!
  @file gmGmkWaterSlider.cpp
  @brief ƒMƒ~ƒbƒN ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkWaterSlider.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 *	GMS_EVE_RECORD_EVENT
 *		left		: ‚È‚µ
 *		top			: ‚È‚µ
 *		width		: ‚È‚µ
 *		height		: ‚È‚µ
 *
 *		flag		: ‚È‚µ
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
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectZone.h"
#include "gmSound.h"
#include "gmPadVib.h"

#include "gmGmkWaterSlider.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "common/model/gmk_water_slider_mdl.hmb"
#include "common/model/gmk_water_slider_mat.hmb"
#include "common/model/gmk_water_slider_mtn.hmb"


//----- Definitions ---------------------------------------------------------

//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[ƒ‚[ƒh
enum GME_GMK_WATER_SLIDER_MODE{
	GMD_GMK_WATER_SLIDER_MODE_WAIT = 0,		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‘Ò‚¿
	GMD_GMK_WATER_SLIDER_MODE_START,		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[ŠJŽn
	GMD_GMK_WATER_SLIDER_MODE_ACTIVE,		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[ŽÀs
	GMD_GMK_WATER_SLIDER_MODE_STOP,			//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[I—¹

	GMD_GMK_WATER_SLIDER_MODE_NUM
};

//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[ƒ^ƒCƒv
enum GME_GMK_WATER_SLIDER_TYPE{
	GMD_GMK_WATER_SLIDER_TYPE_LEFT = 0,		//¶i^‰ºj
	GMD_GMK_WATER_SLIDER_TYPE_LEFT_30D,		//¶i30“xj
	GMD_GMK_WATER_SLIDER_TYPE_LEFT_45D,		//¶i45“xj
	GMD_GMK_WATER_SLIDER_TYPE_LEFT_60D,		//¶i60“xj

	GMD_GMK_WATER_SLIDER_TYPE_RIGHT,		//‰Ei^‰ºj
	GMD_GMK_WATER_SLIDER_TYPE_RIGHT_30D,	//‰Ei30“xj
	GMD_GMK_WATER_SLIDER_TYPE_RIGHT_45D,	//‰Ei45“xj
	GMD_GMK_WATER_SLIDER_TYPE_RIGHT_60D,	//‰Ei60“xj

	GMD_GMK_WATER_SLIDER_TYPE_NUM
};

#define GMD_GMK_WATER_SLIDER_RECT_MARGIN		(8)				//‹éŒ`ƒTƒCƒY‚Ìƒ}[ƒWƒ“
#define GMD_GMK_WATER_SLIDER_SPEED				(15*FX32_ONE)	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[ˆÚ“®—Ê
#define GMD_GMK_WATER_SLIDER_PLAYER_OFFSET_Y	(-2.0f)			//ƒvƒŒƒCƒ„•\Ž¦ƒIƒtƒZƒbƒg

#define GMD_GMK_WATER_SLIDER_INVALID_ID			(-1)	//–³ŒøID

//ƒ[ƒN
typedef struct tag_GMS_GMK_WATER_SLIDER_WORK{
	GMS_ENEMY_3D_WORK gimmick_work;	//ƒMƒ~ƒbƒNƒ[ƒN
	OBS_ACTION3D_NN_WORK obj_3d_parts;	//ƒp[ƒc
}GMS_GMK_WATER_SLIDER_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static u32 gmGmkWaterSlidereGameSystemGetSyncTime( void );
static GMS_ENEMY_3D_WORK* gmGmkWaterSliderLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_WATER_SLIDER_TYPE type );
static GMS_ENEMY_3D_WORK* gmGmkWaterSliderLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_WATER_SLIDER_TYPE type );

//---------------------------------------------------------
//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[
//---------------------------------------------------------
static BOOL gmGmkWaterSliderCheckHFlip( GME_GMK_WATER_SLIDER_TYPE type );
static void gmGmkWaterSliderInit( OBS_OBJECT_WORK *obj_work, GME_GMK_WATER_SLIDER_TYPE slider_type );
static void gmGmkWaterSliderSetRect( GMS_ENEMY_3D_WORK* gimmick_work, GME_GMK_WATER_SLIDER_TYPE slider_type );

static void gmGmkWaterSliderDestFunc( MTS_TASK_TCB *tcb );
static void gmGmkWaterSliderDefFunc( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* target_rect );
static void gmGmkWaterSliderDrawFunc( OBS_OBJECT_WORK* work );

static void gmGmkWaterSliderMainActive( OBS_OBJECT_WORK *obj_work );

//---------------------------------------------------------
//ƒGƒtƒFƒNƒg
//---------------------------------------------------------
static void gmGmkWaterSliderEffectMainFunc( OBS_OBJECT_WORK *obj_work );
static void gmGmkWaterSliderEffectDestFunc( MTS_TASK_TCB *tcb );
//---------------------------------------------------------
//ƒ†[ƒUƒ[ƒN
//---------------------------------------------------------
static void gmGmkWaterSliderSetUserWorkSlideType( OBS_OBJECT_WORK* obj_work, GME_GMK_WATER_SLIDER_TYPE type );
static GME_GMK_WATER_SLIDER_TYPE gmGmkWaterSliderGetUserWorkSlideType( OBS_OBJECT_WORK* obj_work );
static void gmGmkWaterSliderSetUserTimerSlideSpeed( OBS_OBJECT_WORK* obj_work, fx32 speed );
static fx32 gmGmkWaterSliderGetUserTimerSlideSpeed( OBS_OBJECT_WORK* obj_work );

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//---------------------------------------------------------
//mainƒp[ƒc
//---------------------------------------------------------
//ƒ‚ƒfƒ‹ID
static const s32 g_gm_gmk_water_slider_model_id_main[GMD_GMK_WATER_SLIDER_TYPE_NUM] = {
	IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_ZNO,
	IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_30D_ZNO,
	IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_45D_ZNO,
	IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_60D_ZNO,
	IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_ZNO,
	IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_30D_ZNO,
	IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_45D_ZNO,
	IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_60D_ZNO,
};

//ƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“ID
static const s32 g_gm_gmk_water_slider_material_id_main[GMD_GMK_WATER_SLIDER_TYPE_NUM] = {
	IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_ZNV,
	IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_30D_ZNV,
	IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_45D_ZNV,
	IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_60D_ZNV,
	IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_ZNV,
	IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_30D_ZNV,
	IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_45D_ZNV,
	IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_60D_ZNV,	
};

//---------------------------------------------------------
//subƒp[ƒc
//---------------------------------------------------------
//ƒ‚ƒfƒ‹ID
static const s32 g_gm_gmk_water_slider_model_id_sub[GMD_GMK_WATER_SLIDER_TYPE_NUM] = {
	GMD_GMK_WATER_SLIDER_INVALID_ID,
	IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_30D_N_ZNO,
	IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_45D_N_ZNO,
	IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_60D_N_ZNO,
	GMD_GMK_WATER_SLIDER_INVALID_ID,
	IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_30D_N_ZNO,
	IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_45D_N_ZNO,
	IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_60D_N_ZNO,
};

//ƒ‚[ƒVƒ‡ƒ“ID
static const s32 g_gm_gmk_water_slider_motion_id_sub[GMD_GMK_WATER_SLIDER_TYPE_NUM] = {
	GMD_GMK_WATER_SLIDER_INVALID_ID,
	IDB_GMK_WATER_SLIDER_MTN_GMK_WATER_SLDR_30D_N_ZNM,
	IDB_GMK_WATER_SLIDER_MTN_GMK_WATER_SLDR_45D_N_ZNM,
	IDB_GMK_WATER_SLIDER_MTN_GMK_WATER_SLDR_60D_N_ZNM,
	GMD_GMK_WATER_SLIDER_INVALID_ID,
	IDB_GMK_WATER_SLIDER_MTN_GMK_WATER_SLDR_30D_N_ZNM,
	IDB_GMK_WATER_SLIDER_MTN_GMK_WATER_SLDR_45D_N_ZNM,
	IDB_GMK_WATER_SLIDER_MTN_GMK_WATER_SLDR_60D_N_ZNM,	
};

//ƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“ID
static const s32 g_gm_gmk_water_slider_material_id_sub[GMD_GMK_WATER_SLIDER_TYPE_NUM] = {
	GMD_GMK_WATER_SLIDER_INVALID_ID,
	IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_30D_N_ZNV,
	IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_45D_N_ZNV,
	IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_60D_N_ZNV,
	GMD_GMK_WATER_SLIDER_INVALID_ID,
	IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_30D_N_ZNV,
	IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_45D_N_ZNV,
	IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_60D_N_ZNV,	
};

static OBS_ACTION3D_NN_WORK* g_gm_gmk_water_slider_obj_3d_list = NULL;

static GMS_EFFECT_3DES_WORK* g_gm_gmk_water_slider_effct_player = NULL;
static GSS_SND_SE_HANDLE* g_gm_gmk_water_slider_se_handle = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkWaterSliderBuild
/*!
 *	ƒMƒ~ƒbƒN ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[ ƒf[ƒ^\’z
 */
// ==========================================================================
void GmGmkWaterSliderBuild(void)
{
	g_gm_gmk_water_slider_obj_3d_list = GmGameDBuildRegBuildModel(
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_WATER_SLIDER_MODEL ),
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_WATER_SLIDER_TEX ),
			0	//draw_flag
			);
}

// ==========================================================================
// GmGmkWaterSliderFlush
/*!
 *	ƒMƒ~ƒbƒN ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[ ƒf[ƒ^•Ð•t‚¯
 */
// ==========================================================================
void GmGmkWaterSliderFlush(void)
{
	AMS_AMB_HEADER* amb_header = (AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_WATER_SLIDER_MODEL );

	GmGameDBuildRegFlushModel( g_gm_gmk_water_slider_obj_3d_list, amb_header->file_num );
	g_gm_gmk_water_slider_obj_3d_list = NULL;
}

// ==========================================================================
// GmGmkWaterSliderInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ” ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] 
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkWaterSliderInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	UNREFERENCED_PARAMETER(type);

	//ƒ^ƒCƒv
	GME_GMK_WATER_SLIDER_TYPE slider_type;
	switch( eve_rec->id ){
	case GMD_EVENT_ID_WATER_SLIDER_L_30D:
		slider_type = GMD_GMK_WATER_SLIDER_TYPE_LEFT_30D;
		break;
	case GMD_EVENT_ID_WATER_SLIDER_L_45D:
		slider_type = GMD_GMK_WATER_SLIDER_TYPE_LEFT_45D;
		break;
	case GMD_EVENT_ID_WATER_SLIDER_L_60D:
		slider_type = GMD_GMK_WATER_SLIDER_TYPE_LEFT_60D;
		break;
	case GMD_EVENT_ID_WATER_SLIDER_R_30D:
		slider_type = GMD_GMK_WATER_SLIDER_TYPE_RIGHT_30D;
		break;
	case GMD_EVENT_ID_WATER_SLIDER_R_45D:
		slider_type = GMD_GMK_WATER_SLIDER_TYPE_RIGHT_45D;
		break;
	case GMD_EVENT_ID_WATER_SLIDER_R_60D:
		slider_type = GMD_GMK_WATER_SLIDER_TYPE_RIGHT_60D;
		break;
	case GMD_EVENT_ID_WATER_SLIDER_L:
	case GMD_EVENT_ID_WATER_SLIDER_R:
	default:
		amAssert(FALSE);
		return NULL;
	}

	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkWaterSliderLoadObj( eve_rec, pos_x, pos_y, slider_type );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK*obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkWaterSliderInit( obj_work, slider_type );
	return obj_work;
}

// ==========================================================================
// GmGmkWaterSliderGetObj3DList
/*!
 *	OBJ3DƒŠƒXƒg‚ðŽæ“¾i‘•ü‚Å“¯‚¶ƒ‚ƒfƒ‹‚ðŽg‚¢‚Ü‚í‚µ‚Ä‚¢‚é‚½‚ßj
 *
 *	@return OBJ3DƒŠƒXƒg
 */
// ==========================================================================
OBS_ACTION3D_NN_WORK* GmGmkWaterSliderGetObj3DList( void )
{
	return g_gm_gmk_water_slider_obj_3d_list;
}

// ==========================================================================
// GmGmkWaterSliderCreateEffect
/*!
 *	ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒgì¬
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkWaterSliderCreateEffect( void )
{
	//¶¬Ï‚Ý
	if ( !g_gm_gmk_water_slider_effct_player ){
		GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
		amAssert( player_work );
		OBS_OBJECT_WORK* player_obj_work = &player_work->obj_work;
		amAssert( player_obj_work );

		//ƒGƒtƒFƒNƒg
		g_gm_gmk_water_slider_effct_player = GmEfctZoneEsCreate(
				player_obj_work,
				GSD_MAIN_ZONE_TYPE_3,
				GME_EFCT_Z03_IDX_SLIDER_SONIC );

		//À•W
		OBS_OBJECT_WORK* effect_obj_work = (OBS_OBJECT_WORK*)g_gm_gmk_water_slider_effct_player;
		effect_obj_work->parent_ofst.z = GMD_OBJ_DEFAULT_POS_Z_C_FRONT;
		effect_obj_work->ppFunc = gmGmkWaterSliderEffectMainFunc;
		mtTaskChangeTcbDestructor( effect_obj_work->tcb, gmGmkWaterSliderEffectDestFunc );
	}

	//Œø‰Ê‰¹
	if ( !g_gm_gmk_water_slider_se_handle ){
		g_gm_gmk_water_slider_se_handle	= GsSoundAllocSeHandle();
		GmSoundPlaySE("WaterSlider", g_gm_gmk_water_slider_se_handle);
	}

	//U“®
	GMM_PAD_VIB_SMALL_TIME(30);

	return (OBS_OBJECT_WORK*)g_gm_gmk_water_slider_effct_player;
}

// ==========================================================================
// GmGmkWaterSliderDeleteEffect
/*!
 *	ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒgíœ
 *
 */
// ==========================================================================
void GmGmkWaterSliderDeleteEffect( void )
{
	//ƒGƒtƒFƒNƒg
	if ( g_gm_gmk_water_slider_effct_player ){
		g_gm_gmk_water_slider_effct_player->efct_com.obj_work.flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		g_gm_gmk_water_slider_effct_player = NULL;
	}
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkWaterSlidereGameSystemGetSyncTime
/*!
 * ƒQ[ƒ€ƒVƒXƒeƒ€‚©‚ç“¯ŠúŽžŠÔ‚ðŽæ“¾
 *
 *	@return	“¯ŠúŽžŠÔ
 */
// ==========================================================================
u32 gmGmkWaterSlidereGameSystemGetSyncTime( void )
{
	return g_gm_main_system.sync_time;
}


// ==========================================================================
// gmGmkWaterSliderLoadObjNoModel
/*!
 *	ƒMƒ~ƒbƒN“Ç‚Ýž‚Ýƒ‚ƒfƒ‹‚È‚µ
 *
 *	@param eve_rec	[in] ƒCƒxƒ“ƒgƒŒƒR[ƒh
 *	@param pos_x	[in] À•WX
 *	@param pos_y	[in] À•WY
 *	@param type		[in] ƒ^ƒCƒv
 *
 *	@return ƒ[ƒN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkWaterSliderLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_WATER_SLIDER_TYPE type )
{
	UNREFERENCED_PARAMETER(type);

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec, 
			pos_x, 
			pos_y, 
			sizeof(GMS_GMK_WATER_SLIDER_WORK), 
			"GMK_WATER_SLIDER");
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}

// ==========================================================================
// gmGmkWaterSliderLoadObj
/*!
 *	ƒMƒ~ƒbƒN“Ç‚Ýž‚Ý
 *
 *	@param eve_rec	[in] ƒCƒxƒ“ƒgƒŒƒR[ƒh
 *	@param pos_x	[in] À•WX
 *	@param pos_y	[in] À•WY
 *	@param type		[in] ƒ^ƒCƒv
 *
 *	@return ƒ[ƒN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkWaterSliderLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_WATER_SLIDER_TYPE type )
{
	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkWaterSliderLoadObjNoModel(
			eve_rec,
			pos_x,
			pos_y,
			type );
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//-------------------------------------------------
	// ƒ‚ƒfƒ‹‰Šú‰»
	//-------------------------------------------------
	s32 data_model_index = g_gm_gmk_water_slider_model_id_main[type];
	//“Ç‚Ýž‚Ý
	ObjObjectCopyAction3dNNModel(
		obj_work,
		&g_gm_gmk_water_slider_obj_3d_list[data_model_index],
		&gimmick_work->obj_3d);

	//ƒ}ƒeƒŠƒAƒ‹
	s32 data_material_index_main = g_gm_gmk_water_slider_material_id_main[type];
	void* data_material = ObjDataGet(GMD_DWORK_NO_GMK_WATER_SLIDER_MAT)->pData;
	amAssert( data_material );
	ObjAction3dNNMaterialMotionLoad( 
			&gimmick_work->obj_3d,
			0,				//reg_file_id
			NULL,			//data_work
			NULL,			//mtn_data_path
			data_material_index_main,
			data_material
	);

	//-------------------------------------------------
	//ƒTƒuƒ‚ƒfƒ‹‰Šú‰»
	//-------------------------------------------------
	GMS_GMK_WATER_SLIDER_WORK* slider_work = (GMS_GMK_WATER_SLIDER_WORK*)obj_work;
	s32 data_model_index_sub = g_gm_gmk_water_slider_model_id_sub[type];
	if ( data_model_index_sub != GMD_GMK_WATER_SLIDER_INVALID_ID ){
		ObjCopyAction3dNNModel(
			&g_gm_gmk_water_slider_obj_3d_list[data_model_index_sub],
			&slider_work->obj_3d_parts);
	}
	slider_work->obj_3d_parts.drawflag |= NND_DRAWOBJ_DOUBLESIDE;

	//ƒ‚[ƒVƒ‡ƒ“
	s32 data_motion_index_sub = g_gm_gmk_water_slider_motion_id_sub[type];
	if ( data_motion_index_sub != GMD_GMK_WATER_SLIDER_INVALID_ID ){
		void* data_motion = ObjDataGet(GMD_DWORK_NO_GMK_WATER_SLIDER_MTN)->pData;
		amAssert( data_motion );
		ObjAction3dNNMotionLoad(
				&slider_work->obj_3d_parts,
				0,
				FALSE,
				NULL, 
				NULL,
				data_motion_index_sub, 
				data_motion);
	}

	//ƒ}ƒeƒŠƒAƒ‹
	s32 data_material_index_sub = g_gm_gmk_water_slider_material_id_sub[type];
	if ( data_material_index_sub != GMD_GMK_WATER_SLIDER_INVALID_ID ){
		ObjAction3dNNMaterialMotionLoad(
				&slider_work->obj_3d_parts,
				0,
				NULL, 
				NULL,
				data_material_index_sub, 
				data_material);
	}
#if _IPHONE
	obj_work->disp_flag |= OBD_DISP_NOCLIP;
	slider_work->obj_3d_parts.command_state = OBD_DRAW_CMD_STATE_3DNN_WS;
#endif // _IPHONE
	return gimmick_work;
}

// ==========================================================================
// gmGmkWaterSliderCheckHFlip
/*!
 *	ƒMƒ~ƒbƒN@ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[@HƒtƒŠƒbƒv‚·‚é‚©”»’è
 *
 *	@param type	[in] ƒXƒ‰ƒCƒ_[ƒ^ƒCƒv
 *
 *	@return TRUEF”½“]‚·‚é FALSEF”½“]‚µ‚È‚¢
 */
// ==========================================================================
BOOL gmGmkWaterSliderCheckHFlip( GME_GMK_WATER_SLIDER_TYPE type )
{
	switch( type){
	case GMD_GMK_WATER_SLIDER_TYPE_LEFT_30D:
	case GMD_GMK_WATER_SLIDER_TYPE_LEFT_45D:
	case GMD_GMK_WATER_SLIDER_TYPE_LEFT_60D:
	case GMD_GMK_WATER_SLIDER_TYPE_LEFT:
		return FALSE;
	case GMD_GMK_WATER_SLIDER_TYPE_RIGHT_30D:
	case GMD_GMK_WATER_SLIDER_TYPE_RIGHT_45D:
	case GMD_GMK_WATER_SLIDER_TYPE_RIGHT_60D:
	case GMD_GMK_WATER_SLIDER_TYPE_RIGHT:
		return TRUE;
	default:
		amAssert(FALSE);
		break;
	}
	return FALSE;
}

// ==========================================================================
// gmGmkWaterSliderInit
/*!
 *	ƒMƒ~ƒbƒN@ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[@‰Šú‰»
 *
 *	@param obj_work		[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param slider_type	[in] ƒ^ƒCƒv
 */
// ==========================================================================
void gmGmkWaterSliderInit( OBS_OBJECT_WORK *obj_work, GME_GMK_WATER_SLIDER_TYPE slider_type )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	//ƒvƒŒƒCƒ„[‚ ‚½‚è
	gmGmkWaterSliderSetRect( gimmick_work, slider_type );

	//-------------------------------------------------
	// ƒ[ƒNÝ’è
	//-------------------------------------------------
	
	//ƒ^ƒCƒv
	gmGmkWaterSliderSetUserWorkSlideType( obj_work, slider_type );

	//ƒtƒ‰ƒO
	obj_work->move_flag = OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;

	//Œü‚«•ÊÝ’è
	fx32 slider_speed = -GMD_GMK_WATER_SLIDER_SPEED;
	obj_work->dir.y = 0xc000;
	if ( gmGmkWaterSliderCheckHFlip( slider_type )){
		obj_work->disp_flag |= OBD_DISP_HFLIP;
		slider_speed = -slider_speed;
	}
	gmGmkWaterSliderSetUserTimerSlideSpeed( obj_work, slider_speed );

	// •\— ”½“]‘Î‰ž
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_DOUBLESIDE;
	
	//À•W
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_C_FRONT;

	//ƒ}ƒeƒŠƒAƒ‹
	ObjDrawObjectActionSet3DNNMaterial( obj_work, 0 );
	obj_work->disp_flag |= OBD_DISP_STOP | OBD_DISP_REPEAT;

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = NULL;
	obj_work->ppMove = NULL;
	obj_work->ppOut = gmGmkWaterSliderDrawFunc;
	mtTaskChangeTcbDestructor( obj_work->tcb, gmGmkWaterSliderDestFunc );
}

// ==========================================================================
// gmGmkWaterSliderSetRect
/*!
 *	ƒMƒ~ƒbƒN@ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[@‹éŒ`Ý’è
 *
 *	@param gimmick_work	[in] ƒMƒ~ƒbƒNƒ[ƒN
 *	@param slider_type	[in] ƒ^ƒCƒv
 */
// ==========================================================================
void gmGmkWaterSliderSetRect( GMS_ENEMY_3D_WORK* gimmick_work, GME_GMK_WATER_SLIDER_TYPE slider_type )
{
	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	
	s16 left = 0;	
	s16 right = 0;
	s16 height = 0;
	switch( slider_type){
	case GMD_GMK_WATER_SLIDER_TYPE_LEFT_30D:
	case GMD_GMK_WATER_SLIDER_TYPE_RIGHT_30D:
		left = -64;
		height = 32;
		break;
	case GMD_GMK_WATER_SLIDER_TYPE_LEFT_45D:
	case GMD_GMK_WATER_SLIDER_TYPE_RIGHT_45D:
		left = -64;
		height = 64;
		break;
	case GMD_GMK_WATER_SLIDER_TYPE_LEFT_60D:
	case GMD_GMK_WATER_SLIDER_TYPE_RIGHT_60D:
		left = -64;
		height = 128;
		break;
	case GMD_GMK_WATER_SLIDER_TYPE_LEFT:
	case GMD_GMK_WATER_SLIDER_TYPE_RIGHT:
	default:
		amAssert(FALSE);
		break;
	}
	ObjRectWorkZSet(
			rect_work,
			(s16)(left - GMD_GMK_WATER_SLIDER_RECT_MARGIN), -GMD_GMK_WATER_SLIDER_RECT_MARGIN, -500,
			(s16)(right + GMD_GMK_WATER_SLIDER_RECT_MARGIN), (s16)(height + GMD_GMK_WATER_SLIDER_RECT_MARGIN), 500);
	
	//–hŒä—p
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	rect_work->ppDef = gmGmkWaterSliderDefFunc;
}


// ==========================================================================
// gmGmkWaterSliderDestFunc
/*!
 *	I—¹ŠÖ”
 *
 *	@param tcb	[in] TCB
 */
// ==========================================================================
void gmGmkWaterSliderDestFunc( MTS_TASK_TCB *tcb )
{
	GMS_GMK_WATER_SLIDER_WORK* slider_work = (GMS_GMK_WATER_SLIDER_WORK*)mtTaskGetTcbWork( tcb );
	//ƒ‚[ƒVƒ‡ƒ“‰ð•ú
	ObjAction3dNNMotionRelease(	&slider_work->obj_3d_parts );

	//•W€I—¹
	GmEnemyDefaultExit( tcb );
}

// ==========================================================================
// gmGmkWaterSliderDrawFunc
/*!
 *	ƒMƒ~ƒbƒN@ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[@•`‰æŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkWaterSliderDrawFunc( OBS_OBJECT_WORK* obj_work )
{
	//---------------------------------------
	//mainƒp[ƒc
	//---------------------------------------
	OBS_ACTION3D_NN_WORK *obj_3d = obj_work->obj_3d;
	amAssert( obj_3d );
	if ( obj_3d->motion ){
		//ƒ‚[ƒVƒ‡ƒ“‚ð“¯Šú‚³‚¹‚é
		float frame_start = amMotionMaterialGetStartFrame( obj_3d->motion, obj_3d->mat_act_id );
		float frame_end = amMotionMaterialGetEndFrame( obj_3d->motion, obj_3d->mat_act_id );
		float frame_max = frame_end - frame_start;

		float frame = (float)gmGmkWaterSlidereGameSystemGetSyncTime();

		obj_3d->mat_frame = fmod( frame, frame_max );
	}
	ObjDrawActionSummary( obj_work );

	//---------------------------------------
	//ƒTƒuƒp[ƒc
	//---------------------------------------	
	u32 disp_flag = obj_work->disp_flag;

	//ƒTƒuƒp[ƒc‚Í“¯Šú‚³‚¹‚È‚¢
	disp_flag |= OBD_DISP_REPEAT;
	disp_flag &= ~OBD_DISP_STOP;
	
	if ( ObjObjectPauseCheck(0) ){
		disp_flag |= OBD_DISP_NOUPDATE;
	}
	GMS_GMK_WATER_SLIDER_WORK* slider_work = (GMS_GMK_WATER_SLIDER_WORK*)obj_work;
	VecFx32 pos = obj_work->pos;
	pos.z += 32*FX32_ONE;

	ObjDrawAction3DNN(
			&slider_work->obj_3d_parts,
			&pos, 
			&obj_work->dir, 
			&obj_work->scale, 
			&disp_flag );
}

// ==========================================================================
// gmGmkWaterSliderDefFunc
/*!
 *	ƒMƒ~ƒbƒN@ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[@ŠJŽnó‘Ô‹éŒ`ŠÖ”
 *
 *	@param gimmick_rect	[in] Ž©g‚Ì‹éŒ`ƒ[ƒN
 *	@param target_rect	[in] ‘ŠŽè‚Ì‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkWaterSliderDefFunc( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* player_rect )
{
	amAssert( gimmick_rect );
	amAssert( player_rect );

	//ƒMƒ~ƒbƒNƒ[ƒN
	OBS_OBJECT_WORK* gimmick_obj_work = gimmick_rect->parent_obj;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)gimmick_obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* player_obj_work = player_rect->parent_obj;
	amAssert( player_obj_work );
	
	//ƒvƒŒƒCƒ„ˆÈŠO‚Í”»’è‚µ‚È‚¢
	if ( player_obj_work->obj_type != GMD_OBJTYPE_PLAYER ){
		return;
	}
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)player_obj_work;

	//Šù‚ÉŽÀsÏ‚Ý
	if ( player_work->seq_state == GME_PLY_SEQ_STATE_GMK_WATER_SLIDER ){
		player_work->gmk_obj = gimmick_obj_work;
		return;
	}

	//Ý’u‚µ‚½‚ç—¬‚³‚ê‚é
	if ( player_obj_work->move_flag & OBD_MOVE_UNDER ){

		//ƒvƒŒƒCƒ„”½“]iƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‚Æ‚Í‹tj
		GME_GMK_WATER_SLIDER_TYPE slider_type = gmGmkWaterSliderGetUserWorkSlideType( gimmick_obj_work );
		if ( gmGmkWaterSliderCheckHFlip(slider_type) ){
			player_obj_work->disp_flag &= ~OBD_DISP_HFLIP;
		}
		else{
			player_obj_work->disp_flag |= OBD_DISP_HFLIP;
		}

		//ƒvƒŒƒCƒ„ƒXƒs[ƒh
		player_obj_work->spd_m = gmGmkWaterSliderGetUserTimerSlideSpeed( gimmick_obj_work );
		player_obj_work->spd.x = 0;
		player_obj_work->spd.y = 0;
		player_obj_work->spd_add.x = 0;
		player_obj_work->spd_add.y = 0;

		//ƒvƒŒƒCƒ„ƒV[ƒPƒ“ƒX•ÏX
		GmPlySeqInitWaterSlider( player_work, &gimmick_work->ene_com );

		//ƒMƒ~ƒbƒNƒ[ƒN
		gimmick_work->ene_com.target_obj = player_obj_work;
		gimmick_rect->flag |= OBD_RECT_OUT;
		gimmick_obj_work->ppFunc = gmGmkWaterSliderMainActive;

		//ƒGƒtƒFƒNƒg
		GmGmkWaterSliderCreateEffect();

		//•\Ž¦ƒIƒtƒZƒbƒg
		nnMakeUnitMatrix( &player_work->ex_obj_mtx_r );
		Angle32 angle_y = -0x1800;
		if ( player_obj_work->disp_flag & OBD_DISP_HFLIP ){
			angle_y = -angle_y;
		}
		nnRotateYMatrix( &player_work->ex_obj_mtx_r, &player_work->ex_obj_mtx_r, angle_y );
		NNM_MTX(player_work->ex_obj_mtx_r, 1, 3) = GMD_GMK_WATER_SLIDER_PLAYER_OFFSET_Y;

		//Šg’£ƒ}ƒgƒŠƒNƒX
		player_work->gmk_flag |= GMD_PLGF_GMK_EXMTX_R;
	}
}

// ==========================================================================
// gmGmkWaterSliderMainActive
/*!
 *	ƒMƒ~ƒbƒN@ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[@ŽÀsƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkWaterSliderMainActive( OBS_OBJECT_WORK *obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );
	OBS_OBJECT_WORK* player_obj_work = gimmick_work->ene_com.target_obj;
	amAssert( player_obj_work );
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)player_obj_work;
	amAssert( player_work );
	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	amAssert( rect_work );

	//—£‚ê‚½ê‡
	if ( player_work->seq_state != GME_PLY_SEQ_STATE_GMK_WATER_SLIDER ){
		gimmick_work->ene_com.target_obj = NULL;
		rect_work->flag &= ~OBD_RECT_OUT;
		obj_work->ppFunc = NULL;
		return ;
	}
}

// ==========================================================================
// gmGmkWaterSliderEffectMainFunc
/*!
 *	ƒMƒ~ƒbƒN@ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[ƒGƒtƒFƒNƒg@ŽÀsƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkWaterSliderEffectMainFunc( OBS_OBJECT_WORK *obj_work )
{
	UNREFERENCED_PARAMETER(obj_work);

	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‚Å‚Í‚È‚¢
	if ( player_work->seq_state != GME_PLY_SEQ_STATE_GMK_WATER_SLIDER ){
		GmGmkWaterSliderDeleteEffect();
		return;
	}

	//ƒGƒtƒFƒNƒgƒfƒtƒHƒ‹ƒgˆ—
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ( obj_work );
}

// ==========================================================================
// gmGmkWaterSliderEffectDestFunc
/*!
 *	ƒMƒ~ƒbƒN@ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[ƒGƒtƒFƒNƒg@I—¹ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkWaterSliderEffectDestFunc( MTS_TASK_TCB *tcb )
{
	//Œø‰Ê‰¹
	if ( g_gm_gmk_water_slider_se_handle ){
		GmSoundStopSE( g_gm_gmk_water_slider_se_handle );
		GsSoundFreeSeHandle(g_gm_gmk_water_slider_se_handle);
		g_gm_gmk_water_slider_se_handle = NULL;
	}

	//U“®’âŽ~
	GMM_PAD_VIB_STOP();

	//ƒGƒtƒFƒNƒg‹¤’ÊI—¹ˆ—
	GmEffectDefaultExit( tcb );
	g_gm_gmk_water_slider_effct_player = NULL;
}


// ==========================================================================
//ƒ†[ƒUƒtƒ‰ƒO
// ==========================================================================

// ==========================================================================
// gmGmkWaterSliderSetUserWorkSlideSpeed
/*!
 *	ƒ†[ƒUƒ[ƒN‚ðƒXƒ‰ƒCƒ_[ƒ^ƒCƒv‚Æ‚µ‚Ä’Ç‰Á
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param type	[in] ƒXƒ‰ƒCƒ_[ƒ^ƒCƒv
 */
// ==========================================================================
void gmGmkWaterSliderSetUserWorkSlideType( OBS_OBJECT_WORK* obj_work, GME_GMK_WATER_SLIDER_TYPE type )
{
	obj_work->user_work = (u32)type;
}

// ==========================================================================
// gmGmkWaterSliderGetUserWorkSlideSpeed
/*!
 *	ƒ†[ƒUƒ[ƒN‚ðƒXƒ‰ƒCƒ_[ƒ^ƒCƒv‚Æ‚µ‚ÄŽæ“¾
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return ƒXƒ‰ƒCƒ_[ƒ^ƒCƒv
 */
// ==========================================================================
GME_GMK_WATER_SLIDER_TYPE gmGmkWaterSliderGetUserWorkSlideType( OBS_OBJECT_WORK* obj_work )
{
	return (GME_GMK_WATER_SLIDER_TYPE)obj_work->user_work;
}



// ==========================================================================
// gmGmkWaterSliderSetUserTimerSlideSpeed
/*!
 *	ƒ†[ƒUƒ^ƒCƒ}‚ð—¬‚ê‚éƒXƒs[ƒh‚Æ‚µ‚Ä’Ç‰Á
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param speed	[in] —¬‚ê‚éƒXƒs[ƒh
 */
// ==========================================================================
void gmGmkWaterSliderSetUserTimerSlideSpeed( OBS_OBJECT_WORK* obj_work, fx32 speed )
{
	obj_work->user_timer = (s32)speed;
}

// ==========================================================================
// gmGmkWaterSliderGetUserTimerSlideSpeed
/*!
 *	ƒ†[ƒUƒ^ƒCƒ}‚ð—¬‚ê‚éƒXƒs[ƒh‚Æ‚µ‚ÄŽæ“¾
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return —¬‚ê‚éƒXƒs[ƒh
 */
// ==========================================================================
fx32 gmGmkWaterSliderGetUserTimerSlideSpeed( OBS_OBJECT_WORK* obj_work )
{
	return (fx32)obj_work->user_timer;
}



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
