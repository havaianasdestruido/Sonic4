// ==========================================================================
/*!
  @file gmGmkDrainTank.cpp
  @brief ƒMƒ~ƒbƒN ”r‰t‘•’u

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkDrainTank.cpp 2 2011-04-11 05:21:26Z thamada $
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
 *
 * ƒvƒŒƒCƒ„‚Ì§Œä‚È‚ÇA‘•’u‚ÌƒƒCƒ“ˆ—‚ÍoŒû‚ª’S“–‚µ‚Ä‚¢‚é
 * “üŒû‚Í”à‚ª•Â‚Ü‚éˆÈŠO‚ÍA‰½‚às‚Á‚Ä‚¢‚È‚¢
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmPlySeq.h"
#include "gmPlySeqGmk.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"
#include "gmWaterSurface.h"
#include "gmCamera.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectZone.h"
#include "gmRing.h"
#include "gmSound.h"
#include "gmPadVib.h"

#include "gmGmkDrainTank.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "common/model/gmk_d_tank_mdl.hmb"
#include "common/model/gmk_d_tank_mat.hmb"


//----- Definitions ---------------------------------------------------------

#define GMD_GMK_DRAIN_TANK_OFFSET_Y		( -8*FX32_ONE )
#define GMD_GMK_DRAIN_TANK_HIT_LENGTH	( 16*FX32_ONE )
#define GMD_GMK_DRAIN_TANK_SPLASH_OFFSET_Y		( 16*FX32_ONE )

//ƒ^ƒCƒv
enum GME_GMK_DRAIN_TANK_TYPE{
	GMD_GMK_DRAIN_TANK_TYPE_IN = 0,	//“üŒû
	GMD_GMK_DRAIN_TANK_TYPE_OUT,	//oŒû
	GMD_GMK_DRAIN_TANK_TYPE_SPLASH,	//•¬o‚·…

	GMD_GMK_DRAIN_TANK_TYPE_NUM
};

#define GMD_GMK_DRAIN_TANK_OFFSET_WATER_LEVEL		(48*FX32_ONE)	//…–Ê‚Ì‚‚³ioŒû‚©‚çj
#define GMD_GMK_DRAIN_TANK_START_WATER_LEVEL		(48*FX32_ONE)	//ƒvƒŒƒCƒ„ƒV[ƒPƒ“ƒX‚ğ•ÏX‚·‚é[“x
#define GMD_GMK_DRAIN_TANK_KEY_FRAME				(0)			//‰ñ“]‚ğn‚ß‚éƒtƒŒ[ƒ€”
#define GMD_GMK_DRAIN_TANK_KEY_MARGIN				(0x2000)		//ƒL[ŒXÎƒ}[ƒWƒ“
#define GMD_GMK_DRAIN_TANK_PLAYER_SPEED_MAX_X		(0x00000700)	//oŒû‚Éˆø‚Á’£‚ç‚ê‚éƒvƒŒƒCƒ„‚ÌƒXƒs[ƒh
#define GMD_GMK_DRAIN_TANK_PLAYER_SPEED_MAX_Y		(0x00000700)	//oŒû‚Éˆø‚Á’£‚ç‚ê‚éƒvƒŒƒCƒ„‚ÌƒXƒs[ƒh
#define GMD_GMK_DRAIN_TANK_PLAYER_SPEED_FLOAT_MAX	(0x00000800)	//•‚‚¢‚Ä‚¢‚éÛ‚ÌˆÚ“®—Ê
#define GMD_GMK_DRAIN_TANK_OFFSET_FLOAT				(32*FX32_ONE)	//•‚‚­‹——£
#define GMD_GMK_DRAIN_TANK_PLAYER_SPEED_OUT			(0x00010000)	//•¬o‚·‚Æ‚«‚ÌƒXƒs[ƒh
#define GMD_GMK_DRAIN_TANK_PLAYER_SPEED_DOWN_X		(0x00010000)	//—‰º‚Ì‚Æ‚«‚ÌƒXƒs[ƒh
#define GMD_GMK_DRAIN_TANK_PLAYER_SPEED_ADD_DOWN_X	(-0x00000380)	//—‰º‚Ì‚Æ‚«‚ÌƒXƒs[ƒh
#define GMD_GMK_DRAIN_TANK_BITSHIFT_CAMERA					(5)	//ƒL[‚ğƒJƒƒ‰‚ÌŒXÎ‚ÉŠ·‚¦‚éÛ‚ÌƒrƒbƒgƒVƒtƒg
#define GMD_GMK_DRAIN_TANK_BITSHIFT_SPEED_OUT				(3)	//ƒL[‚ğoŒû‚ÌˆÚ“®‘¬“x‚ÉŠ·‚¦‚éÛ‚ÌƒrƒbƒgƒVƒtƒg
#define GMD_GMK_DRAIN_TANK_BITSHIFT_SPEED_PLAYER			(2)	//ƒL[‚ğƒvƒŒƒCƒ„‚ÌˆÚ“®‘¬“x‚ÉŠ·‚¦‚éÛ‚ÌƒrƒbƒgƒVƒtƒg
#define GMD_GMK_DRAIN_TANK_BITSHIFT_WATER_LEVEL_DOWN_SPEED	(4)	//ƒL[‚ğ…–Ê‚Ì‰º~ƒXƒs[ƒh‚ÉŠ·‚¦‚éÛ‚ÌƒrƒbƒgƒVƒtƒg
#define GMD_GMK_DRAIN_TANK_WATER_LEVEL_DOWN_SPEED_SPLASH	(0.001f)	//”ro‚Ì…–ÊˆÚ“®—Ê

#define GMD_GMK_DRAIN_TANK_WATER_CLIP_B				(64)	//…–ÊƒNƒŠƒbƒv—pi’êj
#define GMD_GMK_DRAIN_TANK_WATER_CLIP_L				(152)	//…–ÊƒNƒŠƒbƒv—pi¶j
#define GMD_GMK_DRAIN_TANK_WATER_CLIP_R				(96)	//…–ÊƒNƒŠƒbƒv—pi‰Ej
#define GMD_GMK_DRAIN_TANK_WATER_CLIP_OUT_WIDTH		(64)	//…–ÊƒNƒŠƒbƒv—pioŒû•j
#define GMD_GMK_DRAIN_TANK_WATER_CLIP_OUT_HEIGHT	(96)	//…–ÊƒNƒŠƒbƒv—pioŒû‚‚³j

#define GMD_GMK_DRAIN_TANK_MAP_SIZE	(64)					//ƒ}ƒbƒvƒp[ƒcƒTƒCƒY

#define GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_OFFSET_Y		(-32*FX32_ONE)	//•¬o‚·…ƒGƒtƒFƒNƒg‚ÌƒIƒtƒZƒbƒg
#define GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_OFFSET_Z		(32*FX32_ONE)	//•¬o‚·…ƒGƒtƒFƒNƒg‚ÌƒIƒtƒZƒbƒg
#define GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_SPEED_X		(12*FX32_ONE)	//•¬o‚·…ƒGƒtƒFƒNƒg‚ÌƒXƒs[ƒh
#define GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_SPEED_Y		(3*FX32_ONE)	//•¬o‚·…ƒGƒtƒFƒNƒg‚ÌƒXƒs[ƒh
#define GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_SPEED_ADD_X	(-0x00000560)	//•¬o‚·…ƒGƒtƒFƒNƒg‚ÌƒXƒs[ƒh
#define GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_SPEED_ADD_Y	(-0x00000000)	//•¬o‚·…ƒGƒtƒFƒNƒg‚ÌƒXƒs[ƒh
#define GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_SPEED_ROT_Z	(0x0180)		//•¬o‚·…ƒGƒtƒFƒNƒg‚ÌƒXƒs[ƒh

#define GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_2_OFFSET_Y		(-16*FX32_ONE)	//•¬o‚·…‚ÌªŒ³ƒGƒtƒFƒNƒg‚ÌƒIƒtƒZƒbƒg
#define GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_2_OFFSET_Z		(32*FX32_ONE)	//•¬o‚·…‚ÌªŒ³ƒGƒtƒFƒNƒg‚ÌƒIƒtƒZƒbƒg

#define GMD_GMK_DRAIN_TANK_OFFSET_OUT_TO_CAMERA			(180*FX32_ONE)	//oŒû‚©‚çƒJƒƒ‰‚Ì‹——£

#define GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_X_NORMAL	(450*FX32_ONE)	//ƒ^ƒXƒN‚ğÁ‚·‹——£i’Êíj
#define GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_Y_NORMAL	(300*FX32_ONE)	//ƒ^ƒXƒN‚ğÁ‚·‹——£i’Êíj

#define GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_X_SPLASH	(400*FX32_ONE)	//ƒ^ƒXƒN‚ğÁ‚·‹——£i•¬o‚·j
#define GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_Y_SPLASH	(300*FX32_ONE)	//ƒ^ƒXƒN‚ğÁ‚·‹——£i•¬o‚·j

#define GMD_GMK_DRAIN_TANK_ROLL_ANGLE_MAX				(NNM_DEGtoA32(30))		//ƒJƒƒ‰‚Ì‰ñ“]—ÊÅ‘å
#define GMD_GMK_DRAIN_TANK_ROLL_ANGLE_MIN				(NNM_DEGtoA32(1))		//ƒJƒƒ‰‚Ì‰ñ“]—ÊÅ¬
#define GMD_GMK_DRAIN_TANK_ROLL_ANGLE_SPEED				(NNM_DEGtoA32(0.2f))	//ƒJƒƒ‰‚Ì‰ñ“]—Ê


#define GMD_GMK_DRAIN_TANK_DRAW_WATER_WIDTH				(OBD_LCD_X + 256)	//…–Ê•`‰æ”ÍˆÍ
#define GMD_GMK_DRAIN_TANK_DRAW_WATER_HEIGHT			(OBD_LCD_Y + 256)	//…–Ê•`‰æ”ÍˆÍ

#define GMD_GMK_DRAIN_TANK_ROLL_SE_INTERVAL				(20)	//‘•’u‰ñ“]Œø‰Ê‰¹‚ğ–Â‚ç‚·ŠÔŠu

#define GMD_GMK_DRAINTANK_SPLASH_ALPHA_TIME				(60)	//”r…ƒ‚ƒfƒ‹‚ğ“§–¾‚É‚·‚éŠÔ


//oŒûƒ[ƒN
typedef struct tag_GMS_GMK_DRAIN_TANK_OUT_WORK{
	GMS_ENEMY_3D_WORK enemy_work;			//ƒGƒlƒ~[ƒ[ƒN

	BOOL flag_dir_left;		//oŒû‚ª¶‚É‚ ‚éƒtƒ‰ƒO

	fx32 base_pos_x;		//oŒûŠî€À•WX
	fx32 base_pos_y;		//oŒûŠî€À•WY
	fx32 player_offset_x;	//oŒûŠî€‚©‚ç‚ÌƒvƒŒƒCƒ„À•WƒIƒtƒZƒbƒgX
	fx32 player_offset_y;	//oŒûŠî€‚©‚ç‚ÌƒvƒŒƒCƒ„À•WƒIƒtƒZƒbƒgY

	Angle32 camera_roll;	//ƒJƒƒ‰‚Ì‰ñ“]
	s32 counter_roll_key;	//‰ñ“]ƒL[‚ğ‰Ÿ‚µ‘±‚¯‚Ä‚¢‚éƒtƒŒ[ƒ€”
}GMS_GMK_DRAIN_TANK_OUT_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static GMS_ENEMY_3D_WORK* gmGmkDrainTankLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_DRAIN_TANK_TYPE type);
static GMS_ENEMY_3D_WORK* gmGmkDrainTankLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y,  
									GME_GMK_DRAIN_TANK_TYPE type,
									u32 model_id );

static u16 gmGmkDrainTankGameSystemGetWaterLevel( void );

static void gmGmkDrainTankDrawFuncIn( OBS_OBJECT_WORK* obj_work );
static void gmGmkDrainTankDrawFuncOut( OBS_OBJECT_WORK* obj_work );
static void gmGmkDrainTankTcbDestOut( MTS_TASK_TCB *tcb );


// ==========================================================================
//“üŒû
// ==========================================================================
static BOOL gmGmkDrainTankInCheckDeleteTask( const OBS_OBJECT_WORK* obj_work, fx32 cmp_x, fx32 cmp_y );
static void gmGmkDrainTankInInit( OBS_OBJECT_WORK* obj_work );
static void gmGmkDrainTankInRequestDeleteTask( OBS_OBJECT_WORK* obj_work );
static void gmGmkDrainTankInMainReady( OBS_OBJECT_WORK *obj_work );
static void gmGmkDrainTankInMainWait( OBS_OBJECT_WORK *obj_work );

// ==========================================================================
//oŒû
// ==========================================================================
static BOOL gmGmkDrainTankOutCheckDeleteTask( const OBS_OBJECT_WORK* obj_work, fx32 cmp_x, fx32 cmp_y );
static void gmGmkDrainTankOutRequestDeleteTask( OBS_OBJECT_WORK* obj_work );
static void gmGmkDrainTankOutInit( OBS_OBJECT_WORK* obj_work );
static void gmGmkDrainTankOutChangeModeReady( OBS_OBJECT_WORK* obj_work );
static void gmGmkDrainTankOutChangeModeWait( OBS_OBJECT_WORK* obj_work );
static void gmGmkDrainTankOutChangeModeDamage( OBS_OBJECT_WORK* obj_work );
static void gmGmkDrainTankOutChangeModeSplash( OBS_OBJECT_WORK* obj_work );
static void gmGmkDrainTankOutChangeModeEnd( OBS_OBJECT_WORK* obj_work );
static void gmGmkDrainTankOutMainReady( OBS_OBJECT_WORK *obj_work );
static void gmGmkDrainTankOutMainWait( OBS_OBJECT_WORK *obj_work );
static void gmGmkDrainTankOutMainDamage( OBS_OBJECT_WORK *obj_work );
static void gmGmkDrainTankOutMainSplash( OBS_OBJECT_WORK *obj_work );
static void gmGmkDrainTankOutMainEnd( OBS_OBJECT_WORK *obj_work );
static void gmGmkDrainTankOutMainWaitDelete( OBS_OBJECT_WORK *obj_work );

static void gmGmkDrainTankOutUpdateDie( OBS_OBJECT_WORK *obj_work );
static BOOL gmGmkDrainTankOutCheckDirLeft( 
								const OBS_OBJECT_WORK* gimmick_obj_work, 
								const OBS_OBJECT_WORK* player_obj_work );
static void gmGmkDrainTankOutAdjustPlayerOffsetBuoyancy( GMS_GMK_DRAIN_TANK_OUT_WORK* out_work );
static void gmGmkDrainTankOutAdjustPlayerOffsetWave( 
											 GMS_GMK_DRAIN_TANK_OUT_WORK* out_work,
											 OBS_OBJECT_WORK* player_obj_work);
static void gmGmkDrainTankOutApplyPlayerOffset( 
									   OBS_OBJECT_WORK* player_obj_work, 
									   const GMS_GMK_DRAIN_TANK_OUT_WORK* out_work );
static void gmGmkDrainTankOutUpdateCameraRoll( GMS_GMK_DRAIN_TANK_OUT_WORK* out_work, Angle32 rot_z );
static void gmGmkDrainTankOutUpdateCameraRollDamage( GMS_GMK_DRAIN_TANK_OUT_WORK* out_work );
static void gmGmkDrainTankOutUpdateCameraRollDie( GMS_GMK_DRAIN_TANK_OUT_WORK* out_work );
static void gmGmkDrainTankOutUpdateCameraOffset( 
										 GMS_PLAYER_WORK* player_work,
										 const GMS_GMK_DRAIN_TANK_OUT_WORK* out_work );
static BOOL gmGmkDrainTankOutCheckKeyDir( 
								  const OBS_OBJECT_WORK* gimmick_obj_work,
								  Angle32 camera_roll );

static void gmGmkDrainTankOutSinkRing( void );
// ==========================================================================
//•¬o‚·…
// ==========================================================================
static void gmGmkDrainTankSplashInit( OBS_OBJECT_WORK* obj_work );
static void gmGmkDrainTankSplashMainFunc( OBS_OBJECT_WORK* obj_work );
static void gmGmkDrainTankSplashEffectMain( OBS_OBJECT_WORK *obj_work );


//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//ƒ‚ƒfƒ‹ƒf[ƒ^
static OBS_ACTION3D_NN_WORK* g_gm_gmk_drain_tank_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkDrainTankBuild
/*!
 *	ƒMƒ~ƒbƒN ”r‰t‘•’u ƒf[ƒ^\’z
 */
// ==========================================================================
void GmGmkDrainTankBuild(void)
{
	g_gm_gmk_drain_tank_obj_3d_list = GmGameDBuildRegBuildModel(
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_DRAIN_TANK_MODEL ),
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_DRAIN_TANK_TEX ),
			0	//draw_flag
			);
}

// ==========================================================================
// GmGmkDrainTankFlush
/*!
 *	ƒMƒ~ƒbƒN ”r‰t‘•’u ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
void GmGmkDrainTankFlush(void)
{
	AMS_AMB_HEADER* amb_header = (AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_DRAIN_TANK_MODEL );

	GmGameDBuildRegFlushModel( g_gm_gmk_drain_tank_obj_3d_list, amb_header->file_num );
	g_gm_gmk_drain_tank_obj_3d_list = NULL;
}

// ==========================================================================
// GmGmkDrainTankInitIn
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@”r‰t‘•’ui“üŒûj
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkDrainTankInitIn( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	UNREFERENCED_PARAMETER(type);

	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkDrainTankLoadObj(
			eve_rec, 
			pos_x, 
			pos_y, 
			GMD_GMK_DRAIN_TANK_TYPE_IN, 
			IDB_GMK_D_TANK_MDL_GMK_D_TANK_A_ZNO );
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkDrainTankInInit( obj_work );
	return obj_work;
}

// ==========================================================================
// GmGmkDrainTankInitOut
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@”r‰t‘•’u¶ioŒûj
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkDrainTankInitOut( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	UNREFERENCED_PARAMETER(type);

	//‰E‘¤‚©‚ç—ˆ‚½ê‡¶¬‚µ‚È‚¢
	const OBS_OBJECT_WORK* player_obj_work = (const OBS_OBJECT_WORK*)g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	amAssert( player_obj_work );
	if ( pos_x-128*FX32_ONE < player_obj_work->pos.x ){
		return NULL;
	}


	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkDrainTankLoadObj(
			eve_rec, 
			pos_x, 
			pos_y, 
			GMD_GMK_DRAIN_TANK_TYPE_OUT, 
			IDB_GMK_D_TANK_MDL_GMK_D_TANK_B_ZNO );
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkDrainTankOutInit( obj_work );
	return obj_work;
}

// ==========================================================================
// GmGmkDrainTankSplashInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@”r‰t‘•’u¶i•¬o‚·…j
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkDrainTankSplashInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	UNREFERENCED_PARAMETER(type);

	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkDrainTankLoadObj(
			eve_rec, 
			pos_x, 
			pos_y, 
			GMD_GMK_DRAIN_TANK_TYPE_SPLASH, 
			IDB_GMK_D_TANK_MDL_GMK_D_TANK_SP_ZNO );
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkDrainTankSplashInit( obj_work );
	return obj_work;
}

//----- Local Functions -----------------------------------------------------

// ==========================================================================
// gmGmkDrainTankLoadObjNoModel
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
GMS_ENEMY_3D_WORK* gmGmkDrainTankLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_DRAIN_TANK_TYPE type)
{

	UNREFERENCED_PARAMETER(type);

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = NULL;
	if ( type == GMD_GMK_DRAIN_TANK_TYPE_OUT ){
		GMS_GMK_DRAIN_TANK_OUT_WORK* out_work = (GMS_GMK_DRAIN_TANK_OUT_WORK*)GMM_ENEMY_CREATE_WORK(
				eve_rec, 
				pos_x, 
				pos_y, 
				sizeof(GMS_GMK_DRAIN_TANK_OUT_WORK), 
				"GMK_DRAIN_TANK");
		amAssert( out_work );
		out_work->base_pos_x = pos_x + GMD_GMK_DRAIN_TANK_WATER_CLIP_OUT_WIDTH*FX32_ONE;
		out_work->base_pos_y = pos_y;
		gimmick_work = &out_work->enemy_work;
	}
	else{
		gimmick_work = (GMS_ENEMY_3D_WORK*)GMM_ENEMY_CREATE_WORK(
				eve_rec, 
				pos_x, 
				pos_y, 
				sizeof(GMS_ENEMY_3D_WORK), 
				"GMK_DRAIN_TANK");
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
// gmGmkDrainTankLoadObj
/*!
 *	ƒMƒ~ƒbƒN“Ç‚İ‚İ
 *
 *	@param eve_rec	[in] ƒCƒxƒ“ƒgƒŒƒR[ƒh
 *	@param pos_x	[in] À•WX
 *	@param pos_y	[in] À•WY
 *	@param type		[in] ƒ^ƒCƒv
 *	@param model_id	[in] ƒ‚ƒfƒ‹ID
 *
 *	@return ƒ[ƒN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkDrainTankLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_DRAIN_TANK_TYPE type,
									u32 model_id )
{

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkDrainTankLoadObjNoModel(
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
	//“Ç‚İ‚İ
	ObjObjectCopyAction3dNNModel(
		obj_work,
		&g_gm_gmk_drain_tank_obj_3d_list[model_id],
		&gimmick_work->obj_3d);

	//ƒ‚[ƒVƒ‡ƒ“
	if ( type == GMD_GMK_DRAIN_TANK_TYPE_SPLASH ){
		s32 data_motion_index = IDB_GMK_D_TANK_MAT_GMK_D_TANK_SP_ZNV;
		void* data_motion = ObjDataGet(GMD_DWORK_NO_GMK_DRAIN_TANK_MAT)->pData;
		amAssert( data_motion );
		ObjAction3dNNMaterialMotionLoad( 
				&gimmick_work->obj_3d,
				0,				//reg_file_id
				NULL,			//data_work
				NULL,			//mtn_data_path
				data_motion_index,
				data_motion
		);
	}

	return gimmick_work;
}

// ==========================================================================
// gmGmkDrainTankGameSystemGetWaterLevel
/*!
 * ƒQ[ƒ€ƒVƒXƒeƒ€‚©‚ç…–ÊƒŒƒxƒ‹‚ğæ“¾
 *
 *	@return	…–ÊƒŒƒxƒ‹
 */
// ==========================================================================
u16 gmGmkDrainTankGameSystemGetWaterLevel( void )
{
	return g_gm_main_system.water_level;
}

// ==========================================================================
// gmGmkDrainTankInInit
/*!
 *	ƒMƒ~ƒbƒN@”r‰t‘•’u@‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankInInit( OBS_OBJECT_WORK* obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//’nŒ`‚ ‚½‚è
	ObjObjectFieldRectSet( 
			obj_work, 
			-16,
			0,
			0,
			14 );
	gimmick_work->ene_com.col_work.obj_col.obj = &gimmick_work->ene_com.obj_work;
	gimmick_work->ene_com.col_work.obj_col.width = (s16)16;
	gimmick_work->ene_com.col_work.obj_col.height = (s16)32;
	gimmick_work->ene_com.col_work.obj_col.ofst_x = (s16)(-16);
	gimmick_work->ene_com.col_work.obj_col.ofst_y = (s16)(-32);

	//•`‰æˆ—
	obj_work->ppOut = gmGmkDrainTankDrawFuncIn;

	//ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmGmkDrainTankInMainReady;

	//ƒtƒ‰ƒO
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	//•`‰æƒRƒ}ƒ“ƒh
	gimmick_work->obj_3d.command_state = OBD_DRAW_CMD_STATE_POST_WATER;
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_N_FRONT+32*FX32_ONE*2;
}

// ==========================================================================
// gmGmkDrainTankOutInit
/*!
 *	ƒMƒ~ƒbƒN@”r‰t‘•’u@‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutInit( OBS_OBJECT_WORK* obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//’nŒ`‚ ‚½‚è
	ObjObjectFieldRectSet( 
			obj_work, 
			0,
			0,
			32,
			16 );

	//•`‰æˆ—
	obj_work->ppOut = gmGmkDrainTankDrawFuncOut;

	//ƒƒCƒ“ˆ—
	gmGmkDrainTankOutChangeModeReady( obj_work );

	//ƒtƒ‰ƒO
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->move_flag |= OBD_MOVE_FALL;
	obj_work->flag |= OBD_OBJECT_NOCLIP;

	//•`‰æƒRƒ}ƒ“ƒh
	gimmick_work->obj_3d.command_state = OBD_DRAW_CMD_STATE_POST_WATER;
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_N_FRONT+32*FX32_ONE*2;
	
	//…–Êİ’è
	GmWaterSurfaceRequestChangeWaterLevel( (u16)FX_FX32_TO_F32(obj_work->pos.y - GMD_GMK_DRAIN_TANK_OFFSET_WATER_LEVEL), 0, FALSE );

	//…–Ê”½Ë‚ğ–³Œø‚É
	GmWaterSurfaceSetFlagEnableRef( FALSE );

	//TCBƒfƒXƒgƒ‰ƒNƒ^İ’è
	mtTaskChangeTcbDestructor( obj_work->tcb, gmGmkDrainTankTcbDestOut );
}

// ==========================================================================
// gmGmkDrainTankSplashInit
/*!
 *	ƒMƒ~ƒbƒN@”r‰t‘•’u@‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankSplashInit( OBS_OBJECT_WORK* obj_work )
{
	//ƒtƒ‰ƒO
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->move_flag = OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;

	//ƒ‚[ƒVƒ‡ƒ“
	ObjDrawObjectActionSet3DNNMaterial( obj_work, 0 );

	//À•W
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z + 64*FX32_ONE;

	//”¼“§–¾İ’è
	obj_work->disp_flag |= OBD_DISP_DRAWSTATE;
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_ALPHA;
	obj_work->obj_3d->draw_state.alpha.alpha = 1.0f;
	obj_work->user_timer = GMD_GMK_DRAINTANK_SPLASH_ALPHA_TIME;

	//ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmGmkDrainTankSplashMainFunc;
	obj_work->ppMove = NULL;

	//•¬o‚·…ƒGƒtƒFƒNƒg
	GMS_EFFECT_3DES_WORK* effect_work_water = GmEfctZoneEsCreate(
			obj_work,
			GSD_MAIN_ZONE_TYPE_3,
			GME_EFCT_Z03_IDX_WATER );
	amAssert( effect_work_water );
	effect_work_water->efct_com.obj_work.ppFunc = gmGmkDrainTankSplashEffectMain;
	effect_work_water->efct_com.obj_work.move_flag = OBD_MOVE_NOCOL | OBD_MOVE_FALL;
	effect_work_water->efct_com.obj_work.pos.y += GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_OFFSET_Y;
	effect_work_water->efct_com.obj_work.pos.z = obj_work->pos.z + GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_OFFSET_Z;
	effect_work_water->efct_com.obj_work.spd.x = GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_SPEED_X;
	effect_work_water->efct_com.obj_work.spd.y = GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_SPEED_Y;
	effect_work_water->efct_com.obj_work.spd_add.x = GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_SPEED_ADD_X;
	effect_work_water->efct_com.obj_work.spd_add.y = GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_SPEED_ADD_Y;

	//•¬o‚·…‚ÌªŒ³ƒGƒtƒFƒNƒg
	GMS_EFFECT_3DES_WORK* effect_work_water_2 = GmEfctZoneEsCreate(
			obj_work,
			GSD_MAIN_ZONE_TYPE_3,
			GME_EFCT_Z03_IDX_WATER_02 );
	amAssert( effect_work_water_2 );
	//effect_work_water_2->efct_com.obj_work.dir.z = 0x8000;
	effect_work_water_2->efct_com.obj_work.pos.y += GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_2_OFFSET_Y;
	effect_work_water_2->efct_com.obj_work.pos.z = obj_work->pos.z + GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_2_OFFSET_Z;

}

// ==========================================================================
// gmGmkDrainTankDrawFuncIn
/*!
 *	ƒMƒ~ƒbƒN@”r‰t‘•’u@•`‰æŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankDrawFuncIn( OBS_OBJECT_WORK* obj_work )
{	
	//-------------------------------------------
	//ƒIƒuƒWƒFƒNƒg‚ğ•`‰æ
	//-------------------------------------------
	//ƒJƒƒ‰
	ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, NNE_PROJECTION_TYPE_ORTHO, OBD_DRAW_CMD_STATE_POST_WATER );
	ObjDrawActionSummary( obj_work );
}

// ==========================================================================
// gmGmkDrainTankDrawFuncOut
/*!
 *	ƒMƒ~ƒbƒN@”r‰t‘•’u@•`‰æŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankDrawFuncOut( OBS_OBJECT_WORK* obj_work )
{
	if ( !(obj_work->disp_flag & OBD_DISP_NODISP) ){
		//-------------------------------------------
		//‘•’uŠO‚Ì…‚ğÁ‚·”Â‚ğ•`‰æ
		//-------------------------------------------
		GMS_GMK_DRAIN_TANK_OUT_WORK* out_work = (GMS_GMK_DRAIN_TANK_OUT_WORK*)obj_work;

		//‰ºÀ•WioŒû‚ÌÀ•W‚©‚ç‹tZBmapƒuƒƒbƒNƒTƒCƒYj
		s32 bottom = -(s32)FX_FX32_TO_F32(out_work->base_pos_y) - GMD_GMK_DRAIN_TANK_WATER_CLIP_B;
		bottom /= GMD_GMK_DRAIN_TANK_MAP_SIZE;
		bottom = (bottom-1) * GMD_GMK_DRAIN_TANK_MAP_SIZE;

		//‰EÀ•WioŒû‚ÌÀ•W‚©‚ç‹tZBmapƒuƒƒbƒNƒTƒCƒYj
		s32 right = (s32)FX_FX32_TO_F32(out_work->base_pos_x) - GMD_GMK_DRAIN_TANK_WATER_CLIP_R;
		right /= GMD_GMK_DRAIN_TANK_MAP_SIZE;
		right = (right) * GMD_GMK_DRAIN_TANK_MAP_SIZE;

		//¶À•WioŒû‚ÌÀ•W‚©‚ç‹tZBmapƒuƒƒbƒNƒTƒCƒYj
		s32 left = right - GMD_GMK_DRAIN_TANK_WATER_CLIP_L;

		//oŒû•t‹ß‚ÌãÀ•W
		s32 top_out = bottom + GMD_GMK_DRAIN_TANK_WATER_CLIP_OUT_HEIGHT;

		//oŒû•t‹ß‚Ì¶À•W
		s32 right_out = right + GMD_GMK_DRAIN_TANK_WATER_CLIP_OUT_WIDTH;



		//ƒJƒƒ‰
		ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, NNE_PROJECTION_TYPE_ORTHO, OBD_DRAW_CMD_STATE_PRE_WATER );
		
		//oŒûƒIƒuƒWƒFƒNƒg‚æ‚è‰E
		GmWaterSurfaceDrawNoWaterField(
			FX_FX32_TO_F32(obj_work->pos.x),
			(Float)(-FX_FX32_TO_F32(obj_work->pos.y) + GMD_GMK_DRAIN_TANK_DRAW_WATER_HEIGHT),
			(Float)(FX_FX32_TO_F32(obj_work->pos.x) + GMD_GMK_DRAIN_TANK_DRAW_WATER_WIDTH),
			(Float)(-FX_FX32_TO_F32(obj_work->pos.y) - GMD_GMK_DRAIN_TANK_DRAW_WATER_HEIGHT)
			);

		//oŒû‚æ‚è‰Eã	
		GmWaterSurfaceDrawNoWaterField(
			(Float)right,
			(Float)(-FX_FX32_TO_F32(out_work->base_pos_y) + GMD_GMK_DRAIN_TANK_DRAW_WATER_HEIGHT),
			(Float)(FX_FX32_TO_F32(out_work->base_pos_x) + GMD_GMK_DRAIN_TANK_DRAW_WATER_WIDTH),
			(Float)top_out
			);

		//‘•’u‚æ‚è‰º
		GmWaterSurfaceDrawNoWaterField(
			(Float)(FX_FX32_TO_F32(out_work->base_pos_x) - GMD_GMK_DRAIN_TANK_DRAW_WATER_WIDTH),
			(Float)bottom,
			(Float)(FX_FX32_TO_F32(out_work->base_pos_x) + GMD_GMK_DRAIN_TANK_DRAW_WATER_WIDTH),
			(Float)(bottom - GMD_GMK_DRAIN_TANK_DRAW_WATER_HEIGHT)
			);

		//‘•’u‚æ‚è¶
		GmWaterSurfaceDrawNoWaterField(
			(Float)(left - GMD_GMK_DRAIN_TANK_DRAW_WATER_WIDTH),
			(Float)(-FX_FX32_TO_F32(out_work->base_pos_y) + GMD_GMK_DRAIN_TANK_DRAW_WATER_HEIGHT),
			(Float)left,
			(Float)bottom
			);

		//‘•’u‚æ‚è‰E
		GmWaterSurfaceDrawNoWaterField(
			(Float)right_out,
			(Float)top_out,
			(Float)(right_out + GMD_GMK_DRAIN_TANK_DRAW_WATER_WIDTH),
			(Float)bottom
			);
	}

	//-------------------------------------------
	//ƒIƒuƒWƒFƒNƒg‚ğ•`‰æ
	//-------------------------------------------
	//ƒJƒƒ‰
	ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, NNE_PROJECTION_TYPE_ORTHO, OBD_DRAW_CMD_STATE_POST_WATER );
	ObjDrawActionSummary( obj_work );
}

// ==========================================================================
// gmGmkDrainTankTcbDestOut
/*!
 *	ƒMƒ~ƒbƒN@”r‰t‘•’u@íœŠÖ”
 *
 *	@param tcb	[in] ‚s‚b‚a
 */
// ==========================================================================
void gmGmkDrainTankTcbDestOut( MTS_TASK_TCB *tcb )
{
	//…–Ê”½Ë‚ğ—LŒø‚É
	GmWaterSurfaceSetFlagEnableRef( TRUE );

	//…–Ê—LŒø‰»
	g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_WATER_LEVEL_EFCT_OFF;

	//ƒIƒuƒWƒFƒNƒg‰ğ•ú
	GmEnemyDefaultExit( tcb );
}

// ==========================================================================
//“üŒû
// ==========================================================================


// ==========================================================================
// gmGmkDrainTankInCheckDeleteTask
/*!
 *	ƒ^ƒXƒNíœ”»’è
 *
 *	@param obj_work		[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param cmp_x	[in] íœ‚·‚é‹——£
 *	@param cmp_y	[in] íœ‚·‚é‹——£
 *
 *	@retval	TURE:íœ‚·‚é
 *	@retval	FALSE:íœ‚µ‚È‚¢
 */
// ==========================================================================
BOOL gmGmkDrainTankInCheckDeleteTask( const OBS_OBJECT_WORK* obj_work, fx32 cmp_x, fx32 cmp_y )
{
	amAssert(obj_work );

	const GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	amAssert( player_work );
	const OBS_OBJECT_WORK* player_obj_work = &player_work->obj_work;
	amAssert( player_obj_work );

	fx32 distance_x = MTM_MATH_ABS(obj_work->pos.x - player_obj_work->pos.x);
	fx32 distance_y = MTM_MATH_ABS(obj_work->pos.y - player_obj_work->pos.y);
	if ( distance_x > cmp_x
		|| distance_y > cmp_y
	){
		return TRUE;
	}
	return FALSE;
}

// ==========================================================================
// gmGmkDrainTankInRequestDeleteTask
/*!
 *	ƒ^ƒXƒNíœ—v‹
 *
 *	@param obj_work	[in] ƒMƒ~ƒbƒN‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 */
// ==========================================================================
void gmGmkDrainTankInRequestDeleteTask( OBS_OBJECT_WORK* obj_work )
{
	amAssert(obj_work );

	//íœ
	obj_work->flag |= OBD_OBJECT_TASKCLEAR;
}

// ==========================================================================
// gmGmkDrainTankInMainReady
/*!
 *	‘Ò‹@ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankInMainReady( OBS_OBJECT_WORK *obj_work )
{
	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	//€–S
	if (player_work->player_flag & GMD_PLF_DIE){
		return;
	}

	//I—¹ˆ—
	if ( gmGmkDrainTankInCheckDeleteTask(
			obj_work,
			GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_X_NORMAL,
			GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_Y_NORMAL)
	){
		gmGmkDrainTankInRequestDeleteTask( obj_work );
		return;
	}

	const OBS_OBJECT_WORK* player_obj_work = &player_work->obj_work;
	amAssert( player_obj_work );

	//…’†‚É‚à‚®‚Á‚½‚ç•Â‚Ü‚é
	//fx32 water_level = gmGmkDrainTankGameSystemGetWaterLevel()*FX32_ONE;
	//if ( water_level + GMD_GMK_DRAIN_TANK_START_WATER_LEVEL < player_obj_work->pos.y ){	
	//’Ê‚è‰ß‚¬‚½‚ç•Â‚Ü‚é
	if ( obj_work->pos.x + 16*FX32_ONE < player_obj_work->pos.x ){
		obj_work->pos.y += 8*FX32_ONE;
		obj_work->ppFunc = gmGmkDrainTankInMainWait;
		obj_work->move_flag |= OBD_MOVE_FALL;
	}
}

// ==========================================================================
// gmGmkDrainTankInMainWait
/*!
 *	I—¹ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankInMainWait( OBS_OBJECT_WORK *obj_work )
{
	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	//€–S
	if (player_work->player_flag & GMD_PLF_DIE){
		return;
	}

	//I—¹ˆ—
	if ( gmGmkDrainTankInCheckDeleteTask(
			obj_work,
			GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_X_NORMAL,
			GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_Y_NORMAL)
	){
		gmGmkDrainTankInRequestDeleteTask( obj_work );
		return;
	}
}









// ==========================================================================
//oŒû
// ==========================================================================


// ==========================================================================
// gmGmkDrainTankInCheckDeleteTask
/*!
 *	ƒ^ƒXƒNíœ”»’è
 *
 *	@param obj_work		[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param cmp_x	[in] íœ‚·‚é‹——£
 *	@param cmp_y	[in] íœ‚·‚é‹——£
 *
 *	@retval	TURE:íœ‚·‚é
 *	@retval	FALSE:íœ‚µ‚È‚¢
 */
// ==========================================================================
BOOL gmGmkDrainTankOutCheckDeleteTask( const OBS_OBJECT_WORK* obj_work, fx32 cmp_x, fx32 cmp_y )
{
	amAssert(obj_work );

	const GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	amAssert( player_work );
	const OBS_OBJECT_WORK* player_obj_work = &player_work->obj_work;
	amAssert( player_obj_work );
	GMS_GMK_DRAIN_TANK_OUT_WORK* out_work = (GMS_GMK_DRAIN_TANK_OUT_WORK*)obj_work;

	fx32 distance_x = MTM_MATH_ABS(out_work->base_pos_x - player_obj_work->pos.x);
	fx32 distance_y = MTM_MATH_ABS(out_work->base_pos_y - player_obj_work->pos.y);
	if ( distance_x > cmp_x
		|| distance_y > cmp_y
	){
		return TRUE;
	}
	return FALSE;
}
// ==========================================================================
// gmGmkDrainTankOutRequestDeleteTask
/*!
 *	ƒ^ƒXƒNíœ—v‹
 *
 *	@param obj_work	[in] ƒMƒ~ƒbƒN‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 */
// ==========================================================================
void gmGmkDrainTankOutRequestDeleteTask( OBS_OBJECT_WORK* obj_work )
{
	amAssert(obj_work );

	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	amAssert( player_work );

	//ƒJƒƒ‰ƒIƒtƒZƒbƒg‚ğ–ß‚·
	GmPlayerCameraOffsetSet(player_work, 0, 0);
	GmCameraAllowReset();

	//…–Ê‚ğ–ß‚·
	GmWaterSurfaceRequestChangeWaterLevel( 0xFFFF, 0, FALSE );

	//íœ€”õŠ®—¹‘Ò‚¿
	obj_work->ppFunc = gmGmkDrainTankOutMainWaitDelete;
}



// ==========================================================================
// gmGmkDrainTankOutChangeModeReady
/*!
 *	€”õƒ‚[ƒh‚ÉˆÚs
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutChangeModeReady( OBS_OBJECT_WORK* obj_work )
{
	//ˆÚ“®’â~
	obj_work->spd.x = 0;

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkDrainTankOutMainReady;
}

// ==========================================================================
// gmGmkDrainTankOutChangeModeWait
/*!
 *	‘Ò‹@ƒ‚[ƒh‚ÉˆÚs
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutChangeModeWait( OBS_OBJECT_WORK* obj_work )
{
	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	OBS_OBJECT_WORK* player_obj_work = &player_work->obj_work;
	amAssert( player_obj_work );

	//ƒvƒŒƒCƒ„ƒIƒtƒZƒbƒgİ’è
	GMS_GMK_DRAIN_TANK_OUT_WORK* out_work = (GMS_GMK_DRAIN_TANK_OUT_WORK*)obj_work;
	amAssert( out_work );
	out_work->player_offset_x = player_obj_work->pos.x - out_work->base_pos_x;
	out_work->player_offset_y = player_obj_work->pos.y - out_work->base_pos_y;

	//ˆÚ“®’â~
	obj_work->spd.x = 0;

	//ƒL[‰Ÿ‚³‚ê‚Ä‚¢‚éŠÔƒŠƒZƒbƒg
	out_work->counter_roll_key = 0;

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkDrainTankOutMainWait;
}

// ==========================================================================
// gmGmkDrainTankOutChangeModeDamage
/*!
 *	ƒ_ƒ[ƒWƒ‚[ƒh‚ÉˆÚs
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutChangeModeDamage( OBS_OBJECT_WORK* obj_work )
{
	//ˆÚ“®’â~
	obj_work->spd.x = 0;

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkDrainTankOutMainDamage;
}

// ==========================================================================
// gmGmkDrainTankOutChangeModeSplash
/*!
 *	•¬o‚·ƒ‚[ƒh‚ÉˆÚs
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutChangeModeSplash( OBS_OBJECT_WORK* obj_work )
{
	amAssert( obj_work );
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	GMS_GMK_DRAIN_TANK_OUT_WORK* out_work = (GMS_GMK_DRAIN_TANK_OUT_WORK*)gimmick_work;

	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	amAssert( player_work );
	OBS_OBJECT_WORK* player_obj_work = &player_work->obj_work;

	//ƒvƒŒƒCƒ„
	if ( out_work->flag_dir_left ){
		player_obj_work->spd.x = -GMD_GMK_DRAIN_TANK_PLAYER_SPEED_OUT;
	}
	else{
		player_obj_work->spd.x = GMD_GMK_DRAIN_TANK_PLAYER_SPEED_OUT;
	}
	player_obj_work->pos.y = obj_work->pos.y;
	player_obj_work->move_flag |= OBD_MOVE_NOCOL;

	//ŠÔ‰ñ•œ
	GmPlayerBreathingSet( player_work );

	//ƒtƒ‰ƒO•ÏX
	obj_work->move_flag |= OBD_MOVE_NOCOL;
	g_gm_main_system.game_flag |= GMD_GAME_FLAG_WATER_LEVEL_EFCT_OFF;

	//•¬o‚·…ƒIƒuƒWƒFƒNƒg
	GmEventMgrLocalEventBirth(
		GMD_EVENT_ID_NOSET_DRAIN_TANK_SPLASH,
		obj_work->pos.x,
		obj_work->pos.y + GMD_GMK_DRAIN_TANK_SPLASH_OFFSET_Y,
		gimmick_work->ene_com.eve_rec->flag,
		gimmick_work->ene_com.eve_rec->left,
		gimmick_work->ene_com.eve_rec->top,
		gimmick_work->ene_com.eve_rec->width,
		gimmick_work->ene_com.eve_rec->height,
		0);

	//Œø‰Ê‰¹
	GmSoundPlaySE("Fluid2");

	//U“®
	GMM_PAD_VIB_SMALL();

	//ƒJƒƒ‰ƒIƒtƒZƒbƒg‚ğ–ß‚·
	GmPlayerCameraOffsetSet(player_work, 0, 0);
	GmCameraAllowReset();

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkDrainTankOutMainSplash;
}

// ==========================================================================
// gmGmkDrainTankOutChangeModeEnd
/*!
 *	I—¹ƒ‚[ƒh‚ÉˆÚs
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutChangeModeEnd( OBS_OBJECT_WORK* obj_work )
{
	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkDrainTankOutMainEnd;
}

// ==========================================================================
// gmGmkDrainTankOutMainReady
/*!
 *	€”õOKƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutMainReady( OBS_OBJECT_WORK *obj_work )
{
	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];	
	OBS_OBJECT_WORK* player_obj_work = &player_work->obj_work;
	amAssert( player_obj_work );

	GMS_GMK_DRAIN_TANK_OUT_WORK* out_work = (GMS_GMK_DRAIN_TANK_OUT_WORK*)obj_work;

	//€–S
	if (player_work->player_flag & GMD_PLF_DIE){
		gmGmkDrainTankOutUpdateDie( obj_work );

		//…”ò–—”ñ•\¦À•WioŒû‚ª’u‚©‚ê‚Ä‚¢‚éˆÊ’u‚©‚ç3ƒuƒƒbƒNˆÈ‘Oj
		s32 no_effect_block_x = (s32)FX_FX32_TO_F32(out_work->base_pos_x);
		no_effect_block_x /= GMD_MAP_BLOCK_SIZE;
		no_effect_block_x -= 3;
		no_effect_block_x *= FX32_ONE * GMD_MAP_BLOCK_SIZE;

		if ( player_obj_work->pos.x < no_effect_block_x ){
			g_gm_main_system.game_flag |= GMD_GAME_FLAG_WATER_LEVEL_EFCT_OFF;
		}

		//oŒû‚ÌˆÚ“®‚ğ~‚ß‚é
		obj_work->spd.x = 0;
		obj_work->spd.y = 0;
		return;
	}

	//I—¹ˆ—
	if ( gmGmkDrainTankOutCheckDeleteTask(
			obj_work,
			GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_X_NORMAL,
			GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_Y_NORMAL)
	){
		gmGmkDrainTankOutRequestDeleteTask( obj_work );
		return;
	}

	//…’†‚É‚à‚®‚Á‚½‚ç‘Ò‹@‚Ö
	fx32 water_level = gmGmkDrainTankGameSystemGetWaterLevel()*FX32_ONE;
	if ( water_level + GMD_GMK_DRAIN_TANK_START_WATER_LEVEL < player_obj_work->pos.y ){
		gmGmkDrainTankOutChangeModeWait( obj_work );

		//oŒûŒü‚«ƒtƒ‰ƒO
		out_work->flag_dir_left = gmGmkDrainTankOutCheckDirLeft( obj_work, player_obj_work);

		//ƒvƒŒƒCƒ„ƒV[ƒPƒ“ƒX•ÏX
		fx32 speed_x = player_obj_work->spd.x;
		GmPlySeqInitDrainTank(player_work);

		//ƒvƒŒƒCƒ„ˆÚ“®’l‚ğ”½‰f
		out_work->player_offset_x += speed_x*5;
	
		//ƒJƒƒ‰ƒIƒtƒZƒbƒg
		gmGmkDrainTankOutUpdateCameraOffset( player_work, out_work );
		GmCameraAllowSet(10.0f, 10.f, 10.0f);
		return ;
	}
}

// ==========================================================================
// gmGmkDrainTankOutMainWait
/*!
 *	‘Ò‹@ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutMainWait( OBS_OBJECT_WORK *obj_work )
{
	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	//€–S
	if (player_work->player_flag & GMD_PLF_DIE){
		gmGmkDrainTankOutUpdateDie( obj_work );

		//oŒû‚ÌˆÚ“®‚ğ~‚ß‚é
		obj_work->spd.x = 0;
		obj_work->spd.y = 0;
		return;
	}

	//I—¹ˆ—
	if ( gmGmkDrainTankOutCheckDeleteTask(
			obj_work,
			GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_X_NORMAL,
			GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_Y_NORMAL)
	){
		gmGmkDrainTankOutRequestDeleteTask( obj_work );
		return;
	}
	
	OBS_OBJECT_WORK* player_obj_work = &player_work->obj_work;
	amAssert( player_obj_work );

	//ƒ_ƒ[ƒWŠÄ‹
	if ( player_work->seq_state == GME_PLY_SEQ_STATE_DAMAGE ){
		gmGmkDrainTankOutChangeModeDamage( obj_work );
		return;
	}

	//’n–Ê‚©‚ç—£‚ê‚½‚çI—¹
	if ( !(obj_work->move_flag & OBD_MOVE_UNDER) ){
		gmGmkDrainTankOutChangeModeSplash( obj_work );
		return ;
	}

	GMS_GMK_DRAIN_TANK_OUT_WORK* out_work = (GMS_GMK_DRAIN_TANK_OUT_WORK*)obj_work;

	//ƒL[‘€ìæ“¾
	Angle32 rot_z = GmPlayerKeyGetGimmickRotZ( player_work );

	//ƒL[‰Ÿ‚³‚ê‚Ä‚¢‚éŠÔXV
	if (MTM_MATH_ABS(rot_z) > GMD_GMK_DRAIN_TANK_KEY_MARGIN ){
		++out_work->counter_roll_key;
	}
	else{
		out_work->counter_roll_key = 0;
	}

	//w’èŠÔˆÈãƒL[‚ª‰Ÿ‚³‚ê‚Ä‚¢‚éê‡
	if ( out_work->counter_roll_key >= GMD_GMK_DRAIN_TANK_KEY_FRAME ){
		//ƒJƒƒ‰‰ñ“]—Êİ’è
		gmGmkDrainTankOutUpdateCameraRoll( out_work, rot_z );

		//Œø‰Ê‰¹
		if ( GMD_GMK_DRAIN_TANK_ROLL_SE_INTERVAL-1 == out_work->counter_roll_key % GMD_GMK_DRAIN_TANK_ROLL_SE_INTERVAL
				&& MTM_MATH_ABS(out_work->camera_roll) < GMD_GMK_DRAIN_TANK_ROLL_ANGLE_MAX
		){
			GmSoundPlaySE("Fluid1");
		}
	}

	//oŒû•ûŒü‚ÉŒX‚¢‚Ä‚¢‚éê‡
	if ( gmGmkDrainTankOutCheckKeyDir(obj_work, out_work->camera_roll ) ){
		//oŒû‚ÌˆÚ“®
		fx32 gimmick_speed_x = out_work->camera_roll >> GMD_GMK_DRAIN_TANK_BITSHIFT_SPEED_OUT;
		obj_work->spd.x = gimmick_speed_x;

		//…–Ê‚ÌˆÚ“®
		Float water_level_speed = FX_FX32_TO_F32(out_work->camera_roll >> GMD_GMK_DRAIN_TANK_BITSHIFT_WATER_LEVEL_DOWN_SPEED);
		water_level_speed = MTM_MATH_ABS(water_level_speed);
		GmWaterSurfaceRequestAddWatarLevel( MTM_MATH_ABS(water_level_speed), 0, TRUE );
	}

	//•‚—Í‚É‚æ‚éƒvƒŒƒCƒ„ˆÚ“®
	gmGmkDrainTankOutAdjustPlayerOffsetBuoyancy( out_work );

	//”g‚É‚æ‚éƒvƒŒƒCƒ„ˆÚ“®
	gmGmkDrainTankOutAdjustPlayerOffsetWave( out_work, player_obj_work );

	//ƒvƒŒƒCƒ„ˆÚ“®—Êİ’è
	gmGmkDrainTankOutApplyPlayerOffset( player_obj_work, out_work );

	//ƒJƒƒ‰ŒXÎ”½‰f
	OBS_CAMERA* obj_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);
	obj_camera->roll = out_work->camera_roll;
	
	//ƒJƒƒ‰ƒIƒtƒZƒbƒgXV
	gmGmkDrainTankOutUpdateCameraOffset( player_work, out_work );
}

// ==========================================================================
// gmGmkDrainTankOutMainDamage
/*!
 *	ƒ_ƒ[ƒWƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutMainDamage( OBS_OBJECT_WORK *obj_work )
{
	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	//€–S
	if (player_work->player_flag & GMD_PLF_DIE){
		gmGmkDrainTankOutUpdateDie( obj_work );

		//oŒû‚ÌˆÚ“®‚ğ~‚ß‚é
		obj_work->spd.x = 0;
		obj_work->spd.y = 0;
		return;
	}

	//I—¹ˆ—
	if ( gmGmkDrainTankOutCheckDeleteTask(
			obj_work,
			GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_X_NORMAL,
			GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_Y_NORMAL)
	){
		gmGmkDrainTankOutRequestDeleteTask( obj_work );
		return;
	}

	GMS_GMK_DRAIN_TANK_OUT_WORK* out_work = (GMS_GMK_DRAIN_TANK_OUT_WORK*)obj_work;
	OBS_OBJECT_WORK* player_obj_work = &player_work->obj_work;
	amAssert( player_obj_work );

	//ƒJƒƒ‰‰ñ“]—Êİ’è
	gmGmkDrainTankOutUpdateCameraRollDamage( out_work );
		
	fx32 move_value = (GMD_GMK_DRAIN_TANK_ROLL_ANGLE_MAX - MTM_MATH_ABS(out_work->camera_roll)) * 4;

	//oŒû‚ÌˆÚ“®
	fx32 gimmick_speed_x = (fx32)(move_value >> GMD_GMK_DRAIN_TANK_BITSHIFT_SPEED_OUT);
	if ( out_work->flag_dir_left ){
		obj_work->spd.x = gimmick_speed_x;
	}
	else{
		obj_work->spd.x = -gimmick_speed_x;
	}

	//…–Ê‚ÌˆÚ“®
	Float water_level_speed = FX_FX32_TO_F32(move_value >> GMD_GMK_DRAIN_TANK_BITSHIFT_WATER_LEVEL_DOWN_SPEED);
	GmWaterSurfaceRequestAddWatarLevel( -water_level_speed, 0, TRUE );

	//ƒvƒŒƒCƒ„‚ª’n–Ê‚É‚Â‚¢‚½‚çƒ_ƒ[ƒWI—¹
	++obj_work->user_timer;
	if ( player_obj_work->move_flag & OBD_MOVE_UNDER ){
		obj_work->user_timer = 0;

		//‘Ò‹@ó‘Ô‚Ö
		gmGmkDrainTankOutChangeModeWait( obj_work );

		//ƒvƒŒƒCƒ„ƒV[ƒPƒ“ƒX•ÏX
		GmPlySeqInitDrainTank(player_work);
		return;
	}

	//ƒJƒƒ‰ŒXÎ
	OBS_CAMERA* obj_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);
	obj_camera->roll = out_work->camera_roll;
	
	//ƒJƒƒ‰ƒIƒtƒZƒbƒgXV
	gmGmkDrainTankOutUpdateCameraOffset( player_work, out_work );
	
	//ƒŠƒ“ƒO‚ğ’¾‚ß‚é
	gmGmkDrainTankOutSinkRing();
}

// ==========================================================================
// gmGmkDrainTankOutMainSplash
/*!
 *	•¬o‚·ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutMainSplash( OBS_OBJECT_WORK *obj_work )
{	
	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	//€–S
	if (player_work->player_flag & GMD_PLF_DIE){
		gmGmkDrainTankOutUpdateDie( obj_work );
		return;
	}

	//I—¹ˆ—
	if ( gmGmkDrainTankOutCheckDeleteTask(
			obj_work,
			GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_X_SPLASH,
			GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_Y_SPLASH)
	){
		gmGmkDrainTankOutRequestDeleteTask( obj_work );
		return;
	}

	GMS_GMK_DRAIN_TANK_OUT_WORK* out_work = (GMS_GMK_DRAIN_TANK_OUT_WORK*)obj_work;
	OBS_OBJECT_WORK* player_obj_work = &player_work->obj_work;
	amAssert( player_obj_work );

	//…–Ê‚ÌˆÚ“®
	GmWaterSurfaceRequestAddWatarLevel( GMD_GMK_DRAIN_TANK_WATER_LEVEL_DOWN_SPEED_SPLASH, 0, TRUE );

	//ƒJƒƒ‰ŒXÎ
	OBS_CAMERA* obj_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);
	obj_camera->roll = out_work->camera_roll;

	//ƒvƒŒƒCƒ„
	if ( out_work->base_pos_x < player_obj_work->pos.x ){

		//ƒtƒ‰ƒO
		player_obj_work->move_flag |= OBD_MOVE_FALL;
		player_obj_work->move_flag &= ~(OBD_MOVE_NOCOL);

		//ƒXƒs[ƒh
		player_obj_work->spd.x = GMD_GMK_DRAIN_TANK_PLAYER_SPEED_DOWN_X;
		player_obj_work->spd_add.x = GMD_GMK_DRAIN_TANK_PLAYER_SPEED_ADD_DOWN_X;

		//ƒJƒƒ‰ƒIƒtƒZƒbƒg‚ğ–ß‚·
		GmPlayerCameraOffsetSet(player_work, 0, 0);
		GmCameraAllowReset();

		//I—¹ƒ‚[ƒh
		gmGmkDrainTankOutChangeModeEnd( obj_work );
	}
}

// ==========================================================================
// gmGmkDrainTankOutMainEnd
/*!
 *	I—¹ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutMainEnd( OBS_OBJECT_WORK *obj_work )
{	
	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	if (player_work->player_flag & GMD_PLF_DIE){
		gmGmkDrainTankOutUpdateDie( obj_work );
		return;
	}

	//I—¹ˆ—
	if ( gmGmkDrainTankOutCheckDeleteTask(
			obj_work,
			GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_X_SPLASH,
			GMD_GMK_DRAIN_TANK_DELETE_TASK_DISTANCE_Y_SPLASH)
	){
		gmGmkDrainTankOutRequestDeleteTask( obj_work );

		//ƒvƒŒƒCƒ„ƒV[ƒPƒ“ƒX•ÏX
		amAssert( player_work );
		GmPlySeqInitDrainTankFall(player_work);
		return;
	}
}

// ==========================================================================
// gmGmkDrainTankOutMainWaitDelete
/*!
 *	íœ€”õ‘Ò‚¿ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutMainWaitDelete( OBS_OBJECT_WORK *obj_work )
{
	//ƒEƒH[ƒ^[ƒŒƒxƒ‹İ’èŠ®—¹‘Ò‚¿
	u16 water_level = gmGmkDrainTankGameSystemGetWaterLevel();
	if ( water_level != 0xFFFF ){
		return;
	}

	//íœ
	obj_work->flag |= OBD_OBJECT_TASKCLEAR;
}





// ==========================================================================
// gmGmkDrainTankOutUpdateDie
/*!
 *	€–Sˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutUpdateDie( OBS_OBJECT_WORK *obj_work )
{
	GMS_GMK_DRAIN_TANK_OUT_WORK* out_work = (GMS_GMK_DRAIN_TANK_OUT_WORK*)obj_work;

	//ƒJƒƒ‰XV
	gmGmkDrainTankOutUpdateCameraRollDie( out_work );

	//ƒJƒƒ‰ŒXÎ
	OBS_CAMERA* obj_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);
	obj_camera->roll = out_work->camera_roll;
}

// ==========================================================================
// gmGmkDrainTankOutCheckDirLeft
/*!
 *	ƒMƒ~ƒbƒN‚ÌˆÚ“®•ûŒü‚ª¶‚©”»’è
 *
 *	@param gimmick_obj_work	[in] ƒMƒ~ƒbƒN‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param player_obj_work	[in] ƒvƒŒƒCƒ„‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return TRUEF¶‚ÉˆÚ“®‚·‚é FALSEF‰E‚ÉˆÚ“®‚·‚é
 */
// ==========================================================================
BOOL gmGmkDrainTankOutCheckDirLeft( 
								const OBS_OBJECT_WORK* gimmick_obj_work, 
								const OBS_OBJECT_WORK* player_obj_work )
{
	amAssert( gimmick_obj_work );
	amAssert( player_obj_work );

	if ( player_obj_work->pos.x < gimmick_obj_work->pos.x ){
		return FALSE;
	}
	return TRUE;
}

// ==========================================================================
// gmGmkDrainTankOutAdjustPlayerOffsetBuoyancy
/*!
 *	ƒvƒŒƒCƒ„ƒIƒtƒZƒbƒg‚É•‚—Í”½‰f
 *
 *	@param out_work	[in]”r‰t‘•’uƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutAdjustPlayerOffsetBuoyancy( GMS_GMK_DRAIN_TANK_OUT_WORK* out_work )
{
	//…–Ê‚ÉŒü‚©‚Á‚ÄˆÚ“®
	fx32 target_y = gmGmkDrainTankGameSystemGetWaterLevel()*FX32_ONE + GMD_GMK_DRAIN_TANK_OFFSET_FLOAT;

	//oŒû‚æ‚è‰º‚É‚Ís‚©‚È‚¢
	if ( target_y > out_work->base_pos_y ){
		target_y = out_work->base_pos_y;
	}
	fx32 move = target_y - (out_work->base_pos_y + out_work->player_offset_y);
	if ( MTM_MATH_ABS(move) > GMD_GMK_DRAIN_TANK_PLAYER_SPEED_FLOAT_MAX ){
		if ( move < 0 ){
			move = -GMD_GMK_DRAIN_TANK_PLAYER_SPEED_FLOAT_MAX;
		}
		else{
			move = GMD_GMK_DRAIN_TANK_PLAYER_SPEED_FLOAT_MAX;
		}
	}
	
	out_work->player_offset_y += move;
}

// ==========================================================================
// gmGmkDrainTankOutAdjustPlayerOffsetWave
/*!
 *	ƒvƒŒƒCƒ„ƒIƒtƒZƒbƒg‚É”g”½‰f
 *
 *	@param out_work	[in]”r‰t‘•’uƒ[ƒN
 *	@param player_obj_work	[in]ƒvƒŒƒCƒ„‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutAdjustPlayerOffsetWave( 
											 GMS_GMK_DRAIN_TANK_OUT_WORK* out_work,
											 OBS_OBJECT_WORK* player_obj_work)
{
	fx32 move_x = out_work->camera_roll >> GMD_GMK_DRAIN_TANK_BITSHIFT_SPEED_PLAYER;

	//•Ç‚É“–‚½‚Á‚Ä‚¢‚é
	if ( player_obj_work->move_flag & OBD_MOVE_FRONT ){
		if ( player_obj_work->disp_flag & OBD_DISP_HFLIP ){
			out_work->player_offset_x += FX_F32_TO_FX32(0.4f);
		}
		else{
			out_work->player_offset_x -= FX_F32_TO_FX32(0.4f);
		}
		return;
	}

	if ( out_work->camera_roll < 0 ){
		player_obj_work->disp_flag |= OBD_DISP_HFLIP;
	}
	else if ( out_work->camera_roll > 0 ){
		player_obj_work->disp_flag &= ~OBD_DISP_HFLIP;
	}
	out_work->player_offset_x += move_x;
}

// ==========================================================================
// gmGmkDrainTankOutApplyPlayerOffset
/*!
 *	ƒvƒŒƒCƒ„ˆÚ“®—Ê‚ğİ’è
 *
 *	@param player_obj_work	[in]ƒvƒŒƒCƒ„‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param out_work	[in]”r‰t‘•’uƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutApplyPlayerOffset( 
									   OBS_OBJECT_WORK* player_obj_work, 
									   const GMS_GMK_DRAIN_TANK_OUT_WORK* out_work )
{	
	fx32 target_x = out_work->base_pos_x + out_work->player_offset_x;
	fx32 target_y = out_work->base_pos_y + out_work->player_offset_y;

	player_obj_work->spd.x = target_x - player_obj_work->pos.x;
	player_obj_work->spd.y = target_y - player_obj_work->pos.y;
}

// ==========================================================================
// gmGmkDrainTankOutUpdateCameraRoll
/*!
 *	ƒJƒƒ‰‰ñ“]XV
 *
 *	@param out_work	[in]”r‰t‘•’uƒ[ƒN
 *	@param rot_z	[in]ƒL[“ü—Í—Ê
 */
// ==========================================================================
void gmGmkDrainTankOutUpdateCameraRoll( GMS_GMK_DRAIN_TANK_OUT_WORK* out_work, Angle32 rot_z )
{
	//¶‚ÉŒX‚¢‚Ä‚¢‚é‚Æ‚«
	if ( rot_z < -GMD_GMK_DRAIN_TANK_KEY_MARGIN ){
		if ( out_work->camera_roll <= -GMD_GMK_DRAIN_TANK_ROLL_ANGLE_MAX ){
			out_work->camera_roll = -GMD_GMK_DRAIN_TANK_ROLL_ANGLE_MAX;
		}
		else{
			out_work->camera_roll -= GMD_GMK_DRAIN_TANK_ROLL_ANGLE_SPEED;
		}
	}
	//‰E‚ÉŒX‚¢‚Ä‚¢‚é‚Æ‚«
	else if ( rot_z > GMD_GMK_DRAIN_TANK_KEY_MARGIN ){
		if ( out_work->camera_roll >= GMD_GMK_DRAIN_TANK_ROLL_ANGLE_MAX ){
			out_work->camera_roll = GMD_GMK_DRAIN_TANK_ROLL_ANGLE_MAX;
		}
		else{
			out_work->camera_roll += GMD_GMK_DRAIN_TANK_ROLL_ANGLE_SPEED;
		}	
	}
}

// ==========================================================================
// gmGmkDrainTankOutUpdateCameraRollDamage
/*!
 *	ƒJƒƒ‰‰ñ“]XViƒ_ƒ[ƒWj
 *
 *	@param out_work	[in]”r‰t‘•’uƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutUpdateCameraRollDamage( GMS_GMK_DRAIN_TANK_OUT_WORK* out_work )
{
	Angle32 move = GMD_GMK_DRAIN_TANK_ROLL_ANGLE_SPEED*4;
	if ( out_work->flag_dir_left ){
		out_work->camera_roll += move;
		if ( out_work->camera_roll < -GMD_GMK_DRAIN_TANK_ROLL_ANGLE_MAX ){
			out_work->camera_roll = -GMD_GMK_DRAIN_TANK_ROLL_ANGLE_MAX;
		}
	}
	else {
		out_work->camera_roll -= move;
		if ( out_work->camera_roll > GMD_GMK_DRAIN_TANK_ROLL_ANGLE_MAX ){
			out_work->camera_roll = GMD_GMK_DRAIN_TANK_ROLL_ANGLE_MAX;
		}
	}
}

// ==========================================================================
// gmGmkDrainTankOutUpdateCameraRollDie
/*!
 *	ƒJƒƒ‰‰ñ“]XVi€–Sj
 *
 *	@param out_work	[in]”r‰t‘•’uƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutUpdateCameraRollDie( GMS_GMK_DRAIN_TANK_OUT_WORK* out_work )
{
	out_work->camera_roll -= out_work->camera_roll/5;
}

// ==========================================================================
// gmGmkDrainTankOutUpdateCameraOffset
/*!
 *	ƒJƒƒ‰ƒIƒtƒZƒbƒgXV
 *
 *	@param player_work	[io]ƒvƒŒƒCƒ„ƒ[ƒN
 *	@param out_work		[in]”r‰t‘•’uƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankOutUpdateCameraOffset( 
										 GMS_PLAYER_WORK* player_work,
										 const GMS_GMK_DRAIN_TANK_OUT_WORK* out_work )
{
	const OBS_OBJECT_WORK* player_obj_work = (const OBS_OBJECT_WORK*)player_work;
	amAssert( player_obj_work );
	amAssert( out_work );

	float offset_x = FX_FX32_TO_F32((out_work->base_pos_x - GMD_GMK_DRAIN_TANK_OFFSET_OUT_TO_CAMERA) - player_obj_work->pos.x);
	GmPlayerCameraOffsetSet( player_work, (s16)offset_x, 0 );
}

// ==========================================================================
// gmGmkDrainTankOutCheckKeyDir
/*!
 *	oŒû•ûŒü‚ÉŒX‚¢‚Ä‚¢‚é‚©ƒ`ƒFƒbƒN
 *
 *	@param gimmick_obj_work	[in]ƒMƒ~ƒbƒN‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param camera_roll		[in]ŒX‚«—Ê
 *
 *	@retval	TRUE:³‚µ‚¢Œü‚«
 *	@retval	FALSE:‹tŒü‚«
 */
// ==========================================================================
BOOL gmGmkDrainTankOutCheckKeyDir( 
								  const OBS_OBJECT_WORK* gimmick_obj_work,
								  Angle32 camera_roll )
{
	GMS_GMK_DRAIN_TANK_OUT_WORK* out_work = (GMS_GMK_DRAIN_TANK_OUT_WORK*)gimmick_obj_work;

	//¶‚ÉŒX‚¢‚Ä‚¢‚é‚Æ‚«A¶‚ÉoŒû‚ª‚ ‚éê‡
	if ( (camera_roll < 0) && out_work->flag_dir_left ){
		return TRUE;
	}
	//‰E‚ÉŒX‚¢‚Ä‚¢‚é‚Æ‚«A‰E‚ÉoŒû‚ª‚ ‚éê‡
	else if ( (camera_roll > 0) && (!out_work->flag_dir_left) ){
		return TRUE;
	}

	return FALSE;
}

// ==========================================================================
// gmGmkDrainTankOutSinkRing
/*!
 *	ƒŠƒ“ƒO‚ğ’¾‚ß‚é
 */
// ==========================================================================
void gmGmkDrainTankOutSinkRing( void )
{
	GMS_RING_SYS_WORK* ring_system_work = GmRingGetWork();
	amAssert( ring_system_work );

	//ƒ_ƒ[ƒWƒŠƒ“ƒO
	GMS_RING_WORK* iter = ring_system_work->damage_ring_list_start;
	while( iter ){
		iter->spd_y = FX32_ONE;
		iter->spd_x /= 2;
		iter = iter->post_ring;
	}
}

// ==========================================================================
//•¬o‚·…
// ==========================================================================

// ==========================================================================
// gmGmkDrainTankSplashMainFunc
/*!
 *	•¬o‚·…ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankSplashMainFunc( OBS_OBJECT_WORK* obj_work )
{
	--obj_work->user_timer;
	if ( obj_work->user_timer > 0 ){
		Float alpha = (Float)obj_work->user_timer / (Float)GMD_GMK_DRAINTANK_SPLASH_ALPHA_TIME;
		obj_work->obj_3d->draw_state.alpha.alpha = alpha;
	}
	else{
		obj_work->disp_flag |= OBD_DISP_STOP;
		obj_work->ppFunc = NULL;
	}
}

// ==========================================================================
// gmGmkDrainTankSplashEffectMain
/*!
 *	•¬o‚·…ƒGƒtƒFƒNƒgˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkDrainTankSplashEffectMain( OBS_OBJECT_WORK *obj_work )
{
	obj_work->dir.z += GMD_GMK_DRAIN_TANK_SPLASH_EFFECT_SPEED_ROT_Z;
	
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
