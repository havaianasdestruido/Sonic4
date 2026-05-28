// ==========================================================================
/*!
  @file gmDeco.cpp
  @brief 

  @author hanaoka
				Copyright(c) 2009 Dimpsd

  $Id: gmDeco.cpp 20 2011-04-22 12:46:46Z thamada $
  $Date: 2011-04-22 21:46:46 +0900 (é‡‘, 22 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"

#include "objObject.h"
#include "gsMainSys.h"
#include "gmMain.h"
#include "gmGameDat.h"
#include "gmEventTbl.h"
#include "gmGameDBuild.h"
#include "gmCamera.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectZone.h"
#include "gmPlayer.h"
#include "gmSound.h"

#include "gmGmkWaterSlider.h"	//ƒMƒ~ƒbƒN‚Ìƒ‚ƒfƒ‹‚È‚Ç‚ğg‚¢‚Ü‚í‚µ‚Ä‚¢‚é‚½‚ß

#include "gmMapFar.h"			//ƒŒƒ“ƒ_ƒ^[ƒQƒbƒg‚ğg‚¢‚Ü‚í‚µ‚Ä‚¢‚é‚½‚ß
#include "gmWaterSurface.h"		//ƒŒƒ“ƒ_ƒ^[ƒQƒbƒg‚ğg‚¢‚Ü‚í‚µ‚Ä‚¢‚é‚½‚ß

#include "gmDeco.h"
#include "gmDecoGlare.h"

// ƒf[ƒ^ƒwƒbƒ_
// Zone1
#include "common/model/deco_zone1_mdl.hmb"
#include "common/model/deco_zone1_mtn.hmb"
#include "common/model/deco_zone1_mat.hmb"
#include "common/model/deco_zone1_mdl_fall.hmb"

// Zone2
#include "common/model/deco_zone2_mdl.hmb"
#include "common/model/deco_zone2_mtn.hmb"
#include "common/model/deco_zone2_mat.hmb"

// Zone3
#include "common/model/deco_zone3_mdl.hmb"
#include "common/model/deco_zone3_mtn.hmb"
#include "common/model/deco_zone3_mat.hmb"
#include "common/model/deco_zone3_render_mdl.hmb"
#include "common/model/gmk_water_slider_mdl.hmb"
#include "common/model/gmk_water_slider_mat.hmb"
#include "common/model/gmk_water_slider_mtn.hmb"

// Zone4
#include "common/model/deco_zone4_mdl.hmb"

//Zonef
#include "common/model/deco_zonef_mdl.hmb"
#include "common/model/deco_zonef_mtn.hmb"
#include "common/model/deco_zonef_mat.hmb"


//----- Definitions ---------------------------------------------------------

#define GMD_DECO_USE_DRAW_SERVER (1 & (_IPHONE | _PC))
#define GMD_DECO_USE_DRAW_TVX    (1 & (_IPHONE))
#define GMD_DECO_USE_DRAW_TVX_NOMOTION (1 & (_IPHONE))

#define GMD_DECO_TEST_FINAL_SHUTTER	(1)


#define GMD_DECO_TEST_FALL	(1 & (_PC | _PS3 | _XBOX | _WII | _IPHONE) )


#define GMD_DECO_DATA_INDEX_INVALID		(-1)	//–³Œøƒf[ƒ^ƒCƒ“ƒfƒNƒX

#define GMD_DECO_FX32_BITSHIFT_BLOCK_SIZE	(6)		//ƒ}ƒbƒvƒuƒƒbƒNƒTƒCƒYƒrƒbƒgƒVƒtƒg


#define GMD_DECO_USER_FLAG_CHECK_CHAOS_EMERALD		(1 << 0)	//ƒJƒIƒXƒGƒƒ‰ƒ‹ƒh‚ª‚»‚ë‚Á‚Ä‚¢‚é‚©ƒ`ƒFƒbƒN‚·‚é
#define GMD_DECO_USER_FLAG_NO_MAIN_MODEL			(1 << 1)	//ƒƒCƒ“ƒ‚ƒfƒ‹‚ğ‚½‚È‚¢
#define GMD_DECO_USER_FLAG_SUB_MODEL				(1 << 2)	//ƒTƒuƒ‚ƒfƒ‹‚¿
#define GMD_DECO_USER_FLAG_SYNC_MATERIAL			(1 << 3)	//ƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ““¯ŠúiƒƒCƒ“ƒp[ƒc‚Ì‚İj
#define GMD_DECO_USER_FLAG_TOUCH_REVERSE_MOTION		(1 << 4)	//G‚ê‚½ƒvƒŒƒCƒ„‚ÌŒü‚«‚É‚æ‚Á‚Äƒ‚[ƒVƒ‡ƒ“‚ğ‹tÄ¶‚·‚é
#define GMD_DECO_USER_FLAG_SECOND_FRAME				(1 << 5)	//2”Ô–Ú‚Ì‹¤’ÊƒtƒŒ[ƒ€‚ğg—p‚·‚é
#define GMD_DECO_USER_FLAG_THIRD_FRAME				(1 << 6)	//3”Ô–Ú‚Ì‹¤’ÊƒtƒŒ[ƒ€‚ğg—p‚·‚é
#define GMD_DECO_USER_FLAG_USE_LIGHT_2_SUB_MODEL	(1 << 7)	//ƒTƒuƒ‚ƒfƒ‹‚É2”Ô–Ú‚Ìƒ‰ƒCƒg‚ğg—p‚·‚é
#define GMD_DECO_USER_FLAG_EFFECT_DUPLICATE_DRAW	(1 << 8)	//ƒGƒtƒFƒNƒg‚ğ•¡”‰ñ•`‰æ‚·‚é
#define GMD_DECO_USER_FLAG_NOCLIP					(1 << 9)	//ƒNƒŠƒbƒv‚µ‚È‚¢



#define GMD_DECO_COMMON_FRAME_INDEX_NUM				(3)		//‹¤’ÊƒtƒŒ[ƒ€ƒCƒ“ƒfƒNƒX”
#define GMD_DECO_LOOP_MODEL_NUM						(12)		//ƒ‹[ƒvƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€‚ğ•Û‚·‚é”
#define GMD_DECO_LOOP_MODEL_OFFSET					(2)		//‰æ–ÊŠO‚ÉÁ‚¦‚Ä‚µ‚Ü‚Á‚Ä‚¢‚é‚à‚Ì’²®—p

//ƒ‹[ƒvó‘Ô
enum GME_DECO_LOOP_STATE{
	GMD_DECO_LOOP_STATE_WAIT = 0,	//ƒ‹[ƒv‘Ò‚¿
	GMD_DECO_LOOP_STATE_LOOP,		//ƒ‹[ƒv’†
	GMD_DECO_LOOP_STATE_END,		//ƒ‹[ƒvI—¹

	GME_DECO_LOOP_STATE_NUM
};

//ƒ‚ƒfƒ‹ƒf[ƒ^ƒCƒ“ƒfƒNƒX
enum GME_DECO_DATA_INDEX_MODEL{
	GMD_DECO_DATA_INDEX_MODEL_ZONE1_GLARE = 0,		//ƒOƒŒƒA
	GMD_DECO_DATA_INDEX_MODEL_ZONE1_GLARE_NUM,
};

/// ‹éŒ`İ’è
enum GME_DECO_RECT{
	GMD_DECO_RECT_BODY	= 0,

	GMD_DECO_RECT_NUM
};

#if _IPHONE
/// ƒf[ƒ^g—p”»’è
typedef enum {
	GMD_DECO_USE_MODEL_TYPE_NORMAL = 0, /// ’Êíƒ‚ƒfƒ‹
	GMD_DECO_USE_MODEL_TYPE_RENDER, /// ƒŒƒ“ƒ_[ƒ‚ƒfƒ‹
	
	GMD_DECO_USE_MODEL_TYPE_NUM
} GME_DECO_USE_MODEL_TYPE;
#endif // _IPHONE

#define GMD_DECO_RENDER_NUM	(1)

///ƒIƒuƒWƒFƒNƒgˆ—ŠÖ”
typedef void (*GMF_DECO_OBJ_FUNC)(OBS_OBJECT_WORK*);
///‹éŒ`ˆ—ŠÖ”
typedef void (*GMF_DECO_RECT_FUNC)( OBS_RECT_WORK*, OBS_RECT_WORK* );

///ƒf[ƒ^ŠÇ—
typedef struct tag_GMS_DECO_DATA {
	AMS_AMB_HEADER* amb_header;						///< ambƒwƒbƒ_
	OBS_ACTION3D_NN_WORK* obj_3d_list;
	OBS_ACTION3D_NN_WORK* obj_3d_list_fall;
#if _IPHONE
	AMS_AMB_HEADER* tvx_model;
	AOS_TEXTURE     tvx_tex;
#endif // _IPHONE
}GMS_DECO_DATA;

///‘•üƒ[ƒN
typedef struct tag_GMS_DECO_WORK{
	OBS_OBJECT_WORK obj_work;		///< ƒIƒuƒWƒFƒNƒgƒ[ƒN
	OBS_ACTION3D_NN_WORK obj_3d;	///< 3DNNƒIƒuƒWƒFƒNƒg

	OBS_RECT_WORK rect_work[GMD_DECO_RECT_NUM];	///< ‹éŒ`
	
	GMS_EVE_RECORD_DECORATE* event_record;	///< ƒCƒxƒ“ƒgƒŒƒR[ƒh
	u8 event_x;	///< ƒCƒxƒ“ƒgƒŒƒR[ƒh‚ÌxÀ•W•Û—piƒCƒxƒ“ƒgƒŒƒR[ƒh‚ÌxÀ•W‚ÍƒXƒLƒbƒvƒtƒ‰ƒO‚É’u‚«Š·‚¦‚ç‚ê‚é‚Ì‚Åj
#if GMD_DECO_USE_DRAW_TVX
	AOS_TEXTURE* model_tex; ///< ƒ‚ƒfƒ‹—pƒeƒNƒXƒ`ƒƒ
	s32 model_index; ///< ƒ‚ƒfƒ‹index
#endif // GMD_DECO_USE_DRAW_TVX
} GMS_DECO_WORK;
typedef struct tag_GMS_DECO_SUBMODEL_WORK{
	GMS_DECO_WORK deco_work;			///< ‘•üƒ[ƒN
	OBS_ACTION3D_NN_WORK obj_3d_sub;	///< ƒTƒuƒ‚ƒfƒ‹—p3DNNƒIƒuƒWƒFƒNƒg
#if GMD_DECO_USE_DRAW_TVX
	s32 sub_model_index; ///< ƒTƒuƒ‚ƒfƒ‹index
#endif // GMD_DECO_USE_DRAW_TVX
} GMS_DECO_SUBMODEL_WORK;

///‘•üŠÇ—
typedef struct tag_GMS_DECO_MGR {	
	MTS_TASK_TCB* tcb_post;		///< Œãˆ—

	s32 common_frame_motion[GMD_DECO_COMMON_FRAME_INDEX_NUM];	///< ‹¤’Êƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€”

	//…ƒVƒF[ƒ_
	BOOL flag_render_front;	///< ƒŒƒ“ƒ_ƒtƒ‰ƒO
	BOOL flag_render_back;	///< ƒŒƒ“ƒ_ƒtƒ‰ƒOi‰œj
	AMS_RENDER_TARGET* render_target_front;	///< ƒŒƒ“ƒ_ƒ^[ƒQƒbƒg
	AMS_RENDER_TARGET* render_target_back;	///< ƒŒƒ“ƒ_ƒ^[ƒQƒbƒg

	//ƒ‹[ƒv‚·‚éÛ‚Ìƒ‚[ƒVƒ‡ƒ“Ä¶ˆÊ’u‚ğŠÇ—
	s32 motion_frame_loop[GMD_DECO_LOOP_MODEL_NUM];	//ƒ‹[ƒv¶¬‚Ìƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€”
	GME_DECO_LOOP_STATE state_loop;				//ƒ‹[ƒvó‘Ô
	GSS_SND_SE_HANDLE* se_handle;				//SEƒnƒ“ƒhƒ‹
}GMS_DECO_MGR;


#if GMD_DECO_USE_DRAW_SERVER
// ‘êŠÇ—
#define GMD_DECO_FALL_MANAGER_NUM	(16) // ŠÇ—Å‘å”
#define GMD_DECO_FALL_REGISTER_NUM	( 8) // 1ƒCƒxƒ“ƒg“o˜^Å‘å”

typedef struct tag_GMS_DECO_FALL_REGISTER {
	u32				num; // “o˜^”
	VecFx32			vec; // “o˜^ˆÊ’u
} GMS_DECO_FALL_REGISTER;

typedef struct tag_GMS_DECO_FALL_MANAGER {
	u32				dec_id;		//!< ƒCƒxƒ“ƒgID
	NNS_TEXLIST*	texlist;	//!< ƒeƒNƒXƒ`ƒƒƒŠƒXƒg
	u16				all_num;	//!< ‘S“o˜^”
	u16				reg_num;	//!< “o˜^”
	float			frame;		//!< ƒAƒjƒ[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€
	GMS_DECO_FALL_REGISTER reg[GMD_DECO_FALL_REGISTER_NUM];	//!< “o˜^”(XÀ•W)
} GMS_DECO_FALL_MANAGER;
#endif // GMD_DECO_USE_DRAW_SERVER

#if GMD_DECO_USE_DRAW_TVX
// ƒvƒŠƒ~ƒeƒBƒu•`‰æî•ñ
#define GMD_DECO_PRIM_DRAW_WORK_NUM		(16)	//!<	•`‰æƒ[ƒNŒÂ”(ÅI“I‚É‚ÍŒ¸‚é—\’è)
#define GMD_DECO_PRIM_DRAW_STACK_NUM		(128)	//!<	•`‰æƒXƒ^ƒbƒNŒÂ”(‘‚¦‚éH)

typedef struct tag_GMS_DECO_PRIM_DRAW_STACK {
	AOS_TVX_VERTEX* vtx; //!< ’¸“_î•ñ
	VecFx32 pos; //!< ˆÊ’uî•ñ
	Float off_y; //!< ˆÊ’uƒIƒtƒZƒbƒgY
	u32 disp_flag; //!< •`‰æî•ñ
	u32 vtx_num; //!< ’¸“_”
} GMS_DECO_PRIM_DRAW_STACK;

typedef struct tag_GMS_DECO_PRIM_DRAW_WORK {
	AOS_TEXTURE* tex; //!< aoƒeƒNƒXƒ`ƒƒ
	s32 tex_id; //!< g—pƒeƒNƒXƒ`ƒƒID
	u32 command; //!< •`‰æƒRƒ}ƒ“ƒh
	u16 all_vtx_num; //!< ‘’¸“_”
	u16 stack_num; //!< g—pƒXƒ^ƒbƒN”
	GMS_DECO_PRIM_DRAW_STACK stack[GMD_DECO_PRIM_DRAW_STACK_NUM]; //!< •`‰æ’¸“_ƒXƒ^ƒbƒN
} GMS_DECO_PRIM_DRAW_WORK;
#endif // GMD_DECO_USE_DRAW_TVX

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

// ==========================================================================
// ƒQ[ƒ€ƒVƒXƒeƒ€
// ==========================================================================
//static GSE_MAIN_STAGE_ID gmDecoGameSystemGetStageId( void );
static u32 gmDecoGameSystemGetSyncTime( void );

// ==========================================================================
// ƒf[ƒ^ŠÇ—
// ==========================================================================
static void gmDecoDataInit( void );
static void gmDecoDataRelease( void );
static GMS_DECO_DATA* gmDecoDataGetInfo( void );
static void gmDecoDataSetAmbHeader( AMS_AMB_HEADER* amb );
static AMS_AMB_HEADER* gmDecoDataGetAmbHeader( void );
static void gmDecoDataReleaseAmbHeader( void );
static OBS_ACTION3D_NN_WORK* gmDecoDataGetObj3DList( GME_DECORATE_ID id );
static AMS_AMB_HEADER* gmDecoDataGetMotionHeader( GME_DECORATE_ID id );
static AMS_AMB_HEADER* gmDecoDataGetMatMotionHeader( GME_DECORATE_ID id );
static AMS_RENDER_TARGET* gmDecoGetRenderWorkFront( void );
static AMS_RENDER_TARGET* gmDecoGetRenderWorkBack( void );

// ==========================================================================
// ‘•üŠÇ—
// ==========================================================================
//‘•üİ’è
static GMS_DECO_MGR* gmDecoGetMgr( void );
static void gmDecoInitMgr( void );
static void gmDecoExitMgr( void );
static void gmDecoReleaseMgr( void );


static void gmDecoCopySetRenderTargetForFront( AMS_RENDER_TARGET* target );
static void gmDecoCopySetRenderTargetForBack( AMS_RENDER_TARGET* target );

static MTS_TASK_TCB* gmDecoCreateTcbPost( void );
static void gmDecoDeleteTcbPost( void );
static void gmDecoTcbProcPost( MTS_TASK_TCB *tcb );
static void gmDecoTcbProcPostDT(void *data);

static void gmDecoDraw( OBS_OBJECT_WORK* obj_work );
static void gmDecoDrawFinalShutter3Line( OBS_OBJECT_WORK* obj_work );
static void gmDecoDrawFinalShutter5Line( OBS_OBJECT_WORK* obj_work );
static AMS_RENDER_TARGET* gmDecoDrawFallCopyRenderFront( void );
static AMS_RENDER_TARGET* gmDecoDrawFallCopyRenderBack( void );
static void gmDecoDrawFallRender( AMS_RENDER_TARGET* render_target, NNS_MATRIX44* proj_mtx );
static void gmDecoDrawFallFrontUserFunc(void *data);
static void gmDecoDrawFallBackUserFunc(void *data);
static void gmDecoDrawFallFront( OBS_OBJECT_WORK* obj_work );
static void gmDecoDrawFallBack( OBS_OBJECT_WORK* obj_work );

//ƒIƒuƒWƒFƒNƒg
static GMS_DECO_WORK* gmDecoLoadObj( 
						  GMS_EVE_RECORD_DECORATE* dec_rec, 
						  u32 work_size,
						  fx32 pos_x, 
						  fx32 pos_y,
						  GMF_DECO_OBJ_FUNC main_func,
						  GMF_DECO_OBJ_FUNC move_func,
						  GMF_DECO_OBJ_FUNC out_func,
						  GSF_TASK_DESTRUCTOR dest_func);
static GMS_DECO_WORK* gmDecoLoadModel(
							GMS_DECO_WORK* deco_work,
							OBS_ACTION3D_NN_WORK* obj_3d_work);
static void gmDecoLoadMotion( 
					  GMS_DECO_WORK* deco_work,
					  s32 motion_index,
					  AMS_AMB_HEADER* motion_amb);
static void gmDecoLoadMatMotion( 
					  GMS_DECO_WORK* deco_work,
					  s32 motion_index,
					  AMS_AMB_HEADER* motion_amb );
static void gmDecoSetRect( 
				   GMS_DECO_WORK* deco_work,
				   s16 left,
				   s16 top,
				   s16 back,
				   s16 right,
				   s16 bottom,
				   s16 front,
				   GMF_DECO_RECT_FUNC func);

// ==========================================================================
// ƒƒCƒ“ˆ—
// ==========================================================================
static void gmDecoMainFuncMotionCount( OBS_OBJECT_WORK *obj_work );
static void gmDecoMainFuncDecreaseMotionSpeed( OBS_OBJECT_WORK *obj_work );
static void gmDecoMainFuncMotionApplyCommonFrame( OBS_OBJECT_WORK *obj_work );
static void gmDecoMainFuncMotionCheckCommonFrame( OBS_OBJECT_WORK *obj_work );
static void gmDecoMainFuncLoop( OBS_OBJECT_WORK *obj_work );
static void gmDecoMainFuncEffectCheckCommonFrame( OBS_OBJECT_WORK *obj_work );

// ==========================================================================
// I—¹ˆ—
// ==========================================================================
static void gmDecoTcbDest( MTS_TASK_TCB *tcb );

// ==========================================================================
// ƒ‚[ƒVƒ‡ƒ“ˆ—
// ==========================================================================
static void gmDecoRectFuncChangeMotionCount( OBS_RECT_WORK* own_rect_work, OBS_RECT_WORK* target_rect_work );
static void gmDecoRectFuncChangeDecreaseMotionSpeed( OBS_RECT_WORK* own_rect_work, OBS_RECT_WORK* target_rect_work );
static void gmDecoRectFuncChangeMotionCommonFrame( OBS_RECT_WORK* own_rect_work, OBS_RECT_WORK* target_rect_work );

#if _WII
static NNE_BOOL gmDecoFallMaterialCallback( NNS_DRAWCALLBACK_VAL *val, void *param );
#endif	//_WII

// ==========================================================================
// “Áêˆ—
// ==========================================================================
static void gmDecoSetLightSpecial( OBS_OBJECT_WORK* obj_work, GME_DECORATE_ID id );
#if _IPHONE
static void gmDecoAdjustIPhone( OBS_OBJECT_WORK* obj_work, GME_DECORATE_ID id );
#endif	//_IPHONE

// ==========================================================================
// ‘•ü
// ==========================================================================
//‘•ü‰Šú‰»
static GMS_DECO_WORK* gmDecoInitNomodel( 
										GMS_EVE_RECORD_DECORATE* dec_rec,
										fx32 x,
										fx32 y,
										u8 type,
										u32 work_size);
static GMS_DECO_WORK* gmDecoInitModel( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );
static void gmDecoInitMotion( GMS_DECO_WORK* deco_work );
static GMS_DECO_WORK* gmDecoInitModelMotion( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );
static GMS_DECO_WORK* gmDecoInitModelMotionTouch( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );
static void gmDecoInitMaterial( GMS_DECO_WORK* deco_work );
static GMS_DECO_WORK* gmDecoInitModelMaterial( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );
static GMS_DECO_WORK* gmDecoInitModelMotioinMaterial( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );
static GMS_DECO_WORK* gmDecoInitModelMotionMaterialTouch( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );
static GMS_DECO_WORK* gmDecoInitModelLoop( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );
static GMS_DECO_WORK* gmDecoInitModelEffect( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );
static GMS_DECO_WORK* gmDecoInitPrimitive3D( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );
static GMS_DECO_WORK* gmDecoInitFall( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );
static GMS_DECO_WORK* gmDecoInitEffect( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );
static GMS_DECO_WORK* gmDecoInitEffectBlock( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );
static GMS_DECO_WORK* gmDecoInitEffectBlockAndNext( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );

#if GMD_DECO_USE_DRAW_SERVER
void gmDecoDrawServerMain(MTS_TASK_TCB *tcb);
static void gmDecoAddFallEvent(GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, NNS_TEXLIST* texlist, float frame);
static void gmDecoDelFallEvent(GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x);
static void gmDecoSetDrawFall(OBS_OBJECT_WORK* obj_work);
#endif // GMD_DECO_USE_DRAW_SERVER

#if GMD_DECO_USE_DRAW_TVX
static void gmDecoInitDrawPrimitive(void);
static void gmDecoSetDrawPrimitive(s32 model_index, AOS_TEXTURE* model_tex, VecFx32* pos, Float off_y, u32 command, u32 disp_flag);
static void gmDecoExecuteDrawPrimitive(void);
#endif // GMD_DECO_USE_DRAW_TVX

#if _IPHONE
static BOOL gmDecoIsUseModel(GME_DECO_USE_MODEL_TYPE type);
#endif // _IPHONE

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------


static const NNS_RGBA_U8 g_deco_rendaer_target_color = {0x00, 0x00, 0x00, 0xFF};	//ƒŒƒ“ƒ_[ƒ^[ƒQƒbƒgƒJƒ‰[

// ==========================================================================
///ƒf[ƒ^ŠÇ—
// ==========================================================================
//ƒf[ƒ^
static GMS_DECO_DATA g_deco_data_real;
static GMS_DECO_DATA* g_deco_data = NULL;

// ==========================================================================
///‘•üŠÇ—
// ==========================================================================
//ŠÇ—î•ñ
static GMS_DECO_MGR g_deco_mgr_real;
static GMS_DECO_MGR* g_deco_mgr = NULL;

#if GMD_DECO_USE_DRAW_SERVER
// ‘êŠÇ—
static GMS_DECO_FALL_MANAGER g_deco_fall_manager[GMD_DECO_FALL_MANAGER_NUM];
static MTS_TASK_TCB *gm_deco_draw_server_tcb	= NULL;
#endif // GMD_DECO_USE_DRAW_SERVER

#if GMD_DECO_USE_DRAW_TVX
static GMS_DECO_PRIM_DRAW_WORK g_deco_tvx_work[GMD_DECO_PRIM_DRAW_WORK_NUM];
#endif // GMD_DECO_USE_DRAW_TVX

// ==========================================================================
///‘•ü
// ==========================================================================

//ƒ‚ƒfƒ‹orƒGƒtƒFƒNƒgƒf[ƒ^ƒCƒ“ƒfƒNƒX
static const s32 g_gm_deco_model_index[GMD_DECORATE_ID_MAX][2] = {
	{IDB_DECO_ZONE3_MDL_Z3_D_BRI_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//•Ç
	{IDB_DECO_ZONE3_MDL_Z3_D_BRI_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//•Ç
	{IDB_DECO_ZONE3_MDL_Z3_D_FISH_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//•Ç
	{IDB_DECO_ZONE3_MDL_Z3_D_FISH_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//•Ç
	{IDB_DECO_ZONE3_MDL_Z3_D_PIL_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//•Ç
	{IDB_DECO_ZONE3_MDL_Z3_D_PIL_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//•Ç

	{IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶iƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_J30D_ZNO, IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_J30D_N_ZNO},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶iƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_J45D_ZNO, IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_J45D_N_ZNO},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶iƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_J60D_ZNO, IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_J60D_N_ZNO},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶iƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰EiƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_J30D_ZNO, IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_J30D_N_ZNO},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰EiƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_J45D_ZNO, IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_J45D_N_ZNO},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰EiƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_J60D_ZNO, IDB_GMK_WATER_SLIDER_MDL_GMK_WATER_SLDR_J60D_N_ZNO},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰EiƒMƒ~ƒbƒN‚Ìƒf[ƒ^j

	{GMD_DECO_DATA_INDEX_MODEL_ZONE1_GLARE, GMD_DECO_DATA_INDEX_INVALID},			// ƒOƒŒƒA
	{GMD_DECO_DATA_INDEX_MODEL_ZONE1_GLARE, GMD_DECO_DATA_INDEX_INVALID},			// ƒOƒŒƒA
	{GMD_DECO_DATA_INDEX_MODEL_ZONE1_GLARE, GMD_DECO_DATA_INDEX_INVALID},			// ƒOƒŒƒA

	{IDB_DECO_ZONE1_MDL_DECO_Z1_HIMAWARI_ZNO, GMD_DECO_DATA_INDEX_INVALID},		// ‚Ğ‚Ü‚í‚è
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WALLA_ZNO, GMD_DECO_DATA_INDEX_INVALID},			// •Ç
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WALLB_ZNO, GMD_DECO_DATA_INDEX_INVALID},			// •Ç

	{IDB_DECO_ZONE1_MDL_FALL_Z1_FALL_TEST_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//‘ê

	{IDB_DECO_ZONE1_MDL_DECO_Z1_ASIHANAA_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ƒAƒVƒnƒi
	{IDB_DECO_ZONE1_MDL_DECO_Z1_ASIHANAB_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ƒAƒVƒnƒi
	{IDB_DECO_ZONE1_MDL_DECO_Z1_ASIHANAC_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ƒAƒVƒnƒi
	{IDB_DECO_ZONE1_MDL_DECO_Z1_HANAA_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//‰Ô
	{IDB_DECO_ZONE1_MDL_DECO_Z1_HANAB_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//‰Ô
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WOODA_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//–Ø

	{IDB_DECO_ZONE1_MDL_FALL_Z1_FALL_TEST_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//‘êi‰œj
	
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WALLAA_ZNO, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WALLAB_ZNO, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WALLAC_ZNO, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WALLAD_ZNO, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WALLBA_ZNO, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WALLA_ZNO, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WALLB_ZNO, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WALLAA_ZNO, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WALLAB_ZNO, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WALLAC_ZNO, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WALLAD_ZNO, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WALLBA_ZNO, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj

	{IDB_DECO_ZONE1_MDL_FALL_Z1_FALL_LEFT_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//‘ê
	{IDB_DECO_ZONE1_MDL_FALL_Z1_FALL_RIGHT_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//‘ê
	{IDB_DECO_ZONE1_MDL_FALL_Z1_FALL_ONE_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//‘ê
	{IDB_DECO_ZONE1_MDL_FALL_Z1_D_FALL_TEST_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//‘ê
	{IDB_DECO_ZONE1_MDL_FALL_Z1_D_FALL_LEFT_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//‘ê
	{IDB_DECO_ZONE1_MDL_FALL_Z1_D_FALL_RIGHT_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//‘ê
	{IDB_DECO_ZONE1_MDL_FALL_Z1_D_FALL_ONE_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//‘ê

	{IDB_DECO_ZONE1_MDL_FALL_Z1_FALL_LEFT_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//‘êi‰œj
	{IDB_DECO_ZONE1_MDL_FALL_Z1_FALL_RIGHT_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//‘êi‰œj
	{IDB_DECO_ZONE1_MDL_FALL_Z1_FALL_ONE_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//‘êi‰œj
	{IDB_DECO_ZONE1_MDL_FALL_Z1_D_FALL_TEST_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//‘êi‰œj
	{IDB_DECO_ZONE1_MDL_FALL_Z1_D_FALL_LEFT_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//‘êi‰œj
	{IDB_DECO_ZONE1_MDL_FALL_Z1_D_FALL_RIGHT_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//‘êi‰œj
	{IDB_DECO_ZONE1_MDL_FALL_Z1_D_FALL_ONE_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//‘êi‰œj

	{GMD_DECO_DATA_INDEX_MODEL_ZONE1_GLARE, GMD_DECO_DATA_INDEX_INVALID},			// ƒOƒŒƒA

	{IDB_DECO_ZONE3_MDL_Z3_D_HAI_FR_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//”r‰t‘•’ui¶j
	{IDB_DECO_ZONE3_MDL_Z3_D_HAI_FR_EXIT_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//”r‰t‘•’uoŒûi¶j
	{IDB_DECO_ZONE3_MDL_Z3_D_HAI_FR_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//”r‰t‘•’ui‰Ej
	{IDB_DECO_ZONE3_MDL_Z3_D_HAI_FR_EXIT_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//”r‰t‘•’uoŒûi‰Ej

	{IDB_DECO_ZONE3_MDL_D_EAR_RUB_A_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3Šâ
	{IDB_DECO_ZONE3_MDL_D_EAR_RUB_A_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3Šâ
	{IDB_DECO_ZONE3_MDL_D_EAR_RUB_B_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3Šâ
	{IDB_DECO_ZONE3_MDL_D_EAR_RUB_B_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3Šâ
	{IDB_DECO_ZONE3_MDL_D_EAR_RUB_B_C_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3Šâ
	{IDB_DECO_ZONE3_MDL_D_WAT_RUB_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ
	{IDB_DECO_ZONE3_MDL_D_WAT_RUB_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ

	{GME_EFCT_Z03_IDX_SLIDER, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	{GME_EFCT_Z03_IDX_SLIDER_02, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	{GME_EFCT_Z03_IDX_CANDLE_01, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3 ˜XC
	{GME_EFCT_Z03_IDX_CANDLE_03, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3 ˜XC

	{IDB_DECO_ZONE3_RENDER_MDL_Z3_D_FOUN_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3•¬o‚·…
	{IDB_DECO_ZONE3_RENDER_MDL_Z3_D_FOUN_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3•¬o‚·…
	{IDB_DECO_ZONE3_RENDER_MDL_Z3_D_FOUN_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3•¬o‚·…i‰œj
	{IDB_DECO_ZONE3_RENDER_MDL_Z3_D_FOUN_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3•¬o‚·…i‰œj

	{IDB_DECO_ZONE3_MDL_Z3_D_PLANT_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{IDB_DECO_ZONE3_MDL_Z3_D_PLANT_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{IDB_DECO_ZONE3_MDL_Z3_D_PLANT_C_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{IDB_DECO_ZONE3_MDL_Z3_D_PLANT_D_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{IDB_DECO_ZONE3_MDL_Z3_D_PLANT_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{IDB_DECO_ZONE3_MDL_Z3_D_PLANT_C_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨

	{IDB_DECO_ZONE3_MDL_D_EAR_RUB_B_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3Šâi‘Oj
	{IDB_DECO_ZONE3_MDL_D_EAR_RUB_B_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3Šâi‘Oj
	{IDB_DECO_ZONE3_MDL_D_EAR_RUB_B_C_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3Šâi‘Oj
	{IDB_DECO_ZONE3_MDL_D_WAT_RUB_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâi‘Oj
	{IDB_DECO_ZONE3_MDL_D_WAT_RUB_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâi‘Oj
	{IDB_DECO_ZONE3_MDL_Z3_D_PLANT_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3A•¨i‘Oj
	{IDB_DECO_ZONE3_MDL_Z3_D_PLANT_D_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3A•¨i‘Oj
	{IDB_DECO_ZONE3_MDL_Z3_D_FISH_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3•Çi‘Oj
	{IDB_DECO_ZONE3_MDL_Z3_D_FISH_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3•Çi‘Oj

	{IDB_DECO_ZONE3_MDL_Z3_D_RAIL_EDGE_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3ƒŒ[ƒ‹Šp
	{IDB_DECO_ZONE3_MDL_Z3_D_RAIL_EDGE_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3ƒŒ[ƒ‹Šp
	{IDB_DECO_ZONE3_MDL_Z3_D_RAIL_EDGE_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	{IDB_DECO_ZONE3_MDL_Z3_D_RAIL_EDGE_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	{IDB_DECO_ZONE4_MDL_Z4_D_WHE_HOLD_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{IDB_DECO_ZONE4_MDL_Z4_D_WHE_HOLD_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{IDB_DECO_ZONE4_MDL_Z4_D_WHE_HOLD_C_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{IDB_DECO_ZONE4_MDL_Z4_D_WHE_HOLD_D_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{IDB_DECO_ZONE4_MDL_Z4_D_WHE_HOLD_C_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{IDB_DECO_ZONE4_MDL_Z4_D_WHE_HOLD_D_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p

	{GME_EFCT_Z01_IDX_TAKI_Z1, GMD_DECO_DATA_INDEX_INVALID},					//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},					//ZONE3 ˜XC
	
	{IDB_DECO_ZONE3_MDL_Z3_D_FISH_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	{IDB_DECO_ZONE3_MDL_Z3_D_FISH_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	{IDB_DECO_ZONE3_MDL_Z3_D_RAIL_J_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{IDB_DECO_ZONE3_MDL_Z3_D_RAIL_J_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{IDB_DECO_ZONE3_MDL_Z3_D_RAIL_J_C_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{IDB_DECO_ZONE3_MDL_Z3_D_RAIL_J_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	{IDB_DECO_ZONE3_MDL_Z3_D_RAIL_J_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	{IDB_DECO_ZONE3_MDL_Z3_D_RAIL_J_C_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	{GMD_DECO_DATA_INDEX_MODEL_ZONE1_GLARE, GMD_DECO_DATA_INDEX_INVALID},		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	{GMD_DECO_DATA_INDEX_MODEL_ZONE1_GLARE, GMD_DECO_DATA_INDEX_INVALID},		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	{IDB_DECO_ZONE1_MDL_DECO_Z1_HIMAWARI_ZNO, GMD_DECO_DATA_INDEX_INVALID},		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	{IDB_DECO_ZONE1_MDL_DECO_Z1_HANAA_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	{IDB_DECO_ZONE1_MDL_DECO_Z1_HANAB_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	{IDB_DECO_ZONE1_MDL_DECO_Z1_WOODA_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	{IDB_DECO_ZONE4_MDL_Z4_D_WHEEL_PIL_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_WHEEL_PIL_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_WHEEL_PIL_C_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_WHEEL_PIL_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{IDB_DECO_ZONE4_MDL_Z4_D_WHEEL_PIL_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{IDB_DECO_ZONE4_MDL_Z4_D_WHEEL_PIL_C_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{IDB_DECO_ZONE4_MDL_Z4_D_WHEEL_PIL_D_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_WHEEL_PIL_E_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_WHEEL_PIL_F_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_WHEEL_PIL_D_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œi¶‰E”½“]j
	{IDB_DECO_ZONE4_MDL_Z4_D_WHEEL_PIL_E_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œi¶‰E”½“]j
	{IDB_DECO_ZONE4_MDL_Z4_D_WHEEL_PIL_F_ZNO, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œi¶‰E”½“]j

	{IDB_DECO_ZONE4_MDL_Z4_D_WHE_HOLD_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{IDB_DECO_ZONE4_MDL_Z4_D_WHE_HOLD_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{IDB_DECO_ZONE4_MDL_Z4_D_WHE_HOLD_C_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{IDB_DECO_ZONE4_MDL_Z4_D_WHE_HOLD_D_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{IDB_DECO_ZONE4_MDL_Z4_D_WHE_HOLD_C_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{IDB_DECO_ZONE4_MDL_Z4_D_WHE_HOLD_D_ZNO, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	
	{IDB_DECO_ZONE4_MDL_Z4_D_BRACE_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_BRACE_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_BRACE_C_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_BRACE_D_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_BRACE_E_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_BRACE_F_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_BRACE_G_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ

	{IDB_DECO_ZONE4_MDL_Z4_D_PIL_COR_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ

	{IDB_DECO_ZONE4_MDL_Z4_D_WARNING_ZNO, GME_EFCT_Z04_IDX_LIGHT_Z4},				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},						//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	{IDB_DECO_ZONE4_MDL_Z4_D_BRACE_S_A_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_BRACE_S_B_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_BRACE_S_C_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_BRACE_S_D_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_BRACE_S_E_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_BRACE_S_F_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{IDB_DECO_ZONE4_MDL_Z4_D_BRACE_S_G_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_CO01_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_CO02_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_CO03_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_CO04_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_CO_L_01_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_CO_L_02_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_CO_R_01_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_CO_R_02_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_CO_T_01_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_CO_T_02_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_CO_U_01_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_CO_U_02_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_TUBE01_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_TUBE02_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_TUBE03_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_TUBE04_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_TUBE03_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	{IDB_DECO_ZONE4_MDL_Z4_P_STESM_TUBE04_ZNO, GMD_DECO_DATA_INDEX_INVALID},			//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	

	{IDB_DECO_ZONE4_MDL_Z4_D_UKI_RAIL_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF •‚“‡—pƒŒ[ƒ‹
	{IDB_DECO_ZONE4_MDL_Z4_D_UKI_RAIL_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF •‚“‡—pƒŒ[ƒ‹
	
	{IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_3A_ZNO, IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_B_ZNO},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_B_ZNO},				//ZONEF ƒVƒƒƒbƒ^[	

	{IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_3A_ZNO, IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_B_ZNO},	//ZONEF ƒVƒƒƒbƒ^[	
	{IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_3A_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_B_ZNO},				//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_B_ZNO},				//ZONEF ƒVƒƒƒbƒ^[	

	{IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_5A_ZNO, IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_B_ZNO},	//ZONEF ƒVƒƒƒbƒ^[	
	{IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_5A_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_B_ZNO},				//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_B_ZNO},				//ZONEF ƒVƒƒƒbƒ^[		

	{IDB_DECO_ZONE2_MDL_Z2_DECO_W_PLANTA_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE2 A•¨
	{IDB_DECO_ZONE2_MDL_Z2_DECO_W_PLANTB_ZNO, GMD_DECO_DATA_INDEX_INVALID},				//ZONE2 A•¨

#if 01
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
#else
	{GME_EFCT_ZFINAL_IDX_LIGHT_Z5, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
#endif // 01

	{IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_5A_ZNO, IDB_DECO_ZONEF_MDL_GMK_ZONEF_ST_B_ZNO},	//ZONEF ƒVƒƒƒbƒ^[	
};

//•`‰æƒtƒ‰ƒO
static const u32 g_gm_deco_disp_flag[GMD_DECORATE_ID_MAX] = {
	OBD_DISP_NODIRFLIP,						//•Ç
	OBD_DISP_NODIRFLIP,						//•Ç
	OBD_DISP_NODIRFLIP,						//•Ç
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,	//•Ç	
	OBD_DISP_NODIRFLIP,						//•Ç
	OBD_DISP_NODIRFLIP,						//•Ç

	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,					//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,					//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,					//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,					//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT | OBD_DISP_HFLIP,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT | OBD_DISP_HFLIP,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT | OBD_DISP_HFLIP,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT | OBD_DISP_HFLIP,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E

	OBD_DISP_NODIRFLIP,						//ƒOƒŒƒA
	OBD_DISP_NODIRFLIP,						//ƒOƒŒƒA
	OBD_DISP_NODIRFLIP,						//ƒOƒŒƒA

	OBD_DISP_NODIRFLIP | OBD_DISP_STOP,		//‚Ğ‚Ü‚í‚è
	OBD_DISP_NODIRFLIP,						//•Ç
	OBD_DISP_NODIRFLIP,						//•Ç

	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘ê

	OBD_DISP_NODIRFLIP,		//ƒAƒVƒnƒi
	OBD_DISP_NODIRFLIP,		//ƒAƒVƒnƒi
	OBD_DISP_NODIRFLIP,		//ƒAƒVƒnƒi
	OBD_DISP_NODIRFLIP,		//‰Ô
	OBD_DISP_NODIRFLIP,		//‰Ô
	OBD_DISP_NODIRFLIP | OBD_DISP_STOP | OBD_DISP_REPEAT,		//–Ø

	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‰œ‚Ì‘ê
	
	OBD_DISP_NODIRFLIP,		// •Ç
	OBD_DISP_NODIRFLIP,		// •Ç
	OBD_DISP_NODIRFLIP,		// •Ç
	OBD_DISP_NODIRFLIP,		// •Ç
	OBD_DISP_NODIRFLIP,		// •Ç
	
	OBD_DISP_NODIRFLIP,		// •Çi‰œj
	OBD_DISP_NODIRFLIP,		// •Çi‰œj
	OBD_DISP_NODIRFLIP,		// •Çi‰œj
	OBD_DISP_NODIRFLIP,		// •Çi‰œj
	OBD_DISP_NODIRFLIP,		// •Çi‰œj
	OBD_DISP_NODIRFLIP,		// •Çi‰œj
	OBD_DISP_NODIRFLIP,		// •Çi‰œj

	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘ê
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘ê
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘ê
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘ê
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘ê
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘ê
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘ê

	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘êi‰œj
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘êi‰œj
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘êi‰œj
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘êi‰œj
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘êi‰œj
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘êi‰œj
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,		//‘êi‰œj

	OBD_DISP_NODIRFLIP,						//ƒOƒŒƒA

	OBD_DISP_NODIRFLIP,		//”r‰t‘•’ui¶j
	OBD_DISP_NODIRFLIP,		//”r‰t‘•’uoŒûi¶j
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,		//”r‰t‘•’ui‰Ej
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,		//”r‰t‘•’uoŒûi‰Ej

	OBD_DISP_NODIRFLIP,			//ZONE3Šâ
	OBD_DISP_NODIRFLIP,			//ZONE3Šâ
	OBD_DISP_NODIRFLIP,			//ZONE3Šâ
	OBD_DISP_NODIRFLIP,			//ZONE3Šâ
	OBD_DISP_NODIRFLIP,			//ZONE3Šâ
	OBD_DISP_NODIRFLIP,			//ZONE3Šâ
	OBD_DISP_NODIRFLIP,			//ZONE3Šâ

	OBD_DISP_NODIRFLIP,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	OBD_DISP_NODIRFLIP,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	OBD_DISP_NODIRFLIP,			//ZONE3 ˜XC
	OBD_DISP_NODIRFLIP,			//ZONE3 •Ç‚ÌŠç

	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,			//ZONE3•¬o‚·…
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,			//ZONE3•¬o‚·…
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,			//ZONE3•¬o‚·…i‰œj
	OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT,			//ZONE3•¬o‚·…i‰œj

	OBD_DISP_NODIRFLIP,				//ZONE3A•¨
	OBD_DISP_NODIRFLIP,				//ZONE3A•¨
	OBD_DISP_NODIRFLIP,				//ZONE3A•¨
	OBD_DISP_NODIRFLIP,				//ZONE3A•¨
	OBD_DISP_NODIRFLIP,				//ZONE3A•¨
	OBD_DISP_NODIRFLIP,				//ZONE3A•¨

	OBD_DISP_NODIRFLIP,						//ZONE3Šâi‘Oj
	OBD_DISP_NODIRFLIP,						//ZONE3Šâi‘Oj
	OBD_DISP_NODIRFLIP,						//ZONE3Šâi‘Oj
	OBD_DISP_NODIRFLIP,						//ZONE3Šâi‘Oj
	OBD_DISP_NODIRFLIP,						//ZONE3Šâi‘Oj
	OBD_DISP_NODIRFLIP,						//ZONE3A•¨i‘Oj
	OBD_DISP_NODIRFLIP,						//ZONE3A•¨i‘Oj
	OBD_DISP_NODIRFLIP,						//ZONE3•Çi‘Oj
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,	//ZONE3•Çi‘Oj

	OBD_DISP_NODIRFLIP,		//ZONE3ƒŒ[ƒ‹Šp
	OBD_DISP_NODIRFLIP,		//ZONE3ƒŒ[ƒ‹Šp
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	OBD_DISP_NODIRFLIP,		//ZONE4•Ô—p
	OBD_DISP_NODIRFLIP,		//ZONE4•Ô—p
	OBD_DISP_NODIRFLIP,		//ZONE4•Ô—p
	OBD_DISP_NODIRFLIP,		//ZONE4•Ô—p
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,		//ZONE4•Ô—p
	OBD_DISP_NODIRFLIP | OBD_DISP_VFLIP,		//ZONE4•Ô—p

	OBD_DISP_NODIRFLIP,					//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	OBD_DISP_NODIRFLIP,					//ZONE3 ˜XC
	
	OBD_DISP_NODIRFLIP,						//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,	//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	OBD_DISP_NODIRFLIP,						//ZONE3 ‘åŠâƒŒ[ƒ‹
	OBD_DISP_NODIRFLIP,						//ZONE3 ‘åŠâƒŒ[ƒ‹
	OBD_DISP_NODIRFLIP,						//ZONE3 ‘åŠâƒŒ[ƒ‹
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,	//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,	//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,	//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	OBD_DISP_NODIRFLIP,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DISP_NODIRFLIP,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DISP_NODIRFLIP | OBD_DISP_STOP,		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DISP_NODIRFLIP,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DISP_NODIRFLIP,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DISP_NODIRFLIP | OBD_DISP_STOP | OBD_DISP_REPEAT,		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	OBD_DISP_NODIRFLIP,	//ZONE4•Ô—p’Œ
	OBD_DISP_NODIRFLIP,	//ZONE4•Ô—p’Œ
	OBD_DISP_NODIRFLIP,	//ZONE4•Ô—p’Œ
	OBD_DISP_NODIRFLIP | OBD_DISP_VFLIP,	//ZONE4•Ô—p’Œiã‰º”½“]j
	OBD_DISP_NODIRFLIP | OBD_DISP_VFLIP,	//ZONE4•Ô—p’Œiã‰º”½“]j
	OBD_DISP_NODIRFLIP | OBD_DISP_VFLIP,	//ZONE4•Ô—p’Œiã‰º”½“]j
	OBD_DISP_NODIRFLIP,	//ZONE4•Ô—p’Œ
	OBD_DISP_NODIRFLIP,	//ZONE4•Ô—p’Œ
	OBD_DISP_NODIRFLIP,	//ZONE4•Ô—p’Œ
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,	//ZONE4•Ô—p’Œi¶‰E”½“]j

	OBD_DISP_NODIRFLIP,		//ZONE4•Ô—pi‘Oj
	OBD_DISP_NODIRFLIP,		//ZONE4•Ô—pi‘Oj
	OBD_DISP_NODIRFLIP,		//ZONE4•Ô—pi‘Oj
	OBD_DISP_NODIRFLIP,		//ZONE4•Ô—pi‘Oj
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,		//ZONE4•Ô—pi‘Oj
	OBD_DISP_NODIRFLIP | OBD_DISP_VFLIP,		//ZONE4•Ô—pi‘Oj
	
	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ
	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ
	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ
	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ
	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ
	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ
	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ

	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ

	OBD_DISP_NODIRFLIP,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ
	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ
	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ
	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ
	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ
	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ
	OBD_DISP_NODIRFLIP,				//ZONE4 ’Œ
	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DISP_NODIRFLIP,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DISP_NODIRFLIP | OBD_DISP_VFLIP,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	
	OBD_DISP_NODIRFLIP,			//ZONEF •‚“‡—pƒŒ[ƒ‹
	OBD_DISP_NODIRFLIP | OBD_DISP_HFLIP,			//ZONEF •‚“‡—pƒŒ[ƒ‹
	
	OBD_DISP_NODIRFLIP | OBD_DISP_STOP,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DISP_NODIRFLIP | OBD_DISP_STOP,			//ZONEF ƒVƒƒƒbƒ^[	

	OBD_DISP_NODIRFLIP,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DISP_NODIRFLIP,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DISP_NODIRFLIP | OBD_DISP_STOP,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DISP_NODIRFLIP,			//ZONEF ƒVƒƒƒbƒ^[	

	OBD_DISP_NODIRFLIP,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DISP_NODIRFLIP,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DISP_NODIRFLIP | OBD_DISP_STOP,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DISP_NODIRFLIP,			//ZONEF ƒVƒƒƒbƒ^[	
	
	OBD_DISP_NODIRFLIP,				//ZONE2 A•¨
	OBD_DISP_NODIRFLIP,				//ZONE2 A•¨

	OBD_DISP_NODIRFLIP | OBD_DISP_NOUPDATE | OBD_DISP_NODISP,	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
	
	OBD_DISP_NODIRFLIP | OBD_DISP_STOP,			//ZONEF ƒVƒƒƒbƒ^[	
};

//ƒm[ƒhƒ‚[ƒVƒ‡ƒ“ƒf[ƒ^ƒCƒ“ƒfƒNƒX
static const s32 g_gm_deco_node_motion_index[GMD_DECORATE_ID_MAX][2] = {
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//•Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//•Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//•Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//•Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//•Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//•Ç

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶iƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{GMD_DECO_DATA_INDEX_INVALID, IDB_GMK_WATER_SLIDER_MTN_GMK_WATER_SLDR_30D_N_ZNM},		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶iƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{GMD_DECO_DATA_INDEX_INVALID, IDB_GMK_WATER_SLIDER_MTN_GMK_WATER_SLDR_45D_N_ZNM},		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶iƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{GMD_DECO_DATA_INDEX_INVALID, IDB_GMK_WATER_SLIDER_MTN_GMK_WATER_SLDR_60D_N_ZNM},		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶iƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰EiƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{GMD_DECO_DATA_INDEX_INVALID, IDB_GMK_WATER_SLIDER_MTN_GMK_WATER_SLDR_30D_N_ZNM},		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰EiƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{GMD_DECO_DATA_INDEX_INVALID, IDB_GMK_WATER_SLIDER_MTN_GMK_WATER_SLDR_45D_N_ZNM},		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰EiƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{GMD_DECO_DATA_INDEX_INVALID, IDB_GMK_WATER_SLIDER_MTN_GMK_WATER_SLDR_60D_N_ZNM},		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰EiƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ƒOƒŒƒA
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ƒOƒŒƒA
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ƒOƒŒƒA
	
	{IDB_DECO_ZONE1_MTN_DECO_Z1_HIMAWARI_ZNM, GMD_DECO_DATA_INDEX_INVALID},	//‚Ğ‚Ü‚í‚è
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//•Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//•Ç
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//‘ê

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ƒAƒVƒnƒi
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ƒAƒVƒnƒi
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ƒAƒVƒnƒi
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‰Ô
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‰Ô
	{IDB_DECO_ZONE1_MTN_DECO_Z1_WOODA_ZNM, GMD_DECO_DATA_INDEX_INVALID},		//–Ø
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‰œ‚Ì‘ê
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‘ê
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‘ê
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‘ê
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‘ê
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‘ê
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‘ê
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‘ê

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‘êi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‘êi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‘êi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‘êi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‘êi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‘êi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‘êi‰œj

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ƒOƒŒƒA

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//”r‰t‘•’ui¶j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//”r‰t‘•’uoŒûi¶j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//”r‰t‘•’ui‰Ej
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//”r‰t‘•’uoŒûi‰Ej

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3 ˜XC
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3 •Ç‚ÌŠç

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3•¬o‚·…
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3•¬o‚·…
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3•¬o‚·…i‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3•¬o‚·…i‰œj

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3Šâi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3Šâi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3Šâi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3Šâi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3Šâi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨i‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨i‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3•Çi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3•Çi‘Oj

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3ƒŒ[ƒ‹Šp
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3ƒŒ[ƒ‹Šp
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ˜XC
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	{IDB_DECO_ZONE1_MTN_DECO_Z1_HIMAWARI_ZNM, GMD_DECO_DATA_INDEX_INVALID},	// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	{IDB_DECO_ZONE1_MTN_DECO_Z1_WOODA_ZNM, GMD_DECO_DATA_INDEX_INVALID},		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œi¶‰E”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œi¶‰E”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œi¶‰E”½“]j

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONEF •‚“‡—pƒŒ[ƒ‹
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONEF •‚“‡—pƒŒ[ƒ‹	
#if GMD_DECO_TEST_FINAL_SHUTTER
	{IDB_DECO_ZONEF_MTN_GMK_ZONEF_ST_3A_ZNM, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, IDB_DECO_ZONEF_MTN_GMK_ZONEF_ST_B_ZNM},				//ZONEF ƒVƒƒƒbƒ^[	

	{IDB_DECO_ZONEF_MTN_GMK_ZONEF_ST_3A_ZNM, IDB_DECO_ZONEF_MTN_GMK_ZONEF_ST_B_ZNM},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},							//ZONF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, IDB_DECO_ZONEF_MTN_GMK_ZONEF_ST_B_ZNM},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF ƒVƒƒƒbƒ^[	

	{IDB_DECO_ZONEF_MTN_GMK_ZONEF_ST_5A_ZNM, IDB_DECO_ZONEF_MTN_GMK_ZONEF_ST_B_ZNM},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},							//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, IDB_DECO_ZONEF_MTN_GMK_ZONEF_ST_B_ZNM},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF ƒVƒƒƒbƒ^[	
#else
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF ƒVƒƒƒbƒ^[	

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},							//ZONF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF ƒVƒƒƒbƒ^[	

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},							//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF ƒVƒƒƒbƒ^[
#endif	//
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE2 A•¨
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE2 A•¨

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg

	{IDB_DECO_ZONEF_MTN_GMK_ZONEF_ST_5A_ZNM, IDB_DECO_ZONEF_MTN_GMK_ZONEF_ST_B_ZNM},				//ZONEF ƒVƒƒƒbƒ^[
};

//ƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“ƒf[ƒ^ƒCƒ“ƒfƒNƒX
static const s32 g_gm_deco_mat_motion_index[GMD_DECORATE_ID_MAX][2] = {
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//•Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//•Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//•Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//•Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//•Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//•Ç

	{IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_ZNV, GMD_DECO_DATA_INDEX_INVALID},		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶iƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_J30D_ZNV, IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_J30D_N_ZNV},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶iƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_J45D_ZNV, IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_J45D_N_ZNV},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶iƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_J60D_ZNV, IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_J60D_N_ZNV},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶iƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_ZNV, GMD_DECO_DATA_INDEX_INVALID},		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰EiƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_J30D_ZNV, IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_J30D_N_ZNV},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰EiƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_J45D_ZNV, IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_J45D_N_ZNV},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰EiƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	{IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_J60D_ZNV, IDB_GMK_WATER_SLIDER_MAT_GMK_WATER_SLDR_J60D_N_ZNV},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰EiƒMƒ~ƒbƒN‚Ìƒf[ƒ^j
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ƒOƒŒƒA
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ƒOƒŒƒA
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ƒOƒŒƒA
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//‚Ğ‚Ü‚í‚è
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//•Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//•Ç
	
	{IDB_DECO_ZONE1_MAT_Z1_FALL_TEST_ZNV, GMD_DECO_DATA_INDEX_INVALID},			//‘ê

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ƒAƒVƒnƒi
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ƒAƒVƒnƒi
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ƒAƒVƒnƒi
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‰Ô
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‰Ô
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//–Ø
	
	{IDB_DECO_ZONE1_MAT_Z1_FALL_TEST_ZNV, GMD_DECO_DATA_INDEX_INVALID},			//‰œ‚Ì‘ê
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Ç
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// •Çi‰œj

	{IDB_DECO_ZONE1_MAT_Z1_FALL_LEFT_ZNV, GMD_DECO_DATA_INDEX_INVALID},		//‘ê
	{IDB_DECO_ZONE1_MAT_Z1_FALL_RIGHT_ZNV, GMD_DECO_DATA_INDEX_INVALID},		//‘ê
	{IDB_DECO_ZONE1_MAT_Z1_FALL_ONE_ZNV, GMD_DECO_DATA_INDEX_INVALID},			//‘ê
	{IDB_DECO_ZONE1_MAT_Z1_D_FALL_TEST_A_ZNV, GMD_DECO_DATA_INDEX_INVALID},	//‘ê
	{IDB_DECO_ZONE1_MAT_Z1_D_FALL_LEFT_A_ZNV, GMD_DECO_DATA_INDEX_INVALID},	//‘ê
	{IDB_DECO_ZONE1_MAT_Z1_D_FALL_RIGHT_A_ZNV, GMD_DECO_DATA_INDEX_INVALID},	//‘ê
	{IDB_DECO_ZONE1_MAT_Z1_D_FALL_ONE_A_ZNV, GMD_DECO_DATA_INDEX_INVALID},		//‘ê

	{IDB_DECO_ZONE1_MAT_Z1_FALL_LEFT_ZNV, GMD_DECO_DATA_INDEX_INVALID},		//‘êi‰œj
	{IDB_DECO_ZONE1_MAT_Z1_FALL_RIGHT_ZNV, GMD_DECO_DATA_INDEX_INVALID},		//‘êi‰œj
	{IDB_DECO_ZONE1_MAT_Z1_FALL_ONE_ZNV, GMD_DECO_DATA_INDEX_INVALID},			//‘êi‰œj
	{IDB_DECO_ZONE1_MAT_Z1_D_FALL_TEST_A_ZNV, GMD_DECO_DATA_INDEX_INVALID},	//‘êi‰œj
	{IDB_DECO_ZONE1_MAT_Z1_D_FALL_LEFT_A_ZNV, GMD_DECO_DATA_INDEX_INVALID},	//‘êi‰œj
	{IDB_DECO_ZONE1_MAT_Z1_D_FALL_RIGHT_A_ZNV, GMD_DECO_DATA_INDEX_INVALID},	//‘êi‰œj
	{IDB_DECO_ZONE1_MAT_Z1_D_FALL_ONE_A_ZNV, GMD_DECO_DATA_INDEX_INVALID},		//‘êi‰œj

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ƒOƒŒƒA

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//”r‰t‘•’ui¶j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//”r‰t‘•’uoŒûi¶j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//”r‰t‘•’ui‰Ej
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//”r‰t‘•’uoŒûi‰Ej

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3Šâ

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3 ˜XC
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3 •Ç‚ÌŠç

	{IDB_DECO_ZONE3_MAT_Z3_D_FOUN_A_ZNV, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3•¬o‚·…
	{IDB_DECO_ZONE3_MAT_Z3_D_FOUN_B_ZNV, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3•¬o‚·…
	{IDB_DECO_ZONE3_MAT_Z3_D_FOUN_A_ZNV, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3•¬o‚·…i‰œj
	{IDB_DECO_ZONE3_MAT_Z3_D_FOUN_B_ZNV, GMD_DECO_DATA_INDEX_INVALID},			//ZONE3•¬o‚·…i‰œj

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3Šâi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3Šâi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3Šâi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3Šâi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3Šâi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨i‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3A•¨i‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3•Çi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE3•Çi‘Oj

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3ƒŒ[ƒ‹Šp
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3ƒŒ[ƒ‹Šp
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—p

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ˜XC
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œi¶‰E”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œi¶‰E”½“]j
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4•Ô—p’Œi¶‰E”½“]j

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},		//ZONE4•Ô—pi‘Oj
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ’Œ
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	
#if GMD_DECO_TEST_FINAL_SHUTTER
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, IDB_DECO_ZONEF_MAT_GMK_ZONEF_ST_B_ZNV},	//ZONEF ƒVƒƒƒbƒ^[	

	{GMD_DECO_DATA_INDEX_INVALID, IDB_DECO_ZONEF_MAT_GMK_ZONEF_ST_B_ZNV},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	

	{GMD_DECO_DATA_INDEX_INVALID, IDB_DECO_ZONEF_MAT_GMK_ZONEF_ST_B_ZNV},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	
#else
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒVƒƒƒbƒ^[	
#endif	//
	
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE2 A•¨
	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},				//ZONE2 A•¨

	{GMD_DECO_DATA_INDEX_INVALID, GMD_DECO_DATA_INDEX_INVALID},	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
	
	{GMD_DECO_DATA_INDEX_INVALID, IDB_DECO_ZONEF_MAT_GMK_ZONEF_ST_B_ZNV},	//ZONEF ƒVƒƒƒbƒ^[	
};

//ƒ†[ƒUƒ[ƒN
static const u32 g_gm_deco_user_work[GMD_DECORATE_ID_MAX] = {
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç

	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	
	GMD_DECORATE_ID_GLARE_ORANGE,	//ƒOƒŒƒA
	GMD_DECORATE_ID_GLARE_RED,		//ƒOƒŒƒA
	GMD_DECORATE_ID_GLARE_GREEN,	//ƒOƒŒƒA
	
	20,	//‚Ğ‚Ü‚í‚è
	0,	//•Ç
	0,	//•Ç
	
	0,	//‘ê

	0,		//ƒAƒVƒnƒi
	0,		//ƒAƒVƒnƒi
	0,		//ƒAƒVƒnƒi
	0,		//‰Ô
	0,		//‰Ô
	1,		//–Ø
	
	0,		//‰œ‚Ì‘ê
	
	0,		// •Ç
	0,		// •Ç
	0,		// •Ç
	0,		// •Ç
	0,		// •Ç
	
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj

	0,		//‘ê
	0,		//‘ê
	0,		//‘ê
	0,		//‘ê
	0,		//‘ê
	0,		//‘ê
	0,		//‘ê

	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj

	GMD_DECORATE_ID_GLARE_SALMON,	//ƒOƒŒƒA

	0,		//”r‰t‘•’ui¶j
	0,		//”r‰t‘•’uoŒûi¶j
	0,		//”r‰t‘•’ui‰Ej
	0,		//”r‰t‘•’uoŒûi‰Ej

	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ

	0,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	0,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	0,			//ZONE3 ˜XC
	0,			//ZONE3 •Ç‚ÌŠç

	0,			//ZONE3•¬o‚·…
	0,			//ZONE3•¬o‚·…
	0,			//ZONE3•¬o‚·…i‰œj
	0,			//ZONE3•¬o‚·…i‰œj

	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨

	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3A•¨i‘Oj
	0,				//ZONE3A•¨i‘Oj
	0,				//ZONE3•Çi‘Oj
	0,				//ZONE3•Çi‘Oj

	0,		//ZONE3ƒŒ[ƒ‹Šp
	0,		//ZONE3ƒŒ[ƒ‹Šp
	0,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	0,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p

	0,		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	0,		//ZONE3 ˜XC
	
	0,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	0,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	GMD_DECORATE_ID_GLARE_ORANGE_ENDING,	// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	GMD_DECORATE_ID_GLARE_GREEN_ENDING,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	20,		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	0,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	0,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	1,		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œiã‰º”½“]j
	0,	//ZONE4•Ô—p’Œiã‰º”½“]j
	0,	//ZONE4•Ô—p’Œiã‰º”½“]j
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	0,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	0,	//ZONE4•Ô—p’Œi¶‰E”½“]j

	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	
	0,				//ZONE4 ’Œ

	0xffc60020,		//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	0,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	

	0,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	0,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	

	0,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	300,		//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	

	0,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	300,		//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	
	0,				//ZONE2 A•¨
	0,				//ZONE2 A•¨

	0x00000022,	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
	
	230,			//ZONEF ƒVƒƒƒbƒ^[	
};

//ƒ†[ƒUƒ^ƒCƒ}
static const s32 g_gm_deco_user_timer[GMD_DECORATE_ID_MAX] = {
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E

	0,	//ƒOƒŒƒA
	0,	//ƒOƒŒƒA
	0,	//ƒOƒŒƒA

	60,	//‚Ğ‚Ü‚í‚è
	0,	//•Ç
	0,	//•Ç
	
	0,	//‘ê

	0,		//ƒAƒVƒnƒi
	0,		//ƒAƒVƒnƒi
	0,		//ƒAƒVƒnƒi
	0,		//‰Ô
	0,		//‰Ô
	30,		//–Ø
	
	0,	//‰œ‚Ì‘ê
	
	0,		// •Ç
	0,		// •Ç
	0,		// •Ç
	0,		// •Ç
	0,		// •Ç
	
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj

	0,		//‘ê
	0,		//‘ê
	0,		//‘ê
	0,		//‘ê
	0,		//‘ê
	0,		//‘ê
	0,		//‘ê

	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj

	0,	//ƒOƒŒƒA

	0,		//”r‰t‘•’ui¶j
	0,		//”r‰t‘•’uoŒûi¶j
	0,		//”r‰t‘•’ui‰Ej
	0,		//”r‰t‘•’uoŒûi‰Ej

	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ

	0,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	0,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	0,			//ZONE3 ˜XC
	0,			//ZONE3 •Ç‚ÌŠç

	0,			//ZONE3•¬o‚·…
	0,			//ZONE3•¬o‚·…
	0,			//ZONE3•¬o‚·…i‰œj
	0,			//ZONE3•¬o‚·…i‰œj

	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨

	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3A•¨i‘Oj
	0,				//ZONE3A•¨i‘Oj
	0,				//ZONE3•Çi‘Oj
	0,				//ZONE3•Çi‘Oj

	0,		//ZONE3ƒŒ[ƒ‹Šp
	0,		//ZONE3ƒŒ[ƒ‹Šp
	0,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	0,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p

	0,		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	0,		//ZONE3 ˜XC
	
	0,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	0,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	0,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	0,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	60,		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	0,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	0,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	30,		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œiã‰º”½“]j
	0,	//ZONE4•Ô—p’Œiã‰º”½“]j
	0,	//ZONE4•Ô—p’Œiã‰º”½“]j
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	0,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	0,	//ZONE4•Ô—p’Œi¶‰E”½“]j

	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	
	0,				//ZONE4 ’Œ

	0,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	0,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	

	0,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	0,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	

	0,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	300,		//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	

	0,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	300,		//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	
	0,				//ZONE2 A•¨
	0,				//ZONE2 A•¨

	0,	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
};

//ƒ†[ƒUƒtƒ‰ƒO
static const u32 g_gm_deco_user_flag[GMD_DECORATE_ID_MAX] = {
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	GMD_DECO_USER_FLAG_SYNC_MATERIAL | GMD_DECO_USER_FLAG_SUB_MODEL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	GMD_DECO_USER_FLAG_SYNC_MATERIAL | GMD_DECO_USER_FLAG_SUB_MODEL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	GMD_DECO_USER_FLAG_SYNC_MATERIAL | GMD_DECO_USER_FLAG_SUB_MODEL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	GMD_DECO_USER_FLAG_SYNC_MATERIAL | GMD_DECO_USER_FLAG_SUB_MODEL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	GMD_DECO_USER_FLAG_SYNC_MATERIAL | GMD_DECO_USER_FLAG_SUB_MODEL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	GMD_DECO_USER_FLAG_SYNC_MATERIAL | GMD_DECO_USER_FLAG_SUB_MODEL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E

	0,	//ƒOƒŒƒA
	0,	//ƒOƒŒƒA
	0,	//ƒOƒŒƒA

	GMD_DECO_USER_FLAG_TOUCH_REVERSE_MOTION,	//‚Ğ‚Ü‚í‚è
	0,	//•Ç
	0,	//•Ç
	
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,	//‘ê

	0,		//ƒAƒVƒnƒi
	0,		//ƒAƒVƒnƒi
	0,		//ƒAƒVƒnƒi
	0,		//‰Ô
	0,		//‰Ô
	0,		//–Ø
	
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,	//‰œ‚Ì‘ê
	
	0,		// •Ç
	0,		// •Ç
	0,		// •Ç
	0,		// •Ç
	0,		// •Ç
	
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj

	GMD_DECO_USER_FLAG_SYNC_MATERIAL,		//‘ê
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,		//‘ê
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,		//‘ê
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,		//‘ê
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,		//‘ê
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,		//‘ê
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,		//‘ê

	GMD_DECO_USER_FLAG_SYNC_MATERIAL,		//‘êi‰œj
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,		//‘êi‰œj
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,		//‘êi‰œj
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,		//‘êi‰œj
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,		//‘êi‰œj
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,		//‘êi‰œj
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,		//‘êi‰œj

	0,	//ƒOƒŒƒA

	0,		//”r‰t‘•’ui¶j
	0,		//”r‰t‘•’uoŒûi¶j
	0,		//”r‰t‘•’ui‰Ej
	0,		//”r‰t‘•’uoŒûi‰Ej

	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ

	0,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	0,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	0,			//ZONE3 ˜XC
	0,			//ZONE3 •Ç‚ÌŠç

	GMD_DECO_USER_FLAG_SYNC_MATERIAL,			//ZONE3•¬o‚·…
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,			//ZONE3•¬o‚·…
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,			//ZONE3•¬o‚·…i‰œj
	GMD_DECO_USER_FLAG_SYNC_MATERIAL,			//ZONE3•¬o‚·…i‰œj

	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨

	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3A•¨i‘Oj
	0,				//ZONE3A•¨i‘Oj
	0,				//ZONE3•Çi‘Oj
	0,				//ZONE3•Çi‘Oj

	0,		//ZONE3ƒŒ[ƒ‹Šp
	0,		//ZONE3ƒŒ[ƒ‹Šp
	0,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	0,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p

	0,		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	0,		//ZONE3 ˜XC
	
	0,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	0,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	GMD_DECO_USER_FLAG_CHECK_CHAOS_EMERALD,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	GMD_DECO_USER_FLAG_CHECK_CHAOS_EMERALD,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	GMD_DECO_USER_FLAG_CHECK_CHAOS_EMERALD | GMD_DECO_USER_FLAG_TOUCH_REVERSE_MOTION,		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	GMD_DECO_USER_FLAG_CHECK_CHAOS_EMERALD,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	GMD_DECO_USER_FLAG_CHECK_CHAOS_EMERALD,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	GMD_DECO_USER_FLAG_CHECK_CHAOS_EMERALD,		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œiã‰º”½“]j
	0,	//ZONE4•Ô—p’Œiã‰º”½“]j
	0,	//ZONE4•Ô—p’Œiã‰º”½“]j
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	0,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	0,	//ZONE4•Ô—p’Œi¶‰E”½“]j

	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ

	0,				//ZONE4 ’Œ

	0,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	0,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	

	0,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	0,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	
	GMD_DECO_USER_FLAG_SECOND_FRAME | GMD_DECO_USER_FLAG_SUB_MODEL | GMD_DECO_USER_FLAG_USE_LIGHT_2_SUB_MODEL,			//ZONEF ƒVƒƒƒbƒ^[	
	GMD_DECO_USER_FLAG_SECOND_FRAME | GMD_DECO_USER_FLAG_NO_MAIN_MODEL | GMD_DECO_USER_FLAG_SUB_MODEL | GMD_DECO_USER_FLAG_USE_LIGHT_2_SUB_MODEL,			//ZONEF ƒVƒƒƒbƒ^[	

	GMD_DECO_USER_FLAG_SUB_MODEL | GMD_DECO_USER_FLAG_USE_LIGHT_2_SUB_MODEL,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	GMD_DECO_USER_FLAG_NO_MAIN_MODEL | GMD_DECO_USER_FLAG_SUB_MODEL | GMD_DECO_USER_FLAG_USE_LIGHT_2_SUB_MODEL,			//ZONEF ƒVƒƒƒbƒ^[	
	GMD_DECO_USER_FLAG_NO_MAIN_MODEL | GMD_DECO_USER_FLAG_SUB_MODEL | GMD_DECO_USER_FLAG_USE_LIGHT_2_SUB_MODEL,			//ZONEF ƒVƒƒƒbƒ^[	

	GMD_DECO_USER_FLAG_SUB_MODEL | GMD_DECO_USER_FLAG_USE_LIGHT_2_SUB_MODEL,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	GMD_DECO_USER_FLAG_NO_MAIN_MODEL | GMD_DECO_USER_FLAG_SUB_MODEL | GMD_DECO_USER_FLAG_USE_LIGHT_2_SUB_MODEL,			//ZONEF ƒVƒƒƒbƒ^[	
	GMD_DECO_USER_FLAG_NO_MAIN_MODEL | GMD_DECO_USER_FLAG_SUB_MODEL | GMD_DECO_USER_FLAG_USE_LIGHT_2_SUB_MODEL,			//ZONEF ƒVƒƒƒbƒ^[	
	
	0,				//ZONE2 A•¨
	0,				//ZONE2 A•¨

	GMD_DECO_USER_FLAG_THIRD_FRAME | GMD_DECO_USER_FLAG_EFFECT_DUPLICATE_DRAW | GMD_DECO_USER_FLAG_NOCLIP,	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
	
	GMD_DECO_USER_FLAG_SUB_MODEL | GMD_DECO_USER_FLAG_USE_LIGHT_2_SUB_MODEL,			//ZONEF ƒVƒƒƒbƒ^[	
};


//OBJŠÖ”ƒƒCƒ“
static const GMF_DECO_OBJ_FUNC g_gm_deco_func_main[GMD_DECORATE_ID_MAX] = {
	NULL,	//•Ç
	NULL,	//•Ç
	NULL,	//•Ç
	NULL,	//•Ç
	NULL,	//•Ç
	NULL,	//•Ç
	
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E

	NULL,	//ƒOƒŒƒA
	NULL,	//ƒOƒŒƒA
	NULL,	//ƒOƒŒƒA

	NULL,	//‚Ğ‚Ü‚í‚è
	NULL,	//•Ç
	NULL,	//•Ç
	
	NULL,	//‘ê

	NULL,		//ƒAƒVƒnƒi
	NULL,		//ƒAƒVƒnƒi
	NULL,		//ƒAƒVƒnƒi
	NULL,		//‰Ô
	NULL,		//‰Ô
	NULL,		//–Ø
	
	NULL,	//‰œ‚Ì‘ê
	
	NULL,		// •Ç
	NULL,		// •Ç
	NULL,		// •Ç
	NULL,		// •Ç
	NULL,		// •Ç
	
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj

	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê

	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj

	NULL,	//ƒOƒŒƒA

	NULL,		//”r‰t‘•’ui¶j
	NULL,		//”r‰t‘•’uoŒûi¶j
	NULL,		//”r‰t‘•’ui‰Ej
	NULL,		//”r‰t‘•’uoŒûi‰Ej

	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ

	NULL,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	NULL,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	NULL,			//ZONE3 ˜XC
	NULL,			//ZONE3 •Ç‚ÌŠç

	NULL,			//ZONE3•¬o‚·…
	NULL,			//ZONE3•¬o‚·…
	NULL,			//ZONE3•¬o‚·…i‰œj
	NULL,			//ZONE3•¬o‚·…i‰œj

	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨

	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3A•¨i‘Oj
	NULL,				//ZONE3A•¨i‘Oj
	NULL,				//ZONE3•Çi‘Oj
	NULL,				//ZONE3•Çi‘Oj

	NULL,		//ZONE3ƒŒ[ƒ‹Šp
	NULL,		//ZONE3ƒŒ[ƒ‹Šp
	NULL,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	NULL,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p

	NULL,		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	NULL,		//ZONE3 ˜XC
	
	NULL,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	NULL,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	NULL,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	NULL,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	NULL,		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	NULL,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	NULL,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	NULL,		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œiã‰º”½“]j
	NULL,	//ZONE4•Ô—p’Œiã‰º”½“]j
	NULL,	//ZONE4•Ô—p’Œiã‰º”½“]j
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	NULL,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	NULL,	//ZONE4•Ô—p’Œi¶‰E”½“]j

	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	
	NULL,				//ZONE4 ’Œ

	NULL,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	NULL,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	

	NULL,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	NULL,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	

	gmDecoMainFuncMotionApplyCommonFrame,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoMainFuncMotionApplyCommonFrame,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	

	gmDecoMainFuncMotionApplyCommonFrame,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoMainFuncMotionApplyCommonFrame,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	
	NULL,				//ZONE2 A•¨
	NULL,				//ZONE2 A•¨

	gmDecoMainFuncEffectCheckCommonFrame,	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
	
	gmDecoMainFuncLoop,			//ZONEF ƒVƒƒƒbƒ^[	
};

//OBJŠÖ”ˆÚ“®
static const GMF_DECO_OBJ_FUNC g_gm_deco_func_move[GMD_DECORATE_ID_MAX] = {
	NULL,	//•Ç
	NULL,	//•Ç
	NULL,	//•Ç
	NULL,	//•Ç
	NULL,	//•Ç
	NULL,	//•Ç
	
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E

	NULL,	//ƒOƒŒƒA
	NULL,	//ƒOƒŒƒA
	NULL,	//ƒOƒŒƒA

	NULL,	//‚Ğ‚Ü‚í‚è
	NULL,	//•Ç
	NULL,	//•Ç
	
	NULL,	//‘ê

	NULL,		//ƒAƒVƒnƒi
	NULL,		//ƒAƒVƒnƒi
	NULL,		//ƒAƒVƒnƒi
	NULL,		//‰Ô
	NULL,		//‰Ô
	NULL,		//–Ø
	
	NULL,		//‰œ‚Ì‘ê
	
	NULL,		// •Ç
	NULL,		// •Ç
	NULL,		// •Ç
	NULL,		// •Ç
	NULL,		// •Ç
	
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj

	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê

	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj

	NULL,	//ƒOƒŒƒA

	NULL,		//”r‰t‘•’ui¶j
	NULL,		//”r‰t‘•’uoŒûi¶j
	NULL,		//”r‰t‘•’ui‰Ej
	NULL,		//”r‰t‘•’uoŒûi‰Ej

	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ

	NULL,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	NULL,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	NULL,			//ZONE3 ˜XC
	NULL,			//ZONE3 •Ç‚ÌŠç

	NULL,			//ZONE3•¬o‚·…
	NULL,			//ZONE3•¬o‚·…
	NULL,			//ZONE3•¬o‚·…i‰œj
	NULL,			//ZONE3•¬o‚·…i‰œj

	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨

	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3A•¨i‘Oj
	NULL,				//ZONE3A•¨i‘Oj
	NULL,				//ZONE3•Çi‘Oj
	NULL,				//ZONE3•Çi‘Oj

	NULL,		//ZONE3ƒŒ[ƒ‹Šp
	NULL,		//ZONE3ƒŒ[ƒ‹Šp
	NULL,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	NULL,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p

	NULL,		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	NULL,		//ZONE3 ˜XC
	
	NULL,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	NULL,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	NULL,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	NULL,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	NULL,		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	NULL,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	NULL,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	NULL,		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œiã‰º”½“]j
	NULL,	//ZONE4•Ô—p’Œiã‰º”½“]j
	NULL,	//ZONE4•Ô—p’Œiã‰º”½“]j
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	NULL,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	NULL,	//ZONE4•Ô—p’Œi¶‰E”½“]j

	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	
	NULL,				//ZONE4 ’Œ

	NULL,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	NULL,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	

	NULL,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	NULL,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	

	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	

	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	
	NULL,				//ZONE2 A•¨
	NULL,				//ZONE2 A•¨

	NULL,	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
};

//OBJŠÖ”‹éŒ`“o˜^
static const GMF_DECO_RECT_FUNC g_gm_deco_func_rect[GMD_DECORATE_ID_MAX] = {
	NULL,	//•Ç
	NULL,	//•Ç
	NULL,	//•Ç
	NULL,	//•Ç
	NULL,	//•Ç
	NULL,	//•Ç
	
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	NULL,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E

	NULL,	//ƒOƒŒƒA
	NULL,	//ƒOƒŒƒA
	NULL,	//ƒOƒŒƒA

	gmDecoRectFuncChangeDecreaseMotionSpeed,	//‚Ğ‚Ü‚í‚è
	NULL,	//•Ç
	NULL,	//•Ç
	
	NULL,	//‘ê

	NULL,		//ƒAƒVƒnƒi
	NULL,		//ƒAƒVƒnƒi
	NULL,		//ƒAƒVƒnƒi
	NULL,		//‰Ô
	NULL,		//‰Ô
	gmDecoRectFuncChangeMotionCount,		//–Ø
	
	NULL,		//‰œ‚Ì‘ê
	
	NULL,		// •Ç
	NULL,		// •Ç
	NULL,		// •Ç
	NULL,		// •Ç
	NULL,		// •Ç
	
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj
	NULL,		// •Çi‰œj

	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê
	NULL,		//‘ê

	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj
	NULL,		//‘êi‰œj

	NULL,	//ƒOƒŒƒA

	NULL,		//”r‰t‘•’ui¶j
	NULL,		//”r‰t‘•’uoŒûi¶j
	NULL,		//”r‰t‘•’ui‰Ej
	NULL,		//”r‰t‘•’uoŒûi‰Ej

	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ
	NULL,			//ZONE3Šâ

	NULL,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	NULL,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	NULL,			//ZONE3 ˜XC
	NULL,			//ZONE3 •Ç‚ÌŠç

	NULL,			//ZONE3•¬o‚·…
	NULL,			//ZONE3•¬o‚·…
	NULL,			//ZONE3•¬o‚·…i‰œj
	NULL,			//ZONE3•¬o‚·…i‰œj

	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨
	NULL,				//ZONE3A•¨

	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3Šâi‘Oj
	NULL,				//ZONE3A•¨i‘Oj
	NULL,				//ZONE3A•¨i‘Oj
	NULL,				//ZONE3•Çi‘Oj
	NULL,				//ZONE3•Çi‘Oj

	NULL,		//ZONE3ƒŒ[ƒ‹Šp
	NULL,		//ZONE3ƒŒ[ƒ‹Šp
	NULL,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	NULL,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p
	NULL,		//ZONE4•Ô—p

	NULL,		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	NULL,		//ZONE3 ˜XC
	
	NULL,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	NULL,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	NULL,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	NULL,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	NULL,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	gmDecoRectFuncChangeDecreaseMotionSpeed,		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	NULL,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	NULL,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	gmDecoRectFuncChangeMotionCount,		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œiã‰º”½“]j
	NULL,	//ZONE4•Ô—p’Œiã‰º”½“]j
	NULL,	//ZONE4•Ô—p’Œiã‰º”½“]j
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œ
	NULL,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	NULL,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	NULL,	//ZONE4•Ô—p’Œi¶‰E”½“]j

	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	NULL,		//ZONE4•Ô—pi‘Oj
	
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	
	NULL,				//ZONE4 ’Œ

	NULL,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	NULL,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	NULL,				//ZONE4 ’Œ
	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	NULL,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	

	NULL,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	NULL,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	
	gmDecoRectFuncChangeMotionCommonFrame,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoRectFuncChangeMotionCommonFrame,			//ZONEF ƒVƒƒƒbƒ^[	

	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	

	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
	
	NULL,				//ZONE2 A•¨
	NULL,				//ZONE2 A•¨

	NULL,	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg

	NULL,			//ZONEF ƒVƒƒƒbƒ^[	
};

//OBJŠÖ”o—Í
static const GMF_DECO_OBJ_FUNC g_gm_deco_func_out[GMD_DECORATE_ID_MAX] = {
	gmDecoDraw,	//•Ç
	gmDecoDraw,	//•Ç
	gmDecoDraw,	//•Ç
	gmDecoDraw,	//•Ç
	gmDecoDraw,	//•Ç
	gmDecoDraw,	//•Ç
	
	gmDecoDraw,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	gmDecoDraw,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	gmDecoDraw,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	gmDecoDraw,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	gmDecoDraw,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	gmDecoDraw,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	gmDecoDraw,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	gmDecoDraw,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E

	GmDecoGlareDraw,	//ƒOƒŒƒA
	GmDecoGlareDraw,	//ƒOƒŒƒA
	GmDecoGlareDraw,	//ƒOƒŒƒA

	gmDecoDraw,	//‚Ğ‚Ü‚í‚è
	gmDecoDraw,	//•Ç
	gmDecoDraw,	//•Ç
	
#if GMD_DECO_USE_DRAW_SERVER
	gmDecoSetDrawFall,	//‘ê
#else
	gmDecoDrawFallFront,	//‘ê
#endif // GMD_DECO_USE_DRAW_SERVER

	gmDecoDraw,		//ƒAƒVƒnƒi
	gmDecoDraw,		//ƒAƒVƒnƒi
	gmDecoDraw,		//ƒAƒVƒnƒi
	gmDecoDraw,		//‰Ô
	gmDecoDraw,		//‰Ô
	gmDecoDraw,		//–Ø
	
#if GMD_DECO_USE_DRAW_SERVER
	gmDecoSetDrawFall,	//‰œ‚Ì‘ê
#else
	gmDecoDrawFallBack,	//‰œ‚Ì‘ê
#endif // GMD_DECO_USE_DRAW_SERVER
	
	gmDecoDraw,		// •Ç
	gmDecoDraw,		// •Ç
	gmDecoDraw,		// •Ç
	gmDecoDraw,		// •Ç
	gmDecoDraw,		// •Ç
	
	gmDecoDraw,		// •Çi‰œj
	gmDecoDraw,		// •Çi‰œj
	gmDecoDraw,		// •Çi‰œj
	gmDecoDraw,		// •Çi‰œj
	gmDecoDraw,		// •Çi‰œj
	gmDecoDraw,		// •Çi‰œj
	gmDecoDraw,		// •Çi‰œj

#if GMD_DECO_USE_DRAW_SERVER
	gmDecoSetDrawFall,	//‘ê
	gmDecoSetDrawFall,	//‘ê
	gmDecoSetDrawFall,	//‘ê
#else
	gmDecoDrawFallFront,		//‘ê
	gmDecoDrawFallFront,		//‘ê
	gmDecoDrawFallFront,		//‘ê
#endif // GMD_DECO_USE_DRAW_SERVER
	gmDecoDrawFallFront,		//‘ê
	gmDecoDrawFallFront,		//‘ê
	gmDecoDrawFallFront,		//‘ê
	gmDecoDrawFallFront,		//‘ê

#if GMD_DECO_USE_DRAW_SERVER
	gmDecoSetDrawFall,	//‘êi‰œj
	gmDecoSetDrawFall,	//‘êi‰œj
	gmDecoSetDrawFall,	//‘êi‰œj
#else
	gmDecoDrawFallBack,		//‘êi‰œj
	gmDecoDrawFallBack,		//‘êi‰œj
	gmDecoDrawFallBack,		//‘êi‰œj
#endif // GMD_DECO_USE_DRAW_SERVER
	gmDecoDrawFallBack,		//‘êi‰œj
	gmDecoDrawFallBack,		//‘êi‰œj
	gmDecoDrawFallBack,		//‘êi‰œj
	gmDecoDrawFallBack,		//‘êi‰œj

	GmDecoGlareDraw,	//ƒOƒŒƒA

	gmDecoDraw,		//”r‰t‘•’ui¶j
	gmDecoDraw,		//”r‰t‘•’uoŒûi¶j
	gmDecoDraw,		//”r‰t‘•’ui‰Ej
	gmDecoDraw,		//”r‰t‘•’uoŒûi‰Ej

	gmDecoDraw,			//ZONE3Šâ
	gmDecoDraw,			//ZONE3Šâ
	gmDecoDraw,			//ZONE3Šâ
	gmDecoDraw,			//ZONE3Šâ
	gmDecoDraw,			//ZONE3Šâ
	gmDecoDraw,			//ZONE3Šâ
	gmDecoDraw,			//ZONE3Šâ

	NULL,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	NULL,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	NULL,			//ZONE3 ˜XC
	NULL,			//ZONE3 •Ç‚ÌŠç

	gmDecoDrawFallFront,	//ZONE3•¬o‚·…
	gmDecoDrawFallFront,	//ZONE3•¬o‚·…
	gmDecoDrawFallBack,		//ZONE3•¬o‚·…i‰œj
	gmDecoDrawFallBack,		//ZONE3•¬o‚·…i‰œj

	gmDecoDraw,				//ZONE3A•¨
	gmDecoDraw,				//ZONE3A•¨
	gmDecoDraw,				//ZONE3A•¨
	gmDecoDraw,				//ZONE3A•¨
	gmDecoDraw,				//ZONE3A•¨
	gmDecoDraw,				//ZONE3A•¨

	gmDecoDraw,				//ZONE3Šâi‘Oj
	gmDecoDraw,				//ZONE3Šâi‘Oj
	gmDecoDraw,				//ZONE3Šâi‘Oj
	gmDecoDraw,				//ZONE3Šâi‘Oj
	gmDecoDraw,				//ZONE3Šâi‘Oj
	gmDecoDraw,				//ZONE3A•¨i‘Oj
	gmDecoDraw,				//ZONE3A•¨i‘Oj
	gmDecoDraw,				//ZONE3•Çi‘Oj
	gmDecoDraw,				//ZONE3•Çi‘Oj

	gmDecoDraw,		//ZONE3ƒŒ[ƒ‹Šp
	gmDecoDraw,		//ZONE3ƒŒ[ƒ‹Šp
	gmDecoDraw,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	gmDecoDraw,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	gmDecoDraw,		//ZONE4•Ô—p
	gmDecoDraw,		//ZONE4•Ô—p
	gmDecoDraw,		//ZONE4•Ô—p
	gmDecoDraw,		//ZONE4•Ô—p
	gmDecoDraw,		//ZONE4•Ô—p
	gmDecoDraw,		//ZONE4•Ô—p

	NULL,			//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	NULL,			//ZONE3 ˜XC
	
	gmDecoDraw,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	gmDecoDraw,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	gmDecoDraw,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	gmDecoDraw,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	gmDecoDraw,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	gmDecoDraw,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	gmDecoDraw,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	gmDecoDraw,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	GmDecoGlareDraw,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	GmDecoGlareDraw,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	gmDecoDraw,		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	gmDecoDraw,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	gmDecoDraw,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	gmDecoDraw,		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	gmDecoDraw,	//ZONE4•Ô—p’Œ
	gmDecoDraw,	//ZONE4•Ô—p’Œ
	gmDecoDraw,	//ZONE4•Ô—p’Œ
	gmDecoDraw,	//ZONE4•Ô—p’Œiã‰º”½“]j
	gmDecoDraw,	//ZONE4•Ô—p’Œiã‰º”½“]j
	gmDecoDraw,	//ZONE4•Ô—p’Œiã‰º”½“]j
	gmDecoDraw,	//ZONE4•Ô—p’Œ
	gmDecoDraw,	//ZONE4•Ô—p’Œ
	gmDecoDraw,	//ZONE4•Ô—p’Œ
	gmDecoDraw,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	gmDecoDraw,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	gmDecoDraw,	//ZONE4•Ô—p’Œi¶‰E”½“]j

	gmDecoDraw,		//ZONE4•Ô—pi‘Oj
	gmDecoDraw,		//ZONE4•Ô—pi‘Oj
	gmDecoDraw,		//ZONE4•Ô—pi‘Oj
	gmDecoDraw,		//ZONE4•Ô—pi‘Oj
	gmDecoDraw,		//ZONE4•Ô—pi‘Oj
	gmDecoDraw,		//ZONE4•Ô—pi‘Oj
	
	gmDecoDraw,				//ZONE4 ’Œ
	gmDecoDraw,				//ZONE4 ’Œ
	gmDecoDraw,				//ZONE4 ’Œ
	gmDecoDraw,				//ZONE4 ’Œ
	gmDecoDraw,				//ZONE4 ’Œ
	gmDecoDraw,				//ZONE4 ’Œ
	gmDecoDraw,				//ZONE4 ’Œ
	
	gmDecoDraw,				//ZONE4 ’Œ

	gmDecoDraw,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	NULL,					//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	gmDecoDraw,				//ZONE4 ’Œ
	gmDecoDraw,				//ZONE4 ’Œ
	gmDecoDraw,				//ZONE4 ’Œ
	gmDecoDraw,				//ZONE4 ’Œ
	gmDecoDraw,				//ZONE4 ’Œ
	gmDecoDraw,				//ZONE4 ’Œ
	gmDecoDraw,				//ZONE4 ’Œ
	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	gmDecoDraw,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	gmDecoDraw,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	gmDecoDraw,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	

	gmDecoDraw,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	gmDecoDraw,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
#if GMD_DECO_TEST_FINAL_SHUTTER
	gmDecoDrawFinalShutter3Line,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoDrawFinalShutter3Line,			//ZONEF ƒVƒƒƒbƒ^[	

	gmDecoDrawFinalShutter3Line,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoDraw,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoDrawFinalShutter3Line,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoDrawFinalShutter3Line,			//ZONEF ƒVƒƒƒbƒ^[	

	gmDecoDrawFinalShutter5Line,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoDraw,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoDrawFinalShutter5Line,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoDrawFinalShutter5Line,			//ZONEF ƒVƒƒƒbƒ^[	
#else
	gmDecoDrawFinalShutter3Line,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoDrawFinalShutter3Line,			//ZONEF ƒVƒƒƒbƒ^[	

	gmDecoDrawFinalShutter3Line,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoDraw,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoDrawFinalShutter3Line,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoDrawFinalShutter3Line,			//ZONEF ƒVƒƒƒbƒ^[	

	gmDecoDrawFinalShutter5Line,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoDraw,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoDrawFinalShutter5Line,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoDrawFinalShutter5Line,			//ZONEF ƒVƒƒƒbƒ^[	
#endif	//
	
	gmDecoDraw,				//ZONE2 A•¨
	gmDecoDraw,				//ZONE2 A•¨

	NULL,	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg

	gmDecoDrawFinalShutter5Line,			//ZONEF ƒVƒƒƒbƒ^[	
};

//OBJŠÖ”ƒfƒXƒgƒ‰ƒNƒ^
static const GSF_TASK_DESTRUCTOR g_gm_deco_func_dest[GMD_DECORATE_ID_MAX] = {
	gmDecoTcbDest,	//•Ç
	gmDecoTcbDest,	//•Ç
	gmDecoTcbDest,	//•Ç
	gmDecoTcbDest,	//•Ç
	gmDecoTcbDest,	//•Ç
	gmDecoTcbDest,	//•Ç
	
	gmDecoTcbDest,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	gmDecoTcbDest,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	gmDecoTcbDest,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	gmDecoTcbDest,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	gmDecoTcbDest,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	gmDecoTcbDest,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	gmDecoTcbDest,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	gmDecoTcbDest,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E

	gmDecoTcbDest,	//ƒOƒŒƒA
	gmDecoTcbDest,	//ƒOƒŒƒA
	gmDecoTcbDest,	//ƒOƒŒƒA

	gmDecoTcbDest,	//‚Ğ‚Ü‚í‚è
	gmDecoTcbDest,	//•Ç
	gmDecoTcbDest,	//•Ç
	
	gmDecoTcbDest,	//‘ê

	gmDecoTcbDest,		//ƒAƒVƒnƒi
	gmDecoTcbDest,		//ƒAƒVƒnƒi
	gmDecoTcbDest,		//ƒAƒVƒnƒi
	gmDecoTcbDest,		//‰Ô
	gmDecoTcbDest,		//‰Ô
	gmDecoTcbDest,		//–Ø
	
	gmDecoTcbDest,	//‰œ‚Ì‘ê
	
	gmDecoTcbDest,		// •Ç
	gmDecoTcbDest,		// •Ç
	gmDecoTcbDest,		// •Ç
	gmDecoTcbDest,		// •Ç
	gmDecoTcbDest,		// •Ç
	
	gmDecoTcbDest,		// •Çi‰œj
	gmDecoTcbDest,		// •Çi‰œj
	gmDecoTcbDest,		// •Çi‰œj
	gmDecoTcbDest,		// •Çi‰œj
	gmDecoTcbDest,		// •Çi‰œj
	gmDecoTcbDest,		// •Çi‰œj
	gmDecoTcbDest,		// •Çi‰œj

	gmDecoTcbDest,		//‘ê
	gmDecoTcbDest,		//‘ê
	gmDecoTcbDest,		//‘ê
	gmDecoTcbDest,		//‘ê
	gmDecoTcbDest,		//‘ê
	gmDecoTcbDest,		//‘ê
	gmDecoTcbDest,		//‘ê

	gmDecoTcbDest,		//‘êi‰œj
	gmDecoTcbDest,		//‘êi‰œj
	gmDecoTcbDest,		//‘êi‰œj
	gmDecoTcbDest,		//‘êi‰œj
	gmDecoTcbDest,		//‘êi‰œj
	gmDecoTcbDest,		//‘êi‰œj
	gmDecoTcbDest,		//‘êi‰œj

	gmDecoTcbDest,		//ƒOƒŒƒA

	gmDecoTcbDest,		//”r‰t‘•’ui¶j
	gmDecoTcbDest,		//”r‰t‘•’uoŒûi¶j
	gmDecoTcbDest,		//”r‰t‘•’ui‰Ej
	gmDecoTcbDest,		//”r‰t‘•’uoŒûi‰Ej

	gmDecoTcbDest,			//ZONE3Šâ
	gmDecoTcbDest,			//ZONE3Šâ
	gmDecoTcbDest,			//ZONE3Šâ
	gmDecoTcbDest,			//ZONE3Šâ
	gmDecoTcbDest,			//ZONE3Šâ
	gmDecoTcbDest,			//ZONE3Šâ
	gmDecoTcbDest,			//ZONE3Šâ

	gmDecoTcbDest,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	gmDecoTcbDest,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	gmDecoTcbDest,			//ZONE3 ˜XC
	gmDecoTcbDest,			//ZONE3 •Ç‚ÌŠç

	gmDecoTcbDest,			//ZONE3•¬o‚·…
	gmDecoTcbDest,			//ZONE3•¬o‚·…
	gmDecoTcbDest,			//ZONE3•¬o‚·…i‰œj
	gmDecoTcbDest,			//ZONE3•¬o‚·…i‰œj

	gmDecoTcbDest,				//ZONE3A•¨
	gmDecoTcbDest,				//ZONE3A•¨
	gmDecoTcbDest,				//ZONE3A•¨
	gmDecoTcbDest,				//ZONE3A•¨
	gmDecoTcbDest,				//ZONE3A•¨
	gmDecoTcbDest,				//ZONE3A•¨

	gmDecoTcbDest,				//ZONE3Šâi‘Oj
	gmDecoTcbDest,				//ZONE3Šâi‘Oj
	gmDecoTcbDest,				//ZONE3Šâi‘Oj
	gmDecoTcbDest,				//ZONE3Šâi‘Oj
	gmDecoTcbDest,				//ZONE3Šâi‘Oj
	gmDecoTcbDest,				//ZONE3A•¨i‘Oj
	gmDecoTcbDest,				//ZONE3A•¨i‘Oj
	gmDecoTcbDest,				//ZONE3•Çi‘Oj
	gmDecoTcbDest,				//ZONE3•Çi‘Oj

	gmDecoTcbDest,		//ZONE3ƒŒ[ƒ‹Šp
	gmDecoTcbDest,		//ZONE3ƒŒ[ƒ‹Šp
	gmDecoTcbDest,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	gmDecoTcbDest,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	gmDecoTcbDest,		//ZONE4•Ô—p
	gmDecoTcbDest,		//ZONE4•Ô—p
	gmDecoTcbDest,		//ZONE4•Ô—p
	gmDecoTcbDest,		//ZONE4•Ô—p
	gmDecoTcbDest,		//ZONE4•Ô—p
	gmDecoTcbDest,		//ZONE4•Ô—p

	gmDecoTcbDest,		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	gmDecoTcbDest,		//ZONE3 ˜XC
	
	gmDecoTcbDest,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	gmDecoTcbDest,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	gmDecoTcbDest,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	gmDecoTcbDest,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	gmDecoTcbDest,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	gmDecoTcbDest,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	gmDecoTcbDest,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	gmDecoTcbDest,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	gmDecoTcbDest,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	gmDecoTcbDest,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	gmDecoTcbDest,		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	gmDecoTcbDest,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	gmDecoTcbDest,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	gmDecoTcbDest,		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	gmDecoTcbDest,	//ZONE4•Ô—p’Œ
	gmDecoTcbDest,	//ZONE4•Ô—p’Œ
	gmDecoTcbDest,	//ZONE4•Ô—p’Œ
	gmDecoTcbDest,	//ZONE4•Ô—p’Œiã‰º”½“]j
	gmDecoTcbDest,	//ZONE4•Ô—p’Œiã‰º”½“]j
	gmDecoTcbDest,	//ZONE4•Ô—p’Œiã‰º”½“]j
	gmDecoTcbDest,	//ZONE4•Ô—p’Œ
	gmDecoTcbDest,	//ZONE4•Ô—p’Œ
	gmDecoTcbDest,	//ZONE4•Ô—p’Œ
	gmDecoTcbDest,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	gmDecoTcbDest,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	gmDecoTcbDest,	//ZONE4•Ô—p’Œi¶‰E”½“]j

	gmDecoTcbDest,		//ZONE4•Ô—pi‘Oj
	gmDecoTcbDest,		//ZONE4•Ô—pi‘Oj
	gmDecoTcbDest,		//ZONE4•Ô—pi‘Oj
	gmDecoTcbDest,		//ZONE4•Ô—pi‘Oj
	gmDecoTcbDest,		//ZONE4•Ô—pi‘Oj
	gmDecoTcbDest,		//ZONE4•Ô—pi‘Oj
	
	gmDecoTcbDest,				//ZONE4 ’Œ
	gmDecoTcbDest,				//ZONE4 ’Œ
	gmDecoTcbDest,				//ZONE4 ’Œ
	gmDecoTcbDest,				//ZONE4 ’Œ
	gmDecoTcbDest,				//ZONE4 ’Œ
	gmDecoTcbDest,				//ZONE4 ’Œ
	gmDecoTcbDest,				//ZONE4 ’Œ
	
	gmDecoTcbDest,				//ZONE4 ’Œ

	gmDecoTcbDest,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	gmDecoTcbDest,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	gmDecoTcbDest,				//ZONE4 ’Œ
	gmDecoTcbDest,				//ZONE4 ’Œ
	gmDecoTcbDest,				//ZONE4 ’Œ
	gmDecoTcbDest,				//ZONE4 ’Œ
	gmDecoTcbDest,				//ZONE4 ’Œ
	gmDecoTcbDest,				//ZONE4 ’Œ
	gmDecoTcbDest,				//ZONE4 ’Œ
	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	gmDecoTcbDest,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	gmDecoTcbDest,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	gmDecoTcbDest,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu

	gmDecoTcbDest,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	gmDecoTcbDest,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	
	gmDecoTcbDest,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoTcbDest,			//ZONEF ƒVƒƒƒbƒ^[	

	gmDecoTcbDest,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoTcbDest,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoTcbDest,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoTcbDest,			//ZONEF ƒVƒƒƒbƒ^[	

	gmDecoTcbDest,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoTcbDest,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoTcbDest,			//ZONEF ƒVƒƒƒbƒ^[	
	gmDecoTcbDest,			//ZONEF ƒVƒƒƒbƒ^[	
	
	gmDecoTcbDest,				//ZONE2 A•¨
	gmDecoTcbDest,				//ZONE2 A•¨

	gmDecoTcbDest,	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
	
	gmDecoTcbDest,			//ZONEF ƒVƒƒƒbƒ^[	
};

//À•WˆÊ’u
static const fx32 g_gm_deco_pos[GMD_DECORATE_ID_MAX][MTD_XYZ] = {
	{0,0,GMD_OBJ_GIMMICK_POS_Z_FRONT},	//•Ç
	{0,0,GMD_OBJ_GIMMICK_POS_Z_FRONT},	//•Ç
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//•Ç
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//•Ç
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},	//•Ç
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},	//•Ç
	
	{0,0,GMD_OBJ_GIMMICK_POS_Z_FRONT+FX32_ONE},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	{0,0,GMD_OBJ_GIMMICK_POS_Z_FRONT+FX32_ONE},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	{0,0,GMD_OBJ_GIMMICK_POS_Z_FRONT+FX32_ONE},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	{0,0,GMD_OBJ_GIMMICK_POS_Z_FRONT+FX32_ONE},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	{0,0,GMD_OBJ_GIMMICK_POS_Z_FRONT+FX32_ONE},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	{0,0,GMD_OBJ_GIMMICK_POS_Z_FRONT+FX32_ONE},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	{0,0,GMD_OBJ_GIMMICK_POS_Z_FRONT+FX32_ONE},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	{0,0,GMD_OBJ_GIMMICK_POS_Z_FRONT+FX32_ONE},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E

	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},	//ƒOƒŒƒA
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},	//ƒOƒŒƒA
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},	//ƒOƒŒƒA

	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//‚Ğ‚Ü‚í‚è
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},	//•Ç
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},	//•Ç
	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},	//‘ê

	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		//ƒAƒVƒnƒi
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		//ƒAƒVƒnƒi
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		//ƒAƒVƒnƒi
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		//‰Ô
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		//‰Ô
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		//–Ø
	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},	//‰œ‚Ì‘ê
	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		// •Ç
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		// •Ç
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		// •Ç
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		// •Ç
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		// •Ç
	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		// •Çi‰œj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		// •Çi‰œj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		// •Çi‰œj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		// •Çi‰œj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		// •Çi‰œj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		// •Çi‰œj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		// •Çi‰œj

	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		//‘ê
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		//‘ê
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		//‘ê
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		//‘ê
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		//‘ê
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		//‘ê
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		//‘ê

	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},		//‘êi‰œj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},		//‘êi‰œj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},		//‘êi‰œj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},		//‘êi‰œj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},		//‘êi‰œj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},		//‘êi‰œj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},		//‘êi‰œj

	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},	//ƒOƒŒƒA

	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_FRONT+32*FX32_ONE*2},		//”r‰t‘•’ui¶j
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_FRONT+32*FX32_ONE*2},		//”r‰t‘•’uoŒûi¶j
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_FRONT+32*FX32_ONE*2},		//”r‰t‘•’ui‰Ej
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_FRONT+32*FX32_ONE*2},		//”r‰t‘•’uoŒûi‰Ej

	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},			//ZONE3Šâ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},			//ZONE3Šâ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},			//ZONE3Šâ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},			//ZONE3Šâ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},			//ZONE3Šâ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},			//ZONE3Šâ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},			//ZONE3Šâ

	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	{0,0,GMD_OBJ_GIMMICK_POS_Z_FRONT+32*FX32_ONE},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	{32*FX32_ONE,65*FX32_ONE,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE3 ˜XC
	{64*FX32_ONE,40*FX32_ONE,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE3 •Ç‚ÌŠç

	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE3•¬o‚·…
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE3•¬o‚·…
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK-FX32_ONE},			//ZONE3•¬o‚·…i‰œj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK-FX32_ONE},			//ZONE3•¬o‚·…i‰œj

	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},			//ZONE3A•¨
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},			//ZONE3A•¨
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},			//ZONE3A•¨
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_BACK},			//ZONEA•¨
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE3A•¨
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONEA•¨

	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},				//ZONE3Šâi‘Oj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},				//ZONE3Šâi‘Oj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},				//ZONE3Šâi‘Oj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},				//ZONE3Šâi‘Oj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},				//ZONE3Šâi‘Oj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},				//ZONE3A•¨i‘Oj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},				//ZONE3A•¨i‘Oj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},				//ZONE3•Çi‘Oj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},				//ZONE3•Çi‘Oj

	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE3ƒŒ[ƒ‹Šp
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE3ƒŒ[ƒ‹Šp
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},		//ZONE4•Ô—p
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},		//ZONE4•Ô—p
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},		//ZONE4•Ô—p
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},		//ZONE4•Ô—p
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},		//ZONE4•Ô—p
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},		//ZONE4•Ô—p

	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK+16*FX32_ONE},		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	{32*FX32_ONE,32*FX32_ONE,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE3 ˜XC
	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_BACK},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE4•Ô—p’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE4•Ô—p’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE4•Ô—p’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE4•Ô—p’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE4•Ô—p’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE4•Ô—p’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE4•Ô—p’Œi¶‰E”½“]j
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE4•Ô—p’Œi¶‰E”½“]j
	{0,0,GMD_OBJ_DEFAULT_POS_Z_B_FRONT},	//ZONE4•Ô—p’Œi¶‰E”½“]j

	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE4•Ô—pi‘Oj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE4•Ô—pi‘Oj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE4•Ô—pi‘Oj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE4•Ô—pi‘Oj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE4•Ô—pi‘Oj
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},		//ZONE4•Ô—pi‘Oj
	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE4 ’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE4 ’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE4 ’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE4 ’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE4 ’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE4 ’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE4 ’Œ
	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A + FX32_ONE},	//ZONE4 ’Œ

	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},	//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	{32*FX32_ONE,6*FX32_ONE,GMD_OBJ_DEFAULT_POS_Z_A + 37*FX32_ONE},	//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE4 ’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE4 ’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE4 ’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE4 ’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE4 ’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE4 ’Œ
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE4 ’Œ
	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT - 24*FX32_ONE},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu

	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_N_BACK},			//ZONEF •‚“‡—pƒŒ[ƒ‹
	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_M_BACK + GMD_MAP_ADJUST_POS_Z_FINAL_M*FX32_ONE},			//ZONEF ƒVƒƒƒbƒ^[	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_M_BACK + GMD_MAP_ADJUST_POS_Z_FINAL_M*FX32_ONE},			//ZONEF ƒVƒƒƒbƒ^[

	{0,0,GMD_OBJ_DEFAULT_POS_Z_M_BACK + GMD_MAP_ADJUST_POS_Z_FINAL_M*FX32_ONE},			//ZONEF ƒVƒƒƒbƒ^[	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_M_BACK + GMD_MAP_ADJUST_POS_Z_FINAL_M*FX32_ONE},			//ZONEF ƒVƒƒƒbƒ^[	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_M_BACK + GMD_MAP_ADJUST_POS_Z_FINAL_M*FX32_ONE},			//ZONEF ƒVƒƒƒbƒ^[	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_M_BACK + GMD_MAP_ADJUST_POS_Z_FINAL_M*FX32_ONE},			//ZONEF ƒVƒƒƒbƒ^[	

	{0,0,GMD_OBJ_DEFAULT_POS_Z_M_BACK + GMD_MAP_ADJUST_POS_Z_FINAL_M*FX32_ONE},			//ZONEF ƒVƒƒƒbƒ^[	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_M_BACK + GMD_MAP_ADJUST_POS_Z_FINAL_M*FX32_ONE},			//ZONEF ƒVƒƒƒbƒ^[	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_M_BACK + GMD_MAP_ADJUST_POS_Z_FINAL_M*FX32_ONE},			//ZONEF ƒVƒƒƒbƒ^[	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_M_BACK + GMD_MAP_ADJUST_POS_Z_FINAL_M*FX32_ONE},			//ZONEF ƒVƒƒƒbƒ^[	
	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE2 A•¨
	{0,0,GMD_OBJ_DEFAULT_POS_Z_A_FRONT},				//ZONE2 A•¨

	{47*FX32_ONE,12*FX32_ONE,GMD_OBJ_DEFAULT_POS_Z_M_BACK},	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
	
	{0,0,GMD_OBJ_DEFAULT_POS_Z_M_BACK + GMD_MAP_ADJUST_POS_Z_FINAL_M*FX32_ONE},			//ZONEF ƒVƒƒƒbƒ^[	
};

//Z²‰ñ“]
static const u16 g_gm_deco_rot_z[GMD_DECORATE_ID_MAX] = {
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	0,	//•Ç
	
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	0,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E

	0,	//ƒOƒŒƒA
	0,	//ƒOƒŒƒA
	0,	//ƒOƒŒƒA

	0,	//‚Ğ‚Ü‚í‚è
	0,	//•Ç
	0,	//•Ç
	
	0,	//‘ê

	0,		//ƒAƒVƒnƒi
	0,		//ƒAƒVƒnƒi
	0,		//ƒAƒVƒnƒi
	0,		//‰Ô
	0,		//‰Ô
	0,		//–Ø
	
	0,	//‰œ‚Ì‘ê
	
	0,		// •Ç
	0,		// •Ç
	0,		// •Ç
	0,		// •Ç
	0,		// •Ç
	
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj
	0,		// •Çi‰œj

	0,		//‘ê
	0,		//‘ê
	0,		//‘ê
	0,		//‘ê
	0,		//‘ê
	0,		//‘ê
	0,		//‘ê

	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj
	0,		//‘êi‰œj

	0,		//ƒOƒŒƒA

	0,		//”r‰t‘•’ui¶j
	0,		//”r‰t‘•’uoŒûi¶j
	0,		//”r‰t‘•’ui‰Ej
	0,		//”r‰t‘•’uoŒûi‰Ej

	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ
	0,			//ZONE3Šâ

	0,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	0,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	0,			//ZONE3 ˜XC
	0,			//ZONE3 •Ç‚ÌŠç

	0,			//ZONE3•¬o‚·…
	0,			//ZONE3•¬o‚·…
	0,			//ZONE3•¬o‚·…i‰œj
	0,			//ZONE3•¬o‚·…i‰œj

	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨
	0,				//ZONE3A•¨

	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3Šâi‘Oj
	0,				//ZONE3A•¨i‘Oj
	0,				//ZONE3A•¨i‘Oj
	0,				//ZONE3•Çi‘Oj
	0,				//ZONE3•Çi‘Oj

	0,		//ZONE3ƒŒ[ƒ‹Šp
	0,		//ZONE3ƒŒ[ƒ‹Šp
	0,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	0,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p
	0,		//ZONE4•Ô—p

	0,		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	0,		//ZONE3 ˜XC
	
	0,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	0,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	0,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	0,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	0,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	0,		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	0,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	0,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	0,		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œiã‰º”½“]j
	0,	//ZONE4•Ô—p’Œiã‰º”½“]j
	0,	//ZONE4•Ô—p’Œiã‰º”½“]j
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œ
	0,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	0,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	0,	//ZONE4•Ô—p’Œi¶‰E”½“]j

	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	0,		//ZONE4•Ô—pi‘Oj
	
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	
	0,				//ZONE4 ’Œ

	0,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	0,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	0,				//ZONE4 ’Œ
	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	0,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu

	0,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	0,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	

	0,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	

	0,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
	
	0,				//ZONE2 A•¨
	0,				//ZONE2 A•¨

	0,	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
	
	0,			//ZONEF ƒVƒƒƒbƒ^[	
};

//‹éŒ`ƒTƒCƒY
static const s16 g_gm_deco_rect_size[GMD_DECORATE_ID_MAX][MTD_XY] = {
	{0,0},	//•Ç
	{0,0},	//•Ç
	{0,0},	//•Ç
	{0,0},	//•Ç
	{0,0},	//•Ç
	{0,0},	//•Ç
	
	{0,0},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	{0,0},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	{0,0},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	{0,0},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	{0,0},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	{0,0},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	{0,0},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	{0,0},	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E

	{0,0},	//ƒOƒŒƒA
	{0,0},	//ƒOƒŒƒA
	{0,0},	//ƒOƒŒƒA

	{16,128},	//‚Ğ‚Ü‚í‚è
	{0,0},	//•Ç
	{0,0},	//•Ç
	
	{0,0},	//‘ê

	{0,0},		//ƒAƒVƒnƒi
	{0,0},		//ƒAƒVƒnƒi
	{0,0},		//ƒAƒVƒnƒi
	{0,0},		//‰Ô
	{0,0},		//‰Ô
	{32,32},		//–Ø
	
	{0,0},	//‰œ‚Ì‘ê
	
	{0,0},		// •Ç
	{0,0},		// •Ç
	{0,0},		// •Ç
	{0,0},		// •Ç
	{0,0},		// •Ç
	
	{0,0},		// •Çi‰œj
	{0,0},		// •Çi‰œj
	{0,0},		// •Çi‰œj
	{0,0},		// •Çi‰œj
	{0,0},		// •Çi‰œj
	{0,0},		// •Çi‰œj
	{0,0},		// •Çi‰œj

	{0,0},		//‘ê
	{0,0},		//‘ê
	{0,0},		//‘ê
	{0,0},		//‘ê
	{0,0},		//‘ê
	{0,0},		//‘ê
	{0,0},		//‘ê

	{0,0},		//‘êi‰œj
	{0,0},		//‘êi‰œj
	{0,0},		//‘êi‰œj
	{0,0},		//‘êi‰œj
	{0,0},		//‘êi‰œj
	{0,0},		//‘êi‰œj
	{0,0},		//‘êi‰œj

	{0,0},	//ƒOƒŒƒA

	{0,0},		//”r‰t‘•’ui¶j
	{0,0},		//”r‰t‘•’uoŒûi¶j
	{0,0},		//”r‰t‘•’ui‰Ej
	{0,0},		//”r‰t‘•’uoŒûi‰Ej

	{0,0},			//ZONE3Šâ
	{0,0},			//ZONE3Šâ
	{0,0},			//ZONE3Šâ
	{0,0},			//ZONE3Šâ
	{0,0},			//ZONE3Šâ
	{0,0},			//ZONE3Šâ
	{0,0},			//ZONE3Šâ

	{0,0},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	{0,0},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	{0,0},	//ZONE3 ˜XC
	{0,0},	//ZONE3 •Ç‚ÌŠç

	{0,0},		//ZONE3•¬o‚·…
	{0,0},		//ZONE3•¬o‚·…
	{0,0},			//ZONE3•¬o‚·…i‰œj
	{0,0},			//ZONE3•¬o‚·…i‰œj

	{0,0},			//ZONE3A•¨
	{0,0},			//ZONE3A•¨
	{0,0},			//ZONE3A•¨
	{0,0},			//ZONEA•¨
	{0,0},		//ZONE3A•¨
	{0,0},		//ZONEA•¨

	{0,0},				//ZONE3Šâi‘Oj
	{0,0},				//ZONE3Šâi‘Oj
	{0,0},				//ZONE3Šâi‘Oj
	{0,0},				//ZONE3Šâi‘Oj
	{0,0},				//ZONE3Šâi‘Oj
	{0,0},				//ZONE3A•¨i‘Oj
	{0,0},				//ZONE3A•¨i‘Oj
	{0,0},				//ZONE3•Çi‘Oj
	{0,0},				//ZONE3•Çi‘Oj

	{0,0},		//ZONE3ƒŒ[ƒ‹Šp
	{0,0},		//ZONE3ƒŒ[ƒ‹Šp
	{0,0},		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	{0,0},		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	{0,0},		//ZONE4•Ô—p
	{0,0},		//ZONE4•Ô—p
	{0,0},		//ZONE4•Ô—p
	{0,0},		//ZONE4•Ô—p
	{0,0},		//ZONE4•Ô—p
	{0,0},		//ZONE4•Ô—p

	{0,0},		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	{0,0},	//ZONE3 ˜XC
	
	{0,0},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	{0,0},			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	{0,0},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{0,0},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{0,0},		//ZONE3 ‘åŠâƒŒ[ƒ‹
	{0,0},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	{0,0},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	{0,0},		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	{0,0},		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	{0,0},		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	{16,128},		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	{0,0},		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	{0,0},		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	{32,32},		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	{0,0},	//ZONE4•Ô—p’Œ
	{0,0},	//ZONE4•Ô—p’Œ
	{0,0},	//ZONE4•Ô—p’Œ
	{0,0},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{0,0},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{0,0},	//ZONE4•Ô—p’Œiã‰º”½“]j
	{0,0},	//ZONE4•Ô—p’Œ
	{0,0},	//ZONE4•Ô—p’Œ
	{0,0},	//ZONE4•Ô—p’Œ
	{0,0},	//ZONE4•Ô—p’Œi¶‰E”½“]j
	{0,0},	//ZONE4•Ô—p’Œi¶‰E”½“]j
	{0,0},	//ZONE4•Ô—p’Œi¶‰E”½“]j

	{0,0},		//ZONE4•Ô—pi‘Oj
	{0,0},		//ZONE4•Ô—pi‘Oj
	{0,0},		//ZONE4•Ô—pi‘Oj
	{0,0},		//ZONE4•Ô—pi‘Oj
	{0,0},		//ZONE4•Ô—pi‘Oj
	{0,0},		//ZONE4•Ô—pi‘Oj
	
	{0,0},				//ZONE4 ’Œ
	{0,0},				//ZONE4 ’Œ
	{0,0},				//ZONE4 ’Œ
	{0,0},				//ZONE4 ’Œ
	{0,0},				//ZONE4 ’Œ
	{0,0},				//ZONE4 ’Œ
	{0,0},				//ZONE4 ’Œ
	
	{0,0},				//ZONE4 ’Œ

	{0,0},				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	{0,0},				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	{0,0},				//ZONE4 ’Œ
	{0,0},				//ZONE4 ’Œ
	{0,0},				//ZONE4 ’Œ
	{0,0},				//ZONE4 ’Œ
	{0,0},				//ZONE4 ’Œ
	{0,0},				//ZONE4 ’Œ
	{0,0},				//ZONE4 ’Œ
	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{0,0},				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{0,0},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	{0,0},	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu

	{0,0},			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	{0,0},			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	
	{512,384},			//ZONEF ƒVƒƒƒbƒ^[	
	{512,384},			//ZONEF ƒVƒƒƒbƒ^[	

	{0,0},			//ZONEF ƒVƒƒƒbƒ^[	
	{0,0},			//ZONEF ƒVƒƒƒbƒ^[	
	{0,0},			//ZONEF ƒVƒƒƒbƒ^[	
	{0,0},			//ZONEF ƒVƒƒƒbƒ^[	

	{0,0},			//ZONEF ƒVƒƒƒbƒ^[	
	{0,0},			//ZONEF ƒVƒƒƒbƒ^[	
	{0,0},			//ZONEF ƒVƒƒƒbƒ^[	
	{0,0},			//ZONEF ƒVƒƒƒbƒ^[	
	
	{0,0},				//ZONE2 A•¨
	{0,0},				//ZONE2 A•¨

	{0,0},	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg

	{0,0},			//ZONEF ƒVƒƒƒbƒ^[	
};

//ƒRƒ}ƒ“ƒhƒXƒeƒCƒg
static const u32 g_gm_deco_command_state[GMD_DECORATE_ID_MAX] = {
#if _IPHONE
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//•Ç
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//•Ç
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//•Ç
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//•Ç
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//•Ç
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//•Ç
	
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E

	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ƒOƒŒƒA
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ƒOƒŒƒA
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ƒOƒŒƒA

	OBD_DRAW_CMD_STATE_3DNN_PRE,	//‚Ğ‚Ü‚í‚è
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//•Ç
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//•Ç
	
	OBD_DRAW_CMD_STATE_PRE_WATER,	//‘ê

	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ƒAƒVƒnƒi
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ƒAƒVƒnƒi
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ƒAƒVƒnƒi
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//‰Ô
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//‰Ô
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//–Ø
	
	OBD_DRAW_CMD_STATE_WATER_MAPMID,	//‰œ‚Ì‘ê
	
	OBD_DRAW_CMD_STATE_3DNN_PRE,		// •Ç
	OBD_DRAW_CMD_STATE_3DNN_PRE,		// •Ç
	OBD_DRAW_CMD_STATE_3DNN_PRE,		// •Ç
	OBD_DRAW_CMD_STATE_3DNN_PRE,		// •Ç
	OBD_DRAW_CMD_STATE_3DNN_PRE,		// •Ç
	
	OBD_DRAW_CMD_STATE_3DNN_PRE,		// •Çi‰œj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		// •Çi‰œj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		// •Çi‰œj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		// •Çi‰œj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		// •Çi‰œj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		// •Çi‰œj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		// •Çi‰œj

	OBD_DRAW_CMD_STATE_PRE_WATER,		//‘ê
	OBD_DRAW_CMD_STATE_PRE_WATER,		//‘ê
	OBD_DRAW_CMD_STATE_PRE_WATER,		//‘ê
	OBD_DRAW_CMD_STATE_PRE_WATER,		//‘ê
	OBD_DRAW_CMD_STATE_PRE_WATER,		//‘ê
	OBD_DRAW_CMD_STATE_PRE_WATER,		//‘ê
	OBD_DRAW_CMD_STATE_PRE_WATER,		//‘ê

	OBD_DRAW_CMD_STATE_WATER_MAPMID,		//‘êi‰œj
	OBD_DRAW_CMD_STATE_WATER_MAPMID,		//‘êi‰œj
	OBD_DRAW_CMD_STATE_WATER_MAPMID,		//‘êi‰œj
	OBD_DRAW_CMD_STATE_WATER_MAPMID,		//‘êi‰œj
	OBD_DRAW_CMD_STATE_WATER_MAPMID,		//‘êi‰œj
	OBD_DRAW_CMD_STATE_WATER_MAPMID,		//‘êi‰œj
	OBD_DRAW_CMD_STATE_WATER_MAPMID,		//‘êi‰œj

	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ƒOƒŒƒA

	OBD_DRAW_CMD_STATE_POST_WATER,		//”r‰t‘•’ui¶j
	OBD_DRAW_CMD_STATE_POST_WATER,		//”r‰t‘•’uoŒûi¶j
	OBD_DRAW_CMD_STATE_POST_WATER,		//”r‰t‘•’ui‰Ej
	OBD_DRAW_CMD_STATE_POST_WATER,		//”r‰t‘•’uoŒûi‰Ej

	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONE3Šâ
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONE3Šâ
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONE3Šâ
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONE3Šâ
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONE3Šâ
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONE3Šâ
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONE3Šâ

	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONE3 ˜XC
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONE3 •Ç‚ÌŠç

	OBD_DRAW_CMD_STATE_PRE_WATER,			//ZONE3•¬o‚·…
	OBD_DRAW_CMD_STATE_PRE_WATER,			//ZONE3•¬o‚·…
	OBD_DRAW_CMD_STATE_WATER_MAPMID,			//ZONE3•¬o‚·…i‰œj
	OBD_DRAW_CMD_STATE_WATER_MAPMID,			//ZONE3•¬o‚·…i‰œj

	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3A•¨
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3A•¨
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3A•¨
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3A•¨
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3A•¨
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3A•¨

	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3Šâi‘Oj
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3Šâi‘Oj
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3Šâi‘Oj
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3Šâi‘Oj
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3Šâi‘Oj
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3A•¨i‘Oj
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3A•¨i‘Oj
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3•Çi‘Oj
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE3•Çi‘Oj

	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE3ƒŒ[ƒ‹Šp
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE3ƒŒ[ƒ‹Šp
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE4•Ô—p
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE4•Ô—p
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE4•Ô—p
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE4•Ô—p
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE4•Ô—p
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE4•Ô—p

	OBD_DRAW_CMD_STATE_POST_WATER,		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE3 ˜XC
	
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	OBD_DRAW_CMD_STATE_3DNN_PRE,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONE4•Ô—p’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONE4•Ô—p’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONE4•Ô—p’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONE4•Ô—p’Œiã‰º”½“]j
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONE4•Ô—p’Œiã‰º”½“]j
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONE4•Ô—p’Œiã‰º”½“]j
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONE4•Ô—p’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONE4•Ô—p’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONE4•Ô—p’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONE4•Ô—p’Œi¶‰E”½“]j

	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE4•Ô—pi‘Oj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE4•Ô—pi‘Oj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE4•Ô—pi‘Oj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE4•Ô—pi‘Oj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE4•Ô—pi‘Oj
	OBD_DRAW_CMD_STATE_3DNN_PRE,		//ZONE4•Ô—pi‘Oj
	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ
	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ

	OBD_DRAW_CMD_STATE_3DNN_WS,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	OBD_DRAW_CMD_STATE_3DNN_WS,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ’Œ
	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DRAW_CMD_STATE_3DNN_PRE,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu

	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONEF ƒVƒƒƒbƒ^[	

	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONEF ƒVƒƒƒbƒ^[	

	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONEF ƒVƒƒƒbƒ^[	
	
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONE2 A•¨
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONE2 A•¨

	OBD_DRAW_CMD_STATE_3DNN_PRE,	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
	
	OBD_DRAW_CMD_STATE_3DNN_PRE,			//ZONEF ƒVƒƒƒbƒ^[	
#else
	OBD_DRAW_CMD_STATE_3DNN,	//•Ç
	OBD_DRAW_CMD_STATE_3DNN,	//•Ç
	OBD_DRAW_CMD_STATE_3DNN,	//•Ç
	OBD_DRAW_CMD_STATE_3DNN,	//•Ç
	OBD_DRAW_CMD_STATE_3DNN,	//•Ç
	OBD_DRAW_CMD_STATE_3DNN,	//•Ç
	
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	OBD_DRAW_CMD_STATE_3DNN,	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E

	OBD_DRAW_CMD_STATE_3DNN,	//ƒOƒŒƒA
	OBD_DRAW_CMD_STATE_3DNN,	//ƒOƒŒƒA
	OBD_DRAW_CMD_STATE_3DNN,	//ƒOƒŒƒA

	OBD_DRAW_CMD_STATE_3DNN,	//‚Ğ‚Ü‚í‚è
	OBD_DRAW_CMD_STATE_3DNN,	//•Ç
	OBD_DRAW_CMD_STATE_3DNN,	//•Ç
	
	OBD_DRAW_CMD_STATE_PRE_WATER,	//‘ê

	OBD_DRAW_CMD_STATE_3DNN,		//ƒAƒVƒnƒi
	OBD_DRAW_CMD_STATE_3DNN,		//ƒAƒVƒnƒi
	OBD_DRAW_CMD_STATE_3DNN,		//ƒAƒVƒnƒi
	OBD_DRAW_CMD_STATE_3DNN,		//‰Ô
	OBD_DRAW_CMD_STATE_3DNN,		//‰Ô
	OBD_DRAW_CMD_STATE_3DNN,		//–Ø
	
	OBD_DRAW_CMD_STATE_WATER_MAPMID,	//‰œ‚Ì‘ê
	
	OBD_DRAW_CMD_STATE_3DNN,		// •Ç
	OBD_DRAW_CMD_STATE_3DNN,		// •Ç
	OBD_DRAW_CMD_STATE_3DNN,		// •Ç
	OBD_DRAW_CMD_STATE_3DNN,		// •Ç
	OBD_DRAW_CMD_STATE_3DNN,		// •Ç
	
	OBD_DRAW_CMD_STATE_3DNN,		// •Çi‰œj
	OBD_DRAW_CMD_STATE_3DNN,		// •Çi‰œj
	OBD_DRAW_CMD_STATE_3DNN,		// •Çi‰œj
	OBD_DRAW_CMD_STATE_3DNN,		// •Çi‰œj
	OBD_DRAW_CMD_STATE_3DNN,		// •Çi‰œj
	OBD_DRAW_CMD_STATE_3DNN,		// •Çi‰œj
	OBD_DRAW_CMD_STATE_3DNN,		// •Çi‰œj

	OBD_DRAW_CMD_STATE_PRE_WATER,		//‘ê
	OBD_DRAW_CMD_STATE_PRE_WATER,		//‘ê
	OBD_DRAW_CMD_STATE_PRE_WATER,		//‘ê
	OBD_DRAW_CMD_STATE_PRE_WATER,		//‘ê
	OBD_DRAW_CMD_STATE_PRE_WATER,		//‘ê
	OBD_DRAW_CMD_STATE_PRE_WATER,		//‘ê
	OBD_DRAW_CMD_STATE_PRE_WATER,		//‘ê

	OBD_DRAW_CMD_STATE_WATER_MAPMID,		//‘êi‰œj
	OBD_DRAW_CMD_STATE_WATER_MAPMID,		//‘êi‰œj
	OBD_DRAW_CMD_STATE_WATER_MAPMID,		//‘êi‰œj
	OBD_DRAW_CMD_STATE_WATER_MAPMID,		//‘êi‰œj
	OBD_DRAW_CMD_STATE_WATER_MAPMID,		//‘êi‰œj
	OBD_DRAW_CMD_STATE_WATER_MAPMID,		//‘êi‰œj
	OBD_DRAW_CMD_STATE_WATER_MAPMID,		//‘êi‰œj

	OBD_DRAW_CMD_STATE_3DNN,	//ƒOƒŒƒA

	OBD_DRAW_CMD_STATE_POST_WATER,		//”r‰t‘•’ui¶j
	OBD_DRAW_CMD_STATE_POST_WATER,		//”r‰t‘•’uoŒûi¶j
	OBD_DRAW_CMD_STATE_POST_WATER,		//”r‰t‘•’ui‰Ej
	OBD_DRAW_CMD_STATE_POST_WATER,		//”r‰t‘•’uoŒûi‰Ej

	OBD_DRAW_CMD_STATE_3DNN,			//ZONE3Šâ
	OBD_DRAW_CMD_STATE_3DNN,			//ZONE3Šâ
	OBD_DRAW_CMD_STATE_3DNN,			//ZONE3Šâ
	OBD_DRAW_CMD_STATE_3DNN,			//ZONE3Šâ
	OBD_DRAW_CMD_STATE_3DNN,			//ZONE3Šâ
	OBD_DRAW_CMD_STATE_3DNN,			//ZONE3Šâ
	OBD_DRAW_CMD_STATE_3DNN,			//ZONE3Šâ

	OBD_DRAW_CMD_STATE_3DNN,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	OBD_DRAW_CMD_STATE_3DNN,			//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	OBD_DRAW_CMD_STATE_3DNN,			//ZONE3 ˜XC
	OBD_DRAW_CMD_STATE_3DNN,			//ZONE3 •Ç‚ÌŠç

	OBD_DRAW_CMD_STATE_PRE_WATER,			//ZONE3•¬o‚·…
	OBD_DRAW_CMD_STATE_PRE_WATER,			//ZONE3•¬o‚·…
	OBD_DRAW_CMD_STATE_WATER_MAPMID,			//ZONE3•¬o‚·…i‰œj
	OBD_DRAW_CMD_STATE_WATER_MAPMID,			//ZONE3•¬o‚·…i‰œj

	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3A•¨
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3A•¨
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3A•¨
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3A•¨
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3A•¨
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3A•¨

	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3Šâi‘Oj
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3Šâi‘Oj
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3Šâi‘Oj
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3Šâi‘Oj
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3Šâi‘Oj
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3A•¨i‘Oj
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3A•¨i‘Oj
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3•Çi‘Oj
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE3•Çi‘Oj

	OBD_DRAW_CMD_STATE_3DNN,		//ZONE3ƒŒ[ƒ‹Šp
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE3ƒŒ[ƒ‹Šp
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j

	OBD_DRAW_CMD_STATE_3DNN,		//ZONE4•Ô—p
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE4•Ô—p
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE4•Ô—p
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE4•Ô—p
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE4•Ô—p
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE4•Ô—p

	OBD_DRAW_CMD_STATE_POST_WATER,		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE3 ˜XC
	
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE3 ‘åŠâƒŒ[ƒ‹
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE3 ‘åŠâƒŒ[ƒ‹i¶‰E”½“]j

	OBD_DRAW_CMD_STATE_3DNN,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DRAW_CMD_STATE_3DNN,		// ƒOƒŒƒAiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DRAW_CMD_STATE_3DNN,		// ‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DRAW_CMD_STATE_3DNN,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DRAW_CMD_STATE_3DNN,		//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	OBD_DRAW_CMD_STATE_3DNN,		//–ØiƒGƒ“ƒfƒBƒ“ƒOj

	OBD_DRAW_CMD_STATE_3DNN,	//ZONE4•Ô—p’Œ
	OBD_DRAW_CMD_STATE_3DNN,	//ZONE4•Ô—p’Œ
	OBD_DRAW_CMD_STATE_3DNN,	//ZONE4•Ô—p’Œ
	OBD_DRAW_CMD_STATE_3DNN,	//ZONE4•Ô—p’Œiã‰º”½“]j
	OBD_DRAW_CMD_STATE_3DNN,	//ZONE4•Ô—p’Œiã‰º”½“]j
	OBD_DRAW_CMD_STATE_3DNN,	//ZONE4•Ô—p’Œiã‰º”½“]j
	OBD_DRAW_CMD_STATE_3DNN,	//ZONE4•Ô—p’Œ
	OBD_DRAW_CMD_STATE_3DNN,	//ZONE4•Ô—p’Œ
	OBD_DRAW_CMD_STATE_3DNN,	//ZONE4•Ô—p’Œ
	OBD_DRAW_CMD_STATE_3DNN,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	OBD_DRAW_CMD_STATE_3DNN,	//ZONE4•Ô—p’Œi¶‰E”½“]j
	OBD_DRAW_CMD_STATE_3DNN,	//ZONE4•Ô—p’Œi¶‰E”½“]j

	OBD_DRAW_CMD_STATE_3DNN,		//ZONE4•Ô—pi‘Oj
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE4•Ô—pi‘Oj
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE4•Ô—pi‘Oj
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE4•Ô—pi‘Oj
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE4•Ô—pi‘Oj
	OBD_DRAW_CMD_STATE_3DNN,		//ZONE4•Ô—pi‘Oj
	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ
	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ

	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ’Œ
	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp		
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DRAW_CMD_STATE_3DNN,				//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DRAW_CMD_STATE_3DNN,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu	
	OBD_DRAW_CMD_STATE_3DNN,	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu

	OBD_DRAW_CMD_STATE_3DNN,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	OBD_DRAW_CMD_STATE_3DNN,			//ZONEF •‚“‡—pƒŒ[ƒ‹	
	
	OBD_DRAW_CMD_STATE_3DNN,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DRAW_CMD_STATE_3DNN,			//ZONEF ƒVƒƒƒbƒ^[	

	OBD_DRAW_CMD_STATE_3DNN,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DRAW_CMD_STATE_3DNN,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DRAW_CMD_STATE_3DNN,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DRAW_CMD_STATE_3DNN,			//ZONEF ƒVƒƒƒbƒ^[	

	OBD_DRAW_CMD_STATE_3DNN,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DRAW_CMD_STATE_3DNN,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DRAW_CMD_STATE_3DNN,			//ZONEF ƒVƒƒƒbƒ^[	
	OBD_DRAW_CMD_STATE_3DNN,			//ZONEF ƒVƒƒƒbƒ^[	
	
	OBD_DRAW_CMD_STATE_3DNN,			//ZONE2 A•¨
	OBD_DRAW_CMD_STATE_3DNN,			//ZONE2 A•¨

	OBD_DRAW_CMD_STATE_3DNN,	//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
	
	OBD_DRAW_CMD_STATE_3DNN,			//ZONEF ƒVƒƒƒbƒ^[	
#endif // _IPHONE
};

//----- Global Functions ----------------------------------------------------

// ==========================================================================
//ƒf[ƒ^
// ==========================================================================


// ==========================================================================
// GmDecoInitData
/*!
 * ‘•üƒf[ƒ^‚ğ‰Šú‰»
 *
 * @param amb ambƒf[ƒ^ƒwƒbƒ_
 */
// ==========================================================================
void GmDecoInitData( AMS_AMB_HEADER* amb )
{
	amAssert( !g_deco_data );
	amAssert( amb );

	//ƒf[ƒ^ŠÇ—‰Šú‰»
	gmDecoDataInit();

	//ƒRƒ“ƒo[ƒg
	amBindConvertAll( (u8*)amb );

	//“o˜^
	gmDecoDataSetAmbHeader( amb );

#if _IPHONE
	//ƒvƒŠƒ~ƒeƒBƒu‰Šú‰»
	gmDecoInitDrawPrimitive();
#endif // _IPHONE
}

// ==========================================================================
// GmDecoBuildData
/*!
 * ‘•üƒf[ƒ^\’z
 */
// ==========================================================================
void GmDecoBuildData( void )
{

	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
#if !_IPHONE
	if ( GSD_MAIN_ZONE_TYPE_1 == zone_type 
		|| GSD_MAIN_ZONE_TYPE_2 == zone_type
		|| GSD_MAIN_ZONE_TYPE_3 == zone_type
		|| GSD_MAIN_ZONE_TYPE_4 == zone_type
		|| GSD_MAIN_ZONE_TYPE_FINAL == zone_type
	){
		GMS_DECO_DATA* deco_data = gmDecoDataGetInfo();
		amAssert( deco_data );
		AMS_AMB_HEADER* amb_header = gmDecoDataGetAmbHeader();
		amAssert( amb_header );

		AMS_AMB_HEADER* model_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_MDL );
		amAssert( model_amb );
		AMS_AMB_HEADER* texture_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_TEX );
		amAssert( texture_amb );
		deco_data->obj_3d_list = GmGameDBuildRegBuildModel( model_amb, texture_amb, (NNF_DRAWOBJ)0 );
	}

	//‘ê
#if GMD_DECO_TEST_FALL
	if ( GSD_MAIN_ZONE_TYPE_3 == zone_type 
		|| GSD_MAIN_ZONE_TYPE_1 == zone_type
	){
		GMS_DECO_DATA* deco_data = gmDecoDataGetInfo();
		amAssert( deco_data );
		AMS_AMB_HEADER* amb_header = gmDecoDataGetAmbHeader();
		amAssert( amb_header );

		AMS_AMB_HEADER* model_amb_fall = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_MDL_RENDER );
		amAssert( model_amb_fall );
		AMS_AMB_HEADER* texture_amb_fall = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_TEX_RENDER );
		amAssert( texture_amb_fall );

	#if !_WII
		NNF_DRAWOBJ nn_draw_flag = NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1;
	#else
		NNF_DRAWOBJ nn_draw_flag = (NNF_DRAWOBJ)0;
	#endif
		deco_data->obj_3d_list_fall = GmGameDBuildRegBuildModel( model_amb_fall, texture_amb_fall, nn_draw_flag );
	}
#endif	//GMD_DECO_TEST_FALL

	//ƒOƒŒƒA
	if ( GSD_MAIN_ZONE_TYPE_1 == zone_type ||
		 GSD_MAIN_ZONE_TYPE_2 == zone_type ||
		 GSD_MAIN_ZONE_TYPE_3 == zone_type){
		AMS_AMB_HEADER* amb_header = gmDecoDataGetAmbHeader();
		amAssert( amb_header );

		// ambƒf[ƒ^‚ğİ’èi“à•”‚ÅƒeƒNƒXƒ`ƒƒƒ[ƒh‚ğs‚¤j
		GmDecoGlareSetData( amb_header );
	}
#else // !_IPHONE
	GMS_DECO_DATA*  deco_data  = gmDecoDataGetInfo();
	AMS_AMB_HEADER* amb_header = NULL;
	
	// •K—v‚ÈZone‚Ì‚İamb—pˆÓ
	if (deco_data) {
		amb_header = gmDecoDataGetAmbHeader();
	}
#if 0
	// old
	if (GSD_MAIN_ZONE_TYPE_1 == zone_type) {
		amAssert(deco_data);
		amAssert(amb_header);
		//‘ê
		AMS_AMB_HEADER* model_amb_fall = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_MDL_RENDER );
		amAssert( model_amb_fall );
		AMS_AMB_HEADER* texture_amb_fall = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_TEX_RENDER );
		amAssert( texture_amb_fall );
		
		NNF_DRAWOBJ nn_draw_flag = (NNF_DRAWOBJ)0;

		deco_data->obj_3d_list_fall = GmGameDBuildRegBuildModel( model_amb_fall, texture_amb_fall, nn_draw_flag );
	}
#endif // 0
	// ’Êíƒ‚ƒfƒ‹ 
	if (gmDecoIsUseModel(GMD_DECO_USE_MODEL_TYPE_NORMAL)) {
		amAssert(deco_data);
		amAssert(amb_header);
		
		deco_data->tvx_model = (AMS_AMB_HEADER*)amBindGet(amb_header, GMD_DECO_DATA_INDEX_AMB_MDL);
		AoTexBuild(&deco_data->tvx_tex, amBindGet(amb_header, GMD_DECO_DATA_INDEX_AMB_TEX));
		AoTexLoad(&deco_data->tvx_tex);
		amAssert(deco_data->tvx_model);
	}
	// ƒŒƒ“ƒ_[ƒ‚ƒfƒ‹ (iPhone‚Å‚Í•`‰æ‡”Ô‚ªˆá‚¤‚­‚ç‚¢‚µ‚©“Áêˆ—‚È‚µ)
	if (gmDecoIsUseModel(GMD_DECO_USE_MODEL_TYPE_RENDER)) {
		amAssert(deco_data);
		amAssert(amb_header);
		
		//‘ê
		AMS_AMB_HEADER* model_amb_fall = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_MDL_RENDER );
		amAssert( model_amb_fall );
		AMS_AMB_HEADER* texture_amb_fall = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_TEX_RENDER );
		amAssert( texture_amb_fall );
		
		NNF_DRAWOBJ nn_draw_flag = (NNF_DRAWOBJ)0;
		
		deco_data->obj_3d_list_fall = GmGameDBuildRegBuildModel( model_amb_fall, texture_amb_fall, nn_draw_flag );
	}
#endif // !_IPHONE
}

// ==========================================================================
// GmDecoCheckLoading
/*!
 * “Ç‚İ‚İ‘Ò‚¿ƒ`ƒFƒbƒN
 *
 * @return TRUEF“Ç‚İ‚İI—¹ FALSEF“Ç‚İ‚İ‘Ò‚¿
 *
 */
// ==========================================================================
BOOL GmDecoCheckLoading( void )
{
#if _IPHONE
	GMS_DECO_DATA*  deco_data  = gmDecoDataGetInfo();
	
	// ’Êíƒ‚ƒfƒ‹
	if (gmDecoIsUseModel(GMD_DECO_USE_MODEL_TYPE_NORMAL)) {
		amAssert(deco_data);
		if (!AoTexIsLoaded(&deco_data->tvx_tex)) {
			return FALSE;
		}
	}
	// ƒŒƒ“ƒ_[ƒ‚ƒfƒ‹
	if (gmDecoIsUseModel(GMD_DECO_USE_MODEL_TYPE_RENDER)) {
		amAssert(deco_data);
	}
#endif // _IPHONE
	
	return TRUE;
}

// ==========================================================================
// GmDecoInit
/*!
 * ‘•ü‰Šú‰»
 */
// ==========================================================================
void GmDecoInit( void )
{
	amAssert(!g_deco_mgr);

	//ŠÇ—î•ñ‰Šú‰»
	gmDecoInitMgr();

#if GMD_DECO_USE_DRAW_SERVER
	/*
	MTM_ASSERT(!gm_deco_draw_server_tcb);
	gm_deco_draw_server_tcb	=
		MTM_TASK_MAKE_TCB(gmDecoDrawServerMain,
						  NULL,
						  0,	// flag
						  GMD_TASK_PAUSELEVEL_DEF,
						  GMD_TASK_PRIO_DECORATION + 0x10,
						  GMD_TASK_GROUP_DECO_SYS,
						  0,	// worksize
						  "DECO_DRAW_SERVER");
	*/
	amZeroMemory(g_deco_fall_manager, sizeof(GMS_DECO_FALL_MANAGER) * GMD_DECO_FALL_MANAGER_NUM);
#endif // GMD_DECO_USE_DRAW_SERVER
}

// ==========================================================================
// GmDecoExit
/*!
 * ‘•üI—¹
 */
// ==========================================================================
void GmDecoExit( void )
{
	//‘•üŠÇ—‰ğ•ú
	gmDecoExitMgr();

#if GMD_DECO_USE_DRAW_SERVER
//	MTM_ASSERT(gm_deco_draw_server_tcb);
	// ‘•üƒT[ƒo[’â~
//	mtTaskClearTcb(gm_deco_draw_server_tcb);
//	gm_deco_draw_server_tcb = NULL;
#endif // GMD_DECO_USE_DRAW_SERVER
}

// ==========================================================================
// GmDecoRelease
/*!
 * ‘•ü\’z‚µ‚½ƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
void GmDecoRelease( void )
{	
	//ƒOƒŒƒA
	GmDecoGlareDataRelease();

	//ƒf[ƒ^‰ğ•ú
	gmDecoDataRelease();

	//‘•üŠÇ—‰ğ•ú
	gmDecoReleaseMgr();
}

// ==========================================================================
// GmDecoFlushData
/*!
 * ‘•ü“Ç‚İ‚ñ‚¾ƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
void GmDecoFlushData( void )
{

	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
#if !_IPHONE
	if ( GSD_MAIN_ZONE_TYPE_1 == zone_type 
		|| GSD_MAIN_ZONE_TYPE_2 == zone_type
		|| GSD_MAIN_ZONE_TYPE_3 == zone_type
		|| GSD_MAIN_ZONE_TYPE_4 == zone_type
		|| GSD_MAIN_ZONE_TYPE_FINAL == zone_type
	){
		AMS_AMB_HEADER* amb_header = gmDecoDataGetAmbHeader();
		amAssert( amb_header );
		GMS_DECO_DATA* deco_data = gmDecoDataGetInfo();
		amAssert( deco_data );

		//ƒ‚ƒfƒ‹
		AMS_AMB_HEADER* model_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_MDL );
		amAssert( model_amb );
		GmGameDBuildRegFlushModel( deco_data->obj_3d_list, model_amb->file_num );
		deco_data->obj_3d_list = NULL;
	}

	//‘êƒ‚ƒfƒ‹
#if GMD_DECO_TEST_FALL
	if ( GSD_MAIN_ZONE_TYPE_3 == zone_type 
		|| GSD_MAIN_ZONE_TYPE_1 == zone_type
	){
		AMS_AMB_HEADER* amb_header = gmDecoDataGetAmbHeader();
		amAssert( amb_header );
		GMS_DECO_DATA* deco_data = gmDecoDataGetInfo();
		amAssert( deco_data );

		AMS_AMB_HEADER* model_fall_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_MDL_RENDER );
		amAssert( model_fall_amb );
		GmGameDBuildRegFlushModel( deco_data->obj_3d_list_fall, model_fall_amb->file_num );
		deco_data->obj_3d_list_fall = NULL;
	}
#endif	//GMD_DECO_TEST_FALL
#else // !_IPHONE
	GMS_DECO_DATA*  deco_data  = gmDecoDataGetInfo();
	AMS_AMB_HEADER* amb_header = NULL;
	
	// •K—v‚ÈZone‚Ì‚İamb—pˆÓ
	if (deco_data) {
		amb_header = gmDecoDataGetAmbHeader();
	}
#if 0
	if (GSD_MAIN_ZONE_TYPE_1 == zone_type) {
		amAssert(deco_data);
		amAssert(amb_header);
		//‘ê
		AMS_AMB_HEADER* model_fall_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_MDL_RENDER );
		amAssert( model_fall_amb );
		GmGameDBuildRegFlushModel( deco_data->obj_3d_list_fall, model_fall_amb->file_num );
		deco_data->obj_3d_list_fall = NULL;
	}
#endif
	// ’Êíƒ‚ƒfƒ‹ 
	if (gmDecoIsUseModel(GMD_DECO_USE_MODEL_TYPE_NORMAL)) {
		amAssert(deco_data);
		amAssert(amb_header);
		AoTexRelease(&deco_data->tvx_tex);
	}
	// ƒŒƒ“ƒ_[ƒ‚ƒfƒ‹ (iPhone‚Å‚Í•`‰æ‡”Ô‚ªˆá‚¤‚­‚ç‚¢‚µ‚©“Áêˆ—‚È‚µ)
	if (gmDecoIsUseModel(GMD_DECO_USE_MODEL_TYPE_RENDER)) {
		amAssert(deco_data);
		amAssert(amb_header);
		//‘ê
		AMS_AMB_HEADER* model_fall_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_MDL_RENDER );
		amAssert( model_fall_amb );
		GmGameDBuildRegFlushModel( deco_data->obj_3d_list_fall, model_fall_amb->file_num );
		deco_data->obj_3d_list_fall = NULL;
	}
#endif // !_IPHONE
}

#if _IPHONE
// ==========================================================================
// GmDecoCheckFlushing
/*!
 * ƒf[ƒ^ŠJ•ú‘Ò‚¿
 *
 * @return TRUEFŠJ•úI—¹ FALSEFŠJ•ú‘Ò‚¿
 *
 */
// ==========================================================================
BOOL GmDecoCheckFlushing( void )
{
	GMS_DECO_DATA*  deco_data  = gmDecoDataGetInfo();
	
	// ’Êíƒ‚ƒfƒ‹
	if (gmDecoIsUseModel(GMD_DECO_USE_MODEL_TYPE_NORMAL)) {
		amAssert(deco_data);
		if (!AoTexIsReleased(&deco_data->tvx_tex)) {
			return FALSE;
		}
	}
	
	return TRUE;
}
#endif // _IPHONE

// ==========================================================================
// ‘•üŠÇ—
// ==========================================================================


// ==========================================================================
// GmDecoInitModel
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmDecoInitModel( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
#if !_IPHONE || GMD_DECO_USE_DRAW_TVX
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_DECO_WORK* deco_work = gmDecoInitModel( dec_rec, x, y, type );

	return &deco_work->obj_work;
#else
	return NULL;
#endif
}

// ==========================================================================
// GmDecoInitModelMotion
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmDecoInitModelMotion( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
#if !_IPHONE
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_DECO_WORK* deco_work = gmDecoInitModelMotion( dec_rec, x, y, type );

	return &deco_work->obj_work;
#else
	return NULL;
#endif
}

// ==========================================================================
// GmDecoInitModelMotionTouch
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmDecoInitModelMotionTouch( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
#if !_IPHONE
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_DECO_WORK* deco_work = gmDecoInitModelMotionTouch( dec_rec, x, y, type );

	return &deco_work->obj_work;
#else //!_IPHONE
#if (GMD_DECO_USE_DRAW_TVX_NOMOTION & GMD_DECO_USE_DRAW_TVX)
	GMS_DECO_WORK* deco_work = gmDecoInitModel(dec_rec, x, y, type);
	
	return &deco_work->obj_work;
#else
	return NULL;
#endif // (GMD_DECO_USE_DRAW_TVX_NOMOTION & GMD_DECO_USE_DRAW_TVX)
#endif //!_IPHONE
}

// ==========================================================================
// GmDecoInitModelMaterial
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmDecoInitModelMaterial( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_DECO_WORK* deco_work = gmDecoInitModelMaterial( dec_rec, x, y, type );

	return &deco_work->obj_work;
}

// ==========================================================================
// GmDecoInitModelMotionMaterial
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmDecoInitModelMotionMaterial( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_DECO_WORK* deco_work = gmDecoInitModelMotioinMaterial( dec_rec, x, y, type );

	return &deco_work->obj_work;
}

// ==========================================================================
// GmDecoInitModelMotionMaterialTouch
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmDecoInitModelMotionMaterialTouch( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
#if !_IPHONE
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_DECO_WORK* deco_work = gmDecoInitModelMotionMaterialTouch( dec_rec, x, y, type );

	return &deco_work->obj_work;
#else
	return NULL;
#endif //!_IPHONE
}

// ==========================================================================
// GmDecoInitModelLoop
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmDecoInitModelLoop( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_DECO_WORK* deco_work = gmDecoInitModelLoop( dec_rec, x, y, type );

	return &deco_work->obj_work;
}

// ==========================================================================
// GmDecoInitModelEffect
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmDecoInitModelEffect( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_DECO_WORK* deco_work = gmDecoInitModelEffect( dec_rec, x, y, type );

	return &deco_work->obj_work;
}

// ==========================================================================
// GmDecoInitPrimitive3D
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmDecoInitPrimitive3D( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
#if !_IPHONE
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_DECO_WORK* deco_work = gmDecoInitPrimitive3D( dec_rec, x, y, type );

	return &deco_work->obj_work;
#else
	return NULL;
#endif //!_IPHONE
}

// ==========================================================================
// GmDecoInitFall
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmDecoInitFall( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
#if 0
	// b’è‘Î‰@‘•ü‚ªo‚¹‚é‚æ‚¤‚É‚È‚Á‚½‚çÁ‚·
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	if (zone_type != GSD_MAIN_ZONE_TYPE_1) {
		return NULL;
	}
#endif // 0
	
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_DECO_WORK* deco_work = gmDecoInitFall( dec_rec, x, y, type );
#if GMD_DECO_USE_DRAW_SERVER
	OBS_OBJECT_WORK *obj_work    = &deco_work->obj_work;
	OBS_ACTION3D_NN_WORK *obj_3d = &deco_work->obj_3d;
	float frame = amMotionMaterialGetEndFrame(obj_3d->motion, obj_3d->mat_act_id) - amMotionMaterialGetStartFrame(obj_3d->motion, obj_3d->mat_act_id);
	gmDecoAddFallEvent(dec_rec, obj_work->pos.x + obj_work->ofst.x, obj_3d->texlist, frame);
#endif // GMD_DECO_USE_DRAW_SERVER
	
	
	
	return &deco_work->obj_work;
}

// ==========================================================================
// GmDecoInitEffect
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmDecoInitEffect( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_DECO_WORK* deco_work = gmDecoInitEffect( dec_rec, x, y, type );

	return &deco_work->obj_work;
}

// ==========================================================================
// GmDecoInitEffectBlock
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmDecoInitEffectBlock( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_DECO_WORK* deco_work = gmDecoInitEffectBlock( dec_rec, x, y, type );

	return &deco_work->obj_work;
}

// ==========================================================================
// GmDecoInitEffectBlockAndNext
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
OBS_OBJECT_WORK* GmDecoInitEffectBlockAndNext( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	//ƒIƒuƒWƒFƒNƒgì¬
	GMS_DECO_WORK* deco_work = gmDecoInitEffectBlockAndNext( dec_rec, x, y, type );

	return &deco_work->obj_work;
}

// ==========================================================================
// GmDecoSetFrameMotion
/*!
 * ‹¤’Êƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€‚ğİ’è
 *
 * @param frame ƒtƒŒ[ƒ€
 * @param index ƒtƒŒ[ƒ€ƒCƒ“ƒfƒNƒX
 *
 */
// ==========================================================================
void GmDecoSetFrameMotion( s32 frame, s32 index )
{
	amAssert( index < GMD_DECO_COMMON_FRAME_INDEX_NUM );

	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( mgr ){
		mgr->common_frame_motion[index] = frame;
	}
}

// ==========================================================================
// GmDecoStartEffectFinalBossLight
/*!
 * ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒgŠJn
 */
// ==========================================================================
void GmDecoStartEffectFinalBossLight( void )
{
	GmDecoSetFrameMotion( 1, 2 );
}

// ==========================================================================
// GmDecoGetFallRenderTarget
/*!
 * ‰œ‚Ì‘ê—p‚ÌƒŒƒ“ƒ_[ƒ^[ƒQƒbƒg‚ğæ“¾
 *
 *	è‘O‚Ì‘ê‚ª¶¬Ï‚İ‚È‚çè‘O‚ÌƒeƒNƒXƒ`ƒƒ‚ğA
 *	è‘O‚Ì‘ê‚ª‚È‚­AŒã‚ë‚Ì‘ê‚ª¶¬Ï‚İ‚È‚çŒã‚ë‚ÌƒeƒNƒXƒ`ƒƒA
 *	‚Ç‚¿‚ç‚à‚È‚¢‚È‚çAè‘O‚ÌƒeƒNƒXƒ`ƒƒ‚ğ¶¬‚µ‚Ä•Ô‚·B
 *
 *	•`‰æƒXƒŒƒbƒh‚ÅŒÄ‚ñ‚Å‚­‚¾‚³‚¢B
 *
 * @return ƒŒƒ“ƒ_[ƒ^[ƒQƒbƒg
 *
 */
// ==========================================================================
AMS_RENDER_TARGET* GmDecoGetFallRenderTarget( void ) 
{
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( !mgr ){
		return NULL;
	}

	AMS_RENDER_TARGET* render_target = NULL;
	if ( mgr->flag_render_front ){
		render_target = mgr->render_target_front;
	}
	else if ( mgr->render_target_back ){
		render_target = mgr->render_target_back;
	}
	else{
		render_target = gmDecoDrawFallCopyRenderFront();
	}

	return render_target;
}

// =======================================================================
// GmDecoSetLightFinalZone
/*!
 * ‘•ü—pƒ‰ƒCƒg
 */
// =======================================================================
void GmDecoSetLightFinalZone(void)
{
	NNS_RGBA light_color = {
		1.0f, 1.0f, 1.0f, 1.0f,
	};

	NNS_VECTOR light_vec;
	light_vec.x = 0.0f;
	light_vec.y = -0.3f;
	light_vec.z = -0.4f;

	nnNormalizeVector( &light_vec, &light_vec );
	ObjDrawSetParallelLight( NNE_LIGHT_2, &light_color, 0.8f, &light_vec );
}

// =======================================================================
// GmDecoSetFlagLoop
/*!
 * ƒ‹[ƒvæ‚Ìƒ‚ƒfƒ‹‚Éƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€‚ğŒp³‚·‚éİ’è
 */
// =======================================================================
void GmDecoSetLoopState( void )
{
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( !mgr ){
		return;
	}

	s32 index = 0;

	OBS_OBJECT_WORK* obj_work = ObjObjectSearchRegistObject( NULL, GMD_OBJTYPE_DECORATION );
	while ( obj_work ){
		if ( obj_work->ppFunc == gmDecoMainFuncLoop ){
			//¶¬‚ÉŠù‚É‰æ–ÊŠO‚Ì‚à‚Ì‚Í1‚ğİ’è‚µ‚Ä‚¨‚­
			if ( index < GMD_DECO_LOOP_MODEL_OFFSET ){
				mgr->motion_frame_loop[index] = 1;
				++index;
				continue;
			}
			if ( index < GMD_DECO_LOOP_MODEL_NUM ){
				mgr->motion_frame_loop[index] = obj_work->user_timer;
				++index;
			}

			//íœ—v‹
			obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
			obj_work->disp_flag |= OBD_DISP_NODISP;

			//ƒCƒxƒ“ƒgƒŒƒR[ƒh‚ÌƒŠƒ“ƒN‚ğØ‚é
			GMS_DECO_WORK* deco_work = (GMS_DECO_WORK*)obj_work;
			GMS_EVE_RECORD_DECORATE* dec_rec = deco_work->event_record;
			if ( dec_rec ){
				if ( dec_rec->pos_x == GMD_EVE_RECORD_CMD_SKIP ){
					dec_rec->pos_x = deco_work->event_x;
					deco_work->event_x = 0;
				}
				deco_work->event_record = NULL;
			}
		}

		//Ÿ‚ÌƒIƒuƒWƒFƒNƒg
		obj_work = ObjObjectSearchRegistObject( obj_work, GMD_OBJTYPE_DECORATION );
	}
}


// =======================================================================
// GmDecoClearLoopState
/*!
 * ƒ‹[ƒvæ‚Ìƒ‚ƒfƒ‹‚Éƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€‚ğŒp³‚·‚éİ’è‚ğƒNƒŠƒA
 */
// =======================================================================
void GmDecoClearLoopState( void )
{
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( !mgr ){
		return;
	}

	amZeroMemory(mgr->motion_frame_loop, sizeof(mgr->motion_frame_loop));
}

// =======================================================================
// GmDecoStartLoop
/*!
 * ƒ‹[ƒvŠJn
 */
// =======================================================================
void GmDecoStartLoop( void )
{
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( !mgr ){
		return;
	}

	mgr->state_loop = GMD_DECO_LOOP_STATE_LOOP;
}

// =======================================================================
// GmDecoSetFlagLoop
/*!
 * ƒ‹[ƒvI—¹
 */
// =======================================================================
void GmDecoEndLoop( void )
{
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( !mgr ){
		return;
	}

	mgr->state_loop = GMD_DECO_LOOP_STATE_END;
}

//----- Local Functions -----------------------------------------------------



// ==========================================================================
// ƒQ[ƒ€ƒVƒXƒeƒ€
// ==========================================================================


// ==========================================================================
// gmDecoGameSystemGetStageId
/*!
 * ƒQ[ƒ€ƒVƒXƒeƒ€‚©‚çƒXƒe[ƒWID‚ğæ“¾
 *
 * @retun ƒXƒe[ƒWID
 */
// ==========================================================================
/*GSE_MAIN_STAGE_ID gmDecoGameSystemGetStageId( void )
{
	return (GSE_MAIN_STAGE_ID)g_gs_main_sys_info.stage_id;
}*/

// ==========================================================================
// gmDecoGameSystemGetSyncTime
/*!
 * ƒQ[ƒ€ƒVƒXƒeƒ€‚©‚ç“¯ŠúŠÔ‚ğæ“¾
 *
 *	@return	“¯ŠúŠÔ
 */
// ==========================================================================
u32 gmDecoGameSystemGetSyncTime( void )
{
	return g_gm_main_system.sync_time;
}


// ==========================================================================
// ƒf[ƒ^ŠÇ—
// ==========================================================================

// ==========================================================================
// gmDecoDataInit
/*!
 * ƒf[ƒ^ŠÇ—‰Šú‰»
 */
// ==========================================================================
void gmDecoDataInit( void )
{
	amAssert(!g_deco_data);

	amZeroMemory(&g_deco_data_real, sizeof(GMS_DECO_DATA));
	g_deco_data = &g_deco_data_real;
}

// ==========================================================================
// gmDecoDataRelease
/*!
 * “Ç‚İ‚ñ‚¾ƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
void gmDecoDataRelease( void )
{
	if ( g_deco_data ){
		//AMB‰ğ•ú
		gmDecoDataReleaseAmbHeader();

		g_deco_data = NULL;
	}
}

// ==========================================================================
// gmDecoDataGetInfo
/*!
 * ƒf[ƒ^ŠÇ—‚ğæ“¾
 *
 * @return  ƒf[ƒ^ŠÇ—
 */
// ==========================================================================
GMS_DECO_DATA* gmDecoDataGetInfo( void )
{
	return g_deco_data;
}

// ==========================================================================
// gmDecoDataSetAmbHeader
/*!
 * ƒf[ƒ^amb‚ğİ’è
 *
 * @param amb  ƒf[ƒ^amb
 */
// ==========================================================================
void gmDecoDataSetAmbHeader( AMS_AMB_HEADER* amb )
{
	GMS_DECO_DATA* data = gmDecoDataGetInfo();
	amAssert(data);
	amAssert(!data->amb_header);

	data->amb_header = amb;
}

// ==========================================================================
// gmDecoDataGetAmbHeader
/*!
 * ƒf[ƒ^amb‚ğæ“¾
 *
 * @return ƒf[ƒ^amb
 */
// ==========================================================================
AMS_AMB_HEADER* gmDecoDataGetAmbHeader( void )
{
	GMS_DECO_DATA* data = gmDecoDataGetInfo();
	amAssert(data);

	return data->amb_header;
}

// ==========================================================================
// gmDecoDataReleaseAmbHeader
/*!
 * ƒf[ƒ^amb‚ğ‰ğ•ú
 */
// ==========================================================================
void gmDecoDataReleaseAmbHeader( void )
{
	GMS_DECO_DATA* data = gmDecoDataGetInfo();
	amAssert(data);

	if ( data->amb_header ){
		mtMemFreeMain( data->amb_header );
		data->amb_header = NULL;
	}
}


// ==========================================================================
// gmDecoDataGetObj3DList
/*!
 * ƒf[ƒ^ƒ[ƒNƒŠƒXƒg‚ğæ“¾
 */
// ==========================================================================
OBS_ACTION3D_NN_WORK* gmDecoDataGetObj3DList( GME_DECORATE_ID id )
{

	GMS_DECO_DATA* deco_data = gmDecoDataGetInfo();
	amAssert( deco_data );

	OBS_ACTION3D_NN_WORK* work_list = NULL;
	switch ( id ){
	case GMD_DECORATE_ID_EFFECT_WATERSLIDER_UNDER:	//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	case GMD_DECORATE_ID_EFFECT_WATERSLIDER_SPRAY:	//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	case GMD_DECORATE_ID_EFFECT_CANDDLE:			//ZONE3 ˜XC
	case GMD_DECORATE_ID_EFFECT_FACE:				//ZONE3 •Ç‚ÌŠç	
	case GMD_DECORATE_ID_EFFECT_TAKI:				//ZONE1‘êã•”—pƒGƒtƒFƒNƒg	
	case GMD_DECORATE_ID_EFFECT_CROSS:				//ZONE3 ˜XC
	case GMD_DECORATE_ID_EFFECT_WARNING:			//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	case GMD_DECORATE_ID_EFFECT_BOSSF_LIGHT:		//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
		break;
	case GMD_DECORATE_ID_PIL_A:		//•Ç
	case GMD_DECORATE_ID_PIL_B:		//•Ç
	case GMD_DECORATE_ID_FISH_R:	//•Ç
	case GMD_DECORATE_ID_FISH_L:	//•Ç
	case GMD_DECORATE_ID_BRI_A:		//•Ç
	case GMD_DECORATE_ID_BRI_B:		//•Ç
	case GMD_DECORATE_ID_SUNFLOWER:	//‚Ğ‚Ü‚í‚è
	case GMD_DECORATE_ID_WALL_A:	//•Ç
	case GMD_DECORATE_ID_WALL_B:	//•Ç
	case GMD_DECORATE_ID_ZONE1_ASIHANA_A:	//ƒAƒVƒnƒi
	case GMD_DECORATE_ID_ZONE1_ASIHANA_B:	//ƒAƒVƒnƒi
	case GMD_DECORATE_ID_ZONE1_ASIHANA_C:	//ƒAƒVƒnƒi
	case GMD_DECORATE_ID_ZONE1_HANA_A:	//‰Ô
	case GMD_DECORATE_ID_ZONE1_HANA_B:	//‰Ô
	case GMD_DECORATE_ID_ZONE1_WOOD_A:	//–Ø
	case GMD_DECORATE_ID_WALL_AA:	//•Ç
	case GMD_DECORATE_ID_WALL_AB:	//•Ç
	case GMD_DECORATE_ID_WALL_AC:	//•Ç
	case GMD_DECORATE_ID_WALL_AD:	//•Ç
	case GMD_DECORATE_ID_WALL_BA:	//•Ç
	case GMD_DECORATE_ID_WALL_A_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_B_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AA_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AB_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AC_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AD_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_BA_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_HAI_FR_L:		//”r‰t‘•’u
	case GMD_DECORATE_ID_HAI_FR_EXIT_L:	//”r‰t‘•’uoŒû
	case GMD_DECORATE_ID_HAI_FR_R:		//”r‰t‘•’u
	case GMD_DECORATE_ID_HAI_FR_EXIT_R:	//”r‰t‘•’uoŒû
	case GMD_DECORATE_ID_EAR_RUB_A_A:	//ZONE3Šâ
	case GMD_DECORATE_ID_EAR_RUB_A_B:	//ZONE3Šâ
	case GMD_DECORATE_ID_EAR_RUB_B_A:	//ZONE3Šâ
	case GMD_DECORATE_ID_EAR_RUB_B_B:	//ZONE3Šâ
	case GMD_DECORATE_ID_EAR_RUB_B_C:	//ZONE3Šâ
	case GMD_DECORATE_ID_WAT_RUB_A:		//ZONE3Šâ
	case GMD_DECORATE_ID_WAT_RUB_B:		//ZONE3Šâ
	case GMD_DECORATE_ID_PLANT_A:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_B:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_C:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_D:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_A_FRONT:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_C_FRONT:		//ZONE3A•¨
	case GMD_DECORATE_ID_EAR_RUB_B_A_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_EAR_RUB_B_B_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_EAR_RUB_B_C_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_WAT_RUB_A_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_WAT_RUB_B_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_PLANT_B_FRONT:		//ZONE3A•¨i‘Oj
	case GMD_DECORATE_ID_PLANT_D_FRONT:		//ZONE3A•¨i‘Oj
	case GMD_DECORATE_ID_FISH_R_FRONT:		//•Çi‘Oj
	case GMD_DECORATE_ID_FISH_L_FRONT:		//•Çi‘Oj@
	case GMD_DECORATE_ID_RAIL_EDGE_A:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_EDGE_B:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_EDGE_A_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	case GMD_DECORATE_ID_RAIL_EDGE_B_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j@
	case GMD_DECORATE_ID_WHE_HOLD_A:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_B:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_C_L:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_D_T:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_C_R:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_D_B:		//ZONE4•Ô—p
	case GMD_DECORATE_ID_FISH_B_R:				//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	case GMD_DECORATE_ID_FISH_B_L:				//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	case GMD_DECORATE_ID_RAIL_J_A_ENDING:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_J_B_ENDING:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_J_C_ENDING:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_J_A_ENDING_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	case GMD_DECORATE_ID_RAIL_J_B_ENDING_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j@
	case GMD_DECORATE_ID_RAIL_J_C_ENDING_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j@
	case GMD_DECORATE_ID_SUNFLOWER_ENDING:		//‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	case GMD_DECORATE_ID_ZONE1_HANA_A_ENDING:	//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	case GMD_DECORATE_ID_ZONE1_HANA_B_ENDING:	//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	case GMD_DECORATE_ID_ZONE1_WOOD_A_ENDING:	//–ØiƒGƒ“ƒfƒBƒ“ƒOj
	case GMD_DECORATE_ID_WHEEL_PIL_A:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_B:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_C:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_A_FLIP:		//ZONE4•Ô—p’Œiã‰º”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_B_FLIP:		//ZONE4•Ô—p’Œiã‰º”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_C_FLIP:		//ZONE4•Ô—p’Œiã‰º”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_D:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_E:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_F:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_D_FLIP:		//ZONE4•Ô—p’Œi¶‰E”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_E_FLIP:		//ZONE4•Ô—p’Œi¶‰E”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_F_FLIP:		//ZONE4•Ô—p’Œi¶‰E”½“]j
	case GMD_DECORATE_ID_WHE_HOLD_A_FRONT:		//ZONE4•Ô—piè‘Oj@
	case GMD_DECORATE_ID_WHE_HOLD_B_FRONT:		//ZONE4•Ô—piè‘Oj
	case GMD_DECORATE_ID_WHE_HOLD_C_L_FRONT:	//ZONE4•Ô—piè‘Oj@
	case GMD_DECORATE_ID_WHE_HOLD_D_T_FRONT:	//ZONE4•Ô—piè‘Oj
	case GMD_DECORATE_ID_WHE_HOLD_C_R_FRONT:	//ZONE4•Ô—piè‘Oj@
	case GMD_DECORATE_ID_WHE_HOLD_D_B_FRONT:	//ZONE4•Ô—piè‘Oj
	case GMD_DECORATE_ID_BRACE_A:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_B:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_C:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_D:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_E:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_F:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_G:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_PIL_COR:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_WARNING:			//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	case GMD_DECORATE_ID_BRACE_S_A:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_B:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_C:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_D:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_E:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_F:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_G:	//ZONE4 ’Œ	
	case GMD_DECORATE_ID_P_STESM_CO01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO03:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO04:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_L_01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_L_02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_R_01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_R_02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_T_01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_T_02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_U_01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_U_02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_TUBE01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE03:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE04:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE03_FLIP:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE04_FLIP:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_SHUTTER_3_MOVE_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_3_OPEN_MOVE:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_3_MOVE_MOVE:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_3_CLOSE_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_3_OPEN_OPEN:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_3_OPEN_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_5_MOVE_MOVE:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_5_CLOSE_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_5_OPEN_OPEN:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_5_OPEN_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_UKI_RAIL:			//ZONEF •‚“‡—pƒŒ[ƒ‹
	case GMD_DECORATE_ID_UKI_RAIL_FLIP:		//ZONEF •‚“‡—pƒŒ[ƒ‹
	case GMD_DECORATE_ID_PLANTA:		//ZONE2 A•¨
	case GMD_DECORATE_ID_PLANTB:		//ZONE2 A•¨
	case GMD_DECORATE_ID_SHUTTER_LOOP:	//ZONEF ƒVƒƒƒbƒ^[
#if !_IPHONE // iPhone‚ÍTVX‚Å•`‰æ
		work_list = deco_data->obj_3d_list;
		amAssert( work_list );
#endif // !_IPHONE
		break;
	case GMD_DECORATE_ID_FALL:			//‘ê
	case GMD_DECORATE_ID_FALL_LEFT:		//‘ê
	case GMD_DECORATE_ID_FALL_RIGHT:	//‘ê
	case GMD_DECORATE_ID_FALL_ONE:		//‘ê
	case GMD_DECORATE_ID_FALL_A:		//‘ê
	case GMD_DECORATE_ID_FALL_LEFT_A:	//‘ê
	case GMD_DECORATE_ID_FALL_RIGHT_A:	//‘ê
	case GMD_DECORATE_ID_FALL_ONE_A:	//‘ê
	case GMD_DECORATE_ID_FALL_BACK:		//‘ê
	case GMD_DECORATE_ID_FALL_LEFT_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_FALL_RIGHT_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_FALL_ONE_BACK:		//‘êi‰œj
	case GMD_DECORATE_ID_FALL_A_BACK:		//‘êi‰œj
	case GMD_DECORATE_ID_FALL_LEFT_A_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_FALL_RIGHT_A_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_FALL_ONE_A_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_FOUN_A:		//ZONE3•¬o‚·…
	case GMD_DECORATE_ID_FOUN_B:		//ZONE3•¬o‚·…
	case GMD_DECORATE_ID_FOUN_A_BACK:	//ZONE3•¬o‚·…i‰œj
	case GMD_DECORATE_ID_FOUN_B_BACK:	//ZONE3•¬o‚·…i‰œj
		work_list = deco_data->obj_3d_list_fall;
		amAssert( work_list );
		break;
	case GMD_DECORATE_ID_WATER_SLIDER_L:		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	case GMD_DECORATE_ID_WATER_SLIDER_L_J30:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	case GMD_DECORATE_ID_WATER_SLIDER_L_J45:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	case GMD_DECORATE_ID_WATER_SLIDER_L_J60:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	case GMD_DECORATE_ID_WATER_SLIDER_R:		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	case GMD_DECORATE_ID_WATER_SLIDER_R_J30:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	case GMD_DECORATE_ID_WATER_SLIDER_R_J45:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	case GMD_DECORATE_ID_WATER_SLIDER_R_J60:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
		work_list = GmGmkWaterSliderGetObj3DList();
		amAssert( work_list );
		break;
	case GMD_DECORATE_ID_GLARE_ORANGE:
	case GMD_DECORATE_ID_GLARE_RED:
	case GMD_DECORATE_ID_GLARE_GREEN:
	case GMD_DECORATE_ID_GLARE_SALMON:
	case GMD_DECORATE_ID_GLARE_ORANGE_ENDING:	//ƒGƒ“ƒfƒBƒ“ƒO—p
	case GMD_DECORATE_ID_GLARE_GREEN_ENDING:	//ƒGƒ“ƒfƒBƒ“ƒO—p
		break;

	default:
		amAssert( FALSE );
		break;
	}

	return work_list;
}
// ==========================================================================
// gmDecoDataGetMotionHeader
/*!
 * ƒ‚[ƒVƒ‡ƒ“AMB‚ğæ“¾
 */
// ==========================================================================
AMS_AMB_HEADER* gmDecoDataGetMotionHeader( GME_DECORATE_ID id )
{
	AMS_AMB_HEADER* motion_amb = NULL;
	switch ( id ){
	case GMD_DECORATE_ID_PIL_A:		//•Ç
	case GMD_DECORATE_ID_PIL_B:		//•Ç
	case GMD_DECORATE_ID_FISH_R:	//•Ç
	case GMD_DECORATE_ID_FISH_L:	//•Ç
	case GMD_DECORATE_ID_BRI_A:		//•Ç
	case GMD_DECORATE_ID_BRI_B:		//•Ç
	case GMD_DECORATE_ID_WATER_SLIDER_L:		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	case GMD_DECORATE_ID_WATER_SLIDER_R:		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	case GMD_DECORATE_ID_GLARE_ORANGE:
	case GMD_DECORATE_ID_GLARE_RED:
	case GMD_DECORATE_ID_GLARE_GREEN:
	case GMD_DECORATE_ID_WALL_A:
	case GMD_DECORATE_ID_WALL_B:
	case GMD_DECORATE_ID_ZONE1_ASIHANA_A:	//ƒAƒVƒnƒi
	case GMD_DECORATE_ID_ZONE1_ASIHANA_B:	//ƒAƒVƒnƒi
	case GMD_DECORATE_ID_ZONE1_ASIHANA_C:	//ƒAƒVƒnƒi
	case GMD_DECORATE_ID_ZONE1_HANA_A:	//‰Ô
	case GMD_DECORATE_ID_ZONE1_HANA_B:	//‰Ô
	case GMD_DECORATE_ID_WALL_AA:	//•Ç
	case GMD_DECORATE_ID_WALL_AB:	//•Ç
	case GMD_DECORATE_ID_WALL_AC:	//•Ç
	case GMD_DECORATE_ID_WALL_AD:	//•Ç
	case GMD_DECORATE_ID_WALL_BA:	//•Ç
	case GMD_DECORATE_ID_WALL_A_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_B_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AA_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AB_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AC_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AD_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_BA_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_FALL:			//‘ê
	case GMD_DECORATE_ID_FALL_LEFT:		//‘ê
	case GMD_DECORATE_ID_FALL_RIGHT:	//‘ê
	case GMD_DECORATE_ID_FALL_ONE:		//‘ê
	case GMD_DECORATE_ID_FALL_A:		//‘ê
	case GMD_DECORATE_ID_FALL_LEFT_A:	//‘ê
	case GMD_DECORATE_ID_FALL_RIGHT_A:	//‘ê
	case GMD_DECORATE_ID_FALL_ONE_A:	//‘ê
	case GMD_DECORATE_ID_FALL_BACK:		//‘ê
	case GMD_DECORATE_ID_FALL_LEFT_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_FALL_RIGHT_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_FALL_ONE_BACK:		//‘êi‰œj
	case GMD_DECORATE_ID_FALL_A_BACK:		//‘êi‰œj
	case GMD_DECORATE_ID_FALL_LEFT_A_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_FALL_RIGHT_A_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_FALL_ONE_A_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_GLARE_SALMON:
	case GMD_DECORATE_ID_HAI_FR_L:		//”r‰t‘•’u
	case GMD_DECORATE_ID_HAI_FR_EXIT_L:	//”r‰t‘•’uoŒû
	case GMD_DECORATE_ID_HAI_FR_R:		//”r‰t‘•’u
	case GMD_DECORATE_ID_HAI_FR_EXIT_R:	//”r‰t‘•’uoŒû
	case GMD_DECORATE_ID_EAR_RUB_A_A:	//ZONE3Šâ
	case GMD_DECORATE_ID_EAR_RUB_A_B:	//ZONE3Šâ
	case GMD_DECORATE_ID_EAR_RUB_B_A:	//ZONE3Šâ
	case GMD_DECORATE_ID_EAR_RUB_B_B:	//ZONE3Šâ
	case GMD_DECORATE_ID_EAR_RUB_B_C:	//ZONE3Šâ
	case GMD_DECORATE_ID_WAT_RUB_A:		//ZONE3Šâ
	case GMD_DECORATE_ID_WAT_RUB_B:		//ZONE3Šâ
	case GMD_DECORATE_ID_EFFECT_WATERSLIDER_UNDER:	//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	case GMD_DECORATE_ID_EFFECT_WATERSLIDER_SPRAY:	//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	case GMD_DECORATE_ID_EFFECT_CANDDLE:			//ZONE3 ˜XC
	case GMD_DECORATE_ID_EFFECT_FACE:				//ZONE3 •Ç‚ÌŠç	
	case GMD_DECORATE_ID_FOUN_A:		//ZONE3•¬o‚·…
	case GMD_DECORATE_ID_FOUN_B:		//ZONE3•¬o‚·…
	case GMD_DECORATE_ID_FOUN_A_BACK:	//ZONE3•¬o‚·…i‰œj
	case GMD_DECORATE_ID_FOUN_B_BACK:	//ZONE3•¬o‚·…i‰œj
	case GMD_DECORATE_ID_PLANT_A:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_B:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_C:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_D:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_A_FRONT:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_C_FRONT:		//ZONE3A•¨
	case GMD_DECORATE_ID_EAR_RUB_B_A_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_EAR_RUB_B_B_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_EAR_RUB_B_C_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_WAT_RUB_A_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_WAT_RUB_B_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_PLANT_B_FRONT:		//ZONE3A•¨i‘Oj
	case GMD_DECORATE_ID_PLANT_D_FRONT:		//ZONE3A•¨i‘Oj
	case GMD_DECORATE_ID_FISH_R_FRONT:		//•Çi‘Oj
	case GMD_DECORATE_ID_FISH_L_FRONT:		//•Çi‘Oj
	case GMD_DECORATE_ID_RAIL_EDGE_A:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_EDGE_B:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_EDGE_A_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	case GMD_DECORATE_ID_RAIL_EDGE_B_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j@
	case GMD_DECORATE_ID_WHE_HOLD_A:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_B:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_C_L:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_D_T:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_C_R:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_D_B:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_EFFECT_TAKI:		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg	
	case GMD_DECORATE_ID_EFFECT_CROSS:				//ZONE3 ˜XC
	case GMD_DECORATE_ID_FISH_B_R:				//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	case GMD_DECORATE_ID_FISH_B_L:				//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	case GMD_DECORATE_ID_RAIL_J_A_ENDING:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_J_B_ENDING:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_J_C_ENDING:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_J_A_ENDING_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	case GMD_DECORATE_ID_RAIL_J_B_ENDING_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j@
	case GMD_DECORATE_ID_RAIL_J_C_ENDING_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	case GMD_DECORATE_ID_ZONE1_HANA_A_ENDING:	//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	case GMD_DECORATE_ID_ZONE1_HANA_B_ENDING:	//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	case GMD_DECORATE_ID_GLARE_ORANGE_ENDING:	//ƒGƒ“ƒfƒBƒ“ƒO—p
	case GMD_DECORATE_ID_GLARE_GREEN_ENDING:	//ƒGƒ“ƒfƒBƒ“ƒO—p
	case GMD_DECORATE_ID_WHEEL_PIL_A:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_B:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_C:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_A_FLIP:		//ZONE4•Ô—p’Œiã‰º”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_B_FLIP:		//ZONE4•Ô—p’Œiã‰º”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_C_FLIP:		//ZONE4•Ô—p’Œiã‰º”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_D:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_E:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_F:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_D_FLIP:		//ZONE4•Ô—p’Œi¶‰E”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_E_FLIP:		//ZONE4•Ô—p’Œi¶‰E”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_F_FLIP:		//ZONE4•Ô—p’Œi¶‰E”½“]j
	case GMD_DECORATE_ID_WHE_HOLD_A_FRONT:		//ZONE4•Ô—piè‘Oj@
	case GMD_DECORATE_ID_WHE_HOLD_B_FRONT:		//ZONE4•Ô—piè‘Oj
	case GMD_DECORATE_ID_WHE_HOLD_C_L_FRONT:	//ZONE4•Ô—piè‘Oj@
	case GMD_DECORATE_ID_WHE_HOLD_D_T_FRONT:	//ZONE4•Ô—piè‘Oj
	case GMD_DECORATE_ID_WHE_HOLD_C_R_FRONT:	//ZONE4•Ô—piè‘Oj@
	case GMD_DECORATE_ID_WHE_HOLD_D_B_FRONT:	//ZONE4•Ô—piè‘Oj
	case GMD_DECORATE_ID_BRACE_A:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_B:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_C:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_D:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_E:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_F:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_G:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_PIL_COR:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_WARNING:			//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	case GMD_DECORATE_ID_EFFECT_WARNING:	//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	case GMD_DECORATE_ID_BRACE_S_A:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_B:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_C:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_D:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_E:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_F:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_G:	//ZONE4 ’Œ	
	case GMD_DECORATE_ID_P_STESM_CO01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO03:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO04:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_L_01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_L_02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_R_01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_R_02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_T_01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_T_02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_U_01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_U_02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_TUBE01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE03:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE04:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE03_FLIP:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE04_FLIP:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_UKI_RAIL:			//ZONEF •‚“‡—pƒŒ[ƒ‹
	case GMD_DECORATE_ID_UKI_RAIL_FLIP:		//ZONEF •‚“‡—pƒŒ[ƒ‹
	case GMD_DECORATE_ID_SHUTTER_3_CLOSE_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_5_CLOSE_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_PLANTA:		//ZONE2 A•¨
	case GMD_DECORATE_ID_PLANTB:		//ZONE2 A•¨
	case GMD_DECORATE_ID_EFFECT_BOSSF_LIGHT:		//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
		break;
	case GMD_DECORATE_ID_SUNFLOWER:
	case GMD_DECORATE_ID_ZONE1_WOOD_A:	//–Ø
	case GMD_DECORATE_ID_SUNFLOWER_ENDING:		//‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	case GMD_DECORATE_ID_ZONE1_WOOD_A_ENDING:	//–ØiƒGƒ“ƒfƒBƒ“ƒOj
	case GMD_DECORATE_ID_SHUTTER_3_MOVE_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_3_OPEN_MOVE:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_3_MOVE_MOVE:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_3_OPEN_OPEN:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_3_OPEN_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_5_MOVE_MOVE:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_5_OPEN_OPEN:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_5_OPEN_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_LOOP:	//ZONEF ƒVƒƒƒbƒ^[
		{
			//AMBƒf[ƒ^
			AMS_AMB_HEADER* amb_header = gmDecoDataGetAmbHeader();
			amAssert( amb_header );
			motion_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_MTN );
			amAssert( motion_amb );
		}
		break;
	case GMD_DECORATE_ID_WATER_SLIDER_L_J30:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	case GMD_DECORATE_ID_WATER_SLIDER_L_J45:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	case GMD_DECORATE_ID_WATER_SLIDER_L_J60:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	case GMD_DECORATE_ID_WATER_SLIDER_R_J30:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	case GMD_DECORATE_ID_WATER_SLIDER_R_J45:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	case GMD_DECORATE_ID_WATER_SLIDER_R_J60:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
		{
			motion_amb = (AMS_AMB_HEADER*)ObjDataGet(GMD_DWORK_NO_GMK_WATER_SLIDER_MTN)->pData;
			amAssert( motion_amb );
		}
		break;
	default:
		amAssert( FALSE );
		break;
	}
	return motion_amb;
}

// ==========================================================================
// gmDecoDataGetMatMotionHeader
/*!
 * ƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“AMB‚ğæ“¾
 */
// ==========================================================================
AMS_AMB_HEADER* gmDecoDataGetMatMotionHeader( GME_DECORATE_ID id )
{
	AMS_AMB_HEADER* motion_amb = NULL;
	switch ( id ){
	case GMD_DECORATE_ID_PIL_A:		//•Ç
	case GMD_DECORATE_ID_PIL_B:		//•Ç
	case GMD_DECORATE_ID_FISH_R:	//•Ç
	case GMD_DECORATE_ID_FISH_L:	//•Ç
	case GMD_DECORATE_ID_BRI_A:		//•Ç
	case GMD_DECORATE_ID_BRI_B:		//•Ç
	case GMD_DECORATE_ID_ZONE1_ASIHANA_A:	//ƒAƒVƒnƒi
	case GMD_DECORATE_ID_ZONE1_ASIHANA_B:	//ƒAƒVƒnƒi
	case GMD_DECORATE_ID_ZONE1_ASIHANA_C:	//ƒAƒVƒnƒi
	case GMD_DECORATE_ID_ZONE1_HANA_A:	//‰Ô
	case GMD_DECORATE_ID_ZONE1_HANA_B:	//‰Ô
	case GMD_DECORATE_ID_ZONE1_WOOD_A:	//–Ø
	case GMD_DECORATE_ID_WALL_AA:	//•Ç
	case GMD_DECORATE_ID_WALL_AB:	//•Ç
	case GMD_DECORATE_ID_WALL_AC:	//•Ç
	case GMD_DECORATE_ID_WALL_AD:	//•Ç
	case GMD_DECORATE_ID_WALL_BA:	//•Ç
	case GMD_DECORATE_ID_WALL_A_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_B_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AA_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AB_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AC_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AD_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_BA_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_HAI_FR_L:		//”r‰t‘•’u
	case GMD_DECORATE_ID_HAI_FR_EXIT_L:	//”r‰t‘•’uoŒû
	case GMD_DECORATE_ID_HAI_FR_R:		//”r‰t‘•’u
	case GMD_DECORATE_ID_HAI_FR_EXIT_R:	//”r‰t‘•’uoŒû
	case GMD_DECORATE_ID_EAR_RUB_A_A:	//ZONE3Šâ
	case GMD_DECORATE_ID_EAR_RUB_A_B:	//ZONE3Šâ
	case GMD_DECORATE_ID_EAR_RUB_B_A:	//ZONE3Šâ
	case GMD_DECORATE_ID_EAR_RUB_B_B:	//ZONE3Šâ
	case GMD_DECORATE_ID_EAR_RUB_B_C:	//ZONE3Šâ
	case GMD_DECORATE_ID_WAT_RUB_A:		//ZONE3Šâ
	case GMD_DECORATE_ID_WAT_RUB_B:		//ZONE3Šâ
	case GMD_DECORATE_ID_EFFECT_WATERSLIDER_UNDER:	//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…–Ê‰º
	case GMD_DECORATE_ID_EFFECT_WATERSLIDER_SPRAY:	//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[—pƒGƒtƒFƒNƒg…”ò–—
	case GMD_DECORATE_ID_EFFECT_CANDDLE:			//ZONE3 ˜XC
	case GMD_DECORATE_ID_EFFECT_FACE:				//ZONE3 •Ç‚ÌŠç	
	case GMD_DECORATE_ID_PLANT_A:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_B:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_C:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_D:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_A_FRONT:		//ZONE3A•¨
	case GMD_DECORATE_ID_PLANT_C_FRONT:		//ZONE3A•¨
	case GMD_DECORATE_ID_EAR_RUB_B_A_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_EAR_RUB_B_B_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_EAR_RUB_B_C_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_WAT_RUB_A_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_WAT_RUB_B_FRONT:	//ZONE3Šâi‘Oj
	case GMD_DECORATE_ID_PLANT_B_FRONT:		//ZONE3A•¨i‘Oj
	case GMD_DECORATE_ID_PLANT_D_FRONT:		//ZONE3A•¨i‘Oj
	case GMD_DECORATE_ID_FISH_R_FRONT:		//•Çi‘Oj
	case GMD_DECORATE_ID_FISH_L_FRONT:		//•Çi‘Oj
	case GMD_DECORATE_ID_RAIL_EDGE_A:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_EDGE_B:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_EDGE_A_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	case GMD_DECORATE_ID_RAIL_EDGE_B_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j@
	case GMD_DECORATE_ID_WHE_HOLD_A:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_B:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_C_L:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_D_T:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_C_R:		//ZONE4•Ô—p@
	case GMD_DECORATE_ID_WHE_HOLD_D_B:		//ZONE4•Ô—p@	
	case GMD_DECORATE_ID_EFFECT_TAKI:		//ZONE1‘êã•”—pƒGƒtƒFƒNƒg	
	case GMD_DECORATE_ID_EFFECT_CROSS:		//ZONE3 ˜XC	
	case GMD_DECORATE_ID_FISH_B_R:				//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…a
	case GMD_DECORATE_ID_FISH_B_L:				//ZONE3 ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[”r…ai¶‰E”½“]j
	case GMD_DECORATE_ID_RAIL_J_A_ENDING:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_J_B_ENDING:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_J_C_ENDING:		//ZONE3ƒŒ[ƒ‹Šp
	case GMD_DECORATE_ID_RAIL_J_A_ENDING_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	case GMD_DECORATE_ID_RAIL_J_B_ENDING_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j@
	case GMD_DECORATE_ID_RAIL_J_C_ENDING_FLIP:	//ZONE3ƒŒ[ƒ‹Špi¶‰E”½“]j
	case GMD_DECORATE_ID_ZONE1_HANA_A_ENDING:	//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	case GMD_DECORATE_ID_ZONE1_HANA_B_ENDING:	//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	case GMD_DECORATE_ID_ZONE1_WOOD_A_ENDING:	//–ØiƒGƒ“ƒfƒBƒ“ƒOj@
	case GMD_DECORATE_ID_SUNFLOWER_ENDING:		//‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	case GMD_DECORATE_ID_WHEEL_PIL_A:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_B:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_C:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_A_FLIP:		//ZONE4•Ô—p’Œiã‰º”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_B_FLIP:		//ZONE4•Ô—p’Œiã‰º”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_C_FLIP:		//ZONE4•Ô—p’Œiã‰º”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_D:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_E:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_F:			//ZONE4•Ô—p’Œ
	case GMD_DECORATE_ID_WHEEL_PIL_D_FLIP:		//ZONE4•Ô—p’Œi¶‰E”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_E_FLIP:		//ZONE4•Ô—p’Œi¶‰E”½“]j
	case GMD_DECORATE_ID_WHEEL_PIL_F_FLIP:		//ZONE4•Ô—p’Œi¶‰E”½“]j
	case GMD_DECORATE_ID_WHE_HOLD_A_FRONT:		//ZONE4•Ô—piè‘Oj@
	case GMD_DECORATE_ID_WHE_HOLD_B_FRONT:		//ZONE4•Ô—piè‘Oj
	case GMD_DECORATE_ID_WHE_HOLD_C_L_FRONT:	//ZONE4•Ô—piè‘Oj@
	case GMD_DECORATE_ID_WHE_HOLD_D_T_FRONT:	//ZONE4•Ô—piè‘Oj
	case GMD_DECORATE_ID_WHE_HOLD_C_R_FRONT:	//ZONE4•Ô—piè‘Oj@
	case GMD_DECORATE_ID_WHE_HOLD_D_B_FRONT:	//ZONE4•Ô—piè‘Oj
	case GMD_DECORATE_ID_BRACE_A:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_B:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_C:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_D:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_E:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_F:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_G:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_PIL_COR:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_WARNING:			//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	case GMD_DECORATE_ID_EFFECT_WARNING:	//ZONE4 ƒpƒgƒ‰ƒ“ƒv
	case GMD_DECORATE_ID_BRACE_S_A:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_B:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_C:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_D:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_E:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_F:	//ZONE4 ’Œ
	case GMD_DECORATE_ID_BRACE_S_G:	//ZONE4 ’Œ	
	case GMD_DECORATE_ID_P_STESM_CO01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO03:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO04:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_L_01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_L_02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_R_01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_R_02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_T_01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_T_02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_U_01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_CO_U_02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€Šp
	case GMD_DECORATE_ID_P_STESM_TUBE01:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE02:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE03:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE04:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE03_FLIP:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_P_STESM_TUBE04_FLIP:	//ZONE4 ƒ|ƒbƒvƒXƒ`[ƒ€ƒ`ƒ…[ƒu
	case GMD_DECORATE_ID_UKI_RAIL:			//ZONEF •‚“‡—pƒŒ[ƒ‹
	case GMD_DECORATE_ID_UKI_RAIL_FLIP:		//ZONEF •‚“‡—pƒŒ[ƒ‹
	case GMD_DECORATE_ID_SHUTTER_3_CLOSE_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_5_CLOSE_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_PLANTA:		//ZONE2 A•¨
	case GMD_DECORATE_ID_PLANTB:		//ZONE2 A•¨
	case GMD_DECORATE_ID_EFFECT_BOSSF_LIGHT:		//ZONEF ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒg
		break;

	case GMD_DECORATE_ID_FALL:			//‘ê
	case GMD_DECORATE_ID_FALL_LEFT:		//‘ê
	case GMD_DECORATE_ID_FALL_RIGHT:	//‘ê
	case GMD_DECORATE_ID_FALL_ONE:		//‘ê
	case GMD_DECORATE_ID_FALL_A:		//‘ê
	case GMD_DECORATE_ID_FALL_LEFT_A:	//‘ê
	case GMD_DECORATE_ID_FALL_RIGHT_A:	//‘ê
	case GMD_DECORATE_ID_FALL_ONE_A:	//‘ê
	case GMD_DECORATE_ID_FALL_BACK:		//‘ê
	case GMD_DECORATE_ID_FALL_LEFT_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_FALL_RIGHT_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_FALL_ONE_BACK:		//‘êi‰œj
	case GMD_DECORATE_ID_FALL_A_BACK:		//‘êi‰œj
	case GMD_DECORATE_ID_FALL_LEFT_A_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_FALL_RIGHT_A_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_FALL_ONE_A_BACK:	//‘êi‰œj
	case GMD_DECORATE_ID_FOUN_A:		//ZONE3•¬o‚·…
	case GMD_DECORATE_ID_FOUN_B:		//ZONE3•¬o‚·…
	case GMD_DECORATE_ID_FOUN_A_BACK:	//ZONE3•¬o‚·…i‰œj
	case GMD_DECORATE_ID_FOUN_B_BACK:	//ZONE3•¬o‚·…i‰œj
	case GMD_DECORATE_ID_SHUTTER_3_MOVE_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_3_OPEN_MOVE:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_3_MOVE_MOVE:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_3_OPEN_OPEN:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_3_OPEN_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_5_MOVE_MOVE:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_5_OPEN_OPEN:		//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_5_OPEN_CLOSE:	//ZONEF ƒVƒƒƒbƒ^[
	case GMD_DECORATE_ID_SHUTTER_LOOP:	//ZONEF ƒVƒƒƒbƒ^[
		{
			//AMBƒf[ƒ^
			AMS_AMB_HEADER* amb_header = gmDecoDataGetAmbHeader();
			amAssert( amb_header );
			motion_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_MAT );
			amAssert( motion_amb );
		}
		break;
	case GMD_DECORATE_ID_WATER_SLIDER_L:		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	case GMD_DECORATE_ID_WATER_SLIDER_L_J30:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	case GMD_DECORATE_ID_WATER_SLIDER_L_J45:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	case GMD_DECORATE_ID_WATER_SLIDER_L_J60:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[¶
	case GMD_DECORATE_ID_WATER_SLIDER_R:		//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	case GMD_DECORATE_ID_WATER_SLIDER_R_J30:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	case GMD_DECORATE_ID_WATER_SLIDER_R_J45:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
	case GMD_DECORATE_ID_WATER_SLIDER_R_J60:	//ƒEƒH[ƒ^[ƒXƒ‰ƒCƒ_[‰E
		{
			motion_amb = (AMS_AMB_HEADER*)ObjDataGet(GMD_DWORK_NO_GMK_WATER_SLIDER_MAT)->pData;
			amAssert( motion_amb );
		}
		break;

	case GMD_DECORATE_ID_GLARE_ORANGE:
	case GMD_DECORATE_ID_GLARE_RED:
	case GMD_DECORATE_ID_GLARE_GREEN:
	case GMD_DECORATE_ID_GLARE_SALMON:
	case GMD_DECORATE_ID_GLARE_ORANGE_ENDING:	//ƒGƒ“ƒfƒBƒ“ƒO—p
	case GMD_DECORATE_ID_GLARE_GREEN_ENDING:	//ƒGƒ“ƒfƒBƒ“ƒO—p
		break;

	default:
		amAssert( FALSE );
		break;
	}
	return motion_amb;
}

// ==========================================================================
// gmDecoCopySetRenderTargetForFront
/*!
 * ƒŒƒ“ƒ_ƒ^[ƒQƒbƒg‚ğİ’èiè‘O—pj
 *
 * @param target ƒŒƒ“ƒ_ƒ^[ƒQƒbƒg
 */
// ==========================================================================
void gmDecoCopySetRenderTargetForFront( AMS_RENDER_TARGET* target )
{
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( mgr ){
		mgr->flag_render_front = TRUE;
		mgr->render_target_front = target;
	}
}

// ==========================================================================
// gmDecoCopySetRenderTargetForBack
/*!
 * ƒŒƒ“ƒ_ƒ^[ƒQƒbƒg‚ğİ’èi‰œ—pj
 *
 * @param target ƒŒƒ“ƒ_ƒ^[ƒQƒbƒg
 */
// ==========================================================================
void gmDecoCopySetRenderTargetForBack( AMS_RENDER_TARGET* target )
{
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( mgr ){
		mgr->flag_render_back = TRUE;
		mgr->render_target_back = target;
	}
}

// ==========================================================================
// gmDecoGetRenderWorkFront
/*!
 * ƒŒƒ“ƒ_ƒ^[ƒQƒbƒg‚ğæ“¾
 */
// ==========================================================================
AMS_RENDER_TARGET* gmDecoGetRenderWorkFront( void )
{
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( !mgr ){
		return NULL;
	}
	if ( !mgr->render_target_front ){
		return NULL;
	}
	return mgr->render_target_front;
}

// ==========================================================================
// gmDecoGetRenderWorkBack
/*!
 * ƒŒƒ“ƒ_ƒ^[ƒQƒbƒg‚ğæ“¾
 */
// ==========================================================================
AMS_RENDER_TARGET* gmDecoGetRenderWorkBack( void )
{
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( !mgr ){
		return NULL;
	}
	if ( !mgr->render_target_back ){
		return NULL;
	}
	return mgr->render_target_back;
}

// ==========================================================================
// ‘•üŠÇ—
// ==========================================================================


// ==========================================================================
// gmDecoGetMgr
/*!
 * ‘•üŠÇ—æ“¾
 *
 * @retun ‘•üŠÇ—
 */
// ==========================================================================
GMS_DECO_MGR* gmDecoGetMgr( void )
{
	return g_deco_mgr;
}

// ==========================================================================
// gmDecoInitMgr
/*!
 * ‘•üŠÇ—‰Šú‰»
 */
// ==========================================================================
void gmDecoInitMgr( void )
{
	amAssert(!g_deco_mgr);

	amZeroMemory(&g_deco_mgr_real, sizeof(GMS_DECO_MGR));
	g_deco_mgr = &g_deco_mgr_real;

	//Œãˆ—TCBì¬
	gmDecoCreateTcbPost();
}

// ==========================================================================
// gmDecoExitMgr
/*!
 * ‘•üŠÇ—íœ
 */
// ==========================================================================
void gmDecoExitMgr( void )
{
	if ( g_deco_mgr ){
		//SE’â~
		if ( g_deco_mgr->se_handle ){
			GmSoundStopSE( g_deco_mgr->se_handle );
			GsSoundFreeSeHandle( g_deco_mgr->se_handle );	
			g_deco_mgr->se_handle = NULL;
		}
		
		//Œãˆ—TCBíœ
		gmDecoDeleteTcbPost();

		g_deco_mgr = NULL;
	}
}


// ==========================================================================
// gmDecoReleaseMgr
/*!
 * ‘•üŠÇ—‰ğ•ú
 */
// ==========================================================================
void gmDecoReleaseMgr( void )
{
}

// ==========================================================================
// gmDecoCreateTcbPost
/*!
 * Œãˆ—TCB‚ğì¬
 *
 * @return Œãˆ—TCB
 */
// ==========================================================================
MTS_TASK_TCB* gmDecoCreateTcbPost( void )
{
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	amAssert(mgr);
	
	//Šù‚Éì¬Ï‚İ
	amAssert( !mgr->tcb_post );

	//ì¬
	mgr->tcb_post = MTM_TASK_MAKE_TCB(
		gmDecoTcbProcPost,
		NULL,
		0,							//flag
		GMD_TASK_PAUSELEVEL_DEF,	//pause_level
		GMD_TASK_PRIO_DECORATION,
		GMD_TASK_GROUP_DECO_SYS,
		0,							//work_size
		"GM DECO POST");
	amAssert( mgr->tcb_post );

	return mgr->tcb_post;
}

// ==========================================================================
// gmDecoDeleteTcbPost
/*!
 * Œãˆ—TCB‚ğíœ
 *
 */
// ==========================================================================
void gmDecoDeleteTcbPost( void )
{
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	amAssert(mgr);
	if ( mgr->tcb_post ){
		mtTaskClearTcb( mgr->tcb_post );
		mgr->tcb_post = NULL;
	}
}

// ==========================================================================
// gmDecoTcbProcPost
/*!
 * Œãˆ—ƒvƒƒV[ƒWƒƒ
 *
 */
// ==========================================================================
void gmDecoTcbProcPost( MTS_TASK_TCB *tcb )
{
	UNREFERENCED_PARAMETER( tcb );

	//Œãˆ—‚ğ“o˜^i‘ê‚æ‚èŒãj
	ObjDraw3DNNUserFunc(gmDecoTcbProcPostDT, NULL, 0, OBD_DRAW_CMD_STATE_POST_WATER);
}

// ==========================================================================
// gmDecoTcbProcPostDT
/*!
 * Œãˆ—ƒ†[ƒUŠÖ”
 *
 */
// ==========================================================================
void gmDecoTcbProcPostDT(void *data)
{
	UNREFERENCED_PARAMETER( data );

	GMS_DECO_MGR* mgr = gmDecoGetMgr();

	//ƒtƒ‰ƒO‚ğ—‚Æ‚·
	if ( mgr ){
		mgr->flag_render_front = FALSE;
		mgr->flag_render_back = FALSE;
		mgr->render_target_front = NULL;
		mgr->render_target_back = NULL;
	}
}

// ==========================================================================
// gmDecoDraw
/*!
 *	‘•ü•`‰æ
 *
 *	@param	obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmDecoDraw( OBS_OBJECT_WORK* obj_work )
{
	u32 disp_flag = obj_work->disp_flag;

#if GMD_DECO_USE_DRAW_TVX
	GMS_DECO_WORK* deco_work = (GMS_DECO_WORK*)obj_work;
	
	// ê—pƒeƒNƒXƒ`ƒƒ‚ª‚ ‚é = TVXƒtƒ@ƒCƒ‹•`‰æ
	if (deco_work->model_tex) {
		if (!GmMainIsDrawEnable()) {
			return;
		}
		if (disp_flag & OBD_DISP_NODISP) {
			return;
		}
	
		//	•`‰æƒT[ƒo‚Ö‚Ì“o˜^ˆ—
		//---------------------------------------
		//ƒƒCƒ“ƒp[ƒc
		//---------------------------------------
		if (!(obj_work->user_flag & GMD_DECO_USER_FLAG_NO_MAIN_MODEL)){
			deco_work->model_index;
			deco_work->model_tex;
			&obj_work->pos,
			deco_work->obj_3d.command_state;
			gmDecoSetDrawPrimitive(deco_work->model_index,
				deco_work->model_tex,
				&obj_work->pos, 0.0f,
				deco_work->obj_3d.command_state,
				disp_flag);
		}
		
		//---------------------------------------
		//ƒTƒuƒp[ƒc
		//---------------------------------------
		if (obj_work->user_flag & GMD_DECO_USER_FLAG_SUB_MODEL){
			GMS_DECO_SUBMODEL_WORK* deco_submodel_work = (GMS_DECO_SUBMODEL_WORK*)deco_work;
		
			gmDecoSetDrawPrimitive(deco_submodel_work->sub_model_index,
				deco_work->model_tex,
				&obj_work->pos, 0.0f,
				deco_work->obj_3d.command_state,
				disp_flag);
		}
	}
	// ê—pƒeƒNƒXƒ`ƒƒ‚ª–³‚¢ = ƒ‚ƒfƒ‹•`‰æ
	else {
		//---------------------------------------
		//ƒƒCƒ“ƒp[ƒc
		//---------------------------------------	
		if ( !(obj_work->user_flag & GMD_DECO_USER_FLAG_NO_MAIN_MODEL) ){
			OBS_ACTION3D_NN_WORK *obj_3d = obj_work->obj_3d;
			amAssert( obj_3d );
			
			if ( obj_3d ){
				if ( obj_work->user_flag & GMD_DECO_USER_FLAG_SYNC_MATERIAL ){
					if ( obj_3d && obj_3d->motion && obj_3d->motion->mmobject ){
						//ƒ‚[ƒVƒ‡ƒ“‚ğ“¯Šú‚³‚¹‚é
						float frame_start = amMotionMaterialGetStartFrame( obj_3d->motion, obj_3d->mat_act_id );
						float frame_end = amMotionMaterialGetEndFrame( obj_3d->motion, obj_3d->mat_act_id );
						float frame_max = frame_end - frame_start;
						float frame = (float)gmDecoGameSystemGetSyncTime();
						obj_3d->mat_frame = fmod( frame, frame_max );
						obj_work->disp_flag |= OBD_DISP_STOP;
					}
				}
				
				//ƒJƒƒ‰İ’è
//#if _IPHONE
//				if ( obj_3d->command_state != OBD_DRAW_CMD_STATE_3DNN_PRE ){
//#else
				if ( obj_3d->command_state != OBD_DRAW_CMD_STATE_3DNN ){
//#endif // _IPHONE
					ObjDraw3DNNSetCameraEx(GME_CAMERA_NO_MAIN, NNE_PROJECTION_TYPE_ORTHO, obj_work->obj_3d->command_state );
				}
				
				//•`‰æ
				ObjDrawActionSummary( obj_work );
				
				//ƒtƒ‰ƒO‚ğ–ß‚·
				obj_work->disp_flag = disp_flag;
			}
		}
				
		//---------------------------------------
		//ƒTƒuƒp[ƒc
		//---------------------------------------	
		if ( obj_work->user_flag & GMD_DECO_USER_FLAG_SUB_MODEL ){
			if ( ObjObjectPauseCheck(0) ){
				disp_flag |= OBD_DISP_NOUPDATE;
			}
			
			GMS_DECO_SUBMODEL_WORK* deco_submodel_work = (GMS_DECO_SUBMODEL_WORK*)obj_work;
			deco_submodel_work->obj_3d_sub.command_state = OBD_DRAW_CMD_STATE_3DNN_WS; // timing delay
			//ƒJƒƒ‰İ’è
#if _IPHONE
			if ( deco_submodel_work->obj_3d_sub.command_state != OBD_DRAW_CMD_STATE_3DNN_WS ){
#else
			if ( deco_submodel_work->obj_3d_sub.command_state != OBD_DRAW_CMD_STATE_3DNN ){
#endif // _IPHONE
				ObjDraw3DNNSetCameraEx(GME_CAMERA_NO_MAIN, NNE_PROJECTION_TYPE_ORTHO, deco_submodel_work->obj_3d_sub.command_state );
			}
			ObjDrawAction3DNN(
							  &deco_submodel_work->obj_3d_sub,
							  &obj_work->pos, 
							  &obj_work->dir, 
							  &obj_work->scale, 
							  &disp_flag );
		}
	}
	
#else
	//---------------------------------------
	//ƒƒCƒ“ƒp[ƒc
	//---------------------------------------	
	if ( !(obj_work->user_flag & GMD_DECO_USER_FLAG_NO_MAIN_MODEL) ){
		OBS_ACTION3D_NN_WORK *obj_3d = obj_work->obj_3d;
		amAssert( obj_3d );

		if ( obj_3d ){
			if ( obj_work->user_flag & GMD_DECO_USER_FLAG_SYNC_MATERIAL ){
				if ( obj_3d && obj_3d->motion && obj_3d->motion->mmobject ){
					//ƒ‚[ƒVƒ‡ƒ“‚ğ“¯Šú‚³‚¹‚é
					float frame_start = amMotionMaterialGetStartFrame( obj_3d->motion, obj_3d->mat_act_id );
					float frame_end = amMotionMaterialGetEndFrame( obj_3d->motion, obj_3d->mat_act_id );
					float frame_max = frame_end - frame_start;
					float frame = (float)gmDecoGameSystemGetSyncTime();
					obj_3d->mat_frame = fmod( frame, frame_max );
					obj_work->disp_flag |= OBD_DISP_STOP;
				}
			}

			//ƒJƒƒ‰İ’è
			if ( obj_3d->command_state != OBD_DRAW_CMD_STATE_3DNN ){
				ObjDraw3DNNSetCameraEx(GME_CAMERA_NO_MAIN, NNE_PROJECTION_TYPE_ORTHO, obj_work->obj_3d->command_state );
			}

			//•`‰æ
			ObjDrawActionSummary( obj_work );

			//ƒtƒ‰ƒO‚ğ–ß‚·
			obj_work->disp_flag = disp_flag;
		}
	}

	//---------------------------------------
	//ƒTƒuƒp[ƒc
	//---------------------------------------	
	if ( obj_work->user_flag & GMD_DECO_USER_FLAG_SUB_MODEL ){
		if ( ObjObjectPauseCheck(0) ){
			disp_flag |= OBD_DISP_NOUPDATE;
		}
		
		GMS_DECO_SUBMODEL_WORK* deco_submodel_work = (GMS_DECO_SUBMODEL_WORK*)obj_work;

		//ƒJƒƒ‰İ’è
		if ( deco_submodel_work->obj_3d_sub.command_state != OBD_DRAW_CMD_STATE_3DNN ){
			ObjDraw3DNNSetCameraEx(GME_CAMERA_NO_MAIN, NNE_PROJECTION_TYPE_ORTHO, deco_submodel_work->obj_3d_sub.command_state );
		}
		ObjDrawAction3DNN(
				&deco_submodel_work->obj_3d_sub,
				&obj_work->pos, 
				&obj_work->dir, 
				&obj_work->scale, 
				&disp_flag );
	}
#endif // GMD_DECO_USE_DRAW_TVX
}

// ==========================================================================
// gmDecoDrawFinalShutter3Line
/*!
 *	ƒtƒ@ƒCƒiƒ‹ƒ][ƒ“—pƒVƒƒƒbƒ^[‘•ü•`‰æi3’i—pj
 *
 *	@param	obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmDecoDrawFinalShutter3Line( OBS_OBJECT_WORK* obj_work )
{
	//---------------------------------------
	//ƒƒCƒ“ƒp[ƒc
	//---------------------------------------	
	if ( !(obj_work->user_flag & GMD_DECO_USER_FLAG_NO_MAIN_MODEL) ){
		//•`‰æ
		ObjDrawActionSummary( obj_work );
	}

	//---------------------------------------
	//ƒTƒuƒp[ƒc
	//---------------------------------------	
	if ( obj_work->user_flag & GMD_DECO_USER_FLAG_SUB_MODEL ){
		u32 disp_flag = obj_work->disp_flag;
		if ( ObjObjectPauseCheck(0) ){
			disp_flag |= OBD_DISP_NOUPDATE;
		}

		VecFx32 pos_sub_under = obj_work->pos;
		pos_sub_under.z = GMD_OBJ_DEFAULT_POS_Z_M1_BACK;
		
		GMS_DECO_SUBMODEL_WORK* deco_submodel_work = (GMS_DECO_SUBMODEL_WORK*)obj_work;

		//ƒJƒƒ‰İ’è
#if _IPHONE
		if ( deco_submodel_work->obj_3d_sub.command_state != OBD_DRAW_CMD_STATE_3DNN_PRE ){
#else
		if ( deco_submodel_work->obj_3d_sub.command_state != OBD_DRAW_CMD_STATE_3DNN ){
#endif // _IPHONE
			ObjDraw3DNNSetCameraEx(GME_CAMERA_NO_MAIN, NNE_PROJECTION_TYPE_ORTHO, deco_submodel_work->obj_3d_sub.command_state );
		}
		ObjDrawAction3DNN(
				&deco_submodel_work->obj_3d_sub,
				&pos_sub_under, 
				&obj_work->dir, 
				&obj_work->scale, 
				&disp_flag );
	}
}

// ==========================================================================
// gmDecoDrawFinalShutter5Line
/*!
 *	ƒtƒ@ƒCƒiƒ‹ƒ][ƒ“—pƒVƒƒƒbƒ^[‘•ü•`‰æi5’i—pj
 *
 *	@param	obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmDecoDrawFinalShutter5Line( OBS_OBJECT_WORK* obj_work )
{
	GMS_DECO_WORK* deco_work = (GMS_DECO_WORK*)obj_work;
	u32 disp_flag = obj_work->disp_flag;
	disp_flag |= OBD_DISP_NOUPDATE;	//XV‚Í‰º‘¤ƒp[ƒc‚ªs‚Á‚Ä‚¢‚é‚Ì‚Ås‚í‚È‚¢
	
#if GMD_DECO_USE_DRAW_TVX
	if (!GmMainIsDrawEnable()) {
		return;
	}
	if (disp_flag & OBD_DISP_NODISP) {
		return;
	}
	// ê—pƒeƒNƒXƒ`ƒƒ‚ª‚ ‚é = TVXƒtƒ@ƒCƒ‹•`‰æ
	//	•`‰æƒT[ƒo‚Ö‚Ì“o˜^ˆ—
	//---------------------------------------
	//ƒƒCƒ“ƒp[ƒc
	//---------------------------------------
	if (!(obj_work->user_flag & GMD_DECO_USER_FLAG_NO_MAIN_MODEL)){
		gmDecoSetDrawPrimitive(deco_work->model_index,
							   deco_work->model_tex,
							   &obj_work->pos,
							   (Float)obj_work->user_timer,
							   deco_work->obj_3d.command_state,
							   disp_flag);
	}
	//---------------------------------------
	//ƒTƒuƒp[ƒc
	//---------------------------------------
	if (obj_work->user_flag & GMD_DECO_USER_FLAG_SUB_MODEL){
		VecFx32 pos_sub = obj_work->pos;
		pos_sub.x = obj_work->pos.x;
		pos_sub.y = obj_work->pos.y;
		pos_sub.z = GMD_OBJ_DEFAULT_POS_Z_M3_BACK - GMD_MAP_ADJUST_POS_Z_FINAL_M*FX32_ONE*2; // Z‚ğM3–Ê‚ÌŒã‚ë‚Ö
		GMS_DECO_SUBMODEL_WORK* deco_submodel_work = (GMS_DECO_SUBMODEL_WORK*)deco_work;
		
		gmDecoSetDrawPrimitive(deco_submodel_work->sub_model_index,
							   deco_work->model_tex,
							   &pos_sub,
							   (Float)(-obj_work->user_timer / 2),
							   deco_work->obj_3d.command_state,
							   disp_flag);
	}
#else
	//3’i‚Ì‚à‚Ì‚ğ•`‰æ
	gmDecoDrawFinalShutter3Line(obj_work);

	//---------------------------------------
	//ƒTƒuƒp[ƒc‚ğã‘¤‚É‚à•`‰æ
	//---------------------------------------	
	if ( obj_work->user_flag & GMD_DECO_USER_FLAG_SUB_MODEL ){
		u32 disp_flag = obj_work->disp_flag;
		disp_flag |= OBD_DISP_NOUPDATE;	//XV‚Í‰º‘¤ƒp[ƒc‚ªs‚Á‚Ä‚¢‚é‚Ì‚Ås‚í‚È‚¢

		VecFx32 pos_sub_under = obj_work->pos;
		pos_sub_under.z = GMD_OBJ_DEFAULT_POS_Z_M1_BACK;
		
		GMS_DECO_SUBMODEL_WORK* deco_submodel_work = (GMS_DECO_SUBMODEL_WORK*)obj_work;

		VecFx32 pos_sub_up = obj_work->pos;
		pos_sub_up.y += (fx32)(-(64*6 + 12)*FX32_ONE);
		pos_sub_up.z = GMD_OBJ_DEFAULT_POS_Z_M1_BACK;
		VecFx32 scale = obj_work->scale;
		scale.y *= -1;
		

		//ƒJƒƒ‰İ’è
		if ( deco_submodel_work->obj_3d_sub.command_state != OBD_DRAW_CMD_STATE_3DNN ){
			ObjDraw3DNNSetCameraEx(GME_CAMERA_NO_MAIN, NNE_PROJECTION_TYPE_ORTHO, deco_submodel_work->obj_3d_sub.command_state );
		}
		ObjDrawAction3DNN(
				&deco_submodel_work->obj_3d_sub,
				&pos_sub_up, 
				&obj_work->dir, 
				&scale, 
				&disp_flag );
	}
#endif // GMD_DECO_USE_DRAW_TVX
}

// ==========================================================================
// gmDecoDrawFallFrontUserFunc
/*!
 * ‘•ü•`‰æƒ†[ƒUŠÖ”i‘êj
 *
 */
// ==========================================================================
AMS_RENDER_TARGET* gmDecoDrawFallCopyRenderFront( void )
{
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];

	AMS_RENDER_TARGET* render_target = NULL;

	//ƒ][ƒ“1
	if ( zone_type == GSD_MAIN_ZONE_TYPE_1 ){
		render_target = gmDecoGetRenderWorkFront();
		if ( !render_target ){
			render_target = &_gm_mapFar_render_work;

			//ƒŒƒ“ƒ_ƒ^[ƒQƒbƒg‚ªì¬‚³‚ê‚Ä‚¢‚È‚¢
			if ( render_target->width == 0 ){
				return NULL;
			}
		}
	}
	//ƒ][ƒ“3
	else if ( zone_type == GSD_MAIN_ZONE_TYPE_3 ) {
		//…–Ê‚ÌƒŒƒ“ƒ_ƒ^[ƒQƒbƒg‚ğæ“¾
		render_target = GmWaterSurfaceGetRenderTarget();
		if ( render_target ){
			gmDecoCopySetRenderTargetForFront( render_target );
			return render_target;
		}
		render_target = _am_render_manager.targetp;
		amAssert( render_target );

		if ( render_target == &_gm_mapFar_render_work ){
			render_target = &_am_draw_target;
		}
		else {
			render_target = &_gm_mapFar_render_work;
		}
	}
	else{
		amAssert( FALSE );
		return NULL;
	}

	//ƒeƒNƒXƒ`ƒƒİ’è
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( mgr && !mgr->flag_render_front ) {
		NNS_RGBA_U8 color = g_deco_rendaer_target_color;
		amRenderCopyTarget( render_target, &color );
		gmDecoCopySetRenderTargetForFront( render_target );
	}

	return render_target;
}
AMS_RENDER_TARGET* gmDecoDrawFallCopyRenderBack( void )
{
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];

	AMS_RENDER_TARGET* render_target = gmDecoGetRenderWorkBack();

	//ƒ][ƒ“1
	if ( zone_type == GSD_MAIN_ZONE_TYPE_1 ){
		if ( !render_target ){
			render_target = &_gm_mapFar_render_work;

			//ƒŒƒ“ƒ_ƒ^[ƒQƒbƒg‚ªì¬‚³‚ê‚Ä‚¢‚È‚¢
			if ( render_target->width == 0 ){
				return NULL;
			}
		}
	}
	//ƒ][ƒ“3
	else if ( zone_type == GSD_MAIN_ZONE_TYPE_3 ) {		
		render_target = _am_render_manager.targetp;
		amAssert( render_target );

		if ( render_target == &_gm_mapFar_render_work ){
			render_target = &_am_draw_target;
		}
		else {
			render_target = &_gm_mapFar_render_work;
		}
	}
	else{
		amAssert( FALSE );
		return NULL;
	}

	//ƒeƒNƒXƒ`ƒƒİ’è
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( mgr && !mgr->flag_render_back ) {
		NNS_RGBA_U8 color = g_deco_rendaer_target_color;
		amRenderCopyTarget( render_target, &color );
		gmDecoCopySetRenderTargetForBack( render_target );
	}
	return render_target;
}
void gmDecoDrawFallRender( AMS_RENDER_TARGET* render_target, NNS_MATRIX44* proj_mtx )
{
#if _PC | _XBOX
	Uint32	state[NNE_SAMPLERSTATETYPE_MAX];
	NNS_MATRIX44	mtx;
	nnMakeUnitMatrix(&mtx);
	nnInitMaterialControlUserSamplerDXG20();
	nnGetMaterialControlUserSamplerDefaultStateDXG20(state);
	state[NNE_SAMPLERSTATETYPE_ADDRESSU]	= D3DTADDRESS_CLAMP;
	state[NNE_SAMPLERSTATETYPE_ADDRESSV]	= D3DTADDRESS_CLAMP;
	state[NNE_SAMPLERSTATETYPE_ADDRESSW]	= D3DTADDRESS_CLAMP;
	state[NNE_SAMPLERSTATETYPE_MAGFILTER]	= D3DTEXF_LINEAR;
	state[NNE_SAMPLERSTATETYPE_MINFILTER]	= D3DTEXF_LINEAR;
	state[NNE_SAMPLERSTATETYPE_MIPFILTER]	= D3DTEXF_NONE;
	nnSetMaterialControlUserSamplerDXG20(NNE_USER_SAMPLER_2D_1,
			render_target->texture_color[0], &mtx, state);

	nnSetUserUniformDXG20(0,
				NNM_MTX(*proj_mtx, 0, 0), NNM_MTX(*proj_mtx, 1, 0),
				NNM_MTX(*proj_mtx, 2, 0), NNM_MTX(*proj_mtx, 3, 0));
		nnSetUserUniformDXG20(1,
				NNM_MTX(*proj_mtx, 0, 1), NNM_MTX(*proj_mtx, 1, 1),
				NNM_MTX(*proj_mtx, 2, 1), NNM_MTX(*proj_mtx, 3, 1));
		nnSetUserUniformDXG20(2,
				NNM_MTX(*proj_mtx, 0, 2), NNM_MTX(*proj_mtx, 1, 2),
				NNM_MTX(*proj_mtx, 2, 2), NNM_MTX(*proj_mtx, 3, 2));
		nnSetUserUniformDXG20(3,
				NNM_MTX(*proj_mtx, 0, 3), NNM_MTX(*proj_mtx, 1, 3),
				NNM_MTX(*proj_mtx, 2, 3), NNM_MTX(*proj_mtx, 3, 3));
#elif _PS3

	Uint32	state[NNE_SAMPLERSTATETYPE_MAX];
		NNS_MATRIX44	mtx;

		nnMakeUnitMatrix(&mtx);
		nnInitMaterialControlUserSamplerPS3();
		nnGetMaterialControlUserSamplerDefaultStatePS3(state);
		state[NNE_SAMPLERSTATETYPE_ADDRESSU]	= CELL_GCM_TEXTURE_CLAMP_TO_EDGE;
		state[NNE_SAMPLERSTATETYPE_ADDRESSV]	= CELL_GCM_TEXTURE_CLAMP_TO_EDGE;
		state[NNE_SAMPLERSTATETYPE_ADDRESSW]	= CELL_GCM_TEXTURE_CLAMP_TO_EDGE;
		state[NNE_SAMPLERSTATETYPE_MAGFILTER]	= CELL_GCM_TEXTURE_LINEAR;
		state[NNE_SAMPLERSTATETYPE_MINFILTER]	= CELL_GCM_TEXTURE_LINEAR;
		state[NNE_SAMPLERSTATETYPE_MIPFILTER]	= CELL_GCM_TEXTURE_NEAREST;
		nnSetMaterialControlUserSamplerPS3(NNE_USER_SAMPLER_2D_1,
				render_target->texture_color[0], &mtx, state);

		nnSetUserUniformPS3(0,
				NNM_MTX(*proj_mtx, 0, 0), NNM_MTX(*proj_mtx, 1, 0),
				NNM_MTX(*proj_mtx, 2, 0), NNM_MTX(*proj_mtx, 3, 0));
		nnSetUserUniformPS3(1,
				NNM_MTX(*proj_mtx, 0, 1), NNM_MTX(*proj_mtx, 1, 1),
				NNM_MTX(*proj_mtx, 2, 1), NNM_MTX(*proj_mtx, 3, 1));
		nnSetUserUniformPS3(2,
				NNM_MTX(*proj_mtx, 0, 2), NNM_MTX(*proj_mtx, 1, 2),
				NNM_MTX(*proj_mtx, 2, 2), NNM_MTX(*proj_mtx, 3, 2));
		nnSetUserUniformPS3(3,
				NNM_MTX(*proj_mtx, 0, 3), NNM_MTX(*proj_mtx, 1, 3),
				NNM_MTX(*proj_mtx, 2, 3), NNM_MTX(*proj_mtx, 3, 3));
#elif _WII
	UNREFERENCED_PARAMETER( proj_mtx );
	memcpy(&_am_draw_render_work, render_target, sizeof(AMS_RENDER_TARGET));
#endif
}
void gmDecoDrawFallFrontUserFunc(void *data)
{
	UNREFERENCED_PARAMETER( data );

	NNS_MATRIX44 *proj_mtx = amDrawGetProjectionMatrix();

#if _WII
	if (proj_mtx != NULL){
		memcpy(_am_draw_fall_projmtx, proj_mtx, sizeof(NNS_MATRIX44));
	}
#endif	//_WII
#if !_IPHONE
	AMS_RENDER_TARGET* render_target = gmDecoDrawFallCopyRenderFront();
	if ( render_target ){
		//•`‰æˆ—
		gmDecoDrawFallRender( render_target, proj_mtx );
	}
#endif //!_IPHONE

}
void gmDecoDrawFallBackUserFunc(void *data)
{
	UNREFERENCED_PARAMETER( data );


	NNS_MATRIX44 *proj_mtx = amDrawGetProjectionMatrix();

#if _WII
	if (proj_mtx != NULL){
		memcpy(_am_draw_fall_projmtx, proj_mtx, sizeof(NNS_MATRIX44));
	}
#endif	//_WII

	AMS_RENDER_TARGET* render_target = gmDecoDrawFallCopyRenderBack();
	if ( render_target ){
		//•`‰æˆ—
		gmDecoDrawFallRender( render_target, proj_mtx );
	}
}

void gmDecoDrawFallFront( OBS_OBJECT_WORK* obj_work )
{
#if	GMD_DECO_TEST_FALL
	ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, NNE_PROJECTION_TYPE_ORTHO, obj_work->obj_3d->command_state );
	ObjDraw3DNNUserFunc(gmDecoDrawFallFrontUserFunc, NULL, 0, obj_work->obj_3d->command_state);


	//•`‰æƒtƒ‰ƒO
#if _PC | _XBOX | _PS3
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1;
#elif _WII
	// ƒ}ƒeƒŠƒAƒ‹ƒR[ƒ‹ƒoƒbƒNİ’è
	obj_work->obj_3d->material_cb_func = gmDecoFallMaterialCallback;
	obj_work->obj_3d->material_cb_param = NULL;
#endif
	//obj_work->obj_3d->command_state = OBD_DRAW_CMD_STATE_PRE_WATER;

	//ƒ}ƒeƒŠƒAƒ‹•`‰æ
	gmDecoDraw( obj_work );
#endif	//GMD_DECO_TEST_FALL
}

void gmDecoDrawFallBack( OBS_OBJECT_WORK* obj_work )
{

#if	GMD_DECO_TEST_FALL
	ObjDraw3DNNSetCameraEx( g_obj.glb_camera_id, NNE_PROJECTION_TYPE_ORTHO, obj_work->obj_3d->command_state );
	ObjDraw3DNNUserFunc(gmDecoDrawFallBackUserFunc, NULL, 0, obj_work->obj_3d->command_state);

	//•`‰æƒtƒ‰ƒO
#if _PC | _XBOX | _PS3
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1;
#elif _WII
	// ƒ}ƒeƒŠƒAƒ‹ƒR[ƒ‹ƒoƒbƒNİ’è
	obj_work->obj_3d->material_cb_func = gmDecoFallMaterialCallback;
	obj_work->obj_3d->material_cb_param = NULL;
#endif
	//obj_work->obj_3d->command_state = OBD_DRAW_CMD_STATE_WATER_BACK;

	//ƒ}ƒeƒŠƒAƒ‹•`‰æ
	gmDecoDraw( obj_work );
#endif	//GMD_DECO_TEST_FALL
}

// ==========================================================================
// ‹éŒ`‰Šú‰»
// ==========================================================================


// ==========================================================================
// gmDecoLoadObj

/*!
 * ƒIƒuƒWƒFƒNƒg‚ğ“Ç‚İ‚İ
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param pos_x À•WX
 * @param pos_y À•WY
 * @param main_func ƒƒCƒ“ˆ—ŠÖ”
 * @param move_func ˆÚ“®ˆ—ŠÖ”
 * @param out_func •`‰æŠÖ”
 * @param dest_func I—¹ˆ—ŠÖ”
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
GMS_DECO_WORK* gmDecoLoadObj( 
						  GMS_EVE_RECORD_DECORATE* dec_rec, 
						  u32 work_size,
						  fx32 pos_x, 
						  fx32 pos_y,
						  GMF_DECO_OBJ_FUNC main_func,
						  GMF_DECO_OBJ_FUNC move_func,
						  GMF_DECO_OBJ_FUNC out_func,
						  GSF_TASK_DESTRUCTOR dest_func)
{

	//-------------------------------------------------
	// ƒ[ƒN‰Šú‰»
	//-------------------------------------------------
	GMS_DECO_WORK* deco_work = (GMS_DECO_WORK*)OBM_OBJECT_TASK_DETAIL_INIT(
		GMD_TASK_PRIO_DECORATION, 
		GMD_TASK_GROUP_DECO_SYS, 
		GMD_TASK_PAUSELEVEL_DEF,//pause_level
		GMD_TASK_PAUSELEVEL_DEF,//obj_pause_level
		work_size,
		"DECO OBJ");
	amAssert( deco_work );

	//ƒ[ƒNİ’è
	OBS_OBJECT_WORK* obj_work = &deco_work->obj_work;
	amAssert( obj_work );

	//TCBƒfƒXƒgƒ‰ƒNƒ^İ’è
	mtTaskChangeTcbDestructor( obj_work->tcb, dest_func );

	//----------------------------------------------------------
	//OBJƒ[ƒNİ’è
	//----------------------------------------------------------
	//ƒ^ƒCƒv
	obj_work->obj_type	= GMD_OBJTYPE_DECORATION;

	// À•W
	obj_work->pos.x	= pos_x + g_gm_deco_pos[dec_rec->id][MTD_X];
	obj_work->pos.y	= pos_y + g_gm_deco_pos[dec_rec->id][MTD_Y];
	obj_work->pos.z	= g_gm_deco_pos[dec_rec->id][MTD_Z];

	//‰ñ“]
	obj_work->dir.z = g_gm_deco_rot_z[dec_rec->id];

	//ƒtƒ‰ƒO
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	//ˆÚ“®–³‚µA’nŒ`‚ ‚½‚è–³‚µ

	//ˆ—ŠÖ”“o˜^
	obj_work->ppFunc = main_func;
	obj_work->ppMove = move_func;
	obj_work->ppOut = out_func;

	//‰æ–ÊŠOÁ‹
	obj_work->view_out_ofst = GMM_EVENT_DECO_VIEW_OUT_OFST(dec_rec->id);
	obj_work->ppViewCheck = ObjObjectViewOutCheck;

	//----------------------------------------------------------
	//‘•üƒ[ƒNİ’è
	//----------------------------------------------------------
	// ƒŒƒR[ƒhİ’è
	deco_work->event_record	= dec_rec;
	deco_work->event_x = dec_rec->pos_x;	//À•W‚ğ‘Ò”ğ
	//ƒŒƒR[ƒh‚ÌƒCƒxƒ“ƒg¶¬‚ğƒXƒLƒbƒv‚É•ÏX
	dec_rec->pos_x = GMD_EVE_RECORD_CMD_SKIP;

	return deco_work;
}

// ==========================================================================
// gmDecoLoadModel
/*!
 * ƒIƒuƒWƒFƒNƒg‚ğ“Ç‚İ‚İ
 *
 * @param deco_work ‘•üƒ[ƒN
 * @param obj_3d_work ƒIƒuƒWƒFƒNƒg3Dƒ‚ƒfƒ‹•\¦ƒ[ƒN
 *
 * @return ‘•üƒ[ƒN
 *
 */
// ==========================================================================
GMS_DECO_WORK* gmDecoLoadModel(
							GMS_DECO_WORK* deco_work,
							OBS_ACTION3D_NN_WORK* obj_3d_work)
{
	amAssert( deco_work );
	amAssert( obj_3d_work );

	//ƒ[ƒNİ’è
	OBS_OBJECT_WORK* obj_work = &deco_work->obj_work;
	amAssert( obj_work );

	//ƒ‚ƒfƒ‹“Ç‚İ‚İ
	ObjObjectCopyAction3dNNModel(
		obj_work,
		obj_3d_work,
		&deco_work->obj_3d);

	// •\— ”½“]‘Î‰
	deco_work->obj_3d.drawflag |= NND_DRAWOBJ_DOUBLESIDE;
	return deco_work;
}

// ==========================================================================
// gmDecoLoadMotion
/*!
 * ƒIƒuƒWƒFƒNƒg‚Ìƒ‚[ƒVƒ‡ƒ“‚ğ“Ç‚İ‚İ
 *
 * @param deco_work ‘•üƒ[ƒN
 * @param motion_index ƒ‚[ƒVƒ‡ƒ“‚Ìƒf[ƒ^ƒCƒ“ƒfƒNƒX
 * @param motion_amb ƒ‚[ƒVƒ‡ƒ“amb
 */
// ==========================================================================
void gmDecoLoadMotion( 
					  GMS_DECO_WORK* deco_work,
					  s32 motion_index,
					  AMS_AMB_HEADER* motion_amb )
{
	amAssert( deco_work );
	amAssert( GMD_DECO_DATA_INDEX_INVALID != motion_index );
	amAssert( motion_amb );
	

	//ƒ[ƒNİ’è
	OBS_OBJECT_WORK* obj_work = &deco_work->obj_work;
	amAssert( obj_work );

	// ƒ‚[ƒVƒ‡ƒ“‰Šú‰»
	ObjObjectAction3dNNMotionLoad(
			obj_work,
			0,				//reg_file_id
			FALSE,			//marge
			NULL,			//data_work
			NULL,			//mtn_data_path
			motion_index,
			motion_amb);
}

// ==========================================================================
// gmDecoLoadMatMotion
/*!
 * ƒIƒuƒWƒFƒNƒg‚Ìƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“‚ğ“Ç‚İ‚İ
 *
 * @param deco_work ‘•üƒ[ƒN
 * @param motion_index ƒ‚[ƒVƒ‡ƒ“‚Ìƒf[ƒ^ƒCƒ“ƒfƒNƒX
 * @param motion_amb ƒ‚[ƒVƒ‡ƒ“amb
 */
// ==========================================================================
void gmDecoLoadMatMotion( 
					  GMS_DECO_WORK* deco_work,
					  s32 motion_index,
					  AMS_AMB_HEADER* motion_amb )
{
	amAssert( deco_work );
	amAssert( GMD_DECO_DATA_INDEX_INVALID != motion_index );
	amAssert( motion_amb );
	

	//ƒ[ƒNİ’è
	OBS_OBJECT_WORK* obj_work = &deco_work->obj_work;
	amAssert( obj_work );

	//ƒ‚[ƒVƒ‡ƒ“
	ObjObjectAction3dNNMaterialMotionLoad( 
			obj_work,
			0,				//reg_file_id
			NULL,			//data_work
			NULL,			//mtn_data_path
			motion_index,
			motion_amb
	);
}

// ==========================================================================
// gmDecoSetRect
/*!
 * ƒIƒuƒWƒFƒNƒg‚Ìƒ‚[ƒVƒ‡ƒ“‚ğ“Ç‚İ‚İ
 *
 * @param deco_work ‘•üƒ[ƒN
  @param left ¶’[
  @param top ã’[
  @param back ‰œ’[
  @param right ‰E’[
  @param bottom ‰º’[
  @param front ‘O’[
  @param func ‹éŒ`ˆ—ŠÖ”
 */
// ==========================================================================
void gmDecoSetRect( 
				   GMS_DECO_WORK* deco_work,
				   s16 left,
				   s16 top,
				   s16 back,
				   s16 right,
				   s16 bottom,
				   s16 front,
				   GMF_DECO_RECT_FUNC func
				   )
{
	amAssert( deco_work );

	OBS_OBJECT_WORK *obj_work = &deco_work->obj_work;
	amAssert( obj_work );

	OBS_RECT_WORK* rect_work = &deco_work->rect_work[GMD_DECO_RECT_BODY];

	// ‹éŒ`İ’è
	ObjObjectGetRectBuf( obj_work, rect_work, 1 );	// ©“®“o˜^ƒoƒbƒtƒ@‚Éİ’è
	ObjRectGroupSet(rect_work, GMD_OBJ_RECT_GROUP_ENEMY, GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER);
	ObjRectAtkSet(rect_work, 0, 0);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	rect_work->parent_obj = obj_work;
	rect_work->ppDef = func;
	rect_work->ppHit = NULL;
	rect_work->flag |= OBD_RECT_NODAMAGE | OBD_RECT_OUT;

	ObjRectWorkZSet( rect_work, left, top, back, right, bottom, front );
}

// ==========================================================================
// ƒƒCƒ“ˆ—
// ==========================================================================

// ==========================================================================
// gmDecoMainFuncMotionCount
/*!
 *	w’è‰ñ”ƒ‚[ƒVƒ‡ƒ“‚ğÄ¶
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmDecoMainFuncMotionCount( OBS_OBJECT_WORK *obj_work )
{	
	--obj_work->user_timer;

	//I—¹
	if ( 0 < obj_work->user_timer ){
		return;
	}
	obj_work->obj_3d->frame[0] = 0;
	obj_work->user_timer = 0;
	obj_work->disp_flag |= OBD_DISP_STOP;
	obj_work->ppFunc = NULL;
}

// ==========================================================================
// gmDecoMainFuncDecreaseMotionSpeed
/*!
 *	ƒ‚[ƒVƒ‡ƒ“‚ğ‘¬“x‚ğ–ß‚·
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmDecoMainFuncDecreaseMotionSpeed( OBS_OBJECT_WORK *obj_work )
{	
	--obj_work->user_timer;
	if ( obj_work->user_timer > 0 ){ 
		//Œ¸Š
		Float frame = (Float)(obj_work->user_timer);
		Float motion_speed = obj_work->obj_3d->speed[0]/frame;
		obj_work->obj_3d->speed[0] -= motion_speed;
		obj_work->obj_3d->frame[0] += obj_work->obj_3d->speed[0];
		Float frame_start = amMotionGetStartFrame( obj_work->obj_3d->motion, obj_work->obj_3d->act_id[0] );
		Float frame_end = amMotionGetEndFrame( obj_work->obj_3d->motion, obj_work->obj_3d->act_id[0] );

		if ( obj_work->obj_3d->frame[0] < frame_start ){
			obj_work->obj_3d->frame[0] = frame_end;
		}
		else if (obj_work->obj_3d->frame[0] > frame_end){
			obj_work->obj_3d->frame[0] = frame_start;
		}
		return ;
	}
	obj_work->user_timer = 0;
	obj_work->obj_3d->speed[0] = 0.0f;

	//I—¹
	obj_work->ppFunc = NULL;
}

// ==========================================================================
// gmDecoMainFuncMotionApplyCommonFrame
/*!
 *	‹¤’Êƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€‚ğ”½‰f
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmDecoMainFuncMotionApplyCommonFrame( OBS_OBJECT_WORK* obj_work )
{

	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( !mgr ){
		 return;
	}
	
	OBS_ACTION3D_NN_WORK* obj_3d_list[2] = {NULL,NULL};
	obj_3d_list[0] = obj_work->obj_3d;
	if ( obj_work->user_flag & GMD_DECO_USER_FLAG_SUB_MODEL ){
		GMS_DECO_SUBMODEL_WORK* deco_submodel_work = (GMS_DECO_SUBMODEL_WORK*)obj_work;
		obj_3d_list[1] = &deco_submodel_work->obj_3d_sub;
	}

	s32 offset_motion = obj_work->user_work;

	s32 frame_index = 0;
	if ( obj_work->user_flag & GMD_DECO_USER_FLAG_SECOND_FRAME ){
		frame_index = 1;
	}

	for ( s32 i = 0; 2 > i; ++i ){
		OBS_ACTION3D_NN_WORK* obj_3d = obj_3d_list[i];
		if ( !obj_3d || !obj_3d->motion  ){
			continue;
		}
		//ƒm[ƒhƒ‚[ƒVƒ‡ƒ“
		if ( obj_3d->motion->mtnbuf[0] ){
			obj_3d->frame[0] = (Float)(mgr->common_frame_motion[frame_index] + offset_motion);

			Float frame_start = amMotionGetStartFrame( obj_3d->motion, obj_3d->act_id[0] );
			Float frame_end = amMotionGetEndFrame( obj_3d->motion, obj_3d->act_id[0] );
			if ( obj_3d->frame[0] < frame_start ){
				obj_3d->frame[0] = frame_start;
			}
			else if (obj_3d->frame[0] >= frame_end){
				obj_3d->frame[0] = frame_end-1;
			}
		}
		//ƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“
		if ( obj_3d->motion->mmobject ){
			obj_3d->mat_frame = (Float)(mgr->common_frame_motion[frame_index] + offset_motion);

			Float frame_start = amMotionMaterialGetStartFrame( obj_3d->motion, obj_3d->mat_act_id );
			Float frame_end = amMotionMaterialGetEndFrame( obj_3d->motion, obj_3d->mat_act_id );
			if ( obj_3d->mat_frame < frame_start ){
				obj_3d->mat_frame = frame_start;
			}
			else if (obj_3d->mat_frame >= frame_end){
				obj_3d->mat_frame = frame_end-1;
			}
		}

	}
}

// ==========================================================================
// gmDecoMainFuncMotionCheckCommonFrame
/*!
 *	‹¤’Êƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€‚ªXV‚³‚ê‚é‚Ì‚ğ‘Ò‚Á‚ÄAƒ‚[ƒVƒ‡ƒ“Ä¶
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmDecoMainFuncMotionCheckCommonFrame( OBS_OBJECT_WORK* obj_work )
{
	//ƒXƒ^[ƒgƒfƒ‚’†‚Í“®‚©‚È‚¢
	if ( g_gm_main_system.game_flag & GMD_GAME_FLAG_START_DEMO ){
		return;
	}

	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( !mgr ){
		 return;
	}
#if !GMD_DECO_USE_DRAW_TVX
	OBS_ACTION3D_NN_WORK* obj_3d_list[2] = {NULL,NULL};
	obj_3d_list[0] = obj_work->obj_3d;
	if ( obj_work->user_flag & GMD_DECO_USER_FLAG_SUB_MODEL ){
		GMS_DECO_SUBMODEL_WORK* deco_submodel_work = (GMS_DECO_SUBMODEL_WORK*)obj_work;
		obj_3d_list[1] = &deco_submodel_work->obj_3d_sub;
	}
#endif // !GMD_DECO_USE_DRAW_TVX
	//ƒ‚[ƒVƒ‡ƒ“Ä¶
	if ( obj_work->user_timer < (u8)(-1*2) ){
		++obj_work->user_timer;
		obj_work->disp_flag &= ~OBD_DISP_STOP;
	}

	s32 frame_index = 0;
	if ( obj_work->user_flag & GMD_DECO_USER_FLAG_SECOND_FRAME ){
		frame_index = 1;
	}
	//‹¤’ÊƒtƒŒ[ƒ€‚ªİ’è‚³‚ê‚Ä‚¢‚È‚¢ê‡
	if ( 0 == mgr->common_frame_motion[frame_index] ){
		//ƒNƒŠƒbƒv—LŒø
		obj_work->flag &= ~OBD_OBJECT_NOCLIP;

		//ƒ‚[ƒVƒ‡ƒ“‚³‚¹‚È‚¢
		obj_work->user_timer = 0;
	}
	//‹¤’ÊƒtƒŒ[ƒ€‚ªİ’è‚³‚ê‚Ä‚¢‚éê‡
	else{
		//ƒNƒŠƒbƒv–³Œø
		obj_work->flag |= OBD_OBJECT_NOCLIP;
	}	
#if !GMD_DECO_USE_DRAW_TVX
	for ( s32 i = 0; 2 > i; ++i ){
		OBS_ACTION3D_NN_WORK* obj_3d = obj_3d_list[i];
		if ( !obj_3d || !obj_3d->motion ){
			continue;
		}
		//ƒm[ƒhƒ‚[ƒVƒ‡ƒ“
		if ( obj_3d->motion->mtnbuf[0] ){
			obj_3d->frame[0] = (Float)obj_work->user_timer;

			Float frame_start = amMotionGetStartFrame( obj_3d->motion, obj_3d->act_id[0] );
			Float frame_end = amMotionGetEndFrame( obj_3d->motion, obj_3d->act_id[0] );
			if ( obj_3d->frame[0] < frame_start ){
				obj_3d->frame[0] = frame_start;
			}
			else if (obj_3d->frame[0] >= frame_end){
				obj_3d->frame[0] = frame_end-1;
			}
		}
		//ƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“
		if ( obj_3d->motion->mmobject ){
			obj_3d->mat_frame = (Float)obj_work->user_timer;

			Float frame_start = amMotionMaterialGetStartFrame( obj_3d->motion, obj_3d->mat_act_id );
			Float frame_end = amMotionMaterialGetEndFrame( obj_3d->motion, obj_3d->mat_act_id );
			if ( obj_3d->mat_frame < frame_start ){
				obj_3d->mat_frame = frame_start;
			}
			else if (obj_3d->mat_frame >= frame_end){
				obj_3d->mat_frame = frame_end-1;
			}
		}
	}
#endif // !GMD_DECO_USE_DRAW_TVX
}

// ==========================================================================
// gmDecoMainFuncLoop
/*!
 *	ƒ‹[ƒv—p
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmDecoMainFuncLoop( OBS_OBJECT_WORK* obj_work )
{

	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( !mgr ){
		 return;
	}

	//‹¤’ÊƒtƒŒ[ƒ€ŠÇ——pƒCƒ“ƒfƒNƒXİ’è
	s32 frame_index = 0;
	if ( obj_work->user_flag & GMD_DECO_USER_FLAG_SECOND_FRAME ){
		frame_index = 1;
	}

	//ƒ‹[ƒv‘Ò‚¿‚Ìê‡
	if (mgr->state_loop == GMD_DECO_LOOP_STATE_WAIT){
		//Á‚³‚È‚¢
		obj_work->flag |= OBD_OBJECT_NOCLIP;
	}
	//ƒ‹[ƒv’†‚Ìê‡
	else if ( mgr->state_loop == GMD_DECO_LOOP_STATE_LOOP ){
		//ƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€İ’è
		gmDecoMainFuncMotionCheckCommonFrame(obj_work);

		//ƒ‹[ƒv‚Ì‚½‚ß‚ÉÁ‚·
		obj_work->flag &= ~OBD_OBJECT_NOCLIP;
	}
	//ƒ‹[ƒvI—¹
	else{
		//ƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€İ’è
		gmDecoMainFuncMotionCheckCommonFrame(obj_work);
	}	
	

	//‹¤’ÊƒtƒŒ[ƒ€XV
	//Œ»İ“®ì’†‚Ì‚à‚Ì‚ÅÅ‚ài‚ñ‚Å‚¢‚È‚¢‚à‚Ì‚ğİ’èB
	//V‹Kì¬•ª‚Æ‘¼‚Ì•¨‚Æ‚ÌƒtƒŒ[ƒ€isŠÔŠu‚ğ‡‚í‚¹‚é‚½‚ß
	if ( 0 != obj_work->user_timer ){
		if ( 0 != mgr->common_frame_motion[frame_index]
				&& mgr->common_frame_motion[frame_index]+1 >= obj_work->user_timer
		){
			mgr->common_frame_motion[frame_index] = obj_work->user_timer;
		}
	}

	const GMS_DECO_WORK* deco_work = (const GMS_DECO_WORK*)obj_work;
	GME_DECORATE_ID deco_id = (GME_DECORATE_ID)deco_work->event_record->id;

	//‰¹‚ª–Â‚Á‚Ä‚¢‚é
	if ( mgr->se_handle ){
		//‹¤—pƒ‹[ƒvSE’â~”»’è
		if ( mgr->common_frame_motion[frame_index] > (s32)obj_work->user_work ){
			GmSoundStopSE( mgr->se_handle );

			//ˆê“x~‚ß‚½‚ç–Â‚ç‚È‚¢
			GsSoundFreeSeHandle( mgr->se_handle );	
			mgr->se_handle = NULL;
		}
	}
	//‰¹‚ª–Â‚Á‚Ä‚¢‚È‚¢
	else {
		//‹¤—pƒ‹[ƒvSEÄ¶”»’è
		if ( mgr->state_loop == GMD_DECO_LOOP_STATE_LOOP 
				&& mgr->common_frame_motion[frame_index] < (s32)obj_work->user_work
		){
			if ( deco_id ){
				mgr->se_handle = GsSoundAllocSeHandle(); 
				switch ( deco_id ){
				case GMD_DECORATE_ID_SHUTTER_LOOP:	//ZONEF ƒVƒƒƒbƒ^[
					GmSoundPlaySE("Shutter1", mgr->se_handle);
					break;
				default:
					break;
				}
			}
		}
	}

	//’â~SE
	if ( obj_work->user_timer == (s32)obj_work->user_work ){
		if ( deco_id ){
			switch ( deco_id ){
			case GMD_DECORATE_ID_SHUTTER_LOOP:	//ZONEF ƒVƒƒƒbƒ^[
				GmSoundPlaySE("Shutter2");
				break;
			default:
				break;
			}
		}
	}

}

// ==========================================================================
// gmDecoMainFuncMotionCheckCommonFrame
/*!
 *	‹¤’Êƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€‚ªXV‚³‚ê‚é‚Ì‚ğ‘Ò‚Á‚ÄAƒ‚[ƒVƒ‡ƒ“Ä¶
 *
 *	@param obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmDecoMainFuncEffectCheckCommonFrame( OBS_OBJECT_WORK *obj_work )
{
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( !mgr ){
		 return;
	}

	//‹¤’ÊƒtƒŒ[ƒ€æ“¾
	s32 frame_index = 0;
	if ( obj_work->user_flag & GMD_DECO_USER_FLAG_SECOND_FRAME ){
		frame_index = 1;
	}
	else if ( obj_work->user_flag & GMD_DECO_USER_FLAG_THIRD_FRAME ){
		frame_index = 2;
	}

	//‹¤’ÊƒtƒŒ[ƒ€‚ªİ’è‚³‚ê‚Ä‚¢‚È‚¢ê‡A”ñ•\¦
	if ( 0 == mgr->common_frame_motion[frame_index] ){
		obj_work->disp_flag |= OBD_DISP_NOUPDATE | OBD_DISP_NODISP;
	}
	//İ’è‚³‚ê‚Ä‚¢‚éê‡•\¦
	else {
		obj_work->disp_flag &= ~(OBD_DISP_NOUPDATE | OBD_DISP_NODISP);
	}
}


// ==========================================================================
// I—¹ˆ—
// ==========================================================================

// ==========================================================================
// gmDecoTcbDest
/*!
 * OBJƒfƒXƒgƒ‰ƒNƒ^
 *
 * @param tcb TCB
 */
// ==========================================================================
void gmDecoTcbDest( MTS_TASK_TCB *tcb )
{
	GMS_DECO_WORK* deco_work = (GMS_DECO_WORK*)mtTaskGetTcbWork(tcb);

#if GMD_DECO_USE_DRAW_SERVER
	// ƒCƒxƒ“ƒgíœ
	gmDecoDelFallEvent(deco_work->event_record, deco_work->obj_work.pos.x + deco_work->obj_work.ofst.x);
#endif // GMD_DECO_USE_DRAW_SERVER

	//ƒTƒuƒ‚ƒfƒ‹‰ğ•ú
	if ( deco_work->obj_work.user_flag & GMD_DECO_USER_FLAG_SUB_MODEL ){
#if GMD_DECO_USE_DRAW_TVX
		if (!deco_work->model_tex) {
			// ƒeƒNƒXƒ`ƒƒ‚ª‚È‚¢ê‡‚Íƒ‚ƒfƒ‹‚Å•`‰æ‚µ‚Ä‚¢‚é‚Ì‚ÅŠJ•ú
#endif // GMD_DECO_USE_DRAW_TVX
			GMS_DECO_SUBMODEL_WORK* deco_submodel_work = (GMS_DECO_SUBMODEL_WORK*)deco_work;
			//ƒ‚[ƒVƒ‡ƒ“‰ğ•ú
			ObjAction3dNNMotionRelease(	&deco_submodel_work->obj_3d_sub );
#if GMD_DECO_USE_DRAW_TVX
		}
#endif // GMD_DECO_USE_DRAW_TVX
	}

	// ƒŒƒR[ƒhİ’è
	GMS_EVE_RECORD_DECORATE* dec_rec = deco_work->event_record;
	if ( dec_rec ){
		if ( dec_rec->pos_x == GMD_EVE_RECORD_CMD_SKIP ){
			dec_rec->pos_x = deco_work->event_x;
			deco_work->event_x = 0;
		}
		deco_work->event_record = NULL;
	}

	//ƒIƒuƒWƒFƒNƒg‰ğ•ú
	ObjObjectExit( tcb );
}

// ==========================================================================
// ‹éŒ`ˆ—
// ==========================================================================

// ==========================================================================
// gmDecoRectFuncChangeMotionCount
/*!
 * user_timer‚Éİ’è‚³‚ê‚Ä‚¢‚éƒ‚[ƒVƒ‡ƒ“‘¬“x‚É•ÏX
 *
 * @param own_rect_work ©g‚Ì‹éŒ`ƒ[ƒN
 * @param target_rect_work ‘Šè‚Ì‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmDecoRectFuncChangeMotionCount( OBS_RECT_WORK* own_rect_work, OBS_RECT_WORK* target_rect_work )
{
	UNREFERENCED_PARAMETER( target_rect_work );

	OBS_OBJECT_WORK* obj_work_gimmick = own_rect_work->parent_obj; 
	amAssert( obj_work_gimmick );

	//ŠÔ‚ğİ’è
	GMS_DECO_WORK* deco_work = (GMS_DECO_WORK*)obj_work_gimmick;
	u32 deco_id = deco_work->event_record->id;
	obj_work_gimmick->user_timer = g_gm_deco_user_timer[deco_id];

	//ƒ‚[ƒVƒ‡ƒ“‘¬“x‚ğİ’è
	Float frame_start = amMotionGetStartFrame( obj_work_gimmick->obj_3d->motion, obj_work_gimmick->obj_3d->act_id[0] );
	Float frame_end = amMotionGetEndFrame( obj_work_gimmick->obj_3d->motion, obj_work_gimmick->obj_3d->act_id[0] );
	Float length = (frame_end - frame_start)*obj_work_gimmick->user_work;
	obj_work_gimmick->obj_3d->speed[0] = length/obj_work_gimmick->user_timer;
	obj_work_gimmick->disp_flag &= ~OBD_DISP_STOP;

	//ƒ‚[ƒVƒ‡ƒ“Ä¶İ’è
	obj_work_gimmick->ppFunc = gmDecoMainFuncMotionCount;
	
}

// ==========================================================================
// gmDecoRectFuncChangeDecreaseMotionSpeed
/*!
 * ƒvƒŒƒCƒ„‘¬“x‚É‡‚í‚¹‚ÄÄ¶‘¬“x‚ğİ’è‚µA™X‚ÉŒ¸Š
 *
 * @param own_rect_work ©g‚Ì‹éŒ`ƒ[ƒN
 * @param target_rect_work ‘Šè‚Ì‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmDecoRectFuncChangeDecreaseMotionSpeed( OBS_RECT_WORK* own_rect_work, OBS_RECT_WORK* target_rect_work )
{
	OBS_OBJECT_WORK* obj_work_gimmick = own_rect_work->parent_obj;
	amAssert( obj_work_gimmick );
	OBS_OBJECT_WORK* obj_work_target = target_rect_work->parent_obj;
	amAssert( obj_work_target );

	//ƒ‚[ƒVƒ‡ƒ“‘¬“x‚ğİ’è
	Float motion_speed = 0.0f;
	if ( obj_work_target->move_flag & OBD_MOVE_NOSPDM ){
		motion_speed = FX_FX32_TO_F32( obj_work_target->spd.x );
	}
	else{
		motion_speed = FX_FX32_TO_F32( obj_work_target->spd_m );
	}
	motion_speed = (Float)(motion_speed * (Float)obj_work_gimmick->user_work/10.0f);
	obj_work_gimmick->obj_3d->speed[0] = motion_speed;

	//‹tÄ¶ƒ`ƒFƒbƒN
	if ( !(obj_work_gimmick->user_flag & GMD_DECO_USER_FLAG_TOUCH_REVERSE_MOTION) ){
		obj_work_gimmick->obj_3d->speed[0] = MTM_MATH_ABS(obj_work_gimmick->obj_3d->speed[0]);
	}

	//ƒƒCƒ“ŠÖ”‚ğŒ¸Š‚É
	obj_work_gimmick->ppFunc = gmDecoMainFuncDecreaseMotionSpeed;

	//Œ¸ŠŠÔ‚ğİ’è
	GMS_DECO_WORK* deco_work = (GMS_DECO_WORK*)obj_work_gimmick;
	u32 deco_id = deco_work->event_record->id;
	obj_work_gimmick->user_timer = g_gm_deco_user_timer[deco_id];

}

// ==========================================================================
// gmDecoRectFuncChangeMotionCommonFrame
/*!
 * ‹¤’Êƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€‚ªXV‚³‚ê‚é‚Ì‚ğ‘Ò‚Á‚ÄAƒ‚[ƒVƒ‡ƒ“Ä¶‚·‚éˆ—‚É•ÏX
 *
 * @param own_rect_work ©g‚Ì‹éŒ`ƒ[ƒN
 * @param target_rect_work ‘Šè‚Ì‹éŒ`ƒ[ƒN
 */
// ==========================================================================
void gmDecoRectFuncChangeMotionCommonFrame( OBS_RECT_WORK* own_rect_work, OBS_RECT_WORK* target_rect_work )
{
	UNREFERENCED_PARAMETER( target_rect_work );

	OBS_OBJECT_WORK* obj_work_gimmick = own_rect_work->parent_obj;
	amAssert( obj_work_gimmick );

	//ƒƒCƒ“ŠÖ”‚ğ•ÏX
	obj_work_gimmick->ppFunc = gmDecoMainFuncMotionCheckCommonFrame;

	//‹éŒ`”»’è‚ğ‚Æ‚ß‚é
	obj_work_gimmick->flag	|= OBD_OBJECT_NOHIT;
}

#if _WII
// ==========================================================================
// gmDecoFallMaterialCallback
/*!
 *	‘•ü ƒ}ƒeƒŠƒAƒ‹ƒR[ƒ‹ƒoƒbƒN(OBF_MATERIAL_CB)
 *
 *	@param	val		[in]	ƒhƒ[ƒR[ƒ‹ƒoƒbƒN•Ï”
 *	@param	param	[in]	ƒ†[ƒU[ƒpƒ‰ƒ[ƒ^
 */
// ==========================================================================
NNE_BOOL gmDecoFallMaterialCallback( NNS_DRAWCALLBACK_VAL *val, void *param )
{
	UNREFERENCED_PARAMETER( param );

	return (amDrawWaterFallMaterial(val));
}
#endif	//_WII


// ==========================================================================
// gmDecoSetLightSpecial
/*!
 * ê—pƒ‰ƒCƒgİ’è
 *
 * @param obj_work ƒIƒuƒWƒFƒNƒgƒ[ƒN
 * @param id ‘•üID
 */
// ==========================================================================
void gmDecoSetLightSpecial( OBS_OBJECT_WORK* obj_work, GME_DECORATE_ID id )
{
#if _IPHONE
	return;
#endif //_IPHONE
	switch ( id ){
	case GMD_DECORATE_ID_SUNFLOWER:	//‚Ğ‚Ü‚í‚è
	case GMD_DECORATE_ID_WALL_A:	//•Ç
	case GMD_DECORATE_ID_WALL_B:	//•Ç
	case GMD_DECORATE_ID_ZONE1_ASIHANA_A:	//ƒAƒVƒnƒi
	case GMD_DECORATE_ID_ZONE1_ASIHANA_B:	//ƒAƒVƒnƒi
	case GMD_DECORATE_ID_ZONE1_ASIHANA_C:	//ƒAƒVƒnƒi
	case GMD_DECORATE_ID_ZONE1_HANA_A:	//‰Ô
	case GMD_DECORATE_ID_ZONE1_HANA_B:	//‰Ô
	case GMD_DECORATE_ID_ZONE1_WOOD_A:	//–Ø
	case GMD_DECORATE_ID_WALL_AA:	//•Ç
	case GMD_DECORATE_ID_WALL_AB:	//•Ç
	case GMD_DECORATE_ID_WALL_AC:	//•Ç
	case GMD_DECORATE_ID_WALL_AD:	//•Ç
	case GMD_DECORATE_ID_WALL_BA:	//•Ç
	case GMD_DECORATE_ID_WALL_A_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_B_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AA_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AB_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AC_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_AD_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_WALL_BA_BACK:	//•Çi‰œj
	case GMD_DECORATE_ID_SUNFLOWER_ENDING:		//‚Ğ‚Ü‚í‚èiƒGƒ“ƒfƒBƒ“ƒOj
	case GMD_DECORATE_ID_ZONE1_HANA_A_ENDING:	//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	case GMD_DECORATE_ID_ZONE1_HANA_B_ENDING:	//‰ÔiƒGƒ“ƒfƒBƒ“ƒOj
	case GMD_DECORATE_ID_ZONE1_WOOD_A_ENDING:	//–ØiƒGƒ“ƒfƒBƒ“ƒOj
		{
			if ( g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_1
					|| g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_2
			){
				obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
				obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_5;
			}
		}
		break;
	default:
		break;
	}
}
	
#if _IPHONE
	
// ==========================================================================
// gmDecoAdjustIPhone
/*!
 * IPhoneİ’è
 *
 * @param obj_work ƒIƒuƒWƒFƒNƒgƒ[ƒN
 * @param id ‘•üID
*/
// ==========================================================================
void gmDecoAdjustIPhone( OBS_OBJECT_WORK* obj_work, GME_DECORATE_ID id )
{
	if ( !obj_work ){
		return;
	}
	
	switch ( id ){
	case GMD_DECORATE_ID_FOUN_B:	//ZONE3 ‚«o‚·…
		{
			float base_pos_y = 0;
			float adjust_size = 0.0f;
			
			switch ( g_gs_main_sys_info.stage_id){
			case GSD_MAIN_STAGE_ID_3_1:
				{
					//“Á’è”ÍˆÍ‚Ì‚à‚Ì
					if ( obj_work->pos.x > FX_F32_TO_FX32(16.0f*64.0f) 
							&& obj_work->pos.x < FX_F32_TO_FX32(20.0f*64.0f) 
					){
						base_pos_y = 576.0f;
						adjust_size = 64.0f-48.0f/5.0f;	//5–‡‚Å48k‚ß‚é
					}
					else if (obj_work->pos.x > FX_F32_TO_FX32(47.0f*64.0f) 
						 	&& obj_work->pos.x < FX_F32_TO_FX32(48.0f*64.0f) 
					){
						base_pos_y = 768.0f;
						adjust_size = 64.0f-48.0f/2.0f;	//2–‡‚Å48k‚ß‚é
					}
					else if (obj_work->pos.x > FX_F32_TO_FX32(107.0f*64.0f) 
						 	&& obj_work->pos.x < FX_F32_TO_FX32(109.0f*64.0f) 
							 && obj_work->pos.y > FX_F32_TO_FX32(1213.0f)
					){
						base_pos_y = 1213.0f;
						adjust_size = 64.0f-12.0f/4.0f;	//4–‡‚Å12k‚ß‚é
					}
					else {
						return;
					}
				}
				break;
			default:
				return;
			}
			
			//ƒXƒP[ƒ‹
			obj_work->scale.y = FX_F32_TO_FX32( adjust_size/64.0f );
			
			//À•W
			float adjust_pos = FX_FX32_TO_F32(obj_work->pos.y) - base_pos_y;
			adjust_pos /= 64.0f;
			obj_work->pos.y = FX_F32_TO_FX32(base_pos_y + adjust_pos * adjust_size);
		}
		break;
	default:
		break;
	}
}
#endif //_IPHONE
	
// ==========================================================================
// ‘•ü‰Šú‰»
// ==========================================================================


// ==========================================================================
// gmDecoInitNomodel
/*!
 * ‘•üì¬iƒIƒuƒWƒFƒNƒgì¬‚Ì‚İj
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
GMS_DECO_WORK* gmDecoInitNomodel( 
								 GMS_EVE_RECORD_DECORATE* dec_rec, 
								 fx32 x, 
								 fx32 y, 
								 u8 type,
								 u32 work_size )
{
	UNREFERENCED_PARAMETER( type );

	amAssert( dec_rec );
	amAssert( work_size >= sizeof(GMS_DECO_WORK) );

	//‘•üID
	GME_DECORATE_ID deco_id = (GME_DECORATE_ID)dec_rec->id;

	//ƒ†[ƒUƒtƒ‰ƒOƒ`ƒFƒbƒNiƒJƒIƒXƒGƒƒ‰ƒ‹ƒh‚ª‚È‚¢ê‡A¶¬‚µ‚È‚¢j
	if ( g_gm_deco_user_flag[ deco_id ] & GMD_DECO_USER_FLAG_CHECK_CHAOS_EMERALD ){
		if ( !(g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD) ){
			dec_rec->pos_x = GMD_EVE_RECORD_CMD_SKIP;
			return NULL;
		}
	}
	
	//----------------------------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	//----------------------------------------------------------
	GMS_DECO_WORK* deco_work = gmDecoLoadObj(
			dec_rec,
			work_size,
			x,
			y,
			g_gm_deco_func_main[deco_id],
			g_gm_deco_func_move[deco_id],
			g_gm_deco_func_out[deco_id],
			g_gm_deco_func_dest[deco_id]);
	amAssert( deco_work );

	//ƒtƒ‰ƒO
	deco_work->obj_work.disp_flag |= g_gm_deco_disp_flag[deco_id];
	deco_work->obj_work.user_work = g_gm_deco_user_work[ deco_id ];
	deco_work->obj_work.user_timer = g_gm_deco_user_timer[ deco_id ];
	deco_work->obj_work.user_flag = g_gm_deco_user_flag[ deco_id ];
	deco_work->obj_work.flag |= OBD_OBJECT_NOHIT;
 
	//¶‰E‘ÎÌˆ—
	if ( deco_work->obj_work.disp_flag & OBD_DISP_HFLIP ){
		deco_work->obj_work.scale.x = -FX32_ONE;
		deco_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
	}

	//ã‰º‘ÎÌˆ—
	if ( deco_work->obj_work.disp_flag & OBD_DISP_VFLIP ){
		deco_work->obj_work.scale.y = -FX32_ONE;
		deco_work->obj_work.disp_flag &= ~OBD_DISP_VFLIP;
	}

	//ƒNƒŠƒbƒv‚µ‚È‚¢
	if ( deco_work->obj_work.user_flag & GMD_DECO_USER_FLAG_NOCLIP ){
		 deco_work->obj_work.flag |= OBD_OBJECT_NOCLIP;
	}


	return deco_work;
}

// ==========================================================================
// gmDecoInitModel
/*!
 * ‘•üì¬iƒ‚ƒfƒ‹‚Ì‚İj
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
GMS_DECO_WORK* gmDecoInitModel( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	amAssert( dec_rec );

	//‘•üID
	GME_DECORATE_ID deco_id = (GME_DECORATE_ID)dec_rec->id;
	
	//----------------------------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	//----------------------------------------------------------
	u32 work_size = 0;
	s32 model_index_sub = GMD_DECO_DATA_INDEX_INVALID;
	if ( g_gm_deco_user_flag[ deco_id ] & GMD_DECO_USER_FLAG_SUB_MODEL ){
		work_size = sizeof(GMS_DECO_SUBMODEL_WORK);
		model_index_sub = g_gm_deco_model_index[deco_id][1];
	}
	else{
		work_size = sizeof(GMS_DECO_WORK);
	}
	GMS_DECO_WORK* deco_work = gmDecoInitNomodel( 
			dec_rec,
			x, 
			y,
			type,
			work_size );
	if ( !deco_work ){
		return NULL;
	}

	OBS_ACTION3D_NN_WORK* obj_3d_list = gmDecoDataGetObj3DList( deco_id );
#if GMD_DECO_USE_DRAW_TVX
	// TVXƒtƒ@ƒCƒ‹g—p
	if (!obj_3d_list) {
		deco_work->model_tex = &(gmDecoDataGetInfo()->tvx_tex);
		
		//----------------------------------------------------------
		//ƒ‚ƒfƒ‹
		//----------------------------------------------------------
		s32 model_index = g_gm_deco_model_index[ deco_id ][0];
		deco_work->obj_3d.command_state = g_gm_deco_command_state[deco_id];
		deco_work->model_index = model_index;
	
		//----------------------------------------------------------
		//ƒTƒuƒ‚ƒfƒ‹
		//----------------------------------------------------------
		if ( model_index_sub != GMD_DECO_DATA_INDEX_INVALID ){
			GMS_DECO_SUBMODEL_WORK* deco_submodel_work = (GMS_DECO_SUBMODEL_WORK*)deco_work;
			deco_submodel_work->obj_3d_sub.command_state = g_gm_deco_command_state[deco_id];
			deco_submodel_work->sub_model_index = model_index_sub;
		} 
		
		//ƒtƒ‰ƒO
		deco_work->obj_work.disp_flag |= g_gm_deco_disp_flag[deco_id];
	}
	// ƒ‚ƒfƒ‹ƒf[ƒ^g—p(’Êí)
	else {
		//----------------------------------------------------------
		//ƒ‚ƒfƒ‹
		//----------------------------------------------------------
		s32 model_index = g_gm_deco_model_index[ deco_id ][0];
		if ( model_index != GMD_DECO_DATA_INDEX_INVALID ){
			gmDecoLoadModel( deco_work, &obj_3d_list[model_index] );
			
			//•`‰æƒRƒ}ƒ“ƒh
			deco_work->obj_3d.command_state = g_gm_deco_command_state[deco_id];
		}
		
		//----------------------------------------------------------
		//ƒTƒuƒ‚ƒfƒ‹
		//----------------------------------------------------------
		if ( model_index_sub != GMD_DECO_DATA_INDEX_INVALID ){
			GMS_DECO_SUBMODEL_WORK* deco_submodel_work = (GMS_DECO_SUBMODEL_WORK*)deco_work;
			ObjCopyAction3dNNModel(
								   &obj_3d_list[model_index_sub],
								   &deco_submodel_work->obj_3d_sub);
			// •\— ”½“]‘Î‰
			deco_submodel_work->obj_3d_sub.drawflag |= NND_DRAWOBJ_DOUBLESIDE;
			
			//ê—pƒ‰ƒCƒg
			if ( g_gm_deco_user_flag[ deco_id ] & GMD_DECO_USER_FLAG_USE_LIGHT_2_SUB_MODEL ){
				deco_submodel_work->obj_3d_sub.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
				deco_submodel_work->obj_3d_sub.use_light_flag |= OBD_LIGHT_USE_FLAG_2;
			}
			
			//•`‰æƒRƒ}ƒ“ƒh
			deco_submodel_work->obj_3d_sub.command_state = g_gm_deco_command_state[deco_id];
		} 
		
		
		//ƒtƒ‰ƒO
		deco_work->obj_work.disp_flag |= g_gm_deco_disp_flag[deco_id];
		
		//¶‰E‘ÎÌˆ—
		if ( deco_work->obj_work.disp_flag & OBD_DISP_HFLIP ){
			deco_work->obj_work.scale.x = -FX32_ONE;
			deco_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
		}
		
		//ã‰º‘ÎÌˆ—
		if ( deco_work->obj_work.disp_flag & OBD_DISP_VFLIP ){
			deco_work->obj_work.scale.y = -FX32_ONE;
			deco_work->obj_work.disp_flag &= ~OBD_DISP_VFLIP;
		}
	}
#else
	amAssert( obj_3d_list );

	//----------------------------------------------------------
	//ƒ‚ƒfƒ‹
	//----------------------------------------------------------
	s32 model_index = g_gm_deco_model_index[ deco_id ][0];
	if ( model_index != GMD_DECO_DATA_INDEX_INVALID ){
		gmDecoLoadModel( deco_work, &obj_3d_list[model_index] );

		//•`‰æƒRƒ}ƒ“ƒh
		deco_work->obj_3d.command_state = g_gm_deco_command_state[deco_id];
	}

	//----------------------------------------------------------
	//ƒTƒuƒ‚ƒfƒ‹
	//----------------------------------------------------------
	if ( model_index_sub != GMD_DECO_DATA_INDEX_INVALID ){
		GMS_DECO_SUBMODEL_WORK* deco_submodel_work = (GMS_DECO_SUBMODEL_WORK*)deco_work;
		ObjCopyAction3dNNModel(
				&obj_3d_list[model_index_sub],
				&deco_submodel_work->obj_3d_sub);
		// •\— ”½“]‘Î‰
		deco_submodel_work->obj_3d_sub.drawflag |= NND_DRAWOBJ_DOUBLESIDE;

		//ê—pƒ‰ƒCƒg
		if ( g_gm_deco_user_flag[ deco_id ] & GMD_DECO_USER_FLAG_USE_LIGHT_2_SUB_MODEL ){
			deco_submodel_work->obj_3d_sub.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
			deco_submodel_work->obj_3d_sub.use_light_flag |= OBD_LIGHT_USE_FLAG_2;
		}

		//•`‰æƒRƒ}ƒ“ƒh
		deco_submodel_work->obj_3d_sub.command_state = g_gm_deco_command_state[deco_id];
	} 


	//ƒtƒ‰ƒO
	deco_work->obj_work.disp_flag |= g_gm_deco_disp_flag[deco_id];

	//¶‰E‘ÎÌˆ—
	if ( deco_work->obj_work.disp_flag & OBD_DISP_HFLIP ){
		deco_work->obj_work.scale.x = -FX32_ONE;
		deco_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
	}

	//ã‰º‘ÎÌˆ—
	if ( deco_work->obj_work.disp_flag & OBD_DISP_VFLIP ){
		deco_work->obj_work.scale.y = -FX32_ONE;
		deco_work->obj_work.disp_flag &= ~OBD_DISP_VFLIP;
	}
#endif // GMD_DECO_USE_DRAW_TVX

	//ê—pƒ‰ƒCƒg
	gmDecoSetLightSpecial( &deco_work->obj_work, deco_id );

	return deco_work;
}

// ==========================================================================
// gmDecoInitModelMotion
/*!
 * ‘•üì¬iƒtƒbƒgƒ[ƒNƒ‚[ƒVƒ‡ƒ“‚ğs‚¤ƒ‚ƒfƒ‹j
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
void gmDecoInitMotion( GMS_DECO_WORK* deco_work )
{
	amAssert( deco_work );

	//‘•üID
	GME_DECORATE_ID deco_id = (GME_DECORATE_ID)deco_work->event_record->id;

	//----------------------------------------------------------
	//ƒ‚[ƒVƒ‡ƒ“
	//----------------------------------------------------------
	AMS_AMB_HEADER* motion_amb = gmDecoDataGetMotionHeader( deco_id );
	amAssert( motion_amb );

	s32 motion_index = g_gm_deco_node_motion_index[deco_id][0];
	if ( motion_index != GMD_DECO_DATA_INDEX_INVALID ){
		gmDecoLoadMotion( deco_work, motion_index, motion_amb );

		//ƒ‚[ƒVƒ‡ƒ“İ’è
		OBS_OBJECT_WORK* obj_work = &deco_work->obj_work;
		amAssert( obj_work );

		//ƒŠƒs[ƒgƒtƒ‰ƒO•Û—piObjDrawObjectActionSet‚Ì’†‚Å‰Šú‰»‚³‚ê‚é‚Ì‚Åj
		u32 disp_flag = obj_work->disp_flag;
		ObjDrawObjectActionSet( obj_work, 0 );
		obj_work->disp_flag = disp_flag;
	}

	//----------------------------------------------------------
	//ƒTƒuƒ‚ƒfƒ‹—pƒ‚[ƒVƒ‡ƒ“
	//----------------------------------------------------------
	if ( g_gm_deco_user_flag[ deco_id ] & GMD_DECO_USER_FLAG_SUB_MODEL ){
		s32 motion_index_sub = g_gm_deco_node_motion_index[deco_id][1];

		if ( motion_index_sub != GMD_DECO_DATA_INDEX_INVALID ){
			GMS_DECO_SUBMODEL_WORK* deco_submodel_work = (GMS_DECO_SUBMODEL_WORK*)deco_work;
			ObjAction3dNNMotionLoad(
					&deco_submodel_work->obj_3d_sub,
					0,
					FALSE,
					NULL, 
					NULL,
					motion_index_sub, 
					motion_amb);
		}
	}
}

GMS_DECO_WORK* gmDecoInitModelMotion( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	amAssert( dec_rec );
	
	//----------------------------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	//----------------------------------------------------------
	GMS_DECO_WORK* deco_work = gmDecoInitModel( dec_rec, x, y, type );
	if ( !deco_work ){
		return NULL;
	}

	//----------------------------------------------------------
	//ƒ‚[ƒVƒ‡ƒ“
	//----------------------------------------------------------
	gmDecoInitMotion( deco_work );

	return deco_work;
}

// ==========================================================================
// gmDecoInitModelMotionTouch
/*!
 * ‘•üì¬iƒvƒŒƒCƒ„‚ªG‚ê‚é‹éŒ`‚ğ‚Âƒ‚ƒfƒ‹i‹éŒ`”ÍˆÍİ’è‚Í–¢À‘•jj
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
GMS_DECO_WORK* gmDecoInitModelMotionTouch( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	amAssert( dec_rec );
	
	//----------------------------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	//----------------------------------------------------------
	GMS_DECO_WORK* deco_work = gmDecoInitModelMotion( dec_rec, x, y, type );
	if ( !deco_work ){
		return NULL;
	}

	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)deco_work;
	obj_work->flag &= ~OBD_OBJECT_NOHIT;

	//----------------------------------------------------------
	//‹éŒ`İ’è
	//----------------------------------------------------------
	GME_DECORATE_ID deco_id = (GME_DECORATE_ID)dec_rec->id;
	s16 width = g_gm_deco_rect_size[deco_id][MTD_X];
	s16 height = g_gm_deco_rect_size[deco_id][MTD_Y];
	s16 left = (s16)(-width/2);
	s16 top = (s16)(-height/2);
	s16 right = (s16)(width/2);
	s16 bottom = (s16)(height/2);
	gmDecoSetRect( 
			deco_work, 
			left, top, -500,
			right, bottom, 500,
			g_gm_deco_func_rect[deco_id] );	//‰¼

	return deco_work;
}

// ==========================================================================
// gmDecoInitModelMaterial
/*!
 * ‘•üì¬iƒ}ƒeƒŠƒAƒ‹‚ğs‚¤ƒ‚ƒfƒ‹j
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
void gmDecoInitMaterial( GMS_DECO_WORK* deco_work )
{
	amAssert( deco_work );

	//‘•üID
	GME_DECORATE_ID deco_id = (GME_DECORATE_ID)deco_work->event_record->id;

	//----------------------------------------------------------
	//ƒ‚[ƒVƒ‡ƒ“
	//----------------------------------------------------------
	AMS_AMB_HEADER* motion_amb = gmDecoDataGetMatMotionHeader( deco_id );
	amAssert( motion_amb );

	s32 motion_index = g_gm_deco_mat_motion_index[deco_id][0];
	if ( motion_index != GMD_DECO_DATA_INDEX_INVALID ){
		gmDecoLoadMatMotion( deco_work, motion_index, motion_amb );

		//ƒ‚[ƒVƒ‡ƒ“İ’è
		OBS_OBJECT_WORK* obj_work = &deco_work->obj_work;
		amAssert( obj_work );

		//ƒŠƒs[ƒgƒtƒ‰ƒO•Û—piObjDrawObjectActionSet3DNNMaterial‚Ì’†‚Å‰Šú‰»‚³‚ê‚é‚Ì‚Åj
		u32 disp_flag = obj_work->disp_flag;
		ObjDrawObjectActionSet3DNNMaterial( obj_work, 0 );
		obj_work->disp_flag = disp_flag;
	}

	//----------------------------------------------------------
	//ƒTƒuƒ‚ƒfƒ‹—pƒ‚[ƒVƒ‡ƒ“
	//----------------------------------------------------------
	if ( g_gm_deco_user_flag[ deco_id ] & GMD_DECO_USER_FLAG_SUB_MODEL ){
		s32 motion_index_sub = g_gm_deco_mat_motion_index[deco_id][1];

		if ( motion_index_sub != GMD_DECO_DATA_INDEX_INVALID ){
			GMS_DECO_SUBMODEL_WORK* deco_submodel_work = (GMS_DECO_SUBMODEL_WORK*)deco_work;
			ObjAction3dNNMaterialMotionLoad(
					&deco_submodel_work->obj_3d_sub,
					0,
					NULL, 
					NULL,
					motion_index_sub, 
					motion_amb);
		}
	}
}
GMS_DECO_WORK* gmDecoInitModelMaterial( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	amAssert( dec_rec );
	
	//----------------------------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	//----------------------------------------------------------
	GMS_DECO_WORK* deco_work = gmDecoInitModel( dec_rec, x, y, type );
	if ( !deco_work ){
		return NULL;
	}

	//----------------------------------------------------------
	//ƒ‚[ƒVƒ‡ƒ“
	//----------------------------------------------------------
	gmDecoInitMaterial( deco_work );

	return deco_work;
}

// ==========================================================================
// gmDecoInitModelMotioinMaterial
/*!
 * ‘•üì¬iƒ}ƒeƒŠƒAƒ‹Aƒ‚[ƒVƒ‡ƒ“‚ğs‚¤ƒ‚ƒfƒ‹j
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
GMS_DECO_WORK* gmDecoInitModelMotioinMaterial( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	amAssert( dec_rec );
	
	//----------------------------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	//----------------------------------------------------------
	GMS_DECO_WORK* deco_work = gmDecoInitModel( dec_rec, x, y, type );
	if ( !deco_work ){
		return NULL;
	}

	//----------------------------------------------------------
	//ƒ‚[ƒVƒ‡ƒ“
	//----------------------------------------------------------
	gmDecoInitMotion( deco_work );

	//----------------------------------------------------------
	//ƒ}ƒeƒŠƒAƒ‹
	//----------------------------------------------------------
	gmDecoInitMaterial( deco_work );

	return deco_work;
}

// ==========================================================================
// gmDecoInitModelMotionMaterialTouch
/*!
 * ‘•üì¬iƒvƒŒƒCƒ„‚ªG‚ê‚é‹éŒ`‚ğ‚Âƒ‚ƒfƒ‹i‹éŒ`”ÍˆÍİ’è‚Í–¢À‘•jj
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
GMS_DECO_WORK* gmDecoInitModelMotionMaterialTouch( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	amAssert( dec_rec );
	
	//----------------------------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	//----------------------------------------------------------
	GMS_DECO_WORK* deco_work = gmDecoInitModelMotioinMaterial( dec_rec, x, y, type );
	if ( !deco_work ){
		return NULL;
	}

	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)deco_work;
	obj_work->flag &= ~OBD_OBJECT_NOHIT;

	//----------------------------------------------------------
	//‹éŒ`İ’è
	//----------------------------------------------------------
	GME_DECORATE_ID deco_id = (GME_DECORATE_ID)dec_rec->id;
	s16 width = g_gm_deco_rect_size[deco_id][MTD_X];
	s16 height = g_gm_deco_rect_size[deco_id][MTD_Y];
	s16 left = (s16)(-width/2);
	s16 top = (s16)(-height/2);
	s16 right = (s16)(width/2);
	s16 bottom = (s16)(height/2);
	gmDecoSetRect( 
			deco_work, 
			left, top, -500,
			right, bottom, 500,
			g_gm_deco_func_rect[deco_id] );	//‰¼

	return deco_work;
}
// ==========================================================================
// gmDecoInitModelEffect
/*!
 * ‘•üì¬iƒ‹[ƒv‚ÌÛ‚Éƒ‚[ƒVƒ‡ƒ“Ä¶ˆÊ’u‚ğ‡‚í‚¹‚éƒ‚ƒfƒ‹j
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
GMS_DECO_WORK* gmDecoInitModelLoop( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	amAssert( dec_rec );
	
	//----------------------------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	//----------------------------------------------------------
	GMS_DECO_WORK* deco_work = gmDecoInitModel( dec_rec, x, y, type );
	if ( !deco_work ){
		return NULL;
	}
#if !GMD_DECO_USE_DRAW_TVX
	//----------------------------------------------------------
	//ƒ‚[ƒVƒ‡ƒ“
	//----------------------------------------------------------
	gmDecoInitMotion( deco_work );

	//----------------------------------------------------------
	//ƒ}ƒeƒŠƒAƒ‹
	//----------------------------------------------------------
	gmDecoInitMaterial( deco_work );
#endif // !GMD_DECO_USE_DRAW_TVX
	//ƒ‚[ƒVƒ‡ƒ“Ä¶ˆÊ’u‚ğİ’è
	GMS_DECO_MGR* mgr = gmDecoGetMgr();
	if ( mgr ){
		//ŠJn‘Ò‚¿
		if ( mgr->state_loop == GMD_DECO_LOOP_STATE_WAIT ){
			gmDecoMainFuncLoop(&deco_work->obj_work);
		}
		//ƒ‹[ƒv
		else if ( mgr->state_loop == GMD_DECO_LOOP_STATE_LOOP ){
			for ( s32 i = 0; GMD_DECO_LOOP_MODEL_NUM > i; ++i ){
				if ( mgr->motion_frame_loop[i] == 0){
					continue;
				}
				deco_work->obj_work.user_timer = mgr->motion_frame_loop[i];
				mgr->motion_frame_loop[i] = 0;
				gmDecoMainFuncLoop(&deco_work->obj_work);
				break;
			}
		}
		//I—¹
		else{
			//¶Œü‚«‚ÍŠJ‚¢‚½ó‘Ô‚©‚çŠJn
			const GMS_PLAYER_WORK* player_work = (const GMS_PLAYER_WORK*)g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
			if ( deco_work->obj_work.pos.x < player_work->obj_work.pos.x ){
				deco_work->obj_work.user_timer = (u8)(-1*2);
			}
			else{
				//‹¤’ÊƒtƒŒ[ƒ€‚©‚çƒIƒtƒZƒbƒg‚Å‰Šú’lİ’è
				s32 frame_index = 0;
				if ( deco_work->obj_work.user_flag & GMD_DECO_USER_FLAG_SECOND_FRAME ){
					frame_index = 1;
				}
				if ( 16 < mgr->common_frame_motion[frame_index] ){
					deco_work->obj_work.user_timer = mgr->common_frame_motion[frame_index] - 16;
				}
			}
			gmDecoMainFuncLoop(&deco_work->obj_work);
		}
	}	

	return deco_work;
}

// ==========================================================================
// gmDecoInitModelEffect
/*!
 * ‘•üì¬iƒTƒuƒ‚ƒfƒ‹‚h‚c‚ğƒGƒtƒFƒNƒg‚Æ‚µ‚Äè‘O‚É•\¦j
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
GMS_DECO_WORK* gmDecoInitModelEffect( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	amAssert( dec_rec );
	
	//----------------------------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	//----------------------------------------------------------
	GMS_DECO_WORK* deco_work = gmDecoInitModel( dec_rec, x, y, type );
	if ( !deco_work ){
		return NULL;
	}

	//----------------------------------------------------------
	//ƒGƒtƒFƒNƒg
	//----------------------------------------------------------
	GME_DECORATE_ID deco_id = (GME_DECORATE_ID)dec_rec->id;
	s32 effect_index = g_gm_deco_model_index[ deco_id ][1];
	amAssert( GMD_DECO_DATA_INDEX_INVALID != effect_index );

	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];

	GMS_EFFECT_3DES_WORK* effect = GmEfctZoneEsCreate(
			&deco_work->obj_work,
			zone_type,
			effect_index );
	effect->efct_com.obj_work.dir.z = g_gm_deco_rot_z[dec_rec->id];

	//ƒ†[ƒUƒ[ƒN‚Ì’l‚ÅƒIƒtƒZƒbƒg
	fx32 effect_x = g_gm_deco_user_work[ deco_id ];
	effect_x = effect_x << 16;
	effect_x = effect_x >> 16;
	fx32 effect_y = g_gm_deco_user_work[ deco_id ];
	effect_y = effect_y >> 16;
	effect->efct_com.obj_work.pos.x = deco_work->obj_work.pos.x + effect_x*FX32_ONE;
	effect->efct_com.obj_work.pos.y = deco_work->obj_work.pos.y + effect_y*FX32_ONE;
	effect->efct_com.obj_work.pos.z = deco_work->obj_work.pos.z + 32*FX32_ONE;

#if _IPHONE
	if (deco_work->obj_3d.command_state == OBD_DRAW_CMD_STATE_3DNN_WS) {
		effect->obj_3des.command_state = OBD_DRAW_CMD_STATE_3DNN_WS;
	}
#endif // _IPHONE

	return deco_work;
}

// ==========================================================================
// gmDecoInitModelMatMotion
/*!
 * ‘•üì¬i3DƒvƒŠƒ~ƒeƒBƒu•`‰æiƒeƒNƒXƒ`ƒƒ¶¬‚È‚Ç‚Í–¢À‘•jj
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
GMS_DECO_WORK* gmDecoInitPrimitive3D( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	amAssert( dec_rec );

	//‘•üID
	GME_DECORATE_ID deco_id = (GME_DECORATE_ID)dec_rec->id;
	
	//----------------------------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	//----------------------------------------------------------
	GMS_DECO_WORK* deco_work = gmDecoInitNomodel( 
			dec_rec,
			x, 
			y,
			type,
			sizeof(GMS_DECO_WORK) );
	if ( !deco_work ){
		return NULL;
	}

	//ƒtƒ‰ƒO
	deco_work->obj_work.disp_flag |= g_gm_deco_disp_flag[deco_id];
	
	//----------------------------------------------------------
	//ƒeƒNƒXƒ`ƒƒì¬
	//----------------------------------------------------------

	return deco_work;
}

// ==========================================================================
// gmDecoInitFall
/*!
 * ‘•üì¬i‘ê•`‰æj
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
GMS_DECO_WORK* gmDecoInitFall( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
#if	GMD_DECO_TEST_FALL
	amAssert( dec_rec );
	
	//----------------------------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	//----------------------------------------------------------
	GMS_DECO_WORK* deco_work = gmDecoInitModelMaterial( dec_rec, x, y, type );
	if ( !deco_work ){
		return NULL;
	}

	// ƒ}ƒeƒŠƒAƒ‹ƒR[ƒ‹ƒoƒbƒNİ’è
	//deco_work->obj_work.obj_3d->material_cb_func = gmDecoFallMaterialCallback;
	//deco_work->obj_work.obj_3d->material_cb_param = NULL;

#else
	GMS_DECO_WORK* deco_work = NULL;
#endif	//GMD_DECO_TEST_FALL
	
#if _IPHONE
	//‘•üID
	GME_DECORATE_ID deco_id = (GME_DECORATE_ID)deco_work->event_record->id;
	gmDecoAdjustIPhone((OBS_OBJECT_WORK*)deco_work,deco_id);
#endif //_IPHONE

	return deco_work;
}

// ==========================================================================
// gmDecoInitEffect
/*!
 * ‘•üì¬iƒGƒtƒFƒNƒgj
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
GMS_DECO_WORK* gmDecoInitEffect( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	amAssert( dec_rec );

	//‘•üID
	GME_DECORATE_ID deco_id = (GME_DECORATE_ID)dec_rec->id;
	
	//----------------------------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	//----------------------------------------------------------
	GMS_DECO_WORK* deco_work = gmDecoInitNomodel( 
			dec_rec,
			x, 
			y,
			type,
			sizeof(GMS_DECO_WORK) );
	if ( !deco_work ){
		return NULL;
	}

	//----------------------------------------------------------
	//ƒGƒtƒFƒNƒg
	//----------------------------------------------------------
	s32 effect_index = g_gm_deco_model_index[ deco_id ][0];
	amAssert( GMD_DECO_DATA_INDEX_INVALID != effect_index );

	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];

	GMS_EFFECT_3DES_WORK* effect = GmEfctZoneEsCreate(
			&deco_work->obj_work,
			zone_type,
			effect_index );
	effect->efct_com.obj_work.dir.z = g_gm_deco_rot_z[dec_rec->id];

	//ƒtƒ‰ƒO
	effect->efct_com.obj_work.disp_flag |= g_gm_deco_disp_flag[deco_id];
	effect->efct_com.obj_work.user_flag |= g_gm_deco_user_flag[deco_id];
	effect->efct_com.obj_work.user_work |= g_gm_deco_user_work[deco_id];

	//•`‰æƒRƒ}ƒ“ƒh
	effect->obj_3des.command_state = g_gm_deco_command_state[deco_id];

	//ŠÖ”İ’è
	if ( deco_work->obj_work.ppFunc ){
		effect->efct_com.obj_work.ppFunc = deco_work->obj_work.ppFunc;
		deco_work->obj_work.ppFunc = NULL;
	}
	return deco_work;
}

// ==========================================================================
// gmDecoInitEffectBlock
/*!
 * ‘•üì¬iƒGƒtƒFƒNƒgƒuƒƒbƒN’PˆÊj
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
GMS_DECO_WORK* gmDecoInitEffectBlock( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	amAssert( dec_rec );

	//‘•üID
	GME_DECORATE_ID deco_id = (GME_DECORATE_ID)dec_rec->id;
	
	//----------------------------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	//----------------------------------------------------------
	s32 block_x = x >> (FX32_SHIFT + GMD_DECO_FX32_BITSHIFT_BLOCK_SIZE);
	block_x <<= (FX32_SHIFT + GMD_DECO_FX32_BITSHIFT_BLOCK_SIZE);

	s32 block_y = y >> (FX32_SHIFT + GMD_DECO_FX32_BITSHIFT_BLOCK_SIZE);
	block_y <<= (FX32_SHIFT + GMD_DECO_FX32_BITSHIFT_BLOCK_SIZE);

	GMS_DECO_WORK* deco_work = gmDecoInitNomodel( 
			dec_rec,
			block_x, 
			block_y,
			type,
			sizeof(GMS_DECO_WORK) );
	if ( !deco_work ){
		return NULL;
	}

	//----------------------------------------------------------
	//ƒGƒtƒFƒNƒg
	//----------------------------------------------------------
	s32 effect_index = g_gm_deco_model_index[ deco_id ][0];
	amAssert( GMD_DECO_DATA_INDEX_INVALID != effect_index );

	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];

	GMS_EFFECT_3DES_WORK* effect = GmEfctZoneEsCreate(
			&deco_work->obj_work,
			zone_type,
			effect_index );
	effect->efct_com.obj_work.dir.z = g_gm_deco_rot_z[dec_rec->id];

	//ƒtƒ‰ƒO
	effect->efct_com.obj_work.disp_flag |= g_gm_deco_disp_flag[deco_id];
	effect->efct_com.obj_work.user_flag |= g_gm_deco_user_flag[deco_id];
	effect->efct_com.obj_work.user_work |= g_gm_deco_user_work[deco_id];

	//•`‰æƒRƒ}ƒ“ƒh
	effect->obj_3des.command_state = g_gm_deco_command_state[deco_id];

	//ƒ†[ƒUƒtƒ‰ƒO
	effect->efct_com.obj_work.user_flag |= deco_work->obj_work.user_flag;

	//ƒƒCƒ“ˆ—
	if ( deco_work->obj_work.ppFunc ){
		effect->efct_com.obj_work.ppFunc = deco_work->obj_work.ppFunc;
		deco_work->obj_work.ppFunc = NULL;
	}

	//•¡”•`‰æ
	if ( g_gm_deco_user_flag[ deco_id ] & GMD_DECO_USER_FLAG_EFFECT_DUPLICATE_DRAW ){
		//ƒ†[ƒUƒ[ƒN‚Ì’l‚ÅƒIƒtƒZƒbƒg
		s32 offset_x = g_gm_deco_user_work[ deco_id ];
		offset_x = offset_x << 16;
		offset_x = offset_x >> 16;
		s32 offset_y = g_gm_deco_user_work[ deco_id ];
		offset_y = offset_y >> 16;
		GmEffect3DESSetDuplicateDraw( 
				effect,
				(Float)offset_x,
				(Float)offset_y,
				0 );
	}

	return deco_work;
}

// ==========================================================================
// gmDecoInitEffectBlockAndNext
/*!
 * ‘•üì¬iƒGƒtƒFƒNƒgƒuƒƒbƒN’PˆÊ & Ÿ‚ÌƒCƒ“ƒfƒNƒX‚Ì•¨‚à¶¬j
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 * @return ‘•üƒ[ƒN
 */
// ==========================================================================
GMS_DECO_WORK* gmDecoInitEffectBlockAndNext( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type )
{
	amAssert( dec_rec );

	//‘•üID
	GME_DECORATE_ID deco_id = (GME_DECORATE_ID)dec_rec->id;
	
	//----------------------------------------------------------
	//ƒIƒuƒWƒFƒNƒgì¬
	//----------------------------------------------------------
	s32 block_x = x >> (FX32_SHIFT + GMD_DECO_FX32_BITSHIFT_BLOCK_SIZE);
	block_x <<= (FX32_SHIFT + GMD_DECO_FX32_BITSHIFT_BLOCK_SIZE);

	s32 block_y = y >> (FX32_SHIFT + GMD_DECO_FX32_BITSHIFT_BLOCK_SIZE);
	block_y <<= (FX32_SHIFT + GMD_DECO_FX32_BITSHIFT_BLOCK_SIZE);

	GMS_DECO_WORK* deco_work = gmDecoInitNomodel( 
			dec_rec,
			block_x, 
			block_y,
			type,
			sizeof(GMS_DECO_WORK) );
	if ( !deco_work ){
		return NULL;
	}

	//----------------------------------------------------------
	//ƒGƒtƒFƒNƒg
	//----------------------------------------------------------
	s32 effect_index = g_gm_deco_model_index[ deco_id ][0];
	amAssert( GMD_DECO_DATA_INDEX_INVALID != effect_index );

	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];

	GMS_EFFECT_3DES_WORK* effect_base = GmEfctZoneEsCreate(
			&deco_work->obj_work,
			zone_type,
			effect_index );
	effect_base->efct_com.obj_work.dir.z = g_gm_deco_rot_z[dec_rec->id];

	//ƒtƒ‰ƒO
	effect_base->efct_com.obj_work.disp_flag |= g_gm_deco_disp_flag[deco_id];
	effect_base->efct_com.obj_work.user_flag |= g_gm_deco_user_flag[deco_id];
	effect_base->efct_com.obj_work.user_work |= g_gm_deco_user_work[deco_id];

	//•`‰æƒRƒ}ƒ“ƒh
	effect_base->obj_3des.command_state = g_gm_deco_command_state[deco_id];

	GMS_EFFECT_3DES_WORK* effect_next = GmEfctZoneEsCreate(
			&deco_work->obj_work,
			zone_type,
			effect_index+1 );
	effect_next->efct_com.obj_work.dir.z = g_gm_deco_rot_z[dec_rec->id];

	//ƒtƒ‰ƒO
	effect_next->efct_com.obj_work.disp_flag |= g_gm_deco_disp_flag[deco_id];
	effect_next->efct_com.obj_work.user_flag |= g_gm_deco_user_flag[deco_id];
	effect_next->efct_com.obj_work.user_work |= g_gm_deco_user_work[deco_id];

	//•`‰æƒRƒ}ƒ“ƒh
	effect_next->obj_3des.command_state = g_gm_deco_command_state[deco_id];

	//ŠÖ”İ’è
	if ( deco_work->obj_work.ppFunc ){
		effect_base->efct_com.obj_work.ppFunc = deco_work->obj_work.ppFunc;
		effect_next->efct_com.obj_work.ppFunc = deco_work->obj_work.ppFunc;
		deco_work->obj_work.ppFunc = NULL;
	}

	return deco_work;
}


#if GMD_DECO_USE_DRAW_SERVER
// ==========================================================================
// gmDecoDrawServerMain
/*!
 *	‘•ü@•`‰æ—pƒT[ƒo
 *
 *	@param tcb		[in]	TCB
 */
// ==========================================================================
void gmDecoDrawServerMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	
	if (!GmMainIsDrawEnable()) {
		return;
	}

	u32 command = 0;
	GMS_DECO_FALL_MANAGER* mgr = NULL;
	float frame = (float)gmDecoGameSystemGetSyncTime();
	for (int i = 0; i < GMD_DECO_FALL_MANAGER_NUM; ++i) {
		mgr = &g_deco_fall_manager[i];
		if (mgr->dec_id == 0) {
			continue;
		}
		switch (mgr->dec_id) {
			case GMD_DECORATE_ID_FALL:			//‘ê
			case GMD_DECORATE_ID_FALL_LEFT:		//‘ê
			case GMD_DECORATE_ID_FALL_RIGHT:	//‘ê
			case GMD_DECORATE_ID_FALL_ONE:		//‘ê
				command = OBD_DRAW_CMD_STATE_PRE_WATER;
				//ObjDraw3DNNSetCameraEx(GME_CAMERA_NO_WATER, NNE_PROJECTION_TYPE_ORTHO, OBD_DRAW_CMD_STATE_PRE_WATER);

				break;
				
			case GMD_DECORATE_ID_FALL_BACK:			//‘êi‰œj
			case GMD_DECORATE_ID_FALL_LEFT_BACK:	//‘êi‰œj
			case GMD_DECORATE_ID_FALL_RIGHT_BACK:	//‘êi‰œj
			case GMD_DECORATE_ID_FALL_ONE_BACK:		//‘êi‰œj
				command = OBD_DRAW_CMD_STATE_WATER_MAPMID;
				//ObjDraw3DNNSetCamera(GME_CAMERA_NO_FALL_BACK, NNE_PROJECTION_TYPE_ORTHO);
				//command = OBD_DRAW_CMD_STATE_PRE_WATER;
				ObjDraw3DNNSetCameraEx(GME_CAMERA_NO_MAIN, NNE_PROJECTION_TYPE_ORTHO, OBD_DRAW_CMD_STATE_WATER_MAPMID);
				break;
		}

		VecFx32 vec = mgr->reg->vec;
		NNS_TEXLIST* texlist = mgr->texlist;

		NNS_MATRIX	obj_mtx;
		nnMakeUnitMatrix(&obj_mtx);
		nnTranslateMatrix(&obj_mtx, &obj_mtx,
						FX_FX32_TO_F32(vec.x), FX_FX32_TO_F32(vec.y), FX_FX32_TO_F32(vec.z));
	//	nnScaleMatrix(&obj_mtx, &obj_mtx, 3.2f, 3.2f, 3.2f);

		//	ƒvƒŠƒ~ƒeƒBƒu‚Æ‚µ‚Ä“o˜^
		AMS_PARAM_DRAW_PRIMITIVE dat;
		// ƒx[ƒXƒ}ƒgƒŠƒbƒNƒX‚È‚µ
		
		// ƒvƒŠƒ~ƒeƒBƒuİ’è
		dat.type = NNE_PRIM_TRIANGLE_LIST;
		dat.count = 6 * mgr->reg_num;
		dat.ablend = NNE_PRIM_ALPHABLEND_ON;
		// ƒAƒ‹ƒtƒ@ƒuƒŒƒ“ƒhİ’è
#if defined(_PC) | defined(_XBOX)
		dat.bldSrc = NNE_BLENDMODE_SRCALPHA;
		dat.bldDst = NNE_BLENDMODE_INVSRCALPHA;
		dat.bldMode = NNE_BLENDOP_ADD;
#else
		dat.bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
		dat.bldDst = NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA;
		dat.bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif
		// ƒeƒXƒgİ’è
		dat.aTest = 0;
		dat.zMask = 0;
		dat.zTest = 1;
		
		// ƒ\[ƒg‚µ‚È‚¢
		dat.noSort = 1;

		// ƒeƒNƒXƒ`ƒƒƒNƒ‰ƒ“ƒvİ’è
		dat.uwrap = NNE_PRIM_TEXWRAP_CLAMP;
		dat.vwrap = NNE_PRIM_TEXWRAP_REPEAT;
		
		// ƒeƒNƒXƒ`ƒƒİ’è
		dat.texlist = texlist;
		dat.texId   = texlist->nTex - 1;
		
		// ’¸“_ƒf[ƒ^ì¬
		NNS_PRIM3D_PCT* v_tbl = (NNS_PRIM3D_PCT*)amDrawMallocDataBuffer((s32)(sizeof(NNS_PRIM3D_PCT) * dat.count));
		dat.vtxPCT3D = v_tbl;
		dat.format3D = NNE_PRIM3D_FMT_PCT;
		
		u32 count = 0;
		float v_diff = fmod(frame, mgr->frame) / mgr->frame * 5.027991f;
		
		for (u32 j = 0; j < GMD_DECO_FALL_REGISTER_NUM; j++) {
			VecFx32    add_fx_vec = mgr->reg[j].vec;
			if (add_fx_vec.x == 0) {
				continue;
			}
			NNS_PRIM3D_PCT* v = v_tbl + 6 * count;
			NNS_VECTOR add_vec;
			// UVÀ•Wİ’è (b’è)
			v[0].Tex.u = v[1].Tex.u = 1.0f;
			v[2].Tex.u = v[3].Tex.u = 0.0f;
			v[0].Tex.v = v[2].Tex.v = -1.0f * (float)(mgr->reg[j].num) - v_diff;
			v[1].Tex.v = v[3].Tex.v = 0.0f - v_diff;
			
			// ƒJƒ‰[İ’è
			v[0].Col = (u32)(0xffffffff);
			v[1].Col = v[2].Col = v[3].Col = v[0].Col;
					
			// ’¸“_İ’è & ˆÚ“®
			add_vec.x = FX_FX32_TO_F32(add_fx_vec.x - vec.x);
			add_vec.y = FX_FX32_TO_F32(add_fx_vec.y - vec.y);
			add_vec.z = FX_FX32_TO_F32(add_fx_vec.z - vec.z);
			v[0].Pos.x = v[1].Pos.x = -(-32.0f) + add_vec.x;
			v[2].Pos.x = v[3].Pos.x =  (-32.0f) + add_vec.x;
			v[0].Pos.y = v[2].Pos.y = -(-32.0f) + add_vec.y;
			v[1].Pos.y = v[3].Pos.y =  (-32.0f) + add_vec.y + (-64.0f) * (float)(mgr->reg[j].num - 1);

			v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z =  +1.0f  + add_vec.z;

			v[4] = v[2];
			v[5] = v[3];
			v[3] = v[1];

			mgr->reg[j].vec.y = 0;
			count++;
		}
		
		amMatrixPush(&obj_mtx);
		ObjDraw3DNNDrawPrimitive(&dat, command);
		amMatrixPop();
	}
#if GMD_DECO_USE_DRAW_TVX
	gmDecoExecuteDrawPrimitive();
	gmDecoInitDrawPrimitive();
#endif 
}

// ==========================================================================
// gmDecoAddFallEvent
/*!
 *	‘•ü@‘êƒCƒxƒ“ƒg’Ç‰Á
 *
 *	@param dec_rec		[in]	‘•üƒCƒxƒ“ƒg
 *	@param x			[in]	ƒCƒxƒ“ƒg”­¶À•Wx
 *	@param texlist		[in]	g—p‚·‚éƒeƒNƒXƒ`ƒƒƒŠƒXƒg
 *	@param frame		[in]	‘ƒtƒŒ[ƒ€”
 */
// ==========================================================================
static void gmDecoAddFallEvent(GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, NNS_TEXLIST* texlist, float frame)
{
	// À‘Ì‚ª–³‚¯‚ê‚ÎI—¹
	if (!dec_rec) {
		return;
	}
	int i;
	u16 id = dec_rec->id;
	GMS_DECO_FALL_MANAGER*  mgr_use = NULL;
	GMS_DECO_FALL_REGISTER* reg_use = NULL;
	// ƒCƒxƒ“ƒgID‚Ìƒ`ƒFƒbƒN(‘ê‚©”Û‚©)
	switch (id) {
		case GMD_DECORATE_ID_FALL:			//‘ê
		case GMD_DECORATE_ID_FALL_LEFT:		//‘ê
		case GMD_DECORATE_ID_FALL_RIGHT:	//‘ê
		case GMD_DECORATE_ID_FALL_ONE:		//‘ê
//		case GMD_DECORATE_ID_FALL_A:		//‘ê
//		case GMD_DECORATE_ID_FALL_LEFT_A:	//‘ê
//		case GMD_DECORATE_ID_FALL_RIGHT_A:	//‘ê
//		case GMD_DECORATE_ID_FALL_ONE_A:	//‘ê
		case GMD_DECORATE_ID_FALL_BACK:			//‘êi‰œj
		case GMD_DECORATE_ID_FALL_LEFT_BACK:	//‘êi‰œj
		case GMD_DECORATE_ID_FALL_RIGHT_BACK:	//‘êi‰œj
		case GMD_DECORATE_ID_FALL_ONE_BACK:		//‘êi‰œj
//		case GMD_DECORATE_ID_FALL_A_BACK:		//‘êi‰œj
//		case GMD_DECORATE_ID_FALL_LEFT_A_BACK:	//‘êi‰œj
//		case GMD_DECORATE_ID_FALL_RIGHT_A_BACK:	//‘êi‰œj
//		case GMD_DECORATE_ID_FALL_ONE_A_BACK:	//‘êi‰œj
			for (i = 0; i < GMD_DECO_FALL_MANAGER_NUM; ++i) {
				GMS_DECO_FALL_MANAGER* mgr = &g_deco_fall_manager[i];
				// ƒCƒxƒ“ƒgID“o˜^‚È‚µ‚ğŠm•Û‚µ‚Ä‚¨‚­
				if (mgr->dec_id == 0) {
					if (mgr_use == NULL) {
						mgr_use = mgr;
					}
				}
				// ƒCƒxƒ“ƒgID‚ª“¯‚¶
				else if (mgr->dec_id == id) {
					mgr_use = mgr; // ‹­§ã‘‚«
					break;
				}
			}
			MTM_ASSERT(mgr_use != NULL);

			mgr_use->dec_id   = id;
			mgr_use->texlist  = texlist;
			mgr_use->frame    = frame;
			++mgr_use->all_num;
			
			for (i = 0; i < GMD_DECO_FALL_REGISTER_NUM; i++) {
				GMS_DECO_FALL_REGISTER* reg = &mgr_use->reg[i];
				//	“o˜^‚È‚µ‚ğŠm•Û
				if (reg->num == 0) {
					if (reg_use == NULL) {
						reg_use = reg;
					}
				}
				// “¯‚¶XÀ•W‚Å”­¶‚·‚éƒCƒxƒ“ƒg‚ğŠm•Û
				else if (reg->vec.x == x) {
					reg_use = reg;
					break;
				}
			}
			MTM_ASSERT(reg_use != NULL);

			if (reg_use->num == 0) {
				++mgr_use->reg_num;
			}
			++reg_use->num;
			reg_use->vec.x = x;
			break;

		default:
			break;
	}
}

// ==========================================================================
// gmDecoDelFallEvent
/*!
 *	‘•ü@‘êƒCƒxƒ“ƒgíœ
 *
 *	@param dec_rec		[in]	‘•üƒCƒxƒ“ƒg
 *	@param x			[in]	ƒCƒxƒ“ƒg”­¶À•Wx
 */
// ==========================================================================
static void gmDecoDelFallEvent(GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x)
{
	// À‘Ì‚ª–³‚¯‚ê‚ÎI—¹
	if (!dec_rec) {
		return;
	}
	u16 id = dec_rec->id;
	// ƒCƒxƒ“ƒgID‚Ìƒ`ƒFƒbƒN(‘ê‚©”Û‚©)
	switch (id) {
		case GMD_DECORATE_ID_FALL:			//‘ê
		case GMD_DECORATE_ID_FALL_LEFT:		//‘ê
		case GMD_DECORATE_ID_FALL_RIGHT:	//‘ê
		case GMD_DECORATE_ID_FALL_ONE:		//‘ê
//		case GMD_DECORATE_ID_FALL_A:		//‘ê
//		case GMD_DECORATE_ID_FALL_LEFT_A:	//‘ê
//		case GMD_DECORATE_ID_FALL_RIGHT_A:	//‘ê
//		case GMD_DECORATE_ID_FALL_ONE_A:	//‘ê
		case GMD_DECORATE_ID_FALL_BACK:			//‘êi‰œj
		case GMD_DECORATE_ID_FALL_LEFT_BACK:	//‘êi‰œj
		case GMD_DECORATE_ID_FALL_RIGHT_BACK:	//‘êi‰œj
		case GMD_DECORATE_ID_FALL_ONE_BACK:		//‘êi‰œj
//		case GMD_DECORATE_ID_FALL_A_BACK:		//‘êi‰œj
//		case GMD_DECORATE_ID_FALL_LEFT_A_BACK:	//‘êi‰œj
//		case GMD_DECORATE_ID_FALL_RIGHT_A_BACK:	//‘êi‰œj
//		case GMD_DECORATE_ID_FALL_ONE_A_BACK:	//‘êi‰œj
			int i, j;
			for (i = 0; i < GMD_DECO_FALL_MANAGER_NUM; ++i) {
				GMS_DECO_FALL_MANAGER* mgr = &g_deco_fall_manager[i];
				// ƒCƒxƒ“ƒgID‚ª“¯‚¶
				if (mgr->dec_id == id) {
					--mgr->all_num;

					// “o˜^íœ
					if (mgr->all_num == 0) {
						mgr->dec_id = 0;
						mgr->texlist = NULL;
					}
					// “¯‚¶XÀ•W‚ğ’T‚µ‚Äíœ
					for (j = 0; j < GMD_DECO_FALL_REGISTER_NUM; j++) {
						GMS_DECO_FALL_REGISTER* reg = &mgr->reg[j];
						if (reg->vec.x == x) {
							--reg->num;
							if (reg->num == 0) {
								reg->vec.x = 0;
								--mgr->reg_num;
							}
							break;
						}
					}

					break;
				}
			}
			break;

		default:
			break;
	}
}

// ==========================================================================
// gmDecoSetDrawFall
/*!
 *	‘•ü@‘ê•`‰æ“o˜^
 *
 *	@param	obj_work	[in] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *
 *	@note ÀÛ‚Ì•`‰æ‚Í‘•üƒT[ƒo[‚Ås‚¢‚Ü‚·B
 */
// ==========================================================================
static void gmDecoSetDrawFall(OBS_OBJECT_WORK* obj_work)
{
	if (!GmMainIsDrawEnable()) {
		return;
	}
		
	GMS_DECO_WORK *deco_work = (GMS_DECO_WORK*)obj_work;
	
	VecFx32 vec;

	vec.x = obj_work->pos.x + obj_work->ofst.x;
	vec.y = -(obj_work->pos.y + obj_work->ofst.y);
	vec.z = obj_work->pos.z + obj_work->ofst.z;

	GMS_EVE_RECORD_DECORATE* dec_rec = deco_work->event_record;

	// ƒCƒxƒ“ƒg‚ÆXÀ•W‚©‚çg—p‚·‚éƒIƒuƒWƒFƒNƒg‚ğ’²¸
	GMS_DECO_FALL_REGISTER* reg = NULL;
	for (int i = 0; i < GMD_DECO_FALL_MANAGER_NUM; ++i) {
		if (g_deco_fall_manager[i].dec_id == dec_rec->id) {
			for (int j = 0; j < GMD_DECO_FALL_REGISTER_NUM; ++j) {
				if (g_deco_fall_manager[i].reg[j].vec.x == vec.x) {
					reg = &g_deco_fall_manager[i].reg[j];
					break;
				}
			}
		}
	}
	if (reg != NULL) {
		// YÀ•W‚Ì“o˜^(Z‚Íí‚É“o˜^)
		reg->vec.z = vec.z;
		if (reg->vec.y == 0 || reg->vec.y < vec.y) {
			reg->vec.y = vec.y;
		}
	}
}
#endif // GMD_DECO_USE_DRAW_SERVER

#if GMD_DECO_USE_DRAW_TVX
// ==========================================================================
// gmDecoInitDrawPrimitive
/*!
 *	‘•ü@ƒvƒŠƒ~ƒeƒBƒu•`‰æ‰Šú‰»
 *
 *	@note ƒvƒŠƒ~ƒeƒBƒuƒ[ƒN‚ğ•K—vÅ¬ŒÀ‚Ì‚İ‰Šú‰»‚µ‚Ü‚·B<BR>
 */
// ==========================================================================
static void gmDecoInitDrawPrimitive(void)
{
	GMS_DECO_PRIM_DRAW_WORK* work = g_deco_tvx_work; // ƒvƒŠƒ~ƒeƒBƒu•`‰æƒ[ƒN
	
	for (int i = 0; i < GMD_DECO_PRIM_DRAW_WORK_NUM; i++) {
		work[i].tex         = NULL;
		work[i].tex_id      = -1;
		work[i].command     = (u32)-1;
		work[i].all_vtx_num = 0;
		work[i].stack_num   = 0;
	}
}

// ==========================================================================
// gmDecoSetDrawPrimitive
/*!
 *	‘•ü@ƒvƒŠƒ~ƒeƒBƒu•`‰æİ’è
 *
 *	@param	model_index	[in] g—p‚·‚éTVXƒ‚ƒfƒ‹”Ô†
 *	@param	model_tex	[in] g—p‚·‚éƒeƒNƒXƒ`ƒƒƒŠƒXƒg‚ğŠi”[‚µ‚½AOS_TEXTURE
 *	@param	pos			[in] •`‰æÀ•W
 *	@param	off_y		[in] ƒIƒtƒZƒbƒgY(À•WˆÚ“®g—p)
 *	@param	command		[in] •`‰æ‚Ég—p‚·‚éƒRƒ}ƒ“ƒhó‘Ô
 *	@param	disp_flag	[in] •`‰æƒtƒ‰ƒO
 *
 *	@note ÀÛ‚Ì•`‰æ‚ÍgmDecoExecuteDrawPrimitive‚Ås‚¢‚Ü‚·B
 *        TVXƒ‚ƒfƒ‹‚É‚ÍgmDecoDataGetInfo()‚Å“¾‚ç‚ê‚é‘•üƒf[ƒ^‚Ìtvx_model‚ğg—p‚µ‚Ü‚·B
 */
// ==========================================================================
static void gmDecoSetDrawPrimitive(s32 model_index, AOS_TEXTURE* model_tex, VecFx32* pos, Float off_y, u32 command, u32 disp_flag)
{
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	void* obj_tvx_file = amBindGet((AMS_AMB_HEADER*)gmDecoDataGetInfo()->tvx_model, model_index);
	
	MTM_ASSERT(AoTvxIsTvxFile(obj_tvx_file));
	
	u32 tex_num = AoTvxGetTextureNum(obj_tvx_file);
	
	GMS_DECO_PRIM_DRAW_WORK* work = g_deco_tvx_work; // ƒvƒŠƒ~ƒeƒBƒu•`‰æƒ[ƒN
	GMS_DECO_PRIM_DRAW_STACK* stack = NULL; // ƒXƒ^ƒbƒN
	
	// ’¸“_ˆ—
	for (u32 num = 0; num < tex_num; num++) {
		MTM_ASSERT(AOD_TVX_PRIMTYPE_TRIANGLESTRIP == AoTvxGetPrimitiveType(obj_tvx_file, num));

		u32 vtx_num = AoTvxGetVertexNum(obj_tvx_file, num);
		s32 tex_id  = AoTvxGetTextureId(obj_tvx_file, num);
		//tex_id = 0; // ‹­§ƒeƒNƒXƒ`ƒƒ‚P–‡‰»

		// work İ’è
		int i;
		for (i = 0; i < GMD_DECO_PRIM_DRAW_WORK_NUM; i++) {
			//	ƒ[ƒN‚Ö–¢“o˜^‚Ü‚½‚ÍŠY“–ID“o˜^Ï‚İ‚È‚çƒeƒNƒXƒ`ƒƒî•ñ‚ğ“o˜^
			if (
				(work[i].tex == NULL && work[i].tex_id == -1 && work[i].command == (u32)-1)
				||	(work[i].tex == model_tex && work[i].tex_id == tex_id && work[i].command == command)
				) {
				work[i].tex     = model_tex;
				work[i].tex_id  = tex_id;
				work[i].command = command;
				work[i].all_vtx_num += (u16)vtx_num;

				stack = &work[i].stack[work[i].stack_num];
				stack->vtx       = (AOS_TVX_VERTEX*)AoTvxGetVertex(obj_tvx_file, num);
				stack->vtx_num   = vtx_num;
				stack->pos       = *pos;
				stack->off_y     = off_y;
				stack->disp_flag = disp_flag;
				
				++work[i].stack_num;

				MTM_ASSERT(work[i].stack_num < GMD_DECO_PRIM_DRAW_STACK_NUM);

				break;
			}
			//	‘¼ƒeƒNƒXƒ`ƒƒ‚Ìê‡‚ÍŸ‚Ö
		}
		MTM_ASSERT(i < GMD_DECO_PRIM_DRAW_WORK_NUM); // ƒ‹[ƒv‚ğŠ®‘–‚µ‚½=ƒvƒŠƒ~ƒeƒBƒuƒ[ƒN•s‘«
	}
}

// ==========================================================================
// gmDecoExecuteDrawPrimitive
/*!
 *	‘•ü@ƒvƒŠƒ~ƒeƒBƒu•`‰æ
 *
 *	@note gmDecoSetDrawPrimitive‚É‚æ‚Á‚Ä“o˜^‚³‚ê‚½’¸“_ƒvƒŠƒ~ƒeƒBƒuî•ñ‚Ì•`‰æ‚ğs‚¢‚Ü‚·B
 */
// ==========================================================================
static void gmDecoExecuteDrawPrimitive(void)
{
	AMS_PARAM_DRAW_PRIMITIVE dat;
	GMS_DECO_PRIM_DRAW_WORK* work = g_deco_tvx_work; // ƒvƒŠƒ~ƒeƒBƒu•`‰æƒ[ƒN
	GMS_DECO_PRIM_DRAW_STACK* stack = NULL; // ƒXƒ^ƒbƒN
	
	NNS_MATRIX h_mtx, v_mtx, u_mtx;
	
	// æ“ª‚ÌƒeƒNƒXƒ`ƒƒ‚ÅÀs‚ğ”»’è
	if (!work->tex) {
		return;
	}
	
	nnMakeUnitMatrix(&u_mtx);
	nnMakeScaleMatrix(&h_mtx, -1.0f,  1.0f, 1.0f);
	nnMakeScaleMatrix(&v_mtx,  1.0f, -1.0f, 1.0f);
	
	u32 color = GmMainGetLightColor();
	
	dat.ablend = NNE_PRIM_ALPHABLEND_OFF;
	// ƒAƒ‹ƒtƒ@ƒuƒŒƒ“ƒhİ’è
#if defined(_PC) | defined(_XBOX)
	dat.bldSrc = NNE_BLENDMODE_SRCALPHA;
	dat.bldDst = NNE_BLENDMODE_INVSRCALPHA;
	dat.bldMode = NNE_BLENDOP_ADD;
#else
	dat.bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
	//dat.bldDst = NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA;
	dat.bldDst = NND_BLENDFUNC_GL_ONE;
	dat.bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif
	// ƒeƒXƒgİ’è
	dat.aTest = 1;
	dat.zMask = 0;
	dat.zTest = 1;
	
	// ƒ\[ƒg‚µ‚È‚¢
	dat.noSort = 1;
	
	// ƒeƒNƒXƒ`ƒƒƒNƒ‰ƒ“ƒvİ’è
	dat.uwrap = NNE_PRIM_TEXWRAP_CLAMP;
	dat.vwrap = NNE_PRIM_TEXWRAP_CLAMP;
	
	dat.format3D = NNE_PRIM3D_FMT_PCT;

	// •`‰æ
	u32 prim;
	for (prim = 0; prim < GMD_DECO_PRIM_DRAW_WORK_NUM; ++prim) {
		//	“o˜^‚³‚ê‚Ä‚È‚¢‚È‚çI—¹
		if (work[prim].tex_id == -1) {
			break;
		}
		if (work[prim].tex_id != 0) {
			dat.ablend = NNE_PRIM_ALPHABLEND_ON;
			dat.aTest  = 0;
		}
		else {
			dat.ablend = NNE_PRIM_ALPHABLEND_OFF;
			dat.aTest  = 1;
		}
		// ƒeƒNƒXƒ`ƒƒİ’è
		dat.texlist = work[prim].tex->texlist;
		
		// •`‰æ
		dat.type = NNE_PRIM_TRIANGLE_STRIP;
		dat.count = work[prim].all_vtx_num + work[prim].stack_num * 2 - 2;
		
		NNS_PRIM3D_PCT* v_tbl = (NNS_PRIM3D_PCT*)amDrawMallocDataBuffer((s32)(sizeof(NNS_PRIM3D_PCT) * dat.count));
		dat.vtxPCT3D = v_tbl;
		dat.texId = work[prim].tex_id;
		u32 v_tbl_pos = 0;
		
		float dx, dy, dz;
		for (u32 num = 0; num < work[prim].stack_num; ++num) {
			stack = &(work[prim].stack[num]);
			// ƒvƒŠƒ~ƒeƒBƒuİ’è
			s32 draw_num = stack->vtx_num / 3;
			dx =  FXM_FX32_TO_FLOAT(stack->pos.x);
			dy = -FXM_FX32_TO_FLOAT(stack->pos.y) + stack->off_y;
			dz =  FXM_FX32_TO_FLOAT(stack->pos.z);

			NNS_VECTOR vec;
			
			NNS_PRIM3D_PCT* v = v_tbl;
			AOS_TVX_VERTEX* vtx = stack->vtx;
			
			// ’¸“_İ’è (STRIP‘Î‰)
			for (int i = 0; i < stack->vtx_num; i++) {
				vec.x = vtx[i].x;
				vec.y = vtx[i].y;
				vec.z = vtx[i].z;
				
				if (stack->disp_flag & OBD_DISP_HFLIP) {
					nnTransformVector(&v[i].Pos, &h_mtx, &vec);
				}
				else if (stack->disp_flag & OBD_DISP_VFLIP) {
					nnTransformVector(&v[i].Pos, &v_mtx, &vec);
				}
				else {
					nnCopyVector(&v[i].Pos, (const NNS_VECTOR*)&vec);
				}
				
				
				v[i].Pos.x += dx;
				v[i].Pos.y += dy;
				v[i].Pos.z += dz;
				
				v[i].Tex.u = vtx[i].u;
				v[i].Tex.v = vtx[i].v;
				v[i].Col   = vtx[i].c & color;
			}
			
			v_tbl += stack->vtx_num + 2;
			
			// STRIP‚Èƒf[ƒ^‚É‚·‚é‚½‚ß‚Ì‘Î‰
			NNS_PRIM3D_PCT* v_strip;
			// æ“ª‚ÌSTRIPˆÈŠO
			if (num != 0) {
				v_strip = v - 1;
				v_strip[0] = v_strip[1];
			}
			// ÅŒã‚ÌSTRIPˆÈŠO
			if (num != work[prim].stack_num - 1) {
				v_strip = &v[stack->vtx_num - 1];
				v_strip[1] = v_strip[0];
			}
		}
		ObjDraw3DNNSetCameraEx(GME_CAMERA_NO_MAIN, NNE_PROJECTION_TYPE_ORTHO, work[prim].command);

		amMatrixPush(&u_mtx);
		ObjDraw3DNNDrawPrimitive(&dat, work[prim].command, NNE_PRIM_LIGHT_DISABLE, NNE_PRIM_CULL_NONE);
		amMatrixPop();
	}
}
#endif // GMD_DECO_USE_DRAW_TVX

#if _IPHONE
// ==========================================================================
// gmDecoIsUseModel
/*!
 *	‘•ü@ƒ‚ƒfƒ‹g—p”»’è
 *
 *	@param	type	”»’è‚·‚éƒ‚ƒfƒ‹ƒ^ƒCƒv
 *
 *	@note ¡‰ñ‚ÌƒXƒe[ƒW‚Åw’è‚µ‚½ƒ^ƒCƒv‚Ìƒ‚ƒfƒ‹‚ğg—p‚·‚é‚©”Û‚©‚ğ”»’è‚µ‚Ü‚·B
 */
// ==========================================================================
static BOOL gmDecoIsUseModel(GME_DECO_USE_MODEL_TYPE type)
{
	BOOL use = FALSE;
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	
	// ’Êíƒ‚ƒfƒ‹
	if (GMD_DECO_USE_MODEL_TYPE_NORMAL == type) {
		if (GSD_MAIN_ZONE_TYPE_SS != zone_type) {
			use = TRUE;
		}
	}
	
	// ƒŒƒ“ƒ_[ƒ‚ƒfƒ‹
	if (GMD_DECO_USE_MODEL_TYPE_RENDER == type) {
		if ((GSD_MAIN_ZONE_TYPE_1 == zone_type) || (GSD_MAIN_ZONE_TYPE_3 == zone_type)) {
			use = TRUE;
		}
	}
	
	return use;
}
#endif // _IPHONE


// ==========================================================================
// GmDecoStaticVarInit
/*!
 *	static•Ï”‚Ì‰Šú‰»
 */
// ==========================================================================
void GmDecoStaticVarInit(void)
{
	//ƒf[ƒ^
	memset(&g_deco_data_real, 0, sizeof(g_deco_data_real));
	g_deco_data = NULL;
	
	//ŠÇ—î•ñ
	memset(&g_deco_mgr_real, 0, sizeof(g_deco_mgr_real));
	g_deco_mgr = NULL;
	
#if GMD_DECO_USE_DRAW_SERVER
	// ‘êŠÇ—
	memset(g_deco_fall_manager, 0, sizeof(g_deco_fall_manager));
	gm_deco_draw_server_tcb = NULL;
#endif // GMD_DECO_USE_DRAW_SERVER
	
#if GMD_DECO_USE_DRAW_TVX
	memset(g_deco_tvx_work, 0, sizeof(g_deco_tvx_work));
#endif // GMD_DECO_USE_DRAW_TVX
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
