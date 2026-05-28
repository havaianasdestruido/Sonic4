// ==========================================================================
/*!
  @file gmGmkBoss3Pillar.cpp
  @brief ƒMƒ~ƒbƒN ƒ{ƒX3’Œ

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkBoss3Pillar.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"
#include "gmCamera.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectZone.h"
#include "gmPlySeq.h"
#include "gmPlySeqGmk.h"
#include "gmSound.h"
#include "gmPadVib.h"

#if _IPHONE
#include "gmTvx.h"
#endif // _IPHONE

#include "gmGmkBoss3Pillar.h"
// ƒf[ƒ^ƒwƒbƒ_
#include "common/model/gmk_b3_pillar_mdl.hmb"
#include "common/model/GMK_SW_WALL3_MDL.HMB"
#include "common/model/gmk_b3_pillar_f_mdl.hmb"
#include "common/model/gmk_b3_pillar_f_mat.hmb"

#if _IPHONE
// common‚Æ\¬‚ª•Ï‚í‚Á‚Ä‚¢‚éê‡‚Ég—p‚·‚é’è‹`
#include "iPhone/model/GMK_B3_PILLAR_TVX.HMB"
#include "iPhone/model/GMK_B3_PILLAR_F_TVX.HMB"
#include "iPhone/model/GMK_SW_WALL3_TVX.HMB"
#endif // _IPHONE


//----- Definitions ---------------------------------------------------------

#define GMD_GMK_BOSS3_PILLAR_TEST_NO_WALL	( 0 & _WII )

#define GMD_GMK_BOSS3_PILLAR_TEST_TVX       (1 & _IPHONE)

//------------------------------------------------------------------
//ŠÇ—
//------------------------------------------------------------------

#define GMD_GMK_BOSS_PILLAR_NUM				(26)	//’Œ‚Ì”
#define GMD_GMK_BOSS3_PILLAR_SCALE			(1.25f)	//’Œ‚ÌƒXƒP[ƒ‹
#define GMD_GMK_BOSS3_PILLAR_SIZE			((s32)(32*GMD_GMK_BOSS3_PILLAR_SCALE))	//’Œ‚ÌƒTƒCƒY
#define GMD_GMK_BOSS3_PILLAR_OFFSET_Y		((s32)(-4))	//’Œ‚Ì”z’uƒIƒtƒZƒbƒg
#define GMD_GMK_BOSS3_PILLAR_DEFAULT_OFFSET	((s32)(16))	//’Œ‚ÌƒfƒtƒHƒ‹ƒg”z’uƒIƒtƒZƒbƒg

#define GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT	((s32)(8*GMD_GMK_BOSS3_PILLAR_SCALE))	//’Œ‘O•û‚Ì—]”’•”•ª

#define GMD_GMK_BOSS3_PILLAR_SPEED_NORMAL	(0.5f)	//’ÊíˆÚ“®—Ê
#define GMD_GMK_BOSS3_PILLAR_SPEED_ADJUST	(1.5f)	//ˆÚ“®—Ê•â³’l(ƒtƒ@ƒCƒiƒ‹ƒ][ƒ“)

#define GMD_GMK_BOSS3_PILLAR_FRAME_HURRY	(50)	//‹}‚®ƒ‚[ƒh‚ÌƒtƒŒ[ƒ€”
#define GMD_GMK_BOSS3_PILLAR_FRAME_RETURN	(30)	//–ß‚éƒ‚[ƒh‚ÌƒtƒŒ[ƒ€”

#define	GMD_GMK_BOSS3_PILLAR_QUAKE_FALL						(3 << FX32_SHIFT)	//U“®—Ê

//byte_param—pƒtƒ‰ƒO
#define GMD_GMK_BOSS3_PILLAR_BYTE_PARAM_FLAG_FINISH		(1<<0)	//I—¹

//ƒIƒuƒWƒFƒNƒgƒtƒ‰ƒO
#define GMD_GMK_BOSS3_PILLAR_FLAG_HIT_EFFECT			(1<<0)	//HITƒGƒtƒFƒNƒg”­¶ƒtƒ‰ƒO
#define GMD_GMK_BOSS3_PILLAR_FLAG_CAMERA_VIBRATION		(1<<1)	//U“®ƒtƒ‰ƒO
#define GMD_GMK_BOSS3_PILLAR_FLAG_SE_1					(1<<2)	//SEƒtƒ‰ƒOiUŒ‚‚ä‚Á‚­‚èSEj
#define GMD_GMK_BOSS3_PILLAR_FLAG_SE_2					(1<<3)	//SEƒtƒ‰ƒOiUŒ‚ƒXƒs[ƒhƒAƒbƒvSEj
#define GMD_GMK_BOSS3_PILLAR_FLAG_DRAW_WALL_BACK		(1<<4)	//•Ç‚Ì— ‚É”Â‚ğ•`‰æ‚·‚éƒtƒ‰ƒO

#if _IPHONE
#define GMD_GMK_BOSS3_PILLAR_TIME_EFFECT_APPEAR			(45)	//ƒGƒtƒFƒNƒg•\¦ŠÔ
#endif // _IPHONE

//------------------------------------------------------------------
//ƒp[ƒc
//------------------------------------------------------------------
//ƒ^ƒCƒv
enum GME_GMK_BOSS3_PILLAR_TYPE{
	GMD_GMK_BOSS3_PILLAR_TYPE_LEFT = 0,	//¶
	GMD_GMK_BOSS3_PILLAR_TYPE_RIGHT,	//‰E
	GMD_GMK_BOSS3_PILLAR_TYPE_TOP,		//ã
	GMD_GMK_BOSS3_PILLAR_TYPE_BOTTOM,	//‰º

	GMD_GMK_BOSS3_PILLAR_TYPE_NUM
};

#define GMD_GMK_BOSS3_PILLAR_HORIZONTAL_TYPE_NUM	(3)		//‰¡Œ^‚Ì”
#define GMD_GMK_BOSS3_PILLAR_HORIZONTAL_PARTS_NUM	(6)		//‰¡Œ^‚Ì”
#define GMD_GMK_BOSS3_PILLAR_F_HORIZONTAL_PARTS_NUM	(9)		//‰¡Œ^‚Ì”
#define GMD_GMK_BOSS3_PILLAR_HORIZONTAL_WIDTH		((s32)(GMD_GMK_BOSS3_PILLAR_SIZE*8-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT))	//’nŒ`•Ó‚è•
#define GMD_GMK_BOSS3_PILLAR_HORIZONTAL_HEIGHT		((s32)GMD_GMK_BOSS3_PILLAR_SIZE*1)	//’nŒ`•Ó‚è‚‚³

#define GMD_GMK_BOSS3_PILLAR_VERTICAL_TYPE_NUM		(5)		//cŒ^‚Ì”
#define GMD_GMK_BOSS3_PILLAR_VERTICAL_PARTS_NUM		(4)		//cŒ^‚Ì”
#define GMD_GMK_BOSS3_PILLAR_F_VERTICAL_PARTS_NUM	(5)		//cŒ^‚Ì”
#define GMD_GMK_BOSS3_PILLAR_VERTICAL_WIDTH			((s32)GMD_GMK_BOSS3_PILLAR_SIZE*1)	//’nŒ`•Ó‚è•
#define GMD_GMK_BOSS3_PILLAR_VERTICAL_HEIGHT		((s32)(GMD_GMK_BOSS3_PILLAR_SIZE*5-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT))	//’nŒ`•Ó‚è‚‚³

#define GMD_GMK_BOSS3_PILLAR_PARTS_NUM_MAX			(9)		//ƒp[ƒcÅ‘å”

//------------------------------------------------------------------
//•Ç
//------------------------------------------------------------------
#define GMD_GMK_BOSS3_PILLAR_WALL_SPEED			((fx32)3*FX32_ONE)	//•Çƒ‚ƒfƒ‹‚ÌˆÚ“®—Ê

#define GMD_GMK_BOSS3_PILLAR_WALL_SIZE			(32)				//•Çƒ‚ƒfƒ‹‚ÌƒTƒCƒY
#define GMD_GMK_BOSS3_PILLAR_WALL_HEIGHT		(6)					//•Çƒ‚ƒfƒ‹‚Ì”
#define GMD_GMK_BOSS3_PILLAR_WALL_OFFSET_Y		((fx32)(GMD_GMK_BOSS3_PILLAR_WALL_HEIGHT+1)*GMD_GMK_BOSS3_PILLAR_WALL_SIZE)		//•Çƒ‚ƒfƒ‹‚Ì”z’uƒIƒtƒZƒbƒg

#define GMD_GMK_BOSS3_PILLAR_WALL_HEIGHT_F		(3)					//•Çƒ‚ƒfƒ‹‚Ì”iƒtƒ@ƒCƒiƒ‹ƒ][ƒ“j
#define GMD_GMK_BOSS3_PILLAR_WALL_SIZE_F		(64)				//•Çƒ‚ƒfƒ‹‚ÌƒTƒCƒY
#define GMD_GMK_BOSS3_PILLAR_WALL_OFFSET_Y_F	((fx32)(GMD_GMK_BOSS3_PILLAR_WALL_HEIGHT_F)*GMD_GMK_BOSS3_PILLAR_WALL_SIZE_F)		//•Çƒ‚ƒfƒ‹‚Ì”z’uƒIƒtƒZƒbƒgiƒtƒ@ƒCƒiƒ‹ƒ][ƒ“j


#define GMD_GMK_BOSS3_PILLAR_WALL_NO_PRESS_DIE_FLOW_DISTANCE	(64)	//ˆ³€‚³‚¹‚È‚¢‚æ‚¤‚ÉˆÚ“®‚³‚¹‚é“Vˆä‚Æ‚Ì‹——£
#define GMD_GMK_BOSS3_PILLAR_WALL_NO_PRESS_DIE_MOVE_LAND		(4)		//ˆ³€‚³‚¹‚È‚¢‚æ‚¤‚ÉˆÚ“®‚³‚¹‚é—Êi’nãj
#define GMD_GMK_BOSS3_PILLAR_WALL_NO_PRESS_DIE_MOVE_JUMP		(1)		//ˆ³€‚³‚¹‚È‚¢‚æ‚¤‚ÉˆÚ“®‚³‚¹‚é—Êi‹ó’†j

enum GME_GMK_BOSS3_PILLAR_WALL_TYPE{
	GMD_GMK_BOSS3_PILLAR_WALL_TYPE_LEFT = 0,	//¶‘¤
	GMD_GMK_BOSS3_PILLAR_WALL_TYPE_RIGHT,		//‰E‘¤

	GMD_GMK_BOSS3_PILLAR_WALL_TYPE_NUM
};

//------------------------------------------------------------------
//ƒGƒtƒFƒNƒg
//------------------------------------------------------------------
#define GMD_GMK_BOSS3_PILLAR_PILLAR_EFFECT_OFFSET_Y			((fx32)(GMD_GMK_BOSS3_PILLAR_SIZE-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT+GMD_GMK_BOSS3_PILLAR_DEFAULT_OFFSET)*FX32_ONE)	//’ŒoŒ»ƒGƒtƒFƒNƒgƒIƒtƒZƒbƒg
#define GMD_GMK_BOSS3_PILLAR_WALL_EFFECT_OFFSET_Y			((fx32)(48*FX32_ONE))	//•ÇoŒ»ƒGƒtƒFƒNƒgƒIƒtƒZƒbƒg

#define GMD_GMK_BOSS3_PILLAR_PILLAR_EFFECT_OFFSET_Y_FINAL	((fx32)(GMD_GMK_BOSS3_PILLAR_SIZE+GMD_GMK_BOSS3_PILLAR_DEFAULT_OFFSET)*FX32_ONE)	//’ŒoŒ»ƒGƒtƒFƒNƒgƒIƒtƒZƒbƒg
#define GMD_GMK_BOSS3_PILLAR_WALL_EFFECT_OFFSET_Y_FINAL		((fx32)(32*FX32_ONE))	//•ÇoŒ»ƒGƒtƒFƒNƒgƒIƒtƒZƒbƒg

//ƒp[ƒc
enum GME_GMK_BOSS3_PILLAR_WALL_PARTS{
	GMD_GMK_BOSS3_PILLAR_WALL_PARTS_B = 0,	//Bƒp[ƒc
	GMD_GMK_BOSS3_PILLAR_WALL_PARTS_GLARE,	//ƒOƒŒƒA

	GMD_GMK_BOSS3_PILLAR_WALL_PARTS_NUM,
};

//------------------------------------------------------------------
//ƒ[ƒN
//------------------------------------------------------------------

//’ŒƒƒCƒ“ƒ[ƒN
typedef struct tag_GMS_GMK_BOSS3_PILLAR_MAIN_WORK{
	GMS_ENEMY_3D_WORK gimmick_work;	//ƒMƒ~ƒbƒNƒ[ƒN

	OBS_ACTION3D_NN_WORK obj_3d_parts[GMD_GMK_BOSS3_PILLAR_PARTS_NUM_MAX-1];	//ƒp[ƒc
	GMS_EFFECT_3DES_WORK* effect_work;	//ƒGƒtƒFƒNƒgƒ[ƒN

	VecFx32 target_pos;				//–Ú“I’nÀ•W
	VecFx32 default_pos;			//Šî€À•W

	GSS_SND_SE_HANDLE* se_handle;		//SEƒnƒ“ƒhƒ‹
}GMS_GMK_BOSS3_PILLAR_MAIN_WORK;

//•Çƒ[ƒN
typedef struct tag_GMS_GMK_BOSS3_PILLAR_WALL_WORK{
	GMS_ENEMY_3D_WORK gimmick_work;	//ƒMƒ~ƒbƒNƒ[ƒN
	OBS_ACTION3D_NN_WORK obj_3d_parts[GMD_GMK_BOSS3_PILLAR_WALL_PARTS_NUM];	//ƒp[ƒc
	GMS_EFFECT_3DES_WORK* effect_work;	//ƒGƒtƒFƒNƒgƒ[ƒN

	VecFx32 target_pos;				//–Ú“I’nÀ•W
	VecFx32 default_pos;			//Šî€À•W

	GSS_SND_SE_HANDLE* se_handle;		//SEƒnƒ“ƒhƒ‹
}GMS_GMK_BOSS3_PILLAR_WALL_WORK;

//’ŒŠÇ—ƒ[ƒN
typedef struct tag_GMS_GMK_BOSS3_PILLAR_MANAGER_WORK{
	GMS_ENEMY_3D_WORK gimmick_work;	//ƒMƒ~ƒbƒNƒ[ƒN

	OBS_OBJECT_WORK* obj_work_pillar[GMD_GMK_BOSS_PILLAR_NUM];	//’Œƒ[ƒN
	OBS_OBJECT_WORK* obj_work_wall[GMD_GMK_BOSS3_PILLAR_WALL_TYPE_NUM];	//¶‰E‚Ì•Ç
	s32 pattern_no;	//ƒpƒ^[ƒ“”Ô†
}GMS_GMK_BOSS3_PILLAR_MANAGER_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static GMS_ENEMY_3D_WORK* gmGmkBoss3PillarLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y,
									u32 work_size);
static GMS_ENEMY_3D_WORK* gmGmkBoss3PillarLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									OBS_ACTION3D_NN_WORK* data_work_list,
									u32 model_id,
									u32 work_size);

//------------------------------------------------------
//ŠÇ—
//------------------------------------------------------
static void gmGmkBoss3PillarManagerInit( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarManagerMainFuncWaitHurry( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarManagerMainFuncWaitHurryEnd( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarManagerMainFuncWaitReturn( OBS_OBJECT_WORK* obj_work );
#if _IPHONE
static void gmGmkBoss3PillarManagerMainFuncFw( OBS_OBJECT_WORK* obj_work );
#endif // _IPHONE


//------------------------------------------------------
//ƒp[ƒc
//------------------------------------------------------
static GME_GMK_BOSS3_PILLAR_TYPE gmGmkBoss3PillarCalcPillarType( s32 event_id );
static void gmGmkBoss3PillarSetFieldRect( 
								  OBS_COLLISION_OBJ* field_obj, 
								  GME_GMK_BOSS3_PILLAR_TYPE pillar_type );
static void gmGmkBoss3PillarSetMoveSpeed( 
							 OBS_OBJECT_WORK* obj_work, 
							 GME_GMK_BOSS3_PILLAR_TYPE pillar_type,
							 fx32 speed);
static void gmGmkBoss3PillarSetMoveTarget( 
							 OBS_OBJECT_WORK* obj_work, 
							 GME_GMK_BOSS3_PILLAR_TYPE pillar_type,
							 fx32 distance);
static void gmGmkBoss3PillarSetMoveHurry( 
							 OBS_OBJECT_WORK* obj_work, 
							 s32 frame);
static BOOL gmGmkBoss3PillarCheckMoveEnd( 
								  OBS_OBJECT_WORK* obj_work,
								  GME_GMK_BOSS3_PILLAR_TYPE pillar_type);
static void gmBoss3PillarDestFunc( MTS_TASK_TCB *tcb );
static void gmBoss3PillarOutFunc( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarPartsInitMain( OBS_OBJECT_WORK* obj_work, GME_GMK_BOSS3_PILLAR_TYPE pillar_type );

static void gmGmkBoss3PillarPartsChangeModeWait( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarPartsChangeModeNormal( OBS_OBJECT_WORK* obj_work, fx32 distance, s32 wait_time );
static void gmGmkBoss3PillarPartsChangeModeHurry( OBS_OBJECT_WORK* obj_work, s32 move_frame );
static void gmGmkBoss3PillarPartsChangeModeReturn( OBS_OBJECT_WORK* obj_work, s32 wait_time, s32 move_frame );
static void gmGmkBoss3PillarPartsMainWait( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarPartsMainReady( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarPartsMainActive( OBS_OBJECT_WORK* obj_work );

#if _IPHONE
static GSS_SND_SE_HANDLE* gmGmkBoss3PillarGetSeHandle(void);
static void gmGmkBoss3PillarFreeHandle(GSS_SND_SE_HANDLE* se_handle);
#endif // _IPHONE

//----------------------------------------------------------------
//•Ç
//----------------------------------------------------------------
static void gmGmkBoss3PillarWallInit( OBS_OBJECT_WORK* obj_work );
static void gmBoss3PillarWallDestFunc( MTS_TASK_TCB *tcb );
static void gmBoss3PillarWallOutFunc( OBS_OBJECT_WORK* obj_work );
static void gmBoss3PillarWallOutFuncForFinalZone( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarWallChangeModeWait( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarWallChangeModeActive( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarWallChangeModeReturn( OBS_OBJECT_WORK* obj_work );
static fx32 gmGmkBoss3PillarWallCheckMoveEnd( GMS_GMK_BOSS3_PILLAR_WALL_WORK* wall_work );
static void gmGmkBoss3PillarWallMainWait( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarWallMainActive( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarWallDrawBack(
									Float left,
									Float top,
									Float right,
									Float bottom,
									Float z);
static void gmGmkBoss3PillarWallMatrixPush( u32 command_state );
static void gmGmkBoss3PillarWallMatrixPop( u32 command_state );
static void gmGmkBoss3PillarWallUserFuncMatrixPush( void* param );
static void gmGmkBoss3PillarWallUserFuncPop( void* param );
static void gmGmkBoss3PillarEffectCreatePillarAppear( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarEffectCreatePillarHit( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarEffectCreateWallHit( OBS_OBJECT_WORK* obj_work );
static void gmGmkBoss3PillarEffectCreateWallAppear( OBS_OBJECT_WORK* obj_work );

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
#if _IPHONE
static int gm_gmk_boss3_pillar_se_use_count = 0; //!< SEg—pŒÂ”

static int gm_gmk_boss3_pillar_global_flag = 0; //!< ‘S‘ÌŠÇ—ƒtƒ‰ƒO
#endif // _IPHONE

//------------------------------------------------------------------
//ŠÇ—
//------------------------------------------------------------------

//’Œƒ^ƒCƒv
static const s32 g_gm_gmk_boss3_pillar_event_id[GMD_GMK_BOSS_PILLAR_NUM] = {
	//’ŒA`H
	GMD_EVENT_ID_BOSS3_PILLAR_TOP,	
	GMD_EVENT_ID_BOSS3_PILLAR_TOP,	
	GMD_EVENT_ID_BOSS3_PILLAR_TOP,	
	GMD_EVENT_ID_BOSS3_PILLAR_TOP,	
	GMD_EVENT_ID_BOSS3_PILLAR_TOP,	
	GMD_EVENT_ID_BOSS3_PILLAR_TOP,	
	GMD_EVENT_ID_BOSS3_PILLAR_TOP,	
	GMD_EVENT_ID_BOSS3_PILLAR_TOP,
	//’ŒI`P
	GMD_EVENT_ID_BOSS3_PILLAR_BOTTOM,	
	GMD_EVENT_ID_BOSS3_PILLAR_BOTTOM,	
	GMD_EVENT_ID_BOSS3_PILLAR_BOTTOM,	
	GMD_EVENT_ID_BOSS3_PILLAR_BOTTOM,	
	GMD_EVENT_ID_BOSS3_PILLAR_BOTTOM,	
	GMD_EVENT_ID_BOSS3_PILLAR_BOTTOM,	
	GMD_EVENT_ID_BOSS3_PILLAR_BOTTOM,	
	GMD_EVENT_ID_BOSS3_PILLAR_BOTTOM,
	//’ŒQ`U
	GMD_EVENT_ID_BOSS3_PILLAR_LEFT,	
	GMD_EVENT_ID_BOSS3_PILLAR_LEFT,
	GMD_EVENT_ID_BOSS3_PILLAR_LEFT,	
	GMD_EVENT_ID_BOSS3_PILLAR_LEFT,	
	GMD_EVENT_ID_BOSS3_PILLAR_LEFT,
	//’ŒV`Z
	GMD_EVENT_ID_BOSS3_PILLAR_RIGHT,	
	GMD_EVENT_ID_BOSS3_PILLAR_RIGHT,
	GMD_EVENT_ID_BOSS3_PILLAR_RIGHT,	
	GMD_EVENT_ID_BOSS3_PILLAR_RIGHT,	
	GMD_EVENT_ID_BOSS3_PILLAR_RIGHT,
};

//‰Šú”z’u
static const s32 g_gm_gmk_boss3_pillar_default_pos[GMD_GMK_BOSS_PILLAR_NUM][MTD_XY] = {
	{ 0, 0}, { 1, 0}, { 2, 0}, { 3, 0}, { 4, 0}, { 5, 0}, { 6, 0}, { 7, 0},	//’ŒA`H
	{ 0, 5}, { 1, 5}, { 2, 5}, { 3, 5}, { 4, 5}, { 5, 5}, { 6, 5}, { 7, 5},	//’ŒI`P
	{ 0, 0}, { 0, 1}, { 0, 2}, { 0, 3}, { 0, 4},							//’ŒQ`U
	{ 8, 0}, { 8, 1}, { 8, 2}, { 8, 3}, { 8, 4},							//’ŒV`Z
};

//ˆÚ“®—Ê
static const s32 g_gm_gmk_boss3_pillar_move_distance[GMD_GMK_BOSS3_PILLAR_PATTERN_NUM][GMD_GMK_BOSS_PILLAR_NUM] = {
	{
		 0, 5, 0, 0, 0, 0, 5, 0,	//’ŒA`H
		 5, 0, 5, 0, 0, 5, 0, 5,	//’ŒI`P
		 0, 0, 0, 0, 0,				//’ŒQ`U
		 0, 0, 0, 0, 0,				//’ŒV`Z
	},
	{
		 0, 0, 0, 0, 0, 0, 0, 0,	//’ŒA`H
		 0, 0, 0, 0, 0, 0, 0, 0,	//’ŒI`P
		 2, 2, 4, 4, 4,				//’ŒQ`U
		 2, 2, 4, 4, 4,				//’ŒV`Z
	},
	{
		 2, 2, 2, 2, 0, 0, 0, 0,	//’ŒA`H
		 0, 0, 0, 0, 0, 0, 0, 0,	//’ŒI`P
		 0, 0, 4, 8, 0,				//’ŒQ`U
		 0, 0, 4, 0, 8,				//’ŒV`Z
	},
	{
		 0, 0, 5, 4, 0, 0, 0, 0,	//’ŒA`H
		 0, 0, 0, 0, 4, 4, 4, 4,	//’ŒI`P
		 0, 0, 0, 2, 2,				//’ŒQ`U
		 0, 0, 0, 0, 0,				//’ŒV`Z
	},
	{
		 0, 0, 0, 0, 0, 2, 2, 2,	//’ŒA`H
		 2, 2, 2, 0, 0, 0, 0, 0,	//’ŒI`P
		 0, 0, 5, 0, 0,				//’ŒQ`U
		 0, 0, 0, 5, 5,				//’ŒV`Z
	},
	{
		 0, 0, 0, 2, 0, 0, 0, 0,	//’ŒA`H
		 0, 0, 0, 3, 0, 0, 0, 0,	//’ŒI`P
		 0, 0, 3, 0, 0,				//’ŒQ`U
		 0, 0, 4, 0, 0,				//’ŒV`Z
	},
	{
		 0, 0, 0, 0, 2, 0, 0, 0,	//’ŒA`H
		 0, 0, 0, 0, 3, 0, 0, 0,	//’ŒI`P
		 0, 0, 4, 0, 0,				//’ŒQ`U
		 0, 0, 3, 0, 0,				//’ŒV`Z
	},
};

//‘Ò‚¿ŠÔ
static const s32 g_gm_gmk_boss3_pillar_wait_frame[GMD_GMK_BOSS3_PILLAR_PATTERN_NUM][GMD_GMK_BOSS_PILLAR_NUM] = {
	{
		0,	2,	0,	0,	0,	0,	2,	0,	//’ŒA`H
		1,	0,	1,	0,	0,	1,	0,	1,	//’ŒI`P
		0,	0,	0,	0,	0,				//’ŒQ`U
		0,	0,	0,	0,	0,				//’ŒV`Z
	},
	{
		0,	0,	0,	0,	0,	0,	0,	0,	//’ŒA`H
		0,	0,	0,	0,	0,	0,	0,	0,	//’ŒI`P
		3,	3,	3,	2,	1,				//’ŒQ`U
		3,	3,	3,	2,	1,				//’ŒV`Z
	},
	{
		4,	4,	4,	4,	0,	0,	0,	0,	//’ŒA`H
		0,	0,	0,	0,	0,	0,	0,	0,	//’ŒI`P
		0,	0,	3,	1,	0,				//’ŒQ`U
		0,	0,	3,	0,	2,				//’ŒV`Z
	},
	{
		0,	0,	1,	2,	0,	0,	0,	0,	//’ŒA`H
		0,	0,	0,	0,	1,	1,	1,	1,	//’ŒI`P
		0,	0,	0,	3,	2,				//’ŒQ`U
		0,	0,	0,	0,	0,				//’ŒV`Z
	},
	{
		0,	0,	0,	0,	0,	4,	4,	4,	//’ŒA`H
		2,	2,	3,	0,	0,	0,	0,	0,	//’ŒI`P
		0,	0,	1,	0,	0,				//’ŒQ`U
		0,	0,	0,	3,	2,				//’ŒV`Z
	},
	{
		0,	0,	0,	2,	0,	0,	0,	0,	//’ŒA`H
		0,	0,	0,	1,	0,	0,	0,	0,	//’ŒI`P
		0,	0,	1,	0,	0,				//’ŒQ`U
		0,	0,	2,	0,	0,				//’ŒV`Z
	},
	{
		0,	0,	0,	0,	2,	0,	0,	0,	//’ŒA`H
		0,	0,	0,	0,	1,	0,	0,	0,	//’ŒI`P
		0,	0,	2,	0,	0,				//’ŒQ`U
		0,	0,	1,	0,	0,				//’ŒV`Z
	},
};

//‹}‚®ƒ‚[ƒh‚É‚È‚éƒtƒŒ[ƒ€”
static const s32 g_gm_gmk_boss3_pillar_frame_change_hurry[GMD_GMK_BOSS3_PILLAR_PATTERN_NUM] = {
	4,
	4,
	5,
	4,
	5,
	3,
	3,
};


//------------------------------------------------------------------
//ƒp[ƒciƒ][ƒ“3j
//------------------------------------------------------------------
//‰Šú”z’u”÷’²®
static const Float g_gm_gmk_boss3_pillar_adjust_default_pos[GMD_GMK_BOSS3_PILLAR_TYPE_NUM][MTD_XY] = {
	{ -(GMD_GMK_BOSS3_PILLAR_SIZE-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT) - GMD_GMK_BOSS3_PILLAR_DEFAULT_OFFSET,	 GMD_GMK_BOSS3_PILLAR_SIZE/2 + GMD_GMK_BOSS3_PILLAR_OFFSET_Y}, 
	{  (GMD_GMK_BOSS3_PILLAR_SIZE-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT) + GMD_GMK_BOSS3_PILLAR_DEFAULT_OFFSET,	 GMD_GMK_BOSS3_PILLAR_SIZE/2 + GMD_GMK_BOSS3_PILLAR_OFFSET_Y}, 
	{  GMD_GMK_BOSS3_PILLAR_SIZE/2,	 -(GMD_GMK_BOSS3_PILLAR_SIZE-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT) + GMD_GMK_BOSS3_PILLAR_OFFSET_Y - GMD_GMK_BOSS3_PILLAR_DEFAULT_OFFSET}, 
	{  GMD_GMK_BOSS3_PILLAR_SIZE/2,	 (GMD_GMK_BOSS3_PILLAR_SIZE-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT) + GMD_GMK_BOSS3_PILLAR_OFFSET_Y + GMD_GMK_BOSS3_PILLAR_DEFAULT_OFFSET},
};

//ƒ‚ƒfƒ‹ID
static const s32 g_gm_boss3_pillar_model_id_left[GMD_GMK_BOSS3_PILLAR_HORIZONTAL_PARTS_NUM] = {
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_2A_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_2B_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_2C_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_2B_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_2C_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_2D_ZNO,
};
static const s32 g_gm_boss3_pillar_model_id_right[GMD_GMK_BOSS3_PILLAR_HORIZONTAL_PARTS_NUM] = {
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_2A_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_2B_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_2C_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_2B_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_2C_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_2D_ZNO,
};
static const s32 g_gm_boss3_pillar_model_id_top[GMD_GMK_BOSS3_PILLAR_VERTICAL_PARTS_NUM] = {
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_A_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_B_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_C_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_D_ZNO,
};
static const s32 g_gm_boss3_pillar_model_id_bottom[GMD_GMK_BOSS3_PILLAR_VERTICAL_PARTS_NUM] = {
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_A_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_B_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_C_ZNO,
	IDB_GMK_B3_PILLAR_MDL_GMK_B_PILLAR_D_ZNO,
};

//ƒp[ƒcƒIƒtƒZƒbƒg
static const s32 g_gm_boss3_pillar_parts_offset_left[GMD_GMK_BOSS3_PILLAR_HORIZONTAL_PARTS_NUM] = {
	0,
	0,
	2,
	4,
	6,
	8,
};
static const s32 g_gm_boss3_pillar_parts_offset_right[GMD_GMK_BOSS3_PILLAR_HORIZONTAL_PARTS_NUM] = {
	0,
	0,
	2,
	4,
	6,
	8,
};
static const s32 g_gm_boss3_pillar_parts_offset_top[GMD_GMK_BOSS3_PILLAR_VERTICAL_PARTS_NUM] = {
	0,
	0,
	2,
	4,
};
static const s32 g_gm_boss3_pillar_parts_offset_bottom[GMD_GMK_BOSS3_PILLAR_VERTICAL_PARTS_NUM] = {
	0,
	0,
	2,
	4,
};

//ƒp[ƒc”
static const s32 g_gm_gmk_boss3_pillar_parts_num[GMD_GMK_BOSS3_PILLAR_TYPE_NUM] = {
	GMD_GMK_BOSS3_PILLAR_HORIZONTAL_PARTS_NUM,
	GMD_GMK_BOSS3_PILLAR_HORIZONTAL_PARTS_NUM,
	GMD_GMK_BOSS3_PILLAR_VERTICAL_PARTS_NUM,
	GMD_GMK_BOSS3_PILLAR_VERTICAL_PARTS_NUM,
};
//ƒ‚ƒfƒ‹ID
static const s32* g_gm_boss3_pillar_model_id[GMD_GMK_BOSS3_PILLAR_TYPE_NUM] = {
	g_gm_boss3_pillar_model_id_left,
	g_gm_boss3_pillar_model_id_right,
	g_gm_boss3_pillar_model_id_top,
	g_gm_boss3_pillar_model_id_bottom,
};
//ƒp[ƒcƒIƒtƒZƒbƒg
static const s32* g_gm_boss3_pillar_parts_offset[GMD_GMK_BOSS3_PILLAR_TYPE_NUM] = {
	g_gm_boss3_pillar_parts_offset_left,
	g_gm_boss3_pillar_parts_offset_right,
	g_gm_boss3_pillar_parts_offset_top,
	g_gm_boss3_pillar_parts_offset_bottom,
};
//’nŒ`‹éŒ`—p•‚Æ‚‚³
static const u32 g_gm_boss3_pillar_field_rect_wh[GMD_GMK_BOSS3_PILLAR_TYPE_NUM][MTD_WH] = {
	{GMD_GMK_BOSS3_PILLAR_HORIZONTAL_WIDTH, GMD_GMK_BOSS3_PILLAR_HORIZONTAL_HEIGHT},
	{GMD_GMK_BOSS3_PILLAR_HORIZONTAL_WIDTH, GMD_GMK_BOSS3_PILLAR_HORIZONTAL_HEIGHT},
	{GMD_GMK_BOSS3_PILLAR_VERTICAL_WIDTH, GMD_GMK_BOSS3_PILLAR_VERTICAL_HEIGHT},
	{GMD_GMK_BOSS3_PILLAR_VERTICAL_WIDTH, GMD_GMK_BOSS3_PILLAR_VERTICAL_HEIGHT},
};
//’nŒ`‹éŒ`—pƒIƒtƒZƒbƒg
static const s32 g_gm_boss3_pillar_field_rect_offset[GMD_GMK_BOSS3_PILLAR_TYPE_NUM][MTD_XY] = {
	{	-(GMD_GMK_BOSS3_PILLAR_SIZE-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT),
		-GMD_GMK_BOSS3_PILLAR_HORIZONTAL_HEIGHT/2
	},
	{	-(GMD_GMK_BOSS3_PILLAR_SIZE-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT),
		-GMD_GMK_BOSS3_PILLAR_HORIZONTAL_HEIGHT/2
	},
	{	-GMD_GMK_BOSS3_PILLAR_VERTICAL_WIDTH/2,	
		-(GMD_GMK_BOSS3_PILLAR_SIZE-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT)
	},
	{
		-GMD_GMK_BOSS3_PILLAR_VERTICAL_WIDTH/2,	
		-(GMD_GMK_BOSS3_PILLAR_SIZE-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT)
	},
};

//Œü‚«’²®’l
static const s32 g_gm_boss3_pillar_adjust_dir[GMD_GMK_BOSS3_PILLAR_TYPE_NUM][MTD_XY] = {
	{ 1,  0},
	{-1,  0},
	{ 0,  1},
	{ 0, -1},
};

//ƒqƒbƒgƒGƒtƒFƒNƒg‚Ì—L–³
static const s32 g_gm_gmk_boss3_pillar_flag_hit_effect[GMD_GMK_BOSS3_PILLAR_PATTERN_NUM][GMD_GMK_BOSS_PILLAR_NUM] = {
	{
		0,	1,	0,	0,	0,	0,	1,	0,	//’ŒA`H
		1,	0,	1,	0,	0,	1,	0,	1,	//’ŒI`P
		0,	0,	0,	0,	0,				//’ŒQ`U
		0,	0,	0,	0,	0,				//’ŒV`Z
	},
	{
		0,	0,	0,	0,	0,	0,	0,	0,	//’ŒA`H
		0,	0,	0,	0,	0,	0,	0,	0,	//’ŒI`P
		0,	0,	1,	1,	1,				//’ŒQ`U
		0,	0,	1,	1,	1,				//’ŒV`Z
	},
	{
		1,	1,	1,	1,	0,	0,	0,	0,	//’ŒA`H
		0,	0,	0,	0,	0,	0,	0,	0,	//’ŒI`P
		0,	0,	1,	1,	0,				//’ŒQ`U
		0,	0,	1,	0,	1,				//’ŒV`Z
	},
	{
		0,	0,	1,	0,	0,	0,	0,	0,	//’ŒA`H
		0,	0,	0,	0,	0,	0,	0,	0,	//’ŒI`P
		0,	0,	0,	1,	0,				//’ŒQ`U
		0,	0,	0,	0,	0,				//’ŒV`Z
	},
	{
		0,	0,	0,	0,	0,	0,	0,	0,	//’ŒA`H
		0,	0,	0,	0,	0,	0,	0,	0,	//’ŒI`P
		0,	0,	0,	0,	0,				//’ŒQ`U
		0,	0,	0,	1,	1,				//’ŒV`Z
	},
	{
		0,	0,	0,	1,	0,	0,	0,	0,	//’ŒA`H
		0,	0,	0,	1,	0,	0,	0,	0,	//’ŒI`P
		0,	0,	1,	0,	0,				//’ŒQ`U
		0,	0,	1,	0,	0,				//’ŒV`Z
	},
	{
		0,	0,	0,	0,	1,	0,	0,	0,	//’ŒA`H
		0,	0,	0,	0,	1,	0,	0,	0,	//’ŒI`P
		0,	0,	1,	0,	0,				//’ŒQ`U
		0,	0,	1,	0,	0,				//’ŒV`Z
	},
};



//------------------------------------------------------------------
//•Çiƒ][ƒ“3j
//------------------------------------------------------------------
//•Ç‚Ì‰Šú”z’u
static const fx32 g_gm_gmk_boss3_pillar_wall_default_pos[GMD_GMK_BOSS3_PILLAR_WALL_TYPE_NUM][MTD_XY] = {
	{-GMD_GMK_BOSS3_PILLAR_WALL_SIZE/2, GMD_GMK_BOSS3_PILLAR_WALL_SIZE/2 + GMD_GMK_BOSS3_PILLAR_WALL_OFFSET_Y },
	{ GMD_GMK_BOSS3_PILLAR_WALL_SIZE/2 + GMD_GMK_BOSS3_PILLAR_SIZE*8, GMD_GMK_BOSS3_PILLAR_WALL_SIZE/2 + GMD_GMK_BOSS3_PILLAR_WALL_OFFSET_Y },
};


//------------------------------------------------------------------
//ƒp[ƒciƒtƒ@ƒCƒiƒ‹ƒ][ƒ“j
//------------------------------------------------------------------
//‰Šú”z’u”÷’²®
static const Float g_gm_gmk_boss3_pillar_f_adjust_default_pos[GMD_GMK_BOSS3_PILLAR_TYPE_NUM][MTD_XY] = {
	{ -(GMD_GMK_BOSS3_PILLAR_SIZE) - GMD_GMK_BOSS3_PILLAR_DEFAULT_OFFSET,	 GMD_GMK_BOSS3_PILLAR_SIZE/2 + GMD_GMK_BOSS3_PILLAR_OFFSET_Y}, 
	{  (GMD_GMK_BOSS3_PILLAR_SIZE) + GMD_GMK_BOSS3_PILLAR_DEFAULT_OFFSET,	 GMD_GMK_BOSS3_PILLAR_SIZE/2 + GMD_GMK_BOSS3_PILLAR_OFFSET_Y}, 
	{  GMD_GMK_BOSS3_PILLAR_SIZE/2,	 -(GMD_GMK_BOSS3_PILLAR_SIZE) + GMD_GMK_BOSS3_PILLAR_OFFSET_Y - GMD_GMK_BOSS3_PILLAR_DEFAULT_OFFSET}, 
	{  GMD_GMK_BOSS3_PILLAR_SIZE/2,	 (GMD_GMK_BOSS3_PILLAR_SIZE) + GMD_GMK_BOSS3_PILLAR_OFFSET_Y + GMD_GMK_BOSS3_PILLAR_DEFAULT_OFFSET},
};

//ƒ‚ƒfƒ‹ID
static const s32 g_gm_boss3_pillar_f_model_id_left[GMD_GMK_BOSS3_PILLAR_F_HORIZONTAL_PARTS_NUM] = {
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_1H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_2H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_2H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_2H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_2H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_2H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_2H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_2H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_2H_ZNO,
};
static const s32 g_gm_boss3_pillar_f_model_id_right[GMD_GMK_BOSS3_PILLAR_F_HORIZONTAL_PARTS_NUM] = {
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_3H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_4H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_4H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_4H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_4H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_4H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_4H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_4H_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_4H_ZNO,
};
static const s32 g_gm_boss3_pillar_f_model_id_top[GMD_GMK_BOSS3_PILLAR_F_VERTICAL_PARTS_NUM] = {
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_3V_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_4V_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_4V_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_4V_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_4V_ZNO,
};
static const s32 g_gm_boss3_pillar_f_model_id_bottom[GMD_GMK_BOSS3_PILLAR_F_VERTICAL_PARTS_NUM] = {
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_1V_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_2V_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_2V_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_2V_ZNO,
	IDB_GMK_B3_PILLAR_F_MDL_GMK_B_PILLAR_F_2V_ZNO,
};

//ƒp[ƒcƒIƒtƒZƒbƒg
static const s32 g_gm_boss3_pillar_f_parts_offset_left[GMD_GMK_BOSS3_PILLAR_F_HORIZONTAL_PARTS_NUM] = {
	0,
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
};
static const s32 g_gm_boss3_pillar_f_parts_offset_right[GMD_GMK_BOSS3_PILLAR_F_HORIZONTAL_PARTS_NUM] = {
	0,
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
};
static const s32 g_gm_boss3_pillar_f_parts_offset_top[GMD_GMK_BOSS3_PILLAR_F_VERTICAL_PARTS_NUM] = {
	0,
	0,
	1,
	2,
	3,
};
static const s32 g_gm_boss3_pillar_f_parts_offset_bottom[GMD_GMK_BOSS3_PILLAR_F_VERTICAL_PARTS_NUM] = {
	0,
	0,
	1,
	2,
	3,
};

//ƒp[ƒc”
static const s32 g_gm_gmk_boss3_pillar_f_parts_num[GMD_GMK_BOSS3_PILLAR_TYPE_NUM] = {
	GMD_GMK_BOSS3_PILLAR_F_HORIZONTAL_PARTS_NUM,
	GMD_GMK_BOSS3_PILLAR_F_HORIZONTAL_PARTS_NUM,
	GMD_GMK_BOSS3_PILLAR_F_VERTICAL_PARTS_NUM,
	GMD_GMK_BOSS3_PILLAR_F_VERTICAL_PARTS_NUM,
};
#if !GMD_GMK_BOSS3_PILLAR_TEST_TVX
//ƒ}ƒeƒŠƒAƒ‹ID
static const s32 g_gm_boss3_pillar_f_mat_id_left[GMD_GMK_BOSS3_PILLAR_F_HORIZONTAL_PARTS_NUM] = {
	-1,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_4H_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_4H_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_4H_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_4H_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_4H_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_4H_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_4H_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_4H_ZNV,
};
static const s32 g_gm_boss3_pillar_f_mat_id_right[GMD_GMK_BOSS3_PILLAR_F_HORIZONTAL_PARTS_NUM] = {
	-1,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_2H_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_2H_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_2H_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_2H_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_2H_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_2H_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_2H_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_2H_ZNV,
};
static const s32 g_gm_boss3_pillar_f_mat_id_top[GMD_GMK_BOSS3_PILLAR_F_VERTICAL_PARTS_NUM] = {
	-1,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_2V_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_2V_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_2V_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_2V_ZNV,
};
static const s32 g_gm_boss3_pillar_f_mat_id_bottom[GMD_GMK_BOSS3_PILLAR_F_VERTICAL_PARTS_NUM] = {
	-1,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_4V_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_4V_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_4V_ZNV,
	IDB_GMK_B3_PILLAR_F_MAT_GMK_B_PILLAR_F_4V_ZNV,
};
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX
//ƒ‚ƒfƒ‹ID
static const s32* g_gm_boss3_pillar_f_model_id[GMD_GMK_BOSS3_PILLAR_TYPE_NUM] = {
	g_gm_boss3_pillar_f_model_id_left,
	g_gm_boss3_pillar_f_model_id_right,
	g_gm_boss3_pillar_f_model_id_top,
	g_gm_boss3_pillar_f_model_id_bottom,
};
//ƒp[ƒcƒIƒtƒZƒbƒg
static const s32* g_gm_boss3_pillar_f_parts_offset[GMD_GMK_BOSS3_PILLAR_TYPE_NUM] = {
	g_gm_boss3_pillar_f_parts_offset_left,
	g_gm_boss3_pillar_f_parts_offset_right,
	g_gm_boss3_pillar_f_parts_offset_top,
	g_gm_boss3_pillar_f_parts_offset_bottom,
};
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
// ƒTƒuƒ‚ƒfƒ‹ID
static const s32 g_gm_boss3_pillar_f_sub_model_id[GMD_GMK_BOSS3_PILLAR_TYPE_NUM] = {
	IDB_GMK_B3_PILLAR_F_TVX_GMK_B_PILLAR_F_4H_P_TVX,
	IDB_GMK_B3_PILLAR_F_TVX_GMK_B_PILLAR_F_2H_P_TVX,
	IDB_GMK_B3_PILLAR_F_TVX_GMK_B_PILLAR_F_2V_P_TVX,
	IDB_GMK_B3_PILLAR_F_TVX_GMK_B_PILLAR_F_4V_P_TVX,
};
#else 
//ƒ}ƒeƒŠƒAƒ‹ID
static const s32* g_gm_boss3_pillar_f_mat_id[GMD_GMK_BOSS3_PILLAR_TYPE_NUM] = {
	g_gm_boss3_pillar_f_mat_id_left,
	g_gm_boss3_pillar_f_mat_id_right,
	g_gm_boss3_pillar_f_mat_id_top,
	g_gm_boss3_pillar_f_mat_id_bottom,
};
#endif // !GMD_GMK_BOSS3_PILLAR_TEST_TVX
//’nŒ`‹éŒ`—p•‚Æ‚‚³
static const u32 g_gm_boss3_pillar_f_field_rect_wh[GMD_GMK_BOSS3_PILLAR_TYPE_NUM][MTD_WH] = {
	{GMD_GMK_BOSS3_PILLAR_HORIZONTAL_WIDTH, GMD_GMK_BOSS3_PILLAR_HORIZONTAL_HEIGHT},
	{GMD_GMK_BOSS3_PILLAR_HORIZONTAL_WIDTH, GMD_GMK_BOSS3_PILLAR_HORIZONTAL_HEIGHT},
	{GMD_GMK_BOSS3_PILLAR_VERTICAL_WIDTH, GMD_GMK_BOSS3_PILLAR_VERTICAL_HEIGHT},
	{GMD_GMK_BOSS3_PILLAR_VERTICAL_WIDTH, GMD_GMK_BOSS3_PILLAR_VERTICAL_HEIGHT},
};
//’nŒ`‹éŒ`—pƒIƒtƒZƒbƒg
static const s32 g_gm_boss3_pillar_f_field_rect_offset[GMD_GMK_BOSS3_PILLAR_TYPE_NUM][MTD_XY] = {
	{	-(GMD_GMK_BOSS3_PILLAR_HORIZONTAL_WIDTH - (GMD_GMK_BOSS3_PILLAR_SIZE)),
		-GMD_GMK_BOSS3_PILLAR_HORIZONTAL_HEIGHT/2
	},
	{	-(GMD_GMK_BOSS3_PILLAR_SIZE),
		-GMD_GMK_BOSS3_PILLAR_HORIZONTAL_HEIGHT/2
	},
	{	-GMD_GMK_BOSS3_PILLAR_VERTICAL_WIDTH/2,	
		-(GMD_GMK_BOSS3_PILLAR_VERTICAL_HEIGHT - (GMD_GMK_BOSS3_PILLAR_SIZE))
	},
	{
		-GMD_GMK_BOSS3_PILLAR_VERTICAL_WIDTH/2,	
		-(GMD_GMK_BOSS3_PILLAR_SIZE)
	},
};

//Œü‚«’²®’l
static const s32 g_gm_boss3_pillar_f_adjust_dir[GMD_GMK_BOSS3_PILLAR_TYPE_NUM][MTD_XY] = {
	{ 1,  0},
	{-1,  0},
	{ 0,  1},
	{ 0, -1},
};
//------------------------------------------------------------------
//•Çiƒ][ƒ“ƒtƒ@ƒCƒiƒ‹j
//------------------------------------------------------------------
//•Ç‚Ì‰Šú”z’u
static const fx32 g_gm_gmk_boss3_pillar_f_wall_default_pos[GMD_GMK_BOSS3_PILLAR_WALL_TYPE_NUM][MTD_XY] = {
	{-GMD_GMK_BOSS3_PILLAR_WALL_SIZE/2, GMD_GMK_BOSS3_PILLAR_WALL_SIZE_F/2 + GMD_GMK_BOSS3_PILLAR_WALL_OFFSET_Y_F },
	{ GMD_GMK_BOSS3_PILLAR_WALL_SIZE/2 + GMD_GMK_BOSS3_PILLAR_SIZE*8, GMD_GMK_BOSS3_PILLAR_WALL_SIZE_F/2 + GMD_GMK_BOSS3_PILLAR_WALL_OFFSET_Y_F },
};




static OBS_ACTION3D_NN_WORK* g_gm_gmk_boss3_pillar_obj_3d_list = NULL;
static OBS_ACTION3D_NN_WORK* g_gm_gmk_boss3_wall_obj_3d_list = NULL;
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
static AMS_AMB_HEADER* g_gm_gmk_boss3_pillar_obj_tvx_list = NULL;
static AMS_AMB_HEADER* g_gm_gmk_boss3_wall_obj_tvx_list = NULL;
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX

//----- Global Functions ----------------------------------------------------

// ==========================================================================
// GmGmkBoss3PillarBuild
/*!
 *	ƒMƒ~ƒbƒN ƒ{ƒX3’Œ ƒf[ƒ^\’z
 */
// ==========================================================================
void GmGmkBoss3PillarBuild(void)
{
	g_gm_gmk_boss3_pillar_obj_3d_list = GmGameDBuildRegBuildModel(
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BOSS3_PILLAR_MODEL ),
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BOSS3_PILLAR_TEX ),
			0	//draw_flag
			);
	g_gm_gmk_boss3_wall_obj_3d_list = GmGameDBuildRegBuildModel(
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BOSS3_WALL_MODEL ),
			(AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BOSS3_WALL_TEX ),
			0	//draw_flag
			);
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
	void* tvx = GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BOSS3_PILLAR_TVX );
	amBindConv((Uint8*)tvx);
	g_gm_gmk_boss3_pillar_obj_tvx_list = (AMS_AMB_HEADER*)tvx;
	tvx = GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BOSS3_WALL_TVX );
	amBindConv((Uint8*)tvx);
	g_gm_gmk_boss3_wall_obj_tvx_list = (AMS_AMB_HEADER*)tvx;
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX
#if _IPHONE
	gm_gmk_boss3_pillar_se_use_count = 0;
	gm_gmk_boss3_pillar_global_flag = 0;
#endif // _IPHONE
}

// ==========================================================================
// GmGmkBoss3PillarFlush
/*!
 *	ƒMƒ~ƒbƒN ƒ{ƒX3’Œ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
void GmGmkBoss3PillarFlush(void)
{
	AMS_AMB_HEADER* amb_header_pillar = (AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BOSS3_PILLAR_MODEL );
	GmGameDBuildRegFlushModel( g_gm_gmk_boss3_pillar_obj_3d_list, amb_header_pillar->file_num );
	g_gm_gmk_boss3_pillar_obj_3d_list = NULL;

	AMS_AMB_HEADER* amb_header_wall = (AMS_AMB_HEADER*)GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BOSS3_WALL_MODEL );
	GmGameDBuildRegFlushModel( g_gm_gmk_boss3_wall_obj_3d_list, amb_header_wall->file_num );
	g_gm_gmk_boss3_wall_obj_3d_list = NULL;
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
	g_gm_gmk_boss3_pillar_obj_tvx_list = NULL;
	g_gm_gmk_boss3_wall_obj_tvx_list   = NULL;
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX
#if _IPHONE
	gm_gmk_boss3_pillar_se_use_count = 0;
	gm_gmk_boss3_pillar_global_flag = 0;
#endif // _IPHONE
}

// ==========================================================================
// GmGmkBoss3PillarInitManager
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒ{ƒX3’Œ
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkBoss3PillarInitManager( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	UNREFERENCED_PARAMETER( type );

	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_GMK_BOSS3_PILLAR_MANAGER_WORK* manager_work = (GMS_GMK_BOSS3_PILLAR_MANAGER_WORK*)gmGmkBoss3PillarLoadObjNoModel( 
			eve_rec, 
			pos_x, 
			pos_y, 
			sizeof(GMS_GMK_BOSS3_PILLAR_MANAGER_WORK));
	amAssert( manager_work );

	OBS_OBJECT_WORK* obj_work_manager = (OBS_OBJECT_WORK*)manager_work;

	//‰Šú‰»
	gmGmkBoss3PillarManagerInit( obj_work_manager );

	//•Ç¶¬
	for ( s32 i = 0; GMD_GMK_BOSS3_PILLAR_WALL_TYPE_NUM > i; ++i ){
#if GMD_GMK_BOSS3_PILLAR_TEST_NO_WALL
		continue;
#endif	//GMD_GMK_BOSS3_PILLAR_TEST_NO_WALL

		s32 default_pos_x = 0;
		s32 default_pos_y = 0;
		GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
		if ( zone_type == GSD_MAIN_ZONE_TYPE_3 ){
			default_pos_x = g_gm_gmk_boss3_pillar_wall_default_pos[i][MTD_X];
			default_pos_y = g_gm_gmk_boss3_pillar_wall_default_pos[i][MTD_Y];
		}
		else if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL ){
			default_pos_x = g_gm_gmk_boss3_pillar_f_wall_default_pos[i][MTD_X];
			default_pos_y = g_gm_gmk_boss3_pillar_f_wall_default_pos[i][MTD_Y];
		}
		else{
			amAssert(FALSE);
		}

		OBS_OBJECT_WORK* obj_work_part = GmEventMgrLocalEventBirth(
				GMD_EVENT_ID_BOSS3_PILLAR_WALL,
				obj_work_manager->pos.x + default_pos_x*FX32_ONE,
				obj_work_manager->pos.y + default_pos_y*FX32_ONE,
				eve_rec->flag,
				eve_rec->left,
				eve_rec->top,
				eve_rec->width,
				eve_rec->height,
				(u8)i );

		//eİ’è
		obj_work_part->parent_obj = obj_work_manager;

		//ŠÇ—‚É“o˜^
		manager_work->obj_work_wall[i] = obj_work_part;

		//I—¹Ï‚İ
		if ( manager_work->gimmick_work.ene_com.eve_rec->byte_param[1] & GMD_GMK_BOSS3_PILLAR_BYTE_PARAM_FLAG_FINISH ){
			//¶‘¤‚È‚ç•Â‚¶‚½‚Ü‚Ü
			if ( i == GMD_GMK_BOSS3_PILLAR_WALL_TYPE_LEFT ){
				fx32 distance = (GMD_GMK_BOSS3_PILLAR_WALL_HEIGHT+1) * GMD_GMK_BOSS3_PILLAR_WALL_SIZE * FX32_ONE;
				obj_work_part->pos.y -= distance;
			}
		}
	}
	

	return obj_work_manager;
}

// ==========================================================================
// GmGmkBoss3PillarInitParts
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒ{ƒX3’Œ
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkBoss3PillarInitParts( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	UNREFERENCED_PARAMETER( type );

	//’Œƒ^ƒCƒvæ“¾
	GME_GMK_BOSS3_PILLAR_TYPE pillar_type = gmGmkBoss3PillarCalcPillarType(eve_rec->id);

	s32 parts_num = 0;
	const s32* model_id_list = NULL;
#if !GMD_GMK_BOSS3_PILLAR_TEST_TVX
	const s32* mat_id_list = NULL;
#endif // !GMD_GMK_BOSS3_PILLAR_TEST_TVX
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	if ( zone_type == GSD_MAIN_ZONE_TYPE_3 ){
		parts_num = g_gm_gmk_boss3_pillar_parts_num[pillar_type];
		model_id_list = g_gm_boss3_pillar_model_id[pillar_type];
#if !GMD_GMK_BOSS3_PILLAR_TEST_TVX
		mat_id_list = NULL;
#endif // !GMD_GMK_BOSS3_PILLAR_TEST_TVX
	}
	else if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL ){
		parts_num = g_gm_gmk_boss3_pillar_f_parts_num[pillar_type];
		model_id_list = g_gm_boss3_pillar_f_model_id[pillar_type];
#if !GMD_GMK_BOSS3_PILLAR_TEST_TVX
		mat_id_list = g_gm_boss3_pillar_f_mat_id[pillar_type];
#endif // !GMD_GMK_BOSS3_PILLAR_TEST_TVX
	}
	else{
		amAssert(FALSE);
		return NULL;
	}

	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkBoss3PillarLoadObj( 
			eve_rec, 
			pos_x, 
			pos_y, 
			g_gm_gmk_boss3_pillar_obj_3d_list,
			model_id_list[0],
			sizeof(GMS_GMK_BOSS3_PILLAR_MAIN_WORK));
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)gimmick_work;
	amAssert( obj_work );

	//‰Šú‰»
	gmGmkBoss3PillarPartsInitMain( obj_work, pillar_type );
	//ƒTƒuƒp[ƒc¶¬	
	GMS_GMK_BOSS3_PILLAR_MAIN_WORK* pillar_work = (GMS_GMK_BOSS3_PILLAR_MAIN_WORK*)gimmick_work;
	for ( s32 i = 1; parts_num > i; ++i ){
		//ƒ‚ƒfƒ‹
		s32 model_id = model_id_list[i];
		ObjCopyAction3dNNModel(
				&g_gm_gmk_boss3_pillar_obj_3d_list[model_id],
				&pillar_work->obj_3d_parts[i-1]);
#if !_IPHONE
		//ƒ}ƒeƒŠƒAƒ‹
		if ( mat_id_list && (mat_id_list[i] != -1) ){
			ObjAction3dNNMaterialMotionLoad(
					&pillar_work->obj_3d_parts[i-1],
					0,
					NULL, 
					NULL,
					mat_id_list[i], 
					ObjDataGet( GMD_DWORK_NO_GMK_BOSS3_PILLAR_MAT )->pData);
		}
#endif // !_IPHONE
		pillar_work->obj_3d_parts[i-1].drawflag |= NND_DRAWOBJ_DOUBLESIDE;
	}
	return obj_work;
}

// ==========================================================================
// GmGmkBoss3PillarInitWall
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒ{ƒX3•Ç
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkBoss3PillarInitWall( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type )
{
	UNREFERENCED_PARAMETER( type );

#if GMD_GMK_BOSS3_PILLAR_TEST_NO_WALL
	UNREFERENCED_PARAMETER( eve_rec );
	UNREFERENCED_PARAMETER( pos_x );
	UNREFERENCED_PARAMETER( pos_y );
	return NULL;
#endif	//GMD_GMK_BOSS3_PILLAR_TEST_NO_WALL

	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkBoss3PillarLoadObj( 
			eve_rec, 
			pos_x, 
			pos_y, 
			g_gm_gmk_boss3_wall_obj_3d_list,
			IDB_GMK_SW_WALL3_MDL_GMK_SW_WALL_3_ZNO,
			sizeof(GMS_GMK_BOSS3_PILLAR_WALL_WORK));
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)gimmick_work;
	amAssert( obj_work );
	GMS_GMK_BOSS3_PILLAR_WALL_WORK* wall_work = (GMS_GMK_BOSS3_PILLAR_WALL_WORK*)obj_work;
	//-------------------------------------------------
	// ƒp[ƒc
	//-------------------------------------------------
	//Bƒp[ƒc
	ObjCopyAction3dNNModel(
			&g_gm_gmk_boss3_wall_obj_3d_list[IDB_GMK_SW_WALL3_MDL_GMK_SW_WALL_S_3_ZNO],
			&wall_work->obj_3d_parts[GMD_GMK_BOSS3_PILLAR_WALL_PARTS_B]);
#if !_IPHONE
	//ƒOƒŒƒA
	ObjCopyAction3dNNModel(
			&g_gm_gmk_boss3_wall_obj_3d_list[IDB_GMK_SW_WALL3_MDL_GMK_SW_WALL_G_3_ZNO],
			&wall_work->obj_3d_parts[GMD_GMK_BOSS3_PILLAR_WALL_PARTS_GLARE]);
	ObjAction3dNNMaterialMotionLoad(
			&wall_work->obj_3d_parts[GMD_GMK_BOSS3_PILLAR_WALL_PARTS_GLARE],
			0,
			ObjDataGet( GMD_DWORK_NO_GMK_BOSS3_WALL_MAT ), 
			NULL,
			0, 
			NULL,
			1, 
			1 );
#endif // !_IPHONE
	//‰Šú‰»
	gmGmkBoss3PillarWallInit( obj_work );

	return obj_work;
}


//----------------------------------------------------------------
//’Œ
//----------------------------------------------------------------

// ==========================================================================
// GmGmkBoss3PillarChangeModeActive
/*!
 *	ƒAƒNƒeƒBƒu‚É•ÏX@ƒ{ƒX3’Œ
 *
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param pattern_no	[in] ƒpƒ^[ƒ“”Ô†
 */
// ==========================================================================
void GmGmkBoss3PillarChangeModeActive( OBS_OBJECT_WORK* obj_work, s32 pattern_no )
{
	GMS_GMK_BOSS3_PILLAR_MANAGER_WORK* manager_work = (GMS_GMK_BOSS3_PILLAR_MANAGER_WORK*)obj_work;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( manager_work );

	//’ŒoŒ»ŠÔŠuiƒtƒ@ƒCƒiƒ‹ƒ][ƒ“‚Ìê‡ƒXƒs[ƒh’²®j
	Float interval = GMD_GMK_BOSS3_PILLAR_SIZE/GMD_GMK_BOSS3_PILLAR_SPEED_NORMAL;
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL ){
		interval /= GMD_GMK_BOSS3_PILLAR_SPEED_ADJUST;
	}

	//’Œ¶¬
	for ( s32 i = 0; GMD_GMK_BOSS_PILLAR_NUM > i; ++i ){
		OBS_OBJECT_WORK* obj_work_pillar = manager_work->obj_work_pillar[i];
		s32 distance = g_gm_gmk_boss3_pillar_move_distance[pattern_no][i] * GMD_GMK_BOSS3_PILLAR_SIZE;

		//‹——£‚ªİ’è‚³‚ê‚Ä‚¢‚È‚¢‚à‚Ì‚Í¶¬‚µ‚È‚¢
		if ( distance == 0 ){
			if ( obj_work_pillar ){
				//íœ—v‹
				obj_work_pillar->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;

				//ƒGƒtƒFƒNƒgíœ—v‹
				GMS_GMK_BOSS3_PILLAR_MAIN_WORK* main_work = (GMS_GMK_BOSS3_PILLAR_MAIN_WORK*)obj_work_pillar;
				if ( main_work->effect_work ){
					ObjDrawKillAction3DES( (OBS_OBJECT_WORK*)main_work->effect_work );
					main_work->effect_work = NULL;
				}

				//ŠÇ—‚©‚ç‰ğœ
				manager_work->obj_work_pillar[i] = NULL;
			}
			continue;
		}

		//¶¬‚³‚ê‚Ä‚¢‚È‚¢ê‡Aì¬
		if ( !obj_work_pillar ){
			u16 event_id = (u16)g_gm_gmk_boss3_pillar_event_id[i];

			obj_work_pillar = GmEventMgrLocalEventBirth(
					event_id,
					obj_work->pos.x + g_gm_gmk_boss3_pillar_default_pos[i][MTD_X] * GMD_GMK_BOSS3_PILLAR_SIZE * FX32_ONE,
					obj_work->pos.y + g_gm_gmk_boss3_pillar_default_pos[i][MTD_Y] * GMD_GMK_BOSS3_PILLAR_SIZE * FX32_ONE,
					gimmick_work->ene_com.eve_rec->flag,
					gimmick_work->ene_com.eve_rec->left,
					gimmick_work->ene_com.eve_rec->top,
					gimmick_work->ene_com.eve_rec->width,
					gimmick_work->ene_com.eve_rec->height,
					0 );

			//eİ’è
			obj_work_pillar->parent_obj = obj_work;

			//ŠÇ—ƒ[ƒN‚É“o˜^
			manager_work->obj_work_pillar[i] = obj_work_pillar;
		}
		//¶¬‚³‚ê‚Ä‚¢‚éê‡A‰Šú‰»
		else{
			gmGmkBoss3PillarPartsChangeModeWait( obj_work_pillar );
		}

		//ƒqƒbƒgƒtƒ‰ƒOİ’è
		if ( g_gm_gmk_boss3_pillar_flag_hit_effect[pattern_no][i]){
			obj_work_pillar->user_flag |= GMD_GMK_BOSS3_PILLAR_FLAG_HIT_EFFECT | GMD_GMK_BOSS3_PILLAR_FLAG_SE_2;
#if _IPHONE
			gm_gmk_boss3_pillar_global_flag |= GMD_GMK_BOSS3_PILLAR_FLAG_SE_2;
#endif // _IPHONE
		}

		//‘Ò‚¿ŠÔİ’è
		s32 wait_time = g_gm_gmk_boss3_pillar_wait_frame[pattern_no][i] * (s32)interval;
		gmGmkBoss3PillarPartsChangeModeNormal( obj_work_pillar, (distance + GMD_GMK_BOSS3_PILLAR_DEFAULT_OFFSET) * FX32_ONE, wait_time );
	}

	//ƒpƒ^[ƒ“”Ô†İ’è
	manager_work->pattern_no = pattern_no;
	
	//ƒƒCƒ“ˆ—•ÏX
	obj_work->user_timer = (s32)(g_gm_gmk_boss3_pillar_frame_change_hurry[manager_work->pattern_no] * interval);
	obj_work->ppFunc = gmGmkBoss3PillarManagerMainFuncWaitHurry;
}

// ==========================================================================
// GmGmkBoss3PillarChangeModeHurry
/*!
 *	ƒXƒs[ƒhƒAƒbƒv‚É•ÏX@ƒ{ƒX3’Œ
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param type		[in] ƒ^ƒCƒv
 */
// ==========================================================================
void GmGmkBoss3PillarChangeModeHurry( OBS_OBJECT_WORK* obj_work )
{
	GMS_GMK_BOSS3_PILLAR_MANAGER_WORK* manager_work = (GMS_GMK_BOSS3_PILLAR_MANAGER_WORK*)obj_work;

	//’ŒŒŸõ
	for ( s32 i = 0; GMD_GMK_BOSS_PILLAR_NUM > i; ++i ){
		OBS_OBJECT_WORK* pillar_obj_work = manager_work->obj_work_pillar[i];
		if ( !pillar_obj_work ){
			continue;
		}

		//‹}‚®ƒ‚[ƒh‚É•ÏX
		gmGmkBoss3PillarPartsChangeModeHurry( pillar_obj_work, GMD_GMK_BOSS3_PILLAR_FRAME_HURRY );
	}
}

// ==========================================================================
// GmGmkBoss3PillarChangeModeWait
/*!
 *	‘Ò‹@‚É•ÏX@ƒ{ƒX3’Œ
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void GmGmkBoss3PillarChangeModeReturn( OBS_OBJECT_WORK* obj_work )
{
	GMS_GMK_BOSS3_PILLAR_MANAGER_WORK* manager_work = (GMS_GMK_BOSS3_PILLAR_MANAGER_WORK*)obj_work;

	//’ŒŒŸõ
	for ( s32 i = 0; GMD_GMK_BOSS_PILLAR_NUM > i; ++i ){
		OBS_OBJECT_WORK* pillar_obj_work = manager_work->obj_work_pillar[i];
		if ( !pillar_obj_work ){
			continue;
		}
		//–ß‚éƒ‚[ƒh‚É•ÏX
		gmGmkBoss3PillarPartsChangeModeReturn( pillar_obj_work, 0, GMD_GMK_BOSS3_PILLAR_FRAME_RETURN );
	}

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->user_timer = GMD_GMK_BOSS3_PILLAR_FRAME_RETURN;
	obj_work->ppFunc = gmGmkBoss3PillarManagerMainFuncWaitReturn;
}

// ==========================================================================
// GmGmkBoss3PillarChangeModeDelete
/*!
 *	íœ‚É•ÏX@ƒ{ƒX3’Œ
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void GmGmkBoss3PillarChangeModeDelete( OBS_OBJECT_WORK* obj_work )
{
	GMS_GMK_BOSS3_PILLAR_MANAGER_WORK* manager_work = (GMS_GMK_BOSS3_PILLAR_MANAGER_WORK*)obj_work;

	//’ŒŒŸõ
	for ( s32 i = 0; GMD_GMK_BOSS_PILLAR_NUM > i; ++i ){
		OBS_OBJECT_WORK* obj_work_pillar = (OBS_OBJECT_WORK*)manager_work->obj_work_pillar[i];
		if ( !obj_work_pillar ){
			continue;
		}
		//íœ—v‹
		obj_work_pillar->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;

		//ŠÇ—‚©‚ç‰ğœ
		manager_work->obj_work_pillar[i] = NULL;
	}

	//ƒƒCƒ“ˆ—•ÏX
#if _IPHONE
	obj_work->ppFunc = gmGmkBoss3PillarManagerMainFuncFw;
#else
	obj_work->ppFunc = NULL;
#endif // _IPHONE
}

// ==========================================================================
// GmGmkBoss3PillarGetActiveTime
/*!
 *	ƒAƒNƒeƒBƒuó‘Ô‚ÌƒtƒŒ[ƒ€”‚ğæ“¾
 *
 *	@param pattern_no	[in] ƒpƒ^[ƒ“”Ô†
 */
// ==========================================================================
s32 GmGmkBoss3PillarGetActiveTime( s32 pattern_no)
{
	amAssert( 0 <= pattern_no && pattern_no < GMD_GMK_BOSS3_PILLAR_PATTERN_NUM );

	//’ŒoŒ»ŠÔŠuiƒtƒ@ƒCƒiƒ‹ƒ][ƒ“‚Ìê‡ƒXƒs[ƒh’²®j
	Float interval = GMD_GMK_BOSS3_PILLAR_SIZE/GMD_GMK_BOSS3_PILLAR_SPEED_NORMAL;
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL ){
		interval /= GMD_GMK_BOSS3_PILLAR_SPEED_ADJUST;
	}
	return (s32)(g_gm_gmk_boss3_pillar_frame_change_hurry[pattern_no]*interval + GMD_GMK_BOSS3_PILLAR_FRAME_HURRY);
}





//----------------------------------------------------------------
//•Ç
//----------------------------------------------------------------

// ==========================================================================
// GmGmkBoss3PillarWallChangeModeActive
/*!
 *	ƒAƒNƒeƒBƒu‚É•ÏX@ƒ{ƒX3•Ç
 *
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param pattern_no	[in] ƒpƒ^[ƒ“”Ô†
 */
// ==========================================================================
void GmGmkBoss3PillarWallChangeModeActive( OBS_OBJECT_WORK* obj_work )
{
	GMS_GMK_BOSS3_PILLAR_MANAGER_WORK* manager_work = (GMS_GMK_BOSS3_PILLAR_MANAGER_WORK*)obj_work;

	for ( s32 i = 0; GMD_GMK_BOSS3_PILLAR_WALL_TYPE_NUM > i; ++i ){
		OBS_OBJECT_WORK* obj_work_wall = manager_work->obj_work_wall[i];

		//ƒqƒbƒgƒGƒtƒFƒNƒgƒtƒ‰ƒOİ’è
		obj_work_wall->user_flag |= GMD_GMK_BOSS3_PILLAR_FLAG_HIT_EFFECT;

		//SEƒtƒ‰ƒOİ’è
		obj_work_wall->user_flag |= GMD_GMK_BOSS3_PILLAR_FLAG_SE_1;

		gmGmkBoss3PillarWallChangeModeActive( obj_work_wall );
	}
	//U“®ƒtƒ‰ƒO‚ğİ’èi¶‚Ì‚İj
	manager_work->obj_work_wall[GMD_GMK_BOSS3_PILLAR_WALL_TYPE_LEFT]->user_flag |= GMD_GMK_BOSS3_PILLAR_FLAG_CAMERA_VIBRATION | GMD_GMK_BOSS3_PILLAR_FLAG_SE_2;
#if _IPHONE
	gm_gmk_boss3_pillar_global_flag |= GMD_GMK_BOSS3_PILLAR_FLAG_SE_1 | GMD_GMK_BOSS3_PILLAR_FLAG_SE_2;
#endif // _IPHONE
}

// ==========================================================================
// GmGmkBoss3PillarWallChangeModeReturn
/*!
 *	‘Ò‹@‚É•ÏX@ƒ{ƒX3•Ç
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void GmGmkBoss3PillarWallChangeModeReturn( OBS_OBJECT_WORK* obj_work )
{
	GMS_GMK_BOSS3_PILLAR_MANAGER_WORK* manager_work = (GMS_GMK_BOSS3_PILLAR_MANAGER_WORK*)obj_work;

	//I—¹İ’è
	manager_work->gimmick_work.ene_com.eve_rec->byte_param[1] |= GMD_GMK_BOSS3_PILLAR_BYTE_PARAM_FLAG_FINISH;

	//‚à‚Ç‚·i‰E‘¤‚Ì‚İ)
	OBS_OBJECT_WORK* obj_work_wall = manager_work->obj_work_wall[GMD_GMK_BOSS3_PILLAR_WALL_TYPE_RIGHT];
	gmGmkBoss3PillarWallChangeModeReturn( obj_work_wall );
}

// ==========================================================================
// GmGmkBoss3PillarWallClearFlagNoPressDie
/*!
 *	ƒvƒŒƒCƒ„‚ğˆ³€‚³‚¹‚È‚¢ƒtƒ‰ƒO‚ğƒNƒŠƒA‚·‚éi=ˆ³€‚³‚¹‚éj
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void GmGmkBoss3PillarWallClearFlagNoPressDie( OBS_OBJECT_WORK* obj_work )
{
	GMS_GMK_BOSS3_PILLAR_MANAGER_WORK* manager_work = (GMS_GMK_BOSS3_PILLAR_MANAGER_WORK*)obj_work;

	for ( s32 i = 0; GMD_GMK_BOSS3_PILLAR_WALL_TYPE_NUM > i; ++i ){
		GMS_GMK_BOSS3_PILLAR_WALL_WORK* wall_work = (GMS_GMK_BOSS3_PILLAR_WALL_WORK*)manager_work->obj_work_wall[i];
		//ƒvƒŒƒCƒ„[‚ğˆ³€‚³‚¹‚é
		wall_work->gimmick_work.ene_com.enemy_flag &= ~GMD_ENEMY_FLAG_NOPRESSDIE;	
	}
	
}

//----- Local Functions -----------------------------------------------------

// ==========================================================================
// gmGmkBoss3PillarLoadObjNoModel
/*!
 *	ƒMƒ~ƒbƒN“Ç‚İ‚İƒ‚ƒfƒ‹‚È‚µ
 *
 *	@param eve_rec		[in] ƒCƒxƒ“ƒgƒŒƒR[ƒh
 *	@param pos_x		[in] À•WX
 *	@param pos_y		[in] À•WY
 *	@param work_size	[in] ƒ[ƒNƒTƒCƒY
 *
 *	@return ƒ[ƒN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkBoss3PillarLoadObjNoModel( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y,
									u32 work_size )
{

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec, 
			pos_x, 
			pos_y, 
			work_size, 
			"GMK_BOSS3_PILLAR");
	amAssert( gimmick_work );

	//-------------------------------------------------
	//‹éŒ`‰Šú‰»
	//-------------------------------------------------
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gimmick_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	return gimmick_work;
}

// ==========================================================================
// gmGmkBoss3PillarLoadObj
/*!
 *	ƒMƒ~ƒbƒN“Ç‚İ‚İ
 *
 *	@param eve_rec			[in] ƒCƒxƒ“ƒgƒŒƒR[ƒh
 *	@param pos_x			[in] À•WX
 *	@param pos_y			[in] À•WY
 *	@param data_work_list	[in] ƒf[ƒ^ƒ[ƒNƒŠƒXƒg
 *	@param model_id			[in] ƒ‚ƒfƒ‹‚h‚c
 *	@param work_size		[in] ƒ[ƒNƒTƒCƒY
 *
 *	@return ƒ[ƒN
 */
// ==========================================================================
GMS_ENEMY_3D_WORK* gmGmkBoss3PillarLoadObj( 
									GMS_EVE_RECORD_EVENT* eve_rec,
									fx32 pos_x, 
									fx32 pos_y, 
									OBS_ACTION3D_NN_WORK* data_work_list,
									u32 model_id,
									u32 work_size)
{

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_ENEMY_3D_WORK* gimmick_work = gmGmkBoss3PillarLoadObjNoModel(
			eve_rec,
			pos_x,
			pos_y,
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
		&data_work_list[model_id],
		&gimmick_work->obj_3d);

	return gimmick_work;
}


//----------------------------------------------------------------
//ŠÇ—
//----------------------------------------------------------------

// ==========================================================================
// gmGmkBoss3PillarManagerInit
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒ{ƒX3’ŒŠÇ—
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
void gmGmkBoss3PillarManagerInit( OBS_OBJECT_WORK* obj_work )
{
	amAssert( obj_work );

	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->move_flag |= OBD_MOVE_NOCOL;

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppMove = NULL;
	obj_work->ppOut = NULL;
#if _IPHONE
	obj_work->ppFunc = gmGmkBoss3PillarManagerMainFuncFw;
#else
	obj_work->ppFunc = NULL;
#endif // _IPHONE
}

// ==========================================================================
// gmGmkBoss3PillarManagerMainFuncWaitHurry
/*!
 *	‹}‚®ƒ‚[ƒh‘Ò‹@ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkBoss3PillarManagerMainFuncWaitHurry( OBS_OBJECT_WORK* obj_work )
{
#if _IPHONE
	gm_gmk_boss3_pillar_global_flag = 0xffffffff;
#endif // _IPHONE
	if ( obj_work->user_timer > 0 ){
		--obj_work->user_timer;
		return;
	}

	//‹}‚®ƒ‚[ƒh‚É•ÏX—v‹
	GmGmkBoss3PillarChangeModeHurry( obj_work );

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkBoss3PillarManagerMainFuncWaitHurryEnd;

	//‘Ò‚¿ŠÔ
	obj_work->user_timer = GMD_GMK_BOSS3_PILLAR_FRAME_HURRY;
}

// ==========================================================================
// gmGmkBoss3PillarManagerMainFuncWaitHurryEnd
/*!
 *	‹}‚®ƒ‚[ƒhI—¹ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkBoss3PillarManagerMainFuncWaitHurryEnd( OBS_OBJECT_WORK* obj_work )
{
#if _IPHONE
	gm_gmk_boss3_pillar_global_flag = 0xffffffff;
#endif // _IPHONE
	if ( obj_work->user_timer > 0 ){
		--obj_work->user_timer;
		return;
	}

	//ƒJƒƒ‰U“®
	GmCameraVibrationSet( 0, GMD_GMK_BOSS3_PILLAR_QUAKE_FALL, 0 );

	//ƒƒCƒ“ˆ—•ÏX
#if _IPHONE
	obj_work->ppFunc = gmGmkBoss3PillarManagerMainFuncFw;
#else
	obj_work->ppFunc = NULL;
#endif // _IPHONE
}

// ==========================================================================
// gmGmkBoss3PillarManagerMainFuncWaitReturn
/*!
 *	‘Ò‹@ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkBoss3PillarManagerMainFuncWaitReturn( OBS_OBJECT_WORK* obj_work )
{
#if _IPHONE
	gm_gmk_boss3_pillar_global_flag = 0xffffffff;
#endif // _IPHONE
	if ( obj_work->user_timer > 0 ){
		--obj_work->user_timer;
		return;
	}

	//íœƒ‚[ƒh‚É•ÏX—v‹
	GmGmkBoss3PillarChangeModeDelete( obj_work );

	//ƒƒCƒ“ˆ—•ÏX
#if _IPHONE
	obj_work->ppFunc = gmGmkBoss3PillarManagerMainFuncFw;
#else
	obj_work->ppFunc = NULL;
#endif // _IPHONE
}

#if _IPHONE
// ==========================================================================
// gmGmkBoss3PillarManagerMainFuncFw
/*!
 *	‘Ò‹@ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
static void gmGmkBoss3PillarManagerMainFuncFw( OBS_OBJECT_WORK* obj_work )
{
#if _IPHONE
	gm_gmk_boss3_pillar_global_flag = 0xffffffff;
#endif // _IPHONE
}
#endif // _IPHONE

//----------------------------------------------------------------
//ƒp[ƒc
//----------------------------------------------------------------

// ==========================================================================
// gmGmkBoss3PillarCalcPillarType
/*!
 *	ƒMƒ~ƒbƒN@ƒCƒxƒ“ƒgID‚©‚ç’Œƒ^ƒCƒv‚ğZo
 *
 *	@param event_id	[in] ƒCƒxƒ“ƒgID
 *
 *	@return ’Œƒ^ƒCƒv
 */
// ==========================================================================
GME_GMK_BOSS3_PILLAR_TYPE gmGmkBoss3PillarCalcPillarType( s32 event_id )
{
	amAssert( GMD_EVENT_ID_BOSS3_PILLAR_LEFT <= event_id && event_id <= GMD_EVENT_ID_BOSS3_PILLAR_BOTTOM );

	GME_GMK_BOSS3_PILLAR_TYPE pillar_type = (GME_GMK_BOSS3_PILLAR_TYPE)(event_id - GMD_EVENT_ID_BOSS3_PILLAR_LEFT);
	return pillar_type;
}

// ==========================================================================
// gmGmkBoss3PillarSetFieldRect
/*!
 *	ƒMƒ~ƒbƒN@ƒtƒB[ƒ‹ƒh‹éŒ`İ’è
 *
 *	@param field_obj	[io] ’nŒ`ƒ[ƒN
 *	@param pillar_type	[in] ’Œƒ^ƒCƒv
 */
// ==========================================================================
void gmGmkBoss3PillarSetFieldRect( 
								  OBS_COLLISION_OBJ* field_obj, 
								  GME_GMK_BOSS3_PILLAR_TYPE pillar_type )
{	
	amAssert( field_obj );
	amAssert( GMD_GMK_BOSS3_PILLAR_TYPE_NUM > pillar_type );

	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];

	if ( zone_type == GSD_MAIN_ZONE_TYPE_3 ){
		field_obj->width = (u16)g_gm_boss3_pillar_field_rect_wh[pillar_type][MTD_WIDTH];
		field_obj->height = (u16)g_gm_boss3_pillar_field_rect_wh[pillar_type][MTD_HEIGHT];
		field_obj->ofst_x = (s16)g_gm_boss3_pillar_field_rect_offset[pillar_type][MTD_X];
		field_obj->ofst_y = (s16)g_gm_boss3_pillar_field_rect_offset[pillar_type][MTD_Y];
	}
	else if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL ){
		field_obj->width = (u16)g_gm_boss3_pillar_f_field_rect_wh[pillar_type][MTD_WIDTH];
		field_obj->height = (u16)g_gm_boss3_pillar_f_field_rect_wh[pillar_type][MTD_HEIGHT];
		field_obj->ofst_x = (s16)g_gm_boss3_pillar_f_field_rect_offset[pillar_type][MTD_X];
		field_obj->ofst_y = (s16)g_gm_boss3_pillar_f_field_rect_offset[pillar_type][MTD_Y];
	}
	else{
		amAssert(FALSE);
	}
}

// ==========================================================================
// gmGmkBoss3PillarSetMove
/*!
 *	ƒMƒ~ƒbƒN@ƒAƒNƒeƒBƒu‚ÌˆÚ“®‚ğİ’è
 *
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param pillar_type	[in] ’Œƒ^ƒCƒv
 *	@param speed		[in] ˆÚ“®ƒXƒs[ƒh
 */
// ==========================================================================
void gmGmkBoss3PillarSetMoveSpeed( 
							 OBS_OBJECT_WORK* obj_work, 
							 GME_GMK_BOSS3_PILLAR_TYPE pillar_type,
							 fx32 speed)
{	
	amAssert( obj_work );
	amAssert( GMD_GMK_BOSS3_PILLAR_TYPE_NUM > pillar_type );

	//ˆÚ“®’l
	fx32 speed_x = FX_Mul( speed, g_gm_boss3_pillar_adjust_dir[pillar_type][MTD_X]*FX32_ONE );
	fx32 speed_y = FX_Mul( speed, g_gm_boss3_pillar_adjust_dir[pillar_type][MTD_Y]*FX32_ONE );

	obj_work->spd.x = speed_x;
	obj_work->spd.y = speed_y;
}

// ==========================================================================
// gmGmkBoss3PillarSetMoveTarget
/*!
 *	ƒMƒ~ƒbƒN@ƒAƒNƒeƒBƒu‚ÌˆÚ“®‚ğİ’è
 *
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param pillar_type	[in] ’Œƒ^ƒCƒv
 *	@param distance		[in] ƒfƒtƒHƒ‹ƒgÀ•W‚©‚ç‚ÌˆÚ“®‹——£
 *	@param speed		[in] ˆÚ“®ƒXƒs[ƒh
 */
// ==========================================================================
void gmGmkBoss3PillarSetMoveTarget( 
							 OBS_OBJECT_WORK* obj_work, 
							 GME_GMK_BOSS3_PILLAR_TYPE pillar_type,
							 fx32 distance)
{	
	GMS_GMK_BOSS3_PILLAR_MAIN_WORK* main_work = (GMS_GMK_BOSS3_PILLAR_MAIN_WORK*)obj_work;
	amAssert( main_work );
	amAssert( GMD_GMK_BOSS3_PILLAR_TYPE_NUM > pillar_type );

	//–Ú•W
	fx32 distance_x = FX_Mul( distance, g_gm_boss3_pillar_adjust_dir[pillar_type][MTD_X]*FX32_ONE );
	fx32 distance_y = FX_Mul( distance, g_gm_boss3_pillar_adjust_dir[pillar_type][MTD_Y]*FX32_ONE );
	
	main_work->target_pos.x = main_work->default_pos.x + distance_x;
	main_work->target_pos.y = main_work->default_pos.y + distance_y;
}

// ==========================================================================
// gmGmkBoss3PillarSetMoveHurry
/*!
 *	ƒMƒ~ƒbƒN@İ’è‚³‚ê‚Ä‚¢‚é–Ú“I’n‚ÉAw’è‚³‚ê‚½ƒtƒŒ[ƒ€‚Å“’…‚·‚éˆÚ“®’l‚ğİ’è‚·‚é
 *
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param move_frame	[in] “’…‚Ü‚Å‚ÌƒtƒŒ[ƒ€”
 */
// ==========================================================================
void gmGmkBoss3PillarSetMoveHurry( 
							 OBS_OBJECT_WORK* obj_work, 
							 s32 frame)
{	
	GMS_GMK_BOSS3_PILLAR_MAIN_WORK* main_work = (GMS_GMK_BOSS3_PILLAR_MAIN_WORK*)obj_work;
	amAssert( main_work );

	//0ƒtƒŒ[ƒ€w’è‚Ìê‡A‘¦“’…
	if ( frame <= 0 ){
		obj_work->pos = main_work->target_pos;
		obj_work->spd.x = 0;
		obj_work->spd.y = 0;
		return;
	}
	
	//ˆÚ“®’l
	fx32 distance_x = main_work->target_pos.x - obj_work->pos.x;
	fx32 distance_y = main_work->target_pos.y - obj_work->pos.y;
	fx32 speed_x = FX_Div(distance_x, frame*FX32_ONE);
	fx32 speed_y = FX_Div(distance_y, frame*FX32_ONE);
	obj_work->spd.x = speed_x;
	obj_work->spd.y = speed_y;
}

// ==========================================================================
// gmGmkBoss3PillarSetMove
/*!
 *	ƒMƒ~ƒbƒN@ƒAƒNƒeƒBƒu‚ÌˆÚ“®‚ğİ’è
 *
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param pillar_type	[in] ’Œƒ^ƒCƒv
 *
 *	@retval TRUE:ˆÚ“®I—¹
 *	@retval FALSE:ˆÚ“®’†
 */
// ==========================================================================
BOOL gmGmkBoss3PillarCheckMoveEnd( 
								  OBS_OBJECT_WORK* obj_work,
								  GME_GMK_BOSS3_PILLAR_TYPE pillar_type)
{	
	UNREFERENCED_PARAMETER( pillar_type );

	GMS_GMK_BOSS3_PILLAR_MAIN_WORK* main_work = (GMS_GMK_BOSS3_PILLAR_MAIN_WORK*)obj_work;
	amAssert( main_work );
	amAssert( GMD_GMK_BOSS3_PILLAR_TYPE_NUM > pillar_type );


	BOOL result = FALSE;

	//ˆÚ“®•ûŒü
	fx32 speed_x = obj_work->spd.x;
	fx32 speed_y = obj_work->spd.y;

	//ˆÚ“®‚ªİ’è‚ª‚³‚ê‚Ä‚¢‚È‚¢ê‡AI—¹ˆµ‚¢
	if ( speed_x == 0 && speed_y == 0 ){
		result = TRUE;
	}
	//ˆÚ“®‚ªİ’è‚³‚ê‚Ä‚¢‚é
	else{	
		fx32 distance_x = main_work->target_pos.x - obj_work->pos.x;
		fx32 distance_y = main_work->target_pos.y - obj_work->pos.y;
		
		if ( speed_x < 0 && distance_x >= 0
				|| speed_x > 0 && distance_x <= 0
				|| speed_y < 0 && distance_y >= 0
				|| speed_y > 0 && distance_y <= 0
		){
			result = TRUE;
		}
	}

	return result;
}




// ==========================================================================
//ƒƒCƒ“ƒp[ƒc
// ==========================================================================

// ==========================================================================
// gmBoss3PillarDestFunc
/*!
 *	I—¹ŠÖ”
 *
 *	@param tcb	[in] TCB
 */
// ==========================================================================
void gmBoss3PillarDestFunc( MTS_TASK_TCB *tcb )
{
	GMS_GMK_BOSS3_PILLAR_MAIN_WORK* pillar_work = (GMS_GMK_BOSS3_PILLAR_MAIN_WORK*)mtTaskGetTcbWork( tcb );

	//ƒGƒtƒFƒNƒgíœ—v‹
	if ( pillar_work->effect_work ){
		ObjDrawKillAction3DES( (OBS_OBJECT_WORK*)pillar_work->effect_work );
		pillar_work->effect_work = NULL;
	}

	//Œø‰Ê‰¹’â~
	if ( pillar_work->se_handle ){
		GmSoundStopSE( pillar_work->se_handle );
	}

	//U“®’â~
	GMM_PAD_VIB_STOP();
#if !_IPHONE
	//ƒ‚[ƒVƒ‡ƒ“‰ğ•ú
	for ( s32 i = 1; GMD_GMK_BOSS3_PILLAR_PARTS_NUM_MAX > i; ++i ){
		ObjAction3dNNMotionRelease(	&pillar_work->obj_3d_parts[i-1] );
	}
#endif // !_IPHONE

	//Œø‰Ê‰¹ƒnƒ“ƒhƒ‹‰ğ•ú
	if ( pillar_work->se_handle ){
#if 0 //_IPHONE
		gmGmkBoss3PillarFreeHandle(pillar_work->se_handle);
#else
		GmSoundStopSE( pillar_work->se_handle );
		GsSoundFreeSeHandle( pillar_work->se_handle );
#endif // _IPHONE
		pillar_work->se_handle = NULL;
	}

	//•W€I—¹
	GmEnemyDefaultExit( tcb );
}

// =======================================================================
// gmBoss3PillarOutFunc
/*!
 *	•`‰æŠÖ”
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3PillarOutFunc( OBS_OBJECT_WORK* obj_work )
{	
	amAssert( obj_work );
#if _IPHONE
	if (!GmMainIsDrawEnable()) {
		return;
	}
#endif // _IPHONE
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX
	GMS_GMK_BOSS3_PILLAR_MAIN_WORK* pillar_work = (GMS_GMK_BOSS3_PILLAR_MAIN_WORK*)obj_work;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;

	//’Œƒ^ƒCƒvæ“¾
	GME_GMK_BOSS3_PILLAR_TYPE pillar_type = gmGmkBoss3PillarCalcPillarType( gimmick_work->ene_com.eve_rec->id );

	//ƒ][ƒ“•Êİ’è
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	s32 parts_num = 0;
	const s32* parts_offset_list = NULL;
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
	const s32* model_id_list = NULL;
	s32 sub_model_id;
	AMS_AMB_HEADER* amb_header = g_gm_gmk_boss3_pillar_obj_tvx_list;
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX
	if ( zone_type == GSD_MAIN_ZONE_TYPE_3 ){
		parts_num = g_gm_gmk_boss3_pillar_parts_num[pillar_type];
		parts_offset_list = g_gm_boss3_pillar_parts_offset[pillar_type];
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
		model_id_list = g_gm_boss3_pillar_model_id[pillar_type];
		sub_model_id = -1;
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX
	}
	else if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL ){
		parts_num = g_gm_gmk_boss3_pillar_f_parts_num[pillar_type];
		parts_offset_list = g_gm_boss3_pillar_f_parts_offset[pillar_type];
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
		model_id_list = g_gm_boss3_pillar_f_model_id[pillar_type];
		sub_model_id  = g_gm_boss3_pillar_f_sub_model_id[pillar_type];
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX
	}
	else{
		amAssert( FALSE );
	}
	
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
	u32 disp_flag = GMD_TVX_DISP_SCALE; // ƒXƒP[ƒ‹—LŒø
	VecFx32 scale;
	scale.x = FX_F32_TO_FX32(GMD_GMK_BOSS3_PILLAR_SCALE);
	scale.y = FX_F32_TO_FX32(GMD_GMK_BOSS3_PILLAR_SCALE);
	scale.z = obj_work->scale.z;
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		scale.x *= -1;
	}
	else if (obj_work->disp_flag & OBD_DISP_VFLIP) {
		scale.y *= -1;
	}
	GmTvxSetModel(amBindGet(amb_header, model_id_list[0]), obj_work->obj_3d->texlist, &obj_work->pos, &scale, disp_flag, 0);
#else
	//ƒXƒP[ƒ‹’²®
	fx32 scale_x = obj_work->scale.x;
	fx32 scale_y = obj_work->scale.y;
	obj_work->scale.x = FX_F32_TO_FX32(GMD_GMK_BOSS3_PILLAR_SCALE);
	obj_work->scale.y = FX_F32_TO_FX32(GMD_GMK_BOSS3_PILLAR_SCALE);
		
	//æ“ª•`‰æ
	ObjDrawActionSummary( obj_work );
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX

	//ƒp[ƒc•`‰æ
	for ( s32 i = 1; parts_num > i; ++i ){

		//eƒIƒtƒZƒbƒg
		s32 offset_x = -parts_offset_list[i];
		s32 offset_y = -parts_offset_list[i];

		offset_x *= g_gm_boss3_pillar_adjust_dir[pillar_type][MTD_X] * GMD_GMK_BOSS3_PILLAR_SIZE;
		offset_y *= g_gm_boss3_pillar_adjust_dir[pillar_type][MTD_Y] * GMD_GMK_BOSS3_PILLAR_SIZE;

		VecFx32 pos = obj_work->pos;
		pos.x += (fx32)offset_x * FX32_ONE;
		pos.y += (fx32)offset_y * FX32_ONE;
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
		GmTvxSetModel(amBindGet(amb_header, model_id_list[i]), obj_work->obj_3d->texlist, &pos, &scale, disp_flag, 0);
//		if (sub_model_id != -1) {
//			GmTvxSetModel(amBindGet(amb_header, sub_model_id), obj_work->obj_3d->texlist, &pos, &scale, disp_flag, 0);
//		}
#else
		u32 disp_flag = obj_work->disp_flag | OBD_DISP_REPEAT;
	
		//ƒ}ƒeƒŠƒAƒ‹
		if ( ObjObjectPauseCheck(0) ){
			disp_flag |= OBD_DISP_NOUPDATE;
		}

		//•`‰æ
		ObjDrawAction3DNN(
				&pillar_work->obj_3d_parts[i-1],
				&pos, 
				&obj_work->dir, 
				&obj_work->scale, 
				&disp_flag );
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX
	}
#if !GMD_GMK_BOSS3_PILLAR_TEST_TVX
	obj_work->scale.x = scale_x;
	obj_work->scale.y = scale_y;
#endif // !GMD_GMK_BOSS3_PILLAR_TEST_TVX
}

// ==========================================================================
// gmGmkBoss3PillarPartsInitMain
/*!
 *	ƒMƒ~ƒbƒN@ƒ{ƒX3’ŒƒƒCƒ“ƒp[ƒc‰Šú‰»
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param pillar_type	[in] ’Œƒ^ƒCƒv
 */
// ==========================================================================
void gmGmkBoss3PillarPartsInitMain( OBS_OBJECT_WORK* obj_work, GME_GMK_BOSS3_PILLAR_TYPE pillar_type )
{	
	GMS_GMK_BOSS3_PILLAR_MAIN_WORK* pillar_work = (GMS_GMK_BOSS3_PILLAR_MAIN_WORK*)obj_work;
	amAssert( pillar_work );
	amAssert( GMD_GMK_BOSS3_PILLAR_TYPE_NUM > pillar_type );

	//-------------------------------------------------
	//‹éŒ`
	//-------------------------------------------------
	//’nŒ`•Ó‚è
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );
	gimmick_work->ene_com.col_work.obj_col.obj = &gimmick_work->ene_com.obj_work;
	gmGmkBoss3PillarSetFieldRect( &gimmick_work->ene_com.col_work.obj_col, pillar_type );

	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->disp_flag |= OBD_DISP_DIR2DFLIP;
	obj_work->flag |= OBD_OBJECT_NOCLIP;
	obj_work->move_flag |= OBD_MOVE_NOCOLFIELD | OBD_MOVE_JUMP;
	obj_work->move_flag &= ~OBD_MOVE_FALL;
	obj_work->move_flag |= OBD_MOVE_NOCOL;

	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	if ( zone_type == GSD_MAIN_ZONE_TYPE_3 ){
		//”½“]
		if ( pillar_type == GMD_GMK_BOSS3_PILLAR_TYPE_LEFT ){
			obj_work->disp_flag |= OBD_DISP_HFLIP;
		}
		else if ( pillar_type == GMD_GMK_BOSS3_PILLAR_TYPE_TOP ){
			obj_work->disp_flag |= OBD_DISP_VFLIP;
		}
		gimmick_work->obj_3d.drawflag |= NND_DRAWOBJ_DOUBLESIDE;

		//À•W
		obj_work->pos.x += FX_F32_TO_FX32( g_gm_gmk_boss3_pillar_adjust_default_pos[pillar_type][MTD_X] );
		obj_work->pos.y += FX_F32_TO_FX32( g_gm_gmk_boss3_pillar_adjust_default_pos[pillar_type][MTD_Y] );
	}
	else if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL ){
		//À•W
		obj_work->pos.x += FX_F32_TO_FX32( g_gm_gmk_boss3_pillar_f_adjust_default_pos[pillar_type][MTD_X] );
		obj_work->pos.y += FX_F32_TO_FX32( g_gm_gmk_boss3_pillar_f_adjust_default_pos[pillar_type][MTD_Y] );
	}
	else{
		amAssert(FALSE);
	}
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_B_BACK;

	//ƒfƒtƒHƒ‹ƒgÀ•W
	pillar_work->default_pos = obj_work->pos;

	//ƒ^[ƒQƒbƒgÀ•W
	pillar_work->target_pos = pillar_work->default_pos;

	//SEƒnƒ“ƒhƒ‹Šm•Û
#if 0 //_IPHONE
	pillar_work->se_handle = gmGmkBoss3PillarGetSeHandle();
#else
	pillar_work->se_handle = GsSoundAllocSeHandle();
#endif // _IPHONE

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	//‘Ò‹@ó‘Ô‚Ö
	gmGmkBoss3PillarPartsChangeModeWait( obj_work );
	obj_work->ppOut = gmBoss3PillarOutFunc;
	mtTaskChangeTcbDestructor( obj_work->tcb, gmBoss3PillarDestFunc );
}


// ==========================================================================
// gmGmkBoss3PillarPartsChangeModeWait
/*!
 *	‘Ò‹@ó‘Ô‚Ö
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkBoss3PillarPartsChangeModeWait( OBS_OBJECT_WORK* obj_work )
{	
	GMS_GMK_BOSS3_PILLAR_MAIN_WORK* main_work = (GMS_GMK_BOSS3_PILLAR_MAIN_WORK*)obj_work;
	obj_work->pos = main_work->target_pos;
	obj_work->spd.x = 0;
	obj_work->spd.y = 0;

	//‘Ò‹@ó‘Ô‚Ö
	obj_work->ppFunc = gmGmkBoss3PillarPartsMainWait;

	//ƒGƒtƒFƒNƒgíœ
	if ( main_work->effect_work ){
		ObjDrawKillAction3DES( (OBS_OBJECT_WORK*)main_work->effect_work );
		main_work->effect_work = NULL;
	}
}

// ==========================================================================
// gmGmkBoss3PillarPartsChangeModeNormal
/*!
 *	ƒAƒNƒeƒBƒuó‘Ô‚Ö
 *
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param distance		[in] ƒfƒtƒHƒ‹ƒgÀ•W‚©‚ç‚ÌˆÚ“®‹——£
 *	@param wait_time	[in] ‘Ò‚¿ŠÔ
 */
// ==========================================================================
void gmGmkBoss3PillarPartsChangeModeNormal( OBS_OBJECT_WORK* obj_work, fx32 distance, s32 wait_time )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//’Œƒ^ƒCƒvæ“¾
	GME_GMK_BOSS3_PILLAR_TYPE pillar_type = gmGmkBoss3PillarCalcPillarType( gimmick_work->ene_com.eve_rec->id );

	//ˆÚ“®æİ’è
	gmGmkBoss3PillarSetMoveTarget( obj_work, pillar_type, distance );

	//ƒXƒs[ƒhİ’è
	Float speed = GMD_GMK_BOSS3_PILLAR_SPEED_NORMAL;
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL ){
		speed *= GMD_GMK_BOSS3_PILLAR_SPEED_ADJUST;
	}
	gmGmkBoss3PillarSetMoveSpeed( obj_work, pillar_type, FX_F32_TO_FX32(speed) );

	//SEİ’è
	obj_work->user_flag |= GMD_GMK_BOSS3_PILLAR_FLAG_SE_1;
#if _IPHONE
	gm_gmk_boss3_pillar_global_flag |= GMD_GMK_BOSS3_PILLAR_FLAG_SE_1;
#endif // _IPHONE

	//‘Ò‚¿ŠÔ
	obj_work->user_timer = wait_time;

	//ˆÚ“®‘Ò‹@
	obj_work->move_flag |= OBD_MOVE_NOMOVE;

	//€”õó‘Ô‚Ö
	obj_work->ppFunc = gmGmkBoss3PillarPartsMainReady;
}

// ==========================================================================
// gmGmkBoss3PillarPartsChangeModeHurry
/*!
 *	‹}‚®ó‘Ô‚Öiİ’è‚³‚ê‚Ä‚¢‚é–Ú“I’n‚ÉAw’è‚³‚ê‚½ƒtƒŒ[ƒ€‚Å“’…‚·‚éˆÚ“®’l‚ğİ’è‚·‚éj
 *
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param move_frame	[in] “’…‚Ü‚Å‚ÌƒtƒŒ[ƒ€”
 */
// ==========================================================================
void gmGmkBoss3PillarPartsChangeModeHurry( OBS_OBJECT_WORK* obj_work, s32 move_frame )
{
	//ƒXƒs[ƒhİ’è
	gmGmkBoss3PillarSetMoveHurry( obj_work, move_frame );

	//‘Ò‚¿ŠÔ
	obj_work->user_timer = 0;

	//ˆÚ“®ŠJn
	obj_work->move_flag &= ~OBD_MOVE_NOMOVE;

	//€”õó‘Ô‚Ö
	obj_work->ppFunc = gmGmkBoss3PillarPartsMainActive;
}

// ==========================================================================
// gmGmkBoss3PillarPartsChangeModeReturn
/*!
 *	ˆø‚Á‚Şó‘Ô‚Ö
 
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param wait_time	[in] ‘Ò‚¿ŠÔ
 *	@param move_frame	[in] “’…‚Ü‚Å‚ÌƒtƒŒ[ƒ€”
 */
// ==========================================================================
void gmGmkBoss3PillarPartsChangeModeReturn( OBS_OBJECT_WORK* obj_work, s32 wait_time, s32 move_frame )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//’Œƒ^ƒCƒvæ“¾
	GME_GMK_BOSS3_PILLAR_TYPE pillar_type = gmGmkBoss3PillarCalcPillarType( gimmick_work->ene_com.eve_rec->id );

	//ˆÚ“®æİ’è
	gmGmkBoss3PillarSetMoveTarget( obj_work, pillar_type, 0 );

	//ƒXƒs[ƒhİ’è
	gmGmkBoss3PillarSetMoveHurry( obj_work, move_frame );

	//ƒGƒtƒFƒNƒgİ’è
	obj_work->user_flag &= ~GMD_GMK_BOSS3_PILLAR_FLAG_HIT_EFFECT;

	//SEİ’è
	obj_work->user_flag |= GMD_GMK_BOSS3_PILLAR_FLAG_SE_1;
	obj_work->user_flag &= ~GMD_GMK_BOSS3_PILLAR_FLAG_SE_2;
#if _IPHONE
	gm_gmk_boss3_pillar_global_flag |= GMD_GMK_BOSS3_PILLAR_FLAG_SE_1;
	gm_gmk_boss3_pillar_global_flag &= ~GMD_GMK_BOSS3_PILLAR_FLAG_SE_2;
#endif // _IPHONE

	//‘Ò‚¿ŠÔ
	obj_work->user_timer = wait_time;

	//ˆÚ“®‘Ò‹@
	obj_work->move_flag |= OBD_MOVE_NOMOVE;

	//€”õó‘Ô‚Ö
	obj_work->ppFunc = gmGmkBoss3PillarPartsMainReady;
}

// ==========================================================================
// gmGmkBoss3PillarPartsMainWait
/*!
 *	‘Ò‹@ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkBoss3PillarPartsMainWait( OBS_OBJECT_WORK* obj_work )
{
	UNREFERENCED_PARAMETER( obj_work );
}

// ==========================================================================
// gmGmkBoss3PillarPartsMainReady
/*!
 *	‘Ò‹@ó‘Ô
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkBoss3PillarPartsMainReady( OBS_OBJECT_WORK* obj_work )
{
	//‘Ò‚¿ŠÔ
	if ( obj_work->user_timer > 0 ){
		--obj_work->user_timer;
		return;
	}
	//ˆÚ“®ŠJn
	obj_work->move_flag &= ~OBD_MOVE_NOMOVE;

	//ƒAƒNƒeƒBƒu‚É
	obj_work->ppFunc = gmGmkBoss3PillarPartsMainActive;

	//ƒGƒtƒFƒNƒg¶¬
	gmGmkBoss3PillarEffectCreatePillarAppear( obj_work);

#if _IPHONE
	// ƒGƒtƒFƒNƒg¶‘¶ŠÔİ’è
	obj_work->user_timer = GMD_GMK_BOSS3_PILLAR_TIME_EFFECT_APPEAR;
#endif // _IPHONE
#if 1 //!_IPHONE
	//Œø‰Ê‰¹i’ŒUŒ‚‚ä‚Á‚­‚èSEj
#if _IPHONE
	if ( obj_work->user_flag & gm_gmk_boss3_pillar_global_flag & GMD_GMK_BOSS3_PILLAR_FLAG_SE_1 ){
#else
	if ( obj_work->user_flag & GMD_GMK_BOSS3_PILLAR_FLAG_SE_1){
#endif // _IPHONE
		GMS_GMK_BOSS3_PILLAR_MAIN_WORK* pillar_work = (GMS_GMK_BOSS3_PILLAR_MAIN_WORK*)obj_work;
		if ( pillar_work->se_handle ){
			GmSoundPlaySE("Boss3_01", pillar_work->se_handle );
		}
		obj_work->user_flag &= ~GMD_GMK_BOSS3_PILLAR_FLAG_SE_1;
#if _IPHONE
		gm_gmk_boss3_pillar_global_flag &= ~GMD_GMK_BOSS3_PILLAR_FLAG_SE_1;
#endif // _IPHONE

		//U“®
		GMM_PAD_VIB_SMALL_NOEND();
	}
#endif // !_IPHONE
}

// ==========================================================================
// gmGmkBoss3PillarPartsMainActive
/*!
 *	ƒAƒNƒeƒBƒuó‘Ô
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkBoss3PillarPartsMainActive( OBS_OBJECT_WORK* obj_work )
{
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( gimmick_work );

	//’Œƒ^ƒCƒvæ“¾
	GME_GMK_BOSS3_PILLAR_TYPE pillar_type = gmGmkBoss3PillarCalcPillarType( gimmick_work->ene_com.eve_rec->id );

#if _IPHONE
	// ƒGƒtƒFƒNƒgÁ‹ƒ^ƒCƒ~ƒ“ƒO
	if (--obj_work->user_timer == 0) {
		GMS_GMK_BOSS3_PILLAR_MAIN_WORK* main_work = (GMS_GMK_BOSS3_PILLAR_MAIN_WORK*)obj_work;
		if ( main_work->effect_work ){
			ObjDrawKillAction3DES( (OBS_OBJECT_WORK*)main_work->effect_work );
			main_work->effect_work = NULL;
		}
	}
#endif // _IPHONE

	//I—¹‘Ò‚¿
	if ( !gmGmkBoss3PillarCheckMoveEnd(obj_work, pillar_type) ){
		return;
	}

	//ƒqƒbƒgƒGƒtƒFƒNƒg
	if ( obj_work->user_flag & GMD_GMK_BOSS3_PILLAR_FLAG_HIT_EFFECT ){
		gmGmkBoss3PillarEffectCreatePillarHit( obj_work );
		obj_work->user_flag &= ~GMD_GMK_BOSS3_PILLAR_FLAG_HIT_EFFECT;

		//U“®’â~
		GMM_PAD_VIB_STOP();
	}
#if 1 //!_IPHONE
	//Œø‰Ê‰¹i’ŒUŒ‚ƒXƒs[ƒhƒAƒbƒvSEj
	GMS_GMK_BOSS3_PILLAR_MAIN_WORK* pillar_work = (GMS_GMK_BOSS3_PILLAR_MAIN_WORK*)obj_work;
	if ( pillar_work->se_handle ){
		GmSoundStopSE( pillar_work->se_handle );
#if _IPHONE
		if ( obj_work->user_flag & gm_gmk_boss3_pillar_global_flag & GMD_GMK_BOSS3_PILLAR_FLAG_SE_2 ){
#else
		if ( obj_work->user_flag & GMD_GMK_BOSS3_PILLAR_FLAG_SE_2 ){
#endif // _IPHONE
			GmSoundPlaySE("Boss3_02", pillar_work->se_handle);
			obj_work->user_flag &= ~GMD_GMK_BOSS3_PILLAR_FLAG_SE_2;
#if _IPHONE
			gm_gmk_boss3_pillar_global_flag &= ~GMD_GMK_BOSS3_PILLAR_FLAG_SE_2;
#endif // _IPHONE
		}
	}
#endif // !_IPHONE
	//‘Ò‹@ó‘Ô‚Ö
	gmGmkBoss3PillarPartsChangeModeWait(obj_work);
}


#if _IPHONE
static GSS_SND_SE_HANDLE* gmGmkBoss3PillarGetSeHandle(void)
{
	GSS_SND_SE_HANDLE* se_handle = NULL;
	
	// ‚Ğ‚Æ‚Â‚àg—p‚³‚ê‚Ä‚¢‚È‚¯‚ê‚ÎŠm•Û
	if (gm_gmk_boss3_pillar_se_use_count <= 0) {
		se_handle = GsSoundAllocSeHandle();
		gm_gmk_boss3_pillar_se_use_count++;
	}
	
	return se_handle;
}

static void gmGmkBoss3PillarFreeHandle(GSS_SND_SE_HANDLE* se_handle)
{
	//	À‘Ì‚ª‚ ‚ê‚ÎŠJ•ú
	if (se_handle) {
		GmSoundStopSE( se_handle );
		GsSoundFreeSeHandle( se_handle );
		gm_gmk_boss3_pillar_se_use_count--;
	}
}
#endif // _IPHONE




//----------------------------------------------------------------
//•Ç
//----------------------------------------------------------------

// ==========================================================================
// gmGmkBoss3PillarWallInit
/*!
 *	ƒMƒ~ƒbƒN@ƒ{ƒX3•Ç‰Šú‰»
 *
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkBoss3PillarWallInit( OBS_OBJECT_WORK* obj_work )
{	
	GMS_GMK_BOSS3_PILLAR_WALL_WORK* wall_work = (GMS_GMK_BOSS3_PILLAR_WALL_WORK*)obj_work;
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	amAssert( wall_work );

	//-------------------------------------------------
	//‹éŒ`
	//-------------------------------------------------
	//’nŒ`•Ó‚è
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	if ( zone_type == GSD_MAIN_ZONE_TYPE_3 ){
		gimmick_work->ene_com.col_work.obj_col.obj = &gimmick_work->ene_com.obj_work;
		gimmick_work->ene_com.col_work.obj_col.width = GMD_GMK_BOSS3_PILLAR_WALL_SIZE;
		gimmick_work->ene_com.col_work.obj_col.height = GMD_GMK_BOSS3_PILLAR_WALL_HEIGHT*GMD_GMK_BOSS3_PILLAR_WALL_SIZE;
		gimmick_work->ene_com.col_work.obj_col.ofst_x = -GMD_GMK_BOSS3_PILLAR_WALL_SIZE/2;
		gimmick_work->ene_com.col_work.obj_col.ofst_y = -GMD_GMK_BOSS3_PILLAR_WALL_SIZE/2;
	}
	else if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL ){
		gimmick_work->ene_com.col_work.obj_col.obj = &gimmick_work->ene_com.obj_work;
		gimmick_work->ene_com.col_work.obj_col.width = GMD_GMK_BOSS3_PILLAR_WALL_SIZE;
		gimmick_work->ene_com.col_work.obj_col.height = GMD_GMK_BOSS3_PILLAR_WALL_HEIGHT_F*GMD_GMK_BOSS3_PILLAR_WALL_SIZE_F;
		gimmick_work->ene_com.col_work.obj_col.ofst_x = -GMD_GMK_BOSS3_PILLAR_WALL_SIZE/2;
		gimmick_work->ene_com.col_work.obj_col.ofst_y = 0;
	}
	else{
		amAssert( FALSE );
	}

	//-------------------------------------------------
	// ƒ[ƒNİ’è
	//-------------------------------------------------
	//ƒtƒ‰ƒO
	obj_work->disp_flag |= OBD_DISP_DIR2DFLIP;
	obj_work->flag |= OBD_OBJECT_NOCLIP;
	obj_work->move_flag |= OBD_MOVE_NOCOL | OBD_MOVE_JUMP;
	obj_work->move_flag &= ~OBD_MOVE_FALL;

	//ƒvƒŒƒCƒ„[‚ğˆ³€‚³‚¹‚È‚¢
	wall_work->gimmick_work.ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOPRESSDIE;	

	//À•W
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_B_FRONT;

	//ƒfƒtƒHƒ‹ƒgÀ•W•Û‘¶
	wall_work->default_pos = obj_work->pos;

	//ƒ^[ƒQƒbƒgÀ•W
	wall_work->target_pos = wall_work->default_pos;

	//SEƒnƒ“ƒhƒ‹Šm•Û
	wall_work->se_handle = GsSoundAllocSeHandle();

	//ƒ†[ƒUƒtƒ‰ƒO
	obj_work->user_flag |= GMD_GMK_BOSS3_PILLAR_FLAG_DRAW_WALL_BACK;

	//-------------------------------------------------
	// ƒƒCƒ“ˆ—
	//-------------------------------------------------
	obj_work->ppFunc = gmGmkBoss3PillarWallMainWait;
	mtTaskChangeTcbDestructor( obj_work->tcb, gmBoss3PillarWallDestFunc );

	if ( zone_type == GSD_MAIN_ZONE_TYPE_3 ){
		obj_work->ppOut = gmBoss3PillarWallOutFunc;
	}
	else if ( zone_type == GSD_MAIN_ZONE_TYPE_FINAL ){
		obj_work->ppOut = gmBoss3PillarWallOutFuncForFinalZone;
	}
}

// ==========================================================================
// gmBoss3PillarWallDestFunc
/*!
 *	I—¹ŠÖ”
 *
 *	@param tcb	[in] TCB
 */
// ==========================================================================
void gmBoss3PillarWallDestFunc( MTS_TASK_TCB *tcb )
{
	GMS_GMK_BOSS3_PILLAR_WALL_WORK* wall_work = (GMS_GMK_BOSS3_PILLAR_WALL_WORK*)mtTaskGetTcbWork( tcb );
#if !_IPHONE
	//ƒOƒŒƒAƒ‚[ƒVƒ‡ƒ“‰ğ•ú
	ObjAction3dNNMotionRelease(	&wall_work->obj_3d_parts[GMD_GMK_BOSS3_PILLAR_WALL_PARTS_GLARE] );
#endif // !_IPHONE

	//ƒGƒtƒFƒNƒgíœ—v‹
	if ( wall_work->effect_work ){
		ObjDrawKillAction3DES( (OBS_OBJECT_WORK*)wall_work->effect_work );
		wall_work->effect_work = NULL;
	}

	//U“®’â~
	GMM_PAD_VIB_STOP();

	//Œø‰Ê‰¹ƒnƒ“ƒhƒ‹‰ğ•ú
	if ( wall_work->se_handle ){
		GmSoundStopSE( wall_work->se_handle );
		GsSoundFreeSeHandle( wall_work->se_handle );
		wall_work->se_handle = NULL;
	}

	//•W€I—¹
	GmEnemyDefaultExit( tcb );
}

// =======================================================================
// gmBoss3PillarWallOutFunc
/*!
 *	•`‰æŠÖ”
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3PillarWallOutFunc( OBS_OBJECT_WORK* obj_work )
{	
	amAssert( obj_work );

	s32 height_num = GMD_GMK_BOSS3_PILLAR_WALL_HEIGHT;
	s32 height_size_x = GMD_GMK_BOSS3_PILLAR_WALL_SIZE;
	s32 height_size_y = GMD_GMK_BOSS3_PILLAR_WALL_SIZE;
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX
#if !_IPHONE
	//— ‚É”Â‚ğ•`‰æ
	if ( obj_work->user_flag & GMD_GMK_BOSS3_PILLAR_FLAG_DRAW_WALL_BACK
			&& !(obj_work->disp_flag & OBD_DISP_NODISP)
	){

		Float width = (Float)height_size_x;
		Float height = (Float)height_num*height_size_y;
		Float ofst_x = (Float)-height_size_x;
		Float ofst_y = (Float)-height_size_y/2;
		s32 center = g_gm_main_system.map_fcol.left + (g_gm_main_system.map_fcol.right - g_gm_main_system.map_fcol.left)/2;
		if ( center*FX32_ONE < obj_work->pos.x ){
			ofst_x = 0.0f;
		}

		Float top = FX_FX32_TO_F32(obj_work->pos.y) + ofst_y;
		Float bottom = top + height;
		Float left = FX_FX32_TO_F32(obj_work->pos.x) + ofst_x;
		Float right = left + width;
		Float z = FX_FX32_TO_F32(obj_work->pos.z) - 10.0f;

		gmGmkBoss3PillarWallDrawBack(
				left,
				-top, 
				right, 
				-bottom,
				z );
	}
#endif // !_IPHONE
	
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
	void* model_tvx = amBindGet(g_gm_gmk_boss3_wall_obj_tvx_list, 0);
	VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
#else
	//ƒp[ƒc
	GMS_GMK_BOSS3_PILLAR_WALL_WORK* wall_work = (GMS_GMK_BOSS3_PILLAR_WALL_WORK*)obj_work;
	//ƒ}ƒeƒŠƒAƒ‹
	u32 disp_flag_glare = obj_work->disp_flag | OBD_DISP_REPEAT;
#if !_IPHONE
	if ( !ObjObjectPauseCheck(0) ){
		ObjDrawAction3DNNMaterialUpdate(
				&wall_work->obj_3d_parts[GMD_GMK_BOSS3_PILLAR_WALL_PARTS_GLARE],
				&disp_flag_glare );
	}
#endif // !_IPHONE
	disp_flag_glare |= OBD_DISP_NOUPDATE;
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX
	//•`‰æ
	for ( s32 i = 0; height_num > i; ++i ){
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
		// ˆÊ’uŒvZ
		VecFx32 pos = obj_work->pos;
		pos.y += height_size_y*i * FX32_ONE;
		
		GmTvxSetModel(model_tvx, obj_work->obj_3d->texlist, &pos, &scale, 0, 0);
#else
		obj_work->ofst.y = height_size_y*i * FX32_ONE;
		
		ObjDrawActionSummary( obj_work );
#if !_IPHONE
		VecFx32 pos = obj_work->pos;
		pos.y += obj_work->ofst.y;

		//Î
		ObjDrawAction3DNN(
				&wall_work->obj_3d_parts[GMD_GMK_BOSS3_PILLAR_WALL_PARTS_B],
				&pos, 
				&obj_work->dir, 
				&obj_work->scale, 
				&obj_work->disp_flag );
		//ƒOƒŒƒA
		ObjDrawAction3DNN(
				&wall_work->obj_3d_parts[GMD_GMK_BOSS3_PILLAR_WALL_PARTS_GLARE],
				&pos, 
				&obj_work->dir, 
				&obj_work->scale, 
				&disp_flag_glare );
#endif // !_IPHONE
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX
	}
}

// =======================================================================
// gmBoss3PillarWallOutFuncForFinalZone
/*!
 *	•`‰æŠÖ”
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3PillarWallOutFuncForFinalZone( OBS_OBJECT_WORK* obj_work )
{	
	amAssert( obj_work );

	s32 height_num = GMD_GMK_BOSS3_PILLAR_WALL_HEIGHT_F;
	s32 height_size_x = GMD_GMK_BOSS3_PILLAR_WALL_SIZE;
	s32 height_size_y = GMD_GMK_BOSS3_PILLAR_WALL_SIZE_F;	
#if !_IPHONE
	//— ‚É”Â‚ğ•`‰æ
	if ( obj_work->user_flag & GMD_GMK_BOSS3_PILLAR_FLAG_DRAW_WALL_BACK
			&& !(obj_work->disp_flag & OBD_DISP_NODISP)
	){

		Float width = (Float)height_size_x;
		Float height = (Float)height_num*height_size_y;
		Float ofst_x = (Float)-height_size_x;
		Float ofst_y = 0.0f;
		s32 center = g_gm_main_system.map_fcol.left + (g_gm_main_system.map_fcol.right - g_gm_main_system.map_fcol.left)/2;
		if ( center*FX32_ONE < obj_work->pos.x ){
			ofst_x = 0.0f;
		}

		Float top = FX_FX32_TO_F32(obj_work->pos.y) + ofst_y;
		Float bottom = top + height;
		Float left = FX_FX32_TO_F32(obj_work->pos.x) + ofst_x;
		Float right = left + width;
		Float z = FX_FX32_TO_F32(obj_work->pos.z) - 10.0f;

		gmGmkBoss3PillarWallDrawBack(
				left,
				-top, 
				right, 
				-bottom,
				z );
	}
#endif // !_IPHONE
	//ƒp[ƒc
	GMS_GMK_BOSS3_PILLAR_WALL_WORK* wall_work = (GMS_GMK_BOSS3_PILLAR_WALL_WORK*)obj_work;
	
	//ƒ}ƒeƒŠƒAƒ‹
	u32 disp_flag_glare = obj_work->disp_flag | OBD_DISP_REPEAT;
#if !_IPHONE
	if ( !ObjObjectPauseCheck(0) ){
		ObjDrawAction3DNNMaterialUpdate(
				&wall_work->obj_3d_parts[GMD_GMK_BOSS3_PILLAR_WALL_PARTS_GLARE],
				&disp_flag_glare );
	}
#endif // !_IPHONE
	disp_flag_glare |= OBD_DISP_NOUPDATE;

	//æ“ª•`‰æ
	obj_work->ofst.y = height_size_y * FX32_ONE;
	ObjDrawActionSummary( obj_work );
#if !_IPHONE
	//ƒOƒŒƒA•`‰æ
	VecFx32 pos_grare = obj_work->pos;
	pos_grare.y += obj_work->ofst.y;
	ObjDrawAction3DNN(
			&wall_work->obj_3d_parts[GMD_GMK_BOSS3_PILLAR_WALL_PARTS_GLARE],
			&pos_grare, 
			&obj_work->dir, 
			&obj_work->scale, 
			&disp_flag_glare );
#endif // !_IPHONE
	//Bƒp[ƒc•`‰æ
	for ( s32 i = 1; height_num > i; ++i ){
		VecFx32 pos_b_parts = obj_work->pos;
		pos_b_parts.y += obj_work->ofst.y + height_size_y*(i-1) * FX32_ONE;
		
		ObjDrawAction3DNN(
				&wall_work->obj_3d_parts[GMD_GMK_BOSS3_PILLAR_WALL_PARTS_B],
				&pos_b_parts, 
				&obj_work->dir, 
				&obj_work->scale, 
				&obj_work->disp_flag );
	}
}

// =======================================================================
// gmGmkBoss3PillarWallChangeModeWait
/*!
 *	‘Ò‹@ƒ‚[ƒh‚É•ÏX
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmGmkBoss3PillarWallChangeModeWait( OBS_OBJECT_WORK* obj_work )
{
#if GMD_GMK_BOSS3_PILLAR_TEST_NO_WALL
	UNREFERENCED_PARAMETER( obj_work );
	return;
#endif	//GMD_GMK_BOSS3_PILLAR_TEST_NO_WALL

	GMS_GMK_BOSS3_PILLAR_WALL_WORK* wall_work = (GMS_GMK_BOSS3_PILLAR_WALL_WORK*)obj_work;
	amAssert( wall_work );
	obj_work->spd.y = 0;

	//ƒƒCƒ“ˆ—•ÏX
	obj_work->ppFunc = gmGmkBoss3PillarWallMainWait;

	//ƒGƒtƒFƒNƒgÁ‚µ
	if ( wall_work->effect_work ){
		ObjDrawKillAction3DES( (OBS_OBJECT_WORK*)wall_work->effect_work );
		wall_work->effect_work = NULL;
	}
}

// =======================================================================
// gmGmkBoss3PillarWallChangeModeActive
/*!
 *	ƒAƒNƒeƒBƒuƒ‚[ƒh‚É•ÏX
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmGmkBoss3PillarWallChangeModeActive( OBS_OBJECT_WORK* obj_work )
{
#if GMD_GMK_BOSS3_PILLAR_TEST_NO_WALL
	UNREFERENCED_PARAMETER( obj_work );
	return;
#endif	//GMD_GMK_BOSS3_PILLAR_TEST_NO_WALL
	GMS_GMK_BOSS3_PILLAR_WALL_WORK* wall_work = (GMS_GMK_BOSS3_PILLAR_WALL_WORK*)obj_work;
	amAssert( wall_work );

	fx32 distance = (GMD_GMK_BOSS3_PILLAR_WALL_HEIGHT+1) * GMD_GMK_BOSS3_PILLAR_WALL_SIZE * FX32_ONE;

	wall_work->target_pos.y = wall_work->default_pos.y - distance;
	obj_work->spd.y = -GMD_GMK_BOSS3_PILLAR_WALL_SPEED;

	obj_work->ppFunc = gmGmkBoss3PillarWallMainActive;

	//ƒGƒtƒFƒNƒg¶¬
	gmGmkBoss3PillarEffectCreateWallAppear( obj_work );

	//Œø‰Ê‰¹i’ŒUŒ‚‚ä‚Á‚­‚èSEj
	if ( obj_work->user_flag & GMD_GMK_BOSS3_PILLAR_FLAG_SE_1 ){
		if ( wall_work->se_handle ){
			GmSoundPlaySE("Boss3_01", wall_work->se_handle);
		}
		obj_work->user_flag &= ~GMD_GMK_BOSS3_PILLAR_FLAG_SE_1;

		//U“®
		GMM_PAD_VIB_SMALL_NOEND();
	}

	//’nŒ`•Ó‚è••ÏX
	s16 ofst_x = (s16)(-GMD_GMK_BOSS3_PILLAR_WALL_SIZE/2);
	s32 center = g_gm_main_system.map_fcol.left + (g_gm_main_system.map_fcol.right - g_gm_main_system.map_fcol.left)/2;
	if ( center*FX32_ONE > obj_work->pos.x ){
		ofst_x = (s16)-GMD_GMK_BOSS3_PILLAR_WALL_SIZE;
	}
	GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
	gimmick_work->ene_com.col_work.obj_col.ofst_x = ofst_x;
	gimmick_work->ene_com.col_work.obj_col.width = (u16)(GMD_GMK_BOSS3_PILLAR_WALL_SIZE*1.5f);
}

// =======================================================================
// gmGmkBoss3PillarWallChangeModeReturn
/*!
 *	–ß‚éƒ‚[ƒh‚É•ÏX
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmGmkBoss3PillarWallChangeModeReturn( OBS_OBJECT_WORK* obj_work )
{
#if GMD_GMK_BOSS3_PILLAR_TEST_NO_WALL
	UNREFERENCED_PARAMETER( obj_work );
	return;
#endif	//GMD_GMK_BOSS3_PILLAR_TEST_NO_WALL
	GMS_GMK_BOSS3_PILLAR_WALL_WORK* wall_work = (GMS_GMK_BOSS3_PILLAR_WALL_WORK*)obj_work;
	amAssert( wall_work );

	wall_work->target_pos = wall_work->default_pos;
	obj_work->spd.y = GMD_GMK_BOSS3_PILLAR_WALL_SPEED;

	obj_work->ppFunc = gmGmkBoss3PillarWallMainActive;

	//— ‚Ì”Â‚ğ•`‰æ‚µ‚È‚¢
	obj_work->user_flag &= ~GMD_GMK_BOSS3_PILLAR_FLAG_DRAW_WALL_BACK;

	//ƒGƒtƒFƒNƒg¶¬
	gmGmkBoss3PillarEffectCreateWallAppear( obj_work );

	//Œø‰Ê‰¹i’ŒUŒ‚‚ä‚Á‚­‚èSEj
	if ( wall_work->se_handle ){
		GmSoundPlaySE("Boss3_01", wall_work->se_handle);
	}

	//U“®
	GMM_PAD_VIB_SMALL_NOEND();
}

// =======================================================================
// gmGmkBoss3PillarWallCheckMoveEnd
/*!
 *	ˆÚ“®I—¹Šm”F
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@return c‚è‹——£
 */
// =======================================================================
fx32 gmGmkBoss3PillarWallCheckMoveEnd( GMS_GMK_BOSS3_PILLAR_WALL_WORK* wall_work )
{
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)wall_work;

	//ˆÚ“®İ’è‚ª‚³‚ê‚Ä‚¢‚È‚¢
	if ( wall_work->target_pos.y == obj_work->pos.y
			|| obj_work->spd.y == 0
	){
		return 0;
	}

	//ˆÚ“®Ï‚İ
	fx32 distance = wall_work->target_pos.y - obj_work->pos.y;
	if ( obj_work->spd.y < 0 ){
		if ( distance >= 0 ){
			obj_work->pos.y = wall_work->target_pos.y;
			return 0;
		}
	}
	else if ( obj_work->spd.y > 0 ){
		if ( distance <= 0 ){
			obj_work->pos.y = wall_work->target_pos.y;
			return 0;
		}
	}

	return distance;
}

// ==========================================================================
// gmGmkBoss3PillarWallMainWait
/*!
 *	‘Ò‹@ó‘Ô
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkBoss3PillarWallMainWait( OBS_OBJECT_WORK* obj_work )
{
	UNREFERENCED_PARAMETER( obj_work );
}

// ==========================================================================
// gmGmkBoss3PillarWallMainActive
/*!
 *	ƒAƒNƒeƒBƒuó‘Ô
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmGmkBoss3PillarWallMainActive( OBS_OBJECT_WORK* obj_work )
{
	GMS_GMK_BOSS3_PILLAR_WALL_WORK* wall_work = (GMS_GMK_BOSS3_PILLAR_WALL_WORK*)obj_work;
	amAssert( wall_work );

	//ˆÚ“®
	fx32 distance = gmGmkBoss3PillarWallCheckMoveEnd( wall_work );

	OBS_OBJECT_WORK* player_obj_work = (OBS_OBJECT_WORK*)g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	amAssert( player_obj_work );

	//oŒ»
	if ( obj_work->spd.y < 0 ){

		//ƒvƒŒƒCƒ„‚ª•Ç‚Ìã•û‚É‚¢‚ÄA“Vˆä‚É‹²‚Ü‚ê‚»‚¤‚È‚Æ‚«AƒvƒŒƒCƒ„‚ªˆ³€‚µ‚È‚¢‚æ‚¤‚É’²®
		if ( (MTM_MATH_ABS(player_obj_work->pos.x - obj_work->pos.x) < GMD_GMK_BOSS3_PILLAR_WALL_SIZE/2*FX32_ONE)
				&& (player_obj_work->pos.y <= obj_work->pos.y )
				&& (MTM_MATH_ABS(distance) < GMD_GMK_BOSS3_PILLAR_WALL_NO_PRESS_DIE_FLOW_DISTANCE*FX32_ONE)
		){
			s32 center = g_gm_main_system.map_fcol.left + (g_gm_main_system.map_fcol.right - g_gm_main_system.map_fcol.left)/2;

			//İ’u‚µ‚Ä‚¢‚é‚Æ‚«
			if ( player_obj_work->move_flag & OBD_MOVE_UNDER ){
				fx32 move = GMD_GMK_BOSS3_PILLAR_WALL_NO_PRESS_DIE_MOVE_LAND*FX32_ONE;
				if ( center*FX32_ONE < player_obj_work->pos.x ){
					move *= -1;
				}
				player_obj_work->flow.x += move;
			}
			//ƒWƒƒƒ“ƒv’†
			else{
				fx32 move = GMD_GMK_BOSS3_PILLAR_WALL_NO_PRESS_DIE_MOVE_JUMP*FX32_ONE;
				if ( center*FX32_ONE < player_obj_work->pos.x ){
					move *= -1;
				}
				GmPlySeqGmkInitGmkJump( (GMS_PLAYER_WORK*)player_obj_work, move, 0);
				GmPlySeqChangeSequenceState( (GMS_PLAYER_WORK*)player_obj_work, GME_PLY_SEQ_STATE_JUMP );
			}
		}
	}
	
	//ˆÚ“®I—¹‘Ò‚¿
	if ( distance ){
		return;
	}
	obj_work->pos = wall_work->target_pos;

	//‘Ò‹@‚Ö
	gmGmkBoss3PillarWallChangeModeWait( obj_work );

	//ƒqƒbƒgƒGƒtƒFƒNƒg
	if ( obj_work->user_flag & GMD_GMK_BOSS3_PILLAR_FLAG_HIT_EFFECT ){
		gmGmkBoss3PillarEffectCreateWallHit( obj_work );
		obj_work->user_flag &= ~GMD_GMK_BOSS3_PILLAR_FLAG_HIT_EFFECT;
	}

	//U“®’â~
	GMM_PAD_VIB_STOP();

	//ƒJƒƒ‰U“®
	if ( obj_work->user_flag & GMD_GMK_BOSS3_PILLAR_FLAG_CAMERA_VIBRATION ){
		GmCameraVibrationSet( 0, GMD_GMK_BOSS3_PILLAR_QUAKE_FALL, 0 );
		obj_work->user_flag &= ~GMD_GMK_BOSS3_PILLAR_FLAG_CAMERA_VIBRATION;
	}

	//Œø‰Ê‰¹i’ŒUŒ‚ƒXƒs[ƒhƒAƒbƒvSEj
	if ( wall_work->se_handle ){
		GmSoundStopSE( wall_work->se_handle );
		if ( obj_work->user_flag & GMD_GMK_BOSS3_PILLAR_FLAG_SE_2 ){
			GmSoundPlaySE("Boss3_02", wall_work->se_handle);
			obj_work->user_flag &= ~GMD_GMK_BOSS3_PILLAR_FLAG_SE_2;
		}
	}
}


// ==========================================================================
// gmGmkBoss3PillarWallDrawBack
/*!
 *	•Ç‚Ì”wŒã‚ğ•`‰æ
 *
 * @param left ‹éŒ`
 * @param top ‹éŒ`
 * @param right ‹éŒ`
 * @param bottom ‹éŒ`
 * @param z ‚yˆÊ’u
 */
// ==========================================================================
void gmGmkBoss3PillarWallDrawBack(
									Float left,
									Float top,
									Float right,
									Float bottom,
									Float z)
{	AMS_PARAM_DRAW_PRIMITIVE param;
	amZeroMemory( &param, sizeof(param) );

	param.aTest = 0;
	param.zMask = 0;
	param.zTest = 1;

	//ƒAƒ‹ƒtƒ@
	param.ablend = NNE_PRIM_ALPHABLEND_OFF;
	param.noSort = 1;

	// ƒeƒNƒXƒ`ƒƒ‚È‚µ
	NNS_PRIM3D_PC* poli_data = (NNS_PRIM3D_PC *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PC) * 6);

	// ’¸“_
	amVectorSet(
			(NNS_VECTOR*)&poli_data[0], 
			left, 
			top, 
			z );
	amVectorSet(
			(NNS_VECTOR*)&poli_data[1], 
			right, 
			top, 
			z );
	amVectorSet(
			(NNS_VECTOR*)&poli_data[2], 
			left, 
			bottom, 
			z );
	amVectorSet(
			(NNS_VECTOR*)&poli_data[5],
			right, 
			bottom, 
			z );

	// ƒJƒ‰[
	NNS_RGBA8888 color = AMD_RGBA8888( 0, 0, 0, 255);
	poli_data[0].Col = color;
	poli_data[1].Col = color;
	poli_data[2].Col = color;
	poli_data[5].Col = color;

	poli_data[3] = poli_data[1];
	poli_data[4] = poli_data[2];

	// ƒvƒŠƒ~ƒeƒBƒu•`‰æİ’è
	param.format3D = NNE_PRIM3D_FMT_PC;
	param.type = NNE_PRIM_TRIANGLE_LIST;
	param.vtxPC3D = poli_data;
	param.texlist = NULL;
	param.texId = 0;
	param.count = 6;
	param.sortZ = -1.0f;
	
	//•`‰æƒRƒ}ƒ“ƒh
	gmGmkBoss3PillarWallMatrixPush(OBD_DRAW_CMD_STATE_3DNN);
	amDrawPrimitive3D( OBD_DRAW_CMD_STATE_3DNN, &param );
	gmGmkBoss3PillarWallMatrixPop(OBD_DRAW_CMD_STATE_3DNN);
}

// ==========================================================================
// gmGmkBoss3PillarWallMatrixPush
/*!
 *	ƒ}ƒgƒŠƒNƒXƒvƒbƒVƒ…ƒRƒ}ƒ“ƒh
 */
// ==========================================================================
void gmGmkBoss3PillarWallMatrixPush( u32 command_state )
{
	ObjDraw3DNNUserFunc( 
			gmGmkBoss3PillarWallUserFuncMatrixPush,
			NULL, 
			0, 
			command_state);
}

// ==========================================================================
// gmGmkBoss3PillarWallMatrixPop
/*!
 *	ƒ}ƒgƒŠƒNƒXƒ|ƒbƒvƒRƒ}ƒ“ƒh
 */
// ==========================================================================
void gmGmkBoss3PillarWallMatrixPop( u32 command_state )
{
	ObjDraw3DNNUserFunc(
		gmGmkBoss3PillarWallUserFuncPop,
		NULL, 
		0, 
		command_state);
}

// ==========================================================================
// gmGmkBoss3PillarWallUserFuncMatrixPush
/*!
 *	ƒ}ƒgƒŠƒNƒXƒvƒbƒVƒ…ƒRƒ}ƒ“ƒh
 */
// ==========================================================================
void gmGmkBoss3PillarWallUserFuncMatrixPush( void* param )
{
	UNREFERENCED_PARAMETER( param );
	
	amMatrixPush();
	
	NNS_MATRIX* current_matrix = amMatrixGetCurrent();	

	NNS_MATRIX matrix;
	nnMultiplyMatrix( &matrix, amDrawGetWorldViewMatrix(), current_matrix );
	
	// 3DƒvƒŠƒ~ƒeƒBƒu•`‰æ—pƒ}ƒgƒŠƒbƒNƒX‚ğƒZƒbƒgiƒrƒ…[ƒ}ƒgƒŠƒNƒX‚İj
	nnSetPrimitive3DMatrix( &matrix );
}

// ==========================================================================
// gmGmkBoss3PillarWallUserFuncPop
/*!
 *	ƒ}ƒgƒŠƒNƒXƒ|ƒbƒvƒRƒ}ƒ“ƒh
 */
// ==========================================================================
void gmGmkBoss3PillarWallUserFuncPop( void* param )
{
	UNREFERENCED_PARAMETER( param );

	amMatrixPop();
}

// ==========================================================================
// gmGmkBoss3PillarEffectCreatePillarAppear
/*!
 *	’ŒoŒ»ƒGƒtƒFƒNƒg¶¬
 *
 *	@param obj_work eƒIƒuƒWƒFƒNƒg
 */
// ==========================================================================
void gmGmkBoss3PillarEffectCreatePillarAppear( OBS_OBJECT_WORK* obj_work )
{
	amAssert( obj_work );

	GMS_GMK_BOSS3_PILLAR_MAIN_WORK* main_work = (GMS_GMK_BOSS3_PILLAR_MAIN_WORK*)obj_work;

	//¶¬Ï‚İ
	if ( main_work->effect_work ){
		return;
	}


	//ƒ^ƒCƒv•Êİ’è
	fx32 pos_x = main_work->default_pos.x;
	fx32 pos_y = main_work->default_pos.y;
	s32 effect_id = 0;
	u32 disp_flag = 0;

	//¶¬
	GME_GMK_BOSS3_PILLAR_TYPE pillar_type = gmGmkBoss3PillarCalcPillarType( main_work->gimmick_work.ene_com.eve_rec->id);	
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	GMS_EFFECT_3DES_WORK* effect_work = NULL;
	if ( zone_type == GSD_MAIN_ZONE_TYPE_3 ){
		switch ( pillar_type ){
		case GMD_GMK_BOSS3_PILLAR_TYPE_LEFT:
			pos_x += GMD_GMK_BOSS3_PILLAR_PILLAR_EFFECT_OFFSET_Y;
			effect_id = GME_EFCT_Z03_IDX_PILLAR_02;
			disp_flag = OBD_DISP_HFLIP;
			break;
		case GMD_GMK_BOSS3_PILLAR_TYPE_RIGHT:
			pos_x -= GMD_GMK_BOSS3_PILLAR_PILLAR_EFFECT_OFFSET_Y;
			effect_id = GME_EFCT_Z03_IDX_PILLAR_02;
			break;
		case GMD_GMK_BOSS3_PILLAR_TYPE_TOP:
			pos_y += GMD_GMK_BOSS3_PILLAR_PILLAR_EFFECT_OFFSET_Y;
			effect_id = GME_EFCT_Z03_IDX_PILLAR_03;
			break;
		case GMD_GMK_BOSS3_PILLAR_TYPE_BOTTOM:
			pos_y -= GMD_GMK_BOSS3_PILLAR_PILLAR_EFFECT_OFFSET_Y;
			effect_id = GME_EFCT_Z03_IDX_PILLAR_01;
			break;
		default:
			amAssert( FALSE );
			break;
		}
		effect_work = GmEfctZoneEsCreate(
				NULL,	//e‚É‚Í‚µ‚È‚¢iíœ‚ÉAƒGƒtƒFƒNƒg‚ª‹}‚ÉÁ‚¦‚È‚¢‚æ‚¤‚Éj
				zone_type,
				effect_id );
	}
	else{
		switch ( pillar_type ){
		case GMD_GMK_BOSS3_PILLAR_TYPE_LEFT:
			pos_x += GMD_GMK_BOSS3_PILLAR_PILLAR_EFFECT_OFFSET_Y_FINAL;
			effect_id = GME_EFCT_CMN_IDX_PILLAR_F_02;
			disp_flag = OBD_DISP_HFLIP;
			break;
		case GMD_GMK_BOSS3_PILLAR_TYPE_RIGHT:
			pos_x -= GMD_GMK_BOSS3_PILLAR_PILLAR_EFFECT_OFFSET_Y_FINAL;
			effect_id = GME_EFCT_CMN_IDX_PILLAR_F_02;
			break;
		case GMD_GMK_BOSS3_PILLAR_TYPE_TOP:
			pos_y += GMD_GMK_BOSS3_PILLAR_PILLAR_EFFECT_OFFSET_Y_FINAL;
			effect_id = GME_EFCT_CMN_IDX_PILLAR_F_03;
			break;
		case GMD_GMK_BOSS3_PILLAR_TYPE_BOTTOM:
			pos_y -= GMD_GMK_BOSS3_PILLAR_PILLAR_EFFECT_OFFSET_Y_FINAL;
			effect_id = GME_EFCT_CMN_IDX_PILLAR_F_01;
			break;
		default:
			amAssert( FALSE );
			break;
		}
		effect_work = GmEfctCmnEsCreate( 
				NULL,	//e‚É‚Í‚µ‚È‚¢iíœ‚ÉAƒGƒtƒFƒNƒg‚ª‹}‚ÉÁ‚¦‚È‚¢‚æ‚¤‚Éj
				(GME_EFCT_CMN_IDX)effect_id );
	}
	amAssert( effect_work );

	//ƒ[ƒNİ’è
	OBS_OBJECT_WORK* obj_work_effect = (OBS_OBJECT_WORK*)effect_work;
	obj_work_effect->pos.x = pos_x;
	obj_work_effect->pos.y = pos_y;
	obj_work_effect->pos.z = GMD_OBJ_DEFAULT_POS_Z_B;
	obj_work_effect->disp_flag |= disp_flag;

	//’Œƒ[ƒN‚É“o˜^
	main_work->effect_work = effect_work;
}

// ==========================================================================
// gmGmkBoss3PillarEffectCreatePillarHit
/*!
 *	’ŒƒqƒbƒgƒGƒtƒFƒNƒg¶¬
 *
 *	@param obj_work eƒIƒuƒWƒFƒNƒg
 */
// ==========================================================================
void gmGmkBoss3PillarEffectCreatePillarHit( OBS_OBJECT_WORK* obj_work )
{
	amAssert( obj_work );

	GMS_GMK_BOSS3_PILLAR_MAIN_WORK* main_work = (GMS_GMK_BOSS3_PILLAR_MAIN_WORK*)obj_work;

	//ƒ^ƒCƒv•Êİ’è
	fx32 pos_x = main_work->target_pos.x;
	fx32 pos_y = main_work->target_pos.y;
	s32 effect_id = 0;
	u16 dir_z = 0;

	//¶¬
	GME_GMK_BOSS3_PILLAR_TYPE pillar_type = gmGmkBoss3PillarCalcPillarType( main_work->gimmick_work.ene_com.eve_rec->id);	
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	GMS_EFFECT_3DES_WORK* effect_work = NULL;
	if ( zone_type == GSD_MAIN_ZONE_TYPE_3 ){
		effect_id = GME_EFCT_Z03_IDX_PILLAR_HIT;
		switch ( pillar_type ){
		case GMD_GMK_BOSS3_PILLAR_TYPE_LEFT:
			pos_x += (fx32)((GMD_GMK_BOSS3_PILLAR_SIZE-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT)*FX32_ONE);
			dir_z = (u16)NNM_DEGtoA16(-90);
			break;
		case GMD_GMK_BOSS3_PILLAR_TYPE_RIGHT:
			pos_x -= (fx32)((GMD_GMK_BOSS3_PILLAR_SIZE-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT)*FX32_ONE);
			dir_z = (u16)NNM_DEGtoA16(90);
			break;
		case GMD_GMK_BOSS3_PILLAR_TYPE_TOP:
			pos_y += (fx32)((GMD_GMK_BOSS3_PILLAR_SIZE-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT)*FX32_ONE);
			break;
		case GMD_GMK_BOSS3_PILLAR_TYPE_BOTTOM:
			pos_y -= (fx32)((GMD_GMK_BOSS3_PILLAR_SIZE-GMD_GMK_BOSS3_PILLAR_MARGIN_FRONT)*FX32_ONE);
			dir_z = (u16)NNM_DEGtoA16(180);
			break;
		default:
			break;
		}
		effect_work = GmEfctZoneEsCreate(
				NULL,
				zone_type,
				effect_id );
	}
	else{
		effect_id = GME_EFCT_CMN_IDX_PILLAR_F_HIT;
		switch ( pillar_type ){
		case GMD_GMK_BOSS3_PILLAR_TYPE_LEFT:
			pos_x += (fx32)((GMD_GMK_BOSS3_PILLAR_SIZE)*FX32_ONE);
			dir_z = (u16)NNM_DEGtoA16(-90);
			break;
		case GMD_GMK_BOSS3_PILLAR_TYPE_RIGHT:
			pos_x -= (fx32)((GMD_GMK_BOSS3_PILLAR_SIZE)*FX32_ONE);
			dir_z = (u16)NNM_DEGtoA16(90);
			break;
		case GMD_GMK_BOSS3_PILLAR_TYPE_TOP:
			pos_y += (fx32)((GMD_GMK_BOSS3_PILLAR_SIZE)*FX32_ONE);
			break;
		case GMD_GMK_BOSS3_PILLAR_TYPE_BOTTOM:
			pos_y -= (fx32)((GMD_GMK_BOSS3_PILLAR_SIZE)*FX32_ONE);
			dir_z = (u16)NNM_DEGtoA16(180);
			break;
		default:
			break;
		}
		effect_work = GmEfctCmnEsCreate( 
				NULL,	//e‚É‚Í‚µ‚È‚¢iíœ‚ÉAƒGƒtƒFƒNƒg‚ª‹}‚ÉÁ‚¦‚È‚¢‚æ‚¤‚Éj
				(GME_EFCT_CMN_IDX)effect_id );
	}
	amAssert( effect_work );

	//ƒ[ƒNİ’è
	OBS_OBJECT_WORK* obj_work_effect = (OBS_OBJECT_WORK*)effect_work;
	obj_work_effect->pos.x = pos_x;
	obj_work_effect->pos.y = pos_y;
	obj_work_effect->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_FRONT;
	obj_work_effect->dir.z = dir_z;
}

// ==========================================================================
// gmGmkBoss3PillarEffectCreateWallAppear
/*!
 *	•ÇoŒ»ƒGƒtƒFƒNƒg¶¬
 *
 *	@param obj_work eƒIƒuƒWƒFƒNƒg
 */
// ==========================================================================
void gmGmkBoss3PillarEffectCreateWallAppear( OBS_OBJECT_WORK* obj_work )
{
	amAssert( obj_work );

	GMS_GMK_BOSS3_PILLAR_WALL_WORK* wall_work = (GMS_GMK_BOSS3_PILLAR_WALL_WORK*)obj_work;

	//¶¬Ï‚İ
	if ( wall_work->effect_work ){
		return;
	}	
	
	//ƒ^ƒCƒv•Êİ’è
	fx32 pos_x = wall_work->default_pos.x;
	fx32 pos_y = wall_work->default_pos.y;

	GMS_EFFECT_3DES_WORK* effect_work = NULL;
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	if ( zone_type == GSD_MAIN_ZONE_TYPE_3 ){
		pos_y -= GMD_GMK_BOSS3_PILLAR_WALL_EFFECT_OFFSET_Y;

		//¶¬
		effect_work = GmEfctZoneEsCreate(
				NULL,	//e‚É‚Í‚µ‚È‚¢iíœ‚ÉAƒGƒtƒFƒNƒg‚ª‹}‚ÉÁ‚¦‚È‚¢‚æ‚¤‚Éj
				zone_type,
				GME_EFCT_Z03_IDX_PILLAR_01 );
		amAssert( effect_work );
	}
	else{ 
		pos_y -= GMD_GMK_BOSS3_PILLAR_WALL_EFFECT_OFFSET_Y_FINAL;

		//¶¬
		effect_work = GmEfctCmnEsCreate( 
				NULL,	//e‚É‚Í‚µ‚È‚¢iíœ‚ÉAƒGƒtƒFƒNƒg‚ª‹}‚ÉÁ‚¦‚È‚¢‚æ‚¤‚Éj
				GME_EFCT_CMN_IDX_PILLAR_F_01 );
		amAssert( effect_work );
	}

	//ƒ[ƒNİ’è
	OBS_OBJECT_WORK* obj_work_effect = (OBS_OBJECT_WORK*)effect_work;
	obj_work_effect->pos.x = pos_x;
	obj_work_effect->pos.y = pos_y;
	obj_work_effect->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_BACK;

	//•Çƒ[ƒN‚É“o˜^
	wall_work->effect_work = effect_work;
}

// ==========================================================================
// gmGmkBoss3PillarEffectCreateWallHit
/*!
 *	•ÇƒqƒbƒgƒGƒtƒFƒNƒg¶¬
 *
 *	@param obj_work eƒIƒuƒWƒFƒNƒg
 */
// ==========================================================================
void gmGmkBoss3PillarEffectCreateWallHit( OBS_OBJECT_WORK* obj_work )
{
	amAssert( obj_work );

	GMS_GMK_BOSS3_PILLAR_WALL_WORK* wall_work = (GMS_GMK_BOSS3_PILLAR_WALL_WORK*)obj_work;

	//ƒ^ƒCƒv•Êİ’è
	fx32 pos_x = wall_work->target_pos.x;
	fx32 pos_y = wall_work->target_pos.y;
	
	//¶¬
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	GMS_EFFECT_3DES_WORK* effect_work = NULL;
	if ( zone_type == GSD_MAIN_ZONE_TYPE_3 ){
		pos_y -= GMD_GMK_BOSS3_PILLAR_WALL_SIZE/2*FX32_ONE;
		effect_work = GmEfctZoneEsCreate(
				NULL,	//e‚É‚Í‚µ‚È‚¢iíœ‚ÉAƒGƒtƒFƒNƒg‚ª‹}‚ÉÁ‚¦‚È‚¢‚æ‚¤‚Éj
				zone_type,
				GME_EFCT_Z03_IDX_PILLAR_HIT );
	}
	else{
		pos_y -= 0*FX32_ONE;
		effect_work = GmEfctCmnEsCreate( 
				NULL,	//e‚É‚Í‚µ‚È‚¢iíœ‚ÉAƒGƒtƒFƒNƒg‚ª‹}‚ÉÁ‚¦‚È‚¢‚æ‚¤‚Éj
				GME_EFCT_CMN_IDX_PILLAR_F_HIT );
	}
	amAssert( effect_work );

	//ƒ[ƒNİ’è
	OBS_OBJECT_WORK* obj_work_effect = (OBS_OBJECT_WORK*)effect_work;
	obj_work_effect->pos.x = pos_x;
	obj_work_effect->pos.y = pos_y;
	obj_work_effect->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_FRONT;
	obj_work_effect->dir.z = (s16)NNM_DEGtoA16(180);
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
