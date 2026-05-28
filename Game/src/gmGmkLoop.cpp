// ==========================================================================
/*!
  @file gmGmkLoop.cpp
  @brief ƒMƒ~ƒbƒN ƒ‹[ƒv

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkLoop.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 *	GMS_EVE_RECORD_EVENT
 *		left		: ƒ‹[ƒvæƒuƒƒbƒNƒIƒtƒZƒbƒgiXj
 *		top			: ƒ‹[ƒvæƒuƒƒbƒNƒIƒtƒZƒbƒgiYj
 *		width		: •Ó‚è‹éŒ`•
 *		height		: •Ó‚è‹éŒ`‚‚³
 *
 *		flag		: ‚È‚µ
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectZone.h"
#include "gmCamera.h"
#include "gmRing.h"

#include "gmGmkLoop.h"


//----- Definitions ---------------------------------------------------------

#define GMD_GMK_LOOP_BLOCK_SIZE	(64)

#define GMD_GMK_LOOP_FLAG_EXECUTE	(1<<0)	//ƒ‹[ƒvÀs

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static GMS_ENEMY_3D_WORK* gmGmkLoopLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type );
static void gmGmkLoopInit( OBS_OBJECT_WORK* obj_work );
static void gmGmkLoopSetRect( OBS_OBJECT_WORK* obj_work );
static void gmGmkLoopDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect );
static void gmGmkLoopMainFunc( OBS_OBJECT_WORK* obj_work );
static void gmGmkLoopExecute( OBS_OBJECT_WORK* obj_work );
static void gmGmkLoopExecuteObj( fx32 loop_x, fx32 loop_y, GME_OBJTYPE obj_type );
static void gmGmkLoopExecuteEffect( fx32 loop_x, fx32 loop_y );
static void gmGmkLoopExecuteRing( fx32 loop_x, fx32 loop_y );
static void gmGmkLoopExecuteCamera( fx32 loop_x, fx32 loop_y );

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkLoopInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒ‹[ƒvŠÇ—
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkLoopInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkLoopLoadObjNoModel( eve_rec, pos_x, pos_y, type );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK* obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkLoopInit( obj_work );

	return obj_work;
}


//----- Local Functions -----------------------------------------------------

// ==========================================================================
// gmGmkLoopLoadObjNoModel
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
GMS_ENEMY_3D_WORK* gmGmkLoopLoadObjNoModel( 
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
			"GMK_LOOP");
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}

// ==========================================================================
// gmGmkLoopInit
/*!
 *	ƒMƒ~ƒbƒN@ƒ‹[ƒvŠÇ—@‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkLoopInit( OBS_OBJECT_WORK* obj_work )
{
	//‹éŒ`
	gmGmkLoopSetRect( obj_work );

	//ƒtƒ‰ƒO
	obj_work->move_flag |= (OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);

	//ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmGmkLoopMainFunc;
}

// ==========================================================================
// gmGmkLoopSetRect
/*!
 *	ƒMƒ~ƒbƒN@ƒ‹[ƒvŠÇ—@‹éŒ`İ’è
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkLoopSetRect( OBS_OBJECT_WORK* obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	
	s16 left = (s16)(-gimmick_work->ene_com.eve_rec->width*GMD_GMK_LOOP_BLOCK_SIZE/2);	
	s16 right = (s16)(gimmick_work->ene_com.eve_rec->width*GMD_GMK_LOOP_BLOCK_SIZE/2);
	s16 top = (s16)(-gimmick_work->ene_com.eve_rec->height*GMD_GMK_LOOP_BLOCK_SIZE/2);
	s16 bottom = (s16)(gimmick_work->ene_com.eve_rec->height*GMD_GMK_LOOP_BLOCK_SIZE/2);

	ObjRectWorkZSet(
			rect_work,
			left, top, -500,
			right, bottom, 500);
	rect_work->flag |= OBD_RECT_OUT;
	
	//–hŒä—p
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	rect_work->ppDef = gmGmkLoopDefFunc;
}

// ==========================================================================
// gmGmkLoopDefFunc
/*!
 *	ƒMƒ~ƒbƒN ƒ‹[ƒvŠÇ— ‹éŒ`ŠÖ”
 *
 *	@param own_rect	[in] ©g‚Ì‹éŒ`ƒ[ƒN
 *	@param target_rect	[in] ‘Šè‚Ì‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkLoopDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect )
{
	UNREFERENCED_PARAMETER( target_rect );

	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)own_rect->parent_obj;
	obj_work->user_flag |= GMD_GMK_LOOP_FLAG_EXECUTE;
}

// ==========================================================================
// gmGmkLoopMainFunc
/*!
 *	ƒMƒ~ƒbƒN ƒ‹[ƒv ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒg
 */
// ==========================================================================
void gmGmkLoopMainFunc( OBS_OBJECT_WORK* obj_work )
{
	//Às
	if ( obj_work->user_flag & GMD_GMK_LOOP_FLAG_EXECUTE) {
		gmGmkLoopExecute(obj_work);
		obj_work->user_flag &= ~GMD_GMK_LOOP_FLAG_EXECUTE;
	}
}

// ==========================================================================
// gmGmkLoopExecute
/*!
 *	ƒMƒ~ƒbƒN ƒ‹[ƒv
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒg
 */
// ==========================================================================
void gmGmkLoopExecute( OBS_OBJECT_WORK* obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;

	//ƒ‹[ƒv—Ê
	fx32 loop_x = (fx32)(gimmick_work->ene_com.eve_rec->left * GMD_GMK_LOOP_BLOCK_SIZE * FX32_ONE);
	fx32 loop_y = (fx32)(gimmick_work->ene_com.eve_rec->top * GMD_GMK_LOOP_BLOCK_SIZE * FX32_ONE);

	//ƒvƒŒƒCƒ„
	gmGmkLoopExecuteObj( loop_x, loop_y, GMD_OBJTYPE_PLAYER );

	//“G
	gmGmkLoopExecuteObj( loop_x, loop_y, GMD_OBJTYPE_ENEMY );

	//ƒGƒtƒFƒNƒg
	gmGmkLoopExecuteEffect( loop_x, loop_y );

	//ƒŠƒ“ƒO
	gmGmkLoopExecuteRing( loop_x, loop_y );

	//ƒJƒƒ‰
	gmGmkLoopExecuteCamera( loop_x, loop_y );

	//ƒ‹[ƒvæ‚ÌƒIƒuƒWƒFƒNƒg‚ğ¶¬
	GmEveMgrCreateEventLcd( GMD_EVE_SEARCH_PROC_FLAG_NONE );
}


// ==========================================================================
// gmGmkLoopExecuteObj
/*!
 *	ƒMƒ~ƒbƒN ƒ‹[ƒviƒIƒuƒWƒFƒNƒgj
 *
 *	@param loop_x	[in] ƒ‹[ƒvƒTƒCƒY
 *	@param loop_y	[in] ƒ‹[ƒvƒTƒCƒY
 *	@param obj_type	[in] ‘ÎÛƒIƒuƒWƒFƒNƒg‚Ìƒ^ƒCƒv
 */
// ==========================================================================
void gmGmkLoopExecuteObj( fx32 loop_x, fx32 loop_y, GME_OBJTYPE obj_type )
{
	OBS_OBJECT_WORK* obj_work = ObjObjectSearchRegistObject( NULL, (u16)obj_type );
	while ( obj_work ){
		obj_work->pos.x += loop_x;
		obj_work->pos.y += loop_y;
		
		//Ÿ‚ÌƒIƒuƒWƒFƒNƒg
		obj_work = ObjObjectSearchRegistObject( obj_work, (u16)obj_type );
	}
}

// ==========================================================================
// gmGmkLoopExecuteEffect
/*!
 *	ƒMƒ~ƒbƒN ƒ‹[ƒviƒGƒtƒFƒNƒgj
 *
 *	@param loop_x	[in] ƒ‹[ƒvƒTƒCƒY
 *	@param loop_y	[in] ƒ‹[ƒvƒTƒCƒY
 */
// ==========================================================================
void gmGmkLoopExecuteEffect( fx32 loop_x, fx32 loop_y )
{
	OBS_OBJECT_WORK* obj_work = ObjObjectSearchRegistObject( NULL, (u16)GMD_OBJTYPE_EFFECT );
	while ( obj_work ){
		obj_work->pos.x += loop_x;
		obj_work->pos.y += loop_y;

		if ( obj_work->obj_3des ){
			//•¡”•`‰æ
			GmEffect3DESSetDuplicateDraw( 
					(GMS_EFFECT_3DES_WORK*)obj_work,
					FX_FX32_TO_F32(loop_x),
					FX_FX32_TO_F32(loop_y),
					0 );
		}
		
		//Ÿ‚ÌƒIƒuƒWƒFƒNƒg
		obj_work = ObjObjectSearchRegistObject( obj_work, (u16)GMD_OBJTYPE_EFFECT );
	}
}

// ==========================================================================
// gmGmkLoopExecuteRing
/*!
 *	ƒMƒ~ƒbƒN ƒ‹[ƒviƒŠƒ“ƒOj
 *
 *	@param loop_x	[in] ƒ‹[ƒvƒTƒCƒY
 *	@param loop_y	[in] ƒ‹[ƒvƒTƒCƒY
 */
// ==========================================================================
void gmGmkLoopExecuteRing( fx32 loop_x, fx32 loop_y )
{
	GMS_RING_SYS_WORK* ring_system_work = GmRingGetWork();
	amAssert( ring_system_work );

	//ƒ_ƒ[ƒWƒŠƒ“ƒO
	GMS_RING_WORK* iter = ring_system_work->damage_ring_list_start;
	while( iter ){
		iter->pos.x += loop_x;
		iter->pos.y += loop_y;
		iter = iter->post_ring;
	}
}

// ==========================================================================
// gmGmkLoopExecuteCamera
/*!
 *	ƒMƒ~ƒbƒN ƒ‹[ƒviƒJƒƒ‰j
 *
 *	@param loop_x	[in] ƒ‹[ƒvƒTƒCƒY
 *	@param loop_y	[in] ƒ‹[ƒvƒTƒCƒY
 */
// ==========================================================================
void gmGmkLoopExecuteCamera( fx32 loop_x, fx32 loop_y )
{
	const OBS_CAMERA* obj_camera = ObjCameraGet( GME_CAMERA_NO_MAIN );
	GmCameraPosSet( 
			FX_F32_TO_FX32(obj_camera->pos.x) + loop_x,
			-FX_F32_TO_FX32(obj_camera->pos.y) + loop_y,
			FX_F32_TO_FX32(obj_camera->pos.z) );
	ObjObjectCameraSet(	FXM_FLOAT_TO_FX32( obj_camera->disp_pos.x - (float)(OBD_LCD_X/2)),
						FXM_FLOAT_TO_FX32(-obj_camera->disp_pos.y - (float)(OBD_LCD_Y/2)),
						FXM_FLOAT_TO_FX32( obj_camera->disp_pos.x - (float)(OBD_LCD_X/2)),
						FXM_FLOAT_TO_FX32(-obj_camera->disp_pos.y - (float)(OBD_LCD_Y/2)));
						
	// ƒNƒŠƒbƒsƒ“ƒOƒJƒƒ‰İ’è
	GmCameraSetClipCamera((OBS_CAMERA*)obj_camera);
}




// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
