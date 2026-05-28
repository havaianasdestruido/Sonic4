// ==========================================================================
/*!
  @file gmGmkTarzanRope.cpp
  @brief ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkTarzanRope.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 * ‹éŒ`¶ãXˆÊ’u   ƒ[ƒv‚Ì’·‚³ƒ^ƒCƒv
 * ‹éŒ`¶ãYˆÊ’u   ‚È‚µ
 * ‹éŒ`‰¡ƒTƒCƒY    ‚È‚µ
 * ‹éŒ`cƒTƒCƒY    ‚È‚µ
 *
 * ƒtƒ‰ƒO          ‚È‚µ
 * 
 * 
 * ƒ[ƒv‚Ì’·‚³ƒ^ƒCƒv‚Í3í—Ş
 * İ’è‚³‚ê‚Ä‚¢‚é’l‚©‚çƒ^ƒCƒv‚ğZo
 * 
 *  0`19  1.0”{ƒ^ƒCƒv
 * 20`49  1.2”{ƒ^ƒCƒv
 * 50`    1.5”{ƒ^ƒCƒv
 * 
 * ¦ƒ‚ƒfƒ‹ƒfƒUƒCƒ“•ÏX‚É‚æ‚èAİ’è’l‚Åˆø‚«L‚Î‚·d—l‚Í”p~
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
#include "gmPadVib.h"

#include "akMath.h"

#include "gmGmkTarzanRope.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "common/model/gmk_t_rope_mdl.hmb"
#include "common/model/gmk_t_rope_mtn.hmb"


//----- Definitions ---------------------------------------------------------

#define _TEST_LENGTH	(0)

//ƒ^[ƒUƒ“ƒ[ƒvƒ‚[ƒh
enum GME_GMK_TARZAN_ROPE_MODE{
	GMD_GMK_TARZAN_ROPE_MODE_WAIT = 0,		//ƒ^[ƒUƒ“ƒ[ƒv‘Ò‚¿
	GMD_GMK_TARZAN_ROPE_MODE_START,			//ƒ^[ƒUƒ“ƒ[ƒvŠJn
	GMD_GMK_TARZAN_ROPE_MODE_STOP,			//ƒ^[ƒUƒ“ƒ[ƒvI—¹

	GMD_GMK_TARZAN_ROPE_MODE_NUM
};

//ƒ^[ƒUƒ“ƒ[ƒvƒ^ƒCƒv
enum GME_GMK_TARZAN_ROPE_TYPE{
	GMD_GMK_TARZAN_ROPE_TYPE_NORMAL = 0,	//’Êí
	GMD_GMK_TARZAN_ROPE_TYPE_LEFT,			//¶
	GMD_GMK_TARZAN_ROPE_TYPE_RIGHT,			//‰E

	GMD_GMK_TARZAN_ROPE_TYPE_NUM,
	GMD_GMK_TARZAN_ROPE_TYPE_INVALID = -1
};

#define GMD_GMK_TARZAN_ROPE_SPEED				(0x00000800L)	//ƒ^[ƒUƒ“ƒ[ƒvˆÚ“®—Ê
#define GMD_GMK_TARZAN_ROPE_TARGET_NODE_INDEX	(13)			//ƒ‚ƒfƒ‹ƒm[ƒh”
#define GMD_GMK_TARZAN_ROPE_PLAYER_OFFSET_Y		(-5.0f)			//ƒvƒŒƒCƒ„ƒIƒtƒZƒbƒgY

#define GMD_GMK_TARZAN_ROPE_LENGTH_NUM	(3)

#define GMD_GMK_TARZAN_ROPE_FULCRUM			(32)				// Yx“_ˆÊ’uƒIƒtƒZƒbƒg
#define GMD_GMK_TARZAN_ROPE_LENGTH			(96 << FX32_SHIFT)	// ƒ^[ƒUƒ“ƒ[ƒv‚Ì‘S’·
#define GMD_GMK_TARZAN_ROPE_LENGTH_F		(20.0f)				// ƒ^[ƒUƒ“ƒ[ƒv‚Ì‘S’·
#define GMD_GMK_TARZAN_ROPE_FALL_SPD		(6 << FX32_SHIFT)	// ƒ^[ƒUƒ“ƒ[ƒvŠŠ‚è—‚¿‚é‘¬“x
	
#define GM_GMK_TARZAN_ROPE_ANGLE_MAX		(0x4000)		//Å‘åŠp“x
#define GM_GMK_TARZAN_ROPE_ANGLE_LOW		(0x0700)		//ƒL[•â³‚·‚éŠp“x
#define GM_GMK_TARZAN_ROPE_ANGLE_STOP		(0x00d0)		//’â~‚Æ‚İ‚È‚·Šp“x

#define GM_GMK_TARZAN_ROPE_ANGLE_ADD		(0x0084)		//ƒL[‚Å‘‰Á‚·‚éŠp“x
#define GM_GMK_TARZAN_ROPE_ANGLE_ADD_N		(0x001f)		//ƒL[‚Å‘‰Á‚·‚éŠp“xi‹tŒü‚«‚Ì‚Æ‚«j
#define GM_GMK_TARZAN_ROPE_ANGLE_DEL		(0x0030)		//Œ¸Š

#define GM_GMK_TARZAN_ROPE_ANGLE_JUMP		(0x0700)		//ƒWƒƒƒ“ƒv—LŒø‚ ‚»‚Ñ

#define GM_GMK_TARZAN_ROPE_SPEED			(0x0780)		//ƒXƒs[ƒh
#define GM_GMK_TARZAN_ROPE_SPEED_WIDTH		(0x5000)		//ƒXƒs[ƒhZo—p•

#define GM_GMK_TARZAN_ROPE_MOTION_FRAME_BACK		(34)	//‘«‚ğŒã‚ë‚ÉU‚èã‚°‚éƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€

#if _IPHONE
#define GMD_GMK_TARZAN_ROPE_CATCH_WAIT			(0x0a)			//!< ƒ^[ƒUƒ“ƒ[ƒv’Í‚İ‘Ò‚¿
#define GMD_GMK_TARZAN_ROPE_CATCH_WAIT_BIT		(0xff)			//!< ƒ^[ƒUƒ“ƒ[ƒv’Í‚İ‘Ò‚¿İ’èƒrƒbƒg
#endif // _IPHONE

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

static void gmGmkTarzanRopeMotionCallback( const AMS_MOTION* motion, const NNS_OBJECT* obj, void* val );

static GMS_ENEMY_3D_WORK* gmGmkTarzanRopeLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_TARZAN_ROPE_TYPE type );
static GMS_ENEMY_3D_WORK* gmGmkTarzanRopeLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_TARZAN_ROPE_TYPE type );

//---------------------------------------------------------
//ƒ^[ƒUƒ“ƒ[ƒvİ’è
//---------------------------------------------------------
static void gmGmkTarzanRopeInit( 
								OBS_OBJECT_WORK *obj_work, 
								GME_GMK_TARZAN_ROPE_TYPE type, 
								Float length );
static void gmGmkTarzanRopeSetRect( 
								   GMS_ENEMY_3D_WORK* gimmick_work,
								   GME_GMK_TARZAN_ROPE_TYPE type );

static void gmGmkTarzanRopeDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect );
static Float gmGmkTarzanRopeCalcFlame( OBS_OBJECT_WORK* obj_work, Angle32 angle );
static void gmGmkTarzanRopeDrawFunc( OBS_OBJECT_WORK* work );


//---------------------------------------------------------
//XVˆ—
//---------------------------------------------------------
static Angle32 gmGmkTarzanRopeUpdateAngleCurrent( 
									   OBS_OBJECT_WORK *obj_work, 
									   Angle32 angle_target, 
									   Angle32 angle_current );
static Angle32 gmGmkTarzanRopeUpdateAngleTarget( 
									  OBS_OBJECT_WORK *obj_work, 
									  Angle32 angle_target, 
									   Angle32 angle_current,
									   BOOL flag_motion_change );
static Angle32 gmGmkTarzanRopeApplyKeyLeft( 
							  OBS_OBJECT_WORK *obj_work, 
							  Angle32 angle_target, 
							  Angle32 angle_current  );
static Angle32 gmGmkTarzanRopeApplyKeyRight( 
							  OBS_OBJECT_WORK *obj_work, 
							  Angle32 angle_target, 
							  Angle32 angle_current  );
static void gmGmkTarzanRopeCheckStop( 
							  OBS_OBJECT_WORK *obj_work, 
							  Angle32 angle_target, 
							  Angle32 angle_current );
static BOOL gmGmkTarzanRopeCheckPlayerJump( 
							  OBS_OBJECT_WORK *obj_work, 
							  Angle32 angle_target, 
							  Angle32 angle_current );
static void gmGmkTarzanRopeUpdatePlayerPos( 
							  OBS_OBJECT_WORK *obj_work );
static void gmGmkTarzanRopeChangeDirMotion( 
									OBS_OBJECT_WORK *obj_work, 
									Angle32 angle_current );
static void gmGmkTarzanRopeUpdatePlayerMotion( 
							  OBS_OBJECT_WORK *obj_work, 
							  Angle32 angle_target, 
							  Angle32 angle_current );


//---------------------------------------------------------
//ƒƒCƒ“ˆ—
//---------------------------------------------------------
static void gmGmkTarzanRopeMainWait( OBS_OBJECT_WORK *obj_work );
static void gmGmkTarzanRopeMainKey( OBS_OBJECT_WORK *obj_work );
#if 0
static void gmGmkTarzanRopeMainKeyLeft( OBS_OBJECT_WORK *obj_work );
static void gmGmkTarzanRopeMainKeyRight( OBS_OBJECT_WORK *obj_work );
#endif
static void gmGmkTarzanRopeMainEnd( OBS_OBJECT_WORK *obj_work );

static Angle32 gmGmkTarzanRopeGetGimmickRotZ( GMS_PLAYER_WORK *ply_work);

//---------------------------------------------------------
//ƒ†[ƒUƒ[ƒN
//---------------------------------------------------------
static void gmGmkTarzanRopeSetUserWorkTargetAngle( OBS_OBJECT_WORK* obj_work, Angle32 angle );
static Angle32 gmGmkTarzanRopeGetUserWorkTargetAngle( OBS_OBJECT_WORK* obj_work );
static Angle32 gmGmkTarzanRopeAddUserWorkTargetAngle( OBS_OBJECT_WORK* obj_work, Angle32 angle );

static void gmGmkTarzanRopeSetUserTimerCurrentAngle( OBS_OBJECT_WORK* obj_work, Angle32 angle );
static Angle32 gmGmkTarzanRopeGetUserTimerCurrentAngle( OBS_OBJECT_WORK* obj_work );
static Angle32 gmGmkTarzanRopeAddUserTimerCurrentAngle( OBS_OBJECT_WORK* obj_work, Angle32 angle );

static void gmGmkTarzanRopeeSetUserFlagType( OBS_OBJECT_WORK* obj_work, GME_GMK_TARZAN_ROPE_TYPE type );
static GME_GMK_TARZAN_ROPE_TYPE gmGmkTarzanRopeGetUserFlagType( const OBS_OBJECT_WORK* obj_work );

#if _IPHONE
static void gmGmkTarzanRopeInitCatchWait(OBS_OBJECT_WORK* obj_work);
static void gmGmkTarzanRopeUpdateCatchWait(OBS_OBJECT_WORK* obj_work);
static u32 gmGmkTarzanRopeGetCatchWait(OBS_OBJECT_WORK* obj_work);
#endif // _IPHONE

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//ƒ‚ƒfƒ‹ID
static const s32 g_gm_gmk_tarzan_rope_model_id[GMD_GMK_TARZAN_ROPE_LENGTH_NUM] = {
	IDB_GMK_T_ROPE_MDL_GMK_T_ROPE_SHAKES02_ZNO,
	IDB_GMK_T_ROPE_MDL_GMK_T_ROPE_SHAKES02_B_ZNO,
	IDB_GMK_T_ROPE_MDL_GMK_T_ROPE_SHAKES02_A_ZNO,
};

//ƒ‚[ƒVƒ‡ƒ“ID
static const s32 g_gm_gmk_tarzan_rope_motion_id[GMD_GMK_TARZAN_ROPE_TYPE_NUM] = {
	IDB_GMK_T_ROPE_MTN_GMK_T_ROPE_SHAKES02_ZNM,
	IDB_GMK_T_ROPE_MTN_GMK_T_ROPE_SHAKES02_ZNM,
	IDB_GMK_T_ROPE_MTN_GMK_T_ROPE_SHAKES02_ZNM,
};

static OBS_ACTION3D_NN_WORK* g_gm_gmk_tarzan_rope_obj_3d_list = NULL;

//ƒAƒNƒeƒBƒu‚ÈƒMƒ~ƒbƒN‚Ì‚ÌÚ’…ƒm[ƒhƒ}ƒgƒŠƒNƒX
static NNS_MATRIX g_gm_gmk_tarzan_rope_active_matrix;

#if _TEST_LENGTH
static Float g_gm_gmk_tarzan_rope_model_trans_y[GMD_GMK_TARZAN_ROPE_TARGET_NODE_INDEX+5] = {0.0f};
static BOOL g_gm_gmk_tarzan_rope_flag_init_model_trans_y = FALSE;
#endif	//_TEST_LENGTH

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkTarzanRopeBuild
/*!
 *	ƒMƒ~ƒbƒN ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[ ƒf[ƒ^\’z
 */
// ==========================================================================
void GmGmkTarzanRopeBuild(void)
{
	g_gm_gmk_tarzan_rope_obj_3d_list = GmGameDBuildRegBuildModel(
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_T_ROPE_MODEL ),
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_T_ROPE_TEX ),
			0	//draw_flag
			);
}

// ==========================================================================
// GmGmkTarzanRopeFlush
/*!
 *	ƒMƒ~ƒbƒN ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
void GmGmkTarzanRopeFlush(void)
{
	AMS_AMB_HEADER* amb_header = (AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_T_ROPE_MODEL );

	GmGameDBuildRegFlushModel( g_gm_gmk_tarzan_rope_obj_3d_list, amb_header->file_num );
	g_gm_gmk_tarzan_rope_obj_3d_list = NULL;
}

// ==========================================================================
// GmGmkTarzanRopeInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ” ƒ^[ƒUƒ“ƒ[ƒv
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] 
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkTarzanRopeInit( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	UNREFERENCED_PARAMETER(type);

	GME_GMK_TARZAN_ROPE_TYPE slider_type = GMD_GMK_TARZAN_ROPE_TYPE_INVALID;
	switch( eve_rec->id ){
	case GMD_EVENT_ID_TARZAN_ROPE:
		slider_type = GMD_GMK_TARZAN_ROPE_TYPE_NORMAL;
		break;
	case GMD_EVENT_ID_TARZAN_ROPE_L:
		slider_type = GMD_GMK_TARZAN_ROPE_TYPE_LEFT;
		break;
	case GMD_EVENT_ID_TARZAN_ROPE_R:
		slider_type = GMD_GMK_TARZAN_ROPE_TYPE_RIGHT;
		break;
	default:
		amAssert(FALSE);
		return NULL;
	}

	//’·‚³•â³’l
	Float length = 1.0f + (Float)eve_rec->left / 100.0f;

	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkTarzanRopeLoadObj( eve_rec, pos_x, pos_y, slider_type );
	amAssert( gimmick_work );
	OBS_OBJECT_WORK*obj_work = &gimmick_work->ene_com.obj_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkTarzanRopeInit( obj_work, slider_type, length );
	return obj_work;
}


//----- Local Functions -----------------------------------------------------

// ==========================================================================
// gmGmkTarzanRopeMotionCallback
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ” ƒ‚[ƒVƒ‡ƒ“—pƒR[ƒ‹ƒoƒbƒN
 *
 *	@param motion	[io] ƒ‚[ƒVƒ‡ƒ“
 *	@param obj		[in] ƒIƒuƒWƒFƒNƒg
 *	@param val		[in] ’l
 */
// ==========================================================================
void gmGmkTarzanRopeMotionCallback( const AMS_MOTION* motion, const NNS_OBJECT* obj, void* val )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)val;

	//ƒ^[ƒQƒbƒg‚ªİ’è‚³‚ê‚Ä‚¢‚éƒMƒ~ƒbƒN‚Ì‚İƒ}ƒgƒŠƒbƒNƒX‚ğæ“¾
	if ( !gimmick_work->ene_com.target_obj ){
		return ;
	}

	// ƒx[ƒXƒ}ƒgƒŠƒNƒXæ“¾
	NNS_MATRIX	base_mtx;
	nnMakeUnitMatrix( &base_mtx );
	nnMultiplyMatrix( &base_mtx, &base_mtx, amMatrixGetCurrent() );
	
	//ŠK‘wƒ}ƒgƒŠƒNƒX‚ğ‹‚ß‚é
	nnCalcNodeMatrixTRSList( 
			&g_gm_gmk_tarzan_rope_active_matrix,
			obj, 
			GMD_GMK_TARZAN_ROPE_TARGET_NODE_INDEX, 
			motion->data, 
			&base_mtx );
}

// ==========================================================================
// gmGmkTarzanRopeLoadObjNoModel
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
GMS_ENEMY_3D_WORK* gmGmkTarzanRopeLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_TARZAN_ROPE_TYPE type )
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
			"GMK_T_ROPE");
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}

// ==========================================================================
// gmGmkTarzanRopeLoadObj
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
GMS_ENEMY_3D_WORK* gmGmkTarzanRopeLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									GME_GMK_TARZAN_ROPE_TYPE type )
{
	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkTarzanRopeLoadObjNoModel(
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
	s32 length_type = 0;
#if _TEST_LENGTH
#else
	if ( eve_rec->left >= 50 ){
		length_type = 2;
	}
	else if ( eve_rec->left >= 20 ){
		length_type = 1;
	}
	else {
		length_type = 0;
	}
#endif
	s32 data_model_index = g_gm_gmk_tarzan_rope_model_id[length_type];

	//“Ç‚İ‚İ
	ObjObjectCopyAction3dNNModel(
		obj_work,
		&g_gm_gmk_tarzan_rope_obj_3d_list[data_model_index],
		&gimmick_work->obj_3d);

	//-------------------------------------------------
	// ƒ‚[ƒVƒ‡ƒ“‰Šú‰»
	//-------------------------------------------------
	ObjObjectAction3dNNMotionLoad( 
			obj_work,
			0,
			FALSE,
			ObjDataGet(GMD_DWORK_NO_GMK_T_ROPE_MTN),
			NULL,
			0,
			NULL
	);
	obj_work->obj_3d->mtn_cb_func = gmGmkTarzanRopeMotionCallback;
	obj_work->obj_3d->mtn_cb_param = obj_work;


#if _TEST_LENGTH
#else
	NNS_OBJECT* object = obj_work->obj_3d->object;
	Float length = (Float)object->pNodeList[0].Translation.y;
	length *= (Float)eve_rec->left / 30.0f;
	fx32 offset = FX_F32_TO_FX32(length);
	obj_work->pos.y -= offset;

	

#endif


	return gimmick_work;
}

// ==========================================================================
// gmGmkTarzanRopeInit
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param type		[in] ƒ^[ƒUƒ“ƒ[ƒvƒ^ƒCƒv	
 *	@param length	[in] ’·‚³•â³
 */
// ==========================================================================
void gmGmkTarzanRopeInit( 
						 OBS_OBJECT_WORK *obj_work,
						 GME_GMK_TARZAN_ROPE_TYPE type, 
						 Float length )
{
	UNREFERENCED_PARAMETER(length);

	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	//ƒvƒŒƒCƒ„[‚ ‚½‚è
	gmGmkTarzanRopeSetRect( gimmick_work, type );

	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->move_flag = OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// •\— ”½“]‘Î‰
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_DOUBLESIDE;
	
	//ƒ‚[ƒVƒ‡ƒ“
	s32 data_motion_index = g_gm_gmk_tarzan_rope_motion_id[type];
	ObjDrawObjectActionSet3DNN( obj_work, data_motion_index, 0 );
	obj_work->disp_flag |= OBD_DISP_STOP;

	//Šp“x
	Angle32 angle_target = 0;
	switch( type ){
	case GMD_GMK_TARZAN_ROPE_TYPE_NORMAL:
		angle_target = 0;
		break;
	case GMD_GMK_TARZAN_ROPE_TYPE_LEFT:
		angle_target = -GM_GMK_TARZAN_ROPE_ANGLE_MAX;
		break;
	case GMD_GMK_TARZAN_ROPE_TYPE_RIGHT:
		angle_target = GM_GMK_TARZAN_ROPE_ANGLE_MAX;
		break;
	default:
		amAssert(FALSE);
		break;
	}
	gmGmkTarzanRopeSetUserWorkTargetAngle( obj_work, angle_target );
	gmGmkTarzanRopeSetUserTimerCurrentAngle( obj_work, angle_target );

	//ƒ^ƒCƒv
	gmGmkTarzanRopeeSetUserFlagType( obj_work, type );

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = NULL;
	obj_work->ppMove = NULL;
	obj_work->ppOut = gmGmkTarzanRopeDrawFunc;

	//-------------------------------------------------
	//ƒ‚ƒfƒ‹
	//-------------------------------------------------
#if	_TEST_LENGTH
	NNS_OBJECT* object = obj_work->obj_3d->object;
	if ( !g_gm_gmk_tarzan_rope_flag_init_model_trans_y ){
		for ( s32 i = 0; GMD_GMK_TARZAN_ROPE_TARGET_NODE_INDEX+1 > i; ++i ){
			g_gm_gmk_tarzan_rope_model_trans_y[i] = object->pNodeList[i].Translation.y;
		}
		g_gm_gmk_tarzan_rope_flag_init_model_trans_y = TRUE;
	}

	for ( s32 i = 0; GMD_GMK_TARZAN_ROPE_TARGET_NODE_INDEX+1 > i; ++i ){
		object->pNodeList[i].Translation.y = g_gm_gmk_tarzan_rope_model_trans_y[i] * length;
	}
#endif	//_TEST_LENGTH

}

// ==========================================================================
// gmGmkTarzanRopeSetRect
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@‹éŒ`İ’è
 *
 *	@param gimmick_work	[in] ƒMƒ~ƒbƒNƒ[ƒN
 *	@param type			[in] ƒ^[ƒUƒ“ƒ[ƒvƒ^ƒCƒv	
 */
// ==========================================================================
void gmGmkTarzanRopeSetRect( 
							GMS_ENEMY_3D_WORK* gimmick_work,
							GME_GMK_TARZAN_ROPE_TYPE type )
{
	OBS_RECT_WORK* rect_work = &gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
//	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)gimmick_work;

	Float size_per = 1.0f + gimmick_work->ene_com.eve_rec->left / 100.0f;
	s16 size = (s16)(64.0f * size_per);
	switch( type ){
	case GMD_GMK_TARZAN_ROPE_TYPE_NORMAL:
		ObjRectWorkZSet(rect_work, -32, -32, -500, 32, (s16)(size - 32), 500 );
		break;
	case GMD_GMK_TARZAN_ROPE_TYPE_LEFT:
		ObjRectWorkZSet(rect_work, (s16)(-size), -48, -500, 0, -16, 500);
		break;
	case GMD_GMK_TARZAN_ROPE_TYPE_RIGHT:
		ObjRectWorkZSet(rect_work, 0, -48, -500, size, -16, 500);
		break;
	default:
		amAssert(FALSE);
		break;
	}
	rect_work->flag |= OBD_RECT_OUT;
	
	//–hŒä—p
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	rect_work->ppDef = gmGmkTarzanRopeDefFunc;
}

// ==========================================================================
// gmGmkTarzanRopeCalcFlame
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@Šp“x‚©‚çAƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€”‚ğZo
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param angle	[in] Šp“x
 *
 *	@return ƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€”
 */
// ==========================================================================
Float gmGmkTarzanRopeCalcFlame( OBS_OBJECT_WORK* obj_work, Angle32 angle )
{
	OBS_ACTION3D_NN_WORK *obj_3d = obj_work->obj_3d;

	Angle32 angle_90 = GM_GMK_TARZAN_ROPE_ANGLE_MAX;
	Angle32 angle_M90 = -GM_GMK_TARZAN_ROPE_ANGLE_MAX;
	if ( angle > angle_90 ){
		angle = angle_90;
	}
	else if ( angle < angle_M90 ){
		angle = angle_M90;
	}

	Float frame_start = amMotionGetStartFrame( obj_3d->motion, obj_3d->act_id[0] );
	Float frame_end = amMotionGetEndFrame( obj_3d->motion, obj_3d->act_id[0] );

	Float frame_center = (frame_end - frame_start)/2.0f;

	Float frame_per = (Float)angle / (Float)angle_90;

	Float frame_current = frame_center - frame_per * (frame_end - frame_start)/4.0f;
	return frame_current;
}

// ==========================================================================
// gmGmkTarzanRopeDrawFunc
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@•`‰æŠÖ”
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkTarzanRopeDrawFunc( OBS_OBJECT_WORK* obj_work )
{
	OBS_ACTION3D_NN_WORK *obj_3d = obj_work->obj_3d;
	amAssert( obj_3d );
	if (!obj_3d->motion){
		return;
	}

	//Šp“x‚©‚çƒtƒŒ[ƒ€‚É•ÏŠ·
	obj_3d->frame[0] = gmGmkTarzanRopeCalcFlame( obj_work, gmGmkTarzanRopeGetUserTimerCurrentAngle(obj_work) );

	//•`‰æ
	ObjDrawActionSummary( obj_work );
}

// ==========================================================================
// gmGmkTarzanRopeDefFunc
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@ŠJnó‘Ô‹éŒ`ŠÖ”
 *
 *	@param own_rect	[in] ©g‚Ì‹éŒ`ƒ[ƒN
 *	@param target_rect	[in] ‘Šè‚Ì‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmGmkTarzanRopeDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect )
{

	//ƒMƒ~ƒbƒNƒ[ƒN
	OBS_OBJECT_WORK* own_obj_work = own_rect->parent_obj;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)own_obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* target_obj_work = target_rect->parent_obj;
	amAssert( target_obj_work );
	
	//ƒvƒŒƒCƒ„ˆÈŠO‚Í”»’è‚µ‚È‚¢
	if ( target_obj_work->obj_type != GMD_OBJTYPE_PLAYER ){
		return;
	}
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)target_obj_work;

	//Šù‚ÉÀs’†
	if ( player_work->seq_state == GME_PLY_SEQ_STATE_GMK_TARZAN_ROPE ){
		return;
	}

	//U“®
	if ( player_work->seq_state == GME_PLY_SEQ_STATE_HOMING ){
		GMM_PAD_VIB_SMALL();
	}

	GmPlySeqInitTarzanRope( player_work, &gimmick_work->ene_com );
	own_obj_work->ppFunc = gmGmkTarzanRopeMainWait;

	//ƒ^[ƒQƒbƒgİ’è
	gimmick_work->ene_com.target_obj = target_obj_work;

	//ŠJnŠp“x
	GME_GMK_TARZAN_ROPE_TYPE type = gmGmkTarzanRopeGetUserFlagType( own_obj_work );
	Angle32 start_angle = 0;
	if ( type == GMD_GMK_TARZAN_ROPE_TYPE_NORMAL ){
		//X•ûŒüˆÚ“®—Ê
		start_angle = target_obj_work->spd.x >> 2;
		//Y•ûŒüˆÚ“®—Ê
		if ( start_angle > 0x00f0 ){
			start_angle += MTM_MATH_ABS(target_obj_work->spd.y >> 2);
		}
		else if ( start_angle < -0x00f0){
			start_angle -= MTM_MATH_ABS(target_obj_work->spd.y >> 2);
		}
		//”ÍˆÍ’²®
		if ( start_angle > 0x4000 ){
			start_angle = 0x4000;
		}
		else if (start_angle < -0x4000){
			start_angle = -0x4000;
		}
	}
	else{
		start_angle = gmGmkTarzanRopeGetUserTimerCurrentAngle( own_obj_work );
	}

	// ƒvƒŒƒCƒ„[ÚGÀ•WŠm”F
	u16	fulcrum_pos;			// x“_ˆÊ’u
	u16	player_pos;				// ƒvƒŒƒCƒ„[ˆÊ’u
	switch (type) {
		case GMD_GMK_TARZAN_ROPE_TYPE_NORMAL:
			fulcrum_pos = (u16)((own_obj_work->pos.y >> FX32_SHIFT) - GMD_GMK_TARZAN_ROPE_FULCRUM);
			player_pos  = (u16)(target_obj_work->pos.y >> FX32_SHIFT);						// Y‚Å”»’f
			gimmick_work->ene_com.enemy_flag &= 0xffff0000;
			
			if (player_pos > fulcrum_pos) {
				gimmick_work->ene_com.enemy_flag |= (u16)(player_pos - fulcrum_pos);		// x“_‚©‚çƒvƒŒƒCƒ„[‚Ü‚Å‚Ì‹——£‚ğenemy_work(‰ºˆÊ16bit)‚É•Û
			}
			break;
		case GMD_GMK_TARZAN_ROPE_TYPE_LEFT:
			fulcrum_pos = (u16)(own_obj_work->pos.x >> FX32_SHIFT);
			player_pos  = (u16)(target_obj_work->pos.x >> FX32_SHIFT);						// X‚Å”»’f
			gimmick_work->ene_com.enemy_flag &= 0xffff0000;
			
			if (player_pos < fulcrum_pos) {
				gimmick_work->ene_com.enemy_flag |= (u16)(fulcrum_pos - player_pos);		// x“_‚©‚çƒvƒŒƒCƒ„[‚Ü‚Å‚Ì‹——£‚ğenemy_work(‰ºˆÊ16bit)‚É•Û
			}
			break;
		case GMD_GMK_TARZAN_ROPE_TYPE_RIGHT:
			fulcrum_pos = (u16)(own_obj_work->pos.x >> FX32_SHIFT);
			player_pos  = (u16)(target_obj_work->pos.x >> FX32_SHIFT);						// X‚Å”»’f
			gimmick_work->ene_com.enemy_flag &= 0xffff0000;
			
			if (player_pos > fulcrum_pos) {
				gimmick_work->ene_com.enemy_flag |= (u16)(player_pos - fulcrum_pos);		// x“_‚©‚çƒvƒŒƒCƒ„[‚Ü‚Å‚Ì‹——£‚ğenemy_work(‰ºˆÊ16bit)‚É•Û
			}
			break;
		default:
			amAssert(0);
			break;
	}

	gmGmkTarzanRopeSetUserWorkTargetAngle( own_obj_work, start_angle );

	//’Êíƒ^ƒCƒv‚É‚È‚é
	type = GMD_GMK_TARZAN_ROPE_TYPE_NORMAL;
	gmGmkTarzanRopeeSetUserFlagType( own_obj_work, type );
	gmGmkTarzanRopeSetRect( gimmick_work, type );

	//ƒvƒŒƒCƒ„İ’è
	target_obj_work->spd_m = 0;
	target_obj_work->spd.x = 0;
	target_obj_work->spd.y = 0;
	target_obj_work->dir.z = 0;

}

// ==========================================================================
// gmGmkTarzanRopeUpdateAngleCurrent
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@Œ»İŠp“xXV
 *
 *	@param obj_work			[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param angle_target		[in] –Ú“IŠp“x
 *	@param angle_current	[in] Œ»İŠp“x
 *
 *	@return XVŒã‚ÌŒ»İŠp“x
 */
// ==========================================================================
Angle32 gmGmkTarzanRopeUpdateAngleCurrent( 
									   OBS_OBJECT_WORK *obj_work, 
									   Angle32 angle_target, 
									   Angle32 angle_current )
{
	amAssert( obj_work );

	Angle32 angle_target_value = MTM_MATH_ABS(angle_target);
	Angle32 angle_current_value = MTM_MATH_ABS(angle_current);

	//Šp“xŒvZ
	Angle32 angle_add = GM_GMK_TARZAN_ROPE_SPEED;
	Float Angle_per = (Float)(angle_target_value - angle_current_value)/(Float)GM_GMK_TARZAN_ROPE_SPEED_WIDTH;
	if ( Angle_per > 0.55f ){
		Angle_per = 0.55f;
	}
	else if ( Angle_per < -0.55f ){
		Angle_per = -0.55f;
	}
	angle_add = (Angle32)(angle_add * (MTM_MATH_ABS(Angle_per) + 0.05f));

	if ( angle_target < angle_current ){
		angle_add = -angle_add;
	}
	return gmGmkTarzanRopeAddUserTimerCurrentAngle( obj_work, angle_add );
}

// ==========================================================================
// gmGmkTarzanRopeUpdateAngleTarget
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@–Ú“IŠp“xXV
 *
 *	@param obj_work			[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param angle_target		[in] –Ú“IŠp“x
 *	@param angle_current	[in] Œ»İŠp“x
 *	@param flag_motion_change	[in] TRUEFŒü‚«•ÏX‚ÉƒvƒŒƒCƒ„‚Ìƒ‚[ƒVƒ‡ƒ“‚ğ•ÏX‚·‚é
 *
 *	@return XVŒã‚Ì–Ú“IŠp“x
 */
// ==========================================================================
Angle32 gmGmkTarzanRopeUpdateAngleTarget( 
									  OBS_OBJECT_WORK *obj_work, 
									  Angle32 angle_target, 
									   Angle32 angle_current,
									   BOOL flag_motion_change)
{
	amAssert( obj_work );

	if ( angle_target > 0 ){
		angle_target = gmGmkTarzanRopeAddUserWorkTargetAngle( obj_work, -GM_GMK_TARZAN_ROPE_ANGLE_DEL );
		if ( angle_target > GM_GMK_TARZAN_ROPE_ANGLE_MAX ){
			angle_target = GM_GMK_TARZAN_ROPE_ANGLE_MAX;
		}
		else if ( angle_target < 0 ){
			angle_target = 0;
		}
		if ( angle_current >= angle_target){
			//–Ú“IŠp“x‚ğ”½“]
			angle_target = -angle_target;
			gmGmkTarzanRopeSetUserWorkTargetAngle( obj_work, angle_target );

			//ƒ‚[ƒVƒ‡ƒ“”½“]
			if ( flag_motion_change ){
				gmGmkTarzanRopeChangeDirMotion( obj_work, angle_current );
			}
		}
	}
	else if (angle_target < 0){
		angle_target = gmGmkTarzanRopeAddUserWorkTargetAngle( obj_work, GM_GMK_TARZAN_ROPE_ANGLE_DEL );
		if ( angle_target < -GM_GMK_TARZAN_ROPE_ANGLE_MAX ){
			angle_target = -GM_GMK_TARZAN_ROPE_ANGLE_MAX;
		}
		else if ( angle_target > 0 ){
			angle_target = 0;
		}
		if ( angle_current <= angle_target ){
			//–Ú“IŠp“x‚ğ”½“]
			angle_target = -angle_target;
			gmGmkTarzanRopeSetUserWorkTargetAngle( obj_work, angle_target );

			//ƒ‚[ƒVƒ‡ƒ“”½“]
			if ( flag_motion_change ){
				gmGmkTarzanRopeChangeDirMotion( obj_work, angle_current );
			}
		}
	}
	return angle_target;
}


// ==========================================================================
// gmGmkTarzanRopeUpdatePlayerMotion
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@ƒ‚[ƒVƒ‡ƒ“‚ÌŒü‚«•ÏX
 *
 *	@param obj_work			[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkTarzanRopeChangeDirMotion( 
									OBS_OBJECT_WORK *obj_work, 
									Angle32 angle_current )
{
	UNREFERENCED_PARAMETER(obj_work);

	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	amAssert( player_work );
	OBS_OBJECT_WORK* player_obj_work = &player_work->obj_work;
	OBS_ACTION3D_NN_WORK *obj_3d = player_obj_work->obj_3d;
	amAssert( obj_3d );
	amAssert( obj_3d->motion );

	Float frame_start = amMotionGetStartFrame( obj_3d->motion, obj_3d->act_id[0] );
	//Float frame_end = amMotionGetEndFrame( obj_3d->motion, obj_3d->act_id[0] );
	Float frame_center = GM_GMK_TARZAN_ROPE_MOTION_FRAME_BACK;

	if ( player_obj_work->disp_flag & OBD_DISP_HFLIP ){
		//‰E‚ÉŒü‚©‚¤
		if ( angle_current < 0 ){
			obj_3d->frame[0] = frame_start;
		}
		//¶‚ÉŒü‚©‚¤
		else if ( angle_current > 0 ){
			obj_3d->frame[0] = frame_center;
		}
	}
	else{
		//‰E‚ÉŒü‚©‚¤
		if ( angle_current < 0 ){
			obj_3d->frame[0] = frame_center;
		}
		//¶‚ÉŒü‚©‚¤
		else if ( angle_current > 0 ){
			obj_3d->frame[0] = frame_start;
		}
	}
	player_obj_work->disp_flag &= ~OBD_DISP_STOP;
}

// ==========================================================================
// gmGmkTarzanRopeUpdatePlayerMotion
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@ƒvƒŒƒCƒ„ƒ‚[ƒVƒ‡ƒ“XV
 *
 *	@param obj_work			[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkTarzanRopeUpdatePlayerMotion(  
									   OBS_OBJECT_WORK *obj_work, 
									   Angle32 angle_target, 
									   Angle32 angle_current )
{
	UNREFERENCED_PARAMETER(obj_work);

	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	amAssert( player_work );
	OBS_OBJECT_WORK* player_obj_work = &player_work->obj_work;
	OBS_ACTION3D_NN_WORK *obj_3d = player_obj_work->obj_3d;
	amAssert( obj_3d );
	amAssert( obj_3d->motion );

	//Float frame_start = amMotionGetStartFrame( obj_3d->motion, obj_3d->act_id[0] );
	//Float frame_end = amMotionGetEndFrame( obj_3d->motion, obj_3d->act_id[0] );
	Float frame_center = GM_GMK_TARZAN_ROPE_MOTION_FRAME_BACK;
	
	Angle32 angle_target_value = MTM_MATH_ABS(angle_target);
	Angle32 angle_current_value = MTM_MATH_ABS(angle_current);

	if ( angle_target_value < GM_GMK_TARZAN_ROPE_ANGLE_LOW 
			&& angle_current_value < GM_GMK_TARZAN_ROPE_ANGLE_LOW 		
	){
		Angle32 key_rot_z = gmGmkTarzanRopeGetGimmickRotZ( player_work );
		if ( player_work->act_state != GME_PLY_ACT_STATE_GMK_ROPE_ST
			&& MTM_MATH_ABS(key_rot_z) < GM_GMK_TARZAN_ROPE_ANGLE_STOP
		){
			GmPlayerActionChange( player_work, GME_PLY_ACT_STATE_GMK_ROPE_ST );
			player_obj_work->disp_flag |= OBD_DISP_REPEAT;
		}
		return;
	}
	else{
		if ( player_work->act_state == GME_PLY_ACT_STATE_GMK_ROPE_ST ){
			GmPlayerActionChange( player_work, GME_PLY_ACT_STATE_GMK_ROPE );
			player_obj_work->disp_flag |= OBD_DISP_REPEAT;
			return;
		}
	}


	if ( player_obj_work->disp_flag & OBD_DISP_HFLIP ){
		//‰E‚ÉŒü‚©‚Á‚Ä‚¢‚é
		if ( angle_target > 0 && angle_current > 0 ){
			if ( obj_3d->frame[0] > frame_center ){
				player_obj_work->disp_flag |= OBD_DISP_STOP;
			}
		}
		//¶‚ÉŒü‚©‚Á‚Ä‚¢‚é
		else if ( angle_target < 0 && angle_current < 0){
			if ( obj_3d->frame[0] < frame_center ){
				player_obj_work->disp_flag |= OBD_DISP_STOP;
			}
		}
	}
	else{
		//‰E‚ÉŒü‚©‚Á‚Ä‚¢‚é
		if ( angle_target > 0 && angle_current > 0 ){
			if ( obj_3d->frame[0] < frame_center ){
				player_obj_work->disp_flag |= OBD_DISP_STOP;
			}
		}
		//¶‚ÉŒü‚©‚Á‚Ä‚¢‚é
		else if ( angle_target < 0 && angle_current < 0 ){
			if ( obj_3d->frame[0] > frame_center ){
				player_obj_work->disp_flag |= OBD_DISP_STOP;
			}
		}
	}
}

// ==========================================================================
// gmGmkTarzanRopeApplyKeyLeft
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@ƒL[“ü—Í”½‰fi¶•ûŒüj
 *
 *	@param obj_work			[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param angle_target		[in] –Ú“IŠp“x
 *	@param angle_current	[in] Œ»İŠp“x
 *
 *	@return XVŒã‚Ì–Ú“IŠp“x
 */
// ==========================================================================
Angle32 gmGmkTarzanRopeApplyKeyLeft( 
							  OBS_OBJECT_WORK *obj_work, 
							  Angle32 angle_target, 
							  Angle32 angle_current  )
{
	amAssert( obj_work );

	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* player_obj_work = gimmick_work->ene_com.target_obj;
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)player_obj_work;
	amAssert( player_work );

	Angle32 angle_target_value = MTM_MATH_ABS(angle_target);
	Angle32 angle_current_value = MTM_MATH_ABS(angle_current);

	Angle32 key_rot_z = gmGmkTarzanRopeGetGimmickRotZ( player_work );
	if ( angle_target_value < GM_GMK_TARZAN_ROPE_ANGLE_LOW && angle_current_value < GM_GMK_TARZAN_ROPE_ANGLE_LOW ){
		if ( key_rot_z < 0
				//&& !(player_obj_work->disp_flag & OBD_DISP_HFLIP)
		){
			angle_target = gmGmkTarzanRopeAddUserWorkTargetAngle( obj_work, -GM_GMK_TARZAN_ROPE_ANGLE_LOW );
		}
	}
	else{
		if (angle_target < 0){
			if ( key_rot_z < 0 ){
				angle_target = gmGmkTarzanRopeAddUserWorkTargetAngle( obj_work, -GM_GMK_TARZAN_ROPE_ANGLE_ADD );
			}
			else if ( key_rot_z > 0 ){
				angle_target = gmGmkTarzanRopeAddUserWorkTargetAngle( obj_work, GM_GMK_TARZAN_ROPE_ANGLE_ADD_N );
			}
		}
	}
	return angle_target;
}

// ==========================================================================
// gmGmkTarzanRopeApplyKeyRight
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@ƒL[“ü—Í”½‰fi‰E•ûŒüj
 *
 *	@param obj_work			[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param angle_target		[in] –Ú“IŠp“x
 *	@param angle_current	[in] Œ»İŠp“x
 *
 *	@return XVŒã‚Ì–Ú“IŠp“x
 */
// ==========================================================================
Angle32 gmGmkTarzanRopeApplyKeyRight( 
							  OBS_OBJECT_WORK *obj_work, 
							  Angle32 angle_target, 
							  Angle32 angle_current  )
{
	amAssert( obj_work );

	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* player_obj_work = gimmick_work->ene_com.target_obj;
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)player_obj_work;
	amAssert( player_work );

	Angle32 angle_target_value = MTM_MATH_ABS(angle_target);
	Angle32 angle_current_value = MTM_MATH_ABS(angle_current);

	Angle32 key_rot_z = gmGmkTarzanRopeGetGimmickRotZ( player_work );
	if ( angle_target_value < GM_GMK_TARZAN_ROPE_ANGLE_LOW && angle_current_value < GM_GMK_TARZAN_ROPE_ANGLE_LOW ){
		if ( key_rot_z > 0
				//&& player_obj_work->disp_flag & OBD_DISP_HFLIP
		){
			angle_target = gmGmkTarzanRopeAddUserWorkTargetAngle( obj_work, GM_GMK_TARZAN_ROPE_ANGLE_LOW );
		}
	}
	else{
		if ( angle_target > 0 ){
			if ( key_rot_z > 0 ){
				angle_target = gmGmkTarzanRopeAddUserWorkTargetAngle( obj_work, GM_GMK_TARZAN_ROPE_ANGLE_ADD );
			}
			else if ( key_rot_z < 0 ){
				angle_target = gmGmkTarzanRopeAddUserWorkTargetAngle( obj_work, -GM_GMK_TARZAN_ROPE_ANGLE_ADD_N );
			}
		}
	}
	return angle_target;
}

// ==========================================================================
// gmGmkTarzanRopeCheckStop
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@’â~ˆ—
 *
 *	@param obj_work			[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param angle_target		[in] –Ú“IŠp“x
 *	@param angle_current	[in] Œ»İŠp“x
 */
// ==========================================================================
void gmGmkTarzanRopeCheckStop( 
							  OBS_OBJECT_WORK *obj_work, 
							  Angle32 angle_target, 
							  Angle32 angle_current )
{
	//’â~”»’è
	Angle32 angle_target_value = MTM_MATH_ABS(angle_target);
	Angle32 angle_current_value = MTM_MATH_ABS(angle_current);
	if ( angle_target_value < GM_GMK_TARZAN_ROPE_ANGLE_STOP && angle_current_value < GM_GMK_TARZAN_ROPE_ANGLE_STOP ){
		gmGmkTarzanRopeSetUserTimerCurrentAngle( obj_work, 0 );	
		gmGmkTarzanRopeSetUserWorkTargetAngle( obj_work, 0 );
	}
}

// ==========================================================================
// gmGmkTarzanRopeCheckPlayerJump
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@ƒWƒƒƒ“ƒvˆ—
 *
 *	@param obj_work			[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param angle_target		[in] –Ú“IŠp“x
 *	@param angle_current	[in] Œ»İŠp“x
 *
 *	@return TRUEFƒWƒƒƒ“ƒv‚µ‚½@FALSEFƒWƒƒƒ“ƒv‚µ‚Ä‚¢‚È‚¢
 */
// ==========================================================================
BOOL gmGmkTarzanRopeCheckPlayerJump( 
							  OBS_OBJECT_WORK *obj_work, 
							  Angle32 angle_target, 
							  Angle32 angle_current )
{
	amAssert( obj_work );

	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* player_obj_work = gimmick_work->ene_com.target_obj;
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)player_obj_work;
	amAssert( player_work );

#if _IPHONE
	// ‘Ò‚¿ŠÔ‚È‚çI—¹
	if (gmGmkTarzanRopeGetCatchWait(obj_work) > 0) {
		return FALSE;
	}
#endif // _IPHONE

	if ( GmPlayerKeyCheckJumpKeyPush(player_work) ){
		//ƒ‚[ƒVƒ‡ƒ“Ä¶
		player_obj_work->disp_flag &= ~OBD_DISP_STOP;
#if 1	
		player_obj_work->spd_m = 0;
		player_obj_work->spd.x = 0;
		player_obj_work->spd.y = 0;
		player_obj_work->dir.z = 0;
		player_obj_work->spd_add.x = 0;
		player_obj_work->spd_add.y = 0;
		player_obj_work->spd_add.z = 0;

		//ƒMƒ~ƒbƒNƒWƒƒƒ“ƒv
		Float add = 1.0f+gimmick_work->ene_com.eve_rec->left /10000.0f;
		fx32 jump_x = (fx32)(angle_target * 0.8f * add );
		fx32 jump_y = (fx32)(MTM_MATH_ABS(angle_current) * 2.3f * add);
		//‚Æ‚Ü‚Á‚Ä‚¢‚é‚Æ‚«i^ã‚É¬‚³‚­ƒWƒƒƒ“ƒvj
		if ( angle_target == 0 && angle_current == 0 ){
			jump_x = 0;
			jump_y = 0x00004000L;
		}
		//–ß‚Á‚Ä‚¢‚é‚Æ‚«
		else if ( ( angle_target < 0 && 0 < angle_current )
			|| (0 < angle_target && angle_current < 0) 
		){
			//Ø‚è‘Ö‚í‚Á‚½’¼Œãi’Êí‚Æ“¯‚¶ƒWƒƒƒ“ƒv‚ğ‚·‚é‚ªAŒü‚«‚ª•Ï‚í‚Á‚Ä‚¢‚é‚Ì‚Å•â³j
			if ( (MTM_MATH_ABS(angle_target+angle_current) < GM_GMK_TARZAN_ROPE_ANGLE_JUMP) ){
				jump_x = -jump_x;
			}
			//Ø‚è‘Ö‚í‚Á‚Ä‚µ‚Î‚ç‚­Œo‰ßi^ã‚É¬‚³‚­ƒWƒƒƒ“ƒvj
			else{
				jump_x = 0;
				jump_y = 0x00004000L;
			}
		}

		GmPlySeqGmkInitGmkJump( player_work, jump_x, -jump_y);
		GmPlySeqChangeSequenceState( player_work, GME_PLY_SEQ_STATE_JUMP );
#else
		//ƒXƒsƒ“ƒWƒƒƒ“ƒv
		Float add = 1.0f+gimmick_work->ene_com.eve_rec->left /100.0f;
		target_obj_work->dir.z = angle_current* add;
		target_obj_work->spd_m = (MTM_MATH_ABS(angle_current) * add;
		GmPlySeqChangeSequence(player_work, GME_PLY_SEQ_STATE_JUMP);
#endif	//

		//ƒMƒ~ƒbƒNİ’è
		obj_work->ppFunc = gmGmkTarzanRopeMainEnd;
		gimmick_work->ene_com.target_obj = NULL;

		return TRUE;
	}
	return FALSE;
}

// ==========================================================================
// gmGmkTarzanRopeUpdatePlayerPos
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@ƒvƒŒƒCƒ„À•WXV
 *
 *	@param obj_work			[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkTarzanRopeUpdatePlayerPos( 
							  OBS_OBJECT_WORK *obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* target_obj_work = gimmick_work->ene_com.target_obj;
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)target_obj_work;
	amAssert( player_work );

	//ƒvƒŒƒCƒ„‚ÆƒMƒ~ƒbƒN‚Ì‰ñ“]²’²®iX²‰ñ“]‚Í–¢‘Î‰j
	NNS_MATRIX matrix_rot;
	nnMakeUnitMatrix( &matrix_rot );
	NNM_MTX(matrix_rot, 1, 1) = NNM_MTX(g_gm_gmk_tarzan_rope_active_matrix, 1, 1);
	NNM_MTX(matrix_rot, 2, 2) = NNM_MTX(g_gm_gmk_tarzan_rope_active_matrix, 0, 0);
	NNM_MTX(matrix_rot, 2, 1) = NNM_MTX(g_gm_gmk_tarzan_rope_active_matrix, 0, 1);
	NNM_MTX(matrix_rot, 1, 2) = NNM_MTX(g_gm_gmk_tarzan_rope_active_matrix, 1, 0);	
	NNM_MTX(matrix_rot, 0, 3) = -5.0f;	
	AkMathNormalizeMtx( &player_work->ex_obj_mtx_r, &matrix_rot );
	if ( target_obj_work->disp_flag & OBD_DISP_HFLIP ){
		NNM_MTX(player_work->ex_obj_mtx_r, 2, 1) = -NNM_MTX(player_work->ex_obj_mtx_r, 2, 1);
		NNM_MTX(player_work->ex_obj_mtx_r, 1, 2) = -NNM_MTX(player_work->ex_obj_mtx_r, 1, 2);
		NNM_MTX(matrix_rot, 0, 3) = -NNM_MTX(matrix_rot, 0, 3);	
	}
	
	//À•W
//	NNS_VECTOR offset = { 0.0f, GMD_GMK_TARZAN_ROPE_PLAYER_OFFSET_Y, 0.0f};
	Float	rate;
	fx32	ply_offset = (fx32)((gimmick_work->ene_com.enemy_flag & 0x0000ffff) << FX32_SHIFT);
	ply_offset += GMD_GMK_TARZAN_ROPE_FALL_SPD;
	if (ply_offset > GMD_GMK_TARZAN_ROPE_LENGTH) {
		ply_offset = GMD_GMK_TARZAN_ROPE_LENGTH;
	}
	gimmick_work->ene_com.enemy_flag &= 0xffff0000;
	gimmick_work->ene_com.enemy_flag |= (ply_offset >> FX32_SHIFT);
	rate = (Float)ply_offset / (Float)GMD_GMK_TARZAN_ROPE_LENGTH;
	rate = -rate * GMD_GMK_TARZAN_ROPE_LENGTH_F;
	rate += GMD_GMK_TARZAN_ROPE_LENGTH_F + GMD_GMK_TARZAN_ROPE_PLAYER_OFFSET_Y;
	NNS_VECTOR offset = { 0.0f, rate, 0.0f};

	nnTransformVector( &offset, &matrix_rot, &offset );

	target_obj_work->pos.x = FX_F32_TO_FX32( NNM_MTX(g_gm_gmk_tarzan_rope_active_matrix, 0, 3) + offset.z );
	target_obj_work->pos.y = - FX_F32_TO_FX32( NNM_MTX(g_gm_gmk_tarzan_rope_active_matrix, 1, 3) + offset.y );
	target_obj_work->pos.z = FX_F32_TO_FX32( NNM_MTX(g_gm_gmk_tarzan_rope_active_matrix, 2, 3) + offset.x );

	//Šg’£ƒ}ƒgƒŠƒNƒX
	player_work->gmk_flag |= GMD_PLGF_GMK_EXMTX_R;
}

// ==========================================================================
// gmGmkTarzanRopeMainWait
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@‘Ò‹@ƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkTarzanRopeMainWait( OBS_OBJECT_WORK *obj_work )
{
	amAssert( obj_work );

	//€–SAƒ_ƒ[ƒW
	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	if ( (player_work->player_flag & GMD_PLF_DIE)
			|| (player_work->seq_state == GME_PLY_SEQ_STATE_DAMAGE)
	){
		//ƒMƒ~ƒbƒNİ’è
		obj_work->ppFunc = gmGmkTarzanRopeMainEnd;
		gimmick_work->ene_com.target_obj = NULL;
		player_work->gmk_flag &= ~GMD_PLGF_GMK_EXMTX_R;
		return;
	}

	Angle32 angle_target = gmGmkTarzanRopeGetUserWorkTargetAngle( obj_work );
	Angle32 angle_current = gmGmkTarzanRopeGetUserTimerCurrentAngle( obj_work );

#if _IPHONE
	// ’Í‚İƒ^ƒCƒ~ƒ“ƒO‰Šú‰»
	gmGmkTarzanRopeInitCatchWait(obj_work);
#endif // _IPHONE

	//ƒvƒŒƒCƒ„ƒWƒƒƒ“ƒvˆ—
	if ( gmGmkTarzanRopeCheckPlayerJump(
			obj_work,
			angle_target,
			angle_current)
	){
		return;
	}

	//Œ»İŠp“xXV
	angle_current = gmGmkTarzanRopeUpdateAngleCurrent(
			obj_work,
			angle_target,
			angle_current);

	//–Ú“IŠp“xXV
	angle_target = gmGmkTarzanRopeUpdateAngleTarget( 
			obj_work,
			angle_target,
			angle_current,
			TRUE);

	//’â~”»’è
	gmGmkTarzanRopeCheckStop(
			obj_work,
			angle_target,
			angle_current );

	//ƒvƒŒƒCƒ„À•WXV
	gmGmkTarzanRopeUpdatePlayerPos( obj_work );
	
	obj_work->ppFunc = gmGmkTarzanRopeMainKey;
}

void gmGmkTarzanRopeMainKey( OBS_OBJECT_WORK *obj_work )
{	
	amAssert( obj_work );

	//€–SAƒ_ƒ[ƒW
	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	if ( (player_work->player_flag & GMD_PLF_DIE)
			|| (player_work->seq_state == GME_PLY_SEQ_STATE_DAMAGE)
	){
		//ƒMƒ~ƒbƒNİ’è
		obj_work->ppFunc = gmGmkTarzanRopeMainEnd;
		gimmick_work->ene_com.target_obj = NULL;
		player_work->gmk_flag &= ~GMD_PLGF_GMK_EXMTX_R;
		return;
	}

	Angle32 angle_target = gmGmkTarzanRopeGetUserWorkTargetAngle( obj_work );
	Angle32 angle_current = gmGmkTarzanRopeGetUserTimerCurrentAngle( obj_work );

#if _IPHONE
	// ’Í‚İƒ^ƒCƒ~ƒ“ƒOXV
	gmGmkTarzanRopeUpdateCatchWait(obj_work);
#endif // _IPHONE

	//ƒvƒŒƒCƒ„ƒWƒƒƒ“ƒvˆ—
	if ( gmGmkTarzanRopeCheckPlayerJump(
			obj_work,
			angle_target,
			angle_current)
	){
		return;
	}

	//ƒL[“ü—Í”½‰f
	if ( angle_target <= 0 ){
		angle_target = gmGmkTarzanRopeApplyKeyLeft(
				obj_work,
				angle_target,
				angle_current);
	}
	if ( angle_target >= 0 ){
		angle_target = gmGmkTarzanRopeApplyKeyRight(
				obj_work,
				angle_target,
				angle_current);
	}

	//Œ»İŠp“xXV
	angle_current = gmGmkTarzanRopeUpdateAngleCurrent(
			obj_work,
			angle_target,
			angle_current);

	//–Ú“IŠp“xXV
	angle_target = gmGmkTarzanRopeUpdateAngleTarget( 
			obj_work,
			angle_target,
			angle_current,
			TRUE );

	//ƒvƒŒƒCƒ„ƒ‚[ƒVƒ‡ƒ“ŠÇ—
	gmGmkTarzanRopeUpdatePlayerMotion( 
			obj_work,
			angle_target,
			angle_current );

	//’â~”»’è
	gmGmkTarzanRopeCheckStop(
			obj_work,
			angle_target,
			angle_current );

	//ƒvƒŒƒCƒ„À•WXV
	gmGmkTarzanRopeUpdatePlayerPos( obj_work );
}

#if 0
void gmGmkTarzanRopeMainKeyLeft( OBS_OBJECT_WORK *obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* target_obj_work = gimmick_work->ene_com.target_obj;
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)target_obj_work;
	amAssert( player_work );
	
	Angle32 angle_target = gmGmkTarzanRopeGetUserWorkTargetAngle( obj_work );
	Angle32 angle_current = gmGmkTarzanRopeGetUserTimerCurrentAngle( obj_work );

	//ƒvƒŒƒCƒ„ƒWƒƒƒ“ƒvˆ—
	if ( gmGmkTarzanRopeCheckPlayerJump(
			obj_work,
			angle_target,
			angle_current)
	){
		return;
	}

	//ƒL[“ü—Í”½‰f
	angle_target = gmGmkTarzanRopeApplyKeyLeft(
			obj_work,
			angle_target,
			angle_current);
	angle_target = gmGmkTarzanRopeApplyKeyRight(
			obj_work,
			angle_target,
			angle_current);

	//Œ»İŠp“xXV
	angle_current = gmGmkTarzanRopeUpdateAngleCurrent(
			obj_work,
			angle_target,
			angle_current);

	//–Ú“IŠp“xXV
	angle_target = gmGmkTarzanRopeUpdateAngleTarget( 
			obj_work,
			angle_target,
			angle_current,
			TRUE );

	//ƒvƒŒƒCƒ„ƒ‚[ƒVƒ‡ƒ“ŠÇ—
	gmGmkTarzanRopeUpdatePlayerMotion( 
			obj_work,
			angle_target,
			angle_current );

	//’â~”»’è
	gmGmkTarzanRopeCheckStop(
			obj_work,
			angle_target,
			angle_current );

	//ƒvƒŒƒCƒ„À•WXV
	gmGmkTarzanRopeUpdatePlayerPos( obj_work );
}

void gmGmkTarzanRopeMainKeyRight( OBS_OBJECT_WORK *obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	OBS_OBJECT_WORK* target_obj_work = gimmick_work->ene_com.target_obj;
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)target_obj_work;
	amAssert( player_work );
	
	Angle32 angle_target = gmGmkTarzanRopeGetUserWorkTargetAngle( obj_work );
	Angle32 angle_current = gmGmkTarzanRopeGetUserTimerCurrentAngle( obj_work );

	//ƒvƒŒƒCƒ„ƒWƒƒƒ“ƒvˆ—
	if ( gmGmkTarzanRopeCheckPlayerJump(
			obj_work,
			angle_target,
			angle_current)
	){
		return;
	}

	//ƒL[“ü—Í”½‰f
	angle_target = gmGmkTarzanRopeApplyKeyRight(
			obj_work,
			angle_target,
			angle_current);

	//Œ»İŠp“xXV
	angle_current = gmGmkTarzanRopeUpdateAngleCurrent(
			obj_work,
			angle_target,
			angle_current);

	//–Ú“IŠp“xXV
	angle_target = gmGmkTarzanRopeUpdateAngleTarget( 
			obj_work,
			angle_target,
			angle_current,
			TRUE );

	//ƒvƒŒƒCƒ„ƒ‚[ƒVƒ‡ƒ“ŠÇ—
	gmGmkTarzanRopeUpdatePlayerMotion( 
			obj_work,
			angle_target,
			angle_current );

	//’â~”»’è
	gmGmkTarzanRopeCheckStop(
			obj_work,
			angle_target,
			angle_current );

	//ƒvƒŒƒCƒ„À•WXV
	gmGmkTarzanRopeUpdatePlayerPos( obj_work );

}
#endif
// ==========================================================================
// gmGmkTarzanRopeMainEnd
/*!
 *	ƒMƒ~ƒbƒN@ƒ^[ƒUƒ“ƒ[ƒv@I—¹ƒƒCƒ“
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkTarzanRopeMainEnd( OBS_OBJECT_WORK *obj_work )
{
	Angle32 angle_target = gmGmkTarzanRopeGetUserWorkTargetAngle( obj_work );
	Angle32 angle_current = gmGmkTarzanRopeGetUserTimerCurrentAngle( obj_work );

	//Œ»İŠp“xXV
	gmGmkTarzanRopeUpdateAngleCurrent(
			obj_work,
			angle_target,
			angle_current);

	//–Ú“IŠp“xXV
	gmGmkTarzanRopeUpdateAngleTarget( 
			obj_work,
			angle_target,
			angle_current,
			FALSE );

	//’â~”»’è
	gmGmkTarzanRopeCheckStop(
			obj_work,
			angle_target,
			angle_current );
}


// ==========================================================================
// gmGmkTarzanRopeGetGimmickRotZ
/*!
 *	ŒXÎ‚ğæ“¾
 *
 *	@param ply_work	[in] ƒvƒŒƒCƒ„ƒ[ƒN
 *
 *	@return ƒRƒ“ƒgƒ[ƒ‰[‰ñ“]—Êæ“¾
 */
// ==========================================================================
Angle32 gmGmkTarzanRopeGetGimmickRotZ( GMS_PLAYER_WORK *ply_work)
{
	Angle32 rot_z = GmPlayerKeyGetGimmickRotZ( ply_work );

	if ( !(g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC) ){
		rot_z = rot_z;
	}
	return rot_z;
}






// ==========================================================================
// gmGmkTarzanRopeSetUserWorkTargetAngle
/*!
 *	ƒ†[ƒUƒ[ƒN‚ğ–Ú“IŠp“x‚Æ‚µ‚Äİ’è
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param angle	[in] –Ú“IŠp“x
 */
// ==========================================================================
void gmGmkTarzanRopeSetUserWorkTargetAngle( OBS_OBJECT_WORK* obj_work, Angle32 angle )
{
	obj_work->user_work = (u32)angle;
}

// ==========================================================================
// gmGmkTarzanRopeGetUserWorkTargetAngle
/*!
 *	ƒ†[ƒUƒ[ƒN‚ğ–Ú“IŠp“x‚Æ‚µ‚Äæ“¾
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return –Ú“IŠp“x
 */
// ==========================================================================
Angle32 gmGmkTarzanRopeGetUserWorkTargetAngle( OBS_OBJECT_WORK* obj_work )
{
	return (Angle32)obj_work->user_work;
}

// ==========================================================================
// gmGmkTarzanRopeSetUserWorkTargetAngle
/*!
 *	ƒ†[ƒUƒ[ƒN‚ğ–Ú“IŠp“x‚Æ‚µ‚Ä’Ç‰Á
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param angle	[in] ‰ÁZŠp“x
 *
 *	@return ‰ÁZŒã‚Ì–Ú“IŠp“x
 */
// ==========================================================================
Angle32 gmGmkTarzanRopeAddUserWorkTargetAngle( OBS_OBJECT_WORK* obj_work, Angle32 angle )
{
	obj_work->user_work += (u32)angle;
	return (Angle32)obj_work->user_work;
}


// ==========================================================================
// gmGmkTarzanRopeSetUserTimerCurrentAngle
/*!
 *	ƒ†[ƒUƒ^ƒCƒ}‚ğŒ»İŠp“x‚Æ‚µ‚Äİ’è
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param angle	[in] –ÚŒ»İŠp“x
 */
// ==========================================================================
void gmGmkTarzanRopeSetUserTimerCurrentAngle( OBS_OBJECT_WORK* obj_work, Angle32 angle )
{
	obj_work->user_timer = (s32)angle;
}

// ==========================================================================
// gmGmkTarzanRopeGetUserTimerCurrentAngle
/*!
 *	ƒ†[ƒUƒ^ƒCƒ}‚ğŒ»İŠp“x‚Æ‚µ‚Äæ“¾
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return Œ»İŠp“x
 */
// ==========================================================================
Angle32 gmGmkTarzanRopeGetUserTimerCurrentAngle( OBS_OBJECT_WORK* obj_work )
{
	return (Angle32)obj_work->user_timer;
}

// ==========================================================================
// gmGmkTarzanRopeAddUserTimerCurrentAngle
/*!
 *	ƒ†[ƒUƒ^ƒCƒ}‚ğŒ»İŠp“x‚Æ‚µ‚Ä’Ç‰Á
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param angle	[in] Œ»İŠp“x
 *	@param angle	[in] ‰ÁZŠp“x
 *
 *	@return ‰ÁZŒã‚ÌŒ»İŠp“x
 */
// ==========================================================================
Angle32 gmGmkTarzanRopeAddUserTimerCurrentAngle( OBS_OBJECT_WORK* obj_work, Angle32 angle )
{
	obj_work->user_timer += (s32)angle;
	return (Angle32)obj_work->user_timer;
}

// ==========================================================================
// gmGmkTarzanRopeeSetUserFlagType
/*!
 *	ƒ†[ƒUƒtƒ‰ƒO‚ğƒ[ƒvƒ^ƒCƒv‚Æ‚µ‚Äİ’è
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param length	[in] ƒ[ƒvƒ^ƒCƒv
 */
// ==========================================================================
void gmGmkTarzanRopeeSetUserFlagType( OBS_OBJECT_WORK* obj_work, GME_GMK_TARZAN_ROPE_TYPE type )
{
	obj_work->user_flag |= (u16)type << 16;
}

// ==========================================================================
// gmGmkTarzanRopeGetUserFlagType
/*!
 *	ƒ†[ƒUƒtƒ‰ƒO‚ğƒ[ƒvƒ^ƒCƒv‚Æ‚µ‚Äæ“¾
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return ƒ[ƒvƒ^ƒCƒv
 */
// ==========================================================================
GME_GMK_TARZAN_ROPE_TYPE gmGmkTarzanRopeGetUserFlagType( const OBS_OBJECT_WORK* obj_work )
{
	return (GME_GMK_TARZAN_ROPE_TYPE)(obj_work->user_flag >> 16);
}

#if _IPHONE
// ==========================================================================
// gmGmkTarzanRopeInitCatchWait
/*!
 *	ƒ†[ƒU[ƒtƒ‰ƒO‚Ì‹ó‚«‚É’Í‚İ‘Ò‹@ŠÔ‚ğİ’è
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkTarzanRopeInitCatchWait(OBS_OBJECT_WORK* obj_work)
{
	obj_work->user_flag |= GMD_GMK_TARZAN_ROPE_CATCH_WAIT;
}

// ==========================================================================
// gmGmkTarzanRopeUpdateCatchWait
/*!
 *	ƒ†[ƒU[ƒtƒ‰ƒO‚Ì‹ó‚«‚Éİ’è‚³‚ê‚Ä‚¢‚é’Í‚İ‘Ò‹@ŠÔ‚ğXV
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkTarzanRopeUpdateCatchWait(OBS_OBJECT_WORK* obj_work)
{
	u32 wait = (obj_work->user_flag & GMD_GMK_TARZAN_ROPE_CATCH_WAIT_BIT);
	if (wait > 0) {
		wait--;
	}
	obj_work->user_flag = (obj_work->user_flag & (~GMD_GMK_TARZAN_ROPE_CATCH_WAIT_BIT)) | wait;
}

// ==========================================================================
// gmGmkTarzanRopeGetCatchWait
/*!
 *	ƒ†[ƒU[ƒtƒ‰ƒO‚Ì‹ó‚«‚Éİ’è‚³‚ê‚Ä‚¢‚é’Í‚İ‘Ò‹@ŠÔ‚ğ“üè
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return c‚è‘Ò‚¿ŠÔ(0`7)
 */
// ==========================================================================
u32 gmGmkTarzanRopeGetCatchWait(OBS_OBJECT_WORK* obj_work)
{
	u32 wait = (obj_work->user_flag & GMD_GMK_TARZAN_ROPE_CATCH_WAIT_BIT);
	return wait;
}
#endif // _IPHONE

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
