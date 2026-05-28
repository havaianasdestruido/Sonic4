// ==========================================================================
/*!
  @file gmBoss2.cpp
  @brief ƒ{ƒX2

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmBoss2.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 */


//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "akMath.h"
#include "gs.h"
#include "gmMain.h"
#include "gmGameDBuild.h"
#include "gmGamedat.h"
#include "gmEventTbl.h"
#include "gmCamera.h"
#include "gmSound.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectBoss.h"
#include "gmBossCommon.h"
#include "gmPlySeq.h"
#include "gmPlySeqGmk.h"
#include "gmPlyScoreDef.h"
#include "hgTrophy.h"
#include "gmPadVib.h"
#include "gmMap.h"

#include "gmGmkCamScrLim.h"
#include "gmGmkNeedleNeon.h"
#include "gmGmkShutter.h"

#if _IPHONE
#include "gmMap.h"
#endif // _IPHONE

#include "gmBoss2.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "../file/common/arc/BOSS02.hmb"
#include "../file/common/model/BOSS02_MDL.hmb"
#include "../file/common/model/BOSS02_BODY_MTN.hmb"
#include "../file/common/model/BOSS02_BODY_MAT.hmb"
#include "../file/common/model/BOSS02_EGG_MTN.hmb"

//----- Definitions ---------------------------------------------------------

#define _BOSS2_TEST_STOP_MOVE	(0)	//ƒeƒXƒg—pƒ{ƒXˆÚ“®’â~
#define _BOSS2_TEST_DEFEAT		(0)	//ƒeƒXƒg—pƒ{ƒX‘Ì—Í1

#define GMD_BOSS2_BODY_CATCH_RELEASE_TILT (0 & _IPHONE)	// ŒX‚«‚É‚æ‚éƒLƒƒƒbƒ`ó‘Ô‚©‚ç‚ÌƒŠƒŠ[ƒX

// =======================================================================
//‹¤’Ê
// =======================================================================
#define GMD_BOSS2_BLEND_SPD					((Float)0.125f)		//ƒ‚[ƒVƒ‡ƒ“ƒuƒŒƒ“ƒh‘¬“x

//ƒp[ƒcƒCƒ“ƒfƒbƒNƒX
enum GME_BOSS2_PART_IDX{
	GMD_BOSS2_PART_IDX_BODY	= 0,	//–{‘Ì
	GMD_BOSS2_PART_IDX_EGG,			//ƒGƒbƒOƒ}ƒ“
	
	GMD_BOSS2_PART_IDX_MAX
};

//ƒAƒNƒVƒ‡ƒ“ID
enum GME_BOSS2_ACT_ID{
	GMD_BOSS2_ACT_ID_START	= 0,			//ŠJn‰‰oi“oêj

	GMD_BOSS2_ACT_ID_CATCH_MOVE,			//ƒLƒƒƒbƒ`iˆÚ“®j
	GMD_BOSS2_ACT_ID_CATCH_ARM_OPEN,		//ƒLƒƒƒbƒ`iŠJ‚­j
	GMD_BOSS2_ACT_ID_CATCH_ARM_READY,		//ƒLƒƒƒbƒ`i€”õj
	GMD_BOSS2_ACT_ID_CATCH_ARM_DOWN,		//ƒLƒƒƒbƒ`i~‚ë‚·j
	GMD_BOSS2_ACT_ID_CATCH_ARM_CLOSE,		//ƒLƒƒƒbƒ`i•Â‚¶‚éj
	GMD_BOSS2_ACT_ID_CATCH_ARM_UP,			//ƒLƒƒƒbƒ`i‚ ‚°‚éj
	GMD_BOSS2_ACT_ID_CATCH_ARM_CARRY,		//ƒLƒƒƒbƒ`i‰^‚Ôj
	GMD_BOSS2_ACT_ID_CATCH_ARM_THROW,		//ƒLƒƒƒbƒ`i“Š‚°‚éj

	GMD_BOSS2_ACT_ID_BALL_MOVE,				//ƒgƒQƒ{[ƒ‹iˆÚ“®j
	GMD_BOSS2_ACT_ID_BALL_READY,			//ƒgƒQƒ{[ƒ‹i€”õj
	GMD_BOSS2_ACT_ID_BALL_SHOOT,			//ƒgƒQƒ{[ƒ‹iËoj

	GMD_BOSS2_ACT_ID_PINBALL_START,			//ƒsƒ“ƒ{[ƒ‹i“d‹CƒGƒtƒFƒNƒg”­¶‰‰oj
	GMD_BOSS2_ACT_ID_PINBALL_MOVE,			//ƒsƒ“ƒ{[ƒ‹iˆÚ“®j
	GMD_BOSS2_ACT_ID_PINBALL_MOVE_CLOSE,	//ƒsƒ“ƒ{[ƒ‹i‚Ó‚½•Â‚¶ˆÚ“®j
	GMD_BOSS2_ACT_ID_PINBALL_SEARCH,		//ƒsƒ“ƒ{[ƒ‹i’T‚·j
	GMD_BOSS2_ACT_ID_PINBALL_FIND,			//ƒsƒ“ƒ{[ƒ‹i”­Œ©j

	GMD_BOSS2_ACT_ID_ANGRY,					//“{‚é
	GMD_BOSS2_ACT_ID_ESCAPE,				//“¦–S

	GMD_BOSS2_ACT_ID_MAX
};
enum GME_BOSS2_EGG_ACT_ID{
	GMD_BOSS2_EGG_ACT_ID_LAUGH_L = 0,	//Î‚¢
	GMD_BOSS2_EGG_ACT_ID_LAUGH_R,		//Î‚¢
	GMD_BOSS2_EGG_ACT_ID_DAMAGE,		//ƒ_ƒ[ƒW
	
	GMD_BOSS2_EGG_ACT_ID_MAX
};

// =======================================================================
//ŠÇ—
// =======================================================================
#define GMD_BOSS2_MGR_LIFE_CATCH	( 3 )				//ƒ{ƒX‘Ì—ÍiƒLƒƒƒbƒ`j
#define GMD_BOSS2_MGR_LIFE_BALL		( 2 )				//ƒ{ƒX‘Ì—ÍiƒgƒQƒ{[ƒ‹j
#define GMD_BOSS2_MGR_LIFE_RUSH		( 3 )				//ƒ{ƒX‘Ì—Íiƒsƒ“ƒ{[ƒ‹j
#if	_BOSS2_TEST_DEFEAT
	#define GMD_BOSS2_MGR_LIFE			( 1 )
#else
	#define GMD_BOSS2_MGR_LIFE			( GMD_BOSS2_MGR_LIFE_CATCH + GMD_BOSS2_MGR_LIFE_BALL + GMD_BOSS2_MGR_LIFE_RUSH )	//ƒ{ƒX‘Ì—Í
	#define GMD_BOSS2_MGR_LIFE_FINAL	( 4 )	//ƒ{ƒX‘Ì—Íiƒtƒ@ƒCƒiƒ‹ƒ][ƒ“j
#endif	//_BOSS2_TEST_DEFEAT

//ƒtƒ‰ƒO
#define GMD_BOSS2_MGR_FLAG_SETUP_COMPLETE	(1 << 0)	//¶¬Š®—¹
#define GMD_BOSS2_MGR_FLAG_CLEAR_BOSS		(1 << 1)	//íœƒtƒ‰ƒO

// =======================================================================
//–{‘Ì
// =======================================================================
#define GMD_BOSS2_ANGLE_LEFT						(AKM_DEGtoA16(300.f))	//¶Œü‚«Šp“x
#define GMD_BOSS2_ANGLE_RIGHT						(AKM_DEGtoA16(60.f))	//‰EŒü‚«Šp“x
#define GMD_BOSS2_BODY_MOVE_AREA_LIMIT_WIDTH		((fx32)85*FX32_ONE)		//ˆÚ“®”ÍˆÍ
#define GMD_BOSS2_BODY_FIELD_RECT_SIZE				(24)					//’nŒ`•Ó‚è
#define GMD_BOSS2_EFFECT_CLIP_OFFSET				(64)					//‰æ–ÊŠO”»’èƒIƒtƒZƒbƒg 

//ƒm[ƒhƒ}ƒgƒŠƒNƒX
enum GME_BOSS2_BODY_SNM_INDEX{				
	GMD_BOSS2_BODY_SNM_INDEX_BODY = 0,		//–{‘ÌÚ‘±iƒGƒbƒOƒ}ƒ“AƒAƒtƒ^ƒo[ƒij
	GMD_BOSS2_BODY_SNM_INDEX_BALL,			//ƒgƒQƒ{[ƒ‹Ú‘±

	GMD_BOSS2_BODY_SNM_INDEX_START_ARM,

	GMD_BOSS2_BODY_SNM_INDEX_CATCH = GMD_BOSS2_BODY_SNM_INDEX_START_ARM,	//ƒA[ƒ€‚Â‚©‚İÚ‘±iƒ\ƒjƒbƒNA“dŒ‚ƒRƒAj
	GMD_BOSS2_BODY_SNM_INDEX_BOT_R,			//ƒA[ƒ€
	GMD_BOSS2_BODY_SNM_INDEX_BOT_L,			//ƒA[ƒ€
	GMD_BOSS2_BODY_SNM_INDEX_L_ARM1,		//ƒA[ƒ€
	GMD_BOSS2_BODY_SNM_INDEX_L_ARM2,		//ƒA[ƒ€
	GMD_BOSS2_BODY_SNM_INDEX_L_ARM3,		//ƒA[ƒ€
	GMD_BOSS2_BODY_SNM_INDEX_R_ARM1,		//ƒA[ƒ€
	GMD_BOSS2_BODY_SNM_INDEX_R_ARM2,		//ƒA[ƒ€
	GMD_BOSS2_BODY_SNM_INDEX_R_ARM3,		//ƒA[ƒ€
	GMD_BOSS2_BODY_SNM_INDEX_L_CHAIN1,		//ƒA[ƒ€Ú‘±ƒp[ƒc
	GMD_BOSS2_BODY_SNM_INDEX_R_CHAIN1,		//ƒA[ƒ€Ú‘±ƒp[ƒc
	GMD_BOSS2_BODY_SNM_INDEX_L_CHAIN2,		//ƒA[ƒ€Ú‘±ƒp[ƒc
	GMD_BOSS2_BODY_SNM_INDEX_R_CHAIN2,		//ƒA[ƒ€Ú‘±ƒp[ƒc

	GMD_BOSS2_BODY_SNM_INDEX_MAX,

	GMD_BOSS2_BODY_SNM_INDEX_BLITZ_CORE_L = GMD_BOSS2_BODY_SNM_INDEX_L_ARM3,	//“dŒ‚ƒRƒAƒGƒtƒFƒNƒgÚ‘±
	GMD_BOSS2_BODY_SNM_INDEX_BLITZ_CORE_R = GMD_BOSS2_BODY_SNM_INDEX_R_ARM3,	//“dŒ‚ƒRƒAƒGƒtƒFƒNƒgÚ‘±
	GMD_BOSS2_BODY_SNM_INDEX_BLITZ_L = GMD_BOSS2_BODY_SNM_INDEX_L_ARM2,			//“dŒ‚ƒGƒtƒFƒNƒgÚ‘±
	GMD_BOSS2_BODY_SNM_INDEX_BLITZ_R = GMD_BOSS2_BODY_SNM_INDEX_R_ARM2,			//“dŒ‚ƒGƒtƒFƒNƒgÚ‘±
};
//ƒm[ƒhƒ}ƒgƒŠƒNƒX
enum GME_BOSS2_BODY_CNM_INDEX{			
	GMD_BOSS2_BODY_CNM_INDEX_CATCH = 0,	//ƒA[ƒ€‚Â‚©‚İÚ‘±iƒ\ƒjƒbƒNA“dŒ‚ƒRƒAj
	GMD_BOSS2_BODY_CNM_INDEX_BOT_R,			//ƒA[ƒ€
	GMD_BOSS2_BODY_CNM_INDEX_BOT_L,			//ƒA[ƒ€
	GMD_BOSS2_BODY_CNM_INDEX_L_ARM1,		//ƒA[ƒ€
	GMD_BOSS2_BODY_CNM_INDEX_L_ARM2,		//ƒA[ƒ€
	GMD_BOSS2_BODY_CNM_INDEX_L_ARM3,		//ƒA[ƒ€
	GMD_BOSS2_BODY_CNM_INDEX_R_ARM1,		//ƒA[ƒ€
	GMD_BOSS2_BODY_CNM_INDEX_R_ARM2,		//ƒA[ƒ€
	GMD_BOSS2_BODY_CNM_INDEX_R_ARM3,		//ƒA[ƒ€
	GMD_BOSS2_BODY_CNM_INDEX_L_CHAIN1,		//ƒA[ƒ€Ú‘±ƒp[ƒc
	GMD_BOSS2_BODY_CNM_INDEX_R_CHAIN1,		//ƒA[ƒ€Ú‘±ƒp[ƒc
	GMD_BOSS2_BODY_CNM_INDEX_L_CHAIN2,		//ƒA[ƒ€Ú‘±ƒp[ƒc
	GMD_BOSS2_BODY_CNM_INDEX_R_CHAIN2,		//ƒA[ƒ€Ú‘±ƒp[ƒc

	GMD_BOSS2_BODY_CNM_INDEX_MAX,
};	

//ˆÚ“®’l
#if _BOSS2_TEST_STOP_MOVE
	#define GMD_BOSS2_BODY_CATCH_MOVE_SPEED			((fx32)(0*FX32_ONE))	//ƒXƒs[ƒh
	#define GMD_BOSS2_BODY_BALL_MOVE_SPEED			((fx32)(0*FX32_ONE))	//ƒXƒs[ƒh
	#define GMD_BOSS2_BODY_BALL_POS_Y				((fx32)0*FX32_ONE)		//À•WƒIƒtƒZƒbƒgiƒLƒƒƒbƒ`ó‘Ô‚ÌÀ•W‚©‚çj
	#define GMD_BOSS2_BODY_PINBALL_MOVE_SPEED_MOVE	((fx32)0*FX32_ONE)		//’Êíó‘Ô‚ÌˆÚ“®—Ê
	#define GMD_BOSS2_BODY_PINBALL_MOVE_SPEED_ROLL	((fx32)0*FX32_ONE)		//‰ñ“]ó‘Ô‚ÌˆÚ“®—Ê
#else
	#define GMD_BOSS2_BODY_CATCH_MOVE_SPEED			((fx32)(1.2f*FX32_ONE))	//ƒXƒs[ƒh
	#define GMD_BOSS2_BODY_BALL_MOVE_SPEED			((fx32)(1.2f*FX32_ONE))	//ƒXƒs[ƒh
	#define GMD_BOSS2_BODY_BALL_POS_Y				((fx32)100*FX32_ONE)	//À•WƒIƒtƒZƒbƒgiƒLƒƒƒbƒ`ó‘Ô‚ÌÀ•W‚©‚çj
	#define GMD_BOSS2_BODY_PINBALL_MOVE_SPEED_MOVE	((fx32)0*FX32_ONE)		//’Êíó‘Ô‚ÌˆÚ“®—Ê
	#define GMD_BOSS2_BODY_PINBALL_MOVE_SPEED_ROLL	((fx32)5*FX32_ONE)		//‰ñ“]ó‘Ô‚ÌˆÚ“®—Ê

#endif	//_BOSS2_TEST_STOP_MOVE
#define GMD_BOSS2_BODY_ADJUST_SPEED_FINAL			((fx32)(1.5f*FX32_ONE))//ˆÚ“®—Ê’²®iƒtƒ@ƒCƒiƒ‹ƒ][ƒ“j

//ŠJn
#define GMD_BOSS2_BODY_START_TIME_WAIT_END			(180)				//I—¹‘Ò‹@ŠÔ

//ƒLƒƒƒbƒ`
#define GMD_BOSS2_BODY_CATCH_FRAME_TURN				(60.0f)					//•ûŒü“]Š·ƒtƒŒ[ƒ€”
#define GMD_BOSS2_BODY_CATCH_FRAME_DRIFT			(60.0f)					//ƒhƒŠƒtƒgƒtƒŒ[ƒ€”
#define GMD_BOSS2_BODY_CATCH_AREA_WIDTH				((fx32)8*FX32_ONE)		//ƒLƒƒƒbƒ`UŒ‚‚ğs‚¤•
#define GMD_BOSS2_BODY_CATCH_FRAME_WAIT_DOWN		(5)						//ƒA[ƒ€‚ğL‚Î‚·‘O‚Ì‘Ò‚¿ŠÔ
#define GMD_BOSS2_BODY_CATCH_ARM_STRETCH_DOWN		(6.0f)					//ƒA[ƒ€‚ªL‚Ñ‚é—Ê
#define GMD_BOSS2_BODY_CATCH_ARM_STRETCH_UP			(1.0f)					//ƒA[ƒ€‚ªL‚Ñ‚é—Ê
#define GMD_BOSS2_BODY_CATCH_ARM_STRETCH_MAX		(60.0f)					//ƒA[ƒ€‚ªL‚Ñ‚éÅ‘å’l
#define GMD_BOSS2_BODY_CATCH_FRAME_CATCH_END		(10.0f)					//‚Â‚©‚İUŒ‚—]‰C
#define GMD_BOSS2_BODY_CATCH_OFFSET_SONIC			((fx32)40*FX32_ONE)		//ƒ\ƒjƒbƒN‚ğ‚­‚Á‚Â‚¯‚éÀ•W
#define GMD_BOSS2_BODY_CATCH_FRAME_CARRY_OPEN		(40.0f)					//ƒ\ƒjƒbƒN‚ğ•ú‚·ƒtƒŒ[ƒ€
#define GMD_BOSS2_BODY_CATCH_RELEASE_KEY			(PAD_BUTTON_JUMP|PAD_BUTTON_TRANSFORM|KEYS_LEVER)	//‚Â‚©‚Ü‚ê‚½Û‚É‰ğ•ú‚³‚ê‚é—pƒL[
#if _IPHONE
#define GMD_BOSS2_BODY_CATCH_RELEASE_KEY_PUSH_NUM	(6)					//‚Â‚©‚Ü‚ê‚½Û‚É‰ğ•ú‚³‚ê‚é“ü—Í‰ñ”
#else
#define GMD_BOSS2_BODY_CATCH_RELEASE_KEY_PUSH_NUM	(10)					//‚Â‚©‚Ü‚ê‚½Û‚É‰ğ•ú‚³‚ê‚é“ü—Í‰ñ”
#endif // _IPHONE
#define GMD_BOSS2_BODY_CATCH_SHAKE_WIDTH			(10)		//‰ğ•úƒL[“ü—ÍƒvƒŒƒCƒ„U“®•
#define GMD_BOSS2_BODY_CATCH_SHAKE_SPEED			(2)		//‰ğ•úƒL[“ü—ÍƒvƒŒƒCƒ„U“®‘¬“x

//ƒgƒQƒ{[ƒ‹
#define GMD_BOSS2_BODY_BALL_FRAME_RISE				(60.0f)					//ã¸ˆÚ“®ƒtƒŒ[ƒ€”
#define GMD_BOSS2_BODY_BALL_FRAME_TURN				(60.0f)					//•ûŒü“]Š·ƒtƒŒ[ƒ€”
#define GMD_BOSS2_BODY_BALL_FRAME_DRIFT				(60.0f)					//•ûŒü“]Š·ƒtƒŒ[ƒ€”
#define GMD_BOSS2_BODY_BALL_AREA_WIDTH				((fx32)8*FX32_ONE)		//ƒgƒQƒ{[ƒ‹UŒ‚‚ğs‚¤•
#define GMD_BOSS2_BODY_BALL_WAIT_FRAME_CREATE		(30)					//¶¬‘Ò‚¿ƒtƒŒ[ƒ€”
#define GMD_BOSS2_BODY_BALL_FLICKER_RADIUS			((Float)16.0f)			//“_–Åˆ—”¼Œa
#if _IPHONE
#define GMD_BOSS2_BODY_BALL_CAMERA_SCALE			(0.82f)					//ƒJƒƒ‰ƒXƒP[ƒ‹
#else
#define GMD_BOSS2_BODY_BALL_CAMERA_SCALE			(0.72f)					//ƒJƒƒ‰ƒXƒP[ƒ‹
#endif // _IPHONE

//ƒsƒ“ƒ{[ƒ‹
#define GMD_BOSS2_BODY_PINBALL_FRAME_MOVE				(180)					//’Êíó‘Ô‚ÌƒtƒŒ[ƒ€”
#define GMD_BOSS2_BODY_PINBALL_FRAME_READY				(10)					//‰ñ“]€”õƒtƒŒ[ƒ€
#define GMD_BOSS2_BODY_PINBALL_FRAME_ROLL				(180)					//‰ñ“]ó‘Ô‚ÌƒtƒŒ[ƒ€”
#define GMD_BOSS2_BODY_PINBALL_FRAME_ROLL_STOP			(10)					//‰ñ“]’â~ó‘Ô‚ÌƒtƒŒ[ƒ€”
#define GMD_BOSS2_BODY_PINBALL_FRAME_TURN				(20.0f)					//•ûŒü“]Š·ƒtƒŒ[ƒ€”
#define GMD_BOSS2_BODY_PINBALL_ROLL_ROT_Z				((Angle16)AKM_DEGtoA32(20.0f))	//‰ñ“]ó‘Ô‚Ì‰ñ“]—Ê
#define GMD_BOSS2_BODY_PINBALL_FRAME_CREATE_BLITZ		(119.0f)				//“dŒ‚ƒGƒtƒFƒNƒg¶¬‘Ò‚¿ƒtƒŒ[ƒ€
#define GMD_BOSS2_BODY_PINBALL_FRAME_CREATE_BLITZ_END	(64.0f)				//“dŒ‚ƒGƒtƒFƒNƒg¶¬I—¹ƒtƒŒ[ƒ€
#define GMD_BOSS2_BODY_PINBALL_ANGLE_LEFT_ROLL			(AKM_DEGtoA16(270.f))	//¶Œü‚«Šp“xi‰ñ“]UŒ‚j
#define GMD_BOSS2_BODY_PINBALL_ANGLE_RIGHT_ROLL			(AKM_DEGtoA16(90.f))	//‰EŒü‚«Šp“xi‰ñ“]UŒ‚j
#define GMD_BOSS2_BODY_PINBALL_NO_STOP_AREA_SIZE_X		(70)
#define GMD_BOSS2_BODY_PINBALL_NO_STOP_AREA_SIZE_Y		(110)
#define	GMD_BOSS2_BODY_PINBALL_MOVE_DISTANCE_MAX		(500)	//ˆÚ“®•ûŒüİ’è—p‹——£

//ƒ_ƒ[ƒW
#define GMD_BOSS2_BODY_DEF_PLAYER_MOVE_X			((fx32)5*FX32_ONE)	//‚­‚ç‚¢ƒvƒŒƒCƒ„ˆÚ“®—Ê
#define GMD_BOSS2_BODY_DEF_PLAYER_MOVE_Y			((fx32)4*FX32_ONE)	//‚­‚ç‚¢ƒvƒŒƒCƒ„ˆÚ“®—Ê
#define GMD_BOSS2_BODY_DEF_PLAYER_NO_JUMP_MOVE_TIME	((fx32)25*FX32_ONE)	//‚­‚ç‚¢ƒvƒŒƒCƒ„‚ªƒWƒƒƒ“ƒvˆÚ“®‚Å‚«‚È‚¢ŠÔ
#define GMD_BOSS2_BODY_DEF_NO_HIT_TIME				((u32)10)			//‚­‚ç‚¢ƒqƒbƒg–³ŒøŠÔ
#define GMD_BOSS2_BODY_INVINVIBLE_TIME				((u32)90)			//–³“GŠÔ
#define GMD_BOSS2_BODY_DMG_FLICKER_RADIUS			((Float)32.0f)		//“_–Åˆ—”¼Œa

//Œ‚”j
#define GMD_BOSS2_BODY_DEFEAT_TIME_WAIT_START		((s32)40)				//ŠJn‘Ò‚¿ŠÔ
#define GMD_BOSS2_BODY_DEFEAT_TIME_WAIT_BOMB		((s32)120)				//”š”­‘Ò‚¿ŠÔ
#define GMD_BOSS2_BODY_DEFEAT_TIME_WAIT_SCATTER		((s32)40)				//‚Î‚çT‚«‘Ò‚¿ŠÔ
#define GMD_BOSS2_BODY_DEFEAT_TIME_WAIT_END			((s32)120)				//I—¹‘Ò‚¿ŠÔ
#define GMD_BOSS2_BODY_DEFEAT_FALL_POS_Y			((fx32)150*FX32_ONE)	//—‰ºÀ•W

//‘å”š”­
#define GMD_BOSS2_BODY_DEFEAT_FLASH_INTO_TIME		(4)						// Œ‚”j‚Ìƒtƒ‰ƒbƒVƒ…Š®‘S‚É”’‚É‚È‚é‚Ü‚Å‚ÌƒtƒŒ[ƒ€
#define GMD_BOSS2_BODY_DEFEAT_FLASH_KEEP_TIME		(5)						// Œ‚”j‚Ìƒtƒ‰ƒbƒVƒ…Š®‘S‚É”’‚ÌŠÔ‚ÌƒtƒŒ[ƒ€
#define GMD_BOSS2_BODY_DEFEAT_FLASH_RETURN_TIME		(30)	

//“¦–S
#define GMD_BOSS2_BODY_ESCAPE_SCROLL_UNLOCK_DISTANCE	((fx32)(32.0f*FX32_ONE))	//ƒXƒNƒ[ƒ‹ƒƒbƒN‰ğœ‚·‚é‹——£i‰æ–Êã‚©‚çj
#define GMD_BOSS2_BODY_ESCAPE_SPD_X_ADD					((fx32)(0.0f*FX32_ONE))		//ƒXƒs[ƒhÅ‘å’l
#define GMD_BOSS2_BODY_ESCAPE_SPD_Y_ADD					((fx32)(0.16f*FX32_ONE))	//ƒXƒs[ƒhÅ‘å’l
#define GMD_BOSS2_BODY_ESCAPE_SPD_X_MAX					((fx32)(0.0f*FX32_ONE))	//ƒXƒs[ƒhÅ‘å’l
#define GMD_BOSS2_BODY_ESCAPE_SPD_Y_MAX					((fx32)(-0.8f*FX32_ONE))	//ƒXƒs[ƒhÅ‘å’l
#define GMD_BOSS2_BODY_ESCAPE_SCREEN_OUT_LENGTH			((s32)(64))		//‰æ–ÊŠO”»’è‹——£

//ƒtƒ‰ƒO
#define GMD_BOSS2_BODY_FLAG_INVINCIBLE				(1 << 0)	//–³“Gó‘Ôi“–‚½‚è‚Í‚ ‚é‚ªAƒ‰ƒCƒt‚ÍŒ¸‚ç‚È‚¢j
#define GMD_BOSS2_BODY_FLAG_AFTERBURNER_ACTIVE		(1 << 1)	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg—LŒø’†
#define GMD_BOSS2_BODY_FLAG_BLITZ_ACTIVE			(1 << 2)	//“dŒ‚ƒGƒtƒFƒNƒg—LŒø’†
#define GMD_BOSS2_BODY_FLAG_CATCH					(1 << 3)	//ƒ\ƒjƒbƒNƒLƒƒƒbƒ`’†
#define GMD_BOSS2_BODY_FLAG_NOATTACK				(1 << 4)	//UŒ‚‚µ‚È‚¢
#define GMD_BOSS2_BODY_FLAG_ROLL_ACTIVE				(1 << 5)	//‰ñ“]ƒGƒtƒFƒNƒg—LŒø’†
#define GMD_BOSS2_BODY_FLAG_EGG_NODISP				(1 << 6)	//ƒGƒbƒOƒ}ƒ“”ñ•\¦’†
#define GMD_BOSS2_BODY_FLAG_NO_SE_HIT_WALL			(1 << 7)	//•Ç‚É“–‚½‚Á‚Ä‚à‰¹‚ª–Â‚ç‚È‚¢


//ƒVƒOƒiƒ‹
#define GMD_BOSS2_BODY_FLAG_SIGNAL_B2N_SHOOT		(1 << 22)	//ƒgƒQƒ{[ƒ‹Ëo’Ê’m
#define GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_ESCAPE		(1 << 23)	//“¦–S’Ê’m
#define GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_BURNT		(1 << 24)	//•‚±‚°ƒeƒNƒXƒ`ƒƒ‚Ö‚Ì•ÏX’Ê’m
#define GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_AFTERBURNER	(1 << 25)	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg¶¬’Ê’m
#define GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_SCATTER		(1 << 27)	//ƒp[ƒc”òU’Ê’m
#define GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_HIT			(1 << 28)	//ƒqƒbƒg’Ê’m
#define GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_DAMAGE		(1 << 29)	//ƒ_ƒ[ƒW’Ê’m
#define GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_DAMAGE		(1 << 30)	//ƒ_ƒ[ƒW’Ê’m
#define GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_DEFEAT		(1 << 31)	//Œ‚”j’Ê’m



//ó‘Ô
enum GME_BOSS2_BODY_STATE{
	GMD_BOSS2_BODY_STATE_NO_OPERATION = 0,		//‰½‚à‚µ‚È‚¢
	GMD_BOSS2_BODY_STATE_START,					//ŠJn

	GMD_BOSS2_BODY_STATE_CATCH_MOVE,			//ƒLƒƒƒbƒ`ˆÚ“®
	GMD_BOSS2_BODY_STATE_CATCH_ARM,				//ƒLƒƒƒbƒ`’Í‚Ş
	GMD_BOSS2_BODY_STATE_CATCH_CARRY,			//ƒLƒƒƒbƒ`‰^‚Ô

	GMD_BOSS2_BODY_STATE_PRE_BALL,				//ƒgƒQƒ{[ƒ‹‘O‰‰o
	GMD_BOSS2_BODY_STATE_BALL_MOVE,				//ƒgƒQƒ{[ƒ‹ˆÚ“®
	GMD_BOSS2_BODY_STATE_BALL_SHOOT,			//ƒgƒQƒ{[ƒ‹Ëo

	GMD_BOSS2_BODY_STATE_PRE_PINBALL,			//ƒsƒ“ƒ{[ƒ‹‘O‰‰o
	GMD_BOSS2_BODY_STATE_PINBALL_MOVE,			//ƒsƒ“ƒ{[ƒ‹ˆÚ“®
	GMD_BOSS2_BODY_STATE_PINBALL_ROLL,			//ƒsƒ“ƒ{[ƒ‹‰ñ“]

	GMD_BOSS2_BODY_STATE_DEFEAT,				//Œ‚”j
	GMD_BOSS2_BODY_STATE_ESCAPE,				//“¦–S
	
	GMD_BOSS2_BODY_STATE_MAX
};

// =======================================================================
//ƒGƒbƒOƒ}ƒ“
// =======================================================================
//ƒtƒ‰ƒO
#define GMD_BOSS2_EGG_FLAG_EGG_ACT_ACTIVE	(1 << 0)	//ê—pƒAƒNƒVƒ‡ƒ“’†ƒtƒ‰ƒO
#define GMD_BOSS2_EGG_FLAG_SWEAT_ACTIVE		(1 << 1)	//Š¾ƒGƒtƒFƒNƒg—LŒø’†ƒtƒ‰ƒO

//ƒV[ƒPƒ“ƒX
enum GME_BOSS2_EGGMAN_SEQ{
	GMD_BOSS2_EGGMAN_SEQ_IDLE = 0,	//‘Ò‹@

	GMD_BOSS2_EGGMAN_SEQ_MAX
};

// =======================================================================
//ƒgƒQƒ{[ƒ‹
// =======================================================================
#define GMD_BOSS2_BALL_OFFSET_CATCH		( (fx32)56*FX32_ONE )	//ƒ{[ƒ‹’Í‚ŞƒIƒtƒZƒbƒg
#define GMD_BOSS2_BALL_BOMB_TIME		( 2*60 )				//”š”­‘Ò‚¿ŠÔ
#define GMD_BOSS2_BALL_FLICKER_TIME		( 1*60 )				//“_–Å‘Ò‚¿ŠÔ
#define GMD_BOSS2_BALL_PART_SPEED_X		( (fx32)1*FX32_ONE )	//ˆÚ“®’l
#define GMD_BOSS2_BALL_PART_SPEED_Y		( (fx32)4*FX32_ONE )	//ˆÚ“®’l


// =======================================================================
//ƒGƒtƒFƒNƒg
// =======================================================================
#define GMD_BOSS2_EFFECT_BOMB_OFFSET_Z						((fx32)(FX32_ONE * 32))	//”š”­ƒGƒtƒFƒNƒgÀ•W

#define GMD_BOSS2_EFFECT_SWEAT_DISP_OFFSET_Y				((Float)32.f)	//Š¾ƒGƒtƒFƒNƒg•\¦À•W
#define GMD_BOSS2_EFFECT_AFTERBURNER_DISP_OFFSET_Z			((Float)-30.f)	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg•\¦À•W
#define GMD_BOSS2_EFFECT_AFTERBURNER_SMOKE_DISP_OFFSET_Z	((Float)-32.f)	//ƒAƒtƒ^ƒo[ƒi‰ŒƒGƒtƒFƒNƒg•\¦À•W
#define GMD_BOSS2_EFFECT_BODY_SMOKE_DISP_OFFSET_Z			((Float)-32.f)	//–{‘Ì‰ŒƒGƒtƒFƒNƒg•\¦À•W
#define GMD_BOSS2_EFFECT_BLITZ_LINE_DISP_OFFSET_Y_CREATE	((Float)-30.f)	//“dŒ‚ƒGƒtƒFƒNƒg•\¦ƒIƒtƒZƒbƒg
#define GMD_BOSS2_EFFECT_BLITZ_LINE_DISP_OFFSET_Y_NORMAL	((Float)-4.f)	//“dŒ‚ƒGƒtƒFƒNƒg•\¦ƒIƒtƒZƒbƒg
#define GMD_BOSS2_EFFECT_BLITZ_LINE_DISP_ROT_Z				((Angle16)AKM_DEGtoA32(90.0f))	//“dŒ‚ƒGƒtƒFƒNƒg‰ñ“]
#define GMD_BOSS2_EFFECT_BLITZ_CORE_DISP_OFFSET				((Float)24.f)	//“dŒ‚ƒGƒtƒFƒNƒg•\¦ƒIƒtƒZƒbƒg
#define GMD_BOSS2_EFFECT_BLITZ_ARM_DISP_ROT_Z				((Angle16)AKM_DEGtoA32(90.0f))	//“dŒ‚ƒGƒtƒFƒNƒg‰ñ“]
#define GMD_BOSS2_EFFECT_BLITZ_ARM_DISP_OFFSET				((Float)4.f)	//“dŒ‚ƒGƒtƒFƒNƒg•\¦ƒIƒtƒZƒbƒg
#define GMD_BOSS2_EFFECT_ROLLATTACK_DISP_ROT_Y				(90.0f)	//‰ñ“]ƒGƒtƒFƒNƒg‰ñ“]
#define GMD_BOSS2_EFFECT_ROLLATTACK_DISP_OFFSET_Z			((Float)64.f)	//‰ñ“]ƒGƒtƒFƒNƒgƒIƒtƒZƒbƒg

#define	GMD_BOSS2_SCATTER_SPEED								(1.0f)			//”òUƒGƒtƒFƒNƒg‘¬“x

// =======================================================================
//\‘¢‘Ì
// =======================================================================
//ƒp[ƒcƒAƒNƒVƒ‡ƒ“î•ñ\‘¢‘Ì
typedef struct tag_GMS_BOSS2_PART_ACT_INFO
{
	Uint16 mtn_id;			///< ƒ‚[ƒVƒ‡ƒ“”Ô†
	Uint8 is_maintain;		///< ‘O‚ÌƒAƒNƒVƒ‡ƒ“Œp‘±
	Uint8 is_repeat;		///< ƒŠƒs[ƒg
	Float mtn_spd;			///< ƒ‚[ƒVƒ‡ƒ“Ä¶‘¬“x
	BOOL is_blend;			///< ƒuƒŒƒ“ƒh—L–³
	Float blend_spd;		///< ƒuƒŒƒ“ƒh‘¬“x
	BOOL is_merge_manual;	///< ƒ}ƒjƒ…ƒAƒ‹ƒ}[ƒW—L–³i–¢À‘•j
}GMS_BOSS2_PART_ACT_INFO;

typedef struct tag_GMS_BOSS2_EFF_BOMB_WORK GMS_BOSS2_EFF_BOMB_WORK;
typedef struct tag_GMS_BOSS2_MGR_WORK GMS_BOSS2_MGR_WORK;
typedef struct tag_GMS_BOSS2_BODY_WORK GMS_BOSS2_BODY_WORK;
typedef struct tag_GMS_BOSS2_EGG_WORK GMS_BOSS2_EGG_WORK;
typedef struct tag_GMS_BOSS2_BALL_WORK GMS_BOSS2_BALL_WORK;

//ó‘Ô—pŠÖ”
typedef void (*GMF_BOSS2_BODY_STATE_FUNC)(GMS_BOSS2_BODY_WORK* body_work);		
typedef void (*GMF_BOSS2_EGG_STATE_FUNC)(GMS_BOSS2_EGG_WORK* egg_work);	
typedef void (*GMF_BOSS2_BALL_STATE_FUNC)(GMS_BOSS2_BALL_WORK* ball_work);	

//”š”­ƒGƒtƒFƒNƒgƒ[ƒN
struct tag_GMS_BOSS2_EFF_BOMB_WORK
{
	OBS_OBJECT_WORK* parent_obj;
	Uint32 interval_timer;
	Uint32 interval_min;
	Uint32 interval_max;
	fx32 pos[MTD_XY];
	fx32 area[MTD_WH];
};

//ŠÇ—ƒ[ƒN
struct tag_GMS_BOSS2_MGR_WORK
{
	GMS_ENEMY_3D_WORK ene_3d;		///< ƒGƒlƒ~[ƒ[ƒN
	Sint32 life;					///< ƒ‰ƒCƒt	
	Uint32 flag;					///< ƒtƒ‰ƒO
	GMS_BOSS2_BODY_WORK* body_work;	///< –{‘Ìƒ[ƒN

	s32 obj_create_count;			///< ƒIƒuƒWƒFƒNƒg¶¬”
};

//–{‘Ìƒ[ƒN
struct tag_GMS_BOSS2_BODY_WORK
{
	GMS_ENEMY_3D_WORK ene_3d;				///< ƒGƒlƒ~[ƒ[ƒN

	OBS_OBJECT_WORK* parts_objs[GMD_BOSS2_PART_IDX_MAX];	///< qƒIƒuƒWƒFƒ[ƒN

	GME_BOSS2_BODY_STATE state;				///< ó‘Ô
	GME_BOSS2_BODY_STATE prev_state;		///< ‘O‰ñ‚Ìó‘Ô
	GMF_BOSS2_BODY_STATE_FUNC proc_update;	///< XVŠÖ”
	u32 flag;								///< ƒtƒ‰ƒO
	GME_BOSS2_ACT_ID action_id;				///< ƒAƒNƒVƒ‡ƒ“ID

	//ƒLƒƒƒbƒ`UŒ‚
	Float offset_arm;						///< ƒA[ƒ€‚ÌL‚Ñ—Ê
	OBS_RECT_WORK rect_work_arm;			///< ƒA[ƒ€—p‹éŒ`
	s32 count_release_key;					///< ‰ğ•úƒL[“ü—Í‰ñ”
	s32 shake_pos;							///< ‰ğ•úƒL[“ü—ÍƒvƒŒƒCƒ„U“®—p
	s32 shake_speed;						///< ‰ğ•úƒL[“ü—ÍƒvƒŒƒCƒ„U“®—p‘¬“x
	u32 shake_count;						///< ‰ğ•úƒL[“ü—ÍƒvƒŒƒCƒ„U“®—pÜ‚è•Ô‚µ‰ñ”
#if GMD_BOSS2_BODY_CATCH_RELEASE_TILT
	Angle32 prev_rot_z;						///< ‘O‰ñ—LŒø‚¾‚Á‚½ŒX‚«‚Ì•Û‘¶
#endif // GMD_BOSS2_BODY_CATCH_RELEASE_TILT

	//ƒsƒ“ƒ{[ƒ‹UŒ‚
	u32 counter_pinball;					///< ‰ñ“]‚ÉˆÚs‚·‚éƒJƒEƒ“ƒ^

	//Œü‚«
	Angle16 angle_current;					///< Œ»İ‚ÌŒü‚«

	//ˆÚ“®—pî•ñ
	VecFx32 start_pos;						///< ˆÚ“®ŠJnÀ•W
	VecFx32 end_pos;						///< ˆÚ“®I—¹À•W
	Float move_counter;						///< ˆÚ“®—pƒJƒEƒ“ƒ^
	Float move_frame;						///< ˆÚ“®‚ÌƒtƒŒ[ƒ€”

	//•ûŒü“]Š·—pî•ñ
	Angle16 turn_start;						///< •ûŒü“]Š·ŠJn‚ÌŠp“x
	Angle32 turn_amount;					///< •ûŒü“]Š·‚·‚é‘—Ê
	Float turn_counter;						///< •ûŒü“]Š·—pƒJƒEƒ“ƒ^
	Float turn_frame;						///< •ûŒü“]Š·‚ÌƒtƒŒ[ƒ€”

	//ƒm[ƒhƒ}ƒgƒŠƒNƒXŒn
	GMS_BS_CMN_BMCB_MGR bmcb_mgr;			///< ƒ{ƒXƒ‚[ƒVƒ‡ƒ“ƒR[ƒ‹ƒoƒbƒNŠÇ—
	GMS_BS_CMN_SNM_WORK snm_work;			///< ƒm[ƒhƒ}ƒgƒŠƒNƒXæ“¾ˆ—ƒ[ƒN
	s32 snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_MAX];	///< Ú‘±ƒm[ƒhID
	
	GMS_BS_CMN_CNM_MGR_WORK cnm_mgr_work;	///< ƒm[ƒhƒ}ƒgƒŠƒNƒX‘€ìˆ—ŠÇ—ƒ[ƒN
	s32 cnm_reg_id[GMD_BOSS2_BODY_CNM_INDEX_MAX];	///< ‘€ìƒm[ƒhID

	//ƒ_ƒ[ƒWŒn
	GMS_BS_CMN_DMG_FLICKER_WORK flk_work;	///< ƒ_ƒ[ƒW“_–Åƒ[ƒN
	u32 counter_no_hit;						///< HIT–³ŒøŠÔƒJƒEƒ“ƒ^
	u32 counter_invincible;					///< –³“GŠÔƒJƒEƒ“ƒ^
	
	GMS_CMN_FLASH_SCR_WORK flash_work;		///< ƒtƒ‰ƒbƒVƒ…ƒ[ƒN

	//ƒGƒtƒFƒNƒg
	GMS_BOSS2_EFF_BOMB_WORK bomb_work;		//!< ”š”­ˆ—ƒ[ƒN

	//SE
	GSS_SND_SE_HANDLE* se_handle;		//SEƒnƒ“ƒhƒ‹
};

//ƒGƒbƒOƒ}ƒ“ƒ[ƒN
struct tag_GMS_BOSS2_EGG_WORK
{
	GMS_ENEMY_3D_WORK ene_3d;				///< ƒGƒlƒ~[ƒ[ƒN
	GME_BOSS2_EGG_ACT_ID egg_action_id;		///< ê—pƒAƒNƒVƒ‡ƒ“
	Uint32 flag;							///< ƒtƒ‰ƒO

	GMF_BOSS2_EGG_STATE_FUNC proc_update;	///< XVŠÖ”
};

//ƒgƒQƒ{[ƒ‹ƒ[ƒN
struct tag_GMS_BOSS2_BALL_WORK
{
	GMS_ENEMY_3D_WORK ene_3d;				///< ƒGƒlƒ~[ƒ[ƒN

	GMS_BS_CMN_DMG_FLICKER_WORK flk_work;	///< ƒ_ƒ[ƒW“_–Åƒ[ƒN

	GMF_BOSS2_BALL_STATE_FUNC proc_update;	///< XVŠÖ”
};

//”òUƒGƒtƒFƒNƒg
typedef struct tag_GMS_BOSS2_EFFECT_SCATTER_WORK
{
	GMS_BS_CMN_NODE_CTRL_OBJECT control_node_work;
	AMS_QUAT spin_quat;		//‰ñ“]—Ê—p
} GMS_BOSS2_EFFECT_SCATTER_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------


// =======================================================================
//ƒ{ƒX
// =======================================================================
static void* GmBoss2GetGameDatEnemyArc( void );
static void gmBoss2ChangeTextureBurnt( OBS_OBJECT_WORK *obj_work );

static void gmBoss2ExitFunc( MTS_TASK_TCB *tcb );
static void gmBoss2EffectExitFunc( MTS_TASK_TCB *tcb );

static BOOL gmBoss2CheckScrollLocked( void );

// =======================================================================
//ŠÇ—
// =======================================================================
static BOOL gmBoss2MgrCheckSetupComplete( GMS_BOSS2_MGR_WORK* mgr_work );
static GMS_BOSS2_MGR_WORK* gmBoss2MgrGetMgrWork( OBS_OBJECT_WORK* obj_work_parts );
static void gmBoss2MgrAddObject( GMS_BOSS2_MGR_WORK* mgr_work, OBS_OBJECT_WORK* obj_work_parts );
static void gmBoss2MgrDeleteObject( OBS_OBJECT_WORK* obj_work_parts );
static void gmBoss2MgrMainFuncWaitLoad( OBS_OBJECT_WORK* obj_work );
static void gmBoss2MgrMainFuncWaitSetup( OBS_OBJECT_WORK* obj_work );
static void gmBoss2MgrMainFunc( OBS_OBJECT_WORK* obj_work );
static void gmBoss2MgrMainFuncWaitRelease( OBS_OBJECT_WORK* obj_work );

// =======================================================================
//–{‘Ì
// =======================================================================
static void gmBoss2BodyExit( MTS_TASK_TCB* tcb );
static void gmBoss2BodyReactionPlayer( OBS_OBJECT_WORK* obj_work_player, const OBS_OBJECT_WORK* obj_work_body );
static void gmBoss2BodyRecFuncRegistArmRect(OBS_OBJECT_WORK *obj_work);
static void gmBoss2BodyCatchChangeArmRectNormal( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyCatchChangeArmRectActive( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyCatchChangeArmRectCatch( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodySetRectNormal( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodySetRectActive( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodySetRectRoll( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodySetRectArm( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyRectApplyOffsetArm( GMS_BOSS2_BODY_WORK* body_work );
extern void gmBoss2BodySetActionAllParts(
								  GMS_BOSS2_BODY_WORK* body_work,
								  GME_BOSS2_ACT_ID action_id,
								  BOOL force_change	= FALSE );
static void gmBoss2BodyOutFunc( OBS_OBJECT_WORK* obj_work );
static void gmBoss2BodyDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect );
static void gmBoss2BodyHitFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect );
static OBS_OBJECT_WORK* gmBoss2BodySearchShutterIn( void );
static OBS_OBJECT_WORK* gmBoss2BodySearchShutterOut( void );

//UŒ‚–³Œø
static void gmBoss2BodySetNoHitTime( GMS_BOSS2_BODY_WORK *body_work, u32 time );
static void gmBoss2BodyUpdateNoHitTime( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodySetInvincibleTime( GMS_BOSS2_BODY_WORK *body_work, u32 time );
static void gmBoss2BodyUpdateInvincibleTime( GMS_BOSS2_BODY_WORK* body_work );


//Œü‚«
static void gmBoss2BodySetDirection( GMS_BOSS2_BODY_WORK* body_work, Angle16 deg );
static void gmBoss2BodySetDirectionNormal( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodySetDirectionRoll( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyUpdateDirection( GMS_BOSS2_BODY_WORK* body_work );

//À•WˆÚ“®
static Float gmBoss2BodyCalcMoveXNormalFrame( 
									const GMS_BOSS2_BODY_WORK* body_work,
									fx32 x,
									fx32 speed );
static void gmBoss2BodyInitMoveNormal(
						 GMS_BOSS2_BODY_WORK* body_work, 
						 const VecFx32* dest_pos,
						 Float frame );
static Float gmBoss2BodyUpdateMoveNormal( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyInitMovePinBall( 
									   GMS_BOSS2_BODY_WORK* body_work,
									   const VecFx32* dir_pos,
									   fx32 speed);
static BOOL gmBoss2BodyPinBallCheckFieldUnder( const OBS_OBJECT_WORK* obj_work );
static BOOL gmBoss2BodyPinBallCheckFieldOver( const OBS_OBJECT_WORK* obj_work );
static BOOL gmBoss2BodyPinBallCheckFieldFront( const OBS_OBJECT_WORK* obj_work );
static BOOL gmBoss2BodyPinBallCheckFieldBack( const OBS_OBJECT_WORK* obj_work );
static void gmBoss2BodyUpdateMovePinBall( GMS_BOSS2_BODY_WORK* body_work );

//•ûŒü“]Š·
static void gmBoss2BodyInitTurn(
						 GMS_BOSS2_BODY_WORK* body_work, 
						 Angle16 dest_angle,
						 Float frame, 
						 BOOL flag_positive );
static Float gmBoss2BodyUpdateTurn( GMS_BOSS2_BODY_WORK* body_work );

//ƒLƒƒƒbƒ`
static BOOL gmBoss2BodyCatchArmCheckTarget( const GMS_BOSS2_BODY_WORK* body_work );
static BOOL gmBoss2BodyCatchArmCountReleaseKey( GMS_BOSS2_BODY_WORK* body_work );
static BOOL gmBoss2BodyCatchArmCheckRelease( const GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyCatchArmUpdateShakePlayer( GMS_BOSS2_BODY_WORK* body_work, u32 shake_count );
static void gmBoss2BodyCatchSetArmLength( GMS_BOSS2_BODY_WORK* body_work, Float length );
static void gmBoss2BodyCatchHitFuncArmCatch( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect );
static void gmBoss2BodyCatchChangeNeedleModeActive( void );
static void gmBoss2BodyCatchChangeNeedleModeWait( void );
//static void gmBoss2BodyCatchChangeNeedleModeTimer( void );

//ƒgƒQƒ{[ƒ‹
static BOOL gmBoss2BodyBallShootCheckTarget( const GMS_BOSS2_BODY_WORK* body_work );

//ƒsƒ“ƒ{[ƒ‹
static BOOL gmBoss2BodyPinBallCheckTurn( const GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyPinBallAdjustMoveSpeed( GMS_BOSS2_BODY_WORK* body_work, fx32 speed );
static BOOL gmBoss2BodyPinBallCheckAreaStop( const GMS_BOSS2_BODY_WORK* body_work );

//ƒ_ƒ[ƒW
static void gmBoss2BodyDamage( GMS_BOSS2_BODY_WORK* body_work );

//“¦–S
static BOOL gmBoss2BodyEscapeCheckScrollUnlock( const GMS_BOSS2_BODY_WORK* body_work );
static BOOL gmBoss2BodyEscapeCheckScreenOut( const GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyEscapeAddjustSpeed( GMS_BOSS2_BODY_WORK* body_work );

//ó‘Ô
static void gmBoss2BodyChangeState(
							GMS_BOSS2_BODY_WORK* body_work,
							GME_BOSS2_BODY_STATE state );
static void gmBoss2BodyMainFuncWaitSetup( OBS_OBJECT_WORK* obj_work );
static void gmBoss2BodyMainFunc( OBS_OBJECT_WORK* obj_work );

//ŠJn
static void gmBoss2BodyStateStartEnter( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateStartLeave( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateStartUpdateWait( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateStartUpdateEnd( GMS_BOSS2_BODY_WORK* body_work );

//ƒLƒƒƒbƒ`ˆÚ“®
static void gmBoss2BodyStateCatchMoveEnter( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateCatchMoveLeave( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateCatchMoveUpdateMove( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateCatchMoveUpdateTurn( GMS_BOSS2_BODY_WORK* body_work );

//ƒLƒƒƒbƒ`’Í‚Ş
static void gmBoss2BodyStateCatchArmEnter( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateCatchArmLeave( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateCatchArmUpdateOpen( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateCatchArmUpdateReady( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateCatchArmUpdateDown( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateCatchArmUpdateClose( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateCatchArmUpdateUp( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateCatchArmUpdateEnd( GMS_BOSS2_BODY_WORK* body_work );

//ƒLƒƒƒbƒ`‰^‚Ô
static void gmBoss2BodyStateCatchCarryEnter( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateCatchCarryLeave( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateCatchCarryUpdateMove( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateCatchCarryUpdateOpen( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateCatchCarryUpdateEnd( GMS_BOSS2_BODY_WORK* body_work );

//ƒgƒQƒ{[ƒ‹‘O‰‰o
static void gmBoss2BodyStatePreBallEnter( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStatePreBallLeave( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStatePreBallUpdateAngry( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStatePreBallUpdateRise( GMS_BOSS2_BODY_WORK* body_work );

//ƒgƒQƒ{[ƒ‹ˆÚ“®
static void gmBoss2BodyStateBallMoveEnter( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateBallMoveLeave( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateBallMoveUpdateMove( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateBallMoveUpdateTurn( GMS_BOSS2_BODY_WORK* body_work );

//ƒgƒQƒ{[ƒ‹Ëo
static void gmBoss2BodyStateBallShootEnter( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateBallShootLeave( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateBallShootUpdateWaitCreate( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateBallShootUpdateCatch( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateBallShootUpdateShoot( GMS_BOSS2_BODY_WORK* body_work );

//ƒsƒ“ƒ{[ƒ‹‘O‰‰o
static void gmBoss2BodyStatePrePinBallEnter( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStatePrePinBallLeave( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStatePrePinBallUpdateWaitEffect( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStatePrePinBallUpdateWaitMotion( GMS_BOSS2_BODY_WORK* body_work );

//ƒsƒ“ƒ{[ƒ‹ˆÚ“®
static void gmBoss2BodyStatePinBallMoveEnter( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStatePinBallMoveLeave( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStatePinBallMoveUpdateMove( GMS_BOSS2_BODY_WORK* body_work );

//ƒsƒ“ƒ{[ƒ‹‰ñ“]
static void gmBoss2BodyStatePinBallRollEnter( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStatePinBallRollLeave( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStatePinBallRollUpdateSearch( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStatePinBallRollUpdateFind( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStatePinBallRollReady( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStatePinBallRollUpdateMove( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStatePinBallRollUpdateStop( GMS_BOSS2_BODY_WORK* body_work );

//Œ‚”j
static void gmBoss2BodyStateDefeatEnter( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateDefeatLeave( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateDefeatUpdateStart( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateDefeatUpdateFall( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateDefeatUpdateExplode( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateDefeatUpdateScatter( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateDefeatUpdateEnd( GMS_BOSS2_BODY_WORK* body_work );

//“¦–S
static void gmBoss2BodyStateEscapeEnter( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateEscapeLeave( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateEscapeUpdateScrollLock( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateEscapeUpdateWaitScreenOut( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2BodyStateEscapeUpdateFinalZone( GMS_BOSS2_BODY_WORK* body_work );

// =======================================================================
//ƒGƒbƒOƒ}ƒ“
// =======================================================================
static void gmBoss2EggChangeAction( 
							GMS_BOSS2_EGG_WORK* egg_work,
							GME_BOSS2_EGG_ACT_ID action_id,
							BOOL force_change = FALSE );
static void gmBoss2EggRevertAction( GMS_BOSS2_EGG_WORK* egg_work );

static void gmBoss2EggStateIdleInit( GMS_BOSS2_EGG_WORK* egg_work );
static void gmBoss2EggStateIdleUpdate( GMS_BOSS2_EGG_WORK* egg_work );
static void gmBoss2EggStateLaughInit( GMS_BOSS2_EGG_WORK* egg_work );
static void gmBoss2EggStateLaughUpdate( GMS_BOSS2_EGG_WORK* egg_work );
static void gmBoss2EggStateDamageInit( GMS_BOSS2_EGG_WORK* egg_work );
static void gmBoss2EggStateDamageUpdate( GMS_BOSS2_EGG_WORK* egg_work );
static void gmBoss2EggStateEscapeInit( GMS_BOSS2_EGG_WORK* egg_work );
static void gmBoss2EggStateEscapeUpdate( GMS_BOSS2_EGG_WORK* egg_work );

static void gmBoss2EggmanMainFuncWaitSetup( OBS_OBJECT_WORK* obj_work );
static void gmBoss2EggmanMainFunc( OBS_OBJECT_WORK* obj_work );

// =======================================================================
//ƒgƒQƒ{[ƒ‹
// =======================================================================
static void gmBoss2BallHitFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect );
static void gmBoss2BallMainFuncWaitSetup( OBS_OBJECT_WORK* obj_work );
static void gmBoss2BallMainFunc( OBS_OBJECT_WORK* obj_work );
static void gmBoss2BallInit( GMS_BOSS2_BALL_WORK* ball_work );
static void gmBoss2BallUpdateCatch( GMS_BOSS2_BALL_WORK* ball_work );
static void gmBoss2BallUpdateWaitShoot( GMS_BOSS2_BALL_WORK* ball_work );
static void gmBoss2BallUpdateShoot( GMS_BOSS2_BALL_WORK* ball_work );
static void gmBoss2BallUpdateWaitBomb( GMS_BOSS2_BALL_WORK* ball_work );
static void gmBoss2BallUpdateFlicker( GMS_BOSS2_BALL_WORK* ball_work );

// =======================================================================
//ƒGƒtƒFƒNƒg
// =======================================================================
static void gmBoss2EffDamageInit( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2EffBombsInit( 
						 GMS_BOSS2_EFF_BOMB_WORK* bomb_work,
						 OBS_OBJECT_WORK* parent_obj,
						 fx32 pos_x,
						 fx32 pos_y,
						 fx32 width,
						 fx32 height,
						 Uint32 interval_min,
						 Uint32 interval_max );
static void gmBoss2EffBombsUpdate( GMS_BOSS2_EFF_BOMB_WORK* bomb_work );

static void gmBoss2EffAfterburnerRequestCreate( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2EffAfterburnerRequestDelete( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2EffAfterburnerInit( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2EffAfterburnerMainFunc( OBS_OBJECT_WORK* obj_work );

static void gmBoss2EffAfterburnerSmokeInit( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2EffAfterburnerSmokeMainFunc( OBS_OBJECT_WORK* obj_work );

static void gmBoss2EffBodySmokeInit( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2EffBodySmokeMainFunc( OBS_OBJECT_WORK* obj_work );

static void gmBoss2EffSweatInit( GMS_BOSS2_EGG_WORK* egg_work );
static void gmBoss2EffSweatMainFunc( OBS_OBJECT_WORK* obj_work );

static OBS_OBJECT_WORK* gmBoss2EffInit( 
					OBS_DATA_WORK* data_work,
					GME_EFFECT_3DES_POS_TYPE effect_type,
					OBS_OBJECT_WORK* parent_obj_work,
					Angle16 rot_x,
					Angle16 rot_y,
					Angle16 rot_z,
					Float offset_x,
					Float offset_y,
					Float offset_z,
					BOOL flag_flip,
					BOOL flag_data_rotate);
static void gmBoss2EffBallBombInit( const VecFx32* create_pos, 
							 OBS_OBJECT_WORK* body_obj_work );
static OBS_OBJECT_WORK* gmBoss2EffBallBombPartInit( 
		const VecFx32* create_pos,
		OBS_OBJECT_WORK* body_obj_work,
		fx32 spd_x);
static void gmBoss2EffBallBombPartMainFunc( OBS_OBJECT_WORK* obj_work );
static void gmBoss2EffBlitzInit( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2EffBlitzMainFuncBlitzLineCreate( OBS_OBJECT_WORK* obj_work );
static void gmBoss2EffBlitzMainFuncBlitzLineNormal( OBS_OBJECT_WORK* obj_work );
static void gmBoss2EffBlitzMainFuncBlitzCoreL( OBS_OBJECT_WORK* obj_work );
static void gmBoss2EffBlitzMainFuncBlitzCoreR( OBS_OBJECT_WORK* obj_work );
static void gmBoss2EffBlitzMainFuncBlitzL( OBS_OBJECT_WORK* obj_work );
static void gmBoss2EffBlitzMainFuncBlitzR( OBS_OBJECT_WORK* obj_work );

static void gmBoss2EffScatterInit( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2EffScatterMainFunc( OBS_OBJECT_WORK* obj_work );
static void gmBoss2EffScatterSetParamMove( OBS_OBJECT_WORK* obj_work, BOOL right_flag );
//static void gmBoss2EffScatterSetParamSpin( OBS_OBJECT_WORK* obj_work );

static void gmBoss2EffCreateRollModel( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2EffCreateRollModelLost( GMS_BOSS2_BODY_WORK* body_work );
static void gmBoss2EffRollModelMainFunc( OBS_OBJECT_WORK* obj_work );
static void gmBoss2EffRollMainFunc( OBS_OBJECT_WORK* obj_work );

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

static OBS_ACTION3D_NN_WORK* gm_boss2_obj_3d_list = NULL;	//ƒIƒuƒWƒFƒNƒgƒŠƒXƒg

//ƒAƒNƒVƒ‡ƒ“î•ñ
static const GMS_BOSS2_PART_ACT_INFO gm_boss2_act_info_tbl[GMD_BOSS2_ACT_ID_MAX][GMD_BOSS2_PART_IDX_MAX] = {
		//MTN_ID									IS_MAINTAIN	IS_REPEAT	MTN_SPD	IS_BLEND	BLEND_SPD

	//GMD_BOSS2_ACT_ID_START
	{
		{IDB_BOSS02_BODY_MTN_B02_1_STA_01B_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_1_STA_01E_ZNM,		FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_CATCH_MOVE
	{
		{IDB_BOSS02_BODY_MTN_B02_1_ATT01_01B_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_1_ATT01_01E_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_CATCH_ARM_OPEN
	{
		{IDB_BOSS02_BODY_MTN_B02_1_ATT04_01B_ZNM,	FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_1_ATT04_01E_ZNM,	FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_CATCH_ARM_READY
	{
		{IDB_BOSS02_BODY_MTN_B02_1_ATT04_02_1B_ZNM,	FALSE,		FALSE,		1.f,	FALSE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_1_ATT04_02_1E_ZNM,	FALSE,		FALSE,		1.f,	FALSE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_CATCH_ARM_DOWN
	{
		{IDB_BOSS02_BODY_MTN_B02_1_ATT04_02_2B_ZNM,	FALSE,		TRUE,		1.f,	FALSE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_1_ATT04_02_2E_ZNM,	FALSE,		TRUE,		1.f,	FALSE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_CATCH_ARM_CLOSE
	{
		{IDB_BOSS02_BODY_MTN_B02_1_ATT04_03B_ZNM,	FALSE,		FALSE,		1.f,	FALSE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_1_ATT04_03E_ZNM,	FALSE,		FALSE,		1.f,	FALSE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_CATCH_ARM_UP
	{
		{IDB_BOSS02_BODY_MTN_B02_1_ATT04_06B_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_BODY_MTN_B02_1_ATT04_02_2B_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_CATCH_ARM_CARRY
	{
		{IDB_BOSS02_BODY_MTN_B02_1_ATT04_04B_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_1_ATT04_04E_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_CATCH_ARM_THROW
	{
		{IDB_BOSS02_BODY_MTN_B02_1_ATT04_05B_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_1_ATT04_05E_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_BALL_MOVE
	{
		{IDB_BOSS02_BODY_MTN_B02_2_ATT01_01B_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_2_ATT01_01E_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_BALL_READY
	{
		{IDB_BOSS02_BODY_MTN_B02_2_ATT04_01B_ZNM,	FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_2_ATT04_01E_ZNM,	FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_BALL_SHOOT
	{
		{IDB_BOSS02_BODY_MTN_B02_2_ATT04_03B_ZNM,	FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_2_ATT04_03E_ZNM,	FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_PINBALL_START
	{
		{IDB_BOSS02_BODY_MTN_B02_3_STA_01B_ZNM,		FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_3_STA_01E_ZNM,		FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_PINBALL_MOVE
	{
		{IDB_BOSS02_BODY_MTN_B02_3_ATT01_01B_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_3_ATT01_01E_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_PINBALL_MOVE_CLOSE
	{
		{IDB_BOSS02_BODY_MTN_B02_3_ATT01_02B_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_3_ATT01_01E_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_PINBALL_SEARCH
	{
		{IDB_BOSS02_BODY_MTN_B02_3_LOOK_01B_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_3_LOOK_01E_ZNM,		FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_PINBALL_FIND
	{
		{IDB_BOSS02_BODY_MTN_B02_3_ATT04_01B_ZNM,	FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_BODY_MTN_B02_3_ATT04_01B_ZNM,	FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS2_ACT_ID_ANGRY
	{
		{IDB_BOSS02_BODY_MTN_B02_1_ANG_01B_ZNM,		FALSE,		FALSE,		1.f,	FALSE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_1_ANG_01E_ZNM,		FALSE,		FALSE,		1.f,	FALSE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},
	
	//GMD_BOSS2_ACT_ID_ESCAPE
	{
		{IDB_BOSS02_BODY_MTN_B02_DMG02_01B_ZNM,		FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS02_EGG_MTN_B02_DMG02_01E_ZNM,		FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG
	},
};
//ƒGƒbƒOƒ}ƒ“—pƒAƒNƒVƒ‡ƒ“î•ñ
static const GMS_BOSS2_PART_ACT_INFO gm_boss2_egg_act_info_tbl[GMD_BOSS2_EGG_ACT_ID_MAX] = {
	//MTN_ID									IS_MAINTAIN	IS_REPEAT	MTN_SPD	IS_BLEND	BLEND_SPD

	//GMD_BOSS2_EGG_ACT_ID_LAUGH_L
	{IDB_BOSS02_EGG_MTN_B02_1_STA_01E_ZNM,		FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG

	//GMD_BOSS2_EGG_ACT_ID_LAUGH_R
	{IDB_BOSS02_EGG_MTN_B02_1_STA_02E_ZNM,		FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG

	//GMD_BOSS2_EGG_ACT_ID_DAMAGE
	{IDB_BOSS02_EGG_MTN_B02_DMG01_01E_ZNM,		FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS2_BLEND_SPD,	FALSE},		// EGG

};

//‰Šú‰»ŠÖ”ƒe[ƒuƒ‹
const static GMF_BOSS2_BODY_STATE_FUNC gm_boss2_body_state_func_tbl_enter[GMD_BOSS2_BODY_STATE_MAX]	= {
	NULL,
	gmBoss2BodyStateStartEnter,

	gmBoss2BodyStateCatchMoveEnter,
	gmBoss2BodyStateCatchArmEnter,
	gmBoss2BodyStateCatchCarryEnter,

	gmBoss2BodyStatePreBallEnter,
	gmBoss2BodyStateBallMoveEnter,
	gmBoss2BodyStateBallShootEnter,

	gmBoss2BodyStatePrePinBallEnter,
	gmBoss2BodyStatePinBallMoveEnter,
	gmBoss2BodyStatePinBallRollEnter,

	gmBoss2BodyStateDefeatEnter,
	gmBoss2BodyStateEscapeEnter,
};

//I—¹ŠÖ”ƒe[ƒuƒ‹
const static GMF_BOSS2_BODY_STATE_FUNC gm_boss2_body_state_func_tbl_leave[GMD_BOSS2_BODY_STATE_MAX]	= {
	NULL,
	gmBoss2BodyStateStartLeave,

	gmBoss2BodyStateCatchMoveLeave,
	gmBoss2BodyStateCatchArmLeave,
	gmBoss2BodyStateCatchCarryLeave,

	gmBoss2BodyStatePreBallLeave,
	gmBoss2BodyStateBallMoveLeave,
	gmBoss2BodyStateBallShootLeave,

	gmBoss2BodyStatePrePinBallLeave,
	gmBoss2BodyStatePinBallMoveLeave,
	gmBoss2BodyStatePinBallRollLeave,

	gmBoss2BodyStateDefeatLeave,
	gmBoss2BodyStateEscapeLeave,
};

//ƒm[ƒhƒCƒ“ƒfƒNƒXƒŠƒXƒg
static const s32 g_boss2_node_index_list[GMD_BOSS2_BODY_SNM_INDEX_MAX] = {
	2,	//–{‘ÌÚ‘±iƒGƒbƒOƒ}ƒ“AƒAƒtƒ^ƒo[ƒij
#if _IPHONE
	21,	//‚Æ‚°ƒ{[ƒ‹Ú‘±
#else
	20,	//ƒgƒQƒ{[ƒ‹Ú‘±
#endif

	3,	//‚Â‚©‚İÚ‘±iƒ\ƒjƒbƒNA“dŒ‚ƒRƒAj
	4,	//ƒA[ƒ€
	5,	//ƒA[ƒ€
	6,	//ƒA[ƒ€
	7,	//ƒA[ƒ€
	8,	//ƒA[ƒ€
	9,	//ƒA[ƒ€
	10,	//ƒA[ƒ€
	11,	//ƒA[ƒ€

	12,	//ƒA[ƒ€Ú‘±ƒp[ƒc
	13,	//ƒA[ƒ€Ú‘±ƒp[ƒc
	14,	//ƒA[ƒ€Ú‘±ƒp[ƒc
	15,	//ƒA[ƒ€Ú‘±ƒp[ƒc
};

//----- Global Functions ----------------------------------------------------
// =======================================================================
// GmBoss2Build
/*!
 *	ƒ{ƒX2\’z
 */
// =======================================================================
void GmBoss2Build(void)
{
	void* amb_data = GmBoss2GetGameDatEnemyArc();

	AMS_AMB_HEADER* amb_model = (AMS_AMB_HEADER*)ObjDataLoadAmbIndex(
			NULL, 
			IDB_BOSS02_BOSS02_MDL_AMB, 
			amb_data );
	AMS_AMB_HEADER* amb_texture = (AMS_AMB_HEADER*)ObjDataLoadAmbIndex(
			NULL, 
			IDB_BOSS02_BOSS02_TEX_AMB,
			amb_data );
	
	//ƒ‚ƒfƒ‹
	gm_boss2_obj_3d_list = GmGameDBuildRegBuildModel(
			amb_model, 
			amb_texture,
			NND_DRAWOBJ_SHADER_USER_PROFILE_TOON );
	
	//ƒ‚[ƒVƒ‡ƒ“
	ObjDataLoadAmbIndex(
			ObjDataGet(GMD_DWORK_NO_BOSS_02_BODY_MTN),
			IDB_BOSS02_BOSS02_BODY_MTN_AMB,
			amb_data );
	ObjDataLoadAmbIndex(
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EGG_MTN),
			IDB_BOSS02_BOSS02_EGG_MTN_AMB, 
			amb_data );

	//ƒ}ƒeƒŠƒAƒ‹
	ObjDataLoadAmbIndex(
			ObjDataGet(GMD_DWORK_NO_BOSS_02_BODY_MAT),
			IDB_BOSS02_BOSS02_BODY_MAT_AMB,
			amb_data );	
	
	//ƒGƒtƒFƒNƒg
	ObjDataLoadAmbIndex(
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_BLITZ00_ES),
			IDB_BOSS02_EFF_BLITZ_CORA_AME,
			amb_data );
	ObjDataLoadAmbIndex(
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_BLITZ01_ES),
			IDB_BOSS02_EFF_BLITZ_AME,
			amb_data );
	ObjDataLoadAmbIndex(
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_BLITZ02_ES),
			IDB_BOSS02_EFF_BLITZ_ARM_AME,
			amb_data );
	ObjDataLoadAmbIndex(
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_BALL_ES),
			IDB_BOSS02_EFF_BLITZ_BALL_AME,
			amb_data );
	ObjDataLoadAmbIndex(
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_BALL_PART_ES),
			IDB_BOSS02_EFF_BALL_AME,
			amb_data );
	
	//ƒGƒtƒFƒNƒg‚ğVRAM‚É
	GmEfctBossBuildSingleDataReg(
			IDB_BOSS02_EFF_BS2_TEX_AMB,
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_AMBTEX),
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_TEXLIST),
			0, 
			NULL, 
			NULL,
			amb_data );
	GmEfctBossBuildSingleDataReg(
			IDB_BOSS02_EFF_BS2_TEX_AMB,
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_AMBTEX),
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_TEXLIST),
			0, 
			NULL, 
			NULL,
			amb_data );
	GmEfctBossBuildSingleDataReg(
			IDB_BOSS02_EFF_BS2_TEX_AMB,
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_AMBTEX),
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_TEXLIST),
			0, 
			NULL, 
			NULL,
			amb_data );
	GmEfctBossBuildSingleDataReg(
			IDB_BOSS02_EFF_BS2_TEX_AMB,
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_AMBTEX),
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_TEXLIST),
			0, 
			NULL, 
			NULL,
			amb_data );
	GmEfctBossBuildSingleDataReg(
			IDB_BOSS02_EFF_BS2_TEX_AMB,
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_AMBTEX),
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_TEXLIST),
			0, 
			NULL, 
			NULL,
			amb_data );

	//‰ñ“]UŒ‚ƒGƒtƒFƒNƒg‚ğVRAM‚É
	GmEfctBossBuildSingleDataReg(
			IDB_BOSS02_EFF_BS2_TEX_MD_RO_AMB,
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_ROLL_AMBTEX),
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_ROLL_TEXLIST),
			IDB_BOSS02_ROLL_ZNO,
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_ROLL_MDL_DATA),
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_ROLL_OBJECT),
			amb_data);
	GmEfctBossBuildSingleDataReg(
			IDB_BOSS02_EFF_BS2_TEX_AMB,
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_AMBTEX),
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_TEXLIST),
			0, 
			NULL, 
			NULL,
			amb_data );
	GmEfctBossBuildSingleDataReg(
			IDB_BOSS02_EFF_BS2_TEX_MD_RO_AMB,
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_ROLL_AMBTEX),
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_ROLL_TEXLIST),
			IDB_BOSS02_ROLL_ZNO,
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_ROLL_MDL_DATA),
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_ROLL_OBJECT),
			amb_data);
			
}

// =======================================================================
// GmBoss2Flush
/*!
 *	ƒ{ƒX2‰ğ•ú
 */
// =======================================================================
void GmBoss2Flush( void )
{
	//ƒGƒtƒFƒNƒg‰ğ•ú
	GmEfctBossFlushSingleDataInit();	//ƒ{ƒXê—pEFFƒtƒ‰ƒbƒVƒ…ŠJn
	
	//ƒGƒtƒFƒNƒg
	ObjDataRelease( ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_BALL_PART_ES) );
	ObjDataRelease( ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_BALL_ES) );
	ObjDataRelease( ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_BLITZ02_ES) );
	ObjDataRelease( ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_BLITZ01_ES) );
	ObjDataRelease( ObjDataGet(GMD_DWORK_NO_BOSS_02_EF_BLITZ00_ES) );
	
	//ƒ‚[ƒVƒ‡ƒ“
	ObjDataRelease( ObjDataGet(GMD_DWORK_NO_BOSS_02_EGG_MTN) );
	ObjDataRelease( ObjDataGet(GMD_DWORK_NO_BOSS_02_BODY_MTN) );
	
	//ƒ}ƒeƒŠƒAƒ‹
	ObjDataRelease( ObjDataGet(GMD_DWORK_NO_BOSS_02_BODY_MAT) );

	//ƒ‚ƒfƒ‹‰ğ•ú
	void* amb_data = GmBoss2GetGameDatEnemyArc();
	AMS_AMB_HEADER* amb_model = (AMS_AMB_HEADER*)ObjDataLoadAmbIndex(
			NULL, 
			IDB_BOSS02_BOSS02_MDL_AMB, 
			amb_data );	
	GmGameDBuildRegFlushModel( gm_boss2_obj_3d_list, amb_model->file_num );	
	gm_boss2_obj_3d_list = NULL;
}

// =======================================================================
// GmBoss2GetGameDatEnemyArc
/*!
 *	ƒA[ƒJƒCƒuæ“¾
 *
 *	@return ƒA[ƒJƒCƒu
 */
// =======================================================================
void* GmBoss2GetGameDatEnemyArc( void )
{
	return g_gm_gamedat_enemy_arc;
}

// ==========================================================================
// GmBoss2Init
/*!
 *	ƒ{ƒX2ŠÇ—‰Šú‰»ŠÖ”
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmBoss2Init(
							 GMS_EVE_RECORD_EVENT *eve_rec,
							 fx32 pos_x, 
							 fx32 pos_y, 
							 u8 type)
{	
	UNREFERENCED_PARAMETER( type );

	//-----------------------------------------
	//ŠÇ—
	//-----------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_BOSS2_MGR_WORK* mgr_work = (GMS_BOSS2_MGR_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec,
			pos_x, 
			pos_y,
			sizeof(GMS_BOSS2_MGR_WORK),
			"BOSS2_MGR");
	amAssert( mgr_work );

	OBS_OBJECT_WORK* mgr_obj_work = &mgr_work->ene_3d.ene_com.obj_work;
	
	//ƒ[ƒN
	mgr_obj_work->flag |= OBD_OBJECT_NOCLIP;
	mgr_obj_work->disp_flag |= OBD_DISP_NODISP;
	mgr_obj_work->move_flag	|= (OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);

	//ƒz[ƒ~ƒ“ƒO–³Œø
	mgr_work->ene_3d.ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;
	
	//ˆ—ŠÖ”
	mgr_obj_work->ppFunc = gmBoss2MgrMainFuncWaitLoad;

	//ƒ‰ƒCƒt
	if ( GmBsCmnIsFinalZoneType(mgr_obj_work) ){
		mgr_work->life = GMD_BOSS2_MGR_LIFE_FINAL;
	}
	else{
		mgr_work->life = GMD_BOSS2_MGR_LIFE;
	}
	
	

	return mgr_obj_work;
}

// ==========================================================================
// GmBoss2BodyInit
/*!
 *	ƒ{ƒX2–{‘Ì‰Šú‰»ŠÖ”
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmBoss2BodyInit(
								 GMS_EVE_RECORD_EVENT *eve_rec,
								 fx32 pos_x, 
								 fx32 pos_y, 
								 u8 type)
{	
	UNREFERENCED_PARAMETER( type );

	//-----------------------------------------
	//ƒIƒuƒWƒFƒNƒgƒ[ƒN
	//-----------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec,
			pos_x, 
			pos_y,
			sizeof(GMS_BOSS2_BODY_WORK),
			"BOSS2_BODY");
	amAssert( body_work );

	GMS_ENEMY_3D_WORK* ene_3d = &body_work->ene_3d;
	OBS_OBJECT_WORK* obj_work = &ene_3d->ene_com.obj_work;

	//-----------------------------------------
	//ƒ‚ƒfƒ‹Aƒ‚[ƒVƒ‡ƒ“
	//-----------------------------------------
	//ƒ‚ƒfƒ‹
	ObjObjectCopyAction3dNNModel(
			obj_work,
			&gm_boss2_obj_3d_list[IDB_BOSS02_MDL_B02_BODY_ZNO],
			&ene_3d->obj_3d);
	
	//ƒ‚[ƒVƒ‡ƒ“
	ObjObjectAction3dNNMotionLoad(
			obj_work,
			0,
			TRUE,
			ObjDataGet(GMD_DWORK_NO_BOSS_02_BODY_MTN),
			NULL,
			0,
			NULL);
#if !_IPHONE
	//ƒ}ƒeƒŠƒAƒ‹
	ObjObjectAction3dNNMaterialMotionLoad( 
			obj_work,
			0,				//reg_file_id
			ObjDataGet(GMD_DWORK_NO_BOSS_02_BODY_MAT),
			NULL,			//mtn_data_path
			0,
			NULL
	);
	ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_BOSS02_BODY_MAT_B02_BODY_ZNV );
#endif // _IPHONE
	//-----------------------------------------
	//‹éŒ`
	//-----------------------------------------
	//ƒ‰ƒCƒt
	ene_3d->ene_com.vit = 1;

	//–{‘Ì
	ObjRectWorkSet(
			&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY],
			-16, -16, 16, 16);
	ObjRectGroupSet(
			&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY], 
			GMD_OBJ_RECT_GROUP_ENEMY, 
			GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER | GMD_OBJ_RECT_TARGET_GROUPFLAG_ENEMY );
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag |= OBD_RECT_OUT;

	//UŒ‚
	body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_RECT_OUT;

	//‚­‚ç‚¢
	ObjRectWorkSet(
			&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF],
			-26, -26, 26, 26 );
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].ppDef	= gmBoss2BodyDefFunc;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag |= OBD_RECT_OUT;

	//ƒA[ƒ€‹éŒ`İ’è
	gmBoss2BodySetRectArm( body_work );

	//‹éŒ`Ø‚è‘Ö‚¦
	gmBoss2BodySetRectNormal( body_work );

	//’nŒ`‹éŒ`
	ObjObjectFieldRectSet( 
			obj_work, 
			-GMD_BOSS2_BODY_FIELD_RECT_SIZE, 
			-GMD_BOSS2_BODY_FIELD_RECT_SIZE, 
			GMD_BOSS2_BODY_FIELD_RECT_SIZE,
			GMD_BOSS2_BODY_FIELD_RECT_SIZE );

	//-----------------------------------------
	//ƒ[ƒN
	//-----------------------------------------
	//À•W
	obj_work->pos.z	= GMD_OBJ_ENEMY_POS_Z_FRONT;

	//ƒtƒ‰ƒO
	obj_work->flag |= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag |= OBD_DISP_HFLIP | OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP;
	obj_work->move_flag &= ~OBD_MOVE_FALL;
	obj_work->move_flag |= OBD_MOVE_NOSPD | OBD_MOVE_NOSPDM | OBD_MOVE_JUMP;
	
	
	//ƒuƒŒƒ“ƒh‘¬“x
	obj_work->obj_3d->blend_spd	= GMD_BOSS2_BLEND_SPD;
	
	//ƒgƒD[ƒ“
	ObjDrawObjectSetToon( obj_work );
	
	//ƒ†[ƒU•`‰æƒXƒe[ƒg
	obj_work->disp_flag |= OBD_DISP_DRAWSTATE;

	//SEƒnƒ“ƒhƒ‹Šm•Û
	body_work->se_handle = GsSoundAllocSeHandle();
	
	//ˆ—
	obj_work->ppFunc = gmBoss2BodyMainFuncWaitSetup;
	obj_work->ppOut = gmBoss2BodyOutFunc;
	
	//‰½‚à‚µ‚È‚¢ó‘Ô‚Ö
	gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_NO_OPERATION );

#if _IPHONE
	// ê—pƒ‰ƒCƒgİ’è
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}

// ==========================================================================
// GmBoss2EggInit
/*!
 *	ƒ{ƒX2ƒGƒbƒOƒ}ƒ“‰Šú‰»ŠÖ”
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmBoss2EggInit(
								 GMS_EVE_RECORD_EVENT *eve_rec,
								 fx32 pos_x, 
								 fx32 pos_y, 
								 u8 type)
{	
	UNREFERENCED_PARAMETER( type );

	//-----------------------------------------
	//ƒIƒuƒWƒFƒNƒgƒ[ƒN
	//-----------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_BOSS2_EGG_WORK* eggman_work = (GMS_BOSS2_EGG_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec,
			pos_x, 
			pos_y,
			sizeof(GMS_BOSS2_EGG_WORK),
			"BOSS2_EGG");
	amAssert( eggman_work );

	GMS_ENEMY_3D_WORK* ene_3d = &eggman_work->ene_3d;
	OBS_OBJECT_WORK* obj_work = &ene_3d->ene_com.obj_work;

	//-----------------------------------------
	//ƒ‚ƒfƒ‹Aƒ‚[ƒVƒ‡ƒ“
	//-----------------------------------------
	//ƒ‚ƒfƒ‹
	ObjObjectCopyAction3dNNModel(
			obj_work,
			&gm_boss2_obj_3d_list[IDB_BOSS02_MDL_EGGMAN_ZNO],
			&ene_3d->obj_3d);
	
	//ƒ‚[ƒVƒ‡ƒ“
	ObjObjectAction3dNNMotionLoad(
			obj_work,
			0,
			TRUE,
			ObjDataGet(GMD_DWORK_NO_BOSS_02_EGG_MTN),
			NULL,
			0,
			NULL);

	//-----------------------------------------
	//‹éŒ`
	//-----------------------------------------
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_RECT_NOHIT | OBD_RECT_OUT;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag |= OBD_RECT_NOHIT | OBD_RECT_OUT;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag |= OBD_RECT_NOHIT | OBD_RECT_OUT;

	//-----------------------------------------
	//ƒ[ƒN
	//-----------------------------------------
	//ƒtƒ‰ƒO
	obj_work->flag |= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag |= OBD_DISP_HFLIP | OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP;
	obj_work->move_flag |= OBD_MOVE_NOCOLFIELD | OBD_MOVE_NOCOL;
	obj_work->move_flag &= ~OBD_MOVE_FALL;
	
	//ƒuƒŒƒ“ƒh‘¬“x
	obj_work->obj_3d->blend_spd	= GMD_BOSS2_BLEND_SPD;
	
	//ƒgƒD[ƒ“
	ObjDrawObjectSetToon( obj_work );
	
	//ƒ†[ƒU•`‰æƒXƒe[ƒg
	obj_work->disp_flag |= OBD_DISP_DRAWSTATE;

	//ƒz[ƒ~ƒ“ƒO–³Œø
	ene_3d->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;
	
	//ˆ—
	obj_work->ppFunc = gmBoss2EggmanMainFuncWaitSetup;
	
#if _IPHONE
	// ê—pƒ‰ƒCƒgİ’è
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}

// ==========================================================================
// GmBoss2BallInit
/*!
 *	ƒ{ƒX2ƒgƒQƒ{[ƒ‹‰Šú‰»ŠÖ”
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmBoss2BallInit(
								 GMS_EVE_RECORD_EVENT *eve_rec,
								 fx32 pos_x, 
								 fx32 pos_y, 
								 u8 type)
{	
	UNREFERENCED_PARAMETER( type );

	//-----------------------------------------
	//ƒIƒuƒWƒFƒNƒgƒ[ƒN
	//-----------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_BOSS2_BALL_WORK* ball_work = (GMS_BOSS2_BALL_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec,
			pos_x, 
			pos_y,
			sizeof(GMS_BOSS2_BALL_WORK),
			"BOSS2_BALL");
	amAssert( ball_work );

	GMS_ENEMY_3D_WORK* ene_3d = &ball_work->ene_3d;
	OBS_OBJECT_WORK* obj_work = &ene_3d->ene_com.obj_work;

	//-----------------------------------------
	//ƒ‚ƒfƒ‹Aƒ‚[ƒVƒ‡ƒ“
	//-----------------------------------------
	//ƒ‚ƒfƒ‹
	ObjObjectCopyAction3dNNModel(
			obj_work,
			&gm_boss2_obj_3d_list[IDB_BOSS02_MDL_B02_BALL_ZNO],
			&ene_3d->obj_3d);

	//-----------------------------------------
	//‹éŒ`
	//-----------------------------------------
	//UŒ‚
	ObjRectWorkSet( 
			&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK],
			-8, -8, 8, 8 );
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].ppHit = gmBoss2BallHitFunc;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_RECT_OUT;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag |= OBD_RECT_NOHIT;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag |= OBD_RECT_NOHIT;

	//ƒz[ƒ~ƒ“ƒO–³Œø
	ene_3d->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;

	//-----------------------------------------
	//ƒ[ƒN
	//-----------------------------------------
	//ƒtƒ‰ƒO
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	
	//ƒuƒŒƒ“ƒh‘¬“x
	obj_work->obj_3d->blend_spd	= GMD_BOSS2_BLEND_SPD;
	
	//ƒgƒD[ƒ“
	ObjDrawObjectSetToon( obj_work );
	
	//ƒ†[ƒU•`‰æƒXƒe[ƒg
	obj_work->disp_flag |= OBD_DISP_DRAWSTATE;

	//’nŒ`‹éŒ`
	ObjObjectFieldRectSet( obj_work, -4, -8, 4, 6 );
	
	//ˆ—
	obj_work->ppFunc = gmBoss2BallMainFuncWaitSetup;
	
#if _IPHONE
	// ê—pƒ‰ƒCƒgİ’è
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}

//----- Local Functions -----------------------------------------------------

// ==========================================================================
// gmBoss2ChangeTextureBurnt
/*!
 *	ƒeƒNƒXƒ`ƒƒ‚ğ•‚±‚°‚É•ÏX
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@note uvƒXƒNƒ[ƒ‹iu‚ğ0.5ƒIƒtƒZƒbƒgj
 */
// ==========================================================================
void gmBoss2ChangeTextureBurnt( OBS_OBJECT_WORK *obj_work )
{
	amAssert( obj_work );
	amAssert( obj_work->obj_3d );
	amAssert( obj_work->disp_flag & OBD_DISP_DRAWSTATE );
	
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_TEXOFFSET;
	obj_work->obj_3d->draw_state.texoffset[0].mode	= NNE_MATCTRLMODE_ADD;
	obj_work->obj_3d->draw_state.texoffset[0].u	= 0.5f;
}

// =======================================================================
// gmBoss2ExitFunc
/*!
 *	I—¹ŠÖ”
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2ExitFunc( MTS_TASK_TCB *tcb )
{
	//ŠÇ—‚©‚ç‰ğ•ú
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)mtTaskGetTcbWork( tcb );
	gmBoss2MgrDeleteObject( obj_work );

	//ƒGƒlƒ~[‹¤’ÊI—¹ˆ—
	GmEnemyDefaultExit( tcb );
}

// =======================================================================
// gmBoss2EffectExitFunc
/*!
 *	I—¹ŠÖ”
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffectExitFunc( MTS_TASK_TCB *tcb )
{

	//ŠÇ—‚©‚ç‰ğ•ú
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)mtTaskGetTcbWork( tcb );
	gmBoss2MgrDeleteObject( obj_work );

	//ƒGƒtƒFƒNƒg‹¤’ÊI—¹ˆ—
	GmEffectDefaultExit( tcb );

}

// =======================================================================
// gmBoss2CheckScrollLocked
/*!
 *	ƒXƒNƒ[ƒ‹ƒƒbƒN”»’è
 *
 *	@retval TRUE ƒXƒNƒ[ƒ‹ƒƒbƒN‚³‚ê‚Ä‚¢‚é
 *	@retval FALSE ƒXƒNƒ[ƒ‹ƒƒbƒN‚³‚ê‚Ä‚¢‚È‚¢
 */
// =======================================================================
BOOL gmBoss2CheckScrollLocked( void )
{
	if ( (g_gm_main_system.game_flag & GMD_GAME_FLAG_SCR_LIMIT_BUSY) ){
		return TRUE;
	}
	return FALSE;
}

// ==========================================================================
//ŠÇ—
// ==========================================================================

// =======================================================================
// gmBoss2MgrCheckSetupComplete
/*!
 *	¶¬Š®—¹Šm”F
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@retval TRUE “Ç‚İ‚İŠ®—¹
 *	@retval FALSE “Ç‚İ‚İ‘Ò‚¿
 */
// =======================================================================
BOOL gmBoss2MgrCheckSetupComplete( GMS_BOSS2_MGR_WORK* mgr_work )
{
	amAssert( mgr_work );

	if ( mgr_work->flag & GMD_BOSS2_MGR_FLAG_SETUP_COMPLETE ){
		return TRUE;
	}
	return FALSE;
}

// =======================================================================
// gmBoss2MgrGetMgrWork
/*!
 *	ƒ}ƒl[ƒWƒƒƒ[ƒN‚ğæ“¾
 *
 *	@param obj_work_parts	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
GMS_BOSS2_MGR_WORK* gmBoss2MgrGetMgrWork( OBS_OBJECT_WORK* obj_work_parts )
{
	amAssert( obj_work_parts );
	amAssert( obj_work_parts->user_work );

	return (GMS_BOSS2_MGR_WORK*)obj_work_parts->user_work;
}

// =======================================================================
// gmBoss2MgrAddObject
/*!
 *	ƒp[ƒc’Ç‰Á
 *
 *	@note ŠÇ—ƒ[ƒN‚ÌƒIƒuƒWƒFƒNƒg¶¬”‚ğƒCƒ“ƒNƒŠƒƒ“ƒg‚µA
 *		  ’Ç‰ÁƒIƒuƒWƒFƒNƒg‚Ìuser_work‚ÉŠÇ—ƒ[ƒN‚ğİ’è‚·‚é
 *		  ƒIƒuƒWƒFƒNƒgíœ‚ÉAŠÇ—ƒ[ƒN‚Ì¶¬”‚ğƒfƒNƒŠƒƒ“ƒg‚·‚é
 *
 *	@param obj_work_mgr		[io] ŠÇ—ƒ[ƒN
 *	@param obj_work_parts	[io] ’Ç‰Á‚·‚éƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2MgrAddObject( GMS_BOSS2_MGR_WORK* mgr_work, OBS_OBJECT_WORK* obj_work_parts )
{
	amAssert( mgr_work );
	amAssert( obj_work_parts );

	++mgr_work->obj_create_count;
	obj_work_parts->user_work = (u32)mgr_work;
}

// =======================================================================
// gmBoss2MgrDeleteObject
/*!
 *	ƒp[ƒc“o˜^íœ
 *
 *	@note ƒIƒuƒWƒFƒNƒgíœ‚ÉAŠÇ—ƒ[ƒN‚Ì¶¬”‚ğƒfƒNƒŠƒƒ“ƒg‚·‚é
 *
 *	@param obj_work_parts	[io] ’Ç‰Á‚·‚éƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2MgrDeleteObject( OBS_OBJECT_WORK* obj_work_parts )
{
	amAssert( obj_work_parts );

	GMS_BOSS2_MGR_WORK* mgr_work = gmBoss2MgrGetMgrWork( obj_work_parts );
	amAssert( mgr_work );
	amAssert( mgr_work->obj_create_count > 0 );

	--mgr_work->obj_create_count;
	obj_work_parts->user_work = 0;
}

// =======================================================================
// gmBoss2MgrMainFuncWaitLoad
/*!
 *	ƒƒCƒ“ˆ—i“Ç‚İ‚İ‘Ò‚¿j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2MgrMainFuncWaitLoad( OBS_OBJECT_WORK* obj_work )
{
	amAssert( obj_work );

	//ƒtƒ@ƒCƒiƒ‹ƒ][ƒ“‚Ìê‡Aƒf[ƒ^“Ç‚İ‚İŠ®—¹”»’è
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		if ( !GmMainDatLoadBossBattleLoadCheck(GMD_GAMEDAT_LOAD_BOSS_TYPE_2) ){
			return;
		}
	}

	GMS_BOSS2_MGR_WORK* mgr_work = (GMS_BOSS2_MGR_WORK*)obj_work;

	//-----------------------------------------
	//–{‘Ì
	//-----------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)GmEventMgrLocalEventBirth(
			GMD_EVENT_ID_BOSS2_BODY,
			obj_work->pos.x, obj_work->pos.y,
			0,
			0,0,0,0,
			0 );
	amAssert( body_work );

	OBS_OBJECT_WORK* body_obj_work = &body_work->ene_3d.ene_com.obj_work;

	//e
	body_obj_work->parent_obj = obj_work;

	//–{‘Ìƒp[ƒc“o˜^
	body_work->parts_objs[GMD_BOSS2_PART_IDX_BODY] = body_obj_work;

	//ŠÇ—‚É“o˜^
	mgr_work->body_work = body_work;
	gmBoss2MgrAddObject( mgr_work, body_obj_work );

	//I—¹ˆ—
	mtTaskChangeTcbDestructor( body_obj_work->tcb, gmBoss2BodyExit );

	//-----------------------------------------
	//ƒGƒbƒOƒ}ƒ“
	//-----------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_BOSS2_EGG_WORK* eggman_work = (GMS_BOSS2_EGG_WORK*)GmEventMgrLocalEventBirth(
			GMD_EVENT_ID_BOSS2_EGG,
			obj_work->pos.x, obj_work->pos.y,
			0,
			0,0,0,0,
			0 );
	amAssert( eggman_work );

	OBS_OBJECT_WORK* eggman_obj_work = &eggman_work->ene_3d.ene_com.obj_work;

	//e
	eggman_obj_work->parent_obj = body_obj_work;	

	//ŠÇ—
	gmBoss2MgrAddObject( mgr_work, eggman_obj_work );

	//I—¹ˆ—
	mtTaskChangeTcbDestructor( eggman_obj_work->tcb, gmBoss2ExitFunc );

	//–{‘Ìƒp[ƒc“o˜^
	body_work->parts_objs[GMD_BOSS2_PART_IDX_EGG] = eggman_obj_work;

	obj_work->ppFunc = gmBoss2MgrMainFuncWaitSetup;
}

// =======================================================================
// gmBoss2MgrMainFuncWaitSetup
/*!
 *	ƒƒCƒ“ˆ—i¶¬‘Ò‚¿j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2MgrMainFuncWaitSetup( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_MGR_WORK* mgr_work = (GMS_BOSS2_MGR_WORK*)obj_work;
	amAssert( mgr_work );
	GMS_BOSS2_BODY_WORK* body_work = mgr_work->body_work;
	amAssert( body_work );

	for ( s32 i = 0; GMD_BOSS2_PART_IDX_MAX > i; ++i ){
		if ( !body_work->parts_objs[i] ) {
			return;
		}
	}

	//“Ç‚İ‚İI—¹
	mgr_work->flag |= GMD_BOSS2_MGR_FLAG_SETUP_COMPLETE;
	obj_work->ppFunc = gmBoss2MgrMainFunc;
}

// =======================================================================
// gmBoss2MgrMainFunc
/*!
 *	ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2MgrMainFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_MGR_WORK* mgr_work = (GMS_BOSS2_MGR_WORK*)obj_work;
	amAssert( mgr_work );

	//ƒIƒuƒWƒFƒNƒgíœ
	if ( mgr_work->flag & GMD_BOSS2_MGR_FLAG_CLEAR_BOSS ){
		OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ(mgr_work->body_work);
		amAssert( body_obj_work );
		
		//íœ—v‹
		body_obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		mgr_work->body_work	= NULL;
		
		//ƒƒCƒ“ˆ—I—¹
		obj_work->ppFunc = gmBoss2MgrMainFuncWaitRelease;
	}
}

// =======================================================================
// gmBoss2MgrMainFuncWaitRelease
/*!
 *	ƒƒCƒ“ˆ—i‰ğ•ú‘Ò‚¿j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2MgrMainFuncWaitRelease( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_MGR_WORK* mgr_work = (GMS_BOSS2_MGR_WORK*)obj_work;
	amAssert( mgr_work );

	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		//ƒIƒuƒWƒFƒNƒgíœ‘Ò‚¿
		if ( mgr_work->obj_create_count > 0 ){
			return;
		}

		//ƒtƒ@ƒCƒiƒ‹ƒ][ƒ“‚Å‚ÍŠÇ—‚àÁ‹
		GMS_ENEMY_COM_WORK* ene_com	= (GMS_ENEMY_COM_WORK*)obj_work;
		ene_com->enemy_flag |= GMD_ENEMY_FLAG_DIE;
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		
		//ƒf[ƒ^ŠJ•ú—v‹
		GmGameDatReleaseBossBattleStart( GMD_GAMEDAT_LOAD_BOSS_TYPE_2 );

		//ƒXƒNƒ[ƒ‹ƒƒbƒN‰ğœ
		GmGmkCamScrLimitRelease(
				GMD_GMK_SCR_LMT_RELEASE_TOP | GMD_GMK_SCR_LMT_RELEASE_RIGHT | GMD_GMK_SCR_LMT_RELEASE_BOTTOM );

		//oŒûƒVƒƒƒbƒ^[ŠJ‚¯‚é
		OBS_OBJECT_WORK* obj_work_shutter_out = gmBoss2BodySearchShutterOut();
		if ( obj_work_shutter_out ){
			GmGmkShutterOutChangeModeOpen(obj_work_shutter_out);
		}
	}
	obj_work->ppFunc = NULL;
}

// ==========================================================================
//–{‘Ì
// ==========================================================================

// =======================================================================
// gmBoss2BodyExit
/*!
 *	I—¹ˆ—
 *
 *	@param tcb	[io] TCB
 *
 *	@note ƒm[ƒhƒ}ƒgƒŠƒNƒXæ“¾ŠÖ˜A‚Ì‰ğ•ú
 */
// =======================================================================
void gmBoss2BodyExit( MTS_TASK_TCB* tcb )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)mtTaskGetTcbWork( tcb );
	amAssert( body_work );
	OBS_OBJECT_WORK* obj_work = &body_work->ene_3d.ene_com.obj_work;

	//ƒ{ƒXƒ‚[ƒVƒ‡ƒ“ƒR[ƒ‹ƒoƒbƒNƒVƒXƒeƒ€‚ğ‰ğ•ú
	GmBsCmnClearBossMotionCBSystem( obj_work );

	//ƒm[ƒhƒ}ƒgƒŠƒNƒXæ“¾ˆ—ƒ[ƒN‚ğ‰ğ•ú
	GmBsCmnDeleteSNMWork( &body_work->snm_work );

	//ƒm[ƒhƒ}ƒgƒŠƒNƒX‘€ìˆ—ƒR[ƒ‹ƒoƒbƒN‚ğ‰ğ•ú
	GmBsCmnClearCNMCb( obj_work );

	//ƒm[ƒhƒ}ƒgƒŠƒNƒX‘€ìˆ—ŠÇ—ƒ[ƒN‚ğ‰ğ•ú
	GmBsCmnDeleteCNMMgrWork( &body_work->cnm_mgr_work );

	//SEƒnƒ“ƒhƒ‹‰ğ•ú
	if ( body_work->se_handle ){
		GmSoundStopSE( body_work->se_handle );
		GsSoundFreeSeHandle( body_work->se_handle );
		body_work->se_handle = NULL;
	}

	//ƒ{ƒX2—pI—¹ˆ—
	gmBoss2ExitFunc( tcb );
}

// =======================================================================
// gmBoss2BodyReactionPlayer
/*!
 *	ƒvƒŒƒCƒ„‚ğ‚Í‚¶‚­
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyReactionPlayer( OBS_OBJECT_WORK* obj_work_player, const OBS_OBJECT_WORK* obj_work_body )
{
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)obj_work_player;
	amAssert( player_work );

	//’µ‚Ë•Ô‚è‚É
	GmPlySeqAtkReactionInit( player_work );

	//ƒWƒƒƒ“ƒvİ’è
	GmPlySeqSetJumpState( 
			player_work,
			0,
			(GMD_PLY_SEQ_SETJUMPSTATE_IGNORE_JUMPBTN | GMD_PLY_SEQ_SETJUMPSTATE_NOHOMING) );

	//ˆÚ“®’l
	obj_work_player->spd_m = 0;
	if ( obj_work_player->move.x >= 0 ){
		obj_work_player->spd.x = -GMD_BOSS2_BODY_DEF_PLAYER_MOVE_X;
	}
	else {
		obj_work_player->spd.x = GMD_BOSS2_BODY_DEF_PLAYER_MOVE_X;
	}

	if ( obj_work_player->pos.y <= obj_work_body->pos.y ){
		obj_work_player->spd.y = -GMD_BOSS2_BODY_DEF_PLAYER_MOVE_Y;
	}
	else {
		obj_work_player->spd.y = GMD_BOSS2_BODY_DEF_PLAYER_MOVE_Y;
	}

	//ƒWƒƒƒ“ƒv’†ˆÚ“®‚³‚¹‚È‚¢ŠÔi˜A‘±HIT‚ğ–h‚®‚½‚ßj
	GmPlySeqSetNoJumpMoveTime( player_work, GMD_BOSS2_BODY_DEF_PLAYER_NO_JUMP_MOVE_TIME );
}

// =======================================================================
// gmBoss2BodyRecFuncRegistArmRect
/*!
 *	ƒA[ƒ€‹éŒ`“o˜^
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒg‚Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyRecFuncRegistArmRect(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work;
	amAssert( body_work );

	ObjObjectRectRegist( obj_work, &body_work->rect_work_arm );
}

// =======================================================================
// gmBoss2BodyCatchChangeArmRectNormal
/*!
 *	ƒA[ƒ€‹éŒ`ƒ^ƒCƒv•ÏXi’Êíj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyCatchChangeArmRectNormal( GMS_BOSS2_BODY_WORK* body_work )
{
	//‹éŒ`ƒqƒbƒgŠÖ”•ÏX
	OBS_RECT_WORK* rect_work = &body_work->rect_work_arm;
	rect_work->ppHit = NULL;
	rect_work->ppDef = NULL;
	ObjRectAtkSet( rect_work, 0, 0 );
	ObjRectDefSet( rect_work, 0, 0 );
	rect_work->flag &= ~OBD_RECT_ENABLE;
}

// =======================================================================
// gmBoss2BodyCatchChangeArmRectActive
/*!
 *	ƒA[ƒ€‹éŒ`ƒ^ƒCƒv•ÏXi—LŒøj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyCatchChangeArmRectActive( GMS_BOSS2_BODY_WORK* body_work )
{
	//‹éŒ`ƒqƒbƒgŠÖ”•ÏX
	OBS_RECT_WORK* rect_work = &body_work->rect_work_arm;
	rect_work->ppHit = gmBoss2BodyHitFunc;
	rect_work->ppDef = NULL;
	ObjRectAtkSet( rect_work, GMD_OBJ_RECT_ATK_FLAG_NORMALATK, GMD_OBJ_RECT_ATK_POWER_DEFAULT );
	ObjRectDefSet( rect_work, GMD_OBJ_RECT_DEF_FLAG_NORMALATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT );
	rect_work->flag |= OBD_RECT_ENABLE;
}

// =======================================================================
// gmBoss2BodyCatchChangeArmRectCatch
/*!
 *	ƒA[ƒ€‹éŒ`ƒ^ƒCƒv•ÏXi’Í‚Şj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyCatchChangeArmRectCatch( GMS_BOSS2_BODY_WORK* body_work )
{
	//‹éŒ`ƒqƒbƒgŠÖ”•ÏX
	OBS_RECT_WORK* rect_work = &body_work->rect_work_arm;
	rect_work->ppHit = NULL;
	rect_work->ppDef = gmBoss2BodyCatchHitFuncArmCatch;
	ObjRectAtkSet( rect_work, 0, 0  );
	ObjRectDefSet( rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT );
	rect_work->flag |= OBD_RECT_ENABLE;
}

// =======================================================================
// gmBoss2BodySetRectNormal
/*!
 *	‹éŒ`İ’èi’Êíj
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒg‚Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodySetRectNormal( GMS_BOSS2_BODY_WORK* body_work )
{
	//UŒ‚‹éŒ`”ÍˆÍ‚ğ’Êí‚É
	ObjRectWorkSet(
			&body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK],
			0, 0, 0, 0 );

	//‚­‚ç‚¢‹éŒ`—LŒø
	body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag |= OBD_RECT_ENABLE;

	//ƒA[ƒ€‹éŒ`
	gmBoss2BodyCatchChangeArmRectNormal(body_work);
}

// =======================================================================
// gmBoss2BodySetRectActive
/*!
 *	‹éŒ`İ’èi’Êíj
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒg‚Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodySetRectActive( GMS_BOSS2_BODY_WORK* body_work )
{
	//UŒ‚‹éŒ`”ÍˆÍ‚ğ’Êí‚É
	ObjRectWorkSet(
			&body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK],
			-8, -8, 8, 8 );

	//‚­‚ç‚¢‹éŒ`—LŒø
	body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag |= OBD_RECT_ENABLE;

	//ƒA[ƒ€‹éŒ`
	gmBoss2BodyCatchChangeArmRectActive(body_work);
}

// =======================================================================
// gmBoss2BodySetRectRoll
/*!
 *	‹éŒ`İ’èi‰ñ“]UŒ‚j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒg‚Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodySetRectRoll( GMS_BOSS2_BODY_WORK* body_work )
{
	//UŒ‚‹éŒ`”ÍˆÍ‚ğŠg‘å
	ObjRectWorkSet(
			&body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK],
			-36, -36, 36, 36 );

	//‚­‚ç‚¢‹éŒ`–³Œø
	body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;

	//ƒA[ƒ€‹éŒ`–³Œø
	gmBoss2BodyCatchChangeArmRectNormal(body_work);
}

// =======================================================================
// gmBoss2BodySetRectArm
/*!
 *	ƒA[ƒ€‹éŒ`İ’è
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒg‚Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodySetRectArm( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)body_work;
	amAssert( obj_work );

	OBS_RECT_WORK* rect_work = &body_work->rect_work_arm;

	ObjRectGroupSet( rect_work, GMD_OBJ_RECT_GROUP_ENEMY, GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER );
	gmBoss2BodyRectApplyOffsetArm( body_work );
	rect_work->ppHit = gmBoss2BodyHitFunc;
	rect_work->parent_obj = obj_work;
	rect_work->flag |= OBD_RECT_ENABLE | OBD_RECT_OUT;
	obj_work->ppRec = gmBoss2BodyRecFuncRegistArmRect;

	//’Êíƒ^ƒCƒv
	gmBoss2BodyCatchChangeArmRectNormal( body_work );
}

// =======================================================================
// gmBoss2BodyRectApplyOffsetArm
/*!
 *	ƒA[ƒ€‹éŒ`‚ÉƒIƒtƒZƒbƒg‚ğ”½‰f
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒg‚Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyRectApplyOffsetArm( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_RECT_WORK* rect_work = &body_work->rect_work_arm;

	//ƒtƒ‰ƒO‚ğ•Û‘¶iObjRectWorkSet“à‚Å•ÏX‚³‚ê‚é‚Ì‚Åj
	u32 flag = rect_work->flag;
	ObjRectWorkSet(
			rect_work,
			-40, (s16)(24 - body_work->offset_arm), 40, (s16)(40 - body_work->offset_arm) );
	rect_work->flag = flag;
}

// =======================================================================
// gmBoss2BodySetActionAllParts
/*!
 *	ƒAƒNƒVƒ‡ƒ“İ’èi–{‘Ìƒp[ƒc‘S‚Äj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param action_id	[in] ƒAƒNƒVƒ‡ƒ“ID
 *	@param force_change	[in] ‹­§•ÏXƒtƒ‰ƒO
 */
// =======================================================================
void gmBoss2BodySetActionAllParts(
								  GMS_BOSS2_BODY_WORK* body_work,
								  GME_BOSS2_ACT_ID action_id,
								  BOOL force_change	// = FALSE
								  )
{
	amAssert( body_work );

	//İ’èÏ‚İ
	if ( !force_change && body_work->action_id == action_id ){
		return;
	}

	//ƒAƒNƒVƒ‡ƒ“ID‚ğİ’è
	body_work->action_id = action_id;

	//–{‘Ìƒp[ƒc‚Ìƒ‚[ƒVƒ‡ƒ“‚ğ•ÏX
	for ( s32 i = 0; GMD_BOSS2_PART_IDX_MAX > i; ++i ){

		OBS_OBJECT_WORK* part_obj_work = body_work->parts_objs[i];
		if ( !part_obj_work ){
			continue;
		}

		//ƒAƒNƒVƒ‡ƒ“î•ñ
		const GMS_BOSS2_PART_ACT_INFO* action_info = &gm_boss2_act_info_tbl[action_id][i];

		//ƒGƒbƒOƒ}ƒ“ê—pˆ—
		if ( i == GMD_BOSS2_PART_IDX_EGG ){
			GMS_BOSS2_EGG_WORK* egg_work = (GMS_BOSS2_EGG_WORK*)part_obj_work;

			//ƒGƒbƒOƒ}ƒ“ê—pƒAƒNƒVƒ‡ƒ“’†‚Ìê‡A•ÏX‚µ‚È‚¢
			if ( egg_work->flag & GMD_BOSS2_EGG_FLAG_EGG_ACT_ACTIVE ){
				continue;
			}
		}

		//Œp‘±ƒtƒ‰ƒO—LŒø
		if ( action_info->is_maintain ){
			//ƒ‚[ƒVƒ‡ƒ“‚Í•ÏX‚µ‚È‚¢‚ªAƒŠƒs[ƒgƒtƒ‰ƒO‚Ì‚İ”½‰f‚³‚¹‚é
			if ( action_info->is_repeat ){
				part_obj_work->disp_flag |= OBD_DISP_REPEAT;
			}
		}
		//Œp‘±ƒtƒ‰ƒO–³Œø
		else{
			//ƒ‚[ƒVƒ‡ƒ“‚ğ•ÏX‚·‚é
			GmBsCmnSetAction(
					part_obj_work,
					action_info->mtn_id,
					action_info->is_repeat,
					action_info->is_blend);
		}

		//ƒ‚[ƒVƒ‡ƒ“‘¬“x
		part_obj_work->obj_3d->speed[0] = action_info->mtn_spd;

		//ƒuƒŒƒ“ƒh‘¬“x
		part_obj_work->obj_3d->blend_spd = action_info->blend_spd;
	}
}

// =======================================================================
// gmBoss2BodyOutFunc
/*!
 *	•`‰æŠÖ”
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@note ƒm[ƒh‘€ìˆ—‚ğs‚¤
 */
// =======================================================================
void gmBoss2BodyOutFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work;

	//ƒm[ƒhƒ}ƒgƒŠƒNƒX‘€ìˆ—ŠÇ—ƒ[ƒN‚ğXV
	GmBsCmnUpdateCNMParam( obj_work, &body_work->cnm_mgr_work );

	//•`‰æ
	ObjDrawActionSummary( obj_work );
}

// =======================================================================
// gmBoss2BodyDefFunc
/*!
 *	‚­‚ç‚¢ŠÖ”
 *
 *	@param own_rect	[io] ©g‚Ì‹éŒ`
 *	@param target_rect	[io] ‘Šè‚Ì‹éŒ`
 */
// =======================================================================
void gmBoss2BodyDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect )
{
	OBS_OBJECT_WORK* player_obj_work = target_rect->parent_obj;
	amAssert( player_obj_work );

	OBS_OBJECT_WORK* body_obj_work = own_rect->parent_obj;
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)body_obj_work;
	amAssert( body_work );

	//ƒvƒŒƒCƒ„‚¶‚á‚È‚¢
	if ( !player_obj_work || GMD_OBJTYPE_PLAYER != player_obj_work->obj_type ){
		return;
	}

	//ƒvƒŒƒCƒ„‚ğ’e‚­
	gmBoss2BodyReactionPlayer( player_obj_work, body_obj_work );

	//ƒqƒbƒg–³ŒøŠÔ
	gmBoss2BodySetNoHitTime( body_work, GMD_BOSS2_BODY_DEF_NO_HIT_TIME );

	//ƒA[ƒ€‚ª—LŒø‚Ì‚Æ‚«A‰º‘¤‚©‚ç‚ÌUŒ‚‚Í–³Œø 
	OBS_RECT_WORK* arm_rect_work = &body_work->rect_work_arm;
	if ( arm_rect_work->flag & OBD_RECT_ENABLE ){
		if ( body_obj_work->pos.y < player_obj_work->pos.y ){
			return;
		}
	}

	//ƒ_ƒ[ƒW
	gmBoss2BodyDamage( body_work );
}

// =======================================================================
// gmBoss2BodyHitFunc
/*!
 *	UŒ‚ŠÖ”
 *
 *	@param own_rect	[io] ©g‚Ì‹éŒ`
 *	@param target_rect	[io] ‘Šè‚Ì‹éŒ`
 */
// =======================================================================
void gmBoss2BodyHitFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect )
{
	UNREFERENCED_PARAMETER(target_rect);

	OBS_OBJECT_WORK* body_obj_work = own_rect->parent_obj;
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)body_obj_work;
	amAssert( body_work );

	//HITƒVƒOƒiƒ‹
	body_work->flag |= GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_HIT;
}

// =======================================================================
// gmBoss2BodySearchShutterIn
/*!
 *	üˆÍ‚©‚çƒVƒƒƒbƒ^[ƒMƒ~ƒbƒNi“üŒûj‚ğ’T‚·
 *
 *	@return ƒVƒƒƒbƒ^[ƒMƒ~ƒbƒNi“üŒûj‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
OBS_OBJECT_WORK* gmBoss2BodySearchShutterIn( void )
{
	OBS_OBJECT_WORK* target_obj_work = ObjObjectSearchRegistObject( NULL, GMD_OBJTYPE_GIMMICK );
	while ( target_obj_work ){
		const GMS_ENEMY_3D_WORK* target_gimmick_work = (const GMS_ENEMY_3D_WORK*)target_obj_work;
		if ( target_gimmick_work->ene_com.eve_rec->id == GMD_EVENT_ID_SHUTTER_IN ){
			break;
		}

		//Ÿ‚ÌƒIƒuƒWƒFƒNƒg
		target_obj_work = ObjObjectSearchRegistObject( target_obj_work, GMD_OBJTYPE_GIMMICK );
	}
	return target_obj_work;
}

// =======================================================================
// gmBoss2BodySearchShutterOut
/*!
 *	üˆÍ‚©‚çƒVƒƒƒbƒ^[ƒMƒ~ƒbƒNioŒûj‚ğ’T‚·
 *
 *	@return ƒVƒƒƒbƒ^[ƒMƒ~ƒbƒNioŒûj‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
OBS_OBJECT_WORK* gmBoss2BodySearchShutterOut( void )
{
	OBS_OBJECT_WORK* target_obj_work = ObjObjectSearchRegistObject( NULL, GMD_OBJTYPE_GIMMICK );
	while ( target_obj_work ){
		const GMS_ENEMY_3D_WORK* target_gimmick_work = (const GMS_ENEMY_3D_WORK*)target_obj_work;
		if ( target_gimmick_work->ene_com.eve_rec->id == GMD_EVENT_ID_SHUTTER_OUT ){
			break;
		}

		//Ÿ‚ÌƒIƒuƒWƒFƒNƒg
		target_obj_work = ObjObjectSearchRegistObject( target_obj_work, GMD_OBJTYPE_GIMMICK );
	}
	return target_obj_work;
}


// =======================================================================
// gmBoss2BodySetNoHitTime
/*!
 *	ƒqƒbƒg–³ŒøŠÔ‚ğİ’è
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param time			[in] ƒqƒbƒg–³ŒøŠÔ
 *
 *	@note gmBoss2BodyUpdateNoHitTimeŠÖ”‚ÅXVA‰ğœ‚ğs‚¤
 */
// =======================================================================
void gmBoss2BodySetNoHitTime( GMS_BOSS2_BODY_WORK *body_work, u32 time )
{
	amAssert( body_work );

	GMS_ENEMY_COM_WORK* ene_com = &body_work->ene_3d.ene_com;

	//ƒqƒbƒg–³Œø‚ğİ’è
	body_work->counter_no_hit = time;
	ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag |= OBD_RECT_NOHIT;
}

// =======================================================================
// gmBoss2BodyUpdateNoHitTime
/*!
 *	ƒqƒbƒg–³ŒøŠÔ‚ğXV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyUpdateNoHitTime( GMS_BOSS2_BODY_WORK* body_work )
{
	amAssert( body_work );

	//ƒJƒEƒ“ƒgƒ_ƒEƒ“
	if ( body_work->counter_no_hit > 0 ){
		--body_work->counter_no_hit;
		return;
	}

	//ƒqƒbƒg–³Œøƒtƒ‰ƒO‚ğ‰ğœ
	GMS_ENEMY_COM_WORK* ene_com = &body_work->ene_3d.ene_com;
	ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_NOHIT;
}

// =======================================================================
// gmBoss2BodySetInvincibleTime
/*!
 *	–³“GŠÔ‚ğİ’è
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param time			[in] ƒqƒbƒg–³ŒøŠÔ
 *
 *	@note gmBoss2BodyUpdateNoHitTimeŠÖ”‚ÅXVA‰ğœ‚ğs‚¤
 */
// =======================================================================
void gmBoss2BodySetInvincibleTime( GMS_BOSS2_BODY_WORK *body_work, u32 time )
{
	amAssert( body_work );

	body_work->counter_invincible = time;
	body_work->flag |= GMD_BOSS2_BODY_FLAG_INVINCIBLE;
}

// =======================================================================
// gmBoss2BodyUpdateInvincibleTime
/*!
 *	–³“GŠÔ‚ğXV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyUpdateInvincibleTime( GMS_BOSS2_BODY_WORK* body_work )
{
	amAssert( body_work );

	//ƒJƒEƒ“ƒgƒ_ƒEƒ“
	if ( body_work->counter_invincible > 0 ){
		--body_work->counter_invincible;
		return;
	}
	body_work->flag &= ~GMD_BOSS2_BODY_FLAG_INVINCIBLE;
}

// =======================================================================
// gmBoss2BodySetDirection
/*!
 *	Œü‚«‚ğİ’è
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param deg			[in] Šp“x
 */
// =======================================================================
void gmBoss2BodySetDirection( GMS_BOSS2_BODY_WORK* body_work, Angle16 deg )
{
	body_work->angle_current = deg;
}

// =======================================================================
// gmBoss2BodySetDirectionNormal
/*!
 *	Œü‚«‚ğİ’èi^‰¡‚æ‚è³–ÊŒü‚«j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodySetDirectionNormal( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );

	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		gmBoss2BodySetDirection( body_work, GMD_BOSS2_ANGLE_LEFT );
	}
	else {
		gmBoss2BodySetDirection( body_work, GMD_BOSS2_ANGLE_RIGHT );
	}
}

// =======================================================================
// gmBoss2BodySetDirectionRoll
/*!
 *	Œü‚«‚ğİ’èi^‰¡‚æ‚è³–ÊŒü‚«j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodySetDirectionRoll( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );

	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		gmBoss2BodySetDirection( body_work, GMD_BOSS2_BODY_PINBALL_ANGLE_LEFT_ROLL );
	}
	else {
		gmBoss2BodySetDirection( body_work, GMD_BOSS2_BODY_PINBALL_ANGLE_RIGHT_ROLL );
	}
}

// =======================================================================
// gmBoss2BodyUpdateDirection
/*!
 *	Œü‚«XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyUpdateDirection( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );

	obj_work->dir.y	= (Uint16)body_work->angle_current;
}

// =======================================================================
// gmBoss2BodyCalcMoveXNormalFrame
/*!
 *	À•W‚Ü‚Å‚ÌˆÚ“®‚ÉŠ|‚©‚éƒtƒŒ[ƒ€”‚ğZo
 *
 *	@param body_work		[in] –{‘Ìƒ[ƒN
 *	@param x				[in] –Ú•WÀ•W
 *	@param speed			[in] •½‹ÏƒXƒs[ƒh
 *
 *	@return ƒtƒŒ[ƒ€”
 */
// =======================================================================
Float gmBoss2BodyCalcMoveXNormalFrame( 
									const GMS_BOSS2_BODY_WORK* body_work,
									fx32 x,
									fx32 speed )
{
	//–Ú“I’n‚Ü‚Å‚Ì‹——£
	const OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );
	fx32 dest = MTM_MATH_ABS(x - obj_work->pos.x);

	//ˆÚ“®‚ÉŠ|‚©‚éƒtƒŒ[ƒ€”
	Float frame = (Float)dest / (Float)speed;
	return frame;
}

// =======================================================================
// gmBoss2BodyInitMoveNormal
/*!
 *	À•WˆÚ“®‰Šú‰»
 *
 *	@param body_work		[io] –{‘Ìƒ[ƒN
 *	@param dest_pos			[in] –Ú•WÀ•W
 *	@param frame			[in] ƒtƒŒ[ƒ€”
 */
// =======================================================================
void gmBoss2BodyInitMoveNormal(
						 GMS_BOSS2_BODY_WORK* body_work, 
						 const VecFx32* dest_pos,
						 Float frame )
{
	amAssert( dest_pos );

	const OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	body_work->start_pos.x = obj_work->pos.x;
	body_work->start_pos.y = obj_work->pos.y;
	body_work->start_pos.z = obj_work->pos.z;

	body_work->end_pos.x = dest_pos->x;
	body_work->end_pos.y = dest_pos->y;
	body_work->end_pos.z = body_work->start_pos.z;

	body_work->move_counter = 0.0f;
	if ( frame > 0  ){
		body_work->move_frame = frame;
	}
	else{
		body_work->move_frame = 1.0f;
	}
}

// =======================================================================
// gmBoss2BodyUpdateMoveNormal
/*!
 *	À•WˆÚ“®XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *
 *	@return c‚èƒtƒŒ[ƒ€”
 */
// =======================================================================
Float gmBoss2BodyUpdateMoveNormal( GMS_BOSS2_BODY_WORK* body_work )
{
	amAssert( body_work );

	VecFx32 dest_pos;

	body_work->move_counter += 1.0f;

	//ˆÚ“®I—¹
	if ( body_work->move_counter >= body_work->move_frame ){
		dest_pos.x = body_work->end_pos.x;
		dest_pos.y = body_work->end_pos.y;
		dest_pos.z = body_work->end_pos.z;
	}
	//ƒTƒCƒ“ƒJ[ƒu‚ÅˆÚ“®’†
	else{
		//ƒTƒCƒ“ƒJ[ƒu—pŠp“x‚ğZo
		Float per = body_work->move_counter / body_work->move_frame;
		Angle32 angle_cos = AKM_DEGtoA32( 180 * per );

		//ˆÚ“®—Ê‚ğZo
		Float move_f = 0.5f * (1.0f - nnCos(angle_cos));
		VecFx32 move;
		move.x = (fx32)((body_work->end_pos.x - body_work->start_pos.x) * move_f);
		move.y = (fx32)((body_work->end_pos.y - body_work->start_pos.y) * move_f);
		move.z = (fx32)((body_work->end_pos.z - body_work->start_pos.z) * move_f);
		dest_pos.x = body_work->start_pos.x + move.x;
		dest_pos.y = body_work->start_pos.y + move.y;
		dest_pos.z = body_work->start_pos.z + move.z;
	}

	//À•Wİ’è
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	obj_work->pos.x = dest_pos.x;
	obj_work->pos.y = dest_pos.y;
	obj_work->pos.z = dest_pos.z;
	
	return body_work->move_frame - body_work->move_counter;
}
// =======================================================================
// gmBoss2BodyInitMovePinBall
/*!
 *	ƒsƒ“ƒ{[ƒ‹ˆÚ“®‰Šú‰»
 *
 *	@param body_work		[io] –{‘Ìƒ[ƒN
 *	@param dir_pos			[in] ˆÚ“®•ûŒüÀ•W
 *	@param speed			[in] ˆÚ“®ƒXƒs[ƒh
 */
// =======================================================================
void gmBoss2BodyInitMovePinBall( 
								GMS_BOSS2_BODY_WORK* body_work,
								const VecFx32* dir_pos,
								fx32 speed )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );
	amAssert( dir_pos );

	VecFx32 dir;
	dir.x = dir_pos->x - obj_work->pos.x;
	dir.y = dir_pos->y - obj_work->pos.y;
	dir.z = 0;	//Z‚ÍŒ©‚È‚¢


	

	if ( dir.x > (GMD_BOSS2_BODY_PINBALL_MOVE_DISTANCE_MAX)*FX32_ONE ){
		dir.x = (fx32)(GMD_BOSS2_BODY_PINBALL_MOVE_DISTANCE_MAX)*FX32_ONE;
	}
	else if ( dir.x < -(GMD_BOSS2_BODY_PINBALL_MOVE_DISTANCE_MAX)*FX32_ONE ){
		dir.x = -(fx32)(GMD_BOSS2_BODY_PINBALL_MOVE_DISTANCE_MAX)*FX32_ONE;
	}

	if ( dir.y > (GMD_BOSS2_BODY_PINBALL_MOVE_DISTANCE_MAX)*FX32_ONE ){
		dir.y = (fx32)(GMD_BOSS2_BODY_PINBALL_MOVE_DISTANCE_MAX)*FX32_ONE;
	}
	else if ( dir.y < -(GMD_BOSS2_BODY_PINBALL_MOVE_DISTANCE_MAX)*FX32_ONE ){
		dir.y = -(fx32)(GMD_BOSS2_BODY_PINBALL_MOVE_DISTANCE_MAX)*FX32_ONE;
	}

	fx32 length = FX_Sqrt( FX_Mul( dir.x, dir.x ) + FX_Mul( dir.y, dir.y ) );
	//ƒXƒs[ƒhİ’è
	if ( 0 != length ){
		obj_work->spd.x = FX_Mul( FX_Div( dir.x, length ), speed);
		obj_work->spd.y = FX_Mul( FX_Div( dir.y, length ), speed);
	}

}

// =======================================================================
// gmBoss2BodyUpdateMovePinBall
/*!
 *	ƒsƒ“ƒ{[ƒ‹ˆÚ“®XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *
 */
// =======================================================================
BOOL gmBoss2BodyPinBallCheckFieldUnder( const OBS_OBJECT_WORK* obj_work )
{
#if 0
	if ( obj_work->pos.y + GMD_BOSS2_BODY_FIELD_RECT_SIZE*FX32_ONE > g_gm_main_system.map_fcol.bottom*FX32_ONE ){
		return TRUE;
	}
#else
	if ( obj_work->move_flag & OBD_MOVE_UNDER ){
		return TRUE;
	}
#endif	
	return FALSE;
}
BOOL gmBoss2BodyPinBallCheckFieldOver( const OBS_OBJECT_WORK* obj_work )
{
#if 0
	if ( obj_work->pos.y - GMD_BOSS2_BODY_FIELD_RECT_SIZE*FX32_ONE < g_gm_main_system.map_fcol.top*FX32_ONE ){
		return TRUE;
	}
#else
	if ( obj_work->move_flag & OBD_MOVE_OVER ){
		return TRUE;
	}
#endif	
	return FALSE;
}
BOOL gmBoss2BodyPinBallCheckFieldFront( const OBS_OBJECT_WORK* obj_work )
{
#if 0
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		if ( obj_work->pos.x - GMD_BOSS2_BODY_FIELD_RECT_SIZE*FX32_ONE < g_gm_main_system.map_fcol.left*FX32_ONE ){
			return TRUE;
		}
	}
	else{
		if ( obj_work->pos.x + GMD_BOSS2_BODY_FIELD_RECT_SIZE*FX32_ONE > g_gm_main_system.map_fcol.right*FX32_ONE ){
			return TRUE;
		}
	}
#else
	if ( obj_work->move_flag & OBD_MOVE_FRONT ){
		return TRUE;
	}
#endif	
	return FALSE;
}
BOOL gmBoss2BodyPinBallCheckFieldBack( const OBS_OBJECT_WORK* obj_work )
{
#if 0
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		if ( obj_work->pos.x + GMD_BOSS2_BODY_FIELD_RECT_SIZE*FX32_ONE > g_gm_main_system.map_fcol.right*FX32_ONE ){
			return TRUE;
		}
	}
	else{
		if ( obj_work->pos.x - GMD_BOSS2_BODY_FIELD_RECT_SIZE*FX32_ONE < g_gm_main_system.map_fcol.left*FX32_ONE ){
			return TRUE;
		}
	}
#else
	if ( obj_work->move_flag & OBD_MOVE_BACK ){
		return TRUE;
	}
#endif	
	return FALSE;
}
void gmBoss2BodyUpdateMovePinBall( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	BOOL flag_hit = FALSE;

	fx32 speed_val_x = MTM_MATH_ABS( obj_work->spd.x );
	fx32 speed_val_y = MTM_MATH_ABS( obj_work->spd.y );
	//ã‚É“–‚½‚Á‚½ê‡
	if ( gmBoss2BodyPinBallCheckFieldUnder(obj_work) ){
		obj_work->spd.y = -speed_val_y;
		//‹²‚Ü‚ç‚È‚¢‚æ‚¤‚É•â³‚·‚é
		if (MTM_MATH_ABS(obj_work->spd.x) < 0x00000100L){
			obj_work->spd.x = 0x00000100L;
		}
		obj_work->move_flag |= OBD_MOVE_NOSPDM | OBD_MOVE_JUMP;

		//Œø‰Ê‰¹
		flag_hit = TRUE;
	}
	//‰º
	else if ( gmBoss2BodyPinBallCheckFieldOver(obj_work) ){
		obj_work->spd.y = speed_val_y;
		//‹²‚Ü‚ç‚È‚¢‚æ‚¤‚É•â³‚·‚é
		if (MTM_MATH_ABS(obj_work->spd.x) < 0x00000100L){
			obj_work->spd.x = 0x00000100L;
		}
		obj_work->move_flag |= OBD_MOVE_NOSPDM | OBD_MOVE_JUMP;

		//Œø‰Ê‰¹
		flag_hit = TRUE;
	}
	//‘OŒã‚É“–‚½‚Á‚½ê‡
	if ( gmBoss2BodyPinBallCheckFieldFront(obj_work)
			|| gmBoss2BodyPinBallCheckFieldBack(obj_work)){
		if ( obj_work->spd.x < 0 ){
			obj_work->spd.x = speed_val_x;
		}
		else{
			obj_work->spd.x = -speed_val_x;
		}
		//‹²‚Ü‚ç‚È‚¢‚æ‚¤‚É•â³‚·‚é
		if (MTM_MATH_ABS(obj_work->spd.y) < 0x00000100L){
			obj_work->spd.y = 0x00000100L;
		}
		obj_work->move_flag |= OBD_MOVE_NOSPDM | OBD_MOVE_JUMP;

		//Œø‰Ê‰¹
		flag_hit = TRUE;
	}

	if ( flag_hit ){
		if ( !(body_work->flag & GMD_BOSS2_BODY_FLAG_NO_SE_HIT_WALL) ){
			GmSoundPlaySE( "Boss2_05" );
			body_work->flag |= GMD_BOSS2_BODY_FLAG_NO_SE_HIT_WALL;
		}
	}
	else{
		body_work->flag &= ~GMD_BOSS2_BODY_FLAG_NO_SE_HIT_WALL;
	}
}

// =======================================================================
// gmBoss2BodyInitTurn
/*!
 *	U‚èŒü‚«‰Šú‰»
 *
 *	@param body_work		[io] –{‘Ìƒ[ƒN
 *	@param dest_angle		[in] –Ú•WŠp
 *	@param frame			[in] ƒtƒŒ[ƒ€”
 *	@param flag_positive	[in] ³•ûŒü‰ñ“]ƒtƒ‰ƒO(TRUE:³ FALSE:•‰)
 */
// =======================================================================
void gmBoss2BodyInitTurn(
						 GMS_BOSS2_BODY_WORK* body_work, 
						 Angle16 dest_angle,
						 Float frame, 
						 BOOL flag_positive )
{
	amAssert( frame > 0 );

	body_work->turn_counter = 0.0f;
	body_work->turn_frame = frame;
	body_work->turn_start = body_work->angle_current;

	//³•ûŒü
	u16 amount_u16 = (u16)((Angle32)dest_angle - (Angle32)body_work->angle_current);
	body_work->turn_amount = (Angle32)amount_u16;

	//•‰•ûŒü
	if ( !flag_positive ){
		u16 amount_u16_neg = (u16)(body_work->turn_amount - AKM_DEGtoA32(360));
		body_work->turn_amount = amount_u16_neg - AKM_DEGtoA32(360);
	}
}

// =======================================================================
// gmBoss2BodyUpdateTurn
/*!
 *	U‚èŒü‚«XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *
 *	@return c‚èƒtƒŒ[ƒ€”
 */
// =======================================================================
Float gmBoss2BodyUpdateTurn( GMS_BOSS2_BODY_WORK* body_work )
{

	//İ’è‚³‚ê‚Ä‚¢‚È‚¢
	if ( body_work->turn_frame < 1.0f ){
		return 0;
	}

	Angle16 angle_current;

	body_work->turn_counter += 1.0f;

	//U‚èŒü‚«I—¹
	if ( body_work->turn_counter >= body_work->turn_frame ){
		angle_current = (Angle16)(body_work->turn_start + (Angle16)body_work->turn_amount);
	}
	//ƒTƒCƒ“ƒJ[ƒu‚Å‰ñ“]’†
	else{
		//ƒTƒCƒ“ƒJ[ƒu—pŠp“x‚ğZo
		Float per = body_work->turn_counter / body_work->turn_frame;
		Angle32 angle_cos = AKM_DEGtoA32( 180 * per );

		//U‚èŒü‚«Šp“x‚ğZo
		Float angle_f = body_work->turn_amount * 0.5f * (1.0f - nnCos(angle_cos));
		angle_current = (Angle16)(body_work->turn_start + (Angle16)angle_f);
	}

	//İ’è
	gmBoss2BodySetDirection( body_work, angle_current );
	
	return body_work->turn_frame - body_work->turn_counter;
}

// =======================================================================
// gmBoss2BodyCatchArmCheckTarget
/*!
 *	’Í‚Ş‘ÎÛ‚ª‚¢‚é‚©ƒ`ƒFƒbƒN
 *
 *	@param body_work	[in] –{‘Ìƒ[ƒN
 *
 *	@retval TRUE ’Í‚Ş‘ÎÛ‚ª‚¢‚é
 *	@retval FALSE ’Í‚Ş‘ÎÛ‚ª‚¢‚È‚¢
 */
// =======================================================================
BOOL gmBoss2BodyCatchArmCheckTarget( const GMS_BOSS2_BODY_WORK* body_work )
{
	const OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );
	amAssert( body_obj_work );
	
	const OBS_OBJECT_WORK* player_obj_work = GmBsCmnGetPlayerObj();
	amAssert( player_obj_work );

	//UŒ‚‚µ‚È‚¢
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_NOATTACK ){
		return FALSE;
	}

	//ƒvƒŒƒCƒ„‚ªã‚É‚¢‚é
	if ( player_obj_work->pos.y < body_obj_work->pos.y ){
		return FALSE;
	}

	//‚Â‚©‚ß‚é”ÍˆÍŠO‚É‚¢‚é
	fx32 distance = MTM_MATH_ABS(player_obj_work->pos.x - body_obj_work->pos.x);
	if ( distance > GMD_BOSS2_BODY_CATCH_AREA_WIDTH ){
		return FALSE;
	}

	//return FALSE;
	return TRUE;
}

// =======================================================================
// gmBoss2BodyCatchArmCountReleaseKey
/*!
 *	‰ğ•úƒL[‚Ì“ü—Í‰ñ”‚ğXV
 *
 *	@param body_work	[in] –{‘Ìƒ[ƒN
 *
 *	@retval TRUE ƒL[‚ª‰Ÿ‚³‚ê‚½
 *	@retval FALSE ƒL[‚ª‰Ÿ‚³‚ê‚Ä‚¢‚È‚¢
 */
// =======================================================================
BOOL gmBoss2BodyCatchArmCountReleaseKey( GMS_BOSS2_BODY_WORK* body_work )
{
	const OBS_OBJECT_WORK* player_obj_work = GmBsCmnGetPlayerObj();
	const GMS_PLAYER_WORK* player_work = (const GMS_PLAYER_WORK*)player_obj_work;

	BOOL result = FALSE;

#if GMD_BOSS2_BODY_CATCH_RELEASE_TILT
	// ŒX‚«‚ğg‚Á‚ÄƒŒƒoƒKƒ`ƒƒ‚Á‚Û‚³‚ğÄŒ»‚·‚é
	// ‘O‰ñ‚ª•‰‚ÌŒX‚«A‚©‚ÂA¡‰ñ‚ª³‚ÌŒX‚«
	// ‚Ü‚½‚ÍA‘O‰ñ‚ª³‚ÌŒX‚«A‚©‚ÂA¡‰ñ‚ª•‰‚ÌŒX‚«
	if (
		(body_work->prev_rot_z <= 0 && player_work->key_rot_z > 0x1000)
		|| (body_work->prev_rot_z >= 0 && player_work->key_rot_z < -0x1000)
		) {
		++body_work->count_release_key;
		body_work->prev_rot_z = player_work->key_rot_z; // Ÿ‰ñ—p‚ÉŒX‚«‚ğ•Û‘¶‚µ‚Ä‚¨‚­
		result = TRUE;
	}
#else
    // ƒŒƒoƒKƒ`ƒƒ“ü—Í‚ğƒ`ƒFƒbƒN‚·‚é
    for( s32 i = 0; 16 > i; ++i ){
		if( GMD_BOSS2_BODY_CATCH_RELEASE_KEY & player_work->key_push & (1<<i) ){
			++body_work->count_release_key;
			result = TRUE;
		}
    }
#endif // GMD_BOSS2_BODY_CATCH_RELEASE_TILT

	//ƒL[‚ª‰Ÿ‚³‚ê‚½
	if ( result ){
		return TRUE;
	}

	return FALSE;
}

// =======================================================================
// gmBoss2BodyCatchArmCheckRelease
/*!
 *	‰ğ•ú‚·‚é‚©ƒ`ƒFƒbƒN
 *
 *	@param body_work	[in] –{‘Ìƒ[ƒN
 *
 *	@retval TRUE ‰ğ•ú‚·‚é
 *	@retval FALSE ‰ğ•ú‚µ‚È‚¢
 */
// =======================================================================
BOOL gmBoss2BodyCatchArmCheckRelease( const GMS_BOSS2_BODY_WORK* body_work )
{
	if ( body_work->count_release_key < GMD_BOSS2_BODY_CATCH_RELEASE_KEY_PUSH_NUM ){
		return FALSE;
	}
	return TRUE;
}

// =======================================================================
// gmBoss2BodyCatchArmUpdateShakePlayer
/*!
 *	ƒvƒŒƒCƒ„U“®XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param shake_count	[in] U“®‰ñ”
 */
// =======================================================================
void gmBoss2BodyCatchArmUpdateShakePlayer( GMS_BOSS2_BODY_WORK* body_work, u32 shake_count )
{
	body_work->shake_pos += body_work->shake_speed;

	s32 shake_pos_value = MTM_MATH_ABS(body_work->shake_pos);
	s32 shake_speed_value = MTM_MATH_ABS(body_work->shake_speed);

	//Ü‚è•Ô‚µ
	if ( shake_pos_value > GMD_BOSS2_BODY_CATCH_SHAKE_WIDTH/2 ){
		body_work->shake_speed = -body_work->shake_speed;
	}
	//‰•œ‚Ì”¼•ª
	else if ( shake_pos_value < shake_speed_value ){
		++body_work->shake_count;
	}

	//w’è‰ñ”‰•œ‚µ‚½‚çI—¹
	if ( body_work->shake_count >= shake_count*2 ){	//ƒ[ƒN‚Ìshake_count‚ÍÜ‚è•Ô‚µ–ˆ‚ÉƒJƒEƒ“ƒg‚µ‚Ä‚¢‚é‚Ì‚ÅA2”{‚·‚é
		body_work->shake_speed = 0;
		body_work->shake_pos = 0;
		body_work->shake_count = 0;
		return ;
	}
}

// =======================================================================
// gmBoss2BodyCatchSetArmLength
/*!
 *	ƒA[ƒ€‚ğL‚Î‚·
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param length		[in] L‚Î‚·’·‚³
 */
// =======================================================================
void gmBoss2BodyCatchSetArmLength( 
								  GMS_BOSS2_BODY_WORK* body_work,
								  Float length)
{	
	NNS_MATRIX* w_mtx = GmBsCmnGetSNMMtx(&body_work->snm_work, body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_CATCH] );
	NNS_MATRIX matrix_scale;
	AkMathExtractScaleMtx( &matrix_scale, w_mtx );	

	//e—pƒ}ƒgƒŠƒNƒX
	NNS_MATRIX matrix;
	nnMakeTranslateMatrix( &matrix, 0.0f, length/NNM_MTX(*w_mtx, 1, 1), 0.0f );

	for ( s32 i = 0; GMD_BOSS2_BODY_CNM_INDEX_MAX > i; ++i ){
		s32 index = body_work->cnm_reg_id[i];

		GmBsCmnChangeCNMModeNode( 
				&body_work->cnm_mgr_work,
				index,
				GME_BS_CMN_CNM_MODE_MULT_LEFT );
		GmBsCmnSetCNMMtx(
				&body_work->cnm_mgr_work,
				&matrix,
				index );
		GmBsCmnEnableCNMLocalCoordinate(
				&body_work->cnm_mgr_work,
				index,
				TRUE );
		GmBsCmnEnableCNMMtxNode(
				&body_work->cnm_mgr_work,
				index,
				TRUE );

		//eˆÈ~‚Í’PˆÊs—ñ
		nnMakeUnitMatrix( &matrix );
	}
}

// =======================================================================
// gmBoss2BodyCatchHitFuncArmCatch
/*!
 *	ƒA[ƒ€ƒqƒbƒg‹éŒ`ŠÖ”
 *
 *	@param own_rect		[io] ©g‚Ì‹éŒ`ƒ[ƒN
 *	@param target_rect	[io] ‘Šè‚Ì‹éŒ`ƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyCatchHitFuncArmCatch( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect )
{
	OBS_OBJECT_WORK* body_obj_work = own_rect->parent_obj;
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)body_obj_work;
	amAssert( body_work );
	
	OBS_OBJECT_WORK* player_obj_work = target_rect->parent_obj;
	if ( player_obj_work->obj_type != GMD_OBJTYPE_PLAYER  ){
		return;
	}

	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)player_obj_work;
	amAssert( player_work );

	//ƒqƒbƒg‹éŒ`–³Œø
	own_rect->flag &= ~OBD_RECT_ENABLE;

	//ƒvƒŒƒCƒ„ƒV[ƒPƒ“ƒX•ÏX
	GmPlySeqGmkInitBoss2Catch( player_work );

	//ƒ\ƒjƒbƒNƒLƒƒƒbƒ`’†ƒtƒ‰ƒO
	body_work->flag |= GMD_BOSS2_BODY_FLAG_CATCH;

	//HITƒVƒOƒiƒ‹
	body_work->flag |= GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_HIT;
}

// =======================================================================
// gmBoss2BodyCatchChangeNeedleModeActive
/*!
 *	jƒMƒ~ƒbƒN‚Ìƒ‚[ƒh‚ğƒAƒNƒeƒBƒu‚É
 */
// =======================================================================
void gmBoss2BodyCatchChangeNeedleModeActive( void )
{
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)ObjObjectSearchRegistObject( NULL, GMD_OBJTYPE_GIMMICK );
	while ( obj_work ){
		GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
		if ( gimmick_work->ene_com.eve_rec->id == GMD_EVENT_ID_NEEDLE_NEON_NEEDLE ){
			GmGmkNeedleNeonChangeModeActive( obj_work );
		}
		
		//Ÿ‚ÌƒIƒuƒWƒFƒNƒg
		obj_work = ObjObjectSearchRegistObject( obj_work, GMD_OBJTYPE_GIMMICK );
	}
}

// =======================================================================
// gmBoss2BodyCatchChangeNeedleModeWait
/*!
 *	jƒMƒ~ƒbƒN‚Ìƒ‚[ƒh‚ğ‘Ò‹@‚É
 */
// =======================================================================
void gmBoss2BodyCatchChangeNeedleModeWait( void )
{
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)ObjObjectSearchRegistObject( NULL, GMD_OBJTYPE_GIMMICK );
	while ( obj_work ){
		GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
		if ( gimmick_work->ene_com.eve_rec->id == GMD_EVENT_ID_NEEDLE_NEON_NEEDLE ){
			GmGmkNeedleNeonChangeModeWait( obj_work );
		}
		
		//Ÿ‚ÌƒIƒuƒWƒFƒNƒg
		obj_work = ObjObjectSearchRegistObject( obj_work, GMD_OBJTYPE_GIMMICK );
	}
}

// =======================================================================
// gmBoss2BodyCatchChangeNeedleModeTimer
/*!
 *	jƒMƒ~ƒbƒN‚Ìƒ‚[ƒh‚ğŒÀ®‚É
 */
// =======================================================================
/*void gmBoss2BodyCatchChangeNeedleModeTimer( void )
{
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)ObjObjectSearchRegistObject( NULL, GMD_OBJTYPE_GIMMICK );
	while ( obj_work ){
		GMS_ENEMY_3D_WORK* gimmick_work = (GMS_ENEMY_3D_WORK*)obj_work;
		if ( gimmick_work->ene_com.eve_rec->id == GMD_EVENT_ID_NEEDLE_NEON_NEEDLE ){
			GmGmkNeedleNeonChangeModeTimer( obj_work );
		}
		
		//Ÿ‚ÌƒIƒuƒWƒFƒNƒg
		obj_work = ObjObjectSearchRegistObject( obj_work, GMD_OBJTYPE_GIMMICK );
	}
}*/

// =======================================================================
// gmBoss2BodyBallShootCheckTarget
/*!
 *	“Š‚°‚é‘ÎÛ‚ª‚¢‚é‚©ƒ`ƒFƒbƒN
 *
 *	@param body_work	[in] –{‘Ìƒ[ƒN
 *
 *	@retval TRUE “Š‚°‚é‘ÎÛ‚ª‚¢‚é
 *	@retval FALSE “Š‚°‚é‘ÎÛ‚ª‚¢‚È‚¢
 */
// =======================================================================
BOOL gmBoss2BodyBallShootCheckTarget( const GMS_BOSS2_BODY_WORK* body_work )
{
	const OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );
	amAssert( body_obj_work );
	
	const OBS_OBJECT_WORK* player_obj_work = GmBsCmnGetPlayerObj();
	amAssert( player_obj_work );

	//UŒ‚‚µ‚È‚¢
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_NOATTACK ){
		return FALSE;
	}

	//ƒvƒŒƒCƒ„‚ªã‚É‚¢‚é
	if ( player_obj_work->pos.y < body_obj_work->pos.y ){
		return FALSE;
	}

	//“Š‚°‚é”ÍˆÍŠO‚É‚¢‚é
	fx32 distance = MTM_MATH_ABS(player_obj_work->pos.x - body_obj_work->pos.x);
	if ( distance > GMD_BOSS2_BODY_BALL_AREA_WIDTH ){
		return FALSE;
	}

	return TRUE;
}

// =======================================================================
// gmBoss2BodyPinBallCheckTurn
/*!
 *	ƒ^[ƒ“‚·‚é‚©ƒ`ƒFƒbƒN
 *
 *	@param body_work	[in] –{‘Ìƒ[ƒN
 *
 *	@retval TRUE ƒ^[ƒ“‚·‚é
 *	@retval FALSE ƒ^[ƒ“‚µ‚È‚¢
 */
// =======================================================================
BOOL gmBoss2BodyPinBallCheckTurn( const GMS_BOSS2_BODY_WORK* body_work )
{
	const OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );
	amAssert( body_obj_work );
	
	const OBS_OBJECT_WORK* player_obj_work = GmBsCmnGetPlayerObj();
	amAssert( player_obj_work );

	if ( body_obj_work->disp_flag & OBD_DISP_HFLIP ){
		if ( player_obj_work->pos.x < body_obj_work->pos.x ){
			return FALSE;
		}
	}
	else{
		if ( body_obj_work->pos.x < player_obj_work->pos.x ){
			return FALSE;
		}
	}
	return TRUE;
}

// =======================================================================
// gmBoss2BodyPinBallAdjustMoveSpeed
/*!
 *	ˆÚ“®—Ê’²®
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param speed		[in] ˆÚ“®—Ê
 */
// =======================================================================
void gmBoss2BodyPinBallAdjustMoveSpeed( GMS_BOSS2_BODY_WORK* body_work, fx32 speed )
{
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );
	amAssert( body_obj_work );


	VecFx32 normal;

	//’PˆÊƒxƒNƒgƒ‹‰»
	fx32 length = FX_Mul(body_obj_work->spd.x, body_obj_work->spd.x) + FX_Mul(body_obj_work->spd.y, body_obj_work->spd.y);
	length = FX_Sqrt(length);
	if ( length == 0 ){
		normal.x = FX32_ONE;
		normal.y = 0;
	}
	else{
		normal.x = FX_Div( body_obj_work->spd.x, length );
		normal.y = FX_Div( body_obj_work->spd.y, length );
	}
	normal.z = 0;

	fx32 adjust_speed = (fx32)((speed - length)/10);

	//ˆÚ“®—Ê”½‰f
	body_obj_work->spd.x = FX_Mul(normal.x, length + adjust_speed);
	body_obj_work->spd.y = FX_Mul(normal.y, length + adjust_speed);
	body_obj_work->spd.z = 0;
}

// =======================================================================
// gmBoss2BodyPinBallCheckAreaStop
/*!
 *	’â~•s‰Â”ÍˆÍƒ`ƒFƒbƒN
 *
 *	@param body_work	[in] –{‘Ìƒ[ƒN
 *
 *	@retval TRUE ’â~‰Â”\	
 *	@retval FALSE ’â~•s‰Â
 */
// =======================================================================
BOOL gmBoss2BodyPinBallCheckAreaStop( const GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );
	amAssert( body_obj_work );
	
	s32 center_x = g_gm_main_system.map_fcol.left + (g_gm_main_system.map_fcol.right - g_gm_main_system.map_fcol.left)/2;
	s32 center_y = g_gm_main_system.map_fcol.top + (g_gm_main_system.map_fcol.bottom - g_gm_main_system.map_fcol.top)/2;

	if ( (center_x - GMD_BOSS2_BODY_PINBALL_NO_STOP_AREA_SIZE_X)*FX32_ONE < body_obj_work->pos.x
			&& body_obj_work->pos.x < (center_x + GMD_BOSS2_BODY_PINBALL_NO_STOP_AREA_SIZE_X)*FX32_ONE
			&& (center_y - GMD_BOSS2_BODY_PINBALL_NO_STOP_AREA_SIZE_Y)*FX32_ONE < body_obj_work->pos.y
			&& body_obj_work->pos.y < (center_y + GMD_BOSS2_BODY_PINBALL_NO_STOP_AREA_SIZE_Y)*FX32_ONE
	){
		return FALSE;
	}
	return TRUE;
}

// =======================================================================
// gmBoss2BodyDamage
/*!
 *	ƒ_ƒ[ƒWˆ—
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyDamage( GMS_BOSS2_BODY_WORK* body_work )
{
	amAssert( body_work );
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );

	//–³“G
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_INVINCIBLE ){
		return;
	}

	GMS_BOSS2_MGR_WORK* mgr_work = gmBoss2MgrGetMgrWork(body_obj_work);
	amAssert( mgr_work );

	//‘Ì—Í
	--mgr_work->life;

	//¶‘¶‚µ‚Ä‚¢‚éê‡
	if ( mgr_work->life > 0 ){
		body_work->flag |= GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_DAMAGE;
	}
	//Œ‚”j‚³‚ê‚½ê‡
	else {
		body_work->flag |= GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_DEFEAT;
		
		// ƒ{ƒXŒ‚”jƒ^ƒCƒ~ƒ“ƒO‚ÌƒgƒƒtƒB[Šl“¾ƒ`ƒFƒbƒN
		HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING_DEFEAT_BOSS);
	}

	//Œø‰Ê‰¹iƒ_ƒ[ƒWj
	GmSoundPlaySE( "Boss0_01" );

	//ƒGƒtƒFƒNƒg
	gmBoss2EffDamageInit( body_work );

	//U“®
	GMM_PAD_VIB_SMALL();

	//–³“GŠÔ
	gmBoss2BodySetInvincibleTime( body_work, GMD_BOSS2_BODY_INVINVIBLE_TIME );
}

// =======================================================================
// gmBoss2BodyEscapeCheckScrollUnlock
/*!
 *	“¦–S‚ÌƒXƒNƒ[ƒ‹‰ğœ”»’è
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *
 *	@retval TRUE ƒXƒNƒ[ƒ‹‰ğœ‚·‚é
 *	@retval FALSE ƒXƒNƒ[ƒ‹‰ğœ‚µ‚È‚¢
 */
// =======================================================================
BOOL gmBoss2BodyEscapeCheckScrollUnlock( const GMS_BOSS2_BODY_WORK* body_work )
{
	const OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );
	amAssert( body_obj_work );

	fx32 unlock_line = (g_gm_main_system.map_fcol.top*FX32_ONE) + GMD_BOSS2_BODY_ESCAPE_SCROLL_UNLOCK_DISTANCE;

	if ( body_obj_work->pos.y <= unlock_line ){
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// gmBoss2BodyEscapeCheckScreenOut
/*!
 *	“¦–S‚Ì‰æ–ÊŠO”»’è
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *
 *	@retval TRUE ‰æ–ÊŠO‚Éo‚½
 *	@retval FALSE ‰æ–Ê“à‚É‚¢‚é
 */
// =======================================================================
BOOL gmBoss2BodyEscapeCheckScreenOut( const GMS_BOSS2_BODY_WORK* body_work )
{
	const OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	if ( obj_work->pos.y <= (g_gm_main_system.map_fcol.top - GMD_BOSS2_BODY_ESCAPE_SCREEN_OUT_LENGTH)*FX32_ONE ){
		return TRUE;
	}	
	return FALSE;
}

// =======================================================================
// gmBoss2BodyEscapeAddjustSpeed
/*!
 *	“¦–S‚ÌƒXƒs[ƒh’²®
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyEscapeAddjustSpeed( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	if ( MTM_MATH_ABS(obj_work->spd.x) > GMD_BOSS2_BODY_ESCAPE_SPD_Y_MAX ){
		obj_work->spd.x = GMD_BOSS2_BODY_ESCAPE_SPD_X_MAX;
		obj_work->spd.y = GMD_BOSS2_BODY_ESCAPE_SPD_Y_MAX;
		obj_work->spd_add.x = 0;
		obj_work->spd_add.y = 0;
	}
}

// =======================================================================
// gmBoss2BodyChangeState
/*!
 *	ó‘Ô‘JˆÚ
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param state		[in] ‘JˆÚæ‚Ìó‘Ô
 */
// =======================================================================
void gmBoss2BodyChangeState(
							GMS_BOSS2_BODY_WORK* body_work,
							GME_BOSS2_BODY_STATE state )
{
	//I—¹ˆ—
	GMF_BOSS2_BODY_STATE_FUNC exit_func = gm_boss2_body_state_func_tbl_leave[body_work->state];
	if ( exit_func ){
		exit_func( body_work );
	}

	//ó‘Ô•ÏXİ’è
	body_work->prev_state = body_work->state;
	body_work->state = state;

	//ŠJnˆ—
	GMF_BOSS2_BODY_STATE_FUNC init_func = gm_boss2_body_state_func_tbl_enter[body_work->state];
	if ( init_func ){
		init_func( body_work );
	}
}

// =======================================================================
// gmBoss2BodyStateStartEnter
/*!
 *	ŠJn‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateStartEnter( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‹éŒ`–³Œø
	obj_work->flag |= OBD_OBJECT_NOHIT;
	
	//ƒAƒNƒVƒ‡ƒ“i‹­§İ’èj
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_START, TRUE);

	//ˆÚ“®’l
	GmBsCmnSetObjSpdZero( obj_work );
		
	//Šp“x
	gmBoss2BodySetDirectionNormal( body_work );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateStartUpdateWait;

	//ƒz[ƒ~ƒ“ƒOƒAƒ^ƒbƒN‚Ì‘ÎÛ‚©‚ç‚Í‚¸‚·
	body_work->ene_3d.ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;

}

// =======================================================================
// gmBoss2BodyStateStartLeave
/*!
 *	ŠJnI—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateStartLeave( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‹éŒ`—LŒø
	obj_work->flag &= ~OBD_OBJECT_NOHIT;

	//UŒ‚‚µ‚È‚¢ƒtƒ‰ƒO‚ğ‚¨‚ë‚·
	body_work->flag &= ~GMD_BOSS2_BODY_FLAG_NOATTACK;

	//ƒz[ƒ~ƒ“ƒOƒAƒ^ƒbƒN‚Ì‘ÎÛ‚É‚·‚é
	body_work->ene_3d.ene_com.enemy_flag &= ~GMD_ENEMY_FLAG_NOHOMING;
}

// =======================================================================
// gmBoss2BodyStateStartUpdateWait
/*!
 *	ŠJnXVi‘Ò‹@j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateStartUpdateWait( GMS_BOSS2_BODY_WORK* body_work )
{
	amAssert( body_work );

	//ƒXƒNƒ[ƒ‹ƒƒbƒN‚³‚ê‚é‚Ü‚Å‘Ò‹@
	if ( !gmBoss2CheckScrollLocked() ){
		return;
	}

#if _IPHONE
	// MAP”ÍˆÍ‚ğk‚ß‚é
	GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_ZONE2_BOSS);
#endif // _IPHONE

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateStartUpdateEnd;
}

// =======================================================================
// gmBoss2BodyStateStartUpdateEnd
/*!
 *	ŠJnXViI—¹j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateStartUpdateEnd( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‰æ–ÊŠO
	if ( ObjViewOutCheck(
			obj_work->pos.x, 
			obj_work->pos.y, 
			0, 
			0, 0, 0, 0) 
	){
		return;
	}

	//‘Ò‹@
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_BOSS2_BODY_START_TIME_WAIT_END ){
		return;
	}
	obj_work->user_timer = 0;

	//“üŒûƒVƒƒƒbƒ^[•Â‚¶‚é
	OBS_OBJECT_WORK* obj_work_shutter_in = gmBoss2BodySearchShutterIn();
	if ( obj_work_shutter_in ){
		GmGmkShutterInChangeModeClose(obj_work_shutter_in);
	}

	//ƒLƒƒƒbƒ`ó‘Ô‚Ö•ÏX
	gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_CATCH_MOVE );
}

// =======================================================================
// gmBoss2BodyStateCatchMoveEnter
/*!
 *	ƒLƒƒƒbƒ`‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchMoveEnter( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );
	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_CATCH_MOVE );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateCatchMoveUpdateMove;

	//ƒAƒtƒ^ƒo[ƒi—LŒø‰»
	gmBoss2EffAfterburnerRequestCreate( body_work );

	//–Ú“IÀ•W
	fx32 dest_x;
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		dest_x = (fx32)g_gm_main_system.map_fcol.left * FX32_ONE + GMD_BOSS2_BODY_MOVE_AREA_LIMIT_WIDTH;
	}
	else{
		dest_x = (fx32)g_gm_main_system.map_fcol.right * FX32_ONE - GMD_BOSS2_BODY_MOVE_AREA_LIMIT_WIDTH;
	}
	VecFx32 dest_pos = {
		dest_x,
		obj_work->pos.y,
		obj_work->pos.z
	};

	//ˆÚ“®ƒtƒŒ[ƒ€”
	fx32 speed = GMD_BOSS2_BODY_CATCH_MOVE_SPEED;
	Float frame = gmBoss2BodyCalcMoveXNormalFrame( 
			body_work, 
			dest_pos.x, 
			speed );

	//ˆÚ“®
	gmBoss2BodyInitMoveNormal(
			body_work,
			&dest_pos,
			frame );
}

// =======================================================================
// gmBoss2BodyStateCatchMoveLeave
/*!
 *	ƒLƒƒƒbƒ`I—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchMoveLeave( GMS_BOSS2_BODY_WORK* body_work )
{		
	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss2EffAfterburnerRequestDelete( body_work );
}

// =======================================================================
// gmBoss2BodyStateCatchMoveUpdateMove
/*!
 *	ƒLƒƒƒbƒ`XViˆÚ“®j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchMoveUpdateMove( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//Œü‚«
	gmBoss2BodySetDirectionNormal( body_work );

	//UŒ‚ˆÚs”»’è 	
	GMS_BOSS2_MGR_WORK* mgr_work = gmBoss2MgrGetMgrWork(obj_work);
	amAssert( mgr_work );
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		if ( mgr_work->life <= GMD_BOSS2_MGR_LIFE_RUSH ){
			gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_PRE_BALL );
			return;
		}
	}
	else{
		if ( GMD_BOSS2_MGR_LIFE - mgr_work->life >= GMD_BOSS2_MGR_LIFE_CATCH ){
			gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_PRE_BALL );
			return;
		}
	}

	//ƒLƒƒƒbƒ`UŒ‚”»’è
	if ( gmBoss2BodyCatchArmCheckTarget(body_work) ){
		//ƒLƒƒƒbƒ`’Í‚Şó‘Ô‚Ö•ÏX
		gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_CATCH_ARM );
		return;
	}

	//ˆÚ“®
	Float frame = gmBoss2BodyUpdateMoveNormal( body_work );

	//ƒhƒŠƒtƒg‹——£”»’è
	if ( frame > GMD_BOSS2_BODY_CATCH_FRAME_DRIFT ){
		return;
	}

	Angle16 dest_angle;
	BOOL flag_positive;

	//‰E‚ÉŒü‚©‚¤
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		obj_work->disp_flag &= ~OBD_DISP_HFLIP;
		dest_angle = GMD_BOSS2_ANGLE_RIGHT;
		flag_positive = TRUE;
	}
	//¶‚ÉŒü‚©‚¤
	else {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
		dest_angle = GMD_BOSS2_ANGLE_LEFT;
		flag_positive = FALSE;
	}

	//•ûŒü“]Š·
	gmBoss2BodyInitTurn(
			body_work, 
			dest_angle,
			GMD_BOSS2_BODY_CATCH_FRAME_TURN, 
			flag_positive );

	//UŒ‚‚µ‚È‚¢ƒtƒ‰ƒO‚ğ‚¨‚ë‚·
	body_work->flag &= ~GMD_BOSS2_BODY_FLAG_NOATTACK;

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateCatchMoveUpdateTurn;

	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss2EffAfterburnerRequestDelete( body_work );
}

// =======================================================================
// gmBoss2BodyStateCatchMoveUpdateTurn
/*!
 *	ƒLƒƒƒbƒ`XViƒhƒŠƒtƒgj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchMoveUpdateTurn( GMS_BOSS2_BODY_WORK* body_work )
{
	amAssert( body_work );

	//ˆÚ“®
	gmBoss2BodyUpdateMoveNormal( body_work );

	//Œü‚«•ÏX’†
	Float frame = gmBoss2BodyUpdateTurn(body_work);
	if ( 0 < frame ){
		return;
	}

	//ƒLƒƒƒbƒ`ó‘Ô‚Ö•ÏX
	gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_CATCH_MOVE );
}


// =======================================================================
// gmBoss2BodyStateCatchArmEnter
/*!
 *	ƒLƒƒƒbƒ`’Í‚Ş‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchArmEnter( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );
	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_CATCH_ARM_OPEN );

	//ˆÚ“®’l
	GmBsCmnSetObjSpdZero( obj_work );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateCatchArmUpdateOpen;

	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss2EffAfterburnerRequestDelete( body_work );
}

// =======================================================================
// gmBoss2BodyStateCatchArmLeave
/*!
 *	ƒLƒƒƒbƒ`’Í‚ŞI—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchArmLeave( GMS_BOSS2_BODY_WORK* body_work )
{		
	//ƒA[ƒ€‚ğŒ³‚ÌˆÊ’u‚Éİ’è
	body_work->offset_arm = 0;	
	gmBoss2BodyCatchSetArmLength( body_work, body_work->offset_arm );

	//‰ğ•ú—pƒL[“ü—Í‰ñ”‚ğƒŠƒZƒbƒg
	body_work->count_release_key = 0;

#if GMD_BOSS2_BODY_CATCH_RELEASE_TILT
	//‘O‰ñŠp“x‰Šú‰»
	body_work->prev_rot_z = 0;
#endif // GMD_BOSS2_BODY_CATCH_RELEASE_TILT

	//UŒ‚‚µ‚È‚¢ƒtƒ‰ƒO
	body_work->flag |= GMD_BOSS2_BODY_FLAG_NOATTACK;
}

// =======================================================================
// gmBoss2BodyStateCatchArmUpdateOpen
/*!
 *	ƒLƒƒƒbƒ`’Í‚ŞXViŠJ‚­j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchArmUpdateOpen( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateCatchArmUpdateReady;
	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_CATCH_ARM_READY );
}

// =======================================================================
// gmBoss2BodyStateCatchArmUpdateReady
/*!
 *	ƒLƒƒƒbƒ`’Í‚ŞXVi€”õj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchArmUpdateReady( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‘Ò‹@
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_BOSS2_BODY_CATCH_FRAME_WAIT_DOWN ){
		return;
	}
	obj_work->user_timer = 0;

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateCatchArmUpdateDown;
	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_CATCH_ARM_CLOSE );

	//‹éŒ`ƒqƒbƒgŠÖ”•ÏX
	gmBoss2BodyCatchChangeArmRectCatch( body_work );

	//Œø‰Ê‰¹
	GmSoundPlaySE("Boss2_01", body_work->se_handle);
}

// =======================================================================
// gmBoss2BodyStateCatchArmUpdateDown
/*!
 *	ƒLƒƒƒbƒ`’Í‚ŞXVi~‚ë‚·j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchArmUpdateDown( GMS_BOSS2_BODY_WORK* body_work )
{
	//ƒm[ƒh‚ğˆÚ“®
	body_work->offset_arm -= GMD_BOSS2_BODY_CATCH_ARM_STRETCH_DOWN;	
	gmBoss2BodyCatchSetArmLength( body_work, body_work->offset_arm );

	//ƒA[ƒ€‹éŒ`XV
	gmBoss2BodyRectApplyOffsetArm( body_work );

	//ƒLƒƒƒbƒ`ƒtƒ‰ƒOŠÄ‹
	if ( !(body_work->flag & GMD_BOSS2_BODY_FLAG_CATCH) ){
		//I—¹‘Ò‚¿
		if ( body_work->offset_arm > -GMD_BOSS2_BODY_CATCH_ARM_STRETCH_MAX ){
			return;
		}
	}

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateCatchArmUpdateClose;

	//Œø‰Ê‰¹
	GmSoundStopSE(body_work->se_handle);
	GmSoundPlaySE("Boss2_02");

}

// =======================================================================
// gmBoss2BodyStateCatchArmUpdateClose
/*!
 *	ƒLƒƒƒbƒ`’Í‚ŞXVi•Â‚¶‚éj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchArmUpdateClose( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	OBS_OBJECT_WORK* player_obj_work = GmBsCmnGetPlayerObj();
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)player_obj_work;
	amAssert( player_work );

	//€–S‚Í•ß‚Ü‚¦‚Ä‚¢‚È‚¢‚±‚Æ‚É‚·‚é
	if ( player_work->player_flag & GMD_PLF_DIE ){
		body_work->flag &= ~GMD_BOSS2_BODY_FLAG_CATCH;
	}
	
	//ƒ\ƒjƒbƒN•ß‚Ü‚¦‚Ä‚¢‚é‚Æ‚«
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_CATCH ){
		//İ’uƒm[ƒh‚Ö
		GmBsCmnUpdateObject3DNNStuckWithNode(
				player_obj_work,
				&body_work->snm_work,
				body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_CATCH],
				TRUE );
		player_obj_work->pos.y += -FX_F32_TO_FX32(body_work->offset_arm) + GMD_BOSS2_BODY_CATCH_OFFSET_SONIC;
	}

	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateCatchArmUpdateUp;
	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_CATCH_ARM_UP );

	
	//ƒ\ƒjƒbƒN•ß‚Ü‚¦‚Ä‚¢‚È‚¢‚Æ‚«
	if ( !(body_work->flag & GMD_BOSS2_BODY_FLAG_CATCH) )
	{
		//‹éŒ`ƒqƒbƒgŠÖ”–ß‚·
		gmBoss2BodyCatchChangeArmRectNormal(body_work);
	}
}

// =======================================================================
// gmBoss2BodyStateCatchArmUpdateUp
/*!
 *	ƒLƒƒƒbƒ`’Í‚ŞXViã‚°‚éj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchArmUpdateUp( GMS_BOSS2_BODY_WORK* body_work )
{
	amAssert( body_work );

	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );

	OBS_OBJECT_WORK* player_obj_work = GmBsCmnGetPlayerObj();
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)player_obj_work;
	amAssert( player_work );

	//ƒm[ƒh‚ğˆÚ“®
	body_work->offset_arm += GMD_BOSS2_BODY_CATCH_ARM_STRETCH_UP;	
	gmBoss2BodyCatchSetArmLength( body_work, body_work->offset_arm );

	//ƒA[ƒ€‹éŒ`XV
	gmBoss2BodyRectApplyOffsetArm( body_work );

	//‚ä‚êXV
	gmBoss2BodyCatchArmUpdateShakePlayer( body_work, 1 );
	body_obj_work->dir.x = (u16)AKM_DEGtoA16(body_work->shake_pos);

	//€–S‚Í•ß‚Ü‚¦‚Ä‚¢‚È‚¢‚±‚Æ‚É‚·‚é
	if ( player_work->player_flag & GMD_PLF_DIE ){
		body_work->flag &= ~GMD_BOSS2_BODY_FLAG_CATCH;
	}
	
	//ƒ\ƒjƒbƒN•ß‚Ü‚¦‚Ä‚¢‚é‚Æ‚«
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_CATCH ){		
		//ƒŒƒoƒKƒ`ƒƒ‰ñ”XV
		if ( gmBoss2BodyCatchArmCountReleaseKey(body_work) ){
			if ( body_work->shake_speed == 0 ){
				body_work->shake_speed = GMD_BOSS2_BODY_CATCH_SHAKE_SPEED;
			}
		}
		//İ’uƒm[ƒh‚Ö
		GmBsCmnUpdateObject3DNNStuckWithNode(
				player_obj_work,
				&body_work->snm_work,
				body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_CATCH],
				TRUE );
		player_obj_work->pos.y += -FX_F32_TO_FX32(body_work->offset_arm) + GMD_BOSS2_BODY_CATCH_OFFSET_SONIC;

		//ƒŒƒoƒKƒ`ƒƒƒ`ƒFƒbƒN
		if ( gmBoss2BodyCatchArmCheckRelease(body_work) ){
			//ƒ\ƒjƒbƒN‚ğ•ú‚·
			body_work->flag &= ~GMD_BOSS2_BODY_FLAG_CATCH;

			//ƒvƒŒƒCƒ„ƒV[ƒPƒ“ƒX•ÏX
			GmPlySeqChangeSequence( player_work, GME_PLY_SEQ_STATE_FALL );
			player_work->player_flag |= GMD_PLF_NOJUMPMOVE | GMD_PLF_NOHOMING;
			player_obj_work->move_flag &= ~(OBD_MOVE_NOCOL | OBD_MOVE_NOMOVE);
			player_obj_work->spd.x = 0;
			player_obj_work->spd.y = 0;
			player_obj_work->spd_add.x = 0;
			player_obj_work->spd_add.y = 0;
		}
	}

	//I—¹‘Ò‚¿
	if ( body_work->offset_arm < 0 || body_obj_work->dir.x != 0 ){
		return;
	}

	//ƒm[ƒhˆÊ’u‚ğƒŠƒZƒbƒg
	body_work->offset_arm = 0;
	gmBoss2BodyCatchSetArmLength( body_work, body_work->offset_arm );

	//ƒA[ƒ€‹éŒ`XV
	gmBoss2BodyRectApplyOffsetArm( body_work );

	//ƒ\ƒjƒbƒN‚ğ•ß‚Ü‚¦‚Ä‚¢‚é‚Æ‚«‚Í‰^‚Ôƒ‚[ƒh
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_CATCH ){
		//‰^‚Ôó‘Ô‚Ö•ÏX
		gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_CATCH_CARRY );
	}
	//•ß‚Ü‚¦‚Ä‚¢‚È‚¢‚Æ‚«‚ÍAˆÚ“®ƒ‚[ƒh
	else {
		//ˆ—ŠÖ”•ÏX
		body_work->proc_update = gmBoss2BodyStateCatchArmUpdateEnd;
	}
}

// =======================================================================
// gmBoss2BodyStateCatchArmUpdateEnd
/*!
 *	ƒLƒƒƒbƒ`’Í‚ŞXViI—¹j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchArmUpdateEnd( GMS_BOSS2_BODY_WORK* body_work )
{
	amAssert( body_work );
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );

	//I—¹‘Ò‚¿
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_BOSS2_BODY_CATCH_FRAME_CATCH_END ){
		return;
	}
	obj_work->user_timer = 0;

	//ˆÚ“®ó‘Ô‚Ö•ÏX
	gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_CATCH_MOVE );

	//ƒA[ƒ€‹éŒ`İ’è‚ğ–ß‚·
	gmBoss2BodyCatchChangeArmRectNormal( body_work );
}

// =======================================================================
// gmBoss2BodyStateCatchCarryEnter
/*!
 *	ƒLƒƒƒbƒ`‰^‚Ô‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchCarryEnter( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );
	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_CATCH_ARM_CARRY );

	//–Ú“IÀ•W
	s32 widht = g_gm_main_system.map_fcol.right - g_gm_main_system.map_fcol.left; 
	fx32 dest_x = (fx32)(g_gm_main_system.map_fcol.left + widht/2) * FX32_ONE;
	VecFx32 dest_pos = {
		dest_x,
		obj_work->pos.y,
		obj_work->pos.z
	};

	//ˆÚ“®ƒtƒŒ[ƒ€”
	fx32 speed = GMD_BOSS2_BODY_CATCH_MOVE_SPEED;
	Float frame = gmBoss2BodyCalcMoveXNormalFrame( 
			body_work, 
			dest_pos.x, 
			speed );

	//ˆÚ“®
	gmBoss2BodyInitMoveNormal(
			body_work,
			&dest_pos,
			frame );

	//Œü‚«
	gmBoss2BodySetDirectionNormal( body_work );

	Angle16 dest_angle = 0;
	BOOL flag_positive = FALSE;
	BOOL flag_turn = FALSE;

	//‰E‚É•ûŒü“]Š·
	if ( obj_work->disp_flag & OBD_DISP_HFLIP && dest_x - obj_work->pos.x >= 0 ){
		obj_work->disp_flag &= ~OBD_DISP_HFLIP;
		dest_angle = GMD_BOSS2_ANGLE_RIGHT;
		flag_positive = TRUE;
		flag_turn = TRUE;
	}
	//¶‚É•ûŒü“]Š·
	else if ( !(obj_work->disp_flag & OBD_DISP_HFLIP) && dest_x - obj_work->pos.x < 0 ){
		obj_work->disp_flag |= OBD_DISP_HFLIP;
		dest_angle = GMD_BOSS2_ANGLE_LEFT;
		flag_positive = FALSE;
		flag_turn = TRUE;
	}

	//•ûŒü“]Š·
	if ( flag_turn ){
		gmBoss2BodyInitTurn(
				body_work, 
				dest_angle,
				GMD_BOSS2_BODY_CATCH_FRAME_TURN, 
				flag_positive );
	}

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateCatchCarryUpdateMove;

	//ƒAƒtƒ^ƒo[ƒi—LŒø‰»
	gmBoss2EffAfterburnerRequestCreate( body_work );

	//”z’u‚³‚ê‚Ä‚¢‚éƒlƒIƒ“j‚ğƒAƒNƒeƒBƒu‚É
	gmBoss2BodyCatchChangeNeedleModeActive();
}

// =======================================================================
// gmBoss2BodyStateCatchCarryLeave
/*!
 *	ƒLƒƒƒbƒ`‰^‚ÔI—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchCarryLeave( GMS_BOSS2_BODY_WORK* body_work )
{
	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss2EffAfterburnerRequestDelete( body_work );

	//ƒ\ƒjƒbƒN‚ğ•ú‚·
	body_work->flag &= ~GMD_BOSS2_BODY_FLAG_CATCH;

	//ƒA[ƒ€‹éŒ`İ’è‚ğ–ß‚·
	gmBoss2BodyCatchChangeArmRectNormal( body_work );

	//”z’u‚³‚ê‚Ä‚¢‚éƒlƒIƒ“j‚ğ‘Ò‹@‚É
	gmBoss2BodyCatchChangeNeedleModeWait();
}

// =======================================================================
// gmBoss2BodyStateCatchCarryUpdateMove
/*!
 *	ƒLƒƒƒbƒ`‰^‚ÔXV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchCarryUpdateMove( GMS_BOSS2_BODY_WORK* body_work )
{
	amAssert( body_work );

	OBS_OBJECT_WORK* player_obj_work = GmBsCmnGetPlayerObj();
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)player_obj_work;
	amAssert( player_work );

	//€–S‚Í•ß‚Ü‚¦‚Ä‚¢‚È‚¢‚±‚Æ‚É‚·‚é
	if ( player_work->player_flag & GMD_PLF_DIE ){
		body_work->flag &= ~GMD_BOSS2_BODY_FLAG_CATCH;
	}

	//•ß‚Ü‚¦‚Ä‚¢‚é‚Æ‚«
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_CATCH ){
		//İ’uƒm[ƒh‚Ö
		GmBsCmnUpdateObject3DNNStuckWithNode(
				player_obj_work,
				&body_work->snm_work,
				body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_CATCH],
				TRUE );
		player_obj_work->pos.y += -FX_F32_TO_FX32(body_work->offset_arm) + GMD_BOSS2_BODY_CATCH_OFFSET_SONIC;
	}

	//•ûŒü“]Š·
	Float frame_turn = gmBoss2BodyUpdateTurn(body_work);
	//ˆÚ“®
	Float frame_move = gmBoss2BodyUpdateMoveNormal( body_work );
	if ( 0 < frame_move || 0 < frame_turn ){
		return ;
	}

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateCatchCarryUpdateOpen;

	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss2EffAfterburnerRequestDelete( body_work );

	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_CATCH_ARM_THROW );

}

// =======================================================================
// gmBoss2BodyStateCatchCarryUpdateOpen
/*!
 *	ƒLƒƒƒbƒ`•ú‚·XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchCarryUpdateOpen( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	OBS_OBJECT_WORK* player_obj_work = GmBsCmnGetPlayerObj();
	GMS_PLAYER_WORK* player_work = (GMS_PLAYER_WORK*)player_obj_work;
	amAssert( player_work );

	//ŠJ‚­‚Ì‘Ò‚¿
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_BOSS2_BODY_CATCH_FRAME_CARRY_OPEN ){
		return;
	}
	obj_work->user_timer = 0;

	//€–S‚Í•ß‚Ü‚¦‚Ä‚¢‚È‚¢‚±‚Æ‚É‚·‚é
	if ( player_work->player_flag & GMD_PLF_DIE ){
		body_work->flag &= ~GMD_BOSS2_BODY_FLAG_CATCH;
	}

	//ƒ\ƒjƒbƒN‚ğ•ß‚Ü‚¦‚Ä‚¢‚ê‚Î—£‚·
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_CATCH ){
		body_work->flag &= ~GMD_BOSS2_BODY_FLAG_CATCH;

		//ƒvƒŒƒCƒ„ƒV[ƒPƒ“ƒX•ÏX
		GmPlySeqChangeSequence( player_work, GME_PLY_SEQ_STATE_FALL );
		player_work->player_flag |= GMD_PLF_NOJUMPMOVE | GMD_PLF_NOHOMING;
		player_obj_work->move_flag &= ~(OBD_MOVE_NOCOL | OBD_MOVE_NOMOVE);
		player_obj_work->spd.x = 0;
		player_obj_work->spd.y = 0;
		player_obj_work->spd_add.x = 0;
		player_obj_work->spd_add.y = 0;
	}

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateCatchCarryUpdateEnd;
}

// =======================================================================
// gmBoss2BodyStateCatchCarryUpdateEnd
/*!
 *	ƒLƒƒƒbƒ`Î‚¢XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateCatchCarryUpdateEnd( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//ƒLƒƒƒbƒ`ó‘Ô‚Ö•ÏX
	gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_CATCH_MOVE );
}

// =======================================================================
// gmBoss2BodyStatePreBallEnter
/*!
 *	ƒgƒQƒ{[ƒ‹‘O‰‰o‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePreBallEnter( GMS_BOSS2_BODY_WORK* body_work )
{	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_ANGRY, TRUE );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStatePreBallUpdateAngry;

	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss2EffAfterburnerRequestDelete( body_work );
}

// =======================================================================
// gmBoss2BodyStatePreBallLeave
/*!
 *	ƒgƒQƒ{[ƒ‹‘O‰‰oI—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePreBallLeave( GMS_BOSS2_BODY_WORK* body_work )
{		
	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss2EffAfterburnerRequestDelete( body_work );

	//UŒ‚‚µ‚È‚¢ƒtƒ‰ƒO‚ğ‚¨‚ë‚·
	body_work->flag &= ~GMD_BOSS2_BODY_FLAG_NOATTACK;
}

// =======================================================================
// gmBoss2BodyStatePreBallUpdateAngry
/*!
 *	ƒgƒQƒ{[ƒ‹‘O‰‰oXVi“{‚éj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePreBallUpdateAngry( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}
	body_work->proc_update = gmBoss2BodyStatePreBallUpdateRise;

	//–Ú“IÀ•W
	VecFx32 dest_pos = {
		obj_work->pos.x,
		obj_work->pos.y - GMD_BOSS2_BODY_BALL_POS_Y,
		obj_work->pos.z
	};

	//ˆÚ“®
	gmBoss2BodyInitMoveNormal(
			body_work,
			&dest_pos,
			GMD_BOSS2_BODY_BALL_FRAME_RISE );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStatePreBallUpdateRise;
#if !_IPNONE | 1
	//ƒJƒƒ‰ƒXƒP[ƒ‹
	GmCameraScaleSet(GMD_BOSS2_BODY_BALL_CAMERA_SCALE, (1.0f-GMD_BOSS2_BODY_BALL_CAMERA_SCALE)/GMD_BOSS2_BODY_BALL_FRAME_RISE);
#endif // !_IPHONE | 1
#if !_IPHONE
	GmMapSetDrawMarginMag();
#endif // !_IPHONE
}

// =======================================================================
// gmBoss2BodyStatePreBallUpdateRise
/*!
 *	ƒgƒQƒ{[ƒ‹‘O‰‰oXViã¸j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePreBallUpdateRise( GMS_BOSS2_BODY_WORK* body_work )
{
	amAssert( body_work );

	//ã¸’†
	Float frame = gmBoss2BodyUpdateMoveNormal( body_work );
	if ( 0 < frame ){
		return ;
	}

	//ƒgƒQƒ{[ƒ‹ó‘Ô‚Ö
	gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_BALL_MOVE );
}

// =======================================================================
// gmBoss2BodyStateBallMoveEnter
/*!
 *	ƒgƒQƒ{[ƒ‹ˆÚ“®‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateBallMoveEnter( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );
	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_BALL_MOVE );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateBallMoveUpdateMove;

	//ƒAƒtƒ^ƒo[ƒi—LŒø‰»
	gmBoss2EffAfterburnerRequestCreate( body_work );

	//–Ú“IÀ•W
	fx32 dest_x;
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		dest_x = (fx32)g_gm_main_system.map_fcol.left * FX32_ONE + GMD_BOSS2_BODY_MOVE_AREA_LIMIT_WIDTH;
	}
	else{
		dest_x = (fx32)g_gm_main_system.map_fcol.right * FX32_ONE - GMD_BOSS2_BODY_MOVE_AREA_LIMIT_WIDTH;
	}
	VecFx32 dest_pos = {
		dest_x,
		obj_work->pos.y,
		obj_work->pos.z
	};

	//ˆÚ“®ƒtƒŒ[ƒ€”
	fx32 speed = GMD_BOSS2_BODY_BALL_MOVE_SPEED;
	Float frame = gmBoss2BodyCalcMoveXNormalFrame( 
			body_work, 
			dest_pos.x, 
			speed );

	//ˆÚ“®
	gmBoss2BodyInitMoveNormal(
			body_work,
			&dest_pos,
			frame );
}

// =======================================================================
// gmBoss2BodyStateBallMoveLeave
/*!
 *	ƒgƒQƒ{[ƒ‹ˆÚ“®I—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateBallMoveLeave( GMS_BOSS2_BODY_WORK* body_work )
{		
	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss2EffAfterburnerRequestDelete( body_work );
}

// =======================================================================
// gmBoss2BodyStateBallMoveUpdateMove
/*!
 *	ƒgƒQƒ{[ƒ‹ˆÚ“®XViˆÚ“®j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateBallMoveUpdateMove( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//Œü‚«
	gmBoss2BodySetDirectionNormal( body_work );

	//ƒsƒ“ƒ{[ƒ‹UŒ‚ˆÚs”»’è
	GMS_BOSS2_MGR_WORK* mgr_work = gmBoss2MgrGetMgrWork(obj_work);
	amAssert( mgr_work );
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_PRE_PINBALL );
		return;
	}
	else{
		if ( GMD_BOSS2_MGR_LIFE - mgr_work->life >= GMD_BOSS2_MGR_LIFE_BALL + GMD_BOSS2_MGR_LIFE_CATCH){
			//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
			if ( !GmBsCmnIsActionEnd(obj_work) ){
				return;
			}
			gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_PRE_PINBALL );
			return;
		}
	}

	//ƒgƒQƒ{[ƒ‹UŒ‚”»’è
	if ( gmBoss2BodyBallShootCheckTarget(body_work) ){
		//ƒgƒQƒ{[ƒ‹Ëoó‘Ô‚Ö•ÏX
		gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_BALL_SHOOT );
		return;
	}


	//ˆÚ“®
	Float frame = gmBoss2BodyUpdateMoveNormal( body_work );

	//ƒhƒŠƒtƒg‹——£”»’è
	if ( frame > GMD_BOSS2_BODY_BALL_FRAME_DRIFT ){
		return;
	}

	Angle16 dest_angle;
	BOOL flag_positive;

	//‰E‚ÉŒü‚©‚¤
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		obj_work->disp_flag &= ~OBD_DISP_HFLIP;
		dest_angle = GMD_BOSS2_ANGLE_RIGHT;
		flag_positive = TRUE;
	}
	//¶‚ÉŒü‚©‚¤
	else {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
		dest_angle = GMD_BOSS2_ANGLE_LEFT;
		flag_positive = FALSE;
	}

	//•ûŒü“]Š·
	gmBoss2BodyInitTurn(
			body_work, 
			dest_angle,
			GMD_BOSS2_BODY_BALL_FRAME_TURN, 
			flag_positive );

	//UŒ‚‚µ‚È‚¢ƒtƒ‰ƒO‚ğ‚¨‚ë‚·
	body_work->flag &= ~GMD_BOSS2_BODY_FLAG_NOATTACK;

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateBallMoveUpdateTurn;

	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss2EffAfterburnerRequestDelete( body_work );
}

// =======================================================================
// gmBoss2BodyStateBallMoveUpdateMove
/*!
 *	ƒgƒQƒ{[ƒ‹ˆÚ“®XViƒhƒŠƒtƒgj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateBallMoveUpdateTurn( GMS_BOSS2_BODY_WORK* body_work )
{
	amAssert( body_work );

	//ˆÚ“®
	gmBoss2BodyUpdateMoveNormal( body_work );

	//Œü‚«•ÏX
	Float frame = gmBoss2BodyUpdateTurn(body_work);
	if ( 0 < frame ){
		return;
	}

	//ƒgƒQƒ{[ƒ‹ˆÚ“®ó‘Ô‚Ö•ÏX
	gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_BALL_MOVE );
}

// =======================================================================
// gmBoss2BodyStateBallShootEnter
/*!
 *	ƒgƒQƒ{[ƒ‹Ëo‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateBallShootEnter( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );
	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_BALL_READY );

	//ˆÚ“®‚µ‚È‚¢
	GmBsCmnSetObjSpdZero( obj_work );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateBallShootUpdateWaitCreate;

	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~	
	gmBoss2EffAfterburnerRequestDelete( body_work );
}

// =======================================================================
// gmBoss2BodyStateBallShootLeave
/*!
 *	ƒgƒQƒ{[ƒ‹ËoI—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateBallShootLeave( GMS_BOSS2_BODY_WORK* body_work )
{
	//UŒ‚‚µ‚È‚¢ƒtƒ‰ƒO
	body_work->flag |= GMD_BOSS2_BODY_FLAG_NOATTACK;
}

// =======================================================================
// gmBoss2BodyStateBallShootUpdateWaitCreate
/*!
 *	ƒgƒQƒ{[ƒ‹XVi€”õj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateBallShootUpdateWaitCreate( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒ{[ƒ‹ƒIƒuƒWƒFƒNƒg¶¬
	OBS_OBJECT_WORK* ball_obj_work = GmEventMgrLocalEventBirth(
		GMD_EVENT_ID_BOSS2_BALL,
		obj_work->pos.x, obj_work->pos.y,
		0,
		0,0,0,0,
		0 );
	ball_obj_work->parent_obj = obj_work;

	//ŠÇ—
	GMS_BOSS2_MGR_WORK* mgr_work = gmBoss2MgrGetMgrWork(obj_work);
	gmBoss2MgrAddObject( mgr_work, ball_obj_work );

	//I—¹ˆ—
	mtTaskChangeTcbDestructor( ball_obj_work->tcb, gmBoss2ExitFunc );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateBallShootUpdateCatch;
}

// =======================================================================
// gmBoss2BodyStateBallShootUpdateCatch
/*!
 *	ƒgƒQƒ{[ƒ‹XVi’Í‚Şj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateBallShootUpdateCatch( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//ƒgƒQƒ{[ƒ‹ËoƒVƒOƒiƒ‹	
	body_work->flag	|= GMD_BOSS2_BODY_FLAG_SIGNAL_B2N_SHOOT;

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateBallShootUpdateShoot;
}

// =======================================================================
// gmBoss2BodyStateBallShootUpdateShoot
/*!
 *	ƒgƒQƒ{[ƒ‹XViËoj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateBallShootUpdateShoot( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//ƒgƒQƒ{[ƒ‹ˆÚ“®ó‘Ô‚Ö•ÏX
	gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_BALL_MOVE );
}

// =======================================================================
// gmBoss2BodyStatePrePinBallEnter
/*!
 *	ƒsƒ“ƒ{[ƒ‹‘O‰‰o‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePrePinBallEnter( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );
	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_PINBALL_START, TRUE );

	//–{‘Ì‹éŒ`‚ğ—LŒø‚É
	body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag |= OBD_RECT_ENABLE;

	//ˆÚ“®‚µ‚È‚¢
	GmBsCmnSetObjSpdZero( obj_work );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStatePrePinBallUpdateWaitEffect;

	//”z’u‚³‚ê‚Ä‚¢‚éƒlƒIƒ“j‚ğŒÀ®‚É
	//gmBoss2BodyCatchChangeNeedleModeTimer();

	//BGM•ÏX
	if ( !GmBsCmnIsFinalZoneType(obj_work) ){
		GmSoundChangeAngryBossBGM();
	}
}

// =======================================================================
// gmBoss2BodyStatePrePinBallLeave
/*!
 *	ƒsƒ“ƒ{[ƒ‹‘O‰‰oI—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePrePinBallLeave( GMS_BOSS2_BODY_WORK* body_work )
{
	//UŒ‚‚µ‚È‚¢ƒtƒ‰ƒO‚ğ‚¨‚ë‚·
	body_work->flag &= ~GMD_BOSS2_BODY_FLAG_NOATTACK;
}

// =======================================================================
// gmBoss2BodyStatePrePinBallUpdateWaitEffect
/*!
 *	ƒsƒ“ƒ{[ƒ‹‘O‰‰oXViƒGƒtƒFƒNƒg¶¬‘Ò‚¿j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePrePinBallUpdateWaitEffect( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒGƒtƒFƒNƒg¶¬ƒtƒŒ[ƒ€‘Ò‚¿
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_BOSS2_BODY_PINBALL_FRAME_CREATE_BLITZ ){
		return;
	}
	obj_work->user_timer = 0;

	//ƒGƒtƒFƒNƒg¶¬
	gmBoss2EffBlitzInit( body_work );

	//Œø‰Ê‰¹
	GmSoundPlaySE("FinalBoss11", body_work->se_handle);

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStatePrePinBallUpdateWaitMotion;
}

// =======================================================================
// gmBoss2BodyStatePrePinBallUpdateWaitMotion
/*!
 *	ƒsƒ“ƒ{[ƒ‹‘O‰‰oXViƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePrePinBallUpdateWaitMotion( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );
	
	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//ƒsƒ“ƒ{[ƒ‹ˆÚ“®ó‘Ô‚Ö•ÏX
	gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_PINBALL_MOVE );
}

// =======================================================================
// gmBoss2BodyStatePinBallMoveEnter
/*!
 *	ƒsƒ“ƒ{[ƒ‹ˆÚ“®‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePinBallMoveEnter( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );
	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_PINBALL_SEARCH );

	//’nŒ`•Ó‚è‚ ‚è
	obj_work->move_flag &= ~OBD_MOVE_NOCOLFIELD;

	//–Ú“IÀ•W	
	/*fx32 speed = GMD_BOSS2_BODY_PINBALL_MOVE_SPEED_ROLL;
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		speed = FX_Mul(speed, GMD_BOSS2_BODY_ADJUST_SPEED_FINAL);
	}
	const OBS_OBJECT_WORK* player_obj_work = GmBsCmnGetPlayerObj();
	amAssert( player_obj_work );
	gmBoss2BodyInitMovePinBall( 
			body_work,
			&player_obj_work->pos,
			speed );*/

	//UŒ‚‹éŒ`
	gmBoss2BodySetRectActive( body_work );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStatePinBallMoveUpdateMove;

	//”z’u‚³‚ê‚Ä‚¢‚éƒlƒIƒ“j‚ğƒAƒNƒeƒBƒu‚É
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		gmBoss2BodyCatchChangeNeedleModeActive();
	}
}

// =======================================================================
// gmBoss2BodyStatePinBallMoveLeave
/*!
 *	ƒsƒ“ƒ{[ƒ‹ˆÚ“®I—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePinBallMoveLeave( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//’nŒ`•Ó‚è‚È‚µ
	obj_work->move_flag|= OBD_MOVE_NOCOLFIELD;
}

// =======================================================================
// gmBoss2BodyStatePinBallMoveUpdateMove
/*!
 *	ƒsƒ“ƒ{[ƒ‹ˆÚ“®XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePinBallMoveUpdateMove( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ˆÚ“®—Ê’²®
	fx32 speed = GMD_BOSS2_BODY_PINBALL_MOVE_SPEED_MOVE;
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		speed = FX_Mul(speed, GMD_BOSS2_BODY_ADJUST_SPEED_FINAL);
	}
	gmBoss2BodyPinBallAdjustMoveSpeed( body_work, speed );

	//•Ç‚ ‚½‚è
	gmBoss2BodyUpdateMovePinBall( body_work );

	//ƒ^[ƒ“”»’è
	/*if ( gmBoss2BodyPinBallCheckTurn(body_work) ){
		Angle16 dest_angle;
		BOOL flag_positive;

		//‰E‚ÉŒü‚©‚¤
		if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
			obj_work->disp_flag &= ~OBD_DISP_HFLIP;
			dest_angle = GMD_BOSS2_ANGLE_RIGHT;
			flag_positive = TRUE;
		}
		//¶‚ÉŒü‚©‚¤
		else {
			obj_work->disp_flag |= OBD_DISP_HFLIP;
			dest_angle = GMD_BOSS2_ANGLE_LEFT;
			flag_positive = FALSE;
		}

		//•ûŒü“]Š·
		gmBoss2BodyInitTurn(
				body_work, 
				dest_angle,
				GMD_BOSS2_BODY_PINBALL_FRAME_TURN, 
				flag_positive );
	}
	Float frame = gmBoss2BodyUpdateTurn(body_work);
	
	//ƒ^[ƒ“’†
	if ( 0 < frame ){
		return;
	}*/

	//Œü‚«
	gmBoss2BodySetDirectionNormal( body_work );

	//ƒ_ƒ[ƒW‚ÉˆÚ“®ƒJƒEƒ“ƒ^ƒŠƒZƒbƒg
	/*if ( body_work->flag & GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_DAMAGE ){
		body_work->counter_pinball = 0;
	}*/

	//ˆÚ“®ƒJƒEƒ“ƒ^
	++body_work->counter_pinball;

	


	if ( body_work->counter_pinball == GMD_BOSS2_BODY_PINBALL_FRAME_MOVE/2 ){
		//”z’u‚³‚ê‚Ä‚¢‚éƒlƒIƒ“j‚ğ‘Ò‹@‚É
		if ( GmBsCmnIsFinalZoneType(obj_work) ){
			gmBoss2BodyCatchChangeNeedleModeWait();
		}
	}
	
#if !_IPHONE
	if ( body_work->counter_pinball < GMD_BOSS2_BODY_PINBALL_FRAME_MOVE ){
#else
	if ( body_work->counter_pinball < GMD_BOSS2_BODY_PINBALL_FRAME_MOVE*2 ){
#endif	//_IPHONE
		return;
	}
	body_work->counter_pinball = 0;

	//ƒsƒ“ƒ{[ƒ‹‰ñ“]ó‘Ô‚Ö•ÏX
	gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_PINBALL_ROLL );
}

// =======================================================================
// gmBoss2BodyStatePinBallRollEnter
/*!
 *	ƒsƒ“ƒ{[ƒ‹‰ñ“]‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePinBallRollEnter( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//’nŒ`•Ó‚è‚ ‚è
	obj_work->move_flag &= ~OBD_MOVE_NOCOLFIELD;

	//ˆÚ“®‚µ‚È‚¢
	GmBsCmnSetObjSpdZero( obj_work );
	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_PINBALL_SEARCH );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStatePinBallRollUpdateSearch;
}

// =======================================================================
// gmBoss2BodyStatePinBallRollLeave
/*!
 *	ƒsƒ“ƒ{[ƒ‹‰ñ“]I—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePinBallRollLeave( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//’nŒ`•Ó‚è‚È‚µ
	obj_work->move_flag|= OBD_MOVE_NOCOLFIELD;

	//ƒGƒbƒOƒ}ƒ“•\¦
	body_work->flag &= ~GMD_BOSS2_BODY_FLAG_EGG_NODISP;
}

// =======================================================================
// gmBoss2BodyStatePinBallRollUpdateSearch
/*!
 *	ƒsƒ“ƒ{[ƒ‹‰ñ“]XVi’Tõj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePinBallRollUpdateSearch( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );
	
	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_PINBALL_FIND );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStatePinBallRollUpdateFind;
}

// =======================================================================
// gmBoss2BodyStatePinBallRollUpdateFind
/*!
 *	ƒsƒ“ƒ{[ƒ‹‰ñ“]XVi”­Œ©j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePinBallRollUpdateFind( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );
	
	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//ƒ^[ƒ“”»’è
	if ( gmBoss2BodyPinBallCheckTurn(body_work) ){
		Angle16 dest_angle;
		BOOL flag_positive;

		//‰E‚ÉŒü‚©‚¤
		if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
			obj_work->disp_flag &= ~OBD_DISP_HFLIP;
			dest_angle = GMD_BOSS2_ANGLE_RIGHT;
			flag_positive = TRUE;
		}
		//¶‚ÉŒü‚©‚¤
		else {
			obj_work->disp_flag |= OBD_DISP_HFLIP;
			dest_angle = GMD_BOSS2_ANGLE_LEFT;
			flag_positive = FALSE;
		}

		//•ûŒü“]Š·
		gmBoss2BodyInitTurn(
				body_work, 
				dest_angle,
				GMD_BOSS2_BODY_PINBALL_FRAME_TURN, 
				flag_positive );
	}

	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_PINBALL_MOVE_CLOSE );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStatePinBallRollReady;
}

// =======================================================================
// gmBoss2BodyStatePinBallRollReady
/*!
 *	ƒsƒ“ƒ{[ƒ‹‰ñ“]XVi€”õj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePinBallRollReady( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒ^[ƒ“
	Float frame = gmBoss2BodyUpdateTurn(body_work);

	//‘Ò‚¿
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_BOSS2_BODY_PINBALL_FRAME_READY ){
		return;
	}
	
	//ƒ^[ƒ“’†
	if ( 0 < frame ){
		return;
	}
	obj_work->user_timer = 0;

	//–Ú“IÀ•W	
	fx32 speed = GMD_BOSS2_BODY_PINBALL_MOVE_SPEED_ROLL;
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		speed = FX_Mul(speed, GMD_BOSS2_BODY_ADJUST_SPEED_FINAL);
	}
	const OBS_OBJECT_WORK* player_obj_work = GmBsCmnGetPlayerObj();
	amAssert( player_obj_work );
	gmBoss2BodyInitMovePinBall( 
			body_work,
			&player_obj_work->pos,
			speed );

	//UŒ‚‹éŒ`
	gmBoss2BodySetRectRoll( body_work );

	//ƒAƒNƒVƒ‡ƒ“
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_PINBALL_MOVE_CLOSE );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStatePinBallRollUpdateMove;

#if !_IPHONE | 1

	//ƒ{ƒXƒ‰ƒbƒVƒ…—p
	//if ( GmBsCmnIsFinalZoneType(obj_work) )
	{
		//‰ñ“]ƒGƒtƒFƒNƒg
		gmBoss2EffCreateRollModel( body_work );
	}
#endif //!_IPHONE

}

// =======================================================================
// gmBoss2BodyStatePinBallRollUpdateMove
/*!
 *	ƒsƒ“ƒ{[ƒ‹‰ñ“]XViˆÚ“®j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePinBallRollUpdateMove( GMS_BOSS2_BODY_WORK* body_work )
{
	
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ˆÚ“®—Ê’²®	
	fx32 speed = GMD_BOSS2_BODY_PINBALL_MOVE_SPEED_ROLL;
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		speed = FX_Mul(speed, GMD_BOSS2_BODY_ADJUST_SPEED_FINAL);
	}
	gmBoss2BodyPinBallAdjustMoveSpeed( body_work, speed );

	//•Ç‚ ‚½‚è
	gmBoss2BodyUpdateMovePinBall( body_work );

	//‰ñ“]
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		obj_work->dir.z -= GMD_BOSS2_BODY_PINBALL_ROLL_ROT_Z;
	}
	else {
		obj_work->dir.z += GMD_BOSS2_BODY_PINBALL_ROLL_ROT_Z;
	}
	gmBoss2BodySetDirectionRoll(body_work);

	//ƒGƒbƒOƒ}ƒ“”ñ•\¦
	body_work->flag |= GMD_BOSS2_BODY_FLAG_EGG_NODISP;

	//ƒJƒEƒ“ƒ^
	++obj_work->user_timer;	
	if ( obj_work->user_timer < GMD_BOSS2_BODY_PINBALL_FRAME_ROLL ){
		return;
	}

	//’â~•s‰Â”ÍˆÍ
	if ( !gmBoss2BodyPinBallCheckAreaStop(body_work) ){
		return;
	}

	obj_work->user_timer = 0;	

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStatePinBallRollUpdateStop;
	
#if !_IPHONE | 1
	//ƒ{ƒXƒ‰ƒbƒVƒ…—p
	//if ( GmBsCmnIsFinalZoneType(obj_work) )
	{
		//‰ñ“]ƒGƒtƒFƒNƒgI—¹’Ê’m
		body_work->flag &= ~GMD_BOSS2_BODY_FLAG_ROLL_ACTIVE;

		//ƒƒXƒgƒGƒtƒFƒNƒg¶¬
		gmBoss2EffCreateRollModelLost( body_work );
	}
#endif //!_IPHONE
}

// =======================================================================
// gmBoss2BodyStatePinBallRollUpdateStop
/*!
 *	ƒsƒ“ƒ{[ƒ‹‰ñ“]XVi’â~j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStatePinBallRollUpdateStop( GMS_BOSS2_BODY_WORK* body_work )
{
	
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ˆÚ“®—Ê’²®
	fx32 speed = GMD_BOSS2_BODY_PINBALL_MOVE_SPEED_ROLL;
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		speed = FX_Mul(speed, GMD_BOSS2_BODY_ADJUST_SPEED_FINAL);
	}
	gmBoss2BodyPinBallAdjustMoveSpeed( body_work, speed );

	//•Ç‚ ‚½‚è
	gmBoss2BodyUpdateMovePinBall( body_work );

	//‰ñ“]
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		obj_work->dir.z -= GMD_BOSS2_BODY_PINBALL_ROLL_ROT_Z;
	}
	else {
		obj_work->dir.z += GMD_BOSS2_BODY_PINBALL_ROLL_ROT_Z;
	}
	gmBoss2BodySetDirectionRoll(body_work);

	//ƒJƒEƒ“ƒ^
	++obj_work->user_timer;	
	if ( obj_work->user_timer < GMD_BOSS2_BODY_PINBALL_FRAME_ROLL_STOP ){
		return;
	}

	//’â~•s‰Â”ÍˆÍ
	if ( !gmBoss2BodyPinBallCheckAreaStop(body_work) ){
		return;
	}

	obj_work->dir.z = 0;
	obj_work->user_timer = 0;
	
	obj_work->dir.z = 0;

	//ƒsƒ“ƒ{[ƒ‹‰ñ“]ó‘Ô‚Ö•ÏX
	gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_PINBALL_MOVE );
}

// =======================================================================
// gmBoss2BodyStateDefeatEnter
/*!
 *	Œ‚”j‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateDefeatEnter( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‹éŒ`–³Œø
	obj_work->flag |= OBD_OBJECT_NOHIT;

	//ƒAƒjƒ[ƒVƒ‡ƒ“ƒXƒgƒbƒv
	obj_work->disp_flag |= OBD_DISP_STOP;

	//ƒz[ƒ~ƒ“ƒOƒAƒ^ƒbƒN‚Ì‘ÎÛ‚©‚ç‚Í‚¸‚·
	body_work->ene_3d.ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;

	//’â~
	GmBsCmnSetObjSpdZero( obj_work );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateDefeatUpdateStart;

	//“dŒ‚ƒGƒtƒFƒNƒgI—¹’Ê’m
	body_work->flag &= ~GMD_BOSS2_BODY_FLAG_BLITZ_ACTIVE;

	//Œø‰Ê‰¹’â~
	GmSoundStopSE(body_work->se_handle);

	//ƒ{ƒXƒ‰ƒbƒVƒ…—p
	//if ( GmBsCmnIsFinalZoneType(obj_work) )
	{
		//‰ñ“]ƒGƒtƒFƒNƒgI—¹’Ê’m
		body_work->flag &= ~GMD_BOSS2_BODY_FLAG_ROLL_ACTIVE;
	}

	//”z’u‚³‚ê‚Ä‚¢‚éƒlƒIƒ“j‚ğ‘Ò‹@‚É
	gmBoss2BodyCatchChangeNeedleModeWait();

	//ƒ{ƒXíŸ—˜BGM‚Ö•ÏX
	GmSoundChangeWinBossBGM();
}

// =======================================================================
// gmBoss2BodyStateDefeatLeave
/*!
 *	Œ‚”jI—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateDefeatLeave( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒAƒjƒ[ƒVƒ‡ƒ“ÄŠJ
	obj_work->disp_flag &= ~OBD_DISP_STOP;

	//‹éŒ`—LŒø
	obj_work->flag &= ~OBD_OBJECT_NOHIT;
}

// =======================================================================
// gmBoss2BodyStateDefeatUpdateStart
/*!
 *	Œ‚”jXViŠJnj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateDefeatUpdateStart( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‘Ò‹@
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_BOSS2_BODY_DEFEAT_TIME_WAIT_START ){
		return;
	}
	obj_work->user_timer = 0;

	//¬”š”­ƒGƒtƒFƒNƒg‰Šú‰»
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ(body_work);
	gmBoss2EffBombsInit( 
			&body_work->bomb_work,
			body_obj_work,
			body_obj_work->pos.x,
			body_obj_work->pos.y,
			FX32_ONE * 80,
			FX32_ONE * 80,
			10, 
			30);

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateDefeatUpdateFall;
}

// =======================================================================
// gmBoss2BodyStateDefeatUpdateExplode
/*!
 *	Œ‚”jXVi”š”­j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateDefeatUpdateFall( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‘Ò‹@
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_BOSS2_BODY_DEFEAT_TIME_WAIT_BOMB ){
		//¬”š”­ƒGƒtƒFƒNƒgXV
		gmBoss2EffBombsUpdate( &body_work->bomb_work );
		return;
	}
	obj_work->user_timer = 0;

	//—‰ºİ’è
	obj_work->move_flag |= OBD_MOVE_FALL;


	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateDefeatUpdateExplode;
}

// =======================================================================
// gmBoss2BodyStateDefeatUpdateExplode
/*!
 *	Œ‚”jXVi”š”­j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateDefeatUpdateExplode( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‘Ò‹@
	fx32 fall_pos = g_gm_main_system.map_fcol.bottom*FX32_ONE - GMD_BOSS2_BODY_DEFEAT_FALL_POS_Y;
	if ( obj_work->pos.y < fall_pos ){
		return;
	}

	//—‰ºİ’è‰ğœ
	obj_work->move_flag &= ~OBD_MOVE_FALL;

	//—‰º‘¬“xƒŠƒZƒbƒg
	GmBsCmnSetObjSpdZero( obj_work );

	//ƒp[ƒc”òUƒVƒOƒiƒ‹
	body_work->flag	|= GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_SCATTER;
	
	//Œø‰Ê‰¹i‘å”š”­j
	GmSoundPlaySE( "Boss0_03" );

	// ƒRƒ“ƒgƒ[ƒ‰[U“®
	GMM_PAD_VIB_MID_TIME(120);

	// ‰æ–Êƒtƒ‰ƒbƒVƒ…İ’è
	GmBsCmnInitFlashScreen(
			&body_work->flash_work, 
			GMD_BOSS2_BODY_DEFEAT_FLASH_INTO_TIME,
			GMD_BOSS2_BODY_DEFEAT_FLASH_KEEP_TIME,
			GMD_BOSS2_BODY_DEFEAT_FLASH_RETURN_TIME );

	//‘å”š”­ƒGƒtƒFƒNƒg‰Šú‰»
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );
	amAssert( body_obj_work );
	OBS_OBJECT_WORK* bomb_obj_work = (OBS_OBJECT_WORK*)GmEfctCmnEsCreate(
			body_obj_work, 
			GME_EFCT_CMN_IDX_BOMB_BIG );
	bomb_obj_work->pos.z = body_obj_work->pos.z + GMD_BOSS2_EFFECT_BOMB_OFFSET_Z;
		
	//ƒXƒRƒA‰ÁZ
	GmPlayerAddScoreNoDisp( (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj(), GMD_PLY_SCORE_BOSS );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateDefeatUpdateScatter;
}

// =======================================================================
// gmBoss2BodyStateDefeatUpdateScatter
/*!
 *	Œ‚”jXVi‚Î‚çT‚«j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateDefeatUpdateScatter( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‰æ–Êƒtƒ‰ƒbƒVƒ…XV
	GmBsCmnUpdateFlashScreen( &body_work->flash_work );

	//‘Ò‹@
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_BOSS2_BODY_DEFEAT_TIME_WAIT_SCATTER ){
		return;
	}
	obj_work->user_timer = 0;

	//•‚±‚°ƒeƒNƒXƒ`ƒƒ‚É•ÏX
	gmBoss2ChangeTextureBurnt( obj_work );

	//•‚±‚°ƒVƒOƒiƒ‹
	body_work->flag |= GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_BURNT;

	//ƒAƒtƒ^[ƒo[ƒi[‰ŒƒGƒtƒFƒNƒg‰Šú‰»
	gmBoss2EffAfterburnerSmokeInit( body_work );

	//–{‘Ì‰ŒƒGƒtƒFƒNƒg‰Šú‰»
	gmBoss2EffBodySmokeInit( body_work );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss2BodyStateDefeatUpdateEnd;

	//”òUƒp[ƒc
	gmBoss2EffScatterInit( body_work );
#if !_IPHONE | 1
	//ƒJƒƒ‰ƒXƒP[ƒ‹
	GmCameraScaleSet(1.0f, (1.0f-GMD_BOSS2_BODY_BALL_CAMERA_SCALE)/GMD_BOSS2_BODY_DEFEAT_TIME_WAIT_END);
	GmMapSetDrawMarginNormal();
#endif // !_IPHONE
}

// =======================================================================
// gmBoss2BodyStateDefeatUpdateEnd
/*!
 *	Œ‚”jXViI—¹j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateDefeatUpdateEnd( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‘Ò‹@
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_BOSS2_BODY_DEFEAT_TIME_WAIT_END ){
		return;
	}
	obj_work->user_timer = 0;

	//“¦–Só‘Ô‚Ö•ÏX
	gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_ESCAPE );
}

// =======================================================================
// gmBoss2BodyStateEscapeEnter
/*!
 *	“¦–S‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateEscapeEnter( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ˆÚ“®’l
	obj_work->spd.x	= 0;
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		obj_work->spd_add.x = -GMD_BOSS2_BODY_ESCAPE_SPD_X_ADD;
	}
	else {
		obj_work->spd_add.x = GMD_BOSS2_BODY_ESCAPE_SPD_X_ADD;
	}
	obj_work->spd_add.y = -GMD_BOSS2_BODY_ESCAPE_SPD_Y_ADD;

	//‹éŒ`–³Œø
	obj_work->flag |= OBD_OBJECT_NOHIT;
	obj_work->move_flag |= OBD_MOVE_NOCOLFIELD | OBD_MOVE_NOCOL;

	//Œü‚«
	gmBoss2BodySetDirectionNormal( body_work );

	//ƒAƒNƒVƒ‡ƒ“i‹­§İ’èj
	gmBoss2BodySetActionAllParts( body_work, GMD_BOSS2_ACT_ID_ESCAPE, TRUE );
	
	//“¦–SƒVƒOƒiƒ‹
	body_work->flag |= GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_ESCAPE;

	//ˆ—ŠÖ”•ÏX
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		body_work->proc_update = gmBoss2BodyStateEscapeUpdateFinalZone;
	}
	else{
		body_work->proc_update = gmBoss2BodyStateEscapeUpdateScrollLock;
	}
#if _IPHONE
	// MAP”ÍˆÍ‚ğ–ß‚·
	GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_HORI);
#endif // _IPHONE
}

// =======================================================================
// gmBoss2BodyStateEscapeLeave
/*!
 *	“¦–SI—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateEscapeLeave( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‹éŒ`—LŒø
	obj_work->flag &= ~OBD_OBJECT_NOHIT;
}

// =======================================================================
// gmBoss2BodyStateEscapeUpdateScrollLock
/*!
 *	“¦–SXViƒXƒNƒ[ƒ‹ƒƒbƒNj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateEscapeUpdateScrollLock( GMS_BOSS2_BODY_WORK* body_work )
{
	amAssert( body_work );

	//ƒXƒs[ƒh’²®
	gmBoss2BodyEscapeAddjustSpeed( body_work );
		
	//ƒXƒNƒ[ƒ‹‰ğœ”»’è
	if ( gmBoss2BodyEscapeCheckScrollUnlock(body_work) ){
		//ƒXƒNƒ[ƒ‹ƒƒbƒN‰ğœ
		GmGmkCamScrLimitRelease( GMD_GMK_SCR_LMT_RELEASE_RIGHT );

		//oŒûƒVƒƒƒbƒ^[ŠJ‚¯‚é
		OBS_OBJECT_WORK* obj_work_shutter_out = gmBoss2BodySearchShutterOut();
		if ( obj_work_shutter_out ){
			GmGmkShutterOutChangeModeOpen(obj_work_shutter_out);
		}

		//ˆ—ŠÖ”•ÏX
		body_work->proc_update = gmBoss2BodyStateEscapeUpdateWaitScreenOut;

		//ƒGƒtƒFƒNƒg
		GmEfctBossCmnEsCreate(
				GMM_BS_OBJ(body_work),
				GME_EFCT_BOSS_CMN_IDX_BOSS_PARTS );
	}
}

// =======================================================================
// gmBoss2BodyStateEscapeUpdateWaitScreenOut
/*!
 *	“¦–SXViƒXƒNƒ[ƒ‹ƒAƒ“ƒƒbƒNj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateEscapeUpdateWaitScreenOut( GMS_BOSS2_BODY_WORK* body_work )
{
	amAssert( body_work );

	//ƒXƒs[ƒh’²®
	gmBoss2BodyEscapeAddjustSpeed( body_work );

	//‰æ–ÊŠO”»’è
	if ( gmBoss2BodyEscapeCheckScreenOut(body_work) ){
		OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
		GMS_BOSS2_MGR_WORK* mgr_work = gmBoss2MgrGetMgrWork(obj_work);
		amAssert( mgr_work );
		mgr_work->flag |= GMD_BOSS2_MGR_FLAG_CLEAR_BOSS;
		body_work->proc_update = NULL;
	}
}

// =======================================================================
// gmBoss2BodyStateEscapeUpdateFinalZone
/*!
 *	“¦–SXViƒtƒ@ƒCƒiƒ‹ƒ][ƒ“—pj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyStateEscapeUpdateFinalZone( GMS_BOSS2_BODY_WORK* body_work )
{
	amAssert( body_work );

	//ƒXƒs[ƒh’²®
	gmBoss2BodyEscapeAddjustSpeed( body_work );
		
	//’ÊíƒXƒe[ƒW‚ÌƒXƒNƒ[ƒ‹‰ğœ”»’è‚ğs‚Á‚ÄAƒGƒtƒFƒNƒg‚ğ¶¬
	if ( gmBoss2BodyEscapeCheckScrollUnlock(body_work) ){
		//ƒGƒtƒFƒNƒg
		GmEfctBossCmnEsCreate(
				GMM_BS_OBJ(body_work),
				GME_EFCT_BOSS_CMN_IDX_BOSS_PARTS );

		//ˆ—ŠÖ”•ÏX
		body_work->proc_update = gmBoss2BodyStateEscapeUpdateWaitScreenOut;
	}
}

// =======================================================================
// gmBoss2BodyMainFuncWaitSetup
/*!
 *	ƒƒCƒ“ˆ—i¶¬‘Ò‚¿j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyMainFuncWaitSetup( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work;
	amAssert( body_work );

	//“Ç‚İ‚İ‘Ò‚¿
	GMS_BOSS2_MGR_WORK* mgr_work = gmBoss2MgrGetMgrWork(obj_work);
	amAssert( mgr_work );
	if ( !gmBoss2MgrCheckSetupComplete(mgr_work) ){
		return;
	}

	//-----------------------------------------
	//ƒ{ƒXƒ‚[ƒVƒ‡ƒ“ƒR[ƒ‹ƒoƒbƒNƒVƒXƒeƒ€
	//-----------------------------------------
	//‰Šú‰»
	GmBsCmnInitBossMotionCBSystem(
			obj_work,
			&body_work->bmcb_mgr );

	//-----------------------------------------
	//ƒm[ƒhƒ}ƒgƒŠƒNƒXæ“¾ˆ—
	//-----------------------------------------
	//ƒ[ƒN‚ğ‰Šú‰»
	GmBsCmnCreateSNMWork(
			&body_work->snm_work,
			obj_work->obj_3d->object,
			GMD_BOSS2_BODY_SNM_INDEX_MAX );

	//ƒ‚[ƒVƒ‡ƒ“ƒR[ƒ‹ƒoƒbƒN‚ğÀsƒŠƒXƒg‚É’Ç‰Á
	GmBsCmnAppendBossMotionCallback(
			&body_work->bmcb_mgr,
			&body_work->snm_work.bmcb_link );


	//Ú‘±ƒm[ƒh‚ğ“o˜^
	for ( s32 i = 0; GMD_BOSS2_BODY_SNM_INDEX_MAX > i; ++i ){
		body_work->snm_reg_id[i] = GmBsCmnRegisterSNMNode(
				&body_work->snm_work, 
				g_boss2_node_index_list[i] );	
	}

	//-----------------------------------------
	//ƒm[ƒhƒ}ƒgƒŠƒNƒX‘€ìˆ—ŠÇ—
	//-----------------------------------------
	//ƒ[ƒN‚ğ‰Šú‰»
	GmBsCmnCreateCNMMgrWork(
			&body_work->cnm_mgr_work,
			obj_work->obj_3d->object,
			GMD_BOSS2_BODY_CNM_INDEX_MAX );

	//ƒR[ƒ‹ƒoƒbƒN‚ğ‰Šú‰»
	GmBsCmnInitCNMCb(
			obj_work, 
			&body_work->cnm_mgr_work );

	//ƒm[ƒh‚ğ“o˜^
	for ( s32 i = 0; GMD_BOSS2_BODY_CNM_INDEX_MAX > i; ++i ){
		body_work->cnm_reg_id[i] = GmBsCmnRegisterCNMNode(
				&body_work->cnm_mgr_work,
				g_boss2_node_index_list[GMD_BOSS2_BODY_SNM_INDEX_START_ARM+i]);
	}
			

	//-----------------------------------------
	//ƒƒCƒ“ˆ—
	//-----------------------------------------
	//ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmBoss2BodyMainFunc;

	//ŠJnó‘Ô‚Ö
	gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_START );
}

// =======================================================================
// gmBoss2BodyMainFunc
/*!
 *	ƒƒCƒ“ˆ—i“Ç‚İ‚İ‘Ò‚¿j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2BodyMainFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work;
	amAssert( body_work );

	//ƒqƒbƒg–³ŒøŠÔ‚ğXV
	gmBoss2BodyUpdateNoHitTime( body_work );

	//–³“GŠÔ‚ğXV
	gmBoss2BodyUpdateInvincibleTime( body_work );

	//ó‘ÔXV
	if ( body_work->proc_update ){
		body_work->proc_update( body_work );
	}

	//-----------------------------------------
	//ƒVƒOƒiƒ‹ƒ`ƒFƒbƒN
	//-----------------------------------------
	//ƒAƒtƒ^ƒo[ƒi¶¬
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_AFTERBURNER ){
		gmBoss2EffAfterburnerInit( body_work );
	}

	//Œ‚”j
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_DEFEAT ){
		//ƒ_ƒ[ƒWI—¹’Ê’m
		body_work->flag	&= ~(GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_DEFEAT | GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_DAMAGE);

		//ó‘Ô•ÏX
		gmBoss2BodyChangeState( body_work, GMD_BOSS2_BODY_STATE_DEFEAT );
		return;
	}

	//ƒ_ƒ[ƒW
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_DAMAGE ){
		body_work->flag	&= ~GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_DAMAGE;

		// ƒGƒbƒOƒ}ƒ“’Ê’m
		body_work->flag |= GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_DAMAGE;

		//ƒ_ƒ[ƒW“_–Å‚ğ‰Šú‰»
		GmBsCmnInitObject3DNNDamageFlicker( 
				obj_work, 
				&body_work->flk_work,
				GMD_BOSS2_BODY_DMG_FLICKER_RADIUS );
	}

	//-----------------------------------------
	//ŠeíXV
	//-----------------------------------------
	//ƒ_ƒ[ƒW“_–Å‚ğXV
	GmBsCmnUpdateObject3DNNDamageFlicker( obj_work, &body_work->flk_work );

	//Šp“x”½‰f
	gmBoss2BodyUpdateDirection( body_work );
}


// =======================================================================
//ƒGƒbƒOƒ}ƒ“
// =======================================================================
// =======================================================================
// gmBoss1EggSetActionIndependent
/*!
  ƒGƒbƒOƒ}ƒ“ “Æ—§ƒAƒNƒVƒ‡ƒ“İ’è
  
  @param egg_work	[io]	–{‘Ìƒ[ƒN
  @param act_id		[in]	‘S‘ÌƒAƒNƒVƒ‡ƒ“ƒCƒ“ƒfƒbƒNƒX(GME_BOSS1_ACT_ID_XXX)
  
  @note
  ƒGƒbƒOƒ}ƒ“‚Ì“Æ—§ƒAƒNƒVƒ‡ƒ“‚ğİ’è‚µ‚Ü‚·B
  gmBoss1EggRevertActionIndependent()‚ğŒÄ‚Ô‚Æ‘S‘ÌƒAƒNƒVƒ‡ƒ“‚Ìİ’è‚É–ß‚è‚Ü‚·B
 */
// =======================================================================

// =======================================================================
// gmBoss2EggChangeAction
/*!
 *	ê—pƒAƒNƒVƒ‡ƒ“•ÏX
 *
 *	@param egg_work		[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 *	@param action_id	[in] ƒAƒNƒVƒ‡ƒ“‚h‚c
 *	@param force_change	[in] ‹­§ƒtƒ‰ƒO
 */
// =======================================================================
void gmBoss2EggChangeAction( 
							GMS_BOSS2_EGG_WORK* egg_work,
							GME_BOSS2_EGG_ACT_ID action_id,
							BOOL force_change	// = FALSE
							)
{
	amAssert( action_id < GMD_BOSS2_EGG_ACT_ID_MAX );

	const GMS_BOSS2_PART_ACT_INFO* action_info = &gm_boss2_egg_act_info_tbl[action_id];

	OBS_OBJECT_WORK* obj_work_egg = GMM_BS_OBJ (egg_work );
	amAssert( obj_work_egg );

	//İ’èÏ‚İ
	if ( !force_change && 
			( (egg_work->egg_action_id == action_id) && (egg_work->flag & GMD_BOSS2_EGG_FLAG_EGG_ACT_ACTIVE) ) 
	){
		return;
	}

	//ƒAƒNƒVƒ‡ƒ“IDİ’è
	egg_work->egg_action_id = action_id;

	//İ’è’†ƒtƒ‰ƒO
	egg_work->flag |= GMD_BOSS2_EGG_FLAG_EGG_ACT_ACTIVE;

	//Œp‘±ƒtƒ‰ƒO—LŒø
	if ( action_info->is_maintain ){
		//ƒ‚[ƒVƒ‡ƒ“‚Í•ÏX‚µ‚È‚¢‚ªAƒŠƒs[ƒgƒtƒ‰ƒO‚Ì‚İ”½‰f‚³‚¹‚é
		if ( action_info->is_repeat ){
			obj_work_egg->disp_flag |= OBD_DISP_REPEAT;
		}
	}
	//Œp‘±ƒtƒ‰ƒO–³Œø
	else{
		//ƒ‚[ƒVƒ‡ƒ“‚ğ•ÏX‚·‚é
		GmBsCmnSetAction(
				obj_work_egg,
				action_info->mtn_id,
				action_info->is_repeat,
				action_info->is_blend);
	}

	//ƒ‚[ƒVƒ‡ƒ“‘¬“x
	obj_work_egg->obj_3d->speed[0] = action_info->mtn_spd;
	
	//ƒuƒŒƒ“ƒh‘¬“xİ’è
	obj_work_egg->obj_3d->blend_spd = action_info->blend_spd;
}

// =======================================================================
// gmBoss2EggRevertAction
/*!
 *	ê—pƒAƒNƒVƒ‡ƒ“I—¹
 *
 *	@param egg_work		[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss2EggRevertAction( GMS_BOSS2_EGG_WORK* egg_work )
{
	OBS_OBJECT_WORK* obj_work_egg = GMM_BS_OBJ( egg_work );
	amAssert( obj_work_egg );
	
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work_egg->parent_obj;
	amAssert( body_work );
	OBS_OBJECT_WORK* obj_work_body = GMM_BS_OBJ( body_work );
	amAssert( obj_work_body );

	amAssert( egg_work->flag & GMD_BOSS2_EGG_FLAG_EGG_ACT_ACTIVE );
	
	//ê—pƒAƒNƒVƒ‡ƒ“‰ğœ
	egg_work->flag &= ~GMD_BOSS2_EGG_FLAG_EGG_ACT_ACTIVE;
	
	//ƒAƒNƒVƒ‡ƒ“İ’è
	const GMS_BOSS2_PART_ACT_INFO* action_info = &gm_boss2_act_info_tbl[body_work->action_id][GMD_BOSS2_PART_IDX_EGG];
	GmBsCmnSetAction(
			obj_work_egg,
			action_info->mtn_id,
			action_info->is_repeat,
			TRUE );
	
	//–{‘Ì‚ÌŒo‰ßƒtƒŒ[ƒ€‚É‡‚í‚¹‚é
	obj_work_egg->obj_3d->frame[0] = obj_work_body->obj_3d->frame[0];
}

// =======================================================================
// gmBoss2EggStateIdleInit
/*!
 *	‘Ò‹@‰Šú‰»
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss2EggStateIdleInit( GMS_BOSS2_EGG_WORK* egg_work )
{
	//ˆ—ŠÖ”İ’è
	egg_work->proc_update = gmBoss2EggStateIdleUpdate;
}

// =======================================================================
// gmBoss2EggStateIdleUpdate
/*!
 *	‘Ò‹@XV
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss2EggStateIdleUpdate( GMS_BOSS2_EGG_WORK* egg_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( egg_work );
	amAssert( obj_work );

	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );

	//ƒqƒbƒgƒVƒOƒiƒ‹”»’è
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_HIT ){
		body_work->flag &= ~GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_HIT;

		//Î‚¢ƒV[ƒPƒ“ƒX‰Šú‰»
		gmBoss2EggStateLaughInit( egg_work );
	}
}

// =======================================================================
// gmBoss2EggStateLaughInit
/*!
 *	Î‚¢‰Šú‰»
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss2EggStateLaughInit( GMS_BOSS2_EGG_WORK* egg_work )
{
	OBS_OBJECT_WORK* egg_obj_work = GMM_BS_OBJ( egg_work );
	amAssert( egg_obj_work );
	OBS_OBJECT_WORK* body_obj_work = egg_obj_work->parent_obj;
	amAssert( body_obj_work );

	//ê—pƒAƒNƒVƒ‡ƒ“
	if ( body_obj_work->disp_flag & OBD_DISP_HFLIP ){
		gmBoss2EggChangeAction( egg_work, GMD_BOSS2_EGG_ACT_ID_LAUGH_L );
	}
	else{
		gmBoss2EggChangeAction( egg_work, GMD_BOSS2_EGG_ACT_ID_LAUGH_R );
	}
	
	//ˆ—ŠÖ”İ’è
	egg_work->proc_update = gmBoss2EggStateLaughUpdate;
}

// =======================================================================
// gmBoss2EggStateLaughUpdate
/*!
 *	Î‚¢XV
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss2EggStateLaughUpdate( GMS_BOSS2_EGG_WORK* egg_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( egg_work );
	amAssert( obj_work );

	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//ƒAƒNƒVƒ‡ƒ“‚ğŒ³‚É–ß‚·
	gmBoss2EggRevertAction( egg_work );

	//‘Ò‹@‚Ö
	gmBoss2EggStateIdleInit( egg_work );
}

// =======================================================================
// gmBoss2EggStateDamageInit
/*!
 *	ƒ_ƒ[ƒW‰Šú‰»
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss2EggStateDamageInit( GMS_BOSS2_EGG_WORK* egg_work )
{
	//ê—pƒAƒNƒVƒ‡ƒ“
	gmBoss2EggChangeAction( egg_work, GMD_BOSS2_EGG_ACT_ID_DAMAGE );

	//Š¾ƒGƒtƒFƒNƒg‰Šú‰»
	gmBoss2EffSweatInit( egg_work );

	//ˆ—ŠÖ”•ÏX
	egg_work->proc_update = gmBoss2EggStateDamageUpdate;
}


// =======================================================================
// gmBoss2EggStateDamageUpdate
/*!
 *	ƒ_ƒ[ƒWXV
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss2EggStateDamageUpdate( GMS_BOSS2_EGG_WORK* egg_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( egg_work );
	amAssert( obj_work );

	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//Š¾ƒGƒtƒFƒNƒgI—¹
	egg_work->flag &= ~GMD_BOSS2_EGG_FLAG_SWEAT_ACTIVE;

	//ƒAƒNƒVƒ‡ƒ“‚ğŒ³‚É–ß‚·
	gmBoss2EggRevertAction( egg_work );

	//‘Ò‹@‚Ö
	gmBoss2EggStateIdleInit( egg_work );
}

// =======================================================================
// gmBoss2EggStateEscapeInit
/*!
 *	“¦–S‰Šú‰»
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss2EggStateEscapeInit( GMS_BOSS2_EGG_WORK* egg_work )
{
	//Š¾ƒGƒtƒFƒNƒg‰Šú‰»
	if ( !(egg_work->flag & GMD_BOSS2_EGG_FLAG_SWEAT_ACTIVE) ){
		gmBoss2EffSweatInit( egg_work );
	}
	
	//ˆ—ŠÖ”İ’è
	egg_work->proc_update = gmBoss2EggStateEscapeUpdate;
}

// =======================================================================
// gmBoss2EggStateEscapeUpdate
/*!
 *	“¦–SXV
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss2EggStateEscapeUpdate( GMS_BOSS2_EGG_WORK* egg_work )
{
	UNREFERENCED_PARAMETER( egg_work );
}

// =======================================================================
// gmBoss2EggmanMainFuncWaitSetup
/*!
 *	ƒƒCƒ“ˆ—i¶¬‘Ò‚¿j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EggmanMainFuncWaitSetup( OBS_OBJECT_WORK* obj_work )
{
	amAssert( obj_work );

	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );

	//¶¬‘Ò‚¿
	GMS_BOSS2_MGR_WORK* mgr_work = gmBoss2MgrGetMgrWork(body_obj_work);
	amAssert( mgr_work );
	if ( !gmBoss2MgrCheckSetupComplete(mgr_work) ){
		return;
	}

	//-----------------------------------------
	//ƒƒCƒ“ˆ—
	//-----------------------------------------
	//ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmBoss2EggmanMainFunc;

	//‘Ò‹@ƒV[ƒPƒ“ƒX‚É
	GMS_BOSS2_EGG_WORK* egg_work = (GMS_BOSS2_EGG_WORK*)obj_work;
	amAssert( egg_work );
	gmBoss2EggStateIdleInit( egg_work );
}

// =======================================================================
// gmBoss2EggmanMainFunc
/*!
 *	ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EggmanMainFunc( OBS_OBJECT_WORK* obj_work )
{
	amAssert( obj_work );

	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );
	GMS_BOSS2_EGG_WORK* egg_work = (GMS_BOSS2_EGG_WORK*)obj_work;
	amAssert( egg_work );

	//İ’uƒm[ƒh‚Ö
	GmBsCmnUpdateObject3DNNStuckWithNode(
			obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_BODY],
			TRUE );

	//ó‘ÔXV
	if ( egg_work->proc_update ){
		egg_work->proc_update( egg_work );
	}

	//-----------------------------------------
	//ƒVƒOƒiƒ‹ƒ`ƒFƒbƒN
	//-----------------------------------------
	//“¦–S
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_ESCAPE ){
		body_work->flag &= ~GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_ESCAPE;

		gmBoss2EggStateEscapeInit( egg_work );
	}

	//ƒ_ƒ[ƒW
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_DAMAGE ){
		body_work->flag &= ~GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_DAMAGE;

		gmBoss2EggStateDamageInit( egg_work );
	}

	//•‚±‚°
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_BURNT ){
		body_work->flag &= ~GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_BURNT;
		gmBoss2ChangeTextureBurnt( obj_work );
	}

	//-----------------------------------------
	//ŠeíXV
	//-----------------------------------------
	//•`‰æƒXƒgƒbƒvƒ`ƒFƒbƒN
	const OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );
	if ( body_obj_work->disp_flag & OBD_DISP_STOP ){
		obj_work->disp_flag	|= OBD_DISP_STOP;
	}
	else{
		obj_work->disp_flag	&= ~OBD_DISP_STOP;
	}

	//”ñ•\¦ƒ`ƒFƒbƒN
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_EGG_NODISP ){
		obj_work->disp_flag	|= OBD_DISP_NODISP;
	}
	else{
		obj_work->disp_flag	&= ~OBD_DISP_NODISP;
	}
}

// =======================================================================
//ƒgƒQƒ{[ƒ‹
// =======================================================================

// =======================================================================
// gmBoss2BallHitFunc
/*!
 *	UŒ‚ŠÖ”
 *
 *	@param own_rect	[io] ©g‚Ì‹éŒ`
 *	@param target_rect	[io] ‘Šè‚Ì‹éŒ`
 */
// =======================================================================
void gmBoss2BallHitFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect )
{
	UNREFERENCED_PARAMETER(target_rect);

	OBS_OBJECT_WORK* ball_obj_work = own_rect->parent_obj;
	amAssert( ball_obj_work );
	OBS_OBJECT_WORK* body_obj_work = ball_obj_work->parent_obj;
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)body_obj_work;
	amAssert( body_work );

	//HITƒVƒOƒiƒ‹
	body_work->flag |= GMD_BOSS2_BODY_FLAG_SIGNAL_B2E_HIT;
}


// =======================================================================
// gmBoss2BallMainFuncWaitSetup
/*!
 *	ƒƒCƒ“ˆ—i¶¬‘Ò‚¿j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2BallMainFuncWaitSetup( OBS_OBJECT_WORK* obj_work )
{
	amAssert( obj_work );

	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );

	//¶¬‘Ò‚¿
	GMS_BOSS2_MGR_WORK* mgr_work = gmBoss2MgrGetMgrWork(body_obj_work);
	amAssert( mgr_work );
	if ( !gmBoss2MgrCheckSetupComplete(mgr_work) ){
		return;
	}

	//-----------------------------------------
	//ƒƒCƒ“ˆ—
	//-----------------------------------------
	//ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmBoss2BallMainFunc;

	//‘Ò‹@ƒV[ƒPƒ“ƒX‚É
	GMS_BOSS2_BALL_WORK* ball_work = (GMS_BOSS2_BALL_WORK*)obj_work;
	amAssert( ball_work );
	gmBoss2BallInit( ball_work );
}


// =======================================================================
// gmBoss2BallMainFunc
/*!
 *	ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2BallMainFunc( OBS_OBJECT_WORK* obj_work )
{	
	amAssert( obj_work );

	GMS_BOSS2_BALL_WORK* ball_work = (GMS_BOSS2_BALL_WORK*)obj_work;
	amAssert( ball_work );

	//ó‘ÔXV
	if ( ball_work->proc_update ){
		ball_work->proc_update( ball_work );
	}
}

// =======================================================================
// gmBoss2BallInit
/*!
 *	‰Šú‰»
 *
 *	@param ball_work	[io] ƒgƒQƒ{[ƒ‹ƒ[ƒN
 */
// =======================================================================
void gmBoss2BallInit( GMS_BOSS2_BALL_WORK* ball_work )
{
	//ˆ—ŠÖ”İ’è
	ball_work->proc_update = gmBoss2BallUpdateCatch;
}

// =======================================================================
// gmBoss2BallUpdateCatch
/*!
 *	XVi’Í‚Şj
 *
 *	@param ball_work	[io] ƒgƒQƒ{[ƒ‹ƒ[ƒN
 */
// =======================================================================
void gmBoss2BallUpdateCatch( GMS_BOSS2_BALL_WORK* ball_work )
{
	OBS_OBJECT_WORK* ball_obj_work = GMM_BS_OBJ( ball_work );
	amAssert( ball_obj_work );

	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)ball_obj_work->parent_obj;
	amAssert( body_work );

	//İ’uƒm[ƒh‚Ö
	GmBsCmnUpdateObject3DNNStuckWithNode(
			ball_obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_BALL],
			TRUE );

	//Ëo‘Ò‚¿‚Ö
	ball_work->proc_update = gmBoss2BallUpdateWaitShoot;

}

// =======================================================================
// gmBoss2BallUpdateWaitShoot
/*!
 *	XViËoj
 *
 *	@param ball_work	[io] ƒgƒQƒ{[ƒ‹ƒ[ƒN
 */
// =======================================================================
void gmBoss2BallUpdateWaitShoot( GMS_BOSS2_BALL_WORK* ball_work )
{
	OBS_OBJECT_WORK* ball_obj_work = GMM_BS_OBJ( ball_work );
	amAssert( ball_obj_work );

	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)ball_obj_work->parent_obj;
	amAssert( body_work );

	//İ’uƒm[ƒh‚Ö
	GmBsCmnUpdateObject3DNNStuckWithNode(
			ball_obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_BALL],
			TRUE );

	//ËoƒVƒOƒiƒ‹‘Ò‚¿
	if ( !(body_work->flag & GMD_BOSS2_BODY_FLAG_SIGNAL_B2N_SHOOT) ){
		return;
	}
	body_work->flag &= ~GMD_BOSS2_BODY_FLAG_SIGNAL_B2N_SHOOT;

	//Ëo‚Ö
	ball_work->proc_update = gmBoss2BallUpdateShoot;
	ball_obj_work->move_flag |= OBD_MOVE_FALL;

}


// =======================================================================
// gmBoss2BallUpdateShoot
/*!
 *	XViËoj
 *
 *	@param ball_work	[io] ƒgƒQƒ{[ƒ‹ƒ[ƒN
 */
// =======================================================================
void gmBoss2BallUpdateShoot( GMS_BOSS2_BALL_WORK* ball_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( ball_work );
	amAssert( obj_work );

	//’n–Ê‚É’…‚­‚Ì‘Ò‚¿
	if ( !(obj_work->move_flag & OBD_MOVE_UNDER) ){
		return;
	}

	//”š”­‘Ò‚¿‚Ö
	ball_work->proc_update = gmBoss2BallUpdateWaitBomb;
}

// =======================================================================
// gmBoss2BallUpdateWaitBomb
/*!
 *	XVi”š”­‘Ò‚¿j
 *
 *	@param ball_work	[io] ƒgƒQƒ{[ƒ‹ƒ[ƒN
 */
// =======================================================================
void gmBoss2BallUpdateWaitBomb( GMS_BOSS2_BALL_WORK* ball_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( ball_work );
	amAssert( obj_work );

	//“_–ÅƒJƒEƒ“ƒg‘Ò‚¿
	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_BOSS2_BALL_BOMB_TIME ){
		return;
	}
	obj_work->user_timer = 0;

	//“_–Å‚Ö
	ball_work->proc_update = gmBoss2BallUpdateFlicker;

	//ƒ_ƒ[ƒW“_–Å‚ğ‰Šú‰»
	GmBsCmnInitObject3DNNDamageFlicker( 
			obj_work, 
			&ball_work->flk_work,
			GMD_BOSS2_BODY_BALL_FLICKER_RADIUS );

	//ƒGƒtƒFƒNƒgì¬
	gmBoss2EffBallBombInit( &obj_work->pos, obj_work );
}

// =======================================================================
// gmBoss2BallUpdateFlicker
/*!
 *	XVi“_–Åj
 *
 *	@param ball_work	[io] ƒgƒQƒ{[ƒ‹ƒ[ƒN
 */
// =======================================================================
void gmBoss2BallUpdateFlicker( GMS_BOSS2_BALL_WORK* ball_work )
{
	OBS_OBJECT_WORK* ball_obj_work = GMM_BS_OBJ( ball_work );
	amAssert( ball_obj_work );
	OBS_OBJECT_WORK* body_obj_work = ball_obj_work->parent_obj;
	amAssert( body_obj_work );


	//ƒ_ƒ[ƒW“_–Å‚ğXV
	BOOL end_flag = GmBsCmnUpdateObject3DNNDamageFlicker( ball_obj_work, &ball_work->flk_work );

	//“_–Å‘Ò‚¿
	if ( !end_flag ){
		return;
	}

	//”j•Ğ¶¬
	gmBoss2EffBallBombPartInit( &ball_obj_work->pos, body_obj_work, GMD_BOSS2_BALL_PART_SPEED_X );
	gmBoss2EffBallBombPartInit( &ball_obj_work->pos, body_obj_work, -GMD_BOSS2_BALL_PART_SPEED_X );

	//‰ŒƒGƒtƒFƒNƒg
	GMS_EFFECT_3DES_WORK* effect_work = GmEfctCmnEsCreate(NULL, GME_EFCT_CMN_IDX_BOMB_SMOKE);
	OBS_OBJECT_WORK* effect_obj_work = (OBS_OBJECT_WORK*)effect_work;
	effect_obj_work->pos = ball_obj_work->pos;

	ball_obj_work->flag |= OBD_OBJECT_TASKCLEAR;

	//Œø‰Ê‰¹
	GmSoundPlaySE("Boss2_03");
}


// ==========================================================================
//ƒGƒtƒFƒNƒg
// ==========================================================================

// =======================================================================
// gmBoss2EffDamageInit
/*!
 *	ƒ_ƒ[ƒWƒGƒtƒFƒNƒg‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2EffDamageInit( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* parent_obj	= GMM_BS_OBJ( body_work );
	amAssert( parent_obj );

	GMS_EFFECT_3DES_WORK* efct_work = GmEfctBossCmnEsCreate(
			parent_obj, 
			GME_EFCT_BOSS_CMN_IDX_BOSS_DM );
	OBS_OBJECT_WORK* effect_obj_work = GMM_BS_OBJ( efct_work );
	amAssert( effect_obj_work );

	effect_obj_work->pos.z += GMD_BOSS2_EFFECT_BOMB_OFFSET_Z;
}

// =======================================================================
// gmBoss2EffBombInitCreate
/*!
 *	”š”­ƒGƒtƒFƒNƒgŒQ‰Šú‰»
 *
 *	@param bomb_work	[io] ”š”­ƒ[ƒN
 *	@param parent_obj	[io] eƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param pos_x		[in] ¶¬”ÍˆÍ’†SÀ•WX
 *	@param pos_y		[in] ¶¬”ÍˆÍ’†SÀ•WY
 *	@param width		[in] ¶¬”ÍˆÍ•
 *	@param height		[in] ¶¬”ÍˆÍ‚‚³
 *	@param interval_min	[in] ¶¬ŠÔŠuÅ’ZŠÔ
 *	@param interval_max	[in] ¶¬ŠÔŠuÅ’·ŠÔ
 */
// =======================================================================
void gmBoss2EffBombsInit( 
						 GMS_BOSS2_EFF_BOMB_WORK* bomb_work,
						 OBS_OBJECT_WORK* parent_obj,
						 fx32 pos_x,
						 fx32 pos_y,
						 fx32 width,
						 fx32 height,
						 Uint32 interval_min,
						 Uint32 interval_max )
{
	amAssert( bomb_work );
	amAssert( parent_obj );

	bomb_work->parent_obj = parent_obj;
	bomb_work->interval_timer = 0;
	bomb_work->interval_min = interval_min;
	bomb_work->interval_max = interval_max;
	bomb_work->pos[MTD_X] = pos_x;
	bomb_work->pos[MTD_Y] = pos_y;
	bomb_work->area[MTD_WIDTH] = width;
	bomb_work->area[MTD_HEIGHT] = height;
}

// =======================================================================
// gmBoss2EffBombsUpdate
/*!
 *	”š”­ƒGƒtƒFƒNƒgŒQXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffBombsUpdate( GMS_BOSS2_EFF_BOMB_WORK* bomb_work )
{
	amAssert( bomb_work );

	//¶¬ŠÔŠuƒ`ƒFƒbƒN
	if ( bomb_work->interval_timer > 0 ){	
		--bomb_work->interval_timer;
		return;
	}

	//Œø‰Ê‰¹
	GmSoundPlaySE( "Boss0_02" );

	//¶¬
	GMS_EFFECT_3DES_WORK* effect_work = GmEfctCmnEsCreate( 
		NULL, 
		GME_EFCT_CMN_IDX_BOMB );
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( effect_work );
	amAssert( obj_work );

	//¶¬À•W
	OBS_OBJECT_WORK* parent_obj_work = GMM_BS_OBJ( bomb_work->parent_obj );

	fx32 width = bomb_work->area[MTD_WIDTH];
	fx32 height = bomb_work->area[MTD_HEIGHT];
	fx32 rand_x = FX_Mul( AkMathRandFx(), width );
	fx32 rand_y = FX_Mul( AkMathRandFx(), height );
	obj_work->pos.x	= bomb_work->pos[MTD_X] - (width >> 1) + rand_x;
	obj_work->pos.y	= bomb_work->pos[MTD_Y] - (height >> 1) + rand_y;
	obj_work->pos.z	= parent_obj_work->pos.z + GMD_BOSS2_EFFECT_BOMB_OFFSET_Z;	//eÀ•W‚©‚çƒIƒtƒZƒbƒg


	//¶¬ŠÔŠu
	fx32 interval = (fx32)(bomb_work->interval_max - bomb_work->interval_min);
	fx32 rand_interval = AkMathRandFx();
	Uint32 rand_ofst = (Uint32)((rand_interval*interval) >> FX32_SHIFT);
	bomb_work->interval_timer = bomb_work->interval_min + rand_ofst;
}

// =======================================================================
// gmBoss2EffAfterburnerRequestCreate
/*!
 *	ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒgì¬—v‹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2EffAfterburnerRequestCreate( GMS_BOSS2_BODY_WORK* body_work )
{
	body_work->flag |= GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_AFTERBURNER;
}

// =======================================================================
// gmBoss2EffAfterburnerRequestDelete
/*!
 *	ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒgíœ—v‹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2EffAfterburnerRequestDelete( GMS_BOSS2_BODY_WORK* body_work )
{
	body_work->flag &= ~GMD_BOSS2_BODY_FLAG_AFTERBURNER_ACTIVE;
	body_work->flag &= ~GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_AFTERBURNER;
}

// =======================================================================
// gmBoss2EffAfterburnerInit
/*!
 *	ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg‚ğ‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2EffAfterburnerInit( GMS_BOSS2_BODY_WORK* body_work )
{
	//¶¬Ï‚İ
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_AFTERBURNER_ACTIVE ){
		return;
	}

	//ƒtƒ‰ƒOİ’è
	body_work->flag &= ~GMD_BOSS2_BODY_FLAG_SIGNAL_B2B_AFTERBURNER;
	body_work->flag |= GMD_BOSS2_BODY_FLAG_AFTERBURNER_ACTIVE;

	//ƒGƒtƒFƒNƒg¶¬
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ(body_work);
	amAssert( body_obj_work );
	GMS_EFFECT_3DES_WORK* efct_work = GmEfctBossCmnEsCreate(
		body_obj_work, 
		GME_EFCT_BOSS_CMN_IDX_JET_B );

	//•\¦ƒIƒtƒZƒbƒg’²®
	GmEffect3DESAddDispOffset( 
		efct_work, 
		0, 
		0, 
		GMD_BOSS2_EFFECT_AFTERBURNER_DISP_OFFSET_Z );

	//ƒƒCƒ“ˆ—
	OBS_OBJECT_WORK* effct_obj_work = GMM_BS_OBJ( efct_work );
	amAssert( effct_obj_work );
	effct_obj_work->ppFunc = gmBoss2EffAfterburnerMainFunc;
}

// =======================================================================
// gmBoss2EffAfterburnerInit
/*!
 *	ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffAfterburnerMainFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );
	MTM_ASSERT( body_work->snm_work.reg_node_max );


	//I—¹ŠÄ‹
	if ( obj_work->disp_flag & OBD_DISP_END ){
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}

	//—LŒøƒtƒ‰ƒOŠÄ‹
	if ( !(body_work->flag & GMD_BOSS2_BODY_FLAG_AFTERBURNER_ACTIVE) ){
		ObjDrawKillAction3DES( obj_work );
	}

	// –{‘ÌSNMƒ}ƒgƒŠƒNƒX‚Å‚­‚Á‚Â‚¯‚é
	GmBsCmnUpdateObject3DESStuckWithNode(
			obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_BODY],
			TRUE );
}

// =======================================================================
// gmBoss2EffAfterburnerSmokeInit
/*!
 *	ƒAƒtƒ^ƒo[ƒi‰ŒƒGƒtƒFƒNƒg‚ğ‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2EffAfterburnerSmokeInit( GMS_BOSS2_BODY_WORK* body_work )
{
	//ƒGƒtƒFƒNƒg¶¬
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ(body_work);
	amAssert( body_obj_work );
	GMS_EFFECT_3DES_WORK* efct_work = GmEfctBossCmnEsCreate(
		body_obj_work, 
		GME_EFCT_BOSS_CMN_IDX_JET_B_SMORK );

	//•\¦ƒIƒtƒZƒbƒg’²®
	GmEffect3DESAddDispOffset( 
		efct_work, 
		0, 
		0, 
		GMD_BOSS2_EFFECT_AFTERBURNER_SMOKE_DISP_OFFSET_Z );

	//ƒƒCƒ“ˆ—
	OBS_OBJECT_WORK* effct_obj_work = GMM_BS_OBJ( efct_work );
	amAssert( effct_obj_work );
	effct_obj_work->ppFunc = gmBoss2EffAfterburnerSmokeMainFunc;
}

// =======================================================================
// gmBoss2EffAfterburnerSmokeMainFunc
/*!
 *	ƒAƒtƒ^ƒo[ƒi‰ŒƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffAfterburnerSmokeMainFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );
	MTM_ASSERT( body_work->snm_work.reg_node_max );

	// –{‘ÌSNMƒ}ƒgƒŠƒNƒX‚Å‚­‚Á‚Â‚¯‚é
	GmBsCmnUpdateObject3DESStuckWithNode(
			obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_BODY],
			TRUE );
}

// =======================================================================
// gmBoss2EffBodySmokeInit
/*!
 *	–{‘Ì‰ŒƒGƒtƒFƒNƒg‚ğ‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2EffBodySmokeInit( GMS_BOSS2_BODY_WORK* body_work )
{
	//ƒGƒtƒFƒNƒg¶¬
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ(body_work);
	amAssert( body_obj_work );
	GMS_EFFECT_3DES_WORK* efct_work = GmEfctBossCmnEsCreate(
		body_obj_work, 
		GME_EFCT_BOSS_CMN_IDX_BOSS_SMORK );

	//•\¦ƒIƒtƒZƒbƒg’²®
	GmEffect3DESAddDispOffset( 
		efct_work, 
		0, 
		0, 
		GMD_BOSS2_EFFECT_BODY_SMOKE_DISP_OFFSET_Z );

	//ƒƒCƒ“ˆ—
	OBS_OBJECT_WORK* effct_obj_work = GMM_BS_OBJ( efct_work );
	amAssert( effct_obj_work );
	effct_obj_work->ppFunc = gmBoss2EffBodySmokeMainFunc;
}

// =======================================================================
// gmBoss2EffBodySmokeMainFunc
/*!
 *	–{‘Ì‰ŒƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffBodySmokeMainFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );
	MTM_ASSERT( body_work->snm_work.reg_node_max );

	// –{‘ÌSNMƒ}ƒgƒŠƒNƒX‚Å‚­‚Á‚Â‚¯‚éiƒAƒtƒ^ƒo[ƒi‚Æ“¯‚¶À•Wj
	GmBsCmnUpdateObject3DESStuckWithNode(
			obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_BODY],
			TRUE );
}

// =======================================================================
// gmBoss2EffSweatInit
/*!
 *	Š¾ƒGƒtƒFƒNƒg‚ğ‰Šú‰»
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss2EffSweatInit( GMS_BOSS2_EGG_WORK* egg_work )
{
	//ƒGƒtƒFƒNƒg¶¬
	OBS_OBJECT_WORK* egg_obj_work = GMM_BS_OBJ(egg_work);
	amAssert( egg_obj_work );
	GMS_EFFECT_3DES_WORK* efct_work = GmEfctCmnEsCreate(
		egg_obj_work, 
		GME_EFCT_CMN_IDX_SWEAT );
	
	//•\¦ƒIƒtƒZƒbƒg’²®
	GmEffect3DESAddDispOffset(
			efct_work, 
			0, 
			GMD_BOSS2_EFFECT_SWEAT_DISP_OFFSET_Y, 
			0 );

	//ƒƒCƒ“ˆ—
	OBS_OBJECT_WORK* effct_obj_work = GMM_BS_OBJ( efct_work );
	amAssert( effct_obj_work );
	effct_obj_work->ppFunc = gmBoss2EffSweatMainFunc;

	//Š¾Às’†
	egg_work->flag |= GMD_BOSS2_EGG_FLAG_SWEAT_ACTIVE;
}

// =======================================================================
// gmBoss2EffSweatMainFunc
/*!
 *	Š¾ƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffSweatMainFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_EGG_WORK* egg_work = (GMS_BOSS2_EGG_WORK*)obj_work->parent_obj;
	amAssert( egg_work );
	
	if ( !(egg_work->flag & GMD_BOSS2_EGG_FLAG_SWEAT_ACTIVE) ){
		ObjDrawKillAction3DES( obj_work );
	}
	
	if ( obj_work->disp_flag & OBD_DISP_END ){
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss2EffInit
/*!
 *	ƒGƒtƒFƒNƒg‚ğ‰Šú‰»
 *
 *	@param parent_obj_work	[in] eƒIƒuƒWƒFƒNƒg
 */
// =======================================================================
OBS_OBJECT_WORK* gmBoss2EffInit( 
					OBS_DATA_WORK* data_work,
					GME_EFFECT_3DES_POS_TYPE effect_type,
					OBS_OBJECT_WORK* parent_obj_work,
					Angle16 rot_x,
					Angle16 rot_y,
					Angle16 rot_z,
					Float offset_x,
					Float offset_y,
					Float offset_z,
					BOOL flag_flip,
					BOOL flag_data_rotate)
{
	//¶¬
	OBS_OBJECT_WORK* effect_obj_work = GMM_EFFECT_CREATE_WORK(
			sizeof(GMS_EFFECT_3DES_WORK),
			NULL,
			0,
			"B02_effect");
	amAssert( effect_obj_work );

	GMS_EFFECT_3DES_WORK* effect_3des = (GMS_EFFECT_3DES_WORK*)effect_obj_work;

	//ƒf[ƒ^ƒ[ƒh
	ObjObjectAction3dESEffectLoad(
			effect_obj_work,
			&effect_3des->obj_3des,
			data_work,
			NULL,
			0,
			NULL );
	
	//ƒeƒNƒXƒ`ƒƒƒf[ƒ^ƒ[ƒh
	ObjObjectAction3dESTextureLoad(
			effect_obj_work,
			&effect_3des->obj_3des,
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_AMBTEX ),
			NULL,
			0,
			NULL,
			FALSE );	// “]‘—‚µ‚È‚¢
	
	//ƒ[ƒhÏ‚İƒeƒNƒXƒ`ƒƒƒZƒbƒg
	ObjObjectAction3dESTextureSetByDwork(
			effect_obj_work,
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_TEXLIST) );


	u32 flag = 0;

	//e
	if ( parent_obj_work ){
		effect_obj_work->parent_obj = parent_obj_work;
		effect_obj_work->pos.x = parent_obj_work->pos.x;
		effect_obj_work->pos.y = parent_obj_work->pos.y;
		effect_obj_work->pos.z = parent_obj_work->pos.z;
		flag |= GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR;
	}

	//‰ñ“]
	GmEffect3DESSetDispRotation(
			effect_3des, 
			rot_x, 
			rot_y, 
			rot_z );

	//ƒIƒtƒZƒbƒg
	GmEffect3DESAddDispOffset( 
			effect_3des, 
			offset_x, 
			offset_y,  
			offset_z );

	//ƒtƒŠƒbƒv
	if ( !flag_flip ){
		flag |= GMD_EFFECT_3DES_FLAG_NOFLIP;
	}

	//ƒf[ƒ^‚Ì‰ñ“]•Û
	if ( flag_data_rotate ){
		flag |= GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT;
	}
	
	//ƒGƒtƒFƒNƒgİ’è
	GmEffect3DESSetupBase(
			effect_3des, 
			effect_type,
			flag );

	return effect_obj_work;
}

// =======================================================================
// gmBoss2EffBallBombInit
/*!
 *	ƒgƒQƒ{[ƒ‹”š”­ƒGƒtƒFƒNƒg‚ğ‰Šú‰»
 *
 *	@param pos	[in] ¶¬À•W
 */
// =======================================================================
void gmBoss2EffBallBombInit( const VecFx32* create_pos, 
							 OBS_OBJECT_WORK* body_obj_work )
{

	OBS_OBJECT_WORK* effect_obj_work = gmBoss2EffInit( 
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_BALL_ES ),
			GME_EFFECT_3DES_POS_TYPE_EMT,
			NULL, 
			0,0,0,
			0,0,0,
			FALSE,
			FALSE);

	//À•W
	effect_obj_work->pos.x = create_pos->x;
	effect_obj_work->pos.y = create_pos->y;
	effect_obj_work->pos.z = create_pos->z + GMD_BOSS2_EFFECT_BOMB_OFFSET_Z;

	//ŠÇ—‚É’Ç‰Á
	GMS_BOSS2_MGR_WORK* mgr_work = gmBoss2MgrGetMgrWork( body_obj_work );
	gmBoss2MgrAddObject( mgr_work, effect_obj_work );

	//I—¹ˆ—
	mtTaskChangeTcbDestructor( effect_obj_work->tcb, gmBoss2EffectExitFunc );
}

// =======================================================================
// gmBoss2EffBallBombPartInit
/*!
 *	ƒgƒQƒ{[ƒ‹”j•ĞƒGƒtƒFƒNƒg‚ğ‰Šú‰»
 *
 *	@param pos	[in] ¶¬À•W
 */
// =======================================================================
OBS_OBJECT_WORK* gmBoss2EffBallBombPartInit( const VecFx32* create_pos,
											OBS_OBJECT_WORK* body_obj_work,
											fx32 spd_x)
{

	GMS_EFFECT_3DES_WORK* effect_work = (GMS_EFFECT_3DES_WORK*)gmBoss2EffInit( 
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_BALL_PART_ES ),
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			NULL, 
			0,0,0,
			0,0,0,
			FALSE,
			FALSE);
	amAssert( effect_work );

	OBS_OBJECT_WORK* effect_obj_work = &effect_work->efct_com.obj_work;
	OBS_RECT_WORK* rect_work = effect_work->efct_com.rect_work;

	//-----------------------------------------
	//‹éŒ`
	//-----------------------------------------
	// UŒ‚ƒIƒuƒWƒFƒNƒg‚Éİ’è
	GmBsCmnSetEfctAtkVsPly(&effect_work->efct_com,
						   64);//‰¼
	//UŒ‚
	ObjRectWorkSet( 
			&rect_work[GME_EFFECT_RECT_ATK],
			-8, -8, 8, 8 );
	rect_work[GME_EFFECT_RECT_ATK].ppHit = gmBoss2BallHitFunc;
	rect_work[GME_EFFECT_RECT_ATK].flag	|= OBD_RECT_ENABLE | OBD_RECT_OUT;
	rect_work[GME_EFFECT_RECT_DEF].flag |= OBD_RECT_NOHIT | OBD_RECT_OUT;

	//-----------------------------------------
	//ƒ[ƒN
	//-----------------------------------------
	//À•W
	effect_obj_work->pos.x = create_pos->x;
	effect_obj_work->pos.y = create_pos->y;
	effect_obj_work->pos.z = create_pos->z;

	//ˆÚ“®İ’è
	effect_obj_work->spd.x = spd_x;
	effect_obj_work->spd.y = -GMD_BOSS2_BALL_PART_SPEED_Y;

	effect_obj_work->move_flag |= OBD_MOVE_FALL | OBD_MOVE_NOSPDM | OBD_MOVE_JUMP;
	effect_obj_work->flag	|= OBD_OBJECT_NOCLIP;

	effect_obj_work->parent_obj = body_obj_work;	//–{‘Ì‚ğe‚Éİ’è‚·‚é‚ªƒGƒtƒFƒNƒg‚Í’Ç‚³‚¹‚È‚¢

	effect_obj_work->ppFunc = gmBoss2EffBallBombPartMainFunc;

	//ŠÇ—‚É’Ç‰Á
	GMS_BOSS2_MGR_WORK* mgr_work = gmBoss2MgrGetMgrWork( body_obj_work );
	gmBoss2MgrAddObject( mgr_work, effect_obj_work );

	//I—¹ˆ—
	mtTaskChangeTcbDestructor( effect_obj_work->tcb, gmBoss2EffectExitFunc );

	return effect_obj_work;
}

// =======================================================================
// gmBoss2EffBallBombPartMainFunc
/*!
 *	ƒgƒQƒ{[ƒ‹”j•ĞƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffBallBombPartMainFunc( OBS_OBJECT_WORK* obj_work )
{
	//I—¹ŠÄ‹
	if ( obj_work->disp_flag & OBD_DISP_END ){
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}
	
	if ( ObjViewOutCheck(obj_work->pos.x, obj_work->pos.y, GMD_BOSS2_EFFECT_CLIP_OFFSET, 0, 0, 0, 0) ){
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss2EffBlitzInit
/*!
 *	“dŒ‚ƒGƒtƒFƒNƒg‚ğ‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2EffBlitzInit( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ(body_work);
	amAssert( body_obj_work );

	//“dŒ‚ƒRƒA
	OBS_OBJECT_WORK* effect_obj_work_core_l = gmBoss2EffInit( 
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_BLITZ00_ES ),
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			body_obj_work, 
			0,0,0,
			GMD_BOSS2_EFFECT_BLITZ_CORE_DISP_OFFSET,0,0,
			FALSE,
			FALSE);	
	effect_obj_work_core_l->ppFunc = gmBoss2EffBlitzMainFuncBlitzCoreL;
	OBS_OBJECT_WORK* effect_obj_work_core_r = gmBoss2EffInit( 
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_BLITZ00_ES ),
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			body_obj_work, 
			0,0,0,
			-GMD_BOSS2_EFFECT_BLITZ_CORE_DISP_OFFSET,0,0,
			FALSE,
			FALSE);	
	effect_obj_work_core_r->ppFunc = gmBoss2EffBlitzMainFuncBlitzCoreR;

	//“dŒ‚ü
	OBS_OBJECT_WORK* effect_obj_work_line = gmBoss2EffInit( 
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_BLITZ01_ES ),
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			body_obj_work, 
			0,0,GMD_BOSS2_EFFECT_BLITZ_LINE_DISP_ROT_Z,
			0,GMD_BOSS2_EFFECT_BLITZ_LINE_DISP_OFFSET_Y_CREATE,0,
			FALSE,
			FALSE);	
	effect_obj_work_line->ppFunc = gmBoss2EffBlitzMainFuncBlitzLineCreate;
#if !_IPHONE
	//“dŒ‚ƒA[ƒ€
	OBS_OBJECT_WORK* effect_obj_work_arm_l = gmBoss2EffInit( 
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_BLITZ02_ES ),
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			body_obj_work, 
			0,0,-GMD_BOSS2_EFFECT_BLITZ_ARM_DISP_ROT_Z,
			-GMD_BOSS2_EFFECT_BLITZ_ARM_DISP_OFFSET,GMD_BOSS2_EFFECT_BLITZ_ARM_DISP_OFFSET,0,
			FALSE,
			FALSE);
	effect_obj_work_arm_l->ppFunc = gmBoss2EffBlitzMainFuncBlitzCoreL;
	OBS_OBJECT_WORK* effect_obj_work_arm_r = gmBoss2EffInit( 
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_BLITZ02_ES ),
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			body_obj_work, 
			0,0,GMD_BOSS2_EFFECT_BLITZ_ARM_DISP_ROT_Z,
			GMD_BOSS2_EFFECT_BLITZ_ARM_DISP_OFFSET,GMD_BOSS2_EFFECT_BLITZ_ARM_DISP_OFFSET,0,
			FALSE,
			FALSE);
	effect_obj_work_arm_r->ppFunc = gmBoss2EffBlitzMainFuncBlitzCoreR;

	//“dŒ‚ƒA[ƒ€
	OBS_OBJECT_WORK* effect_obj_work_arm_l_2 = gmBoss2EffInit( 
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_BLITZ02_ES ),
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			body_obj_work, 
			0,0,-GMD_BOSS2_EFFECT_BLITZ_ARM_DISP_ROT_Z,
			-GMD_BOSS2_EFFECT_BLITZ_ARM_DISP_OFFSET,GMD_BOSS2_EFFECT_BLITZ_ARM_DISP_OFFSET,0,
			FALSE,
			FALSE);
	effect_obj_work_arm_l_2->ppFunc = gmBoss2EffBlitzMainFuncBlitzL;
	OBS_OBJECT_WORK* effect_obj_work_arm_r_2 = gmBoss2EffInit( 
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_BLITZ02_ES ),
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			body_obj_work, 
			0,0,GMD_BOSS2_EFFECT_BLITZ_ARM_DISP_ROT_Z,
			GMD_BOSS2_EFFECT_BLITZ_ARM_DISP_OFFSET,GMD_BOSS2_EFFECT_BLITZ_ARM_DISP_OFFSET,0,
			FALSE,
			FALSE);
	effect_obj_work_arm_r_2->ppFunc = gmBoss2EffBlitzMainFuncBlitzR;
#endif // !_IPHONE
	//Às’†
	body_work->flag |= GMD_BOSS2_BODY_FLAG_BLITZ_ACTIVE;

	//ŠÇ—‚É’Ç‰Á
	GMS_BOSS2_MGR_WORK* mgr_work = gmBoss2MgrGetMgrWork( body_obj_work );
	gmBoss2MgrAddObject( mgr_work, effect_obj_work_core_l );
	gmBoss2MgrAddObject( mgr_work, effect_obj_work_core_r );
	gmBoss2MgrAddObject( mgr_work, effect_obj_work_line );
#if !_IPHONE
	gmBoss2MgrAddObject( mgr_work, effect_obj_work_arm_l );
	gmBoss2MgrAddObject( mgr_work, effect_obj_work_arm_r );
	gmBoss2MgrAddObject( mgr_work, effect_obj_work_arm_l_2 );
	gmBoss2MgrAddObject( mgr_work, effect_obj_work_arm_r_2 );
#endif // !_IPHONE
	//I—¹ˆ—
	mtTaskChangeTcbDestructor( effect_obj_work_core_l->tcb, gmBoss2EffectExitFunc );
	mtTaskChangeTcbDestructor( effect_obj_work_core_r->tcb, gmBoss2EffectExitFunc );
	mtTaskChangeTcbDestructor( effect_obj_work_line->tcb, gmBoss2EffectExitFunc );
#if !_IPHONE
	mtTaskChangeTcbDestructor( effect_obj_work_arm_l->tcb, gmBoss2EffectExitFunc );
	mtTaskChangeTcbDestructor( effect_obj_work_arm_r->tcb, gmBoss2EffectExitFunc );
	mtTaskChangeTcbDestructor( effect_obj_work_arm_l_2->tcb, gmBoss2EffectExitFunc );
	mtTaskChangeTcbDestructor( effect_obj_work_arm_r_2->tcb, gmBoss2EffectExitFunc );
#endif // !_IPHONE
}

// =======================================================================
// gmBoss2EffBlitzMainFuncBlitzLineCreate
/*!
 *	“dŒ‚ƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffBlitzMainFuncBlitzLineCreate( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );

	//I—¹ŠÄ‹
	if ( obj_work->disp_flag & OBD_DISP_END ){
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}
	
	if ( !(body_work->flag & GMD_BOSS2_BODY_FLAG_BLITZ_ACTIVE) ){
		ObjDrawKillAction3DES( obj_work );
	}
	if (obj_work->parent_obj) {
		obj_work->dir.z	= obj_work->parent_obj->dir.z;
	}

	//İ’uƒm[ƒh‚Ö
	GmBsCmnUpdateObject3DESStuckWithNode(
			obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_CATCH],
			TRUE );

	++obj_work->user_timer;
	if ( obj_work->user_timer < GMD_BOSS2_BODY_PINBALL_FRAME_CREATE_BLITZ_END ){
		return;
	}
	obj_work->user_timer = 0;

	//•`‰æƒIƒtƒZƒbƒg•ÏX
	GMS_EFFECT_3DES_WORK* effect_3des = (GMS_EFFECT_3DES_WORK*)obj_work;
	GmEffect3DESAddDispOffset( 
			effect_3des, 
			0, 
			GMD_BOSS2_EFFECT_BLITZ_LINE_DISP_OFFSET_Y_NORMAL,  
			0 );

	//ˆ—ŠÖ”•ÏX
	obj_work->ppFunc = gmBoss2EffBlitzMainFuncBlitzLineNormal;
}

// =======================================================================
// gmBoss2EffBlitzMainFuncBlitzLineNormal
/*!
 *	“dŒ‚ƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffBlitzMainFuncBlitzLineNormal( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );

	//I—¹ŠÄ‹
	if ( obj_work->disp_flag & OBD_DISP_END ){
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}
	
	if ( !(body_work->flag & GMD_BOSS2_BODY_FLAG_BLITZ_ACTIVE) ){
		ObjDrawKillAction3DES( obj_work );
	}
	if (obj_work->parent_obj) {
		obj_work->dir.z	= obj_work->parent_obj->dir.z;
	}

	//İ’uƒm[ƒh‚Ö
	GmBsCmnUpdateObject3DESStuckWithNode(
			obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_CATCH],
			TRUE );
}

// =======================================================================
// gmBoss2EffBlitzMainFuncBlitzCoreL
/*!
 *	“dŒ‚ƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffBlitzMainFuncBlitzCoreL( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );

	//I—¹ŠÄ‹
	if ( obj_work->disp_flag & OBD_DISP_END ){
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}
	
	if ( !(body_work->flag & GMD_BOSS2_BODY_FLAG_BLITZ_ACTIVE) ){
		ObjDrawKillAction3DES( obj_work );
	}
	if (obj_work->parent_obj) {
		obj_work->dir.z	= obj_work->parent_obj->dir.z;
	}
	//İ’uƒm[ƒh‚Ö
	GmBsCmnUpdateObject3DESStuckWithNode(
			obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_BLITZ_CORE_L],
			TRUE );
}

// =======================================================================
// gmBoss2EffBlitzMainFuncBlitzCoreR
/*!
 *	“dŒ‚ƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffBlitzMainFuncBlitzCoreR( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );
	
	if ( !(body_work->flag & GMD_BOSS2_BODY_FLAG_BLITZ_ACTIVE) ){
		ObjDrawKillAction3DES( obj_work );
	}
	if (obj_work->parent_obj) {
		obj_work->dir.z	= obj_work->parent_obj->dir.z;
	}

	//İ’uƒm[ƒh‚Ö
	GmBsCmnUpdateObject3DESStuckWithNode(
			obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_BLITZ_CORE_R],
			TRUE );
}

// =======================================================================
// gmBoss2EffBlitzMainFuncBlitzL
/*!
 *	“dŒ‚ƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffBlitzMainFuncBlitzL( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );
	
	if ( !(body_work->flag & GMD_BOSS2_BODY_FLAG_BLITZ_ACTIVE) ){
		ObjDrawKillAction3DES( obj_work );
	}
	if (obj_work->parent_obj) {
		obj_work->dir.z	= obj_work->parent_obj->dir.z;
	}
	//İ’uƒm[ƒh‚Ö
	GmBsCmnUpdateObject3DESStuckWithNode(
			obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_BLITZ_L],
			TRUE );
}

// =======================================================================
// gmBoss2EffBlitzMainFuncBlitzR
/*!
 *	“dŒ‚ƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffBlitzMainFuncBlitzR( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );


	//I—¹ŠÄ‹
	if ( obj_work->disp_flag & OBD_DISP_END ){
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}
	
	if ( !(body_work->flag & GMD_BOSS2_BODY_FLAG_BLITZ_ACTIVE) ){
		ObjDrawKillAction3DES( obj_work );
	}
	if (obj_work->parent_obj) {
		obj_work->dir.z	= obj_work->parent_obj->dir.z;
	}

	//İ’uƒm[ƒh‚Ö
	GmBsCmnUpdateObject3DESStuckWithNode(
			obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_BLITZ_R],
			TRUE );
}

// =======================================================================
// gmBoss2EffScatterInit
/*!
 *	”òUƒGƒtƒFƒNƒg‚ğ‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2EffScatterInit( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );
	amAssert( body_obj_work );

	//”òUƒp[ƒc
	GMS_BOSS2_EFFECT_SCATTER_WORK* parent_scatter_work = NULL;
	for ( s32 i = 3; GMD_BOSS2_BODY_CNM_INDEX_MAX > i; ++i ){
		GMS_BOSS2_EFFECT_SCATTER_WORK* scatter_work = (GMS_BOSS2_EFFECT_SCATTER_WORK*)GmBsCmnCreateNodeControlObjectBySize(
				body_obj_work,
				&body_work->cnm_mgr_work,
				body_work->cnm_reg_id[i],
				&body_work->snm_work,
				body_work->snm_reg_id[GMD_BOSS2_BODY_SNM_INDEX_START_ARM+i],
				sizeof(GMS_BOSS2_EFFECT_SCATTER_WORK) );
		GMS_BS_CMN_NODE_CTRL_OBJECT* node_control_work = (GMS_BS_CMN_NODE_CTRL_OBJECT*)scatter_work;
		amAssert( node_control_work );

		//ƒ‚[ƒh•ÏX
		GmBsCmnChangeCNMModeNode( 
				&body_work->cnm_mgr_work,
				body_work->cnm_reg_id[i],
				GME_BS_CMN_CNM_MODE_REPLACE );
		GmBsCmnEnableCNMLocalCoordinate(
				&body_work->cnm_mgr_work,
				body_work->cnm_reg_id[i],
				FALSE );

		//İ’è
		GmBsCmnAttachNCObjectToSNMNode( node_control_work );
		node_control_work->is_enable = TRUE;

		//ˆ—ŠÖ”
		node_control_work->proc_update = gmBoss2EffScatterMainFunc;

		OBS_OBJECT_WORK* node_obj_work = GMM_BS_OBJ( node_control_work );
		node_obj_work->move_flag |= OBD_MOVE_FALL;

		if ( i == 4 || i == 5 || i == 7 || i == 8  ){
			OBS_OBJECT_WORK* parent_obj_work = GMM_BS_OBJ( parent_scatter_work );
			node_obj_work->spd.x = parent_obj_work->spd.x;
			node_obj_work->spd.y = parent_obj_work->spd.y;

			//scatter_work->spin_quat = parent_scatter_work->spin_quat;
		}
		else
		{
			BOOL right_flag = FALSE;
			if ( i%2 != 0 ){
				right_flag = TRUE;
			}
			//ˆÚ“®
			gmBoss2EffScatterSetParamMove( node_obj_work, right_flag );

			//‰ñ“]
			//gmBoss2EffScatterSetParamSpin( node_obj_work );
		}
		
		//ƒ†[ƒUƒNƒH[ƒ^ƒjƒIƒ“‰Šú‰»
		//nnMakeUnitQuaternion( &node_control_work->user_quat );

		//e•Û‘¶
		parent_scatter_work = scatter_work;
	}
}

// =======================================================================
// gmBoss2EffScatterMainFunc
/*!
 *	”òUƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffScatterMainFunc( OBS_OBJECT_WORK* obj_work )
{
	//GMS_BOSS2_EFFECT_SCATTER_WORK* scatter_work = (GMS_BOSS2_EFFECT_SCATTER_WORK*)obj_work;
	GMS_BS_CMN_NODE_CTRL_OBJECT* node_control_work = (GMS_BS_CMN_NODE_CTRL_OBJECT*)obj_work;

	
	//‰ñ“]
	/*nnMultiplyQuaternion( 
			&node_control_work->user_quat, 
			&scatter_work->spin_quat, 
			&node_control_work->user_quat );*/

	// p¨İ’è
	GmBsCmnSetWorldMtxFromNCObjectPosture( node_control_work );

	++obj_work->user_timer;
	if ( obj_work->user_timer < 100 ){
		return;
	}
	obj_work->user_timer = 0;
	obj_work->flag |= OBD_OBJECT_TASKCLEAR;
}

// =======================================================================
// gmBoss2EffScatterSetParamMove
/*!
 *	”òUƒGƒtƒFƒNƒgˆÚ“®İ’è
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffScatterSetParamMove( OBS_OBJECT_WORK* obj_work, BOOL right_flag )
{
	//ˆÚ“®
	s32 rand_deg = ((s32)mtMathRand() % 30) + 45;
	if ( right_flag ){
		rand_deg = -rand_deg;
	}
	Angle32 rand_angle = AKM_DEGtoA32(rand_deg + 90);

	obj_work->spd.y = -(fx32)(FX32_ONE * GMD_BOSS2_SCATTER_SPEED * nnSin(rand_angle));
	obj_work->spd.x = (fx32)(FX32_ONE * GMD_BOSS2_SCATTER_SPEED * nnCos(rand_angle));
}

// =======================================================================
// gmBoss2EffScatterSetParamSpin
/*!
 *	”òUƒGƒtƒFƒNƒg‰ñ“]İ’è
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
/*void gmBoss2EffScatterSetParamSpin( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_EFFECT_SCATTER_WORK* scatter_work = (GMS_BOSS2_EFFECT_SCATTER_WORK*)obj_work;

	//‰ñ“]
	nnMakeUnitQuaternion( &scatter_work->spin_quat );
	for (Sint32 i = 0; i < 2; ++i) {
		
		// ƒ‰ƒ“ƒ_ƒ€‚È‰ñ“]²‚ğİ’è
		NNS_VECTOR spin_axis;
		Float rand_z = FX_FX32_TO_F32( AkMathRandFx() );
		Angle16 rand_angle = AKM_DEGtoA16( 360.0f * FX_FX32_TO_F32(AkMathRandFx()) );
		AkMathGetRandomUnitVector( &spin_axis, rand_z, rand_angle );
		
		AMS_QUAT diff_rot;
		nnMakeRotateAxisQuaternion(
				&diff_rot, 
				spin_axis.x, 
				spin_axis.y, 
				spin_axis.z,
				AKM_DEGtoA32(5.0f) );
		nnMultiplyQuaternion( &scatter_work->spin_quat, &diff_rot, &scatter_work->spin_quat );
	}
}*/


// =======================================================================
// gmBoss2EffCreateRollModel
/*!
 *	‰ñ“]ƒƒXƒgƒGƒtƒFƒNƒg‚ğ¶¬iƒ‚ƒfƒ‹•”j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2EffCreateRollModel( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ(body_work);
	amAssert( body_obj_work );

	//¶¬Ï‚İ
	if ( body_work->flag & GMD_BOSS2_BODY_FLAG_ROLL_ACTIVE ){
		return;
	}

	//ƒtƒ‰ƒOİ’è
	body_work->flag |= GMD_BOSS2_BODY_FLAG_ROLL_ACTIVE;
	Angle16 rot_y = 0;
	if ( body_obj_work->disp_flag & OBD_DISP_HFLIP ){
		rot_y = (Angle16)AKM_DEGtoA32(-GMD_BOSS2_EFFECT_ROLLATTACK_DISP_ROT_Y); 
	}
	else{
		rot_y = (Angle16)AKM_DEGtoA32(GMD_BOSS2_EFFECT_ROLLATTACK_DISP_ROT_Y); 
	}
	
	//rollattack01
	const GMS_EFFECT_CREATE_PARAM create_param_01 = {
		IDB_BOSS02_EFF_ROLLATTACK01_AME,
		GME_EFFECT_3DES_POS_TYPE_MTX,
		(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),
		{ 0.0f, 0.0f, GMD_BOSS2_EFFECT_ROLLATTACK_DISP_OFFSET_Z},
		{ 0, rot_y, 0},
		3.2f,
		GmEffectDefaultMainFuncDeleteAtEnd,
		IDB_BOSS02_ROLL_ZNO,
	};
	OBS_OBJECT_WORK* obj_work_effect01 = (OBS_OBJECT_WORK*)GmEffect3dESCreateByParam(
			&create_param_01,
			body_obj_work,
			GmBoss2GetGameDatEnemyArc(),
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_ROLLATTACK01_ES ),
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_ROLL_AMBTEX ),
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_ROLL_TEXLIST ),
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_ROLL_MDL_DATA ),
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_ROLL_OBJECT ),
			sizeof(GMS_EFFECT_3DES_WORK));
	obj_work_effect01->ppFunc = gmBoss2EffRollModelMainFunc;
#if _IPHONE
	obj_work_effect01->obj_3des->command_state = OBD_DRAW_CMD_STATE_3DNN_POST;
#endif // _IPHONE

#if !_IPHONE
	//rollattack02
	const GMS_EFFECT_CREATE_PARAM create_param_02 = {
		IDB_BOSS02_EFF_ROLLATTACK02_AME,
		GME_EFFECT_3DES_POS_TYPE_MTX,
		(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),
		{ 0.0f, 0.0f, GMD_BOSS2_EFFECT_ROLLATTACK_DISP_OFFSET_Z},
		{ 0, rot_y, 0},
		1.0f,
		GmEffectDefaultMainFuncDeleteAtEnd,
		GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
	};
	OBS_OBJECT_WORK* obj_work_effect02 = (OBS_OBJECT_WORK*)GmEffect3dESCreateByParam(
			&create_param_02,
			body_obj_work,
			GmBoss2GetGameDatEnemyArc(),
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_ROLLATTACK02_ES ),
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_AMBTEX ),
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_TEXLIST ),
			NULL,
			NULL,
			sizeof(GMS_EFFECT_3DES_WORK));
	obj_work_effect02->ppFunc = gmBoss2EffRollMainFunc;
#if _IPHONE
	obj_work_effect02->obj_3des->command_state = OBD_DRAW_CMD_STATE_3DNN_POST;
#endif // _IPHONE
#endif // !_IPHONE

	//ŠÇ—‚É’Ç‰Á
	GMS_BOSS2_MGR_WORK* mgr_work = gmBoss2MgrGetMgrWork( body_obj_work );
	gmBoss2MgrAddObject( mgr_work, obj_work_effect01 );
#if !_IPHONE
	gmBoss2MgrAddObject( mgr_work, obj_work_effect02 );
#endif // !_IPHONE

	//I—¹ˆ—
	mtTaskChangeTcbDestructor( obj_work_effect01->tcb, gmBoss2EffectExitFunc );
#if !_IPHONE
	mtTaskChangeTcbDestructor( obj_work_effect02->tcb, gmBoss2EffectExitFunc );
#endif // !_IPHONE
}

// =======================================================================
// gmBoss2EffCreateRollModelLost
/*!
 *	‰ñ“]ƒƒXƒgƒGƒtƒFƒNƒg‚ğ¶¬
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss2EffCreateRollModelLost( GMS_BOSS2_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ(body_work);
	amAssert( body_obj_work );

	Angle16 rot_y = 0;
	if ( body_obj_work->disp_flag & OBD_DISP_HFLIP ){
		rot_y = (Angle16)AKM_DEGtoA32(-GMD_BOSS2_EFFECT_ROLLATTACK_DISP_ROT_Y); 
	}
	else{
		rot_y = (Angle16)AKM_DEGtoA32(GMD_BOSS2_EFFECT_ROLLATTACK_DISP_ROT_Y); 
	}
#if !_IPHONE
	//rollattack03
	const GMS_EFFECT_CREATE_PARAM create_param_03 = {
		IDB_BOSS02_EFF_ROLLATTACK03_AME,
		GME_EFFECT_3DES_POS_TYPE_MTX,
		(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),
		{ 0.0f, 0.0f, GMD_BOSS2_EFFECT_ROLLATTACK_DISP_OFFSET_Z},
		{ 0, rot_y, 0},
		3.2f,
		GmEffectDefaultMainFuncDeleteAtEnd,
		IDB_BOSS02_ROLL_ZNO,
	};
	OBS_OBJECT_WORK* effect_work_03 = (OBS_OBJECT_WORK*)GmEffect3dESCreateByParam(
			&create_param_03,
			body_obj_work,
			GmBoss2GetGameDatEnemyArc(),
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_ROLLATTACK03_ES ),
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_ROLL_AMBTEX ),
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_ROLL_TEXLIST ),
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_ROLL_MDL_DATA ),
			ObjDataGet( GMD_DWORK_NO_BOSS_02_EF_ROLL_OBJECT ),
			sizeof(GMS_EFFECT_3DES_WORK));

	//ŠÇ—‚É’Ç‰Á
	GMS_BOSS2_MGR_WORK* mgr_work = gmBoss2MgrGetMgrWork(body_obj_work);
	gmBoss2MgrAddObject( mgr_work, effect_work_03 );

	//I—¹ˆ—
	mtTaskChangeTcbDestructor( effect_work_03->tcb, gmBoss2EffectExitFunc );
#endif // !_IPHONE
}

// =======================================================================
// gmBoss2EffRollModelMainFunc
/*!
 *	‰ñ“]ƒGƒtƒFƒNƒg‚ğXViƒ‚ƒfƒ‹•”j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffRollModelMainFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );

	//I—¹ŠÄ‹
	if ( obj_work->disp_flag & OBD_DISP_END ){
		//obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}

	//—LŒøƒtƒ‰ƒOŠÄ‹
	if ( !(body_work->flag & GMD_BOSS2_BODY_FLAG_ROLL_ACTIVE) ){
		//ObjDrawKillAction3DES( obj_work );
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss2EffRollMainFunc
/*!
 *	‰ñ“]ƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss2EffRollMainFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS2_BODY_WORK* body_work = (GMS_BOSS2_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );

	//I—¹ŠÄ‹
	if ( obj_work->disp_flag & OBD_DISP_END ){
		//obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}

	//—LŒøƒtƒ‰ƒOŠÄ‹
	if ( !(body_work->flag & GMD_BOSS2_BODY_FLAG_ROLL_ACTIVE) ){
		//ObjDrawKillAction3DES( obj_work );
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
