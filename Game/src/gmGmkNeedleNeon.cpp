// ==========================================================================
/*!
  @file gmGmkNeedleNeon.cpp
  @brief ÉMÉ~ÉbÉN ÉlÉIÉìêj

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkNeedleNeon.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
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
#include "gmSound.h"

#if _IPHONE
#include "gmTvx.h"
#endif // _IPHONE

#include "gmGmkNeedleNeon.h"
// ÉfÅ[É^ÉwÉbÉ_
#include "common/model/gmk_needle_mdl.hmb"
//#include "common/model/gmk_needle_neon_mat.hmb"

#if _IPHONE
// commonÇ∆ç\ê¨Ç™ïœÇÌÇ¡ÇƒÇ¢ÇÈèÍçáÇ…égópÇ∑ÇÈíËã`
#include "iPhone/model/GMK_NEEDLE_TVX.HMB"
#endif // _IPHONE


//----- Definitions ---------------------------------------------------------

#define GMD_GMK_NEEDLE_NEON_TEST_TVX       (1 & _IPHONE)

#define GMD_GMK_NEEDLE_NEON_FLAG_MODE_TIMER		(1<<0)				//éûå¿éÆÉÇÅ[ÉhÉtÉâÉO
#define GMD_GMK_NEEDLE_NEON_SIGNAL_ON			(1<<1)				//ÉOÉåÉAÇONÇ…ïœÇ¶ÇÈ
#define GMD_GMK_NEEDLE_NEON_SIGNAL_ACTIVE		(1<<2)				//ÉOÉåÉAÇÉAÉNÉeÉBÉuÇ…ïœÇ¶ÇÈ
#define GMD_GMK_NEEDLE_NEON_SIGNAL_OFF			(1<<3)				//ÉOÉåÉAÇOFFÇ…ïœÇ¶ÇÈ

#define	GMD_GMK_NEEDLE_NEON_DRAW_NUM			(5)					//ï`âÊñ{êî


#define GMD_GMK_NEEDLE_NEON_ACTIVE_OFFSET_Y		(32)	//à¯Ç¡çûÇﬁèÛë‘ÉIÉtÉZÉbÉg
#define GMD_GMK_NEEDLE_NEON_WAIT_TIME			(480)	//ë“ã@éûä‘
#define GMD_GMK_NEEDLE_NEON_ON_TIME				(10)	//îÚÇ—èoÇ∑éûä‘
#define GMD_GMK_NEEDLE_NEON_ACTIVE_TIME			(180)	//ÉAÉNÉeÉBÉuéûä‘
#define GMD_GMK_NEEDLE_NEON_OFF_TIME			(10)	//à¯Ç¡çûÇﬁéûä‘

#define GMD_GMK_NEEDLE_NEON_MOVE_Y				(32)	//à⁄ìÆãóó£

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static GMS_ENEMY_3D_WORK* gmGmkNeedleNeonLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type );
static GMS_ENEMY_3D_WORK* gmGmkNeedleNeonLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type,
									u32 model_index);
static void gmGmkNeedleNeonDrawFunc( OBS_OBJECT_WORK* obj_work );
#if GMD_GMK_NEEDLE_NEON_TEST_TVX
static void gmGmkNeedleNeonStandDrawFunc( OBS_OBJECT_WORK* obj_work );
static void gmGmkNeedleNeonTvxDrawFunc( void* tvx, NNS_TEXLIST* texlist, VecFx32* base_pos );
#endif // GMD_GMK_NEEDLE_NEON_TEST_TVX

static void gmGmkNeedleNeonStandInit( OBS_OBJECT_WORK* obj_work );
static void gmGmkNeedleNeonNeedleInit( OBS_OBJECT_WORK* obj_work );
static void gmGmkNeedleNeonNeedleChangeModeWait( OBS_OBJECT_WORK* obj_work );
static void gmGmkNeedleNeonNeedleChangeModeOn( OBS_OBJECT_WORK* obj_work );
static void gmGmkNeedleNeonNeedleChangeModeActive( OBS_OBJECT_WORK* obj_work );
static void gmGmkNeedleNeonNeedleChangeModeOff( OBS_OBJECT_WORK* obj_work );
static void gmGmkNeedleNeonNeedleMainWait( OBS_OBJECT_WORK* obj_work );
static void gmGmkNeedleNeonNeedleMainOn( OBS_OBJECT_WORK* obj_work );
static void gmGmkNeedleNeonNeedleMainActive( OBS_OBJECT_WORK* obj_work );
static void gmGmkNeedleNeonNeedleMainOff( OBS_OBJECT_WORK* obj_work );
static void gmGmkNeedleNeonNeedleUpdateHitRect( OBS_OBJECT_WORK* obj_work );

/*static void gmGmkNeedleNeonGlaerInit( OBS_OBJECT_WORK* obj_work );
static void gmGmkNeedleNeonGlaerMain( OBS_OBJECT_WORK* obj_work );*/


//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//ï\é¶à íu
static const VecFx32 g_gm_gmk_disp_offset[GMD_GMK_NEEDLE_NEON_DRAW_NUM] = {
	{  0 * FX32_ONE, 0,  0 * FX32_ONE},		//íÜâõ
	{-10 * FX32_ONE, 0,  0 * FX32_ONE},		//éËëOç∂
	{ 10 * FX32_ONE, 0,  0 * FX32_ONE},		//éËëOâE
	{ -5 * FX32_ONE, 0, -2 * FX32_ONE},		//âúç∂
	{  5 * FX32_ONE, 0, -2 * FX32_ONE},		//âúâE
};


static OBS_ACTION3D_NN_WORK* g_gm_gmk_needle_neon_obj_3d_list = NULL;

#if GMD_GMK_NEEDLE_NEON_TEST_TVX
static AMS_AMB_HEADER* g_gm_gmk_needle_neon_obj_tvx_list = NULL;
#endif // GMD_GMK_NEEDLE_NEON_TEST_TVX
//----- Global Functions ----------------------------------------------------

// ==========================================================================
// GmGmkNeedleNeonBuild
/*!
 *	ÉMÉ~ÉbÉN ÉlÉIÉìêj ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkNeedleNeonBuild(void)
{
	g_gm_gmk_needle_neon_obj_3d_list = GmGameDBuildRegBuildModel(
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_NEEDLE_MODEL ),
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_NEEDLE_TEX ),
			0	//draw_flag
			);
#if GMD_GMK_NEEDLE_NEON_TEST_TVX
	void* tvx = GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_NEEDLE_TVX );
	amBindConv((Uint8*)tvx);
	g_gm_gmk_needle_neon_obj_tvx_list = (AMS_AMB_HEADER*)tvx;
#endif // GMD_GMK_NEEDLE_NEON_TEST_TVX
}

// ==========================================================================
// GmGmkNeedleNeonFlush
/*!
 *	ÉMÉ~ÉbÉN ÉlÉIÉìêj ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkNeedleNeonFlush(void)
{
	AMS_AMB_HEADER* amb_header = (AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_NEEDLE_MODEL );

	GmGameDBuildRegFlushModel( g_gm_gmk_needle_neon_obj_3d_list, amb_header->file_num );
	g_gm_gmk_needle_neon_obj_3d_list = NULL;
#if GMD_GMK_NEEDLE_NEON_TEST_TVX
	g_gm_gmk_needle_neon_obj_tvx_list = NULL;
#endif // GMD_GMK_NEEDLE_NEON_TEST_TVX
}

// ==========================================================================
// GmGmkNeedleNeonInitStand
/*!
 *	ÉMÉ~ÉbÉNèâä˙âªä÷êîÅ@ÉlÉIÉìêj
 *
 *	@param eve_rec	[io] ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param pos_x	[in] èoåªç¿ïW
 *	@param pos_y	[in] 
 *	@param type		[in] èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkNeedleNeonInitStand( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	//ÉIÉuÉWÉFÉNÉgçÏê¨
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkNeedleNeonLoadObj( 
			eve_rec, 
			pos_x, 
			pos_y, 
			type, 
			IDB_GMK_NEEDLE_MDL_GMK_NEEDLE_STAND_ZNO );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//èâä˙âª
	gmGmkNeedleNeonStandInit( obj_work );

	//êj
	OBS_OBJECT_WORK* obj_work_needle = GmEventMgrLocalEventBirth(
			GMD_EVENT_ID_NEEDLE_NEON_NEEDLE,
			obj_work->pos.x,
			obj_work->pos.y,
			eve_rec->flag,
			eve_rec->left,
			eve_rec->top,
			eve_rec->width,
			eve_rec->height,
			type );
	obj_work_needle->parent_obj = obj_work;

	//ÉÜÅ[ÉUÉèÅ[ÉNÇ…ÉfÉtÉHÉãÉgÇÃYç¿ïWÇê›íËÅià¯Ç¡çûÇÒÇæèÛë‘Åj
	obj_work_needle->user_work = (u32)(obj_work->pos.y + GMD_GMK_NEEDLE_NEON_MOVE_Y*FX32_ONE);

	return obj_work;
}

// ==========================================================================
// GmGmkNeedleNeonInitNeedle
/*!
 *	ÉMÉ~ÉbÉNèâä˙âªä÷êîÅ@ÉlÉIÉìêj
 *
 *	@param eve_rec	[io] ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param pos_x	[in] èoåªç¿ïW
 *	@param pos_y	[in] 
 *	@param type		[in] èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkNeedleNeonInitNeedle( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	//ÉIÉuÉWÉFÉNÉgçÏê¨
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkNeedleNeonLoadObj( 
			eve_rec, 
			pos_x, 
			pos_y, 
			type, 
			IDB_GMK_NEEDLE_MDL_GMK_NEEDLE_F_ZNO );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	// É}ÉeÉäÉAÉã
	/*OBS_DATA_WORK* data_mat_motion = ObjDataGet(GMD_DWORK_NO_GMK_NEEDLE_NEON_MAT);
	amAssert( data_mat_motion );
	ObjObjectAction3dNNMaterialMotionLoad( 
			obj_work,
			0,
			data_mat_motion,
			NULL,
			0,
			NULL
	);
	ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_NEEDLE_NEON_MAT_GMK_NEEDLE_Z2B_F_ZNV );
	*/

	//èâä˙âª
	gmGmkNeedleNeonNeedleInit( obj_work );

	//ÉOÉåÉA
	/*OBS_OBJECT_WORK* obj_work_glaer = GmEventMgrLocalEventBirth(
			GMD_EVENT_ID_NEEDLE_NEON_GLAER,
			obj_work->pos.x,
			obj_work->pos.y,
			eve_rec->flag,
			eve_rec->left,
			eve_rec->top,
			eve_rec->width,
			eve_rec->height,
			type );
	obj_work_glaer->parent_obj = obj_work;*/

	return obj_work;
}

// ==========================================================================
// GmGmkNeedleNeonInitNeedle
/*!
 *	ÉMÉ~ÉbÉNèâä˙âªä÷êîÅ@ÉlÉIÉìêj
 *
 *	@param eve_rec	[io] ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param pos_x	[in] èoåªç¿ïW
 *	@param pos_y	[in] 
 *	@param type		[in] èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkNeedleNeonInitGlaer( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	UNREFERENCED_PARAMETER(eve_rec);
	UNREFERENCED_PARAMETER(pos_x);
	UNREFERENCED_PARAMETER(pos_y);
	UNREFERENCED_PARAMETER(type);
	return NULL;
/*
	//ÉIÉuÉWÉFÉNÉgçÏê¨
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkNeedleNeonLoadObj( 
			eve_rec, 
			pos_x, 
			pos_y, 
			type, 
			IDB_GMK_NEEDLE_NEON_MDL_GMK_NEEDLE_Z2B_G_ZNO );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	// É}ÉeÉäÉAÉã
	OBS_DATA_WORK* data_mat_motion = ObjDataGet(GMD_DWORK_NO_GMK_NEEDLE_NEON_MAT);
	amAssert( data_mat_motion );
	ObjObjectAction3dNNMaterialMotionLoad( 
			obj_work,
			0,
			data_mat_motion,
			NULL,
			0,
			NULL
	);
	ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_NEEDLE_NEON_MAT_GMK_NEEDLE_Z2B_G_ZNV );


	//èâä˙âª
	gmGmkNeedleNeonGlaerInit( obj_work );

	return obj_work;*/
}

// ==========================================================================
// GmGmkNeedleNeonChangeModeActive
/*!
 *	ÉAÉNÉeÉBÉuÇ…ïœçXÅ@ÉlÉIÉìêj
 *
 *	@param obj_work	[io] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void GmGmkNeedleNeonChangeModeActive( OBS_OBJECT_WORK* obj_work )
{
	obj_work->user_flag &= ~GMD_GMK_NEEDLE_NEON_FLAG_MODE_TIMER;

	//ïœçXçœÇ›
	if ( obj_work->ppFunc == gmGmkNeedleNeonNeedleMainOn
			|| obj_work->ppFunc == gmGmkNeedleNeonNeedleMainActive
	){
		return;
	}

	gmGmkNeedleNeonNeedleChangeModeOn( obj_work );
}

// ==========================================================================
// GmGmkNeedleNeonChangeModeWait
/*!
 *	ë“ã@Ç…ïœçXÅ@ÉlÉIÉìêj
 *
 *	@param obj_work	[io] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void GmGmkNeedleNeonChangeModeWait( OBS_OBJECT_WORK* obj_work )
{
	obj_work->user_flag &= ~GMD_GMK_NEEDLE_NEON_FLAG_MODE_TIMER;

	//ïœçXçœÇ›
	if ( obj_work->ppFunc == gmGmkNeedleNeonNeedleMainWait
			|| obj_work->ppFunc == gmGmkNeedleNeonNeedleMainOff
	){
		return;
	}

	gmGmkNeedleNeonNeedleChangeModeOff( obj_work );
}

// ==========================================================================
// GmGmkNeedleNeonChangeModeTimer
/*!
 *	éûå¿éÆÇ…ïœçXÅ@ÉlÉIÉìêj
 *
 *	@param obj_work	[io] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void GmGmkNeedleNeonChangeModeTimer( OBS_OBJECT_WORK* obj_work )
{
	obj_work->user_flag |= GMD_GMK_NEEDLE_NEON_FLAG_MODE_TIMER;
}

//----- Local Functions -----------------------------------------------------

// ==========================================================================
// gmGmkNeedleNeonLoadObjNoModel
/*!
 *	ÉMÉ~ÉbÉNì«Ç›çûÇ›ÉÇÉfÉãÇ»Çµ
 *
 *	@param eve_rec	[in] ÉCÉxÉìÉgÉåÉRÅ[Éh
 *	@param pos_x	[in] ç¿ïWX
 *	@param pos_y	[in] ç¿ïWY
 *	@param type		[in] É^ÉCÉv
 *
 *	@return ÉèÅ[ÉN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkNeedleNeonLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type )
{

	UNREFERENCED_PARAMETER(type);

	//-------------------------------------------------
	// ÉèÅ[ÉNèâä˙âª
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec, 
			pos_x, 
			pos_y, 
			sizeof(GMS_ENEMY_3D_WORK), 
			"GMK_NEEDLE_NEON");
	amAssert( gimmick_work );

	//-------------------------------------------------
	//ãÈå`èâä˙âª
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}

// ==========================================================================
// gmGmkNeedleNeonLoadObj
/*!
 *	ÉMÉ~ÉbÉNì«Ç›çûÇ›
 *
 *	@param eve_rec		[in] ÉCÉxÉìÉgÉåÉRÅ[Éh
 *	@param pos_x		[in] ç¿ïWX
 *	@param pos_y		[in] ç¿ïWY
 *	@param type			[in] É^ÉCÉv
 *	@param model_index	[in] ÉÇÉfÉãÉCÉìÉfÉNÉX
 *
 *	@return ÉèÅ[ÉN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkNeedleNeonLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type,
									u32 model_index)
{

	//-------------------------------------------------
	// ÉèÅ[ÉNèâä˙âª
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkNeedleNeonLoadObjNoModel(
			eve_rec,
			pos_x,
			pos_y,
			type );
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//-------------------------------------------------
	// ÉÇÉfÉãèâä˙âª
	//-------------------------------------------------
	//ì«Ç›çûÇ›
	ObjObjectCopyAction3dNNModel(
		obj_work,
		&g_gm_gmk_needle_neon_obj_3d_list[model_index],
		&gimmick_work->obj_3d);

	return gimmick_work;
}

// ==========================================================================
// gmGmkNeedleNeonDrawFunc
/*!
 *	ÉMÉ~ÉbÉN êj ï`âÊê›íËèàóù
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkNeedleNeonDrawFunc( OBS_OBJECT_WORK* obj_work )
{
	if (!GmMainIsDrawEnable()) {
		return;
	}
#if GMD_GMK_NEEDLE_NEON_TEST_TVX
	if (obj_work->ppFunc == gmGmkNeedleNeonNeedleMainWait) {
		return;
	}
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}
	gmGmkNeedleNeonTvxDrawFunc(amBindGet(g_gm_gmk_needle_neon_obj_tvx_list, IDB_MODEL_GMK_NEEDLE_F_TVX), obj_work->obj_3d->texlist, &obj_work->pos);
#else
	for ( s32 i = 0; GMD_GMK_NEEDLE_NEON_DRAW_NUM > i; ++i ){
		obj_work->ofst.x = g_gm_gmk_disp_offset[i].x;
		obj_work->ofst.y = g_gm_gmk_disp_offset[i].y;
		obj_work->ofst.z = g_gm_gmk_disp_offset[i].z;

		ObjDrawActionSummary( obj_work );
	}
#endif // GMD_GMK_NEEDLE_NEON_TEST_TVX
}
#if GMD_GMK_NEEDLE_NEON_TEST_TVX
static void gmGmkNeedleNeonStandDrawFunc( OBS_OBJECT_WORK* obj_work )
{
	if (!GmMainIsDrawEnable()) {
		return;
	}
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}
	gmGmkNeedleNeonTvxDrawFunc(amBindGet(g_gm_gmk_needle_neon_obj_tvx_list, IDB_MODEL_GMK_NEEDLE_STAND_TVX), obj_work->obj_3d->texlist, &obj_work->pos);
}
static void gmGmkNeedleNeonTvxDrawFunc( void* tvx, NNS_TEXLIST* texlist, VecFx32* base_pos )
{
	VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
	
	for ( s32 i = 0; GMD_GMK_NEEDLE_NEON_DRAW_NUM > i; ++i ){
		VecFx32 pos;
		pos.x = base_pos->x + g_gm_gmk_disp_offset[i].x;
		pos.y = base_pos->y + g_gm_gmk_disp_offset[i].y;
		pos.z = base_pos->z + g_gm_gmk_disp_offset[i].z;
		
		GmTvxSetModel(tvx, texlist, &pos, &scale, 0, 0);
	}
}
#endif // GMD_GMK_NEEDLE_NEON_TEST_TVX


// ==========================================================================
//ÉXÉ^ÉìÉh
// ==========================================================================

// ==========================================================================
// gmGmkNeedleNeonStandInit
/*!
 *	ÉMÉ~ÉbÉNÅ@ÉlÉIÉìêjÅiÉXÉ^ÉìÉhÅjèâä˙âª
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkNeedleNeonStandInit( OBS_OBJECT_WORK* obj_work )
{	
	//-------------------------------------------------
	// ÉèÅ[ÉNê›íË
	//-------------------------------------------------
	//ÉtÉâÉO
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_MOVE_UNDER;
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	//ç¿ïW
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_B_BACK;

	//-------------------------------------------------
	// ÉÅÉCÉìèàóù
	//-------------------------------------------------
	obj_work->ppFunc = NULL;
	obj_work->ppMove = NULL;
#if GMD_GMK_NEEDLE_NEON_TEST_TVX
	obj_work->ppOut = gmGmkNeedleNeonStandDrawFunc;
#else
	obj_work->ppOut = gmGmkNeedleNeonDrawFunc;
#endif // GMD_GMK_NEEDLE_NEON_TEST_TVX
}




// ==========================================================================
//êj
// ==========================================================================

// ==========================================================================
// gmGmkNeedleNeonNeedleInit
/*!
 *	ÉMÉ~ÉbÉNÅ@ÉlÉIÉìêjèâä˙âª
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkNeedleNeonNeedleInit( OBS_OBJECT_WORK* obj_work )
{	
	//-------------------------------------------------
	//ãÈå`
	//-------------------------------------------------
	//ínå`
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );
	gimmick_work->ene_com.col_work.obj_col.obj = &gimmick_work->ene_com.obj_work;
	gimmick_work->ene_com.col_work.obj_col.width = 24;
	gimmick_work->ene_com.col_work.obj_col.height = 30;
	gimmick_work->ene_com.col_work.obj_col.ofst_x = -12;
	gimmick_work->ene_com.col_work.obj_col.ofst_y = -32;

	//çUåÇ
	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	ObjRectWorkZSet(
			rect_work, 
			-16, -33, -500,
			16, -8, 500 );
	rect_work->flag |= OBD_RECT_OUT;
	//çUåÇãÈå`ñ≥å¯
	obj_work->flag |= OBD_OBJECT_NOHIT;

	//-------------------------------------------------
	// ÉèÅ[ÉNê›íË
	//-------------------------------------------------
	//ÉtÉâÉO
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_MOVE_UNDER;
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->flag |= OBD_OBJECT_NOCLIP;

	//ç¿ïW
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_B_BACK;

	//-------------------------------------------------
	// ÉÅÉCÉìèàóù
	//-------------------------------------------------
	obj_work->ppMove = NULL;
	obj_work->ppOut = gmGmkNeedleNeonDrawFunc;


	//ë“ã@èÛë‘Ç÷
	gmGmkNeedleNeonNeedleChangeModeWait( obj_work );
}


// ==========================================================================
// gmGmkNeedleNeonNeedleChangeModeWait
/*!
 *	ë“ã@èÛë‘Ç÷
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkNeedleNeonNeedleChangeModeWait( OBS_OBJECT_WORK* obj_work )
{
	/*
	//ínå`ï”ÇËÇñ≥å¯Ç…
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );
	gimmick_work->ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NOHIT;

	//É}ÉeÉäÉAÉãïœçX
	ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_NEEDLE_NEON_MAT_GMK_NEEDLE_Z2B_F_ZNV );
	obj_work->disp_flag |= OBD_DISP_STOP;
	
	//ÉVÉOÉiÉã
	obj_work->user_flag |= GMD_GMK_NEEDLE_NEON_SIGNAL_OFF;
	*/

	//ë“ã@èÛë‘Ç÷
	obj_work->ppFunc = gmGmkNeedleNeonNeedleMainWait;
}

// ==========================================================================
// gmGmkNeedleNeonNeedleChangeModeOn
/*!
 *	îÚÇ—èoÇ∑èÛë‘Ç÷
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkNeedleNeonNeedleChangeModeOn( OBS_OBJECT_WORK* obj_work )
{
	//îÚÇ—èoÇ∑èÛë‘Ç÷
	obj_work->ppFunc = gmGmkNeedleNeonNeedleMainOn;
	
	//å¯â âπ
	GmSoundPlaySE( "Boss2_06" );

	//É}ÉeÉäÉAÉãïœçX
	//ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_NEEDLE_NEON_MAT_GMK_NEEDLE_Z2B_F_ZNV );
	//obj_work->disp_flag &= ~OBD_DISP_STOP;
	
	//ÉVÉOÉiÉã
	//obj_work->user_flag |= GMD_GMK_NEEDLE_NEON_SIGNAL_ON;
}

// ==========================================================================
// gmGmkNeedleNeonNeedleChangeModeActive
/*!
 *	ÉAÉNÉeÉBÉuèÛë‘Ç÷
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkNeedleNeonNeedleChangeModeActive( OBS_OBJECT_WORK* obj_work )
{
	/*
	//ínå`ï”ÇËÇóLå¯Ç…
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );
	gimmick_work->ene_com.col_work.obj_col.flag &= ~OBD_COLOBJ_NOHIT;

	//É}ÉeÉäÉAÉãïœçX
	ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_NEEDLE_NEON_MAT_GMK_NEEDLE_Z2B_F_ON_ZNV );
	obj_work->disp_flag |= OBD_DISP_REPEAT;
	
	//ÉVÉOÉiÉã
	obj_work->user_flag |= GMD_GMK_NEEDLE_NEON_SIGNAL_ACTIVE;
	*/

	//ÉAÉNÉeÉBÉuèÛë‘Ç÷
	obj_work->ppFunc = gmGmkNeedleNeonNeedleMainActive;
}

// ==========================================================================
// gmGmkNeedleNeonNeedleChangeModeOff
/*!
 *	à¯Ç¡çûÇﬁèÛë‘Ç÷
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkNeedleNeonNeedleChangeModeOff( OBS_OBJECT_WORK* obj_work )
{
	//çUåÇãÈå`ñ≥å¯
	obj_work->flag |= OBD_OBJECT_NOHIT;

	//ÉAÉNÉeÉBÉuèÛë‘Ç÷
	obj_work->ppFunc = gmGmkNeedleNeonNeedleMainOff;

	//É}ÉeÉäÉAÉãïœçX
	//ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_NEEDLE_NEON_MAT_GMK_NEEDLE_Z2B_F_ZNV );
	//obj_work->disp_flag |= OBD_DISP_STOP;
	
	//ÉVÉOÉiÉã
	//obj_work->user_flag |= GMD_GMK_NEEDLE_NEON_SIGNAL_OFF;
}

// ==========================================================================
// gmGmkNeedleNeonNeedleMainWait
/*!
 *	ë“ã@ÉÅÉCÉìèàóùÅiéûå¿Åj
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkNeedleNeonNeedleMainWait( OBS_OBJECT_WORK* obj_work )
{
	//ç¿ïW
	fx32 default_pos_y = (fx32)obj_work->user_work;
	obj_work->pos.y = default_pos_y;

	//éûå¿éÆÇ≈ÇÕÇ»Ç¢
	if ( !(obj_work->user_flag & GMD_GMK_NEEDLE_NEON_FLAG_MODE_TIMER) ){
		return;
	}

	//îÚÇ—èoÇ∑ë“Çø
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_GMK_NEEDLE_NEON_WAIT_TIME ){
		return;
	}
	obj_work->user_timer = 0;

	//îÚÇ—èoÇ∑èÛë‘Ç÷
	gmGmkNeedleNeonNeedleChangeModeOn( obj_work );
}

// ==========================================================================
// gmGmkNeedleNeonNeedleMainOn
/*!
 *	îÚÇ—èoÇ∑èÛë‘
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkNeedleNeonNeedleMainOn( OBS_OBJECT_WORK* obj_work )
{
	//çUåÇãÈå`ÉÇÅ[ÉhçXêV
	gmGmkNeedleNeonNeedleUpdateHitRect( obj_work );

	//ç¿ïW
	fx32 default_pos_y = (fx32)obj_work->user_work;
	fx32 move_y = FX_F32_TO_FX32(GMD_GMK_NEEDLE_NEON_MOVE_Y*obj_work->user_timer/GMD_GMK_NEEDLE_NEON_ON_TIME);
	obj_work->pos.y = default_pos_y - move_y;

	//èIóπë“Çø
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_GMK_NEEDLE_NEON_ON_TIME ){
		return;
	}
	obj_work->user_timer = 0;

	//ÉAÉNÉeÉBÉuèÛë‘Ç÷
	gmGmkNeedleNeonNeedleChangeModeActive( obj_work );
}

// ==========================================================================
// gmGmkNeedleNeonNeedleMainActive
/*!
 *	ÉAÉNÉeÉBÉuèÛë‘
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkNeedleNeonNeedleMainActive( OBS_OBJECT_WORK* obj_work )
{
	//çUåÇãÈå`ÉÇÅ[ÉhçXêV
	gmGmkNeedleNeonNeedleUpdateHitRect( obj_work );

	//ç¿ïW
	fx32 default_pos_y = (fx32)obj_work->user_work;
	obj_work->pos.y = default_pos_y - GMD_GMK_NEEDLE_NEON_MOVE_Y*FX32_ONE;

	//éûå¿éÆÇ≈ÇÕÇ»Ç¢
	if ( !(obj_work->user_flag & GMD_GMK_NEEDLE_NEON_FLAG_MODE_TIMER) ){
		return;
	}

	//èIóπë“Çø
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_GMK_NEEDLE_NEON_ACTIVE_TIME ){
		return;
	}
	obj_work->user_timer = 0;

	//à¯Ç¡çûÇﬁèÛë‘Ç÷
	gmGmkNeedleNeonNeedleChangeModeOff( obj_work );
}

// ==========================================================================
// gmGmkNeedleNeonNeedleMainOff
/*!
 *	à¯Ç¡çûÇﬁèÛë‘
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkNeedleNeonNeedleMainOff( OBS_OBJECT_WORK* obj_work )
{

	//ç¿ïW
	fx32 default_pos_y = (fx32)obj_work->user_work;
	fx32 move_y = FX_F32_TO_FX32(GMD_GMK_NEEDLE_NEON_MOVE_Y - GMD_GMK_NEEDLE_NEON_MOVE_Y*obj_work->user_timer/GMD_GMK_NEEDLE_NEON_OFF_TIME);
	obj_work->pos.y = default_pos_y - move_y;

	//èIóπë“Çø
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_GMK_NEEDLE_NEON_OFF_TIME ){
		return;
	}
	obj_work->user_timer = 0;

	//ë“ã@èÛë‘Ç÷
	gmGmkNeedleNeonNeedleChangeModeWait( obj_work );
}

// ==========================================================================
// gmGmkNeedleNeonUpdateHitRect
/*!
 *	çUåÇãÈå`ÉÇÅ[ÉhçXêV
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkNeedleNeonNeedleUpdateHitRect( OBS_OBJECT_WORK* obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//íNÇ©Ç™èÊÇ¡ÇƒÇ¢ÇÈèÍçáÅAçUåÇãÈå`ÇóLå¯Ç…
	const OBS_OBJECT_WORK* obj_work_rider = gimmick_work->ene_com.col_work.obj_col.rider_obj;
	if ( obj_work_rider ){
		obj_work->flag &= ~OBD_OBJECT_NOHIT;
	}
	else{
		obj_work->flag |= OBD_OBJECT_NOHIT;
	}
}


// ==========================================================================
//ÉOÉåÉA
// ==========================================================================

// ==========================================================================
// gmGmkNeedleNeonGlaerInit
/*!
 *	ÉMÉ~ÉbÉNÅ@ÉlÉIÉìêjèâä˙âª
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
/*void gmGmkNeedleNeonGlaerInit( OBS_OBJECT_WORK* obj_work )
{	

	//-------------------------------------------------
	// ÉèÅ[ÉNê›íË
	//-------------------------------------------------
	//ÉtÉâÉO
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_MOVE_UNDER;
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->flag |= OBD_OBJECT_NOCLIP;

	//ç¿ïW
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_B_BACK;

	//-------------------------------------------------
	// ÉÅÉCÉìèàóù
	//-------------------------------------------------
	obj_work->ppFunc = gmGmkNeedleNeonGlaerMain;
	obj_work->ppMove = NULL;
	obj_work->ppOut = gmGmkNeedleNeonDrawFunc;
}*/

// ==========================================================================
// gmGmkNeedleNeonGlaerMain
/*!
 *	ÉÅÉCÉìèàóù
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
/*void gmGmkNeedleNeonGlaerMain( OBS_OBJECT_WORK* obj_work )
{
	//êeèÛë‘Ç…çáÇÌÇπÇƒÉ}ÉeÉäÉAÉãïœçX
	OBS_OBJECT_WORK* needle_obj_work = obj_work->parent_obj;
	if ( !needle_obj_work ){
		return;
	}
	if ( needle_obj_work->user_flag & GMD_GMK_NEEDLE_NEON_SIGNAL_ON ){
		needle_obj_work->user_flag &= ~GMD_GMK_NEEDLE_NEON_SIGNAL_ON;
		ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_NEEDLE_NEON_MAT_GMK_NEEDLE_Z2B_G_ZNV );
		obj_work->disp_flag &= ~OBD_DISP_STOP;
	}
	if ( needle_obj_work->user_flag & GMD_GMK_NEEDLE_NEON_SIGNAL_ACTIVE ){
		needle_obj_work->user_flag &= ~GMD_GMK_NEEDLE_NEON_SIGNAL_ACTIVE;
		ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_NEEDLE_NEON_MAT_GMK_NEEDLE_Z2B_G_ON_ZNV );
		obj_work->disp_flag |= OBD_DISP_REPEAT;
	}
	if ( needle_obj_work->user_flag & GMD_GMK_NEEDLE_NEON_SIGNAL_OFF ){
		needle_obj_work->user_flag &= ~GMD_GMK_NEEDLE_NEON_SIGNAL_OFF;
		ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_NEEDLE_NEON_MAT_GMK_NEEDLE_Z2B_G_ZNV );
		obj_work->disp_flag |= OBD_DISP_STOP;
	}
}*/


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
