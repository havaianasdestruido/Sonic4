// ==========================================================================
/*!
  @file gmGmkRockRide.cpp
  @brief ƒMƒ~ƒbƒN ‘åŠâiŒXÎj

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkRockRide.cpp 2 2011-04-11 05:21:26Z thamada $
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
 *		flag 1:ˆÚ“®‚·‚éŒü‚«
 * @~~~~~~~~~¡ on  :¶Œü‚«
 * @~~~~~~~~~  off :‰EŒü‚«
 * 
 * ƒtƒ‰ƒOw’è‚Ì•ûŒü‚Ö‰Ÿ‚·‚±‚Æ‚ª‰Â”\
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"
#include "gmEventTbl.h"
#include "gmPlySeqGmk.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectZone.h"
#include "gmSound.h"
#include "gmPadVib.h"

#include "gmGmkRockRide.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "common/model/gmk_rock_mdl.hmb"


//----- Definitions ---------------------------------------------------------

#define GMD_GMK_ROCK_RIDE_START_SPEED_ADD			(0x000000e0L)	//ŠJn‰‰o‰Á‘¬“x
#define GMD_GMK_ROCK_RIDE_START_SPEED				(0x00003000L)	//ŠJn‰‰oI—¹‘¬“x

#define GMD_GMK_ROCK_RIDE_SLOP						(0x00100*3/4)	//ƒXƒ[ƒv•â³
#define GMD_GMK_ROCK_RIDE_SLOP_MAX					(0x0f000)		//ƒXƒ[ƒvÅ‘å‘¬“x

#define GMD_GMK_ROCK_RIDE_LEFT						( 1 << 0)		// ¶‚©‚çƒtƒ‰ƒO

#define GMD_GMK_ROCK_RIDE_SIZE_HIT_RECT				(40)			//ƒqƒbƒg‹éŒ`ƒTƒCƒY
#define GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT			(16)			//’nŒ`‹éŒ`ƒTƒCƒY
#define GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT_BOTTOM	(16)			//’nŒ`‹éŒ`ƒTƒCƒY
#define GMD_GMK_ROCK_RIDE_SIZE_EFFECT_OFFEST_Y		((GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT_BOTTOM+8)*FX32_ONE)	//’nŒ`ƒGƒtƒFƒNƒgƒIƒtƒZƒbƒg
#define GMD_GMK_ROCK_RIDE_SIZE_EFFECT_OFFEST_Z		(32*FX32_ONE)	//’nŒ`ƒGƒtƒFƒNƒgƒIƒtƒZƒbƒg

#define GMD_GMK_ROCK_RIDE_TXB_INDEX	(0)	//txbƒCƒ“ƒfƒNƒX

#define GMD_GMK_ROCK_RIDE_SE_MAX_SPEED				(6)	//‰ñ“]Œø‰Ê‰¹—p‚ÌÅ‘åƒXƒs[ƒh

#define GMD_GMK_ROCK_RIDE_INTERVAL_VIB	(30)	//U“®ŠÔŠu
#define GMD_GMK_ROCK_RIDE_TIME_VIB		(10)	//U“®ŠÔ

//‘åŠâƒ[ƒN
typedef struct tag_GMS_GMK_ROCK_WORK{
	GMS_ENEMY_3D_WORK enemy_work;			//ƒGƒlƒ~[ƒ[ƒN
	GMS_EFFECT_3DES_WORK* effect_work;	//ƒGƒtƒFƒNƒgƒ[ƒN

	GSS_SND_SE_HANDLE* se_handle;		//SEƒnƒ“ƒhƒ‹
	s32 vib_timer;						//U“®—pƒ^ƒCƒ}[
}GMS_GMK_ROCK_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static GMS_ENEMY_3D_WORK* gmGmkRockRideLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type );

static void gmGmkRockRideMoveFunc( OBS_OBJECT_WORK* work );
static void gmGmkRockRideDrawFunc( OBS_OBJECT_WORK* obj_work );
static void gmGmkRockRideTcbDest( MTS_TASK_TCB *tcb );

static void gmGmkRockRideSetUserTimerAngleZ( OBS_OBJECT_WORK* obj_work, u16 angle_z );
static void gmGmkRockRideAddUserTimerAngleZ( OBS_OBJECT_WORK* obj_work, s16 angle_z );
static u16 gmGmkRockRideGetUserTimerAngleZ( const OBS_OBJECT_WORK* obj_work );

static void gmGmkRockRideWaitInit( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockRideWaitMain( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockRideWaitSetRect( GMS_ENEMY_3D_WORK* gimmick_work );
static void gmGmkRockRideWaitDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect );

static void gmGmkRockRideStartInit( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockRideStartMain( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockRideStartSetRect( GMS_ENEMY_3D_WORK* gimmick_work );

static void gmGmkRockRideRollInit( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockRideRollMain( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockRideRollMainNoPlayer( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockRideRollSetRect( GMS_ENEMY_3D_WORK* gimmick_work );
static void gmGmkRockRideRollDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect );

static void gmGmkRockRideStopInit( OBS_OBJECT_WORK *obj_work );
static void gmGmkRockRideStopSetRect( GMS_ENEMY_3D_WORK* gimmick_work );

//---------------------------------------------------------
//ƒ†[ƒUƒ[ƒN
//---------------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

static OBS_ACTION3D_NN_WORK* g_gm_gmk_rock_ride_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkRockRideBuild
/*!
 *	ƒMƒ~ƒbƒN ‘åŠâ ƒf[ƒ^\’z
 */
// ==========================================================================
void GmGmkRockRideBuild(void)
{
	AMS_AMB_HEADER* amb_texture = (AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_ROCK_TEX );
	amAssert( amb_texture );
	amConvertAddress(amb_texture);
	void* txb = amBindGet(amb_texture, GMD_GMK_ROCK_RIDE_TXB_INDEX);
	amAssert( txb );

	g_gm_gmk_rock_ride_obj_3d_list = GmGameDBuildRegBuildModel(
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_ROCK_MODEL ),
			amb_texture,
			0,	//draw_flag
			txb	);
}

// ==========================================================================
// GmGmkRockRideFlush
/*!
 *	ƒMƒ~ƒbƒN ‘åŠâ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
void GmGmkRockRideFlush(void)
{
	AMS_AMB_HEADER* amb_header = (AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_ROCK_MODEL );

	GmGameDBuildRegFlushModel( g_gm_gmk_rock_ride_obj_3d_list, amb_header->file_num );
	g_gm_gmk_rock_ride_obj_3d_list = NULL;
}

// ==========================================================================
// GmGmkRockRideInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”i‘åŠâŒXÎj
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkRockRideInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkRockRideLoadObj( eve_rec, pos_x, pos_y, type );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkRockRideWaitInit( obj_work );

	return obj_work;
}

//----- Local Functions -----------------------------------------------------

// ==========================================================================
// gmGmkRockRideLoadObj
/*!
 *	ƒMƒ~ƒbƒN“Ç‚İ‚İi‘åŠâj
 *
 *	@param eve_rec	[in] ƒCƒxƒ“ƒgƒŒƒR[ƒh
 *	@param pos_x	[in] À•WX
 *	@param pos_y	[in] À•WY
 *	@param type		[in] ƒ^ƒCƒv
 *
 *	@return ƒ[ƒN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkRockRideLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type )
{

	UNREFERENCED_PARAMETER(type);

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_GMK_ROCK_WORK* rock_work = (GMS_GMK_ROCK_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec, 
			pos_x, 
			pos_y, 
			sizeof(GMS_GMK_ROCK_WORK), 
			"GMK_ROCK_RIDE");
	amAssert( rock_work );

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = &rock_work->enemy_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* obj_work = &rock_work->enemy_work.ene_com.obj_work;
	amAssert( obj_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	//-------------------------------------------------
	// ƒ‚ƒfƒ‹‰Šú‰»
	//-------------------------------------------------
	ObjObjectCopyAction3dNNModel(
		obj_work,
		&g_gm_gmk_rock_ride_obj_3d_list[IDB_GMK_ROCK_MDL_GMK_ROCK_ZNO],
		&gimmick_work->obj_3d);
	
#if _IPHONE
	// ê—pƒ‰ƒCƒgİ’è
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return gimmick_work;
}

// ==========================================================================
// gmGmkRockRideMoveFunc
/*!
 *	ƒMƒ~ƒbƒN‘åŠâˆÚ“®ŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideMoveFunc( OBS_OBJECT_WORK* obj_work )
{
	ObjObjectMove( obj_work );
}

// ==========================================================================
// gmGmkRockRideDrawFunc
/*!
 *	ƒMƒ~ƒbƒN‘åŠâ•`‰æŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideDrawFunc( OBS_OBJECT_WORK* obj_work )
{
	VecU16 dir = obj_work->dir;

	//‰ñ“]‚³‚¹‚é
	u16 roll = gmGmkRockRideGetUserTimerAngleZ(obj_work);
	if ( obj_work->spd_m < 0 ){
		obj_work->dir.z = roll;
	}
	else{
		obj_work->dir.z = roll;
	}
	u16 rot_x = (u16)obj_work->user_work;
	obj_work->dir.x = rot_x; 

	//•`‰æ
	ObjDrawActionSummary( obj_work );
/*
	//•`‰æ
	ObjDrawActionSummary( obj_work );
*/	
	obj_work->dir = dir;
}

// ==========================================================================
// gmGmkRockRideTcbDest
/*!
 * OBJƒfƒXƒgƒ‰ƒNƒ^
 *
 * @param tcb TCB
 */
// ==========================================================================
void gmGmkRockRideTcbDest( MTS_TASK_TCB *tcb )
{
	GMS_GMK_ROCK_WORK* rock_work = (GMS_GMK_ROCK_WORK*)mtTaskGetTcbWork(tcb);

	//Œø‰Ê‰¹ƒnƒ“ƒhƒ‹‰ğ•ú
	if ( rock_work->se_handle ){
		GmSoundStopSE( rock_work->se_handle );
		GsSoundFreeSeHandle( rock_work->se_handle );
		rock_work->se_handle = NULL;
	}

	//ƒIƒuƒWƒFƒNƒg‰ğ•ú
	GmEnemyDefaultExit( tcb );
}

// ==========================================================================
// gmGmkRockRideSetUserTimerAngleZ
/*!
 *	ƒ†[ƒUƒ^ƒCƒ}‚ğZ²‰ñ“]—Ê‚Æ‚µ‚Äİ’è
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param length	[in] Z²‰ñ“]—Ê
 */
// ==========================================================================
void gmGmkRockRideSetUserTimerAngleZ( OBS_OBJECT_WORK* obj_work, u16 angle_z )
{
	obj_work->user_timer = angle_z;
}

// ==========================================================================
// gmGmkRockRideAddUserTimerAngleZ
/*!
 *	ƒ†[ƒUƒ^ƒCƒ}‚ğZ²‰ñ“]—Ê‚Æ‚µ‚Ä’Ç‰Á
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param length	[in] Z²‰ñ“]—Ê
 */
// ==========================================================================
void gmGmkRockRideAddUserTimerAngleZ( OBS_OBJECT_WORK* obj_work, s16 angle_z )
{
	obj_work->user_timer += angle_z;
}

// ==========================================================================
// gmGmkRockRideGetUserTimerAngleZ
/*!
 *	ƒ†[ƒUƒ^ƒCƒ}‚ğZ²‰ñ“]—Ê‚Æ‚µ‚Äæ“¾
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return Z²‰ñ“]—Ê
 */
// ==========================================================================
u16 gmGmkRockRideGetUserTimerAngleZ( const OBS_OBJECT_WORK* obj_work )
{
	return (u16)obj_work->user_timer;
}



// ==========================================================================
// gmGmkRockRideWaitInit
/*!
 *	ƒMƒ~ƒbƒN‘åŠâ‘Ò‚¿ó‘Ô‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideWaitInit( OBS_OBJECT_WORK *obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	//ƒvƒŒƒCƒ„[‚ ‚½‚è
	gmGmkRockRideWaitSetRect( gimmick_work );

	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->flag |= OBD_OBJECT_B;
	obj_work->move_flag |= (OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	//‘¬“xƒŠƒZƒbƒg
	obj_work->spd_m = 0;

	//ƒ^[ƒQƒbƒgİ’è
	gimmick_work->ene_com.target_obj = &g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_work;

	//‰Šú‰ñ“]
	u16	rand_angle = (u16)mtMathRand();
	gmGmkRockRideSetUserTimerAngleZ( obj_work, rand_angle );
	obj_work->user_work = (u32)mtMathRand();

	//SEƒnƒ“ƒhƒ‹Šm•Û
	GMS_GMK_ROCK_WORK* rock_work = (GMS_GMK_ROCK_WORK*)obj_work;
	rock_work->se_handle = GsSoundAllocSeHandle();

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = gmGmkRockRideWaitMain;
	obj_work->ppMove = NULL;
	obj_work->ppOut = gmGmkRockRideDrawFunc;

	//TCBƒfƒXƒgƒ‰ƒNƒ^İ’è
	mtTaskChangeTcbDestructor( obj_work->tcb, gmGmkRockRideTcbDest );
}

// ==========================================================================
// gmGmkRockRideWaitMain
/*!
 *	ƒMƒ~ƒbƒN‘åŠâiŒXÎj‘Ò‚¿ó‘ÔƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideWaitMain( OBS_OBJECT_WORK *obj_work )
{
	UNREFERENCED_PARAMETER(obj_work);
}

// ==========================================================================
// gmGmkRockRideWaitDefFunc
/*!
 *	ƒMƒ~ƒbƒN‘åŠâ‘Ò‹@‹éŒ`ŠÖ”
 *
 *	@param own_rect	[in] ©g‚Ì‹éŒ`ƒ[ƒN
 *	@param target_rect	[in] ‘Šè‚Ì‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideWaitDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect )
{
	amAssert( own_rect );
	amAssert( target_rect );

	//ƒMƒ~ƒbƒNƒ[ƒN
	OBS_OBJECT_WORK* own_obj_work = own_rect->parent_obj;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)own_obj_work;
	amAssert( gimmick_work );
	GMS_ENEMY_COM_WORK* com_work = (GMS_ENEMY_COM_WORK*)&gimmick_work->ene_com;
	amAssert( com_work );

	OBS_OBJECT_WORK* target_obj_work = target_rect->parent_obj;
	amAssert( target_obj_work );
	
	//ƒvƒŒƒCƒ„ˆÈŠO‚Í”»’è‚µ‚È‚¢
	if ( target_obj_work->obj_type != GMD_OBJTYPE_PLAYER ){
		return;
	}

	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)target_obj_work;	

	if ( target_obj_work->move_flag & OBD_MOVE_UNDER ){
		//Œü‚«ƒtƒ‰ƒO
		if (gimmick_work->ene_com.eve_rec->flag ) {
			if ( own_obj_work->pos.x >= target_obj_work->pos.x ){
				return;
			}
		}
		else{
			if ( own_obj_work->pos.x <= target_obj_work->pos.x ){
				return;
			}
		}

		//Šâæ‚èŠJn
		GmPlySeqInitRockRideStart( player_work, com_work );

		//ŠJnó‘Ô‚Ö
		gmGmkRockRideStartInit( own_obj_work );
	}
	else{
		//‹——£”»’è
		fx32 distance_x = own_obj_work->pos.x - target_obj_work->pos.x;	
		fx32 distance_y = own_obj_work->pos.y - target_obj_work->pos.y;	
		fx32 distance = FX_Mul(distance_x, distance_x) + FX_Mul(distance_y, distance_y);
		if ( distance > 56*56*FX32_ONE ){
			return;
		}

		//ƒWƒƒƒ“ƒv’†‚¶‚á‚È‚¢‚Æ‚«
		if ( player_work->seq_state != GME_PLY_SEQ_STATE_JUMP
				&& player_work->seq_state != GME_PLY_SEQ_STATE_JUMPDASH
				&& player_work->seq_state != GME_PLY_SEQ_STATE_FALL
				&& player_work->seq_state != GME_PLY_SEQ_STATE_GMK_SPRINGJUMP
		){
			return;
		}

		//ƒvƒŒƒCƒ„ƒV[ƒPƒ“ƒX
		fx32 speed_x = -(56*FX32_ONE-MTM_MATH_ABS(distance_x))/30;
		fx32 speed_y = 0;
		if ( own_obj_work->pos.x < target_obj_work->pos.x ){
			speed_x = -speed_x;
		}
		GmPlySeqInitPinballAir(
				player_work, 
				speed_x, 
				speed_y, 
				60,
				TRUE,
				0);
	}
}

// ==========================================================================
// gmGmkRockRideWaitSetRect
/*!
 *	ƒMƒ~ƒbƒN‘åŠâ‘Ò‹@‹éŒ`İ’è
 *
 *	@param gimmick_work	[in] ƒMƒ~ƒbƒNƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideWaitSetRect( GMS_ENEMY_3D_WORK* gimmick_work )
{
	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	ObjRectWorkZSet(rect_work,
						-48, -56, -500,
						48, 56, 500);
	rect_work->flag &= ~OBD_RECT_OUT;
	
	//–hŒä—p
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	rect_work->ppDef = gmGmkRockRideWaitDefFunc;
}

// ==========================================================================
// gmGmkRockRideStartInit
/*!
 *	ƒMƒ~ƒbƒN‘åŠâiŒXÎjŠJnó‘Ô‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideStartInit( OBS_OBJECT_WORK *obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	//ƒvƒŒƒCƒ„[‚ ‚½‚è
	gmGmkRockRideStartSetRect( gimmick_work );

	//’nŒ`‚ ‚½‚è
	ObjObjectFieldRectSet( 
			obj_work, 
			-GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT,
			-GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT,
			GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT,
			GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT_BOTTOM );

	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->flag &= ~(OBD_OBJECT_B);
	obj_work->move_flag &= ~(OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL );
	obj_work->move_flag |= OBD_MOVE_DIR | OBD_MOVE_FALL;
	obj_work->move_flag &= ~(OBD_MOVE_SLOPE);
	//‘¬“xİ’è
	obj_work->spd_m = 0;

	//Œø‰Ê‰¹i‰ñ“]‰¹j
	GMS_GMK_ROCK_WORK* rock_work = (GMS_GMK_ROCK_WORK*)obj_work;
	GmSoundPlaySE("BigRock3", rock_work->se_handle);

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = gmGmkRockRideStartMain;
	obj_work->ppMove = gmGmkRockRideMoveFunc;
	obj_work->ppOut = gmGmkRockRideDrawFunc;
}

// ==========================================================================
// gmGmkRockRideStartMain
/*!
 *	ƒMƒ~ƒbƒN‘åŠâiŒXÎjŠJnó‘ÔƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideStartMain( OBS_OBJECT_WORK *obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );
	const OBS_OBJECT_WORK* player_obj_work = gimmick_work->ene_com.target_obj;
	amAssert( player_obj_work );

	gmGmkRockRideAddUserTimerAngleZ( obj_work, (s16)(obj_work->spd_m >>4) );

	fx32 add_speed = GMD_GMK_ROCK_RIDE_START_SPEED_ADD;
	if ( obj_work->pos.x < player_obj_work->pos.x ){
		add_speed = -add_speed;
	}

	obj_work->spd_m += add_speed;
	if ( MTM_MATH_ABS(obj_work->spd_m) > GMD_GMK_ROCK_RIDE_START_SPEED ){
		gmGmkRockRideRollInit( obj_work );
	}

	//Œø‰Ê‰¹
	Float speed_se = FX_FX32_TO_F32(FX_Div(MTM_MATH_ABS(obj_work->spd_m), GMD_GMK_ROCK_RIDE_SE_MAX_SPEED));
	if ( speed_se > 1.0f ){
		speed_se = 1.0f;
	}
	GMS_GMK_ROCK_WORK* rock_work = (GMS_GMK_ROCK_WORK*)obj_work;
	if ( rock_work->se_handle ){
		rock_work->se_handle->au_player->SetAisac("Speed", speed_se);
	}
}

// ==========================================================================
// gmGmkRockRideStartSetRect
/*!
 *	ƒMƒ~ƒbƒN‘åŠâŠJn‹éŒ`İ’è
 *
 *	@param gimmick_work	[in] ƒMƒ~ƒbƒNƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideStartSetRect( GMS_ENEMY_3D_WORK* gimmick_work )
{
	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];

	//–hŒä—p
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_NOHIT, GMD_OBJ_RECT_DEF_POWER_INVINCIBLE);
	rect_work->ppDef = NULL;
}


// ==========================================================================
// gmGmkRockRideRollInit
/*!
 *	ƒMƒ~ƒbƒN‘åŠâiŒXÎj‰ñ“]ó‘Ô‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideRollInit( OBS_OBJECT_WORK *obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	//ƒvƒŒƒCƒ„[‚ ‚½‚è
	gmGmkRockRideRollSetRect( gimmick_work );

	//’nŒ`‚ ‚½‚è
	ObjObjectFieldRectSet( 
			obj_work, 
			-GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT,
			-GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT,
			GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT, 
			GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT_BOTTOM );

	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->flag &= ~(OBD_OBJECT_B);
	obj_work->move_flag &= ~(OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);
	obj_work->move_flag |= OBD_MOVE_DIR | OBD_MOVE_SLOPE | OBD_MOVE_FALL;

	//‘¬“xİ’è
	obj_work->spd_slope = GMD_GMK_ROCK_RIDE_SLOP;
	obj_work->spd_slope_max = GMD_GMK_ROCK_RIDE_SLOP_MAX;

	//ZÀ•W
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_FRONT;

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = gmGmkRockRideRollMainNoPlayer;
	obj_work->ppMove = gmGmkRockRideMoveFunc;
	obj_work->ppOut = gmGmkRockRideDrawFunc;
	
	//ƒGƒtƒFƒNƒg
	GMS_GMK_ROCK_WORK* rock_work = (GMS_GMK_ROCK_WORK*)obj_work;
	if ( !rock_work->effect_work ){
		//ì¬
		GMS_EFFECT_3DES_WORK* effect_work = GmEfctZoneEsCreate(
				obj_work,
				GSD_MAIN_ZONE_TYPE_3,
				GME_EFCT_Z03_IDX_ROCK_02 );
		amAssert( effect_work );
		effect_work->efct_com.obj_work.ppFunc = NULL;
		effect_work->efct_com.obj_work.parent_ofst.z = GMD_GMK_ROCK_RIDE_SIZE_EFFECT_OFFEST_Y;
		effect_work->efct_com.obj_work.parent_ofst.y = GMD_GMK_ROCK_RIDE_SIZE_EFFECT_OFFEST_Z;
		
		//ƒ[ƒN‚É“o˜^
		rock_work->effect_work = effect_work;
	}
}

// ==========================================================================
// gmGmkRockRideRollMain
/*!
 *	ƒMƒ~ƒbƒN‘åŠâiŒXÎj‰ñ“]ó‘ÔƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideRollMain( OBS_OBJECT_WORK *obj_work )
{
	gmGmkRockRideAddUserTimerAngleZ( obj_work, (s16)(obj_work->spd_m >>4) );

	//Œø‰Ê‰¹
	Float speed_se = FX_FX32_TO_F32(FX_Div(MTM_MATH_ABS(obj_work->spd_m), GMD_GMK_ROCK_RIDE_SE_MAX_SPEED));
	if ( speed_se > 1.0f ){
		speed_se = 1.0f;
	}
	GMS_GMK_ROCK_WORK* rock_work = (GMS_GMK_ROCK_WORK*)obj_work;
	if ( rock_work->se_handle ){
		rock_work->se_handle->au_player->SetAisac("Speed", speed_se);
	}

	if ( obj_work->move_flag & OBD_MOVE_FRONT
		|| obj_work->move_flag & OBD_MOVE_BACK
	){
		gmGmkRockRideStopInit( obj_work );
	}

	//U“®
	if ( rock_work->vib_timer%GMD_GMK_ROCK_RIDE_INTERVAL_VIB == 0 ){
		//U“®
		GMM_PAD_VIB_SMALL_TIME( GMD_GMK_ROCK_RIDE_TIME_VIB );
	}
	++rock_work->vib_timer;

	//ƒvƒŒƒCƒ„‚ªŠâæ‚èó‘Ô‚Å‚È‚¯‚ê‚Îˆ—•ÏX
	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	if ( player_work->seq_state != GME_PLY_SEQ_STATE_GMK_ROCK_RIDE ){
		obj_work->ppFunc = gmGmkRockRideRollMainNoPlayer;

		//U“®’â~
		GMM_PAD_VIB_STOP();
		rock_work->vib_timer = 0;

		//ZÀ•W•ÏX
		obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK-32*FX32_ONE;
	}
}

// ==========================================================================
// gmGmkRockRideRollMainNoPlayer
/*!
 *	ƒMƒ~ƒbƒN‘åŠâiŒXÎj‰ñ“]ó‘ÔƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideRollMainNoPlayer( OBS_OBJECT_WORK *obj_work )
{
	gmGmkRockRideAddUserTimerAngleZ( obj_work, (s16)(obj_work->spd_m >>4) );

	//Œø‰Ê‰¹
	Float speed_se = FX_FX32_TO_F32(FX_Div(MTM_MATH_ABS(obj_work->spd_m), GMD_GMK_ROCK_RIDE_SE_MAX_SPEED));
	if ( speed_se > 1.0f ){
		speed_se = 1.0f;
	}
	GMS_GMK_ROCK_WORK* rock_work = (GMS_GMK_ROCK_WORK*)obj_work;
	if ( rock_work->se_handle ){
		rock_work->se_handle->au_player->SetAisac("Speed", speed_se);
	}

	if ( obj_work->move_flag & OBD_MOVE_FRONT
		|| obj_work->move_flag & OBD_MOVE_BACK
	){
		gmGmkRockRideStopInit( obj_work );
	}
}

// ==========================================================================
// gmGmkRockRideRollDefFunc
/*!
 *	ƒMƒ~ƒbƒN‘åŠâ‰ñ“]‹éŒ`ŠÖ”
 *
 *	@param own_rect	[in] ©g‚Ì‹éŒ`ƒ[ƒN
 *	@param target_rect	[in] ‘Šè‚Ì‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideRollDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect )
{
	amAssert( own_rect );
	amAssert( target_rect );

	//ƒMƒ~ƒbƒNƒ[ƒN
	OBS_OBJECT_WORK* own_obj_work = own_rect->parent_obj;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)own_obj_work;
	amAssert( gimmick_work );
	GMS_ENEMY_COM_WORK* com_work = (GMS_ENEMY_COM_WORK*)&gimmick_work->ene_com;
	amAssert( com_work );

	OBS_OBJECT_WORK* target_obj_work = target_rect->parent_obj;
	amAssert( target_obj_work );
	
	//ƒvƒŒƒCƒ„ˆÈŠO‚Í”»’è‚µ‚È‚¢
	if ( target_obj_work->obj_type != GMD_OBJTYPE_PLAYER ){
		return;
	}
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)target_obj_work;

	//Šâæ‚èŠJn
	GmPlySeqInitRockRide( player_work, com_work );
	own_rect->ppDef = gmGmkRockRideRollDefFunc;
	own_obj_work->ppFunc = gmGmkRockRideRollMain;

	//ZÀ•W•ÏX
	own_obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_FRONT;
}

// ==========================================================================
// gmGmkRockRideRollSetRect
/*!
 *	ƒMƒ~ƒbƒN‘åŠâ‰ñ“]‹éŒ`İ’è
 *
 *	@param gimmick_work	[in] ƒMƒ~ƒbƒNƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideRollSetRect( GMS_ENEMY_3D_WORK* gimmick_work )
{
	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	ObjRectWorkZSet(rect_work,
						-48, -48, -500,
						48, 48, 500);
	rect_work->flag |= OBD_RECT_OUT;

	//–hŒä—p
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	rect_work->ppDef = gmGmkRockRideRollDefFunc;
}


// ==========================================================================
// gmGmkRockRideStopInit
/*!
 *	ƒMƒ~ƒbƒN‘åŠâiŒXÎjI—¹ó‘Ô‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideStopInit( OBS_OBJECT_WORK *obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	//ƒvƒŒƒCƒ„[‚ ‚½‚è
	gmGmkRockRideStopSetRect( gimmick_work );

	//’nŒ`‚ ‚½‚è
	ObjObjectFieldRectSet( 
			obj_work, 
			-GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT, 
			-GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT, 
			GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT, 
			GMD_GMK_ROCK_RIDE_SIZE_FIELD_RECT_BOTTOM );

	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->flag &= ~(OBD_OBJECT_B);
	obj_work->move_flag |= OBD_MOVE_NOCOL;
	obj_work->move_flag &= ~OBD_MOVE_UNDER;

	//‘¬“xİ’è
	obj_work->spd_slope = 0;
	obj_work->spd_slope_max = 0;
	obj_work->spd_m = 0;

	//Œø‰Ê‰¹’â~i‰ñ“]‰¹j
	GMS_GMK_ROCK_WORK* rock_work = (GMS_GMK_ROCK_WORK*)obj_work;
	if ( rock_work->se_handle ){
		GmSoundStopSE( rock_work->se_handle );
	}

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = NULL;

	//ƒGƒtƒFƒNƒg
	if ( rock_work->effect_work ){
		ObjDrawKillAction3DES( (OBS_OBJECT_WORK*)rock_work->effect_work );
	}

	//U“®’â~
	GMM_PAD_VIB_STOP();
}

// ==========================================================================
// gmGmkRockRideStopSetRect
/*!
 *	ƒMƒ~ƒbƒN‘åŠâI—¹‹éŒ`İ’è
 *
 *	@param gimmick_work	[in] ƒMƒ~ƒbƒNƒ[ƒN
 */
// ==========================================================================
void gmGmkRockRideStopSetRect( GMS_ENEMY_3D_WORK* gimmick_work )
{
	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->flag |= OBD_RECT_OUT;

	//–hŒä—p
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_NOHIT, GMD_OBJ_RECT_DEF_POWER_INVINCIBLE);
	rect_work->ppDef = NULL;
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
