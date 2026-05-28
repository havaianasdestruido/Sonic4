// ==========================================================================
/*!
  @file gmGmkWaterArea.c
  @brief ƒMƒ~ƒbƒN …’†”ÍˆÍ

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkWaterArea.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 * ‹éŒ`¶ãXˆÊ’u   •ÏXŒã‚Ì…ˆÊi100‚ÌˆÊj
 * ‹éŒ`¶ãYˆÊ’u   •ÏXŒã‚Ì…ˆÊi1‚ÌˆÊj
 * ‹éŒ`‰¡ƒTƒCƒY    ‹éŒ`‰¡ƒTƒCƒY
 * ‹éŒ`cƒTƒCƒY    ‹éŒ`cƒTƒCƒY
 *
 * 
 * ƒtƒ‰ƒO 1`10F…ˆÊ•ÏXŠÔ
 * @~~~~~~~~~¡ on :+1•b
 * @~~~~~~~~¡~ on :+2•b
 * @~~~~~~~¡~~ on :+3•b
 * @@@F
 * @¡~~~~~~~~~ on :+10•b
 * 
 * ‰E‘¤‚©‚ç¶‘¤‚É’Ê‚è”²‚¯‚½ê‡‚É“®ìŠJn
 * İ’è‚µ‚½ŠÔ‚ğŠ|‚¯‚ÄAİ’è‚µ‚½…ˆÊ‚É•ÏX‚·‚é
 * ƒ}ƒbƒv‚Ìˆê”Ôã‚Ì…ˆÊ‚Í0
 * 
 * •ÏXŒã‚Ì…ˆÊŒvZ®
 * (‹éŒ`¶ãXˆÊ’u)*100 + ‹éŒ`¶ãYˆÊ’u 
 * 
 * ‹éŒ`‚Ì’†SÀ•W‚ÍƒIƒuƒWƒFƒNƒg‚Ì”z’uÀ•W
 * GMD_GMK_WATER_AREA_RECT_MARGIN‚æ‚è¬‚³‚¢’l‚ğİ’è‚µ‚½ê‡AGMD_GMK_WATER_AREA_RECT_MARGIN‚Æ‚µ‚Ä“®ì‚·‚é
 * 
 * ƒtƒ‰ƒO‚ğ‘S‚Ä—§‚Ä‚½Û‚Ì…ˆÊ•ÏXŠÔ‚Í55•b
 */

#if defined(GMD_MAIN_GAME_THUMB)
#pragma thumb on
#endif

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEventTbl.h"
#include "gmEnemy.h"

#include "gmWaterSurface.h"

#include "gmGmkWaterArea.h"

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------

#define GMD_GMD_WATER_AREA_USE_DIRECT_CHANGE_LEVEL (0)


#define GMD_GMK_WATER_AREA_RECT_MARGIN (32 + 2)	//ƒvƒŒƒCƒ„‚ÌˆÚ“®—Ê‚ª‘½‚¢ê‡A“–‚½‚è”»’è‚ğs‚¤‚Æ‚«‚É”½‘Î‘¤‚É”²‚¯‚È‚¢‚æ‚¤‚É

//…–Êƒ^ƒCƒv
enum GME_DMK_WATER_AREA_TYPE{
	GMD_DMK_WATER_AREA_TYPE_START = 0,		//ŠJnAÄŠJ

	GMD_DMK_WATER_AREA_TYPE_DELAY_LEFT,		//¶‚©‚ç‚ ‚½‚Á‚½ê‡‚É—LŒøi’x‰„j
	GMD_DMK_WATER_AREA_TYPE_DELAY_RIGHT,	//‰E‚©‚ç‚ ‚½‚Á‚½ê‡‚É—LŒøi’x‰„j
	GMD_DMK_WATER_AREA_TYPE_DELAY_TOP,		//ã‚©‚ç‚ ‚½‚Á‚½ê‡‚É—LŒøi’x‰„j	
	GMD_DMK_WATER_AREA_TYPE_DELAY_BOTTOM,	//‰º‚©‚ç‚ ‚½‚Á‚½ê‡‚É—LŒøi’x‰„j

	GMD_DMK_WATER_AREA_TYPE_NUM,
	GMD_EVENT_ID_WATER_AREA_INVALID = -1

};

#define GMD_GMK_WATER_AREA_RESTART_SIZE		((fx32)128*FX32_ONE)	//ƒŠƒXƒ^[ƒgİ’è‚ª—LŒø‚È”ÍˆÍ

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

static GMS_ENEMY_3D_WORK* gmGmkWaterAreaLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type );

static void gmGmkWaterAreaInit( 
							   GMS_ENEMY_3D_WORK* gimmick_work,
							   u16 water_level,
							   u16 time );

static GME_DMK_WATER_AREA_TYPE gmGmkWaterAreaGetType( const GMS_EVE_RECORD_EVENT* eve_rec );
static BOOL gmGmkWaterAreaCheckRestart( fx32 pos_x, fx32 pos_y );
static void gmGmkWaterAreaRequestChangeWatarLevel( OBS_OBJECT_WORK* obj_work );

// ==========================================================================
//‹éŒ`
// ==========================================================================
static void gmGmkWaterAreaSetRect( 
						   GMS_ENEMY_3D_WORK* gimmick_work,
						   u8 width,
						   u8 height,
						   GME_DMK_WATER_AREA_TYPE type );
static BOOL gmGmkWaterAreaCheckDir( 
							const OBS_OBJECT_WORK* gimmick_obj_work, 
							const OBS_OBJECT_WORK* player_obj_work,
							GME_DMK_WATER_AREA_TYPE type );
static void gmGmkWaterAreaDefFuncDelay( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect );


// ==========================================================================
//ƒ‚[ƒh
// ==========================================================================
static BOOL gmGmkWaterAreaModeCheckWait( const OBS_OBJECT_WORK* obj_work );
static void gmGmkWaterAreaModeChangeWait( OBS_OBJECT_WORK* obj_work );
static void gmGmkWaterAreaModeChangeLady( OBS_OBJECT_WORK* obj_work );
static void gmGmkWaterAreaModeChangeActive( OBS_OBJECT_WORK* obj_work );
static void gmGmkWaterAreaMainLady( OBS_OBJECT_WORK* obj_work );
static void gmGmkWaterAreaMainActive( OBS_OBJECT_WORK* obj_work );

// ==========================================================================
//ƒ†[ƒUƒ[ƒNAƒ†[ƒUƒ^ƒCƒ}
// ==========================================================================
static void gmGmkWaterAreaUserWorkSetLevel( OBS_OBJECT_WORK* obj_work, u16 level );
static u16 gmGmkWaterAreaUserWorkGetLevel( const OBS_OBJECT_WORK* obj_work );
static void gmGmkWaterAreaUserWorkSetTime( OBS_OBJECT_WORK* obj_work, u16 time );
static u16 gmGmkWaterAreaUserWorkGetTime( const OBS_OBJECT_WORK* obj_work );

static void gmGmkWaterAreaUserTimerSetCounter( OBS_OBJECT_WORK* obj_work, s32 time );
static void gmGmkWaterAreaUserTimerAddCounter( OBS_OBJECT_WORK* obj_work, s32 add  );
static s32 gmGmkWaterAreaUserTimerGetCounter( const OBS_OBJECT_WORK* obj_work );

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------

// ==========================================================================
// GmGmkWaterAreaInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»i…’†”ÍˆÍj
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkWaterAreaInit( GMS_EVE_RECORD_EVENT* eve_rec, fx32 pos_x, fx32 pos_y, u8 type )
{
	
	//------------------------------------------
	//ƒŒƒR[ƒh‰ğÍ
	//------------------------------------------
	//…–Ê‚‚³
	u16 water_level = (u16)(eve_rec->left * 100 + eve_rec->top * 1);

	//ŠÔ
	u16 time = 0;
	u16 flag = eve_rec->flag;
	for ( u16 i = 0; 10 > i; ++i ){
		if (flag & 0x0001){
			time += i+1;
		}
		flag = (u16)(flag >> 1);
	}
	
	//ƒ^ƒCƒv
	GME_DMK_WATER_AREA_TYPE gimmick_type = gmGmkWaterAreaGetType( eve_rec );
	if  ( gimmick_type == GMD_DMK_WATER_AREA_TYPE_START ){
		//ƒŠƒXƒ^[ƒg‚Ì‚Æ‚«‚Ì‚İ
		if ( gmGmkWaterAreaCheckRestart(pos_x, pos_y) ){
			//ƒQ[ƒ€î•ñ‚É…–ÊƒŒƒxƒ‹‚ğİ’è	
			GmWaterSurfaceRequestChangeWaterLevel( water_level, (u16)(time*60), FALSE );
		}	

		mppEnemyList_add(eve_rec); //qqq
		
		// ƒXƒLƒbƒvƒtƒ‰ƒO‘‚«‚İ
		eve_rec->pos_x = GMD_EVE_RECORD_CMD_SKIP;	

		return NULL;
	}

	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkWaterAreaLoadObj( eve_rec, pos_x, pos_y, type );
	amAssert( gimmick_work );

	//------------------------------------------
	//‰Šú‰»
	//------------------------------------------
	gmGmkWaterAreaInit( gimmick_work, water_level, time );

	return (OBS_OBJECT_WORK*)gimmick_work;
}

//----- Local Functions -----------------------------------------------------


// ==========================================================================
// gmGmkWaterAreaLoadObj
/*!
 *	ƒMƒ~ƒbƒN“Ç‚İ‚İi…’†”ÍˆÍj
 *
 *	@param eve_rec	[in] ƒCƒxƒ“ƒgƒŒƒR[ƒh
 *	@param pos_x	[in] À•WX
 *	@param pos_y	[in] À•WY
 *	@param type		[in] ƒ^ƒCƒv
 *
 *	@return ƒ[ƒN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkWaterAreaLoadObj( 
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
			sizeof(GMS_ENEMY_3D_WORK), 
			"GMK_WATER_AREA");
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}

// ==========================================================================
// gmGmkWaterAreaInit
/*!
 *	ƒMƒ~ƒbƒNi…’†”ÍˆÍj‰Šú‰»
 *
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param water_level	[in] …ˆÊ
 *	@param time			[in] ŠÔ
 */
// ==========================================================================
void gmGmkWaterAreaInit( 
						GMS_ENEMY_3D_WORK* gimmick_work,
						u16 water_level,
						u16 time)
{
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* gimmick_obj_work = (OBS_OBJECT_WORK*)gimmick_work;
	amAssert( gimmick_obj_work );
	const  GMS_EVE_RECORD_EVENT* eve_rec = gimmick_work->ene_com.eve_rec;
	amAssert( eve_rec );
	GME_DMK_WATER_AREA_TYPE gimmick_type = gmGmkWaterAreaGetType( eve_rec );

	//‹éŒ`İ’è
	u8 widht = eve_rec->width;
	u8 height = eve_rec->height;
	gmGmkWaterAreaSetRect( gimmick_work, widht, height, gimmick_type );

	//------------------------------------------
	//ƒ[ƒNİ’è
	//------------------------------------------

	//ƒ^[ƒQƒbƒgİ’è
	gimmick_work->ene_com.target_obj = &g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_work;

	//ƒtƒ‰ƒO
	gimmick_obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// ˆÚ“®–³‚µ “–‚½‚è–³‚µ
	gimmick_obj_work->disp_flag |= OBD_DISP_NODISP;						// •`‰æ–³‚µ

	//…–Ê
	gmGmkWaterAreaUserWorkSetLevel( gimmick_obj_work, water_level );
	gmGmkWaterAreaUserWorkSetTime( gimmick_obj_work, time );
}

// ==========================================================================
// gmGmkWaterAreaCheckRestart
/*!
 *	ƒŠƒXƒ^[ƒgƒ`ƒFƒbƒN
 *
 *	@param	pos_x [in] À•W
 *	@param	pos_y [in] À•W
 *
 *	@return TRUEFƒŠƒXƒ^[ƒg FALSEFƒŠƒXƒ^[ƒg‚Å‚È‚¢
 */
// ==========================================================================
BOOL gmGmkWaterAreaCheckRestart( fx32 pos_x, fx32 pos_y )
{

	//ƒVƒXƒeƒ€ƒtƒ‰ƒOƒ`ƒFƒbƒN
	/*if ( !(g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_RESTART) ){
		return FALSE;
	}*/

	//ƒvƒŒƒCƒ„‚ª‰æ–Ê“à‚É‚¢‚é‚©ƒ`ƒFƒbƒN
	fx32 distance_x = MTM_MATH_ABS( g_gm_main_system.resume_pos_x - pos_x );
	fx32 distance_y = MTM_MATH_ABS( g_gm_main_system.resume_pos_y - pos_y );
	if ( distance_y > GMD_GMK_WATER_AREA_RESTART_SIZE 
			|| distance_x > GMD_GMK_WATER_AREA_RESTART_SIZE
	){
		return FALSE;
	}
	return TRUE;
}

// ==========================================================================
// gmGmkWaterAreaGetType
/*!
 *	…–Êƒ^ƒCƒv‚ğæ“¾
 *
 *	@param eve_rec	[in] ƒCƒxƒ“ƒgî•ñ
 *
 *	@return …–Êƒ^ƒCƒv
 */
// ==========================================================================
GME_DMK_WATER_AREA_TYPE gmGmkWaterAreaGetType( const GMS_EVE_RECORD_EVENT* eve_rec )
{

	GME_DMK_WATER_AREA_TYPE type = GMD_EVENT_ID_WATER_AREA_INVALID;
	switch ( eve_rec->id ){
	//‹­§
	case GMD_EVENT_ID_WATER_AREA_F:
		type = GMD_DMK_WATER_AREA_TYPE_START;
		break;
	//¶‚©‚ç‚ ‚½‚Á‚½ê‡‚É—LŒø
	case GMD_EVENT_ID_WATER_AREA_L:
		type = GMD_DMK_WATER_AREA_TYPE_DELAY_LEFT;
		break;
	//‰E‚©‚ç‚ ‚½‚Á‚½ê‡‚É—LŒø
	case GMD_EVENT_ID_WATER_AREA_R:
		type = GMD_DMK_WATER_AREA_TYPE_DELAY_RIGHT;
		break;
	//ã‚©‚ç‚ ‚½‚Á‚½ê‡‚É—LŒø
	case GMD_EVENT_ID_WATER_AREA_T:
		type = GMD_DMK_WATER_AREA_TYPE_DELAY_TOP;
		break;
	//‰º‚©‚ç‚ ‚½‚Á‚½ê‡‚É—LŒø
	case GMD_EVENT_ID_WATER_AREA_B:
		type = GMD_DMK_WATER_AREA_TYPE_DELAY_BOTTOM;
		break;
	default:
		amAssert( FALSE );
		break;
	}

	return type;
}

// ==========================================================================
// gmGmkWaterAreaRequestChangeWatarLevel
/*!
 *	ƒMƒ~ƒbƒN…–Ê‹éŒ`İ’è
 *
 *	@param obj_work	[in] ƒMƒ~ƒbƒNƒ[ƒN
 *	@param width		[in] ‹éŒ`•
 *	@param height		[in] ‹éŒ`‚‚³
 *	@param type			[in] …–Êƒ^ƒCƒv
 */
// ==========================================================================
void gmGmkWaterAreaRequestChangeWatarLevel( OBS_OBJECT_WORK* obj_work )
{
	u16 water_level = gmGmkWaterAreaUserWorkGetLevel(obj_work);
	u16 time = gmGmkWaterAreaUserWorkGetTime(obj_work);

	GmWaterSurfaceRequestChangeWaterLevel( water_level, (u16)(time*60), FALSE );
}


// ==========================================================================
// gmGmkWaterAreaSetRect
/*!
 *	ƒMƒ~ƒbƒN…–Ê‹éŒ`İ’è
 *
 *	@param gimmick_work	[in] ƒMƒ~ƒbƒNƒ[ƒN
 *	@param width		[in] ‹éŒ`•
 *	@param height		[in] ‹éŒ`‚‚³
 *	@param type			[in] …–Êƒ^ƒCƒv
 */
// ==========================================================================
void gmGmkWaterAreaSetRect( 
						   GMS_ENEMY_3D_WORK* gimmick_work,
						   u8 width,
						   u8 height,
						   GME_DMK_WATER_AREA_TYPE type )
{
	amAssert( gimmick_work );

	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	amAssert( rect_work );

	//ƒvƒŒƒCƒ„‚ÌˆÚ“®—Ê‚ª‘½‚¢ê‡A“–‚½‚è”»’è‚ğs‚¤‚Æ‚«‚É”½‘Î‘¤‚É”²‚¯‚È‚¢‚æ‚¤‚É
	if ( width < GMD_GMK_WATER_AREA_RECT_MARGIN ){
		width = GMD_GMK_WATER_AREA_RECT_MARGIN;
	}
	if ( height < GMD_GMK_WATER_AREA_RECT_MARGIN ){
		height = GMD_GMK_WATER_AREA_RECT_MARGIN;
	}
	ObjRectWorkZSet( rect_work, (s16)(-width/2), (s16)(-height/2), -500, (s16)(width/2), (s16)(height/2), 500);

	//UŒ‚—p
	ObjRectAtkSet(rect_work, 0, 0);
	rect_work->ppHit = NULL;

	//–hŒä—p
	ObjRectDefSet(rect_work, 0, 0);
	switch (type){
		//ŠJnAÄŠJ
		case GMD_DMK_WATER_AREA_TYPE_START:
			break;
		//¶A‰EAãA‰º‚©‚ç‚ ‚½‚Á‚½ê‡‚É—LŒøi’x‰„‚Â‚«j
		case GMD_DMK_WATER_AREA_TYPE_DELAY_LEFT:
		case GMD_DMK_WATER_AREA_TYPE_DELAY_RIGHT:
		case GMD_DMK_WATER_AREA_TYPE_DELAY_TOP:
		case GMD_DMK_WATER_AREA_TYPE_DELAY_BOTTOM:
			rect_work->flag |= OBD_RECT_OUT;
			rect_work->ppDef = gmGmkWaterAreaDefFuncDelay;
			break;
		default:
			amAssert(FALSE);
			break;
	}
}
// ==========================================================================
// gmGmkWaterAreaCheckDir
/*!
 *	i“ü•ûŒüƒ`ƒFƒbƒN
 * 
 *	@param gimmick_obj_work	[in] ©g‚Ì‹éŒ`ƒ[ƒN
 *	@param player_obj_work	[in] ‘Šè‚Ì‹éŒ`ƒ[ƒN
 *	@param type	[in] …–Êƒ^ƒCƒv
 * 
 *	@return TRUEF“ü‚é•ûŒü  flaseFo‚é•ûŒü
 */
// ==========================================================================
BOOL gmGmkWaterAreaCheckDir( 
							const OBS_OBJECT_WORK* gimmick_obj_work, 
							const OBS_OBJECT_WORK* player_obj_work,
							GME_DMK_WATER_AREA_TYPE type )
{
	BOOL result = FALSE;

	switch ( type ){
	case GMD_DMK_WATER_AREA_TYPE_DELAY_LEFT:
		if ( player_obj_work->pos.x < gimmick_obj_work->pos.x ){
			result = TRUE;
		}
		break;
	case GMD_DMK_WATER_AREA_TYPE_DELAY_RIGHT:
		if ( gimmick_obj_work->pos.x < player_obj_work->pos.x  ){
			result = TRUE;
		}
		break;
	case GMD_DMK_WATER_AREA_TYPE_DELAY_TOP:
		if ( player_obj_work->pos.y < gimmick_obj_work->pos.y ){
			result = TRUE;
		}
		break;
	case GMD_DMK_WATER_AREA_TYPE_DELAY_BOTTOM:
		if ( gimmick_obj_work->pos.y < player_obj_work->pos.y ){
			result = TRUE;
		}
		break;
	case GMD_DMK_WATER_AREA_TYPE_START:
		result = TRUE;
		break;
	default:
		break;
	}

	return result;
}

#if GMD_GMD_WATER_AREA_USE_DIRECT_CHANGE_LEVEL

// ==========================================================================
// gmGmkWaterAreaDefFuncLeft
/*!
 *	ƒMƒ~ƒbƒN…–Ê‘Œ¸‹éŒ`ŠÖ”i¶‚©‚ç“–‚½‚Á‚½i‘¦jj
 *
 *	‘¦‚Ìê‡A–ˆƒtƒŒ[ƒ€‹éŒ`”»’è‚ğs‚¢A
 *	’†S‚æ‚è”½‘Î‘¤‚ÉƒvƒŒƒCƒ„‚ªˆÚ“®‚µ‚½‚çA…–ÊƒŒƒxƒ‹‚ğ•ÏX‚·‚éB
 *	©•ª‘¤‚Ì‚Í•`‰æ‚ğ–³Œø‚É‚·‚é
 *
 *	@param own_rect	[in] ©g‚Ì‹éŒ`ƒ[ƒN
 *	@param target_rect	[in] ‘Šè‚Ì‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkWaterAreaDefFuncLeft( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect )
{
	OBS_OBJECT_WORK* gimmick_obj_work = own_rect->parent_obj;
	amAssert( gimmick_obj_work );
	const OBS_OBJECT_WORK* player_obj_work = target_rect->parent_obj;
	amAssert( player_obj_work );

	GME_DMK_WATER_AREA_TYPE type = gmGmkWaterAreaGetType( gimmick_work );

	//‚Ü‚¾i“ü•ûŒü‘¤‚É‚¢‚é
	if ( gmGmkWaterAreaCheckDir(gimmick_obj_work, player_obj_work, type) ){
		GmDecoSetFlagDrawWatar( FALSE );
	}

	//”½‘Î‘¤‚ÉˆÚ“®‚µ‚½
	else{
		//ƒQ[ƒ€î•ñ‚É…–ÊƒŒƒxƒ‹‚ğİ’è	
		gmGmkWaterAreaRequestChangeWatarLevel( gimmick_obj_work );
		GmDecoSetFlagDrawWatar( TRUE );
	}
}

#endif	//GMD_GMD_WATER_AREA_USE_DIRECT_CHANGE_LEVEL

// ==========================================================================
// gmGmkWaterAreaDefFuncDelayLeft
/*!
 *	ƒMƒ~ƒbƒN…–Ê‘Œ¸‹éŒ`ŠÖ”i¶‚©‚ç“–‚½‚Á‚½i’x‰„jj
 *
 *	‘¦‚Ìê‡A‹éŒ`”»’è‚ÍÅ‰‚É‚ ‚½‚Á‚Æ‚«‚Ì‚İs‚¢A
 *	’†S‚Ì‚Ç‚¿‚ç‘¤‚©‚çƒvƒŒƒCƒ„“ü‚Á‚Ä‚«‚½‚©‚Å”»’è‚·‚éB
 *
 *	…–ÊƒŒƒxƒ‹‚Ì‘Œ¸‚ÍƒƒCƒ“ˆ—‚É”C‚¹‚éB
 *
 *	@param own_rect	[in] ©g‚Ì‹éŒ`ƒ[ƒN
 *	@param target_rect	[in] ‘Šè‚Ì‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkWaterAreaDefFuncDelay( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect )
{
	OBS_OBJECT_WORK* gimmick_obj_work = own_rect->parent_obj;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)gimmick_obj_work;
	amAssert( gimmick_work );
	const OBS_OBJECT_WORK* player_obj_work = target_rect->parent_obj;
	amAssert( player_obj_work );

	//‘Ò‹@ƒ‚[ƒhˆÈŠO‚Íˆ—‚µ‚È‚¢
	if ( !gmGmkWaterAreaModeCheckWait(gimmick_obj_work) ){
		return ;
	}

	//w’è‘¤‚©‚ç‹éŒ`‚É“ü‚Á‚½ê‡A€”õƒ‚[ƒh‚ÉˆÚs
	const  GMS_EVE_RECORD_EVENT* eve_rec = gimmick_work->ene_com.eve_rec;
	amAssert( eve_rec );
	GME_DMK_WATER_AREA_TYPE type = gmGmkWaterAreaGetType( eve_rec );
	if ( gmGmkWaterAreaCheckDir(gimmick_obj_work, player_obj_work, type) ){
		gmGmkWaterAreaModeChangeLady( gimmick_obj_work );
	}
}

// ==========================================================================
// gmGmkWaterAreaModeCheckWait
/*!
 *	ƒMƒ~ƒbƒN…–Ê ‘Ò‹@ƒ‚[ƒh‚©‚ğŠm”FiƒƒCƒ“ˆ—‚ª‚ ‚é‚©‚ğ”»’èj
 *	@param rect_work	[in] ‹éŒ`ƒ[ƒN
 *
 *	@param TRUE:‘Ò‹@ƒ‚[ƒh FALSE:‚»‚êˆÈŠO
 */
// ==========================================================================
BOOL gmGmkWaterAreaModeCheckWait( const OBS_OBJECT_WORK* obj_work )
{
	if ( obj_work->ppFunc ){
		return FALSE;
	}
	return TRUE;
}

// ==========================================================================
// gmGmkWaterAreaModeChangeWait
/*!
 *	ƒMƒ~ƒbƒN…–Ê ‘Ò‹@ƒ‚[ƒh‚ÉØ‚è‘Ö‚¦
 *	@param rect_work	[io] ‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkWaterAreaModeChangeWait( OBS_OBJECT_WORK* obj_work )
{
	//ƒNƒŠƒbƒv‚³‚ê‚é
	obj_work->flag &= ~OBD_OBJECT_NOCLIP;

	//ƒƒCƒ“ˆ—‚È‚µ
	obj_work->ppFunc = NULL;
}

// ==========================================================================
// gmGmkWaterAreaModeChangeLady
/*!
 *	ƒMƒ~ƒbƒN…–Ê €”õƒ‚[ƒh‚ÉØ‚è‘Ö‚¦iw’è‚ÌŒü‚«‚©‚çi“üŠJnj
 *	@param rect_work	[io] ‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkWaterAreaModeChangeLady( OBS_OBJECT_WORK* obj_work )
{
	//ƒNƒŠƒbƒv‚³‚ê‚é
	obj_work->flag &= ~OBD_OBJECT_NOCLIP;

	//ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmGmkWaterAreaMainLady;
}


// ==========================================================================
// gmGmkWaterAreaModeChangeActive
/*!
 *	ƒMƒ~ƒbƒN…–Ê ƒAƒNƒeƒBƒuƒ‚[ƒh‚ÉØ‚è‘Ö‚¦i”½‘Î‘¤‚É”²‚¯‚½j
 *	@param rect_work	[io] ‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkWaterAreaModeChangeActive( OBS_OBJECT_WORK* obj_work )
{
	//ƒQ[ƒ€î•ñ‚É…–ÊƒŒƒxƒ‹‚ğİ’è	
	gmGmkWaterAreaRequestChangeWatarLevel( obj_work );

	//ƒNƒŠƒbƒv‚³‚ê‚È‚¢
	obj_work->flag |= OBD_OBJECT_NOCLIP;

	//ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmGmkWaterAreaMainActive;

	//ƒJƒEƒ“ƒ^‰Šú‰»
	gmGmkWaterAreaUserTimerSetCounter( obj_work, 0 );

	//ƒQ[ƒ€î•ñ‚É…–ÊƒŒƒxƒ‹‚ğİ’è
	gmGmkWaterAreaRequestChangeWatarLevel( obj_work );

	//…–Ê•\¦‚É
	GmWaterSurfaceSetFlagDraw( TRUE );
}

// ==========================================================================
// gmGmkWaterAreaMainLady
/*!
 *	ƒMƒ~ƒbƒN…–Ê ‘Ò‹@ƒ‚[ƒh‚ÌƒƒCƒ“ˆ—
 *	@param rect_work	[io] ‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkWaterAreaMainLady( OBS_OBJECT_WORK *obj_work )
{
	const GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );
	const OBS_RECT_WORK* own_rect = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	amAssert( own_rect );

	//‹éŒ`”ÍˆÍ“à‚È‚çA“Á‚Éˆ—‚È‚µ
	if ( own_rect->flag & OBD_RECT_FRAMEHIT ){
		return;
	}

	//‹éŒ`”ÍˆÍŠO‚Éo‚½
	const  GMS_EVE_RECORD_EVENT* eve_rec = gimmick_work->ene_com.eve_rec;
	amAssert( eve_rec );
	GME_DMK_WATER_AREA_TYPE type = gmGmkWaterAreaGetType( eve_rec );

	//i“ü•ûŒü‚Ìê‡A‘Ò‹@‚É–ß‚é
	const OBS_OBJECT_WORK* player_obj_work = gimmick_work->ene_com.target_obj;
	amAssert( player_obj_work );
	if ( gmGmkWaterAreaCheckDir( obj_work, player_obj_work, type ) ){
		gmGmkWaterAreaModeChangeWait(obj_work);
	}
	//”½‘Î•ûŒü‚Ìê‡AƒAƒNƒeƒBƒu‚É
	else{
		gmGmkWaterAreaModeChangeActive(obj_work);
	}
}

// ==========================================================================
// gmGmkWaterAreaMainActive
/*!
 *	ƒMƒ~ƒbƒN…–Ê ƒAƒNƒeƒBƒuƒ‚[ƒh‚ÌƒƒCƒ“ˆ—
 *	@param rect_work	[io] ‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkWaterAreaMainActive( OBS_OBJECT_WORK *obj_work )
{
	s32 time = gmGmkWaterAreaUserWorkGetTime( obj_work );
	s32 counter = gmGmkWaterAreaUserTimerGetCounter( obj_work );
	gmGmkWaterAreaUserTimerAddCounter( obj_work, 1 );

	//w’èŠÔ‚ª‰ß‚¬‚½‚ç‘Ò‹@ƒ‚[ƒh‚É•ÏX
	if ( counter >= time*60 ){
		gmGmkWaterAreaModeChangeWait( obj_work );
	}
}

// ==========================================================================
//ƒ†[ƒUƒ[ƒN
// ==========================================================================

// ==========================================================================
// gmGmkWaterAreaUserWorkSetLevel
/*!
 *	ƒ†[ƒUƒ[ƒN‚ğ…–ÊƒŒƒxƒ‹‚Æ‚µ‚Äİ’è
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param length	[in] …–ÊƒŒƒxƒ‹
 */
// ==========================================================================
void gmGmkWaterAreaUserWorkSetLevel( OBS_OBJECT_WORK* obj_work, u16 level )
{
	obj_work->user_work |= level << 16;
}

// ==========================================================================
// gmGmkWaterAreaUserWorkLevel
/*!
 *	ƒ†[ƒUƒ[ƒN‚ğ…–ÊƒŒƒxƒ‹‚Æ‚µ‚Äæ“¾
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return …–ÊƒŒƒxƒ‹
 */
// ==========================================================================
u16 gmGmkWaterAreaUserWorkGetLevel( const OBS_OBJECT_WORK* obj_work )
{
	return (u16)(obj_work->user_work >> 16);
}

// ==========================================================================
// gmGmkWaterAreaUserWorkSetTime
/*!
 *	ƒ†[ƒUƒ[ƒN‚ğ…–Ê‘Œ¸ŠÔ‚Æ‚µ‚Äİ’è
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param time		[in] …–Ê‘Œ¸ŠÔi•bj
 */
// ==========================================================================
void gmGmkWaterAreaUserWorkSetTime( OBS_OBJECT_WORK* obj_work, u16 time )
{
	obj_work->user_work &= 0xffff0000;
	obj_work->user_work |= time;
}

// ==========================================================================
// gmGmkWaterAreaUserWorkGetTime
/*!
 *	ƒ†[ƒUƒ[ƒN‚ğ…–Ê‘Œ¸ŠÔ‚Æ‚µ‚Äæ“¾
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return …–Ê‘Œ¸ŠÔi•bj
 */
// ==========================================================================
u16 gmGmkWaterAreaUserWorkGetTime( const OBS_OBJECT_WORK* obj_work )
{
	return (u16)obj_work->user_work;
}

// ==========================================================================
// gmGmkWaterAreaUserTimerSetCounter
/*!
 *	ƒ†[ƒUƒ^ƒCƒ}‚ğãƒJƒEƒ“ƒ^‚Æ‚µ‚Äİ’è
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param length	[in] ƒJƒEƒ“ƒ^
 */
// ==========================================================================
void gmGmkWaterAreaUserTimerSetCounter( OBS_OBJECT_WORK* obj_work, s32 time )
{
	obj_work->user_timer = time;
}

// ==========================================================================
// gmGmkWaterAreaUserTimerAddCounter
/*!
 *	ƒ†[ƒUƒ^ƒCƒ}‚ğƒJƒEƒ“ƒ^‚Æ‚µ‚Ä’Ç‰Á
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param length	[in] ƒJƒEƒ“ƒ^
 */
// ==========================================================================
void gmGmkWaterAreaUserTimerAddCounter( OBS_OBJECT_WORK* obj_work, s32 time )
{
	obj_work->user_timer += time;
}

// ==========================================================================
// gmGmkWaterAreaUserTimerGetCounter
/*!
 *	ƒ†[ƒUƒ^ƒCƒ}‚ğƒJƒEƒ“ƒ^‚Æ‚µ‚Äæ“¾
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return ƒJƒEƒ“ƒ^
 */
// ==========================================================================
s32 gmGmkWaterAreaUserTimerGetCounter( const OBS_OBJECT_WORK* obj_work )
{
	return obj_work->user_timer;
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
