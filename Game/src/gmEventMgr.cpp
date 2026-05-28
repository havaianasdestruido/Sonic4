// ==========================================================================
/*!
  @file gmEventMgr.c
  @brief ƒQ[ƒ€ ƒCƒxƒ“ƒgƒ}ƒl[ƒWƒƒ[

  @author Ishizaki
				Copyright(c) 2008 Dimps

  $Id: gmEventMgr.cpp 20 2011-04-22 12:46:46Z thamada $
  $Date: 2011-04-22 21:46:46 +0900 (é‡‘, 22 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmEventMgr.c,v $
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gmMain.h"
#include "gsMainSys.h"
#include "gmGameDat.h"
#include "gmRing.h"

#include "gmEventMgr.h"
#include "gmEventTbl.h"

#include "gmEnemy.h"

#include "mppCheckPointStorage.h"

//----- Definitions ---------------------------------------------------------
/* ƒfƒoƒbƒO */
#define GMD_EVE_DEBUG_PRINTMSG		(0 && MTD_DEBUG)		//!< ƒfƒoƒbƒOƒƒbƒZ[ƒWo—Íİ’è
#define GMD_EVE_DEBUG_EVENT_CHECK	(0 && MTD_DEBUG)		//!< ƒCƒxƒ“ƒgŠÔˆá‚¢ƒ`ƒFƒbƒN
/**/

/// ƒCƒxƒ“ƒgƒ^ƒCƒv
typedef enum tag_GME_EVT_TYPE {
	GMD_EVE_TYPE_EVENT	= 0,					//!< ƒMƒ~ƒbƒNA“G
	GMD_EVE_TYPE_RING,							//!< ƒŠƒ“ƒO
	GMD_EVE_TYPE_DECO,							//!< ‘•ü•¨

	GMD_EVE_TYPE_MAX
} GME_EVE_TYPE; 


/* ƒ[ƒJƒ‹ƒCƒxƒ“ƒg */
#define GMD_EVE_LOCAL_EVT_OBJ_MAX				(64)									//!< ƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒIƒuƒWƒFƒNƒgƒ[ƒN”
#define GMD_EVE_LOCAL_EVT_USE_FLAG_WORK_NUM		((GMD_EVE_LOCAL_EVT_OBJ_MAX + 31)/32)	//!< ƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒIƒuƒWƒFƒNƒgƒ[ƒNƒtƒ‰ƒO ƒ[ƒN”
#define GMD_EVE_LOCAL_DECO_OBJ_MAX				(64)									//!< ƒ[ƒJƒ‹‘•üƒIƒuƒWƒFƒNƒgƒ[ƒN”
#define GMD_EVE_LOCAL_DECO_USE_FLAG_WORK_NUM	((GMD_EVE_LOCAL_DECO_OBJ_MAX + 31)/32)	//!< ƒ[ƒJƒ‹‘•üƒIƒuƒWƒFƒNƒgƒ[ƒNƒtƒ‰ƒO ƒ[ƒN”
#define GMD_EVE_LOCAL_RING_OBJ_MAX				(64)									//!< ƒ[ƒJƒ‹ƒŠƒ“ƒOƒIƒuƒWƒFƒNƒgƒ[ƒN”
#define GMD_EVE_LOCAL_RING_USE_FLAG_WORK_NUM	((GMD_EVE_LOCAL_RING_OBJ_MAX + 31)/32)	//!< ƒ[ƒJƒ‹ƒŠƒ“ƒOƒIƒuƒWƒFƒNƒgƒ[ƒNƒtƒ‰ƒO ƒ[ƒN”

// ƒCƒxƒ“ƒg¶¬‹éŒ` ƒfƒtƒHƒ‹ƒg’l
#define GMD_EVE_EVENT_CREATE_CHECK_SIZE		(256)		//!< •W€ƒCƒxƒ“ƒg ¶¬ƒ`ƒFƒbƒNŠî–{ƒTƒCƒY
//#define GMD_EVE_DECO_CREATE_SIZE		(256)		//!< ‘•ü•¨



/// ƒCƒxƒ“ƒgƒ}ƒl[ƒWƒƒ[ƒ[ƒN
typedef struct tag_GMS_EVE_MGR_WORK {
	void			(*sts_proc)(void);					//!< ó‘Ô–ˆˆ—ŠÖ”
	u32				flag;

#if !_DS
	float			prev_pos[MTD_XY];		//!< 1F‘O‚Ìƒ}ƒbƒvÀ•W(ƒCƒxƒ“ƒg¶¬ƒ`ƒFƒbƒN—p)
#else
	fx32			prev_pos[MTE_LCD_MAX/*MTE_GE2_MAX*/][MTD_XY];		//!< 1F‘O‚Ìƒ}ƒbƒvÀ•W(ƒCƒxƒ“ƒg¶¬ƒ`ƒFƒbƒN—p)
#endif

//	s32				max_scrl_size;						//!< 1ƒtƒŒ[ƒ€‚ ‚½‚è‚ÌÅ‘åƒXƒNƒ[ƒ‹ƒTƒCƒY
//	s32				birth_width;						//!< ¶¬—Ìˆæ”ÍˆÍ

	u16				map_size[MTD_XY];					//!< ƒCƒxƒ“ƒgƒuƒƒbƒNî•ñ‚©‚çæ“¾‚µ‚½ƒ}ƒbƒvƒTƒCƒY

} GMS_EVE_MGR_WORK;


/// ƒCƒxƒ“ƒgƒf[ƒ^
//typedef struct tag_GMS_EVE_MGR_EVENT_DATA {
//	const void *data_eve;				//!< ƒCƒxƒ“ƒgƒf[ƒ^
////	const void *const	data_deco;				//!< ‘•üƒf[ƒ^
//} GMS_EVE_MGR_EVENT_DATA;


/* ƒf[ƒ^\‘¢‘Ì */
// ƒf[ƒ^ƒuƒƒbƒNƒTƒCƒY
#define GMD_EVE_DATA_BLOCK_SIZE			(256)
#define GMD_EVE_BLOCK_SIZE_SHIFT	(8)

/// ƒCƒxƒ“ƒgî•ñ ƒŠƒXƒg\‘¢‘Ì(*.ev)
typedef struct tag_GMS_EVE_DATA_EV_LIST {
	u16	eve_num;		//!< ƒCƒxƒ“ƒgî•ñ”
	// ˆÈ‰º‚É‘±‚­ƒf[ƒ^
	GMS_EVE_RECORD_EVENT	eve_rec[1];			//!< ƒf[ƒ^ƒAƒNƒZƒX—p ƒCƒxƒ“ƒgƒŒƒR[ƒh
	// GMS_EVE_RECORD_EVENT	eve_rec[eve_num];	//!< ƒCƒxƒ“ƒgƒŒƒR[ƒh
} GMS_EVE_DATA_EV_LIST;

/// ƒCƒxƒ“ƒgî•ñ ƒwƒbƒ_[\‘¢‘Ì(*.ev)
typedef struct tag_GMS_EVE_DATA_EV_HEADER {
	u16	width;			//!< ƒuƒƒbƒN’PˆÊ •
	u16	height;			//!< ƒuƒƒbƒN’PˆÊ ‚‚³
	// ˆÈ‰º‚É‘±‚­ƒf[ƒ^
	u32	ofst[1];		//!< ƒf[ƒ^ƒAƒNƒZƒX—p ƒCƒxƒ“ƒgî•ñƒŠƒXƒgƒIƒtƒZƒbƒg
	// u32	ofst[width*height];						//!< ƒCƒxƒ“ƒgî•ñƒŠƒXƒgƒIƒtƒZƒbƒg width * height”•ª
	// GMS_EVE_DATA_EV_LIST eve_list[width*height];	//!< ƒCƒxƒ“ƒgî•ñƒŠƒXƒg width * height”•ª
} GMS_EVE_DATA_EV_HEADER;

/// ƒŠƒ“ƒOî•ñ ƒŠƒXƒg\‘¢‘Ì(*.rg)
typedef struct tag_GMS_EVE_DATA_RG_LIST {
	u16	ring_num;		//!< ‘•üî•ñ”
	// ˆÈ‰º‚É‘±‚­ƒf[ƒ^
	GMS_EVE_RECORD_RING	ring_data[1];			//!< ƒf[ƒ^ƒAƒNƒZƒX—p ƒCƒxƒ“ƒgƒŒƒR[ƒh
	// GMS_EVE_RECORD_RING	ring_data[ring_num];
} GMS_EVE_DATA_RG_LIST;

/// ƒŠƒ“ƒOî•ñ ƒwƒbƒ_[\‘¢‘Ì(*.rg)
typedef struct tag_GMS_EVE_DATA_RG_HEADER {
	u16	width;			//!< ƒuƒƒbƒN’PˆÊ •
	u16	height;			//!< ƒuƒƒbƒN’PˆÊ ‚‚³
	// ˆÈ‰º‚É‘±‚­ƒf[ƒ^
	u32	ofst[1];						//!< ‘•üî•ñƒŠƒXƒgƒIƒtƒZƒbƒg width * height”•ª
	// u32	ofst[width*height];						//!< ‘•üî•ñƒŠƒXƒgƒIƒtƒZƒbƒg width * height”•ª
	// GMS_EVE_DATA_RG_LIST dec_list[width*height];	//!< ‘•üî•ñƒŠƒXƒg width * height”•ª
} GMS_EVE_DATA_RG_HEADER;


/// ‘•üî•ñ ƒŠƒXƒg\‘¢‘Ì(*.dc)
typedef struct tag_GMS_EVE_DATA_DC_LIST {
	u16	dec_num;		//!< ‘•üî•ñ”
	// ˆÈ‰º‚É‘±‚­ƒf[ƒ^
	GMS_EVE_RECORD_DECORATE	dec_data[1];			//!< ƒf[ƒ^ƒAƒNƒZƒX—p ƒCƒxƒ“ƒgƒŒƒR[ƒh
	// GMS_EVE_RECORD_DECORATE	dec_data[dec_num];
} GMS_EVE_DATA_DC_LIST;

/// ‘•üî•ñ ƒwƒbƒ_[\‘¢‘Ì(*.dc)
typedef struct tag_GMS_EVE_DATA_DC_HEADER {
	u16	width;			//!< ƒuƒƒbƒN’PˆÊ •
	u16	height;			//!< ƒuƒƒbƒN’PˆÊ ‚‚³
	// ˆÈ‰º‚É‘±‚­ƒf[ƒ^
	u32	ofst[1];						//!< ‘•üî•ñƒŠƒXƒgƒIƒtƒZƒbƒg width * height”•ª
	// u32	ofst[width*height];						//!< ‘•üî•ñƒŠƒXƒgƒIƒtƒZƒbƒg width * height”•ª
	// GMS_EVE_DATA_DC_LIST dec_list[width*height];	//!< ‘•üî•ñƒŠƒXƒg width * height”•ª
} GMS_EVE_DATA_DC_HEADER;


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------
/*

// ƒCƒxƒ“ƒg‚ğ¶¬‚µ‚È‚¢”ÍˆÍ‚ğ‹‚ß‚é
// ‰æ–Ê’[‚æ‚èA‚±‚Ì’l‚¾‚¯—£‚ê‚Ä‚¢‚È‚¢ê‡‚ÍAƒCƒxƒ“ƒg‚ğ¶¬‚µ‚È‚¢
// ƒMƒ~ƒbƒN
//#define GMM_EVE_GET_EVENT_LCD_SIZE(id)					  \
//		(FX_DivS32(gm_eve_create_check_size_event - g_gm_enemy_size_tbl[(id)], 3))
////	  ((gm_eve_create_check_size_event - g_gm_enemy_size_tbl[(id)]) / 3)
////	  ((gm_eve_create_check_size_event - g_gm_enemy_size_tbl[(id)]) >> 1)

// ‘•ü•¨
//#define GMM_EVE_GET_DECO_LCD_SIZE(id)				   \
//		((gm_eve_create_check_size_deco - g_gm_decorate_size_tbl[(id)]) >> 1)
////	  ((gm_eve_create_check_size_deco - g_gm_decorate_size_tbl[(id)]) / 3)
////	  ((gm_eve_create_check_size_deco - g_gm_decorate_size_tbl[(id)]) >> 1)
*/

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmEveMgrMain(MTS_TASK_TCB *tcb);
static void gmEveMgrDest(MTS_TASK_TCB *tcb);

static s16 gmEventMgrLocalEventNoGet(GME_EVE_TYPE eve_type);

// ó‘Ô–ˆˆ—ŠÖ”
//static void gmEveMgrStateFuncInit(void);
#if _DS
static void gmEveMgrStateFuncDependScr(void);
static void gmEveMgrStateFuncIndependentScr(void);
#else
static void gmEveMgrStateFuncSingleScr(void);
#endif

static void gmEveMgrCreateEventBlkEvent(u32 flag, u16 bx, u16 by, s32 *r_on, s32 *r_off);
static void gmEveMgrCreateEventBlkRing(u32 flag, u16 bx, u16 by, s32 *r_on, s32 *r_off);
static void gmEveMgrCreateEventBlkDecorate(u32 flag, u16 bx, u16 by, s32 *r_on, s32 *r_off);

#if defined (MTD_DEBUG)
//static void gmEventMgrDebugEventCheck(void);
#endif // #if defined (MTD_DEBUG)
//----- Global Variables ----------------------------------------------------
GMS_EVE_MGR_WORK	*g_gm_eve_mgr_work = NULL;	//!< ƒCƒxƒ“ƒgƒ}ƒl[ƒWƒƒ[ƒ[ƒN

//----- Local Variables -----------------------------------------------------
MTS_TASK_TCB		*gm_eve_mgr_tcb = NULL;		//!< ƒCƒxƒ“ƒgƒ}ƒl[ƒWƒƒ[TCB


/* ƒCƒxƒ“ƒgŠÖ˜Aƒf[ƒ^ */
static GMS_EVE_DATA_EV_HEADER *gm_eve_data = NULL;				//!< ƒCƒxƒ“ƒgƒf[ƒ^
static GMS_EVE_DATA_RG_HEADER *gm_ring_data = NULL;				//!< ƒŠƒ“ƒOƒf[ƒ^
static GMS_EVE_DATA_DC_HEADER *gm_deco_data = NULL;				//!< ‘•üƒf[ƒ^

static s32 gm_eve_data_size = -1;//qqq
static s32 gm_ring_data_size = -1;//qqq
//static s32 gm_deco_data_size = -1;

/// ƒCƒxƒ“ƒg¶¬’TõŠÖ”
static void (*gm_evemgr_create_eve_func_tbl[GMD_EVE_TYPE_MAX])(u32 flag, u16 bx, u16 by, s32 *r_on, s32 *r_off) = {
	gmEveMgrCreateEventBlkEvent,			// ƒMƒ~ƒbƒNA“G
	gmEveMgrCreateEventBlkRing,				// ƒŠƒ“ƒO
	gmEveMgrCreateEventBlkDecorate,			// ‘•ü•¨
};

/// ƒCƒxƒ“ƒg¶¬‹éŒ`ƒTƒCƒYƒe[ƒuƒ‹
static const u16 gm_evemgr_create_size_tbl[GMD_EVE_TYPE_MAX] = {
	/* ƒCƒxƒ“ƒgƒ^ƒCƒv•Ê‚É‰æ–ÊŠO‚Ì¶¬”ÍˆÍ—Ìˆæ‚ğİ’è */
	/* ŠeƒCƒxƒ“ƒgƒ^ƒCƒv‚ÌÅ‘åƒTƒCƒY‚ğİ’è */
	256,			///< ƒMƒ~ƒbƒNA“G
	16,				///< ƒŠƒ“ƒO
	256,			///< ‘•ü•¨
};

/* ƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒ[ƒN */
// ƒCƒxƒ“ƒg
static u32	gm_eve_local_evt_obj_use_flag[GMD_EVE_LOCAL_EVT_USE_FLAG_WORK_NUM] = {0};	//!< ƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒIƒuƒWƒFƒNƒgg—pƒ`ƒFƒbƒNƒtƒ‰ƒO
static GMS_EVE_RECORD_EVENT			gm_eve_local_evt_record[GMD_EVE_LOCAL_EVT_OBJ_MAX]; 	//!< ƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒIƒuƒWƒFƒNƒgƒ[ƒN
// ƒŠƒ“ƒO
static u32	gm_eve_local_ring_obj_use_flag[GMD_EVE_LOCAL_RING_USE_FLAG_WORK_NUM] = {0};	//!< ƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒIƒuƒWƒFƒNƒgg—pƒ`ƒFƒbƒNƒtƒ‰ƒO
static GMS_EVE_RECORD_RING			gm_eve_local_ring_record[GMD_EVE_LOCAL_RING_OBJ_MAX]; 	//!< ƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒIƒuƒWƒFƒNƒgƒ[ƒN
// ‘•ü
static u32	gm_eve_local_deco_obj_use_flag[GMD_EVE_LOCAL_DECO_USE_FLAG_WORK_NUM] = {0};	//!< ƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒIƒuƒWƒFƒNƒgg—pƒ`ƒFƒbƒNƒtƒ‰ƒO
static GMS_EVE_RECORD_DECORATE		gm_eve_local_deco_record[GMD_EVE_LOCAL_DECO_OBJ_MAX]; 	//!< ƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒIƒuƒWƒFƒNƒgƒ[ƒN



//----- Global Functions ----------------------------------------------------
// ==========================================================================
// ‰Šú‰» I—¹ˆ—
// ==========================================================================
// ==========================================================================
// GmEventMgrInit
/*!
 *	ƒCƒxƒ“ƒgƒ}ƒl[ƒWƒƒ‰Šú‰»
 *
 *	@note
 *		ƒCƒxƒ“ƒgƒf[ƒ^“Ç‚İ‚İŒã‚ÉŒÄ‚Ño‚µ‚Ü‚·\n
 *		ƒCƒxƒ“ƒg¶¬ŠJn‚Í GmEventMgrStart ‚ğŒÄ‚Ño‚µ‚½Œã‚É‚È‚è‚Ü‚·B
 */
// ==========================================================================
void GmEventMgrInit(void)
{
	GMS_EVE_MGR_WORK	*mgr_work;

	MTM_ASSERT(g_gm_eve_mgr_work == NULL);
	MTM_ASSERT(gm_eve_mgr_tcb == NULL);
	MTM_ASSERT(gm_eve_data);
	MTM_ASSERT(gm_ring_data);
	MTM_ASSERT(gm_deco_data);

	// ƒƒCƒ“ˆ—¶¬
	gm_eve_mgr_tcb = MTM_TASK_MAKE_TCB(gmEveMgrMain, gmEveMgrDest,
										0, GMD_TASK_PAUSE_LEVEL_EVTMGR,
										GMD_TASK_PRIO_EVTMGR, GMD_TASK_GROUP_EVTMGR,
										sizeof(GMS_EVE_MGR_WORK), "GM_EVT_MGR");
	// ƒCƒxƒ“ƒgƒ}ƒl[ƒWƒƒ[ƒ[ƒNæ“¾
	mgr_work = (GMS_EVE_MGR_WORK*)mtTaskGetTcbWork(gm_eve_mgr_tcb);
	MI_CpuClear8(mgr_work, sizeof(GMS_EVE_MGR_WORK));
	g_gm_eve_mgr_work = mgr_work;

	// ƒCƒxƒ“ƒg¶¬‹éŒ`İ’è
	//gm_eve_create_check_size_event	= GMD_EVE_EVENT_CREATE_CHECK_SIZE;
	//gm_eve_create_check_size_deco	 = GMD_EVE_DECO_CREATE_SIZE;

	// ó‘ÔŠÖ”İ’è
	//g_gm_eve_mgr_work->sts_proc  = gmEveMgrStateFuncInit;

	// ƒ}ƒbƒvƒTƒCƒYæ“¾
	g_gm_eve_mgr_work->map_size[MTD_X] = (u16)(gm_eve_data->width << GMD_EVE_BLOCK_SIZE_SHIFT);
	g_gm_eve_mgr_work->map_size[MTD_Y] = (u16)(gm_eve_data->height << GMD_EVE_BLOCK_SIZE_SHIFT);

	// Å‘åƒXƒNƒ[ƒ‹ƒTƒCƒY
	//g_gm_eve_mgr_work->max_scrl_size = GMD_MAIN_SCR_SPD_MAX;

	// ¶¬—Ìˆæ•
	//g_gm_eve_mgr_work->birth_width = GMD_MAIN_SCR_SPD_MAX*2;

#if defined (MTD_DEBUG)
	// ƒCƒxƒ“ƒgƒf[ƒ^İ’èŠÔˆá‚¢ƒ`ƒFƒbƒN
#if GMD_EVE_DEBUG_EVENT_CHECK
	gmEventMgrDebugEventCheck();
#endif // GMD_EVE_DEBUG_EVENT_CHECK
#endif	// #if defined (MTD_DEBUG)

}

// ==========================================================================
// GmEventMgrStart
/*!
 *	ƒCƒxƒ“ƒgƒ}ƒl[ƒWƒƒ ˆ—ŠJn
 *
 *	@note
 *		GmEventMgrInit ÀsŒã‚©‚çŒÄ‚Ño‚¹‚Ü‚·B\n
 *		‹­§¶¬ƒCƒxƒ“ƒg‚ÆA–ˆƒtƒŒ[ƒ€Às—p‚ÌŠÖ”İ’è‚È‚Ç‚ğs‚¢‚Ü‚·B\n
 *		Œ»İ‚ÌƒJƒƒ‰ˆÊ’u‚ÌƒCƒxƒ“ƒg¶¬‚ÍAƒJƒƒ‰İ’èŒã‚És‚Á‚Ä‰º‚³‚¢B
 */
// ==========================================================================
void GmEventMgrStart(void)
{
	// ‹­§¶¬ƒCƒxƒ“ƒg¶¬
	GmEventMgrCreateEventEnforce();

#if !_DS
	// –ˆƒtƒŒ[ƒ€‚Ìˆ—‚ğİ’è
	g_gm_eve_mgr_work->sts_proc  = gmEveMgrStateFuncSingleScr;
#else
	// ‰æ–Ê“àƒCƒxƒ“ƒg¶¬
	// Ÿ‰æ–Êİ’è‚ªŒˆ‚Ü‚Á‚½‚ç‚»‚ê‚¼‚ê‘Î‰
	if (g_obj.flag & OBD_OBJ_CAMERA_STICK) {
		// 2‰æ–Ê˜A“®
		//GmEveMgrSearchEventLcdDS(GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE | GMD_EVE_SEARCH_PROC_FLAG_RECT_LCD_OFF);
		//GmEveMgrSearchEventLcdDS(GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE);

		// –ˆƒtƒŒ[ƒ€‚Ìˆ—‚ğİ’è
		g_gm_eve_mgr_work->sts_proc  = gmEveMgrStateFuncDependScr;
	}
	else {
		// ‰æ–Ê’P‘Ì
	//	GmEveMgrCreateEventLcd(MTE_LCD_UP, GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE | GMD_EVE_SEARCH_PROC_FLAG_RECT_LCD_OFF);
	//	GmEveMgrCreateEventLcd(MTE_LCD_DOWN, GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE | GMD_EVE_SEARCH_PROC_FLAG_RECT_LCD_OFF);

		// –ˆƒtƒŒ[ƒ€‚Ìˆ—‚ğİ’è
		g_gm_eve_mgr_work->sts_proc  = gmEveMgrStateFuncIndependentScr;
	}
#endif

#if 0

	if (GmMainIsBossStage()) {
		// ƒ{ƒXƒXƒe[ƒW‚È‚çAƒ}ƒbƒv‘S‘Ì‚ÌƒCƒxƒ“ƒg‚ğ¶¬
		gmEveMgrSearchAllEvent(work);
		work->proc  = gmEveMgrProc0300;

	} else {
		// ’ÊíƒXƒe[ƒWFã‰ºLCD‚Ì‘SƒCƒxƒ“ƒg¶¬
		if (g_gm_map.flag & GMD_MAP_FLAG_CAM_DEPEND) {
			// ‚Q‰æ–Ê˜A“®
			gmEveMgrSearchLcdEventDS(work, MTE_GE2_A, GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE);
			work->proc  = gmEveMgrProc0100;
			
		} else {
			// ‚Q‰æ–Ê“Æ—§
			gmEveMgrSearchLcdEvent(work, MTE_GE2_A, GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE);
			gmEveMgrSearchLcdEvent(work, MTE_GE2_B, GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE);
			work->proc  = gmEveMgrProc0200;
		}
	}
}
#endif
}

// ==========================================================================
// GmEventMgrExit
/*!
 *	ƒCƒxƒ“ƒgƒ}ƒl[ƒWƒƒI—¹ˆ—
 */
// ==========================================================================
void GmEventMgrExit(void)
{
#if 1
	// ˆ—I—¹
	if (gm_eve_mgr_tcb) {
		MTM_ASSERT(g_gm_eve_mgr_work);
		mtTaskClearTcb(gm_eve_mgr_tcb);
	}

#else
	if (!g_gm_eve_mgr_work) {
		MTM_ASSERT(0);
		return;
	}

	// ˆ—I—¹
	if (gm_eve_mgr_tcb) {
		mtTaskClearTcb(gm_eve_mgr_tcb);
	}
#endif
	
	mppEnemyList_clearAll();
}

// ==========================================================================
// GmEventDataBuild
/*!
 *	ƒCƒxƒ“ƒgƒf[ƒ^ \’z
 */
// ==========================================================================
void GmEventDataBuild(void)
{
#if 1
	AMS_AMB_FILE	*amb_info;
	void			*data;


	// ƒCƒxƒ“ƒgƒf[ƒ^
	data = amBindGet((AMS_AMB_HEADER*)g_gm_gamedat_map[GMD_GAMEDAT_MAP_MAP_SET],
				GMD_GAMEDAT_MAPSET_EV,
				&amb_info);
	gm_eve_data = (GMS_EVE_DATA_EV_HEADER*)amMemAlloc(amb_info->size);
	memcpy(gm_eve_data, data, amb_info->size);
	gm_eve_data_size = amb_info->size;

	// ƒŠƒ“ƒOƒf[ƒ^
	data = amBindGet((AMS_AMB_HEADER*)g_gm_gamedat_map[GMD_GAMEDAT_MAP_MAP_SET],
				GMD_GAMEDAT_MAPSET_RG,
				&amb_info);
	gm_ring_data = (GMS_EVE_DATA_RG_HEADER*)amMemAlloc(amb_info->size);
	memcpy(gm_ring_data, data, amb_info->size);
	gm_ring_data_size = amb_info->size;

	// ‘•üƒf[ƒ^
	data = amBindGet((AMS_AMB_HEADER*)g_gm_gamedat_map[GMD_GAMEDAT_MAP_MAP_SET],
				GMD_GAMEDAT_MAPSET_DC,
				&amb_info);
	gm_deco_data = (GMS_EVE_DATA_DC_HEADER*)amMemAlloc(amb_info->size);
	memcpy(gm_deco_data, data, amb_info->size);
	//gm_deco_data_size = amb_info->size;
	
	mppEnemyList_clearAll();
	mppEM_StoreOriginMapDataToMemory();//qqq
	
#else
	// ƒCƒxƒ“ƒgƒf[ƒ^
	gm_eve_data		= (GMS_EVE_DATA_EV_HEADER*)g_gm_gamedat_map_set[GMD_GAMEDAT_MAPSET_EV];
	// ƒŠƒ“ƒOƒf[ƒ^
	gm_ring_data	= (GMS_EVE_DATA_RG_HEADER*)g_gm_gamedat_map_set[GMD_GAMEDAT_MAPSET_RG];
	// ‘•üƒf[ƒ^
	gm_deco_data	= (GMS_EVE_DATA_DC_HEADER*)g_gm_gamedat_map_set[GMD_GAMEDAT_MAPSET_DC];
#endif
#if 0
	void	*buf_temp;

	MTM_ASSERT(gm_eve_data == NULL);
	MTM_ASSERT(g_gm_gamedat_map[GME_MAIN_STAGE_DATA_TYPE_EVE]);

	// ƒCƒxƒ“ƒgƒf[ƒ^æ“¾
	buf_temp = g_gm_gamedat_map[GME_MAIN_STAGE_DATA_TYPE_EVE];

	gm_eve_data = (GMS_EVE_DATA_EV_HEADER*)mtMemAllocMain(MI_GetUncompressedSize(buf_temp));
	mtUncompress(buf_temp, gm_eve_data);
#endif
	

}

// ==========================================================================
// GmEventDataFlush
/*!
 *	ƒCƒxƒ“ƒgƒf[ƒ^ ŠJ•ú
 */
// ==========================================================================
void GmEventDataFlush(void)
{
#if 1
	if (gm_eve_data) {
		amMemFree(gm_eve_data);
		gm_eve_data = NULL;
	}
	if (gm_ring_data) {
		mtMemFreeMain(gm_ring_data);
		gm_ring_data = NULL;
	}
	if (gm_deco_data) {
		mtMemFreeMain(gm_deco_data);
		gm_deco_data = NULL;
	}
	gm_eve_data_size = -1;
	gm_ring_data_size = -1;
	//gm_deco_data_size = -1;
#else
	// ƒA[ƒJƒCƒu’¼QÆ‚È‚Ì‚ÅŠJ•ú‚Ì•K—v‚È‚µ
	// ƒCƒxƒ“ƒgƒf[ƒ^
	gm_eve_data		= NULL;
	// ƒŠƒ“ƒOƒf[ƒ^
	gm_ring_data	= NULL;
	// ‘•üƒf[ƒ^
	gm_deco_data	= NULL;
#endif
#if 0
	if (gm_eve_data) {
		mtMemFreeMain(gm_eve_data);
		gm_eve_data = NULL;
	}
	if (gm_ring_data) {
		mtMemFreeMain(gm_ring_data);
		gm_ring_data = NULL;
	}
	if (gm_deco_data) {
		mtMemFreeMain(gm_deco_data);
		gm_deco_data = NULL;
	}
#endif
}

// ==========================================================================
// ƒCƒxƒ“ƒg¶¬
// ==========================================================================
// ==========================================================================
// GmEventMgrCreateEventEnforce
/*!
 *	ƒCƒxƒ“ƒg¶¬ ‹­§¶¬ƒCƒxƒ“ƒg¶¬
 *
 *	@note
 *		ƒXƒ^[ƒgƒMƒ~ƒbƒN“™‚àŠÜ‚Ş \n
 *		ƒvƒŒƒCƒ„[ƒIƒuƒWƒFƒNƒg¶¬‘O‚ÉÀs
 */
// ==========================================================================
void GmEventMgrCreateEventEnforce(void)
{
	s32		ev_rect[MTD_RECT];	// ƒCƒxƒ“ƒg¶¬‹éŒ`(ƒhƒbƒg’PˆÊ)
	u16		bx_cnt, by_cnt;

	MTM_ASSERT(g_gm_eve_mgr_work);
	OS_TPrintf(">>GmEventMgrCreateEventEnforce -------- BEGIN\n");

	// ƒCƒxƒ“ƒg¶¬‹éŒ`(ƒ}ƒbƒv‘S‘Ì)
	ev_rect[MTD_LEFT]	= 0;
	ev_rect[MTD_RIGHT]	= g_gm_eve_mgr_work->map_size[MTD_X] - 1;
	ev_rect[MTD_TOP]	= 0;
	ev_rect[MTD_BOTTOM]	= g_gm_eve_mgr_work->map_size[MTD_Y] - 1;

	// ƒCƒxƒ“ƒg‚Ì‚İƒ`ƒFƒbƒN
	for (by_cnt = 0; by_cnt < gm_eve_data->height; by_cnt++) {
		for (bx_cnt = 0; bx_cnt < gm_eve_data->width; bx_cnt++) {
			gmEveMgrCreateEventBlkEvent(GMD_EVE_SEARCH_PROC_FLAG_ONLY_ENFORCE,
										bx_cnt, by_cnt, ev_rect, NULL);
		}
	}
	
	OS_TPrintf("<<GmEventMgrCreateEventEnforce -------- END\n");

}

// ==========================================================================
// GmEventMgrCreateEventInRect
/*!
 *	ƒCƒxƒ“ƒg¶¬ w’è‹éŒ`ƒCƒxƒ“ƒg¶¬
 *
 *	@param	left	[in]	¶¬‹éŒ`¶
 *	@param	top		[in]	¶¬‹éŒ`ã
 *	@param	right	[in]	¶¬‹éŒ`‰E
 *	@param	bottom	[in]	¶¬‹éŒ`‰º
 *
 *	@note
 *		w’è‹éŒ`“à‚Ì–¢¶¬ƒCƒxƒ“ƒg‚ğ¶¬
 */
// ==========================================================================
void GmEventMgrCreateEventInRect(u16 left, u16 top, u16 right, u16 bottom)
{
	s32		ev_rect[MTD_RECT];		// ƒCƒxƒ“ƒg¶¬‹éŒ`(ƒhƒbƒg’PˆÊ)
	u16		block_rect[MTD_RECT];
	u16		bx_cnt, by_cnt;
	s32		eve_type;
	static  void(*eve_func)(u32 flag, u16 bx, u16 by, s32 *r_on, s32 *r_off);

	MTM_ASSERT(g_gm_eve_mgr_work);

	// ƒCƒxƒ“ƒg¶¬‹éŒ` ƒ}ƒbƒvƒTƒCƒY‚ÅƒNƒŠƒbƒsƒ“ƒO
#if 1
	MTM_ASSERT(g_gm_eve_mgr_work->map_size[MTD_X] > 0);
	MTM_ASSERT(g_gm_eve_mgr_work->map_size[MTD_Y] > 0);

	ev_rect[MTD_LEFT] = left;
	if (ev_rect[MTD_LEFT] > g_gm_eve_mgr_work->map_size[MTD_X] - 1) {
		ev_rect[MTD_LEFT] = g_gm_eve_mgr_work->map_size[MTD_X] - 1;
	}
	ev_rect[MTD_TOP] = top;
	if (ev_rect[MTD_TOP] > g_gm_eve_mgr_work->map_size[MTD_Y] - 1) {
		ev_rect[MTD_TOP] = g_gm_eve_mgr_work->map_size[MTD_Y] - 1;
	}
	ev_rect[MTD_RIGHT] = right;
	if (ev_rect[MTD_RIGHT] > g_gm_eve_mgr_work->map_size[MTD_X] - 1) {
		ev_rect[MTD_RIGHT] = g_gm_eve_mgr_work->map_size[MTD_X] - 1;
	}
	ev_rect[MTD_BOTTOM] = bottom;
	if (ev_rect[MTD_BOTTOM] > g_gm_eve_mgr_work->map_size[MTD_Y] - 1) {
		ev_rect[MTD_BOTTOM] = g_gm_eve_mgr_work->map_size[MTD_Y] - 1;
	}

#else
	ev_rect[MTD_LEFT]	= (s32)MTM_MATH_CLIP(left,	0, g_gm_eve_mgr_work->map_size[MTD_X] - 1);
	ev_rect[MTD_TOP]	= (s32)MTM_MATH_CLIP(top,	0, g_gm_eve_mgr_work->map_size[MTD_Y] - 1);
	ev_rect[MTD_RIGHT]	= (s32)MTM_MATH_CLIP(right,	0, g_gm_eve_mgr_work->map_size[MTD_X] - 1);
	ev_rect[MTD_BOTTOM]	= (s32)MTM_MATH_CLIP(bottom,	0, g_gm_eve_mgr_work->map_size[MTD_Y] - 1);
#endif

	// ƒuƒƒbƒN‹éŒ`
	block_rect[MTD_LEFT]	= (u16)((ev_rect[MTD_LEFT] -  (GMD_EVE_DATA_BLOCK_SIZE-1)) >> GMD_EVE_BLOCK_SIZE_SHIFT);
	block_rect[MTD_RIGHT]	= (u16)((ev_rect[MTD_RIGHT] +  (GMD_EVE_DATA_BLOCK_SIZE-1)) >> GMD_EVE_BLOCK_SIZE_SHIFT);
	block_rect[MTD_TOP]		= (u16)((ev_rect[MTD_TOP] - (GMD_EVE_DATA_BLOCK_SIZE-1)) >> GMD_EVE_BLOCK_SIZE_SHIFT);
	block_rect[MTD_BOTTOM]	= (u16)((ev_rect[MTD_BOTTOM] + (GMD_EVE_DATA_BLOCK_SIZE-1)) >> GMD_EVE_BLOCK_SIZE_SHIFT);

	// ŠeƒCƒxƒ“ƒg‚É‚Â‚¢‚ÄA¶¬
	for (eve_type = 0; eve_type < GMD_EVE_TYPE_MAX; ++eve_type) {
		eve_func = gm_evemgr_create_eve_func_tbl[eve_type];

		for (by_cnt = block_rect[MTD_TOP]; by_cnt <= block_rect[MTD_BOTTOM]; by_cnt++) {
			for (bx_cnt = block_rect[MTD_LEFT]; bx_cnt <= block_rect[MTD_RIGHT]; bx_cnt++) {
				eve_func(GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE/*GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE_NO_REV_ALL*/,
											bx_cnt, by_cnt, ev_rect, NULL);
			}
		}
	}
}

// ==========================================================================
// GmEveMgrCreateEventAll
/*!
 *	ƒCƒxƒ“ƒg¶¬ ƒ}ƒbƒv‘S•”
 */
// ==========================================================================
void GmEveMgrCreateEventAll(void)
{
	s32		ev_rect[MTD_RECT];	// ƒCƒxƒ“ƒg¶¬‹éŒ`(ƒhƒbƒg’PˆÊ)
	u16		bx_cnt, by_cnt;
	s32		eve_type;
	static  void(*eve_func)(u32 flag, u16 bx, u16 by, s32 *r_on, s32 *r_off);

	MTM_ASSERT(g_gm_eve_mgr_work);

	// ƒCƒxƒ“ƒg¶¬‹éŒ`(ƒ}ƒbƒv‘S‘Ì)
	ev_rect[MTD_LEFT]	= 0;
	ev_rect[MTD_RIGHT]	= g_gm_eve_mgr_work->map_size[MTD_X] - 1;
	ev_rect[MTD_TOP]	= 0;
	ev_rect[MTD_BOTTOM]	= g_gm_eve_mgr_work->map_size[MTD_Y] - 1;

	// ŠeƒCƒxƒ“ƒg‚É‚Â‚¢‚ÄA¶¬
	for (eve_type = 0; eve_type < GMD_EVE_TYPE_MAX; ++eve_type) {
		eve_func = gm_evemgr_create_eve_func_tbl[eve_type];

		for (by_cnt = 0; by_cnt < gm_eve_data->height; by_cnt++) {
			for (bx_cnt = 0; bx_cnt < gm_eve_data->width; bx_cnt++) {
				eve_func(GMD_EVE_SEARCH_PROC_FLAG_NONE,
											bx_cnt, by_cnt, ev_rect, NULL);
			}
		}
	}
}

// ==========================================================================
// GmEveMgrCreateStateEvent
/*!
 *	ƒCƒxƒ“ƒgŠÇ— ‰ŠúˆÊ’uƒCƒxƒ“ƒg¶¬
 *
 *	@note
 *		ƒJƒƒ‰‚Ì‚ ‚éˆÊ’u‚ÌƒCƒxƒ“ƒg‚É‚Â‚¢‚Ä‰æ–Ê“à‘S¶¬‚ğs‚¢‚Ü‚·B
 *		ƒJƒƒ‰ˆÊ’u‚ª³‚µ‚¢ˆÊ’u‚Éİ’è‚³‚ê‚Ä‚¢‚é•K—v‚ª‚ ‚è‚Ü‚·B
 */
// ==========================================================================
void GmEveMgrCreateStateEvent(void)
{
	MTM_ASSERT(g_gm_eve_mgr_work);

	GmEveMgrCreateEventLcd(GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE);
}

// ==========================================================================
// GmEveMgrCreateEventLcd
/*!
 *	ƒCƒxƒ“ƒg¶¬ 1‰æ–Ê•ª
 *
 *	@param	ge		[in]	ƒCƒxƒ“ƒg¶¬‚ğs‚¤‰æ–Ê
 *	@param	flag	[in]	“®ìw’èƒtƒ‰ƒO
 *
 *	@note
 *		2‰æ–Ê“Æ—§ê—p‚Å2‰æ–ÊŠÔ‚ÌŒ„ŠÔ‚ğl—¶‚µ‚Ü‚¹‚ñ
 */
// ==========================================================================
#if _DS
void GmEveMgrCreateEventLcd(MTE_LCD_TYPE ge, u32 flag)
#else
void GmEveMgrCreateEventLcd(u32 flag)
#endif
{
	s32		ev_rect[MTD_RECT];	// ƒCƒxƒ“ƒg¶¬‹éŒ`(ƒhƒbƒg’PˆÊ)
	s32		lcd_rect[MTD_RECT];	// ‰æ–Ê‹éŒ`(ƒhƒbƒg’PˆÊ)
	s32		block_rect[MTD_RECT];
	s32		eve_type;
	u16		bx_cnt, by_cnt;
	s32	cam_x, cam_y, width, height;
	s32		eve_size;
	static  void(*eve_func)(u32 flag, u16 bx, u16 by, s32 *r_on, s32 *r_off);

	MTM_ASSERT(g_gm_eve_mgr_work);

	// ƒJƒƒ‰î•ñæ“¾
#if _DS
	GmCameraGetEasyScalingCameraPos(ge, &cam_x, &cam_y);
	GmCameraGetEasyScalingViewSize(&width, &height);
#else
	{
#if 1
		cam_x = g_obj.clip_camera[MTD_X] >> FX32_SHIFT;
		cam_y = g_obj.clip_camera[MTD_Y] >> FX32_SHIFT;
		width = OBD_OBJ_CLIP_LCD_X;
		height= OBD_OBJ_CLIP_LCD_Y;
#else
		cam_x = g_obj.camera[0][MTD_X] >> FX32_SHIFT;
		cam_y = g_obj.camera[0][MTD_Y] >> FX32_SHIFT;
		width = OBD_OBJ_CLIP_LCD_X;
		height= OBD_OBJ_CLIP_LCD_Y;
#endif
	}
#endif

	// ‰æ–Ê•\¦‹éŒ`(¶¬‹Ö~‹éŒ`ƒx[ƒX)
	lcd_rect[MTD_LEFT]		= cam_x;
	lcd_rect[MTD_RIGHT]		= (cam_x + width);
	lcd_rect[MTD_TOP]		= cam_y;
	lcd_rect[MTD_BOTTOM]	= (cam_y + height);

	// ƒCƒxƒ“ƒg¶¬‹éŒ` ƒ}ƒbƒvƒTƒCƒY‚ÅƒNƒŠƒbƒsƒ“ƒO
	lcd_rect[MTD_LEFT]	= MTM_MATH_CLIP(lcd_rect[MTD_LEFT],		0, g_gm_eve_mgr_work->map_size[MTD_X] - 1);
	lcd_rect[MTD_TOP]	= MTM_MATH_CLIP(lcd_rect[MTD_TOP],		0, g_gm_eve_mgr_work->map_size[MTD_Y] - 1);
	lcd_rect[MTD_RIGHT]	= MTM_MATH_CLIP(lcd_rect[MTD_RIGHT],	0, g_gm_eve_mgr_work->map_size[MTD_X] - 1);
	lcd_rect[MTD_BOTTOM]= MTM_MATH_CLIP(lcd_rect[MTD_BOTTOM],	0, g_gm_eve_mgr_work->map_size[MTD_Y] - 1);
	//rect_lcd[MTD_LEFT]  = cam->pos[MTD_X] >> FX32_SHIFT;
	//rect_lcd[MTD_TOP]   = cam->pos[MTD_Y] >> FX32_SHIFT;
	//rect_lcd[MTD_RIGHT] =(cam->pos[MTD_X] + GX_LCD_SIZE_X * cam->scale[MTD_X]) >> FX32_SHIFT;
	//rect_lcd[MTD_BOTTOM]=(cam->pos[MTD_Y] + GX_LCD_SIZE_Y * cam->scale[MTD_Y]) >> FX32_SHIFT;

	for (eve_type = 0; eve_type < GMD_EVE_TYPE_MAX; ++eve_type) {
		eve_func = gm_evemgr_create_eve_func_tbl[eve_type];
		eve_size = gm_evemgr_create_size_tbl[eve_type];		// Å‘åƒ`ƒFƒbƒN”ÍˆÍ

		// ƒCƒxƒ“ƒg¶¬ƒx[ƒX‹éŒ`
#if 1
		ev_rect[MTD_LEFT]		= lcd_rect[MTD_LEFT];
		ev_rect[MTD_RIGHT]		= lcd_rect[MTD_RIGHT];
		ev_rect[MTD_TOP]		= lcd_rect[MTD_TOP];
		ev_rect[MTD_BOTTOM]		= lcd_rect[MTD_BOTTOM];
#else
		ev_rect[MTD_LEFT]		= lcd_rect[MTD_LEFT] - GMD_MAIN_SCR_SPD_MAX - eve_size;
		ev_rect[MTD_RIGHT]		= lcd_rect[MTD_RIGHT] + GMD_MAIN_SCR_SPD_MAX + eve_size;
		ev_rect[MTD_TOP]		= lcd_rect[MTD_TOP] - GMD_MAIN_SCR_SPD_MAX - eve_size;
		ev_rect[MTD_BOTTOM]		= lcd_rect[MTD_BOTTOM] + GMD_MAIN_SCR_SPD_MAX + eve_size;
		//ev_rect[MTD_LEFT]   = (cam->pos[MTD_X] >> FX32_SHIFT) - eve_size;
		//ev_rect[MTD_TOP]	= (cam->pos[MTD_Y] >> FX32_SHIFT) - eve_size;
		//ev_rect[MTD_RIGHT]  = ((cam->pos[MTD_X] + GX_LCD_SIZE_X * cam->scale[MTD_X]) >> FX32_SHIFT) + eve_size;
		//ev_rect[MTD_BOTTOM] = ((cam->pos[MTD_Y] + GX_LCD_SIZE_Y * cam->scale[MTD_Y]) >> FX32_SHIFT) + eve_size;
#endif

		// ƒ`ƒFƒbƒNƒuƒƒbƒN‹éŒ`
#if 1
		block_rect[MTD_LEFT]	= (ev_rect[MTD_LEFT] - GMD_MAIN_SCR_SPD_MAX - eve_size - (GMD_EVE_DATA_BLOCK_SIZE-1))
											>> GMD_EVE_BLOCK_SIZE_SHIFT;
		block_rect[MTD_RIGHT]	= (ev_rect[MTD_RIGHT] + GMD_MAIN_SCR_SPD_MAX + eve_size + (GMD_EVE_DATA_BLOCK_SIZE-1))
											>> GMD_EVE_BLOCK_SIZE_SHIFT;
		block_rect[MTD_TOP]		= (ev_rect[MTD_TOP] - GMD_MAIN_SCR_SPD_MAX - eve_size - (GMD_EVE_DATA_BLOCK_SIZE-1))
											>> GMD_EVE_BLOCK_SIZE_SHIFT;
		block_rect[MTD_BOTTOM]	= (ev_rect[MTD_BOTTOM] + GMD_MAIN_SCR_SPD_MAX + eve_size + (GMD_EVE_DATA_BLOCK_SIZE-1))
											>> GMD_EVE_BLOCK_SIZE_SHIFT;
#else
		block_rect[MTD_LEFT]	= ev_rect[MTD_LEFT] >> GMD_EVE_BLOCK_SIZE_SHIFT;
		block_rect[MTD_RIGHT]	= ev_rect[MTD_RIGHT] >> GMD_EVE_BLOCK_SIZE_SHIFT;
		block_rect[MTD_TOP]		= ev_rect[MTD_TOP] >> GMD_EVE_BLOCK_SIZE_SHIFT;
		block_rect[MTD_BOTTOM]	= ev_rect[MTD_BOTTOM] >> GMD_EVE_BLOCK_SIZE_SHIFT;
#endif

		if (block_rect[MTD_LEFT] < 0) {
			block_rect[MTD_LEFT] = 0;
		}
		if (block_rect[MTD_RIGHT] >= gm_eve_data->width) {
			block_rect[MTD_RIGHT] = gm_eve_data->width - 1;
		}
		if (block_rect[MTD_TOP] < 0) {
			block_rect[MTD_TOP] = 0;
		}
		if (block_rect[MTD_BOTTOM] >= gm_eve_data->height) {
			block_rect[MTD_BOTTOM] = gm_eve_data->height - 1;
		}

		for (by_cnt = (u16)block_rect[MTD_TOP]; by_cnt <= (u16)block_rect[MTD_BOTTOM]; by_cnt++) {
			for (bx_cnt = (u16)block_rect[MTD_LEFT]; bx_cnt <= (u16)block_rect[MTD_RIGHT]; bx_cnt++) {
				eve_func(flag, bx_cnt, by_cnt, ev_rect, lcd_rect);
			}
		}
	}
}

#if _DS
// ==========================================================================
// GmEveMgrSearchEventLcdDS
/*!
 *	ƒCƒxƒ“ƒg¶¬ 2‰æ–Ê•ª
 *
 *	@param	flag	[in]	“®ìw’èƒtƒ‰ƒO
 *
 *	@note
 *		2‰æ–Ê˜A“®ê—p‚Å2‰æ–ÊŠÔ‚ÌŒ„ŠÔ‚àl—¶‚µ‚Ü‚·
 */
// ==========================================================================
void GmEveMgrSearchEventLcdDS(u32 flag)
{
	s32		ev_rect[MTD_RECT];	// ƒCƒxƒ“ƒg¶¬‹éŒ`(ƒhƒbƒg’PˆÊ)
	s32		lcd_rect[MTD_RECT];	// ‰æ–Ê‹éŒ`(ƒhƒbƒg’PˆÊ)
	s32		block_rect[MTD_RECT];
	u16		bx_cnt, by_cnt;
	fx32	cam_up_x, cam_up_y, cam_down_x, cam_down_y, width, height;

	MTM_ASSERT(g_gm_eve_mgr_work);
	MTM_ASSERT(g_obj.flag & OBD_OBJ_CAMERA_STICK);

	// ƒJƒƒ‰î•ñæ“¾
	GmCameraGetEasyScalingCameraPos(MTE_LCD_UP, &cam_up_x, &cam_up_y);
	GmCameraGetEasyScalingCameraPos(MTE_LCD_DOWN, &cam_down_x, &cam_down_y);
	GmCameraGetEasyScalingViewSize(&width, &height);
	if (cam_down_y < cam_up_y) {
		MTM_ASSERT(0);
		MTM_MATH_SWAP(cam_up_y, cam_down_y);
	}

	// ‰æ–Ê•\¦‹éŒ`(¶¬‹Ö~‹éŒ`ƒx[ƒX)
	lcd_rect[MTD_LEFT]		= cam_up_x >> FX32_SHIFT;
	lcd_rect[MTD_RIGHT]		= (cam_up_x + width) >> FX32_SHIFT;
	lcd_rect[MTD_TOP]		= cam_up_y >> FX32_SHIFT;
	lcd_rect[MTD_BOTTOM]	= (cam_down_y + height) >> FX32_SHIFT;

	// ƒCƒxƒ“ƒg¶¬ƒx[ƒX‹éŒ`
	ev_rect[MTD_LEFT]		= lcd_rect[MTD_LEFT] - GMD_MAIN_SCR_SPD_MAX;
	ev_rect[MTD_RIGHT]		= lcd_rect[MTD_RIGHT] + GMD_MAIN_SCR_SPD_MAX;
	ev_rect[MTD_TOP]		= lcd_rect[MTD_TOP] - GMD_MAIN_SCR_SPD_MAX;
	ev_rect[MTD_BOTTOM]		= lcd_rect[MTD_BOTTOM] + GMD_MAIN_SCR_SPD_MAX;

	// ƒ`ƒFƒbƒNƒuƒƒbƒN‹éŒ`
	block_rect[MTD_LEFT]	= (ev_rect[MTD_LEFT] - gm_eve_create_check_size_event - (GMD_EVE_DATA_BLOCK_SIZE-1)) >> GMD_EVE_BLOCK_SIZE_SHIFT;
	block_rect[MTD_RIGHT]	= (ev_rect[MTD_RIGHT] + gm_eve_create_check_size_event + (GMD_EVE_DATA_BLOCK_SIZE-1)) >> GMD_EVE_BLOCK_SIZE_SHIFT;
	block_rect[MTD_TOP]		= (ev_rect[MTD_TOP] - gm_eve_create_check_size_event - (GMD_EVE_DATA_BLOCK_SIZE-1)) >> GMD_EVE_BLOCK_SIZE_SHIFT;
	block_rect[MTD_BOTTOM]	= (ev_rect[MTD_BOTTOM] + gm_eve_create_check_size_event + (GMD_EVE_DATA_BLOCK_SIZE-1)) >> GMD_EVE_BLOCK_SIZE_SHIFT;

	if (block_rect[MTD_LEFT] < 0) {
		block_rect[MTD_LEFT] = 0;
	}
	if (block_rect[MTD_RIGHT] >= gm_eve_data->width) {
		block_rect[MTD_RIGHT] = gm_eve_data->width - 1;
	}
	if (block_rect[MTD_TOP] < 0) {
		block_rect[MTD_TOP] = 0;
	}
	if (block_rect[MTD_BOTTOM] >= gm_eve_data->height) {
		block_rect[MTD_BOTTOM] = gm_eve_data->height - 1;
	}

	for (by_cnt = (u16)block_rect[MTD_TOP]; by_cnt <= (u16)block_rect[MTD_BOTTOM]; by_cnt++) {
		for (bx_cnt = (u16)block_rect[MTD_LEFT]; bx_cnt <= (u16)block_rect[MTD_RIGHT]; bx_cnt++) {
			gmEveMgrCreateEventBlkEvent(flag, bx_cnt, by_cnt, ev_rect, lcd_rect);
		}
	}
}
#endif

// ==========================================================================
// ƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒIƒuƒWƒFƒNƒg
// ==========================================================================
// ==========================================================================
// GmEventMgrLocalEventBirth
/*!
 *	ƒ[ƒJƒ‹ƒCƒxƒ“ƒg‚Ì¶¬
 *
 *	@param	id		[in]	¶¬‚·‚éƒIƒuƒWƒFƒNƒgID (GMS_EVE_RECORD_EVENT:id)
 *	@param	pos_x	[in]	oŒ»À•WX
 *	@param	pos_y	[in]	oŒ»À•WY
 *	@param	flag	[in]	GMS_EVE_RECORD_EVENT:flag ‚Éİ’è‚·‚éƒtƒ‰ƒO
 *	@param	left	[in]	GMS_EVE_RECORD_EVENT:left ‚Éİ’è‚·‚éƒtƒ‰ƒO
 *	@param	top		[in]	GMS_EVE_RECORD_EVENT:top ‚Éİ’è‚·‚éƒtƒ‰ƒO
 *	@param	width	[in]	GMS_EVE_RECORD_EVENT:width ‚Éİ’è‚·‚éƒtƒ‰ƒO
 *	@param	height	[in]	GMS_EVE_RECORD_EVENT:height ‚Éİ’è‚·‚éƒtƒ‰ƒO
 *	@param	type	[in]	ƒCƒxƒ“ƒg¶¬ŠÖ”ŒÄ‚Ño‚µƒ^ƒCƒv ’Êí‚Í0
 *
 *	@return	¶¬‚µ‚½ƒIƒuƒWƒFƒNƒg‚Ìƒ[ƒNƒ|ƒCƒ“ƒ^
 */
// ==========================================================================
OBS_OBJECT_WORK* GmEventMgrLocalEventBirth(u16 id, fx32 pos_x, fx32 pos_y, u16 flag, s8 left, s8 top, u8 width, u8 height, u8 type)
{
	OBS_OBJECT_WORK	*obj_work = NULL;
	s16				local_no;

	// ‹ó‚«ƒ[ƒJƒ‹ƒ}ƒbƒvƒIƒuƒWƒFæ“¾
	local_no = gmEventMgrLocalEventNoGet(GMD_EVE_TYPE_EVENT);

	// ‹ó‚«ƒ[ƒJƒ‹ƒ}ƒbƒvƒIƒuƒWƒF•s‘«ƒ`ƒFƒbƒN
	if (local_no != -1) {
		gm_eve_local_evt_record[local_no].pos_x	= 0xff;			// X‚ÆY‚ ‚í‚¹‚Äƒ[ƒJƒ‹—pİ’è
		gm_eve_local_evt_record[local_no].pos_y	= 0xff;
		gm_eve_local_evt_record[local_no].id	= (u16)id;

		gm_eve_local_evt_record[local_no].flag	= flag;

		gm_eve_local_evt_record[local_no].left	= left;
		gm_eve_local_evt_record[local_no].top	= top;
		gm_eve_local_evt_record[local_no].width	= width;
		gm_eve_local_evt_record[local_no].height= height;
		gm_eve_local_evt_record[local_no].word_param= 0;

		// ƒCƒxƒ“ƒg¶¬
		OS_TPrintf("LOCAL EVENT BIRTH: id=%d\n", id);//qqq
		obj_work = g_gm_event_tbl[id](&gm_eve_local_evt_record[local_no], pos_x, pos_y, type);

		if (obj_work == NULL) {
			// ƒIƒuƒWƒFƒNƒgì¬ƒ^ƒCƒvƒCƒxƒ“ƒg‚Å‚È‚¯‚ê‚Îƒ[ƒJƒ‹ƒCƒxƒ“ƒg‰ğ•ú
			GmEventMgrLocalEventRelease(&gm_eve_local_evt_record[local_no]);
		}
	}
#if defined (MTD_DEBUG)
	else {
		MTM_ASSERT(!"gmEventMgr::GmEventMgrLocalEventBirth() Error! no record work!\n" );
	}
#endif

	return (obj_work);
}

// ==========================================================================
// GmEventMgrLocalRingBirth
/*!
 *	ƒ[ƒJƒ‹ƒŠƒ“ƒO‚Ì¶¬
 *
 *	@param	pos_x	[in]	oŒ»À•WX
 *	@param	pos_y	[in]	oŒ»À•WY
 *	@param	type	[in]	ƒCƒxƒ“ƒg¶¬ŠÖ”ŒÄ‚Ño‚µƒ^ƒCƒv ’Êí‚Í0
 *
 *	@return	¶¬‚µ‚½ƒIƒuƒWƒFƒNƒg‚Ìƒ[ƒNƒ|ƒCƒ“ƒ^
 */
// ==========================================================================
void GmEventMgrLocalRingBirth(fx32 pos_x, fx32 pos_y, u8 type)
{
	s16				local_no;

	// ‹ó‚«ƒ[ƒJƒ‹ƒ}ƒbƒvƒIƒuƒWƒFæ“¾
	local_no = gmEventMgrLocalEventNoGet(GMD_EVE_TYPE_RING);

	// ‹ó‚«ƒ[ƒJƒ‹ƒ}ƒbƒvƒIƒuƒWƒF•s‘«ƒ`ƒFƒbƒN
	if (local_no != -1) {
		gm_eve_local_ring_record[local_no].pos_x	= 0xff;			// X‚ÆY‚ ‚í‚¹‚Äƒ[ƒJƒ‹—pİ’è
		gm_eve_local_ring_record[local_no].pos_y	= 0xff;

		// ƒCƒxƒ“ƒg¶¬
		GmRingCreate(&gm_eve_local_ring_record[local_no],
				pos_x, pos_y, type);
	}
#if defined (MTD_DEBUG)
	else {
		MTM_ASSERT(!"gmEventMgr::GmEventMgrLocalRingBirth() Error! no record work!\n" );
	}
#endif

	return;
}

// ==========================================================================
// GmEventMgrLocalDecoBirth
/*!
 *	ƒ[ƒJƒ‹ƒCƒxƒ“ƒg(‘•ü)‚Ì¶¬
 *
 *	@param	id		[in]	¶¬‚·‚éƒIƒuƒWƒFƒNƒgID (GMS_EVE_RECORD_EVENT:id)
 *	@param	pos_x	[in]	oŒ»À•WX
 *	@param	pos_y	[in]	oŒ»À•WY
 *	@param	type	[in]	ƒCƒxƒ“ƒg¶¬ŠÖ”ŒÄ‚Ño‚µƒ^ƒCƒv ’Êí‚Í0
 *
 *	@return	¶¬‚µ‚½ƒIƒuƒWƒFƒNƒg‚Ìƒ[ƒNƒ|ƒCƒ“ƒ^
 */
// ==========================================================================
OBS_OBJECT_WORK* GmEventMgrLocalDecoBirth(u16 id, fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK	*obj_work = NULL;
	s16				local_no;

	// ‹ó‚«ƒ[ƒJƒ‹ƒ}ƒbƒvƒIƒuƒWƒFæ“¾
	local_no = gmEventMgrLocalEventNoGet(GMD_EVE_TYPE_DECO);

	// ‹ó‚«ƒ[ƒJƒ‹ƒ}ƒbƒvƒIƒuƒWƒF•s‘«ƒ`ƒFƒbƒN
	if (local_no != -1) {
		gm_eve_local_deco_record[local_no].pos_x	= 0xff;			// X‚ÆY‚ ‚í‚¹‚Äƒ[ƒJƒ‹—pİ’è
		gm_eve_local_deco_record[local_no].pos_y	= 0xff;
		gm_eve_local_deco_record[local_no].id	= (u16)id;

		// ƒCƒxƒ“ƒg¶¬
		obj_work = g_gm_decorate_tbl[id](&gm_eve_local_deco_record[local_no], pos_x, pos_y, type);

		if (obj_work == NULL) {
			// ƒIƒuƒWƒFƒNƒgì¬ƒ^ƒCƒvƒCƒxƒ“ƒg‚Å‚È‚¯‚ê‚Îƒ[ƒJƒ‹ƒCƒxƒ“ƒg‰ğ•ú
			GmEventMgrLocalDecoRelease(&gm_eve_local_deco_record[local_no]);
		}
	}
#if defined (MTD_DEBUG)
	else {
		MTM_ASSERT(!"gmEventMgr::GmEventMgrLocalDecoBirth() Error! no record work!\n" );
	}
#endif

	return (obj_work);
}

// ==========================================================================
// gmEventMgrLocalEventNoGet
/*!
 *	‹ó‚¢‚Ä‚éƒ[ƒJƒ‹ƒ}ƒbƒvƒIƒuƒWƒFƒNƒgNO‚ğæ“¾‚·‚é
 *
 *	@param	eve_type	[in]	ƒCƒxƒ“ƒgƒ^ƒCƒv
 *
 *	@return æ“¾‚µ‚½ƒ[ƒJƒ‹ƒCƒxƒ“ƒg”Ô†  -1‚Åæ“¾–³‚µ
 */
// ==========================================================================
s16 gmEventMgrLocalEventNoGet(GME_EVE_TYPE eve_type)
{
	s16		i, j;
	s16		max_num;
	u32		*use_flag;

	switch (eve_type) {
	default:
		MTM_ASSERT(0);
	case GMD_EVE_TYPE_EVENT:
		use_flag = gm_eve_local_evt_obj_use_flag;
		max_num = GMD_EVE_LOCAL_EVT_USE_FLAG_WORK_NUM;
		break;
	case GMD_EVE_TYPE_RING:
		use_flag = gm_eve_local_ring_obj_use_flag;
		max_num = GMD_EVE_LOCAL_RING_USE_FLAG_WORK_NUM;
		break;
	case GMD_EVE_TYPE_DECO:
		use_flag = gm_eve_local_deco_obj_use_flag;
		max_num = GMD_EVE_LOCAL_DECO_USE_FLAG_WORK_NUM;
		break;
	}

#if 1
	for (i = 0; i < max_num; i++) {

		if (*(use_flag + i) < 0xFFFFFFFF) {

			for (j = 0; j < 32; j++) {
				if ( !(*(use_flag + i) & (1 << j)) ) {
					*(use_flag + i) |= 1 << j;
					return ((s16)(i * 32 + j));
				}
			}
		}
	}
	return (-1);
#else
	-/*
	for (i = 0; i < GMD_EVE_LOCAL_EVT_USE_FLAG_WORK_NUM; i++) {

		if (gm_eve_local_evt_obj_use_flag[i] < 0xFFFFFFFF) {

			for (j = 0; j < 32; j++) {
				if ( !(gm_eve_local_evt_obj_use_flag[i] & (1 << j)) ) {
					gm_eve_local_evt_obj_use_flag[i] |= 1 << j;
					return ((s16)(i * 32 + j));
				}
			}
		}
	}
	return (GMD_EVE_LOCAL_EVT_OBJ_MAX);
	 */
#endif

}

// ==========================================================================
// GmEventMgrLocalEventRelease
/*!
 *	ƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒ[ƒNg—pƒtƒ‰ƒO‚ğQ‚©‚·@(–¢g—pó‘Ô‚É‚·‚é)
 *
 *	@param eve_rec	[in] ŠJ•ú‚·‚éƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒ[ƒN
 *
 */
// ==========================================================================
void GmEventMgrLocalEventRelease(GMS_EVE_RECORD_EVENT *eve_rec)
{
	u32						no, ofst;

	MTM_ASSERT(eve_rec);

	/* ƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒIƒuƒWƒFƒNƒgƒoƒbƒtƒ@‚ğ•Ô‹p */
	ofst = (u32)eve_rec - (u32)&gm_eve_local_evt_record[0];
	no = (u32)FX_DivS32((s32)ofst, sizeof(GMS_EVE_RECORD_EVENT));			// •Ô‹p‚³‚ê‚½ƒoƒbƒtƒ@NO‚ğæ“¾

#if defined (MTD_DEBUG)
	if (&gm_eve_local_evt_record[no] != eve_rec) {
		// ƒAƒhƒŒƒXƒGƒ‰[
		MTM_ASSERT(0 && "gmEventMgr:gmEventMgrLocalEventRelease Error! Invalid address!\n");
		return;
	}
	// ‰ğ•úÏ‚İƒ`ƒFƒbƒN
	MTM_ASSERT((gm_eve_local_evt_obj_use_flag[no >> 5] & (1 << (no & 0x1F))) && "gmEventMgr:gmEventMgrLocalEventRelease Error! Invalid address!\n");
#endif

	// g—p’†ƒtƒ‰ƒO‚ğ‚¨‚Æ‚·
	gm_eve_local_evt_obj_use_flag[no >> 5] &= ~(1 << (no & 0x1F));
}

// ==========================================================================
// GmEventMgrLocalRingRelease
/*!
 *	ƒ[ƒJƒ‹‘•üƒ[ƒNg—pƒtƒ‰ƒO‚ğQ‚©‚·@(–¢g—pó‘Ô‚É‚·‚é)
 *
 *	@param eve_rec	[in] ŠJ•ú‚·‚éƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒ[ƒN
 *
 */
// ==========================================================================
void GmEventMgrLocalRingRelease(GMS_EVE_RECORD_RING *eve_rec)
{
	u32						no, ofst;

	MTM_ASSERT(eve_rec);

	/* ƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒIƒuƒWƒFƒNƒgƒoƒbƒtƒ@‚ğ•Ô‹p */
	ofst = (u32)eve_rec - (u32)&gm_eve_local_ring_record[0];
	no = (u32)FX_DivS32((s32)ofst, sizeof(GMS_EVE_RECORD_RING));			// •Ô‹p‚³‚ê‚½ƒoƒbƒtƒ@NO‚ğæ“¾

#if defined (MTD_DEBUG)
	if (&gm_eve_local_ring_record[no] != eve_rec) {
		// ƒAƒhƒŒƒXƒGƒ‰[
		MTM_ASSERT(0 && "gmEventMgr:GmEventMgrLocalRingRelease Error! Invalid address!\n");
		return;
	}
	// ‰ğ•úÏ‚İƒ`ƒFƒbƒN
	MTM_ASSERT((gm_eve_local_ring_obj_use_flag[no >> 5] & (1 << (no & 0x1F))) && "gmEventMgr:GmEventMgrLocalRingRelease Error! Invalid address!\n");
#endif

	// g—p’†ƒtƒ‰ƒO‚ğ‚¨‚Æ‚·
	gm_eve_local_ring_obj_use_flag[no >> 5] &= ~(1 << (no & 0x1F));
}

// ==========================================================================
// GmEventMgrLocalDecoRelease
/*!
 *	ƒ[ƒJƒ‹‘•üƒ[ƒNg—pƒtƒ‰ƒO‚ğQ‚©‚·@(–¢g—pó‘Ô‚É‚·‚é)
 *
 *	@param eve_rec	[in] ŠJ•ú‚·‚éƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒ[ƒN
 *
 */
// ==========================================================================
void GmEventMgrLocalDecoRelease(GMS_EVE_RECORD_DECORATE *eve_rec)
{
	u32						no, ofst;

	MTM_ASSERT(eve_rec);

	/* ƒ[ƒJƒ‹ƒCƒxƒ“ƒgƒIƒuƒWƒFƒNƒgƒoƒbƒtƒ@‚ğ•Ô‹p */
	ofst = (u32)eve_rec - (u32)&gm_eve_local_deco_record[0];
	no = (u32)FX_DivS32((s32)ofst, sizeof(GMS_EVE_RECORD_EVENT));			// •Ô‹p‚³‚ê‚½ƒoƒbƒtƒ@NO‚ğæ“¾

#if defined (MTD_DEBUG)
	if (&gm_eve_local_deco_record[no] != eve_rec) {
		// ƒAƒhƒŒƒXƒGƒ‰[
		MTM_ASSERT(0 && "gmEventMgr:GmEventMgrLocalDecoRelease Error! Invalid address!\n");
		return;
	}
	// ‰ğ•úÏ‚İƒ`ƒFƒbƒN
	MTM_ASSERT((gm_eve_local_deco_obj_use_flag[no >> 5] & (1 << (no & 0x1F))) && "gmEventMgr:GmEventMgrLocalDecoRelease Error! Invalid address!\n");
#endif

	// g—p’†ƒtƒ‰ƒO‚ğ‚¨‚Æ‚·
	gm_eve_local_deco_obj_use_flag[no >> 5] &= ~(1 << (no & 0x1F));
}

#if 0
// ==========================================================================
// GmEventMgrGetEventNumId
/*!
 *	w’èID‚ÌƒCƒxƒ“ƒg”‚ğæ“¾‚·‚é
 *
 *	@param	id			[in]	”‚ğæ“¾‚·‚éID
 *	@param	live_num	[out]	w’èIDƒCƒxƒ“ƒg‚Å¶‚«‚Ä‚¢‚éƒCƒxƒ“ƒg‚Ì”(NULL‰Â)
 *
 *	@return w’èID‚ÌƒCƒxƒ“ƒg”
 */
// ==========================================================================
s32 GmEventMgrGetEventNumId(u16 id, u16 *live_num)
{
	s32						i, j;
	GMS_EVE_DATA_EV_LIST	*eve_list;
	GMS_EVE_RECORD_EVENT	*eve_rec;
	s32						block_max;
	s32						eve_num, live_eve_num;

	MTM_ASSERT(gm_eve_data);

	eve_num = live_eve_num = 0;

	// ƒCƒxƒ“ƒgî•ñƒŠƒXƒgæ“¾
	block_max = gm_eve_data->width * gm_eve_data->height;

	for (i = 0; i < block_max; i++) {
		eve_list = (GMS_EVE_DATA_EV_LIST*)((u32)gm_eve_data + gm_eve_data->ofst[i]);

		eve_rec = &eve_list->eve_rec[0];
		for (j = 0; j < eve_list->eve_num; j++, eve_rec++) {
			if (eve_rec->id == id) {
				eve_num++;
				if (eve_rec->pos_x != GMD_EVE_RECORD_CMD_SKIP) {
					// ¶‘¶’†
					live_eve_num++;
				}
			}
		}
	}

	if (live_num) {
		*live_num = live_eve_num;
	}

	return (eve_num);
}
#endif

// ==========================================================================
// GmEventMgrSearchEventWorkInit
/*!
 *	ƒCƒxƒ“ƒgŒŸõ—pƒ[ƒN‰Šú‰»
 *
 *	@param	eve_search_work [io]	GMS_EVE_SEARCH_WORK
 */
// ==========================================================================
void GmEventMgrSearchEventWorkInit(GMS_EVE_SEARCH_WORK *eve_search_work)
{
	GMS_EVE_DATA_EV_LIST	*eve_list;

	MTM_ASSERT(eve_search_work);
	MTM_ASSERT(gm_eve_data);

	eve_list = (GMS_EVE_DATA_EV_LIST*)((u32)gm_eve_data + gm_eve_data->ofst[0]);	// ƒCƒxƒ“ƒgî•ñƒŠƒXƒg

	eve_search_work->block_no	= 0;
	eve_search_work->eve_rec_top= &eve_list->eve_rec[0];
	eve_search_work->eve_no		= -1;
}

// ==========================================================================
// GmEventMgrSearchEvent
/*!
 *	ƒCƒxƒ“ƒg‚ğ‡”Ô‚ÉŒŸõ‚·‚é
 *
 *	@return æ“¾‚µ‚½ƒ[ƒJƒ‹ƒCƒxƒ“ƒg”Ô†
 */
// ==========================================================================
GMS_EVE_RECORD_EVENT* GmEventMgrSearchEvent(GMS_EVE_SEARCH_WORK *eve_search_work)
{
	GMS_EVE_DATA_EV_LIST	*eve_list;

	MTM_ASSERT(eve_search_work);
	MTM_ASSERT(gm_eve_data);

	if (eve_search_work->block_no >= gm_eve_data->width * gm_eve_data->height) {
		// ƒf[ƒ^I—¹
		return (NULL);
	}

	// ƒCƒxƒ“ƒgî•ñƒŠƒXƒgæ“¾
	eve_list = (GMS_EVE_DATA_EV_LIST*)((u32)gm_eve_data + gm_eve_data->ofst[eve_search_work->block_no]);

	eve_search_work->eve_no++;
	if (eve_search_work->eve_no >= eve_list->eve_num) {
		// Ÿ‚ÌƒuƒƒbƒN‚Ö
		eve_search_work->block_no++;
		if (eve_search_work->block_no >= gm_eve_data->width * gm_eve_data->height) {
			// ƒf[ƒ^I—¹
			return (NULL);
		}
		// ƒCƒxƒ“ƒgî•ñƒŠƒXƒgÄæ“¾
		eve_list = (GMS_EVE_DATA_EV_LIST*)((u32)gm_eve_data + gm_eve_data->ofst[eve_search_work->block_no]);
		// ƒCƒxƒ“ƒgNO“ª‚¾‚µ
		eve_search_work->eve_no = 0;
	}

	return (&eve_list->eve_rec[eve_search_work->eve_no]);
}

// ==========================================================================
// Utility
// ==========================================================================
// ==========================================================================
// GmEventMgrEveAddr2OfstEveAddr
/*!
 *	ƒCƒxƒ“ƒgƒAƒhƒŒƒX‚ğƒIƒtƒZƒbƒgƒAƒhƒŒƒX‚É•ÏŠ·
 *
 *	@param	eve_addr	[in]	ƒCƒxƒ“ƒgƒAƒhƒŒƒX(GMS_EVE_RECORD_EVENT)
 *
 *	@return	ƒIƒtƒZƒbƒgƒAƒhƒŒƒX
 */
// ==========================================================================
u32 GmEventMgrEveAddr2OfstEveAddr(u32 eve_addr)
{
	u32	ofst_addr = NULL;
	if (gm_eve_data) {
		ofst_addr = eve_addr - (u32)gm_eve_data;
	}

	return (ofst_addr);
}

// ==========================================================================
// GmEventMgrGetRingNum
/*!
 *	ƒXƒe[ƒW“àƒŠƒ“ƒO”‚ğæ“¾
 *
 *	@return	ƒXƒe[ƒW“àƒŠƒ“ƒO”
 */
// ==========================================================================
u32 GmEventMgrGetRingNum(void)
{
	u32						i, block_num;
	u32						ring_num = 0;

	if (gm_ring_data) {
		block_num = gm_ring_data->width * gm_ring_data->height;

		for (i = 0; i < block_num; i++) {
			ring_num += ((GMS_EVE_DATA_RG_LIST*)((u32)gm_ring_data + gm_ring_data->ofst[i]))->ring_num;
		}
	}

	return (ring_num);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// ƒƒCƒ“ˆ—
// ==========================================================================
// ==========================================================================
// gmEveMgrMain
/*!
 *	ƒCƒxƒ“ƒgŠÇ— ƒƒCƒ“ˆ—
 */
// ==========================================================================
void gmEveMgrMain(MTS_TASK_TCB *tcb)
{
#if _DS
	GMS_EVE_MGR_WORK	*mgr_work = (GMS_EVE_MGR_WORK*)mtTaskGetTcbWork(tcb);

	// ó‘ÔŠÖ”Às
	if (mgr_work->sts_proc != NULL) {
		mgr_work->sts_proc();
	}

	// Œ»İ‚ÌƒJƒƒ‰ˆÊ’u‚ğ•Û‘¶
	GmCameraGetLCDCameraPos(MTE_LCD_UP,
					&mgr_work->prev_pos[MTE_LCD_UP][MTD_X],
					&mgr_work->prev_pos[MTE_LCD_UP][MTD_Y]);
	GmCameraGetLCDCameraPos(MTE_LCD_DOWN,
					&mgr_work->prev_pos[MTE_LCD_DOWN][MTD_X],
					&mgr_work->prev_pos[MTE_LCD_DOWN][MTD_Y]);
#else
	
	GMS_EVE_MGR_WORK	*mgr_work = (GMS_EVE_MGR_WORK*)mtTaskGetTcbWork(tcb);
	OBS_CAMERA			*camera;

	// ó‘ÔŠÖ”Às
	if (mgr_work->sts_proc != NULL) {
		mgr_work->sts_proc();
	}

	camera = ObjCameraGet(g_obj.glb_camera_id);

	if (camera) {
		mgr_work->prev_pos[MTD_X] = camera->disp_pos.x;
		mgr_work->prev_pos[MTD_Y] = camera->disp_pos.y;
	}
#endif
//	g_gm_eve_mgr_work->prev_pos[MTE_GE2_A][MTD_X] = g_gm_camera_work->map_cam[MTE_GE2_A].pos[MTD_X];
//	g_gm_eve_mgr_work->prev_pos[MTE_GE2_A][MTD_Y] = g_gm_camera_work->map_cam[MTE_GE2_A].pos[MTD_Y];
//	g_gm_eve_mgr_work->prev_pos[MTE_GE2_B][MTD_X] = g_gm_camera_work->map_cam[MTE_GE2_B].pos[MTD_X];
//	g_gm_eve_mgr_work->prev_pos[MTE_GE2_B][MTD_Y] = g_gm_camera_work->map_cam[MTE_GE2_B].pos[MTD_Y];
}

// ================================================================
// gmEveMgrDest
/*!
  ƒCƒxƒ“ƒgŠÇ— ƒfƒXƒgƒ‰ƒNƒ^
 */
// ================================================================
void gmEveMgrDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	g_gm_eve_mgr_work	= NULL;
	gm_eve_mgr_tcb		= NULL;
}


// ==========================================================================
// ó‘Ô–ˆˆ—
// ==========================================================================
// ==========================================================================
// gmEveMgrStateFuncInit
/*!
 *	ƒCƒxƒ“ƒgŠÇ— ó‘Ô–ˆˆ— ‰Šúˆ—
 */
// ==========================================================================
#if 0
void gmEveMgrStateFuncInit(void)
{
	if (1) {
		// 2‰æ–Ê˜A“®

		// ƒCƒxƒ“ƒg‰Šú“]‘—


		// ˆ—Ø‚è‘Ö‚¦
		g_gm_eve_mgr_work->sts_proc = gmEveMgrStateFuncDependScr;
	}
	else {
		// 2‰æ–Ê“Æ—§

		// ƒCƒxƒ“ƒg‰Šú“]‘—

		// ˆ—Ø‚è‘Ö‚¦
		g_gm_eve_mgr_work->sts_proc = gmEveMgrStateFuncIndependentScr;
	}

	// Ÿƒ{ƒXƒXƒe[ƒW‚Ü‚Ü‚½•Ê‚©


#if 0

	if (GmMainIsBossStage()) {
		// ƒ{ƒXƒXƒe[ƒW‚È‚çAƒ}ƒbƒv‘S‘Ì‚ÌƒCƒxƒ“ƒg‚ğ¶¬
		gmEveMgrSearchAllEvent(work);
		work->proc  = gmEveMgrProc0300;

	} else {
		// ’ÊíƒXƒe[ƒWFã‰ºLCD‚Ì‘SƒCƒxƒ“ƒg¶¬
		if (g_gm_map.flag & GMD_MAP_FLAG_CAM_DEPEND) {
			// ‚Q‰æ–Ê˜A“®
			GmEveMgrSearchEventLcdDS(work, MTE_GE2_A, GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE);
			work->proc  = gmEveMgrProc0100;
			
		} else {
			// ‚Q‰æ–Ê“Æ—§
			gmEveMgrSearchLcdEvent(work, MTE_GE2_A, GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE);
			gmEveMgrSearchLcdEvent(work, MTE_GE2_B, GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE);
			work->proc  = gmEveMgrProc0200;
		}
	}
#endif

}
#endif

#if _DS
// ==========================================================================
// gmEveMgrStateFuncDependScr
/*!
 *	ƒCƒxƒ“ƒgŠÇ— ó‘Ô–ˆˆ— 2‰æ–Ê˜A“®ˆ—
 */
// ==========================================================================
void gmEveMgrStateFuncDependScr(void)
{
	fx32	cam_up_pos_x, cam_up_pos_y, cam_down_pos_x, cam_down_pos_y;

//	// ƒJƒƒ‰‚ÌˆÊ’u‚ª•Ï‚í‚Á‚½‚ç“]‘—
	GmCameraGetLCDCameraPos(MTE_LCD_UP, &cam_up_pos_x, &cam_up_pos_y);
	GmCameraGetLCDCameraPos(MTE_LCD_DOWN, &cam_down_pos_x, &cam_down_pos_y);

	if (cam_down_pos_x != g_gm_eve_mgr_work->prev_pos[MTE_LCD_DOWN][MTD_X] ||
			cam_down_pos_y != g_gm_eve_mgr_work->prev_pos[MTE_LCD_DOWN][MTD_Y] ||
			cam_up_pos_x != g_gm_eve_mgr_work->prev_pos[MTE_LCD_UP][MTD_X] ||
			cam_up_pos_y != g_gm_eve_mgr_work->prev_pos[MTE_LCD_UP][MTD_Y]) {

		GmEveMgrSearchEventLcdDS(GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE | GMD_EVE_SEARCH_PROC_FLAG_RECT_LCD_OFF);
	}
}

// ==========================================================================
// gmEveMgrStateFuncIndependentScr
/*!
 *	ƒCƒxƒ“ƒgŠÇ— ó‘Ô–ˆˆ— 2‰æ–Ê“Æ—§ˆ—
 */
// ==========================================================================
void gmEveMgrStateFuncIndependentScr(void)
{
//	// ƒJƒƒ‰‚ÌˆÊ’u‚ª•Ï‚í‚Á‚½‚ç“]‘—

	fx32	cam_up_pos_x, cam_up_pos_y, cam_down_pos_x, cam_down_pos_y;

//	// ƒJƒƒ‰‚ÌˆÊ’u‚ª•Ï‚í‚Á‚½‚ç“]‘—
	GmCameraGetLCDCameraPos(MTE_LCD_UP, &cam_up_pos_x, &cam_up_pos_y);
	GmCameraGetLCDCameraPos(MTE_LCD_DOWN, &cam_down_pos_x, &cam_down_pos_y);

	if (cam_up_pos_x != g_gm_eve_mgr_work->prev_pos[MTE_LCD_UP][MTD_X] ||
			cam_up_pos_y != g_gm_eve_mgr_work->prev_pos[MTE_LCD_UP][MTD_Y]) {

		GmEveMgrCreateEventLcd(MTE_LCD_UP, GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE | GMD_EVE_SEARCH_PROC_FLAG_RECT_LCD_OFF);
	}

	if (cam_down_pos_x != g_gm_eve_mgr_work->prev_pos[MTE_LCD_DOWN][MTD_X] ||
			cam_down_pos_y != g_gm_eve_mgr_work->prev_pos[MTE_LCD_DOWN][MTD_Y]) {

		GmEveMgrCreateEventLcd(MTE_LCD_DOWN, GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE | GMD_EVE_SEARCH_PROC_FLAG_RECT_LCD_OFF);
	}
}
#else

// ==========================================================================
// gmEveMgrStateFuncSingleScr
/*!
 *	ƒCƒxƒ“ƒgŠÇ— ó‘Ô–ˆˆ— 1‰æ–Êˆ—
 */
// ==========================================================================
void gmEveMgrStateFuncSingleScr(void)
{
//	// ƒJƒƒ‰‚ÌˆÊ’u‚ª•Ï‚í‚Á‚½‚ç“]‘—
	OBS_CAMERA	*camera;

	MTM_ASSERT(g_gm_eve_mgr_work);

	camera = ObjCameraGet(g_obj.glb_camera_id);

	if (camera) {
		if (camera->disp_pos.x != g_gm_eve_mgr_work->prev_pos[MTD_X] ||
				camera->disp_pos.y != g_gm_eve_mgr_work->prev_pos[MTD_Y]) {

			GmEveMgrCreateEventLcd(GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE | GMD_EVE_SEARCH_PROC_FLAG_RECT_LCD_OFF);
		}
	}
}
#endif


// ==========================================================================
// ƒCƒxƒ“ƒg¶¬
// ==========================================================================
// ==========================================================================
// gmEveMgrCreateEventBlkEvent
/*!
 *	ƒCƒxƒ“ƒg¶¬ ƒuƒƒbƒNw’è
 *
 *	@param	flag	[in]	“®ìw’èƒtƒ‰ƒO
 *	@param	bx		[in]	ƒCƒxƒ“ƒg‚ğ¶¬‚·‚éƒuƒƒbƒNÀ•WX (ƒuƒƒbƒN’PˆÊ)
 *	@param	by		[in]	ƒCƒxƒ“ƒg‚ğ¶¬‚·‚éƒuƒƒbƒNÀ•WY (ƒuƒƒbƒN’PˆÊ)
 *	@param	r_on	[in]	ƒCƒxƒ“ƒg¶¬‹éŒ`
 *	@param	r_off	[in]	ƒCƒxƒ“ƒg‚ğ¶¬‚µ‚È‚¢‹éŒ`
 *
 *	@note
 *		’ÊíƒCƒxƒ“ƒg‚Ì¶¬
 */
// ==========================================================================
void gmEveMgrCreateEventBlkEvent(u32 flag, u16 bx, u16 by, s32 *r_on, s32 *r_off)
{
	GMS_EVE_RECORD_EVENT	*eve_rec;	// ƒCƒxƒ“ƒgƒŒƒR[ƒh
//	const GMS_EVE_DATA_EV_HEADER *data_eve = (const GMS_EVE_DATA_EV_HEADER*)gm_eve_data_dest.data_eve;   // ƒCƒxƒ“ƒgƒf[ƒ^
//	u32						list_ofst;	// ƒCƒxƒ“ƒgƒŠƒXƒgƒf[ƒ^ˆÊ’uƒIƒtƒZƒbƒg
	u32						block_no;	// ƒuƒƒbƒNNO
	GMS_EVE_DATA_EV_LIST	*eve_list;	// ƒCƒxƒ“ƒgî•ñ ƒŠƒXƒg
	u16						eve_num;	// ƒCƒxƒ“ƒgƒŠƒXƒg“àƒCƒxƒ“ƒg”
	
	static int test_print = 0;//qqq
	if(0==test_print++) 
	{
		mppEM_dbgPrintEventMap(true);		
	}

	block_no	= (u32)(bx + gm_eve_data->width * by);	// ¶¬ƒ`ƒFƒbƒNƒuƒƒbƒNNO
//	list_ofst	= gm_eve_data->ofst[block_no];
	eve_list	= (GMS_EVE_DATA_EV_LIST*)((u32)gm_eve_data + gm_eve_data->ofst[block_no]);	// ƒCƒxƒ“ƒgî•ñƒŠƒXƒg
	eve_num		= eve_list->eve_num;			// ƒuƒƒbƒN“àƒCƒxƒ“ƒg”
	eve_rec		= &eve_list->eve_rec[0];		// ƒuƒƒbƒN“àƒCƒxƒ“ƒgƒŒƒR[ƒhæ“ª

	{
		s32		i;
		s32		ofst_x = bx << GMD_EVE_BLOCK_SIZE_SHIFT;	// ƒuƒƒbƒN¶ãÀ•W
		s32		ofst_y = by << GMD_EVE_BLOCK_SIZE_SHIFT;
		s32		pos_x, pos_y;									// ƒOƒ[ƒoƒ‹À•W
		s32		eve_size;										// ƒCƒxƒ“ƒgƒTƒCƒY
		s32		r_on_revise, r_off_revise;						// ƒCƒxƒ“ƒg¶¬”ÍˆÍƒ`ƒFƒbƒN’l

		// ƒCƒxƒ“ƒg¶¬ƒ‹[ƒv
		for (i = 0; i < eve_num; i++, eve_rec++) {
			// ‹­§¶¬ƒ`ƒFƒbƒN
			if ((!(flag & GMD_EVE_SEARCH_PROC_FLAG_ONLY_ENFORCE)) ||				// ‹­§¶¬w’è‚Å‚È‚¢
					(eve_rec->flag & GMD_EVE_RECORD_EVENT_FLAG_CREATE_ENFORCE)) {	// ‹­§¶¬w’è‚Ì‚ÍƒCƒxƒ“ƒg‚É‹­§¶¬ƒtƒ‰ƒO‚ª‚ ‚é

				if (eve_rec->pos_x == GMD_EVE_RECORD_CMD_SKIP) {
					// ¶¬Ï‚İ or ‚à‚¤¶¬‚µ‚È‚¢
					continue;
				}

				// –¢¶¬

				// ƒOƒ[ƒoƒ‹À•Wæ“¾
				pos_x = eve_rec->pos_x + ofst_x;
				pos_y = eve_rec->pos_y + ofst_y;

				// ƒCƒxƒ“ƒgƒTƒCƒYæ“¾
				eve_size = g_gm_event_size_tbl[eve_rec->id];

				// ¶¬‹éŒ`“àƒ`ƒFƒbƒN
			//	r_on_revise = /* + scale‘Î‰’lŸ + */eve_size + GMD_MAIN_SCR_SPD_MAX;
				r_on_revise = /* + scale‘Î‰’lŸ + */eve_size + GMD_MAIN_SCR_SPD_MAX +
									 + GMD_EVE_BIRTH_WIDTH;
				if (!(flag & GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE) ||		// ¶¬”ÍˆÍ‹éŒ`ƒ`ƒFƒbƒN‚ğs‚í‚È‚¢ ‚©
						((pos_x >= r_on[MTD_LEFT]	- r_on_revise) &&		// ¶¬”ÍˆÍ‹éŒ`“à
						 (pos_x <= r_on[MTD_RIGHT]	+ r_on_revise) &&
						 (pos_y >= r_on[MTD_TOP]	- r_on_revise) &&
						 (pos_y <= r_on[MTD_BOTTOM]	+ r_on_revise)) ) {

					// LCD•\¦—Ìˆæ¶¬‹Ö~ƒ`ƒFƒbƒN
					r_off_revise = /* + scale‘Î‰’lŸ + */eve_size + GMD_MAIN_SCR_SPD_MAX;
					if (!(flag & GMD_EVE_SEARCH_PROC_FLAG_RECT_LCD_OFF) ||			// LCD•\¦—Ìˆæ¶¬‹Ö~ƒ`ƒFƒbƒN‚ğs‚í‚È‚¢ ‚©
							((pos_x <= r_off[MTD_LEFT]		- r_off_revise) ||		// LCD•\¦—Ìˆæ¶¬‹Ö~‹éŒ`‚ÉŠÜ‚Ü‚ê‚È‚¢
							 (pos_x >= r_off[MTD_RIGHT]		+ r_off_revise) ||
							 (pos_y <= r_off[MTD_TOP]		- r_off_revise) ||
							 (pos_y >= r_off[MTD_BOTTOM]	+ r_off_revise)) ) {
						// ƒCƒxƒ“ƒg¶¬
						if (eve_rec->id < GMD_EVENT_ID_MAX &&
								g_gm_event_tbl[eve_rec->id] != NULL) {
							/* “ïˆÕ“x•Ê¶¬ƒ`ƒFƒbƒN */
							if (eve_rec->flag & (GMD_EVE_RECORD_EVENT_FLAG_NO_CREATE_EASE << GsGetGameLevel())) {
								// “ïˆÕ“x‚É‚æ‚é¶¬–³‚µƒCƒxƒ“ƒg
								eve_rec->pos_x = GMD_EVE_RECORD_CMD_SKIP;
								continue;
							}

							// ŸŠÂ‹«®”õ‚Ü‚Å‚Ìb’èƒJƒbƒg
#if defined GMD_DEBUG_NO_CREATE_ENEMY
#if defined GMD_DEBUG_NO_CREATE_GIMMICK
							if (!((GMD_EVENT_ID_GMK_TOUCH_EARTH <= eve_rec->id &&
										eve_rec->id <= GMD_EVENT_ID_GMK_B) ||
									eve_rec->id == GMD_EVENT_ID_GMK_START)) {
								// ’n–ÊÚ’n
								// A–ÊØ‚è‘Ö‚¦
								// B–ÊØ‚è‘Ö‚¦
								// ƒXƒ^[ƒgˆÊ’uİ’è ˆÈŠO‚Í¶¬‚µ‚È‚¢
								eve_rec->pos_x = GMD_EVE_RECORD_CMD_SKIP;
								continue;
							}
#else
							if (eve_rec->id < GMD_EVENT_ID_GIMMICK_START) {
								// “G‚Í¶¬‚µ‚È‚¢
								eve_rec->pos_x = GMD_EVE_RECORD_CMD_SKIP;
								continue;
							}
#endif
#else
#if defined GMD_DEBUG_NO_CREATE_GIMMICK
							if (!((GMD_EVENT_ID_GMK_TOUCH_EARTH <= eve_rec->id &&
								   eve_rec->id <= GMD_EVENT_ID_GMK_B) ||
								  eve_rec->id == GMD_EVENT_ID_SCR_LIMIT_SET || 
								  eve_rec->id == GMD_EVENT_ID_GMK_START)) {
								// ’n–ÊÚ’n
								// A–ÊØ‚è‘Ö‚¦
								// B–ÊØ‚è‘Ö‚¦
								// ƒXƒ^[ƒgˆÊ’uİ’è ˆÈŠO‚Í¶¬‚µ‚È‚¢
								// ƒMƒ~ƒbƒN‚Í¶¬‚µ‚È‚¢
								eve_rec->pos_x = GMD_EVE_RECORD_CMD_SKIP;
								continue;
							}
#endif
#endif


							// ƒCƒxƒ“ƒg¶¬
#if GMD_EVE_DEBUG_PRINTMSG
							OS_TPrintf("¡ƒCƒxƒ“ƒg¶¬ %d\n", eve_rec->id);
#endif	// #if GMD_EVE_DEBUG_PRINTMSG
							OS_TPrintf("CREATE EVENT: ID=%d [%d,%d]\n", eve_rec->id, bx, by);//qqq
							g_gm_event_tbl[eve_rec->id](eve_rec,
										pos_x << FX32_SHIFT,
										pos_y << FX32_SHIFT,
										0); /*type*/
							mppEM_dbgPrintEventMap(true);	
						}
#if defined (MTD_DEBUG)
						else {
							OS_Printf("gmEventMgr.c::Invalid event ID: %d\n", eve_rec->id);
						}
#endif	// #if defined (MTD_DEBUG)
					}	// LCD•\¦—Ìˆæ¶¬‹Ö~ƒ`ƒFƒbƒN
				}	// ¶¬‹éŒ`“àƒ`ƒFƒbƒN
			}	// ‹­§¶¬ƒ`ƒFƒbƒN
		}	// ƒCƒxƒ“ƒg¶¬ƒ‹[ƒv
	}
}

// ==========================================================================
// gmEveMgrCreateEventBlkRing
/*!
 *	ƒCƒxƒ“ƒg¶¬ ƒuƒƒbƒNw’è
 *
 *	@param	flag	[in]	“®ìw’èƒtƒ‰ƒO
 *	@param	bx		[in]	ƒCƒxƒ“ƒg‚ğ¶¬‚·‚éƒuƒƒbƒNÀ•WX (ƒuƒƒbƒN’PˆÊ)
 *	@param	by		[in]	ƒCƒxƒ“ƒg‚ğ¶¬‚·‚éƒuƒƒbƒNÀ•WY (ƒuƒƒbƒN’PˆÊ)
 *	@param	r_on	[in]	ƒCƒxƒ“ƒg¶¬‹éŒ`
 *	@param	r_off	[in]	ƒCƒxƒ“ƒg‚ğ¶¬‚µ‚È‚¢‹éŒ`
 *
 *	@note
 *		‘•üƒCƒxƒ“ƒg‚Ì¶¬
 */
// ==========================================================================
void gmEveMgrCreateEventBlkRing(u32 flag, u16 bx, u16 by, s32 *r_on, s32 *r_off)
{
	GMS_EVE_RECORD_RING		*ring_data;	// ƒCƒxƒ“ƒgƒŒƒR[ƒh
	u32						block_no;	// ƒuƒƒbƒNNO
	GMS_EVE_DATA_RG_LIST	*ring_list;	// ‘•üî•ñ ƒŠƒXƒg
	u16						ring_num;	// ƒCƒxƒ“ƒgƒŠƒXƒg“àƒCƒxƒ“ƒg”

	UNREFERENCED_PARAMETER(flag);
	UNREFERENCED_PARAMETER(r_off);

#if defined GMD_DEBUG_NO_CREATE_RING
	// ŸŠÂ‹«®”õ‚Ü‚Å‚Ìb’èƒJƒbƒg
	return;
#endif

	block_no	= (u32)(bx + gm_ring_data->width * by);	// ¶¬ƒ`ƒFƒbƒNƒuƒƒbƒNNO
//	list_ofst	= gm_ring_data->ofst[block_no];
	ring_list	= (GMS_EVE_DATA_RG_LIST*)((u32)gm_ring_data + gm_ring_data->ofst[block_no]);	// ƒCƒxƒ“ƒgî•ñƒŠƒXƒg
	ring_num	= ring_list->ring_num;			// ƒuƒƒbƒN“àƒCƒxƒ“ƒg”
	ring_data	= &ring_list->ring_data[0];		// ƒuƒƒbƒN“àƒCƒxƒ“ƒgƒŒƒR[ƒhæ“ª

	{
		u16	 i;			  // ƒ‹[ƒvƒJƒEƒ“ƒ^A¶¬ƒŠƒ“ƒO‚ÌID‚àŒ“‚Ë‚é
		s32	 ofs_x   = (bx << GMD_EVE_BLOCK_SIZE_SHIFT);	// ƒuƒƒbƒN¶ã‚ÌÀ•W
		s32	 ofs_y   = (by << GMD_EVE_BLOCK_SIZE_SHIFT);	// 
		s32	 pos_x, pos_y;
		s32	r_on_revise;									// ƒCƒxƒ“ƒg¶¬”ÍˆÍƒ`ƒFƒbƒN’l
		
		// ¶¬‹éŒ`“àƒ`ƒFƒbƒN
		//	r_on_revise = /* + scale‘Î‰’lŸ + */eve_size + GMD_MAIN_SCR_SPD_MAX;
		r_on_revise = /* + scale‘Î‰’lŸ + */GMD_RING_SIZE + GMD_MAIN_SCR_SPD_MAX +
									 + GMD_EVE_BIRTH_WIDTH;
		for (i = 0; i < ring_num; ++i) {
			
			// –¢¶¬‚©ƒ`ƒFƒbƒN
			if (ring_data->pos_x != (u8)GMD_EVE_RECORD_CMD_SKIP) {
				
				pos_x   = ring_data->pos_x + ofs_x;
				pos_y   = ring_data->pos_y + ofs_y;
				
				// ¶¬‹éŒ`“à‚©ƒ`ƒFƒbƒN
				if ((pos_x >= r_on[MTD_LEFT]	- r_on_revise) &&
						 (pos_x <= r_on[MTD_RIGHT]	+ r_on_revise) &&
						 (pos_y >= r_on[MTD_TOP]	- r_on_revise) &&
						 (pos_y <= r_on[MTD_BOTTOM]	+ r_on_revise)) {
				//if (pos_x >= r_on[MTD_LEFT]
				 //&& pos_x <= r_on[MTD_RIGHT]
				 //&& pos_y >= r_on[MTD_TOP]
				 //&& pos_y <= r_on[MTD_BOTTOM]) {

					// ƒŠƒ“ƒO¶¬
					// LCD•\¦—Ìˆæƒ`ƒFƒbƒN‚Ís‚í‚È‚¢
					//OS_TPrintf("CREATE RING: BX=%d BY=%d\n", (bx), by);//qqq
					GmRingCreate( ring_data,
							((bx << GMD_EVE_BLOCK_SIZE_SHIFT) + ring_data->pos_x) << FX32_SHIFT,
							((by << GMD_EVE_BLOCK_SIZE_SHIFT) + ring_data->pos_y) << FX32_SHIFT,
							0);
				}
			}
			++ring_data;
		}
	}
}

// ==========================================================================
// gmEveMgrCreateEventBlkDecorate
/*!
 *	ƒCƒxƒ“ƒg¶¬ ƒuƒƒbƒNw’è
 *
 *	@param	flag	[in]	“®ìw’èƒtƒ‰ƒO
 *	@param	bx		[in]	ƒCƒxƒ“ƒg‚ğ¶¬‚·‚éƒuƒƒbƒNÀ•WX (ƒuƒƒbƒN’PˆÊ)
 *	@param	by		[in]	ƒCƒxƒ“ƒg‚ğ¶¬‚·‚éƒuƒƒbƒNÀ•WY (ƒuƒƒbƒN’PˆÊ)
 *	@param	r_on	[in]	ƒCƒxƒ“ƒg¶¬‹éŒ`
 *	@param	r_off	[in]	ƒCƒxƒ“ƒg‚ğ¶¬‚µ‚È‚¢‹éŒ`
 *
 *	@note
 *		‘•üƒCƒxƒ“ƒg‚Ì¶¬
 */
// ==========================================================================
void gmEveMgrCreateEventBlkDecorate(u32 flag, u16 bx, u16 by, s32 *r_on, s32 *r_off)
{
	GMS_EVE_RECORD_DECORATE	*dec_data;	// ƒCƒxƒ“ƒgƒŒƒR[ƒh
	u32						block_no;	// ƒuƒƒbƒNNO
	GMS_EVE_DATA_DC_LIST	*dec_list;	// ‘•üî•ñ ƒŠƒXƒg
	u16						dec_num;	// ƒCƒxƒ“ƒgƒŠƒXƒg“àƒCƒxƒ“ƒg”

	block_no	= (u32)(bx + gm_deco_data->width * by);	// ¶¬ƒ`ƒFƒbƒNƒuƒƒbƒNNO
//	list_ofst	= gm_deco_data->ofst[block_no];
	dec_list	= (GMS_EVE_DATA_DC_LIST*)((u32)gm_deco_data + gm_deco_data->ofst[block_no]);	// ƒCƒxƒ“ƒgî•ñƒŠƒXƒg
	dec_num		= dec_list->dec_num;			// ƒuƒƒbƒN“àƒCƒxƒ“ƒg”
	dec_data	= &dec_list->dec_data[0];		// ƒuƒƒbƒN“àƒCƒxƒ“ƒgƒŒƒR[ƒhæ“ª

	{
		s32		i;
		s32		ofst_x = bx << GMD_EVE_BLOCK_SIZE_SHIFT;	// ƒuƒƒbƒN¶ãÀ•W
		s32		ofst_y = by << GMD_EVE_BLOCK_SIZE_SHIFT;
		s32		pos_x, pos_y;									// ƒOƒ[ƒoƒ‹À•W
		s32		eve_size;										// ƒCƒxƒ“ƒgƒTƒCƒY
		s32		r_on_revise, r_off_revise;						// ƒCƒxƒ“ƒg¶¬”ÍˆÍƒ`ƒFƒbƒN’l

		// ƒCƒxƒ“ƒg¶¬ƒ‹[ƒv
		for (i = 0; i < dec_num; i++, dec_data++) {
				if (dec_data->pos_x == GMD_EVE_RECORD_CMD_SKIP) {
					// ¶¬Ï‚İ or ‚à‚¤¶¬‚µ‚È‚¢
					continue;
				}

				// –¢¶¬

				// ƒOƒ[ƒoƒ‹À•Wæ“¾
				pos_x = dec_data->pos_x + ofst_x;
				pos_y = dec_data->pos_y + ofst_y;

				// ƒCƒxƒ“ƒgƒTƒCƒYæ“¾
				eve_size = g_gm_decorate_size_tbl[dec_data->id];

				// ¶¬‹éŒ`“àƒ`ƒFƒbƒN
			//	r_on_revise = /* + scale‘Î‰’lŸ + */eve_size + GMD_MAIN_SCR_SPD_MAX;
				r_on_revise = /* + scale‘Î‰’lŸ + */eve_size + GMD_MAIN_SCR_SPD_MAX +
									 + GMD_EVE_BIRTH_WIDTH;
				if (!(flag & GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE) ||		// ¶¬”ÍˆÍ‹éŒ`ƒ`ƒFƒbƒN‚ğs‚í‚È‚¢ ‚©
						((pos_x >= r_on[MTD_LEFT]	- r_on_revise) &&		// ¶¬”ÍˆÍ‹éŒ`“à
						 (pos_x <= r_on[MTD_RIGHT]	+ r_on_revise) &&
						 (pos_y >= r_on[MTD_TOP]	- r_on_revise) &&
						 (pos_y <= r_on[MTD_BOTTOM]	+ r_on_revise)) ) {

					// LCD•\¦—Ìˆæ¶¬‹Ö~ƒ`ƒFƒbƒN
					r_off_revise = /* + scale‘Î‰’lŸ + */eve_size + GMD_MAIN_SCR_SPD_MAX;
					if (!(flag & GMD_EVE_SEARCH_PROC_FLAG_RECT_LCD_OFF) ||			// LCD•\¦—Ìˆæ¶¬‹Ö~ƒ`ƒFƒbƒN‚ğs‚í‚È‚¢ ‚©
							((pos_x <= r_off[MTD_LEFT]		- r_off_revise) ||		// LCD•\¦—Ìˆæ¶¬‹Ö~‹éŒ`‚ÉŠÜ‚Ü‚ê‚È‚¢
							 (pos_x >= r_off[MTD_RIGHT]		+ r_off_revise) ||
							 (pos_y <= r_off[MTD_TOP]		- r_off_revise) ||
							 (pos_y >= r_off[MTD_BOTTOM]	+ r_off_revise)) ) {
						// ƒCƒxƒ“ƒg¶¬
						if (dec_data->id < GMD_DECORATE_ID_MAX &&
								g_gm_decorate_tbl[dec_data->id] != NULL) {
								// ƒCƒxƒ“ƒg¶¬
#if GMD_EVE_DEBUG_PRINTMSG
							OS_TPrintf("¡ƒCƒxƒ“ƒg¶¬ %d\n", dec_data->id);
#endif	// #if GMD_EVE_DEBUG_PRINTMSG
							//OS_TPrintf("CREATE DECOR: posX=%d posY=%d\n", (pos_x), (pos_y));//qqq
							g_gm_decorate_tbl[dec_data->id](dec_data,
										pos_x << FX32_SHIFT,
										pos_y << FX32_SHIFT,
										0); /*type*/
						}
#if defined (MTD_DEBUG)
						else {
							OS_Printf("gmEventMgr.c::Invalid deco ID: %d\n", dec_data->id);
						}
#endif	// #if defined (MTD_DEBUG)
					}	// LCD•\¦—Ìˆæ¶¬‹Ö~ƒ`ƒFƒbƒN
				}	// ¶¬‹éŒ`“àƒ`ƒFƒbƒN
		}	// ƒCƒxƒ“ƒg¶¬ƒ‹[ƒv
	}

}


// ==========================================================================
// ƒfƒoƒbƒN
// ==========================================================================
// ==========================================================================
// gmEventMgrDebugEventCheck
/*!
 *	ƒfƒoƒbƒN—pƒCƒxƒ“ƒgƒ`ƒFƒbƒN
 */
// ==========================================================================
#if GMD_EVE_DEBUG_EVENT_CHECK
void gmEventMgrDebugEventCheck(void)
{
	GMS_EVE_SEARCH_WORK		eve_search_work;
	GMS_EVE_RECORD_EVENT	*eve_rec;

	BOOL					kuumon_mgr[16] = {FALSE};
	BOOL					eneroom_mgr[16] = {FALSE};
	s32						mgr_id;

	GmEventMgrSearchEventWorkInit(&eve_search_work);

	eve_rec = GmEventMgrSearchEvent(&eve_search_work);
	while (eve_rec) {

		if (eve_rec->id == GMD_EVENT_ID_DUMMY) {
			if () {
				MTM_ASSERT(!"gmEventMgr:gmEventMgrDebugEventCheck() \n");
			}
			else {
				kuumon_mgr[mgr_id] = TRUE;
			}
		}

		eve_rec = GmEventMgrSearchEvent(&eve_search_work);
	}

}
#endif // #if defined (MTD_DEBUG)

// ==========================================================================
// GmEventMgrStaticVarInit
/*!
 *	static•Ï”‚Ì‰Šú‰»
 */
// ==========================================================================
void GmEventMgrStaticVarInit(void)
{
	g_gm_eve_mgr_work = NULL;	//!< ƒCƒxƒ“ƒgƒ}ƒl[ƒWƒƒ[ƒ[ƒN
	
	gm_eve_mgr_tcb = NULL;		//!< ƒCƒxƒ“ƒgƒ}ƒl[ƒWƒƒ[TCB
	
	/* ƒCƒxƒ“ƒgŠÖ˜Aƒf[ƒ^ */
	gm_eve_data = NULL;			//!< ƒCƒxƒ“ƒgƒf[ƒ^
	gm_ring_data = NULL;		//!< ƒŠƒ“ƒOƒf[ƒ^
	gm_deco_data = NULL;		//!< ‘•üƒf[ƒ^
	
	gm_eve_data_size = -1;//qqq
	gm_ring_data_size = -1;//qqq
	//gm_deco_data_size = -1;
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================

// ----------------------------- mpp area -----------------------------------------------------
// ----------------------------- added by mpp developer ---------------------------------------

#include "mppUtil.h"


static GMS_EVE_DATA_EV_HEADER *mpp_eve_data = NULL;				//!< ƒCƒxƒ“ƒgƒf[ƒ^
static GMS_EVE_DATA_RG_HEADER *mpp_ring_data = NULL;				//!< ƒŠƒ“ƒOƒf[ƒ^
//static GMS_EVE_DATA_DC_HEADER *mpp_deco_data = NULL;				//!< ‘•üƒf[ƒ^
static GMS_EVE_DATA_EV_HEADER *orig_eve_data = NULL;				//!< ƒCƒxƒ“ƒgƒf[ƒ^


void mppEM_StoreOriginMapDataToMemory()
{
	MTM_ASSERT(gm_eve_data_size>0);
	if(orig_eve_data!=NULL) {
		amMemFree(orig_eve_data);
		orig_eve_data = NULL;
	}
	orig_eve_data = (GMS_EVE_DATA_EV_HEADER*)amMemAlloc(gm_eve_data_size);	
	memcpy(orig_eve_data, gm_eve_data, gm_eve_data_size);	
}

void mppEM_ReserveMapData()
{
	mppEM_DeleteMapData();
	//
	MTM_ASSERT(gm_eve_data_size>0);
	MTM_ASSERT(gm_ring_data_size>0);
	//MTM_ASSERT(gm_deco_data_size>0);	
	mpp_eve_data = (GMS_EVE_DATA_EV_HEADER*)amMemAlloc(gm_eve_data_size);
	mpp_ring_data = (GMS_EVE_DATA_RG_HEADER*)amMemAlloc(gm_ring_data_size);
	//mpp_deco_data = (GMS_EVE_DATA_DC_HEADER*)amMemAlloc(gm_deco_data_size);
}


void mppEM_ReturnSomeRingsBackToMap()
{
	extern GMS_RING_SYS_WORK			*gm_ring_sys_work;
	//
	for(GMS_RING_WORK	*ring_work = gm_ring_sys_work->ring_list_start; ring_work;) {
		GMS_EVE_RECORD_RING		*eve_rec = ring_work->eve_rec;
		if(eve_rec!=NULL) {
			size_t delta = ((char*)eve_rec - (char*)gm_ring_data);
			GMS_EVE_RECORD_RING		*mpp_eve_rec = (GMS_EVE_RECORD_RING*)((char*)mpp_ring_data + delta);
			MTM_ASSERT(eve_rec->pos_y == mpp_eve_rec->pos_y);
			mpp_eve_rec->pos_x = 	((ring_work->pos.x)>>FX32_SHIFT) % (1<< GMD_EVE_BLOCK_SIZE_SHIFT);
			OS_TPrintf("force restore ring before saving [%i,%i](from ring list)\n", mpp_eve_rec->pos_x, mpp_eve_rec->pos_y);
		}
		//
		ring_work = ring_work->post_ring;
	}
}


#define MPP_FAST_SAVE_METHOD

void mppEM_ReturnSomeEventsBackToMap()
{
	mppEM_ReturnSomeRingsBackToMap();
	
	MTM_ASSERT(mpp_eve_data!=NULL);
	MTM_ASSERT(orig_eve_data!=NULL);
	
#ifdef 	MPP_FAST_SAVE_METHOD
	{
		for(int jj=0; jj<mppEnemyList_getCount(); jj++) {
			GMS_EVE_RECORD_EVENT	*eve_rec = mppEnemyList_getRec(jj);	
			size_t delta = ((char*)eve_rec-(char*)gm_eve_data);
			GMS_EVE_RECORD_EVENT	*mpp_eve_rec = (GMS_EVE_RECORD_EVENT*)((char*)mpp_eve_data + delta);
			GMS_EVE_RECORD_EVENT	*orig_eve_rec = (GMS_EVE_RECORD_EVENT*)((char*)orig_eve_data + delta);
			
			MTM_ASSERT(eve_rec->id == mpp_eve_rec->id);
			MTM_ASSERT(eve_rec->id == orig_eve_rec->id);
			
			mpp_eve_rec->pos_x = orig_eve_rec->pos_x;
			OS_TPrintf("force restore eve: id=%i before saving [%i,%i](from task list)\n", mpp_eve_rec->id, mpp_eve_rec->pos_x, mpp_eve_rec->pos_y);
		}
	}
#else
/*old, unused	
	GMS_EVE_RECORD_EVENT	*eve_rec, *orig_eve_rec;	// ƒCƒxƒ“ƒgƒŒƒR[ƒh
	//	const GMS_EVE_DATA_EV_HEADER *data_eve = (const GMS_EVE_DATA_EV_HEADER*)gm_eve_data_dest.data_eve;   // ƒCƒxƒ“ƒgƒf[ƒ^
	//	u32						list_ofst;	// ƒCƒxƒ“ƒgƒŠƒXƒgƒf[ƒ^ˆÊ’uƒIƒtƒZƒbƒg
	u32						block_no;	// ƒuƒƒbƒNNO
	GMS_EVE_DATA_EV_LIST	*eve_list, *orig_eve_list;	// ƒCƒxƒ“ƒgî•ñ ƒŠƒXƒg
	u16						eve_num;	// ƒCƒxƒ“ƒgƒŠƒXƒg“àƒCƒxƒ“ƒg”
	{
		int iy, ix, ir;
		for(iy=0; iy<mpp_eve_data->height; iy++)
		{
			block_no	= (u32)(mpp_eve_data->width * iy);	// ¶¬ƒ`ƒFƒbƒNƒuƒƒbƒNNO
			for(ix=0; ix<mpp_eve_data->width; ix++)
			{
				
				eve_list	= (GMS_EVE_DATA_EV_LIST*)((u32)mpp_eve_data + mpp_eve_data->ofst[block_no]);	// ƒCƒxƒ“ƒgî•ñƒŠƒXƒg
				eve_num		= eve_list->eve_num;			// ƒuƒƒbƒN“àƒCƒxƒ“ƒg”
				
				if(eve_num>0) {					
					eve_rec		= &eve_list->eve_rec[0];		// ƒuƒƒbƒN“àƒCƒxƒ“ƒgƒŒƒR[ƒhæ“ª
					orig_eve_list	= (GMS_EVE_DATA_EV_LIST*)((u32)orig_eve_data + orig_eve_data->ofst[block_no]);	// ƒCƒxƒ“ƒgî•ñƒŠƒXƒg
					orig_eve_rec	= &orig_eve_list->eve_rec[0];		// ƒuƒƒbƒN“àƒCƒxƒ“ƒgƒŒƒR[ƒhæ“ª
					
					MTM_ASSERT(eve_num == orig_eve_list->eve_num);
					
					//OS_TPrintf("[%2d,%2d] : num=%d : ", ix,iy,eve_num);
					for(ir=0; ir<eve_num; ir++) {
						bool isValid = (eve_rec->pos_x!=GMD_EVE_RECORD_CMD_SKIP);
						if(!isValid) {
							if(mppEM_ForceRestoreEvent(eve_rec->id)) {
								OS_TPrintf("force restore eve: id=%i [%2d,%2d] before saving\n", eve_rec->id, ix,iy);
								MTM_ASSERT(eve_rec->pos_y == orig_eve_rec->pos_y);
								eve_rec->pos_x = orig_eve_rec->pos_x;
							}							
						}
						eve_rec++;
						orig_eve_rec++;
						
					}
					
				}
				
				block_no++;
			}
			
		}
		OS_TPrintf("---------------------\n");
	}
 */
#endif
	
}

void mppEM_PushMapData()
{
	MTM_ASSERT(gm_eve_data_size>0);
	MTM_ASSERT(gm_ring_data_size>0);
	//MTM_ASSERT(gm_deco_data_size>0);
	MTM_ASSERT(mpp_eve_data!=NULL);
	MTM_ASSERT(mpp_ring_data!=NULL);
	//MTM_ASSERT(mpp_deco_data!=NULL);
	//
	memcpy(mpp_eve_data, gm_eve_data, gm_eve_data_size);
	memcpy(mpp_ring_data, gm_ring_data, gm_ring_data_size);
	//memcpy(mpp_deco_data, gm_deco_data, gm_deco_data_size);		
}


void mppEM_PopMapData() 
{
	MTM_ASSERT(gm_eve_data_size>0);
	MTM_ASSERT(gm_ring_data_size>0);
	//MTM_ASSERT(gm_deco_data_size>0);
	MTM_ASSERT(mpp_eve_data!=NULL);
	MTM_ASSERT(mpp_ring_data!=NULL);
	//MTM_ASSERT(mpp_deco_data!=NULL);
	//
	memcpy(gm_eve_data, mpp_eve_data, gm_eve_data_size);
	memcpy(gm_ring_data, mpp_ring_data, gm_ring_data_size);	
	//???memcpy(gm_deco_data, mpp_deco_data, gm_deco_data_size);			
}

void mppEM_DeleteMapData(void)
{
	if (mpp_eve_data) {
		amMemFree(mpp_eve_data);
		mpp_eve_data = NULL;
	}
	if (mpp_ring_data) {
		mtMemFreeMain(mpp_ring_data);
		mpp_ring_data = NULL;
	}
	/*if (mpp_deco_data) {
		mtMemFreeMain(mpp_deco_data);
		mpp_deco_data = NULL;
	}*/
}


void mppEM_SaveMapData(mppStorageWriter& sw)
{
	sw.write(gm_eve_data_size);
	sw.write(gm_ring_data_size);
	//sw.write(gm_deco_data_size);
	sw.writeRaw(mpp_eve_data, gm_eve_data_size);
	sw.writeRaw(mpp_ring_data, gm_ring_data_size);
	//sw.writeRaw(mpp_deco_data, gm_deco_data_size);
}

bool mppEM_LoadMapData(mppStorageReader& sr)
{

	s32 sz = -1;
	if(!sr.read(sz)) return false;
	if(sz!=gm_eve_data_size) return false;
	if(!sr.read(sz)) return false;
	if(sz!=gm_ring_data_size) return false;
	//if(!sr.read(sz)) return false;
	//if(sz!=gm_deco_data_size) return false;
	//
	if(!sr.readRaw(mpp_eve_data, gm_eve_data_size)) return false;
	if(!sr.readRaw(mpp_ring_data, gm_ring_data_size)) return false;
	//if(!sr.readRaw(mpp_deco_data, gm_deco_data_size)) return false;
	//
	return true;
}


void mppEM_dbgPrintEventMap(bool minimap)
{
	if(true) return;
	GMS_EVE_RECORD_EVENT	*eve_rec;	// ƒCƒxƒ“ƒgƒŒƒR[ƒh
	//	const GMS_EVE_DATA_EV_HEADER *data_eve = (const GMS_EVE_DATA_EV_HEADER*)gm_eve_data_dest.data_eve;   // ƒCƒxƒ“ƒgƒf[ƒ^
	//	u32						list_ofst;	// ƒCƒxƒ“ƒgƒŠƒXƒgƒf[ƒ^ˆÊ’uƒIƒtƒZƒbƒg
	u32						block_no;	// ƒuƒƒbƒNNO
	GMS_EVE_DATA_EV_LIST	*eve_list;	// ƒCƒxƒ“ƒgî•ñ ƒŠƒXƒg
	u16						eve_num;	// ƒCƒxƒ“ƒgƒŠƒXƒg“àƒCƒxƒ“ƒg”
	{
		int iy, ix, ir;
		if(minimap) {
			OS_TPrintf("---------------------\n");
			
			for(iy=0; iy<gm_eve_data->height; iy++)
			{
				for(ix=0; ix<gm_eve_data->width; ix++)
				{
					block_no	= (u32)(ix + gm_eve_data->width * iy);	// ¶¬ƒ`ƒFƒbƒNƒuƒƒbƒNNO
					
					eve_list	= (GMS_EVE_DATA_EV_LIST*)((u32)gm_eve_data + gm_eve_data->ofst[block_no]);	// ƒCƒxƒ“ƒgî•ñƒŠƒXƒg
					eve_num		= eve_list->eve_num;			// ƒuƒƒbƒN“àƒCƒxƒ“ƒg”
					
					OS_TPrintf("%2d", eve_num);				
				}
				OS_TPrintf("\n");
			}
		}
		OS_TPrintf("---------------------\n");
		for(iy=0; iy<gm_eve_data->height; iy++)
		{
			for(ix=0; ix<gm_eve_data->width; ix++)
			{
				block_no	= (u32)(ix + gm_eve_data->width * iy);	// ¶¬ƒ`ƒFƒbƒNƒuƒƒbƒNNO
				
				eve_list	= (GMS_EVE_DATA_EV_LIST*)((u32)gm_eve_data + gm_eve_data->ofst[block_no]);	// ƒCƒxƒ“ƒgî•ñƒŠƒXƒg
				eve_num		= eve_list->eve_num;			// ƒuƒƒbƒN“àƒCƒxƒ“ƒg”
				
				eve_rec		= &eve_list->eve_rec[0];		// ƒuƒƒbƒN“àƒCƒxƒ“ƒgƒŒƒR[ƒhæ“ª
				if(eve_num>0) {
					OS_TPrintf("[%2d,%2d] : num=%d : ", ix,iy,eve_num);
					for(ir=0; ir<eve_num; ir++) {
						bool isValid = (eve_rec->pos_x!=GMD_EVE_RECORD_CMD_SKIP);
						OS_TPrintf(" %d%s, ", eve_rec->id, isValid?"":"(-)");
						eve_rec++;
						
					}
					OS_TPrintf("\n");
				}
			}
			
		}
		OS_TPrintf("---------------------\n");
	}	
}


void mppEM_StaticVarInit(void)
{
	mpp_eve_data = NULL;				//!< ƒCƒxƒ“ƒgƒf[ƒ^
	mpp_ring_data = NULL;				//!< ƒŠƒ“ƒOƒf[ƒ^
	//mpp_deco_data = NULL;				//!< ‘•üƒf[ƒ^
	orig_eve_data = NULL;				//!< ƒCƒxƒ“ƒgƒf[ƒ^
}

