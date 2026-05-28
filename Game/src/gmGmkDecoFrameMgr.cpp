// ==========================================================================
/*!
  @file gmGmkDecoFrameMgr.cpp
  @brief ÉMÉ~ÉbÉN ëïè¸ã§í ÉtÉåÅ[ÉÄä«óù

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkDecoFrameMgr.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 *	GMS_EVE_RECORD_EVENT
 *		left		: Ç»Çµ
 *		top			: Ç»Çµ
 *		width		: Ç»Çµ
 *		height		: Ç»Çµ
 *
 *		flag 1:ÉäÉZÉbÉg
 * Å@Å~Å~Å~Å~Å~Å~Å~Å~Å~Å° on  :ÉtÉåÅ[ÉÄÇ0Ç…ÉäÉZÉbÉgÇ∑ÇÈ
 * Å@Å~Å~Å~Å~Å~Å~Å~Å~Å~Å† off :Ç»Çµ
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmDeco.h"

#include "gmGmkDecoFrameMgr.h"


//----- Definitions ---------------------------------------------------------



#define GMD_GMK_DECO_FRAME_MGR_FRAME_MAX	((u8)-1)


//
#define GMD_GMK_DECO_FRAME_MGR_FLAG_USE_RESET_FRAM	(1<<0)	//ÉtÉåÅ[ÉÄÇ0Ç…ÉäÉZÉbÉgÇ∑ÇÈ


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static GMS_ENEMY_3D_WORK* gmGmkDecoFrameMgrLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									u8 type );
static void gmGmkDecoFrameMgrInit( OBS_OBJECT_WORK* obj_work );
static void gmGmkDecoFrameMgrMainFunc( OBS_OBJECT_WORK *obj_work );

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkDecoFrameMgrManagerInit
/*!
 *	ÉMÉ~ÉbÉNèâä˙âªä÷êîÅ@ëïè¸ã§í ÉtÉåÅ[ÉÄä«óùê∂ê¨ä«óù
 *
 *	@param eve_rec	[io] ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param pos_x	[in] èoåªç¿ïW
 *	@param pos_y	[in] 
 *	@param type		[in] èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkDecoFrameMgrInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	//ê∂ê¨çœÇ›
	if ( 0 != eve_rec->byte_param[1] ){
		//ÉXÉLÉbÉvÉtÉâÉO
		eve_rec->pos_x = GMD_EVE_RECORD_CMD_SKIP;
		return NULL;
	}

	//ÉäÉZÉbÉg
	if ( eve_rec->flag & GMD_GMK_DECO_FRAME_MGR_FLAG_USE_RESET_FRAM ){
		s32 frame_index = 0;
		if (  eve_rec->id == GMD_EVENT_ID_GMK_DECO_FRAME_MGR_WAY ){
			frame_index = 1;
		}
		GmDecoSetFrameMotion( 0, frame_index );

		//ÉXÉLÉbÉvÉtÉâÉO
		eve_rec->pos_x = GMD_EVE_RECORD_CMD_SKIP;

		eve_rec->byte_param[1] = 1;
		return NULL;
	}

	//ÉIÉuÉWÉFÉNÉgçÏê¨
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)gmGmkDecoFrameMgrLoadObjNoModel( eve_rec, pos_x, pos_y, type );
	amAssert( obj_work );

	//èâä˙âª
	gmGmkDecoFrameMgrInit( obj_work );

	return obj_work;
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkDecoFrameMgrLoadObjNoModel
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
GMS_ENEMY_3D_WORK* gmGmkDecoFrameMgrLoadObjNoModel( 
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
			"GMK_DECO_FRAME_MGR");
	amAssert( gimmick_work );

	//-------------------------------------------------
	//ãÈå`èâä˙âª
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}

// ==========================================================================
// gmGmkDecoFrameMgrInit
/*!
 *	ÉMÉ~ÉbÉNÅ@ëïè¸ã§í ÉtÉåÅ[ÉÄä«óùÅ@èâä˙âª
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉg
 *	@param type			[in] ëïè¸ã§í ÉtÉåÅ[ÉÄä«óùÉ^ÉCÉv	
 */
// ==========================================================================
void gmGmkDecoFrameMgrInit( OBS_OBJECT_WORK* obj_work )
{
	//-------------------------------------------------
	// ÉèÅ[ÉNê›íË
	//-------------------------------------------------
	//ÉtÉâÉO
	obj_work->disp_flag	|= OBD_DISP_NODISP;
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;
	obj_work->flag |= OBD_OBJECT_NOCLIP;

	//-------------------------------------------------
	// ÉÅÉCÉìèàóù
	//-------------------------------------------------
	obj_work->ppFunc = gmGmkDecoFrameMgrMainFunc;
	obj_work->ppOut = NULL;
	obj_work->ppMove = NULL;

	//ãLò^
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	obj_work->user_timer = gimmick_work->ene_com.eve_rec->byte_param[1]*2;
}

// ==========================================================================
// gmGmkDecoFrameMgrMainFunc
/*!
 *	ÉMÉ~ÉbÉNÅ@ëïè¸ã§í ÉtÉåÅ[ÉÄä«óùÅ@ÉÅÉCÉì
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkDecoFrameMgrMainFunc( OBS_OBJECT_WORK *obj_work )
{

	if ( obj_work->user_timer >= GMD_GMK_DECO_FRAME_MGR_FRAME_MAX*2 ){
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		return;
	}
	++obj_work->user_timer;

	//ê›íË
	s32 frame_index = 0;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	if ( gimmick_work->ene_com.eve_rec->id == GMD_EVENT_ID_GMK_DECO_FRAME_MGR_WAY ){
		frame_index = 1;
	}
	GmDecoSetFrameMotion( obj_work->user_timer, frame_index );

	//ÉtÉåÅ[ÉÄãLâØ
	gimmick_work->ene_com.eve_rec->byte_param[1] = (u8)(obj_work->user_timer/2);
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
