// ==========================================================================
/*!
  @file gmEnding.cpp
  @brief ƒGƒ“ƒfƒBƒ“ƒOŠÖ˜A

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmEnding.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gs.h"
#include "objObject.h"
#include "gmMainDat.h"
#include "gmEnemy.h"
#include "gmEventTbl.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"
#include "gmFix.h"
#include "izFade.h"
#include "gmPlySeq.h"
#include "gmPlySeqGmk.h"
#include "gmCamera.h"
#include "gmEffect.h"
#include "hgTrophy.h"
#include "gmSound.h"

#include "gmEnding.h"

#define GMD_ENDING_CUTIN_ON			// ƒGƒ“ƒfƒBƒ“ƒOƒ‰ƒXƒgƒJƒbƒg ƒf[ƒ^‚ª—ˆ‚é‚Ü‚Å‚n‚e‚e‚É‚Å‚«‚é‚æ‚¤define’è‹`

#if _WII  | _IPHONE
#ifdef GMD_ENDING_CUTIN_ON
// ƒf[ƒ^ƒwƒbƒ_
#include "Wii/model/END_SONIC_MDL.HMB"
#endif//GMD_ENDING_CUTIN_ON
#endif//_WII  | _IPHONE


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------


//GMS_ENDING_WORK::step (isó‹µƒXƒeƒbƒv)
typedef enum tag_GME_END_STEP {
	GME_END_STEP_INIT = 0,				// ‰Šú‰»
	GME_END_STEP_GAME,					// ƒQ[ƒ€ƒvƒŒƒC(ƒ\ƒjƒbƒNJUMP‚Ì‚İ—LŒø)
	GME_END_STEP_NOP,					// ƒ\ƒjƒbƒN‘€ì‹Ö~
	GME_END_STEP_BRAKE,					// ƒ\ƒjƒbƒNƒuƒŒ[ƒL
	GME_END_STEP_WAIT,					// ƒ\ƒjƒbƒN‘Ò‹@
	GME_END_STEP_MUTATE,				// ƒX[ƒp[ƒ\ƒjƒbƒN•Ïg(‘SƒJƒIƒXƒGƒƒ‰ƒ‹ƒh•‘SƒŠƒ“ƒOæ“¾‚Ì‚İ)
	GME_END_STEP_JUMP,					// ƒ\ƒjƒbƒNU‚èŒü‚«•ƒWƒƒƒ“ƒv‰‰o
	GME_END_STEP_STOP,					// ƒ\ƒjƒbƒN’â~•‘Ò‹@
	GME_END_STEP_FADEOUT,				// I—¹ƒtƒF[ƒhƒAƒEƒg
	
	GME_END_STEP_MAX,
} GME_END_STEP;

//GMS_ENDING_WORK::type
typedef enum tag_GME_END_TYPE {
	GME_END_TYPE_NORM1 = 0,				// ’Êíƒ\ƒjƒbƒN‚P
	GME_END_TYPE_NORM2,					// ’Êíƒ\ƒjƒbƒN‚Qi‚¿‚å‚Á‚Æ‹‰Øj
	GME_END_TYPE_SUPER,					// ƒX[ƒp[ƒ\ƒjƒbƒN
} GME_END_TYPE;


//ƒGƒ“ƒfƒBƒ“ƒOƒƒCƒ“ƒ[ƒN
typedef struct tag_GMS_ENDING_WORK {	
	GME_END_STEP step;				// isƒXƒeƒbƒv
	GME_END_TYPE type;				// ƒGƒ“ƒfƒBƒ“ƒO‰‰oƒ^ƒCƒv
	u32 		flag;				// §Œäƒtƒ‰ƒO
	u32			get_ring;			// ÅI”»’è‚ÌƒŠƒ“ƒOæ“¾”
	u32			timer;				// ”Ä—pƒ^ƒCƒ}[(HBM‘Îô)

} GMS_ENDING_WORK;

//GMS_ENDING_WORK::flag
#define GMD_END_FLAG_SON_NOMOVE	(1 <<  0)	// ƒ\ƒjƒbƒN‘€ì•s‰Â	iƒ†[ƒU[‘€ì‚Ì–³Œøj
#define GMD_END_FLAG_SON_ACCEL	(1 <<  1)	// ƒ\ƒjƒbƒN¶ˆÚ“®	iƒGƒ“ƒfƒBƒ“ƒOŠÇ—‚É‚æ‚é‘€ìj
#define GMD_END_FLAG_SON_BRAKE	(1 <<  2)	// ƒ\ƒjƒbƒNƒuƒŒ[ƒL	iƒGƒ“ƒfƒBƒ“ƒOŠÇ—‚É‚æ‚é‘€ìj

#define GMD_END_SON_BRAKE_RELEASE_SPD	(-0x1000)	// ƒ\ƒjƒbƒN‚ªƒuƒŒ[ƒL‰ğœ‚·‚éspd_m‚Ì’l
#define GMD_END_SON_FRONTSIDE_TIME		(720)		// ƒ\ƒjƒbƒN‚ªè‘O‚ğŒü‚«n‚ß‚éŠÔ
#define GMD_END_SON_FINISHJUMP_TIME		(900)		// ƒ\ƒjƒbƒN‚ªƒtƒBƒjƒbƒVƒ…ƒWƒƒƒ“ƒv‚·‚éŠÔ
#define GMD_END_SON_FADEOUT_TIME		(1140)		// ƒtƒF[ƒhƒAƒEƒgŠJnŠÔ
#define GMD_END_SCR_LMT_BOTTOM_DEC		(32)		// ƒXƒNƒ[ƒ‹§ŒÀ(bottom)‚ğk‚ß‚é—Ê
#define GMD_END_START_WAIT				(16)		// HBM‘ÎôFAMD_WII_HOMEMENU_DELAY_IN ˆÈã‚Ì’l‚ğİ’è

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmEndingCtrl(MTS_TASK_TCB* tcb);
static GMS_ENDING_WORK* gmEndingGetWork(void);
static void gmEndingLastPicInit(void);

#if _WII  | _IPHONE
#ifdef GMD_ENDING_CUTIN_ON
static void gmEndingLastPic(OBS_OBJECT_WORK *obj_work);
#endif//GMD_ENDING_CUTIN_ON
#endif//_WII  | _IPHONE
//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static MTS_TASK_TCB* gm_ending_tcb = NULL;

#if _WII  | _IPHONE
#ifdef GMD_ENDING_CUTIN_ON
static OBS_ACTION3D_NN_WORK *gm_ending_obj_3d_list = NULL;

static const fx32 gm_ending_obj_offset[] = {
	0,
	(-8*FX32_ONE),
	(-8*FX32_ONE),
};
#endif//GMD_ENDING_CUTIN_ON
#endif//_WII  | _IPHONE

//----- Global Functions ----------------------------------------------------

// ==========================================================================
// GmEndingBuild
/*!
 *	ƒGƒ“ƒfƒBƒ“ƒOê—pƒf[ƒ^\’z
 */
// ==========================================================================
void GmEndingBuild(void)
{
#if _WII  | _IPHONE
#ifdef GMD_ENDING_CUTIN_ON
	gm_ending_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_END_SONIC_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_END_SONIC_TEX),
								0/*draw_flag*/);
#endif//GMD_ENDING_CUTIN_ON
#endif //_WII  | _IPHONE
}

// ==========================================================================
// GmEndingFlush
/*!
 *	ƒGƒ“ƒfƒBƒ“ƒOê—pƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
void GmEndingFlush(void)
{
#if _WII  | _IPHONE
#ifdef GMD_ENDING_CUTIN_ON
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_END_SONIC_MODEL);
	GmGameDBuildRegFlushModel(gm_ending_obj_3d_list, amb->file_num);
#endif//GMD_ENDING_CUTIN_ON
#endif //_WII  | _IPHONE
}

// ==========================================================================
// GmEndingStart
/*!
 *	ƒGƒ“ƒfƒBƒ“ƒO‚Ì‰Šú‰»AŠÇ—ŠJn
 */
// ==========================================================================
void GmEndingStart(void)
{
	GMS_ENDING_WORK* end_work;

	// ƒGƒ“ƒfƒBƒ“ƒO ƒQ[ƒ€ƒ^ƒCƒ}[ŠJn
	g_gm_main_system.game_flag |= GMD_GAME_FLAG_ENDING;

	// Fix OFF
#if !_IPHONE
	GmFixSetDisp(FALSE);
#else //!_IPHONE
	GmFixSetDispEx(FALSE, FALSE, FALSE, TRUE, FALSE);	//ƒAƒNƒVƒ‡ƒ“ƒ{ƒ^ƒ“‚Íc‚·
#endif //!_IPHONE

	// ŠÇ—ƒ^ƒXƒNì¬
	gm_ending_tcb = MTM_TASK_MAKE_TCB(gmEndingCtrl,
									NULL,
									0,
									GMD_TASK_PAUSELEVEL_DEF,
									GMD_TASK_PRIO_STARTDEMO,
									GMD_TASK_GROUP_STARTDEMO,
									sizeof( GMS_ENDING_WORK ),
									"ENDING_CTRL" );

	end_work = (GMS_ENDING_WORK*)mtTaskGetTcbWork(gm_ending_tcb);
	end_work->step = GME_END_STEP_INIT;
	end_work->flag = GMD_END_FLAG_SON_NOMOVE;

#if _WII
	// HBM‹Ö~
	amWiiSetEnableHBM(0);
#endif
	end_work->timer = GMD_END_START_WAIT;								// AMD_WII_HOMEMENU_DELAY_IN ˆÈã‚Ì’l‚ğİ’è

	// ƒXƒNƒ[ƒ‹—V‚Ñi¶‰E‚Íƒ[ƒj
	GmCameraAllowSet(0.0f, 50.0f, 0.0f);

	// ƒXƒNƒ[ƒ‹‰ºŒÀ‚ğİ’èi‘¦”½‰fj
	g_gm_main_system.map_fcol.bottom = g_gm_main_system.map_fcol.bottom - GMD_END_SCR_LMT_BOTTOM_DEC;

}


// ==========================================================================
// GmSplStageExit
/*!
	ƒXƒyƒVƒƒƒ‹ƒXƒe[ƒW‹­§I—¹ˆ—
 */
// ==========================================================================
void GmEndingExit(void)
{
	// ƒ^ƒXƒNƒNƒŠƒA
	if (gm_ending_tcb) {
		mtTaskClearTcb(gm_ending_tcb);

		g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_ENDING;
		gm_ending_tcb = NULL;
	}
}

// ==========================================================================
// GmEndingPlyKeyCustom
/*!
 *	ƒGƒ“ƒfƒBƒ“ƒO‚ÌƒvƒŒƒCƒ„[ƒL[‚ğ‰ü•Ï
 *
 * @param ply_work [in] ƒvƒŒƒCƒ„[ƒ[ƒNƒ|ƒCƒ“ƒ^
 */
// ==========================================================================
void GmEndingPlyKeyCustom(GMS_PLAYER_WORK *ply_work)
{
	GMS_ENDING_WORK* end_work = gmEndingGetWork();

	// ƒvƒŒƒCƒ„[‚É‚æ‚é“ü—Í‘€ì
	if (end_work->flag & GMD_END_FLAG_SON_NOMOVE) {
		// “ü—Í–³Œø
		ply_work->key_on	= 0;
		ply_work->key_push	= 0;
		ply_work->key_release = 0;
	} else {
		// ƒWƒƒƒ“ƒv“ü—Í—LŒø
		ply_work->key_on	&= PAD_BUTTON_JUMP;
		ply_work->key_push	&= PAD_BUTTON_JUMP;
		ply_work->key_release &= PAD_BUTTON_JUMP;
	}
	ply_work->key_rot_z = 
	ply_work->key_walk_rot_z = 0;

	// ƒGƒ“ƒfƒBƒ“ƒOŠÇ—‚É‚æ‚éƒL[‘€ì
	if (end_work->flag & GMD_END_FLAG_SON_ACCEL) {
		// ¶“ü—Í‚Åƒ\ƒjƒbƒNˆÚ“®
		ply_work->key_on	|= PAD_KEY_LEFT;
		ply_work->key_rot_z = 
		ply_work->key_walk_rot_z = -0x7FFF;
	} else if (end_work->flag & GMD_END_FLAG_SON_BRAKE) {
		// ‰E“ü—Í‚Åƒ\ƒjƒbƒNƒuƒŒ[ƒL
		ply_work->key_on	|= PAD_KEY_RIGHT;
		ply_work->key_rot_z = 
		ply_work->key_walk_rot_z = 0x7FFF;
	} else {
		// ‰½‚à‘€ì–³‚µ
	}
}

// ==========================================================================
// GmEndingPlyNopSet
/*!
 *	ƒGƒ“ƒfƒBƒ“ƒO ƒvƒŒƒCƒ„[NoOperation§Œä‚ğƒZƒbƒg
 */
// ==========================================================================
void GmEndingPlyNopSet(void)
{
	GMS_ENDING_WORK* end_work = gmEndingGetWork();

	if (end_work->step == GME_END_STEP_GAME) {
		end_work->step = GME_END_STEP_NOP;									// ƒ\ƒjƒbƒN‘€ì‹Ö~

		// ƒuƒŒ[ƒLI—¹Œã‚ÌˆÊ’u’²®‚Ì‚½‚ßƒuƒŒ[ƒLŠJn‚Ì‘¬“x‚ğw’è
		//g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_work.pos.x = 0x3c7000;
		g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_work.spd_m = (-0x9000);
	}
}
// ==========================================================================
// GmEndingPlyBrakeSet
/*!
 *	ƒGƒ“ƒfƒBƒ“ƒO ƒvƒŒƒCƒ„[ƒuƒŒ[ƒL§Œä‚ğƒZƒbƒg
 */
// ==========================================================================
void GmEndingPlyBrakeSet(void)
{
	GMS_ENDING_WORK* end_work = gmEndingGetWork();

	if (end_work->step == GME_END_STEP_NOP) {
		end_work->step = GME_END_STEP_BRAKE;								// ƒ\ƒjƒbƒNƒuƒŒ[ƒL

		// ƒuƒŒ[ƒLI—¹Œã‚ÌˆÊ’u’²®‚Ì‚½‚ßƒuƒŒ[ƒLŠJn‚Ì‘¬“x‚ğw’è
		g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_work.pos.x = 0x1fc000;
		g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_work.spd_m = (-0x9000);
	}
}

// ==========================================================================
// GmEndingAnimalForwardChk
/*!
 *	ƒGƒ“ƒfƒBƒ“ƒO “®•¨‘OŒü‚«ƒ`ƒFƒbƒN
 */
// ==========================================================================
BOOL GmEndingAnimalForwardChk(void)
{
	GMS_ENDING_WORK* end_work = gmEndingGetWork();

	if (  (end_work->type != GME_END_TYPE_NORM1)
		&&(end_work->step >= GME_END_STEP_STOP) ) {
		return TRUE;
	}
	return FALSE;
}

// ==========================================================================
// GmEndingTrophySet
/*!
 *	ƒGƒ“ƒfƒBƒ“ƒO ƒgƒƒtƒB[æ“¾ƒZƒbƒg
 */
// ==========================================================================
void GmEndingTrophySet(void)
{
	// ƒŠƒ“ƒO‘Sæ“¾‚ÅƒgƒƒtƒB[ƒZƒbƒg
	GMS_ENDING_WORK* end_work = gmEndingGetWork();

	if (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]) { 
	  GsGetMainSysInfo()->clear_ring = end_work->get_ring;
	} 
	else { 
	  GsGetMainSysInfo()->clear_ring = 0; 
	} 
	 
	HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING_CLEAR_ENDING_STAGE);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmEndingCtrl
/*!
 *	ƒGƒ“ƒfƒBƒ“ƒOŠÇ—
 *
 * @param tcb 	[in] TCBƒ[ƒN
 */
// ==========================================================================
void gmEndingCtrl(MTS_TASK_TCB* tcb)
{
	GMS_ENDING_WORK* end_work = (GMS_ENDING_WORK*)mtTaskGetTcbWork(tcb);
	GMS_PLAYER_WORK* ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	switch (end_work->step) {
		case GME_END_STEP_INIT:
			// ’¼‘O‚ÉHBMŒÄ‚Î‚ê‚½‚Ì‘Îô‚Æ‚µ‚Ä­‚µ‘Ò‚Â
			// (‘Îô•K—v«‚ª‚ ‚é‚Ì‚ÍWii‚Ì‚İ‚¾‚ªAŠePF“ˆê‚Ì‚½‚ß‹¤’Êˆ—‚Æ‚·‚é)
			if (end_work->timer) {
				end_work->timer--;
				break;
			}
#if _WII
			// HBM•\¦’†‚Í‘Ò‹@
			if (amWiiIsDrawHBM()) {
				break;
			}
#endif
			// ƒ^ƒCƒ}[ƒJƒEƒ“ƒgŠJn
			g_gm_main_system.game_flag |= GMD_GAME_FLAG_COUNT_GAME_TIME;

			// ƒtƒF[ƒhƒCƒ“ŠJn
			IzFadeInitEasy(IZE_FADE_SET_TYPE_TAKEOEVER,
							IZE_FADE_TYPE_WHITE_FADEIN,
							60.f);
			end_work->step = GME_END_STEP_GAME;
			end_work->flag = GMD_END_FLAG_SON_ACCEL;

			// BGMÄ¶
			GmSoundPlayStageBGM(0);
			break;
			
		case GME_END_STEP_GAME:
		default:
			break;

		case GME_END_STEP_NOP:		// ƒ\ƒjƒbƒN‘€ì–³‚µ
			// ‘€ì–³Œø‚ğƒZƒbƒg
			end_work->flag |= GMD_END_FLAG_SON_NOMOVE;
			ply_work->spd_jump_add = ply_work->spd_add;	// ‹ó’†‚ÅƒuƒŒ[ƒL‰E“ü—Í“ü‚Á‚Ä‚µ‚Ü‚Á‚½‚ÉU‚èŒü‚¢‚Ä‚µ‚Ü‚¤‘Îˆ
			break;
			
		case GME_END_STEP_BRAKE:	// ƒ\ƒjƒbƒNƒuƒŒ[ƒL
			ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;	// ‹ó’†‚ÅƒuƒŒ[ƒL‰E“ü—Í“ü‚Á‚Ä‚µ‚Ü‚Á‚½‚ÉU‚èŒü‚¢‚Ä‚µ‚Ü‚¤‘Îˆ
			if (  (ply_work->obj_work.move_flag & OBD_MOVE_UNDER)
				&&(ply_work->obj_work.spd_m > GMD_END_SON_BRAKE_RELEASE_SPD) ) {
#ifdef MTD_DEBUG// Debug‹@”\[360:‚`&‚a/PS3:›&~/Wii:‚P&‚Q]‚ğ‰Ÿ‚µ‚Ä‚¢‚½‚çƒŠƒ“ƒO‘Sæ“¾‚Ìˆµ‚¢‚Æ‚·‚é
#if _IPHONE
				// ƒ^ƒbƒ`‚Å“®ì‚ğ•ÏX
				if (amTpIsTouchOn(0)) {
					ply_work->ring_num = (s16)GmEventMgrGetRingNum();
					if (amTpIsTouchOn(1)) {
						g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD;
					}
					else {
						g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD;
					}
				}
#else
				if ((AoPadDirect() & (KEY_L_UP | KEY_R_DOWN | KEY_R_RIGHT)) == (KEY_L_UP | KEY_R_DOWN | KEY_R_RIGHT)) {
					ply_work->ring_num = (s16)GmEventMgrGetRingNum();
				}
#endif // _IPHONE
#endif			// Debug‹@”\[360:‚`&‚a/PS3:›&~/Wii:‚P&‚Q]‚ğ‰Ÿ‚µ‚Ä‚¢‚½‚çƒŠƒ“ƒO‘Sæ“¾‚Ìˆµ‚¢‚Æ‚·‚é

				// ƒuƒŒ[ƒLI—¹
				end_work->get_ring = (u32)ply_work->ring_num;
				if (end_work->get_ring < GmEventMgrGetRingNum()) {
					// ‹K’èƒmƒ‹ƒ}–¢–:’Êíƒ\ƒjƒbƒN‚P
					end_work->step = GME_END_STEP_WAIT;
					end_work->type = GME_END_TYPE_NORM1;
				} else { 
					// ‹K’èƒmƒ‹ƒ}”ƒNƒŠƒA
					if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD) {
						// ƒJƒIƒXƒGƒƒ‰ƒ‹ƒh‘Sæ“¾‚ÍƒX[ƒp[ƒ\ƒjƒbƒN
						end_work->step = GME_END_STEP_MUTATE;
						end_work->type = GME_END_TYPE_SUPER;
					} else {
						// ƒJƒIƒXƒGƒƒ‰ƒ‹ƒhnot‘Sæ“¾‚Í’Êíƒ\ƒjƒbƒN‚Q
						end_work->step = GME_END_STEP_WAIT;
						end_work->type = GME_END_TYPE_NORM2;
					}
				}
				end_work->flag &= ~GMD_END_FLAG_SON_BRAKE;
				gmEndingLastPicInit();										// ƒ\ƒjƒbƒN‚P–‡ŠG¶¬FWii(‚ÆiPhone)‚Ì‚İH
			} else {
				// ƒuƒŒ[ƒL‘±s
				end_work->flag &= ~GMD_END_FLAG_SON_ACCEL;
				end_work->flag |= GMD_END_FLAG_SON_BRAKE;
			}
			break;

		case GME_END_STEP_WAIT:		// è‘OU‚èŒü‚«‘Ò‚¿
			if (g_gm_main_system.game_time > GMD_END_SON_FRONTSIDE_TIME) {
				// è‘OU‚èŒü‚«ŠJn
				if (ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_ENDING_DEMO1) {
					GmPlySeqGmkInitEndingDemo1(ply_work);

					end_work->step = GME_END_STEP_JUMP;
				}
			}
			break;

		case GME_END_STEP_MUTATE:	// ƒX[ƒp[ƒ\ƒjƒbƒN•Ïg
			if (g_gm_main_system.game_time > GMD_END_SON_FRONTSIDE_TIME) {
				// è‘OU‚èŒü‚«ŠJn
				if (ply_work->seq_state != GME_PLY_SEQ_STATE_TRANS_SUPER) {
					 GmPlySeqChangeTransformSuper(ply_work);

					end_work->step = GME_END_STEP_JUMP;
				}
			}
			break;

		case GME_END_STEP_JUMP:		// è‘O‚ÉƒWƒƒƒ“ƒv(ƒtƒBƒjƒbƒVƒ…)
			if (g_gm_main_system.game_time > GMD_END_SON_FINISHJUMP_TIME) {
				if (ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_ENDING_DEMO2) {
					BOOL type2	= FALSE;
					if (end_work->type == GME_END_TYPE_NORM2) {
						// normal type2 ‚Ìê‡ƒtƒ‰ƒOƒZƒbƒg
						type2 = TRUE;
					}
					GmPlySeqGmkInitEndingDemo2(ply_work, type2);
					end_work->step = GME_END_STEP_STOP;
				}
			}
			break;
			
		case GME_END_STEP_STOP:		// ƒLƒƒ|[ƒY
			if (g_gm_main_system.game_time > GMD_END_SON_FADEOUT_TIME) {
				// ƒtƒF[ƒhƒAƒEƒgŠJn
				IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL,
								IZE_FADE_TYPE_BLACK_FADEOUT,
								32.0f);
				end_work->step = GME_END_STEP_FADEOUT;
			}
			break;

		case GME_END_STEP_FADEOUT:	// I—¹ƒtƒF[ƒhƒAƒEƒg
			if (IzFadeIsEnd()) {
				// I—¹ˆ—‚ğ‚±‚±‚ÅƒZƒbƒg‚µ‚Ü‚·B

#if _WII
				// HBM‹–‰Â
				amWiiSetEnableHBM(1);
#endif
				GmMainEnd();								// ƒ^ƒXƒNƒNƒŠƒA (ƒf[ƒ^‚ÍƒXƒ^ƒbƒtƒ[ƒ‹‚Ì‚½‚ß‚Éc‚·)
				SyDecideEvtCase(0);							// ƒCƒxƒ“ƒg‘JˆÚæƒZƒbƒg
				SyChangeNextEvt();							// ƒCƒxƒ“ƒg‘JˆÚ
			}
			break;
	}
}

// ==========================================================================
// gmEndingGetWork
/*!
 *	ƒGƒ“ƒfƒBƒ“ƒOŠÇ—ƒ[ƒNæ“¾
 *
 * @return	ƒGƒ“ƒfƒBƒ“ƒOŠÇ—ƒ[ƒN
 */
// ==========================================================================
GMS_ENDING_WORK* gmEndingGetWork(void)
{
	GMS_ENDING_WORK* end_work;
	MTM_ASSERT(gm_ending_tcb);

	end_work = (GMS_ENDING_WORK*)mtTaskGetTcbWork(gm_ending_tcb);
	return(end_work);
}

// ==========================================================================
// gmEndingLastPicInit
/*!
 *	ƒGƒ“ƒfƒBƒ“ƒO ƒ‰ƒXƒgƒJƒbƒg•\¦‰Šú‰»
 *
 */
// ==========================================================================
void gmEndingLastPicInit(void)
{
#if _WII  | _IPHONE
#ifdef GMD_ENDING_CUTIN_ON
	GMS_ENDING_WORK* 		end_work = gmEndingGetWork();
	GMS_PLAYER_WORK* 		ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	OBS_OBJECT_WORK			*obj_work;
	GMS_EFFECT_3DNN_WORK	*efct_work;

	obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_3DNN_WORK), &ply_work->obj_work/*parent_obj*/, 0/*sort_prio*/, "END_PIC");
	efct_work = (GMS_EFFECT_3DNN_WORK*)obj_work;
	obj_work->ppFunc = gmEndingLastPic;

	// ƒIƒuƒWƒFƒNƒg“Ç‚İ‚İ
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ending_obj_3d_list[end_work->type],
					&efct_work->obj_3d);
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL_MASK;
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_NODISP;
	obj_work->flag		|= OBD_OBJECT_NOHIT | OBD_OBJECT_PARENT_FIX;
	obj_work->scale.x = 
	obj_work->scale.y = 
	obj_work->scale.z = 0x00001400;

	// •\¦offset_Y
	obj_work->parent_ofst.y = gm_ending_obj_offset[end_work->type];

#endif//GMD_ENDING_CUTIN_ON
#endif//_WII  | _IPHONE
}

#if _WII  | _IPHONE
#ifdef GMD_ENDING_CUTIN_ON
// ==========================================================================
// gmEndingLastPic
/*!
 *	ƒGƒ“ƒfƒBƒ“ƒO ƒ‰ƒXƒgƒJƒbƒg•\¦
 *
 */
// ==========================================================================
void gmEndingLastPic(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK* ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	obj_work->disp_flag |= OBD_DISP_NODISP;
	if (  (ply_work->act_state == GME_PLY_ACT_STATE_GMK_ENDING_FN12)
	    ||(ply_work->act_state == GME_PLY_ACT_STATE_GMK_ENDING_FN22)
	    ||(ply_work->act_state == GME_PLY_ACT_STATE_GMK_ENDING_FNS2) ) {
		// •\¦ON
		obj_work->disp_flag &= ~OBD_DISP_NODISP;
#if _IPHONE
		ply_work->obj_work.disp_flag |= OBD_DISP_NODISP; // ƒvƒŒƒCƒ„[‚ğÁ‚·
#endif // _IPHONE
	}
}
#endif//GMD_ENDING_CUTIN_ON
#endif//_WII  | _IPHONE
//	// ==========================================================================
//	// _pt
//	/*!
//	 *	@param	tcb	[in]	TCB
//	 */
//	// ==========================================================================
