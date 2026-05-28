// ===========================================================================
/*!
	@file	gmStartDemo.cpp
	@brief	

	@author	hanaoka
				Copyright(c) 2009 Dimps
	$Id: gmStartDemo.cpp 20 2011-04-22 12:46:46Z thamada $
	$Date: 2011-04-22 21:46:46 +0900 (Èáë, 22 4 2011) $
	
 */
// ===========================================================================
/*
 *
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"

#include "gmStartDemo.h"
#include "../library/ao/include/aoTexture.h"
#include "../library/ao/include/aoAction.h"

#include "gs.h"
#include "gsMainSys.h"
#include "gmMain.h"
#include "izFade.h"
#include "gmTask.h"

#include "gmGameDat.h"
#include "gmMainDat.h"
#include "objObject.h"
#include "gmCockpit.h"

#include "gmEnemy.h"
#include "gmPlySeq.h"
#include "gmPlySeqGmk.h"
#include "gmStartMsg.h"

// ÉfÅ[É^ÉwÉbÉ_
#include "common/arc/CPIT_MAIN.HMB"
#include "common/ace/G_START.HMA"
#include "common/ace/G_START_US.HMA"


//----- Definitions ---------------------------------------------------------


#define GMD_START_DEMO_INVLID_ID	(-1)	//ñ≥å¯
#define GMD_START_DEMO_ACT_NO_NUM	(4)		//ÉAÉNÉgêî
#define GMD_START_DEMO_FADEIN_TIME	(30.0f)	//ÉtÉFÅ[ÉhÉCÉì

//ÉtÉâÉO
#define GMD_START_DEMO_FLAG_EXIT	(1 << 0)		/// èIóπÉtÉâÉO

//ìoò^â¬î\ÉRÉ}ÉìÉhêîÇ…Ç±ÇÃêîÇæÇØãÛÇ´Ç™Ç≈Ç´ÇΩÇÁÉçÅ[ÉhäJén
#define GMD_START_DEMO_BUILD_REG_ALLOWANCE_NUM		(64)	

//ÉfÅ[É^ID
enum GME_START_DEMO_DATA_TYPE {
	GMD_START_DEMO_DATA_TYPE_CMN = 0,	//ã§í 
	GMD_START_DEMO_DATA_TYPE_LANG,	//åæåÍï 

	GMD_START_DEMO_DATA_TYPE_NUM
};

//ÉAÉNÉVÉáÉìÉCÉìÉfÉNÉXÅiÉ]Å[Éìã§í Åj
enum GME_START_DEMO_ACTION_INDEX_CMN{
	GMD_START_DEMO_ACTION_INDEX_CMN_BG_WHITE = 0,		//îwåiÅiîíÅj
	GMD_START_DEMO_ACTION_INDEX_CMN_BG_BLUE,			//îwåiÅiê¬Åj
	GMD_START_DEMO_ACTION_INDEX_CMN_BG_RED,				//îwåiÅiê‘Åj

	GMD_START_DEMO_ACTION_INDEX_CMN_TEX_ZONE,			//É]Å[Éì

	GMD_START_DEMO_ACTION_INDEX_CMN_NUM
};

//ÉAÉNÉVÉáÉìÉCÉìÉfÉNÉXÅiÉ]Å[Éìï Åj
enum GME_START_DEMO_ACTION_INDEX_ZONE{
	GMD_START_DEMO_ACTION_INDEX_ZONE_TEX_ZNAME,			//É]Å[Éìñºï∂éö

	GMD_START_DEMO_ACTION_INDEX_ZONE_NUM
};

//ÉAÉNÉVÉáÉìÉCÉìÉfÉNÉXÅiÉAÉNÉgï Åj
enum GME_START_DEMO_ACTION_INDEX_ACT{
	GMD_START_DEMO_ACTION_INDEX_ACT_NUMBER,				//êîéö
	GMD_START_DEMO_ACTION_INDEX_ACT_TEX_ACT,			//ÉAÉNÉg

	GMD_START_DEMO_ACTION_INDEX_ACT_NUM
};

//ÉfÅ[É^ä«óù
typedef struct tag_GMS_START_DEMO_DATA {	
	AOS_TEXTURE aos_texture[GMD_START_DEMO_DATA_TYPE_NUM];
	void* demo_amb[GMD_START_DEMO_DATA_TYPE_NUM];

	BOOL flag_regist;	
}GMS_START_DEMO_DATA;

//ÉèÅ[ÉN
typedef struct tag_GMS_START_DEMO_WORK	GMS_START_DEMO_WORK;
struct tag_GMS_START_DEMO_WORK {	
	u32 counter;
	u32 flag;
	GMS_COCKPIT_2D_WORK* action_obj_work_cmn[GMD_START_DEMO_ACTION_INDEX_CMN_NUM];		//ÉAÉNÉVÉáÉìÅiÉ]Å[Éìã§í Åj
	GMS_COCKPIT_2D_WORK* action_obj_work_zone[GMD_START_DEMO_ACTION_INDEX_ZONE_NUM];	//ÉAÉNÉVÉáÉìÅiÉ]Å[Éìï Åj
	GMS_COCKPIT_2D_WORK* action_obj_work_act[GMD_START_DEMO_ACTION_INDEX_ACT_NUM];		//ÉAÉNÉVÉáÉìÅiÉAÉNÉgï Åj
	GMS_COCKPIT_2D_WORK* action_obj_work_message;										//ÉAÉNÉVÉáÉìÅiÉÅÉbÉZÅ[ÉWÅj
	void (*update)(tag_GMS_START_DEMO_WORK *);		//çXêVä÷êî
};

//ä«óù
typedef struct tag_GMS_START_DEMO_MGR {	
	MTS_TASK_TCB* main_tcb;
}GMS_START_DEMO_MGR;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

// ==========================================================================
//ÉfÅ[É^ä«óù
// ==========================================================================
static void gmStartDemoDataInit( void );
static void gmStartDemoDataFlush( void );
static GMS_START_DEMO_DATA* gmStartDemoDataGetInfo( void );
static BOOL gmStartDemoDataCheckLoading( void );
static BOOL gmStartDemoDataCheckRelease( void );

// ==========================================================================
//ÉfÉÇä«óù
// ==========================================================================
static GMS_START_DEMO_MGR* gmStartDemoMgrGetInfo( void );
static void gmStartDemoInit( void );
static void gmStartDemoExit( void );
static void gmStartDemoRequestExit( void );
static void gmStartDemoProcMain( MTS_TASK_TCB* tcb );
static void gmStartDemoSetGameFlag( u32 flag );
static void gmStartDemoClearGameFlag( u32 flag );

// ==========================================================================
//ÉAÉNÉVÉáÉìä«óù
// ==========================================================================
static void gmStartDemo2DActionCreate( GMS_START_DEMO_WORK* work );
static GMS_COCKPIT_2D_WORK* gmStartDemo2DActionCreate( 
							   const char* tcb_name,
							   AOS_TEXTURE* aos_texture,
							   s32 ama_id,
							   s32 action_id,
							   BOOL node_flag );


// ==========================================================================
//ÉÅÉCÉìèàóù
// ==========================================================================
static void gmStartDemoProcFade( GMS_START_DEMO_WORK* work );
static void gmStartDemoProcIn( GMS_START_DEMO_WORK* work );
static void gmStartDemoProcWait( GMS_START_DEMO_WORK* work );
static void gmStartDemoProcOut( GMS_START_DEMO_WORK* work );
static void gmStartDemoProcEnd( GMS_START_DEMO_WORK* work );
//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

static GMS_START_DEMO_DATA g_start_demo_data_real;
static GMS_START_DEMO_DATA* g_start_demo_data = NULL;

static GMS_START_DEMO_MGR g_start_demo_mgr_real;
static GMS_START_DEMO_MGR* g_start_demo_mgr = NULL;

//AMBÉfÅ[É^ID
static const s32 g_gm_start_demo_data_amb_id[GSD_LANGUAGE_NUM][GMD_START_DEMO_DATA_TYPE_NUM] = {
	{IDB_CPIT_MAIN_G_START_AMB,	IDB_CPIT_MAIN_G_START_JP_AMB,},	//ì˙ñ{åÍ
	{IDB_CPIT_MAIN_G_START_AMB,	IDB_CPIT_MAIN_G_START_US_AMB,},	//âpåÍ
	{IDB_CPIT_MAIN_G_START_AMB,	IDB_CPIT_MAIN_G_START_FR_AMB,},	//ÉtÉâÉìÉXåÍ
	{IDB_CPIT_MAIN_G_START_AMB,	IDB_CPIT_MAIN_G_START_IT_AMB,},	//ÉCÉ^ÉäÉAåÍ
	{IDB_CPIT_MAIN_G_START_AMB,	IDB_CPIT_MAIN_G_START_GE_AMB,},	//ÉhÉCÉcåÍ
	{IDB_CPIT_MAIN_G_START_AMB,	IDB_CPIT_MAIN_G_START_SP_AMB,},	//ÉXÉyÉCÉìåÍ
};
//AMAÉfÅ[É^ID
static const s32 g_gm_start_demo_data_ama_id[GSD_LANGUAGE_NUM][GMD_START_DEMO_DATA_TYPE_NUM] = {
	{IDB_CPIT_MAIN_G_START_AMA,	IDB_CPIT_MAIN_G_START_JP_AMA,},	//ì˙ñ{åÍ
	{IDB_CPIT_MAIN_G_START_AMA,	IDB_CPIT_MAIN_G_START_US_AMA,},	//âpåÍ
	{IDB_CPIT_MAIN_G_START_AMA,	IDB_CPIT_MAIN_G_START_FR_AMA,},	//ÉtÉâÉìÉXåÍ
	{IDB_CPIT_MAIN_G_START_AMA,	IDB_CPIT_MAIN_G_START_IT_AMA,},	//ÉCÉ^ÉäÉAåÍ
	{IDB_CPIT_MAIN_G_START_AMA,	IDB_CPIT_MAIN_G_START_GE_AMA,},	//ÉhÉCÉcåÍ
	{IDB_CPIT_MAIN_G_START_AMA,	IDB_CPIT_MAIN_G_START_SP_AMA,},	//ÉXÉyÉCÉìåÍ
};

//------------------------------------------------------
//ã§í 
//------------------------------------------------------
//ÉAÉNÉVÉáÉìÇ™ëÆÇµÇƒÇ¢ÇÈÉfÅ[É^ÉCÉìÉfÉNÉX
static const GME_START_DEMO_DATA_TYPE g_gm_start_demo_data_type_cmn[GMD_START_DEMO_ACTION_INDEX_CMN_NUM] = {
	GMD_START_DEMO_DATA_TYPE_CMN,		//îwåiÅiîíÅj
	GMD_START_DEMO_DATA_TYPE_CMN,		//îwåiÅiê¬Åj
	GMD_START_DEMO_DATA_TYPE_CMN,		//îwåiÅiê‘Åj

	GMD_START_DEMO_DATA_TYPE_CMN,	//É]Å[Éì
};


//ÉAÉNÉVÉáÉìID
static const s32 g_gm_start_demo_action_id_cmn[GMD_START_DEMO_ACTION_INDEX_CMN_NUM] = {
	IDA_G_START_ACT_BG_WHITE,			//îwåiÅiîíÅj
	IDA_G_START_ROOT_NODE_BG_BLUE,		//îwåiÅiê¬Åj
	IDA_G_START_ROOT_NODE_BG_RED,		//îwåiÅiê‘Åj

	IDA_G_START_ACT_TEX_ZONE,		//É]Å[Éì
}; 


//ÉAÉNÉVÉáÉìÇÃï\é¶ñº
static const char* g_gm_start_demo_action_name_cmn[GMD_START_DEMO_ACTION_INDEX_CMN_NUM] = {
	"START_DEMO_BG_WHITE",			//îwåiÅiîíÅj
	"START_DEMO_BG_BLUE",			//îwåiÅiê¬Åj
	"START_DEMO_BG_RED",			//îwåiÅiê‘Åj

	"START_DEMO_TEX_ZONE",			//É]Å[Éì
}; 

//ÉmÅ[ÉhÉ^ÉCÉvÉtÉâÉO
static const BOOL g_gm_start_demo_action_node_flag_cmn[GMD_START_DEMO_ACTION_INDEX_CMN_NUM] = {
	FALSE,	//îwåiÅiîíÅj
	TRUE,	//îwåiÅiê¬Åj
	TRUE,	//îwåiÅiê‘Åj

	FALSE,	//É]Å[Éì
};

//------------------------------------------------------
//É]Å[Éìï 
//------------------------------------------------------
//ÉAÉNÉVÉáÉìÇ™ëÆÇµÇƒÇ¢ÇÈÉfÅ[É^ÉCÉìÉfÉNÉX
static const GME_START_DEMO_DATA_TYPE g_gm_start_demo_data_type_zone[GMD_START_DEMO_ACTION_INDEX_ZONE_NUM] = {
	GMD_START_DEMO_DATA_TYPE_CMN,	//É]Å[Éìñºï∂éö
};


//ÉAÉNÉVÉáÉìID
static const s32 g_gm_start_demo_action_id_zone[GSD_MAIN_ZONE_TYPE_MAX][GMD_START_DEMO_ACTION_INDEX_ZONE_NUM] = {	
	{
		IDA_G_START_ROOT_NODE_ZONE1,		//É]Å[Éìñºï∂éö
	},
	{
		IDA_G_START_ROOT_NODE_ZONE2,		//É]Å[Éìñºï∂éö
	},
	{
		IDA_G_START_ROOT_NODE_ZONE3,		//É]Å[Éìñºï∂éö
	},
	{
		IDA_G_START_ROOT_NODE_ZONE4,		//É]Å[Éìñºï∂éö
	},
	{
		IDA_G_START_ROOT_NODE_FINAL,		//É]Å[Éìñºï∂éö
	},
	{	// SpecialStage
		NULL,		//É]Å[Éìñºï∂éö
	},
}; 

//ÉAÉNÉVÉáÉìÇÃï\é¶ñº
static const char* g_gm_start_demo_action_name_zone[GMD_START_DEMO_ACTION_INDEX_ZONE_NUM] = {
	"START_DEMO_TEX_ZNAME",			//É]Å[Éìñºï∂éö
}; 

//ÉmÅ[ÉhÉ^ÉCÉvÉtÉâÉO
static const BOOL g_gm_start_demo_action_node_flag_zone[GMD_START_DEMO_ACTION_INDEX_ZONE_NUM] = {
	TRUE,	//É]Å[Éìñºï∂éö
};

//------------------------------------------------------
//ÉAÉNÉgï 
//------------------------------------------------------
//ÉAÉNÉVÉáÉìÇ™ëÆÇµÇƒÇ¢ÇÈÉfÅ[É^ÉCÉìÉfÉNÉX
static const GME_START_DEMO_DATA_TYPE g_gm_start_demo_data_type_act[GMD_START_DEMO_ACTION_INDEX_ACT_NUM] = {
	GMD_START_DEMO_DATA_TYPE_CMN,	//ÉAÉNÉgêî
	GMD_START_DEMO_DATA_TYPE_LANG,	//ÉAÉNÉg
};

//ÉAÉNÉgî‘çÜ
const s32 g_gm_start_demo_act_no[GSD_MAIN_STAGE_ID_MAX] = {
	0, 1, 2, 3,			// ZONE1	
	0, 1, 2, 3,			// ZONE2	
	0, 1, 2, 3,			// ZONE3	
	0, 1, 2, 3,			// ZONE4	
	0, 1, 2, 3,	3,		// ZONEFINAL
	0, 0, 0, 0, 0, 0, 0,// SPECIAL STAGE
	0,					// ÉGÉìÉfÉBÉìÉOÉXÉeÅ[ÉW
};

//ÉAÉNÉVÉáÉìID
static const s32 g_gm_start_demo_action_id_act[GMD_START_DEMO_ACT_NO_NUM][GMD_START_DEMO_ACTION_INDEX_ACT_NUM] = {
	{
		IDA_G_START_ROOT_NODE_NUM_1,				//É]Å[Éìêî
		IDA_G_START_US_ROOT_NODE_TEX_ACT,			//ÉAÉNÉg
	},
	{
		IDA_G_START_ROOT_NODE_NUM_2,				//É]Å[Éìêî
		IDA_G_START_US_ROOT_NODE_TEX_ACT,			//ÉAÉNÉg
	},
	{	
		IDA_G_START_ROOT_NODE_NUM_3,				//É]Å[Éìêî
		IDA_G_START_US_ROOT_NODE_TEX_ACT,			//ÉAÉNÉg
	},
	{
		GMD_START_DEMO_INVLID_ID,					//É]Å[Éìêî
		IDA_G_START_US_ROOT_NODE_TEX_BOSS,			//ÉAÉNÉg
	},
};

//------------------------------------------------------
//ÉÅÉbÉZÅ[ÉW
//------------------------------------------------------
static const s32 g_gm_start_demo_action_id_message[GSD_MAIN_ZONE_TYPE_MAX][GMD_START_DEMO_ACT_NO_NUM] = {
	// ZONE1
	{
		IDA_G_START_US_ROOT_NODE_Z1_ACT1,		
		IDA_G_START_US_ROOT_NODE_Z1_ACT2,	
		IDA_G_START_US_ROOT_NODE_Z1_ACT3,	
		IDA_G_START_US_ROOT_NODE_Z1_BOSS,	
	},
	// ZONE2
	{
		IDA_G_START_US_ROOT_NODE_Z2_ACT1,		
		IDA_G_START_US_ROOT_NODE_Z2_ACT2,	
		IDA_G_START_US_ROOT_NODE_Z2_ACT3,	
		IDA_G_START_US_ROOT_NODE_Z2_BOSS,	
	},
	// ZONE3
	{
		IDA_G_START_US_ROOT_NODE_Z3_ACT1,		
		IDA_G_START_US_ROOT_NODE_Z3_ACT2,	
		IDA_G_START_US_ROOT_NODE_Z3_ACT3,	
		IDA_G_START_US_ROOT_NODE_Z3_BOSS,	
	},
	// ZONE4
	{
		IDA_G_START_US_ROOT_NODE_Z4_ACT1,		
		IDA_G_START_US_ROOT_NODE_Z4_ACT2,	
		IDA_G_START_US_ROOT_NODE_Z4_ACT3,	
		IDA_G_START_US_ROOT_NODE_Z4_BOSS,	
	},
	// ZONEFINAL
	{
		IDA_G_START_US_ROOT_NODE_FINAL_BOSS,		
		IDA_G_START_US_ROOT_NODE_FINAL_BOSS,	
		IDA_G_START_US_ROOT_NODE_FINAL_BOSS,	
		IDA_G_START_US_ROOT_NODE_FINAL_BOSS,
	},
	// SPECIAL STAGE	
	{
		IDA_G_START_US_ROOT_NODE_FINAL_BOSS,		
		IDA_G_START_US_ROOT_NODE_FINAL_BOSS,	
		IDA_G_START_US_ROOT_NODE_FINAL_BOSS,	
		IDA_G_START_US_ROOT_NODE_FINAL_BOSS,	
	},
};


//ÉAÉNÉVÉáÉìÇÃï\é¶ñº
static const char* g_gm_start_demo_action_name_act[GMD_START_DEMO_ACTION_INDEX_ACT_NUM] = {
	"START_DEMO_NUM",				//ÉAÉNÉgêî
	"START_DEMO_TEX_ACT",			//ÉAÉNÉg
}; 
static const char* g_gm_start_demo_action_name_message = "START_DEMO_TEX_MESS";		//çïë—ÉÅÉbÉZÅ[ÉW

//ÉmÅ[ÉhÉ^ÉCÉvÉtÉâÉO
static const BOOL g_gm_start_demo_action_node_flag_act[GMD_START_DEMO_ACTION_INDEX_ACT_NUM] = {
	TRUE,	//É]Å[Éìêî
	TRUE,	//ÉAÉNÉg
};

//----- Global Functions ----------------------------------------------------

// ==========================================================================
// GmStartDemoBuild
/*!
 *	ÉXÉ^Å[ÉgÉfÉÇÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmStartDemoBuild( void )
{

	// ÉfÅ[É^ä«óùèâä˙âª
	gmStartDemoDataInit();
	GMS_START_DEMO_DATA* start_demo_data = gmStartDemoDataGetInfo();
	amAssert( start_demo_data );
	
	for ( s32 i = 0; GMD_START_DEMO_DATA_TYPE_NUM > i; ++i ){
		GSE_LANGUAGE lang = GsEnvGetLanguage();
		// AMBÉtÉ@ÉCÉãÉçÅ[Éh
		start_demo_data->demo_amb[i] = (AMS_FS*)ObjDataLoadAmbIndex(
			NULL,
			g_gm_start_demo_data_amb_id[lang][i],
			GmGameDatGetCockpitData() );
		
		// ÉAÉhÉåÉXïœä∑
		amConvertAddress( start_demo_data->demo_amb[i] );
	}
	
	start_demo_data->flag_regist = FALSE;
}

// ==========================================================================
// GmStartDemoBuildCheck
/*!
 * ì«Ç›çûÇ›ë“ÇøÉ`ÉFÉbÉN
 *
 * @return TRUEÅFì«Ç›çûÇ›èIóπ FALSEÅFì«Ç›çûÇ›ë“Çø
 */
// ==========================================================================
BOOL GmStartDemoBuildCheck(void)
{
	GMS_START_DEMO_DATA* start_demo_data = gmStartDemoDataGetInfo();
	amAssert( start_demo_data );

	if ( !start_demo_data->flag_regist ){
		//ãÛÇ´Ç™Ç»Ç¢
		if (GsMainSysGetDisplayListRegistNum() >= AMD_REGISTLIST_NUM - GMD_START_DEMO_BUILD_REG_ALLOWANCE_NUM) {
			return FALSE;
		}

		// ÉeÉNÉXÉ`ÉÉç\íz
		for ( s32 i = 0; GMD_START_DEMO_DATA_TYPE_NUM > i; ++i ){
			AoTexBuild( &start_demo_data->aos_texture[i], start_demo_data->demo_amb[i] );
			AoTexLoad( &start_demo_data->aos_texture[i] );
		}
		start_demo_data->flag_regist = TRUE;
	}
	
	return gmStartDemoDataCheckLoading();
}

// ==========================================================================
// GmStartDemoFlush
/*!
 *	ÉXÉ^Å[ÉgÉfÉÇÉfÅ[É^âï˙
 *
 */
// ==========================================================================
void GmStartDemoFlush(void)
{
	GMS_START_DEMO_DATA* start_demo_data = gmStartDemoDataGetInfo();
	amAssert( start_demo_data );

	// ÉeÉNÉXÉ`ÉÉâï˙
	for ( s32 i = 0; GMD_START_DEMO_DATA_TYPE_NUM > i; ++i ){
		AoTexRelease( &start_demo_data->aos_texture[i] );
	}
}

// ==========================================================================
// GmStartDemoFlushCheck
/*!
 *	ÉXÉ^Å[ÉgÉfÉÇÉfÅ[É^âï˙É`ÉFÉbÉN
 *
 * @return TRUEÅFâï˙èIóπ FALSEÅFâï˙ë“Çø
 */
// ==========================================================================
BOOL GmStartDemoFlushCheck( void )
{
	if ( !gmStartDemoDataCheckRelease() ){
		return FALSE;
	}
	
	gmStartDemoDataFlush();
	return TRUE;
}

// ==========================================================================
// GmStartDemoStart
/*!
	ÉXÉ^Å[ÉgÉfÉÇäJén
 */
// ==========================================================================
void GmStartDemoStart(void)
{
	gmStartDemoInit();

	gmStartDemoSetGameFlag( GMD_GAME_FLAG_START_DEMO );
}


// ==========================================================================
// GmStartDemoExit
/*!
	ÉXÉ^Å[ÉgÉfÉÇèIóπ
 */
// ==========================================================================
void GmStartDemoExit(void)
{
	gmStartDemoExit();
}


//----- Local Functions -----------------------------------------------------



// ==========================================================================
//ÉfÅ[É^ä«óù
// ==========================================================================


// ==========================================================================
// gmStartDemoDataInit
/*!
 * ÉfÅ[É^ä«óùÇèâä˙âª
 *
 */
// ==========================================================================
void gmStartDemoDataInit( void )
{
	amAssert(!g_start_demo_data);

	amZeroMemory(&g_start_demo_data_real, sizeof(GMS_START_DEMO_DATA));
	g_start_demo_data = &g_start_demo_data_real;
}

// ==========================================================================
// gmStartDemoDataFlush
/*!
 * ÉfÅ[É^ä«óùÇâï˙
 *
 */
// ==========================================================================
void gmStartDemoDataFlush( void )
{
	if ( g_start_demo_data ){
		g_start_demo_data = NULL;
	}
}

// ==========================================================================
// gmStartDemoDataGetInfo
/*!
 * ÉfÅ[É^ä«óùÇéÊìæ
 *
 * @return  ÉfÅ[É^ä«óù
 */
// ==========================================================================
GMS_START_DEMO_DATA* gmStartDemoDataGetInfo( void )
{
	return g_start_demo_data;
}

// ==========================================================================
// gmStartDemoDataCheckLoading
/*!
 * ì«Ç›çûÇ›ë“Çø
 *
 * @return TRUEÅFì«Ç›çûÇ›èIóπ FALSEÅFì«Ç›çûÇ›ë“Çø
 */
// ==========================================================================
BOOL gmStartDemoDataCheckLoading( void )
{
	s32 result = 0;
	GMS_START_DEMO_DATA* start_demo_data = gmStartDemoDataGetInfo();
	amAssert( start_demo_data );

	for ( s32 i = 0; GMD_START_DEMO_DATA_TYPE_NUM > i; ++i ){
		if ( TRUE == AoTexIsLoaded(&start_demo_data->aos_texture[i]) ){
			result |= 1 << i;
		}
	}

	if ( 3 != result ){
		return FALSE;
	}
	return TRUE;
}

// ==========================================================================
// gmStartDemoDataCheckRelease
/*!
 * ÉeÉNÉXÉ`ÉÉâï˙ë“Çø
 *
 * @return TRUEÅFâï˙èIóπ FALSEÅFâï˙ë“Çø
 */
// ==========================================================================
BOOL gmStartDemoDataCheckRelease( void )
{
	s32 result = 0;
	GMS_START_DEMO_DATA* start_demo_data = gmStartDemoDataGetInfo();

	if ( !start_demo_data ){
		return TRUE;
	}

	for ( s32 i = 0; GMD_START_DEMO_DATA_TYPE_NUM > i; ++i ){
		if ( TRUE == AoTexIsReleased(&start_demo_data->aos_texture[i]) ){
			result |= 1 << i;
		}
	}

	if ( 3 != result ){
		return FALSE;
	}
	return TRUE;
}



// ==========================================================================
//ÉfÉÇä«óù
// ==========================================================================

// ==========================================================================
// gmStartDemoMgrGetInfo
/*!
 * ÉfÅ[É^ä«óùÇéÊìæ
 *
 * @return  ÉfÅ[É^ä«óù
 */
// ==========================================================================
GMS_START_DEMO_MGR* gmStartDemoMgrGetInfo( void )
{
	return g_start_demo_mgr;
}

// ==========================================================================
// gmStartDemoInit
/*!
 * ÉfÅ[É^ä«óùÇèâä˙âª
 */
// ==========================================================================
void gmStartDemoInit( void )
{
	//ä«óùèâä˙âª
	amAssert( !g_start_demo_mgr );

	amZeroMemory( &g_start_demo_mgr_real, sizeof(GMS_START_DEMO_MGR) );
	g_start_demo_mgr = &g_start_demo_mgr_real;

	// ÉÅÉCÉìÉ^ÉXÉNçÏê¨
	g_start_demo_mgr->main_tcb = MTM_TASK_MAKE_TCB(
			gmStartDemoProcMain,
			NULL,
			0,
			GMD_TASK_PAUSELEVEL_DEF,
			GMD_TASK_PRIO_STARTDEMO,
			GMD_TASK_GROUP_STARTDEMO,
			sizeof( GMS_START_DEMO_WORK ),
			"START_DEMO_MAIN" );
	amAssert( g_start_demo_mgr->main_tcb ); 

	//ÉèÅ[ÉNê›íË
	GMS_START_DEMO_WORK* work = (GMS_START_DEMO_WORK*)mtTaskGetTcbWork( g_start_demo_mgr->main_tcb );
	amAssert( work );
	work->counter = 0;
	work->update = gmStartDemoProcFade;

	// 2DÉAÉNÉVÉáÉìçÏê¨
	gmStartDemo2DActionCreate( work );	

	//ÉvÉåÉCÉÑÉVÅ[ÉPÉìÉX
	GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	GmPlySeqInitDemoFw( player_work );

	
		
	// ÉtÉFÅ[ÉhèàóùäJén
	IzFadeInitEasy(
			IZE_FADE_SET_TYPE_TAKEOEVER,
			IZE_FADE_TYPE_WHITE_FADEIN,
			GMD_START_DEMO_FADEIN_TIME );
}

// ==========================================================================
// gmStartDemoRequestExit
/*!
 * èIóπóvãÅ
 */
// ==========================================================================
void gmStartDemoRequestExit( void )
{
	GMS_START_DEMO_MGR* start_demo_mgr = gmStartDemoMgrGetInfo();
	amAssert( start_demo_mgr );
	if ( start_demo_mgr ){
		//É^ÉXÉN
		if ( start_demo_mgr->main_tcb ){
			mtTaskClearTcb( start_demo_mgr->main_tcb );
			start_demo_mgr->main_tcb = NULL;
		}

		//èIóπê›íË
		gmStartDemoClearGameFlag( GMD_GAME_FLAG_START_DEMO );
	}
}

// ==========================================================================
// gmStartDemoExit
/*!
 * èIóπèàóù
 */
// ==========================================================================
void gmStartDemoExit( void )
{
	if ( g_start_demo_mgr ){
		//èIóπóvãÅ
		gmStartDemoRequestExit();

		//ÉèÅ[ÉN
		g_start_demo_mgr = NULL;
	}
}

// ==========================================================================
// gmStartDemoProcMain
/*!
 * ÉÅÉCÉìèàóù
 */
// ==========================================================================
void gmStartDemoProcMain( MTS_TASK_TCB* tcb )
{
	GMS_START_DEMO_WORK* work = (GMS_START_DEMO_WORK*)mtTaskGetTcbWork( tcb );
	amAssert( work );

	// èIóπóvãÅ
	if ( work->flag & GMD_START_DEMO_FLAG_EXIT ) {
		gmStartDemoRequestExit();
		return;
	}

	if ( work->update ){
		work->update( work );
	}
	

	// çXêV
	++work->counter;
}

// ==========================================================================
// gmStartDemoSetGameFlag
/*!
 * ÉQÅ[ÉÄÉÅÉCÉìÇ…ÉtÉâÉOÇê›íËÇ∑ÇÈ
 */
// ==========================================================================
void gmStartDemoSetGameFlag( u32 flag )
{
	g_gm_main_system.game_flag |= flag;
}

// ==========================================================================
// gmStartDemoClearGameFlag
/*!
 * ÉQÅ[ÉÄÉÅÉCÉìÇÃÉtÉâÉOÇâèúÇ∑ÇÈ
 */
// ==========================================================================
void gmStartDemoClearGameFlag( u32 flag )
{
	g_gm_main_system.game_flag &= ~flag;
}

// ==========================================================================
//2DÉAÉNÉVÉáÉìä«óù
// ==========================================================================


// ==========================================================================
// gmStartDemoActionCreate
/*!
 * ÉAÉNÉVÉáÉìçÏê¨
 *
 * @param work ÉèÅ[ÉN
 */
// ==========================================================================
void gmStartDemo2DActionCreate( GMS_START_DEMO_WORK* work )
{
	amAssert( work );

	GMS_START_DEMO_DATA* start_demo_data = gmStartDemoDataGetInfo();
	amAssert( start_demo_data );
	GSS_MAIN_SYS_INFO* main_sys_info = GsGetMainSysInfo();
	amAssert( main_sys_info );

	GSE_LANGUAGE lang = GsEnvGetLanguage();


	//ã§í 
	for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_CMN_NUM > i; ++i ){
		GME_START_DEMO_DATA_TYPE data_type = g_gm_start_demo_data_type_cmn[i];
		GMS_COCKPIT_2D_WORK* cockpit_2d_work = gmStartDemo2DActionCreate(
				g_gm_start_demo_action_name_cmn[i],
				&start_demo_data->aos_texture[data_type],
				g_gm_start_demo_data_ama_id[lang][data_type],
				g_gm_start_demo_action_id_cmn[i],
				g_gm_start_demo_action_node_flag_cmn[i] );
		if ( cockpit_2d_work ){
			//ÉAÉjÉÅÅ[ÉVÉáÉìÇé~ÇﬂÇƒÇ®Ç≠
			cockpit_2d_work->obj_2d.speed = 0.0f;
		}

		//ÉèÅ[ÉNÇ…ê›íË
		work->action_obj_work_cmn[i] = cockpit_2d_work;
	}

	//É]Å[Éìï 
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[main_sys_info->stage_id];
	for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_ZONE_NUM > i; ++i ){
		GME_START_DEMO_DATA_TYPE data_type = g_gm_start_demo_data_type_zone[i];
		GMS_COCKPIT_2D_WORK* cockpit_2d_work = gmStartDemo2DActionCreate(
				g_gm_start_demo_action_name_zone[i],
				&start_demo_data->aos_texture[data_type],
				g_gm_start_demo_data_ama_id[lang][data_type],
				g_gm_start_demo_action_id_zone[zone_type][i],
				g_gm_start_demo_action_node_flag_zone[i] );
		if ( cockpit_2d_work ){
			//ÉAÉjÉÅÅ[ÉVÉáÉìÇé~ÇﬂÇƒÇ®Ç≠
			cockpit_2d_work->obj_2d.speed = 0.0f;
		}

		//ÉèÅ[ÉNÇ…ê›íË
		work->action_obj_work_zone[i] = cockpit_2d_work;
	}

	//ÉAÉNÉVÉáÉìï 
	s32 act_no = g_gm_start_demo_act_no[main_sys_info->stage_id];
	if ( zone_type != GSD_MAIN_ZONE_TYPE_FINAL ){
		for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_ACT_NUM > i; ++i ){
			GME_START_DEMO_DATA_TYPE data_type = g_gm_start_demo_data_type_act[i];
			GMS_COCKPIT_2D_WORK* cockpit_2d_work = gmStartDemo2DActionCreate(
					g_gm_start_demo_action_name_act[i],
					&start_demo_data->aos_texture[data_type],
					g_gm_start_demo_data_ama_id[lang][data_type],
					g_gm_start_demo_action_id_act[act_no][i],
					g_gm_start_demo_action_node_flag_act[i] );
			if ( cockpit_2d_work ){
				//ÉAÉjÉÅÅ[ÉVÉáÉìÇé~ÇﬂÇƒÇ®Ç≠
				cockpit_2d_work->obj_2d.speed = 0.0f;
			}

			//ÉèÅ[ÉNÇ…ê›íË
			work->action_obj_work_act[i] = cockpit_2d_work;
		}
	}

	//ÉÅÉbÉZÅ[ÉW
	{
		GME_START_DEMO_DATA_TYPE data_type = GMD_START_DEMO_DATA_TYPE_LANG;
		GMS_COCKPIT_2D_WORK* cockpit_2d_work = gmStartDemo2DActionCreate(
				g_gm_start_demo_action_name_message,
				&start_demo_data->aos_texture[data_type],
				g_gm_start_demo_data_ama_id[lang][data_type],
				g_gm_start_demo_action_id_message[zone_type][act_no],
				TRUE );
		if ( cockpit_2d_work ){
			//ÉAÉjÉÅÅ[ÉVÉáÉìÇé~ÇﬂÇƒÇ®Ç≠
			cockpit_2d_work->obj_2d.speed = 0.0f;
		}

		//ÉèÅ[ÉNÇ…ê›íË
		work->action_obj_work_message = cockpit_2d_work;
	}

}

// ==========================================================================
// gmStartDemoActionCreate
/*!
 * ÉAÉNÉVÉáÉìçÏê¨
 *
 * @param tcb_name TCBÇ…ìnÇ∑ñºëO
 * @param aos_texture AOSÉeÉNÉXÉ`ÉÉ
 * @param ama_id AMAÇÃID
 * @param action_id ÉAÉNÉVÉáÉìÇÃID
 * @param node_flag tureÅFÉmÅ[Éh FALSEÅFÉAÉNÉVÉáÉì
 */
// ==========================================================================
GMS_COCKPIT_2D_WORK* gmStartDemo2DActionCreate( 
							   const char* tcb_name,
							   AOS_TEXTURE* aos_texture,
							   s32 ama_id,
							   s32 action_id,
							   BOOL node_flag )
{
	UNREFERENCED_PARAMETER(tcb_name);

	if ( action_id == GMD_START_DEMO_INVLID_ID ){
		return NULL;
	}

	//ÉèÅ[ÉNéÊìæ
	GMS_COCKPIT_2D_WORK* obj_work = (GMS_COCKPIT_2D_WORK*)GMM_COCKPIT_CREATE_WORK(
			sizeof(GMS_COCKPIT_2D_WORK),
			NULL,
			0,
			tcb_name ); 
	amAssert( obj_work );

	//ÉèÅ[ÉNê›íË
	obj_work->cpit_com.obj_work.disp_flag |= node_flag;

	//ÉAÉNÉVÉáÉìê›íË
	ObjObjectAction2dAMALoadSetTexlist(
			&obj_work->cpit_com.obj_work,
			&obj_work->obj_2d,
			NULL,
			NULL,
			ama_id,
			GmGameDatGetCockpitData(),
			AoTexGetTexList(aos_texture ),
			(u32)action_id,
			node_flag );
#if _IPHONE
	((OBS_OBJECT_WORK *)obj_work)->pos.z -= FX_F32_TO_FX32(10.0f);
#endif
	return obj_work;
}

// ==========================================================================
// ÉÅÉCÉìèàóù
// ==========================================================================
void gmStartDemoProcFade( GMS_START_DEMO_WORK* work )
{
	//ÉtÉFÅ[ÉhèIóπë“Çø
	if (IzFadeIsEnd()) {
		IzFadeExit();

		work->update = gmStartDemoProcIn;
		work->counter = 0;

		//ã§í 
		for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_CMN_NUM > i; ++i ){
			GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_cmn[i];
			if ( cockpit_2d_work ){
				cockpit_2d_work->obj_2d.speed = 1.0f;
				cockpit_2d_work->obj_2d.frame = 0.0f;
			}
		}
		//É]Å[Éìï 
		//ÉAÉNÉgï 
		for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_ZONE_NUM > i; ++i ){
			GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_zone[i];
			if ( cockpit_2d_work ){
				cockpit_2d_work->obj_2d.speed = 1.0f;
				cockpit_2d_work->obj_2d.frame = 0.0f;
			}
		}
		//ÉAÉNÉgï 
		for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_ACT_NUM > i; ++i ){
			GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_act[i];
			if ( cockpit_2d_work ){
				cockpit_2d_work->obj_2d.speed = 1.0f;
				cockpit_2d_work->obj_2d.frame = 0.0f;
			}
		}
		//ÉÅÉbÉZÅ[ÉW
		{
			GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_message;
			if ( cockpit_2d_work ){
				cockpit_2d_work->obj_2d.speed = 1.0f;
				cockpit_2d_work->obj_2d.frame = 0.0f;
			}
		}
	}
}
void gmStartDemoProcIn( GMS_START_DEMO_WORK* work )
{
	//ì¸èÍèIóπ
	if ( work->counter >= 39 ){
		work->update = gmStartDemoProcWait;

		//ã§í 
		for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_CMN_NUM > i; ++i ){
			GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_cmn[i];
			if ( cockpit_2d_work ){
				cockpit_2d_work->obj_2d.speed = 0.0f;
				cockpit_2d_work->obj_2d.frame = 40.0f;
			}
		}
		//É]Å[Éìï 
		for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_ZONE_NUM > i; ++i ){
			GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_zone[i];
			if ( cockpit_2d_work ){
				cockpit_2d_work->obj_2d.speed = 0.0f;
				cockpit_2d_work->obj_2d.frame = 40.0f;
			}
		}
		//ÉAÉNÉgï 
		for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_ACT_NUM > i; ++i ){
			GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_act[i];
			if ( cockpit_2d_work ){
				cockpit_2d_work->obj_2d.speed = 0.0f;
				cockpit_2d_work->obj_2d.frame = 40.0f;
			}
		}
		//ÉÅÉbÉZÅ[ÉWï 
		{
			GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_message;
			if ( cockpit_2d_work ){
				cockpit_2d_work->obj_2d.speed = 0.0f;
				cockpit_2d_work->obj_2d.frame = 40.0f;
			}
		}
	}
}

void gmStartDemoProcWait( GMS_START_DEMO_WORK* work )
{
	//ë“ÇøèIóπ
	if ( work->counter >= 160 ){
		work->update = gmStartDemoProcOut;
		
		if (GmStartMsgIsExe()) {	// 20091128 Ishizaki
			// ê‡ñæÉÅÉbÉZÅ[ÉWÇ†ÇË
			GmStartMsgInit();
		}
		else {
			// ÉvÉåÉCÉÑFWÇ÷
			GMS_PLAYER_WORK* player_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
			GmPlySeqChangeSequence(player_work, GME_PLY_SEQ_STATE_FW);

			//É^ÉCÉ}Å[äJén
			gmStartDemoSetGameFlag( GMD_GAME_FLAG_COUNT_GAME_TIME );
		}

		//ã§í 
		for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_CMN_NUM > i; ++i ){
			GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_cmn[i];
			if ( cockpit_2d_work ){
				cockpit_2d_work->obj_2d.speed = 1.0f;
			}
		}
		//É]Å[Éìï 
		for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_ZONE_NUM > i; ++i ){
			GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_zone[i];
			if ( cockpit_2d_work ){
				cockpit_2d_work->obj_2d.speed = 1.0f;
			}
		}
		//ÉAÉNÉgï 
		for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_ACT_NUM > i; ++i ){
			GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_act[i];
			if ( cockpit_2d_work ){
				cockpit_2d_work->obj_2d.speed = 1.0f;
			}
		}
		//ÉÅÉbÉZÅ[ÉW
		{
			GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_message;
			if ( cockpit_2d_work ){
				cockpit_2d_work->obj_2d.speed = 1.0f;
			}
		}
	}
}

void gmStartDemoProcOut( GMS_START_DEMO_WORK* work )
{
	//ëﬁèoèIóπ
	if ( work->counter > 230 ){
		work->update = gmStartDemoProcEnd;
	}
}

void gmStartDemoProcEnd( GMS_START_DEMO_WORK* work )
{
	work->update = NULL;
	gmStartDemoRequestExit();

	//ã§í 
	for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_CMN_NUM > i; ++i ){
		GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_cmn[i];
		if ( cockpit_2d_work ){
			cockpit_2d_work->cpit_com.obj_work.flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		}
	}
	//É]Å[Éìï 
	for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_ZONE_NUM > i; ++i ){
		GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_zone[i];
		if ( cockpit_2d_work ){
			cockpit_2d_work->cpit_com.obj_work.flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		}
	}
	//ÉAÉNÉgï 
	for ( s32 i = 0; GMD_START_DEMO_ACTION_INDEX_ACT_NUM > i; ++i ){
		GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_act[i];
		if ( cockpit_2d_work ){
			cockpit_2d_work->cpit_com.obj_work.flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		}
	}
	//ÉÅÉbÉZÅ[ÉW
	{
		GMS_COCKPIT_2D_WORK* cockpit_2d_work = work->action_obj_work_message;
		if ( cockpit_2d_work ){
			cockpit_2d_work->cpit_com.obj_work.flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		}
	}
}

void GmStartDemoStaticVarInit(void)
{
	memset(&g_start_demo_data_real, 0, sizeof(g_start_demo_data_real));
	g_start_demo_data = NULL;
	
	memset(&g_start_demo_mgr_real, 0, sizeof(g_start_demo_mgr_real));
	g_start_demo_mgr = NULL;
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
