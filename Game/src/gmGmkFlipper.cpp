// ==========================================================================
/*!
  @file gmGmkFlipper.cpp
  @brief ƒMƒ~ƒbƒN ƒtƒŠƒbƒp[

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkFlipper.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 *	GMS_EVE_RECORD_EVENT
 *		left		: X•ûŒü’l’²®
 *		top			: Y•ûŒü’l’²®
 *		width		: “Áêƒ^ƒCƒv—pƒXƒRƒAi1000‚ÌˆÊj
 *		height		: ‚È‚µ
 *		flag 1:G‚ê‚½Û‚Éƒz[ƒ~ƒ“ƒOƒAƒ^ƒbƒN‚ª‰ñ•œ‚µ‚È‚¢İ’è
 * @~~~~~~~~~¡ on  :‰ñ•œ‚µ‚È‚¢
 * @~~~~~~~~~  off :‰ñ•œ‚·‚é
 * @
 * @	flag 2:ƒXƒs[ƒhŒ¸Š‚ğs‚í‚È‚¢ŠÔ‚ğ’Êí‚æ‚è’·‚ß‚É‚·‚éİ’è
 * @~~~~~~~~¡~ on  :‰¡Œ^‚Í30ƒtƒŒ[ƒ€ cŒ^‚Í180ƒtƒŒ[ƒ€
 * @~~~~~~~~ ~ off :‰¡Œ^‚Í0ƒtƒŒ[ƒ€ cŒ^‚Í12ƒtƒŒ[ƒ€
 * 
 * Še•ûŒü•â³‚ÍŠ„‡‚Å•â³‚µ‚Ü‚·
 * d—Í—Ê‚Í•ÏX‚³‚ê‚È‚¢‚½‚ßAy•ûŒü‚Ì‹­‚³‚ÍáŠ±¬‚³‚ß‚É‚È‚è‚Ü‚·
 * 
 * ¦—á. 100‚ğİ’è‚·‚é‚Æ+100%i=2”{j
 *       -50‚ğİ’è‚·‚é‚Æ-50%i=”¼•ªj
 * 
 * cŒ^ƒtƒŠƒbƒp[‚Ìê‡
 * “Áêƒ^ƒCƒv—pƒXƒRƒA‚ªİ’è‚³‚ê‚Ä‚¢‚é‚Æ“Áêƒ^ƒCƒv‚É‚È‚é
 * –Ú•WƒXƒRƒA‚Íİ’è’l‚Ì1000”{
 * 
 * ¦—á. 100‚ğİ’è‚·‚é‚Æ10–œ“_
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

#include "gmGmkFlipper.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "common/model/gmk_flipper_mdl.hmb"
#include "common/model/gmk_flipper_mat.hmb"


//----- Definitions ---------------------------------------------------------

#define GMD_FLIPPER_TEST_SPTYPE_THROUGH_BUG	(0)	//“ÁêcŒ^ƒtƒŠƒbƒp[‚ğ’Ê‚è”²‚¯‚½ê‡AƒŠƒ“ƒOMAX‚É‚·‚éƒfƒoƒbƒO—p


//ƒtƒŠƒbƒp[ƒ^ƒCƒv
enum GME_GMK_FLIPPER_TYPE{
	GMD_GMK_FLIPPER_TYPE_UL = 0,	//ƒtƒŠƒbƒp[ã‚É‚Í‚¶‚­i‰E‘¤j
	GMD_GMK_FLIPPER_TYPE_UR,		//ƒtƒŠƒbƒp[ã‚É‚Í‚¶‚­i¶‘¤j
	GMD_GMK_FLIPPER_TYPE_LR,		//ƒtƒŠƒbƒp[¶‰E‚É‚Í‚¶‚­

	GMD_GMK_FLIPPER_TYPE_NUM
};

#define GMD_GMK_FLIPPER_RECT_OFFSET_PLAYER_Y	(12)			//“–‚½‚è”»’èƒIƒtƒZƒbƒg

#define GMD_GMK_FLIPPER_POS_Z				(-30*FX32_ONE)		//•\¦ˆÊ’u

#define GMD_GMK_FLIPPER_WIDTH				(25)				//‰¡Œ^ƒtƒŠƒbƒp[‚Ì”ò‚Ô‚‚³‚ğ•â³‚·‚é•

//‹éŒ`ƒTƒCƒY
#define GMD_GMK_FLIPPER_UL_RECT_LEFT		(-8)
#define GMD_GMK_FLIPPER_UL_RECT_RIGHT		(40)
#define GMD_GMK_FLIPPER_UL_RECT_TOP			(-12)
#define GMD_GMK_FLIPPER_UL_RECT_BOTTOM		(16)
#define GMD_GMK_FLIPPER_UR_RECT_LEFT		(-GMD_GMK_FLIPPER_UL_RECT_RIGHT)
#define GMD_GMK_FLIPPER_UR_RECT_RIGHT		(-GMD_GMK_FLIPPER_UL_RECT_LEFT)
#define GMD_GMK_FLIPPER_UR_RECT_TOP			(GMD_GMK_FLIPPER_UL_RECT_TOP)
#define GMD_GMK_FLIPPER_UR_RECT_BOTTOM		(GMD_GMK_FLIPPER_UL_RECT_BOTTOM)
#define GMD_GMK_FLIPPER_LR_RECT_LEFT			(-8)
#define GMD_GMK_FLIPPER_LR_RECT_RIGHT		(8)
#define GMD_GMK_FLIPPER_LR_RECT_TOP			(8)
#define GMD_GMK_FLIPPER_LR_RECT_BOTTOM		(32)

#define GMD_GMK_FLIPPER_CALC_RIDE_OFFSET	(-2.0f)					//‰¡Œ^ƒtƒŠƒbƒp[‚Éæ‚Á‚½Û‚ÌÀ•WŒvZ—pƒIƒtƒZƒbƒg
#define GMD_GMK_FLIPPER_OFFSET_PLAYER_Y_NORMAL	(-9*FX32_ONE)		//‰¡Œ^ƒtƒŠƒbƒp[‚Éæ‚Á‚½Û‚ÌƒvƒŒƒCƒ„ƒIƒtƒZƒbƒg
#define GMD_GMK_FLIPPER_OFFSET_PLAYER_Y_PINBALL	(-15*FX32_ONE)		//‰¡Œ^ƒtƒŠƒbƒp[‚Éæ‚Á‚½Û‚ÌƒvƒŒƒCƒ„ƒIƒtƒZƒbƒg
#define GMD_GMK_FLIPPER_RIDE_SPEED_X		((float)(GMD_GMK_FLIPPER_UL_RECT_RIGHT-GMD_GMK_FLIPPER_UL_RECT_LEFT)/70.0f)	
																	//‰¡Œ^ƒtƒŠƒbƒp[‚Éæ‚Á‚½Û‚ÌƒXƒs[ƒh
#define GMD_GMK_FLIPPER_RIDE_SPEED_Y		((float)(GMD_GMK_FLIPPER_UL_RECT_BOTTOM-GMD_GMK_FLIPPER_UL_RECT_TOP+GMD_GMK_FLIPPER_CALC_RIDE_OFFSET)/70.0f)	
																	//‰¡Œ^ƒtƒŠƒbƒp[‚Éæ‚Á‚½Û‚ÌƒXƒs[ƒh

#define GMD_GMK_FLIPPER_WAIT_SPEED_LR_X		(0x00015000)		//cŒ^ƒtƒŠƒbƒp[‚ÌƒXƒs[ƒh
#define GMD_GMK_FLIPPER_WAIT_SPEED_LR_Y		(0x00000000)		//cŒ^ƒtƒŠƒbƒp[‚ÌƒXƒs[ƒh

#define GMD_GMK_FLIPPER_WAIT_SPEED_U_X		(0x00003000)		//‰¡Œ^ƒtƒŠƒbƒp[‚ÌƒXƒs[ƒh
#define GMD_GMK_FLIPPER_WAIT_SPEED_U_Y		(-0x0000D000)		//‰¡Œ^ƒtƒŠƒbƒp[‚ÌƒXƒs[ƒh

#define GMD_GMK_FLIPPER_ANGLE_READY			(0x1f00)			//ƒqƒbƒg‚³‚¹‚é€”õ‚É“ü‚éŒXÎŠp“x

#define GMD_GMK_FLIPPER_FOOK_MOTION_FRAME	(6)					//ƒtƒbƒNó‘Ô‚Ìƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€”
#define GMD_GMK_FLIPPER_FOOK_MARGIN_X		((fx32)4*FX32_ONE)	//ƒtƒbƒNó‘Ô‚É‚È‚é•


#define GMD_GMK_FLIPPER_NO_MOVE_TIME		( 5 )				//‚Í‚¶‚©‚ê‚½Œã‘€ì‚Å‚«‚È‚¢ŠÔ
#define GMD_GMK_FLIPPER_NO_SPDDOWN_TIMER_LR_DEFAULT	(0)			//Œ¸‘¬‚È‚µŠÔi‰¡Œ^’Êíj
#define GMD_GMK_FLIPPER_NO_SPDDOWN_TIMER_LR_SPECIAL	(30)		//Œ¸‘¬‚È‚µŠÔi‰¡Œ^“Áêj
#define GMD_GMK_FLIPPER_NO_SPDDOWN_TIMER_U_DEFAULT	(12)		//Œ¸‘¬‚È‚µŠÔicŒ^’Êíj
#define GMD_GMK_FLIPPER_NO_SPDDOWN_TIMER_U_SPECIAL	(180)		//Œ¸‘¬‚È‚µŠÔicŒ^“Áêj

//ƒtƒ‰ƒO
#define GMD_GMK_FLIPPER_FLAG_NO_RECOVER_HOMING	(1<<0)	//ƒz[ƒ~ƒ“ƒO‰ñ•œ‚µ‚È‚¢ƒtƒ‰ƒO
#define GMD_GMK_FLIPPER_FLAG_NO_SPDDOWN_TIMER	(1<<1)	//ƒXƒs[ƒhƒ_ƒEƒ“‚µ‚È‚¢ŠÔİ’èƒtƒ‰ƒO

#define GMD_GMK_FLIPPER_HIT_FRAME			(6)	//‰ñ“]ƒtƒŒ[ƒ€”
#define GMD_GMK_FLIPPER_HIT_ANGLE			(70.0f)	//‰ñ“]Šp“x


//ƒ[ƒN
typedef struct tag_GMS_GMK_FLIPPER_WORK{
	GMS_ENEMY_3D_WORK gimmick_work;	//ƒMƒ~ƒbƒNƒ[ƒN
	OBS_ACTION3D_NN_WORK obj_3d_parts;	//ƒp[ƒc
}GMS_GMK_FLIPPER_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static u32 gmGmkFlipperGameSystemGetSyncTime( void );
static GMS_ENEMY_3D_WORK* gmGmkFlipperLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_FLIPPER_TYPE type );
static GMS_ENEMY_3D_WORK* gmGmkFlipperLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_FLIPPER_TYPE type );

//---------------------------------------------------------
//ƒtƒŠƒbƒp[
//---------------------------------------------------------
static void gmGmkFlipperInit( OBS_OBJECT_WORK *obj_work, GME_GMK_FLIPPER_TYPE flipper_type );
static void gmGmkFlipperSetRect( GMS_ENEMY_3D_WORK* gimmick_work, GME_GMK_FLIPPER_TYPE flipper_type );

static void gmGmkFlipperDrawFunc( OBS_OBJECT_WORK* work );

static GME_GMK_FLIPPER_TYPE gmGmkFlipperCalcType( GME_EVENT_ID id );
static void gmGmkFlipperDefPlayer( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* target_rect );
static void gmGmkFlipperDefEnemy( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* target_rect );
static void gmGmkFlipperDefFuncU( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* player_rect );
static void gmGmkFlipperDefFuncLR( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* player_rect );

static void gmGmkFlipperChangeModeWait( OBS_OBJECT_WORK* obj_work );
static void gmGmkFlipperChangeModeReady( OBS_OBJECT_WORK* obj_work );
static void gmGmkFlipperChangeModeHit( OBS_OBJECT_WORK* obj_work );
static void gmGmkFlipperChangeModeHook( OBS_OBJECT_WORK* obj_work );
static void gmGmkFlipperChangeModeOpen( OBS_OBJECT_WORK* obj_work );
static void gmGmkFlipperMainWait( OBS_OBJECT_WORK *obj_work );
static void gmGmkFlipperMainReady( OBS_OBJECT_WORK *obj_work );
static void gmGmkFlipperMainHit( OBS_OBJECT_WORK *obj_work );
static void gmGmkFlipperMainHook( OBS_OBJECT_WORK *obj_work );


static BOOL gmGmkFlipperCheckKeyHit(
							 OBS_OBJECT_WORK* gimmick_obj_work, 
							 GMS_PLAYER_WORK* player_work );
static BOOL gmGmkFlipperCheckControlPlayer( void );
static BOOL gmGmkFlipperCheckScore( OBS_OBJECT_WORK* obj_work );

static BOOL gmGmkFlipperCheckLeft( const VecFx32* line_start, const VecFx32* line_end, const VecFx32* point );
static BOOL gmGmkFlipperCheckRect( 
						 const VecFx32* gimmick_pos,
						 const VecFx32* target_pos,
						 GME_GMK_FLIPPER_TYPE type);
static BOOL gmGmkFlipperCheckHook( OBS_OBJECT_WORK* obj_work );
static void gmGmkFlipperSetRideSpeed(
							  OBS_OBJECT_WORK* target_obj_work,
							  const OBS_OBJECT_WORK* gimmick_obj_work,
							  GME_GMK_FLIPPER_TYPE flipper_type );
static fx32 gmGmkFlipperCalcRideOffsetY(
							  fx32 x,
							  const OBS_OBJECT_WORK* gimmick_obj_work,
							  GME_GMK_FLIPPER_TYPE flipper_type );
static BOOL gmGmkFlipperUpdateAngle( OBS_OBJECT_WORK *obj_work );
//---------------------------------------------------------
//ƒ†[ƒUƒ[ƒN
//---------------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//ƒ^ƒCƒv•Êƒ‚ƒfƒ‹ID
static const s32 g_gm_gmk_flipper_model_id[GMD_GMK_FLIPPER_TYPE_NUM] = {
	IDB_GMK_FLIPPER_MDL_GMK_FLIPPER_ZNO,	//
	IDB_GMK_FLIPPER_MDL_GMK_FLIPPER_ZNO,	//
	IDB_GMK_FLIPPER_MDL_GMK_FLIPPER_TT_ZNO,	//
};
#if !_IPHONE
//ƒ^ƒCƒv•ÊƒTƒuƒ‚ƒfƒ‹ID
static const s32 g_gm_gmk_flipper_sub_model_id[GMD_GMK_FLIPPER_TYPE_NUM] = {
	IDB_GMK_FLIPPER_MDL_GMK_FLIPPER_GRE_ZNO,	//
	IDB_GMK_FLIPPER_MDL_GMK_FLIPPER_GRE_ZNO,	//
	IDB_GMK_FLIPPER_MDL_GMK_FLIPPER_GRE_ORE_ZNO,	//
};
#endif // !_IPHONE
//ƒ^ƒCƒv•Êƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“ID
static const s32 g_gm_gmk_flipper_mat_motion_id[GMD_GMK_FLIPPER_TYPE_NUM] = {
	IDB_GMK_FLIPPER_MAT_GMK_FLIPPER_ZNV,	//
	IDB_GMK_FLIPPER_MAT_GMK_FLIPPER_ZNV,	//
	IDB_GMK_FLIPPER_MAT_GMK_FLIPPER_TT_ZNV,	//
};

//ƒ^ƒCƒv•ÊZŠp“x
static const u16 g_gm_gmk_flipper_angle_z[GMD_GMK_FLIPPER_TYPE_NUM] = {
	0x4f00,
	(u16)-0x4f00,
	0x8000,
};


//‹éŒ`
static const s16 g_gmk_flipper_rect[GMD_GMK_FLIPPER_TYPE_NUM][MTD_RECT] = {
	{GMD_GMK_FLIPPER_UL_RECT_LEFT, GMD_GMK_FLIPPER_UL_RECT_TOP, GMD_GMK_FLIPPER_UL_RECT_RIGHT, GMD_GMK_FLIPPER_UL_RECT_BOTTOM},
	{GMD_GMK_FLIPPER_UR_RECT_LEFT, GMD_GMK_FLIPPER_UR_RECT_TOP, GMD_GMK_FLIPPER_UR_RECT_RIGHT, GMD_GMK_FLIPPER_UR_RECT_BOTTOM},
	{GMD_GMK_FLIPPER_LR_RECT_LEFT, GMD_GMK_FLIPPER_LR_RECT_TOP, GMD_GMK_FLIPPER_LR_RECT_RIGHT, GMD_GMK_FLIPPER_LR_RECT_BOTTOM},
};

static OBS_ACTION3D_NN_WORK* g_gm_gmk_flipper_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkFlipperBuild
/*!
 *	ƒMƒ~ƒbƒN ƒtƒŠƒbƒp[ ƒf[ƒ^\’z
 */
// ==========================================================================
void GmGmkFlipperBuild(void)
{
	g_gm_gmk_flipper_obj_3d_list = GmGameDBuildRegBuildModel(
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_FLIPPER_MODEL ),
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_FLIPPER_TEX ),
			0	//draw_flag
			);
}

// ==========================================================================
// GmGmkFlipperFlush
/*!
 *	ƒMƒ~ƒbƒN ƒtƒŠƒbƒp[ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
void GmGmkFlipperFlush(void)
{
	AMS_AMB_HEADER* amb_header = (AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_FLIPPER_MODEL );

	GmGameDBuildRegFlushModel( g_gm_gmk_flipper_obj_3d_list, amb_header->file_num );
	g_gm_gmk_flipper_obj_3d_list = NULL;
}

// ==========================================================================
// GmGmkFlipperInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ” ƒtƒŠƒbƒp[
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] 
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkFlipperInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	UNREFERENCED_PARAMETER(type);
	//ƒ^ƒCƒv
	GME_GMK_FLIPPER_TYPE flipper_type = gmGmkFlipperCalcType( (GME_EVENT_ID)eve_rec->id );

	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkFlipperLoadObj( eve_rec, pos_x, pos_y, flipper_type );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK*obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkFlipperInit( obj_work, flipper_type );
	return obj_work;
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkFlipperGameSystemGetSyncTime
/*!
 * ƒQ[ƒ€ƒVƒXƒeƒ€‚©‚ç“¯ŠúŠÔ‚ğæ“¾
 *
 *	@return	“¯ŠúŠÔ
 */
// ==========================================================================
u32 gmGmkFlipperGameSystemGetSyncTime( void )
{
	return g_gm_main_system.sync_time;
}

// ==========================================================================
// gmGmkFlipperLoadObjNoModel
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
GMS_ENEMY_3D_WORK* gmGmkFlipperLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_FLIPPER_TYPE type )
{
	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = NULL;
	if ( type == GMD_GMK_FLIPPER_TYPE_LR ){
		gimmick_work = (GMS_ENEMY_3D_WORK*)GMM_ENEMY_CREATE_RIDE_WORK(
				eve_rec, 
				pos_x, 
				pos_y, 
				sizeof(GMS_GMK_FLIPPER_WORK), 
				"GMK_FLIPPER_LR");
	}
	else {
		gimmick_work = (GMS_ENEMY_3D_WORK*)GMM_ENEMY_CREATE_WORK(
				eve_rec, 
				pos_x, 
				pos_y, 
				sizeof(GMS_GMK_FLIPPER_WORK), 
				"GMK_FLIPPER_U");
	}
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}

// ==========================================================================
// gmGmkFlipperLoadObj
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
GMS_ENEMY_3D_WORK* gmGmkFlipperLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_FLIPPER_TYPE type )
{

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkFlipperLoadObjNoModel(
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
	s32 data_model_index = g_gm_gmk_flipper_model_id[type];
	//“Ç‚İ‚İ
	ObjObjectCopyAction3dNNModel(
		obj_work,
		&g_gm_gmk_flipper_obj_3d_list[data_model_index],
		&gimmick_work->obj_3d);

	//-------------------------------------------------
	// ƒ‚[ƒVƒ‡ƒ“‰Šú‰»
	//-------------------------------------------------
	//ƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“
	OBS_DATA_WORK* data_mat_motion = ObjDataGet(GMD_DWORK_NO_GMK_FLIPPER_MAT);
	amAssert( data_mat_motion );
	ObjObjectAction3dNNMaterialMotionLoad( 
			obj_work,
			0,
			data_mat_motion,
			NULL,
			0,
			NULL
	);
#if !_IPHONE
	//-------------------------------------------------
	//ƒTƒuƒ‚ƒfƒ‹‰Šú‰»
	//-------------------------------------------------
	GMS_GMK_FLIPPER_WORK* flipper_work = (GMS_GMK_FLIPPER_WORK*)obj_work;
	s32 data_model_index_sub = g_gm_gmk_flipper_sub_model_id[type];
	ObjCopyAction3dNNModel(
		&g_gm_gmk_flipper_obj_3d_list[data_model_index_sub],
		&flipper_work->obj_3d_parts);
#endif // !_IPHONE
	//-------------------------------------------------
	// ’nŒ`İ’è
	//-------------------------------------------------
	if ( type == GMD_GMK_FLIPPER_TYPE_LR ){
		gimmick_work->ene_com.col_work.obj_col.obj = &gimmick_work->ene_com.obj_work;

		gimmick_work->ene_com.col_work.obj_col.width = 1*16;
		gimmick_work->ene_com.col_work.obj_col.height = 1*8;
		gimmick_work->ene_com.col_work.obj_col.ofst_x = (s16)(-gimmick_work->ene_com.col_work.obj_col.width /2);
		gimmick_work->ene_com.col_work.obj_col.ofst_y = -8+1;
		gimmick_work->ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NODIR_PARENT;
	}

	return gimmick_work;
}

// ==========================================================================
// gmGmkFlipperInit
/*!
 *	ƒMƒ~ƒbƒN@ƒtƒŠƒbƒp[@‰Šú‰»
 *
 *	@param obj_work		[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param flipper_type	[in] ƒ^ƒCƒv
 */
// ==========================================================================
void gmGmkFlipperInit( OBS_OBJECT_WORK *obj_work, GME_GMK_FLIPPER_TYPE flipper_type )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	//ƒvƒŒƒCƒ„[‚ ‚½‚è
	gmGmkFlipperSetRect( gimmick_work, flipper_type );

	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->move_flag = OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;

	//Œü‚«•Êİ’è
	obj_work->dir.z = g_gm_gmk_flipper_angle_z[flipper_type];
	if ( flipper_type == GMD_GMK_FLIPPER_TYPE_UL ){
		obj_work->user_flag = 1;
	}
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	//ƒ‚[ƒVƒ‡ƒ“
	ObjDrawObjectActionSet3DNNMaterial( obj_work, g_gm_gmk_flipper_mat_motion_id[flipper_type] );
	obj_work->disp_flag |= OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP;

	//À•W
	obj_work->pos.z = GMD_GMK_FLIPPER_POS_Z;

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = NULL;
	obj_work->ppMove = NULL;
	obj_work->ppOut = gmGmkFlipperDrawFunc;

	//‘Ò‹@ƒ‚[ƒh‚É
	gmGmkFlipperChangeModeWait( obj_work );
}

// ==========================================================================
// gmGmkFlipperSetRect
/*!
 *	ƒMƒ~ƒbƒN@ƒtƒŠƒbƒp[@‹éŒ`İ’è
 *
 *	@param gimmick_work	[in] ƒMƒ~ƒbƒNƒ[ƒN
 *	@param flipper_type	[in] ƒ^ƒCƒv
 */
// ==========================================================================
void gmGmkFlipperSetRect( GMS_ENEMY_3D_WORK* gimmick_work, GME_GMK_FLIPPER_TYPE flipper_type )
{
	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	
	switch ( flipper_type ){
	case GMD_GMK_FLIPPER_TYPE_UL:
		rect_work->ppDef = gmGmkFlipperDefFuncU;
		break;
	case GMD_GMK_FLIPPER_TYPE_UR:
		rect_work->ppDef = gmGmkFlipperDefFuncU;
		break;
	case GMD_GMK_FLIPPER_TYPE_LR:
		rect_work->ppDef = gmGmkFlipperDefFuncLR;
		break;
	default:
		amAssert( FALSE );
		break;
	}
	ObjRectWorkZSet(
			rect_work,
			g_gmk_flipper_rect[flipper_type][MTD_LEFT], g_gmk_flipper_rect[flipper_type][MTD_TOP], -500,
			g_gmk_flipper_rect[flipper_type][MTD_RIGHT], g_gmk_flipper_rect[flipper_type][MTD_BOTTOM],  500);
	
	rect_work->flag |= OBD_RECT_OUT;

	//–hŒä—p
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
}

// ==========================================================================
// gmGmkFlipperDrawFunc
/*!
 *	ƒMƒ~ƒbƒN@ƒtƒŠƒbƒp[@•`‰æŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkFlipperDrawFunc( OBS_OBJECT_WORK* obj_work )
{
	//---------------------------------------
	//mainƒp[ƒc
	//---------------------------------------
	OBS_ACTION3D_NN_WORK *obj_3d = obj_work->obj_3d;
	amAssert( obj_3d );
	if ( obj_3d->motion ){
		//ƒ‚[ƒVƒ‡ƒ“‚ğ“¯Šú‚³‚¹‚é
		float frame_start = amMotionMaterialGetStartFrame( obj_3d->motion, obj_3d->mat_act_id );
		float frame_end = amMotionMaterialGetEndFrame( obj_3d->motion, obj_3d->mat_act_id );
		float frame_max = frame_end - frame_start;

		float frame = (float)gmGmkFlipperGameSystemGetSyncTime();

		obj_3d->mat_frame = fmod( frame, frame_max );
	}

	ObjDrawActionSummary( obj_work );
#if !_IPHONE
	//---------------------------------------
	//ƒTƒuƒp[ƒc
	//---------------------------------------	
	u32 disp_flag = obj_work->disp_flag;
	
	GMS_GMK_FLIPPER_WORK* flipper_work = (GMS_GMK_FLIPPER_WORK*)obj_work;
	VecFx32 pos = obj_work->pos;
	pos.z += 8*FX32_ONE;

	ObjDrawAction3DNN(
			&flipper_work->obj_3d_parts,
			&pos, 
			&obj_work->dir, 
			&obj_work->scale, 
			&disp_flag );
#endif // !_IPHONE
}

// ==========================================================================
// gmGmkFlipperCalcType
/*!
 *	ƒMƒ~ƒbƒN@ƒtƒŠƒbƒp[@ƒ^ƒCƒvæ“¾
 *
 *	@param id	[in] ƒCƒxƒ“ƒgID
 */
// ==========================================================================
GME_GMK_FLIPPER_TYPE gmGmkFlipperCalcType( GME_EVENT_ID id )
{
	GME_GMK_FLIPPER_TYPE flipper_type = (GME_GMK_FLIPPER_TYPE)(id - GMD_EVENT_ID_FLIPPER_UL);
	amAssert( GMD_GMK_FLIPPER_TYPE_UL <= flipper_type && flipper_type < GMD_GMK_FLIPPER_TYPE_NUM );

	return flipper_type;
}

// ==========================================================================
// gmGmkFlipperDefFuncU
/*!
 *	ƒMƒ~ƒbƒN@ƒtƒŠƒbƒp[@ŠJnó‘Ô‹éŒ`ŠÖ”
 *
 *	@param gimmick_rect	[in] ©g‚Ì‹éŒ`ƒ[ƒN
 *	@param target_rect	[in] ‘Šè‚Ì‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkFlipperDefPlayer( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* target_rect )
{
	amAssert( gimmick_rect );
	amAssert( target_rect );

	//ƒMƒ~ƒbƒNƒ[ƒN
	OBS_OBJECT_WORK* gimmick_obj_work = gimmick_rect->parent_obj;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)gimmick_obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* target_obj_work = target_rect->parent_obj;
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)target_obj_work;
	amAssert( player_work );

	//ƒ{ƒX2ƒV[ƒPƒ“ƒX’†
	if ( player_work->seq_state >= GME_PLY_SEQ_STATE_GMK_BOSS2_CATCH ){
		return;
	}

	GME_GMK_FLIPPER_TYPE flipper_type = gmGmkFlipperCalcType( (GME_EVENT_ID)gimmick_work->ene_com.eve_rec->id );

	//ƒ^[ƒQƒbƒgİ’è
	gimmick_work->ene_com.target_obj = target_obj_work;
	
	//‰º‘¤‚©‚ç‚ ‚½‚Á‚½ê‡
	fx32 distance_y = gmGmkFlipperCalcRideOffsetY(
			target_obj_work->pos.x,
			gimmick_obj_work,
			flipper_type );
	if ( gimmick_obj_work->pos.y + distance_y < target_obj_work->pos.y ){
		//ªŒ³‘¤‚©‚ç‚ ‚½‚Á‚½ê‡
		fx32 dir_target_x = target_obj_work->pos.x - gimmick_obj_work->pos.x;
		if ( flipper_type == GMD_GMK_FLIPPER_TYPE_UR ){
			dir_target_x = -dir_target_x;
		}
		if ( dir_target_x < 0 ){
			target_obj_work->spd.x = 0;
		}

		//ƒz[ƒ~ƒ“ƒO‰ñ•œ‚µ‚È‚¢ƒtƒ‰ƒO
		BOOL flag_no_recover_homing = FALSE;
		if ( gimmick_work->ene_com.eve_rec->flag & GMD_GMK_FLIPPER_FLAG_NO_RECOVER_HOMING ){
			flag_no_recover_homing = TRUE;
		}
		//ƒvƒŒƒCƒ„ƒV[ƒPƒ“ƒX
		GmPlySeqInitPinballAir(
				player_work, 
				target_obj_work->spd.x, 
				2*FX32_ONE, 
				GMD_GMK_FLIPPER_NO_MOVE_TIME,
				flag_no_recover_homing);
		return ;
	}

	//ƒqƒbƒg”»’è
	if ( !gmGmkFlipperCheckRect(&gimmick_obj_work->pos, &target_obj_work->pos, flipper_type) ){	
		gimmick_rect->flag &= ~OBD_RECT_OUT;
		return;
	}

	//ƒ‚[ƒh•ÏX
	gmGmkFlipperChangeModeReady( gimmick_obj_work );
	gimmick_rect->flag |= OBD_RECT_OUT;

	//ƒvƒŒƒCƒ„
	gmGmkFlipperSetRideSpeed( target_obj_work, gimmick_obj_work, flipper_type );
	GmPlySeqInitFlipper(
			(GMS_PLAYER_WORK*)target_obj_work, 
			target_obj_work->spd.x, 
			target_obj_work->spd.y, 
			&gimmick_work->ene_com );

	//ŒXÎ‚Éİ’u‚³‚¹‚é
	fx32 offset_y = distance_y;
	if ( player_work->player_flag & GMD_PLF_PINBALL_SONIC ){
		offset_y += GMD_GMK_FLIPPER_OFFSET_PLAYER_Y_PINBALL;
	}
	else{
		offset_y += GMD_GMK_FLIPPER_OFFSET_PLAYER_Y_NORMAL;
	}
	target_obj_work->pos.y = gimmick_obj_work->pos.y + offset_y;
}
void gmGmkFlipperDefEnemy( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* target_rect )
{
	amAssert( gimmick_rect );
	amAssert( target_rect );

	//ƒMƒ~ƒbƒNƒ[ƒN
	OBS_OBJECT_WORK* gimmick_obj_work = gimmick_rect->parent_obj;
	amAssert( gimmick_obj_work );

	OBS_OBJECT_WORK* target_obj_work = target_rect->parent_obj;
	amAssert( target_obj_work );

	//ƒ{ƒX2‚Ì‚İ
	GMS_ENEMY_3D_WORK* enemy_work = (GMS_ENEMY_3D_WORK*)target_obj_work;
	if ( enemy_work->ene_com.eve_rec->id != GMD_EVENT_ID_BOSS2_BODY ){
		return;
	}

	//‰º‘¤‚©‚ç‚ ‚½‚Á‚½ê‡
	if ( gimmick_obj_work->pos.y < target_obj_work->pos.y ){
		//ˆ—‚È‚µ
		return;
	}

	//”½Ë
	target_obj_work->spd.y = -target_obj_work->spd.y;
	//‹²‚Ü‚ç‚È‚¢‚æ‚¤‚É•â³‚·‚é
	if (MTM_MATH_ABS(target_obj_work->spd.x) < 0x00000100L){
		target_obj_work->spd.x = 0x00000100L;
	}
}
void gmGmkFlipperDefFuncU( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* target_rect )
{
	amAssert( gimmick_rect );
	amAssert( target_rect );

	OBS_OBJECT_WORK* target_obj_work = target_rect->parent_obj;
	amAssert( target_obj_work );
	
	//ƒvƒŒƒCƒ„
	if ( target_obj_work->obj_type == GMD_OBJTYPE_PLAYER ){
		gmGmkFlipperDefPlayer( gimmick_rect, target_rect );
	}
	//‚»‚êˆÈŠO
	else if ( target_obj_work->obj_type == GMD_OBJTYPE_ENEMY ){
		gmGmkFlipperDefEnemy( gimmick_rect, target_rect );
	}

}

// ==========================================================================
// gmGmkFlipperDefFuncLR
/*!
 *	ƒMƒ~ƒbƒN@ƒtƒŠƒbƒp[@ŠJnó‘Ô‹éŒ`ŠÖ”
 *
 *	@param gimmick_rect	[in] ©g‚Ì‹éŒ`ƒ[ƒN
 *	@param target_rect	[in] ‘Šè‚Ì‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkFlipperDefFuncLR( OBS_RECT_WORK* gimmick_rect, OBS_RECT_WORK* target_rect )
{
	amAssert( gimmick_rect );
	amAssert( target_rect );

	//ƒMƒ~ƒbƒNƒ[ƒN
	OBS_OBJECT_WORK* gimmick_obj_work = gimmick_rect->parent_obj;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)gimmick_obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* target_obj_work = target_rect->parent_obj;
	amAssert( target_obj_work );
	
	//ƒvƒŒƒCƒ„ˆÈŠO‚Í”»’è‚µ‚È‚¢
	if ( target_obj_work->obj_type != GMD_OBJTYPE_PLAYER ){
		return;
	}

	//ƒ^[ƒQƒbƒgİ’è
	gimmick_work->ene_com.target_obj = target_obj_work;

	fx32 speed_x = GMD_GMK_FLIPPER_WAIT_SPEED_LR_X;
	fx32 speed_y = GMD_GMK_FLIPPER_WAIT_SPEED_LR_Y;

	//ƒXƒRƒA‚ªİ’è‚³‚ê‚Ä‚¢‚È‚¢‚Æ‚«‚ÍŒü‚«‚ğŒ©‚Ä’e‚­
	u32 open_score = (u32)(gimmick_work->ene_com.eve_rec->width * 1000);
	if ( 0 == open_score ){
		//Œü‚«”»’è
		if ( target_obj_work->pos.x < gimmick_obj_work->pos.x ){
			gimmick_obj_work->user_flag = 0;
			speed_x *= -1;
		}
		else{
			gimmick_obj_work->user_flag = 1;
		}
	}
	//ƒXƒRƒA‚ªİ’è‚³‚ê‚Ä‚¢‚é‚Í¶‚É’e‚­
	else{
#if GMD_FLIPPER_TEST_SPTYPE_THROUGH_BUG
		if ( !(target_obj_work->pos.x < gimmick_obj_work->pos.x) ){
			GmPlayerRingGet((GMS_PLAYER_WORK*)target_obj_work,999);
		}
#endif	// GMD_FLIPPER_TEST_SPTYPE_THROUGH_BUG
		gimmick_obj_work->user_flag = 0;
		speed_x *= -1;
	}

	//”z’uƒc[ƒ‹İ’è‚É‚æ‚é•â³
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

	//ƒ‚[ƒh•ÏX
	gmGmkFlipperChangeModeHit( gimmick_obj_work );

	//ƒvƒŒƒCƒ„‚È‚ç
	if ( 1 ){
		//no spddown timerƒtƒ‰ƒO
		s32 no_spddown_timer = GMD_GMK_FLIPPER_NO_SPDDOWN_TIMER_U_DEFAULT;
		if ( gimmick_work->ene_com.eve_rec->flag & GMD_GMK_FLIPPER_FLAG_NO_SPDDOWN_TIMER ){
			no_spddown_timer = GMD_GMK_FLIPPER_NO_SPDDOWN_TIMER_U_SPECIAL;
		}

		//ƒV[ƒPƒ“ƒX•ÏX
		GmPlySeqInitPinball((GMS_PLAYER_WORK*)target_obj_work, speed_x, speed_y, no_spddown_timer);

		// ƒGƒtƒFƒNƒgˆ—
		GMS_EFFECT_3DES_WORK* effect_work = GmEfctCmnEsCreate(
				gimmick_obj_work,
				GME_EFCT_CMN_IDX_BUMPER );
		amAssert( effect_work );
		effect_work->efct_com.obj_work.pos.x = target_obj_work->pos.x;
		effect_work->efct_com.obj_work.pos.y = target_obj_work->pos.y;
		effect_work->efct_com.obj_work.pos.z = GMD_OBJ_GIMMICK_POS_Z_FRONT;
		effect_work->efct_com.obj_work.dir.z = (u16)(nnArcTan2( FX_FX32_TO_F32(speed_y), FX_FX32_TO_F32(speed_x) ) - 0x4000);
	}
	else{
		
	}
}

// ==========================================================================
// gmGmkFlipperChangeModeWait
/*!
 *	‘Ò‹@ƒ‚[ƒh‚ÉˆÈ~
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkFlipperChangeModeWait( OBS_OBJECT_WORK* obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//ƒ^ƒCƒv
	GME_GMK_FLIPPER_TYPE flipper_type = gmGmkFlipperCalcType( (GME_EVENT_ID)gimmick_work->ene_com.eve_rec->id );

	//‰ñ“]Šp“xİ’è
	obj_work->user_work = g_gm_gmk_flipper_angle_z[flipper_type];

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkFlipperMainWait;
}

// ==========================================================================
// gmGmkFlipperChangeModeReady
/*!
 *	€”õ‚n‚jƒ‚[ƒh‚ÉˆÈ~
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkFlipperChangeModeReady( OBS_OBJECT_WORK* obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//ƒ^ƒCƒv
	GME_GMK_FLIPPER_TYPE flipper_type = gmGmkFlipperCalcType( (GME_EVENT_ID)gimmick_work->ene_com.eve_rec->id );

	//‰ñ“]Šp“xİ’è
	obj_work->user_work = g_gm_gmk_flipper_angle_z[flipper_type];

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkFlipperMainReady;	
}

// ==========================================================================
// gmGmkFlipperChangeModeHit
/*!
 *	ƒqƒbƒgƒ‚[ƒh‚ÉˆÈ~
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkFlipperChangeModeHit( OBS_OBJECT_WORK* obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//ƒ^ƒCƒv
	GME_GMK_FLIPPER_TYPE flipper_type = gmGmkFlipperCalcType( (GME_EVENT_ID)gimmick_work->ene_com.eve_rec->id );

	u16 angle = g_gm_gmk_flipper_angle_z[flipper_type]; 

	//‰ñ“]Šp“xİ’è
	if ( obj_work->user_flag ){
		angle += (u16)NNM_DEGtoA16(-GMD_GMK_FLIPPER_HIT_ANGLE);
	}
	else{
		angle += (u16)NNM_DEGtoA16(GMD_GMK_FLIPPER_HIT_ANGLE);
	}
	obj_work->user_work = (u32)angle;

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkFlipperMainHit;
	
	//Œø‰Ê‰¹
	GmSoundPlaySE("Casino2");
}

// ==========================================================================
// gmGmkFlipperChangeModeHook
/*!
 *	ƒtƒbƒNƒ‚[ƒh‚ÉˆÈ~
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkFlipperChangeModeHook( OBS_OBJECT_WORK* obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//ƒ^ƒCƒv
	GME_GMK_FLIPPER_TYPE flipper_type = gmGmkFlipperCalcType( (GME_EVENT_ID)gimmick_work->ene_com.eve_rec->id );

	u16 angle = g_gm_gmk_flipper_angle_z[flipper_type]; 

	//‰ñ“]Šp“xİ’è
	if ( obj_work->user_flag ){
		angle += (u16)NNM_DEGtoA16(-GMD_GMK_FLIPPER_HIT_ANGLE);
	}
	else{
		angle += (u16)NNM_DEGtoA16(GMD_GMK_FLIPPER_HIT_ANGLE);
	}
	obj_work->user_work = (u32)angle;

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkFlipperMainHook;
}

// ==========================================================================
// gmGmkFlipperChangeModeOpen
/*!
 *	‰ğ•úƒ‚[ƒh‚ÉˆÈ~
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkFlipperChangeModeOpen( OBS_OBJECT_WORK* obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//ƒ^ƒCƒv
	GME_GMK_FLIPPER_TYPE flipper_type = gmGmkFlipperCalcType( (GME_EVENT_ID)gimmick_work->ene_com.eve_rec->id );

	//‰ñ“]Šp“xİ’è
	obj_work->user_work = g_gm_gmk_flipper_angle_z[flipper_type];

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = NULL;

	//‰ñ“]
	obj_work->dir.z = 0;

	//‹éŒ`İ’è
	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppDef = NULL;
}

// ==========================================================================
// gmGmkFlipperMainWait
/*!
 *	‘Ò‹@ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkFlipperMainWait( OBS_OBJECT_WORK *obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//‰ñ“]XV
	gmGmkFlipperUpdateAngle( obj_work );

	//“Áêƒ^ƒCƒv”»’è
	GME_GMK_FLIPPER_TYPE flipper_type = gmGmkFlipperCalcType( (GME_EVENT_ID)gimmick_work->ene_com.eve_rec->id );
	if ( flipper_type == GMD_GMK_FLIPPER_TYPE_LR ){
		//ƒXƒRƒA‚ªã‰ñ‚Á‚½‚Æ‚«
		if ( gmGmkFlipperCheckScore(obj_work) ){
			gmGmkFlipperChangeModeOpen(obj_work);
			return;
		}
	}
}

// ==========================================================================
// gmGmkFlipperMainReady
/*!
 *	€”õ‚n‚jƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkFlipperMainReady( OBS_OBJECT_WORK *obj_work )
{

	//ƒMƒ~ƒbƒNƒ[ƒN
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* target_obj_work = gimmick_work->ene_com.target_obj;
	amAssert( target_obj_work );

	//‰ñ“]XV
	gmGmkFlipperUpdateAngle( obj_work );

	//ƒvƒŒƒCƒ„‚Ìê‡AƒL[ƒRƒ“ƒgƒ[ƒ‹ŠÄ‹BƒvƒŒƒCƒ„ˆÈŠO‚Ìê‡A‚»‚Ìê‚ÅƒqƒbƒgB
	BOOL flag_player = TRUE;
	if ( flag_player ){

		GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

		//ƒRƒ“ƒgƒ[ƒ‹‚©‚çŠO‚ê‚½
		if ( !gmGmkFlipperCheckControlPlayer() ){
			//ƒ‚[ƒh•ÏX
			gmGmkFlipperChangeModeWait( obj_work );
			return;
		}

		//ƒL[ŠÄ‹
		if ( !gmGmkFlipperCheckKeyHit(obj_work, ply_work) ){
			return;
		}

		//ƒtƒbƒNƒ‚[ƒhŠÄ‹
		if ( gmGmkFlipperCheckHook(obj_work) ){
			target_obj_work->spd.x = 0;
			target_obj_work->spd.y = 0;

			//ƒ‚[ƒh•ÏX
			gmGmkFlipperChangeModeHook( obj_work );
			return;
		}

		fx32 speed_x = GMD_GMK_FLIPPER_WAIT_SPEED_U_X;
		fx32 speed_y = GMD_GMK_FLIPPER_WAIT_SPEED_U_Y;	

		GME_GMK_FLIPPER_TYPE flipper_type = gmGmkFlipperCalcType( (GME_EVENT_ID)gimmick_work->ene_com.eve_rec->id );
		if ( flipper_type == GMD_GMK_FLIPPER_TYPE_UR ){
			speed_x = -speed_x;
		}		
		//À•W‚É‚æ‚é•â³
		speed_x += (target_obj_work->pos.x - obj_work->pos.x) >> 2;			
		fx32 addjust_y = (GMD_GMK_FLIPPER_WIDTH*FX32_ONE - MTM_MATH_ABS(target_obj_work->pos.x - obj_work->pos.x))/10;
		if ( addjust_y > 0 ){
			speed_y += 	addjust_y;
		}

		//”z’uƒc[ƒ‹İ’è‚É‚æ‚é•â³
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

		//ƒz[ƒ~ƒ“ƒO‰ñ•œ‚µ‚È‚¢ƒtƒ‰ƒO
		BOOL flag_no_recover_homing = FALSE;
		if ( gimmick_work->ene_com.eve_rec->flag & GMD_GMK_FLIPPER_FLAG_NO_RECOVER_HOMING ){
			flag_no_recover_homing = TRUE;
		}

		//no spddown timerƒtƒ‰ƒO
		s32 no_spddown_timer = GMD_GMK_FLIPPER_NO_SPDDOWN_TIMER_LR_DEFAULT;
		if ( gimmick_work->ene_com.eve_rec->flag & GMD_GMK_FLIPPER_FLAG_NO_SPDDOWN_TIMER ){
			no_spddown_timer = GMD_GMK_FLIPPER_NO_SPDDOWN_TIMER_LR_SPECIAL;
		}

		// UŒ‚İ’è
		GmPlayerSetAtk(ply_work);

		//ƒvƒŒƒCƒ„ƒV[ƒPƒ“ƒX
		GmPlySeqInitPinballAir( 
				ply_work, 
				speed_x, 
				speed_y, 
				GMD_GMK_FLIPPER_NO_MOVE_TIME,
				flag_no_recover_homing,
				no_spddown_timer);

		// ƒGƒtƒFƒNƒgˆ—
		GMS_EFFECT_3DES_WORK* effect_work = GmEfctCmnEsCreate(
				obj_work,
				GME_EFCT_CMN_IDX_BUMPER );
		amAssert( effect_work );
		effect_work->efct_com.obj_work.pos.x = target_obj_work->pos.x;
		effect_work->efct_com.obj_work.pos.y = target_obj_work->pos.y;
		effect_work->efct_com.obj_work.pos.z = GMD_OBJ_GIMMICK_POS_Z_FRONT;
		effect_work->efct_com.obj_work.dir.z = (u16)(nnArcTan2( FX_FX32_TO_F32(speed_y), FX_FX32_TO_F32(speed_x) ) - 0x4000);
	}
	
	//ƒ‚[ƒh•ÏX
	gmGmkFlipperChangeModeHit( obj_work );

}

// ==========================================================================
// gmGmkFlipperMainHit
/*!
 *	ƒqƒbƒgƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkFlipperMainHit( OBS_OBJECT_WORK *obj_work )
{
	//‘Ò‹@‚Ö
	if ( gmGmkFlipperUpdateAngle(obj_work) ){
		gmGmkFlipperChangeModeWait( obj_work );
	}
}

// ==========================================================================
// gmGmkFlipperMainHook
/*!
 *	ƒtƒbƒNƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkFlipperMainHook( OBS_OBJECT_WORK *obj_work )
{
	//ƒMƒ~ƒbƒNƒ[ƒN
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* target_obj_work = gimmick_work->ene_com.target_obj;
	amAssert( target_obj_work );

	//‰ñ“]XV
	gmGmkFlipperUpdateAngle(obj_work);

	//ƒvƒŒƒCƒ„‚Ìê‡AƒL[ƒRƒ“ƒgƒ[ƒ‹ŠÄ‹BƒvƒŒƒCƒ„ˆÈŠO‚Ìê‡A‚»‚Ìê‚ÅƒqƒbƒgB
	BOOL flag_player = TRUE;
	if ( flag_player ){

		//ƒRƒ“ƒgƒ[ƒ‹‚©‚çŠO‚ê‚½
		if ( !gmGmkFlipperCheckControlPlayer() ){
			//ƒ‚[ƒh•ÏX
			gmGmkFlipperChangeModeWait( obj_work );
			return;
		}

		//ƒXƒs[ƒhİ’è	
		GME_GMK_FLIPPER_TYPE flipper_type = gmGmkFlipperCalcType( (GME_EVENT_ID)gimmick_work->ene_com.eve_rec->id );
		gmGmkFlipperSetRideSpeed( target_obj_work, obj_work, flipper_type );

		//ƒL[‚ª—£‚³‚ê‚½‚ç“]‚ª‚èn‚ß‚é
		GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
		if ( !(player_work->key_on & PAD_BUTTON_JUMP) ){
			//ƒ‚[ƒh•ÏX
			gmGmkFlipperChangeModeReady( obj_work );
			return;
		}

		//ƒL[‚ª‰Ÿ‚³‚ê‚½‚Ü‚ÜAw’èˆÊ’u‚Ü‚Å“’B‚µ‚½‚ç~‚Ü‚é
		fx32 dir_target_x = target_obj_work->pos.x - obj_work->pos.x;
		if ( flipper_type == GMD_GMK_FLIPPER_TYPE_UR ){
			dir_target_x = -dir_target_x;
		}
		if ( dir_target_x > 0 ){
			target_obj_work->spd.x = 0;
			target_obj_work->spd.y = 0;
		}
	}

}

// ==========================================================================
// gmGmkFlipperCheckKeyHit
/*!
 *	ƒqƒbƒgƒRƒ“ƒgƒ[ƒ‹‚ªs‚í‚ê‚½‚©”»’è
 *
 *	@param gimmick_obj_work	[in] ƒMƒ~ƒbƒNƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param player_work		[in] ƒvƒŒƒCƒ„ƒ[ƒN
 *
 *	@return TRUEFs‚í‚ê‚½ FALSEFs‚í‚ê‚Ä‚¢‚È‚¢
 */
// ==========================================================================
BOOL gmGmkFlipperCheckKeyHit( 
							 OBS_OBJECT_WORK* gimmick_obj_work, 
							 GMS_PLAYER_WORK* player_work )
{
	UNREFERENCED_PARAMETER(gimmick_obj_work);

	//ƒWƒƒƒ“ƒvƒ{ƒ^ƒ“
	if ( GmPlayerKeyCheckJumpKeyPush(player_work) ){
		return TRUE;
	}

	return FALSE;
}

// ==========================================================================
// gmGmkFlipperCheckControlPlayer
/*!
 *	ƒvƒŒƒCƒ„‚ª§Œä‰º‚É‚ ‚é‚©”»’è
 *
 *	@return TRUEF§Œä‰º‚É‚ ‚é FALSEF§Œä‰º‚ğ—£‚ê‚½
 */
// ==========================================================================
BOOL gmGmkFlipperCheckControlPlayer( void )
{
	GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	if ( ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_FLIPPER ){
		return FALSE;
	}
	return TRUE;
}

// ==========================================================================
// gmGmkFlipperCheckScore
/*!
 *	ƒXƒRƒA”»’è
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return TRUEF–Ú•W‚ğ’´‚¦‚½ FALSEF–Ú•W‚É’B‚µ‚Ä‚¢‚È‚¢
 */
// ==========================================================================
BOOL gmGmkFlipperCheckScore( OBS_OBJECT_WORK* obj_work )
{
	//ƒMƒ~ƒbƒNƒ[ƒN
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	u32 open_score = (u32)(gimmick_work->ene_com.eve_rec->width * 1000);

	//ƒXƒRƒA‚ªİ’è‚³‚ê‚Ä‚¢‚È‚¢‰ğ•ú‚³‚ê‚é‚±‚Æ‚Í‚È‚¢
	if ( open_score == 0 ){
		return FALSE;
	}

	//ƒXƒRƒA‚ğ’´‚¦‚Ä‚¢‚È‚¢
	u32 score = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->score;
	if ( open_score > score ){
		return FALSE;
	}
	return TRUE;
}

// ==========================================================================
// gmGmkFlipperCheckLeft
/*!
 *	ƒMƒ~ƒbƒN@ƒtƒŠƒbƒp[@’¼ü‚Ì¶”»’èi2ŸŒ³‚Ì‚İj
 *
 *	@param line_start	[in] ’¼ü‚ÌŠJnˆÊ’u
 *	@param line_end		in] ’¼ü‚ÌI—¹ˆÊ’u
 *	@param point		[in] –Ú•W“_
 *
 *	@return TRUEF¶orüã FALSEF‰E
 *	
 */
// ==========================================================================
BOOL gmGmkFlipperCheckLeft( const VecFx32* line_start, const VecFx32* line_end, const VecFx32* point )
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
// gmGmkFlipperCheckRect
/*!
 *	ƒMƒ~ƒbƒN@ƒtƒŠƒbƒp[@ƒqƒbƒg”»’è
 *
 *	@param gimmick_obj_work	[in] ƒMƒ~ƒbƒN‚ÌÀ•W
 *	@param target_obj_work	[in] ƒ^[ƒQƒbƒg‚Ì
 *	@param type				[in] ƒoƒ“ƒp[ƒ^ƒCƒv
 */
// ==========================================================================
BOOL gmGmkFlipperCheckRect( 
						 const VecFx32* gimmick_pos,
						 const VecFx32* target_pos,
						 GME_GMK_FLIPPER_TYPE type)
{
	switch ( type ){
	case GMD_GMK_FLIPPER_TYPE_UL:
		{
			//ŒXÎƒ`ƒFƒbƒN
			VecFx32 point_top = *gimmick_pos;
			point_top.y += FX_F32_TO_FX32(g_gmk_flipper_rect[type][MTD_TOP] - GMD_GMK_FLIPPER_RECT_OFFSET_PLAYER_Y);

			VecFx32 point_bottom = *gimmick_pos;
			point_bottom.x += FX_F32_TO_FX32(g_gmk_flipper_rect[type][MTD_RIGHT]);
			point_bottom.y += FX_F32_TO_FX32(g_gmk_flipper_rect[type][MTD_BOTTOM] - GMD_GMK_FLIPPER_RECT_OFFSET_PLAYER_Y);

			if ( gmGmkFlipperCheckLeft(&point_top, &point_bottom, target_pos) ){
				return FALSE;
			}			
		}
		break;
	case GMD_GMK_FLIPPER_TYPE_UR:
		{
			//ŒXÎƒ`ƒFƒbƒN
			VecFx32 point_top = *gimmick_pos;
			point_top.y += FX_F32_TO_FX32(g_gmk_flipper_rect[type][MTD_TOP] - GMD_GMK_FLIPPER_RECT_OFFSET_PLAYER_Y);

			VecFx32 point_bottom = *gimmick_pos;
			point_bottom.x += FX_F32_TO_FX32(g_gmk_flipper_rect[type][MTD_LEFT]);
			point_bottom.y += FX_F32_TO_FX32(g_gmk_flipper_rect[type][MTD_BOTTOM] - GMD_GMK_FLIPPER_RECT_OFFSET_PLAYER_Y);

			if ( gmGmkFlipperCheckLeft(&point_bottom, &point_top, target_pos) ){
				return FALSE;
			}
		}
		break;
	default:
		break;
	}

	return TRUE;
}

// ==========================================================================
// gmGmkFlipperCheckHook
/*!
 *	ƒtƒbƒNƒ‚[ƒh‚ÉˆÚs‚·‚é‚©‚Ç‚¤‚©
 *
 *	@return TRUEFˆÚs‚·‚é FALSEFˆÚs‚µ‚È‚¢
 */
// ==========================================================================
BOOL gmGmkFlipperCheckHook( OBS_OBJECT_WORK* obj_work )
{
	//ƒMƒ~ƒbƒNƒ[ƒN
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );
	OBS_OBJECT_WORK* target_obj_work = gimmick_work->ene_com.target_obj;
	amAssert( target_obj_work );

	//Œü‚«”»’è
	fx32 dir_target_x = target_obj_work->pos.x - obj_work->pos.x;
	fx32 offset_x = GMD_GMK_FLIPPER_FOOK_MARGIN_X;
	GME_GMK_FLIPPER_TYPE flipper_type = gmGmkFlipperCalcType( (GME_EVENT_ID)gimmick_work->ene_com.eve_rec->id );
	if ( flipper_type == GMD_GMK_FLIPPER_TYPE_UR ){
		dir_target_x = -dir_target_x;
		offset_x = -offset_x;
	}	

	if ( dir_target_x > GMD_GMK_FLIPPER_FOOK_MARGIN_X ){
		return FALSE;
	}

	//À•W’²®
	if ( dir_target_x > 0 ){
		GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)target_obj_work;

		target_obj_work->pos.x = obj_work->pos.x;
		fx32 offset_y = gmGmkFlipperCalcRideOffsetY(
				obj_work->pos.x + offset_x,
				obj_work,
				flipper_type ) ; 
		
		if ( player_work->player_flag & GMD_PLF_PINBALL_SONIC ){
			offset_y += GMD_GMK_FLIPPER_OFFSET_PLAYER_Y_PINBALL;
		}
		else{
			offset_y += GMD_GMK_FLIPPER_OFFSET_PLAYER_Y_NORMAL;
		}
		target_obj_work->pos.y = obj_work->pos.y + offset_y;
	}
	return TRUE;
}

// ==========================================================================
// gmGmkFlipperSetRideSpeed
/*!
 *	ƒtƒŠƒbƒp[‚Ìã‚ğ“]‚ª‚éƒXƒs[ƒh‚ğİ’è
 *
 *	@param target_obj_work	[io] ƒ^[ƒQƒbƒg‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param gimmick_obj_work	[in] ƒMƒ~ƒbƒN‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param flipper_type		[in] ƒMƒ~ƒbƒNƒ^ƒCƒv
 */
// ==========================================================================
void gmGmkFlipperSetRideSpeed(
							  OBS_OBJECT_WORK* target_obj_work,
							  const OBS_OBJECT_WORK* gimmick_obj_work,
							  GME_GMK_FLIPPER_TYPE flipper_type )
{
	amAssert( target_obj_work );
	amAssert( gimmick_obj_work );
	UNREFERENCED_PARAMETER(gimmick_obj_work);

	//ƒXƒs[ƒhİ’è	
	fx32 speed_x = FX_F32_TO_FX32( GMD_GMK_FLIPPER_RIDE_SPEED_X );
	fx32 speed_y = FX_F32_TO_FX32( GMD_GMK_FLIPPER_RIDE_SPEED_Y );
	if ( flipper_type == GMD_GMK_FLIPPER_TYPE_UR ){
		speed_x = -speed_x;
	}
	target_obj_work->spd.x = speed_x;
	target_obj_work->spd.y = speed_y;
#if _IPHONE
	target_obj_work->spd.x = FX_Div(target_obj_work->spd.x, FX32_ONE*3);
	target_obj_work->spd.y = FX_Div(target_obj_work->spd.y, FX32_ONE*3);
#endif //_IPHONE
}


// ==========================================================================
// gmGmkFlipperCalcRideOffsetY
/*!
 *	ƒtƒŠƒbƒp[‚Ìã‚ğ“]‚ª‚éÀ•W‚x‚ğZo
 *
 *	@param x				[in] XÀ•W
 *	@param gimmick_obj_work	[in] ƒMƒ~ƒbƒN‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param flipper_type		[in] ƒMƒ~ƒbƒNƒ^ƒCƒv
 *
 *	@return ƒtƒŠƒbƒp[‚Ìã‚ğ“]‚ª‚éÀ•W‚x
 */
// ==========================================================================
fx32 gmGmkFlipperCalcRideOffsetY(
							  fx32 x,
							  const OBS_OBJECT_WORK* gimmick_obj_work,
							  GME_GMK_FLIPPER_TYPE flipper_type )
{
	float width = (float)(g_gmk_flipper_rect[flipper_type][MTD_RIGHT] - g_gmk_flipper_rect[flipper_type][MTD_LEFT]);
	if ( flipper_type == GMD_GMK_FLIPPER_TYPE_UR ){
		width = -width;
	}
	float height = (float)(g_gmk_flipper_rect[flipper_type][MTD_BOTTOM] - g_gmk_flipper_rect[flipper_type][MTD_TOP] + GMD_GMK_FLIPPER_CALC_RIDE_OFFSET);
	fx32 distance_y = FX_Mul( (fx32)(height/width*FX32_ONE) , x - gimmick_obj_work->pos.x);

	return distance_y;
}

// ==========================================================================
// gmGmkFlipperUpdateAngle
/*!
 *	‰ñ“]XV
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
BOOL gmGmkFlipperUpdateAngle( OBS_OBJECT_WORK *obj_work )
{
	++obj_work->user_timer;
	
	//‰ñ“]
	u16 dest_angle = (u16)obj_work->user_work;
	u16 add_angle = (u16)((dest_angle - obj_work->dir.z)/GMD_GMK_FLIPPER_HIT_FRAME);

	obj_work->dir.z += add_angle;
	if ( obj_work->user_timer < GMD_GMK_FLIPPER_HIT_FRAME ){
		return FALSE;
	}
	obj_work->dir.z = dest_angle;
	obj_work->user_timer = 0;

	return TRUE;
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
