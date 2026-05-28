// ==========================================================================
/*!
  @file gmGmkBobbin.cpp
  @brief ÉMÉ~ÉbÉN É{ÉrÉì

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkBobbin.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 *	GMS_EVE_RECORD_EVENT
 *		left		: Xï˚å¸ílí≤êÆ
 *		top			: Yï˚å¸ílí≤êÆ
 *		width		: Ç»Çµ
 *		height		: Ç»Çµ
 *
 *		flag		: 1	ÉzÅ[É~ÉìÉOâÒïúÇµÇ»Ç¢ÉtÉâÉO
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
#include "gmPlyScoreDef.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmPadVib.h"

#include "gmGmkBobbin.h"

// ÉfÅ[É^ÉwÉbÉ_
#include "common/model/gmk_bobbin_mdl.hmb"
#include "common/model/gmk_bobbin_mtn.hmb"
#include "common/model/gmk_bobbin_mat.hmb"


//----- Definitions ---------------------------------------------------------


//É{ÉrÉìÉÇÅ[Éh
enum GME_GMK_BOBBIN_MODE{
	GMD_GMK_BOBBIN_MODE_WAIT = 0,	//É{ÉrÉìë“Çø
	GMD_GMK_BOBBIN_MODE_HIT,		//É{ÉrÉìÉqÉbÉg

	GMD_GMK_BOBBIN_MODE_NUM
};

#define GMD_GMK_BOBBIN_RECT_MARGIN_PLAYER_X	(0x00008000L)		//ìñÇΩÇËîªíËÉ}Å[ÉWÉì
#define GMD_GMK_BOBBIN_RECT_MARGIN_PLAYER_Y	(0x00010000L)		//ìñÇΩÇËîªíËÉ}Å[ÉWÉì

#define GMD_GMK_BOBBIN_RECT_OFFSET_PLAYER_Y	(-0x00003000L)		//ìñÇΩÇËîªíËÉIÉtÉZÉbÉg

#define GMD_GMK_BOBBIN_OFFSET_RADIUS		( 28 * FX32_ONE )	//ìñÇΩÇËîªíËãóó£
#define GMD_GMK_BOBBIN_SPEED_X				( 6*FX32_ONE )		//ÇÕÇ∂Ç≠ÉXÉsÅ[Éh
#define GMD_GMK_BOBBIN_SPEED_Y				( 6*FX32_ONE )		//ÇÕÇ∂Ç≠ÉXÉsÅ[Éh

#define GMD_GMK_BOBBIN_NO_MOVE_TIME			( 5 )				//ÇÕÇ∂Ç©ÇÍÇΩå„ëÄçÏÇ≈Ç´Ç»Ç¢éûä‘


//ÉtÉâÉO
#define GMD_GMK_BOBBIN_FLAG_NO_RECOVER_HOMING	(1)	//ÉzÅ[É~ÉìÉOâÒïúÇµÇ»Ç¢ÉtÉâÉO

#define GMD_GMK_BOBBIN_SCORE_LIMIT_NUM			(10) //ÉXÉRÉAälìæâÒêî

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static GMS_ENEMY_3D_WORK* gmGmkBobbinLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y );
static GMS_ENEMY_3D_WORK* gmGmkBobbinLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y);

//---------------------------------------------------------
//É{ÉrÉì
//---------------------------------------------------------
static void gmGmkBobbinInit( OBS_OBJECT_WORK *obj_work );
static void gmGmkBobbinSetRect( GMS_ENEMY_3D_WORK* gimmick_work );

static void gmGmkBobbinDrawFunc( OBS_OBJECT_WORK* work );

static VecFx32 gmGmkBobbinNormalizeVectorXY( const VecFx32* vec );
static void gmGmkBobbinDefPlayer( 
						  GMS_ENEMY_3D_WORK* gimmick_work, 
						  GMS_PLAYER_WORK* player_work,
						  fx32 speed_x,
						  fx32 speed_y );
static void gmGmkBobbinDefEnemy( 
						  OBS_OBJECT_WORK* obj_work, 
						  fx32 speed_x,
						  fx32 speed_y );
static void gmGmkBobbinDefFunc( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* player_rect );

static void gmGmkBobbinChangeModeWait( OBS_OBJECT_WORK* obj_work );
static void gmGmkBobbinChangeModeHit( OBS_OBJECT_WORK* obj_work );
static void gmGmkBobbinMainWait( OBS_OBJECT_WORK *obj_work );
static void gmGmkBobbinMainHit( OBS_OBJECT_WORK *obj_work );


//---------------------------------------------------------
//ÉÜÅ[ÉUÉèÅ[ÉN
//---------------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------


static OBS_ACTION3D_NN_WORK* g_gm_gmk_bobbin_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkBobbinBuild
/*!
 *	ÉMÉ~ÉbÉN É{ÉrÉì ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkBobbinBuild(void)
{
	g_gm_gmk_bobbin_obj_3d_list = GmGameDBuildRegBuildModel(
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BOBBIN_MODEL ),
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BOBBIN_TEX ),
			0	//draw_flag
			);
}

// ==========================================================================
// GmGmkBobbinFlush
/*!
 *	ÉMÉ~ÉbÉN É{ÉrÉì ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkBobbinFlush(void)
{
	AMS_AMB_HEADER* amb_header = (AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BOBBIN_MODEL );

	GmGameDBuildRegFlushModel( g_gm_gmk_bobbin_obj_3d_list, amb_header->file_num );
	g_gm_gmk_bobbin_obj_3d_list = NULL;
}

// ==========================================================================
// GmGmkBobbinInit
/*!
 *	ÉMÉ~ÉbÉNèâä˙âªä÷êî É{ÉrÉì
 *
 *	@param eve_rec	[io] ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param pos_x	[in] èoåªç¿ïW
 *	@param pos_y	[in] 
 *	@param type		[in] 
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkBobbinInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	UNREFERENCED_PARAMETER(type);

	//ÉIÉuÉWÉFÉNÉgçÏê¨
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkBobbinLoadObj( eve_rec, pos_x, pos_y );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK*obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//èâä˙âª
	gmGmkBobbinInit( obj_work );
	return obj_work;
}

//----- Local Functions -----------------------------------------------------

// ==========================================================================
// gmGmkBobbinLoadObjNoModel
/*!
 *	ÉMÉ~ÉbÉNì«Ç›çûÇ›ÉÇÉfÉãÇ»Çµ
 *
 *	@param eve_rec	[in] ÉCÉxÉìÉgÉåÉRÅ[Éh
 *	@param pos_x	[in] ç¿ïWX
 *	@param pos_y	[in] ç¿ïWY
 *
 *	@return ÉèÅ[ÉN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkBobbinLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y )
{
	//-------------------------------------------------
	// ÉèÅ[ÉNèâä˙âª
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec, 
			pos_x, 
			pos_y, 
			sizeof(GMS_ENEMY_3D_WORK), 
			"GMK_BOBBIN");
	amAssert( gimmick_work );

	//-------------------------------------------------
	//ãÈå`èâä˙âª
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}

// ==========================================================================
// gmGmkBobbinLoadObj
/*!
 *	ÉMÉ~ÉbÉNì«Ç›çûÇ›
 *
 *	@param eve_rec	[in] ÉCÉxÉìÉgÉåÉRÅ[Éh
 *	@param pos_x	[in] ç¿ïWX
 *	@param pos_y	[in] ç¿ïWY
 *
 *	@return ÉèÅ[ÉN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkBobbinLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y )
{

	//-------------------------------------------------
	// ÉèÅ[ÉNèâä˙âª
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkBobbinLoadObjNoModel(
			eve_rec,
			pos_x,
			pos_y );
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//-------------------------------------------------
	// ÉÇÉfÉãèâä˙âª
	//-------------------------------------------------
	s32 data_model_index = IDB_GMK_BOBBIN_MDL_GMK_BOBBIN_ZNO;
	//ì«Ç›çûÇ›
	ObjObjectCopyAction3dNNModel(
		obj_work,
		&g_gm_gmk_bobbin_obj_3d_list[data_model_index],
		&gimmick_work->obj_3d);

	//-------------------------------------------------
	// ÉÇÅ[ÉVÉáÉìèâä˙âª
	//-------------------------------------------------
	OBS_DATA_WORK* data_motion = ObjDataGet(GMD_DWORK_NO_GMK_BOBBIN_MTN);
	amAssert( data_motion );
	ObjObjectAction3dNNMotionLoad( 
			obj_work,
			0,
			FALSE,
			data_motion,
			NULL,
			0,
			NULL );

	//É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
	OBS_DATA_WORK* data_mat_motion = ObjDataGet(GMD_DWORK_NO_GMK_BOBBIN_MAT);
	amAssert( data_mat_motion );
	ObjObjectAction3dNNMaterialMotionLoad( 
			obj_work,
			0,
			data_mat_motion,
			NULL,
			0,
			NULL
	);

	return gimmick_work;
}

// ==========================================================================
// gmGmkBobbinInit
/*!
 *	ÉMÉ~ÉbÉNÅ@É{ÉrÉìÅ@èâä˙âª
 *
 *	@param obj_work		[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBobbinInit( OBS_OBJECT_WORK *obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//-------------------------------------------------
	//ãÈå`èâä˙âª
	//-------------------------------------------------
	gmGmkBobbinSetRect( gimmick_work );

	//-------------------------------------------------
	// ÉèÅ[ÉNê›íË
	//-------------------------------------------------
	//ÉtÉâÉO
	obj_work->move_flag = OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;

	//ÉÇÅ[ÉVÉáÉì
	obj_work->disp_flag |= OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP;

	//ç¿ïW
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;
	if (GSM_MAIN_STAGE_IS_SPSTAGE()) {
		obj_work->pos.z = (GMD_OBJ_DEFAULT_POS_Z_C + GMD_OBJ_GIMMICK_POS_Z_BACK) /2;
	}

	//-------------------------------------------------
	// ÉÅÉCÉìèàóù
	//-------------------------------------------------
	obj_work->ppFunc = NULL;
	obj_work->ppMove = NULL;
	obj_work->ppOut = gmGmkBobbinDrawFunc;

	//ë“ã@ÉÇÅ[ÉhÇ…
	gmGmkBobbinChangeModeWait( obj_work );
}

// ==========================================================================
// gmGmkBobbinSetRect
/*!
 *	ÉMÉ~ÉbÉNÅ@É{ÉrÉìÅ@ãÈå`ê›íË
 *
 *	@param gimmick_work	[in] ÉMÉ~ÉbÉNÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBobbinSetRect( GMS_ENEMY_3D_WORK* gimmick_work )
{
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)gimmick_work;
	amAssert(obj_work);

	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	
	s16 left = -24;	
	s16 right = 24;
	s16 top = -24;
	s16 bottom = 24;

	ObjRectWorkZSet(
			rect_work,
			left, top, -500,
			right, bottom, 500);
	rect_work->flag |= OBD_RECT_OUT;

	ObjRectGroupSet(
			rect_work, 
			GMD_OBJ_RECT_GROUP_ENEMY, 
			GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER );
	
	//ñhå‰óp
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	rect_work->ppDef = gmGmkBobbinDefFunc;

	if (GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// ÉXÉyÉXÉeéûÇÕínå`ÉRÉäÉWÉáÉìÇ‡ÉZÉbÉg
		OBS_COLLISION_WORK	*col_work = &((GMS_ENEMY_3D_WORK*)obj_work)->ene_com.col_work;
	
		col_work->obj_col.obj		= obj_work;
		col_work->obj_col.diff_data	= (s8*)g_gm_default_col;			// è„â∫ç∂âEê⁄êGñ äpìxèÓïÒÇìæÇÈÇΩÇﬂÇ…ÇÕ
		col_work->obj_col.width		= 2*8;								// ínå`ÉTÉCÉYê›íË(8dotíPà êßå¿Ç†ÇË)
		col_work->obj_col.height	= 2*8;								// êßñÒèúäOÇ∑ÇÈÇΩÇﬂÇ…ÇÕdiff_dataÇêÍópÇ≈éùÇ¬ïKóvÇ™Ç†ÇÈ
		col_work->obj_col.ofst_x	= (s16)(0 - col_work->obj_col.width / 2);
		col_work->obj_col.ofst_y	= (s16)(0 - col_work->obj_col.height / 2);
		col_work->obj_col.attr		= OBD_COL_DATA_ATTR_CLIFF;			// äRàµÇ¢(ínå`äpìxÇéÊìæÇµÇ»Ç¢)
		col_work->obj_col.flag		|= OBD_COLOBJ_NOFREE_DIFF_DATA		// diff_dataÇäJï˙ÇµÇ»Ç¢
									 | OBD_COLOBJ_NODIR_PARENT;			// êeÇÃäpìxñ≥éã
	}
}

// ==========================================================================
// gmGmkBobbinDrawFunc
/*!
 *	ÉMÉ~ÉbÉNÅ@É{ÉrÉìÅ@ï`âÊä÷êî
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBobbinDrawFunc( OBS_OBJECT_WORK* obj_work )
{
	ObjDrawActionSummary( obj_work );
}

// ==========================================================================
// gmGmkBobbinNormalizeVectorXY
/*!
 *	ÉMÉ~ÉbÉNÅ@É{ÉrÉìÅ@ÉxÉNÉ^Çê≥ãKâª(XYç¿ïWÇÃÇ›ÅBZÇÕ0Ç…èâä˙âª)
 *
 *	@param vec	[in] É^Å[ÉQÉbÉg
 *
 *	@return ê≥ãKâªå„ÇÃÉ^Å[ÉQÉbÉg
 */
// ==========================================================================
VecFx32 gmGmkBobbinNormalizeVectorXY( const VecFx32* vec )
{
	amAssert( vec );

	VecFx32 normal;

	fx32 length = FX_Mul(vec->x, vec->x) + FX_Mul(vec->y, vec->y);
	if ( length == 0 ){
		normal.x = FX32_ONE;
		normal.y = 0;
	}
	else{
		length = FX_Sqrt(length);
		fx32 r_length = FX_Div( FX32_ONE, length );
		normal.x = FX_Mul( vec->x, r_length );
		normal.y = FX_Mul( vec->y, r_length );
	}
	normal.z = 0;

	if (GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// ÉXÉyÉXÉeéûÇÕâÊñ âÒì]Ç…çáÇÌÇπÇƒà⁄ìÆï˚å¸ï‚ê≥
		fx32 spd_x, spd_y;
		ObjUtilGetRotPosXY(normal.x, normal.y, &spd_x, &spd_y, (u16)(-g_gm_main_system.pseudofall_dir));
		normal.x = spd_x;
		normal.y = spd_y;
	}
	return normal;
}

// ==========================================================================
// gmGmkBobbinDefFunc
/*!
 *	ÉMÉ~ÉbÉNÅ@É{ÉrÉìÅ@äJénèÛë‘ãÈå`ä÷êî
 *
 *	@param gimmick_work	[io] ÉMÉ~ÉbÉNÉèÅ[ÉN
 *	@param player_work	[io] ÉvÉåÉCÉÑÉèÅ[ÉN
 *	@param speed_x	[in] à⁄ìÆë¨ìx
 *	@param speed_y	[in] à⁄ìÆë¨ìx
 */
// ==========================================================================
void gmGmkBobbinDefPlayer( 
						  GMS_ENEMY_3D_WORK* gimmick_work, 
						  GMS_PLAYER_WORK* player_work,
						  fx32 speed_x,
						  fx32 speed_y )
{	
	//ÉzÅ[É~ÉìÉOâÒïúÇµÇ»Ç¢ÉtÉâÉO
	BOOL flag_no_recover_homing = FALSE;
	if ( gimmick_work->ene_com.eve_rec->flag & GMD_GMK_BOBBIN_FLAG_NO_RECOVER_HOMING ){
		flag_no_recover_homing = TRUE;
	}

	//ÉvÉåÉCÉÑÉVÅ[ÉPÉìÉX
	GmPlySeqInitPinballAir(
			player_work,
			speed_x, 
			speed_y, 
			GMD_GMK_BOBBIN_NO_MOVE_TIME,
			flag_no_recover_homing );


	//ÉXÉRÉAâ¡éZ
	if (!GMM_MAIN_STAGE_IS_SS()) {
		//if ( gimmick_work->ene_com.eve_rec->byte_param[1] < GMD_GMK_BOBBIN_SCORE_LIMIT_NUM )
		{
			//gimmick_work->ene_com.eve_rec->byte_param[1] += 1;
			const OBS_OBJECT_WORK* obj_work = (const OBS_OBJECT_WORK*)gimmick_work; 
			GmPlayerAddScore(
					player_work,
					GMD_PLY_SCORE_BOBBIN,
					obj_work->pos.x, 
					obj_work->pos.y );
		}
	}
}
void gmGmkBobbinDefEnemy( 
						  OBS_OBJECT_WORK* obj_work, 
						  fx32 speed_x,
						  fx32 speed_y )
{	
	//É{ÉX2à»äO
	GMS_ENEMY_3D_WORK* enemy_work = (GMS_ENEMY_3D_WORK*)obj_work;
	if ( enemy_work->ene_com.eve_rec->id != GMD_EVENT_ID_BOSS2_BODY ){
		return;
	}
	obj_work->spd.x = speed_x;
	obj_work->spd.y = speed_y;
	obj_work->spd_add.x = 0;
	obj_work->spd_add.y = 0;

	//ã≤Ç‹ÇÁÇ»Ç¢ÇÊÇ§Ç…ï‚ê≥Ç∑ÇÈ
	if (MTM_MATH_ABS(obj_work->spd.x) < 0x00000100L){
		obj_work->spd.x = 0x00000100L;
	}
	else if (MTM_MATH_ABS(obj_work->spd.y) < 0x00000100L){
		obj_work->spd.y = 0x00000100L;
	}
}

void gmGmkBobbinDefFunc( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* player_rect )
{
	amAssert( gimmick_rect );
	amAssert( player_rect );

	//ÉMÉ~ÉbÉNÉèÅ[ÉN
	OBS_OBJECT_WORK* gimmick_obj_work = gimmick_rect->parent_obj;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)gimmick_obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* target_obj_work = player_rect->parent_obj;
	amAssert( target_obj_work );

	VecFx32 offset;
	offset.x = target_obj_work->prev_pos.x - gimmick_obj_work->pos.x;
	offset.y = target_obj_work->prev_pos.y + GMD_GMK_BOBBIN_RECT_OFFSET_PLAYER_Y - gimmick_obj_work->pos.y;
	offset.z = 0;

	//îÕàÕîªíË
	fx32 length = FX_Mul(offset.x, offset.x) + FX_Mul(offset.y, offset.y);
	if ( FX_Mul(GMD_GMK_BOBBIN_OFFSET_RADIUS, GMD_GMK_BOBBIN_OFFSET_RADIUS) < length ){
		gimmick_rect->flag &= ~OBD_RECT_OUT;
		return;
	}
	gimmick_rect->flag |= OBD_RECT_OUT;

	offset = gmGmkBobbinNormalizeVectorXY( &offset );

	//åXÇ´
	target_obj_work->dir.z = 0;

	//ÉXÉsÅ[Éhê›íË
	fx32 speed_x = FX_Mul( offset.x, GMD_GMK_BOBBIN_SPEED_X );
	fx32 speed_y = FX_Mul( offset.y, GMD_GMK_BOBBIN_SPEED_Y );

	//îzíuÉcÅ[Éãê›íËÇ…ÇÊÇÈï‚ê≥
	fx32 adjust_x = FX_F32_TO_FX32( (100.0f + (Float)(gimmick_work->ene_com.eve_rec->left))*0.01f );
	if ( adjust_x < 0 ){
		adjust_x = 0;
	}
	fx32 adjust_y = FX_F32_TO_FX32( (100.0f + (Float)(gimmick_work->ene_com.eve_rec->top))*0.01f );
	if ( adjust_y < 0 ){
		adjust_y = 0;
	}

	speed_x = FX_Mul( speed_x, adjust_x );
	speed_y = FX_Mul( speed_y, adjust_y );

	//ÉvÉåÉCÉÑ
	if (target_obj_work->obj_type == GMD_OBJTYPE_PLAYER) {
		gmGmkBobbinDefPlayer(
				gimmick_work,
				(GMS_PLAYER_WORK*)target_obj_work,
				speed_x,
				speed_y );
	}
	//ìG
	else if ( target_obj_work->obj_type == GMD_OBJTYPE_ENEMY ){
		gmGmkBobbinDefEnemy(
				target_obj_work,
				speed_x,
				speed_y);
	}

	//ÉÇÅ[ÉhïœçX
	gmGmkBobbinChangeModeHit( gimmick_obj_work );
	
	//å¯â âπ
	GmSoundPlaySE("Casino1");

	// ÉGÉtÉFÉNÉgèàóù
	GMS_EFFECT_3DES_WORK* effect_work = GmEfctCmnEsCreate(
			gimmick_obj_work,
			GME_EFCT_CMN_IDX_BUMPER );
	amAssert( effect_work );
	effect_work->efct_com.obj_work.pos.x = target_obj_work->pos.x;
	effect_work->efct_com.obj_work.pos.y = target_obj_work->pos.y;
	effect_work->efct_com.obj_work.pos.z = GMD_OBJ_GIMMICK_POS_Z_FRONT;
	effect_work->efct_com.obj_work.dir.z = (u16)(nnArcTan2( FX_FX32_TO_F32(speed_y), FX_FX32_TO_F32(speed_x) ) - 0x4000);

	if (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {
		// ÉXÉyÉXÉeíÜÇÕâÊñ äpìxÇ‡çló∂Ç∑ÇÈ
		OBS_CAMERA	*obj_camera = ObjCameraGet(g_obj.glb_camera_id);
		if (obj_camera) {
			effect_work->efct_com.obj_work.dir.z -= (u16)obj_camera->roll;
		}
	}
	//êUìÆ
	GMM_PAD_VIB_SMALL();
}

// ==========================================================================
// gmGmkBobbinChangeModeWait
/*!
 *	ë“ã@ÉÇÅ[ÉhÇ…à»ç~
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBobbinChangeModeWait( OBS_OBJECT_WORK* obj_work )
{
	//ÉÇÅ[ÉVÉáÉìïœçX
	ObjDrawObjectActionSet3DNN( obj_work, IDB_GMK_BOBBIN_MTN_GMK_BOBBIN_ZNM, 0 );

	//ÉÅÉCÉìèàóùïœçX
	obj_work->ppFunc = gmGmkBobbinMainWait;
}

// ==========================================================================
// gmGmkBobbinChangeModeHit
/*!
 *	ÉqÉbÉgÉÇÅ[ÉhÇ…à»ç~
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBobbinChangeModeHit( OBS_OBJECT_WORK* obj_work )
{
	//É}ÉeÉäÉAÉãïœçX
	ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_BOBBIN_MAT_GMK_BOBBIN_HIT_ZNV );

	//ÉÇÅ[ÉVÉáÉìïœçX
	ObjDrawObjectActionSet3DNN( obj_work, IDB_GMK_BOBBIN_MTN_GMK_BOBBIN_HIT_ZNM, 0 );

	//ÉÅÉCÉìèàóùïœçX
	obj_work->ppFunc = gmGmkBobbinMainHit;
}

// ==========================================================================
// gmGmkBobbinMainWait
/*!
 *	ë“ã@éûÉÅÉCÉìèàóù
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBobbinMainWait( OBS_OBJECT_WORK *obj_work )
{
	UNREFERENCED_PARAMETER(obj_work);
}

// ==========================================================================
// gmGmkBobbinMainHit
/*!
 *	ÉqÉbÉgéûÉÅÉCÉìèàóù
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBobbinMainHit( OBS_OBJECT_WORK *obj_work )
{
	//ÉÇÅ[ÉVÉáÉìèIÇÌÇ¡ÇΩÇ∆Ç´
	if ( obj_work->disp_flag & OBD_DISP_END ){
		gmGmkBobbinChangeModeWait( obj_work );

	}
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
