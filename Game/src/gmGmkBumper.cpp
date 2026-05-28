// ==========================================================================
/*!
  @file gmGmkBumper.cpp
  @brief ÉMÉ~ÉbÉN ÉoÉìÉpÅ[

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkBumper.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 *	GMS_EVE_RECORD_EVENT
 *		left		: Ç»Çµ
 *		top			: Ç»Çµ
 *		width		: Ç»Çµ
 *		height		: Ç»Çµ
 *
 *		flag		: 1 ÉzÅ[É~ÉìÉOâÒïúÇµÇ»Ç¢ÉtÉâÉO
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
#include "gmPlySeq.h"
#include "gmSound.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmPadVib.h"

#include "gmGmkBumper.h"

// ÉfÅ[É^ÉwÉbÉ_
#include "common/model/gmk_bumper_mdl.hmb"
#include "common/model/gmk_bumper_mtn.hmb"
#include "common/model/gmk_bumper_mat.hmb"


//----- Definitions ---------------------------------------------------------

//ÉoÉìÉpÅ[ÉÇÅ[Éh
enum GME_GMK_BUMPER_MODE{
	GMD_GMK_BUMPER_MODE_WAIT = 0,	//ÉoÉìÉpÅ[ë“Çø
	GMD_GMK_BUMPER_MODE_ACTIVE,		//ÉoÉìÉpÅ[é¿çs

	GMD_GMK_BUMPER_MODE_NUM
};

//ÉoÉìÉpÅ[É^ÉCÉv
enum GME_GMK_BUMPER_TYPE{
	GMD_GMK_BUMPER_TYPE_TRI_I_TOP = 0,	//ìÒìôï”éOäpå`Åiè„Ç…îzíuópÅj
	GMD_GMK_BUMPER_TYPE_TRI_I_BOTTOM,	//ìÒìôï”éOäpå`Åiâ∫Ç…îzíuópÅj
	GMD_GMK_BUMPER_TYPE_TRI_I_LEFT,		//ìÒìôï”éOäpå`Åiç∂Ç…îzíuópÅj
	GMD_GMK_BUMPER_TYPE_TRI_I_RIGHT,	//ìÒìôï”éOäpå`ÅiâEÇ…îzíuópÅj

	GMD_GMK_BUMPER_TYPE_TRI_R_LT,		//íºäpéOäpå`Åiç∂è„Ç…îzíuópÅj
	GMD_GMK_BUMPER_TYPE_TRI_R_LB,		//íºäpéOäpå`Åiç∂â∫Ç…îzíuópÅj
	GMD_GMK_BUMPER_TYPE_TRI_R_RT,		//íºäpéOäpå`ÅiâEè„Ç…îzíuópÅj
	GMD_GMK_BUMPER_TYPE_TRI_R_RB,		//íºäpéOäpå`ÅiâEâ∫Ç…îzíuópÅj

	GMD_GMK_BUMPER_TYPE_HEX_W,			//òZäpå`Åiâ°í∑Åj
	GMD_GMK_BUMPER_TYPE_HEX_H,			//òZäpå`Åiècí∑Åj

	GMD_GMK_BUMPER_TYPE_NUM
};

#define GMD_GMK_BUMPER_RECT_MARGIN_PLAYER_X	(0x00008000L)		//ìñÇΩÇËîªíËÉ}Å[ÉWÉì
#define GMD_GMK_BUMPER_RECT_MARGIN_PLAYER_Y	(0x00008000L)		//ìñÇΩÇËîªíËÉ}Å[ÉWÉì

#define GMD_GMK_BUMPER_RECT_OFFSET_PLAYER_Y	(-0x00003000L)		//ìñÇΩÇËîªíËÉIÉtÉZÉbÉg

#define GMD_GMK_BUMPER_POS_Z				(-30*FX32_ONE)		//ï\é¶à íu
#define GMD_GMK_BUMPER_OFFSET_RADIUS		( 16 * FX32_ONE )	//ÇﬂÇËçûÇ›ñhé~ãóó£

#define GMD_GMK_BUMPER_SPEED_MIN			( 2*FX32_ONE )		//ÇÕÇ∂Ç≠ç≈è¨ÉXÉsÅ[Éh


#define GMD_GMK_BUMPER_SPEED_X				( 4*FX32_ONE )		//ÇÕÇ∂Ç≠ÉXÉsÅ[Éh
#define GMD_GMK_BUMPER_SPEED_Y				( 6*FX32_ONE )		//ÇÕÇ∂Ç≠ÉXÉsÅ[Éh
#define GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I	( 3*FX32_ONE )		//ÇÕÇ∂Ç≠ÉXÉsÅ[ÉhÅiäpìxï ï‚ê≥Åj
#define GMD_GMK_BUMPER_SPEED_OFFSET_TRI_R	( 5*FX32_ONE )		//ÇÕÇ∂Ç≠ÉXÉsÅ[ÉhÅiäpìxï ï‚ê≥Åj

#define GMD_GMK_BUMPER_NO_MOVE_TIME_UD		( 5 )				//ÇÕÇ∂Ç©ÇÍÇΩå„ëÄçÏÇ≈Ç´Ç»Ç¢éûä‘
#define GMD_GMK_BUMPER_NO_MOVE_TIME_LR		( 15 )				//ÇÕÇ∂Ç©ÇÍÇΩå„ëÄçÏÇ≈Ç´Ç»Ç¢éûä‘
//ÉtÉâÉO
#define GMD_GMK_BUMPER_FLAG_NO_RECOVER_HOMING	(1)	//ÉzÅ[É~ÉìÉOâÒïúÇµÇ»Ç¢ÉtÉâÉO


//ÉoÉìÉpÅ[ÉèÅ[ÉN
typedef struct tag_GMS_GMK_BUMPER_WORK
{
	GMS_ENEMY_3D_WORK ene_3d;				// ÉGÉlÉ~Å[ÉèÅ[ÉN

	OBS_ACTION3D_NN_WORK obj_3d_parts;	//ÉpÅ[Éc
	GSS_SND_SE_HANDLE* se_handle;		//å¯â âπÉnÉìÉhÉã
} GMS_GMK_BUMPER_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static u32 gmGmkBumpereGameSystemGetSyncTime( void );
static GMS_ENEMY_3D_WORK* gmGmkBumperLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_BUMPER_TYPE type );
static GMS_ENEMY_3D_WORK* gmGmkBumperLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_BUMPER_TYPE type );

//---------------------------------------------------------
//ÉoÉìÉpÅ[
//---------------------------------------------------------
static void gmGmkBumperInit( OBS_OBJECT_WORK *obj_work, GME_GMK_BUMPER_TYPE bumper_type );
static void gmGmkBumperSetRect( GMS_ENEMY_3D_WORK* gimmick_work, GME_GMK_BUMPER_TYPE bumper_type );
static GME_GMK_BUMPER_TYPE gmGmkBumperCalcType( GME_EVENT_ID id );

static void gmGmkBumperDestFunc( MTS_TASK_TCB *tcb );
static void gmGmkBumperDrawFunc( OBS_OBJECT_WORK* work );
static void gmGmkBumperDefFunc( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* player_rect );
static BOOL gmGmkBumperCheckHit( 
						 const VecFx32* gimmick_pos,
						 const VecFx32* target_pos,
						 GME_GMK_BUMPER_TYPE type);
static BOOL gmGmkBumperCheckLeft( const VecFx32* line_start, const VecFx32* line_end, const VecFx32* point );


static BOOL gmGmkBumperCheckHitEffect( const GMS_GMK_BUMPER_WORK* bumper_work );

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//ÉÇÉfÉãID
static const s32 g_gm_gmk_bumper_model_id[GMD_GMK_BUMPER_TYPE_NUM] = {
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_ZNO,
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_ZNO,
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_ZNO,
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_ZNO,

	IDB_GMK_BUMPER_MDL_GMK_BUMPER_TRI_ZNO,
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_TRI_ZNO,
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_TRI_ZNO,
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_TRI_ZNO,

	IDB_GMK_BUMPER_MDL_GMK_BUMPER_SQ_ZNO,
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_SQ_ZNO,
};
#if !_IPHONE
//ÉTÉuÉÇÉfÉãID
static const s32 g_gm_gmk_bumper_sub_model_id[GMD_GMK_BUMPER_TYPE_NUM] = {
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_GLARE_ZNO,
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_GLARE_ZNO,
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_GLARE_ZNO,
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_GLARE_ZNO,

	IDB_GMK_BUMPER_MDL_GMK_BUMPER_TRI_GLARE_ZNO,
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_TRI_GLARE_ZNO,
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_TRI_GLARE_ZNO,
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_TRI_GLARE_ZNO,

	IDB_GMK_BUMPER_MDL_GMK_BUMPER_SQ_GLARE_ZNO,
	IDB_GMK_BUMPER_MDL_GMK_BUMPER_SQ_GLARE_ZNO,
};
#endif // !_IPHONE
//É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉìID
static const s32 g_gm_gmk_bumper_motion_id[GMD_GMK_BUMPER_TYPE_NUM] = {
	IDB_GMK_BUMPER_MAT_GMK_BUMPER_ZNV,
	IDB_GMK_BUMPER_MAT_GMK_BUMPER_ZNV,
	IDB_GMK_BUMPER_MAT_GMK_BUMPER_ZNV,
	IDB_GMK_BUMPER_MAT_GMK_BUMPER_ZNV,

	IDB_GMK_BUMPER_MAT_GMK_BUMPER_TRI_ZNV,
	IDB_GMK_BUMPER_MAT_GMK_BUMPER_TRI_ZNV,
	IDB_GMK_BUMPER_MAT_GMK_BUMPER_TRI_ZNV,
	IDB_GMK_BUMPER_MAT_GMK_BUMPER_TRI_ZNV,

	IDB_GMK_BUMPER_MAT_GMK_BUMPER_SQ_ZNV,
	IDB_GMK_BUMPER_MAT_GMK_BUMPER_SQ_ZNV,
};

//Zäpìx
static const u16 g_gm_gmk_bumper_angle_z[GMD_GMK_BUMPER_TYPE_NUM] = {
	0x8000,
	0,
	0x4000,
	0xc000,

	0x4000,
	0,
	0x8000,
	0xc000,

	0,
	0x4000,
};

//ãÈå`
static const s16 g_gmk_bumper_rect[GMD_GMK_BUMPER_TYPE_NUM][MTD_RECT] = {
	{-48,   0, 48, 28},
	{-48, -28, 48,  0},
	{  0, -48, 28, 48},
	{-28, -48,  0, 48},

	{  0,   0, 64, 64},
	{  0, -64, 64,  0},
	{-64,   0,  0, 64},
	{-64, -64,  0,  0},

	{-24, -8, 24, 8},
	{-8, -24, 8, 24},
};

//î≠åıÉGÉtÉFÉNÉgID
static const GME_EFCT_CMN_IDX g_gmk_bumper_effect_id_flush[GMD_GMK_BUMPER_TYPE_NUM] = {
	GME_EFCT_CMN_IDX_BUMPER_1,
	GME_EFCT_CMN_IDX_BUMPER_1,
	GME_EFCT_CMN_IDX_BUMPER_1,
	GME_EFCT_CMN_IDX_BUMPER_1,

	GME_EFCT_CMN_IDX_BUMPER_2,
	GME_EFCT_CMN_IDX_BUMPER_2,
	GME_EFCT_CMN_IDX_BUMPER_2,
	GME_EFCT_CMN_IDX_BUMPER_2,

	GME_EFCT_CMN_IDX_NONE,
	GME_EFCT_CMN_IDX_NONE,
};

//î≠åıÉGÉtÉFÉNÉgÉIÉtÉZÉbÉg
static const s32 g_gmk_bumper_effect_offset_flush[GMD_GMK_BUMPER_TYPE_NUM][MTD_XY] = {
	{ 0,  7},
	{ 0, -7},
	{ 7,  0},
	{-7,  0},

	{ 28,  28},
	{ 28, -28},
	{-28,  28},
	{-28, -28},

	{0, 0},
	{0, 0},
};

//ñ@ê¸
static OBS_ACTION3D_NN_WORK* g_gm_gmk_bumper_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkBumperBuild
/*!
 *	ÉMÉ~ÉbÉN ÉoÉìÉpÅ[ ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkBumperBuild(void)
{
	g_gm_gmk_bumper_obj_3d_list = GmGameDBuildRegBuildModel(
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BUMPER_MODEL ),
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BUMPER_TEX ),
			0	//draw_flag
			);
}

// ==========================================================================
// GmGmkBumperFlush
/*!
 *	ÉMÉ~ÉbÉN ÉoÉìÉpÅ[ ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkBumperFlush(void)
{
	AMS_AMB_HEADER* amb_header = (AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BUMPER_MODEL );

	GmGameDBuildRegFlushModel( g_gm_gmk_bumper_obj_3d_list, amb_header->file_num );
	g_gm_gmk_bumper_obj_3d_list = NULL;
}

// ==========================================================================
// GmGmkBumperInit
/*!
 *	ÉMÉ~ÉbÉNèâä˙âªä÷êî ÉoÉìÉpÅ[
 *
 *	@param eve_rec	[io] ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param pos_x	[in] èoåªç¿ïW
 *	@param pos_y	[in] 
 *	@param type		[in] 
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkBumperInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	UNREFERENCED_PARAMETER(type);
	//É^ÉCÉv
	GME_GMK_BUMPER_TYPE bumper_type = (GME_GMK_BUMPER_TYPE)(eve_rec->id - GMD_EVENT_ID_BUMPER_TRI_I_TOP);
	amAssert( GMD_GMK_BUMPER_TYPE_TRI_I_TOP <= bumper_type && bumper_type < GMD_GMK_BUMPER_TYPE_NUM );

	//ÉIÉuÉWÉFÉNÉgçÏê¨
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkBumperLoadObj( eve_rec, pos_x, pos_y, bumper_type );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK*obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//èâä˙âª
	gmGmkBumperInit( obj_work, bumper_type );
	return obj_work;
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkBumpereGameSystemGetSyncTime
/*!
 * ÉQÅ[ÉÄÉVÉXÉeÉÄÇ©ÇÁìØä˙éûä‘ÇéÊìæ
 *
 *	@return	ìØä˙éûä‘
 */
// ==========================================================================
u32 gmGmkBumpereGameSystemGetSyncTime( void )
{
	return g_gm_main_system.sync_time;
}

// ==========================================================================
// gmGmkBumperLoadObjNoModel
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
GMS_ENEMY_3D_WORK* gmGmkBumperLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_BUMPER_TYPE type )
{
	UNREFERENCED_PARAMETER(type);

	//-------------------------------------------------
	// ÉèÅ[ÉNèâä˙âª
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec, 
			pos_x, 
			pos_y, 
			sizeof(GMS_GMK_BUMPER_WORK), 
			"GMK_BUMPER");
	amAssert( gimmick_work );

	//-------------------------------------------------
	//ãÈå`èâä˙âª
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}

// ==========================================================================
// gmGmkBumperLoadObj
/*!
 *	ÉMÉ~ÉbÉNì«Ç›çûÇ›
 *
 *	@param eve_rec	[in] ÉCÉxÉìÉgÉåÉRÅ[Éh
 *	@param pos_x	[in] ç¿ïWX
 *	@param pos_y	[in] ç¿ïWY
 *	@param type		[in] É^ÉCÉv
 *
 *	@return ÉèÅ[ÉN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkBumperLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_BUMPER_TYPE type )
{

	//-------------------------------------------------
	// ÉèÅ[ÉNèâä˙âª
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkBumperLoadObjNoModel(
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
	s32 data_model_index = g_gm_gmk_bumper_model_id[type];
	//ì«Ç›çûÇ›
	ObjObjectCopyAction3dNNModel(
		obj_work,
		&g_gm_gmk_bumper_obj_3d_list[data_model_index],
		&gimmick_work->obj_3d);

	s32 data_motion_index = g_gm_gmk_bumper_motion_id[type];
	void* data_motion = ObjDataGet(GMD_DWORK_NO_GMK_BUMPER_MAT)->pData;
	amAssert( data_motion );

	//É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
	ObjObjectAction3dNNMaterialMotionLoad( 
			obj_work,
			0,				//reg_file_id
			NULL,			//data_work
			NULL,			//mtn_data_path
			data_motion_index,
			data_motion
	);
#if !_IPHONE
	//-------------------------------------------------
	//ÉTÉuÉÇÉfÉãèâä˙âª
	//-------------------------------------------------
	GMS_GMK_BUMPER_WORK* bumper_work = (GMS_GMK_BUMPER_WORK*)obj_work;
	s32 data_model_index_sub = g_gm_gmk_bumper_sub_model_id[type];
	ObjCopyAction3dNNModel(
		&g_gm_gmk_bumper_obj_3d_list[data_model_index_sub],
		&bumper_work->obj_3d_parts);
#endif // !_IPHONE
	return gimmick_work;
}

// ==========================================================================
// gmGmkBumperInit
/*!
 *	ÉMÉ~ÉbÉNÅ@ÉoÉìÉpÅ[Å@èâä˙âª
 *
 *	@param obj_work		[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *	@param bumper_type	[in] É^ÉCÉv
 */
// ==========================================================================
void gmGmkBumperInit( OBS_OBJECT_WORK *obj_work, GME_GMK_BUMPER_TYPE bumper_type )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );



	//-------------------------------------------------
	//ãÈå`èâä˙âª
	//-------------------------------------------------
	//ÉvÉåÉCÉÑÅ[Ç†ÇΩÇË
	gmGmkBumperSetRect( gimmick_work, bumper_type );

	//-------------------------------------------------
	// ÉèÅ[ÉNê›íË
	//-------------------------------------------------

	//ÉtÉâÉO
	obj_work->move_flag = OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;

	//å¸Ç´ï ê›íË
	obj_work->dir.z = g_gm_gmk_bumper_angle_z[bumper_type];

	//ÉÇÅ[ÉVÉáÉì
	ObjDrawObjectActionSet3DNNMaterial( obj_work, 0 );
	obj_work->disp_flag |= OBD_DISP_STOP | OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP;

	//ç¿ïW
	obj_work->pos.z = GMD_GMK_BUMPER_POS_Z;

	//å¯â âπ
	GMS_GMK_BUMPER_WORK* bumper_work = (GMS_GMK_BUMPER_WORK*)gimmick_work;
	bumper_work->se_handle = GsSoundAllocSeHandle();

	//-------------------------------------------------
	// ÉÅÉCÉìèàóù
	//-------------------------------------------------
	obj_work->ppFunc = NULL;
	obj_work->ppMove = NULL;
	obj_work->ppOut = gmGmkBumperDrawFunc;
	mtTaskChangeTcbDestructor( obj_work->tcb, gmGmkBumperDestFunc );
}

// ==========================================================================
// gmGmkBumperSetRect
/*!
 *	ÉMÉ~ÉbÉNÅ@ÉoÉìÉpÅ[Å@ãÈå`ê›íË
 *
 *	@param gimmick_work	[in] ÉMÉ~ÉbÉNÉèÅ[ÉN
 *	@param bumper_type	[in] É^ÉCÉv
 */
// ==========================================================================
void gmGmkBumperSetRect( GMS_ENEMY_3D_WORK* gimmick_work, GME_GMK_BUMPER_TYPE bumper_type )
{
	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	
	s16 left = g_gmk_bumper_rect[bumper_type][MTD_LEFT];	
	s16 right = g_gmk_bumper_rect[bumper_type][MTD_RIGHT];
	s16 top = g_gmk_bumper_rect[bumper_type][MTD_TOP];
	s16 bottom = g_gmk_bumper_rect[bumper_type][MTD_BOTTOM];

	ObjRectWorkZSet(
			rect_work,
			left, top, -500,
			right, bottom, 500);
	
	//ñhå‰óp
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	rect_work->ppDef = gmGmkBumperDefFunc;
}

// ==========================================================================
// gmGmkBumperDestFunc
/*!
 *	èIóπä÷êî
 *
 *	@param tcb	[in] TCB
 */
// ==========================================================================
void gmGmkBumperDestFunc( MTS_TASK_TCB *tcb )
{
	GMS_GMK_BUMPER_WORK* bumper_work = (GMS_GMK_BUMPER_WORK*)mtTaskGetTcbWork( tcb );

	//å¯â âπÉnÉìÉhÉãâï˙
	if ( bumper_work->se_handle ){
		GmSoundStopSE( bumper_work->se_handle );
		GsSoundFreeSeHandle( bumper_work->se_handle );
		bumper_work->se_handle = NULL;
	}

	//ïWèÄèIóπ
	GmEnemyDefaultExit( tcb );
}

// ==========================================================================
// gmGmkBumperDrawFunc
/*!
 *	ÉMÉ~ÉbÉNÅ@ÉoÉìÉpÅ[Å@ï`âÊä÷êî
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBumperDrawFunc( OBS_OBJECT_WORK* obj_work )
{
	OBS_ACTION3D_NN_WORK *obj_3d = obj_work->obj_3d;
	amAssert( obj_3d );
	if ( obj_3d->motion ){
		//ÉÇÅ[ÉVÉáÉìÇìØä˙Ç≥ÇπÇÈ
		float frame_start = amMotionMaterialGetStartFrame( obj_3d->motion, obj_3d->mat_act_id );
		float frame_end = amMotionMaterialGetEndFrame( obj_3d->motion, obj_3d->mat_act_id );
		float frame_max = frame_end - frame_start;

		float frame = (float)gmGmkBumpereGameSystemGetSyncTime();

		obj_3d->mat_frame = fmod( frame, frame_max );
	}


	ObjDrawActionSummary( obj_work );
#if !_IPHONE
	//---------------------------------------
	//ÉTÉuÉpÅ[Éc
	//---------------------------------------	
	u32 disp_flag = obj_work->disp_flag;
	
	GMS_GMK_BUMPER_WORK* bumper_work = (GMS_GMK_BUMPER_WORK*)obj_work;
	VecFx32 pos = obj_work->pos;
	pos.z += 16*FX32_ONE;

	ObjDrawAction3DNN(
			&bumper_work->obj_3d_parts,
			&pos, 
			&obj_work->dir, 
			&obj_work->scale, 
			&disp_flag );
#endif // !_IPHONE
}

// ==========================================================================
// gmGmkBumperCalcType
/*!
 *	ÉMÉ~ÉbÉNÅ@ÉoÉìÉpÅ[Å@É^ÉCÉvéÊìæ
 *
 *	@param id	[in] ÉCÉxÉìÉgID
 */
// ==========================================================================
GME_GMK_BUMPER_TYPE gmGmkBumperCalcType( GME_EVENT_ID id )
{
	GME_GMK_BUMPER_TYPE bumper_type = (GME_GMK_BUMPER_TYPE)(id - GMD_EVENT_ID_BUMPER_TRI_I_TOP);
	amAssert( GMD_GMK_BUMPER_TYPE_TRI_I_TOP <= bumper_type && bumper_type < GMD_GMK_BUMPER_TYPE_NUM );

	return bumper_type;
}

// ==========================================================================
// gmGmkBumperDefFunc
/*!
 *	ÉMÉ~ÉbÉNÅ@ÉoÉìÉpÅ[Å@äJénèÛë‘ãÈå`ä÷êî
 *
 *	@param gimmick_rect	[in] é©êgÇÃãÈå`ÉèÅ[ÉN
 *	@param target_rect	[in] ëäéËÇÃãÈå`ÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBumperDefFunc( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* player_rect )
{
	amAssert( gimmick_rect );
	amAssert( player_rect );

	//ÉMÉ~ÉbÉNÉèÅ[ÉN
	OBS_OBJECT_WORK* gimmick_obj_work = gimmick_rect->parent_obj;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)gimmick_obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* target_obj_work = player_rect->parent_obj;
	amAssert( target_obj_work );

	//ÉvÉåÉCÉÑÉVÅ[ÉPÉìÉXÇ™ÉXÉgÉbÉpÅ[ÇÃèÍçáèàóùÇµÇ»Ç¢
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)target_obj_work;
	if ( player_work->seq_state == GME_PLY_SEQ_STATE_GMK_STOPPER ){
		return;
	}


	fx32 margin_x = GMD_GMK_BUMPER_RECT_MARGIN_PLAYER_X;
	fx32 margin_y = GMD_GMK_BUMPER_RECT_MARGIN_PLAYER_Y;
	fx32 offset_y = GMD_GMK_BUMPER_RECT_OFFSET_PLAYER_Y;

	GME_GMK_BUMPER_TYPE bumper_type = gmGmkBumperCalcType( (GME_EVENT_ID)gimmick_work->ene_com.eve_rec->id );
	
	//ÉvÉåÉCÉÑÇÃèÍçáÅAãÈå`îªíË
	if ( target_obj_work->obj_type == GMD_OBJTYPE_PLAYER ){
		if ( !gmGmkBumperCheckHit(&gimmick_obj_work->pos, &target_obj_work->pos, bumper_type ) )
		{
			return ;
		}
	}

	//åXÇ´
	target_obj_work->dir.z = 0;

	//ÉXÉsÅ[Éhê›íË
	VecFx32 speed;
	speed.z = 0;

	speed.x =target_obj_work->spd.x;
	speed.y =target_obj_work->spd.y;	

	fx32 distance_x = (target_obj_work->pos.x) - gimmick_obj_work->pos.x;
	fx32 distance_y = (target_obj_work->pos.y+offset_y) - gimmick_obj_work->pos.y;

	fx32 abs_speed_x = MTM_MATH_ABS(speed.x);
	fx32 abs_speed_y = MTM_MATH_ABS(speed.y);

	s32 no_move_time = 0;
	switch ( bumper_type ){
	case GMD_GMK_BUMPER_TYPE_TRI_I_TOP:
		if ( distance_x > g_gmk_bumper_rect[GMD_GMK_BUMPER_TYPE_TRI_I_TOP][MTD_RIGHT]*FX32_ONE + margin_x ){
			speed.x = GMD_GMK_BUMPER_SPEED_X;
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_LR;
		}
		else if ( distance_x < g_gmk_bumper_rect[GMD_GMK_BUMPER_TYPE_TRI_I_TOP][MTD_LEFT]*FX32_ONE - margin_x ){
			speed.x = -GMD_GMK_BUMPER_SPEED_X;
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_LR;
		}
		else{
			speed.y = GMD_GMK_BUMPER_SPEED_Y;
			if ( distance_x < -margin_x ){
				speed.x -= GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}
			else if (distance_x > margin_x){
				speed.x += GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_UD;
		}
		break;
	case GMD_GMK_BUMPER_TYPE_TRI_I_BOTTOM:
		if ( distance_x > g_gmk_bumper_rect[GMD_GMK_BUMPER_TYPE_TRI_I_BOTTOM][MTD_RIGHT]*FX32_ONE ){
			speed.x = GMD_GMK_BUMPER_SPEED_X;
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_LR;
		}
		else if ( distance_x < g_gmk_bumper_rect[GMD_GMK_BUMPER_TYPE_TRI_I_BOTTOM][MTD_LEFT]*FX32_ONE ){
			speed.x = -GMD_GMK_BUMPER_SPEED_X;
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_LR;
		}
		else{
			speed.y = -GMD_GMK_BUMPER_SPEED_Y;
			if ( distance_x < -margin_x ){
				speed.x -= GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}
			else if (distance_x > margin_x){
				speed.x += GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}	
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_UD;
		}
		break;
	case GMD_GMK_BUMPER_TYPE_TRI_I_LEFT:
		if ( distance_y < g_gmk_bumper_rect[GMD_GMK_BUMPER_TYPE_TRI_I_LEFT][MTD_TOP]*FX32_ONE ){
			speed.y = -GMD_GMK_BUMPER_SPEED_Y;	
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_UD;
		}
		else if ( distance_y > g_gmk_bumper_rect[GMD_GMK_BUMPER_TYPE_TRI_I_LEFT][MTD_BOTTOM]*FX32_ONE ){
			speed.y = GMD_GMK_BUMPER_SPEED_Y;	
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_UD;
		}
		else {
			speed.x = GMD_GMK_BUMPER_SPEED_X;		
			if ( distance_y < -margin_y ){
				speed.y -= GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}
			else if (distance_y > margin_y){
				speed.y += GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}	
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_LR;
		}
		break;
	case GMD_GMK_BUMPER_TYPE_TRI_I_RIGHT:
		if ( distance_y < g_gmk_bumper_rect[GMD_GMK_BUMPER_TYPE_TRI_I_RIGHT][MTD_TOP]*FX32_ONE ){
			speed.y = -GMD_GMK_BUMPER_SPEED_Y;	
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_UD;
		}
		else if ( distance_y > g_gmk_bumper_rect[GMD_GMK_BUMPER_TYPE_TRI_I_RIGHT][MTD_BOTTOM]*FX32_ONE ){
			speed.y = GMD_GMK_BUMPER_SPEED_Y;	
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_UD;
		}
		else {
			speed.x = -GMD_GMK_BUMPER_SPEED_X;
			if ( distance_y < -margin_y ){
				speed.y -= GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}
			else if (distance_y > margin_y){
				speed.y += GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}	
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_LR;
		}
		break;
	case GMD_GMK_BUMPER_TYPE_TRI_R_LT:
		{
			speed.x = abs_speed_x;
			speed.y = abs_speed_y;
			speed.x = GMD_GMK_BUMPER_SPEED_OFFSET_TRI_R;
			speed.y = GMD_GMK_BUMPER_SPEED_OFFSET_TRI_R;
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_UD;
		}
		break;
	case GMD_GMK_BUMPER_TYPE_TRI_R_LB:
		{
			speed.x = abs_speed_x;
			speed.y = -abs_speed_y;
			speed.x = GMD_GMK_BUMPER_SPEED_OFFSET_TRI_R;
			speed.y = -GMD_GMK_BUMPER_SPEED_OFFSET_TRI_R;
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_UD;
		}
		break;
	case GMD_GMK_BUMPER_TYPE_TRI_R_RT:
		{
			speed.x = -abs_speed_x;
			speed.y = abs_speed_y;
			speed.x = -GMD_GMK_BUMPER_SPEED_OFFSET_TRI_R;
			speed.y = GMD_GMK_BUMPER_SPEED_OFFSET_TRI_R;
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_UD;
		}
		break;
	case GMD_GMK_BUMPER_TYPE_TRI_R_RB:
		{
			speed.x = -abs_speed_x;
			speed.y = -abs_speed_y;
			speed.x = -GMD_GMK_BUMPER_SPEED_OFFSET_TRI_R;
			speed.y = -GMD_GMK_BUMPER_SPEED_OFFSET_TRI_R;
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_UD;
		}
		break;
	case GMD_GMK_BUMPER_TYPE_HEX_W:
		if ( distance_x > g_gmk_bumper_rect[GMD_GMK_BUMPER_TYPE_HEX_W][MTD_RIGHT]*FX32_ONE ){
			speed.x = GMD_GMK_BUMPER_SPEED_X;
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_LR;
		}
		else if ( distance_x < g_gmk_bumper_rect[GMD_GMK_BUMPER_TYPE_HEX_W][MTD_LEFT]*FX32_ONE ){
			speed.x = -GMD_GMK_BUMPER_SPEED_X;
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_LR;
		}
		else if ( distance_y < -margin_y ){
			speed.y = -GMD_GMK_BUMPER_SPEED_Y;
			if ( distance_x < -margin_x ){
				speed.x -= GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}
			else if (distance_x > margin_x){
				speed.x += GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_UD;
		}
		else{
			speed.y = GMD_GMK_BUMPER_SPEED_Y;
			if ( distance_x < -margin_x ){
				speed.x -= GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}
			else if (distance_x > margin_x){
				speed.x += GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_UD;
		}
		break;
	case GMD_GMK_BUMPER_TYPE_HEX_H:
		if ( distance_y < g_gmk_bumper_rect[GMD_GMK_BUMPER_TYPE_HEX_H][MTD_TOP]*FX32_ONE ){
			speed.y = -GMD_GMK_BUMPER_SPEED_Y;
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_UD;
		}
		else if ( distance_y > g_gmk_bumper_rect[GMD_GMK_BUMPER_TYPE_HEX_H][MTD_BOTTOM]*FX32_ONE ){
			speed.y = GMD_GMK_BUMPER_SPEED_Y;
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_UD;
		}
		else if ( distance_x < -margin_x ){
			speed.x = -GMD_GMK_BUMPER_SPEED_X;
			if ( distance_y < -margin_y ){
				speed.y -= GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}
			else if (distance_x > margin_x){
				speed.y += GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_LR;
		}
		else{
			speed.x = GMD_GMK_BUMPER_SPEED_X;
			if ( distance_y < -margin_y ){
				speed.y -= GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}
			else if (distance_x > margin_x){
				speed.y += GMD_GMK_BUMPER_SPEED_OFFSET_TRI_I;
			}	
			no_move_time = GMD_GMK_BUMPER_NO_MOVE_TIME_LR;
		}
		break;
	default:
		break;
	}
	//ÉNÉäÉbÉv
	speed.x = MTM_MATH_CLIP( speed.x, -GMD_GMK_BUMPER_SPEED_X, GMD_GMK_BUMPER_SPEED_X );
	speed.y = MTM_MATH_CLIP( speed.y, -GMD_GMK_BUMPER_SPEED_Y, GMD_GMK_BUMPER_SPEED_Y );
	
	//ÉvÉåÉCÉÑÇ»ÇÁ
	if ( target_obj_work->obj_type == GMD_OBJTYPE_PLAYER ){
		//ÉzÅ[É~ÉìÉOâÒïúÇµÇ»Ç¢ÉtÉâÉO
		BOOL flag_no_recover_homing = FALSE;
		if ( gimmick_work->ene_com.eve_rec->flag & GMD_GMK_BUMPER_FLAG_NO_RECOVER_HOMING ){
			flag_no_recover_homing = TRUE;
		}
		//ÉvÉåÉCÉÑÉVÅ[ÉPÉìÉX
		GmPlySeqInitPinballAir(
				(GMS_PLAYER_WORK*)target_obj_work,
				speed.x,
				speed.y,
				no_move_time,
				flag_no_recover_homing );
	}
	//ÇªÇÍà»äO
	else if ( target_obj_work->obj_type == GMD_OBJTYPE_ENEMY ){
		//É{ÉX2à»äO
		GMS_ENEMY_3D_WORK* enemy_work = (GMS_ENEMY_3D_WORK*)target_obj_work;
		if ( enemy_work->ene_com.eve_rec->id != GMD_EVENT_ID_BOSS2_BODY ){
			return;
		}
		target_obj_work->spd.x = speed.x;
		target_obj_work->spd.y = speed.y;
		target_obj_work->spd_add.x = 0;
		target_obj_work->spd_add.y = 0;

		//ã≤Ç‹ÇÁÇ»Ç¢ÇÊÇ§Ç…ï‚ê≥Ç∑ÇÈ
		if (MTM_MATH_ABS(target_obj_work->spd.x) < 0x00000100L){
			target_obj_work->spd.x = 0x00000100L;
		}
		else if (MTM_MATH_ABS(target_obj_work->spd.y) < 0x00000100L){
			target_obj_work->spd.y = 0x00000100L;
		}
	}
	
	//Hitå¯â ÇçsÇ§Ç©îªíË
	GMS_GMK_BUMPER_WORK* bumper_work = (GMS_GMK_BUMPER_WORK*)gimmick_obj_work;
	if ( gmGmkBumperCheckHitEffect(bumper_work) ){
		//å¯â âπ
		if ( bumper_work->se_handle ){
			GmSoundPlaySE("Casino6", bumper_work->se_handle);
		}

		//î≠åıÉGÉtÉFÉNÉg
		GME_EFCT_CMN_IDX effect_id_hit = g_gmk_bumper_effect_id_flush[bumper_type];
		if ( effect_id_hit != GME_EFCT_CMN_IDX_NONE ){
			GMS_EFFECT_3DES_WORK* efct_work_flush = GmEfctCmnEsCreate(
					gimmick_obj_work, 
					effect_id_hit );
			amAssert( efct_work_flush );

			fx32 effect_offset_hit_x = (fx32)(g_gmk_bumper_effect_offset_flush[bumper_type][MTD_X]*FX32_ONE);
			fx32 effect_offset_hit_y = (fx32)(g_gmk_bumper_effect_offset_flush[bumper_type][MTD_Y]*FX32_ONE);
			efct_work_flush->efct_com.obj_work.pos.x = gimmick_obj_work->pos.x + effect_offset_hit_x;
			efct_work_flush->efct_com.obj_work.pos.y = gimmick_obj_work->pos.y + effect_offset_hit_y;
			efct_work_flush->efct_com.obj_work.pos.z = GMD_OBJ_DEFAULT_POS_Z_A_FRONT;
			efct_work_flush->efct_com.obj_work.dir.z = gimmick_obj_work->dir.z;
		}

		// è’åÇÉGÉtÉFÉNÉg
		GMS_EFFECT_3DES_WORK* effect_work_hit = GmEfctCmnEsCreate(
				gimmick_obj_work,
				GME_EFCT_CMN_IDX_BUMPER );
		amAssert( effect_work_hit );
		effect_work_hit->efct_com.obj_work.pos.x = target_obj_work->pos.x;
		effect_work_hit->efct_com.obj_work.pos.y = target_obj_work->pos.y;
		effect_work_hit->efct_com.obj_work.pos.z = GMD_OBJ_GIMMICK_POS_Z_FRONT;
		effect_work_hit->efct_com.obj_work.dir.z = (u16)(nnArcTan2( speed.y, speed.x ) - 0x4000);

		//êUìÆ
		GMM_PAD_VIB_SMALL();
	}
}


// ==========================================================================
// gmGmkBumperCheckHit
/*!
 *	ÉMÉ~ÉbÉNÅ@ÉoÉìÉpÅ[Å@ÉqÉbÉgîªíË
 *
 *	@param gimmick_obj_work	[in] ÉMÉ~ÉbÉNÇÃç¿ïW
 *	@param target_obj_work	[in] É^Å[ÉQÉbÉgÇÃ
 *	@param type				[in] ÉoÉìÉpÅ[É^ÉCÉv
 */
// ==========================================================================
BOOL gmGmkBumperCheckHit( 
						 const VecFx32* gimmick_pos,
						 const VecFx32* target_pos,
						 GME_GMK_BUMPER_TYPE type)
{
	switch ( type ){
	case GMD_GMK_BUMPER_TYPE_TRI_I_TOP:
		{
			VecFx32 point_center = *gimmick_pos;
			point_center.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_BOTTOM]);

			VecFx32 point_left = *gimmick_pos;
			point_left.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_LEFT]);
			point_left.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_BOTTOM]*0.5f);

			VecFx32 point_right = *gimmick_pos;
			point_right.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_RIGHT]);
			point_right.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_BOTTOM]*0.5f);
			if ( gmGmkBumperCheckLeft(&point_right, &point_center, target_pos) 
				|| gmGmkBumperCheckLeft(&point_center, &point_left, target_pos) 				
			){
				return FALSE;
			}
		}
		break;
	case GMD_GMK_BUMPER_TYPE_TRI_I_BOTTOM:
		{
			VecFx32 point_center = *gimmick_pos;
			point_center.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_TOP]);

			VecFx32 point_left = *gimmick_pos;
			point_left.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_LEFT]);
			point_left.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_TOP]*0.4f);

			VecFx32 point_right = *gimmick_pos;
			point_right.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_RIGHT]);
			point_right.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_TOP]*0.4f);
			if ( gmGmkBumperCheckLeft(&point_left, &point_center, target_pos) 
				|| gmGmkBumperCheckLeft(&point_center, &point_right, target_pos) 				
			){
				return FALSE;
			}
		}
		break;
	case GMD_GMK_BUMPER_TYPE_TRI_I_LEFT:
		{
			VecFx32 point_center = *gimmick_pos;
			point_center.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_RIGHT]);

			VecFx32 point_top = *gimmick_pos;
			point_top.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_RIGHT]*0.4f);
			point_top.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_TOP]);

			VecFx32 point_bottom = *gimmick_pos;
			point_bottom.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_RIGHT]*0.4f);
			point_bottom.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_BOTTOM]);
			if ( gmGmkBumperCheckLeft(&point_top, &point_center, target_pos) 
				|| gmGmkBumperCheckLeft(&point_center, &point_bottom, target_pos) 				
			){
				return FALSE;
			}
		}
		break;
	case GMD_GMK_BUMPER_TYPE_TRI_I_RIGHT:
		{
			VecFx32 point_center = *gimmick_pos;
			point_center.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_LEFT]);

			VecFx32 point_top = *gimmick_pos;
			point_top.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_LEFT]*0.4f);
			point_top.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_TOP]);

			VecFx32 point_bottom = *gimmick_pos;
			point_bottom.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_LEFT]*0.4f);
			point_bottom.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_BOTTOM]);
			if ( gmGmkBumperCheckLeft(&point_bottom, &point_center, target_pos) 
				|| gmGmkBumperCheckLeft(&point_center, &point_top, target_pos) 				
			){
				return FALSE;
			}
		}
		break;
	case GMD_GMK_BUMPER_TYPE_TRI_R_LT:
		{
			VecFx32 point_top = *gimmick_pos;
			point_top.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_RIGHT]);
			point_top.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_BOTTOM]*0.2f);

			VecFx32 point_bottom = *gimmick_pos;
			point_bottom.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_RIGHT]*0.2f);
			point_bottom.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_BOTTOM]);

			if ( gmGmkBumperCheckLeft(&point_top, &point_bottom, target_pos) ){
				return FALSE;
			}
		}
		break;
	case GMD_GMK_BUMPER_TYPE_TRI_R_LB:
		{
			VecFx32 point_top = *gimmick_pos;
			point_top.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_RIGHT]*0.2f);
			point_top.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_TOP]);

			VecFx32 point_bottom = *gimmick_pos;
			point_bottom.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_RIGHT]);
			point_bottom.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_TOP]*0.2f);

			if ( gmGmkBumperCheckLeft(&point_top, &point_bottom, target_pos) ){
				return FALSE;
			}
		}
		break;
	case GMD_GMK_BUMPER_TYPE_TRI_R_RT:
		{
			VecFx32 point_top = *gimmick_pos;
			point_top.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_LEFT]);
			point_top.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_BOTTOM]*0.2f);

			VecFx32 point_bottom = *gimmick_pos;
			point_bottom.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_LEFT]*0.2f);
			point_bottom.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_BOTTOM]);

			if ( gmGmkBumperCheckLeft(&point_bottom, &point_top, target_pos) ){
				return FALSE;
			}
		}
		break;
	case GMD_GMK_BUMPER_TYPE_TRI_R_RB:
		{
			VecFx32 point_top = *gimmick_pos;
			point_top.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_RIGHT]*0.2f);
			point_top.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_TOP]);

			VecFx32 point_bottom = *gimmick_pos;
			point_bottom.x += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_LEFT]);
			point_bottom.y += FX_F32_TO_FX32(g_gmk_bumper_rect[type][MTD_TOP]*0.2f);

			if ( gmGmkBumperCheckLeft(&point_bottom, &point_top, target_pos) ){
				return FALSE;
			}
		}
		break;
	case GMD_GMK_BUMPER_TYPE_HEX_W:
	case GMD_GMK_BUMPER_TYPE_HEX_H:
	default:
		break;
	}

	return TRUE;
}

// ==========================================================================
// gmGmkBumperCheckLeft
/*!
 *	ÉMÉ~ÉbÉNÅ@ÉoÉìÉpÅ[Å@íºê¸ÇÃç∂îªíËÅi2éüå≥ÇÃÇ›Åj
 *
 *	@param line_start	[in] íºê¸ÇÃäJénà íu
 *	@param line_end		in] íºê¸ÇÃèIóπà íu
 *	@param point		[in] ñ⁄ïWì_
 *
 *	@return TRUEÅFç∂orê¸è„ FALSEÅFâE
 *	
 */
// ==========================================================================
BOOL gmGmkBumperCheckLeft( const VecFx32* line_start, const VecFx32* line_end, const VecFx32* point )
{
	fx32 cross_end_x = line_end->x-line_start->x;
	fx32 cross_end_y = line_end->y-line_start->y;
	fx32 cross_point_x = point->x-line_start->x;
	fx32 cross_point_y = point->y-line_start->y;

	fx32 result = FX_Mul( cross_end_x, cross_point_y ) - FX_Mul( cross_end_y, cross_point_x );
	if ( result <= 0 ){
		return TRUE;
	}
	return FALSE;
}


// ==========================================================================
// gmGmkBumperCheckHitEffect
/*!
 *	ÉMÉ~ÉbÉNÅ@ÉoÉìÉpÅ[Å@HITå¯â ÇçsÇ§Ç©îªíËÅiSEÉnÉìÉhÉãÇégópÅj
 *
 *	@param bumper_work	[in] ÉoÉìÉpÅ[ÉèÅ[ÉN
 *
 *	@return TRUEÅFçƒê∂íÜ FALSEÅFçƒê∂íÜÇ≈ÇÕÇ»Ç¢
 *	
 */
// ==========================================================================
BOOL gmGmkBumperCheckHitEffect( const GMS_GMK_BUMPER_WORK* bumper_work )
{
	if ( !bumper_work->se_handle ){
		return FALSE;
	}
	if ( !bumper_work->se_handle->au_player ){
		return FALSE;
	}

	//çƒê∂íÜ
	if ( CriAuPlayer::STATUS_PLAYING == bumper_work->se_handle->au_player->GetStatus()
			|| CriAuPlayer::STATUS_PREP == bumper_work->se_handle->au_player->GetStatus()
	){
		return FALSE;
	}
	return TRUE;
}

// ==========================================================================
//ÉÜÅ[ÉUÉtÉâÉO
// ==========================================================================


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
