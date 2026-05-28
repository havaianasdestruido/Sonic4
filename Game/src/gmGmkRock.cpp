// ==========================================================================
/*!
  @file gmGmkRock.cpp
  @brief ƒMƒ~ƒbƒN ‘åŠâ

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkRock.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 *	GMS_EVE_RECORD_EVENT
 *		left		: ’ÇÕƒ^ƒCƒv‚Ì‹——£(2”{‚·‚é) or —‰ºƒ^ƒCƒv‚ÌŠÔŠu(2”{‚µ‚È‚¢iƒc[ƒ‹İ’è‚Ì”¼•ª‚Ì’l‚É‚È‚éj)
 *		top			: ’ÇÕƒ^ƒCƒv‚ÌÅ’áŒÀ•K—v‚ÈƒvƒŒƒCƒ„‘¬“x(2”{‚·‚é) or —‰ºƒ^ƒCƒv‚Ì¶¬‚xÀ•W(2”{‚·‚é)
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
#include "gmSound.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectZone.h"
#include "gmCamera.h"
#include "gmPadVib.h"

#include "gmGmkRock.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "common/model/gmk_rock_mdl.hmb"
#include "common/model/gmk_rock_mtn.hmb"


//----- Definitions ---------------------------------------------------------

#define GMD_GMK_ROCK_CHASE_SPEED_ADD_SLOW			( 0x00000200L )		//‰Á‘¬“x
#define GMD_GMK_ROCK_CHASE_SPEED_MAX_SLOW			( 0x00010000L )		//‘¬“xÅ‘å’l
#define GMD_GMK_ROCK_CHASE_DISTANCE_FAR				((fx32)300 * FX32_ONE )	//‰“‚Æ”»’f‚·‚é‹——£
#define GMD_GMK_ROCK_CHASE_SPEED_ADD_FAR			( 0x00000f00L )		//‰“‚Ì‰Á‘¬“x
#define GMD_GMK_ROCK_CHASE_SPEED_MAX_OFFSET_FAR		( 0x00008000L )		//‰“‚Ì‘¬“xÅ‘å’lƒIƒtƒZƒbƒg
#define GMD_GMK_ROCK_CHASE_SPEED_ADD_MID			( 0x00000300L )		//’†‚Ì‰Á‘¬“x
#define GMD_GMK_ROCK_CHASE_SPEED_MAX_OFFSET_MID		( 0x00002800L )		//’†‚Ì‘¬“xÅ‘å’lƒIƒtƒZƒbƒg
#define GMD_GMK_ROCK_CHASE_SPEED_ADD_NEAR			(-0x00000300L )		//‹ß‚Ì‰Á‘¬“x
#define GMD_GMK_ROCK_CHASE_SPEED_MAX_OFFSET_NEAR	(-0x00001800L )		//‹ß‚Ì‘¬“xÅ‘å’lƒIƒtƒZƒbƒg
#define GMD_GMK_ROCK_CHASE_BOUND					(10)				//’e‚ŞŠm—¦
#define	GMD_GMK_ROCK_CHASE_BOUND_VAL_MIN			(32)				//’e‚Ş—ÊÅ¬’l
#define	GMD_GMK_ROCK_CHASE_BOUND_VAL				(16)				//’e‚Ş—ÊU‚ê•
#define	GMD_GMK_ROCK_CHASE_BOUND_FALL				(0x00002000L)		//’e‚ŞŒ¸Š
#define	GMD_GMK_ROCK_CHASE_QUAKE					((fx32)3*FX32_ONE)	//‰æ–ÊU“®
#define	GMD_GMK_ROCK_CHASE_DELETE_TCB_DISTANCE		((fx32)512*FX32_ONE)		//TCBíœ‹——£

#define GMD_GMK_ROCK_FALL_SPEED_FALL				(0x00000150L )		//—‰º‰Á‘¬“x
#define GMD_GMK_ROCK_FALL_SPEED_FALL_MAX			(0x00008000L )		//—‰º‘¬“xÅ‘å’l
#define GMD_GMK_ROCK_FALL_QUAKE_FALL				((fx32)1*FX32_ONE)	//‰æ–ÊU“®
#define GMD_GMK_ROCK_FALL_WAIT_TIME					(30)				//—‰º‘Ò‚¿ŠÔ
#define GMD_GMK_ROCK_FALL_INTERCAL_NO_WAIT			(120)				//—‰ºŠÔŠu‚ª’Z‚¢ê‡‚ÍA—‰º‘Ò‚¿‚ğ‚µ‚È‚¢
#define GMD_GMK_ROCK_FALL_EFFECT_WAIT_TIME			(30)				//ƒGƒtƒFƒNƒg‘Ò‚¿ŠÔ
#define GMD_GMK_ROCK_FALL_ROLL						(0x0080)			//‰ñ“]—Ê


#define GMD_GMK_ROCK_SIZE_HIT_RECT					(40)				//ƒqƒbƒg‹éŒ`ƒTƒCƒY
#define GMD_GMK_ROCK_SIZE_FIELD_RECT				(28)				//’nŒ`‹éŒ`ƒTƒCƒYi¶A‰EAãj
#define GMD_GMK_ROCK_SIZE_FIELD_RECT_BOTTOM			(42)				//’nŒ`‹éŒ`ƒTƒCƒYi‰ºj

#define GMD_GMK_ROCK_SIZE_EFFECT_OFFEST_Y			((GMD_GMK_ROCK_SIZE_FIELD_RECT_BOTTOM+8)*FX32_ONE)	//’nŒ`ƒGƒtƒFƒNƒgƒIƒtƒZƒbƒg

#define GMD_GMK_ROCK_FX32_BITSHIFT_BLOCK_SIZE		(5)							//ƒTƒCƒYƒrƒbƒgƒVƒtƒg
#define GMD_GMK_ROCK_FALL_HOOK_CREATE_OFFSET_Y		((fx32)64*FX32_ONE)			//‘•’u‚ğ¶¬‚·‚éÀ•WƒIƒtƒZƒbƒg
#define GMD_GMK_ROCK_HOOK_POS_OFFSET_Y				((fx32)24*FX32_ONE)			//‘åŠâ‚ğ‘•’u‚É”z’u‚·‚éÀ•WƒIƒtƒZƒbƒg



//ƒ‚ƒfƒ‹‰ñ“]‰‰o‰ñ“]—Ê
#define MGD_GMK_ROCK_ADD_ANGLE_Z			(1000)

//—‰ºŠÔŠu
#define GMD_GMK_ROCK_FALL_INTERVAL			( 60 * 5 )

//’ÇÕ‹——£
#define GMD_GMK_ROCK_CHASE_DISTANCE_MID		( 96 )

//’ÇÕ‘¬“x
#define GMD_GMK_ROCK_CHASE_CHECK_SPEED		( 5 )

#define	GMD_GMK_ROCK_QUAKE_FALL						(3 << FX32_SHIFT)	//U“®—Ê

//Œü‚«ƒ^ƒCƒv
enum GME_GMK_ROCK_DIR_TYPE{
	GMD_GMK_ROCK_DIR_TYPE_INVALID = 0,
	GMD_GMK_ROCK_DIR_TYPE_RIGHT,
	GMD_GMK_ROCK_DIR_TYPE_LEFT,

	GMD_GMK_ROCK_DIR_TYPE_MAX
};

//’ÇÕ‘åŠâƒ[ƒN
typedef struct tag_GMS_GMK_ROCK_CHASE_WORK{
	GMS_ENEMY_3D_WORK enemy_work;		//ƒGƒlƒ~[ƒ[ƒN
	GMS_EFFECT_3DES_WORK* effect_work;	//ƒGƒtƒFƒNƒgƒ[ƒN
	fx32 target_bound;					//’e‚Ş—Ê
	fx32 current_bound;					//Œ»İ’l

	fx32 length;						//’ÇÕ‹——£
	fx32 speed;							//’ÇÕƒXƒs[ƒh
	u16 angle_z;						//Z²‰ñ“]—Êi•`‰æ—pj
	u16 reserve; 
	GME_GMK_ROCK_DIR_TYPE dir_type;		//Œü‚«ƒ^ƒCƒv

	GSS_SND_SE_HANDLE* se_handle;		//SEƒnƒ“ƒhƒ‹

	BOOL flag_vib;						//U“®ƒtƒ‰ƒO

	GMS_ENEMY_3D_WORK* hook_work;		//‘åŠâ‚ğx‚¦‚é‘•’u‚Ìƒ[ƒN
}GMS_GMK_ROCK_CHASE_WORK;

//—‰º‘åŠâƒ[ƒN
typedef struct tag_GMS_GMK_ROCK_FALL_WORK{
	GMS_ENEMY_3D_WORK enemy_work;		//ƒGƒlƒ~[ƒ[ƒN
	GMS_EFFECT_3DES_WORK* effect_work;	//ƒGƒtƒFƒNƒgƒ[ƒN

	s32 wait_time;						//—‰º‘Ò‚¿ŠÔ
	u16 roll;							//‰ñ“]
	u16 roll_d;							//‰ñ“]—Ê

	GMS_ENEMY_3D_WORK* hook_work;		//‘åŠâ‚ğx‚¦‚é‘•’u‚Ìƒ[ƒN
}GMS_GMK_ROCK_FALL_WORK;

//‘åŠâŠÇ—ƒ[ƒN
typedef struct tag_GMS_GMK_ROCK_FALL_MGR_WORK{
	GMS_ENEMY_3D_WORK enemy_work;		//ƒGƒlƒ~[ƒ[ƒN

	s32 interval;						//—‰ºŠÔŠu

	GMS_ENEMY_3D_WORK* hook_work;		//‘åŠâ‚ğx‚¦‚é‘•’u‚Ìƒ[ƒN
}GMS_GMK_ROCK_FALL_MGR_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static GMS_ENEMY_3D_WORK* gmGmkRockLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type, 
									u32 work_size );
static GMS_ENEMY_3D_WORK* gmGmkRockLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type, 
									u32 work_size);
static GMS_ENEMY_3D_WORK* gmGmkRockLoadObjHook( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type);

static void gmGmkRockMoveFunc( OBS_OBJECT_WORK* work );
static void gmGmkRockFallDrawFunc( OBS_OBJECT_WORK* obj_work );
static void gmGmkRockChaseDrawFunc( OBS_OBJECT_WORK* obj_work );
static void gmGmkRockChaseTcbDest( MTS_TASK_TCB *tcb );
static void gmGmkRockWaitDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect );
static void gmGmkRockSetRectActive( GMS_ENEMY_3D_WORK* gimmick_work );
static void gmGmkRockSetRectWait( GMS_ENEMY_3D_WORK* gimmick_work );


static void gmGmkRockChaseInit( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockChaseChangeModeFall( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockChaseChangeModeChase( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockChaseChangeModeEnd( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockChaseMainFall( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockChaseMainChase( OBS_OBJECT_WORK *obj_work );

static void gmGmkRockManagerInit( OBS_OBJECT_WORK* obj_work );
static void gmGmkRockManagerMain( OBS_OBJECT_WORK* obj_work );

static void gmGmkRockFallInit( OBS_OBJECT_WORK* obj_work );
static void gmGmkRockFallMainStart( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockFallMainWait( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockFallMainFallWaitEffect( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockFallMainFall( OBS_OBJECT_WORK *obj_work );


static void gmGmkRockHookInit( OBS_OBJECT_WORK* obj_work );
static void gmGmkRockHookChangeModeWait( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockHookkChangeModeActive( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockHookMainActive( OBS_OBJECT_WORK *obj_work );

//---------------------------------------------------------
//’ÇÕ—pİ’è
//---------------------------------------------------------
static void gmGmkRockChaseSetLength( GMS_GMK_ROCK_CHASE_WORK* rock_work, fx32 length );
static fx32 gmGmkRockChaseGetLength( const GMS_GMK_ROCK_CHASE_WORK* rock_work );

static void gmGmkRockChaseSetSpeed( GMS_GMK_ROCK_CHASE_WORK* rock_work, fx32 speed );
static fx32 gmGmkRockChaseGetSpeed( const GMS_GMK_ROCK_CHASE_WORK* rock_work );

static void gmGmkRockChaseSetAngleZ( GMS_GMK_ROCK_CHASE_WORK* rock_work, u16 angle_z );
static void gmGmkRockChaseAddAngleZ( GMS_GMK_ROCK_CHASE_WORK* rock_work, s16 angle_z );
static u16 gmGmkRockChaseGetAngleZ( const GMS_GMK_ROCK_CHASE_WORK* obj_work );

static void gmGmkRockChaseSetDirType( GMS_GMK_ROCK_CHASE_WORK* rock_work, GME_GMK_ROCK_DIR_TYPE dir_type );
static GME_GMK_ROCK_DIR_TYPE gmGmkRockChaseGetDirType( const GMS_GMK_ROCK_CHASE_WORK* rock_work );

//---------------------------------------------------------
//—‰º—pİ’è
//---------------------------------------------------------
static void gmGmkRockFallMgrSetInterval( GMS_GMK_ROCK_FALL_MGR_WORK* mgr_work, s32 interval );
static s32 gmGmkRockFallMgrGetInterval( const GMS_GMK_ROCK_FALL_MGR_WORK* mgr_work );

static void gmGmkRockFallMgrSetUserTimer( OBS_OBJECT_WORK* obj_work, s32 count );
static void gmGmkRockFallMgrAddUserTimer( OBS_OBJECT_WORK* obj_work, s32 count );
static s32 gmGmkRockFallMgrGetUserTimer( const OBS_OBJECT_WORK* obj_work );

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

static OBS_ACTION3D_NN_WORK* g_gm_gmk_rock_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkRockBuild
/*!
 *	ƒMƒ~ƒbƒN ‘åŠâ ƒf[ƒ^\’z
 */
// ==========================================================================
void GmGmkRockBuild(void)
{
	g_gm_gmk_rock_obj_3d_list = GmGameDBuildRegBuildModel(
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_ROCK_MODEL ),
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_ROCK_TEX ),
			0	//draw_flag
			);
}

// ==========================================================================
// GmGmkRockFlush
/*!
 *	ƒMƒ~ƒbƒN ‘åŠâ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
void GmGmkRockFlush(void)
{
	AMS_AMB_HEADER* amb_header = (AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_ROCK_MODEL );

	GmGameDBuildRegFlushModel( g_gm_gmk_rock_obj_3d_list, amb_header->file_num );
	g_gm_gmk_rock_obj_3d_list = NULL;
}

// ==========================================================================
// GmGmkRockChaseManagerInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”i‘åŠâ¶¬j
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] 
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkRockChaseManagerInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒg©‘Ì‚Í‘åŠâ‚ğx‚¦‚é‘•’u‚Æ“¯‚¶‚à‚Ìiˆá‚¢‚Í’ÇÕ‘åŠâ‚ğ¶¬‚·‚é‚±‚Æj
	OBS_OBJECT_WORK* obj_work = GmGmkRockHookInit(
			eve_rec,
			pos_x,
			pos_y, 
			type);
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//---------------------------------------------------------------------------
	//‘åŠâ
	//---------------------------------------------------------------------------
	OBS_OBJECT_WORK* chase_obj_work = GmEventMgrLocalEventBirth(
		GMD_EVENT_ID_NOSET_ROCK_CHASE,
		pos_x,
		pos_y,
		eve_rec->flag,
		eve_rec->left,
		eve_rec->top,
		eve_rec->width,
		eve_rec->height,
		0);

	//“o˜^
	GMS_GMK_ROCK_CHASE_WORK* rock_work = (GMS_GMK_ROCK_CHASE_WORK*)chase_obj_work;
	amAssert( rock_work );
	rock_work->hook_work = gimmick_work;

	return obj_work;
}

// ==========================================================================
// GmGmkRockChaseInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”i’ÇÕ‘åŠâj
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkRockChaseInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_GMK_ROCK_CHASE_WORK* rock_work = (GMS_GMK_ROCK_CHASE_WORK*)gmGmkRockLoadObj( 
			eve_rec, 
			pos_x, 
			pos_y, 
			type,
			sizeof(GMS_GMK_ROCK_CHASE_WORK) );
	amAssert( rock_work );
	OBS_OBJECT_WORK* obj_work = &rock_work->enemy_work.ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkRockChaseInit( obj_work );

	//’ÇÕ‹——£
	gmGmkRockChaseSetLength( rock_work, (fx32)(eve_rec->left*2)*FX32_ONE );

	//’ÇÕ‘¬“x
	gmGmkRockChaseSetSpeed( rock_work, (fx32)(eve_rec->top*2)*FX32_ONE );

	return obj_work;
}

// ==========================================================================
// GmGmkRockFallManagerInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”i‘åŠâ¶¬j
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] 
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkRockFallManagerInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{

	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_GMK_ROCK_FALL_MGR_WORK* mgr_work = (GMS_GMK_ROCK_FALL_MGR_WORK*)gmGmkRockLoadObjNoModel( 
			eve_rec, 
			pos_x, 
			pos_y, 
			type,
			sizeof(GMS_GMK_ROCK_FALL_MGR_WORK));
	amAssert( mgr_work );
	OBS_OBJECT_WORK* obj_work = &mgr_work->enemy_work.ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkRockManagerInit( obj_work );

	//—‰ºŠÔŠui•b‚Åİ’è‚³‚ê‚Ä‚¢‚é‚Ì‚ÅƒtƒŒ[ƒ€”‚É•ÏŠ·j
	gmGmkRockFallMgrSetInterval( mgr_work, eve_rec->left*60 );
	gmGmkRockFallMgrSetUserTimer( obj_work, eve_rec->left*60 );

	//---------------------------------------------------------------------------
	//‘•’u
	//---------------------------------------------------------------------------
	OBS_OBJECT_WORK* hook_obj_work = GmEventMgrLocalEventBirth(
		GMD_EVENT_ID_NOSET_ROCK_HOOK,
		obj_work->pos.x,
		obj_work->pos.y - (eve_rec->top*2*FX32_ONE) + GMD_GMK_ROCK_FALL_HOOK_CREATE_OFFSET_Y,
		eve_rec->flag,
		eve_rec->left,
		eve_rec->top,
		eve_rec->width,
		eve_rec->height,
		0);
	hook_obj_work->flag |= OBD_OBJECT_NOCLIP;
	hook_obj_work->parent_obj = obj_work;

	//“o˜^
	mgr_work->hook_work = (GMS_ENEMY_3D_WORK*)hook_obj_work;

	return obj_work;
}

// ==========================================================================
// GmGmkRockFallInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”i‘åŠâ¶¬j
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] 
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkRockFallInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_GMK_ROCK_FALL_WORK* rock_work = (GMS_GMK_ROCK_FALL_WORK*)gmGmkRockLoadObj( 
			eve_rec, 
			pos_x, 
			pos_y, 
			type,
			sizeof(GMS_GMK_ROCK_FALL_WORK)  );
	amAssert( rock_work );
	OBS_OBJECT_WORK* obj_work = &rock_work->enemy_work.ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkRockFallInit( obj_work );

	//‘Ò‚¿ŠÔ
	if ( type ){
		rock_work->wait_time = GMD_GMK_ROCK_FALL_WAIT_TIME;
	}
	else{
		rock_work->wait_time = 0;
	}

	return obj_work;
}

// ==========================================================================
// GmGmkRockHookInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”i‘åŠâ‚ğx‚¦‚é‘•’uj
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] 
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkRockHookInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkRockLoadObjHook( 
			eve_rec, 
			pos_x, 
			pos_y, 
			type );
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkRockHookInit( obj_work );
	return obj_work;
}

//----- Local Functions -----------------------------------------------------

// ==========================================================================
// gmGmkRockLoadObjNoModel
/*!
 *	ƒMƒ~ƒbƒN“Ç‚İ‚İƒ‚ƒfƒ‹‚È‚µi‘åŠâj
 *
 *	@param eve_rec	[in] ƒCƒxƒ“ƒgƒŒƒR[ƒh
 *	@param pos_x	[in] À•WX
 *	@param pos_y	[in] À•WY
 *	@param type		[in] ƒ^ƒCƒv
 *	@param work_size	[in] ƒ[ƒNƒTƒCƒY
 *
 *	@return ƒ[ƒN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkRockLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type,
									u32 work_size)
{

	UNREFERENCED_PARAMETER(type);

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec, 
			pos_x, 
			pos_y, 
			work_size, 
			"GMK_ROCK");

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}

// ==========================================================================
// gmGmkRockLoadObj
/*!
 *	ƒMƒ~ƒbƒN“Ç‚İ‚İi‘åŠâj
 *
 *	@param eve_rec	[in] ƒCƒxƒ“ƒgƒŒƒR[ƒh
 *	@param pos_x	[in] À•WX
 *	@param pos_y	[in] À•WY
 *	@param type		[in] ƒ^ƒCƒv
 *	@param work_size	[in] ƒ[ƒNƒTƒCƒY
 *
 *	@return ƒ[ƒN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkRockLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type,
									u32 work_size )
{

	UNREFERENCED_PARAMETER(type);

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkRockLoadObjNoModel(
			eve_rec,
			pos_x,
			pos_y,
			type,
			work_size);
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//-------------------------------------------------
	// ƒ‚ƒfƒ‹‰Šú‰»
	//-------------------------------------------------
	//“Ç‚İ‚İ
	ObjObjectCopyAction3dNNModel(
		obj_work,
		&g_gm_gmk_rock_obj_3d_list[IDB_GMK_ROCK_MDL_GMK_ROCK_ZNO],
		&gimmick_work->obj_3d);
#if _IPHONE
	// ê—pƒ‰ƒCƒgİ’è
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE
	
	return gimmick_work;
}

// ==========================================================================
// gmGmkRockLoadObjHook
/*!
 *	ƒMƒ~ƒbƒN“Ç‚İ‚İi‘åŠâ‘•’uj
 *
 *	@param eve_rec	[in] ƒCƒxƒ“ƒgƒŒƒR[ƒh
 *	@param pos_x	[in] À•WX
 *	@param pos_y	[in] À•WY
 *	@param type		[in] ƒ^ƒCƒv
 *
 *	@return ƒ[ƒN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkRockLoadObjHook( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type )
{

	UNREFERENCED_PARAMETER(type);

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	//”z’uÀ•W‚ğ”¼ƒuƒƒbƒN•‚Å’²®
	s32 block_y = (s32)(pos_y >> (FX32_SHIFT + GMD_GMK_ROCK_FX32_BITSHIFT_BLOCK_SIZE));
	fx32 block_y_fx32 = block_y << (FX32_SHIFT + GMD_GMK_ROCK_FX32_BITSHIFT_BLOCK_SIZE);
	
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkRockLoadObjNoModel(
			eve_rec,
			pos_x,
			block_y_fx32,
			type,
			sizeof(GMS_ENEMY_3D_WORK));
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//-------------------------------------------------
	// ƒ‚ƒfƒ‹‰Šú‰»
	//-------------------------------------------------
	//“Ç‚İ‚İ
	ObjObjectCopyAction3dNNModel(
		obj_work,
		&g_gm_gmk_rock_obj_3d_list[IDB_GMK_ROCK_MDL_GMK_ROCK_HOOK_ZNO],
		&gimmick_work->obj_3d);

	//ƒ‚[ƒVƒ‡ƒ“
	ObjObjectAction3dNNMotionLoad( 
			obj_work,
			0,
			FALSE,
			ObjDataGet(GMD_DWORK_NO_GMK_ROCK_MTN),
			NULL,
			0,
			NULL
	);


	return gimmick_work;
}

// ==========================================================================
// gmGmkRockMoveFunc
/*!
 *	ƒMƒ~ƒbƒN‘åŠâƒAƒNƒeƒBƒuˆÚ“®ŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockMoveFunc( OBS_OBJECT_WORK* obj_work )
{
	ObjObjectMove( obj_work );
}

// ==========================================================================
// gmGmkRockFallDrawFunc
/*!
 *	ƒMƒ~ƒbƒN‘åŠâƒAƒNƒeƒBƒu•`‰æŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockFallDrawFunc( OBS_OBJECT_WORK* obj_work )
{
	VecU16 dir = obj_work->dir;

	GMS_GMK_ROCK_FALL_WORK* rock_work = (GMS_GMK_ROCK_FALL_WORK*)obj_work;

	//‰ñ“]‚³‚¹‚é
	u16 roll = (u16)rock_work->roll;
	obj_work->dir.y = roll;
	u16 rot_z = (u16)obj_work->user_work;
	obj_work->dir.z = rot_z; 
	obj_work->dir.z += roll;
		
	//•`‰æ
	ObjDrawActionSummary( obj_work );

	//•`‰æ
	ObjDrawActionSummary( obj_work );
	
	//–ß‚·
	obj_work->dir = dir;

}

// ==========================================================================
// gmGmkRockChaseDrawFunc
/*!
 *	ƒMƒ~ƒbƒN‘åŠâƒAƒNƒeƒBƒu•`‰æŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockChaseDrawFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_GMK_ROCK_CHASE_WORK* rock_work = (GMS_GMK_ROCK_CHASE_WORK*)obj_work;

	VecU16 dir = obj_work->dir;

	//‰ñ“]‚³‚¹‚é
	u16 roll = gmGmkRockChaseGetAngleZ(rock_work);
	if ( obj_work->spd_m < 0 ){
		obj_work->dir.z = roll;
	}
	else{
		obj_work->dir.z = roll;
	}
	u16 rot_x = (u16)obj_work->user_work;
	obj_work->dir.x = rot_x; 

	//’e‚Ş
	obj_work->pos.y += rock_work->current_bound;
		
	//•`‰æ
	ObjDrawActionSummary( obj_work );

	//•`‰æ
	ObjDrawActionSummary( obj_work );
	
	//–ß‚·
	obj_work->dir = dir;
	obj_work->pos.y -= rock_work->current_bound;

}

// ==========================================================================
// gmGmkRockChaseTcbDest
/*!
 * OBJƒfƒXƒgƒ‰ƒNƒ^
 *
 * @param tcb TCB
 */
// ==========================================================================
void gmGmkRockChaseTcbDest( MTS_TASK_TCB *tcb )
{
	GMS_GMK_ROCK_CHASE_WORK* rock_work = (GMS_GMK_ROCK_CHASE_WORK*)mtTaskGetTcbWork(tcb);

	//SEƒnƒ“ƒhƒ‹‰ğ•ú
	if ( rock_work->se_handle ){
		GmSoundStopSE( rock_work->se_handle );
		GsSoundFreeSeHandle( rock_work->se_handle );
		rock_work->se_handle = NULL;
	}

	//ƒIƒuƒWƒFƒNƒg‰ğ•ú
	GmEnemyDefaultExit( tcb );
}

// ==========================================================================
// gmGmkRockWaitDefFunc
/*!
 *	ƒMƒ~ƒbƒN‘åŠâ‘Ò‹@‹éŒ`ŠÖ”
 *
 *	@param own_rect	[in] ©g‚Ì‹éŒ`ƒ[ƒN
 *	@param target_rect	[in] ‘Šè‚Ì‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkRockWaitDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect )
{
	UNREFERENCED_PARAMETER( target_rect );
	OBS_OBJECT_WORK* obj_work = own_rect->parent_obj;

	GMS_GMK_ROCK_CHASE_WORK* rock_work = (GMS_GMK_ROCK_CHASE_WORK*)obj_work;
	OBS_OBJECT_WORK* obj_work_hook = (OBS_OBJECT_WORK*)rock_work->hook_work;
	amAssert( obj_work_hook );

	//‘åŠâ‚ğx‚¦‚é‘•’u‚Ìƒ‚[ƒh•ÏX
	gmGmkRockHookkChangeModeActive( obj_work_hook );

	//‘åŠâ‚ğx‚¦‚é‘•’u‚Ì“o˜^‰ğœ
	rock_work->hook_work = NULL;
	
	//ì¬
	GMS_EFFECT_3DES_WORK* effect_work = GmEfctZoneEsCreate(
			obj_work,
			GSD_MAIN_ZONE_TYPE_3,
			GME_EFCT_Z03_IDX_WALL_M_Z3 );
	amAssert( effect_work );
	effect_work->efct_com.obj_work.ppFunc = NULL;
	effect_work->efct_com.obj_work.pos.z = GMD_OBJ_GIMMICK_POS_Z_FRONT;
	effect_work->efct_com.obj_work.parent_ofst.y = GMD_GMK_ROCK_SIZE_EFFECT_OFFEST_Y;

	//ƒ‚[ƒh•ÏX
	gmGmkRockChaseChangeModeFall( obj_work );
}

// ==========================================================================
// gmGmkRockSetRectActive
/*!
 *	ƒMƒ~ƒbƒN‘åŠâƒAƒNƒeƒBƒu‹éŒ`İ’è
 *
 *	@param gimmick_work	[in] ƒMƒ~ƒbƒNƒ[ƒN
 */
// ==========================================================================
void gmGmkRockSetRectActive( GMS_ENEMY_3D_WORK* gimmick_work )
{
	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	ObjRectWorkZSet(rect_work,
						-GMD_GMK_ROCK_SIZE_HIT_RECT, -GMD_GMK_ROCK_SIZE_HIT_RECT, -500,
						GMD_GMK_ROCK_SIZE_HIT_RECT, GMD_GMK_ROCK_SIZE_HIT_RECT, 500);
	rect_work->flag |= OBD_RECT_OUT;

	//UŒ‚—p
	ObjRectAtkSet(rect_work, GMD_OBJ_RECT_ATK_FLAG_NORMALATK, GMD_OBJ_RECT_ATK_POWER_DEFAULT);

	//–hŒä—p
	ObjRectDefSet(rect_work, 0, 0);
	rect_work->ppDef = NULL;
}

// ==========================================================================
// gmGmkRockSetRectWait
/*!
 *	ƒMƒ~ƒbƒN‘åŠâ‘Ò‹@‹éŒ`İ’è
 *
 *	@param gimmick_work	[in] ƒMƒ~ƒbƒNƒ[ƒN
 */
// ==========================================================================
void gmGmkRockSetRectWait( GMS_ENEMY_3D_WORK* gimmick_work )
{
	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	ObjRectWorkZSet(rect_work,
						-GMD_GMK_ROCK_SIZE_HIT_RECT, -GMD_GMK_ROCK_SIZE_HIT_RECT, -500,
						GMD_GMK_ROCK_SIZE_HIT_RECT, 500, 500);
	rect_work->flag |= OBD_RECT_OUT;

	//UŒ‚—p
	ObjRectAtkSet(rect_work, 0, 0);

	//–hŒä—p
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	rect_work->ppDef = gmGmkRockWaitDefFunc;
}

// ==========================================================================
// gmGmkRockChaseInit
/*!
 *	ƒMƒ~ƒbƒN‘åŠâ’ÇÕó‘Ô‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockChaseInit( OBS_OBJECT_WORK *obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	//ƒvƒŒƒCƒ„[‚ ‚½‚è
	gmGmkRockSetRectWait( gimmick_work );

	//’nŒ`‚ ‚½‚è
	ObjObjectFieldRectSet( 
			obj_work, 
			-GMD_GMK_ROCK_SIZE_FIELD_RECT, 
			-GMD_GMK_ROCK_SIZE_FIELD_RECT,
			GMD_GMK_ROCK_SIZE_FIELD_RECT, 
			GMD_GMK_ROCK_SIZE_FIELD_RECT_BOTTOM );

	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	//obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// ˆÚ“®–³‚µ ’nŒ`‚ ‚½‚è–³‚µ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	//ƒ^[ƒQƒbƒgİ’è
	gimmick_work->ene_com.target_obj = &g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_work;

	//ZÀ•W
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	//‰Šú‰ñ“]
	GMS_GMK_ROCK_CHASE_WORK* rock_work = (GMS_GMK_ROCK_CHASE_WORK*)obj_work;
	u16	rand_angle = (u16)mtMathRand();
	gmGmkRockChaseSetAngleZ( rock_work, rand_angle );
	obj_work->user_work = (u16)mtMathRand();

	//SEƒnƒ“ƒhƒ‹Šm•Û
	rock_work->se_handle = GsSoundAllocSeHandle();

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = NULL;
	obj_work->ppMove = gmGmkRockMoveFunc;
	obj_work->ppOut = gmGmkRockChaseDrawFunc;

	//TCBƒfƒXƒgƒ‰ƒNƒ^İ’è
	mtTaskChangeTcbDestructor( obj_work->tcb, gmGmkRockChaseTcbDest );
}

// ==========================================================================
// gmGmkRockChaseChangeModeFall
/*!
 *	ƒMƒ~ƒbƒN‘åŠâi’ÇÕj —‰ºƒ‚[ƒh•ÏX
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockChaseChangeModeFall( OBS_OBJECT_WORK *obj_work )
{
	amAssert( obj_work );

	GMS_GMK_ROCK_CHASE_WORK* rock_work = (GMS_GMK_ROCK_CHASE_WORK*)obj_work;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;

	//ƒvƒŒƒCƒ„[‚ ‚½‚è
	gmGmkRockSetRectActive( gimmick_work );

	//‘¬“xƒŠƒZƒbƒg
	obj_work->spd_m = 0;
	obj_work->spd.x = 0;
	obj_work->spd.y = 0;

	//ŒXÎƒŠƒZƒbƒg
	obj_work->dir.z = 0;

	//ƒtƒ‰ƒO
	//obj_work->move_flag &= ~(OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);	// ˆÚ“®‚ ‚è ’nŒ`‚ ‚½‚è‚ ‚è
	obj_work->flag |= OBD_OBJECT_NOCLIP;						//‰æ–ÊŠO‚Éˆø‚«—£‚³‚ê‚Ä‚àƒNƒŠƒbƒv‚µ‚È‚¢
	obj_work->move_flag |= OBD_MOVE_DIR | OBD_MOVE_FALL;
	obj_work->move_flag &= ~OBD_MOVE_LIMIT_OUT;	// ‰æ–ÊŠO‚Å~‚Ü‚ç‚È‚¢

	//Œø‰Ê‰¹’â~i‰ñ“]‰¹j
	if ( rock_work->se_handle ){
		GmSoundStopSE( rock_work->se_handle );
	}
				
	//U“®’â~
	if ( rock_work->flag_vib ){
		GMM_PAD_VIB_STOP();
		rock_work->flag_vib = FALSE;
	}

	// ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmGmkRockChaseMainFall;
}

// ==========================================================================
// gmGmkRockChaseChangeModeChase
/*!
 *	ƒMƒ~ƒbƒN‘åŠâi’ÇÕj ’ÇÕƒ‚[ƒh•ÏX
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockChaseChangeModeChase( OBS_OBJECT_WORK *obj_work )
{
	GMS_GMK_ROCK_CHASE_WORK* rock_work = (GMS_GMK_ROCK_CHASE_WORK*)obj_work;

	obj_work->spd_m = 0;
	obj_work->spd.x = 0;
	obj_work->spd.y = 0;

	//Œü‚«ƒŠƒZƒbƒg
	gmGmkRockChaseSetDirType( rock_work, GMD_GMK_ROCK_DIR_TYPE_INVALID );

	// ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmGmkRockChaseMainChase;
	
	//ƒGƒtƒFƒNƒg
	if ( !rock_work->effect_work ){
		//ì¬
		GMS_EFFECT_3DES_WORK* effect_work = GmEfctZoneEsCreate(
				obj_work,
				GSD_MAIN_ZONE_TYPE_3,
				GME_EFCT_Z03_IDX_SMORK_B_Z3 );
		amAssert( effect_work );
		effect_work->efct_com.obj_work.ppFunc = NULL;
		effect_work->efct_com.obj_work.pos.z = GMD_OBJ_GIMMICK_POS_Z_FRONT;
		effect_work->efct_com.obj_work.parent_ofst.y = GMD_GMK_ROCK_SIZE_EFFECT_OFFEST_Y;
		
		//ƒ[ƒN‚É“o˜^
		rock_work->effect_work = effect_work;
	}

	//Œø‰Ê‰¹i‰ñ“]‰¹j
	GmSoundPlaySE("BigRock2", rock_work->se_handle);
}

// ==========================================================================
// gmGmkRockChaseChangeModeEnd
/*!
 *	ƒMƒ~ƒbƒN‘åŠâi’ÇÕj I—¹ƒ‚[ƒh•ÏX
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockChaseChangeModeEnd( OBS_OBJECT_WORK *obj_work )
{				
	//U“®’â~
	GMS_GMK_ROCK_CHASE_WORK* rock_work = (GMS_GMK_ROCK_CHASE_WORK*)obj_work;
	if ( rock_work->flag_vib ){
		GMM_PAD_VIB_STOP();
		rock_work->flag_vib = FALSE;
	}

	//ƒNƒŠƒbƒv—LŒø‚É
	obj_work->flag &= ~OBD_OBJECT_NOCLIP;

	//’nŒ`‚ ‚½‚è‚È‚µi‰æ–ÊŠO‚É—‚¿‚Ä‚¢‚­j
	obj_work->move_flag |= OBD_MOVE_NOCOL;

	// ƒƒCƒ“ˆ—
	obj_work->ppFunc = NULL;

}

// ==========================================================================
// gmGmkRockChaseMainFall
/*!
 *	ƒMƒ~ƒbƒN‘åŠâi’ÇÕj—‰ºƒ‚[ƒhƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockChaseMainFall( OBS_OBJECT_WORK *obj_work )
{

	//ƒ‚[ƒh•ÏX
	if ( obj_work->move_flag & OBD_MOVE_UNDER ){
		//Œø‰Ê‰¹
		GmSoundPlaySE("BigRock1");

		//ƒJƒƒ‰U“®
		GmCameraVibrationSet(0,GMD_GMK_ROCK_QUAKE_FALL,0);

		//’ÇÕó‘Ô‚É
		gmGmkRockChaseChangeModeChase( obj_work );
	}
	else{
		//I—¹”»’èiƒvƒŒƒCƒ„‚æ‚è‚àˆê’è‹——£‰º‚É—‰º‚µ‚½ê‡j
		OBS_OBJECT_WORK* player_obj_work = &g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_work;
		if ( player_obj_work->pos.y < obj_work->pos.y-GMD_GMK_ROCK_CHASE_DELETE_TCB_DISTANCE ){
			gmGmkRockChaseChangeModeEnd( obj_work );
		}
	}
}

// ==========================================================================
// gmGmkRockChaseMainChase
/*!
 *	ƒMƒ~ƒbƒN‘åŠâi’ÇÕj’ÇÕƒ‚[ƒhƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockChaseMainChase( OBS_OBJECT_WORK *obj_work )
{
	GMS_GMK_ROCK_CHASE_WORK* rock_work = (GMS_GMK_ROCK_CHASE_WORK*)obj_work;
	const GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );
	const OBS_OBJECT_WORK* player_work = gimmick_work->ene_com.target_obj;
	amAssert( player_work );

	fx32 max_speed = 0;
	fx32 add_speed = 0;
	
	//ƒvƒŒƒCƒ„‚ª‚¢‚é•ûŒü
	fx32 player_dir = player_work->pos.x - obj_work->pos.x;

	//“]‚ª‚é•ûŒü
	GME_GMK_ROCK_DIR_TYPE dir_type = gmGmkRockChaseGetDirType( rock_work );
	if ( dir_type == GMD_GMK_ROCK_DIR_TYPE_INVALID){
		//–¢İ’è‚È‚çİ’è
		if ( obj_work->dir.z <= NNM_DEGtoA16(180) ){
			dir_type = GMD_GMK_ROCK_DIR_TYPE_RIGHT;
		}
		else{
			dir_type = GMD_GMK_ROCK_DIR_TYPE_LEFT;
		}
		gmGmkRockChaseSetDirType( rock_work, dir_type );
	}

	//Œü‚«‚ªˆÙ‚È‚é‚Ì‚Å’Ç‚¢‚©‚¯‚ç‚ê‚È‚¢
	if ( (dir_type == GMD_GMK_ROCK_DIR_TYPE_RIGHT && player_dir < 0)
			|| (dir_type == GMD_GMK_ROCK_DIR_TYPE_LEFT  && player_dir >= 0)
	){
		add_speed = GMD_GMK_ROCK_CHASE_SPEED_ADD_MID;
		max_speed = GMD_GMK_ROCK_CHASE_SPEED_MAX_OFFSET_FAR;
				
		//U“®’â~
		if ( rock_work->flag_vib ){
			GMM_PAD_VIB_STOP();
			rock_work->flag_vib = FALSE;
		}
	}
	//’Ç‚¢‚©‚¯‚ç‚ê‚é
	else{
		fx32 player_speed = MTM_MATH_ABS(player_work->spd_m);

		//ˆÚ“®‘¬“x”»’è
		if ( player_speed < gmGmkRockChaseGetSpeed( rock_work ) ){
			add_speed = GMD_GMK_ROCK_CHASE_SPEED_ADD_SLOW;
			max_speed = GMD_GMK_ROCK_CHASE_SPEED_MAX_SLOW;
		}
		//‹——£•Ê”»’è
		else {
			fx32 length = MTM_MATH_ABS(player_dir);
			fx32 near_length = gmGmkRockChaseGetLength( rock_work );
			fx32 far_length = FX_Mul(near_length, 2*FX32_ONE);
			if ( far_length < GMD_GMK_ROCK_CHASE_DISTANCE_FAR ){
				far_length = GMD_GMK_ROCK_CHASE_DISTANCE_FAR;
			}

			//‰“
			if ( length > far_length ){
				add_speed = GMD_GMK_ROCK_CHASE_SPEED_ADD_FAR;
				max_speed = player_speed + GMD_GMK_ROCK_CHASE_SPEED_MAX_OFFSET_FAR;
				
				//U“®’â~
				if ( rock_work->flag_vib ){
					GMM_PAD_VIB_STOP();
					rock_work->flag_vib = FALSE;
				}
			}
			//’†
			else if ( length > near_length ){
				add_speed = GMD_GMK_ROCK_CHASE_SPEED_ADD_MID;
				max_speed = player_speed + GMD_GMK_ROCK_CHASE_SPEED_MAX_OFFSET_MID;

				//U“®İ’è
				if ( !rock_work->flag_vib ){
					GMM_PAD_VIB_MID_NOEND();
					rock_work->flag_vib = TRUE;
				}
			}
			//‹ß
			else{
				add_speed = GMD_GMK_ROCK_CHASE_SPEED_ADD_NEAR;
				max_speed = player_speed + GMD_GMK_ROCK_CHASE_SPEED_MAX_OFFSET_NEAR;

				//U“®İ’è
				if ( !rock_work->flag_vib ){
					GMM_PAD_VIB_MID_NOEND();
					rock_work->flag_vib = TRUE;
				}
			}
		}
	}

	//‰EŒü‚«
	if ( dir_type == GMD_GMK_ROCK_DIR_TYPE_RIGHT ){
		obj_work->spd_m += add_speed;
		gmGmkRockChaseAddAngleZ( rock_work, MGD_GMK_ROCK_ADD_ANGLE_Z );

		//Å‘å‘¬“x§ŒÀ
		if ( obj_work->spd_m > max_speed ){
			obj_work->spd_m = max_speed;
		}
	}
	//¶Œü‚«
	else {
		add_speed = -add_speed;
		max_speed = -max_speed;
		obj_work->spd_m += add_speed;
		gmGmkRockChaseAddAngleZ( rock_work, -MGD_GMK_ROCK_ADD_ANGLE_Z );

		//Å‘å‘¬“x§ŒÀ
		if ( obj_work->spd_m < max_speed ){
			obj_work->spd_m = max_speed;
		}
	}

	
	//’n–Ê‚ª‚È‚¢ê‡
	if ( !(obj_work->move_flag & OBD_MOVE_UNDER) ){
		//—‰ºó‘Ô‚Ö
		gmGmkRockChaseChangeModeFall( obj_work );
	}
	//’e‚Ş
	else{
		//’n–Ê‚É‚Â‚¢‚Ä‚¢‚é‚Æ‚«
		if ( rock_work->current_bound >= 0 ){
			rock_work->current_bound = 0;
			if (0 == (mtMathRand() % GMD_GMK_ROCK_CHASE_BOUND)){
				s32 bound_value = GMD_GMK_ROCK_CHASE_BOUND_VAL_MIN + (mtMathRand() % GMD_GMK_ROCK_CHASE_BOUND_VAL);
				rock_work->target_bound = -(fx32)bound_value*FX32_ONE;
				rock_work->current_bound -= GMD_GMK_ROCK_CHASE_BOUND_FALL;

				//Œø‰Ê‰¹’â~i‰ñ“]‰¹j
				if ( rock_work->se_handle ){
					GmSoundStopSE( rock_work->se_handle );
				}
			}
		}
		//•‚‚¢‚Ä‚¢‚é‚Æ‚«
		else{
			//—‰º‚·‚é
			if ( rock_work->target_bound > rock_work->current_bound ){
				rock_work->target_bound = 0;
				rock_work->current_bound += GMD_GMK_ROCK_CHASE_BOUND_FALL;
				if ( rock_work->current_bound >= 0 ){
					//Œø‰Ê‰¹i—‰º‰¹j
					GmSoundPlaySE("BigRock1");
	
					//Œø‰Ê‰¹i‰ñ“]‰¹j
					GmSoundPlaySE("BigRock2", rock_work->se_handle);

					//ƒJƒƒ‰U“®
					GmCameraVibrationSet(0,GMD_GMK_ROCK_CHASE_QUAKE,0);
				}
			}
			//•‚‚«ã‚ª‚é
			else{
				rock_work->current_bound -= GMD_GMK_ROCK_CHASE_BOUND_FALL;
			}
		}
	}

}

// ==========================================================================
//‘åŠâi—‰ºj¶¬ŠÇ—
// ==========================================================================
// ==========================================================================
// gmGmkRockManagerInit
/*!
 *	ƒMƒ~ƒbƒN@‘åŠâi—‰ºj¶¬ŠÇ—@‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockManagerInit( OBS_OBJECT_WORK* obj_work )
{
	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->move_flag |= (OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);

	gmGmkRockFallMgrSetUserTimer( obj_work, 0 );

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = gmGmkRockManagerMain;
}

// ==========================================================================
// gmGmkRockManagerMain
/*!
 *	ƒMƒ~ƒbƒN‘åŠâ¶¬ŠÇ—ƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockManagerMain( OBS_OBJECT_WORK* obj_work )
{
	GMS_GMK_ROCK_FALL_MGR_WORK* mgr_work = (GMS_GMK_ROCK_FALL_MGR_WORK*)obj_work;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	s32 create_interval = gmGmkRockFallMgrGetInterval( mgr_work );
	s32 counter = gmGmkRockFallMgrGetUserTimer( obj_work );

	//‘åŠâ¶¬
	if ( counter >= create_interval ){
		gmGmkRockFallMgrSetUserTimer( obj_work, 0 );

		//—‰º‘Ò‚¿ŠÔƒtƒ‰ƒOi¶¬ŠÔŠu‚ª‚ ‚é’ö“x’·‚¢ê‡A—‰º‘Ò‚¿ŠÔ‚ğ“ü‚ê‚éj
		u8 wait_time_flag = 0;
		if ( create_interval >= GMD_GMK_ROCK_FALL_INTERCAL_NO_WAIT ){
			wait_time_flag = 1;
		}

		OBS_OBJECT_WORK* fall_obj_work = GmEventMgrLocalEventBirth(
			GMD_EVENT_ID_NOSET_ROCK_FALL,
			obj_work->pos.x,
			obj_work->pos.y - (gimmick_work->ene_com.eve_rec->top*2*FX32_ONE),
			gimmick_work->ene_com.eve_rec->flag,
			gimmick_work->ene_com.eve_rec->left,
			gimmick_work->ene_com.eve_rec->top,
			gimmick_work->ene_com.eve_rec->width,
			gimmick_work->ene_com.eve_rec->height,
			wait_time_flag );
		fall_obj_work->spd_fall = GMD_GMK_ROCK_FALL_SPEED_FALL;
		fall_obj_work->spd_fall_max = GMD_GMK_ROCK_FALL_SPEED_FALL_MAX;

		//‘åŠâ‚ğx‚¦‚é‘•’u‚ğ“o˜^
		GMS_GMK_ROCK_FALL_WORK* rock_work = (GMS_GMK_ROCK_FALL_WORK*)fall_obj_work;
		rock_work->hook_work = mgr_work->hook_work;
	}

	//ƒ^ƒCƒ}§Œä
	gmGmkRockFallMgrAddUserTimer( obj_work, 1 );
}


// ==========================================================================
//‘åŠâi—‰ºj
// ==========================================================================
// ==========================================================================
// gmGmkRockFallInit
/*!
 *	ƒMƒ~ƒbƒN‘åŠâ—‰ºó‘Ô‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockFallInit( OBS_OBJECT_WORK *obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	//ƒvƒŒƒCƒ„[‚ ‚½‚è
	gmGmkRockSetRectActive( gimmick_work );

	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->move_flag |= OBD_MOVE_FALL | OBD_MOVE_NOCOL;
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	//ZÀ•W
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	//‰Šú‰ñ“]
	obj_work->user_work = (u16)mtMathRand();

	//‰ñ“]—Ê‰Šúİ’è
	GMS_GMK_ROCK_FALL_WORK* rock_work = (GMS_GMK_ROCK_FALL_WORK*)obj_work;
	rock_work->roll = mtMathRand();
	rock_work->roll_d = GMD_GMK_ROCK_FALL_ROLL;
	if ( rock_work->roll%2 ){
		rock_work->roll_d = (u16)(-rock_work->roll_d);
	}

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = gmGmkRockFallMainStart;
	obj_work->ppMove = gmGmkRockMoveFunc;
	obj_work->ppOut = gmGmkRockFallDrawFunc;
}

// ==========================================================================
// gmGmkRockFallMainStart
/*!
 *	ƒMƒ~ƒbƒN‘åŠâi—‰ºj¶¬ƒ‚[ƒhƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockFallMainStart( OBS_OBJECT_WORK *obj_work )
{
	GMS_GMK_ROCK_FALL_WORK* rock_work = (GMS_GMK_ROCK_FALL_WORK*)obj_work;
	const OBS_OBJECT_WORK* obj_work_hook = (OBS_OBJECT_WORK*)rock_work->hook_work;
	amAssert( obj_work_hook );

	//‘•’u‚É‚Â‚­‚Ü‚Å‘Ò‹@
	if ( obj_work_hook->pos.y + GMD_GMK_ROCK_HOOK_POS_OFFSET_Y > obj_work->pos.y ){
		return;
	}

	//ƒGƒtƒFƒNƒg
	GMS_EFFECT_3DES_WORK* effect_work = GmEfctZoneEsCreate(
			obj_work,
			GSD_MAIN_ZONE_TYPE_3,
			GME_EFCT_Z03_IDX_ROCK_01 );
	amAssert( effect_work );
	effect_work->efct_com.obj_work.pos.z = GMD_OBJ_GIMMICK_POS_Z_FRONT;

	//Œø‰Ê‰¹
	GmSoundPlaySE("BigRock4");

	//À•W’²®
	obj_work->pos.y = obj_work_hook->pos.y + GMD_GMK_ROCK_HOOK_POS_OFFSET_Y;

	//ƒJƒƒ‰U“®
	GmCameraVibrationSet( 0, GMD_GMK_ROCK_FALL_QUAKE_FALL, 0 );

	//—‰º’â~
	obj_work->move_flag &= ~OBD_MOVE_FALL;
	obj_work->spd.y = 0;

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkRockFallMainWait;
}

// ==========================================================================
// gmGmkRockFallMainWait
/*!
 *	ƒMƒ~ƒbƒN‘åŠâi—‰ºj‘Ò‹@ƒ‚[ƒhƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockFallMainWait( OBS_OBJECT_WORK *obj_work )
{

	GMS_GMK_ROCK_FALL_WORK* rock_work = (GMS_GMK_ROCK_FALL_WORK*)obj_work;
	OBS_OBJECT_WORK* obj_work_hook = (OBS_OBJECT_WORK*)rock_work->hook_work;
	amAssert( obj_work_hook );

	//—‰º‘Ò‚¿
	++obj_work->user_timer;
	if ( obj_work->user_timer < rock_work->wait_time ){
		return;
	}
	obj_work->user_timer = 0;

	//—‰ºÄŠJ
	obj_work->move_flag |= OBD_MOVE_FALL;

	//‘•’u‚Ìƒ‚[ƒh•ÏX
	gmGmkRockHookkChangeModeActive( obj_work_hook );

	//‘åŠâ‚ğx‚¦‚é‘•’u‚Ì“o˜^‰ğœ
	rock_work->hook_work = NULL;

	//Œø‰Ê‰¹
	GmSoundPlaySE("BigRock5");

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkRockFallMainFallWaitEffect;
}

// ==========================================================================
// gmGmkRockFallMainFallWaitEffect
/*!
 *	ƒMƒ~ƒbƒN‘åŠâi—‰ºj—‰ºƒ‚[ƒhƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockFallMainFallWaitEffect( OBS_OBJECT_WORK *obj_work )
{
	//‰ñ“]
	GMS_GMK_ROCK_FALL_WORK* rock_work = (GMS_GMK_ROCK_FALL_WORK*)obj_work;
	rock_work->roll += rock_work->roll_d;

	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_GMK_ROCK_FALL_EFFECT_WAIT_TIME ){
		return ;
	}
	obj_work->user_timer = 0;

	//ƒGƒtƒFƒNƒg
	amAssert( !rock_work->effect_work );
	GMS_EFFECT_3DES_WORK* effect_work = GmEfctZoneEsCreate(
			obj_work,
			GSD_MAIN_ZONE_TYPE_3,
			GME_EFCT_Z03_IDX_WALL_M_Z3 );
	amAssert( effect_work );
	effect_work->efct_com.obj_work.ppFunc = NULL;
	effect_work->efct_com.obj_work.pos.y -= 64*FX32_ONE;
	effect_work->efct_com.obj_work.pos.z = GMD_OBJ_GIMMICK_POS_Z_FRONT;
	effect_work->efct_com.obj_work.parent_ofst.y = GMD_GMK_ROCK_SIZE_EFFECT_OFFEST_Y;
	
	//ƒ[ƒN‚ÉƒGƒtƒFƒNƒg“o˜^
	rock_work->effect_work = effect_work;

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkRockFallMainFall;
}

// ==========================================================================
// gmGmkRockFallMainFall
/*!
 *	ƒMƒ~ƒbƒN‘åŠâi—‰ºj—‰ºƒ‚[ƒhƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockFallMainFall( OBS_OBJECT_WORK *obj_work )
{
	//—‰ºÄŠJ
	obj_work->move_flag |= OBD_MOVE_FALL;

	//‰ñ“]
	GMS_GMK_ROCK_FALL_WORK* rock_work = (GMS_GMK_ROCK_FALL_WORK*)obj_work;
	rock_work->roll += rock_work->roll_d;
}



// ==========================================================================
//‘åŠâi‘•’uj
// ==========================================================================
// ==========================================================================
// gmGmkRockHookInit
/*!
 *	ƒMƒ~ƒbƒN‘åŠâ‘•’uó‘Ô‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockHookInit( OBS_OBJECT_WORK *obj_work )
{
	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->move_flag |= OBD_MOVE_NOCOL;
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	//ZÀ•W
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z;

	gmGmkRockHookChangeModeWait( obj_work );
}

void gmGmkRockHookChangeModeWait( OBS_OBJECT_WORK *obj_work )
{
	ObjDrawObjectActionSet3DNN( obj_work, IDB_GMK_ROCK_MTN_GMK_ROCK_HOOK_ZNM, 0 );

	obj_work->ppFunc = NULL;
}

void gmGmkRockHookkChangeModeActive( OBS_OBJECT_WORK *obj_work )
{
	ObjDrawObjectActionSet3DNN( obj_work, IDB_GMK_ROCK_MTN_GMK_ROCK_HOOK_OPEN_ZNM, 0 );

	obj_work->ppFunc = gmGmkRockHookMainActive;
}
void gmGmkRockHookMainActive( OBS_OBJECT_WORK *obj_work )
{
	if ( obj_work->disp_flag & OBD_DISP_END ){
		gmGmkRockHookChangeModeWait(obj_work);
	}
}

// ==========================================================================
//’ÇÕ—p
// ==========================================================================

// ==========================================================================
// gmGmkRockChaseSetLength
/*!
 *	’ÇÕ‹——£‚ğİ’è
 *
 *	@param rock_work	[io] ƒƒbƒNƒ[ƒN
 *	@param length		[in] ’ÇÕ‹——£
 */
// ==========================================================================
void gmGmkRockChaseSetLength( GMS_GMK_ROCK_CHASE_WORK* rock_work, fx32 length )
{
	rock_work->length = length;
}

// ==========================================================================
// gmGmkRockChaseGetLength
/*!
 *	’ÇÕ‹——£‚ğæ“¾
 *
 *	@param rock_work	[in] ƒƒbƒNƒ[ƒN
 *
 *	@return ’ÇÕ‹——£
 */
// ==========================================================================
fx32 gmGmkRockChaseGetLength( const GMS_GMK_ROCK_CHASE_WORK* rock_work )
{
	return rock_work->length;
}

// ==========================================================================
// gmGmkRockChaseSetUserWorkSpeed
/*!
 *	’ÇÕ‘¬“x‚ğİ’è
 *
 *	@param rock_work	[io] ƒƒbƒNƒ[ƒN
 *	@param length		[in] ’ÇÕ‘¬“x
 */
// ==========================================================================
void gmGmkRockChaseSetSpeed( GMS_GMK_ROCK_CHASE_WORK* rock_work, fx32 speed )
{
	rock_work->speed = speed;
}

// ==========================================================================
// gmGmkRockChaseGetSpeed
/*!
 *	’ÇÕ‘¬“x‚ğæ“¾
 *
 *	@param rock_work	[in] ƒƒbƒNƒ[ƒN
 *
 *	@return ’ÇÕ‘¬“x
 */
// ==========================================================================
fx32 gmGmkRockChaseGetSpeed( const GMS_GMK_ROCK_CHASE_WORK* rock_work )
{
	return rock_work->speed;
}

// ==========================================================================
// gmGmkRockChaseSetAngleZ
/*!
 *	Z²‰ñ“]—Ê‚ğİ’è
 *
 *	@param rock_work	[io] ƒƒbƒNƒ[ƒN
 *	@param angle_z		[in] Z²‰ñ“]—Ê
 */
// ==========================================================================
void gmGmkRockChaseSetAngleZ( GMS_GMK_ROCK_CHASE_WORK* rock_work, u16 angle_z )
{
	rock_work->angle_z = angle_z;
}

// ==========================================================================
// gmGmkRockChaseAddAngleZ
/*!
 *	Z²‰ñ“]—Ê‚ğ’Ç‰Á
 *
 *	@param rock_work	[io] ƒƒbƒNƒ[ƒN
 *	@param angle_z		[in] Z²‰ñ“]—Ê
 */
// ==========================================================================
void gmGmkRockChaseAddAngleZ( GMS_GMK_ROCK_CHASE_WORK* rock_work, s16 angle_z )
{
	rock_work->angle_z = (u16)(rock_work->angle_z + angle_z);
}

// ==========================================================================
// gmGmkRockChaseGetAngleZ
/*!
 *	Z²‰ñ“]—Ê‚ğæ“¾
 *
 *	@param rock_work	[in] ƒƒbƒNƒ[ƒN
 *
 *	@return Z²‰ñ“]—Ê
 */
// ==========================================================================
u16 gmGmkRockChaseGetAngleZ( const GMS_GMK_ROCK_CHASE_WORK* rock_work )
{
	return rock_work->angle_z;
}


// ==========================================================================
// gmGmkRockChaseSetDir
/*!
 *	‰EŒü‚«ƒtƒ‰ƒO‚ğİ’è
 *
 *	@param rock_work	[io] ƒƒbƒNƒ[ƒN
 *	@param length		[in] Œü‚«ƒ^ƒCƒv
 */
// ==========================================================================
void gmGmkRockChaseSetDirType( GMS_GMK_ROCK_CHASE_WORK* rock_work, GME_GMK_ROCK_DIR_TYPE type )
{
	rock_work->dir_type = type;
}

// ==========================================================================
// gmGmkRockChaseGetDirType
/*!
 *	‰EŒü‚«ƒtƒ‰ƒO‚ğæ“¾
 *
 *	@param rock_work	[in] ƒƒbƒNƒ[ƒN
 *
 *	@return Œü‚«ƒ^ƒCƒv
 */
// ==========================================================================
GME_GMK_ROCK_DIR_TYPE gmGmkRockChaseGetDirType( const GMS_GMK_ROCK_CHASE_WORK* rock_work )
{
	return rock_work->dir_type;
}


// ==========================================================================
//—‰º—pƒ†[ƒU—Ìˆæ
// ==========================================================================



// ==========================================================================
// gmGmkRockFallMgrSetInterval
/*!
 *	—‰ºŠÔŠu‚ğİ’è
 *
 *	@param mgr_work	[io] ŠÇ—ƒ[ƒN
 *	@param interval	[in] —‰ºŠÔŠu
 */
// ==========================================================================
void gmGmkRockFallMgrSetInterval( GMS_GMK_ROCK_FALL_MGR_WORK* mgr_work, s32 interval )
{
	mgr_work->interval = interval;
}

// ==========================================================================
// gmGmkRockFallMgrGetInterval
/*!
 *	—‰ºŠÔŠu‚ğæ“¾
 *
 *	@param mgr_work	[in] ŠÇ—ƒ[ƒN
 *
 *	@return —‰ºŠÔŠu
 */
// ==========================================================================
s32 gmGmkRockFallMgrGetInterval( const GMS_GMK_ROCK_FALL_MGR_WORK* mgr_work )
{
	return mgr_work->interval;
}

// ==========================================================================
// gmGmkRockFallMgrSetUserTimer
/*!
 *	ƒ†[ƒUƒ^ƒCƒ}‚ğİ’è
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param count	[in] Œo‰ßŠÔ
 */
// ==========================================================================
void gmGmkRockFallMgrSetUserTimer( OBS_OBJECT_WORK* obj_work, s32 count )
{
	obj_work->user_timer = count;
}

// ==========================================================================
// gmGmkRockFallMgrAddUserTimer
/*!
 *	ƒ†[ƒUƒ^ƒCƒ}‚ğ’Ç‰Á
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param count	[in] Œo‰ßŠÔ
 */
// ==========================================================================
void gmGmkRockFallMgrAddUserTimer( OBS_OBJECT_WORK* obj_work, s32 count )
{
	obj_work->user_timer += count;
}

// ==========================================================================
// gmGmkRockFallMgrGetUserTimer
/*!
 *	ƒ†[ƒUƒ^ƒCƒ}‚ğæ“¾
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return Œo‰ßŠÔ
 */
// ==========================================================================
s32 gmGmkRockFallMgrGetUserTimer( const OBS_OBJECT_WORK* obj_work )
{
	return obj_work->user_timer;
}



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
