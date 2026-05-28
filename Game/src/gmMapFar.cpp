// ==========================================================================
/*!
  @file gmMapFar.cpp
  @brief 

  @author hanaoka, SyuichiGotou
				Copyright(c) 2009-2010 Dimps	
  $Id: gmMapFar.cpp 20 2011-04-22 12:46:46Z thamada $
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
#include "gmMap.h"
#include "gmCamera.h"
#include "gmGameDBuild.h"

#include "gmMapFar.h"


// ƒf[ƒ^ƒwƒbƒ_
// Zone1
#if _IPHONE
#include "iPhone/model/mapfar_zone1_mdl.hmb"
#include "iPhone/model/mapfar_zone1_mat.hmb"
#else
#include "common/model/mapfar_zone1_mdl.hmb"
#include "common/model/mapfar_zone1_mat.hmb"
#endif
#include "common/model/mapfar_zone1_render_mdl.hmb"
#include "common/model/zone1_mapfar.hmb"

// Zone2
#if _IPHONE
#include "iPhone/model/mapfar_zone2_mdl.hmb"
#include "iPhone/model/mapfar_zone2_mtn.hmb"
#include "iPhone/model/mapfar_zone2_mat.hmb"
#else
#include "common/model/mapfar_zone2_mdl.hmb"
#include "common/model/mapfar_zone2_mtn.hmb"
#include "common/model/mapfar_zone2_mat.hmb"
#endif

#include "common/model/zone2_mapfar.hmb"

// Zone3
#if _IPHONE
#include "iPhone/model/mapfar_zone3_mdl.hmb"
#include "iPhone/model/mapfar_zone3_mat.hmb"
#else
#include "common/model/mapfar_zone3_mdl.hmb"
#include "common/model/mapfar_zone3_mat.hmb"
#endif
#include "common/model/zone3_mapfar.hmb"

// Zonef
#if _IPHONE
#include "iPhone/model/mapfar_zonef_mdl.hmb"
#include "iPhone/model/mapfar_zonef_mat.hmb"
#else
#include "common/model/mapfar_zonef_mdl.hmb"
#include "common/model/mapfar_zonef_mat.hmb"
#endif
#include "common/model/zonef_mapfar.hmb"

// ZoneSS
#include "common/model/MAPFAR_SS01_MDL.HMB"
#include "common/model/MAPFAR_SS01_MAT.HMB"
#include "common/model/SS01_MAPFAR.HMB"

//----- Definitions ---------------------------------------------------------

#define GMD_MAP_FAR_TEST_MPDATA (0 & GMD_MAP_FAR_TEST)

// ƒ†[ƒU[ƒtƒ@ƒ“ƒNƒVƒ‡ƒ“‚Ì’†‚ÅAƒŒƒ“ƒ_[ƒ^[ƒQƒbƒgİ’è‚·‚é
#define GMD_MAP_FAR_SEAMODE	(_PC | _PS3 | _XBOX | _WII)

// ƒGƒfƒBƒ^—LŒø‚©‚Ç‚¤‚©iƒtƒHƒO‚âƒ‰ƒCƒgj
#if !defined (HOG_ALPHA_ROM)	
#define GMD_MAP_FAR_EDIT (0 & AMD_DEBUG & (_PC | _PS3 | _XBOX)) // ƒAƒ‹ƒtƒ@ROM‚Å‚ÍEDIT–³Œø
#define GMD_MAP_FAR_LIGHT_EDIT			 (0)
#endif

// Šâ‚ğƒvƒŠƒ~ƒeƒBƒu˜AŒ‹‚µ‚ÄA‚P‰ñ‚Å•`‰æ‚·‚é‚©Hiƒf[ƒ^‚Æ‚Ì˜A“®•K{j
#if _IPHONE
#define GMD_MAP_FAR_ROCK_PRIMITIVE       (0)
#elif _PC
#define GMD_MAP_FAR_ROCK_PRIMITIVE       (0)
#else
#define GMD_MAP_FAR_ROCK_PRIMITIVE       (0)
#endif

// ‰“Œi‚ğˆê–‡”Â‚Å•\Œ»‚·‚é‚©H
#if _IPHONE
#define GMD_MAP_FAR_2DBG       (1)
#elif _PC
#define GMD_MAP_FAR_2DBG       (0)
#else
#define GMD_MAP_FAR_2DBG       (0)
#endif

#define GMD_MAP_FAR_NNMODEL_OBJECT_START (1)

#if GMD_MAP_FAR_TEST_MPDATA
	#define GMD_MAP_FAR_ZONE_1_CAMERA_POS_Z	( 500.0f )		///< ƒJƒƒ‰ˆÊ’uiZ²j
#else 
	#define GMD_MAP_FAR_ZONE_1_CAMERA_POS_Z	( 160.0f )		///< ƒJƒƒ‰ˆÊ’uiZ²j
#endif //GMD_MAP_FAR_TEST_MPDATA
#define GMD_MAP_FAR_MP_DRAW_POS_Z		( 32.f )	///< MPƒf[ƒ^•`‰æˆÊ’uiZ²j
#define GMD_MPA_FAR_OBJ_NUM				( 16 )			///< ƒIƒuƒWƒFƒNƒg”

#define GMD_MAP_FAR_SCENE_TILE_SIZE_X	( 128 )			///< ƒV[ƒ“—pƒ^ƒCƒ‹ƒTƒCƒYX
#define GMD_MAP_FAR_SCENE_TILE_SIZE_Z	( 128 )			///< ƒV[ƒ“—pƒ^ƒCƒ‹ƒTƒCƒYY
#define GMD_MAP_FAR_SCENE_TILE_NUM_X	( 3 )			///< ƒV[ƒ“—pƒ^ƒCƒ‹”X
#define GMD_MAP_FAR_SCENE_TILE_NUM_Z	( 12 )			///< ƒV[ƒ“—pƒ^ƒCƒ‹”Y

#define GMD_MAP_FAR_MP_OBJECT_START		(1)				///< MPƒf[ƒ^ƒIƒuƒWƒFƒNƒgÀ—LŒøƒ‚ƒfƒ‹ŠJnˆÊ’u
#define GMD_MAP_FAR_MP_DRAW_WIDTH		(6)				///< MPƒf[ƒ^•`‰æ•	
#define GMD_MAP_FAR_MP_DRAW_HEIGHT		(6)				///< MPƒf[ƒ^•`‰æ‚‚³
#define GMD_MAP_FAR_MP_DRAW_MARGIN		(1)				///< MPƒf[ƒ^•`‰æ—]•ª
#define GMD_MAP_FAR_MP_BLOCK_SIZE		(20.f)			///< MPƒf[ƒ^ƒuƒƒbƒNƒf[ƒ^ƒTƒCƒY
#define GMD_MAP_FAR_MP_BLOCK_DRAW_SIZE	(64.f)			///< MPƒf[ƒ^ƒuƒƒbƒN•`‰æƒTƒCƒY
#define GMD_MAP_FAR_MP_POS_Z			(32*FX32_ONE)	///< MPƒf[ƒ^•`‰æ ZˆÊ’u


#define GMD_MAP_FAR_DATA_INDEX_ZONE_1_MP_HEADER (GMD_GAMEDAT_MAPSET_B_MP)
#define GMD_MAP_FAR_DATA_INDEX_ZONE_1_MD_HEADER (GMD_GAMEDAT_MAPSET_B_MD)

#define GMD_MAP_FAR_DATA_INDEX_INVALID	((u32)-1)

///OBJƒCƒ“ƒfƒNƒX
enum GMD_MAP_FAR_OBJ_INDEX{
	//ƒ][ƒ“1
#if _IPHONE
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_SEA = 0,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_SKY,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_ROCKA,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_ROCKB,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_ROCKC,
#else
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_SKYB = 0,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_ROCKA_B,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_ROCKB_B,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_ROCKC_B,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_SEA,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_SKY,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_ROCKA,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_ROCKB,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_ROCKC,
#endif

#if GMD_MAP_FAR_2DBG
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_2DBG,
#endif
	GMD_MAP_FAR_OBJ_INDEX_ZONE_1_MAX,

	//ƒ][ƒ“2
#if _IPHONE
	GMD_MAP_FAR_OBJ_INDEX_ZONE_2DBG = 0,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_2_WHEEL,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_2_SLIGHT,
#else
	GMD_MAP_FAR_OBJ_INDEX_ZONE_2_SKY = 0,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_2_GROUND,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_2_WHEEL,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_2_GLARE,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_2_SLIGHT,
#endif
	GMD_MAP_FAR_OBJ_INDEX_ZONE_2_MAX,

	//ƒ][ƒ“3
	GMD_MAP_FAR_OBJ_INDEX_ZONE_3_ISEKI = 0,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_3_MAX,

	//ƒ][ƒ“4
	GMD_MAP_FAR_OBJ_INDEX_ZONE_4_DUMMY = 0,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_4_MAX,

	//ƒ][ƒ“FINAL
#if _IPHONE
	GMD_MAP_FAR_OBJ_INDEX_ZONE_FINAL_SPACE = 0,
#else
	GMD_MAP_FAR_OBJ_INDEX_ZONE_FINAL_SPACE = 0,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_FINAL_EARTH,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_FINAL_CLOUD,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_FINAL_FILM,
#endif
	GMD_MAP_FAR_OBJ_INDEX_ZONE_FINAL_MAX,

	//ƒXƒyƒXƒe
	GMD_MAP_FAR_OBJ_INDEX_ZONE_SS_KALEIDO = 0,
	GMD_MAP_FAR_OBJ_INDEX_ZONE_SS_MAX,
};

// ŠâƒvƒŠƒ~ƒeƒBƒuAMB
#if GMD_MAP_FAR_ROCK_PRIMITIVE
enum GMD_MAPFAR_TVXAMB_INDEX{
	GMD_MAPFAR_TVXAMB_TXB = 0, // ƒeƒNƒXƒ`ƒƒƒtƒ@ƒCƒ‹ƒŠƒXƒg
	GMD_MAPFAR_TVXAMB_ROCKA,   // ŠâA‚ÌTVX
	GMD_MAPFAR_TVXAMB_ROCKB,
	GMD_MAPFAR_TVXAMB_ROCKC,
	GMD_MAPFAR_TVXAMB_MAX,
};
#endif


///ƒIƒuƒWƒFƒNƒgˆ—ŠÖ”
typedef void (*GM_MAP_FAR_OBJ_FUNC)(struct _OBS_OBJECT_WORK*);

///ƒIƒuƒWƒFƒNƒgƒ[ƒN
typedef struct tag_GMS_MAP_FAR_OBJ_WORK{
	OBS_OBJECT_WORK			obj_work;	///< ƒIƒuƒWƒFƒNƒgƒ[ƒN
	OBS_ACTION3D_NN_WORK	obj_3d;		///< 3DNNƒIƒuƒWƒFƒNƒg
} GMS_MAP_FAR_OBJ_WORK;

///ƒXƒNƒ[ƒ‹î•ñ
typedef struct tag_GMS_MAP_FAR_SCROLL {
	s32 pos;		///< Šî€ˆÊ’u
	s32 width;		///< •
	s32 loop_num;	///< ƒ‹[ƒv‰ñ”
}GMS_MAP_FAR_SCROLL;

///ƒJƒƒ‰î•ñ
typedef struct tag_GMS_MAP_FAR_CAMERA {
	s32 camera_id;						///< ƒJƒƒ‰ID
	NNE_PROJECTION_TYPE camera_type;	///< ƒJƒƒ‰ƒ^ƒCƒv
	float camera_speed_x;				///< ƒJƒƒ‰ƒXƒs[ƒh•â³’liXj
	float camera_speed_y;				///< ƒJƒƒ‰ƒXƒs[ƒh•â³’liYj
}GMS_MAP_FAR_CAMERA;

///ƒf[ƒ^ŠÇ—
typedef struct tag_GMS_MAP_FAR_DATA {
	AMS_AMB_HEADER* amb_header;						///< ambƒwƒbƒ_
	OBS_ACTION3D_NN_WORK* obj_3d_list;
	OBS_ACTION3D_NN_WORK* obj_3d_list_render;
	
	//ƒIƒuƒWƒFƒNƒg
	OBS_OBJECT_WORK* obj_work[GMD_MPA_FAR_OBJ_NUM];	///< objƒ[ƒN

	//3DNNƒ‚ƒfƒ‹
	OBS_ACTION3D_NN_WORK* nn_work;		///< 3DNN•`‰æƒ[ƒN
	s32 nn_work_num;					///< 3DNN•`‰æƒ[ƒN”
	s32 nn_regist_num;					///< 3DNN•`‰æ“o˜^”

	NNS_VECTOR pos;						///< ƒJƒƒ‰ˆÊ’u

	//MPƒf[ƒ^
	MP_HEADER* mp_header;				///< MPƒwƒbƒ_
	MD_HEADER* md_header;				///< MDƒwƒbƒ_

	// “V‹…—p‰ñ“]•Ï”
	float		degSky;
	float		degSky2;

#if GMD_MAP_FAR_ROCK_PRIMITIVE
	AMS_AMB_HEADER*		tvx_amb_header;	///< tvxƒwƒbƒ_
	AMS_AMB_HEADER*		tex_amb_header;	///< texƒwƒbƒ_
	void*				tvx_txb;
	void*				tvx_rockA;
	void*				tvx_rockB;
	void*				tvx_rockC;
	NNS_TEXFILELIST*	tex_buf;		//!< ƒeƒNƒXƒ`ƒƒƒtƒ@ƒCƒ‹æ“ª
	void*				texlistbuf;		//!< ƒeƒNƒXƒ`ƒƒƒŠƒXƒgƒoƒbƒtƒ@
	NNS_TEXLIST*		texlist;		//!< ƒeƒNƒXƒ`ƒƒƒŠƒXƒg
	Sint32				regId;			//!< “o˜^‚h‚ciŠJ•úŠ®—¹ƒ`ƒFƒbƒN‚É•K—vj
#endif

#if GMD_MAP_FAR_EDIT
	Sint32      fogCursor;
	NNS_VECTOR	fogColor;				//!< ƒtƒHƒOƒJƒ‰[
	float		fogNear;
	float		fogFar;	

	NNS_VECTOR  lightDir;               //!< ƒ‰ƒCƒg•ûŒü
#endif
}GMS_MAP_FAR_DATA;

///‰“ŒiŠÇ—
typedef struct tag_GMS_MAP_FAR_MGR {	
	//ƒJƒƒ‰
	GMS_MAP_FAR_CAMERA camera;			///< ƒJƒƒ‰î•ñ

	//ƒ^ƒXƒN
	MTS_TASK_TCB* tcb_pre_draw;			///< •`‰æ‘Oˆ—TCB
	MTS_TASK_TCB* tcb_draw;				///< •`‰æˆ—TCB
	MTS_TASK_TCB* tcb_post_draw;		///< •`‰æŒãˆ—TCB
} GMS_MAP_FAR_MGR;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

// ƒŒƒ“ƒ_[ƒ^[ƒQƒbƒgi‹N“®‚ÉŠm•Û‚µ‚ÄAI—¹‚Ü‚Å‘ê‚âŠC‚Åg‚¢‚Ü‚·j
AMS_RENDER_TARGET	_gm_mapFar_render_work;	

//----- Static Declarations -------------------------------------------------

// ==========================================================================
//ƒQ[ƒ€İ’è
// ==========================================================================
static GSE_MAIN_STAGE_ID gmMapFarGetStageId( void );
static GSE_MAIN_ZONE_TYPE gmMapFarGetZoneType( GSE_MAIN_STAGE_ID stage_id );
static const MP_HEADER* gmMapFarGetMapsetMpA( void );
static const OBS_OBJECT* gmMapFarGetObject( void );

#if GMD_MAP_FAR_TEST_MPDATA
static BOOL gmMapFarCheckDisplayList( void );
#endif

static void gmMapFarClearColor( void );
//static void gmMapFarClearZBuffer( void );

// ==========================================================================
//ƒf[ƒ^ŠÇ—
// ==========================================================================
static void gmMapFarDataInit( void );
static void gmMapFarDataRelease( void );
static GMS_MAP_FAR_DATA* gmMapFarDataGetInfo( void );
static void gmMapFarDataSetAmbHeader( AMS_AMB_HEADER* amb );
static AMS_AMB_HEADER* gmMapFarDataGetAmbHeader( void );
static void gmMapFarDataReleaseAmbHeader( void );

//ƒIƒuƒWƒFƒNƒg
static GMS_MAP_FAR_OBJ_WORK* gmMapFarDataLoadObj( 
						  GMD_MAP_FAR_OBJ_INDEX obj_index,
						  OBS_ACTION3D_NN_WORK* obj_3d_work, 
						  GM_MAP_FAR_OBJ_FUNC main_func,
						  GM_MAP_FAR_OBJ_FUNC out_func);
static void gmMapFarDataClearObjWork( void );
static void gmMapFarDataSetObjWork( OBS_OBJECT_WORK* obj_work, GMD_MAP_FAR_OBJ_INDEX obj_index );
static OBS_OBJECT_WORK* gmMapFarDataGetObjWork( GMD_MAP_FAR_OBJ_INDEX obj_index );

//3DNNƒ‚ƒfƒ‹
#if GMD_MAP_FAR_TEST_MPDATA
static OBS_ACTION3D_NN_WORK* gmMapFarDataAllocNNModelWork( s32 num );
static OBS_ACTION3D_NN_WORK* gmMapFarDataGetNNModelWorkList( void );
static OBS_ACTION3D_NN_WORK* gmMapFarDataLoadNNModel( 
							 s32 model_index,
							 AMS_AMB_HEADER* model_amb,
							 AMS_AMB_HEADER* texture_amb);
static void gmMapFarDataIncNNModelRegistNum( void );
static s32 gmMapFarDataGetNNModelRegistNum( void );
static BOOL gmMapFarDataCheckNNModelLoad( void );
static OBS_ACTION3D_NN_WORK* gmMapFarDataGetNNModelWork( s32 index );
static s32 gmMapFarDataGetNNModelWorkNum( void );

//MPƒf[ƒ^
static void gmMapFarDataSetMpHeader( MP_HEADER* mp_header );
static MP_HEADER* gmMapFarDataGetMpHeader( void );
static void gmMapFarDataSetMdHeader( MD_HEADER* md_header );
static MD_HEADER* gmMapFarDataGetMdHeader( void );
#endif

static void gmMapFarDataFreeNNModelWork( void );

// ==========================================================================
//‰“ŒiŠÇ—
// ==========================================================================
//‰“Œiİ’è
static GMS_MAP_FAR_MGR* gmMapFarGetMgr( void );
static void gmMapFarInitMgr( void );
static void gmMapFarReleaseMgr( void );
static void gmMapFarExitMgr( void );

//•`‰æ‘Oˆ—
static MTS_TASK_TCB* gmMapFarCreateTcbPreDraw( void );
static void gmMapFarDeleteTcbPreDraw( void );
static void gmMapFarTcbProcPreDraw( MTS_TASK_TCB *tcb );
static void gmMapFarTcbProcPreDrawDT( void *data );

//•`‰æˆ—
static MTS_TASK_TCB* gmMapFarCreateTcbDraw( void );
static void gmMapFarDeleteTcbDraw( void );
static void gmMapFarChangeTcbProcDraw( const GSF_TASK_PROCEDURE proc );

//•`‰æŒãˆ—
static MTS_TASK_TCB* gmMapFarCreateTcbPostDraw( void );
static void gmMapFarDeleteTcbPostDraw( void );
static void gmMapFarTcbProcPostDraw( MTS_TASK_TCB *tcb );
static void gmMapFarTcbProcPostDrawDT( void *data );

// ==========================================================================
//ƒJƒƒ‰
// ==========================================================================
static GMS_MAP_FAR_CAMERA* gmMapFarCameraGetInfo( void );
static void gmMapFarCameraSetInfo( s32 camear_id, NNE_PROJECTION_TYPE camera_type );
static void gmMapFarCameraApply( void );
static void gmMapFarCameraSetSpeed( float speed_x, float speed_y );
static float gmMapFarCameraGetSpeedX( void );
static float gmMapFarCameraGetSpeedY( void );
static s32 gmMapFarCameraGetScrollDistance(
						   const GMS_MAP_FAR_SCROLL* scroll_list,
						   u32 scroll_info_num);
static float gmMapFarCameraGetPos(
						   float player_camera_pos,
						   const GMS_MAP_FAR_SCROLL* scroll_list,
						   u32 scroll_info_num,
						   float scroll_speed );
static NNS_VECTOR gmMapFarCameraGetPos( 
						   const NNS_VECTOR* player_camera_pos,
						   const GMS_MAP_FAR_SCROLL* scroll_list_x,
						   u32 scroll_info_num_x,
						   const GMS_MAP_FAR_SCROLL* scroll_list_y,
						   u32 scroll_info_num_y );

// ==========================================================================
//ƒV[ƒ“ƒf[ƒ^ŠÇ—
// ==========================================================================
static void gmMapFarSceneLoadObj( 
						  GMD_MAP_FAR_OBJ_INDEX obj_index,
						  OBS_ACTION3D_NN_WORK* obj_3d_work, 
						  u32 mat_motion_index,
						  AMS_AMB_HEADER* mat_amb_header,
						  u32 mtn_motion_index,
						  AMS_AMB_HEADER* mtn_amb_header,
						  GM_MAP_FAR_OBJ_FUNC main_func,
						  GM_MAP_FAR_OBJ_FUNC out_func,
						  u32 command_state);

static void gmMapFarSceneObjFuncDrawRotate( OBS_OBJECT_WORK* pWork );  // “V‹…
static void gmMapFarSceneObjFuncDrawSea( OBS_OBJECT_WORK* pWork );    // ŠC
static void gmMapFarDrawSeaUserFunc(void *data);
#if _WII
static NNE_BOOL gmMapFarWaterFallMaterial(NNS_DRAWCALLBACK_VAL *val, void *param);
#endif

#if GMD_MAP_FAR_ROCK_PRIMITIVE
static void gmMapFarSceneObjFuncDrawRockPrimitive( OBS_OBJECT_WORK* pWork );    // Šâ

static Uint32 gmMapFarSceneObjFuncDrawRockAPrimitive( NNS_PRIM3D_PCT *poliData, Uint32 startId );    // ŠâA
static Uint32 gmMapFarSceneObjFuncDrawRockBPrimitive( NNS_PRIM3D_PCT *poliData, Uint32 startId );    // ŠâB
static Uint32 gmMapFarSceneObjFuncDrawRockCPrimitive( NNS_PRIM3D_PCT *poliData, Uint32 startId );    // ŠâC

// Šâ‚Ì”æ“¾
static Uint32 gmMapFarSceneGetRockANum( void );
static Uint32 gmMapFarSceneGetRockBNum( void );
static Uint32 gmMapFarSceneGetRockCNum( void );
static Uint32 gmMapFarSceneObjFuncSetRockPrimitiveVtx( NNS_PRIM3D_PCT *poliData, AOS_TVX_VERTEX* vtx, Uint32 vnum,
												   Uint32 startId, Sint32 finalFlag, float ofx, float ofz, float scale );
#endif

#if GMD_MAP_FAR_2DBG
static void gmMapFarSceneObjFuncDraw2DBG( OBS_OBJECT_WORK* pWork );    // ”Âƒ|ƒŠ‰“Œi
#endif

static void gmMapFarSceneObjFuncDrawRockA( OBS_OBJECT_WORK* pWork );    // Šâ
static void gmMapFarSceneObjFuncDrawRockB( OBS_OBJECT_WORK* pWork );    // Šâ
static void gmMapFarSceneObjFuncDrawRockC( OBS_OBJECT_WORK* pWork );    // Šâ
static Sint32 gmMapFarDrawCheckRock(void);
static Sint32 gmMapFarDrawCheckYakei(void);
static void gmMapFarSceneObjFuncDrawBg( OBS_OBJECT_WORK* pWork );    // ƒrƒ‹ŒQ‚â’ŒŒQ‚È‚Ç
static void gmMapFarSceneObjFuncDrawWheel( OBS_OBJECT_WORK* pWork );    // ŠÏ——Ô
static void gmMapFarSceneObjFuncDrawSLight( OBS_OBJECT_WORK* pWork );    // ƒT[ƒ`ƒ‰ƒCƒg

// ==========================================================================
//ƒc[ƒ‹ƒf[ƒ^ŠÇ—
// ==========================================================================
#if GMD_MAP_FAR_TEST_MPDATA
static void gmMapFarMpBuild(
					 GMD_MAP_FAR_DATA_INDEX model_amb_index,
					 GMD_MAP_FAR_DATA_INDEX mapset_amb_index,
					 GMD_MAP_FAR_DATA_INDEX mp_header_index,
					 GMD_MAP_FAR_DATA_INDEX md_header_index);
static BOOL gmMapFarMpLoading( 
					   GMD_MAP_FAR_DATA_INDEX model_index,
					   GMD_MAP_FAR_DATA_INDEX texture_index );
static void gmMapFarMpDraw( void );
#endif //GMD_MAP_FAR_TEST_MPDATA
		
// ==========================================================================
//ƒ][ƒ“1
// ==========================================================================
static void gmMapFarZone1Build( void );

#if GMD_MAP_FAR_ROCK_PRIMITIVE
static void gmMapFarZone1BuildRock( void );
#endif

static BOOL gmMapFarZone1CheckLoading( void );
static void gmMapFarZone1Flush( void );
static void gmMapFarZone1Init( void );
static void gmMapFarZone1Release( void );
static void gmMpaFarZone1TcbProcDraw( MTS_TASK_TCB* tcb );
static NNS_VECTOR gmMapFarZone1GetCameraPos( const NNS_VECTOR* player_camera_pos );
		
// ==========================================================================
//ƒ][ƒ“2
// ==========================================================================
static void gmMapFarZone2Build( void );
static BOOL gmMapFarZone2CheckLoading( void );
static void gmMapFarZone2Flush( void );
static void gmMapFarZone2Init( void );
static void gmMapFarZone2Release( void );
static void gmMpaFarZone2TcbProcDraw( MTS_TASK_TCB* tcb );
static NNS_VECTOR gmMapFarZone2GetCameraPos( const NNS_VECTOR* player_camera_pos );
		
// ==========================================================================
//ƒ][ƒ“3
// ==========================================================================
static void gmMapFarZone3Build( void );
static BOOL gmMapFarZone3CheckLoading( void );
static void gmMapFarZone3Flush( void );
static void gmMapFarZone3Init( void );
static void gmMapFarZone3Release( void );
static void gmMpaFarZone3TcbProcDraw( MTS_TASK_TCB* tcb );
static NNS_VECTOR gmMapFarZone3GetCameraPos( const NNS_VECTOR* player_camera_pos );
		
// ==========================================================================
//ƒ][ƒ“4
// ==========================================================================
static void gmMapFarZone4Build( void );
static BOOL gmMapFarZone4CheckLoading( void );
static void gmMapFarZone4Flush( void );
static void gmMapFarZone4Init( void );
static void gmMapFarZone4Release( void );
static void gmMpaFarZone4TcbProcDraw( MTS_TASK_TCB* tcb );
//static NNS_VECTOR gmMapFarZone4GetCameraPos( const NNS_VECTOR* player_camera_pos );
		
// ==========================================================================
//ƒ][ƒ“FINAL
// ==========================================================================
static void gmMapFarZoneFinalBuild( void );
static BOOL gmMapFarZoneFinalCheckLoading( void );
static void gmMapFarZoneFinalFlush( void );
static void gmMapFarZoneFinalInit( void );
static void gmMapFarZoneFinalRelease( void );
static void gmMpaFarZoneFinalTcbProcDraw( MTS_TASK_TCB* tcb );
static NNS_VECTOR gmMapFarZoneFinalGetCameraPos( const NNS_VECTOR* player_camera_pos );

// ==========================================================================
// ƒXƒyƒXƒe
// ==========================================================================
static void gmMapFarZoneSSBuild( void );
static BOOL gmMapFarZoneSSCheckLoading( void );
static void gmMapFarZoneSSFlush( void );
static void gmMapFarZoneSSInit( void );
static void gmMapFarZoneSSRelease( void );
static void gmMpaFarZoneSSTcbProcDraw( MTS_TASK_TCB* tcb );
static NNS_VECTOR gmMapFarZoneSSGetCameraPos( const NNS_VECTOR* player_camera_pos );

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

// ==========================================================================
///ƒf[ƒ^ŠÇ—
// ==========================================================================
//ƒf[ƒ^
static GMS_MAP_FAR_DATA g_map_far_data_real;
static GMS_MAP_FAR_DATA* g_map_far_data = NULL;

// ==========================================================================
///‰“ŒiŠÇ—
// ==========================================================================
//ŠÇ—î•ñ
static GMS_MAP_FAR_MGR g_map_far_mgr_real;
static GMS_MAP_FAR_MGR* g_map_far_mgr = NULL;

// ==========================================================================
//ƒ][ƒ“1
// ==========================================================================
//ƒV[ƒ“OBJ—pƒ‚ƒfƒ‹ƒf[ƒ^
static const u32 g_map_far_zone_1_scene_obj_data[GMD_MAP_FAR_OBJ_INDEX_ZONE_1_MAX] = {
#if _IPHONE
	IDB_MAPFAR_ZONE1_RENDER_MDL_Z1_SEA_D_ZNO,
	IDB_MAPFAR_ZONE1_MDL_Z1_SKYL_T_D_INO,
	IDB_MAPFAR_ZONE1_MDL_Z1_ROCK_A_T_D_INO,
	IDB_MAPFAR_ZONE1_MDL_Z1_ROCK_B_T_D_INO,
	IDB_MAPFAR_ZONE1_MDL_Z1_ROCK_C_T_D_INO,
#if GMD_MAP_FAR_2DBG
	IDB_MAPFAR_ZONE1_MDL_Z1_BOSS_FAR_INO,
#endif
#else
	IDB_MAPFAR_ZONE1_MDL_Z1_SKYL_B_D_ZNO,
	IDB_MAPFAR_ZONE1_MDL_Z1_ROCK_A_B_D_ZNO,
	IDB_MAPFAR_ZONE1_MDL_Z1_ROCK_B_B_D_ZNO,
	IDB_MAPFAR_ZONE1_MDL_Z1_ROCK_C_B_D_ZNO,
	IDB_MAPFAR_ZONE1_RENDER_MDL_Z1_SEA_D_ZNO,
	IDB_MAPFAR_ZONE1_MDL_Z1_SKYL_T_D_ZNO,
	IDB_MAPFAR_ZONE1_MDL_Z1_ROCK_A_T_D_ZNO,
	IDB_MAPFAR_ZONE1_MDL_Z1_ROCK_B_T_D_ZNO,
	IDB_MAPFAR_ZONE1_MDL_Z1_ROCK_C_T_D_ZNO,
#if GMD_MAP_FAR_2DBG
	IDB_MAPFAR_ZONE1_MDL_Z1_BOSS_IP_FAR_ZNO,
#endif
#endif
};
//ƒV[ƒ“OBJ—pƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“ƒf[ƒ^
static const u32 g_map_far_zone_1_scene_obj_data_mat_motion[GMD_MAP_FAR_OBJ_INDEX_ZONE_1_MAX] = {
#if _IPHONE
	IDB_MAPFAR_ZONE1_MAT_Z1_SEA_D_INV,
	IDB_MAPFAR_ZONE1_MAT_Z1_SKYL_T_D_INV,
	GMD_MAP_FAR_DATA_INDEX_INVALID,
	GMD_MAP_FAR_DATA_INDEX_INVALID,
	GMD_MAP_FAR_DATA_INDEX_INVALID,
#else
	IDB_MAPFAR_ZONE1_MAT_Z1_SKYL_T_D_ZNV,
	GMD_MAP_FAR_DATA_INDEX_INVALID,
	GMD_MAP_FAR_DATA_INDEX_INVALID,
	GMD_MAP_FAR_DATA_INDEX_INVALID,
	IDB_MAPFAR_ZONE1_MAT_Z1_SEA_D_ZNV,
	IDB_MAPFAR_ZONE1_MAT_Z1_SKYL_T_D_ZNV,
	GMD_MAP_FAR_DATA_INDEX_INVALID,
	GMD_MAP_FAR_DATA_INDEX_INVALID,
	GMD_MAP_FAR_DATA_INDEX_INVALID,
#endif

#if GMD_MAP_FAR_2DBG
	IDB_MAPFAR_ZONE1_MAT_Z1_BOSS_IP_FAR_INV,
#endif
};

//ƒV[ƒ“OBJ—pƒƒCƒ“ŠÖ”
static const GM_MAP_FAR_OBJ_FUNC g_map_far_zone_1_scene_obj_func_main[GMD_MAP_FAR_OBJ_INDEX_ZONE_1_MAX] = {
#if _IPHONE
	NULL, //sea
	NULL,
	NULL,
	NULL,
	NULL,
#else
	NULL,
	NULL,
	NULL,
	NULL,
	NULL, //sea
	NULL,
	NULL,
	NULL,
	NULL,
#endif
#if GMD_MAP_FAR_2DBG
	NULL,
#endif
};

//ƒV[ƒ“OBJ—p•`‰æŠÖ”
static const GM_MAP_FAR_OBJ_FUNC g_map_far_zone_1_scene_obj_func_out[GMD_MAP_FAR_OBJ_INDEX_ZONE_1_MAX] = {
#if _IPHONE
	gmMapFarSceneObjFuncDrawSea,
	gmMapFarSceneObjFuncDrawRotate,
	gmMapFarSceneObjFuncDrawRockA,
	gmMapFarSceneObjFuncDrawRockB,
	gmMapFarSceneObjFuncDrawRockC,
#else
	gmMapFarSceneObjFuncDrawRotate,
	gmMapFarSceneObjFuncDrawRockA,
	gmMapFarSceneObjFuncDrawRockB,
	gmMapFarSceneObjFuncDrawRockC,
	gmMapFarSceneObjFuncDrawSea,
	gmMapFarSceneObjFuncDrawRotate,
	gmMapFarSceneObjFuncDrawRockA,
	gmMapFarSceneObjFuncDrawRockB,
	gmMapFarSceneObjFuncDrawRockC,
#endif
#if GMD_MAP_FAR_2DBG
	gmMapFarSceneObjFuncDraw2DBG,
#endif
};

//ƒXƒNƒ[ƒ‹İ’èX
static const GMS_MAP_FAR_SCROLL g_map_far_zone_1_scroll_x[] = {
	{0, 600, 1},
};
// ƒXƒNƒ[ƒ‹İ’èX BOSS
static const GMS_MAP_FAR_SCROLL g_map_far_zone_1_boss_scroll_x[] = {
	{0, 10, 1},
};
//ƒXƒNƒ[ƒ‹İ’è”X
static const u32 g_map_far_zone_1_scroll_num_x = sizeof(g_map_far_zone_1_scroll_x)/sizeof(GMS_MAP_FAR_SCROLL);
//ƒXƒNƒ[ƒ‹İ’èY
static const GMS_MAP_FAR_SCROLL g_map_far_zone_1_scroll_y[] = {
	{0, 20, 1},
};
//ƒXƒNƒ[ƒ‹İ’èY BOSS
static const GMS_MAP_FAR_SCROLL g_map_far_zone_1_boss_scroll_y[] = {
	{0, 0, 0},
};
//ƒXƒNƒ[ƒ‹İ’è”Y
static const u32 g_map_far_zone_1_scroll_num_y = sizeof(g_map_far_zone_1_scroll_y)/sizeof(GMS_MAP_FAR_SCROLL);

// ==========================================================================
//ƒ][ƒ“2
// ==========================================================================

//ƒV[ƒ“OBJ—pƒ‚ƒfƒ‹ƒf[ƒ^
static const u32 g_map_far_zone_2_scene_obj_data[GMD_MAP_FAR_OBJ_INDEX_ZONE_2_MAX] = {
#if _IPHONE
	IDB_MAPFAR_ZONE2_MDL_Z2_BOSS_IP_FAR_INO,
	IDB_MAPFAR_ZONE2_MDL_BG_02_WHEEL_INO,
	IDB_MAPFAR_ZONE2_MDL_ZONE2_SLIGHT_INO,
#else
	IDB_MAPFAR_ZONE2_MDL_BG_02_SKY_ZNO,			/* ‹ó */
	IDB_MAPFAR_ZONE2_MDL_BG_02_GROUND_ZNO,		/* ’n–Ê‚Æƒrƒ‹ŒQ */
	IDB_MAPFAR_ZONE2_MDL_BG_02_WHEEL_ZNO,		/* ŠÏ——Ô */
	IDB_MAPFAR_ZONE2_MDL_BG_02_GLARE_ZNO,		/* ƒOƒŒƒA */
	IDB_MAPFAR_ZONE2_MDL_BG_02_SLIGHT_ZNO,		/* ƒT[ƒ`ƒ‰ƒCƒg */
#endif
};

//ƒV[ƒ“OBJ—pƒ‚[ƒVƒ‡ƒ“ƒf[ƒ^
static const u32 g_map_far_zone_2_scene_obj_data_motion[GMD_MAP_FAR_OBJ_INDEX_ZONE_2_MAX] = {
#if _IPHONE
	GMD_MAP_FAR_DATA_INDEX_INVALID,
	IDB_MAPFAR_ZONE2_MTN_BG_02_WHEEL_INM,
	IDB_MAPFAR_ZONE2_MTN_ZONE2_SLIGHT_INM,
#else
	GMD_MAP_FAR_DATA_INDEX_INVALID,				/* ‹ó */
	GMD_MAP_FAR_DATA_INDEX_INVALID,				/* ’n–Ê‚Æƒrƒ‹ŒQ */
	IDB_MAPFAR_ZONE2_MTN_BG_02_WHEEL_ZNM,		/* ŠÏ——Ô */
	GMD_MAP_FAR_DATA_INDEX_INVALID,				/* ƒOƒŒƒA */
	IDB_MAPFAR_ZONE2_MTN_BG_02_SLIGHT_ZNM,		/* ƒT[ƒ`ƒ‰ƒCƒg */
#endif
};

//ƒV[ƒ“OBJ—pƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“ƒf[ƒ^
static const u32 g_map_far_zone_2_scene_obj_data_mat_motion[GMD_MAP_FAR_OBJ_INDEX_ZONE_2_MAX] = {
#if _IPHONE
	GMD_MAP_FAR_DATA_INDEX_INVALID,
	IDB_MAPFAR_ZONE2_MAT_BG_02_WHEEL_INV,
	GMD_MAP_FAR_DATA_INDEX_INVALID,
#else
	GMD_MAP_FAR_DATA_INDEX_INVALID,				/* ‹ó */
	IDB_MAPFAR_ZONE2_MAT_BG_02_GROUND_ZNV,		/* ’n–Ê‚Æƒrƒ‹ŒQ */
	IDB_MAPFAR_ZONE2_MAT_BG_02_WHEEL_ZNV,		/* ŠÏ——Ô */
	IDB_MAPFAR_ZONE2_MAT_BG_02_GLARE_ZNV,		/* ƒOƒŒƒA */
	GMD_MAP_FAR_DATA_INDEX_INVALID,				/* ƒT[ƒ`ƒ‰ƒCƒg */
#endif
};

//ƒV[ƒ“OBJ—pƒƒCƒ“ŠÖ”
static const GM_MAP_FAR_OBJ_FUNC g_map_far_zone_2_scene_obj_func_main[GMD_MAP_FAR_OBJ_INDEX_ZONE_2_MAX] = {
#if _IPHONE
	NULL,
	NULL,
	NULL,
#else
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
#endif
};

//ƒV[ƒ“OBJ—p•`‰æŠÖ”
static const GM_MAP_FAR_OBJ_FUNC g_map_far_zone_2_scene_obj_func_out[GMD_MAP_FAR_OBJ_INDEX_ZONE_2_MAX] = {
#if _IPHONE
	gmMapFarSceneObjFuncDraw2DBG,   // 2D‰“Œi
	gmMapFarSceneObjFuncDrawWheel,  // ŠÏ——Ô
	gmMapFarSceneObjFuncDrawSLight, // ƒT[ƒ`ƒ‰ƒCƒg
#else
	gmMapFarSceneObjFuncDrawRotate, // ‹ó
	gmMapFarSceneObjFuncDrawBg,		// ƒrƒ‹
	gmMapFarSceneObjFuncDrawWheel,  // ŠÏ——Ô
	gmMapFarSceneObjFuncDrawBg,		// ƒOƒŒƒA
	gmMapFarSceneObjFuncDrawSLight, // ƒT[ƒ`ƒ‰ƒCƒg
#endif
};

//ƒXƒNƒ[ƒ‹İ’èX
#if _IPHONE
static const GMS_MAP_FAR_SCROLL g_map_far_zone_2_scroll_x[] = {
	{0, 90, 1},
};
#else
static const GMS_MAP_FAR_SCROLL g_map_far_zone_2_scroll_x[] = {
	{0, 300, 1},
};
#endif
// ƒXƒNƒ[ƒ‹İ’èX BOSS
static const GMS_MAP_FAR_SCROLL g_map_far_zone_2_boss_scroll_x[] = {
	{0, 20, 1},
};
//ƒXƒNƒ[ƒ‹İ’è”X
static const u32 g_map_far_zone_2_scroll_num_x = sizeof(g_map_far_zone_2_scroll_x)/sizeof(GMS_MAP_FAR_SCROLL);

#if _IPHONE
//ƒXƒNƒ[ƒ‹İ’èY
static const GMS_MAP_FAR_SCROLL g_map_far_zone_2_scroll_y[] = {
	{0, 40, 1},
};
//ƒXƒNƒ[ƒ‹İ’èY BOSS
static const GMS_MAP_FAR_SCROLL g_map_far_zone_2_boss_scroll_y[] = {
	{0, 40, 1},
};
#else
//ƒXƒNƒ[ƒ‹İ’èY
static const GMS_MAP_FAR_SCROLL g_map_far_zone_2_scroll_y[] = {
	{0, 50, 1},
};
//ƒXƒNƒ[ƒ‹İ’èY BOSS
static const GMS_MAP_FAR_SCROLL g_map_far_zone_2_boss_scroll_y[] = {
	{0, 50, 1},
};
#endif
//ƒXƒNƒ[ƒ‹İ’è”Y
static const u32 g_map_far_zone_2_scroll_num_y = sizeof(g_map_far_zone_2_scroll_y)/sizeof(GMS_MAP_FAR_SCROLL);
		
// ==========================================================================
//ƒ][ƒ“3
// ==========================================================================
//ƒV[ƒ“OBJ—pƒf[ƒ^iƒ‚ƒfƒ‹AƒeƒNƒXƒ`ƒƒj
static const u32 g_map_far_zone_3_scene_obj_data[GMD_MAP_FAR_OBJ_INDEX_ZONE_3_MAX] = {
#if _IPHONE
	IDB_MAPFAR_ZONE3_MDL_Z3_FAR_INO,
#else
	IDB_MAPFAR_ZONE3_MDL_Z3_FAR_ZNO,
#endif
};

//ƒV[ƒ“OBJ—pƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“ƒf[ƒ^
static const u32 g_map_far_zone_3_scene_obj_data_mat_motion[GMD_MAP_FAR_OBJ_INDEX_ZONE_3_MAX] = {
#if _IPHONE
	GMD_MAP_FAR_DATA_INDEX_INVALID,
#else
	IDB_MAPFAR_ZONE3_MAT_Z3_FAR_ZNV,	/* ‹ó‚©‚çË‚µ‚ŞŒõ */
#endif
};

//ƒV[ƒ“OBJ—pƒƒCƒ“ŠÖ”
static const GM_MAP_FAR_OBJ_FUNC g_map_far_zone_3_scene_obj_func_main[GMD_MAP_FAR_OBJ_INDEX_ZONE_3_MAX] = {
#if _IPHONE
	NULL,
#else
	NULL,
#endif
};
//ƒV[ƒ“OBJ—p•`‰æŠÖ”
static const GM_MAP_FAR_OBJ_FUNC g_map_far_zone_3_scene_obj_func_out[GMD_MAP_FAR_OBJ_INDEX_ZONE_3_MAX] = {
#if _IPHONE
	gmMapFarSceneObjFuncDraw2DBG,
#else
	gmMapFarSceneObjFuncDrawBg,
#endif
};

//ƒXƒNƒ[ƒ‹İ’èX
#if _IPHONE
static const GMS_MAP_FAR_SCROLL g_map_far_zone_3_scroll_x[] = {
	//{0, 90, 1,},
	{0, 678, 1,},
};
#else
static const GMS_MAP_FAR_SCROLL g_map_far_zone_3_scroll_x[] = {
	{0, 600, 1,},
};
#endif
//ƒXƒNƒ[ƒ‹İ’è”X
static const u32 g_map_far_zone_3_scroll_num_x = sizeof(g_map_far_zone_3_scroll_x)/sizeof(GMS_MAP_FAR_SCROLL);

#if _IPHONE
static const GMS_MAP_FAR_SCROLL g_map_far_zone_3_scroll_y[] = {
	{0, 51, 1,},
};
#else
//ƒXƒNƒ[ƒ‹İ’èY
static const GMS_MAP_FAR_SCROLL g_map_far_zone_3_scroll_y[] = {
	{0, 100, 1,},
};
#endif
//ƒXƒNƒ[ƒ‹İ’è”Y
static const u32 g_map_far_zone_3_scroll_num_y = sizeof(g_map_far_zone_3_scroll_y)/sizeof(GMS_MAP_FAR_SCROLL);
		
// ==========================================================================
//ƒ][ƒ“4
// ==========================================================================
//ƒXƒNƒ[ƒ‹İ’èX
static const GMS_MAP_FAR_SCROLL g_map_far_zone_4_scroll_x[] = {
	{0, 0, 0,},
};
//ƒXƒNƒ[ƒ‹İ’è”X
static const u32 g_map_far_zone_4_scroll_num_x = sizeof(g_map_far_zone_4_scroll_x)/sizeof(GMS_MAP_FAR_SCROLL);
//ƒXƒNƒ[ƒ‹İ’èY
static const GMS_MAP_FAR_SCROLL g_map_far_zone_4_scroll_y[] = {
	{0, 0, 0,},
};
//ƒXƒNƒ[ƒ‹İ’è”Y
static const u32 g_map_far_zone_4_scroll_num_y = sizeof(g_map_far_zone_4_scroll_y)/sizeof(GMS_MAP_FAR_SCROLL);
		
// ==========================================================================
//ƒ][ƒ“FINAL
// ==========================================================================
//ƒV[ƒ“OBJ—pƒf[ƒ^iƒ‚ƒfƒ‹AƒeƒNƒXƒ`ƒƒj
static const u32 g_map_far_zone_final_scene_obj_data[GMD_MAP_FAR_OBJ_INDEX_ZONE_FINAL_MAX] = {
#if _IPHONE
	IDB_MAPFAR_ZONEF_MDL_ZF_BOSS_IP_FAR_INO,
#else
	IDB_MAPFAR_ZONEF_MDL_ZFF_SPACE_ZNO,
	IDB_MAPFAR_ZONEF_MDL_ZFF_EARTH_ZNO,
	IDB_MAPFAR_ZONEF_MDL_ZFF_CLOUD_ZNO,
	IDB_MAPFAR_ZONEF_MDL_ZFF_FILM_A_ZNO,
#endif
};

//ƒV[ƒ“OBJ—pƒƒCƒ“ŠÖ”
static const GM_MAP_FAR_OBJ_FUNC g_map_far_zone_final_scene_obj_func_main[GMD_MAP_FAR_OBJ_INDEX_ZONE_FINAL_MAX] = {
#if _IPHONE
	NULL,
#else
	NULL,
	NULL,
	NULL,
	NULL,
#endif
};
//ƒV[ƒ“OBJ—p•`‰æŠÖ”
static const GM_MAP_FAR_OBJ_FUNC g_map_far_zone_final_scene_obj_func_out[GMD_MAP_FAR_OBJ_INDEX_ZONE_FINAL_MAX] = {
#if _IPHONE
	gmMapFarSceneObjFuncDraw2DBG,
#else
	gmMapFarSceneObjFuncDrawBg,
	gmMapFarSceneObjFuncDrawRotate,
	gmMapFarSceneObjFuncDrawRotate,
	gmMapFarSceneObjFuncDrawRotate,
#endif
};

//ƒXƒNƒ[ƒ‹İ’èX
static const GMS_MAP_FAR_SCROLL g_map_far_zone_final_scroll_x[] = {
	{0, 50, 1,},
};
//ƒXƒNƒ[ƒ‹İ’è”X
static const u32 g_map_far_zone_final_scroll_num_x = sizeof(g_map_far_zone_final_scroll_x)/sizeof(GMS_MAP_FAR_SCROLL);
//ƒXƒNƒ[ƒ‹İ’èY
static const GMS_MAP_FAR_SCROLL g_map_far_zone_final_scroll_y[] = {
	{0, 10, 1,},
};
//ƒXƒNƒ[ƒ‹İ’è”Y
static const u32 g_map_far_zone_final_scroll_num_y = sizeof(g_map_far_zone_final_scroll_y)/sizeof(GMS_MAP_FAR_SCROLL);

// ==========================================================================
// ƒXƒyƒXƒe
// ==========================================================================
//ƒV[ƒ“OBJ—pƒ‚ƒfƒ‹ƒf[ƒ^
static const u32 g_map_far_zone_ss_scene_obj_data[GMD_MAP_FAR_OBJ_INDEX_ZONE_SS_MAX] = {
	IDB_MAPFAR_SS01_MDL_SS_KALEIDO_01_ZNO,
};
//ƒV[ƒ“OBJ—pƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“ƒf[ƒ^
static const u32 g_map_far_zone_ss_scene_obj_data_mat_motion[GMD_MAP_FAR_OBJ_INDEX_ZONE_SS_MAX] = {
	IDB_MAPFAR_SS01_MAT_SS_KALEIDO_01_ZNV,
};
//ƒV[ƒ“OBJ—pƒƒCƒ“ŠÖ”
static const GM_MAP_FAR_OBJ_FUNC g_map_far_zone_ss_scene_obj_func_main[GMD_MAP_FAR_OBJ_INDEX_ZONE_SS_MAX] = {
	NULL,
};
//ƒV[ƒ“OBJ—p•`‰æŠÖ”
static const GM_MAP_FAR_OBJ_FUNC g_map_far_zone_ss_scene_obj_func_out[GMD_MAP_FAR_OBJ_INDEX_ZONE_SS_MAX] = {
	gmMapFarSceneObjFuncDrawBg,
};
//ƒXƒNƒ[ƒ‹İ’èX
static const GMS_MAP_FAR_SCROLL g_map_far_zone_ss_scroll_x[] = {
//	{0, 300, 1,},
	{0, 0, 1},
};
//ƒXƒNƒ[ƒ‹İ’è”X
static const u32 g_map_far_zone_ss_scroll_num_x = sizeof(g_map_far_zone_ss_scroll_x)/sizeof(GMS_MAP_FAR_SCROLL);
//ƒXƒNƒ[ƒ‹İ’èY
static const GMS_MAP_FAR_SCROLL g_map_far_zone_ss_scroll_y[] = {
//	{0, 50, 1,},
	{0, 0, 0},
};
//ƒXƒNƒ[ƒ‹İ’è”Y
static const u32 g_map_far_zone_ss_scroll_num_y = sizeof(g_map_far_zone_ss_scroll_y)/sizeof(GMS_MAP_FAR_SCROLL);


// ==========================================================================
//ƒ][ƒ“‘S‘Ì
// ==========================================================================
//ƒJƒƒ‰‚Ìƒ^[ƒQƒbƒgÀ•WƒIƒtƒZƒbƒg
static const NNS_VECTOR g_gm_map_far_camera_target_offset[GSD_MAIN_ZONE_TYPE_MAX] = {
#if GMD_MAP_FAR_TEST_MPDATA
	{0.0f, 0.0f, -GMD_MAP_FAR_ZONE_1_CAMERA_POS_Z,},
	{0.0f, 0.0f, 0.0f,},
	{0.0f, 0.0f, -GMD_MAP_FAR_ZONE_1_CAMERA_POS_Z,},
#else
	{-GMD_MAP_FAR_ZONE_1_CAMERA_POS_Z, 0.0f, 0.0f,},
	{-GMD_MAP_FAR_ZONE_1_CAMERA_POS_Z, 0.0f, 0.0f,},
	{-GMD_MAP_FAR_ZONE_1_CAMERA_POS_Z, 0.0f, 0.0f,},
#endif	//GMD_MAP_FAR_TEST_MPDATA
	{-GMD_MAP_FAR_ZONE_1_CAMERA_POS_Z, 0.0f, 0.0f,},
	{-GMD_MAP_FAR_ZONE_1_CAMERA_POS_Z, 0.0f, 0.0f,},
	{0.0f, 0.0f, -200.0f,},	// SpecialStage
};

//ƒNƒŠƒAƒJƒ‰[
static NNS_RGBA_U8 g_gm_map_far_clear_color[GSD_MAIN_ZONE_TYPE_MAX] = {
#if _IPHONE
	{ 0x5a, 0x30, 0x10, 0xff},  // Zone1
#else
	{ 0x44, 0x66, 0xff, 0xff},  // Zone1
#endif // _IPHONE
	{ 0x00, 0x00, 0x00, 0xff},  // Zone2
	{ 0x55, 0x76, 0x6D, 0xff},  // Zone3
#if _IPHONE
	{ 0x00, 0x00, 0x00, 0xff},  // Zone4
#else
	{ 0x44, 0xff, 0xff, 0xff},  // Zone4
#endif // _IPHONE
	{ 0x00, 0x00, 0x00, 0xff},  // Final
	{ 0xff, 0xff, 0xff, 0xff},	// SpecialStage
};


//----- Global Functions ----------------------------------------------------

// ==========================================================================
//ƒf[ƒ^
// ==========================================================================


// ==========================================================================
// GmMapFarInitData
/*!
 * ‰“Œiƒf[ƒ^‚ğ‰Šú‰»
 *
 * @param amb ambƒf[ƒ^ƒwƒbƒ_
 */
// ==========================================================================
void GmMapFarInitData( AMS_AMB_HEADER* amb )
{
	amAssert( !g_map_far_data );
	amAssert( amb );

	//ƒf[ƒ^ŠÇ—‰Šú‰»
	gmMapFarDataInit();

	//ƒRƒ“ƒo[ƒg
	amBindConv( (u8*)amb );

	//“o˜^
	gmMapFarDataSetAmbHeader( amb );
}

// ==========================================================================
// GmMapFarBuildData
/*!
 * ‰“Œiƒf[ƒ^\’z
 */
// ==========================================================================
void GmMapFarBuildData( void )
{
	//-------------------------------------------------
	//‹¤’Êˆ—
	//-------------------------------------------------
	//-------------------------------------------------
	//ƒ][ƒ“ê—pˆ—
	//-------------------------------------------------
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmMapFarGetZoneType(stage_id);
	switch (zone_id){
		case GSD_MAIN_ZONE_TYPE_1:
			gmMapFarZone1Build();
			break;
		case GSD_MAIN_ZONE_TYPE_2:
			gmMapFarZone2Build();
			break;
		case GSD_MAIN_ZONE_TYPE_3:
			gmMapFarZone3Build();
			break;
		case GSD_MAIN_ZONE_TYPE_4:
			gmMapFarZone4Build();
			break;
		case GSD_MAIN_ZONE_TYPE_FINAL:
			gmMapFarZoneFinalBuild();
			break;
		case GSD_MAIN_ZONE_TYPE_SS:	// SpecialStage
			gmMapFarZoneSSBuild();
			break;
		default:
			amAssert(0);
			break;
	}
}

// ==========================================================================
// GmMapFarCheckLoading
/*!
 * “Ç‚İ‚İ‘Ò‚¿ƒ`ƒFƒbƒN
 *
 * @return TRUEF“Ç‚İ‚İI—¹ FALSEF“Ç‚İ‚İ‘Ò‚¿
 *
 */
// ==========================================================================
BOOL GmMapFarCheckLoading( void )
{
	//-------------------------------------------------
	//‹¤’Êˆ—
	//-------------------------------------------------
	//-------------------------------------------------
	//ƒ][ƒ“ê—pˆ—
	//-------------------------------------------------
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmMapFarGetZoneType(stage_id);
	switch (zone_id){
		case GSD_MAIN_ZONE_TYPE_1:
			return gmMapFarZone1CheckLoading();
		case GSD_MAIN_ZONE_TYPE_2:
			return gmMapFarZone2CheckLoading();
		case GSD_MAIN_ZONE_TYPE_3:
			return gmMapFarZone3CheckLoading();
		case GSD_MAIN_ZONE_TYPE_4:
			return gmMapFarZone4CheckLoading();
		case GSD_MAIN_ZONE_TYPE_FINAL:
			return gmMapFarZoneFinalCheckLoading();
		case GSD_MAIN_ZONE_TYPE_SS:
			return gmMapFarZoneSSCheckLoading();
		default:
			amAssert(0);
			break;
	}

	return TRUE;
}

// ==========================================================================
// GmMapFarInit
/*!
 * ‰“Œi‰Šú‰»
 */
// ==========================================================================
void GmMapFarInit( void )
{
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmMapFarGetZoneType(stage_id);

	//-------------------------------------------------
	//‹¤’Êˆ—
	//-------------------------------------------------
	amAssert(!g_map_far_mgr);

	//ŠÇ—î•ñ‰Šú‰»
	gmMapFarInitMgr();

	//•`‰æ‘Oˆ—ƒ^ƒXƒNì¬
	gmMapFarCreateTcbPreDraw();

	//•`‰æƒ^ƒXƒNì¬
	gmMapFarCreateTcbDraw();

	//•`‰æŒãˆ—ƒ^ƒXƒNì¬
	gmMapFarCreateTcbPostDraw();

	//-------------------------------------------------
	//ƒ][ƒ“ê—pˆ—
	//-------------------------------------------------
	switch (zone_id){
		case GSD_MAIN_ZONE_TYPE_1:
			gmMapFarZone1Init();
			break;
		case GSD_MAIN_ZONE_TYPE_2:
			gmMapFarZone2Init();
			break;
		case GSD_MAIN_ZONE_TYPE_3:
			gmMapFarZone3Init();
			break;
		case GSD_MAIN_ZONE_TYPE_4:
			gmMapFarZone4Init();
			break;
		case GSD_MAIN_ZONE_TYPE_FINAL:
			gmMapFarZoneFinalInit();
			break;
		case GSD_MAIN_ZONE_TYPE_SS:
			gmMapFarZoneSSInit();
			break;
		default:
			amAssert(0);
			break;
	}
}

// ==========================================================================
// GmMapFarExit
/*!
 * ‰“ŒiI—¹
 */
// ==========================================================================
void GmMapFarExit( void )
{
	//-------------------------------------------------
	//‹¤’Êˆ—
	//-------------------------------------------------
	gmMapFarDataClearObjWork();
	gmMapFarExitMgr();
}

// ==========================================================================
// GmMapFarFlushData
/*!
 * ‰“Œi“Ç‚İ‚ñ‚¾ƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
void GmMapFarFlushData( void )
{
//	amRenderDelete(&g_map_far_data->render_work);

	//-------------------------------------------------
	//ƒ][ƒ“ê—pˆ—
	//-------------------------------------------------
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmMapFarGetZoneType(stage_id);
	switch (zone_id){
		case GSD_MAIN_ZONE_TYPE_1:
			gmMapFarZone1Flush();
			break;
		case GSD_MAIN_ZONE_TYPE_2:
			gmMapFarZone2Flush();
			break;
		case GSD_MAIN_ZONE_TYPE_3:
			gmMapFarZone3Flush();
			break;
		case GSD_MAIN_ZONE_TYPE_4:
			gmMapFarZone4Flush();
			break;
		case GSD_MAIN_ZONE_TYPE_FINAL:
			gmMapFarZoneFinalFlush();
			break;
		case GSD_MAIN_ZONE_TYPE_SS:
			gmMapFarZoneSSFlush();
			break;
		default:
			amAssert(0);
			break;
	}
}

// ==========================================================================
// GmMapFarRelease
/*!
 * ‰“Œi\’z‚µ‚½ƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
void GmMapFarRelease( void )
{

	//-------------------------------------------------
	//ƒ][ƒ“ê—pˆ—
	//-------------------------------------------------
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmMapFarGetZoneType(stage_id);
	switch (zone_id){
		case GSD_MAIN_ZONE_TYPE_1:
			gmMapFarZone1Release();
			break;
		case GSD_MAIN_ZONE_TYPE_2:
			gmMapFarZone2Release();
			break;
		case GSD_MAIN_ZONE_TYPE_3:
			gmMapFarZone3Release();
			break;
		case GSD_MAIN_ZONE_TYPE_4:
			gmMapFarZone4Release();
			break;
		case GSD_MAIN_ZONE_TYPE_FINAL:
			gmMapFarZoneFinalRelease();
			break;
		case GSD_MAIN_ZONE_TYPE_SS:
			gmMapFarZoneSSRelease();
			break;
		default:
			amAssert(0);
			break;
	}

	//-------------------------------------------------
	//‹¤’Êˆ—
	//-------------------------------------------------
	//ƒf[ƒ^‰ğ•ú
	gmMapFarDataRelease();

	//‰“ŒiŠÇ—‰ğ•ú
	gmMapFarReleaseMgr();
}

// ==========================================================================
//‰“ŒiŠÇ—
// ==========================================================================

// ==========================================================================
// GmMapFarGetCameraPos
/*!
 * ’ÊíƒJƒƒ‰‚ÌÀ•W‚ğ“n‚µ‚ÄA‰“ŒiƒJƒƒ‰‚ÌÀ•W‚ğæ“¾
 *
 * @param player_camera_pos ’ÊíƒJƒƒ‰‚ÌÀ•W
 *
 * @return ‰“ŒiƒJƒƒ‰‚ÌÀ•W
 */
// ==========================================================================
NNS_VECTOR GmMapFarGetCameraPos( const NNS_VECTOR* player_camera_pos )
{
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmMapFarGetZoneType(stage_id);

	switch (zone_id){
		case GSD_MAIN_ZONE_TYPE_1:
			return gmMapFarZone1GetCameraPos( player_camera_pos ); 
		case GSD_MAIN_ZONE_TYPE_2:
			return gmMapFarZone2GetCameraPos( player_camera_pos ); 
		case GSD_MAIN_ZONE_TYPE_3:
			return gmMapFarZone3GetCameraPos( player_camera_pos ); 
		case GSD_MAIN_ZONE_TYPE_4:
			return *player_camera_pos;
		case GSD_MAIN_ZONE_TYPE_FINAL:
			return gmMapFarZoneFinalGetCameraPos( player_camera_pos ); 
		case GSD_MAIN_ZONE_TYPE_SS:
			return gmMapFarZoneSSGetCameraPos( player_camera_pos ); 
		default:
			amAssert(0);
			break;
	}

	return *player_camera_pos;
}

// ==========================================================================
// GmMapFarGetCameraTarget
/*!
 * ‰“ŒiƒJƒƒ‰‚ÌÀ•W‚ğ“n‚µ‚ÄA‰“ŒiƒJƒƒ‰‚Ìƒ^[ƒQƒbƒgÀ•W‚ğæ“¾
 *
 * @param camera_pos ‰“ŒiƒJƒƒ‰‚ÌÀ•W
 *
 * @return ‰“ŒiƒJƒƒ‰‚Ìƒ^[ƒQƒbƒgÀ•W
 */
// ==========================================================================
NNS_VECTOR GmMapFarGetCameraTarget( const NNS_VECTOR* camera_pos )
{
	amAssert( camera_pos );

	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmMapFarGetZoneType(stage_id);
	
	NNS_VECTOR camera_target;
	nnAddVector( &camera_target, camera_pos, &g_gm_map_far_camera_target_offset[zone_id] );

	return camera_target;
}

//----- Local Functions -----------------------------------------------------

// ==========================================================================
//ƒQ[ƒ€İ’è
// ==========================================================================

// ==========================================================================
// gmMapFarGetStageId
/*!
 * ƒXƒe[ƒWID‚ğæ“¾
 *
 * @retun ƒXƒe[ƒWID
 */
// ==========================================================================
GSE_MAIN_STAGE_ID gmMapFarGetStageId( void )
{
	return (GSE_MAIN_STAGE_ID)g_gs_main_sys_info.stage_id;
}

// ==========================================================================
// gmMapFarGetZoneType
/*!
 * ƒ][ƒ“ƒ^ƒCƒv‚ğæ“¾
 *
 * @param stage_id ƒXƒe[ƒWID
 *
 * @return ƒ][ƒ“ƒ^ƒCƒv
 */
// ==========================================================================
GSE_MAIN_ZONE_TYPE gmMapFarGetZoneType( GSE_MAIN_STAGE_ID stage_id )
{
	amAssert( GSD_MAIN_STAGE_ID_MAX > stage_id );

	return g_gm_gamedat_zone_type_tbl[stage_id];
}

// ==========================================================================
// gmMapFarGetMapsetMpA
/*!
 * A–Ê‚ÌƒTƒCƒYî•ñ‚ğæ“¾
 *
 * @return A–Ê‚ÌƒTƒCƒYî•ñ
 */
// ==========================================================================
const MP_HEADER* gmMapFarGetMapsetMpA( void )
{
	return (const MP_HEADER*)g_gm_gamedat_map_set[GMD_GAMEDAT_MAPSET_A_MP];
}

// ==========================================================================
// gmMapFarGetObject
/*!
 * ƒIƒuƒWƒFƒNƒgƒVƒXƒeƒ€ŠÇ—ƒ[ƒN‚ğæ“¾
 *
 * @return ƒIƒuƒWƒFƒNƒgƒVƒXƒeƒ€ŠÇ—ƒ[ƒN
 */
// ==========================================================================
const OBS_OBJECT* gmMapFarGetObject( void )
{
	return &g_obj;
}

#if GMD_MAP_FAR_TEST_MPDATA
// ==========================================================================
// gmMapFarCheckDisplayList
/*!
 * ƒfƒBƒXƒvƒŒƒCƒŠƒXƒg‚Ì‹ó‚«‚ğƒ`ƒFƒbƒN
 *
 * @return TRUEF‹ó‚«‚ ‚è FALSEF‹ó‚«‚È‚µ
 */
// ==========================================================================
BOOL gmMapFarCheckDisplayList( void )
{
	if ( GsMainSysGetDisplayListRegistNum() >= AMD_REGISTLIST_NUM - 64 ){
		return FALSE;
	}
	return TRUE;
}
#endif

// ==========================================================================
// gmMapFarClearColor
/*!
 * ‰æ–Ê‚ğƒNƒŠƒA
 */
// ==========================================================================
void gmMapFarClearColor( void )
{ 
	
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmMapFarGetZoneType(stage_id);
	
	amDrawSetBGColor( &g_gm_map_far_clear_color[zone_id] );

#if (0)

#if _PC | _XBOX
	#if _PC
		LPDIRECT3DDEVICE9 d3dDev = amWinDxGetDirect3DDevice();
	#elif _XBOX
		LPDIRECT3DDEVICE9 d3dDev = amXboxDxGetDirect3DDevice();
	#endif
	
	d3dDev->Clear(
			0, 
			NULL, 
			AMD_RENDER_CLEAR_COLOR,
			D3DCOLOR_RGBA(g_gm_map_far_clear_color[zone_id].r, g_gm_map_far_clear_color[zone_id].g, g_gm_map_far_clear_color[zone_id].b, g_gm_map_far_clear_color[zone_id].a),
			1.0f,
			0);
#elif _PS3
	nnDevicePs3SetClearColor( _am_ps3_device, &g_gm_map_far_clear_color[zone_id] );
	nnDevicePs3Clear( _am_ps3_device, AMD_RENDER_CLEAR_COLOR );
#elif _WII
#else
#endif

#endif
}

#if (0)
// ==========================================================================
// gmMapFarClearZBuffer
/*!
 * Zƒoƒbƒtƒ@‚ğƒNƒŠƒA
 */
// ==========================================================================
void gmMapFarClearZBuffer( void )
{ 
#if _PC | _XBOX
	#if _PC
		LPDIRECT3DDEVICE9 d3dDev = amWinDxGetDirect3DDevice();
	#elif _XBOX
		LPDIRECT3DDEVICE9 d3dDev = amXboxDxGetDirect3DDevice();
	#endif

	d3dDev->Clear(0, NULL, AMD_RENDER_CLEAR_DEPTH | AMD_RENDER_CLEAR_STENCIL, 0, 1.0f, 0 );
#elif _PS3
	nnDevicePs3SetClearDepth( _am_ps3_device, 1.0f );
	nnDevicePs3SetClearStencil(_am_ps3_device, 0 );
	nnDevicePs3Clear( _am_ps3_device, AMD_RENDER_CLEAR_DEPTH | AMD_RENDER_CLEAR_STENCIL );
#elif _WII
#else
#endif
}
#endif


// ==========================================================================
//ƒf[ƒ^ŠÇ—
// ==========================================================================

// ==========================================================================
// gmMapFarDataInit
/*!
 * ƒf[ƒ^ŠÇ—‰Šú‰»
 */
// ==========================================================================
void gmMapFarDataInit( void )
{
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];

	amAssert(!g_map_far_data);

	amZeroMemory(&g_map_far_data_real, sizeof(GMS_MAP_FAR_DATA));
	g_map_far_data = &g_map_far_data_real;

	if ( GSD_MAIN_ZONE_TYPE_1 == zone_type )
	{
		g_map_far_data->degSky = nnRandom() * 340.0f;
		g_map_far_data->degSky2 = nnRandom() * 340.0f;
	}
	else if ( GSD_MAIN_ZONE_TYPE_FINAL == zone_type )
	{
		float check = nnRandom();

		// Œ©‰h‚¦‚Ì—Ç‚³‚»‚¤‚ÈêŠ‚ğ‘½‚ß‚É‚·‚éi‚R‰ÓŠ‚®‚ç‚¢j
		if ( check < 0.25f )
		{
			g_map_far_data->degSky = 260.0f + nnRandom() * 50.0f;
		}
		else if ( check < 0.5f )
		{
			g_map_far_data->degSky = 130.0f + nnRandom() * 50.0f;
		}
		else if ( check < 0.75f )
		{
			g_map_far_data->degSky = 20.0f + nnRandom() * 50.0f;
		}
		// ‚»‚Ì‘¼
		else
		{
			g_map_far_data->degSky = nnRandom() * 355.0f;
		}

		g_map_far_data->degSky2 = nnRandom() * 355.0f;
	}
	else
	{
		g_map_far_data->degSky = 90.0f;
		g_map_far_data->degSky2 = 90.0f;
	}

#if GMD_MAP_FAR_EDIT
	// ƒtƒHƒO
	g_map_far_data->fogCursor = 0;
	if ( GSD_MAIN_ZONE_TYPE_1 == zone_type )
	{
		if (  (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3) 
			||(g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_BOSS))
		{
			g_map_far_data->fogColor.x = 0.85f;
			g_map_far_data->fogColor.y = 0.5f;
			g_map_far_data->fogColor.z = 0.25f;
		}
		else
		{
			g_map_far_data->fogColor.x = 0.7f;
			g_map_far_data->fogColor.y = 0.95f;
			g_map_far_data->fogColor.z = 1.0f;
		}

		g_map_far_data->fogNear = 100.0f;
		g_map_far_data->fogFar = 500.0f;
	}
	else if ( GSD_MAIN_ZONE_TYPE_2 == zone_type )
	{
		g_map_far_data->fogColor.x = 0.00f;
		g_map_far_data->fogColor.y = 0.00f;
		g_map_far_data->fogColor.z = 0.30f;
		g_map_far_data->fogNear = 450.0f;
		g_map_far_data->fogFar = 650.0f;
	}
	else if ( GSD_MAIN_ZONE_TYPE_3 == zone_type )
	{
		g_map_far_data->fogColor.x = 85.0f/255.0f;
		g_map_far_data->fogColor.y = 118.0f/255.0f;
		g_map_far_data->fogColor.z = 109.0f/255.0f;
		g_map_far_data->fogNear = 300.0f;
		g_map_far_data->fogFar = 1300.0f;
	}
	else if ( GSD_MAIN_ZONE_TYPE_FINAL == zone_type )
	{
		g_map_far_data->fogColor.x = 0.10f;
		g_map_far_data->fogColor.y = 0.08f;
		g_map_far_data->fogColor.z = 0.22f;
		g_map_far_data->fogNear = 160.0f;
		g_map_far_data->fogFar = 1100.0f;
	}
	else if ( GSD_MAIN_ZONE_TYPE_SS == zone_type )
	{
		g_map_far_data->fogColor.x = 0.00f;
		g_map_far_data->fogColor.y = 0.00f;
		g_map_far_data->fogColor.z = 0.00f;
		g_map_far_data->fogNear = 100.0f;
		g_map_far_data->fogFar = 1000.0f;
	}

	// ƒ‰ƒCƒg
	g_map_far_data->lightDir.x = -1.0f;
	g_map_far_data->lightDir.y = -1.0f;
	g_map_far_data->lightDir.z = -1.0f;
#endif
}

// ==========================================================================
// gmMapFarDataRelease
/*!
 * \’z‚µ‚½ƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
void gmMapFarDataRelease( void )
{
	if ( g_map_far_data ){
		//NNƒ‚ƒfƒ‹ƒ[ƒN‰ğ•ú
		gmMapFarDataFreeNNModelWork();

		//AMB‰ğ•ú
		gmMapFarDataReleaseAmbHeader();

		g_map_far_data = NULL;
	}
}

// ==========================================================================
// gmMapFarDataGetInfo
/*!
 * ƒf[ƒ^ŠÇ—‚ğæ“¾
 *
 * @return  ƒf[ƒ^ŠÇ—
 */
// ==========================================================================
static GMS_MAP_FAR_DATA* gmMapFarDataGetInfo( void )
{
	return g_map_far_data;
}

// ==========================================================================
// gmMapFarDataSetAmbHeader
/*!
 * ‰“Œiƒf[ƒ^amb‚ğİ’è
 *
 * @param amb  ‰“Œiƒf[ƒ^amb
 */
// ==========================================================================
void gmMapFarDataSetAmbHeader( AMS_AMB_HEADER* amb )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);
	amAssert(!data->amb_header);

	data->amb_header = amb;
}

// ==========================================================================
// gmMapFarDataGetAmbHeader
/*!
 * ‰“Œiƒf[ƒ^amb‚ğæ“¾
 *
 * @return ‰“Œiƒf[ƒ^amb
 */
// ==========================================================================
AMS_AMB_HEADER* gmMapFarDataGetAmbHeader( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);

	return data->amb_header;
}

// ==========================================================================
// gmMapFarDataReleaseAmbHeader
/*!
 * ‰“Œiƒf[ƒ^amb‚ğ‰ğ•ú
 */
// ==========================================================================
void gmMapFarDataReleaseAmbHeader( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);

	if ( data->amb_header ){
		mtMemFreeMain( data->amb_header );
		data->amb_header = NULL;
	}
}

// ==========================================================================
// gmMapFarDataLoadObj
/*!
 * ƒIƒuƒWƒFƒNƒg‚ğ“Ç‚İ‚İ
 *
 * @param obj_index ƒIƒuƒWƒFƒNƒgƒCƒ“ƒfƒNƒX
 * @param model_index ƒ‚ƒfƒ‹‚Ìƒf[ƒ^ƒCƒ“ƒfƒNƒX
 * @param main_func ƒƒCƒ“ˆ—ŠÖ”
 * @param out_func •`‰æŠÖ”
 *
 * @return ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
GMS_MAP_FAR_OBJ_WORK* gmMapFarDataLoadObj( 
						  GMD_MAP_FAR_OBJ_INDEX obj_index,
						  OBS_ACTION3D_NN_WORK* obj_3d_work, 
						  GM_MAP_FAR_OBJ_FUNC main_func,
						  GM_MAP_FAR_OBJ_FUNC out_func)
{
	// ƒ^ƒXƒNì¬
	GMS_MAP_FAR_OBJ_WORK* map_far_work = (GMS_MAP_FAR_OBJ_WORK*)OBM_OBJECT_TASK_DETAIL_INIT(
		GMD_TASK_PRIO_MAPFAR, 
		GMD_TASK_GROUP_MAPFAR, 
		GMD_TASK_PAUSELEVEL_DEF,//pause_level
		GMD_TASK_PAUSELEVEL_DEF,//obj_pause_level
		sizeof(GMS_MAP_FAR_OBJ_WORK),
		"MAP FAR OBJ");
	amAssert( map_far_work );

	// ƒ[ƒNİ’è
	OBS_OBJECT_WORK* obj_work = &map_far_work->obj_work;

	//ƒ‚ƒfƒ‹“Ç‚İ‚İ
	ObjObjectCopyAction3dNNModel(
		obj_work,
		obj_3d_work,
		&map_far_work->obj_3d);

	// ƒIƒuƒWƒFƒNƒgƒ[ƒNİ’è
	obj_work->obj_type	= GMD_OBJTYPE_MAPFAR;
	obj_work->flag |= OBD_OBJECT_NOCLIP | OBD_OBJECT_NOHIT;
	obj_work->move_flag |= OBD_MOVE_NOCOL | OBD_MOVE_NOCOLOBJ;
	obj_work->user_flag = obj_index;

	// ˆ—ŠÖ”“o˜^
	obj_work->ppFunc = main_func;
	obj_work->ppOut = out_func;

	//TCBƒfƒXƒgƒ‰ƒNƒ^İ’è
	mtTaskChangeTcbDestructor( obj_work->tcb, ObjObjectExit );

	//‰“ŒiŠÇ—‚É“o˜^
	gmMapFarDataSetObjWork( obj_work, obj_index );

	return map_far_work;
}

// ==========================================================================
// gmMapFarDataClearObjWork
/*!
 * OBJƒ[ƒN‚ğ‰ğ•ú
 */
// ==========================================================================
void gmMapFarDataClearObjWork( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);

	amZeroMemory(data->obj_work, sizeof(OBS_OBJECT_WORK*)*GMD_MPA_FAR_OBJ_NUM );
}

// ==========================================================================
// gmMapFarDataSetObjWork
/*!
 * OBJƒ[ƒN‚ğİ’è
 *
 * @param obj_work OBJƒ[ƒN
 * @param obj_index OBJƒCƒ“ƒfƒNƒX
 */
// ==========================================================================
void gmMapFarDataSetObjWork( OBS_OBJECT_WORK* obj_work, GMD_MAP_FAR_OBJ_INDEX obj_index )
{
	amAssert( obj_index < GMD_MPA_FAR_OBJ_NUM );

	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);
	amAssert(!data->obj_work[obj_index]);
	data->obj_work[obj_index] = obj_work;
}

// ==========================================================================
// gmMapFarDataGetObjWork
/*!
 * OBJƒ[ƒN‚ğæ“¾
 *
 * @param obj_index OBJƒCƒ“ƒfƒNƒX
 *
 * @return OBJƒ[ƒN
 */
// ==========================================================================
OBS_OBJECT_WORK* gmMapFarDataGetObjWork( GMD_MAP_FAR_OBJ_INDEX obj_index )
{
	if ( GMD_MPA_FAR_OBJ_NUM <= obj_index ){
		return NULL;
	}
	
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);

	return data->obj_work[obj_index];
}

#if GMD_MAP_FAR_TEST_MPDATA
// ==========================================================================
// gmMapFarDataAllocNNModelWork
/*!
 * 3Dƒ‚ƒfƒ‹ƒ[ƒN—Ìˆæ‚ğŠm•Û
 *
 */
// ==========================================================================
 OBS_ACTION3D_NN_WORK* gmMapFarDataAllocNNModelWork( s32 num )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);
	amAssert( !data->nn_work );

	// 3Dƒ‚ƒfƒ‹ƒ[ƒNŠm•Û
	data->nn_work = (OBS_ACTION3D_NN_WORK*)amMemAlloc(sizeof(OBS_ACTION3D_NN_WORK) * num);
	amZeroMemory(data->nn_work , sizeof(OBS_ACTION3D_NN_WORK) * num);

	data->nn_work_num = num;

	return data->nn_work;
}

// ==========================================================================
// gmMapFarDataGetNNModelWorkList
/*!
 * NNƒ‚ƒfƒ‹ƒ[ƒNƒŠƒXƒg‚ğæ“¾
 *
 * @return 3Dƒ‚ƒfƒ‹ƒ[ƒNƒŠƒXƒg
 */
// ==========================================================================
OBS_ACTION3D_NN_WORK* gmMapFarDataGetNNModelWorkList( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);

	return data->nn_work;
}

// ==========================================================================
// gmMapFarDataLoadNNModel
/*!
 * NNƒ‚ƒfƒ‹“Ç‚İ‚İ’†
 *
 * @param model_index ƒ‚ƒfƒ‹ƒCƒ“ƒfƒNƒX
 * @param model_amb ƒ‚ƒfƒ‹amb
 * @param texture_amb ƒeƒNƒXƒ`ƒƒamb
 *
 * @return ƒ[ƒN
 */
// ==========================================================================
OBS_ACTION3D_NN_WORK* gmMapFarDataLoadNNModel( 
							 s32 model_index,
							 AMS_AMB_HEADER* model_amb,
							 AMS_AMB_HEADER* texture_amb)
{
	//ƒ[ƒNæ“¾
	OBS_ACTION3D_NN_WORK* nn_work = gmMapFarDataGetNNModelWork( model_index );
	amAssert( nn_work );

	//“Ç‚İ‚İ
	ObjAction3dNNModelLoad(
			nn_work,
			NULL,
			NULL,
			model_index,
			model_amb,
			NULL, 
			texture_amb,
			0);

	// •`‰æƒRƒ}ƒ“ƒh”­s ƒXƒe[ƒg
	nn_work->command_state = OBD_DRAW_CMD_STATE_PRE_MAPFAR;	// •W€İ’è

	//—¼–Ê•`‰æ(•\— ”½“]‘Î‰)
	nn_work->drawflag |= NND_DRAWOBJ_DOUBLESIDE;

	//“o˜^”‰ÁZ
	gmMapFarDataIncNNModelRegistNum();

	return nn_work;
}

// ==========================================================================
// gmMapFarDataIncNNModelRegistNum
/*!
 * NNƒ‚ƒfƒ‹“o˜^Ï‚İ”‰ÁZ
 *
 */
// ==========================================================================
void gmMapFarDataIncNNModelRegistNum( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);
	amAssert( data->nn_work_num > data->nn_regist_num );

	++data->nn_regist_num;
}

// ==========================================================================
// gmMapFarDataGetNNModelRegistNum
/*!
 * NNƒ‚ƒfƒ‹“o˜^Ï‚İ”‚ğæ“¾
 *
 * @return “o˜^Ï‚İ”
 *
 */
// ==========================================================================
s32 gmMapFarDataGetNNModelRegistNum( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);

	return data->nn_regist_num;
}

// ==========================================================================
// gmMapFarDataCheckNNModelLoad
/*!
 * NNƒ‚ƒfƒ‹“Ç‚İ‚İÏ‚İŠm”F
 *
 * @return TRUEF“Ç‚İ‚İÏ‚İ FALSEF“Ç‚İ‚İ‘Ò‚¿
 */
// ==========================================================================
BOOL gmMapFarDataCheckNNModelLoad( void )
{
	for ( s32 i = GMD_MAP_FAR_NNMODEL_OBJECT_START; gmMapFarDataGetNNModelWorkNum() > i; ++i ){
		OBS_ACTION3D_NN_WORK* nn_work = gmMapFarDataGetNNModelWork( i );

		if ( FALSE == ObjAction3dNNModelLoadCheck(nn_work) ){
			// ‚Ü‚¾“o˜^‚³‚ê‚Ä‚¢‚È‚¢‚à‚Ì‚ ‚è
			return FALSE;
		}
	}
	return TRUE;
}

// ==========================================================================
// gmMapFarDataGetNNModelWork
/*!
 * NNƒ‚ƒfƒ‹ƒ[ƒN‚ğæ“¾
 *
 * @param index ƒCƒ“ƒfƒNƒX
 *
 * @return 3Dƒ‚ƒfƒ‹ƒ[ƒN
 */
// ==========================================================================
OBS_ACTION3D_NN_WORK* gmMapFarDataGetNNModelWork( s32 index )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);
	amAssert( data->nn_work );

	if ( index >= data->nn_work_num ){
		return NULL;
	}
	return &data->nn_work[index];
}

// ==========================================================================
// gmMapFarDataGetNNModelWorkNum
/*!
 * NNƒ‚ƒfƒ‹ƒ[ƒN”‚ğæ“¾
 *
 * @return NNƒ‚ƒfƒ‹ƒ[ƒN”
 */
// ==========================================================================
s32 gmMapFarDataGetNNModelWorkNum( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);
	return data->nn_work_num;
}

// ==========================================================================
// gmMapFarDataSetMpHeader
/*!
 * MPƒwƒbƒ_‚ğİ’è
 *
 * @param md_header MPƒwƒbƒ_
 */
// ==========================================================================
void gmMapFarDataSetMpHeader( MP_HEADER* mp_header )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);
	
	data->mp_header = mp_header;
}

// ==========================================================================
// gmMapFarDataGetMpHeader
/*!
 * MPƒwƒbƒ_‚ğæ“¾
 *
 * @return MPƒwƒbƒ_
 */
// ==========================================================================
MP_HEADER* gmMapFarDataGetMpHeader( void )
{	
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);

	return data->mp_header;
}

// ==========================================================================
// gmMapFarDataGetMpHeader
/*!
 * MDƒwƒbƒ_‚ğİ’è
 *
 * @param md_header MDƒwƒbƒ_
 */
// ==========================================================================
void gmMapFarDataSetMdHeader( MD_HEADER* md_header )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);
	
	data->md_header = md_header;
}

// ==========================================================================
// gmMapFarDataGetMdHeader
/*!
 * MDƒwƒbƒ_‚ğæ“¾
 *
 * @return MDƒwƒbƒ_
 */
// ==========================================================================
MD_HEADER* gmMapFarDataGetMdHeader( void )
{	
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);

	return data->md_header;
}
#endif

// ==========================================================================
// gmMapFarDataFreeNNModelWork
/*!
 * 3Dƒ‚ƒfƒ‹ƒ[ƒN—Ìˆæ‚ğ‰ğ•ú
 *
 */
// ==========================================================================
void gmMapFarDataFreeNNModelWork( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert(data);

	if ( data->nn_work ){
		amMemFree(data->nn_work);
		data->nn_work = NULL;
	}
	data->nn_work_num = 0;
	data->nn_regist_num = 0;
	data->mp_header = NULL;
	data->md_header = NULL;
}

// ==========================================================================
//‰“ŒiŠÇ—
// ==========================================================================

// ==========================================================================
// GmMapFarGetMgr
/*!
 * ‰“ŒiŠÇ—æ“¾
 *
 * @retun ‰“ŒiŠÇ—
 */
// ==========================================================================
GMS_MAP_FAR_MGR* gmMapFarGetMgr( void )
{
	return g_map_far_mgr;
}

// ==========================================================================
// gmMapFarInitMgr
/*!
 * ‰“ŒiŠÇ—‰Šú‰»
 */
// ==========================================================================
void gmMapFarInitMgr( void )
{
	amAssert(!g_map_far_mgr);

	amZeroMemory(&g_map_far_mgr_real, sizeof(GMS_MAP_FAR_MGR));
	g_map_far_mgr = &g_map_far_mgr_real;
}

// ==========================================================================
// gmMapFarExitMgr
/*!
 * ‰“ŒiŠÇ—I—¹
 */
// ==========================================================================
void gmMapFarExitMgr( void )
{
	if ( g_map_far_mgr ){
		//•`‰æ‘Oˆ—TCBíœ
		gmMapFarDeleteTcbPreDraw();
		
		//•`‰æTCBíœ
		gmMapFarDeleteTcbDraw();
	
		//•`‰æŒãˆ—TCBíœ
		gmMapFarDeleteTcbPostDraw();

		g_map_far_mgr = NULL;
	}
}

// ==========================================================================
// gmMapFarReleaseMgr
/*!
 * ‰“ŒiŠÇ—‰ğ•ú
 */
// ==========================================================================
void gmMapFarReleaseMgr( void )
{
}

// ==========================================================================
// gmMapFarCreateTcbPreDraw
/*!
 * •`‰æ‘Oˆ—TCB‚ğì¬
 *
 * @return •`‰æ‘Oˆ—TCB
 */
// ==========================================================================
MTS_TASK_TCB* gmMapFarCreateTcbPreDraw( void )
{
	GMS_MAP_FAR_MGR* mgr = gmMapFarGetMgr();
	amAssert(mgr);
	
	//Šù‚Éì¬Ï‚İ
	amAssert( !mgr->tcb_pre_draw );

	//ì¬
	mgr->tcb_pre_draw = MTM_TASK_MAKE_TCB(
		gmMapFarTcbProcPreDraw,
		NULL,
		0,							//flag
		GMD_TASK_PAUSELEVEL_DEF,	//pause_level
		GMD_TASK_PRIO_MAPFAR,
		GMD_TASK_GROUP_MAPFAR,
		0,							//work_size
		"GM MAP FAR PRE DRAW");
	amAssert( mgr->tcb_pre_draw );

	return mgr->tcb_pre_draw;
}

// ==========================================================================
// gmMapFarDeleteTcbPreDraw
/*!
 * •`‰æ‘Oˆ—TCB‚ğíœ
 *
 */
// ==========================================================================
void gmMapFarDeleteTcbPreDraw( void )
{
	GMS_MAP_FAR_MGR* mgr = gmMapFarGetMgr();
	amAssert(mgr);
	if ( mgr->tcb_pre_draw ){
		mtTaskClearTcb( mgr->tcb_pre_draw );
		mgr->tcb_pre_draw = NULL;
	}
}

// ==========================================================================
// gmMapFarTcbProcPreDraw
/*!
 * •`‰æ‘Oˆ—ƒvƒƒV[ƒWƒƒ
 *
 */
// ==========================================================================
void gmMapFarTcbProcPreDraw( MTS_TASK_TCB *tcb )
{
	UNREFERENCED_PARAMETER( tcb );
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];

	//•`‰æ‘Oˆ—‚ğ“o˜^
	ObjDraw3DNNUserFunc(gmMapFarTcbProcPreDrawDT, NULL, 0, OBD_DRAW_CMD_STATE_PRE_MAPFAR);

	//ƒJƒƒ‰İ’è
	gmMapFarCameraApply();

#if GMD_MAP_FAR_EDIT
	Sint32 dx = 20;
	Sint32 dy = 3;
	float speed = 1.0f;

	// ‰EƒVƒ‡ƒ‹ƒ_[‚Å‰Á‘¬‚·‚é
	if (PAD_DIRECT(0) & KEY_R1 )
	{
        speed = 5.0f;
	}

	// ƒJ[ƒ\ƒ‹ã‰º
	if (PAD_REPEAT(0) & KEY_L_UP) 
	{
		if ( g_map_far_data->fogCursor != 0 ) g_map_far_data->fogCursor--;
	}
	else if (PAD_REPEAT(0) & KEY_L_DOWN )
	{
		if ( g_map_far_data->fogCursor != 7) g_map_far_data->fogCursor++;
	}

	// ƒJ[ƒ\ƒ‹¶‰E
	if (PAD_REPEAT(0) & KEY_L_RIGHT) 
	{
		switch ( g_map_far_data->fogCursor )
		{
		case 0: g_map_far_data->fogColor.x += (0.01f*speed);break;
		case 1: g_map_far_data->fogColor.y += (0.01f*speed);break;
		case 2: g_map_far_data->fogColor.z += (0.01f*speed);break;
		case 3: g_map_far_data->fogNear += speed;break;
		case 4: g_map_far_data->fogFar += speed;break;
		case 5: g_map_far_data->lightDir.x += (0.01f*speed);break;
		case 6: g_map_far_data->lightDir.y += (0.01f*speed);break;
		case 7: g_map_far_data->lightDir.z += (0.01f*speed);break;
		}
	}
	else if (PAD_REPEAT(0) & KEY_L_LEFT )
	{
		switch ( g_map_far_data->fogCursor )
		{
		case 0: g_map_far_data->fogColor.x -= (0.01f*speed);break;
		case 1: g_map_far_data->fogColor.y -= (0.01f*speed);break;
		case 2: g_map_far_data->fogColor.z -= (0.01f*speed);break;
		case 3: g_map_far_data->fogNear -= speed;break;
		case 4: g_map_far_data->fogFar -= speed;break;
		case 5: g_map_far_data->lightDir.x -= (0.01f*speed);break;
		case 6: g_map_far_data->lightDir.y -= (0.01f*speed);break;
		case 7: g_map_far_data->lightDir.z -= (0.01f*speed);break;
		}
	}

	// ƒtƒHƒOƒJƒ‰[§ŒÀ
	if ( g_map_far_data->fogColor.x > 1.0f) g_map_far_data->fogColor.x = 1.0f;
	if ( g_map_far_data->fogColor.y > 1.0f) g_map_far_data->fogColor.y = 1.0f;
	if ( g_map_far_data->fogColor.z > 1.0f) g_map_far_data->fogColor.z = 1.0f;
	if ( g_map_far_data->fogColor.x < 0.0f) g_map_far_data->fogColor.x = 0.0f;
	if ( g_map_far_data->fogColor.y < 0.0f) g_map_far_data->fogColor.y = 0.0f;
	if ( g_map_far_data->fogColor.z < 0.0f) g_map_far_data->fogColor.z = 0.0f;

	if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_DEBUG_DISP) {
		// ƒtƒHƒOƒJƒ‰[
		amPrint( dx++, dy + g_map_far_data->fogCursor, ">" );
		amPrintf(dx, dy++, "FogColorR = %2.2f", g_map_far_data->fogColor.x);
		amPrintf(dx, dy++, "FogColorG = %2.2f", g_map_far_data->fogColor.y);
		amPrintf(dx, dy++, "FogColorB = %2.2f", g_map_far_data->fogColor.z);

		// ƒtƒHƒOƒŒƒ“ƒW
		amPrintf(dx, dy++, "FogNear   = %3.1f", g_map_far_data->fogNear);
		amPrintf(dx, dy++, "FogFar    = %3.1f", g_map_far_data->fogFar);

		// ƒ‰ƒCƒg
		amPrintf(dx, dy++, "dir_x     = %3.2f", g_map_far_data->lightDir.x);
		amPrintf(dx, dy++, "dir_y     = %3.2f", g_map_far_data->lightDir.y);
		amPrintf(dx, dy++, "dir_z     = %3.2f", g_map_far_data->lightDir.z);
	}

	if ( GSD_MAIN_ZONE_TYPE_1 == zone_type )
	{
		amDrawSetFog( OBD_DRAW_CMD_STATE_PRE_MAPFAR, TRUE );

		if (  (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3) 
			||(g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_BOSS))
		{
			amDrawSetFogColor( OBD_DRAW_CMD_STATE_PRE_MAPFAR, g_map_far_data->fogColor.x, g_map_far_data->fogColor.y, g_map_far_data->fogColor.z );
		}
		else
		{
		    amDrawSetFogColor( OBD_DRAW_CMD_STATE_PRE_MAPFAR, g_map_far_data->fogColor.x, g_map_far_data->fogColor.y, g_map_far_data->fogColor.z );
		}
		amDrawSetFogRange( OBD_DRAW_CMD_STATE_PRE_MAPFAR, g_map_far_data->fogNear, g_map_far_data->fogFar );
	}
	else if ( GSD_MAIN_ZONE_TYPE_2 == zone_type )
	{
		amDrawSetFog( OBD_DRAW_CMD_STATE_PRE_MAPFAR, TRUE );
		amDrawSetFogColor( OBD_DRAW_CMD_STATE_PRE_MAPFAR, g_map_far_data->fogColor.x, g_map_far_data->fogColor.y, g_map_far_data->fogColor.z );
		amDrawSetFogRange( OBD_DRAW_CMD_STATE_PRE_MAPFAR, g_map_far_data->fogNear, g_map_far_data->fogFar );
	}
	else if ( GSD_MAIN_ZONE_TYPE_3 == zone_type || GSD_MAIN_ZONE_TYPE_FINAL == zone_type)
	{
		amDrawSetFog( OBD_DRAW_CMD_STATE_PRE_MAPFAR, TRUE );
		amDrawSetFogColor( OBD_DRAW_CMD_STATE_PRE_MAPFAR, g_map_far_data->fogColor.x, g_map_far_data->fogColor.y, g_map_far_data->fogColor.z );
		amDrawSetFogRange( OBD_DRAW_CMD_STATE_PRE_MAPFAR, g_map_far_data->fogNear, g_map_far_data->fogFar );
	}

	// ‰EƒAƒiƒƒOƒL[‚Åƒ‰ƒCƒgŒü‚«•ÏX
	if (abs(PAD_A_RX(0)) > 0x2000) {
		NNS_MATRIX	mtx;
		nnMakeRotateYMatrix(&mtx, PAD_A_RX(0) >> 4);
		nnTransformNormalVector(&g_map_far_data->lightDir, &mtx, &g_map_far_data->lightDir);
	}

	// ƒ‰ƒCƒgİ’è
//	nnNormalizeVector(&g_map_far_data->lightDir, &g_map_far_data->lightDir);
#if GMD_MAP_FAR_LIGHT_EDIT
	NNS_RGBA	light_col = {
			1.0f, 1.0f, 1.0f, 1.0f,
		};
	ObjDrawSetParallelLight(NNE_LIGHT_6, &light_col, 1.f, &g_map_far_data->lightDir);
#endif
#else

	if ( GSD_MAIN_ZONE_TYPE_1 == zone_type )
	{
		amDrawSetFog( OBD_DRAW_CMD_STATE_PRE_MAPFAR, TRUE );

		if (  (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3) 
			||(g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_BOSS))
		{
			amDrawSetFogColor( OBD_DRAW_CMD_STATE_PRE_MAPFAR, 0.85f, 0.5f, 0.25f );
#if _IPHONE
			if ( g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_BOSS )
			{
				amDrawSetFog( OBD_DRAW_CMD_STATE_PRE_MAPFAR, FALSE );
			}
#endif
		}
		else
		{
		    amDrawSetFogColor( OBD_DRAW_CMD_STATE_PRE_MAPFAR, 0.7f, 0.95f, 1.0f );
		}
		amDrawSetFogRange( OBD_DRAW_CMD_STATE_PRE_MAPFAR, 100.0f, 500.0f );
	}
	else if ( GSD_MAIN_ZONE_TYPE_2 == zone_type )
	{
#if _IPHONE
		amDrawSetFog( OBD_DRAW_CMD_STATE_PRE_MAPFAR, FALSE );
#else
		amDrawSetFog( OBD_DRAW_CMD_STATE_PRE_MAPFAR, TRUE );
#endif
		amDrawSetFogColor( OBD_DRAW_CMD_STATE_PRE_MAPFAR, 0.0f, 0.0f, 0.30f );
		amDrawSetFogRange( OBD_DRAW_CMD_STATE_PRE_MAPFAR, 450.0f, 650.0f );
	}
	else if ( GSD_MAIN_ZONE_TYPE_3 == zone_type )
	{
#if _IPHONE
		amDrawSetFog( OBD_DRAW_CMD_STATE_PRE_MAPFAR, FALSE );
#else
		amDrawSetFog( OBD_DRAW_CMD_STATE_PRE_MAPFAR, TRUE );
#endif
		amDrawSetFogColor( OBD_DRAW_CMD_STATE_PRE_MAPFAR, 85.0f/255.0f, 118.0f/255.0f, 109.0f/255.0f );
		amDrawSetFogRange( OBD_DRAW_CMD_STATE_PRE_MAPFAR, 300.0f, 1300.0f );
	}
	else if ( GSD_MAIN_ZONE_TYPE_FINAL == zone_type )
	{
#if _IPHONE
		amDrawSetFog( OBD_DRAW_CMD_STATE_PRE_MAPFAR, FALSE );
#else
		amDrawSetFog( OBD_DRAW_CMD_STATE_PRE_MAPFAR, TRUE );
#endif
		amDrawSetFogColor( OBD_DRAW_CMD_STATE_PRE_MAPFAR, 0.10f, 0.08f, 0.22f );
		amDrawSetFogRange( OBD_DRAW_CMD_STATE_PRE_MAPFAR, 160.0f, 1100.0f );
	}
	else if ( GSD_MAIN_ZONE_TYPE_SS == zone_type )
	{
		amDrawSetFog( OBD_DRAW_CMD_STATE_PRE_MAPFAR, TRUE );
		amDrawSetFogColor( OBD_DRAW_CMD_STATE_PRE_MAPFAR, 0.0f, 0.0f, 0.0f );
		amDrawSetFogRange( OBD_DRAW_CMD_STATE_PRE_MAPFAR, 100.0f, 1000.0f );
	}
#endif

	amDrawSetFog( OBD_DRAW_CMD_STATE_3DNN, FALSE );
	amDrawSetFog( OBD_DRAW_CMD_STATE_POST_MAPFAR, FALSE );
}

// ==========================================================================
// gmMapFarCreateTcbDraw
/*!
 * •`‰æTCBì¬
 *
 * @parama proc TCBƒvƒƒV[ƒWƒƒ
 *
 * @return •`‰æTCB
 */
// ==========================================================================
MTS_TASK_TCB* gmMapFarCreateTcbDraw( void )
{
	// ’nŒ`•`‰æ
	GMS_MAP_FAR_MGR* mgr = gmMapFarGetMgr();
	amAssert(mgr);
	
	//Šù‚Éì¬Ï‚İ
	amAssert( !mgr->tcb_draw );

	//ì¬
	mgr->tcb_draw = MTM_TASK_MAKE_TCB(
			NULL,
			NULL,
			0, 
			GMD_TASK_PAUSELEVEL_DEF, 
			GMD_TASK_PRIO_MAPFAR,
			GMD_TASK_GROUP_MAPFAR,
			0, 
			"GM_MAP_FAR_DRAW");
	amAssert( mgr->tcb_draw );

	return mgr->tcb_draw;
}

// ==========================================================================
// gmMapFarDeleteTcbDraw
/*!
 * •`‰æTCB‚ğíœ
 *
 */
// ==========================================================================
void gmMapFarDeleteTcbDraw( void )
{
	GMS_MAP_FAR_MGR* mgr = gmMapFarGetMgr();
	amAssert(mgr);
	if ( mgr->tcb_draw ){
		mtTaskClearTcb( mgr->tcb_draw );
		mgr->tcb_draw = NULL;
	}
}

// ==========================================================================
// gmMapFarChangeTcbProcDraw
/*!
 * •`‰æƒvƒƒV[ƒWƒƒ•ÏX
 *
 */
// ==========================================================================
void gmMapFarChangeTcbProcDraw( const GSF_TASK_PROCEDURE proc )
{
	GMS_MAP_FAR_MGR* mgr = gmMapFarGetMgr();
	amAssert(mgr);
	amAssert( mgr->tcb_draw );
	mtTaskChangeTcbProcedure( mgr->tcb_draw, proc );
}

// ==========================================================================
// gmMapFarTcbProcPreDrawDT
/*!
 * •`‰æŒãˆ—ƒ†[ƒUŠÖ”
 *
 */
// ==========================================================================
void gmMapFarTcbProcPreDrawDT(void *data)
{
	UNREFERENCED_PARAMETER( data );

	//‰æ–Ê‚ğƒNƒŠƒA
	gmMapFarClearColor();
}

// ==========================================================================
// gmMapFarCreateTcbPostDraw
/*!
 * •`‰æŒãˆ—TCB‚ğì¬
 *
 * @return •`‰æŒãˆ—TCB
 */
// ==========================================================================
MTS_TASK_TCB* gmMapFarCreateTcbPostDraw( void )
{
	GMS_MAP_FAR_MGR* mgr = gmMapFarGetMgr();
	amAssert(mgr);
	
	//Šù‚Éì¬Ï‚İ
	amAssert( !mgr->tcb_post_draw );

	//ì¬
	mgr->tcb_post_draw = MTM_TASK_MAKE_TCB(
		gmMapFarTcbProcPostDraw,
		NULL,
		0,							//flag
		GMD_TASK_PAUSELEVEL_DEF,	//pause_level
		GMD_TASK_PRIO_MAPFAR,
		GMD_TASK_GROUP_MAPFAR,
		0,							//work_size
		"GM MAP FAR POST DRAW");
	amAssert( mgr->tcb_post_draw );

	return mgr->tcb_post_draw;
}

// ==========================================================================
// gmMapFarDeleteTcbPostDraw
/*!
 * •`‰æŒãˆ—TCB‚ğíœ
 *
 */
// ==========================================================================
void gmMapFarDeleteTcbPostDraw( void )
{
	GMS_MAP_FAR_MGR* mgr = gmMapFarGetMgr();
	amAssert(mgr);
	if ( mgr->tcb_post_draw ){
		mtTaskClearTcb( mgr->tcb_post_draw );
		mgr->tcb_post_draw = NULL;
	}
}

// ==========================================================================
// gmMapFarTcbProcPostDraw
/*!
 * •`‰æŒãˆ—ƒvƒƒV[ƒWƒƒ
 *
 */
// ==========================================================================
void gmMapFarTcbProcPostDraw( MTS_TASK_TCB *tcb )
{
	UNREFERENCED_PARAMETER( tcb );

	//•`‰æŒãˆ—‚ğ“o˜^
	ObjDraw3DNNUserFunc(gmMapFarTcbProcPostDrawDT, NULL, 0, OBD_DRAW_CMD_STATE_POST_MAPFAR);

	//ƒJƒƒ‰İ’è‚ğ–ß‚·i•`‰æÀs‚ÍAƒJƒƒ‰‚Éİ’è‚³‚ê‚Ä‚¢‚é•`‰æƒRƒ}ƒ“ƒh‚ÅÀs‚³‚ê‚éBj
	const OBS_OBJECT* object = gmMapFarGetObject();
	ObjDraw3DNNSetCamera(object->glb_camera_id, object->glb_camera_type );
}

// ==========================================================================
// gmMapFarTcbProcPostDrawDT
/*!
 * •`‰æŒãˆ—ƒ†[ƒUŠÖ”
 *
 */
// ==========================================================================
void gmMapFarTcbProcPostDrawDT(void *data)
{
	UNREFERENCED_PARAMETER( data );

	//Zƒoƒbƒtƒ@ƒNƒŠƒA
//	gmMapFarClearZBuffer();
}

// ==========================================================================
//ƒJƒƒ‰
// ==========================================================================


// ==========================================================================
// gmMapFarCameraGetInfo
/*!
 * ƒJƒƒ‰î•ñ‚ğæ“¾
 *
 * @param camear_id ƒJƒƒ‰ID
 * @param camera_type ƒJƒƒ‰ƒ^ƒCƒv
 */
// ==========================================================================
GMS_MAP_FAR_CAMERA* gmMapFarCameraGetInfo( void )
{
	GMS_MAP_FAR_MGR* mgr = gmMapFarGetMgr();
	amAssert(mgr);
	return &mgr->camera;
}

// ==========================================================================
// gmMapFarCameraSetInfo
/*!
 * ƒJƒƒ‰î•ñ‚ğİ’è
 *
 * @param camear_id ƒJƒƒ‰ID
 * @param camera_type ƒJƒƒ‰ƒ^ƒCƒv
 */
// ==========================================================================
void gmMapFarCameraSetInfo( s32 camear_id, NNE_PROJECTION_TYPE camera_type )
{
	GMS_MAP_FAR_CAMERA* camera_info = gmMapFarCameraGetInfo();
	amAssert(camera_info);
	camera_info->camera_id = camear_id;
	camera_info->camera_type = camera_type;
}

// ==========================================================================
// gmMapFarCameraApply
/*!
 * ƒJƒƒ‰î•ñ‚ğ”½‰f
 */
// ==========================================================================
void gmMapFarCameraApply( void )
{
	//ƒJƒƒ‰‚ğİ’è
	GMS_MAP_FAR_CAMERA* camera_info = gmMapFarCameraGetInfo();
	amAssert(camera_info);
	ObjDraw3DNNSetCamera(camera_info->camera_id, camera_info->camera_type);
}

// ==========================================================================
// gmMapFarCameraSetSpeed
/*!
 * ƒJƒƒ‰ƒXƒNƒ[ƒ‹•â³’l‚ğİ’è
 *
 * @param speed_x X•â³’l
 * @param speed_y Y•â³’l
 */
// ==========================================================================
void gmMapFarCameraSetSpeed( float speed_x, float speed_y )
{
	GMS_MAP_FAR_CAMERA* camera = gmMapFarCameraGetInfo();
	amAssert(camera);
	
	camera->camera_speed_x = speed_x;
	camera->camera_speed_y = speed_y;
}

// ==========================================================================
// gmMapFarCameraGetSpeedX
/*!
 * ƒJƒƒ‰ƒXƒNƒ[ƒ‹•â³’l‚ğæ“¾X
 *
 * @param ƒJƒƒ‰ƒXƒNƒ[ƒ‹•â³’l
 */
// ==========================================================================
float gmMapFarCameraGetSpeedX( void )
{
	GMS_MAP_FAR_CAMERA* camera = gmMapFarCameraGetInfo();
	amAssert(camera);
	
	return camera->camera_speed_x;
}

// ==========================================================================
// gmMapFarCameraGetSpeedY
/*!
 * ƒJƒƒ‰ƒXƒNƒ[ƒ‹•â³’l‚ğæ“¾Y
 *
 * @param ƒJƒƒ‰ƒXƒNƒ[ƒ‹•â³’l
 */
// ==========================================================================
float gmMapFarCameraGetSpeedY( void )
{
	GMS_MAP_FAR_CAMERA* camera = gmMapFarCameraGetInfo();
	amAssert(camera);
	
	return camera->camera_speed_y;
}

// ==========================================================================
// gmMapFarCameraGetScrollDistance
/*!
 * ‘SƒXƒNƒ[ƒ‹‚ğs‚Á‚½ê‡‚Ì‹——£‚ğæ“¾
 *
 * @param scroll_list ƒXƒNƒ[ƒ‹î•ñ
 * @param scroll_info_num ƒXƒNƒ[ƒ‹î•ñ‚Ì”
 *
 * @return ‘SƒXƒNƒ[ƒ‹‚ğs‚Á‚½ê‡‚Ì‹——£
 */
// ==========================================================================
s32 gmMapFarCameraGetScrollDistance(
						   const GMS_MAP_FAR_SCROLL* scroll_list,
						   u32 scroll_info_num)
{
	if ( !scroll_list ){
		return 0;
	}

	s32 distance = 0;
	for ( u32 i = 0; scroll_info_num > i; ++i ){
		const GMS_MAP_FAR_SCROLL* current = &scroll_list[i];
		distance += current->width * current->loop_num;
	}
	return distance;
}

// ==========================================================================
// gmMapFarCameraGetPos
/*!
 * ƒvƒŒƒCƒ„ƒJƒƒ‰‚ÌÀ•W‚ğ“n‚µ‚ÄA‰“ŒiƒJƒƒ‰‚ÌÀ•W‚ğæ“¾
 *
 * ƒXƒNƒ[ƒ‹î•ñ‚ª‚È‚¢ê‡AƒXƒNƒ[ƒ‹ƒXƒs[ƒh‚Ì‚İ”½‰f
 * ƒXƒNƒ[ƒ‹‚ğ‘S•””²‚¯‚Ä‚µ‚Ü‚Á‚½ê‡AÅŒã‚Éİ’è‚³‚ê‚Ä‚¢‚éˆÊ’u‚©‚çæ‚Öi‚ñ‚Å‚¢‚­
 *
 * @param player_camera_pos ƒvƒŒƒCƒ„ƒJƒƒ‰‚ÌÀ•W
 * @param scroll_list ƒXƒNƒ[ƒ‹î•ñ
 * @param scroll_info_num ƒXƒNƒ[ƒ‹î•ñ‚Ì”
 * @param scroll_speed ƒXƒNƒ[ƒ‹ƒXƒs[ƒh—¦iƒvƒŒƒCƒ„ƒJƒƒ‰‚ÌÀ•W‚ÉæZj
 *
 * @return ‰“ŒiƒJƒƒ‰‚ÌÀ•W
 */
// ==========================================================================
float gmMapFarCameraGetPos( 
						   float player_camera_pos,
						   const GMS_MAP_FAR_SCROLL* scroll_list,
						   u32 scroll_info_num,
						   float scroll_speed )
{

	//‰“ŒiƒJƒƒ‰‚ÌˆÚ“®‹——£
	float distance = scroll_speed * player_camera_pos;	
	float far_camera_pos = distance;

	//ƒXƒNƒ[ƒ‹î•ñ‚ª‚È‚¢ê‡‚ÍƒvƒŒƒCƒ„ƒJƒƒ‰‚ÌˆÊ’u
	if ( !scroll_list ){
		return distance;
	}

	//İ’è‚³‚ê‚Ä‚¢‚éƒXƒNƒ[ƒ‹‚ğƒ`ƒFƒbƒN
	for ( u32 i = 0; scroll_info_num > i; ++i ){
		const GMS_MAP_FAR_SCROLL* current = &scroll_list[i];

		//ƒ‹[ƒv‰ñ”‚ª0‚È‚ç”ò‚Î‚·
		if ( 0 == current->loop_num ){
			continue;
		}
		amAssert( 0 != current->width );

		//‘Sƒ‹[ƒv‚µ‚½ê‡‚Ì‹——£ˆÈã‚Ìê‡AƒGƒŠƒA•ª‚Ì‹——£‚ğˆø‚¢‚ÄŸ‚Ö
		s32 area_distance = current->width * current->loop_num;
		if ( (float)area_distance <= distance ){
			distance -= (float)area_distance;

			//ƒXƒNƒ[ƒ‹‚ğ‘S•””²‚¯‚Ä‚µ‚Ü‚Á‚½ê‡‚ÍI’[‚Å~‚Ü‚é
			far_camera_pos = (float)(current->pos + current->width);
			continue;
		}

		//ƒGƒŠƒA“à‚È‚çƒ‹[ƒv•ª‚ÌˆÚ“®‹——£‚ğˆø‚¢‚ÄA’l‚ğİ’è
		s32 loop_num = (s32)distance/current->width;
		distance -= (float)(loop_num*current->width);

		far_camera_pos = (float)current->pos + distance;
		break;
	}
	
	return far_camera_pos;
}

// ==========================================================================
// gmMapFarCameraGetPos
/*!
 * ƒvƒŒƒCƒ„ƒJƒƒ‰‚ÌÀ•W‚ğ“n‚µ‚ÄA‰“ŒiƒJƒƒ‰‚ÌÀ•W‚ğæ“¾
 *
 * ƒXƒNƒ[ƒ‹î•ñ‚ª‚È‚¢ê‡AƒXƒNƒ[ƒ‹ƒXƒs[ƒh‚Ì‚İ”½‰f
 * ƒXƒNƒ[ƒ‹‚ğ‘S•””²‚¯‚Ä‚µ‚Ü‚Á‚½ê‡AÅŒã‚Éİ’è‚³‚ê‚Ä‚¢‚éˆÊ’u‚©‚çæ‚Öi‚ñ‚Å‚¢‚­
 *
 * @param player_camera_pos ƒvƒŒƒCƒ„ƒJƒƒ‰‚ÌÀ•W
 * @param scroll_list ƒXƒNƒ[ƒ‹î•ñ
 * @param scroll_info_num ƒXƒNƒ[ƒ‹î•ñ‚Ì”
 * @param scroll_speed ƒXƒNƒ[ƒ‹ƒXƒs[ƒh—¦iƒvƒŒƒCƒ„ƒJƒƒ‰‚ÌÀ•W‚ÉæZj
 *
 * @return ‰“ŒiƒJƒƒ‰‚ÌÀ•W
 */
// ==========================================================================
NNS_VECTOR gmMapFarCameraGetPos( 
						   const NNS_VECTOR* player_camera_pos,
						   const GMS_MAP_FAR_SCROLL* scroll_list_x,
						   u32 scroll_info_num_x,
						   const GMS_MAP_FAR_SCROLL* scroll_list_y,
						   u32 scroll_info_num_y )
{
	amAssert(player_camera_pos);

	NNS_VECTOR map_far_camera_pos;

	// ----------------------------------------------------------------------
	//	X²
	// ----------------------------------------------------------------------
	//ƒvƒŒƒCƒ„ƒJƒƒ‰‚ÌˆÚ“®—Ê
	float player_camera_x = player_camera_pos->x;

	//ƒJƒƒ‰ˆÊ’uæ“¾
	map_far_camera_pos.x = gmMapFarCameraGetPos( 
			player_camera_x, 
			scroll_list_x,
			scroll_info_num_x,
			gmMapFarCameraGetSpeedX());

	// ----------------------------------------------------------------------
	//	Y²
	// ----------------------------------------------------------------------
	//ƒvƒŒƒCƒ„ƒJƒƒ‰‚ÌˆÚ“®—Ê
	float player_camera_y = player_camera_pos->y;

	//ƒJƒƒ‰ˆÊ’uæ“¾
	map_far_camera_pos.y = gmMapFarCameraGetPos( 
			player_camera_y, 
			scroll_list_y,
			scroll_info_num_y,
			gmMapFarCameraGetSpeedY());

	// ----------------------------------------------------------------------
	//	Z²
	// ----------------------------------------------------------------------
	map_far_camera_pos.z = GMD_MAP_FAR_ZONE_1_CAMERA_POS_Z;

	return map_far_camera_pos;
}

// ==========================================================================
//ƒV[ƒ“ƒf[ƒ^ŠÇ—
// ==========================================================================

// ==========================================================================
// gmMapFarSceneLoadObj
/*!

 * ƒV[ƒ“‰“Œi‚ğ“Ç‚İ‚İ
 *
 * @param obj_index ƒIƒuƒWƒFƒNƒgƒCƒ“ƒfƒNƒX
 * @param obj_3d_work ƒ‚ƒfƒ‹ƒf[ƒ^‚ÌƒRƒs[Œ³
 * @param mat_motion_index ƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“‚Ìƒf[ƒ^ƒCƒ“ƒfƒNƒXID
 * @param mat_amb_header ƒ}ƒeƒŠƒAƒ‹ƒ‚[ƒVƒ‡ƒ“‚Ìambƒwƒbƒ_
 * @param main_func ƒƒCƒ“ˆ—ŠÖ”
 * @param out_func •`‰æŠÖ”
 * @param command_state •`‰æƒRƒ}ƒ“ƒh
 */
// ==========================================================================
void gmMapFarSceneLoadObj( 
						  GMD_MAP_FAR_OBJ_INDEX obj_index,
						  OBS_ACTION3D_NN_WORK* obj_3d_work,
						  u32 mat_motion_index,
						  AMS_AMB_HEADER* mat_amb_header,
						  u32 motion_index,
						  AMS_AMB_HEADER* mtn_amb_header,
						  GM_MAP_FAR_OBJ_FUNC main_func,
						  GM_MAP_FAR_OBJ_FUNC out_func,
						  u32 command_state)
{
	//ƒ‚ƒfƒ‹“Ç‚İ‚İ
	GMS_MAP_FAR_OBJ_WORK* map_far_work = gmMapFarDataLoadObj(
			obj_index,
			obj_3d_work,
			main_func,
			out_func );
	map_far_work->obj_3d.command_state = command_state;

	//ƒ‚[ƒVƒ‡ƒ“
	if ( mat_amb_header ){
		ObjAction3dNNMaterialMotionLoad( 
				&map_far_work->obj_3d,
				0,				//reg_file_id
				NULL,			//data_work
				NULL,			//mtn_data_path
				(s32)mat_motion_index,
				mat_amb_header
										);
#if _IPHONE
		// MatMotion‚ª‚¨‚©‚µ‚¢‚Ì‚Å‹­§“I‚È‘Î‰
		// 0 - 4800‚Ìƒ‚[ƒVƒ‡ƒ“‚ğ 2400 - 7200‚ÖA
		// ŠJnƒtƒŒ[ƒ€‚ğƒvƒƒOƒ‰ƒ€“I‚É‚Ë‚¶‹È‚°‚é
		if (gmMapFarGetZoneType(gmMapFarGetStageId()) == GSD_MAIN_ZONE_TYPE_SS) {
			AMS_MOTION *motion = map_far_work->obj_3d.motion;
			motion->mmtn[motion->mmotion_id]->StartFrame = 2400.0f;
		}
#endif // _IPHONE
	}
	if ( mtn_amb_header )
	{
		ObjAction3dNNMotionLoad( 
				&map_far_work->obj_3d,
				0,				//reg_file_id
				FALSE,			//marge
				NULL,			//data_work
				NULL,			//mtn_data_path
				(s32)motion_index,
				mtn_amb_header,
				8, // motion_Num
				8  // matmotion_num
		);
	}
}

#if GMD_MAP_FAR_2DBG
// ==========================================================================
// gmMapFarSceneObjFuncDraw2DBG
/*!
 * ”Âƒ|ƒŠ‚Ì‰“Œi‚ğ•`‰æ
 *
 *	@param	work	[in]	ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
static void gmMapFarSceneObjFuncDraw2DBG( OBS_OBJECT_WORK* pWork )
{
	OBS_ACTION3D_NN_WORK* obj_3d = pWork->obj_3d;
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();

	if ( stage_id <= GSD_MAIN_STAGE_ID_1_3 )
	{
		return;
	}
#if _IPHONE
	//‰“Œi”ñ•\¦”ÍˆÍ
	else if (stage_id >= GSD_MAIN_STAGE_ID_2_1 && stage_id <= GSD_MAIN_STAGE_ID_2_BOSS) {
		if (gmMapFarDrawCheckYakei() == 0) {
			return;
		}
	}
	else if (stage_id == GSD_MAIN_STAGE_ID_3_1) {
		if ( g_map_far_data->pos.z >= -33.33f && g_map_far_data->pos.z <= -26.37f ) {return;}
		else if ( g_map_far_data->pos.z >= -97.64f && g_map_far_data->pos.z <= -88.98f ) {return;}
		else if ( g_map_far_data->pos.z >= -283.79f && g_map_far_data->pos.z <= -243.6f ) {return;}
		else if ( g_map_far_data->pos.z >= -376.93f && g_map_far_data->pos.z <= -352.83f ) {return;}
		else if ( g_map_far_data->pos.z >= -467.01f && g_map_far_data->pos.z <= -454.16f ) {return;}
	}
	// 3-2‚Í–³‚µ
	else if (stage_id == GSD_MAIN_STAGE_ID_3_3) {
		if ( g_map_far_data->pos.z >= -117.26f) {return;}
		else if ( g_map_far_data->pos.z >= -183.14f && g_map_far_data->pos.z <= -117.26f && g_map_far_data->pos.y <= 26.73f) {return;}
		else if ( g_map_far_data->pos.z >= -444.00f && g_map_far_data->pos.z <= -200.00f ) {return;}
		else if ( g_map_far_data->pos.z >= -636.40f && g_map_far_data->pos.z <= -486.14f ) {return;}
	}
	else if (stage_id == GSD_MAIN_STAGE_ID_3_BOSS ){
		if ( g_map_far_data->pos.z >= -115.0f && g_map_far_data->pos.y < 15.0f )
		{
		}
		else if ( g_map_far_data->pos.z <= -245.0f )
		{
		}
		else if ( g_map_far_data->pos.z <= -105.0f && g_map_far_data->pos.z >= -220.0f && g_map_far_data->pos.y >= 40.0f )
		{
		}
		else
		{
			return;
		}
	}
	else if (stage_id == GSD_MAIN_STAGE_ID_FINAL_1) {
		if (g_map_far_data->pos.z >= -5.6f) {
			return;
		}
		else if (g_map_far_data->pos.z >= -13.3f && g_map_far_data->pos.z <= -7.15f) {
			return;
		}
		else if (g_map_far_data->pos.z >= -21.3f && g_map_far_data->pos.z <= -15.65f) {
			return;
		}
	}
#endif	//_IPHONE

	pWork->disp_flag |= (OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE | OBD_DISP_USERMTX);

	nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
#if _PC
	nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 400.0f, -97.0f, -20.0f);
#elif _IPHONE
	if ( stage_id == GSD_MAIN_STAGE_ID_1_BOSS )
	{
		pWork->disp_flag |= OBD_DISP_REPEAT;
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 250.0f, -97.0f, -20.0f);
	}
	else if ( stage_id >= GSD_MAIN_STAGE_ID_2_1 && stage_id <= GSD_MAIN_STAGE_ID_2_BOSS)
	{
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 250.0f, -70.0f, -50.0f);
	}
	else if ( stage_id >= GSD_MAIN_STAGE_ID_3_1 && stage_id <= GSD_MAIN_STAGE_ID_3_BOSS)
	{
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 10.0f, -30.0f, -50.0f);
	}
	else if ( stage_id >= GSD_MAIN_STAGE_ID_FINAL_1 &&  stage_id <= GSD_MAIN_STAGE_ID_FINAL_5 )
	{
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 250.0f, -70.0f, -50.0f);
	}
#endif
	ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
}
#endif


// ==========================================================================
// gmMapFarSceneObjFuncDrawRockA
/*!
 * Šâ‚ğ•¡”ŒÂ•`‰æi‘å‚«‚Èˆê‚ÂŠâj
 *
 *	@param	work	[in]	ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmMapFarSceneObjFuncDrawRockA(OBS_OBJECT_WORK *pWork)
{
	OBS_ACTION3D_NN_WORK* obj_3d = pWork->obj_3d;
	float offsetY = -10.0f;
	float offsetX = -100.0f;
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();

	pWork->disp_flag |= (OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE | OBD_DISP_USERMTX);

#if GMD_MAP_FAR_ROCK_PRIMITIVE
	return;
#endif

	// Œ©‚¦‚Ä‚¢‚È‚¢‚Ì‚Å•`‰æ‚µ‚È‚¢
	if ( gmMapFarDrawCheckRock() == 0 ) return;

	if (  (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3))
	{
		if ( g_map_far_data->pos.z <= -18.0f && g_map_far_data->pos.z >= -295.0f)
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX - 40, offsetY, -90);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}

		if ( g_map_far_data->pos.z <= -112.0f && g_map_far_data->pos.z >= -366.0f)
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 100, offsetY, -240);
			nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 2.0f, 2.0f, 2.0f);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}

		if (g_map_far_data->pos.z <= -238.0f && g_map_far_data->pos.z >= -559.0f)
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 30, offsetY, -400);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}
	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_1_BOSS )
	{
#if !_IPHONE
		nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 180, offsetY, 40);
		nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 0.7f, 0.7f, 0.7f);
		ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
#endif
		nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX - 30, offsetY, -100);
		nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 3.0f, 3.0f, 3.0f);
		ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		
	}
	else
	{
		// ‚PŒÂ–Ú
		if ( g_map_far_data->pos.z <= -10.0f && g_map_far_data->pos.z >= -260.0f)
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 100, offsetY, -135);
			nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 2.0f, 2.0f, 2.0f);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}

		// ‚QŒÂ–Ú
		if ( g_map_far_data->pos.z <= -84.0f && g_map_far_data->pos.z >= -500.0f )
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX - 40, offsetY, -290);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}

		// ‚RŒÂ–Ú
		if ( g_map_far_data->pos.z <= -182.0f && g_map_far_data->pos.z >= -475.0f )
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 80, offsetY, -330);
			nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 2.0f, 2.0f, 2.0f);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}
	}
}

// ==========================================================================
// gmMapFarSceneObjFuncDrawRockB
/*!
 * Šâ‚ğ•¡”ŒÂ•`‰æi“ñ‚Â‚ÉŠ„‚ê‚½‘åŠâj
 *
 *	@param	work	[in]	ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmMapFarSceneObjFuncDrawRockB(OBS_OBJECT_WORK *pWork)
{
	OBS_ACTION3D_NN_WORK* obj_3d = pWork->obj_3d;
	float offsetY = -10.0f;
	float offsetX = -100.0f;
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	
	pWork->disp_flag |= (OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE | OBD_DISP_USERMTX);

#if GMD_MAP_FAR_ROCK_PRIMITIVE
	return;
#endif

	// Œ©‚¦‚Ä‚¢‚È‚¢‚Ì‚Å•`‰æ‚µ‚È‚¢
	if ( gmMapFarDrawCheckRock() == 0 ) return;

	if (  (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3) )
	{
		if (g_map_far_data->pos.z <= -18.0f && g_map_far_data->pos.z >= -256.0f)
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 60, offsetY, -100);
			nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 2.5f, 2.5f, 2.5f);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}

		if (g_map_far_data->pos.z <= -112.0f && g_map_far_data->pos.z >= -406.0f)
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 20, offsetY, -280);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}

		if (g_map_far_data->pos.z <= -275.0f && g_map_far_data->pos.z >= -526.0f)
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 100, offsetY, -400);
			nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 2.0f, 2.0f, 2.0f);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}
	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_1_BOSS )
	{
#if !_IPHONE
		nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 30, offsetY, -40);
		ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
#endif

		// ’Ç‰Á•ª
		nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX, offsetY, 75);
		nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 5.0f, 5.0f, 5.0f);
		ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );

		nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX - 130, offsetY, -180);
		nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 3.0f, 3.0f, 3.0f);
		ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
	
	}
	else
	{
		if (g_map_far_data->pos.z <= -6.0f && g_map_far_data->pos.z >= -317.0f)
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 40, offsetY, -160);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}

		if (g_map_far_data->pos.z <= -100.0f && g_map_far_data->pos.z >= -397.0f)
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 70, offsetY, -250);
			nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 2.0f, 2.0f, 2.0f);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}

		if (g_map_far_data->pos.z <= -314.0f && g_map_far_data->pos.z >= -574.0f)
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 100, offsetY, -440);
			nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 2.5f, 2.5f, 2.5f);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}
	}
}

// ==========================================================================
// gmMapFarSceneObjFuncDrawRockC
/*!
 * Šâ‚ğ•¡”ŒÂ•`‰æiŒQ“‡j
 *
 *	@param	work	[in]	ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmMapFarSceneObjFuncDrawRockC(OBS_OBJECT_WORK *pWork)
{
	OBS_ACTION3D_NN_WORK* obj_3d = pWork->obj_3d;
	float offsetY = -10.0f;
	float offsetX = -100.0f;
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	
	pWork->disp_flag |= (OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE | OBD_DISP_USERMTX);

#if GMD_MAP_FAR_ROCK_PRIMITIVE
	return;
#endif

	// Œ©‚¦‚Ä‚¢‚È‚¢‚Ì‚Å•`‰æ‚µ‚È‚¢
	if ( gmMapFarDrawCheckRock() == 0 ) return;
	
	if (  (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3))
	{
		if (g_map_far_data->pos.z <= -18.0f && g_map_far_data->pos.z >= -387.0f)
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 20, offsetY, -180);
			nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 2.0f, 2.0f, 2.0f);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}

		if (g_map_far_data->pos.z <= -167.0f && g_map_far_data->pos.z >= -582.0f)
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 80, offsetY, -360);
			nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 4.0f, 4.0f, 4.0f);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}

	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_1_BOSS )
	{
		nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 160, offsetY, 10);
		nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 2.0f, 2.0f, 2.0f);
		ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
	}
	else
	{
		if (g_map_far_data->pos.z <= -6.0f && g_map_far_data->pos.z >= -526.0f)
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 30, offsetY, -250);
			nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 5.0f, 5.0f, 5.0f);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}

		if (g_map_far_data->pos.z <= -202.0f && g_map_far_data->pos.z >= -577.0f)
		{
			nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX + 40, offsetY, -380);
			nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 2.0f, 2.0f, 2.0f);
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
		}
	}
}

// ==========================================================================
// gmMapFarDrawCheckRock
/*!
 * Šâ‚ğ•`‰æ‚µ‚Ä‚à‚æ‚¢‚©‚Ç‚¤‚©‚ğƒ`ƒFƒbƒN
 *	@output	1 •`‚¢‚Ä‚à‚æ‚¢    0 •`‚­•K—v‚È‚µ
 */
// ==========================================================================
Sint32 gmMapFarDrawCheckRock(void)
{
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();

	if ( stage_id == GSD_MAIN_STAGE_ID_1_3 )
	{

	}
	else if (stage_id == GSD_MAIN_STAGE_ID_1_BOSS)
	{
#if GMD_MAP_FAR_2DBG
		return 0;
#endif
	}
	// Zone1-1
	else if (stage_id == GSD_MAIN_STAGE_ID_1_1)
	{
#if _IPHONE
		if ( g_map_far_data->pos.y <= 5.2f && g_map_far_data->pos.z >= -436.0f) return 0;
		if ( g_map_far_data->pos.y <= 4.14f && g_map_far_data->pos.z < -436.0f && g_map_far_data->pos.z >= -471.4f) return 0;
		if ( g_map_far_data->pos.y <= 10.0f && g_map_far_data->pos.z < -182.0f && g_map_far_data->pos.z >= -192.0f) return 0;
#else
		if (g_map_far_data->pos.y <= 5.2f) return 0;
#endif	
		if (g_map_far_data->pos.y <= 5.40f && g_map_far_data->pos.z <= -62.0f && g_map_far_data->pos.z >= -110.0f)
		{
			return 0;
		}
#if !_IPHONE
		if (g_map_far_data->pos.y <= 13.0f && g_map_far_data->pos.z <= -179.0f && g_map_far_data->pos.z >= -182.0f)
		{
			return 0;
		}
#endif // !_IPHONE
		if (g_map_far_data->pos.y <= 8.85f && g_map_far_data->pos.z <= -267.0f && g_map_far_data->pos.z >= -308.0f)
		{
			return 0;
		}

		if (g_map_far_data->pos.y <= 9.97f && g_map_far_data->pos.z <= -337.0f && g_map_far_data->pos.z >= -358.0f)
		{
			return 0;
		}
		if (g_map_far_data->pos.y >= 7.0f && g_map_far_data->pos.z <= -347.0f && g_map_far_data->pos.z >= -359.0f)
		{
			return 0;
		}
		if (g_map_far_data->pos.z <= -495.0f && g_map_far_data->pos.z >= -503.0f)
		{
			return 0;
		}
	}
	else if (stage_id == GSD_MAIN_STAGE_ID_1_2)
	{
#if _IPHONE
		if (g_map_far_data->pos.y <= 2.95f) return 0;
#else
		if (g_map_far_data->pos.y <= 4.62f) return 0;
		if (g_map_far_data->pos.y <= 5.85f && g_map_far_data->pos.z >= -50.0f) return 0;
#endif

		if (g_map_far_data->pos.y <= 6.47f && g_map_far_data->pos.z <= -135.4f && g_map_far_data->pos.z >= -156.0f) return 0;
		if (g_map_far_data->pos.y <= 4.5f && g_map_far_data->pos.z <= -135.4f && g_map_far_data->pos.z >= -170.0f) return 0;
		if (g_map_far_data->pos.z <= -419.0f) return 0;
	}

	return 1;
}

// ==========================================================================
// gmMapFarDrawCheckYakei
/*!
 * –éŒi‚ğ•`‰æ‚µ‚Ä‚à‚æ‚¢‚©‚Ç‚¤‚©‚ğƒ`ƒFƒbƒN
 *	@output	1 •`‚¢‚Ä‚à‚æ‚¢    0 •`‚­•K—v‚È‚µ
 */
// ==========================================================================
Sint32 gmMapFarDrawCheckYakei(void)
{
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();

	if ( stage_id == GSD_MAIN_STAGE_ID_2_1 )
	{
#if _IPHONE
		if ( g_map_far_data->pos.y >= 26.5f && g_map_far_data->pos.z <= -36.7f && g_map_far_data->pos.z >= -41.9f) return 0;
		if ( g_map_far_data->pos.y <= 25.46f && g_map_far_data->pos.z <= -50.88f && g_map_far_data->pos.z >= -58.56f )
		{
			return 0;
		}
		if ( g_map_far_data->pos.y >= 7.06f && g_map_far_data->pos.y <= 31.67f && g_map_far_data->pos.z <= -62.56f && g_map_far_data->pos.z >= -64.97f )
		{
			return 0;
		}
#else
		if ( g_map_far_data->pos.y <= 10.0f && g_map_far_data->pos.z <= -48.0f && g_map_far_data->pos.z >= -50.8f) return 0;
		if ( g_map_far_data->pos.y <= 33.0f && g_map_far_data->pos.z <= -170.0f && g_map_far_data->pos.z >= -186.0f )
		{
			return 0;
		}
		if ( g_map_far_data->pos.y >= 32.0f && g_map_far_data->pos.z <= -123.0f && g_map_far_data->pos.z >= -138.0f )
		{
			return 0;
		}
		if ( g_map_far_data->pos.y >= 19.0f && g_map_far_data->pos.y <= 27.0f && 
			g_map_far_data->pos.z <= -208.0f && g_map_far_data->pos.z >= -224.0f )
		{
			return 0;
		}
#endif // _IPHONE
	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_2_3 )
	{
#if _IPHONE
		if ( g_map_far_data->pos.y <= 9.55f) return 0;
		if ( g_map_far_data->pos.y <= 15.31f && g_map_far_data->pos.y >= 13.0f && g_map_far_data->pos.z <= -56.48f) return 0;
		if ( g_map_far_data->pos.y <= 19.39f && g_map_far_data->pos.y >= 16.63f && g_map_far_data->pos.z <= -58.88f) return 0;
		if ( g_map_far_data->pos.y <= 19.39f && g_map_far_data->pos.y >= 16.42f && g_map_far_data->pos.z >= -37.0f) return 0;
		if ( g_map_far_data->pos.y <= 32.49f && g_map_far_data->pos.y >= 29.01f && g_map_far_data->pos.z >= -29.98f) return 0;
#else
		if ( g_map_far_data->pos.y <= 11.95f) return 0;
		if ( g_map_far_data->pos.y <= 24.0f && g_map_far_data->pos.z <= -197.0f && g_map_far_data->pos.z >= -229.0f) return 0;
		if ( g_map_far_data->pos.y <= 24.0f && g_map_far_data->pos.y >= 20.38f && g_map_far_data->pos.z >= -116.5f) return 0;
		if ( g_map_far_data->pos.y >= 35.55f && g_map_far_data->pos.y <= 40.76f && g_map_far_data->pos.z >= -95.0f) return 0;
		if ( g_map_far_data->pos.y >= 35.55f && g_map_far_data->pos.z >= -81.0f) return 0;
#endif // _IPHONE
	}

	return 1;
}

// ==========================================================================
// gmMapFarSceneObjFuncDrawBg
/*!
 * ”wŒi•`‰æ
 *
 *	@param	work	[in]	ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmMapFarSceneObjFuncDrawBg(OBS_OBJECT_WORK *pWork)
{
	OBS_ACTION3D_NN_WORK* obj_3d = pWork->obj_3d;
	float offsetY = -30.0f;
	float offsetX = 50.0f;
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();

	pWork->disp_flag |= (OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE | OBD_DISP_USERMTX);

	if ( ObjObjectPauseCheck(obj_3d->flag) )
	{
		pWork->disp_flag |= OBD_DISP_NOUPDATE;
	} else {
		pWork->disp_flag &= ~OBD_DISP_NOUPDATE;
	}

	if ( GSD_MAIN_ZONE_TYPE_2 == zone_type)
	{
		if ( gmMapFarDrawCheckYakei() == 0 ) return;

		pWork->disp_flag |= OBD_DISP_REPEAT;
		nnMakeUnitMatrix(&obj_3d->user_obj_mtx);

		if ( stage_id == GSD_MAIN_STAGE_ID_2_BOSS) 
		{
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX, offsetY+10, 0);
		}
		else
		{
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX, offsetY, -135);
		}
		nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 1.0f, 1.0f, 1.0f);
		ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
	}
	else if (GSD_MAIN_ZONE_TYPE_3 == zone_type)
	{
		// •‰‰×ŒyŒ¸‘Îôiƒ\ƒjƒbƒN‚ªˆê’è‚Ì”ÍˆÍ‚É‚¢‚é‚Æ‚«‚Í‰“Œi‚ªŒ©‚¦‚È‚¢‚Ì‚Å•`‰æ‚µ‚È‚¢j
		// ƒf[ƒ^‚ª•ÏX‚³‚ê‚é‚Æ’l‚à•Ï‚í‚é‚Ì‚ÅAƒf[ƒ^Šm’èŒã‚Éƒpƒ‰ƒ[ƒ^‚ğ’²®‚·‚é‚±‚Æ
		if ( stage_id == GSD_MAIN_STAGE_ID_3_1 )
		{
			if ( g_map_far_data->pos.z <= -312.5f && g_map_far_data->pos.z >= -333.0f )
			{
				return;
			}
		}
		else if ( stage_id == GSD_MAIN_STAGE_ID_3_3 )
		{
			if ( g_map_far_data->pos.z >= -102.0f) 
			{
				return;
			}
			if ( g_map_far_data->pos.z <= -102.0f && g_map_far_data->pos.z >= -160.0f && g_map_far_data->pos.y <= 54.0f) 
			{
				return;
			}
			if ( g_map_far_data->pos.z <= -177.56f && g_map_far_data->pos.z >= -392.0f) 
			{
				return;
			}
			if ( g_map_far_data->pos.z <= -410.0f && g_map_far_data->pos.z >= -431.0f && g_map_far_data->pos.y <= 64.0f) 
			{
				return;
			}
			if ( g_map_far_data->pos.z <= -431.0f && g_map_far_data->pos.z >= -550.0f) 
			{
				return;
			}
			if ( g_map_far_data->pos.z >= -550.0f && g_map_far_data->pos.y < 50.0f)
			{
				return;
			}
		}
		else if ( stage_id == GSD_MAIN_STAGE_ID_3_BOSS )
		{
			if ( g_map_far_data->pos.z >= -103.0f && g_map_far_data->pos.y < 24.0f )
			{
			}
			else if ( g_map_far_data->pos.z <= -216.0f )
			{
			}
			else if ( g_map_far_data->pos.z <= -95.0f && g_map_far_data->pos.z >= -190.0f && g_map_far_data->pos.y >= 84.0f )
			{
			}
			else
			{
				return;
			}
		}

		pWork->disp_flag |= OBD_DISP_REPEAT;
		nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX, offsetY, -160);
		nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 1.0f, 1.0f, 1.0f);
		ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
	}
	else if (GSD_MAIN_ZONE_TYPE_FINAL == zone_type)
	{
	//	pWork->disp_flag = 0;
		pWork->disp_flag |= OBD_DISP_NOPOS | OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE |
					OBD_DISP_NODIR | OBD_DISP_NODIRFLIP | OBD_DISP_USERMTX;

		nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
		//nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX+250, offsetY+30.0f, g_map_far_data->pos.z);
		
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, g_map_far_data->pos.x, -10, g_map_far_data->pos.z);
		nnRotateYMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, (u16)NNM_DEGtoA16(90.0f));
		ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
	}
	else if (GSD_MAIN_ZONE_TYPE_SS == zone_type)
	{
		if (g_gm_main_system.game_flag & GMD_GAME_FLAG_CLEAR) {
			// ƒNƒŠƒAŒã‚Í”ñ•\¦
			return;
		}
		offsetX = (float)(OBD_LCD_X);
		offsetY = (float)(OBD_LCD_Y);
		pWork->dir.y = 0xc000;
		pWork->disp_flag |= OBD_DISP_REPEAT;

		nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX, offsetY, -160);
		nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 3.0f, 3.0f, 3.0f);
		ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
	}

}

#if GMD_MAP_FAR_ROCK_PRIMITIVE
// ==========================================================================
// gmMapFarSceneObjFuncDrawRotate
/*!
 *  Šâ‚ğƒvƒŠƒ~ƒeƒBƒu‚ÅˆêŠ‡•`‰æ‚·‚é(iPhone)
 *
 *	@param	work	[in]	ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmMapFarSceneObjFuncDrawRockPrimitive( OBS_OBJECT_WORK* pWork )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	Uint32 rockAllPoliNum = 0;
	Uint32 poliId = 0;

	if ( !amDrawIsRegistComplete(data->regId) ) return;
	if ( pWork->disp_flag & OBD_DISP_NODISP ) return;

	// Šâ‚Ì”‚ğ‹‚ß‚é
	Uint32 rockANum = gmMapFarSceneGetRockANum();
	Uint32 rockBNum = gmMapFarSceneGetRockBNum();
	Uint32 rockCNum = gmMapFarSceneGetRockCNum();

	// Šâ‚Ì’¸“_”
	Uint32 vnumA = AoTvxGetVertexNum(data->tvx_rockA, 0);
	Uint32 vnumB = AoTvxGetVertexNum(data->tvx_rockB, 0);
	Uint32 vnumC = AoTvxGetVertexNum(data->tvx_rockC, 0);

	AMS_PARAM_DRAW_PRIMITIVE param;
	memset(&param, 0, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	param.aTest = 1;
	param.zMask = 0;
	param.zTest = 1;
	param.ablend = NNE_PRIM_ALPHABLEND_ON;

#if _PC
	param.bldSrc = NNE_BLENDMODE_SRCALPHA;
	param.bldDst = NNE_BLENDMODE_INVSRCALPHA;
	param.bldMode = NNE_BLENDOP_ADD;
#elif _IPHONE
	param.bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
	param.bldDst = NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA;
	param.bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif

	// ŠâA,ŠâB,ŠâC‚Åg‚¤‘S’¸“_”‚ğŒvZiŠâ‚Ì”~’¸“_”{k‘Ş’¸“_”j
	if ( rockANum > 0 )
	{
		rockAllPoliNum += (vnumA * rockANum + (rockANum-1)*2);
	}
	if ( rockBNum > 0 )
	{
		rockAllPoliNum += (vnumB * rockBNum + (rockBNum-1)*2);
	}
	if ( rockCNum > 0 )
	{
		rockAllPoliNum += (vnumC * rockCNum + (rockCNum-1)*2);
	}
	if ( rockAllPoliNum == 0 ) return;

	// ‘S•”‚ÌŠâ‚Ìƒ|ƒŠƒSƒ“ƒf[ƒ^‚ğŠm•Û
	NNS_PRIM3D_PCT *poliData = (NNS_PRIM3D_PCT *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * rockAllPoliNum);

	// ŠâA
	poliId = gmMapFarSceneObjFuncDrawRockAPrimitive(poliData, poliId);
	poliId = gmMapFarSceneObjFuncDrawRockBPrimitive(poliData, poliId);
	poliId = gmMapFarSceneObjFuncDrawRockCPrimitive(poliData, poliId);

	// ƒvƒŠƒ~ƒeƒBƒu•`‰æİ’è
	param.format3D = NNE_PRIM3D_FMT_PCT;
	param.type = NNE_PRIM_TRIANGLE_STRIP;
	param.vtxPCT3D = poliData;
	param.texlist = data->texlist;
	param.texId = 0;
	param.count = rockAllPoliNum;
	amDrawPrimitive3D(OBD_DRAW_CMD_STATE_MAPFAR, &param);
}

// ==========================================================================
// gmMapFarSceneObjFuncDrawRotate
/*!
 *  ŠâAƒvƒŠƒ~ƒeƒBƒuî•ñİ’è
 *
 *	@param	poliData	[in]	ƒ|ƒŠƒSƒ“ƒf[ƒ^
	@param  startId     [in]    ƒ|ƒŠƒSƒ“ƒf[ƒ^‚Ìƒf[ƒ^Ši”[ŠJnˆÊ’u
	@return	ÅŒã‚ÉŠi”[‚µ‚½’¸“_ˆÊ’u	
 */
// ==========================================================================
Uint32 gmMapFarSceneObjFuncDrawRockAPrimitive( NNS_PRIM3D_PCT *poliData, Uint32 startId  )
{
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	AOS_TVX_VERTEX* vtx = (AOS_TVX_VERTEX*)AoTvxGetVertex(data->tvx_rockA, 0);
	Uint32 vnum = AoTvxGetVertexNum(data->tvx_rockA, 0);
	float offsetY = -10.0f;
	float offsetX = -100.0f;

#if _PC
	float scaleY = 0.35f;
#elif _IPHONE
	float scaleY = 0.3f;
#endif
	Uint32 p = startId;

	// Œ©‚¦‚Ä‚¢‚È‚¢‚Ì‚Å•`‰æ‚µ‚È‚¢
	if ( gmMapFarDrawCheckRock() == 0 ) return p;

	if (  (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3))
	{
		for (Uint32 i = 0; i < vnum; i++, p++)
		{
			amVectorSet((NNS_VECTOR*)&poliData[p], offsetX -40.0f + vtx[i].x, offsetY + vtx[i].y * scaleY,  vtx[i].z -90.0f );
			poliData[p].Col = vtx[i].c;
			poliData[p].Tex.u = vtx[i].u; poliData[p].Tex.v = vtx[i].v;
		}
		poliData[p] = poliData[p-1]; // k‘Şƒ|ƒŠƒSƒ“—pi––”öj

		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, 100.0f, -240.0f, 2.0f);
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, 30.0f, -400.0f, 1.0f);
	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_1_BOSS )
	{
		for (Uint32 i = 0; i < vnum; i++, p++)
		{
			amVectorSet((NNS_VECTOR*)&poliData[p], offsetX + 180.0f + vtx[i].x*0.7f, offsetY + vtx[i].y * scaleY*0.7f,  vtx[i].z*0.7f + 40.0f );
			poliData[p].Col = vtx[i].c;
			poliData[p].Tex.u = vtx[i].u; poliData[p].Tex.v = vtx[i].v;
		}
		poliData[p] = poliData[p-1]; // k‘Şƒ|ƒŠƒSƒ“—pi––”öj

		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, -30.0f, -100.0f, 3.0f);

	}
	else
	{
		for (Uint32 i = 0; i < vnum; i++, p++)
		{
			amVectorSet((NNS_VECTOR*)&poliData[p], offsetX + 100.0f + vtx[i].x*2.0f, offsetY + vtx[i].y * scaleY*2.0f,  vtx[i].z*2.0f -135.0f );
			poliData[p].Col = vtx[i].c;
			poliData[p].Tex.u = vtx[i].u; poliData[p].Tex.v = vtx[i].v;
		}
		poliData[p] = poliData[p-1]; // k‘Şƒ|ƒŠƒSƒ“—pi––”öj

		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, -40.0f, -290.0f, 1.0f);
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, 80.0f, -330.0f, 2.0f);
	}

	return p;
}

// ==========================================================================
// gmMapFarSceneObjFuncDrawRotate
/*!
 *  ŠâBƒvƒŠƒ~ƒeƒBƒuî•ñİ’è
 *
 *	@param	poliData	[in]	ƒ|ƒŠƒSƒ“ƒf[ƒ^
	@param  startId     [in]    ƒ|ƒŠƒSƒ“ƒf[ƒ^‚Ìƒf[ƒ^Ši”[ŠJnˆÊ’u
	@return	ÅŒã‚ÉŠi”[‚µ‚½’¸“_ˆÊ’u	
 */
// ==========================================================================
Uint32 gmMapFarSceneObjFuncDrawRockBPrimitive( NNS_PRIM3D_PCT *poliData, Uint32 startId )
{
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	AOS_TVX_VERTEX* vtx = (AOS_TVX_VERTEX*)AoTvxGetVertex(data->tvx_rockB, 0);
	Uint32 vnum = AoTvxGetVertexNum(data->tvx_rockB, 0);
	Uint32 p = startId;

	// Œ©‚¦‚Ä‚¢‚È‚¢‚Ì‚Å•`‰æ‚µ‚È‚¢
	if ( gmMapFarDrawCheckRock() == 0 ) return p;

	if (  (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3) )
	{
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, 60.0f, -100.0f, 2.5f);
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, 20.0f, -280.0f, 1.0f);
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, 100.0f, -400.0f, 2.0f);
	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_1_BOSS )
	{
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, 30.0f, -40.0f, 1.0f);
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, 0.0f, 75.0f, 5.0f);
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, -130.0f, -180.0f, 3.0f);
	}
	else
	{
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, 40.0f, -160.0f, 1.0f);
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, 70.0f, -250.0f, 2.0f);
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, 100.0f, -440.0f, 2.5f);
	}

	return p;
}

// ==========================================================================
// gmMapFarSceneObjFuncDrawRotate
/*!
 *  ŠâCƒvƒŠƒ~ƒeƒBƒuî•ñİ’è
 *
 *	@param	poliData	[in]	ƒ|ƒŠƒSƒ“ƒf[ƒ^
	@param  startId     [in]    ƒ|ƒŠƒSƒ“ƒf[ƒ^‚Ìƒf[ƒ^Ši”[ŠJnˆÊ’u
	@return	ÅŒã‚ÉŠi”[‚µ‚½’¸“_ˆÊ’u	
 */
// ==========================================================================
Uint32 gmMapFarSceneObjFuncDrawRockCPrimitive( NNS_PRIM3D_PCT *poliData, Uint32 startId )
{
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	AOS_TVX_VERTEX* vtx = (AOS_TVX_VERTEX*)AoTvxGetVertex(data->tvx_rockC, 0);
	Uint32 vnum = AoTvxGetVertexNum(data->tvx_rockC, 0);
	Uint32 p = startId;

	// Œ©‚¦‚Ä‚¢‚È‚¢‚Ì‚Å•`‰æ‚µ‚È‚¢
	if ( gmMapFarDrawCheckRock() == 0 ) return p;

	if (  (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3))
	{
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, 20.0f, -180.0f, 2.0f);
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 1, 80.0f, -360.0f, 4.0f);
	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_1_BOSS )
	{
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 1, 160.0f, 10.0f, 2.0f);
	}
	else
	{
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 0, 30.0f, -250.0f, 5.0f);
		p = gmMapFarSceneObjFuncSetRockPrimitiveVtx(poliData, vtx, vnum, p, 1, 40.0f, -380.0f, 2.0f);
	}

	return p;
}

// ==========================================================================
// gmMapFarSceneObjFuncSetRockPrimitiveVtx
/*!
 *  ‚Q‚Â–ÚˆÈ~‚ÌŠâƒvƒŠƒ~ƒeƒBƒuî•ñİ’è(’¸“_AƒJƒ‰[AUV)
 *
 *	@param	poliData	[in]	ƒ|ƒŠƒSƒ“ƒf[ƒ^
	@param	vtx			[in]	’¸“_ƒf[ƒ^
	@param	vnum		[in]	’¸“_”
	@param  startId     [in]    ƒ|ƒŠƒSƒ“ƒf[ƒ^‚Ìƒf[ƒ^Ši”[ŠJnˆÊ’u
	@param  finalFlag   [in]    ÅŒã‚ÌƒvƒŠƒ~ƒeƒBƒu‚Ìê‡‚Í 1
	@param  ofx         [in]    ƒIƒtƒZƒbƒgX’l
	@param  ofz         [in]    ƒIƒtƒZƒbƒgZ’l
	@param  scale       [in]    ƒXƒP[ƒ‹’l
	@return	ÅŒã‚ÉŠi”[‚µ‚½’¸“_ˆÊ’u	
 */
// ==========================================================================
Uint32 gmMapFarSceneObjFuncSetRockPrimitiveVtx( NNS_PRIM3D_PCT *poliData, AOS_TVX_VERTEX* vtx, Uint32 vnum,
												   Uint32 startId, Sint32 finalFlag, float ofx, float ofz, float scale )
{
	float offsetY = -10.0f;
	float offsetX = -100.0f;
#if _PC
	float scaleY = 0.35f;
#elif _IPHONE
	float scaleY = 0.3f;
#endif
	Uint32 p = startId;

	p += 2; // k‘Şƒ|ƒŠƒSƒ“iæ“ªj‚Ì—Ìˆæ‚Í‚Ü‚¾Ši”[‚µ‚È‚¢
	for (Uint32 i = 0; i < vnum; i++, p++)
	{
		amVectorSet((NNS_VECTOR*)&poliData[p], offsetX + ofx + vtx[i].x*scale, offsetY + vtx[i].y * scaleY*scale,  vtx[i].z*scale + ofz );
		poliData[p].Col = vtx[i].c;
		poliData[p].Tex.u = vtx[i].u; poliData[p].Tex.v = vtx[i].v;
	}
	p -= (vnum+1);
	poliData[p] = poliData[p+1]; // k‘Şƒ|ƒŠƒSƒ“iæ“ªj‚Í‚±‚±‚Åw’è
	p += (vnum+1);

	if ( finalFlag == 0 )
		poliData[p] = poliData[p-1]; // k‘Şƒ|ƒŠƒSƒ“—pi––”öj

	return p;
}

// ==========================================================================
// gmMapFarSceneGetRockANum
/*!
 *  ŠâA‚Ì”‚ğæ“¾‚·‚é
 *
 *	@param	work	[in]	ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
static Uint32 gmMapFarSceneGetRockANum( void )
{
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	Uint32	rockNum = 0;

	// Œ©‚¦‚Ä‚¢‚È‚¢‚Ì‚Å•`‰æ‚µ‚È‚¢
	if ( gmMapFarDrawCheckRock() == 0 ) return 0;

	if (  (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3))
	{
//		if ( g_map_far_data->pos.z <= -18.0f && g_map_far_data->pos.z >= -295.0f)
			rockNum++;

//		if ( g_map_far_data->pos.z <= -112.0f && g_map_far_data->pos.z >= -366.0f)
			rockNum++;

//		if (g_map_far_data->pos.z <= -238.0f && g_map_far_data->pos.z >= -559.0f)
			rockNum++;
	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_1_BOSS )
	{
		rockNum = 2;
	}
	else
	{
//		if ( g_map_far_data->pos.z <= -10.0f && g_map_far_data->pos.z >= -260.0f)
			rockNum++;

//		if ( g_map_far_data->pos.z <= -84.0f && g_map_far_data->pos.z >= -500.0f )
			rockNum++;

//		if ( g_map_far_data->pos.z <= -182.0f && g_map_far_data->pos.z >= -475.0f )
			rockNum++;
	}

	return rockNum;
}

// ==========================================================================
// gmMapFarSceneGetRockBNum
/*!
 *  ŠâB‚Ì”‚ğæ“¾‚·‚é
 *
 *	@param	work	[in]	ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
static Uint32 gmMapFarSceneGetRockBNum( void )
{
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	Uint32	rockNum = 0;

	// Œ©‚¦‚Ä‚¢‚È‚¢‚Ì‚Å•`‰æ‚µ‚È‚¢
	if ( gmMapFarDrawCheckRock() == 0 ) return 0;

	if (  (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3))
	{
//		if (g_map_far_data->pos.z <= -18.0f && g_map_far_data->pos.z >= -256.0f)
			rockNum++;

//		if (g_map_far_data->pos.z <= -112.0f && g_map_far_data->pos.z >= -406.0f)
			rockNum++;

//		if (g_map_far_data->pos.z <= -275.0f && g_map_far_data->pos.z >= -526.0f)
			rockNum++;
	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_1_BOSS )
	{
		rockNum = 3;
	}
	else
	{
//		if (g_map_far_data->pos.z <= -6.0f && g_map_far_data->pos.z >= -317.0f)
			rockNum++;

//		if (g_map_far_data->pos.z <= -100.0f && g_map_far_data->pos.z >= -397.0f)
			rockNum++;

//		if (g_map_far_data->pos.z <= -314.0f && g_map_far_data->pos.z >= -574.0f)
			rockNum++;
	}

	return rockNum;
}

// ==========================================================================
// gmMapFarSceneGetRockCNum
/*!
 *  ŠâC‚Ì”‚ğæ“¾‚·‚é
 *
 *	@param	work	[in]	ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
static Uint32 gmMapFarSceneGetRockCNum( void )
{
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	Uint32	rockNum = 0;

	// Œ©‚¦‚Ä‚¢‚È‚¢‚Ì‚Å•`‰æ‚µ‚È‚¢
	if ( gmMapFarDrawCheckRock() == 0 ) return 0;

	if (  (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3))
	{
	//	if (g_map_far_data->pos.z <= -18.0f && g_map_far_data->pos.z >= -387.0f)
			rockNum++;

	//	if (g_map_far_data->pos.z <= -167.0f && g_map_far_data->pos.z >= -582.0f)
			rockNum++;
	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_1_BOSS )
	{
		rockNum = 1;
	}
	else
	{
	//	if (g_map_far_data->pos.z <= -6.0f && g_map_far_data->pos.z >= -526.0f)
			rockNum++;

	//	if (g_map_far_data->pos.z <= -202.0f && g_map_far_data->pos.z >= -577.0f)
			rockNum++;
	}

	return rockNum;
}


#endif

// ==========================================================================
// gmMapFarSceneObjFuncDrawRotate
/*!
 * ƒIƒuƒWƒFƒNƒg‚ğ‰ñ“]‚³‚¹‚È‚ª‚ç•`‰æi‹ó‚Ì“V‹…j
 *
 *	@param	work	[in]	ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmMapFarSceneObjFuncDrawRotate(OBS_OBJECT_WORK *pWork)
{
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
//	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	OBS_ACTION3D_NN_WORK* obj_3d = pWork->obj_3d;
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();

	// ‰ñ“]XV
	if ( ObjObjectPauseCheck(obj_3d->flag) == 0)
	{
		if ( GSD_MAIN_ZONE_TYPE_1 == zone_type )
		{
			g_map_far_data->degSky += (amSystemGetFrameRateMain() * 0.005f);
			g_map_far_data->degSky2 += (amSystemGetFrameRateMain() * 0.01f);
		}
		else if ( GSD_MAIN_ZONE_TYPE_FINAL == zone_type )
		{
			g_map_far_data->degSky += (amSystemGetFrameRateMain() * 0.01f);
			g_map_far_data->degSky2 += (amSystemGetFrameRateMain() * 0.02f);
		}
	}
	else
	{
		pWork->disp_flag |= OBD_DISP_NOUPDATE;
	}

	if (g_map_far_data->degSky > 360.0f) g_map_far_data->degSky = 0.0f;
	if (g_map_far_data->degSky2 > 360.0f) g_map_far_data->degSky2 = 0.0f;

	// •`‰æ
	nnMakeUnitMatrix(&obj_3d->user_obj_mtx);

	if ( GSD_MAIN_ZONE_TYPE_2 == zone_type)
	{
		pWork->disp_flag |= OBD_DISP_NOPOS | OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE |
					OBD_DISP_NODIR | OBD_DISP_NODIRFLIP | OBD_DISP_USERMTX;

		if ( gmMapFarDrawCheckYakei() == 0 ) return;

		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx,
		g_map_far_data->pos.x, -30, g_map_far_data->pos.z);
		nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 1.4f, 1.4f, 1.4f);
	}
	else if ( GSD_MAIN_ZONE_TYPE_1 == zone_type)
	{
		pWork->disp_flag |= OBD_DISP_REPEAT;
		pWork->disp_flag |= OBD_DISP_NOPOS | OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE |
					OBD_DISP_NODIR | OBD_DISP_NODIRFLIP | OBD_DISP_USERMTX;

#if GMD_MAP_FAR_ROCK_PRIMITIVE
		gmMapFarSceneObjFuncDrawRockPrimitive(pWork); // ŠâƒvƒŠƒ~ƒeƒBƒu•`‰æ
#endif

#if GMD_MAP_FAR_2DBG
		if ( stage_id == GSD_MAIN_STAGE_ID_1_BOSS)
		{
			return;
		}
#endif

		// Œ©‚¦‚Ä‚¢‚È‚¢‚Ì‚Å•`‚©‚È‚¢
		if ( stage_id == GSD_MAIN_STAGE_ID_1_1 )
		{
			if (g_map_far_data->pos.y <= 10.0f && g_map_far_data->pos.z <= -182.0f && g_map_far_data->pos.z >= -191.8f)
			{
				return;
			}

			if (g_map_far_data->pos.y <= 7.85f && g_map_far_data->pos.z <= -269.0f && g_map_far_data->pos.z >= -304.0f)
			{
				return;
			}

			if (g_map_far_data->pos.y <= 9.97f && g_map_far_data->pos.z <= -337.0f && g_map_far_data->pos.z >= -358.0f)
			{
				return;
			}
			if (g_map_far_data->pos.y >= 7.0f && g_map_far_data->pos.z <= -347.0f && g_map_far_data->pos.z >= -359.0f)
			{
				return;
			}
			if (g_map_far_data->pos.z <= -495.0f && g_map_far_data->pos.z >= -503.0f)
			{
				return;
			}
		}
		else if (stage_id == GSD_MAIN_STAGE_ID_1_2)
		{
			if (g_map_far_data->pos.y <= 6.47f && g_map_far_data->pos.z <= -135.4f && g_map_far_data->pos.z >= -156.0f) return;
			if (g_map_far_data->pos.y <= 3.05f && g_map_far_data->pos.z <= -130.4f && g_map_far_data->pos.z >= -190.0f) return;
			if (g_map_far_data->pos.z <= -419.0f) return;
		}

		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx,
		g_map_far_data->pos.x, -10, g_map_far_data->pos.z);
	}
	else if ( GSD_MAIN_ZONE_TYPE_FINAL == zone_type)
	{
		pWork->disp_flag |= OBD_DISP_NOPOS | OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE |
					OBD_DISP_NODIR | OBD_DISP_NODIRFLIP | OBD_DISP_USERMTX;

	//	nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, g_map_far_data->pos.x, -10, -100);
		
#if !_IPHONE
		// ‰_
		if ( pWork->user_work == GMD_MAP_FAR_OBJ_INDEX_ZONE_FINAL_CLOUD )
		{
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, -100, -10, -100);
			nnRotateYMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, (u16)NNM_DEGtoA16(g_map_far_data->degSky2));
			nnRotateZMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, (u16)NNM_DEGtoA16(23.4f));
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
			return;
		}
		// ‘å‹CŒ—
		else if ( pWork->user_work == GMD_MAP_FAR_OBJ_INDEX_ZONE_FINAL_FILM )
		{
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, -100, -10, -100);
			nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 1.0f, 1.0f, 1.0f);
			nnRotateYMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, (u16)NNM_DEGtoA16(75.0f));
		//	nnRotateYMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, (u16)NNM_DEGtoA16(90.0f));
			nnRotateZMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, (u16)NNM_DEGtoA16(23.4f));
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
			return;
		}
		// ’n‹…
		else
		{
			nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, -100, -10, -100);
			nnRotateYMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, (u16)NNM_DEGtoA16(g_map_far_data->degSky));
			nnRotateZMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, (u16)NNM_DEGtoA16(23.4f));
			ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
			return;
		}
#endif
	}

	
	nnRotateYMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, (u16)NNM_DEGtoA16(g_map_far_data->degSky));

	ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );


}

// ==========================================================================
// gmMapFarSceneObjFuncDrawWheel
/*!
 * ŠÏ——Ô•`‰æ
 *
 *	@param	work	[in]	ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
static void gmMapFarSceneObjFuncDrawWheel( OBS_OBJECT_WORK* pWork )
{
	OBS_ACTION3D_NN_WORK* obj_3d = pWork->obj_3d;
	float offsetY = -30.0f;
	float offsetX = 50.0f;
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
#if _IPHONE
	if ( GSD_MAIN_STAGE_ID_2_BOSS == stage_id ){
		return;
	}
#endif	//_IPHONE

	pWork->disp_flag |= OBD_DISP_REPEAT;
	pWork->disp_flag |= (OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE | OBD_DISP_USERMTX);

	if ( stage_id == GSD_MAIN_STAGE_ID_2_1 )
	{
		if (g_map_far_data->pos.z >= -119.4f) return;
		if ( gmMapFarDrawCheckYakei() == 0 ) return;
	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_2_2 /*|| stage_id == GSD_MAIN_STAGE_ID_2_BOSS*/)
	{
		return;
	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_2_3 )
	{
		if (g_map_far_data->pos.z >= -119.4f) return;
	}

	if ( ObjObjectPauseCheck(obj_3d->flag) )
	{
		pWork->disp_flag |= OBD_DISP_NOUPDATE;
	} else {
		pWork->disp_flag &= ~OBD_DISP_NOUPDATE;
	}

	nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
	if ( stage_id == GSD_MAIN_STAGE_ID_2_BOSS) 
	{
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX, offsetY+10, 0);
	}
	else
	{
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX, offsetY, -135);
	}
	nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 1.0f, 1.0f, 1.0f);
	ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
}

// ==========================================================================
// gmMapFarSceneObjFuncDrawSLight
/*!
 * ƒT[ƒ`ƒ‰ƒCƒg‚ğ•`‰æ
 *
 *	@param	work	[in]	ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
static void gmMapFarSceneObjFuncDrawSLight( OBS_OBJECT_WORK* pWork )
{
	OBS_ACTION3D_NN_WORK* obj_3d = pWork->obj_3d;
	float offsetY = -30.0f;
	float offsetX = 50.0f;
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
#if _IPHONE
	if ( GSD_MAIN_STAGE_ID_2_BOSS == stage_id ){
		return;
	}
#endif	//_IPHONE

	pWork->disp_flag |= OBD_DISP_REPEAT;
	pWork->disp_flag |= (OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE | OBD_DISP_USERMTX);

	if ( gmMapFarDrawCheckYakei() == 0 ) return;

	if ( ObjObjectPauseCheck(obj_3d->flag) )
	{
		pWork->disp_flag |= OBD_DISP_NOUPDATE;
	} else {
		pWork->disp_flag &= ~OBD_DISP_NOUPDATE;
	}

	nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
	if ( stage_id == GSD_MAIN_STAGE_ID_2_BOSS) 
	{
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX, offsetY+10, 0);
	}
	else
	{
		nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, offsetX, offsetY, -135);
	}
	
	nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, 1.0f, 1.0f, 1.0f);
	ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );
}

// ==========================================================================
// gmMapFarSceneObjFuncDrawSea
/*!
 * ŠC•`‰æ
 *
 *	@param	work	[in]	ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
void gmMapFarSceneObjFuncDrawSea( OBS_OBJECT_WORK* pWork )
{
	OBS_ACTION3D_NN_WORK *obj_3d = pWork->obj_3d;
	amAssert( obj_3d );
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();

#if GMD_MAP_FAR_SEAMODE
	ObjDraw3DNNUserFunc(gmMapFarDrawSeaUserFunc, NULL, 0, OBD_DRAW_CMD_STATE_MAPFAR);

#if _WII
	obj_3d->material_cb_func = gmMapFarWaterFallMaterial;
	obj_3d->material_cb_param = NULL;
#endif

#endif



	pWork->disp_flag |= OBD_DISP_REPEAT;
	pWork->disp_flag |= (OBD_DISP_NOPOS | OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE |
					OBD_DISP_NODIR | OBD_DISP_NODIRFLIP | OBD_DISP_USERMTX);

	// Œ©‚¦‚Ä‚¢‚È‚¢‚Ì‚Å•`‰æ‚µ‚È‚¢
	if ( stage_id == GSD_MAIN_STAGE_ID_1_1 )
	{
#if !_IPHONE // ƒƒjƒ…[‚É‰½ŒÌ‚©ƒuƒcƒuƒc‚ªo‚é‚½‚ß(PVRTC2?)AŠC‚Íí‚É•`‰æ
#if _IPHONE
		if ( g_map_far_data->pos.y <= 6.13f && g_map_far_data->pos.z >= -436.0f) return;
		if ( g_map_far_data->pos.y <= 4.57f && g_map_far_data->pos.z < -436.0f && g_map_far_data->pos.z >= -471.4f) return;
#else
		if ( g_map_far_data->pos.y <= 6.36f ) return;
#endif
		if (g_map_far_data->pos.y <= 10.0f && g_map_far_data->pos.z <= -182.0f && g_map_far_data->pos.z >= -191.8f)
		{
			return;
		}
		if (g_map_far_data->pos.y <= 7.85f && g_map_far_data->pos.z <= -269.0f && g_map_far_data->pos.z >= -304.0f)
		{
			return;
		}

		if (g_map_far_data->pos.y <= 9.97f && g_map_far_data->pos.z <= -337.0f && g_map_far_data->pos.z >= -358.0f)
		{
			return;
		}
		if (g_map_far_data->pos.y >= 7.0f && g_map_far_data->pos.z <= -347.0f && g_map_far_data->pos.z >= -359.0f)
		{
			return;
		}
		if (g_map_far_data->pos.z <= -495.0f && g_map_far_data->pos.z >= -503.0f)
		{
			return;
		}
#endif // _IPHONE
	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_1_2 )
	{
#if !_IPHONE // ƒƒjƒ…[‚É‰½ŒÌ‚©ƒuƒcƒuƒc‚ªo‚é‚½‚ß(PVRTC2?)AŠC‚Íí‚É•`‰æ
#if _IPHONE
		if ( g_map_far_data->pos.y <= 4.04f ) return;
#else
		if ( g_map_far_data->pos.y <= 6.18f ) return;
#endif

		if (g_map_far_data->pos.y <= 6.47f && g_map_far_data->pos.z <= -135.4f && g_map_far_data->pos.z >= -156.0f) return;
		if (g_map_far_data->pos.z <= -419.0f) return;
#endif // !_IPHONE
	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_1_3 )
	{
#if !_IPHONE // ƒƒjƒ…[‚É‰½ŒÌ‚©ƒuƒcƒuƒc‚ªo‚é‚½‚ß(PVRTC2?)AŠC‚Íí‚É•`‰æ
#if _IPHONE
		if ( g_map_far_data->pos.y <= 1.10f ) return;
#else
		if ( g_map_far_data->pos.y <= 1.49f ) return;
#endif
#endif // !_IPHONE
	}
	else if ( stage_id == GSD_MAIN_STAGE_ID_1_BOSS )
	{
#if GMD_MAP_FAR_2DBG
		return;
#endif
	}

	nnMakeUnitMatrix(&obj_3d->user_obj_mtx);

	float ofstx = 40.0f /*-(obj_3d->object->Radius/1.41421356f) + 20.0f*/;
//	float ofstx = -(obj_3d->object->Radius/1.41421356f) + 20.0f;

	// ƒXƒNƒ[ƒ‹•ª‚ğ‘Å‚¿Á‚µ‚Ä‚¨‚­
	nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, ofstx, -10, g_map_far_data->pos.z);
	

	ObjDrawAction3DNN(pWork->obj_3d, &pWork->pos, &pWork->dir, &pWork->scale, &pWork->disp_flag );

}

// ==========================================================================
// gmMapFarDrawSeaUserFunc
/*!
 * ŠC•`‰æƒ†[ƒUŠÖ”
 *
 */
// ==========================================================================
void gmMapFarDrawSeaUserFunc(void *data)
{
	UNREFERENCED_PARAMETER( data );
	NNS_RGBA_U8 color = {0x00, 0x00, 0x00, 0xFF};
	NNS_MATRIX44 *proj_mtx = amDrawGetProjectionMatrix();

	amDrawEndScene();

#if _WII
	if (proj_mtx != NULL)
		memcpy(_am_draw_fall_projmtx, proj_mtx, sizeof(NNS_MATRIX44));
#endif

	// ƒeƒNƒXƒ`ƒƒ‚Ìİ’è
#if _WII
	amRenderCopyTarget(&_gm_mapFar_render_work, &color);
#elif _XBOX
	amRenderSetTarget(&_am_draw_target, 0, &color);
#else
	amRenderSetTarget(&_am_draw_target,
			AMD_RENDER_CLEAR_COLOR | AMD_RENDER_CLEAR_DEPTH, &color);
#endif
	
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
				_gm_mapFar_render_work.texture_color[0], &mtx, state);

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
				_gm_mapFar_render_work.texture_color[0], &mtx, state);

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
	memcpy(&_am_draw_render_work, &_gm_mapFar_render_work, sizeof(AMS_RENDER_TARGET));
#endif
}

#if _WII
// ==========================================================================
// gmMapFarWaterFallMaterial
/*!
 * ŠCƒ}ƒeƒŠƒAƒ‹ƒR[ƒ‹ƒoƒbƒN
 */
// ==========================================================================
NNE_BOOL gmMapFarWaterFallMaterial(NNS_DRAWCALLBACK_VAL *val, void *param)
{
	UNREFERENCED_PARAMETER(param);
	return (amDrawSeaMaterial(val));
}
#endif


#if GMD_MAP_FAR_TEST_MPDATA
// ==========================================================================
//MPƒf[ƒ^ŠÇ—
// ==========================================================================

// ==========================================================================
// gmMapFarMpBuild
/*!
 * MPƒf[ƒ^\’z
 *
 * @param model_amb_index ƒ‚ƒfƒ‹ambƒwƒbƒ_‚ÌƒCƒ“ƒfƒNƒX
 * @param model_amb_index ƒ}ƒbƒvƒZƒbƒgambƒwƒbƒ_‚ÌƒCƒ“ƒfƒNƒX
 * @param mp_header_index MPƒwƒbƒ_‚ÌƒCƒ“ƒfƒNƒX
 * @param md_header_index MDƒwƒbƒ_‚ÌƒCƒ“ƒfƒNƒX
 *
 */
// ==========================================================================
void gmMapFarMpBuild(
					 u32 model_amb_index,
					 u32 mapset_amb_index,
					 u32 mp_header_index,
					 u32 md_header_index )
{

	//ƒf[ƒ^æ“¾
	AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
	amAssert(amb_header);

	AMS_AMB_HEADER* mapset_amb = (AMS_AMB_HEADER*)amBindGet(amb_header,  (s32)mapset_amb_index);
	amBindConv((u8*)mapset_amb);
	gmMapFarDataSetMpHeader( (MP_HEADER*)amBindGet(mapset_amb,  (s32)mp_header_index) );
	gmMapFarDataSetMdHeader( (MD_HEADER*)amBindGet(mapset_amb, (s32)md_header_index) );

	//NNƒ[ƒN—ÌˆæŠm•Û
	AMS_AMB_HEADER* model_amb = (AMS_AMB_HEADER*)amBindGet(amb_header,  (s32)model_amb_index);
	gmMapFarDataAllocNNModelWork( model_amb->file_num );
	
	//ƒ}ƒbƒvƒuƒƒbƒN0‚Íƒuƒ‰ƒ“ƒN‚È‚Ì‚ÅA1‚©‚ç
	gmMapFarDataIncNNModelRegistNum();
}

// ==========================================================================
// gmMapFarMpLoading
/*!
 * MPƒf[ƒ^“Ç‚İ‚İ’†ˆ—
 *
 * @param model_index ƒ‚ƒfƒ‹ambƒwƒbƒ_‚ÌƒCƒ“ƒfƒNƒX
 * @param texture_index ƒeƒNƒXƒ`ƒƒambƒwƒbƒ_‚ÌƒCƒ“ƒfƒNƒX
 *
 * @return TRUEF“Ç‚İ‚İI—¹ FALSEF“Ç‚İ‚İ’†
 */
// ==========================================================================
BOOL gmMapFarMpLoading( 
					   u32 model_index,
					   u32 texture_index )
{
	//ƒf[ƒ^æ“¾
	AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
	amAssert(amb_header);

	AMS_AMB_HEADER* model_amb = (AMS_AMB_HEADER*)amBindGet(amb_header,  (s32)model_index);
	amAssert(model_amb);
	AMS_AMB_HEADER* texture_amb = (AMS_AMB_HEADER*)amBindGet(amb_header,  (s32)texture_index);
	amAssert(texture_amb);

	s32 model_num = gmMapFarDataGetNNModelWorkNum();
	s32 regist_num = gmMapFarDataGetNNModelRegistNum();

	//“o˜^Ï‚İƒ`ƒFƒbƒN
	if ( regist_num < model_num ){
		for ( s32 i = regist_num; model_num > i; ++i ){
			//ƒfƒBƒXƒvƒŒƒCƒŠƒXƒgƒ`ƒFƒbƒN
			if ( !gmMapFarCheckDisplayList() ){
				return FALSE;
			}
			//“Ç‚İ‚İ—v‹
			OBS_ACTION3D_NN_WORK* work = gmMapFarDataLoadNNModel( i, model_amb, texture_amb );
			work->command_state = OBD_DRAW_CMD_STATE_PRE_MAPFAR;
		}
	}

	//“Ç‚İ‚İÏ‚İƒ`ƒFƒbƒN
	if ( gmMapFarDataCheckNNModelLoad() ){
		return FALSE;
	}

	//“o˜^I—¹
	return TRUE;
}

// ==========================================================================
// gmMapFarMpDraw
/*!
 * MPƒf[ƒ^•`‰æ
 *
 */
// ==========================================================================
void gmMapFarMpDraw( void )
{
	//ƒJƒƒ‰æ“¾
	GMS_MAP_FAR_CAMERA* camera_info = gmMapFarCameraGetInfo();
	amAssert( camera_info );
	OBS_CAMERA* camera = ObjCameraGet(camera_info->camera_id);

	float pos_x = camera->disp_pos.x;
	float pos_y = -camera->disp_pos.y;

	GmMapDrawMap(
			gmMapFarDataGetNNModelWorkList(),
			gmMapFarDataGetMpHeader(),
			gmMapFarDataGetMdHeader(),
			pos_x, pos_y,
			0, 0, GMD_MAP_FAR_MP_DRAW_POS_Z);			
}
#endif //GMD_MAP_FAR_TEST_MPDATA

// ==========================================================================
//	ƒ][ƒ“1
// ==========================================================================

// ==========================================================================
// gmMapFarZone1Load
/*!
 * ‰“Œiƒf[ƒ^“Ç‚İ‚İ
 */
// ==========================================================================
void gmMapFarZone1Build( void )
{

#if GMD_MAP_FAR_TEST_MPDATA
	//MPƒf[ƒ^
	gmMapFarMpBuild( 
			GMD_MAP_FAR_DATA_INDEX_ZONE_1_NN_MODEL_AMB_HEADER, 
			GMD_MAP_FAR_DATA_INDEX_ZONE_1_NN_MAPSET_AMB_HEADER, 
			(GMD_MAP_FAR_DATA_INDEX)GMD_MAP_FAR_DATA_INDEX_ZONE_1_MP_HEADER, 
			(GMD_MAP_FAR_DATA_INDEX)GMD_MAP_FAR_DATA_INDEX_ZONE_1_MD_HEADER );
#endif //GMD_MAP_FAR_TEST_MPDATA

	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );

	//ƒf[ƒ^æ“¾
	AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
	amAssert( amb_header );
	AMS_AMB_HEADER* amb_model = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE1_MAPFAR_MAPFAR_ZONE1_MDL_AMB );
	amAssert( amb_model );
	AMS_AMB_HEADER* amb_texture = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE1_MAPFAR_MAPFAR_TEX_AMB );
	amAssert( amb_texture );
	data->obj_3d_list = GmGameDBuildRegBuildModel( amb_model, amb_texture, (NNF_DRAWOBJ)0 );
	amAssert( data->obj_3d_list );
#if GMD_MAP_FAR_ROCK_PRIMITIVE
	data->tvx_amb_header = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE1_MAPFAR_MAPFAR_ZONE1_TVX_AMB );
	data->tex_amb_header = amb_texture; 
	amAssert( data->tvx_amb_header );
	gmMapFarZone1BuildRock();
#endif

	//ƒŒƒ“ƒ_ƒeƒNƒXƒ`ƒƒg—pƒ‚ƒfƒ‹İ’è
	NNF_DRAWOBJ drawflag = (NNF_DRAWOBJ)0;
#if (_PC | _XBOX | _PS3)
#if (GMD_MAP_FAR_SEAMODE)
	drawflag = NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1;
#endif
#endif
	AMS_AMB_HEADER* amb_model_render = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE1_MAPFAR_MAPFAR_ZONE1_RENDER_MDL_AMB );
	amAssert( amb_model_render );
	data->obj_3d_list_render = GmGameDBuildRegBuildModel( amb_model_render, amb_texture, drawflag );
	amAssert( data->obj_3d_list_render );

}

#if GMD_MAP_FAR_ROCK_PRIMITIVE
// ==========================================================================
// gmMapFarZone1BuildRock
/*!
 *  iPhone—p@Šâ•`‰æ€”õ
 */
// ==========================================================================
void gmMapFarZone1BuildRock( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();

	// æ“ª‚Étxb‚ª‚ ‚é‚±‚Æ‘O’ñ
	amBindConv( (Uint8*)data->tvx_amb_header);
	char* buf = (char*)amBindGet(data->tvx_amb_header, GMD_MAPFAR_TVXAMB_TXB);
	amTxbConv((Uint8*)buf);
	data->tex_buf = amTxbGetTexFileList(buf);

	data->texlistbuf = amMemAlloc((Uint32)nnEstimateTexlistSize(data->tex_buf->nTex));
	nnSetUpTexlist(&data->texlist, data->tex_buf->nTex, data->texlistbuf);

	amBindConv( (Uint8*)data->tex_amb_header);
	data->regId = amTextureLoad(data->texlist, data->tex_buf, NULL, data->tex_amb_header);

	data->tvx_rockA = (char*)amBindGet(data->tvx_amb_header, GMD_MAPFAR_TVXAMB_ROCKA);
	data->tvx_rockB = (char*)amBindGet(data->tvx_amb_header, GMD_MAPFAR_TVXAMB_ROCKB);
	data->tvx_rockC = (char*)amBindGet(data->tvx_amb_header, GMD_MAPFAR_TVXAMB_ROCKC);
}
#endif

// ==========================================================================
// GmMapFarZone1CheckLoading
/*!
 * “Ç‚İ‚İ‘Ò‚¿ƒ`ƒFƒbƒN
 *
 * @return TRUEF“Ç‚İ‚İI—¹ FALSEF“Ç‚İ‚İ‘Ò‚¿
 *
 */
// ==========================================================================
BOOL gmMapFarZone1CheckLoading( void )
{
	BOOL finish_flag = TRUE;

#if GMD_MAP_FAR_TEST_MPDATA
	//MPƒf[ƒ^
	finish_flag &= gmMapFarMpLoading(
			GMD_MAP_FAR_DATA_INDEX_ZONE_1_NN_MODEL_AMB_HEADER,
			GMD_MAP_FAR_DATA_INDEX_ZONE_1_NN_TEXTURE_AMB_HEADER);
#endif //GMD_MAP_FAR_TEST_MPDATA
	return finish_flag;
}

// ==========================================================================
// gmMapFarZone1Flush
/*!
 * ‰“Œiƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
void gmMapFarZone1Flush( void )
{

	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );

	//ƒf[ƒ^‰ğ•ú
	AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
	amAssert( amb_header );
	AMS_AMB_HEADER* model_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE1_MAPFAR_MAPFAR_ZONE1_MDL_AMB );
	amAssert( model_amb );
	GmGameDBuildRegFlushModel( data->obj_3d_list, model_amb->file_num );
	data->obj_3d_list = NULL;

	//ƒŒƒ“ƒ_ƒeƒNƒXƒ`ƒƒg—pƒ‚ƒfƒ‹‰ğ•ú
	AMS_AMB_HEADER* amb_model_render = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE1_MAPFAR_MAPFAR_ZONE1_RENDER_MDL_AMB );
	amAssert( amb_model_render );
	GmGameDBuildRegFlushModel( data->obj_3d_list_render, amb_model_render->file_num );
	data->obj_3d_list_render = NULL;

}

// ==========================================================================
// gmMapFarZone1Init
/*!
 * ‰“Œi‰Šú‰»
 */
// ==========================================================================
void gmMapFarZone1Init( void )
{
	const GMS_MAP_FAR_SCROLL* scroll_list_x = NULL;
	const GMS_MAP_FAR_SCROLL* scroll_list_y = NULL;
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	s32 scroll_list_num_x = 0;
	s32 scroll_list_num_y = 0;

	//•`‰æƒvƒƒV[ƒWƒƒ‚ğİ’è
	gmMapFarChangeTcbProcDraw( gmMpaFarZone1TcbProcDraw );
	
	//ƒJƒƒ‰
	gmMapFarCameraSetInfo( GME_CAMERA_NO_FAR, NNE_PROJECTION_TYPE_PERSPECTIVE );

#if GMD_MAP_FAR_TEST_MPDATA

	//ƒXƒNƒ[ƒ‹‹——£
	const MP_HEADER* mapset_far = gmMapFarDataGetMpHeader();
	amAssert( mapset_far );
	amAssert( mapset_far->map_w > 0 );
	amAssert( mapset_far->map_h > 0 );

	GMS_MAP_FAR_SCROLL scroll_x = {0,mapset_far->map_w*64,1};
	GMS_MAP_FAR_SCROLL scroll_y = {0,mapset_far->map_h*64,1};
	scroll_list_x = &scroll_x;
	scroll_list_y = &scroll_y;
	scroll_list_num_x = 1;
	scroll_list_num_y = 1;
#else

	//ƒXƒNƒ[ƒ‹‹——£
	if ( stage_id == GSD_MAIN_STAGE_ID_1_BOSS )
	{
		scroll_list_x = &g_map_far_zone_1_boss_scroll_x[0];
		scroll_list_num_x = sizeof(g_map_far_zone_1_boss_scroll_x)/sizeof(GMS_MAP_FAR_SCROLL);

		scroll_list_y = &g_map_far_zone_1_boss_scroll_y[0];
		scroll_list_num_y = sizeof(g_map_far_zone_1_boss_scroll_y)/sizeof(GMS_MAP_FAR_SCROLL);
	}
	else
	{	
		scroll_list_x = &g_map_far_zone_1_scroll_x[0];
		scroll_list_num_x = g_map_far_zone_1_scroll_num_x;

		scroll_list_y = &g_map_far_zone_1_scroll_y[0];
		scroll_list_num_y = g_map_far_zone_1_scroll_num_y;
	}

#endif	//GMD_MAP_FAR_TEST_MPDATA

	//ƒ}ƒbƒv”ÍˆÍ‚©‚ç‰“ŒiƒJƒƒ‰‚ÌƒXƒs[ƒh•â³’l‚ğZo
	const MP_HEADER* mapset_a = gmMapFarGetMapsetMpA();
	amAssert( mapset_a );
	amAssert( mapset_a->map_w > 0 );
	amAssert( mapset_a->map_h > 0 );

	s32 scroll_distance_x = gmMapFarCameraGetScrollDistance( 
			scroll_list_x, 
			(u32)scroll_list_num_x );

	s32 scroll_distance_y = gmMapFarCameraGetScrollDistance( 
			scroll_list_y, 
			(u32)scroll_list_num_y );

	float speed_x = (float)scroll_distance_x / (float)(mapset_a->map_w*64);
	float speed_y = (float)scroll_distance_y / (float)(mapset_a->map_h*64);
	gmMapFarCameraSetSpeed( speed_x, speed_y );


	//ƒIƒuƒWƒFƒNƒg
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );
	for ( s32 i = 0; GMD_MAP_FAR_OBJ_INDEX_ZONE_1_MAX > i; ++i ){

		//ƒ‚ƒfƒ‹ƒf[ƒ^‚ÌƒRƒs[Œ³‚ğæ“¾
		u32 model_index = g_map_far_zone_1_scene_obj_data[i];
		OBS_ACTION3D_NN_WORK* obj_3d_work = NULL;
		if ( i == GMD_MAP_FAR_OBJ_INDEX_ZONE_1_SEA ){
			obj_3d_work = &data->obj_3d_list_render[model_index];
		}
		else{
			obj_3d_work = &data->obj_3d_list[model_index];
		}
		amAssert( obj_3d_work );

		//•`‰æƒRƒ}ƒ“ƒhİ’è
		u32 command_state;
		if ( i >= GMD_MAP_FAR_OBJ_INDEX_ZONE_1_SEA)
		{
			command_state = OBD_DRAW_CMD_STATE_MAPFAR;
		}
		else
		{
			command_state = OBD_DRAW_CMD_STATE_PRE_MAPFAR;
		}

		//ƒ}ƒeƒŠƒAƒ‹
		u32 mat_motion_index = g_map_far_zone_1_scene_obj_data_mat_motion[i];
		AMS_AMB_HEADER* mat_amb = NULL;
		if ( GMD_MAP_FAR_DATA_INDEX_INVALID != mat_motion_index ){
			AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
			mat_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE1_MAPFAR_MAPFAR_ZONE1_MAT_AMB );
			amAssert( mat_amb );
		}

		//OBJ“Ç‚İ‚İ
		gmMapFarSceneLoadObj(
			(GMD_MAP_FAR_OBJ_INDEX)i,
			obj_3d_work,
			mat_motion_index,
			mat_amb,
			NULL,
			NULL,
			g_map_far_zone_1_scene_obj_func_main[i],
			g_map_far_zone_1_scene_obj_func_out[i],
			command_state
			);
	}


	//‹ó
	OBS_OBJECT_WORK* sky_work = gmMapFarDataGetObjWork( GMD_MAP_FAR_OBJ_INDEX_ZONE_1_SKY );
	amAssert( sky_work );
	sky_work->scale.y = 0x00002000L;

	//ŠC
	OBS_OBJECT_WORK* sea_work = gmMapFarDataGetObjWork( GMD_MAP_FAR_OBJ_INDEX_ZONE_1_SEA );
	amAssert( sea_work );
	sea_work->disp_flag |= OBD_DISP_REPEAT;
	ObjDrawObjectActionSet3DNNMaterial( sea_work, 0 );
	sea_work->obj_3d->mat_speed = 0.2f;

	// ¯•ÊID‚ğd‚Ş
	for ( s32 i = 0; GMD_MAP_FAR_OBJ_INDEX_ZONE_1_MAX > i; ++i)
	{
		OBS_OBJECT_WORK* work = gmMapFarDataGetObjWork( (GMD_MAP_FAR_OBJ_INDEX)i );
		work->user_work = (u32)i;
	}
}

// ==========================================================================
// gmMapFarZone1Release
/*!
 * ‰“ŒiI—¹ˆ—
 */
// ==========================================================================
void gmMapFarZone1Release( void )
{
#if GMD_MAP_FAR_ROCK_PRIMITIVE
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();

	if ( data->texlist )
	{
        data->regId = amTextureRelease(data->texlist);
		data->texlist = NULL;
	}

	if ( data->texlistbuf)
	{
        amMemFree(data->texlistbuf);
		data->texlistbuf = NULL;
	}
#endif
}

// ==========================================================================
// gmMpaFarZone1TcbProcDraw
/*!
 * ‰“Œi•`‰æƒvƒƒV[ƒWƒƒ
 */
// ==========================================================================
void gmMpaFarZone1TcbProcDraw( MTS_TASK_TCB* tcb )
{
	UNREFERENCED_PARAMETER( tcb );

#if GMD_MAP_FAR_TEST_MPDATA
	//•`‰æ
	gmMapFarMpDraw();
#endif	//GMD_MAP_FAR_TEST_MPDATA
}

// ==========================================================================
// gmMapFarZone1ScrollCamera
/*!
 * ’ÊíƒJƒƒ‰‚ÌÀ•W‚ğ“n‚µ‚ÄA‰“ŒiƒJƒƒ‰‚ÌÀ•W‚ğæ“¾
 *
 * @param map_far_camera_pos ‰“ŒiƒJƒƒ‰‚ÌÀ•W
 * @param player_camera_pos ’ÊíƒJƒƒ‰‚ÌÀ•W
 */
// ==========================================================================
NNS_VECTOR gmMapFarZone1GetCameraPos( const NNS_VECTOR* player_camera_pos )
{
	amAssert(player_camera_pos);

#if GMD_MAP_FAR_TEST_MPDATA

	//ƒXƒNƒ[ƒ‹‹——£
	const MP_HEADER* mapset_far = gmMapFarDataGetMpHeader();
	amAssert( mapset_far );
	amAssert( mapset_far->map_w > 0 );
	amAssert( mapset_far->map_h > 0 );

	GMS_MAP_FAR_SCROLL scroll_x = {0,mapset_far->map_w*64,1};
	GMS_MAP_FAR_SCROLL scroll_y = {0,mapset_far->map_h*64,1};

	NNS_VECTOR camera = gmMapFarCameraGetPos(
			player_camera_pos,
			&scroll_x,
			1,
			&scroll_y,
			1);
#else
	const MP_HEADER* mapset_a = gmMapFarGetMapsetMpA();
	amAssert( mapset_a );
	amAssert( mapset_a->map_h > 0 );

	NNS_VECTOR camera = *player_camera_pos;
	camera.y += mapset_a->map_h*64;

	camera = gmMapFarCameraGetPos(
			&camera,
			g_map_far_zone_1_scroll_x,
			g_map_far_zone_1_scroll_num_x,
			g_map_far_zone_1_scroll_y,
			g_map_far_zone_1_scroll_num_y);
		

	//‰¼‘Î‰iƒV[ƒ“ƒf[ƒ^‚Ì²‚ÌŒü‚«‚ªˆÙ‚È‚é‚½‚ßj
	float temp_z = camera.z;
	camera.z = -camera.x;
	camera.y = camera.y;
	camera.x = temp_z;
#endif	//GMD_MAP_FAR_TEST_MPDATA

	g_map_far_data->pos = camera;

	return camera;
}

// ==========================================================================
//	ƒ][ƒ“2
// ==========================================================================

// ==========================================================================
// gmMapFarZone2Load
/*!
 * ‰“Œiƒf[ƒ^“Ç‚İ‚İ
 */
// ==========================================================================
void gmMapFarZone2Build( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );

	//ƒf[ƒ^æ“¾
	AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
	amAssert( amb_header );
	AMS_AMB_HEADER* amb_model = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE2_MAPFAR_MAPFAR_ZONE2_MDL_AMB );
	amAssert( amb_model );
	AMS_AMB_HEADER* amb_texture = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE2_MAPFAR_MAPFAR_TEX_AMB );
	amAssert( amb_texture );
	data->obj_3d_list = GmGameDBuildRegBuildModel( amb_model, amb_texture, (NNF_DRAWOBJ)0 );
	amAssert( data->obj_3d_list );
}

// ==========================================================================
// GmMapFarZone2CheckLoading
/*!
 * “Ç‚İ‚İ‘Ò‚¿ƒ`ƒFƒbƒN
 *
 * @return TRUEF“Ç‚İ‚İI—¹ FALSEF“Ç‚İ‚İ‘Ò‚¿
 *
 */
// ==========================================================================
BOOL gmMapFarZone2CheckLoading( void )
{
	BOOL finish_flag = TRUE;

	return finish_flag;
}

// ==========================================================================
// gmMapFarZone2Flush
/*!
 * ‰“Œiƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
void gmMapFarZone2Flush( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );

	//ƒf[ƒ^‰ğ•ú
	AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
	amAssert( amb_header );
	AMS_AMB_HEADER* model_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE2_MAPFAR_MAPFAR_ZONE2_MDL_AMB );
	amAssert( model_amb );
	GmGameDBuildRegFlushModel( data->obj_3d_list, model_amb->file_num );
	data->obj_3d_list = NULL;
}

// ==========================================================================
// gmMapFarZone2Init
/*!
 * ‰“Œi‰Šú‰»
 */
// ==========================================================================
void gmMapFarZone2Init( void )
{
	const GMS_MAP_FAR_SCROLL* scroll_list_x = NULL;
	const GMS_MAP_FAR_SCROLL* scroll_list_y = NULL;
	GSE_MAIN_STAGE_ID stage_id = gmMapFarGetStageId();
	s32 scroll_list_num_x = 0;
	s32 scroll_list_num_y = 0;

	//•`‰æƒvƒƒV[ƒWƒƒ‚ğİ’è
	gmMapFarChangeTcbProcDraw( gmMpaFarZone2TcbProcDraw );
	
	//ƒJƒƒ‰
	gmMapFarCameraSetInfo( GME_CAMERA_NO_FAR, NNE_PROJECTION_TYPE_PERSPECTIVE );

	//ƒXƒNƒ[ƒ‹‹——£
	if ( stage_id == GSD_MAIN_STAGE_ID_2_BOSS )
	{
		scroll_list_x = &g_map_far_zone_2_boss_scroll_x[0];
		scroll_list_num_x = sizeof(g_map_far_zone_2_boss_scroll_x)/sizeof(GMS_MAP_FAR_SCROLL);

		scroll_list_y = &g_map_far_zone_2_boss_scroll_y[0];
		scroll_list_num_y = sizeof(g_map_far_zone_2_boss_scroll_y)/sizeof(GMS_MAP_FAR_SCROLL);
	}
	else
	{	
		scroll_list_x = &g_map_far_zone_2_scroll_x[0];
		scroll_list_num_x = g_map_far_zone_2_scroll_num_x;

		scroll_list_y = &g_map_far_zone_2_scroll_y[0];
		scroll_list_num_y = g_map_far_zone_2_scroll_num_y;
	}

	//ƒ}ƒbƒv”ÍˆÍ‚©‚ç‰“ŒiƒJƒƒ‰‚ÌƒXƒs[ƒh•â³’l‚ğZo
	const MP_HEADER* mapset_a = gmMapFarGetMapsetMpA();
	amAssert( mapset_a );
	amAssert( mapset_a->map_w > 0 );
	amAssert( mapset_a->map_h > 0 );

	s32 scroll_distance_x = gmMapFarCameraGetScrollDistance( 
			scroll_list_x, 
			(u32)scroll_list_num_x );

	s32 scroll_distance_y = gmMapFarCameraGetScrollDistance( 
			scroll_list_y, 
			(u32)scroll_list_num_y );

	float speed_x = (float)scroll_distance_x / (float)(mapset_a->map_w*64);
	float speed_y = (float)scroll_distance_y / (float)(mapset_a->map_h*64);
	gmMapFarCameraSetSpeed( speed_x, speed_y );


	//ƒIƒuƒWƒFƒNƒg
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );
	for ( s32 i = 0; GMD_MAP_FAR_OBJ_INDEX_ZONE_2_MAX > i; ++i ){

		//ƒ‚ƒfƒ‹ƒf[ƒ^‚ÌƒRƒs[Œ³‚ğæ“¾
		u32 model_index = g_map_far_zone_2_scene_obj_data[i];
		OBS_ACTION3D_NN_WORK* obj_3d_work = NULL;
		obj_3d_work = &data->obj_3d_list[model_index];
		amAssert( obj_3d_work );

		//•`‰æƒRƒ}ƒ“ƒhİ’è
		u32 command_state;
		command_state = OBD_DRAW_CMD_STATE_PRE_MAPFAR;

		// ƒ‚[ƒVƒ‡ƒ“
		u32 motion_index = g_map_far_zone_2_scene_obj_data_motion[i];
		AMS_AMB_HEADER* mtn_amb = NULL;
		if ( GMD_MAP_FAR_DATA_INDEX_INVALID != motion_index ){
			AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
			mtn_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE2_MAPFAR_MAPFAR_ZONE2_MTN_AMB );
			amAssert( mtn_amb );
		}

		//ƒ}ƒeƒŠƒAƒ‹
		u32 mat_motion_index = g_map_far_zone_2_scene_obj_data_mat_motion[i];
		AMS_AMB_HEADER* mat_amb = NULL;
		if ( GMD_MAP_FAR_DATA_INDEX_INVALID != mat_motion_index ){
			AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
			mat_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE2_MAPFAR_MAPFAR_ZONE2_MAT_AMB );
			amAssert( mat_amb );
		}

		//OBJ“Ç‚İ‚İ
		gmMapFarSceneLoadObj(
			(GMD_MAP_FAR_OBJ_INDEX)i,
			obj_3d_work,
			mat_motion_index,
			mat_amb,
			motion_index,
			mtn_amb,
			g_map_far_zone_2_scene_obj_func_main[i],
			g_map_far_zone_2_scene_obj_func_out[i],
			command_state
			);
	}

#if !_IPHONE
	//‹ó
	OBS_OBJECT_WORK* sky_work = gmMapFarDataGetObjWork( GMD_MAP_FAR_OBJ_INDEX_ZONE_2_SKY );
	amAssert( sky_work );
	sky_work->scale.y = 2 * FX32_ONE;
	sky_work->scale.x = 2 * FX32_ONE;
	sky_work->scale.z = 2 * FX32_ONE;
#endif

	// ¯•ÊID‚ğd‚Ş
	for ( s32 i = 0; GMD_MAP_FAR_OBJ_INDEX_ZONE_2_MAX > i; ++i)
	{
		OBS_OBJECT_WORK* work = gmMapFarDataGetObjWork( (GMD_MAP_FAR_OBJ_INDEX)i );
		work->user_work = (u32)i;
	}
}

// ==========================================================================
// gmMapFarZone2Release
/*!
 * ‰“ŒiI—¹ˆ—
 */
// ==========================================================================
void gmMapFarZone2Release( void )
{
}

// ==========================================================================
// gmMpaFarZone2TcbProcDraw
/*!
 * ‰“Œi•`‰æƒvƒƒV[ƒWƒƒ
 */
// ==========================================================================
void gmMpaFarZone2TcbProcDraw( MTS_TASK_TCB* tcb )
{
	UNREFERENCED_PARAMETER( tcb );
}

// ==========================================================================
// gmMapFarZone2ScrollCamera
/*!
 * ’ÊíƒJƒƒ‰‚ÌÀ•W‚ğ“n‚µ‚ÄA‰“ŒiƒJƒƒ‰‚ÌÀ•W‚ğæ“¾
 *
 * @param map_far_camera_pos ‰“ŒiƒJƒƒ‰‚ÌÀ•W
 * @param player_camera_pos ’ÊíƒJƒƒ‰‚ÌÀ•W
 */
// ==========================================================================
NNS_VECTOR gmMapFarZone2GetCameraPos( const NNS_VECTOR* player_camera_pos )
{
	amAssert(player_camera_pos);

	const MP_HEADER* mapset_a = gmMapFarGetMapsetMpA();
	amAssert( mapset_a );
	amAssert( mapset_a->map_h > 0 );

	NNS_VECTOR camera = *player_camera_pos;
	camera.y += mapset_a->map_h*64;

	camera = gmMapFarCameraGetPos(
			&camera,
			g_map_far_zone_2_scroll_x,
			g_map_far_zone_2_scroll_num_x,
			g_map_far_zone_2_scroll_y,
			g_map_far_zone_2_scroll_num_y);
		

	//‰¼‘Î‰iƒV[ƒ“ƒf[ƒ^‚Ì²‚ÌŒü‚«‚ªˆÙ‚È‚é‚½‚ßj
	float temp_z = camera.z;
	camera.z = -camera.x;
	camera.y = camera.y;
	camera.x = temp_z;

	g_map_far_data->pos = camera;

	return camera;
}

// ==========================================================================
//	ƒ][ƒ“3
// ==========================================================================

// ==========================================================================
// gmMapFarZone3Load
/*!
 * ‰“Œiƒf[ƒ^“Ç‚İ‚İ
 */
// ==========================================================================
void gmMapFarZone3Build( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );

	//ƒf[ƒ^æ“¾
	AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
	amAssert( amb_header );
	AMS_AMB_HEADER* amb_model = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE3_MAPFAR_MAPFAR_ZONE3_MDL_AMB );
	amAssert( amb_model );
	AMS_AMB_HEADER* amb_texture = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE3_MAPFAR_MAPFAR_TEX_AMB );
	amAssert( amb_texture );
	data->obj_3d_list = GmGameDBuildRegBuildModel( amb_model, amb_texture, (NNF_DRAWOBJ)0 );
	amAssert( data->obj_3d_list );
}

// ==========================================================================
// GmMapFarZone3CheckLoading
/*!
 * “Ç‚İ‚İ‘Ò‚¿ƒ`ƒFƒbƒN
 *
 * @return TRUEF“Ç‚İ‚İI—¹ FALSEF“Ç‚İ‚İ‘Ò‚¿
 *
 */
// ==========================================================================
BOOL gmMapFarZone3CheckLoading( void )
{
	BOOL finish_flag = TRUE;
	return finish_flag;
}

// ==========================================================================
// gmMapFarZone3Flush
/*!
 * ‰“Œiƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
void gmMapFarZone3Flush( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );

	//ƒf[ƒ^‰ğ•ú
	AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
	amAssert( amb_header );
	AMS_AMB_HEADER* model_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE3_MAPFAR_MAPFAR_ZONE3_MDL_AMB );
	amAssert( model_amb );
	GmGameDBuildRegFlushModel( data->obj_3d_list, model_amb->file_num );
	data->obj_3d_list = NULL;
}

// ==========================================================================
// gmMapFarZone3Init
/*!
 * ‰“Œi‰Šú‰»
 */
// ==========================================================================
void gmMapFarZone3Init( void )
{
	const GMS_MAP_FAR_SCROLL* scroll_list_x = NULL;
	const GMS_MAP_FAR_SCROLL* scroll_list_y = NULL;
	s32 scroll_list_num_x = 0;
	s32 scroll_list_num_y = 0;

	//•`‰æƒvƒƒV[ƒWƒƒ‚ğİ’è
	gmMapFarChangeTcbProcDraw( gmMpaFarZone3TcbProcDraw );
	
	//ƒJƒƒ‰
	gmMapFarCameraSetInfo( GME_CAMERA_NO_FAR, NNE_PROJECTION_TYPE_PERSPECTIVE );

	//ƒXƒNƒ[ƒ‹‹——£
	scroll_list_x = &g_map_far_zone_3_scroll_x[0];
	scroll_list_num_x = g_map_far_zone_3_scroll_num_x;

	scroll_list_y = &g_map_far_zone_3_scroll_y[0];
	scroll_list_num_y = g_map_far_zone_3_scroll_num_y;

	//ƒ}ƒbƒv”ÍˆÍ‚©‚ç‰“ŒiƒJƒƒ‰‚ÌƒXƒs[ƒh•â³’l‚ğZo
	const MP_HEADER* mapset_a = gmMapFarGetMapsetMpA();
	amAssert( mapset_a );
	amAssert( mapset_a->map_w > 0 );
	amAssert( mapset_a->map_h > 0 );

	s32 scroll_distance_x = gmMapFarCameraGetScrollDistance( 
			scroll_list_x, 
			(u32)scroll_list_num_x );

	s32 scroll_distance_y = gmMapFarCameraGetScrollDistance( 
			scroll_list_y, 
			(u32)scroll_list_num_y );

	float speed_x = (float)scroll_distance_x / (float)(mapset_a->map_w*64);
	float speed_y = (float)scroll_distance_y / (float)(mapset_a->map_h*64);
	gmMapFarCameraSetSpeed( speed_x, speed_y );

	//ƒIƒuƒWƒFƒNƒg
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );
	for ( s32 i = 0; GMD_MAP_FAR_OBJ_INDEX_ZONE_3_MAX > i; ++i ){

		//ƒ‚ƒfƒ‹ƒf[ƒ^‚ÌƒRƒs[Œ³‚ğæ“¾
		u32 model_index = g_map_far_zone_3_scene_obj_data[i];
		OBS_ACTION3D_NN_WORK* obj_3d_work = &data->obj_3d_list[model_index];
		amAssert( obj_3d_work );

		//•`‰æƒRƒ}ƒ“ƒhİ’è
		u32 command_state;
		command_state = OBD_DRAW_CMD_STATE_MAPFAR;
//		command_state = OBD_DRAW_CMD_STATE_PRE_MAPFAR;

		//ƒ}ƒeƒŠƒAƒ‹
		u32 mat_motion_index = g_map_far_zone_3_scene_obj_data_mat_motion[i];
		AMS_AMB_HEADER* mat_amb = NULL;
		if ( GMD_MAP_FAR_DATA_INDEX_INVALID != mat_motion_index ){
			AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
			mat_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONE3_MAPFAR_MAPFAR_ZONE3_MAT_AMB );
			amAssert( mat_amb );
		}

		//OBJ“Ç‚İ‚İ
		gmMapFarSceneLoadObj(
			(GMD_MAP_FAR_OBJ_INDEX)i,
			obj_3d_work,
			mat_motion_index,
			mat_amb,
			NULL,
			NULL,
			g_map_far_zone_3_scene_obj_func_main[i],
			g_map_far_zone_3_scene_obj_func_out[i],
			command_state);
	}

	//”wŒi
/*	OBS_OBJECT_WORK* work = gmMapFarDataGetObjWork(GMD_MAP_FAR_OBJ_INDEX_ZONE_3_ITA);
	amAssert( work );
	work->scale.x = 2 * FX32_ONE;
	work->scale.y = 2 * FX32_ONE;
*/
	// ¯•ÊID‚ğd‚Ş
	for ( s32 i = 0; GMD_MAP_FAR_OBJ_INDEX_ZONE_3_MAX > i; ++i)
	{
		OBS_OBJECT_WORK* work = gmMapFarDataGetObjWork( (GMD_MAP_FAR_OBJ_INDEX)i );
		work->user_work = (u32)i;
	}
}

// ==========================================================================
// gmMapFarZone3Release
/*!
 * ‰“ŒiI—¹ˆ—
 */
// ==========================================================================
void gmMapFarZone3Release( void )
{
}

// ==========================================================================
// gmMpaFarZone3TcbProcDraw
/*!
 * ‰“Œi•`‰æƒvƒƒV[ƒWƒƒ
 */
// ==========================================================================
void gmMpaFarZone3TcbProcDraw( MTS_TASK_TCB* tcb )
{
	UNREFERENCED_PARAMETER( tcb );
}

// ==========================================================================
// gmMapFarZone3GetCameraPos
/*!
 * ’ÊíƒJƒƒ‰‚ÌÀ•W‚ğ“n‚µ‚ÄA‰“ŒiƒJƒƒ‰‚ÌÀ•W‚ğæ“¾
 *
 * @param map_far_camera_pos ‰“ŒiƒJƒƒ‰‚ÌÀ•W
 * @param player_camera_pos ’ÊíƒJƒƒ‰‚ÌÀ•W
 */
// ==========================================================================
NNS_VECTOR gmMapFarZone3GetCameraPos( const NNS_VECTOR* player_camera_pos )
{	
	amAssert(player_camera_pos);

	const MP_HEADER* mapset_a = gmMapFarGetMapsetMpA();
	amAssert( mapset_a );
	amAssert( mapset_a->map_h > 0 );

	NNS_VECTOR camera = *player_camera_pos;
	camera.y += mapset_a->map_h*64;

	camera = gmMapFarCameraGetPos(
			&camera,
			g_map_far_zone_3_scroll_x,
			g_map_far_zone_3_scroll_num_x,
			g_map_far_zone_3_scroll_y,
			g_map_far_zone_3_scroll_num_y);
		

	//‰¼‘Î‰iƒV[ƒ“ƒf[ƒ^‚Ì²‚ÌŒü‚«‚ªˆÙ‚È‚é‚½‚ßj
	float temp_z = camera.z;
	camera.z = -camera.x;
	camera.y = camera.y;
	camera.x = temp_z;

	g_map_far_data->pos = camera;

	return camera;
}

// ==========================================================================
//	ƒ][ƒ“4
// ==========================================================================

// ==========================================================================
// gmMapFarZone4Load
/*!
 * ‰“Œiƒf[ƒ^“Ç‚İ‚İ
 */
// ==========================================================================
void gmMapFarZone4Build( void )
{
	
}

// ==========================================================================
// GmMapFarZone4CheckLoading
/*!
 * “Ç‚İ‚İ‘Ò‚¿ƒ`ƒFƒbƒN
 *
 * @return TRUEF“Ç‚İ‚İI—¹ FALSEF“Ç‚İ‚İ‘Ò‚¿
 *
 */
// ==========================================================================
BOOL gmMapFarZone4CheckLoading( void )
{
	return TRUE;
}

// ==========================================================================
// gmMapFarZone4Flush
/*!
 * ‰“Œiƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
void gmMapFarZone4Flush( void )
{
}

// ==========================================================================
// gmMapFarZone4Init
/*!
 * ‰“Œi‰Šú‰»
 */
// ==========================================================================
void gmMapFarZone4Init( void )
{
	//•`‰æƒvƒƒV[ƒWƒƒ‚ğİ’è
	gmMapFarChangeTcbProcDraw( gmMpaFarZone4TcbProcDraw );
}

// ==========================================================================
// gmMapFarZone4Release
/*!
 * ‰“ŒiI—¹ˆ—
 */
// ==========================================================================
void gmMapFarZone4Release( void )
{
}

// ==========================================================================
// gmMpaFarZone4TcbProcDraw
/*!
 * ‰“Œi•`‰æƒvƒƒV[ƒWƒƒ
 */
// ==========================================================================
void gmMpaFarZone4TcbProcDraw( MTS_TASK_TCB* tcb )
{
	UNREFERENCED_PARAMETER( tcb );
}

// ==========================================================================
//	ƒ][ƒ“FINAL
// ==========================================================================

// ==========================================================================
// gmMapFarZoneFinalLoad
/*!
 * ‰“Œiƒf[ƒ^“Ç‚İ‚İ
 */
// ==========================================================================
void gmMapFarZoneFinalBuild( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );

	//ƒf[ƒ^æ“¾
	AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
	amAssert( amb_header );
	AMS_AMB_HEADER* amb_model = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONEF_MAPFAR_MAPFAR_ZONEF_MDL_AMB );
	amAssert( amb_model );
	AMS_AMB_HEADER* amb_texture = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONEF_MAPFAR_MAPFAR_TEX_AMB );
	amAssert( amb_texture );
	data->obj_3d_list = GmGameDBuildRegBuildModel( amb_model, amb_texture, (NNF_DRAWOBJ)0 );
	amAssert( data->obj_3d_list );
	
}

// ==========================================================================
// GmMapFarZoneFinalCheckLoading
/*!
 * “Ç‚İ‚İ‘Ò‚¿ƒ`ƒFƒbƒN
 *
 * @return TRUEF“Ç‚İ‚İI—¹ FALSEF“Ç‚İ‚İ‘Ò‚¿
 *
 */
// ==========================================================================
BOOL gmMapFarZoneFinalCheckLoading( void )
{
	return TRUE;
}

// ==========================================================================
// gmMapFarZoneFinalFlush
/*!
 * ‰“Œiƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
void gmMapFarZoneFinalFlush( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );

	//ƒf[ƒ^‰ğ•ú
	AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
	amAssert( amb_header );
	AMS_AMB_HEADER* model_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_ZONEF_MAPFAR_MAPFAR_ZONEF_MDL_AMB );
	amAssert( model_amb );
	GmGameDBuildRegFlushModel( data->obj_3d_list, model_amb->file_num );
	data->obj_3d_list = NULL;
}

// ==========================================================================
// gmMapFarZoneFinalInit
/*!
 * ‰“Œi‰Šú‰»
 */
// ==========================================================================
void gmMapFarZoneFinalInit( void )
{
	const GMS_MAP_FAR_SCROLL* scroll_list_x = NULL;
	const GMS_MAP_FAR_SCROLL* scroll_list_y = NULL;
	s32 scroll_list_num_x = 0;
	s32 scroll_list_num_y = 0;

	//•`‰æƒvƒƒV[ƒWƒƒ‚ğİ’è
	gmMapFarChangeTcbProcDraw( gmMpaFarZoneFinalTcbProcDraw );
	
	//ƒJƒƒ‰
	gmMapFarCameraSetInfo( GME_CAMERA_NO_FAR, NNE_PROJECTION_TYPE_PERSPECTIVE );

	//ƒXƒNƒ[ƒ‹‹——£
	scroll_list_x = &g_map_far_zone_final_scroll_x[0];
	scroll_list_num_x = g_map_far_zone_final_scroll_num_x;

	scroll_list_y = &g_map_far_zone_final_scroll_y[0];
	scroll_list_num_y = g_map_far_zone_final_scroll_num_y;

	//ƒ}ƒbƒv”ÍˆÍ‚©‚ç‰“ŒiƒJƒƒ‰‚ÌƒXƒs[ƒh•â³’l‚ğZo
	const MP_HEADER* mapset_a = gmMapFarGetMapsetMpA();
	amAssert( mapset_a );
	amAssert( mapset_a->map_w > 0 );
	amAssert( mapset_a->map_h > 0 );

	s32 scroll_distance_x = gmMapFarCameraGetScrollDistance( 
			scroll_list_x, 
			(u32)scroll_list_num_x );

	s32 scroll_distance_y = gmMapFarCameraGetScrollDistance( 
			scroll_list_y, 
			(u32)scroll_list_num_y );

	float speed_x = (float)scroll_distance_x / (float)(mapset_a->map_w*64);
	float speed_y = (float)scroll_distance_y / (float)(mapset_a->map_h*64);
	gmMapFarCameraSetSpeed( speed_x, speed_y );

	//ƒIƒuƒWƒFƒNƒg
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );
	for ( s32 i = 0; GMD_MAP_FAR_OBJ_INDEX_ZONE_FINAL_MAX > i; ++i ){

		//ƒ‚ƒfƒ‹ƒf[ƒ^‚ÌƒRƒs[Œ³‚ğæ“¾
		u32 model_index = g_map_far_zone_final_scene_obj_data[i];
		OBS_ACTION3D_NN_WORK* obj_3d_work = &data->obj_3d_list[model_index];
		amAssert( obj_3d_work );

		//OBJ“Ç‚İ‚İ
		gmMapFarSceneLoadObj(
			(GMD_MAP_FAR_OBJ_INDEX)i,
			obj_3d_work,
			GMD_MAP_FAR_DATA_INDEX_INVALID,
			NULL,
			NULL,
			NULL,
			g_map_far_zone_final_scene_obj_func_main[i],
			g_map_far_zone_final_scene_obj_func_out[i],
			OBD_DRAW_CMD_STATE_MAPFAR);
	}

	// ¯•ÊID‚ğd‚Şi’n‹…‚Æ‰_‚Ì‰ñ“]‘¬“x‚ğ•Ï‚¦‚é‚½‚ß‚Éj
	for ( s32 i = 0; GMD_MAP_FAR_OBJ_INDEX_ZONE_FINAL_MAX > i; ++i)
	{
		OBS_OBJECT_WORK* work = gmMapFarDataGetObjWork( (GMD_MAP_FAR_OBJ_INDEX)i );
		work->user_work = (u32)i;
	}
}

// ==========================================================================
// gmMapFarZoneFinalRelease
/*!
 * ‰“ŒiI—¹ˆ—
 */
// ==========================================================================
void gmMapFarZoneFinalRelease( void )
{
}

// ==========================================================================
// gmMpaFarZoneFinalTcbProcDraw
/*!
 * ‰“Œi•`‰æƒvƒƒV[ƒWƒƒ
 */
// ==========================================================================
void gmMpaFarZoneFinalTcbProcDraw( MTS_TASK_TCB* tcb )
{
	UNREFERENCED_PARAMETER( tcb );
}

// ==========================================================================
// gmMapFarZone3GetCameraPos
/*!
 * ’ÊíƒJƒƒ‰‚ÌÀ•W‚ğ“n‚µ‚ÄA‰“ŒiƒJƒƒ‰‚ÌÀ•W‚ğæ“¾
 *
 * @param map_far_camera_pos ‰“ŒiƒJƒƒ‰‚ÌÀ•W
 * @param player_camera_pos ’ÊíƒJƒƒ‰‚ÌÀ•W
 */
// ==========================================================================
NNS_VECTOR gmMapFarZoneFinalGetCameraPos( const NNS_VECTOR* player_camera_pos )
{
	amAssert(player_camera_pos);

	const MP_HEADER* mapset_a = gmMapFarGetMapsetMpA();
	amAssert( mapset_a );
	amAssert( mapset_a->map_h > 0 );

	NNS_VECTOR camera = *player_camera_pos;
	camera.y += mapset_a->map_h*64;

	camera = gmMapFarCameraGetPos(
			&camera,
			g_map_far_zone_final_scroll_x,
			g_map_far_zone_final_scroll_num_x,
			g_map_far_zone_final_scroll_y,
			g_map_far_zone_final_scroll_num_y);
		
	//‰¼‘Î‰iƒV[ƒ“ƒf[ƒ^‚Ì²‚ÌŒü‚«‚ªˆÙ‚È‚é‚½‚ßj
	float temp_z = camera.z;
	camera.z = -camera.x;
	camera.y = camera.y;
	camera.x = temp_z;

	g_map_far_data->pos = camera;

	return camera;
}

// ==========================================================================
//	ƒXƒyƒXƒe
// ==========================================================================

// ==========================================================================
// gmMapFarZoneSSBuild
/*!
 * ‰“Œiƒf[ƒ^“Ç‚İ‚İ
 */
// ==========================================================================
void gmMapFarZoneSSBuild( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );

	//ƒf[ƒ^æ“¾
	AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
	amAssert( amb_header );
	AMS_AMB_HEADER* amb_model = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_SS01_MAPFAR_MAPFAR_SS01_MDL_AMB );
	amAssert( amb_model );
	AMS_AMB_HEADER* amb_texture = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_SS01_MAPFAR_MAPFAR_SS01_TEX_AMB );
	amAssert( amb_texture );
	data->obj_3d_list = GmGameDBuildRegBuildModel( amb_model, amb_texture, (NNF_DRAWOBJ)0 );
	amAssert( data->obj_3d_list );
}

// ==========================================================================
// GmMapFarZoneSSCheckLoading
/*!
 * “Ç‚İ‚İ‘Ò‚¿ƒ`ƒFƒbƒN
 *
 * @return TRUEF“Ç‚İ‚İI—¹ FALSEF“Ç‚İ‚İ‘Ò‚¿
 *
 */
// ==========================================================================
BOOL gmMapFarZoneSSCheckLoading( void )
{
	BOOL finish_flag = TRUE;

	return finish_flag;
}

// ==========================================================================
// gmMapFarZoneSSFlush
/*!
 * ‰“Œiƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
void gmMapFarZoneSSFlush( void )
{
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );

	//ƒf[ƒ^‰ğ•ú
	AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
	amAssert( amb_header );
	AMS_AMB_HEADER* model_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_SS01_MAPFAR_MAPFAR_SS01_MDL_AMB );
	amAssert( model_amb );
	GmGameDBuildRegFlushModel( data->obj_3d_list, model_amb->file_num );
	data->obj_3d_list = NULL;
}

// ==========================================================================
// gmMapFarZoneSSInit
/*!
 * ‰“Œi‰Šú‰»
 */
// ==========================================================================
void gmMapFarZoneSSInit( void )
{
	const GMS_MAP_FAR_SCROLL* scroll_list_x = NULL;
	const GMS_MAP_FAR_SCROLL* scroll_list_y = NULL;
	s32 scroll_list_num_x = 0;
	s32 scroll_list_num_y = 0;

	//•`‰æƒvƒƒV[ƒWƒƒ‚ğİ’è
	gmMapFarChangeTcbProcDraw( gmMpaFarZoneSSTcbProcDraw );
	
	//ƒJƒƒ‰
	gmMapFarCameraSetInfo( GME_CAMERA_NO_FAR, NNE_PROJECTION_TYPE_PERSPECTIVE );

	//ƒXƒNƒ[ƒ‹‹——£
	scroll_list_x = &g_map_far_zone_ss_scroll_x[0];
	scroll_list_num_x = g_map_far_zone_ss_scroll_num_x;

	scroll_list_y = &g_map_far_zone_ss_scroll_y[0];
	scroll_list_num_y = g_map_far_zone_ss_scroll_num_y;

	//ƒ}ƒbƒv”ÍˆÍ‚©‚ç‰“ŒiƒJƒƒ‰‚ÌƒXƒs[ƒh•â³’l‚ğZo
#if 1
	const MP_HEADER* mapset_a = gmMapFarGetMapsetMpA();
	amAssert( mapset_a );
	amAssert( mapset_a->map_w > 0 );
	amAssert( mapset_a->map_h > 0 );

	s32 scroll_distance_x = gmMapFarCameraGetScrollDistance( 
			scroll_list_x, 
			(u32)scroll_list_num_x );

	s32 scroll_distance_y = gmMapFarCameraGetScrollDistance( 
			scroll_list_y, 
			(u32)scroll_list_num_y );

	float speed_x = (float)scroll_distance_x / (float)(mapset_a->map_w*64);
	float speed_y = (float)scroll_distance_y / (float)(mapset_a->map_h*64);
	gmMapFarCameraSetSpeed( speed_x, speed_y );
#else
	gmMapFarCameraSetSpeed( 0.0f, 0.0f );
#endif
	//ƒIƒuƒWƒFƒNƒg
	GMS_MAP_FAR_DATA* data = gmMapFarDataGetInfo();
	amAssert( data );
	for ( s32 i = 0; GMD_MAP_FAR_OBJ_INDEX_ZONE_SS_MAX > i; ++i ){

		//ƒ‚ƒfƒ‹ƒf[ƒ^‚ÌƒRƒs[Œ³‚ğæ“¾
		u32 model_index = g_map_far_zone_ss_scene_obj_data[i];
		OBS_ACTION3D_NN_WORK* obj_3d_work = NULL;
		obj_3d_work = &data->obj_3d_list[model_index];
		amAssert( obj_3d_work );

		//ƒ}ƒeƒŠƒAƒ‹
		u32 mat_motion_index = g_map_far_zone_ss_scene_obj_data_mat_motion[i];
		AMS_AMB_HEADER* mat_amb = NULL;
		if ( GMD_MAP_FAR_DATA_INDEX_INVALID != mat_motion_index ){
			AMS_AMB_HEADER* amb_header = gmMapFarDataGetAmbHeader();
			mat_amb = (AMS_AMB_HEADER*)amBindGet( amb_header, IDB_SS01_MAPFAR_MAPFAR_SS01_MAT_AMB );
			amAssert( mat_amb );
		}

		//OBJ“Ç‚İ‚İ
		gmMapFarSceneLoadObj(
			(GMD_MAP_FAR_OBJ_INDEX)i,
			obj_3d_work,
			mat_motion_index,
			mat_amb,
			NULL,
			NULL,
			g_map_far_zone_ss_scene_obj_func_main[i],
			g_map_far_zone_ss_scene_obj_func_out[i],
			OBD_DRAW_CMD_STATE_PRE_MAPFAR
			);
	}
}

// ==========================================================================
// gmMapFarZoneSSRelease
/*!
 * ‰“ŒiI—¹ˆ—
 */
// ==========================================================================
void gmMapFarZoneSSRelease( void )
{
}

// ==========================================================================
// gmMpaFarZoneSSTcbProcDraw
/*!
 * ‰“Œi•`‰æƒvƒƒV[ƒWƒƒ
 */
// ==========================================================================
void gmMpaFarZoneSSTcbProcDraw( MTS_TASK_TCB* tcb )
{
	UNREFERENCED_PARAMETER( tcb );
}

// ==========================================================================
// gmMapFarZoneSSGetCameraPos
/*!
 * ’ÊíƒJƒƒ‰‚ÌÀ•W‚ğ“n‚µ‚ÄA‰“ŒiƒJƒƒ‰‚ÌÀ•W‚ğæ“¾
 *
 * @param map_far_camera_pos ‰“ŒiƒJƒƒ‰‚ÌÀ•W
 * @param player_camera_pos ’ÊíƒJƒƒ‰‚ÌÀ•W
 */
// ==========================================================================
NNS_VECTOR gmMapFarZoneSSGetCameraPos( const NNS_VECTOR* player_camera_pos )
{	
	amAssert(player_camera_pos);

	NNS_VECTOR camera = *player_camera_pos;

	camera.x = (float)(OBD_LCD_X);
	camera.y = (float)(OBD_LCD_Y);
	camera.z = (float)(50.0f);

	return camera;
}

// ==========================================================================
// GmMapFarStaticVarInit
/*!
 * static•Ï”‚Ì‰Šú‰»
 */
 // ==========================================================================
void GmMapFarStaticVarInit( void )
{
	//ƒf[ƒ^
	memset(&g_map_far_data_real, 0, sizeof(g_map_far_data_real));
	g_map_far_data = NULL;
	
	//ŠÇ—î•ñ
	memset(&g_map_far_mgr_real, 0, sizeof(g_map_far_mgr_real));
	g_map_far_mgr = NULL;
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
