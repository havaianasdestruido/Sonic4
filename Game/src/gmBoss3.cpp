// ==========================================================================
/*!
  @file gmBoss3.cpp
  @brief ƒ{ƒX3

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmBoss3.cpp 2 2011-04-11 05:21:26Z thamada $
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
#if _IPHONE
#include "gmWaterSurface.h"
#endif //_IPHONE
#include "gmGmkCamScrLim.h"
#include "gmGmkBoss3Pillar.h"

#include "gmBoss3.h"

// ƒf[ƒ^ƒwƒbƒ_
#include "../file/common/arc/BOSS03.hmb"
#include "../file/common/model/BOSS03_MDL.hmb"
#include "../file/common/model/BOSS03_BODY_MTN.hmb"
#include "../file/common/model/BOSS03_EGG_MTN.hmb"


//----- Definitions ---------------------------------------------------------

#define _BOSS3_TEST_STOP_MOVE	(0)	//ƒeƒXƒg—pƒ{ƒXˆÚ“®’â~
#define _BOSS3_TEST_DEFEAT		(0)	//ƒeƒXƒg—pƒ{ƒX‘Ì—Í1



// =======================================================================
//‹¤’Ê
// =======================================================================
#define GMD_BOSS3_BLEND_SPD					((Float)0.125f)		//ƒ‚[ƒVƒ‡ƒ“ƒuƒŒƒ“ƒh‘¬“x

//ƒp[ƒcƒCƒ“ƒfƒbƒNƒX
enum GME_BOSS3_PART_IDX{
	GMD_BOSS3_PART_IDX_BODY	= 0,	//–{‘Ì
	GMD_BOSS3_PART_IDX_EGG,			//ƒGƒbƒOƒ}ƒ“
	
	GMD_BOSS3_PART_IDX_MAX
};

//ƒAƒNƒVƒ‡ƒ“ID
enum GME_BOSS3_ACT_ID{
	GMD_BOSS3_ACT_ID_START	= 0,		//ŠJn‰‰oi“oêj
	GMD_BOSS3_ACT_ID_MOVE,				//ˆÚ“®

	GMD_BOSS3_ACT_ID_PRE_BATTLE,		//ƒoƒgƒ‹‘O‰‰o

	GMD_BOSS3_ACT_ID_SEARCH_L,			//’T‚·
	GMD_BOSS3_ACT_ID_SEARCH_R,			//’T‚·
	GMD_BOSS3_ACT_ID_SIGN,				//‡}
	GMD_BOSS3_ACT_ID_LAUGH_L,			//Î‚¢
	GMD_BOSS3_ACT_ID_LAUGH_R,			//Î‚¢

	GMD_BOSS3_ACT_ID_ESCAPE,			//“¦–S

	GMD_BOSS3_ACT_ID_MAX
};
enum GME_BOSS3_EGG_ACT_ID{
	GMD_BOSS3_EGG_ACT_ID_LAUGH_L = 0,	//Î‚¢
	GMD_BOSS3_EGG_ACT_ID_LAUGH_R,		//Î‚¢
	GMD_BOSS3_EGG_ACT_ID_DAMAGE,		//ƒ_ƒ[ƒW
	
	GMD_BOSS3_EGG_ACT_ID_MAX
};

// =======================================================================
//ŠÇ—
// =======================================================================
#define GMD_BOSS3_MGR_LIFE_CHASE	( 6 )				//ƒ{ƒX‘Ì—Íi’ÇÕj
#define GMD_BOSS3_MGR_LIFE_BATTLE	( 2 )				//ƒ{ƒX‘Ì—Íiƒoƒgƒ‹j
#if	_BOSS3_TEST_DEFEAT
	#define GMD_BOSS3_MGR_LIFE			( 1 )
#else
	#define GMD_BOSS3_MGR_LIFE			( GMD_BOSS3_MGR_LIFE_CHASE + GMD_BOSS3_MGR_LIFE_BATTLE )	//ƒ{ƒX‘Ì—Í
	#define GMD_BOSS3_MGR_LIFE_FINAL	( 4 )	//ƒ{ƒX‘Ì—Íiƒtƒ@ƒCƒiƒ‹ƒ][ƒ“j
#endif	//_BOSS3_TEST_DEFEAT

//ƒtƒ‰ƒO
#define GMD_BOSS3_MGR_FLAG_SETUP_COMPLETE	(1 << 0)	//¶¬Š®—¹
#define GMD_BOSS3_MGR_FLAG_CLEAR_BOSS		(1 << 1)	//íœƒtƒ‰ƒO

// =======================================================================
//–{‘Ì
// =======================================================================
#define GMD_BOSS3_ANGLE_LEFT						(AKM_DEGtoA16(300.f))	//¶Œü‚«Šp“x
#define GMD_BOSS3_ANGLE_RIGHT						(AKM_DEGtoA16(60.f))	//‰EŒü‚«Šp“x
#define GMD_BOSS3_BODY_MOVE_AREA_LIMIT_WIDTH		((fx32)80*FX32_ONE)		//ˆÚ“®”ÍˆÍ
#define GMD_BOSS3_BODY_FIELD_RECT_SIZE				(24)					//’nŒ`•Ó‚è
#define GMD_BOSS3_EFFECT_CLIP_OFFSET				(64)					//‰æ–ÊŠO”»’èƒIƒtƒZƒbƒg 

//ƒm[ƒhƒ}ƒgƒŠƒNƒX
enum GME_BOSS3_BODY_SNM_INDEX{				
	GMD_BOSS3_BODY_SNM_INDEX_BODY = 0,		//–{‘ÌÚ‘±iƒGƒbƒOƒ}ƒ“AƒAƒtƒ^ƒo[ƒij
	GMD_BOSS3_BODY_SNM_INDEX_MAX,
};
#define GMD_BOSS3_BODY_NODE_CNM_NUM			(0)	//ƒm[ƒhƒ}ƒgƒŠƒNƒX‘€ìˆ—‚É“o˜^‚·‚éƒm[ƒh”

//ˆÚ“®’l
#if _BOSS3_TEST_STOP_MOVE
	#define GMD_BOSS3_BODY_CHASE_MOVE_SPEED			((fx32)(0*FX32_ONE))	//ƒXƒs[ƒh
	#define GMD_BOSS3_BODY_CHASE_MOVE_SPEED_ESCAPE	((fx32)(0*FX32_ONE))	//ƒXƒs[ƒh
	#define GMD_BOSS3_BODY_BATTLE_MOVE_SPEED_MOVE	((fx32)0*FX32_ONE)		//’Êíó‘Ô‚ÌˆÚ“®—Ê
#else
	#define GMD_BOSS3_BODY_CHASE_MOVE_SPEED			((fx32)(1.6f*FX32_ONE))	//ƒXƒs[ƒh
	#define GMD_BOSS3_BODY_CHASE_MOVE_SPEED_ESCAPE	((fx32)(1.6f*FX32_ONE))	//ƒXƒs[ƒh
	#define GMD_BOSS3_BODY_BATTLE_MOVE_SPEED_MOVE	((fx32)1.6f*FX32_ONE)		//’Êíó‘Ô‚ÌˆÚ“®—Ê
#endif	//_BOSS3_TEST_STOP_MOVE

//ŠJn
#define GMD_BOSS3_BODY_START_TIME_WAIT_END			(180)				//I—¹‘Ò‹@ŠÔ
#define GMD_BOSS3_BODY_START_WAIT_OUT_OFFSET		(64)				//‰æ–ÊŠO”»’è—p

//ˆÚ“®
#define GMD_BOSS3_BODY_FRAME_TURN				(60.0f)					//•ûŒü“]Š·ƒtƒŒ[ƒ€”
#define GMD_BOSS3_BODY_FRAME_DRIFT				(60.0f)					//ƒhƒŠƒtƒgƒtƒŒ[ƒ€”
//’ÇÕ
#define GMD_BOSS3_CHASE_ADJUST_SPEED			(0.2f)					//ƒ_ƒ[ƒWó‚¯‚½Û‚ÌˆÚ“®’l•â³

//ƒoƒgƒ‹
#define GMD_BOSS3_BATTLE_MOVE_NUM				(2)						//ƒoƒgƒ‹‚É‚¨‚¯‚éÅ‘åˆÚ“®‰ñ”

#if _IPHONE
#define GMD_BOSS3_BODY_BATTLE_CAMERA_SCALE		(0.85f)					//ƒJƒƒ‰ƒXƒP[ƒ‹
#else
#define GMD_BOSS3_BODY_BATTLE_CAMERA_SCALE		(0.73f)					//ƒJƒƒ‰ƒXƒP[ƒ‹
#endif //_IPHONE
#define GMD_BOSS3_BODY_BATTLE_CAMERA_FRAME		(60.0f)					//ƒJƒƒ‰ƒXƒP[ƒ‹ƒtƒŒ[ƒ€

#define GMD_BOSS3_BATTLE_FRAME_WAIT_START			(120)					//ŠJn‘Ò‚¿
#define GMD_BOSS3_BATTLE_FRAME_WAIT_ACTIVE			(240)					//’Œ‚ªo‚é‚Ì‘Ò‚¿
#define GMD_BOSS3_BATTLE_FRAME_WAIT_ACTIVE_FINAL	(150)					//’Œ‚ªo‚é‚Ì‘Ò‚¿
#define GMD_BOSS3_BATTLE_FRAME_WAIT					(120)					//’â~‚Ì‘Ò‚¿
#define GMD_BOSS3_BATTLE_FRAME_WAIT_RETURN			(30)					//’Œ‚ª–ß‚é‚Ì‘Ò‚¿


//ƒ_ƒ[ƒW
#define GMD_BOSS3_BODY_DEF_PLAYER_MOVE_X_HOMING		((fx32)3*FX32_ONE)	//‚­‚ç‚¢ƒvƒŒƒCƒ„ˆÚ“®—Ê
#define GMD_BOSS3_BODY_DEF_PLAYER_MOVE_Y_HOMING		((fx32)3*FX32_ONE)	//‚­‚ç‚¢ƒvƒŒƒCƒ„ˆÚ“®—Ê
#define GMD_BOSS3_BODY_DEF_PLAYER_MOVE_X_NORMAL		((fx32)(0.5f*FX32_ONE))	//‚­‚ç‚¢ƒvƒŒƒCƒ„ˆÚ“®—Ê
#define GMD_BOSS3_BODY_DEF_PLAYER_MOVE_Y_NORMAL		((fx32)(0.3f*FX32_ONE))	//‚­‚ç‚¢ƒvƒŒƒCƒ„ˆÚ“®—Ê
#define GMD_BOSS3_BODY_DEF_PLAYER_NO_JUMP_MOVE_TIME	((fx32)25*FX32_ONE)	//‚­‚ç‚¢ƒvƒŒƒCƒ„‚ªƒWƒƒƒ“ƒvˆÚ“®‚Å‚«‚È‚¢ŠÔ
#define GMD_BOSS3_BODY_DEF_NO_HIT_TIME				((u32)10)			//‚­‚ç‚¢ƒqƒbƒg–³ŒøŠÔ
#define GMD_BOSS3_BODY_INVINVIBLE_TIME				((u32)120)			//–³“GŠÔ
#define GMD_BOSS3_BODY_DMG_FLICKER_RADIUS			((Float)32.0f)		//“_–Åˆ—”¼Œa

//Œ‚”j
#define GMD_BOSS3_BODY_DEFEAT_TIME_WAIT_START		((s32)40)				//ŠJn‘Ò‚¿ŠÔ
#define GMD_BOSS3_BODY_DEFEAT_TIME_WAIT_BOMB		((s32)120)				//”š”­‘Ò‚¿ŠÔ
#define GMD_BOSS3_BODY_DEFEAT_TIME_WAIT_SCATTER		((s32)40)				//‚Î‚çT‚«‘Ò‚¿ŠÔ
#define GMD_BOSS3_BODY_DEFEAT_TIME_WAIT_END			((s32)120)				//I—¹‘Ò‚¿ŠÔ
#define GMD_BOSS3_BODY_DEFEAT_FALL_POS_Y			((fx32)450*FX32_ONE)	//—‰ºÀ•W

//‘å”š”­
#define GMD_BOSS3_BODY_DEFEAT_FLASH_INTO_TIME		(4)						// Œ‚”j‚Ìƒtƒ‰ƒbƒVƒ…Š®‘S‚É”’‚É‚È‚é‚Ü‚Å‚ÌƒtƒŒ[ƒ€
#define GMD_BOSS3_BODY_DEFEAT_FLASH_KEEP_TIME		(5)						// Œ‚”j‚Ìƒtƒ‰ƒbƒVƒ…Š®‘S‚É”’‚ÌŠÔ‚ÌƒtƒŒ[ƒ€
#define GMD_BOSS3_BODY_DEFEAT_FLASH_RETURN_TIME		(30)	

//“¦–S
#define GMD_BOSS3_BODY_ESCAPE_SCROLL_UNLOCK_DISTANCE	((fx32)(-32.0f*FX32_ONE))	//ƒXƒNƒ[ƒ‹ƒƒbƒN‰ğœ‚·‚é‹——£i‰æ–Ê‰E‚©‚çj
#define GMD_BOSS3_BODY_ESCAPE_SPD_X_ADD					((fx32)(0.08f*FX32_ONE))		//ƒXƒs[ƒhÅ‘å’l
#define GMD_BOSS3_BODY_ESCAPE_SPD_Y_ADD					((fx32)(-0.01f*FX32_ONE))	//ƒXƒs[ƒhÅ‘å’l
#define GMD_BOSS3_BODY_ESCAPE_SPD_X_MAX					((fx32)(1.5f*FX32_ONE))	//ƒXƒs[ƒhÅ‘å’l
#define GMD_BOSS3_BODY_ESCAPE_SPD_Y_MAX					((fx32)(-0.3f*FX32_ONE))	//ƒXƒs[ƒhÅ‘å’l
#define GMD_BOSS3_BODY_ESCAPE_SCREEN_OUT_LENGTH			((s32)(64))		//‰æ–ÊŠO”»’è‹——£

//ƒtƒ‰ƒO
#define GMD_BOSS3_BODY_FLAG_INVINCIBLE				(1 << 0)	//–³“Gó‘Ôi“–‚½‚è‚Í‚ ‚é‚ªAƒ‰ƒCƒt‚ÍŒ¸‚ç‚È‚¢j
#define GMD_BOSS3_BODY_FLAG_AFTERBURNER_ACTIVE		(1 << 1)	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg—LŒø’†
#define GMD_BOSS3_BODY_FLAG_NOATTACK				(1 << 4)	//UŒ‚‚µ‚È‚¢

//ƒVƒOƒiƒ‹
#define GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_ESCAPE		(1 << 23)	//“¦–S’Ê’m
#define GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_BURNT		(1 << 24)	//•‚±‚°ƒeƒNƒXƒ`ƒƒ‚Ö‚Ì•ÏX’Ê’m
#define GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_AFTERBURNER	(1 << 25)	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg¶¬’Ê’m
#define GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_SCATTER		(1 << 27)	//ƒp[ƒc”òU’Ê’m
#define GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_HIT			(1 << 28)	//ƒqƒbƒg’Ê’m
#define GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_DAMAGE		(1 << 29)	//ƒ_ƒ[ƒW’Ê’m
#define GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_DAMAGE		(1 << 30)	//ƒ_ƒ[ƒW’Ê’m
#define GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_DEFEAT		(1 << 31)	//Œ‚”j’Ê’m



//ó‘Ô
enum GME_BOSS3_BODY_STATE{
	GMD_BOSS3_BODY_STATE_NO_OPERATION = 0,		//‰½‚à‚µ‚È‚¢
	GMD_BOSS3_BODY_STATE_START,					//ŠJn

	GMD_BOSS3_BODY_STATE_CHASE_MOVE,			//’ÇÕˆÚ“®

	GMD_BOSS3_BODY_STATE_PRE_BATTLE,			//ƒoƒgƒ‹‘O‰‰o
	GMD_BOSS3_BODY_STATE_BATTLE,				//ƒoƒgƒ‹ˆÚ“®

	GMD_BOSS3_BODY_STATE_DEFEAT,				//Œ‚”j
	GMD_BOSS3_BODY_STATE_ESCAPE,				//“¦–S
	
	GMD_BOSS3_BODY_STATE_MAX
};

// =======================================================================
//ƒGƒbƒOƒ}ƒ“
// =======================================================================
//ƒtƒ‰ƒO
#define GMD_BOSS3_EGG_FLAG_EGG_ACT_ACTIVE	(1 << 0)	//ê—pƒAƒNƒVƒ‡ƒ“’†ƒtƒ‰ƒO
#define GMD_BOSS3_EGG_FLAG_SWEAT_ACTIVE		(1 << 1)	//Š¾ƒGƒtƒFƒNƒg—LŒø’†ƒtƒ‰ƒO

//ƒV[ƒPƒ“ƒX
enum GME_BOSS3_EGGMAN_SEQ{
	GMD_BOSS3_EGGMAN_SEQ_IDLE = 0,	//‘Ò‹@

	GMD_BOSS3_EGGMAN_SEQ_MAX
};

// =======================================================================
//ƒGƒtƒFƒNƒg
// =======================================================================
#define GMD_BOSS3_EFFECT_BOMB_OFFSET_Z						((fx32)(FX32_ONE * 32))	//”š”­ƒGƒtƒFƒNƒgÀ•W

#define GMD_BOSS3_EFFECT_SWEAT_DIST_OFFSET_Y				((Float)32.f)	//Š¾ƒGƒtƒFƒNƒg•\¦À•W
#define GMD_BOSS3_EFFECT_AFTERBURNER_DIST_OFFSET_Z			((Float)-30.f)	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg•\¦À•W
#define GMD_BOSS3_EFFECT_AFTERBURNER_SMOKE_DIST_OFFSET_Z	((Float)-32.f)	//ƒAƒtƒ^ƒo[ƒi‰ŒƒGƒtƒFƒNƒg•\¦À•W
#define GMD_BOSS3_EFFECT_BODY_SMOKE_DIST_OFFSET_Z			((Float)-32.f)	//–{‘Ì‰ŒƒGƒtƒFƒNƒg•\¦À•W

#define	GMD_BOSS3_SCATTER_SPEED								(1.0f)			//”òUƒGƒtƒFƒNƒg‘¬“x

// =======================================================================
//\‘¢‘Ì
// =======================================================================
//ƒp[ƒcƒAƒNƒVƒ‡ƒ“î•ñ\‘¢‘Ì
typedef struct tag_GMS_BOSS3_PART_ACT_INFO
{
	Uint16 mtn_id;			///< ƒ‚[ƒVƒ‡ƒ“”Ô†
	Uint8 is_maintain;		///< ‘O‚ÌƒAƒNƒVƒ‡ƒ“Œp‘±
	Uint8 is_repeat;		///< ƒŠƒs[ƒg
	Float mtn_spd;			///< ƒ‚[ƒVƒ‡ƒ“Ä¶‘¬“x
	BOOL is_blend;			///< ƒuƒŒƒ“ƒh—L–³
	Float blend_spd;		///< ƒuƒŒƒ“ƒh‘¬“x
	BOOL is_merge_manual;	///< ƒ}ƒjƒ…ƒAƒ‹ƒ}[ƒW—L–³i–¢À‘•j
}GMS_BOSS3_PART_ACT_INFO;

typedef struct tag_GMS_BOSS3_EFF_BOMB_WORK GMS_BOSS3_EFF_BOMB_WORK;
typedef struct tag_GMS_BOSS3_MGR_WORK GMS_BOSS3_MGR_WORK;
typedef struct tag_GMS_BOSS3_BODY_WORK GMS_BOSS3_BODY_WORK;
typedef struct tag_GMS_BOSS3_EGG_WORK GMS_BOSS3_EGG_WORK;

//ó‘Ô—pŠÖ”
typedef void (*GMF_BOSS3_BODY_STATE_FUNC)(GMS_BOSS3_BODY_WORK* body_work);		
typedef void (*GMF_BOSS3_EGG_STATE_FUNC)(GMS_BOSS3_EGG_WORK* egg_work);	

//”š”­ƒGƒtƒFƒNƒgƒ[ƒN
struct tag_GMS_BOSS3_EFF_BOMB_WORK
{
	OBS_OBJECT_WORK* parent_obj;
	Uint32 interval_timer;
	Uint32 interval_min;
	Uint32 interval_max;
	fx32 pos[MTD_XY];
	fx32 area[MTD_WH];
};

//ŠÇ—ƒ[ƒN
struct tag_GMS_BOSS3_MGR_WORK
{
	GMS_ENEMY_3D_WORK ene_3d;		///< ƒGƒlƒ~[ƒ[ƒN
	Sint32 life;					///< ƒ‰ƒCƒt	
	Uint32 flag;					///< ƒtƒ‰ƒO
	GMS_BOSS3_BODY_WORK* body_work;	///< –{‘Ìƒ[ƒN

	s32 obj_create_count;			///< ƒIƒuƒWƒFƒNƒg¶¬”
};

//–{‘Ìƒ[ƒN
struct tag_GMS_BOSS3_BODY_WORK
{
	GMS_ENEMY_3D_WORK ene_3d;				///< ƒGƒlƒ~[ƒ[ƒN

	OBS_OBJECT_WORK* parts_objs[GMD_BOSS3_PART_IDX_MAX];	///< qƒIƒuƒWƒFƒ[ƒN

	GME_BOSS3_BODY_STATE state;				///< ó‘Ô
	GME_BOSS3_BODY_STATE prev_state;		///< ‘O‰ñ‚Ìó‘Ô
	GMF_BOSS3_BODY_STATE_FUNC proc_update;	///< XVŠÖ”
	u32 flag;								///< ƒtƒ‰ƒO
	GME_BOSS3_ACT_ID action_id;				///< ƒAƒNƒVƒ‡ƒ“ID

	s32 pattern_no;							///< ƒoƒgƒ‹ƒpƒ^[ƒ“”Ô†

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
#if _IPHONE
	BOOL is_move;							///< ˆÚ“®‚·‚é‚©”Û‚©
#endif // _IPHONE

	//ƒm[ƒhƒ}ƒgƒŠƒNƒXŒn
	GMS_BS_CMN_BMCB_MGR bmcb_mgr;			///< ƒ{ƒXƒ‚[ƒVƒ‡ƒ“ƒR[ƒ‹ƒoƒbƒNŠÇ—
	GMS_BS_CMN_SNM_WORK snm_work;			///< ƒm[ƒhƒ}ƒgƒŠƒNƒXæ“¾ˆ—ƒ[ƒN
	s32 snm_reg_id[GMD_BOSS3_BODY_SNM_INDEX_MAX];	///< Ú‘±ƒm[ƒhID

	//ƒ_ƒ[ƒWŒn
	GMS_BS_CMN_DMG_FLICKER_WORK flk_work;	///< ƒ_ƒ[ƒW“_–Åƒ[ƒN
	u32 counter_no_hit;						///< HIT–³ŒøŠÔƒJƒEƒ“ƒ^
	u32 counter_invincible;					///< –³“GŠÔƒJƒEƒ“ƒ^
	
	GMS_CMN_FLASH_SCR_WORK flash_work;		///< ƒtƒ‰ƒbƒVƒ…ƒ[ƒN

	//ƒGƒtƒFƒNƒg
	GMS_BOSS3_EFF_BOMB_WORK bomb_work;		//!< ”š”­ˆ—ƒ[ƒN
};

//ƒGƒbƒOƒ}ƒ“ƒ[ƒN
struct tag_GMS_BOSS3_EGG_WORK
{
	GMS_ENEMY_3D_WORK ene_3d;				///< ƒGƒlƒ~[ƒ[ƒN
	GME_BOSS3_EGG_ACT_ID egg_action_id;		///< ê—pƒAƒNƒVƒ‡ƒ“
	Uint32 flag;							///< ƒtƒ‰ƒO

	GMF_BOSS3_EGG_STATE_FUNC proc_update;	///< XVŠÖ”
};

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------


// =======================================================================
//ƒ{ƒX
// =======================================================================
static void* GmBoss3GetGameDatEnemyArc( void );
static void gmBoss3ChangeTextureBurnt( OBS_OBJECT_WORK *obj_work );

static void gmBoss3ExitFunc( MTS_TASK_TCB *tcb );

// =======================================================================
//ŠÇ—
// =======================================================================
static BOOL gmBoss3MgrCheckSetupComplete( GMS_BOSS3_MGR_WORK* mgr_work );
static GMS_BOSS3_MGR_WORK* gmBoss3MgrGetMgrWork( OBS_OBJECT_WORK* obj_work_parts );
static void gmBoss3MgrAddObject( GMS_BOSS3_MGR_WORK* mgr_work, OBS_OBJECT_WORK* obj_work_parts );
static void gmBoss3MgrDeleteObject( OBS_OBJECT_WORK* obj_work_parts );
static void gmBoss3MgrMainFuncWaitLoad( OBS_OBJECT_WORK* obj_work );
static void gmBoss3MgrMainFuncWaitSetup( OBS_OBJECT_WORK* obj_work );
static void gmBoss3MgrMainFunc( OBS_OBJECT_WORK* obj_work );
static void gmBoss3MgrMainFuncWaitRelease( OBS_OBJECT_WORK* obj_work );

// =======================================================================
//–{‘Ì
// =======================================================================
static void gmBoss3BodyExit( MTS_TASK_TCB* tcb );
static void gmBoss3BodyReactionPlayer( OBS_OBJECT_WORK* obj_work_player, const OBS_OBJECT_WORK* obj_work_body );
static void gmBoss3BodySetRectNormal( GMS_BOSS3_BODY_WORK* body_work );
extern void gmBoss3BodySetActionAllParts(
								  GMS_BOSS3_BODY_WORK* body_work,
								  GME_BOSS3_ACT_ID action_id,
								  BOOL force_change	= FALSE );
static void gmBoss3BodyOutFunc( OBS_OBJECT_WORK* obj_work );
static void gmBoss3BodyChaseMoveFunc( OBS_OBJECT_WORK* obj_work );
static void gmBoss3BodyDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect );

//UŒ‚–³Œø
static void gmBoss3BodySetNoHitTime( GMS_BOSS3_BODY_WORK *body_work, u32 time );
static void gmBoss3BodyUpdateNoHitTime( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodySetInvincibleTime( GMS_BOSS3_BODY_WORK *body_work, u32 time );
static void gmBoss3BodyUpdateInvincibleTime( GMS_BOSS3_BODY_WORK* body_work );


//Œü‚«
static void gmBoss3BodySetDirection( GMS_BOSS3_BODY_WORK* body_work, Angle16 deg );
static void gmBoss3BodySetDirectionNormal( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyUpdateDirection( GMS_BOSS3_BODY_WORK* body_work );

//À•WˆÚ“®
static Float gmBoss3BodyCalcMoveXNormalFrame( 
									const GMS_BOSS3_BODY_WORK* body_work,
									fx32 x,
									fx32 speed );
static void gmBoss3BodyInitMoveNormal(
						 GMS_BOSS3_BODY_WORK* body_work, 
						 const VecFx32* dest_pos,
						 Float frame );
static Float gmBoss3BodyUpdateMoveNormal( GMS_BOSS3_BODY_WORK* body_work );

//•ûŒü“]Š·
static void gmBoss3BodyInitTurn(
						 GMS_BOSS3_BODY_WORK* body_work, 
						 Angle16 dest_angle,
						 Float frame, 
						 BOOL flag_positive );
static Float gmBoss3BodyUpdateTurn( GMS_BOSS3_BODY_WORK* body_work );

//’ÇÕ
static BOOL gmBoss3BodyChaseCheckTurn( const GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyChaseAdjustMoveSpeed( GMS_BOSS3_BODY_WORK* body_work );

//ƒoƒgƒ‹
static s32 gmBoss3BodyBattleCalcPattern( GMS_BOSS3_BODY_WORK* body_work );
static BOOL gmBoss3BodyBattleInitMovePattern( 
											 GMS_BOSS3_BODY_WORK* body_work,
											 s32 pattern_no,
											 s32 pos_index,
											 fx32 move_speed);
static BOOL gmBoss3BodyBattleCheckTurn( const GMS_BOSS3_BODY_WORK* body_work );
static OBS_OBJECT_WORK* gmBoss3BodyBattleSearchPillar( void );

//ƒ_ƒ[ƒW
static void gmBoss3BodyDamage( GMS_BOSS3_BODY_WORK* body_work );

//“¦–S
static BOOL gmBoss3BodyEscapeCheckScreenOut( const GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyEscapeAddjustSpeed( GMS_BOSS3_BODY_WORK* body_work );

//ó‘Ô
static void gmBoss3BodyChangeState(
							GMS_BOSS3_BODY_WORK* body_work,
							GME_BOSS3_BODY_STATE state );
static void gmBoss3BodyMainFuncWaitSetup( OBS_OBJECT_WORK* obj_work );
static void gmBoss3BodyMainFunc( OBS_OBJECT_WORK* obj_work );

//ŠJn
static void gmBoss3BodyStateStartEnter( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateStartLeave( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateStartUpdateWaitScrLimit( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateStartUpdateWait( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateStartUpdateEnd( GMS_BOSS3_BODY_WORK* body_work );

//’ÇÕˆÚ“®
static void gmBoss3BodyStateChaseMoveEnter( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateChaseMoveLeave( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateChaseMoveUpdate( GMS_BOSS3_BODY_WORK* body_work );

//ƒoƒgƒ‹‘O‰‰o
static void gmBoss3BodyStatePreBattleEnter( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStatePreBattleLeave( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStatePreBattleUpdateStart( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStatePreBattleUpdateTurn( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStatePreBattleUpdateLaugh( GMS_BOSS3_BODY_WORK* body_work );

//ƒoƒgƒ‹ˆÚ“®
static void gmBoss3BodyStateBattleEnter( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateBattleLeave( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateBattleUpdateMoveCenter( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateBattleUpdateSearch( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateBattleUpdateMoveFirst( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateBattleUpdateSign( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateBattleUpdateWaitPillar( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateBattleUpdateMoveSecond( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateBattleUpdateWaitActive( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateBattleUpdateWaitReturn( GMS_BOSS3_BODY_WORK* body_work );

//Œ‚”j
static void gmBoss3BodyStateDefeatEnter( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateDefeatLeave( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateDefeatUpdateStart( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateDefeatUpdateFall( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateDefeatUpdateExplode( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateDefeatUpdateScatter( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateDefeatUpdateEnd( GMS_BOSS3_BODY_WORK* body_work );

//“¦–S
static void gmBoss3BodyStateEscapeEnter( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateEscapeLeave( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateEscapeUpdateScrollLock( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateEscapeUpdateWaitScreenOut( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3BodyStateEscapeUpdateFinalZone( GMS_BOSS3_BODY_WORK* body_work );

// =======================================================================
//ƒGƒbƒOƒ}ƒ“
// =======================================================================
static void gmBoss3EggChangeAction( 
							GMS_BOSS3_EGG_WORK* egg_work,
							GME_BOSS3_EGG_ACT_ID action_id,
							BOOL force_change = FALSE );
static void gmBoss3EggRevertAction( GMS_BOSS3_EGG_WORK* egg_work );

static void gmBoss3EggStateIdleInit( GMS_BOSS3_EGG_WORK* egg_work );
static void gmBoss3EggStateIdleUpdate( GMS_BOSS3_EGG_WORK* egg_work );
static void gmBoss3EggStateLaughInit( GMS_BOSS3_EGG_WORK* egg_work );
static void gmBoss3EggStateLaughUpdate( GMS_BOSS3_EGG_WORK* egg_work );
static void gmBoss3EggStateDamageInit( GMS_BOSS3_EGG_WORK* egg_work );
static void gmBoss3EggStateDamageUpdate( GMS_BOSS3_EGG_WORK* egg_work );
static void gmBoss3EggStateEscapeInit( GMS_BOSS3_EGG_WORK* egg_work );
static void gmBoss3EggStateEscapeUpdate( GMS_BOSS3_EGG_WORK* egg_work );

static void gmBoss3EggmanMainFuncWaitSetup( OBS_OBJECT_WORK* obj_work );
static void gmBoss3EggmanMainFunc( OBS_OBJECT_WORK* obj_work );


// =======================================================================
//ƒGƒtƒFƒNƒg
// =======================================================================
static void gmBoss3EffDamageInit( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3EffBombsInit( 
						 GMS_BOSS3_EFF_BOMB_WORK* bomb_work,
						 OBS_OBJECT_WORK* parent_obj,
						 fx32 pos_x,
						 fx32 pos_y,
						 fx32 width,
						 fx32 height,
						 Uint32 interval_min,
						 Uint32 interval_max );
static void gmBoss3EffBombsUpdate( GMS_BOSS3_EFF_BOMB_WORK* bomb_work );

static void gmBoss3EffAfterburnerRequestCreate( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3EffAfterburnerRequestDelete( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3EffAfterburnerInit( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3EffAfterburnerMainFunc( OBS_OBJECT_WORK* obj_work );

static void gmBoss3EffAfterburnerSmokeInit( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3EffAfterburnerSmokeMainFunc( OBS_OBJECT_WORK* obj_work );

static void gmBoss3EffBodySmokeInit( GMS_BOSS3_BODY_WORK* body_work );
static void gmBoss3EffBodySmokeMainFunc( OBS_OBJECT_WORK* obj_work );

static void gmBoss3EffSweatInit( GMS_BOSS3_EGG_WORK* egg_work );
static void gmBoss3EffSweatMainFunc( OBS_OBJECT_WORK* obj_work );

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

static OBS_ACTION3D_NN_WORK* gm_boss3_obj_3d_list = NULL;	//ƒIƒuƒWƒFƒNƒgƒŠƒXƒg

//ƒAƒNƒVƒ‡ƒ“î•ñ
static const GMS_BOSS3_PART_ACT_INFO gm_boss3_act_info_tbl[GMD_BOSS3_ACT_ID_MAX][GMD_BOSS3_PART_IDX_MAX] = {
		//MTN_ID									IS_MAINTAIN	IS_REPEAT	MTN_SPD	IS_BLEND	BLEND_SPD

	//GMD_BOSS3_ACT_ID_START
	{
		{IDB_BOSS03_BODY_MTN_B03_1_STA_01B_ZNM,		FALSE,		TRUE,		1.f,	FALSE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS03_EGG_MTN_B03_1_STA_01E_ZNM,		FALSE,		TRUE,		1.f,	FALSE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS3_ACT_ID_MOVE
	{
		{IDB_BOSS03_BODY_MTN_B03_1_ATT01_01B_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS03_EGG_MTN_B03_1_ATT01_01E_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS3_ACT_ID_PRE_BATTLE
	{
		{IDB_BOSS03_BODY_MTN_B03_2_STA_01B_ZNM,		FALSE,		TRUE,		1.f,	FALSE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS03_EGG_MTN_B03_2_STA_01E_ZNM,		FALSE,		TRUE,		1.f,	FALSE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS3_ACT_ID_SEARCH_L
	{
		{IDB_BOSS03_BODY_MTN_B03_2_LOOK_01B_ZNM,	FALSE,		TRUE,		1.f,	FALSE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS03_EGG_MTN_B03_2_LOOK_01E_ZNM,		FALSE,		TRUE,		1.f,	FALSE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS3_ACT_ID_SEARCH_R
	{
		{IDB_BOSS03_BODY_MTN_B03_2_LOOK_02B_ZNM,	FALSE,		TRUE,		1.f,	FALSE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS03_EGG_MTN_B03_2_LOOK_02E_ZNM,		FALSE,		TRUE,		1.f,	FALSE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS3_ACT_ID_SIGN
	{
		{IDB_BOSS03_BODY_MTN_B03_2_SIGN_01B_ZNM,	FALSE,		FALSE,		1.f,	FALSE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS03_EGG_MTN_B03_2_SIGN_01E_ZNM,		FALSE,		FALSE,		1.f,	FALSE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS3_ACT_ID_LAUGH_L
	{
		{IDB_BOSS03_BODY_MTN_B03_1_ATT01_01B_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS03_EGG_MTN_B03_2_ATT01_01E_ZNM,		FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// EGG
	},

	//GMD_BOSS3_ACT_ID_LAUGH_R
	{
		{IDB_BOSS03_BODY_MTN_B03_1_ATT01_01B_ZNM,	FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS03_EGG_MTN_B03_2_ATT01_02E_ZNM,		FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// EGG
	},
	
	//GMD_BOSS3_ACT_ID_ESCAPE
	{
		{IDB_BOSS03_BODY_MTN_B03_DMG02_01B_ZNM,		FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS03_EGG_MTN_B03_DMG02_01E_ZNM,		FALSE,		TRUE,		1.f,	TRUE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// EGG
	},
};

//ƒGƒbƒOƒ}ƒ“—pƒAƒNƒVƒ‡ƒ“î•ñ
static const GMS_BOSS3_PART_ACT_INFO gm_boss3_egg_act_info_tbl[GMD_BOSS3_EGG_ACT_ID_MAX] = {
	//MTN_ID									IS_MAINTAIN	IS_REPEAT	MTN_SPD	IS_BLEND	BLEND_SPD

	//GMD_BOSS3_EGG_ACT_ID_LAUGH_L
	{IDB_BOSS03_EGG_MTN_B03_1_STA_01E_ZNM,		FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// EGG

	//GMD_BOSS3_EGG_ACT_ID_LAUGH_R
	{IDB_BOSS03_EGG_MTN_B03_1_STA_02E_ZNM,		FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// EGG

	//GMD_BOSS3_EGG_ACT_ID_DAMAGE
	{IDB_BOSS03_EGG_MTN_B03_DMG01_01E_ZNM,		FALSE,		FALSE,		1.f,	TRUE,		GMD_BOSS3_BLEND_SPD,	FALSE},		// EGG

};

//‰Šú‰»ŠÖ”ƒe[ƒuƒ‹
const static GMF_BOSS3_BODY_STATE_FUNC gm_boss3_body_state_func_tbl_enter[GMD_BOSS3_BODY_STATE_MAX]	= {
	NULL,
	gmBoss3BodyStateStartEnter,

	gmBoss3BodyStateChaseMoveEnter,

	gmBoss3BodyStatePreBattleEnter,
	gmBoss3BodyStateBattleEnter,

	gmBoss3BodyStateDefeatEnter,
	gmBoss3BodyStateEscapeEnter,
};

//I—¹ŠÖ”ƒe[ƒuƒ‹
const static GMF_BOSS3_BODY_STATE_FUNC gm_boss3_body_state_func_tbl_leave[GMD_BOSS3_BODY_STATE_MAX]	= {
	NULL,
	gmBoss3BodyStateStartLeave,

	gmBoss3BodyStateChaseMoveLeave,

	gmBoss3BodyStatePreBattleLeave,
	gmBoss3BodyStateBattleLeave,

	gmBoss3BodyStateDefeatLeave,
	gmBoss3BodyStateEscapeLeave,
};

//ƒm[ƒhƒCƒ“ƒfƒNƒXƒŠƒXƒg
static const s32 g_boss3_node_index_list[GMD_BOSS3_BODY_SNM_INDEX_MAX] = {
	2,	//–{‘ÌÚ‘±iƒGƒbƒOƒ}ƒ“AƒAƒtƒ^ƒo[ƒij
};

//ƒoƒgƒ‹ƒpƒ^[ƒ“•ÊˆÚ“®—Ê
static const fx32 g_gm_boss3_battle_move_x[GMD_GMK_BOSS3_PILLAR_PATTERN_NUM][GMD_BOSS3_BATTLE_MOVE_NUM] = {
	{ 0,	0},
	{ 0,	0},
	{ 0,	3*40},
	{-3*40,	0},
	{-3*40,	0},
	{ 3*40,	0},
	{-3*40,	0},
};

//‘Ì—Í•Êƒoƒgƒ‹ƒpƒ^[ƒ“‘I‘ğ—¦
static const s32 g_gm_boss3_battle_pattern_per[GMD_BOSS3_MGR_LIFE][GMD_GMK_BOSS3_PILLAR_PATTERN_NUM] = {
	{10, 10, 20, 10, 20, 15, 15},
	{10, 10, 20, 10, 20, 15, 15},
	{10, 10, 30, 10, 30,  5,  5},
	{10, 10, 30, 10, 30,  5,  5},
	{30, 30, 10, 20, 10,  0,  0},
	{30, 30, 10, 20, 10,  0,  0},
	{40, 40,  0, 20,  0,  0,  0},
	{40, 40,  0, 20,  0,  0,  0},
};


//----- Global Functions ----------------------------------------------------
// =======================================================================
// GmBoss3Build
/*!
 *	ƒ{ƒX3\’z
 */
// =======================================================================
void GmBoss3Build(void)
{
	void* amb_data = GmBoss3GetGameDatEnemyArc();

	AMS_AMB_HEADER* amb_model = (AMS_AMB_HEADER*)ObjDataLoadAmbIndex(
			NULL, 
			IDB_BOSS03_BOSS03_MDL_AMB, 
			amb_data );
	AMS_AMB_HEADER* amb_texture = (AMS_AMB_HEADER*)ObjDataLoadAmbIndex(
			NULL, 
			IDB_BOSS03_BOSS03_TEX_AMB,
			amb_data );
	
	//ƒ‚ƒfƒ‹
	gm_boss3_obj_3d_list = GmGameDBuildRegBuildModel(
			amb_model, 
			amb_texture,
			NND_DRAWOBJ_SHADER_USER_PROFILE_TOON );
	
	//ƒ‚[ƒVƒ‡ƒ“
	ObjDataLoadAmbIndex(
			ObjDataGet(GMD_DWORK_NO_BOSS_03_BODY_MTN),
			IDB_BOSS03_BOSS03_BODY_MTN_AMB,
			amb_data );
	ObjDataLoadAmbIndex(
			ObjDataGet(GMD_DWORK_NO_BOSS_03_EGG_MTN),
			IDB_BOSS03_BOSS03_EGG_MTN_AMB, 
			amb_data );
}

// =======================================================================
// GmBoss3Flush
/*!
 *	ƒ{ƒX3‰ğ•ú
 */
// =======================================================================
void GmBoss3Flush( void )
{
	//ƒGƒtƒFƒNƒg‰ğ•ú
	GmEfctBossFlushSingleDataInit();	//ƒ{ƒXê—pEFFƒtƒ‰ƒbƒVƒ…ŠJn
	
	//ƒ‚[ƒVƒ‡ƒ“
	ObjDataRelease( ObjDataGet(GMD_DWORK_NO_BOSS_03_EGG_MTN) );
	ObjDataRelease( ObjDataGet(GMD_DWORK_NO_BOSS_03_BODY_MTN) );

	//ƒ‚ƒfƒ‹‰ğ•ú
	void* amb_data = GmBoss3GetGameDatEnemyArc();
	AMS_AMB_HEADER* amb_model = (AMS_AMB_HEADER*)ObjDataLoadAmbIndex(
			NULL, 
			IDB_BOSS03_BOSS03_MDL_AMB, 
			amb_data );	
	GmGameDBuildRegFlushModel( gm_boss3_obj_3d_list, amb_model->file_num );	
	gm_boss3_obj_3d_list = NULL;
}

// =======================================================================
// GmBoss3GetGameDatEnemyArc
/*!
 *	ƒA[ƒJƒCƒuæ“¾
 *
 *	@return ƒA[ƒJƒCƒu
 */
// =======================================================================
void* GmBoss3GetGameDatEnemyArc( void )
{
	return g_gm_gamedat_enemy_arc;
}

// ==========================================================================
// GmBoss3Init
/*!
 *	ƒ{ƒX3ŠÇ—‰Šú‰»ŠÖ”
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmBoss3Init(
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
	GMS_BOSS3_MGR_WORK* mgr_work = (GMS_BOSS3_MGR_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec,
			pos_x, 
			pos_y,
			sizeof(GMS_BOSS3_MGR_WORK),
			"BOSS3_MGR");
	amAssert( mgr_work );

	OBS_OBJECT_WORK* mgr_obj_work = &mgr_work->ene_3d.ene_com.obj_work;
	
	//ƒ[ƒN
	mgr_obj_work->flag |= OBD_OBJECT_NOCLIP;
	mgr_obj_work->disp_flag |= OBD_DISP_NODISP;
	mgr_obj_work->move_flag	|= (OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);

	//ƒz[ƒ~ƒ“ƒO–³Œø
	mgr_work->ene_3d.ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;
	
	//ˆ—ŠÖ”
	mgr_obj_work->ppFunc = gmBoss3MgrMainFuncWaitLoad;
	
	//ƒ‰ƒCƒt
	if ( GmBsCmnIsFinalZoneType(mgr_obj_work) ){
		mgr_work->life = GMD_BOSS3_MGR_LIFE_FINAL;
	}
	else{
		mgr_work->life = GMD_BOSS3_MGR_LIFE;
	}
	
#ifdef MPPDEBUG_INFINITE_LIFE
	mgr_work->life = 1;
#endif
	
	return mgr_obj_work;
}

// ==========================================================================
// GmBoss3BodyInit
/*!
 *	ƒ{ƒX3–{‘Ì‰Šú‰»ŠÖ”
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmBoss3BodyInit(
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
	GMS_BOSS3_BODY_WORK* body_work = (GMS_BOSS3_BODY_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec,
			pos_x, 
			pos_y,
			sizeof(GMS_BOSS3_BODY_WORK),
			"BOSS3_BODY");
	amAssert( body_work );

	GMS_ENEMY_3D_WORK* ene_3d = &body_work->ene_3d;
	OBS_OBJECT_WORK* obj_work = &ene_3d->ene_com.obj_work;

	//-----------------------------------------
	//ƒ‚ƒfƒ‹Aƒ‚[ƒVƒ‡ƒ“
	//-----------------------------------------
	//ƒ‚ƒfƒ‹
	ObjObjectCopyAction3dNNModel(
			obj_work,
			&gm_boss3_obj_3d_list[IDB_BOSS03_MDL_B03_BODY_ZNO],
			&ene_3d->obj_3d);
	
	//ƒ‚[ƒVƒ‡ƒ“
	ObjObjectAction3dNNMotionLoad(
			obj_work,
			0,
			TRUE,
			ObjDataGet(GMD_DWORK_NO_BOSS_03_BODY_MTN),
			NULL,
			0,
			NULL);

	//-----------------------------------------
	//‹éŒ`
	//-----------------------------------------
	//ƒ‰ƒCƒt
	ene_3d->ene_com.vit = 1;

	//–{‘Ì
	ObjRectWorkSet(
			&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY],
			-24, -24, 24, 24);
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
			-28, -28, 28, 24 );
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].ppDef	= gmBoss3BodyDefFunc;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag |= OBD_RECT_OUT;

	//‹éŒ`Ø‚è‘Ö‚¦
	gmBoss3BodySetRectNormal( body_work );

	//-----------------------------------------
	//ƒ[ƒN
	//-----------------------------------------
	//À•W
	obj_work->pos.z	= GMD_OBJ_DEFAULT_POS_Z_A_FRONT;

	//ƒtƒ‰ƒO
	obj_work->flag |= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag |= OBD_DISP_HFLIP | OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP;
	obj_work->move_flag &= ~OBD_MOVE_FALL;
#if _IPHONE
	obj_work->move_flag |= OBD_MOVE_NOSPD | OBD_MOVE_NOSPDM | OBD_MOVE_JUMP | OBD_MOVE_NOCOLFIELD | OBD_MOVE_NOCOLOBJ;
	body_work->is_move = FALSE; // ˆÚ“®‚µ‚È‚¢
#else
	obj_work->move_flag |= OBD_MOVE_NOSPD | OBD_MOVE_NOSPDM | OBD_MOVE_JUMP | OBD_MOVE_NOCOLFIELD | OBD_MOVE_NOCOLOBJ | OBD_MOVE_NOMOVE;
#endif // _IPHONE
	
	//ƒuƒŒƒ“ƒh‘¬“x
	obj_work->obj_3d->blend_spd	= GMD_BOSS3_BLEND_SPD;
	
	//ƒgƒD[ƒ“
	ObjDrawObjectSetToon( obj_work );
	
	//ƒ†[ƒU•`‰æƒXƒe[ƒg
	obj_work->disp_flag |= OBD_DISP_DRAWSTATE;
	
	//ˆ—
	obj_work->ppFunc = gmBoss3BodyMainFuncWaitSetup;
	obj_work->ppOut = gmBoss3BodyOutFunc;
	obj_work->ppMove = gmBoss3BodyChaseMoveFunc;	
	
	//‰½‚à‚µ‚È‚¢ó‘Ô‚Ö
	gmBoss3BodyChangeState( body_work, GMD_BOSS3_BODY_STATE_NO_OPERATION );

#if _IPHONE
	// ê—pƒ‰ƒCƒgİ’è
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}

// ==========================================================================
// GmBoss3EggInit
/*!
 *	ƒ{ƒX3ƒGƒbƒOƒ}ƒ“‰Šú‰»ŠÖ”
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmBoss3EggInit(
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
	GMS_BOSS3_EGG_WORK* eggman_work = (GMS_BOSS3_EGG_WORK*)GMM_ENEMY_CREATE_WORK(
			eve_rec,
			pos_x, 
			pos_y,
			sizeof(GMS_BOSS3_EGG_WORK),
			"BOSS3_EGG");
	amAssert( eggman_work );

	GMS_ENEMY_3D_WORK* ene_3d = &eggman_work->ene_3d;
	OBS_OBJECT_WORK* obj_work = &ene_3d->ene_com.obj_work;

	//-----------------------------------------
	//ƒ‚ƒfƒ‹Aƒ‚[ƒVƒ‡ƒ“
	//-----------------------------------------
	//ƒ‚ƒfƒ‹
	ObjObjectCopyAction3dNNModel(
			obj_work,
			&gm_boss3_obj_3d_list[IDB_BOSS03_MDL_EGGMAN_ZNO],
			&ene_3d->obj_3d);
	
	//ƒ‚[ƒVƒ‡ƒ“
	ObjObjectAction3dNNMotionLoad(
			obj_work,
			0,
			TRUE,
			ObjDataGet(GMD_DWORK_NO_BOSS_03_EGG_MTN),
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
	obj_work->obj_3d->blend_spd	= GMD_BOSS3_BLEND_SPD;
	
	//ƒgƒD[ƒ“
	ObjDrawObjectSetToon( obj_work );
	
	//ƒ†[ƒU•`‰æƒXƒe[ƒg
	obj_work->disp_flag |= OBD_DISP_DRAWSTATE;

	//ƒz[ƒ~ƒ“ƒO–³Œø
	ene_3d->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;
	
	//ˆ—
	obj_work->ppFunc = gmBoss3EggmanMainFuncWaitSetup;
	
#if _IPHONE
	// ê—pƒ‰ƒCƒgİ’è
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}

//----- Local Functions -----------------------------------------------------

// ==========================================================================
// gmBoss3ChangeTextureBurnt
/*!
 *	ƒeƒNƒXƒ`ƒƒ‚ğ•‚±‚°‚É•ÏX
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@note uvƒXƒNƒ[ƒ‹iu‚ğ0.5ƒIƒtƒZƒbƒgj
 */
// ==========================================================================
void gmBoss3ChangeTextureBurnt( OBS_OBJECT_WORK *obj_work )
{
	amAssert( obj_work );
	amAssert( obj_work->obj_3d );
	amAssert( obj_work->disp_flag & OBD_DISP_DRAWSTATE );
	
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_TEXOFFSET;
	obj_work->obj_3d->draw_state.texoffset[0].mode	= NNE_MATCTRLMODE_ADD;
	obj_work->obj_3d->draw_state.texoffset[0].u	= 0.5f;
}

// =======================================================================
// gmBoss3ExitFunc
/*!
 *	I—¹ŠÖ”
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3ExitFunc( MTS_TASK_TCB *tcb )
{
	//ŠÇ—‚©‚ç‰ğ•ú
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)mtTaskGetTcbWork( tcb );
	gmBoss3MgrDeleteObject( obj_work );

	//ƒGƒlƒ~[‹¤’ÊI—¹ˆ—
	GmEnemyDefaultExit( tcb );
}

// ==========================================================================
//ŠÇ—
// ==========================================================================

// =======================================================================
// gmBoss3MgrCheckSetupComplete
/*!
 *	“Ç‚İ‚İŠ®—¹Šm”F
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@retval TRUE “Ç‚İ‚İŠ®—¹
 *	@retval FALSE “Ç‚İ‚İ‘Ò‚¿
 */
// =======================================================================
BOOL gmBoss3MgrCheckSetupComplete( GMS_BOSS3_MGR_WORK* mgr_work )
{
	amAssert( mgr_work );

	if ( mgr_work->flag & GMD_BOSS3_MGR_FLAG_SETUP_COMPLETE ){
		return TRUE;
	}
	return FALSE;
}

// =======================================================================
// gmBoss3MgrGetMgrWork
/*!
 *	ƒ}ƒl[ƒWƒƒƒ[ƒN‚ğæ“¾
 *
 *	@param obj_work_parts	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
GMS_BOSS3_MGR_WORK* gmBoss3MgrGetMgrWork( OBS_OBJECT_WORK* obj_work_parts )
{
	amAssert( obj_work_parts );
	amAssert( obj_work_parts->user_work );

	return (GMS_BOSS3_MGR_WORK*)obj_work_parts->user_work;
}

// =======================================================================
// gmBoss3MgrAddObject
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
void gmBoss3MgrAddObject( GMS_BOSS3_MGR_WORK* mgr_work, OBS_OBJECT_WORK* obj_work_parts )
{
	amAssert( mgr_work );
	amAssert( obj_work_parts );

	++mgr_work->obj_create_count;
	obj_work_parts->user_work = (u32)mgr_work;
}

// =======================================================================
// gmBoss3MgrDeleteObject
/*!
 *	ƒp[ƒc“o˜^íœ
 *
 *	@note ƒIƒuƒWƒFƒNƒgíœ‚ÉAŠÇ—ƒ[ƒN‚Ì¶¬”‚ğƒfƒNƒŠƒƒ“ƒg‚·‚é
 *
 *	@param obj_work_parts	[io] ’Ç‰Á‚·‚éƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3MgrDeleteObject( OBS_OBJECT_WORK* obj_work_parts )
{
	amAssert( obj_work_parts );

	GMS_BOSS3_MGR_WORK* mgr_work = gmBoss3MgrGetMgrWork( obj_work_parts );
	amAssert( mgr_work );
	amAssert( mgr_work->obj_create_count > 0 );

	--mgr_work->obj_create_count;
	obj_work_parts->user_work = 0;
}

// =======================================================================
// gmBoss3MgrMainFuncWaitLoad
/*!
 *	ƒƒCƒ“ˆ—i“Ç‚İ‚İ‘Ò‚¿j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3MgrMainFuncWaitLoad( OBS_OBJECT_WORK* obj_work )
{
	amAssert( obj_work );

	//ƒtƒ@ƒCƒiƒ‹ƒ][ƒ“‚Ìê‡Aƒf[ƒ^“Ç‚İ‚İŠ®—¹”»’è
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		if ( !GmMainDatLoadBossBattleLoadCheck(GMD_GAMEDAT_LOAD_BOSS_TYPE_3) ){
			return;
		}
	}

	GMS_BOSS3_MGR_WORK* mgr_work = (GMS_BOSS3_MGR_WORK*)obj_work;

	//-----------------------------------------
	//–{‘Ì
	//-----------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_BOSS3_BODY_WORK* body_work = (GMS_BOSS3_BODY_WORK*)GmEventMgrLocalEventBirth(
			GMD_EVENT_ID_BOSS3_BODY,
			obj_work->pos.x, obj_work->pos.y,
			0,
			0,0,0,0,
			0 );
	amAssert( body_work );

	OBS_OBJECT_WORK* body_obj_work = &body_work->ene_3d.ene_com.obj_work;

	//e
	body_obj_work->parent_obj = obj_work;

	//–{‘Ìƒp[ƒc“o˜^
	body_work->parts_objs[GMD_BOSS3_PART_IDX_BODY] = body_obj_work;

	//ŠÇ—‚É“o˜^
	mgr_work->body_work = body_work;
	gmBoss3MgrAddObject( mgr_work, body_obj_work );

	//I—¹ˆ—
	mtTaskChangeTcbDestructor( body_obj_work->tcb, gmBoss3BodyExit );

	
	//-----------------------------------------
	//ƒGƒbƒOƒ}ƒ“
	//-----------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_BOSS3_EGG_WORK* eggman_work = (GMS_BOSS3_EGG_WORK*)GmEventMgrLocalEventBirth(
			GMD_EVENT_ID_BOSS3_EGG,
			obj_work->pos.x, obj_work->pos.y,
			0,
			0,0,0,0,
			0 );
	amAssert( eggman_work );

	OBS_OBJECT_WORK* eggman_obj_work = &eggman_work->ene_3d.ene_com.obj_work;

	//e
	eggman_obj_work->parent_obj = body_obj_work;	

	//ŠÇ—
	gmBoss3MgrAddObject( mgr_work, eggman_obj_work );

	//I—¹ˆ—
	mtTaskChangeTcbDestructor( eggman_obj_work->tcb, gmBoss3ExitFunc );

	//–{‘Ìƒp[ƒc“o˜^
	body_work->parts_objs[GMD_BOSS3_PART_IDX_EGG] = eggman_obj_work;

	obj_work->ppFunc = gmBoss3MgrMainFuncWaitSetup;
}

// =======================================================================
// gmBoss3MgrMainFuncWaitSetup
/*!
 *	ƒƒCƒ“ˆ—i¶¬‘Ò‚¿j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3MgrMainFuncWaitSetup( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS3_MGR_WORK* mgr_work = (GMS_BOSS3_MGR_WORK*)obj_work;
	amAssert( mgr_work );
	GMS_BOSS3_BODY_WORK* body_work = mgr_work->body_work;
	amAssert( body_work );

	for ( s32 i = 0; GMD_BOSS3_PART_IDX_MAX > i; ++i ){
		if ( !body_work->parts_objs[i] ) {
			return;
		}
	}

	//“Ç‚İ‚İI—¹
	mgr_work->flag |= GMD_BOSS3_MGR_FLAG_SETUP_COMPLETE;
	obj_work->ppFunc = gmBoss3MgrMainFunc;
}

// =======================================================================
// gmBoss3MgrMainFunc
/*!
 *	ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3MgrMainFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS3_MGR_WORK* mgr_work = (GMS_BOSS3_MGR_WORK*)obj_work;
	amAssert( mgr_work );

	//ƒIƒuƒWƒFƒNƒgíœ
	if ( mgr_work->flag & GMD_BOSS3_MGR_FLAG_CLEAR_BOSS ){
		OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ(mgr_work->body_work);
		amAssert( body_obj_work );
		
		//íœ—v‹
		body_obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		mgr_work->body_work	= NULL;

		//ƒƒCƒ“ˆ—I—¹
		obj_work->ppFunc = gmBoss3MgrMainFuncWaitRelease;
	}
}

// =======================================================================
// gmBoss3MgrMainFuncWaitRelease
/*!
 *	ƒƒCƒ“ˆ—i‰ğ•ú‘Ò‚¿j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3MgrMainFuncWaitRelease( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS3_MGR_WORK* mgr_work = (GMS_BOSS3_MGR_WORK*)obj_work;
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
		GmGameDatReleaseBossBattleStart( GMD_GAMEDAT_LOAD_BOSS_TYPE_3 );

		//ƒXƒNƒ[ƒ‹ƒƒbƒN‰ğœ
		GmGmkCamScrLimitRelease(
				GMD_GMK_SCR_LMT_RELEASE_TOP | GMD_GMK_SCR_LMT_RELEASE_RIGHT | GMD_GMK_SCR_LMT_RELEASE_BOTTOM );

		//’Œ‚É“®ì—v‹
		OBS_OBJECT_WORK* obj_work_pillar = gmBoss3BodyBattleSearchPillar();
		if ( obj_work_pillar ){
			GmGmkBoss3PillarWallChangeModeReturn( obj_work_pillar );
		}
	}
	obj_work->ppFunc = NULL;
}

// ==========================================================================
//–{‘Ì
// ==========================================================================

// =======================================================================
// gmBoss3BodyExit
/*!
 *	I—¹ˆ—
 *
 *	@param tcb	[io] TCB
 *
 *	@note ƒm[ƒhƒ}ƒgƒŠƒNƒXæ“¾ŠÖ˜A‚Ì‰ğ•ú
 */
// =======================================================================
void gmBoss3BodyExit( MTS_TASK_TCB* tcb )
{
	GMS_BOSS3_BODY_WORK* body_work = (GMS_BOSS3_BODY_WORK*)mtTaskGetTcbWork( tcb );
	amAssert( body_work );
	OBS_OBJECT_WORK* obj_work = &body_work->ene_3d.ene_com.obj_work;

	//ƒ{ƒXƒ‚[ƒVƒ‡ƒ“ƒR[ƒ‹ƒoƒbƒNƒVƒXƒeƒ€‚ğ‰ğ•ú
	GmBsCmnClearBossMotionCBSystem( obj_work );

	//ƒm[ƒhƒ}ƒgƒŠƒNƒXæ“¾ˆ—ƒ[ƒN‚ğ‰ğ•ú
	GmBsCmnDeleteSNMWork( &body_work->snm_work );

	//ƒm[ƒhƒ}ƒgƒŠƒNƒX‘€ìˆ—ƒR[ƒ‹ƒoƒbƒN‚ğ‰ğ•ú
	GmBsCmnClearCNMCb( obj_work );

	//ƒ{ƒX3—pI—¹ˆ—
	gmBoss3ExitFunc( tcb );
}

// =======================================================================
// gmBoss3BodyReactionPlayer
/*!
 *	ƒvƒŒƒCƒ„‚ğ‚Í‚¶‚­
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyReactionPlayer( OBS_OBJECT_WORK* obj_work_player, const OBS_OBJECT_WORK* obj_work_body )
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


	//‚Í‚¶‚­—Ê
	fx32 ref_x = GMD_BOSS3_BODY_DEF_PLAYER_MOVE_X_NORMAL;
	fx32 ref_y = GMD_BOSS3_BODY_DEF_PLAYER_MOVE_Y_NORMAL;

	//ƒz[ƒ~ƒ“ƒOUŒ‚‚Ì‚Æ‚«
	if (player_work->seq_state == GME_PLY_SEQ_STATE_HOMING_REF) {
		ref_x = GMD_BOSS3_BODY_DEF_PLAYER_MOVE_X_HOMING;
		ref_y = GMD_BOSS3_BODY_DEF_PLAYER_MOVE_Y_HOMING;
	}

	//ˆÚ“®’l
	obj_work_player->spd_m = 0;
	if ( obj_work_player->move.x >= 0 ){
		obj_work_player->spd.x = -ref_x;
	}
	else {
		obj_work_player->spd.x = ref_x;
	}

	if ( obj_work_player->pos.y <= obj_work_body->pos.y ){
		obj_work_player->spd.y = -ref_y;
	}
	else {
		obj_work_player->spd.y = ref_y;
	}

	//ƒWƒƒƒ“ƒv’†ˆÚ“®‚³‚¹‚È‚¢ŠÔi˜A‘±HIT‚ğ–h‚®‚½‚ßj
	GmPlySeqSetNoJumpMoveTime( player_work, GMD_BOSS3_BODY_DEF_PLAYER_NO_JUMP_MOVE_TIME );
}


// =======================================================================
// gmBoss3BodySetRectNormal
/*!
 *	‹éŒ`İ’èi’Êíj
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒg‚Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodySetRectNormal( GMS_BOSS3_BODY_WORK* body_work )
{
	//UŒ‚‹éŒ`”ÍˆÍ‚ğ’Êí‚É
	ObjRectWorkSet(
			&body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK],
			-8, -8, 8, 8 );

	//‚­‚ç‚¢‹éŒ`—LŒø
	body_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag |= OBD_RECT_ENABLE;
}

// =======================================================================
// gmBoss3BodySetActionAllParts
/*!
 *	ƒAƒNƒVƒ‡ƒ“İ’èi–{‘Ìƒp[ƒc‘S‚Äj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param action_id	[in] ƒAƒNƒVƒ‡ƒ“ID
 *	@param force_change	[in] ‹­§•ÏXƒtƒ‰ƒO
 */
// =======================================================================
void gmBoss3BodySetActionAllParts(
								  GMS_BOSS3_BODY_WORK* body_work,
								  GME_BOSS3_ACT_ID action_id,
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
	for ( s32 i = 0; GMD_BOSS3_PART_IDX_MAX > i; ++i ){

		OBS_OBJECT_WORK* part_obj_work = body_work->parts_objs[i];
		if ( !part_obj_work ){
			continue;
		}

		//ƒAƒNƒVƒ‡ƒ“î•ñ
		const GMS_BOSS3_PART_ACT_INFO* action_info = &gm_boss3_act_info_tbl[action_id][i];

		//ƒGƒbƒOƒ}ƒ“ê—pˆ—
		if ( i == GMD_BOSS3_PART_IDX_EGG ){
			GMS_BOSS3_EGG_WORK* egg_work = (GMS_BOSS3_EGG_WORK*)part_obj_work;

			//ƒGƒbƒOƒ}ƒ“ê—pƒAƒNƒVƒ‡ƒ“’†‚Ìê‡A•ÏX‚µ‚È‚¢
			if ( egg_work->flag & GMD_BOSS3_EGG_FLAG_EGG_ACT_ACTIVE ){
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
// gmBoss3BodyOutFunc
/*!
 *	•`‰æŠÖ”
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyOutFunc( OBS_OBJECT_WORK* obj_work )
{
	ObjDrawActionSummary( obj_work );
}

// =======================================================================
// gmBoss3BodyChaseMoveFunc
/*!
 *	ˆÚ“®ŠÖ”
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyChaseMoveFunc( OBS_OBJECT_WORK* obj_work )
{
	fx32 speed_x = obj_work->spd.x;
	fx32 speed_y = obj_work->spd.y;

	//ƒXƒs[ƒh’²®
	GMS_BOSS3_BODY_WORK* body_work = (GMS_BOSS3_BODY_WORK*)obj_work;
	gmBoss3BodyChaseAdjustMoveSpeed( body_work );

	//ˆÚ“®
	ObjObjectMove( obj_work );

	//ƒXƒs[ƒh–ß‚·
	obj_work->spd.x = speed_x;
	obj_work->spd.y = speed_y;
}

// =======================================================================
// gmBoss3BodyDefFunc
/*!
 *	‚­‚ç‚¢ŠÖ”
 *
 *	@param own_rect	[io] ©g‚Ì‹éŒ`
 *	@param target_rect	[io] ‘Šè‚Ì‹éŒ`
 */
// =======================================================================
void gmBoss3BodyDefFunc( OBS_RECT_WORK* own_rect, OBS_RECT_WORK* target_rect )
{
	OBS_OBJECT_WORK* player_obj_work = target_rect->parent_obj;
	amAssert( player_obj_work );

	OBS_OBJECT_WORK* body_obj_work = own_rect->parent_obj;
	GMS_BOSS3_BODY_WORK* body_work = (GMS_BOSS3_BODY_WORK*)body_obj_work;
	amAssert( body_work );

	//ƒvƒŒƒCƒ„‚¶‚á‚È‚¢
	if ( !player_obj_work || GMD_OBJTYPE_PLAYER != player_obj_work->obj_type ){
		return;
	}

	//ƒvƒŒƒCƒ„‚ğ’e‚­
	gmBoss3BodyReactionPlayer( player_obj_work, body_obj_work );

	//ƒqƒbƒg–³ŒøŠÔ
	gmBoss3BodySetNoHitTime( body_work, GMD_BOSS3_BODY_DEF_NO_HIT_TIME );

	//ƒ_ƒ[ƒW
	gmBoss3BodyDamage( body_work );
}

// =======================================================================
// gmBoss3BodySetNoHitTime
/*!
 *	ƒqƒbƒg–³ŒøŠÔ‚ğİ’è
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param time			[in] ƒqƒbƒg–³ŒøŠÔ
 *
 *	@note gmBoss3BodyUpdateNoHitTimeŠÖ”‚ÅXVA‰ğœ‚ğs‚¤
 */
// =======================================================================
void gmBoss3BodySetNoHitTime( GMS_BOSS3_BODY_WORK *body_work, u32 time )
{
	amAssert( body_work );

	GMS_ENEMY_COM_WORK* ene_com = &body_work->ene_3d.ene_com;

	//ƒqƒbƒg–³Œø‚ğİ’è
	body_work->counter_no_hit = time;
	ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag |= OBD_RECT_NOHIT;
}

// =======================================================================
// gmBoss3BodyUpdateNoHitTime
/*!
 *	ƒqƒbƒg–³ŒøŠÔ‚ğXV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyUpdateNoHitTime( GMS_BOSS3_BODY_WORK* body_work )
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
// gmBoss3BodySetInvincibleTime
/*!
 *	–³“GŠÔ‚ğİ’è
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param time			[in] ƒqƒbƒg–³ŒøŠÔ
 *
 *	@note gmBoss3BodyUpdateNoHitTimeŠÖ”‚ÅXVA‰ğœ‚ğs‚¤
 */
// =======================================================================
void gmBoss3BodySetInvincibleTime( GMS_BOSS3_BODY_WORK *body_work, u32 time )
{
	amAssert( body_work );

	body_work->counter_invincible = time;
	body_work->flag |= GMD_BOSS3_BODY_FLAG_INVINCIBLE;
}

// =======================================================================
// gmBoss3BodyUpdateInvincibleTime
/*!
 *	–³“GŠÔ‚ğXV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyUpdateInvincibleTime( GMS_BOSS3_BODY_WORK* body_work )
{
	amAssert( body_work );

	//ƒJƒEƒ“ƒgƒ_ƒEƒ“
	if ( body_work->counter_invincible > 0 ){
		--body_work->counter_invincible;
		return;
	}
	body_work->flag &= ~GMD_BOSS3_BODY_FLAG_INVINCIBLE;
}

// =======================================================================
// gmBoss3BodySetDirection
/*!
 *	Œü‚«‚ğİ’è
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param deg			[in] Šp“x
 */
// =======================================================================
void gmBoss3BodySetDirection( GMS_BOSS3_BODY_WORK* body_work, Angle16 deg )
{
	body_work->angle_current = deg;
}

// =======================================================================
// gmBoss3BodySetDirectionNormal
/*!
 *	Œü‚«‚ğİ’èi^‰¡‚æ‚è³–ÊŒü‚«j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodySetDirectionNormal( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );

	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		gmBoss3BodySetDirection( body_work, GMD_BOSS3_ANGLE_LEFT );
	}
	else {
		gmBoss3BodySetDirection( body_work, GMD_BOSS3_ANGLE_RIGHT );
	}
}

// =======================================================================
// gmBoss3BodyUpdateDirection
/*!
 *	Œü‚«XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyUpdateDirection( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );

	obj_work->dir.y	= (Uint16)body_work->angle_current;
}

// =======================================================================
// gmBoss3BodyCalcMoveXNormalFrame
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
Float gmBoss3BodyCalcMoveXNormalFrame( 
									const GMS_BOSS3_BODY_WORK* body_work,
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
// gmBoss3BodyInitMoveNormal
/*!
 *	À•WˆÚ“®‰Šú‰»
 *
 *	@param body_work		[io] –{‘Ìƒ[ƒN
 *	@param dest_pos			[in] –Ú•WÀ•W
 *	@param frame			[in] ƒtƒŒ[ƒ€”
 */
// =======================================================================
void gmBoss3BodyInitMoveNormal(
						 GMS_BOSS3_BODY_WORK* body_work, 
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
// gmBoss3BodyUpdateMoveNormal
/*!
 *	À•WˆÚ“®XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *
 *	@return c‚èƒtƒŒ[ƒ€”
 */
// =======================================================================
Float gmBoss3BodyUpdateMoveNormal( GMS_BOSS3_BODY_WORK* body_work )
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
// gmBoss3BodyInitTurn
/*!
 *	U‚èŒü‚«‰Šú‰»
 *
 *	@param body_work		[io] –{‘Ìƒ[ƒN
 *	@param dest_angle		[in] –Ú•WŠp
 *	@param frame			[in] ƒtƒŒ[ƒ€”
 *	@param flag_positive	[in] ³•ûŒü‰ñ“]ƒtƒ‰ƒO(TRUE:³ FALSE:•‰)
 */
// =======================================================================
void gmBoss3BodyInitTurn(
						 GMS_BOSS3_BODY_WORK* body_work, 
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
// gmBoss3BodyUpdateTurn
/*!
 *	U‚èŒü‚«XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *
 *	@return c‚èƒtƒŒ[ƒ€”
 */
// =======================================================================
Float gmBoss3BodyUpdateTurn( GMS_BOSS3_BODY_WORK* body_work )
{
	//İ’è‚³‚ê‚Ä‚¢‚È‚¢
	if ( body_work->turn_frame < 1.0f ){
		return 0;
	}

	Angle16 angle_current;

	body_work->turn_counter += 1.0f;

	//U‚èŒü‚«I—¹
	if ( body_work->turn_counter >= body_work->turn_frame ){
		angle_current = (Angle16)(body_work->turn_start + body_work->turn_amount);
	}
	//ƒTƒCƒ“ƒJ[ƒu‚Å‰ñ“]’†
	else{
		//ƒTƒCƒ“ƒJ[ƒu—pŠp“x‚ğZo
		Float per = body_work->turn_counter / body_work->turn_frame;
		Angle32 angle_cos = AKM_DEGtoA32( 180 * per );

		//U‚èŒü‚«Šp“x‚ğZo
		Float angle_f = body_work->turn_amount * 0.5f * (1.0f - nnCos(angle_cos));
		angle_current = (Angle16)(body_work->turn_start + angle_f);
	}

	//İ’è
	gmBoss3BodySetDirection( body_work, angle_current );
	
	return body_work->turn_frame - body_work->turn_counter;
}

// =======================================================================
// gmBoss3BodyChaseCheckTurn
/*!
 *	ƒ^[ƒ“‚·‚é‚©ƒ`ƒFƒbƒN
 *
 *	@param body_work	[in] –{‘Ìƒ[ƒN
 *
 *	@retval TRUE ƒ^[ƒ“‚·‚é
 *	@retval FALSE ƒ^[ƒ“‚µ‚È‚¢
 */
// =======================================================================
BOOL gmBoss3BodyChaseCheckTurn( const GMS_BOSS3_BODY_WORK* body_work )
{
	const OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );
	amAssert( body_obj_work );

	if ( body_obj_work->disp_flag & OBD_DISP_HFLIP ){
		if ( body_obj_work->spd.x < 0 ){
			return FALSE;
		}
	}
	else{
		if ( body_obj_work->spd.x > 0 ){
			return FALSE;
		}
	}
	return TRUE;
}

// =======================================================================
// gmBoss3BodyChaseAdjustMoveSpeed
/*!
 *	ˆÚ“®—Ê’²®
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param speed		[in] ˆÚ“®—Ê
 */
// =======================================================================
static void gmBoss3BodyChaseAdjustMoveSpeed( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );
	amAssert( body_obj_work );

	//ƒ‰ƒCƒt‚É‚æ‚é•â³
	GMS_BOSS3_MGR_WORK* mgr_work = gmBoss3MgrGetMgrWork( body_obj_work );
	amAssert( mgr_work );
	fx32 speed_adjust = FX_F32_TO_FX32(1.0f + (GMD_BOSS3_MGR_LIFE - mgr_work->life) * GMD_BOSS3_CHASE_ADJUST_SPEED);

	//ƒvƒŒƒCƒ„‚Ì•û‚ªæs‚µ‚Ä‚¢‚éê‡‚Ì•â³
	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	if ( player_work->obj_work.pos.y < body_obj_work->pos.y ){
		//‰E‚ÉˆÚ“®’†
		if ( body_obj_work->spd.x > 0 ){
			if ( body_obj_work->pos.x < player_work->obj_work.pos.x ){
				speed_adjust = FX_Mul(speed_adjust,FX32_ONE*2);
			}
		}
		//¶‚ÉˆÚ“®’†
		if ( body_obj_work->spd.x < 0 ){
			if ( player_work->obj_work.pos.x < body_obj_work->pos.x ){
				speed_adjust = FX_Mul(speed_adjust,FX32_ONE*2);
			}
		}
	}

	//’²®’l”»’è
	body_obj_work->spd.x = FX_Mul( body_obj_work->spd.x, speed_adjust );
	body_obj_work->spd.y = FX_Mul( body_obj_work->spd.y, speed_adjust );
#if _IPHONE
	if (!body_work->is_move) {
		body_obj_work->spd.x = 0;
		body_obj_work->spd.y = 0;
	}
#endif // _IPHONE
}

// =======================================================================
// gmBoss3BodyBattleCalcPattern
/*!
 *	ƒoƒgƒ‹ƒpƒ^[ƒ“”Ô†Zo
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *
 *	@return ƒoƒgƒ‹ƒpƒ^[ƒ“”Ô†
 */
// =======================================================================
s32 gmBoss3BodyBattleCalcPattern( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	GMS_BOSS3_MGR_WORK* mgr_work = gmBoss3MgrGetMgrWork( obj_work );
	amAssert( mgr_work );

	s32 rand = mtMathRand() % 100;
	s32 cmp = 0;
	for ( s32 i = 0; GMD_GMK_BOSS3_PILLAR_PATTERN_NUM > i; ++i ){
		cmp += g_gm_boss3_battle_pattern_per[mgr_work->life-1][i];
		if ( rand < cmp ){
			return i;
		}
	}

	amAssert( FALSE );
	return 0; 
}

// =======================================================================
// gmBoss3BodyBattleInitMovePattern
/*!
 *	ƒoƒgƒ‹ƒpƒ^[ƒ“‚É‰ˆ‚Á‚ÄˆÚ“®À•W‚ğİ’è‚·‚é
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param pattern_no	[in] ƒoƒgƒ‹ƒpƒ^[ƒ“”Ô†
 *	@param pos_index	[in] À•W”Ô†
 *
 *	@retval TRUE:ˆÚ“®‚·‚é•K—v‚ ‚è
 *	@retval FALSE:ˆÚ“®‚·‚é•K—v‚ª‚È‚¢
 */
// =======================================================================
BOOL gmBoss3BodyBattleInitMovePattern( 
									  GMS_BOSS3_BODY_WORK* body_work,
									  s32 pattern_no,
									  s32 pos_index,
									  fx32 move_speed)
{
	const OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );
	amAssert( body_obj_work );
	amAssert( 0 <= pattern_no && pattern_no < GMD_GMK_BOSS3_PILLAR_PATTERN_NUM );
	amAssert( 0 <= pos_index && pos_index < GMD_BOSS3_BATTLE_MOVE_NUM );

	if ( g_gm_boss3_battle_move_x[pattern_no][pos_index] == 0 ){
		return FALSE;
	}

	VecFx32 dest_pos = {
		body_obj_work->pos.x + g_gm_boss3_battle_move_x[pattern_no][pos_index]*FX32_ONE,
		body_obj_work->pos.y,
		body_obj_work->pos.z
	};

	//ˆÚ“®ƒtƒŒ[ƒ€”
	Float frame = gmBoss3BodyCalcMoveXNormalFrame( 
			body_work, 
			dest_pos.x, 
			move_speed );

	//ˆÚ“®İ’è
	gmBoss3BodyInitMoveNormal(
			body_work,
			&dest_pos,
			frame );

	return TRUE;
}

// =======================================================================
// gmBoss3BodyBattleCheckTurn
/*!
 *	ƒ^[ƒ“‚·‚é‚©ƒ`ƒFƒbƒN
 *
 *	@param body_work	[in] –{‘Ìƒ[ƒN
 *
 *	@retval TRUE ƒ^[ƒ“‚·‚é
 *	@retval FALSE ƒ^[ƒ“‚µ‚È‚¢
 */
// =======================================================================
BOOL gmBoss3BodyBattleCheckTurn( const GMS_BOSS3_BODY_WORK* body_work )
{
	const OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );
	amAssert( body_obj_work );

	if ( body_obj_work->disp_flag & OBD_DISP_HFLIP ){
		if ( body_work->end_pos.x <= body_work->start_pos.x ){
			return FALSE;
		}
	}
	else{
		if ( body_work->start_pos.x <= body_work->end_pos.x ){
			return FALSE;
		}
	}
	return TRUE;
}

// =======================================================================
// gmBoss3BodyBattleSearchPillar
/*!
 *	üˆÍ‚©‚ç’ŒƒMƒ~ƒbƒN‚ğ’T‚·
 *
 *	@return ’ŒƒMƒ~ƒbƒN‚ÌƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
OBS_OBJECT_WORK* gmBoss3BodyBattleSearchPillar( void )
{
	OBS_OBJECT_WORK* target_obj_work = ObjObjectSearchRegistObject( NULL, GMD_OBJTYPE_GIMMICK );
	while ( target_obj_work ){
		const GMS_ENEMY_3D_WORK* target_gimmick_work = (const GMS_ENEMY_3D_WORK*)target_obj_work;
		if ( target_gimmick_work->ene_com.eve_rec->id == GMD_EVENT_ID_BOSS3_PILLAR_MANAGER ){
			break;
		}

		//Ÿ‚ÌƒIƒuƒWƒFƒNƒg
		target_obj_work = ObjObjectSearchRegistObject( target_obj_work, GMD_OBJTYPE_GIMMICK );
	}
	return target_obj_work;
}

// =======================================================================
// gmBoss3BodyDamage
/*!
 *	ƒ_ƒ[ƒWˆ—
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyDamage( GMS_BOSS3_BODY_WORK* body_work )
{
	amAssert( body_work );
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );

	//–³“G
	if ( body_work->flag & GMD_BOSS3_BODY_FLAG_INVINCIBLE ){
		return;
	}

	GMS_BOSS3_MGR_WORK* mgr_work = gmBoss3MgrGetMgrWork(body_obj_work);
	amAssert( mgr_work );

	//‘Ì—Í
	--mgr_work->life;

	//¶‘¶‚µ‚Ä‚¢‚éê‡
	if ( mgr_work->life > 0 ){
		body_work->flag |= GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_DAMAGE;
	}
	//Œ‚”j‚³‚ê‚½ê‡
	else {
		body_work->flag |= GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_DEFEAT;
		
		// ƒ{ƒXŒ‚”jƒ^ƒCƒ~ƒ“ƒO‚ÌƒgƒƒtƒB[Šl“¾ƒ`ƒFƒbƒN
		HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING_DEFEAT_BOSS);
	}

	//Œø‰Ê‰¹iƒ_ƒ[ƒWj
	GmSoundPlaySE( "Boss0_01" );

	//ƒGƒtƒFƒNƒg
	gmBoss3EffDamageInit( body_work );

	//U“®
	GmPadVibSet(
			GMD_PAD_VIB_SMALL_TYPE, 
			GMD_PAD_VIB_SMALL_TIME,
			GMD_PAD_VIB_SMALL_LEFT_VIB, 
			GMD_PAD_VIB_SMALL_RIGHT_VIB,
			GMD_PAD_VIB_SMALL_ADD_DEC_TIME,
			GMD_PAD_VIB_SMALL_INT_VIB_TIME, 
			GMD_PAD_VIB_SMALL_INT_STOP_TIME,
			GMD_PAD_VIB_SMALL_PRIO - 1);

	//–³“GŠÔ
	gmBoss3BodySetInvincibleTime( body_work, GMD_BOSS3_BODY_INVINVIBLE_TIME );
}
// =======================================================================
// gmBoss3BodyEscapeCheckScreenOut
/*!
 *	“¦–S‚Ì‰æ–ÊŠO”»’è
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *
 *	@retval TRUE ‰æ–ÊŠO‚Éo‚½
 *	@retval FALSE ‰æ–Ê“à‚É‚¢‚é
 */
// =======================================================================
BOOL gmBoss3BodyEscapeCheckScreenOut( const GMS_BOSS3_BODY_WORK* body_work )
{
	const OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	if ( obj_work->pos.x >= (g_gm_main_system.map_fcol.right + GMD_BOSS3_BODY_ESCAPE_SCREEN_OUT_LENGTH)*FX32_ONE ){
		return TRUE;
	}	
	return FALSE;
}

// =======================================================================
// gmBoss3BodyEscapeAddjustSpeed
/*!
 *	“¦–S‚ÌƒXƒs[ƒh’²®
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyEscapeAddjustSpeed( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	if ( MTM_MATH_ABS(obj_work->spd.x) > GMD_BOSS3_BODY_ESCAPE_SPD_Y_MAX ){
		obj_work->spd.x = GMD_BOSS3_BODY_ESCAPE_SPD_X_MAX;
		obj_work->spd.y = GMD_BOSS3_BODY_ESCAPE_SPD_Y_MAX;
		obj_work->spd_add.x = 0;
		obj_work->spd_add.y = 0;
	}
}

// =======================================================================
// gmBoss3BodyChangeState
/*!
 *	ó‘Ô‘JˆÚ
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 *	@param state		[in] ‘JˆÚæ‚Ìó‘Ô
 */
// =======================================================================
void gmBoss3BodyChangeState(
							GMS_BOSS3_BODY_WORK* body_work,
							GME_BOSS3_BODY_STATE state )
{
	//I—¹ˆ—
	GMF_BOSS3_BODY_STATE_FUNC exit_func = gm_boss3_body_state_func_tbl_leave[body_work->state];
	if ( exit_func ){
		exit_func( body_work );
	}

	//ó‘Ô•ÏXİ’è
	body_work->prev_state = body_work->state;
	body_work->state = state;

	//ŠJnˆ—
	GMF_BOSS3_BODY_STATE_FUNC init_func = gm_boss3_body_state_func_tbl_enter[body_work->state];
	if ( init_func ){
		init_func( body_work );
	}
}

// =======================================================================
// gmBoss3BodyStateStartEnter
/*!
 *	ŠJn‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateStartEnter( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‹éŒ`–³Œø
	obj_work->flag |= OBD_OBJECT_NOHIT;
	
	//ƒAƒNƒVƒ‡ƒ“i‹­§İ’èj
	gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_START, TRUE);
		
	//Šp“x
	gmBoss3BodySetDirectionNormal( body_work );

	//ˆ—ŠÖ”•ÏX
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		body_work->proc_update = gmBoss3BodyStateStartUpdateWait;
	}
	else{
		body_work->proc_update = gmBoss3BodyStateStartUpdateWaitScrLimit;
	}

	//‘Ò‚¿ŠÔİ’è
	obj_work->user_timer = GMD_BOSS3_BODY_START_TIME_WAIT_END;

	//ƒz[ƒ~ƒ“ƒOƒAƒ^ƒbƒN‚Ì‘ÎÛ‚©‚ç‚Í‚¸‚·
	body_work->ene_3d.ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;
}

// =======================================================================
// gmBoss3BodyStateStartLeave
/*!
 *	ŠJnI—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateStartLeave( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‹éŒ`—LŒø
	obj_work->flag &= ~OBD_OBJECT_NOHIT;

	//UŒ‚‚µ‚È‚¢ƒtƒ‰ƒO‚ğ‚¨‚ë‚·
	body_work->flag &= ~GMD_BOSS3_BODY_FLAG_NOATTACK;

	//ƒz[ƒ~ƒ“ƒOƒAƒ^ƒbƒN‚Ì‘ÎÛ‚É‚·‚é
	body_work->ene_3d.ene_com.enemy_flag &= ~GMD_ENEMY_FLAG_NOHOMING;
}

// =======================================================================
// gmBoss3BodyStateStartUpdateWaitScrLimit
/*!
 *	ŠJnXVi‘Ò‹@j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateStartUpdateWaitScrLimit( GMS_BOSS3_BODY_WORK* body_work )
{
	//ƒXƒNƒ[ƒ‹ƒƒbƒNŠÄ‹
	if ( !(g_gm_main_system.game_flag & GMD_GAME_FLAG_SCR_LIMIT_BUSY) ){
		return;
	}

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateStartUpdateWait;
}

// =======================================================================
// gmBoss3BodyStateStartUpdateWait
/*!
 *	ŠJnXVi‘Ò‹@j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateStartUpdateWait( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//Œü‚«
	gmBoss3BodySetDirectionNormal( body_work );

	//‘Ò‹@
	if ( obj_work->user_timer > 0 ){
		--obj_work->user_timer;
		return;
	}
	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_MOVE );

	//ˆÚ“®ŠJn
#if _IPHONE
	body_work->is_move = TRUE;
#else
	obj_work->move_flag &= ~OBD_MOVE_NOMOVE;
#endif // _IPHONE

	//ƒAƒtƒ^ƒo[ƒi—LŒø‰»
	gmBoss3EffAfterburnerRequestCreate( body_work );

	//•ûŒü“]Š·
	obj_work->disp_flag &= ~OBD_DISP_HFLIP;
	gmBoss3BodyInitTurn(
			body_work, 
			GMD_BOSS3_ANGLE_RIGHT,
			GMD_BOSS3_BODY_FRAME_TURN, 
			TRUE );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateStartUpdateEnd;
}

// =======================================================================
// gmBoss3BodyStateStartUpdateEnd
/*!
 *	ŠJnXViI—¹j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateStartUpdateEnd( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//•ûŒü“]Š·Às
	gmBoss3BodyUpdateTurn(body_work);

	//‰æ–ÊŠO‚Éo‚é‚Ì‘Ò‚¿
	if ( !ObjViewOutCheck(obj_work->pos.x, obj_work->pos.y, GMD_BOSS3_BODY_START_WAIT_OUT_OFFSET, 0, 0, 0, 0) ){
		return;
	}	

	//ƒXƒNƒ[ƒ‹ƒƒbƒN‰ğœ
	GmGmkCamScrLimitRelease( GMD_GMK_SCR_LMT_RELEASE_RIGHT );

	//’ÇÕó‘Ô‚Ö•ÏX
	gmBoss3BodyChangeState( body_work, GMD_BOSS3_BODY_STATE_CHASE_MOVE );
}

// =======================================================================
// gmBoss3BodyStateChaseMoveEnter
/*!
 *	’ÇÕˆÚ“®‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateChaseMoveEnter( GMS_BOSS3_BODY_WORK* body_work )
{	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_MOVE );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateChaseMoveUpdate;

	//ƒAƒtƒ^ƒo[ƒi—LŒø‰»
	gmBoss3EffAfterburnerRequestCreate( body_work );
}

// =======================================================================
// gmBoss3BodyStateChaseMoveLeave
/*!
 *	’ÇÕˆÚ“®I—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateChaseMoveLeave( GMS_BOSS3_BODY_WORK* body_work )
{		
	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss3EffAfterburnerRequestDelete( body_work );
}

// =======================================================================
// gmBoss3BodyStateChaseMoveUpdate
/*!
 *	’ÇÕˆÚ“®XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateChaseMoveUpdate( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//•ûŒü“]Š·”»’è
	if ( gmBoss3BodyChaseCheckTurn(body_work) ){
		Angle16 dest_angle;
		BOOL flag_positive;

		//‰E‚ÉŒü‚©‚¤
		if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
			obj_work->disp_flag &= ~OBD_DISP_HFLIP;
			dest_angle = GMD_BOSS3_ANGLE_RIGHT;
			flag_positive = TRUE;
		}
		//¶‚ÉŒü‚©‚¤
		else {
			obj_work->disp_flag |= OBD_DISP_HFLIP;
			dest_angle = GMD_BOSS3_ANGLE_LEFT;
			flag_positive = FALSE;
		}

		//•ûŒü“]Š·
		gmBoss3BodyInitTurn(
				body_work, 
				dest_angle,
				GMD_BOSS3_BODY_FRAME_TURN, 
				flag_positive );
	}

	//•ûŒü“]Š·Às
	gmBoss3BodyUpdateTurn(body_work);

	//ƒtƒ‰ƒOƒ`ƒFƒbƒN
	if ( obj_work->user_flag ){
		//’ÇÕó‘Ô‚Ö•ÏX
		gmBoss3BodyChangeState( body_work, GMD_BOSS3_BODY_STATE_PRE_BATTLE );
	}
}

// =======================================================================
// gmBoss3BodyStatePreBattleEnter
/*!
 *	ƒoƒgƒ‹‘O‰‰o‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStatePreBattleEnter( GMS_BOSS3_BODY_WORK* body_work )
{	
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒAƒNƒVƒ‡ƒ“
	gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_MOVE );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStatePreBattleUpdateStart;

	//‘Ò‚¿ŠÔİ’è
	obj_work->user_timer = GMD_BOSS3_BATTLE_FRAME_WAIT_START;

	//ˆÚ“®ŠÖ”•ÏX
	obj_work->ppMove = ObjObjectMove;	

	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss3EffAfterburnerRequestDelete( body_work );
}

// =======================================================================
// gmBoss3BodyStatePreBattleLeave
/*!
 *	ƒoƒgƒ‹‘O‰‰oI—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStatePreBattleLeave( GMS_BOSS3_BODY_WORK* body_work )
{		
	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss3EffAfterburnerRequestDelete( body_work );
}

// =======================================================================
// gmBoss3BodyStatePreBattleUpdateStart
/*!
 *	ƒoƒgƒ‹‘O‰‰oXV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStatePreBattleUpdateStart( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‰æ–ÊŠO”»’è
	if ( ObjViewOutCheck(obj_work->pos.x, obj_work->pos.y, 0, 0, 0, 0, 0) ){
		return;
	}

	//‘Ò‹@
	if ( obj_work->user_timer > 0 ){
		--obj_work->user_timer;
		return;
	}

	//ƒJƒƒ‰ƒXƒP[ƒ‹
	if ( !_am_draw_video.wide_screen ){
		GmCameraScaleSet(GMD_BOSS3_BODY_BATTLE_CAMERA_SCALE, (1.0f-GMD_BOSS3_BODY_BATTLE_CAMERA_SCALE)/GMD_BOSS3_BODY_BATTLE_CAMERA_FRAME);
		GmMapSetDrawMarginMag();
	}


	//•ûŒü“]Š·
	obj_work->disp_flag |= OBD_DISP_HFLIP;
	gmBoss3BodyInitTurn(
			body_work, 
			GMD_BOSS3_ANGLE_LEFT,
			GMD_BOSS3_BODY_FRAME_TURN, 
			FALSE );

	//ƒAƒNƒVƒ‡ƒ“
	gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_MOVE );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStatePreBattleUpdateTurn;

	//BGM•ÏX
	if ( !GmBsCmnIsFinalZoneType(obj_work) ){
		GmSoundChangeAngryBossBGM();
	}

}

// =======================================================================
// gmBoss3BodyStatePreBattleUpdateTurn
/*!
 *	ƒoƒgƒ‹‘O‰‰oXViU‚èŒü‚«j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStatePreBattleUpdateTurn( GMS_BOSS3_BODY_WORK* body_work )
{
	//•ûŒü“]Š·Às
	Float frame = gmBoss3BodyUpdateTurn(body_work);

	//•ûŒü“]Š·I—¹‘Ò‚¿
	if ( frame > 0 ){
		return;
	}

	//ƒAƒNƒVƒ‡ƒ“
	gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_PRE_BATTLE );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStatePreBattleUpdateLaugh;

	//’Œ‚É“®ì—v‹
	OBS_OBJECT_WORK* obj_work_pillar = gmBoss3BodyBattleSearchPillar();
	if ( obj_work_pillar ){
		GmGmkBoss3PillarWallChangeModeActive( obj_work_pillar );
	}
	
#if _IPHONE
	// ƒ}ƒbƒvƒTƒCƒY§ŒÀ
	GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_ZONE3_BOSS);

	//…–Ê•`‰æ‚ğØ‚é
	GmWaterSurfaceSetFlagDraw( FALSE );
#endif //_IPHONE
}

// =======================================================================
// gmBoss3BodyStatePreBattleUpdateLaugh
/*!
 *	ƒoƒgƒ‹‘O‰‰oXViÎ‚¢j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStatePreBattleUpdateLaugh( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//’Œ‚ÉƒvƒŒƒCƒ„‚ğˆ³€‚³‚¹‚éİ’è
	OBS_OBJECT_WORK* obj_work_pillar = gmBoss3BodyBattleSearchPillar();
	if ( obj_work_pillar ){
		GmGmkBoss3PillarWallClearFlagNoPressDie( obj_work_pillar );
	}

	//ƒoƒgƒ‹ó‘Ô‚Ö•ÏX
	gmBoss3BodyChangeState( body_work, GMD_BOSS3_BODY_STATE_BATTLE );

	//ƒJƒƒ‰ƒXƒP[ƒ‹
	GmCameraScaleSet(1.0f, (1.0f-GMD_BOSS3_BODY_BATTLE_CAMERA_SCALE)/GMD_BOSS3_BODY_BATTLE_CAMERA_FRAME);
	GmMapSetDrawMarginNormal();
}

// =======================================================================
// gmBoss3BodyStateBattleEnter
/*!
 *	ƒoƒgƒ‹ˆÚ“®‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateBattleEnter( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );
	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_MOVE );

	//–Ú“IÀ•WiˆÚ“®”ÍˆÍ‚Ì’†Sj
	s32 widht = g_gm_main_system.map_fcol.right - g_gm_main_system.map_fcol.left; 
	fx32 dest_x = (fx32)(g_gm_main_system.map_fcol.left + widht/2) * FX32_ONE;
	VecFx32 dest_pos = {
		dest_x,
		obj_work->pos.y,
		obj_work->pos.z
	};

	//ˆÚ“®ƒtƒŒ[ƒ€”
	Float frame = gmBoss3BodyCalcMoveXNormalFrame( 
			body_work, 
			dest_pos.x, 
			GMD_BOSS3_BODY_BATTLE_MOVE_SPEED_MOVE );

	//ˆÚ“®İ’è
	gmBoss3BodyInitMoveNormal(
			body_work,
			&dest_pos,
			frame );

	//•ûŒü“]Š·”»’è
	if ( gmBoss3BodyBattleCheckTurn(body_work) ){
		Angle16 dest_angle;
		BOOL flag_positive;

		//‰E‚ÉŒü‚©‚¤
		if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
			obj_work->disp_flag &= ~OBD_DISP_HFLIP;
			dest_angle = GMD_BOSS3_ANGLE_RIGHT;
			flag_positive = TRUE;
		}
		//¶‚ÉŒü‚©‚¤
		else {
			obj_work->disp_flag |= OBD_DISP_HFLIP;
			dest_angle = GMD_BOSS3_ANGLE_LEFT;
			flag_positive = FALSE;
		}

		//•ûŒü“]Š·
		gmBoss3BodyInitTurn(
				body_work, 
				dest_angle,
				GMD_BOSS3_BODY_FRAME_TURN, 
				flag_positive );
	}

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateBattleUpdateMoveCenter;

	//ƒAƒtƒ^ƒo[ƒi—LŒø‰»
	gmBoss3EffAfterburnerRequestCreate( body_work );
}

// =======================================================================
// gmBoss3BodyStateBattleLeave
/*!
 *	ƒoƒgƒ‹ˆÚ“®I—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateBattleLeave( GMS_BOSS3_BODY_WORK* body_work )
{		
	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss3EffAfterburnerRequestDelete( body_work );
}

// =======================================================================
// gmBoss3BodyStateBattleUpdateMoveCenter
/*!
 *	ƒoƒgƒ‹ˆÚ“®XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateBattleUpdateMoveCenter( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//•ûŒü“]Š·Às
	Float frame_turn = gmBoss3BodyUpdateTurn(body_work);

	//ˆÚ“®Às
	Float frame_move = gmBoss3BodyUpdateMoveNormal(body_work);

	///ˆÚ“®I—¹‘Ò‚¿
	if ( frame_turn > 0 || frame_move > 0 ){
		return;
	}
	
	//ƒAƒNƒVƒ‡ƒ“
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_SEARCH_L );
	}
	else{
		gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_SEARCH_R );
	}

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateBattleUpdateSearch;

	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss3EffAfterburnerRequestDelete( body_work );
}

// =======================================================================
// gmBoss3BodyStateBattleUpdateSearch
/*!
 *	ƒoƒgƒ‹ˆÚ“®XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateBattleUpdateSearch( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//ƒoƒgƒ‹ƒpƒ^[ƒ“Œˆ’è
	body_work->pattern_no = gmBoss3BodyBattleCalcPattern( body_work );

	//ˆÚ“®À•Wİ’è
	if ( gmBoss3BodyBattleInitMovePattern( body_work, body_work->pattern_no, 0, GMD_BOSS3_BODY_BATTLE_MOVE_SPEED_MOVE ) ){

		//•ûŒü“]Š·”»’è
		if ( gmBoss3BodyBattleCheckTurn(body_work) ){
			Angle16 dest_angle;
			BOOL flag_positive;

			//‰E‚ÉŒü‚©‚¤
			if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
				obj_work->disp_flag &= ~OBD_DISP_HFLIP;
				dest_angle = GMD_BOSS3_ANGLE_RIGHT;
				flag_positive = TRUE;
			}
			//¶‚ÉŒü‚©‚¤
			else {
				obj_work->disp_flag |= OBD_DISP_HFLIP;
				dest_angle = GMD_BOSS3_ANGLE_LEFT;
				flag_positive = FALSE;
			}

			//•ûŒü“]Š·
			gmBoss3BodyInitTurn(
					body_work, 
					dest_angle,
					GMD_BOSS3_BODY_FRAME_TURN, 
					flag_positive );
		}

		//ˆ—ŠÖ”•ÏX
		body_work->proc_update = gmBoss3BodyStateBattleUpdateMoveFirst;

		//ƒAƒtƒ^ƒo[ƒi—LŒø‰»
		gmBoss3EffAfterburnerRequestCreate( body_work );
		
		//ƒAƒNƒVƒ‡ƒ“
		gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_MOVE );
	}
	else{
		//ƒAƒNƒVƒ‡ƒ“
		gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_SIGN );

		//ˆ—ŠÖ”•ÏX
		body_work->proc_update = gmBoss3BodyStateBattleUpdateSign;
	}
}

// =======================================================================
// gmBoss3BodyStateBattleUpdateMoveFirst
/*!
 *	ƒoƒgƒ‹ˆÚ“®XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateBattleUpdateMoveFirst( GMS_BOSS3_BODY_WORK* body_work )
{
	//•ûŒü“]Š·Às
	Float frame_turn = gmBoss3BodyUpdateTurn(body_work);

	//ˆÚ“®Às
	Float frame_move = gmBoss3BodyUpdateMoveNormal(body_work);

	///ˆÚ“®I—¹‘Ò‚¿
	if ( frame_turn > 0 || frame_move > 0 ){
		return;
	}
	
	//ƒAƒNƒVƒ‡ƒ“
	gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_SIGN );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateBattleUpdateSign;

	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss3EffAfterburnerRequestDelete( body_work );
}

// =======================================================================
// gmBoss3BodyStateBattleUpdateSign
/*!
 *	ƒoƒgƒ‹ˆÚ“®XViƒTƒCƒ“j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateBattleUpdateSign( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//’Œ‚É“®ì—v‹
	OBS_OBJECT_WORK* obj_work_pillar = gmBoss3BodyBattleSearchPillar();
	if ( obj_work_pillar ){
		GmGmkBoss3PillarChangeModeActive( obj_work_pillar, body_work->pattern_no );
	}

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateBattleUpdateWaitPillar;

	//‘Ò‚¿ŠÔİ’è
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		obj_work->user_timer = GMD_BOSS3_BATTLE_FRAME_WAIT_ACTIVE_FINAL;
	}
	else{
		obj_work->user_timer = GMD_BOSS3_BATTLE_FRAME_WAIT_ACTIVE;
	}	
	
	//ƒAƒNƒVƒ‡ƒ“
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_LAUGH_L );
	}
	else{
		gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_LAUGH_R );
	}
}

// =======================================================================
// gmBoss3BodyStateBattleUpdateWaitPillar
/*!
 *	ƒoƒgƒ‹ˆÚ“®XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateBattleUpdateWaitPillar( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‘Ò‹@
	if ( obj_work->user_timer > 0 ){
		--obj_work->user_timer;
		return;
	}

	//ˆÚ“®À•Wİ’è
	fx32 move_speed = GMD_BOSS3_BODY_BATTLE_MOVE_SPEED_MOVE;
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		//move_speed = FX_Mul( move_speed, GMD_BOSS3_BATTLE_ADJUST_SPEED_MOVE_SECOND );
	}
	if ( gmBoss3BodyBattleInitMovePattern( body_work, body_work->pattern_no, 1, move_speed ) ){
		//•ûŒü“]Š·”»’è
		if ( gmBoss3BodyBattleCheckTurn(body_work) ){
			Angle16 dest_angle;
			BOOL flag_positive;

			//‰E‚ÉŒü‚©‚¤
			if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
				obj_work->disp_flag &= ~OBD_DISP_HFLIP;
				dest_angle = GMD_BOSS3_ANGLE_RIGHT;
				flag_positive = TRUE;
			}
			//¶‚ÉŒü‚©‚¤
			else {
				obj_work->disp_flag |= OBD_DISP_HFLIP;
				dest_angle = GMD_BOSS3_ANGLE_LEFT;
				flag_positive = FALSE;
			}

			//•ûŒü“]Š·
			gmBoss3BodyInitTurn(
					body_work, 
					dest_angle,
					GMD_BOSS3_BODY_FRAME_TURN, 
					flag_positive );
		}
		//ƒAƒtƒ^ƒo[ƒi—LŒø‰»
		gmBoss3EffAfterburnerRequestCreate( body_work );

		//ˆ—ŠÖ”•ÏX
		body_work->proc_update = gmBoss3BodyStateBattleUpdateMoveSecond;
	
		//ƒAƒNƒVƒ‡ƒ“
		gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_MOVE );
	}
	else{
		//ˆ—ŠÖ”•ÏX
		body_work->proc_update = gmBoss3BodyStateBattleUpdateWaitActive;

		//‘Ò‚¿ŠÔİ’è
		obj_work->user_timer = GmGmkBoss3PillarGetActiveTime(body_work->pattern_no) - GMD_BOSS3_BATTLE_FRAME_WAIT_ACTIVE + GMD_BOSS3_BATTLE_FRAME_WAIT;
	
		//ƒAƒNƒVƒ‡ƒ“
		if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
			gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_LAUGH_L );
		}
		else{
			gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_LAUGH_R );
		}
	}
}

// =======================================================================
// gmBoss3BodyStateBattleUpdateMoveSecond
/*!
 *	ƒoƒgƒ‹ˆÚ“®XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateBattleUpdateMoveSecond( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//•ûŒü“]Š·Às
	Float frame_turn = gmBoss3BodyUpdateTurn(body_work);

	//ˆÚ“®Às
	Float frame_move = gmBoss3BodyUpdateMoveNormal(body_work);

	///ˆÚ“®I—¹‘Ò‚¿
	if ( frame_turn > 0 || frame_move > 0 ){
		return;
	}	
	
	//ƒAƒNƒVƒ‡ƒ“
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_LAUGH_L );
	}
	else{
		gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_LAUGH_R );
	}

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateBattleUpdateWaitActive;

	//‘Ò‚¿ŠÔİ’è
	s32 wait_move = GMD_BOSS3_BATTLE_FRAME_WAIT_ACTIVE;
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		wait_move = GMD_BOSS3_BATTLE_FRAME_WAIT_ACTIVE_FINAL;
	}
	obj_work->user_timer = GmGmkBoss3PillarGetActiveTime(body_work->pattern_no) - wait_move - (s32)body_work->move_frame + GMD_BOSS3_BATTLE_FRAME_WAIT;

	//ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg’â~
	gmBoss3EffAfterburnerRequestDelete( body_work );
}

// =======================================================================
// gmBoss3BodyStateBattleUpdateWaitActive
/*!
 *	ƒoƒgƒ‹ˆÚ“®XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateBattleUpdateWaitActive( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	///I—¹‘Ò‚¿
	if ( obj_work->user_timer > 0 ){
		--obj_work->user_timer;
		return;
	}

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateBattleUpdateWaitReturn;

	//‘Ò‚¿ŠÔİ’è
	obj_work->user_timer = GMD_BOSS3_BATTLE_FRAME_WAIT_RETURN;

	//’Œ‚É“®ì—v‹
	OBS_OBJECT_WORK* obj_work_pillar = gmBoss3BodyBattleSearchPillar();
	if ( obj_work_pillar ){
		GmGmkBoss3PillarChangeModeReturn( obj_work_pillar );
	}
}

// =======================================================================
// gmBoss3BodyStateBattleUpdateWait
/*!
 *	ƒoƒgƒ‹ˆÚ“®XV
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateBattleUpdateWaitReturn( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	///I—¹‘Ò‚¿
	if ( obj_work->user_timer > 0 ){
		--obj_work->user_timer;
		return;
	}

	//ƒoƒgƒ‹ó‘Ô‚Ö•ÏX
	gmBoss3BodyChangeState( body_work, GMD_BOSS3_BODY_STATE_BATTLE );

	//’Œ‚É“®ì—v‹
	OBS_OBJECT_WORK* obj_work_pillar = gmBoss3BodyBattleSearchPillar();
	if ( obj_work_pillar ){
		GmGmkBoss3PillarChangeModeDelete( obj_work_pillar );
	}
}

// =======================================================================
// gmBoss3BodyStateDefeatEnter
/*!
 *	Œ‚”j‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateDefeatEnter( GMS_BOSS3_BODY_WORK* body_work )
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
	body_work->proc_update = gmBoss3BodyStateDefeatUpdateStart;

	//‘Ò‚¿ŠÔİ’è
	obj_work->user_timer = GMD_BOSS3_BODY_DEFEAT_TIME_WAIT_START;

	//’Œ‚É“®ì—v‹
	OBS_OBJECT_WORK* obj_work_pillar = gmBoss3BodyBattleSearchPillar();
	if ( obj_work_pillar ){
		GmGmkBoss3PillarChangeModeReturn( obj_work_pillar );
	}

	//ƒ{ƒXíŸ—˜BGM‚Ö•ÏX
	GmSoundChangeWinBossBGM();
}

// =======================================================================
// gmBoss3BodyStateDefeatLeave
/*!
 *	Œ‚”jI—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateDefeatLeave( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒAƒjƒ[ƒVƒ‡ƒ“ÄŠJ
	obj_work->disp_flag &= ~OBD_DISP_STOP;

	//‹éŒ`—LŒø
	obj_work->flag &= ~OBD_OBJECT_NOHIT;

	//’Œ‚É“®ì—v‹
	OBS_OBJECT_WORK* obj_work_pillar = gmBoss3BodyBattleSearchPillar();
	if ( obj_work_pillar ){
		GmGmkBoss3PillarChangeModeDelete( obj_work_pillar );
	}
}

// =======================================================================
// gmBoss3BodyStateDefeatUpdateStart
/*!
 *	Œ‚”jXViŠJnj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateDefeatUpdateStart( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	///I—¹‘Ò‚¿
	if ( obj_work->user_timer > 0 ){
		--obj_work->user_timer;
		return;
	}

	//¬”š”­ƒGƒtƒFƒNƒg‰Šú‰»
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ(body_work);
	gmBoss3EffBombsInit( 
			&body_work->bomb_work,
			body_obj_work,
			body_obj_work->pos.x,
			body_obj_work->pos.y,
			FX32_ONE * 80,
			FX32_ONE * 80,
			10, 
			30);

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateDefeatUpdateFall;

	//‘Ò‚¿ŠÔİ’è
	obj_work->user_timer = GMD_BOSS3_BODY_DEFEAT_TIME_WAIT_BOMB;
}

// =======================================================================
// gmBoss3BodyStateDefeatUpdateExplode
/*!
 *	Œ‚”jXVi”š”­j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateDefeatUpdateFall( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	///I—¹‘Ò‚¿
	if ( obj_work->user_timer > 0 ){
		--obj_work->user_timer;

		//¬”š”­ƒGƒtƒFƒNƒgXV
		gmBoss3EffBombsUpdate( &body_work->bomb_work );
		return;
	}

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateDefeatUpdateExplode;
}

// =======================================================================
// gmBoss3BodyStateDefeatUpdateExplode
/*!
 *	Œ‚”jXVi”š”­j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateDefeatUpdateExplode( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ƒp[ƒc”òUƒVƒOƒiƒ‹
	body_work->flag	|= GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_SCATTER;
	
	//Œø‰Ê‰¹i‘å”š”­j
	GmSoundPlaySE( "Boss0_03" );

	// ƒRƒ“ƒgƒ[ƒ‰[U“®
	GMM_PAD_VIB_MID_TIME(120);

	// ‰æ–Êƒtƒ‰ƒbƒVƒ…İ’è
	GmBsCmnInitFlashScreen( 
			&body_work->flash_work, 
			GMD_BOSS3_BODY_DEFEAT_FLASH_INTO_TIME,
			GMD_BOSS3_BODY_DEFEAT_FLASH_KEEP_TIME,
			GMD_BOSS3_BODY_DEFEAT_FLASH_RETURN_TIME );

	//‘å”š”­ƒGƒtƒFƒNƒg‰Šú‰»
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );
	amAssert( body_obj_work );
	OBS_OBJECT_WORK* bomb_obj_work = (OBS_OBJECT_WORK*)GmEfctCmnEsCreate(
			body_obj_work, 
			GME_EFCT_CMN_IDX_BOMB_BIG );
	bomb_obj_work->pos.z = body_obj_work->pos.z + GMD_BOSS3_EFFECT_BOMB_OFFSET_Z;

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateDefeatUpdateScatter;
		
	//ƒXƒRƒA‰ÁZ
	GmPlayerAddScoreNoDisp( (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj(), GMD_PLY_SCORE_BOSS );

	//‘Ò‚¿ŠÔİ’è
	obj_work->user_timer = GMD_BOSS3_BODY_DEFEAT_TIME_WAIT_SCATTER;
}

// =======================================================================
// gmBoss3BodyStateDefeatUpdateScatter
/*!
 *	Œ‚”jXVi‚Î‚çT‚«j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateDefeatUpdateScatter( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‰æ–Êƒtƒ‰ƒbƒVƒ…XV
	GmBsCmnUpdateFlashScreen( &body_work->flash_work );

	///I—¹‘Ò‚¿
	if ( obj_work->user_timer > 0 ){
		--obj_work->user_timer;
		return;
	}

	//•‚±‚°ƒeƒNƒXƒ`ƒƒ‚É•ÏX
	gmBoss3ChangeTextureBurnt( obj_work );

	//•‚±‚°ƒVƒOƒiƒ‹
	body_work->flag |= GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_BURNT;

	//ƒAƒtƒ^[ƒo[ƒi[‰ŒƒGƒtƒFƒNƒg‰Šú‰»
	gmBoss3EffAfterburnerSmokeInit( body_work );

	//–{‘Ì‰ŒƒGƒtƒFƒNƒg‰Šú‰»
	gmBoss3EffBodySmokeInit( body_work );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateDefeatUpdateEnd;

	//‘Ò‚¿ŠÔİ’è
	obj_work->user_timer = GMD_BOSS3_BODY_DEFEAT_TIME_WAIT_END;
}

// =======================================================================
// gmBoss3BodyStateDefeatUpdateEnd
/*!
 *	Œ‚”jXViI—¹j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateDefeatUpdateEnd( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	///I—¹‘Ò‚¿
	if ( obj_work->user_timer > 0 ){
		--obj_work->user_timer;
		return;
	}

	//“¦–Só‘Ô‚Ö•ÏX
	gmBoss3BodyChangeState( body_work, GMD_BOSS3_BODY_STATE_ESCAPE );
}

// =======================================================================
// gmBoss3BodyStateEscapeEnter
/*!
 *	“¦–S‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateEscapeEnter( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//ˆÚ“®’l
	obj_work->spd.x	= 0;
	obj_work->spd.y	= 0;
	obj_work->spd_add.x = GMD_BOSS3_BODY_ESCAPE_SPD_X_ADD;
	obj_work->spd_add.y = -GMD_BOSS3_BODY_ESCAPE_SPD_Y_ADD;

	//•ûŒü“]Š·”»’è
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		obj_work->disp_flag &= ~OBD_DISP_HFLIP;

		//•ûŒü“]Š·
		gmBoss3BodyInitTurn(
				body_work, 
				GMD_BOSS3_ANGLE_RIGHT,
				GMD_BOSS3_BODY_FRAME_TURN, 
				TRUE );
	}

	//‹éŒ`–³Œø
	obj_work->flag |= OBD_OBJECT_NOHIT;
	obj_work->move_flag |= OBD_MOVE_NOCOLFIELD | OBD_MOVE_NOCOL;

	//Œü‚«
	gmBoss3BodySetDirectionNormal( body_work );

	//ƒAƒNƒVƒ‡ƒ“i‹­§İ’èj
	gmBoss3BodySetActionAllParts( body_work, GMD_BOSS3_ACT_ID_ESCAPE, TRUE );
	
	//“¦–SƒVƒOƒiƒ‹
	body_work->flag |= GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_ESCAPE;
	
#if _IPHONE
	GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_HORI);
#endif // _IPHONE
	
	//ˆ—ŠÖ”•ÏX
	if ( GmBsCmnIsFinalZoneType(obj_work) ){
		body_work->proc_update = gmBoss3BodyStateEscapeUpdateFinalZone;
	}
	else{
		body_work->proc_update = gmBoss3BodyStateEscapeUpdateScrollLock;
	}
}

// =======================================================================
// gmBoss3BodyStateEscapeLeave
/*!
 *	“¦–SI—¹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateEscapeLeave( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
	amAssert( obj_work );

	//‹éŒ`—LŒø
	obj_work->flag &= ~OBD_OBJECT_NOHIT;
}

// =======================================================================
// gmBoss3BodyStateEscapeUpdateScrollUnlock
/*!
 *	“¦–SXViƒXƒNƒ[ƒ‹ƒƒbƒNj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateEscapeUpdateScrollLock( GMS_BOSS3_BODY_WORK* body_work )
{
	amAssert( body_work );

	//ƒXƒs[ƒh’²®
	gmBoss3BodyEscapeAddjustSpeed( body_work );

	//•ûŒü“]Š·Às
	Float frame_turn = gmBoss3BodyUpdateTurn(body_work);
	if ( frame_turn > 0 ){
		return;
	}

	//ƒXƒNƒ[ƒ‹ƒƒbƒN‰ğœ
	GmGmkCamScrLimitRelease( GMD_GMK_SCR_LMT_RELEASE_RIGHT );

	//’Œ‚É“®ì—v‹
	OBS_OBJECT_WORK* obj_work_pillar = gmBoss3BodyBattleSearchPillar();
	if ( obj_work_pillar ){
		GmGmkBoss3PillarWallChangeModeReturn( obj_work_pillar );
	}

	//ƒGƒtƒFƒNƒg
	GmEfctBossCmnEsCreate(
			GMM_BS_OBJ(body_work),
			GME_EFCT_BOSS_CMN_IDX_BOSS_PARTS );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateEscapeUpdateWaitScreenOut;
}

// =======================================================================
// gmBoss3BodyStateEscapeUpdateWaitScreenOut
/*!
 *	“¦–SXViƒXƒNƒ[ƒ‹ƒAƒ“ƒƒbƒNj
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateEscapeUpdateWaitScreenOut( GMS_BOSS3_BODY_WORK* body_work )
{
	//ƒXƒs[ƒh’²®
	gmBoss3BodyEscapeAddjustSpeed( body_work );

	//‰æ–ÊŠO”»’è
	if ( gmBoss3BodyEscapeCheckScreenOut(body_work) ){
		OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( body_work );
		GMS_BOSS3_MGR_WORK* mgr_work = gmBoss3MgrGetMgrWork(obj_work);
		amAssert( mgr_work );
		mgr_work->flag |= GMD_BOSS3_MGR_FLAG_CLEAR_BOSS;
		body_work->proc_update = NULL;
	}
}

// =======================================================================
// gmBoss3BodyStateEscapeUpdateFinalZone
/*!
 *	“¦–SXViƒtƒ@ƒCƒiƒ‹ƒ][ƒ“j
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyStateEscapeUpdateFinalZone( GMS_BOSS3_BODY_WORK* body_work )
{
	amAssert( body_work );

	//ƒXƒs[ƒh’²®
	gmBoss3BodyEscapeAddjustSpeed( body_work );

	//•ûŒü“]Š·Às
	Float frame_turn = gmBoss3BodyUpdateTurn(body_work);
	if ( frame_turn > 0 ){
		return;
	}

	//ƒGƒtƒFƒNƒg
	GmEfctBossCmnEsCreate(
			GMM_BS_OBJ(body_work),
			GME_EFCT_BOSS_CMN_IDX_BOSS_PARTS );

	//ˆ—ŠÖ”•ÏX
	body_work->proc_update = gmBoss3BodyStateEscapeUpdateWaitScreenOut;
}

// =======================================================================
// gmBoss3BodyMainFuncWaitSetup
/*!
 *	ƒƒCƒ“ˆ—i¶¬‘Ò‚¿j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyMainFuncWaitSetup( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS3_BODY_WORK* body_work = (GMS_BOSS3_BODY_WORK*)obj_work;
	amAssert( body_work );

	//“Ç‚İ‚İ‘Ò‚¿
	GMS_BOSS3_MGR_WORK* mgr_work = gmBoss3MgrGetMgrWork(obj_work);
	amAssert( mgr_work );
	if ( !gmBoss3MgrCheckSetupComplete(mgr_work) ){
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
			GMD_BOSS3_BODY_SNM_INDEX_MAX );

	//ƒ‚[ƒVƒ‡ƒ“ƒR[ƒ‹ƒoƒbƒN‚ğÀsƒŠƒXƒg‚É’Ç‰Á
	GmBsCmnAppendBossMotionCallback(
			&body_work->bmcb_mgr,
			&body_work->snm_work.bmcb_link );


	//Ú‘±ƒm[ƒh‚ğ“o˜^
	for ( s32 i = 0; GMD_BOSS3_BODY_SNM_INDEX_MAX > i; ++i ){
		body_work->snm_reg_id[i] = GmBsCmnRegisterSNMNode(
				&body_work->snm_work, 
				g_boss3_node_index_list[i] );	
	}			

	//-----------------------------------------
	//ƒƒCƒ“ˆ—
	//-----------------------------------------
	//ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmBoss3BodyMainFunc;

	//ŠJnó‘Ô‚Ö
	gmBoss3BodyChangeState( body_work, GMD_BOSS3_BODY_STATE_START );
}

// =======================================================================
// gmBoss3BodyMainFunc
/*!
 *	ƒƒCƒ“ˆ—i“Ç‚İ‚İ‘Ò‚¿j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3BodyMainFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS3_BODY_WORK* body_work = (GMS_BOSS3_BODY_WORK*)obj_work;
	amAssert( body_work );

	//ƒqƒbƒg–³ŒøŠÔ‚ğXV
	gmBoss3BodyUpdateNoHitTime( body_work );

	//–³“GŠÔ‚ğXV
	gmBoss3BodyUpdateInvincibleTime( body_work );

	//ó‘ÔXV
	if ( body_work->proc_update ){
		body_work->proc_update( body_work );
	}

	//-----------------------------------------
	//ƒVƒOƒiƒ‹ƒ`ƒFƒbƒN
	//-----------------------------------------
	//ƒAƒtƒ^ƒo[ƒi¶¬
	if ( body_work->flag & GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_AFTERBURNER ){
		gmBoss3EffAfterburnerInit( body_work );
	}

	//Œ‚”j
	if ( body_work->flag & GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_DEFEAT ){
		//ƒ_ƒ[ƒWI—¹’Ê’m
		body_work->flag	&= ~(GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_DEFEAT | GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_DAMAGE);

		//ó‘Ô•ÏX
		gmBoss3BodyChangeState( body_work, GMD_BOSS3_BODY_STATE_DEFEAT );
		return;
	}

	//ƒ_ƒ[ƒW
	if ( body_work->flag & GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_DAMAGE ){
		body_work->flag	&= ~GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_DAMAGE;

		// ƒGƒbƒOƒ}ƒ“’Ê’m
		body_work->flag |= GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_DAMAGE;

		//ƒ_ƒ[ƒW“_–Å‚ğ‰Šú‰»
		GmBsCmnInitObject3DNNDamageFlicker( 
				obj_work, 
				&body_work->flk_work,
				GMD_BOSS3_BODY_DMG_FLICKER_RADIUS );
	}

	//-----------------------------------------
	//ŠeíXV
	//-----------------------------------------
	//ƒ_ƒ[ƒW“_–Å‚ğXV
	GmBsCmnUpdateObject3DNNDamageFlicker( obj_work, &body_work->flk_work );

	//Šp“x”½‰f
	gmBoss3BodyUpdateDirection( body_work );
}


// =======================================================================
//ƒGƒbƒOƒ}ƒ“
// =======================================================================
// =======================================================================
// gmBoss3EggChangeAction
/*!
 *	ê—pƒAƒNƒVƒ‡ƒ“•ÏX
 *
 *	@param egg_work		[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 *	@param action_id	[in] ƒAƒNƒVƒ‡ƒ“‚h‚c
 *	@param force_change	[in] ‹­§ƒtƒ‰ƒO
 */
// =======================================================================
void gmBoss3EggChangeAction( 
							GMS_BOSS3_EGG_WORK* egg_work,
							GME_BOSS3_EGG_ACT_ID action_id,
							BOOL force_change	// = FALSE
							)
{
	amAssert( action_id < GMD_BOSS3_EGG_ACT_ID_MAX );

	const GMS_BOSS3_PART_ACT_INFO* action_info = &gm_boss3_egg_act_info_tbl[action_id];

	OBS_OBJECT_WORK* obj_work_egg = GMM_BS_OBJ (egg_work );
	amAssert( obj_work_egg );

	//İ’èÏ‚İ
	if ( !force_change && 
			( (egg_work->egg_action_id == action_id) && (egg_work->flag & GMD_BOSS3_EGG_FLAG_EGG_ACT_ACTIVE) ) 
	){
		return;
	}

	//ƒAƒNƒVƒ‡ƒ“IDİ’è
	egg_work->egg_action_id = action_id;

	//İ’è’†ƒtƒ‰ƒO
	egg_work->flag |= GMD_BOSS3_EGG_FLAG_EGG_ACT_ACTIVE;

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
// gmBoss3EggRevertAction
/*!
 *	ê—pƒAƒNƒVƒ‡ƒ“I—¹
 *
 *	@param egg_work		[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss3EggRevertAction( GMS_BOSS3_EGG_WORK* egg_work )
{
	OBS_OBJECT_WORK* obj_work_egg = GMM_BS_OBJ( egg_work );
	amAssert( obj_work_egg );
	
	GMS_BOSS3_BODY_WORK* body_work = (GMS_BOSS3_BODY_WORK*)obj_work_egg->parent_obj;
	amAssert( body_work );
	OBS_OBJECT_WORK* obj_work_body = GMM_BS_OBJ( body_work );
	amAssert( obj_work_body );

	amAssert( egg_work->flag & GMD_BOSS3_EGG_FLAG_EGG_ACT_ACTIVE );
	
	//ê—pƒAƒNƒVƒ‡ƒ“‰ğœ
	egg_work->flag &= ~GMD_BOSS3_EGG_FLAG_EGG_ACT_ACTIVE;
	
	//ƒAƒNƒVƒ‡ƒ“İ’è
	const GMS_BOSS3_PART_ACT_INFO* action_info = &gm_boss3_act_info_tbl[body_work->action_id][GMD_BOSS3_PART_IDX_EGG];
	GmBsCmnSetAction(
			obj_work_egg,
			action_info->mtn_id,
			action_info->is_repeat,
			TRUE );
	
	//–{‘Ì‚ÌŒo‰ßƒtƒŒ[ƒ€‚É‡‚í‚¹‚é
	obj_work_egg->obj_3d->frame[0] = obj_work_body->obj_3d->frame[0];
}

// =======================================================================
// gmBoss3EggStateIdleInit
/*!
 *	‘Ò‹@‰Šú‰»
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss3EggStateIdleInit( GMS_BOSS3_EGG_WORK* egg_work )
{
	//ˆ—ŠÖ”İ’è
	egg_work->proc_update = gmBoss3EggStateIdleUpdate;
}

// =======================================================================
// gmBoss3EggStateIdleUpdate
/*!
 *	‘Ò‹@XV
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss3EggStateIdleUpdate( GMS_BOSS3_EGG_WORK* egg_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( egg_work );
	amAssert( obj_work );

	GMS_BOSS3_BODY_WORK* body_work = (GMS_BOSS3_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );

	//ƒqƒbƒgƒVƒOƒiƒ‹”»’è
	if ( body_work->flag & GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_HIT ){
		body_work->flag &= ~GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_HIT;

		//Î‚¢ƒV[ƒPƒ“ƒX‰Šú‰»
		gmBoss3EggStateLaughInit( egg_work );
	}
}

// =======================================================================
// gmBoss3EggStateLaughInit
/*!
 *	Î‚¢‰Šú‰»
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss3EggStateLaughInit( GMS_BOSS3_EGG_WORK* egg_work )
{
	OBS_OBJECT_WORK* egg_obj_work = GMM_BS_OBJ( egg_work );
	amAssert( egg_obj_work );
	OBS_OBJECT_WORK* body_obj_work = egg_obj_work->parent_obj;
	amAssert( body_obj_work );

	//ê—pƒAƒNƒVƒ‡ƒ“
	if ( body_obj_work->disp_flag & OBD_DISP_HFLIP ){
		gmBoss3EggChangeAction( egg_work, GMD_BOSS3_EGG_ACT_ID_LAUGH_L );
	}
	else{
		gmBoss3EggChangeAction( egg_work, GMD_BOSS3_EGG_ACT_ID_LAUGH_R );
	}
	
	//ˆ—ŠÖ”İ’è
	egg_work->proc_update = gmBoss3EggStateLaughUpdate;
}

// =======================================================================
// gmBoss3EggStateLaughUpdate
/*!
 *	Î‚¢XV
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss3EggStateLaughUpdate( GMS_BOSS3_EGG_WORK* egg_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( egg_work );
	amAssert( obj_work );

	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//ƒAƒNƒVƒ‡ƒ“‚ğŒ³‚É–ß‚·
	gmBoss3EggRevertAction( egg_work );

	//‘Ò‹@‚Ö
	gmBoss3EggStateIdleInit( egg_work );
}

// =======================================================================
// gmBoss3EggStateDamageInit
/*!
 *	ƒ_ƒ[ƒW‰Šú‰»
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss3EggStateDamageInit( GMS_BOSS3_EGG_WORK* egg_work )
{
	//ê—pƒAƒNƒVƒ‡ƒ“
	gmBoss3EggChangeAction( egg_work, GMD_BOSS3_EGG_ACT_ID_DAMAGE );

	//Š¾ƒGƒtƒFƒNƒg‰Šú‰»
	gmBoss3EffSweatInit( egg_work );

	//ˆ—ŠÖ”•ÏX
	egg_work->proc_update = gmBoss3EggStateDamageUpdate;
}


// =======================================================================
// gmBoss3EggStateDamageUpdate
/*!
 *	ƒ_ƒ[ƒWXV
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss3EggStateDamageUpdate( GMS_BOSS3_EGG_WORK* egg_work )
{
	OBS_OBJECT_WORK* obj_work = GMM_BS_OBJ( egg_work );
	amAssert( obj_work );

	//ƒ‚[ƒVƒ‡ƒ“I—¹‘Ò‚¿
	if ( !GmBsCmnIsActionEnd(obj_work) ){
		return;
	}

	//Š¾ƒGƒtƒFƒNƒgI—¹
	egg_work->flag &= ~GMD_BOSS3_EGG_FLAG_SWEAT_ACTIVE;

	//ƒAƒNƒVƒ‡ƒ“‚ğŒ³‚É–ß‚·
	gmBoss3EggRevertAction( egg_work );

	//‘Ò‹@‚Ö
	gmBoss3EggStateIdleInit( egg_work );
}

// =======================================================================
// gmBoss3EggStateEscapeInit
/*!
 *	“¦–S‰Šú‰»
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss3EggStateEscapeInit( GMS_BOSS3_EGG_WORK* egg_work )
{
	//Š¾ƒGƒtƒFƒNƒg‰Šú‰»
	if ( !(egg_work->flag & GMD_BOSS3_EGG_FLAG_SWEAT_ACTIVE) ){
		gmBoss3EffSweatInit( egg_work );
	}
	
	//ˆ—ŠÖ”İ’è
	egg_work->proc_update = gmBoss3EggStateEscapeUpdate;
}

// =======================================================================
// gmBoss3EggStateEscapeUpdate
/*!
 *	“¦–SXV
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss3EggStateEscapeUpdate( GMS_BOSS3_EGG_WORK* egg_work )
{
	UNREFERENCED_PARAMETER( egg_work );
}

// =======================================================================
// gmBoss3EggmanMainFuncWaitSetup
/*!
 *	ƒƒCƒ“ˆ—i¶¬‘Ò‚¿j
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3EggmanMainFuncWaitSetup( OBS_OBJECT_WORK* obj_work )
{
	amAssert( obj_work );

	GMS_BOSS3_BODY_WORK* body_work = (GMS_BOSS3_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );
	OBS_OBJECT_WORK* body_obj_work = GMM_BS_OBJ( body_work );

	//¶¬‘Ò‚¿
	GMS_BOSS3_MGR_WORK* mgr_work = gmBoss3MgrGetMgrWork(body_obj_work);
	amAssert( mgr_work );
	if ( !gmBoss3MgrCheckSetupComplete(mgr_work) ){
		return;
	}

	//-----------------------------------------
	//ƒƒCƒ“ˆ—
	//-----------------------------------------
	//ƒƒCƒ“ˆ—
	obj_work->ppFunc = gmBoss3EggmanMainFunc;

	//‘Ò‹@ƒV[ƒPƒ“ƒX‚É
	GMS_BOSS3_EGG_WORK* egg_work = (GMS_BOSS3_EGG_WORK*)obj_work;
	amAssert( egg_work );
	gmBoss3EggStateIdleInit( egg_work );
}

// =======================================================================
// gmBoss3EggmanMainFunc
/*!
 *	ƒƒCƒ“ˆ—
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3EggmanMainFunc( OBS_OBJECT_WORK* obj_work )
{
	amAssert( obj_work );

	GMS_BOSS3_BODY_WORK* body_work = (GMS_BOSS3_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );
	GMS_BOSS3_EGG_WORK* egg_work = (GMS_BOSS3_EGG_WORK*)obj_work;
	amAssert( egg_work );

	//İ’uƒm[ƒh‚Ö
	GmBsCmnUpdateObject3DNNStuckWithNode(
			obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS3_BODY_SNM_INDEX_BODY],
			TRUE );

	//ó‘ÔXV
	if ( egg_work->proc_update ){
		egg_work->proc_update( egg_work );
	}

	//-----------------------------------------
	//ƒVƒOƒiƒ‹ƒ`ƒFƒbƒN
	//-----------------------------------------
	//“¦–S
	if ( body_work->flag & GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_ESCAPE ){
		body_work->flag &= ~GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_ESCAPE;

		gmBoss3EggStateEscapeInit( egg_work );
	}

	//ƒ_ƒ[ƒW
	if ( body_work->flag & GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_DAMAGE ){
		body_work->flag &= ~GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_DAMAGE;

		gmBoss3EggStateDamageInit( egg_work );
	}

	//•‚±‚°
	if ( body_work->flag & GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_BURNT ){
		body_work->flag &= ~GMD_BOSS3_BODY_FLAG_SIGNAL_B2E_BURNT;
		gmBoss3ChangeTextureBurnt( obj_work );
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
}


// ==========================================================================
//ƒGƒtƒFƒNƒg
// ==========================================================================

// =======================================================================
// gmBoss3EffDamageInit
/*!
 *	ƒ_ƒ[ƒWƒGƒtƒFƒNƒg‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3EffDamageInit( GMS_BOSS3_BODY_WORK* body_work )
{
	OBS_OBJECT_WORK* parent_obj	= GMM_BS_OBJ( body_work );
	amAssert( parent_obj );

	GMS_EFFECT_3DES_WORK* efct_work = GmEfctBossCmnEsCreate(
			parent_obj, 
			GME_EFCT_BOSS_CMN_IDX_BOSS_DM );
	OBS_OBJECT_WORK* effect_obj_work = GMM_BS_OBJ( efct_work );
	amAssert( effect_obj_work );

	effect_obj_work->pos.z += GMD_BOSS3_EFFECT_BOMB_OFFSET_Z;
}

// =======================================================================
// gmBoss3EffBombInitCreate
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
void gmBoss3EffBombsInit( 
						 GMS_BOSS3_EFF_BOMB_WORK* bomb_work,
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
// gmBoss3EffBombsUpdate
/*!
 *	”š”­ƒGƒtƒFƒNƒgŒQXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3EffBombsUpdate( GMS_BOSS3_EFF_BOMB_WORK* bomb_work )
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
	obj_work->pos.z	= parent_obj_work->pos.z + GMD_BOSS3_EFFECT_BOMB_OFFSET_Z;	//eÀ•W‚©‚çƒIƒtƒZƒbƒg


	//¶¬ŠÔŠu
	fx32 interval = (fx32)(bomb_work->interval_max - bomb_work->interval_min);
	fx32 rand_interval = AkMathRandFx();
	Uint32 rand_ofst = (Uint32)((rand_interval*interval) >> FX32_SHIFT);
	bomb_work->interval_timer = bomb_work->interval_min + rand_ofst;
}

// =======================================================================
// gmBoss3EffAfterburnerRequestCreate
/*!
 *	ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒgì¬—v‹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3EffAfterburnerRequestCreate( GMS_BOSS3_BODY_WORK* body_work )
{
	body_work->flag |= GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_AFTERBURNER;
}

// =======================================================================
// gmBoss3EffAfterburnerRequestDelete
/*!
 *	ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒgíœ—v‹
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3EffAfterburnerRequestDelete( GMS_BOSS3_BODY_WORK* body_work )
{
	body_work->flag &= ~GMD_BOSS3_BODY_FLAG_AFTERBURNER_ACTIVE;
	body_work->flag &= ~GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_AFTERBURNER;
}

// =======================================================================
// gmBoss3EffAfterburnerInit
/*!
 *	ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg‚ğ‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3EffAfterburnerInit( GMS_BOSS3_BODY_WORK* body_work )
{
	//¶¬Ï‚İ
	if ( body_work->flag & GMD_BOSS3_BODY_FLAG_AFTERBURNER_ACTIVE ){
		return;
	}

	//ƒtƒ‰ƒOİ’è
	body_work->flag &= ~GMD_BOSS3_BODY_FLAG_SIGNAL_B2B_AFTERBURNER;
	body_work->flag |= GMD_BOSS3_BODY_FLAG_AFTERBURNER_ACTIVE;

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
		GMD_BOSS3_EFFECT_AFTERBURNER_DIST_OFFSET_Z );

	//ƒƒCƒ“ˆ—
	OBS_OBJECT_WORK* effct_obj_work = GMM_BS_OBJ( efct_work );
	amAssert( effct_obj_work );
	effct_obj_work->ppFunc = gmBoss3EffAfterburnerMainFunc;
}

// =======================================================================
// gmBoss3EffAfterburnerInit
/*!
 *	ƒAƒtƒ^ƒo[ƒiƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3EffAfterburnerMainFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS3_BODY_WORK* body_work = (GMS_BOSS3_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );
	MTM_ASSERT( body_work->snm_work.reg_node_max );


	//I—¹ŠÄ‹
	if ( obj_work->disp_flag & OBD_DISP_END ){
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}

	//—LŒøƒtƒ‰ƒOŠÄ‹
	if ( !(body_work->flag & GMD_BOSS3_BODY_FLAG_AFTERBURNER_ACTIVE) ){
		ObjDrawKillAction3DES( obj_work );
	}

	// –{‘ÌSNMƒ}ƒgƒŠƒNƒX‚Å‚­‚Á‚Â‚¯‚é
	GmBsCmnUpdateObject3DESStuckWithNode(
			obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS3_BODY_SNM_INDEX_BODY],
			TRUE );
}

// =======================================================================
// gmBoss3EffAfterburnerSmokeInit
/*!
 *	ƒAƒtƒ^ƒo[ƒi‰ŒƒGƒtƒFƒNƒg‚ğ‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3EffAfterburnerSmokeInit( GMS_BOSS3_BODY_WORK* body_work )
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
		GMD_BOSS3_EFFECT_AFTERBURNER_SMOKE_DIST_OFFSET_Z );

	//ƒƒCƒ“ˆ—
	OBS_OBJECT_WORK* effct_obj_work = GMM_BS_OBJ( efct_work );
	amAssert( effct_obj_work );
	effct_obj_work->ppFunc = gmBoss3EffAfterburnerSmokeMainFunc;
}

// =======================================================================
// gmBoss3EffAfterburnerSmokeMainFunc
/*!
 *	ƒAƒtƒ^ƒo[ƒi‰ŒƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3EffAfterburnerSmokeMainFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS3_BODY_WORK* body_work = (GMS_BOSS3_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );
	MTM_ASSERT( body_work->snm_work.reg_node_max );

	// –{‘ÌSNMƒ}ƒgƒŠƒNƒX‚Å‚­‚Á‚Â‚¯‚é
	GmBsCmnUpdateObject3DESStuckWithNode(
			obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS3_BODY_SNM_INDEX_BODY],
			TRUE );
}

// =======================================================================
// gmBoss3EffBodySmokeInit
/*!
 *	–{‘Ì‰ŒƒGƒtƒFƒNƒg‚ğ‰Šú‰»
 *
 *	@param body_work	[io] –{‘Ìƒ[ƒN
 */
// =======================================================================
void gmBoss3EffBodySmokeInit( GMS_BOSS3_BODY_WORK* body_work )
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
		GMD_BOSS3_EFFECT_BODY_SMOKE_DIST_OFFSET_Z );

	//ƒƒCƒ“ˆ—
	OBS_OBJECT_WORK* effct_obj_work = GMM_BS_OBJ( efct_work );
	amAssert( effct_obj_work );
	effct_obj_work->ppFunc = gmBoss3EffBodySmokeMainFunc;
}

// =======================================================================
// gmBoss3EffBodySmokeMainFunc
/*!
 *	–{‘Ì‰ŒƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3EffBodySmokeMainFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS3_BODY_WORK* body_work = (GMS_BOSS3_BODY_WORK*)obj_work->parent_obj;
	amAssert( body_work );
	MTM_ASSERT( body_work->snm_work.reg_node_max );

	// –{‘ÌSNMƒ}ƒgƒŠƒNƒX‚Å‚­‚Á‚Â‚¯‚éiƒAƒtƒ^ƒo[ƒi‚Æ“¯‚¶À•Wj
	GmBsCmnUpdateObject3DESStuckWithNode(
			obj_work,
			&body_work->snm_work,
			body_work->snm_reg_id[GMD_BOSS3_BODY_SNM_INDEX_BODY],
			TRUE );
}

// =======================================================================
// gmBoss3EffSweatInit
/*!
 *	Š¾ƒGƒtƒFƒNƒg‚ğ‰Šú‰»
 *
 *	@param egg_work	[io] ƒGƒbƒOƒ}ƒ“ƒ[ƒN
 */
// =======================================================================
void gmBoss3EffSweatInit( GMS_BOSS3_EGG_WORK* egg_work )
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
			GMD_BOSS3_EFFECT_SWEAT_DIST_OFFSET_Y, 
			0 );

	//ƒƒCƒ“ˆ—
	OBS_OBJECT_WORK* effct_obj_work = GMM_BS_OBJ( efct_work );
	amAssert( effct_obj_work );
	effct_obj_work->ppFunc = gmBoss3EffSweatMainFunc;

	//Š¾Às’†
	egg_work->flag |= GMD_BOSS3_EGG_FLAG_SWEAT_ACTIVE;
}

// =======================================================================
// gmBoss3EffSweatMainFunc
/*!
 *	Š¾ƒGƒtƒFƒNƒg‚ğXV
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// =======================================================================
void gmBoss3EffSweatMainFunc( OBS_OBJECT_WORK* obj_work )
{
	GMS_BOSS3_EGG_WORK* egg_work = (GMS_BOSS3_EGG_WORK*)obj_work->parent_obj;
	amAssert( egg_work );
	
	if ( !(egg_work->flag & GMD_BOSS3_EGG_FLAG_SWEAT_ACTIVE) ){
		ObjDrawKillAction3DES( obj_work );
	}
	
	if ( obj_work->disp_flag & OBD_DISP_END ){
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
