// ==========================================================================
/*!
  @file gmGmkEnBmpr.cpp
  @brief ƒMƒ~ƒbƒN ‚R‘Ïƒoƒ“ƒp[

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkEnBmpr.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 *	GMS_EVE_RECORD_EVENT
 *		left		: ‰Šú‘Ï‹v’l(0‚Ìê‡‚ÍMAX)
 *		top			: ‚È‚µ
 *		width		: ‚È‚µ
 *		height		: ‚È‚µ
 *
 *		flag		: ‚È‚µ
 *
 * ‰Šú‘Ï‹v’l‚Ì—LŒø’l‚Í1`2
 * ‚»‚êˆÈŠO‚Ì’l‚ğİ’è‚µ‚½ê‡AMAX‚ªİ’è‚³‚ê‚é
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

#include "gmGmkEnBmpr.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "common/model/gmk_bumper_mdl.hmb"
#include "common/model/gmk_bumper_mtn.hmb"
#include "common/model/gmk_bumper_mat.hmb"


//----- Definitions ---------------------------------------------------------


//‚R‘Ïƒoƒ“ƒp[ƒ‚[ƒh
enum GME_GMK_EN_BMPR_MODE{
	GMD_GMK_EN_BMPR_MODE_WAIT = 0,	//‚R‘Ïƒoƒ“ƒp[‘Ò‚¿
	GMD_GMK_EN_BMPR_MODE_HIT,		//‚R‘Ïƒoƒ“ƒp[ƒqƒbƒg
	GMD_GMK_EN_BMPR_MODE_LOST,		//‚R‘Ïƒoƒ“ƒp[Á–Å

	GMD_GMK_EN_BMPR_MODE_NUM
};

//‚R‘Ïƒoƒ“ƒp[ƒ^ƒCƒv
enum GME_GMK_EN_BMPR_TYPE{
	GMD_GMK_EN_BMPR_TYPE_0 = 0,	//0“x
	GMD_GMK_EN_BMPR_TYPE_45,	//45“x
	GMD_GMK_EN_BMPR_TYPE_90,	//90“x
	GMD_GMK_EN_BMPR_TYPE_135,	//135“x

	GMD_GMK_EN_BMPR_TYPE_NUM
};


#define GMD_GMK_EN_BMPR_RECT_MARGIN_PLAYER_X	(0x00008000L)		//“–‚½‚è”»’èƒ}[ƒWƒ“
#define GMD_GMK_EN_BMPR_RECT_MARGIN_PLAYER_Y	(0x00010000L)		//“–‚½‚è”»’èƒ}[ƒWƒ“

#define GMD_GMK_EN_BMPR_RECT_OFFSET_PLAYER_Y	(-0x00003000L)		//“–‚½‚è”»’èƒIƒtƒZƒbƒg

#define GMD_GMK_EN_BMPR_POS_Z				(-30*FX32_ONE)		//•\¦ˆÊ’u
#define GMD_GMK_EN_BMPR_OFFSET_RADIUS		( 24 * FX32_ONE )	//“–‚½‚è”»’è‹——£
#define GMD_GMK_EN_BMPR_SPEED			( 6*FX32_ONE )		//‚Í‚¶‚­ƒXƒs[ƒh

#define GMD_GMK_EN_BMPR_LIFE_MAX			(3)	//ƒoƒ“ƒp[‘Ì—Í

#define GMD_GMK_EN_BMPR_NO_MOVE_TIME		( 5 )				//‚Í‚¶‚©‚ê‚½Œã‘€ì‚Å‚«‚È‚¢ŠÔ

#define GMD_GMK_ENBMPR_SCORE_BONUS			(50)				//ƒXƒRƒA‚ÌƒOƒ‹[ƒv‘SÁ‚µ•â³


#define GMD_GMK_ENBMPR_FLAG_ALIVE			(1<<0)			//¶‘¶ƒtƒ‰ƒO

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static u32 gmGmkEnBmpreGameSystemGetSyncTime( void );
static GMS_ENEMY_3D_WORK* gmGmkEnBmprLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_EN_BMPR_TYPE type );
static GMS_ENEMY_3D_WORK* gmGmkEnBmprLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_EN_BMPR_TYPE type );

//---------------------------------------------------------
//‚R‘Ïƒoƒ“ƒp[
//---------------------------------------------------------
static void gmGmkEnBmprInit( OBS_OBJECT_WORK *obj_work, GME_GMK_EN_BMPR_TYPE en_bmpr_type, s32 life_max );
static void gmGmkEnBmprSetRect( GMS_ENEMY_3D_WORK* gimmick_work, GME_GMK_EN_BMPR_TYPE en_bmpr_type );

static void gmGmkEnBmprDrawFunc( OBS_OBJECT_WORK* work );

static GME_GMK_EN_BMPR_TYPE gmGmkEnBmprCalcType( GME_EVENT_ID id );
static VecFx32 gmGmkEnBmprNormalizeVectorXY( const VecFx32* vec );
static void gmGmkEnBmprDefFunc( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* player_rect );

static BOOL gmGmkEnBmprCheckGroupBonus( OBS_OBJECT_WORK *obj_work );

static void gmGmkEnBmprChangeModeWait( OBS_OBJECT_WORK* obj_work );
static void gmGmkEnBmprChangeModeHit( OBS_OBJECT_WORK* obj_work );
static void gmGmkEnBmprChangeModeLost( OBS_OBJECT_WORK* obj_work );
static void gmGmkEnBmprMainWait( OBS_OBJECT_WORK *obj_work );
static void gmGmkEnBmprMainHit( OBS_OBJECT_WORK *obj_work );
static void gmGmkEnBmprMainLost( OBS_OBJECT_WORK *obj_work );

//---------------------------------------------------------
//ƒ†[ƒUƒ[ƒN
//---------------------------------------------------------
static void gmGmkEnBmperSetUserWorkLife( OBS_OBJECT_WORK* obj_work, s32 life );
static s32 gmGmkEnBmperGetUserWorkLife( OBS_OBJECT_WORK* obj_work );
static s32 gmGmkEnBmperAddUserWorkLife( OBS_OBJECT_WORK* obj_work, s32 add );

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//c‚èƒ‰ƒCƒt•Êƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“ID
static const s32 g_gm_gmk_en_bmpr_mat_motion_id[GMD_GMK_EN_BMPR_LIFE_MAX+1] = {
	IDB_GMK_BUMPER_MAT_GMK_EN_BMPR_R_ZNV,	//c‚èƒ‰ƒCƒt0‚Ì‚Æ‚«
	IDB_GMK_BUMPER_MAT_GMK_EN_BMPR_R_ZNV,	//c‚èƒ‰ƒCƒt1‚Ì‚Æ‚«
	IDB_GMK_BUMPER_MAT_GMK_EN_BMPR_Y_ZNV,	//c‚èƒ‰ƒCƒt2‚Ì‚Æ‚«
	IDB_GMK_BUMPER_MAT_GMK_EN_BMPR_ZNV,		//c‚èƒ‰ƒCƒt3‚Ì‚Æ‚«
};

//ZŠp“x
static const u16 g_gm_gmk_en_bmpr_angle_z[GMD_GMK_EN_BMPR_TYPE_NUM] = {
	0,
	0xe000,
	0xc000,
	0xa000,
};

//‹éŒ`
static const s16 g_gmk_en_bmpr_rect[GMD_GMK_EN_BMPR_TYPE_NUM][MTD_RECT] = {
	{ -8, -4, 8, 4},
	{ -6, -6, 6, 6},
	{ -4, -8, 4, 8},
	{ -6, -6, 6, 6},
};

static OBS_ACTION3D_NN_WORK* g_gm_gmk_en_bmpr_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkEnBmprBuild
/*!
 *	ƒMƒ~ƒbƒN ‚R‘Ïƒoƒ“ƒp[ ƒf[ƒ^\’z
 */
// ==========================================================================
void GmGmkEnBmprBuild(void)
{
	g_gm_gmk_en_bmpr_obj_3d_list = GmGameDBuildRegBuildModel(
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BUMPER_MODEL ),
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BUMPER_TEX ),
			0	//draw_flag
			);
}

// ==========================================================================
// GmGmkEnBmprFlush
/*!
 *	ƒMƒ~ƒbƒN ‚R‘Ïƒoƒ“ƒp[ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
void GmGmkEnBmprFlush(void)
{
	AMS_AMB_HEADER* amb_header = (AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BUMPER_MODEL );

	GmGameDBuildRegFlushModel( g_gm_gmk_en_bmpr_obj_3d_list, amb_header->file_num );
	g_gm_gmk_en_bmpr_obj_3d_list = NULL;
}

// ==========================================================================
// GmGmkEnBmprInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ” ‚R‘Ïƒoƒ“ƒp[
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] 
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkEnBmprInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	//‘Ì—ÍÅ‘å’lİ’è
	s32 life_max = GMD_GMK_EN_BMPR_LIFE_MAX;
	if ( eve_rec->left > 0 && eve_rec->left < GMD_GMK_EN_BMPR_LIFE_MAX ){
		life_max = eve_rec->left;
	}

	//”j‰óÏ‚İƒ`ƒFƒbƒN
	if ( life_max <= eve_rec->byte_param[1] ){
		return NULL;
	}

	UNREFERENCED_PARAMETER(type);
	//ƒ^ƒCƒv
	GME_GMK_EN_BMPR_TYPE en_bmpr_type = gmGmkEnBmprCalcType( (GME_EVENT_ID)eve_rec->id );

	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkEnBmprLoadObj( eve_rec, pos_x, pos_y, en_bmpr_type );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK*obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkEnBmprInit( obj_work, en_bmpr_type, life_max );

	return obj_work;
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkEnBmpreGameSystemGetSyncTime
/*!
 * ƒQ[ƒ€ƒVƒXƒeƒ€‚©‚ç“¯ŠúŠÔ‚ğæ“¾
 *
 *	@return	“¯ŠúŠÔ
 */
// ==========================================================================
u32 gmGmkEnBmpreGameSystemGetSyncTime( void )
{
	return g_gm_main_system.sync_time;
}

// ==========================================================================
// gmGmkEnBmprLoadObjNoModel
/*!
 *	ƒMƒ~ƒbƒN“Ç‚İ‚İƒ‚ƒfƒ‹‚È‚µ
 *
 *	@param eve_rec	[in] ƒCƒxƒ“ƒgƒŒƒR[ƒh
 *	@param pos_x	[in] À•WX
 *	@param pos_y	[in] À•WY
 *	@param type		[in] ƒ^ƒCƒv
 *
 *	@return ƒ[ƒN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkEnBmprLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_EN_BMPR_TYPE type )
{
	UNREFERENCED_PARAMETER(type);

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec, 
			pos_x, 
			pos_y, 
			sizeof(GMS_ENEMY_3D_WORK), 
			"GMK_EN_BMPR");
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}

// ==========================================================================
// gmGmkEnBmprLoadObj
/*!
 *	ƒMƒ~ƒbƒN“Ç‚İ‚İ
 *
 *	@param eve_rec	[in] ƒCƒxƒ“ƒgƒŒƒR[ƒh
 *	@param pos_x	[in] À•WX
 *	@param pos_y	[in] À•WY
 *	@param type		[in] ƒ^ƒCƒv
 *
 *	@return ƒ[ƒN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkEnBmprLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_EN_BMPR_TYPE type )
{

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkEnBmprLoadObjNoModel(
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
	s32 data_model_index = IDB_GMK_BUMPER_MDL_GMK_EN_BMPR_ZNO;
	//s32 data_model_index = IDB_GMK_BUMPER_MDL_GMK_BUMPER_ZNO;
	//“Ç‚İ‚İ
	ObjObjectCopyAction3dNNModel(
		obj_work,
		&g_gm_gmk_en_bmpr_obj_3d_list[data_model_index],
		&gimmick_work->obj_3d);

	//-------------------------------------------------
	// ƒ‚[ƒVƒ‡ƒ“‰Šú‰»
	//-------------------------------------------------
	OBS_DATA_WORK* data_motion = ObjDataGet(GMD_DWORK_NO_GMK_BUMPER_MTN);
	amAssert( data_motion );
	ObjObjectAction3dNNMotionLoad( 
			obj_work,
			0,
			FALSE,
			data_motion,
			NULL,
			0,
			NULL );

	//ƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“
	OBS_DATA_WORK* data_mat_motion = ObjDataGet(GMD_DWORK_NO_GMK_BUMPER_MAT);
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
// gmGmkEnBmprInit
/*!
 *	ƒMƒ~ƒbƒN@‚R‘Ïƒoƒ“ƒp[@‰Šú‰»
 *
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param en_bmpr_type	[in] ƒ^ƒCƒv
 *	@param life_max		[in] ‘Ì—ÍÅ‘å’l
 */
// ==========================================================================
void gmGmkEnBmprInit( OBS_OBJECT_WORK *obj_work, GME_GMK_EN_BMPR_TYPE en_bmpr_type, s32 life_max )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	//ƒvƒŒƒCƒ„[‚ ‚½‚è
	gmGmkEnBmprSetRect( gimmick_work, en_bmpr_type );

	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->move_flag = OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;

	//¶‘¶ƒtƒ‰ƒO
	obj_work->user_flag |= GMD_GMK_ENBMPR_FLAG_ALIVE;

	//Œü‚«•Êİ’è
	obj_work->dir.z = g_gm_gmk_en_bmpr_angle_z[en_bmpr_type];

	//ƒ‰ƒCƒt
	s32 life = life_max - gimmick_work->ene_com.eve_rec->byte_param[1];
	gmGmkEnBmperSetUserWorkLife( obj_work, life );

	//ƒ‚[ƒVƒ‡ƒ“
	ObjDrawObjectActionSet3DNNMaterial( obj_work, g_gm_gmk_en_bmpr_mat_motion_id[life] );
	obj_work->disp_flag |= OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP;

	//À•W
	obj_work->pos.z = GMD_GMK_EN_BMPR_POS_Z;

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = NULL;
	obj_work->ppMove = NULL;
	obj_work->ppOut = gmGmkEnBmprDrawFunc;

	//‘Ò‹@ƒ‚[ƒh‚É
	gmGmkEnBmprChangeModeWait( obj_work );
}

// ==========================================================================
// gmGmkEnBmprSetRect
/*!
 *	ƒMƒ~ƒbƒN@‚R‘Ïƒoƒ“ƒp[@‹éŒ`İ’è
 *
 *	@param gimmick_work	[in] ƒMƒ~ƒbƒNƒ[ƒN
 *	@param en_bmpr_type	[in] ƒ^ƒCƒv
 */
// ==========================================================================
void gmGmkEnBmprSetRect( GMS_ENEMY_3D_WORK* gimmick_work, GME_GMK_EN_BMPR_TYPE en_bmpr_type )
{
	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	
	s16 left = g_gmk_en_bmpr_rect[en_bmpr_type][MTD_LEFT];	
	s16 right = g_gmk_en_bmpr_rect[en_bmpr_type][MTD_RIGHT];
	s16 top = g_gmk_en_bmpr_rect[en_bmpr_type][MTD_TOP];
	s16 bottom = g_gmk_en_bmpr_rect[en_bmpr_type][MTD_BOTTOM];

	ObjRectWorkZSet(
			rect_work,
			left, top, -500,
			right, bottom, 500);
	rect_work->flag |= OBD_RECT_OUT;
	
	//–hŒä—p
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	rect_work->ppDef = gmGmkEnBmprDefFunc;
}

// ==========================================================================
// gmGmkEnBmprDrawFunc
/*!
 *	ƒMƒ~ƒbƒN@‚R‘Ïƒoƒ“ƒp[@•`‰æŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkEnBmprDrawFunc( OBS_OBJECT_WORK* obj_work )
{
	OBS_ACTION3D_NN_WORK *obj_3d = obj_work->obj_3d;
	amAssert( obj_3d );
	if ( obj_3d->motion ){
		//ƒ‚[ƒVƒ‡ƒ“‚ğ“¯Šú‚³‚¹‚é
		float frame_start = amMotionMaterialGetStartFrame( obj_3d->motion, obj_3d->mat_act_id );
		float frame_end = amMotionMaterialGetEndFrame( obj_3d->motion, obj_3d->mat_act_id );
		float frame_max = frame_end - frame_start;

		float frame = (float)gmGmkEnBmpreGameSystemGetSyncTime();

		obj_3d->mat_frame = fmod( frame, frame_max );
	}

	ObjDrawActionSummary( obj_work );
}

// ==========================================================================
// gmGmkEnBmprCalcType
/*!
 *	ƒMƒ~ƒbƒN@‚R‘Ïƒoƒ“ƒp[@ƒ^ƒCƒvæ“¾
 *
 *	@param id	[in] ƒCƒxƒ“ƒgID
 */
// ==========================================================================
GME_GMK_EN_BMPR_TYPE gmGmkEnBmprCalcType( GME_EVENT_ID id )
{
	GME_GMK_EN_BMPR_TYPE en_bmpr_type = (GME_GMK_EN_BMPR_TYPE)(id - GMD_EVENT_ID_EN_BMPR_0);
	amAssert( GMD_GMK_EN_BMPR_TYPE_0 <= en_bmpr_type && en_bmpr_type < GMD_GMK_EN_BMPR_TYPE_NUM );

	return en_bmpr_type;
}

// ==========================================================================
// gmGmkEnBmprNormalizeVectorXY
/*!
 *	ƒMƒ~ƒbƒN@‚R‘Ïƒoƒ“ƒp[@ƒxƒNƒ^‚ğ³‹K‰»(XYÀ•W‚Ì‚İBZ‚Í0‚É‰Šú‰»)
 *
 *	@param vec	[in] ƒ^[ƒQƒbƒg
 *
 *	@return ³‹K‰»Œã‚Ìƒ^[ƒQƒbƒg
 */
// ==========================================================================
VecFx32 gmGmkEnBmprNormalizeVectorXY( const VecFx32* vec )
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
	
	return normal;
}

// ==========================================================================
// gmGmkEnBmprDefFunc
/*!
 *	ƒMƒ~ƒbƒN@‚R‘Ïƒoƒ“ƒp[@ŠJnó‘Ô‹éŒ`ŠÖ”
 *
 *	@param gimmick_rect	[in] ©g‚Ì‹éŒ`ƒ[ƒN
 *	@param target_rect	[in] ‘Šè‚Ì‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkEnBmprDefFunc( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* player_rect )
{
	amAssert( gimmick_rect );
	amAssert( player_rect );

	//ƒMƒ~ƒbƒNƒ[ƒN
	OBS_OBJECT_WORK* gimmick_obj_work = gimmick_rect->parent_obj;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)gimmick_obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* target_obj_work = player_rect->parent_obj;
	amAssert( target_obj_work );
	
	//ƒvƒŒƒCƒ„ˆÈŠO‚Í”»’è‚µ‚È‚¢
	if ( target_obj_work->obj_type != GMD_OBJTYPE_PLAYER ){
		return;
	}

	//Œü‚«Äİ’èiƒqƒbƒg‚µ‚½Œü‚«‚É‚æ‚Á‚Ä‚ÍA‹tŒü‚«‚É‚È‚é‚½‚ßj
	GME_GMK_EN_BMPR_TYPE en_bmpr_type = gmGmkEnBmprCalcType( (GME_EVENT_ID)gimmick_work->ene_com.eve_rec->id );
	gimmick_obj_work->dir.z = g_gm_gmk_en_bmpr_angle_z[en_bmpr_type];

	//ŒX‚«
	target_obj_work->dir.z = 0;

	//ƒXƒs[ƒhİ’è
	fx32 speed_x = target_obj_work->spd.x;
	fx32 speed_y = target_obj_work->spd.y;	
    if( target_obj_work->move_flag & OBD_MOVE_NOSPDM ){
		if ( target_obj_work->spd_m != 0 ){
			speed_x = FX_Mul((target_obj_work->spd_m ), mtMathCos( target_obj_work->dir.z ));
			speed_y = FX_Mul((target_obj_work->spd_m ), mtMathSin( target_obj_work->dir.z ));
		}
		else{
			VecFx32 offset;
			offset.x = target_obj_work->pos.x - gimmick_obj_work->pos.x;
			offset.y = target_obj_work->pos.y - gimmick_obj_work->pos.y;
			offset.z = 0;
			offset = gmGmkEnBmprNormalizeVectorXY( &offset );

			speed_x = FX_Mul( offset.x, GMD_GMK_EN_BMPR_OFFSET_RADIUS );
			speed_y = FX_Mul( offset.y, GMD_GMK_EN_BMPR_OFFSET_RADIUS );
		}
	}

	//‚R‘Ïƒoƒ“ƒp[ƒ^ƒCƒv•ÊŒXÎŠp“x‚ğZo
	fx32 offset_y = GMD_GMK_EN_BMPR_RECT_OFFSET_PLAYER_Y;

	fx32 distance_x = (target_obj_work->pos.x) - gimmick_obj_work->pos.x;
	fx32 distance_y = (target_obj_work->pos.y+offset_y) - gimmick_obj_work->pos.y;
	
	switch ( en_bmpr_type ){
	case GMD_GMK_EN_BMPR_TYPE_0:
		{
			speed_x = 0;
			if ( distance_y < 0 ){
				speed_y = -GMD_GMK_EN_BMPR_SPEED;
			}
			else{
				speed_y = GMD_GMK_EN_BMPR_SPEED;
				gimmick_obj_work->dir.z += 0x8000;
			}
		}
		break;
	case GMD_GMK_EN_BMPR_TYPE_45:
		{
			fx32 ref_speed = FX_Mul(GMD_GMK_EN_BMPR_SPEED, FX32_SQRT1_2);
			if ( distance_y < 0 ){
				speed_x = -ref_speed;
				speed_y = -ref_speed;
			}
			else{
				speed_x = ref_speed;
				speed_y = ref_speed;
				gimmick_obj_work->dir.z += 0x8000;
			}
		}
		break;
	case GMD_GMK_EN_BMPR_TYPE_90:
		{
			speed_y = 0;
			if ( distance_x < 0 ){
				speed_x = -GMD_GMK_EN_BMPR_SPEED;
			}
			else{
				speed_x = GMD_GMK_EN_BMPR_SPEED;
				gimmick_obj_work->dir.z += 0x8000;
			}
		}
		break;
	case GMD_GMK_EN_BMPR_TYPE_135:
		{
			fx32 ref_speed = FX_Mul(GMD_GMK_EN_BMPR_SPEED, FX32_SQRT1_2);
			if ( distance_y > 0 ){
				speed_x = -ref_speed;
				speed_y = ref_speed;
			}
			else{
				speed_x = ref_speed;
				speed_y = -ref_speed;
				gimmick_obj_work->dir.z += 0x8000;
			}
		}
		break;
	default:
		amAssert( FALSE );
		break;
	}
	
	GmPlySeqInitPinballAir(
			(GMS_PLAYER_WORK*)target_obj_work, 
			speed_x,  
			speed_y,
			GMD_GMK_EN_BMPR_NO_MOVE_TIME );

	//ƒ‚[ƒh•ÏX
	gmGmkEnBmprChangeModeHit( gimmick_obj_work );

	

	//¶‘¶ƒtƒ‰ƒO
	s32 life = gmGmkEnBmperGetUserWorkLife( gimmick_obj_work );
	if ( life <= 0 ){
		gimmick_obj_work->user_flag &= ~GMD_GMK_ENBMPR_FLAG_ALIVE;
	}

	//ƒXƒRƒA‰ÁZ
	s32 score = GMD_PLY_SCORE_EN_BUMPER;
	if ( gmGmkEnBmprCheckGroupBonus(gimmick_obj_work) ){
		score *= GMD_GMK_ENBMPR_SCORE_BONUS;
	}
	GmPlayerAddScore(
			(GMS_PLAYER_WORK*)target_obj_work,
			score,
			gimmick_obj_work->pos.x, 
			gimmick_obj_work->pos.y );

	// ƒGƒtƒFƒNƒgˆ—
	GMS_EFFECT_3DES_WORK* effect_work = GmEfctCmnEsCreate(
			gimmick_obj_work,
			GME_EFCT_CMN_IDX_BUMPER );
	amAssert( effect_work );
	effect_work->efct_com.obj_work.pos.x = target_obj_work->pos.x;
	effect_work->efct_com.obj_work.pos.y = target_obj_work->pos.y;
	effect_work->efct_com.obj_work.pos.z = GMD_OBJ_GIMMICK_POS_Z_FRONT;
	effect_work->efct_com.obj_work.dir.z = (u16)(nnArcTan2( FX_FX32_TO_F32(speed_y), FX_FX32_TO_F32(speed_x) ) - 0x4000);

	//U“®
	GMM_PAD_VIB_SMALL();
}

// ==========================================================================
// gmGmkEnBmprCheckGroupBonus
/*!
 *	ƒOƒ‹[ƒv‘SÁ‚µƒ{[ƒiƒX‚ª‚ ‚é‚©ƒ`ƒFƒbƒN
 *
 *	ƒOƒ‹[ƒv‚ªİ’è‚³‚ê‚Ä‚¢‚ÄAüˆÍ‚É“¯‚¶ƒOƒ‹[ƒv‚Ì‚à‚Ì‚ª‚È‚¢‚Æ‚«
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@retval	TRUE:ƒOƒ‹[ƒv‘SÁ‚µƒ{[ƒiƒX‚ª‚ ‚é
 *	@retval	FALSE:ƒOƒ‹[ƒv‘SÁ‚µƒ{[ƒiƒX‚ª‚È‚¢
 */
// ==========================================================================
BOOL gmGmkEnBmprCheckGroupBonus( OBS_OBJECT_WORK *obj_work )
{
	const GMS_ENEMY_3D_WORK* gimmick_work = (const GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//ƒ‰ƒCƒt‚ªc‚Á‚Ä‚¢‚é
	s32 life = gmGmkEnBmperGetUserWorkLife( obj_work );
	if ( life > 0 ){
		return FALSE;
	}
	
	//ƒOƒ‹[ƒvİ’è‚ª‚³‚ê‚Ä‚¢‚È‚¢
	s8 group_id = gimmick_work->ene_com.eve_rec->top;
	if ( 0 == group_id ){
		return FALSE;
	}

	OBS_OBJECT_WORK* target_obj_work = ObjObjectSearchRegistObject( NULL, GMD_OBJTYPE_GIMMICK );
	while ( target_obj_work ){
		const GMS_ENEMY_3D_WORK* target_gimmick_work = (const GMS_ENEMY_3D_WORK*)target_obj_work;
		//©g‚Í’e‚­
		if ( target_obj_work == obj_work ){
			;	//no op
		}
		//3‘Ïƒoƒ“ƒp[‚Å‚Í‚È‚¢
		else if ( target_gimmick_work->ene_com.eve_rec->id != GMD_EVENT_ID_EN_BMPR_0
				&& target_gimmick_work->ene_com.eve_rec->id != GMD_EVENT_ID_EN_BMPR_45
				&& target_gimmick_work->ene_com.eve_rec->id != GMD_EVENT_ID_EN_BMPR_90
				&& target_gimmick_work->ene_com.eve_rec->id != GMD_EVENT_ID_EN_BMPR_135
		){
			;	//no op
		}
		//“¯‚¶ƒOƒ‹[ƒv‚Ì3‘Ïƒoƒ“ƒp[‚ª‚ ‚é
		else if ( target_gimmick_work->ene_com.eve_rec->top == group_id ){
			//¶‘¶‚µ‚Ä‚¢‚é
			if ( target_obj_work->user_flag & GMD_GMK_ENBMPR_FLAG_ALIVE ){
				return FALSE;
			}
		}

		//Ÿ‚ÌƒIƒuƒWƒFƒNƒg
		target_obj_work = ObjObjectSearchRegistObject( target_obj_work, GMD_OBJTYPE_GIMMICK );
	}
	return TRUE;
}

// ==========================================================================
// gmGmkEnBmprChangeModeWait
/*!
 *	‘Ò‹@ƒ‚[ƒh‚ÉˆÈ~
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkEnBmprChangeModeWait( OBS_OBJECT_WORK* obj_work )
{
	//ƒ‚[ƒVƒ‡ƒ“•ÏX
	ObjDrawObjectActionSet3DNN( obj_work, IDB_GMK_BUMPER_MTN_GMK_EN_BMPR_ZNM, 0 );

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkEnBmprMainWait;
}

// ==========================================================================
// gmGmkEnBmprChangeModeHit
/*!
 *	ƒqƒbƒgƒ‚[ƒh‚ÉˆÈ~
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkEnBmprChangeModeHit( OBS_OBJECT_WORK* obj_work )
{	
	//Œø‰Ê‰¹
	GmSoundPlaySE("Casino7");

	//ƒ_ƒ[ƒW‹L‰¯
	u8 damage = 1;

	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );
	gimmick_work->ene_com.eve_rec->byte_param[1] += damage;

	//‘Ì—ÍŒ¸­
	s32 life = gmGmkEnBmperAddUserWorkLife( obj_work, -damage );
	if ( life < 0 ){
		//ˆ—‚È‚µ
		return ;
	}

	//ƒ}ƒeƒŠƒAƒ‹•ÏX
	ObjDrawObjectActionSet3DNNMaterial( obj_work, g_gm_gmk_en_bmpr_mat_motion_id[life] );

	//ƒ‚[ƒVƒ‡ƒ“•ÏX
	ObjDrawObjectActionSet3DNN( obj_work, IDB_GMK_BUMPER_MTN_GMK_EN_BMPR_HIT_ZNM, 0 );

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkEnBmprMainHit;
}

// ==========================================================================
// gmGmkEnBmprChangeModeLost
/*!
 *	Á–Åƒ‚[ƒh‚ÉˆÈ~
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkEnBmprChangeModeLost( OBS_OBJECT_WORK* obj_work )
{
	//ƒ‚[ƒVƒ‡ƒ“•ÏX
	ObjDrawObjectActionSet3DNN( obj_work, IDB_GMK_BUMPER_MTN_GMK_EN_BMPR_ZNM, 0 );

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkEnBmprMainLost;

	// ƒGƒtƒFƒNƒgˆ—
	GMS_EFFECT_3DES_WORK* effect_work = GmEfctCmnEsCreate(
			NULL,
			GME_EFCT_CMN_IDX_BUMPER_LOST );
	amAssert( effect_work );
	effect_work->efct_com.obj_work.pos.x = obj_work->pos.x;
	effect_work->efct_com.obj_work.pos.y = obj_work->pos.y;
	effect_work->efct_com.obj_work.pos.z = GMD_OBJ_GIMMICK_POS_Z_FRONT;
}

// ==========================================================================
// gmGmkEnBmprMainWait
/*!
 *	‘Ò‹@ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkEnBmprMainWait( OBS_OBJECT_WORK *obj_work )
{
	UNREFERENCED_PARAMETER(obj_work);
}

// ==========================================================================
// gmGmkEnBmprMainHit
/*!
 *	ƒqƒbƒgƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkEnBmprMainHit( OBS_OBJECT_WORK *obj_work )
{
	//ƒ‚[ƒVƒ‡ƒ“I‚í‚Á‚½‚Æ‚«
	if ( obj_work->disp_flag & OBD_DISP_END ){
		s32 life = gmGmkEnBmperGetUserWorkLife( obj_work );
		//‘Ì—Í‚ªc‚Á‚Ä‚¢‚é‚È‚çA‘Ò‚¿ƒ‚[ƒh
		if ( life > 0 ){
			gmGmkEnBmprChangeModeWait( obj_work );
		}
		//c‚Á‚Ä‚¢‚È‚¢‚È‚çAÁ–Å
		else {
			gmGmkEnBmprChangeModeLost( obj_work );
		}

	}
}

// ==========================================================================
// gmGmkEnBmprMainLost
/*!
 *	Á–ÅƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkEnBmprMainLost( OBS_OBJECT_WORK *obj_work )
{
	obj_work->flag |= OBD_OBJECT_TASKCLEAR;
}

// ==========================================================================
//ƒ†[ƒUƒtƒ‰ƒO
// ==========================================================================


// ==========================================================================
// gmGmkEnBmperSetUserWorkLife
/*!
 *	ƒ†[ƒUƒ[ƒN‚ğƒ‰ƒCƒt‚Æ‚µ‚Äİ’è
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param life		[in] ƒ‰ƒCƒt
 */
// ==========================================================================
void gmGmkEnBmperSetUserWorkLife( OBS_OBJECT_WORK* obj_work, s32 life )
{
	obj_work->user_work = (u32)life;
}

// ==========================================================================
// gmGmkEnBmperGetUserWorkLife
/*!
 *	ƒ†[ƒUƒ[ƒN‚ğƒ‰ƒCƒt‚Æ‚µ‚Äæ“¾
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return ƒ‰ƒCƒt
 */
// ==========================================================================
s32 gmGmkEnBmperGetUserWorkLife( OBS_OBJECT_WORK* obj_work )
{
	return (s32)obj_work->user_work;
}

// ==========================================================================
// gmGmkEnBmperAddUserWorkLife
/*!
 *	ƒ†[ƒUƒ[ƒN‚ğ–Ú“IŠp“x‚Æ‚µ‚Ä’Ç‰Á
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param add	[in] ‰ÁZƒ‰ƒCƒt
 *
 *	@return ‰ÁZŒã‚Ìƒ‰ƒCƒt
 */
// ==========================================================================
s32 gmGmkEnBmperAddUserWorkLife( OBS_OBJECT_WORK* obj_work, s32 add )
{
	obj_work->user_work += (u32)add;
	return (s32)obj_work->user_work;
}
// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
