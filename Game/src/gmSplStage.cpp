// ===========================================================================
/*!
	@file	gmSplStage.cpp
	@brief	ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒWŠÇ—

	@author	K.Kuramoto
				Copyright(c) 2009 Dimps
	$Id: gmSplStage.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
	
 */
// ===========================================================================
/*
 *
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"

#include "gs.h"
#include "gsMainSys.h"
#include "gmMain.h"
#include "izFade.h"
#include "gmTask.h"

#include "gmGameDat.h"
#include "gmMainDat.h"
#include "objObject.h"
#include "objCamera.h"
#include "gmCamera.h"
#include "gmCockpit.h"
#include "gmObj.h"
#include "gmEnemy.h"
#include "gmPlySeq.h"
#include "gmPlySeqGmk.h"
#include "gmPlySpec.h"
#include "gmRing.h"
#include "gmSplStage.h"
#include "gmFix.h"
#include "gmStartMsg.h"
#include "gmPadVib.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "common/arc/CPIT_MAIN.HMB"


//----- Definitions ---------------------------------------------------------


#define GMD_SPL_STAGE_FADEIN_TIME	(30.0f)	//ƒtƒF[ƒhƒCƒ“ŠÔ

#define GMD_SPL_STAGE_ROLL_SPD_INIT	(0x0100)	// ƒS[ƒ‹Œã‚Ì‰æ–Ê‰ñ“]‘¬“x‰‘¬“x
#define GMD_SPL_STAGE_ROLL_SPD_ACL	(0x0038)	// ƒS[ƒ‹Œã‚Ì‰æ–Ê‰ñ“]‘¬“x‰Á‘¬“x
#define GMD_SPL_STAGE_ROLL_FADEWAIT	(90)		// ƒS[ƒ‹Œã‚Ì‰æ–Ê‰ñ“]ŠJn‚©‚çƒtƒF[ƒhƒAƒEƒgŠJn‚Ü‚Å‚ÌŠÔ

// ƒ‰ƒCƒgˆÊ’uî•ñ
#define GMD_SPL_STG_BASE_LIGHT_POS_X	(-1.0f)		// Šî–{ƒ‰ƒCƒgˆÊ’uXiƒƒCƒ“ƒQ[ƒ€‚Æ“¯‚¶j
#define GMD_SPL_STG_BASE_LIGHT_POS_Y	(-1.0f)		// Šî–{ƒ‰ƒCƒgˆÊ’uY
#define GMD_SPL_STG_BASE_LIGHT_POS_Z	(-1.0f)		// Šî–{ƒ‰ƒCƒgˆÊ’uZ
#define GMD_SPL_STG_OPT1_LIGHT_POS_X	(-0.4f)		// Šg’£ƒ‰ƒCƒgˆÊ’uXiƒXƒyƒXƒe—pj
#define GMD_SPL_STG_OPT1_LIGHT_POS_Y	(-0.4f)		// Šg’£ƒ‰ƒCƒgˆÊ’uY
#define GMD_SPL_STG_OPT1_LIGHT_POS_Z	(-1.0f)		// Šg’£ƒ‰ƒCƒgˆÊ’uZ

// ƒXƒCƒbƒ`î•ñ—ˆê•û’ÊsŠÛ’Œ
#define GMD_SPL_STG_SW_MAX			(16)		// ƒXƒCƒbƒ`Å‘å”

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmSplStageFadeInWait( MTS_TASK_TCB* tcb );
static void gmSplStageFadeInWait2( MTS_TASK_TCB* tcb );
static void gmSplStagePlayEndChk( MTS_TASK_TCB* tcb );
static void gmSplStageRolling( MTS_TASK_TCB* tcb );
static void gmSplStageGotoEnd( MTS_TASK_TCB* tcb );
static void gmSplStageEnd( MTS_TASK_TCB* tcb );
static void gmSplStageLightCtrl(GMS_SPL_STG_WORK *tcb_work);
static NNS_VECTOR gmSplStageLightRot(float pos_x, float pos_y, float pos_z);
static void gmSplStageNudgeCtrl(void);
static void gmSplStageRingGateChk(GMS_SPL_STG_WORK *tcb_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static MTS_TASK_TCB* gm_spl_tcb = NULL;

// ƒXƒyƒXƒeŠJn‚ÌŠÔİ’è
static const u32 gm_spl_stage_init_time[] = {
	//		 •ª		 •b
	( 3600 * 1 ) + ( 30 * 60 ),			// ss1
	( 3600 * 1 ) + ( 30 * 60 ),			// ss2
	( 3600 * 0 ) + ( 30 * 60 ),			// ss3
	( 3600 * 1 ) + ( 30 * 60 ),			// ss4
	( 3600 * 0 ) + ( 30 * 60 ),			// ss5
	( 3600 * 1 ) + ( 30 * 60 ),			// ss6
	( 3600 * 1 ) + ( 30 * 60 ),			// ss7
};


// ƒŠƒ“ƒOƒQ[ƒgŠJù–‡”
static const u16 gm_spl_stage_ringgate_num[7][GMD_SS_RINGGATE_MAX] = {
	{
	// Stage1
		35,												// 0
		67,												// 1
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		// dummy
	},{
	// Stage2
		55,												// 0
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,	// dummy
	},{
	// Stage3
		80,												// 0
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,	// dummy
	},{
	// Stage4
		25,												// 0
		50,												// 1
		52,												// 2
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff,				// dummy
	},{
	// Stage5
		30,												// 0
		99,												// 1
		99,												// 2
		99,												// 3
		99,												// 4
		0xff, 0xff, 0xff, 0xff,							// dummy
	},{
	// Stage6
		10,												// 0
		28,												// 1
		30,												// 2
		45,												// 3
		57,												// 4
		70,												// 5
		75,												// 6
		93,												// 7
		99,												// 8
	},{
	// Stage7
		99,												// 0
		99,												// 1
		99,												// 2
		99,												// 3
		0xff, 0xff, 0xff, 0xff, 0xff,					// dummy
	}
};
/// ƒXƒCƒbƒ`ó‘Ôƒ[ƒN—ˆê•û’ÊsŠÛ’Œ
static u8 gm_gmk_ss_switch[GMD_SPL_STG_SW_MAX] = {0};
//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmSplStageStart
/*!
	ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒWŠJn
 */
// ==========================================================================
void GmSplStageStart(void)
{
	u16	stage_id;
	stage_id = (u16)(g_gs_main_sys_info.stage_id - GSD_MAIN_STAGE_ID_SS1);		// SplStg 1`7
	MTM_ASSERT(stage_id <= (GSD_MAIN_STAGE_ID_SS7-GSD_MAIN_STAGE_ID_SS1));

	gm_spl_tcb = MTM_TASK_MAKE_TCB(gmSplStageFadeInWait,
									NULL,
									0,
									GMD_TASK_PAUSELEVEL_DEF,
/*
									GMD_TASK_PRIO_STARTDEMO,
*/
									GMD_TASK_PRIO_MAIN_PRE + 0x050,
									GMD_TASK_GROUP_MAIN_PRE,
									sizeof( GMS_SPL_STG_WORK ),
									"SPL_STG_CTRL" );
	amAssert( gm_spl_tcb ); 

	// ƒ[ƒNİ’è
	GMS_SPL_STG_WORK* tcb_work = (GMS_SPL_STG_WORK*)mtTaskGetTcbWork( gm_spl_tcb );
	amAssert( tcb_work );
	tcb_work->counter = 0;
	tcb_work->light_vec.x = -1.0f;
	tcb_work->light_vec.y = -1.0f;
	tcb_work->light_vec.z = -1.0f;
	tcb_work->get_ring = 0;
#if defined(MTD_DEBUG)
	tcb_work->dbg_cursor = 0;
#endif//defined(MTD_DEBUG)

	// ƒVƒXƒeƒ€I—¹ƒtƒ‰ƒO‰Šú‰»
	g_gm_main_system.game_flag &= ~(  GMD_GAME_FLAG_SPL_CHAOSGET
									| GMD_GAME_FLAG_SPL_FAILED
									| GMD_GAME_FLAG_SPL_TIMEOVER );

	// ƒtƒF[ƒhˆ—ŠJn
	IzFadeInitEasy(
			IZE_FADE_SET_TYPE_TAKEOEVER,
			IZE_FADE_TYPE_WHITE_FADEOUT,
			8.0f );

	g_gm_main_system.game_time = gm_spl_stage_init_time[stage_id];
	g_gm_main_system.game_flag |= GMD_GAME_FLAG_START_DEMO;


	MI_CpuClear8(gm_gmk_ss_switch, sizeof(gm_gmk_ss_switch));				// ƒXƒCƒbƒ`‰Šú‰»
}

// ==========================================================================
// GmSplStageExit
/*!
	ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒW‹­§I—¹ˆ—
 */
// ==========================================================================
void GmSplStageExit(void)
{
	// ƒ^ƒXƒNƒNƒŠƒA
	if (gm_spl_tcb) {
		mtTaskClearTcb(gm_spl_tcb);

		gm_spl_tcb = NULL;

		// ƒVƒXƒeƒ€I—¹ƒtƒ‰ƒO‰Šú‰»
		g_gm_main_system.game_flag &= ~(  GMD_GAME_FLAG_SPL_CHAOSGET
										| GMD_GAME_FLAG_SPL_FAILED
										| GMD_GAME_FLAG_SPL_TIMEOVER );
	}
}

// ==========================================================================
// GmSplStageSetLight
/*!
 * ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒW—pƒ‰ƒCƒgƒZƒbƒg
 *
 */
// ==========================================================================
void GmSplStageSetLight(void)
{
	NNS_RGBA	light_col = {
		1.0f, 1.0f, 1.0f, 1.0f,
	};
	NNS_VECTOR	light_vec;

	light_vec.x = GMD_SPL_STG_OPT1_LIGHT_POS_X;
	light_vec.y = GMD_SPL_STG_OPT1_LIGHT_POS_Y;
	light_vec.z = GMD_SPL_STG_OPT1_LIGHT_POS_Z;
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_1, &light_col, 1.f, &light_vec);
}

// ==========================================================================
// GmSplStageGetWork
/*!
 * ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒW—pƒ[ƒNæ“¾
 *
 *	@return	ƒXƒyƒXƒe—p(GMS_SPL_STG_WORK)ƒ[ƒNƒAƒhƒŒƒX
 */
// ==========================================================================
GMS_SPL_STG_WORK* GmSplStageGetWork(void)
{
	MTM_ASSERT(gm_spl_tcb);

	GMS_SPL_STG_WORK* tcb_work = (GMS_SPL_STG_WORK*)mtTaskGetTcbWork( gm_spl_tcb );
	return (tcb_work);
}

// ==========================================================================
// GmSplStageSwSet
/*!
 * ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒW—pƒXƒCƒbƒ`ƒtƒ‰ƒOƒZƒbƒg—ˆê•û’ÊsŠÛ’Œ
 *
 *	@param sw_no	[in] ƒXƒCƒbƒ`No.(0-15)
 */
// ==========================================================================
void GmSplStageSwSet(u32 sw_no)
{
	sw_no &= 0x0f;
	gm_gmk_ss_switch[sw_no] = 1;
}

// ==========================================================================
// GmSplStageSwCheck
/*!
 * ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒW—pƒXƒCƒbƒ`ƒtƒ‰ƒOƒ`ƒFƒbƒN—ˆê•û’ÊsŠÛ’Œ
 *
 *	@param sw_no	[in] ƒXƒCƒbƒ`No.(0-15)
 */
// ==========================================================================
BOOL GmSplStageSwCheck(u32 sw_no)
{
	sw_no &= 0x0f;
	if (gm_gmk_ss_switch[sw_no]) {
		return TRUE;
	} else {
		return FALSE;
	}
}

// ==========================================================================
// GmSplStageRingGateNumGet
/*!
 *	ƒŠƒ“ƒOƒQ[ƒgæ“¾–‡”æ“¾
 *
 *	@param gate_id	[in] ƒQ[ƒgID”Ô†( 0`GMD_SS_RINGGATE_MAX )
 */
// ==========================================================================
u16 GmSplStageRingGateNumGet(u16 gate_id)
{
	u16	stage_id = (u16)(g_gs_main_sys_info.stage_id - GSD_MAIN_STAGE_ID_SS1);		// SplStg 1`7
	MTM_ASSERT(stage_id <= (GSD_MAIN_STAGE_ID_SS7-GSD_MAIN_STAGE_ID_SS1));

	return (gm_spl_stage_ringgate_num[stage_id][gate_id]);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmSplStageFadeInWait
/*!
 * ‰ŠúƒtƒF[ƒhƒCƒ“ˆ—
 */
// ==========================================================================
void gmSplStageFadeInWait( MTS_TASK_TCB* tcb )
{
	GMS_SPL_STG_WORK*	tcb_work = (GMS_SPL_STG_WORK*)mtTaskGetTcbWork( tcb );
	tcb_work->counter++;
	if (tcb_work->counter > 30) {
		tcb_work->counter = 0;
		mtTaskChangeTcbProcedure(tcb, gmSplStageFadeInWait2);

		// ƒtƒF[ƒhˆ—ŠJn
		IzFadeInitEasy(
					   IZE_FADE_SET_TYPE_NORMAL,
					   IZE_FADE_TYPE_WHITE_FADEIN,
					   GMD_SPL_STAGE_FADEIN_TIME );
	}
}

void gmSplStageFadeInWait2( MTS_TASK_TCB* tcb )
{
//	GMS_SPL_STG_WORK* work = (GMS_SPL_STG_WORK*)mtTaskGetTcbWork( tcb );
//	amAssert( work );

	// ƒ|[ƒYƒ`ƒFƒbƒN
	if (ObjObjectPauseCheck(0)) return;

	if (IzFadeIsEnd()) {
		// ƒtƒF[ƒhI—¹
		IzFadeExit();

		//ƒvƒŒƒCƒ„s“®ŠJn
		GMS_PLAYER_WORK* ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
//		GmPlySeqInitDemoFw( player_work );
		ply_work->obj_work.move_flag &= ~(OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);
		ply_work->nudge_di_timer= 0;					//!< —h‚ç‚µ‹Ö~ŠÔ
		ply_work->nudge_timer	= 0;					//!< —h‚ç‚µŠÔ
		ply_work->nudge_ofst_x	= 0;					//!< —h‚ç‚µƒIƒtƒZƒbƒg—Ê

		// ƒQ[ƒ€ƒ^ƒCƒ}[ŠJn
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_COUNT_GAME_TIME;

		// ƒXƒ^[ƒgƒfƒ‚I—¹
		g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_START_DEMO;

		// à–¾ƒƒbƒZ[ƒWƒ`ƒFƒbƒN
		if (GmStartMsgIsExe()) {	// 20091128 Ishizaki
			GmStartMsgInit();
		}

		mtTaskChangeTcbProcedure(tcb, gmSplStagePlayEndChk);
	}

}

// ==========================================================================
// gmSplStagePlayEndChk
/*!
 * ƒvƒŒƒCI—¹ƒ`ƒFƒbƒN
 */
// ==========================================================================
void gmSplStagePlayEndChk( MTS_TASK_TCB* tcb )
{
	GMS_SPL_STG_WORK*	tcb_work = (GMS_SPL_STG_WORK*)mtTaskGetTcbWork( tcb );
	GMS_PLAYER_WORK*	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	OBS_CAMERA*			obj_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);

	amAssert( tcb_work );

	// ƒ|[ƒYƒ`ƒFƒbƒN
	if (ObjObjectPauseCheck(0)) return;

	// ƒ‰ƒCƒg§Œä
	gmSplStageLightCtrl(tcb_work);

	// •Ç’µ‚Ë•Ô‚èŠÖ˜A
	tcb_work->flag &= ~GMD_SPL_STAGE_BOUNCE_HIT;						// •Ç’µ‚Ë•Ô‚èî•ñ‚ğ–ˆ‰ñƒNƒŠƒA
	
	// —h‚ç‚µ§Œä
	gmSplStageNudgeCtrl();

	// ƒŠƒ“ƒOƒQ[ƒgæ“¾–‡”ŠÇ—
	gmSplStageRingGateChk(tcb_work);
	
	if (g_gm_main_system.game_flag & (  GMD_GAME_FLAG_SPL_CHAOSGET
									  | GMD_GAME_FLAG_SPL_FAILED
									  | GMD_GAME_FLAG_SPL_TIMEOVER ) ) {

		// I—¹‚ÆŒ©‚È‚·
		g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_COUNT_GAME_TIME;	// ƒQ[ƒ€ƒ^ƒCƒ}’â~ 

		// ƒvƒŒƒCƒ„[’â~
		ply_work->obj_work.flag |= (OBD_OBJECT_NOFUNC | OBD_OBJECT_NOHIT);
		ply_work->obj_work.move_flag |= (OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);
		ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;				// ‰ñ“]ƒ\ƒjƒbƒN‚Éˆá˜aŠ´¶‚¶‚é‚½‚ß”½“]OFF
		// ƒvƒŒƒCƒ„[ƒL[“ü—Í–³‹ƒtƒ‰ƒOİ’è
		ply_work->player_flag |= GMD_PLF_NOKEY;

		if (g_gm_main_system.game_flag & (  GMD_GAME_FLAG_SPL_FAILED			// ¸”s‚Ì‚İ
										  | GMD_GAME_FLAG_SPL_TIMEOVER ) ) {	// ƒRƒ“ƒgƒ[ƒ‰U“®
			// ƒRƒ“ƒgƒ[ƒ‰[U“®
			GMM_PAD_VIB_MID_TIME(GMD_SPL_STAGE_ROLL_FADEWAIT);
		}

		// ƒJƒƒ‰î•ñ•Û
#if _PC | _PS3 | _WII | _XBOX
		tcb_work->roll = obj_camera->roll;								// Œ»İ‰ñ“]Šp“x•Û

#elif _IPHONE
#ifdef GMD_MAIN_USE_BODY_ROTATE
		tcb_work->roll = obj_camera->roll;								// Œ»İ‰ñ“]Šp“x•Û
#else
#if 1
		tcb_work->roll = obj_camera->roll;								// Œ»İ‚Ì‰ñ“]Šp“x•Û
#else
		if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK)
		{
			tcb_work->roll = obj_camera->roll;								// Œ»İ‰ñ“]Šp“x•Û
		}
		else 
		{
			tcb_work->roll = 0;
			obj_camera->roll = 0;
			obj_camera->flag &= ~OBD_CAMERA_ROT_OFF;
		}
#endif // 0
#endif // GMD_MAIN_USE_BODY_ROTATE
#endif
		tcb_work->roll_spd = GMD_SPL_STAGE_ROLL_SPD_INIT;
		tcb_work->counter  = 0;

		mtTaskChangeTcbProcedure(tcb, gmSplStageRolling);
	}
}

// ==========================================================================
// gmSplStageRolling
/*!
 * ‰æ–Ê‰ñ“]`ƒtƒF[ƒhI—¹
 */
// ==========================================================================
void gmSplStageRolling( MTS_TASK_TCB* tcb )
{
	GMS_SPL_STG_WORK *tcb_work = (GMS_SPL_STG_WORK*)mtTaskGetTcbWork( tcb );
	OBS_CAMERA		*obj_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);
	amAssert( tcb_work );

	// ƒ|[ƒYƒ`ƒFƒbƒN
	if (ObjObjectPauseCheck(0)) return;
	if (!obj_camera) return;

	// ‰æ–Ê‰ñ“]‚µ‘±‚¯‚é
	tcb_work->roll_spd += GMD_SPL_STAGE_ROLL_SPD_ACL;
	tcb_work->roll += tcb_work->roll_spd;
	obj_camera->roll = tcb_work->roll;
	
	tcb_work->counter++;
	if (tcb_work->counter == GMD_SPL_STAGE_ROLL_FADEWAIT) {
		// ƒtƒF[ƒhŠJn
		IzFadeInitEasy(
				IZE_FADE_SET_TYPE_NORMAL,
				IZE_FADE_TYPE_WHITE_FADEOUT,
				GMD_SPL_STAGE_FADEIN_TIME );
	}
	if (tcb_work->counter > GMD_SPL_STAGE_ROLL_FADEWAIT) {
		if (IzFadeIsEnd()) {
			// ƒtƒF[ƒhI—¹

			IzFadeExit();
			IzFadeRestoreDrawSetting();
			
			// Œ»İ•\¦‚³‚ê‚Ä‚¢‚éOBJ‚ğ‘S‚Ä•\¦OFF
			GmObjSetAllObjectNoDisp();										// Obj OFF
			GmRingGetWork()->flag |= GMD_RING_SYS_FLAG_NODISP;				// Ring OFF
			GmFixSetDisp(FALSE);											// Fix OFF

			tcb_work->flag |= GMD_SPL_STAGE_RESULT;							// ƒŠƒUƒ‹ƒgˆÚs‚ğ’Ê’m—ƒ^ƒXƒNÁ‹
			tcb_work->counter = 1;											// ƒ^ƒXƒNÁ‹ŠÔ‚ÉŠ|‚©‚éŠÔƒEƒFƒCƒg
			
			mtTaskChangeTcbProcedure(tcb, gmSplStageGotoEnd);
			obj_camera->roll = 0;
			g_gm_main_system.pseudofall_dir = 0;
		}
	}

}

// ==========================================================================
// gmSplStageFadeOutWait
/*!
 * I—¹‚ÖˆÚs
 */
// ==========================================================================
void gmSplStageGotoEnd( MTS_TASK_TCB* tcb )
{
	GMS_SPL_STG_WORK *tcb_work = (GMS_SPL_STG_WORK*)mtTaskGetTcbWork( tcb );

	if (tcb_work->counter) {
		tcb_work->counter--;
	} else {
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_CLEAR;				// ƒXƒe[ƒWƒNƒŠƒA
		mtTaskChangeTcbProcedure(tcb, gmSplStageEnd);
	}
/*
	// ƒJƒIƒXƒGƒƒ‰ƒ‹ƒhæ“¾
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPL_CHAOSGET)
	// GOALÚG
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPL_FAILED)
	// ƒ^ƒCƒ€ƒI[ƒo[
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPL_TIMEOVER)
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_CLEAR;		// ƒXƒe[ƒWƒNƒŠƒA
*/
}
// ==========================================================================
// gmSplStageFadeOutWait
/*!
 * I—¹‘Ò‹@
 */
// ==========================================================================
void gmSplStageEnd( MTS_TASK_TCB* tcb )
{
	UNREFERENCED_PARAMETER(tcb);
}

// ==========================================================================
// gmSplStageLightCtrl
/*!
 * ƒ‰ƒCƒg§Œä
 *
 *	@param	tcb_work	[in]	ƒXƒyƒXƒetcbƒ[ƒN
 */
// ==========================================================================
void gmSplStageLightCtrl(GMS_SPL_STG_WORK *tcb_work)
{
	NNS_RGBA	light_col = {
		1.0f, 1.0f, 1.0f, 1.0f,
	};
	NNS_VECTOR	light_vec;

#if 0//defined(MTD_DEBUG)
// ƒ‰ƒCƒgˆÊ’uƒGƒfƒBƒbƒgiƒfƒoƒbƒO‹@”\j
	// ƒJ[ƒ\ƒ‹ã‰º
	if (AoPadRepeat() & KEY_L_UP)	tcb_work->dbg_cursor--;
	if (AoPadRepeat() & KEY_L_DOWN)	tcb_work->dbg_cursor++;
	tcb_work->dbg_cursor = MTM_MATH_CLIP(tcb_work->dbg_cursor, 0, 2);

	// ”’lƒŠƒZƒbƒg
	if ((AoPadDirect() & KEY_R1) && (AoPadDirect() & KEY_L1)) {
		tcb_work->light_vec.x = GMD_SPL_STG_BASE_LIGHT_POS_X;
		tcb_work->light_vec.y = GMD_SPL_STG_BASE_LIGHT_POS_Y;
		tcb_work->light_vec.z = GMD_SPL_STG_BASE_LIGHT_POS_Z;
	}
	// ƒJ[ƒ\ƒ‹¶‰E
	if (AoPadRepeat() & KEY_R1) {
		switch(tcb_work->dbg_cursor) {
			case 0: tcb_work->light_vec.x += 0.1f; break;
			case 1: tcb_work->light_vec.y += 0.1f; break;
			case 2: tcb_work->light_vec.z += 0.1f; break;
		}
	}
	if (AoPadRepeat() & KEY_L1) {
		switch(tcb_work->dbg_cursor) {
			case 0: tcb_work->light_vec.x -= 0.1f; break;
			case 1: tcb_work->light_vec.y -= 0.1f; break;
			case 2: tcb_work->light_vec.z -= 0.1f; break;
		}
	}
	nnNormalizeVector(&light_vec, &tcb_work->light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_0, &light_col, 1.f, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_1, &light_col, 1.f, &light_vec);

#if defined(MTD_DEBUG)
	// ƒfƒoƒbƒO•\¦(ƒpƒ‰ƒŒƒ‹ƒ‰ƒCƒgˆÊ’u)
	if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_DEBUG_DISP) {
		((tcb_work->dbg_cursor == 0) ? amPrintColor(0xffffffff) : amPrintColor(0xff7f7f7f));
		amPrintf(16, 10, "light x : %f", tcb_work->light_vec.x);

		((tcb_work->dbg_cursor == 1) ? amPrintColor(0xffffffff) : amPrintColor(0xff7f7f7f));
		amPrintf(16, 11, "light y : %f", tcb_work->light_vec.y);

		((tcb_work->dbg_cursor == 2) ? amPrintColor(0xffffffff) : amPrintColor(0xff7f7f7f));
		amPrintf(16, 12, "light z : %f", tcb_work->light_vec.z);

		amPrintColor(0xffffffff);
	}
#endif//defined(MTD_DEBUG)
#else//defined(MTD_DEBUG)
	UNREFERENCED_PARAMETER(tcb_work);
// ƒ‰ƒCƒgˆÊ’u‚ğ‰ñ“]‚É‰‚¶‚Ä•â³i”ñƒfƒoƒbƒO ƒQ[ƒ€—pˆ—j
	// Šî–{ƒ‰ƒCƒg
	light_vec = gmSplStageLightRot(GMD_SPL_STG_BASE_LIGHT_POS_X,			// ‰ñ“]‚É‰‚¶‚½ƒ‰ƒCƒgˆÊ’u•â³
								   GMD_SPL_STG_BASE_LIGHT_POS_Y,
								   GMD_SPL_STG_BASE_LIGHT_POS_Z);
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_0, &light_col, 1.f, &light_vec);
	// Šg’£ƒ‰ƒCƒg
	light_vec = gmSplStageLightRot(GMD_SPL_STG_OPT1_LIGHT_POS_X,			// ‰ñ“]‚É‰‚¶‚½ƒ‰ƒCƒgˆÊ’u•â³
								   GMD_SPL_STG_OPT1_LIGHT_POS_Y,
								   GMD_SPL_STG_OPT1_LIGHT_POS_Z);
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_1, &light_col, 1.f, &light_vec);
#endif//defined(MTD_DEBUG)
#if 0//defined(MTD_DEBUG)
	// ƒfƒoƒbƒO•\¦(ƒpƒ‰ƒŒƒ‹ƒ‰ƒCƒgˆÊ’u)
	if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_DEBUG_DISP) {
		amPrintf(16, 10, "light x : %f", light_vec.x);
		amPrintf(16, 11, "light y : %f", light_vec.y);
		amPrintf(16, 12, "light z : %f", light_vec.z);
	}
#endif//defined(MTD_DEBUG)
}

// ==========================================================================
// gmSplStageLightRot
/*!
 * ƒ‰ƒCƒg•ÀsŒõŒ¹‚ğAd—Í‰ñ“]‚É‡‚í‚¹‚Ä‰ñ‚·
 *
 *	@param	pos_x		[in]	ƒ‰ƒCƒgÀ•WX
 *	@param	pos_y		[in]	ƒ‰ƒCƒgÀ•WY
 *	@return						•â³Œã‚Ìƒ‰ƒCƒgˆÊ’u
 *
 *	NOTE:
 *		‚yˆÊ’u‚Í•ÏX‚È‚µ
 */
// ==========================================================================
NNS_VECTOR gmSplStageLightRot(float pos_x, float pos_y, float pos_z)
{
	NNS_VECTOR	light_vec;
	float x_sin, x_cos, y_sin, y_cos;

	x_sin = pos_x * nnSin(-g_gm_main_system.pseudofall_dir);
	x_cos = pos_x * nnCos(-g_gm_main_system.pseudofall_dir);
	y_sin = pos_y * nnSin(-g_gm_main_system.pseudofall_dir);
	y_cos = pos_y * nnCos(-g_gm_main_system.pseudofall_dir);

	light_vec.x = x_cos - y_sin;
	light_vec.y = x_sin + y_cos;
	light_vec.z = pos_z;

	return light_vec;
}

// ==========================================================================
// gmSplStageNudgeCtrl
/*!
 * ƒiƒbƒWƒ“ƒO‚Ì‰æ–Ê—h‚ç‚µ•\¦ƒIƒtƒZƒbƒg§Œä
 */
// ==========================================================================
void gmSplStageNudgeCtrl(void)
{
	GMS_PLAYER_WORK*	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	OBS_CAMERA*			obj_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);

	// ƒiƒbƒWƒ“ƒO(—h‚ç‚µ)ŠÇ—
	if (ply_work->nudge_timer) {
		fx32 ofst_x, ofst_y;
		ofst_x = (ply_work->nudge_timer * GMD_PL_SS_NUDGING_WIDTH << FX32_SHIFT) / GMD_PL_SS_NUDGING_TIME;
		if (ply_work->nudge_timer & 2) {
			// ‚S‰ñ‚É‚Q‰ñ‚Í•„†”½“]
			ofst_x = 0 - ofst_x;
		}
		ofst_y = 0;

#if 0	// ƒ\ƒjƒbƒNˆÊ’u‚ğÀÛ‚ÉƒIƒtƒZƒbƒg‚·‚éê‡iƒIƒtƒZƒbƒg—Ê(3-4ˆÈãH)‚É‚æ‚èƒRƒŠƒWƒ‡ƒ“”²‚¯‚ª”­¶‚µ‚Ü‚·j
		// ƒvƒŒƒCƒ„[ƒIƒtƒZƒbƒg
		fx32 ply_x, ply_y;
		ply_x = ofst_x - ply_work->nudge_ofst_x;
		ply_y = ofst_y;
		GmObjGetRotPosXY(ply_x, ply_y,
						&ply_x, &ply_y,
						g_gm_main_system.pseudofall_dir);
		ply_work->obj_work.pos.x += ply_x;
		ply_work->obj_work.pos.y += ply_y;
		ply_work->nudge_ofst_x = ofst_x;
#else	// ƒ\ƒjƒbƒNÀÛˆÊ’u‚Í•s•ÏB•\¦‚Ì‚İƒIƒtƒZƒbƒg‚·‚éê‡
		// ƒvƒŒƒCƒ„[ƒIƒtƒZƒbƒg
		GmObjGetRotPosXY(ofst_x, ofst_y,
						&ply_work->obj_work.ofst.x, &ply_work->obj_work.ofst.y,
						g_gm_main_system.pseudofall_dir);

		// ƒJƒƒ‰ƒIƒtƒZƒbƒg
		GmObjGetRotPosXY(ofst_x, ofst_y,
						&ofst_x, &ofst_y,
						(u16)(-g_gm_main_system.pseudofall_dir));
		obj_camera->ofst.x = FXM_FX32_TO_FLOAT(ofst_x);
		obj_camera->ofst.y = FXM_FX32_TO_FLOAT(ofst_y);
#endif

		ply_work->nudge_timer--;
	} else {
		obj_camera->ofst.x = 0;
        obj_camera->ofst.y = 0;
		ply_work->obj_work.ofst.x = 0;
		ply_work->obj_work.ofst.y = 0;
		ply_work->nudge_ofst_x = 0;
	}
}

// ==========================================================================
// gmSplStageRingGateChk
/*!
 *	ƒŠƒ“ƒOƒQ[ƒgæ“¾–‡”ƒ`ƒFƒbƒN
 */
// ==========================================================================
void gmSplStageRingGateChk(GMS_SPL_STG_WORK *tcb_work)
{
	GMS_PLAYER_WORK *ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	u16	ring_base = tcb_work->get_ring;
	u16	i;

	for (i = 0; i < GMD_SS_RINGGATE_MAX; i++) {
		if (!(tcb_work->get_ring & (1 << i))) {
			// ’Ê‰ß‹K’è–‡”ˆÈ‰º‚ÌƒQ[ƒg–‡”ƒ`ƒFƒbƒN
			u16	ring_num = GmSplStageRingGateNumGet(i);
			if (ring_num == 0xff) {
				// –³Œø‚ÈID‚Ìê‡ƒ`ƒFƒbƒNI—¹
				break;
			}
			if (ring_num <= (u16)(ply_work->ring_num)) {
				// ‹K’è–‡”ƒNƒŠƒA
				tcb_work->get_ring |= (1 << i);
			}
		}
	}

	if (ring_base != tcb_work->get_ring) {
		// ƒŠƒ“ƒOƒQ[ƒg’Ê‰ßó‹µ‚ª•Ï‚í‚Á‚½‚çSE”­s
		GmSoundPlaySE("Special7");								// SE
	}
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
