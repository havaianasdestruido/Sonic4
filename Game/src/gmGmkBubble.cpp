// ==========================================================================
/*!
  @file gmGmkBubble.cpp
  @brief ÉMÉ~ÉbÉN ëßåpÇ¨ÇÃñA

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkBubble.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 *	GMS_EVE_RECORD_EVENT
 *		left		: ñAê∂ê¨ÇÃä‘äu
 *		top			: ê∂ê¨ë“ÇøèÛë‘ÇÃéûä‘
 *		width		: Ç»Çµ
 *		height		: Ç»Çµ
 *
 *		flag		: Ç»Çµ
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmPlySeqGmk.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectZone.h"
#include "gmPlyEfct.h"
#include "gmPadVib.h"

#include "gmGmkBubble.h"

// ÉfÅ[É^ÉwÉbÉ_
//#include "common/model/gmk_bubble_mdl.hmb"


//----- Definitions ---------------------------------------------------------

//ñAÉÇÅ[Éh
enum GME_GMK_BUBBLE_MODE{
	GMD_GMK_BUBBLE_MODE_WAIT = 0,		//ñAê∂ê¨ë“Çø
	GMD_GMK_BUBBLE_MODE_LADY,			//ñAê∂ê¨
	GMD_GMK_BUBBLE_MODE_START,			//ñAê∂ê¨å„
	GMD_GMK_BUBBLE_MODE_HIT_PLAYER,		//ñAÇ™ÉvÉåÉCÉÑÇ…Ç†ÇΩÇ¡ÇΩèÍçá
	GMD_GMK_BUBBLE_MODE_WATER_LEVEL,	//ñAÇ™êÖñ Ç…íBÇµÇΩèÍçá

	GMD_GMK_BUBBLE_MODE_NUM
};

#define GMD_GMK_BUBBLE_INTERVAL_EFFECT_START	(5*60)			//ÉGÉtÉFÉNÉgê∂ê¨ä‘äuÅiñAê∂ê¨å„Åj
#define GMD_GMK_BUBBLE_SPEED_X_ADD				(0x00000080L)	//ñAà⁄ìÆó ÅiXâ¡ë¨Åj
#define GMD_GMK_BUBBLE_SPEED_X_MAX				(0x00000800L)	//ñAà⁄ìÆó ÅiXç≈ëÂílÅj
#define GMD_GMK_BUBBLE_SPEED_Y					(0x00000800L)	//ñAà⁄ìÆó ÅiYÅj
#define GMD_GMK_BUBBLE_FRAME_HIT_DELETE			(21)			//ÉqÉbÉgéûè¡Ç¶ÇÈÇÃë“ÇøÉtÉåÅ[ÉÄêî

#define GMD_GMK_BUBBLE_OFFSET_X					( 4 )	//ñAÇ©ÇÁÉvÉåÉCÉÑÇÃäÁÇ÷ÇÃÉIÉtÉZÉbÉg
#define GMD_GMK_BUBBLE_OFFSET_Y					( 10 )	//ñAÇ©ÇÁÉvÉåÉCÉÑÇÃäÁÇ÷ÇÃÉIÉtÉZÉbÉg
#define GMD_GMK_BUBBLE_HIT_LENGTH				( 8 )	//ÉvÉåÉCÉÑÇ∆ÉqÉbÉgÇ∑ÇÈãóó£

#define GMD_GMK_BUBBLE_HIT_EFFECT_NUM				( 6 )	//ÉqÉbÉgéûÉGÉtÉFÉNÉgê∂ê¨êî

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static GMS_ENEMY_3D_WORK* gmGmkBubbleLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type );

static u16 gmGmkBubbleGameSystemGetWaterLevel( void );

//---------------------------------------------------------
//ê∂ê¨ä«óù
//---------------------------------------------------------
static void gmGmkBubbleManagerInit( OBS_OBJECT_WORK *obj_work );
static void gmGmkBubbleManagerMainWait( OBS_OBJECT_WORK* obj_work );
static void gmGmkBubbleManagerMain( OBS_OBJECT_WORK* obj_work );
static void gmGmkBubbleManagerEffectMain( OBS_OBJECT_WORK* obj_work );

//---------------------------------------------------------
//ñA
//---------------------------------------------------------
static void gmGmkBubbleInit( GMS_EFFECT_3DES_WORK* effect_work );
static void gmGmkBubbleDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect );

static void gmGmkBubbleMainMoveLeft( OBS_OBJECT_WORK *obj_work );
static void gmGmkBubbleMainMoveRight( OBS_OBJECT_WORK *obj_work );
static void gmGmkBubbleMainHit( OBS_OBJECT_WORK *obj_work );
static void gmGmkBubbleMainEnd( OBS_OBJECT_WORK *obj_work );
//---------------------------------------------------------
//ê∂ê¨ä«óùópÉÜÅ[ÉUÉèÅ[ÉN
//---------------------------------------------------------

static void gmGmkBubbleSetUserWorkIntervalNormal( OBS_OBJECT_WORK* obj_work, u16 interval );
static u16 gmGmkBubbleGetUserWorkIntervalNormal( const OBS_OBJECT_WORK* obj_work );

static void gmGmkBubbleSetUserTimerCounter( OBS_OBJECT_WORK* obj_work, u32 count );
static void gmGmkBubbleAddUserTimerCounter( OBS_OBJECT_WORK* obj_work, s32 count );
static u32 gmGmkBubbleGetUserTimerCounter( const OBS_OBJECT_WORK* obj_work );


//---------------------------------------------------------
//ñAópÉÜÅ[ÉUÉèÅ[ÉN
//---------------------------------------------------------
//static void gmGmkBubbleSetUserWorkSpeedX( OBS_OBJECT_WORK* obj_work, fx32 speed );
static fx32 gmGmkBubbleAddUserWorkSpeedX( OBS_OBJECT_WORK* obj_work, fx32 speed );
//static fx32 gmGmkBubbleGetUserWorkSpeedX( const OBS_OBJECT_WORK* obj_work );

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkBubbleManagerInit
/*!
 *	ÉMÉ~ÉbÉNèâä˙âªä÷êîÅ@ëßåpÇ¨ÇÃñAê∂ê¨ä«óù
 *
 *	@param eve_rec	[io] ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param pos_x	[in] èoåªç¿ïW
 *	@param pos_y	[in] 
 *	@param type		[in] èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkBubbleManagerInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	//ÉIÉuÉWÉFÉNÉgçÏê¨
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkBubbleLoadObjNoModel( eve_rec, pos_x, pos_y, type );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//èâä˙âª
	gmGmkBubbleManagerInit( obj_work );

	//óéâ∫ä‘äuÅiïbÇ≈ê›íËÇ≥ÇÍÇƒÇ¢ÇÈÇÃÇ≈ÉtÉåÅ[ÉÄêîÇ…ïœä∑Åj
	gmGmkBubbleSetUserWorkIntervalNormal( obj_work, (u16)(eve_rec->left*60) );

	return obj_work;
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkBubbleLoadObjNoModel
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
GMS_ENEMY_3D_WORK* gmGmkBubbleLoadObjNoModel( 
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
			"GMK_BUBBLE");
	amAssert( gimmick_work );

	//-------------------------------------------------
	//ãÈå`èâä˙âª
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}


// ==========================================================================
// gmGmkBubbleGameSystemGetWaterLevel
/*!
 * ÉQÅ[ÉÄÉVÉXÉeÉÄÇ©ÇÁêÖñ ÉåÉxÉãÇéÊìæ
 *
 *	@return	êÖñ ÉåÉxÉã
 */
// ==========================================================================
u16 gmGmkBubbleGameSystemGetWaterLevel( void )
{
	return g_gm_main_system.water_level;
}

// ==========================================================================
//ê∂ê¨ä«óù
// ==========================================================================

// ==========================================================================
// gmGmkBubbleManagerInit
/*!
 *	ÉMÉ~ÉbÉNÅ@ëßåpÇ¨ÇÃñAê∂ê¨ä«óùÅ@èâä˙âª
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBubbleManagerInit( OBS_OBJECT_WORK* obj_work )
{
	//-------------------------------------------------
	// ÉèÅ[ÉNê›íË
	//-------------------------------------------------
	//ÉtÉâÉO
	obj_work->move_flag |= (OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);

	//ÉJÉEÉìÉ^
	gmGmkBubbleSetUserTimerCounter( obj_work, 0 );

	//-------------------------------------------------
	// ÉÅÉCÉìèàóù
	//-------------------------------------------------
	obj_work->ppFunc = gmGmkBubbleManagerMainWait;

	//-------------------------------------------------
	// ÉGÉtÉFÉNÉgèàóù
	//-------------------------------------------------
	GMS_EFFECT_3DES_WORK* effect_work = GmEfctZoneEsCreate(
			obj_work,
			GSD_MAIN_ZONE_TYPE_3,
			GME_EFCT_Z03_IDX_BUBBLE_POINT );
	amAssert( effect_work );
	effect_work->efct_com.obj_work.ppFunc = gmGmkBubbleManagerEffectMain;
}

// ==========================================================================
// gmGmkBubbleManagerMain
/*!
 *	ÉMÉ~ÉbÉNÅ@ëßåpÇ¨ÇÃñAê∂ê¨ä«óùÅ@ÉÅÉCÉì
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBubbleManagerMainWait( OBS_OBJECT_WORK* obj_work )
{
	//êÖñ Ç…èoÇΩèÍçáèàóùÇ»Çµ
	fx32 water_level = gmGmkBubbleGameSystemGetWaterLevel() * FX32_ONE;
	if ( water_level > obj_work->pos.y ){
		return;
	}

	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	u32 counter = gmGmkBubbleGetUserTimerCounter( obj_work );

	//ê∂ê¨OKéûä‘Ç‹Ç≈ÇÕê∂ê¨ÇµÇ»Ç¢
	if ( (u32)(gimmick_work->ene_com.eve_rec->top) * 60 < counter ){
		obj_work->ppFunc = gmGmkBubbleManagerMain;
	}

	//É^ÉCÉ}êßå‰
	gmGmkBubbleAddUserTimerCounter( obj_work, 1 );
}

// ==========================================================================
// gmGmkBubbleManagerMain
/*!
 *	ÉMÉ~ÉbÉNÅ@ëßåpÇ¨ÇÃñAê∂ê¨ä«óùÅ@ÉÅÉCÉì
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBubbleManagerMain( OBS_OBJECT_WORK* obj_work )
{
	//êÖñ Ç…èoÇΩèÍçáèàóùÇ»Çµ
	fx32 water_level = gmGmkBubbleGameSystemGetWaterLevel() * FX32_ONE;
	if ( water_level > obj_work->pos.y ){
		return;
	}

	u32 create_interval_normal = gmGmkBubbleGetUserWorkIntervalNormal( obj_work );
	if ( create_interval_normal == 0 ){
		create_interval_normal = 1*60; 
	}
	u32 counter = gmGmkBubbleGetUserTimerCounter( obj_work );

	//ëßåpÇ¨ÇÃñAê∂ê¨
	if ( counter % create_interval_normal == 0 ){
		//ÉGÉtÉFÉNÉgê∂ê¨
		GMS_EFFECT_3DES_WORK* effect_work = GmEfctZoneEsCreate(
				obj_work,
				GSD_MAIN_ZONE_TYPE_3,
				GME_EFCT_Z03_IDX_BUBBLE_BIG );
		amAssert( effect_work );

		gmGmkBubbleInit( effect_work );
	}

	//É^ÉCÉ}êßå‰
	gmGmkBubbleAddUserTimerCounter( obj_work, 1 );


}

// ==========================================================================
// gmGmkBubbleManagerEffectMain
/*!
 *	ÉMÉ~ÉbÉNÅ@ëßåpÇ¨ÇÃñAê∂ê¨ä«óùÅ@ÉGÉtÉFÉNÉgópÉÅÉCÉì
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBubbleManagerEffectMain( OBS_OBJECT_WORK* obj_work )
{
	//êÖñ Ç…èoÇΩèÍçá
	fx32 water_level = gmGmkBubbleGameSystemGetWaterLevel() * FX32_ONE;
	if ( water_level > obj_work->pos.y ){
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
	else{
		obj_work->disp_flag &= ~OBD_DISP_NODISP;
	}
}

// ==========================================================================
//ñA
// ==========================================================================
// ==========================================================================
// gmGmkBubbleInit
/*!
 *	ÉMÉ~ÉbÉNÅ@ëßåpÇ¨ÇÃñAÅ@èâä˙âª
 *
 *	@param effect_work	[in] ÉGÉtÉFÉNÉgÉèÅ[ÉN
 *	@param type			[in] ñAÉ^ÉCÉv	
 */
// ==========================================================================
const static Uint16 gm_gmk_bubble_table_atk[GME_EFFECT_RECT_NUM]	= {
	0,	// êHÇÁÇ¢ãÈå`
	0,	// çUåÇãÈå`
};
const static Uint16 gm_gmk_bubble_table_def[GME_EFFECT_RECT_NUM]	= {
	GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK,	// êHÇÁÇ¢ãÈå`
	0,	// çUåÇãÈå`
};
void gmGmkBubbleInit( GMS_EFFECT_3DES_WORK* effect_work )
{
	OBS_OBJECT_WORK* effect_obj_work = (OBS_OBJECT_WORK*)effect_work;
	amAssert( effect_obj_work );

	//-------------------------------------------------
	// ÉèÅ[ÉNê›íË
	//-------------------------------------------------
	//ÉtÉâÉO
	effect_obj_work->flag &= ~(OBD_OBJECT_NOHIT);
	effect_obj_work->move_flag &= ~(OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL );
	effect_obj_work->move_flag |= OBD_MOVE_NOCOLOBJ | OBD_MOVE_NOCOLFIELD;

	//ë¨ìxê›íË
	effect_obj_work->spd.y = -GMD_GMK_BUBBLE_SPEED_Y;
	
	//ç¿ïW
	effect_obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_N;

	//-------------------------------------------------
	// ãÈå`ê›íË
	//-------------------------------------------------
	OBS_RECT_WORK* rect_work = effect_work->efct_com.rect_work;
	GmEffectRectInit(
			&effect_work->efct_com,
			gm_gmk_bubble_table_atk,
			gm_gmk_bubble_table_def,
			GMD_OBJ_RECT_GROUP_ENEMY, 
			GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER );
	ObjRectWorkSet( 
			&rect_work[GME_EFFECT_RECT_DEF],
			-8,
			7, 
			8,
			8 );
	rect_work[GME_EFFECT_RECT_DEF].flag |= OBD_RECT_OUT | OBD_RECT_ENABLE;
	rect_work[GME_EFFECT_RECT_DEF].ppDef = gmGmkBubbleDefFunc;
	rect_work[GME_EFFECT_RECT_ATK].flag |= OBD_RECT_NOHIT | OBD_RECT_OUT;

	//-------------------------------------------------
	// ÉÅÉCÉìèàóù
	//-------------------------------------------------
	effect_obj_work->ppFunc = gmGmkBubbleMainMoveLeft;
}

// ==========================================================================
// gmGmkBubbleDefFunc
/*!
 *	ÉMÉ~ÉbÉNÅ@ëßåpÇ¨ÇÃñAÅ@êÖñ Ç…ìûíB
 *
 *	@param own_rect	[in] é©êgÇÃãÈå`ÉèÅ[ÉN
 *	@param target_rect	[in] ëäéËÇÃãÈå`ÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBubbleDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect )
{


	OBS_OBJECT_WORK* gimmick_obj_work = (OBS_OBJECT_WORK*)own_rect->parent_obj;
	amAssert( gimmick_obj_work );
	OBS_OBJECT_WORK* target_obj_work = (OBS_OBJECT_WORK*)target_rect->parent_obj;
	amAssert( target_obj_work );

	//ÉvÉåÉCÉÑÇ∂Ç·Ç»Ç¢
	if ( target_obj_work->obj_type != GMD_OBJTYPE_PLAYER ){
		return;
	}

	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)target_obj_work;

	//ÉvÉåÉCÉÑÉVÅ[ÉPÉìÉX
	GmPlySeqInitBreathing( player_work );

	//éûä‘âÒïú
	GmPlayerBreathingSet( player_work );

	//è¡Ç¶ÇÈ
	gimmick_obj_work->flag |= OBD_OBJECT_TASKCLEAR;

	//ÉGÉtÉFÉNÉgçÏê¨
	GMS_EFFECT_3DES_WORK* effect_work = GmEfctZoneEsCreate(
			NULL,
			GSD_MAIN_ZONE_TYPE_3,
			GME_EFCT_Z03_IDX_BUBBLE_LOST_2 );
	amAssert( effect_work );

	effect_work->efct_com.obj_work.pos.x = gimmick_obj_work->pos.x;
	effect_work->efct_com.obj_work.pos.y = gimmick_obj_work->pos.y;
	effect_work->efct_com.obj_work.pos.z = gimmick_obj_work->pos.z;

	//êUìÆ
	GMM_PAD_VIB_SMALL();

	//ÉqÉbÉgèÛë‘Ç÷
	effect_work->efct_com.obj_work.ppFunc = gmGmkBubbleMainHit;
}

// ==========================================================================
// gmGmkBubbleMainMoveLeft
/*!
 *	ÉMÉ~ÉbÉNÅ@ëßåpÇ¨ÇÃñAÅ@ÉÅÉCÉìÅiç∂å¸Ç´Åj
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBubbleMainMoveLeft( OBS_OBJECT_WORK *obj_work )
{
	//â°à⁄ìÆ
	obj_work->spd.x = gmGmkBubbleAddUserWorkSpeedX( obj_work, -GMD_GMK_BUBBLE_SPEED_X_ADD );
	if ( obj_work->spd.x < -GMD_GMK_BUBBLE_SPEED_X_MAX ){
		obj_work->ppFunc = gmGmkBubbleMainMoveRight;
	}

	//êÖñ Ç…èoÇΩèÍçá
	fx32 water_level = gmGmkBubbleGameSystemGetWaterLevel() * FX32_ONE;
	if ( water_level > obj_work->pos.y ){
		obj_work->ppFunc = gmGmkBubbleMainEnd;
	}
}

// ==========================================================================
// gmGmkBubbleMainMoveRight
/*!
 *	ÉMÉ~ÉbÉNÅ@ëßåpÇ¨ÇÃñAÅ@ÉÅÉCÉìÅiâEå¸Ç´Åj
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBubbleMainMoveRight( OBS_OBJECT_WORK *obj_work )
{
	//â°à⁄ìÆ
	obj_work->spd.x = gmGmkBubbleAddUserWorkSpeedX( obj_work, GMD_GMK_BUBBLE_SPEED_X_ADD );
	if ( obj_work->spd.x > GMD_GMK_BUBBLE_SPEED_X_MAX ){
		obj_work->ppFunc = gmGmkBubbleMainMoveLeft;
	}

	//êÖñ Ç…èoÇΩèÍçá
	fx32 water_level = gmGmkBubbleGameSystemGetWaterLevel() * FX32_ONE;
	if ( water_level > obj_work->pos.y ){
		obj_work->ppFunc = gmGmkBubbleMainEnd;
	}
}

// ==========================================================================
// gmGmkBubbleMainHit
/*!
 *	ÉMÉ~ÉbÉNÅ@ëßåpÇ¨ÇÃñAÅ@ÉÅÉCÉìÅihitéûÅj
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBubbleMainHit( OBS_OBJECT_WORK *obj_work )
{
	++obj_work->user_timer;
	s32 time = GMD_GMK_BUBBLE_FRAME_HIT_DELETE - obj_work->user_timer;

	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	//è¨Ç≥Ç¢ñAÉGÉtÉFÉNÉg
	if ( time < GMD_GMK_BUBBLE_HIT_EFFECT_NUM ){
		GmPlyEfctCreateBubble(player_work);
	}

	if ( time > 0 ){
		//äÁÇ…å¸Ç©Ç¡Çƒà⁄ìÆ
		OBS_OBJECT_WORK* player_obj_work = (OBS_OBJECT_WORK*)player_work;
		fx32 distance_x = player_obj_work->pos.x - obj_work->pos.x;
		fx32 distance_y = player_obj_work->pos.y-GMD_GMK_BUBBLE_OFFSET_Y*FX32_ONE - obj_work->pos.y;
		
		//å¸Ç´í≤êÆ
		if (player_obj_work->disp_flag & OBD_DISP_HFLIP ){
			distance_x -= GMD_GMK_BUBBLE_OFFSET_X*FX32_ONE;
		}
		else{
			distance_x += GMD_GMK_BUBBLE_OFFSET_X*FX32_ONE;
		}

		obj_work->spd.x = distance_x/time;
		obj_work->spd.y = distance_y/time;
		return;
	}
	obj_work->user_timer = 0;

	//è¡Ç¶ÇÈ
	obj_work->flag |= OBD_OBJECT_TASKCLEAR;
}

// ==========================================================================
// gmGmkBubbleMainEnd
/*!
 *	ÉMÉ~ÉbÉNÅ@ëßåpÇ¨ÇÃñAÅ@êÖñ Ç…ìûíB
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBubbleMainEnd( OBS_OBJECT_WORK *obj_work )
{

	//è¡Ç¶ÇÈ
	obj_work->flag |= OBD_OBJECT_TASKCLEAR;

	//ÉGÉtÉFÉNÉgçÏê¨
	GMS_EFFECT_3DES_WORK* effect_work = GmEfctZoneEsCreate(
			NULL,
			GSD_MAIN_ZONE_TYPE_3,
			GME_EFCT_Z03_IDX_BUBBLE_LOST );
	amAssert( effect_work );

	effect_work->efct_com.obj_work.pos.x = obj_work->pos.x;
	effect_work->efct_com.obj_work.pos.y = obj_work->pos.y;
	effect_work->efct_com.obj_work.pos.z = obj_work->pos.z;
}

// ==========================================================================
// ê∂ê¨ä«óùópÉÜÅ[ÉUóÃàÊ
// ==========================================================================

// ==========================================================================
// gmGmkBubbleSetUserWorkIntervalNormal
/*!
 *	ÉÜÅ[ÉUÉèÅ[ÉNÇê∂ê¨ä‘äuÇ∆ÇµÇƒê›íË
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *	@param length	[in] ê∂ê¨ä‘äu
 */
// ==========================================================================
void gmGmkBubbleSetUserWorkIntervalNormal( OBS_OBJECT_WORK* obj_work, u16 interval )
{
	obj_work->user_work |= interval << 16;
}

// ==========================================================================
// gmGmkBubbleGetUserWorkIntervalNormal
/*!
 *	ÉÜÅ[ÉUÉèÅ[ÉNÇê∂ê¨ä‘äuÇ∆ÇµÇƒéÊìæ
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *
 *	@return ê∂ê¨ä‘äu
 */
// ==========================================================================
u16 gmGmkBubbleGetUserWorkIntervalNormal( const OBS_OBJECT_WORK* obj_work )
{
	return (u16)(obj_work->user_work >> 16);
}

// ==========================================================================
// gmGmkBubbleSetUserTimerCounter
/*!
 *	ÉÜÅ[ÉUÉ^ÉCÉ}Çåoâﬂéûä‘Ç∆ÇµÇƒê›íË
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *	@param length	[in] åoâﬂéûä‘
 */
// ==========================================================================
void gmGmkBubbleSetUserTimerCounter( OBS_OBJECT_WORK* obj_work, u32 count )
{
	obj_work->user_timer = (s32)count;
}

// ==========================================================================
// gmGmkBubbleAddUserTimerCounter
/*!
 *	ÉÜÅ[ÉUÉ^ÉCÉ}Çåoâﬂéûä‘Ç∆ÇµÇƒí«â¡
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *	@param length	[in] åoâﬂéûä‘
 */
// ==========================================================================
void gmGmkBubbleAddUserTimerCounter( OBS_OBJECT_WORK* obj_work, s32 count )
{
	obj_work->user_timer += count;
}

// ==========================================================================
// gmGmkBubbleGetUserTimerCounter
/*!
 *	ÉÜÅ[ÉUÉ^ÉCÉ}Çåoâﬂéûä‘Ç∆ÇµÇƒéÊìæ
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *
 *	@return åoâﬂéûä‘
 */
// ==========================================================================
u32 gmGmkBubbleGetUserTimerCounter( const OBS_OBJECT_WORK* obj_work )
{
	return (u32)obj_work->user_timer;
}



// ==========================================================================
// ñAópÉÜÅ[ÉUóÃàÊ
// ==========================================================================

// ==========================================================================
// gmGmkBubbleSetUserWorkSpeedX
/*!
 *	ÉÜÅ[ÉUÉèÅ[ÉNÇXÉXÉsÅ[ÉhÇ∆ÇµÇƒê›íË
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *	@param length	[in] XÉXÉsÅ[Éh
 */
// ==========================================================================
/*void gmGmkBubbleSetUserWorkSpeedX( OBS_OBJECT_WORK* obj_work, fx32 speed )
{
	obj_work->user_work = (u32)speed;
}*/

// ==========================================================================
// gmGmkBubbleAddUserWorkSpeedX
/*!
 *	ÉÜÅ[ÉUÉèÅ[ÉNÇXÉXÉsÅ[ÉhÇ∆ÇµÇƒí«â¡
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *	@param length	[in] XÉXÉsÅ[Éh
 */
// ==========================================================================
fx32 gmGmkBubbleAddUserWorkSpeedX( OBS_OBJECT_WORK* obj_work, fx32 speed )
{
	obj_work->user_work += (u32)speed;
	return (fx32)obj_work->user_work;
}

// ==========================================================================
// gmGmkBubbleGetUserWorkSpeedX
/*!
 *	ÉÜÅ[ÉUÉèÅ[ÉNÇXÉXÉsÅ[ÉhÇ∆ÇµÇƒéÊìæ
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 *
 *	@return XÉXÉsÅ[Éh
 */
// ==========================================================================
/*fx32 gmGmkBubbleGetUserWorkSpeedX( const OBS_OBJECT_WORK* obj_work )
{
	return (fx32)obj_work->user_work;
}*/




// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
