// ==========================================================================
/*!
  @file gmGmkShutter.cpp
  @brief ƒMƒ~ƒbƒN ƒVƒƒƒbƒ^[

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkShutter.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"
#include "gmPlySeq.h"
#include "gmPlySeqGmk.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"

#include "gmGmkShutter.h"
// ƒf[ƒ^ƒwƒbƒ_
#include "common/model/gmk_shutter_mdl.hmb"
#include "common/model/gmk_shutter_f_mdl.hmb"
#include "common/model/gmk_shutter_f_mat.hmb"


//----- Definitions ---------------------------------------------------------

#define GMD_GMK_SHUTTER_BLOCK_SIZE					(64)	//ƒuƒƒbƒNƒTƒCƒY

#define GMD_GMK_SHUTTER_SIZE						(64)	//’ŒƒTƒCƒY

#define GMD_GMK_SHUTTER_NO_PRESS_DIE_FLOW_DISTANCE	(64)	//ˆ³Ž€‚³‚¹‚È‚¢‚æ‚¤‚ÉˆÚ“®‚³‚¹‚é“Vˆä‚Æ‚Ì‹——£
#define GMD_GMK_SHUTTER_NO_PRESS_DIE_MOVE_LAND		(4)		//ˆ³Ž€‚³‚¹‚È‚¢‚æ‚¤‚ÉˆÚ“®‚³‚¹‚é—Êi’nãj
#define GMD_GMK_SHUTTER_NO_PRESS_DIE_MOVE_JUMP		(1)		//ˆ³Ž€‚³‚¹‚È‚¢‚æ‚¤‚ÉˆÚ“®‚³‚¹‚é—Êi‹ó’†j

#define GMD_GMK_SHUTTER_EFFECT_OFFSET_X				(16)		//ƒGƒtƒFƒNƒg•\Ž¦ƒIƒtƒZƒbƒg
#define GMD_GMK_SHUTTER_EFFECT_OFFSET_Y				(32)		//ƒGƒtƒFƒNƒg•\Ž¦ƒIƒtƒZƒbƒg

//“üŒû
#define GMD_GMK_SHUTTER_IN_CLOSE_DISTANCE	((fx32)(GMD_GMK_SHUTTER_SIZE*FX32_ONE))	//•Â‚¶Žn‚ß‚éƒvƒŒƒCƒ„À•W
#define GMD_GMK_SHUTTER_IN_CLOSE_POS_OFFSET	((fx32)(GMD_GMK_SHUTTER_SIZE*FX32_ONE))	//•Â‚¶‚éó‘Ô‚ÌÀ•W—p
#define GMD_GMK_SHUTTER_IN_CLOSE_MOVE		((fx32)(4*FX32_ONE))	//•Â‚¶‚éˆÚ“®—Ê


//oŒû
#define GMD_GMK_SHUTTER_OUT_CLOSE_DISTANCE	((fx32)(GMD_GMK_SHUTTER_SIZE*FX32_ONE))	//•Â‚¶Žn‚ß‚éƒvƒŒƒCƒ„À•W

//ƒ[ƒN
typedef struct tag_GMS_GMK_SHUTTER_WORK{
	GMS_ENEMY_3D_WORK gimmick_work;	//ƒMƒ~ƒbƒNƒ[ƒN
	OBS_ACTION3D_NN_WORK obj_3d_parts;	//ƒp[ƒc
	GMS_EFFECT_3DES_WORK* effect_work;	//ƒGƒtƒFƒNƒgƒ[ƒN
}GMS_GMK_SHUTTER_WORK;


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static GMS_ENEMY_3D_WORK* gmGmkShutterLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type );
static GMS_ENEMY_3D_WORK* gmGmkShutterLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type );
static void gmGmkShutterDestFuncForFinaleZone( MTS_TASK_TCB *tcb );

//“üŒû
static void gmGmkShutterInInit( OBS_OBJECT_WORK* obj_work );
static void gmGmkShutterInOutFuncForFinalZone( OBS_OBJECT_WORK* obj_work );
static void gmGmkShutterInMainWaitClose( OBS_OBJECT_WORK* obj_work );
static void gmGmkShutterInMainClose( OBS_OBJECT_WORK* obj_work );

//oŒû
static void gmGmkShutterOutInit( OBS_OBJECT_WORK* obj_work );
static void gmGmkShutterOutOutFuncForFinalZone( OBS_OBJECT_WORK* obj_work );
static void gmGmkShutterOutMainOpen( OBS_OBJECT_WORK* obj_work );

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------


//ƒtƒ@ƒCƒiƒ‹ƒ][ƒ“—p•`‰æƒIƒtƒZƒbƒg
static const s32 gm_gmk_shutter_disp_offset_for_final_zone[MTD_XY] = {
	GMD_GMK_SHUTTER_BLOCK_SIZE/4,
	GMD_GMK_SHUTTER_BLOCK_SIZE/2
};


static OBS_ACTION3D_NN_WORK* g_gm_gmk_shutter_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------

// ==========================================================================
// GmGmkShutterBuild
/*!
 *	ƒMƒ~ƒbƒN ƒVƒƒƒbƒ^[ ƒf[ƒ^\’z
 */
// ==========================================================================
void GmGmkShutterBuild(void)
{
	g_gm_gmk_shutter_obj_3d_list = GmGameDBuildRegBuildModel(
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_SHUTTER_MODEL ),
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_SHUTTER_TEX ),
			0	//draw_flag
			);
}

// ==========================================================================
// GmGmkShutterFlush
/*!
 *	ƒMƒ~ƒbƒN ƒVƒƒƒbƒ^[ ƒf[ƒ^•Ð•t‚¯
 */
// ==========================================================================
void GmGmkShutterFlush(void)
{
	AMS_AMB_HEADER* amb_header = (AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_SHUTTER_MODEL );

	GmGameDBuildRegFlushModel( g_gm_gmk_shutter_obj_3d_list, amb_header->file_num );
	g_gm_gmk_shutter_obj_3d_list = NULL;
}

// ==========================================================================
// GmGmkShutterInInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒVƒƒƒbƒ^[“üŒû
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkShutterInInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkShutterLoadObj( eve_rec, pos_x, pos_y, type );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkShutterInInit( obj_work );

	return obj_work;
}

// ==========================================================================
// GmGmkShutterOutInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒVƒƒƒbƒ^[oŒû
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkShutterOutInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkShutterLoadObj( eve_rec, pos_x, pos_y, type );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkShutterOutInit( obj_work );

	return obj_work;
}



// ==========================================================================
// GmGmkShutterOutChangeModeClose
/*!
 *	ƒVƒƒƒbƒ^[“üŒû•Â‚ß‚é
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void GmGmkShutterInChangeModeClose( OBS_OBJECT_WORK* obj_work )
{
	//Šù‚É•Â‚¶‚Ä‚¢‚é
	if ( obj_work->pos.y <= (fx32)obj_work->user_work ){
		return;
	}

	amAssert( obj_work );
	obj_work->spd.y = -GMD_GMK_SHUTTER_IN_CLOSE_MOVE;
	obj_work->ppFunc = gmGmkShutterInMainClose;

	//•`‰æ
	obj_work->disp_flag &= ~OBD_DISP_NODISP;

	//ƒGƒtƒFƒNƒg¶¬
	GMS_GMK_SHUTTER_WORK* shutter_work = (GMS_GMK_SHUTTER_WORK*)obj_work;
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL && !shutter_work->effect_work ){
		
		OBS_OBJECT_WORK* effect_obj_work = (OBS_OBJECT_WORK*)GmEfctCmnEsCreate( 
				NULL,	//e‚É‚Í‚µ‚È‚¢iíœŽž‚ÉAƒGƒtƒFƒNƒg‚ª‹}‚ÉÁ‚¦‚È‚¢‚æ‚¤‚Éj
				GME_EFCT_CMN_IDX_PILLAR_F_01 );
		amAssert( effect_obj_work );
		effect_obj_work->pos.x = obj_work->pos.x+GMD_GMK_SHUTTER_EFFECT_OFFSET_X*FX32_ONE;
		effect_obj_work->pos.y = obj_work->pos.y-GMD_GMK_SHUTTER_EFFECT_OFFSET_Y*FX32_ONE;
		effect_obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_BACK;

		shutter_work->effect_work = (GMS_EFFECT_3DES_WORK*)effect_obj_work;
	}
}

// ==========================================================================
// GmGmkShutterOutChangeModeOpen
/*!
 *	ƒVƒƒƒbƒ^[oŒûŠJ‚¯‚é
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void GmGmkShutterOutChangeModeOpen( OBS_OBJECT_WORK* obj_work )
{
	//Šù‚ÉŠJ‚¢‚Ä‚¢‚é
	if ( obj_work->pos.y >= (fx32)obj_work->user_work ){
		return;
	}

	amAssert( obj_work );
	obj_work->spd.y = GMD_GMK_SHUTTER_IN_CLOSE_MOVE;
	obj_work->ppFunc = gmGmkShutterOutMainOpen;	

	//ƒGƒtƒFƒNƒg¶¬
	GMS_GMK_SHUTTER_WORK* shutter_work = (GMS_GMK_SHUTTER_WORK*)obj_work;
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL && !shutter_work->effect_work  ){
		
		OBS_OBJECT_WORK* effect_obj_work = (OBS_OBJECT_WORK*)GmEfctCmnEsCreate( 
				NULL,	//e‚É‚Í‚µ‚È‚¢iíœŽž‚ÉAƒGƒtƒFƒNƒg‚ª‹}‚ÉÁ‚¦‚È‚¢‚æ‚¤‚Éj
				GME_EFCT_CMN_IDX_PILLAR_F_01 );
		amAssert( effect_obj_work );
		effect_obj_work->pos.x = obj_work->pos.x-GMD_GMK_SHUTTER_EFFECT_OFFSET_X*FX32_ONE;
		effect_obj_work->pos.y = obj_work->pos.y+GMD_GMK_SHUTTER_EFFECT_OFFSET_Y*FX32_ONE;
		effect_obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_BACK;

		shutter_work->effect_work = (GMS_EFFECT_3DES_WORK*)effect_obj_work;
	}
}

//----- Local Functions -----------------------------------------------------

// ==========================================================================
// gmGmkShutterLoadObjNoModel
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
GMS_ENEMY_3D_WORK* gmGmkShutterLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type )
{

	UNREFERENCED_PARAMETER(type);

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec, 
			pos_x, 
			pos_y, 
			sizeof(GMS_GMK_SHUTTER_WORK), 
			"GMK_SHUTTER");
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}

// ==========================================================================
// gmGmkShutterLoadObj
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
GMS_ENEMY_3D_WORK* gmGmkShutterLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type )
{
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	s32 data_model_index;
	if ( zone_type == GSD_MAIN_ZONE_TYPE_2 ){
		data_model_index = IDB_GMK_SHUTTER_MDL_GMK_SHUTTER_2_ZNO;
	}
	else if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL ){
		data_model_index = IDB_GMK_SHUTTER_F_MDL_GMK_SHUTTER_F_A_ZNO;
	}
	else{
		amAssert(FALSE);
		return NULL;
	}

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkShutterLoadObjNoModel(
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

	//“Ç‚Ýž‚Ý
	ObjObjectCopyAction3dNNModel(
		obj_work,
		&g_gm_gmk_shutter_obj_3d_list[data_model_index],
		&gimmick_work->obj_3d);

	//-------------------------------------------------
	//ƒOƒŒƒAƒp[ƒc“Ç‚Ýž‚Ýiƒtƒ@ƒCƒiƒ‹ƒ][ƒ“‚Ìê‡j
	//-------------------------------------------------
	if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL ){
		//ƒTƒuƒp[ƒc¶¬	
		GMS_GMK_SHUTTER_WORK* shutter_work = (GMS_GMK_SHUTTER_WORK*)obj_work;

		//ƒ‚ƒfƒ‹
		s32 model_id = IDB_GMK_SHUTTER_F_MDL_GMK_SHUTTER_F_G_ZNO;
		ObjCopyAction3dNNModel(
				&g_gm_gmk_shutter_obj_3d_list[model_id],
				&shutter_work->obj_3d_parts);

		//ƒ}ƒeƒŠƒAƒ‹
		ObjAction3dNNMaterialMotionLoad(
				&shutter_work->obj_3d_parts,
				0,
				NULL, 
				NULL,
				IDB_GMK_SHUTTER_F_MAT_GMK_SHUTTER_F_G_ZNV, 
				ObjDataGet( GMD_DWORK_NO_GMK_SHUTTER_MAT )->pData);
	}

	return gimmick_work;
}

// =======================================================================
// gmGmkShutterDestFuncForFinaleZone
/*!
 *	I—¹ŠÖ”
 *
 *	@param tcb	[io] tcb
 */
// =======================================================================
void gmGmkShutterDestFuncForFinaleZone( MTS_TASK_TCB *tcb )
{	
	GMS_GMK_SHUTTER_WORK* shutter_work = (GMS_GMK_SHUTTER_WORK*)mtTaskGetTcbWork( tcb );
	//ƒ‚[ƒVƒ‡ƒ“‰ð•ú
	ObjAction3dNNMotionRelease(	&shutter_work->obj_3d_parts );

	//•W€I—¹
	GmEnemyDefaultExit( tcb );
}



// ==========================================================================
//“üŒû
// ==========================================================================

// ==========================================================================
// gmGmkShutterInInit
/*!
 *	ƒMƒ~ƒbƒN@ƒVƒƒƒbƒ^[“üŒû@‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkShutterInInit( OBS_OBJECT_WORK* obj_work )
{
	//-------------------------------------------------
	//ƒtƒB[ƒ‹ƒh’nŒ`
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );
	gimmick_work->ene_com.col_work.obj_col.obj = &gimmick_work->ene_com.obj_work;
	gimmick_work->ene_com.col_work.obj_col.width = GMD_GMK_SHUTTER_BLOCK_SIZE;
	gimmick_work->ene_com.col_work.obj_col.height = GMD_GMK_SHUTTER_BLOCK_SIZE;
	gimmick_work->ene_com.col_work.obj_col.ofst_x = (s16)(-gimmick_work->ene_com.col_work.obj_col.width / 2);
	gimmick_work->ene_com.col_work.obj_col.ofst_y = (s16)(-gimmick_work->ene_com.col_work.obj_col.height / 2);
	
	//-------------------------------------------------
	// ƒ[ƒNÝ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->move_flag |= OBD_MOVE_NOCOL;
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_NODISP;
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	gimmick_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOPRESSDIE;	// ƒvƒŒƒCƒ„[‚ðˆ³Ž€‚³‚¹‚È‚¢

	//À•W
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_B_BACK;

	//•Â‚¶‚éó‘Ô‚ÌÀ•W
	obj_work->user_work = (u32)(obj_work->pos.y - GMD_GMK_SHUTTER_IN_CLOSE_POS_OFFSET);	

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = gmGmkShutterInMainWaitClose;
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL ){
		//ˆ—ŠÖ”
		obj_work->ppOut = gmGmkShutterInOutFuncForFinalZone;
		mtTaskChangeTcbDestructor( obj_work->tcb, gmGmkShutterDestFuncForFinaleZone );
	}
}

// =======================================================================
// gmGmkShutterInOutFuncForFinalZone
/*!
 *	•`‰æŠÖ”
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmGmkShutterInOutFuncForFinalZone( OBS_OBJECT_WORK* obj_work )
{	
	amAssert( obj_work );

	GMS_GMK_SHUTTER_WORK* shutter_work = (GMS_GMK_SHUTTER_WORK*)obj_work;

	obj_work->ofst.x = gm_gmk_shutter_disp_offset_for_final_zone[MTD_X] * FX32_ONE;
	obj_work->ofst.y = gm_gmk_shutter_disp_offset_for_final_zone[MTD_Y] * FX32_ONE;

	//æ“ª•`‰æ
	ObjDrawActionSummary( obj_work );

	//ƒOƒŒƒAˆÊ’u
	VecFx32 pos = obj_work->pos;
	pos.x += obj_work->ofst.x;
	pos.y += obj_work->ofst.y;

	u32 disp_flag = obj_work->disp_flag | OBD_DISP_REPEAT;

	//ƒOƒŒƒAƒ}ƒeƒŠƒAƒ‹
	if ( !ObjObjectPauseCheck(0) ){
		ObjDrawAction3DNNMaterialUpdate(
				&shutter_work->obj_3d_parts,
				&disp_flag );
	}

	//ƒOƒŒƒA•`‰æ
	ObjDrawAction3DNN(
			&shutter_work->obj_3d_parts,
			&pos, 
			&obj_work->dir, 
			&obj_work->scale, 
			&disp_flag );
}
// ==========================================================================
// gmGmkShutterInMainWaitClose
/*!
 *	•Â‚¶‚é‘Ò‚¿ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkShutterInMainWaitClose( OBS_OBJECT_WORK* obj_work )
{
	OBS_OBJECT_WORK* player_obj_work = (OBS_OBJECT_WORK*)g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	amAssert( player_obj_work );

	if ( player_obj_work->pos.x - obj_work->pos.x < GMD_GMK_SHUTTER_IN_CLOSE_DISTANCE ){
		return;
	}

	GmGmkShutterInChangeModeClose( obj_work );
}

// ==========================================================================
// gmGmkShutterInMainClose
/*!
 *	•Â‚¶‚éƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkShutterInMainClose( OBS_OBJECT_WORK* obj_work )
{
	OBS_OBJECT_WORK* player_obj_work = (OBS_OBJECT_WORK*)g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	amAssert( player_obj_work );
	//ƒvƒŒƒCƒ„‚ªã•û‚É‚¢‚ÄA“Vˆä‚É‹²‚Ü‚ê‚»‚¤‚È‚Æ‚«AƒvƒŒƒCƒ„‚ªˆ³Ž€‚µ‚È‚¢‚æ‚¤‚É’²®
	fx32 distance = (fx32)(obj_work->user_work - obj_work->pos.y);
	if ( (MTM_MATH_ABS(player_obj_work->pos.x - obj_work->pos.x) < GMD_GMK_SHUTTER_SIZE/2*FX32_ONE)
			&& (player_obj_work->pos.y <= obj_work->pos.y )
			&& (MTM_MATH_ABS(distance) < GMD_GMK_SHUTTER_NO_PRESS_DIE_FLOW_DISTANCE*FX32_ONE)
	){
		s32 center = g_gm_main_system.map_fcol.left + (g_gm_main_system.map_fcol.right - g_gm_main_system.map_fcol.left)/2;

		//Ý’u‚µ‚Ä‚¢‚é‚Æ‚«
		if ( player_obj_work->move_flag & OBD_MOVE_UNDER ){
			fx32 move = GMD_GMK_SHUTTER_NO_PRESS_DIE_MOVE_LAND*FX32_ONE;
			if ( center*FX32_ONE < player_obj_work->pos.x ){
				move *= -1;
			}
			player_obj_work->flow.x += move;
		}
		//ƒWƒƒƒ“ƒv’†
		else{
			fx32 move = GMD_GMK_SHUTTER_NO_PRESS_DIE_MOVE_JUMP*FX32_ONE;
			if ( center*FX32_ONE < player_obj_work->pos.x ){
				move *= -1;
			}
			GmPlySeqGmkInitGmkJump( (GMS_PLAYER_WORK*)player_obj_work, move, 0);
			GmPlySeqChangeSequenceState( (GMS_PLAYER_WORK*)player_obj_work, GME_PLY_SEQ_STATE_JUMP );
		}
	}

	//•Â‚¶‚½‚ç
	if ( obj_work->pos.y <= (fx32)obj_work->user_work ){
		obj_work->pos.y = (fx32)obj_work->user_work;
		obj_work->spd.y = 0;
		obj_work->ppFunc = NULL;
		obj_work->ppMove = NULL;

		//ƒGƒtƒFƒNƒgÁ‚µ
		GMS_GMK_SHUTTER_WORK* shutter_work = (GMS_GMK_SHUTTER_WORK*)obj_work;
		if ( shutter_work->effect_work ){
			ObjDrawKillAction3DES( (OBS_OBJECT_WORK*)shutter_work->effect_work );
			shutter_work->effect_work = NULL;
		}
	}
}


// ==========================================================================
//oŒû
// ==========================================================================


// ==========================================================================
// gmGmkShutterOutInit
/*!
 *	ƒMƒ~ƒbƒN@ƒVƒƒƒbƒ^[oŒû@‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkShutterOutInit( OBS_OBJECT_WORK* obj_work )
{
	//-------------------------------------------------
	//ƒtƒB[ƒ‹ƒh’nŒ`
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );
	gimmick_work->ene_com.col_work.obj_col.obj = &gimmick_work->ene_com.obj_work;
	gimmick_work->ene_com.col_work.obj_col.width = GMD_GMK_SHUTTER_BLOCK_SIZE;
	gimmick_work->ene_com.col_work.obj_col.height = GMD_GMK_SHUTTER_BLOCK_SIZE;
	gimmick_work->ene_com.col_work.obj_col.ofst_x = (s16)(-gimmick_work->ene_com.col_work.obj_col.width / 2);
	gimmick_work->ene_com.col_work.obj_col.ofst_y = (s16)(-gimmick_work->ene_com.col_work.obj_col.height / 2);
	
	//-------------------------------------------------
	// ƒ[ƒNÝ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->move_flag |= OBD_MOVE_NOCOL;
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	gimmick_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOPRESSDIE;	// ƒvƒŒƒCƒ„[‚ðˆ³Ž€‚³‚¹‚È‚¢

	//À•W
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_B_BACK;

	//ŠJ‚­ó‘Ô‚ÌÀ•W
	obj_work->user_work = (u32)(obj_work->pos.y + GMD_GMK_SHUTTER_IN_CLOSE_POS_OFFSET);	

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = NULL;
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL ){
		//ˆ—ŠÖ”
		obj_work->ppOut = gmGmkShutterOutOutFuncForFinalZone;
		mtTaskChangeTcbDestructor( obj_work->tcb, gmGmkShutterDestFuncForFinaleZone );
	}
}

// =======================================================================
// gmGmkShutterOutOutFuncForFinalZone
/*!
 *	•`‰æŠÖ”
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmGmkShutterOutOutFuncForFinalZone( OBS_OBJECT_WORK* obj_work )
{	
	amAssert( obj_work );

	GMS_GMK_SHUTTER_WORK* shutter_work = (GMS_GMK_SHUTTER_WORK*)obj_work;

	obj_work->ofst.x = -gm_gmk_shutter_disp_offset_for_final_zone[MTD_X] * FX32_ONE;
	obj_work->ofst.y = gm_gmk_shutter_disp_offset_for_final_zone[MTD_Y] * FX32_ONE;

	//æ“ª•`‰æ
	ObjDrawActionSummary( obj_work );

	//ƒOƒŒƒAˆÊ’u
	VecFx32 pos = obj_work->pos;
	pos.x += obj_work->ofst.x;
	pos.y += obj_work->ofst.y;

	u32 disp_flag = obj_work->disp_flag | OBD_DISP_REPEAT;

	//ƒOƒŒƒAƒ}ƒeƒŠƒAƒ‹
	if ( !ObjObjectPauseCheck(0) ){
		ObjDrawAction3DNNMaterialUpdate(
				&shutter_work->obj_3d_parts,
				&disp_flag );
	}

	//ƒOƒŒƒA•`‰æ
	ObjDrawAction3DNN(
			&shutter_work->obj_3d_parts,
			&pos, 
			&obj_work->dir, 
			&obj_work->scale, 
			&disp_flag );
}

// ==========================================================================
// gmGmkShutterOutMainOpen
/*!
 *	ŠJ‚­ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkShutterOutMainOpen( OBS_OBJECT_WORK* obj_work )
{

	//ŠJ‚¢‚½‚ç
	if ( obj_work->pos.y >= (fx32)obj_work->user_work ){
		obj_work->pos.y = (fx32)obj_work->user_work;
		obj_work->spd.y = 0;
		obj_work->ppFunc = NULL;
		obj_work->ppMove = NULL;

		//•`‰æ‚µ‚È‚¢
		obj_work->disp_flag |= OBD_DISP_NODISP;

		//ƒGƒtƒFƒNƒgÁ‚µ
		GMS_GMK_SHUTTER_WORK* shutter_work = (GMS_GMK_SHUTTER_WORK*)obj_work;
		if ( shutter_work->effect_work ){
			ObjDrawKillAction3DES( (OBS_OBJECT_WORK*)shutter_work->effect_work );
			shutter_work->effect_work = NULL;
		}
	}
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
