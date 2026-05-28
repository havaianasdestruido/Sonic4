// ==========================================================================
/*!
  @file gmGmkBoss3Route.cpp
  @brief ƒMƒ~ƒbƒN ƒ{ƒX3Œo˜H

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkBoss3Route.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 *	GMS_EVE_RECORD_EVENT
 *		left		: –Ú“I’nƒuƒƒbƒN‚ÌƒIƒtƒZƒbƒgX
 *		top			: –Ú“I’nƒuƒƒbƒN‚ÌƒIƒtƒZƒbƒgY
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

#include "gmGmkBoss3Route.h"


//----- Definitions ---------------------------------------------------------

#define GMD_GMK_BOSS3_ROUTE_FLAG_WAIT	(1 << 0)	//‘Ò‚Âƒtƒ‰ƒO
#define GMD_GMK_BOSS3_ROUTE_FLAG_GOAL	(1 << 1)	//“’…


#define GMD_GMK_BOSS3_ROUTE_HIT_LENGTH	( 64*FX32_ONE )	//Õ“Ë”»’è‹——£
#define GMD_GMK_BOSS3_ROUTE_BLOCK_SIZE	( 64 )			//ƒuƒƒbƒNƒTƒCƒY

#define GMD_GMK_BOSS3_ROUTE_WAIT_OFFSET	( 96 )	//ˆÚ“®‘Ò‚¿—p‰æ–ÊŠO”»’èƒIƒtƒZƒbƒg

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static GMS_ENEMY_3D_WORK* gmGmkBoss3RouteLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type );
static void gmGmkBoss3RouteInit( OBS_OBJECT_WORK* obj_work );
static BOOL gmGmkBoss3RouteCheckHit( const OBS_OBJECT_WORK* target_obj_work, const OBS_OBJECT_WORK* gimmick_obj_work );
static BOOL gmGmkBoss3RouteSetMoveParam( OBS_OBJECT_WORK* target_obj_work, const OBS_OBJECT_WORK* gimmick_obj_work );

static void gmGmkBoss3RouteMainFunc( OBS_OBJECT_WORK *obj_work );

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkBoss3RouteManagerInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒ{ƒX3Œo˜H¶¬ŠÇ—
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkBoss3RouteInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkBoss3RouteLoadObjNoModel( eve_rec, pos_x, pos_y, type );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkBoss3RouteInit( obj_work );

	return obj_work;
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkBoss3RouteLoadObjNoModel
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
GMS_ENEMY_3D_WORK* gmGmkBoss3RouteLoadObjNoModel( 
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
			"GMK_BOSS3_ROUTE");
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}

// ==========================================================================
// gmGmkBoss3RouteInit
/*!
 *	ƒMƒ~ƒbƒN@ƒ{ƒX3Œo˜H@‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒg
 *	@param type			[in] ƒ{ƒX3Œo˜Hƒ^ƒCƒv	
 */
// ==========================================================================
void gmGmkBoss3RouteInit( OBS_OBJECT_WORK* obj_work )
{
	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;
	obj_work->flag	|= OBD_OBJECT_NOCLIP;

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = gmGmkBoss3RouteMainFunc;
	obj_work->ppOut = NULL;
	obj_work->ppMove = NULL;
}

// ==========================================================================
// gmGmkBoss3RouteCheckHit
/*!
 *	ƒMƒ~ƒbƒN@ƒ{ƒX3Œo˜H
 *
 *	@param target_obj_work	[in] ƒ^[ƒQƒbƒg‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param gimmick_obj_work	[in] ƒMƒ~ƒbƒN‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
BOOL gmGmkBoss3RouteCheckHit( const OBS_OBJECT_WORK* target_obj_work, const OBS_OBJECT_WORK* gimmick_obj_work )
{
	//‹——£”»’è
	fx32 distance_x = target_obj_work->pos.x - gimmick_obj_work->pos.x;
	fx32 distance_y = target_obj_work->pos.y - gimmick_obj_work->pos.y;

	if ( (MTM_MATH_ABS(distance_x) > GMD_GMK_BOSS3_ROUTE_HIT_LENGTH)
			|| (MTM_MATH_ABS(distance_y) > GMD_GMK_BOSS3_ROUTE_HIT_LENGTH)
	){
		return FALSE;
	}

	fx32 length = FX_Mul(distance_x, distance_x) + FX_Mul(distance_y, distance_y);
	fx32 hit_length = FX_Mul( GMD_GMK_BOSS3_ROUTE_HIT_LENGTH, GMD_GMK_BOSS3_ROUTE_HIT_LENGTH);

	if ( length > hit_length ){
		return FALSE;
	}

	return TRUE;
}

// ==========================================================================
// gmGmkBoss3RouteSetMoveParam
/*!
 *	ƒMƒ~ƒbƒN@ƒ{ƒX3Œo˜H@ˆÚ“®ƒpƒ‰ƒƒ^‚ğİ’è
 *
 *	@param gimmick_obj_work	[in] ƒMƒ~ƒbƒN‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param target_obj_work	[in] ƒ^[ƒQƒbƒg‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@retval	TRUE:İ’è‚µ‚½
 *	@retval	FALSE:–¢İ’è
 */
// ==========================================================================
BOOL gmGmkBoss3RouteSetMoveParam( OBS_OBJECT_WORK* target_obj_work, const OBS_OBJECT_WORK* gimmick_obj_work )
{
	amAssert( target_obj_work );
	const GMS_ENEMY_3D_WORK* gimmick_work = (const GMS_ENEMY_3D_WORK*)gimmick_obj_work;
	amAssert( gimmick_obj_work );
	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	Float move_speed = (Float)gimmick_work->ene_com.eve_rec->width/10.0f;

	//ƒvƒŒƒCƒ„‚ªæ‚És‚Á‚Ä‚¢‚éê‡
	if ( player_work->obj_work.pos.y < target_obj_work->pos.y ){
		;	//ˆ—‚È‚µ
	}
	//ƒ{ƒX‚ªæ‚És‚Á‚Ä‚¢‚éê‡
	else{
		//‘Ò‹@ƒtƒ‰ƒO‚ªİ’è‚³‚ê‚Ä‚¢‚éê‡
		if ( gimmick_work->ene_com.eve_rec->flag & GMD_GMK_BOSS3_ROUTE_FLAG_WAIT ){
			//‰æ–ÊŠO‚Ì‚Æ‚«‚Í~‚ß‚Ä‚¨‚­
			if ( ObjViewOutCheck(target_obj_work->pos.x, target_obj_work->pos.y, GMD_GMK_BOSS3_ROUTE_WAIT_OFFSET, 0, 0, 0, 0) ){
				target_obj_work->spd.x = 0;
				target_obj_work->spd.y = 0;
				return FALSE;
			}
		}
	}

	//–Ú“I’nÀ•W
	fx32 dest_pos_x_fx32 = gimmick_obj_work->pos.x + gimmick_work->ene_com.eve_rec->left*GMD_GMK_BOSS3_ROUTE_BLOCK_SIZE*FX32_ONE;
	fx32 dest_pos_y_fx32 = gimmick_obj_work->pos.y + gimmick_work->ene_com.eve_rec->top*GMD_GMK_BOSS3_ROUTE_BLOCK_SIZE*FX32_ONE;

	Float dest_pos_x_f = FX_FX32_TO_F32(dest_pos_x_fx32);
	Float dest_pos_y_f = FX_FX32_TO_F32(dest_pos_y_fx32);
	Float target_pos_x = FX_FX32_TO_F32(target_obj_work->pos.x);
	Float target_pos_y = FX_FX32_TO_F32(target_obj_work->pos.y);

	Float distance_x = dest_pos_x_f - target_pos_x;
	Float distance_y = dest_pos_y_f - target_pos_y;
	
	NNS_VECTOR dir;
	amVectorSet( 
			&dir,
			distance_x,
			distance_y,
			0 );
	Float inv_length = 1.0f / nnLengthVector( &dir );

	
	Float speed_x = distance_x * inv_length * move_speed;
	Float speed_y = distance_y * inv_length * move_speed;
	
	//ƒXƒs[ƒhİ’è
	target_obj_work->spd.x = FX_F32_TO_FX32( speed_x );
	target_obj_work->spd.y = FX_F32_TO_FX32( speed_y );

	return TRUE;
}

// ==========================================================================
// gmGmkBoss3RouteMainFunc
/*!
 *	ƒMƒ~ƒbƒN@ƒ{ƒX3Œo˜H@ƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkBoss3RouteMainFunc( OBS_OBJECT_WORK *obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* target_obj_work = ObjObjectSearchRegistObject( NULL, GMD_OBJTYPE_ENEMY );
	while ( target_obj_work ){
		const GMS_ENEMY_3D_WORK* target_gimmick_work = (const GMS_ENEMY_3D_WORK*)target_obj_work;
		//ƒ{ƒX3‚È‚ç
		if ( target_gimmick_work->ene_com.eve_rec->id == GMD_EVENT_ID_BOSS3_BODY ){
			//ƒqƒbƒg”»’è
			if ( gmGmkBoss3RouteCheckHit(target_obj_work, obj_work) ){
				//ƒS[ƒ‹
				if ( gimmick_work->ene_com.eve_rec->flag & GMD_GMK_BOSS3_ROUTE_FLAG_GOAL ){		
					target_obj_work->spd.x = 0;
					target_obj_work->spd.y = 0;
					target_obj_work->user_flag = 1;	//ƒS[ƒ‹‚µ‚½‚çƒ†[ƒUƒtƒ‰ƒO‚ğ—§‚Ä‚Ä‚¨‚­

					//İ’è‚µ‚½‚ç©•ª‚ğÁ‚·
					obj_work->flag |= OBD_OBJECT_TASKCLEAR;
					obj_work->ppFunc = NULL;

					//•œŠˆ‚µ‚È‚¢
					gimmick_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_DIE;
				}
				else{			
					if ( gmGmkBoss3RouteSetMoveParam( target_obj_work, obj_work ) ){
						//İ’è‚µ‚½‚ç©•ª‚ğÁ‚·
						obj_work->flag |= OBD_OBJECT_TASKCLEAR;
						obj_work->ppFunc = NULL;

						//•œŠˆ‚µ‚È‚¢
						gimmick_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_DIE;
					} 
				}
			}
			return;
		}

		//Ÿ‚ÌƒIƒuƒWƒFƒNƒg
		target_obj_work = ObjObjectSearchRegistObject( target_obj_work, GMD_OBJTYPE_ENEMY );
	}
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
